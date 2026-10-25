#pragma once

#include "driver/imgdrv.h"
#include "sdk/texPalette.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>

typedef void (*OffscreenCallback)(CameraId cameraId, void* param);

typedef enum FadeType {
    FADE_IN_BLACK = 9,
    FADE_OUT_BLACK = 10,






    FADE_OUT_WHITE = 17,
    FADE_IN_WHITE2 = 18,





    TITLE_SCREEN_CURTAIN_LEAVE_GRADUAL2 = 39,

    MARIO_HEAD_FADE_IN = 52,
    MARIO_HEAD_FADE_OUT = 53,
} FadeType;

typedef struct FadeEntry {
    u16 flags; //0x0, 4 = use virtual pos
    u8 pad_2[2]; //0x2
    FadeType type; //0x4
    s32 state; //0x8, step within the fade type
    u8 pad_0C[0x10 - 0xC]; //0xC, never accessed
    s32 durationHi; //0x10, durationHi/Lo hold the s64 fade duration (FADE_DURATION)
    s32 durationLo; //0x14
    GXColor color; //0x18
    f32 fadeSpotX; //0x1C
    f32 fadeSpotY; //0x20
    f32 progress; //0x24, 0..1 (1 - elapsed/duration when fading in)
    f32 rotation; //0x28, degrees (z rotation of the fade texture)
    Mtx texMtx; //0x2C, built in the fade draw (entries[0] only), loaded as GX_TEXMTX0
    f32 elapsed; //0x5C, ms, +16.67 per frame
    const char* animFile; //0x60, e.g. "OFF_d_dokan_down"
    const char* animName; //0x64, e.g. "A_1"
    s32 imageType; //0x68
    s32 poseId; //0x6C, animPoseEntry handle, -1 = none
    s32 paperPoseId; //0x70, animPaperPoseEntry handle, -1 = none
    GXColor animColor; //0x74
    f32 animOffsetX; //0x78
    f32 animOffsetY; //0x7C
    Vec VirtualPos; //0x80
    ImageEntry* image; //0x8C
    f32 scale; //0x90, fade texture scale
    s32 scaleTimer; //0x94, frames into the scale interpolation
    s32 sfxId; //0x98, SFX started by the fade, psndSFXOff on finish
    OffscreenCallback callback; //0x9C
    void* offparam; //0xA0
    u32 delayFrames; //0xA4, frames to wait before the fade advances
} FadeEntry;

typedef struct FadeWork {
    u16 flags; //0x0
    u8 pad_2[2]; //0x2
    u32 pad_04; //0x4, never accessed
    FadeEntry entries[5]; //0x8
    s32 lastEntryId; //0x350, entry index used by the last fadeEntry
    f32 softness; //0x354, 0 after fadeTecSoftOn, 1 after fadeTecSoftOff
    TEXPalette* texture; //0x358
    BOOL loaded; //0x35C
} FadeWork;

void fadeInit(void);
void fadeTexSetup(void);
void fadeEntry(s32 type, s32 duration, GXColor color);
void fadeTecOn(void);
void fadeTecOff(void);
void fadeTecSoftOn(void);
void fadeTecSoftOff(void);
void fadeSoftFocusOn(void);
void fadeSoftFocusOff(void);
void fadeWhiteOutOn(void);
void fadeWhiteOutOff(void);
void fadeSetSpotPos(f32 x, f32 y);
void fadeSetAnimOfsPos(f32 x, f32 y);
void fadeSetOffscreenCallback(OffscreenCallback callback, void* param);
void fadeMain(void);
BOOL fadeIsFinish(void);
void fadeReset(s32 id);
TEXPalette* fadeGetTpl(void);
void fadeSetNarrowFast(void);

#ifdef VERSION_G8ME01
extern const f32 float_1_8041f770;
#endif
