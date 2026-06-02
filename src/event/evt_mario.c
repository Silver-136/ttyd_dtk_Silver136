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


u8 evt_mario_set_dir(s32 pEvt, s32 param_2) {
    return 0;
}


u8 evt_mario_mov_pos2(s32 pEvt, s32 param_2) {
    return 0;
}


s32 evt_mario_dokan_prepare(int param_1) {
    return 0;
}


s32 evt_mario_set_normal_pose(void) {
    return 0;
}


u8 evt_mario_mov_pos(s32 pEvt, s32 param_2) {
    return 0;
}


u8 evt_mario_set_prev_party_pos(void) {
    return 0;
}


u8 evt_mario_set_party_pos(s32 pEvt, s32 param_2) {
    return 0;
}


s32 evt_mario_set_pose(int param_1) {
    return 0;
}


s32 evt_mario_hello_exparty_pos(int param_1, int param_2) {
    return 0;
}


s32 evt_mario_check_landon(void* pEvt) {
    return 0;
}


s32 evt_mario_hello_party(void* pEvt, int param_2) {
    return 0;
}


s32 evt_koopa_fire(void* pEvt, int param_2) {
    return 0;
}


s32 evt_mario_set_prev_party_dokan(void* pEvt, int param_2) {
    return 0;
}


u8 evt_mario_set_dir_npc(void) {
    return 0;
}


s32 evt_mario_set_prev_party(void* pEvt, int param_2) {
    return 0;
}


s32 evt_mario_wait_rideon(void) {
    return 0;
}


s32 evt_mario_wait_landon(void) {
    return 0;
}


s32 evt_mario_get_pos(void* pEvt) {
    return 0;
}


s32 evt_mario_paper_plane(void* pEvt, int param_2) {
    return 0;
}


s32 evt_mario_set_party(void* pEvt, int param_2) {
    return 0;
}


s32 evt_mario_get_state(void* pEvt) {
    return 0;
}


s32 evt_mario_set_force_dir(void* pEvt) {
    return 0;
}


s32 evt_mario_trigflag_onoff(void* pEvt) {
    return 0;
}


s32 evt_mario_dispflag_onoff(void* pEvt) {
    return 0;
}


s32 evt_mario_flag_onoff(void* pEvt) {
    return 0;
}


s32 evt_mario_goodbye_party(void* pEvt, int param_2) {
    return 0;
}


s32 evt_peach_set_condition(void* pEvt) {
    return 0;
}


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


s32 evt_mario_chk_hipbump(void* pEvt) {
    return 0;
}


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


s32 evt_mario_dokan_end(s32 param_1, int param_2) {
    return 0;
}


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


s32 evt_mario_chk_join_party(void* pEvt) {
    return 0;
}


s32 N_evt_mario_party_door_halve_hitbox(void) {
    return 0;
}


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
