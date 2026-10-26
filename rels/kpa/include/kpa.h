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

// Area-local saved values (KPA; swReInit clears them on every area change), named from the area's
// scripts
// kpa_01 scene cue: cleared by kpa_01_syuppatu_evt, douzou_3_touch_evt and hammer_AB_talk_evt before a message; the
// callbacks kpa_stg1_008/014_2/034/036_cb set it at a page so waiting inline events start (dossier names those C uses
// kpa_01_init_evt)
#define LSWF_KPA_01_MSG_CUE LSWF(1)
// cleared by kpa_init; set when kpa_stage_start ends (kpa_04 and kpa_06 init set it directly); kpa_time waits for it
// before counting down
#define LSWF_KPA_TIMER_STARTED LSWF(2)
// flag of the goal pole (evt_mobj_koopa_pole sets it when Bowser touches it) and set by kpa_dead; kpa_dead returns if
// already set, kpa_time and kairyu_evt (kpa_03) stop; kpa_init clears it
#define LSWF_KPA_STAGE_ENDED LSWF(3)
// set by kpa_dead; kpa_bgm_change then plays BGM_FF_KUPPA_DOWN1; kpa_init clears it
#define LSWF_KPA_BOWSER_DEAD LSWF(4)
// set by kpa_move_to_toride (src/event/evt_mobj.c, run by the goal pole after the score) as Bowser walks to the
// toride object; kpa_bgm_change then plays BGM_FF_KUPPA_GOAL1; kpa_init clears it
#define LSWF_KPA_GOAL_WALK_STARTED LSWF(5)
// set by each room's init (kpa_02/04/06 = 0, kpa_03 = 1, kpa_05 = 2); kpa_bgm_change plays
// BGM_STGK_FIELD1/FIELD3/FIELD4 by it, +3 for the _UP1 version when 30 or less time is left
#define LSW_KPA_FIELD_BGM LSW(1)
