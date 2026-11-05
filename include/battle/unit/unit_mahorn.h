#pragma once

// Work slots of unit_mahorn, shared by its copies in the jon and tst RELs.

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_BUNSIN 4       // 0 for the original: wait_event then also restarts the poses of the copies (mahorn_get_bunsin); nothing in this file sets it nonzero
#define UNIT_WORK_FADING 6       // cleared by init_event; the slot unit_super_mahorn's clones use to fade out only once
#define UNIT_WORK_MAGIC_EFFECT 7 // EffectEntry* of the magic shot (effMahorn2Entry, eff_magic); eff_magic_get_posX reads the x of its type-3 element
