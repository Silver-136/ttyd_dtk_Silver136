// mot_roll.c: Mario's tube (roll) motion (mot_roll_post) and its cancel, key-lock and event-flag helpers.

#pragma once

#include "dolphin/types.h"

// MarioWork.currSubMotionId in mot_roll (names from what each step does)
enum {
    ROLL_SUB_INIT = 0,             // sets up the tube shape and the roll-up animation (PM_R_1A)
    ROLL_SUB_ROLL_UP = 1,          // rolling up; no control
    ROLL_SUB_UNROLL_START = 0x14,  // B pressed (or a script): starts the unroll animation (PM_R_1C)
    ROLL_SUB_UNROLL = 0x15,        // unrolling; no control
    ROLL_SUB_UNROLL_END = 0x16,    // back to the stay motion
    ROLL_SUB_ROLLING = 0x1E,       // rolled up and moving; mot_roll treats every state from here up as rolling
    ROLL_SUB_ROLLING_DIRECT = 50,  // marioChgRollMotion: rolling, entered without the roll-up
};

extern f32 roll_grx;
extern f32 roll_grz;
void roll_upstairs_on(void);
void clrRollEvtFlag(void);
void marioForceRollAnime(void);
void marioReInit_roll(void);
void marioResetRollSpd(void);
s32 marioRollCancelDisable(void);
void marioRollFixOn(void);
void marioRollFixOff(void);
void marioRollForceCancel(void);
void marioSetRollDispDir(void);
void mot_fall_roll(void);
void mot_roll(void);
void mot_roll_post(void);
void rollJumpSetup(void);
void rollEvtJumpSetup(void);
void setRollEvtFlag(void);
s32 getRollEvtFlag(void);
s32 marioRollCancel(void);
s32 marioRollKeyDisable(void);
s32 marioRollTryCancel(void);
