#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "libc_decls.h"
#include "mario/mario_sbr.h"
#include "evt/evt_bg.h"
#include "evt/evt_cam.h"
#include "evt/evt_eff.h"
#include "evt/evt_ext.h"
#include "evt/evt_fade.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_mobj.h"
#include "evt/evt_msg.h"
#include "evt/evt_party.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_item.h"
#include "evt/evt_sub.h"
#include "mario/mario_pouch.h"
#include "driver/itemdrv.h"

#include "party.h"
#include "manager/evtmgr_cmd.h"
#include "driver/npcdrv.h"
#include "mario/mario.h"
#include "mario/mariost.h"
#include "string.h"
#include "memory.h"
#include "dolphin/mtx.h"
#include "pmario_sound.h"
#include "driver/dispdrv.h"
#include "driver/camdrv.h"
#include "evt/evt_bero.h"
#include "evt/evt_npc.h"
#include "data_tables.h"
#include "driver/extdrv.h"

void partyChgPose(PartyEntry* party, const char* pose);
typedef struct Senkaron {
    s8 state;
    s32 timer;
    f32 vx;
    f32 vy;
    f32 vz;
    u32 flags;
} Senkaron;
Senkaron* senkaron;
s32 senkaronAttackStartFlag;
s32 senkaronEscapeFlag;
s32 senkaronMesPushingFlag;
s32 marioPushedFlag;
s32 marioHammerFrame;
static f32 randf(f32 min, f32 max);
double sqrt(double x);
static void senkaron_ext_disp(CameraId camera, void* param);
static void senkaron_ext_shadow_disp(CameraId camera, void* param);

USER_FUNC(evt_mario_dispflag_onoff);
static USER_FUNC(senkaronAttackStart);
static USER_FUNC(senkaronFlagOffMesPushing);
static USER_FUNC(senkaronFlagOnMesPushing);
static USER_FUNC(senkaronGetMarioPushedFlag);
static USER_FUNC(senkaronToEscape);
USER_FUNC(leader_touch_check);
void nokotaro_callback(s32 type);
static void senkaron_ext_init(void);
static void senkaron_ext_main(void);
static void senkaron_ext_dispent(void);

static NpcSetupInfo npcEnt[4];
static EvtWord door01_close[12];
static EvtWord door01_open[12];
static EvtWord door02_close[12];
static EvtWord door02_open[12];
static BeroEntry bero_entry_data[3];
const char* nokotaro_pose[2];
EvtWord tegami_event[305];
static EvtWord senkaron_event[579];
static EvtWord key_get[5];
EvtWord gon_03_init_evt[256];
EvtWord richard_init_event[29];
EvtWord richard_regl_event[2];
EvtWord richard_talk_event[53];
EvtWord leaderkaron_init_event[44];
EvtWord leaderkaron_talk_event[40];
static ExtTexInfo senkaron_ext_entry_data[26];
EvtWord senkarondummy_init_event[8];
EvtWord senkarondummy_regl_event[2];
EvtWord senkarondummy_talk_event[2];
EvtWord senkaron_entry_event[9];
EvtWord senkaron_escape_event[4];

