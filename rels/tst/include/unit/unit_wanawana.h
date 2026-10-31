// Wanawana (Pider-like trap unit): the paper texture capture shared by tou2 and its tst copy.
#pragma once

#include "battle/battle_unit.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <dolphin/types.h>

// Kept in the unit's countersetWeapon slot (BTL_ALLOC'd).
typedef struct WanaWork {
    s32 ready;            // 0x00, the texture has been captured
    s32 poseId;           // 0x04
    Mtx mtx;              // 0x08
    GXTexObj obj;         // 0x38
    BattleUnitPart* part; // 0x58
    smartEntry* tex;      // 0x5C, GXCopyTex destination (RGB5A3, half size)
} WanaWork;
