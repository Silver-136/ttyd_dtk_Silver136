// nok_01 (ノコ村, Petalburg): the east half of the village with the mayor's and Koops's houses; the gate to Petal
// Meadows (hei_02) and the pipe to Rogueport Sewers (tik_16).

#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_story.h"
#include "evt/evt_urouro.h"
#include "evt/evt_window.h"
#include "eff/eff_starstone.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bg.h"
#include "evt/evt_cam.h"
#include "evt/evt_eff.h"
#include "evt/evt_fade.h"
#include "evt/evt_hit.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_mobj.h"
#include "evt/evt_msg.h"
#include "evt/evt_party.h"
#include "evt/evt_pouch.h"
#include "evt/evt_seq.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_item.h"
#include "driver/mapdrv.h"
#include "system.h"
#include "pmario_sound.h"
#include "driver/itemdrv.h"
#include "mario/mario.h"
#include "driver/envdrv.h"

#include "manager/evtmgr_cmd.h"
#include "dolphin/mtx.h"
#include "driver/npcdrv.h"
#include "driver/animdrv.h"
#include "driver/effdrv.h"
#include "evt/evt_bero.h"
#include "evt/evt_door.h"
#include "evt/evt_damage.h"
#include "evt/evt_sub.h"
#include "evt/evt_npc.h"
#include "driver/hitdrv.h"
#include "data_tables.h"
#include "mario/mario_pouch.h"
#include "driver/seqdrv.h"
#include "nok.h"

static USER_FUNC(anim_tevcallback);
static USER_FUNC(starstone_entry);
static USER_FUNC(starstone_set_pos);
static USER_FUNC(starstone_set_ry);
static USER_FUNC(yami_view);
extern s32 envSetYami[];

EvtWord nokonoko_G_regl[19];
EvtWord nokonoko_G_talk[44];
EvtWord nokonoko_H_regl[19];
EvtWord nokonoko_H_talk[52];
EvtWord nokonoko_I_regl[19];
EvtWord nokonoko_I_talk[44];
EvtWord open_gate[221];
EvtWord monban_init[14];
EvtWord monban_talk[56];
static EvtWord nokotarou_init[26];
static EvtWord nokotarou_talk[36];
EvtWord nokopapa_init[19];
EvtWord nokopapa_talk[72];
EvtWord nokonoko_K_talk[69];
#if VERSION >= VERSION_US
EvtWord boss_noko_open_gate[184];
#else
EvtWord boss_noko_open_gate[181];
#endif
static s32 iri_17_item_tbl[2];
#if VERSION <= VERSION_DEMO_US
extern EvtWord boss_noko_talk[];
#else
EvtWord boss_noko_talk[352];
#endif
EvtWord nokorin_init[30];
#if VERSION == VERSION_US
EvtWord nokorin_talk[281];
#else
EvtWord nokorin_talk[267];
#endif
static EvtWord odoriko_1_talk[44];
static EvtWord odoriko_1_init[33];
static EvtWord odoriko_2_talk[44];
static EvtWord odoriko_2_init[33];
static EvtWord odoriko_3_talk[44];
static EvtWord odoriko_3_init[33];
static NpcSetupInfo npcEnt[10];
static BeroEntry bero_entry_data[4];
const char* son_npc[2];
const char* son_mobj[2];
const char* son_item[2];
HouseParam son_data;
const char* taro_npc[2];
const char* taro_map[2];
const char* taro_mobj[2];
const char* taro_item[2];
HouseParam taro_data;
static const char* ie_npc[2];
static const char* ie_map[2];
static HouseParam ie_data;
static HitDamageReturn hitDmgRetPoints[13];
#if VERSION <= VERSION_DEMO_US
extern EvtWord nokotarou_tabidachi[];
#else
EvtWord nokotarou_tabidachi[328];
#endif
EvtWord nokotarou_tabidachi_init[89];
static EvtWord sekai_yami_npc[59];
static EvtWord sekai_yami_evt[234];
static EvtWord minnnanokoe_evt[416];
static EvtWord evt_switch_init[17];
EvtWord e3_talk[12];
static EvtWord epilogue_npc_entry[31];
static EvtWord epilogue_evt_sub[194];
static EvtWord epilogue_evt[148];
static EvtWord evt_kusa_4[25];
static EvtWord evt_kusa_5[25];
static EvtWord evt_kusa_6[25];
static KusaAccess kusa_data[4];
EvtWord nok_01_init_evt[337];

/**
 * Message callback for the Trouble Center request 8 (irai_08_001) text:
 * when the text reaches character 9 it clears GF(0),
 * which releases the parallel thread that puts Mario to sleep (evt_mario_sleep_on) while Mayor Kroop talks.
 */
void iri_08_msg(s32 type, void** work) {
    u16* msg = *work;

    if (type == 3 && msg[4] == 9) {
        evtSetValue(NULL, GF(0), 0);
    }
}

/**
 * Regular script for Koopa G: wanders at random within 100 units of
 * where he stands (urouro_init_func / urouro_main_func).
 */
EVT_BEGIN(nokonoko_G_regl)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(urouro_init_func, STRING("me"), LW(0), LW(2), FLOAT(100.0f), FLOAT(30.0f), 0)
    CALL(urouro_main_func, STRING("me"))
    RETURN
EVT_END()

/**
 * Talk script for Koopa G: story-progress dependent chatter (before the Gold Fuzzy,
 * before the father and son reunite, before Frankly's house, up to the end of chapter 8, and afterwards).
 */
EVT_BEGIN(nokonoko_G_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG1_06_01_GOLD_FUZZY)
        CALL(evt_msg_print, 0, STRING("stg1_nok_20"), 0, 0)
        CASE_LESS(STG1_17_FATHER_SON_REUNITED)
        CALL(evt_msg_print, 0, STRING("stg1_nok_20_01"), 0, 0)
        CASE_LESS(STG1_31_FRANKLYS_HOUSE_STG_1)
        CALL(evt_msg_print, 0, STRING("stg1_nok_21"), 0, 0)
        CASE_LESS_OR_EQUAL(STG8_END_AFTER_THE_BATTLE)
        CALL(evt_msg_print, 0, STRING("stg1_nok_21_1"), 0, 0)
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg1_nok_21_1_1"), 0, 0)
    END_SWITCH
    RETURN
EVT_END()

/** Regular script for Koopa H: wanders at random within 100 units of his spot. */
EVT_BEGIN(nokonoko_H_regl)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(urouro_init_func, STRING("me"), LW(0), LW(2), FLOAT(100.0f), FLOAT(30.0f), 0)
    CALL(urouro_main_func, STRING("me"))
    RETURN
EVT_END()

/**
 * Talk script for Koopa H: story-progress dependent chatter;
 * before Frankly's house he asks a yes/no question and follows up on the answer.
 */
EVT_BEGIN(nokonoko_H_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG1_06_01_GOLD_FUZZY)
        CALL(evt_msg_print, 0, STRING("stg1_nok_22"), 0, 0)
        CASE_LESS(STG1_17_FATHER_SON_REUNITED)
        CALL(evt_msg_print, 0, STRING("stg1_nok_22_01"), 0, 0)
        CASE_LESS(STG1_31_FRANKLYS_HOUSE_STG_1)
        CALL(evt_msg_print, 0, STRING("stg1_nok_23"), 0, 0)
        CALL(evt_msg_select, 0, STRING("stg1_nok_23_yesno"))
        CALL(evt_msg_print_add, 0, STRING("stg1_nok_23_01"))
        CASE_LESS_OR_EQUAL(STG8_END_AFTER_THE_BATTLE)
        CALL(evt_msg_print, 0, STRING("stg1_nok_23_01_1"), 0, 0)
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg1_nok_23_01_1_1"), 0, 0)
    END_SWITCH
    RETURN
EVT_END()

/** Regular script for Koopa I: wanders at random within 100 units of his spot. */
EVT_BEGIN(nokonoko_I_regl)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(urouro_init_func, STRING("me"), LW(0), LW(2), FLOAT(100.0f), FLOAT(30.0f), 0)
    CALL(urouro_main_func, STRING("me"))
    RETURN
EVT_END()

/** Talk script for Koopa I: story-progress dependent chatter, same schedule as Koopa G. */
EVT_BEGIN(nokonoko_I_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG1_06_01_GOLD_FUZZY)
        CALL(evt_msg_print, 0, STRING("stg1_nok_24"), 0, 0)
        CASE_LESS(STG1_17_FATHER_SON_REUNITED)
        CALL(evt_msg_print, 0, STRING("stg1_nok_24_01"), 0, 0)
        CASE_LESS(STG1_31_FRANKLYS_HOUSE_STG_1)
        CALL(evt_msg_print, 0, STRING("stg1_nok_25"), 0, 0)
        CASE_LESS_OR_EQUAL(STG8_END_AFTER_THE_BATTLE)
        CALL(evt_msg_print, 0, STRING("stg1_nok_25_1"), 0, 0)
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg1_nok_25_1_1"), 0, 0)
    END_SWITCH
    RETURN
EVT_END()

/**
 * Cutscene when Mario first talks to the gatekeeper (monban) after meeting Kroop:
 * the gatekeeper opens the village gate and Koops, standing outside,
 * runs in and has a short exchange with Mario and the partner before walking off.
 * Sets the story to STG1_03_PETALBURG_GATEKEEPER.
 */
