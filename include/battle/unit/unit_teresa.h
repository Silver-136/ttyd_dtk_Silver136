#pragma once

// Work slots of unit_teresa, shared by its copies in the jin, jon and tst RELs.

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_RAISED 0 // 1 once move_event_sub has lifted it to y 40 out of hammer and shell-toss reach; move_event and teresa_check_move only count Boos still at 0
