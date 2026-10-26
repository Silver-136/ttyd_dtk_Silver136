#include "eff/n64/eff_kameki_tornade_n64.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_eff.h"
#include "evt/evt_fade.h"
#include "evt/evt_mario.h"
#include "evt/evt_msg.h"
#include "evt/evt_party.h"
#include "evt/evt_seq.h"
#include "evt/evt_sub.h"
#include "driver/npcdrv.h"
#include "driver/mapdrv.h"
#include "pmario_sound.h"
#include "mario/mario.h"

#include "driver/effdrv.h"
#include "manager/evtmgr_cmd.h"
#include "mario/mariost.h"
#include "system.h"
#include "evt/evt_bero.h"
#include "evt/evt_cam.h"
#include "evt/evt_damage.h"
#include "evt/evt_hit.h"
#include "evt/evt_map.h"
#include "evt/evt_mobj.h"
#include "evt/evt_npc.h"
#include "evt/evt_nannpc.h"
#include "evt/evt_snd.h"
#include "party/party_motion.h"
#include "muj.h"
#include "driver/hitdrv.h"
#include "data_tables.h"
#include "driver/seqdrv.h"

USER_FUNC(evt_mario_dispflag_onoff);
USER_FUNC(evt_mario_get_name_hitobj_ride);
USER_FUNC(evt_nannpc_entry);
USER_FUNC(suifu_option_func);
USER_FUNC(suifu_option_init_func);
#ifdef VERSION_G8ME01
#endif
static USER_FUNC(tornade_alpha_func);
void elmosalpha_func(NanNPC* npc);

static EvtWord megane_init[18];
static EvtWord megane_talk[8];
static NpcSetupInfo npcEnt[2];
static EvtWord e_close[12];
static EvtWord e_open[12];
static EvtWord door_custom[53];
static EvtWord bero_in_event[80];
static BeroEntry bero_entry_data[3];
EvtWord nazo_event[178];
EvtWord oidasare_event[156];
ExtTexInfo test_ext_entry_data[31];
static s16 suifu_animtbl[42];
static s16 elmos_animtbl[22];
s32 senin_event_suifupos[380];
s32 senin_event_erumospos[180];
#ifdef VERSION_G8ME01
EvtWord senin_event[282];
#else
EvtWord senin_event[374];
#endif
static EvtWord dai3_shurai_after[46];
static HitDamageReturn hitDmgRetPoints[7];
#ifdef VERSION_G8ME01
EvtWord muj_10_init_evt[207];
#else
EvtWord muj_10_init_evt[204];
#endif

static USER_FUNC(tornade_alpha_func) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    f32 alpha = evtGetFloat(event, args[1]);

    ((EffKamekiTornadeN64Work*)effNameToPtr(name)->userdata)->alphaScale = alpha;
    return EVT_RETURN_DONE;
}

