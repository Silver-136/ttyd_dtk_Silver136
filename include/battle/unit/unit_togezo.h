#pragma once

// Work slots of unit_togezo, shared by its copies in the jon, tou2 and tst RELs.

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_GUARD 0      // 1 while in the guard stance (guard_event, or the attack ending in guard); guard_cancel_event clears it
#define UNIT_WORK_ROLLING 1    // 1 keeps the sibling loop that rolls it during the attack; 0 stops it
#define UNIT_WORK_ROLL_SPEED 2 // degrees per frame of that roll (initially -36 times the direction; ramped by 1 per step after a miss or hit)
