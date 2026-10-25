#pragma once

#include "dolphin/types.h"
#include "evt/evt_word.h"
#include "evt/evt_cmd.h"

extern EvtWord kpa_00_init_evt[];
extern EvtWord kpa_01_init_evt[];
extern EvtWord kpa_02_init_evt[];
extern EvtWord kpa_03_init_evt[];
extern EvtWord kpa_04_init_evt[];
extern EvtWord kpa_05_init_evt[];
extern EvtWord kpa_06_init_evt[];
extern EvtWord kpa_07_init_evt[];
extern EvtWord kpa_bgm_change[];
extern EvtWord kpa_dead[];
extern EvtWord kpa_fall_sub[];
extern EvtWord kpa_init[];
extern EvtWord kpa_read_item[];
extern EvtWord kpa_read_mobj[];
extern EvtWord kpa_stage_start[];
extern EvtWord kpa_time[];

extern EvtWord kpa_status_main_evt[];
USER_FUNC(kpa_add_coin);
USER_FUNC(kpa_fire_off);
USER_FUNC(kpa_get_cam_pt);
USER_FUNC(kpa_get_msg);
USER_FUNC(kpa_make_item_name);
USER_FUNC(kpa_make_mobj_name);
USER_FUNC(kpa_mobj_on);
USER_FUNC(kpa_muteki_check);
USER_FUNC(kpa_nohitenemy_ct_clear);
USER_FUNC(kpa_pole_hitoff);
USER_FUNC(kpa_set_scale);
USER_FUNC(kpa_set_time);
USER_FUNC(kpa_stageclear_disp_entry);
USER_FUNC(kpa_status_get_stage);
USER_FUNC(kpa_status_init);
USER_FUNC(kpa_status_main);
USER_FUNC(kpa_time_countdown);
