// usu_00 (ウスグラタウン, Twilight Town): the west side of town with the pipe to Rogueport Sewers (tik_08), the mayor and the
// villagers turned into pigs; also the town's chapter 7 and 8 scenes.

#include "version.h"
#pragma defer_codegen on
#include "battle/battle_unit.h"
#include "evt/evt_story.h"
#include "evt/evt_urouro.h"
#include "evt/evt_window.h"
#include "kpa_act.h"
#include "eff/eff_starstone.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bg.h"
#include "evt/evt_cam.h"
#include "evt/evt_eff.h"
#include "evt/evt_case.h"
#include "evt/evt_ext.h"
#include "evt/evt_fade.h"
#include "evt/evt_hit.h"
#include "evt/evt_item.h"
#include "evt/evt_kinopio.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_mobj.h"
#include "evt/evt_msg.h"
#include "evt/evt_party.h"
#include "evt/evt_pouch.h"
#include "evt/evt_seq.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_telop.h"
#include "eff/eff_kemutest.h"
#include "driver/mapdrv.h"
#include "system.h"
#include "driver/casedrv.h"
#include "driver/itemdrv.h"
#include "mario/mario.h"
#include "driver/envdrv.h"

#include "mario/mario_pouch.h"
#include "party.h"
#include "driver/animdrv.h"
#include "pmario_sound.h"
#include "driver/npcdrv.h"
#include "memory.h"
#include "dolphin/mtx.h"
#include "stdlib.h"
#include "mario/mariost.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "eff/eff_small_star.h"
#include "manager/evtmgr_cmd.h"
#include "eff/eff_kemu_test.h"
#include "driver/fadedrv.h"
#include "driver/effdrv.h"
#include "evt/evt_bero.h"
#include "evt/evt_door.h"
#include "evt/evt_memcard.h"
#include "evt/evt_sub.h"
#include "evt/evt_npc.h"
#include "usu.h"
#include "data_tables.h"
#include "driver/msgdrv.h"
#include "driver/extdrv.h"
#include "driver/seqdrv.h"

typedef struct KpaSen {
    s8 state;
    u8 field_0x1;
    u16 wait;
    f32 t;
    Vec start;
    Vec end;
    u8 pose99Lo;
    u8 pose99Hi;
    u8 pose1Lo;
    u8 pose1Hi;
} KpaSen;
KpaSen* kpaSen;

static USER_FUNC(anim_tevcallback);
USER_FUNC(bakuhatu_kemuri);
static USER_FUNC(booboo_main);
static USER_FUNC(delete_npc_regl_evt);
static USER_FUNC(getYoshiColor);
USER_FUNC(kpaSen_toge_syugo);
USER_FUNC(kpaSen_zako_syugo);
static USER_FUNC(npc_process);
USER_FUNC(party_scale);
static USER_FUNC(small_star);
static USER_FUNC(starstone_entry);
static USER_FUNC(starstone_set_pos);
static USER_FUNC(starstone_set_ry);
USER_FUNC(white_off);
USER_FUNC(white_on);
static USER_FUNC(yami_view);
USER_FUNC(party_tev);
#if VERSION >= VERSION_US
#endif
extern s32 envSetYami[];

EvtWord villagerA_init[28];
EvtWord villagerA_pig_init[19];
EvtWord villagerA_talk[65];
EvtWord villagerB_init[7];
EvtWord villagerB_talk[52];
EvtWord villagerC_init[7];
EvtWord villagerC_talk[52];
EvtWord villagerD_init[19];
EvtWord villagerD_pig_init[19];
EvtWord villagerD_talk[88];
EvtWord villagerE_init[19];
EvtWord villagerE_pig_init[19];
EvtWord villagerE_talk[65];
EvtWord villagerK_init[19];
EvtWord villagerK_pig_init[19];
EvtWord villagerK_talk[65];
EvtWord village_chief_init[20];
EvtWord village_chief_pig_init[27];
s32 iri_15_item_tbl[2];
#if VERSION <= VERSION_DEMO_US
extern EvtWord village_chief_talk[];
#else
EvtWord village_chief_talk[199];
#endif
EvtWord chief_mario_talk_init[183];
EvtWord chief_mario_talk[55];
static EvtWord vivian_init[44];
static s32 key_tbl[2];
#if VERSION <= VERSION_DEMO_US
static EvtWord vivian_talk[27];
#else
static EvtWord vivian_talk[29];
#endif
EvtWord nise_mario_talk[8];
EvtWord christine_talk[8];
EvtWord nokotaro_talk[8];
EvtWord cloud_talk[8];
EvtWord yoshi_talk[8];
EvtWord crow_a_init[8];
EvtWord crow_b_init[8];
EvtWord crow_c_init[8];
EvtWord crow_d_init[8];
EvtWord crow_e_init[8];
static EvtWord urouro_move[24];
static NpcSetupInfo npcEnt[20];
EvtWord usu00_stage4_coming_event[839];
EvtWord usu00_3witch_init[41];
EvtWord usu00_3witch_event[275];
EvtWord gra00_2witch_find_ranpel_event[197];
#if VERSION <= VERSION_DEMO_US
extern EvtWord usu00_vivian_cry_event[];
#else
EvtWord usu00_vivian_cry_event[58];
#endif
EvtWord usu00_vivian_joinup_event[608];
static EvtWord cam_up[34];
static EvtWord cam_down[8];
EvtWord usu00_check_vivian[33];
#if VERSION >= VERSION_US
EvtWord usu00_crowAB_talk_event[338];
#else
#if VERSION == VERSION_DEMO_JP
EvtWord usu00_crowAB_talk_event[291];
#else
EvtWord usu00_crowAB_talk_event[299];
#endif
#endif
#if VERSION >= VERSION_US
EvtWord usu00_crowCDE_talk_event[346];
#else
#if VERSION == VERSION_DEMO_JP
EvtWord usu00_crowCDE_talk_event[299];
#else
EvtWord usu00_crowCDE_talk_event[307];
#endif
#endif
EvtWord chief_pig_event[66];
static KinopioData kino_dt;
static const char* yado_npc[2];
const char* yado_mobj[2];
const char* village_chief_npc[8];
static EvtWord evt_door_1_1[12];
static EvtWord evt_door_1_2[2];
static EvtWord evt_wall_1[30];
const char* door_1_npc[2];
static EvtWord door_1_open[12];
static EvtWord door_1_close[14];
static DoorParam door_1;
EvtWord evt_door_2_1[12];
EvtWord evt_door_2_2[15];
static EvtWord evt_wall_2[123];
EvtWord evt_door2_chk[30];
EvtWord evt_door2_close[14];
static DoorParam door_2;
static EvtWord evt_door_3_1[12];
static EvtWord evt_door_3_2[12];
static EvtWord evt_wall_3[30];
static EvtWord door_3_open[12];
static EvtWord door_3_close[14];
static DoorParam door_3;
EvtWord dokan_1_evt[32];
static BeroEntry bero_entry_data[3];
static EvtWord oshimodo_event[28];
static BeroEntry bero_oshimodo[1];
EvtWord koopa_dokan_evt[57];
BeroEntry bero_koopa_dokan[1];
EvtWord door_ie1_koopa_evt[26];
EvtWord door_ie2_koopa_evt[26];
EvtWord door_ie3_koopa_evt[26];
static EvtWord evt_ki_1[20];
static EvtWord evt_ki_2[28];
static TreeAccess tree_tbl[4];
EvtWord usu_00_tree_init[7];
EvtWord kusa_01_evt[26];
EvtWord evt_kusa_04[25];
EvtWord evt_kusa_06[25];
static KusaAccess kusa_tbl[21];
EvtWord usu_00_kusa_init[7];
EvtWord evt_ichikorori_ball_get[5];
EvtWord usu_00_touch_ichikorori_ball[72];
static BoobooName booboo_name[5];
ExtTexInfo kpaSen_ext_entry_data[39];
static EvtWord baba_talk[8];
EvtWord kanbu_regl[48];
EvtWord kanbu_talk[4];
static EvtWord koopa_npc_entry[205];
EvtWord usu_00_koopa_first_evt[196];
EvtWord bakuhatu_evt[64];
EvtWord murabito_out[172];
EvtWord usu_00_koopa_second_evt[1315];
static EvtWord sekai_yami_npc_init[17];
static EvtWord sekai_yami_npc[22];
static EvtWord sekai_yami_shake[11];
static EvtWord sekai_yami_evt[260];
static EvtWord minnnanokoe_npc_entry[94];
static EvtWord minnnanokoe_evt[381];
#if VERSION <= VERSION_DEMO_US
static EvtWord evt_after_peach[149];
#else
static EvtWord evt_after_peach[151];
#endif
static EvtWord epilogue_npc_entry[48];
static EvtWord epilogue_evt_sub[167];
static EvtWord epilogue_evt[73];
#if VERSION >= VERSION_US
EvtWord usu_00_init_evt[660];
#else
#if VERSION == VERSION_DEMO_JP
EvtWord usu_00_init_evt[544];
#else
EvtWord usu_00_init_evt[658];
#endif
#endif

/**
 * Script helper: writes the color index of Mario's Yoshi partner (pouchGetPartyColor(4)) to args[0],
 * so the stand-in Yoshi NPC wears the same color.
 */
static USER_FUNC(getYoshiColor) {
    EvtWord* args = event->args;
    evtSetValue(event, args[0], pouchGetPartyColor(PARTNER_YOSHI));
    return EVT_RETURN_DONE;
}

/**
 * Script helper: clears the TEV-callback material flags on Mario's paper-anim group,
 * so the shadow tint applied during the night scenes no longer applies to him.
 */
USER_FUNC(party_tev) {
    animPoseSetMaterialFlagOff(partyGetPtr(PARTY_SLOT_PARTY)->paperAnimGroup, MAP_OBJ_TEV_CALLBACK_MASK);
    return EVT_RETURN_DONE;
}

/**
 * Message-window callback for the Bowser interlude text "kpa_stg5_009_7":
 * when page 1 is shown (type 3) it fades BGM channel 1 out and starts the "BGM_FF_GET_ITEM1" jingle.
 */
void kpa_stg5_009_7_cb(s32 type, smartEntry* smart) {
    MsgWinData* info = smart->address;

    switch (type) {
        case 3:
            if (info->curPage == 1) {
                psndBGMOn(PSND_FADEOUT | PSND_SLOT(1), "BGM_FF_GET_ITEM1");
            }
            break;
    }
}

/**
 * Script helper: gives each of the five pig NPCs in booboo_name[] (pig villagers A, D, E, K and the pig
 * Mayor) the "paper, no ambient light" material flag, so their models are drawn without scene ambient light.
 */
static USER_FUNC(booboo_main) {
    u32 i;

    for (i = 0; i < 5; i++) {
        animPoseSetMaterialLightFlagOn(npcNameToPtr(booboo_name[i].name)->poseId, MATERIAL_LIGHT_PAPER_PLAIN);
    }
    return EVT_RETURN_DONE;
}

/**
 * Init script of villager A (the human form): enables move/talk animation,
 * hides her below the map until the Doopliss fight (STG4_10)
 * and parks her at (-180, 0, 0) during the Bowser interlude.
 */
