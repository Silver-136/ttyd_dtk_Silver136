#pragma once

#include <dolphin/types.h>
#include <dolphin/gx.h>
#include <dolphin/mtx.h>

// one pose of an extEntry/evt_ext_entry table
typedef struct ExtTexInfo {
    const char* group; // 0x0
    const char* anim;  // 0x4
    f32 frame;         // 0x8
} ExtTexInfo;

// ExtdrvPose.flags (extMain, extDraw, extDrawShadow; punidrv's bit 2 differs)
typedef enum ExtPoseFlags {
    EXT_POSE_FLAG_VISIBLE = (1 << 0),    // 0x1, drawn and sorted
    EXT_POSE_FLAG_BILLBOARD = (1 << 1),  // 0x2, extMain turns it toward the camera
    EXT_POSE_FLAG_NO_SHADOW = (1 << 2),  // 0x4, extDrawShadow skips it (evt_nannpc_ppflag_onoff)
    EXT_POSE_FLAG_SHADOW_POS = (1 << 4), // 0x10, shadow at the fixed shadowY (evt_nannpc_set_shadow_position)
} ExtPoseFlags;

typedef struct ExtdrvPose {
    u32 flags;     // 0x0, ExtPoseFlags
    f32 dir;       // 0x4
    f32 shadowY;   // 0x8
    Mtx* mtx;      // 0xC
    s32 texId;     // 0x10
    f32 ry;        // 0x14
    u8 pad_18[4];   // 0x18
    f32 targetRy;  // 0x1C
} ExtdrvPose;

typedef struct ExtdrvTex {
    void** image;       // 0x0
    GXTexObj obj;       // 0x4
    GXTexRegion region; // 0x24
} ExtdrvTex;

typedef struct ExtdrvSort {
    s32 index; // 0x0
    f32 z;     // 0x4
} ExtdrvSort;

typedef struct ExtdrvWork {
    s32 poseNum;             // 0x0
    s32 texNum;              // 0x4
    ExtdrvPose* poses;       // 0x8
    ExtdrvTex* texs;         // 0xC
    Mtx* mtxs;               // 0x10
    f32* vtx;                // 0x14
    f32* normals;            // 0x18
    GXColor* colors;         // 0x1C
    f32* uvs;                // 0x20
    ExtdrvSort* sort;        // 0x24
    u8 (*dl)[0x80];          // 0x28, one 0x80-byte display list per texture
    u32* dlSize;             // 0x2C
    void (*mainFunc)(void);  // 0x30
    void (*mainFunc2)(void); // 0x34
    s32 curTex;              // 0x38
    Mtx shadowMtx;           // 0x3C
    u32 usedTmem;            // 0x6C
} ExtdrvWork; // size 0x70

void extInit(void);
void extMain(void);
void extReset(void);
void extEntry(s32 poseNum, ExtTexInfo* texInfo, void (*initFunc)(void), void (*mainFunc)(void), void (*mainFunc2)(void));
ExtdrvPose* extGetPosePtr(void);
s32 extGetPoseNum(void);
void extPoseDraw(s32 index);
void extDraw(void);
void extLoadRenderMode(void);
void extLoadVertex(void);
void extLoadTexture(void);
void extLoadTextureExit(void);
void extLoadTev(void);
void extLoadShadowVertex(void);
void extLoadShadowTev(void);
void extLoadShadowRenderMode(void);
void extLoadShadowMtx(Mtx mtx);
void extDrawShadow(void);