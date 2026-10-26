#pragma once

#include "mario/mario_pouch.h"
#include <dolphin/types.h>

typedef struct BattleAudienceItemKind {
    ItemType type; // 0x0
    s32 chance;    // 0x4
} BattleAudienceItemKind;

/* seems unused, just static table size
typedef struct BattleAudienceAnimTable {
    s32* animWaitFront;     // 0x0
    s32* animWaitBack;      // 0x4
    s32* animJumpFront;     // 0x8
    s32* animJumpBack;      // 0xC
    s32* animRaiseBack;     // 0x10
    s32* animItemOnBack;    // 0x14
    s32* animItemThrowBack; // 0x18
    s32* animRunFront;      // 0x1C
    s32* animTumbleFront;   // 0x20
    s32* animDamageFront;   // 0x24
    s32* animSleepFront;    // 0x28
    s32* animSleepBack;     // 0x2C
    s32* animConfuseFront;  // 0x30
    s32* animConfuseBack;   // 0x34
    s32* animSingFront;     // 0x38, for Crazy Dayzees
    s32* animShellBack;     // 0x3C, for Koopas
    s32* animIgniteFront;   // 0x40, for Bulky Bob-Ombs
    s32* animIgniteBack;    // 0x44, for Bulky Bob-Ombs
    s32* animEatFront;      // 0x48, for Piranha Plants
} BattleAudienceAnimTable; */

#define AUDIENCE_KIND_WIDE (1 << 0) // BattleAudienceKind.flags: takes two seats (counts double in the type weights)

typedef struct BattleAudienceKind {
    u8 flags;                          // 0x0, AUDIENCE_KIND_WIDE
    s32** animTable;                   // 0x4
    BattleAudienceItemKind* itemTable; // 0x8
    f32 itemOffsetX;                   // 0xC, thrown-item spawn offset from the member
    f32 itemOffsetY;                   // 0x10, thrown-item spawn offset from the member
    f32 spWeight;                      // 0x14, star power per member of this kind (summed, then sqrt)
    f32 texScaleX;                     // 0x18, member quad scale per 64 texels
    f32 texScaleY;                     // 0x1C, member quad scale per 64 texels
} BattleAudienceKind;

extern BattleAudienceKind audience_kind[];
