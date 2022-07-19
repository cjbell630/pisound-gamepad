#ifndef PISOUND_GAMEPAD_WIRE_H
#define PISOUND_GAMEPAD_WIRE_H


#include "Drawable.h"
#include "Sprite.h"

class Wire : public Drawable {
public:
    Wire(Sprite *leftPlug, Sprite *rightPlug, uint32_t color);
    void draw(GamepadScreen *screen) override;

private:
    Sprite *leftPlug;
    Sprite *rightPlug;
    uint32_t color;
};


#endif //PISOUND_GAMEPAD_WIRE_H
