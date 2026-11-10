// win_03 (ウィンディん家周辺, Boggly Woods): the path between win_02 and Flurrie's house (win_04), with two Piranha Plants.

#include "version.h"
#include "battle/battle_unit.h"
#include "driver/fadedrv.h"
#include "evt/evt_cmd.h"
#include "evt/evt_cam.h"
#include "evt/evt_env.h"
#include "evt/evt_fade.h"
#include "evt/evt_mario.h"
#include "evt/evt_msg.h"
#include "evt/evt_party.h"
#include "evt/evt_seq.h"
#include "evt/evt_snd.h"
#include "evt/evt_item.h"
#include "driver/mapdrv.h"
#include "mario/mario_pouch.h"
#include "driver/casedrv.h"
#include "pmario_sound.h"
#include "driver/itemdrv.h"

#include "driver/camdrv.h"
#include "mario/mariost.h"
#include "evt/evt_bero.h"
#include "evt/evt_hit.h"
#include "evt/evt_map.h"
#include "evt/evt_mobj.h"
#include "evt/evt_npc.h"
#include "evt/evt_case.h"
#include "evt/evt_sub.h"
#include "party/party_motion.h"
#include "data_tables.h"
#include "driver/seqdrv.h"
#include "driver/npcdrv.h"

static USER_FUNC(camera_action);

static EvtWord clouda_house_in[12];
static EvtWord clouda_house_out[19];
static EvtWord cloud_out[12];
static EvtWord cloud_in[12];
static BeroEntry bero_entry_data[5];

#if VERSION <= VERSION_DEMO_JP
extern EvtWord lookat_house[];
#else
EvtWord lookat_house[45];
#endif

static EvtWord cloud_toujou[76];
static EvtWord cloud_lostitem[215];
static EvtWord tree_evt_01[20];
static TreeAccess tree_access_data[5];
static EvtWord dummy_event[4];

#if VERSION <= VERSION_DEMO_JP
extern EvtWord win_03_init_evt[];
#else
EvtWord win_03_init_evt[108];
#endif

/**
 * Bero-style script that nothing in the tree references: locks Mario's keys
 * and changes the map to win_04 at its "w_bero" entry (Flurrie's house interior); returns LW(0) = 1.
 */
static EVT_BEGIN(clouda_house_in)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_bero_mapchange, STRING("win_04"), STRING("w_bero"))
    SET_VALUE(LW(0), 1)
    RETURN
EVT_END()

/**
 * Bero script that nothing in the tree references:
 * turns on Mario's background mode, removes the partner, turns off the camera's y follow,
 * places Mario at (277, 54, -450), next to Flurrie's door at (306, 23, -448), and runs bero_case_entry;
 * returns LW(0) = 1.
 * Guess: an unused variant for coming out of Flurrie's house.
 */
static EVT_BEGIN(clouda_house_out)
    CALL(evt_mario_bgmode_on)
    CALL(evt_mario_kill_party, 0)
    CALL(evt_mario_cam_y_off)
    CALL(evt_mario_set_pos, 277, 54, -450)
    CALL_CHILD(bero_case_entry)
    SET_VALUE(LW(0), 1)
    RETURN
EVT_END()

/** Enter script of the "n_bero" entry (Mario arrives back from Flurrie's house): plays the wooden door-shut sound at her door (306, 23, -448); returns LW(0) = 0. */
static EVT_BEGIN(cloud_out)
    CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_SHUT_WOOD2"), 306, 23, -448, 0)
    SET_VALUE(LW(0), 0)
    RETURN
EVT_END()

/** Exit script of the "n_bero" entry (Mario leaves for Flurrie's house): plays the wooden door-open sound at her door (306, 23, -448); returns LW(0) = 0. */
static EVT_BEGIN(cloud_in)
    CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_OPEN_WOOD3"), 306, 23, -448, 0)
    SET_VALUE(LW(0), 0)
    RETURN
EVT_END()

