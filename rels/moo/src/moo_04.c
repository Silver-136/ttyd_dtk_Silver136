#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "libc_decls.h"
#include "evt/evt_bero.h"
#include "evt/evt_bg.h"
#include "evt/evt_cam.h"
#include "evt/evt_eff.h"
#include "evt/evt_fade.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_msg.h"
#include "evt/evt_party.h"
#include "evt/evt_seq.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_sub.h"
#include "driver/npcdrv.h"
#include "driver/mapdrv.h"
#include "system.h"
#include "pmario_sound.h"

#include "mario/mario.h"
#include "party.h"
#include "manager/evtmgr_cmd.h"
#include "evt/evt_npc.h"
#include "driver/seqdrv.h"


USER_FUNC(evt_mario_dispflag_onoff);
USER_FUNC(evt_snd_envoff_f);
static USER_FUNC(mario_kirimomi);

static f32 spline_dt[9];
static EvtWord party_damage_pose[52];
static EvtWord mario_fly[168];
EvtWord kanbu_fly[198];
static NpcSetupInfo npcEnt[2];
static EvtWord epilogue_evt[64];
EvtWord moo_04_init_evt[220];

static f32 spline_dt[] = {
    -12.916925f, 123.84991f, 512.97614f, 17.94919f, 46.050007f, 5.011878f, 65.15421f, 48.400024f,
    -75.32023f,
};

static EVT_BEGIN(party_damage_pose)
    CALL(evt_mario_get_party, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CASE_EQUAL(1)
        CALL(evt_party_set_pose, 0, STRING("PKR_D_1"))
        CASE_EQUAL(2)
        CALL(evt_party_set_pose, 0, STRING("PNK_D_1"))
        CASE_EQUAL(3)
        CALL(evt_party_set_pose, 0, STRING("D_1"))
        CASE_EQUAL(4)
        CALL(evt_party_set_pose, 0, STRING("PYS_D_1"))
        CASE_EQUAL(5)
        CALL(evt_party_set_pose, 0, STRING("PWD_D_1"))
        CASE_EQUAL(6)
        CALL(evt_party_set_pose, 0, STRING("PTR_D_1"))
        CASE_EQUAL(7)
        CALL(evt_party_set_pose, 0, STRING("PCH_D_1"))
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(mario_fly)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_stop, 0)
    WAIT_FRAMES(1)
    CALL(evt_snd_sfxon, STRING("SFX_STG7_MARIO_FLY1"), LW(10))
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_POSE_LOCK)
    CALL(evt_mario_set_pose, STRING("M_N_7"))
    CALL_CHILD(PTR(party_damage_pose))
    WAIT_FRAMES(1)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_party_set_dir, 0, 90, 0)
    CALL(evt_mario_set_pos, -300, -300, 0)
    CALL(evt_party_set_pos, 0, -300, -300, 0)
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, EASING_QUADRATIC_OUT, 1000, 10, 5000)
        LABEL(0)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(1000.0f))
        CALL(mario_kirimomi, LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(0)
        END_IF
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_sub_spline_init, 1, 6, PTR(spline_dt), 3, 0, 5000)
        LABEL(0)
        CALL(evt_sub_spline_get_value, LW(0), LW(1), LW(2), LW(3))
        CALL(evt_mario_set_pos, LW(0), LW(1), LW(2))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(3), 1)
            GOTO(0)
        END_IF
        CALL(evt_mario_set_pos, 0, -1000, 0)
    END_INLINE_EVENT
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        CALL(evt_sub_spline_init, 1, 6, PTR(spline_dt), 3, 0, 5000)
        LABEL(1)
        CALL(evt_sub_spline_get_value, LW(0), LW(1), LW(2), LW(3))
        CALL(evt_party_set_pos, 0, LW(0), LW(1), LW(2))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(3), 1)
            GOTO(1)
        END_IF
        CALL(evt_party_set_pos, 0, 0, -1000, 0)
    END_INLINE_EVENT
    WAIT_MS(7000)
    BEGIN_INLINE_EVENT
        WAIT_MS(500)
        CALL(evt_snd_sfxoff, LW(10))
    END_INLINE_EVENT
    CALL(evt_bero_mapchange, STRING("moo_00"), 0)
    RETURN
