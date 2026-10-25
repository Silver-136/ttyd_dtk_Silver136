#pragma once

#include "dolphin/types.h"
#include "evt/evt_word.h"
#include "evt/evt_cmd.h"

extern EvtWord all_juyoitem_lecture_rel[];
extern EvtWord all_party_lecture[];
extern EvtWord evt_gardman_spawn[];
extern EvtWord evt_tou_08_cake[];
extern EvtWord evt_tou_bed[];
extern EvtWord evt_tou_chibi_yoshi[];
extern EvtWord evt_tou_locker_init[];
extern EvtWord evt_tou_match_after_default[];
extern EvtWord evt_tou_match_make_default[];
extern EvtWord evt_tou_monitor_init[];
extern EvtWord evt_tou_monitor_off[];
extern EvtWord evt_tou_monitor_on[];
extern EvtWord evt_tou_ranking_msg[];
extern EvtWord tou_00_init_evt[];
extern EvtWord tou_01_init_evt[];
extern EvtWord tou_02_init_evt[];
extern EvtWord tou_04_init_evt[];
extern EvtWord tou_05_init_evt[];
extern EvtWord tou_06_init_evt[];
extern EvtWord tou_07_init_evt[];
extern EvtWord tou_08_init_evt[];
extern EvtWord tou_09_init_evt[];
extern EvtWord tou_10_init_evt[];
extern EvtWord tou_11_init_evt[];
extern EvtWord tou_12_init_evt[];
extern EvtWord tou_13_init_evt[];
extern EvtWord tou_20_init_evt[];

extern EvtWord evt_match_after_stop_npcs[];
USER_FUNC(chk);
USER_FUNC(evt_rankingReset);
USER_FUNC(fight_money);
USER_FUNC(make_monitor);
USER_FUNC(octo_start);
USER_FUNC(octo_stop);
USER_FUNC(onoff);
USER_FUNC(tou_mario_free_wait);
USER_FUNC(tou_mario_keyoff_check);
USER_FUNC(tou_mario_talk_end_wait);