EVT_BEGIN(villagerA_init)
    CALL(evt_npc_flag_onoff, 1, STRING("村人Ａ"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG4_10_DOOPLISS_BATTLE)
        CALL(evt_npc_set_position, STRING("村人Ａ"), 0, -2000, 0)
        CASE_BETWEEN(STG5_47_PEACH_TEC_TALK, STG5_49_BOWSER_TWILIGHT_TOWN_2)
        CALL(evt_npc_set_position, STRING("村人Ａ"), -180, 0, 0)
        CASE_DEFAULT
    END_SWITCH
    RETURN
EVT_END()

/** Init script of the pig form of villager A: shown until STG4_10, then moved below the map for good. */
EVT_BEGIN(villagerA_pig_init)
    CALL(evt_npc_flag_onoff, 1, STRING("ブタ村人Ａ"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG4_10_DOOPLISS_BATTLE)
        CASE_DEFAULT
        CALL(evt_npc_set_position, STRING("ブタ村人Ａ"), 0, -2000, 0)
    END_SWITCH
    RETURN
EVT_END()

/**
 * Talk script of villager A: the Bowser interlude line, an oink plus pig text before the Doopliss fight,
 * then human text that changes with Chapter 4 progress and after the ending.
 */
EVT_BEGIN(villagerA_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(STG5_48_BOWSER_TWILIGHT_TOWN)
        CALL(evt_msg_print, 0, STRING("kpa_stg5_005"), 0, STRING("me"))
        CASE_LESS(STG4_10_DOOPLISS_BATTLE)
        CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG4_VOICE_PIG1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_msg_print, 0, STRING("stg4_usu_29"), 0, STRING("me"))
        CASE_LESS(STG4_16_LEAVING_TWILIGHT_TOWN)
        CALL(evt_msg_print, 0, STRING("stg4_usu_30"), 0, STRING("me"))
        CASE_LESS(STG4_31_FRANKLYS_HOUSE_STG_4)
        CALL(evt_msg_print, 0, STRING("stg4_usu_31"), 0, STRING("me"))
        CASE_LESS_OR_EQUAL(STG8_END_AFTER_THE_BATTLE)
        CALL(evt_msg_print, 0, STRING("stg4_usu_31_1"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg4_usu_31_1_1"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

/** Init script of villager B: enables move/talk animation. */
EVT_BEGIN(villagerB_init)
    CALL(evt_npc_flag_onoff, 1, STRING("村人Ｂ"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    RETURN
EVT_END()

/**
 * Talk script of villager B: the Bowser interlude line,
 * then text by Chapter 4 progress (before the Doopliss fight, until leaving Twilight Town,
 * until Frankly's house, then after the ending).
 */
EVT_BEGIN(villagerB_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(STG5_48_BOWSER_TWILIGHT_TOWN)
        CALL(evt_msg_print, 0, STRING("kpa_stg5_006"), 0, STRING("me"))
        CASE_LESS(STG4_10_DOOPLISS_BATTLE)
        CALL(evt_msg_print, 0, STRING("stg4_usu_32"), 0, STRING("me"))
        CASE_LESS(STG4_16_LEAVING_TWILIGHT_TOWN)
        CALL(evt_msg_print, 0, STRING("stg4_usu_33"), 0, STRING("me"))
        CASE_LESS(STG4_31_FRANKLYS_HOUSE_STG_4)
        CALL(evt_msg_print, 0, STRING("stg4_usu_34"), 0, STRING("me"))
        CASE_LESS_OR_EQUAL(STG8_END_AFTER_THE_BATTLE)
        CALL(evt_msg_print, 0, STRING("stg4_usu_34_1"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg4_usu_34_1_1"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

/** Init script of villager C: enables move/talk animation. */
EVT_BEGIN(villagerC_init)
    CALL(evt_npc_flag_onoff, 1, STRING("村人Ｃ"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    RETURN
EVT_END()

/**
 * Talk script of villager C: the Bowser interlude line,
 * then text by Chapter 4 progress (before the Doopliss fight, until leaving Twilight Town,
 * until Frankly's house, then after the ending).
 */
EVT_BEGIN(villagerC_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(STG5_48_BOWSER_TWILIGHT_TOWN)
        CALL(evt_msg_print, 0, STRING("kpa_stg5_007"), 0, STRING("me"))
        CASE_LESS(STG4_10_DOOPLISS_BATTLE)
        CALL(evt_msg_print, 0, STRING("stg4_usu_35"), 0, STRING("me"))
        CASE_LESS(STG4_16_LEAVING_TWILIGHT_TOWN)
        CALL(evt_msg_print, 0, STRING("stg4_usu_36"), 0, STRING("me"))
        CASE_LESS(STG4_31_FRANKLYS_HOUSE_STG_4)
        CALL(evt_msg_print, 0, STRING("stg4_usu_37"), 0, STRING("me"))
        CASE_LESS_OR_EQUAL(STG8_END_AFTER_THE_BATTLE)
        CALL(evt_msg_print, 0, STRING("stg4_usu_37_1"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg4_usu_37_1_1"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

/**
 * Init script of villager D (human form): enables move/talk animation and keeps him below the map
 * until the Doopliss fight (STG4_10).
 */
EVT_BEGIN(villagerD_init)
    CALL(evt_npc_flag_onoff, 1, STRING("村人Ｄ"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG4_10_DOOPLISS_BATTLE)
        CALL(evt_npc_set_position, STRING("村人Ｄ"), 0, -2000, 0)
        CASE_DEFAULT
    END_SWITCH
    RETURN
EVT_END()

/** Init script of the pig form of villager D: shown until STG4_10, then moved below the map. */
EVT_BEGIN(villagerD_pig_init)
    CALL(evt_npc_flag_onoff, 1, STRING("ブタ村人Ｄ"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG4_10_DOOPLISS_BATTLE)
        CASE_DEFAULT
        CALL(evt_npc_set_position, STRING("ブタ村人Ｄ"), 0, -2000, 0)
    END_SWITCH
    RETURN
EVT_END()

/**
 * Talk script of villager D: the Bowser interlude line, an oink plus pig text before the Doopliss fight,
 * then human text by story progress.
 */
EVT_BEGIN(villagerD_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(STG5_48_BOWSER_TWILIGHT_TOWN)
        CALL(evt_msg_print, 0, STRING("kpa_stg5_008"), 0, STRING("me"))
        CASE_LESS(STG4_10_DOOPLISS_BATTLE)
        CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG4_VOICE_PIG1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_msg_print, 0, STRING("stg4_usu_38"), 0, STRING("me"))
        CASE_LESS(STG4_16_LEAVING_TWILIGHT_TOWN)
        CALL(evt_msg_print, 0, STRING("stg4_usu_39"), 0, STRING("me"))
        CASE_BETWEEN(STG7_02_MAYOR_BOB, STG7_02_MAYOR_BOB)
        IF_INT_EQUAL(GSWF_MRI_GENERAL_WHITE_HINT_HEARD, 1)
            SET_VALUE(GSWF_USU_GENERAL_WHITE_HINT_HEARD, 1)
            CALL(evt_msg_print, 0, STRING("stg7_usu_01"), 0, STRING("me"))
            ELSE
            CALL(evt_msg_print, 0, STRING("stg4_usu_40"), 0, STRING("me"))
        END_IF
        CASE_LESS(STG4_31_FRANKLYS_HOUSE_STG_4)
        CALL(evt_msg_print, 0, STRING("stg4_usu_40"), 0, STRING("me"))
        CASE_LESS_OR_EQUAL(STG8_END_AFTER_THE_BATTLE)
        CALL(evt_msg_print, 0, STRING("stg4_usu_40_1"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg4_usu_40_1_1"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

/**
 * Init script of villager E (human form): enables move/talk animation and keeps him below the map
 * until the Doopliss fight (STG4_10).
 */
EVT_BEGIN(villagerE_init)
    CALL(evt_npc_flag_onoff, 1, STRING("村人Ｅ"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG4_10_DOOPLISS_BATTLE)
        CALL(evt_npc_set_position, STRING("村人Ｅ"), 0, -2000, 0)
        CASE_DEFAULT
    END_SWITCH
    RETURN
EVT_END()

/** Init script of the pig form of villager E: shown until STG4_10, then moved below the map. */
EVT_BEGIN(villagerE_pig_init)
    CALL(evt_npc_flag_onoff, 1, STRING("ブタ村人Ｅ"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG4_10_DOOPLISS_BATTLE)
        CASE_DEFAULT
        CALL(evt_npc_set_position, STRING("ブタ村人Ｅ"), 0, -2000, 0)
    END_SWITCH
    RETURN
EVT_END()

/**
 * Talk script of villager E: the Bowser interlude line, an oink plus pig text before the Doopliss fight,
 * then human text by story progress.
 */
EVT_BEGIN(villagerE_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(STG5_48_BOWSER_TWILIGHT_TOWN)
        CALL(evt_msg_print, 0, STRING("kpa_stg5_009"), 0, STRING("me"))
        CASE_LESS(STG4_10_DOOPLISS_BATTLE)
        CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG4_VOICE_PIG1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_msg_print, 0, STRING("stg4_usu_41"), 0, STRING("me"))
        CASE_LESS(STG4_16_LEAVING_TWILIGHT_TOWN)
        CALL(evt_msg_print, 0, STRING("stg4_usu_42"), 0, STRING("me"))
        CASE_LESS(STG4_31_FRANKLYS_HOUSE_STG_4)
        CALL(evt_msg_print, 0, STRING("stg4_usu_43"), 0, STRING("me"))
        CASE_LESS_OR_EQUAL(STG8_END_AFTER_THE_BATTLE)
        CALL(evt_msg_print, 0, STRING("stg4_usu_43_1"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg4_usu_43_1_1"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

/**
 * Init script of villager K (human form): enables move/talk animation and keeps him below the map
 * until the Doopliss fight (STG4_10).
 */
EVT_BEGIN(villagerK_init)
    CALL(evt_npc_flag_onoff, 1, STRING("村人Ｋ"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG4_10_DOOPLISS_BATTLE)
        CALL(evt_npc_set_position, STRING("村人Ｋ"), 0, -2000, 0)
        CASE_DEFAULT
    END_SWITCH
    RETURN
EVT_END()

/** Init script of the pig form of villager K: shown until STG4_10, then moved below the map. */
EVT_BEGIN(villagerK_pig_init)
    CALL(evt_npc_flag_onoff, 1, STRING("ブタ村人Ｋ"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG4_10_DOOPLISS_BATTLE)
        CASE_DEFAULT
        CALL(evt_npc_set_position, STRING("ブタ村人Ｋ"), 0, -2000, 0)
    END_SWITCH
    RETURN
EVT_END()

/**
 * Talk script of villager K: the Bowser interlude line, an oink plus pig text before the Doopliss fight,
 * then human text by story progress.
 */
EVT_BEGIN(villagerK_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(STG5_48_BOWSER_TWILIGHT_TOWN)
        CALL(evt_msg_print, 0, STRING("kpa_stg5_006"), 0, STRING("me"))
        CASE_LESS(STG4_10_DOOPLISS_BATTLE)
        CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG4_VOICE_PIG1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_msg_print, 0, STRING("stg4_usu_43_01"), 0, STRING("me"))
        CASE_LESS(STG4_16_LEAVING_TWILIGHT_TOWN)
        CALL(evt_msg_print, 0, STRING("stg4_usu_43_02"), 0, STRING("me"))
        CASE_LESS(STG4_31_FRANKLYS_HOUSE_STG_4)
        CALL(evt_msg_print, 0, STRING("stg4_usu_43_03"), 0, STRING("me"))
        CASE_LESS_OR_EQUAL(STG8_END_AFTER_THE_BATTLE)
        CALL(evt_msg_print, 0, STRING("stg4_usu_43_03_1"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg4_usu_43_03_1_1"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

/**
 * Init script of the village chief (the Mayor, human form):
 * hidden below the map from the first gatekeeper conversation (STG4_01_00)
 * until the Super Bombomb is lost (STG4_11).
 */
EVT_BEGIN(village_chief_init)
    CALL(evt_npc_flag_onoff, 1, STRING("村長"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG4_01_00_GATEKEEPER_CONVERSATION)
        CASE_LESS(STG4_11_SUPERBOMBBOMB_LOST)
        CALL(evt_npc_set_position, STRING("村長"), 0, -2000, 0)
    END_SWITCH
    RETURN
EVT_END()

/**
 * Init script of the pig Mayor: visible only between the gatekeeper conversation (STG4_01_00)
 * and the loss of the Super Bombomb (STG4_11), otherwise parked below the map.
 */
EVT_BEGIN(village_chief_pig_init)
    CALL(evt_npc_flag_onoff, 1, STRING("ブタ村長"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG4_01_00_GATEKEEPER_CONVERSATION)
        CALL(evt_npc_set_position, STRING("ブタ村長"), 0, -2000, 0)
        CASE_LESS(STG4_11_SUPERBOMBBOMB_LOST)
        CASE_DEFAULT
        CALL(evt_npc_set_position, STRING("ブタ村長"), 0, -2000, 0)
    END_SWITCH
    RETURN
EVT_END()

/** Item menu choices in the Mayor's trouble 17: only the Routing Slip, which Mario hands over to finish it. */
s32 iri_15_item_tbl[] = {
    ITEM_ROUTING_SLIP, -1,
};

/**
 * Talk script of the Mayor.
 * While trouble 17 is active it gives Mario the Routing Slip,
 * then takes it back (item menu) and pays the reward; otherwise it says the line for the current story progress.
 */
EVT_BEGIN(village_chief_talk)
    IF_INT_EQUAL(GSW_TROUBLE_ACCEPTED, TROUBLE_NEWSLETTER)
        IF_INT_EQUAL(GSWF_TROUBLE_WORK(0), 0)
            CALL(evt_msg_print, 0, STRING("irai_17_00"), 0, 0)
            CALL(evt_item_entry, STRING("kairanban"), ITEM_ROUTING_SLIP, 0, -1000, 0, ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
            CALL(evt_item_get_item, STRING("kairanban"))
            CALL(evt_item_get_item_end_wait, STRING("kairanban"))
            SET_VALUE(GSWF_TROUBLE_WORK(0), 1)
            ELSE
            IF_INT_EQUAL(GSWF_TROUBLE_WORK(1), 0)
                CALL(evt_msg_print, 0, STRING("irai_17_01"), 0, 0)
                ELSE
                IF_INT_EQUAL(GSWF_TROUBLE_WORK(2), 0)
                    CALL(evt_msg_print, 0, STRING("irai_17_02"), 0, 0)
                    ELSE
                    CALL(evt_msg_print, 0, STRING("irai_17_03"), 0, 0)
#if VERSION <= VERSION_DEMO_US
                    CALL(evt_win_item_select, PTR(iri_15_item_tbl), LW(0))
#else
                    CALL(evt_win_item_select, 0, PTR(iri_15_item_tbl), LW(0), 0)
#endif
                    IF_INT_NOT_EQUAL(LW(0), -1)
                        CALL(evt_pouch_remove_item, ITEM_ROUTING_SLIP, LW(0))
                        CALL(evt_msg_print, 0, STRING("irai_17_04"), 0, 0)
                        CALL(evt_sub_get_coin, 30)
                        CALL(evt_msg_print, 0, STRING("irai_17_05"), 0, 0)
                        SET_VALUE(GSW_TROUBLE_ACCEPTED, TROUBLE_NONE)
                        SET_VALUE(GSWF_TROUBLE_DONE(TROUBLE_NEWSLETTER), 1)
                        CALL(evt_msg_print, 0, STRING("sys_request_finish"), 0, 0)
                    END_IF
                END_IF
            END_IF
        END_IF
        RETURN
    END_IF
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG4_01_THE_MAYORS_A_PIG)
        CALL(evt_msg_print, 0, STRING("stg4_usu_20"), 0, STRING("me"))
        CASE_LESS(STG4_10_DOOPLISS_BATTLE)
        CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG4_VOICE_PIG1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_msg_print, 0, STRING("stg4_usu_22"), 0, STRING("me"))
        CASE_LESS(STG4_14_NAME_GUESSING)
        IF_INT_EQUAL(GSWF_USU_00_MAYOR_SHADOW_TALK_DONE, 0)
            SET_VALUE(GSWF_USU_00_MAYOR_SHADOW_TALK_DONE, 1)
            CALL(evt_msg_print, 0, STRING("stg4_usu_24"), 0, STRING("me"))
            CALL(evt_set_dir_to_target, STRING("me"), STRING("にせマリオ"))
            ELSE
            CALL(evt_msg_print, 0, STRING("stg4_usu_25"), 0, STRING("me"))
            CALL(evt_set_dir_to_target, STRING("me"), STRING("にせマリオ"))
        END_IF
        CASE_LESS(STG4_16_LEAVING_TWILIGHT_TOWN)
        CALL(evt_msg_print, 0, STRING("stg4_usu_25"), 0, STRING("me"))
        CASE_LESS(STG4_31_FRANKLYS_HOUSE_STG_4)
        CALL(evt_msg_print, 0, STRING("stg4_usu_157"), 0, STRING("me"))
        CASE_LESS_OR_EQUAL(STG8_END_AFTER_THE_BATTLE)
        CALL(evt_msg_print, 0, STRING("stg4_usu_157_1"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg4_usu_157_1_1"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

/**
 * Init script of the Mayor's lawn scene: spawns the stand-in Mario ("nise" Mario), Goombella, Koops and Flurrie,
 * plus a baby Yoshi model chosen by the Yoshi partner's color.
 */
EVT_BEGIN(chief_mario_talk_init)
    CALL(evt_npc_entry, STRING("にせマリオ"), STRING("c_mario"))
    CALL(evt_npc_entry, STRING("クリスチーヌ"), STRING("c_pkr"))
    CALL(evt_npc_entry, STRING("ノコタロウ"), STRING("c_pnk"))
    CALL(evt_npc_entry, STRING("クラウダ"), STRING("c_windy"))
    CALL(getYoshiColor, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        SET_VALUE(LW(0), STRING("c_babyyoshi"))
        CASE_EQUAL(1)
        SET_VALUE(LW(0), STRING("c_babyyoshi2"))
        CASE_EQUAL(2)
        SET_VALUE(LW(0), STRING("c_babyyoshi3"))
        CASE_EQUAL(3)
        SET_VALUE(LW(0), STRING("c_babyyoshi4"))
        CASE_EQUAL(4)
        SET_VALUE(LW(0), STRING("c_babyyoshi5"))
        CASE_EQUAL(5)
        SET_VALUE(LW(0), STRING("c_babyyoshi6"))
        CASE_EQUAL(6)
        SET_VALUE(LW(0), STRING("c_babyyoshi7"))
    END_SWITCH
    CALL(evt_npc_entry, STRING("チビヨッシー"), LW(0))
    CALL(evt_npc_set_tribe, STRING("にせマリオ"), STRING("にせマリオ"))
    CALL(evt_npc_set_tribe, STRING("クリスチーヌ"), STRING("クリスチーヌ"))
    CALL(evt_npc_set_tribe, STRING("ノコタロウ"), STRING("ノコタロウ"))
    CALL(evt_npc_set_tribe, STRING("クラウダ"), STRING("クラウダ"))
    CALL(evt_npc_set_tribe, STRING("チビヨッシー"), STRING("ヨッシー"))
    CALL(evt_npc_flag_onoff, 1, STRING("にせマリオ"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_flag_onoff, 1, STRING("クリスチーヌ"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_flag_onoff, 1, STRING("ノコタロウ"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_flag_onoff, 1, STRING("クラウダ"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_flag_onoff, 1, STRING("チビヨッシー"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_set_position, STRING("にせマリオ"), -3, 0, -388)
    CALL(evt_npc_set_position, STRING("クリスチーヌ"), -34, 0, -347)
    CALL(evt_npc_set_position, STRING("ノコタロウ"), -75, 0, -316)
    CALL(evt_npc_set_position, STRING("クラウダ"), -125, 0, -341)
    CALL(evt_npc_set_position, STRING("チビヨッシー"), -105, 0, -291)
    CALL(evt_npc_set_ry, STRING("にせマリオ"), 90)
    CALL(evt_npc_set_ry, STRING("クリスチーヌ"), 90)
    CALL(evt_npc_set_ry, STRING("ノコタロウ"), 90)
    CALL(evt_npc_set_ry, STRING("クラウダ"), 90)
    CALL(evt_npc_set_ry, STRING("チビヨッシー"), 90)
    CALL(evt_npc_change_interrupt, STRING("にせマリオ"), 6, PTR(nise_mario_talk))
    CALL(evt_npc_change_interrupt, STRING("クリスチーヌ"), 6, PTR(christine_talk))
    CALL(evt_npc_change_interrupt, STRING("ノコタロウ"), 6, PTR(nokotaro_talk))
    CALL(evt_npc_change_interrupt, STRING("クラウダ"), 6, PTR(cloud_talk))
    CALL(evt_npc_change_interrupt, STRING("チビヨッシー"), 6, PTR(yoshi_talk))
    RETURN
EVT_END()

EVT_BEGIN(chief_mario_talk)
    IF_INT_NOT_EQUAL(GSWF_USU_00_MAYOR_THANKED_FAKE_MARIO, 0)
        RETURN
    END_IF
    LABEL(0)
    CALL(evt_npc_get_position, STRING("村長"), LW(0), LW(1), LW(2))
    CALL(evt_mario_get_pos, 0, LW(3), LW(4), LW(5))
    CALL(evt_sub_get_dist, LW(0), LW(2), LW(3), LW(5), LW(6))
    IF_INT_GREATER(LW(6), 100)
        WAIT_FRAMES(1)
        GOTO(0)
    END_IF
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_set_dir_to_target, STRING("村長"), STRING("にせマリオ"))
    CALL(evt_msg_print, 0, STRING("stg4_usu_23"), 0, STRING("村長"))
    SET_VALUE(GSWF_USU_00_MAYOR_THANKED_FAKE_MARIO, 1)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/**
 * Init script of Vivian in Twilight Town: enables her move and talk animations;
 * from STG4_11_02_WAITING_DOOPLISS to STG4_12_01_SUPERBOMBBOMB_FOUND she uses the tearful tribe
 * "ビビアンべそ" with its stand and talk poses (PTR_S_3, PTR_T_3), stands at (-71, 0, 329) facing right,
 * and from STG4_12_VIVIAN on also wanders (urouro_move)
 * while she searches for the lost Super Bombomb (her line stg4_usu_109).
 */
static EVT_BEGIN(vivian_init)
    CALL(evt_npc_flag_onoff, 1, STRING("ビビアン"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG4_11_02_WAITING_DOOPLISS, STG4_12_01_SUPERBOMBBOMB_FOUND)
        CALL(evt_npc_set_tribe, STRING("ビビアン"), STRING("ビビアンべそ"))
        CALL(evt_npc_set_autotalkpose, STRING("me"), STRING("PTR_S_3"), STRING("PTR_T_3"))
        CALL(evt_npc_set_position, STRING("me"), -71, 0, 329)
        CALL(evt_npc_set_ry, STRING("me"), 90)
        IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG4_12_VIVIAN)
            CALL(evt_npc_change_interrupt, STRING("me"), 1, PTR(urouro_move))
            CALL(evt_npc_restart_regular_event, STRING("me"))
        END_IF
    END_SWITCH
    RETURN
EVT_END()

/** Item menu choices when Mario talks to Vivian while she searches: only the Super Bombomb. */
static s32 key_tbl[] = {
    ITEM_SUPERBOMBOMB, -1,
};

/**
 * Talk script of Vivian: if Mario carries the Super Bombomb he can pick it from the item menu,
 * which starts usu00_vivian_joinup_event; otherwise she says "stg4_usu_109".
 */
static EVT_BEGIN(vivian_talk)
    CALL(evt_pouch_check_item, ITEM_SUPERBOMBOMB, LW(0))
    IF_INT_GREATER(LW(0), 0)
#if VERSION <= VERSION_DEMO_US
        CALL(evt_win_item_select, PTR(key_tbl), LW(0))
#else
        CALL(evt_win_item_select, 0, PTR(key_tbl), LW(0), 0)
#endif
        IF_INT_NOT_EQUAL(LW(0), -1)
            CALL_CHILD(PTR(usu00_vivian_joinup_event))
            RETURN
        END_IF
    END_IF
    CALL(evt_msg_print, 0, STRING("stg4_usu_109"), 0, STRING("me"))
    RETURN
EVT_END()

/** Talk script of the stand-in Mario NPC: one line, "stg4_usu_25_01". */
EVT_BEGIN(nise_mario_talk)
    CALL(evt_msg_print, 0, STRING("stg4_usu_25_01"), 0, STRING("me"))
    RETURN
EVT_END()

/** Talk script of the Goombella (Christine) stand-in: one line, "stg4_usu_25_02". */
EVT_BEGIN(christine_talk)
    CALL(evt_msg_print, 0, STRING("stg4_usu_25_02"), 0, STRING("me"))
    RETURN
EVT_END()

/** Talk script of the Koops (Nokotaro) stand-in: one line, "stg4_usu_25_03". */
EVT_BEGIN(nokotaro_talk)
    CALL(evt_msg_print, 0, STRING("stg4_usu_25_03"), 0, STRING("me"))
    RETURN
EVT_END()

/** Talk script of the Flurrie (Clouda) stand-in: one line, "stg4_usu_25_04". */
EVT_BEGIN(cloud_talk)
    CALL(evt_msg_print, 0, STRING("stg4_usu_25_04"), 0, STRING("me"))
    RETURN
EVT_END()

/** Talk script of the Yoshi stand-in: one line, "stg4_usu_25_05". */
EVT_BEGIN(yoshi_talk)
    CALL(evt_msg_print, 0, STRING("stg4_usu_25_05"), 0, STRING("me"))
    RETURN
EVT_END()

/** Init script of crow A: puts it on its perch at (-420, 90, 26). */
EVT_BEGIN(crow_a_init)
    CALL(evt_npc_set_position, STRING("me"), -420, 90, 26)
    RETURN
EVT_END()

/** Init script of crow B: puts it on its perch at (-405, 60, 30). */
EVT_BEGIN(crow_b_init)
    CALL(evt_npc_set_position, STRING("me"), -405, 60, 30)
    RETURN
EVT_END()

/** Init script of crow C: puts it on its perch at (29, 120, -215). */
EVT_BEGIN(crow_c_init)
    CALL(evt_npc_set_position, STRING("me"), 29, 120, -215)
    RETURN
EVT_END()

/** Init script of crow D: puts it on its perch at (75, 110, -233). */
EVT_BEGIN(crow_d_init)
    CALL(evt_npc_set_position, STRING("me"), 75, 110, -233)
    RETURN
EVT_END()

/** Init script of crow E: puts it on its perch at (104, 110, -233). */
EVT_BEGIN(crow_e_init)
    CALL(evt_npc_set_position, STRING("me"), 104, 110, -233)
    RETURN
EVT_END()

/**
 * Wandering script of Villagers C and E, the pig villagers A, D and E, and Vivian while she searches:
 * from where it stands, the NPC wanders within 50 units of that spot at speed 30 with the shared urouro helpers.
 */
static EVT_BEGIN(urouro_move)
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(urouro_init_func, STRING("me"), LW(0), LW(2), FLOAT(50.0f), FLOAT(30.0f), 0)
    CALL(urouro_main_func, STRING("me"))
    RETURN
EVT_END()

/**
 * NPCs of this map: the human villagers A to E and K, the Mayor, Vivian and the innkeeper,
 * their pig forms (pig villagers A, D, E, K and the pig Mayor) and the five crows A to E on the rooftops.
 */
static NpcSetupInfo npcEnt[] = {
    {
        /*"村人Ａ"*/"\x91\xBA\x90\x6C\x82\x60", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        villagerA_init, // initEvent
        NULL, // regularEvent
        villagerA_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"村人Ｂ"*/"\x91\xBA\x90\x6C\x82\x61", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        villagerB_init, // initEvent
        NULL, // regularEvent
        villagerB_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"村人Ｃ"*/"\x91\xBA\x90\x6C\x82\x62", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR, // flags
        0, // reactionFlags
        villagerC_init, // initEvent
        urouro_move, // regularEvent
        villagerC_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"村人Ｄ"*/"\x91\xBA\x90\x6C\x82\x63", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        villagerD_init, // initEvent
        NULL, // regularEvent
        villagerD_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"村人Ｅ"*/"\x91\xBA\x90\x6C\x82\x64", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR, // flags
        0, // reactionFlags
        villagerE_init, // initEvent
        urouro_move, // regularEvent
        villagerE_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"村人Ｋ"*/"\x91\xBA\x90\x6C\x82\x6A", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        villagerK_init, // initEvent
        NULL, // regularEvent
        villagerK_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"村長"*/"\x91\xBA\x92\xB7", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        village_chief_init, // initEvent
        NULL, // regularEvent
        village_chief_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ビビアン"*/"\x83\x72\x83\x72\x83\x41\x83\x93", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        vivian_init, // initEvent
        NULL, // regularEvent
        vivian_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"宿店員"*/"\x8F\x68\x93\x58\x88\xF5", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        npc_init_evt, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ブタ村人Ａ"*/"\x83\x75\x83\x5E\x91\xBA\x90\x6C\x82\x60", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR, // flags
        0, // reactionFlags
        villagerA_pig_init, // initEvent
        urouro_move, // regularEvent
        villagerA_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ブタ村人Ｄ"*/"\x83\x75\x83\x5E\x91\xBA\x90\x6C\x82\x63", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR, // flags
        0, // reactionFlags
        villagerD_pig_init, // initEvent
        urouro_move, // regularEvent
        villagerD_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ブタ村人Ｅ"*/"\x83\x75\x83\x5E\x91\xBA\x90\x6C\x82\x64", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR, // flags
        0, // reactionFlags
        villagerE_pig_init, // initEvent
        urouro_move, // regularEvent
        villagerE_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ブタ村人Ｋ"*/"\x83\x75\x83\x5E\x91\xBA\x90\x6C\x82\x6A", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR, // flags
        0, // reactionFlags
        villagerK_pig_init, // initEvent
        urouro_move, // regularEvent
        villagerK_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ブタ村長"*/"\x83\x75\x83\x5E\x91\xBA\x92\xB7", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR, // flags
        0, // reactionFlags
        village_chief_pig_init, // initEvent
        urouro_move, // regularEvent
        village_chief_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"カラスＡ"*/"\x83\x4A\x83\x89\x83\x58\x82\x60", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        crow_a_init, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"カラスＢ"*/"\x83\x4A\x83\x89\x83\x58\x82\x61", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        crow_b_init, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"カラスＣ"*/"\x83\x4A\x83\x89\x83\x58\x82\x62", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        crow_c_init, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"カラスＤ"*/"\x83\x4A\x83\x89\x83\x58\x82\x63", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        crow_d_init, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"カラスＥ"*/"\x83\x4A\x83\x89\x83\x58\x82\x64", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        crow_e_init, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

/**
 * Chapter 4 opening cutscene: Mario arrives in Twilight Town, a human villager turns out to be a pig,
 * the Mayor explains and Mario's party reacts; the town gate rotates open at the end.
 */
EVT_BEGIN(usu00_stage4_coming_event)
    // The camera drops from the sky as Mario and the party arrive by pipe; the partner looks around.
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_snd_bgm_scope, 0, 1)
    CALL(evt_cam_letter_box_onoff, 1, 0)
    CALL(evt_npc_get_position, STRING("ブタ村人Ａ"), LW(0), LW(1), LW(2))
    CALL(evt_npc_set_position, STRING("村人Ａ"), LW(0), LW(1), LW(2))
    CALL(evt_npc_set_position, STRING("ブタ村人Ａ"), 0, -2000, 0)
    CALL(evt_cam3d_evt_set, -255, 593, 704, -255, 497, 162, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_seq_wait, SEQ_GAME)
    WAIT_MS(1000)
    CALL(evt_npc_set_tribe, STRING("村人Ａ"), STRING("ウスグラ村人音付き"))
    CALL(evt_npc_set_tribe, STRING("村長"), STRING("ウスグラ村長音付き"))
    CALL(evt_cam3d_evt_set, -255, 166, 704, -255, 71, 162, 5000, EASING_LINEAR)
    WAIT_MS(5500)
    SET_VALUE(GF(0), 0)
    CALL(evt_bero_exec_wait, 65536)
    WAIT_MS(600)
    CALL(evt_party_stop, 0)
    BEGIN_INLINE_EVENT
        CALL(evt_party_move_pos2, 0, -255, 175, FLOAT(60.0f))
    END_INLINE_EVENT
    CALL(evt_mario_mov_pos2, -225, 175, FLOAT(60.0f))
    CALL(evt_party_set_dir, 0, 270, 300)
    WAIT_MS(300)
    CALL(evt_party_set_dir, 0, 90, 300)
    WAIT_MS(300)
    CALL(evt_party_set_dir, 0, 270, 300)
    WAIT_MS(300)
    CALL(evt_mario_set_dir, 270, 300, 0)
    WAIT_MS(300)
    CALL(evt_party_set_dir, 0, 90, 300)
    WAIT_MS(300)
    CALL(evt_party_stop, 0)
    WAIT_MS(300)
    // A villager greets the "adventurers" from outside and starts to tell of the town's trouble.
    CALL(evt_msg_print_party, STRING("stg4_usu_00_0"))
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("stg4_usu_00"), 0, STRING("村人Ａ"))
    CALL(evt_mario_set_dir, 90, 200, 0)
    CALL(evt_cam3d_evt_set, -210, 100, 419, -210, 41, 185, 1000, EASING_SINE_IN_OUT)
    CALL(evt_npc_move_position, STRING("村人Ａ"), -175, 175, 1000, 0, 0)
    CALL(evt_npc_set_ry, STRING("村人Ａ"), 270)
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        CALL(evt_party_set_dir, 0, 90, 300)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_mario_set_dir, 90, 300, 0)
    END_INLINE_EVENT
    CALL(evt_msg_print, 0, STRING("stg4_usu_01"), 0, STRING("村人Ａ"))
    WAIT_MS(100)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_NOD1_5"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING("M_N_2"))
    WAIT_MS(400)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_msg_print, 0, STRING("stg4_usu_02"), 0, STRING("村人Ａ"))
    WAIT_MS(200)
    CALL(evt_msg_print_party, STRING("stg4_usu_03"))
    WAIT_MS(200)
    CALL(evt_msg_print, 0, STRING("stg4_usu_04"), 0, STRING("村人Ａ"))
    // The steeple bell rings and the villager turns into a pig before Mario's eyes.
    WAIT_MS(500)
    CALL(evt_snd_sfxon, STRING("SFX_STG4_TEMPLE_BELL1"), 0)
    WAIT_MS(500)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_NPC, 0, STRING("村人Ａ"), 0, 0, 0, 0, 0, 0, 0, 60)
    WAIT_MS(1000)
    CALL(evt_npc_reverse_ry, STRING("村人Ａ"))
    WAIT_MS(500)
    CALL(evt_npc_reverse_ry, STRING("村人Ａ"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg4_usu_05"), 0, STRING("村人Ａ"))
    WAIT_MS(500)
    CALL(evt_npc_get_position, STRING("村人Ａ"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG4_CHANGE_PIG1"), LW(0), LW(1), LW(2), 0)
    BEGIN_INLINE_EVENT
        ADD(LW(2), 20)
        CALL(evt_eff, 0, STRING("kemuri_test"), 7, LW(0), LW(1), LW(2), FLOAT(1.5f), 0, 0, 0, 0, 0, 0, 0)
    END_INLINE_EVENT
    WAIT_MS(300)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_SURPRISED2"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_mario_set_pose, STRING("M_N_5B"))
    CALL(evt_npc_get_reglid, STRING("ブタ村人Ａ"), LW(10))
    DELETE_EVENT(LW(10))
    CALL(evt_npc_set_position, STRING("ブタ村人Ａ"), LW(0), LW(1), LW(2))
    CALL(evt_npc_set_position, STRING("村人Ａ"), 0, -2000, 0)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_MARIO, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 60)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_PARTNER, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 60)
    WAIT_MS(1000)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_npc_get_position, STRING("ブタ村人Ａ"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG4_VOICE_PIG1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_msg_print, 0, STRING("stg4_usu_06"), 0, STRING("ブタ村人Ａ"))
    BEGIN_INLINE_EVENT
        WAIT_MS(500)
        CALL(evt_npc_move_position, STRING("ブタ村人Ａ"), -197, 90, 0, FLOAT(40.0f), 0)
        CALL(evt_npc_move_position, STRING("ブタ村人Ａ"), -276, -14, 0, FLOAT(40.0f), 0)
        CALL(evt_npc_restart_regular_event, STRING("ブタ村人Ａ"))
    END_INLINE_EVENT
    CALL(evt_npc_flag_onoff, 1, STRING("ブタ村人Ａ"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_flag_onoff, 1, STRING("ブタ村人Ｄ"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_flag_onoff, 1, STRING("ブタ村人Ｋ"), NPC_FLAG_NO_HIT)
    CALL(evt_msg_print_party, STRING("stg4_usu_07"))
    // The Mayor (Dour) appears, introduces himself and leads them to his house.
    WAIT_MS(1000)
    CALL(evt_npc_get_position, STRING("村長"), LW(0), LW(1), LW(2))
    CALL(evt_npc_set_position, STRING("村長"), -40, 0, 184)
    CALL(evt_msg_print, 0, STRING("stg4_usu_08"), 0, STRING("村長"))
    CALL(evt_npc_move_position, STRING("村長"), -175, 175, 4000, 0, 0)
    WAIT_MS(200)
    CALL(evt_msg_print_party, STRING("stg4_usu_09"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg4_usu_10"), 0, STRING("村長"))
    BEGIN_INLINE_EVENT
        CALL(evt_npc_set_ry, STRING("村長"), 90)
        WAIT_MS(100)
        CALL(evt_npc_move_position, STRING("村長"), -77, -169, 5000, 0, 0)
        WAIT_MS(500)
        CALL(evt_npc_move_position, STRING("村長"), -77, -190, 500, 0, 0)
        CALL(evt_npc_set_position, STRING("村長"), LW(0), LW(1), LW(2))
        CALL(evt_npc_set_ry, STRING("村長"), 270)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(5100)
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 180, 500)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
            CALL(evt_mapobj_rotate, 1, STRING("S__ie2_door"), 0, LW(0), 0)
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
        WAIT_MS(500)
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 180, 0, 500)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
            CALL(evt_mapobj_rotate, 1, STRING("S__ie2_door"), 0, LW(0), 0)
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
    END_INLINE_EVENT
    // Mario follows the Mayor to his front door while the chapter title card plays.
    WAIT_MS(600)
    CALL(evt_party_set_dir, 0, 270, 300)
    WAIT_MS(500)
    CALL(evt_party_set_dir, 0, 90, 300)
    WAIT_MS(500)
    CALL(evt_party_run, 0)
    CALL(evt_cam3d_evt_set, -428, 588, 1255, -15, -195, -327, 5000, EASING_SINE_IN_OUT)
    BEGIN_INLINE_EVENT
        CALL(evt_telop_entry, 7)
    END_INLINE_EVENT
    CALL(evt_mario_mov_pos2, -215, 137, FLOAT(60.0f))
    CALL(evt_mario_mov_pos2, -95, -105, FLOAT(60.0f))
    CALL(evt_cam3d_evt_set, -45, 132, 111, -45, 49, -200, 0, EASING_SINE_IN_OUT)
    CALL(evt_npc_set_position, STRING("村長"), 5, 0, -340)
    CALL(evt_party_stop, 0)
    BEGIN_INLINE_EVENT
        CALL(evt_party_move_pos2, 0, -98, -131, FLOAT(60.0f))
    END_INLINE_EVENT
    CALL(evt_mario_mov_pos2, -79, -180, FLOAT(60.0f))
    CALL(evt_party_run, 0)
    // Inside, the Mayor tells of the curse: the Creepy Steeple bell turns a villager into a pig with each ring.
    SET_VALUE(LW(0), PTR(&door_2))
    SET_VALUE(LW(15), 0)
    CALL_CHILD(PTR(evt_door_open))
    CALL(evt_cam3d_evt_set, -45, 132, 111, -45, 49, -361, 1000, EASING_SINE_IN_OUT)
    CALL(evt_party_stop, 0)
    BEGIN_INLINE_EVENT
        CALL(evt_party_move_pos2, 0, -80, -320, FLOAT(60.0f))
        CALL(evt_party_set_dir, 0, 90, 300)
    END_INLINE_EVENT
    CALL(evt_mario_mov_pos2, -55, -335, FLOAT(60.0f))
    CALL(evt_mario_set_dir, 90, 300, 0)
    CALL(evt_msg_print, 0, STRING("stg4_usu_11"), 0, STRING("村長"))
    CALL(evt_mario_set_pose, STRING("M_I_2"))
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg4_usu_12"), 0, STRING("村長"))
    CALL(evt_cam3d_evt_set, -70, 100, -16, -70, 39, -361, 400, EASING_SINE_IN_OUT)
    WAIT_MS(400)
    CALL(evt_party_stop, 0)
    WAIT_MS(200)
    CALL(evt_msg_print_party, STRING("stg4_usu_13"))
    CALL(evt_cam3d_evt_set, -40, 80, -90, -40, 33, -361, 900, EASING_SINE_IN_OUT)
    WAIT_MS(900)
    CALL(evt_msg_print, 0, STRING("stg4_usu_14"), 0, STRING("村長"))
    CALL(evt_mario_set_pose, STRING("M_N_5B"))
    WAIT_MS(600)
    CALL(evt_msg_print, 0, STRING("stg4_usu_15"), 0, STRING("村長"))
    // The partner asks about the Crystal Star; the Mayor tells of a red star stone in the steeple and is horrified
    // that they mean to go there.
    WAIT_MS(300)
    CALL(evt_party_move_pos2, 0, -60, -305, FLOAT(60.0f))
    WAIT_MS(300)
    CALL(evt_party_stop, 0)
    WAIT_MS(200)
    CALL(evt_msg_print_party, STRING("stg4_usu_16"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg4_usu_17"), 0, STRING("村長"))
    WAIT_MS(250)
    CALL(evt_msg_print_party, STRING("stg4_usu_18"))
    WAIT_MS(250)
    CALL(evt_msg_print, 0, STRING("stg4_usu_19"), 0, STRING("村長"))
    CALL(evt_cam3d_evt_off, 1000, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    SET_VALUE(GSW_STORY_PROGRESS, STG4_00_WELCOME_TO_STAGE_4)
    CALL(evt_snd_bgm_scope, 0, 0)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_party_run, 0)
    CALL(evt_npc_flag_onoff, 0, STRING("ブタ村人Ａ"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_flag_onoff, 0, STRING("ブタ村人Ｄ"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_flag_onoff, 0, STRING("ブタ村人Ｋ"), NPC_FLAG_NO_HIT)
    RETURN
EVT_END()

/**
 * Spawns the Shadow Sirens (Marilyn, Beldam as Majolyne) and Doopliss (Ranpel) as NPCs with no wall hit
 * or shadow collision, for their cutscenes.
 */
EVT_BEGIN(usu00_3witch_init)
    CALL(evt_npc_entry, STRING("マリリン"), STRING("c_maririn"))
    CALL(evt_npc_entry, STRING("マジョリン"), STRING("c_majyorin"))
    CALL(evt_npc_entry, STRING("ランペル"), STRING("c_ranpel"))
    CALL(evt_npc_set_tribe, STRING("マリリン"), STRING("マリリン"))
    CALL(evt_npc_set_tribe, STRING("マジョリン"), STRING("マジョリン"))
    CALL(evt_npc_set_tribe, STRING("ランペル"), STRING("ランペル"))
    CALL(evt_npc_flag_onoff, 1, STRING("マリリン"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_flag_onoff, 1, STRING("マジョリン"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_flag_onoff, 1, STRING("ランペル"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR | NPC_FLAG_NO_WALL_COLLISION)
    RETURN
EVT_END()

/**
 * Cutscene in Twilight Town (Mario and the partner moved off screen):
 * Beldam gloats that the Super Bombomb will win them Mario's map and asks Vivian for it (stg4_usu_97 to 99);
 * Vivian never had it (100, 102; she switches to the troubled tribe "ビビアン困る"),
 * Beldam calls her a liar and sends her to find it or be punished (101, 103 to 106),
 * with Marilyn grunting along (98, 107).
 * Beldam and Marilyn vanish, the story becomes STG4_11_SUPERBOMBBOMB_LOST and the map changes to gra_06.
 */
EVT_BEGIN(usu00_3witch_event)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_stop, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_mario_set_pos, 0, -1000, 0)
    CALL(evt_party_set_pos, 0, 0, -1000, 0)
    CALL(evt_cam3d_evt_set, -25, 495, 530, -25, 400, -62, 0, EASING_SINE_IN_OUT)
    CALL(evt_npc_set_position, STRING("ビビアン"), -71, 0, 179)
    CALL(evt_npc_set_position, STRING("マリリン"), 23, 0, 159)
    CALL(evt_npc_set_position, STRING("マジョリン"), -9, 0, 198)
    CALL(evt_npc_set_ry, STRING("ビビアン"), 90)
    CALL(evt_npc_set_ry, STRING("マリリン"), 270)
    CALL(evt_npc_set_ry, STRING("マジョリン"), 270)
    CALL(evt_npc_set_position, STRING("ランペル"), 0, -1000, 0)
    CALL(evt_seq_wait, SEQ_GAME)
    WAIT_MS(1000)
    WAIT_MS(1000)
    CALL(evt_msg_toge, MSG_TOGE_NONE, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING("stg4_usu_97"), 0, STRING("マジョリン"))
    CALL(evt_cam3d_evt_set, -25, 100, 530, -25, 5, -62, 3000, EASING_SINE_IN_OUT)
    WAIT_MS(3500)
    CALL(evt_msg_print, 0, STRING("stg4_usu_97_01"), 0, STRING("マジョリン"))
    CALL(evt_msg_print, 0, STRING("stg4_usu_98"), 0, STRING("マリリン"))
    CALL(evt_msg_print, 0, STRING("stg4_usu_99"), 0, STRING("マジョリン"))
    CALL(evt_msg_print, 0, STRING("stg4_usu_100"), 0, STRING("ビビアン"))
    CALL(evt_npc_set_tribe, STRING("ビビアン"), STRING("ビビアン困る"))
    CALL(evt_npc_set_autotalkpose, STRING("ビビアン"), STRING("PTR_S_4"), STRING("PTR_T_5"))
    CALL(evt_cam3d_evt_set, -10, 60, 382, -10, -15, -62, 300, EASING_SINE_IN_OUT)
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("stg4_usu_101"), 0, STRING("マジョリン"))
    CALL(evt_cam3d_evt_set, -68, 68, 382, -68, -7, -62, 300, EASING_SINE_IN_OUT)
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("stg4_usu_102"), 0, STRING("ビビアン"))
    CALL(evt_cam3d_evt_set, -10, 60, 382, -10, -15, -62, 300, EASING_SINE_IN_OUT)
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("stg4_usu_103"), 0, STRING("マジョリン"))
    CALL(evt_cam3d_evt_set, -68, 68, 382, -68, -7, -62, 300, EASING_SINE_IN_OUT)
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("stg4_usu_103_02"), 0, STRING("ビビアン"))
    CALL(evt_cam3d_evt_set, -25, 100, 530, -25, 5, -62, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_msg_print, 0, STRING("stg4_usu_104"), 0, STRING("マジョリン"))
    CALL(evt_msg_print, 0, STRING("stg4_usu_105"), 0, STRING("ビビアン"))
    CALL(evt_cam3d_evt_set, 0, 85, 455, 0, -5, -62, 1200, EASING_SINE_IN_OUT)
    WAIT_MS(1200)
    CALL(evt_msg_print, 0, STRING("stg4_usu_106"), 0, STRING("マジョリン"))
    CALL(evt_msg_print, 0, STRING("stg4_usu_107"), 0, STRING("マリリン"))
    BEGIN_INLINE_EVENT
        CALL(evt_npc_majo_disp_off, STRING("マジョリン"))
    END_INLINE_EVENT
    WAIT_MS(1000)
    CALL(evt_npc_majo_disp_off, STRING("マリリン"))
    WAIT_MS(1000)
    SET_VALUE(GSW_STORY_PROGRESS, STG4_11_SUPERBOMBBOMB_LOST)
    CALL(evt_bero_mapchange, STRING("gra_06"), STRING("n_bero"))
    RETURN
EVT_END()

/**
 * Cutscene: Beldam and Marilyn find Doopliss (Ranpel),
 * set STG4_15_04_SHADOW_SIRENS_DOOPLISS and change the map to aji_10 with a story fade.
 */
EVT_BEGIN(gra00_2witch_find_ranpel_event)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_stop, 0)
    CALL(evt_mario_set_pos, -500, 0, 0)
    CALL(evt_party_set_pos, 0, -500, 0, 0)
    CALL(evt_cam3d_evt_set, 10, 100, 730, 10, 0, 88, 0, EASING_SINE_IN_OUT)
    CALL(evt_npc_set_position, STRING("ランペル"), 149, 0, 385)
    CALL(evt_npc_set_position, STRING("マリリン"), -9, 0, 348)
    CALL(evt_npc_set_position, STRING("マジョリン"), 27, 0, 348)
    CALL(evt_npc_set_ry, STRING("ランペル"), 270)
    CALL(evt_npc_set_ry, STRING("マリリン"), 90)
    CALL(evt_npc_set_ry, STRING("マジョリン"), 270)
    CALL(evt_npc_flag_onoff, 1, STRING("ランペル"), NPC_FLAG_NO_HIT | NPC_FLAG_SHADOW_NO_FLOOR | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_flag_onoff, 1, STRING("マリリン"), NPC_FLAG_NO_HIT | NPC_FLAG_SHADOW_NO_FLOOR | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_flag_onoff, 1, STRING("マジョリン"), NPC_FLAG_NO_HIT | NPC_FLAG_SHADOW_NO_FLOOR | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_flag_onoff, 1, STRING("マリリン"), NPC_FLAG_INSTANT_TURN)
    WAIT_FRAMES(1)
    CALL(evt_npc_flag_onoff, 0, STRING("マリリン"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_fade_end_wait)
    CALL(evt_msg_print, 0, STRING("stg4_usu_151"), 0, STRING("マジョリン"))
    CALL(evt_msg_print, 0, STRING("stg4_usu_152"), 0, STRING("マリリン"))
    WAIT_MS(1000)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("ランペル"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG4_LANPEL_ESCAPE1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_npc_set_anim, STRING("ランペル"), STRING("R_2"))
        CALL(evt_npc_flag_onoff, 0, STRING("ランペル"), NPC_FLAG_AUTO_MOVE_ANIM)
        CALL(evt_npc_move_position, STRING("ランペル"), -131, 385, 1200, 0, 0)
    END_INLINE_EVENT
    WAIT_MS(400)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_NPC, STRING(""), STRING("マジョリン"), 0, 0, 0, 0, 0, 0, 0, 60)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_NPC, STRING(""), STRING("マリリン"), 0, 0, 0, 0, 0, 0, 0, 60)
    WAIT_MS(250)
    CALL(evt_npc_set_ry, STRING("マリリン"), 270)
    WAIT_MS(950)
    SET_VALUE(GSW_STORY_PROGRESS, STG4_15_04_SHADOW_SIRENS_DOOPLISS)
    CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 2000)
    CALL(evt_snd_envoff_f, PSND_FADEOUT, 2000)
    CALL(evt_snd_sfx_all_stop)
    CALL(evt_fade_set_mapchange_type, 1, FADE_STORY_MARIO_A1, 0, FADE_STORY_MARIO_A3, 0)
    CALL(evt_bero_mapchange, STRING("aji_10"), 0)
    RETURN
EVT_END()

/** Cutscene: Vivian, left alone, cries and says "stg4_usu_108"; sets the story to STG4_12_VIVIAN. */
EVT_BEGIN(usu00_vivian_cry_event)
    WAIT_FRAMES(1)
    CALL(evt_npc_set_autotalkpose, STRING("ビビアン"), STRING("PTR_S_3"), STRING("PTR_T_3"))
    CALL(evt_seq_wait, SEQ_GAME)
#if VERSION > VERSION_DEMO_US
    CALL(evt_bero_exec_wait, 65536)
#endif
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    WAIT_MS(500)
    CALL(evt_cam3d_evt_set, -71, 68, 588, -71, -8, 88, 1000, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg4_usu_108"), 0, STRING("ビビアン"))
    CALL(evt_cam3d_evt_off, 0, EASING_SINE_IN_OUT)
    SET_VALUE(GSW_STORY_PROGRESS, STG4_12_VIVIAN)
    CALL(evt_npc_change_interrupt, STRING("ビビアン"), 1, PTR(urouro_move))
    CALL(evt_npc_restart_regular_event, STRING("ビビアン"))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/**
 * Cutscene where Vivian asks about the Super Bombomb:
 * Mario can hand it over from the item menu, she thanks him and (on yes) the item is spent
 * and she joins the party.
 */
EVT_BEGIN(usu00_vivian_joinup_event)
    // Vivian hears Mario found the Super Bombomb; he holds it out and she takes it, then notices it is broken.
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_npc_flag_onoff, 1, STRING("ビビアン"), NPC_FLAG_NO_HIT)
    CALL(evt_msg_print, 0, STRING("stg4_usu_110"), 0, STRING("ビビアン"))
    CALL(evt_npc_get_position, STRING("ビビアン"), LW(0), LW(1), LW(2))
    ADD(LW(0), -40)
    CALL(evt_mario_mov_pos, LW(0), LW(2), 500)
    CALL(evt_mario_set_dir, 90, 200, 0)
    CALL(evt_npc_set_ry, STRING("ビビアン"), 270)
    CALL(evt_npc_flag_onoff, 0, STRING("ビビアン"), NPC_FLAG_NO_HIT)
    CALL(evt_cam3d_evt_set_rel, 0, 52, 296, 20, 43, 26, 500, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_POSE_LOCK)
    CALL(evt_mario_set_pose, STRING("M_C_3"))
    CALL(evt_npc_get_position, STRING("ビビアン"), LW(0), LW(1), LW(2))
    ADD(LW(0), -20)
    CALL(evt_item_entry, STRING("ichikorori"), ITEM_SUPERBOMBOMB, LW(0), 10, LW(2), ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
    WAIT_MS(1000)
    CALL(evt_item_delete, STRING("ichikorori"))
    CALL(evt_mario_dump_item, ITEM_SUPERBOMBOMB)
    CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_POSE_LOCK)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_msg_print, 0, STRING("stg4_usu_111"), 0, STRING("ビビアン"))
    WAIT_MS(500)
    CALL(evt_npc_set_ry, STRING("ビビアン"), 90)
    CALL(evt_npc_get_position, STRING("ビビアン"), LW(0), LW(1), LW(2))
    ADD(LW(0), 30)
    ADD(LW(2), -1)
    CALL(evt_item_entry, STRING("shine"), ITEM_SUPERBOMBOMB, LW(0), LW(1), LW(2), ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
    WAIT_MS(700)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_NPC, STRING(""), STRING("ビビアン"), 0, 0, 0, 0, 0, 0, 0, 60)
    CALL(evt_npc_get_position, STRING("ビビアン"), LW(0), LW(1), LW(2))
    CALL(evt_npc_jump_position_nohit, STRING("ビビアン"), LW(0), LW(1), LW(2), 200, FLOAT(15.0f))
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg4_usu_112"), 0, STRING("ビビアン"))
    // Mario jumps in surprise; Vivian wails that her sister will punish her forever and Mario picks his answer.
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_POSE_LOCK)
    CALL(evt_mario_set_pose, STRING("M_N_H"))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_SURPRISED2"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_jump_pos, LW(0), LW(1), LW(2), 200, 1, -1000)
    WAIT_MS(700)
    CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_POSE_LOCK)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_npc_set_ry, STRING("ビビアン"), 270)
    CALL(evt_msg_print, 0, STRING("stg4_usu_113"), 0, STRING("ビビアン"))
    CALL(evt_msg_select, 0, STRING("stg4_usu_113_select"))
    CALL(evt_msg_continue)
    CALL(evt_mario_set_pose, STRING("M_I_2"))
    WAIT_MS(500)
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_msg_print, 0, STRING("stg4_usu_114"), 0, STRING("ビビアン"))
        CALL(evt_mario_set_pose, STRING("M_N_1"))
        WAIT_MS(1000)
        ELSE
        CALL(evt_msg_print, 0, STRING("stg4_usu_115"), 0, STRING("ビビアン"))
        CALL(evt_mario_set_pose, STRING("M_N_2"))
        WAIT_MS(500)
    END_IF
    // The broken Super Bombomb vanishes; Vivian cheers up and asks Mario's name.
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_item_get_position, STRING("shine"), LW(0), LW(1), LW(2))
    LOOP(10)
        CALL(evt_item_set_position, STRING("shine"), LW(0), LW(1), LW(2))
        WAIT_FRAMES(4)
        CALL(evt_item_set_position, STRING("shine"), 0, -1000, 0)
        WAIT_FRAMES(2)
    END_LOOP
    CALL(evt_item_delete, STRING("shine"))
    CALL(evt_npc_set_tribe, STRING("ビビアン"), STRING("ビビアン"))
    CALL(evt_npc_set_autotalkpose, STRING("ビビアン"), STRING("PTR_S_1"), STRING("PTR_T_1"))
    CALL(evt_msg_print, 0, STRING("stg4_usu_116"), 0, STRING("ビビアン"))
    CALL(evt_mario_set_pose, STRING("M_I_G"))
    WAIT_MS(750)
    CALL(evt_mario_set_pose, STRING("M_I_2"))
    WAIT_MS(500)
    // Mario mimes that his name was stolen; Vivian, moved, vows to help him get back his name and friends.
    CALL(evt_msg_print, 0, STRING("stg4_usu_117"), 0, STRING("ビビアン"))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_HAND_UP1_4"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING("M_I_2"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg4_usu_118"), 0, STRING("ビビアン"))
    CALL(evt_mario_set_pose, STRING("M_N_2"))
    WAIT_MS(500)
    CALL(evt_cam3d_evt_set, -84, 190, 1146, -41, 110, 21, 0, EASING_LINEAR)
    WAIT_MS(500)
    CALL(evt_mario_set_pose, STRING("M_I_2"))
    WAIT_MS(500)
    CALL(evt_npc_set_anim, STRING("ビビアン"), STRING("PTR_T_1"))
    WAIT_MS(3000)
    CALL(evt_npc_set_anim, STRING("ビビアン"), STRING("PTR_S_1"))
    CALL(evt_mario_set_pose, STRING("M_N_2"))
    WAIT_MS(500)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_cam3d_evt_set_rel, 0, 52, 296, 20, 43, 26, 0, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg4_usu_119"), 0, STRING("ビビアン"))
    CALL(evt_mario_set_pose, STRING("M_N_5B"))
    WAIT_MS(500)
    // She insists on coming along and kisses Mario on the cheek.
    CALL(evt_msg_print, 0, STRING("stg4_usu_120"), 0, STRING("ビビアン"))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_NO1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING("M_N_B"))
    WAIT_MS(700)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_msg_print, 0, STRING("stg4_usu_121"), 0, STRING("ビビアン"))
    CALL(evt_npc_get_position, STRING("ビビアン"), LW(0), LW(1), LW(2))
    ADD(LW(0), -15)
    CALL(evt_npc_flag_onoff, 1, STRING("ビビアン"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_move_position, STRING("ビビアン"), LW(0), LW(2), 0, 60, 0)
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        WAIT_MS(300)
        ADD(LW(0), -10)
        ADD(LW(1), 50)
        ADD(LW(2), 60)
        CALL(evt_eff64, 0, STRING("recover_heart_n64"), 2, LW(0), LW(1), LW(2), 60, 0, 0, 0, 0, 0, 0, 0)
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG4_VIVIAN_KISS2"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_npc_get_position, STRING("ビビアン"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG4_VIVIAN_KISS1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_npc_set_anim, STRING("ビビアン"), STRING("PTR_A4_2"))
    CALL(evt_mario_set_pose, STRING("M_N_5B"))
    WAIT_MS(400)
    CALL(evt_npc_set_anim, STRING("ビビアン"), STRING("PTR_A4_1"))
    CALL(evt_npc_flag_onoff, 0, STRING("ビビアン"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_status_onoff, 0, STRING("ビビアン"), NPC_STATUS_FACE_MOVE_DIR)
    SUBTRACT(LW(0), -15)
    CALL(evt_npc_move_position, STRING("ビビアン"), LW(0), LW(2), 0, 20, 0)
    WAIT_MS(300)
    // Vivian joins the party and shows her field ability (all_party_lecture).
    CALL(evt_msg_print, 0, STRING("stg4_usu_121_01"), 0, STRING("ビビアン"))
    CALL(evt_cam3d_evt_set_npc_rel, STRING("ビビアン"), 0, 50, 290, 0, 45, 14, 800, EASING_SINE_IN_OUT)
    WAIT_MS(800)
    SET_VALUE(LW(8), STRING("ビビアン"))
    SET_VALUE(LW(9), PARTY_MEMBER_VIVIAN)
#if VERSION <= VERSION_DEMO_US
    CALL_CHILD(all_party_lecture)
CALL(evt_snd_bgmon, PSND_FADE_OUT, STRING("BGM_STG4_USU1"))
#else
    CALL_CHILD(PTR(all_party_lecture))
#endif
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG4_USU1"))
    CALL(evt_msg_print_party, STRING("stg4_usu_123"))
    CALL(evt_mario_set_pose, STRING("M_N_2"))
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("stg4_usu_124"))
    CALL(evt_cam3d_evt_off, 1000, EASING_SINE_IN_OUT)
#if VERSION > VERSION_DEMO_US
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG4_USU1"))
#endif
    WAIT_MS(1000)
    SET_VALUE(GSW_STORY_PROGRESS, STG4_13_VIVIAN_JOINS)
    SET_VALUE(GSWF_JIN_TRADING_PLACES_ACTIVE, 0)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_party_run, 0)
    RETURN
EVT_END()

/**
 * Raises the 3D camera and its target by 85 units over 500 ms (curve 11), then waits 600 ms;
 * used by usu00_crowAB_talk_event and usu00_crowCDE_talk_event before a crow speaks from its high perch.
 */
static EVT_BEGIN(cam_up)
    BEGIN_INLINE_EVENT
        CALL(evt_cam_get_pos, CAMERA_3D, LW(0), LW(1), LW(2))
        CALL(evt_cam_get_at, CAMERA_3D, LW(3), LW(4), LW(5))
        ADD(LW(1), 85)
        ADD(LW(4), 85)
        CALL(evt_cam3d_evt_set, LW(0), LW(1), LW(2), LW(3), LW(4), LW(5), 500, EASING_SINE_IN_OUT)
    END_INLINE_EVENT
    WAIT_MS(600)
    RETURN
EVT_END()

/**
 * Returns the 3D camera to normal (evt_cam3d_evt_off over 500 ms) and waits 500 ms;
 * ends cam_up in the crow talk events.
 */
static EVT_BEGIN(cam_down)
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    RETURN
EVT_END()

/**
 * Background loop: while Vivian is the active partner (id 6) but not in a normal status,
 * it clears the crow-talk gates GW(0) and GW(1).
 */
EVT_BEGIN(usu00_check_vivian)
    LABEL(0)
    CALL(evt_mario_get_party, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 6)
        WAIT_FRAMES(1)
        GOTO(0)
    END_IF
    CALL(evt_party_get_status, 6, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 2)
        SET_VALUE(GW(0), 0)
        SET_VALUE(GW(1), 0)
    END_IF
    WAIT_FRAMES(1)
    GOTO(0)
    RETURN
EVT_END()

/**
 * Background watcher for crows A and B: when Mario is near (area depends on his mode)
 * and Vivian is not the partner, the crow talks ("kpa_stg5_009_7_1" in the Bowser interlude,
 * "stg4_jin_152" otherwise); GW(0) gates repeats.
 */
#if VERSION <= VERSION_DEMO_US
/**
 * Background watcher for crows A and B: when Mario is near (area depends on his mode)
 * and Vivian is not the partner, the crow talks ("kpa_stg5_009_7_1" in the Bowser interlude,
 * "stg4_jin_152" otherwise); GW(0) gates repeats.
 */
EVT_BEGIN(usu00_crowAB_talk_event)
    // Wait until this crow pair is allowed to talk again.
    SET_VALUE(LW(15), 0)
    LABEL(0)
    IF_INT_NOT_EQUAL(GW(0), 0)
        WAIT_FRAMES(1)
        GOTO(0)
    END_IF
    // Show the "..." balloon while Mario stands below crow B (a wider area in Bowser's interlude).
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_npc_get_position, STRING("カラスＢ"), LW(3), LW(4), LW(5))
    CALL(evt_mario_get_mode, LW(7))
    IF_INT_EQUAL(LW(7), 2)
        SET_VALUE(LW(8), 60)
    ELSE
        SET_VALUE(LW(8), 20)
    END_IF
    CALL(evt_sub_area_check, 1, LW(3), LW(5), LW(8), LW(0), LW(2), LW(6))
IF_INT_EQUAL(LW(6), 1)
        IF_INT_EQUAL(LW(15), 0)
            CALL(evt_mario_balloon_tenten)
            SET_VALUE(LW(15), 1)
        END_IF
    ELSE
        IF_INT_EQUAL(LW(15), 1)
            CALL(evt_mario_balloon_off)
            SET_VALUE(LW(15), 0)
        END_IF
    END_IF
    // Pressing A there gets only a caw ("Cawf cawf cawf!", or Bowser's version in the interlude).
    IF_INT_EQUAL(LW(15), 1)
        CALL(evt_key_get_buttontrg, 0, LW(0))
        CALL(evt_mario_party_use_check, LW(1))
        IF_INT_EQUAL(LW(1), 0)
            IF_BITS_SET(LW(0), 256)
                CALL(evt_mario_balloon_off)
                SET_VALUE(LW(15), 0)
                CALL(evt_mario_key_onoff, 0)
                CALL(evt_party_cont_onoff, 0, 0)
CALL_CHILD(cam_up)
                CALL(evt_npc_get_position, STRING("カラスＢ"), LW(0), LW(1), LW(2))
                CALL(evt_snd_sfxon_3d, STRING("SFX_STG4_CROW_HOWL1"), LW(0), LW(1), LW(2), 0)
                IF_INT_EQUAL(LW(7), 2)
                    CALL(evt_msg_print, 0, STRING("kpa_stg5_009_7_1"), 0, STRING("カラスＢ"))
                ELSE
                    CALL(evt_msg_print, 0, STRING("stg4_jin_152"), 0, STRING("カラスＢ"))
                END_IF
                CALL_CHILD(cam_down)
                CALL(evt_mario_key_onoff, 1)
                CALL(evt_party_cont_onoff, 0, 1)
END_IF
        END_IF
    END_IF
    // When Vivian is the partner in party status 2 (Guess: hiding Mario in her shadow), the crows talk for real:
    // A and B chat about the internet and business ideas, by story progress; once per visit (GW(0)).
    CALL(evt_mario_get_party, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 6)
        WAIT_FRAMES(1)
        GOTO(0)
    END_IF
    CALL(evt_party_get_status, 6, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 2)
        WAIT_FRAMES(1)
        GOTO(0)
    END_IF
    IF_INT_EQUAL(LW(15), 1)
        CALL(evt_mario_balloon_off)
        SET_VALUE(LW(15), 0)
        IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG4_13_VIVIAN_JOINS)
            CALL(evt_mario_key_onoff, 0)
            CALL(evt_party_cont_onoff, 0, 0)
CALL_CHILD(cam_up)
            WAIT_MS(1000)
            CALL(evt_npc_get_position, STRING("カラスＢ"), LW(0), LW(1), LW(2))
            CALL(evt_snd_sfxon_3d, STRING("SFX_STG4_CROW_HOWL1"), LW(0), LW(1), LW(2), 0)
            SWITCH(GSW_STORY_PROGRESS)
                CASE_LESS(STG4_14_NAME_GUESSING)
                CALL(evt_msg_print, 0, STRING("stg4_usu_125"), 0, STRING("カラスＡ"))
                CALL(evt_msg_print, 0, STRING("stg4_usu_126"), 0, STRING("カラスＢ"))
                CALL(evt_msg_print, 0, STRING("stg4_usu_127"), 0, STRING("カラスＡ"))
                CALL(evt_msg_print, 0, STRING("stg4_usu_128"), 0, STRING("カラスＢ"))
                CASE_LESS(STG4_31_FRANKLYS_HOUSE_STG_4)
                CALL(evt_msg_print, 0, STRING("stg4_usu_129"), 0, STRING("カラスＡ"))
                CALL(evt_msg_print, 0, STRING("stg4_usu_130"), 0, STRING("カラスＢ"))
                CASE_LESS_OR_EQUAL(STG8_END_AFTER_THE_BATTLE)
                CALL(evt_msg_print, 0, STRING("stg4_usu_130_1"), 0, STRING("カラスＡ"))
                CALL(evt_msg_print, 0, STRING("stg4_usu_130_2"), 0, STRING("カラスＢ"))
                CASE_DEFAULT
                CALL(evt_msg_print, 0, STRING("stg4_usu_130_2_1"), 0, STRING("カラスＡ"))
                CALL(evt_msg_print, 0, STRING("stg4_usu_130_2_2"), 0, STRING("カラスＢ"))
            END_SWITCH
            CALL_CHILD(cam_down)
            CALL(evt_mario_key_onoff, 1)
            CALL(evt_party_cont_onoff, 0, 1)
SET_VALUE(GW(0), 1)
        END_IF
    END_IF
    WAIT_FRAMES(1)
    GOTO(0)
    RETURN
EVT_END()
#else
/**
 * Background watcher for crows A and B: when Mario is near (area depends on his mode)
 * and Vivian is not the partner, the crow talks ("kpa_stg5_009_7_1" in the Bowser interlude,
 * "stg4_jin_152" otherwise); GW(0) gates repeats.
 */
EVT_BEGIN(usu00_crowAB_talk_event)
    // Wait until this crow pair is allowed to talk again.
    SET_VALUE(LW(15), 0)
    LABEL(0)
    IF_INT_NOT_EQUAL(GW(0), 0)
        WAIT_FRAMES(1)
        GOTO(0)
    END_IF
    // Show the "..." balloon while Mario stands below crow B (a wider area in Bowser's interlude).
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_npc_get_position, STRING("カラスＢ"), LW(3), LW(4), LW(5))
    CALL(evt_mario_get_mode, LW(7))
    IF_INT_EQUAL(LW(7), 2)
        SET_VALUE(LW(8), 60)
        ELSE
        SET_VALUE(LW(8), 20)
    END_IF
    CALL(evt_sub_area_check, 1, LW(3), LW(5), LW(8), LW(0), LW(2), LW(6))
#if VERSION >= VERSION_US
    CALL(evt_mario_get_party, LW(1))
    IF_INT_NOT_EQUAL(LW(1), 6)
        CALL(evt_mario_party_use_check, LW(1))
        IF_INT_NOT_EQUAL(LW(1), 0)
            SET_VALUE(LW(6), 0)
        END_IF
    END_IF
    CALL(evt_mario_get_motion, LW(1))
    IF_INT_NOT_EQUAL(LW(1), 28)
        IF_INT_NOT_EQUAL(LW(1), 0)
            IF_INT_NOT_EQUAL(LW(1), 1)
                IF_INT_NOT_EQUAL(LW(1), 2)
                    SET_VALUE(LW(6), 0)
                END_IF
            END_IF
        END_IF
    END_IF
#endif
    IF_INT_EQUAL(LW(6), 1)
        IF_INT_EQUAL(LW(15), 0)
            CALL(evt_mario_balloon_tenten)
            SET_VALUE(LW(15), 1)
        END_IF
        ELSE
        IF_INT_EQUAL(LW(15), 1)
            CALL(evt_mario_balloon_off)
            SET_VALUE(LW(15), 0)
        END_IF
    END_IF
    // Pressing A there gets only a caw ("Cawf cawf cawf!", or Bowser's version in the interlude).
    IF_INT_EQUAL(LW(15), 1)
        CALL(evt_key_get_buttontrg, 0, LW(0))
        CALL(evt_mario_party_use_check, LW(1))
        IF_INT_EQUAL(LW(1), 0)
            IF_BITS_SET(LW(0), 256)
                CALL(evt_mario_balloon_off)
                SET_VALUE(LW(15), 0)
                CALL(evt_mario_key_onoff, 0)
                CALL(evt_party_cont_onoff, 0, 0)
                CALL(mainwindow_opendisable)
                CALL_CHILD(PTR(cam_up))
                CALL(evt_npc_get_position, STRING("カラスＢ"), LW(0), LW(1), LW(2))
                CALL(evt_snd_sfxon_3d, STRING("SFX_STG4_CROW_HOWL1"), LW(0), LW(1), LW(2), 0)
                IF_INT_EQUAL(LW(7), 2)
                    CALL(evt_msg_print, 0, STRING("kpa_stg5_009_7_1"), 0, STRING("カラスＢ"))
                    ELSE
                    CALL(evt_msg_print, 0, STRING("stg4_jin_152"), 0, STRING("カラスＢ"))
                END_IF
                CALL_CHILD(PTR(cam_down))
                CALL(evt_mario_key_onoff, 1)
                CALL(evt_party_cont_onoff, 0, 1)
                CALL(mainwindow_openenable)
            END_IF
        END_IF
    END_IF
    // When Vivian is the partner in party status 2 (Guess: hiding Mario in her shadow), the crows talk for real:
    // A and B chat about the internet and business ideas, by story progress; once per visit (GW(0)).
    CALL(evt_mario_get_party, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 6)
        WAIT_FRAMES(1)
        GOTO(0)
    END_IF
    CALL(evt_party_get_status, 6, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 2)
        WAIT_FRAMES(1)
        GOTO(0)
    END_IF
    IF_INT_EQUAL(LW(15), 1)
        CALL(evt_mario_balloon_off)
        SET_VALUE(LW(15), 0)
        IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG4_13_VIVIAN_JOINS)
            CALL(evt_mario_key_onoff, 0)
            CALL(evt_party_cont_onoff, 0, 0)
            CALL(mainwindow_opendisable)
            CALL_CHILD(PTR(cam_up))
            WAIT_MS(1000)
            CALL(evt_npc_get_position, STRING("カラスＢ"), LW(0), LW(1), LW(2))
            CALL(evt_snd_sfxon_3d, STRING("SFX_STG4_CROW_HOWL1"), LW(0), LW(1), LW(2), 0)
            SWITCH(GSW_STORY_PROGRESS)
                CASE_LESS(STG4_14_NAME_GUESSING)
                CALL(evt_msg_print, 0, STRING("stg4_usu_125"), 0, STRING("カラスＡ"))
                CALL(evt_msg_print, 0, STRING("stg4_usu_126"), 0, STRING("カラスＢ"))
                CALL(evt_msg_print, 0, STRING("stg4_usu_127"), 0, STRING("カラスＡ"))
                CALL(evt_msg_print, 0, STRING("stg4_usu_128"), 0, STRING("カラスＢ"))
                CASE_LESS(STG4_31_FRANKLYS_HOUSE_STG_4)
                CALL(evt_msg_print, 0, STRING("stg4_usu_129"), 0, STRING("カラスＡ"))
                CALL(evt_msg_print, 0, STRING("stg4_usu_130"), 0, STRING("カラスＢ"))
                CASE_LESS_OR_EQUAL(STG8_END_AFTER_THE_BATTLE)
                CALL(evt_msg_print, 0, STRING("stg4_usu_130_1"), 0, STRING("カラスＡ"))
                CALL(evt_msg_print, 0, STRING("stg4_usu_130_2"), 0, STRING("カラスＢ"))
                CASE_DEFAULT
                CALL(evt_msg_print, 0, STRING("stg4_usu_130_2_1"), 0, STRING("カラスＡ"))
                CALL(evt_msg_print, 0, STRING("stg4_usu_130_2_2"), 0, STRING("カラスＢ"))
            END_SWITCH
            CALL_CHILD(PTR(cam_down))
            CALL(evt_mario_key_onoff, 1)
            CALL(evt_party_cont_onoff, 0, 1)
            CALL(mainwindow_openenable)
            SET_VALUE(GW(0), 1)
        END_IF
    END_IF
    WAIT_FRAMES(1)
    GOTO(0)
    RETURN
EVT_END()
#endif

/** Background watcher for crows C, D and E; same as the A/B watcher but using GW(1) as its gate. */
#if VERSION <= VERSION_DEMO_US
/** Background watcher for crows C, D and E; same as the A/B watcher but using GW(1) as its gate. */
EVT_BEGIN(usu00_crowCDE_talk_event)
    // Wait until this crow trio is allowed to talk again.
    SET_VALUE(LW(15), 0)
    LABEL(0)
    IF_INT_NOT_EQUAL(GW(1), 0)
        WAIT_FRAMES(1)
        GOTO(0)
    END_IF
    // Show the "..." balloon while Mario stands below crow D, but not past the north edge.
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_npc_get_position, STRING("カラスＤ"), LW(3), LW(4), LW(5))
    CALL(evt_sub_area_check, 1, LW(3), LW(5), 60, LW(0), LW(2), LW(6))
IF_INT_LESS_OR_EQUAL(LW(2), -250)
        SET_VALUE(LW(6), 0)
    END_IF
    IF_INT_EQUAL(LW(6), 1)
        IF_INT_EQUAL(LW(15), 0)
            CALL(evt_mario_balloon_tenten)
            SET_VALUE(LW(15), 1)
        END_IF
    ELSE
        IF_INT_EQUAL(LW(15), 1)
            CALL(evt_mario_balloon_off)
            SET_VALUE(LW(15), 0)
        END_IF
    END_IF
    // Pressing A there gets only a caw ("Cawf cawf cawf!", or Bowser's version in the interlude).
    IF_INT_EQUAL(LW(15), 1)
        CALL(evt_key_get_buttontrg, 0, LW(0))
        CALL(evt_mario_party_use_check, LW(1))
        IF_INT_EQUAL(LW(1), 0)
            IF_BITS_SET(LW(0), 256)
                CALL(evt_mario_balloon_off)
                SET_VALUE(LW(15), 0)
                CALL(evt_mario_key_onoff, 0)
                CALL(evt_party_cont_onoff, 0, 0)
CALL_CHILD(cam_up)
                CALL(evt_npc_get_position, STRING("カラスＤ"), LW(0), LW(1), LW(2))
                CALL(evt_snd_sfxon_3d, STRING("SFX_STG4_CROW_HOWL1"), LW(0), LW(1), LW(2), 0)
                CALL(evt_mario_get_mode, LW(7))
                IF_INT_EQUAL(LW(7), 2)
                    CALL(evt_msg_print, 0, STRING("kpa_stg5_009_7_2"), 0, STRING("カラスＤ"))
                ELSE
                    CALL(evt_msg_print, 0, STRING("stg4_jin_152"), 0, STRING("カラスＤ"))
                END_IF
                CALL_CHILD(cam_down)
                CALL(evt_mario_key_onoff, 1)
                CALL(evt_party_cont_onoff, 0, 1)
END_IF
        END_IF
    END_IF
    // When Vivian is the partner in party status 2 (Guess: hiding Mario in her shadow), the crows talk for real:
    // C, D and E chat about energy and solar power, by story progress; once per visit (GW(1)).
    CALL(evt_mario_get_party, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 6)
        WAIT_FRAMES(1)
        GOTO(0)
    END_IF
    CALL(evt_party_get_status, 6, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 2)
        WAIT_FRAMES(1)
        GOTO(0)
    END_IF
    IF_INT_EQUAL(LW(15), 1)
        CALL(evt_mario_balloon_off)
        SET_VALUE(LW(15), 0)
        IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG4_13_VIVIAN_JOINS)
            CALL(evt_mario_key_onoff, 0)
            CALL(evt_party_cont_onoff, 0, 0)
CALL_CHILD(cam_up)
            WAIT_MS(1000)
            CALL(evt_npc_get_position, STRING("カラスＤ"), LW(0), LW(1), LW(2))
            CALL(evt_snd_sfxon_3d, STRING("SFX_STG4_CROW_HOWL1"), LW(0), LW(1), LW(2), 0)
            SWITCH(GSW_STORY_PROGRESS)
                CASE_LESS(STG4_14_NAME_GUESSING)
                CALL(evt_msg_print, 0, STRING("stg4_usu_131"), 0, STRING("カラスＣ"))
                CALL(evt_msg_print, 0, STRING("stg4_usu_132"), 0, STRING("カラスＤ"))
                CALL(evt_msg_print, 0, STRING("stg4_usu_133"), 0, STRING("カラスＥ"))
                CASE_LESS(STG4_31_FRANKLYS_HOUSE_STG_4)
                CALL(evt_msg_print, 0, STRING("stg4_usu_134"), 0, STRING("カラスＣ"))
                CALL(evt_msg_print, 0, STRING("stg4_usu_135"), 0, STRING("カラスＤ"))
                CALL(evt_msg_print, 0, STRING("stg4_usu_136"), 0, STRING("カラスＥ"))
                CASE_LESS_OR_EQUAL(STG8_END_AFTER_THE_BATTLE)
                CALL(evt_msg_print, 0, STRING("stg4_usu_136_1"), 0, STRING("カラスＣ"))
                CALL(evt_msg_print, 0, STRING("stg4_usu_136_2"), 0, STRING("カラスＤ"))
                CALL(evt_msg_print, 0, STRING("stg4_usu_136_3"), 0, STRING("カラスＥ"))
                CASE_DEFAULT
                CALL(evt_msg_print, 0, STRING("stg4_usu_136_3_1"), 0, STRING("カラスＣ"))
                CALL(evt_msg_print, 0, STRING("stg4_usu_136_3_2"), 0, STRING("カラスＤ"))
                CALL(evt_msg_print, 0, STRING("stg4_usu_136_3_3"), 0, STRING("カラスＥ"))
            END_SWITCH
            CALL_CHILD(cam_down)
            CALL(evt_mario_key_onoff, 1)
            CALL(evt_party_cont_onoff, 0, 1)
SET_VALUE(GW(1), 1)
        END_IF
    END_IF
    WAIT_FRAMES(1)
    GOTO(0)
    RETURN
EVT_END()
#else
/** Background watcher for crows C, D and E; same as the A/B watcher but using GW(1) as its gate. */
EVT_BEGIN(usu00_crowCDE_talk_event)
    // Wait until this crow trio is allowed to talk again.
    SET_VALUE(LW(15), 0)
    LABEL(0)
    IF_INT_NOT_EQUAL(GW(1), 0)
        WAIT_FRAMES(1)
        GOTO(0)
    END_IF
    // Show the "..." balloon while Mario stands below crow D, but not past the north edge.
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_npc_get_position, STRING("カラスＤ"), LW(3), LW(4), LW(5))
    CALL(evt_sub_area_check, 1, LW(3), LW(5), 60, LW(0), LW(2), LW(6))
#if VERSION >= VERSION_US
    CALL(evt_mario_get_party, LW(1))
    IF_INT_NOT_EQUAL(LW(1), 6)
        CALL(evt_mario_party_use_check, LW(1))
        IF_INT_NOT_EQUAL(LW(1), 0)
            SET_VALUE(LW(6), 0)
        END_IF
    END_IF
    CALL(evt_mario_get_motion, LW(1))
    IF_INT_NOT_EQUAL(LW(1), 28)
        IF_INT_NOT_EQUAL(LW(1), 0)
            IF_INT_NOT_EQUAL(LW(1), 1)
                IF_INT_NOT_EQUAL(LW(1), 2)
                    SET_VALUE(LW(6), 0)
                END_IF
            END_IF
        END_IF
    END_IF
#endif
    IF_INT_LESS_OR_EQUAL(LW(2), -250)
        SET_VALUE(LW(6), 0)
    END_IF
    IF_INT_EQUAL(LW(6), 1)
        IF_INT_EQUAL(LW(15), 0)
            CALL(evt_mario_balloon_tenten)
            SET_VALUE(LW(15), 1)
        END_IF
        ELSE
        IF_INT_EQUAL(LW(15), 1)
            CALL(evt_mario_balloon_off)
            SET_VALUE(LW(15), 0)
        END_IF
    END_IF
    // Pressing A there gets only a caw ("Cawf cawf cawf!", or Bowser's version in the interlude).
    IF_INT_EQUAL(LW(15), 1)
        CALL(evt_key_get_buttontrg, 0, LW(0))
        CALL(evt_mario_party_use_check, LW(1))
        IF_INT_EQUAL(LW(1), 0)
            IF_BITS_SET(LW(0), 256)
                CALL(evt_mario_balloon_off)
                SET_VALUE(LW(15), 0)
                CALL(evt_mario_key_onoff, 0)
                CALL(evt_party_cont_onoff, 0, 0)
                CALL(mainwindow_opendisable)
                CALL_CHILD(PTR(cam_up))
                CALL(evt_npc_get_position, STRING("カラスＤ"), LW(0), LW(1), LW(2))
                CALL(evt_snd_sfxon_3d, STRING("SFX_STG4_CROW_HOWL1"), LW(0), LW(1), LW(2), 0)
                CALL(evt_mario_get_mode, LW(7))
                IF_INT_EQUAL(LW(7), 2)
                    CALL(evt_msg_print, 0, STRING("kpa_stg5_009_7_2"), 0, STRING("カラスＤ"))
                    ELSE
                    CALL(evt_msg_print, 0, STRING("stg4_jin_152"), 0, STRING("カラスＤ"))
                END_IF
                CALL_CHILD(PTR(cam_down))
                CALL(evt_mario_key_onoff, 1)
                CALL(evt_party_cont_onoff, 0, 1)
                CALL(mainwindow_openenable)
            END_IF
        END_IF
    END_IF
    // When Vivian is the partner in party status 2 (Guess: hiding Mario in her shadow), the crows talk for real:
    // C, D and E chat about energy and solar power, by story progress; once per visit (GW(1)).
    CALL(evt_mario_get_party, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 6)
        WAIT_FRAMES(1)
        GOTO(0)
    END_IF
    CALL(evt_party_get_status, 6, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 2)
        WAIT_FRAMES(1)
        GOTO(0)
    END_IF
    IF_INT_EQUAL(LW(15), 1)
        CALL(evt_mario_balloon_off)
        SET_VALUE(LW(15), 0)
        IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG4_13_VIVIAN_JOINS)
            CALL(evt_mario_key_onoff, 0)
            CALL(evt_party_cont_onoff, 0, 0)
            CALL(mainwindow_opendisable)
            CALL_CHILD(PTR(cam_up))
            WAIT_MS(1000)
            CALL(evt_npc_get_position, STRING("カラスＤ"), LW(0), LW(1), LW(2))
            CALL(evt_snd_sfxon_3d, STRING("SFX_STG4_CROW_HOWL1"), LW(0), LW(1), LW(2), 0)
            SWITCH(GSW_STORY_PROGRESS)
                CASE_LESS(STG4_14_NAME_GUESSING)
                CALL(evt_msg_print, 0, STRING("stg4_usu_131"), 0, STRING("カラスＣ"))
                CALL(evt_msg_print, 0, STRING("stg4_usu_132"), 0, STRING("カラスＤ"))
                CALL(evt_msg_print, 0, STRING("stg4_usu_133"), 0, STRING("カラスＥ"))
                CASE_LESS(STG4_31_FRANKLYS_HOUSE_STG_4)
                CALL(evt_msg_print, 0, STRING("stg4_usu_134"), 0, STRING("カラスＣ"))
                CALL(evt_msg_print, 0, STRING("stg4_usu_135"), 0, STRING("カラスＤ"))
                CALL(evt_msg_print, 0, STRING("stg4_usu_136"), 0, STRING("カラスＥ"))
                CASE_LESS_OR_EQUAL(STG8_END_AFTER_THE_BATTLE)
                CALL(evt_msg_print, 0, STRING("stg4_usu_136_1"), 0, STRING("カラスＣ"))
                CALL(evt_msg_print, 0, STRING("stg4_usu_136_2"), 0, STRING("カラスＤ"))
                CALL(evt_msg_print, 0, STRING("stg4_usu_136_3"), 0, STRING("カラスＥ"))
                CASE_DEFAULT
                CALL(evt_msg_print, 0, STRING("stg4_usu_136_3_1"), 0, STRING("カラスＣ"))
                CALL(evt_msg_print, 0, STRING("stg4_usu_136_3_2"), 0, STRING("カラスＤ"))
                CALL(evt_msg_print, 0, STRING("stg4_usu_136_3_3"), 0, STRING("カラスＥ"))
            END_SWITCH
            CALL_CHILD(PTR(cam_down))
            CALL(evt_mario_key_onoff, 1)
            CALL(evt_party_cont_onoff, 0, 1)
            CALL(mainwindow_openenable)
            SET_VALUE(GW(1), 1)
        END_IF
    END_IF
    WAIT_FRAMES(1)
    GOTO(0)
    RETURN
EVT_END()
#endif

/**
 * Trigger: when Mario walks within 100 units of the pig Mayor,
 * the party says "stg4_usu_21" and the story moves to STG4_01_THE_MAYORS_A_PIG.
 */
EVT_BEGIN(chief_pig_event)
    LOOP(0)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_npc_get_position, STRING("ブタ村長"), LW(3), LW(4), LW(5))
        CALL(evt_sub_get_dist, LW(0), LW(2), LW(3), LW(5), LW(6))
        IF_INT_LESS_OR_EQUAL(LW(6), 100)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_party_yoshi_ride, 0, 0)
    CALL(evt_mario_wait_rideon)
    CALL(evt_party_stop, 0)
    CALL(evt_mario_set_dir_npc, STRING("ブタ村長"))
    CALL(evt_party_set_dir_npc, 0, STRING("ブタ村長"))
    WAIT_MS(300)
    CALL(evt_msg_print_party, STRING("stg4_usu_21"))
    SET_VALUE(GSW_STORY_PROGRESS, STG4_01_THE_MAYORS_A_PIG)
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/**
 * Twilight Town inn (house 3), run by the innkeeper: breakfast item 176 and price 10 coins, the bed and camera
 * spots for sleeping, and the innkeeper's lines usugurayado_00 to 13.
 */
static KinopioData kino_dt = {
    176, // breakfastItem
    {250.0f, 16.0f, -300.0f}, // breakfastPos
    10, // ryoukin
#if VERSION <= VERSION_DEMO_US
    {427.0f, 16.0f, -200.0f},
#else
    {431.0f, 16.0f, -195.0f}, // bed
#endif
    {397.0f, 0.0f, -168.0f}, // bedside
    {135.0f, 122.0f, -171.0f, 366.0f, 46.0f, -228.0f}, // cam
    -50.0f, // bedDeg
    {345.0f, 0.0f, -208.0f}, // npcTalk
    /*"宿店員"*/"\x8F\x68\x93\x58\x88\xF5", // npcName
    {"usugurayado_00", "usugurayado_01", "usugurayado_02", "usugurayado_03", "usugurayado_04", "usugurayado_05", "usugurayado_06", "usugurayado_07", "usugurayado_08", "usugurayado_09", "usugurayado_10", "usugurayado_11", "usugurayado_12", "usugurayado_13"}, // msg
    NULL, NULL, // toBedEvt..fromBedEvt
};

/** NPCs hidden and shown with the inn's interior (house 3): the innkeeper. */
static const char* yado_npc[] = {
    /*"宿店員"*/"\x8F\x68\x93\x58\x88\xF5",
    NULL,
};

/** Map objects hidden and shown with the inn's interior: its save block. */
const char* yado_mobj[] = {
    "pblk00",
    NULL,
};

/**
 * NPCs that belong to the Mayor's house (house 2) interior: the Mayor in human and pig form, the fake Mario
 * and the partners who can stand inside during his scenes.
 */
const char* village_chief_npc[] = {
    /*"村長"*/"\x91\xBA\x92\xB7",
    /*"ブタ村長"*/"\x83\x75\x83\x5E\x91\xBA\x92\xB7",
    /*"にせマリオ"*/"\x82\xC9\x82\xB9\x83\x7D\x83\x8A\x83\x49",
    /*"クリスチーヌ"*/"\x83\x4E\x83\x8A\x83\x58\x83\x60\x81\x5B\x83\x6B",
    /*"ノコタロウ"*/"\x83\x6D\x83\x52\x83\x5E\x83\x8D\x83\x45",
    /*"クラウダ"*/"\x83\x4E\x83\x89\x83\x45\x83\x5F",
    /*"チビヨッシー"*/"\x83\x60\x83\x72\x83\x88\x83\x62\x83\x56\x81\x5B",
    NULL,
};

/** Door script for house 1's door "S_ie1_door" (evtIn of door_1): turns it about y by LW(0) scaled to door units. */
static EVT_BEGIN(evt_door_1_1)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.5546875f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie1_door"), 0, LW(0), 0)
    RETURN
EVT_END()

/** Empty door script (evtOut of door_1); just returns. */
static EVT_BEGIN(evt_door_1_2)
    RETURN
EVT_END()

/**
 * Wall script of house 1 (DoorParam evtWall), which evt_door_open spawns every frame with the opening angle in
 * LW(0) (0 to 180 as the house front opens, back to 0 as it closes):
 * turns the front-wall pieces ie1_kai1 and ie1_kai2 about x by -LW(0) / 2 and LW(0) / 2,
 * the second 10 frames later.
 */
static EVT_BEGIN(evt_wall_1)
    SET_FLOAT_VALUE(LW(1), LW(0))
    MULTIPLY_FLOAT(LW(1), FLOAT(-0.5f))
    CALL(evt_mapobj_rotate, 1, STRING("ie1_kai1"), LW(1), 0, 0)
    WAIT_FRAMES(10)
    SET_FLOAT_VALUE(LW(2), LW(0))
    MULTIPLY_FLOAT(LW(2), FLOAT(0.5f))
    CALL(evt_mapobj_rotate, 1, STRING("ie1_kai2"), LW(2), 0, 0)
    RETURN
EVT_END()

/** NPCs that belong to house 1's interior: villager B. */
const char* door_1_npc[] = {
    /*"村人Ｂ"*/"\x91\xBA\x90\x6C\x82\x61",
    NULL,
};

/** House 1 open callback: dips BGM and ambience to 50 and low-pass filters the environment sound. */
static EVT_BEGIN(door_1_open)
    CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_envoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_env_lpf, 0, 800)
    RETURN
EVT_END()

/** House 1 close callback: restores BGM and ambience and removes the low-pass filter. */
static EVT_BEGIN(door_1_close)
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_envon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_env_lpf, 0, 0)
    RETURN
EVT_END()

/** House 1, a villager's house with a fold-down front wall (door S_ie1_door). */
static DoorParam door_1 = {
    0, // flags
    3, // type
    2, // se
    "S_ie1_door", // mapobj
    "A_ie1door_u", // hitIn
    "A_ie1door", // hitOut
    "S_ie1", // outside
    NULL, // outsideHit
    "S_ie1_naka", // inside
    NULL, // insideHit
    evt_door_1_1, // evtIn
    NULL, // evtOut
    evt_wall_1, // evtWall
    (const char**)door_1_npc, // npcs
    NULL, NULL, // mobjs..items
    door_1_open, // evtOpen
    door_1_close, // evtClose
};

/**
 * Door script for house 2's door "S__ie2_door" (evtIn of door_2):
 * turns it about y by the angle in LW(0), scaled to door units.
 */
EVT_BEGIN(evt_door_2_1)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.5546875f))
    CALL(evt_mapobj_rotate, 1, STRING("S__ie2_door"), 0, LW(0), 0)
    RETURN
EVT_END()

/**
 * Door script for house 2's door (evtOut of door_2):
 * turns "S__ie2_door" about x by LW(0) scaled by 0.5 and adds 1 to GW(2).
 */
EVT_BEGIN(evt_door_2_2)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.5f))
    CALL(evt_mapobj_rotate, 1, STRING("S__ie2_door"), LW(0), 0, 0)
    ADD(GW(2), 1)
    RETURN
EVT_END()

/**
 * Wall script of house 2 (DoorParam evtWall), spawned every frame with the opening angle in LW(0) (0 to 180 and
 * back): turns the front-wall pieces ie2_kai1, ie2_kai2 and ie2_kai4 about x and ie2_kai3 about z,
 * by half the angle with alternating signs, one after the other (5, 5 and 10 frames apart).
 * When LW(1) equals LW(0) it goes from kai1 to kai4, otherwise in the reverse order.
 */
static EVT_BEGIN(evt_wall_2)
    IF_INT_EQUAL(LW(1), LW(0))
        SET_FLOAT_VALUE(LW(1), LW(0))
        MULTIPLY_FLOAT(LW(1), FLOAT(-0.5f))
        CALL(evt_mapobj_rotate, 1, STRING("ie2_kai1"), LW(1), 0, 0)
        WAIT_FRAMES(5)
        SET_FLOAT_VALUE(LW(2), LW(0))
        MULTIPLY_FLOAT(LW(2), FLOAT(0.5f))
        CALL(evt_mapobj_rotate, 1, STRING("ie2_kai2"), LW(2), 0, 0)
        WAIT_FRAMES(5)
        SET_FLOAT_VALUE(LW(3), LW(0))
        MULTIPLY_FLOAT(LW(3), FLOAT(-0.5f))
        CALL(evt_mapobj_rotate, 1, STRING("ie2_kai3"), 0, 0, LW(3))
        WAIT_FRAMES(10)
        SET_FLOAT_VALUE(LW(4), LW(0))
        MULTIPLY_FLOAT(LW(4), FLOAT(0.5f))
        CALL(evt_mapobj_rotate, 1, STRING("ie2_kai4"), LW(4), 0, 0)
        ELSE
        SET_FLOAT_VALUE(LW(4), LW(0))
        MULTIPLY_FLOAT(LW(4), FLOAT(0.5f))
        CALL(evt_mapobj_rotate, 1, STRING("ie2_kai4"), LW(4), 0, 0)
        WAIT_FRAMES(10)
        SET_FLOAT_VALUE(LW(3), LW(0))
        MULTIPLY_FLOAT(LW(3), FLOAT(-0.5f))
        CALL(evt_mapobj_rotate, 1, STRING("ie2_kai3"), 0, 0, LW(3))
        WAIT_FRAMES(5)
        SET_FLOAT_VALUE(LW(2), LW(0))
        MULTIPLY_FLOAT(LW(2), FLOAT(0.5f))
        CALL(evt_mapobj_rotate, 1, STRING("ie2_kai2"), LW(2), 0, 0)
        WAIT_FRAMES(5)
        SET_FLOAT_VALUE(LW(1), LW(0))
        MULTIPLY_FLOAT(LW(1), FLOAT(-0.5f))
        CALL(evt_mapobj_rotate, 1, STRING("ie2_kai1"), LW(1), 0, 0)
    END_IF
    RETURN
EVT_END()

/**
 * Enter-house-2 script: hides the "S_gochisou" feast
 * and its hit area outside the story range (STG4_11 to STG4_15), and dips BGM and ambience.
 */
EVT_BEGIN(evt_door2_chk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG4_11_SUPERBOMBBOMB_LOST, STG4_15_DOOPLISS_BATTLE_2)
        CASE_DEFAULT
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_gochisou"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("A_gochisou"), 1, 0)
    END_SWITCH
    CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_envoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_env_lpf, 0, 800)
    RETURN
EVT_END()

/** Leave-house-2 script: restores BGM and ambient sound and removes the low-pass filter set on entry. */
EVT_BEGIN(evt_door2_close)
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_envon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_env_lpf, 0, 0)
    RETURN
EVT_END()

/** House 2, the Mayor's house (door S__ie2_door), opened for him in the chapter 4 arrival scene. */
static DoorParam door_2 = {
    0, // flags
    3, // type
    2, // se
    "S__ie2_door", // mapobj
    "A_ie2door_u", // hitIn
    "A_ie2door", // hitOut
    "S_ie2", // outside
    NULL, // outsideHit
    "S_ie2_naka", // inside
    NULL, // insideHit
    evt_door_2_1, // evtIn
    evt_door_2_2, // evtOut
    evt_wall_2, // evtWall
    (const char**)village_chief_npc, // npcs
    NULL, NULL, // mobjs..items
    evt_door2_chk, // evtOpen
    evt_door2_close, // evtClose
};

/** Door script for house 3's door "S_ie3_door" (evtIn of door_3): turns it about y by LW(0) scaled to door units. */
static EVT_BEGIN(evt_door_3_1)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.5546875f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie3_door"), 0, LW(0), 0)
    RETURN
EVT_END()

/**
 * Door script for house 3's door (evtOut of door_3), spawned with the door angle in LW(0) (0 to 180):
 * turns S_ie3_door about x by LW(0) / 2.
 */
static EVT_BEGIN(evt_door_3_2)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.5f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie3_door"), LW(0), 0, 0)
    RETURN
EVT_END()

/**
 * Wall script of house 3 (DoorParam evtWall), spawned every frame with the opening angle in LW(0) (0 to 180 and
 * back): turns the front-wall pieces ie3_kai1 and ie3_kai2 about x by -LW(0) / 2 and LW(0) / 2,
 * the second 10 frames later.
 */
static EVT_BEGIN(evt_wall_3)
    SET_FLOAT_VALUE(LW(1), LW(0))
    MULTIPLY_FLOAT(LW(1), FLOAT(-0.5f))
    CALL(evt_mapobj_rotate, 1, STRING("ie3_kai1"), LW(1), 0, 0)
    WAIT_FRAMES(10)
    SET_FLOAT_VALUE(LW(2), LW(0))
    MULTIPLY_FLOAT(LW(2), FLOAT(0.5f))
    CALL(evt_mapobj_rotate, 1, STRING("ie3_kai2"), LW(2), 0, 0)
    RETURN
EVT_END()

/** House 3 open callback: dips BGM and ambience to 50 and low-pass filters the environment sound. */
static EVT_BEGIN(door_3_open)
    CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_envoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_env_lpf, 0, 800)
    RETURN
EVT_END()

/** House 3 close callback: restores BGM and ambience and removes the low-pass filter. */
static EVT_BEGIN(door_3_close)
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_envon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_env_lpf, 0, 0)
    RETURN
EVT_END()

/** House 3, the inn (door S_ie3_door), with the innkeeper and the save block inside. */
static DoorParam door_3 = {
    0, // flags
    3, // type
    2, // se
    "S_ie3_door", // mapobj
    "A_ie3door_u", // hitIn
    "A_ie3door", // hitOut
    "S_ie3", // outside
    NULL, // outsideHit
    "S_ie3_naka", // inside
    NULL, // insideHit
    evt_door_3_1, // evtIn
    evt_door_3_2, // evtOut
    evt_wall_3, // evtWall
    yado_npc, // npcs
    yado_mobj, // mobjs
    NULL, // items
    door_3_open, // evtOpen
    door_3_close, // evtClose
};

/**
 * Enter script of the "dokan_1" pipe (to tik_08):
 * before Chapter 4 starts it hides Mario and his partner and waits on GF(0) for the arrival cutscene;
 * returns LW(0) = 0.
 */
EVT_BEGIN(dokan_1_evt)
    IF_INT_LESS(GSW_STORY_PROGRESS, STG4_00_WELCOME_TO_STAGE_4)
        CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_HIDE)
        CALL(evt_party_dispflg_onoff, 0, PARTY_DISP_FLAG_HIDE, 1)
        SET_VALUE(GF(0), 1)
        WAIT_WHILE(GF(0))
        CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_HIDE)
        CALL(evt_party_dispflg_onoff, 0, PARTY_DISP_FLAG_HIDE, 0)
    END_IF
    SET_VALUE(LW(0), 0)
    RETURN
EVT_END()

/** Exits of this map: the pipe down to tik_08 in the Rogueport Sewers and the east road to usu_01. */
static BeroEntry bero_entry_data[] = {
    {
        "dokan_1", // hitName
        2, // type
        0, // sfxId
        BERO_DIR_DOWN, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        dokan_1_evt, // enterEvent
        6, // caseType
        NULL, // exitEvent
        "tik_08", // nextMapName
        "dokan", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "e_bero", // hitName
        0, 0, // type..sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "usu_01", // nextMapName
        "w_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

/**
 * Exit script of bero_oshimodo, which replaces the dokan_1 pipe to tik_08 from STG4_10_DOOPLISS_BATTLE
 * until STG4_15_02_BACK_TO_NORMAL (while Doopliss has Mario's name):
 * the pipe sends Mario back out of itself ("oshimodo", pushed back),
 * then the system message stg4_jin_151 says he cannot enter the pipe and control returns. Returns LW(0) = 0.
 */
static EVT_BEGIN(oshimodo_event)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_key_onoff, 0)
        CALL(evt_bero_exec_wait, 65536)
        CALL(evt_msg_toge, MSG_TOGE_NONE, 0, 0, 0)
        CALL(evt_msg_print, 0, STRING("stg4_jin_151"), 0, 0)
        CALL(evt_mario_key_onoff, 1)
    END_INLINE_EVENT
    SET_VALUE(LW(0), 0)
    RETURN
EVT_END()

/**
 * Replacement for the pipe while Doopliss has Mario's name: it does not lead anywhere and pushes Mario back out
 * (oshimodo_event).
 */
static BeroEntry bero_oshimodo[] = {
    {
        "dokan_1", // hitName
        2, // type
        0, // sfxId
        BERO_DIR_DOWN, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        oshimodo_event, // exitEvent
        0, // nextMapName
        "dokan_1", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
};

/**
 * Bowser interlude: Bowser's (the player slot's) pipe attempt;
 * plays "kpa_stg5_009_2", runs the pipe, then loops on the pipe cases until the scene is done.
 */
EVT_BEGIN(koopa_dokan_evt)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("KPA_S_1"), STRING("KPA_T_1"))
    CALL(evt_msg_print, 0, STRING("kpa_stg5_009_2"), 0, STRING("mario"))
    BEGIN_INLINE_EVENT
        CALL(evt_bero_exec_wait, 65536)
        SET_VALUE(LW(0), STRING("dokan_1"))
        CALL_CHILD(PTR(bero_case_switch_off))
        LOOP(0)
            CALL(evt_mario_get_motion, LW(0))
            IF_INT_EQUAL(LW(0), 3)
                BREAK_LOOP
            END_IF
            IF_INT_EQUAL(LW(0), 10)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        DEBUG_PUT_MSG(STRING("おりた"))
        SET_VALUE(LW(0), STRING("dokan_1"))
        CALL_CHILD(PTR(bero_case_switch_on))
    END_INLINE_EVENT
    CALL(evt_mario_key_onoff, 1)
    SET_VALUE(LW(0), 1)
    RETURN
EVT_END()

/** Replacement for the pipe in Bowser's interlude: Bowser tries it and is told off (koopa_dokan_evt). */
BeroEntry bero_koopa_dokan[] = {
    {
        "dokan_1", // hitName
        2, // type
        0, // sfxId
        BERO_DIR_DOWN, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        koopa_dokan_evt, // exitEvent
        0, NULL, // nextMapName..nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
};

/**
 * Bowser interlude: stops the player (Bowser) from entering house 1 with the message "kpa_stg5_009_4" shown at
 * the door.
 */
EVT_BEGIN(door_ie1_koopa_evt)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mapobj_get_position, STRING("S_ie1_door"), LW(0), LW(1), LW(2))
    CALL(evt_msg_toge, MSG_TOGE_AT_POS, LW(0), LW(1), LW(2))
    CALL(evt_msg_print, 0, STRING("kpa_stg5_009_4"), 0, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/**
 * Bowser interlude: stops the player (Bowser) from entering house 2 with the message "kpa_stg5_009_5" shown at
 * the door.
 */
EVT_BEGIN(door_ie2_koopa_evt)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mapobj_get_position, STRING("S__ie2_door"), LW(0), LW(1), LW(2))
    CALL(evt_msg_toge, MSG_TOGE_AT_POS, LW(0), LW(1), LW(2))
    CALL(evt_msg_print, 0, STRING("kpa_stg5_009_5"), 0, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/**
 * Bowser interlude: stops the player (Bowser) from entering house 3 with the message "kpa_stg5_009_6" shown at
 * the door.
 */
EVT_BEGIN(door_ie3_koopa_evt)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mapobj_get_position, STRING("S_ie3_door"), LW(0), LW(1), LW(2))
    CALL(evt_msg_toge, MSG_TOGE_AT_POS, LW(0), LW(1), LW(2))
    CALL(evt_msg_print, 0, STRING("kpa_stg5_009_6"), 0, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/**
 * Tree-shake script of tree 1 (tree_tbl, A_ki_1):
 * crows A and B on it flap (animation A_1) for 1.5 seconds, then stand again (S_1).
 */
static EVT_BEGIN(evt_ki_1)
    CALL(evt_npc_set_anim, STRING("カラスＡ"), STRING("A_1"))
    CALL(evt_npc_set_anim, STRING("カラスＢ"), STRING("A_1"))
    WAIT_MS(1500)
    CALL(evt_npc_set_anim, STRING("カラスＡ"), STRING("S_1"))
    CALL(evt_npc_set_anim, STRING("カラスＢ"), STRING("S_1"))
    RETURN
EVT_END()

/**
 * Tree-shake script of tree 2 (tree_tbl, A_ki_2):
 * crows C, D and E on it flap (animation A_1) for 1.5 seconds, then stand again (S_1).
 */
static EVT_BEGIN(evt_ki_2)
    CALL(evt_npc_set_anim, STRING("カラスＣ"), STRING("A_1"))
    CALL(evt_npc_set_anim, STRING("カラスＤ"), STRING("A_1"))
    CALL(evt_npc_set_anim, STRING("カラスＥ"), STRING("A_1"))
    WAIT_MS(1500)
    CALL(evt_npc_set_anim, STRING("カラスＣ"), STRING("S_1"))
    CALL(evt_npc_set_anim, STRING("カラスＤ"), STRING("S_1"))
    CALL(evt_npc_set_anim, STRING("カラスＥ"), STRING("S_1"))
    RETURN
EVT_END()

/** Trees of the town square: tree 1 and tree 2 (where crows C, D and E perch) react to being shaken. */
static TreeAccess tree_tbl[] = {
    {2, "A_ki_1", "S_usu00_ki01", NULL, 0, evt_ki_1},
    {2, "A_ki_2", "S_usu00_ki02", NULL, 0, evt_ki_2},
    {2, "A_ki_3", "S_usu00_ki03", NULL, 0, NULL},
    {7, NULL, NULL, NULL, 0, NULL},
};

/** Registers tree_tbl with the shared tree-shaking script (evt_sub_tree_access_entry). */
EVT_BEGIN(usu_00_tree_init)
    SET_VALUE(LW(0), PTR(tree_tbl))
    CALL_CHILD(PTR(evt_sub_tree_access_entry))
    RETURN
EVT_END()

/**
 * Bush "kusa_01" script: during the Bowser interlude,
 * Bowser touching it plays "kpa_stg5_009_7" when the bush is touched and ends the BGM.
 */
EVT_BEGIN(kusa_01_evt)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG5_48_BOWSER_TWILIGHT_TOWN)
        CALL(evt_mario_key_onoff, 0)
        CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("KPA_S_1"), STRING("KPA_T_1"))
        CALL(evt_msg_print, 0, STRING("kpa_stg5_009_7"), PTR(kpa_stg5_009_7_cb), STRING("mario"))
        CALL(evt_snd_bgmoff, PSND_FADEOUT | PSND_SLOT(1))
        CALL(evt_mario_key_onoff, 1)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(evt_kusa_04)
    IF_INT_EQUAL(GF(1), 1)
        RETURN
    END_IF
    CALL(evt_item_delete_check, STRING("item_kusa_04"), LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_item_entry, STRING("item_kusa_04"), ITEM_STAR_PIECE, 130, 0, 455, ITEM_TYPE_SCATTER_NO_PICKUP, GSWF_STARPIECE_61, 0)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(evt_kusa_06)
    IF_INT_EQUAL(GF(1), 1)
        RETURN
    END_IF
    CALL(evt_item_delete_check, STRING("item_kusa_06"), LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_item_entry, STRING("item_kusa_06"), ITEM_COIN, 330, 0, 275, ITEM_TYPE_SCATTER_NO_PICKUP, GSWF_USU_00_COIN_BUSH, 0)
    END_IF
    RETURN
EVT_END()

/**
 * Bushes of the town square: bush 1 has Bowser's line in his interlude, bush 4 hides a Star Piece and bush 6
 * coins; the rest only rustle.
 */
static KusaAccess kusa_tbl[] = {
    {2, "A_kusa_01", "S_kusa_01", kusa_01_evt},
    {2, "A_kusa_02", "S_kusa_02", NULL},
    {2, "A_kusa_03", "S_kusa_03", NULL},
    {2, "A_kusa_04", "S_kusa_04", evt_kusa_04},
    {2, "A_kusa_05", "S_kusa_05", NULL},
    {2, "A_kusa_06", "S_kusa_06", evt_kusa_06},
    {2, "A_kusa_07", "S_kusa_07", NULL},
    {2, "A_kusa_08", "S_kusa_08", NULL},
    {2, "A_kusa_09", "S_kusa_09", NULL},
    {2, "A_kusa_10", "S_kusa_010", NULL},
    {2, "A_kusa_11", "S_kusa_011", NULL},
    {2, "A_kusa_12", "S_kusa_012", NULL},
    {2, "A_kusa_13", "S_kusa_013", NULL},
    {2, "A_kusa_14", "S_kusa_014", NULL},
    {2, "A_kusa_15", "S_kusa_015", NULL},
    {2, "A_kusa_16", "S_kusa_016", NULL},
    {2, "A_kusa_17", "S_kusa_017", NULL},
    {2, "A_kusa_18", "S_kusa_018", NULL},
    {2, "A_kusa_19", "S_kusa_019", NULL},
    {2, "A_kusa_20", "S_kusa_020", NULL},
    {4, NULL, NULL, NULL},
};

/** Registers kusa_tbl with the shared bush script (evt_sub_kusa_access_entry). */
EVT_BEGIN(usu_00_kusa_init)
    SET_VALUE(LW(0), PTR(kusa_tbl))
    CALL_CHILD(PTR(evt_sub_kusa_access_entry))
    RETURN
EVT_END()

/** Pickup callback for the rolling Super Bombomb: sets the story to STG4_12_01_SUPERBOMBBOMB_FOUND. */
EVT_BEGIN(evt_ichikorori_ball_get)
    SET_VALUE(GSW_STORY_PROGRESS, STG4_12_01_SUPERBOMBBOMB_FOUND)
    RETURN
EVT_END()

/**
 * Touch trigger for the loose Super Bombomb ("ichikorori" ball):
 * once Vivian has joined, it bounces the item away from Mario, with gravity and bounce rate set,
 * until he picks it up.
 */
EVT_BEGIN(usu_00_touch_ichikorori_ball)
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG4_12_VIVIAN)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        SET_VALUE(LW(3), -260)
        SET_VALUE(LW(4), 0)
        SET_VALUE(LW(5), 325)
        IF_INT_LESS(LW(2), LW(5))
            ADD(LW(5), 20)
            ELSE
            ADD(LW(5), -20)
        END_IF
        CALL(evt_sub_get_dir, LW(0), LW(2), LW(3), LW(5), LW(6))
        CALL(evt_item_entry, STRING("ichikorori_ball"), ITEM_SUPERBOMBOMB, LW(3), LW(4), LW(5), ITEM_TYPE_SCATTER, -1, PTR(evt_ichikorori_ball_get))
        CALL(evt_item_set_move_dir_speed, STRING("ichikorori_ball"), LW(6), FLOAT(90.0f))
        CALL(evt_item_set_gravity, STRING("ichikorori_ball"), FLOAT(1.0f))
        CALL(evt_item_set_bound_rate, STRING("ichikorori_ball"), FLOAT(0.5996094f))
        CALL(evt_item_set_jump_power, STRING("ichikorori_ball"), FLOAT(500.0f))
        CALL(evt_item_status_on, STRING("ichikorori_ball"), ITEM_STATUS_CUSTOM_LAUNCH)
        CALL(evt_exit_case_evt)
    END_IF
    RETURN
EVT_END()

/** The five pig NPCs (pig villagers A, D, E, K and the pig Mayor), whose models get the paper lighting. */
static BoobooName booboo_name[] = {
    {/*"ブタ村人Ａ"*/"\x83\x75\x83\x5E\x91\xBA\x90\x6C\x82\x60", 2, 1},
    {/*"ブタ村人Ｄ"*/"\x83\x75\x83\x5E\x91\xBA\x90\x6C\x82\x63", 0, 1},
    {/*"ブタ村人Ｅ"*/"\x83\x75\x83\x5E\x91\xBA\x90\x6C\x82\x64", 6, 1},
    {/*"ブタ村人Ｋ"*/"\x83\x75\x83\x5E\x91\xBA\x90\x6C\x82\x6A", 5, 1},
    {/*"ブタ村長"*/"\x83\x75\x83\x5E\x91\xBA\x92\xB7", 9, 0},
};

/**
 * Sprite list of the 200-strong crowd in Bowser's second Twilight Town scene: X-Nauts (c_zako_n) for Crump's
 * side, Koopatrols, Hammer Bros and Magikoopas for Kammy's, each pose with a staggered start time.
 */
ExtTexInfo kpaSen_ext_entry_data[] = {
    {"c_zako_n", "S_1", 0.0f},
    {"c_zako_n", "S_1", 8.0f},
    {"c_zako_n", "S_1", 16.0f},
    {"c_zako_n", "S_1", 24.0f},
    {"c_zako_n", "S_1", 32.0f},
    {"c_zako_n", "S_1", 40.0f},
    {"c_zako_n", "R_1", 0.0f},
    {"c_zako_n", "R_1", 7.0f},
    {"c_zako_n", "R_1", 14.0f},
    {"c_togenoko", "S_1", 0.0f},
    {"c_togenoko", "S_1", 8.0f},
    {"c_togenoko", "S_1", 16.0f},
    {"c_togenoko", "S_1", 24.0f},
    {"c_togenoko", "S_1", 32.0f},
    {"c_togenoko", "S_1", 40.0f},
    {"c_togenoko", "R_1", 0.0f},
    {"c_togenoko", "R_1", 7.0f},
    {"c_togenoko", "R_1", 14.0f},
    {"c_burosu_h", "BRO_S_1", 0.0f},
    {"c_burosu_h", "BRO_S_1", 8.0f},
    {"c_burosu_h", "BRO_S_1", 16.0f},
    {"c_burosu_h", "BRO_S_1", 24.0f},
    {"c_burosu_h", "BRO_S_1", 32.0f},
    {"c_burosu_h", "BRO_S_1", 40.0f},
    {"c_burosu_h", "BRO_R_1", 0.0f},
    {"c_burosu_h", "BRO_R_1", 7.0f},
    {"c_burosu_h", "BRO_R_1", 14.0f},
    {"c_kamek", "KMK_S_1", 0.0f},
    {"c_kamek", "KMK_S_1", 8.0f},
    {"c_kamek", "KMK_S_1", 16.0f},
    {"c_kamek", "KMK_S_1", 24.0f},
    {"c_kamek", "KMK_S_1", 32.0f},
    {"c_kamek", "KMK_S_1", 40.0f},
    {"c_kamek", "KMK_S_1", 48.0f},
    {"c_kamek", "KMK_S_1", 56.0f},
    {"c_kamek", "KMK_R_1", 0.0f},
    {"c_kamek", "KMK_R_1", 7.0f},
    {"c_kamek", "KMK_R_1", 14.0f},
    {0},
};

/**
 * Init of the Koopa-troop crowd effect used in the Bowser interlude:
 * allocates 200 sprite records and lays them out in rows (100 troopers, 100 spiky ones).
 */
void kpaSen_ext_init(void) {
    ExtdrvPose* pose = extGetPosePtr();
    Vec base = {85.0f, 0.0f, 75.0f};
    s32 i;

    kpaSen = _mapAlloc(sizeof(KpaSen) * 200);
    for (i = 0; i < extGetPoseNum(); i++, pose++) {
        pose->flags |= (EXT_POSE_FLAG_VISIBLE | EXT_POSE_FLAG_BILLBOARD);
        PSMTXIdentity(*pose->mtx);
        if (i < 100) {
            pose->dir = 90.0f;
            kpaSen[i].end.x = (base.x + 30.0f * ((f32)i / 10.0f)) - 9.0f;
            kpaSen[i].end.y = 0.0f;
            kpaSen[i].end.z = base.z + 50.0f * (f32)(i % 10 - 5);
            (*pose->mtx)[0][3] = kpaSen[i].start.x = base.x + 200.0f;
            (*pose->mtx)[1][3] = kpaSen[i].start.y = 0.0f;
            (*pose->mtx)[2][3] = kpaSen[i].start.z = base.z + 20.0f * (f32)(i % 10 - 5);
            kpaSen[i].wait = (i / 10) * 3 + 1;
            kpaSen[i].pose99Lo = 0;
            kpaSen[i].pose99Hi = 5;
            kpaSen[i].pose1Lo = 6;
            kpaSen[i].pose1Hi = 8;
        } else {
            pose->dir = 270.0f;
            kpaSen[i].end.x = (base.x + -30.0f * ((f32)(i - 100) / 10.0f)) - 150.0f;
            kpaSen[i].end.y = 0.0f;
            kpaSen[i].end.z = base.z + 50.0f * (f32)((i - 100) % 10 - 5);
            (*pose->mtx)[0][3] = kpaSen[i].start.x = base.x - 350.0f;
            (*pose->mtx)[1][3] = kpaSen[i].start.y = 0.0f;
            (*pose->mtx)[2][3] = kpaSen[i].start.z = base.z + 20.0f * (f32)((i - 100) % 10 - 5);
            kpaSen[i].wait = ((i - 100) / 10) * 3 + 1;
            switch (rand() % 3) {
                case 0:
                    kpaSen[i].pose99Lo = 9;
                    kpaSen[i].pose99Hi = 14;
                    kpaSen[i].pose1Lo = 15;
                    kpaSen[i].pose1Hi = 17;
                    break;
                case 1:
                    kpaSen[i].pose99Lo = 18;
                    kpaSen[i].pose99Hi = 23;
                    kpaSen[i].pose1Lo = 24;
                    kpaSen[i].pose1Hi = 26;
                    break;
                case 2:
                    kpaSen[i].pose99Lo = 27;
                    kpaSen[i].pose99Hi = 34;
                    kpaSen[i].pose1Lo = 35;
                    kpaSen[i].pose1Hi = 37;
                    break;
            }
        }
        kpaSen[i].state = -1;
        kpaSen[i].t = 0.0f;
        if (i == 6) {
            pose->flags &= ~EXT_POSE_FLAG_VISIBLE;
        }
        if (i == 106 || i == 116) {
            pose->flags &= ~EXT_POSE_FLAG_VISIBLE;
        }
    }
}

/**
 * Picks an animation frame between lo and hi for crowd sprite i,
 * mixing in retraceCount so neighboring sprites animate out of step (6 or 7 ticks per frame depending on flag).
 */
s32 calcPose(s32 i, s32 lo, s32 hi, s32 flag) {
    s32 range = hi - lo + 1;
    s32 n = flag ? 6 : 7;
    u32 t = gp->retraceCount + i * i;

    return lo + (t % (range * n)) / n;
}

/** Per-frame update of the Koopa-troop crowd: counts down each sprite's wait and runs its marching/animation state. */
void kpaSen_ext_main(void) {
    ExtdrvPose* pose = extGetPosePtr();
    s32 i;

    for (i = 0; i < extGetPoseNum(); i++, pose++) {
        switch (kpaSen[i].state) {
            case 0:
                kpaSen[i].wait--;
                if (kpaSen[i].wait == 0) {
                    kpaSen[i].state++;
                }
                break;
            case 1:
                if (i < 100) {
                    pose->texId = calcPose(i, kpaSen[i].pose1Lo, kpaSen[i].pose1Hi, 1);
                    if (i % 10 == 0) {
                        kpaSen[i].t = kpaSen[i].t;
                        pose[i / 10].dir = pose[i / 10].dir;
                        (*pose[i / 10].mtx)[0][3] = (*pose[i / 10].mtx)[0][3];
                    }
                } else {
                    pose->texId = calcPose(i - 100, kpaSen[i].pose1Lo, kpaSen[i].pose1Hi, 1);
                }
                (*pose->mtx)[0][3] = kpaSen[i].start.x * (1.0f - kpaSen[i].t) + kpaSen[i].end.x * kpaSen[i].t;
                (*pose->mtx)[2][3] = kpaSen[i].start.z * (1.0f - kpaSen[i].t) + kpaSen[i].end.z * kpaSen[i].t;
                kpaSen[i].t += 0.03;
                kpaSen[i].wait++;
                if (kpaSen[i].t >= 1.0f) {
                    (*pose->mtx)[0][3] = kpaSen[i].end.x;
                    (*pose->mtx)[2][3] = kpaSen[i].end.z;
                    kpaSen[i].state = 99;
                }
                break;
            case 99:
                if (i < 100) {
                    pose->texId = calcPose(i, kpaSen[i].pose99Lo, kpaSen[i].pose99Hi, 0);
                } else {
                    {
                        s32 lo = kpaSen[i].pose99Lo;
                        s32 hi = kpaSen[i].pose99Hi;
                        pose->texId = calcPose(i - 100, lo, hi, 0);
                    }
                }
                break;
            case -1:
                break;
        }
    }
}

/**
 * Draw callback of the Koopa-troop crowd: loads the render mode, vertices, textures and TEV,
 * then draws every sprite.
 */
void kpaSen_ext_disp(CameraId camera, void* param) {
    extLoadRenderMode();
    extLoadVertex();
    extLoadTexture();
    extLoadTev();
    extDraw();
    extLoadTextureExit();
}

/** Shadow draw callback of the Koopa-troop crowd: draws a flattened copy of the sprites using the shadow matrix. */
void kpaSen_ext_shadow_disp(CameraId camera, void* param) {
    static Mtx mt = {
        {10.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 10.0f, 0.0f, 0.1f},
        {0.0f, 0.0f, 10.0f, 0.0f},
    };

    extLoadShadowMtx(mt);
    extLoadShadowRenderMode();
    extLoadShadowVertex();
    extLoadShadowTev();
    extDrawShadow();
}

/** Talk script of Kammy ("Baba") in the Bowser interlude: one line, "kpa_stg5_009_1". */
static EVT_BEGIN(baba_talk)
    CALL(evt_msg_print, 0, STRING("kpa_stg5_009_1"), 0, STRING("me"))
    RETURN
EVT_END()

/**
 * Regular-behavior script of Kanbu (the Lord Crump-like "third-force executive"):
 * paces between x=-200 and x=100, turning around at each end, forever.
 */
EVT_BEGIN(kanbu_regl)
    CALL(evt_seq_wait, SEQ_GAME)
    LOOP(0)
        CALL(evt_npc_move_position, STRING("カンブー"), -200, 200, 0, FLOAT(30.0f), 0)
        WAIT_MS(300)
        CALL(evt_npc_reverse_ry, STRING("カンブー"))
        WAIT_MS(300)
        CALL(evt_npc_reverse_ry, STRING("カンブー"))
        WAIT_MS(300)
        CALL(evt_npc_move_position, STRING("カンブー"), 100, 200, 0, FLOAT(30.0f), 0)
        WAIT_MS(300)
        CALL(evt_npc_reverse_ry, STRING("カンブー"))
        WAIT_MS(300)
        CALL(evt_npc_reverse_ry, STRING("カンブー"))
        WAIT_MS(300)
    END_LOOP
    RETURN
EVT_END()


void kpaSen_ext_dispent(void) {
    dispEntry(CAMERA_3D, 0.0f, 1, kpaSen_ext_disp, NULL);
    dispEntry(CAMERA_3D, 0.0f, 2, kpaSen_ext_shadow_disp, NULL);
}

/** Script helper: resets the first 100 crowd sprites (the troopers) to state 0 so they gather again. */
USER_FUNC(kpaSen_zako_syugo) {
    s32 i;

    for (i = 0; i < 100; i++) {
        kpaSen[i].state = 0;
    }
    return EVT_RETURN_DONE;
}

/** Script helper: resets crowd sprites 100 to 199 (the spiky ones) to state 0 so they gather again. */
USER_FUNC(kpaSen_toge_syugo) {
    s32 i;

    for (i = 100; i < 200; i++) {
        kpaSen[i].state = 0;
    }
    return EVT_RETURN_DONE;
}

/** Script helper: spawns a small star effect at the x, y, z in args[0..2]. */
static USER_FUNC(small_star) {
    EvtWord* args = event->args;
    f32 x = evtGetFloat(event, args[0]);
    f32 y = evtGetFloat(event, args[1]);
    f32 z = evtGetFloat(event, args[2]);

    effSmallStarEntry(3, 2, x, y, z, 0.0f, -1.0f, 0.0f);
    return EVT_RETURN_DONE;
}

/** Script helper: spawns an explosion smoke puff at x = args[0], y = args[1] for the Bowser interlude blast. */
USER_FUNC(bakuhatu_kemuri) {
    EvtWord* args = event->args;
    f32 x = evtGetFloat(event, args[0]);
    f32 y = evtGetFloat(event, args[1]);

    effKemuTestDrawCam(effKemuTestEntry(8, x, y, 0.0f, 2.5f), 8);
    return EVT_RETURN_DONE;
}

/** Script helper: turns the white fade-out overlay on. */
USER_FUNC(white_on) {
    fadeWhiteOutOn();
    return EVT_RETURN_DONE;
}

/** Script helper: turns the white fade-out overlay off. */
USER_FUNC(white_off) {
    fadeWhiteOutOff();
    return EVT_RETURN_DONE;
}

/** Talk script of Kanbu (the Lord Crump-like "third-force executive"): runs the second Bowser interlude scene. */
EVT_BEGIN(kanbu_talk)
    CALL_CHILD(PTR(usu_00_koopa_second_evt))
    RETURN
EVT_END()

static EVT_BEGIN(koopa_npc_entry)
    CALL(evt_npc_get_reglid, STRING("ビビアン"), LW(0))
    DELETE_EVENT(LW(0))
    CALL(evt_npc_get_reglid, STRING("ブタ村長"), LW(0))
    DELETE_EVENT(LW(0))
    CALL(evt_npc_get_reglid, STRING("ブタ村人Ｋ"), LW(0))
    DELETE_EVENT(LW(0))
    CALL(evt_npc_get_reglid, STRING("ブタ村人Ｅ"), LW(0))
    DELETE_EVENT(LW(0))
    CALL(evt_npc_get_reglid, STRING("ブタ村人Ｄ"), LW(0))
    DELETE_EVENT(LW(0))
    CALL(evt_npc_get_reglid, STRING("ブタ村人Ａ"), LW(0))
    DELETE_EVENT(LW(0))
    CALL(evt_npc_get_reglid, STRING("宿店員"), LW(0))
    DELETE_EVENT(LW(0))
    CALL(evt_npc_get_reglid, STRING("村人Ｂ"), LW(0))
    DELETE_EVENT(LW(0))
    CALL(evt_npc_get_reglid, STRING("村長"), LW(0))
    DELETE_EVENT(LW(0))
    CALL(evt_npc_delete, STRING("ビビアン"))
    CALL(evt_npc_delete, STRING("ブタ村長"))
    CALL(evt_npc_delete, STRING("ブタ村人Ｋ"))
    CALL(evt_npc_delete, STRING("ブタ村人Ｅ"))
    CALL(evt_npc_delete, STRING("ブタ村人Ｄ"))
    CALL(evt_npc_delete, STRING("ブタ村人Ａ"))
    CALL(evt_npc_delete, STRING("宿店員"))
    CALL(evt_npc_delete, STRING("村人Ｂ"))
    CALL(evt_npc_delete, STRING("村長"))
    CALL(evt_npc_entry, STRING("ババ"), STRING("c_kamek_be"))
    CALL(evt_npc_set_tribe, STRING("ババ"), STRING("カメックババ"))
    CALL(evt_npc_set_position, STRING("ババ"), 0, -1000, 0)
    CALL(evt_npc_set_ry, STRING("ババ"), 90)
    CALL(evt_npc_flag_onoff, 1, STRING("ババ"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_status_onoff, 1, STRING("ババ"), NPC_STATUS_FACE_MOVE_DIR)
    CALL(evt_npc_change_interrupt, STRING("ババ"), 6, PTR(baba_talk))
    CALL(evt_npc_set_width, STRING("ババ"), 30)
    CALL(evt_npc_set_height, STRING("ババ"), 40)
    CALL(evt_npc_entry, STRING("カンブー"), STRING("c_kanbu"))
    CALL(evt_npc_set_tribe, STRING("カンブー"), STRING("第三勢力幹部"))
    CALL(evt_npc_set_position, STRING("カンブー"), 100, 0, 200)
    CALL(evt_npc_set_ry, STRING("カンブー"), 90)
    CALL(evt_npc_flag_onoff, 1, STRING("カンブー"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_TALK_TURN)
    CALL(evt_npc_status_onoff, 1, STRING("カンブー"), NPC_STATUS_FACE_MOVE_DIR)
    CALL(evt_npc_change_interrupt, STRING("カンブー"), 6, PTR(kanbu_talk))
    CALL(evt_npc_set_width, STRING("カンブー"), 30)
    CALL(evt_npc_set_height, STRING("カンブー"), 60)
    CALL(evt_npc_entry, STRING("トゲノコ"), STRING("c_togenoko"))
    CALL(evt_npc_set_position, STRING("トゲノコ"), 0, -1000, 0)
    CALL(evt_npc_entry, STRING("ハンマー"), STRING("c_burosu_h"))
    CALL(evt_npc_set_position, STRING("ハンマー"), 0, -1000, 0)
    CALL(evt_npc_entry, STRING("カメック"), STRING("c_kamek"))
    CALL(evt_npc_set_position, STRING("カメック"), 0, -1000, 0)
    CALL(evt_npc_entry, STRING("ザコ"), STRING("c_zako_n"))
    CALL(evt_npc_set_position, STRING("ザコ"), 0, -1000, 0)
    RETURN
EVT_END()

EVT_BEGIN(usu_00_koopa_first_evt)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_set_pos, -300, 0, 50)
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_mario_flag_onoff, 1, 1024)
    CALL(evt_npc_set_position, STRING("ババ"), -400, 25, 100)
    CALL(evt_npc_set_ry, STRING("ババ"), 90)
    CALL(evt_npc_flag_onoff, 1, STRING("ババ"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_cam3d_evt_set, -50, 213, 677, -50, 100, 37, 0, EASING_SINE_IN_OUT)
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_npc_get_reglid, STRING("村人Ｃ"), LW(0))
    DELETE_EVENT(LW(0))
    CALL(evt_bero_exec_wait, 65536)
    CALL(evt_fade_end_wait)
    SET_VALUE(GF(0), 0)
    BEGIN_INLINE_EVENT
        LOOP(0)
            IF_INT_NOT_EQUAL(GF(0), 0)
                BREAK_LOOP
            END_IF
            CALL(evt_npc_get_position, STRING("ババ"), LW(0), LW(1), LW(2))
            CALL(evt_snd_sfxon_3d, STRING("SFX_KUPPA_KAMEBABA_FLY1"), LW(0), LW(1), LW(2), 0)
            WAIT_FRAMES(10)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_kamek_move_position, STRING("ババ"), -100, 100, 10, 0, 60, 0)
        SET_VALUE(GF(0), 1)
    END_INLINE_EVENT
    CALL(evt_mario_mov_pos2, 0, 50, 60)
    CALL(evt_mario_set_dir, 270, 300, 0)
    WAIT_MS(300)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_KUPPA1"))
    CALL(evt_snd_bgm_scope, 0, 1)
    CALL(evt_cam3d_evt_set, -50, 138, 480, -50, 60, 37, 800, EASING_SINE_IN_OUT)
    WAIT_MS(800)
    CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("KPA_S_1"), STRING("KPA_T_1"))
    CALL(evt_msg_print, 0, STRING("kpa_stg5_000"), 0, STRING("mario"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("kpa_stg5_001"), 0, STRING("ババ"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("kpa_stg5_002"), 0, STRING("mario"))
    CALL(evt_mario_set_dir, 90, 300, 0)
    CALL(evt_mario_flag_onoff, 0, 1024)
    CALL(evt_npc_flag_onoff, 0, STRING("ババ"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_cam3d_evt_off, 800, EASING_SINE_IN_OUT)
    WAIT_MS(800)
    CALL(evt_npc_restart_regular_event, STRING("村人Ｃ"))
    SET_VALUE(GSW_STORY_PROGRESS, STG5_48_BOWSER_TWILIGHT_TOWN)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_snd_bgm_scope, 0, 0)
    CALL(evt_snd_bgm_scope, 0, 0)
    RETURN
EVT_END()

EVT_BEGIN(bakuhatu_evt)
    CALL(bakuhatu_kemuri, FLOAT(0.0f), FLOAT(0.0f))
    WAIT_MS(100)
    CALL(bakuhatu_kemuri, FLOAT(0.0f), FLOAT(180.0f))
    CALL(bakuhatu_kemuri, FLOAT(161.19531f), FLOAT(-127.259766f))
    CALL(bakuhatu_kemuri, FLOAT(-228.0f), FLOAT(0.0f))
    WAIT_MS(40)
    CALL(bakuhatu_kemuri, FLOAT(0.0f), FLOAT(-180.0f))
    CALL(bakuhatu_kemuri, FLOAT(-161.19531f), FLOAT(127.259766f))
    CALL(bakuhatu_kemuri, FLOAT(161.19531f), FLOAT(127.259766f))
    WAIT_MS(40)
    CALL(bakuhatu_kemuri, FLOAT(228.0f), FLOAT(0.0f))
    CALL(bakuhatu_kemuri, FLOAT(-161.19531f), FLOAT(-127.259766f))
    WAIT_MS(100)
    CALL(bakuhatu_kemuri, FLOAT(0.0f), FLOAT(0.0f))
    CALL(bakuhatu_kemuri, FLOAT(171.19531f), FLOAT(-127.259766f))
    WAIT_MS(70)
    CALL(bakuhatu_kemuri, FLOAT(-171.19531f), FLOAT(127.259766f))
    CALL(bakuhatu_kemuri, FLOAT(171.19531f), FLOAT(127.259766f))
    RETURN
EVT_END()

EVT_BEGIN(murabito_out)
    CALL(delete_npc_regl_evt, STRING("村人Ｃ"))
    CALL(delete_npc_regl_evt, STRING("村人Ｅ"))
    BEGIN_INLINE_EVENT
        CALL(evt_npc_flag_onoff, 1, STRING("村人Ｄ"), NPC_FLAG_NO_WALL_COLLISION)
        CALL(evt_npc_status_onoff, 1, STRING("村人Ｄ"), NPC_STATUS_FACE_MOVE_DIR)
        CALL(evt_npc_set_ry, STRING("村人Ｄ"), 90)
        WAIT_MS(300)
        CALL(evt_npc_move_position, STRING("村人Ｄ"), -207, -150, 0, 180, 0)
        CALL(evt_npc_set_position, STRING("村人Ｄ"), 0, -1000, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_flag_onoff, 1, STRING("村人Ｃ"), NPC_FLAG_NO_WALL_COLLISION)
        CALL(evt_npc_status_onoff, 1, STRING("村人Ｃ"), NPC_STATUS_FACE_MOVE_DIR)
        CALL(evt_npc_set_ry, STRING("村人Ｃ"), 90)
        WAIT_MS(300)
        CALL(evt_npc_move_position, STRING("村人Ｃ"), -340, 60, 0, 180, 0)
        CALL(evt_npc_set_position, STRING("村人Ｃ"), 0, -1000, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_flag_onoff, 1, STRING("村人Ｅ"), NPC_FLAG_NO_WALL_COLLISION)
        CALL(evt_npc_status_onoff, 1, STRING("村人Ｅ"), NPC_STATUS_FACE_MOVE_DIR)
        CALL(evt_npc_move_position, STRING("村人Ｅ"), 440, 200, 0, 180, 0)
        CALL(evt_npc_set_position, STRING("村人Ｅ"), 0, -1000, 0)
    END_INLINE_EVENT
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_flag_onoff, 1, STRING("村人Ａ"), NPC_FLAG_NO_WALL_COLLISION)
        CALL(evt_npc_status_onoff, 1, STRING("村人Ａ"), NPC_STATUS_FACE_MOVE_DIR)
        CALL(evt_npc_set_ry, STRING("村人Ａ"), 90)
        WAIT_MS(300)
        CALL(evt_npc_move_position, STRING("村人Ａ"), -340, 30, 0, 180, 0)
        CALL(evt_npc_set_position, STRING("村人Ａ"), 0, -1000, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_flag_onoff, 1, STRING("村人Ｋ"), NPC_FLAG_NO_WALL_COLLISION)
        CALL(evt_npc_status_onoff, 1, STRING("村人Ｋ"), NPC_STATUS_FACE_MOVE_DIR)
        CALL(evt_npc_set_ry, STRING("村人Ｋ"), 90)
        WAIT_MS(300)
        CALL(evt_npc_move_position, STRING("村人Ｋ"), -170, -190, 0, 180, 0)
        CALL(evt_npc_move_position, STRING("村人Ｋ"), -207, -215, 0, 150, 0)
        CALL(evt_npc_set_position, STRING("村人Ｋ"), 0, -1000, 0)
    END_INLINE_EVENT
    RETURN
EVT_END()

EVT_BEGIN(usu_00_koopa_second_evt)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_set_motion, MARIO_MOTION_STAY)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_npc_get_position, STRING("カンブー"), LW(3), LW(4), LW(5))
    CALL(evt_sub_get_dir, LW(0), LW(2), LW(3), LW(5), LW(6))
    IF_INT_LESS(LW(6), 180)
        SET_VALUE(LW(7), 90)
        ELSE
        SET_VALUE(LW(7), 270)
    END_IF
    CALL(evt_npc_set_ry, STRING("カンブー"), LW(7))
    CALL(evt_msg_print, 0, STRING("kpa_stg5_010_00"), 0, STRING("カンブー"))
    CALL(evt_npc_get_position, STRING("カンブー"), LW(0), LW(1), LW(2))
    IF_INT_LESS(LW(0), 75)
        CALL(evt_npc_set_ry, STRING("カンブー"), 90)
        ELSE
        CALL(evt_npc_set_ry, STRING("カンブー"), 270)
    END_IF
    WAIT_FRAMES(1)
    CALL(evt_npc_wait_pera, STRING("カンブー"))
    WAIT_MS(500)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_NPC, STRING(""), STRING("カンブー"), 0, 0, 0, 0, 0, 0, 0, 60)
    WAIT_MS(1200)
    BEGIN_INLINE_EVENT
        WAIT_MS(500)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        IF_INT_LESS(LW(0), 75)
            CALL(evt_mario_set_dir, 90, 300, 0)
            CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_NO_MAKE_DISP_DIR)
        END_IF
        CALL(evt_mario_mov_pos, -70, 290, 1500)
        CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_NO_MAKE_DISP_DIR)
        CALL(evt_mario_set_dir_npc, STRING("カンブー"))
    END_INLINE_EVENT
    SET_VALUE(GF(0), 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(500)
        LOOP(0)
            IF_INT_NOT_EQUAL(GF(0), 0)
                BREAK_LOOP
            END_IF
            CALL(evt_npc_get_position, STRING("ババ"), LW(0), LW(1), LW(2))
            CALL(evt_snd_sfxon_3d, STRING("SFX_KUPPA_KAMEBABA_FLY1"), LW(0), LW(1), LW(2), 0)
            WAIT_FRAMES(10)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_flag_onoff, 1, STRING("ババ"), NPC_FLAG_NO_HIT)
        WAIT_MS(500)
        CALL(evt_npc_kamek_move_position, STRING("ババ"), -15, 240, 8, 1500, 0, 0)
        CALL(evt_npc_flag_onoff, 0, STRING("ババ"), NPC_FLAG_NO_HIT)
        SET_VALUE(GF(0), 1)
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_set, 75, 116, 612, 75, 50, 268, 1500, EASING_SINE_IN_OUT)
    CALL(evt_npc_flag_onoff, 1, STRING("カンブー"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_move_position, STRING("カンブー"), 75, 275, 1500, 0, 4)
    WAIT_MS(300)
    CALL(evt_npc_flag_onoff, 0, STRING("カンブー"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_get_position, STRING("カンブー"), LW(0), LW(1), LW(2))
    CALL(evt_npc_jump_position_nohit, STRING("カンブー"), LW(0), LW(1), LW(2), 300, 20)
    WAIT_MS(500)
    CALL(evt_snd_sfxon_3d, STRING("SFX_GRASS_SHAKE1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_npc_set_anim, STRING("カンブー"), STRING("C_2"))
    WAIT_MS(300)
    CALL(evt_npc_get_dir, STRING("カンブー"), LW(3))
    IF_INT_LESS(LW(3), 180)
        ADD(LW(0), 10)
        ELSE
        SUBTRACT(LW(0), 10)
    END_IF
    ADD(LW(1), 45)
    ADD(LW(2), 5)
    CALL(evt_item_entry, STRING("item"), ITEM_SUPERBOMBOMB, LW(0), LW(1), LW(2), ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
    WAIT_MS(800)
    CALL(evt_npc_set_stay_anim, STRING("カンブー"))
    CALL(evt_item_delete, STRING("item"))
    CALL(evt_msg_print, 0, STRING("kpa_stg5_010"), 0, STRING("カンブー"))
    CALL(evt_cam3d_evt_set, 0, 147, 737, 0, 60, 245, 800, EASING_SINE_IN_OUT)
    WAIT_MS(800)
    CALL(evt_npc_set_anim, STRING("ババ"), STRING("A_3A"))
    WAIT_MS(300)
    CALL(evt_npc_set_autotalkpose, STRING("ババ"), STRING("A_3B"), STRING("T_10"))
    CALL(evt_msg_print, 0, STRING("kpa_stg5_011"), 0, STRING("ババ"))
    CALL(evt_npc_set_autotalkpose, STRING("ババ"), STRING("S_2"), STRING("T_9"))
    CALL(evt_npc_set_stay_anim, STRING("ババ"))
    CALL(evt_npc_set_ry, STRING("カンブー"), 270)
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("kpa_stg5_012"), 0, STRING("カンブー"))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    ADD(LW(0), 10)
    CALL(evt_mario_mov_pos2, LW(0), LW(2), FLOAT(60.0f))
    CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("KPA_S_4"), STRING("KPA_T_3"))
    CALL(evt_msg_print, 0, STRING("kpa_stg5_013"), 0, STRING("mario"))
    CALL(evt_npc_status_onoff, 0, STRING("カンブー"), NPC_STATUS_FACE_MOVE_DIR)
    CALL(evt_npc_get_position, STRING("カンブー"), LW(0), LW(1), LW(2))
    ADD(LW(0), 10)
    CALL(evt_npc_move_position, STRING("カンブー"), LW(0), LW(2), 0, FLOAT(60.0f), 0)
    CALL(evt_msg_print, 0, STRING("kpa_stg5_014"), 0, STRING("カンブー"))
    CALL(evt_npc_get_position, STRING("カンブー"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_KUPPA_3RD_SHUGOU1"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(500)
    CALL(evt_ext_entry, 200, PTR(kpaSen_ext_entry_data), PTR(kpaSen_ext_init), PTR(kpaSen_ext_main), PTR(kpaSen_ext_dispent))
    SPAWN_EVENT(PTR(murabito_out))
    WAIT_MS(500)
    CALL(evt_cam3d_evt_set, 40, 90, 381, 85, 45, 113, 800, EASING_SINE_IN_OUT)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("カンブー"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_KUPPA_TAIGUN_MOVE1"), LW(0), LW(1), LW(2), 0)
        CALL(kpaSen_zako_syugo)
    END_INLINE_EVENT
    CALL(evt_npc_jump_position_nohit, STRING("カンブー"), 93, 0, 109, 500, 60)
    WAIT_MS(1300)
    CALL(evt_cam3d_evt_set, 42, 29, 375, 85, 45, 113, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_msg_print, 0, STRING("kpa_stg5_015"), 0, STRING("カンブー"))
    CALL(evt_cam3d_evt_set, -80, 172, 456, -80, 50, 78, 800, EASING_SINE_IN_OUT)
    WAIT_MS(800)
    CALL(evt_mario_flag_onoff, 1, 1024)
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_NO_MAKE_DISP_DIR)
    CALL(evt_npc_status_onoff, 0, STRING("ババ"), NPC_STATUS_FACE_MOVE_DIR)
    SET_VALUE(GF(0), 0)
    BEGIN_INLINE_EVENT
        LOOP(0)
            IF_INT_NOT_EQUAL(GF(0), 0)
                BREAK_LOOP
            END_IF
            CALL(evt_npc_get_position, STRING("ババ"), LW(0), LW(1), LW(2))
            CALL(evt_snd_sfxon_3d, STRING("SFX_KUPPA_KAMEBABA_FLY1"), LW(0), LW(1), LW(2), 0)
            WAIT_FRAMES(10)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_kamek_move_position, STRING("ババ"), -43, 90, 10, 0, FLOAT(90.0f), 0)
        SET_VALUE(GF(0), 1)
        CALL(evt_npc_set_ry, STRING("ババ"), 90)
    END_INLINE_EVENT
    CALL(evt_mario_mov_pos2, -103, 120, FLOAT(90.0f))
    CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_NO_MAKE_DISP_DIR)
    CALL(evt_mario_set_dir, 90, 300, 0)
    LOOP(0)
        IF_INT_NOT_EQUAL(GF(0), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_MS(500)
    CALL(evt_npc_set_anim, STRING("ババ"), STRING("A_3A"))
    WAIT_MS(300)
    CALL(evt_npc_set_autotalkpose, STRING("ババ"), STRING("A_3B"), STRING("T_10"))
    CALL(evt_msg_print, 0, STRING("kpa_stg5_016"), 0, STRING("ババ"))
    WAIT_FRAMES(1)
    CALL(evt_npc_get_position, STRING("ババ"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_KUPPA_KAMEBABA_SHUGOU1"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(500)
    CALL(evt_snd_sfxon_3d, STRING("SFX_KUPPA_TAIGUN_MOVE1"), LW(0), LW(1), LW(2), 0)
    CALL(kpaSen_toge_syugo)
    WAIT_MS(1500)
    CALL(evt_npc_set_autotalkpose, STRING("ババ"), STRING("S_2"), STRING("T_9"))
    CALL(evt_npc_set_stay_anim, STRING("ババ"))
    CALL(evt_cam3d_evt_set, -14, 225, 715, -4, 57, 120, 1000, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("kpa_stg5_017"), 0, STRING("カンブー"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("kpa_stg5_018"), 0, STRING("ババ"))
    WAIT_MS(2000)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_NPC, STRING(""), STRING("カンブー"), 0, 0, 0, 0, 0, 0, 0, 60)
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_set, 40, 90, 381, 85, 45, 113, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_msg_print, 0, STRING("kpa_stg5_019"), 0, STRING("カンブー"))
    CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 2000)
    CALL(evt_npc_set_anim, STRING("カンブー"), STRING("C_2"))
    WAIT_MS(250)
    CALL(evt_npc_get_position, STRING("カンブー"), LW(2), LW(3), LW(4))
    CALL(evt_mario_get_pos, 0, LW(5), LW(6), LW(7))
    ADD_FLOAT(LW(5), FLOAT(60.0f))
    SUBTRACT_FLOAT(LW(2), FLOAT(10.0f))
    ADD_FLOAT(LW(3), FLOAT(45.0f))
    ADD_FLOAT(LW(4), FLOAT(5.0f))
    ADD_FLOAT(LW(6), FLOAT(50.0f))
    SET_FLOAT_VALUE(GW(0), LW(2))
    SET_FLOAT_VALUE(GW(1), LW(3))
    SET_FLOAT_VALUE(GW(2), LW(4))
    CALL(evt_snd_sfxon_3d, STRING("SFX_KUPPA_ITIKORO_APPEAR1"), LW(2), LW(3), LW(4), 0)
    WAIT_MS(100)
    CALL(evt_item_entry, STRING("itikoro"), ITEM_SUPERBOMBOMB, LW(2), LW(3), LW(4), ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
    SET_VALUE(LW(8), LW(3))
    ADD(LW(8), 12)
    SET_VALUE(LW(9), LW(4))
    ADD(LW(9), 5)
    CALL(evt_eff64, STRING(""), STRING("stardust2_n64"), 3, LW(2), LW(8), LW(9), 32, 32, 4, 12, 0, 0, 0, 0)
    CALL(evt_snd_sfxon_3d, STRING("SFX_KUPPA_ITIKORO_SHINE1"), LW(2), LW(8), LW(9), 0)
    WAIT_MS(1000)
    CALL(evt_npc_set_anim, STRING("カンブー"), STRING("S_1"))
    SET_VALUE(GF(0), 1)
    BEGIN_INLINE_EVENT
        SET_VALUE(LW(1), 30)
        SET_FLOAT_VALUE(LW(8), LW(7))
        SUBTRACT_FLOAT(LW(8), LW(4))
        DIVIDE_FLOAT(LW(8), LW(1))
        SET_VALUE(LW(0), 0)
        LOOP(0)
            IF_INT_EQUAL(GF(0), 0)
                WAIT_FRAMES(1)
                CONTINUE_LOOP
            END_IF
            IF_INT_EQUAL(LW(0), LW(1))
                BREAK_LOOP
            END_IF
            ADD_FLOAT(LW(4), LW(8))
            SET_FLOAT_VALUE(GW(2), LW(4))
            ADD(LW(0), 1)
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        SET_FLOAT_VALUE(LW(8), FLOAT(-4.1992188f))
        SET_FLOAT_VALUE(LW(9), FLOAT(4.0f))
        SET_FLOAT_VALUE(LW(10), FLOAT(0.0f))
        SET_FLOAT_VALUE(LW(11), FLOAT(-0.25976563f))
        SET_VALUE(LF(0), 0)
        LOOP(0)
            IF_INT_EQUAL(GF(0), 0)
                WAIT_FRAMES(1)
                CONTINUE_LOOP
            END_IF
            ADD_FLOAT(LW(2), LW(8))
            ADD_FLOAT(LW(3), LW(9))
            CALL(evt_item_set_position, STRING("itikoro"), LW(2), LW(3), GW(2))
            IF_INT_EQUAL(LF(0), 0)
                IF_INT_LESS_OR_EQUAL(LW(2), LW(5))
                    SET_VALUE(LF(0), 1)
                    MULTIPLY_FLOAT(LW(8), FLOAT(-0.44921875f))
                    SET_FLOAT_VALUE(LW(10), FLOAT(-0.049804688f))
                    SET_FLOAT_VALUE(LW(12), LW(2))
                    SUBTRACT_FLOAT(LW(12), FLOAT(5.0f))
                    SET_FLOAT_VALUE(LW(13), LW(3))
                    ADD_FLOAT(LW(13), FLOAT(15.0f))
                    CALL(small_star, LW(12), LW(13), GW(2))
                END_IF
            END_IF
            IF_INT_LESS_OR_EQUAL(LW(3), 0)
                SET_FLOAT_VALUE(LW(3), FLOAT(0.0f))
                MULTIPLY_FLOAT(LW(9), FLOAT(-0.5f))
                SET_FLOAT_VALUE(LW(12), LW(9))
                MULTIPLY_FLOAT(LW(12), FLOAT(10.0f))
                IF_INT_LESS_OR_EQUAL(LW(12), 2)
                    BREAK_LOOP
                    ELSE
                    CALL(evt_snd_sfxon_3d, STRING("SFX_KUPPA_ITIKORO_JUMP1"), LW(2), LW(3), GW(2), 0)
                END_IF
            END_IF
            ADD_FLOAT(LW(8), LW(10))
            ADD_FLOAT(LW(9), LW(11))
            IF_INT_EQUAL(LF(0), 1)
                SET_FLOAT_VALUE(LW(12), LW(8))
                MULTIPLY_FLOAT(LW(12), 10)
                IF_INT_LESS_OR_EQUAL(LW(12), 2)
                    SET_FLOAT_VALUE(LW(8), FLOAT(0.19921875f))
                END_IF
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_item_set_position, STRING("itikoro"), LW(2), 0, GW(2))
    END_INLINE_EVENT
    CALL(evt_item_get_position, STRING("itikoro"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_KUPPA_ITIKORO_FLY1"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(160)
    CALL(evt_cam3d_evt_set, -80, 172, 456, -80, 50, 78, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    SET_VALUE(GF(0), 0)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_KOOPA_SURPRISED2_2"), LW(0), LW(1), LW(2), 0)
    CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("KPA_S_5"), STRING("KPA_T_5"))
    CALL(evt_msg_print, 0, STRING("kpa_stg5_020"), 0, STRING("mario"))
    SET_VALUE(GF(0), 1)
    WAIT_MS(300)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    ADD(LW(0), 60)
    ADD(LW(1), 50)
    CALL(evt_snd_sfxon_3d, STRING("SFX_KUPPA_ITIKORO_HIT1"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(500)
    WAIT_MS(1000)
    CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("KPA_S_1"), STRING("KPA_T_1"))
    CALL(evt_msg_print, 0, STRING("kpa_stg5_021"), 0, STRING("mario"))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_KOOPA_ANGRY1_2"), LW(0), LW(1), LW(2), 0)
    CALL(evt_koopa_fire, 1000)
    SET_VALUE(GF(0), 0)
    BEGIN_INLINE_EVENT
        CALL(evt_item_get_position, STRING("itikoro"), LW(0), LW(1), LW(2))
        SET_VALUE(LW(3), LW(0))
        SUBTRACT(LW(3), 1)
        LOOP(0)
            IF_INT_NOT_EQUAL(GF(0), 0)
                BREAK_LOOP
            END_IF
            CALL(evt_item_set_position, STRING("itikoro"), LW(3), LW(1), LW(2))
            WAIT_FRAMES(2)
            ADD(LW(3), 2)
            IF_INT_NOT_EQUAL(GF(0), 0)
                BREAK_LOOP
            END_IF
            CALL(evt_item_set_position, STRING("itikoro"), LW(3), LW(1), LW(2))
            WAIT_FRAMES(2)
            SUBTRACT(LW(3), 2)
        END_LOOP
    END_INLINE_EVENT
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, -14, 225, 715, -4, 57, 120, 1000, EASING_SINE_IN_OUT)
    WAIT_MS(800)
    CALL(evt_msg_print, 0, STRING("kpa_stg5_022"), 0, STRING("カンブー"))
    WAIT_MS(100)
    CALL(evt_item_get_position, STRING("itikoro"), LW(0), LW(1), LW(2))
    ADD(LW(1), 10)
    CALL(evt_snd_sfxon_3d, STRING("SFX_KUPPA_ITIKORO_BIRIBIRI1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_eff64, STRING(""), STRING("akari_charge_n64"), 0, LW(0), LW(1), LW(2), FLOAT(0.5f), 48, 0, 0, 0, 0, 0, 0)
    WAIT_MS(800)
    CALL(evt_eff64, STRING("rad"), STRING("radiation_n64"), 2, LW(0), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_snd_sfxon_3d, STRING("SFX_KUPPA_ITIKORO_LIGHT1"), LW(0), LW(1), LW(2), LW(15))
    WAIT_MS(1000)
    CALL(kpa_chg_pose, STRING("KPA_O_1"))
    SPAWN_EVENT(PTR(bakuhatu_evt))
    WAIT_MS(350)
    BEGIN_INLINE_EVENT
        CALL(evt_sub_rumble_onoff, 0, 0)
        WAIT_MS(5000)
        CALL(evt_sub_rumble_onoff, 1, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(200)
        CALL(evt_eff_delete, STRING("rad"))
        CALL(evt_snd_sfxoff, LW(15))
        CALL(evt_item_get_position, STRING("itikoro"), LW(0), LW(1), LW(2))
        CALL(evt_item_delete, STRING("itikoro"))
        SET_VALUE(GF(0), 1)
        CALL(evt_snd_sfxon, STRING("SFX_KUPPA_ITIKORO_EXPLOSION1"), 0)
    END_INLINE_EVENT
    CALL(evt_fade_entry, FADE_OUT_BLACK, 600, 255, 255, 255)
    CALL(evt_fade_end_wait)
    WAIT_MS(3000)
    SET_VALUE(GSW_STORY_PROGRESS, STG5_49_BOWSER_TWILIGHT_TOWN_2)
    CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 1000)
    CALL(white_on)
    CALL(evt_cam_letter_box_onoff, 0, 1)
    CALL(evt_cam_letter_box_disable, 1)
    CALL(evt_fade_entry, 6, 300, 0, 0, 0)
    CALL(evt_fade_end_wait)
    WAIT_MS(2000)
    CALL(white_off)
    CALL(evt_cam_letter_box_disable, 0)
    SET_VALUE(LF(10), 0)
    SET_VALUE(LW(0), 15)
#if VERSION >= VERSION_US
    CALL_CHILD(evt_memcard_save_wrapper)
#else
    CALL_CHILD(PTR(evt_memcard_save))
#endif
    CALL(evt_mario_set_mode, 0)
    CALL(evt_fade_set_mapchange_type, 1, FADE_STORY_BOWSER_A1, 0, FADE_STORY_BOWSER_A3, 0)
    CALL(evt_bero_mapchange, STRING("muj_00"), STRING(""))
    RETURN
EVT_END()


static USER_FUNC(yami_view) {
    f32 z = evtGetFloat(event, event->args[0]);
    Mtx mtx;

    C_MTXLookAt(mtx, &(Vec){0.0f, 100.0f, 100.0f + z}, &(Vec){0.0f, 1.0f, 0.0f}, &(Vec){0.0f, 0.0f, z});
    envSetYamiView(mtx);
    return EVT_RETURN_DONE;
}

/**
 * Script helper: turns on TEV-callback material flag (args[1] & 0xF) on the NPC named by args[0];
 * ignores missing NPCs.
 */
static USER_FUNC(anim_tevcallback) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    s32 flag = evtGetValue(event, args[1]);
    NpcEntry* npc = npcNameToPtr_NoAssert(name);

    if (npc == NULL) {
        return EVT_RETURN_DONE;
    }
    animPoseSetMaterialFlagOn(npc->poseId, MAP_OBJ_TEV_CALLBACK(flag & 0xF));
    return EVT_RETURN_DONE;
}

/** Script helper: stops the regular (idle behavior) event of the NPC named by args[0] and clears its move event. */
static USER_FUNC(delete_npc_regl_evt) {
    NpcEntry* npc = npcNameToPtr_NoAssert((const char*)evtGetValue(event, event->args[0]));

    if (npc == NULL) {
        return EVT_RETURN_DONE;
    }
    if (npc->regularEvtId != 0) {
        evtDeleteID(npc->regularEvtId);
        npc->moveEvent = NULL;
        npc->regularEvtId = 0;
    } else if (npc->moveEvent != NULL) {
        npc->moveEvent = NULL;
    }
    return EVT_RETURN_DONE;
}


/** Script helper: spawns a Star Stone effect at (args[1], args[2], args[3]) and names it args[0]. */
static USER_FUNC(starstone_entry) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    f32 x = evtGetFloat(event, args[1]);
    f32 y = evtGetFloat(event, args[2]);
    f32 z = evtGetFloat(event, args[3]);

    effSetName(effStarStoneEntry(3, x, y, z, 1.0f), name);
    return EVT_RETURN_DONE;
}

/** Script helper: moves the Star Stone effect named args[0] to (args[1], args[2], args[3]). */
static USER_FUNC(starstone_set_pos) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    f32 x = evtGetFloat(event, args[1]);
    f32 y = evtGetFloat(event, args[2]);
    f32 z = evtGetFloat(event, args[3]);
    EffectEntry* eff = effNameToPtr(name);

    ((EffStarStoneWork*)eff->userdata)->position.x = x;
    ((EffStarStoneWork*)eff->userdata)->position.y = y;
    ((EffStarStoneWork*)eff->userdata)->position.z = z;
    return EVT_RETURN_DONE;
}

/** Script helper: sets the Y rotation of the Star Stone effect named args[0] to args[1]. */
static USER_FUNC(starstone_set_ry) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    f32 ry = evtGetFloat(event, args[1]);

    ((EffStarStoneWork*)effNameToPtr(name)->userdata)->rot.y = ry;
    return EVT_RETURN_DONE;
}

/**
 * Stops the regular events of villagers A to E for sekai_yami_evt, which then runs sekai_yami_npc on each.
 */
static EVT_BEGIN(sekai_yami_npc_init)
    CALL(delete_npc_regl_evt, STRING("村人Ａ"))
    CALL(delete_npc_regl_evt, STRING("村人Ｂ"))
    CALL(delete_npc_regl_evt, STRING("村人Ｃ"))
    CALL(delete_npc_regl_evt, STRING("村人Ｄ"))
    CALL(delete_npc_regl_evt, STRING("村人Ｅ"))
    RETURN
EVT_END()

/**
 * Panic loop of one villager (name in LW(0)) in sekai_yami_evt:
 * every 100 to 500 ms it turns around and waits for the paper turn to finish.
 */
static EVT_BEGIN(sekai_yami_npc)
    LOOP(0)
        CALL(evt_sub_random, 400, LW(1))
        ADD(LW(1), 100)
        WAIT_MS(LW(1))
        CALL(evt_npc_reverse_ry, LW(0))
        WAIT_FRAMES(1)
        CALL(evt_npc_wait_pera, LW(0))
    END_LOOP
    RETURN
EVT_END()

/**
 * Endless loop that shakes the 3D camera slightly (0.0098 in y, 500 ms per shake);
 * spawned by sekai_yami_evt with the quake sound.
 */
static EVT_BEGIN(sekai_yami_shake)
    LOOP(0)
        CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0f), FLOAT(0.009765625f), 500)
    END_LOOP
    RETURN
EVT_END()

/**
 * Final-chapter scene ("sekai yami", the world goes dark)
 * when the map is entered through sekai_yami2 at STORY_STG8_15_GRODUS:
 * with Mario and the partner off stage the town is drawn in darkness (envSetYami),
 * the light flickers with SFX_STG8_LIGHT1 (once from US on, twice before),
 * villagers A to E turn about in panic (sekai_yami_npc), crows C, D and E fly off,
 * a quake rumbles with the camera shaking, a villager screams (stg8_dark_05)
 * and the game moves on to muj_01 (entrance sekai_yami2).
 */
#if VERSION <= VERSION_DEMO_US
/**
 * Final-chapter scene ("sekai yami", the world goes dark)
 * when the map is entered through sekai_yami2 at STG8_15_GRODUS:
 * with Mario and the partner off stage the town is drawn in darkness (envSetYami),
 * the light flickers with SFX_STG8_LIGHT1 (once from US on, twice before),
 * villagers A to E turn about in panic (sekai_yami_npc), crows C, D and E fly off,
 * a quake rumbles with the camera shaking, a villager screams (stg8_dark_05)
 * and the game moves on to muj_01 (entrance sekai_yami2).
 */
static EVT_BEGIN(sekai_yami_evt)
    // The town is drawn in darkness and the light flickers.
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_mario_set_pos, 0, -1000, 0)
    CALL(evt_party_set_pos, 0, 0, -1000, 0)
    CALL(evt_map_set_tevcallback, 1, PTR(envSetYami))
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_usu00"), MAP_OBJ_TEV_CALLBACK(1))
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_usu00"), MAP_OBJ_FLAG_DISPLAY_LIST)
    CALL(anim_tevcallback, STRING("村人Ａ"), 1)
    CALL(anim_tevcallback, STRING("村人Ｂ"), 1)
    CALL(anim_tevcallback, STRING("村人Ｃ"), 1)
    CALL(anim_tevcallback, STRING("村人Ｄ"), 1)
    CALL(anim_tevcallback, STRING("村人Ｅ"), 1)
    CALL(anim_tevcallback, STRING("村人Ｋ"), 1)
    CALL(anim_tevcallback, STRING("カラスＣ"), 1)
    CALL(anim_tevcallback, STRING("カラスＤ"), 1)
    CALL(anim_tevcallback, STRING("カラスＥ"), 1)
    CALL(evt_bg_set_color, 64, 64, 64, 255)
    CALL(yami_view, -800)
    CALL(evt_cam3d_evt_set, 0, 80, 687, 0, 70, -13, 0, EASING_SINE_IN_OUT)
    CALL(evt_seq_wait, SEQ_GAME)
    WAIT_MS(1000)
LOOP(2)
CALL(evt_snd_sfxon, STRING("SFX_STG8_LIGHT1"), 0)
        CALL(evt_fade_entry, FADE_OUT_BLACK, 0, 255, 255, 255)
        WAIT_MS(100)
        CALL(evt_fade_entry, FADE_IN_BLACK, 0, 255, 255, 255)
WAIT_MS(100)
END_LOOP
    // The villagers turn about in panic and the crows fly off.
    SET_VALUE(LW(0), STRING("村人Ａ"))
    SPAWN_EVENT(sekai_yami_npc)
    SET_VALUE(LW(0), STRING("村人Ｂ"))
    SPAWN_EVENT(sekai_yami_npc)
    SET_VALUE(LW(0), STRING("村人Ｃ"))
    SPAWN_EVENT(sekai_yami_npc)
    SET_VALUE(LW(0), STRING("村人Ｄ"))
    SPAWN_EVENT(sekai_yami_npc)
    SET_VALUE(LW(0), STRING("村人Ｅ"))
    SPAWN_EVENT(sekai_yami_npc)
    BEGIN_INLINE_EVENT
        WAIT_MS(100)
        CALL(evt_npc_status_onoff, 1, STRING("カラスＣ"), NPC_STATUS_FACE_MOVE_DIR)
        CALL(evt_npc_get_position, STRING("カラスＣ"), LW(0), LW(1), LW(2))
        CALL(evt_npc_set_ry, STRING("カラスＣ"), 270)
        CALL(evt_npc_jump_position_nohit, STRING("カラスＣ"), -10, 250, LW(2), 1500, 30)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(100)
        CALL(evt_npc_status_onoff, 1, STRING("カラスＤ"), NPC_STATUS_FACE_MOVE_DIR)
        CALL(evt_npc_set_ry, STRING("カラスＤ"), 90)
        CALL(evt_npc_get_position, STRING("カラスＤ"), LW(0), LW(1), LW(2))
        CALL(evt_npc_jump_position_nohit, STRING("カラスＤ"), 100, 250, LW(2), 1500, 30)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(100)
        CALL(evt_npc_status_onoff, 1, STRING("カラスＥ"), NPC_STATUS_FACE_MOVE_DIR)
        CALL(evt_npc_set_ry, STRING("カラスＥ"), 90)
        CALL(evt_npc_get_position, STRING("カラスＥ"), LW(0), LW(1), LW(2))
        CALL(evt_npc_jump_position_nohit, STRING("カラスＥ"), 150, 250, LW(2), 1500, 30)
    END_INLINE_EVENT
    // The ground quakes and a villager cries out, then the game moves on to muj_01.
    CALL(evt_snd_sfxon, STRING("SFX_STG8_QUAKE1"), LW(0))
    SPAWN_EVENT(sekai_yami_shake)
    WAIT_MS(2000)
    CALL(evt_msg_print, 0, STRING("stg8_dark_05"), 0, 0)
WAIT_MS(3000)
CALL(evt_snd_sfxoff, LW(0))
    CALL(evt_fade_set_mapchange_type, 1, FADE_OUT_BLACK, 300, FADE_IN_BLACK, 300)
    CALL(evt_bero_mapchange, STRING("muj_01"), STRING("sekai_yami2"))
    RETURN
EVT_END()
#else
/**
 * Final-chapter scene ("sekai yami", the world goes dark)
 * when the map is entered through sekai_yami2 at STG8_15_GRODUS:
 * with Mario and the partner off stage the town is drawn in darkness (envSetYami),
 * the light flickers with SFX_STG8_LIGHT1 (once from US on, twice before),
 * villagers A to E turn about in panic (sekai_yami_npc), crows C, D and E fly off,
 * a quake rumbles with the camera shaking, a villager screams (stg8_dark_05)
 * and the game moves on to muj_01 (entrance sekai_yami2).
 */
static EVT_BEGIN(sekai_yami_evt)
    // The town is drawn in darkness and the light flickers.
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_mario_set_pos, 0, -1000, 0)
    CALL(evt_party_set_pos, 0, 0, -1000, 0)
    CALL(evt_map_set_tevcallback, 1, PTR(envSetYami))
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_usu00"), MAP_OBJ_TEV_CALLBACK(1))
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_usu00"), MAP_OBJ_FLAG_DISPLAY_LIST)
    CALL(anim_tevcallback, STRING("村人Ａ"), 1)
    CALL(anim_tevcallback, STRING("村人Ｂ"), 1)
    CALL(anim_tevcallback, STRING("村人Ｃ"), 1)
    CALL(anim_tevcallback, STRING("村人Ｄ"), 1)
    CALL(anim_tevcallback, STRING("村人Ｅ"), 1)
    CALL(anim_tevcallback, STRING("村人Ｋ"), 1)
    CALL(anim_tevcallback, STRING("カラスＣ"), 1)
    CALL(anim_tevcallback, STRING("カラスＤ"), 1)
    CALL(anim_tevcallback, STRING("カラスＥ"), 1)
    CALL(evt_bg_set_color, 64, 64, 64, 255)
    CALL(yami_view, -800)
    CALL(evt_cam3d_evt_set, 0, 80, 687, 0, 70, -13, 0, EASING_SINE_IN_OUT)
    CALL(evt_seq_wait, SEQ_GAME)
    WAIT_MS(1000)
