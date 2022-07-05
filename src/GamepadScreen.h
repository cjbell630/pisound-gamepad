#ifndef PISOUND_GAMEPAD_GAMEPADSCREEN_H
#define PISOUND_GAMEPAD_GAMEPADSCREEN_H


#include <drc/types.h>
#include "Sprite.h"

class GamepadScreen {
public:
    GamepadScreen(int width, int height);

    template<int numSprites>
    void draw(Sprite<numSprites> sprite) {
        int pixelOffset = screenIndex(sprite.x, sprite.y);

        for (int yI = 0; yI < 32; yI++) {
            for (int xI = 0; xI < 32; xI++) {
                auto pixel_int = sprite.getImage()[32 * yI + xI];
                drc::byte b;
                for (int i = 0; i < 4; i++) {
                    b = pixel_int & 0xff;
                    for (int scaleY = 0; scaleY < sprite.scale; scaleY++) {
                        for (int scaleX = 0; scaleX < sprite.scale; scaleX++) {
                            pixels->at((xI * sprite.scale + scaleX) * 4 +
                                       (screenWidth * 4 * (yI * sprite.scale + scaleY)) + i +
                                       pixelOffset) = b;
                        }
                    }

                    pixel_int = pixel_int >> 8;
                }
            }
        }
    }

    void drawLine(int x0, int y0, int x1, int y1, float width, uint32_t color);

    void circle2(int xc, int yc, int thickness, int radius, const drc::byte *colorBytes);

    std::vector<drc::byte> *getPixels();


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
