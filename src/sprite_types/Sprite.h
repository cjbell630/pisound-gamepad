#ifndef PISOUND_GAMEPAD_SPRITE_H
#define PISOUND_GAMEPAD_SPRITE_H

#include <stdint.h>
#include <cmath>
#include "Drawable.h"

class Sprite : public Drawable {
public:
    // function definitions in here bc of cringe templates
    Sprite(uint32_t **imageBank, int width, int height, int x = 0, int y = 0, int scale = 1) {
        this->imageBank = imageBank;
        this->width = width;
        this->height = height;
        this->currFrame = 0;
        this->x = x;
        this->y = y;
        this->scale = scale;
    }

    void setFrame(int frame) {
        this->currFrame = frame;
        untilMove = frameDelay;
    }

    bool advFrame() {
        if (untilMove == 0) {
            this->currFrame++;
            untilMove = frameDelay;
            return true;
        } else {
            untilMove--;
            return false;
        }
    }

    int getFrame() const {
        return currFrame;
    }

    const uint32_t *getImage() {
        return this->imageBank[this->currFrame];
    }

    void glideTowards(int targX, int targY, int stepsLeft) {
        this->x += (targX - this->x) / stepsLeft;
        this->y += (targY - this->y) / stepsLeft;
    }

    void setGripPoint(int x, int y) {
        this->gripX = x;
        this->gripY = y;
    }

    int distForComp(Sprite *otherSprite) {
        return pow(x - otherSprite->x, 2) + pow(y - otherSprite->y, 2);
    }

    void draw(GamepadScreen *screen) override {
        int pixelOffset = screen->screenIndex(x, y);

        // TODO averaging

        for (int yI = 0; yI < height; yI++) {
            for (int xI = 0; xI < width; xI++) {
                auto pixel_int = getImage()[32 * yI + xI];
                //TODO
                if (pixel_int & 0x000000FF) { // if alpha is not 0
                    drc::byte b;

                    int flippedX = hFlip ? width - xI - 1 : xI;
                    int flippedY = vFlip ? height - yI - 1 : yI;

                    for (int i = 0; i < 4; i++) {
                        b = pixel_int & 0xff;
                        for (int scaleY = 0; scaleY < scale; scaleY++) {
                            for (int scaleX = 0; scaleX < scale; scaleX++) {
                                int screenIndex = (flippedX * scale + scaleX) * 4 +
                                                  (screen->screenWidth * 4 * (flippedY * scale + scaleY)) + i +
                                                  pixelOffset;
                                if (0 <= screenIndex &&
                                    screenIndex < screen->pixelArraySize()) { // do not draw out of bounds
                                    screen->setPixelByte(screenIndex, b);
                                }
                            }
                        }

                        pixel_int = pixel_int >> 8;
                    }
                }
            }
        }
    }

    int x{0};
    int y{0};
    int scale{1};
    bool hFlip{false};
    bool vFlip{false};
    int width{};
    int height{};
    int frameDelay{2}; //TODO doc + getter + setter
    int gripX{0};
    int gripY{0};//TODO doc + getter + setter

private:
    uint32_t **imageBank;
    int currFrame{};
    int untilMove{0};
    //TODO grip coord
};


#endif //PISOUND_GAMEPAD_SPRITE_H
