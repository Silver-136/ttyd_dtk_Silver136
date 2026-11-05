#pragma once

// Work slots of unit_wanwan, shared by its copies in the jon, las and tst RELs.

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_SOUND_VOLUME 1 // volume of the wait sounds (SFX_ENM_WAN_*): 127 at init and each attack, sound_gensui_event lowers it to 70 % per loop, at least 32
#define UNIT_WORK_CHAIN_STATE 2  // chain_main mode: 0 the links hang free between parts 1 and 9; 2 records each link's offset from part 9 and becomes 1, which keeps them there (damage, sleep, stop and freeze)

// BattleUnitPart.work slots (btlevtcmd_Get/SetPartsWork)
#define PARTS_WORK_OFFSET_X 0 // parts 1-8: x offset from part 9 recorded by chain_main for UNIT_WORK_CHAIN_STATE 1
#define PARTS_WORK_OFFSET_Y 1 // parts 1-8: y offset from part 9 recorded by chain_main for UNIT_WORK_CHAIN_STATE 1
