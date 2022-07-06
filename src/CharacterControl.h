#ifndef PISOUND_GAMEPAD_CHARACTERCONTROL_H
#define PISOUND_GAMEPAD_CHARACTERCONTROL_H


#include "Sprite.h"
#include "GamepadInput.h"

class CharacterControl {
public:
    explicit CharacterControl(Sprite *sprite, GamepadInput *gamepadInput);

    void updatePosition();

    enum MovementState {
        IDLE, FIDGET, WALKING, STRAFING, ROLLING
    };

    void setMovementSpeed(int pixels);

private:
    Sprite *sprite;
    GamepadInput *gamepadInput;
    std::pair<drc::InputData::ButtonMask, drc::InputData::ButtonMask> direction;
    MovementState movementState;

    std::map<GamepadInput::ButtonState, std::vector<drc::InputData::ButtonMask>> invertDPad();

    int movementSpeed;

    void move(drc::InputData::ButtonMask direction);

    // TODO list of collision objects
};

#endif //PISOUND_GAMEPAD_CHARACTERCONTROL_H
