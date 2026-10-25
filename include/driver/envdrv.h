#pragma once

#include <dolphin/gx.h>
#include "memory.h"

typedef struct EnvironmentGlareTexCoord {
    f32 x; //0x0, screen pixels, scaled by coordScale / fbWidth
    f32 y; //0x4
} EnvironmentGlareTexCoord;

typedef struct EnvironmentGlare {
    GXBlendMode mode; //0x0
    GXBlendFactor srcFactor; //0x4
    GXBlendFactor dstFactor; //0x8
    GXTevKColorSel kcolorSel0; //0xC
    GXTevKColorSel kcolorSel1; //0x10
    f32 coordScale; //0x14
    s32 count; //0x18, number of coords that follors
    EnvironmentGlareTexCoord coords[]; //0x1C
} EnvironmentGlare;

typedef struct EnvironmentGlareFilter {
    u16 flags; //0x0
    u16 type; //0x2
    u16 y0; //0x4
    u16 y1; //0x6
    u16 x0; //0x8
    u16 x1; //0xA
    f32 zscale; //0xC
} EnvironmentGlareFilter;

typedef struct EnvironmentDof {
    u16 flags; //0x0, bit0 = pass 1 on, bit1 = pass 2 on
    u8 level1; //0x2, pass 1 konst color
    u8 level2; //0x3, pass 2 konst color
    f32 radius1; //0x4, pass 1 sample offset (4 taps)
    f32 radius2; //0x8, pass 2 sample offset
} EnvironmentDof; // two-pass depth-of-field style screen blur

typedef struct EnvironmentWork {
    s32 flags; //0x0
    smartEntry* capture; //0x4
    smartEntry* capture2; //0x8
    GXTexObj captureObj; //0xC
    GXTexObj captureObj2; //0x2C
    void* envBlur; //0x4C, see envBlurOn
    GXTexObj envBlurObj; //0x50
    u8 texMtxWork[0x88 - 0x70]; //0x70, EnvTexMtxWork view (view, mtx, model, scale, rotY, rotX)
    EnvironmentDof dof; //0x88
    u8 pad_94[0x98 - 0x94]; //0x94, never accessed
    u16 blurWork; //0x98, start of the EnvBlurWork view; bit 1 = blur on
    u8 blurWorkRest[0xB0 - 0x9A]; //0x9A, rest of the EnvBlurWork view
    EnvironmentGlareFilter filter; //0xB0
    u8 yamiView[0xF0 - 0xC0]; //0xC0, Mtx set by envSetYamiView
} EnvironmentWork;

void envInit(void);
void envReInit(void);
void envTexSetup(void);
void envMain(void);
void envBlurOn(s32 type, s32 msec);
void envBlurOff(void);
void envGlare(EnvironmentGlare* glare);
struct MapTevCallbackInfo;
void envSetWater(struct MapTevCallbackInfo* info);


void envGlareFilterZ(f32 zscale);