#if VERSION >= VERSION_US
    LOOP(1)
#else
    LOOP(2)
#endif
        CALL(evt_snd_sfxon, STRING("SFX_STG8_LIGHT1"), 0)
        CALL(evt_fade_entry, FADE_OUT_BLACK, 0, 255, 255, 255)
        WAIT_MS(100)
        CALL(evt_fade_entry, FADE_IN_BLACK, 0, 255, 255, 255)
#if VERSION >= VERSION_US
        WAIT_MS(500)
#else
        WAIT_MS(100)
#endif
    END_LOOP
    // The villagers turn about in panic and the crows fly off.
    SET_VALUE(LW(0), STRING("村人Ａ"))
    SPAWN_EVENT(PTR(sekai_yami_npc))
    SET_VALUE(LW(0), STRING("村人Ｂ"))
    SPAWN_EVENT(PTR(sekai_yami_npc))
    SET_VALUE(LW(0), STRING("村人Ｃ"))
    SPAWN_EVENT(PTR(sekai_yami_npc))
    SET_VALUE(LW(0), STRING("村人Ｄ"))
    SPAWN_EVENT(PTR(sekai_yami_npc))
    SET_VALUE(LW(0), STRING("村人Ｅ"))
    SPAWN_EVENT(PTR(sekai_yami_npc))
    BEGIN_INLINE_EVENT
        WAIT_MS(100)
        CALL(evt_npc_status_onoff, 1, STRING("カラスＣ"), NPC_STATUS_FACE_MOVE_DIR)
        CALL(evt_npc_get_position, STRING("カラスＣ"), LW(0), LW(1), LW(2))
        CALL(evt_npc_set_ry, STRING("カラスＣ"), 270)
        CALL(evt_npc_jump_position_nohit, STRING("カラスＣ"), -10, 250, LW(2), 1500, 30)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(100)
        CALL(evt_npc_status_onoff, 1, STRING("カラスＤ"), NPC_STATUS_FACE_MOVE_DIR)
        CALL(evt_npc_set_ry, STRING("カラスＤ"), 90)
        CALL(evt_npc_get_position, STRING("カラスＤ"), LW(0), LW(1), LW(2))
        CALL(evt_npc_jump_position_nohit, STRING("カラスＤ"), 100, 250, LW(2), 1500, 30)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(100)
        CALL(evt_npc_status_onoff, 1, STRING("カラスＥ"), NPC_STATUS_FACE_MOVE_DIR)
        CALL(evt_npc_set_ry, STRING("カラスＥ"), 90)
        CALL(evt_npc_get_position, STRING("カラスＥ"), LW(0), LW(1), LW(2))
        CALL(evt_npc_jump_position_nohit, STRING("カラスＥ"), 150, 250, LW(2), 1500, 30)
    END_INLINE_EVENT
    // The ground quakes and a villager cries out, then the game moves on to muj_01.
    CALL(evt_snd_sfxon, STRING("SFX_STG8_QUAKE1"), LW(0))
    SPAWN_EVENT(PTR(sekai_yami_shake))
    WAIT_MS(2000)
    CALL(evt_msg_print, 0, STRING("stg8_dark_05"), 0, 0)
    WAIT_MS(1500)
    CALL(evt_snd_sfxoff, LW(0))
    CALL(evt_fade_set_mapchange_type, 1, FADE_OUT_BLACK, 300, FADE_IN_BLACK, 300)
    CALL(evt_bero_mapchange, STRING("muj_01"), STRING("sekai_yami2"))
    RETURN
