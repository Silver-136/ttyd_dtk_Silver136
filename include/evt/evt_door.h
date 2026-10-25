#pragma once

#include "evt/evt_cmd.h"
#include <dolphin/types.h>

USER_FUNC(snd_door_in);
USER_FUNC(snd_door_out);
USER_FUNC(door_entry);

extern EvtWord door_close_se[];
extern EvtWord door_dark[];
extern EvtWord door_light[];
extern EvtWord door_light2[];
extern EvtWord door_open_se[];
extern EvtWord door_up_se[];
extern EvtWord evt_door_close[];
extern EvtWord evt_door_close_roll[];
USER_FUNC(evt_door_data_copy);
extern EvtWord evt_door_entry[];
extern EvtWord evt_door_open[];
extern EvtWord evt_door_open_bom_01[];
extern EvtWord evt_door_open_quick[];
extern EvtWord evt_door_open_roll[];
extern EvtWord evt_door_setup[];
extern EvtWord house_close_se[];
USER_FUNC(animation);
USER_FUNC(door_dark_flag);
USER_FUNC(door_group);
USER_FUNC(evt_door_end_wait);
USER_FUNC(evt_door_load_mapflag);
USER_FUNC(evt_door_param);
USER_FUNC(evt_door_save_mapflag);
USER_FUNC(evt_door_set_param);
USER_FUNC(get_noclip_map);
USER_FUNC(in_pos);
USER_FUNC(init_inside_group);
USER_FUNC(inout_hit);
USER_FUNC(inside_group);
USER_FUNC(npc_dark);
USER_FUNC(npc_hide_onoff);
USER_FUNC(npc_light);
USER_FUNC(out_pos);
USER_FUNC(outside_group);
void door_position_sub(s32 type, const char* map, const char* hitIn, const char* hitOut, Vec* in, Vec* out, BOOL mario);
