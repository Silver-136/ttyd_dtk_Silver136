// muj_04 (無人島, Keelhaul Key): the rope bridge where Admiral Bobbery (sanders) and the Embers
// (erumosu) attack, and the coconut palm.

#include "version.h"
#include "battle/battle_unit.h"
#include "driver/fadedrv.h"
#include "evt/evt_cmd.h"
#include "evt/evt_hit.h"
#include "evt/evt_item.h"
#include "evt/evt_map.h"
#include "evt/evt_case.h"
#include "evt/evt_snd.h"
#include "party/party_motion.h"
#include "evt/evt_mobj.h"
#include "driver/npcdrv.h"
#include "mario/mario_pouch.h"
#include "system.h"
#include "driver/casedrv.h"
#include "pmario_sound.h"
#include "driver/itemdrv.h"

#include "mario/mario.h"
#include "driver/hitdrv.h"
#include "string.h"
#include "evt/evt_bero.h"
#include "evt/evt_cam.h"
#include "evt/evt_eff.h"
#include "evt/evt_fade.h"
#include "evt/evt_damage.h"
#include "evt/evt_mario.h"
#include "evt/evt_msg.h"
#include "evt/evt_npc.h"
#include "evt/evt_party.h"
#include "evt/evt_sub.h"
#include "data_tables.h"

USER_FUNC(mario_moveride_check);

static NpcSetupInfo npcEnt[6];
static EvtWord evt_haikei_in[16];
static BeroEntry bero_entry_data[5];
const char* hitname[17];
const char* mapanim[17];
EvtWord mario_hashi_anime[34];
EvtWord erumos00_attack[50];
EvtWord erumos01_attack[50];
EvtWord sanders_attack[88];
EvtWord mario_katsugi[24];
EvtWord sanders_funto[];
EvtWord yashi_yure_init[5];
EvtWord yashi_yure[255];
static HitDamageReturn hitDmgRetPoints[4];
static TreeAccess tree_tbl[3];
static KusaAccess kusa_tbl[5];
EvtWord muj_04_init_evt[131];

