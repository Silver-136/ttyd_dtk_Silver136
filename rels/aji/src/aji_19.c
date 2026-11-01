#include "version.h"
#include "mario/mariost.h"
// aji_19 (敵のアジト, X-Naut Fortress): the room reached from the Moon (moo_02), with the first-visit scene
// (evt_hajimete).

#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_msg.h"
#include "evt/evt_party.h"
#include "evt/evt_seq.h"
#include "evt/evt_snd.h"
#include "party/party_motion.h"
#include "evt/evt_telop.h"
#include "driver/mapdrv.h"
#include "pmario_sound.h"

#include "mario/mario.h"
#include "mario/mario_party.h"
#include "party.h"
#include "driver/hitdrv.h"
#include "string.h"
#include "evt/evt_bero.h"
#include "driver/seqdrv.h"

BOOL hitIsGroup(HitEntry* target, HitEntry* group);
static BOOL _hitIsGroupSub(HitEntry* target, HitEntry* node, BOOL checkSibling);

static USER_FUNC(mario_chk);
static BOOL hitIsGroup(HitEntry* target, HitEntry* group);

#if VERSION <= VERSION_DEMO_US
static EvtWord w_door_close[];
#else
static EvtWord w_door_close[22];
#endif
#if VERSION <= VERSION_DEMO_US
static EvtWord w_door_open[];
#else
static EvtWord w_door_open[22];
#endif
#if VERSION <= VERSION_DEMO_US
static EvtWord e_door_close[];
#else
static EvtWord e_door_close[22];
#endif
#if VERSION <= VERSION_DEMO_US
static EvtWord e_door_open[];
#else
static EvtWord e_door_open[22];
#endif
static BeroEntry bero_entry_data[3];
EvtWord evt_hajimete[33];
#if VERSION <= VERSION_DEMO_US
extern EvtWord aji_19_init_evt[];
#else
EvtWord aji_19_init_evt[56];
#endif

static EVT_BEGIN(w_door_close)
#if VERSION <= VERSION_DEMO_US
MULTIPLY_FLOAT(LW(0), FLOAT(0.22167969f))
#else
    MULTIPLY_FLOAT(LW(0), FLOAT(0.25f))
#endif
    CALL(evt_mapobj_trans, 0, STRING("S_door1_1"), 0, 0, LW(0))
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_trans, 0, STRING("S_door1_2"), 0, 0, LW(0))
    RETURN
EVT_END()

static EVT_BEGIN(w_door_open)
#if VERSION <= VERSION_DEMO_US
MULTIPLY_FLOAT(LW(0), FLOAT(0.22167969f))
#else
    MULTIPLY_FLOAT(LW(0), FLOAT(0.25f))
#endif
    CALL(evt_mapobj_trans, 0, STRING("S_door1_1"), 0, 0, LW(0))
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_trans, 0, STRING("S_door1_2"), 0, 0, LW(0))
    RETURN
EVT_END()

static EVT_BEGIN(e_door_close)
#if VERSION <= VERSION_DEMO_US
MULTIPLY_FLOAT(LW(0), FLOAT(-0.22167969f))
#else
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.25f))
#endif
    CALL(evt_mapobj_trans, 0, STRING("S_door2_1"), 0, 0, LW(0))
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_trans, 0, STRING("S_door2_2"), 0, 0, LW(0))
    RETURN
EVT_END()

static EVT_BEGIN(e_door_open)
#if VERSION <= VERSION_DEMO_US
MULTIPLY_FLOAT(LW(0), FLOAT(-0.22167969f))
#else
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.25f))
#endif
    CALL(evt_mapobj_trans, 0, STRING("S_door2_1"), 0, 0, LW(0))
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_trans, 0, STRING("S_door2_2"), 0, 0, LW(0))
    RETURN
EVT_END()

/** Exits of this room: the west door to moo_02 and the east door to aji_00. */
static BeroEntry bero_entry_data[] = {
    {
        "w_bero", // hitName
        1, // type
        10, // sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "moo_02", // nextMapName
        "n_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        w_door_close, // enterAnimeArg
        w_door_open, // exitAnimeArg
    },
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
        "aji_00", // nextMapName
        "w_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        e_door_close, // enterAnimeArg
        e_door_open, // exitAnimeArg
    },
    {0},
};

/**
 * First-visit scene of the fortress (aji_19, before STG7_07_01_INSIDE_THE_BUILDING):
 * the partner comments (stg7_aji_00_00_01), the story becomes STG7_07_01_INSIDE_THE_BUILDING
 * and the area name telop is shown.
 */
