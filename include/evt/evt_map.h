#pragma once

#include "evt/evt_cmd.h"
#include "driver/mapdrv.h" // MapAnimPlayMode
#include <dolphin/types.h>

USER_FUNC(evt_map_playanim);

USER_FUNC(evt_map_blend_off);
USER_FUNC(evt_map_get_fog);
USER_FUNC(evt_map_replayanim);
USER_FUNC(evt_map_set_blend);
USER_FUNC(evt_map_set_fog);
USER_FUNC(evt_mapobj_flag_onoff);
USER_FUNC(evt_mapobj_get_position);
USER_FUNC(evt_mapobj_trans);
extern EvtWord evt_nonride_airport[];
extern EvtWord evt_ride_airport[];
USER_FUNC(N_evt_mapdisp_onoff);
USER_FUNC(check2);
USER_FUNC(evt_map_blend_set_flag);
USER_FUNC(evt_map_blend_set_mobj_flag);
USER_FUNC(evt_map_checkanim);
USER_FUNC(evt_map_entry_airport_harbor);
USER_FUNC(evt_map_fog_onoff);
USER_FUNC(evt_map_get_flush_color);
USER_FUNC(evt_map_pauseanim);
USER_FUNC(evt_map_replace_mapobj);
USER_FUNC(evt_map_set_flush_color);
USER_FUNC(evt_map_set_flush_onoff);
USER_FUNC(evt_map_set_playrate);
USER_FUNC(evt_map_set_tevcallback);
USER_FUNC(evt_mapobj_clear_offscreen);
USER_FUNC(evt_mapobj_color);
USER_FUNC(evt_mapobj_rotate);
USER_FUNC(evt_mapobj_scale);
USER_FUNC(evt_mapobj_set_offscreen);
