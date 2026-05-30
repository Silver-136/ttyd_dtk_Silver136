#include "event/evt_npc.h"

void _npc_check_coin_group(void* group, s32* coin);

void _npc_check_coin(void* npc, s32* coin) {
    *coin += *(u8*)((s32)npc + 0x315);
}

USER_FUNC(evt_npc_check_escape_battle) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern void marioSetMutekiTime(s32 time);
    if (evtGetValue(event, event->args[0]) == 4) {
        marioSetMutekiTime(3000);
    }
    return 2;
}

USER_FUNC(evt_npc_start_for_event) {
    extern void npcStartForEvent(void);
    npcStartForEvent();
    return 2;
}

USER_FUNC(evt_npc_stop_for_event) {
    extern void npcStopForEvent(void);
    npcStopForEvent();
    return 2;
}

USER_FUNC(evt_npc_change_fbat_mode) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern void fbatChangeMode(s16 mode);
    fbatChangeMode((s16)evtGetValue(event, event->args[0]));
    return 2;
}

USER_FUNC(evt_npc_delete) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    extern void npcDeleteGroup(void* npc);
    s32 value = evtGetValue(event, event->args[0]);
    npcDeleteGroup(evtNpcNameToPtr(event, value));
    return 2;
}

USER_FUNC(evt_npc_getback_item_entry) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    extern void npcGetBackItemEntry(void* npc);
    s32 value = evtGetValue(event, event->args[0]);
    npcGetBackItemEntry(evtNpcNameToPtr(event, value));
    return 2;
}

USER_FUNC(evt_npc_start_for_one_event) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    extern void npcStartForOneEvent(void* npc);
    s32 value = evtGetValue(event, event->args[0]);
    npcStartForOneEvent(evtNpcNameToPtr(event, value));
    return 2;
}

USER_FUNC(evt_npc_stop_for_one_event) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    extern void npcStopForOneEvent(void* npc);
    s32 value = evtGetValue(event, event->args[0]);
    npcStopForOneEvent(evtNpcNameToPtr(event, value));
    return 2;
}

USER_FUNC(evt_npc_get_battle_rule_keep_result) {
    extern void* fbatGetPointer(void);
    extern s32 evtSetValue(EventEntry* event, s32 index, s32 value);
    EventEntry* evt = event;
    s32 arg = event->args[0];
    void* fbat = fbatGetPointer();
    evtSetValue(evt, arg, *(u8*)((s32)fbat + 0x39));
    return 2;
}

USER_FUNC(evt_npc_get_battle_result) {
    extern void* fbatGetPointer(void);
    extern s32 BattleInformationGetResult(void* info);
    extern s32 evtSetValue(EventEntry* event, s32 index, s32 value);
    s32 arg = event->args[0];
    evtSetValue(event, arg, BattleInformationGetResult((void*)((s32)fbatGetPointer() + 0x20)));
    return 2;
}

USER_FUNC(evt_npc_set_attack_mode) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    s32* args = event->args;
    s32 name = evtGetValue(event, args[0]);
    s16 value = evtGetValue(event, args[1]);
    *(s32*)((s32)evtNpcNameToPtr(event, name) + 0x2F8) = value;
    return 2;
}

USER_FUNC(evt_npc_set_camid) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    s32* args = event->args;
    s32 name = evtGetValue(event, args[0]);
    s32 value = evtGetValue(event, args[1]);
    *(s32*)((s32)evtNpcNameToPtr(event, name) + 0x1F4) = value;
    return 2;
}

USER_FUNC(evt_npc_check_delete) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern void* evtNpcNameToPtr_NoAssert(EventEntry* event, s32 name);
    extern void npcDeleteGroup(void* npc);
    s32 name;
    void* npc;
    name = evtGetValue(event, event->args[0]);
    npc = evtNpcNameToPtr_NoAssert(event, name);
    if (npc != 0) {
        npcDeleteGroup(npc);
    }
    return 2;
}