/** NPCs of the rope bridge: Admiral Bobbery, the nervous sailor, the bespectacled sailor and the two Embers. */
static NpcSetupInfo npcEnt[] = {
    {
        "サンダース", // name (Admiral Bobbery)
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // reactionFlags..returnEvent
        NULL, 0, // blowEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "オドオド水夫", // name (the timid Toad sailor)
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, // reactionFlags
        npc_init_evt, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "めがね水夫", // name (Four-Eyes, a Toad sailor)
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, // reactionFlags
        npc_init_evt, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "エルモスA", // name (an Ember attacking Bobbery on the rope bridge)
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, // reactionFlags
        npc_init_evt, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "エルモスB", // name (an Ember attacking Bobbery on the rope bridge)
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, // reactionFlags
        npc_init_evt, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

/**
 * Enter event of the pipe A_dokan: in the background it waits
 * until the entrance has finished (BeroEXEC bit 0x10000) and 10 more frames,
 * then makes the camera follow Mario's height again; returns LW(0) = 0.
 */
static EVT_BEGIN(evt_haikei_in)
    BEGIN_INLINE_EVENT
        CALL(evt_bero_exec_wait, BERO_EXEC_ENTRY_DONE)
        WAIT_FRAMES(10)
        CALL(evt_mario_cam_keep_off)
        CALL(evt_mario_cam_y_on)
    END_INLINE_EVENT
    SET_VALUE(LW(0), 0)
    RETURN
EVT_END()

/** Doors and exits of muj_04: w_bero to muj_03; e_bero to muj_05; dokan_1; A_dokan. */
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
        "muj_03", // nextMapName
        "e_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "e_bero", // hitName
        0, 0, // type..sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "muj_05", // nextMapName
        "w_bero", // nextBeroName
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
        "A_dokan", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "A_dokan", // hitName
        4098, // type
        0, // sfxId
        BERO_DIR_DOWN, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        evt_haikei_in, // enterEvent
        6, // caseType
        NULL, 0, // exitEvent..nextMapName
        "dokan_1", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

/** Hit areas of the 17 planks of the rope bridge, in order; mapanim holds the matching map animations. */
const char* hitname[] = {
    "A_hasi01",
    "A_hasi02",
    "A_hasi03",
    "A_hasi04",
    "A_hasi05",
    "A_hasi06",
    "A_hasi07",
    "A_hasi08",
    "A_hasi09",
    "A_hasi010",
    "A_hasi011",
    "A_hasi012",
    "A_hasi013",
    "A_hasi014",
    "A_hasi015",
    "A_hasi016",
    "A_hasi017",
};

const char* mapanim[] = {
    "S_hasi01",
    "S_hasi02",
    "S_hasi03",
    "S_hasi04",
    "S_hasi05",
    "S_hasi06",
    "S_hasi07",
    "S_hasi08",
    "S_hasi09",
    "S_hasi10",
    "S_hasi11",
    "S_hasi12",
    "S_hasi13",
    "S_hasi14",
    "S_hasi15",
    "S_hasi16",
    "S_hasi17",
};

/**
 * Loop that makes the rope bridge sway: whenever Mario steps on a different plank (mario_moveride_check) it
 * plays a creak sound at his feet and the plank's map animation.
 */
EVT_BEGIN(mario_hashi_anime)
    SET_VALUE(LW(1), 0)
    LOOP(0)
        CALL(mario_moveride_check)
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(evt_mario_get_pos, 0, LW(10), LW(11), LW(12))
            CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_BRIDGE_CREAK1"), LW(10), LW(11), LW(12), 0)
            CALL(evt_map_playanim, LW(0), MAP_ANIM_PLAY_ONCE, 0)
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

/**
 * First Ember (Elmos A) swoops in over the bridge at Bobbery, gliding to x = -25 then -35 at height 125
 * over and over, with its footstep sounds muted.
 */
EVT_BEGIN(erumos00_attack)
    CALL(evt_npc_sound_data_set, STRING("エルモスA"), 0, 0, EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR)
    WAIT_MS(500)
    LOOP(0)
        CALL(evt_npc_get_position, STRING("エルモスA"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_ELMOS_MOVE1_2"), LW(0), LW(1), LW(2), 0)
        CALL(evt_npc_glide_position, STRING("エルモスA"), -25, 125, 0, 0, FLOAT(115.0f), 0, 11, 0)
        CALL(evt_npc_glide_position, STRING("エルモスA"), -35, 125, 0, 0, FLOAT(122.0f), 0, 11, 0)
    END_LOOP
    RETURN
EVT_END()

/** Second Ember (Elmos B) swoops in from the other side, gliding to x = 25 then 35 at height 125. */
EVT_BEGIN(erumos01_attack)
    CALL(evt_npc_sound_data_set, STRING("エルモスB"), 0, 0, EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR)
    WAIT_MS(500)
    LOOP(0)
        CALL(evt_npc_get_position, STRING("エルモスB"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_ELMOS_MOVE1_2"), LW(0), LW(1), LW(2), 0)
        CALL(evt_npc_glide_position, STRING("エルモスB"), 25, 125, 0, 0, FLOAT(110.0f), 0, 11, 0)
        CALL(evt_npc_glide_position, STRING("エルモスB"), 35, 125, 0, 0, FLOAT(121.0f), 0, 11, 0)
    END_LOOP
    RETURN
EVT_END()

/**
 * Admiral Bobbery (Sanders) is attacked by the Embers:
 * repeatedly spawns hit effects at random offsets around him and spins him round while they attack him.
 */
EVT_BEGIN(sanders_attack)
    WAIT_MS(600)
    BEGIN_INLINE_EVENT
        LOOP(0)
            WAIT_MS(200)
            CALL(evt_npc_get_position, STRING("サンダース"), LW(0), LW(1), LW(2))
            CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_THUNDERS_DAMAGED1"), LW(0), LW(1), LW(2), 0)
            CALL(evt_sub_random, 30, LW(10))
            ADD(LW(10), -15)
            ADD(LW(10), LW(0))
            CALL(evt_sub_random, 30, LW(11))
            ADD(LW(11), LW(1))
            CALL(evt_eff, STRING(""), STRING("hit"), 3, LW(10), LW(11), LW(2), FLOAT(0.5f), 18, 0, 0, 0, 0, 0, 0)
            WAIT_MS(400)
        END_LOOP
    END_INLINE_EVENT
    LOOP(0)
        WAIT_MS(100)
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 360, 200)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL(evt_npc_set_rotate, STRING("サンダース"), 0, LW(0), 0)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_LOOP
    RETURN
EVT_END()

/**
 * Keeps Mario riding on the bespectacled sailor's shoulders:
 * every frame sets Mario's position to the sailor's position plus 40 up,
 * so he appears carried (used while Bobbery rescues them).
 */
EVT_BEGIN(mario_katsugi)
    LOOP(0)
        CALL(evt_npc_get_position, STRING("めがね水夫"), LW(0), LW(1), LW(2))
        ADD(LW(1), 40)
        ADD(LW(2), 5)
        CALL(evt_mario_set_pos, LW(0), LW(1), LW(2))
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

/**
 * Bobbery's scene on the rope bridge (runs at STG5_12_FLAVIO_PA_PATCH, sets STG5_13_BOBBERY_THE_BRIDGE):
 * Mario meets Admiral Bobbery and the two scared sailors, Bobbery orders the sailors away (stg5_muj_23 to 28),
 * the Embers keep attacking him, Mario is carried off by the sailors and the party moves to muj_03.
 */
EVT_BEGIN(sanders_funto)
    CALL(evt_npc_set_position, STRING("サンダース"), 0, 112, 0)
    CALL(evt_npc_set_ry, STRING("サンダース"), 90)
    CALL(evt_npc_set_position, STRING("オドオド水夫"), -25, 112, -20)
    CALL(evt_npc_set_ry, STRING("オドオド水夫"), 270)
    CALL(evt_npc_set_position, STRING("めがね水夫"), 25, 112, -20)
    CALL(evt_npc_set_ry, STRING("めがね水夫"), 90)
#if VERSION >= VERSION_DEMO_EU
    CALL(evt_npc_flag_onoff, 1, STRING("オドオド水夫"), NPC_FLAG_NO_WALL_COLLISION | NPC_FLAG_NO_HIT)
    CALL(evt_npc_flag_onoff, 1, STRING("めがね水夫"), NPC_FLAG_NO_WALL_COLLISION | NPC_FLAG_NO_HIT)
#else
    CALL(evt_npc_flag_onoff, 1, STRING("オドオド水夫"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_flag_onoff, 1, STRING("めがね水夫"), NPC_FLAG_NO_HIT)
#endif
    CALL(evt_npc_sound_data_set, STRING("めがね水夫"), STRING("SFX_STG5_KANBU_GLASSES_MOVE1L"), STRING("SFX_STG5_KANBU_GLASSES_MOVE1R"), EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR)
    CALL(evt_npc_set_ry, STRING("エルモスA"), 90)
    CALL(evt_npc_set_ry, STRING("エルモスB"), 270)
    CALL(evt_npc_flag_onoff, 1, STRING("エルモスA"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_flag_onoff, 1, STRING("エルモスB"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_set_position, STRING("エルモスA"), -60, 125, 0)
    CALL(evt_npc_set_position, STRING("エルモスB"), 60, 125, 0)

    // Mario arrives at the bridge: control is taken and the entrance is waited out.
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_bero_exec_wait, BERO_EXEC_ENTRY_DONE)
    CALL(evt_mario_normalize)
    CALL(evt_party_stop, 0)

    // Bobbery, holding the bridge against the ghosts, curses them and spots Mario.
    CALL(evt_msg_print, 0, STRING("stg5_muj_23"), 0, STRING("サンダース"))
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_FIND1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_MARIO, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 60)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_PARTNER, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 60)
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_set, 0, 260, 532, 0, 165, -10, 1000, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_DANGER4"))
    BEGIN_INLINE_EVENT
        CALL(evt_mario_mov_pos2, -150, 0, FLOAT(150.0f))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_party_move_pos2, 0, -180, 0, FLOAT(150.0f))
    END_INLINE_EVENT
    WAIT_MS(500)
    WAIT_MS(500)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_NPC, STRING(""), STRING("サンダース"), 0, 0, 0, 0, 0, 0, 0, 60)
    WAIT_MS(1000)
    CALL(evt_set_dir_to_target, STRING("サンダース"), STRING("mario"))
    CALL(evt_cam3d_evt_set, -60, 260, 532, -60, 165, -10, 500, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg5_muj_24"), 0, STRING("サンダース"))
    CALL(evt_mario_set_pose, STRING("M_N_5B"))
    CALL(evt_set_dir_to_target, STRING("オドオド水夫"), STRING("サンダース"))
    WAIT_MS(500)
    CALL(evt_mario_set_pose, STRING("M_S_1"))

    // The nervous sailor worries about the admiral; Bobbery tells them to leave the ghosts to him.
    CALL(evt_msg_print, 0, STRING("stg5_muj_25"), 0, STRING("オドオド水夫"))
    CALL(evt_msg_print, 0, STRING("stg5_muj_26"), 0, STRING("サンダース"))
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 360, 200)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL(evt_npc_set_rotate, STRING("サンダース"), 0, LW(0), 0)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    WAIT_MS(200)
    SET_VALUE(GF(0), 1)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_flag_onoff, 0, STRING("めがね水夫"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
        CALL(evt_npc_get_position, STRING("めがね水夫"), LW(0), LW(1), LW(2))

        // The bespectacled sailor is thrown toward Mario.
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_THUNDERS_THROW1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_npc_set_ry, STRING("めがね水夫"), 270)
        CALL(evt_npc_jump_position_nohit, STRING("めがね水夫"), LW(0), LW(1), LW(2), 500, 70)
        CALL(evt_npc_flag_onoff, 1, STRING("めがね水夫"), NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_GRAVITY)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_flag_onoff, 0, STRING("オドオド水夫"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
        CALL(evt_npc_set_ry, STRING("オドオド水夫"), 270)
        CALL(evt_npc_jump_position_nohit, STRING("オドオド水夫"), -105, 116, 0, 500, 70)
        CALL(evt_npc_set_anim, STRING("オドオド水夫"), STRING("F_1"))
        CALL(evt_npc_flag_onoff, 1, STRING("オドオド水夫"), NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_GRAVITY)

        // The nervous sailor screams.
        CALL(evt_msg_print, 0, STRING("stg5_muj_27"), 0, STRING("オドオド水夫"))
        SET_VALUE(GF(0), 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(350)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_LAND_DAMAGE2"), LW(0), LW(1), LW(2), 0)
        CALL(evt_mario_set_pose, STRING("M_D_8"))
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_set, -110, 260, 532, -110, 165, -10, 500, EASING_SINE_IN_OUT)
    WAIT_MS(500)

    // The Embers charge and Bobbery fights back.
    SPAWN_EVENT(erumos00_attack)
    SPAWN_EVENT(erumos01_attack)
    SPAWN_EVENT(sanders_attack)
    WAIT_WHILE(GF(0))
    CALL(evt_npc_set_anim, STRING("オドオド水夫"), STRING("S_1"))
    CALL(evt_npc_flag_onoff, 1, STRING("めがね水夫"), NPC_FLAG_TALK_POSE)
    CALL(evt_npc_flag_onoff, 1, STRING("オドオド水夫"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    WAIT_FRAMES(1)

    // The bespectacled sailor says Bobbery thinks this is the right thing to do, and the group leaves the
    // bridge.
    CALL(evt_msg_print, 0, STRING("stg5_muj_28"), 0, STRING("めがね水夫"))
    WAIT_MS(1000)
    CALL(evt_mario_cam_keep_off)
    CALL(evt_mario_cont_onoff, 0)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_set_anim, STRING("めがね水夫"), STRING("U_1"))
        CALL(evt_mario_set_pose, STRING("M_I_6"))
        SPAWN_EVENT(mario_katsugi)
        WAIT_MS(300)
        CALL(evt_npc_set_anim, STRING("めがね水夫"), STRING("R_4"))
        CALL(evt_npc_move_position, STRING("めがね水夫"), -300, -20, 0, FLOAT(180.0f), 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(300)
        CALL(evt_npc_move_position, STRING("オドオド水夫"), -300, -20, 0, FLOAT(180.0f), 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_party_move_pos2, 0, -300, -20, FLOAT(180.0f))
    END_INLINE_EVENT
    WAIT_MS(2000)

    // The story advances and Mario crosses to muj_03.
    SET_VALUE(GSW_STORY_PROGRESS, STG5_13_BOBBERY_THE_BRIDGE)
    CALL(evt_fade_set_mapchange_type, 0, FADE_OUT_IRIS, 500, FADE_IN_IRIS, 500)
    CALL(evt_bero_mapchange, STRING("muj_03"), STRING("e_bero"))
    RETURN
EVT_END()

/** Init event of the coconut palm: sets its state GW(10) to 0 (nothing dropped yet). */
EVT_BEGIN(yashi_yure_init)
    SET_VALUE(GW(10), 0)
    RETURN
EVT_END()

/**
 * The coconut palm (S_yasi_ki): the first and second hammer hits each shake the tree and drop one coconut
 * (ITEM_COCONUT), state GW(10) counting the hits; each coconut can be picked up where it lands.
 */
// Only the first two shakes schedule coconut pickups; later shakes still increment the counter.
EVT_BEGIN(yashi_yure)
    ADD(GW(10), 1)
    IF_INT_EQUAL(GW(10), 1)
        CALL(evt_mario_key_onoff, 0)
        CALL(evt_mapobj_get_position, STRING("S_yasi_ki"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_OBJ_TREE_SHAKE1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_OBJ_LEAF_SHAKE1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_sub_intpl_init, EASING_QUADRATIC_IN, 0, -40, 15)
        LOOP(16)
            CALL(evt_sub_intpl_get_value)
            CALL(evt_mapobj_trans, 1, STRING("S_yasi_mi02"), 0, LW(0), 0)
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_mapobj_get_position, STRING("S_yasi_mi01"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_COCONUTS_FALL1"), LW(0), LW(1), LW(2), 0)
        WAIT_MS(500)
        CALL(evt_mario_key_onoff, 1)
        BEGIN_INLINE_EVENT
            LOOP(0)
                CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
                CALL(evt_sub_area_check, 0, 40, -489, 20, LW(0), LW(2), LW(10))
                IF_INT_EQUAL(LW(10), 1)
                    BREAK_LOOP
                END_IF
                WAIT_FRAMES(1)
            END_LOOP
            CALL(evt_mario_key_onoff, 0)
            CALL(evt_mapobj_trans, 1, STRING("S_yasi_mi02"), 0, -1000, 0)
            CALL(evt_item_entry, STRING("yashi00"), ITEM_COCONUT, 0, -1000, 0, ITEM_TYPE_STILL, -1, 0)
            CALL(evt_item_flag_on, STRING("yashi00"), ITEM_FLAG_NO_TOUCH_GET)
            CALL(evt_item_get_item, STRING("yashi00"))
            CALL(evt_mario_key_onoff, 1)
        END_INLINE_EVENT
        RETURN
    END_IF
    IF_INT_EQUAL(GW(10), 2)
        CALL(evt_mario_key_onoff, 0)
        CALL(evt_mapobj_get_position, STRING("S_yasi_ki"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_OBJ_TREE_SHAKE1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_OBJ_LEAF_SHAKE1"), LW(0), LW(1), LW(2), 0)
        WAIT_MS(100)
        WAIT_MS(50)
        CALL(evt_sub_intpl_init, EASING_QUADRATIC_IN, 0, -40, 15)
        LOOP(16)
            CALL(evt_sub_intpl_get_value)
            CALL(evt_mapobj_trans, 1, STRING("S_yasi_mi01"), 0, LW(0), 0)
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_mapobj_get_position, STRING("S_yasi_mi01"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_COCONUTS_FALL1"), LW(0), LW(1), LW(2), 0)
        WAIT_MS(500)
        CALL(evt_mario_key_onoff, 1)
        BEGIN_INLINE_EVENT
            LOOP(0)
                CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
                CALL(evt_sub_area_check, 0, 50, -489, 20, LW(0), LW(2), LW(10))
                IF_INT_EQUAL(LW(10), 1)
                    BREAK_LOOP
                END_IF
                WAIT_FRAMES(1)
            END_LOOP
            CALL(evt_mario_key_onoff, 0)
            CALL(evt_mapobj_trans, 1, STRING("S_yasi_mi01"), 0, -1000, 0)
            CALL(evt_item_entry, STRING("yashi01"), ITEM_COCONUT, 0, -1000, 0, ITEM_TYPE_STILL, -1, 0)
            CALL(evt_item_flag_on, STRING("yashi01"), ITEM_FLAG_NO_TOUCH_GET)
            CALL(evt_item_get_item, STRING("yashi01"))
            CALL(evt_mario_key_onoff, 1)
        END_INLINE_EVENT
        RETURN
    END_IF
    RETURN
EVT_END()

/**
 * Return points of the hazard areas around the rope bridge: its two ends (S_l_dai_03 and A_r_dai) and the
 * pipe (dokan_1).
 */
static HitDamageReturn hitDmgRetPoints[] = {
    {"S_l_dai_03", {-432.0f, 150.0f, 0.0f}},
    {"A_r_dai", {411.0f, 150.0f, 0.0f}},
    {"dokan_1", {0.0f, -1000.0f, 0.0f}},
    {0},
};

/** Trees of muj_04 that Mario can shake with his hammer (2 in all). */
static TreeAccess tree_tbl[] = {
    {5, "A_treeA_02", "treeA_02_m", "treeA_02_ha", 0, NULL},
    {5, "A_treeA_04", "treeA_04_m", "treeA_04_ha", 0, NULL},
    {7, NULL, NULL, NULL, 0, NULL},
};

/** Bushes of muj_04 that Mario can check for hidden things (4 in all). */
static KusaAccess kusa_tbl[] = {
    {1, "A_kuwa_move_01", "S_kuwa_move_01", NULL},
    {1, "A_kuwa_move_02", "S_kuwa_move_02", NULL},
    {1, "A_kuwa_move_03", "S_kuwa_move_03", NULL},
    {1, "A_kuwa_move_04", "S_kuwa_move_04", NULL},
    {4, NULL, NULL, NULL},
};

/**
 * Map init for muj_04 (the rope bridge): starts the jungle music and ambience,
 * places the Ice Power and Inn Coupon items, a Shine Sprite block and the NPCs,
 * starts the water animations and sets the story-dependent bridge and Ember scene.
 */
EVT_BEGIN(muj_04_init_evt)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG5_MUJ1"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG5_MUJ6"))
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(evt_bero_info_run)
    CALL(evt_item_entry, STRING("item_00"), ITEM_ICE_POWER, 290, 60, 10, ITEM_TYPE_STILL, GSWF_MUJ_04_ICE_POWER, 0)
    CALL(evt_item_entry, STRING("item_01"), ITEM_INN_COUPON, -367, 150, -73, ITEM_TYPE_STILL, GSWF_MUJ_04_INN_COUPON, 0)
    CALL(evt_mobj_powerupblk, STRING("pblk00"), 450, 210, -90, 0, GSWF_SHINE_28, 0)
    CALL(evt_npc_setup, PTR(npcEnt))
    CALL(evt_map_playanim, STRING("S_mizu"), MAP_ANIM_PLAY_LOOP, 0)
    CALL(evt_map_playanim, STRING("S_mizu_02"), MAP_ANIM_PLAY_LOOP, 0)
    SPAWN_EVENT(mario_hashi_anime)
    SPAWN_EVENT(yashi_yure_init)
    CALL(evt_run_case_evt, CASE_HAMMER, 1, STRING("A_hai_yasi"), 0, PTR(yashi_yure), 0)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(STG5_12_FLAVIO_PA_PATCH)
        SPAWN_EVENT(sanders_funto)
    END_SWITCH
    CALL(evt_hit_damage_return_set, PTR(hitDmgRetPoints))
    CALL_CHILD(evt_gazigazi_entry)
    CALL(evt_hitobj_attr_onoff, 1, 1, STRING("A_hasi18"), 2097152)
    CALL(evt_hitobj_attr_onoff, 1, 1, STRING("A_srt"), 2097152)
    CALL(evt_mobj_jumpstand_blue, 0, STRING("mobj_jump00"), 150, 425, 10, 130, 0, 0)
    CALL(evt_eff64, STRING("sunlight"), STRING("sunlight_n64"), 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
    SET_VALUE(LW(0), PTR(tree_tbl))
    CALL_CHILD(evt_sub_tree_access_entry)
    SET_VALUE(LW(0), PTR(kusa_tbl))
    CALL_CHILD(evt_sub_kusa_access_entry)
    RETURN
EVT_END()

USER_FUNC(mario_moveride_check) {
    s32 i;
    const char* name;
    const char* anim;
    HitEntry* hit = marioGetPtr()->hitobjStandOn;

    name = NULL;
    for (i = 0; i < 17; i++) {
        if (hit != NULL && strcmp(hitGetName(hit), hitname[i]) == 0) {
            name = hitname[i];
            anim = mapanim[i];
            break;
        }
    }
    event->lwData[0] = 0;

    // Compare the stable table name pointer to detect a newly occupied bridge segment.
    if (name != NULL && event->lwData[1] != (EvtWord)name) {
        event->lwData[0] = (EvtWord)anim;
    }
    event->lwData[1] = (EvtWord)name;
    return EVT_RETURN_DONE;
}
