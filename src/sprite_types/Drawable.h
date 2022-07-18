#ifndef PISOUND_GAMEPAD_DRAWABLE_H
#define PISOUND_GAMEPAD_DRAWABLE_H


#include "../GamepadScreen.h"

class Drawable {
public:
    virtual void draw(GamepadScreen *screen) = 0;
};


#endif //PISOUND_GAMEPAD_DRAWABLE_H
