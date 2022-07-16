#ifndef PISOUND_GAMEPAD_SPRITE_H
#define PISOUND_GAMEPAD_SPRITE_H

#include <stdint.h>

class Sprite {
public:
    // function definitions in here bc of cringe templates
    Sprite(uint32_t **imageBank, int width, int height, int x = 0, int y = 0, int scale = 1) {
        this->imageBank = imageBank;
        this->width = width;
        this->height = height;
        this->currFrame = 30;
        this->x = x;
        this->y = y;
        this->scale = scale;
    }

    void setFrame(int frame) {
        this->currFrame = frame;
    }

    int getFrame() const {
        return currFrame;
    }

    const uint32_t *getImage() {
        return this->imageBank[this->currFrame];
    }

    int x{0};
    int y{0};
    int scale{1};
    bool hFlip{false};
    bool vFlip{false};
    int width{};
    int height{};

private:
    uint32_t **imageBank;
    int currFrame{};
};


#endif //PISOUND_GAMEPAD_SPRITE_H
