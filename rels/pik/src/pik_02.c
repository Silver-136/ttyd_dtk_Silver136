#include "battle/battle_unit.h"
#include "eff/eff_itemget.h"
#include "eff/eff_starstone.h"
#include "evt/evt_cmd.h"
#include "driver/camdrv.h"
#include "evt/evt_cam.h"
#include "evt/evt_case.h"
#include "evt/evt_eff.h"
#include "evt/evt_fade.h"
#include "evt/evt_hit.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_msg.h"
#include "evt/evt_party.h"
#include "evt/evt_seq.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_mobj.h"
#include "poly_trig.h"
#include "driver/mapdrv.h"
#include "driver/casedrv.h"
#include "pmario_sound.h"

#include "driver/effdrv.h"
#include "mario/mario.h"
#include "manager/evtmgr_cmd.h"
#include "driver/npcdrv.h"
#include "system.h"
#include "evt/evt_npc.h"
#include "evt/evt_bero.h"
#include "evt/evt_memcard.h"
#include "evt/evt_sub.h"
#include "mario/mario_cam.h"
#include "driver/seqdrv.h"
#include "pik.h"



USER_FUNC(evt_npc_set_stay_anim);
USER_FUNC(evt_snd_envoff_f);
USER_FUNC(evt_snd_sfx_all_stop);
static USER_FUNC(get_starstone_pos);
USER_FUNC(get_starstone_seq);
USER_FUNC(hoshi);
USER_FUNC(hoshi_delete);
USER_FUNC(hoshi_jump);
USER_FUNC(hoshi_r);
USER_FUNC(kpa_chg_pose);
USER_FUNC(reset_stone_cam);
static USER_FUNC(set_starstone_pos);
static USER_FUNC(starstone_entry);
static USER_FUNC(starstone_fall);
static USER_FUNC(stone_bg);
static USER_FUNC(npc_set_pos);

EvtWord haikei_on[24];
EvtWord haikei_off[28];
EvtWord evt_return_sub[99];
EvtWord evt_return[13];
EvtWord evt_go[7];
static EvtWord s_door_open[22];
static EvtWord s_door_close[22];
static BeroEntry bero_data[5];
EvtWord out_room[12];
BeroEntry s_bero_peach[1];
static EvtWord init_powan[30];
static EvtWord talk_powan[53];
EvtWord init_majo[17];
EvtWord init_mari[17];
EvtWord init_ramp[17];
EvtWord patakuri_init[7];
static EvtWord baba_init[7];
static NpcSetupInfo npc_data[7];
EvtWord evt_garnet[562];
static EvtWord evt_sw[89];
EvtWord evt_kaiga[14];
static EvtWord evt_daiza[27];
EvtWord pik_02_koopa_evt[1256];
EvtWord nise_starstone_init[18];
#ifdef VERSION_G8ME01
EvtWord pik_02_init_evt[172];
#else
EvtWord pik_02_init_evt[166];
#endif

