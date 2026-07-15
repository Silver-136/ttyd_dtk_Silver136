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


s32 evt_npc_kamek_move_position(EventEntry* event, s32 isFirstCall) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern f32 evtGetFloat(EventEntry* event, s32 value);
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    extern void PSVECSubtract(void* a, void* b, void* out);
    extern f32 angleABf(f32 x1, f32 z1, f32 x2, f32 z2);
    extern void* fbatGetPointer(void);
    extern void npcTuningRy(void* npc, f32 ry);
    extern f64 sin(f64 x);
    extern f64 cos(f64 x);
    extern f32 sqrtf(f32 x);
    extern u8 _kamek_houki_kemuri(s32 npc, s32 animState);
    extern f32 float_0_8041fd84;
    extern f32 float_1_8041fd88;
    extern f32 float_0p1_8041fd98;
    extern f32 float_20_8041fd9c;
    extern f32 float_1000_8041fd94;
    extern f32 float_6p2832_8041fda0;
    extern f32 float_360_8041fda4;
    extern f32 float_3p1416_8041fda8;
    extern f32 float_2_8041fdac;

    s32* args;
    void* npc;
    void* fbat;
    s32 name;
    f32 targetX;
    f32 targetZ;
    f32 arc;
    f32 duration;
    f32 step;
    s32 flags;
    f32 delta[3];
    f32 totalDist;
    f32 remainDist;
    f32 movedDist;
    f32 angle;
    f32 speed;
    f32 phase0;
    f32 phase1;
    f32 s0;
    f32 s1;
    f32 c0;
    f32 zero;

    args = event->args;
    name = evtGetValue(event, args[0]);
    targetX = evtGetFloat(event, args[1]);
    targetZ = evtGetFloat(event, args[2]);
    arc = evtGetFloat(event, args[3]);
    duration = (f32)evtGetValue(event, args[4]) / float_1000_8041fd94;
    step = evtGetFloat(event, args[5]);
    flags = args[6];
    zero = float_0_8041fd84;

    npc = evtNpcNameToPtr(event, name);
    *(u32*)((s32)npc + 0x1D4) |= 0x10;
    if ((flags & 2) != 0) {
        *(u32*)((s32)npc + 0x1D4) |= 0x1000;
    }

    if (isFirstCall != 0) {
        *(s32*)((s32)npc + 0x18C) = 0;
        *(s32*)((s32)npc + 0x188) = 0;
        *(s32*)((s32)npc + 0x108) = 0;

        *(f32*)((s32)npc + 0x164) = targetX;
        *(f32*)((s32)npc + 0x168) = *(f32*)((s32)npc + 0x90);
        *(f32*)((s32)npc + 0x16C) = targetZ;

        *(s32*)((s32)npc + 0x158) = *(s32*)((s32)npc + 0x8C);
        *(s32*)((s32)npc + 0x15C) = *(s32*)((s32)npc + 0x90);
        *(s32*)((s32)npc + 0x160) = *(s32*)((s32)npc + 0x94);

        PSVECSubtract((void*)((s32)npc + 0x164), (void*)((s32)npc + 0x158), delta);
        totalDist = sqrtf(delta[0] * delta[0] + delta[2] * delta[2]);
        *(f32*)((s32)npc + 0x1AC) = angleABf(zero, zero, delta[0], delta[2]);
        *(f32*)((s32)npc + 0x1B0) = step;
        *(f32*)((s32)npc + 0x1C0) = duration;
        *(f32*)((s32)npc + 0x1B8) = zero;

        if ((*(f32*)((s32)npc + 0x1B0) <= float_0p1_8041fd98) && (zero == *(f32*)((s32)npc + 0x1C0))) {
            *(f32*)((s32)npc + 0x1B0) = float_1_8041fd88;
        }
        if (*(f32*)((s32)npc + 0x1B0) <= float_0p1_8041fd98) {
            *(f32*)((s32)npc + 0x1B0) = totalDist / *(f32*)((s32)npc + 0x1C0);
            *(f32*)((s32)npc + 0x1C0) = zero;
        }
    }

    if (((flags & 4) == 0) || (*(s32*)((s32)npc + 0x2FC) == 0)) {
        PSVECSubtract((void*)((s32)npc + 0x164), (void*)((s32)npc + 0x158), delta);
        totalDist = sqrtf(delta[0] * delta[0] + delta[2] * delta[2]);

        PSVECSubtract((void*)((s32)npc + 0x164), (void*)((s32)npc + 0x8C), delta);
        remainDist = sqrtf(delta[0] * delta[0] + delta[2] * delta[2]);

        if (*(f32*)((s32)npc + 0x1C0) == zero) {
            if (remainDist <= (float_20_8041fd9c * *(f32*)((s32)npc + 0x1B0)) / float_1000_8041fd94) {
                *(s32*)((s32)npc + 0x98) = *(s32*)((s32)npc + 0x164);
                *(s32*)((s32)npc + 0x9C) = *(s32*)((s32)npc + 0x168);
                *(s32*)((s32)npc + 0xA0) = *(s32*)((s32)npc + 0x16C);
                *(s32*)((s32)npc + 0x8C) = *(s32*)((s32)npc + 0x98);
                *(s32*)((s32)npc + 0x90) = *(s32*)((s32)npc + 0x9C);
                *(s32*)((s32)npc + 0x94) = *(s32*)((s32)npc + 0xA0);
                *(f32*)((s32)npc + 0x1B0) = zero;
                return 2;
            }
        } else if (*(f32*)((s32)npc + 0x1C0) <= *(f32*)((s32)npc + 0x1A0)) {
            *(s32*)((s32)npc + 0x98) = *(s32*)((s32)npc + 0x164);
            *(s32*)((s32)npc + 0x9C) = *(s32*)((s32)npc + 0x168);
            *(s32*)((s32)npc + 0xA0) = *(s32*)((s32)npc + 0x16C);
            *(s32*)((s32)npc + 0x8C) = *(s32*)((s32)npc + 0x98);
            *(s32*)((s32)npc + 0x90) = *(s32*)((s32)npc + 0x9C);
            *(s32*)((s32)npc + 0x94) = *(s32*)((s32)npc + 0xA0);
            *(f32*)((s32)npc + 0x1B0) = zero;
            return 2;
        }

        if (((flags & 1) != 0) && ((*(u32*)((s32)npc + 0x1D4) & 1) == 0)) {
            *(f32*)((s32)npc + 0x1AC) = angleABf(zero, zero, delta[0], delta[2]);
        }

        if (((*(u32*)((s32)npc + 0x1D4) & 2) != 0) && (*(f32*)((s32)npc + 0x1B0) > float_0p1_8041fd98)) {
            fbat = fbatGetPointer();
            if ((npc != *(void**)((s32)fbat + 8)) || ((*(s16*)fbat != 5) && (*(s16*)fbat != 6))) {
                npcTuningRy(npc, *(f32*)((s32)npc + 0x1AC));
            }
        }

        angle = *(f32*)((s32)npc + 0x1AC);
        speed = *(f32*)((s32)npc + 0x1B0);
        s0 = (f32)sin((f64)((float_6p2832_8041fda0 * angle) / float_360_8041fda4));
        *(f32*)((s32)npc + 0x1B4) += *(f32*)((s32)npc + 0x1A4) * (speed * s0);
        c0 = (f32)cos((f64)((float_6p2832_8041fda0 * angle) / float_360_8041fda4));
        *(f32*)((s32)npc + 0x1BC) += *(f32*)((s32)npc + 0x1A4) * (speed * -c0);

        movedDist = totalDist - remainDist;
        phase0 = float_3p1416_8041fda8 * (float_2_8041fdac * (movedDist / totalDist));
        s0 = (f32)sin((f64)phase0);
        phase1 = float_3p1416_8041fda8 * (float_2_8041fdac * (((speed * *(f32*)((s32)npc + 0x1A4)) + movedDist) / totalDist));
        s1 = (f32)sin((f64)phase1);
        *(f32*)((s32)npc + 0x1B8) += arc * s1 - arc * s0;

        if ((flags & 8) == 0) {
            _kamek_houki_kemuri((s32)npc, *(s32*)((s32)npc + 0x108));
        }
        return 0;
    }
    return 2;
}

s32 evt_npc_homing_target(EventEntry* event, s32 isFirstCall) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern f32 evtGetFloat(EventEntry* event, s32 value);
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    extern s32 strcmp(const char* a, const char* b);
    extern void* marioGetPtr(void);
    extern f32 angleABf(f32 x0, f32 z0, f32 x1, f32 z1);
    extern f32 compAngle(f32 a, f32 b);
    extern f32 reviseAngle(f32 a);
    extern void npcTuningRy(void* npc, f32 ry);
    extern void PSVECSubtract(void* a, void* b, void* out);
    extern void* fbatGetPointer(void);
    extern f64 sin(f64 x);
    extern f64 cos(f64 x);
    extern f32 sqrtf(f32 x);
    extern u32 psndGetFlag(void);
    extern u32 animPoseGetMaterialFlag(s32 poseId);
    extern s32 searchPSSFXList(s32 id);
    extern void psndSFXOnEx_3D(s32 id, void* pos, s32 a, s32 b, s32 c, s32 d);
    extern f32 float_0_8041fd84;
    extern f32 float_1000_8041fd94;
    extern f32 float_60_8041fdf8;
    extern f32 float_6p2832_8041fda0;
    extern f32 float_360_8041fda4;
    extern char str_mario_8041fdd4[];

    s32* args;
    s32 npcName;
    s32 targetName;
    f32 durationSec;
    f32 speedParam;
    f32 stopDist;
    f32 turnScale;
    u32 flags;
    void* npc;
    void* target;
    void* fbat;
    f32 delta[3];
    f32 dist;
    f32 dx;
    f32 dz;
    f32 targetAngle;
    f32 angleDiff;
    f32 turnAmount;
    s32 soundMask;
    s32 sfx;
    s32 userBase;

    args = event->args;
    npcName = evtGetValue(event, args[0]);
    targetName = evtGetValue(event, args[1]);
    durationSec = (f32)evtGetValue(event, args[2]) / float_1000_8041fd94;
    speedParam = evtGetFloat(event, args[3]);
    stopDist = evtGetFloat(event, args[4]);
    turnScale = evtGetFloat(event, args[5]);
    flags = args[6];
    npc = evtNpcNameToPtr(event, npcName);
    *(u32*)((s32)npc + 0x1D4) |= 0x10;
    userBase = (s32)event + 0x78;

    if (isFirstCall != 0) {
        *(s32*)((s32)npc + 0x18C) = 0;
        *(s32*)((s32)npc + 0x188) = 0;
        *(s32*)((s32)npc + 0x108) = 0;

        if (strcmp((const char*)targetName, str_mario_8041fdd4) == 0) {
            *(s32*)userBase = 0;
            target = marioGetPtr();
            *(void**)(userBase + 4) = target;
            target = *(void**)(userBase + 4);
            dx = *(f32*)((s32)target + 0x8C) - *(f32*)((s32)npc + 0x8C);
            dz = *(f32*)((s32)target + 0x94) - *(f32*)((s32)npc + 0x94);
        } else {
            *(s32*)userBase = 1;
            target = evtNpcNameToPtr(event, targetName);
            *(void**)(userBase + 4) = target;
            target = *(void**)(userBase + 4);
            dx = *(f32*)((s32)target + 0x8C) - *(f32*)((s32)npc + 0x8C);
            dz = *(f32*)((s32)target + 0x94) - *(f32*)((s32)npc + 0x94);
        }

        targetAngle = angleABf(float_0_8041fd84, float_0_8041fd84, dx, dz);
        *(f32*)((s32)npc + 0x1AC) = targetAngle;
        *(f32*)((s32)npc + 0x1B0) = speedParam;
        if (speedParam == float_0_8041fd84) {
            return 2;
        }
        *(f32*)((s32)npc + 0x1C0) = durationSec;

        if (((*(u32*)((s32)npc + 0x1D4) & 2) != 0) && (*(f32*)((s32)npc + 0x1B0) != float_0_8041fd84)) {
            fbat = fbatGetPointer();
            if ((npc != *(void**)((s32)fbat + 8)) || ((*(s16*)fbat != 5) && (*(s16*)fbat != 6))) {
                npcTuningRy(npc, *(f32*)((s32)npc + 0x1AC));
            }
        }

        *(f32*)((s32)npc + 0x1DC) = float_0_8041fd84;
        *(s16*)((s32)npc + 0x1D8) = 1;
        return 0;
    }

    if ((*(u32*)((s32)npc + 0x1D4) & 1) == 0) {
        target = *(void**)(userBase + 4);
        *(f32*)((s32)npc + 0x164) = *(f32*)((s32)target + 0x8C);
        *(f32*)((s32)npc + 0x168) = *(f32*)((s32)target + 0x90);
        *(f32*)((s32)npc + 0x16C) = *(f32*)((s32)target + 0x94);

        *(s32*)((s32)npc + 0x158) = *(s32*)((s32)npc + 0x8C);
        *(s32*)((s32)npc + 0x15C) = *(s32*)((s32)npc + 0x90);
        *(s32*)((s32)npc + 0x160) = *(s32*)((s32)npc + 0x94);

        PSVECSubtract((void*)((s32)npc + 0x164), (void*)((s32)npc + 0x158), delta);
        dist = sqrtf(delta[0] * delta[0] + delta[2] * delta[2]);
        targetAngle = angleABf(float_0_8041fd84, float_0_8041fd84, delta[0], delta[2]);
        angleDiff = compAngle(*(f32*)((s32)npc + 0x1AC), targetAngle);
        turnAmount = float_60_8041fdf8 * turnScale * *(f32*)((s32)npc + 0x1B0);
        if (turnAmount < ((angleDiff < float_0_8041fd84) ? -angleDiff : angleDiff)) {
            if (angleDiff <= float_0_8041fd84) {
                *(f32*)((s32)npc + 0x1AC) -= turnAmount;
            } else {
                *(f32*)((s32)npc + 0x1AC) += turnAmount;
            }
            *(f32*)((s32)npc + 0x1AC) = reviseAngle(*(f32*)((s32)npc + 0x1AC));
        } else {
            *(f32*)((s32)npc + 0x1AC) = targetAngle;
        }

        if (((*(u32*)((s32)npc + 0x1D4) & 2) != 0) && (*(f32*)((s32)npc + 0x1B0) != float_0_8041fd84)) {
            fbat = fbatGetPointer();
            if ((npc != *(void**)((s32)fbat + 8)) || ((*(s16*)fbat != 5) && (*(s16*)fbat != 6))) {
                npcTuningRy(npc, *(f32*)((s32)npc + 0x1AC));
            }
        }
    } else {
        dist = float_1000_8041fd94;
    }

    PSVECSubtract((void*)((s32)npc + 0x164), (void*)((s32)npc + 0x8C), delta);
    dx = delta[0];
    dz = delta[2];

    if (((*(f32*)((s32)npc + 0x1C0) != float_0_8041fd84) && (*(f32*)((s32)npc + 0x1A0) >= *(f32*)((s32)npc + 0x1C0))) ||
        ((stopDist > float_0_8041fd84) && (sqrtf(dx * dx + dz * dz) < stopDist)) ||
        (((flags & 4) != 0) && (*(s32*)((s32)npc + 0x2FC) != 0))) {
        *(f32*)((s32)npc + 0x1B0) = float_0_8041fd84;
        return 1;
    }

    targetAngle = *(f32*)((s32)npc + 0x1AC);
    *(f32*)((s32)npc + 0x1B4) += *(f32*)((s32)npc + 0x1B0) * (f32)sin((f64)((float_6p2832_8041fda0 * targetAngle) / float_360_8041fda4));
    *(f32*)((s32)npc + 0x1BC) += *(f32*)((s32)npc + 0x1B0) * -(f32)cos((f64)((float_6p2832_8041fda0 * targetAngle) / float_360_8041fda4));

    soundMask = 0;
    if (((psndGetFlag() & 0x100) != 0) && ((animPoseGetMaterialFlag(*(s32*)((s32)npc + 0x104)) & 0x1800) == 0)) {
        soundMask = 0x10000;
    }
    if ((f32)((f32)*(u16*)((s32)npc + 0x314) / *(f32*)((s32)npc + 0x1B0)) < *(f32*)((s32)npc + 0x1DC)) {
        if (*(s16*)((s32)npc + 0x1D8) == 0) {
            if (*(s32*)((s32)npc + 0x2F0) != 0) {
                sfx = searchPSSFXList(*(s32*)((s32)npc + 0x2F0));
                psndSFXOnEx_3D(soundMask | sfx, (void*)((s32)npc + 0x8C), -1, -1, 0, 8);
            }
        } else if (*(s32*)((s32)npc + 0x2EC) != 0) {
            sfx = searchPSSFXList(*(s32*)((s32)npc + 0x2EC));
            psndSFXOnEx_3D(soundMask | sfx, (void*)((s32)npc + 0x8C), -1, -1, 0, 8);
        }
        *(f32*)((s32)npc + 0x1DC) = float_0_8041fd84;
        *(s16*)((s32)npc + 0x1D8) = (s16)(1 - *(s16*)((s32)npc + 0x1D8));
    }
    if ((*(s32*)((s32)npc + 0x2F0) != 0) || (*(s16*)((s32)npc + 0x1D8) != 0)) {
        *(f32*)((s32)npc + 0x1DC) += float_1000_8041fd94 * *(f32*)((s32)npc + 0x1B0);
    }

    return 0;
}