EVT_BEGIN(open_gate)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_HIT)
    CALL(evt_msg_print, 0, STRING("stg1_nok_28"), 0, STRING("me"))
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_hitobj_onoff, STRING("A_mon"), 1, 0)
    CALL(evt_npc_set_ry, STRING("me"), 90)
    CALL(evt_npc_move_position, STRING("門番ノコノコ"), 420, 0, 0, FLOAT(55.0f), 0)
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        WAIT_MS(2200)
        CALL(evt_npc_set_ry, STRING("me"), 270)
        CALL(evt_npc_move_position, STRING("門番ノコノコ"), 370, -76, 0, FLOAT(51.0f), 0)
    END_INLINE_EVENT
    CALL(evt_hit_get_position, STRING("A_mon"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_FENCE_OPEN1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_map_playanim, STRING("S_mon"), MAP_ANIM_PLAY_ONCE, 0)
    CALL(evt_map_checkanim, STRING("S_mon"), LW(0), LW(1))
    WAIT_MS(LW(1))
    CALL(evt_npc_set_position, STRING("ノコタロウ"), 165, 0, -41)
    CALL(evt_set_dir_to_target, STRING("ノコタロウ"), STRING("mario"))
    BEGIN_INLINE_EVENT
        WAIT_MS(500)
        CALL(evt_mario_set_dir_npc, STRING("ノコタロウ"))
        CALL(evt_party_set_dir_npc, 0, STRING("ノコタロウ"))
    END_INLINE_EVENT
    CALL(evt_msg_print, 0, STRING("stg1_nok_29"), 0, STRING("ノコタロウ"))
    CALL(evt_cam3d_evt_set, 197, 75, 230, 197, 20, -52, 1000, EASING_SINE_IN_OUT)
    WAIT_MS(700)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_mov_pos, 350, 0, 500)
        CALL(evt_mario_set_dir_npc, STRING("ノコタロウ"))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_party_move_pos, 0, 330, -30, 500)
        CALL(evt_party_set_dir_npc, 0, STRING("ノコタロウ"))
    END_INLINE_EVENT
    WAIT_MS(700)
    CALL(evt_cam3d_evt_set, 320, 85, 261, 320, 32, -11, 1500, EASING_SINE_IN_OUT)
    CALL(evt_npc_move_position, STRING("ノコタロウ"), 295, 0, 0, FLOAT(130.0f), 0)
    WAIT_MS(1000)
    CALL(evt_npc_set_ry, STRING("ノコタロウ"), 270)
    WAIT_MS(500)
    CALL(evt_npc_set_ry, STRING("ノコタロウ"), 90)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg1_nok_30"), 0, STRING("ノコタロウ"))
    CALL(evt_npc_set_ry, STRING("ノコタロウ"), 270)
    CALL(evt_npc_move_position, STRING("ノコタロウ"), 165, -14, 0, FLOAT(100.0f), 0)
    CALL(evt_npc_set_position, STRING("ノコタロウ"), -30, 65, -472)
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_NO_HIT)
    CALL(evt_party_stop, 0)
    WAIT_FRAMES(1)
    CALL(evt_msg_print, 0, STRING("stg1_nok_30_01"), 0, STRING("party"))
    CALL(evt_party_run, 0)
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
    SET_VALUE(GSW_STORY_PROGRESS, STG1_03_PETALBURG_GATEKEEPER)
    RETURN
EVT_END()

/**
 * Init for the gatekeeper Koopa (門番ノコノコ): once the gate has been opened (STG1_03 and later) he stands
 * beside the gate at (370, 0, -76) instead of in front of it.
 */
EVT_BEGIN(monban_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG1_03_PETALBURG_GATEKEEPER)
        CASE_DEFAULT
        CALL(evt_npc_set_position, STRING("me"), 370, 0, -76)
    END_SWITCH
    RETURN
EVT_END()

/**
 * Talk script for the gatekeeper: refuses to open the gate until Mario has spoken with Kroop,
 * runs open_gate the first time after that, and afterwards has story-progress dependent lines.
 */
EVT_BEGIN(monban_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG1_02_TALKING_WITH_KROOP)
        CALL(evt_msg_print, 0, STRING("stg1_nok_26"), 0, 0)
        CASE_LESS(STG1_03_PETALBURG_GATEKEEPER)
        CALL_CHILD(PTR(open_gate))
        CASE_LESS(STG1_06_01_GOLD_FUZZY)
        CALL(evt_msg_print, 0, STRING("stg1_nok_31_01_00"), 0, 0)
        CASE_LESS(STG1_17_FATHER_SON_REUNITED)
        CALL(evt_msg_print, 0, STRING("stg1_nok_31_01_01"), 0, 0)
        CASE_LESS(STG1_31_FRANKLYS_HOUSE_STG_1)
        CALL(evt_msg_print, 0, STRING("stg1_nok_31_02_1"), 0, 0)
        CASE_LESS_OR_EQUAL(STG8_END_AFTER_THE_BATTLE)
        CALL(evt_msg_print, 0, STRING("stg1_nok_31_02_2"), 0, 0)
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg1_nok_31_02_2_1"), 0, 0)
    END_SWITCH
    RETURN
EVT_END()

/**
 * Init of Nokotarou (Koops) in the village:
 * runs the standard NPC init, then, before the Gold Fuzzy (STG1_06_01),
 * stands him at (-35, 65, -425) facing 90; after that it sends him off the map to (0, -1000, 0).
 */
static EVT_BEGIN(nokotarou_init)
    CALL_CHILD(PTR(npc_init_evt))
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG1_06_01_GOLD_FUZZY)
        CALL(evt_npc_set_position, STRING("me"), -35, 65, -425)
        CALL(evt_npc_set_ry, STRING("me"), 90)
        CASE_DEFAULT
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(nokotarou_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG1_02_TALKING_WITH_KROOP)
        CALL(evt_msg_print, 0, STRING("stg1_nok_27"), 0, 0)
        CASE_LESS(STG1_03_PETALBURG_GATEKEEPER)
        CALL(evt_msg_print, 0, STRING("stg1_nok_27_01"), 0, 0)
        CASE_LESS(STG1_04_BALD_CLEFT)
        CALL(evt_msg_print, 0, STRING("stg1_nok_31"), 0, 0)
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg1_nok_31_01"), 0, 0)
    END_SWITCH
    RETURN
EVT_END()

/**
 * Init for Koops's father (nokopapa): after Hooktail is defeated he moves to a spot by the house at (92, 50,
 * -308), facing west.
 */
EVT_BEGIN(nokopapa_init)
    CALL_CHILD(PTR(npc_init_evt))
    SWITCH(GSW_STORY_PROGRESS)
        CASE_GREATER(STG1_16_HOOKTAIL)
        CALL(evt_npc_set_position, STRING("me"), 92, 50, -308)
        CALL(evt_npc_set_ry, STRING("me"), 270)
    END_SWITCH
    RETURN
EVT_END()

/**
 * Talk script for Koops's father.
 * After Hooktail, the line depends on the chapter
 * and on whether Koops (party id 2) is currently Mario's partner.
 */
EVT_BEGIN(nokopapa_talk)
    CALL(evt_mario_get_party, LW(0))
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG4_00_WELCOME_TO_STAGE_4)
        IF_INT_EQUAL(LW(0), 2)
            CALL(evt_msg_print, 0, STRING("stg1_nok_79"), 0, 0)
            ELSE
            CALL(evt_msg_print, 0, STRING("stg1_nok_80"), 0, 0)
        END_IF
        CASE_LESS(STG7_20_LORD_CRUMP_DEFEATED)
        IF_INT_EQUAL(LW(0), 2)
            CALL(evt_msg_print, 0, STRING("stg1_nok_81"), 0, 0)
            ELSE
            CALL(evt_msg_print, 0, STRING("stg1_nok_82"), 0, 0)
        END_IF
        CASE_LESS(STG8_END_AFTER_THE_BATTLE)
        IF_INT_EQUAL(LW(0), 2)
            CALL(evt_msg_print, 0, STRING("stg1_nok_83"), 0, 0)
            ELSE
            CALL(evt_msg_print, 0, STRING("stg1_nok_84"), 0, 0)
        END_IF
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg1_nok_84_1"), 0, 0)
    END_SWITCH
    RETURN
EVT_END()

/**
 * Talk script for Koopa K. If Trouble Center request 14 is active he gives it the first time he is spoken to and
 * repeats a reminder afterwards; otherwise story-progress dependent chatter.
 */
EVT_BEGIN(nokonoko_K_talk)
    IF_INT_EQUAL(GSW_TROUBLE_ACCEPTED, TROUBLE_HEARTFUL_CAKE_RECIPE)
        IF_INT_EQUAL(GSWF_TROUBLE_WORK(0), 0)
            CALL(evt_msg_print, 0, STRING("irai_14_00"), 0, 0)
            SET_VALUE(GSWF_TROUBLE_WORK(0), 1)
            ELSE
            CALL(evt_msg_print, 0, STRING("irai_14_01"), 0, 0)
        END_IF
        RETURN
    END_IF
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG1_06_01_GOLD_FUZZY)
        CALL(evt_msg_print, 0, STRING("stg1_nok_32"), 0, 0)
        CASE_LESS(STG1_17_FATHER_SON_REUNITED)
        CALL(evt_msg_print, 0, STRING("stg1_nok_32_01"), 0, 0)
        CASE_LESS(STG1_31_FRANKLYS_HOUSE_STG_1)
        CALL(evt_msg_print, 0, STRING("stg1_nok_33"), 0, 0)
        CASE_LESS_OR_EQUAL(STG8_END_AFTER_THE_BATTLE)
        CALL(evt_msg_print, 0, STRING("stg1_nok_33_1"), 0, 0)
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg1_nok_33_1_1"), 0, 0)
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(boss_noko_open_gate)
    CALL(evt_msg_print, 0, STRING("stg1_nok_37"), 0, STRING("me"))
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_POSE_LOCK)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_HIT)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_SURPRISED1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_mario_set_pose, STRING("M_N_7"))
        CALL(evt_mario_jump_pos, -310, 110, -318, 200, 1, -1000)
        CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_NO_HIT)
        CALL(evt_mario_set_dir, 270, 0, 0)
    END_INLINE_EVENT
    WAIT_MS(1000)
#if VERSION >= VERSION_US
    CALL(evt_party_move_behind_mario, 0)
#endif
    CALL(evt_set_dir_to_target, STRING("me"), STRING("mario"))
    CALL(evt_mario_set_pose, STRING("M_N_B"))
    WAIT_MS(800)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_msg_print, 0, STRING("stg1_nok_38"), 0, STRING("me"))
    CALL(evt_mario_set_pose, STRING("M_I_2"))
    WAIT_MS(1000)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_msg_print, 0, STRING("stg1_nok_39"), 0, STRING("me"))
    CALL(evt_mario_set_pose, STRING("M_N_2"))
    WAIT_MS(1000)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_msg_print, 0, STRING("stg1_nok_40"), 0, STRING("me"))
    CALL(evt_msg_select, 0, STRING("stg1_nok_40_yn"))
    CALL(evt_msg_print_add, 0, STRING("stg1_nok_41"))
    CALL(evt_msg_select, 0, STRING("stg1_nok_41_yn"))
    CALL(evt_msg_print_add, 0, STRING("stg1_nok_42"))
    CALL(evt_msg_select, 0, STRING("stg1_nok_42_yn"))
    CALL(evt_msg_print_add, 0, STRING("stg1_nok_43"))
    CALL(evt_mario_set_pose, STRING("M_I_2"))
    WAIT_MS(1000)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_msg_print, 0, STRING("stg1_nok_44"), 0, STRING("me"))
    CALL(evt_msg_select, 0, STRING("stg1_nok_44_yn"))
    CALL(evt_msg_print_add, 0, STRING("stg1_nok_45"))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_NOD1_4"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING("M_N_2"))
    WAIT_MS(1000)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_msg_print, 0, STRING("stg1_nok_46"), 0, STRING("me"))
    CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_POSE_LOCK)
    SET_VALUE(GSW_STORY_PROGRESS, STG1_02_TALKING_WITH_KROOP)
    RETURN
