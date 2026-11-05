#pragma once

// Work slots of unit_hyper_jyugem, shared by its copies in the jon, tou2 and tst RELs.

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_HOLD 0          // 1 while it holds the thrown part (part 2) overhead: unison_phase_event sets it with the top-spiky counter and pose_table_hold; a throw, or a hit that does damage or stuns, clears it (damage_event drops the part)
#define UNIT_WORK_REBOUND_SPAWN 1 // 1: when the throw is guarded, the part rebounds to free space and becomes an entry_togezo unit (SFX_ENM_JUGEM_REBOUND1); 0: no rebound
#define UNIT_WORK_CHARGED 2       // 1 when charge is selected (weapon_jyugem_charge); after item-use and friend-spawn choices, attack_event throws without rebound spawning (never cleared)
