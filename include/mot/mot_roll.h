#pragma once

#include "dolphin/types.h"

extern f32 roll_grx;
extern f32 roll_grz;
void roll_upstairs_on(void);
void clrRollEvtFlag(void);
void marioForceRollAnime(void);
void marioReInit_roll(void);
void marioResetRollSpd(void);
s32 marioRollCancelDisable(void);
void marioRollFixOn(void);
void marioRollForceCancel(void);
void marioSetRollDispDir(void);
void mot_fall_roll(void);
void mot_roll(void);
void mot_roll_post(void);
void rollEvtJumpSetup(void);
void setRollEvtFlag(void);
s32 getRollEvtFlag(void);
s32 marioRollCancel(void);
s32 marioRollKeyDisable(void);