EVT_END()

EVT_BEGIN(kanbu_fly)
    WAIT_MS(2000)
    CALL(evt_npc_set_position, STRING("幹部"), 25, 75, -70)
    CALL(evt_npc_set_scale, STRING("幹部"), FLOAT(0.2998047f), FLOAT(0.2998047f), FLOAT(0.2998047f))
    BEGIN_INLINE_EVENT
        LOOP(0)
            SET_FLOAT_VALUE(LW(0), FLOAT(25.0f))
            CALL(evt_npc_get_scale, STRING("幹部"), LW(1), LW(2), LW(3))
            MULTIPLY_FLOAT(LW(0), LW(2))
            CALL(evt_npc_set_rotate_offset, STRING("幹部"), 0, LW(0), 0)
            CALL(evt_npc_add_rotate, STRING("幹部"), 0, 0, 30)
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        SET_FLOAT_VALUE(LW(0), FLOAT(-1.25f))
        SET_FLOAT_VALUE(LW(1), FLOAT(0.25f))
        SET_FLOAT_VALUE(LW(2), FLOAT(0.0f))
        CALL(evt_npc_get_position, STRING("幹部"), LW(3), LW(4), LW(5))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG7_3RD_FLY1"), LW(3), LW(4), LW(5), LW(9))
        CALL(evt_sub_intpl_msec_init, EASING_QUADRATIC_OUT, 0, 100, 2000)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
            SET_FLOAT_VALUE(LW(2), FLOAT(-125.0f))
            SET_FLOAT_VALUE(LW(3), FLOAT(25.0f))
            SET_FLOAT_VALUE(LW(4), FLOAT(0.0f))
            MULTIPLY_FLOAT(LW(2), LW(0))
            MULTIPLY_FLOAT(LW(3), LW(0))
            MULTIPLY_FLOAT(LW(4), LW(0))
            ADD_FLOAT(LW(2), FLOAT(25.0f))
            ADD_FLOAT(LW(3), FLOAT(75.0f))
            ADD_FLOAT(LW(4), FLOAT(-70.0f))
            CALL(evt_npc_set_position, STRING("幹部"), LW(2), LW(3), LW(4))
            CALL(evt_snd_sfx_pos, LW(9), LW(2), LW(3), LW(4))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
        CALL(evt_npc_set_position, STRING("幹部"), 0, -1000, 0)
        CALL(evt_snd_sfxoff, LW(9))
    END_INLINE_EVENT
    CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 30, 1, 2000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
        CALL(evt_npc_set_scale, STRING("幹部"), LW(0), LW(0), LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    WAIT_MS(1000)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG7_3RD_LIGHT1"), -100, 100, -70, 0)
    CALL(evt_eff64, 0, STRING("wangflush_n64"), 0, -100, 100, -70, 60, 0, 0, 0, 0, 0, 0, 0)
    WAIT_MS(3000)
    CALL(evt_bero_mapchange, STRING("aji_14"), STRING("tuzuki"))
    RETURN
EVT_END()

static NpcSetupInfo npcEnt[] = {
    {
        /*"幹部"*/"\x8A\xB2\x95\x94", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // reactionFlags..returnEvent
        NULL, 0, // blowEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

static EVT_BEGIN(epilogue_evt)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG7_DUN1"))
    CALL(evt_snd_bgm_scope, 0, 1)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_HIDE)
    CALL(evt_cam3d_evt_set, 75, 165, 533, 75, 70, -8, 0, 11)
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_fade_end_wait)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("ep_45"), 0, 0)
    WAIT_MS(2000)
    CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 3000)
    CALL(evt_snd_envoff_f, PSND_FADEOUT, 3000)
    CALL(evt_fade_set_mapchange_type, 1, 10, 900, 9, 900)
    CALL(evt_bero_mapchange, STRING("gor_00"), STRING("epilogue2"))
    RETURN
EVT_END()

