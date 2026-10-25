#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"

USER_FUNC(evt_bero_get_info);
USER_FUNC(evt_img_alloc_capture);
USER_FUNC(evt_img_entry);
USER_FUNC(evt_img_onoff);
USER_FUNC(evt_img_release);
USER_FUNC(evt_img_set_paper);
USER_FUNC(evt_img_set_paper_anim);
USER_FUNC(evt_img_wait_animend);
USER_FUNC(evt_item_entry);
USER_FUNC(evt_item_flag_on);
USER_FUNC(evt_item_get_item);
USER_FUNC(evt_mario_key_onoff);
USER_FUNC(evt_mario_wait_landon);
USER_FUNC(evt_mobj_flag_onoff);
USER_FUNC(evt_mobj_floatswitch_blue);
USER_FUNC(evt_mobj_get_position);
USER_FUNC(evt_mobj_hit_onoff);
USER_FUNC(evt_mobj_itembox);
USER_FUNC(evt_mobj_wait_animation_end);
USER_FUNC(evt_paper_delete);
USER_FUNC(evt_paper_entry);
USER_FUNC(evt_snd_bgmon);
USER_FUNC(evt_snd_envon);
USER_FUNC(evt_snd_set_rev_mode);
USER_FUNC(evt_snd_sfxon);

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

EVT_BEGIN(item_get03)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mobj_get_position, STRING("mobj_box04"), LW(0), LW(1), LW(2))
    CALL(evt_item_entry, STRING("item00"), 108, LW(0), LW(1), LW(2), 16, -1, 0)
    CALL(evt_item_flag_on, STRING("item00"), 2)
    CALL(evt_mobj_wait_animation_end, STRING("mobj_box04"))
    CALL(evt_item_get_item, STRING("item00"))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(switch_evt)
    CALL(evt_mario_wait_landon)
    CALL(evt_paper_entry, STRING("OFF_d_biri"))
    CALL(evt_img_entry, STRING("img"))
    CALL(evt_img_set_paper, STRING("img"), STRING("OFF_d_biri"))
    CALL(evt_img_onoff, STRING("img"), 0)
    CALL(evt_img_alloc_capture, STRING("img"), 0, 0, 1, 0, 0, 604, 480)
    WAIT_FRAMES(1)
    CALL(evt_img_onoff, STRING("img"), 1)
    CALL(evt_mobj_flag_onoff, 0, STRING("mobj_box04"), 16384)
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

EVT_BEGIN(gon_13_init_evt)
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG1_GON1"))
    CALL(evt_snd_envon, 272, STRING("ENV_STG1_GON1"))
    CALL(evt_snd_set_rev_mode, 2)
    CALL(evt_snd_bgmon, 160, 0)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_mobj_floatswitch_blue, 0, STRING("mobj_switch00"), 50, 60, 40, PTR(switch_evt), GSWF(1499))
    CALL(evt_mobj_itembox, STRING("mobj_box04"), 0, 0, -50, 0, 0, PTR(item_get03), GSWF(1510))
    IF_INT_EQUAL(GSWF(1499), 0)
        CALL(evt_mobj_flag_onoff, 1, STRING("mobj_box04"), 16384)
        CALL(evt_mobj_hit_onoff, STRING("mobj_box04"), 0)
    END_IF
    RETURN
EVT_END()

