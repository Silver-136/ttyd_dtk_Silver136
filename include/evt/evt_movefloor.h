#pragma once

#include "dolphin/types.h"
#include "evt/evt_word.h"
#include "evt/evt_cmd.h"

extern EvtWord moving_floor[];
extern EvtWord ridedown_floor[];
USER_FUNC(evt_moving_floor_alloc);
USER_FUNC(evt_moving_floor_delete);
USER_FUNC(evt_moving_floor_free);
USER_FUNC(evt_moving_floor_init);
USER_FUNC(evt_moving_floor_main);
USER_FUNC(evt_moving_floor_main2);
USER_FUNC(moving_floor_get_end);
USER_FUNC(moving_floor_get_kind);
USER_FUNC(moving_floor_get_mode);
USER_FUNC(moving_floor_get_pause);
USER_FUNC(moving_floor_get_stmsec);
USER_FUNC(moving_floor_getf_now_bpoint);
USER_FUNC(moving_floor_getf_pos);
USER_FUNC(moving_floor_getf_xz_scale);
USER_FUNC(moving_floor_halt_se);
USER_FUNC(moving_floor_reverse);
USER_FUNC(moving_floor_set_se);
USER_FUNC(moving_floor_stop_se);
USER_FUNC(moving_floor_update_turn);