EVT_END()
#endif

/**
 * Spawns a woman (おばさん, tribe ウスグラおばさん) and three children (子供１ to ３, tribes ウスグラ子供A to C) of Twilight Town for
 * minnnanokoe_evt ("minna no koe", everyone's voices).
 */
static EVT_BEGIN(minnnanokoe_npc_entry)
    CALL(evt_npc_entry, STRING("おばさん"), STRING("c_f_mama"))
    CALL(evt_npc_set_tribe, STRING("おばさん"), STRING("ウスグラおばさん"))
    CALL(evt_npc_set_position, STRING("おばさん"), -120, 0, -90)
    CALL(evt_npc_set_ry, STRING("おばさん"), 90)
    CALL(evt_npc_flag_onoff, 1, STRING("おばさん"), NPC_FLAG_TALK_POSE)
    CALL(evt_npc_entry, STRING("子供１"), STRING("c_f_child_a"))
    CALL(evt_npc_set_tribe, STRING("子供１"), STRING("ウスグラ子供A"))
    CALL(evt_npc_set_position, STRING("子供１"), -80, 0, -25)
    CALL(evt_npc_set_ry, STRING("子供１"), 90)
    CALL(evt_npc_flag_onoff, 1, STRING("子供１"), NPC_FLAG_TALK_POSE)
    CALL(evt_npc_entry, STRING("子供２"), STRING("c_f_child_b"))
    CALL(evt_npc_set_tribe, STRING("子供２"), STRING("ウスグラ子供B"))
    CALL(evt_npc_set_position, STRING("子供２"), -50, 0, 0)
    CALL(evt_npc_set_ry, STRING("子供２"), 270)
    CALL(evt_npc_flag_onoff, 1, STRING("子供２"), NPC_FLAG_TALK_POSE)
    CALL(evt_npc_entry, STRING("子供３"), STRING("c_f_child_c"))
    CALL(evt_npc_set_tribe, STRING("子供３"), STRING("ウスグラ子供C"))
    CALL(evt_npc_set_position, STRING("子供３"), -100, 0, 10)
    CALL(evt_npc_set_ry, STRING("子供３"), 90)
    CALL(evt_npc_flag_onoff, 1, STRING("子供３"), NPC_FLAG_TALK_POSE)
    RETURN
