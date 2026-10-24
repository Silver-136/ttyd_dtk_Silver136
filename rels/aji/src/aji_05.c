#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"

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
#include "aji.h"
#include "manager/evtmgr_cmd.h"

void mapGrpFlagOn(const char* name, u32 flags);
void mapGrpFlagOff(const char* name, u32 flags);

USER_FUNC(_hikaru_off);
USER_FUNC(_hikaru_on);
USER_FUNC(evt_bero_get_info);
USER_FUNC(evt_hitobj_attr_onoff);
USER_FUNC(evt_item_entry);
USER_FUNC(evt_item_get_item);
USER_FUNC(evt_mapobj_flag_onoff);
USER_FUNC(evt_mapobj_trans);
USER_FUNC(evt_mario_key_onoff);
USER_FUNC(evt_mario_set_dir);
USER_FUNC(evt_mobj_badgeblk);
USER_FUNC(evt_mobj_get_position);
USER_FUNC(evt_mobj_wait_animation_end);
USER_FUNC(evt_snd_bgmon);
USER_FUNC(evt_snd_envon);
USER_FUNC(evt_snd_set_rev_mode);
static USER_FUNC(ride_chk);
static USER_FUNC(thunder_main);
static USER_FUNC(loop_onoff);

f32 fd_aji_05_negone_one_374[2] __attribute__((aligned(8)));
f32 fd_aji_05_neg_one_382;
f32 fd_aji_05___local_PI_383;
f32 fd_aji_05___local_3PIO2_385;
f32 fd_aji_05_one_negone_373[2] __attribute__((aligned(8)));
f32 fd_aji_05___local_PIO2_384;
f32 fd_aji_05_one_381;
f32 fd_aji_05_negone_one_360[2] __attribute__((aligned(8)));
f32 fd_aji_05_neg_one_365;
f32 fd_aji_05___local_PI_366;
f32 fd_aji_05___local_3PIO2_368;
f32 fd_aji_05_one_negone_359[2] __attribute__((aligned(8)));
f32 fd_aji_05___local_PIO2_367;
f32 fd_aji_05_one_364;
static EvtWord e_door_close[22];
static EvtWord e_door_open[22];
static BeroEntry bero_entry_data[2];
static EvtWord evt_box[95];
static EvtWord hikaru_on[4];
static EvtWord hikaru_off[4];
static HitDamageReturn dmgReturnPt[2];
static EvtWord hikaru_yuka_init[48];
EvtWord aji_05_init_evt[77];

f32 fd_aji_05_negone_one_374[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_aji_05_neg_one_382 = -1.0f;

f32 fd_aji_05___local_PI_383 = 3.1415927f;

f32 fd_aji_05___local_3PIO2_385 = 4.712389f;

f32 fd_aji_05_one_negone_373[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_aji_05___local_PIO2_384 = 1.5707964f;

f32 fd_aji_05_one_381 = 1.0f;

f32 fd_aji_05_negone_one_360[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_aji_05_neg_one_365 = -1.0f;

f32 fd_aji_05___local_PI_366 = 3.1415927f;

f32 fd_aji_05___local_3PIO2_368 = 4.712389f;

f32 fd_aji_05_one_negone_359[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_aji_05___local_PIO2_367 = 1.5707964f;

f32 fd_aji_05_one_364 = 1.0f;

static EVT_BEGIN(e_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.25f))
    CALL(evt_mapobj_trans, 0, STRING("S_door1_1"), 0, 0, LW(0))
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_trans, 0, STRING("S_door1_2"), 0, 0, LW(0))
    RETURN
EVT_END()

static EVT_BEGIN(e_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.25f))
    CALL(evt_mapobj_trans, 0, STRING("S_door1_1"), 0, 0, LW(0))
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_trans, 0, STRING("S_door1_2"), 0, 0, LW(0))
    RETURN
EVT_END()

static BeroEntry bero_entry_data[] = {
    {
        "e_bero", // hitName
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
        "w_bero_3", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        e_door_close, // enterAnimeArg
        e_door_open, // exitAnimeArg
    },
    {0},
};

static EVT_BEGIN(evt_box)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mobj_get_position, STRING("box"), LW(0), LW(1), LW(2))
    CALL(evt_item_entry, STRING("dummy"), 29, LW(0), LW(1), LW(2), 17, -1, 0)
    CALL(evt_mobj_wait_animation_end, STRING("box"))
    CALL(evt_item_get_item, STRING("dummy"))
    CALL(evt_hitobj_attr_onoff, 1, 0, STRING("A_no1"), 2048)
    SET_VALUE(GSWF(4182), 1)
    WAIT_FRAMES(1)
    CALL_CHILD(PTR(hikaru_off))
    BEGIN_INLINE_EVENT
        WAIT_MS(1000)
        CALL(evt_mario_set_dir, 90, 300, 0)
    END_INLINE_EVENT
    LOOP(5)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("yukayu1"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("yukayu2"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("ato"), 1)
        WAIT_MS(150)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("yukayu1"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("yukayu2"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("ato"), 1)
        WAIT_MS(150)
    END_LOOP
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(hikaru_on)
    CALL(_hikaru_on)
    RETURN
EVT_END()

static EVT_BEGIN(hikaru_off)
    CALL(_hikaru_off)
    RETURN
EVT_END()

static HitDamageReturn dmgReturnPt[] = {
    {"mod_01", {0.0f, -1000.0f, 0.0f}},
    {0},
};

static EvtWord hikaru_yuka_init[] = {
    CALL_CHILD(hikaru_off)
    CALL(evt_hitobj_attr_onoff, 1, 1, STRING("A_no1"), 2048)
    CALL(evt_mobj_itembox, STRING("box"), -255, 10, -10, 2, 0, PTR(evt_box), GSWF(4178))
    CALL(evt_hit_damage_return_set, PTR(dmgReturnPt))
    BEGIN_INLINE_EVENT
        CALL(ride_chk)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(thunder_main)
    END_INLINE_EVENT
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("yukayu1"), 1)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("yukayu2"), 1)
    CALL(evt_map_playanim, STRING("denyuka"), 1, 0)
    OPCODE_END_SCRIPT,
    RETURN
};

