#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <random>

#include "drc/streamer.h"
#include "drc/types.h"
#include "drc/pixel-format.h"
#include "drc/input.h"

#include "okayu.c"
#include "okayu.h"
#include "GamepadScreen.h"
#include "GamepadInput.h"
#include "CharacterControl.h"


using namespace std;

int main(int, char **) {


    GamepadScreen screen(854, 480);
    auto *streamer = new drc::Streamer();
    streamer->Start();

    int pixel_offset = (480 * 854 / 2) + 480 / 2;
    int scale = 10;

    uint32_t **okayu_pp_data;
    okayu_pp_data = new uint32_t *[32 * 32];
    for (int i = 0; i < 32 * 32; i++) {
        okayu_pp_data[i] = const_cast<uint32_t *>(okayu_data[i]);
    }


    Sprite okayu(okayu_pp_data, 32, 32, 0, 0, 5);

    cout << "making okayu\n";

    screen.draw(okayu);

    cout << "done making okayu. drawing line\n";

    screen.drawLine(0, 0, 800, 400, 5, 0xFF0000FF);

    cout << "done drawing line. drawing circle\n";

    screen.drawCircle(854 / 2, 480 / 2, 200, 5, 0xFF0000FF);

    cout << "done drawing line. drawing circle\n";

    auto input = new GamepadInput(streamer);

    auto characterControl = new CharacterControl(&okayu, input);

    // main loop
    bool running = true;
    int frameNum = 0;
    while (running) {
        // int random_pixel = (rand() % (854 * 480)) * 4;{
        int random_pixel = -1;
        /*
        vector<drc::byte> pixels_to_send(854 * 480 * 4, 1);
        for (int i = 0; i < 854 * 480 * 4; i++) {
            pixels_to_send[i] = screen.getPixels()->at(i);
        }*/
        input->updateStates();
        if (frameNum % 2 == 0) {
            characterControl->updatePosition();
        }

        screen.wipe();
        screen.draw(okayu);

        //cout << "Pixels: " << pixels.size() << ", random_pixel: " << random_pixel << "\n";
        streamer->PushVidFrame(screen.getPixels(), 854, 480, drc::PixelFormat::kRGBA,
                               drc::Streamer::FlippingMode::NoFlip,
                               drc::Streamer::StretchMode::NoStretch);
        frameNum = (frameNum + 1) % 2;
    }


    return 0;
}