static NpcSetupInfo npcEnt[] = {
    {
        /*"カロン1"*/"\x83\x4A\x83\x8D\x83\x93\x31", // name
        0, 0, // flags..reactionFlags
        senkarondummy_init_event, // initEvent
        senkarondummy_regl_event, // regularEvent
        senkarondummy_talk_event, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"リチャード"*/"\x83\x8A\x83\x60\x83\x83\x81\x5B\x83\x68", // name
        0, 0, // flags..reactionFlags
        richard_init_event, // initEvent
        richard_regl_event, // regularEvent
        richard_talk_event, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"リーダー"*/"\x83\x8A\x81\x5B\x83\x5F\x81\x5B", // name
        0, 0, // flags..reactionFlags
        leaderkaron_init_event, // initEvent
        NULL, // regularEvent
        leaderkaron_talk_event, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

static EVT_BEGIN(door01_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("door01"), 0, LW(0), 0)
    RETURN
EVT_END()


static EVT_BEGIN(door01_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("door01"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(door02_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("door02"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(door02_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("door02"), 0, LW(0), 0)
    RETURN
EVT_END()

static BeroEntry bero_entry_data[] = {
    {
        "w_bero", // hitName
        1, // type
        12, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "gon_02", // nextMapName
        "e_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        door01_close, // enterAnimeArg
        door01_open, // exitAnimeArg
    },
    {
        "e_bero", // hitName
        1, // type
        12, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "gon_04", // nextMapName
        "w_bero_1", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        door02_close, // enterAnimeArg
        door02_open, // exitAnimeArg
    },
    {0},
};

const char* nokotaro_pose[] = {
    "PNK_L_1",
    "PNK_L_2",
};

EVT_BEGIN(tegami_event)
    LOOP(0)
        WAIT_FRAMES(1)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        IF_INT_GREATER_OR_EQUAL(LW(0), -130)
            BREAK_LOOP
        END_IF
    END_LOOP
    SET_VALUE(GSW_STORY_PROGRESS, STG1_10_FATHERS_LETTER)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_get_party, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 2)
        CALL(evt_mario_goodbye_party, 0)
        CALL(evt_mario_hello_party, 0, 2)
    END_IF
    CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_party_stop, 0)
    CALL(evt_eff_fukidashi, 3, STRING("fukidashi"), 0, 0, 0, 0, 0, 0, 0, 0, 0)
    WAIT_MS(1300)
    CALL(evt_msg_print, 0, STRING("stg1_gon_02"), 0, STRING("party"))
    LOOP(0)
        WAIT_FRAMES(1)
        CALL(evt_party_get_pos, 0, LW(0), LW(1), LW(2))
        IF_INT_LESS_OR_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_cam3d_evt_set, -30, 75, 150, -30, 25, -250, 3000, 11)
    CALL(evt_party_set_dir, 0, 270, -1)
    CALL(evt_party_move_pos, 0, -40, -110, 1500)
    BEGIN_SIBLING_EVENT
        CALL(evt_mario_mov_pos2, -80, -112, FLOAT(120.0f))
        CALL(evt_mario_wait_move_end)
        CALL(evt_mario_mov_pos2, -75, -100, FLOAT(300.0f))
        CALL(evt_mario_wait_move_end)
        CALL(evt_mario_set_dir, 90, 0, 0)
        CALL(evt_mario_set_pose, STRING("M_S_1"))
    END_SIBLING_EVENT
    WAIT_MS(1500)
    CALL(evt_msg_print, 0, STRING("stg1_gon_03"), 0, STRING("party"))
    CALL(evt_party_set_pose, 0, STRING("PNK_T_2"))
    CALL(evt_msg_npc, STRING("(^x^)party"))
    CALL(evt_msg_continue)
    WAIT_MS(1000)
    CALL(evt_eff_fukidashi, 3, STRING("fukidashi"), 0, 0, 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_party_set_pose, 0, STRING("PNK_S_1"))
    WAIT_MS(1300)
    CALL(evt_msg_print, 0, STRING("stg1_gon_04"), 0, STRING("party"))
    CALL(evt_party_set_dir, 0, 270, 200)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg1_gon_04_1"), 0, STRING("party"))
    CALL(evt_msg_select, 0, STRING("stg1_gon_05_yesno"))
    CALL(evt_msg_continue)
    IF_INT_EQUAL(LW(0), 0)
        SET_VALUE(GSWF(1481), 1)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_NOD1_5"), LW(0), LW(1), LW(2), 0)
        CALL(evt_mario_set_pose, STRING("M_N_2"))
        WAIT_MS(1000)
        CALL(evt_mario_set_pose, STRING("M_S_1"))
        CALL(evt_msg_print, 0, STRING("stg1_gon_07"), 0, STRING("party"))
        CALL(evt_party_set_dir, 0, 90, 200)
        WAIT_MS(500)
        CALL(evt_msg_print, 0, STRING("stg1_gon_08"), PTR(nokotaro_callback), STRING("(^x^)party"))
        WAIT_MS(1000)
        CALL(evt_party_set_dir, 0, 270, 200)
        CALL(evt_party_set_pose, 0, STRING("PNK_T_3"))
        CALL(evt_msg_print, 0, STRING("stg1_gon_09"), 0, STRING("(^x^)party"))
        CALL(evt_party_set_pose, 0, STRING("PNK_S_1"))
        CALL(evt_party_set_dir, 0, 90, 200)
        WAIT_MS(500)
        CALL(evt_msg_print, 0, STRING("stg1_gon_09_00_01"), 0, STRING("party"))
        ELSE
        CALL(evt_mario_set_pose, STRING("M_N_B"))
        WAIT_MS(1000)
        CALL(evt_mario_set_pose, STRING("M_S_1"))
        CALL(evt_msg_print, 0, STRING("stg1_gon_06"), 0, STRING("party"))
    END_IF
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_cam3d_evt_off, 1000, 11)
    WAIT_MS(1000)
    CALL(evt_party_cont_onoff, 0, 1)
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(senkaron_event)
    IF_INT_LESS_OR_EQUAL(GSW_STORY_PROGRESS, STG1_11_1000_DRY_BONES)
        CALL(senkaronFlagOffMesPushing)
        SET_VALUE(GW(14), 0)
        SET_VALUE(GW(15), 1)
        CALL(evt_npc_change_interrupt, STRING("リチャード"), 6, 0)
        CALL_CHILD(PTR(senkaron_entry_event))
        CALL(evt_snd_bgmoff, PSND_FADEOUT)
        CALL(evt_snd_envoff, PSND_FADEOUT)
        CALL(evt_mario_key_onoff, 0)
        CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_POSE_LOCK)
        CALL(evt_mario_set_pose, STRING("M_S_1"))
        CALL(evt_eff_fukidashi, 0, STRING("fukidashi"), 0, 0, 0, 0, 0, 0, 0, 0, 0)
        CALL(evt_npc_set_anim, STRING("リーダー"), STRING("KRN_D_3"))
        WAIT_MS(1000)
        CALL(evt_cam3d_evt_set, 350, 80, 310, 350, 30, -90, 300, 0)
        WAIT_MS(300)
        CALL(evt_msg_print, 0, STRING("stg1_gon_10"), 0, 0)
        WAIT_MS(300)
        BEGIN_INLINE_EVENT
            CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_DANGER4"))
        END_INLINE_EVENT
        BEGIN_SIBLING_EVENT
            CALL(evt_mario_set_pose, STRING("M_N_5B"))
            WAIT_MS(500)
            CALL(evt_mario_set_pose, STRING("M_I_Y"))
        END_SIBLING_EVENT
        SET_VALUE(LW(0), 0)
        CALL(evt_npc_set_anim, STRING("リーダー"), STRING("KRN_D_4"))
        LOOP(0)
            SET_FLOAT_VALUE(LW(10), FLOAT(0.0f))
            SET_FLOAT_VALUE(LW(11), FLOAT(10.0f))
            SET_FLOAT_VALUE(LW(12), FLOAT(-1.0f))
            LOOP(0)
                WAIT_FRAMES(1)
                CALL(evt_npc_get_position, STRING("リーダー"), LW(13), LW(10), LW(14))
                ADD_FLOAT(LW(10), LW(11))
                ADD_FLOAT(LW(11), LW(12))
                IF_INT_LESS_OR_EQUAL(LW(10), FLOAT(0.0f))
                    SET_FLOAT_VALUE(LW(10), FLOAT(0.0f))
                    CALL(evt_npc_set_position, STRING("リーダー"), LW(13), LW(10), LW(14))
                    BREAK_LOOP
                END_IF
                CALL(evt_npc_set_position, STRING("リーダー"), LW(13), LW(10), LW(14))
            END_LOOP
            ADD(LW(0), 1)
            IF_INT_GREATER_OR_EQUAL(LW(0), 1)
                BREAK_LOOP
            END_IF
        END_LOOP
        CALL(evt_npc_set_anim, STRING("リーダー"), STRING("KRN_R_1"))
        CALL(evt_cam3d_evt_off, 300, 0)
        WAIT_MS(700)
        CALL(evt_snd_sfxon, STRING("SFX_STG1_HONE_DOWN1_2"), 0)
        CALL(senkaronAttackStart)
        BEGIN_INLINE_EVENT
            WAIT_MS(400)
            CALL(evt_cam_shake, 4, FLOAT(0.0f), FLOAT(0.001953125f), 1500)
        END_INLINE_EVENT
        SET_FLOAT_VALUE(LW(3), FLOAT(-1.0996094f))
        SET_FLOAT_VALUE(LW(4), FLOAT(0.009765625f))
        SET_VALUE(LW(5), 90)
        WAIT_MS(1300)
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG1_HONE__MOVE1"), 0, 0, 0, GW(13))
        CALL(evt_mario_set_pose, STRING("M_I_O"))
        WAIT_FRAMES(1)
        CALL(evt_mario_cont_onoff, 0)
        CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_NO_MAKE_DISP_DIR)
        LOOP(0)
            WAIT_FRAMES(1)
            CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
            IF_INT_LESS_OR_EQUAL(LW(0), 100)
                BREAK_LOOP
            END_IF
            ADD_FLOAT(LW(0), LW(3))
            ADD_FLOAT(LW(3), LW(4))
            IF_INT_GREATER_OR_EQUAL(LW(3), FLOAT(-0.009765625f))
                SET_FLOAT_VALUE(LW(3), FLOAT(-0.009765625f))
            END_IF
            CALL(evt_mario_set_pos, LW(0), LW(1), LW(2))
            ADD(LW(5), 10)
            IF_INT_GREATER_OR_EQUAL(LW(5), 360)
                SUBTRACT(LW(5), 360)
            END_IF
            CALL(evt_mario_set_dispdir, LW(5))
        END_LOOP
        CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_NO_MAKE_DISP_DIR)
        CALL(evt_mario_cont_onoff, 1)
        CALL(evt_cam_letter_box_disable, 1)
        CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_POSE_LOCK)
        CALL(evt_mario_key_onoff, 1)
        SET_VALUE(LW(0), STRING("w_bero"))
        CALL_CHILD(PTR(bero_case_switch_off))
        IF_INT_EQUAL(GSWF(1479), 0)
            CALL(evt_mario_cont_onoff, 0)
            CALL(senkaronFlagOnMesPushing)
            CALL(evt_mario_get_party, LW(3))
            SWITCH(LW(3))
                CASE_EQUAL(1)
                CALL(evt_msg_print, 0, STRING("stg1_gon_11"), 0, STRING("party"))
                CASE_EQUAL(2)
                CALL(evt_msg_print, 0, STRING("stg1_gon_12"), 0, STRING("party"))
            END_SWITCH
            SET_VALUE(GSWF(1479), 1)
            CALL(senkaronFlagOffMesPushing)
            CALL(evt_mario_cont_onoff, 1)
        END_IF
        CALL(evt_cam_letter_box_disable, 0)
        BEGIN_SIBLING_EVENT
            LOOP(0)
                CALL(senkaronGetMarioPushedFlag, LW(6))
                IF_INT_NOT_EQUAL(LW(6), 0)
                    SET_VALUE(LW(7), 18)
                END_IF
                IF_INT_GREATER(LW(7), 0)
                    CALL(evt_mario_get_motion, LW(8))
                    IF_INT_EQUAL(LW(8), 0)
                        CALL(evt_mario_get_pose, LW(9))
                        IF_STR_NOT_EQUAL(LW(9), STRING("M_I_O"))
                            CALL(evt_mario_set_pose, STRING("M_I_O"))
                        END_IF
                        CALL(evt_mario_get_dispdir, LW(5))
                        ADD(LW(5), 10)
                        IF_INT_GREATER_OR_EQUAL(LW(5), 360)
                            SUBTRACT(LW(5), 360)
                        END_IF
                        CALL(evt_mario_set_dispdir, LW(5))
                        SUBTRACT(LW(7), 1)
                        IF_INT_EQUAL(LW(7), 0)
                            CALL(evt_mario_set_pose, STRING("M_S_1"))
                        END_IF
                        ELSE
                        SET_VALUE(LW(7), 0)
                    END_IF
                END_IF
                WAIT_FRAMES(1)
            END_LOOP
        END_SIBLING_EVENT
        LOOP(0)
            WAIT_FRAMES(1)
            CALL(leader_touch_check)
            IF_INT_GREATER_OR_EQUAL(LW(0), 1)
                BREAK_LOOP
            END_IF
            CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
            CALL(evt_sub_area_check, 0, -390, 0, 50, LW(0), LW(2), LW(10))
            IF_INT_EQUAL(LW(10), 1)
                IF_INT_EQUAL(GW(14), 0)
                    SET_VALUE(GW(14), 1)
                    CALL(evt_map_playanim, STRING("m_door01_b"), 0, 0)
                END_IF
            END_IF
            CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
            CALL(evt_sub_area_check, 0, -390, 0, 20, LW(0), LW(2), LW(10))
            IF_INT_EQUAL(LW(10), 1)
                SET_VALUE(GW(15), 0)
                CALL(evt_npc_change_interrupt, STRING("リチャード"), 6, PTR(richard_talk_event))
                CALL(evt_mario_key_onoff, 0)
                BEGIN_INLINE_EVENT
                    LOOP(0)
                        CALL(evt_mario_get_dispdir, LW(5))
                        ADD(LW(5), 10)
                        CALL(evt_mario_set_dispdir, LW(5))
                        WAIT_FRAMES(1)
                    END_LOOP
                END_INLINE_EVENT
                SET_VALUE(GSWF(1476), 1)
                IF_INT_EQUAL(GSWF(1477), 0)
                    SET_VALUE(GSWF(1477), 1)
                    ELSE
                    SET_VALUE(GSWF(1478), 1)
                END_IF
                CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
                ADD(LW(1), 50)
                CALL(evt_fade_set_spot_pos, LW(0), LW(1), LW(2))
                CALL(evt_fade_set_mapchange_type, 0, 4, 500, 9, 300)
                CALL(evt_bero_mapchange, STRING("gon_01"), STRING("e_bero"))
            END_IF
        END_LOOP
        CALL(senkaronFlagOnMesPushing)
        CALL(evt_npc_set_battle_info, STRING("リーダー"), 6)
        CALL(evt_npc_battle_start, STRING("リーダー"))
        CALL(evt_npc_wait_battle_end)
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG1_GON1"))
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG1_GON1"))
        CALL(evt_snd_sfxoff, GW(13))
        SET_VALUE(GW(15), 0)
        CALL(evt_npc_change_interrupt, STRING("リチャード"), 6, PTR(richard_talk_event))
        CALL(evt_npc_set_position, STRING("リーダー"), 0, -1000, 0)
        CALL(evt_snd_sfxon, STRING("SFX_STG1_HONE_ESCAPE1"), 0)
        SPAWN_EVENT(PTR(senkaron_escape_event))
        SET_VALUE(GSW_STORY_PROGRESS, STG1_11_1000_DRY_BONES)
        CALL(evt_map_playanim, STRING("m_door01_a"), 0, 0)
        SET_VALUE(LW(0), STRING("w_bero"))
        CALL_CHILD(PTR(bero_case_switch_on))
        SET_VALUE(LW(0), STRING("e_bero"))
        CALL_CHILD(PTR(bero_case_switch_on))
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(key_get)
    SET_VALUE(GSW_STORY_PROGRESS, STG1_13_02_KEY_2)
    RETURN
