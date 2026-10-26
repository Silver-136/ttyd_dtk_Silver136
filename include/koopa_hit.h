#pragma once

#include "driver/hitdrv.h"

HitEntry* kpaChkHitobjRide(void);
long kpaClearHitobjRide(void);
// kpaEnemyHitChk results
typedef enum KpaEnemyHit {
    KPA_ENEMY_HIT_NONE = 0,
    KPA_ENEMY_HIT_STOMP = 1,   // Bowser lands on it
    KPA_ENEMY_HIT_FIRE = 2,    // in Bowser's fire
    KPA_ENEMY_HIT_STAR = 3,    // touched while Bowser is invincible
    KPA_ENEMY_HIT_BOWSER = 4,  // it hurts Bowser
} KpaEnemyHit;

s32 kpaEnemyHitChk(f32 x, f32 y, f32 z, f32 h, f32 w);
HitEntry* kpaFlameHitCheck(f32 x, f32 y, f32 z, f32 r);
HitEntry* kpaGetHitFlame(void);
HitEntry* kpaGetHitobj(void);
HitEntry* kpaGetHitobjHead(s32 i);
HitEntry* kpaGetHitobjHead2(void);
HitEntry* kpaGetHitobjRide(s32 i);
HitEntry* kpaHitCheck(void);
HitEntry* kpaSearchFrontUnder(void);
HitEntry* kpaSearchGround(f32 range, f32* outY, f32* outDepth, f32* outAng);
HitEntry* kpaSearchHead(f32 hgt, Vec* pos, f32* outB);
