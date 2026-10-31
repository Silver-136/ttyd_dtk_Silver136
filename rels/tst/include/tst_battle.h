// Views of BattleUnit and BattleWork at the older layout offsets the tst REL was built against.
#pragma once

#include "dolphin/types.h"
#include "battle/battle_ac.h"
#include "battle/battle_stage_object.h"
#include "version.h"

struct BattleWeapon;

// tst.rel was built against older battle headers than the PAL game it ships with, so a few of its units read the
// battle structs at other offsets. These views give those fields their tst offsets.
// - BattleUnit: tst's has no PAL slideReturnEventId (the US layout), so countersetWeapon and the fields after it sit 4 bytes
//   lower (countersetWeapon 0x314, not PAL 0x318). The JP demo's tst has the JP layout, 4 bytes lower again (0x310).
// - BattleWork: tst's places stageObjectWork 0x2A0 further in, at 0x1837C (PAL and US 0x180DC).
typedef struct TstBattleUnit {
#if VERSION <= VERSION_DEMO_US
    u8 pad_000[0x304]; // 0x0, as the JP BattleUnit
#else
    u8 pad_000[0x308]; // 0x0, as BattleUnit without slideReturnEventId
#endif
    ItemType heldItem;                      // 0x308 (JP demo 0x304)
    ItemDropData* dropTable;                // 0x30C (JP demo 0x308)
    u8 stolenCoins;                         // 0x310 (JP demo 0x30C)
    u8 moveColorLv;                         // 0x311 (JP demo 0x30D)
    u8 moveColorLvDisp;                     // 0x312 (JP demo 0x30E)
    u8 pad_313;                             // 0x313 (JP demo 0x30F)
    struct BattleWeapon* countersetWeapon;  // 0x314 (JP demo 0x310)
    void (*deleteFunc)(struct BattleUnit*); // 0x318 (JP demo 0x314)
    struct smartEntry* screenTexBuf;        // 0x31C (JP demo 0x318)
} TstBattleUnit;

typedef struct TstBattleWork {
    u8 pad_00000[0x1837C];                 // 0x0, as BattleWork in tst's headers
    BattleStageObjectWork stageObjectWork; // 0x1837C
} TstBattleWork;

#define TST_BATTLE_UNIT(unit) ((TstBattleUnit*)(unit))
#define TST_BATTLE_WORK(work) ((TstBattleWork*)(work))

#if VERSION <= VERSION_DEMO_US
// The JP demo's tst places BattleWork's actionCommands 0x2A0 lower than the game, at 0x19F0 (main 0x1C90).
typedef struct TstDemoBattleWork {
    u8 pad_00000[0x19F0];         // 0x0
    BattleActions actionCommands; // 0x19F0
} TstDemoBattleWork;

#define TST_DEMO_BATTLE_WORK(work) ((TstDemoBattleWork*)(work))
#endif
