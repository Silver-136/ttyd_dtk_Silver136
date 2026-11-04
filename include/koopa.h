// koopa.c: Bowser (koopa) as the player in the side-scrolling interludes: level, camera, attack and invincibility functions.

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
// kpaGetStageViewType
enum {
    KPA_VIEW_SIDE = 0,  // side-scrolling stages kpa_02 to kpa_06
    KPA_VIEW_3D = 1,
};

s32 kpaGetStageViewType(void);
f32 kpaGetWidth(s32 lv);
void kpaMSetMutekiTimer(void);
s32 kpaMutekiCheck(void);
void kpaMutekiOff(void);
void kpaMutekiOn(void);

struct HitEntry;

/* Bowser (kpa) work, pointed to by MarioWork.kpaWork (0x28C in JP, 0x298 from US on).
 * kpaPreInit allocates it once (0x154) and marioReset keeps the pointer.
 * Same layout in every version. */
typedef enum KpaWorkFlags {
    KPA_WORK_FLAG_NO_HIP_ATTACK = (1 << 4),  // 0x10, kpaHipAttackOk returns FALSE
    KPA_WORK_FLAG_WATER_FALL_SFX = (1 << 8), // 0x100, play SFX_KUPPA_WATER_FALL1 on the next drop into water (set while falling or leaving the water)
    KPA_WORK_FLAG_MUTEKI = (1 << 16),        // 0x10000, kpaMutekiOn/Off/Check
    KPA_WORK_FLAG_WEARY = (1 << 24),         // 0x1000000, evt_koopa_weary
    KPA_WORK_FLAG_DEAD = (1 << 28),          // 0x10000000, evt_koopa_chk_dead
} KpaWorkFlags;

#define KPA_BODY_NOT_BOWSER (-1)
#define KPA_BODY_DRY 0
#define KPA_BODY_PARTLY_SUBMERGED 1
#define KPA_BODY_SUBMERGED 2

// KpaWork.attackState: Bowser's fire breath, from kpaAttackStart (1, 20..24) or kpaAttackStart2 (30, 40..44; no motion change).
typedef enum KpaAttackState {
    KPA_ATTACK_NONE = 0,
    KPA_ATTACK_START = 1,            // stop moving, reset the fire counters
    KPA_ATTACK_WINDUP_POSE = 20,     // KPA_A_1A
    KPA_ATTACK_WINDUP = 21,          // timer, then KPA_A_1B and the fire sound
    KPA_ATTACK_FIRE_START = 22,      // fire entries, then KPA_A_1C
    KPA_ATTACK_FIRE_HOLD = 23,       // fire entries while B or fireHold, then KPA_A_1D
    KPA_ATTACK_FINISH = 24,          // timer, then back to standing
    KPA_ATTACK_SCRIPTED_START = 30,
    KPA_ATTACK_SCRIPTED_WINDUP_POSE = 40,
    KPA_ATTACK_SCRIPTED_WINDUP = 41,
    KPA_ATTACK_SCRIPTED_FIRE_START = 42,
    KPA_ATTACK_SCRIPTED_FIRE_HOLD = 43,
    KPA_ATTACK_SCRIPTED_FINISH = 44,
} KpaAttackState;

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
void kpaFireDelete(void);
void kpaMain(void);
s32 kpaNoHitEnemyCheck(void);
void kpaPauseSE2(void);
void kpaPreInit(void);
void kpaSetLevel(s32 lv);
void kpa_jumpNpc(void);
s32 kpaCamFollow(void);
s32 kpaFireAttackCheck(void);
s32 kpaHipAttackCheck(void);
f32 kpaSetJumpPara(void);
