#include "event/evt_yuugijou.h"

void* yuwp;
void* unk_8041ed24;
s32 evtSetValue(EventEntry* evt, s32 target, s32 value);
s32 evtGetValue(EventEntry* evt, s32 value);
void evtEntryType(void* script, s32 a, s32 b, s32 type);
extern s32 minigame_end[];
extern s32 window_open[];

void unk_802295a8(void* value) {
    unk_8041ed24 = value;
}

void evtYuuWindow(void) {
    void* script = window_open;

    *(u32*)yuwp &= ~8;
    evtEntryType(script, 0, 0, 0);
}

u32 evtYuuWindowEndChk(void) {
    return (*(u32*)yuwp >> 3) & 1;
}

USER_FUNC(unk_802295b0) {
    s32* args = event->args;
    evtSetValue(event, args[0], (s32)unk_8041ed24);
    return 2;
}

USER_FUNC(yuuminigame_end) {
    evtEntryType(minigame_end, 0, 0, 0xEF);
    return 2;
}

USER_FUNC(yuuminigame_window_end) {
    *(u32*)yuwp |= 8;
    return 2;
}

USER_FUNC(yuugijou_set_yupflag) {
    s32 value = evtGetValue(event, event->args[0]);
    if (value != 0) {
        *(u32*)yuwp |= 4;
    } else {
        *(u32*)yuwp &= ~4;
    }
    return 2;
}

USER_FUNC(yuugijou_get_takeoutflag) {
    s32* args = event->args;
    if (*(u32*)yuwp & 2) {
        evtSetValue(event, args[0], 1);
    } else {
        evtSetValue(event, args[0], 0);
    }
    return 2;
}

USER_FUNC(yuugijou_montemedal_return) {
    if (!(*(u32*)yuwp & 2)) {
        s32 value = *(s32*)((s32)yuwp + 4);
        *(s32*)((s32)yuwp + 8) = value;
        *(s32*)((s32)yuwp + 0x10) = value;
        *(u32*)yuwp |= 2;
        *(u32*)yuwp |= 1;
    }
    return 2;
}

USER_FUNC(yuugijou_montemedal_keep) {
    if (*(u32*)yuwp & 2) {
        *(s32*)((s32)yuwp + 4) = *(s32*)((s32)yuwp + 8);
        *(s32*)((s32)yuwp + 8) = 0;
        *(s32*)((s32)yuwp + 0x10) = 0;
        *(u32*)yuwp &= ~2;
        *(u32*)yuwp &= ~1;
    }
    return 2;
}

USER_FUNC(yuugijou_montemedal_wait) {
    if (*(s32*)((s32)yuwp + 0x10) != *(s32*)((s32)yuwp + 8)) {
        return 0;
    }
    return 2;
}

