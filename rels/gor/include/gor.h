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

typedef struct CookRecipe { const char* a; const char* b; const char* out; } CookRecipe;
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
