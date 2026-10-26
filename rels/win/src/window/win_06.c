#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "eff/eff_magic1_n64.h"
#include "libc_decls.h"
#include "evt/evt_cam.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_mobj.h"
#include "evt/evt_msg.h"
#include "evt/evt_party.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_sub.h"
#include "evt/evt_telop.h"
#include "driver/mapdrv.h"
#include "system.h"

#include "manager/evtmgr_cmd.h"
#include "driver/camdrv.h"
#include "mario/mariost.h"
#include "memory.h"
#include "string.h"
#include "dolphin/mtx.h"
#include "evt/evt_bero.h"

typedef struct KaitenParam {
    Vec pos;
    Vec target;
    f32 angle;
    f32 dist;
} KaitenParam;
static KaitenParam* ckp0;
static KaitenParam* ckp1;
static KaitenParam* ckpw;

static USER_FUNC(cam_kaiten_get_para);
static USER_FUNC(cam_kaiten_init);
static USER_FUNC(cam_kaiten_set_direct);
static USER_FUNC(cam_kaiten_set_para);
USER_FUNC(camps25);
USER_FUNC(camps30);

static EvtWord first_evt_dokan[18];
static BeroEntry bero_entry_data[3];
EvtWord welcome_wonderforest_init[6];
EvtWord welcome_wonderforest[260];
EvtWord win_06_init_evt[82];

static EVT_BEGIN(first_evt_dokan)
    IF_INT_NOT_EQUAL(GSW_STORY_PROGRESS, 70)
        SET_VALUE(LW(0), 0)
        RETURN
    END_IF
    SET_VALUE(GF(0), 1)
    WAIT_WHILE(GF(0))
    SET_VALUE(LW(0), 0)
    RETURN
EVT_END()

static BeroEntry bero_entry_data[] = {
    {
        "dokan1", // hitName
        2, // type
        0, // sfxId
        9, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        first_evt_dokan, // enterEvent
        6, // caseType
        NULL, // exitEvent
        "tik_03", // nextMapName
        "dokan", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "e_bero", // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "win_00", // nextMapName
        "w_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

EVT_BEGIN(welcome_wonderforest_init)
    CALL(evt_cam_letter_box_onoff, 1, 0)
    RETURN
EVT_END()

EVT_BEGIN(welcome_wonderforest)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_party_cont_onoff, 1, 0)
    CALL(evt_snd_bgm_scope, 0, 1)
    BEGIN_INLINE_EVENT
        WAIT_MS(5000)
        CALL(evt_telop_entry, 4)
    END_INLINE_EVENT
    CALL(camps30)
    CALL(cam_kaiten_init, -350, 16, 276, 0, 75, 0, 255, 16, 366, 0, 75, 0)
    BEGIN_INLINE_EVENT_GET_ID(LW(7))
        LOOP(0)
            CALL(cam_kaiten_set_direct)
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(cam_kaiten_get_para, 0, LF(0), LF(1), LF(2), LF(3), LF(4), LF(5), LW(0), LF(7))
        ADD_FLOAT(LW(0), FLOAT(360.0f))
        CALL(cam_kaiten_get_para, 1, LF(0), LF(1), LF(2), LF(3), LF(4), LF(5), LW(1), LF(7))
        MULTIPLY_FLOAT(LW(0), FLOAT(10.0f))
        MULTIPLY_FLOAT(LW(1), FLOAT(10.0f))
        CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, LW(0), LW(1), 7500)
        LOOP(0)
            CALL(cam_kaiten_get_para, 2, LW(0), LW(1), LW(2), LW(3), LW(4), LW(5), LW(6), LW(7))
            CALL(evt_sub_intpl_msec_get_value_para, LW(6), LW(9))
            DIVIDE_FLOAT(LW(6), FLOAT(10.0f))
            CALL(cam_kaiten_set_para, 2, LW(0), LW(1), LW(2), LW(3), LW(4), LW(5), LW(6), LW(7))
            IF_INT_EQUAL(LW(9), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    WAIT_MS(8000)
    DELETE_EVENT(LW(7))
    CALL(evt_mario_cont_onoff, 1)
    CALL(evt_party_cont_onoff, 0, 1)
    CALL(evt_party_cont_onoff, 1, 1)
    SET_VALUE(GF(0), 0)
    CALL(evt_bero_exec_wait, 65536)
    CALL(evt_party_stop, 0)
    CALL(evt_party_stop, 1)
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        CALL(evt_party_set_dir, 1, 90, 200)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_mario_mov_pos2, 20, 50, 125)
        CALL(evt_mario_set_dir, 270, 200, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_party_move_pos2, 0, -20, 40, 125)
        CALL(evt_party_set_dir, 0, 90, 200)
    END_INLINE_EVENT
    CALL(camps25)
    CALL(evt_cam3d_evt_set, 0, FLOAT(92.5f), FLOAT(336.69922f), 0, FLOAT(32.5f), -8, 0, 0)
    WAIT_MS(1000)
    CALL(evt_msg_print_party, STRING("stg2_win_e01_00_00"))
    WAIT_MS(300)
    CALL(evt_party_get_pos, 1, LW(0), LW(1), LW(2))
    ADD(LW(0), 1)
    CALL(evt_party_move_pos2, 1, LW(0), 85, 125)
    WAIT_MS(200)
    CALL(evt_msg_print, 0, STRING("stg2_win_e00_00"), 0, STRING("extparty"))
    CALL(evt_cam3d_evt_off, 500, 11)
    WAIT_MS(500)
    SET_VALUE(GSW_STORY_PROGRESS, 71)
    CALL(evt_snd_bgm_scope, 0, 0)
    CALL(evt_party_run, 0)
    CALL(evt_party_run, 1)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_cam_letter_box_onoff, 0, 0)
    RETURN
