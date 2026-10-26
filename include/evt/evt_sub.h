#pragma once

#include "manager/winmgr.h"
#include "evt/evt_cmd.h"
#include "mario/mariost.h" // GlobalWorkFlags for evt_sub_get_system_flag / evt_sub_system_flag_onoff
#include "dolphin/types.h"

USER_FUNC(evt_key_get_buttontrg);
USER_FUNC(evt_sub_animgroup_async);
USER_FUNC(evt_sub_countdown_end);
USER_FUNC(evt_sub_countdown_flag_onoff);
USER_FUNC(evt_sub_get_dir);
USER_FUNC(evt_sub_get_sincos);
USER_FUNC(evt_sub_get_stopwatch);
USER_FUNC(evt_sub_get_system_flag);
USER_FUNC(evt_sub_intpl_get_float);
USER_FUNC(evt_sub_intpl_get_value);
USER_FUNC(evt_sub_intpl_get_value_para);
USER_FUNC(evt_sub_intpl_init);
USER_FUNC(evt_sub_intpl_init_float);
USER_FUNC(evt_sub_intpl_msec_get_value);
USER_FUNC(evt_sub_intpl_msec_get_value_para);
USER_FUNC(evt_sub_intpl_msec_init);
extern EvtWord evt_sub_kusa_access_entry[];
USER_FUNC(evt_sub_load_progresstime);
extern EvtWord evt_sub_mail_event[];
extern EvtWord evt_sub_mail_vibration[];
USER_FUNC(evt_sub_random);
USER_FUNC(evt_sub_rumble_onoff);
extern EvtWord evt_sub_starstone[];
extern EvtWord evt_sub_stopwatch[];
extern EvtWord evt_sub_tree_access_entry[];

extern WinMgrDesc coinget_window_desc[1];
extern EvtWord kusa_evt[43];
extern EvtWord mail_evt[80];
extern EvtWord mail_evt_bom_01[65];
extern EvtWord mail_evt_gor_01[61];
extern EvtWord mail_evt_gor_01_2[93];
extern EvtWord mail_evt_gor_02[87];
extern EvtWord mail_evt_gor_02_2[61];
extern EvtWord mail_evt_gor_03[87];
extern EvtWord mail_evt_gor_04[97];
extern EvtWord mail_evt_hom_00[61];
extern EvtWord mail_evt_nok_01[64];
extern EvtWord mail_evt_pik_00[71];
extern EvtWord mail_evt_rsh_03_a[61];
extern EvtWord mail_evt_rsh_03_a_2[61];
extern EvtWord mail_evt_tik_00[80];
extern EvtWord mail_evt_tou_01[67];
extern EvtWord mail_evt_usu_01[61];
extern EvtWord mail_evt_win_01[61];
extern s32 tree_data[75];
extern EvtWord tree_evt[251];
USER_FUNC(N_evt_sub_status_gauge_force_update);
USER_FUNC(evt_sub_area_check);
USER_FUNC(evt_sub_check_intersect);
USER_FUNC(evt_sub_countdown_get_status);
USER_FUNC(evt_sub_countdown_set_restart_time);
USER_FUNC(evt_sub_countdown_start);
USER_FUNC(evt_sub_get_areaname);
USER_FUNC(evt_sub_get_coin);
USER_FUNC(evt_sub_get_dist);
USER_FUNC(evt_sub_get_language);
USER_FUNC(evt_sub_get_mapname);
USER_FUNC(evt_sub_save_playtime);
USER_FUNC(evt_sub_scale_by_fps);
USER_FUNC(evt_sub_spline_free);
USER_FUNC(evt_sub_spline_get_value);
USER_FUNC(evt_sub_spline_get_value_manual);
USER_FUNC(evt_sub_spline_init);
USER_FUNC(evt_sub_status_gauge_check_update);
USER_FUNC(evt_sub_status_gauge_force_close);
USER_FUNC(evt_sub_status_gauge_force_open);
USER_FUNC(evt_sub_system_flag_onoff);
USER_FUNC(get_stop);
USER_FUNC(irai_mail_check);
USER_FUNC(mail_evt_gor_04_keyoff_check);
USER_FUNC(set_stop);
USER_FUNC(stageclear_wait);
USER_FUNC(stone_bg_open);
USER_FUNC(stone_ry);
