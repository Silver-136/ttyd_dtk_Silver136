#pragma once

#include "dolphin/gx/GXStruct.h"
#include "dolphin/types.h"
#include "evt/evt_word.h"
#include "evt/evt_cmd.h"

extern const GXColor twod_icon_fog_color;
extern const GXColor twod_text_fog_color;
extern EvtWord twoddisp_main_event[];
USER_FUNC(twoddisp_main);
USER_FUNC(evt_twoddisp_entry);
USER_FUNC(evt_twoddisp_flag_onoff);
USER_FUNC(evt_twoddisp_init);
USER_FUNC(evt_twoddisp_set_base_position);
USER_FUNC(evt_twoddisp_set_position);
