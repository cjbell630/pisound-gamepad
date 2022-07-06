#include "CharacterControl.h"
#include "okayu.h"
#include <iostream>

std::map<GamepadInput::ButtonState, std::vector<drc::InputData::ButtonMask>> CharacterControl::invertDPad() {

    std::map<GamepadInput::ButtonState, std::vector<drc::InputData::ButtonMask>> inverted = {
            {GamepadInput::ButtonState::DOWN,      {}},
            {GamepadInput::ButtonState::UP,        {}},
            {GamepadInput::ButtonState::HELD,      {}},
            {GamepadInput::ButtonState::UNTOUCHED, {}}
    };
    for (auto &i: {drc::InputData::ButtonMask::kBtnUp, drc::InputData::ButtonMask::kBtnDown,
                   drc::InputData::ButtonMask::kBtnLeft, drc::InputData::ButtonMask::kBtnRight}) {
        inverted[gamepadInput->getState(i)].push_back(i);
    }
    return inverted;
}

CharacterControl::CharacterControl(Sprite *sprite, GamepadInput *gamepadInput) {
    this->sprite = sprite;
    this->gamepadInput = gamepadInput;
    this->movementState = MovementState::IDLE;
    this->direction = {drc::InputData::ButtonMask::kBtnDown, drc::InputData::ButtonMask::kBtnDown};
    this->movementSpeed = 2 * sprite->scale;
}

void CharacterControl::updatePosition() {
    //std::cout << "updatePosition" << std::endl;

    auto invertedInputs = invertDPad();
    auto numPressed = invertedInputs[GamepadInput::ButtonState::HELD].size() +
                      invertedInputs[GamepadInput::ButtonState::DOWN].size();

    std::pair<drc::InputData::ButtonMask, drc::InputData::ButtonMask> newDirection;
    MovementState newMovementState;

    //std::cout << "num pressed: " << numPressed << std::endl;
    if (numPressed == 0) {
        newMovementState = MovementState::IDLE;
        newDirection = direction;
        // TODO add fidgeting
    } else if (numPressed == 1) {
        newMovementState = MovementState::WALKING;
        newDirection.first = invertedInputs[
                invertedInputs[GamepadInput::ButtonState::HELD].size() == 1 ? GamepadInput::ButtonState::HELD :
                GamepadInput::ButtonState::DOWN
        ].front();
        newDirection.second = newDirection.first;
    } else {
        newMovementState = MovementState::STRAFING;
        switch (invertedInputs[GamepadInput::ButtonState::HELD].size()) {
            case 0:
                newDirection.first = invertedInputs[GamepadInput::ButtonState::DOWN].front();
                newDirection.second = invertedInputs[GamepadInput::ButtonState::DOWN].at(1);
                break;
            case 1:
                newDirection.first = invertedInputs[GamepadInput::ButtonState::HELD].front();
                newDirection.second = invertedInputs[GamepadInput::ButtonState::DOWN].front();
                break;
            default:
                // can't just pick first one if multiple held; try to preserve order
                if (gamepadInput->getState(direction.first) == GamepadInput::ButtonState::HELD) {
                    // if still moving in the same direction
                    newDirection.first = direction.first;
                    newDirection.second = invertedInputs[GamepadInput::ButtonState::HELD].at(
                            invertedInputs[GamepadInput::ButtonState::HELD].front() == direction.first ? 1 : 0
                    );
                } else {
                    newDirection.first = invertedInputs[GamepadInput::ButtonState::HELD].front();
                    newDirection.second = invertedInputs[GamepadInput::ButtonState::HELD].at(1);
                }
                break;
        }
    }

    //TODO this is cringe
    int directionThingy =
            direction.first == drc::InputData::kBtnDown ? 0 : direction.first == drc::InputData::kBtnUp ? 2 : 1;

    //std::cout << "directionThingy: " << directionThingy << std::endl;
    //std::cout << "newDirection: " << newDirection.first << ", " << newDirection.second << std::endl;
    //std::cout << "newMovement: " << newMovementState << std::endl;

    switch (newMovementState) {
        case STRAFING:
            // move newDirection.second
            move(newDirection.second);
        case WALKING:
            // if walking or strafing
            // move newDirection.first
            move(newDirection.first);
            if ((movementState == WALKING || movementState == STRAFING) &&
                newDirection.first == direction.first) {
                // if continue walking
                // move to next frame
                // TODO change macros to something more generic
                if (sprite->getFrame() == Okayu_walkStart(directionThingy) + Okayu_walkFrames - 1) {
                    sprite->setFrame(Okayu_walkStart(directionThingy));
                } else {
                    sprite->setFrame(sprite->getFrame() + 1);
                }
            } else {
                // set to init walking frame
                sprite->setFrame(Okayu_walkStart(directionThingy));
            }
            break;
        case IDLE:
            // if idle
            if (newDirection.first != direction.first || movementState != IDLE) {
                // update sprite to face new direction / start idling
                sprite->setFrame(Okayu_idleStart(directionThingy));
            }
            break;

    }

    // TODO flip sprite if facing other direction

    movementState = newMovementState;
    direction = newDirection;

}

void CharacterControl::setMovementSpeed(int pixels) {
    movementSpeed = pixels;
}

void CharacterControl::move(drc::InputData::ButtonMask direction) {
    switch (direction) {
        case drc::InputData::kBtnUp:
            sprite->y -= movementSpeed;
            break;
        case drc::InputData::kBtnDown:
            sprite->y += movementSpeed;
            break;
        case drc::InputData::kBtnLeft:
            sprite->x -= movementSpeed;
            break;
        case drc::InputData::kBtnRight:
            sprite->x += movementSpeed;
            break;
        default:
            break;
    }
}
