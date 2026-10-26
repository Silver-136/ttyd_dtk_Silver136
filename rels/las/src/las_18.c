#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "manager/evtmgr_cmd.h"
#include "evt/evt_cam.h"
#include "evt/evt_mario.h"
#include "evt/evt_mobj.h"
#include "evt/evt_msg.h"
#include "evt/evt_case.h"
#include "evt/evt_seq.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_item.h"
#include "evt/evt_map.h"
#include "evt/evt_party.h"
#include "mario/mario_pouch.h"

#include "driver/mobjdrv.h"
#include "driver/animdrv.h"
#include "evt/evt_bero.h"
#include "las.h"

USER_FUNC(get_mobj_seq);
USER_FUNC(switch_alpha_off);
USER_FUNC(switch_set_alpha);

static EvtWord w_door_close[12];
static EvtWord w_door_open[12];
static BeroEntry bero_entry_data[2];
static EvtWord box_evt[30];
EvtWord vivian_check[92];
static EvtWord switch_evt[48];
static EvtWord hint_plate[14];
EvtWord las_18_init_evt[71];

static EVT_BEGIN(w_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("S_door1"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(w_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("S_door1"), 0, LW(0), 0)
    RETURN
EVT_END()

static BeroEntry bero_entry_data[] = {
    {
        "w_bero_1", // hitName
        1, // type
        1, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "las_10", // nextMapName
        "se_bero_2", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        w_door_close, // enterAnimeArg
        w_door_open, // exitAnimeArg
    },
    {0},
};

static EVT_BEGIN(box_evt)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mobj_get_position, STRING("box"), LW(0), LW(1), LW(2))
    CALL(evt_item_entry, STRING("key"), ITEM_PALACE_KEY_0x2D, LW(0), LW(1), LW(2), 17, -1, 0)
    CALL(evt_mobj_wait_animation_end, STRING("box"))
    CALL(evt_item_get_item, STRING("key"))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

USER_FUNC(switch_set_alpha) {
    s32 alpha = evtGetValue(event, event->args[0]);
    MapObjectEntry* mobj = mobjNameToPtrNoAssert("switch");

    if (mobj == NULL) {
        return EVT_RETURN_DONE;
    }
    if (alpha > 0x24) {
        mobj->flags &= ~MOBJ_FLAG_NO_SHADOW;
    } else {
        mobj->flags |= MOBJ_FLAG_NO_SHADOW;
    }
    animPoseSetMaterialEvtColor(mobj->poseId, (GXColor){255, 255, 255, alpha});
    animPoseSetMaterialFlagOn(mobj->poseId, 0x40);
    return EVT_RETURN_DONE;
}

USER_FUNC(switch_alpha_off) {
    MapObjectEntry* mobj = mobjNameToPtrNoAssert("switch");

    if (mobj == NULL) {
        return EVT_RETURN_DONE;
    }
    mobj->flags &= ~MOBJ_FLAG_NO_SHADOW;
    animPoseSetMaterialEvtColor(mobj->poseId, (GXColor){255, 255, 255, 255});
    animPoseSetMaterialFlagOff(mobj->poseId, 0x40);
    return EVT_RETURN_DONE;
}

EVT_BEGIN(vivian_check)
    CALL(evt_seq_wait, 2)
    SET_VALUE(LW(15), 0)
    LOOP(0)
        CALL(get_mobj_seq, STRING("switch"), LW(0))
        IF_INT_GREATER(LW(0), 0)
            BREAK_LOOP
        END_IF
        CALL(evt_mario_get_party, LW(0))
        IF_INT_EQUAL(LW(0), 6)
            CALL(evt_party_get_status, 6, LW(0))
            IF_INT_EQUAL(LW(0), 2)
                ADD(LW(15), 4)
                ELSE
                SUBTRACT(LW(15), 4)
            END_IF
            ELSE
            SUBTRACT(LW(15), 4)
        END_IF
        IF_INT_GREATER(LW(15), 160)
            SET_VALUE(LW(15), 160)
        END_IF
        IF_INT_LESS(LW(15), 0)
            SET_VALUE(LW(15), 0)
        END_IF
        CALL(switch_set_alpha, LW(15))
        IF_INT_EQUAL(LW(15), 160)
            IF_INT_EQUAL(GSWF(4382), 0)
                SET_VALUE(GSWF(4382), 1)
                CALL(evt_snd_bgmoff, 1024)
                CALL(evt_snd_bgmon, 1, STRING("BGM_FF_ENV_SUCCESS1"))
                WAIT_MS(2000)
                CALL(evt_snd_bgmoff, 513)
                CALL(evt_snd_bgmon, 288, 0)
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

USER_FUNC(get_mobj_seq) {
    EvtWord* args = event->args;
    MapObjectEntry* mobj = mobjNameToPtrNoAssert((const char*)evtGetValue(event, args[0]));

    if (mobj == NULL) {
        evtSetValue(event, args[1], 1);
    } else {
        evtSetValue(event, args[1], mobj->state);
    }
    return EVT_RETURN_DONE;
}

static EVT_BEGIN(switch_evt)
    CALL(switch_alpha_off)
    CALL(evt_mario_key_onoff, 0)
    WAIT_MS(1250)
    CALL(evt_cam3d_evt_set, 0, 100, 363, 0, 30, -30, 500, 11)
    WAIT_MS(1000)
    CALL(evt_mobj_set_position, STRING("box"), 0, -30, 0)
    CALL(evt_mobj_flag_onoff, 0, STRING("box"), MOBJ_FLAG_GRAVITY | MOBJ_FLAG_NO_DISP)
    CALL(evt_mobj_hit_onoff, STRING("box"), 1)
    SET_VALUE(LW(0), STRING("box"))
    CALL_CHILD(PTR(box_syutugen))
    CALL(evt_cam3d_evt_off, 500, 11)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(hint_plate)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_msg_print, 0, STRING("stg8_las_43"), 0, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(las_18_init_evt)
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG8_LST2"))
    CALL(evt_snd_envon, 272, STRING("ENV_STG8_LAS1"))
    CALL(evt_snd_set_rev_mode, 2)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_mobj_floatswitch_blue, 0, STRING("switch"), 75, 60, 0, PTR(switch_evt), GSWF(4352))
    CALL(evt_mobj_flag_onoff, 1, STRING("switch"), MOBJ_FLAG_NO_SHADOW)
    IF_INT_EQUAL(GSWF(4352), 0)
        SPAWN_EVENT(PTR(vivian_check))
    END_IF
    CALL(evt_mobj_itembox, STRING("box"), 0, 0, 0, 0, 0, PTR(box_evt), GSWF(4369))
    IF_INT_EQUAL(GSWF(4352), 0)
        CALL(evt_mobj_flag_onoff, 1, STRING("box"), MOBJ_FLAG_NO_DISP)
        CALL(evt_mobj_hit_onoff, STRING("box"), 0)
    END_IF
    CALL(evt_run_case_evt, 9, 1, STRING("A_pure"), 0, PTR(hint_plate), 0)
    RETURN
EVT_END()