EVT_END()

/**
 * Final-battle scene ("minna no koe", everyone's voices)
 * when the map is entered through minnnanokoe at STORY_STG8_15_GRODUS:
 * with Mario off stage and the town in darkness,
 * a Crystal Star effect hovers and spins over the square
 * while the three children cheer Mario on (stg8_las_148_26 to 31), villager A,
 * the woman (worried about Vivian) and the mayor speak (32 to 34), the children cheer again (35 to 37),
 * and the game moves on to muj_01 (entrance minnnanokoe).
 */
static EVT_BEGIN(minnnanokoe_evt)
    // Set up the dark town with the townsfolk and a spinning Crystal Star effect over the square.
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_mario_set_pos, 0, -1000, 0)
    CALL(evt_party_set_pos, 0, 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("村長"), -60, 0, -75)
    CALL(evt_npc_set_ry, STRING("村長"), 270)
    CALL(evt_npc_set_position, STRING("村人Ａ"), 0, 0, -100)
    CALL(evt_npc_set_ry, STRING("村人Ａ"), 270)
    CALL(evt_npc_set_position, STRING("村人Ｃ"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("村人Ｄ"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("村人Ｅ"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("村人Ｋ"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("カラスＣ"), 1000, -1000, 0)
    CALL(evt_npc_set_position, STRING("カラスＤ"), 1000, -1000, 0)
    CALL(evt_npc_set_position, STRING("カラスＥ"), 1000, -1000, 0)
    CALL(evt_map_set_tevcallback, 1, PTR(envSetYami))
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_usu00"), MAP_OBJ_TEV_CALLBACK(1))
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_usu00"), MAP_OBJ_FLAG_DISPLAY_LIST)
    CALL(anim_tevcallback, STRING("おばさん"), 1)
    CALL(anim_tevcallback, STRING("子供１"), 1)
    CALL(anim_tevcallback, STRING("子供２"), 1)
    CALL(anim_tevcallback, STRING("子供３"), 1)
    CALL(anim_tevcallback, STRING("村人Ａ"), 1)
    CALL(anim_tevcallback, STRING("村長"), 1)
    CALL(yami_view, -800)
    CALL(evt_bg_set_color, 64, 64, 64, 255)
    CALL(evt_cam3d_evt_set, -350, 90, 737, -76, 70, 0, 0, EASING_SINE_IN_OUT)
    CALL(starstone_entry, STRING("starstone"), -75, 60, 0)
    SET_VALUE(GW(3), 60)
    BEGIN_INLINE_EVENT
        LOOP(0)
            CALL(starstone_set_pos, STRING("starstone"), -75, GW(3), 0)
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        SET_FLOAT_VALUE(LW(0), FLOAT(0.0f))
        LOOP(0)
            CALL(starstone_set_ry, STRING("starstone"), LW(0))
            SUBTRACT_FLOAT(LW(0), FLOAT(3.0f))
            IF_INT_LESS_OR_EQUAL(LW(0), FLOAT(0.0f))
                ADD_FLOAT(LW(0), FLOAT(360.0f))
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        LOOP(0)
            CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 6000, 6500, 500)
            LABEL(1)
            CALL(evt_sub_intpl_msec_get_value)
            DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
            SET_FLOAT_VALUE(GW(3), LW(0))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 1)
                GOTO(1)
            END_IF
            CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 6500, 6000, 500)
            LABEL(2)
            CALL(evt_sub_intpl_msec_get_value)
            DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
            SET_FLOAT_VALUE(GW(3), LW(0))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 1)
                GOTO(2)
            END_IF
        END_LOOP
    END_INLINE_EVENT
    // The children cheer Mario on.
    CALL(evt_seq_wait, SEQ_GAME)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg8_las_148_26"), 0, STRING("子供１"))
    CALL(evt_msg_print, 0, STRING("stg8_las_148_27"), 0, STRING("子供２"))
    CALL(evt_msg_print, 0, STRING("stg8_las_148_28"), 0, STRING("子供３"))
    CALL(evt_msg_print, 0, STRING("stg8_las_148_29"), 0, STRING("子供１"))
    CALL(evt_msg_print, 0, STRING("stg8_las_148_30"), 0, STRING("子供２"))
    CALL(evt_msg_print, 0, STRING("stg8_las_148_31"), 0, STRING("子供３"))
    // Villager A, the woman and the Mayor speak, then the children cheer again and the game moves on to muj_01.
    CALL(evt_npc_flag_onoff, 1, STRING("子供１"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_flag_onoff, 1, STRING("子供２"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_flag_onoff, 1, STRING("子供３"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_flag_onoff, 1, STRING("おばさん"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_cam3d_evt_set, -75, 92, 313, -75, 52, -35, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg8_las_148_32"), 0, STRING("村人Ａ"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg8_las_148_33"), 0, STRING("おばさん"))
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg8_las_148_34"), 0, STRING("村長"))
    CALL(evt_cam3d_evt_set, -185, 70, 858, -91, 111, -36, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_msg_print, 0, STRING("stg8_las_148_35"), 0, STRING("子供１"))
    CALL(evt_msg_print, 0, STRING("stg8_las_148_36"), 0, STRING("子供２"))
    CALL(evt_msg_print, 0, STRING("stg8_las_148_37"), 0, STRING("子供３"))
    CALL(evt_fade_set_mapchange_type, 1, FADE_OUT_BLACK, 300, FADE_IN_BLACK, 300)
    CALL(evt_bero_mapchange, STRING("muj_01"), STRING("minnnanokoe"))
    RETURN
EVT_END()

/** Normal (1, 1, 1) scale the partner is reset to by party_scale before the after-Peach scene. */
__declspec(section ".rodata") Vec party_scale_vec = {1.0f, 1.0f, 1.0f};

/** Script helper: sets Mario's partner scale to party_scale_vec. */
USER_FUNC(party_scale) {
    partyGetPtr(0)->scale = party_scale_vec;
    return EVT_RETURN_DONE;
}

/**
 * Cutscene in Twilight Town after the Peach and Bowser interludes (spawned at
 * STG4_15_08_BOWSER_ROGUEPORT_DOCK): the mayor apologizes for being fooled by the fake Mario
 * and asks whether they hurt his feelings (stg4_usu_153 with a choice);
 * "Nah, not really" draws praise (154), "Maybe a little" an awkward reply (155).
 * The partner suggests heading back to Rogueport (stg4_usu_156), the mayor invites him back anytime (157),
 * and the story becomes STG4_16_LEAVING_TWILIGHT_TOWN.
 */
static EVT_BEGIN(evt_after_peach)
    CALL(evt_mario_key_onoff, 0)
    CALL(party_scale)
    CALL(evt_mario_set_pos, -90, 0, -50)
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_party_set_pos, 0, -130, 0, -60)
    CALL(evt_party_set_dir, 0, 90, 0)
    CALL(evt_party_run, 0)
    CALL(evt_npc_set_position, STRING("村長"), -30, 0, -50)
    CALL(evt_npc_set_ry, STRING("村長"), 270)
    CALL(evt_cam3d_evt_set, -75, 105, 275, -75, 45, -67, 0, EASING_SINE_IN_OUT)
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_npc_get_reglid, STRING("村人Ｃ"), LW(10))
    DELETE_EVENT(LW(10))
    CALL(evt_npc_set_position, STRING("村人Ｃ"), 85, 0, 10)
    CALL(evt_fade_end_wait)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg4_usu_153"), 0, STRING("村長"))
    CALL(evt_msg_select, 0, STRING("stg4_usu_153_select"))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_msg_print_add, 0, STRING("stg4_usu_154"))
        ELSE
        CALL(evt_msg_print_add, 0, STRING("stg4_usu_155"))
    END_IF
    CALL(evt_cam3d_evt_set, -110, 105, 275, -110, 45, -67, 1000, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    CALL(evt_party_stop, 0)
    WAIT_MS(300)
    CALL(evt_msg_print_party, STRING("stg4_usu_156"))
    CALL(evt_party_run, 0)
    CALL(evt_msg_print, 0, STRING("stg4_usu_157"), 0, STRING("村長"))
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
#if VERSION <= VERSION_DEMO_US
    SET_VALUE(GSW_STORY_PROGRESS, STG4_16_LEAVING_TWILIGHT_TOWN)
#else
    SET_VALUE(GSW_STORY_PROGRESS, STG4_16_LEAVING_TWILIGHT_TOWN)
    CALL(evt_bero_set_mario_bottomless_respawn_pos_to_current_mario_pos)
#endif
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_npc_restart_regular_event, STRING("村人Ｃ"))
    CALL(evt_npc_flag_onoff, 1, STRING("村人Ｃ"), NPC_FLAG_STOP_FOR_EVENT)
    CALL(evt_npc_stop_for_event)
    CALL(evt_npc_start_for_event)
    CALL(evt_npc_flag_onoff, 0, STRING("村人Ｃ"), NPC_FLAG_STOP_FOR_EVENT)
    RETURN
