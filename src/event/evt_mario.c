#include "event/evt_mario.h"

USER_FUNC(evt_mario_bgmode_off) {
    extern void marioBgmodeOff(void);
    marioBgmodeOff();
    return 2;
}

USER_FUNC(evt_mario_bgmode_on) {
    extern void marioBgmodeOn(void);
    marioBgmodeOn();
    return 2;
}

USER_FUNC(evt_mario_init_camid) {
    extern void marioInitCamId(void);
    marioInitCamId();
    return 2;
}

USER_FUNC(evt_mario_set_talk_motion) {
    extern void marioChgTalkMotion(void);
    marioChgTalkMotion();
    return 2;
}

USER_FUNC(evt_mario_adjust_dir) {
    extern void marioAdjustMoveDir(void);
    marioAdjustMoveDir();
    return 2;
}

USER_FUNC(evt_mario_cam_keep_off) {
    extern void camFollowYOn(void);
    camFollowYOn();
    return 2;
}

USER_FUNC(evt_mario_cam_keep_on) {
    extern void camFollowYOff(void);
    camFollowYOff();
    return 2;
}

USER_FUNC(evt_mario_cam_off) {
    extern void marioNoUpdateCamPos(void);
    marioNoUpdateCamPos();
    return 2;
}

USER_FUNC(evt_mario_cam_on) {
    extern void marioUpdateCamPos(void);
    marioUpdateCamPos();
    return 2;
}

USER_FUNC(evt_mario_cam_y_off) {
    extern void camFollowYOff(void);
    camFollowYOff();
    return 2;
}

USER_FUNC(evt_mario_cam_y_on) {
    extern void camFollowYOn(void);
    camFollowYOn();
    return 2;
}

USER_FUNC(evt_mario_paper_pose_off) {
    extern void marioPaperOff(void);
    marioPaperOff();
    return 2;
}

USER_FUNC(evt_mario_set_camid) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern void marioSetCamId(s32 id);
    marioSetCamId(evtGetValue(event, event->args[0]));
    return 2;
}

USER_FUNC(evt_mario_set_mode) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern void marioSetCharMode(s32 mode);
    marioSetCharMode(evtGetValue(event, event->args[0]));
    return 2;
}

USER_FUNC(evt_mario_set_motion) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern void marioChgMot(s32 motion);
    marioChgMot(evtGetValue(event, event->args[0]));
    return 2;
}

USER_FUNC(evt_mario_cont_onoff) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern void marioCtrlOn(void);
    extern void marioCtrlOff(void);
    if (!evtGetValue(event, event->args[0])) {
        marioCtrlOff();
    } else {
        marioCtrlOn();
    }
    return 2;
}

USER_FUNC(evt_mario_set_enable_key) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern void* marioGetPtr(void);
    s32 value = evtGetValue(event, event->args[0]);
    *(s32*)((s32)marioGetPtr() + 0x290) = value;
    return 2;
}

USER_FUNC(evt_mario_sleep_on) {
    extern void marioChgEvtPose(const char* pose);
    extern const char str_M_N_5A_802e3eb0[];
    marioChgEvtPose(str_M_N_5A_802e3eb0);
    return 2;
}

USER_FUNC(evt_mario_dump_item) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern void pouchRemoveItem(s32 item);
    pouchRemoveItem(evtGetValue(event, event->args[0]));
    return 2;
}

USER_FUNC(evt_mario_push) {
    extern void* marioGetPtr(void);
    void* mario = marioGetPtr();
    *(u32*)mario |= 0x8000;
    return 2;
}

USER_FUNC(evt_mario_set_reardir) {
    extern void* marioGetPtr(void);
    extern void marioAdjustMoveDir(void);
    extern f32 float_0_80421adc;
    void* mario = marioGetPtr();
    f32 zero;
    marioAdjustMoveDir();
    zero = float_0_80421adc;
    *(f32*)((s32)mario + 0x1A4) = zero;
    *(f32*)((s32)mario + 0x1A0) = zero;
    return 2;
}

USER_FUNC(evt_mario_wait_movable) {
    extern void* marioGetPtr(void);
    return (*(u32*)marioGetPtr() & 8) ? 0 : 2;
}

USER_FUNC(evt_mario_balloon_off) {
    extern void* marioGetPtr(void);
    extern void marioBalloonOff(void);
    void* mario = marioGetPtr();
    if (*(u32*)((s32)mario + 4) & 0x04000000) {
        marioBalloonOff();
        *(u32*)((s32)mario + 4) &= ~0x04000000;
    }
    return 2;
}

USER_FUNC(evt_mario_balloon_bikkuri) {
    extern void* marioGetPtr(void);
    extern void marioBalloonOn(s32 type);
    void* mario = marioGetPtr();
    if (!(*(u32*)((s32)mario + 4) & 0x04000000)) {
        marioBalloonOn(0);
        *(u32*)((s32)mario + 4) |= 0x04000000;
    }
    return 2;
}

USER_FUNC(evt_mario_balloon_tenten) {
    extern void* marioGetPtr(void);
    extern void marioBalloonOn(s32 type);
    void* mario = marioGetPtr();
    if (!(*(u32*)((s32)mario + 4) & 0x04000000)) {
        marioBalloonOn(1);
        *(u32*)((s32)mario + 4) |= 0x04000000;
    }
    return 2;
}


u8 evt_mario_jump_jumpstand(void) {
    return 0;
}


u8 evt_mario_party_door_move(s32 pEvt, s32 param_2) {
    return 0;
}


u8 evt_mario_party_bero_move(s32 pEvt, s32 param_2) {
    return 0;
}


int evt_mario_paper_pose_dokan(void* pEvt, int param_2) {
    return 0;
}


u8 evt_mario_jump_pos(s32 pEvt, s32 param_2) {
    return 0;
}


u8 evt_koopa_hip_attack(void) {
    return 0;
}


