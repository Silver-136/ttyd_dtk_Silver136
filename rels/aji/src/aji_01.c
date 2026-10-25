#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "manager/evtmgr_cmd.h"
#include "evt/evt_cam.h"
#include "evt/evt_eff.h"
#include "evt/evt_case.h"
#include "evt/evt_fade.h"
#include "evt/evt_hit.h"
#include "evt/evt_img.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_msg.h"
#include "evt/evt_paper.h"
#include "evt/evt_party.h"
#include "evt/evt_seq.h"
#include "evt/evt_snd.h"
#include "party/party_motion.h"
#include "evt/evt_sub.h"
#include "evt/evt_pouch.h"
#include "driver/npcdrv.h"

#include "driver/msgdrv.h"
#include "mario/mario.h"
#include "evt/evt_bero.h"
#include "evt/evt_npc.h"
#include "aji.h"
#include "mario/mario_pouch.h"

USER_FUNC(evt_npc_set_stay_anim);
USER_FUNC(evt_win_item_select);
USER_FUNC(evt_win_nameent_name);
USER_FUNC(evt_win_nameent_off);
USER_FUNC(evt_win_nameent_on);
USER_FUNC(evt_win_nameent_wait);
USER_FUNC(get_password);
static USER_FUNC(peach_evt_toumei);
static USER_FUNC(peach_set_pose);

static EvtWord w1_door_open[22];
static EvtWord w1_door_close[22];
static EvtWord e1_door_open[22];
static EvtWord e1_door_close[22];
static EvtWord w2_door_open[22];
static EvtWord w2_door_close[22];
static EvtWord e2_door_open[22];
static EvtWord e2_door_close[22];
static EvtWord n1_door_open[22];
static EvtWord n1_door_close[22];
static EvtWord n2_door_open[22];
static EvtWord n2_door_close[22];
EvtWord n3_door_open[22];
EvtWord n3_door_close[22];
EvtWord n4_door_open[22];
EvtWord n4_door_close[22];
EvtWord w3_door_open[22];
EvtWord w3_door_close[22];
static EvtWord e3_door_open[22];
static EvtWord e3_door_close[22];
EvtWord w4_door_open[22];
EvtWord w4_door_close[22];
EvtWord e4_door_open[22];
EvtWord e4_door_close[22];
EvtWord w5_door_open[22];
EvtWord w5_door_close[22];
EvtWord e5_door_open[22];
EvtWord e5_door_close[22];
static BeroEntry bero_entry_data[15];
BeroEntry e_bero_5_peach[2];
s32 elv_key1[2];
EvtWord elv_unlock1[131];
s32 elv_key2[2];
EvtWord elv_unlock2[131];
EvtWord ele1_door_open[139];
EvtWord ele1_door_close[139];
#ifdef VERSION_G8ME01
EvtWord ele_1[607];
#else
EvtWord ele_1[595];
#endif
EvtWord ele2_door_open[139];
EvtWord ele2_door_close[139];
#ifdef VERSION_G8ME01
EvtWord ele_2[634];
#else
EvtWord ele_2[622];
#endif
static EvtWord evt_elv_init[271];
EvtWord key_lock[14];
EvtWord last_key_lock[36];
EvtWord last_elv_lock[36];
EvtWord evt_key_init[242];
EvtWord evt_aikotoba[188];
static EvtWord peach_evt_stg1[32];
static EvtWord peach_evt_stg1_1[41];
EvtWord peach_evt_stg1_2[106];
static EvtWord peach_evt_stg3_1[64];
static EvtWord peach_evt_stg3[25];
static EvtWord peach_evt_stg5[51];
static EvtWord peach_evt_stg5_1[46];
static EvtWord peach_evt_door_stop[14];
EvtWord peach_evt_ele_2[505];
EvtWord peach_evt_ele_init[177];
EvtWord peach_evt_key_init[178];
EvtWord peach_evt_init[365];
static EvtWord teshita1_init[42];
EvtWord teshita1_talk[14];
static EvtWord teshita2_init[51];
EvtWord teshita2_talk[14];
EvtWord teshita2_regl[35];
static EvtWord teshita3_init[46];
EvtWord teshita3_talk[14];
EvtWord teshita3_regl[35];
static NpcSetupInfo npcEnt[4];
EvtWord aji_01_init_evt[171];

static EVT_BEGIN(w1_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.25f))
    CALL(evt_mapobj_trans, 1, STRING("S_door1_1"), 0, 0, LW(0))
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_trans, 1, STRING("S_door1_2"), 0, 0, LW(0))
    RETURN
EVT_END()

static EVT_BEGIN(w1_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.25f))
    CALL(evt_mapobj_trans, 1, STRING("S_door1_1"), 0, 0, LW(0))
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_trans, 1, STRING("S_door1_2"), 0, 0, LW(0))
    RETURN
EVT_END()

static EVT_BEGIN(e1_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.25f))
    CALL(evt_mapobj_trans, 1, STRING("S_door2_2"), 0, 0, LW(0))
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_trans, 1, STRING("S_door2_1"), 0, 0, LW(0))
    RETURN
EVT_END()

static EVT_BEGIN(e1_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.25f))
    CALL(evt_mapobj_trans, 1, STRING("S_door2_2"), 0, 0, LW(0))
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_trans, 1, STRING("S_door2_1"), 0, 0, LW(0))
    RETURN
EVT_END()

static EVT_BEGIN(w2_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.25f))
    CALL(evt_mapobj_trans, 1, STRING("S_door3_1"), 0, 0, LW(0))
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_trans, 1, STRING("S_door3_2"), 0, 0, LW(0))
    RETURN
EVT_END()

static EVT_BEGIN(w2_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.25f))
    CALL(evt_mapobj_trans, 1, STRING("S_door3_1"), 0, 0, LW(0))
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_trans, 1, STRING("S_door3_2"), 0, 0, LW(0))
    RETURN
EVT_END()

static EVT_BEGIN(e2_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.25f))
    CALL(evt_mapobj_trans, 1, STRING("S_door4_2"), 0, 0, LW(0))
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_trans, 1, STRING("S_door4_1"), 0, 0, LW(0))
    RETURN
EVT_END()

static EVT_BEGIN(e2_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.25f))
    CALL(evt_mapobj_trans, 1, STRING("S_door4_2"), 0, 0, LW(0))
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_trans, 1, STRING("S_door4_1"), 0, 0, LW(0))
    RETURN
EVT_END()

static EVT_BEGIN(n1_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.16601563f))
    CALL(evt_mapobj_trans, 1, STRING("S_door6_2"), LW(0), 0, 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_trans, 1, STRING("S_door6_1"), LW(0), 0, 0)
    RETURN
EVT_END()

static EVT_BEGIN(n1_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.16601563f))
    CALL(evt_mapobj_trans, 1, STRING("S_door6_2"), LW(0), 0, 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_trans, 1, STRING("S_door6_1"), LW(0), 0, 0)
    RETURN
EVT_END()

static EVT_BEGIN(n2_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.16601563f))
    CALL(evt_mapobj_trans, 1, STRING("S_door7_2"), LW(0), 0, 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_trans, 1, STRING("S_door7_1"), LW(0), 0, 0)
    RETURN
EVT_END()

static EVT_BEGIN(n2_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.16601563f))
    CALL(evt_mapobj_trans, 1, STRING("S_door7_2"), LW(0), 0, 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_trans, 1, STRING("S_door7_1"), LW(0), 0, 0)
    RETURN
EVT_END()

EVT_BEGIN(n3_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.16601563f))
    CALL(evt_mapobj_trans, 1, STRING("S_door8_2"), LW(0), 0, 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_trans, 1, STRING("S_door8_1"), LW(0), 0, 0)
    RETURN
EVT_END()

EVT_BEGIN(n3_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.16601563f))
    CALL(evt_mapobj_trans, 1, STRING("S_door8_2"), LW(0), 0, 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_trans, 1, STRING("S_door8_1"), LW(0), 0, 0)
    RETURN
EVT_END()

EVT_BEGIN(n4_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.16601563f))
    CALL(evt_mapobj_trans, 1, STRING("S_door9_2"), LW(0), 0, 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_trans, 1, STRING("S_door9_1"), LW(0), 0, 0)
    RETURN
EVT_END()

EVT_BEGIN(n4_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.16601563f))
    CALL(evt_mapobj_trans, 1, STRING("S_door9_2"), LW(0), 0, 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_trans, 1, STRING("S_door9_1"), LW(0), 0, 0)
    RETURN
EVT_END()

EVT_BEGIN(w3_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.25f))
    CALL(evt_mapobj_trans, 1, STRING("S_door5_1"), 0, 0, LW(0))
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_trans, 1, STRING("S_door5_2"), 0, 0, LW(0))
    RETURN
EVT_END()

EVT_BEGIN(w3_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.25f))
    CALL(evt_mapobj_trans, 1, STRING("S_door5_1"), 0, 0, LW(0))
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_trans, 1, STRING("S_door5_2"), 0, 0, LW(0))
    RETURN
EVT_END()

static EVT_BEGIN(e3_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.25f))
    CALL(evt_mapobj_trans, 1, STRING("S_door10_2"), 0, 0, LW(0))
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_trans, 1, STRING("S_door10_1"), 0, 0, LW(0))
    RETURN
EVT_END()

static EVT_BEGIN(e3_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.25f))
    CALL(evt_mapobj_trans, 1, STRING("S_door10_2"), 0, 0, LW(0))
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_trans, 1, STRING("S_door10_1"), 0, 0, LW(0))
    RETURN
EVT_END()

EVT_BEGIN(w4_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.25f))
    CALL(evt_mapobj_trans, 1, STRING("S_door11_1"), 0, 0, LW(0))
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_trans, 1, STRING("S_door11_2"), 0, 0, LW(0))
    RETURN
EVT_END()

EVT_BEGIN(w4_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.25f))
    CALL(evt_mapobj_trans, 1, STRING("S_door11_1"), 0, 0, LW(0))
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_trans, 1, STRING("S_door11_2"), 0, 0, LW(0))
    RETURN
EVT_END()

EVT_BEGIN(e4_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.25f))
    CALL(evt_mapobj_trans, 1, STRING("S_door12_2"), 0, 0, LW(0))
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_trans, 1, STRING("S_door12_1"), 0, 0, LW(0))
    RETURN
EVT_END()

EVT_BEGIN(e4_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.25f))
    CALL(evt_mapobj_trans, 1, STRING("S_door12_2"), 0, 0, LW(0))
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_trans, 1, STRING("S_door12_1"), 0, 0, LW(0))
    RETURN
