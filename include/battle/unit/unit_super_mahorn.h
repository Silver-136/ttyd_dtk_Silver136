#pragma once

// Work slots of unit_super_mahorn, shared by its copies in the jon, las and tst RELs.

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_BUNSIN 4       // 1 on a clone (spawned by the bunsin event), 0 on the real one; clones skip attack decisions, still move their hands while waiting, and die when hit
#define UNIT_WORK_KILL_BUNSIN 5  // damage_event: 1 if the real one was hit while clones exist; after the damage all clones are killed
#define UNIT_WORK_FADING 6       // clone: 1 when bunsin_kill_event starts fading it out, so it fades only once
#define UNIT_WORK_MAGIC_EFFECT 7 // EffectEntry of the magic shot (effMahorn2Entry); eff_magic_get_posX reads its type-3 element's x