USER_FUNC(evt_npc_get_ReactionOfLivingBody) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern s32 npcGetReactionOfLivingBody(s32 name);
    extern s32 evtSetValue(EventEntry* event, s32 index, s32 value);
    EventEntry* evt = event;
    s32* args = event->args;
    s32 name = evtGetValue(event, args[0]);
    s32 dst = args[1];
    s32 reaction = npcGetReactionOfLivingBody(name);
    evtSetValue(evt, dst, reaction);
    return 2;
}

USER_FUNC(evt_npc_get_drop_fixitem) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    extern s32 evtSetValue(EventEntry* event, s32 index, s32 value);
    EventEntry* evt = event;
    s32* args = event->args;
    s32 name = evtGetValue(event, args[0]);
    void* npc = evtNpcNameToPtr(event, name);
    evtSetValue(evt, args[1], *(s32*)((s32)npc + 0x238));
    return 2;
}

USER_FUNC(evt_npc_get_reglid) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    extern s32 evtSetValue(EventEntry* event, s32 index, s32 value);
    EventEntry* evt = event;
    s32* args = event->args;
    s32 name = evtGetValue(event, args[0]);
    void* npc = evtNpcNameToPtr(event, name);
    evtSetValue(evt, args[1], *(s32*)((s32)npc + 0x11C));
    return 1;
}

USER_FUNC(evt_npc_set_balloontype) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    s32* args = event->args;
    s32 name;
    void* npc;
    name = evtGetValue(event, args[0]);
    npc = evtNpcNameToPtr(event, name);
    *(u8*)((s32)npc + 0x317) = evtGetValue(event, args[1]);
    return 1;
}

USER_FUNC(evt_npc_get_dir) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    extern s32 evtSetValue(EventEntry* event, s32 index, s32 value);
    s32* args = event->args;
    s32 name = evtGetValue(event, args[0]);
    void* npc = evtNpcNameToPtr(event, name);
    evtSetValue(event, args[1], (s32)*(f32*)((s32)npc + 0x144));
    return 2;
}

USER_FUNC(evt_npc_set_battle_info) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    extern void npcSetBattleInfo(void* npc, s32 info);
    s32* args = event->args;
    s32 name = evtGetValue(event, args[0]);
    s32 info = evtGetValue(event, args[1]);
    npcSetBattleInfo(evtNpcNameToPtr(event, name), info);
    return 2;
}

USER_FUNC(evt_npc_set_height) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern f32 evtGetFloat(EventEntry* event, s32 value);
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    s32* args = event->args;
    s32 name = evtGetValue(event, args[0]);
    f32 value = evtGetFloat(event, args[1]);
    *(f32*)((s32)evtNpcNameToPtr(event, name) + 0x150) = value;
    return 2;
}

USER_FUNC(evt_npc_set_ry) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern f32 evtGetFloat(EventEntry* event, s32 value);
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    s32* args = event->args;
    s32 name = evtGetValue(event, args[0]);
    f32 value = evtGetFloat(event, args[1]);
    *(f32*)((s32)evtNpcNameToPtr(event, name) + 0x144) = value;
    return 2;
}

USER_FUNC(evt_npc_set_width) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern f32 evtGetFloat(EventEntry* event, s32 value);
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    s32* args = event->args;
    s32 name = evtGetValue(event, args[0]);
    f32 value = evtGetFloat(event, args[1]);
    *(f32*)((s32)evtNpcNameToPtr(event, name) + 0x14C) = value;
    return 2;
}

USER_FUNC(evt_npc_flag_check) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    extern s32 evtSetValue(EventEntry* event, s32 index, s32 value);
    s32* args = event->args;
    s32 name = evtGetValue(event, args[0]);
    s32 flag = evtGetValue(event, args[1]);
    void* npc = evtNpcNameToPtr(event, name);
    evtSetValue(event, args[2], *(s32*)npc & flag);
    return 2;
}

USER_FUNC(evt_npc_set_paper_anim) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    extern void animPoseSetPaperAnim(void* pose, s32 anim, s32 enabled);
    s32* args = event->args;
    s32 name = evtGetValue(event, args[0]);
    s32 anim = evtGetValue(event, args[1]);
    animPoseSetPaperAnim(*(void**)((s32)evtNpcNameToPtr(event, name) + 0x104), anim, 1);
    return 2;
}