EVT_END()

EVT_BEGIN(w5_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.25f))
    CALL(evt_mapobj_trans, 1, STRING("S_door13_1"), 0, 0, LW(0))
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_trans, 1, STRING("S_door13_2"), 0, 0, LW(0))
    RETURN
EVT_END()

EVT_BEGIN(w5_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.25f))
    CALL(evt_mapobj_trans, 1, STRING("S_door13_1"), 0, 0, LW(0))
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_trans, 1, STRING("S_door13_2"), 0, 0, LW(0))
    RETURN
EVT_END()

EVT_BEGIN(e5_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.25f))
    CALL(evt_mapobj_trans, 1, STRING("S_door14_2"), 0, 0, LW(0))
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_trans, 1, STRING("S_door14_1"), 0, 0, LW(0))
    RETURN
EVT_END()

EVT_BEGIN(e5_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.25f))
    CALL(evt_mapobj_trans, 1, STRING("S_door14_2"), 0, 0, LW(0))
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_trans, 1, STRING("S_door14_1"), 0, 0, LW(0))
    RETURN
EVT_END()

static BeroEntry bero_entry_data[] = {
    {
        "w_bero_1", // hitName
        1, // type
        10, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "aji_00", // nextMapName
        "e_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        w1_door_close, // enterAnimeArg
        w1_door_open, // exitAnimeArg
    },
    {
        "e_bero_1", // hitName
        1, // type
        10, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "aji_02", // nextMapName
        "w_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        e1_door_close, // enterAnimeArg
        e1_door_open, // exitAnimeArg
    },
    {
        "w_bero_2", // hitName
        1, // type
        10, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "aji_03", // nextMapName
        "e_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        w2_door_close, // enterAnimeArg
        w2_door_open, // exitAnimeArg
    },
    {
        "e_bero_2", // hitName
        1, // type
        10, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "aji_04", // nextMapName
        "w_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        e2_door_close, // enterAnimeArg
        e2_door_open, // exitAnimeArg
    },
    {
        "n_bero_1", // hitName
        1, // type
        10, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "aji_06", // nextMapName
        "s_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        n1_door_close, // enterAnimeArg
        n1_door_open, // exitAnimeArg
    },
    {
        "n_bero_2", // hitName
        1, // type
        10, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "aji_07", // nextMapName
        "s_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        n2_door_close, // enterAnimeArg
        n2_door_open, // exitAnimeArg
    },
    {
        "n_bero_3", // hitName
        1, // type
        10, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "aji_08", // nextMapName
        "s_bero_1", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        n3_door_close, // enterAnimeArg
        n3_door_open, // exitAnimeArg
    },
    {
        "n_bero_4", // hitName
        1, // type
        10, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "aji_09", // nextMapName
        "s_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        n4_door_close, // enterAnimeArg
        n4_door_open, // exitAnimeArg
    },
    {
        "w_bero_3", // hitName
        1, // type
        10, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "aji_05", // nextMapName
        "e_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        w3_door_close, // enterAnimeArg
        w3_door_open, // exitAnimeArg
    },
    {
        "e_bero3", // hitName
        1, // type
        10, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "aji_10", // nextMapName
        "w_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        e3_door_close, // enterAnimeArg
        e3_door_open, // exitAnimeArg
    },
    {
        "w_bero_4", // hitName
        1, // type
        10, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "aji_12", // nextMapName
        "e_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        w4_door_close, // enterAnimeArg
        w4_door_open, // exitAnimeArg
    },
    {
        "e_bero_4", // hitName
        1, // type
        10, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "aji_18", // nextMapName
        "w_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        e4_door_close, // enterAnimeArg
        e4_door_open, // exitAnimeArg
    },
    {
        "w_bero_5", // hitName
        1, // type
        10, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "aji_16", // nextMapName
        "e_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        w5_door_close, // enterAnimeArg
        w5_door_open, // exitAnimeArg
    },
    {
        "e_bero_5", // hitName
        1, // type
        10, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "aji_17", // nextMapName
        "w_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        e5_door_close, // enterAnimeArg
        e5_door_open, // exitAnimeArg
    },
    {0},
};

BeroEntry e_bero_5_peach[] = {
    {
        "e_bero_5", // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        8, // caseType
        NULL, // exitEvent
        "aji_17", // nextMapName
        "w_bero", // nextBeroName
        0, 0, 0, 0, // enterAnimeType..exitAnimeArg
    },
    {0},
};

s32 elv_key1[] = {
    ITEM_ELEVATOR_KEY_0x1B, -1,
};

EVT_BEGIN(elv_unlock1)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_msg_print, 0, STRING("stg7_aji_65"), 0, 0)
    CALL(evt_pouch_check_item, 27, LW(0))
    IF_INT_LESS(LW(0), 1)
        CALL(evt_mario_key_onoff, 1)
        RETURN
    END_IF
    CALL(evt_win_item_select, 0, PTR(elv_key1), LW(0), 0)
    IF_INT_NOT_EQUAL(LW(0), -1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ekagi1_aka"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ekagi2_aka"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ekagi3_aka"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ekagi1_mid"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ekagi2_mid"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ekagi3_mid"), 1)
        CALL(evt_pouch_remove_item, LW(0), LW(1))
        SET_VALUE(LW(0), 0)
        CALL(evt_run_case_evt, 9, 1, STRING("A_ekagi1"), 0, PTR(ele_1), 0)
        SET_VALUE(LW(0), 1)
        CALL(evt_run_case_evt, 9, 1, STRING("A_ekagi2"), 0, PTR(ele_1), 0)
        SET_VALUE(LW(0), 2)
        CALL(evt_run_case_evt, 9, 1, STRING("A_ekagi3"), 0, PTR(ele_1), 0)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG7_ELEVATOR_KEY1"), LW(0), LW(1), LW(2), 0)
        SET_VALUE(GSW_STORY_PROGRESS, 363)
        CALL(evt_mario_key_onoff, 1)
        CALL(evt_exit_case_evt)
        RETURN
    END_IF
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

s32 elv_key2[] = {
    ITEM_ELEVATOR_KEY_0x1C, -1,
};

