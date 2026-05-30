#include "event/evt_sub.h"

s32 stop;

USER_FUNC(evt_sub_countdown_end) { extern void countDownEnd(void); countDownEnd(); return 2; }

USER_FUNC(N_evt_sub_status_gauge_force_update) { extern void statusWinForceUpdate(void); statusWinForceUpdate(); return 2; }

USER_FUNC(evt_sub_status_gauge_force_close) { extern void statusWinForceClose(void); statusWinForceClose(); return 2; }

USER_FUNC(evt_sub_status_gauge_force_open) { extern void statusWinForceOpen(void); statusWinForceOpen(); return 2; }

USER_FUNC(stone_bg_open) {
    extern void* effNameToPtr(const char* name);
    extern const char str_sub_bg_802c162c[];
    void* eff = effNameToPtr(str_sub_bg_802c162c);
    *(s32*)((s32)*(void**)((s32)eff + 0xC) + 0x20) = 1;
    return 2;
}

USER_FUNC(get_stop) {
    evtSetValue(event, event->args[0], stop);
    return 2;
}

USER_FUNC(set_stop) {
    stop = evtGetValue(event, event->args[0]);
    return 2;
}

USER_FUNC(evt_sub_get_areaname) {
    extern void* gp;
    evtSetValue(event, event->args[0], (s32)gp + 0x13C);
    return 2;
}

USER_FUNC(evt_sub_get_mapname) {
    extern void* gp;
    evtSetValue(event, event->args[0], (s32)gp + 0x12C);
    return 2;
}

USER_FUNC(evt_sub_get_language) {
    extern void* gp;
    evtSetValue(event, event->args[0], *(s32*)((s32)gp + 0x16C));
    return 2;
}

USER_FUNC(evt_sub_get_system_flag) {
    extern void* gp;
    evtSetValue(event, event->args[0], *(s32*)gp);
    return 2;
}

USER_FUNC(evt_sub_countdown_set_restart_time) {
    extern void countDownSetRestartTime(s32 time);
    countDownSetRestartTime(evtGetValue(event, event->args[0]));
    return 2;
}

USER_FUNC(bgm_start_wait) {
    extern s32 psndBGMStartCheck(s32);
    return psndBGMStartCheck(1) ? 2 : 0;
}

USER_FUNC(stageclear_wait) {
    extern void* effNameToPtr(const char* name);
    extern s32 effStageClearEndChk(void* eff);
    extern const char str_sub_clear_802c16ec[];
    return effStageClearEndChk(effNameToPtr(str_sub_clear_802c16ec)) ? 2 : 0;
}

USER_FUNC(evt_key_get_button) {
    extern s32 keyGetButton(s32);
    s32* args = event->args;
    evtSetValue(event, args[1], keyGetButton(evtGetValue(event, args[0])));
    return 2;
}

USER_FUNC(evt_key_get_buttontrg) {
    extern s32 keyGetButtonTrg(s32);
    s32* args = event->args;
    evtSetValue(event, args[1], keyGetButtonTrg(evtGetValue(event, args[0])));
    return 2;
}

USER_FUNC(evt_key_get_dir) {
    extern s32 keyGetDir(s32);
    s32* args = event->args;
    evtSetValue(event, args[1], keyGetDir(evtGetValue(event, args[0])));
    return 2;
}

USER_FUNC(evt_sub_countdown_start) {
    extern void countDownStart(s32 time, s32 flags);
    s32* args = event->args;
    s32 time = evtGetValue(event, args[0]);
    s32 flags = evtGetValue(event, args[1]);
    countDownStart(time, flags);
    return 2;
}

USER_FUNC(evt_sub_countdown_get_status) {
    extern s32 countDownGetStatus(void);
    s32* args = event->args;
    evtSetValue(event, args[0], countDownGetStatus());
    return 2;
}

USER_FUNC(evt_sub_system_flag_onoff) {
    extern void* gp;
    s32* args = event->args;
    s32 on = evtGetValue(event, args[0]);
    s32 flag = args[1];
    if (on) {
        *(u32*)gp |= flag;
    } else {
        *(u32*)gp &= ~flag;
    }
    return 2;
}

USER_FUNC(evt_sub_status_gauge_check_update) {
    extern s32 statusWinCheckUpdate(void);
    s32* args = event->args;
    if (statusWinCheckUpdate()) {
        evtSetValue(event, args[0], 1);
    } else {
        evtSetValue(event, args[0], 0);
    }
    return 2;
}

USER_FUNC(mail_evt_gor_04_keyoff_check) {
    extern s32 marioCtrlOffChk(void);
    extern s32 marioKeyOffChk(void);
    s32* args = event->args;
    s32 ctrl = marioCtrlOffChk();
    s32 key = marioKeyOffChk();
    evtSetValue(event, args[0], key + ctrl);
    return 2;
}

USER_FUNC(evt_sub_countdown_flag_onoff) {
    extern void countDownFlagOn(u16 flag);
    extern void countDownFlagOff(u16 flag);
    s32* args = event->args;
    s32 on = evtGetValue(event, args[0]);
    s32 flag = evtGetValue(event, args[1]);
    if (on) {
        countDownFlagOn(flag);
    } else {
        countDownFlagOff(flag);
    }
    return 2;
}


s32 evt_sub_get_coin(void* evt, int param_2) {
    return 0;
}


s32 irai_mail_check(void* pEvt) {
    return 0;
}


u8 evt_sub_check_intersect(s32 pEvt) {
    return 0;
}


u8 evt_sub_spline_get_value(void) {
    return 0;
}


u8 evt_sub_area_check(s32 pEvt) {
    return 0;
}


s32 evt_sub_spline_get_value_manual(void* pEvt) {
    return 0;
}


s32 evt_sub_load_progresstime(void* pEvt) {
    return 0;
}


u8 evt_sub_get_dir(void) {
    return 0;
}


u8 evt_sub_get_dist(s32 pEvt) {
    return 0;
}


u8 coingetDisp(int param_1) {
    return 0;
}


s32 evt_sub_intpl_msec_get_value_para(int param_1) {
    return 0;
}


s32 evt_sub_spline_init(u32* param_1) {
    return 0;
}


s32 evt_sub_intpl_get_float(int param_1) {
    return 0;
}


u8 evt_sub_intpl_init_float(void) {
    return 0;
}


s32 evt_sub_intpl_msec_get_value(void* pEvt) {
    return 0;
}


s32 stone_bg(int param_1) {
    return 0;
}


s32 evt_sub_intpl_get_value_para(int param_1) {
    return 0;
}


u8 evt_sub_get_sincos(void) {
    return 0;
}


s32 stone_ry(int param_1) {
    return 0;
}


s32 evt_sub_save_playtime(void* pEvt) {
    return 0;
}


s32 evt_sub_intpl_get_value(void* param_1) {
    return 0;
}


s32 evt_sub_intpl_msec_init(void* pEvt) {
    return 0;
}


s32 evt_sub_intpl_init(void* param_1) {
    return 0;
}


s32 evt_sub_get_stopwatch(void* pEvt) {
    return 0;
}


s32 evt_sub_rumble_onoff(int param_1) {
    return 0;
}


s32 evt_sub_random(void* param_1) {
    return 0;
}


s32 evt_sub_animgroup_async(int param_1) {
    return 0;
}


s32 evt_sub_spline_free(int param_1) {
    return 0;
}
