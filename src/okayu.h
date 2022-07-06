#ifndef PISOUND_GAMEPAD_OKAYU_H
#define PISOUND_GAMEPAD_OKAYU_H

#define Okayu_walkFrames 10
#define Okayu_DOWN 0
#define Okayu_SIDE 1
#define Okayu_UP 2
#define Okayu_walkStart(dir) (dir * Okayu_walkFrames + 36)
#define Okayu_idleStart(dir) (dir + 29)
#define Okayu_WalkFrame(dir, step) (step % Okayu_walkFrames) + Okayu_walkStart(dir)


#endif //PISOUND_GAMEPAD_OKAYU_H