EVT_BEGIN(elv_unlock2)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_msg_print, 0, STRING("stg7_aji_65"), 0, 0)
    CALL(evt_pouch_check_item, 28, LW(0))
    IF_INT_LESS(LW(0), 1)
        CALL(evt_mario_key_onoff, 1)
        RETURN
    END_IF
    CALL(evt_win_item_select, 0, PTR(elv_key2), LW(0), 0)
    IF_INT_NOT_EQUAL(LW(0), -1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ekagi4_aka"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ekagi5_aka"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ekagi6_aka"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ekagi4_mid"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ekagi5_mid"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ekagi6_mid"), 1)
        CALL(evt_pouch_remove_item, LW(0), LW(1))
        SET_VALUE(LW(0), 2)
        CALL(evt_run_case_evt, 9, 1, STRING("A_ekagi4"), 0, PTR(ele_2), 0)
        SET_VALUE(LW(0), 3)
        CALL(evt_run_case_evt, 9, 1, STRING("A_ekagi5"), 0, PTR(ele_2), 0)
        SET_VALUE(LW(0), 4)
        CALL(evt_run_case_evt, 9, 1, STRING("A_ekagi6"), 0, PTR(ele_2), 0)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG7_ELEVATOR_KEY1"), LW(0), LW(1), LW(2), 0)
        SET_VALUE(GSW_STORY_PROGRESS, 368)
        CALL(evt_mario_key_onoff, 1)
        CALL(evt_exit_case_evt)
        RETURN
    END_IF
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(ele1_door_open)
    SWITCH(LW(0))
        CASE_EQUAL(0)
        SET_VALUE(LW(6), STRING("e1_1_door1"))
        SET_VALUE(LW(7), STRING("e1_1_door2"))
        CASE_EQUAL(1)
        SET_VALUE(LW(6), STRING("e1_2_door1"))
        SET_VALUE(LW(7), STRING("e1_2_door2"))
        CASE_EQUAL(2)
        SET_VALUE(LW(6), STRING("e1_3_door1"))
        SET_VALUE(LW(7), STRING("e1_3_door2"))
    END_SWITCH
    BEGIN_INLINE_EVENT
        CALL(evt_mapobj_get_position, LW(6), LW(10), LW(11), LW(12))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG7_ELEVATOR_OPEN1"), LW(10), LW(11), LW(12), 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, 11, 0, -9000, 500)
        LABEL(1)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
        CALL(evt_mapobj_rotate, 1, LW(6), 0, LW(0), 0)
        MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
        CALL(evt_mapobj_rotate, 1, LW(7), 0, LW(0), 0)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(1)
        END_IF
    END_INLINE_EVENT
    WAIT_MS(300)
    BEGIN_INLINE_EVENT
        CALL(evt_mapobj_get_position, STRING("ee1_door1"), LW(10), LW(11), LW(12))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG7_ELEVATOR_OPEN2"), LW(10), LW(11), LW(12), 0)
    END_INLINE_EVENT
    CALL(evt_sub_intpl_msec_init, 11, 0, -9000, 500)
    LABEL(2)
    CALL(evt_sub_intpl_msec_get_value)
    DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
    CALL(evt_mapobj_rotate, 1, STRING("ee1_door1"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 1, STRING("ee1_door2"), 0, LW(0), 0)
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(2)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(ele1_door_close)
    SWITCH(LW(0))
        CASE_EQUAL(0)
        SET_VALUE(LW(6), STRING("e1_1_door1"))
        SET_VALUE(LW(7), STRING("e1_1_door2"))
        CASE_EQUAL(1)
        SET_VALUE(LW(6), STRING("e1_2_door1"))
        SET_VALUE(LW(7), STRING("e1_2_door2"))
        CASE_EQUAL(2)
        SET_VALUE(LW(6), STRING("e1_3_door1"))
        SET_VALUE(LW(7), STRING("e1_3_door2"))
    END_SWITCH
    BEGIN_INLINE_EVENT
        CALL(evt_mapobj_get_position, STRING("ee1_door1"), LW(10), LW(11), LW(12))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG7_ELEVATOR_CLOSE2"), LW(10), LW(11), LW(12), 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, 11, -9000, 0, 500)
        LABEL(3)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
        CALL(evt_mapobj_rotate, 1, STRING("ee1_door1"), 0, LW(0), 0)
        MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
        CALL(evt_mapobj_rotate, 1, STRING("ee1_door2"), 0, LW(0), 0)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(3)
        END_IF
    END_INLINE_EVENT
    WAIT_MS(300)
    BEGIN_INLINE_EVENT
        CALL(evt_mapobj_get_position, LW(6), LW(10), LW(11), LW(12))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG7_ELEVATOR_CLOSE1"), LW(10), LW(11), LW(12), 0)
    END_INLINE_EVENT
    CALL(evt_sub_intpl_msec_init, 11, -9000, 0, 500)
    LABEL(4)
    CALL(evt_sub_intpl_msec_get_value)
    DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
    CALL(evt_mapobj_rotate, 1, LW(6), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 1, LW(7), 0, LW(0), 0)
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(4)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(ele_1)
    SET_VALUE(LW(5), LW(0))
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    BEGIN_INLINE_EVENT_GET_ID(LW(10))
        CALL(evt_mario_get_party, LW(0))
        SWITCH(LW(0))
            CASE_EQUAL(2)
            LOOP(0)
                CALL(evt_party_nokotaro_get_status, 0, LW(0))
                IF_INT_EQUAL(LW(0), 0)
                    BREAK_LOOP
                END_IF
                WAIT_FRAMES(1)
            END_LOOP
            CASE_EQUAL(3)
            LOOP(0)
                CALL(evt_party_get_status, 3, LW(0))
                IF_INT_EQUAL(LW(0), 0)
                    BREAK_LOOP
                END_IF
                WAIT_FRAMES(1)
            END_LOOP
        END_SWITCH
        CALL(evt_party_stop, 0)
        CALL(evt_party_move_pos2, 0, -530, 0, FLOAT(80.0f))
        CALL(evt_party_set_dir, 0, 270, 300)
    END_INLINE_EVENT
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG7_ELEVATOR_ACCESS1"), LW(0), LW(1), LW(2), 0)
    BEGIN_INLINE_EVENT
        CALL(evt_cam3d_evt_set_now)
        CALL(evt_cam_road_reset)
        CALL(evt_cam_shift_reset)
        CALL(evt_cam3d_event_from_road, -545, LW(1), -120, 0, LW(0), LW(1), LW(2), LW(3), LW(4), LW(5))
        CALL(evt_cam3d_evt_set, LW(0), LW(1), LW(2), LW(3), LW(4), LW(5), 500, 11)
    END_INLINE_EVENT
    CALL(evt_mario_mov_pos2, -560, 0, FLOAT(80.0f))
    CALL(evt_mario_set_dir, 90, 200, 0)
    IF_INT_NOT_EQUAL(GW(0), LW(5))
        SWITCH(GW(0))
            CASE_EQUAL(0)
            SET_VALUE(LW(0), 2000)
            CASE_EQUAL(1)
            SET_VALUE(LW(0), 1000)
            CASE_EQUAL(2)
            SET_VALUE(LW(0), 0)
        END_SWITCH
        SWITCH(LW(5))
            CASE_EQUAL(0)
            SET_VALUE(LW(1), 2000)
            CASE_EQUAL(1)
            SET_VALUE(LW(1), 1000)
            CASE_EQUAL(2)
            SET_VALUE(LW(1), 0)
        END_SWITCH
        SET_VALUE(LW(2), LW(0))
        SUBTRACT(LW(2), LW(1))
        IF_INT_LESS(LW(2), 0)
            MULTIPLY(LW(2), -1)
        END_IF
        BEGIN_INLINE_EVENT
            SUBTRACT(LW(2), 200)
            WAIT_MS(LW(2))
            CALL(evt_mapobj_get_position, STRING("ere1_m"), LW(10), LW(11), LW(12))
            CALL(evt_snd_sfxon_3d, STRING("SFX_STG7_ELEVATOR_APPEAR1"), LW(10), LW(11), LW(12), 0)
        END_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, 11, LW(0), LW(1), LW(2))
        LABEL(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_trans, 1, STRING("ere1_m"), 0, LW(0), 0)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(0)
        END_IF
        CALL(evt_hit_bind_update, STRING("A_ere1_m"))
    END_IF
    SET_VALUE(LW(0), LW(5))
    CALL_CHILD(PTR(ele1_door_open))
    CALL(evt_mario_mov_pos2, -560, -120, FLOAT(120.0f))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    ADD(LW(0), 30)
    LOOP(0)
        IS_EVENT_RUNNING(LW(10), LW(11))
        IF_INT_EQUAL(LW(11), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_party_stop, 0)
    CALL(evt_party_jump_pos, 0, LW(0), LW(1), LW(2), 400, 0, -1000)
    CALL(evt_mario_set_dir, 90, 200, 0)
    CALL(evt_party_set_dir, 0, 270, 300)
    CALL(evt_msg_print, 0, STRING("stg7_aji_66"), 0, 0)
    SWITCH(LW(5))
        CASE_EQUAL(0)
        CALL(evt_msg_select, 0, STRING("stg7_aji_ele_1_00"))
        CALL(evt_msg_continue)
        SWITCH(LW(0))
            CASE_EQUAL(0)
            SET_VALUE(LW(0), 1)
            CASE_EQUAL(1)
            SET_VALUE(LW(0), 2)
            CASE_DEFAULT
            SET_VALUE(LW(0), -1)
        END_SWITCH
        CASE_EQUAL(1)
        CALL(evt_msg_select, 0, STRING("stg7_aji_ele_1_01"))
        CALL(evt_msg_continue)
        SWITCH(LW(0))
            CASE_EQUAL(0)
            SET_VALUE(LW(0), 0)
            CASE_EQUAL(1)
            SET_VALUE(LW(0), 2)
            CASE_DEFAULT
            SET_VALUE(LW(0), -1)
        END_SWITCH
        CASE_EQUAL(2)
        CALL(evt_msg_select, 0, STRING("stg7_aji_ele_1_02"))
        CALL(evt_msg_continue)
        SWITCH(LW(0))
            CASE_EQUAL(0)
            SET_VALUE(LW(0), 0)
            CASE_EQUAL(1)
            SET_VALUE(LW(0), 1)
            CASE_DEFAULT
            SET_VALUE(LW(0), -1)
        END_SWITCH
    END_SWITCH
    SET_VALUE(LW(8), LW(0))
    CALL(evt_cam3d_evt_off, 500, 11)
    IF_INT_EQUAL(LW(0), -1)
        CALL(evt_mario_mov_pos2, -560, 0, FLOAT(120.0f))
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        ADD(LW(0), 30)
        CALL(evt_party_jump_pos, 0, LW(0), LW(1), LW(2), 400, 0, -1000)
        SET_VALUE(LW(0), LW(5))
        CALL_CHILD(PTR(ele1_door_close))
        SET_VALUE(GW(0), LW(5))
        CALL(N_evt_bero_set_mario_bottomless_respawn_pos_to_current_mario_pos)
        CALL(evt_party_run, 0)
        CALL(evt_mario_key_onoff, 1)
        RETURN
    END_IF
    SET_VALUE(LW(0), LW(5))
    CALL_CHILD(PTR(ele1_door_close))
    SWITCH(LW(5))
        CASE_EQUAL(0)
        SET_VALUE(LW(0), 2000)
        CASE_EQUAL(1)
        SET_VALUE(LW(0), 1000)
        CASE_EQUAL(2)
        SET_VALUE(LW(0), 0)
    END_SWITCH
    SWITCH(LW(8))
        CASE_EQUAL(0)
        SET_VALUE(LW(1), 2000)
        CASE_EQUAL(1)
        SET_VALUE(LW(1), 1000)
        CASE_EQUAL(2)
        SET_VALUE(LW(1), 0)
    END_SWITCH
    SET_VALUE(LW(2), LW(0))
    SUBTRACT(LW(2), LW(1))
    IF_INT_LESS(LW(2), 0)
        MULTIPLY(LW(2), -1)
    END_IF
    MULTIPLY(LW(2), 4)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_mapobj_get_position, STRING("ere1_m"), LW(10), LW(11), LW(12))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG7_ELEVATOR_MOVE1"), LW(10), LW(11), LW(12), LW(9))
    CALL(evt_sub_intpl_msec_init, 11, LW(0), LW(1), LW(2))
    LABEL(5)
    CALL(evt_sub_intpl_msec_get_value)
    CALL(evt_mapobj_trans, 1, STRING("ere1_m"), 0, LW(0), 0)
    CALL(evt_hit_bind_update, STRING("A_ere1_m"))
#ifdef VERSION_G8ME01
    CALL(evt_mapobj_get_position, STRING("ere1_m"), LW(2), LW(3), LW(4))
    CALL(evt_snd_sfx_pos, LW(9), LW(2), LW(3), LW(4))