s32 evt_npc_move_position(EventEntry* event, s32 isFirstCall) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern f32 evtGetFloat(EventEntry* event, s32 value);
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    extern void PSVECSubtract(void* a, void* b, void* out);
    extern f32 angleABf(f32 x0, f32 z0, f32 x1, f32 z1);
    extern void* fbatGetPointer(void);
    extern void npcTuningRy(void* npc, f32 ry);
    extern f64 sin(f64 x);
    extern f64 cos(f64 x);
    extern f64 __frsqrte(f64 x);
    extern u32 psndGetFlag(void);
    extern u32 animPoseGetMaterialFlag(s32 poseId);
    extern s32 searchPSSFXList(s32 id);
    extern void psndSFXOnEx_3D(s32 id, void* pos, s32 a, s32 b, s32 c, s32 d);
    extern f32 float_0_8041fd84;
    extern f32 float_1_8041fd88;
    extern f32 float_20_8041fd9c;
    extern f32 float_1000_8041fd94;
    extern f32 float_0p1_8041fd98;
    extern f32 float_6p2832_8041fda0;
    extern f32 float_360_8041fda4;
    extern f32 float_0p0005_8041fe04;
    extern f32 __float_nan;

    s32* args;
    s32 npcName;
    f32 targetX;
    f32 targetZ;
    f32 duration;
    f32 speed;
    u32 flags;
    void* npc;
    f32 delta[3];
    f32 dist;
    f32 distSq;
    f64 sqrtD;
    f64 inv;
    u32 bits;
    s32 cls;
    union { f32 f; u32 u; } bitsView;
    f32 angle;
    f32 s;
    f32 c;
    s32 soundMask;
    s32 sfx;

    args = event->args;
    npcName = evtGetValue(event, args[0]);
    targetX = evtGetFloat(event, args[1]);
    targetZ = evtGetFloat(event, args[2]);
    duration = (f32)evtGetValue(event, args[3]) / float_1000_8041fd94;
    speed = evtGetFloat(event, args[4]);
    flags = args[5];

    npc = evtNpcNameToPtr(event, npcName);
    *(u32*)((s32)npc + 0x1D4) |= 0x10;
    if ((flags & 2) != 0) {
        *(u32*)((s32)npc + 0x1D4) |= 0x1000;
    }

    if (isFirstCall != 0) {
        *(s32*)((s32)npc + 0x18C) = 0;
        *(s32*)((s32)npc + 0x188) = 0;
        *(s32*)((s32)npc + 0x108) = 0;

        *(f32*)((s32)npc + 0x164) = targetX;
        *(f32*)((s32)npc + 0x168) = *(f32*)((s32)npc + 0x90);
        *(f32*)((s32)npc + 0x16C) = targetZ;

        *(s32*)((s32)npc + 0x158) = *(s32*)((s32)npc + 0x8C);
        *(s32*)((s32)npc + 0x15C) = *(s32*)((s32)npc + 0x90);
        *(s32*)((s32)npc + 0x160) = *(s32*)((s32)npc + 0x94);

        PSVECSubtract((void*)((s32)npc + 0x164), (void*)((s32)npc + 0x158), delta);
        distSq = delta[0] * delta[0] + delta[2] * delta[2];
        sqrtD = (f64)distSq;
        if (sqrtD > (f64)float_0_8041fd84) {
            inv = __frsqrte(sqrtD);
            inv = 0.5 * inv * -((sqrtD * inv * inv) - 3.0);
            inv = 0.5 * inv * -((sqrtD * inv * inv) - 3.0);
            dist = (f32)(sqrtD * 0.5 * inv * -((sqrtD * inv * inv) - 3.0));
        } else {
            if ((f64)float_0_8041fd84 <= sqrtD) {
                bitsView.f = distSq;
                bits = bitsView.u & 0x7F800000;
                if (bits == 0x7F800000) {
                    if ((bitsView.u & 0x7FFFFF) == 0) {
                        cls = 2;
                    } else {
                        cls = 1;
                    }
                } else if ((bits < 0x7F800000) && (bits == 0)) {
                    if ((bitsView.u & 0x7FFFFF) == 0) {
                        cls = 3;
                    } else {
                        cls = 5;
                    }
                } else {
                    cls = 4;
                }
                if (cls == 1) {
                    dist = __float_nan;
                } else {
                    dist = distSq;
                }
            } else {
                dist = __float_nan;
            }
        }
        *(f32*)((s32)npc + 0x1AC) = angleABf(float_0_8041fd84, float_0_8041fd84, delta[0], delta[2]);

        *(f32*)((s32)npc + 0x1B0) = speed;
        *(f32*)((s32)npc + 0x1C0) = duration;
        *(f32*)((s32)npc + 0x1B8) = float_0_8041fd84;

        if ((*(f32*)((s32)npc + 0x1B0) <= float_0p1_8041fd98) && (float_0_8041fd84 == *(f32*)((s32)npc + 0x1C0))) {
            *(f32*)((s32)npc + 0x1B0) = float_1_8041fd88;
        }
        if (*(f32*)((s32)npc + 0x1B0) <= float_0p1_8041fd98) {
            *(f32*)((s32)npc + 0x1B0) = dist / *(f32*)((s32)npc + 0x1C0);
            *(f32*)((s32)npc + 0x1C0) = float_0_8041fd84;
        }

        *(f32*)((s32)npc + 0x1DC) = float_0_8041fd84;
        *(s16*)((s32)npc + 0x1D8) = 1;
        *(s32*)((s32)event + 0x78) = 0;
    }

    *(s32*)((s32)event + 0x78) = *(s32*)((s32)event + 0x78) + 1;
    if (((flags & 4) != 0) && (*(s32*)((s32)npc + 0x2FC) != 0) && (*(s32*)((s32)event + 0x78) > 5)) {
        return 2;
    }

    PSVECSubtract((void*)((s32)npc + 0x164), (void*)((s32)npc + 0x8C), delta);
    distSq = delta[0] * delta[0] + delta[2] * delta[2];
    sqrtD = (f64)distSq;
    if (sqrtD > (f64)float_0_8041fd84) {
        inv = __frsqrte(sqrtD);
        inv = 0.5 * inv * -((sqrtD * inv * inv) - 3.0);
        inv = 0.5 * inv * -((sqrtD * inv * inv) - 3.0);
        dist = (f32)(sqrtD * 0.5 * inv * -((sqrtD * inv * inv) - 3.0));
    } else {
        if ((f64)float_0_8041fd84 <= sqrtD) {
            bitsView.f = distSq;
            bits = bitsView.u & 0x7F800000;
            if (bits == 0x7F800000) {
                if ((bitsView.u & 0x7FFFFF) == 0) {
                    cls = 2;
                } else {
                    cls = 1;
                }
            } else if ((bits < 0x7F800000) && (bits == 0)) {
                if ((bitsView.u & 0x7FFFFF) == 0) {
                    cls = 3;
                } else {
                    cls = 5;
                }
            } else {
                cls = 4;
            }
            if (cls == 1) {
                dist = __float_nan;
            } else {
                dist = distSq;
            }
        } else {
            dist = __float_nan;
        }
    }

    if (*(f32*)((s32)npc + 0x1C0) == float_0_8041fd84) {
        if (dist <= (float_20_8041fd9c * *(f32*)((s32)npc + 0x1B0)) / float_1000_8041fd94) {
            *(f32*)((s32)npc + 0x1B0) = float_0_8041fd84;
            return 2;
        }
    } else if (*(f32*)((s32)npc + 0x1C0) <= *(f32*)((s32)npc + 0x1A0)) {
        *(f32*)((s32)npc + 0x1B0) = float_0_8041fd84;
        return 2;
    }

    if (dist < float_0p0005_8041fe04) {
        *(f32*)((s32)npc + 0x1B0) = float_0_8041fd84;
        return 2;
    }

    if (((*(u32*)((s32)npc + 0x0) & 1) != 0) && ((*(u32*)((s32)npc + 0x1D4) & 1) == 0)) {
        *(f32*)((s32)npc + 0x1AC) = angleABf(float_0_8041fd84, float_0_8041fd84, delta[0], delta[2]);
    }

    if (((*(u32*)((s32)npc + 0x1D4) & 2) != 0) && (*(f32*)((s32)npc + 0x1B0) > float_0p1_8041fd98)) {
        if ((npc != *(void**)((s32)fbatGetPointer() + 8)) ||
            ((*(s16*)fbatGetPointer() != 5) && (*(s16*)fbatGetPointer() != 6))) {
            npcTuningRy(npc, *(f32*)((s32)npc + 0x1AC));
        }
    }

    if ((*(u32*)((s32)npc + 0x1D4) & 0x10000) == 0) {
        angle = *(f32*)((s32)npc + 0x1AC);
        s = (f32)sin((f64)((float_6p2832_8041fda0 * angle) / float_360_8041fda4));
        *(f32*)((s32)npc + 0x1B4) += *(f32*)((s32)npc + 0x1A4) * (*(f32*)((s32)npc + 0x1B0) * s);
        c = (f32)cos((f64)((float_6p2832_8041fda0 * angle) / float_360_8041fda4));
        *(f32*)((s32)npc + 0x1BC) += *(f32*)((s32)npc + 0x1A4) * (*(f32*)((s32)npc + 0x1B0) * -c);
    }

    soundMask = 0;
    if ((psndGetFlag() & 0x100) != 0) {
        if ((animPoseGetMaterialFlag(*(s32*)((s32)npc + 0x104)) & 0x1800) == 0) {
            soundMask = 0x10000;
        }
    }

    if (((f32)*(u16*)((s32)npc + 0x1E8) / *(f32*)((s32)npc + 0x1B0)) < *(f32*)((s32)npc + 0x1DC)) {
        if (*(s16*)((s32)npc + 0x1D8) != 0) {
            if (*(s32*)((s32)npc + 0x1E0) != 0) {
                sfx = searchPSSFXList(*(s32*)((s32)npc + 0x1E0));
                psndSFXOnEx_3D(soundMask | sfx, (void*)((s32)npc + 0x8C), -1, -1, 0, 8);
            }
        } else {
            if (*(s32*)((s32)npc + 0x1E4) != 0) {
                sfx = searchPSSFXList(*(s32*)((s32)npc + 0x1E4));
                psndSFXOnEx_3D(soundMask | sfx, (void*)((s32)npc + 0x8C), -1, -1, 0, 8);
            }
        }
        *(f32*)((s32)npc + 0x1DC) = float_0_8041fd84;
        *(s16*)((s32)npc + 0x1D8) = (s16)(1 - *(s16*)((s32)npc + 0x1D8));
    }

    if ((*(s32*)((s32)npc + 0x1E4) != 0) || (*(s16*)((s32)npc + 0x1D8) != 0)) {
        *(f32*)((s32)npc + 0x1DC) += float_1000_8041fd94 * *(f32*)((s32)npc + 0x1B0);
    }

    return 0;
}