EVT_END()

static USER_FUNC(npc_process) {
    NpcWork* wp = npcGetWorkPtr();
    NpcEntry* npc = wp->entries;
    s32 i;

    for (i = 0; i < wp->total; i++, npc++) {
        if (npc->flags & NPC_FLAG_IN_USE) {
            if (npc->position.y <= -1000.0f) {
                npc->flags |= NPC_FLAG_HIDE;
            } else {
                npc->flags &= ~NPC_FLAG_HIDE;
            }
        }
    }
    return EVT_RETURN_BLOCK;
}

/**
 * Spawns Goombella (クリスチーヌ) beside Vivian, and Beldam and Marilyn off stage,
 * for the ending epilogue in Twilight Town.
 */
static EVT_BEGIN(epilogue_npc_entry)
    CALL(evt_npc_entry, STRING("クリスチーヌ"), STRING("c_pkr"))
    CALL(evt_npc_set_tribe, STRING("クリスチーヌ"), STRING("クリスチーヌ"))
    CALL(evt_npc_set_position, STRING("クリスチーヌ"), -25, 0, 100)
    CALL(evt_npc_set_ry, STRING("クリスチーヌ"), 90)
    CALL(evt_npc_entry, STRING("マジョリン"), STRING("c_majyorin"))
    CALL(evt_npc_set_tribe, STRING("マジョリン"), STRING("マジョリン"))
    CALL(evt_npc_set_position, STRING("マジョリン"), 0, -1000, 0)
    CALL(evt_npc_entry, STRING("マリリン"), STRING("c_maririn"))
    CALL(evt_npc_set_tribe, STRING("マリリン"), STRING("マリリン"))
    CALL(evt_npc_set_position, STRING("マリリン"), 0, -1000, 0)
    RETURN