#endif
    ADD(LW(0), 1000)
    CALL(evt_mario_get_pos, 0, LW(2), LW(3), LW(4))
    CALL(evt_mario_set_pos, LW(2), LW(0), LW(4))
    CALL(evt_party_get_pos, 0, LW(2), LW(3), LW(4))
    CALL(evt_party_set_pos, 0, LW(2), LW(0), LW(4))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(5)
    END_IF
    CALL(evt_snd_sfxoff, LW(9))
    CALL(evt_mario_cont_onoff, 1)
    CALL(evt_party_cont_onoff, 0, 1)
    SET_VALUE(LW(0), LW(8))
    CALL_CHILD(PTR(ele1_door_open))
    CALL(evt_mario_mov_pos2, -560, 0, FLOAT(120.0f))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    ADD(LW(0), 30)
    CALL(evt_party_jump_pos, 0, LW(0), LW(1), LW(2), 400, 0, -1000)
    BEGIN_INLINE_EVENT
        SET_VALUE(LW(0), LW(8))
        CALL_CHILD(PTR(ele1_door_close))
    END_INLINE_EVENT
    SET_VALUE(GW(0), LW(8))
    CALL(N_evt_bero_set_mario_bottomless_respawn_pos_to_current_mario_pos)
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(ele2_door_open)
    SWITCH(LW(0))
        CASE_EQUAL(2)
        SET_VALUE(LW(6), STRING("e2_4_door1"))
        SET_VALUE(LW(7), STRING("e2_4_door2"))
        CASE_EQUAL(3)
        SET_VALUE(LW(6), STRING("e2_5_door1"))
        SET_VALUE(LW(7), STRING("e2_5_door2"))
        CASE_EQUAL(4)
        SET_VALUE(LW(6), STRING("e2_6_door1"))
        SET_VALUE(LW(7), STRING("e2_6_door2"))
    END_SWITCH
    BEGIN_INLINE_EVENT
        CALL(evt_mapobj_get_position, LW(6), LW(10), LW(11), LW(12))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG7_ELEVATOR_OPEN1"), LW(10), LW(11), LW(12), 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, 11, 0, -9000, 500)
        LABEL(1)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
        CALL(evt_mapobj_rotate, 1, LW(6), 0, LW(0), 0)
        MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
        CALL(evt_mapobj_rotate, 1, LW(7), 0, LW(0), 0)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(1)
        END_IF
    END_INLINE_EVENT
    WAIT_MS(300)
    BEGIN_INLINE_EVENT
        CALL(evt_mapobj_get_position, STRING("ee2_door1"), LW(10), LW(11), LW(12))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG7_ELEVATOR_OPEN2"), LW(10), LW(11), LW(12), 0)
    END_INLINE_EVENT
    CALL(evt_sub_intpl_msec_init, 11, 0, -9000, 500)
    LABEL(2)
    CALL(evt_sub_intpl_msec_get_value)
    DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
    CALL(evt_mapobj_rotate, 1, STRING("ee2_door1"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 1, STRING("ee2_door2"), 0, LW(0), 0)
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(2)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(ele2_door_close)
    SWITCH(LW(0))
        CASE_EQUAL(2)
        SET_VALUE(LW(6), STRING("e2_4_door1"))
        SET_VALUE(LW(7), STRING("e2_4_door2"))
        CASE_EQUAL(3)
        SET_VALUE(LW(6), STRING("e2_5_door1"))
        SET_VALUE(LW(7), STRING("e2_5_door2"))
        CASE_EQUAL(4)
        SET_VALUE(LW(6), STRING("e2_6_door1"))
        SET_VALUE(LW(7), STRING("e2_6_door2"))
    END_SWITCH
    BEGIN_INLINE_EVENT
        CALL(evt_mapobj_get_position, STRING("ee2_door1"), LW(10), LW(11), LW(12))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG7_ELEVATOR_CLOSE2"), LW(10), LW(11), LW(12), 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, 11, -9000, 0, 500)
        LABEL(3)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
        CALL(evt_mapobj_rotate, 1, STRING("ee2_door1"), 0, LW(0), 0)
        MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
        CALL(evt_mapobj_rotate, 1, STRING("ee2_door2"), 0, LW(0), 0)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(3)
        END_IF
    END_INLINE_EVENT
    WAIT_MS(300)
    BEGIN_INLINE_EVENT
        CALL(evt_mapobj_get_position, LW(6), LW(10), LW(11), LW(12))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG7_ELEVATOR_CLOSE1"), LW(10), LW(11), LW(12), 0)
    END_INLINE_EVENT
    CALL(evt_sub_intpl_msec_init, 11, -9000, 0, 500)
    LABEL(4)
    CALL(evt_sub_intpl_msec_get_value)
    DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
    CALL(evt_mapobj_rotate, 1, LW(6), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 1, LW(7), 0, LW(0), 0)
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(4)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(ele_2)
    SET_VALUE(LW(5), LW(0))
    IF_INT_LESS(GSW_STORY_PROGRESS, 367)
        CALL(evt_mario_key_onoff, 0)
        CALL(evt_msg_print, 0, STRING("stg7_aji_65"), 0, 0)
        CALL(evt_mario_key_onoff, 1)
        RETURN
    END_IF
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    BEGIN_INLINE_EVENT_GET_ID(LW(10))
        CALL(evt_mario_get_party, LW(0))
        SWITCH(LW(0))
            CASE_EQUAL(2)
            LOOP(0)
                CALL(evt_party_nokotaro_get_status, 0, LW(0))
                IF_INT_EQUAL(LW(0), 0)
                    BREAK_LOOP
                END_IF
                WAIT_FRAMES(1)
            END_LOOP
            CASE_EQUAL(3)
            LOOP(0)
                CALL(evt_party_get_status, 3, LW(0))
                IF_INT_EQUAL(LW(0), 0)
                    BREAK_LOOP
                END_IF
                WAIT_FRAMES(1)
            END_LOOP
        END_SWITCH
        CALL(evt_party_stop, 0)
        CALL(evt_party_move_pos2, 0, 565, 0, FLOAT(80.0f))
        CALL(evt_party_set_dir, 0, 270, 300)
    END_INLINE_EVENT
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG7_ELEVATOR_ACCESS1"), LW(0), LW(1), LW(2), 0)
    BEGIN_INLINE_EVENT
        CALL(evt_cam3d_evt_set_now)
        CALL(evt_cam_road_reset)
        CALL(evt_cam_shift_reset)
        CALL(evt_cam3d_event_from_road, 550, LW(1), -120, 0, LW(0), LW(1), LW(2), LW(3), LW(4), LW(5))
        CALL(evt_cam3d_evt_set, LW(0), LW(1), LW(2), LW(3), LW(4), LW(5), 500, 11)
    END_INLINE_EVENT
    CALL(evt_mario_mov_pos2, 535, 0, FLOAT(80.0f))
    CALL(evt_mario_set_dir, 90, 200, 0)
    IF_INT_NOT_EQUAL(GW(1), LW(5))
        SWITCH(GW(1))
            CASE_EQUAL(2)
            SET_VALUE(LW(0), 2000)
            CASE_EQUAL(3)
            SET_VALUE(LW(0), 1000)
            CASE_EQUAL(4)
            SET_VALUE(LW(0), 0)
        END_SWITCH
        SWITCH(LW(5))
            CASE_EQUAL(2)
            SET_VALUE(LW(1), 2000)
            CASE_EQUAL(3)
            SET_VALUE(LW(1), 1000)
            CASE_EQUAL(4)
            SET_VALUE(LW(1), 0)
        END_SWITCH
        SET_VALUE(LW(2), LW(0))
        SUBTRACT(LW(2), LW(1))
        IF_INT_LESS(LW(2), 0)
            MULTIPLY(LW(2), -1)
        END_IF
        BEGIN_INLINE_EVENT
            CALL(evt_mapobj_get_position, STRING("ere2_m"), LW(10), LW(11), LW(12))
            CALL(evt_snd_sfxon_3d, STRING("SFX_STG7_ELEVATOR_APPEAR1"), LW(10), LW(11), LW(12), 0)
        END_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, 11, LW(0), LW(1), LW(2))
        LABEL(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_trans, 1, STRING("ere2_m"), 0, LW(0), 0)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(0)
        END_IF
        BEGIN_INLINE_EVENT
            CALL(evt_mapobj_get_position, STRING("ere2_m"), LW(10), LW(11), LW(12))
            CALL(evt_snd_sfxon_3d, STRING("SFX_STG7_ELEVATOR_APPEAR1"), LW(10), LW(11), LW(12), 0)
        END_INLINE_EVENT
        CALL(evt_hit_bind_update, STRING("A_ere2_m"))
    END_IF
    SET_VALUE(LW(0), LW(5))
    CALL_CHILD(PTR(ele2_door_open))
    CALL(evt_mario_mov_pos2, 535, -120, FLOAT(120.0f))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    ADD(LW(0), 30)
    LOOP(0)
        IS_EVENT_RUNNING(LW(10), LW(11))
        IF_INT_EQUAL(LW(11), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_party_stop, 0)
    CALL(evt_party_jump_pos, 0, LW(0), LW(1), LW(2), 400, 0, -1000)
    CALL(evt_mario_set_dir, 90, 200, 0)
    CALL(evt_party_set_dir, 0, 270, 300)
    CALL(evt_msg_print, 0, STRING("stg7_aji_66"), 0, 0)
    SWITCH(LW(5))
        CASE_EQUAL(2)
        CALL(evt_msg_select, 0, STRING("stg7_aji_ele_2_00"))
        CALL(evt_msg_continue)
        SWITCH(LW(0))
            CASE_EQUAL(0)
            SET_VALUE(LW(0), 3)
            CASE_EQUAL(1)
            SET_VALUE(LW(0), 4)
            CASE_DEFAULT
            SET_VALUE(LW(0), -1)
        END_SWITCH
        CASE_EQUAL(3)
        CALL(evt_msg_select, 0, STRING("stg7_aji_ele_2_01"))
        CALL(evt_msg_continue)
        SWITCH(LW(0))
            CASE_EQUAL(0)
            SET_VALUE(LW(0), 2)
            CASE_EQUAL(1)
            SET_VALUE(LW(0), 4)
            CASE_DEFAULT
            SET_VALUE(LW(0), -1)
        END_SWITCH
        CASE_EQUAL(4)
        CALL(evt_msg_select, 0, STRING("stg7_aji_ele_2_02"))
        CALL(evt_msg_continue)
        SWITCH(LW(0))
            CASE_EQUAL(0)
            SET_VALUE(LW(0), 2)
            CASE_EQUAL(1)
            SET_VALUE(LW(0), 3)
            CASE_DEFAULT
            SET_VALUE(LW(0), -1)
        END_SWITCH
    END_SWITCH
    SET_VALUE(LW(8), LW(0))
    CALL(evt_cam3d_evt_off, 500, 11)
    IF_INT_EQUAL(LW(0), -1)
        CALL(evt_mario_mov_pos2, 535, 0, FLOAT(120.0f))
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        ADD(LW(0), 30)
        CALL(evt_party_jump_pos, 0, LW(0), LW(1), LW(2), 400, 0, -1000)
        SET_VALUE(LW(0), LW(5))
        CALL_CHILD(PTR(ele2_door_close))
        SET_VALUE(GW(1), LW(5))
        CALL(N_evt_bero_set_mario_bottomless_respawn_pos_to_current_mario_pos)
        CALL(evt_party_run, 0)
        CALL(evt_mario_key_onoff, 1)
        RETURN
    END_IF
    SET_VALUE(LW(0), LW(5))
    CALL_CHILD(PTR(ele2_door_close))
    SWITCH(LW(5))
        CASE_EQUAL(2)
        SET_VALUE(LW(0), 2000)
        CASE_EQUAL(3)
        SET_VALUE(LW(0), 1000)
        CASE_EQUAL(4)
        SET_VALUE(LW(0), 0)
    END_SWITCH
    SWITCH(LW(8))
        CASE_EQUAL(2)
        SET_VALUE(LW(1), 2000)
        CASE_EQUAL(3)
        SET_VALUE(LW(1), 1000)
        CASE_EQUAL(4)
        SET_VALUE(LW(1), 0)
    END_SWITCH
    SET_VALUE(LW(2), LW(0))
    SUBTRACT(LW(2), LW(1))
    IF_INT_LESS(LW(2), 0)
        MULTIPLY(LW(2), -1)
    END_IF
    MULTIPLY(LW(2), 4)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_mapobj_get_position, STRING("ere2_m"), LW(10), LW(11), LW(12))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG7_ELEVATOR_MOVE1"), LW(10), LW(11), LW(12), LW(9))
    CALL(evt_sub_intpl_msec_init, 11, LW(0), LW(1), LW(2))
    LABEL(5)
    CALL(evt_sub_intpl_msec_get_value)
    CALL(evt_mapobj_trans, 1, STRING("ere2_m"), 0, LW(0), 0)
    CALL(evt_hit_bind_update, STRING("A_ere2_m"))