s32 evt_npc_jump_position(EventEntry* event, s32 isFirstCall) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern f32 evtGetFloat(EventEntry* event, s32 value);
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    extern void PSVECSubtract(void* a, void* b, void* out);
    extern f32 angleABf(f32 x0, f32 z0, f32 x1, f32 z1);
    extern u32 psndGetFlag(void);
    extern u32 animPoseGetMaterialFlag(s32 poseId);
    extern s32 searchPSSFXList(s32 id);
    extern void psndSFXOnEx_3D(s32 id, void* pos, s32 a, s32 b, s32 c, s32 d);
    extern void* fbatGetPointer(void);
    extern void npcTuningRy(void* npc, f32 ry);
    extern f64 sin(f64 x);
    extern f64 cos(f64 x);
    extern f32 sqrtf(f32 x);
    extern f32 float_0_8041fd84;
    extern f32 float_1_8041fd88;
    extern f32 float_0p5_8041fd90;
    extern f32 float_1000_8041fd94;
    extern f32 float_0p1_8041fd98;
    extern f32 float_6p2832_8041fda0;
    extern f32 float_360_8041fda4;
    extern f32 float_neg980_8041fe00;

    s32* args;
    s32 npcName;
    f32 targetX;
    f32 targetY;
    f32 targetZ;
    f32 duration;
    f32 speed;
    f32 height;
    s32 mode;
    void* npc;
    void* fbat;
    f32 diff[3];
    f32 dist;
    f32 dy;
    f32 halfTime;
    f32 denom;
    f32 angle;
    f32 s;
    f32 c;
    s32 soundMask;
    s32 sfx;
    f32 zero;
    f32 one;

    args = event->args;
    npcName = evtGetValue(event, args[0]);
    targetX = evtGetFloat(event, args[1]);
    targetY = evtGetFloat(event, args[2]);
    targetZ = evtGetFloat(event, args[3]);
    duration = (f32)evtGetValue(event, args[4]) / float_1000_8041fd94;
    speed = evtGetFloat(event, args[5]);
    height = evtGetFloat(event, args[6]);
    mode = evtGetValue(event, args[7]);

    npc = evtNpcNameToPtr(event, npcName);
    *(u32*)((s32)npc + 0x1D4) |= 0x10;
    *(u32*)((s32)npc + 0x1D4) |= 0x1000;

    zero = float_0_8041fd84;
    one = float_1_8041fd88;

    if (isFirstCall != 0) {
        *(s32*)((s32)npc + 0x18C) = 0;
        *(s32*)((s32)npc + 0x188) = 0;
        *(s32*)((s32)npc + 0x17C) = 0;
        *(s32*)((s32)npc + 0x178) = 0;
        *(s32*)((s32)npc + 0x108) = 0;

        *(f32*)((s32)npc + 0x164) = targetX;
        *(f32*)((s32)npc + 0x168) = targetY;
        *(f32*)((s32)npc + 0x16C) = targetZ;

        *(s32*)((s32)npc + 0x158) = *(s32*)((s32)npc + 0x8C);
        *(s32*)((s32)npc + 0x15C) = *(s32*)((s32)npc + 0x90);
        *(s32*)((s32)npc + 0x160) = *(s32*)((s32)npc + 0x94);

        PSVECSubtract((void*)((s32)npc + 0x164), (void*)((s32)npc + 0x158), diff);
        dist = sqrtf(diff[0] * diff[0] + diff[2] * diff[2]);
        *(f32*)((s32)npc + 0x1AC) = angleABf(zero, zero, diff[0], diff[2]);

        *(f32*)((s32)npc + 0x1B0) = speed;
        *(f32*)((s32)npc + 0x1C0) = zero;
        *(f32*)((s32)npc + 0x1B8) = zero;

        soundMask = 0;
        if ((psndGetFlag() & 0x100) != 0) {
            if ((animPoseGetMaterialFlag(*(s32*)((s32)npc + 0x104)) & 0x1800) == 0) {
                soundMask = 0x10000;
            }
        }
        if (*(s32*)((s32)npc + 0x1EC) != 0) {
            sfx = searchPSSFXList(*(s32*)((s32)npc + 0x1EC));
            psndSFXOnEx_3D(soundMask | sfx, (void*)((s32)npc + 0x8C), -1, -1, 0, 8);
        }

        *(f32*)((s32)npc + 0x1C4) = zero;
        *(f32*)((s32)npc + 0x1C8) = one;

        dy = targetY - *(f32*)((s32)npc + 0x15C);
        if (height == zero) {
            if (zero <= dy) {
                return 1;
            }
            if (mode != 1) {
                if (mode != 0) {
                    return 1;
                }
                if (*(f32*)((s32)npc + 0x1B0) <= float_0p1_8041fd98) {
                    if (zero < duration) {
                        *(f32*)((s32)npc + 0x1CC) = (dy / (duration * duration)) / float_neg980_8041fe00;
                    }
                } else {
                    if (duration <= zero) {
                        duration = dist / *(f32*)((s32)npc + 0x1B0);
                    }
                    if (duration <= zero) {
                        *(f32*)((s32)npc + 0x1C8) = one;
                    } else {
                        denom = duration * (float_neg980_8041fe00 * *(f32*)((s32)npc + 0x1CC) * duration);
                        if (denom != zero) {
                            *(f32*)((s32)npc + 0x1C8) = dy / denom;
                        }
                    }
                }
            }
        } else {
            if (zero < dy) {
                height += dy;
            }
            *(f32*)((s32)npc + 0x1C4) = zero;
            if (mode == 1) {
                if (duration == zero) {
                    *(f32*)((s32)npc + 0x1C4) = height;
                } else {
                    halfTime = float_0p5_8041fd90 * duration;
                    *(f32*)((s32)npc + 0x1C4) = -(duration * (float_neg980_8041fe00 * *(f32*)((s32)npc + 0x1CC) * duration)) / duration;
                    denom = *(f32*)((s32)npc + 0x1C4) * halfTime + halfTime * float_neg980_8041fe00 * *(f32*)((s32)npc + 0x1CC) * halfTime;
                    if (denom != zero) {
                        *(f32*)((s32)npc + 0x1C8) = height / denom;
                    }
                }
            } else {
                if (mode != 0) {
                    return 1;
                }
                if (duration == zero) {
                    *(f32*)((s32)npc + 0x1C4) = height;
                } else {
                    if (*(f32*)((s32)npc + 0x1B0) <= float_0p1_8041fd98) {
                        *(f32*)((s32)npc + 0x1B0) = dist / duration;
                        denom = duration;
                    } else {
                        denom = dist / *(f32*)((s32)npc + 0x1B0);
                        if (denom == zero) {
                            denom = duration;
                        }
                    }
                    if (denom == zero) {
                        denom = one;
                    }
                    halfTime = float_0p5_8041fd90 * denom;
                    *(f32*)((s32)npc + 0x1C4) = -(denom * (float_neg980_8041fe00 * *(f32*)((s32)npc + 0x1CC) * denom)) / denom;
                    denom = *(f32*)((s32)npc + 0x1C4) * halfTime + halfTime * float_neg980_8041fe00 * *(f32*)((s32)npc + 0x1CC) * halfTime;
                    if (denom != zero) {
                        *(f32*)((s32)npc + 0x1C8) = height / denom;
                    }
                }
            }
        }
        return 0;
    }

    if (((*(u32*)((s32)npc + 0x1D4) & 2) != 0) && (*(f32*)((s32)npc + 0x1B0) > float_0p1_8041fd98)) {
        fbat = fbatGetPointer();
        if ((npc != *(void**)((s32)fbat + 8)) || ((*(s16*)fbat != 5) && (*(s16*)fbat != 6))) {
            npcTuningRy(npc, *(f32*)((s32)npc + 0x1AC));
        }
    }

    angle = *(f32*)((s32)npc + 0x1AC);
    s = (f32)sin((f64)((float_6p2832_8041fda0 * angle) / float_360_8041fda4));
    *(f32*)((s32)npc + 0x1B4) += *(f32*)((s32)npc + 0x1A4) * (*(f32*)((s32)npc + 0x1B0) * s);
    c = (f32)cos((f64)((float_6p2832_8041fda0 * angle) / float_360_8041fda4));
    *(f32*)((s32)npc + 0x1BC) += *(f32*)((s32)npc + 0x1A4) * (*(f32*)((s32)npc + 0x1B0) * -c);

    if ((*(u32*)((s32)npc + 0x1D4) & 1) != 0) {
        return 0;
    }

    *(f32*)((s32)npc + 0x1B0) = zero;
    *(f32*)((s32)npc + 0x1C4) = zero;
    *(f32*)((s32)npc + 0x1C8) = one;
    *(f32*)((s32)npc + 0x1CC) = one;

    soundMask = 0;
    if ((psndGetFlag() & 0x100) != 0) {
        if ((animPoseGetMaterialFlag(*(s32*)((s32)npc + 0x104)) & 0x1800) == 0) {
            soundMask = 0x10000;
        }
    }
    if (*(s32*)((s32)npc + 0x1F0) != 0) {
        sfx = searchPSSFXList(*(s32*)((s32)npc + 0x1F0));
        psndSFXOnEx_3D(soundMask | sfx, (void*)((s32)npc + 0x8C), -1, -1, 0, 8);
    }
    return 1;
}

s32 evt_npc_glide_position(EventEntry* event, s32 isFirstCall) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern f32 evtGetFloat(EventEntry* event, s32 value);
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    extern void PSVECSubtract(void* a, void* b, void* out);
    extern f32 _intplGetFloat(f32 value, s32 kind);
    extern void* fbatGetPointer(void);
    extern f32 angleABf(f32 x1, f32 z1, f32 x2, f32 z2);
    extern void npcTuningRy(void* npc, f32 ry);
    extern f64 sin(f64 x);
    extern u32 psndGetFlag(void);
    extern u32 animPoseGetMaterialFlag(s32 poseId);
    extern s32 searchPSSFXList(s32 id);
    extern void psndSFXOnEx_3D(s32 id, void* pos, s32 a, s32 b, s32 c, s32 d);
    extern f64 sqrt(f64);
    extern f32 float_0_8041fd84;
    extern f32 float_0p1_8041fd98;
    extern f32 float_1000_8041fd94;
    extern f32 float_3p1416_8041fda8;
    s32* args;
    void* npc;
    void* fbat;
    s32 name;
    f32 targetX;
    f32 targetY;
    f32 targetZ;
    f32 durationMs;
    f32 speed;
    f32 arc;
    s32 mode;
    s32 flags;
    f32 delta[3];
    f32 dist;
    f32 denom;
    f32 now;
    f32 old;
    f32 dt;
    f32 dx;
    f32 dz;
    s32 soundMask;
    s32 sfx;

    args = event->args;
    name = evtGetValue(event, args[0]);
    targetX = evtGetFloat(event, args[1]);
    targetY = evtGetFloat(event, args[2]);
    targetZ = evtGetFloat(event, args[3]);
    durationMs = (f32)evtGetValue(event, args[4]) / float_1000_8041fd94;
    speed = evtGetFloat(event, args[5]);
    arc = evtGetFloat(event, args[6]);
    mode = evtGetValue(event, args[7]);
    flags = args[8];
    npc = evtNpcNameToPtr(event, name);

    *(u32*)((s32)npc + 0x1D4) |= 0x10;
    if ((flags & 2) == 0) {
        *(u32*)((s32)npc + 0x1D4) |= 0x1000;
    }
    if (isFirstCall != 0) {
        *(s32*)((s32)npc + 0x188) = 0;
        *(s32*)((s32)npc + 0x18C) = 0;
        *(s32*)((s32)npc + 0x108) = 0;
        *(f32*)((s32)npc + 0x164) = targetX;
        *(f32*)((s32)npc + 0x168) = targetY;
        *(f32*)((s32)npc + 0x16C) = targetZ;
        *(s32*)((s32)npc + 0x158) = *(s32*)((s32)npc + 0x8C);
        *(s32*)((s32)npc + 0x15C) = *(s32*)((s32)npc + 0x90);
        *(s32*)((s32)npc + 0x160) = *(s32*)((s32)npc + 0x94);
        PSVECSubtract((void*)((s32)npc + 0x164), (void*)((s32)npc + 0x158), delta);
        dist = (f32)sqrt((double)(delta[0] * delta[0] + delta[1] * delta[1] + delta[2] * delta[2]));
        if (durationMs <= float_0_8041fd84) {
            if (speed == float_0_8041fd84) {
                return 1;
            }
            if (dist == float_0_8041fd84) {
                return 1;
            }
            durationMs = dist / speed;
        }
        *(f32*)((s32)npc + 0x178) = durationMs;
        *(f32*)((s32)npc + 0x17C) = speed;
        if (*(f32*)((s32)npc + 0x17C) <= float_0p1_8041fd98) {
            *(f32*)((s32)npc + 0x17C) = dist / *(f32*)((s32)npc + 0x178);
        }
        *(f32*)((s32)npc + 0x198) = float_0_8041fd84;
        *(s16*)((s32)npc + 0x1A6) = 1;
        return 0;
    }
    if ((flags & 1) != 0 && *(s32*)((s32)npc + 0x18C) != 0) {
        return 2;
    }
    now = *(f32*)((s32)npc + 0x1A0);
    denom = *(f32*)((s32)npc + 0x178);
    speed = *(f32*)((s32)npc + 0x170);
    if (denom <= now) {
        if (denom < now) {
            speed -= denom - now;
            if (speed <= float_0_8041fd84) {
                return 2;
            }
        }
        old = _intplGetFloat((now - speed) / denom, mode);
        now = _intplGetFloat(now / denom, mode);
    } else {
        old = _intplGetFloat((now - speed) / denom, mode);
        now = _intplGetFloat(now / denom, mode);
    }
    dt = now - old;
    dx = *(f32*)((s32)npc + 0x164) - *(f32*)((s32)npc + 0x158);
    dz = *(f32*)((s32)npc + 0x16C) - *(f32*)((s32)npc + 0x160);
    *(f32*)((s32)npc + 0x180) += dx * dt;
    *(f32*)((s32)npc + 0x184) += dz * dt;
    if ((*(u32*)((s32)npc + 0x1D4) & 2) != 0 && (dx != float_0_8041fd84 || dz != float_0_8041fd84)) {
        fbat = fbatGetPointer();
        if (npc != *(void**)((s32)fbat + 0x48) || (*(s16*)fbat != 5 && *(s16*)fbat != 6)) {
            npcTuningRy(npc, angleABf(float_0_8041fd84, float_0_8041fd84, dx, dz));
        }
    }
    *(f32*)((s32)npc + 0x17C) += (*(f32*)((s32)npc + 0x168) - *(f32*)((s32)npc + 0x15C)) * dt;
    *(f32*)((s32)npc + 0x17C) += arc * (f32)sin((double)(float_3p1416_8041fda8 * now)) - arc * (f32)sin((double)(float_3p1416_8041fda8 * old));
    soundMask = 0;
    if ((psndGetFlag() & 0x100) != 0 && (animPoseGetMaterialFlag(*(s32*)((s32)npc + 0x104)) & 0x1800) == 0) {
        soundMask = 0x10000;
    }
    if ((f32)((f32)*(u16*)((s32)npc + 0x314) / *(f32*)((s32)npc + 0x17C)) < *(f32*)((s32)npc + 0x198)) {
        if (*(s16*)((s32)npc + 0x1A6) == 0) {
            if (*(s32*)((s32)npc + 0x2F0) != 0) {
                sfx = searchPSSFXList(*(s32*)((s32)npc + 0x2F0));
                psndSFXOnEx_3D(soundMask | sfx, (void*)((s32)npc + 0x8C), -1, -1, 0, 8);
            }
        } else if (*(s32*)((s32)npc + 0x2EC) != 0) {
            sfx = searchPSSFXList(*(s32*)((s32)npc + 0x2EC));
            psndSFXOnEx_3D(soundMask | sfx, (void*)((s32)npc + 0x8C), -1, -1, 0, 8);
        }
        *(f32*)((s32)npc + 0x198) = float_0_8041fd84;
        *(s16*)((s32)npc + 0x1A6) = (s16)(1 - *(s16*)((s32)npc + 0x1A6));
    }
    if (*(s32*)((s32)npc + 0x2F0) != 0 || *(s16*)((s32)npc + 0x1A6) != 0) {
        *(f32*)((s32)npc + 0x198) += float_1000_8041fd94 * *(f32*)((s32)npc + 0x170);
    }
    if (denom <= *(f32*)((s32)npc + 0x1A0)) {
        return 1;
    }
    return 0;
}

