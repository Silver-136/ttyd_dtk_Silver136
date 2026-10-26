#include "battle/battle_unit.h"
#include "eff/eff_starstone.h"
#include "evt/evt_cmd.h"
#include "evt/evt_cam.h"
#include "evt/evt_eff.h"
#include "evt/evt_fade.h"
#include "evt/evt_hit.h"
#include "evt/evt_item.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_msg.h"
#include "evt/evt_paper.h"
#include "evt/evt_party.h"
#include "mri_puni.h"
#include "evt/evt_case.h"
#include "evt/evt_seq.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "driver/mapdrv.h"
#include "mario/mario_pouch.h"
#include "system.h"
#include "driver/casedrv.h"
#include "pmario_sound.h"

#include "driver/fadedrv.h"
#include "mario/mariost.h"
#include "driver/itemdrv.h"
#include "driver/effdrv.h"
#include "driver/npcdrv.h"
#include "driver/animdrv.h"
#include "evt/evt_npc.h"
#include "manager/evtmgr_cmd.h"
#include "evt/evt_bero.h"
#include "evt/evt_sub.h"
#include "mri.h"
#include "data_tables.h"
#include "driver/seqdrv.h"

USER_FUNC(evt_npc_set_anim_localtime);
static USER_FUNC(evtflash);
USER_FUNC(starstone_rotate_func);

static EvtWord first_time_w_bero[60];
static BeroEntry bero_entry_data[3];
EvtWord npc_rotate[28];
EvtWord kanbu_urouro[75];
EvtWord dai3_kanbu[192];
EvtWord flash_and_wait[16];
EvtWord flash2_and_wait[26];
EvtWord starstone_rotate[6];
EvtWord starstone_rotate_exec[13];
static EvtWord starstone_fuwafuwa[71];
EvtWord npc_dokan[209];
static EvtWord punipuni_mario_halt[33];
#ifdef VERSION_G8ME01
EvtWord time_bomb[931];
#else
EvtWord time_bomb[937];
#endif
EvtWord bomb_check[14];
static EvtWord yuka_move[96];
static EvtWord yuka_check[116];
static KusaAccess ueki_access_data[7];
static EvtWord punipuni_ball00_init[35];
static EvtWord punipuni_ball00[57];
EvtWord mri_14_init_evt[179];