#ifdef VERSION_G8ME01
    CALL(evt_mapobj_get_position, STRING("ere2_m"), LW(2), LW(3), LW(4))
    CALL(evt_snd_sfx_pos, LW(9), LW(2), LW(3), LW(4))
#endif
    ADD(LW(0), -998)
    CALL(evt_mario_get_pos, 0, LW(2), LW(3), LW(4))
    CALL(evt_mario_set_pos, LW(2), LW(0), LW(4))
    CALL(evt_party_get_pos, 0, LW(2), LW(3), LW(4))
    CALL(evt_party_set_pos, 0, LW(2), LW(0), LW(4))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(5)
    END_IF
    CALL(evt_snd_sfxoff, LW(9))
    CALL(evt_mario_cont_onoff, 1)
    CALL(evt_party_cont_onoff, 0, 1)
    SET_VALUE(LW(0), LW(8))
    CALL_CHILD(PTR(ele2_door_open))
    CALL(evt_mario_mov_pos2, 535, 0, FLOAT(120.0f))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    ADD(LW(0), 30)
    CALL(evt_party_jump_pos, 0, LW(0), LW(1), LW(2), 400, 0, -1000)
    BEGIN_INLINE_EVENT
        SET_VALUE(LW(0), LW(8))
        CALL_CHILD(PTR(ele2_door_close))
    END_INLINE_EVENT
    SET_VALUE(GW(1), LW(8))
    CALL(N_evt_bero_set_mario_bottomless_respawn_pos_to_current_mario_pos)
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(evt_elv_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(373, 375)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ekagi1_aka"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ekagi2_aka"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ekagi3_aka"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ekagi1_mid"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ekagi2_mid"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ekagi3_mid"), 1)
        CALL(evt_run_case_evt, 9, 1, STRING("A_ekagi1"), 0, PTR(last_elv_lock), 0)
        CALL(evt_run_case_evt, 9, 1, STRING("A_ekagi2"), 0, PTR(last_elv_lock), 0)
        CALL(evt_run_case_evt, 9, 1, STRING("A_ekagi3"), 0, PTR(last_elv_lock), 0)
        CASE_DEFAULT
        IF_INT_LESS(GSW_STORY_PROGRESS, 363)
            CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ekagi1_aka"), 1)
            CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ekagi1_mid"), 1)
            CALL(evt_run_case_evt, 9, 1, STRING("A_ekagi1"), 0, PTR(elv_unlock1), 0)
            ELSE
            CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ekagi1_aka"), 1)
            CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ekagi2_aka"), 1)
            CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ekagi3_aka"), 1)
            CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ekagi1_mid"), 1)
            CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ekagi2_mid"), 1)
            CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ekagi3_mid"), 1)
            SET_VALUE(LW(0), 0)
            CALL(evt_run_case_evt, 9, 1, STRING("A_ekagi1"), 0, PTR(ele_1), 0)
            SET_VALUE(LW(0), 1)
            CALL(evt_run_case_evt, 9, 1, STRING("A_ekagi2"), 0, PTR(ele_1), 0)
            SET_VALUE(LW(0), 2)
            CALL(evt_run_case_evt, 9, 1, STRING("A_ekagi3"), 0, PTR(ele_1), 0)
        END_IF
    END_SWITCH
    IF_INT_LESS(GSW_STORY_PROGRESS, 368)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ekagi4_aka"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ekagi4_mid"), 1)
        CALL(evt_run_case_evt, 9, 1, STRING("A_ekagi4"), 0, PTR(elv_unlock2), 0)
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ekagi4_aka"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ekagi5_aka"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ekagi6_aka"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ekagi4_mid"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ekagi5_mid"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ekagi6_mid"), 1)
        SET_VALUE(LW(0), 2)
        CALL(evt_run_case_evt, 9, 1, STRING("A_ekagi4"), 0, PTR(ele_2), 0)
        SET_VALUE(LW(0), 3)
        CALL(evt_run_case_evt, 9, 1, STRING("A_ekagi5"), 0, PTR(ele_2), 0)
        SET_VALUE(LW(0), 4)
        CALL(evt_run_case_evt, 9, 1, STRING("A_ekagi6"), 0, PTR(ele_2), 0)
    END_IF
    SET_VALUE(GW(0), 2)
    SET_VALUE(GW(1), 4)
    CALL(evt_hit_bind_mapobj, STRING("A_ere1_m"), STRING("ere1_m"))
    CALL(evt_hit_bind_mapobj, STRING("A_ere2_m"), STRING("ere2_m"))
    RETURN
EVT_END()

EVT_BEGIN(key_lock)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_msg_print, 0, STRING("stg7_aji_71"), 0, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(last_key_lock)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_msg_print, 0, STRING("stg7_aji_45_02"), 0, 0)
    IF_INT_LESS(GSW(1071), 3)
        ADD(GSW(1071), 1)
        IF_INT_EQUAL(GSW(1071), 3)
            CALL(evt_party_stop, 0)
            WAIT_MS(500)
            CALL(evt_msg_print_party, STRING("stg7_aji_45_03"))
            CALL(evt_party_run, 0)
        END_IF
    END_IF
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(last_elv_lock)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_msg_print, 0, STRING("stg7_aji_45_01"), 0, 0)
    IF_INT_LESS(GSW(1071), 3)
        ADD(GSW(1071), 1)
        IF_INT_EQUAL(GSW(1071), 3)
            CALL(evt_party_stop, 0)
            WAIT_MS(500)
            CALL(evt_msg_print_party, STRING("stg7_aji_45_03"))
            CALL(evt_party_run, 0)
        END_IF
    END_IF
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(evt_key_init)
    IF_INT_LESS(GSW_STORY_PROGRESS, 373)
        CALL(evt_run_case_evt, 9, 1, STRING("e_bero_5"), 0, PTR(key_lock), 0)
        SET_VALUE(LW(0), STRING("e_bero_5"))
        CALL_CHILD(PTR(bero_case_switch_off))
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 376)
        CALL(evt_run_case_evt, 9, 1, STRING("e_bero_5"), 0, PTR(last_key_lock), 0)
        SET_VALUE(LW(0), STRING("e_bero_5"))
        CALL_CHILD(PTR(bero_case_switch_off))
    END_IF
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(373, 375)
        CALL(evt_run_case_evt, 9, 1, STRING("n_bero_1"), 0, PTR(last_key_lock), 0)
        CALL(evt_run_case_evt, 9, 1, STRING("n_bero_2"), 0, PTR(last_key_lock), 0)
        CALL(evt_run_case_evt, 9, 1, STRING("n_bero_3"), 0, PTR(last_key_lock), 0)
        CALL(evt_run_case_evt, 9, 1, STRING("n_bero_4"), 0, PTR(last_key_lock), 0)
        CALL(evt_run_case_evt, 9, 1, STRING("e_bero3"), 0, PTR(last_key_lock), 0)
        CALL(evt_run_case_evt, 9, 1, STRING("w_bero_3"), 0, PTR(last_key_lock), 0)
        CALL(evt_run_case_evt, 9, 1, STRING("w_bero_4"), 0, PTR(last_key_lock), 0)
        SET_VALUE(LW(0), STRING("n_bero_1"))
        CALL_CHILD(PTR(bero_case_switch_off))
        SET_VALUE(LW(0), STRING("n_bero_2"))
        CALL_CHILD(PTR(bero_case_switch_off))
        SET_VALUE(LW(0), STRING("n_bero_3"))
        CALL_CHILD(PTR(bero_case_switch_off))
        SET_VALUE(LW(0), STRING("n_bero_4"))
        CALL_CHILD(PTR(bero_case_switch_off))
        SET_VALUE(LW(0), STRING("e_bero3"))
        CALL_CHILD(PTR(bero_case_switch_off))
        SET_VALUE(LW(0), STRING("w_bero_3"))
        CALL_CHILD(PTR(bero_case_switch_off))
        SET_VALUE(LW(0), STRING("w_bero_4"))
        CALL_CHILD(PTR(bero_case_switch_off))
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_lights1_m"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_lights1_r"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_lights2_m"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_lights2_r"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_lights3_m"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_lights3_r"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_lights4_m"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_lights4_r"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_lights5_m"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_lights5_r"), 1)
        CASE_DEFAULT
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_lights1_r"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_lights1_m"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_lights2_r"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_lights2_m"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_lights3_r"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_lights3_m"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_lights4_r"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_lights4_m"), 1)
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(evt_aikotoba)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_msg_print, 0, STRING("stg7_aji_68"), 0, 0)
    CALL(evt_paper_entry, STRING("OFF_d_roll"))
    CALL(evt_img_entry, STRING("img"))
    CALL(evt_img_set_paper, STRING("img"), STRING("OFF_d_roll"))
    CALL(evt_img_set_paper_anim, STRING("img"), STRING("Z_1"))
    CALL(evt_img_alloc_capture, STRING("img"), 0, 0, 1, 0, 0, 608, 480)
    CALL(evt_img_clear_virtual_point, STRING("img"))
    CALL(evt_img_onoff, STRING("img"), 1)
    WAIT_FRAMES(1)
    CALL(evt_cam_letter_box_disable, 1)
    CALL(evt_cam_letter_box_onoff, 0, 0)
    CALL(evt_win_nameent_on, 4)
    BEGIN_INLINE_EVENT
        WAIT_MS(300)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_OFF4_NAME_ENTRY1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_img_set_paper_anim, STRING("img"), STRING("A_2"))
    CALL(evt_img_wait_animend, STRING("img"))
    CALL(evt_win_nameent_wait)
    CALL(evt_win_nameent_name, LW(0))
    BEGIN_INLINE_EVENT
        WAIT_MS(300)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_OFF4_NAME_ENTRY2"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_img_set_paper_anim, STRING("img"), STRING("A_1"))
    CALL(evt_img_wait_animend, STRING("img"))
    CALL(evt_win_nameent_off)
    CALL(evt_img_release, STRING("img"))
    CALL(evt_paper_delete, STRING("OFF_d_roll"))
    CALL(evt_cam_letter_box_disable, 0)
    CALL(get_password, LW(1))
    IF_STR_EQUAL(LW(0), LW(1))
        CALL(evt_msg_print, 0, STRING("stg7_aji_70"), 0, 0)
        SET_VALUE(GSW_STORY_PROGRESS, 365)
        SET_VALUE(LW(0), STRING("e_bero_2"))
        CALL_CHILD(PTR(bero_case_switch_on))
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_kagi1_mid"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_kagi1_aka"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_lights5_r"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_lights5_m"), 1)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG7_DOOR_UNLOCK1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_exit_case_evt)
        ELSE
        CALL(evt_msg_print, 0, STRING("stg7_aji_69"), 0, 0)
    END_IF
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(peach_evt_stg1)
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG7_DUN1"))
    CALL(evt_snd_envon, 272, STRING("ENV_STG7_AJI1"))
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_seq_wait, 2)
    WAIT_MS(1500)
    CALL(evt_fade_set_mapchange_type, 0, 12, 100, 11, 100)
    CALL(evt_bero_mapchange, STRING("aji_02"), STRING("peach_evt_stg1"))
    RETURN
