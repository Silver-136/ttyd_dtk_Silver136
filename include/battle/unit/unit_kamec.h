#pragma once

// Work slots of unit_kamec, shared by its copies in the tik and tst RELs.

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_SKY 0          // nonzero = sky form (pose_table_sky, data_table_sky*, airborne, height 40) from the battle setup; dmg_fall_event clears it; spawn_event copies it to clones; picks the sky or ground magic events
#define UNIT_WORK_BUNSIN 1       // 1 for a clone made by spawn_event; a clone dies on any damage or miss (bunsin_kill_event), skips the attack and the wait-pose refresh, and uses the *_bunsin data tables
#define UNIT_WORK_KILL_BUNSIN 2  // damage_event sets 1 when the original is hit while clones exist; after the damage it then removes all clones (kamec_subset_bunsin_all_kill)
#define UNIT_WORK_FADING 3       // 1 once bunsin_kill_event has started fading the clone out (a second call skips the fade)
#define UNIT_WORK_STAFF_EFFECT 4 // EffectEntry* of the staff effect (effThunderflareN64Entry in eff_staff1, effPokopiPchargeN64Entry in eff_staff2), kept at the staff tip
#define UNIT_WORK_MAGIC_EFFECT 5 // EffectEntry* of the magic shot (effMagic1N64Entry, eff_magic); eff_magic_get_posX reads its x offset
