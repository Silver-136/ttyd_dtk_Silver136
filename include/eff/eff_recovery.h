// eff_recovery.c: HP/FP recovery effect entry (effRecoveryEntry) and its per-icon work data (RecoveryWork).

#pragma once

#include "version.h"
#include "driver/effdrv.h"
#include "dolphin/types.h"
#include <dolphin/mtx.h>

struct MayaAnim;

typedef struct RecoveryWork {
    s8 type;         // 0x0
    s8 amount;       // 0x1
    s8 state;        // 0x2
    s32 alpha;       // 0x4
    Vec position;    // 0x8
    Vec velocity;    // 0x14
    Vec scale;       // 0x20
    s16 timer;       // 0x2c
    f32 angle;       // 0x30
    u32 tick;        // 0x34
    struct MayaAnim* anim; // 0x38
    s32 delay;       // 0x3c
#if VERSION >= VERSION_JP
    f32 baseY;       // 0x40
#endif
} RecoveryWork;

EffectEntry* effRecoveryEntry(s32 type, s32 amount, f32 x, f32 y, f32 z);
