#pragma once

#include "evt/evt_cmd.h"

// evt_mobj_brick kind (args[5], raw): the model and block behaviour; 10 and up are hidden until hit
typedef enum MobjBrickKind {
    MOBJ_BRICK_PLAIN = 0,         // MOBJ_FlowBrickBlock, breakable brick
    MOBJ_BRICK_ITEM = 1,          // MOBJ_BrickHatenaBlock, gives its item
    MOBJ_BRICK_BADGE = 2,         // MOBJ_BrickBadgeBlock
    MOBJ_BRICK_TEN_COIN = 3,      // MOBJ_10CountBlock, coins on up to ten hits
    MOBJ_BRICK_HIDDEN_PLAIN = 10, // MOBJ_HiddenHatenaBlock with the plain brick behaviour
    MOBJ_BRICK_HIDDEN_ITEM = 11,  // MOBJ_HiddenHatenaBlock
    MOBJ_BRICK_HIDDEN_BADGE = 12, // MOBJ_HiddenBadgeBlock
    MOBJ_BRICK_HIDDEN_TEN_COIN = 13, // MOBJ_Hidden10CountBlock
} MobjBrickKind;

// evt_mobj_itembox kind (args[4]): the chest model
typedef enum MobjItemboxKind {
    MOBJ_ITEMBOX_NORMAL = 0,      // MOBJ_TreasureBox
    MOBJ_ITEMBOX_BIG = 1,         // MOBJ_BigTreasureBox
    MOBJ_ITEMBOX_GRAY = 2,        // MOBJ_GrayTreasureBox
    MOBJ_ITEMBOX_BLACK = 3,       // MOBJ_BlackTreasureBox
    MOBJ_ITEMBOX_GOLDEN = 4,      // MOBJ_GoldenTreasureBox (also any other value)
} MobjItemboxKind;

USER_FUNC(evt_mobj_delete);
USER_FUNC(evt_mobj_entry);
USER_FUNC(evt_mobj_flag_onoff);
USER_FUNC(evt_mobj_get_position);
USER_FUNC(evt_mobj_get_x_position);
USER_FUNC(evt_mobj_get_y_position);
USER_FUNC(evt_mobj_itembox);
USER_FUNC(evt_mobj_kururing_floor);
USER_FUNC(evt_mobj_recovery_blk);
USER_FUNC(evt_mobj_save_blk);
USER_FUNC(evt_mobj_set_anim);
USER_FUNC(evt_mobj_set_position);
USER_FUNC(evt_mobj_set_scale);
USER_FUNC(evt_mobj_set_x_position);
USER_FUNC(evt_mobj_set_y_position);

extern EvtWord mobj_save_blk_sysevt[];
USER_FUNC(evt_mobj_badgeblk);
USER_FUNC(evt_mobj_blk);
USER_FUNC(evt_mobj_breaking_floor);
USER_FUNC(evt_mobj_brick);
USER_FUNC(evt_mobj_check);
USER_FUNC(evt_mobj_exec_cancel);
USER_FUNC(evt_mobj_float_blk);
USER_FUNC(evt_mobj_floatswitch_blue);
USER_FUNC(evt_mobj_floatswitch_red);
USER_FUNC(evt_mobj_get_kindname);
USER_FUNC(evt_mobj_get_z_position);
USER_FUNC(evt_mobj_hit_onoff);
USER_FUNC(evt_mobj_hitevt_onoff);
USER_FUNC(evt_mobj_jumpstand_blue);
USER_FUNC(evt_mobj_jumpstand_red);
USER_FUNC(evt_mobj_koopa_badgeblk);
USER_FUNC(evt_mobj_koopa_blk);
USER_FUNC(evt_mobj_koopa_brick);
USER_FUNC(evt_mobj_koopa_dokan);
USER_FUNC(evt_mobj_koopa_fireber_dodai);
USER_FUNC(evt_mobj_koopa_hidden_blk);
USER_FUNC(evt_mobj_koopa_ojama_blk);
USER_FUNC(evt_mobj_koopa_pole);
USER_FUNC(evt_mobj_koopa_sango);
USER_FUNC(evt_mobj_koopa_stone_blk);
USER_FUNC(evt_mobj_lock);
USER_FUNC(evt_mobj_lock_unlock);
USER_FUNC(evt_mobj_lv_blk);
USER_FUNC(evt_mobj_powerupblk);
USER_FUNC(evt_mobj_rideswitch_lightblue);
USER_FUNC(evt_mobj_set_camid);
USER_FUNC(evt_mobj_set_gravity_bound);
USER_FUNC(evt_mobj_set_z_position);
USER_FUNC(evt_mobj_signboard);
USER_FUNC(evt_mobj_switch_blue);
USER_FUNC(evt_mobj_switch_float_blk);
USER_FUNC(evt_mobj_switch_red);
USER_FUNC(evt_mobj_timerswitch);
USER_FUNC(evt_mobj_tornadoswitch_blue);
USER_FUNC(evt_mobj_wait_animation_end);
USER_FUNC(fire_func);
USER_FUNC(kpa_clear_jump_data);
USER_FUNC(kpa_disp_pole_init);
USER_FUNC(kpa_disp_pole_score_main);
USER_FUNC(kpa_get_hata_name);
USER_FUNC(kpa_get_pole_name);
USER_FUNC(kpa_get_width);
USER_FUNC(kpa_powerup);
USER_FUNC(kpa_score_to_str);
