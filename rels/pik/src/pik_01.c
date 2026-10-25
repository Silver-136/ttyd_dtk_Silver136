#include "battle/battle_unit.h"
#include "eff/eff_starstone.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bg.h"
#include "evt/evt_cam.h"
#include "evt/evt_eff.h"
#include "evt/evt_case.h"
#include "evt/evt_fade.h"
#include "evt/evt_hit.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_mobj.h"
#include "evt/evt_msg.h"
#include "evt/evt_party.h"
#include "evt/evt_seq.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_sub.h"
#include "evt/evt_item.h"
#include "evt/evt_pouch.h"

#include "mario/mario.h"
#include "manager/evtmgr_cmd.h"
#include "dolphin/mtx.h"
#include "driver/npcdrv.h"
#include "driver/animdrv.h"
#include "driver/effdrv.h"
#include "driver/mobjdrv.h"
#include "driver/mapdrv.h"
#include "evt/evt_bero.h"
#include "evt/evt_damage.h"
#include "evt/evt_npc.h"
#include "pik.h"
#include "driver/hitdrv.h"
#include "mario/mario_pouch.h"

static USER_FUNC(anim_tevcallback);
USER_FUNC(chk_luigi_color);
USER_FUNC(evt_mario_dispflag_onoff);
USER_FUNC(evt_pouch_get_haveitemcnt);
USER_FUNC(evt_win_item_select);
USER_FUNC(kpa_chg_pose);
USER_FUNC(kpa_lock_hit_scale);
USER_FUNC(set_r);
static USER_FUNC(starstone_entry);
static USER_FUNC(starstone_set_pos);
static USER_FUNC(starstone_set_ry);
static USER_FUNC(yami_view);
void test_tev2(MapTevCallbackInfo* info);
void test_tev(MapTevCallbackInfo* info);
extern s32 envSetYami[];

static HitDamageReturn dmgReturnPt[9];
static EvtWord n_door_open[22];
static EvtWord n_door_close[22];
static BeroEntry bero_data[4];
static const char* name_tbl[2];
EvtWord evt_powan[346];
static EvtWord init_powan[18];
static EvtWord talk_powan[8];
EvtWord miyageya_talk[563];
EvtWord miyageya_init[24];
s32 iri_29_item_tbl1[2];
s32 iri_29_item_tbl2[2];
s32 iri_29_item_tbl3[2];
EvtWord konarikin_talk[426];
EvtWord konarikin_init[29];
EvtWord narikin_papa_mama_talk[55];
EvtWord narikin_papa_init[23];
EvtWord narikin_mama_init[23];
EvtWord madam_talk[19];
EvtWord madam_init[18];
static EvtWord baba_talk[8];
static EvtWord baba_init[9];
EvtWord takun4_init[7];
EvtWord takun4_talk[37];
EvtWord takun5_init[25];
EvtWord takun5_talk[28];
EvtWord takun6_init[25];
EvtWord takun6_talk[28];
static NpcSetupInfo npc_data[8];
EvtWord teresa_fuwafuwa[54];
EvtWord resaresa_talk[8];
EvtWord resaresa_entry[35];
EvtWord sebaschan_talk[8];
EvtWord sebaschan_entry[35];
EvtWord koopa_door_evt[227];
EvtWord pik_01_koopa_evt[241];
static EvtWord delete_npc_regl_evt[35];
static EvtWord sekai_yami_shake[11];
static EvtWord sekai_yami_evt[140];
static EvtWord minnnanokoe_evt[391];
static EvtWord evt_switch_init[17];
EvtWord iri_27_npc_entry[26];
EvtWord pik_01_init_evt[486];

USER_FUNC(chk_luigi_color) {
    EvtWord* args = event->args;

    if (marioGetColor() == 1) {
        evtSetValue(event, args[0], 1);
    } else {
        evtSetValue(event, args[0], 0);
    }
    return EVT_RETURN_DONE;
}

static HitDamageReturn dmgReturnPt[] = {
    {"mod_00", {0.0f, -1000.0f, 0.0f}},
    {"mod_01", {0.0f, -1000.0f, 0.0f}},
    {"mod_02", {0.0f, -1000.0f, 0.0f}},
    {"mod_03", {0.0f, -1000.0f, 0.0f}},
    {"mod_04", {0.0f, -1000.0f, 0.0f}},
    {"mod_05", {0.0f, -1000.0f, 0.0f}},
    {"mod_06", {0.0f, -1000.0f, 0.0f}},
    {"mod_07", {0.0f, -1000.0f, 0.0f}},
    {0},
};

