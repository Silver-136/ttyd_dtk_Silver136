#pragma once

#include <dolphin/mtx.h>
#include "evt/evt_cmd.h"
#include "battle/battle_unit.h"
#include "battle/battle_database_common.h"

typedef struct BattleStageObjectData {
    char* name; //0x0
    u16 mSpawnSpecialUnitType; //0x4
    u16 mLayer; //0x8
    Vec mPosition; //0xC
    u8 mNumFramesToStartFalling; //0x14
    u8 mNumFramesToFall; //0x15
    u8 pad_16[2]; //0x16, TODO remove cuz compiler auto-pads
} BattleStageObjectData;

typedef struct BattleStageObject {
    s32 id; //0x0
    Vec mPosition; //0x4
    Vec positionOffset; //0x10, added to mPosition for mapObjTranslate
    Vec mRotation; //0x1C
    Vec positionOffset2; //0x28, second offset (btlevtcmd stage-object position)
    Vec prevPosition; //0x34, change detection
    Vec prevPositionOffset; //0x40
    Vec prevRotation; //0x4C
    Vec prevPositionOffset2; //0x58
    BattleStageObjectData* data; //0x64
    u32 mFlags; //0x68
    u32 mBattleUnitIdx; //0x6C
    u8 mNumFramesToStartFalling; //0x70
    u8 mNumFramesFalling; //0x71
    u8 reserved_72; //0x72, only set to 0
    u8 mShakePeriodLength; //0x73
    u8 mShakePeriodPosition; //0x74
    u8 pad_75[3]; //0x75, never accessed
    f32 mShakeAngle; //0x78
} BattleStageObject;

// one weapon of the stage tables; the tables initialise `weapon`
typedef union BattleWeaponData {
    BattleWeapon weapon;
    u32 raw[0xC0 / 4];
} BattleWeaponData;

typedef struct BattleStageNozzleData {
    u8 dirRate[3][4]; //0x0, weights for each nozzle's next direction
    u8 typeRate[4]; //0xC
    BattleWeaponData weapons[4]; //0x10, per nozzle type
} BattleStageNozzleData;

typedef struct BattleStageFallObjectData {
    BattleWeaponData frameWeapon; //0x0, falling iron frame
    u8 rates[9]; //0xC0
    BattleWeaponData weapons[9]; //0xCC
} BattleStageFallObjectData;

// BattleWork.stageObjectWork (0x180C0)
typedef struct BattleStageObjectWork {
    u8 pad_00[0x4 - 0x0]; //0x0, never accessed
    Vec bgRotation; //0x4
    Vec bgRotationPrev; //0x10
    u8 flags; //0x1C
    s32 bgDestroyEvtId; //0x20
    s32 bgEvtId[4]; //0x24
    BattleStageNozzleData* nozzleData; //0x34
    BattleStageFallObjectData* fallObjectData; //0x38
    BattleWeapon* nozzleWeapons[2]; //0x3C
    BattleWeapon nozzleWeaponWork[2]; //0x44
    u8 nozzleDir[3]; //0x1C4
    u8 nozzleNextDir[3]; //0x1C7
    s8 nozzleType; //0x1CA
    s32 nozzleTurnEvtId[3]; //0x1CC
    s32 nozzleWorkEvtId; //0x1D8
    s32 fogTimer; //0x1DC
    EffectEntry* fogEffect; //0x1E0
    s32 closeWallEvtId; //0x1E4
    s32 fallFrameEvtId; //0x1E8
    s32 objectFallEvtId; //0x1EC
} BattleStageObjectWork;

BattleStageObject* BattleGetObjectPtr(s32 id);
BattleStageObject* BattleSearchObjectPtr(const char* name);

void BattleStageObjectMain(void);





USER_FUNC(_set_mobj_shake_init);


void BattleStageObjectInit(void);

extern EvtWord _close_wall_event[];
extern BattleUnitSetup _entry_switch_data;
extern BattleUnitSetup _entry_tree_data;
extern EvtWord _fall_frame_event[];
extern EvtWord _nozzle_turn_event[];
extern EvtWord _nozzle_work_event_0[];
extern EvtWord _nozzle_work_event_1[];
extern EvtWord _nozzle_work_event_core[];
extern EvtWord _object_fall_event[];
extern BattleStageObjectData _object_nozzle_data[];
extern BattleStageObjectData _object_spot_light_data[];
extern EvtWord _object_turn_event_A1[];
extern EvtWord _object_turn_event_A2[];
extern EvtWord _object_turn_event_B[];
extern EvtWord _object_turn_event_core[];
extern BattleStageObjectData _object_wall_data;
extern void* jumptable_80374848[];

BOOL BattleFogForceStop(void);
void BattleStage_DestroyA1(void);
void BattleStage_DestroyA2(void);
void BattleStage_DestroyB(void);
void BattleStage_IronFrameFallCheck(void);
void BattleStage_NozzleDirChangeCheck(void);
void BattleStage_NozzleWorkCheck(s32 type);
void BattleStage_WallCloseCheck(void);
BOOL BattleFogEndCheck(void);
s32 BattleStage_ObjectFallCheck(void);
