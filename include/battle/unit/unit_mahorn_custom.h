#pragma once

// Work slots of unit_mahorn_custom, shared by its copies in the jon and tst RELs.

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_BUNSIN 4       // 1 for a clone made by spawn_event (unit_mahorn_custom_bunsin); the clone skips attacks and dies on any hit or miss
#define UNIT_WORK_KILL_BUNSIN 5  // 1 when it was hit while clones exist; after the damage event mahorn_subset_bunsin_all_kill removes them
#define UNIT_WORK_FADING 6       // clone: 1 once bunsin_kill_event has started its fade-out (alpha_down_event), so a second kill skips the fade
#define UNIT_WORK_MAGIC_EFFECT 7 // EffectEntry* of the magic bolt (effMahorn2Entry); eff_magic_get_posX reads the bolt's x from it
