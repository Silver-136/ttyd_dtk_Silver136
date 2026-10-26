#pragma once

/*
icon.bin structure:
s16 numEntries
s16 entryOffset[numEntries]
s16 IconBinEntry[6][numEntries] (?, width override? height override? numImages, tplId, iconDelay?)
2 extra bytes for some reason
*/

#include <dolphin/gx.h>
#include <dolphin/mtx.h>

#define numBinEntries 0x218

typedef struct IconBinaryEntry {
    u16 type;       // 0x0, 1 if animated
    u16 width;      // 0x2, overrides tpl size
    u16 height;     // 0x4, overrides tpl size
    u16 numIcons;   // 0x6, for simple anims like button press
    u16 tplId;      // 0x8, for tpl lookup
    u16 frameDelay; // 0xA, if animated, number of frames until next
} IconBinaryEntry;

typedef struct IconBinary {
    u16 count;                           // 0x0, numBinEntries
    u16 offset[numBinEntries];           // 0x2
    IconBinaryEntry data[numBinEntries]; // 0x432
} IconBinary;

// flags, 1 = in use, 0x10 = 2D, 0x1000 = animated(?), 0x2000 = ?(something to do with bin)

// IconEntry.flags (iconFlagOn/iconFlagOff)
typedef enum IconFlags {
    ICON_FLAG_ACTIVE = (1 << 0),             // 0x1, slot in use
    ICON_FLAG_HIDDEN = (1 << 1),             // 0x2, not drawn
    ICON_FLAG_FLIP_X = (1 << 2),             // 0x4
    ICON_FLAG_FLIP_Y = (1 << 3),             // 0x8
    ICON_FLAG_2D = (1 << 4),                 // 0x10, iconEntry2D: 2D camera, no light, fog or Z
    ICON_FLAG_GRAY = (1 << 5),               // 0x20, drawn in gray (channels mixed in equal parts)
    ICON_FLAG_NO_MAP_BLEND = (1 << 6),       // 0x40, ignores mapSetBlend (set by iconEntry while a blend is on)
    ICON_FLAG_ITEM_NO_MAP_BLEND = (1 << 7),  // 0x80, ignores mapSetBlend (set by itemdrv)
    ICON_FLAG_CAM_EFFECT_A = (1 << 8),       // 0x100, drawn by CAMERA_3D_EFFECTA
    ICON_FLAG_CAM_EFFECT_B = (1 << 9),       // 0x200, drawn by CAMERA_3D_EFFECTB
    ICON_FLAG_CULL_BACK = (1 << 10),         // 0x400
    ICON_FLAG_CULL_FRONT = (1 << 11),        // 0x800
    ICON_FLAG_ANIMATED = (1 << 12),          // 0x1000, frames advance (icon data type bit 0)
    ICON_FLAG_DATA_PENDING = (1 << 13),      // 0x2000, the icon data was not loaded yet at the entry
} IconFlags;

typedef struct IconEntry {
    union {
        s16 flags;    // 0x0, IconFlags
        u16 uflags;   // 0x0, unsigned view (icondrv uses lhz)
    };
    u8 pad_02;     // 0x2, never accessed
    Vec position;     // 0x4
    f32 scale;        // 0x10
    GXColor color;    // 0x14
    char name[16];    // 0x18
    u16 iconId;       // 0x28
    u16 width;        // 0x2A
    u16 height;       // 0x2C
    u16 numIcons;     // 0x2E
    u16 currIcon;     // 0x30, if animated, currently displayed icon (starts at 0)
    u16 tplId;        // 0x32
    u16 frameDelay;   // 0x34
    u8 pad_36[2]; // 0x36, never accessed
} IconEntry;

typedef struct IconWork {
    s32 count;          // 0x0
    IconEntry* entries; // 0x4
} IconWork;

void iconInit(void);
void iconTexSetup(void);
void iconReInit(void);
void iconMain(void);
void iconEntry(const char* name, u16 iconId);
void iconEntry2D(const char* name, s16 iconId);
void iconDelete(const char* name);
void iconChange(const char* name, u16 iconId);
void iconDispGxAlpha(Vec* position, s16 flags, u16 iconId, u8 alpha, f32 scale);
void iconDispGx(Vec* position, s16 flags, u16 iconId, f32 scale);
void iconDispGx2(Mtx mtx, s16 flags, s16 iconId);
void iconDispGxCol(Mtx pos, s16 flags, s32 iconId, GXColor color);
#ifdef VERSION_G8ME01
void iconGetTexObj(GXTexObj* texObj, s32 iconId);
#else
void iconGetTexObj(GXTexObj* texObj, s16 iconId);
#endif
void iconGetWidthHeight(u16* width, u16* height, s16 iconId);
void iconGetWidthHight(u16* width, u16* height, s32 iconId);
IconEntry* iconNameToPtr(const char* name);
void iconSetPos(const char* name, f32 x, f32 y, f32 z);
void iconFlagOn(const char* name, s16 flags);
void iconFlagOff(const char* name, s16 mask);
void iconSetScale(const char* name, f32 scale);
void iconSetAlpha(const char* name, u8 alpha);
s32 iconNumberDispGx(Mtx mtx, s32 number, BOOL small, GXColor color);
s32 iconNumberDispGx3D(Mtx mtx, s32 number, BOOL small, GXColor color);