USER_FUNC(evt_npc_status_check) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    extern s32 evtSetValue(EventEntry* event, s32 index, s32 value);
    s32* args = event->args;
    s32 name = evtGetValue(event, args[0]);
    s32 flag = evtGetValue(event, args[1]);
    void* npc = evtNpcNameToPtr(event, name);
    evtSetValue(event, args[2], *(s32*)((s32)npc + 0x1D4) & flag);
    return 2;
}

USER_FUNC(evt_npc_check) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern void* npcNameToPtr_NoAssert(s32 name);
    extern s32 evtSetValue(EventEntry* event, s32 index, s32 value);
    s32* args = event->args;
    if (npcNameToPtr_NoAssert(evtGetValue(event, args[0])) == 0) {
        evtSetValue(event, args[1], 0);
    } else {
        evtSetValue(event, args[1], 1);
    }
    return 2;
}

USER_FUNC(evt_npc_get_unitwork) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    extern s32 evtSetValue(EventEntry* event, s32 index, s32 value);
    EventEntry* evt = event;
    s32* args = event->args;
    s32 name = evtGetValue(event, args[0]);
    void* npc = evtNpcNameToPtr(event, name);
    s32 idx = evtGetValue(event, args[1]);
    evtSetValue(evt, args[2], *(s32*)((s32)npc + 0x304 + idx * 4));
    return 2;
}

USER_FUNC(evt_npc_set_unitwork) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    EventEntry* evt = event;
    s32* args = event->args;
    s32 name = evtGetValue(event, args[0]);
    void* npc = evtNpcNameToPtr(event, name);
    s32 idx = evtGetValue(event, args[1]);
    s32 value = evtGetValue(evt, args[2]);
    *(s32*)((s32)npc + 0x304 + idx * 4) = value;
    return 2;
}

USER_FUNC(evt_npc_get_btlsetup_work) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    extern s32 npcGetBtlSetupWork(void* npc, s32 index);
    extern s32 evtSetValue(EventEntry* event, s32 index, s32 value);
    EventEntry* evt = event;
    s32* args = event->args;
    s32 name = evtGetValue(event, args[0]);
    void* npc = evtNpcNameToPtr(event, name);
    s32 idx = evtGetValue(event, args[1]);
    s32 dst = args[2];
    evtSetValue(evt, dst, npcGetBtlSetupWork(npc, idx));
    return 2;
}

USER_FUNC(evt_npc_set_btlsetup_work) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    extern void npcSetBtlSetupWork(void* npc, s32 index, s32 value);
    EventEntry* evt = event;
    s32* args = event->args;
    s32 name = evtGetValue(event, args[0]);
    void* npc = evtNpcNameToPtr(event, name);
    s32 idx = evtGetValue(event, args[1]);
    s32 value = evtGetValue(evt, args[2]);
    npcSetBtlSetupWork(npc, idx, value);
    return 2;
}

void _npc_check_coin_group(void* group, s32* coin) {
    void* child;
    s32 offset;
    s32 i;
    s32 entryOffset;

    child = *(void**)((s32)group + 0x328);
    if (child != 0) {
        _npc_check_coin_group(child, coin);
    }

    i = 0;
    offset = 0;
    while (i < 4) {
        entryOffset = offset;
        entryOffset += 0x330;
        child = *(void**)((s32)group + entryOffset);

        if (child != 0) {
            _npc_check_coin(child, coin);
        }

        i++;
        offset += 4;
    }

    _npc_check_coin(group, coin);
}

USER_FUNC(evt_get_target_dir) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    extern f32 _get_target_dir(EventEntry* event, void* npc, s32 target);
    extern void evtSetFloat(EventEntry* event, s32 index, f32 value);

    s32* args;
    s32 name;
    s32 target;
    s32 dst;
    void* npc;

    args = event->args;

    name = evtGetValue(event, args[0]);
    target = evtGetValue(event, args[1]);
    dst = args[2];

    npc = evtNpcNameToPtr(event, name);

    evtSetFloat(event, dst, _get_target_dir(event, npc, target));
    return 2;
}