EVT_END()

/** Items the player may hand to Mayor Kroop for request 17: the Routing Slip only, ended by -1. */
static s32 iri_17_item_tbl[] = {
    ITEM_ROUTING_SLIP, -1,
};

/**
 * Talk script for Mayor Kroop.
 * In order of priority: Trouble Center request 8 (he asks for help and gives a Turtley Leaf,
 * handed over directly if Mario has room, otherwise he hops to a ledge and drops it as a pickup;
 * the request is then marked solved), request 17 (he takes the Routing Slip from Mario and returns it),
 * then the story schedule: the first meeting (boss_noko_open_gate), the Sun Stone / Moon Stone errand,
 * and chatter up to the end of chapter 8. In chapter 7 he gives the white Petalburg hint (JP and later).
 */
EVT_BEGIN(boss_noko_talk)
    IF_INT_EQUAL(GSW_TROUBLE_ACCEPTED, TROUBLE_LISTEN_TO_ME)
        CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_HIT)
        BEGIN_INLINE_EVENT
            CALL(evt_party_move_pos2, 1, -250, -320, 40)
            CALL(evt_party_set_dir, 1, 270, 300)
        END_INLINE_EVENT
        BEGIN_INLINE_EVENT
            CALL(evt_party_move_pos2, 0, -280, -320, 40)
            CALL(evt_party_set_dir, 0, 270, 300)
        END_INLINE_EVENT
        CALL(evt_mario_mov_pos2, -310, -320, 40)
        CALL(evt_mario_set_dir, 270, 300, 0)
        CALL(evt_set_dir_to_target, STRING("me"), STRING("mario"))
        WAIT_FRAMES(1)
        CALL(evt_npc_wait_pera, STRING("me"))
        CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_NO_HIT)
        SET_VALUE(GF(0), 1)
        BEGIN_INLINE_EVENT
            WAIT_WHILE(GF(0))
            CALL(evt_mario_sleep_on)
        END_INLINE_EVENT
        // The mayor asks Mario for help with Trouble Center request 8.
        CALL(evt_msg_print, 0, STRING("irai_08_001"), PTR(iri_08_msg), STRING("me"))
        CALL(evt_mario_sleep_off)
        // He forgot to set out the tea and gives Mario a Turtley Leaf.
        CALL(evt_msg_print, 0, STRING("irai_08_002"), 0, STRING("me"))
        CALL(evt_pouch_get_haveitemcnt, LW(0))
        CALL(evt_pouch_check_item, ITEM_STRANGE_SACK, LW(1))
        IF_INT_EQUAL(LW(1), 0)
            SET_VALUE(LW(1), 10)
            ELSE
            SET_VALUE(LW(1), 20)
        END_IF
        IF_INT_LESS(LW(0), LW(1))
            CALL(evt_msg_continue)
            CALL(evt_item_entry, STRING("item_irai"), ITEM_TURTLEY_LEAF, 0, -1000, 0, ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
            CALL(evt_item_get_item, STRING("item_irai"))
            SET_VALUE(GSWF_NOK_01_LISTEN_TO_ME_TURTLEY_LEAF, 1)
            ELSE
            CALL(evt_msg_print_add, 0, STRING("irai_08_003_1"))
            CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
            CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
            CALL(evt_npc_status_onoff, 1, STRING("me"), NPC_STATUS_FACE_MOVE_DIR)
            CALL(evt_npc_move_position, STRING("me"), -317, -350, 0, 40, 0)
            WAIT_MS(300)
            CALL(evt_item_entry, STRING("iri_08"), ITEM_TURTLEY_LEAF, -317, 128, -374, ITEM_TYPE_STILL, GSWF_NOK_01_LISTEN_TO_ME_TURTLEY_LEAF, 0)
            WAIT_MS(300)
            CALL(evt_npc_move_position, STRING("me"), LW(0), LW(2), 0, 40, 0)
            CALL(evt_set_dir_to_target, STRING("me"), STRING("mario"))
            WAIT_FRAMES(1)
            CALL(evt_npc_wait_pera, STRING("me"))
            CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
        END_IF
        // Request 8 is marked solved.
        CALL(evt_msg_print, 0, STRING("irai_08_004"), 0, STRING("me"))
        SET_VALUE(GSW_TROUBLE_ACCEPTED, TROUBLE_NONE)
        SET_VALUE(GSWF_TROUBLE_DONE(TROUBLE_LISTEN_TO_ME), 1)
        CALL(evt_msg_print, 0, STRING("sys_request_finish"), 0, 0)
        RETURN
    END_IF
    IF_INT_EQUAL(GSW_TROUBLE_ACCEPTED, TROUBLE_NEWSLETTER)
        IF_INT_EQUAL(GSWF_TROUBLE_WORK(0), 0)
            ELSE
            IF_INT_EQUAL(GSWF_TROUBLE_WORK(1), 0)
                ELSE
                IF_INT_EQUAL(GSWF_TROUBLE_WORK(2), 0)
                    // Request 17: the mayor takes the Routing Slip from Mario.
                    CALL(evt_msg_print, 0, STRING("irai_17_00"), 0, 0)
#if VERSION <= VERSION_DEMO_US
                    CALL(evt_win_item_select, PTR(iri_17_item_tbl), LW(0))
#else
                    CALL(evt_win_item_select, 0, PTR(iri_17_item_tbl), LW(0), 0)
#endif
                    IF_INT_NOT_EQUAL(LW(0), -1)
                        CALL(evt_pouch_remove_item, ITEM_ROUTING_SLIP, LW(0))
                        CALL(evt_msg_print, 0, STRING("irai_17_01"), 0, 0)
                        CALL(evt_item_entry, STRING("kairanban"), ITEM_ROUTING_SLIP, 0, -1000, 0, ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
                        CALL(evt_item_get_item, STRING("kairanban"))
                        CALL(evt_item_get_item_end_wait, STRING("kairanban"))
                        SET_VALUE(GSWF_TROUBLE_WORK(2), 1)
                    END_IF
                    RETURN
                    ELSE
                    CALL(evt_msg_print, 0, STRING("irai_17_02"), 0, 0)
                    RETURN
                END_IF
            END_IF
        END_IF
    END_IF
    // With no request pending, his talk follows the story.
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG1_02_TALKING_WITH_KROOP)
        CALL_CHILD(PTR(boss_noko_open_gate))
        CASE_LESS(STG1_08_OPENING_THE_PATH)
        IF_INT_EQUAL(GSWF_HEI_SUN_STONE_TAKEN, 1)
            IF_INT_EQUAL(GSWF_HEI_MOON_STONE_TAKEN, 1)
                // After the Sun Stone and Moon Stone errand he congratulates Mario.
                CALL(evt_msg_print, 0, STRING("stg1_nok_56"), 0, 0)
                RETURN
            END_IF
        END_IF
        CALL(evt_msg_print, 0, STRING("stg1_nok_47"), 0, 0)
        CASE_LESS(STG1_17_FATHER_SON_REUNITED)
        CALL(evt_msg_print, 0, STRING("stg1_nok_56_01"), 0, 0)
        CASE_LESS(STG1_31_FRANKLYS_HOUSE_STG_1)
        CALL(evt_msg_print, 0, STRING("stg1_nok_57"), 0, 0)
#if VERSION > VERSION_DEMO_US
        CASE_EQUAL(STG7_02_MAYOR_BOB)
        SET_VALUE(GSWF_NOK_GENERAL_WHITE_HINT_HEARD, 1)
        CALL(evt_msg_print, 0, STRING("stg1_nok_57_1_00"), 0, 0)
#endif
        CASE_LESS_OR_EQUAL(STG8_END_AFTER_THE_BATTLE)
        CALL(evt_msg_print, 0, STRING("stg1_nok_57_1"), 0, 0)
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg1_nok_57_1_1"), 0, 0)
    END_SWITCH
    RETURN
EVT_END()

/**
 * Init for Koopie Koo (ノコリン): places her at (-85, 50, -300);
 * while Koops has joined but she has not yet spoken to him (STG1_07 up to 17) she stands at (30, 50, -315)
 * instead.
 */
EVT_BEGIN(nokorin_init)
    CALL(evt_npc_set_position, STRING("me"), -85, 50, -300)
    IF_INT_LESS(GSW_STORY_PROGRESS, STG1_17_FATHER_SON_REUNITED)
        IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG1_07_KOOPS_JOINS)
            IF_INT_EQUAL(GSWF_NOK_KOOPIE_KOO_BLESSING_DONE, 0)
                CALL(evt_npc_set_position, STRING("me"), 30, 50, -315)
            END_IF
        END_IF
    END_IF
    CALL(evt_npc_set_ry, STRING("me"), 90)
    RETURN
EVT_END()

