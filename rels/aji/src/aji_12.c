// aji_12 (敵のアジト, X-Naut Fortress): an electrified floor-panel room (hikaru_yuka) with 44 panels.

#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "driver/mapdrv.h"
#include "libc_decls.h"
#include "evt/evt_hit.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_mobj.h"
#include "evt/evt_snd.h"
#include "party/party_motion.h"
#include "evt/evt_item.h"
#include "mario/mario_pouch.h"
#include "driver/itemdrv.h"

#include "mario/mario.h"
#include "pmario_sound.h"
#include "eff/n64/eff_akari_charge_n64.h"
#include "party.h"
#include "driver/hitdrv.h"
#include "stdio.h"
#include "string.h"
#include "evt/evt_bero.h"
#include "aji.h"
#include "manager/evtmgr_cmd.h"


static USER_FUNC(hikaru_off);
static USER_FUNC(mario_chk);
static USER_FUNC(thunder_main);

static EvtWord e_door_close[22];
static EvtWord e_door_open[22];
static BeroEntry bero_entry_data[2];
const char* hit_list[50];
static EvtWord evt_box[89];
static HitDamageReturn dmgReturnPt[2];
static EvtWord hikaru_yuka_init[40];
#if VERSION <= VERSION_DEMO_US
extern EvtWord aji_12_init_evt[];
#else
EvtWord aji_12_init_evt[71];
#endif

/**
 * Door close animation step: moves the two door leaves S_door1_1
 * and S_door1_2 in opposite directions along Z by the animation value in LW(0) times -0.25;
 * the open and close scripts apply the same transform. The demo builds use -0.2217.
 */
static EVT_BEGIN(e_door_close)
#if VERSION <= VERSION_DEMO_US
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.22167969f))
#else
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.25f))
#endif
    CALL(evt_mapobj_trans, 0, STRING("S_door1_1"), 0, 0, LW(0))
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_trans, 0, STRING("S_door1_2"), 0, 0, LW(0))
    RETURN
EVT_END()

/**
 * Door open animation step: moves the two door leaves S_door1_1
 * and S_door1_2 in opposite directions along Z by the animation value in LW(0) times -0.25;
 * the open and close scripts apply the same transform. The demo builds use -0.2217.
 */
static EVT_BEGIN(e_door_open)
#if VERSION <= VERSION_DEMO_US
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.22167969f))
#else
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.25f))
#endif
    CALL(evt_mapobj_trans, 0, STRING("S_door1_1"), 0, 0, LW(0))
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_trans, 0, STRING("S_door1_2"), 0, 0, LW(0))
    RETURN
EVT_END()

/** Exit of this room: the east door to aji_01. */
static BeroEntry bero_entry_data[] = {
    {
        "e_bero", // hitName
        1, // type
        10, // sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "aji_01", // nextMapName
        "w_bero_4", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        e_door_close, // enterAnimeArg
        e_door_open, // exitAnimeArg
    },
    {0},
};

/** Hit areas of the electrified floor panels, in panel order. */
const char* hit_list[] = {
    "A_hikaru_01",
    "A_hikaru_02",
    "A_hikaru_03",
    "A_hikaru_04",
    "A_hikaru_05",
    "A_hikaru_06",
    "A_hikaru_07",
    "A_hikaru_08",
    "A_hikaru_09",
    "A_hikaru_10",
    "A_hikaru_11",
    "A_hikaru_12",
    "A_hikaru_13",
    "A_hikaru_14",
    "A_hikaru_15",
    "A_hikaru_16",
    "A_hikaru_17",
    "A_hikaru_18",
    "A_hikaru_19",
    "A_hikaru_20",
    "A_hikaru_21",
    "A_hikaru_22",
    "A_hikaru_23",
    "A_hikaru_24",
    "A_hikaru_25",
    "A_hikaru_26",
    "A_hikaru_27",
    "A_hikaru_08",
    "A_hikaru_28",
    "A_hikaru_29",
    "A_hikaru_30",
    "A_hikaru_31",
    "A_hikaru_32",
    "A_hikaru_12",
    "A_hikaru_13",
    "A_hikaru_33",
    "A_hikaru_15",
    "A_hikaru_34",
    "A_hikaru_35",
    "A_hikaru_36",
    "A_hikaru_37",
    "A_hikaru_38",
    "A_hikaru_39",
    "A_hikaru_40",
    "A_hikaru_41",
    "A_hikaru_42",
    "A_hikaru_41",
    "A_hikaru_40",
    "A_hikaru_43",
    "A_hikaru_44",
};

/**
 * Item-box script for the chest at "box": gives Mario the card key (ITEM_CARD_KEY_0x1E),
 * sets GSWF_AJI12_CARD_KEY_TAKEN, turns off the floor lights and makes Mario turn to face east.
 */
