#pragma once

// Work slots of unit_bomhei, shared by its copies in the jon, tou2 and tst RELs.

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_SPARK_SFX 0 // sound id of the looping SFX_ENM_BOMB_WAIT1 that wait_event_spark starts once lit; -1 = none; bomb_event stops it
