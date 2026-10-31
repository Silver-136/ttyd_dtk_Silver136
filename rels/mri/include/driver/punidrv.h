// The Great Tree (mri): punidrv, the Puni billboard renderer's types and entry points.

#pragma once

#include <dolphin/types.h>
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include "driver/camdrv.h"

// Puni (pikmin-like followers in Pit of 100 Trials / Great Tree) billboard driver.
// Structurally a copy of the DOL extdrv.

typedef struct PunidrvTexInfo {
    const char* group; // 0x0
    const char* anim;  // 0x4
    f32 frame;         // 0x8
} PunidrvTexInfo;

typedef struct PunidrvPose {
    u32 flags;     // 0x0
    f32 dir;       // 0x4
    f32 rotZ;      // 0x8
    f32 offsetY;   // 0xC
    f32 shadowY;   // 0x10
    f32 scale;     // 0x14
    Mtx* mtx;      // 0x18
    s32 texId;     // 0x1C
    f32 ry;        // 0x20
    u8 pad_24[4];   // 0x24
    f32 targetRy;  // 0x28
} PunidrvPose;

typedef struct PunidrvTex {
    void** image;         // 0x0
    GXTexObj obj;         // 0x4
    GXTexRegion region;   // 0x24
} PunidrvTex;

typedef struct PunidrvSort {
    s32 index; // 0x0
    f32 z;     // 0x4
} PunidrvSort;

typedef struct PunidrvWork {
    s32 poseNum;            // 0x0
    s32 texNum;             // 0x4
    PunidrvPose* poses;     // 0x8
    PunidrvTex* texs;       // 0xC
    Mtx* mtxs;              // 0x10
    f32* vtx;               // 0x14
    f32* normals;           // 0x18
    GXColor* colors;        // 0x1C
    f32* uvs;               // 0x20
    PunidrvSort* sort;      // 0x24
    u8 (*dl)[0x80];                 // 0x28, one 0x80-byte display list per texture
    u32* dlSize;            // 0x2C
    void (*mainFunc)(void); // 0x30
    void (*mainFunc2)(void); // 0x34
    s32 curTex;             // 0x38
    Mtx shadowMtx;          // 0x3C
    u32 usedTmem;           // 0x6C
    u32 useFade;            // 0x70
} PunidrvWork; // size 0x74

void puniSinCosRet(u32 deg, f32* x, f32* z, f32 rx, f32 rz);
void puniInit(void);
void puniEntry(s32 poseNum, PunidrvTexInfo* texInfo, void (*initFunc)(void), void (*mainFunc)(void),
               void (*mainFunc2)(void));
void puniMakeTexture(CameraId camId, void* param);
void puniReset(void);
void puniFadeOff(BOOL useFade);
void puniMain(void);
PunidrvPose* puniGetPosePtr(void);
s32 puniGetPoseNum(void);
void puniPoseDraw(s32 index);
void puniDraw(void);
void puniLoadRenderMode(void);
void puniLoadVertex(void);
void puniLoadTexture(void);
u32 puniGetUsedTmemSize(void);
void puniLoadTextureExit(void);
void puniLoadTev(void);
void puniLoadShadowVertex(void);
void puniLoadShadowTev(void);
void puniLoadShadowRenderMode(void);
void puniLoadShadowMtx(Mtx mtx);
void puniDrawShadow(void);