EVT_BEGIN(haikei_on)
    CALL(evt_hitobj_onoff, STRING("a_E_yuka"), 1, 1)
    CALL(evt_hitobj_onoff, STRING("h_bero"), 1, 1)
    CALL(evt_hitobj_onoff, STRING("h_dokan"), 1, 1)
    CALL(evt_del_case_evt, 0, GW(10))
    SET_VALUE(LW(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(haikei_off)
    CALL(evt_hitobj_onoff, STRING("a_E_yuka"), 1, 0)
    CALL(evt_hitobj_onoff, STRING("h_bero"), 1, 0)
    CALL(evt_hitobj_onoff, STRING("h_dokan"), 1, 0)
    CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("a_kaiga"), 0, PTR(evt_kaiga), GW(10))
    SET_VALUE(LW(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(evt_return_sub)
    CALL(evt_mario_wait_movable)
    CALL(evt_mario_key_onoff, 0)
    WAIT_MS(300)
    BEGIN_INLINE_EVENT
        CALL(evt_party_move_pos2, 0, -170, -960, FLOAT(80.0f))
        CALL(evt_party_set_dir_npc, 0, STRING("ポワン探偵"))
    END_INLINE_EVENT
    CALL(evt_mario_mov_pos2, -160, -935, FLOAT(80.0f))
    CALL(evt_mario_set_dir_npc, STRING("ポワン探偵"))
    CALL(evt_cam3d_evt_set, -130, 64, -629, -130, 50, -929, 500, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg6_pik_21"), 0, STRING("ポワン探偵"))
    SET_VALUE(GSW_STORY_PROGRESS, STG6_54_WELCOME_BACK)
    CALL(evt_fade_set_mapchange_type, 1, 55, 0, 54, 0)
    SET_VALUE(LW(0), STRING("s_bero"))
    CALL_CHILD(PTR(bero_case_switch_off))
    CALL(evt_bero_overwrite, STRING("s_bero"), PTR(s_bero_peach))
    SET_VALUE(LW(0), STRING("s_bero"))
    CALL_CHILD(PTR(bero_case_switch_on))
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
    CALL(evt_mario_key_onoff, 1)
    BEGIN_INLINE_EVENT
        LOOP(0)
            CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
            IF_INT_GREATER(LW(0), -100)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_npc_reverse_ry, STRING("ポワン探偵"))
    END_INLINE_EVENT
    RETURN
EVT_END()

EVT_BEGIN(evt_return)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG6_53_THE_REAL_GARNET_STAR)
        SPAWN_EVENT(PTR(evt_return_sub))
    END_IF
    CALL_CHILD(PTR(haikei_off))
    SET_VALUE(LW(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(evt_go)
    CALL_CHILD(PTR(haikei_on))
    SET_VALUE(LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(s_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.38867188f))
    CALL(evt_mapobj_rotate, 1, STRING("door_1_a"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 1, STRING("door_1_b"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(s_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.38867188f))
    CALL(evt_mapobj_rotate, 1, STRING("door_1_a"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 1, STRING("door_1_b"), 0, LW(0), 0)
    RETURN
EVT_END()

static BeroEntry bero_data[] = {
    {
        "s_bero", // hitName
        1, // type
        6, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
#ifdef VERSION_G8ME01
        80, // length
#else
        -1, // length
#endif
        haikei_off, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "pik_01", // nextMapName
        "n_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        s_door_close, // enterAnimeArg
        s_door_open, // exitAnimeArg
    },
    {
        "dokan", // hitName
        2, // type
        0, // sfxId
        9, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        evt_return, // enterEvent
        6, // caseType
        evt_go, // exitEvent
        0, // nextMapName
        "h_dokan", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "h_dokan", // hitName
        4098, // type
        0, // sfxId
        9, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        NULL, 0, // exitEvent..nextMapName
        "dokan", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "h_bero", // hitName
        4096, // type
        0, // sfxId
        2, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, 0, NULL, // enterEvent..exitEvent
        "pik_03", // nextMapName
        "s_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

EVT_BEGIN(out_room)
    CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 2000)
    CALL(evt_snd_envoff_f, PSND_FADEOUT, 2000)
    CALL(evt_snd_sfx_all_stop)
    RETURN
EVT_END()

BeroEntry s_bero_peach[] = {
    {
        "s_bero", // hitName
        1, // type
        6, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        80, // length
        haikei_off, // enterEvent
        -1, // caseType
        out_room, // exitEvent
        "aji_10", // nextMapName
        NULL, // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        s_door_close, // enterAnimeArg
        s_door_open, // exitAnimeArg
    },
};

static EVT_BEGIN(init_powan)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG6_51_3_SURPRISE_GUESTS, STG6_52_THE_HIDDEN_PIPE)
        CALL(evt_npc_set_position, STRING("me"), -120, 0, 490)
        CASE_EQUAL(STG6_53_THE_REAL_GARNET_STAR)
        CALL(evt_npc_set_position, STRING("me"), -100, 10, -930)
        CASE_GREATER(STG6_53_THE_REAL_GARNET_STAR)
        CALL(evt_npc_set_position, STRING("me"), -120, 0, 490)
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(talk_powan)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG6_53_THE_REAL_GARNET_STAR)
        CALL(evt_msg_print, 0, STRING("stg6_pik_19"), 0, STRING("me"))
        CASE_LESS_OR_EQUAL(STG6_54_WELCOME_BACK)
        IF_INT_EQUAL(LSWF_PIK_02_PENNINGTON_TALK_TOGGLE, 0)
            SET_VALUE(LSWF_PIK_02_PENNINGTON_TALK_TOGGLE, 1)
            CALL(evt_msg_print, 0, STRING("stg6_pik_22"), 0, STRING("me"))
            ELSE
            SET_VALUE(LSWF_PIK_02_PENNINGTON_TALK_TOGGLE, 0)
            CALL(evt_msg_print, 0, STRING("stg6_pik_23"), 0, STRING("me"))
        END_IF
        CASE_LESS_OR_EQUAL(STG8_END_AFTER_THE_BATTLE)
        CALL(evt_msg_print, 0, STRING("stg7_pik_clear_09_1"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg7_pik_clear_09_1_1"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(init_majo)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG6_51_3_SURPRISE_GUESTS)
        CALL(evt_npc_set_position, STRING("me"), -35, 10, -950)
        CALL(evt_npc_set_ry, STRING("me"), 90)
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(init_mari)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG6_51_3_SURPRISE_GUESTS)
        CALL(evt_npc_set_position, STRING("me"), 30, 10, -950)
        CALL(evt_npc_set_ry, STRING("me"), 270)
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(init_ramp)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG6_51_3_SURPRISE_GUESTS)
        CALL(evt_npc_set_position, STRING("me"), 20, 10, -910)
        CALL(evt_npc_set_ry, STRING("me"), 270)
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(patakuri_init)
    CALL(evt_npc_status_onoff, 1, STRING("me"), NPC_STATUS_FACE_MOVE_DIR)
    RETURN
EVT_END()

static EVT_BEGIN(baba_init)
    CALL(evt_npc_status_onoff, 1, STRING("me"), NPC_STATUS_FACE_MOVE_DIR)
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
        /*"マジョリン"*/"\x83\x7D\x83\x57\x83\x87\x83\x8A\x83\x93", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_majo, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"マリリン"*/"\x83\x7D\x83\x8A\x83\x8A\x83\x93", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_mari, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ランペル"*/"\x83\x89\x83\x93\x83\x79\x83\x8B", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_ramp, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"パタクリ"*/"\x83\x70\x83\x5E\x83\x4E\x83\x8A", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        patakuri_init, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ババ"*/"\x83\x6F\x83\x6F", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        baba_init, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

static USER_FUNC(npc_set_pos) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    f32 x = evtGetFloat(event, args[1]);
    f32 y = evtGetFloat(event, args[2]);
    f32 z = evtGetFloat(event, args[3]);
    NpcEntry* npc = evtNpcNameToPtr(event, name);

    npc->prevPosition = (Vec){x, y, z};
    npc->position = npc->prevPosition;
    return EVT_RETURN_DONE;
}

USER_FUNC(hoshi) {
    event->lwData[10] = (EvtWord)effStarStoneEntry(5, 0.0f, 60.0f, -950.0f, 1.0f);
    return EVT_RETURN_DONE;
}

USER_FUNC(hoshi_delete) {
    effSoftDelete((EffectEntry*)event->lwData[10]);
    return EVT_RETURN_DONE;
}

USER_FUNC(hoshi_r) {
    EffectEntry* eff = (EffectEntry*)event->lwData[10];

    ((EffStarStoneWork*)eff->userdata)->rot.y = evtGetValue(event, event->args[0]);
    return EVT_RETURN_DONE;
}

USER_FUNC(hoshi_jump) {
    f32* pos = (f32*)((EffectEntry*)event->lwData[10])->userdata + 2;
    NpcEntry* npc = npcNameToPtr("\203}\203W\203\207\203\212\203\223");
    f32 vy;
    f32 speed;
    f32 t;
    f32 x = npc->position.x;
    f32 y = 50.0f + npc->position.y;
    f32 z = npc->position.z;
    f32 angle;
    f32 g;
    f32 dy;

    speed = 0.5f;
    if (isFirstCall) {
        t = distABf(x, z, pos[0], pos[2]) / speed;
        dy = y - pos[1];
        g = 0.1f * t;
        vy = dy / t + g / 2.0f;
        event->userdata[0] = *(s32*)&t;
        event->userdata[1] = *(s32*)&vy;
        event->userdata[2] = *(s32*)&speed;
        event->userdata3 = 0;
    }
    t = event->userdataf[0];
    vy = event->userdataf[1];
    speed = event->userdataf[2];
    angle = 6.2831855f * angleABf(pos[0], pos[2], x, z) / 360.0f;
    pos[0] = speed * polySinf(angle) + pos[0];
    pos[2] = pos[2] - speed * polyCosf(angle);
    pos[1] += vy;
    vy -= 0.1f;
    t -= 1.0f;
    if (t < 0.0f) {
        pos[0] = x;
        pos[1] = y;
        pos[2] = z;
        return EVT_RETURN_DONE;
    }
    event->userdata[0] = *(s32*)&t;
    event->userdata[1] = *(s32*)&vy;
    return EVT_RETURN_BLOCK;
}

USER_FUNC(get_starstone_seq) {
    EvtWord* args = event->args;
    EffectEntry* eff = effNameToPtr((const char*)evtGetValue(event, args[0]));

    evtSetValue(event, args[1], ((EffStarStoneWork*)eff->userdata)->state);
    return EVT_RETURN_DONE;
}

static USER_FUNC(starstone_fall) {
    EffectEntry* eff = effNameToPtr((const char*)evtGetValue(event, event->args[0]));

    ((EffStarStoneWork*)eff->userdata)->state = 1;
    return EVT_RETURN_DONE;
}

static USER_FUNC(set_starstone_pos) {
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

static USER_FUNC(get_starstone_pos) {
    EvtWord* args = event->args;
    EffectEntry* eff = effNameToPtr((const char*)evtGetValue(event, args[0]));

    evtSetFloat(event, args[1], ((EffStarStoneWork*)eff->userdata)->position.x);
    evtSetFloat(event, args[2], ((EffStarStoneWork*)eff->userdata)->position.y);
    evtSetFloat(event, args[3], ((EffStarStoneWork*)eff->userdata)->position.z);
    return EVT_RETURN_DONE;
}

static USER_FUNC(starstone_entry) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    s32 type = evtGetValue(event, args[1]);
    f32 x = evtGetFloat(event, args[2]);
    f32 y = evtGetFloat(event, args[3]);
    f32 z = evtGetFloat(event, args[4]);
    f32 scale = evtGetFloat(event, args[5]);
    EffectEntry* eff = effStarStoneEntry(type, x, y, z, scale);

    effSetName(eff, name);
    ((EffStarStoneWork*)eff->userdata)->flags |= 0x10;
    return EVT_RETURN_DONE;
}

static USER_FUNC(stone_bg) {
    EffectEntry* eff;

    eff = effNameToPtr("sub_bg");
    ((ItemGetWork*)eff->userdata)->camera = 5;
    eff = effNameToPtr("nise");
    ((EffStarStoneWork*)eff->userdata)->cam = 5;
    marioSetCamId(CAMERA_3D_EFFECTA);
    return EVT_RETURN_DONE;
}

USER_FUNC(reset_stone_cam) {
    EffectEntry* eff = effNameToPtr("nise");

    ((EffStarStoneWork*)eff->userdata)->cam = 4;
    return EVT_RETURN_DONE;
}

EVT_BEGIN(evt_garnet)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mapobj_rotate, 1, STRING("door_1_a"), 0, 70, 0)
    CALL(evt_mapobj_rotate, 1, STRING("door_1_b"), 0, -70, 0)
    CALL(evt_mario_set_pos, 0, 0, 650)
    CALL(evt_party_set_pos, 0, 30, 0, 680)
    CALL(evt_npc_set_position, STRING("ポワン探偵"), -30, 0, 710)
    CALL(evt_npc_set_ry, STRING("ポワン探偵"), 90)
    CALL(hoshi)
    CALL(evt_seq_wait, SEQ_GAME)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_mov_pos, 0, 570, 1000)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_party_move_pos, 0, 30, 580, 1000)
    END_INLINE_EVENT
    CALL(evt_npc_status_onoff, 1, STRING("ポワン探偵"), NPC_STATUS_FACE_MOVE_DIR)
    CALL(evt_npc_move_position, STRING("ポワン探偵"), -30, 600, 1000, 0, 1)
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT(PTR(bero_close_door_play_se))
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 70, 0, 500)
    LABEL(1)
    CALL(evt_sub_intpl_msec_get_value)
    CALL(evt_mapobj_rotate, 1, STRING("door_1_a"), 0, LW(0), 0)
    MULTIPLY(LW(0), -1)
    CALL(evt_mapobj_rotate, 1, STRING("door_1_b"), 0, LW(0), 0)
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(1)
    END_IF
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_SURPRISED2_3"), LW(0), LW(1), LW(2), 0)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_MARIO, STRING("eff0"), 0, 0, 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_PARTNER, STRING("eff1"), 0, 0, 0, 0, 0, 0, 0, 0, 0)
    WAIT_MS(500)
    CALL(evt_cam3d_evt_set, 0, 125, -590, 0, 65, -930, 1000, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    CALL(evt_snd_bgmoff, PSND_PAUSE)
    CALL(evt_snd_bgmon, PSND_SLOT(1), STRING("BGM_FF_ENV_SURPRISED1"))
    WAIT_MS(1000)
    WAIT_MS(500)
    CALL(evt_npc_set_anim, STRING("マジョリン"), STRING("MJR_A2_1"))
    WAIT_MS(1000)
    BEGIN_INLINE_EVENT_GET_ID(LW(15))
        LOOP(0)
            CALL(hoshi_r, LW(0))
            ADD(LW(0), 20)
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    CALL(hoshi_jump)
    WAIT_MS(500)
    DELETE_EVENT(LW(15))
    CALL(hoshi_delete)
    CALL(evt_npc_get_position, STRING("マジョリン"), LW(0), LW(1), LW(2))
    ADD(LW(1), 50)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_STARATONE_LIGHT1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_eff64, 0, STRING("stardust_n64"), 0, LW(0), LW(1), LW(2), FLOAT(20.0f), 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_WITCH1"))
    CALL(evt_snd_envoff, PSND_PAUSE)
    CALL(evt_msg_print, 0, STRING("stg6_pik_13"), 0, STRING("マジョリン"))
    BEGIN_INLINE_EVENT
        CALL(evt_npc_majo_disp_off, STRING("マジョリン"))
    END_INLINE_EVENT
    WAIT_MS(1000)
    CALL(evt_npc_majo_disp_off, STRING("マリリン"))
    CALL(evt_npc_set_position, STRING("マジョリン"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("マリリン"), 0, -1000, 0)
    LOOP(2)
        CALL(evt_npc_reverse_ry, STRING("ランペル"))
        WAIT_MS(400)
    END_LOOP
    CALL(evt_msg_print, 0, STRING("stg6_pik_14"), 0, STRING("ランペル"))
    CALL(evt_cam3d_evt_set, 0, 54, -568, -230, 30, -930, 2900, EASING_SINE_IN_OUT)
    CALL(evt_npc_status_onoff, 1, STRING("ランペル"), NPC_STATUS_FACE_MOVE_DIR)
    CALL(evt_npc_flag_onoff, 1, STRING("ランペル"), NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_NO_HIT | NPC_FLAG_GRAVITY)
    CALL(evt_npc_move_position, STRING("ランペル"), -105, -845, 0, FLOAT(100.0f), 1)
    CALL(evt_npc_flag_onoff, 0, STRING("ランペル"), NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_GRAVITY)
    CALL(evt_npc_move_position, STRING("ランペル"), -185, -795, 0, FLOAT(100.0f), 1)
    WAIT_MS(500)
    CALL(evt_npc_jump_position_nohit, STRING("ランペル"), -250, 0, -795, FLOAT(300.0f), 40)
    CALL(evt_npc_set_position, STRING("ランペル"), 0, -1000, 0)
    WAIT_MS(300)
    CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 3000)
    CALL(evt_snd_envoff, PSND_FADEOUT | PSND_SLOT(1))
    CALL(evt_snd_envon, PSND_CONTINUE | PSND_FADEIN, 0)
    CALL(evt_npc_flag_onoff, 1, STRING("ポワン探偵"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_party_set_pos, 0, -90, 0, 450)
    CALL(evt_mario_set_pos, -70, 0, 490)
    CALL(evt_npc_set_position, STRING("ポワン探偵"), -120, 0, 490)
    CALL(evt_set_dir_to_target, STRING("ポワン探偵"), STRING("mario"))
    CALL(evt_party_stop, 0)
    CALL(evt_party_set_dir, 0, 90, 0)
    CALL(evt_mario_set_dir, 270, 0, 0)
    CALL(evt_cam3d_evt_set, -100, 77, 748, -100, 35, 450, 0, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_party_stop, 0)
    WAIT_MS(300)
    CALL(evt_mario_get_party, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(1)
        CALL(evt_msg_print, 0, STRING("stg6_pik_15_0"), 0, STRING("party"))
        CASE_EQUAL(2)
        CALL(evt_msg_print, 0, STRING("stg6_pik_15_1"), 0, STRING("party"))
        CASE_EQUAL(4)
        CALL(evt_msg_print, 0, STRING("stg6_pik_15_2"), 0, STRING("party"))
        CASE_EQUAL(5)
        CALL(evt_msg_print, 0, STRING("stg6_pik_15_3"), 0, STRING("party"))
        CASE_EQUAL(6)
        CALL(evt_msg_print, 0, STRING("stg6_pik_15_4"), 0, STRING("party"))
        CASE_EQUAL(3)
        CALL(evt_msg_print, 0, STRING("stg6_pik_15_5"), 0, STRING("party"))
        CASE_EQUAL(7)
        CALL(evt_msg_print, 0, STRING("stg6_pik_15_6"), 0, STRING("party"))
    END_SWITCH
    WAIT_MS(500)
    CALL(evt_party_run, 0)
    CALL(evt_msg_print, 0, STRING("stg6_pik_16"), 0, STRING("ポワン探偵"))
    WAIT_MS(500)
    CALL(evt_party_set_dir, 0, 270, 200)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_HAND_UP1_4"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING("M_I_2"))
    WAIT_MS(800)
    WAIT_MS(1000)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_msg_print, 0, STRING("stg6_pik_17"), 0, STRING("ポワン探偵"))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_AWAKE1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING("M_N_5B"))
    WAIT_MS(500)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_msg_print, 0, STRING("stg6_pik_18"), 0, STRING("ポワン探偵"))
    WAIT_MS(500)
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
    SET_VALUE(GSW_STORY_PROGRESS, STG6_51_3_SURPRISE_GUESTS)
    CALL(evt_bero_set_mario_bottomless_respawn_pos_to_current_mario_pos)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(evt_sw)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_cam3d_evt_set, -65, 450, -480, -165, 115, -930, 1000, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    WAIT_MS(1000)
    CALL(evt_mapobj_get_position, STRING("ka_ten"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_CURTAIN_UP1"), LW(0), LW(1), LW(2), LW(9))
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 150, 1500)
    LABEL(1)
    CALL(evt_sub_intpl_msec_get_value)
    CALL(evt_mapobj_trans, 1, STRING("ka_ten"), 0, LW(0), 0)
    CALL(evt_hit_bind_update, STRING("a_ka_ten"))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(1)
    END_IF
    CALL(evt_snd_sfxoff, LW(9))
    SET_VALUE(GSW_STORY_PROGRESS, STG6_52_THE_HIDDEN_PIPE)
    SET_VALUE(LW(0), 1)
    CALL_CHILD(PTR(bero_case_switch_on))
    CALL(evt_snd_bgmoff, PSND_PAUSE)
    CALL(evt_snd_bgmon, PSND_SLOT(1), STRING("BGM_FF_ENV_SUCCESS1"))
    WAIT_MS(1000)
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(evt_kaiga)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_msg_print, 0, STRING("stg6_pik_24"), 0, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(evt_daiza)
    CALL(evt_mario_key_onoff, 0)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG6_70_THOUSAND_YEAR_DOOR, STG7_25_00_BOWSER_POSHLEY_HEIGHTS)
        CALL(evt_msg_print, 0, STRING("stg7_pik_clear_09_2"), 0, 0)
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg6_pik_25"), 0, 0)
    END_SWITCH
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(pik_02_koopa_evt)
    CALL(evt_snd_envoff, PSND_FADEOUT)
    CALL(evt_snd_set_rev_mode, 2)
    CALL(evt_mario_key_onoff, 0)
    CALL(starstone_entry, STRING("nise"), 5, 0, 57, -955, 1)
    CALL(evt_mario_set_pos, 60, 0, -500)
    CALL(evt_npc_set_position, STRING("ババ"), 140, 0, -500)
    CALL(evt_mario_set_dir, 270, 0, 0)
    CALL(evt_npc_set_ry, STRING("ババ"), 270)
    CALL(evt_cam3d_evt_set, 0, 360, -338, 0, 250, -1130, 0, EASING_SINE_IN_OUT)
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_KUPPA1"))
    CALL(evt_snd_bgm_scope, 0, 1)
    CALL(evt_cam3d_evt_set, 0, 150, -338, 0, 40, -1130, 7000, EASING_LINEAR)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING("ババ"), 140, -880, 0, 60, 0)
        CALL(evt_npc_set_position, STRING("ババ"), 140, 9, -885)
        CALL(evt_npc_move_position, STRING("ババ"), 140, -925, 0, 60, 0)
    END_INLINE_EVENT
    CALL(evt_mario_mov_pos2, 60, -880, 60)
    CALL(evt_mario_set_pos, 60, 9, -885)
    CALL(evt_mario_mov_pos2, 60, -925, 60)
    CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("KPA_S_1"), STRING("KPA_T_1"))
    CALL(evt_msg_print, 0, STRING("kpa_stg7_013_01"), 0, STRING("mario"))
    CALL(evt_cam3d_evt_set, 60, 127, -467, 60, 80, -914, 500, EASING_SINE_IN_OUT)
    WAIT_MS(600)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_KOOPA_HAPPY2_1"), LW(0), LW(1), LW(2), 0)
    CALL(set_starstone_pos, STRING("nise"), 80, 85, -915)
    CALL(kpa_chg_pose, STRING("KPA_Y_1"))
    CALL(evt_eff64, STRING("sub_hikari"), STRING("radiation_n64"), 2, 60, 56, -925, 1, 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_eff, STRING("sub_bg"), STRING("itemget"), 0, 60, 56, -935, 0, 0, 0, 0, 0, 0, 0, 0)
    CALL(stone_bg)
    CALL(evt_snd_bgmoff, PSND_PAUSE)
    CALL(evt_snd_bgmon, PSND_SLOT(1), STRING("BGM_FF_GET_IMPORTANT_ITEM1"))
    WAIT_MS(2000)
    CALL(evt_snd_bgmoff, PSND_FADEOUT | PSND_SLOT(1))
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_FADEIN, 0)
    CALL(evt_eff_softdelete, STRING("sub_hikari"))
    CALL(evt_eff_softdelete, STRING("sub_bg"))
    WAIT_MS(300)
    CALL(reset_stone_cam)
    CALL(evt_mario_init_camid)
    CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("KPA_S_6"), STRING("KPA_T_6"))
    CALL(evt_msg_print, 0, STRING("kpa_stg7_014"), 0, STRING("mario"))
    WAIT_MS(500)
    CALL(evt_npc_set_autotalkpose, STRING("ババ"), STRING("U_1"), STRING("T_5"))
    CALL(evt_msg_print, 0, STRING("kpa_stg7_015"), 0, STRING("ババ"))
    CALL(evt_cam3d_evt_set, 19, 143, -268, 19, 74, -914, 800, EASING_SINE_IN_OUT)
    WAIT_MS(800)
    CALL(evt_npc_status_onoff, 1, STRING("ポワン探偵"), NPC_STATUS_FACE_MOVE_DIR)
    CALL(evt_npc_set_position, STRING("ポワン探偵"), -80, 0, -600)
    CALL(evt_npc_set_ry, STRING("ポワン探偵"), 90)
    WAIT_MS(300)
    CALL(evt_npc_move_position, STRING("ポワン探偵"), -70, -800, 0, FLOAT(80.0f), 0)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_NPC, STRING(""), STRING("ポワン探偵"), 0, 0, 0, 0, 0, 0, 0, 60)
    WAIT_MS(1000)
    BEGIN_INLINE_EVENT
        WAIT_MS(700)
        CALL(kpa_chg_pose, STRING("KPA_S_1"))
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        SUBTRACT_FLOAT(LW(0), 35)
        ADD_FLOAT(LW(1), 20)
        CALL(set_starstone_pos, STRING("nise"), LW(0), LW(1), LW(2))
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_set, -65, 60, -563, -65, 40, -762, 800, EASING_SINE_IN_OUT)
    WAIT_MS(800)
    CALL(evt_msg_print, 0, STRING("kpa_stg7_016"), 0, STRING("ポワン探偵"))
    CALL(evt_npc_set_autotalkpose, STRING("ババ"), STRING("S_1"), STRING("T_1"))
    CALL(evt_npc_set_stay_anim, STRING("ババ"))
    CALL(evt_cam3d_evt_set, 10, 113, -367, 10, 55, -914, 1000, EASING_SINE_IN_OUT)
    WAIT_MS(1300)
    SET_VALUE(GF(0), 0)
    BEGIN_INLINE_EVENT
        LOOP(0)
            IF_INT_NOT_EQUAL(GF(0), 0)
                BREAK_LOOP
            END_IF
            CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
            SUBTRACT_FLOAT(LW(0), 35)
            ADD_FLOAT(LW(1), 20)
            CALL(set_starstone_pos, STRING("nise"), LW(0), LW(1), LW(2))
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        SUBTRACT_FLOAT(LW(0), 35)
        ADD_FLOAT(LW(1), 20)
        CALL(set_starstone_pos, STRING("nise"), LW(0), LW(1), LW(2))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_flag_onoff, 1, STRING("ババ"), NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_GRAVITY)
        CALL(evt_npc_move_position, STRING("ババ"), 111, -781, 1750, 0, 0)
    END_INLINE_EVENT
    CALL(evt_mario_mov_pos, 60, -800, 1750)
    SET_VALUE(GF(0), 1)
    CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("KPA_S_1"), STRING("KPA_T_1"))
    CALL(evt_msg_print, 0, STRING("kpa_stg7_017"), 0, STRING("mario"))
    CALL(evt_msg_select, 0, STRING("kpa_stg7_018"))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        SET_VALUE(LW(1), STRING("kpa_stg7_019"))
        SET_VALUE(LW(2), STRING("kpa_stg7_020"))
        CASE_EQUAL(1)
        SET_VALUE(LW(1), STRING("kpa_stg7_021"))
        SET_VALUE(LW(2), STRING("kpa_stg7_022"))
        CASE_EQUAL(2)
        SET_VALUE(LW(1), STRING("kpa_stg7_023"))
        SET_VALUE(LW(2), STRING("kpa_stg7_024"))
    END_SWITCH
    CALL(evt_msg_print_add, 0, LW(1))
    CALL(evt_cam3d_evt_set, -65, 60, -563, -65, 40, -762, 800, EASING_SINE_IN_OUT)
    WAIT_MS(800)
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_npc_get_position, STRING("ポワン探偵"), LW(3), LW(4), LW(5))
        SET_VALUE(LW(6), LW(3))
        SUBTRACT(LW(6), 1)
        LOOP(10)
            CALL(evt_npc_set_position, STRING("ポワン探偵"), LW(6), LW(4), LW(5))
            WAIT_FRAMES(2)
            ADD(LW(6), 2)
            CALL(evt_npc_set_position, STRING("ポワン探偵"), LW(6), LW(4), LW(5))
            WAIT_FRAMES(2)
            SUBTRACT(LW(6), 2)
        END_LOOP
        CALL(evt_npc_set_position, STRING("ポワン探偵"), LW(3), LW(4), LW(5))
    END_IF
    CALL(evt_msg_print, 0, LW(2), 0, STRING("ポワン探偵"))
    CALL(evt_cam3d_evt_set, 10, 113, -367, 10, 55, -914, 800, EASING_SINE_IN_OUT)
    WAIT_MS(800)
    BEGIN_INLINE_EVENT
        CALL(evt_snd_bgmoff, PSND_PAUSE)
        CALL(evt_snd_bgmon, PSND_SLOT(1), STRING("BGM_FF_ENV_SURPRISED3"))
        WAIT_MS(1000)
        CALL(evt_snd_bgmoff, PSND_FADEOUT | PSND_SLOT(1))
        CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_FADEIN, 0)
    END_INLINE_EVENT
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_KOOPA_SURPRISED2_1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_npc_set_anim, STRING("ババ"), STRING("P_1"))
    CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("KPA_S_5"), STRING("KPA_T_5"))
    CALL(evt_msg_print, 0, STRING("kpa_stg7_025"), 0, STRING("mario"))
    CALL(get_starstone_pos, STRING("nise"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_KUPPA_STONE_DOWN1"), LW(0), LW(1), LW(2), LW(15))
    BEGIN_INLINE_EVENT
        LOOP(0)
            CALL(get_starstone_seq, STRING("nise"), LW(0))
            IF_INT_EQUAL(LW(0), 3)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_snd_sfxoff, LW(15))
        CALL(get_starstone_pos, STRING("nise"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_KUPPA_STONE_BREAK1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(starstone_fall, STRING("nise"))
    WAIT_MS(1000)
    CALL(get_starstone_pos, STRING("nise"), LW(0), LW(1), LW(2))
    LOOP(10)
        CALL(set_starstone_pos, STRING("nise"), 0, -1000, 0)
        WAIT_FRAMES(2)
        CALL(set_starstone_pos, STRING("nise"), LW(0), LW(1), LW(2))
        WAIT_FRAMES(2)
    END_LOOP
    CALL(evt_eff_softdelete, STRING("nise"))
    CALL(evt_npc_flag_onoff, 1, STRING("ババ"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_move_position, STRING("ババ"), -13, -792, 0, FLOAT(90.0f), 0)
    CALL(evt_npc_flag_onoff, 0, STRING("ババ"), NPC_FLAG_NO_HIT)
    WAIT_FRAMES(1)
    CALL(evt_msg_print, 0, STRING("kpa_stg7_026"), 0, STRING("ババ"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("kpa_stg7_027"), 0, STRING("ポワン探偵"))
    CALL(evt_npc_set_ry, STRING("ババ"), 90)
    WAIT_MS(300)
    CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("KPA_S_1"), STRING("KPA_T_1"))
    CALL(evt_msg_print, 0, STRING("kpa_stg7_028"), 0, STRING("mario"))
    CALL(kpa_chg_pose, STRING("KPA_I_2"))
    SET_VALUE(GF(0), 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(200)
        SPAWN_EVENT_GET_ID(PTR(evt_sub_stopwatch), LW(0))
        LOOP(0)
            CALL(evt_sub_get_stopwatch, LW(0), LW(1))
            IF_INT_GREATER_OR_EQUAL(LW(1), 30000)
                BREAK_LOOP
            END_IF
            IF_INT_NOT_EQUAL(GF(0), 0)
                BREAK_LOOP
            END_IF
            CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0f), FLOAT(0.009765625f), 150)
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
    CALL(evt_msg_print, 0, STRING("kpa_stg7_029"), 0, STRING("ババ"))
    WAIT_MS(1000)
    CALL(evt_npc_set_position, STRING("パタクリ"), -250, 130, -800)
    CALL(evt_snd_sfxon_3d, STRING("SFX_KUPPA_PKURI_GLASS_BREAK1"), -250, 130, -800, LW(0))
    LOOP(0)
        CALL(evt_snd_sfxchk, LW(0), LW(1))
        IF_INT_NOT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(kpa_chg_pose, STRING("KPA_S_1"))
    SET_VALUE(GF(0), 1)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_KOOPA_SURPRISED1_2"), LW(0), LW(1), LW(2), 0)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_MARIO, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 60)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_NPC, STRING(""), STRING("ババ"), 0, 0, 0, 0, 0, 0, 0, 60)
    WAIT_MS(1000)
    CALL(evt_mario_set_dir, 270, 300, 0)
    CALL(evt_npc_set_ry, STRING("ババ"), 270)
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("kpa_stg7_030"), 0, STRING("パタクリ"))
    CALL(evt_npc_set_position, STRING("パタクリ"), -200, 80, -800)
    CALL(evt_npc_set_ry, STRING("パタクリ"), 90)
    WAIT_MS(1000)
    SET_VALUE(GW(0), 80)
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 180, 330, 1300)
        LABEL(1)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_sub_get_sincos, LW(0), LW(2), LW(3))
        MULTIPLY_FLOAT(LW(2), FLOAT(50.0f))
        ADD_FLOAT(LW(2), FLOAT(80.0f))
        SET_VALUE(GW(0), LW(2))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(1)
        END_IF
    END_INLINE_EVENT
    CALL(evt_npc_get_position, STRING("ポワン探偵"), LW(2), LW(3), LW(4))
    CALL(evt_sub_intpl_msec_init, EASING_LINEAR, -200, LW(2), 1300)
    LABEL(2)
    CALL(evt_sub_intpl_msec_get_value)
    CALL(evt_npc_set_position, STRING("パタクリ"), LW(0), GW(0), -800)
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(2)
    END_IF
    CALL(evt_npc_flag_onoff, 0, STRING("パタクリ"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_set_anim, STRING("パタクリ"), STRING("PTK_A_1"))
    CALL(evt_npc_get_position, STRING("パタクリ"), LW(2), LW(3), LW(4))
    CALL(evt_sub_intpl_msec_init, EASING_QUADRATIC_OUT, LW(3), 30, 1200)
    LABEL(0)
    CALL(evt_sub_intpl_msec_get_value)
    CALL(evt_npc_set_position, STRING("パタクリ"), LW(2), LW(0), LW(4))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    CALL(evt_npc_set_anim, STRING("パタクリ"), STRING("PTK_S_2"))
    SET_VALUE(LW(5), LW(0))
    SUBTRACT(LW(5), 5)
    CALL(evt_eff, STRING(""), STRING("kemuri"), 0, LW(2), LW(5), LW(4), 0, 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_snd_sfxon_3d, STRING("SFX_KUPPA_PKURI_LANDING1"), LW(2), LW(0), LW(4), 0)
    WAIT_MS(300)
    CALL(evt_npc_set_anim, STRING("パタクリ"), STRING("PTK_S_3"))
    CALL(evt_msg_print, 0, STRING("kpa_stg7_031"), 0, STRING("ババ"))
    CALL(evt_cam3d_evt_set, -65, 83, -504, -65, 47, -852, 800, EASING_SINE_IN_OUT)
    WAIT_MS(800)
    CALL(evt_npc_set_autotalkpose, STRING("パタクリ"), STRING("PTK_S_3"), STRING("PTK_T_2"))
    CALL(evt_msg_print, 0, STRING("kpa_stg7_032"), 0, STRING("パタクリ"))
    CALL(evt_npc_set_autotalkpose, STRING("ババ"), STRING("S_1"), STRING("T_1"))
    CALL(evt_npc_set_stay_anim, STRING("ババ"))
    CALL(evt_cam3d_evt_set, 20, 114, -369, 20, 58, -914, 800, EASING_SINE_IN_OUT)
    WAIT_MS(800)
    CALL(evt_msg_print, 0, STRING("kpa_stg7_033"), 0, STRING("mario"))
    CALL(evt_npc_set_ry, STRING("ババ"), 90)
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("kpa_stg7_034"), 0, STRING("ババ"))
    CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("KPA_S_4"), STRING("KPA_T_3"))
    CALL(evt_msg_print, 0, STRING("kpa_stg7_035"), 0, STRING("mario"))
    WAIT_MS(300)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_mario_mov_pos2, LW(0), -400, FLOAT(90.0f))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(400)
        CALL(evt_npc_get_position, STRING("ババ"), LW(0), LW(1), LW(2))
        CALL(evt_npc_move_position, STRING("ババ"), LW(0), -400, 0, FLOAT(90.0f), 0)
    END_INLINE_EVENT
    WAIT_MS(800)
    CALL(evt_npc_flag_onoff, 1, STRING("パタクリ"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_get_position, STRING("パタクリ"), LW(0), LW(1), LW(2))
    CALL(evt_npc_glide_position, STRING("パタクリ"), LW(0), LW(1), -400, 0, FLOAT(90.0f), FLOAT(30.0f), 1, 0)
    CALL(evt_cam3d_evt_set, -65, 60, -563, -65, 40, -762, 800, EASING_SINE_IN_OUT)
    WAIT_MS(800)
    CALL(evt_msg_print, 0, STRING("kpa_stg7_036"), 0, STRING("ポワン探偵"))
    SET_VALUE(GSW_STORY_PROGRESS, STG7_25_00_1_BOWSER_POSHLEY_SANCTUM)
    CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 1000)
    CALL(evt_fade_entry, 6, 300, 0, 0, 0)
    CALL(evt_fade_end_wait)
    WAIT_MS(2000)
    SET_VALUE(LF(10), 0)
    SET_VALUE(LW(0), 20)
