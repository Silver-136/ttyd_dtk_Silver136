#include "event/evt_fade.h"

extern s32 evtGetValue();
extern f32 evtGetFloat();
extern void fadeReset();
extern void fadeSoftFocusOn();
extern void fadeSoftFocusOff();
extern s32 fadeIsFinish();
extern void fadeSetAnimOfsPos(f32, f32);
extern void fadeTecSoftOn(void);
extern void fadeTecOn(void);
extern void fadeTecSoftOff(void);
extern void fadeTecOff(void);
extern void fadeSetAnimVirtualPos(f32, f32, f32);
extern void fadeEntry(s32 type, s32 duration, void* color);
extern void* gp;
extern u32 dat_80422a88;

s32 evt_fade_set_spot_pos(int param_1) {
    return 0;
}


s32 evt_fade_set_mapchange_type(int param_1) {
    s32* args;
    s32 type;
    s32 a;
    s32 b;
    s32 c;
    s32 d;

    args = *(s32**)(param_1 + 0x18);
    type = evtGetValue(param_1, args[0]);
    a = evtGetValue(param_1, args[1]);
    b = evtGetValue(param_1, args[2]);
    c = evtGetValue(param_1, args[3]);
    d = evtGetValue(param_1, args[4]);

    if (type == 0) {
        if (a != -1) {
            *(s32*)((s32)gp + 0x100) = a;
        }
        if (b != -1) {
            *(s32*)((s32)gp + 0x104) = b;
        }
        if (c != -1) {
            *(s32*)((s32)gp + 0xF8) = c;
        }
        if (d != -1) {
            *(s32*)((s32)gp + 0xFC) = d;
        }
    } else {
        if (a != -1) {
            *(s32*)((s32)gp + 0x110) = a;
        }
        if (b != -1) {
            *(s32*)((s32)gp + 0x114) = b;
        }
        if (c != -1) {
            *(s32*)((s32)gp + 0x108) = c;
        }
        if (d != -1) {
            *(s32*)((s32)gp + 0x10C) = d;
        }
    }

    return 2;
}


s32 evt_fade_entry(void* pEvt) {
    s32* args;
    s32 type;
    s32 duration;
    u32 color;
    u32 colorTemp;
    u8 r;
    u8 g;
    u8 b;

    args = *(s32**)((s32)pEvt + 0x18);
    type = evtGetValue(pEvt, args[0]);
    duration = evtGetValue(pEvt, args[1]);
    r = evtGetValue(pEvt, args[2]);
    g = evtGetValue(pEvt, args[3]);
    b = evtGetValue(pEvt, args[4]);

    colorTemp = dat_80422a88;
    ((u8*)&colorTemp)[2] = b;
    ((u8*)&colorTemp)[0] = r;
    ((u8*)&colorTemp)[1] = g;
    color = colorTemp;

    fadeEntry(type, duration, &color);
    return 2;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_fade_tec_onoff(int param_1) {
    s32* args;
    s32 onoff;
    s32 soft;

    args = *(s32**)(param_1 + 0x18);
    onoff = evtGetValue(param_1, args[0]);
    soft = evtGetValue(param_1, args[1]);

    if (onoff != 0) {
        if (soft != 0) {
            fadeTecSoftOn();
        } else {
            fadeTecOn();
        }
    } else {
        if (soft != 0) {
            fadeTecSoftOff();
        } else {
            fadeTecOff();
        }
    }

    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_fade_set_anim_virtual_pos(int param_1) {
    s32* args;
    f32 x;
    f32 y;
    f32 z;

    args = *(s32**)(param_1 + 0x18);
    x = evtGetFloat(param_1, args[0]);
    y = evtGetFloat(param_1, args[1]);
    z = evtGetFloat(param_1, args[2]);
    fadeSetAnimVirtualPos(x, y, z);

    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


s32 evt_fade_out(void* pEvt, int param_2) {
    return 0;
}


s32 evt_fade_in(void* pEvt, int param_2) {
    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_fade_set_anim_ofs_pos(int param_1) {
    void* args = *(void**)((s32)param_1 + 0x18);
    f32 x = evtGetFloat(param_1, *(s32*)args);
    f32 y = evtGetFloat(param_1, *(s32*)((s32)args + 4));

    fadeSetAnimOfsPos(x, y);
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


s32 evt_fade_softfocus_onoff(int param_1) {
    if (evtGetValue(param_1, **(s32**)((s32)param_1 + 0x18)) != 0) {
        fadeSoftFocusOn();
    } else {
        fadeSoftFocusOff();
    }
    return 2;
}


u32 evt_fade_end_wait(void) {
    s32 done = fadeIsFinish();
    return 2 & ~((done - 1 | 1 - done) >> 31);
}


s32 evt_fade_reset(void* param_1) {
    fadeReset(evtGetValue(param_1, **(s32**)((s32)param_1 + 0x18)));
    return 2;
}