EVT_BEGIN(nokorin_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG1_06_01_GOLD_FUZZY)
        IF_INT_EQUAL(GSWF_NOK_KOOPIE_KOO_FIRST_TALK_DONE, 0)
            SET_VALUE(GSWF_NOK_KOOPIE_KOO_FIRST_TALK_DONE, 1)
            // Koopie Koo introduces herself to Mario.
            CALL(evt_msg_print, 0, STRING("stg1_nok_64"), 0, 0)
            CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
            CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_NOD1_5"), LW(0), LW(1), LW(2), 0)
            CALL(evt_mario_set_pose, STRING("M_N_2"))
            WAIT_MS(300)
            CALL(evt_mario_set_pose, STRING("M_S_1"))
            CALL(evt_msg_print, 0, STRING("stg1_nok_64_01"), 0, 0)
            ELSE
            // She confides her worries about her boyfriend Koops.
            CALL(evt_msg_print, 0, STRING("stg1_nok_65"), 0, 0)
            IF_INT_EQUAL(GSWF_NOK_KOOPIE_KOO_CONFESSION_SEEN, 0)
                SET_VALUE(GSWF_NOK_KOOPIE_KOO_CONFESSION_SEEN, 1)
                WAIT_MS(500)
                CALL(evt_eff_fukidashi, FUKIDASHI_AT_NPC, STRING("a"), STRING("me"), 0, 0, 0, 0, 0, 0, 0, 60)
                WAIT_MS(1200)
                CALL(evt_msg_print, 0, STRING("stg1_nok_66"), 0, 0)
            END_IF
        END_IF
        CASE_LESS(STG1_07_KOOPS_JOINS)
        CALL(evt_msg_print, 0, STRING("stg1_nok_66_01"), 0, 0)
        CASE_LESS(STG1_17_FATHER_SON_REUNITED)
        IF_INT_EQUAL(GSWF_NOK_KOOPIE_KOO_BLESSING_DONE, 0)
            CALL(evt_mario_get_party, LW(0))
            IF_INT_EQUAL(LW(0), 2)
                SET_VALUE(GSWF_NOK_KOOPIE_KOO_BLESSING_DONE, 1)
                // Koops is in the party: she apologizes to him and hops away.
                CALL(evt_msg_print, 0, STRING("stg1_nok_67"), 0, 0)
                CALL(evt_msg_print, 0, STRING("stg1_nok_68"), 0, 0)
                CALL(evt_npc_status_onoff, 1, STRING("me"), NPC_STATUS_FACE_MOVE_DIR)
                CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_NO_HIT | NPC_FLAG_GRAVITY)
                CALL(evt_npc_move_position, STRING("me"), -85, -300, 0, FLOAT(60.0f), 1)
                CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_NO_HIT | NPC_FLAG_GRAVITY)
                ELSE
                CALL(evt_msg_print, 0, STRING("stg1_nok_70"), 0, 0)
            END_IF
            ELSE
            CALL(evt_mario_get_party, LW(0))
            IF_INT_EQUAL(LW(0), 2)
                CALL(evt_msg_print, 0, STRING("stg1_nok_69"), 0, 0)
                ELSE
                CALL(evt_msg_print, 0, STRING("stg1_nok_70"), 0, 0)
            END_IF
        END_IF
    END_IF
    CASE_LESS(STG2_00_BOGGLY_WOODS)
    CALL(evt_mario_get_party, LW(0))
    IF_INT_EQUAL(LW(0), 2)
        // Chapter 2 on: she cheers Koops on and asks Mario to look after him.
        CALL(evt_msg_print, 0, STRING("stg1_nok_71"), 0, 0)
        ELSE
        CALL(evt_msg_print, 0, STRING("stg1_nok_72"), 0, 0)
    END_IF
    CASE_LESS(STG4_00_WELCOME_TO_STAGE_4)
    CALL(evt_mario_get_party, LW(0))
    IF_INT_EQUAL(LW(0), 2)
        CALL(evt_msg_print, 0, STRING("stg1_nok_73"), 0, 0)
        ELSE
        CALL(evt_msg_print, 0, STRING("stg1_nok_74"), 0, 0)
    END_IF
    CASE_LESS(STG7_00_START)
    CALL(evt_mario_get_party, LW(0))
    IF_INT_EQUAL(LW(0), 2)
        CALL(evt_msg_print, 0, STRING("stg1_nok_75"), 0, 0)
        ELSE
        CALL(evt_msg_print, 0, STRING("stg1_nok_76"), 0, 0)
    END_IF
    CASE_LESS_OR_EQUAL(STG8_END_AFTER_THE_BATTLE)
    CALL(evt_mario_get_party, LW(0))
    IF_INT_EQUAL(LW(0), 2)
        // The late game: she wonders whether Koops has changed.
        CALL(evt_msg_print, 0, STRING("stg1_nok_77"), 0, 0)
        ELSE
        CALL(evt_msg_print, 0, STRING("stg1_nok_78"), 0, 0)
    END_IF
    CASE_DEFAULT
#if VERSION == VERSION_US // PAL has the JP line
    CALL(evt_mario_get_party, LW(0))
    IF_INT_EQUAL(LW(0), 2)
        CALL(evt_msg_print, 0, STRING("stg1_nok_77"), 0, 0)
        ELSE
        CALL(evt_msg_print, 0, STRING("stg1_nok_78"), 0, 0)
    END_IF
#else
    CALL(evt_msg_print, 0, STRING("stg1_nok_78_01"), 0, 0)
#endif
    END_SWITCH
    RETURN
EVT_END()

/**
 * Talk script of the first dancer (踊り子１, the red one):
 * before the Gold Fuzzy the three dancers talk in turn (stg1_nok_34, 35, 36),
 * afterwards they use the second set (stg1_nok_36_01 to 03).
 */
