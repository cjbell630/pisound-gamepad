#include "Character.h"
#include "../images/okayu.h"
#include <iostream>

void Character::updatePosition() {
    //TODO also cringe
    int prevDirectionThingy =
            direction.first == drc::InputData::kBtnDown ? 0 : direction.first == drc::InputData::kBtnUp ? 2 : 1;
    if (movementState == LIFTING || movementState == DROPPING) {
        int lastFrame = (movementState == LIFTING ? Okayu_liftStart(prevDirectionThingy) + Okayu_liftFrames :
                         Okayu_dropStart(prevDirectionThingy) + Okayu_dropFrames) - 1;
        if (getFrame() == lastFrame) { // if on last anim frame
            heldSprite = movementState == DROPPING ? nullptr : heldSprite;
            setFrame(Okayu_idleStart(prevDirectionThingy, heldSprite));
            movementState = IDLE;
        } else {
            // TODO dropping needs to pick and store a targ pos, then move towards it
            if (movementState == LIFTING) {
                heldSprite->glideTowards(x + gripX * scale - heldSprite->gripX * heldSprite->scale,
                                         y + gripY * scale - heldSprite->gripY * heldSprite->scale,
                                         (lastFrame - getFrame()) * frameDelay);
            } else {
                int xOff, yOff;
                switch (direction.first) {
                    case drc::InputData::kBtnUp:
                        //xOff = (width * scale - heldSprite->width * heldSprite->scale) / 2;
                        xOff = gripX * scale;
                        yOff = height * scale / 2;
                        break;
                    case drc::InputData::kBtnDown:
                        //xOff = (width * scale - heldSprite->width * heldSprite->scale) / 2;
                        xOff = gripX * scale;
                        //yOff = height * scale * 7 / 8;
                        yOff = height * scale + heldSprite->gripY * heldSprite->scale;
                        break;
                    case drc::InputData::kBtnLeft:
                        xOff = -width * scale * 2 / 8;
                        yOff = (height * scale);
                        break;
                    case drc::InputData::kBtnRight:
                        xOff = width * scale + heldSprite->gripX * heldSprite->scale;
                        yOff = (height * scale);
                        break;
                }
                heldSprite->glideTowards(x + xOff - heldSprite->gripX * heldSprite->scale,
                                         y + yOff - heldSprite->gripY * heldSprite->scale,
                                         (lastFrame - getFrame()) * frameDelay);
            }
            advFrame();
        }
    } else if (gamepadInput->getState(drc::InputData::ButtonMask::kBtnA) == GamepadInput::ButtonState::DOWN) {
        if (heldSprite) { // if holding something
            movementState = DROPPING;
            setFrame(Okayu_dropStart(prevDirectionThingy));
        } else if (!holdablesWithinRange->empty()) { // if not holding and there is an object that can be picked up
            heldSprite = holdablesWithinRange->front().first; // pick up first object
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
                    if (getFrame() == Okayu_walkStart(directionThingy, heldSprite) + Okayu_walkFrames - 1) {
                        setFrame(Okayu_walkStart(directionThingy, heldSprite));
                    } else {
                        advFrame();
                    }
                } else {
                    // set to init walking frame
                    setFrame(Okayu_walkStart(directionThingy, heldSprite));
                }
                break;
            case IDLE:
                // if idle
                if (newDirection.first != direction.first || movementState != IDLE) {
                    // update sprite to face new direction / start idling
                    setFrame(Okayu_idleStart(directionThingy, heldSprite));
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
    //TODO add sine jiggle for held object
    switch (direction) {
        case drc::InputData::kBtnUp:
            y -= movementSpeed;
            if (heldSprite) {
                heldSprite->y -= movementSpeed;
            }
            break;
        case drc::InputData::kBtnDown:
            y += movementSpeed;
            if (heldSprite) {
                heldSprite->y += movementSpeed;
            }
            break;
        case drc::InputData::kBtnLeft:
            x -= movementSpeed;
            if (heldSprite) {
                heldSprite->x -= movementSpeed;
            }
            break;
        case drc::InputData::kBtnRight:
            x += movementSpeed;
            if (heldSprite) {
                heldSprite->x += movementSpeed;
            }
            break;
        default:
            break;
    }
}

void Character::setHoldables(std::vector<std::pair<Sprite *, int>> *holdables) {
    this->holdablesWithinRange = holdables;
}

Character::Character(uint32_t **imageBank, int width, int height, GamepadInput *gamepadInput, int x, int y, int scale)
        : Sprite(imageBank, width, height, x, y, scale) {
    this->gamepadInput = gamepadInput;
    this->movementState = MovementState::IDLE;
    this->direction = {drc::InputData::ButtonMask::kBtnDown, drc::InputData::ButtonMask::kBtnDown};
    this->movementSpeed = 2 * scale; //TODO
    this->setFrame(Okayu_idleStart(0, heldSprite));
}

Sprite *Character::getHeldSprite() {
    return heldSprite;
}
