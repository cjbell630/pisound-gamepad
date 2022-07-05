#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <random>

#include "drc/streamer.h"
#include "drc/types.h"
#include "drc/pixel-format.h"

#include "okayu.c"
#include "GamepadScreen.h"


using namespace std;

int main(int, char **) {


    GamepadScreen screen(854, 480);
    auto *streamer = new drc::Streamer();
    streamer->Start();

    int pixel_offset = (480 * 854 / 2) + 480 / 2;
    int scale = 10;


    Sprite<1024> okayu(okayu_data, 32, 32, 0, 0, 10);

    cout << "making okayu\n";

    screen.draw(okayu);

    cout << "done making okayu. drawing line\n";

    screen.drawLine(0, 0, 800, 400, 5, 0xFF0000FF);

    cout << "done drawing line. drawing circle\n";
    drc::byte colorBytes[4];
    uint32_t color = 0xFF0000FF;
    for (unsigned char &colorByte: colorBytes) {
        colorByte = color & 0xFF;
        color = color >> 8;
    }

    screen.circle2(854 / 2, 480 / 2, 5, 200, colorBytes);

    cout << "done drawing line. drawing circle\n";
    // main loop
    bool running = true;
    while (running) {
        // int random_pixel = (rand() % (854 * 480)) * 4;{
        int random_pixel = -1;
        vector<drc::byte> pixels_to_send(854 * 480 * 4, 1);
        for (int i = 0; i < 854 * 480 * 4; i++) {
            pixels_to_send[i] = screen.getPixels()->at(i);
        }
        //cout << "Pixels: " << pixels.size() << ", random_pixel: " << random_pixel << "\n";
        streamer->PushVidFrame(&pixels_to_send, 854, 480, drc::PixelFormat::kRGBA, drc::Streamer::FlippingMode::NoFlip,
                               drc::Streamer::StretchMode::NoStretch);
    }


    return 0;
}