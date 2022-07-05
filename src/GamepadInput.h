#ifndef PISOUND_GAMEPAD_GAMEPADINPUT_H
#define PISOUND_GAMEPAD_GAMEPADINPUT_H

#include <map>
#include "drc/input.h"
#include "drc/streamer.h"

class GamepadInput {
public:
    explicit GamepadInput(drc::Streamer *streamer);

    enum ButtonState {
        DOWN, UP, HELD, UNTOUCHED
    };

    void updateStates();

    ButtonState getState(drc::InputData::ButtonMask button);

    std::vector<float> getTouchCoords() const;

    ButtonState getTouchState() const;


private:
    drc::Streamer *streamer;
    drc::InputData *inputData;
    std::map<drc::InputData::ButtonMask, ButtonState> buttonStates;

    std::pair<ButtonState, std::vector<float>> touchState;
};


#endif //PISOUND_GAMEPAD_GAMEPADINPUT_H
