// mot_walk.c: Mario's walk and dash motions (mot_walk, mot_dash) and their speed getters.

#include "dolphin/types.h"

f32 marioGetDashSpd(void);

BOOL pushEvtChk(struct HitEntry* hit);
f32 marioGetWalkSpd(void);
void mot_dash(void);
void mot_walk(void);