USER_FUNC(suifu_option_func) {
    f32 y = evtGetFloat(event, event->args[0]);
    NanNPCWork* wp = nanNPCWork;
    s32 i;

    for (i = 0; i < 95; i++) {
        // "- -y" reproduces the operand order of the original fadds
        wp->npc[i].pos.y = wp->npc[i].basePos.y - -y;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(suifu_option_init_func) {
    NanNPCWork* wp = nanNPCWork;
    s32 i;

    for (i = 0; i < 95; i++) {
        wp->npc[i].basePos = wp->npc[i].pos;
    }
    return EVT_RETURN_DONE;
}

static EVT_BEGIN(megane_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(251, 252)
        CALL(evt_npc_set_position, STRING("me"), -455, 25, -40)
        CALL(evt_npc_set_ry, STRING("me"), 90)
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(megane_talk)
    CALL(evt_msg_print, 0, STRING("stg5_dou_19_00"), 0, STRING("me"))
    RETURN
EVT_END()

static NpcSetupInfo npcEnt[] = {
    {
        /*"めがね水夫"*/"\x82\xDF\x82\xAA\x82\xCB\x90\x85\x95\x76", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, // reactionFlags
        megane_init, // initEvent
        NULL, // regularEvent
        megane_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

static EVT_BEGIN(e_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("S_b_doa"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(e_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("S_b_doa"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(door_custom)
    CALL(evt_bero_get_info_anime, LW(1), LW(10), LW(11), LW(12), LW(13))
    IF_INT_NOT_EQUAL(LW(12), 0)
        SET_FLOAT_VALUE(LW(0), FLOAT(180.0f))
        CALL_CHILD(LW(12))
        CALL(evt_bero_mario_go)
        WAIT_MS(1500)
        SPAWN_EVENT(PTR(bero_close_door_play_se))
        SET_VALUE(LW(9), LW(12))
        CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 180, 0, 500)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(10))
            IF_INT_EQUAL(LW(10), 0)
                SET_FLOAT_VALUE(LW(0), FLOAT(0.0f))
                CALL_CHILD(LW(9))
                BREAK_LOOP
                ELSE
                CALL_CHILD(LW(9))
            END_IF
        END_LOOP
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(bero_in_event)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 258)
        SPAWN_EVENT(PTR(door_custom))
        CALL(evt_party_stop, 0)
        CALL(evt_party_stop, 1)
        BEGIN_INLINE_EVENT
            CALL(evt_mario_set_pos, -46, 45, -287)
            CALL(evt_mario_set_dir, 270, 0, 0)
            CALL(evt_mario_mov_pos2, -95, -151, FLOAT(120.0f))
        END_INLINE_EVENT
        BEGIN_INLINE_EVENT
            WAIT_MS(200)
            CALL(evt_party_set_pos, 0, -46, 45, -287)
            CALL(evt_party_set_dir, 0, 270, 0)
            CALL(evt_party_move_pos2, 0, -79, -190, FLOAT(120.0f))
        END_INLINE_EVENT
        BEGIN_INLINE_EVENT
            WAIT_MS(400)
            CALL(evt_party_set_pos, 1, -46, 45, -287)
            CALL(evt_party_set_dir, 1, 270, 0)
            CALL(evt_party_move_pos2, 1, -95, -227, FLOAT(120.0f))
        END_INLINE_EVENT
        SET_VALUE(LW(0), 1)
        RETURN
    END_IF
    SET_VALUE(LW(0), 0)
    RETURN
EVT_END()

static BeroEntry bero_entry_data[] = {
    {
        "w_bero", // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        bero_in_event, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "dou_11", // nextMapName
        "e_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "A_b_bero03", // hitName
        1, // type
        2, // sfxId
        2, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        bero_in_event, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "muj_11", // nextMapName
        "w_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        e_close, // enterAnimeArg
        e_open, // exitAnimeArg
    },
    {0},
};

EVT_BEGIN(nazo_event)
    LOOP(0)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_mario_get_name_hitobj_ride, LW(3))
        CALL(evt_mario_get_motion, LW(4))
        IF_INT_GREATER_OR_EQUAL(LW(0), -250)
            IF_STR_EQUAL(LW(3), STRING("A_hasi_ita"))
                IF_INT_NOT_EQUAL(LW(4), 31)
                    BREAK_LOOP
                END_IF
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_party_stop, 0)
    CALL(evt_npc_flag_onoff, 1, STRING("めがね水夫"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_flag_onoff, 1, STRING("めがね水夫"), NPC_FLAG_WALL_BOUNCE)
    CALL(evt_npc_set_position, STRING("めがね水夫"), -540, 25, 0)
    CALL(evt_npc_set_ry, STRING("めがね水夫"), 90)
    CALL(evt_cam3d_evt_set, FLOAT(-382.69922f), FLOAT(158.0f), FLOAT(527.7998f), FLOAT(-382.69922f), FLOAT(62.5f), FLOAT(-13.799805f), 500, 11)
    WAIT_MS(1000)
    BEGIN_INLINE_EVENT
        WAIT_MS(500)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_FIND1_2"), LW(0), LW(1), LW(2), 0)
        CALL(evt_eff_fukidashi, 0, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 60)
        CALL(evt_eff_fukidashi, 3, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 60)
        WAIT_MS(1000)
        CALL(evt_mario_set_dir, 270, 200, 0)
        CALL(evt_party_set_dir, 0, 270, 200)
    END_INLINE_EVENT
    CALL(evt_msg_print, 0, STRING("stg5_dou_19_00_00"), 0, STRING("めがね水夫"))
    WAIT_MS(1000)
    CALL(evt_npc_move_position, STRING("めがね水夫"), -454, 0, 0, FLOAT(120.0f), 0)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg5_dou_19"), 0, STRING("めがね水夫"))
    CALL(evt_npc_get_position, STRING("めがね水夫"), LW(0), LW(1), LW(2))
    CALL(evt_npc_move_position, STRING("めがね水夫"), LW(0), -40, 0, FLOAT(120.0f), 0)
    CALL(evt_cam3d_evt_off, 500, 11)
    SET_VALUE(GSW_STORY_PROGRESS, 251)
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(oidasare_event)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_mario_set_pos, 30, 50, -278)
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_party_set_pos, 0, 30, 50, -278)
    CALL(evt_party_set_dir, 0, 90, 0)
    CALL(evt_cam3d_evt_set, 220, 200, 480, 220, 70, -258, 0, 11)
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_OPEN_WOOD1"), 19, 118, -236, 0)
    WAIT_MS(1000)
    CALL(evt_mario_cont_onoff, 1)
    CALL(evt_party_cont_onoff, 0, 1)
    BEGIN_INLINE_EVENT
        CALL(evt_party_jump_pos, 0, 190, 50, -250, 600, 0, 30)
        CALL(evt_party_jump_pos, 0, 210, 50, -250, 300, 0, -1000)
    END_INLINE_EVENT
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_POSE_LOCK)
    CALL(evt_mario_set_pose, STRING("M_A_1A"))
    CALL(evt_mario_jump_pos, 230, 50, -250, 500, 2, 30)
    CALL(evt_mario_set_pose, STRING("M_D_7B"))
    CALL(evt_mario_jump_pos, 250, 50, -250, 250, 2, -1000)
    CALL(evt_mario_set_pos, 250, 50, -250)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_LAND_DAMAGE1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_snd_sfxon_3d, STRING("SFX_MARIO_FALL_CRUSH1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_mario_set_pose, STRING("M_D_8"))
    WAIT_MS(1000)
    CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_POSE_LOCK)
    CALL(evt_cam3d_evt_off, 500, 11)
    SET_VALUE(GSW_STORY_PROGRESS, 254)
    CALL(N_evt_bero_set_mario_bottomless_respawn_pos_to_current_mario_pos)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

ExtTexInfo test_ext_entry_data[] = {
    {"c_boro_k", "S_1", 0.0f},
    {"c_boro_k", "S_1", 2.0f},
    {"c_boro_k", "S_1", 4.0f},
    {"c_boro_k", "S_1", 6.0f},
    {"c_boro_k", "S_1", 8.0f},
    {"c_boro_k", "S_1", 10.0f},
    {"c_boro_k", "S_1", 12.0f},
    {"c_boro_k", "S_1", 14.0f},
    {"c_boro_k", "S_1", 16.0f},
    {"c_boro_k", "S_1", 18.0f},
    {"c_boro_k", "S_1", 20.0f},
    {"c_boro_k", "S_1", 22.0f},
    {"c_boro_k", "S_1", 24.0f},
    {"c_boro_k", "S_1", 26.0f},
    {"c_boro_k", "S_1", 28.0f},
    {"c_boro_k", "S_1", 30.0f},
    {"c_boro_k", "S_1", 32.0f},
    {"c_boro_k", "S_1", 34.0f},
    {"c_boro_k", "S_1", 36.0f},
    {"c_boro_k", "S_1", 38.0f},
    {"c_elmos", "S_1", 0.0f},
    {"c_elmos", "S_1", 2.0f},
    {"c_elmos", "S_1", 4.0f},
    {"c_elmos", "S_1", 6.0f},
    {"c_elmos", "S_1", 8.0f},
    {"c_elmos", "S_1", 10.0f},
    {"c_elmos", "S_1", 12.0f},
    {"c_elmos", "S_1", 14.0f},
    {"c_elmos", "S_1", 16.0f},
    {"c_elmos", "S_1", 18.0f},
    {0},
};

static s16 suifu_animtbl[] = {
    0, 0, 1, 1, 2, 2, 3, 3,
    4, 4, 5, 5, 6, 6, 7, 7,
    8, 8, 9, 9, 10, 10, 11, 11,
    12, 12, 13, 13, 14, 14, 15, 15,
    16, 16, 17, 17, 18, 18, 19, 19,
    -1, 0,
};

static s16 elmos_animtbl[] = {
    20, 20, 21, 21, 22, 22, 23, 23,
    24, 24, 25, 25, 26, 26, 27, 27,
    28, 28, 29, 29, -1, 0,
};

s32 senin_event_suifupos[] = {
    8, 120, -206, 269, -17, 120, -283, 270,
    21, 120, -308, 270, 33, 120, -308, 270,
    -17, 120, -295, 270, -6, 120, -220, 270,
    -4, 120, -205, 270, 0, 120, -193, 270,
    23, 120, -250, 270, 7, 120, -191, 270,
    43, 120, -215, 270, 54, 120, -201, 270,
    23, 120, -177, 270, 11, 120, -169, 270,
    6, 120, -154, 270, 22, 120, -159, 270,
    -7, 120, -262, 270, 40, 120, -203, 270,
    -4, 120, -185, 270, 0, 120, -275, 270,
    26, 120, -264, 270, 40, 120, -223, 270,
    34, 120, -169, 270, 33, 120, -186, 270,
    30, 120, -216, 270, 9, 120, -233, 270,
    14, 120, -219, 270, 25, 120, -200, 270,
    50, 120, -211, 270, 33, 120, -259, 270,
    10, 120, -295, 270, 10, 120, -284, 270,
    -1, 120, -307, 270, 4, 120, -299, 270,
    13, 120, -253, 270, 20, 120, -292, 270,
    13, 120, -253, 270, 34, 120, -236, 269,
    35, 120, -247, 270, 31, 120, -241, 270,
    49, 120, -193, 270, 3, 120, -258, 270,
    11, 120, -240, 270, 1, 120, -264, 270,
    39, 120, -232, 270, 15, 120, -266, 270,
    25, 120, -286, 270, 40, 120, -252, 270,
    14, 120, -272, 270, 22, 120, -280, 271,
    -305, 79, -124, 90, -286, 79, -116, 90,
    -290, 79, -139, 90, -273, 79, -124, 90,
    -259, 79, -112, 90, -233, 79, -113, 90,
    -204, 79, -122, 90, -247, 79, -128, 90,
    -231, 79, -130, 90, -237, 79, -203, 90,
    -249, 79, -192, 90, -261, 79, -184, 90,
    -272, 79, -174, 90, -287, 79, -154, 90,
    -274, 79, -151, 90, -270, 79, -163, 90,
    -274, 79, -135, 90, -258, 79, -128, 90,
    -251, 79, -176, 90, -235, 79, -102, 90,
    -238, 79, -139, 90, -250, 79, -153, 90,
    -247, 79, -144, 90, -254, 79, -145, 90,
    -254, 79, -163, 90, -237, 79, -163, 90,
    -205, 69, -105, 90, -193, 69, -100, 90,
    -187, 69, -115, 90, -207, 69, -141, 90,
    -217, 69, -180, 90, -199, 69, -168, 90,
    -209, 69, -195, 90, -214, 69, -215, 90,
    -227, 69, -220, 90, -230, 69, -192, 90,
    -213, 69, -142, 90, -211, 69, -158, 90,
    -195, 69, -152, 90, -186, 69, -137, 90,
    -218, 69, -167, 90, -207, 69, -174, 90,
    -224, 69, -201, 90, -185, 69, -125, 90,
    -218, 69, -152, 90,
};

s32 senin_event_erumospos[] = {
    -128, 167, -132, -121, 191, -133, -117, 157,
    -93, -112, 145, -78, -114, 167, -79, -143,
    145, -160, -153, 144, -192, -182, 145, -230,
    -190, 145, -254, -201, 145, -290, -146, 175,
    -187, -191, 179, -291, -176, 175, -241, -183,
    218, -263, -212, 219, -275, -157, 219, -238,
    -150, 218, -193, -121, 217, -139, -124, 219,
    -87, -120, 218, -109, -114, 283, -63, -121,
    283, -103, -151, 240, -172, -130, 283, -132,
    -158, 283, -221, -178, 283, -209, -171, 283,
    -261, -181, 283, -288, -178, 249, -282, -166,
    198, -254, -171, 252, -263, -180, 253, -220,
    -183, 196, -220, -152, 196, -213, -162, 196,
    -194, -141, 213, -167, -152, 213, -138, -146,
    233, -165, -136, 233, -112, -121, 208, -100,
    -116, 209, -126, -144, 184, -179, -159, 183,
    -237, -178, 184, -286, -193, 264, -291, -168,
    245, -247, -147, 238, -206, -125, 243, -154,
    -109, 243, -87, -88, 244, -75, -78, 209,
    -94, -87, 251, -137, -87, 266, -186, -118,
    266, -216, -124, 253, -271, -124, 253, -271,
    -132, 252, -275, -147, 252, -280, -149, 250,
    -305, -126, 252, -242,
};

EVT_BEGIN(senin_event)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_nannpc_init, PTR(test_ext_entry_data), 0, 0, 0, 155)
    CALL(evt_nannpc_set_subfunc, 1, PTR(nannpc_ext_main_sub_fast))
    CALL(evt_nannpcwork_flag_onoff, 0, 1)
    SET_INT_READ_ADDR(PTR(senin_event_suifupos))
    SET_VALUE(LW(0), 0)
    LOOP(95)
        CALL(evt_nannpc_entry, LW(0))
        CALL(evt_nannpc_set_func, LW(0), 0)
        CALL(evt_nannpc_set_animtbl, LW(0), PTR(suifu_animtbl))
        READ_INT4(LW(1), LW(2), LW(3), LW(4))
        CALL(evt_nannpc_set_position, LW(0), LW(1), LW(2), LW(3))
        CALL(evt_nannpc_set_dir, LW(0), LW(4))
        CALL(evt_nannpc_ppflag_onoff, LW(0), 0, 4)
        ADD(LW(0), 1)
    END_LOOP
    SET_INT_READ_ADDR(PTR(senin_event_erumospos))
    SET_VALUE(LW(0), 95)
    LOOP(60)
        CALL(evt_nannpc_entry, LW(0))
        CALL(evt_nannpc_set_func, LW(0), 0)
        CALL(evt_nannpc_set_animtbl, LW(0), PTR(elmos_animtbl))
        READ_INT3(LW(1), LW(2), LW(3))
        CALL(evt_nannpc_set_position, LW(0), LW(1), LW(2), LW(3))
        CALL(evt_nannpc_set_dir, LW(0), 90)
        CALL(evt_nannpc_set_color, LW(0), 255, 255, 255, 0)
        CALL(evt_nannpc_ppflag_onoff, LW(0), 0, 4)
        ADD(LW(0), 1)
    END_LOOP
    SET_RAW_VALUE(LW(0), GW(0))
    SET_FLOAT_VALUE(LW(1), FLOAT(1.5f))
    SPAWN_EVENT(PTR(muj_ship_puka))
    CALL(evt_hit_bind_mapobj, STRING("A_b"), STRING("S_b"))
    BEGIN_INLINE_EVENT
        LOOP(0)
            CALL(evt_mapobj_trans, 1, STRING("S_b"), 0, GW(0), 0)
            CALL(evt_hit_bind_update, STRING("A_b"))
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(suifu_option_init_func)
        LOOP(0)
            CALL(suifu_option_func, GW(0))
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_npc_entry, STRING("オドオド水夫"), STRING("c_odo_k"))
    CALL(evt_npc_set_tribe, STRING("オドオド水夫"), STRING("オドオド水夫"))
    CALL(evt_npc_entry, STRING("ピートン"), STRING("c_m_waka"))
    CALL(evt_npc_set_tribe, STRING("ピートン"), STRING("ピートン（指輪）"))
    CALL(evt_npc_set_tribe, STRING("モニー"), STRING("モニー（指輪）"))
    CALL(evt_npc_set_position, STRING("コルテス"), 0, -1000, 0)
    CALL(evt_npc_set_ry, STRING("コルテス"), 90)
    CALL(evt_npc_flag_onoff, 1, STRING("コルテス"), NPC_FLAG_INSTANT_TURN | NPC_FLAG_NO_SHADOW | NPC_FLAG_SHADOW_NO_FLOOR)
    CALL(evt_npc_set_position, STRING("水夫D"), -158, 44, -160)
    CALL(evt_npc_set_ry, STRING("水夫D"), 90)
    CALL(evt_npc_flag_onoff, 1, STRING("水夫D"), NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_TALK_POSE | NPC_FLAG_INSTANT_TURN | NPC_FLAG_GRAVITY | NPC_FLAG_SHADOW_NO_FLOOR)
    CALL(evt_npc_flag_onoff, 0, STRING("水夫D"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_set_position, STRING("モニー"), -159, 44, -257)
    CALL(evt_npc_set_ry, STRING("モニー"), 90)
    CALL(evt_npc_flag_onoff, 1, STRING("モニー"), NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_TALK_POSE | NPC_FLAG_INSTANT_TURN | NPC_FLAG_GRAVITY | NPC_FLAG_SHADOW_NO_FLOOR)
    CALL(evt_npc_flag_onoff, 0, STRING("モニー"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_set_position, STRING("ピートン"), -195, 44, -264)
    CALL(evt_npc_set_ry, STRING("ピートン"), 90)
    CALL(evt_npc_flag_onoff, 1, STRING("ピートン"), NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_TALK_POSE | NPC_FLAG_INSTANT_TURN | NPC_FLAG_GRAVITY | NPC_FLAG_SHADOW_NO_FLOOR)
    CALL(evt_npc_flag_onoff, 0, STRING("ピートン"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_set_position, STRING("ガラの悪い水夫"), -45, 120, -289)
    CALL(evt_npc_set_ry, STRING("ガラの悪い水夫"), 270)
    CALL(evt_npc_flag_onoff, 1, STRING("ガラの悪い水夫"), NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_TALK_POSE | NPC_FLAG_INSTANT_TURN | NPC_FLAG_GRAVITY | NPC_FLAG_SHADOW_NO_FLOOR)
    CALL(evt_npc_flag_onoff, 0, STRING("ガラの悪い水夫"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_set_position, STRING("オドオド水夫"), -18, 120, -179)
    CALL(evt_npc_flag_onoff, 1, STRING("オドオド水夫"), NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_TALK_POSE | NPC_FLAG_INSTANT_TURN | NPC_FLAG_GRAVITY | NPC_FLAG_SHADOW_NO_FLOOR)
    CALL(evt_npc_flag_onoff, 0, STRING("オドオド水夫"), NPC_FLAG_AUTO_MOVE_ANIM)
#ifndef VERSION_G8ME01
    CALL(evt_cam3d_evt_set, -71, 178, 259, -71, 83, -282, 0, 11)
    CALL(evt_bero_exec_wait, 65536)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg5_dou_19_01"), 0, STRING("水夫D"))
    CALL(evt_msg_print, 0, STRING("stg5_dou_19_02"), 0, STRING("モニー"))
    CALL(evt_msg_print, 0, STRING("stg5_dou_19_03"), 0, STRING("ガラの悪い水夫"))
    CALL(evt_cam3d_evt_set, -150, 300, 682, -150, 150, -175, 500, 11)
    WAIT_MS(500)
    CALL(evt_npc_set_position, STRING("コルテス"), -215, 120, -160)
    CALL_CHILD(PTR(korutesu_apper))
    CALL(evt_msg_print, 0, STRING("stg5_dou_19_04"), 0, STRING("コルテス"))
    CALL(evt_snd_sfxon, STRING("SFX_STG5_ELMOS_APPEAR3"), 0)
    SET_VALUE(LW(0), 95)
    LOOP(60)
        CALL(evt_nannpc_set_func, LW(0), PTR(elmosalpha_func))
        ADD(LW(0), 1)
    END_LOOP
    WAIT_MS(3000)
    SET_VALUE(GSW_STORY_PROGRESS, 259)
    CALL(evt_fade_set_mapchange_type, 0, 2, 500, 1, 500)
    CALL(evt_bero_mapchange, STRING("muj_00"), STRING(""))
#endif
    RETURN
EVT_END()

#ifdef VERSION_G8ME01
EVT_BEGIN(korutesu_appear_event)
    CALL(evt_cam3d_evt_set, -71, 178, 259, -71, 83, -282, 0, 11)
    CALL(evt_bero_exec_wait, 65536)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg5_dou_19_01"), 0, STRING("水夫D"))
    CALL(evt_msg_print, 0, STRING("stg5_dou_19_02"), 0, STRING("モニー"))
    CALL(evt_msg_print, 0, STRING("stg5_dou_19_03"), 0, STRING("ガラの悪い水夫"))
    CALL(evt_cam3d_evt_set, -150, 300, 682, -150, 150, -175, 500, 11)
    WAIT_MS(500)
    CALL(evt_npc_set_position, STRING("コルテス"), -215, 120, -160)
    CALL_CHILD(PTR(korutesu_apper))
    CALL(evt_msg_print, 0, STRING("stg5_dou_19_04"), 0, STRING("コルテス"))
    CALL(evt_snd_sfxon, STRING("SFX_STG5_ELMOS_APPEAR3"), 0)
    SET_VALUE(LW(0), 95)
    LOOP(60)
        CALL(evt_nannpc_set_func, LW(0), PTR(elmosalpha_func))
        ADD(LW(0), 1)
    END_LOOP
    WAIT_MS(3000)
    SET_VALUE(GSW_STORY_PROGRESS, 259)
    CALL(evt_fade_set_mapchange_type, 0, 2, 500, 1, 500)
    CALL(evt_bero_mapchange, STRING("muj_00"), STRING(""))
    RETURN
EVT_END()
#endif

static EVT_BEGIN(dai3_shurai_after)
    LOOP(0)
        CALL(evt_sub_random, 2500, LW(10))
        WAIT_MS(LW(10))
        WAIT_MS(1500)
        BEGIN_INLINE_EVENT
            CALL(evt_cam_shake, 4, FLOAT(0.0048828125f), FLOAT(0.0f), 500)
        END_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        ADD(LW(1), 250)
        CALL(evt_eff64, STRING(""), STRING("hokori_n64"), 0, LW(0), LW(1), LW(2), 60, 0, 0, 0, 0, 0, 0, 0)
    END_LOOP
    RETURN
EVT_END()

static HitDamageReturn hitDmgRetPoints[] = {
    {"A_yuka01", {0.0f, -1000.0f, 0.0f}},
    {"A_yuka02", {0.0f, -1000.0f, 0.0f}},
    {"A_yuka03", {0.0f, -1000.0f, 0.0f}},
    {"A_yuka04", {0.0f, -1000.0f, 0.0f}},
    {"A_hasi_ita", {0.0f, -1000.0f, 0.0f}},
    {"ship_change_", {0.0f, -1000.0f, 0.0f}},
    {0},
};

EVT_BEGIN(muj_10_init_evt)
    IF_INT_LESS(GSW_STORY_PROGRESS, 253)
        CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_STG5_GHOST1"))
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG5_MUJ8"))
#ifdef VERSION_G8ME01
        ELSE
#else
    ELSE
#endif
        CALL(evt_snd_bgmon, 512, STRING("BGM_STG5_DOU1"))
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG5_MUJ8"))
    END_IF
    CALL(evt_snd_set_rev_mode, 2)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
#ifdef VERSION_G8ME01
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_cam_road_reset)
    CALL(evt_npc_setup, PTR(npcEnt))
    CALL(evt_npc_sound_data_set, STRING("めがね水夫"), STRING("SFX_STG5_KANBU_GLASSES_MOVE1L"), STRING("SFX_STG5_KANBU_GLASSES_MOVE1R"), EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 250)
        SPAWN_EVENT(PTR(nazo_event))
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 253)
        SPAWN_EVENT(PTR(oidasare_event))
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 258)
        CALL_CHILD(PTR(senin_event))
        SPAWN_EVENT(PTR(korutesu_appear_event))
#else
    CALL_CHILD(evt_bero_info_run)
    CALL(evt_cam_road_reset)
    CALL(evt_npc_setup, PTR(npcEnt))
    CALL(evt_npc_sound_data_set, STRING(/*"めがね水夫"*/"\x82\xDF\x82\xAA\x82\xCB\x90\x85\x95\x76"), STRING("SFX_STG5_KANBU_GLASSES_MOVE1L"), STRING("SFX_STG5_KANBU_GLASSES_MOVE1R"), EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 250)
        SPAWN_EVENT(nazo_event)
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 253)
        SPAWN_EVENT(oidasare_event)
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 258)
        SPAWN_EVENT(senin_event)
