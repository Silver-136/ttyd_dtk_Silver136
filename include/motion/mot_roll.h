#pragma once

#include <dolphin/types.h>

s32 marioRollKeyDisable(void);
void setRollEvtFlag(void);
void clrRollEvtFlag(void);
s32 getRollEvtFlag(void);
void marioResetRollSpd(void);
u8 marioForceRollAnime(void);
s32 unk_800a1454(void);
s32 marioRollCancel(void);
void marioRollForceCancel(void);
void marioSetRollDispDir(void);
void rollEvtJumpSetup(void);
u8 marioReInit_roll(void);
void roll_upstairs_on(void);
void mot_fall_roll(void);
