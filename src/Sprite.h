#ifndef PISOUND_GAMEPAD_SPRITE_H
#define PISOUND_GAMEPAD_SPRITE_H

#include <stdint.h>

template<int numSprites>
class Sprite {
public:
    Sprite(const uint32_t (*imageBank)[numSprites], int width, int height, int x = 0, int y = 0, int scale = 1){
        this->imageBank = imageBank;
        this->width = width;
        this->height = height;
        this->frame = 0;
        this->x = x;
        this->y = y;
        this->scale = scale;
    }

    void setFrame(int frame){
        this->frame = frame;
    }

    const uint32_t *getImage(){
        return this->imageBank[this->frame];
    }

    int x{};
    int y{};
    int scale{};

private:
    const uint32_t (*imageBank)[numSprites];
    int width{};
    int height{};
    int frame{};
};


#endif //PISOUND_GAMEPAD_SPRITE_H