EVT_END()

EVT_BEGIN(gon_03_init_evt)
    CALL(evt_bg_disp_onoff, 0)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    SET_VALUE(LW(0), STRING("e_bero"))
    CALL_CHILD(PTR(bero_case_switch_off))
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG1_GON1"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG1_GON1"))
    CALL(evt_snd_set_rev_mode, 2)
    SET_VALUE(GW(15), 0)
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG1_11_1000_DRY_BONES)
        SET_VALUE(LW(0), STRING("e_bero"))
        CALL_CHILD(PTR(bero_case_switch_on))
    END_IF
    CALL(evt_npc_setup, PTR(npcEnt))
    CALL(evt_npc_change_fbat_mode, 1)
    IF_INT_LESS(GSW_STORY_PROGRESS, STG1_10_FATHERS_LETTER)
        SPAWN_EVENT(PTR(tegami_event))
    END_IF
    IF_INT_LESS(GSW_STORY_PROGRESS, STG1_13_02_KEY_2)
        CALL(evt_item_entry, STRING("key00"), ITEM_CASTLE_KEY_0x0C, 200, 0, -200, ITEM_TYPE_STILL, -1, PTR(key_get))
    END_IF
    CALL(evt_npc_entry, STRING("\"カロン2\""), STRING("c_karon_g"))
    CALL(evt_npc_set_position, STRING("\"カロン2\""), -150, 0, 130)
    CALL(evt_npc_set_rotate, STRING("\"カロン2\""), 0, 0, 0)
    CALL(evt_npc_set_anim, STRING("\"カロン2\""), STRING("KRN_Z_2"))
    CALL(evt_npc_pera_onoff, STRING("\"カロン2\""), 0)
    CALL(evt_npc_flag_onoff, 0, STRING("\"カロン2\""), NPC_FLAG_UPDATE_BY_EVENT)
    CALL(evt_npc_flag_onoff, 1, STRING("\"カロン2\""), NPC_FLAG_NO_VIEW_CORRECTION)
    CALL(evt_npc_set_width, STRING("\"カロン2\""), 30)
    CALL(evt_npc_set_height, STRING("\"カロン2\""), 7)
    CALL(evt_npc_entry, STRING("\"カロン3\""), STRING("c_karon_g"))
    CALL(evt_npc_set_position, STRING("\"カロン3\""), 270, 0, -100)
    CALL(evt_npc_set_rotate, STRING("\"カロン3\""), 0, 180, 0)
    CALL(evt_npc_set_anim, STRING("\"カロン3\""), STRING("KRN_Z_2"))
    CALL(evt_npc_pera_onoff, STRING("\"カロン3\""), 0)
    CALL(evt_npc_flag_onoff, 0, STRING("\"カロン3\""), NPC_FLAG_UPDATE_BY_EVENT)
    CALL(evt_npc_flag_onoff, 1, STRING("\"カロン3\""), NPC_FLAG_NO_VIEW_CORRECTION)
    CALL(evt_npc_set_width, STRING("\"カロン3\""), 30)
    CALL(evt_npc_set_height, STRING("\"カロン3\""), 7)
    CALL(evt_npc_entry, STRING("\"カロン4\""), STRING("c_karon_g"))
    CALL(evt_npc_set_position, STRING("\"カロン4\""), 290, 0, 80)
    CALL(evt_npc_set_rotate, STRING("\"カロン4\""), 0, 0, 0)
    CALL(evt_npc_set_anim, STRING("\"カロン4\""), STRING("KRN_Z_2"))
    CALL(evt_npc_pera_onoff, STRING("\"カロン4\""), 0)
    CALL(evt_npc_flag_onoff, 0, STRING("\"カロン4\""), NPC_FLAG_UPDATE_BY_EVENT)
    CALL(evt_npc_flag_onoff, 1, STRING("\"カロン4\""), NPC_FLAG_NO_VIEW_CORRECTION)
    CALL(evt_npc_set_width, STRING("\"カロン4\""), 30)
    CALL(evt_npc_set_height, STRING("\"カロン4\""), 7)
    CALL(evt_npc_entry, STRING("\"カロン5\""), STRING("c_karon_g"))
    CALL(evt_npc_set_position, STRING("\"カロン5\""), -300, 0, -40)
    CALL(evt_npc_set_rotate, STRING("\"カロン5\""), 0, 180, 0)
    CALL(evt_npc_set_anim, STRING("\"カロン5\""), STRING("KRN_Z_2"))
    CALL(evt_npc_pera_onoff, STRING("\"カロン5\""), 0)
    CALL(evt_npc_flag_onoff, 0, STRING("\"カロン5\""), NPC_FLAG_UPDATE_BY_EVENT)
    CALL(evt_npc_flag_onoff, 1, STRING("\"カロン5\""), NPC_FLAG_NO_VIEW_CORRECTION)
    CALL(evt_npc_set_width, STRING("\"カロン5\""), 30)
    CALL(evt_npc_set_height, STRING("\"カロン5\""), 7)
    CALL(evt_map_playanim, STRING("hokori"), 1, 0)
    CALL(evt_npc_set_balloontype, STRING("リーダー"), 0)
    CALL(evt_npc_set_balloontype, STRING("リチャード"), 0)
    CALL(evt_mobj_kururing_floor, STRING("kururin1"), -298, 0, -197, STRING("s_heya"), 125, GSWF_STARPIECE_36)
    RETURN