EVT_END()

/**
 * Animation script of the ending epilogue, spawned by epilogue_evt: Vivian and Goombella chat;
 * Beldam and Marilyn appear beside them (evt_npc_majo_disp_on), Vivian turns to them and the three talk;
 * the sisters vanish again, Vivian turns back and she and Goombella keep chatting until the scene ends.
 * GF(0) is set while Marilyn's appear or vanish effect runs.
 */
static EVT_BEGIN(epilogue_evt_sub)
    CALL(evt_npc_set_anim, STRING("ビビアン"), STRING("PTR_T_1"))
    WAIT_MS(1500)
    CALL(evt_npc_set_anim, STRING("ビビアン"), STRING("PTR_S_1"))
    CALL(evt_npc_set_anim, STRING("クリスチーヌ"), STRING("PKR_T_1"))
    WAIT_MS(1500)
    CALL(evt_npc_set_anim, STRING("クリスチーヌ"), STRING("PKR_S_1"))
    CALL(evt_npc_set_anim, STRING("ビビアン"), STRING("PTR_A4_1"))
    WAIT_MS(1500)
    CALL(evt_npc_set_anim, STRING("ビビアン"), STRING("PTR_S_1"))
    SET_VALUE(GF(0), 1)
    BEGIN_INLINE_EVENT
        WAIT_MS(400)
        CALL(evt_npc_majo_disp_on, STRING("マリリン"), 60, 0, 80, 0)
        SET_VALUE(GF(0), 0)
    END_INLINE_EVENT
    CALL(evt_npc_majo_disp_on, STRING("マジョリン"), 75, 0, 120, 0)
    WAIT_WHILE(GF(0))
    WAIT_MS(300)
    CALL(evt_npc_set_ry, STRING("ビビアン"), 90)
    WAIT_FRAMES(1)
    CALL(evt_npc_wait_pera, STRING("ビビアン"))
    WAIT_MS(300)
    CALL(evt_npc_set_anim, STRING("マジョリン"), STRING("MJR_T_1"))
    WAIT_MS(1500)
    CALL(evt_npc_set_anim, STRING("マジョリン"), STRING("MJR_S_1"))
    CALL(evt_npc_set_anim, STRING("マリリン"), STRING("MRR_T_1"))
    WAIT_MS(1000)
    CALL(evt_npc_set_anim, STRING("マリリン"), STRING("MRR_S_1"))
    CALL(evt_npc_set_anim, STRING("ビビアン"), STRING("PTR_T_1"))
    WAIT_MS(1000)
    CALL(evt_npc_set_anim, STRING("ビビアン"), STRING("PTR_S_1"))
    CALL(evt_npc_set_anim, STRING("マジョリン"), STRING("MJR_T_1"))
    WAIT_MS(1000)
    CALL(evt_npc_set_anim, STRING("マジョリン"), STRING("MJR_S_1"))
    WAIT_MS(500)
    SET_VALUE(GF(0), 1)
    BEGIN_INLINE_EVENT
        WAIT_MS(400)
        CALL(evt_npc_majo_disp_off, STRING("マリリン"))
        SET_VALUE(GF(0), 0)
    END_INLINE_EVENT
    CALL(evt_npc_majo_disp_off, STRING("マジョリン"))
    WAIT_WHILE(GF(0))
    WAIT_MS(500)
    CALL(evt_npc_set_ry, STRING("ビビアン"), 270)
    WAIT_FRAMES(1)
    CALL(evt_npc_wait_pera, STRING("ビビアン"))
    WAIT_MS(500)
    LOOP(0)
        CALL(evt_npc_set_anim, STRING("クリスチーヌ"), STRING("PKR_T_1"))
        WAIT_MS(1500)
        CALL(evt_npc_set_anim, STRING("クリスチーヌ"), STRING("PKR_S_1"))
        CALL(evt_npc_set_anim, STRING("ビビアン"), STRING("PTR_T_1"))
        WAIT_MS(1500)
        CALL(evt_npc_set_anim, STRING("ビビアン"), STRING("PTR_S_1"))
    END_LOOP
    RETURN