int evt_mario_normalize(void* pEvt, int param_2) {
    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_mario_set_dir(EventEntry* event, s32 first) {
    extern void* marioGetPtr(void);
    extern f32 evtGetFloat(EventEntry* event, s32 value);
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern s32 sysMsec2Frame(s32 msec);
    extern f32 revise360(f32 angle);
    extern f32 toMovedirSimple(f32 angle);
    extern const f32 float_270_80421b1c;
    extern const f32 float_360_80421b44;
    extern const f32 float_0_80421adc;
    EventEntry* evt;
    s32 firstCall;
    s32* args;
    void* mario;
    f32 dir;
    s32 frames;
    s32 wait;
    f32 angle;

    evt = event;
    firstCall = first;
    args = event->args;
    mario = marioGetPtr();
    dir = evtGetFloat(evt, args[0]);
    frames = sysMsec2Frame(evtGetValue(evt, args[1]));
    wait = evtGetValue(evt, args[2]);

    if (firstCall != 0) {
        *(s32*)((s32)evt + 0x78) = 0;
        *(s32*)((s32)evt + 0x80) = 0;
        *(f32*)((s32)evt + 0x7C) = *(f32*)((s32)mario + 0x1AC);
        if (dir > float_270_80421b1c) {
            dir -= float_360_80421b44;
        }
    }

    if (*(s32*)((s32)evt + 0x80) == 1) {
        *(f32*)((s32)mario + 0x1A4) = float_0_80421adc;
        *(f32*)((s32)mario + 0x1A0) = float_0_80421adc;
        return 2;
    }

    if (frames > 0) {
        angle = revise360(float_270_80421b1c - dir);
        if (angle > float_270_80421b1c) {
            angle -= float_360_80421b44;
        }
        angle = revise360(*(f32*)((s32)evt + 0x7C) + ((angle - *(f32*)((s32)evt + 0x7C)) * ((f32)*(s32*)((s32)evt + 0x78) / (f32)frames)));
        *(f32*)((s32)mario + 0x1AC) = angle;
        *(f32*)((s32)mario + 0x1B0) = *(f32*)((s32)mario + 0x1AC);
        *(f32*)((s32)mario + 0x1A4) = toMovedirSimple(*(f32*)((s32)mario + 0x1AC));
        *(f32*)((s32)mario + 0x1A0) = *(f32*)((s32)mario + 0x1A4);
        *(s32*)((s32)evt + 0x78) = *(s32*)((s32)evt + 0x78) + 1;
        if (*(s32*)((s32)evt + 0x78) > frames) {
            if (wait == 0) {
                return 1;
            }
            *(s32*)((s32)evt + 0x80) = 1;
        }
        return 0;
    }

    *(f32*)((s32)mario + 0x1AC) = revise360(float_270_80421b1c - dir);
    *(f32*)((s32)mario + 0x1B0) = *(f32*)((s32)mario + 0x1AC);
    *(f32*)((s32)mario + 0x1A4) = toMovedirSimple(*(f32*)((s32)mario + 0x1AC));
    *(f32*)((s32)mario + 0x1A0) = *(f32*)((s32)mario + 0x1A4);
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_mario_mov_pos2(EventEntry* event, s32 first) {
    extern void* marioGetPtr(void);
    extern f32 evtGetFloat(EventEntry* event, s32 value);
    extern f32 angleABf(f32 x1, f32 z1, f32 x2, f32 z2);
    extern f32 distABf(f32 x1, f32 z1, f32 x2, f32 z2);
    extern void* gp;
    extern void marioSetForceMove(s32 frames, f32 dir, f32 speed);
    extern const f32 float_0p4_80421b0c;
    extern const f32 float_1_80421ad8;
    EventEntry* evt = event;
    s32* args = event->args;
    void* mario = marioGetPtr();
    f32 x = evtGetFloat(evt, args[0]);
    f32 z = evtGetFloat(evt, args[1]);
    f32 speed = evtGetFloat(evt, args[2]);
    f32 dist;
    s32 frames;

    if (first != 0) {
        dist = distABf(*(f32*)((s32)mario + 0x8C), *(f32*)((s32)mario + 0x94), x, z);
        *(f32*)((s32)mario + 0x1A4) = angleABf(*(f32*)((s32)mario + 0x8C), *(f32*)((s32)mario + 0x94), x, z);
        if ((*(u32*)mario & 0x02000000) != 0) {
            speed *= float_0p4_80421b0c;
        }
        speed = speed / (f32)*(s32*)((s32)gp + 4);
        frames = (s32)((float_1_80421ad8 / speed) + (dist / speed));
        marioSetForceMove(frames, *(f32*)((s32)mario + 0x1A4), speed);
    }
    if ((*(u32*)mario & 0x20) != 0) {
        return 0;
    }
    *(f32*)((s32)mario + 0x8C) = x;
    *(f32*)((s32)mario + 0x94) = z;
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_mario_dokan_prepare(EventEntry* event) {
    extern void* marioGetPtr(void);
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern s32 marioGetPartyId(void);
    extern s32 marioGetExtraPartyId(void);
    extern void* partyGetPtr(s32 party);
    extern void partyShadowOff(void* party);
    extern s32 vec3_802e3cd8[9];
    EventEntry* evt = event;
    s32* args = event->args;
    void* mario = marioGetPtr();
    void* party;
    s32 mode;

    *(u32*)mario |= 0x100;
    mode = evtGetValue(evt, args[0]);
    if (mode == 4) {
        *(s32*)((s32)mario + 0xC8) = vec3_802e3cd8[0];
        *(s32*)((s32)mario + 0xCC) = vec3_802e3cd8[1];
        *(s32*)((s32)mario + 0xD0) = vec3_802e3cd8[2];
        *(u32*)((s32)mario + 4) |= 0x1000;
        party = partyGetPtr(marioGetPartyId());
        if (party != 0) {
            partyShadowOff(party);
            *(s32*)((s32)party + 0x70) = vec3_802e3cd8[3];
            *(s32*)((s32)party + 0x74) = vec3_802e3cd8[4];
            *(s32*)((s32)party + 0x78) = vec3_802e3cd8[5];
            *(u32*)((s32)party + 4) |= 1;
            *(u32*)party |= 0x04000000;
            *(u32*)party |= 0x02000000;
        }
        party = partyGetPtr(marioGetExtraPartyId());
        if (party != 0) {
            partyShadowOff(party);
            *(s32*)((s32)party + 0x70) = vec3_802e3cd8[6];
            *(s32*)((s32)party + 0x74) = vec3_802e3cd8[7];
            *(s32*)((s32)party + 0x78) = vec3_802e3cd8[8];
            *(u32*)((s32)party + 4) |= 1;
            *(u32*)party |= 0x04000000;
            *(u32*)party |= 0x02000000;
        }
    } else if (mode == 5) {
        *(u32*)((s32)mario + 4) |= 0x1000;
        partyShadowOff(partyGetPtr(marioGetPartyId()));
        partyShadowOff(partyGetPtr(marioGetExtraPartyId()));
    }
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_mario_set_normal_pose(void) {
    extern void* marioGetPtr(void);
    extern void* toDotMarioPose(void* pose);
    extern s32 strcmp(const char* a, const char* b);
    extern char* a_mario_group[];
    extern const char str_M_W_1_804219a0[];
    extern const char str_M_R_1_804219a8[];
    extern const char str_M_S_1_80421990[];
    void* mario;
    void* pose;
    char** group;
    s32 notMarioGroup;

    mario = marioGetPtr();
    switch (*(u16*)((s32)mario + 0x2E)) {
        case 0x18:
        case 0x19:
        case 0x16:
        case 0x15:
            return 2;
    }
    if (*(u16*)((s32)mario + 0x2E) == 1) {
        pose = (void*)str_M_W_1_804219a0;
    } else if (*(u16*)((s32)mario + 0x2E) == 2) {
        pose = (void*)str_M_R_1_804219a8;
    } else {
        pose = (void*)str_M_S_1_80421990;
    }
    if ((*(u32*)mario & 0x80000000) != 0) {
        pose = toDotMarioPose(pose);
        if (pose == 0) {
            return 2;
        }
        *(void**)((s32)mario + 0x18) = pose;
        *(u32*)((s32)mario + 0xC) |= 0x1000;
    } else {
        notMarioGroup = 1;
        group = a_mario_group;
        while (*group != 0) {
            if (strcmp((char*)pose, *group) == 0) {
                notMarioGroup = 0;
                break;
            }
            group++;
        }
        *(void**)((s32)mario + 0x18) = pose;
        *(u32*)((s32)mario + 0xC) |= 0x1000;
        if (notMarioGroup == 0) {
            *(u32*)((s32)mario + 4) &= ~0x10000000;
        } else {
            *(u32*)((s32)mario + 4) |= 0x10000000;
        }
    }
    if ((*(u32*)((s32)mario + 4) & 0x01000000) != 0) {
        *(u32*)((s32)mario + 0xC) |= 0x8000;
    }
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_mario_mov_pos(EventEntry* event, s32 first) {
    extern void* marioGetPtr(void);
    extern f32 evtGetFloat(EventEntry* event, s32 value);
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern s32 sysMsec2Frame(s32 msec);
    extern f32 angleABf(f32 x1, f32 z1, f32 x2, f32 z2);
    extern f32 distABf(f32 x1, f32 z1, f32 x2, f32 z2);
    extern void marioSetForceMove(s32 frames, f32 dir, f32 speed);
    EventEntry* evt = event;
    s32* args = event->args;
    void* mario = marioGetPtr();
    f32 x;
    f32 z;
    f32 speed;
    s32 frames;

    if (first != 0) {
        x = evtGetFloat(evt, args[0]);
        z = evtGetFloat(evt, args[1]);
        *(s32*)((s32)evt + 0x78) = sysMsec2Frame(evtGetValue(evt, args[2]));
        *(f32*)((s32)mario + 0x1A4) = angleABf(*(f32*)((s32)mario + 0x8C), *(f32*)((s32)mario + 0x94), x, z);
        if (*(s32*)((s32)evt + 0x78) == 0) {
            frames = (s32)(distABf(*(f32*)((s32)mario + 0x8C), *(f32*)((s32)mario + 0x94), x, z) / *(f32*)((s32)mario + 0x180));
            *(s32*)((s32)evt + 0x78) = frames;
            speed = *(f32*)((s32)mario + 0x180);
        } else {
            speed = distABf(*(f32*)((s32)mario + 0x8C), *(f32*)((s32)mario + 0x94), x, z) / (f32)*(s32*)((s32)evt + 0x78);
        }
        marioSetForceMove(*(s32*)((s32)evt + 0x78), *(f32*)((s32)mario + 0x1A4), speed);
    }
    *(s32*)((s32)evt + 0x78) = *(s32*)((s32)evt + 0x78) - 1;
    return (*(s32*)((s32)evt + 0x78) < 0) ? 1 : 0;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_mario_set_prev_party_pos(EventEntry* event, s32 first) {
    extern void* marioGetPtr(void);
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern s32 partyEntryPos(s32 party, f32 x, f32 y, f32 z);
    extern s32 partyEntry2Pos(s32 party, f32 x, f32 y, f32 z);
    EventEntry* evt = event;
    s32* args = event->args;
    void* mario = marioGetPtr();
    s32 offset = args[0];
    void* entry = (void*)((s32)mario + offset);
    f32 x;
    f32 y;
    f32 z;
    s32 party;

    if (first != 0) {
        if (*(s8*)((s32)entry + 0x245) >= 0) {
            return 2;
        }
        x = (f32)evtGetValue(evt, args[1]);
        y = (f32)evtGetValue(evt, args[2]);
        z = (f32)evtGetValue(evt, args[3]);
        party = *(s8*)((s32)entry + 0x247);
        if (party == 0) {
            return 2;
        }
        if (offset == 0) {
            party = partyEntryPos(party, x, y, z);
        } else {
            party = partyEntry2Pos(party, x, y, z);
        }
        if (party < 0) {
            return 2;
        }
        *(s8*)((s32)entry + 0x245) = party;
    }
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_mario_set_party_pos(EventEntry* event, s32 first) {
    extern void* marioGetPtr(void);
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern s32 partyEntryPos(s32 party, f32 x, f32 y, f32 z);
    extern s32 partyEntry2Pos(s32 party, f32 x, f32 y, f32 z);
    EventEntry* evt = event;
    s32* args = event->args;
    void* mario = marioGetPtr();
    s32 offset = args[0];
    void* entry = (void*)((s32)mario + offset);
    s32 party;
    f32 x;
    f32 y;
    f32 z;

    if (first != 0) {
        if (*(s8*)((s32)entry + 0x245) >= 0) {
            return 2;
        }
        party = evtGetValue(evt, args[1]);
        x = (f32)evtGetValue(evt, args[2]);
        y = (f32)evtGetValue(evt, args[3]);
        z = (f32)evtGetValue(evt, args[4]);
        if (offset == 0) {
            party = partyEntryPos(party, x, y, z);
        } else {
            party = partyEntry2Pos(party, x, y, z);
        }
        if (party < 0) {
            return 2;
        }
        *(s8*)((s32)entry + 0x245) = party;
    }
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_mario_set_pose(EventEntry* event) {
    extern void* marioGetPtr(void);
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern void* toDotMarioPose(void* pose);
    extern s32 strcmp(const char* a, const char* b);
    extern char* a_mario_group[];
    EventEntry* evt = event;
    s32* args = event->args;
    void* mario;
    void* pose;
    char** group;
    s32 notMarioGroup;

    mario = marioGetPtr();
    switch (*(u16*)((s32)mario + 0x2E)) {
        case 0x18:
        case 0x19:
        case 0x16:
        case 0x15:
            return 2;
    }
    pose = (void*)evtGetValue(evt, args[0]);
    if ((*(u32*)mario & 0x80000000) != 0) {
        pose = toDotMarioPose(pose);
        if (pose == 0) {
            return 2;
        }
        *(void**)((s32)mario + 0x18) = pose;
        *(u32*)((s32)mario + 0xC) |= 0x1000;
    } else {
        notMarioGroup = 1;
        group = a_mario_group;
        while (*group != 0) {
            if (strcmp((char*)pose, *group) == 0) {
                notMarioGroup = 0;
                break;
            }
            group++;
        }
        *(void**)((s32)mario + 0x18) = pose;
        *(u32*)((s32)mario + 0xC) |= 0x1000;
        if (notMarioGroup == 0) {
            *(u32*)((s32)mario + 4) &= ~0x10000000;
        } else {
            *(u32*)((s32)mario + 4) |= 0x10000000;
        }
    }
    if ((*(u32*)((s32)mario + 4) & 0x01000000) != 0) {
        *(u32*)((s32)mario + 0xC) |= 0x8000;
    }
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_mario_hello_exparty_pos(EventEntry* event, s32 first) {
    extern void* marioGetPtr(void);
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern s32 partyEntry2Hello(s32 party);
    extern void* partyGetPtr(s32 party);
    EventEntry* evt = event;
    s32* args = event->args;
    void* mario = marioGetPtr();
    void* partyPtr;
    s32 party;

    if (first != 0) {
        if (*(s8*)((s32)mario + 0x246) >= 0) {
            return 2;
        }
        party = evtGetValue(evt, args[0]);
        evtGetValue(evt, args[1]);
        evtGetValue(evt, args[2]);
        evtGetValue(evt, args[3]);
        party = partyEntry2Hello(party);
        if (party < 0) {
            return 2;
        }
        *(s8*)((s32)mario + 0x246) = party;
        return 0;
    }
    partyPtr = partyGetPtr(1);
    if (partyPtr == 0) {
        return 2;
    }
    if ((*(u32*)partyPtr & 1) != 0 && *(u8*)((s32)partyPtr + 0x34) == 1) {
        return 2;
    }
    return 0;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_mario_check_landon(EventEntry* event) {
    extern void* marioGetPtr(void);
    extern s32 evtSetValue(EventEntry* event, s32 target, s32 value);
    EventEntry* evt = event;
    s32* args = event->args;
    void* mario;
    u32 motion;
    s32 ok;

    if (*(void**)((s32)marioGetPtr() + 0x1E8) == 0) {
        ok = 0;
    } else {
        mario = marioGetPtr();
        motion = *(u16*)((s32)mario + 0x2E);
        if (motion <= 1) {
            ok = 1;
        } else if (motion == 2) {
            ok = 1;
        } else if (motion == 0x16 && *(s16*)((s32)mario + 0x50) == 0) {
            ok = 1;
        } else if (motion == 0x15 && *(s16*)((s32)mario + 0x50) == 0) {
            ok = 1;
        } else if (motion == 0x19 && *(s16*)((s32)mario + 0x50) == 0) {
            ok = 1;
        } else if (motion == 0x1A) {
            ok = 1;
        } else {
            ok = 0;
        }
    }
    if (ok != 0) {
        evtSetValue(evt, args[0], 1);
        return 1;
    }
    evtSetValue(evt, args[0], 0);
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_mario_hello_party(EventEntry* event, s32 first) {
    extern void* marioGetPtr(void);
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern s32 partyEntryHello(s32 party);
    extern s32 partyEntry2Hello(s32 party);
    extern void* partyGetPtr(s32 party);
    EventEntry* evt = event;
    s32* args = event->args;
    void* mario = marioGetPtr();
    s32 offset = args[0];
    void* entry;
    void* partyPtr;
    s32 party;

    if (first != 0) {
        entry = (void*)((s32)mario + offset);
        if (*(s8*)((s32)entry + 0x245) >= 0) {
            return 2;
        }
        party = evtGetValue(evt, args[1]);
        if (offset == 0) {
            party = partyEntryHello(party);
        } else {
            party = partyEntry2Hello(party);
        }
        if (party < 0) {
            return 2;
        }
        *(s8*)((s32)entry + 0x245) = party;
        return 0;
    }
    partyPtr = partyGetPtr(offset);
    if (partyPtr == 0) {
        return 2;
    }
    if ((*(u32*)partyPtr & 1) != 0 && *(u8*)((s32)partyPtr + 0x34) == 1) {
        return 2;
    }
    return 0;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_koopa_fire(EventEntry* event, s32 first) {
    extern void* marioGetPtr(void);
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern s32 sysMsec2Frame(s32 msec);
    extern void kpaAttackStart(void);
    extern void marioChgMot(s32 mot);
    EventEntry* evt = event;
    s32* args = event->args;
    void* mario = marioGetPtr();
    s32 time;

    if (first != 0) {
        *(s32*)((s32)evt + 0x78) = 0;
        time = (s32)(f32)sysMsec2Frame(evtGetValue(evt, args[0]));
        *(s32*)((s32)evt + 0x80) = time;
        kpaAttackStart();
        marioChgMot(0);
    }
    if (*(s32*)((s32)evt + 0x78) == 0) {
        time = *(s32*)((s32)evt + 0x80) - 1;
        *(s32*)((s32)evt + 0x80) = time;
        if (time != 0) {
            *(s32*)(*(s32*)((s32)mario + 0x298) + 0x144) = 1;
            return 0;
        }
    }
    *(s32*)(*(s32*)((s32)mario + 0x298) + 0x144) = 0;
    return 1;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_mario_set_prev_party_dokan(EventEntry* event, s32 first) {
    extern void* marioGetPtr(void);
    extern s32 partyEntry(s32 party);
    extern s32 partyEntry2(s32 party);
    extern void* partyGetPtr(s32 party);
    extern void partyChgRunMode(void* party, s32 mode);
    extern s32 vec3_802e3e9c[3];
    s32* args = event->args;
    void* mario = marioGetPtr();
    s32 offset = args[0];
    void* entry;
    void* partyPtr;
    s32 party;

    if (first != 0) {
        entry = (void*)((s32)mario + offset);
        if (*(s8*)((s32)entry + 0x245) >= 0) {
            return 2;
        }
        party = *(s8*)((s32)entry + 0x247);
        if (party == 0) {
            return 2;
        }
        if (offset == 0) {
            party = partyEntry(party);
        } else {
            party = partyEntry2(party);
        }
        if (party >= 0) {
            *(s8*)((s32)entry + 0x245) = party;
            partyPtr = partyGetPtr(party);
            partyChgRunMode(partyPtr, 0);
            *(u32*)partyPtr |= 0x04000000;
            *(u32*)partyPtr |= 0x02000000;
            *(s32*)((s32)partyPtr + 0x70) = vec3_802e3e9c[0];
            *(s32*)((s32)partyPtr + 0x74) = vec3_802e3e9c[1];
            *(s32*)((s32)partyPtr + 0x78) = vec3_802e3e9c[2];
        }
    }
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_mario_set_dir_npc(EventEntry* event) {
    extern void* marioGetPtr(void);
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern void* evtNpcNameToPtr(EventEntry* event, s32 name);
    extern void* camGetPtr(s32 id);
    extern f32 angleABf(f32 x1, f32 z1, f32 x2, f32 z2);
    extern f32 revise360(f32 angle);
    extern f32 toMovedirSimple(f32 angle);
    extern const f32 float_180_80421b34;
    extern const f32 float_0_80421adc;
    EventEntry* evt = event;
    s32* args = event->args;
    void* mario;
    void* npc;
    f32 angle;
    f32 bucket;

    mario = marioGetPtr();
    npc = evtNpcNameToPtr(evt, evtGetValue(evt, args[0]));
    if (npc == 0) {
        return 2;
    }
    angle = revise360(angleABf(*(f32*)((s32)mario + 0x8C), *(f32*)((s32)mario + 0x94), *(f32*)((s32)npc + 0x8C), *(f32*)((s32)npc + 0x94)) - *(f32*)((s32)camGetPtr(4) + 0x114));
    bucket = float_180_80421b34;
    if (angle >= float_180_80421b34) {
        bucket = float_0_80421adc;
    }
    *(f32*)((s32)mario + 0x1B0) = bucket;
    *(f32*)((s32)mario + 0x1A0) = toMovedirSimple(*(f32*)((s32)mario + 0x1B0));
    *(f32*)((s32)mario + 0x1A4) = *(f32*)((s32)mario + 0x1A0);
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_mario_set_prev_party(EventEntry* event, s32 first) {
    extern void* marioGetPtr(void);
    extern s32 partyEntry(s32 party);
    extern s32 partyEntry2(s32 party);
    extern void* partyGetPtr(s32 party);
    s32* args = event->args;
    void* mario = marioGetPtr();
    s32 offset = args[0];
    void* entry = (void*)((s32)mario + offset);
    s32 party;

    if (first != 0) {
        if (*(s8*)((s32)entry + 0x245) >= 0) {
            return 2;
        }
        party = *(s8*)((s32)entry + 0x247);
        if (party == 0) {
            return 2;
        }
        if (offset == 0) {
            party = partyEntry(party);
        } else {
            party = partyEntry2(party);
        }
        if (party < 0) {
            return 2;
        }
        *(s8*)((s32)entry + 0x245) = party;
    }
    partyGetPtr(*(s8*)((s32)mario + offset + 0x245));
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_mario_wait_rideon(void) {
    extern void* marioGetPtr(void);
    void* mario;
    u32 motion;
    s32 ok;

    if (*(void**)((s32)marioGetPtr() + 0x1E8) == 0) {
        return 0;
    }
    mario = marioGetPtr();
    motion = *(u16*)((s32)mario + 0x2E);
    if (motion <= 1) {
        ok = 1;
    } else if (motion == 2) {
        ok = 1;
    } else if (motion == 0x16 && *(s16*)((s32)mario + 0x50) == 0) {
        ok = 1;
    } else if (motion == 0x15 && *(s16*)((s32)mario + 0x50) == 0) {
        ok = 1;
    } else if (motion == 0x19 && *(s16*)((s32)mario + 0x50) == 0) {
        ok = 1;
    } else if (motion == 0x1A) {
        ok = 1;
    } else {
        ok = 0;
    }
    if (ok != 0) {
        return 2;
    }
    return 0;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_mario_wait_landon(void) {
    extern void* marioGetPtr(void);
    void* mario;
    u32 motion;
    s32 ok;

    if (*(void**)((s32)marioGetPtr() + 0x1E8) == 0) {
        return 0;
    }
    mario = marioGetPtr();
    motion = *(u16*)((s32)mario + 0x2E);
    if (motion <= 1) {
        ok = 1;
    } else if (motion == 2) {
        ok = 1;
    } else if (motion == 0x16 && *(s16*)((s32)mario + 0x50) == 0) {
        ok = 1;
    } else if (motion == 0x15 && *(s16*)((s32)mario + 0x50) == 0) {
        ok = 1;
    } else if (motion == 0x19 && *(s16*)((s32)mario + 0x50) == 0) {
        ok = 1;
    } else if (motion == 0x1A) {
        ok = 1;
    } else {
        ok = 0;
    }
    if (ok != 0) {
        return 2;
    }
    return 0;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_mario_get_pos(EventEntry* event) {
    extern void* marioGetPtr(void);
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern s32 evtSetFloat(EventEntry* event, s32 target, f32 value);
    extern const f32 float_1_80421ad8;
    EventEntry* evt = event;
    s32* args = event->args;
    void* mario = marioGetPtr();

    if (evtGetValue(evt, args[0]) == 0) {
        evtSetFloat(evt, args[1], *(f32*)((s32)mario + 0x8C));
        evtSetFloat(evt, args[2], *(f32*)((s32)mario + 0x90));
        evtSetFloat(evt, args[3], *(f32*)((s32)mario + 0x94));
    } else {
        evtSetFloat(evt, args[1], *(f32*)((s32)mario + 0x8C));
        evtSetFloat(evt, args[2], *(f32*)((s32)mario + 0x90) - float_1_80421ad8);
        evtSetFloat(evt, args[3], *(f32*)((s32)mario + 0x94));
    }
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_mario_paper_plane(EventEntry* event, s32 first) {
    extern void* marioGetPtr(void);
    extern void mario_plane_cancel(void);
    extern void marioChgMot(s32 mot);
    extern s32 marioGetPlaneStatus(void);
    s32* args = event->args;
    void* mario = marioGetPtr();
    s32 mode = args[0];

    if (first != 0) {
        if (mode == 0) {
            mario_plane_cancel();
        } else {
            marioChgMot(0x18);
            *(u32*)((s32)mario + 0xC) |= 4;
        }
    }
    if (mode == 0) {
        if (*(u16*)((s32)mario + 0x2E) == 0 && *(void**)((s32)mario + 0x1E8) != 0) {
            *(u32*)((s32)mario + 0xC) &= ~4;
            return 2;
        }
    } else if (marioGetPlaneStatus() == 2) {
        return 2;
    }
    return 0;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_mario_set_party(EventEntry* event, s32 first) {
    extern void* marioGetPtr(void);
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern s32 partyEntry(s32 party);
    extern s32 partyEntry2(s32 party);
    extern void* partyGetPtr(s32 party);
    EventEntry* evt = event;
    s32* args = event->args;
    void* mario = marioGetPtr();
    s32 offset = args[0];
    void* entry = (void*)((s32)mario + offset);
    s32 party;

    if (first != 0) {
        if (*(s8*)((s32)entry + 0x245) >= 0) {
            return 2;
        }
        party = evtGetValue(evt, args[1]);
        if (offset == 0) {
            party = partyEntry(party);
        } else {
            party = partyEntry2(party);
        }
        if (party < 0) {
            return 2;
        }
        *(s8*)((s32)entry + 0x245) = party;
    }
    partyGetPtr(*(s8*)((s32)mario + offset + 0x245));
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_mario_get_state(EventEntry* event) {
    extern void* marioGetPtr(void);
    extern s32 marioChkSlitThrouhEnd(void);
    extern s32 evtSetValue(EventEntry* event, s32 target, s32 value);
    EventEntry* evt = event;
    s32* args = event->args;
    s32 dst;
    void* mario;
    s32 value;

    mario = marioGetPtr();
    dst = args[0];
    if (marioChkSlitThrouhEnd() != 0) {
        evtSetValue(evt, dst, 2);
        return 2;
    }
    value = 1;
    if (*(u16*)((s32)mario + 0x2E) == 0) {
        value = 0;
    }
    evtSetValue(evt, dst, value);
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_mario_set_force_dir(EventEntry* event) {
    extern void* marioGetPtr(void);
    extern s32 evtGetValue(EventEntry* event, s32 value);
    EventEntry* evt = event;
    s32* args = event->args;
    void* mario = marioGetPtr();

    if (*(s16*)((s32)mario + 0x50) > 0) {
        *(u32*)mario |= 0x40;
        *(f32*)((s32)mario + 0x1A8) = (f32)evtGetValue(evt, args[0]);
    }
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_mario_trigflag_onoff(EventEntry* event) {
    extern void* marioGetPtr(void);
    extern s32 evtGetValue(EventEntry* event, s32 value);
    EventEntry* evt = event;
    s32* args = event->args;
    void* mario = marioGetPtr();

    if (evtGetValue(evt, args[0]) != 0) {
        *(u32*)((s32)mario + 0xC) |= evtGetValue(evt, args[1]);
    } else {
        *(u32*)((s32)mario + 0xC) &= ~evtGetValue(evt, args[1]);
    }
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_mario_dispflag_onoff(EventEntry* event) {
    extern void* marioGetPtr(void);
    extern s32 evtGetValue(EventEntry* event, s32 value);
    EventEntry* evt = event;
    s32* args = event->args;
    void* mario = marioGetPtr();

    if (evtGetValue(evt, args[0]) != 0) {
        *(u32*)((s32)mario + 4) |= evtGetValue(evt, args[1]);
    } else {
        *(u32*)((s32)mario + 4) &= ~evtGetValue(evt, args[1]);
    }
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_mario_flag_onoff(EventEntry* event) {
    extern void* marioGetPtr(void);
    extern s32 evtGetValue(EventEntry* event, s32 value);
    EventEntry* evt = event;
    s32* args = event->args;
    void* mario = marioGetPtr();

    if (evtGetValue(evt, args[0]) != 0) {
        *(u32*)mario |= evtGetValue(evt, args[1]);
    } else {
        *(u32*)mario &= ~evtGetValue(evt, args[1]);
    }
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_mario_goodbye_party(EventEntry* event, s32 first) {
    extern void* marioGetPtr(void);
    extern void partyGoodbye(s32 party);
    EventEntry* evt = event;
    s32* args;
    void* mario;
    s32 offset;
    s32 party;

    mario = marioGetPtr();
    args = evt->args;
    offset = args[0];
    if (first != 0) {
        party = *(s8*)((s32)mario + offset + 0x245);
        if (party < 0) {
            return 2;
        }
        partyGoodbye(party);
    }
    if (*(s8*)((s32)mario + offset + 0x245) >= 0) {
        return 0;
    }
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_peach_set_condition(EventEntry* event) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern void peachSetStsNormal(void);
    extern void peachSetStsAftershower(void);
    extern void peachSetStsInvisible(void);
    extern void peachSetStsHappy(void);
    extern void peachSetStsUneasy(void);
    extern void peachSetStsAngry(void);
    extern void peachSetStsOrdinary(void);
    extern void peachSetStsCarry(void);

    switch (evtGetValue(event, event->args[0])) {
        case 0:
            peachSetStsNormal();
            break;
        case 1:
            peachSetStsAftershower();
            break;
        case 2:
            peachSetStsInvisible();
            break;
        case 3:
            peachSetStsHappy();
            break;
        case 4:
            peachSetStsUneasy();
            break;
        case 5:
            peachSetStsAngry();
            break;
        case 6:
            peachSetStsOrdinary();
            break;
        case 7:
            peachSetStsCarry();
            break;
    }
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

s32 evt_mario_party_use_check(EventEntry* event) {
    extern s32 marioGetPartyId(void);
    extern void* partyGetPtr(s32 id);
    extern s32 evtSetValue(EventEntry* event, s32 target, s32 value);
    s32* args;
    s32 dst;
    s32 value;
    void* party;

    value = 0;
    args = event->args;
    dst = args[0];
    party = partyGetPtr(marioGetPartyId());

    if (party != NULL && (*(u32*)party & 0x100) != 0) {
        value = *(s8*)((s32)party + 0x31);
    }

    evtSetValue(event, dst, value);
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

s32 evt_mario_chk_hipbump(void* pEvt) {
    return 0;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

s32 evt_mario_get_mov_spd(EventEntry* event) {
    extern void* marioGetPtr(void);
    extern void* gp;
    extern s32 evtSetFloat(EventEntry* event, s32 target, f32 value);
    s32* args = event->args;
    s32 dst = args[0];
    f32 speed = *(f32*)((s32)marioGetPtr() + 0x180);

    evtSetFloat(event, dst, (f32)*(s32*)((s32)gp + 4) * speed);
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

s32 evt_mario_get_motion(EventEntry* event) {
    extern void* marioGetPtr(void);
    extern s32 evtSetValue(EventEntry* event, s32 target, s32 value);
    s32* args = event->args;
    void* mario;
    s32 flags;
    s32 dst;

    mario = marioGetPtr();
    flags = *(s32*)((s32)mario + 0xC);
    dst = args[0];
    if ((flags & 1) != 0) {
        evtSetValue(event, dst, *(u16*)((s32)marioGetPtr() + 0x30));
    } else {
        evtSetValue(event, dst, *(u16*)((s32)marioGetPtr() + 0x2E));
    }
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

s32 evt_mario_set_hosei_xyz(EventEntry* event) {
    extern void* marioGetPtr(void);
    extern f32 evtGetFloat(EventEntry* event, s32 value);
    void* mario = marioGetPtr();
    s32* args = event->args;

    *(f32*)((s32)mario + 0x174) = evtGetFloat(event, args[0]);
    *(f32*)((s32)mario + 0x178) = evtGetFloat(event, args[1]);
    *(f32*)((s32)mario + 0x17C) = evtGetFloat(event, args[2]);
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

s32 evt_mario_set_pos(EventEntry* event) {
    extern void* marioGetPtr(void);
    extern f32 evtGetFloat(EventEntry* event, s32 value);
    s32* args = event->args;
    void* mario = marioGetPtr();

    *(f32*)((s32)mario + 0x8C) = evtGetFloat(event, args[0]);
    *(f32*)((s32)mario + 0x90) = evtGetFloat(event, args[1]);
    *(f32*)((s32)mario + 0x94) = evtGetFloat(event, args[2]);
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

s32 evt_mario_check_key_item(EventEntry* event) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern s32 pouchCheckItem(s32 item);
    extern s32 evtSetValue(EventEntry* event, s32 target, s32 value);
    s32* args;
    s32 item;
    s32 dst;

    args = event->args;
    item = evtGetValue(event, args[0]);
    dst = args[1];
    if (pouchCheckItem(item) != 0) {
        evtSetValue(event, dst, 1);
    } else {
        evtSetValue(event, dst, 0);
    }
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

s32 evt_mario_get_name_hitobj_push(EventEntry* event) {
    extern void* marioGetPtr(void);
    extern s32 hitGetName(void* hit);
    extern s32 evtSetValue(EventEntry* event, s32 target, s32 value);
    EventEntry* evt = event;
    s32* args = event->args;
    s32 dst = args[0];
    s32 name = 0;
    void* hit;

    hit = *(void**)((s32)marioGetPtr() + 0x1F0);
    if (hit != NULL) {
        name = hitGetName(hit);
    }

    evtSetValue(evt, dst, name);
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

s32 evt_mario_get_name_hitobj_front(EventEntry* event) {
    extern void* marioGetPtr(void);
    extern s32 hitGetName(void* hit);
    extern s32 evtSetValue(EventEntry* event, s32 target, s32 value);
    EventEntry* evt = event;
    s32* args = event->args;
    s32 dst = args[0];
    s32 name = 0;
    void* hit;

    hit = *(void**)((s32)marioGetPtr() + 0x1E4);
    if (hit != NULL) {
        name = hitGetName(hit);
    }

    evtSetValue(evt, dst, name);
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

s32 evt_mario_get_name_hitobj_under(EventEntry* event) {
    extern void* marioGetPtr(void);
    extern s32 hitGetName(void* hit);
    extern s32 evtSetValue(EventEntry* event, s32 target, s32 value);
    EventEntry* evt = event;
    s32* args = event->args;
    s32 dst = args[0];
    s32 name = 0;
    void* hit;

    hit = *(void**)((s32)marioGetPtr() + 0x1FC);
    if (hit != NULL) {
        name = hitGetName(hit);
    }

    evtSetValue(evt, dst, name);
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

s32 evt_mario_get_name_hitobj_ride(EventEntry* event) {
    extern void* marioGetPtr(void);
    extern s32 hitGetName(void* hit);
    extern s32 evtSetValue(EventEntry* event, s32 target, s32 value);
    EventEntry* evt = event;
    s32* args = event->args;
    s32 dst = args[0];
    s32 name = 0;
    void* hit;

    hit = *(void**)((s32)marioGetPtr() + 0x1E8);
    if (hit != NULL) {
        name = hitGetName(hit);
    }

    evtSetValue(evt, dst, name);
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

s32 evt_mario_get_name_hitobj_head(EventEntry* event) {
    extern void* marioGetPtr(void);
    extern s32 hitGetName(void* hit);
    extern s32 evtSetValue(EventEntry* event, s32 target, s32 value);
    EventEntry* evt = event;
    s32* args = event->args;
    s32 dst = args[0];
    s32 name = 0;
    void* hit;

    hit = *(void**)((s32)marioGetPtr() + 0x1F8);
    if (hit != NULL) {
        name = hitGetName(hit);
    }

    evtSetValue(evt, dst, name);
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

s32 evt_mario_set_mov_spd(EventEntry* event) {
    extern void* gp;
    extern f32 evtGetFloat(EventEntry* event, s32 value);
    extern void* marioGetPtr(void);
    f32 speed;

    speed = evtGetFloat(event, event->args[0]) / (f32)*(s32*)((s32)gp + 4);
    *(f32*)((s32)marioGetPtr() + 0x180) = speed;
    return 2;
}


s32 evt_mario_clear_party(void* pEvt) {
    return 0;
}


s32 evt_mario_kill_party(void* pEvt) {
    return 0;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

s32 evt_mario_get_exparty(EventEntry* event) {
    extern s32 marioGetExtraPartyId(void);
    extern void* partyGetPtr(s32 partyId);
    extern s32 evtSetValue(EventEntry* event, s32 target, s32 value);
    EventEntry* evt = event;
    s32* args = event->args;
    s32 dst = args[0];
    void* party;
    s32 value;

    party = partyGetPtr(marioGetExtraPartyId());
    if (party != NULL) {
        value = *(s8*)((s32)party + 0x31);
    } else {
        value = 0;
    }
    evtSetValue(evt, dst, value);
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

s32 evt_mario_get_party(EventEntry* event) {
    extern s32 marioGetPartyId(void);
    extern void* partyGetPtr(s32 partyId);
    extern s32 evtSetValue(EventEntry* event, s32 target, s32 value);
    EventEntry* evt = event;
    s32* args = event->args;
    s32 dst = args[0];
    void* party;
    s32 value;

    party = partyGetPtr(marioGetPartyId());
    if (party != NULL) {
        value = *(s8*)((s32)party + 0x31);
    } else {
        value = 0;
    }
    evtSetValue(evt, dst, value);
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

s32 evt_mario_dokan_end(s32 param_1, int param_2) {
    return 0;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

s32 evt_mario_key_onoff(EventEntry* event) {
    extern void* marioGetPtr(void);
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern void marioKeyOff(void);
    extern void marioKeyOn(void);
    extern void partyKeyOff(void);
    extern void partyKeyOn(void);
    EventEntry* evt = event;
    s32* args = event->args;

    marioGetPtr();
    if (evtGetValue(evt, args[0]) == 0) {
        marioKeyOff();
        partyKeyOff();
    } else {
        marioKeyOn();
        partyKeyOn();
    }

    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

s32 evt_mario_sleep_off(EventEntry* event, s32 first) {
    extern void marioChgEvtPose(const char* pose);
    extern const char str_M_N_5B_802e3ea8[];

    if (first != 0) {
        marioChgEvtPose(str_M_N_5B_802e3ea8);
        *(s32*)((s32)event + 0x78) = 0x20;
    }

    if (--*(s32*)((s32)event + 0x78) > 0) {
        return 0;
    }
    return 2;
}

s32 evt_koopa_weary(void* pEvt) {
    return 0;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

s32 evt_mario_set_dispdir(EventEntry* event) {
    extern void* marioGetPtr(void);
    extern f32 evtGetFloat(EventEntry* event, s32 value);
    extern f32 reviseAngle(f32 angle);
    EventEntry* evt = event;
    s32* args = event->args;
    void* mario = marioGetPtr();
    s32 result;

    *(f32*)((s32)mario + 0x1B0) = reviseAngle(evtGetFloat(evt, args[0]));
    result = 2;
    *(f32*)((s32)mario + 0x1AC) = *(f32*)((s32)mario + 0x1B0);
    return result;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

s32 evt_mario_chk_join_party(void* pEvt) {
    return 0;
}


s32 N_evt_mario_party_door_halve_hitbox(void) {
    return 0;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

s32 evt_mario_get_mode(EventEntry* event) {
    extern void* marioGetPtr(void);
    extern s32 evtSetValue(EventEntry* event, s32 target, s32 value);
    EventEntry* evt = event;
    s32* args = event->args;
    s32 dst = args[0];
    s32 mode = *(u8*)((s32)marioGetPtr() + 0x3C);

    evtSetValue(evt, dst, (s8)mode);
    return 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

s32 evt_koopa_chk_dead(void* pEvt) {
    return 0;
}


s32 evt_mario_get_dispdir(EventEntry* event) {
    extern void* marioGetPtr(void);
    extern s32 evtSetFloat(EventEntry* event, s32 target, f32 value);
    EventEntry* evt = event;
    s32* args = event->args;
    s32 dst = args[0];
    void* mario = marioGetPtr();

    evtSetFloat(evt, dst, *(f32*)((s32)mario + 0x1AC));
    return 2;
}


s32 evt_mario_get_dir(EventEntry* event) {
    extern void* marioGetPtr(void);
    extern s32 evtSetFloat(EventEntry* event, s32 target, f32 value);
    EventEntry* evt = event;
    s32* args = event->args;
    s32 dst = args[0];
    void* mario = marioGetPtr();

    evtSetFloat(evt, dst, *(f32*)((s32)mario + 0x1A4));
    return 2;
}


s32 evt_koopa_get_level(EventEntry* event) {
    extern s32 kpaGetLevel(void);
    extern s32 evtSetValue(EventEntry* event, s32 target, s32 value);
    EventEntry* evt = event;
    s32* args = event->args;
    s32 level = kpaGetLevel();

    evtSetValue(evt, args[0], level);
    return 2;
}


s32 evt_mario_get_pose(EventEntry* event) {
    extern void* marioGetPtr(void);
    extern s32 evtSetValue(EventEntry* event, s32 target, s32 value);
    EventEntry* evt = event;
    s32* args = event->args;
    void* mario = marioGetPtr();

    evtSetValue(evt, args[0], *(s32*)((s32)mario + 0x18));
    return 2;
}


s32 evt_mario_cancel_roll_motion(s32 param_1, int param_2) {
    extern void unk_800a1454(s32);
    extern void* marioGetPtr(void);

    if (param_2 != 0) {
        unk_800a1454(param_1);
    }
    if ((*(u32*)marioGetPtr() & 0x01000000) != 0) {
        return 0;
    }
    return 2;
}

u32 L_evt_mario_keyon_wait(void) {
    extern void* marioGetPtr(void);
    extern s32 marioKeyOffChk(void*);
    void* mario = marioGetPtr();
    return marioKeyOffChk(mario) != 0 ? 0 : 2;
}


u32 evt_mario_wait_move_end(void) {
    extern void* marioGetPtr(void);
    s32 value = *(s16*)((s32)marioGetPtr() + 0x4E);

    return 2 & ~((s32)((u32)(-value) | (u32)value) >> 31);
}


s32 evt_koopa_set_level(EventEntry* event) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern void kpaSetLevel(s32 level);

    kpaSetLevel(evtGetValue(event, event->args[0]));
    return 2;
}

s32 evt_peach_transform_gundan_off(void) {
    extern void peachTransformOff(void);
    peachTransformOff();
    return 2;
}

s32 evt_peach_transform_gundan_on(void) {
    extern void peachTransformOn(void);
    peachTransformOn();
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 unk_800e6b1c(EventEntry* event) {
    extern void* marioGetPtr(void);
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern void* camGetPtr(s32 id);
    extern f32 angleABf(f32 x1, f32 z1, f32 x2, f32 z2);
    extern f32 revise360(f32 angle);
    extern const f32 float_180_80421b34;
    extern const f32 float_0_80421adc;
    EventEntry* evt = event;
    s32* args = event->args;
    void* mario;
    f32 x;
    f32 z;
    f32 angle;
    f32 bucket;

    mario = marioGetPtr();
    x = (f32)evtGetValue(evt, args[0]);
    z = (f32)evtGetValue(evt, args[1]);
    angle = revise360(angleABf(*(f32*)((s32)mario + 0x8C), *(f32*)((s32)mario + 0x94), x, z) - *(f32*)((s32)camGetPtr(4) + 0x114));
    bucket = float_180_80421b34;
    if (angle >= float_180_80421b34) {
        bucket = float_0_80421adc;
    }
    *(f32*)((s32)mario + 0x1B0) = bucket;
    *(f32*)((s32)mario + 0x1A4) = angle;
    *(f32*)((s32)mario + 0x1A0) = angle;
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
