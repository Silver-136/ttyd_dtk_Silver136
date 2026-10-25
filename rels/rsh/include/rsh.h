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
