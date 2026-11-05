#pragma once

// Work slots of unit_giant_bomb, shared by its copies in the jon and tst RELs.

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_FUSE_LIT 0     // 1 once attack_event has lit the fuse (with BATTLE_UNIT_FLAG_FUSE_LIT_UNUSED); freeze damage puts it out; _fire_point_callback draws the flame only while set
#define UNIT_WORK_FUSE_LEFT 1    // fuse length 3..0 (init 3): picks pose tables, G_1..G_4 poses and the flame locator8..11; phase_event decrements it after the player phase while lit; a phase entered at 0 queues the explosion
#define UNIT_WORK_FUSE_SFX 2     // id of the looping SFX_ENM_HEAVYB_WAIT1 fuse sound, -1 none; switched off on freeze, death and explosion
#define UNIT_WORK_GUARD_WEAPON 3 // which self-targeted weapon the next lit-fuse turn uses: 0 weapon_heavy_bomb_guard, 1 _guard_2, 2 _guard_3, then back to 0
