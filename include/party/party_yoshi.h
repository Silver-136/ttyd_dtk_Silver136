#pragma once

#include "dolphin/types.h"
#include "party.h"

// yoshiGetStatus results, distinct from PartyEntry.useMotionId.
typedef enum YoshiStatus {
    YOSHI_STATUS_INACTIVE = 0,
    YOSHI_STATUS_RIDING = 1,
    YOSHI_STATUS_FLUTTER = 2,
    YOSHI_STATUS_STAND_JUMP = 3,
    YOSHI_STATUS_FALL = 4,
    YOSHI_STATUS_OTHER = 5, // includes get-off motions 100..101 and the default active state
} YoshiStatus;

s32 yoshiGetStatus(void);

s32 evt_get_yoshi_frm(void);
void yoshi_bye(PartyEntry* party);
void yoshi_exit(PartyEntry* party);
void yoshi_init(PartyEntry* party);
void yoshi_use(PartyEntry* party);

void evt_set_yoshi_spd(f32 spd);
void marioRideYoshi(void);
void marioYoshiForceCancel(void);
void yoshi_jumpStand(void);
void evt_set_yoshi_frm(s32 msec);
s32 marioGetoffYoshi(void);
