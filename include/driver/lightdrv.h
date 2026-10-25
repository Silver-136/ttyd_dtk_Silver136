#pragma once

#include <dolphin/gx.h>
#include <dolphin/mtx.h>

#define LIGHT_ENTRY_COUNT 32

#pragma enumsalwaysint off
typedef enum LightEntryFlags {
    LIGHT_ENABLED = 1 << 0,
    LIGHT_NO_DIST_ATTEN = 1 << 6,
    LIGHT_CHARA = 1 << 7,
    LIGHT_POINT = 1 << 12,
    LIGHT_SPOT = 1 << 13,
    LIGHT_DIRECTIONAL = 1 << 14,
    LIGHT_AMBIENT = 1 << 15
} LightEntryFlags;
#pragma enumsalwaysint on

typedef struct LightEntry {
    LightEntryFlags flags; //0x0
    char name[32]; //0x2
    u8 align_22[2]; //0x22
    Vec position; //0x24
    Vec rotation; //0x30
    Vec basePosition; //0x3C, position at lightEntry; evt_light offsets from it when following a map object
    GXColor color; //0x48
    f32 spotAngle; //0x4C
    f32 attenAngle; //0x50
    Vec attenDistCoef; //0x54
} LightEntry;

typedef struct LightWork {
    s32 count; //0x0
    LightEntry* entries; //0x4
} LightWork;

void lightInit(void);
void lightReInit(void);
void lightMain(void);
s32 lightEntry(const char* name, s32 type, Vec* position, Vec* rotation, const GXColor* color, f32 spotAngle, f32 attenAngle, s32 attenType, s32 flags);
LightEntry* lightNameToPtr(const char* name);
LightEntry* lightNumberToPtr(s32 index);
LightEntry* lightGetPaperCraft(void);
s32 lightGetEntryNum(void);
s32 lightGetNearObj(const Vec* position, LightEntry** out, s32 count, LightEntryFlags flags);





s32 lightCheckCharaLight(void);
LightEntry* lightGetAmbient(void);
LightEntry* lightGetCharaAmbient(void);
LightEntry* lightGetPaper(void);