/**
 * Exits of this map: west to win_02, the pipe pair dokan_1/dokan_2 within this map,
 * and Flurrie's door (n_bero) to win_04, which plays the wooden door sounds.
 */
static BeroEntry bero_entry_data[] = {
    {
        "w_bero", // hitName
        0, 0, // type..sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "win_02", // nextMapName
        "e_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "dokan_1", // hitName
        2, // type
        0, // sfxId
        BERO_DIR_DOWN, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        NULL, 0, // exitEvent..nextMapName
        "dokan_2", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "dokan_2", // hitName
        4098, // type
        0, // sfxId
        BERO_DIR_DOWN, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        NULL, 0, // exitEvent..nextMapName
        "dokan_1", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "n_bero", // hitName
        4099, // type
        0, // sfxId
        BERO_DIR_E, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        cloud_out, // enterEvent
        5, // caseType
        cloud_in, // exitEvent
        "win_04", // nextMapName
        "w_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

/**
 * Trigger: when Mario walks past x = 200 the extra partner comments on Flurrie's house ("stg2_win_e04_02")
 * and the story becomes STG2_04_EXTENDING_PLANE_PANEL.
 */
EVT_BEGIN(lookat_house)
    LOOP(0)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        IF_INT_GREATER_OR_EQUAL(LW(0), 200)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_npc_change_fbat_mode, FBAT_MODE_EVENT)
    CALL(evt_npc_stop_for_event)
#if VERSION >= VERSION_JP
    CALL(evt_mario_normalize)
#endif
    CALL(evt_msg_print, 0, STRING("stg2_win_e04_02"), 0, STRING("extparty"))
    SET_VALUE(GSW_STORY_PROGRESS, STG2_04_EXTENDING_PLANE_PANEL)
    CALL(evt_npc_change_fbat_mode, FBAT_MODE_FIELD)
    CALL(evt_npc_start_for_event)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static USER_FUNC(camera_action) {
    CameraEntry* cam = camGetPtr(CAMERA_3D);
    Vec pos = {290.0f, 150.0f, 491.0f};
    Vec at = {290.0f, 90.0f, -6.0f};
    f32 x;

    if (isFirstCall) {
        f32 speed;
        f32 dist;

        event->lwDataF[0] = 0.0f;
        event->lwDataF[1] = 2.5f;
        event->lwData[2] = 0;
        speed = 0.0f;
        dist = 0.0f;
        while (TRUE) {
            if (speed >= 2.5f) {
                break;
            }
            speed += 0.01f;
            dist += speed;
        }
        event->lwDataF[2] = dist - 2.5f;
    }
    if (event->lwDataF[0] < 300.0f - event->lwDataF[2]) {
        event->lwDataF[0] += event->lwDataF[1];
        x = -10.0f + event->lwDataF[0];
        event->lwData[10] = 0;
        at.x = pos.x = x;
    } else if (event->lwDataF[0] < 300.0f) {
        event->lwDataF[0] += event->lwDataF[1];
        event->lwDataF[1] -= 0.01f;
        x = -10.0f + event->lwDataF[0];
        event->lwData[10] = 0;
        at.x = pos.x = x;
    } else {
        event->lwData[10] = 1;
    }
    cam->presetPos = cam->cameraPos;
    cam->presetAt = cam->cameraAt;
    cam->storedPos = pos;
    cam->storedAt = at;
    cam->startTimeA = gp->renderTime;
    cam->durationA = 0;
    cam->stateA = 3;
    cam->curveA = EASING_SINE_IN_OUT;
    return event->lwData[10] == 0 ? EVT_RETURN_BLOCK : EVT_RETURN_DONE;
}

/**
 * First part of Flurrie's intro, spawned by the init script at STG2_03_02:
 * Mario and the partners are moved off the map, the camera pans to her house (camera_action)
 * and a voice without a speaker tail praises the woods and the house she built there (stg2_win_e04_00_01);
 * then the map changes to win_04 with an iris wipe.
 */
static EVT_BEGIN(cloud_toujou)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_party_cont_onoff, 1, 0)
    CALL(evt_mario_set_pos, 0, -1000, 0)
    CALL(evt_party_set_pos, 0, 0, -1000, 0)
    CALL(evt_party_set_pos, 1, 0, -1000, 0)
    CALL(evt_npc_change_fbat_mode, FBAT_MODE_EVENT)
    CALL(evt_npc_stop_for_event)
    CALL(evt_cam3d_evt_set, -10, 150, 491, -10, 90, -6, 0, EASING_SINE_IN_OUT)
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(camera_action)
    CALL(evt_msg_toge, MSG_TOGE_NONE, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING("stg2_win_e04_00_01"), 0, 0)
    CALL(evt_fade_set_mapchange_type, 0, FADE_OUT_IRIS, 500, FADE_IN_IRIS, 500)
    CALL(evt_bero_mapchange, STRING("win_04"), STRING(""))
    RETURN
