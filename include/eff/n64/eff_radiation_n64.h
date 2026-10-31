// eff_radiation_n64.c: radiating rays effect entry (effRadiationN64Entry) and its work data (EffRadiationN64Work).

#include "driver/camdrv.h"
#pragma once

#include "driver/effdrv.h"
#include "dolphin/types.h"
#include <dolphin/mtx.h>

typedef struct EffRadiationN64Work {
    s32 type;       // 0x0
    Vec pos0;       // 0x4
    Vec position;   // 0x10
    f32 scale1c;    // 0x1C
    f32 scale20;    // 0x20
    s32 alpha;      // 0x24
    s32 life;       // 0x28
    s32 frame;      // 0x2C
    f32 unk30;      // 0x30
    f32 scroll0;    // 0x34
    f32 scroll2;    // 0x38
    f32 scroll1;    // 0x3C
    f32 scroll3;    // 0x40
    f32 unk44;      // 0x44
    f32 scrollVel0; // 0x48
    f32 scrollVel2; // 0x4C
    f32 scrollVel1; // 0x50
    f32 scrollVel3; // 0x54
    f32 y;          // 0x58
    f32 rotX;       // 0x5C
    f32 rotY;       // 0x60
    f32 rotZ;       // 0x64
    f32 initY;      // 0x68
    f32 initYVel;   // 0x6C
    f32 initRotX;   // 0x70
    f32 initRotY;   // 0x74
    f32 initRotZ;   // 0x78
    s32 initLife;   // 0x7C
    f32 rotVelX;    // 0x80
    f32 rotVelY;    // 0x84
    f32 rotVelZ;    // 0x88
    f32 yVel;       // 0x8C
    s32 delay;      // 0x90
    s32 flag;       // 0x94
    CameraId camera;     // 0x98
} EffRadiationN64Work;

EffectEntry* effRadiationN64Entry(s32 type, f32 x, f32 y, f32 z, f32 scale);
