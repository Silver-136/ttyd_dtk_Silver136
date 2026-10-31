#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "evt/evt_img.h"
#include "evt/evt_item.h"
#include "evt/evt_mario.h"
#include "evt/evt_mobj.h"
#include "evt/evt_paper.h"
#include "evt/evt_snd.h"
#include "party/party_motion.h"
#include "driver/mobjdrv.h"
#include "mario/mario_pouch.h"
#include "pmario_sound.h"
#include "driver/itemdrv.h"


static BeroEntry bero_entry_data[2];
EvtWord item_get03[34];
static EvtWord switch_evt[68];
EvtWord gon_13_init_evt[56];

static BeroEntry bero_entry_data[] = {
    {
        "w_bero", // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "gon_07", // nextMapName
        "e_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

/** Runs when Mario opens the chest mobj_box04: shows the Up Arrow coming out and gives it to him. */
EVT_BEGIN(item_get03)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mobj_get_position, STRING("mobj_box04"), LW(0), LW(1), LW(2))
    CALL(evt_item_entry, STRING("item00"), ITEM_UP_ARROW, LW(0), LW(1), LW(2), ITEM_TYPE_STILL, -1, 0)
    CALL(evt_item_flag_on, STRING("item00"), ITEM_FLAG_NO_TOUCH_GET)
    CALL(evt_mobj_wait_animation_end, STRING("mobj_box04"))
    CALL(evt_item_get_item, STRING("item00"))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/**
 * Runs when Mario lands on the switch: the paper image OFF_d_biri rips open (SFX_OFF1_PERA_BREAK1,
 * SFX_OFF1_PERA_FALL1, animation A_1) and reveals the chest mobj_box04 (made visible and hittable).
 */
static EVT_BEGIN(switch_evt)
    CALL(evt_mario_wait_landon)
    CALL(evt_paper_entry, STRING("OFF_d_biri"))
    CALL(evt_img_entry, STRING("img"))
    CALL(evt_img_set_paper, STRING("img"), STRING("OFF_d_biri"))
    CALL(evt_img_onoff, STRING("img"), 0)
    CALL(evt_img_alloc_capture, STRING("img"), 0, 0, 1, 0, 0, 604, 480)
    WAIT_FRAMES(1)
    CALL(evt_img_onoff, STRING("img"), 1)
    CALL(evt_mobj_flag_onoff, 0, STRING("mobj_box04"), MOBJ_FLAG_NO_DISP)
    CALL(evt_mobj_hit_onoff, STRING("mobj_box04"), 1)
    BEGIN_INLINE_EVENT
        CALL(evt_snd_sfxon, STRING("SFX_OFF1_PERA_BREAK1"), 0)
        WAIT_MS(1350)
        CALL(evt_snd_sfxon, STRING("SFX_OFF1_PERA_FALL1"), 0)
    END_INLINE_EVENT
    CALL(evt_img_set_paper_anim, STRING("img"), STRING("A_1"))
    CALL(evt_img_wait_animend, STRING("img"))
    CALL(evt_img_release, STRING("img"))
    CALL(evt_paper_delete, STRING("OFF_d_biri"))
    RETURN
EVT_END()

/**
 * Map init for gon_13: sets up the door back to gon_07,
 * the floating blue switch and the chest that stays hidden
 * until the switch is pressed (a paper sheet tears away and reveals it).
 */
EVT_BEGIN(gon_13_init_evt)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG1_GON1"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG1_GON1"))
    CALL(evt_snd_set_rev_mode, 2)
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_UNSILENT, 0)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_mobj_floatswitch_blue, 0, STRING("mobj_switch00"), 50, 60, 40, PTR(switch_evt), GSWF_GON_13_SWITCH_PRESSED)
    CALL(evt_mobj_itembox, STRING("mobj_box04"), 0, 0, -50, MOBJ_ITEMBOX_NORMAL, 0, PTR(item_get03), GSWF_GON_13_UP_ARROW_CHEST)
    IF_INT_EQUAL(GSWF_GON_13_SWITCH_PRESSED, 0)
        CALL(evt_mobj_flag_onoff, 1, STRING("mobj_box04"), MOBJ_FLAG_NO_DISP)
        CALL(evt_mobj_hit_onoff, STRING("mobj_box04"), 0)
    END_IF
    RETURN
EVT_END()

