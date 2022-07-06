#ifndef PISOUND_GAMEPAD_GAMEPADSCREEN_H
#define PISOUND_GAMEPAD_GAMEPADSCREEN_H

#include <drc/types.h>
#include "Sprite.h"

class GamepadScreen {
public:
    GamepadScreen(int width, int height);

    void draw(Sprite sprite);

    void drawLine(int x0, int y0, int x1, int y1, float thickness, uint32_t color);

    void drawCircle(int xc, int yc, int radius, int thickness, uint32_t color);

    std::vector<drc::byte> *getPixels();

    void wipe(uint32_t color = 0xFFFFFFFF);

private:
    int screenWidth;
    int screenHeight;
    std::vector<drc::byte> *pixels;

    int screenIndex(int spriteIndex, int spriteWidth, int xOff, int yOff) const;

    int screenIndex(int x, int y) const;

    void setPixel(int screenIndex, const drc::byte *colorBytes);

    void horLine(int x1, int x2, int y, const drc::byte *colorBytes);

    void verLine(int x, int y1, int y2, const drc::byte *colorBytes);


};


#endif //PISOUND_GAMEPAD_GAMEPADSCREEN_H
