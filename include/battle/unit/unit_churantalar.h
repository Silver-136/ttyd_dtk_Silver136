#pragma once

#include "battle/unit/unit_piders.h"

// Work slots of unit_churantalar, shared by its copies in the jon and tst RELs. Slots 0-4 are unit_piders.h's
// (the same piders_* code runs both units); these name its three renzoku targets one slot each.

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_TARGET1_ID 5     // unit id of the first renzoku shot's target (piders_renzoku_attack_set_target); first of slots 5-7
#define UNIT_WORK_TARGET2_ID 6     // unit id of the second renzoku shot's target
#define UNIT_WORK_TARGET3_ID 7     // unit id of the third renzoku shot's target
#define UNIT_WORK_TARGET1_PART 8   // part id of the first target; first of slots 8-10 (UNIT_WORK_TARGET1_ID + 3)
#define UNIT_WORK_TARGET2_PART 9   // part id of the second target
#define UNIT_WORK_TARGET3_PART 10  // part id of the third target
#define UNIT_WORK_TARGET1_HIT 11   // 1 once the first target has been hit (UNIT_WORK_TARGET1_ID + 6); the last shot then takes the plain CheckDamage path
#define UNIT_WORK_TARGET2_HIT 12   // 1 once the second target has been hit
#define UNIT_WORK_TARGET3_HIT 13   // 1 once the third target has been hit
