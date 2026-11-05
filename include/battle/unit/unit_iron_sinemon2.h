#pragma once

// Work slots of unit_iron_sinemon2, shared by its copies in the tou2 and tst RELs.

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_GREEN 0    // 1 on the green Iron Cleft (init_event2), 0 on the red one (init_event1); only the red one runs the phase and damage messages
#define UNIT_WORK_OTHER_ID 1 // unit id of the other Iron Cleft (get_id); it speaks the second line of the phase and damage messages