EVT_END()

static EVT_BEGIN(peach_evt_stg1_1)
    IF_INT_EQUAL(GSWF(4200), 1)
        RETURN
    END_IF
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_seq_wait, 2)
    CALL(evt_bero_exec_wait, 65536)
    CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("P_S_4"), STRING("P_T_4"))
    CALL(evt_msg_print, 0, STRING("peach_aji_39"), 0, STRING("mario"))
    CALL(peach_set_pose, STRING("P_S_1"))
    SET_VALUE(GSWF(4200), 1)
    CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("P_S_1"), STRING("P_T_1"))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(peach_evt_stg1_2)
    IF_INT_EQUAL(GSWF(4201), 1)
        RETURN
    END_IF
    CALL(evt_seq_wait, 2)
    LOOP(0)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        IF_INT_GREATER_OR_EQUAL(LW(0), 900)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_PEACH_QUESTION1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_eff_fukidashi, 0, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 60)
    SET_VALUE(LW(1), 13)
    SPAWN_EVENT(PTR(bero_open_door_play_se))
    CALL(evt_sub_intpl_msec_init, 11, 0, 180, 500)
    LABEL(0)
    CALL(evt_sub_intpl_msec_get_value)
    CALL_CHILD(PTR(e5_door_open))
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    SET_VALUE(LW(0), STRING("e_bero_5"))
    CALL_CHILD(PTR(bero_case_switch_off))
    CALL(evt_bero_overwrite, STRING("e_bero_5"), PTR(e_bero_5_peach))
    SET_VALUE(LW(0), STRING("e_bero_5"))
    CALL_CHILD(PTR(bero_case_switch_on))
    WAIT_MS(700)
    CALL(evt_msg_print, 0, STRING("peach_aji_41"), 0, STRING("mario"))
    SET_VALUE(GSWF(4201), 1)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(peach_evt_stg3_1)
    CALL(evt_seq_wait, 2)
    LOOP(0)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        IF_INT_GREATER(LW(0), 490)
            IF_INT_LESS(LW(0), 610)
                BREAK_LOOP
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_adjust_dir)
    SET_VALUE(LW(0), 4)
    CALL_CHILD(PTR(ele2_door_open))
    CALL(peach_set_pose, STRING("P_B_1"))
    WAIT_MS(500)
    CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("P_B_2"), STRING("P_T_7"))
    CALL(evt_msg_print, 0, STRING("peach_aji_127"), 0, STRING("mario"))
    WAIT_FRAMES(1)
    CALL(peach_set_pose, STRING("P_S_1"))
    CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("P_S_1"), STRING("P_T_1"))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(peach_evt_stg3)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_seq_wait, 2)
    CALL(evt_bero_exec_wait, 65536)
    CALL(evt_msg_print, 0, STRING("peach_aji_126"), 0, STRING("mario"))
    SET_VALUE(GSW_STORY_PROGRESS, 166)
    SPAWN_EVENT(PTR(peach_evt_stg3_1))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(peach_evt_stg5)
    LOOP(0)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        IF_INT_EQUAL(LW(1), 1000)
            IF_INT_GREATER(LW(0), 650)
                CALL(evt_mario_key_onoff, 0)
                CALL(evt_msg_print, 0, STRING("peach_aji_219"), 0, 0)
                CALL(evt_mario_set_dir, 270, 300, 0)
                WAIT_MS(300)
                CALL(evt_msg_print, 0, STRING("peach_aji_220"), 0, 0)
                CALL(evt_mario_mov_pos2, 640, LW(2), FLOAT(48.0f))
                CALL(evt_mario_key_onoff, 1)
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

static EVT_BEGIN(peach_evt_stg5_1)
    CALL(evt_seq_wait, 2)
    CALL(evt_bero_exec_wait, 65536)
    CALL(evt_bero_get_entername, LW(0))
    IF_STR_EQUAL(LW(0), STRING("n_bero_1"))
        CALL(evt_mario_key_onoff, 0)
        CALL(evt_npc_set_ry, STRING("見張り"), 90)
        CALL(evt_eff_fukidashi, 1, STRING(""), STRING("見張り"), 1, 0, 0, 0, 0, 0, 0, 60)
        WAIT_MS(1000)
        CALL(evt_msg_print, 0, STRING("peach_aji_243"), 0, STRING("見張り"))
        CALL(evt_mario_key_onoff, 1)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(peach_evt_door_stop)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_msg_print, 0, STRING("peach_aji_130"), 0, STRING("mario"))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(peach_evt_ele_2)
    SET_VALUE(LW(5), LW(0))
    SET_VALUE(LW(9), LW(1))
    CALL(evt_mario_key_onoff, 0)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(166)
        SET_VALUE(LW(8), 2)
        CASE_EQUAL(262)
        SET_VALUE(LW(8), 2)
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("peach_aji_40"), 0, STRING("mario"))
        CALL(evt_mario_key_onoff, 1)
        RETURN
    END_SWITCH
    IF_INT_NOT_EQUAL(GW(1), LW(5))
        SWITCH(GW(1))
            CASE_EQUAL(2)
            SET_VALUE(LW(0), 2000)
            CASE_EQUAL(3)
            SET_VALUE(LW(0), 1000)
            CASE_EQUAL(4)
            SET_VALUE(LW(0), 0)
        END_SWITCH
        SWITCH(LW(5))
            CASE_EQUAL(2)
            SET_VALUE(LW(1), 2000)
            CASE_EQUAL(3)
            SET_VALUE(LW(1), 1000)
            CASE_EQUAL(4)
            SET_VALUE(LW(1), 0)
        END_SWITCH
        SET_VALUE(LW(2), LW(0))
        SUBTRACT(LW(2), LW(1))
        IF_INT_LESS(LW(2), 0)
            MULTIPLY(LW(2), -1)
        END_IF
        CALL(evt_mapobj_get_position, STRING("ere2_m"), LW(3), LW(4), LW(6))
        CALL(evt_snd_sfxon_3d, STRING("SFX_PEACH_ELEVATOR_MOVE1"), LW(3), LW(4), LW(6), LW(10))
        CALL(evt_sub_intpl_msec_init, 11, LW(0), LW(1), LW(2))
        LABEL(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_trans, 1, STRING("ere2_m"), 0, LW(0), 0)
        ADD(LW(0), LW(4))
        CALL(evt_snd_sfx_pos, LW(10), LW(3), LW(0), LW(6))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(0)
        END_IF
        CALL(evt_hit_bind_update, STRING("A_ere2_m"))
        CALL(evt_snd_sfxoff, LW(10))
        CALL(evt_snd_sfxon_3d, STRING("SFX_PEACH_ELEVATOR_APPEAR1"), LW(3), LW(0), LW(6), 0)
    END_IF
    IF_INT_NOT_EQUAL(GSW_STORY_PROGRESS, 166)
        SET_VALUE(LW(0), LW(5))
        CALL_CHILD(PTR(ele2_door_open))
    END_IF
    IF_INT_EQUAL(LW(9), 1)
        CALL(evt_mario_mov_pos2, 550, -40, FLOAT(80.0f))
    END_IF
    CALL(evt_mario_mov_pos2, 550, -120, FLOAT(120.0f))
    CALL(evt_mario_adjust_dir)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_cam3d_get_shift, LW(0), LW(1), LW(2), LW(3))
        CALL(evt_cam3d_event_from_road, LW(0), LW(1), LW(2), LW(3), LW(4), LW(6), LW(7), LW(9), LW(10), LW(11))
        CALL(evt_cam3d_evt_set, LW(4), LW(6), LW(7), LW(9), LW(10), LW(11), 0, 11)
        WAIT_FRAMES(1)
        CALL(evt_cam3d_road_shift_onoff, 0)
        CALL(evt_cam3d_evt_off, 1000, 11)
    END_INLINE_EVENT
    SET_VALUE(LW(0), LW(5))
    CALL_CHILD(PTR(ele2_door_close))
    SWITCH(LW(5))
        CASE_EQUAL(2)
        SET_VALUE(LW(0), 2000)
        CASE_EQUAL(3)
        SET_VALUE(LW(0), 1000)
        CASE_EQUAL(4)
        SET_VALUE(LW(0), 0)
    END_SWITCH
    SWITCH(LW(8))
        CASE_EQUAL(2)
        SET_VALUE(LW(1), 2000)
        CASE_EQUAL(3)
        SET_VALUE(LW(1), 1000)
        CASE_EQUAL(4)
        SET_VALUE(LW(1), 0)
    END_SWITCH
    SET_VALUE(LW(2), LW(0))
    SUBTRACT(LW(2), LW(1))
    IF_INT_LESS(LW(2), 0)
        MULTIPLY(LW(2), -1)
    END_IF
    MULTIPLY(LW(2), 4)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_mapobj_get_position, STRING("ere2_m"), LW(3), LW(4), LW(6))
    CALL(evt_snd_sfxon_3d, STRING("SFX_PEACH_ELEVATOR_APPEAR1"), LW(3), LW(0), LW(6), 0)
    WAIT_MS(400)
    CALL(evt_snd_sfxon_3d, STRING("SFX_PEACH_ELEVATOR_MOVE1"), LW(3), LW(4), LW(6), LW(10))
    CALL(evt_sub_intpl_msec_init, 11, LW(0), LW(1), LW(2))
    LABEL(5)
    CALL(evt_sub_intpl_msec_get_value)
    CALL(evt_mapobj_trans, 1, STRING("ere2_m"), 0, LW(0), 0)
    CALL(evt_hit_bind_update, STRING("A_ere2_m"))
    SET_VALUE(LW(7), LW(0))
    ADD(LW(7), LW(4))
    CALL(evt_snd_sfx_pos, LW(10), LW(3), LW(7), LW(6))
    ADD(LW(0), -999)
    CALL(evt_mario_get_pos, 0, LW(2), LW(7), LW(9))
    CALL(evt_mario_set_pos, LW(2), LW(0), LW(9))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(5)
    END_IF
    CALL(evt_snd_sfxoff, LW(10))
    CALL(evt_mapobj_get_position, STRING("ere2_m"), LW(3), LW(4), LW(6))
    CALL(evt_snd_sfxon_3d, STRING("SFX_PEACH_ELEVATOR_APPEAR1"), LW(3), LW(4), LW(6), 0)
    CALL(evt_mario_cont_onoff, 1)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 166)
        CALL(evt_msg_print, 0, STRING("peach_aji_128"), 0, 0)
        WAIT_MS(500)
        CALL(evt_msg_print, 0, STRING("peach_aji_129"), 0, STRING("mario"))
        CALL(evt_npc_flag_onoff, 0, STRING("手下１"), NPC_FLAG_NO_DRAW)
        CALL(evt_npc_set_position, STRING("手下１"), 700, 1000, 0)
        CALL(evt_npc_set_ry, STRING("手下１"), 270)
        CALL(evt_npc_move_position, STRING("手下１"), 300, 0, 0, 70, 0)
        CALL(evt_npc_set_position, STRING("手下１"), 0, -1000, 0)
        CALL(evt_npc_flag_onoff, 1, STRING("手下１"), NPC_FLAG_NO_DRAW)
        WAIT_MS(2000)
        CALL(evt_msg_print, 0, STRING("peach_aji_131"), 0, 0)
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 262)
        CALL(evt_msg_print, 0, STRING("peach_aji_218"), 0, 0)
    END_IF
    SET_VALUE(LW(0), LW(8))
    CALL_CHILD(PTR(ele2_door_open))
    CALL(evt_mario_mov_pos2, 550, 0, FLOAT(120.0f))
    SET_VALUE(LW(0), LW(8))
    CALL_CHILD(PTR(ele2_door_close))
    CALL(evt_cam3d_road_shift_onoff, 1)
    SET_VALUE(GW(1), LW(8))
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ekagi4_aka"), 1)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ekagi4_mid"), 1)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ekagi5_aka"), 1)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ekagi5_mid"), 1)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ekagi6_aka"), 1)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ekagi6_mid"), 1)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_exit_case_evt)
    RETURN