EVT_BEGIN(aji_05_init_evt)
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG7_DUN2"))
    CALL(evt_snd_envon, 272, STRING("ENV_STG7_AJI1"))
    CALL(evt_snd_set_rev_mode, 1)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("no1"), 1)
    IF_INT_EQUAL(GSWF(4182), 0)
        CALL_CHILD(PTR(hikaru_yuka_init))
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("ato"), 1)
        ELSE
        CALL_CHILD(PTR(hikaru_off))
        CALL(evt_hitobj_attr_onoff, 1, 0, STRING("A_no1"), 2048)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("yukayu1"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("yukayu2"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("ato"), 1)
    END_IF
    CALL(evt_mobj_badgeblk, STRING("mobj00"), -75, 60, -125, 138, 0, GSWF(4212), 0)
    SPAWN_EVENT(PTR(evt_emergency))
    RETURN
EVT_END()

static USER_FUNC(ride_chk) {
    static s32 onoff[24];
    char hitName[64];
    char name[64];
    MarioWork* mario = marioGetPtr();
    PartyEntry* party = partyGetPtr(0);
    HitEntry* hit;
    s32 i;

    if (isFirstCall) {
        memset(onoff, 0, sizeof(onoff));
    }
    if (party != NULL && party->currentMemberId == 4 && (party->flags & 0x100)) {
        hit = mario->hitobjStandOn;
        if (hit == NULL) {
            hit = mario->hitobjJumpFrom;
        }
    } else {
        hit = (HitEntry*)mario->hitobjUnder;
    }
    // panels stay lit for 30 frames after being touched
    for (i = 0; i < 24; i++) {
        if (hit != NULL) {
            sprintf(hitName, "A_hikaru_%02d", i + 1);
            if (strcmp(hit->joint->name, hitName) == 0) {
                onoff[i] = 30;
            }
        }
    }
    for (i = 0; i < 24; i++) {
        if (onoff[i] > 0) {
            onoff[i]--;
        }
        if (onoff[i] != 0) {
            sprintf(name, "hikaru_%02d", i + 1);
            mapGrpFlagOff(name, 1);
            sprintf(name, "nashi_%02d", i + 1);
            mapGrpFlagOn(name, 1);
        } else {
            sprintf(name, "hikaru_%02d", i + 1);
            mapGrpFlagOn(name, 1);
            sprintf(name, "nashi_%02d", i + 1);
            mapGrpFlagOff(name, 1);
        }
    }
    if (evtGetValue(event, GSWF(4182))) {
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
        if (mario->currMotionId == 0x20) {
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
    } else if (mario->currMotionId != 0x20) {
        event->userdata[0] = 0;
    }
    return EVT_RETURN_BLOCK;
}

USER_FUNC(_hikaru_off) {
    char name[64];
    s32 i;

    for (i = 1; i < 25; i++) {
        sprintf(name, "hikaru_%02d", i);
        mapGrpFlagOn(name, 1);
        sprintf(name, "nashi_%02d", i);
        mapGrpFlagOn(name, 1);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(_hikaru_on) {
    char name[64];
    s32 i;

    for (i = 1; i < 25; i++) {
        sprintf(name, "hikaru_%02d", i);
        mapGrpFlagOn(name, 1);
        sprintf(name, "nashi_%02d", i);
        mapGrpFlagOff(name, 1);
    }
    return EVT_RETURN_DONE;
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
        if (event->userdata[1] >= 25) {
            event->userdata[1] = 0;
            event->userdata[2]++;
            if (event->userdata[2] > 1) {
                return EVT_RETURN_DONE;
            }
        }
    }
    for (i = 1; i < 25; i++) {
        if (i == event->userdata[1]) {
            sprintf(name, "hikaru_%02d", i);
            mapGrpFlagOff(name, 1);
            sprintf(name, "nashi_%02d", i);
            mapGrpFlagOn(name, 1);
        } else {
            sprintf(name, "hikaru_%02d", i);
            mapGrpFlagOn(name, 1);
            sprintf(name, "nashi_%02d", i);
            mapGrpFlagOff(name, 1);
        }
    }
    return EVT_RETURN_BLOCK;
}

