#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "eff/eff_magic1_n64.h"
#include "libc_decls.h"
#include "evt/evt_map.h"
#include "party/party_motion.h"
#include "evt/evt_item.h"
#include "mario/mario_pouch.h"
#include "pmario_sound.h"

#include "mario/mario.h"
#include "driver/hitdrv.h"
#include "driver/mapdrv.h"
#include "system.h"
#include "dolphin/mtx.h"
#include "evt/evt_bero.h"

USER_FUNC(hit_move);

static EvtWord s_close[12];
static EvtWord s_open[12];
static BeroEntry bero_data[2];
static EvtWord evt_key[5];
EvtWord eki_02_init_evt[54];

static EVT_BEGIN(s_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("door"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(s_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("door"), 0, LW(0), 0)
    RETURN
EVT_END()

static BeroEntry bero_data[] = {
    {
        "s_bero", // hitName
        1, // type
        3, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "eki_01", // nextMapName
        "n_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        s_close, // enterAnimeArg
        s_open, // exitAnimeArg
    },
    {0},
};


USER_FUNC(hit_move) {
    MarioWork* mario = marioGetPtr();
    HitEntry* hit = mario->hitobjStandOn;
    Mtx rot;
    Vec dir = {0.0f, 0.0f, -1.0f};
    Vec pos;
    f32 delta;
    f32 angle;
    f32 dist;
    f32 theta;

    if (isFirstCall) {
        event->userdata[0] = 0;
        event->userdata[1] = 0;
    }
    if (hit == NULL) {
        event->userdata[0] = 0;
        event->userdata[1] = 0;
        return EVT_RETURN_BLOCK;
    }
    if (!(hit->flags & 0x30)) {
        event->userdata[0] = 0;
        event->userdata[1] = 0;
        return EVT_RETURN_BLOCK;
    }
    mtxGetRotationElement(hit->worldMtx, rot, 'x', 'z');
    PSMTXMultVec(rot, &dir, &dir);
    angle = angleABf(0.0f, 0.0f, dir.x, dir.z);
    delta = angle - event->userdataf[1];
    if (event->userdata[0] == 0) {
        event->userdata[0] = 1;
        event->userdataf[1] = angle;
        return EVT_RETURN_BLOCK;
    }
    hitObjGetPos(hit->joint->name, &pos);
    dist = distABf(mario->position.x, mario->position.z, pos.x, pos.z);
    theta = angleABf(pos.x, pos.z, mario->position.x, mario->position.z);
    theta = 6.2831855f * (theta + delta) / 360.0f;
    mario->floorMove.x = pos.x + dist * (f32)sin(theta) - mario->position.x;
    mario->floorMove.z = pos.z - dist * (f32)cos(theta) - mario->position.z;
    event->userdataf[1] = angle;
    return EVT_RETURN_BLOCK;
}

static EVT_BEGIN(evt_key)
    SET_VALUE(GSW_STORY_PROGRESS, 314)
    RETURN
EVT_END()

EVT_BEGIN(eki_02_init_evt)
    SET_VALUE(LW(0), PTR(bero_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_snd_bgmoff, PSND_FADEOUT)
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG6_EKI3"))
#ifdef VERSION_G8ME01
    CALL(evt_map_playanim, STRING("anm_sikake_1"), 1, 1)
    CALL(evt_map_playanim, STRING("anm_sikake_2"), 1, 1)
#else
    CALL(evt_map_playanim, STRING("anm_sikake_1"), 1, 0)
    CALL(evt_map_playanim, STRING("anm_sikake_2"), 1, 0)
#endif
    IF_INT_LESS(GSW_STORY_PROGRESS, 314)
        CALL(evt_item_entry, STRING("key"), ITEM_STEEPLE_KEY_0x19, 525, 220, 29, 16, -1, PTR(evt_key))
    END_IF
    BEGIN_INLINE_EVENT
        CALL(hit_move)
    END_INLINE_EVENT
    CALL(evt_item_entry, STRING("item"), ITEM_STAR_PIECE, 540, 140, -40, 16, GSWF(5653), 0)
    RETURN
EVT_END()

