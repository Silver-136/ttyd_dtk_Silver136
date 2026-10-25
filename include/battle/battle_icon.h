#pragma once

#include <dolphin/types.h>
#include "driver/icondrv.h"
#include "evt/evt_cmd.h"

typedef struct BattleWorkIcon {
    u16 flags; //0x0
    u16 iconId; //0x2
    s32 index; //0x4, TODO: rename?
    Vec position; //0x8
    f32 scale; //0x14, init 1.0; iconSetScale(scale * scaleMul)
    f32 scaleMul; //0x18, init 1.0
    IconEntry* base; //0x1C
    Vec jumpStart; //0x20
    Vec jumpPos; //0x2C
    Vec jumpTarget; //0x38
    s32 jumpFrames; //0x44
    f32 jumpSpeed; //0x48
    f32 fallAccel; //0x4C
    f32 jumpVelY; //0x50
    f32 jumpAngle; //0x54
    f32 jumpDist; //0x58
    u8 pad_5C[4]; //0x5C, never accessed
    s8 jumpDir; //0x60
    u8 pad_61[0x9C - 0x61]; //0x61, never accessed
} BattleIcon;

void BattleIconInit(void);
void BattleIconMain(void);
void BattleIconEnd(void);
BattleIcon* BtlIcon_Entry(u16 iconId, f32 x, f32 y, f32 z);
void BtlIcon_Delete(BattleIcon* icon);

USER_FUNC(btlevtcmd_BtlIconDelete);
USER_FUNC(btlevtcmd_BtlIconEntry);
USER_FUNC(btlevtcmd_BtlIconEntryItemId);
USER_FUNC(btlevtcmd_BtlIconJumpPosition);
USER_FUNC(btlevtcmd_BtlIconSetFallAccel);
USER_FUNC(btlevtcmd_BtlIconSetPosition);