EVT_END()

/**
 * Ending epilogue in Twilight Town (STG8_END_AFTER_THE_BATTLE):
 * with the town theme playing and Mario hidden,
 * Vivian and Goombella stand together while epilogue_evt_sub plays,
 * the epilogue text ep_41 says Vivian has gone back to her sisters
 * and Beldam promised never to be mean to her again, and the game fades to muj_20.
 */
static EVT_BEGIN(epilogue_evt)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG4_USU1"))
    CALL(evt_snd_bgm_scope, 0, 1)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_HIDE)
    CALL(evt_npc_set_position, STRING("ビビアン"), 25, 0, 100)
    CALL(evt_npc_set_ry, STRING("ビビアン"), 270)
    CALL(evt_cam3d_evt_set, 0, 109, 495, 0, 50, 100, 0, EASING_SINE_IN_OUT)
    CALL(evt_seq_wait, SEQ_GAME)
    SPAWN_EVENT(PTR(epilogue_evt_sub))
    CALL(evt_fade_end_wait)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("ep_41"), 0, 0)
    WAIT_MS(1000)
    CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 3000)
    CALL(evt_snd_envoff_f, PSND_FADEOUT, 3000)
    CALL(evt_fade_set_mapchange_type, 1, FADE_OUT_BLACK, 900, FADE_IN_BLACK, 900)
    CALL(evt_bero_mapchange, STRING("muj_20"), STRING(""))
    RETURN
EVT_END()

/**
 * Map init script of Twilight Town's main street:
 * picks BGM by story progress, registers the pipe, doors and bero exits, spawns NPCs and items,
 * and starts Chapter 4 and Bowser interlude cutscenes.
 */
#if VERSION <= VERSION_DEMO_US
/**
 * Map init script of Twilight Town's main street:
 * picks BGM by story progress, registers the pipe, doors and bero exits, spawns NPCs and items,
 * and starts Chapter 4 and Bowser interlude cutscenes.
 */
EVT_BEGIN(usu_00_init_evt)
    // In Bowser's interlude (from Peach's talk with TEC to Bowser's town visit) Mario is Bowser; GF(1) marks it.
    SET_VALUE(GF(1), 0)
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG5_47_PEACH_TEC_TALK)
        IF_INT_LESS_OR_EQUAL(GSW_STORY_PROGRESS, STG5_48_BOWSER_TWILIGHT_TOWN)
            CALL(evt_mario_set_mode, 2)
            SET_VALUE(GF(1), 1)
        END_IF
    END_IF
    // Music: the Shadow Sirens' theme while Mario has lost his name, chapter 8 voices or silence in the darkness
    // scenes, otherwise the town theme.
    IF_INT_EQUAL(GF(1), 0)
        SWITCH(GSW_STORY_PROGRESS)
            CASE_BETWEEN(STG4_10_02_TRADING_PLACES, STG4_11_SUPERBOMBBOMB_LOST)
            CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_WITCH1"))
            CALL(evt_snd_envoff, PSND_FADEOUT)
            CASE_EQUAL(STG4_11_02_WAITING_DOOPLISS)
            CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_WITCH1"))
            CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG4_USU1"))
            CASE_BETWEEN(STG4_12_VIVIAN, STG4_12_01_SUPERBOMBBOMB_FOUND)
            CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_WITCH1"))
            CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG4_USU1"))
            CASE_EQUAL(STG8_15_GRODUS)
            SET_VALUE(LF(1), 0)
            CALL(evt_bero_get_entername, LW(0))
            IF_STR_EQUAL(LW(0), STRING("sekai_yami2"))
                CALL(evt_snd_bgmoff, PSND_FADEOUT)
                CALL(evt_snd_envoff, PSND_FADEOUT)
                SET_VALUE(LF(1), 1)
            END_IF
            IF_STR_EQUAL(LW(0), STRING("minnnanokoe"))
                CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_STG8_VOICE1"))
                CALL(evt_snd_envoff, PSND_FADEOUT)
                SET_VALUE(LF(1), 1)
            END_IF
            IF_INT_EQUAL(LF(1), 0)
                CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG4_USU1"))
                CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG4_USU1"))
            END_IF
            CASE_DEFAULT
            IF_INT_NOT_EQUAL(GSW_STORY_PROGRESS, STG8_END_AFTER_THE_BATTLE)
                CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG4_USU1"))
            END_IF
            CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG4_USU1"))
        END_SWITCH
    ELSE
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG4_USU1"))
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG4_USU1"))
    END_IF
    IF_INT_EQUAL(GF(1), 0)
        CALL(evt_kagemario_init)
    END_IF
    // Register the exits; in Bowser's visit the pipe becomes his scene and the doors, bushes and exits are shut.
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(evt_bero_info_run)
    IF_INT_EQUAL(GF(1), 1)
        SET_VALUE(LW(0), STRING("dokan_1"))
        CALL_CHILD(bero_case_switch_off)
        CALL(evt_bero_overwrite, STRING("dokan_1"), PTR(bero_koopa_dokan))
        SET_VALUE(LW(0), STRING("dokan_1"))
        CALL_CHILD(bero_case_switch_on)
        SET_VALUE(LW(0), 1)
        CALL_CHILD(bero_case_switch_off)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("bero"), MAP_OBJ_FLAG_HIDDEN)
END_IF
    // Save block, Star Piece, NPCs, house doors (Bowser gets locked-out scripts) and the inn.
    IF_INT_EQUAL(GF(1), 0)
        CALL(evt_mobj_save_blk, STRING("mobj_save"), 365, 60, -50, 0, 0)
    END_IF
    IF_INT_EQUAL(GF(1), 0)
        CALL(evt_item_entry, STRING("item00"), ITEM_STAR_PIECE, -220, 0, -225, ITEM_TYPE_STILL, GSWF_STARPIECE_62, 0)
    END_IF
    CALL_CHILD(evt_usu_kagemario_party_kill)
    CALL(evt_npc_setup, PTR(npcEnt))
    IF_INT_EQUAL(GF(1), 1)
        CALL_CHILD(koopa_npc_entry)
    END_IF
    IF_INT_EQUAL(GF(1), 0)
        SET_VALUE(LW(0), PTR(&door_1))
        CALL_CHILD(evt_door_entry)
        SET_VALUE(LW(0), PTR(&door_2))
        CALL_CHILD(evt_door_entry)
        SET_VALUE(LW(0), PTR(&door_3))
        CALL_CHILD(evt_door_entry)
    ELSE
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_ie1door"), 0, PTR(door_ie1_koopa_evt), 0)
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_ie2door"), 0, PTR(door_ie2_koopa_evt), 0)
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_ie3door"), 0, PTR(door_ie3_koopa_evt), 0)
    END_IF
    IF_INT_EQUAL(GF(1), 0)
        CALL(evt_kinopio_setup, PTR(&kino_dt))
    END_IF
    // Trees and bushes.
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("ki1"), MAP_OBJ_FLAG_CLIP)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("ki2"), MAP_OBJ_FLAG_CLIP)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("ki3"), MAP_OBJ_FLAG_CLIP)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("ki4"), MAP_OBJ_FLAG_CLIP)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("ki5"), MAP_OBJ_FLAG_CLIP)
    CALL_CHILD(usu_00_kusa_init)
    CALL_CHILD(usu_00_tree_init)
    // Story scenes: the chapter 4 arrival, the pig Mayor, Mario's talk with the Mayor while a shadow, the Shadow
    // Sirens, Vivian crying, the lost Super Bombomb in bush 7 and Doopliss found again.
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG4_00_WELCOME_TO_STAGE_4)
        SPAWN_EVENT(usu00_stage4_coming_event)
        CASE_BETWEEN(STG4_01_00_GATEKEEPER_CONVERSATION, STG4_01_00_GATEKEEPER_CONVERSATION)
        SPAWN_EVENT(chief_pig_event)
    END_SWITCH
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG4_11_02_WAITING_DOOPLISS, STG4_13_04_THE_LETTER_P)
        CALL_CHILD(chief_mario_talk_init)
        SPAWN_EVENT(chief_mario_talk)
    END_SWITCH
    CALL(evt_bero_get_entername, LW(0))
    IF_STR_EQUAL(LW(0), STRING("evt_majo3"))
        CALL_CHILD(usu00_3witch_init)
        SPAWN_EVENT(usu00_3witch_event)
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG4_11_02_WAITING_DOOPLISS)
        SPAWN_EVENT(usu00_vivian_cry_event)
    END_IF
    IF_INT_LESS(GSW_STORY_PROGRESS, STG4_12_01_SUPERBOMBBOMB_FOUND)
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_kusa_07"), 0, PTR(usu_00_touch_ichikorori_ball), 0)
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG4_15_03_THE_RUBY_STAR)
        CALL_CHILD(usu00_3witch_init)
        SPAWN_EVENT(gra00_2witch_find_ranpel_event)
    END_IF
    // Background watchers for Vivian and the crows; the pipe is blocked while Doopliss has Mario's name, and the
    // pig villagers' material loop runs outside Bowser's visit.
    SPAWN_EVENT(usu00_check_vivian)
    SET_VALUE(GW(0), 0)
    SET_VALUE(GW(1), 0)
    SPAWN_EVENT(usu00_crowAB_talk_event)
    SPAWN_EVENT(usu00_crowCDE_talk_event)
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG4_10_DOOPLISS_BATTLE)
        IF_INT_LESS(GSW_STORY_PROGRESS, STG4_15_02_BACK_TO_NORMAL)
            SET_VALUE(LW(0), STRING("dokan_1"))
            CALL_CHILD(bero_case_switch_off)
            CALL(evt_bero_overwrite, STRING("dokan_1"), PTR(bero_oshimodo))
            SET_VALUE(LW(0), STRING("dokan_1"))
            CALL_CHILD(bero_case_switch_on)
        END_IF
    END_IF
    IF_INT_EQUAL(GF(1), 0)
        BEGIN_INLINE_EVENT
            CALL(booboo_main)
        END_INLINE_EVENT
    END_IF
    // Bowser's interlude: Crump's arrival and Bowser's two scenes with Kammy.
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG5_47_PEACH_TEC_TALK)
        CALL(evt_npc_change_interrupt, STRING("カンブー"), 1, PTR(kanbu_regl))
        CALL(evt_npc_restart_regular_event, STRING("カンブー"))
        CALL(evt_mario_kill_party, 0)
        SPAWN_EVENT(usu_00_koopa_first_evt)
        CALL(evt_cam_road_reset)
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG5_48_BOWSER_TWILIGHT_TOWN)
        CALL(evt_bero_get_entername, LW(0))
        IF_STR_EQUAL(LW(0), STRING("player"))
            CALL(evt_npc_change_interrupt, STRING("カンブー"), 1, PTR(kanbu_regl))
            CALL(evt_npc_restart_regular_event, STRING("カンブー"))
            CALL(evt_mario_set_pos, 0, 0, 50)
            CALL(evt_mario_set_dir, 90, 0, 0)
            CALL(evt_npc_set_position, STRING("ババ"), -100, 25, 100)
        ELSE
            CALL(evt_mario_set_pos, 30, 0, 180)
            CALL(evt_mario_set_dir, 90, 0, 0)
            CALL(evt_npc_set_position, STRING("ババ"), -100, 25, 100)
            BEGIN_INLINE_EVENT
                CALL(evt_seq_wait, SEQ_GAME)
                WAIT_MS(1000)
                SPAWN_EVENT(usu_00_koopa_second_evt)
            END_INLINE_EVENT
        END_IF
        CALL(evt_cam_road_reset)
    END_IF
    // After the Peach and Bowser interludes the party is healed and the Mayor's farewell scene plays.
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG4_15_08_BOWSER_ROGUEPORT_DOCK)
        CALL(evt_mario_get_party, LW(15))
        IF_INT_EQUAL(LW(15), 0)
            CALL(evt_mario_set_party, 0, 1)
            CALL(evt_party_run, 0)
CALL(evt_pouch_mario_recovery)
            CALL(evt_pouch_all_party_recovery)
END_IF
CALL(evt_party_stop, 0)
        CALL(evt_party_set_pos, 0, -130, 0, -60)
        SPAWN_EVENT(evt_after_peach)
    END_IF
    // Chapter 8: the world going dark and the townsfolk's voices cheering Mario.
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG8_15_GRODUS)
        CALL(evt_bero_get_entername, LW(0))
        IF_STR_EQUAL(LW(0), STRING("sekai_yami2"))
            CALL_CHILD(sekai_yami_npc_init)
            SPAWN_EVENT(sekai_yami_evt)
        END_IF
        IF_STR_EQUAL(LW(0), STRING("minnnanokoe"))
            CALL_CHILD(minnnanokoe_npc_entry)
            SPAWN_EVENT(minnnanokoe_evt)
        END_IF
    END_IF
    // Epilogue scene, then the NPC hide/show loop.
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG8_END_AFTER_THE_BATTLE)
        CALL(evt_mario_kill_party, 0)
        CALL_CHILD(epilogue_npc_entry)
        SPAWN_EVENT(epilogue_evt)
    END_IF
    BEGIN_INLINE_EVENT
        CALL(npc_process)
    END_INLINE_EVENT
    RETURN
EVT_END()
#else
/**
 * Map init script of Twilight Town's main street:
 * picks BGM by story progress, registers the pipe, doors and bero exits, spawns NPCs and items,
 * and starts Chapter 4 and Bowser interlude cutscenes.
 */
EVT_BEGIN(usu_00_init_evt)
    // In Bowser's interlude (from Peach's talk with TEC to Bowser's town visit) Mario is Bowser; GF(1) marks it.
    SET_VALUE(GF(1), 0)
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG5_47_PEACH_TEC_TALK)
        IF_INT_LESS_OR_EQUAL(GSW_STORY_PROGRESS, STG5_48_BOWSER_TWILIGHT_TOWN)
            CALL(evt_mario_set_mode, 2)
            SET_VALUE(GF(1), 1)
        END_IF
    END_IF
    // Music: the Shadow Sirens' theme while Mario has lost his name, chapter 8 voices or silence in the darkness
    // scenes, otherwise the town theme.
    IF_INT_EQUAL(GF(1), 0)
        SWITCH(GSW_STORY_PROGRESS)
            CASE_BETWEEN(STG4_10_02_TRADING_PLACES, STG4_11_SUPERBOMBBOMB_LOST)
            CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_WITCH1"))
            CALL(evt_snd_envoff, PSND_FADEOUT)
            CASE_EQUAL(STG4_11_02_WAITING_DOOPLISS)
            CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_WITCH1"))
            CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG4_USU1"))
            CASE_BETWEEN(STG4_12_VIVIAN, STG4_12_01_SUPERBOMBBOMB_FOUND)
            CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_WITCH1"))
            CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG4_USU1"))
            CASE_EQUAL(STG8_15_GRODUS)
            SET_VALUE(LF(1), 0)
            CALL(evt_bero_get_entername, LW(0))
            IF_STR_EQUAL(LW(0), STRING("sekai_yami2"))
                CALL(evt_snd_bgmoff, PSND_FADEOUT)
                CALL(evt_snd_envoff, PSND_FADEOUT)
                SET_VALUE(LF(1), 1)
            END_IF
            IF_STR_EQUAL(LW(0), STRING("minnnanokoe"))
                CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_STG8_VOICE1"))
                CALL(evt_snd_envoff, PSND_FADEOUT)
                SET_VALUE(LF(1), 1)
            END_IF
            IF_INT_EQUAL(LF(1), 0)
                CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG4_USU1"))
                CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG4_USU1"))
            END_IF
            CASE_DEFAULT
            IF_INT_NOT_EQUAL(GSW_STORY_PROGRESS, STG8_END_AFTER_THE_BATTLE)
                CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG4_USU1"))
            END_IF
            CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG4_USU1"))
        END_SWITCH
        ELSE
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG4_USU1"))
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG4_USU1"))
    END_IF
    IF_INT_EQUAL(GF(1), 0)
        CALL(evt_kagemario_init)
    END_IF
    // Register the exits; in Bowser's visit the pipe becomes his scene and the doors, bushes and exits are shut.
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    IF_INT_EQUAL(GF(1), 1)
        SET_VALUE(LW(0), STRING("dokan_1"))
        CALL_CHILD(PTR(bero_case_switch_off))
        CALL(evt_bero_overwrite, STRING("dokan_1"), PTR(bero_koopa_dokan))
        SET_VALUE(LW(0), STRING("dokan_1"))
        CALL_CHILD(PTR(bero_case_switch_on))
        SET_VALUE(LW(0), 1)
        CALL_CHILD(PTR(bero_case_switch_off))
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("bero"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_attr_onoff, 1, 0, STRING("e_bero"), 8388612)
        CALL(evt_hitobj_flag_onoff, 1, 1, STRING("A_kusa_02"), 1)
        CALL(evt_hitobj_flag_onoff, 1, 1, STRING("A_kusa_03"), 1)
        CALL(evt_hitobj_flag_onoff, 1, 1, STRING("A_kusa_04"), 1)
        CALL(evt_hitobj_flag_onoff, 1, 1, STRING("A_kusa_06"), 1)
        CALL(evt_hitobj_flag_onoff, 1, 1, STRING("A_kusa_07"), 1)
        CALL(evt_hitobj_flag_onoff, 1, 1, STRING("A_kusa_08"), 1)
        CALL(evt_hitobj_flag_onoff, 1, 1, STRING("A_kusa_09"), 1)
        CALL(evt_hitobj_flag_onoff, 1, 1, STRING("A_kusa_10"), 1)
        CALL(evt_hitobj_flag_onoff, 1, 1, STRING("A_kusa_11"), 1)
        CALL(evt_hitobj_flag_onoff, 1, 1, STRING("A_kusa_12"), 1)
        CALL(evt_hitobj_flag_onoff, 1, 1, STRING("A_kusa_13"), 1)
        CALL(evt_hitobj_flag_onoff, 1, 1, STRING("A_kusa_14"), 1)
        CALL(evt_hitobj_flag_onoff, 1, 1, STRING("A_kusa_15"), 1)
        CALL(evt_hitobj_flag_onoff, 1, 1, STRING("A_kusa_16"), 1)
        CALL(evt_hitobj_flag_onoff, 1, 1, STRING("A_kusa_17"), 1)
        CALL(evt_hitobj_flag_onoff, 1, 1, STRING("A_kusa_18"), 1)
        CALL(evt_hitobj_flag_onoff, 1, 1, STRING("A_kusa_19"), 1)
        CALL(evt_hitobj_flag_onoff, 1, 1, STRING("A_kusa_20"), 1)
    END_IF
    // Save block, Star Piece, NPCs, house doors (Bowser gets locked-out scripts) and the inn.
    IF_INT_EQUAL(GF(1), 0)
        CALL(evt_mobj_save_blk, STRING("mobj_save"), 365, 60, -50, 0, 0)
    END_IF
    IF_INT_EQUAL(GF(1), 0)
        CALL(evt_item_entry, STRING("item00"), ITEM_STAR_PIECE, -220, 0, -225, ITEM_TYPE_STILL, GSWF_STARPIECE_62, 0)
    END_IF
    CALL_CHILD(PTR(evt_usu_kagemario_party_kill))
    CALL(evt_npc_setup, PTR(npcEnt))
    IF_INT_EQUAL(GF(1), 1)
        CALL_CHILD(PTR(koopa_npc_entry))
    END_IF
    IF_INT_EQUAL(GF(1), 0)
        SET_VALUE(LW(0), PTR(&door_1))
        CALL_CHILD(PTR(evt_door_entry))
        SET_VALUE(LW(0), PTR(&door_2))
        CALL_CHILD(PTR(evt_door_entry))
        SET_VALUE(LW(0), PTR(&door_3))
        CALL_CHILD(PTR(evt_door_entry))
        ELSE
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_ie1door"), 0, PTR(door_ie1_koopa_evt), 0)
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_ie2door"), 0, PTR(door_ie2_koopa_evt), 0)
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_ie3door"), 0, PTR(door_ie3_koopa_evt), 0)
    END_IF
    IF_INT_EQUAL(GF(1), 0)
        CALL(evt_kinopio_setup, PTR(&kino_dt))
    END_IF
    // Trees and bushes.
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("ki1"), MAP_OBJ_FLAG_CLIP)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("ki2"), MAP_OBJ_FLAG_CLIP)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("ki3"), MAP_OBJ_FLAG_CLIP)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("ki4"), MAP_OBJ_FLAG_CLIP)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("ki5"), MAP_OBJ_FLAG_CLIP)
    CALL_CHILD(PTR(usu_00_kusa_init))
    CALL_CHILD(PTR(usu_00_tree_init))
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG4_00_WELCOME_TO_STAGE_4)
        SPAWN_EVENT(PTR(usu00_stage4_coming_event))
        CASE_BETWEEN(STG4_01_00_GATEKEEPER_CONVERSATION, STG4_01_00_GATEKEEPER_CONVERSATION)
        SPAWN_EVENT(PTR(chief_pig_event))
    END_SWITCH
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG4_11_02_WAITING_DOOPLISS, STG4_13_04_THE_LETTER_P)
        CALL_CHILD(PTR(chief_mario_talk_init))
        SPAWN_EVENT(PTR(chief_mario_talk))
    END_SWITCH
    CALL(evt_bero_get_entername, LW(0))
    IF_STR_EQUAL(LW(0), STRING("evt_majo3"))
        CALL_CHILD(PTR(usu00_3witch_init))
        SPAWN_EVENT(PTR(usu00_3witch_event))
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG4_11_02_WAITING_DOOPLISS)
        SPAWN_EVENT(PTR(usu00_vivian_cry_event))
    END_IF
    IF_INT_LESS(GSW_STORY_PROGRESS, STG4_12_01_SUPERBOMBBOMB_FOUND)
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_kusa_07"), 0, PTR(usu_00_touch_ichikorori_ball), 0)
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG4_15_03_THE_RUBY_STAR)
        CALL_CHILD(PTR(usu00_3witch_init))
        SPAWN_EVENT(PTR(gra00_2witch_find_ranpel_event))
    END_IF
    SPAWN_EVENT(PTR(usu00_check_vivian))
    SET_VALUE(GW(0), 0)
    SET_VALUE(GW(1), 0)
    SPAWN_EVENT(PTR(usu00_crowAB_talk_event))
    SPAWN_EVENT(PTR(usu00_crowCDE_talk_event))
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG4_10_DOOPLISS_BATTLE)
        IF_INT_LESS(GSW_STORY_PROGRESS, STG4_15_02_BACK_TO_NORMAL)
            SET_VALUE(LW(0), STRING("dokan_1"))
            CALL_CHILD(PTR(bero_case_switch_off))
            CALL(evt_bero_overwrite, STRING("dokan_1"), PTR(bero_oshimodo))
            SET_VALUE(LW(0), STRING("dokan_1"))
            CALL_CHILD(PTR(bero_case_switch_on))
        END_IF
    END_IF
    IF_INT_EQUAL(GF(1), 0)
        BEGIN_INLINE_EVENT
            CALL(booboo_main)
        END_INLINE_EVENT
    END_IF
    // Bowser's interlude: Crump's arrival and Bowser's two scenes with Kammy.
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG5_47_PEACH_TEC_TALK)
        CALL(evt_npc_change_interrupt, STRING("カンブー"), 1, PTR(kanbu_regl))
        CALL(evt_npc_restart_regular_event, STRING("カンブー"))
        CALL(evt_mario_kill_party, 0)
        SPAWN_EVENT(PTR(usu_00_koopa_first_evt))
        CALL(evt_cam_road_reset)
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG5_48_BOWSER_TWILIGHT_TOWN)
        CALL(evt_bero_get_entername, LW(0))
        IF_STR_EQUAL(LW(0), STRING("player"))
            CALL(evt_npc_change_interrupt, STRING("カンブー"), 1, PTR(kanbu_regl))
            CALL(evt_npc_restart_regular_event, STRING("カンブー"))
            CALL(evt_mario_set_pos, 0, 0, 50)
            CALL(evt_mario_set_dir, 90, 0, 0)
            CALL(evt_npc_set_position, STRING("ババ"), -100, 25, 100)
            ELSE
            CALL(evt_mario_set_pos, 30, 0, 180)
            CALL(evt_mario_set_dir, 90, 0, 0)
            CALL(evt_npc_set_position, STRING("ババ"), -100, 25, 100)
            BEGIN_INLINE_EVENT
                CALL(evt_seq_wait, SEQ_GAME)
                WAIT_MS(1000)
                SPAWN_EVENT(PTR(usu_00_koopa_second_evt))
            END_INLINE_EVENT
        END_IF
        CALL(evt_cam_road_reset)
    END_IF
    // After the Peach and Bowser interludes the party is healed and the Mayor's farewell scene plays.
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG4_15_08_BOWSER_ROGUEPORT_DOCK)
        CALL(evt_mario_get_party, LW(15))
        IF_INT_EQUAL(LW(15), 0)
            CALL(evt_mario_set_party, 0, 1)
            CALL(evt_party_run, 0)
#if VERSION >= VERSION_US
        END_IF
#endif
            CALL(evt_pouch_mario_recovery)
            CALL(evt_pouch_all_party_recovery)
#if VERSION >= VERSION_US
        CALL(evt_sub_status_gauge_force_update)
#else
        END_IF
#endif
        CALL(evt_party_stop, 0)
        CALL(evt_party_set_pos, 0, -130, 0, -60)
        SPAWN_EVENT(PTR(evt_after_peach))
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG8_15_GRODUS)
        CALL(evt_bero_get_entername, LW(0))
        IF_STR_EQUAL(LW(0), STRING("sekai_yami2"))
            CALL_CHILD(PTR(sekai_yami_npc_init))
            SPAWN_EVENT(PTR(sekai_yami_evt))
        END_IF
        IF_STR_EQUAL(LW(0), STRING("minnnanokoe"))
            CALL_CHILD(PTR(minnnanokoe_npc_entry))
            SPAWN_EVENT(PTR(minnnanokoe_evt))
        END_IF
    END_IF
    // Epilogue scene, then the NPC hide/show loop.
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG8_END_AFTER_THE_BATTLE)
        CALL(evt_mario_kill_party, 0)
        CALL_CHILD(PTR(epilogue_npc_entry))
        SPAWN_EVENT(PTR(epilogue_evt))
    END_IF
    BEGIN_INLINE_EVENT
        CALL(npc_process)
    END_INLINE_EVENT
    RETURN
EVT_END()
#endif

