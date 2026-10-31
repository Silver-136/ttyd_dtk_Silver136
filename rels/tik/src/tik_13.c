// tik_13 (埋没した都市, Rogueport Sewers): a small dripping room off tik_02.

#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "evt/evt_map.h"
#include "evt/evt_snd.h"
#include "party/party_motion.h"
#include "evt/evt_item.h"
#include "mario/mario_pouch.h"
#include "pmario_sound.h"
#include "driver/itemdrv.h"


static BeroEntry bero_entry_data[2];
EvtWord tik_13_init_evt[60];

/** Doors of this room: the east exit back to tik_02. */
static BeroEntry bero_entry_data[] = {
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
        "tik_02", // nextMapName
        "w_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

/**
 * Map init script of tik_13, a small dripping-water room off tik_02.
 * Starts the sewer music, sets up the exit, loops the dripping animation (anm_mizu) in the background
 * and places a Star Piece.
 */
EVT_BEGIN(tik_13_init_evt)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG0_TIK1"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG0_TIK6"))
    CALL(evt_snd_set_rev_mode, 2)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    BEGIN_INLINE_EVENT
        LOOP(0)
            CALL(evt_map_playanim, STRING("anm_mizu"), MAP_ANIM_PLAY_ONCE, 0)
            CALL(evt_map_checkanim, STRING("anm_mizu"), LW(0), LW(1))
            WAIT_MS(LW(1))
            CALL(evt_mapobj_get_position, STRING("Tshatu"), LW(0), LW(1), LW(2))
            CALL(evt_snd_sfxon_3d, STRING("SFX_STG0_AMB_WATERDROP2"), LW(0), LW(1), LW(2), 0)
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_item_entry, STRING("item_00"), ITEM_STAR_PIECE, -30, 52, -70, ITEM_TYPE_STILL, GSWF_STARPIECE_29, 0)
    RETURN
EVT_END()

