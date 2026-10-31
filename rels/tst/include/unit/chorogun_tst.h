// Chorobon gundan (Fuzzy Horde) and the gold Chorobon that summons it: shared by the hei units and
// their tst copies.
#pragma once

#include "battle/battle_database_common.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include <dolphin/mtx.h>
#include <dolphin/types.h>

// Chorogun.flags
#define CHOROGUN_FLAG_ALIVE 0x1

// One fuzzy of the horde, drawn as an ext pose (chorogun_ext_*); the horde allocates 1000.
typedef struct Chorogun {
    u8 flags;   // 0x00, CHOROGUN_FLAG_*
    u8 state;   // 0x01
    Vec vel;    // 0x04
    f32 speed;  // 0x10
    Vec pos;    // 0x14
    s32 timer;  // 0x20
    Vec target; // 0x24
} Chorogun;

extern Chorogun* chorogun;
extern s32 chorogun_targetId;

extern BattleUnitKind unit_chorobon_gundan;
extern BattleWeapon weapon_chorobon_gundan_attack;

// unit_chorobon_gundan.c
USER_FUNC(chorogun_appear);
USER_FUNC(chorogun_escape_all);
extern EvtWord chorogun_entry[];
extern EvtWord chorogun_dead_reward[];

// unit_gold_chorobon.c
extern EvtWord escape_chorogun_event[];
