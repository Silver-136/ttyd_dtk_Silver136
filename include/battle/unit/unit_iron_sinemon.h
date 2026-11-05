#pragma once

// Work slots of unit_iron_sinemon, shared by its copies in the tou2 and tst RELs.

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_SILENT 0  // 1 skips the damage and phase talk events (init_event2, used by the green copy); 0 for this one (init_event1)
#define UNIT_WORK_TWIN_ID 1 // unit id of the other Iron Cleft (get_id: the other UNIT_IRON_CLEFT_* kind), who speaks the reply lines
