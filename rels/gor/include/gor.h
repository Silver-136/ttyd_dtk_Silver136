#pragma once

#include "dolphin/types.h"
#include "dolphin/mtx.h"
#include "driver/npcdrv.h"
#include "evt/evt_word.h"
#include "evt/evt_cmd.h"

struct EventEntry;

extern EvtWord all_juyoitem_lecture_rel[];
extern EvtWord all_party_lecture[];
extern NpcTerritoryInfo btl_gor_setup_no_tbl[];
extern NpcSetupBattleInfo btlsetup_gor_tbl[];
extern const char* door_data_05_npc[];
extern const char* door_data_05_npc_01[];
extern EvtWord gor_00_init_evt[];
extern EvtWord gor_01_init_evt[];
extern EvtWord gor_02_init_evt[];
extern EvtWord gor_03_init_evt[];
extern EvtWord gor_04_init_evt[];
extern EvtWord gor_10_init_evt[];
extern EvtWord gor_11_init_evt[];
extern EvtWord gor_12_init_evt[];
extern EvtWord gor_irai_init[];
extern EvtWord gor_villager_urouro[];
extern EvtWord luigi_gor_first_talk[];
extern BattleUnitKind unit_lecture_frankli_sac;
extern BattleUnitKind unit_monban;

typedef struct CookRecipe {
    const char* a;
    const char* b;
    const char* out;
} CookRecipe;
extern CookRecipe R_cookbook_recipe_table[];
extern s32 R_mystery_cook_list[];
/* one-ingredient recipes (cook_chk): result without the Cookbook, result2 once Mario has it (trouble
 * TROUBLE_SEEKING_LEGENDARY_BOOK solved) */
typedef struct SimpleRecipe {
    const char* ingredient;
    const char* result;
    const char* result2;
} SimpleRecipe;
extern SimpleRecipe R_simple_recipe_table[];
extern f32 gor_ship_puka_data[];
extern s32* irai_item_table[];
extern const char* irai_msg_client[];
extern const char* irai_msg_contents[];
extern const char* irai_msg_kakunin[];
extern s32 special_cook_list[];
extern Vec suifu100kaidanDOWN[];
extern Vec suifu100kaidanUP[];
USER_FUNC(evt_goki_sub_intpl_get_value);
USER_FUNC(evt_goki_sub_intpl_init);
USER_FUNC(evt_gor_inemuri_snd_off);
USER_FUNC(evt_gor_inemuri_snd_set);
USER_FUNC(evt_gor_msg_cat_alloc);
USER_FUNC(evt_gor_msg_cat_free);
USER_FUNC(evt_gor_villager_urouro_check_func);
void goryuugijo_disp_montemedal(CameraId camera, void* param);
void goryuugijo_disp_slot(CameraId camera, void* param);
void tegami_delete(void);
void goryuugijo_init_montemedal(struct EventEntry* event, BOOL isFirstCall);
void goryuugijo_main_montemedal(struct EventEntry* event, BOOL isFirstCall);
void goryuugijo_init_slot(struct EventEntry* event, BOOL isFirstCall);
void goryuugijo_main_slot(struct EventEntry* event, BOOL isFirstCall);

// Area-local saved values (GOR; swReInit clears them on every area change), named from the area's
// scripts
// set by cooking_evt (gor_01) after Zess T.'s opening line; while set she starts with the short gor_01_cook_004;
// nancy_init clears it on each load
#define LSWF_GOR_01_ZESS_T_GREETED LSWF(1)
// marco_hitorigoto (gor_01, before STG4_32): 1 while Mario is not in talk range of Flavio (chk_marco_fuki), letting
// his song bubble mac_4_004 show; 0 closes it
#define LSWF_GOR_01_FLAVIO_SONG_BUBBLE_ON LSWF(2)
// set during the Don Pianta scenes and talks (maffiaboss_maffiaboss, maffiaboss_airship_ticket,
// maffiaboss_talk_common) and peton_talk_common after STG5_65; while set the two goons stop turning to Mario and
// desk_talk_check skips its check
#define LSWF_GOR_03_TALK_SCENE_ACTIVE LSWF(3)
// cleared by gor_00_koopa_evt before kpa_stg4_018; the message callback gor_stg4_018_cb sets it on the second page to
// start the camera shake and other inline events (dossier names the C use gor_muj_ship_pera)
#define LSWF_GOR_00_KOOPA_MSG_CUE LSWF(6)
