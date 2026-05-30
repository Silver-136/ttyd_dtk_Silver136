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


s32 evt_mario_party_use_check(void* pEvt) {
    return 0;
}


s32 evt_mario_chk_hipbump(void* pEvt) {
    return 0;
}


s32 evt_mario_get_mov_spd(void* pEvt) {
    return 0;
}


s32 evt_mario_get_motion(void* pEvt) {
    return 0;
}


s32 evt_mario_set_hosei_xyz(void* pEvt) {
    return 0;
}


s32 evt_mario_set_pos(int param_1) {
    return 0;
}


s32 evt_mario_check_key_item(void* pEvt) {
    return 0;
}


s32 evt_mario_get_name_hitobj_push(void* pEvt) {
    return 0;
}


s32 evt_mario_get_name_hitobj_front(void* pEvt) {
    return 0;
}


s32 evt_mario_get_name_hitobj_under(void* pEvt) {
    return 0;
}


s32 evt_mario_get_name_hitobj_ride(void* pEvt) {
    return 0;
}


s32 evt_mario_get_name_hitobj_head(void* pEvt) {
    return 0;
}


u8 evt_mario_set_mov_spd(s32 pEvt) {
    return 0;
}


s32 evt_mario_clear_party(void* pEvt) {
    return 0;
}


s32 evt_mario_kill_party(void* pEvt) {
    return 0;
}


s32 evt_mario_get_exparty(void* pEvt) {
    return 0;
}


s32 evt_mario_get_party(void* pEvt) {
    return 0;
}


s32 evt_mario_dokan_end(s32 param_1, int param_2) {
    return 0;
}


s32 evt_mario_key_onoff(void* pEvt) {
    return 0;
}


s32 evt_mario_sleep_off(void* pEvt, int param_2) {
    return 0;
}


s32 evt_koopa_weary(void* pEvt) {
    return 0;
}


s32 evt_mario_set_dispdir(int param_1) {
    return 0;
}


s32 evt_mario_chk_join_party(void* pEvt) {
    return 0;
}


s32 N_evt_mario_party_door_halve_hitbox(void) {
    return 0;
}


s32 evt_mario_get_mode(void* pEvt) {
    return 0;
}


s32 evt_koopa_chk_dead(void* pEvt) {
    return 0;
}


s32 evt_mario_get_dispdir(void* pEvt) {
    return 0;
}


s32 evt_mario_get_dir(void* pEvt) {
    return 0;
}


s32 evt_koopa_get_level(void* pEvt) {
    return 0;
}


s32 evt_mario_get_pose(int param_1) {
    return 0;
}


s32 evt_mario_cancel_roll_motion(s32 param_1, int param_2) {
    return 0;
}


u32 L_evt_mario_keyon_wait(void) {
    return 0;
}


u32 evt_mario_wait_move_end(void) {
    return 0;
}


s32 evt_koopa_set_level(void* pEvt) {
    return 0;
}


s32 evt_peach_transform_gundan_off(void) {
    return 0;
}


s32 evt_peach_transform_gundan_on(void) {
    return 0;
}