static EVT_BEGIN(n_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.38867188f))
    CALL(evt_mapobj_rotate, 1, STRING("door_1_b"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 1, STRING("door_1_a"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(n_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.38867188f))
    CALL(evt_mapobj_rotate, 1, STRING("door_1_b"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 1, STRING("door_1_a"), 0, LW(0), 0)
    RETURN
EVT_END()

static BeroEntry bero_data[] = {
    {
        "nw_bero", // hitName
        0, 0, // type..sfxId
        7, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "pik_04", // nextMapName
        "se_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "n_bero", // hitName
        1, // type
        6, // sfxId
        1, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "pik_02", // nextMapName
        "s_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        n_door_close, // enterAnimeArg
        n_door_open, // exitAnimeArg
    },
    {
        "dokan_1", // hitName
        2, // type
        0, // sfxId
        9, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        NULL, // exitEvent
        "tik_17", // nextMapName
        "dokan_1", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

static const char* name_tbl[] = {
    "yajirusi_1",
    "yajirusi_2",
};

EVT_BEGIN(evt_powan)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_snd_bgmoff, 18432)
    CALL(evt_msg_print, 0, STRING("stg6_pik_06"), 0, 0)
    WAIT_MS(300)
    CALL(evt_npc_flag_onoff, 1, STRING("ポワン探偵"), NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_GRAVITY)
#ifdef VERSION_G8ME01
    CALL(evt_npc_set_position, STRING("ポワン探偵"), -70, 5, -248)
#else
    CALL(evt_npc_set_position, STRING("ポワン探偵"), -50, 10, -235)
#endif
    WAIT_FRAMES(1)
    CALL(evt_party_stop, 0)
    WAIT_MS(300)
    CALL(evt_mario_get_party, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(1)
        CALL(evt_msg_print, 0, STRING("stg6_pik_07_0"), 0, STRING("party"))
        CASE_EQUAL(2)
        CALL(evt_msg_print, 0, STRING("stg6_pik_07_1"), 0, STRING("party"))
        CASE_EQUAL(4)
        CALL(evt_msg_print, 0, STRING("stg6_pik_07_2"), 0, STRING("party"))
        CASE_EQUAL(5)
        CALL(evt_msg_print, 0, STRING("stg6_pik_07_3"), 0, STRING("party"))
        CASE_EQUAL(6)
        CALL(evt_msg_print, 0, STRING("stg6_pik_07_4"), 0, STRING("party"))
        CASE_EQUAL(3)
        CALL(evt_msg_print, 0, STRING("stg6_pik_07_5"), 0, STRING("party"))
        CASE_EQUAL(7)
        CALL(evt_msg_print, 0, STRING("stg6_pik_07_6"), 0, STRING("party"))
    END_SWITCH
    WAIT_MS(500)
    CALL(evt_snd_bgmoff, 1024)
    CALL(evt_set_dir_to_target, STRING("ポワン探偵"), STRING("mario"))
    CALL(evt_msg_print, 0, STRING("stg6_pik_08"), 0, STRING("ポワン探偵"))
    CALL(evt_mario_set_dir, 270, 200, 0)
    CALL(evt_party_set_dir, 0, 270, 200)
    BEGIN_INLINE_EVENT
        WAIT_MS(500)
        CALL(evt_snd_bgmoff, 1024)
        CALL(evt_snd_bgmon, 1, STRING("BGM_EVT_POWAN1"))
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_set, -350, 120, 40, -55, 50, -236, 500, 11)
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_off, 4000, 11)
    CALL(evt_npc_status_onoff, 1, STRING("ポワン探偵"), 2)
    CALL(evt_npc_move_position, STRING("ポワン探偵"), 85, -100, 0, FLOAT(50.0f), 1)
    CALL(evt_cam3d_evt_set, -108, 131, 121, 188, 58, -167, 1000, 11)
    BEGIN_INLINE_EVENT
        CALL(evt_party_move_pos2, 0, 228, -140, FLOAT(80.0f))
        CALL(evt_party_set_dir, 0, 270, 200)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_mario_mov_pos2, 190, -136, FLOAT(80.0f))
        CALL(evt_mario_set_dir, 270, 200, 0)
    END_INLINE_EVENT
    CALL(evt_npc_move_position, STRING("ポワン探偵"), 159, -171, 0, FLOAT(50.0f), 1)
    CALL(evt_mario_get_party, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(1)
        CALL(evt_msg_print, 0, STRING("stg6_pik_09_0"), 0, STRING("party"))
        CASE_EQUAL(2)
        CALL(evt_msg_print, 0, STRING("stg6_pik_09_1"), 0, STRING("party"))
        CASE_EQUAL(4)
        CALL(evt_msg_print, 0, STRING("stg6_pik_09_2"), 0, STRING("party"))
        CASE_EQUAL(5)
        CALL(evt_msg_print, 0, STRING("stg6_pik_09_3"), 0, STRING("party"))
        CASE_EQUAL(6)
        CALL(evt_msg_print, 0, STRING("stg6_pik_09_4"), 0, STRING("party"))
        CASE_EQUAL(3)
        CALL(evt_msg_print, 0, STRING("stg6_pik_09_5"), 0, STRING("party"))
        CASE_EQUAL(7)
        CALL(evt_msg_print, 0, STRING("stg6_pik_09_6"), 0, STRING("party"))
    END_SWITCH
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg6_pik_10"), 0, STRING("ポワン探偵"))
    CALL(evt_mario_set_pose, STRING("M_N_5B"))
    WAIT_MS(1000)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_msg_print, 0, STRING("stg6_pik_11"), 0, STRING("ポワン探偵"))
    WAIT_MS(500)
    CALL(evt_npc_move_position, STRING("ポワン探偵"), 210, -193, 0, FLOAT(50.0f), 1)
    CALL(evt_mobj_lock_unlock, STRING("lock"))
    WAIT_MS(1000)
    CALL(evt_npc_move_position, STRING("ポワン探偵"), 180, -220, 0, FLOAT(50.0f), 1)
    CALL(evt_set_dir_to_target, STRING("ポワン探偵"), STRING("mario"))
    SET_VALUE(GSW_STORY_PROGRESS, 335)
    SET_VALUE(LW(0), 1)
    CALL_CHILD(PTR(bero_case_switch_on))
    CALL(evt_cam3d_evt_off, 500, 11)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_party_run, 0)
    CALL(evt_snd_bgmoff, 513)
    CALL(evt_snd_bgmon, 288, 0)
    CALL(evt_snd_bgmon, 160, 0)
    CALL(evt_exit_case_evt)
    RETURN
EVT_END()

static EVT_BEGIN(init_powan)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(335)
        CALL(evt_npc_set_position, STRING("me"), 165, 20, -190)
        CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_GRAVITY)
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(talk_powan)
    CALL(evt_msg_print, 0, STRING("stg6_pik_11"), 0, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(miyageya_talk)
    IF_INT_EQUAL(GSW(1420), 27)
        CALL(chk_luigi_color, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            IF_INT_EQUAL(GSWF(5405), 0)
                CALL(evt_msg_print, 0, STRING("irai_27_001"), 0, 0)
                CALL(evt_mario_set_pose, STRING("M_N_5B"))
                WAIT_MS(300)
                CALL(evt_mario_set_pose, STRING("M_I_G"))
                WAIT_MS(800)
                CALL(evt_mario_set_pose, STRING("M_S_1"))
                CALL(evt_msg_print, 0, STRING("irai_27_001_1"), 0, 0)
                SET_VALUE(GSWF(5405), 1)
                ELSE
                CALL(evt_msg_print, 0, STRING("irai_27_002"), 0, 0)
            END_IF
            ELSE
            CALL(evt_npc_flag_onoff, 1, STRING("土産屋"), NPC_FLAG_NO_HIT)
            CALL(evt_cam3d_evt_set, -545, 90, 530, -290, 55, 285, 800, 11)
            BEGIN_INLINE_EVENT
                LOOP(0)
                    CALL(evt_mario_party_use_check, LW(0))
                    IF_INT_EQUAL(LW(0), 0)
                        BREAK_LOOP
                    END_IF
                    WAIT_FRAMES(1)
                END_LOOP
                CALL(evt_party_stop, 0)
                CALL(evt_party_move_pos, 0, -225, 321, 800)
                CALL(evt_party_set_dir, 0, 270, 300)
            END_INLINE_EVENT
            CALL(evt_mario_mov_pos, -245, 300, 800)
            CALL(evt_mario_set_dir, 270, 300, 0)
            WAIT_MS(800)
            CALL(evt_set_dir_to_target, STRING("土産屋"), STRING("mario"))
            WAIT_MS(300)
            CALL(evt_msg_print, 0, STRING("irai_27_003"), 0, 0)
            CALL(evt_mario_set_pose, STRING("M_I_2"))
            WAIT_MS(1000)
            CALL(evt_mario_set_pose, STRING("M_S_1"))
            CALL(evt_msg_print, 0, STRING("irai_27_004"), 0, 0)
            CALL(evt_mario_set_pose, STRING("M_I_Z"))
            WAIT_MS(500)
            CALL(evt_msg_print, 0, STRING("irai_27_005"), 0, 0)
            CALL(evt_npc_get_position, STRING("土産屋"), LW(0), LW(1), LW(2))
            CALL(evt_get_target_dir, STRING("土産屋"), STRING("mario"), LW(3))
            SET_VALUE(LW(4), LW(0))
            SET_VALUE(LW(5), LW(2))
            CALL(evt_npc_add_dirdist, LW(4), LW(5), LW(3), 30)
            CALL(evt_npc_flag_onoff, 1, STRING("土産屋"), NPC_FLAG_NO_HIT)
            CALL(evt_npc_move_position, STRING("土産屋"), LW(4), LW(5), 0, 50, 0)
            CALL(evt_pouch_get_haveitemcnt, LW(3))
            CALL(evt_pouch_check_item, 1, LW(4))
            IF_INT_EQUAL(LW(4), 0)
                SET_VALUE(LW(4), 10)
                ELSE
                SET_VALUE(LW(4), 20)
            END_IF
            IF_INT_LESS(LW(3), LW(4))
                CALL(evt_msg_print, 0, STRING("irai_27_006"), 0, 0)
                CALL(evt_item_entry, STRING("sc"), 214, 0, -1000, 0, 17, -1, 0)
                CALL(evt_item_get_item, STRING("sc"))
                CALL(evt_item_get_item_end_wait, STRING("sc"))
                SET_VALUE(GSWF(3278), 1)
                CALL(evt_npc_move_position, STRING("土産屋"), LW(0), LW(2), 0, 50, 0)
                CALL(evt_npc_flag_onoff, 0, STRING("土産屋"), NPC_FLAG_NO_HIT)
                ELSE
                WAIT_MS(500)
                CALL(evt_npc_move_position, STRING("土産屋"), LW(0), LW(2), 0, 80, 0)
                CALL(evt_npc_flag_onoff, 0, STRING("土産屋"), NPC_FLAG_NO_HIT)
                CALL(evt_msg_print, 0, STRING("irai_27_007_1"), 0, STRING("土産屋"))
                WAIT_MS(300)
                CALL(evt_item_entry, STRING("iri_27"), 214, -246, 15, 246, 16, GSWF(3278), 0)
                WAIT_MS(300)
                CALL(evt_msg_print, 0, STRING("irai_27_007_2"), 0, STRING("土産屋"))
            END_IF
            CALL(evt_npc_set_position, STRING("ルイージ"), -386, 0, 214)
            CALL(evt_npc_flag_onoff, 1, STRING("ルイージ"), NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_NO_HIT | NPC_FLAG_GRAVITY)
            CALL(evt_npc_move_position, STRING("ルイージ"), -330, 272, 0, 60, 0)
            CALL(evt_msg_print, 0, STRING("irai_27_008"), 0, STRING("ルイージ"))
            CALL(evt_mario_set_pose, STRING("M_S_1"))
            CALL(evt_mario_set_dir_npc, STRING("ルイージ"))
            CALL(evt_set_dir_to_target, STRING("土産屋"), STRING("ルイージ"))
            WAIT_MS(300)
            CALL(evt_eff_fukidashi, 1, STRING(""), STRING("土産屋"), 0, 0, 0, 0, 0, 0, 0, 60)
            WAIT_MS(1200)
            CALL(evt_msg_print, 0, STRING("irai_27_009"), 0, 0)
            CALL(evt_npc_get_position, STRING("ルイージ"), LW(0), LW(1), LW(2))
            CALL(evt_npc_jump_position_nohit, STRING("ルイージ"), LW(0), LW(1), LW(2), 300, 10)
            CALL(evt_msg_print, 0, STRING("irai_27_010"), 0, STRING("ルイージ"))
            CALL(evt_mario_set_pose, STRING("M_N_1"))
            WAIT_MS(500)
            CALL(evt_mario_set_pose, STRING("M_S_1"))
            CALL(evt_msg_print, 0, STRING("irai_27_011"), 0, 0)
            CALL(evt_msg_print, 0, STRING("irai_27_012"), 0, STRING("ルイージ"))
            CALL(evt_npc_move_position, STRING("ルイージ"), -386, 214, 0, 150, 0)
            CALL(evt_npc_flag_onoff, 0, STRING("ルイージ"), NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_GRAVITY)
            CALL(evt_npc_set_position, STRING("ルイージ"), 0, -1000, 0)
            CALL(evt_set_dir_to_target, STRING("土産屋"), STRING("mario"))
            WAIT_MS(300)
            CALL(evt_msg_print, 0, STRING("irai_27_013"), 0, 0)
            CALL(evt_mario_set_dir_npc, STRING("土産屋"))
            WAIT_MS(600)
            CALL(evt_mario_set_pose, STRING("M_I_2"))
            WAIT_MS(500)
            CALL(evt_mario_set_pose, STRING("M_S_1"))
            CALL(evt_msg_print, 0, STRING("irai_27_014"), 0, 0)
            CALL(evt_cam3d_evt_off, 800, 11)
            WAIT_MS(800)
            SET_VALUE(GSW(1420), 0)
            SET_VALUE(GSWF(5401), 1)
            CALL(evt_msg_print, 0, STRING("sys_request_finish"), 0, 0)
            CALL(evt_party_run, 0)
        END_IF
        RETURN
    END_IF
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(349)
        CALL(evt_msg_print, 0, STRING("stg7_pik_clear_02"), 0, STRING("me"))
        CASE_BETWEEN(373, 376)
        IF_INT_EQUAL(LSWF(1), 0)
            CALL(evt_npc_set_force_regl_anim, STRING("me"), STRING("KNF_S_2"))
            CALL(evt_npc_set_anim, STRING("me"), STRING("KNF_S_1"))
            CALL(evt_npc_set_autotalkpose, STRING("me"), STRING("KNF_S_2"), STRING("KNF_T_2"))
            CALL(evt_msg_print, 0, STRING("kpa_stg7_004"), 0, STRING("me"))
            CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_TALK_POSE)
            SET_VALUE(LSWF(1), 1)
            ELSE
            CALL(evt_msg_print, 0, STRING("kpa_stg7_004_1"), 0, STRING("me"))
        END_IF
        CASE_DEFAULT
        CALL(chk_luigi_color, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            IF_INT_LESS_OR_EQUAL(GSW_STORY_PROGRESS, 402)
                CALL(evt_msg_print, 0, STRING("stg7_pik_clear_03"), 0, STRING("me"))
                ELSE
                CALL(evt_msg_print, 0, STRING("stg7_pik_clear_03_0"), 0, STRING("me"))
            END_IF
            ELSE
            IF_INT_EQUAL(GSWF(5401), 0)
                CALL(evt_msg_print, 0, STRING("stg7_pik_clear_03_1"), 0, STRING("me"))
                ELSE
                CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
                CALL(evt_npc_jump_position_nohit, STRING("me"), LW(0), LW(1), LW(2), 200, 10)
                CALL(evt_msg_print, 0, STRING("stg7_pik_clear_03_2"), 0, STRING("me"))
            END_IF
        END_IF
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(miyageya_init)
    CALL(evt_npc_set_position, STRING("me"), -270, -5, 260)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(373, 376)
        CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_AUTO_MOVE_ANIM)
    END_SWITCH
    CALL(evt_npc_change_interrupt, STRING("me"), 6, PTR(miyageya_talk))
    RETURN
EVT_END()

s32 iri_29_item_tbl1[] = {
    ITEM_SHROOM_CAKE, -1,
};

s32 iri_29_item_tbl2[] = {
    ITEM_KEEL_MANGO, -1,
};

s32 iri_29_item_tbl3[] = {
    ITEM_FRIGHT_MASK, -1,
};

EVT_BEGIN(konarikin_talk)
    IF_INT_EQUAL(GSW(1420), 29)
        IF_INT_EQUAL(GSWF(5406), 0)
            IF_INT_EQUAL(GSWF(5405), 0)
                CALL(evt_msg_print, 0, STRING("irai_29_00"), 0, 0)
                CALL(evt_msg_select, 0, STRING("irai_29_00_s"))
                IF_INT_GREATER_OR_EQUAL(LW(0), 3)
                    CALL(evt_msg_continue)
                    RETURN
                END_IF
                SET_VALUE(GSW(1469), LW(0))
                CALL(evt_msg_print_add, 0, STRING("irai_29_01"))
                SET_VALUE(GSWF(5405), 1)
                ELSE
                CALL(evt_pouch_check_item, 100, LW(0))
                IF_INT_GREATER(LW(0), 0)
                    CALL(evt_msg_print, 0, STRING("irai_29_11"), 0, 0)
                    RETURN
                END_IF
                SWITCH(GSW(1469))
                    CASE_EQUAL(0)
                    CALL(evt_pouch_check_item, 204, LW(0))
                    IF_INT_GREATER(LW(0), 0)
                        CALL(evt_msg_print, 0, STRING("irai_29_05"), 0, 0)
                        CALL(evt_win_item_select, 0, PTR(iri_29_item_tbl1), LW(0), 0)
                        IF_INT_NOT_EQUAL(LW(0), -1)
                            CALL(evt_pouch_remove_item, 204, LW(0))
                            CALL(evt_msg_print, 0, STRING("irai_29_08"), 0, 0)
                            CALL(evt_item_entry, STRING("pre"), 100, 0, -1000, 0, 17, -1, 0)
                            CALL(evt_item_get_item, STRING("pre"))
                            CALL(evt_item_get_item_end_wait, STRING("pre"))
                        END_IF
                        ELSE
                        CALL(evt_msg_print, 0, STRING("irai_29_02"), 0, 0)
                    END_IF
                    CASE_EQUAL(1)
                    CALL(evt_pouch_check_item, 170, LW(0))
                    IF_INT_GREATER(LW(0), 0)
                        CALL(evt_msg_print, 0, STRING("irai_29_06"), 0, 0)
                        CALL(evt_win_item_select, 0, PTR(iri_29_item_tbl2), LW(0), 0)
                        IF_INT_NOT_EQUAL(LW(0), -1)
                            CALL(evt_pouch_remove_item, 170, LW(0))
                            CALL(evt_msg_print, 0, STRING("irai_29_09"), 0, 0)
                            CALL(evt_item_entry, STRING("pre"), 100, 0, -1000, 0, 17, -1, 0)
                            CALL(evt_item_get_item, STRING("pre"))
                            CALL(evt_item_get_item_end_wait, STRING("pre"))
                        END_IF
                        ELSE
                        CALL(evt_msg_print, 0, STRING("irai_29_03"), 0, 0)
                    END_IF
                    CASE_EQUAL(2)
                    CALL(evt_pouch_check_item, 162, LW(0))
                    IF_INT_GREATER(LW(0), 0)
                        CALL(evt_msg_print, 0, STRING("irai_29_07"), 0, 0)
                        CALL(evt_win_item_select, 0, PTR(iri_29_item_tbl3), LW(0), 0)
                        IF_INT_NOT_EQUAL(LW(0), -1)
                            CALL(evt_pouch_remove_item, 162, LW(0))
                            CALL(evt_msg_print, 0, STRING("irai_29_10"), 0, 0)
                            CALL(evt_item_entry, STRING("pre"), 100, 0, -1000, 0, 17, -1, 0)
                            CALL(evt_item_get_item, STRING("pre"))
                            CALL(evt_item_get_item_end_wait, STRING("pre"))
                        END_IF
                        ELSE
                        CALL(evt_msg_print, 0, STRING("irai_29_04"), 0, 0)
                    END_IF
                END_SWITCH
            END_IF
            ELSE
            CALL(evt_msg_print, 0, STRING("irai_29_12"), 0, 0)
            CALL(evt_mario_set_pose, STRING("M_N_2"))
            WAIT_MS(500)
            CALL(evt_mario_set_pose, STRING("M_S_1"))
            CALL(evt_msg_print, 0, STRING("irai_29_13"), 0, 0)
            CALL(evt_sub_get_coin, 3)
            BEGIN_INLINE_EVENT
                WAIT_MS(300)
                CALL(evt_mario_set_dir_npc, STRING("コナリキン"))
            END_INLINE_EVENT
            BEGIN_INLINE_EVENT
                WAIT_MS(300)
                CALL(evt_party_set_dir_npc, 0, STRING("コナリキン"))
            END_INLINE_EVENT
            CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_HIT)
            CALL(evt_npc_status_onoff, 1, STRING("me"), 2)
            CALL(evt_npc_move_position, STRING("me"), -222, -225, 0, 120, 0)
            CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
            SET_VALUE(GSW(1420), 0)
            SET_VALUE(GSWF(5403), 1)
            CALL(evt_msg_print, 0, STRING("sys_request_finish"), 0, 0)
        END_IF
        RETURN
    END_IF
    CALL(evt_msg_print, 0, STRING("kpa_stg7_005"), 0, STRING("me"))
    CALL(evt_msg_select, 0, STRING("kpa_stg7_005_1"))
    CALL(evt_msg_continue)
    CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("KPA_S_1"), STRING("KPA_T_1"))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CALL(evt_msg_print, 0, STRING("kpa_stg7_006"), 0, STRING("mario"))
        CALL(evt_npc_set_autotalkpose, STRING("me"), STRING("C_1"), STRING("T_4"))
        CALL(evt_msg_print, 0, STRING("kpa_stg7_007"), 0, STRING("me"))
        CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_HIT)
        CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_AUTO_MOVE_ANIM)
        CALL(evt_npc_status_onoff, 1, STRING("me"), 2)
        CALL(evt_npc_set_anim, STRING("me"), STRING("R_2"))
        CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_KUPPA_KONARIKIN_ESCAPE1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_npc_move_position, STRING("me"), -200, 380, 0, FLOAT(150.0f), 0)
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
        CASE_EQUAL(1)
        CALL(evt_msg_print, 0, STRING("kpa_stg7_008"), 0, STRING("mario"))
        CALL(evt_msg_print, 0, STRING("kpa_stg7_009"), 0, STRING("me"))
        CASE_EQUAL(2)
        CALL(evt_msg_print, 0, STRING("kpa_stg7_010"), 0, STRING("mario"))
        CALL(evt_msg_print, 0, STRING("kpa_stg7_011"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(konarikin_init)
    CALL_CHILD(PTR(npc_init_evt))
    IF_INT_EQUAL(GSW(1420), 29)
        CALL(evt_npc_set_position, STRING("me"), -62, -5, -367)
        RETURN
    END_IF
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 373)
        IF_INT_LESS(GSW_STORY_PROGRESS, 376)
            CALL(evt_npc_set_position, STRING("me"), 145, 0, 380)
        END_IF
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(narikin_papa_mama_talk)
    CALL(evt_cam3d_evt_set, -109, 163, 752, -109, 67, 211, 800, 11)
    WAIT_MS(800)
    CALL(evt_msg_print, 0, STRING("kpa_stg7_011_3"), 0, STRING("ナリキンパパ"))
    CALL(evt_msg_print, 0, STRING("kpa_stg7_011_4"), 0, STRING("ナリキンママ"))
    CALL(evt_msg_print, 0, STRING("kpa_stg7_011_5"), 0, STRING("ナリキンパパ"))
    CALL(evt_msg_print, 0, STRING("kpa_stg7_011_6"), 0, STRING("ナリキンママ"))
    CALL(evt_cam3d_evt_off, 1000, 11)
    WAIT_MS(1000)
    CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("KPA_S_1"), STRING("KPA_T_1"))
    CALL(evt_msg_print, 0, STRING("kpa_stg7_011_7"), 0, STRING("mario"))
    RETURN
