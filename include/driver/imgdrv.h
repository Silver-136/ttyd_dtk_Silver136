#pragma once

#include "driver/camdrv.h"
#include "memory.h"
#include <dolphin/mtx.h>
#include <dolphin/gx.h>

typedef struct ImageRect {
    u16 left; //0x0
    u16 top; //0x2
    u16 width; //0x4
    u16 height; //0x6
} ImageRect;

// ImageShadow.flags (evt_img capture setup, imgCapture)
#define IMG_SHADOW_ACTIVE (1 << 2)        // a capture is set up for this shadow
#define IMG_SHADOW_PENDING (1 << 4)       // the capture has not been taken yet
#define IMG_SHADOW_COPY_PENDING (1 << 8)  // allocMode: the captured texture still has to be copied

typedef struct ImageShadow {
    s32 flags;        //0x0, IMG_SHADOW_*
    u16 x;            //0x4
    u16 y;            //0x6
    ImageRect rect;   //0x8, capture rectangle
    s32 offscreenId;  //0x10, offscreen id, -1 = none
    s32 copyFmt;      //0x14, capture copy format
    s32 texFmt;       //0x18, texture format
    GXTexObj texObj;  //0x1C
    smartEntry* data; //0x3C
    s32 allocMode;    //0x40, 1/2: smartAlloc heap for the captured copy
} ImageShadow;

// ImageEntry.flags (imgdrv)
typedef enum ImageEntryFlags {
    IMAGE_FLAG_IN_USE = (1 << 0),           // 0x1
    IMAGE_FLAG_HIDE = (1 << 1),             // 0x2, evt_img_onoff
    IMAGE_FLAG_OFFSCREEN_RECT = (1 << 2),   // 0x4
    IMAGE_FLAG_NO_VIRTUAL_POINT = (1 << 3), // 0x8, imgClearVirtualPoint
    IMAGE_FLAG_NO_CAPTURE = (1 << 28),      // 0x10000000
} ImageEntryFlags;

typedef struct ImageEntry {
    ImageShadow shadows[3]; //0x0
    u32 flags; //0xCC, ImageEntryFlags
    s32 type; //0xD0
    char name[16]; //0xD4
    u16 width; //0xE4
    u16 height; //0xE6
    f32 offsetX; //0xE8, screen offset (evt_img_set_position)
    f32 offsetY; //0xEC
    Vec point; //0xF0, virtual point
    f32 order; //0xFC
    s32 shadowId; //0x100
    s32 poseId; //0x104
    const char* animName; //0x108, anim to play, NULL = none
    u8 pad_10C[0x110 - 0x10C]; //0x10C, never accessed
    OSTime animTime; //0x110, animTimeGetTime() at anim start
    f32 loopTimes; //0x118, animPoseGetLoopTimes result
    f32 offsetZ; //0x11C
    Mtx mtx; //0x120, extra transform
    CameraId camId; //0x150
    s32 dispCamId; //0x154, camera drawn on (default CAMERA_3D)
} ImageEntry;

typedef struct ImageWork {
    s32 count; //0x0
    ImageEntry* entries; //0x4
} ImageWork;

void imgInit(void);
void imgAutoRelease(s32 type);
s32 imgEntry(const char* name, s32 type);
void imgMain(void);
ImageEntry* imgNameToPtr(const char* name, s32 type);
void imgFreeCapture(ImageEntry* entry, s32 shadowId);
void imgSetVirtualPoint(ImageEntry* entry, const Vec* point);
void imgClearVirtualPoint(ImageEntry* entry);
void imgRelease(ImageEntry* entry, s32 type);
void imgSetShadow(ImageEntry* entry, s32 shadowId);