USER_FUNC(evt_set_dir_to_home) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    extern f32 angleABf(f32 x1, f32 z1, f32 x2, f32 z2);

    s32* args;
    s32 name;
    void* npc;

    args = event->args;

    name = evtGetValue(event, args[0]);
    evtGetValue(event, args[1]);

    npc = evtNpcNameToPtr(event, name);

    *(f32*)((s32)npc + 0x144) = angleABf(
        *(f32*)((s32)npc + 0x8C),
        *(f32*)((s32)npc + 0x94),
        *(f32*)((s32)npc + 0x1FC),
        *(f32*)((s32)npc + 0x204)
    );

    return 2;
}

USER_FUNC(evt_set_dir_to_target) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    extern f32 _get_target_dir(EventEntry* event, void* npc, s32 target);

    s32* args;
    s32 name;
    s32 target;
    void* npc;

    args = event->args;

    name = evtGetValue(event, args[0]);
    target = evtGetValue(event, args[1]);

    npc = evtNpcNameToPtr(event, name);

    *(f32*)((s32)npc + 0x144) = _get_target_dir(event, npc, target);

    return 2;
}

USER_FUNC(evt_npc_set_paper) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    extern void animPoseSetPaperAnimGroup(void* pose, s32 anim, s32 enabled);
    extern void animPosePaperPeraOn(void* pose);

    s32* args;
    s32 name;
    s32 anim;
    void* npc;

    args = event->args;

    name = evtGetValue(event, args[0]);
    anim = evtGetValue(event, args[1]);

    npc = evtNpcNameToPtr(event, name);

    animPoseSetPaperAnimGroup(*(void**)((s32)npc + 0x104), anim, 1);
    animPosePaperPeraOn(*(void**)((s32)npc + 0x104));

    return 2;
}


u8 evt_npc_kamek_move_position(void) {
    return 0;
}


u8 evt_npc_homing_target(void) {
    return 0;
}


u8 evt_npc_move_position(s32 pEvt, s32 param_2) {
    return 0;
}


u8 evt_npc_jump_position(s32 pEvt, s32 param_2) {
    return 0;
}


u8 evt_npc_glide_position(s32 pEvt, s32 param_2) {
    return 0;
}


u8 evt_npc_jump_position_nohit(s32 pEvt, s32 param_2) {
    return 0;
}


u8 evt_npc_majo_disp_on(void) {
    return 0;
}


s32 evt_npc_setup(void* pEvt, int param_2) {
    return 0;
}


s32 evt_npc_majo_disp_off(void* pEvt, int param_2) {
    return 0;
}


u8 _intplGetFloat(void) {
    return 0;
}


void* evtNpcNameToPtr_NoAssert(EventEntry* event, s32 name) {
    return 0;
}


void* evtNpcNameToPtr(EventEntry* event, s32 name) {
    return 0;
}


u8 evt_npc_set_rotate_offset(s32 pEvt) {
    return 0;
}


u8 evt_npc_set_rotate(s32 pEvt) {
    return 0;
}


u8 evt_fbat_trans_floor_position(s32 pEvt) {
    return 0;
}


u8 evt_npc_add_rotate(s32 pEvt) {
    return 0;
}


s32 evt_npc_slave_entry(void* pEvt) {
    return 0;
}


u8 evt_npc_add_dirdist(void) {
    return 0;
}


s32 evt_npc_sound_data_set(int param_1) {
    return 0;
}


u8 _kamek_houki_kemuri(int param_1, int param_2) {
    return 0;
}


s32 evt_npc_set_tribe(void* pEvt) {
    return 0;
}


u8 evt_npc_kamek_kemuri2(void) {
    return 0;
}


u8 evt_npc_facedirection_add(void) {
    return 0;
}


s32 evt_npc_set_ry_lr(void* pEvt) {
    return 0;
}