s32 evt_npc_jump_position_nohit(EventEntry* event, s32 isFirstCall) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    extern u32 psndGetFlag(void);
    extern u32 animPoseGetMaterialFlag(s32 poseId);
    extern s32 searchPSSFXList(s32 id);
    extern void psndSFXOnEx_3D(s32 id, void* pos, s32 a, s32 b, s32 c, s32 d);
    extern f64 intplGetValue(f64 start, f64 end, s32 type, s32 cur, s32 max);
    extern u32 DAT_800000f8;
    extern f32 float_neg4_8041fdfc;
    s32* args;
    void* npc;
    f32 x;
    f32 y;
    f32 z;
    s32 duration;
    f32 gravityArg;
    s32 elapsed;
    f32 t;
    u32 soundFlag;
    u32 savedFlags;
    s32 sfxMask;
    s32 sfx;

    args = event->args;
    npc = evtNpcNameToPtr(event, evtGetValue(event, args[0]));
    x = (f32)evtGetValue(event, args[1]);
    y = (f32)evtGetValue(event, args[2]);
    z = (f32)evtGetValue(event, args[3]);
    duration = evtGetValue(event, args[4]);
    gravityArg = (f32)evtGetValue(event, args[5]);
    if (isFirstCall != 0) {
        *(s32*)((s32)event + 0x198) = *(s32*)((s32)event + 0);
        *(s32*)((s32)event + 0x19C) = *(s32*)((s32)event + 4);
        *(s32*)((s32)npc + 0x158) = *(s32*)((s32)npc + 0x8C);
        *(s32*)((s32)npc + 0x15C) = *(s32*)((s32)npc + 0x90);
        *(s32*)((s32)npc + 0x160) = *(s32*)((s32)npc + 0x94);
        *(f32*)((s32)event + 0x7C) = float_neg4_8041fdfc * gravityArg;
        *(f32*)((s32)event + 0x80) = (y - *(f32*)((s32)npc + 0x15C)) - *(f32*)((s32)event + 0x7C);
        savedFlags = *(u32*)npc & 0x80000;
        *(u32*)((s32)event + 0x84) = savedFlags;
        *(u32*)npc &= ~0x80000;
        sfxMask = 0;
        soundFlag = psndGetFlag();
        if ((soundFlag & 0x100) != 0 && (animPoseGetMaterialFlag(*(s32*)((s32)npc + 0x104)) & 0x1800) == 0) {
            sfxMask = 0x10000;
        }
        if (*(s32*)((s32)npc + 0x300) != 0) {
            sfx = searchPSSFXList(*(s32*)((s32)npc + 0x300));
            psndSFXOnEx_3D(sfxMask | sfx, (void*)((s32)npc + 0x8C), -1, -1, 0, 8);
        }
    }
    elapsed = (s32)((*(u32*)((s32)event + 0) - *(u32*)((s32)event + 0x198)) / (DAT_800000f8 / 4000));
    if (elapsed < duration) {
        t = (f32)elapsed / (f32)duration;
        *(f32*)((s32)npc + 0x8C) = (f32)intplGetValue(*(f32*)((s32)npc + 0x158), x, 0, elapsed, duration);
        *(f32*)((s32)npc + 0x94) = (f32)intplGetValue(*(f32*)((s32)npc + 0x160), z, 0, elapsed, duration);
        *(f32*)((s32)npc + 0x90) = t * (*(f32*)((s32)event + 0x7C) * t + *(f32*)((s32)event + 0x80)) + *(f32*)((s32)npc + 0x15C);
        return 0;
    }
    *(f32*)((s32)npc + 0x8C) = x;
    *(f32*)((s32)npc + 0x90) = y;
    *(f32*)((s32)npc + 0x94) = z;
    *(u32*)npc |= *(u32*)((s32)event + 0x84);
    sfxMask = 0;
    soundFlag = psndGetFlag();
    if ((soundFlag & 0x100) != 0 && (animPoseGetMaterialFlag(*(s32*)((s32)npc + 0x104)) & 0x1800) == 0) {
        sfxMask = 0x10000;
    }
    if (*(s32*)((s32)npc + 0x2FC) != 0) {
        sfx = searchPSSFXList(*(s32*)((s32)npc + 0x2FC));
        psndSFXOnEx_3D(sfxMask | sfx, (void*)((s32)npc + 0x8C), -1, -1, 0, 8);
    }
    return 2;
}

s32 evt_npc_majo_disp_on(EventEntry* event, s32 isFirstCall) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern f32 evtGetFloat(EventEntry* event, s32 value);
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    extern s32 animGroupBaseAsync(void* name, s32 battle, s32 flags);
    extern void animPoseSetEffect(s32 poseId, void* name, s32 flag);
    extern void animPoseSetEffectAnim(s32 poseId, void* name, s32 flag);
    extern void animPosePaperPeraOn(s32 poseId);
    extern void animPoseSetLocalTime(s32 poseId, f32 time);
    extern s32 psndSFXOn_3D(s32 id, void* pos);
    extern s32 animPoseGetVivianType(s32 poseId);
    extern void* animPoseGetAnimPosePtr(s32 poseId);
    extern f32 animPoseGetLoopTimes(s32 poseId);
    extern void animPoseSetPaperAnimGroup(s32 poseId, s32 anim, s32 flag);
    extern void* gp;
    extern char str_p_bibi_802c14d8[];
    extern char str_PM_B_2_802c14e8[];
    extern f32 float_0_8041fd84;
    extern f32 float_1_8041fd88;
    extern f32 float_3_8041fdbc;
    extern f32 float_10_8041fdc0;
    extern f32 float_0p66667_8041fdc8;
    s32* args;
    s32 name;
    f32 x;
    f32 y;
    f32 z;
    s32 skip;
    s32 battle;
    void* npc;
    s32 state;
    void* pose;

    args = event->args;
    name = evtGetValue(event, args[0]);
    x = evtGetFloat(event, args[1]);
    y = evtGetFloat(event, args[2]);
    z = evtGetFloat(event, args[3]);
    skip = evtGetValue(event, args[4]);
    battle = (*(s32*)((s32)gp + 0x14) != 0);
    npc = evtNpcNameToPtr(event, name);

    if (isFirstCall != 0) {
        *(s32*)((s32)event + 0x78) = -1;
        *(s32*)((s32)event + 0x7C) = 0;
    }
    if (animGroupBaseAsync(str_p_bibi_802c14d8, battle, 0) == 0) {
        return 0;
    }
    if (skip != 0) {
        return 0;
    }

    state = *(s32*)((s32)event + 0x78);
    switch (state) {
        case -1:
            *(f32*)((s32)npc + 0x98) = x;
            *(f32*)((s32)npc + 0x9C) = y;
            *(f32*)((s32)npc + 0xA0) = z;
            *(s32*)((s32)npc + 0x8C) = *(s32*)((s32)npc + 0x98);
            *(s32*)((s32)npc + 0x90) = *(s32*)((s32)npc + 0x9C);
            *(s32*)((s32)npc + 0x94) = *(s32*)((s32)npc + 0xA0);
            animPoseSetEffect(*(s32*)((s32)npc + 0x104), str_p_bibi_802c14d8, 1);
            animPoseSetEffectAnim(*(s32*)((s32)npc + 0x104), str_PM_B_2_802c14e8, 1);
            animPosePaperPeraOn(*(s32*)((s32)npc + 0x104));
            animPoseSetLocalTime(*(s32*)((s32)npc + 0x104), float_0_8041fd84);
            *(s32*)((s32)event + 0x78) += 1;
            psndSFXOn_3D(0x275, (void*)((s32)npc + 0x8C));
            break;
        case 0:
            *(f32*)((s32)npc + 0x154) += (float_3_8041fdbc - *(f32*)((s32)npc + 0x154)) / float_10_8041fdc0;
            animPoseSetLocalTime(*(s32*)((s32)npc + 0x104), float_0_8041fd84);
            *(s32*)((s32)event + 0x7C) += 1;
            if (*(s32*)((s32)event + 0x7C) > 0x1E) {
                *(s32*)((s32)event + 0x78) += 1;
                *(s32*)((s32)event + 0x7C) = 0;
                state = animPoseGetVivianType(*(s32*)((s32)npc + 0x104));
                if (state == 1) {
                    psndSFXOn_3D(0x937, (void*)((s32)npc + 0x8C));
                } else if (state == 2) {
                    psndSFXOn_3D(0x27B, (void*)((s32)npc + 0x8C));
                } else if (state == 3) {
                    psndSFXOn_3D(0x278, (void*)((s32)npc + 0x8C));
                }
            }
            break;
        case 1:
            animPoseSetEffect(*(s32*)((s32)npc + 0x104), str_p_bibi_802c14d8, 1);
            animPoseSetEffectAnim(*(s32*)((s32)npc + 0x104), str_PM_B_2_802c14e8, 1);
            animPosePaperPeraOn(*(s32*)((s32)npc + 0x104));
            *(s32*)((s32)event + 0x78) += 1;
            break;
        case 2:
            pose = animPoseGetAnimPosePtr(*(s32*)((s32)npc + 0x104));
            if (animPoseGetLoopTimes(*(s32*)((s32)pose + 0x90)) >= float_0p66667_8041fdc8) {
                animPoseSetPaperAnimGroup(*(s32*)((s32)npc + 0x104), 0, 1);
                *(s32*)((s32)event + 0x78) += 1;
            }
            break;
        case 3:
            *(f32*)((s32)npc + 0x154) += (float_1_8041fd88 - *(f32*)((s32)npc + 0x154)) / float_10_8041fdc0;
            *(s32*)((s32)event + 0x7C) += 1;
            if (*(s32*)((s32)event + 0x7C) > 0x1E) {
                return 2;
            }
            break;
    }
    return 0;
}

s32 evt_npc_setup(EventEntry* event, s32 isFirstCall) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern void* fbatGetPointer(void);
    extern void* npcNameToPtr_NoAssert(void* name);
    extern void* npcNameToPtr(void* name);
    extern void evtDeleteID(s32 id);
    extern void* evtEntry(void* code, s32 order, u32 flags);
    extern void npcSetBattleInfo(void* npc, s32 battleInfoId);
    extern char* strcpy(char* dst, char* src);
    extern void animPoseSetAnim(s32 poseId, char* anim, s32 force);
    extern void fbatChangeMode(s16 mode);
    extern s32 evtCheckID(s32 id);
    void* setup;
    void* iter;
    void* tribe;
    void* npc;
    void* evt;

    setup = (void*)evtGetValue(event, event->args[0]);
    fbatGetPointer();

    if (isFirstCall != 0) {
        iter = setup;
        while (*(void**)iter != 0) {
            npc = npcNameToPtr_NoAssert(*(void**)iter);
            if (npc != 0) {
                *(u32*)npc |= *(u32*)((s32)iter + 4);
                *(u32*)((s32)npc + 4) |= *(u32*)((s32)iter + 8);
                tribe = *(void**)((s32)npc + 0x28);
                if (*(void**)((s32)iter + 0xC) != 0) {
                    if (*(s32*)((s32)npc + 0x118) != 0) {
                        evtDeleteID(*(s32*)((s32)npc + 0x118));
                    }
                    evt = evtEntry(*(void**)((s32)iter + 0xC), 0, 0);
                    *(u8*)((s32)evt + 0x10) = 0;
                    *(void**)((s32)evt + 0x170) = npc;
                    *(s32*)((s32)npc + 0x118) = *(s32*)((s32)evt + 0x15C);
                } else {
                    *(s32*)((s32)npc + 0x118) = 0;
                }

                *(void**)((s32)npc + 0x128) = *(void**)((s32)iter + 0x14);
                *(void**)((s32)npc + 0x12C) = *(void**)((s32)iter + 0x18);
                *(void**)((s32)npc + 0x124) = *(void**)((s32)iter + 0x10);
                *(void**)((s32)npc + 0x130) = *(void**)((s32)iter + 0x1C);
                *(void**)((s32)npc + 0x134) = *(void**)((s32)iter + 0x20);
                *(void**)((s32)npc + 0x138) = *(void**)((s32)iter + 0x24);
                *(void**)((s32)npc + 0x13C) = *(void**)((s32)iter + 0x28);
                *(s32*)((s32)npc + 0x1F8) = *(s32*)((s32)iter + 0x2C);

                *(s32*)((s32)npc + 0x1FC) = *(s32*)((s32)iter + 0x30);
                *(s32*)((s32)npc + 0x200) = *(s32*)((s32)iter + 0x34);
                *(s32*)((s32)npc + 0x204) = *(s32*)((s32)iter + 0x38);
                *(s32*)((s32)npc + 0x208) = *(s32*)((s32)iter + 0x3C);
                *(s32*)((s32)npc + 0x20C) = *(s32*)((s32)iter + 0x40);
                *(s32*)((s32)npc + 0x210) = *(s32*)((s32)iter + 0x44);
                *(s32*)((s32)npc + 0x214) = *(s32*)((s32)iter + 0x3C);
                *(s32*)((s32)npc + 0x218) = *(s32*)((s32)iter + 0x40);
                *(s32*)((s32)npc + 0x21C) = *(s32*)((s32)iter + 0x44);
                *(f32*)((s32)npc + 0x220) = *(f32*)((s32)iter + 0x48);
                *(f32*)((s32)npc + 0x224) = *(f32*)((s32)iter + 0x4C);
                *(f32*)((s32)npc + 0x228) = *(f32*)((s32)iter + 0x50);
                *(f32*)((s32)npc + 0x22C) = *(f32*)((s32)iter + 0x54);
                npcSetBattleInfo(npc, *(s32*)((s32)iter + 0x58));

                if (*(void**)((s32)tribe + 0x10) != 0) {
                    strcpy((char*)((s32)npc + 0x4C), *(char**)((s32)tribe + 0x10));
                }
                if (*(void**)((s32)tribe + 0x14) != 0) {
                    strcpy((char*)((s32)npc + 0x6C), *(char**)((s32)tribe + 0x14));
                }
                if (*(void**)((s32)tribe + 8) != 0) {
                    strcpy((char*)((s32)npc + 0x2C), *(char**)((s32)tribe + 8));
                    animPoseSetAnim(*(s32*)((s32)npc + 0x104), (char*)((s32)npc + 0x2C), 1);
                }
            }
            iter = (void*)((s32)iter + 0x5C);
        }
        fbatChangeMode(1);
        return 0;
    }

    iter = setup;
    while (*(void**)iter != 0) {
        npc = npcNameToPtr(*(void**)iter);
        if (*(s32*)((s32)npc + 0x118) != 0 && evtCheckID(*(s32*)((s32)npc + 0x118)) != 0) {
            return 0;
        }
        iter = (void*)((s32)iter + 0x5C);
    }

    iter = setup;
    while (*(void**)iter != 0) {
        npc = npcNameToPtr(*(void**)iter);
        if (*(void**)((s32)npc + 0x124) != 0) {
            if (*(s32*)((s32)npc + 0x11C) != 0) {
                evtDeleteID(*(s32*)((s32)npc + 0x11C));
            }
            evt = evtEntry(*(void**)((s32)npc + 0x124), 0, 0x20);
            *(u8*)((s32)evt + 0x10) = 1;
            *(void**)((s32)evt + 0x170) = npc;
            *(s32*)((s32)npc + 0x11C) = *(s32*)((s32)evt + 0x15C);
        }
        iter = (void*)((s32)iter + 0x5C);
    }
    return 2;
}

