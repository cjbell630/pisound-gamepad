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
        untilMove = frameDelay;
    }

    bool advFrame() {
        if(untilMove==0){
            this->currFrame++;
            untilMove = frameDelay;
            return true;
        }else{
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

    int x{0};
    int y{0};
    int scale{1};
    bool hFlip{false};
    bool vFlip{false};
    int width{};
    int height{};
    int frameDelay{2}; //TODO doc + getter + setter

private:
    uint32_t **imageBank;
    int currFrame{};
    int untilMove{0};
};


#endif //PISOUND_GAMEPAD_SPRITE_H