static EVT_BEGIN(odoriko_1_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG1_06_01_GOLD_FUZZY)
        CALL(evt_msg_print, 0, STRING("stg1_nok_34"), 0, STRING("踊り子１"))
        CALL(evt_msg_print, 0, STRING("stg1_nok_35"), 0, STRING("踊り子２"))
        CALL(evt_msg_print, 0, STRING("stg1_nok_36"), 0, STRING("踊り子３"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg1_nok_36_01"), 0, STRING("踊り子１"))
        CALL(evt_msg_print, 0, STRING("stg1_nok_36_02"), 0, STRING("踊り子２"))
        CALL(evt_msg_print, 0, STRING("stg1_nok_36_03"), 0, STRING("踊り子３"))
    END_SWITCH
    RETURN
EVT_END()

/**
 * Entry of the first dancer (踊り子１, model c_dance_r) at (27, 0, 370) facing 270,
 * with her talk script and the move/talk animations.
 */
static EVT_BEGIN(odoriko_1_init)
    SET_VALUE(LW(0), STRING("踊り子１"))
    CALL(evt_npc_entry, LW(0), STRING("c_dance_r"))
    CALL(evt_npc_set_tribe, LW(0), STRING("踊り子"))
    CALL(evt_npc_change_interrupt, LW(0), 6, PTR(odoriko_1_talk))
    CALL(evt_npc_flag_onoff, 1, LW(0), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_set_ry, LW(0), 270)
    CALL(evt_npc_set_position, LW(0), 27, 0, 370)
    RETURN
EVT_END()

/** Talk script of the second dancer (踊り子２, pink): the same three-dancer conversation as odoriko_1_talk. */
static EVT_BEGIN(odoriko_2_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG1_06_01_GOLD_FUZZY)
        CALL(evt_msg_print, 0, STRING("stg1_nok_34"), 0, STRING("踊り子１"))
        CALL(evt_msg_print, 0, STRING("stg1_nok_35"), 0, STRING("踊り子２"))
        CALL(evt_msg_print, 0, STRING("stg1_nok_36"), 0, STRING("踊り子３"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg1_nok_36_01"), 0, STRING("踊り子１"))
        CALL(evt_msg_print, 0, STRING("stg1_nok_36_02"), 0, STRING("踊り子２"))
        CALL(evt_msg_print, 0, STRING("stg1_nok_36_03"), 0, STRING("踊り子３"))
    END_SWITCH
    RETURN
EVT_END()

/**
 * Entry of the second dancer (踊り子２, model c_dance_p) at (67, 0, 340) facing 270,
 * with her talk script and the move/talk animations.
 */
static EVT_BEGIN(odoriko_2_init)
    SET_VALUE(LW(0), STRING("踊り子２"))
    CALL(evt_npc_entry, LW(0), STRING("c_dance_p"))
    CALL(evt_npc_set_tribe, LW(0), STRING("踊り子ピンク"))
    CALL(evt_npc_change_interrupt, LW(0), 6, PTR(odoriko_2_talk))
    CALL(evt_npc_flag_onoff, 1, LW(0), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_set_ry, LW(0), 270)
    CALL(evt_npc_set_position, LW(0), 67, 0, 340)
    RETURN
EVT_END()

/** Talk script of the third dancer (踊り子３, yellow-green): the same three-dancer conversation as odoriko_1_talk. */
static EVT_BEGIN(odoriko_3_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG1_06_01_GOLD_FUZZY)
        CALL(evt_msg_print, 0, STRING("stg1_nok_34"), 0, STRING("踊り子１"))
        CALL(evt_msg_print, 0, STRING("stg1_nok_35"), 0, STRING("踊り子２"))
        CALL(evt_msg_print, 0, STRING("stg1_nok_36"), 0, STRING("踊り子３"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg1_nok_36_01"), 0, STRING("踊り子１"))
        CALL(evt_msg_print, 0, STRING("stg1_nok_36_02"), 0, STRING("踊り子２"))
        CALL(evt_msg_print, 0, STRING("stg1_nok_36_03"), 0, STRING("踊り子３"))
    END_SWITCH
    RETURN
EVT_END()

/**
 * Entry of the third dancer (踊り子３, model c_dance_k) at (85, 0, 390) facing 270,
 * with her talk script and the move/talk animations.
 */
static EVT_BEGIN(odoriko_3_init)
    SET_VALUE(LW(0), STRING("踊り子３"))
    CALL(evt_npc_entry, LW(0), STRING("c_dance_k"))
    CALL(evt_npc_set_tribe, LW(0), STRING("踊り子黄緑"))
    CALL(evt_npc_change_interrupt, LW(0), 6, PTR(odoriko_3_talk))
    CALL(evt_npc_flag_onoff, 1, LW(0), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_set_ry, LW(0), 270)
    CALL(evt_npc_set_position, LW(0), 85, 0, 390)
    RETURN
EVT_END()

/** NPCs of the east village: the mayor, the gate guard, Koops, Koops's father and Koopie Koo. */
static NpcSetupInfo npcEnt[] = {
    {
        /*"ノコノコG"*/"\x83\x6D\x83\x52\x83\x6D\x83\x52\x47", // name
        0, 0, // flags..reactionFlags
        npc_init_evt, // initEvent
        nokonoko_G_regl, // regularEvent
        nokonoko_G_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ノコノコH"*/"\x83\x6D\x83\x52\x83\x6D\x83\x52\x48", // name
        0, 0, // flags..reactionFlags
        npc_init_evt, // initEvent
        nokonoko_H_regl, // regularEvent
        nokonoko_H_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ノコノコI"*/"\x83\x6D\x83\x52\x83\x6D\x83\x52\x49", // name
        0, 0, // flags..reactionFlags
        npc_init_evt, // initEvent
        nokonoko_I_regl, // regularEvent
        nokonoko_I_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ノコノコK"*/"\x83\x6D\x83\x52\x83\x6D\x83\x52\x4B", // name
        0, 0, // flags..reactionFlags
        npc_init_evt, // initEvent
        NULL, // regularEvent
        nokonoko_K_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"村長"*/"\x91\xBA\x92\xB7", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        npc_init_evt, // initEvent
        NULL, // regularEvent
        boss_noko_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"門番ノコノコ"*/"\x96\xE5\x94\xD4\x83\x6D\x83\x52\x83\x6D\x83\x52", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        monban_init, // initEvent
        NULL, // regularEvent
        monban_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ノコタロウ"*/"\x83\x6D\x83\x52\x83\x5E\x83\x8D\x83\x45", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        nokotarou_init, // initEvent
        NULL, // regularEvent
        nokotarou_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ノコタロウ父"*/"\x83\x6D\x83\x52\x83\x5E\x83\x8D\x83\x45\x95\x83", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        nokopapa_init, // initEvent
        NULL, // regularEvent
        nokopapa_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ノコリン"*/"\x83\x6D\x83\x52\x83\x8A\x83\x93", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        nokorin_init, // initEvent
        NULL, // regularEvent
        nokorin_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

/**
 * Doors and exits of nok_01: w_bero to nok_00; e_bero to hei_02 (Petal Meadows); dokan_1 to tik_16 (Rogueport
 * Sewers).
 */
static BeroEntry bero_entry_data[] = {
    {
        "w_bero", // hitName
        0, 0, // type..sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "nok_00", // nextMapName
        "e_bero", // nextBeroName
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
        "hei_02", // nextMapName
        "w_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
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
        NULL, // exitEvent
        "tik_16", // nextMapName
        "dokan_2", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

/** NPCs inside the mayor's house: the mayor. */
const char* son_npc[] = {
    /*"村長"*/"\x91\xBA\x92\xB7",
    NULL,
};

/** Map objects inside the mayor's house: the spinning floor kururin3. */
const char* son_mobj[] = {
    "kururin3",
    NULL,
};

/** Items inside the mayor's house: the Turtley Leaf iri_08 of request 8. */
const char* son_item[] = {
    "iri_08",
    NULL,
};

/** Door of the mayor's house: outside hit area A_son_doa, the inside room and its NPCs and objects. */
HouseParam son_data = {
    1, // type
    9, // hitInKind
    "A_son_doa", // hitIn
    9, // hitOutKind
    "A_son_in_doa", // hitOut
    {"S_son_open", "S_son_doa_open", "S_son_close", "S_son_doa_close"}, // anim
    "S_son_mae", // outside
    "S_son_doa1", // door
    {"S_son_in", "A_son_in"}, // inside
    {"S_son_in", "A_son_in"}, // initInside
    son_npc, // npcs
    NULL, // noclip
    son_mobj, // mobjs
    son_item, // items
};

/** NPCs inside Koops's house: Koops. */
const char* taro_npc[] = {
    /*"ノコタロウ"*/"\x83\x6D\x83\x52\x83\x5E\x83\x8D\x83\x45",
    NULL,
};

/** Map group whose bounding-box culling is off while Mario is inside Koops's house: the room S_taro_in. */
const char* taro_map[] = {
    "S_taro_in",
    NULL,
};

/** Map objects inside Koops's house: the box. */
const char* taro_mobj[] = {
    "box",
    NULL,
};

/** Items inside Koops's house: item_01. */
const char* taro_item[] = {
    "item_01",
    NULL,
};

/** Door of Koops's house: outside hit area A_taro_doa, the inside room and its NPCs and objects. */
HouseParam taro_data = {
    1, // type
    9, // hitInKind
    "A_taro_doa", // hitIn
    9, // hitOutKind
    "A_taro_in_doa", // hitOut
    {"S_taro_open", "S_taro_doa_open", "S_taro_close", "S_taro_doa_close"}, // anim
    "S_taro_mae", // outside
    "S_taro_doa", // door
    {"S_taro_in", "A_taro_in"}, // inside
    {"S_taro_in", "A_taro_in"}, // initInside
    taro_npc, // npcs
    taro_map, // noclip
    taro_mobj, // mobjs
    taro_item, // items
};

/** NPCs inside the house: Koopa K. */
static const char* ie_npc[] = {
    /*"ノコノコK"*/"\x83\x6D\x83\x52\x83\x6D\x83\x52\x4B",
    NULL,
};

/** Map group whose bounding-box culling is off while Mario is inside this house: S_ie_sit. */
static const char* ie_map[] = {
    "S_ie_sit",
    NULL,
};

/** Door of a village house in the east half: outside hit area A_ie_doa, the inside room and its NPCs. */
static HouseParam ie_data = {
    1, // type
    9, // hitInKind
    "A_ie_doa", // hitIn
    9, // hitOutKind
    "S_ie_in_doa", // hitOut
    {"S_ie_open", "S_ie_doa_open", "S_ie_close", "S_ie_doa_close"}, // anim
    "S_ie_mae", // outside
    "S_ie_doa", // door
    {"S_ie_in", "A_ie_in"}, // inside
    {"S_ie_in", "A_ie_in"}, // initInside
    ie_npc, // npcs
    ie_map, // noclip
    NULL, NULL, // mobjs..items
};

static HitDamageReturn hitDmgRetPoints[] = {
    {"mod_00", {0.0f, -1000.0f, 0.0f}},
    {"mod_01", {0.0f, -1000.0f, 0.0f}},
    {"mod_02", {0.0f, -1000.0f, 0.0f}},
    {"mod_03", {0.0f, -1000.0f, 0.0f}},
    {"mod_04", {0.0f, -1000.0f, 0.0f}},
    {"mod_05", {0.0f, -1000.0f, 0.0f}},
    {"mod_06", {0.0f, -1000.0f, 0.0f}},
    {"mod_07", {0.0f, -1000.0f, 0.0f}},
    {"mod_08", {0.0f, -1000.0f, 0.0f}},
    {"mod_09", {0.0f, -1000.0f, 0.0f}},
    {"mod_010", {0.0f, -1000.0f, 0.0f}},
    {"A_hasi1", {0.0f, -1000.0f, 0.0f}},
    {0},
};

/**
 * Cutscene after Mario returns from Hooktail: Mario, Koops's father, Koopie Koo,
 * Kroop and Goombella gather in Petalburg and Koops says goodbye.
 * Both partners are walked off and deleted, the story becomes STG1_20_KOOPS_LEAVES,
 * the party is cleared and control is returned (JP and later also record the respawn position).
 */
EVT_BEGIN(nokotarou_tabidachi)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_cam3d_evt_set, 0, 142, 453, 0, 47, -86, 0, EASING_SINE_IN_OUT)
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_bero_exec_wait, 65536)
    SUSPEND_TYPES_OTHER(4)
    CALL(evt_fade_end_wait)
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_set, 20, 100, 264, 20, 40, -86, 1000, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg1_nok_85"), 0, STRING("ノコタロウ父"))
    CALL(evt_msg_print, 0, STRING("stg1_nok_86"), 0, STRING("ノコタロウ"))
    CALL(evt_msg_print, 0, STRING("stg1_nok_87"), 0, STRING("ノコタロウ父"))
    CALL(evt_cam3d_evt_set, 77, 100, 264, 77, 40, -86, 400, EASING_SINE_IN_OUT)
    WAIT_MS(400)
    CALL(evt_msg_print, 0, STRING("stg1_nok_88"), 0, STRING("ノコリン"))
    CALL(evt_msg_print, 0, STRING("stg1_nok_89"), 0, STRING("村長"))
    CALL(evt_cam3d_evt_set, 0, 100, 264, 0, 40, -86, 400, EASING_SINE_IN_OUT)
    WAIT_MS(400)
    CALL(evt_msg_print, 0, STRING("stg1_nok_90"), 0, STRING("ノコタロウ"))
    CALL(evt_npc_status_onoff, 1, STRING("ノコタロウ"), NPC_STATUS_FACE_MOVE_DIR)
    CALL(evt_npc_flag_onoff, 1, STRING("ノコタロウ"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_move_position, STRING("ノコタロウ"), -25, -60, 0, FLOAT(60.0f), 1)
    WAIT_MS(200)
    CALL(evt_npc_flag_onoff, 1, STRING("ノコタロウ"), NPC_FLAG_TALK_POSE)
    CALL(evt_msg_print, 0, STRING("stg1_nok_91"), 0, STRING("ノコタロウ"))
    CALL(evt_npc_status_onoff, 1, STRING("クリスチーヌ"), NPC_STATUS_FACE_MOVE_DIR)
    CALL(evt_npc_flag_onoff, 1, STRING("クリスチーヌ"), NPC_FLAG_NO_HIT | NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_move_position, STRING("クリスチーヌ"), -65, -40, 0, FLOAT(60.0f), 1)
    WAIT_MS(200)
    CALL(evt_npc_flag_onoff, 1, STRING("クリスチーヌ"), NPC_FLAG_TALK_POSE)
    CALL(evt_msg_print, 0, STRING("stg1_nok_92"), 0, STRING("クリスチーヌ"))
    CALL(evt_cam3d_evt_set, -40, 100, 264, -40, 40, -86, 400, EASING_SINE_IN_OUT)
    WAIT_MS(400)
    CALL(evt_set_dir_to_target, STRING("クリスチーヌ"), STRING("mario"))
    CALL(evt_msg_print, 0, STRING("stg1_nok_94"), 0, STRING("クリスチーヌ"))
    CALL(evt_msg_print, 0, STRING("stg1_nok_95"), 0, STRING("ノコタロウ"))
    CALL(evt_set_dir_to_target, STRING("クリスチーヌ"), STRING("ノコタロウ"))
    CALL(evt_msg_print, 0, STRING("stg1_nok_96"), 0, STRING("クリスチーヌ"))
    IF_INT_EQUAL(GSWF_GON_11_KOOPS_SWAPPED_IN, 1)
        SET_VALUE(LW(0), 2)
        SET_VALUE(LW(1), 1)
        SET_VALUE(LW(2), STRING("ノコタロウ"))
        SET_VALUE(LW(3), STRING("クリスチーヌ"))
        ELSE
        SET_VALUE(LW(0), 1)
        SET_VALUE(LW(1), 2)
        SET_VALUE(LW(2), STRING("クリスチーヌ"))
        SET_VALUE(LW(3), STRING("ノコタロウ"))
    END_IF
    CALL(evt_mario_set_party_pos, 0, LW(0), 0, -1000, 0)
    CALL(evt_npc_get_dir, LW(2), LW(10))
    CALL(evt_party_set_dir, 0, LW(10), 0)
    CALL(evt_npc_get_position, LW(2), LW(10), LW(11), LW(12))
    CALL(evt_party_set_pos, 0, LW(10), LW(11), LW(12))
    CALL(evt_npc_set_position, LW(2), 0, -1000, 0)
    CALL(evt_npc_delete, LW(2))
    CALL(evt_mario_goodbye_party, 0)
    WAIT_MS(1000)
    CALL(evt_mario_set_party_pos, 0, LW(1), 0, -1000, 0)
    CALL(evt_npc_get_dir, LW(3), LW(10))
    CALL(evt_party_set_dir, 0, LW(10), 0)
    CALL(evt_npc_get_position, LW(3), LW(10), LW(11), LW(12))
    CALL(evt_party_set_pos, 0, LW(10), LW(11), LW(12))
    CALL(evt_npc_set_position, LW(3), 0, -1000, 0)
    CALL(evt_npc_delete, LW(3))
    SET_VALUE(GSW_STORY_PROGRESS, STG1_20_KOOPS_LEAVES)
    RESUME_TYPES_OTHER(4)
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
#if VERSION > VERSION_DEMO_US
    CALL(evt_bero_set_mario_bottomless_respawn_pos_to_current_mario_pos)
#endif
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_party_cont_onoff, 0, 1)
    CALL(evt_party_run, 0)
    RETURN
EVT_END()

/**
 * Set-up for nokotarou_tabidachi: creates the Goombella NPC (クリスチーヌ), positions Mario, Koops, his father,
 * Koopie Koo and the mayor in a line, removes the active partner and starts the scene.
 */
EVT_BEGIN(nokotarou_tabidachi_init)
    CALL(evt_npc_entry, STRING("クリスチーヌ"), STRING("c_pkr"))
    CALL(evt_npc_set_anim, STRING("クリスチーヌ"), STRING("PKR_S_1"))
    CALL(evt_npc_set_tribe, STRING("クリスチーヌ"), STRING("クリスチーヌ"))
    CALL(evt_npc_set_position, STRING("クリスチーヌ"), -100, 0, -65)
    CALL(evt_npc_set_ry, STRING("クリスチーヌ"), 90)
    CALL(evt_npc_flag_onoff, 1, STRING("クリスチーヌ"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_mario_set_pos, -60, 0, -60)
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_npc_set_position, STRING("ノコタロウ"), 0, 0, -60)
    CALL(evt_npc_set_ry, STRING("ノコタロウ"), 90)
    CALL(evt_npc_flag_onoff, 1, STRING("ノコタロウ"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_set_position, STRING("ノコタロウ父"), 40, 0, -70)
    CALL(evt_npc_set_ry, STRING("ノコタロウ父"), 270)
    CALL(evt_npc_set_position, STRING("ノコリン"), 65, 0, -60)
    CALL(evt_npc_set_ry, STRING("ノコリン"), 270)
    CALL(evt_npc_set_position, STRING("村長"), 90, 0, -50)
    CALL(evt_npc_set_ry, STRING("村長"), 270)
    CALL(evt_mario_kill_party, 0)
    SPAWN_EVENT(PTR(nokotarou_tabidachi))
    RETURN
EVT_END()


/**
 * Script helper: sets the view matrix of the yami (darkness) overlay (envSetYamiView) to look from (-100 + d,
 * 100, 0) at (d, 0, 0), with d the float args[0].
 * sekai_yami_evt sweeps d from -600 to 600 to pass the darkness across the village;
 * minnnanokoe_evt sets it to 600.
 */
static USER_FUNC(yami_view) {
    f32 z = evtGetFloat(event, event->args[0]);
    Mtx mtx;

    C_MTXLookAt(mtx, &(Vec){-100.0f + z, 100.0f, 0.0f}, &(Vec){0.0f, 1.0f, 0.0f}, &(Vec){z, 0.0f, 0.0f});
    envSetYamiView(mtx);
    return EVT_RETURN_DONE;
}

/**
 * Reaction of one villager (name in LW(0)) in the darkening-world cutscene:
 * waits until the darkness sweep (GW(0), set by sekai_yami_evt) passes his x position, stops his regular script,
 * shows a surprise bubble and then turns around at random intervals (100 to 499 ms) forever.
 */
static EVT_BEGIN(sekai_yami_npc)
    LOOP(0)
        CALL(evt_npc_get_position, LW(0), LW(1), LW(2), LW(3))
        IF_INT_GREATER_OR_EQUAL(GW(0), LW(1))
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_npc_get_reglid, LW(0), LW(1))
    DELETE_EVENT(LW(1))
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_NPC, STRING(""), LW(0), 0, 0, 0, 0, 0, 0, 0, 48)
    WAIT_MS(1000)
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

static EVT_BEGIN(sekai_yami_evt)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_mario_set_pos, 0, -1000, 0)
    CALL(evt_party_set_pos, 0, 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("村長"), -250, 70, -125)
    CALL(evt_npc_set_ry, STRING("村長"), 270)
    CALL(evt_npc_get_reglid, STRING("ノコノコG"), LW(0))
    DELETE_EVENT(LW(0))
    CALL(evt_npc_get_reglid, STRING("ノコノコH"), LW(0))
    DELETE_EVENT(LW(0))
    CALL(evt_npc_get_reglid, STRING("ノコノコI"), LW(0))
    DELETE_EVENT(LW(0))
    CALL(evt_cam3d_evt_set, 650, 90, 467, 13, 90, -17, 0, EASING_SINE_IN_OUT)
    CALL(evt_seq_wait, SEQ_GAME)
    WAIT_MS(1000)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 255, 64, 2000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_bg_set_color, LW(0), LW(0), LW(0), 255)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    WAIT_MS(2000)
    CALL(evt_map_set_tevcallback, 1, PTR(envSetYami))
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S"), MAP_OBJ_TEV_CALLBACK(1))
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S"), MAP_OBJ_FLAG_DISPLAY_LIST)
    CALL(anim_tevcallback, STRING("ノコノコG"), 1)
    CALL(anim_tevcallback, STRING("ノコノコH"), 1)
    CALL(anim_tevcallback, STRING("ノコノコI"), 1)
    CALL(anim_tevcallback, STRING("村長"), 1)
    CALL(anim_tevcallback, STRING("ノコリン"), 1)
    SET_VALUE(GW(0), -600)
    SET_VALUE(LW(0), STRING("ノコノコG"))
    SPAWN_EVENT(PTR(sekai_yami_npc))
    SET_VALUE(LW(0), STRING("ノコノコH"))
    SPAWN_EVENT(PTR(sekai_yami_npc))
    SET_VALUE(LW(0), STRING("ノコノコI"))
    SPAWN_EVENT(PTR(sekai_yami_npc))
    SET_VALUE(LW(0), STRING("村長"))
    SPAWN_EVENT_GET_ID(PTR(sekai_yami_npc), GW(1))
    SET_VALUE(LW(0), STRING("ノコリン"))
    SPAWN_EVENT(PTR(sekai_yami_npc))
    CALL(evt_snd_sfxon, STRING("SFX_STG8_WORLD_DARK1"), 0)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, -600, 600, 5000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(yami_view, LW(0))
        SET_VALUE(GW(0), LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_set, 43, 120, 141, -117, 120, 20, 1000, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    DELETE_EVENT(GW(1))
    WAIT_FRAMES(1)
    CALL(evt_npc_set_ry, STRING("村長"), 270)
    WAIT_FRAMES(1)
    CALL(evt_npc_wait_pera, STRING("村長"))
    CALL(evt_msg_print, 0, STRING("stg8_dark_01"), 0, STRING("村長"))
    WAIT_MS(1000)
    CALL(evt_fade_set_mapchange_type, 1, FADE_OUT_BLACK, 300, FADE_IN_BLACK, 300)
    CALL(evt_bero_mapchange, STRING("mri_00"), STRING("sekai_yami1"))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(minnnanokoe_evt)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_mario_set_pos, 0, -1000, 0)
    CALL(evt_party_set_pos, 0, 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("村長"), -100, 0, 50)
    CALL(evt_npc_set_ry, STRING("村長"), 270)
    CALL(evt_npc_set_position, STRING("ノコタロウ父"), 100, 0, -150)
    CALL(evt_npc_set_ry, STRING("ノコタロウ父"), 270)
    CALL(evt_npc_set_position, STRING("ノコリン"), 25, 0, -200)
    CALL(evt_npc_set_ry, STRING("ノコリン"), 270)
    CALL(evt_npc_set_position, STRING("ノコノコG"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("ノコノコH"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("ノコノコI"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("ノコノコK"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("門番ノコノコ"), 0, -1000, 0)
    CALL(evt_map_set_tevcallback, 1, PTR(envSetYami))
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S"), MAP_OBJ_TEV_CALLBACK(1))
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S"), MAP_OBJ_FLAG_DISPLAY_LIST)
    CALL(anim_tevcallback, STRING("村長"), 1)
    CALL(anim_tevcallback, STRING("ノコタロウ父"), 1)
    CALL(anim_tevcallback, STRING("ノコリン"), 1)
    CALL(yami_view, 600)
    CALL(evt_bg_set_color, 64, 64, 64, 255)
    CALL(evt_cam3d_evt_set, 562, 320, 474, 0, 100, -50, 0, EASING_SINE_IN_OUT)
    CALL(starstone_entry, STRING("starstone"), 0, 450, -50, 1)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_hasi"), MAP_OBJ_FLAG_CLIP)
    CALL(evt_seq_wait, SEQ_GAME)
    WAIT_MS(500)
    SET_VALUE(GW(0), 450)
    BEGIN_INLINE_EVENT
        LOOP(0)
            CALL(starstone_set_pos, STRING("starstone"), 0, GW(0), -50)
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
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 450, 50, 3000)
    LABEL(0)
    CALL(evt_sub_intpl_msec_get_value)
    SET_VALUE(GW(0), LW(0))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    SET_VALUE(GF(0), 0)
    BEGIN_INLINE_EVENT
        LOOP(0)
            CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 5000, 5500, 500)
            LABEL(1)
            CALL(evt_sub_intpl_msec_get_value)
            DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
            SET_FLOAT_VALUE(GW(0), LW(0))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 1)
                GOTO(1)
            END_IF
            CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 5500, 5000, 500)
            LABEL(2)
            CALL(evt_sub_intpl_msec_get_value)
            DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
            SET_FLOAT_VALUE(GW(0), LW(0))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 1)
                GOTO(2)
            END_IF
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_npc_set_ry, STRING("村長"), 90)
    WAIT_MS(500)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_NPC, STRING(""), STRING("村長"), 0, 0, 0, 0, 0, 0, 0, 60)
    WAIT_MS(1200)
    CALL(evt_msg_print, 0, STRING("stg8_las_148_03"), 0, STRING("村長"))
    CALL(evt_cam3d_evt_set, 211, 133, 145, 0, 50, -50, 2000, EASING_LINEAR)
    CALL(evt_npc_flag_onoff, 1, STRING("村長"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_move_position, STRING("村長"), -50, -50, 2000, 0, 0)
    CALL(evt_msg_print, 0, STRING("stg8_las_148_04"), 0, STRING("村長"))
    CALL(evt_npc_move_position, STRING("ノコタロウ父"), 25, -75, 2000, 0, 0)
    CALL(evt_msg_print, 0, STRING("stg8_las_148_05"), 0, STRING("ノコタロウ父"))
    CALL(evt_npc_move_position, STRING("ノコリン"), 0, -150, 1000, 0, 0)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg8_las_148_06"), 0, STRING("村長"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg8_las_148_07"), 0, STRING("ノコタロウ父"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg8_las_148_08"), 0, STRING("ノコリン"))
    CALL(evt_npc_set_ry, STRING("ノコタロウ父"), 90)
    WAIT_MS(500)
    CALL(evt_cam3d_evt_set, 186, 114, 34, 46, 60, -98, 1000, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg8_las_148_09"), 0, STRING("ノコリン"))
    CALL(evt_cam3d_evt_set, 562, 320, 474, 0, 100, -50, 6000, EASING_SINE_IN_OUT)
    WAIT_MS(3000)
    CALL(evt_fade_set_mapchange_type, 1, FADE_OUT_BLACK, 300, FADE_IN_BLACK, 300)
    CALL(evt_bero_mapchange, STRING("mri_00"), STRING("minnnanokoe"))
    RETURN
EVT_END()

static EVT_BEGIN(evt_switch_init)
    IF_INT_EQUAL(GSWF_TIK_16_NOK_PIPE_RAISED, 0)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_dakan"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("A_dokan"), 1, 0)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(e3_talk)
    CALL(evt_msg_print, 0, STRING("nok_01_for_E3"), 0, 0)
    CALL(evt_bero_mapchange, STRING("kpa_02"), 0)
    RETURN
EVT_END()

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


/**
 * Script helper: creates a star stone effect (effStarStoneEntry) at position args[1..3] (floats)
 * and registers it under the effect name args[0] so later helpers can find it.
 */
static USER_FUNC(starstone_entry) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    f32 x = evtGetFloat(event, args[1]);
    f32 y = evtGetFloat(event, args[2]);
    f32 z = evtGetFloat(event, args[3]);

    effSetName(effStarStoneEntry(0, x, y, z, 1.0f), name);
    return EVT_RETURN_DONE;
}

/** Script helper: moves the star stone effect named args[0] to the position args[1..3] (floats). */
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

/** Script helper: sets the Y rotation of the star stone effect named args[0] to args[1] (float, degrees). */
static USER_FUNC(starstone_set_ry) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    f32 ry = evtGetFloat(event, args[1]);

    ((EffStarStoneWork*)effNameToPtr(name)->userdata)->rot.y = ry;
    return EVT_RETURN_DONE;
}

/**
 * Creates Goombella (クリスチーヌ, model c_pkr) for the epilogue at (-200, 0, -65) facing 90, intangible,
 * and moves Koopa H off the map.
 */
static EVT_BEGIN(epilogue_npc_entry)
    CALL(evt_npc_entry, STRING("クリスチーヌ"), STRING("c_pkr"))
    CALL(evt_npc_set_tribe, STRING("クリスチーヌ"), STRING("クリスチーヌ"))
    CALL(evt_npc_set_position, STRING("クリスチーヌ"), -200, 0, -65)
    CALL(evt_npc_set_ry, STRING("クリスチーヌ"), 90)
    CALL(evt_npc_flag_onoff, 1, STRING("クリスチーヌ"), NPC_FLAG_NO_HIT | NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_set_position, STRING("ノコノコH"), 0, -1000, 0)
    RETURN
EVT_END()

static EVT_BEGIN(epilogue_evt_sub)
    CALL(evt_npc_set_anim, STRING("ノコタロウ"), STRING("PNK_T_1"))
    WAIT_MS(1000)
    CALL(evt_npc_set_anim, STRING("ノコタロウ"), STRING("PNK_S_1"))
    CALL(evt_npc_set_anim, STRING("ノコリン"), STRING("T_1"))
    WAIT_MS(1000)
    CALL(evt_npc_set_anim, STRING("ノコリン"), STRING("S_1"))
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING("クリスチーヌ"), -35, -65, 0, 50, 0)
    END_INLINE_EVENT
    WAIT_MS(700)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_NPC, STRING(""), STRING("ノコリン"), 0, 0, 0, 0, 0, 0, 0, 30)
    WAIT_MS(700)
    CALL(evt_npc_set_ry, STRING("ノコタロウ"), 270)
    WAIT_FRAMES(1)
    CALL(evt_npc_wait_pera, STRING("ノコタロウ"))
    CALL(evt_npc_set_anim, STRING("ノコタロウ"), STRING("PNK_O_1"))
    WAIT_MS(1000)
    SET_VALUE(GF(0), 1)
    BEGIN_INLINE_EVENT
        WAIT_MS(300)
        CALL(evt_npc_move_position, STRING("ノコリン"), 50, -50, 0, 80, 0)
        SET_VALUE(GF(0), 0)
    END_INLINE_EVENT
    CALL(evt_npc_move_position, STRING("ノコタロウ"), 35, -70, 0, 80, 0)
    WAIT_WHILE(GF(0))
    WAIT_MS(500)
    CALL(evt_npc_set_anim, STRING("クリスチーヌ"), STRING("PKR_T_1"))
    WAIT_MS(1500)
    CALL(evt_npc_set_anim, STRING("クリスチーヌ"), STRING("PKR_S_1"))
    CALL(evt_npc_set_anim, STRING("ノコタロウ"), STRING("PNK_T_1"))
    WAIT_MS(1500)
    CALL(evt_npc_set_anim, STRING("ノコタロウ"), STRING("PNK_S_1"))
    CALL(evt_npc_set_anim, STRING("ノコリン"), STRING("T_1"))
    BEGIN_INLINE_EVENT
        WAIT_MS(1000)
        CALL(evt_npc_move_position, STRING("ノコタロウ父"), 100, -100, 0, 40, 0)
        WAIT_MS(1000)
        CALL(evt_npc_flag_onoff, 1, STRING("村長"), NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_GRAVITY)
        CALL(evt_npc_move_position, STRING("村長"), -113, -172, 0, 40, 0)
        CALL(evt_npc_move_position, STRING("村長"), -90, -50, 0, 40, 0)
    END_INLINE_EVENT
    WAIT_MS(1500)
    CALL(evt_npc_set_anim, STRING("ノコリン"), STRING("S_1"))
    LOOP(0)
        CALL(evt_npc_set_anim, STRING("クリスチーヌ"), STRING("PKR_T_1"))
        WAIT_MS(1500)
        CALL(evt_npc_set_anim, STRING("クリスチーヌ"), STRING("PKR_S_1"))
        CALL(evt_npc_set_anim, STRING("ノコタロウ"), STRING("PNK_T_3"))
        WAIT_MS(1500)
        CALL(evt_npc_set_anim, STRING("ノコタロウ"), STRING("PNK_Y_1"))
        CALL(evt_npc_set_anim, STRING("ノコリン"), STRING("T_1"))
        WAIT_MS(1500)
        CALL(evt_npc_set_anim, STRING("ノコリン"), STRING("S_1"))
    END_LOOP
    RETURN
