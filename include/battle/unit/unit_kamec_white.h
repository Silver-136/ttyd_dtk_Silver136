#pragma once

// Work slots of unit_kamec_white, shared by its copies in the tou2 and tst RELs.

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_SKY 0          // nonzero (battle setup unitWork) = on the broom: init_event2 uses pose_table_sky / data_table_sky and raises it to y 40; dmg_fall_event clears it; magic events pick sky or ground; copied to clones
#define UNIT_WORK_BUNSIN 1       // 1 on the clones spawn_event creates (bunsin), 0 on the real one; clones skip attack/wait and die on any hit or miss
#define UNIT_WORK_KILL_BUNSIN 2  // 1 when the real one is hit while clones exist; after the damage event all clones are killed
#define UNIT_WORK_FADING 3       // 1 once bunsin_kill_event started fading this unit out (a second kill skips the fade)
#define UNIT_WORK_STAFF_EFFECT 4 // EffectEntry* of the wand charge effect (eff_staff1 thunderflare, eff_staff2 pokopi charge) that the user function keeps at the wand tip
#define UNIT_WORK_MAGIC_EFFECT 5 // EffectEntry* of the magic shot (eff_magic); eff_magic_get_posX reads its x
