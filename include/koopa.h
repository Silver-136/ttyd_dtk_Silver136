#pragma once

#include "dolphin/types.h"

s32 kpaGetLevel(void);

struct EffectEntry* getKoopaFireEfp(void);
void kpaAttackStart(void);
void kpaAttackStart2(void);
void kpaCancelAttackMode(void);
void kpaChgPose(const char* a, const char* b);
s32 kpaGetAttackMode(void);
f32 kpaGetBaseScale(s32 lv);
s32 kpaGetBodyStatus(void);
s32 kpaGetStageType(void);
s32 kpaGetStageViewType(void);
f32 kpaGetWidth(s32 lv);
void kpaMSetMutekiTimer(void);
s32 kpaMutekiCheck(void);
void kpaMutekiOff(void);
void kpaMutekiOn(void);

struct HitEntry;

/* Bowser (kpa) work, pointed to by MarioWork.kpaWork (0x28C JP, 0x298 US).
 * kpaPreInit allocates it once (0x154) and marioReset keeps the pointer.
 * Same layout on JP and US. */
typedef enum KpaWorkFlags {
    KPA_WORK_FLAG_NO_HIP_ATTACK = (1 << 4),  // 0x10, kpaHipAttackOk returns FALSE
    KPA_WORK_FLAG_100 = (1 << 8),            // 0x100, koopa_motion sets/clears
    KPA_WORK_FLAG_MUTEKI = (1 << 16),        // 0x10000, kpaMutekiOn/Off/Check
    KPA_WORK_FLAG_WEARY = (1 << 24),         // 0x1000000, evt_koopa_weary
    KPA_WORK_FLAG_DEAD = (1 << 28),          // 0x10000000, evt_koopa_chk_dead
} KpaWorkFlags;

typedef struct KpaWork {
    u32 flags;                    // 0x000, KpaWorkFlags
    s32 level;                    // 0x004, kpaGetLevel/kpaSetLevel (0..3)
    u8 pad_08[0x10 - 0x8];        // 0x008, never accessed
    f32 standParamA;              // 0x010, koopa_motion stand init writes 0.02, never read
    f32 standParamB;              // 0x014, koopa_motion stand init writes -0.02, never read
    struct HitEntry* flameHit;    // 0x018, kpaFlameHitCheck result (kpaFireEntry2)
    struct HitEntry* objHit;      // 0x01C, kpaGetHitobj
    struct HitEntry* headHit[10]; // 0x020, kpaSearchHead
    s32 pad_48;                   // 0x048, never accessed (slot after headHit[10])
    struct HitEntry* rideHit[10]; // 0x04C, kpaSearchGround / kpaChkHitobjRide
    s32 pad_74;                   // 0x074, never accessed (slot after rideHit[10])
    f32 headDist[10];             // 0x078, 1000.0f when clear
    s32 pad_A0;                   // 0x0A0, never accessed (slot after headDist[10])
    f32 rideDist[10];             // 0x0A4, 1000.0f when clear
    s32 pad_CC;                   // 0x0CC, never accessed (slot after rideDist[10])
    s32 walkRate;                 // 0x0D0, _rateTbl[level] (init 1)
    s32 pad_D4;                   // 0x0D4, never accessed
    s32 fireRate;                 // 0x0D8, 12 at level 0 else 8 (init 1)
    s32 pad_DC;                   // 0x0DC, never accessed
    s32 fireCount;                // 0x0E0
    u8 pad_E4[0x138 - 0xE4];      // 0x0E4, never accessed
    s32 mutekiTimer;              // 0x138, kpaMSetMutekiTimer: 12 s
    s32 attackState;              // 0x13C, kpaAttackStart (1), fire states 20-24 / 40-44, 30
    s32 attackTimer;              // 0x140
    s32 fireHold;                 // 0x144, evt_koopa_fire keeps the flame going while set
    s32 pad_148;                  // 0x148, never accessed
    s32 fireSfxId;                // 0x14C, kpaFireSEOn/Off, -1 when off
    s32 swimSplashTimer;          // 0x150, kpa_swim_splash
} KpaWork;                  // size 0x154
f32 kpaSetFallPara(void);
