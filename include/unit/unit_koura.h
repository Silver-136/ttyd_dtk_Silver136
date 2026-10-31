// unit_koura.c: the battle unit kind for the Koopa shell (unit_koura), used by Koops.

#pragma once

#include "dolphin/types.h"

extern BattleUnitKind unit_koura;

// Names from LLCOOLT's tree.
enum {
    KOURA_FOLLOW_NONE = 0,
    KOURA_FOLLOW_ON = 1,
    KOURA_FOLLOW_TRAIL = 2,
    KOURA_FOLLOW_SLIDE = 3,
};
