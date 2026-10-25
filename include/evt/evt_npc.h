#pragma once

#include <dolphin/types.h>
#include "driver/npcdrv.h"
#include "evt/evt_cmd.h"
#include "manager/evtmgr.h"

typedef struct NpcSetupInfo {
    const char* name; //0x0
    s32 flags; //0x4
    s32 reactionFlags; //0x8
    void* initEvent; //0xC
    void* regularEvent; //0x10, TODO: rename?
    void* talkEvent; //0x14
    void* deadEvent; //0x18
    void* findEvent; //0x1C
    void* lostEvent; //0x20
    void* returnEvent; //0x24
    void* blowEvent; //0x28
    NpcTerritoryType territoryType; //0x2C
    Vec territoryBase; //0x30
    Vec territoryLoiter; //0x3C
    f32 searchRange; //0x48
    f32 searchAngle; //0x4C
    f32 homingRange; //0x50
    f32 homingAngle; //0x54
    s32 battleInfoId; //0x58, TODO: rename?
} NpcSetupInfo;

USER_FUNC(evt_npc_entry);
USER_FUNC(evt_npc_slave_entry);
USER_FUNC(evt_npc_delete);
USER_FUNC(evt_npc_check_delete);
USER_FUNC(evt_npc_get_ReactionOfLivingBody);
USER_FUNC(evt_npc_setup);
USER_FUNC(evt_npc_set_position);
USER_FUNC(evt_npc_set_width);
USER_FUNC(evt_npc_set_height);
USER_FUNC(evt_npc_get_height);
USER_FUNC(evt_npc_set_scale);
USER_FUNC(evt_npc_get_scale);
USER_FUNC(evt_npc_get_position);
USER_FUNC(evt_npc_get_home_position);
USER_FUNC(evt_npc_set_home_position);
USER_FUNC(evt_npc_get_rotate);
USER_FUNC(evt_npc_set_rotate);
USER_FUNC(evt_npc_add_rotate);
USER_FUNC(evt_npc_set_rotate_offset);
USER_FUNC(evt_npc_move_position);
f32 _intplGetFloat(s32 mode, f32 t);




NpcEntry* evtNpcNameToPtr(EventEntry* event, const char* name);
NpcEntry* evtNpcNameToPtr_NoAssert(EventEntry* evt, const char* name);





USER_FUNC(evt_npc_flag_onoff);



USER_FUNC(evt_npc_change_fbat_mode);

USER_FUNC(evt_npc_battle_start);
USER_FUNC(evt_npc_change_interrupt);
USER_FUNC(evt_npc_get_dir);
USER_FUNC(evt_npc_glide_position);
USER_FUNC(evt_npc_jump_position_nohit);
USER_FUNC(evt_npc_kamek_move_position);
USER_FUNC(evt_npc_majo_disp_off);
USER_FUNC(evt_npc_pera_onoff);
USER_FUNC(evt_npc_set_anim);
USER_FUNC(evt_npc_set_autotalkpose);
USER_FUNC(evt_npc_set_battle_info);
USER_FUNC(evt_npc_set_camid);
USER_FUNC(evt_npc_set_color);
USER_FUNC(evt_npc_set_ry);
USER_FUNC(evt_npc_sound_data_reset);
USER_FUNC(evt_npc_sound_data_set);
USER_FUNC(evt_npc_start_for_event);
USER_FUNC(evt_npc_start_for_one_event);
USER_FUNC(evt_npc_status_check);
USER_FUNC(evt_npc_status_onoff);
USER_FUNC(evt_npc_stop_for_event);
USER_FUNC(evt_npc_wait_battle_end);
USER_FUNC(evt_npc_wait_pera);
USER_FUNC(evt_set_dir_to_target);
extern EvtWord npc_init_evt[];
USER_FUNC(evt_fbat_trans_floor_position);
USER_FUNC(evt_fbat_trans_floor_position_wide);
USER_FUNC(evt_get_target_dir);
USER_FUNC(evt_npc_add_dirdist);
USER_FUNC(evt_npc_blur_onoff);
USER_FUNC(evt_npc_calc_score);
USER_FUNC(evt_npc_check);
USER_FUNC(evt_npc_check_escape_battle);
USER_FUNC(evt_npc_clear_paper);
USER_FUNC(evt_npc_facedirection_add);
USER_FUNC(evt_npc_flag_check);
USER_FUNC(evt_npc_get_battle_result);
USER_FUNC(evt_npc_get_battle_rule_keep_result);
USER_FUNC(evt_npc_get_btlsetup_work);
USER_FUNC(evt_npc_get_drop_coin);
USER_FUNC(evt_npc_get_drop_fixitem);
USER_FUNC(evt_npc_get_drop_flower);
USER_FUNC(evt_npc_get_drop_heart);
USER_FUNC(evt_npc_get_drop_item);
USER_FUNC(evt_npc_get_kpencount_type);
USER_FUNC(evt_npc_get_loiter_dir);
USER_FUNC(evt_npc_get_reglid);
USER_FUNC(evt_npc_get_unitwork);
USER_FUNC(evt_npc_getback_item_entry);
USER_FUNC(evt_npc_homing_target);
USER_FUNC(evt_npc_jump_position);
USER_FUNC(evt_npc_kamek_kemuri1);
USER_FUNC(evt_npc_kamek_kemuri2);
USER_FUNC(evt_npc_majo_disp_on);
USER_FUNC(evt_npc_reaction_flag_onoff);
USER_FUNC(evt_npc_release_fieldnpc);
USER_FUNC(evt_npc_restart_regular_event);
USER_FUNC(evt_npc_return_interrupt);
USER_FUNC(evt_npc_reverse_ry);
USER_FUNC(evt_npc_set_attack_mode);
USER_FUNC(evt_npc_set_balloontype);
USER_FUNC(evt_npc_set_battle_rule);
USER_FUNC(evt_npc_set_btlsetup_work);
USER_FUNC(evt_npc_set_force_regl_anim);
USER_FUNC(evt_npc_set_link);
USER_FUNC(evt_npc_set_offscreen);
USER_FUNC(evt_npc_set_paper);
USER_FUNC(evt_npc_set_paper_anim);
USER_FUNC(evt_npc_set_ry_lr);
USER_FUNC(evt_npc_set_tribe);
USER_FUNC(evt_npc_set_unitwork);
USER_FUNC(evt_npc_stop_for_one_event);
USER_FUNC(evt_npc_wait_anim);
USER_FUNC(evt_npc_wait_msec);
USER_FUNC(evt_set_dir_to_home);
