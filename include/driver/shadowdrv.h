#pragma once

#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include "memory.h"

typedef struct DepthShadowWork {
    s32 flags; //0x0, bit0 = drawn this frame
    s32 drawCount; //0x4
    s32 configIdx; //0x8, index into shadowConfig
    Mtx view; //0xC
    Mtx44 proj; //0x3C
    Mtx texMtx; //0x7C
    Mtx depthMtx; //0xAC
    GXTexObj texObj; //0xDC
    smartEntry* texBuffer; //0xFC, shadow texture memory
    GXColor color; //0x100
} DepthShadowWork;

typedef struct ProjShadowWork {
    s32 flags; //0x0, bit0 = drawn this frame
    s32 drawCount; //0x4
    s32 configIdx; //0x8, index into shadowConfig
    Mtx view; //0xC
    Mtx44 proj; //0x3C
    Mtx texMtx; //0x7C
    Mtx depthMtx; //0xAC
    GXTexObj texObj; //0xDC
    smartEntry* texBuffer; //0xFC, shadow texture memory
    GXColor color; //0x100
} ProjShadowWork;

typedef struct ShadowEntry {
    u16 flags; //0x0
    u8 mode; //0x2
    u8 type; //0x3
    Vec pos; //0x4
    f32 size; //0x10
    f32 sizeY; //0x14
    s16 offscreenId; //0x18
    s16 camId; //0x1A
    f32 volume; //0x1C
    f32 decay; //0x20
    f32 decay2; //0x24
    u8 alpha; //0x28
    u8 pad_29[3]; //0x29
    Mtx mtx; //0x2C
} ShadowEntry;

typedef struct CharShadowWork {
    s32 flags; //0x0, bit0 = drawn this frame
    s32 drawCount; //0x4
    s32 configIdx; //0x8, index into shadowConfig
    Mtx view; //0xC
    Mtx44 proj; //0x3C
    Mtx texMtx; //0x7C
    Mtx depthMtx; //0xAC
    GXTexObj texObj; //0xDC
    smartEntry* texBuffer; //0xFC, shadow texture memory
    GXColor color; //0x100
    s32 numEntries; //0x104, ShadowEntry count this frame
    ShadowEntry* entries; //0x108
    s32 projection; //0x10C, shadowGetCharShadowProjection
    s32 defaultMode; //0x110, mode given to new entries
    f32 defaultSizeY; //0x114, sizeY given to new entries
    s32 defaultEnabled; //0x118, 1 at init: new shadow entries start with flag 1
} CharShadowWork;

typedef struct ShadowConfig {
    s32 follow; //0x0
    Vec pos; //0x4
    Vec up; //0x10
    Vec target; //0x1C
    f32 left; //0x28
    f32 top; //0x2C
    f32 near; //0x30
    f32 far; //0x34
    s32 projType; //0x38
    u32 format; //0x3C
    s32 sizeIdx; //0x40
    f32 offset; //0x44
} ShadowConfig;

void shadowInit(void);
void shadowMain(void);

void shadowDepthShadowDrawCountUp(void);
s32 shadowEntry(f32 x, f32 y, f32 z, f32 size);
void shadowProjShadowDrawCountUp(void);
void shadowSetSize(s32 id, f32 size);

void U_shadowSetMode(s32 id, u8 alpha);
s32 shadowGetCharShadowProjection(void);
ShadowConfig* shadowGetDepthShadowConfig(void);
ShadowConfig* shadowGetProjShadowConfig(void);
s32 shadowEntryMode(s32 mode, f32 x, f32 y, f32 z, f32 size);
void shadowFlagOn(s32 id, u16 flags);
void shadowSetCollision(s32 id, BOOL enable);
void shadowSetType(s32 id, u8 type);
void shadowSetCamId(s32 id, s32 camId);
GXColor shadowGetCharShadowColor(void);
GXTexObj* shadowGetCharShadowTexObj(void);
GXColor shadowGetDepthShadowColor(void);
GXTexObj* shadowGetDepthShadowTexObj(void);
GXColor shadowGetProjShadowColor(void);
GXTexObj* shadowGetProjShadowTexObj(void);
GXTexObj* shadowGetRamp16TexObj(void);
GXTexObj* shadowGetRamp8TexObj(void);
