#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <random>

#include "drc/streamer.h"
#include "drc/types.h"
#include "drc/pixel-format.h"
#include "drc/input.h"

#include "images/okayu.c"
#include "images/module.c"
#include "images/okayu.h"

#include "sprite_types/Character.h"
#include "sprite_types/Cable.h"

#include "GamepadScreen.h"
#include "GamepadInput.h"


using namespace std;

void drawRadialSelector(GamepadScreen *screen, int xc, int yc, int radius, int sectors, uint32_t color) {
    double smallRadius = radius * 0.2;
    //screen->drawArc(xc, yc, radius, 5, color, 0, 0, 0, 0);
    screen->drawCircle(xc, yc, radius, 5, color);
    screen->drawCircle(xc, yc, smallRadius, 5, color);

    double halfRadPerSector = M_PI / sectors;
    double radPerSector = 2 * halfRadPerSector;
    // double angle = (M_PI / 2) + halfRadPerSector;
    double angle = (M_PI / 2) + 0;
    for (int i = 0; i < sectors; i++) {
        double cosA = cos(angle);
        double sinA = sin(angle);
        int x = xc + radius * cosA;
        int y = yc + radius * sinA;
        int x2 = xc + smallRadius * cosA;
        int y2 = yc + smallRadius * sinA;
        screen->drawLine(x, y, x2, y2, 5, color);
        angle += radPerSector;
    }
}

int main(int, char **) {
    GamepadScreen screen(854, 480);
    auto *streamer = new drc::Streamer();
    streamer->Start();

    int pixel_offset = (480 * 854 / 2) + 480 / 2;
    int scale = 10;

    uint32_t **okayu_pp_data;
    okayu_pp_data = new uint32_t *[OKAYU_FRAME_WIDTH * OKAYU_FRAME_HEIGHT];
    for (int i = 0; i < OKAYU_FRAME_COUNT; i++) {
        okayu_pp_data[i] = const_cast<uint32_t *>(okayu_data[i]);
    }

    uint32_t **module_pp_data;
    module_pp_data = new uint32_t *[MODULE_FRAME_WIDTH * MODULE_FRAME_HEIGHT];
    for (int i = 0; i < MODULE_FRAME_COUNT; i++) {
        module_pp_data[i] = const_cast<uint32_t *>(module_data[i]);
    }


    auto input = new GamepadInput(streamer);

    Character okayu(okayu_pp_data, OKAYU_FRAME_WIDTH, OKAYU_FRAME_HEIGHT, input, 0, 0, 3);
    okayu.frameDelay = 2;
    okayu.z = 50;
    vector<Drawable *> spritesToDraw = {&okayu};

    Sprite module(module_pp_data, MODULE_FRAME_WIDTH, MODULE_FRAME_HEIGHT, 100, 100, 2);
    module.setFrame(2);
    module.setGripPoint(14, 31);
    module.addChildren(&spritesToDraw);

    auto *cable = new Cable();
    cable->addChildren(&spritesToDraw);

    vector<pair<Sprite *, int>> holdables = {{&module,          0},
                                             {cable->rightPlug, 0},
                                             {cable->leftPlug,  0}};
    okayu.setHoldables(&holdables);
    okayu.setGripPoint(15, 11);

    cout << "making okayu\n";

    okayu.draw(&screen);

    cout << "done making okayu. drawing line\n";

    screen.drawLine(0, 0, 800, 400, 5, 0xFF0000FF);

    cout << "done drawing line. drawing circle\n";

    screen.drawCircle(854 / 2, 480 / 2, 200, 5, 0xFF0000FF);

    cout << "done drawing line. drawing circle\n";

    cout << "drawables len: " << spritesToDraw.size() << endl;

    // main loop
    bool running = true;
    while (running) {
        // get inputs
        input->updateStates();

        // update holdables distance from okayu
        for (auto &spritePair: holdables) {
            spritePair.second = okayu.distForComp(spritePair.first);
        }

        // sort holdables by distance to okayu
        std::sort(holdables.begin(), holdables.end(), [](pair<Sprite *, int> a, pair<Sprite *, int> b) {
            return a.second < b.second;
        });
        okayu.updatePosition();

        // sort drawables by z pos
        std::sort(spritesToDraw.begin(), spritesToDraw.end(), [](Drawable *drawable1, Drawable *drawable2) {
            // todo if on same z, compare effective y positions (bc of perspective)
            return drawable1->z < drawable2->z;
        });

        // clear screen
        screen.wipe();
        // draw sprites in z order
        for (auto drawable: spritesToDraw) {
            drawable->draw(&screen);
        }

        // drawRadialSelector(&screen, 854 / 2, 480 / 2, 200, 5, 0xFF0000FF);

        streamer->PushVidFrame(screen.getPixels(), 854, 480, drc::PixelFormat::kRGBA,
                               drc::Streamer::FlippingMode::NoFlip,
                               drc::Streamer::StretchMode::NoStretch);
    }


    return 0;
}