EVT_END()

EVT_BEGIN(narikin_papa_init)
    CALL_CHILD(PTR(npc_init_evt))
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_TALK_TURN)
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 373)
        IF_INT_LESS(GSW_STORY_PROGRESS, 376)
            CALL(evt_npc_set_position, STRING("me"), -6, -5, 161)
        END_IF
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(narikin_mama_init)
    CALL_CHILD(PTR(npc_init_evt))
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_TALK_TURN)
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 373)
        IF_INT_LESS(GSW_STORY_PROGRESS, 376)
            CALL(evt_npc_set_position, STRING("me"), 18, -5, 185)
        END_IF
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(madam_talk)
    CALL(evt_msg_print, 0, STRING("kpa_stg7_011_1"), 0, STRING("me"))
    CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("KPA_S_1"), STRING("KPA_T_1"))
    CALL(evt_msg_print, 0, STRING("kpa_stg7_011_2"), 0, STRING("mario"))
    RETURN
EVT_END()

EVT_BEGIN(madam_init)
    CALL_CHILD(PTR(npc_init_evt))
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 373)
        IF_INT_LESS(GSW_STORY_PROGRESS, 376)
            CALL(evt_npc_set_position, STRING("me"), -395, 0, -72)
        END_IF
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(baba_talk)
    CALL(evt_msg_print, 0, STRING("kpa_stg7_003_1"), 0, STRING("me"))
    RETURN