EVT_END()

EVT_BEGIN(peach_evt_ele_init)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ekagi3_aka"), 1)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ekagi3_mid"), 1)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 262)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ekagi4_aka"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ekagi5_aka"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ekagi6_aka"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ekagi4_mid"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ekagi5_mid"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ekagi6_mid"), 1)
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ekagi4_aka"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ekagi5_aka"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ekagi6_aka"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ekagi4_mid"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ekagi5_mid"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ekagi6_mid"), 1)
    END_IF
    SET_VALUE(GW(1), 4)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 165)
        CALL(evt_bero_get_entername, LW(0))
        IF_STR_EQUAL(LW(0), STRING("e_bero_5"))
            SET_VALUE(LW(0), 4)
            SET_VALUE(LW(1), 0)
            CALL(evt_run_case_evt, 8, 1, STRING("A_e2_6_door1"), 0, PTR(peach_evt_ele_2), 0)
            CALL(evt_run_case_evt, 8, 1, STRING("A_e2_6_door2"), 0, PTR(peach_evt_ele_2), 0)
            ELSE
            SET_VALUE(LW(0), 4)
            SET_VALUE(LW(1), 1)
            CALL(evt_run_case_evt, 9, 1, STRING("A_ekagi6"), 0, PTR(peach_evt_ele_2), 0)
        END_IF
        ELSE
        SET_VALUE(LW(0), 4)
        SET_VALUE(LW(1), 1)
        CALL(evt_run_case_evt, 9, 1, STRING("A_ekagi6"), 0, PTR(peach_evt_ele_2), 0)
    END_IF
    CALL(evt_run_case_evt, 9, 1, STRING("A_ekagi4"), 0, PTR(peach_evt_door_stop), 0)
    CALL(evt_run_case_evt, 9, 1, STRING("A_ekagi3"), 0, PTR(peach_evt_door_stop), 0)
    CALL(evt_hit_bind_mapobj, STRING("A_ere2_m"), STRING("ere2_m"))
    RETURN
EVT_END()

EVT_BEGIN(peach_evt_key_init)
    SET_VALUE(LF(0), 0)
    SET_VALUE(LF(1), 0)
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 165)
        IF_INT_LESS(GSW_STORY_PROGRESS, 169)
            SET_VALUE(LF(0), 1)
        END_IF
    END_IF
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 262)
        IF_INT_LESS(GSW_STORY_PROGRESS, 265)
            SET_VALUE(LF(1), 1)
        END_IF
    END_IF
    IF_INT_EQUAL(LF(0), 0)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_lights1_r"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_lights1_m"), 1)
        CALL(evt_run_case_evt, 9, 1, STRING("n_bero_4"), 0, PTR(peach_evt_door_stop), 0)
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_lights1_r"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_lights1_m"), 1)
    END_IF
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_lights2_r"), 1)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_lights2_m"), 1)
    CALL(evt_run_case_evt, 9, 1, STRING("n_bero_3"), 0, PTR(peach_evt_door_stop), 0)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_lights3_r"), 1)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_lights3_m"), 1)
    CALL(evt_run_case_evt, 9, 1, STRING("n_bero_2"), 0, PTR(peach_evt_door_stop), 0)
    IF_INT_EQUAL(LF(1), 0)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_lights4_r"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_lights4_m"), 1)
        CALL(evt_run_case_evt, 9, 1, STRING("n_bero_1"), 0, PTR(peach_evt_door_stop), 0)
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_lights4_r"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_lights4_m"), 1)
    END_IF
    CALL(evt_seq_wait, 2)
    CALL(evt_bero_exec_wait, 65536)
    IF_INT_EQUAL(LF(0), 0)
        SET_VALUE(LW(0), STRING("n_bero_4"))
        CALL_CHILD(PTR(bero_case_switch_off))
    END_IF
    SET_VALUE(LW(0), STRING("n_bero_3"))
    CALL_CHILD(PTR(bero_case_switch_off))
    SET_VALUE(LW(0), STRING("n_bero_2"))
    CALL_CHILD(PTR(bero_case_switch_off))
    IF_INT_EQUAL(LF(1), 0)
        SET_VALUE(LW(0), STRING("n_bero_1"))
        CALL_CHILD(PTR(bero_case_switch_off))
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(peach_evt_init)
    SET_VALUE(LW(15), 0)
    CALL(evt_mario_get_mode, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 1)
        RETURN
    END_IF
    CALL(evt_mario_set_mode, 1)
    IF_INT_EQUAL(GSWF(4204), 1)
        CALL(evt_peach_transform_gundan_on)
    END_IF
    CALL(evt_npc_setup, PTR(npcEnt))
    CALL(evt_bero_get_entername, LW(0))
    IF_STR_EQUAL(LW(0), STRING("peach_evt_stg1"))
        CALL(evt_cam3d_evt_set, -835, 3096, 541, -835, 3070, -7, 0, 11)
        WAIT_FRAMES(1)
        SPAWN_EVENT(PTR(peach_evt_stg1))
        SET_VALUE(LW(15), 1)
        RETURN
    END_IF
    CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_PEACH1"))
    CALL(evt_snd_envoff, 512)
    SPAWN_EVENT(PTR(peach_evt_ele_init))
    SPAWN_EVENT(PTR(peach_evt_key_init))
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 58)
        SPAWN_EVENT(PTR(peach_evt_stg1_1))
        SPAWN_EVENT(PTR(peach_evt_stg1_2))
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 165)
        CALL(evt_bero_get_entername, LW(0))
        IF_STR_EQUAL(LW(0), STRING("e_bero_5"))
            SPAWN_EVENT(PTR(peach_evt_stg3))
            BEGIN_INLINE_EVENT
                CALL(evt_seq_wait, 2)
                CALL(evt_bero_exec_wait, 65536)
                CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ekagi4_aka"), 1)
                CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ekagi4_mid"), 1)
                CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ekagi5_aka"), 1)
                CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ekagi5_mid"), 1)
                CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ekagi6_aka"), 1)
                CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ekagi6_mid"), 1)
                SET_VALUE(LW(0), STRING("w_bero_3"))
                CALL_CHILD(PTR(bero_case_switch_off))
                SET_VALUE(LW(0), STRING("e_bero3"))
                CALL_CHILD(PTR(bero_case_switch_off))
                SET_VALUE(LW(0), STRING("w_bero_5"))
                CALL_CHILD(PTR(bero_case_switch_off))
                SET_VALUE(LW(0), STRING("e_bero_5"))
                CALL_CHILD(PTR(bero_case_switch_off))
                CALL(evt_run_case_evt, 9, 1, STRING("w_bero_3"), 0, PTR(peach_evt_door_stop), 0)
                CALL(evt_run_case_evt, 9, 1, STRING("e_bero3"), 0, PTR(peach_evt_door_stop), 0)
                CALL(evt_run_case_evt, 9, 1, STRING("w_bero_5"), 0, PTR(peach_evt_door_stop), 0)
                CALL(evt_run_case_evt, 9, 1, STRING("e_bero_5"), 0, PTR(peach_evt_door_stop), 0)
            END_INLINE_EVENT
        END_IF
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 166)
        SPAWN_EVENT(PTR(peach_evt_stg3_1))
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 167)
        BEGIN_INLINE_EVENT
            CALL(evt_seq_wait, 2)
            CALL(evt_bero_exec_wait, 65536)
            SET_VALUE(LW(0), STRING("w_bero_3"))
            CALL_CHILD(PTR(bero_case_switch_off))
            CALL(evt_run_case_evt, 9, 1, STRING("w_bero_3"), 0, PTR(peach_evt_door_stop), 0)
        END_INLINE_EVENT
    END_IF
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 263)
        IF_INT_LESS_OR_EQUAL(GSW_STORY_PROGRESS, 264)
            CALL(evt_npc_entry, STRING("見張り"), STRING("c_zako_n"))
            CALL(evt_npc_set_tribe, STRING("見張り"), STRING("第三勢力部下"))
            CALL(evt_npc_set_position, STRING("見張り"), -900, 1000, -50)
            CALL(evt_npc_set_ry, STRING("見張り"), 90)
            CALL(evt_npc_set_width, STRING("見張り"), 30)
            CALL(evt_npc_set_height, STRING("見張り"), 33)
            CALL(evt_npc_flag_onoff, 1, STRING("見張り"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
            CALL(evt_npc_set_stay_anim, STRING("見張り"))
            BEGIN_INLINE_EVENT
                CALL(evt_seq_wait, 2)
                CALL(evt_bero_exec_wait, 65536)
                SET_VALUE(LW(0), STRING("w_bero_3"))
                CALL_CHILD(PTR(bero_case_switch_off))
                SET_VALUE(LW(0), STRING("w_bero_5"))
                CALL_CHILD(PTR(bero_case_switch_off))
                SET_VALUE(LW(0), STRING("e_bero_5"))
                CALL_CHILD(PTR(bero_case_switch_off))
                CALL(evt_run_case_evt, 9, 1, STRING("w_bero_3"), 0, PTR(peach_evt_door_stop), 0)
                CALL(evt_run_case_evt, 9, 1, STRING("w_bero_5"), 0, PTR(peach_evt_door_stop), 0)
                CALL(evt_run_case_evt, 9, 1, STRING("e_bero_5"), 0, PTR(peach_evt_door_stop), 0)
            END_INLINE_EVENT
        END_IF
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 262)
        SPAWN_EVENT(PTR(peach_evt_stg5))
        BEGIN_INLINE_EVENT
            CALL(evt_seq_wait, 2)
            CALL(evt_bero_exec_wait, 65536)
            SET_VALUE(LW(0), STRING("w_bero_3"))
            CALL_CHILD(PTR(bero_case_switch_off))
            SET_VALUE(LW(0), STRING("w_bero_5"))
            CALL_CHILD(PTR(bero_case_switch_off))
            SET_VALUE(LW(0), STRING("e_bero_5"))
            CALL_CHILD(PTR(bero_case_switch_off))
            CALL(evt_run_case_evt, 9, 1, STRING("w_bero_3"), 0, PTR(peach_evt_door_stop), 0)
            CALL(evt_run_case_evt, 9, 1, STRING("w_bero_5"), 0, PTR(peach_evt_door_stop), 0)
            CALL(evt_run_case_evt, 9, 1, STRING("e_bero_5"), 0, PTR(peach_evt_door_stop), 0)
        END_INLINE_EVENT
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 263)
        CALL(peach_evt_toumei)
        SPAWN_EVENT(PTR(peach_evt_stg5_1))
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 264)
        CALL(peach_evt_toumei)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(teshita1_init)
    CALL_CHILD(PTR(npc_init_evt))
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_STOP_FOR_EVENT)
    CALL(evt_npc_status_onoff, 1, STRING("me"), 2)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(167)
        CALL(evt_npc_set_position, STRING("me"), 100, 1000, -50)
        RETURN
        CASE_DEFAULT
        CALL(evt_npc_change_interrupt, STRING("me"), 1, 0)
        CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_DRAW)
    END_SWITCH
    CALL(evt_npc_change_interrupt, STRING("me"), 6, 0)
    RETURN
