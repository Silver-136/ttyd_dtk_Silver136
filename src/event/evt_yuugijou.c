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
void yuugijou_init(void) {
    static u8 yuugijouwork[600];
    u8* w;
    s32 rank;

    yuwp = yuugijouwork;
    memset(yuwp, 0, 600);
    w = yuwp;

    *(s32*)(w + 0x14) = 1; *(s32*)(w + 0x18) = 1; *(s32*)(w + 0x1C) = 1;
    *(s32*)(w + 0x20) = 1; *(s32*)(w + 0x24) = 1;
    *(s32*)(w + 0xD4) = 0x5B964; *(s32*)(w + 0xD8) = 0x5825A;
    *(s32*)(w + 0xDC) = 0x4F826; *(s32*)(w + 0xE0) = 0x4C3D8;
    *(s32*)(w + 0xE4) = 0x4237E;
    *(s32*)(w + 0x194) = 14; *(s32*)(w + 0x198) = 12;
    *(s32*)(w + 0x19C) = 15; *(s32*)(w + 0x1A0) = 11;
    *(s32*)(w + 0x1A4) = 10;

    *(s32*)(w + 0x2C) = 1; *(s32*)(w + 0x30) = 1; *(s32*)(w + 0x34) = 1;
    *(s32*)(w + 0x38) = 1; *(s32*)(w + 0x3C) = 1;
    *(s32*)(w + 0xEC) = 2; *(s32*)(w + 0xF0) = 5; *(s32*)(w + 0xF4) = 0x12;
    *(s32*)(w + 0xF8) = 0x1B; *(s32*)(w + 0xFC) = 0x8A;
    rank = ((700 - *(s32*)(w + 0xEC)) * 15) / 700; if (rank < 0) rank = 0; *(s32*)(w + 0x1AC) = rank + 1;
    rank = ((700 - *(s32*)(w + 0xF0)) * 15) / 700; if (rank < 0) rank = 0; *(s32*)(w + 0x1B0) = rank;
    rank = ((700 - *(s32*)(w + 0xF4)) * 15) / 700; if (rank < 0) rank = 0; *(s32*)(w + 0x1B4) = rank;
    rank = ((700 - *(s32*)(w + 0xF8)) * 15) / 700; if (rank < 0) rank = 0; *(s32*)(w + 0x1B8) = rank;
    rank = ((700 - *(s32*)(w + 0xFC)) * 15) / 700; if (rank < 0) rank = 0; *(s32*)(w + 0x1BC) = rank;

    *(s32*)(w + 0x44) = 1; *(s32*)(w + 0x48) = 1; *(s32*)(w + 0x4C) = 1;
    *(s32*)(w + 0x50) = 1; *(s32*)(w + 0x54) = 1;
    *(s32*)(w + 0x104) = 0x20850; *(s32*)(w + 0x108) = 0x20436;
    *(s32*)(w + 0x10C) = 0x20184; *(s32*)(w + 0x110) = 0x1F91E;
    *(s32*)(w + 0x114) = 0x1E3AC;
    rank = (*(s32*)(w + 0x104) - 0x1A9C8) / 2000; if (rank < 0) rank = 0; *(s32*)(w + 0x1C4) = rank + 7;
    rank = (*(s32*)(w + 0x108) - 0x1A9C8) / 2000; if (rank < 0) rank = 0; *(s32*)(w + 0x1C8) = rank + 6;
    rank = (*(s32*)(w + 0x10C) - 0x1A9C8) / 2000; if (rank < 0) rank = 0; *(s32*)(w + 0x1CC) = rank + 4;
    rank = (*(s32*)(w + 0x110) - 0x1A9C8) / 2000; if (rank < 0) rank = 0; *(s32*)(w + 0x1D0) = rank + 4;
    rank = (*(s32*)(w + 0x114) - 0x1A9C8) / 2000; if (rank < 0) rank = 0; *(s32*)(w + 0x1D4) = rank + 1;

    *(s32*)(w + 0x5C) = 1; *(s32*)(w + 0x60) = 1; *(s32*)(w + 0x64) = 1;
    *(s32*)(w + 0x68) = 1; *(s32*)(w + 0x6C) = 1;
    *(s32*)(w + 0x11C) = 0x16BDE; *(s32*)(w + 0x120) = 0x16968;
    *(s32*)(w + 0x124) = 0x16382; *(s32*)(w + 0x128) = 0x1627E;
    *(s32*)(w + 0x12C) = 0x15950;
    rank = (*(s32*)(w + 0x11C) - 74000) / 2000; if (rank < 0) rank = 0; *(s32*)(w + 0x1DC) = rank + 7;
    rank = (*(s32*)(w + 0x120) - 74000) / 2000; if (rank < 0) rank = 0; *(s32*)(w + 0x1E0) = rank + 6;
    rank = (*(s32*)(w + 0x124) - 74000) / 2000; if (rank < 0) rank = 0; *(s32*)(w + 0x1E4) = rank + 4;
    rank = (*(s32*)(w + 0x128) - 74000) / 2000; if (rank < 0) rank = 0; *(s32*)(w + 0x1E8) = rank + 3;
    rank = (*(s32*)(w + 0x12C) - 74000) / 2000; if (rank < 0) rank = 0; *(s32*)(w + 0x1EC) = rank + 4;

    *(s32*)(w + 0x74) = 1; *(s32*)(w + 0x78) = 1; *(s32*)(w + 0x7C) = 1;
    *(s32*)(w + 0x80) = 1; *(s32*)(w + 0x84) = 1;
    *(s32*)(w + 0x134) = 0x5B964; *(s32*)(w + 0x138) = 0x5825A;
    *(s32*)(w + 0x13C) = 0x4F826; *(s32*)(w + 0x140) = 0x4C3D8;
    *(s32*)(w + 0x144) = 0x4237E;
    *(s32*)(w + 0x1F4) = 14; *(s32*)(w + 0x1F8) = 12;
    *(s32*)(w + 0x1FC) = 15; *(s32*)(w + 0x200) = 11;
    *(s32*)(w + 0x204) = 10;

    *(s32*)(w + 0x8C) = 1; *(s32*)(w + 0x90) = 1; *(s32*)(w + 0x94) = 1;
    *(s32*)(w + 0x98) = 1; *(s32*)(w + 0x9C) = 1;
    *(s32*)(w + 0x14C) = 2; *(s32*)(w + 0x150) = 5; *(s32*)(w + 0x154) = 0x12;
    *(s32*)(w + 0x158) = 0x1B; *(s32*)(w + 0x15C) = 0x8A;
    rank = ((700 - *(s32*)(w + 0x14C)) * 15) / 700; if (rank < 0) rank = 0; *(s32*)(w + 0x20C) = rank + 1;
    rank = ((700 - *(s32*)(w + 0x150)) * 15) / 700; if (rank < 0) rank = 0; *(s32*)(w + 0x210) = rank;
    rank = ((700 - *(s32*)(w + 0x154)) * 15) / 700; if (rank < 0) rank = 0; *(s32*)(w + 0x214) = rank;
    rank = ((700 - *(s32*)(w + 0x158)) * 15) / 700; if (rank < 0) rank = 0; *(s32*)(w + 0x218) = rank;
    rank = ((700 - *(s32*)(w + 0x15C)) * 15) / 700; if (rank < 0) rank = 0; *(s32*)(w + 0x21C) = rank;

    *(s32*)(w + 0xA4) = 1; *(s32*)(w + 0xA8) = 1; *(s32*)(w + 0xAC) = 1;
    *(s32*)(w + 0xB0) = 1; *(s32*)(w + 0xB4) = 1;
    *(s32*)(w + 0x164) = 0x1E3DE; *(s32*)(w + 0x168) = 0x1E00A;
    *(s32*)(w + 0x16C) = 0x1DD8A; *(s32*)(w + 0x170) = 0x1D5BA;
    *(s32*)(w + 0x174) = 0x1C1CE;
    rank = (*(s32*)(w + 0x164) - 0x1A9C8) / 2000; if (rank < 0) rank = 0; *(s32*)(w + 0x224) = rank + 7;
    rank = (*(s32*)(w + 0x168) - 0x1A9C8) / 2000; if (rank < 0) rank = 0; *(s32*)(w + 0x228) = rank + 6;
    rank = (*(s32*)(w + 0x16C) - 0x1A9C8) / 2000; if (rank < 0) rank = 0; *(s32*)(w + 0x22C) = rank + 4;
    rank = (*(s32*)(w + 0x170) - 0x1A9C8) / 2000; if (rank < 0) rank = 0; *(s32*)(w + 0x230) = rank + 4;
    rank = (*(s32*)(w + 0x174) - 0x1A9C8) / 2000; if (rank < 0) rank = 0; *(s32*)(w + 0x234) = rank + 1;

    *(s32*)(w + 0xBC) = 1; *(s32*)(w + 0xC0) = 1; *(s32*)(w + 0xC4) = 1;
    *(s32*)(w + 0xC8) = 1; *(s32*)(w + 0xCC) = 1;
    *(s32*)(w + 0x17C) = 0x1525C; *(s32*)(w + 0x180) = 0x15018;
    *(s32*)(w + 0x184) = 0x14A96; *(s32*)(w + 0x188) = 0x149A6;
    *(s32*)(w + 0x18C) = 0x14122;
    rank = (*(s32*)(w + 0x17C) - 74000) / 2000; if (rank < 0) rank = 0; *(s32*)(w + 0x23C) = rank + 7;
    rank = (*(s32*)(w + 0x180) - 74000) / 2000; if (rank < 0) rank = 0; *(s32*)(w + 0x240) = rank + 6;
    rank = (*(s32*)(w + 0x184) - 74000) / 2000; if (rank < 0) rank = 0; *(s32*)(w + 0x244) = rank + 4;
    rank = (*(s32*)(w + 0x188) - 74000) / 2000; if (rank < 0) rank = 0; *(s32*)(w + 0x248) = rank + 3;
    rank = (*(s32*)(w + 0x18C) - 74000) / 2000; if (rank < 0) rank = 0; *(s32*)(w + 0x24C) = rank + 4;
}