EVT_BEGIN(moo_04_init_evt)
    IF_INT_NOT_EQUAL(GSW_STORY_PROGRESS, 402)
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG7_MON1"))
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG7_MOO4"))
    END_IF
    CALL(evt_npc_setup, PTR(npcEnt))
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(358)
        CALL(evt_cam3d_evt_set, 0, 134, 533, 0, 38, -8, 0, 11)
        SPAWN_EVENT(PTR(mario_fly))
        WAIT_FRAMES(1)
        CASE_LESS(373)
        CALL(evt_cam3d_evt_set, 20, 200, 533, 20, 104, -8, 0, 11)
        CALL(evt_mario_cont_onoff, 0)
        CALL(evt_party_cont_onoff, 0, 0)
        CALL(evt_mario_set_pos, 0, -1000, 0)
        CALL(evt_party_set_pos, 0, 0, -1000, 0)
        WAIT_FRAMES(1)
        SPAWN_EVENT(PTR(kanbu_fly))
        CASE_EQUAL(402)
        CALL(evt_mario_kill_party, 0)
        SPAWN_EVENT(PTR(epilogue_evt))
        CASE_DEFAULT
        CALL(evt_cam3d_evt_set, 75, 160, 533, 75, 65, -8, 0, 11)
        BEGIN_INLINE_EVENT
            CALL(evt_mario_cont_onoff, 0)
            CALL(evt_party_cont_onoff, 0, 0)
            CALL(evt_mario_set_pos, 0, -1000, 0)
            CALL(evt_party_set_pos, 0, 0, -1000, 0)
            WAIT_FRAMES(1)
            CALL(evt_seq_wait, SEQ_GAME)
            WAIT_MS(3000)
            CALL(evt_msg_print, 0, STRING("stg7_aji_63"), 0, 0)
            WAIT_MS(2000)
            CALL(evt_eff64, 0, STRING("crystal_n64"), 0, 30, 90, -75, FLOAT(1.0f), 50, 0, 0, 0, 0, 0, 0)
            CALL(evt_eff64, 0, STRING("crystal_n64"), 6, 30, 90, -75, FLOAT(1.0f), 50, 0, 0, 0, 0, 0, 0)
            WAIT_MS(1000)
            CALL(evt_eff64, STRING("hikari"), STRING("radiation_n64"), 2, 30, 90, -75, FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
            SET_VALUE(GSW_STORY_PROGRESS, 377)
            CALL(evt_fade_entry, 17, 2000, 255, 255, 255)
            CALL(evt_fade_end_wait)
            CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S"), MAP_OBJ_FLAG_HIDDEN)
            CALL(evt_bg_disp_onoff, 0)
            CALL(evt_eff_delete, STRING("hikari"))
            CALL(evt_fade_entry, 16, 2000, 255, 255, 255)
            CALL(evt_fade_end_wait)
            CALL(evt_fade_set_mapchange_type, 1, 10, 1000, 9, 1000)
            SET_VALUE(GSWF(4175), 1)
            CALL(evt_bero_mapchange, STRING("tik_00"), 0)
        END_INLINE_EVENT
    END_SWITCH
    RETURN
EVT_END()

static USER_FUNC(mario_kirimomi) {
    f32 scale = evtGetFloat(event, event->args[0]);
    MarioWork* mario = marioGetPtr();
    PartyEntry* party = partyGetPtr(0);

    mario->scale.x = mario->scale.y = scale;
    mario->rotOfs.y = mario->wPlayerCollisionBox.y / 2;
    mario->rotOfs.y *= mario->scale.y;
    mario->rot.z -= 10.0f;
    mario->rot.z = fmod(mario->rot.z, 360.0);
    if (party != NULL) {
        party->scale.x = party->scale.y = scale;
        if (party->currentMemberId != PARTY_MEMBER_YOSHI) {
            party->rotOfsY = party->height / 2;
        } else {
            party->rotOfsY = (party->height - 10.0f) / 2;
        }
        party->rotOfsY *= party->scale.y;
        party->rotation -= 10.0f;
        party->rotation = fmod(party->rotation, 360.0);
    }
    return EVT_RETURN_DONE;
}