static USER_FUNC(evtflash) {
    if (isFirstCall) {
        event->userdata[2] = 0;
    }
    switch (event->userdata[2]) {
        case 0:
            fadeEntry(FADE_OUT_BLACK, 20, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
            event->userdata[2]++;
        case 1:
            if (fadeIsFinish() != TRUE) {
                return EVT_RETURN_BLOCK;
            }
            event->userdata[0] = 50;
            event->savedTime = (s32)gp->renderTime;
            event->userdata[2]++;
            if (event->userdata[0] == 0) {
                event->userdata[2]++;
            }
            return EVT_RETURN_BLOCK;
        case 2:
            if (OSTicksToMilliseconds((u32)gp->renderTime - (u32)event->savedTime) < (u32)event->userdata[0]) {
                return EVT_RETURN_BLOCK;
            }
            event->userdata[2]++;
            return EVT_RETURN_BLOCK;
        case 3:
            fadeEntry(FADE_IN_BLACK, 80, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
            event->userdata[2]++;
        case 4:
            if (fadeIsFinish() != TRUE) {
                return EVT_RETURN_BLOCK;
            }
            return EVT_RETURN_DONE;
    }
    return EVT_RETURN_DONE;
}

static EVT_BEGIN(first_time_w_bero)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 108)
        IF_INT_EQUAL(GSWF(2864), 1)
            SET_VALUE(GSWF(2864), 0)
            CALL(evt_mario_set_pos, -450, 0, 0)
            CALL(evt_party_set_pos, 0, -480, 0, 0)
            WAIT_MS(1000)
            BEGIN_INLINE_EVENT
                WAIT_MS(500)
                CALL(evt_bero_exec_onoff, 0, 2)
                WAIT_MS(500)
                CALL(evt_bero_exec_onoff, 1, 2)
            END_INLINE_EVENT
            BEGIN_INLINE_EVENT
                CALL(evt_party_move_pos2, 0, -360, 0, FLOAT(150.0f))
            END_INLINE_EVENT
            CALL(evt_mario_mov_pos2, -330, 0, FLOAT(150.0f))
            SET_VALUE(LW(0), 1)
            RETURN
        END_IF
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
        first_time_w_bero, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "mri_13", // nextMapName
        "e_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "dokan", // hitName
        2, // type
        0, // sfxId
        2, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        10, // caseType
        NULL, // exitEvent
        "mri_15", // nextMapName
        "dokan", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

EVT_BEGIN(npc_rotate)
    CALL(evt_sub_intpl_msec_init, EASING_LINEAR, LW(1), LW(2), LW(3))
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
        CALL(evt_npc_set_rotate, LW(10), 0, LW(0), 0)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(kanbu_urouro)
    CALL(evt_npc_set_ry, LW(10), 270)
    WAIT_MS(200)
    CALL(evt_npc_move_position, LW(10), 30, -215, 0, FLOAT(90.0f), 0)
    LOOP(0)
        WAIT_MS(350)
        CALL(evt_npc_set_ry, LW(10), 90)
        WAIT_MS(200)
        CALL(evt_npc_move_position, LW(10), 120, -215, 0, FLOAT(90.0f), 0)
        WAIT_MS(300)
        CALL(evt_npc_set_ry, LW(10), 270)
        WAIT_MS(200)
        CALL(evt_npc_move_position, LW(10), 30, -215, 0, FLOAT(90.0f), 0)
        IF_INT_EQUAL(GF(0), 1)
            WAIT_MS(350)
            CALL(evt_npc_set_ry, LW(10), 90)
            WAIT_MS(200)
            CALL(evt_npc_move_position, LW(10), 75, -215, 0, FLOAT(90.0f), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    SET_VALUE(GF(1), 0)
    RETURN
EVT_END()

EVT_BEGIN(dai3_kanbu)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_mario_set_pos, 0, -1000, 0)
    CALL(evt_party_set_pos, 0, 0, -1000, 0)
    SET_VALUE(LW(10), STRING("第三勢力幹部"))
    CALL(evt_npc_set_ry, LW(10), 270)
    CALL(evt_npc_flag_onoff, 1, LW(10), NPC_FLAG_NO_HIT | NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_set_position, LW(10), 150, 0, -215)
    SET_VALUE(GF(0), 0)
    SET_VALUE(GF(1), 1)
    SPAWN_EVENT(PTR(kanbu_urouro))
    CALL(evt_snd_bgmoff, PSND_PAUSE)
    CALL(evt_cam3d_evt_set, 0, 300, 1000, 0, 30, -1, 0, 11)
    CALL(evt_seq_wait, SEQ_GAME)
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_set, 75, 123, 122, 75, 55, -170, 1000, 11)
    WAIT_MS(2000)
    SET_VALUE(GF(0), 1)
    WAIT_WHILE(GF(1))
    WAIT_MS(500)
    SET_VALUE(LW(1), 0)
    SET_VALUE(LW(2), 180)
    SET_VALUE(LW(3), 400)
    SPAWN_EVENT(PTR(npc_rotate))
    WAIT_MS(500)
    SET_VALUE(LW(1), 180)
    SET_VALUE(LW(2), 0)
    SET_VALUE(LW(3), 400)
    SPAWN_EVENT(PTR(npc_rotate))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e22_00"), 0, STRING("第三勢力幹部"))
    SET_VALUE(LW(1), 0)
    SET_VALUE(LW(2), 180)
    SET_VALUE(LW(3), 200)
    SPAWN_EVENT(PTR(npc_rotate))
    WAIT_MS(300)
    CALL(evt_eff_fukidashi, 1, STRING(""), LW(10), 0, 0, 0, 0, 0, 0, 0, 60)
    WAIT_MS(1200)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e22_01"), 0, LW(10))
    BEGIN_INLINE_EVENT
        CALL(evt_npc_set_ry, LW(10), 270)
        CALL(evt_npc_move_position, LW(10), 200, -215, 0, FLOAT(120.0f), 0)
    END_INLINE_EVENT
    WAIT_MS(1000)
    SET_VALUE(GSWF(2864), 1)
    SET_VALUE(GSW_STORY_PROGRESS, 108)
    CALL(evt_snd_bgmoff, PSND_FADEOUT | PSND_SLOT(1))
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_FADEIN, 0)
    CALL(evt_fade_set_mapchange_type, 0, 2, 500, 9, 500)
    CALL(evt_bero_mapchange, STRING("mri_14"), STRING("w_bero"))
    RETURN
EVT_END()

EVT_BEGIN(flash_and_wait)
    WAIT_WHILE(GF(0))
    SET_VALUE(GF(0), 1)
    CALL(evt_snd_sfxon, STRING("SFX_STG2_LIGHT1"), 0)
    CALL(evtflash)
    SET_VALUE(GF(0), 0)
    RETURN
EVT_END()

