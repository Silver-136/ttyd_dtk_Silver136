#pragma once

// Work slots of unit_sambo_mummy, shared by its copies in the eki, jon and tst RELs.

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_BODY_COUNT 0      // stack height 1-4 (4 at start, 1 = head only): picks the pose part, height 20-80 and weapon variant; losing a body or the spike attack takes 1
#define UNIT_WORK_CALL_GENERATION 1 // how many calls deep it was summoned: call_friend_event gives the new unit the caller's value + 1; only units at <= 1 call
