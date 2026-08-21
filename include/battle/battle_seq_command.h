#pragma once

#include "battle/battle_menu_disp.h"

#include <dolphin/types.h>

struct BattleWork;
struct BattleWorkUnit;

typedef enum BattleCommandFlags {
    COMMAND_ACT_MENU = (1 << 0),       // Display the main Action Menu
    COMMAND_WEAPON_MENU = (1 << 4),    // Display a Weapon Sub-Menu (Boots/Hammer/Item/Star)
    COMMAND_TACTICS_MENU = (1 << 8),   // Display the Tactics Sub-Menu
    COMMAND_PARTNER_MENU = (1 << 12),  // Display the Partner Sub-Menu
    COMMAND_SELECT_TARGET = (1 << 16), // Displays the menu for selecting an enemy/partner
    COMMAND_UNUSED = (1 << 24)         // Set in multiple places but never actually read
} BattleCommandFlags;

typedef struct BattleWorkCommandCursor {
    s32 absolutePos;
    s32 relativePos;
    s32 numOptions;
} BattleWorkCommandCursor;
STATIC_ASSERT_SIZEOF(BattleWorkCommandCursor, 0xC);

typedef struct BattleWorkCommand {
    s32 state;                           //0x0
    BattleCommandFlags flags;            //0x4
    u8 unk8[0x498 - 0x8];                //0x8
    BattleWorkCommandCursor cursors[14]; //0x498
    u8 unk540[0x55C - 0x540];            //0x540
    BattleWorkCommandMenu* menu;         //0x55C
    u8 unk560[0x574 - 0x560];            //0x560
} BattleWorkCommand;
STATIC_ASSERT_SIZEOF(BattleWorkCommand, 0x574);

void* BattleSetConfuseAct(struct BattleWork* battleWork, struct BattleWorkUnit* unit);
