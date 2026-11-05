#pragma once

// Work slots of unit_basabasa_green, shared by its copies in the jon and tst RELs.

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_START_FLYING 0 // 1 (from the setup, all jon setups): init_event runs ceil_sky_event so it starts flying instead of hanging from the ceiling
