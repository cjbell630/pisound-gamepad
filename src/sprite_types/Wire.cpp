#include "Wire.h"

Wire::Wire(Sprite *leftPlug, Sprite *rightPlug, uint32_t color) {
    this->leftPlug = leftPlug;
    this->rightPlug = rightPlug;
    this->color = color;
}

void Wire::draw(GamepadScreen *screen) {
    // TODO magic numbers
    // TODO draw line around stuff, add shadow for clarity
    screen->drawLine(
            leftPlug->x, leftPlug->y,
            rightPlug->x, rightPlug->y,
            10 * rightPlug->scale, color
    );
}