#endif
    END_IF
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 260)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_b"), MAP_OBJ_FLAG_HIDDEN)
    END_IF
    CALL(evt_map_playanim, STRING("mizu"), 1, 0)
    CALL(evt_map_playanim, STRING("noname"), 1, 0)
    IF_INT_LESS(GSW_STORY_PROGRESS, 260)
        IF_INT_NOT_EQUAL(GSW_STORY_PROGRESS, 258)
            CALL(evt_map_playanim, STRING("fune"), 1, 0)
        END_IF
        BEGIN_INLINE_EVENT
            LOOP(0)
                CALL(evt_mapobj_get_position, STRING("S_b"), LW(0), LW(1), LW(2))
                CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_SHIP_KOR_SHAKE1"), LW(0), LW(1), LW(2), 0)
                WAIT_MS(2000)
            END_LOOP
        END_INLINE_EVENT
    END_IF
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 260)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_hasi_rope01"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_hasi_rope02"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_hasi_m"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_10_sui02_01"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("A_hasi_ita"), 1, 0)
#ifdef VERSION_G8ME01
        ELSE
#else
    ELSE
#endif
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_hasi_zan"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_10_sui02_02"), MAP_OBJ_FLAG_HIDDEN)
    END_IF
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 257)
        IF_INT_LESS(GSW_STORY_PROGRESS, 258)
