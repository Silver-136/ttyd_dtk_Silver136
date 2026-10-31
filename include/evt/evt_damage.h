// evt_damage.c: the Gazigazi map damage hazard scripts (evt_gazigazi_appear, evt_gazigazi_entry).

#pragma once

#include "dolphin/types.h"
#include "evt/evt_word.h"
#include "evt/evt_cmd.h"

extern EvtWord evt_gazigazi_appear[];
extern EvtWord evt_gazigazi_entry[];
USER_FUNC(camdir);
USER_FUNC(evt_gazigazi_move_position);
