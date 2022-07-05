#include "GamepadInput.h"

void GamepadInput::updateStates() {
    streamer->PollInput(inputData);
    for (auto &button: buttonStates) {
        if (inputData->buttons & button.first) { // button is now pressed
            switch (button.second) {
                case DOWN: // button has not changed since last poll (do not need to check case HELD bc redundant)
                    button.second = HELD;
                    break;
                case UP:
                case UNTOUCHED: // button changed since last poll
                    button.second = DOWN;
                    break;
                default:
                    break;
            }
        } else { // button is now unpressed
            switch (button.second) {
                case UP: // button has not changed since last poll (do not need to check case UNTOUCHED bc redundant)
                    button.second = UNTOUCHED;
                    break;
                case DOWN:
                case HELD: // button changed since last poll
                    button.second = UP;
                    break;
                default:
                    break;
            }
        }
    }
    if (inputData->ts_pressed) { // screen is now pressed
        switch (touchState.first) {
            case DOWN: // screen has not changed since last poll (do not need to check case HELD bc redundant)
                touchState.first = HELD;
                break;
            case UP:
            case UNTOUCHED: // screen changed since last poll
                touchState.first = DOWN;
                break;
            default:
                break;
        }
    } else { // screen is now unpressed
        switch (touchState.first) {
            case UP: // screen has not changed since last poll (do not need to check case UNTOUCHED bc redundant)
                touchState.first = UNTOUCHED;
                break;
            case DOWN:
            case HELD: // screen changed since last poll
                touchState.first = UP;
                break;
            default:
                break;
        }
    }
    this->touchState.second = {inputData->ts_x, inputData->ts_y, inputData->ts_pressure};
}

GamepadInput::ButtonState GamepadInput::getState(drc::InputData::ButtonMask button) {
    return this->buttonStates[button];
}

GamepadInput::GamepadInput(drc::Streamer *streamer) {
    this->streamer = streamer;
    this->inputData = new drc::InputData();

    // for some stupid f*cking reason I have to explicitly initalize every single entry in the enum bc apparently you
    // can't iterate over an enum
    this->buttonStates = {
            {drc::InputData::kBtnA,     UNTOUCHED},
            {drc::InputData::kBtnB,     UNTOUCHED},
            {drc::InputData::kBtnX,     UNTOUCHED},
            {drc::InputData::kBtnY,     UNTOUCHED},

            {drc::InputData::kBtnL,     UNTOUCHED},
            {drc::InputData::kBtnZL,    UNTOUCHED},
            {drc::InputData::kBtnR,     UNTOUCHED},
            {drc::InputData::kBtnZR,    UNTOUCHED},

            {drc::InputData::kBtnMinus, UNTOUCHED},
            {drc::InputData::kBtnPlus,  UNTOUCHED},

            {drc::InputData::kBtnSync,  UNTOUCHED},
            {drc::InputData::kBtnPower, UNTOUCHED},
            {drc::InputData::kBtnHome,  UNTOUCHED},
            {drc::InputData::kBtnTV,    UNTOUCHED},

            {drc::InputData::kBtnUp,    UNTOUCHED},
            {drc::InputData::kBtnDown,  UNTOUCHED},
            {drc::InputData::kBtnLeft,  UNTOUCHED},
            {drc::InputData::kBtnRight, UNTOUCHED},

            {drc::InputData::kBtnL3,    UNTOUCHED},
            {drc::InputData::kBtnR3,    UNTOUCHED}
    };
    this->touchState = {UNTOUCHED, {0, 0, 0}};
}

std::vector<float> GamepadInput::getTouchCoords() const {
    return this->touchState.second;
}

GamepadInput::ButtonState GamepadInput::getTouchState() const {
    return this->touchState.first;
}
