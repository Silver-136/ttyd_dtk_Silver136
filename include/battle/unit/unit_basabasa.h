#pragma once

// Work slots of unit_basabasa, shared by its copies in the jin, tou2 and tst RELs.

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_START_FLYING 0 // 1 from the battle setup (unitWork) = starts in flight: init_event runs ceil_sky_event (off the ceiling, airborne)