s32 evt_npc_majo_disp_off(EventEntry* event, s32 isFirstCall) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    extern s32 psndSFXOn_3D(s32 id, void* pos);
    extern s32 animGroupBaseAsync(void* name, s32 mode, s32 flags);
    extern s32 animPoseGetVivianType(s32 poseId);
    extern void animPoseSetEffect(s32 poseId, void* name, s32 flag);
    extern void animPoseSetEffectAnim(s32 poseId, void* name, s32 flag);
    extern void animPosePaperPeraOn(s32 poseId);
    extern void* animPoseGetAnimPosePtr(s32 poseId);
    extern void animPoseSetLocalTime(s32 poseId, f32 time);
    extern f32 animPoseGetLoopTimes(s32 poseId);
    extern void* gp;
    extern char str_p_bibi_802c14d8[];
    extern char str_PM_B_1_802c14e0[];
    extern f32 float_0_8041fd84;
    extern f32 float_1_8041fd88;
    extern f32 float_3_8041fdbc;
    extern f32 float_10_8041fdc0;
    extern f32 float_8_8041fdc4;
    s32 name;
    s32 battle;
    void* npc;
    s32 state;
    s32 vivianType;
    s32 effectPose;
    s32 first;
    void* pose;
    EventEntry* evt;

    first = isFirstCall;
    evt = event;
    name = evtGetValue(evt, evt->args[0]);
    battle = (*(s32*)((s32)gp + 0x14) != 0);
    npc = evtNpcNameToPtr(evt, name);
    if (first != 0) {
        *(s32*)((s32)evt + 0x78) = 0;
        *(s32*)((s32)evt + 0x7C) = 0;
        psndSFXOn_3D(0x275, (void*)((s32)npc + 0x8C));
    }

    if (animGroupBaseAsync(str_p_bibi_802c14d8, battle, 0) == 0) {
        return 0;
    }

    state = *(s32*)((s32)evt + 0x78);
    switch (state) {
        case 0:
            *(f32*)((s32)npc + 0x154) += (float_3_8041fdbc - *(f32*)((s32)npc + 0x154)) / float_10_8041fdc0;
            *(s32*)((s32)evt + 0x7C) += 1;
            if (*(s32*)((s32)evt + 0x7C) > 0x1E) {
                *(s32*)((s32)evt + 0x78) += 1;
                *(s32*)((s32)evt + 0x7C) = 0;
                vivianType = animPoseGetVivianType(*(s32*)((s32)npc + 0x104));
                switch (vivianType) {
                    case 0:
                        break;
                    case 1:
                        psndSFXOn_3D(0x936, (void*)((s32)npc + 0x8C));
                        break;
                    case 2:
                        psndSFXOn_3D(0x27A, (void*)((s32)npc + 0x8C));
                        break;
                    case 3:
                        psndSFXOn_3D(0x277, (void*)((s32)npc + 0x8C));
                        break;
                }
            }
            break;
        case 1:
            animPoseSetEffect(*(s32*)((s32)npc + 0x104), str_p_bibi_802c14d8, 1);
            animPoseSetEffectAnim(*(s32*)((s32)npc + 0x104), str_PM_B_1_802c14e0, 1);
            animPosePaperPeraOn(*(s32*)((s32)npc + 0x104));
            pose = animPoseGetAnimPosePtr(*(s32*)((s32)npc + 0x104));
            effectPose = *(s32*)((s32)pose + 0x90);
            animPoseSetLocalTime(effectPose, float_8_8041fdc4);
            *(s32*)((s32)evt + 0x78) += 1;
            break;
        case 2:
            pose = animPoseGetAnimPosePtr(*(s32*)((s32)npc + 0x104));
            if (animPoseGetLoopTimes(*(s32*)((s32)pose + 0x90)) >= float_1_8041fd88) {
                *(s32*)((s32)evt + 0x78) += 1;
            }
            break;
        case 3:
            *(f32*)((s32)npc + 0x154) += -*(f32*)((s32)npc + 0x154) / float_10_8041fdc0;
            *(s32*)((s32)evt + 0x7C) += 1;
            if (*(s32*)((s32)evt + 0x7C) > 0x1E) {
                *(f32*)((s32)npc + 0x154) = float_0_8041fd84;
                return 2;
            }
            break;
    }
    return 0;
}

f32 _intplGetFloat(f32 value, s32 kind) {
    extern f64 cos(f64 x);
    extern f64 sin(f64 x);
    extern f32 float_0_8041fd84;
    extern volatile f32 float_1_8041fd88;
    extern f32 float_0p5_8041fd90;
    extern f32 float_3p1416_8041fda8;
    extern f32 float_4_8041fde8;
    extern f32 float_15_8041fdec;
    extern f32 float_40_8041fdf0;
    extern f32 float_1p5708_8041fdf4;
    f32 t;
    f32 inv;
    f32 temp;
    f32 one;

    t = value;
    if (t > float_1_8041fd88) {
        t = float_1_8041fd88;
    }

    switch (kind) {
        case 0:
            return t;
        case 1:
            return t * t;
        case 2:
            return t * (t * t);
        case 3:
            return t * (t * (t * t));
        case 7:
            temp = (f32)cos(float_4_8041fde8 * (t / float_3p1416_8041fda8));
            one = float_1_8041fd88;
            inv = one - t;
            temp = inv * temp;
            temp = inv * temp;
            return one - temp;
        case 8:
            temp = (f32)cos((float_4_8041fde8 * ((t * t) / float_3p1416_8041fda8)) / float_15_8041fdec);
            one = float_1_8041fd88;
            inv = one - t;
            temp = inv * temp;
            temp = inv * temp;
            return one - temp;
        case 4:
            one = float_1_8041fd88;
            inv = one - t;
            return one - inv * inv;
        case 5:
            one = float_1_8041fd88;
            inv = one - t;
            temp = inv * inv;
            return one - inv * temp;
        case 6:
            one = float_1_8041fd88;
            inv = one - t;
            temp = inv * inv;
            temp = inv * temp;
            return one - inv * temp;
        case 10:
            temp = (f32)cos((float_4_8041fde8 * ((t * t) / float_3p1416_8041fda8)) / float_40_8041fdf0);
            one = float_1_8041fd88;
            inv = one - t;
            temp = inv * temp;
            temp = inv * temp;
            if (temp < float_0_8041fd84) {
                temp = -temp;
            }
            return float_1_8041fd88 - temp;
        case 11:
            temp = (f32)cos(float_3p1416_8041fda8 * t);
            return (float_1_8041fd88 - temp) * float_0p5_8041fd90;
        case 12:
            return (f32)sin(float_1p5708_8041fdf4 * t);
        case 13:
            temp = (f32)cos(float_1p5708_8041fdf4 * t);
            return float_1_8041fd88 - temp;
        default:
            return float_0_8041fd84;
    }
}

void* evtNpcNameToPtr_NoAssert(void* event, s32 name) {
    extern s32 strcmp(const char*, const char*);
    extern void* npcNameToPtr_NoAssert(void*);
    extern char vec3_802c1470[];
    extern char str_me_8041fd80[];
    extern char str_party_8041fdcc[];
    extern char str_mario_8041fdd4[];

    if (strcmp((char*)name, str_me_8041fd80) == 0) {
        return *(void**)((s32)event + 0x170);
    }
    if (strcmp((char*)name, str_party_8041fdcc) == 0) {
        return (void*)-1;
    }
    if (strcmp((char*)name, vec3_802c1470 + 0x80) == 0) {
        return (void*)-1;
    }
    if (strcmp((char*)name, vec3_802c1470 + 0x8C) == 0) {
        return (void*)-2;
    }
    if (strcmp((char*)name, str_mario_8041fdd4) == 0) {
        return (void*)-3;
    }
    if (strcmp((char*)name, vec3_802c1470 + 0x98) == 0) {
        return *(void**)((s32)*(void**)((s32)event + 0x170) + 0x32C);
    }
    if (strcmp((char*)name, vec3_802c1470 + 0xA0) == 0) {
        return *(void**)((s32)*(void**)((s32)event + 0x170) + 0x330);
    }
    if (strcmp((char*)name, vec3_802c1470 + 0xA8) == 0) {
        return *(void**)((s32)*(void**)((s32)event + 0x170) + 0x334);
    }
    if (strcmp((char*)name, vec3_802c1470 + 0xB0) == 0) {
        return *(void**)((s32)*(void**)((s32)event + 0x170) + 0x338);
    }
    if (strcmp((char*)name, vec3_802c1470 + 0xB8) == 0) {
        return *(void**)((s32)*(void**)((s32)event + 0x170) + 0x33C);
    }
    return npcNameToPtr_NoAssert((void*)name);
}

void* evtNpcNameToPtr(void* event, s32 name) {
    extern s32 strcmp(const char*, const char*);
    extern void* npcNameToPtr(void*);
    extern char vec3_802c1470[];
    extern char str_me_8041fd80[];
    extern char str_party_8041fdcc[];
    extern char str_mario_8041fdd4[];

    if (strcmp((char*)name, str_me_8041fd80) == 0) {
        return *(void**)((s32)event + 0x170);
    }
    if (strcmp((char*)name, str_party_8041fdcc) == 0) {
        return (void*)-1;
    }
    if (strcmp((char*)name, vec3_802c1470 + 0x80) == 0) {
        return (void*)-1;
    }
    if (strcmp((char*)name, vec3_802c1470 + 0x8C) == 0) {
        return (void*)-2;
    }
    if (strcmp((char*)name, str_mario_8041fdd4) == 0) {
        return (void*)-3;
    }
    if (strcmp((char*)name, vec3_802c1470 + 0x98) == 0) {
        return *(void**)((s32)*(void**)((s32)event + 0x170) + 0x32C);
    }
    if (strcmp((char*)name, vec3_802c1470 + 0xA0) == 0) {
        return *(void**)((s32)*(void**)((s32)event + 0x170) + 0x330);
    }
    if (strcmp((char*)name, vec3_802c1470 + 0xA8) == 0) {
        return *(void**)((s32)*(void**)((s32)event + 0x170) + 0x334);
    }
    if (strcmp((char*)name, vec3_802c1470 + 0xB0) == 0) {
        return *(void**)((s32)*(void**)((s32)event + 0x170) + 0x338);
    }
    if (strcmp((char*)name, vec3_802c1470 + 0xB8) == 0) {
        return *(void**)((s32)*(void**)((s32)event + 0x170) + 0x33C);
    }
    return npcNameToPtr((void*)name);
}

u8 evt_npc_set_rotate_offset(s32 pEvt) {
    return 0;
}


s32 evt_npc_set_rotate(void* pEvt) {
    extern s32 evtGetValue(void*, s32);
    extern void* evtNpcNameToPtr(void*, s32);
    extern char vec3_802c147c[];
    s32* args;
    s32 name;
    f32 x;
    f32 y;
    f32 z;
    f32 rot[3];
    void* npc;

    args = *(s32**)((s32)pEvt + 0x18);
    name = evtGetValue(pEvt, args[0]);
    x = evtGetValue(pEvt, args[1]);
    y = evtGetValue(pEvt, args[2]);
    z = evtGetValue(pEvt, args[3]);
    npc = evtNpcNameToPtr(pEvt, name);
    *(s32*)&rot[0] = *(s32*)(vec3_802c147c + 0);
    *(s32*)&rot[1] = *(s32*)(vec3_802c147c + 4);
    *(s32*)&rot[2] = *(s32*)(vec3_802c147c + 8);
    rot[0] = x;
    rot[1] = y;
    rot[2] = z;
    *(s32*)((s32)npc + 0xEC) = *(s32*)&rot[0];
    *(s32*)((s32)npc + 0xF0) = *(s32*)&rot[1];
    *(s32*)((s32)npc + 0xF4) = *(s32*)&rot[2];
    return 2;
}

u8 evt_fbat_trans_floor_position(s32 pEvt) {
    return 0;
}


s32 evt_npc_add_rotate(void* pEvt) {
    extern s32 evtGetValue(void*, s32);
    extern void* evtNpcNameToPtr(void*, s32);
    s32* args;
    s32 name;
    f32 x;
    f32 y;
    f32 z;
    void* npc;

    args = *(s32**)((s32)pEvt + 0x18);
    name = evtGetValue(pEvt, args[0]);
    x = evtGetValue(pEvt, args[1]);
    y = evtGetValue(pEvt, args[2]);
    z = evtGetValue(pEvt, args[3]);
    npc = evtNpcNameToPtr(pEvt, name);
    *(f32*)((s32)npc + 0xEC) += x;
    *(f32*)((s32)npc + 0xF0) += y;
    *(f32*)((s32)npc + 0xF4) += z;
    return 2;
}

s32 evt_npc_slave_entry(void* pEvt) {
    extern s32 evtGetValue(void*, s32);
    extern void* evtNpcNameToPtr(void*, s32);
    extern void* npcNameToPtr(void*);
    extern void* memset(void*, s32, u32);
    extern s32 sprintf(char*, const char*, ...);
    extern char* strcpy(char*, const char*);
    extern char* strcat(char*, const char*);
    extern s32 npcEntry(char*, s32);
    extern void npcSetSlave(void*, void*, s32);
    extern void evtSetValue(void*, s32, s32);
    extern char str_sPCTd_8041fe08[];
    s32* args;
    void* parent;
    s32 index;
    s32 model;
    s32 dst;
    s32 interrupt;
    void* slave;
    char suffix[4];
    char name[0xBC];
    s32 i;

    args = *(s32**)((s32)pEvt + 0x18);
    parent = evtNpcNameToPtr(pEvt, evtGetValue(pEvt, args[0]));
    index = evtGetValue(pEvt, args[1]);
    model = evtGetValue(pEvt, args[2]);
    dst = args[3];
    interrupt = evtGetValue(pEvt, args[4]);
    memset(name, 0, 0xB8);
    sprintf(suffix, str_sPCTd_8041fe08, index);
    strcpy(name, (char*)((s32)parent + 8));
    strcat(name, suffix);
    npcEntry(name, model);
    slave = npcNameToPtr(name);
    *(u32*)slave |= 0x120018;
    *(s32*)((s32)slave + 0x12C) = interrupt;
    for (i = 0; i < 0x32; i++) {
        ((s32*)((s32)slave + 0x230))[i] = ((s32*)((s32)parent + 0x230))[i];
    }
    npcSetSlave(parent, slave, index);
    evtSetValue(pEvt, dst, (s32)slave + 8);
    return 2;
}

s32 evt_npc_add_dirdist(void* pEvt) {
    extern f32 evtGetFloat(void*, s32);
    extern void evtSetFloat(void*, s32, f32);
    extern f64 sin(f64);
    extern f64 cos(f64);
    extern f32 float_6p2832_8041fda0;
    extern f32 float_360_8041fda4;
    s32* args;
    s32 xArg;
    s32 zArg;
    f32 dir;
    f32 dist;
    f32 x;
    f32 z;
    f32 radians;
    f32 s;
    f32 c;

    args = *(s32**)((s32)pEvt + 0x18);
    xArg = args[0];
    zArg = args[1];
    dir = evtGetFloat(pEvt, args[2]);
    dist = evtGetFloat(pEvt, args[3]);
    x = evtGetFloat(pEvt, xArg);
    z = evtGetFloat(pEvt, zArg);
    radians = (float_6p2832_8041fda0 * dir) / float_360_8041fda4;
    s = (f32)sin(radians);
    c = (f32)cos(radians);
    evtSetFloat(pEvt, xArg, x + dist * s);
    evtSetFloat(pEvt, zArg, z - dist * c);
    return 2;
}