static EVT_BEGIN(evt_box)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mobj_get_position, STRING("box"), LW(0), LW(1), LW(2))
    CALL(evt_item_entry, STRING("dummy"), ITEM_CARD_KEY_0x1E, LW(0), LW(1), LW(2), ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
    CALL(evt_mobj_wait_animation_end, STRING("box"))
    CALL(evt_item_get_item, STRING("dummy"))
    SET_VALUE(GSWF_AJI_12_LIT_FLOOR_DONE, 1)
    WAIT_FRAMES(2)
    CALL(hikaru_off)
    BEGIN_INLINE_EVENT
        WAIT_MS(1000)
        CALL(evt_mario_set_dir, 90, 300, 0)
    END_INLINE_EVENT
    LOOP(5)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("yukayu1"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("yukayu2"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("ato"), MAP_OBJ_FLAG_HIDDEN)
        WAIT_MS(150)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("yukayu1"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("yukayu2"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("ato"), MAP_OBJ_FLAG_HIDDEN)
        WAIT_MS(150)
    END_LOOP
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/** Where Mario is put back after taking damage from the electrified floor: the map position named mod_01. */
static HitDamageReturn dmgReturnPt[] = {
    {"mod_01", {0.0f, -1000.0f, 0.0f}},
    {0},
};

/**
 * Sets up the electrified-floor puzzle: places the chest, sets the damage return point,
 * starts the thunder effect (thunder_main) and the Mario position checker (mario_chk),
 * and starts the floor animation denyuka.
 */
static EVT_BEGIN(hikaru_yuka_init)
    CALL(evt_mobj_itembox, STRING("box"), -350, 10, -10, MOBJ_ITEMBOX_GRAY, 0, PTR(evt_box), GSWF_AJI_12_CARD_KEY_CHEST)
    CALL(evt_hit_damage_return_set, PTR(dmgReturnPt))
    BEGIN_INLINE_EVENT
        CALL(thunder_main)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(mario_chk)
    END_INLINE_EVENT
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("yukayu1"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("yukayu2"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_map_playanim, STRING("denyuka"), MAP_ANIM_PLAY_LOOP, 0)
    RETURN
EVT_END()

/**
 * Init script of aji_12, a glowing-floor room with 44 panels:
 * plays BGM_STG7_DUN2, sets up the exits and an HP Drain badge block, and,
 * until the card key from the chest has been taken (GSWF_AJI12_CARD_KEY_TAKEN),
 * runs the floor puzzle (hikaru_yuka_init); afterwards the floor is switched off.
 */
EVT_BEGIN(aji_12_init_evt)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG7_DUN2"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG7_AJI1"))
#if VERSION > VERSION_DEMO_US
    CALL(evt_snd_set_rev_mode, 1)
#endif
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("no1"), MAP_OBJ_FLAG_HIDDEN)
    IF_INT_EQUAL(GSWF_AJI_12_LIT_FLOOR_DONE, 0)
        CALL_CHILD(PTR(hikaru_yuka_init))
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("ato"), MAP_OBJ_FLAG_HIDDEN)
    ELSE
        CALL(hikaru_off)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("yukayu1"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("yukayu2"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("ato"), MAP_OBJ_FLAG_HIDDEN)
    END_IF
    CALL(evt_mobj_badgeblk, STRING("mobj00"), 170, 60, 120, ITEM_HP_DRAIN_ITEM, 0, GSWF_AJI_12_HP_DRAIN_BLOCK, 0)
    SPAWN_EVENT(PTR(evt_emergency))
    RETURN
EVT_END()

/**
 * Script function that plays an electric shock when Mario is in the spike-damage motion
 * (MARIO_MOTION_DAMAGE_TOGE): SFX_STG7_ELECTRIC_SHOCK1 and a blue-green spark effect at his position,
 * once per hit.
 */
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

static USER_FUNC(mario_chk) {
    s32 n;
    char name[256];
    MarioWork* mario = marioGetPtr();
    PartyEntry* party;
    HitEntry* hit;
    s32 i;
    s32 j;

    if (isFirstCall) {
        event->userdata[0] = 0;
        event->userdata[1] = 0;
    }
    if (evtGetValue(NULL, GSWF_AJI_12_LIT_FLOOR_DONE)) {
        return EVT_RETURN_DONE;
    }
    for (i = 1; i <= 44; i++) {
        sprintf(name, "hikaru_%02d", i);
        mapGrpFlagOn(name, MAP_OBJ_FLAG_HIDDEN);
        sprintf(name, "nashi_%02d", i);
        mapGrpFlagOff(name, MAP_OBJ_FLAG_HIDDEN);
    }
    for (i = 0; i < 50; i++) {
        hitGrpAttrOn((const char*)hit_list[i], 0x800);
    }
#if VERSION <= VERSION_DEMO_US
    if (mario->hitobjUnder == NULL) {
#else
    party = partyGetPtr(0);
    if (party != NULL && party->currentMemberId == PARTY_MEMBER_YOSHI && (party->flags & PARTY_FLAG_IS_BEING_USED)) {
        hit = mario->hitobjStandOn;
        if (hit == NULL) {
            hit = mario->hitobjJumpFrom;
        }
    } else {
        hit = (HitEntry*)mario->hitobjUnder;
    }
    if (hit == NULL) {
#endif
        mapGrpFlagOff("hikaru_01", MAP_OBJ_FLAG_HIDDEN);
        mapGrpFlagOn("nashi_01", MAP_OBJ_FLAG_HIDDEN);
        hitGrpAttrOff("A_hikaru_01", 0x800);
        event->userdata[2] = 0;
#if VERSION <= VERSION_DEMO_US
    } else if (strncmp(hitGetName(mario->hitobjUnder), "A_hikaru_", strlen("A_hikaru_")) != 0) {
#else
    } else if (strncmp(hitGetName(hit), "A_hikaru_", strlen("A_hikaru_")) != 0) {
#endif
        mapGrpFlagOff("hikaru_01", MAP_OBJ_FLAG_HIDDEN);
        mapGrpFlagOn("nashi_01", MAP_OBJ_FLAG_HIDDEN);
        hitGrpAttrOff("A_hikaru_01", 0x800);
        event->userdata[2] = 0;
#if VERSION <= VERSION_DEMO_US
    } else if (strcmp(hitGetName(mario->hitobjUnder), "A_hikaru_01") == 0 && event->userdata[2] == 0) {
#else
    } else if (strcmp(hitGetName(hit), "A_hikaru_01") == 0 && event->userdata[2] == 0) {
#endif
        event->userdata[2] = 1;
        event->userdata[0] = 0;
        event->userdata[1] = 0;
    }
    // The ordered route revisits panels; clamp its final step rather than wrapping this lookup.
    n = 0;
    j = event->userdata[0];
    if (j >= 50) {
        j = 49;
    }
    sscanf((const char*)hit_list[j], "A_hikaru_%02d", &n);
    sprintf(name, "hikaru_%02d", n);
    mapGrpFlagOff(name, MAP_OBJ_FLAG_HIDDEN);
    sprintf(name, "nashi_%02d", n);
    mapGrpFlagOn(name, MAP_OBJ_FLAG_HIDDEN);
    sprintf(name, "A_hikaru_%02d", n);
    hitGrpAttrOff(name, 0x800);
    j++;
    if (j >= 50) {
        j = 49;
    }
    sscanf((const char*)hit_list[j], "A_hikaru_%02d", &n);
    sprintf(name, "hikaru_%02d", n);
    mapGrpFlagOff(name, MAP_OBJ_FLAG_HIDDEN);
    sprintf(name, "nashi_%02d", n);
    mapGrpFlagOn(name, MAP_OBJ_FLAG_HIDDEN);
    sprintf(name, "A_hikaru_%02d", n);
    hitGrpAttrOff(name, 0x800);
#if VERSION >= VERSION_US
    // This version exposes a third route entry; the other version exposes two.
    j++;
    if (j >= 50) {
        j = 49;
    }
    sscanf((const char*)hit_list[j], "A_hikaru_%02d", &n);
    sprintf(name, "hikaru_%02d", n);
    mapGrpFlagOff(name, MAP_OBJ_FLAG_HIDDEN);
    sprintf(name, "nashi_%02d", n);
    mapGrpFlagOn(name, MAP_OBJ_FLAG_HIDDEN);
    sprintf(name, "A_hikaru_%02d", n);
    hitGrpAttrOff(name, 0x800);
#endif
    event->userdata[1]++;
    // The strict threshold advances after 21 or 61 calls, depending on whether the start was entered.
    if (event->userdata[1] > (event->userdata[2] == 0 ? 20 : 60)) {
        event->userdata[1] = 0;
        event->userdata[0]++;
        if (event->userdata[0] > 50) {
            event->userdata[0] = 0;
        }
    }
    return EVT_RETURN_BLOCK;
}

static USER_FUNC(hikaru_off) {
    char name[256];
    s32 i;

    for (i = 1; i <= 44; i++) {
        sprintf(name, "hikaru_%02d", i);
        mapGrpFlagOn(name, MAP_OBJ_FLAG_HIDDEN);
        sprintf(name, "nashi_%02d", i);
        mapGrpFlagOn(name, MAP_OBJ_FLAG_HIDDEN);
        sprintf(name, "A_hikaru_%02d", i);
        hitGrpAttrOff(name, 0x800);
    }
    hitGrpAttrOff("A_no1", 0x800);
    return EVT_RETURN_DONE;
}

