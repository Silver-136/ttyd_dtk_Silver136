#pragma once

// Work slots of unit_sambo, shared by its copies in the eki, jon, tou2 and tst RELs.

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_BODY_COUNT 0 // body segments left (4 at init; -1 per segment lost in damage_event_bodylost, damage_event_bodylost_blow, damage_swallow_body or thrown by attack_spike_event); picks the pose/damage part (1 -> part 4 ... 4 -> part 1), the spike weapon and the height in _body_param_set
#define UNIT_WORK_CALL_DEPTH 1 // 0 for units from the battle setup; call_friend_event gives the unit it calls this value + 1; attack_event only calls a friend (30 %, all 4 segments left) while it is <= 1
