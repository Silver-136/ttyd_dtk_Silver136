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
    s32 done = psndBGMStartCheck(1);
    done = (-done | done) >> 31;
    return 2 & done;
}

USER_FUNC(stageclear_wait) {
    extern void* effNameToPtr(const char* name);
    extern s32 effStageClearEndChk(void* eff);
    extern const char str_sub_clear_802c16ec[];
    s32 done = effStageClearEndChk(effNameToPtr(str_sub_clear_802c16ec));
    done = (-done | done) >> 31;
    return 2 & done;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(evt_key_get_button) {
    extern s32 keyGetButton(s32);
    s32* args = event->args;
    evtSetValue(event, args[1], keyGetButton(evtGetValue(event, args[0])));
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(evt_key_get_buttontrg) {
    extern s32 keyGetButtonTrg(s32);
    s32* args = event->args;
    evtSetValue(event, args[1], keyGetButtonTrg(evtGetValue(event, args[0])));
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(evt_key_get_dir) {
    extern s32 keyGetDir(s32);
    s32* args = event->args;
    evtSetValue(event, args[1], keyGetDir(evtGetValue(event, args[0])));
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(evt_sub_countdown_start) {
    extern void countDownStart(s32 time, s32 flags);
    s32* args = event->args;
    s32 time = evtGetValue(event, args[0]);
    s32 flags = evtGetValue(event, args[1]);
    countDownStart(time, flags);
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(evt_sub_countdown_get_status) {
    extern s32 countDownGetStatus(void);
    s32* args = event->args;
    evtSetValue(event, args[0], countDownGetStatus());
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

USER_FUNC(evt_sub_system_flag_onoff) {
    extern void* gp;
    s32* args = event->args;
    s32 on = evtGetValue(event, args[0]);
    u32 flag = args[1];
    if (on) {
        *(u32*)gp |= flag;
    } else {
        *(u32*)gp &= ~flag;
    }
    return 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
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

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(mail_evt_gor_04_keyoff_check) {
    extern s32 marioCtrlOffChk(void);
    extern s32 marioKeyOffChk(void);
    s32* args = event->args;
    s32 ctrl = marioCtrlOffChk();
    s32 key = marioKeyOffChk();
    evtSetValue(event, args[0], key + ctrl);
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
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

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


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


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_sub_load_progresstime(void* event) {
    extern void* gp;
    extern s32 swByteGet(s32 id);
    extern void evtSetValue(void* event, s32 id, s32 value);
    s32* args = *(s32**)((s32)event + 0x18);
    s32 base = args[0];
    u32 time[2];
    u32 bus;
    u32 ticks;
    s32 value;
    time[0] = (swByteGet(base + 0xA220000 - 0x180) << 24) |
              (swByteGet(base + 0xA220000 - 0x17F) << 16) |
              (swByteGet(base + 0xA220000 - 0x17E) << 8) |
              swByteGet(base + 0xA220000 - 0x17D);
    time[1] = (swByteGet(base + 0xA220000 - 0x17C) << 24) |
              (swByteGet(base + 0xA220000 - 0x17B) << 16) |
              (swByteGet(base + 0xA220000 - 0x17A) << 8) |
              swByteGet(base + 0xA220000 - 0x179);
    bus = *(u32*)0x800000F8 >> 2;
    ticks = ((u64)bus * 0x10624DD3U) >> 38;
    value = (s32)((*(s64*)((s32)gp + 0x20) - *(s64*)time) / ticks);
    if (value > 86400000) {
        value = 86400000;
    }
    evtSetValue(event, args[1], value);
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_sub_get_dir(void* event) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void evtSetFloat(void* event, s32 id, f32 value);
    extern f32 angleABf(f32 ax, f32 ay, f32 bx, f32 by);
    s32* args = *(s32**)((s32)event + 0x18);
    f32 ax = (f32)evtGetValue(event, args[0]);
    f32 ay = (f32)evtGetValue(event, args[1]);
    f32 bx = (f32)evtGetValue(event, args[2]);
    f32 by = (f32)evtGetValue(event, args[3]);
    evtSetFloat(event, args[4], angleABf(ax, ay, bx, by));
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
s32 evt_sub_get_dist(void* event) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void evtSetFloat(void* event, s32 id, f32 value);
    extern f32 distABf(f32 ax, f32 ay, f32 bx, f32 by);
    s32* args = *(s32**)((s32)event + 0x18);
    f32 ax = (f32)evtGetValue(event, args[0]);
    f32 ay = (f32)evtGetValue(event, args[1]);
    f32 bx = (f32)evtGetValue(event, args[2]);
    f32 by = (f32)evtGetValue(event, args[3]);
    evtSetFloat(event, args[4], distABf(ax, ay, bx, by));
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

u8 coingetDisp(int param_1) {
    return 0;
}


s32 evt_sub_intpl_msec_get_value_para(int param_1) {
    return 0;
}


s32 evt_sub_spline_init(u32* param_1) {
    return 0;
}


#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_sub_intpl_get_float(void* event) {
    extern f32 evtGetFloat(void* event, s32 value);
    extern void evtSetFloat(void* event, s32 id, f32 value);
    extern void evtSetValue(void* event, s32 id, s32 value);
    extern f32 intplGetValue(s32 type, f32 start, f32 end, s32 time, s32 duration);
    s32* args = *(s32**)((s32)event + 0x18);
    s32 out = args[0];
    s32 flag = args[1];
    f32 start = evtGetFloat(event, 0xFE363C8C);
    f32 end = evtGetFloat(event, 0xFE363C8D);
    f32 value = intplGetValue(*(s32*)((s32)event + 0xC8), start, end, *(s32*)((s32)event + 0xD4), *(s32*)((s32)event + 0xD8));
    if (out != -0xEE6B280) {
        evtSetFloat(event, out, value);
    }
    if (flag != -0xEE6B280) {
        if (*(s32*)((s32)event + 0xD4) >= *(s32*)((s32)event + 0xD8)) {
            evtSetValue(event, flag, 0);
        } else {
            evtSetValue(event, flag, 1);
        }
    }
    *(s32*)((s32)event + 0xD4) = *(s32*)((s32)event + 0xD4) + 1;
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_sub_intpl_init_float(void* event) {
    extern s32 evtGetValue(void* event, s32 value);
    extern f32 evtGetFloat(void* event, s32 value);
    s32* args = *(s32**)((s32)event + 0x18);
    s32 type = evtGetValue(event, args[0]);
    f32 start = evtGetFloat(event, args[1]);
    f32 end = evtGetFloat(event, args[2]);
    s32 duration = evtGetValue(event, args[3]);
    *(s32*)((s32)event + 0xC8) = type;
    *(s32*)((s32)event + 0xCC) = (s32)(start * 1024.0f) + 600000;
    *(s32*)((s32)event + 0xD0) = (s32)(end * 1024.0f) + 600000;
    *(s32*)((s32)event + 0xD4) = 0;
    *(s32*)((s32)event + 0xD8) = duration;
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
s32 evt_sub_intpl_msec_get_value(void* event) {
    extern f32 intplGetValue(s32 type, f32 start, f32 end, s32 time, s32 duration);
    u32 bus = *(u32*)0x800000F8 >> 2;
    u32 ticks = ((u64)bus * 0x10624DD3U) >> 38;
    u64 elapsed = *(u64*)event - *(u64*)((s32)event + 0x198);
    u32 msec = elapsed / ticks;
    if ((s32)msec < *(s32*)((s32)event + 0xD8)) {
        s32 start = *(s32*)((s32)event + 0xCC);
        s32 end = *(s32*)((s32)event + 0xD0);
        *(s32*)((s32)event + 0x9C) = (s32)intplGetValue(*(s32*)((s32)event + 0xC8), (f32)start, (f32)end, msec, *(s32*)((s32)event + 0xD8));
        *(s32*)((s32)event + 0xA0) = 1;
    } else {
        *(s32*)((s32)event + 0x9C) = *(s32*)((s32)event + 0xD0);
        *(s32*)((s32)event + 0xA0) = 0;
    }
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

s32 stone_bg(int param_1) {
    return 0;
}


#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_sub_intpl_get_value_para(void* event) {
    extern void evtSetFloat(void* event, s32 id, f32 value);
    extern void evtSetValue(void* event, s32 id, s32 value);
    extern f32 intplGetValue(s32 type, f32 start, f32 end, s32 time, s32 duration);
    s32 start = *(s32*)((s32)event + 0xCC);
    s32 end = *(s32*)((s32)event + 0xD0);
    s32* args = *(s32**)((s32)event + 0x18);
    f32 value = intplGetValue(
        *(s32*)((s32)event + 0xC8),
        (f32)start,
        (f32)end,
        *(s32*)((s32)event + 0xD4),
        *(s32*)((s32)event + 0xD8));
    evtSetFloat(event, args[0], value);
    if (*(s32*)((s32)event + 0xD4) >= *(s32*)((s32)event + 0xD8)) {
        evtSetValue(event, args[1], 0);
    } else {
        evtSetValue(event, args[1], 1);
    }
    *(s32*)((s32)event + 0xD4) = *(s32*)((s32)event + 0xD4) + 1;
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_sub_get_sincos(void* event) {
    extern f32 evtGetFloat(void* event, s32 value);
    extern void evtSetValue(void* event, s32 id, s32 value);
    extern f64 sin(f64);
    extern f64 cos(f64);
    extern const f32 float_3p1416_8041fe7c;
    extern const f32 float_180_8041fe80;
    extern const f32 float_1024_8041fe84;
    s32* args = *(s32**)((s32)event + 0x18);
    f32 rad = (float_3p1416_8041fe7c * evtGetFloat(event, args[0])) / float_180_8041fe80;
    evtSetValue(event, args[1], (s32)((f32)sin(rad) * float_1024_8041fe84) - 230000000);
    evtSetValue(event, args[2], (s32)((f32)cos(rad) * float_1024_8041fe84) - 230000000);
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 stone_ry(void* event) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void* itemNameToPtr(s32 name);
    s32* args = *(s32**)((s32)event + 0x18);
    s32 item = evtGetValue(event, args[0]);
    s32 angle = evtGetValue(event, args[1]);
    void* itemp = itemNameToPtr(item);
    void* a = *(void**)((s32)itemp + 0x1C);
    void* b = *(void**)((s32)a + 0xC);
    *(u16*)((s32)b + 4) &= 0xFFFD;
    b = *(void**)((s32)a + 0xC);
    *(f32*)((s32)b + 0x1C) = (f32)(angle % 360);
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

s32 evt_sub_save_playtime(void* event) {
    extern void* gp;
    extern void swByteSet(s32 id, s32 value);
    s32* args = *(s32**)((s32)event + 0x18);
    s32 base = args[0];
    u32 time[2];
    time[0] = *(u32*)((s32)gp + 0x20);
    time[1] = *(u32*)((s32)gp + 0x24);
    swByteSet(base + 0xA220000 - 0x180, ((u8*)time)[0]);
    swByteSet(base + 0xA220000 - 0x17F, ((u8*)time)[1]);
    swByteSet(base + 0xA220000 - 0x17E, ((u8*)time)[2]);
    swByteSet(base + 0xA220000 - 0x17D, ((u8*)time)[3]);
    swByteSet(base + 0xA220000 - 0x17C, ((u8*)time)[4]);
    swByteSet(base + 0xA220000 - 0x17B, ((u8*)time)[5]);
    swByteSet(base + 0xA220000 - 0x17A, ((u8*)time)[6]);
    swByteSet(base + 0xA220000 - 0x179, ((u8*)time)[7]);
    return 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_sub_intpl_get_value(void* event) {
    extern f32 intplGetValue(s32 type, f32 start, f32 end, s32 time, s32 duration);
    s32 start = *(s32*)((s32)event + 0xCC);
    s32 end = *(s32*)((s32)event + 0xD0);
    *(s32*)((s32)event + 0x9C) = (s32)intplGetValue(
        *(s32*)((s32)event + 0xC8),
        (f32)start,
        (f32)end,
        *(s32*)((s32)event + 0xD4),
        *(s32*)((s32)event + 0xD8));
    if (*(s32*)((s32)event + 0xD4) >= *(s32*)((s32)event + 0xD8)) {
        *(s32*)((s32)event + 0xA0) = 0;
    } else {
        *(s32*)((s32)event + 0xA0) = 1;
    }
    *(s32*)((s32)event + 0xD4) = *(s32*)((s32)event + 0xD4) + 1;
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_sub_intpl_msec_init(void* event) {
    extern s32 evtGetValue(void* event, s32 value);
    s32* args = *(s32**)((s32)event + 0x18);
    s32 a = evtGetValue(event, args[0]);
    s32 b = evtGetValue(event, args[1]);
    s32 c = evtGetValue(event, args[2]);
    s32 d = evtGetValue(event, args[3]);
    s32 t1;
    s32 t0;
    *(s32*)((s32)event + 0xC8) = a;
    *(s32*)((s32)event + 0xCC) = b;
    *(s32*)((s32)event + 0xD0) = c;
    *(s32*)((s32)event + 0xD8) = d;
    t0 = *(s32*)event;
    t1 = *(s32*)((s32)event + 4);
    *(s32*)((s32)event + 0x19C) = t1;
    *(s32*)((s32)event + 0x198) = t0;
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_sub_intpl_init(void* event) {
    extern s32 evtGetValue(void* event, s32 value);
    s32* args = *(s32**)((s32)event + 0x18);
    s32 a = evtGetValue(event, args[0]);
    s32 b = evtGetValue(event, args[1]);
    s32 c = evtGetValue(event, args[2]);
    s32 d = evtGetValue(event, args[3]);
    *(s32*)((s32)event + 0xC8) = a;
    *(s32*)((s32)event + 0xCC) = b;
    *(s32*)((s32)event + 0xD0) = c;
    *(s32*)((s32)event + 0xD4) = 0;
    *(s32*)((s32)event + 0xD8) = d;
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_sub_get_stopwatch(void* event) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void* evtGetPtrID(s32 id);
    extern void evtSetValue(void* event, s32 id, s32 value);
    s32* args = *(s32**)((s32)event + 0x18);
    s64* watch = evtGetPtrID(evtGetValue(event, args[0]));
    u32 bus = *(u32*)0x800000F8 >> 2;
    u32 ticks = ((u64)bus * 0x10624DD3U) >> 38;
    s32 value = (s32)(*watch / ticks);
    if (value > 600000) {
        value = 600000;
    }
    evtSetValue(event, args[1], value);
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_sub_rumble_onoff(void* pEvt) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void padRumbleOn(s32 type);
    extern void padRumbleOff(s32 type);
    extern void padRumbleHardOff(s32 type);
    s32* args = *(s32**)((s32)pEvt + 0x18);
    s32 mode = evtGetValue(pEvt, args[0]);
    s32 type = evtGetValue(pEvt, args[1]);

    switch (mode) {
        case 0:
            padRumbleOn(type);
            break;
        case 1:
            padRumbleOff(type);
            break;
        case 2:
            padRumbleHardOff(type);
            break;
    }
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_sub_random(void* param_1) {
    extern s32 evtGetValue(void* event, s32 value);
    extern s32 evtSetValue(void* event, s32 index, s32 value);
    extern s32 rand(void);
    s32* args;
    s32 max;
    s32 value;

    args = *(s32**)((s32)param_1 + 0x18);
    max = evtGetValue(param_1, args[0]);
    value = rand() % (max + 1);
    evtSetValue(param_1, args[1], value);
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

s32 evt_sub_animgroup_async(int param_1) {
    extern s32 evtGetValue(void* event, s32 value);
    extern s32 animGroupBaseAsync(s32 name, s32 mode, s32 flags);
    extern s32 gp;
    s32 name = evtGetValue((void*)param_1, **(s32**)(param_1 + 0x18));
    s32 mode = (*(s32*)(gp + 0x14) != 0);

    if (animGroupBaseAsync(name, mode, 0) == 0) {
        return 0;
    }
    return 2;
}


s32 evt_sub_spline_free(int param_1) {
    extern void _mapFree(void* heap, void* ptr);
    extern void* mapalloc_base_ptr;
    void* spline = *(void**)(param_1 + 0xD8);

    _mapFree(mapalloc_base_ptr, *(void**)((s32)spline + 4));
    _mapFree(mapalloc_base_ptr, *(void**)((s32)spline + 0xC));
    _mapFree(mapalloc_base_ptr, spline);
    return 2;
}
