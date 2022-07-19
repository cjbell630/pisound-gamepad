#ifndef PISOUND_GAMEPAD_CABLE_H
#define PISOUND_GAMEPAD_CABLE_H


#include "Drawable.h"
#include "Sprite.h"
#include "../images/jack.c"
#include "Wire.h"

class Cable : public Drawable {
    static void _init();

    static uint32_t **jack_pp_data;
    static bool loadedData;
public:
    Cable(); // TODO x pos etc
    void draw(GamepadScreen *screen) override;

    void addChildren(std::vector<Drawable *> *children) override;

    Sprite *leftPlug;
    Sprite *rightPlug;
    Wire *wire;

private:
};

#endif //PISOUND_GAMEPAD_CABLE_H
