#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "driver/mapdrv.h"
#include "evt/evt_cam.h"
#include "evt/evt_fade.h"
#include "evt/evt_mario.h"
#include "evt/evt_seq.h"
#include "evt/evt_snd.h"
#include "party/party_motion.h"
#include "evt/evt_item.h"
#include "mario/mario_pouch.h"
#include "driver/itemdrv.h"

#include "stdio.h"
#include "mario/mario.h"
#include "party.h"
#include "driver/hitdrv.h"
#include "string.h"
#include "pmario_sound.h"
#include "eff/n64/eff_akari_charge_n64.h"
#include "evt/evt_bero.h"
#include "evt/evt_hit.h"
#include "evt/evt_map.h"
#include "evt/evt_mobj.h"
#include "evt/evt_sub.h"
#include "aji.h"
#include "manager/evtmgr_cmd.h"
#include "driver/seqdrv.h"


USER_FUNC(all_flag_off);
USER_FUNC(evt_mario_dispflag_onoff);
USER_FUNC(hikaru_off_func);
USER_FUNC(hikaru_on_func);
static USER_FUNC(ride_chk);
static USER_FUNC(thunder_main);
static USER_FUNC(loop_onoff);

static EvtWord w_door_close[22];
static EvtWord w_door_open[22];
static BeroEntry bero_entry_data[2];
u8 root_0[10];
u8 root_1[14];
u8 root_2[15];
typedef struct RootRoute { u8* panels; s32 count; } RootRoute;
RootRoute root_data[3];
static EvtWord hikaru_on[4];
static EvtWord hikaru_off[4];
EvtWord hikaru_yuka[28];
static EvtWord evt_box[95];
static HitDamageReturn dmgReturnPt[2];
static EvtWord hikaru_yuka_init[83];
static EvtWord peach_evt_stg1[56];
EvtWord aji_02_init_evt[125];

static EVT_BEGIN(w_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.25f))
    CALL(evt_mapobj_trans, 0, STRING("S_door1_1"), 0, 0, LW(0))
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_trans, 0, STRING("S_door1_2"), 0, 0, LW(0))
    RETURN
EVT_END()

static EVT_BEGIN(w_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.25f))
    CALL(evt_mapobj_trans, 0, STRING("S_door1_1"), 0, 0, LW(0))
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_trans, 0, STRING("S_door1_2"), 0, 0, LW(0))
    RETURN
EVT_END()

static BeroEntry bero_entry_data[] = {
    {
        "w_bero", // hitName
        1, // type
        10, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "aji_01", // nextMapName
        "e_bero_1", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        w_door_close, // enterAnimeArg
        w_door_open, // exitAnimeArg
    },
    {0},
};

u8 root_0[] = {
    10, 9, 8, 7, 6, 5, 4, 3,
    2, 1,
};

u8 root_1[] = {
    21, 8, 7, 6, 20, 19, 18, 17,
    16, 15, 14, 13, 12, 11,
};

u8 root_2[] = {
    30, 29, 28, 27, 26, 17, 18, 19,
    20, 6, 5, 25, 24, 23, 22,
};

RootRoute root_data[] = {
    {root_0, 10},
    {root_1, 14},
    {root_2, 15},
};

static EVT_BEGIN(hikaru_on)
    CALL(hikaru_on_func)
    RETURN
EVT_END()

static EVT_BEGIN(hikaru_off)
    CALL(hikaru_off_func)
    RETURN
EVT_END()

EVT_BEGIN(hikaru_yuka)
    CALL_CHILD(PTR(hikaru_on))
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_set, 80, 172, 562, 80, 50, -10, 1000, EASING_SINE_IN_OUT)
    WAIT_MS(2000)
    CALL(evt_cam3d_evt_off, 800, EASING_SINE_IN_OUT)
    WAIT_MS(800)
    CALL_CHILD(PTR(hikaru_off))
    WAIT_MS(500)
    RETURN
EVT_END()

