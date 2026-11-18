#pragma once

#include "dolphin/gx/GXStruct.h"
#include "dolphin/types.h"
#include "evt/evt_word.h"
#include "evt/evt_cmd.h"

USER_FUNC(evt_twoddisp_entry);
USER_FUNC(evt_twoddisp_flag_onoff);
USER_FUNC(evt_twoddisp_init);
USER_FUNC(evt_twoddisp_set_base_position);
USER_FUNC(evt_twoddisp_set_scale);
USER_FUNC(evt_twoddisp_set_position);

// 2D overlay entry flags (TwodEntry.flags; evt_twoddisp_flag_onoff args[2])
enum {
    TWOD_FLAG_DISP = 1, // set by evt_twoddisp_entry; twoddisp_disp draws only entries with it
};