EVT_END()

static EVT_BEGIN(baba_init)
    CALL_CHILD(PTR(npc_init_evt))
    CALL(evt_npc_status_onoff, 1, STRING("me"), 2)
    RETURN
EVT_END()

EVT_BEGIN(takun4_init)
    CALL(evt_npc_flag_onoff, 1, STRING("ターくん４"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    RETURN
EVT_END()

EVT_BEGIN(takun4_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(347)
        CALL(evt_msg_print, 0, STRING("stg7_pik_clear_04"), 0, STRING("me"))
        CASE_BETWEEN(373, 376)
        CALL(evt_msg_print, 0, STRING("kpa_stg7_011_7_1"), 0, STRING("me"))
        CASE_LESS_OR_EQUAL(402)
        CALL(evt_msg_print, 0, STRING("stg7_pik_clear_05"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg7_pik_clear_05_1"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(takun5_init)
    CALL(evt_npc_flag_onoff, 1, STRING("ターくん５"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(373, 376)
        CALL(evt_npc_set_position, STRING("ターくん５"), 0, -1000, 0)
        CASE_DEFAULT
        CALL(evt_npc_flag_onoff, 1, STRING("ターくん５"), NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_GRAVITY)
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(takun5_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(347)
        CALL(evt_msg_print, 0, STRING("stg7_pik_clear_06"), 0, STRING("me"))
        CASE_LESS_OR_EQUAL(402)
        CALL(evt_msg_print, 0, STRING("stg7_pik_clear_07"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg7_pik_clear_07_1"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(takun6_init)
    CALL(evt_npc_flag_onoff, 1, STRING("ターくん６"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(373, 376)
        CALL(evt_npc_set_position, STRING("ターくん６"), 0, -1000, 0)
        CASE_DEFAULT
        CALL(evt_npc_flag_onoff, 1, STRING("ターくん６"), NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_GRAVITY)
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(takun6_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(347)
        CALL(evt_msg_print, 0, STRING("stg7_pik_clear_08"), 0, STRING("me"))
        CASE_LESS_OR_EQUAL(402)
        CALL(evt_msg_print, 0, STRING("stg7_pik_clear_09"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg7_pik_clear_09_1"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

static NpcSetupInfo npc_data[] = {
    {
        /*"ポワン探偵"*/"\x83\x7C\x83\x8F\x83\x93\x92\x54\x92\xE3", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_powan, // initEvent
        NULL, // regularEvent
        talk_powan, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"土産屋"*/"\x93\x79\x8E\x59\x89\xAE", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        miyageya_init, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ナリキンママ"*/"\x83\x69\x83\x8A\x83\x4C\x83\x93\x83\x7D\x83\x7D", // name
        0, 0, // flags..reactionFlags
        narikin_mama_init, // initEvent
        NULL, // regularEvent
        narikin_papa_mama_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ナリキンパパ"*/"\x83\x69\x83\x8A\x83\x4C\x83\x93\x83\x70\x83\x70", // name
        0, 0, // flags..reactionFlags
        narikin_papa_init, // initEvent
        NULL, // regularEvent
        narikin_papa_mama_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"コナリキン"*/"\x83\x52\x83\x69\x83\x8A\x83\x4C\x83\x93", // name
        0, 0, // flags..reactionFlags
        konarikin_init, // initEvent
        NULL, // regularEvent
        konarikin_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"マダム"*/"\x83\x7D\x83\x5F\x83\x80", // name
        0, 0, // flags..reactionFlags
        madam_init, // initEvent
        NULL, // regularEvent
        madam_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ババ"*/"\x83\x6F\x83\x6F", // name
        0, 0, // flags..reactionFlags
        baba_init, // initEvent
        NULL, // regularEvent
        baba_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

EVT_BEGIN(teresa_fuwafuwa)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    SET_VALUE(LW(3), 0)
    CALL(evt_seq_wait, 2)
    CALL(evt_sub_random, 14, LW(4))
    ADD(LW(4), 1)
    MULTIPLY(LW(4), 30)
    WAIT_MS(LW(4))
    LOOP(0)
        CALL(evt_sub_get_sincos, LW(3), LW(4), LW(5))
        MULTIPLY_FLOAT(LW(4), FLOAT(1.5f))
        ADD_FLOAT(LW(4), LW(1))
        CALL(evt_npc_set_position, STRING("me"), LW(0), LW(4), LW(2))
        ADD(LW(3), 3)
        MODULO(LW(3), 360)
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(resaresa_talk)
    CALL(evt_msg_print, 0, STRING("stg7_pik_clear_03_2_01"), 0, 0)
    RETURN
EVT_END()

EVT_BEGIN(resaresa_entry)
    CALL(evt_npc_entry, STRING("レサレサ"), STRING("c_resaresa"))
    CALL(evt_npc_set_tribe, STRING("レサレサ"), STRING("レサレサ"))
    CALL(evt_npc_set_position, STRING("レサレサ"), 30, 15, 120)
    CALL(evt_npc_set_ry, STRING("レサレサ"), 90)
    CALL(evt_npc_flag_onoff, 1, STRING("レサレサ"), NPC_FLAG_TALK_POSE | NPC_FLAG_RESUME_AFTER_TALK)
    CALL(evt_npc_change_interrupt, STRING("レサレサ"), 1, PTR(teresa_fuwafuwa))
    CALL(evt_npc_change_interrupt, STRING("レサレサ"), 6, PTR(resaresa_talk))
    RETURN
EVT_END()

EVT_BEGIN(sebaschan_talk)
    CALL(evt_msg_print, 0, STRING("stg7_pik_clear_03_2_02"), 0, 0)
    RETURN
EVT_END()

EVT_BEGIN(sebaschan_entry)
    CALL(evt_npc_entry, STRING("セバスチャン"), STRING("c_sebaschan"))
    CALL(evt_npc_set_tribe, STRING("セバスチャン"), STRING("セバスチャン"))
    CALL(evt_npc_set_position, STRING("セバスチャン"), 75, 4, 90)
    CALL(evt_npc_set_ry, STRING("セバスチャン"), 270)
    CALL(evt_npc_flag_onoff, 1, STRING("セバスチャン"), NPC_FLAG_TALK_POSE | NPC_FLAG_RESUME_AFTER_TALK | NPC_FLAG_NO_TALK_TURN)
    CALL(evt_npc_change_interrupt, STRING("セバスチャン"), 1, PTR(teresa_fuwafuwa))
    CALL(evt_npc_change_interrupt, STRING("セバスチャン"), 6, PTR(sebaschan_talk))
    RETURN
EVT_END()

EVT_BEGIN(koopa_door_evt)
    CALL(evt_snd_bgm_scope, 0, 1)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_mobj_get_position, STRING("lock"), LW(3), LW(4), LW(5))
    CALL(evt_sub_get_dir, LW(3), LW(5), LW(0), LW(2), LW(6))
    SUBTRACT(LW(6), 45)
    IF_INT_LESS(LW(6), 0)
        ADD(LW(6), 360)
    END_IF
    IF_INT_LESS(LW(6), 180)
        CALL(evt_mario_set_dir, 270, 300, 0)
        ELSE
        CALL(evt_mario_set_dir, 90, 300, 0)
    END_IF
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, -132, 120, 148, 185, 90, -170, 1000, 11)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("kpa_stg7_012"), 0, 0)
    CALL(kpa_chg_pose, STRING("KPA_I_2"))
    SET_VALUE(GF(0), 0)
    SET_VALUE(GF(1), 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(200)
        LOOP(0)
            IF_INT_NOT_EQUAL(GF(1), 0)
                BREAK_LOOP
            END_IF
            CALL(evt_cam_shake, 4, FLOAT(0.0f), FLOAT(0.009765625f), 150)
            WAIT_MS(150)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(200)
        LOOP(0)
            IF_INT_NOT_EQUAL(GF(0), 0)
                BREAK_LOOP
            END_IF
            CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
            CALL(evt_snd_sfxon_3d, STRING("SFX_KUPPA_ANGRY1"), LW(0), LW(1), LW(2), 0)
            CALL(evt_snd_sfxon_3d, STRING("SFX_KUPPA_ANGRY2"), LW(0), LW(1), LW(2), 0)
            WAIT_MS(300)
        END_LOOP
    END_INLINE_EVENT
    WAIT_MS(1000)
    SET_VALUE(GF(1), 1)
    CALL(evt_mario_dispflag_onoff, 1, 2)
    CALL(evt_msg_print, 0, STRING("kpa_stg7_012_01"), 0, STRING("mario"))
    CALL(evt_mario_dispflag_onoff, 0, 2)
    CALL(evt_mobj_lock_unlock, STRING("lock"))
    WAIT_MS(600)
    SET_VALUE(GF(0), 1)
    CALL(evt_msg_print, 0, STRING("kpa_stg7_012_02"), 0, 0)
    CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("KPA_S_6"), STRING("KPA_T_6"))
    CALL(evt_msg_print, 0, STRING("kpa_stg7_013"), 0, STRING("mario"))
    SET_VALUE(LW(1), 1)
    SPAWN_EVENT(PTR(bero_open_door_play_se))
    CALL(evt_sub_intpl_msec_init, 11, 0, 180, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(10))
        IF_INT_EQUAL(LW(10), 0)
            SET_FLOAT_VALUE(LW(0), FLOAT(180.0f))
            CALL_CHILD(PTR(n_door_open))
            BREAK_LOOP
            ELSE
            CALL_CHILD(PTR(n_door_open))
        END_IF
    END_LOOP
    BEGIN_INLINE_EVENT
        CALL(evt_mario_mov_pos, 230, -215, 600)
    END_INLINE_EVENT
    WAIT_MS(300)
    CALL(evt_bero_mapchange, STRING("pik_02"), STRING(""))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(pik_01_koopa_evt)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_set_pos, -325, -5, -325)
    CALL(evt_npc_set_position, STRING("ババ"), -425, 25, -425)
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_npc_flag_onoff, 1, STRING("ババ"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_set_ry, STRING("ババ"), 90)
    CALL(evt_cam3d_evt_set, -273, 145, 196, -273, 69, -237, 0, 11)
    CALL(evt_seq_wait, 2)
    CALL(evt_bero_exec_wait, 65536)
    CALL(evt_npc_flag_onoff, 0, STRING("ババ"), NPC_FLAG_INSTANT_TURN)
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
        CALL(evt_npc_kamek_move_position, STRING("ババ"), -327, -327, 10, 0, FLOAT(60.0f), 0)
        SET_VALUE(GF(0), 1)
    END_INLINE_EVENT
    CALL(evt_mario_mov_pos2, -227, -227, FLOAT(60.0f))
    WAIT_MS(500)
    CALL(evt_mario_set_dir, 270, 300, 0)
    WAIT_MS(300)
    CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("KPA_S_1"), STRING("KPA_T_1"))
    CALL(evt_msg_print, 0, STRING("kpa_stg7_000"), 0, STRING("mario"))
    CALL(evt_msg_print, 0, STRING("kpa_stg7_001"), 0, STRING("ババ"))
    CALL(evt_mario_set_dir, 90, 300, 0)
    CALL(evt_npc_set_anim, STRING("ババ"), STRING("A_3A"))
    WAIT_MS(500)
    CALL(evt_cam3d_evt_set, -150, 95, 514, 114, 130, -241, 2000, 11)
    WAIT_MS(2000)
    CALL(evt_npc_set_anim, STRING("ババ"), STRING("S_2"))
    WAIT_MS(2000)
    CALL(evt_cam3d_evt_set, -273, 145, 196, -273, 69, -237, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_mario_set_dir, 270, 300, 0)
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("kpa_stg7_002"), 0, STRING("mario"))
    BEGIN_INLINE_EVENT
        WAIT_MS(300)
        CALL(evt_mario_set_dir, 90, 400, 0)
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_off, 800, 11)
    SET_VALUE(GSW_STORY_PROGRESS, 374)
    SET_VALUE(GF(1), 0)
    BEGIN_INLINE_EVENT
        LOOP(0)
            IF_INT_NOT_EQUAL(GF(1), 0)
                BREAK_LOOP
            END_IF
            CALL(evt_npc_get_position, STRING("ババ"), LW(0), LW(1), LW(2))
            CALL(evt_snd_sfxon_3d, STRING("SFX_KUPPA_KAMEBABA_FLY1"), LW(0), LW(1), LW(2), 0)
            WAIT_FRAMES(10)
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_npc_kamek_move_position, STRING("ババ"), -255, -300, 10, 0, 80, 0)
    SET_VALUE(GF(1), 1)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_snd_bgm_scope, 0, 0)
    RETURN
EVT_END()

void envSetYamiView(Mtx mtx);

static USER_FUNC(yami_view) {
    f32 z = evtGetFloat(event, event->args[0]);
    Mtx mtx;

    C_MTXLookAt(mtx, &(Vec){0.0f, 100.0f, 100.0f + z}, &(Vec){0.0f, 1.0f, 0.0f}, &(Vec){0.0f, 0.0f, z});
    envSetYamiView(mtx);
    return EVT_RETURN_DONE;
}

static USER_FUNC(anim_tevcallback) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    s32 flag = evtGetValue(event, args[1]);
    NpcEntry* npc = npcNameToPtr_NoAssert(name);

    if (npc == NULL) {
        return EVT_RETURN_DONE;
    }
    animPoseSetMaterialFlagOn(npc->poseId, (flag & 0xF) << 24);
    return EVT_RETURN_DONE;
}


static USER_FUNC(starstone_entry) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    f32 x = evtGetFloat(event, args[1]);
    f32 y = evtGetFloat(event, args[2]);
    f32 z = evtGetFloat(event, args[3]);

    effSetName(effStarStoneEntry(5, x, y, z, 1.0f), name);
    return EVT_RETURN_DONE;
}

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

static USER_FUNC(starstone_set_ry) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    f32 ry = evtGetFloat(event, args[1]);

    ((EffStarStoneWork*)effNameToPtr(name)->userdata)->rot.y = ry;
    return EVT_RETURN_DONE;
}

USER_FUNC(set_r) {
    MapObjectEntry* mobj = mobjNameToPtr("lock");

    mobj->rotation.y = -45.0f;
    return EVT_RETURN_DONE;
}

USER_FUNC(kpa_lock_hit_scale) {
    MapObjectEntry* mobj = mobjNameToPtr("lock");

    mobj->scale2.x *= 2.0f;
    mobj->scale2.y *= 2.0f;
    mobjCalcMtx2(mobj);
    mobj->flags &= ~MOBJ_FLAG_UPDATE_MTX;
    return EVT_RETURN_DONE;
}

void envAddTev(s32 type, MapTevCallbackInfo* info);

void test_tev(MapTevCallbackInfo* info) {
    envAddTev(4, info);
}


void test_tev2(MapTevCallbackInfo* info) {
    envAddTev(7, info);
}

static EVT_BEGIN(delete_npc_regl_evt)
    CALL(evt_npc_get_reglid, STRING("ターくん４"), LW(0))
    CALL(evt_npc_change_interrupt, STRING("ターくん４"), 1, 0)
    DELETE_EVENT(LW(0))
    CALL(evt_npc_get_reglid, STRING("ターくん５"), LW(0))
    CALL(evt_npc_change_interrupt, STRING("ターくん５"), 1, 0)
    DELETE_EVENT(LW(0))
    CALL(evt_npc_get_reglid, STRING("ターくん６"), LW(0))
    CALL(evt_npc_change_interrupt, STRING("ターくん６"), 1, 0)
    DELETE_EVENT(LW(0))
    RETURN
EVT_END()

static EVT_BEGIN(sekai_yami_shake)
    LOOP(0)
        CALL(evt_cam_shake, 4, FLOAT(0.0f), FLOAT(0.009765625f), 500)
    END_LOOP
    RETURN
EVT_END()

static EVT_BEGIN(sekai_yami_evt)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_mario_set_pos, 0, -1000, 0)
    CALL(evt_party_set_pos, 0, 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("ポワン探偵"), 40, -5, 100)
    CALL(evt_npc_set_ry, STRING("ポワン探偵"), 270)
    CALL(evt_map_set_tevcallback, 3, PTR(envSetYami))
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S"), MAP_OBJ_TEV_CALLBACK(3))
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S"), MAP_OBJ_FLAG_DISPLAY_LIST)
    CALL(anim_tevcallback, STRING("ポワン探偵"), 3)
    CALL(anim_tevcallback, STRING("ターくん５"), 3)
    CALL(evt_bg_set_color, 64, 64, 64, 255)
    CALL(yami_view, -800)
    CALL(evt_cam3d_evt_set, 15, 134, 667, 15, 38, 126, 0, 11)
    SPAWN_EVENT(PTR(sekai_yami_shake))
    CALL(evt_seq_wait, 2)
    CALL(evt_snd_sfxon, STRING("SFX_STG8_QUAKE1"), LW(0))
    WAIT_MS(800)
    CALL(evt_npc_reverse_ry, STRING("ポワン探偵"))
    WAIT_FRAMES(1)
    CALL(evt_npc_wait_pera, STRING("ポワン探偵"))
    CALL(evt_npc_reverse_ry, STRING("ポワン探偵"))
    WAIT_FRAMES(1)
    CALL(evt_npc_wait_pera, STRING("ポワン探偵"))
    CALL(evt_msg_print, 0, STRING("stg8_dark_07"), 0, STRING("ポワン探偵"))
    BEGIN_INLINE_EVENT
        LOOP(0)
            CALL(evt_npc_reverse_ry, STRING("ポワン探偵"))
            WAIT_FRAMES(1)
            CALL(evt_npc_wait_pera, STRING("ポワン探偵"))
            WAIT_MS(200)
        END_LOOP
    END_INLINE_EVENT
    WAIT_MS(1000)
    CALL(evt_snd_sfxoff, LW(0))
    CALL(evt_fade_set_mapchange_type, 1, 10, 300, 9, 300)
    CALL(evt_bero_mapchange, STRING("bom_02"), STRING("sekai_yami2"))
    RETURN
EVT_END()

static EVT_BEGIN(minnnanokoe_evt)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_mario_set_pos, 0, -1000, 0)
    CALL(evt_party_set_pos, 0, 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("ポワン探偵"), 90, -5, 10)
    CALL(evt_npc_set_ry, STRING("ポワン探偵"), 270)
    CALL(evt_npc_set_position, STRING("マダム"), 10, -5, 0)
    CALL(evt_npc_set_ry, STRING("マダム"), 90)
    CALL(evt_npc_flag_onoff, 1, STRING("マダム"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_set_position, STRING("コナリキン"), 70, 0, -70)
    CALL(evt_npc_set_ry, STRING("コナリキン"), 270)
    CALL(evt_npc_set_position, STRING("ナリキンママ"), 30, 0, -105)
    CALL(evt_npc_set_ry, STRING("ナリキンママ"), 90)
    CALL(evt_npc_set_position, STRING("ナリキンパパ"), 100, 5, -75)
    CALL(evt_npc_set_ry, STRING("ナリキンパパ"), 270)
    CALL(evt_map_set_tevcallback, 3, PTR(envSetYami))
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S"), MAP_OBJ_TEV_CALLBACK(3))
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S"), MAP_OBJ_FLAG_DISPLAY_LIST)
    CALL(anim_tevcallback, STRING("ポワン探偵"), 3)
    CALL(anim_tevcallback, STRING("マダム"), 3)
    CALL(anim_tevcallback, STRING("コナリキン"), 3)
    CALL(anim_tevcallback, STRING("ナリキンママ"), 3)
    CALL(anim_tevcallback, STRING("ナリキンパパ"), 3)
    CALL(anim_tevcallback, STRING("ターくん４"), 3)
    CALL(anim_tevcallback, STRING("ターくん５"), 3)
    CALL(anim_tevcallback, STRING("ターくん６"), 3)
    CALL(yami_view, -800)
    CALL(evt_bg_set_color, 64, 64, 64, 255)
    CALL(evt_cam3d_evt_set, 50, 120, 750, 50, 55, 0, 0, 11)
    CALL(starstone_entry, STRING("starstone"), 50, 50, 0)
    SET_VALUE(GW(0), 50)
    BEGIN_INLINE_EVENT
        LOOP(0)
            CALL(starstone_set_pos, STRING("starstone"), 50, GW(0), 0)
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
            CALL(evt_sub_intpl_msec_init, 11, 5000, 5500, 500)
            LABEL(1)
            CALL(evt_sub_intpl_msec_get_value)
            DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
            SET_FLOAT_VALUE(GW(0), LW(0))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 1)
                GOTO(1)
            END_IF
            CALL(evt_sub_intpl_msec_init, 11, 5500, 5000, 500)
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
    CALL(evt_seq_wait, 2)
    WAIT_MS(500)
    CALL(evt_npc_flag_onoff, 0, STRING("マダム"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_msg_print, 0, STRING("stg8_las_148_53"), 0, STRING("マダム"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg8_las_148_54"), 0, STRING("ポワン探偵"))
    CALL(evt_cam3d_evt_set, 50, 78, 349, 50, 47, 0, 0, 11)
    WAIT_FRAMES(1)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg8_las_148_55"), 0, STRING("ナリキンママ"))
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg8_las_148_56"), 0, STRING("ナリキンパパ"))
    CALL(evt_npc_set_ry, STRING("コナリキン"), 90)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg8_las_148_57"), 0, STRING("コナリキン"))
    CALL(evt_npc_set_ry, STRING("コナリキン"), 270)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg8_las_148_58"), 0, STRING("コナリキン"))
    CALL(evt_cam3d_evt_set, 50, 120, 750, 50, 55, 0, 0, 11)
    WAIT_FRAMES(1)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg8_las_148_59"), 0, STRING("マダム"))
    CALL(evt_msg_print, 0, STRING("stg8_las_148_60"), 0, STRING("ナリキンパパ"))
    CALL(evt_msg_print, 0, STRING("stg8_las_148_61"), 0, STRING("ナリキンママ"))
    CALL(evt_npc_reverse_ry, STRING("ポワン探偵"))
    WAIT_MS(500)
    CALL(evt_npc_reverse_ry, STRING("ポワン探偵"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg8_las_148_62"), 0, STRING("ポワン探偵"))
    BEGIN_INLINE_EVENT
        LOOP(0)
            CALL(evt_npc_reverse_ry, STRING("ポワン探偵"))
            WAIT_MS(500)
        END_LOOP
    END_INLINE_EVENT
    WAIT_MS(2000)
    CALL(evt_fade_set_mapchange_type, 1, 10, 300, 9, 300)
    CALL(evt_bero_mapchange, STRING("bom_02"), STRING("minnnanokoe"))
    RETURN
EVT_END()

static EVT_BEGIN(evt_switch_init)
    IF_INT_EQUAL(GSWF(1348), 0)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_dakan"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("A_dokan"), 1, 0)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(iri_27_npc_entry)
    CALL(evt_npc_entry, STRING("ルイージ"), STRING("c_luigi"))
    CALL(evt_npc_set_tribe, STRING("ルイージ"), STRING("ルイージ"))
    CALL(evt_npc_flag_onoff, 1, STRING("ルイージ"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_status_onoff, 1, STRING("ルイージ"), 2)
    CALL(evt_npc_set_position, STRING("ルイージ"), 0, -1000, 0)
    RETURN
EVT_END()

EVT_BEGIN(pik_01_init_evt)
    SET_VALUE(LF(0), 0)
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 373)
        IF_INT_LESS(GSW_STORY_PROGRESS, 375)
            CALL(evt_mario_set_mode, 2)
            SET_VALUE(LF(0), 1)
        END_IF
    END_IF
    CALL_CHILD(PTR(evt_switch_init))
    SET_VALUE(LW(0), PTR(bero_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_mobj_powerupblk, STRING("pblk01"), 400, 95, 125, 0, GSWF(5563), 0)
    IF_INT_EQUAL(LF(0), 1)
        SET_VALUE(LW(0), 0)
        CALL_CHILD(PTR(bero_case_switch_off))
        CALL(evt_hitobj_attr_onoff, 1, 0, STRING("nw_bero"), 8388612)
    END_IF
    SET_VALUE(LF(1), 0)
    IF_INT_EQUAL(LF(0), 0)
        IF_INT_EQUAL(GSW_STORY_PROGRESS, 400)
            CALL(evt_bero_get_entername, LW(0))
            IF_STR_EQUAL(LW(0), STRING("sekai_yami2"))
                CALL(evt_snd_bgmoff, 512)
                CALL(evt_snd_envoff, 512)
                SET_VALUE(LF(1), 1)
            END_IF
            IF_STR_EQUAL(LW(0), STRING("minnnanokoe"))
                CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_STG8_VOICE1"))
                CALL(evt_snd_envoff, 512)
                SET_VALUE(LF(1), 1)
            END_IF
        END_IF
        IF_INT_EQUAL(LF(1), 0)
            CALL(evt_snd_bgmon, 512, STRING("BGM_STG6_PIK1"))
            CALL(evt_snd_envon, 272, STRING("ENV_STG6_PIK1_1"))
        END_IF
        ELSE
        CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_KUPPA1"))
        CALL(evt_snd_bgm_scope, 0, 1)
        CALL(evt_snd_bgmon, 512, STRING("ENV_STG6_PIK1_1"))
    END_IF
    CALL(evt_map_playanim, STRING("anm_sibuki"), 1, 0)
    CALL(evt_map_playanim, STRING("anm_mizu"), 1, 0)
    CALL(evt_map_playanim, STRING("anm_hamon"), 1, 0)
    CALL(evt_map_playanim, STRING("anm_opuku"), 1, 0)
    SET_INT_READ_ADDR(PTR(&name_tbl))
    LOOP(2)
        READ_INT(LW(9))
        SPAWN_EVENT(PTR(evt_pik_yajirushi))
    END_LOOP
    CALL(evt_hit_damage_return_set, PTR(dmgReturnPt))
    CALL_CHILD(PTR(evt_gazigazi_entry))
    CALL(evt_map_set_tevcallback, 1, PTR(test_tev))
    CALL(evt_map_set_tevcallback, 2, PTR(test_tev2))
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("opuku"), MAP_OBJ_TEV_CALLBACK(2))
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("opuku"), MAP_OBJ_FLAG_DISPLAY_LIST)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("HEIPO_"), MAP_OBJ_TEV_CALLBACK(2))
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("HEIPO_"), MAP_OBJ_FLAG_DISPLAY_LIST)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("HEIPO_1"), MAP_OBJ_TEV_CALLBACK(2))
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("HEIPO_1"), MAP_OBJ_FLAG_DISPLAY_LIST)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("HEIPO_2"), MAP_OBJ_TEV_CALLBACK(2))
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("HEIPO_2"), MAP_OBJ_FLAG_DISPLAY_LIST)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("HEIPO_3"), MAP_OBJ_TEV_CALLBACK(2))
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("HEIPO_3"), MAP_OBJ_FLAG_DISPLAY_LIST)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("HEIPO_4"), MAP_OBJ_TEV_CALLBACK(2))
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("HEIPO_4"), MAP_OBJ_FLAG_DISPLAY_LIST)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("HEIPO_5"), MAP_OBJ_TEV_CALLBACK(2))
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("HEIPO_5"), MAP_OBJ_FLAG_DISPLAY_LIST)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("HEIPO_6"), MAP_OBJ_TEV_CALLBACK(2))
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("HEIPO_6"), MAP_OBJ_FLAG_DISPLAY_LIST)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("HEIPO_7"), MAP_OBJ_TEV_CALLBACK(2))
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("HEIPO_7"), MAP_OBJ_FLAG_DISPLAY_LIST)
    BEGIN_INLINE_EVENT
        SET_VALUE(LW(0), 0)
        LOOP(0)
            CALL(evt_mapobj_rotate, 1, STRING("opuku"), 0, LW(0), 0)
            ADD(LW(0), 1)
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_npc_setup, PTR(npc_data))
    IF_INT_EQUAL(LF(1), 0)
        IF_INT_EQUAL(GSW(1420), 27)
            CALL_CHILD(PTR(iri_27_npc_entry))
        END_IF
        IF_INT_EQUAL(GSWF(5401), 1)
            CALL(evt_item_entry, STRING("iri_27"), 214, -246, 15, 246, 16, GSWF(3278), 0)
        END_IF
    END_IF
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 403)
        CALL_CHILD(PTR(resaresa_entry))
        CALL_CHILD(PTR(sebaschan_entry))
    END_IF
    CALL_CHILD(PTR(takun4_init))
    CALL(evt_npc_change_interrupt, STRING("ターくん４"), 6, PTR(takun4_talk))
    CALL_CHILD(PTR(takun5_init))
    CALL(evt_npc_change_interrupt, STRING("ターくん５"), 6, PTR(takun5_talk))
    CALL_CHILD(PTR(takun6_init))
    CALL(evt_npc_change_interrupt, STRING("ターくん６"), 6, PTR(takun6_talk))
    IF_INT_LESS(GSW_STORY_PROGRESS, 335)
        CALL(evt_mobj_entry, STRING("lock"), STRING("MOBJ_Lock"))
        CALL(evt_mobj_set_position, STRING("lock"), 226, 30, -216)
        CALL(evt_mobj_set_anim, STRING("lock"), STRING("Z_1"))
        CALL(set_r)
        SET_VALUE(LW(0), 1)
        CALL_CHILD(PTR(bero_case_switch_off))
        CALL(evt_run_case_evt, 9, 1, STRING("a_harigami"), 0, PTR(evt_powan), 0)
        ELSE
        CALL(evt_mapobj_flag_onoff, 0, 1, STRING("kami"), MAP_OBJ_FLAG_HIDDEN)
    END_IF
    IF_INT_EQUAL(LF(0), 1)
        CALL(evt_mobj_entry, STRING("lock"), STRING("MOBJ_Lock"))
        CALL(evt_mobj_flag_onoff, 1, STRING("lock"), 32)
        CALL(evt_mobj_set_position, STRING("lock"), 226, 30, -216)
        CALL(evt_mobj_set_anim, STRING("lock"), STRING("Z_1"))
        CALL(set_r)
        CALL(kpa_lock_hit_scale)
        SET_VALUE(LW(0), 1)
        CALL_CHILD(PTR(bero_case_switch_off))
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("polySurface743_q00096"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_run_case_evt, 9, 1, STRING("MOBJ_lock"), 0, PTR(koopa_door_evt), 0)
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 373)
        CALL(evt_mario_kill_party, 0)
        SPAWN_EVENT(PTR(pik_01_koopa_evt))
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 374)
        CALL(evt_mario_set_pos, -227, 0, -227)
        CALL(evt_npc_set_position, STRING("ババ"), -255, 25, -300)
        CALL(evt_mario_set_dir, 90, 0, 0)
        CALL(evt_npc_set_ry, STRING("ババ"), 90)
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 400)
        CALL(evt_bero_get_entername, LW(0))
        IF_STR_EQUAL(LW(0), STRING("sekai_yami2"))
            CALL_CHILD(PTR(delete_npc_regl_evt))
            SPAWN_EVENT(PTR(sekai_yami_evt))
        END_IF
        IF_STR_EQUAL(LW(0), STRING("minnnanokoe"))
            CALL_CHILD(PTR(delete_npc_regl_evt))
            SPAWN_EVENT(PTR(minnnanokoe_evt))
        END_IF
    END_IF
    RETURN
EVT_END()