EVT_END()

/**
 * Last part of Flurrie's intro, spawned by the init script at STG2_03_03 (after win_05):
 * with Mario off the map, the camera shakes toward the house with a blur and the surprised jingle,
 * a voice wails that something is gone (stg2_win_e04_00_05),
 * the story becomes STG2_03_04_SHES_LOST_HER_PRECIOUS and the map changes to win_01 ("n_bero").
 */
static EVT_BEGIN(cloud_lostitem)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_party_cont_onoff, 1, 0)
    CALL(evt_mario_set_pos, 0, -1000, 0)
    CALL(evt_party_set_pos, 0, 0, -1000, 0)
    CALL(evt_party_set_pos, 1, 0, -1000, 0)
    CALL(evt_npc_change_fbat_mode, FBAT_MODE_EVENT)
    CALL(evt_npc_stop_for_event)
    CALL(evt_npc_check, STRING("パックン1"), LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_npc_set_position, STRING("パックン1"), 0, -100, 0)
    END_IF
    CALL(evt_npc_check, STRING("パックン2"), LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_npc_set_position, STRING("パックン2"), 0, -100, 0)
    END_IF
    CALL(evt_cam3d_evt_set, 290, 150, 491, 290, 90, -6, 0, EASING_SINE_IN_OUT)
    CALL(evt_seq_wait, SEQ_GAME)
    WAIT_MS(1000)
    SET_VALUE(GF(0), 1)
    BEGIN_INLINE_EVENT
        CALL(evt_env_blur_on, 1, 600)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_cam3d_evt_set, 290, 137, 391, 290, 90, -6, 100, EASING_SINE_IN_OUT)
        WAIT_MS(100)
        CALL(evt_cam3d_evt_set, 290, 150, 491, 290, 90, -6, 100, EASING_SINE_IN_OUT)
        WAIT_MS(100)
        CALL(evt_cam3d_evt_set, 290, 137, 391, 290, 90, -6, 100, EASING_SINE_IN_OUT)
        WAIT_MS(100)
        CALL(evt_cam3d_evt_set, 290, 150, 491, 290, 90, -6, 100, EASING_SINE_IN_OUT)
        WAIT_MS(100)
        CALL(evt_cam3d_evt_set, 290, 137, 391, 290, 90, -6, 100, EASING_SINE_IN_OUT)
        WAIT_MS(100)
        CALL(evt_cam3d_evt_set, 290, 150, 491, 290, 90, -6, 100, EASING_SINE_IN_OUT)
        WAIT_MS(100)
        SET_VALUE(GF(0), 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_snd_bgmoff, PSND_PAUSE)
        CALL(evt_snd_bgmon, PSND_SLOT(1), STRING("BGM_FF_ENV_SURPRISED3"))
        WAIT_MS(3000)
        CALL(evt_snd_bgmoff, PSND_FADEOUT | PSND_SLOT(1))
        CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_FADEIN, 0)
    END_INLINE_EVENT
    CALL(evt_msg_toge, MSG_TOGE_NONE, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING("stg2_win_e04_00_05"), 0, 0)
    WAIT_WHILE(GF(0))
    WAIT_MS(2000)
    SET_VALUE(GSW_STORY_PROGRESS, STG2_03_04_SHES_LOST_HER_PRECIOUS)
    CALL(evt_fade_set_mapchange_type, 0, FADE_OUT_IRIS, 500, FADE_IN_IRIS, 500)
    CALL(evt_bero_mapchange, STRING("win_01"), STRING("n_bero"))
    RETURN
