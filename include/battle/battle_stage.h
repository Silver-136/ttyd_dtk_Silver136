#pragma once

#include <dolphin/types.h>
#include "battle/battle_stage_object.h"
#include "battle/battle_database_common.h"

typedef struct BattleSpotEntry {
    u16 flags; // 0x0
    u8 pad_02[0x4 - 0x2]; // 0x2, never accessed
    Vec pos; // 0x4, cone position
    Vec rot; // 0x10, cone rotation, degrees
    Vec scale; // 0x1C, cone scale
    GXColor color; // 0x28, alpha drives the cone
} BattleSpotEntry;

typedef struct BattleSpotWork {
    s32 count; //0x0
    BattleSpotEntry* entries; //0x4
    BOOL skipRender; //0x8
} BattleSpotWork;

typedef struct BattleStageData {
    char* mGlobalStageDataDir; //0x0
    char* mSpecificStageDataDir; //0x4
    u32 mNumObjects; //0x8
    BattleStageObjectData* mObjects; //0xC
    BattleWeapon bgAWeapon; //0x10, background A1/A2 falling
    BattleWeapon bgBWeapon; //0xD0, background B falling
    void* initEvt; //0x190
    void* destroyBgA1Evt; //0x194
    void* destroyBgA2Evt; //0x198
    void* destroyBgBEvt; //0x19C
    void* bgA1Evt; //0x1A0
    void* bgA2Evt; //0x1A4
    void* bgBEvt; //0x1A8
    void* bgCEvt; //0x1AC
    s8 reserved_1B0; //0x1B0, 0 in all 58 data tables, never read
    s8 disableBgA1Destroy; //0x1B1
    s8 disableBgA2Destroy; //0x1B2
    s8 disableBgBDestroy; //0x1B3
} BattleStageData;

typedef struct StageLight {
    u32 flags;              // 0x0
    char name[0x20];        // 0x4
    GXColor color;          // 0x24
    GXColor colorStart;     // 0x28
    GXColor colorTarget;    // 0x2C
    s32 colorTime;          // 0x30
    s32 colorMax;           // 0x34
    s8 colorMode;           // 0x38
    f32 width;              // 0x3C
    f32 widthStart;         // 0x40
    f32 widthTarget;        // 0x44
    s32 widthTime;          // 0x48
    s32 widthMax;           // 0x4C
    s8 widthMode;           // 0x50
    Vec pos;                // 0x54
    Vec posStart;           // 0x60
    Vec posTarget;          // 0x6C
    s32 posTime;            // 0x78
    s32 posMax;             // 0x7C
    s8 posMode;             // 0x80
    Vec target;             // 0x84
    Vec targetStart;        // 0x90
    Vec targetTarget;       // 0x9C
    s32 targetTime;         // 0xA8
    s32 targetMax;          // 0xAC
    s8 targetMode;          // 0xB0
    Vec up;                 // 0xB4
    Vec upStart;            // 0xC0
    Vec upTarget;           // 0xCC
    s32 upTime;             // 0xD8
    s32 upMax;              // 0xDC
    s8 upMode;              // 0xE0
    void* data;             // 0xE4
} StageLight;

typedef struct StageDark {
    s32 flags;      // 0x0
    u8 cur;         // 0x4
    u8 base;        // 0x5
    u8 start;       // 0x6
    u8 baseStart;   // 0x7
    u8 target;      // 0x8
    u8 baseTarget;  // 0x9
    u8 pad_a[2];    // 0xA
    s32 time;       // 0xC
    s32 baseTime;   // 0x10
    s32 max;        // 0x14
    s32 baseMax;    // 0x18
    s8 mode;        // 0x1C
    s8 baseMode;    // 0x1D
} StageDark;

typedef struct BattleWorkStage {
    s32 reserved_00; //0x0, only set to 0
    StageLight lights[12]; //0x4
    StageDark stgDark; //0xAE4, stage darkening
    StageDark audDark; //0xB04, audience darkening
    f32 stageSizeX; //0xB24, btlevtcmd_GetStageSize (init 290)
    f32 stageSizeY; //0xB28 (init 200)
    f32 stageSizeZ; //0xB2C (init 90)
    f32 audienceSizeX; //0xB30, btlevtcmd_GetStageAudienceAreaSize (init = stageSizeX)
    f32 audienceSizeY; //0xB34, also the floor height past stageSizeZ (init -50)
    f32 audienceSizeZ; //0xB38 (init 200)
} BattleStage;

BattleStage* BattleStageGetPtr(void);



void BattleStageInit(void);
void BattleStageMain(void);
void BattleStageEnd(void);
BattleSpotWork* BatSpotGetPtr(void);
void BattleStageDisp(void);
s32 BattleStageGetLightNumberByName(const char* name);
void BattleStageOffLightEnemyInCommand(void);
void BattleStageOffLightFriendInCommand(void);
void BattleStageOnLightEnemyInCommand(void);
void BattleStageOnLightFriendInCommand(void);
void BattleStageOnLightInCommand(void);
s32 BattleStageSysLightEntry(const char* name);
void BattleStageLightRelease(s32 idx, s32 time, u8 mode);
void BattleStageLightSetLightColor(s32 idx, u8 r, u8 g, u8 b, s32 time, u8 mode);
void BattleStageLightSetPosition(s32 idx, f32 x, f32 y, f32 z, s32 time, u8 mode);
void BattleStageLightSetTarget(s32 idx, f32 x, f32 y, f32 z, s32 time, u8 mode);
void BattleStageLightSetTargetByUnit(s32 idx, s32 unitId, f32 x, f32 y, f32 z, s32 time, u8 mode);
void BattleStageLightSetUpVec(s32 idx, f32 x, f32 y, f32 z, s32 time, u8 mode);
void BattleStageLightSetWidth(s32 idx, s32 width, s32 time, u8 mode);
void BattleStageOffLightInCommand(s32 a, s32 b);
