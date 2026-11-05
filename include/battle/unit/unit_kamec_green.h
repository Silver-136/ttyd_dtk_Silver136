#pragma once

// Work slots of unit_kamec_green, shared by its copies in the tou2 and tst RELs.

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_SKY 0          // nonzero = the flying version (sky pose/data tables, airborne, y 40); dmg_fall_event clears it; clones copy it
#define UNIT_WORK_BUNSIN 1       // 1 on a clone (unit_kamec_green_bunsin spawned by spawn_event), 0 on the real one; clones skip attack/wait and die when hit
#define UNIT_WORK_KILL_BUNSIN 2  // damage_event: 1 if the real one was hit while clones exist; after the damage all clones are killed
#define UNIT_WORK_FADING 3       // clone: 1 once bunsin_kill_event has started fading it out, so it fades only once
#define UNIT_WORK_STAFF_EFFECT 4 // EffectEntry of the staff effect (effPokopiPchargeN64Entry / effThunderflareN64Entry) kept on the wand tip
#define UNIT_WORK_MAGIC_EFFECT 5 // EffectEntry of the magic shot (effMagic1N64Entry); eff_magic_get_posX reads its x
