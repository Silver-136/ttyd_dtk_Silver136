#pragma once

#include "dolphin/gx/GXStruct.h"
#include <dolphin/mtx.h>
#include <dolphin/gx.h>
#include "driver/camdrv.h"
#include "driver/effdrv.h"

typedef struct EffFunemizuWork {
    s32 type;      // 0x0
    Vec position;  // 0x4
    f32 size;      // 0x10, wake size (effFunemizuEntry size; mot_ship wakeScale); head: sets the wave scale targets and the splash spawn (> 0.1); splash drops: size/2, horizontal speed along yaw
    f32 yaw;       // 0x14, direction in degrees (effFunemizuEntry yaw); turns the wave mesh about y and moves the splash drops
    f32 wave0Width; // 0x18, x/z scale of the first wave mesh (dl_0); eases toward clamp(size) in effFunemizuMain
    f32 wave1Width; // 0x1C, x/z scale of the second wave mesh (dl_1); eases toward clamp(size - 1)
    f32 wave0Height; // 0x20, y scale of the first wave mesh; eases toward clamp(size)
    f32 wave1Height; // 0x24, y scale of the second wave mesh; eases toward clamp(size - 1)
    f32 wave0Scroll; // 0x28, texture x scroll of the first wave mesh (+= 0.05 * wave0Width per frame)
    f32 wave1Scroll; // 0x2C, texture x scroll of the second wave mesh (+= 0.1 * wave1Width per frame)
    s32 timer;     // 0x30
    f32 speedY;    // 0x34, splash drop vertical speed: added to position.y, reduced by gravity each frame
    f32 gravity;   // 0x38, splash drop: subtracted from speedY each frame (0.1 + irand(10)/100)
    s32 delay;     // 0x3C
    s32 alpha;     // 0x40
    f32 scale;     // 0x44
    CameraId cam;  // 0x48
} EffFunemizuWork;

GXColor effFunemizuGetMapColor(void);
EffectEntry* effFunemizuEntry(s32 type, f32 x, f32 y, f32 z, f32 size, f32 yaw);
