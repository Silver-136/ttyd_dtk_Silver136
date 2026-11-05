#pragma once

// Work slots of unit_burst_wanwan, shared by its copies in the jon and tou2 RELs.

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_SOUND_VOLUME 1 // volume of its wait sounds (sound volume): 127 at init and each attack; sound_gensui_event reduces it each wait cycle to about 70 % (min 32)
#define UNIT_WORK_CHAIN_MODE 2   // how chain_main places the chain links: 2 record their shape relative to the stake (then 1), 1 keep that shape, 0 hang them between chomp and stake

// BattleUnitPart.work slots (btlevtcmd_Get/SetPartsWork)
#define PARTS_WORK_OFFSET_X 0 // chain links (parts 1-8): x offset from the stake (part 9) recorded in chain mode 2
#define PARTS_WORK_OFFSET_Y 1 // chain links (parts 1-8): y offset from the stake (part 9) recorded in chain mode 2