EVT_END()

static EVT_BEGIN(tree_evt_01)
    CALL(evt_item_delete_check, STRING("item_tree01"), LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_item_entry, STRING("item_tree01"), ITEM_STAR_PIECE, -333, 72, -24, ITEM_TYPE_GROUND, GSWF_STARPIECE_42, 0)
    END_IF
    RETURN
EVT_END()

/** Trees of this map that shake when Mario hammers them; the second drops a Star Piece once. */
static TreeAccess tree_access_data[] = {
    {4, "A_tree_01", "S_tree_mki_01", "S_tree_ha01", 0, NULL},
    {4, "A_tree_02", "S_tree_mki_02", "S_tree_ha03", 0, tree_evt_01},
    {4, "A_tree_03", "S_tree_mki_03", "S_tree_ha02", 0, NULL},
    {4, "A_tree_04", "S_tree_mki_04", "S_tree_ha04", 0, NULL},
    {7, NULL, NULL, NULL, 0, NULL},
};

/**
 * Case script the init script registers for pressing A while standing on "n_bero":
 * it only ends the case event, so pressing A there does nothing.
 */
static EVT_BEGIN(dummy_event)
    CALL(evt_exit_case_evt)
    RETURN
EVT_END()

/**
 * Map init script of win_03, outside Flurrie's house:
 * registers exits, the "n_bero" A case, BGM and ambience, trees, a recovery block,
 * the Volt Shroom and a Star Piece,
 * hides the "S_ana" and "S_siro_asu" objects (and the "A_siro_asu" hit),
 * and spawns lookat_house at STG2_03_05,
 * cloud_toujou at STG2_03_02 and cloud_lostitem at STG2_03_03.
 */
EVT_BEGIN(win_03_init_evt)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(evt_bero_info_run)
    CALL(evt_run_case_evt, CASE_STAND_ON_A, 1, STRING("n_bero"), 0, PTR(dummy_event), 0)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG2_WIN1"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG2_WIN1"))
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_UNSILENT, 0)
    SET_VALUE(LW(0), PTR(tree_access_data))
    CALL_CHILD(evt_sub_tree_access_entry)
    CALL(evt_mobj_recovery_blk, STRING("mobj_recover"), 7, 425, 60, 75, 0, 0)
    CALL(evt_item_entry, STRING("i"), ITEM_VOLT_SHROOM, 475, 0, -60, ITEM_TYPE_STILL, GSWF_WIN_03_VOLT_SHROOM, 0)
    CALL(evt_item_entry, STRING("item_00"), ITEM_STAR_PIECE, -150, 0, -30, ITEM_TYPE_STILL, GSWF_STARPIECE_43, 0)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_ana"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_siro_asu"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_hitobj_onoff, STRING("A_siro_asu"), 1, 0)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG2_03_05_TO_FLURRIES_PLACE)
        SPAWN_EVENT(lookat_house)
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG2_03_02_SEALED_ENTRANCE_2)
        CALL(evt_npc_check_delete, STRING("パックン1"))
        SPAWN_EVENT(cloud_toujou)
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG2_03_03_FLURRIE_INTRO)
        CALL(evt_npc_check_delete, STRING("パックン1"))
        SPAWN_EVENT(cloud_lostitem)
    END_IF
    RETURN
#if VERSION <= VERSION_DEMO_JP
EVT_END()
#else
EVT_END()
#endif
