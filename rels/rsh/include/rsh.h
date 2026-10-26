#pragma once

#include "dolphin/types.h"
#include "driver/npcdrv.h"
#include "evt/evt_word.h"
#include "evt/evt_cmd.h"

extern NpcTerritoryInfo btl_rsh_setup_no_tbl[];
extern NpcSetupBattleInfo btlsetup_rsh_tbl[];
extern EvtWord evt_rsh_01_rnd_npc_entry[];
extern EvtWord evt_rsh_03_rnd_npc_entry[];
extern EvtWord evt_rsh_04_rnd_npc_entry[];
extern EvtWord evt_rsh_door_entry[];
extern EvtWord evt_rsh_door_init[];
extern EvtWord evt_rsh_konarikin_irai_party_talk[];
extern EvtWord evt_rsh_moamoa_on_window[];
extern EvtWord evt_rsh_rnd_npc_init[];
extern EvtWord evt_rsh_window[];
extern EvtWord rsh_00_a_init_evt[];
extern EvtWord rsh_00_a_init_evt_nidome[];
extern EvtWord rsh_00_b_init_evt[];
extern EvtWord rsh_00_b_init_evt_nidome[];
extern EvtWord rsh_00_c_init_evt[];
extern EvtWord rsh_00_c_init_evt_nidome[];
extern EvtWord rsh_01_a_init_evt[];
extern EvtWord rsh_01_a_init_evt_nidome[];
extern EvtWord rsh_01_b_init_evt[];
extern EvtWord rsh_01_b_init_evt_nidome[];
extern EvtWord rsh_01_c_init_evt[];
extern EvtWord rsh_01_c_init_evt_nidome[];
extern EvtWord rsh_02_a_init_evt[];
extern EvtWord rsh_02_a_init_evt_nidome[];
extern EvtWord rsh_02_b_init_evt[];
extern EvtWord rsh_02_b_init_evt_nidome[];
extern EvtWord rsh_02_c_init_evt[];
extern EvtWord rsh_02_c_init_evt_nidome[];
extern EvtWord rsh_03_a_init_evt[];
extern EvtWord rsh_03_a_init_evt_nidome[];
extern EvtWord rsh_03_b_init_evt[];
extern EvtWord rsh_03_b_init_evt_nidome[];
extern EvtWord rsh_03_c_init_evt[];
extern EvtWord rsh_03_c_init_evt_nidome[];
extern EvtWord rsh_04_a_init_evt[];
extern EvtWord rsh_04_a_init_evt_nidome[];
extern EvtWord rsh_04_b_init_evt[];
extern EvtWord rsh_04_b_init_evt_nidome[];
extern EvtWord rsh_04_c_init_evt[];
extern EvtWord rsh_04_c_init_evt_nidome[];
extern EvtWord rsh_05_a_init_evt[];
extern EvtWord rsh_05_a_init_evt_nidome[];
extern EvtWord rsh_06_a_init_evt[];
extern EvtWord rsh_06_a_init_evt_nidome[];
extern EvtWord rsh_07_a_init_evt[];
extern EvtWord rsh_07_b_init_evt[];
extern EvtWord rsh_07_c_init_evt[];
extern EvtWord rsh_08_a_init_evt[];
USER_FUNC(debug_print_func);
USER_FUNC(evt_rsh_get_rnd_npc);
USER_FUNC(evt_rsh_get_rnd_npc_data);
USER_FUNC(evt_rsh_get_rnd_npc_talk_evt);
USER_FUNC(evt_rsh_set_rnd_npc);
USER_FUNC(evt_rsh_set_rnd_npc_talk_evt);

// Area-local saved values (RSH; swReInit clears them on every area change), named from the area's
// scripts
// talk_unten (rsh_00, the train engineer, before STG6_06): alternates stg6_rsh_00 (0) and stg6_rsh_01 (1)
#define LSWF_RSH_00_ENGINEER_TALK_TOGGLE LSWF(1)
// talk_unten from STG6_21 (day 2) to STG6_27: first talk stg6_rsh_152 sets it, later talks stg6_rsh_153
#define LSWF_RSH_00_ENGINEER_DAY2_TALKED LSWF(2)
// talk_gurume (rsh_02): alternates two lines (stg6_rsh_54_01/55 before STG6_05, stg6_rsh_63/64 before STG6_13);
// evt_nabe clears it
#define LSWF_RSH_02_GOURMET_TALK_TOGGLE LSWF(3)
// talk_gurume STG6_21..STG6_27: first talk stg6_rsh_161 sets it, later talks stg6_rsh_162
#define LSWF_RSH_02_GOURMET_DAY2_TALKED LSWF(4)
// talk_cook (rsh_03, Chef Shimi, before STG6_01): first talk stg6_rsh_04 sets it, later talks stg6_rsh_05
#define LSWF_RSH_03_CHEF_FIRST_TALKED LSWF(5)
// talk_cook STG6_20..STG6_27: first talk stg6_rsh_156 sets it, later talks stg6_rsh_157
#define LSWF_RSH_03_CHEF_DAY2_TALKED LSWF(6)
// talk_waitless (rsh_03, before STG6_01): first talk stg6_rsh_02 sets it, later talks stg6_rsh_03
#define LSWF_RSH_03_WAITRESS_FIRST_TALKED LSWF(7)
// talk_waitless STG6_20..STG6_27: first talk stg6_rsh_154 sets it, later talks stg6_rsh_155
#define LSWF_RSH_03_WAITRESS_DAY2_TALKED LSWF(8)
// talk_syashou (rsh_04, STG6_01..STG6_06): alternates stg6_rsh_07 (0) and stg6_rsh_08 (1)
#define LSWF_RSH_04_CONDUCTOR_TALK_TOGGLE LSWF(9)
// talk_syashou STG6_20..STG6_27: alternates stg6_rsh_158 (0) and stg6_rsh_159 (1)
#define LSWF_RSH_04_CONDUCTOR_DAY2_TOGGLE LSWF(10)
// evt_access2 (rsh_05, object bin_2): the first check gives a Power Punch (stg6_rsh_277) and sets it; later checks
// print stg6_rsh_279
#define LSWF_RSH_05_BIN_POWER_PUNCH LSWF(12)
