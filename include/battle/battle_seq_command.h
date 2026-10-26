#pragma once

#include <dolphin/types.h>

struct BattleWeapon;
struct BattleWork;
struct BattleUnit;

typedef struct BattleCommandCursor {
    s32 absPos; // 0x0, absolute cursor index
    s32 relPos; // 0x4, first visible row
    s32 count;  // 0x8, number of options
} BattleCursor;

typedef struct BattleCommandAct {
    s32 type;          // 0x0
    s32 enabled;       // 0x4
    const char* name;  // 0x8
    u16 icon;          // 0xC
    u8 padE[2];        // 0xE
    s32 disableReason; // 0x10
} BattleCommandAct;

typedef struct BattleCommandWeapon {
    struct BattleWeapon* weapon; // 0x0
    s32 enabled;                 // 0x4
    const char* name;            // 0x8
    u16 icon;                    // 0xC
    u8 padE[2];                  // 0xE
    s32 index;                   // 0x10
    s32 itemId;                  // 0x14
    s32 disableReason;           // 0x18
} BattleCommandWeapon;

typedef struct BattleCommandOperation {
    s32 type;          // 0x0
    s32 enabled;       // 0x4
    s32 disableReason; // 0x8
    const char* name;  // 0xC
    u16 icon;          // 0x10
    u8 pad12[2];       // 0x12
    const char* help;  // 0x14
    s32 cost;          // 0x18
} BattleCommandOperation;

typedef struct BattleCommandParty {
    s32 kind;          // 0x0
    s32 enabled;       // 0x4
    const char* name;  // 0x8
    u16 icon;          // 0xC
    u8 padE[2];        // 0xE
    const char* help;  // 0x10
    s16 hp;            // 0x14
    s16 maxHp;         // 0x16
} BattleCommandParty;

typedef struct BattleCommandMultiItem {
    s32 enabled;       // 0x0
    const char* name;  // 0x4
    const char* help;  // 0x8
    u16 icon;          // 0xC
    u8 padE[2];        // 0xE
    s32 cost;          // 0x10
    s32 disableReason; // 0x14
} BattleCommandMultiItem;

typedef struct BattleSubMenu {
    BattleCursor* cursor;           // 0x0
    BattleCommandAct* acts;         // 0x4
    BattleCommandWeapon* weapons;   // 0x8
    s32 scrollState;                    // 0xC
    s32 reserved_10;                   // 0x10
    u8 shown;                    // 0x14
    u8 pad_15[3];                    // 0x15
    s32 prevAbsPos;                   // 0x18
    s32 iconsArrived;                   // 0x1C
    s32 iconFlags[5];                // 0x20
    s32 iconState[5];                // 0x34
    s32 iconTimer[15];               // 0x48
    s32 iconSlot[5];                // 0x84
    f32 scrollPos;                   // 0x98
    f32 scrollFrom;                   // 0x9C
    f32 scrollTo;                   // 0xA0
    s32 rowFlags[7];                // 0xA4
    s32 rowState[7];                // 0xC0
    s32 rowTimer[7];                // 0xDC
    s32 reserved_F8[7];                // 0xF8
    u8 reserved_114[8];
    s32 bobTimer; // 0x11C
    f32 partyBtnAngle; // 0x120
} BattleSubMenu;

// BattleCommand.flags: the menus BattleCommandDisplay draws (set by each *MenuSetup, cleared by its *MenuEnd)
typedef enum BattleCommandFlags {
    CMD_MENU_ACT = (1 << 0),               // 0x1
    CMD_MENU_WEAPON_SELECT = (1 << 4),     // 0x10
    CMD_MENU_MULTI_ITEM = (1 << 5),        // 0x20
    CMD_MENU_OPERATION = (1 << 8),         // 0x100
    CMD_MENU_CHANGE_PARTY = (1 << 12),     // 0x1000
    CMD_MENU_TARGET_SELECT = (1 << 16),    // 0x10000
    CMD_MENU_PROTECT_PARTNER = (1 << 20),  // 0x100000
    CMD_MENU_ENEMY_HP = (1 << 24),         // 0x1000000, BattleDrawEnemyHP draws every enemy's HP
} BattleCommandFlags;

typedef struct BattleCommand {
    s32 state;                           // 0x0, command menu state (switch in the main loop)
    s32 flags;                           // 0x4, BattleCommandFlags
    BattleCommandAct act[6];             // 0x8
    BattleCommandWeapon weapon[21];      // 0x80
    BattleCommandOperation operation[7]; // 0x2CC
    BattleCommandParty party[8];         // 0x390
    BattleCommandMultiItem multiItem[3]; // 0x450
    BattleCursor cursor[14];             // 0x498
    s32 actType;                         // 0x540, type of the chosen act (act[cursor].type); 9 on cancel
    s32 prevActType;                     // 0x544, actType saved before it is forced to 12
    s32 operationType;                   // 0x548, BattleCommandOperation.type of the chosen tactic
    s32 partyKind;                       // 0x54C, party kind chosen in the change-partner menu
    u8 dipMode;                          // 0x550, Double/Triple Dip count (2/3), 1 while picking
    u8 cancelled;                        // 0x551, command cancelled: the unit just waits
    u8 pad_552[0x554 - 0x552];           // 0x552, never accessed
    s32 recCommand;                      // 0x554, state recorded by _commandRestoreRec (0 = none)
    u8 attackAudience;                   // 0x558, 1: BattleCommandAttackAudienceCheck fired, 2: reset pending
    u8 pad_559[0x55C - 0x559];           // 0x559, never accessed
    BattleSubMenu* subMenu;                       // 0x55C, battle_menu_disp.c
    s32 idleTimer;                       // 0x560, frames without input; idle camera at 900
    s32 disableReason;                   // 0x564, disableReason of the greyed-out option, picks the error message
    s32 errorEvtId;                      // 0x568, error message event id
    u8 pad_56C[0x574 - 0x56C];           // 0x56C, never accessed
} BattleCommand;
BOOL BattleCommandCheckChangePositionEnable(struct BattleWork* wp);
void BattleCommandDisplay(struct BattleWork* wp);
void BattleCommandInit(struct BattleWork* wp);
s32 BattleCommandInput(struct BattleWork* work);
struct BattleWeapon* BattleGetSelectWeapon(struct BattleWork* wp);
void* BattleSetConfuseAct(struct BattleWork* wp, struct BattleUnit* unit);
