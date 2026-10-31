// Declares the status icon structures and functions.

#pragma once

#include <dolphin/mtx.h>
#include <dolphin/gx.h>

typedef struct BattleWorkStatusIconEntry {
    s32 type; //0x0, status type
    s32 turns; //0x4
    u32 icon; //0x8, icon id (0 = none)
    Vec scale; //0xC
    Vec pivot; //0x18
    Vec rot; //0x24
    Vec trans; //0x30
    GXColor color; //0x3C
    s32 flags; //0x40, 1 = no backing plate
} BattleWorkStatusIconEntry;

typedef struct BattleWorkStatusIcon {
    Vec position; //0x0
    Vec screenSpaceOffset; //0xC
    f32 distanceBetweenIcons; //0x18
    s32 flags; //0x1C
    BattleWorkStatusIconEntry icons[28]; //0x20
    f32 dizzyAngle; //0x790, _bsi_guruguru: +4 per frame
    s16 numStatusIcons; //0x794
    s16 allergyBlinkTimer; //0x796, _bsi_allergy
    s16 hpRegenTimer; //0x798
    s16 fpRegenTimer; //0x79A
    s16 elecState; //0x79C, _bsi_biribiri state
    s16 elecTimer; //0x79E
} BattleWorkStatusIcon;

void BattleStatusIconInit(struct BattleUnit* unit);
void BattleStatusIconMain(struct BattleUnit* unit);
void BattleStatusIconDelete(struct BattleUnit* unit);

// Names from LLCOOLT's tree.
enum {
    STATUS_ICON_NO_PLATE = 1,
    STATUS_ICON_HIDDEN = 2,
};