USER_FUNC(yuugijou_montemedal_disp_onoff) {
    s32 value = evtGetValue(event, event->args[0]);
    if (value != 0) {
        *(u32*)yuwp |= 1;
        *(s32*)((s32)yuwp + 0x10) = *(s32*)((s32)yuwp + 8);
    } else {
        *(u32*)yuwp &= ~1;
    }
    return 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(yuugijou_add_montemedal_moto) {
    s32* args = event->args;
    evtGetValue(event, args[0]);
    if (*(u32*)yuwp & 2) {
        *(s32*)((s32)yuwp + 4) += evtGetValue(event, args[1]);
    }
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

USER_FUNC(yuugijou_get_montemedal_keep) {
    s32* args = event->args;
    if (*(u32*)yuwp & 2) {
        evtSetValue(event, args[0], 0);
    } else {
        evtSetValue(event, args[0], *(s32*)((s32)yuwp + 4));
    }
    return 2;
}

USER_FUNC(yuugijou_get_montemedal) {
    evtSetValue(event, event->args[0], *(s32*)((s32)yuwp + 8));
    return 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(yuugijou_add_montemedal) {
    s32* args = event->args;
    s32 update = evtGetValue(event, args[0]);
    s32 add = evtGetValue(event, args[1]);
    s32 value = *(s32*)((s32)yuwp + 8) + add;

    if (value < 99999) {
        *(s32*)((s32)yuwp + 8) = value;
    } else {
        *(s32*)((s32)yuwp + 8) = 99999;
    }
    if (update != 0) {
        *(s32*)((s32)yuwp + 0x10) = *(s32*)((s32)yuwp + 8);
    }
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


/* CHATGPT STUB FILL: main/event/evt_yuugijou 20260624_184823 */

/* stub-fill: monteCountDisp | missing_definition | ghidra_signature */
void monteCountDisp(void) {
    extern void* yuwp;
    extern void windowDispGX_Waku_col(s32 type, void* color, f32 x, f32 y, f32 w, f32 h, f32 unk);
    extern void PSMTXTrans(void* mtx, f32 x, f32 y, f32 z);
    extern void iconDispGxCol(void* mtx, s32 size, s32 iconId, void* color);
    extern void iconNumberDispGx(void* mtx, s32 value, s32 flags, void* color);
    extern void psndSFXOn(s32 soundId);
    extern u32 dat_80427a08;
    extern u32 dat_80427a0c;
    extern u32 dat_80427a10;
    extern u32 dat_80427a14;
    extern f32 float_neg288_80427a18;
    extern f32 float_200_80427a1c;
    extern f32 float_44_80427a20;
    extern f32 float_16_80427a24;
    extern f32 float_neg260_80427a28;
    extern f32 float_0_80427a2c;
    extern f32 float_neg230_80427a30;
    extern f32 float_neg128_80427a34;

    static s32 monte_snd_flag_484;

    f32 mtx[3][4];
    u32 color;
    s32 x = -0xB4;
    s32 diff;

    if ((*(u32*)yuwp & 4) != 0) {
        x = -0x78;
    }
    if (((*(u32*)yuwp & 1) == 0) || ((*(u32*)yuwp & 2) == 0)) {
        if (monte_snd_flag_484 != 0) {
            monte_snd_flag_484--;
        }
        return;
    }

    color = dat_80427a08;
    windowDispGX_Waku_col(0, &color, float_neg288_80427a18, (f32)(x + 0x2C),
                          float_200_80427a1c, float_44_80427a20, float_16_80427a24);

    PSMTXTrans(mtx, float_neg260_80427a28, (f32)(x + 2), float_0_80427a2c);
    color = dat_80427a0c;
    iconDispGxCol(mtx, 0x10, 0x147, &color);

    PSMTXTrans(mtx, float_neg230_80427a30, (f32)(x + 2), float_0_80427a2c);
    color = dat_80427a10;
    iconDispGxCol(mtx, 0x10, 0x1DE, &color);

    PSMTXTrans(mtx, float_neg128_80427a34, (f32)(x + 2), float_0_80427a2c);
    color = dat_80427a14;
    iconNumberDispGx(mtx, *(s32*)((s32)yuwp + 0x10), 0, &color);

    *(s32*)((s32)yuwp + 0xC) += 1;
    if ((*(u32*)((s32)yuwp + 0xC) & 3) == 0) {
        *(s32*)((s32)yuwp + 0xC) = 0;
        diff = *(s32*)((s32)yuwp + 8) - *(s32*)((s32)yuwp + 0x10);

        if (monte_snd_flag_484 == 0 && diff != 0) {
            psndSFXOn(0x8CA);
            monte_snd_flag_484 = 8;
        }

        if (diff != 0) {
            diff /= 10;
            if (diff == 0) {
                if (*(s32*)((s32)yuwp + 8) > *(s32*)((s32)yuwp + 0x10)) {
                    diff = 1;
                } else {
                    diff = -1;
                }
            }
            *(s32*)((s32)yuwp + 0x10) += diff;
        }
    }

    if (monte_snd_flag_484 != 0) {
        monte_snd_flag_484--;
    }
}

/* stub-fill: yuugijou_init | missing_definition | ghidra_signature */
u8 yuugijou_init(void) {
    return 0;
}
