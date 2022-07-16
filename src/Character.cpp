#include "Character.h"
#include "okayu.h"
#include <iostream>

void Character::updatePosition() {
    //TODO also cringe
    int prevDirectionThingy =
            direction.first == drc::InputData::kBtnDown ? 0 : direction.first == drc::InputData::kBtnUp ? 2 : 1;
    if (movementState == LIFTING || movementState == DROPPING) {
        int lastFrame = (movementState == LIFTING ? Okayu_liftStart(prevDirectionThingy) + Okayu_liftFrames :
                         Okayu_dropStart(prevDirectionThingy) + Okayu_dropFrames) - 1;
        if (getFrame() == lastFrame) { // if on last anim frame
            holding = !holding;
            setFrame(Okayu_idleStart(prevDirectionThingy, holding));
            movementState = IDLE;
        } else {
            advFrame();
        }
    } else if (gamepadInput->getState(drc::InputData::ButtonMask::kBtnA) == GamepadInput::ButtonState::DOWN) {
        if (holding) {
            movementState = DROPPING;
            setFrame(Okayu_dropStart(prevDirectionThingy));
        } else {
            movementState = LIFTING;
            setFrame(Okayu_liftStart(prevDirectionThingy));
        }
    } else {

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
                newDirection.first == drc::InputData::kBtnDown ? 0 : newDirection.first == drc::InputData::kBtnUp ? 2
                                                                                                                  : 1;

        //std::cout << "directionThingy: " << directionThingy << std::endl;
        //std::cout << "newDirection: " << newDirection.first << ", " << newDirection.second << std::endl;
        //std::cout << "newMovement: " << newMovementState << std::endl;

        switch (newMovementState) {
            case STRAFING:
                // move newDirection.second
                //TODO make more efficient
                if (canMove) {
                    move(newDirection.second);
                }
            case WALKING:
                // if walking or strafing
                // move newDirection.first
                if (canMove) {
                    move(newDirection.first);
                }
                if ((movementState == WALKING || movementState == STRAFING) &&
                    newDirection.first == direction.first) {
                    // if continue walking
                    // move to next frame
                    // TODO change macros to something more generic
                    if (getFrame() == Okayu_walkStart(directionThingy, holding) + Okayu_walkFrames - 1) {
                        setFrame(Okayu_walkStart(directionThingy, holding));
                    } else {
                        advFrame();
                    }
                } else {
                    // set to init walking frame
                    setFrame(Okayu_walkStart(directionThingy, holding));
                }
                break;
            case IDLE:
                // if idle
                if (newDirection.first != direction.first || movementState != IDLE) {
                    // update sprite to face new direction / start idling
                    setFrame(Okayu_idleStart(directionThingy, holding));
                }
                break;
        }

        // TODO inefficient, should only set this if dir changed and facing right
        hFlip = newDirection.first == drc::InputData::kBtnRight;

        movementState = newMovementState;
        direction = newDirection;
    }
}

void Character::setMovementSpeed(int pixels) {
    movementSpeed = pixels;
}

std::map<GamepadInput::ButtonState, std::vector<drc::InputData::ButtonMask>> Character::invertDPad() {
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

void Character::move(drc::InputData::ButtonMask direction) {
    switch (direction) {
        case drc::InputData::kBtnUp:
            y -= movementSpeed;
            break;
        case drc::InputData::kBtnDown:
            y += movementSpeed;
            break;
        case drc::InputData::kBtnLeft:
            x -= movementSpeed;
            break;
        case drc::InputData::kBtnRight:
            x += movementSpeed;
            break;
        default:
            break;
    }
}

void Character::pickUp(Sprite *sprite) {
    movementState = LIFTING;
}
