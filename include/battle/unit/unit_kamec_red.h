#pragma once

// Work slots of unit_kamec_red, shared by its copies in the tou2 and tst RELs.

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_SKY 0          // nonzero: flying form (pose_table_sky, airborne at y 40, sky magic events); setup value (0 in the one tou setup), copied to clones, cleared by dmg_fall_event
#define UNIT_WORK_BUNSIN 1       // 1 on a copy made by spawn_event (kClone part, data_table_bunsin, no own attack or wait animation, removed by any hit)
#define UNIT_WORK_KILL_BUNSIN 2  // 1 when the real unit is hit while copies exist; after the damage, kamec_subset_bunsin_all_kill removes the copies
#define UNIT_WORK_FADING 3       // 1 once bunsin_kill_event has started fading this copy out, so a second call skips the fade
#define UNIT_WORK_STAFF_EFFECT 4 // EffectEntry* of the wand charge effect (eff_staff1 thunderflare, eff_staff2 pokopi charge) kept on the wand tip
#define UNIT_WORK_MAGIC_EFFECT 5 // EffectEntry* of the last magic shot (effMagic1N64Entry in eff_magic); eff_magic_get_posX reads its x
