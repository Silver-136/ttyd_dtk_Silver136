// mot_jabara.c: Mario's Spring Jump (jabara, "bellows") motion, mot_grasp, and the jabara state and overhead checks.

#pragma once

#include "dolphin/types.h"

BOOL jabaraNoHitChk(void);

// marioGetJabaraState: Spring Jump phase, from the sub-motion.
enum {
    JABARA_STATE_NONE = 0,
    JABARA_STATE_RISING = 1,
    JABARA_STATE_GRAB_CEILING = 2,
    JABARA_STATE_HANG_MOVE = 3,
    JABARA_STATE_BUMP_CEILING = 4,
    JABARA_STATE_SWING = 5,
    JABARA_STATE_FALLING = 6,   // counts as a jump attack on enemies (npcdrv)
    JABARA_STATE_OTHER = 7,
};

s32 marioGetJabaraState(void);
BOOL marioChkJabara(void);
BOOL marioPriCheckJabara(void);
void mot_grasp(void);
void mot_jabara(void);
void mot_jabara_post(void);