u8 evt_npc_get_loiter_dir(void) {
    return 0;
}


s32 evt_npc_set_home_position(void* pEvt) {
    return 0;
}


s32 evt_npc_set_position(void* pEvt) {
    return 0;
}


int evt_npc_wait_msec(void* pEvt, int param_2) {
    return 0;
}


f32 _get_target_dir(EventEntry* event, void* npc, s32 target) {
    return 0.0f;
}


s32 evt_npc_get_drop_flower(void* pEvt) {
    return 0;
}


s32 evt_npc_get_drop_heart(void* pEvt) {
    return 0;
}


s32 evt_npc_set_autotalkpose(void* pEvt) {
    return 0;
}


s32 evt_npc_battle_start(void* pEvt, int param_2) {
    return 0;
}


s32 evt_npc_set_color(void* pEvt) {
    return 0;
}


void evt_npc_change_interrupt(void* pEvt) {
}


s32 evt_npc_status_onoff(void* pEvt) {
    return 0;
}


s32 evt_npc_reaction_flag_onoff(void* pEvt) {
    return 0;
}


s32 evt_npc_flag_onoff(void* pEvt) {
    return 0;
}


s32 evt_npc_kamek_kemuri1(int param_1, int param_2) {
    return 0;
}


u8 evt_npc_set_scale(void) {
    return 0;
}


s32 evt_npc_get_drop_coin(void* pEvt) {
    return 0;
}


s32 evt_npc_sound_data_reset(int param_1) {
    return 0;
}


s32 evt_npc_set_battle_rule(void* pEvt) {
    return 0;
}


s32 evt_npc_pera_onoff(void* pEvt) {
    return 0;
}


s32 evt_npc_get_rotate(void* pEvt) {
    return 0;
}


s32 evt_npc_get_home_position(void* pEvt) {
    return 0;
}


s32 evt_npc_get_position(void* pEvt) {
    return 0;
}


s32 evt_npc_get_scale(void* pEvt) {
    return 0;
}


s32 evt_npc_set_link(void* pEvt) {
    return 0;
}


s32 evt_npc_set_offscreen(void* pEvt) {
    return 0;
}


s32 evt_npc_entry(void* pEvt) {
    return 0;
}


s32 evt_npc_wait_battle_end(void) {
    return 0;
}


s32 evt_npc_wait_anim(void* pEvt) {
    return 0;
}


s32 evt_npc_set_force_regl_anim(void* pEvt) {
    return 0;
}


s32 evt_npc_set_run_anim(void* pEvt) {
    return 0;
}


s32 evt_npc_set_walk_anim(void* pEvt) {
    return 0;
}


s32 evt_npc_set_talk_anim(void* pEvt) {
    return 0;
}


s32 evt_npc_set_stay_anim(void* pEvt) {
    return 0;
}


s32 evt_npc_set_stop_anim(void* pEvt) {
    return 0;
}


s32 evt_npc_set_confuse_anim(void* pEvt) {
    return 0;
}


s32 evt_npc_set_damage_anim(int param_1) {
    return 0;
}


s32 evt_npc_get_drop_item(void* pEvt) {
    return 0;
}


s32 evt_npc_blur_onoff(void* pEvt) {
    return 0;
}


void evt_npc_release_filednpc(void* pEvt) {
}


s32 evt_npc_reverse_ry(void* pEvt) {
    return 0;
}


s32 evt_npc_get_kpencount_type(void* pEvt) {
    return 0;
}


u32 evt_npc_wait_pera(void* pEvt) {
    return 0;
}


s32 evt_npc_get_height(void* pEvt) {
    return 0;
}


void evt_npc_restart_regular_event(void* pEvt) {
}


s32 evt_npc_clear_paper(void* pEvt) {
    return 0;
}


void evt_npc_return_interrupt(void* pEvt) {
}


s32 evt_npc_calc_score(void* pEvt) {
    return 0;
}


s32 evt_npc_set_anim(void* evt, s32 isFirstCall) {
    return 0;
}
