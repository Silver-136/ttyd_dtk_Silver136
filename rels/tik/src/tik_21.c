// tik_21 (埋没した都市, Rogueport Sewers): the room with the spike floor trap (hari_trap) and an item box.

#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "driver/hitdrv.h"
#include "evt/evt_hit.h"
#include "evt/evt_item.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_mobj.h"
#include "evt/evt_snd.h"
#include "party/party_motion.h"
#include "evt/evt_sub.h"
#include "mario/mario_pouch.h"
#include "system.h"
#include "pmario_sound.h"
#include "driver/itemdrv.h"

static BeroEntry bero_entry_data[2];
EvtWord hari_trap[107];
static HitDamageReturn hitDmgRetPoints[3];
static EvtWord mobj_box00[34];
EvtWord tik_21_init_evt[35];

/** Doors of this room: the south exit back to tik_15. */
static BeroEntry bero_entry_data[] = {
    {
        "s_bero", // hitName
        0, 0, // type..sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "tik_15", // nextMapName
        "n_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

/**
 * Spike floor trap loop of tik_21: every cycle it marks the floor (a_yuka) as spiked,
 * pops the spikes (s_toge) up 35 units in 150 ms, holds them for 1.5 seconds,
 * drops them again in 300 ms and waits about two seconds before repeating.
 * The trap sounds are SFX_MOBJ_FLOOR_TRAP1 and 2.
 */
EVT_BEGIN(hari_trap)
    CALL(evt_hit_bind_mapobj, STRING("a_toge"), STRING("s_toge"))
    LOOP(0)
        CALL(evt_hitobj_attr_onoff, 1, 1, STRING("a_yuka"), HIT_ATTR_SPIKES)
        CALL(evt_mapobj_get_position, STRING("s_toge"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_MOBJ_FLOOR_TRAP1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_sub_intpl_msec_init, EASING_QUADRATIC_IN, -5, 30, 150)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL(evt_mapobj_trans, 1, STRING("s_toge"), 0, LW(0), 0)
            CALL(evt_hit_bind_update, STRING("a_toge"))
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        WAIT_MS(1500)
        CALL(evt_mapobj_get_position, STRING("s_toge"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_MOBJ_FLOOR_TRAP2"), LW(0), LW(1), LW(2), 0)
        CALL(evt_sub_intpl_msec_init, EASING_QUADRATIC_OUT, 30, -5, 300)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL(evt_mapobj_trans, 1, STRING("s_toge"), 0, LW(0), 0)
            CALL(evt_hit_bind_update, STRING("a_toge"))
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_hitobj_attr_onoff, 1, 0, STRING("a_yuka"), HIT_ATTR_SPIKES)
#if VERSION >= VERSION_US
        WAIT_MS(2000)
#else
        WAIT_MS(2200)
#endif
    END_LOOP
    RETURN
EVT_END()

/**
 * Spots Mario is put back on after touching this room's hazards: mod_1 at its own position, mod_2 at
 * (130, 0, -64).
 */
static HitDamageReturn hitDmgRetPoints[] = {
    {"mod_1", {0.0f, -1000.0f, 0.0f}},
    {"mod_2", {130.0f, 0.0f, -64.0f}},
    {0},
};

/**
 * Item-box callback for the Spike Shield chest:
 * takes control, puts the badge at the chest, waits for the chest animation and hands it to Mario.
 */
static EVT_BEGIN(mobj_box00)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mobj_get_position, STRING("mobj_box00"), LW(0), LW(1), LW(2))
    CALL(evt_item_entry, STRING("item00"), ITEM_SPIKE_SHIELD, LW(0), LW(1), LW(2), ITEM_TYPE_STILL, -1, 0)
#if VERSION <= VERSION_DEMO_JP
    CALL(evt_item_flag_on, STRING("item00"), ITEM_FLAG_NO_TOUCH_GET)
#else
    CALL(evt_item_flag_on, STRING("item00"), ITEM_FLAG_NO_TOUCH_GET | ITEM_FLAG_LOST_BADGE_TO_SHOP)
#endif
    CALL(evt_mobj_wait_animation_end, STRING("mobj_box00"))
    CALL(evt_item_get_item, STRING("item00"))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/**
 * Map init script of tik_21, the spike trap room.
 * Sets fall recovery, starts hari_trap and places the Spike Shield chest.
 */
EVT_BEGIN(tik_21_init_evt)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG0_TIK1"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG0_TIK1"))
    CALL(evt_snd_set_rev_mode, 2)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(evt_bero_info_run)
    CALL(evt_hit_damage_return_set, PTR(hitDmgRetPoints))
    SPAWN_EVENT(hari_trap)
    CALL(evt_mobj_itembox, STRING("mobj_box00"), 150, 0, -100, MOBJ_ITEMBOX_NORMAL, 0, PTR(mobj_box00), GSWF_TIK_21_SPIKE_SHIELD_CHEST)
    RETURN
EVT_END()