s32 evt_npc_sound_data_set(void* pEvt) {
    extern s32 evtGetValue(void*, s32);
    extern void* evtNpcNameToPtr(void*, s32);
    extern f32 float_0_8041fd84;
    s32* args;
    void* npc;
    s32 arg1;
    s32 arg2;
    s32 arg3;
    s32 arg4;
    s32 arg5;
    s32 value1;
    s32 value2;
    s32 value3;
    u16 value4;
    s32 value5;

    args = *(s32**)((s32)pEvt + 0x18);
    npc = evtNpcNameToPtr(pEvt, evtGetValue(pEvt, args[0]));
    arg1 = args[1];
    value1 = evtGetValue(pEvt, arg1);
    arg2 = args[2];
    value2 = evtGetValue(pEvt, arg2);
    arg3 = args[3];
    value4 = evtGetValue(pEvt, arg3);
    arg4 = args[4];
    value3 = evtGetValue(pEvt, arg4);
    arg5 = args[5];
    value5 = evtGetValue(pEvt, arg5);

    if ((u32)arg1 != 0xF1194D80) {
        *(s32*)((s32)npc + 0x1E0) = value1;
    }
    if ((u32)arg2 != 0xF1194D80) {
        *(s32*)((s32)npc + 0x1E4) = value2;
    }
    if ((u32)arg4 != 0xF1194D80) {
        *(s32*)((s32)npc + 0x1EC) = value3;
    }
    if ((u32)arg5 != 0xF1194D80) {
        *(s32*)((s32)npc + 0x1F0) = value5;
    }
    if ((u32)arg3 != 0xF1194D80) {
        *(u16*)((s32)npc + 0x1E8) = value4;
    }
    *(u16*)((s32)npc + 0x1D8) = 1;
    *(f32*)((s32)npc + 0x1DC) = float_0_8041fd84;
    return 2;
}

u8 _kamek_houki_kemuri(int param_1, int param_2) {
    return 0;
}


s32 evt_npc_set_tribe(void* pEvt) {
    extern s32 evtGetValue(void*, s32);
    extern void* evtNpcNameToPtr(void*, s32);
    extern void* npcGetTribe(s32);
    extern char* strcpy(char*, const char*);
    extern void animPoseSetAnim(s32, char*, s32);
    extern f32 float_0_8041fd84;
    s32* args;
    s32 name;
    s32 tribeName;
    void* npc;
    void* tribe;

    args = *(s32**)((s32)pEvt + 0x18);
    name = evtGetValue(pEvt, args[0]);
    tribeName = evtGetValue(pEvt, args[1]);
    npc = evtNpcNameToPtr(pEvt, name);
    tribe = npcGetTribe(tribeName);
    *(void**)((s32)npc + 0x28) = tribe;
    *(f32*)((s32)npc + 0x14C) = *(f32*)((s32)tribe + 0x28);
    *(f32*)((s32)npc + 0x150) = *(f32*)((s32)tribe + 0x2C);
    if (*(void**)((s32)tribe + 0x10) != 0) {
        strcpy((char*)((s32)npc + 0x4C), *(char**)((s32)tribe + 0x10));
    }
    if (*(void**)((s32)tribe + 0x14) != 0) {
        strcpy((char*)((s32)npc + 0x6C), *(char**)((s32)tribe + 0x14));
    }
    if (*(void**)((s32)tribe + 8) != 0) {
        strcpy((char*)((s32)npc + 0x2C), *(char**)((s32)tribe + 8));
        animPoseSetAnim(*(s32*)((s32)npc + 0x104), (char*)((s32)npc + 0x2C), 1);
    }
    *(s32*)((s32)npc + 0x1E0) = *(s32*)((s32)tribe + 0x40);
    *(s32*)((s32)npc + 0x1E4) = *(s32*)((s32)tribe + 0x44);
    *(s32*)((s32)npc + 0x1EC) = *(s32*)((s32)tribe + 0x4C);
    *(s32*)((s32)npc + 0x1F0) = *(s32*)((s32)tribe + 0x50);
    *(u16*)((s32)npc + 0x1E8) = *(u16*)((s32)tribe + 0x48);
    *(u16*)((s32)npc + 0x1D8) = 1;
    *(f32*)((s32)npc + 0x1DC) = float_0_8041fd84;
    return 1;
}

u8 evt_npc_kamek_kemuri2(void) {
    return 0;
}


s32 evt_npc_facedirection_add(void* pEvt) {
    extern s32 evtGetValue(void*, s32);
    extern f32 evtGetFloat(void*, s32);
    extern void* evtNpcNameToPtr(void*, s32);
    extern void* marioGetPtr(void);
    extern s32 strcmp(const char*, const char*);
    extern f32 npcTransRytoFaceDir(void*);
    extern void evtSetValue(void*, s32, s32);
    extern char str_mario_8041fdd4[];
    extern f32 float_180_8041fdb0;
    extern f32 float_90_8041fddc;
    extern f32 float_1_8041fd88;
    extern f32 float_neg1_8041fd8c;
    s32* args;
    s32 name;
    f32 base;
    f32 add;
    f32 dir;
    s32 value;

    args = *(s32**)((s32)pEvt + 0x18);
    name = evtGetValue(pEvt, args[0]);
    base = evtGetFloat(pEvt, args[1]);
    add = evtGetFloat(pEvt, args[2]);
    if (strcmp((char*)name, str_mario_8041fdd4) == 0) {
        if (*(f32*)((s32)marioGetPtr() + 0x1AC) == float_180_8041fdb0) {
            dir = float_1_8041fd88;
        } else {
            dir = float_neg1_8041fd8c;
        }
    } else if (npcTransRytoFaceDir(evtNpcNameToPtr(pEvt, name)) == float_90_8041fddc) {
        dir = float_1_8041fd88;
    } else {
        dir = float_neg1_8041fd8c;
    }
    value = (s32)(base + add * dir);
    evtSetValue(pEvt, args[1], value);
    return 2;
}

s32 evt_npc_set_ry_lr(void* pEvt) {
    return 0;
}


s32 evt_npc_get_loiter_dir(void* pEvt) {
    extern f32 evtGetFloat(void*, s32);
    extern void evtSetFloat(void*, s32, f32);
    extern s32 rand(void);
    extern f32 reviseAngle(f32);
    extern f32 float_0_8041fd84;
    s32* args;
    s32 dst;
    f32 min;
    f32 max;
    f32 base;
    f32 range;

    args = *(s32**)((s32)pEvt + 0x18);
    dst = args[0];
    min = evtGetFloat(pEvt, args[1]);
    max = evtGetFloat(pEvt, args[2]);
    base = evtGetFloat(pEvt, dst);
    range = max - min;
    if (range <= float_0_8041fd84) {
        evtSetFloat(pEvt, dst, reviseAngle(base + min));
    } else {
        evtSetFloat(pEvt, dst, reviseAngle(base + min + (f32)rand() / range));
    }
    return 2;
}

s32 evt_npc_set_home_position(void* pEvt) {
    extern s32 evtGetValue(void*, s32);
    extern f32 evtGetFloat(void*, s32);
    extern void* evtNpcNameToPtr(void*, s32);
    s32* args;
    s32 name;
    s32 x;
    s32 y;
    s32 z;
    void* npc;

    args = *(s32**)((s32)pEvt + 0x18);
    name = evtGetValue(pEvt, args[0]);
    x = (s32)evtGetFloat(pEvt, args[1]);
    y = (s32)evtGetFloat(pEvt, args[2]);
    z = (s32)evtGetFloat(pEvt, args[3]);
    npc = evtNpcNameToPtr(pEvt, name);
    *(f32*)((s32)npc + 0x1FC) = x;
    *(f32*)((s32)npc + 0x200) = y;
    *(f32*)((s32)npc + 0x204) = z;
    return 2;
}

s32 evt_npc_set_position(void* pEvt) {
    extern s32 evtGetValue(void*, s32);
    extern f32 evtGetFloat(void*, s32);
    extern void* evtNpcNameToPtr(void*, s32);
    extern char vec3_802c1470[];
    s32* args;
    s32 name;
    f32 x;
    f32 y;
    f32 z;
    f32 pos[3];
    void* npc;

    args = *(s32**)((s32)pEvt + 0x18);
    name = evtGetValue(pEvt, args[0]);
    x = evtGetFloat(pEvt, args[1]);
    y = evtGetFloat(pEvt, args[2]);
    z = evtGetFloat(pEvt, args[3]);
    npc = evtNpcNameToPtr(pEvt, name);
    *(s32*)&pos[0] = *(s32*)(vec3_802c1470 + 0);
    *(s32*)&pos[1] = *(s32*)(vec3_802c1470 + 4);
    *(s32*)&pos[2] = *(s32*)(vec3_802c1470 + 8);
    pos[0] = x;
    pos[1] = y;
    pos[2] = z;
    *(s32*)((s32)npc + 0x98) = *(s32*)&pos[0];
    *(s32*)((s32)npc + 0x9C) = *(s32*)&pos[1];
    *(s32*)((s32)npc + 0xA0) = *(s32*)&pos[2];
    *(s32*)((s32)npc + 0x8C) = *(s32*)((s32)npc + 0x98);
    *(s32*)((s32)npc + 0x90) = *(s32*)((s32)npc + 0x9C);
    *(s32*)((s32)npc + 0x94) = *(s32*)((s32)npc + 0xA0);
    return 2;
}

int evt_npc_wait_msec(void* pEvt, int param_2) {
    return 0;
}


f32 _get_target_dir(void* pEvt, void* npc, s32 target) {
    extern s32 strcmp(const char*, const char*);
    extern void* marioGetPtr(void);
    extern void* evtNpcNameToPtr(void*, s32);
    extern void PSVECSubtract(void*, void*, void*);
    extern f32 angleABf(f32, f32, f32, f32);
    extern char str_mario_8041fdd4[];
    extern f32 float_0_8041fd84;
    f32 targetPos[3];
    f32 npcPos[3];
    f32 diff[3];
    void* ptr;

    if (strcmp((char*)target, str_mario_8041fdd4) == 0) {
        ptr = marioGetPtr();
    } else {
        ptr = evtNpcNameToPtr(pEvt, target);
    }
    targetPos[0] = *(f32*)((s32)ptr + 0x8C);
    targetPos[1] = *(f32*)((s32)ptr + 0x90);
    targetPos[2] = *(f32*)((s32)ptr + 0x94);
    npcPos[0] = *(f32*)((s32)npc + 0x8C);
    npcPos[1] = *(f32*)((s32)npc + 0x90);
    npcPos[2] = *(f32*)((s32)npc + 0x94);
    PSVECSubtract(targetPos, npcPos, diff);
    return angleABf(float_0_8041fd84, float_0_8041fd84, diff[0], diff[2]);
}

s32 evt_npc_get_drop_flower(void* pEvt) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void* evtNpcNameToPtr(void* event, s32 name);
    extern void evtSetValue(void* event, s32 dst, s32 value);
    extern s32 pouchEquipCheckBadge(s32 badge);
    extern s32 irand(s32 max);
    s32* args = *(s32**)((s32)pEvt + 0x18);
    s32 name = evtGetValue(pEvt, args[0]);
    void* npc = evtNpcNameToPtr(pEvt, name);
    s32 dst = args[1];
    s32 badge;
    s32 value;

    if (*(u32*)((s32)npc + 0x234) & 4) {
        evtSetValue(pEvt, dst, 0);
    } else {
        badge = pouchEquipCheckBadge(0x138);
        value = *(s32*)((s32)npc + 0x244);
        if (badge > 0) {
            if (badge == 1) {
                value += irand(3) + 1;
            } else {
                value += irand(badge + 3) + 1;
            }
        }
        evtSetValue(pEvt, dst, value);
    }
    return 2;
}

s32 evt_npc_get_drop_heart(void* pEvt) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void* evtNpcNameToPtr(void* event, s32 name);
    extern void evtSetValue(void* event, s32 dst, s32 value);
    extern s32 pouchEquipCheckBadge(s32 badge);
    extern s32 irand(s32 max);
    s32* args = *(s32**)((s32)pEvt + 0x18);
    s32 name = evtGetValue(pEvt, args[0]);
    void* npc = evtNpcNameToPtr(pEvt, name);
    s32 dst = args[1];
    s32 badge;
    s32 value;

    if (*(u32*)((s32)npc + 0x234) & 2) {
        evtSetValue(pEvt, dst, 0);
    } else {
        badge = pouchEquipCheckBadge(0x137);
        value = *(s32*)((s32)npc + 0x240);
        if (badge > 0) {
            if (badge == 1) {
                value += irand(3) + 1;
            } else {
                value += irand(badge + 3) + 1;
            }
        }
        evtSetValue(pEvt, dst, value);
    }
    return 2;
}

s32 evt_npc_set_autotalkpose(void* pEvt) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void* evtNpcNameToPtr(void* event, s32 name);
    extern void npcSetMarioAutoTalkPose(const char* stay, const char* talk);
    extern char* strcpy(char* dst, const char* src);
    s32* args = *(s32**)((s32)pEvt + 0x18);
    s32 name = evtGetValue(pEvt, args[0]);
    u32 stay = evtGetValue(pEvt, args[1]);
    u32 talk = evtGetValue(pEvt, args[2]);
    void* npc = evtNpcNameToPtr(pEvt, name);

    if ((u32)((s32)npc + 0x10000) == 0xFFFD) {
        npcSetMarioAutoTalkPose((const char*)stay, (const char*)talk);
        return 1;
    }
    if (stay != 0) {
        strcpy((char*)((s32)npc + 0x4C), (char*)stay);
    }
    if (talk != 0) {
        strcpy((char*)((s32)npc + 0x6C), (char*)talk);
    }
    return 1;
}

s32 evt_npc_battle_start(void* pEvt, s32 isFirstCall) {
    extern s32 evtGetValue(void*, s32);
    extern void* fbatGetPointer(void);
    extern void* evtNpcNameToPtr(void*, s32);
    extern void marioFBattlePrepare(void);
    extern void marioCtrlOff(void);
    extern s32 marioGetPartyId(void);
    extern void* partyGetPtr(s32);
    extern void partyCtrlOff(void*);
    extern void BattleInformationSetResult(void*, s32);
    extern void psndSetFlag(s32);
    extern void seqSetSeq(s32, s32, s32);
    s32* args;
    s32 name;
    void* fbat;
    void* info;
    void* npc;
    void* party;

    args = *(s32**)((s32)pEvt + 0x18);
    name = evtGetValue(pEvt, args[0]);
    evtGetValue(pEvt, args[1]);
    fbat = fbatGetPointer();
    npc = evtNpcNameToPtr(pEvt, name);
    info = (void*)((s32)fbat + 0x20);
    if (isFirstCall != 0) {
        *(void**)((s32)fbat + 8) = npc;
        marioFBattlePrepare();
        marioCtrlOff();
        party = partyGetPtr(marioGetPartyId());
        if (party != 0) {
            partyCtrlOff(party);
        }
        BattleInformationSetResult(info, 0);
        psndSetFlag(0x10);
        seqSetSeq(4, 0, 0);
        return 0;
    }
    return 2;
}

