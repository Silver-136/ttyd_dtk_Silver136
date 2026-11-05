#pragma once

// Work slots of unit_gold_chorobon, shared by its copies in the hei and tst RELs.

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_CHOROGUN_CALLED 0 // 1 once damage_event has called the chorogun unit (HP 1-7: entry_chorogun, call_chorogun_event)
#define UNIT_WORK_CHOROGUN_GONE 1   // 1 once the chorogun has been sent off (escape_chorogun_event, or damage_event when this unit drops to 0 HP while the chorogun is out); stops it running twice
#define UNIT_WORK_CHOROGUN 2        // unit id of the chorogun unit it called
