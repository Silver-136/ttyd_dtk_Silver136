#pragma once

#include "driver/hitdrv.h"

HitEntry* kpaChkHitobjRide(void);
long kpaClearHitobjRide(void);
s32 kpaEnemyHitChk(f32 x, f32 y, f32 z, f32 h, f32 w);
HitEntry* kpaFlameHitCheck(f32 x, f32 y, f32 z, f32 r);
HitEntry* kpaGetHitFlame(void);
HitEntry* kpaGetHitobj(void);
HitEntry* kpaGetHitobjHead(s32 i);
HitEntry* kpaGetHitobjHead2(void);
HitEntry* kpaGetHitobjRide(s32 i);
HitEntry* kpaHitCheck(void);
HitEntry* kpaSearchFrontUnder(void);