EVT_END()

EVT_BEGIN(win_06_init_evt)
    IF_INT_LESS(GSW_STORY_PROGRESS, 71)
        BEGIN_INLINE_EVENT
            CALL(evt_snd_bgmoff, 512)
            CALL(evt_snd_envon, 272, STRING("ENV_STG2_WIN1"))
            WAIT_MS(9000)
            CALL(evt_snd_bgmon_f, 768, STRING("BGM_STG2_WIN1"), 2000)
            CALL(evt_snd_bgm_scope, 0, 1)
        END_INLINE_EVENT
        ELSE
        CALL(evt_snd_bgmon, 512, STRING("BGM_STG2_WIN1"))
        CALL(evt_snd_envon, 272, STRING("ENV_STG2_WIN1"))
    END_IF
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_tree_mki"), MAP_OBJ_FLAG_CLIP)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_kusa"), MAP_OBJ_FLAG_CLIP)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("group22"), MAP_OBJ_FLAG_CLIP)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_oku_01H081"), MAP_OBJ_FLAG_CLIP)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 70)
        CALL_CHILD(PTR(welcome_wonderforest_init))
        SPAWN_EVENT(PTR(welcome_wonderforest))
    END_IF
    CALL(evt_mobj_save_blk, STRING("mobj_save"), 275, 60, -50, 0, 0)
    RETURN
EVT_END()

static USER_FUNC(cam_kaiten_set_para) {
    EvtWord* args = event->args;
    KaitenParam* p;

    switch (evtGetValue(event, args[0])) {
        case 0:
            p = ckp0;
            break;
        case 1:
            p = ckp1;
            break;
        case 2:
        default:
            p = ckpw;
            break;
    }
    p->pos.x = evtGetFloat(event, args[1]);
    p->pos.y = evtGetFloat(event, args[2]);
    p->pos.z = evtGetFloat(event, args[3]);
    p->target.x = evtGetFloat(event, args[4]);
    p->target.y = evtGetFloat(event, args[5]);
    p->target.z = evtGetFloat(event, args[6]);
    p->angle = evtGetFloat(event, args[7]);
    p->dist = evtGetFloat(event, args[8]);
    return EVT_RETURN_DONE;
}

