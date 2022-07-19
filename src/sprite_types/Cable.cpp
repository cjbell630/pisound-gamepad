#include "Cable.h"
#include "Wire.h"

uint32_t **Cable::jack_pp_data;
bool Cable::loadedData;

// NOTE this is weird, not sure if this is best practice
void Cable::_init() {
    jack_pp_data = new uint32_t *[JACK_FRAME_WIDTH * JACK_FRAME_HEIGHT];
    for (int i = 0; i < JACK_FRAME_COUNT; i++) {
        jack_pp_data[i] = const_cast<uint32_t *>(jack_data[i]);
    }
}


void Cable::draw(GamepadScreen *screen) {
    // TODO unnecessary now that sprites are extracted
    leftPlug->draw(screen);
    rightPlug->draw(screen);
    wire->draw(screen);
}

Cable::Cable() {
    // TODO there has got to be a better way to do tihs
    if (!loadedData) {
        _init();
        loadedData = true;
    }
    //TODO magic numbers
    rightPlug = new Sprite(jack_pp_data, JACK_FRAME_WIDTH, JACK_FRAME_HEIGHT, 200, 200, 2);
    rightPlug->setGripPoint(30, 20);
    leftPlug = new Sprite(jack_pp_data, JACK_FRAME_WIDTH, JACK_FRAME_HEIGHT, 400, 200, 2);
    rightPlug->setGripPoint(30, 20);
    wire = new Wire(leftPlug, rightPlug, 0xFFb32568);
}

void Cable::addChildren(std::vector<Drawable *> *children) {
    children->push_back(rightPlug);
    children->push_back(leftPlug);
    children->push_back(wire);
}
