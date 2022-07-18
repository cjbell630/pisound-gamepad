#ifndef PISOUND_GAMEPAD_SPRITE_H
#define PISOUND_GAMEPAD_SPRITE_H

#include <stdint.h>
#include <cmath>

class Sprite {
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