EVT_END()

/**
 * Koops's epilogue scene in Petalburg (STG8_END_AFTER_THE_BATTLE):
 * plays the village music, hides Mario, places Koops, Koopie Koo,
 * Koops's father and the mayor and runs epilogue_evt_sub.
 * The narration (ep_38) tells Mario that Koops lives peacefully with his dad and Koopie Koo
 * and wants to become mayor; then Flurrie's stage-play epilogue (btlgrp_nok_hei_ep, the only battle of nok.rel;
 * unit_act_mario) starts as a battle through Koopa H's NPC, and after it the scene fades out to tou_08.
 */
static EVT_BEGIN(epilogue_evt)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG1_NOK1"))
    CALL(evt_snd_bgm_scope, 0, 1)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_HIDE)
    CALL(evt_npc_set_position, STRING("ノコタロウ"), 75, 0, -70)
    CALL(evt_npc_set_ry, STRING("ノコタロウ"), 90)
    CALL(evt_npc_flag_onoff, 1, STRING("ノコタロウ"), NPC_FLAG_NO_HIT | NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_set_position, STRING("ノコリン"), 110, 0, -50)
    CALL(evt_npc_set_ry, STRING("ノコリン"), 270)
    CALL(evt_npc_flag_onoff, 1, STRING("ノコリン"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_set_position, STRING("ノコタロウ父"), 200, 0, -100)
    CALL(evt_npc_set_ry, STRING("ノコタロウ父"), 270)
    CALL(evt_npc_flag_onoff, 1, STRING("ノコタロウ父"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_set_position, STRING("村長"), -250, 75, -190)
    CALL(evt_npc_set_ry, STRING("村長"), 90)
    CALL(evt_npc_flag_onoff, 1, STRING("村長"), NPC_FLAG_NO_HIT)
    CALL(evt_cam3d_evt_set, 0, 133, 517, 0, 50, -77, 0, EASING_SINE_IN_OUT)
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_npc_flag_onoff, 0, STRING("ノコタロウ"), NPC_FLAG_INSTANT_TURN)
    SPAWN_EVENT(PTR(epilogue_evt_sub))
    CALL(evt_fade_end_wait)
    WAIT_MS(2000)
    CALL(evt_msg_print, 0, STRING("ep_38"), 0, 0)
    WAIT_MS(1000)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_ZAKO_BATTLE1"))
    CALL(evt_npc_set_battle_info, STRING("ノコノコH"), 0)
    CALL(evt_npc_battle_start, STRING("ノコノコH"))
    CALL(evt_npc_wait_battle_end)
    CALL(evt_map_set_blend, 0, 0, 0, 0, 255)
    CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 3000)
    CALL(evt_snd_envoff_f, PSND_FADEOUT, 3000)
    CALL(evt_fade_set_mapchange_type, 1, FADE_OUT_BLACK, 1, FADE_IN_BLACK, 900)
    CALL(evt_bero_mapchange, STRING("tou_08"), STRING(""))
    RETURN
