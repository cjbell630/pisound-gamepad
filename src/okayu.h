#ifndef PISOUND_GAMEPAD_OKAYU_H
#define PISOUND_GAMEPAD_OKAYU_H

#define Okayu_walkFrames 10
#define Okayu_liftFrames 5
#define Okayu_dropFrames 4
#define Okayu_DOWN 0
#define Okayu_SIDE 1
#define Okayu_UP 2
#define Okayu_walkStart(dir, holding) (Okayu_walkFrames * dir + (holding ? 288 : 20))
#define Okayu_liftStart(dir) (dir * (Okayu_liftFrames + Okayu_dropFrames) + 262)
#define Okayu_dropStart(dir) (dir * (Okayu_liftFrames + Okayu_dropFrames) + 262 + Okayu_liftFrames)
#define Okayu_idleStart(dir, holding) (holding ? Okayu_liftStart(dir) + Okayu_liftFrames : dir + 13)
#define Okayu_WalkFrame(dir, step) ((step % Okayu_walkFrames) + Okayu_walkStart(dir))
#define Okayu_LiftFrame(dir, step) (step + Okayu_liftStart(dir))


#endif //PISOUND_GAMEPAD_OKAYU_H
