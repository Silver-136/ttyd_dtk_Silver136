// cam_shift.c: the field camera shift (camShiftInit/camShiftMain), eased from Mario's horizontal stick input.

#pragma once

#include "driver/camdrv.h"
#include "mario/mario.h"
#include "dolphin/types.h"

typedef struct CamShiftWork {
    f32 current;        // 0x0
    f32 target;         // 0x4
    f32 rate;           // 0x8
    f32 stickAccum;     // 0xC
    f32 active;         // 0x10
    OSTime prevTime;    // 0x18
    f32 stickLimit;     // 0x20
    f32 maxSpeed;       // 0x24
    f32 accel;          // 0x28
    f32 lastX;          // 0x2C
    f32 lastZ;          // 0x30
} CamShiftWork; // size 0x38

void camShiftInit(void);
void camShiftMain(CameraEntry* camera, MarioWork* mario, f32* out);
void camShiftReset(void);

void camShiftPostMain(void);
CamShiftWork* camShiftGetWorkPtr(void);