static EVT_BEGIN(evt_box)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mobj_get_position, STRING("box"), LW(0), LW(1), LW(2))
    CALL(evt_item_entry, STRING("dummy"), ITEM_ELEVATOR_KEY_0x1B, LW(0), LW(1), LW(2), ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
    CALL(evt_mobj_wait_animation_end, STRING("box"))
    CALL(evt_item_get_item, STRING("dummy"))
    SET_VALUE(GSW_STORY_PROGRESS, STG7_09_ELEVATOR_KEY_A_LIT_FLOOR)
    WAIT_FRAMES(1)
    CALL(evt_hitobj_attr_onoff, 1, 0, STRING("A_no01"), 2048)
    CALL(all_flag_off)
    BEGIN_INLINE_EVENT
        WAIT_MS(1000)
        CALL(evt_mario_set_dir, 270, 300, 0)
    END_INLINE_EVENT
    LOOP(5)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("yukayu1"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("yukayu2"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("ato"), MAP_OBJ_FLAG_HIDDEN)
        WAIT_MS(150)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("yukayu1"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("yukayu2"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("ato"), MAP_OBJ_FLAG_HIDDEN)
        WAIT_MS(150)
    END_LOOP
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static HitDamageReturn dmgReturnPt[] = {
    {"mod_01", {0.0f, -1000.0f, 0.0f}},
    {0},
};

static EvtWord hikaru_yuka_init[] = {
    CALL_CHILD(hikaru_off)
    CALL(evt_hitobj_attr_onoff, 1, 1, STRING("A_no01"), 2048)
    CALL(evt_mobj_itembox, STRING("box"), 160, 10, -10, MOBJ_ITEMBOX_GRAY, 0, PTR(evt_box), GSWF(4176))
    CALL(evt_hit_damage_return_set, PTR(dmgReturnPt))
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("yukayu1"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("yukayu2"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_map_playanim, STRING("denyuka"), MAP_ANIM_PLAY_LOOP, 0)
    CALL(evt_sub_random, 300, LW(0))
    SWITCH(LW(0))
        CASE_LESS(100)
        SET_VALUE(GW(10), 0)
        CASE_LESS(200)
        SET_VALUE(GW(10), 1)
        CASE_DEFAULT
        SET_VALUE(GW(10), 2)
    END_SWITCH
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_bero_exec_wait, 65536)
    CALL_CHILD(hikaru_yuka)
    CALL(evt_mario_key_onoff, 1)
    BEGIN_INLINE_EVENT
        CALL(ride_chk)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(thunder_main)
    END_INLINE_EVENT
    OPCODE_END_SCRIPT,
    RETURN
};

static EVT_BEGIN(peach_evt_stg1)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_HIDE)
    CALL(evt_mario_set_pos, -140, 10, 0)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("no1"), MAP_OBJ_FLAG_HIDDEN)
    CALL_CHILD(PTR(hikaru_off))
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("yukayu1"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("yukayu2"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("ato"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_seq_wait, SEQ_GAME)
    WAIT_MS(1500)
    CALL(evt_fade_set_mapchange_type, 0, 12, 100, 11, 100)
    CALL(evt_bero_mapchange, STRING("aji_16"), STRING("peach_evt_stg1"))
    RETURN
EVT_END()

EVT_BEGIN(aji_02_init_evt)
    CALL(evt_bero_get_entername, LW(0))
    CALL(evt_snd_set_rev_mode, 1)
    IF_INT_LESS(GSW_STORY_PROGRESS, STG7_00_START)
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG7_DUN1"))
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG7_AJI1"))
        ELSE
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG7_DUN2"))
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG7_AJI1"))
    END_IF
    IF_STR_EQUAL(LW(0), STRING("peach_evt_stg1"))
        CALL(evt_mario_kill_party, 0)
        CALL(evt_mario_set_mode, 1)
        CALL(evt_mario_set_pos, -140, 10, 0)
        CALL(evt_cam_road_reset)
        WAIT_FRAMES(1)
        CALL(evt_cam3d_evt_set, -78, 172, 560, -78, 50, -9, 0, EASING_SINE_IN_OUT)
        WAIT_FRAMES(1)
        SPAWN_EVENT(PTR(peach_evt_stg1))
        RETURN
    END_IF
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_cam_road_reset)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("no1"), MAP_OBJ_FLAG_HIDDEN)
    IF_INT_LESS(GSW_STORY_PROGRESS, STG7_09_ELEVATOR_KEY_A_LIT_FLOOR)
        SPAWN_EVENT(PTR(hikaru_yuka_init))
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("ato"), MAP_OBJ_FLAG_HIDDEN)
        WAIT_FRAMES(1)
        ELSE
        CALL_CHILD(PTR(hikaru_off))
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("yukayu1"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("yukayu2"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("ato"), MAP_OBJ_FLAG_HIDDEN)
    END_IF
    CALL(evt_mobj_badgeblk, STRING("mobj00"), -125, 60, -125, ITEM_SUPER_SHROOM, 0, GSWF(4211), 0)
    SPAWN_EVENT(PTR(evt_emergency))
    RETURN
EVT_END()

static USER_FUNC(ride_chk) {
    static s32 onoff[30];
    char hitName[64];
    char name[64];
    MarioWork* mario = marioGetPtr();
    PartyEntry* party = partyGetPtr(0);
    s32 i;
    HitEntry* hit;

    if (isFirstCall) {
        memset(onoff, 0, sizeof(onoff));
    }
    if (party != NULL && party->currentMemberId == PARTY_MEMBER_YOSHI && (party->flags & PARTY_FLAG_IS_BEING_USED)) {
        hit = mario->hitobjStandOn;
        if (hit == NULL) {
            hit = mario->hitobjJumpFrom;
        }
    } else {
        hit = (HitEntry*)mario->hitobjUnder;
    }
    // panels stay lit for 30 frames after being touched
    for (i = 0; i < 30; i++) {
        if (hit != NULL) {
            sprintf(hitName, "A_hikaru_%02d", i + 1);
            if (strcmp(hit->joint->name, hitName) == 0) {
                onoff[i] = 30;
            }
        }
    }
    for (i = 0; i < 30; i++) {
        if (onoff[i] > 0) {
            onoff[i]--;
        }
        if (onoff[i] != 0) {
            sprintf(name, "hikaru_%02d", i + 1);
            mapGrpFlagOff(name, MAP_OBJ_FLAG_HIDDEN);
            sprintf(name, "nashi_%02d", i + 1);
            mapGrpFlagOn(name, MAP_OBJ_FLAG_HIDDEN);
        } else {
            sprintf(name, "hikaru_%02d", i + 1);
            mapGrpFlagOn(name, MAP_OBJ_FLAG_HIDDEN);
            sprintf(name, "nashi_%02d", i + 1);
            mapGrpFlagOff(name, MAP_OBJ_FLAG_HIDDEN);
        }
    }
    if (evtGetValue(event, GSW_STORY_PROGRESS) >= 362) {
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}

static USER_FUNC(thunder_main) {
    MarioWork* mario = marioGetPtr();

    if (isFirstCall) {
        event->userdata[0] = 0;
    }
    if (event->userdata[0] == 0) {
        if (mario->currMotionId == MARIO_MOTION_DAMAGE_TOGE) {
            EffectEntry* eff;

            event->userdata[0] = 1;
            psndSFXOn_3D("SFX_STG7_ELECTRIC_SHOCK1", &mario->position);
            eff = effAkariChargeN64Entry(0, 100, mario->position.x, 30.0f + mario->position.y, mario->position.z, 1.0f);
            ((EffAkariChargeN64Work*)eff->userdata)->elem[0].r = 30;
            ((EffAkariChargeN64Work*)eff->userdata)->elem[0].g = 231;
            ((EffAkariChargeN64Work*)eff->userdata)->elem[0].b = 219;
            ((EffAkariChargeN64Work*)eff->userdata)->elem[0].r2 = 0;
            ((EffAkariChargeN64Work*)eff->userdata)->elem[0].g2 = 0;
            ((EffAkariChargeN64Work*)eff->userdata)->elem[0].b2 = 0;
        }
    } else if (mario->currMotionId != MARIO_MOTION_DAMAGE_TOGE) {
        event->userdata[0] = 0;
    }
    return EVT_RETURN_BLOCK;
}

static USER_FUNC(loop_onoff) {
    char name[64];
    s32 i;

    if (isFirstCall) {
        event->userdata[0] = 0;
        event->userdata[1] = 0;
        event->userdata[2] = 0;
    }
    // light the panels up one after another, twice
    event->userdata[0]++;
    if (event->userdata[0] > 2) {
        event->userdata[0] = 0;
        event->userdata[1]++;
        if (event->userdata[1] > 30) {
            event->userdata[1] = 0;
            event->userdata[2]++;
            if (event->userdata[2] > 1) {
                return EVT_RETURN_DONE;
            }
        }
    }
    for (i = 1; i <= 30; i++) {
        if (i == event->userdata[1]) {
            sprintf(name, "hikaru_%02d", i);
            mapGrpFlagOff(name, MAP_OBJ_FLAG_HIDDEN);
            sprintf(name, "nashi_%02d", i);
            mapGrpFlagOn(name, MAP_OBJ_FLAG_HIDDEN);
        } else {
            sprintf(name, "hikaru_%02d", i);
            mapGrpFlagOn(name, MAP_OBJ_FLAG_HIDDEN);
            sprintf(name, "nashi_%02d", i);
            mapGrpFlagOff(name, MAP_OBJ_FLAG_HIDDEN);
        }
    }
    return EVT_RETURN_BLOCK;
}

USER_FUNC(all_flag_off) {
    char name[64];
    s32 i;

    for (i = 1; i <= 30; i++) {
        sprintf(name, "hikaru_%02d", i);
        mapGrpFlagOn(name, MAP_OBJ_FLAG_HIDDEN);
        sprintf(name, "nashi_%02d", i);
        mapGrpFlagOn(name, MAP_OBJ_FLAG_HIDDEN);
        sprintf(name, "A_hikaru_%02d", i);
        hitGrpAttrOff(name, 0x800);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(hikaru_on_func) {
    char name[64];
    s32 idx = evtGetValue(event, GW(10));
    // root_data holds {panel list, panel count} for each route
    u8* panels = root_data[idx].panels;
    s32 count = root_data[idx].count;
    s32 i;

    // light the panels on the chosen route
    for (i = 1; i <= 30; i++) {
        u8* p = panels;
        s32 j;

        for (j = 0; j < count; j++, p++) {
            if (i == *p) {
                sprintf(name, "hikaru_%02d", i);
                mapGrpFlagOff(name, MAP_OBJ_FLAG_HIDDEN);
                sprintf(name, "nashi_%02d", i);
                mapGrpFlagOn(name, MAP_OBJ_FLAG_HIDDEN);
                sprintf(name, "A_hikaru_%02d", i);
                hitGrpAttrOff(name, 0x800);
                break;
            }
        }
        if (j >= count) {
            sprintf(name, "hikaru_%02d", i);
            mapGrpFlagOn(name, MAP_OBJ_FLAG_HIDDEN);
            sprintf(name, "nashi_%02d", i);
            mapGrpFlagOff(name, MAP_OBJ_FLAG_HIDDEN);
            sprintf(name, "A_hikaru_%02d", i);
            hitGrpAttrOn(name, 0x800);
        }
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(hikaru_off_func) {
    char name[64];
    s32 i;

    for (i = 1; i <= 30; i++) {
        sprintf(name, "hikaru_%02d", i);
        mapGrpFlagOn(name, MAP_OBJ_FLAG_HIDDEN);
        sprintf(name, "nashi_%02d", i);
        mapGrpFlagOn(name, MAP_OBJ_FLAG_HIDDEN);
    }
    return EVT_RETURN_DONE;
}

