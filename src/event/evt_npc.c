#include "event/evt_npc.h"

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