USER_FUNC(starstone_rotate_func) {
    EvtWord* args = event->args;
    char* name = (char*)evtGetValue(event, args[0]);
    f32 angle = evtGetValue(event, args[1]);

    ((EffStarStoneWork*)itemNameToPtr(name)->effect->userdata)->rot.y = angle;
    return EVT_RETURN_BLOCK;
}

USER_FUNC(evt_npc_set_anim_localtime) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    f32 time = evtGetFloat(event, args[1]);

    animPoseSetLocalTime(evtNpcNameToPtr(event, name)->poseId, time);
    return EVT_RETURN_DONE;
}

EVT_BEGIN(flash2_and_wait)
    WAIT_WHILE(GF(0))
    SET_VALUE(GF(0), 1)
    BEGIN_INLINE_EVENT
        CALL(evt_snd_sfxon, STRING("SFX_STG2_LIGHT1"), 0)
        CALL(evtflash)
    END_INLINE_EVENT
    WAIT_MS(200)
    CALL(evt_snd_sfxon, STRING("SFX_STG2_LIGHT1"), 0)
    CALL(evtflash)
    SET_VALUE(GF(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(starstone_rotate)
    CALL(starstone_rotate_func, LW(0), GW(10))
    RETURN
EVT_END()

EVT_BEGIN(starstone_rotate_exec)
    LOOP(0)
        ADD(GW(10), 6)
        MODULO(GW(10), 360)
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

static EVT_BEGIN(starstone_fuwafuwa)
    LOOP(0)
        CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 0, 500, 500)
        CALL(evt_item_get_position, STRING("item00"), LW(5), LW(6), LW(7))
        LABEL(1)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
        ADD_FLOAT(LW(0), LW(6))
        CALL(evt_item_set_position, STRING("item00"), LW(5), LW(0), LW(7))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(1)
        END_IF
        CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 500, 0, 500)
        LABEL(2)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
        ADD_FLOAT(LW(0), LW(6))
        CALL(evt_item_set_position, STRING("item00"), LW(5), LW(0), LW(7))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(2)
        END_IF
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(npc_dokan)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_set_paper, LW(10), STRING("p_dokan_x"))
        CALL(evt_npc_set_paper_anim, LW(10), STRING("PM_D_1A"))
        WAIT_FRAMES(20)
        LOOP(0)
            CALL(evt_npc_set_paper_anim, LW(10), STRING("PM_D_1B"))
            CALL(evt_npc_set_anim_localtime, LW(10), FLOAT(0.0f))
            WAIT_FRAMES(1)
            CALL(evt_npc_set_anim_localtime, LW(10), FLOAT(1.0f))
            WAIT_FRAMES(1)
            CALL(evt_npc_set_anim_localtime, LW(10), FLOAT(2.0f))
            WAIT_FRAMES(1)
            CALL(evt_npc_set_anim_localtime, LW(10), FLOAT(3.0f))
            WAIT_FRAMES(1)
            CALL(evt_npc_set_anim_localtime, LW(10), FLOAT(4.0f))
            WAIT_FRAMES(1)
            CALL(evt_npc_set_anim_localtime, LW(10), FLOAT(5.0f))
            WAIT_FRAMES(1)
            CALL(evt_npc_set_anim_localtime, LW(10), FLOAT(6.0f))
            WAIT_FRAMES(1)
            CALL(evt_npc_set_anim_localtime, LW(10), FLOAT(7.0f))
            WAIT_FRAMES(1)
            CALL(evt_npc_set_anim_localtime, LW(10), FLOAT(8.0f))
            WAIT_FRAMES(1)
            CALL(evt_npc_set_anim_localtime, LW(10), FLOAT(9.0f))
            WAIT_FRAMES(1)
            CALL(evt_npc_set_paper_anim, LW(10), STRING("PM_D_1A"))
            CALL(evt_npc_set_anim_localtime, LW(10), FLOAT(13.0f))
            WAIT_FRAMES(1)
            CALL(evt_npc_set_anim_localtime, LW(10), FLOAT(14.0f))
            WAIT_FRAMES(1)
            CALL(evt_npc_set_anim_localtime, LW(10), FLOAT(15.0f))
            WAIT_FRAMES(1)
            CALL(evt_npc_set_anim_localtime, LW(10), FLOAT(16.0f))
            WAIT_FRAMES(1)
            CALL(evt_npc_set_anim_localtime, LW(10), FLOAT(17.0f))
            WAIT_FRAMES(1)
            CALL(evt_npc_set_anim_localtime, LW(10), FLOAT(18.0f))
            WAIT_FRAMES(1)
            CALL(evt_npc_set_anim_localtime, LW(10), FLOAT(19.0f))
            WAIT_FRAMES(1)
            CALL(evt_npc_set_anim_localtime, LW(10), FLOAT(20.0f))
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, LW(10), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_MARIO_DOKAN1"), LW(0), LW(1), LW(2), 0)
        ADD(LW(2), -20)
        CALL(evt_npc_set_position, LW(10), LW(0), LW(1), LW(2))
        ADD(LW(0), 45)
        WAIT_MS(500)
        CALL(evt_npc_move_position, LW(10), LW(0), LW(2), 200, 0, 0)
        CALL(evt_npc_set_position, LW(10), 0, -1000, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(500)
        CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 100, 0, 200)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
            DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
            CALL(evt_npc_set_scale, STRING("第三勢力幹部"), LW(0), LW(0), LW(0))
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    RETURN
EVT_END()