s32 evt_npc_set_color(void* pEvt) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void* evtNpcNameToPtr(void* event, s32 name);
    extern void npcSetColor(char* name, void* color);
    extern u32 unk_80429560;
    s32* args = *(s32**)((s32)pEvt + 0x18);
    s32 name = evtGetValue(pEvt, args[0]);
    u8 r = evtGetValue(pEvt, args[1]);
    u8 g = evtGetValue(pEvt, args[2]);
    u8 b = evtGetValue(pEvt, args[3]);
    u8 a = evtGetValue(pEvt, args[4]);
    void* npc = evtNpcNameToPtr(pEvt, name);
    u32 copy;
    u32 color = unk_80429560;

    npc = (void*)((s32)npc + 8);
    ((u8*)&color)[0] = r;
    ((u8*)&color)[1] = g;
    ((u8*)&color)[2] = b;
    ((u8*)&color)[3] = a;
    copy = color;
    npcSetColor(npc, &copy);
    return 2;
}

s32 evt_npc_change_interrupt(void* pEvt) {
    extern s32 evtGetValue(void*, s32);
    extern void* evtNpcNameToPtr(void*, s32);
    s32* args;
    s32 name;
    s32 type;
    s32 value;
    void* npc;

    args = *(s32**)((s32)pEvt + 0x18);
    name = evtGetValue(pEvt, args[0]);
    type = evtGetValue(pEvt, args[1]);
    value = args[2];
    npc = evtNpcNameToPtr(pEvt, name);
    switch (type) {
        case 0:
            *(s32*)((s32)npc + 0x124) = value;
            break;
        case 1:
            *(s32*)((s32)npc + 0x130) = value;
            break;
        case 2:
            *(s32*)((s32)npc + 0x134) = value;
            break;
        case 3:
            *(s32*)((s32)npc + 0x138) = value;
            break;
        case 4:
            *(s32*)((s32)npc + 0x13C) = value;
            break;
        case 7:
            *(s32*)((s32)npc + 0x128) = value;
            break;
        case 8:
            *(s32*)((s32)npc + 0x12C) = value;
            break;
    }
    return 2;
}

s32 evt_npc_status_onoff(void* pEvt) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void* evtNpcNameToPtr(void* event, s32 name);
    s32* args = *(s32**)((s32)pEvt + 0x18);
    s32 onoff = evtGetValue(pEvt, args[0]);
    s32 name = evtGetValue(pEvt, args[1]);
    u32 flag = evtGetValue(pEvt, args[2]);
    void* npc = evtNpcNameToPtr(pEvt, name);

    switch (onoff) {
        case 0:
            *(u32*)((s32)npc + 0x1D4) &= ~flag;
            break;
        case 1:
            *(u32*)((s32)npc + 0x1D4) |= flag;
            break;
    }
    return 2;
}

s32 evt_npc_reaction_flag_onoff(void* pEvt) {
    extern s32 evtGetValue(void*, s32);
    extern void* evtNpcNameToPtr(void*, s32);
    s32* args;
    s32 onoff;
    s32 name;
    u32 flag;
    void* npc;

    args = *(s32**)((s32)pEvt + 0x18);
    onoff = evtGetValue(pEvt, args[0]);
    name = evtGetValue(pEvt, args[1]);
    flag = evtGetValue(pEvt, args[2]);
    npc = evtNpcNameToPtr(pEvt, name);
    switch (onoff) {
        case 0:
            *(u32*)((s32)npc + 4) &= ~flag;
            break;
        case 1:
            *(u32*)((s32)npc + 4) |= flag;
            break;
    }
    return 2;
}

s32 evt_npc_flag_onoff(void* pEvt) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void* evtNpcNameToPtr(void* event, s32 name);
    s32* args = *(s32**)((s32)pEvt + 0x18);
    s32 onoff = evtGetValue(pEvt, args[0]);
    s32 name = evtGetValue(pEvt, args[1]);
    u32 flag = evtGetValue(pEvt, args[2]);
    void* npc = evtNpcNameToPtr(pEvt, name);

    switch (onoff) {
        case 0:
            *(u32*)npc &= ~flag;
            break;
        case 1:
            *(u32*)npc |= flag;
            break;
    }
    return 2;
}


s32 evt_npc_kamek_kemuri1(int param_1, int param_2) {
    return 0;
}


s32 evt_npc_set_scale(void* pEvt) {
    extern s32 evtGetValue(void*, s32);
    extern f32 evtGetFloat(void*, s32);
    extern void* evtNpcNameToPtr(void*, s32);
    s32* args;
    s32 name;
    f32 x;
    f32 y;
    f32 z;
    void* npc;

    args = *(s32**)((s32)pEvt + 0x18);
    name = evtGetValue(pEvt, args[0]);
    x = evtGetFloat(pEvt, args[1]);
    y = evtGetFloat(pEvt, args[2]);
    z = evtGetFloat(pEvt, args[3]);
    npc = evtNpcNameToPtr(pEvt, name);
    *(f32*)((s32)npc + 0xE0) = x;
    *(f32*)((s32)npc + 0xE4) = y;
    *(f32*)((s32)npc + 0xE8) = z;
    return 2;
}

s32 evt_npc_get_drop_coin(void* pEvt) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void* evtNpcNameToPtr(void* event, s32 name);
    extern void evtSetValue(void* event, s32 dst, s32 value);
    extern void _npc_check_coin_group(void* group, s32* coin);
    s32* args = *(s32**)((s32)pEvt + 0x18);
    s32 name = evtGetValue(pEvt, args[0]);
    void* npc = evtNpcNameToPtr(pEvt, name);
    s32 dst = args[1];
    s32 coin = 0;

    if (*(u32*)((s32)npc + 0x234) & 1) {
        evtSetValue(pEvt, dst, 0);
        return 2;
    }
    _npc_check_coin_group(npc, &coin);
    evtSetValue(pEvt, dst, coin);
    return 2;
}

s32 evt_npc_sound_data_reset(void* pEvt) {
    extern s32 evtGetValue(void*, s32);
    extern void* evtNpcNameToPtr(void*, s32);
    extern f32 float_0_8041fd84;
    s32 name;
    void* npc;
    void* tribe;
    f32 zero;

    name = evtGetValue(pEvt, *(s32*)*(void**)((s32)pEvt + 0x18));
    npc = evtNpcNameToPtr(pEvt, name);
    tribe = *(void**)((s32)npc + 0x28);
    if (tribe != 0) {
        zero = float_0_8041fd84;
        *(s32*)((s32)npc + 0x1E0) = *(s32*)((s32)tribe + 0x40);
        *(s32*)((s32)npc + 0x1E4) = *(s32*)((s32)tribe + 0x44);
        *(s32*)((s32)npc + 0x1EC) = *(s32*)((s32)tribe + 0x4C);
        *(s32*)((s32)npc + 0x1F0) = *(s32*)((s32)tribe + 0x50);
        *(u16*)((s32)npc + 0x1E8) = *(u16*)((s32)tribe + 0x48);
        *(u16*)((s32)npc + 0x1D8) = 1;
        *(f32*)((s32)npc + 0x1DC) = zero;
    }
    return 2;
}

s32 evt_npc_set_battle_rule(void* pEvt) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void* evtNpcNameToPtr(void* event, s32 name);
    s32* args = *(s32**)((s32)pEvt + 0x18);
    s32 name = evtGetValue(pEvt, args[0]);
    void* npc = evtNpcNameToPtr(pEvt, name);
    s32 v0 = evtGetValue(pEvt, args[1]);
    s32 v1 = evtGetValue(pEvt, args[2]);
    s32 v2 = evtGetValue(pEvt, args[3]);

    *(u8*)((s32)npc + 0x2F4) = v0;
    *(u8*)((s32)npc + 0x2F5) = v1;
    *(u8*)((s32)npc + 0x2F6) = v2;
    return 2;
}

s32 evt_npc_pera_onoff(void* pEvt) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void* evtNpcNameToPtr(void* event, s32 name);
    extern void animPosePeraOff(s32 poseId);
    extern void animPosePeraOn(s32 poseId);
    s32* args = *(s32**)((s32)pEvt + 0x18);
    s32 name = evtGetValue(pEvt, args[0]);
    s32 onoff = evtGetValue(pEvt, args[1]);
    void* npc = evtNpcNameToPtr(pEvt, name);

    switch (onoff) {
        case 0:
            animPosePeraOff(*(s32*)((s32)npc + 0x104));
            break;
        case 1:
            animPosePeraOn(*(s32*)((s32)npc + 0x104));
            break;
    }
    return 2;
}

s32 evt_npc_get_rotate(void* pEvt) {
    extern s32 evtGetValue(void*, s32);
    extern void* evtNpcNameToPtr(void*, s32);
    extern void evtSetFloat(void*, s32, f32);
    s32* args;
    s32 dstX;
    s32 dstY;
    s32 dstZ;
    void* npc;

    args = *(s32**)((s32)pEvt + 0x18);
    npc = evtNpcNameToPtr(pEvt, evtGetValue(pEvt, args[0]));
    dstX = args[1];
    dstY = args[2];
    dstZ = args[3];
    evtSetFloat(pEvt, dstX, *(f32*)((s32)npc + 0xEC));
    evtSetFloat(pEvt, dstY, *(f32*)((s32)npc + 0xF0));
    evtSetFloat(pEvt, dstZ, *(f32*)((s32)npc + 0xF4));
    return 2;
}

s32 evt_npc_get_home_position(void* pEvt) {
    extern s32 evtGetValue(void*, s32);
    extern void* evtNpcNameToPtr(void*, s32);
    extern void evtSetFloat(void*, s32, f32);
    s32* args;
    s32 dstX;
    s32 dstY;
    s32 dstZ;
    void* npc;

    args = *(s32**)((s32)pEvt + 0x18);
    npc = evtNpcNameToPtr(pEvt, evtGetValue(pEvt, args[0]));
    dstX = args[1];
    dstY = args[2];
    dstZ = args[3];
    evtSetFloat(pEvt, dstX, *(f32*)((s32)npc + 0x1FC));
    evtSetFloat(pEvt, dstY, *(f32*)((s32)npc + 0x200));
    evtSetFloat(pEvt, dstZ, *(f32*)((s32)npc + 0x204));
    return 2;
}

s32 evt_npc_get_position(void* pEvt) {
    extern s32 evtGetValue(void*, s32);
    extern void* evtNpcNameToPtr(void*, s32);
    extern void evtSetFloat(void*, s32, f32);
    s32* args;
    s32 dstX;
    s32 dstY;
    s32 dstZ;
    void* npc;

    args = *(s32**)((s32)pEvt + 0x18);
    npc = evtNpcNameToPtr(pEvt, evtGetValue(pEvt, args[0]));
    dstX = args[1];
    dstY = args[2];
    dstZ = args[3];
    evtSetFloat(pEvt, dstX, *(f32*)((s32)npc + 0x8C));
    evtSetFloat(pEvt, dstY, *(f32*)((s32)npc + 0x90));
    evtSetFloat(pEvt, dstZ, *(f32*)((s32)npc + 0x94));
    return 2;
}

s32 evt_npc_get_scale(void* pEvt) {
    extern s32 evtGetValue(void*, s32);
    extern void* evtNpcNameToPtr(void*, s32);
    extern void evtSetFloat(void*, s32, f32);
    s32* args;
    s32 dstX;
    s32 dstY;
    s32 dstZ;
    void* npc;

    args = *(s32**)((s32)pEvt + 0x18);
    npc = evtNpcNameToPtr(pEvt, evtGetValue(pEvt, args[0]));
    dstX = args[1];
    dstY = args[2];
    dstZ = args[3];
    evtSetFloat(pEvt, dstX, *(f32*)((s32)npc + 0xE0));
    evtSetFloat(pEvt, dstY, *(f32*)((s32)npc + 0xE4));
    evtSetFloat(pEvt, dstZ, *(f32*)((s32)npc + 0xE8));
    return 2;
}

s32 evt_npc_set_link(void* pEvt) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void* evtNpcNameToPtr(void* event, s32 name);
    extern void npcSetLink(void* npc, void* link);
    s32* args = *(s32**)((s32)pEvt + 0x18);
    s32 name = evtGetValue(pEvt, args[0]);
    s32 linkName = evtGetValue(pEvt, args[1]);
    void* npc = evtNpcNameToPtr(pEvt, name);
    void* link = evtNpcNameToPtr(pEvt, linkName);

    npcSetLink(npc, link);
    return 2;
}

s32 evt_npc_set_offscreen(void* pEvt) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void* evtNpcNameToPtr(void* event, s32 name);
    extern s32 offscreenNameToId(s32 name);
    s32* args = *(s32**)((s32)pEvt + 0x18);
    s32 name = evtGetValue(pEvt, args[0]);
    u32 offscreen = evtGetValue(pEvt, args[1]);
    void* npc = evtNpcNameToPtr(pEvt, name);

    if (offscreen == 0) {
        *(s32*)((s32)npc + 0x110) = -1;
    } else {
        *(s32*)((s32)npc + 0x110) = offscreenNameToId(offscreen);
    }

    return 2;
}

s32 evt_npc_entry(void* pEvt) {
    extern s32 evtGetValue(void* event, s32 value);
    extern s32 animGroupBaseAsync(s32 name, s32 flag, s32 value);
    extern s32 npcEntry(s32 name, s32 modelName);
    extern void* gp;
    s32* args = *(s32**)((s32)pEvt + 0x18);
    s32 name = evtGetValue(pEvt, args[0]);
    s32 modelName = evtGetValue(pEvt, args[1]);
    s32 flag = *(s32*)((s32)gp + 0x14);

    if (animGroupBaseAsync(modelName, flag != 0, 0) == 0) {
        return 0;
    }

    npcEntry(name, modelName);
    return 2;
}

s32 evt_npc_wait_battle_end(void) {
    extern void* fbatGetPointer(void);
    extern s32 seqGetSeq(void);
    extern s32 BattleInformationGetResult(void*);
    extern void marioCtrlOn(void);
    extern s32 marioGetPartyId(void);
    extern void* partyGetPtr(s32);
    extern void partyCtrlOn(void*);
    extern void L_psndBGM_stop(void);
    extern void marioFBattlePost(void);
    void* info;
    void* party;

    info = (void*)((s32)fbatGetPointer() + 0x20);
    if (seqGetSeq() != 2) {
        return 0;
    }
    if (BattleInformationGetResult(info) == 0) {
        return 0;
    }
    marioCtrlOn();
    party = partyGetPtr(marioGetPartyId());
    if (party != 0) {
        partyCtrlOn(party);
    }
    L_psndBGM_stop();
    marioFBattlePost();
    return 2;
}