#ifdef VERSION_G8ME01
    CALL_CHILD(evt_memcard_save_wrapper)
#else
    CALL_CHILD(PTR(evt_memcard_save))
#endif
    CALL(evt_mario_set_mode, 0)
    CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 2000)
    CALL(evt_snd_envoff_f, PSND_FADEOUT, 2000)
    CALL(evt_fade_set_mapchange_type, 1, 59, 0, 58, 0)
    CALL(evt_bero_mapchange, STRING("aji_18"), STRING("e_bero"))
    RETURN
EVT_END()

EVT_BEGIN(nise_starstone_init)
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG6_70_THOUSAND_YEAR_DOOR)
        IF_INT_LESS(GSW_STORY_PROGRESS, STG7_25_00_BOWSER_POSHLEY_HEIGHTS)
            CALL(starstone_entry, STRING("nisemono"), 5, 0, 57, -955, 1)
        END_IF
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(pik_02_init_evt)
    IF_INT_LESS(GSW_STORY_PROGRESS, STG6_51_3_SURPRISE_GUESTS)
        SET_VALUE(LW(0), PTR(bero_data))
        CALL(evt_bero_get_info)
        CALL_CHILD(PTR(bero_case_entry))
        CALL_CHILD(PTR(haikei_off))
        ELSE
        SET_VALUE(LW(0), PTR(bero_data))
        CALL(evt_bero_get_info)
        CALL_CHILD(PTR(evt_bero_info_run))
    END_IF
    IF_INT_NOT_EQUAL(GSW_STORY_PROGRESS, STG7_25_00_BOWSER_POSHLEY_HEIGHTS)
        CALL(evt_snd_bgmoff, PSND_FADEOUT)
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG6_PIK2"))
        CALL(evt_snd_set_rev_mode, 2)
    END_IF
    CALL(evt_npc_setup, PTR(npc_data))
    CALL(evt_hit_bind_mapobj, STRING("a_ka_ten"), STRING("ka_ten"))
    IF_INT_LESS(GSW_STORY_PROGRESS, STG6_52_THE_HIDDEN_PIPE)
        CALL(evt_mobj_switch_blue, 0, STRING("sw"), -300, 330, -975, PTR(evt_sw), 0)
        SET_VALUE(LW(0), 1)
        CALL_CHILD(PTR(bero_case_switch_off))
        ELSE
        CALL(evt_mapobj_trans, 1, STRING("ka_ten"), 0, 150, 0)
        CALL(evt_hit_bind_update, STRING("a_ka_ten"))
    END_IF
    CALL(evt_bero_get_entername, LW(0))
    IF_STR_EQUAL(LW(0), STRING("s_bero"))
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("a_kaiga"), 0, PTR(evt_kaiga), GW(10))
    END_IF
    CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("dai_a"), 0, PTR(evt_daiza), 0)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("door_1"), MAP_OBJ_FLAG_CLIP)
    IF_INT_LESS(GSW_STORY_PROGRESS, STG6_51_3_SURPRISE_GUESTS)
        SPAWN_EVENT(PTR(evt_garnet))
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG6_54_WELCOME_BACK)
        BEGIN_INLINE_EVENT
            CALL(evt_bero_exec_wait, 65536)
            CALL(evt_fade_set_mapchange_type, 1, 55, 0, 54, 0)
            SET_VALUE(LW(0), STRING("s_bero"))
            CALL_CHILD(PTR(bero_case_switch_off))
            CALL(evt_bero_overwrite, STRING("s_bero"), PTR(s_bero_peach))
            SET_VALUE(LW(0), STRING("s_bero"))
            CALL_CHILD(PTR(bero_case_switch_on))
        END_INLINE_EVENT
    END_IF
    CALL_CHILD(PTR(nise_starstone_init))
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG7_25_00_BOWSER_POSHLEY_HEIGHTS)
        CALL(evt_mario_set_mode, 2)
        CALL(evt_mario_kill_party, 0)
        SPAWN_EVENT(PTR(pik_02_koopa_evt))
    END_IF
    CALL(evt_map_entry_airport_harbor, 0, STRING("hikou"), STRING("a_hi1"))
#ifdef VERSION_G8ME01
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("s_jav"), MAP_OBJ_FLAG_CLIP)
#endif
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("saku_2"), MAP_OBJ_FLAG_CLIP)
    CALL(evt_cam_letter_box_disable, 0)
    RETURN
EVT_END()

