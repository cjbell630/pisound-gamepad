#ifndef PISOUND_GAMEPAD_CHARACTER_H
#define PISOUND_GAMEPAD_CHARACTER_H


#include "Sprite.h"
#include "GamepadInput.h"

class Character : public Sprite {
public:
    Character(uint32_t **imageBank, int width, int height, GamepadInput *gamepadInput, int x = 0, int y = 0,
              int scale = 1) : Sprite(imageBank, width, height, x, y, scale) {
        this->gamepadInput = gamepadInput;
        this->movementState = MovementState::IDLE;
        this->direction = {drc::InputData::ButtonMask::kBtnDown, drc::InputData::ButtonMask::kBtnDown};
        this->movementSpeed = 2 * scale; //TODO
    }

    void updatePosition();

    enum MovementState {
        IDLE, FIDGET, WALKING, STRAFING, ROLLING, LIFTING, DROPPING
    };

    void setMovementSpeed(int pixels);

    void pickUp(Sprite *sprite);

private:
    GamepadInput *gamepadInput;
    std::pair<drc::InputData::ButtonMask, drc::InputData::ButtonMask> direction;
    MovementState movementState;

    std::map<GamepadInput::ButtonState, std::vector<drc::InputData::ButtonMask>> invertDPad();

    int movementSpeed;
    bool canMove{true};
    bool canTurn{true};
    bool holding{false};

    void move(drc::InputData::ButtonMask direction);
    // TODO list of collision objects
};


#endif //PISOUND_GAMEPAD_CHARACTER_H