#ifdef VERSION_G8ME01
            SPAWN_EVENT(PTR(dai3_shurai_after))
#else
            SPAWN_EVENT(dai3_shurai_after)
#endif
        END_IF
    END_IF
    CALL(evt_hit_damage_return_set, PTR(hitDmgRetPoints))
#ifdef VERSION_G8ME01
    CALL_CHILD(PTR(evt_gazigazi_entry))
#else
    CALL_CHILD(evt_gazigazi_entry)
#endif
    CALL(evt_hitobj_onoff, STRING("A_b_bero02"), 1, 0)
    IF_INT_LESS(GSW_STORY_PROGRESS, 258)
        CALL(evt_mobj_recovery_blk, STRING("r_blk"), 12, -260, 135, -145, 0, 0)
        CALL(evt_mobj_save_blk, STRING("s_blk"), -45, 83, 0, 0, 0)
    END_IF
    RETURN
EVT_END()

void elmosalpha_func(NanNPC* npc) {
    s32 elapsed;

    switch (npc->state) {
        case 0:
            npc->startTime = gp->renderTime;
            npc->duration = irand(1000);
            npc->state = 1;
        case 1:
            if ((s32)OSTicksToMilliseconds((u32)gp->renderTime - (u32)npc->startTime) >= npc->duration) {
                npc->startTime = gp->renderTime;
                npc->duration = 500;
                npc->state = 2;
            }
            break;
        case 2:
            elapsed = OSTicksToMilliseconds((u32)gp->renderTime - (u32)npc->startTime);
            if (elapsed < npc->duration) {
                npc->color.a = intplGetValue(EASING_LINEAR, 0.0f, 255.0f, elapsed, npc->duration);
            } else {
                npc->state = 3;
            }
            break;
        case 3:
            break;
    }
}