s32 evt_npc_wait_anim(void* pEvt) {
    extern s32 evtGetValue(void*, s32);
    extern f32 evtGetFloat(void*, s32);
    extern void* evtNpcNameToPtr(void*, s32);
    extern f32 animPoseGetLoopTimes(s32);
    s32* args;
    s32 name;
    f32 wait;
    void* npc;

    args = *(s32**)((s32)pEvt + 0x18);
    name = evtGetValue(pEvt, args[0]);
    wait = evtGetFloat(pEvt, args[1]);
    npc = evtNpcNameToPtr(pEvt, name);
    if (animPoseGetLoopTimes(*(s32*)((s32)npc + 0x104)) >= wait) {
        return 2;
    }
    return 0;
}

s32 evt_npc_set_force_regl_anim(EventEntry* event) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    extern char* strcpy(char* dst, const char* src);
    extern void animPoseSetAnim(void* pose, const char* anim, s32 enabled);
    s32* args;
    s32 name;
    const char* anim;
    void* npc;

    args = event->args;
    name = evtGetValue(event, args[0]);
    anim = (const char*)evtGetValue(event, args[1]);
    npc = evtNpcNameToPtr(event, name);
    strcpy((char*)((s32)npc + 0x2C), anim);
    animPoseSetAnim(*(void**)((s32)npc + 0x104), anim, 1);
    return 2;
}

s32 evt_npc_set_run_anim(EventEntry* event) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    extern char* strcpy(char* dst, const char* src);
    extern void animPoseSetAnim(void* pose, const char* anim, s32 enabled);
    s32 name;
    void* npc;
    const char* anim;
    u8 state;

    name = evtGetValue(event, event->args[0]);
    npc = evtNpcNameToPtr(event, name);
    state = *(u8*)((s32)event + 0x10);
    anim = *(const char**)((s32)*(void**)((s32)npc + 0x28) + 0x1C);

    if (state == 1) {
        strcpy((char*)((s32)npc + 0x2C), anim);
    }

    animPoseSetAnim(*(void**)((s32)npc + 0x104), anim, 1);
    return 2;
}

s32 evt_npc_set_walk_anim(EventEntry* event) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    extern char* strcpy(char* dst, const char* src);
    extern void animPoseSetAnim(void* pose, const char* anim, s32 enabled);
    s32 name;
    void* npc;
    const char* anim;
    u8 state;

    name = evtGetValue(event, event->args[0]);
    npc = evtNpcNameToPtr(event, name);
    state = *(u8*)((s32)event + 0x10);
    anim = *(const char**)((s32)*(void**)((s32)npc + 0x28) + 0x18);

    if (state == 1) {
        strcpy((char*)((s32)npc + 0x2C), anim);
    }

    animPoseSetAnim(*(void**)((s32)npc + 0x104), anim, 1);
    return 2;
}

s32 evt_npc_set_talk_anim(EventEntry* event) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    extern char* strcpy(char* dst, const char* src);
    extern void animPoseSetAnim(void* pose, const char* anim, s32 enabled);
    s32 name;
    void* npc;
    const char* anim;
    u8 state;

    name = evtGetValue(event, event->args[0]);
    npc = evtNpcNameToPtr(event, name);
    state = *(u8*)((s32)event + 0x10);
    anim = *(const char**)((s32)*(void**)((s32)npc + 0x28) + 0x14);

    if (state == 1) {
        strcpy((char*)((s32)npc + 0x2C), anim);
    }

    animPoseSetAnim(*(void**)((s32)npc + 0x104), anim, 1);
    return 2;
}

s32 evt_npc_set_stay_anim(EventEntry* event) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    extern char* strcpy(char* dst, const char* src);
    extern void animPoseSetAnim(void* pose, const char* anim, s32 enabled);
    s32 name;
    void* npc;
    const char* anim;
    u8 state;

    name = evtGetValue(event, event->args[0]);
    npc = evtNpcNameToPtr(event, name);
    state = *(u8*)((s32)event + 0x10);
    anim = *(const char**)((s32)*(void**)((s32)npc + 0x28) + 0x10);

    if (state == 1) {
        strcpy((char*)((s32)npc + 0x2C), anim);
    }

    animPoseSetAnim(*(void**)((s32)npc + 0x104), anim, 1);
    return 2;
}

s32 evt_npc_set_stop_anim(EventEntry* event) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    extern char* strcpy(char* dst, const char* src);
    extern void animPoseSetAnim(void* pose, const char* anim, s32 enabled);
    s32 name;
    void* npc;
    const char* anim;
    u8 state;

    name = evtGetValue(event, event->args[0]);
    npc = evtNpcNameToPtr(event, name);
    state = *(u8*)((s32)event + 0x10);
    anim = *(const char**)((s32)*(void**)((s32)npc + 0x28) + 0xC);

    if (state == 1) {
        strcpy((char*)((s32)npc + 0x2C), anim);
    }

    animPoseSetAnim(*(void**)((s32)npc + 0x104), anim, 1);
    return 2;
}

s32 evt_npc_set_confuse_anim(EventEntry* event) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    extern char* strcpy(char* dst, const char* src);
    extern void animPoseSetAnim(void* pose, const char* anim, s32 enabled);
    s32 name;
    void* npc;
    const char* anim;
    u8 state;

    name = evtGetValue(event, event->args[0]);
    npc = evtNpcNameToPtr(event, name);
    state = *(u8*)((s32)event + 0x10);
    anim = *(const char**)((s32)*(void**)((s32)npc + 0x28) + 0x24);

    if (state == 1) {
        strcpy((char*)((s32)npc + 0x2C), anim);
    }

    animPoseSetAnim(*(void**)((s32)npc + 0x104), anim, 1);
    return 2;
}

s32 evt_npc_set_damage_anim(EventEntry* event) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    extern char* strcpy(char* dst, const char* src);
    extern void animPoseSetAnim(void* pose, const char* anim, s32 enabled);
    s32 name;
    void* npc;
    const char* anim;
    u8 state;

    name = evtGetValue(event, event->args[0]);
    npc = evtNpcNameToPtr(event, name);
    state = *(u8*)((s32)event + 0x10);
    anim = *(const char**)((s32)*(void**)((s32)npc + 0x28) + 0x20);

    if (state == 1) {
        strcpy((char*)((s32)npc + 0x2C), anim);
    }

    animPoseSetAnim(*(void**)((s32)npc + 0x104), anim, 1);
    return 2;
}

USER_FUNC(evt_npc_get_drop_item) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    extern s32 evtSetValue(EventEntry* event, s32 index, s32 value);
    EventEntry* evt = event;
    s32* args = event->args;
    s32 name = evtGetValue(event, args[0]);
    void* npc = evtNpcNameToPtr(event, name);
    s32 dst = args[1];

    if ((*(u32*)((s32)npc + 0x234) & 8) != 0) {
        evtSetValue(evt, dst, 0);
    } else {
        evtSetValue(evt, dst, *(s32*)((s32)npc + 0x23C));
    }
    return 2;
}

s32 evt_npc_blur_onoff(EventEntry* event) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    extern void npcBlurOn(void* npc);
    extern void npcBlurOff(void* npc);
    s32* args = event->args;
    s32 on = evtGetValue(event, args[0]);
    s32 name = evtGetValue(event, args[1]);
    void* npc = evtNpcNameToPtr(event, name);

    npc = (void*)((s32)npc + 8);
    if (on) {
        npcBlurOn(npc);
    } else {
        npcBlurOff(npc);
    }
    return 2;
}

s32 evt_npc_release_filednpc(void* pEvt) {
    extern s32 evtGetValue(void*, s32);
    extern void* npcGetWorkPtr(void);
    s32 value;
    void* work;

    value = evtGetValue(pEvt, *(s32*)*(void**)((s32)pEvt + 0x18));
    work = npcGetWorkPtr();
    if (value != 0) {
        *(u32*)((s32)work + 8) |= 2;
    } else {
        *(u32*)((s32)work + 8) &= ~2;
    }
    return 2;
}

s32 evt_npc_reverse_ry(void* pEvt) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void* evtNpcNameToPtr(void* event, s32 name);
    extern f32 reviseAngle(f32 angle);
    extern f32 float_180_8041fdb0;
    s32* args = *(s32**)((s32)pEvt + 0x18);
    s32 name = evtGetValue(pEvt, args[0]);
    void* npc = evtNpcNameToPtr(pEvt, name);

    *(f32*)((s32)npc + 0x144) = reviseAngle(float_180_8041fdb0 + *(f32*)((s32)npc + 0x144));
    return 2;
}

USER_FUNC(evt_npc_get_kpencount_type) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    extern s32 evtSetValue(EventEntry* event, s32 index, s32 value);
    s32* args = event->args;
    s32 name = evtGetValue(event, args[0]);
    s32 dst = args[1];
    void* npc = evtNpcNameToPtr(event, name);
    evtSetValue(event, dst, *(u16*)((s32)npc + 0x31C));
    return 2;
}

u32 evt_npc_wait_pera(void* pEvt) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void* evtNpcNameToPtr(void* event, s32 name);
    extern u32 animPoseGetPeraEnd(s32 poseId);
    s32* args = *(s32**)((s32)pEvt + 0x18);
    s32 name = evtGetValue(pEvt, args[0]);
    void* npc = evtNpcNameToPtr(pEvt, name);

    return animPoseGetPeraEnd(*(s32*)((s32)npc + 0x104)) != 0 ? 2 : 0;
}


USER_FUNC(evt_npc_get_height) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    extern f32 evtSetFloat(EventEntry* event, s32 target, f32 value);
    s32* args = event->args;
    s32 name = evtGetValue(event, args[0]);
    s32 dst = args[1];
    void* npc = evtNpcNameToPtr(event, name);
    evtSetFloat(event, dst, *(f32*)((s32)npc + 0x150));
    return 2;
}

s32 evt_npc_restart_regular_event(void* pEvt) {
    extern s32 evtGetValue(void*, s32);
    extern void* evtNpcNameToPtr(void*, s32);
    extern void npcCheckInterrupt(void*);
    s32 name;
    void* npc;

    name = evtGetValue(pEvt, *(s32*)*(void**)((s32)pEvt + 0x18));
    npc = evtNpcNameToPtr(pEvt, name);
    *(u32*)((s32)npc + 0x140) |= 0x10000000;
    npcCheckInterrupt(npc);
    return 2;
}

s32 evt_npc_clear_paper(void* pEvt) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void* evtNpcNameToPtr(void* event, s32 name);
    extern void animPoseSetPaperAnimGroup(s32 poseId, s32 anim, s32 enabled);
    s32* args = *(s32**)((s32)pEvt + 0x18);
    s32 name = evtGetValue(pEvt, args[0]);
    void* npc = evtNpcNameToPtr(pEvt, name);

    animPoseSetPaperAnimGroup(*(s32*)((s32)npc + 0x104), 0, 1);
    return 2;
}

s32 evt_npc_return_interrupt(void* pEvt) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void* evtNpcNameToPtr(void* event, s32 name);
    s32* args = *(s32**)((s32)pEvt + 0x18);
    s32 name = evtGetValue(pEvt, args[0]);
    void* npc = evtNpcNameToPtr(pEvt, name);

    *(u32*)((s32)npc + 0x140) |= 0x10000000;
    return 1;
}

s32 evt_npc_calc_score(void* pEvt) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void* evtNpcNameToPtr(void* event, s32 name);
    extern s32 npcCalcScore(void* npc);

    s32* args;
    s32 name;
    void* npc;

    args = *(s32**)((s32)pEvt + 0x18);
    name = evtGetValue(pEvt, args[0]);
    npc = evtNpcNameToPtr(pEvt, name);
    npcCalcScore(npc);

    return 2;
}

s32 evt_npc_set_anim(void* evt, s32 isFirstCall) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void* evtNpcNameToPtr(void* event, s32 name);
    extern char* strcpy(char* dst, const char* src);
    extern void animPoseSetAnim(s32 poseId, void* name, s32 force);
    s32* args = *(s32**)((s32)evt + 0x18);
    s32 name = evtGetValue(evt, args[0]);
    s32 anim = evtGetValue(evt, args[1]);
    void* npc = evtNpcNameToPtr(evt, name);

    if (*(u8*)((s32)evt + 0x10) == 1) {
        strcpy((char*)((s32)npc + 0x2C), (char*)anim);
    }
    animPoseSetAnim(*(s32*)((s32)npc + 0x104), (void*)anim, 1);
    return 2;
}

s32 unk_8004d180(EventEntry* event) {
    extern f32 evtGetFloat(EventEntry* event, s32 index);
    extern void evtSetFloat(EventEntry* event, s32 index, f32 value);
    extern s32 hitCheckFilter(f32 x, f32 y, f32 z, f32 dx, f32 dy, f32 dz, s32 flags, void* out0, f32* outY, void* out2, f32* out3, void* out4, void* out5, void* out6);
    extern f32 float_0_8041fd84;
    extern f32 float_1_8041fd88;
    extern f32 float_neg1_8041fd8c;
    extern f32 float_0p5_8041fd90;
    s32* args;
    s32 xVar;
    s32 yVar;
    s32 zVar;
    f32 x;
    f32 y;
    f32 z;
    f32 addY;
    f32 out3;
    f32 width;
    f32 bestY;
    f32 checkY;
    f32 halfWidth;
    s32 out0;
    s32 out2;
    s32 out4;
    s32 out5;
    s32 out6;

    args = event->args;
    xVar = args[0];
    x = evtGetFloat(event, xVar);
    yVar = args[1];
    y = evtGetFloat(event, yVar);
    zVar = args[2];
    z = evtGetFloat(event, zVar);
    addY = evtGetFloat(event, args[3]);
    out3 = evtGetFloat(event, args[4]);
    width = evtGetFloat(event, args[5]);

    bestY = y;
    y += float_1_8041fd88;
    if (hitCheckFilter(x, y, z, float_0_8041fd84, float_neg1_8041fd8c, float_0_8041fd84, 0, &out0, &checkY, &out2, &out3, &out4, &out5, &out6) != 0) {
        bestY = checkY + addY;
    }

    halfWidth = float_0p5_8041fd90 * width;
    if (hitCheckFilter(x + halfWidth, y, z, float_0_8041fd84, float_neg1_8041fd8c, float_0_8041fd84, 0, &out0, &checkY, &out2, &out3, &out4, &out5, &out6) != 0) {
        if (bestY < checkY + addY) {
            bestY = checkY + addY;
        }
    }

    if (hitCheckFilter(x - halfWidth, y, z, float_0_8041fd84, float_neg1_8041fd8c, float_0_8041fd84, 0, &out0, &checkY, &out2, &out3, &out4, &out5, &out6) != 0) {
        if (bestY < checkY + addY) {
            bestY = checkY + addY;
        }
    }

    evtSetFloat(event, xVar, x);
    evtSetFloat(event, yVar, bestY);
    evtSetFloat(event, zVar, z);
    return 2;
}