EVT_END()

/**
 * Script of bush 4 (S_kamehapa_04, from kusa_data):
 * when it is the bush the map init chose at random for this visit (LSW(1) = 0)
 * and its leaf is not already out (evt_item_delete_check),
 * a Turtley Leaf launches out of it at (-407, 70, -160) and sets LSWF(4) when picked up.
 * In the US demo and earlier it appears at z = -148 instead.
 */
static EVT_BEGIN(evt_kusa_4)
    IF_INT_NOT_EQUAL(LSW_NOK_01_LEAF_BUSH, 0)
        RETURN
    END_IF
    CALL(evt_item_delete_check, STRING("item_kusa_4"), LW(0))
    IF_INT_EQUAL(LW(0), 1)
#if VERSION <= VERSION_DEMO_US
        CALL(evt_item_entry, STRING("item_kusa_4"), ITEM_TURTLEY_LEAF, -407, 70, -148, ITEM_TYPE_SCATTER_NO_PICKUP, LSWF(4), 0)
#else
        CALL(evt_item_entry, STRING("item_kusa_4"), ITEM_TURTLEY_LEAF, -407, 70, -160, ITEM_TYPE_SCATTER_NO_PICKUP, LSWF_NOK_01_TURTLEY_LEAF, 0)
#endif
    END_IF
    RETURN
