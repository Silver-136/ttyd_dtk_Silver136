// mot_hammer.c: Mario's hammer motions (mot_hammer, mot_hammer2 spin, mot_ham2_post) and the hammer level query.

#pragma once

#include <dolphin/types.h>

s32 marioChkHammer2(void);
f32 marioGetAngleKaitenHammer(void);
s32 marioGetHammerLevel(void);
void mot_ham2_post(void);
void mot_hammer(void);
void mot_hammer2(void);
