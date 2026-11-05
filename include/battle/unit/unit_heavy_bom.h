#pragma once

// Work slots of unit_heavy_bom, shared by its copies in the dou, jon, las and tst RELs.

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_FUSE_LIT 0   // 1 once attack_event lit it (with BATTLE_UNIT_FLAG_FUSE_LIT_UNUSED); freeze puts it out; phase_event counts down only while 1; _fire_point_callback draws the flame only while 1
#define UNIT_WORK_COUNTDOWN 1  // countdown 3, 2, 1, 0 (phase_event steps it while lit; at 0 the next phase explodes); picks pose tables, G_1-G_4 poses and the flame locator
#define UNIT_WORK_FUSE_SFX 2   // sound id of the looping SFX_ENM_HEAVYB_WAIT1 started when lit; -1 = none; stopped by freeze, death and explosion
#define UNIT_WORK_GUARD_STEP 3 // which guard weapon the next already-lit turn uses: 0 weapon_heavy_bomb_guard, 1 _guard_2, 2 _guard_3 (cycles)