EVT_END()

/**
 * Script of bush 5 (S_kamehapa_05): launches the Turtley Leaf at (-452, 70, -160) when LSW(1) is 1;
 * otherwise as evt_kusa_4.
 */
static EVT_BEGIN(evt_kusa_5)
    IF_INT_NOT_EQUAL(LSW_NOK_01_LEAF_BUSH, 1)
        RETURN
    END_IF
    CALL(evt_item_delete_check, STRING("item_kusa_5"), LW(0))
    IF_INT_EQUAL(LW(0), 1)
#if VERSION <= VERSION_DEMO_US
        CALL(evt_item_entry, STRING("item_kusa_5"), ITEM_TURTLEY_LEAF, -452, 70, -148, ITEM_TYPE_SCATTER_NO_PICKUP, LSWF(4), 0)
#else
        CALL(evt_item_entry, STRING("item_kusa_5"), ITEM_TURTLEY_LEAF, -452, 70, -160, ITEM_TYPE_SCATTER_NO_PICKUP, LSWF_NOK_01_TURTLEY_LEAF, 0)
#endif
    END_IF
    RETURN
EVT_END()

/**
 * Script of bush 6 (S_kamehapa_06): launches the Turtley Leaf at (-362, 70, -160) when LSW(1) is 2;
 * otherwise as evt_kusa_4.
 */
static EVT_BEGIN(evt_kusa_6)
    IF_INT_NOT_EQUAL(LSW_NOK_01_LEAF_BUSH, 2)
        RETURN
    END_IF
    CALL(evt_item_delete_check, STRING("item_kusa_6"), LW(0))
    IF_INT_EQUAL(LW(0), 1)
#if VERSION <= VERSION_DEMO_US
        CALL(evt_item_entry, STRING("item_kusa_6"), ITEM_TURTLEY_LEAF, -362, 70, -148, ITEM_TYPE_SCATTER_NO_PICKUP, LSWF(4), 0)
#else
        CALL(evt_item_entry, STRING("item_kusa_6"), ITEM_TURTLEY_LEAF, -362, 70, -160, ITEM_TYPE_SCATTER_NO_PICKUP, LSWF_NOK_01_TURTLEY_LEAF, 0)
#endif
    END_IF
    RETURN
EVT_END()

/**
 * Bushes of nok_01 that Mario can check for hidden things (3 in all); A_kamehapa_04 runs evt_kusa_4,
 * A_kamehapa_05 runs evt_kusa_5, A_kamehapa_06 runs evt_kusa_6.
 */
static KusaAccess kusa_data[] = {
    {0, "A_kamehapa_04", "S_kamehapa_04", evt_kusa_4},
    {0, "A_kamehapa_05", "S_kamehapa_05", evt_kusa_5},
    {0, "A_kamehapa_06", "S_kamehapa_06", evt_kusa_6},
    {4, NULL, NULL, NULL},
};

/**
 * Map init script for nok_01: loads the entrances and music (the quiet village theme,
 * or the dark chapter 8 finale tracks when entering through "sekai_yami1" or "minnnanokoe"), spawns the NPCs,
 * the dancers (from Petal Meadows until the father and son reunite), the house doors, the gate state,
 * grass and the star piece panel, the Mega Rush P badge and the Turtley Leaf, butterflies, and the pipe switch.
 * Entering through "nokotarou" heals Mario and the partners and starts the Koops goodbye scene;
 * the chapter 8 finale and epilogue scenes are started here too.
 */
EVT_BEGIN(nok_01_init_evt)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    // Register the exits, then pick the music: the village theme or the dark finale tracks.
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    SPAWN_EVENT(PTR(evt_switch_init))
    SET_VALUE(LF(0), 0)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG8_15_GRODUS)
        CALL(evt_bero_get_entername, LW(0))
        IF_STR_EQUAL(LW(0), STRING("sekai_yami1"))
            SET_VALUE(LF(0), 1)
        END_IF
        IF_STR_EQUAL(LW(0), STRING("minnnanokoe"))
            SET_VALUE(LF(0), 1)
        END_IF
    END_IF
    IF_INT_EQUAL(LF(0), 1)
        CALL(evt_bero_get_entername, LW(0))
        IF_STR_EQUAL(LW(0), STRING("sekai_yami1"))
            CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_STG8_WITCH"))
            CALL(evt_snd_envoff, PSND_FADEOUT)
        END_IF
        IF_STR_EQUAL(LW(0), STRING("minnnanokoe"))
            CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_STG8_VOICE1"))
            CALL(evt_snd_envoff, PSND_FADEOUT)
        END_IF
        ELSE
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG1_NOK1"))
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG1_NOK2"))
    END_IF
    CALL(evt_npc_setup, PTR(npcEnt))
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG1_00_PETAL_MEADOWS)
        IF_INT_LESS_OR_EQUAL(GSW_STORY_PROGRESS, STG1_17_FATHER_SON_REUNITED)
            CALL_CHILD(PTR(odoriko_1_init))
            CALL_CHILD(PTR(odoriko_2_init))
            CALL_CHILD(PTR(odoriko_3_init))
        END_IF
    END_IF
    CALL(evt_map_playanim, STRING("S_kawa"), MAP_ANIM_PLAY_LOOP, 0)
    SET_VALUE(LW(0), PTR(&son_data))
    CALL_CHILD(PTR(evt_door_setup))
    SET_VALUE(LW(0), PTR(&taro_data))
    CALL_CHILD(PTR(evt_door_setup))
    SET_VALUE(LW(0), PTR(&ie_data))
    CALL_CHILD(PTR(evt_door_setup))
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG1_03_PETALBURG_GATEKEEPER)
        CALL(evt_map_playanim, STRING("S_mon"), MAP_ANIM_PLAY_AT_END, 0)
        CALL(evt_hitobj_onoff, STRING("A_mon"), 1, 0)
    END_IF
    SET_VALUE(LW(0), PTR(kusa_data))
    // Set up the bushes, the spinning floor, the Mega Rush P badge and the Turtley Leaf.
    CALL_CHILD(PTR(evt_sub_kusa_access_entry))
    SET_VALUE(LSWF_NOK_01_TURTLEY_LEAF, 0)
    CALL(evt_sub_random, 2, LSW_NOK_01_LEAF_BUSH)
    CALL(evt_sub_get_system_flag, LW(0))
    IF_BITS_SET(LW(0), GW_FLAG_E3)
        SET_VALUE(LSWF_NOK_01_TURTLEY_LEAF, 1)
    END_IF
    CALL(evt_mobj_kururing_floor, STRING("kururin2"), 400, 0, 140, STRING("S_guround"), 125, GSWF_STARPIECE_35)
    CALL(evt_item_entry, STRING("item_00"), ITEM_MEGA_RUSH_P, -180, 70, -425, ITEM_TYPE_STILL, GSWF_NOK_01_MEGA_RUSH_P, 0)
    IF_INT_EQUAL(GSWF_TROUBLE_DONE(TROUBLE_LISTEN_TO_ME), 1)
        CALL(evt_item_entry, STRING("iri_08"), ITEM_TURTLEY_LEAF, -317, 128, -374, ITEM_TYPE_STILL, GSWF_NOK_01_LISTEN_TO_ME_TURTLEY_LEAF, 0)
    END_IF
    IF_INT_EQUAL(LF(0), 0)
        LOOP(2)
            CALL(evt_sub_random, 10, LW(0))
            CALL(evt_eff, 0, STRING("butterfly"), LW(0), -395, 20, 225, 0, 0, 0, 0, 0, 0, 0, 0)
        END_LOOP
        LOOP(5)
            CALL(evt_sub_random, 10, LW(0))
            CALL(evt_eff, 0, STRING("butterfly"), LW(0), -170, 20, 400, 0, 0, 0, 0, 0, 0, 0, 0)
        END_LOOP
        LOOP(5)
            CALL(evt_sub_random, 10, LW(0))
            CALL(evt_eff, 0, STRING("butterfly"), LW(0), 180, 20, 400, 0, 0, 0, 0, 0, 0, 0, 0)
        END_LOOP
        LOOP(2)
            CALL(evt_sub_random, 10, LW(0))
            CALL(evt_eff, 0, STRING("butterfly"), LW(0), 400, 20, 200, 0, 0, 0, 0, 0, 0, 0, 0)
        END_LOOP
    END_IF
    CALL(evt_hit_damage_return_set, PTR(hitDmgRetPoints))
    CALL_CHILD(PTR(evt_gazigazi_entry))
    CALL(evt_bero_get_entername, LW(0))
    IF_STR_EQUAL(LW(0), STRING("nokotarou"))
        CALL(evt_pouch_mario_recovery)
        CALL(evt_pouch_all_party_recovery)
        // Entering through nokotarou heals Mario and starts Koops's goodbye scene.
        CALL_CHILD(PTR(nokotarou_tabidachi_init))
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG8_15_GRODUS)
        CALL(evt_bero_get_entername, LW(0))
        IF_STR_EQUAL(LW(0), STRING("sekai_yami1"))
            // The chapter 8 finale scenes start from here.
            SPAWN_EVENT(PTR(sekai_yami_evt))
        END_IF
        IF_STR_EQUAL(LW(0), STRING("minnnanokoe"))
            SPAWN_EVENT(PTR(minnnanokoe_evt))
        END_IF
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG8_END_AFTER_THE_BATTLE)
        CALL(evt_mario_kill_party, 0)
        CALL_CHILD(PTR(epilogue_npc_entry))
        SPAWN_EVENT(PTR(epilogue_evt))
    END_IF
    SPAWN_EVENT(PTR(evt_sub_mail_event))
    CALL(evt_sub_get_system_flag, LW(0))
    IF_BITS_SET(LW(0), GW_FLAG_E3)
        CALL(evt_npc_change_interrupt, STRING("門番ノコノコ"), 6, PTR(e3_talk))
    END_IF
    RETURN
EVT_END()

