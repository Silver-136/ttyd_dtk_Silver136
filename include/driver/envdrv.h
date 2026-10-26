#pragma once

#include <dolphin/gx.h>
#include <dolphin/mtx.h>
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

// EnvironmentGlareFilter.flags
#define ENV_GLARE_FILTER_Z (1 << 0) // envGlareFilterZ: zscale applies

typedef struct EnvironmentGlareFilter {
    u16 flags; //0x0, ENV_GLARE_FILTER_Z
    u16 type; //0x2
    u16 y0; //0x4
    u16 y1; //0x6
    u16 x0; //0x8
    u16 x1; //0xA
    f32 zscale; //0xC
} EnvironmentGlareFilter;

#define ENV_DOF_PASS1 (1 << 0)
#define ENV_DOF_PASS2 (1 << 1)

typedef struct EnvironmentDof {
    u16 flags; //0x0, ENV_DOF_PASS1 / ENV_DOF_PASS2
    u8 level1; //0x2, pass 1 konst color
    u8 level2; //0x3, pass 2 konst color
    f32 radius1; //0x4, pass 1 sample offset (4 taps)
    f32 radius2; //0x8, pass 2 sample offset
} EnvironmentDof; // two-pass depth-of-field style screen blur

// EnvironmentWork.flags
typedef enum EnvironmentFlags {
    ENV_FLAG_CAPTURE_COLOR_UNUSED = (1 << 0), // 0x1, envCapture copies the screen colour; nothing sets it
    ENV_FLAG_CAPTURE_DEPTH_UNUSED = (1 << 1), // 0x2, envCapture copies the screen depth; nothing sets it
    ENV_FLAG_BLUR_CAPTURE = (1 << 2),         // 0x4, envCapture2 copies the screen for the blur
    ENV_FLAG_BLUR_CAPTURED = (1 << 3),        // 0x8, a blur frame was captured
    ENV_FLAG_FF_UNUSED = (1 << 28),           // 0x10000000, envDisp_FF draws; nothing sets it
    ENV_FLAG_GLARE_FILTER = (1 << 29),        // 0x20000000, envGlareFilter with a type
    ENV_FLAG_BLUR = (1 << 30),                // 0x40000000, envBlurOn
} EnvironmentFlags;
#define ENV_FLAG_DOF_UNUSED 0x80000000 // envDisp_DepthOfField draws; nothing sets it

// EnvironmentWork.texMtxWork: matrices and parameters envTevLoadTexMtxImm reads while setting up texture matrices
typedef struct EnvTexMtxWork {
    MtxPtr view;  // 0x0
    MtxPtr mtx;   // 0x4
    MtxPtr model; // 0x8
    f32 scale;    // 0xC
    f32 rotY;     // 0x10
    f32 rotX;     // 0x14
} EnvTexMtxWork;

// EnvironmentWork.blur (envBlurOn, envDisp_Blur)
typedef struct EnvBlurWork {
    u16 flags;     // 0x0, bit 0: blur on
    u8 pad_02[6];  // 0x2, never accessed
    OSTime time;   // 0x8
    s32 msec;      // 0x10
    u8 pad_14[4];  // 0x14, never accessed
} EnvBlurWork;

typedef struct EnvironmentWork {
    s32 flags; //0x0, EnvironmentFlags
    smartEntry* capture; //0x4
    smartEntry* capture2; //0x8
    GXTexObj captureObj; //0xC
    GXTexObj captureObj2; //0x2C
    void* envBlur; //0x4C, see envBlurOn
    GXTexObj envBlurObj; //0x50
    EnvTexMtxWork texMtxWork; //0x70
    EnvironmentDof dof; //0x88
    u8 pad_94[0x98 - 0x94]; //0x94, never accessed
    EnvBlurWork blur; //0x98
    EnvironmentGlareFilter filter; //0xB0
    Mtx yamiView; //0xC0, set by envSetYamiView
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
void envAddTev(s32 type, struct MapTevCallbackInfo* info);


void envGlareFilterZ(f32 zscale);
void envSetYamiView(Mtx view);
