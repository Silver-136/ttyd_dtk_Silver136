#pragma once

#include <dolphin/mtx.h>
#include "party.h"

struct HitEntry;

typedef struct PartyCloudSweep {
    f32 angle;   // 0x0, swings -20..20 degrees around breathDir
    s32 reverse; // 0x4, 0 = angle rising, 1 = falling
} PartyCloudSweep;

/* Flurrie's Gale Force work: PartyEntry.useStruct, __memAlloc(HEAP_DEFAULT, 0x48) + memset in _cloud_use */
typedef struct PartyCloud {
    f32 reserved_0;                      // 0x0, 100.0f at use start, never read
    f32 breathDir;                 // 0x4, Gale Force breath direction (toMovedir)
    f32 breathDist;                // 0x8, Gale Force breath reach, += 2 per frame
    f32 breathDistMax;             // 0xC, 120.0f; breathDist clamps here
    PartyCloudSweep sweep[2];      // 0x10, two side rays (cloudSweep)
    struct HitEntry* hitFront;     // 0x20, straight ray (getHitBreatheout2); cloudGetHitBreathout(0)
    struct HitEntry* hitDown;      // 0x24, -45 degree ray (_cloud_use case 12); cloudGetHitBreathout(1)
    struct HitEntry* hitSweep[2];  // 0x28, side rays; cloudGetHitBreathout(2), (3)
    s32 autoTimer;                 // 0x30, frames left when used by an event (flags & 0x80000000)
    s32 sfxId;                     // 0x34, breath sfx 0x930, -1 = none
    s16 breathLevel;               // 0x38, 0..2 cycles: ray height 8 / 20 / 40
    u8 pad3A[0x48 - 0x3A];         // 0x3A
} PartyCloud; // size 0x48

void cloudGetAt(Vec* position);
void cloudResetAt(void);
u8 cloudGetStatus(void);
f32 cloudGetBreathDir(void);
struct HitEntry* cloudGetHitBreathout(s32 which); // 0 front, 1 down, 2-3 the two sweep hits

void cloud_exit(PartyEntry* party);
void cloud_hover_init(PartyEntry* party);
void cloud_hover_main(PartyEntry* party);
void cloud_init(PartyEntry* party);
void cloud_move(PartyEntry* party);
void cloud_use(PartyEntry* party);
void cloudLockAnimationsOff(PartyEntry* party);
void cloudLockAnimationsOn(PartyEntry* party);
f32 cloudGetBreathDist(void);
void mot_cloud(void);