EVT_END()

EVT_BEGIN(richard_init_event)
    CALL(evt_npc_set_position, STRING("リチャード"), 0, 0, -100)
    CALL(evt_npc_set_width, STRING("リチャード"), FLOAT(30.0f))
    CALL(evt_npc_set_anim, STRING("リチャード"), STRING("KRN_Z_2"))
    CALL(evt_npc_flag_onoff, 1, STRING("リチャード"), NPC_FLAG_NO_TALK_TURN | NPC_FLAG_NO_VIEW_CORRECTION)
    CALL(evt_npc_set_ry, STRING("リチャード"), 0)
    CALL(evt_npc_pera_onoff, STRING("リチャード"), 0)
    RETURN
EVT_END()

EVT_BEGIN(richard_regl_event)
    RETURN
EVT_END()

EVT_BEGIN(richard_talk_event)
    IF_INT_EQUAL(GW(15), 1)
        RETURN
    END_IF
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_msg_toge, 1, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING("stg1_gon_09_01"), 0, 0)
    CALL(evt_msg_select, 0, STRING("stg1_gon_09_01_yn"))
    CALL(evt_msg_continue)
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_mario_set_pose, STRING("M_L_2"))
        CALL(evt_msg_toge, 1, 0, 0, 0)
        CALL(evt_msg_print, 0, STRING("stg1_gon_09_02"), 0, 0)
        CALL(evt_mario_set_pose, STRING("M_S_1"))
    END_IF
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(leaderkaron_init_event)
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_TALK_POSE)
    IF_INT_LESS(GSW_STORY_PROGRESS, STG1_11_1000_DRY_BONES)
        CALL(evt_npc_set_position, STRING("リーダー"), 380, 0, 10)
        CALL(evt_npc_set_width, STRING("リーダー"), FLOAT(48.0f))
        CALL(evt_npc_set_anim, STRING("リーダー"), STRING("KRN_Z_2"))
        CALL(evt_npc_set_ry, STRING("リーダー"), 270)
        CALL(evt_npc_flag_onoff, 1, STRING("リーダー"), NPC_FLAG_NO_TALK_TURN | NPC_FLAG_NO_VIEW_CORRECTION)
        ELSE
        CALL(evt_npc_set_position, STRING("リーダー"), 0, -1000, 0)
    END_IF
    SET_VALUE(GW(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(leaderkaron_talk_event)
    BEGIN_INLINE_EVENT
        LOOP(10)
            CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
            CALL(evt_snd_sfxon_3d, STRING("SFX_STG1_HONE_ACCESS1"), LW(0), LW(1), LW(2), 0)
            WAIT_MS(200)
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_TALK_BALLOON)
    IF_INT_EQUAL(GW(0), 0)
        CALL(evt_npc_set_width, STRING(/*"リーダー"*/"\x83\x8A\x81\x5B\x83\x5F\x81\x5B"), FLOAT(20.0f))
        SPAWN_EVENT(senkaron_event)
        SET_VALUE(GW(0), 1)
    END_IF
    RETURN
EVT_END()

static ExtTexInfo senkaron_ext_entry_data[] = {
    {"c_karon_g_h", "KRN_R_1", 0.0f},
    {"c_karon_g_h", "KRN_R_1", 1.0f},
    {"c_karon_g_h", "KRN_R_1", 2.0f},
    {"c_karon_g_h", "KRN_R_1", 3.0f},
    {"c_karon_g_h", "KRN_R_1", 4.0f},
    {"c_karon_g_h", "KRN_R_1", 5.0f},
    {"c_karon_g_h", "KRN_R_1", 6.0f},
    {"c_karon_g_h", "KRN_R_1", 7.0f},
    {"c_karon_g_h", "KRN_R_1", 8.0f},
    {"c_karon_g_h", "KRN_R_1", 9.0f},
    {"c_karon_g_h", "KRN_R_1", 10.0f},
    {"c_karon_g_h", "KRN_R_1", 11.0f},
    {"c_karon_g_h", "KRN_R_1", 12.0f},
    {"c_karon_g_h", "KRN_R_1", 13.0f},
    {"c_karon_g_h", "KRN_R_1", 14.0f},
    {"c_karon_g_h", "KRN_R_1", 15.0f},
    {"c_karon_g_h", "KRN_R_1", 16.0f},
    {"c_karon_g_h", "KRN_R_1", 17.0f},
    {"c_karon_g_h", "KRN_R_1", 18.0f},
    {"c_karon_g_h", "KRN_R_1", 19.0f},
    {"c_karon_g_h", "KRN_R_1", 20.0f},
    {"c_karon_g_h", "KRN_R_1", 21.0f},
    {"c_karon_g_h", "KRN_R_1", 22.0f},
    {"c_karon_g_h", "KRN_R_1", 23.0f},
    {"c_karon_g_h", "KRN_D_1", 0.0f},
    {0},
};

EVT_BEGIN(senkarondummy_init_event)
    CALL(evt_npc_set_position, STRING("カロン1"), 0, -1000, 0)
    RETURN
EVT_END()

EVT_BEGIN(senkarondummy_regl_event)
    RETURN
EVT_END()

EVT_BEGIN(senkarondummy_talk_event)
    RETURN
EVT_END()

EVT_BEGIN(senkaron_entry_event)
    CALL(evt_ext_entry, 1000, PTR(senkaron_ext_entry_data), PTR(senkaron_ext_init), PTR(senkaron_ext_main), PTR(senkaron_ext_dispent))
    RETURN
EVT_END()

EVT_BEGIN(senkaron_escape_event)
    CALL(senkaronToEscape)
    RETURN
EVT_END()

void nokotaro_callback(s32 type) {
    PartyEntry* party = partyGetPtr(partyCtrlNo);

    switch (type) {
        case 0:
            partyChgPose(party, (const char*)nokotaro_pose[1]);
            break;
        case 2:
            partyChgPose(party, (const char*)nokotaro_pose[0]);
            break;
        case 3:
            partyChgPose(party, (const char*)nokotaro_pose[1]);
            break;
        case 1:
            partyChgPose(party, (const char*)nokotaro_pose[0]);
            break;
    }
}

USER_FUNC(leader_touch_check) {
    NpcEntry* npc = marioGetTouchNpcPtr();
    BOOL off = FALSE;

    if (marioKeyOffChk() || marioCtrlOffChk()) {
        off = TRUE;
    }
    if (marioStGetSystemLevel() != 0) {
        off = TRUE;
    }
    if (npc != NULL && strcmp(npc->description, "\203\212\201[\203_\201[") == 0 && !off) {
        evtSetValue(event, LW(0), 1);
    } else {
        evtSetValue(event, LW(0), 0);
    }
    return EVT_RETURN_DONE;
}

static f32 randf(f32 min, f32 max) {
    f32 r = rand() / 32767.0f;

    return r * (max - min) + min;
}

static void senkaron_ext_init(void) {
    s32 i;
    ExtdrvPose* pose = extGetPosePtr();

    senkaron = _mapAlloc(sizeof(Senkaron) * 1000);
    for (i = extGetPoseNum() - 1; i >= 0; i--, pose++) {
        pose->flags = 3;
        pose->dir = randf(0.0f, 360.0f);
        PSMTXIdentity(*pose->mtx);
        (*pose->mtx)[0][3] = randf(-385.0f, 375.0f);
        (*pose->mtx)[1][3] = 200.0f;
        (*pose->mtx)[2][3] = randf(-125.0f, 140.0f);
        pose->dir = 90.0f;
        senkaron[i].state = 0;
        senkaron[i].timer = randf(1.0f, 50.0f);
        senkaron[i].vy = -5.0f;
    }
    senkaronAttackStartFlag = 0;
    senkaronEscapeFlag = 0;
    marioHammerFrame = 0;
}

static void senkaron_ext_main(void) {
    MarioWork* mario;
    const char* sfxLand;
    const char* sfxFly;
    const char* sfxFly2;
    s32 n;
    s32 i;
    ExtdrvPose* pose;
    s32 r;
    f32 range;
    f32 near;
    f32 scale;
    f32 dist;

    pose = extGetPosePtr();
    mario = marioGetPtr();
    scale = mario->wPlayerCollisionBox.x / 2;
    marioPushedFlag = 0;
    if (mario->currMotionId == MARIO_MOTION_HAMMER) {
        if (++marioHammerFrame == 15) {
            marioHammerFrame = 0;
        }
    } else {
        marioHammerFrame = 0;
    }
    n = extGetPoseNum();
    // FAKE: comma expression `(0, X)`: X becomes a separate compiler temp; the plain X changes the object
    sfxLand = (0, "SFX_STG1_HONE_LANDING1");
    range = 1.5f * (6.0f * scale);
    // FAKE: comma expression `(0, X)`: X becomes a separate compiler temp; the plain X changes the object
    sfxFly = (0, "SFX_STG1_HONE_FLY1");
    near = 3.0f * scale;
    // FAKE: comma expression `(0, X)`: X becomes a separate compiler temp; the plain X changes the object
    sfxFly2 = (0, "SFX_STG1_HONE_FLY1");
    for (i = n - 1; i >= 0; i--, pose++) {
        switch (senkaron[i].state) {
            case 0:
                pose->texId = (gp->retraceCount + i * i) % 24;
                if (senkaron[i].timer > 0) {
                    if (senkaronEscapeFlag == 1) {
                        senkaron[i].state = 0x62;
                    } else if (senkaronAttackStartFlag == 1) {
                        senkaron[i].timer--;
                    }
                    break;
                }
                (*pose->mtx)[1][3] += senkaron[i].vy;
                senkaron[i].vy -= 0.3f;
                if ((*pose->mtx)[1][3] < 0.0f) {
                    (*pose->mtx)[1][3] = 0.0f;
                    senkaron[i].state = 1;
                    senkaron[i].timer = 0;
                    senkaron[i].flags &= 0xFFFE;
                    if (i % 8 == 0) {
                        Vec pos = {0.0f, 0.0f, 0.0f};
                        pos.x = (*pose->mtx)[0][3];
                        pos.y = (*pose->mtx)[1][3];
                        pos.z = (*pose->mtx)[2][3];
                        psndSFXOn_3D(sfxLand, &pos);
                    }
                }
                break;
            case 1:
                pose->texId = (gp->retraceCount + i * i) % 24;
                if (senkaronEscapeFlag == 1) {
                    if ((*pose->mtx)[0][3] == 0.0f && (*pose->mtx)[2][3] == 0.0f) {
                        senkaron[i].vx = 1.0f;
                        senkaron[i].vz = 1.0f;
                    } else {
                        dist = sqrt((*pose->mtx)[0][3] * (*pose->mtx)[0][3] + (*pose->mtx)[2][3] * (*pose->mtx)[2][3]);
                        senkaron[i].vx = 4.0f * (*pose->mtx)[0][3] / dist;
                        senkaron[i].vz = 4.0f * (*pose->mtx)[2][3] / dist;
                    }
                    if (senkaron[i].vx < 0.0f) {
                        pose->dir = 90.0f;
                    } else {
                        pose->dir = 270.0f;
                    }
                    senkaron[i].state = 0x62;
                    senkaron[i].timer = 300;
                    break;
                }
                if (senkaronMesPushingFlag == 1) {
                    break;
                }
                if (mario->currMotionId == MARIO_MOTION_HAMMER && marioHammerFrame == 10 &&
                    (*pose->mtx)[2][3] > mario->position.z - 80.0f && (*pose->mtx)[2][3] < 80.0f + mario->position.z) {
                    if (mario->directionView > 0.0f && mario->directionView < 180.0f) {
                        if ((*pose->mtx)[0][3] > mario->position.x && (*pose->mtx)[0][3] < mario->position.x + range) {
                            f32 dx;
                            f32 dz;

                            dz = (*pose->mtx)[2][3] - mario->position.z;
                            dx = (*pose->mtx)[0][3] - mario->position.x;
                            dist = sqrt(dx * dx + dz * dz);
                            senkaron[i].vx = 3.0f * dx / dist - 1.0f;
                            senkaron[i].vy = randf(5.0f, 10.0f);
                            senkaron[i].vz = randf(-5.0f, 10.0f);
                            senkaron[i].state = 0x61;
                            {
                                Vec pos = {0.0f, 0.0f, 0.0f};
                                pos.x = (*pose->mtx)[0][3];
                                pos.y = (*pose->mtx)[1][3];
                                pos.z = (*pose->mtx)[2][3];
                                psndSFXOn_3D(sfxFly, &pos);
                            }
                        }
                    } else {
                        if ((*pose->mtx)[0][3] < mario->position.x && (*pose->mtx)[0][3] > mario->position.x - range) {
                            f32 dx;
                            f32 dz;

                            dz = (*pose->mtx)[2][3] - mario->position.z;
                            dx = (*pose->mtx)[0][3] - mario->position.x;
                            dist = sqrt(dx * dx + dz * dz);
                            senkaron[i].vx = 1.0f + 3.0f * dx / dist;
                            senkaron[i].vy = randf(5.0f, 10.0f);
                            senkaron[i].vz = randf(-5.0f, 10.0f);
                            senkaron[i].state = 0x61;
                            {
                                Vec pos = {0.0f, 0.0f, 0.0f};
                                pos.x = (*pose->mtx)[0][3];
                                pos.y = (*pose->mtx)[1][3];
                                pos.z = (*pose->mtx)[2][3];
                                psndSFXOn_3D(sfxFly2, &pos);
                            }
                        }
                    }
                }
                if ((*pose->mtx)[0][3] > mario->position.x + scale - 10.0f && (*pose->mtx)[0][3] < mario->position.x + near &&
                    (*pose->mtx)[2][3] > mario->position.z - 20.0f && (*pose->mtx)[2][3] < 20.0f + mario->position.z) {
                    marioPushedFlag = 1;
                }
                if ((*pose->mtx)[0][3] >= mario->position.x) {
                    if (senkaron[i].flags & 1) {
                        pose->dir = 270.0f;
                    } else {
                        pose->dir = 90.0f;
                    }
                } else {
                    if (senkaron[i].flags & 1) {
                        pose->dir = 90.0f;
                    } else {
                        pose->dir = 270.0f;
                    }
                }
                if (senkaron[i].timer == 0) {
                    f32 dx;
                    f32 dz;
                    f32 dist;

                    dx = (*pose->mtx)[0][3] - mario->position.x;
                    dz = (*pose->mtx)[2][3] - mario->position.z;
                    dist = 10.0f * (f32)sqrt(dx * dx + dz * dz);
                    r = randf(0.0f, 50.0f);
                    senkaron[i].vx = randf(-0.5f, 0.5f);
                    senkaron[i].vz = randf(-0.5f, 0.5f);
                    senkaron[i].vx -= dx / dist;
                    senkaron[i].vz -= dz / dist;
                    if (r == 1) {
                        senkaron[i].flags |= 1;
                    } else {
                        senkaron[i].flags &= 0xFFFE;
                    }
                    senkaron[i].timer = 60;
                }
                senkaron[i].timer--;
                (*pose->mtx)[0][3] += senkaron[i].vx;
                (*pose->mtx)[2][3] += senkaron[i].vz;
                if ((*pose->mtx)[0][3] > 375.0f) {
                    (*pose->mtx)[0][3] = 375.0f;
                }
                if ((*pose->mtx)[0][3] < -385.0f) {
                    (*pose->mtx)[0][3] = -385.0f;
                }
                if ((*pose->mtx)[2][3] > 140.0f) {
                    (*pose->mtx)[2][3] = 140.0f;
                }
                if ((*pose->mtx)[2][3] < -125.0f) {
                    (*pose->mtx)[2][3] = -125.0f;
                }
                break;
            case 0x61:
                pose->flags |= EXT_POSE_FLAG_NO_SHADOW;
                if ((*pose->mtx)[1][3] > -100.0f) {
                    (*pose->mtx)[0][3] += senkaron[i].vx;
                    (*pose->mtx)[1][3] += senkaron[i].vy;
                    (*pose->mtx)[2][3] += senkaron[i].vz;
                    if ((*pose->mtx)[2][3] <= -125.0f) {
                        (*pose->mtx)[2][3] = -125.0f;
                    }
                    senkaron[i].vy -= 0.3f;
                    pose->texId = 24;
                } else {
                    marioGetPtr();
                    (*pose->mtx)[0][3] = randf(-385.0f, 375.0f);
                    (*pose->mtx)[1][3] = 200.0f;
                    (*pose->mtx)[2][3] = randf(-125.0f, 140.0f);
                    pose->dir = randf(0.0f, 360.0f);
                    pose->flags &= ~EXT_POSE_FLAG_NO_SHADOW;
                    senkaron[i].state = 0;
                    senkaron[i].timer = randf(1.0f, 1000.0f);
                    senkaron[i].vy = 0.0f;
                }
                break;
            case 0x62:
                if (senkaron[i].timer > 0) {
                    (*pose->mtx)[0][3] += senkaron[i].vx;
                    (*pose->mtx)[2][3] += senkaron[i].vz;
                    senkaron[i].timer--;
                } else {
                    senkaron[i].state = 0x63;
                }
                pose->texId = (gp->retraceCount + i * i) % 24;
                break;
            case 0x63:
                break;
        }
    }
    if (marioPushedFlag == 1) {
        f32 dx;
        f32 dz;
        f32 dist;
        f32 speed;

        dx = -390.0f - mario->position.x;
        dz = -mario->position.z;
        dist = sqrt(dx * dx + dz * dz);
        switch (mario->currMotionId) {
            case 0x12:
                speed = 0.0f;
                break;
            case 1:
                speed = randf(2.0f, 2.5f);
                break;
            case 2:
                speed = randf(2.8f, 3.3f);
                break;
            case 3:
                speed = randf(2.3f, 2.8f);
                break;
            default:
                speed = randf(0.7f, 1.2f);
                break;
        }
        mario->position.x += speed * (dx / dist);
        mario->position.z += speed * (dz / dist);
    }
}

static void senkaron_ext_dispent(void) {
    dispEntry(CAMERA_3D, 0.0f, 1, senkaron_ext_disp, NULL);
    dispEntry(CAMERA_3D, 0.0f, 2, senkaron_ext_shadow_disp, NULL);
}

static void senkaron_ext_disp(CameraId camera, void* param) {
    extLoadRenderMode();
    extLoadVertex();
    extLoadTexture();
    extLoadTev();
    extDraw();
    extLoadTextureExit();
}

static void senkaron_ext_shadow_disp(CameraId camera, void* param) {
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

static USER_FUNC(senkaronToEscape) {
    senkaronEscapeFlag = 1;
    return EVT_RETURN_DONE;
}

static USER_FUNC(senkaronFlagOnMesPushing) {
    senkaronMesPushingFlag = 1;
    return EVT_RETURN_DONE;
}

static USER_FUNC(senkaronFlagOffMesPushing) {
    senkaronMesPushingFlag = 0;
    return EVT_RETURN_DONE;
}

static USER_FUNC(senkaronAttackStart) {
    senkaronAttackStartFlag = 1;
    return EVT_RETURN_DONE;
}

static USER_FUNC(senkaronGetMarioPushedFlag) {
    evtSetValue(event, event->args[0], marioPushedFlag);
    return EVT_RETURN_DONE;
}