static EVT_BEGIN(punipuni_mario_halt)
    SET_VALUE(GF(11), 1)
    CALL(evt_mario_key_onoff, 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(4000)
        SET_VALUE(GF(11), 0)
    END_INLINE_EVENT
    LOOP(0)
        IF_INT_EQUAL(GF(10), 0)
            BREAK_LOOP
        END_IF
        IF_INT_EQUAL(GF(11), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(time_bomb)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    SET_VALUE(GF(10), 0)
    WAIT_WHILE(GF(15))
    WAIT_MS(1000)
    CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 3000)
    CALL(evt_npc_set_ry, STRING("第三勢力幹部"), 270)
    CALL(evt_npc_flag_onoff, 1, STRING("第三勢力幹部"), NPC_FLAG_NO_HIT | NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION)
    SET_VALUE(LW(10), 1)
    LOOP(0)
        CALL(evt_puni_set_mode, LW(10), 30)
        CALL(evt_puni_set_anim, LW(10), 0)
        ADD(LW(10), 1)
        IF_INT_EQUAL(LW(10), 103)
            BREAK_LOOP
        END_IF
    END_LOOP
    BEGIN_INLINE_EVENT
        WAIT_MS(2000)
        CALL(evt_cam3d_evt_set, 555, 507, 846, 0, 50, 0, 2000, 11)
    END_INLINE_EVENT
    CALL(evt_snd_sfxon, STRING("SFX_STG2_QUAKE1"), LW(11))
    CALL(evt_cam_shake, 4, FLOAT(0.0009765625f), FLOAT(0.0f), 300)
    CALL(evt_cam_shake, 4, FLOAT(0.001953125f), FLOAT(0.0f), 300)
    CALL(evt_cam_shake, 4, FLOAT(0.0029296875f), FLOAT(0.0f), 300)
    CALL(evt_cam_shake, 4, FLOAT(0.00390625f), FLOAT(0.0f), 300)
    SET_VALUE(GF(0), 0)
    SPAWN_EVENT(PTR(flash_and_wait))
    CALL(evt_cam_shake, 4, FLOAT(0.0048828125f), FLOAT(0.0f), 300)
    CALL(evt_cam_shake, 4, FLOAT(0.005859375f), FLOAT(0.0f), 300)
    CALL(evt_cam_shake, 4, FLOAT(0.0068359375f), FLOAT(0.0f), 300)
#ifndef VERSION_G8ME01
    SPAWN_EVENT(PTR(flash2_and_wait))
#endif
    CALL(evt_cam_shake, 4, FLOAT(0.0078125f), FLOAT(0.0f), 300)
    CALL(evt_cam_shake, 4, FLOAT(0.0087890625f), FLOAT(0.0f), 300)
    CALL(evt_cam_shake, 4, FLOAT(0.009765625f), FLOAT(0.0f), 300)
    CALL(evt_cam_shake, 4, FLOAT(0.009765625f), FLOAT(0.0f), 300)
#ifndef VERSION_G8ME01
    SPAWN_EVENT(PTR(flash2_and_wait))
#endif
    CALL(evt_cam_shake, 4, FLOAT(0.009765625f), FLOAT(0.0f), 300)
    CALL(evt_cam_shake, 4, FLOAT(0.009765625f), FLOAT(0.0f), 300)
#ifndef VERSION_G8ME01
    SPAWN_EVENT(PTR(flash_and_wait))
#endif
    CALL(evt_cam_shake, 4, FLOAT(0.009765625f), FLOAT(0.0f), 300)
    CALL(evt_cam_shake, 4, FLOAT(0.009765625f), FLOAT(0.0f), 300)
    BEGIN_INLINE_EVENT
        CALL(evt_cam_shake, 4, FLOAT(0.009765625f), FLOAT(0.0f), 200)
        CALL(evt_cam_shake, 4, FLOAT(0.0087890625f), FLOAT(0.0f), 200)
        SPAWN_EVENT(PTR(flash_and_wait))
        CALL(evt_cam_shake, 4, FLOAT(0.0078125f), FLOAT(0.0f), 200)
        CALL(evt_cam_shake, 4, FLOAT(0.0068359375f), FLOAT(0.0f), 200)
        CALL(evt_cam_shake, 4, FLOAT(0.005859375f), FLOAT(0.0f), 200)
        CALL(evt_cam_shake, 4, FLOAT(0.0048828125f), FLOAT(0.0f), 200)
        CALL(evt_cam_shake, 4, FLOAT(0.00390625f), FLOAT(0.0f), 200)
        CALL(evt_cam_shake, 4, FLOAT(0.0029296875f), FLOAT(0.0f), 200)
        CALL(evt_cam_shake, 4, FLOAT(0.001953125f), FLOAT(0.0f), 200)
        CALL(evt_cam_shake, 4, FLOAT(0.0009765625f), FLOAT(0.0f), 200)
    END_INLINE_EVENT
    WAIT_MS(2000)
    CALL(evt_snd_sfxoff, LW(11))
    CALL(evt_cam3d_evt_set, 200, 305, -33, 0, 140, -338, 1000, 11)
    WAIT_MS(2000)
    CALL(evt_party_stop, 0)
    CALL(evt_mario_set_pos, 0, 0, -89)
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_party_set_pos, 0, -50, 0, -89)
    CALL(evt_party_set_dir, 0, 90, 0)
    BEGIN_INLINE_EVENT
        CALL(evt_eff64, STRING("ripple00"), STRING("ripple_n64"), 0, 0, 150, -340, 0, 0, 0, 0, 0, 0, 0, 0)
        WAIT_MS(400)
        CALL(evt_eff64, STRING("ripple01"), STRING("ripple_n64"), 0, 0, 150, -340, 0, 0, 0, 0, 0, 0, 0, 0)
        WAIT_MS(400)
        CALL(evt_eff64, STRING("ripple02"), STRING("ripple_n64"), 0, 0, 150, -340, 0, 0, 0, 0, 0, 0, 0, 0)
        WAIT_MS(400)
    END_INLINE_EVENT
    CALL(evt_snd_sfxon, STRING("SFX_STG2_STARSTONE_APPEAR1"), 0)
    CALL(evt_item_entry, STRING("item00"), ITEM_EMERALD_STAR, 0, 120, -340, 16, -1, 0)
    CALL(evt_item_flag_on, STRING("item00"), 2)
    SET_VALUE(LW(0), STRING("item00"))
    SET_VALUE(GW(10), 0)
    SPAWN_EVENT_GET_ID(PTR(starstone_rotate), LW(14))
    SPAWN_EVENT_GET_ID(PTR(starstone_rotate_exec), LW(15))
    CALL(evt_item_move_3d_position, STRING("item00"), 0, 150, -340, 1000, 0)
    SPAWN_EVENT_GET_ID(PTR(starstone_fuwafuwa), LW(13))
    WAIT_MS(2000)
    DELETE_EVENT(LW(13))
    CALL(evt_item_move_3d_position, STRING("item00"), 0, 400, -340, 750, 0)
    CALL(evt_item_set_position, STRING("item00"), 0, -1000, 0)
    WAIT_MS(500)
    CALL(evt_puni_set_dir, 1, 270)
    CALL(evt_cam3d_evt_set, 200, 213, 91, 0, 60, -215, 500, 11)
    WAIT_MS(500)
    CALL(evt_item_set_position, STRING("item00"), 0, 400, -215)
    CALL(evt_item_move_3d_position, STRING("item00"), 0, 50, -215, 3000, 11)
    SPAWN_EVENT_GET_ID(PTR(starstone_fuwafuwa), LW(13))
    WAIT_MS(2000)
    CALL(evt_cam3d_evt_set, 0, 95, 283, 0, 55, -215, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_msg_print_party, STRING("stg2_mri_e23_00_00"))
    CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_3RD_ENEMY"))
    CALL(evt_npc_set_position, STRING("第三勢力幹部"), 175, 0, -194)
    CALL(evt_npc_move_position, STRING("第三勢力幹部"), 0, -215, 0, FLOAT(180.0f), 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(150)
        CALL(evt_npc_get_position, STRING("第三勢力幹部"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_KANBU_STARSTONE_GET1"), LW(0), LW(1), LW(2), 0)
        DELETE_EVENT(LW(13))
        DELETE_EVENT(LW(14))
        DELETE_EVENT(LW(15))
        CALL(evt_item_delete, STRING("item00"))
    END_INLINE_EVENT
    CALL(evt_npc_jump_position_nohit, STRING("第三勢力幹部"), 0, 0, -215, 300, 30)
    CALL(evt_npc_set_ry, STRING("第三勢力幹部"), 90)
    WAIT_MS(200)
    CALL(evt_npc_move_position, STRING("第三勢力幹部"), 44, -245, 0, FLOAT(180.0f), 0)
    CALL(evt_npc_set_ry, STRING("第三勢力幹部"), 270)
    WAIT_MS(200)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_FIND1_3"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_eff_fukidashi, 0, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 60)
    CALL(evt_eff_fukidashi, 3, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 60)
    CALL(evt_mario_set_dir, 90, 200, 0)
    CALL(evt_party_set_dir, 0, 90, -1)
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_set, 248, 125, 212, -7, 38, -208, 500, 11)
    WAIT_MS(500)
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG2_MRI9"))
    CALL(evt_msg_print_party, STRING("stg2_mri_e23_00"))
    SET_VALUE(LW(10), STRING("第三勢力幹部"))
    CALL(evt_msg_print, 0, STRING("stg2_mri_e23_01"), 0, LW(10))
    WAIT_MS(500)
    CALL(evt_eff_fukidashi, 1, STRING(""), LW(10), 0, 0, 0, 0, 0, 0, 0, 60)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e23_02"), 0, LW(10))
    WAIT_MS(200)
    CALL(evt_npc_set_autotalkpose, LW(10), STRING("S_8"), STRING("T_8"))
    CALL(evt_npc_get_position, LW(10), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_BOMB_SWITCH_APPEAR1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_npc_set_anim, LW(10), STRING("I_6"))
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_set, 190, 95, 5, 43, 45, -239, 500, 11)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e23_03"), 0, LW(10))
    CALL(evt_npc_get_position, LW(10), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_BOMB_SWITCH_AWAY1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_npc_set_anim, LW(10), STRING("I_8"))
    WAIT_MS(1000)
    CALL(evt_npc_set_ry, LW(10), 90)
    BEGIN_INLINE_EVENT
        WAIT_MS(1000)
        CALL(evt_cam3d_evt_set, 240, 110, 237, 240, 50, -105, 0, 11)
    END_INLINE_EVENT
    CALL(evt_npc_move_position, LW(10), 220, -160, 0, FLOAT(120.0f), 0)
    WAIT_MS(500)
    CALL(evt_npc_set_ry, LW(10), 270)
    WAIT_MS(200)
    CALL(evt_npc_get_position, LW(10), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_BOMB_SWITCH_APPEAR1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_npc_set_anim, LW(10), STRING("I_6"))
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e23_04"), 0, LW(10))
    CALL(evt_npc_get_position, LW(10), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_BOMB_TIMER_SET1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_DANGER4"))
    CALL(evt_sub_countdown_start, 300000, 300000)
    CALL(evt_npc_set_anim, LW(10), STRING("I_7"))
    WAIT_MS(1000)
    CALL(evt_npc_get_position, LW(10), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_BOMB_SWITCH_AWAY1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_npc_set_anim, LW(10), STRING("I_8"))
    WAIT_MS(1000)
    CALL(evt_npc_get_position, LW(10), LW(0), LW(1), LW(2))
    CALL(evt_npc_jump_position_nohit, LW(10), LW(0), LW(1), LW(2), 200, 27)
    BEGIN_INLINE_EVENT
        WAIT_MS(150)
        CALL(evt_cam_shake, 4, FLOAT(0.01953125f), FLOAT(0.0f), 1000)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(350)
        CALL(evt_mapobj_get_position, STRING("S_dokan"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_DOKAN_FALL1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 200, 0, 500)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
            CALL(evt_mapobj_trans, 1, STRING("S_dokan"), 0, LW(0), 0)
            CALL(evt_hit_bind_update, STRING("A_dokan"))
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        SET_VALUE(LW(0), STRING("dokan"))
        CALL_CHILD(PTR(bero_case_switch_on))
    END_INLINE_EVENT
    WAIT_MS(1200)
    CALL(evt_npc_set_ry, LW(10), 90)
    WAIT_MS(200)
    CALL(evt_npc_move_position, LW(10), 250, -160, 0, FLOAT(180.0f), 0)
    WAIT_MS(500)
    CALL_CHILD(PTR(npc_dokan))
    WAIT_MS(2000)
    CALL(evt_cam3d_evt_set, 0, 105, 269, 0, 45, -76, 0, 11)
    WAIT_MS(200)
    CALL(evt_msg_print_party, STRING("stg2_mri_e23_05"))
    CALL(evt_cam3d_evt_off, 300, 11)
    SET_VALUE(LW(10), 1)
    LOOP(0)
        CALL(evt_puni_set_mode, LW(10), 1)
        CALL(evt_puni_flag_onoff, LW(10), 1, 32768)
        ADD(LW(10), 1)
        IF_INT_EQUAL(LW(10), 103)
            BREAK_LOOP
        END_IF
    END_LOOP
    SET_VALUE(GSW_STORY_PROGRESS, 109)
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(bomb_check)
    LOOP(0)
        IF_INT_EQUAL(GW(0), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    SPAWN_EVENT(PTR(time_bomb))
    RETURN
EVT_END()

static EVT_BEGIN(yuka_move)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_isk_02_c"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_isk_03_c"), MAP_OBJ_FLAG_HIDDEN)
    LOOP(0)
        SET_VALUE(LW(2), GW(0))
        DIVIDE(LW(2), 100)
        SET_VALUE(LW(1), GW(0))
        MODULO(LW(1), 100)
        DIVIDE(LW(1), 10)
        SET_VALUE(LW(0), GW(0))
        MODULO(LW(0), 10)
        SET_VALUE(LW(12), LW(2))
        MULTIPLY(LW(12), 36)
        SET_VALUE(LW(11), LW(1))
        MULTIPLY(LW(11), 36)
        SET_VALUE(LW(10), LW(0))
        MULTIPLY(LW(10), 36)
        CALL(evt_mapobj_rotate, 1, STRING("S_isk_02"), LW(10), 0, 0)
        CALL(evt_mapobj_rotate, 1, STRING("S_isk_03"), LW(11), 0, 0)
        CALL(evt_mapobj_rotate, 1, STRING("S_isk_04"), LW(12), 0, 0)
        IF_INT_GREATER_OR_EQUAL(GW(0), 10)
            CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_isk_03_c"), MAP_OBJ_FLAG_HIDDEN)
            ELSE
            CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_isk_03_c"), MAP_OBJ_FLAG_HIDDEN)
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

static EVT_BEGIN(yuka_check)
    SET_VALUE(LW(12), GW(0))
    CALL(evt_puni_wait_systemmode, 2)
    LOOP(0)
        SET_VALUE(LW(10), 1)
        SET_VALUE(LW(11), 0)
        LOOP(0)
            CALL(evt_puni_get_flag, LW(10), LW(0))
            CALL(evt_puni_get_flag, LW(10), LW(1))
            AND(LW(0), 1024)
            AND(LW(1), 128)
            IF_INT_NOT_EQUAL(LW(0), 0)
                IF_INT_EQUAL(LW(1), 0)
                    CALL(evt_puni_get_position, LW(10), LW(0), LW(1), LW(2))
                    IF_INT_GREATER_OR_EQUAL(LW(1), -20)
                        IF_INT_LESS_OR_EQUAL(LW(1), 20)
                            CALL(evt_sub_area_check, 0, -5, 0, 50, LW(0), LW(2), LW(15))
                            IF_INT_EQUAL(LW(15), 1)
                                ADD(LW(11), 1)
                            END_IF
                        END_IF
                    END_IF
                END_IF
            END_IF
            ADD(LW(10), 1)
            IF_INT_EQUAL(LW(10), 103)
                BREAK_LOOP
            END_IF
        END_LOOP
        SET_VALUE(GW(0), 101)
        SUBTRACT(GW(0), LW(11))
        IF_INT_GREATER_OR_EQUAL(GW(0), 101)
            SET_VALUE(GW(0), 101)
        END_IF
        IF_INT_LESS_OR_EQUAL(GW(0), 0)
            SET_VALUE(GW(0), 0)
        END_IF
        IF_INT_NOT_EQUAL(GW(0), LW(12))
            CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_SWITCH_COUNTER1"), 0, 0, -70, 0)
        END_IF
        SET_VALUE(LW(12), GW(0))
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

static KusaAccess ueki_access_data[] = {
    {1, "A_ueki01", "S_ueki01", NULL},
    {1, "A_ueki03", "S_ueki03", NULL},
    {1, "A_ueki04", "S_ueki04", NULL},
    {1, "A_ueki06", "S_ueki06", NULL},
    {1, "A_ueki07", "S_ueki07", NULL},
    {1, "A_ueki08", "S_ueki08", NULL},
    {4, NULL, NULL, NULL},
};

static EVT_BEGIN(punipuni_ball00_init)
    CALL_CHILD(PTR(mri_punipuni_setup))
    IF_INT_EQUAL(GSWF(2863), 1)
        CALL(evt_item_set_position, LW(10), 0, 30, -100)
        CALL(evt_target_flag_onoff, LW(11), 0, 3)
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_sui_hikari"), MAP_OBJ_FLAG_HIDDEN)
    END_IF
    CALL(evt_target_set_position, LW(11), 0, 0, 0)
    SET_VALUE(GF(15), 0)
    RETURN
EVT_END()

static EVT_BEGIN(punipuni_ball00)
    CALL(evt_mario_key_onoff, 0)
    IF_INT_EQUAL(GSWF(2863), 0)
        CALL_CHILD(PTR(mri_punipuni_itemselect))
        IF_INT_EQUAL(LW(1), 0)
            GOTO(100)
        END_IF
        SET_VALUE(GSWF(2863), 1)
        CALL(evt_item_set_position, LW(10), 0, 30, -100)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_sui_hikari"), MAP_OBJ_FLAG_HIDDEN)
        SET_VALUE(GF(15), 1)
        CALL_CHILD(PTR(mri_punipuni_itemset))
        SET_VALUE(GF(15), 0)
        ELSE
        SET_VALUE(GSWF(2863), 0)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_sui_hikari"), MAP_OBJ_FLAG_HIDDEN)
        CALL_CHILD(PTR(mri_punipuni_itemremove))
    END_IF
    LABEL(100)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(mri_14_init_evt)
    CALL(evt_puni_mem_init)
    CALL(evt_puni_set_yuka, 14, 0, -310, -1000, -1000, 0, 0)
    CALL(evt_puni_set_hamari, 0, 0, 0, 2, -2, -1000, -1000)
    SET_VALUE(LW(6), 0)
    SET_VALUE(LW(7), 0)
    SET_VALUE(LW(8), 0)
    SET_VALUE(LW(9), 0)
    IF_INT_NOT_EQUAL(GSW_STORY_PROGRESS, 107)
        CALL_CHILD(PTR(mri_puni_init))
        ELSE
        CALL_CHILD(PTR(mri_puni_init_none))
    END_IF
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    SPAWN_EVENT(PTR(puni_disp_info))
    SPAWN_EVENT(PTR(mri_countdown))
    SET_VALUE(LW(0), PTR(ueki_access_data))
    CALL_CHILD(PTR(evt_sub_kusa_access_entry))
    CALL(evt_paper_entry, STRING("p_dokan_x"))
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(109, 110)
        CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_DANGER4"))
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG2_MRI9"))
        CASE_DEFAULT
        CALL(evt_snd_bgmon, 512, STRING("BGM_STG2_MRI1"))
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG2_MRI1"))
    END_SWITCH
    CALL(evt_map_playanim, STRING("mizu_01"), 1, 0)
    CALL(evt_map_playanim, STRING("mizu_02"), 1, 0)
    CALL(evt_map_playanim, STRING("awa_01"), 1, 0)
    CALL(evt_map_playanim, STRING("awa_02"), 1, 0)
    CALL(evt_map_playanim, STRING("sui_hikari"), 1, 0)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 107)
        SPAWN_EVENT(PTR(dai3_kanbu))
    END_IF
    SET_VALUE(LW(10), STRING("punipuni"))
    SET_VALUE(LW(11), 0)
    SPAWN_EVENT(PTR(punipuni_ball00_init))
    CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_dai_mae"), 0, PTR(punipuni_ball00), 0)
    SET_VALUE(GW(0), 101)
    SPAWN_EVENT(PTR(yuka_check))
    SPAWN_EVENT(PTR(yuka_move))
    IF_INT_LESS(GSW_STORY_PROGRESS, 109)
        SPAWN_EVENT(PTR(bomb_check))
    END_IF
    IF_INT_LESS(GSW_STORY_PROGRESS, 109)
        CALL(evt_hit_bind_mapobj, STRING("A_dokan"), STRING("S_dokan"))
        CALL(evt_mapobj_trans, 1, STRING("S_dokan"), 0, 200, 0)
        CALL(evt_hit_bind_update, STRING("A_dokan"))
        SET_VALUE(LW(0), STRING("dokan"))
        CALL_CHILD(PTR(bero_case_switch_off))
    END_IF
    SET_VALUE(LW(0), STRING("S_sui"))
    SET_RAW_VALUE(LW(1), GW(0))
    SET_VALUE(LW(2), 100)
    SPAWN_EVENT(PTR(mri_hikari_color))
    RETURN
EVT_END()