static USER_FUNC(cam_kaiten_get_para) {
    EvtWord* args = event->args;
    KaitenParam* p;

    switch (evtGetValue(event, args[0])) {
        case 0:
            p = ckp0;
            break;
        case 1:
            p = ckp1;
            break;
        case 2:
        default:
            p = ckpw;
            break;
    }
    evtSetFloat(event, args[1], p->pos.x);
    evtSetFloat(event, args[2], p->pos.y);
    evtSetFloat(event, args[3], p->pos.z);
    evtSetFloat(event, args[4], p->target.x);
    evtSetFloat(event, args[5], p->target.y);
    evtSetFloat(event, args[6], p->target.z);
    evtSetFloat(event, args[7], p->angle);
    evtSetFloat(event, args[8], p->dist);
    return EVT_RETURN_DONE;
}

static USER_FUNC(cam_kaiten_set_direct) {
    CameraEntry* camera = camGetPtr(CAMERA_3D);
    f32 s = sin(6.2831855f * ckpw->angle / 360.0f);
    f32 c = cos(6.2831855f * ckpw->angle / 360.0f);

    ckpw->pos.x = ckpw->dist * s + ckpw->target.x;
    ckpw->pos.z = ckpw->target.z - ckpw->dist * c;
    camera->presetPos = camera->cameraPos;
    camera->presetAt = camera->cameraAt;
    camera->storedPos = ckpw->pos;
    camera->storedAt = ckpw->target;
    camera->startTimeA = gp->renderTime;
    camera->durationA = 0;
    camera->stateA = 3;
    camera->curveA = 11;
    return EVT_RETURN_DONE;
}

static inline f32 fid(f32 x) {
    return x;
}

static USER_FUNC(cam_kaiten_init) {
    EvtWord* args = event->args;
    Vec target;
    Vec pos;
    f32 dist;

    ckp0 = _mapAlloc(sizeof(KaitenParam));
    memset(ckp0, 0, sizeof(KaitenParam));
    ckp1 = _mapAlloc(sizeof(KaitenParam));
    memset(ckp1, 0, sizeof(KaitenParam));
    ckpw = _mapAlloc(sizeof(KaitenParam));
    memset(ckp1, 0, sizeof(KaitenParam));
    ckp0->pos.x = evtGetFloat(event, args[0]);
    ckp0->pos.y = evtGetFloat(event, args[1]);
    ckp0->pos.z = evtGetFloat(event, args[2]);
    ckp0->target.x = evtGetFloat(event, args[3]);
    ckp0->target.y = evtGetFloat(event, args[4]);
    ckp0->target.z = evtGetFloat(event, args[5]);
    ckp1->pos.x = evtGetFloat(event, args[6]);
    ckp1->pos.y = evtGetFloat(event, args[7]);
    ckp1->pos.z = evtGetFloat(event, args[8]);
    ckp1->target.x = evtGetFloat(event, args[9]);
    ckp1->target.y = evtGetFloat(event, args[10]);
    ckp1->target.z = evtGetFloat(event, args[11]);

    pos = ckp0->pos;
    target = ckp0->target;
    target.y = 0.0f;
    pos.y = 0.0f;
    dist = PSVECDistance(&pos, &target);
    ckp0->angle = 57.29579f * (f32)atan2(-(target.x - pos.x), fid(target.z - pos.z));
    ckp0->dist = dist;

    pos = ckp1->pos;
    target = ckp1->target;
    target.y = 0.0f;
    pos.y = 0.0f;
    dist = PSVECDistance(&pos, &target);
    ckp1->angle = 57.29579f * (f32)atan2(-(target.x - pos.x), target.z - pos.z);
    ckp1->dist = dist;

    ckpw->pos = ckp0->pos;
    ckpw->target = ckp0->target;
    ckpw->angle = ckp0->angle;
    ckpw->dist = ckp0->dist;
    return EVT_RETURN_DONE;
}

USER_FUNC(camps25) {
    f32 fovY = 25.0f;

    camGetPtr(CAMERA_3D)->fovY = fovY;
    return EVT_RETURN_DONE;
}

USER_FUNC(camps30) {
    f32 fovY = 30.0f;

    camGetPtr(CAMERA_3D)->fovY = fovY;
    return EVT_RETURN_DONE;
}