EVT_END()

EVT_BEGIN(teshita1_talk)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_msg_print, 0, STRING("peach_aji_138"), 0, STRING("me"))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(teshita2_init)
    CALL_CHILD(PTR(npc_init_evt))
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_STOP_FOR_EVENT)
    CALL(evt_npc_status_onoff, 1, STRING("me"), 2)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(167)
        CALL(evt_npc_set_position, STRING("me"), 0, 1000, -15)
        RETURN
        CASE_BETWEEN(263, 265)
        CALL(evt_npc_set_position, STRING("me"), 0, 1000, -15)
        CASE_DEFAULT
        CALL(evt_npc_change_interrupt, STRING("me"), 1, 0)
        CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_DRAW)
    END_SWITCH
    CALL(evt_npc_change_interrupt, STRING("me"), 6, 0)
    RETURN
EVT_END()

EVT_BEGIN(teshita2_talk)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_msg_print, 0, STRING("peach_aji_139"), 0, STRING("手下２"))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(teshita2_regl)
    LOOP(0)
        CALL(evt_npc_move_position, STRING("me"), 950, -15, 0, 30, 0)
        WAIT_MS(300)
        CALL(evt_npc_reverse_ry, STRING("me"))
        WAIT_MS(300)
        CALL(evt_npc_reverse_ry, STRING("me"))
        WAIT_MS(300)
        CALL(evt_npc_move_position, STRING("me"), -950, -15, 0, 30, 0)
        WAIT_MS(300)
    END_LOOP
    RETURN
EVT_END()

static EVT_BEGIN(teshita3_init)
    CALL_CHILD(PTR(npc_init_evt))
    CALL(evt_npc_status_onoff, 1, STRING("me"), 2)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(167)
        CALL(evt_npc_set_position, STRING("me"), 750, 1000, 20)
        RETURN
        CASE_BETWEEN(263, 265)
        CALL(evt_npc_set_position, STRING("me"), 750, 1000, 20)
        CASE_DEFAULT
        CALL(evt_npc_change_interrupt, STRING("me"), 1, 0)
        CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_DRAW)
    END_SWITCH
    CALL(evt_npc_change_interrupt, STRING("me"), 6, 0)
    RETURN
EVT_END()

EVT_BEGIN(teshita3_talk)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_msg_print, 0, STRING("peach_aji_140"), 0, STRING("手下３"))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(teshita3_regl)
    LOOP(0)
        CALL(evt_npc_move_position, STRING("me"), -950, 20, 0, 30, 0)
        WAIT_MS(300)
        CALL(evt_npc_reverse_ry, STRING("me"))
        WAIT_MS(300)
        CALL(evt_npc_reverse_ry, STRING("me"))
        WAIT_MS(300)
        CALL(evt_npc_move_position, STRING("me"), 950, 20, 0, 30, 0)
        WAIT_MS(300)
    END_LOOP
    RETURN
EVT_END()

static NpcSetupInfo npcEnt[] = {
    {
        /*"手下１"*/"\x8E\xE8\x89\xBA\x82\x50", // name
        0, 0, // flags..reactionFlags
        teshita1_init, // initEvent
        NULL, // regularEvent
        teshita1_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"手下２"*/"\x8E\xE8\x89\xBA\x82\x51", // name
        0, 0, // flags..reactionFlags
        teshita2_init, // initEvent
        teshita2_regl, // regularEvent
        teshita2_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"手下３"*/"\x8E\xE8\x89\xBA\x82\x52", // name
        0, 0, // flags..reactionFlags
        teshita3_init, // initEvent
        teshita3_regl, // regularEvent
        teshita3_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

EVT_BEGIN(aji_01_init_evt)
    CALL_CHILD(PTR(peach_evt_init))
    CALL(evt_snd_set_rev_mode, 1)
    IF_INT_EQUAL(LW(15), 1)
        RETURN
    END_IF
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_map_playanim, STRING("hosi_01"), 1, 0)
    CALL(evt_map_playanim, STRING("hosi_02"), 1, 0)
    CALL(evt_map_playanim, STRING("hosi_03"), 1, 0)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("polySurface1300_q00266"), 2)
    IF_INT_LESS(GSW_STORY_PROGRESS, 365)
        SET_VALUE(LW(0), STRING("e_bero_2"))
        CALL_CHILD(PTR(bero_case_switch_off))
        CALL(evt_run_case_evt, 9, 1, STRING("A_kagi1"), 0, PTR(evt_aikotoba), 0)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_kagi1_mid"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_kagi1_aka"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_lights5_m"), 1)
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_kagi1_mid"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_kagi1_aka"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_lights5_r"), 1)
    END_IF
    SPAWN_EVENT(PTR(evt_emergency))
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 373)
        CALL(evt_npc_check_delete, STRING("軍団ザコ１"))
        CALL(evt_npc_check_delete, STRING("軍団ザコ２"))
        CALL(evt_npc_check_delete, STRING("軍団ザコ３"))
        CALL(evt_npc_check_delete, STRING("軍団ザコ４"))
        CALL(evt_npc_check_delete, STRING("軍団ザコ５"))
        CALL(evt_npc_check_delete, STRING("軍団ザコ６"))
        CALL(evt_npc_check_delete, STRING("軍団ザコ７"))
        CALL(evt_npc_check_delete, STRING("バリアーンＺ"))
    END_IF
    CALL(evt_mario_get_mode, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_mario_kill_party, 0)
        CALL(evt_npc_check_delete, STRING("軍団ザコ１"))
        CALL(evt_npc_check_delete, STRING("軍団ザコ２"))
        CALL(evt_npc_check_delete, STRING("軍団ザコ３"))
        CALL(evt_npc_check_delete, STRING("軍団ザコ４"))
        CALL(evt_npc_check_delete, STRING("軍団ザコ５"))
        CALL(evt_npc_check_delete, STRING("軍団ザコ６"))
        CALL(evt_npc_check_delete, STRING("軍団ザコ７"))
        CALL(evt_npc_check_delete, STRING("バリアーンＺ"))
        ELSE
        CALL(evt_snd_bgmon, 512, STRING("BGM_STG7_DUN2"))
        CALL(evt_snd_envon, 272, STRING("ENV_STG7_AJI1"))
        CALL_CHILD(PTR(evt_elv_init))
        CALL_CHILD(PTR(evt_key_init))
    END_IF
    RETURN
EVT_END()

USER_FUNC(get_password) {
    EvtWord* args = event->args;

    evtSetValue(event, args[0], (EvtWord)msgSearch("stg7_aji_password"));
    return EVT_RETURN_DONE;
}

static USER_FUNC(peach_set_pose) {
    const char* pose = (const char*)evtGetValue(event, event->args[0]);
    MarioWork* mario = marioGetPtr();
    marioChgPose(pose);
    return EVT_RETURN_DONE;
}
static USER_FUNC(peach_evt_toumei) {
    MarioWork* mario = marioGetPtr();

    mario->scale.z = mario->scale.y = mario->scale.x = 0.0f;
    return EVT_RETURN_DONE;
}
