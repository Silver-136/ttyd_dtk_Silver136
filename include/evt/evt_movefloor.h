// evt_movefloor.c: scripts and commands for moving floors, platforms that follow a path.

#pragma once

#include "dolphin/types.h"
#include "evt/evt_word.h"
#include "evt/evt_cmd.h"

extern EvtWord moving_floor[];
extern EvtWord ridedown_floor[];
USER_FUNC(evt_moving_floor_delete);
