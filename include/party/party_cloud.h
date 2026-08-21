#pragma once

#include <dolphin/types.h>

s32 cloudGetStatus(void);
f32 cloudGetBreathPower(void* pos, f32 radius);
s32 cloudGetHitBreathout(int param_1);
void N_cloudLockAnimationsOff(void* party);
void N_cloudLockAnimationsOn(void* party);
u8 cloudGetAt(void* param_1);
void N_cloud_use(void* pParty);