EVT_BEGIN(evt_hajimete)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_bero_exec_wait, 65536)
    WAIT_MS(1000)
    CALL(evt_party_stop, 0)
    WAIT_MS(200)
    CALL(evt_msg_print_party, STRING("stg7_aji_00_00_01"))
    CALL(evt_party_run, 0)
    SET_VALUE(GSW_STORY_PROGRESS, STG7_07_01_INSIDE_THE_BUILDING)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_telop_entry, 15)
    RETURN
EVT_END()

/**
 * Init script of aji_19, the entrance room of the fortress:
 * plays BGM_STG7_DUN1, sets up the exits and the conveyor check, and runs evt_hajimete on the first visit.
 */
EVT_BEGIN(aji_19_init_evt)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG7_DUN1"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG7_AJI1"))
#if VERSION > VERSION_DEMO_US
    CALL(evt_snd_set_rev_mode, 1)
#endif
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
#if VERSION <= VERSION_DEMO_US
    CALL_CHILD(evt_bero_info_run)
#else
    CALL_CHILD(PTR(evt_bero_info_run))
#endif
    CALL(evt_map_playanim, STRING("hosi_01"), MAP_ANIM_PLAY_LOOP, 0)
    CALL(evt_map_playanim, STRING("hosi_02"), MAP_ANIM_PLAY_LOOP, 0)
    CALL(evt_map_playanim, STRING("hosi_03"), MAP_ANIM_PLAY_LOOP, 0)
    CALL(evt_map_playanim, STRING("yuka"), MAP_ANIM_PLAY_LOOP, 0)
    BEGIN_INLINE_EVENT
        CALL(mario_chk)
    END_INLINE_EVENT
#if VERSION <= VERSION_DEMO_US
    IF_INT_LESS(GSW(GSW_STORY_PROGRESS), STG7_07_01_INSIDE_THE_BUILDING)
        SPAWN_EVENT(evt_hajimete)
#else
    IF_INT_LESS(GSW_STORY_PROGRESS, STG7_07_01_INSIDE_THE_BUILDING)
        SPAWN_EVENT(PTR(evt_hajimete))
#endif
    END_IF
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("kanaami2"), MAP_OBJ_FLAG_CLIP)
    RETURN
EVT_END()

/** Conveyor push per frame: PAL scales the 60 Hz speed to the frame rate. */
static inline f32 beltSpeed(void) {
#if VERSION == VERSION_EU
    return 60.0f / gp->framerate;
#else
    return 1.0f;
#endif
}

static USER_FUNC(mario_chk) {
    MarioWork* mario = marioGetPtr();
    PartyEntry* party;

    if (mario != NULL && mario->hitobjStandOn != NULL) {
        if (hitIsGroup(mario->hitobjStandOn, hitNameToPtr("A_b_ue"))) {
            mario->floorMove.x = beltSpeed();
        } else if (hitIsGroup(mario->hitobjStandOn, hitNameToPtr("A_b_shita"))) {
            mario->floorMove.x = -beltSpeed();
        }
    }
    party = partyGetPtr(marioGetPartyId());
    if (party != NULL && party->hitobjStandOn != NULL) {
        if (hitIsGroup(party->hitobjStandOn, hitNameToPtr("A_b_ue"))) {
            party->floorMoveX = beltSpeed();
        }
        if (hitIsGroup(party->hitobjStandOn, hitNameToPtr("A_b_shita"))) {
            party->floorMoveX = -beltSpeed();
        }
    }
    party = partyGetPtr(marioGetExtraPartyId());
    if (party != NULL && party->hitobjStandOn != NULL) {
        if (hitIsGroup(party->hitobjStandOn, hitNameToPtr("A_b_ue"))) {
            party->floorMoveX = beltSpeed();
        }
        if (hitIsGroup(party->hitobjStandOn, hitNameToPtr("A_b_shita"))) {
            party->floorMoveX = -beltSpeed();
        }
    }
    return EVT_RETURN_BLOCK;
}

/** Whether the hit area target is group or one of its descendants. */
static BOOL hitIsGroup(HitEntry* target, HitEntry* group) {
    // Compare joint names through the child-first search, excluding the root's siblings.
    return _hitIsGroupSub(target, group, FALSE);
}

/**
 * Recursive helper of hitIsGroup: whether the joint name of target equals that of node,
 * of any of node's descendants, or (when checkSibling is TRUE) of the siblings that follow.
 */
static BOOL _hitIsGroupSub(HitEntry* target, HitEntry* node, BOOL checkSibling) {
    if (strcmp(target->joint->name, node->joint->name) == 0) {
        return TRUE;
    }
    // A child search returns immediately, so this node's siblings are not searched afterward.
    if (node->child != NULL) {
        return _hitIsGroupSub(target, node->child, TRUE);
    }
    if (checkSibling && node->sibling != NULL) {
        return _hitIsGroupSub(target, node->sibling, TRUE);
    }
    return FALSE;
}

