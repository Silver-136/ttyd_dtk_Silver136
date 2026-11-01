// gon_00 (ゴンババ, Hooktail Castle): entrance area reached from Petal Meadows (hei_00).
// Has a save block, a recovery block and a blue jump pad; doors lead on to gon_01.

#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "evt/evt_map.h"
#include "evt/evt_mobj.h"
#include "party/party_motion.h"
#include "evt/evt_sub.h"
#include "pmario_sound.h"


static BeroEntry bero_entry_data[4];
static BeroEntry e3_bero[2];
#if VERSION <= VERSION_DEMO_US
extern EvtWord gon_00_init_evt[];
#else
EvtWord gon_00_init_evt[77];
#endif

/** Exits (beros) of this room: w_bero to hei_00, e_bero_1 to gon_01 and e_bero_3 to gon_01. */
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
        "hei_00", // nextMapName
        "a_usiro_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "e_bero_1", // hitName
        0, 0, // type..sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "gon_01", // nextMapName
        "w_bero_1", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "e_bero_3", // hitName
        0, 0, // type..sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "gon_01", // nextMapName
        "w_bero_2", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

/** Exits (beros) of this room: w_bero to gon_00. */
static BeroEntry e3_bero[] = {
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
        "gon_00", // nextMapName
        "w_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

EVT_BEGIN(gon_00_init_evt)
#if VERSION > VERSION_DEMO_US
    SET_VALUE(GSWF_GON_00_VISITED, 1)
#endif
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
#if VERSION <= VERSION_DEMO_US
    CALL_CHILD(evt_bero_info_run)
#else
    CALL_CHILD(PTR(evt_bero_info_run))
#endif
    WAIT_FRAMES(1)
    CALL(evt_snd_bgmoff, PSND_FADEOUT)
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG1_GON1"))
    CALL(evt_snd_set_rev_mode, 2)
    CALL(evt_mobj_save_blk, STRING("mobj_save"), 160, 60, -90, 0, 0)
    CALL(evt_mobj_recovery_blk, STRING("mobj_recover"), 6, -150, 260, 0, 0, 0)
    CALL(evt_mobj_jumpstand_blue, 0, STRING("mobj_jump"), 230, 0, 0, -90, 0, 0)
    CALL(evt_map_playanim, STRING("hokori"), MAP_ANIM_PLAY_LOOP, 0)
    CALL(evt_sub_get_system_flag, LW(0))
    IF_BITS_SET(LW(0), GW_FLAG_E3)
        SET_VALUE(LW(0), STRING("w_bero"))
#if VERSION <= VERSION_DEMO_US
        CALL_CHILD(bero_case_switch_off)
#else
        CALL_CHILD(PTR(bero_case_switch_off))
        // The E3 exit loops back to this room instead of returning to Petal Meadows.
#endif
        CALL(evt_bero_overwrite, STRING("w_bero"), PTR(e3_bero))
        SET_VALUE(LW(0), STRING("w_bero"))
#if VERSION <= VERSION_DEMO_US
        CALL_CHILD(bero_case_switch_on)
#else
        CALL_CHILD(PTR(bero_case_switch_on))
#endif
    END_IF
    RETURN
EVT_END()

