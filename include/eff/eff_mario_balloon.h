// eff_mario_balloon.c: balloon effect entry (effMarioBalloonEntry) and its work struct (MarioBalloonWork).

#pragma once

#include "driver/effdrv.h"
#include "driver/camdrv.h"
#include <dolphin/mtx.h>

struct NpcEntry;

typedef struct MarioBalloonWork {
    s32 type;     // 0x0
    Vec position; // 0x4
    f32 rotation; // 0x10
    f32 scale;    // 0x14
    struct NpcEntry* npc; // 0x18, the speaker the balloon floats over (NULL = Mario)
    CameraId camera; // 0x1C
} MarioBalloonWork;

EffectEntry* effMarioBalloonEntry(s32 type, struct NpcEntry* npc);

