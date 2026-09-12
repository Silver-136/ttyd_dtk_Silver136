#include "action/ac_repeatedly_lv.h"

void BattleAcGaugeSeDelete(void* wp);
static void actionCommandDisp(f32 x, f32 y);
f32 intplGetValue(s32 type, f32 start, f32 end, s32 time, s32 duration);
extern const u32 dat_80424b68;
extern const u32 dat_80424b6c;
extern const f32 float_neg200_80424b70;
extern const f32 float_70_80424b74;
extern const f32 float_1_80424b78;
extern const f32 float_neg225_80424b7c;
extern const f32 float_neg175_80424b80;
extern const f32 float_100_80424b84;
extern const f32 float_0p02_80424b88;
extern const f32 float_0p01_80424b8c;
extern const f32 float_neg288_80424b90;
extern const f32 float_45_80424b94;
extern const f32 float_108_80424b98;
extern const f32 float_25_80424b9c;
extern const f32 float_0_80424ba0;
extern const f32 float_neg300_80424ba4;
extern const f32 float_30_80424ba8;
extern const f32 float_0p4_80424bac;
extern const f32 float_1p5_80424bb0;

void _init_param(void* battleWork) {
    u8* work = battleWork;
    u32* buttons = (u32*)(work + 0x1F4C);
    s32 initialCount;
    s32 param;

    *(u32*)(work + 0x1F58) = 0;
    work[0x1F73] = 1;
    param = *(s32*)(work + 0x1CC8);
    switch (param) {
        case 2:
            buttons[2] = 0x200; buttons[1] = 0x200; buttons[0] = 0x200; break;
        case 3:
            buttons[2] = 0x400; buttons[1] = 0x400; buttons[0] = 0x400; break;
        case 4:
            buttons[2] = 0x800; buttons[1] = 0x800; buttons[0] = 0x800; break;
        case 5:
            buttons[2] = 0x20; buttons[1] = 0x20; buttons[0] = 0x20; break;
        case 8:
            *(u32*)(work + 0x1F58) = 0x100;
        case 7:
            work[0x1F73] = 0;
        case 6:
            buttons[2] = 0x40000; buttons[1] = 0x40000; buttons[0] = 0x40000; break;
        case 0xB:
            buttons[1] = 0x100; buttons[0] = 0x100; buttons[2] = 0x200; break;
        case 0xC:
            buttons[1] = 0x400; buttons[0] = 0x400; buttons[2] = 0x800; break;
        case 0xD:
            buttons[1] = 0x10000; buttons[0] = 0x10000; buttons[2] = 0x20000; break;
        case 0xE:
            buttons[1] = 0x40000; buttons[0] = 0x40000; buttons[2] = 0x80000; break;
        case 0xF:
            buttons[1] = 0x40; buttons[0] = 0x40; buttons[2] = 0x20; break;
        default:
            buttons[2] = 0x100; buttons[1] = 0x100; buttons[0] = 0x100; break;
    }
    *(s16*)(work + 0x1F6E) = 0;
    initialCount = *(s32*)(work + 0x1CCC);
    *(s16*)(work + 0x1F6C) = (s16)initialCount;
    *(s16*)(work + 0x1F6A) = (s16)initialCount;
    *(s16*)(work + 0x1F64) = 0;
    *(f32*)(work + 0x1F60) = float_0_80424ba0;
    *(s16*)(work + 0x1F68) = 0;
    work[0x1F70] = (u8)*(s32*)(work + 0x1CD0);
    *(s16*)(work + 0x1F66) = (s16)*(s32*)(work + 0x1CD4);
    *(f32*)(work + 0x1F5C) = float_0p01_80424b8c * (f32)*(s32*)(work + 0x1CD8);
    work[0x1F72] = (u8)*(s32*)(work + 0x1CDC);
    if (*(s16*)(work + 0x1D18) > 0) {
        (*(s16*)(work + 0x1D18))--;
        work[0x1F71] = 1;
    } else {
        work[0x1F71] = 0;
    }
    *(u32*)(work + 0x1CF0) = 0;
    *(u32*)(work + 0x1CE8) = 0;
    *(u32*)(work + 0x1CEC) = 0;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 battleAcMain_RepeatedlyLv(void* battleWork) {
    extern u32 BattlePadCheckNow(u32 buttons);
    extern u32 BattlePadCheckTrigger(u32 buttons);
    extern void BattlePadCheckUp(u32 buttons);
    extern void BattleAcGaugeSeInit(void);
    extern void BattleAcGaugeSeUpdate(f32 value);
    extern s32 irand(s32 max);

    void* unit;
    register u8* disp;
    register u8* extra;
    s32 autoCommand;
    s32 pressed;
    s32 state;
    s32 param;
    s32 value;
    s16 count;

    pressed = 0;
    autoCommand = 0;
    unit = *(void**)((s32)battleWork + 0x1C90);
    if (*(u8*)((s32)unit + 0x307) != 0) {
        autoCommand = 1;
    }

    extra = (u8*)battleWork + 0x1F4C;
    disp = (u8*)battleWork + 0x1F20;
    state = *(s32*)((s32)battleWork + 0x1C9C);

    switch (state) {
        case 0:
            memset(disp, 0, 0x2C);
            *(f32*)((s32)disp + 0x14) = float_neg300_80424ba4;
            *(f32*)((s32)disp + 0x18) = float_0_80424ba0;
            *(s32*)((s32)disp + 0x20) = 0x14;
            *(u32*)((s32)disp + 0x0C) = dat_80424b68;
            *(u8*)((s32)disp + 0x10) = 0;
            *(u8*)((s32)disp + 0x11) = -0x10;
            *(u8*)((s32)disp + 0x12) = -0x10;
            *(u8*)((s32)disp + 0x13) = 0;
            *(f32*)((s32)disp + 0x28) = float_0_80424ba0;
            *(s32*)((s32)battleWork + 0x1CB8) = 1;
            _init_param(battleWork);
            *(s32*)((s32)battleWork + 0x1C9C) = 99;
            return 1;
        case 99:
            return 1;
        case 100:
            if ((((*(u32*)((s32)battleWork + 0x1C94) & 1) == 0) ||
                 ((*(u32*)((s32)unit + 0x27C) & 0x10) == 0)) &&
                *(u32*)((s32)extra + 0x0C) != 0 &&
                BattlePadCheckNow(*(u32*)((s32)extra + 0x0C)) == 0 &&
                autoCommand == 0) {
                *(s16*)((s32)extra + 0x22) += 1;
                if (*(s16*)((s32)extra + 0x22) < 0x96) {
                    return 1;
                }
                *(s32*)((s32)battleWork + 0x1CF0) = 0;
                *(u32*)((s32)battleWork + 0x1CB8) &= ~1;
                *(u32*)((s32)battleWork + 0x1CB8) |= 2;
                *(s32*)((s32)battleWork + 0x1C9C) = 1002;
                return 1;
            }
            BattleAcGaugeSeInit();
            *(s32*)((s32)battleWork + 0x1CE8) = 1;
            *(s32*)((s32)battleWork + 0x1C9C) = 1000;
            break;
        case 1002:
            goto state_1002;
        case 1003:
            goto state_1003;
        case 1004:
            goto state_1004;
        default:
            if (state != 1000) {
                return 1;
            }
            break;
    }

    unit = *(void**)((s32)battleWork + 0x1C90);
    if (((*(u32*)((s32)battleWork + 0x1C94) & 1) != 0) &&
        ((*(u32*)((s32)unit + 0x27C) & 0x10) != 0)) {
        if (((*(u32*)((s32)battleWork + 0x1C94) & 2) == 0) ||
            (--*(s32*)((s32)battleWork + 0x1C98) < 0)) {
            *(u32*)((s32)battleWork + 0x1CB8) &= ~1;
            *(u32*)((s32)battleWork + 0x1CB8) |= 2;
            value = irand(100);
            *(s16*)(extra + 0x18) = 0;
            do {
                value -= *(u8*)(extra + 0x26);
                if (value < 0) {
                    break;
                }
                *(s16*)(extra + 0x18) += 1;
            } while (value >= 0);
            *(s32*)((s32)battleWork + 0x1CF0) = *(s16*)(extra + 0x18);
            *(s32*)((s32)battleWork + 0x1CB4) += 1;
            battleAcDelete_RepeatedlyLv(battleWork);
            return 0;
        }
        return 1;
    } else {
        param = *(s32*)((s32)battleWork + 0x1CC8);
        if (param > 10 && param < 0x10) {
            if ((*(u16*)((s32)extra + 0x1C) & 1) == 0) {
                *(u32*)extra = *(u32*)((s32)extra + 4);
            } else {
                *(u32*)extra = *(u32*)((s32)extra + 8);
            }
        }

        if (*(s8*)((s32)extra + 0x27) == 0) {
            value = *(u32*)extra;
            BattlePadCheckUp(value);
            if (value != 0) {
                pressed = 1;
                *(s16*)((s32)extra + 0x1C) += 1;
            }
        } else if ((*(u32*)((s32)battleWork + 0x1CC4) & 0x20) == 0) {
            if (BattlePadCheckTrigger(*(u32*)extra) != 0) {
                pressed = 1;
                *(s16*)((s32)extra + 0x1C) += 1;
            }
        } else {
            if (BattlePadCheckNow(*(u32*)extra) != 0) {
                pressed = 1;
                *(s16*)((s32)extra + 0x1C) += 1;
            }
        }

        if (autoCommand != 0 && pressed == 0) {
            pressed = 1;
            *(s16*)((s32)extra + 0x1C) += 1;
        }

        if (*(s8*)((s32)extra + 0x25) == 0) {
            if (pressed != 0) {
                *(f32*)((s32)extra + 0x14) += float_0p4_80424bac * (f32)*(s16*)((s32)extra + 0x1A);
            }
            if (*(f32*)((s32)extra + 0x14) < float_100_80424b84) {
                *(f32*)((s32)extra + 0x14) -= *(f32*)((s32)extra + 0x10);
                if (*(f32*)((s32)extra + 0x14) <= float_0_80424ba0) {
                    *(f32*)((s32)extra + 0x14) = float_0_80424ba0;
                }
            } else {
                *(f32*)((s32)extra + 0x14) = float_100_80424b84;
                *(s8*)((s32)extra + 0x25) = 1;
                if ((*(u32*)((s32)battleWork + 0x1CC4) & 0x20) != 0) {
                    *(s8*)((s32)extra + 0x25) = -1;
                }
            }
        } else if (*(s8*)((s32)extra + 0x25) == -1) {
            *(f32*)((s32)extra + 0x14) = -((float_1p5_80424bb0 * *(f32*)((s32)extra + 0x10)) - *(f32*)((s32)extra + 0x14));
            if (*(f32*)((s32)extra + 0x14) <= float_0_80424ba0) {
                *(f32*)((s32)extra + 0x14) = float_0_80424ba0;
                *(s8*)((s32)extra + 0x25) = 0;
            }
        }

        *(s32*)((s32)battleWork + 0x1CEC) = (s32)*(f32*)((s32)extra + 0x14);
        value = (s32)*(f32*)((s32)extra + 0x14);
        if (*(u8*)((s32)extra + 0x26) == 0) {
            *(u8*)((s32)extra + 0x26) = 100;
        }

        *(s16*)((s32)extra + 0x18) = 0;
        while (value > 0) {
            value -= *(u8*)((s32)extra + 0x26);
            *(s16*)((s32)extra + 0x18) += 1;
        }

        if (*(s16*)((s32)extra + 0x18) == 0 && (*(u32*)((s32)battleWork + 0x1C94) & 0x40) != 0) {
            *(s16*)((s32)extra + 0x18) = 1;
        }
        if (*(s8*)((s32)extra + 0x25) != 0 && (*(u32*)((s32)battleWork + 0x1C94) & 1) == 0) {
            *(s16*)((s32)extra + 0x18) += 1;
        }
        *(s32*)((s32)battleWork + 0x1CF0) = *(s16*)((s32)extra + 0x18);

        if (*(u32*)((s32)extra + 0x0C) != 0 && BattlePadCheckNow(*(u32*)((s32)extra + 0x0C)) == 0) {
            *(s16*)((s32)extra + 0x20) = 0;
        }

        BattleAcGaugeSeUpdate(*(f32*)((s32)extra + 0x14));
        count = *(s16*)((s32)extra + 0x20);
        if (count < 1) {
            *(u32*)((s32)battleWork + 0x1CB8) &= ~1;
            if (*(f32*)((s32)extra + 0x14) < (f32)*(s32*)((s32)battleWork + 0x1CE0)) {
                *(u32*)((s32)battleWork + 0x1CB8) &= ~2;
            } else {
                *(u32*)((s32)battleWork + 0x1CB8) |= 2;
            }
            battleAcDelete_RepeatedlyLv(battleWork);
            *(s32*)((s32)battleWork + 0x1C9C) = 1002;
        } else {
            *(s16*)((s32)extra + 0x20) = count - 1;
        }
    }

    return 1;

state_1002:
    *(s32*)(extra + 0x30) = 0x3C;
    *(s32*)((s32)battleWork + 0x1C9C) = 1003;
    return 1;

state_1003:
    *(s32*)(extra + 0x30) -= 1;
    if (*(s32*)(extra + 0x30) <= 0) {
        *(s32*)((s32)battleWork + 0x1C9C) = 1004;
    }
    return 1;

state_1004:
    *(void**)((s32)battleWork + 0x1CA0) = 0;
    *(void**)((s32)battleWork + 0x1CA4) = 0;
    *(void**)((s32)battleWork + 0x1CA8) = 0;
    *(void**)((s32)battleWork + 0x1CAC) = 0;
    return 1;
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset


s32 battleAcResult_RepeatedlyLv(void* wp) {
    return *(s32*)((s32)wp + 0x1CB8);
}
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void battleAcDisp_RepeatedlyLv(s32 unused, void* battleWork) {
    typedef struct AcRepeatedlyLvDispWork {
        u8 pad_00[0x14];
        f32 x;
        f32 y;
        u8 pad_1C[0x4];
        s32 timer;
    } AcRepeatedlyLvDispWork;
    register AcRepeatedlyLvDispWork* disp = (AcRepeatedlyLvDispWork*)((s32)battleWork + 0x1F20);
    s32 state;
    s32 timer;

    state = *(s32*)((s32)battleWork + 0x1C9C);
    if (state < 1000) {
        if (state >= 101) {
            return;
        }
        if (state >= 99) {
            goto opening;
        }
        return;
    } else {
        if (state >= 1004) {
            return;
        }
        if (state >= 1002) {
            goto closing;
        }
    }

opening:
    disp->x = intplGetValue(4, float_neg300_80424ba4, float_30_80424ba8,
                            0x14 - disp->timer, 0x14);
    actionCommandDisp(disp->x, disp->y);
    timer = disp->timer;
    if (timer > 0) {
        disp->timer = timer - 1;
    }
    return;

closing:
    timer = disp->timer;
    if (timer >= 0x28) {
        disp->x = intplGetValue(4, float_30_80424ba8, float_neg300_80424ba4,
                                timer - 0x28, 0x14);
    } else {
        disp->x = float_30_80424ba8;
    }
    actionCommandDisp(disp->x, disp->y);
    timer = disp->timer;
    if (timer < 0x3C) {
        disp->timer = timer + 1;
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


void battleAcDelete_RepeatedlyLv(void* wp) {
    *(s32*)((s32)wp + 0x1C9C) = 1002;
    BattleAcGaugeSeDelete(wp);
}


static void actionCommandDisp(f32 x, f32 y) {
    typedef struct Vec { f32 x, y, z; } Vec;
    typedef f32 Mtx[3][4];
    extern void* g_BattleWork;
    extern void* camGetPtr(s32);
    extern s32 BattleACGetButtonIcon(s32,s32);
    extern void iconDispGx(f32,Vec*,s32,s32);
    extern void BattleAcDrawGauge(f32,s32,s32,s32,s32,s32,s32,s32,s32);
    extern Vec vec3_802faa10[7];
    extern void PSMTXTrans(Mtx, f32, f32, f32);
    extern void PSMTXScale(Mtx, f32, f32, f32);
    extern void PSMTXConcat(Mtx, Mtx, Mtx);
    extern void iconNumberDispGx(Mtx, s32, s32, u32*);
    u8* battle;
    u8* work;
    u8* disp;
    u8* extra;
    Vec* vecs;
    Vec pos;
    Vec posSingleOff;
    Vec posSingleOn;
    Vec posLeftOff;
    Vec posRightOff;
    Vec posLeftOn;
    Vec posRightOn;
    f32 target;
    f32 ratio;
    u16 pressed1, normal1, pressed2, normal2;
    s32 count;
    s32 leftOffset;
    s32 rightOffset;
    s32 flags;
    s32 gauge[4];
    s32 i;
    Mtx translate;
    Mtx scale;
    Mtx model;
    u32 color;

    battle = g_BattleWork;
    work = battle + 0x1C90;
    vecs = vec3_802faa10;
    camGetPtr(8);
    disp = work + 0x290;
    extra = work + 0x2BC;
    pressed2 = normal2 = 0;
    count = 1;
    leftOffset = rightOffset = 0;
    (*(s32*)disp)++;
    pressed1 = BattleACGetButtonIcon(*(s32*)extra, 1);
    normal1 = BattleACGetButtonIcon(*(s32*)extra, 0);
    i = *(s32*)(work + 0x38);
    if (i == 0xE) {
        pressed1 = 0x80; normal1 = 0x82;
    } else if (i < 0xE) {
        if (i >= 0xD) {
            pressed1 = 0x84; normal1 = 0x7E;
        } else if (i >= 0xB) {
            goto two_icons;
        }
    } else if (i < 0x10) {
            leftOffset = -10;
            rightOffset = 10;
two_icons:
        pressed1 = BattleACGetButtonIcon(*(s32*)(extra + 4), 1);
        normal1 = BattleACGetButtonIcon(*(s32*)(extra + 4), 0);
        pressed2 = BattleACGetButtonIcon(*(s32*)(extra + 8), 1);
        normal2 = BattleACGetButtonIcon(*(s32*)(extra + 8), 0);
        count = 2;
    }
    if (count == 1) {
        if (((*(s32*)disp / 7) & 1) != 0) {
            posSingleOff = vecs[1];
            posSingleOff.x = float_neg200_80424b70 + x;
            posSingleOff.y = float_70_80424b74 + y;
            iconDispGx(float_1_80424b78, &posSingleOff, 0x10, normal1);
        } else {
            posSingleOn = vecs[2];
            posSingleOn.x = float_neg200_80424b70 + x;
            posSingleOn.y = float_70_80424b74 + y;
            iconDispGx(float_1_80424b78, &posSingleOn, 0x10, pressed1);
        }
    } else {
        if (((*(s32*)disp / 7) & 1) != 0) {
            posLeftOff = vecs[3];
            posLeftOff.x = (float_neg225_80424b7c + x) + (f32)leftOffset;
            posLeftOff.y = float_70_80424b74 + y;
            iconDispGx(float_1_80424b78, &posLeftOff, 0x10, normal1);
            posRightOff = vecs[4];
            posRightOff.x = (float_neg175_80424b80 + x) + (f32)rightOffset;
            posRightOff.y = float_70_80424b74 + y;
            iconDispGx(float_1_80424b78, &posRightOff, 0x10, normal2);
        } else {
            posLeftOn = vecs[5];
            posLeftOn.x = (float_neg225_80424b7c + x) + (f32)leftOffset;
            posLeftOn.y = float_70_80424b74 + y;
            iconDispGx(float_1_80424b78, &posLeftOn, 0x10, pressed1);
            posRightOn = vecs[6];
            posRightOn.x = (float_neg175_80424b80 + x) + (f32)rightOffset;
            posRightOn.y = float_70_80424b74 + y;
            iconDispGx(float_1_80424b78, &posRightOn, 0x10, pressed2);
        }
    }
    target = *(f32*)(extra + 0x14) / float_100_80424b84;
    ratio = *(f32*)(disp + 0x28);
    if (target < ratio) {
        *(f32*)(disp + 0x28) = ratio - ((*(u32*)(work + 4) & 8) ? float_0p02_80424b88 : float_0p01_80424b8c);
        if ((*(u32*)(work + 4) & 0x10) != 0) {
            *(f32*)(disp + 0x28) = target;
        }
        if (*(f32*)(disp + 0x28) < target) {
            *(f32*)(disp + 0x28) = target;
        }
    } else {
        *(f32*)(disp + 0x28) = ratio + ((*(u32*)(work + 4) & 8) ? float_0p02_80424b88 : float_0p01_80424b8c);
        if ((*(u32*)(work + 4) & 0x10) != 0) {
            *(f32*)(disp + 0x28) = target;
        }
        if (target < *(f32*)(disp + 0x28)) {
            *(f32*)(disp + 0x28) = target;
        }
    }
    ratio = *(f32*)(disp + 0x28);
    flags = 0;
    if (*(s8*)(extra + 0x25) == 1 && ratio >= float_1_80424b78) {
        flags = 1;
        disp[0xD] += *(s8*)(disp + 0x11);
        disp[0xE] += *(s8*)(disp + 0x12);
        if (disp[0xD] < 0x81 && *(s8*)(disp + 0x11) < 0) {
            disp[0x11] = 0x10;
            disp[0x12] = 0x10;
        }
        if (disp[0xD] > 0xF4 && *(s8*)(disp + 0x11) > 0) {
            disp[0x11] = 0xF0;
            disp[0x12] = 0xF0;
        }
    }
    if (*(s8*)(extra + 0x25) == -1) flags |= 2;
    for (i = 0; i < 4; i++) gauge[i] = 100;
    for (i = 0; i < 4; i++) {
        gauge[i] = *(s32*)(work + 0x48 + i * 4);
        if (i != 0 && gauge[i] <= gauge[i - 1]) break;
    }
    BattleAcDrawGauge(ratio, (s32)x, (s32)y, extra[0x24], i, gauge[0], gauge[1], gauge[2], flags);
    if ((*(u32*)(work + 4) & 2) != 0) {
        pos.x = (float_neg288_80424b90 + x) +
                ((f32)(*(s32*)(work + 0x50) * 0xB0) / float_100_80424b84);
        pos.y = float_45_80424b94 + y;
        pos.z = float_0_80424ba0;
        iconDispGx(float_1_80424b78, &pos, 0x10, 0x9E);
    }
    if ((*(u32*)(work + 4) & 4) != 0) {
        PSMTXTrans(translate, (float_neg200_80424b70 + x) + float_108_80424b98,
                   float_25_80424b9c + y, float_0_80424ba0);
        PSMTXScale(scale, float_1_80424b78, float_1_80424b78, float_1_80424b78);
        PSMTXConcat(translate, scale, model);
        color = dat_80424b6c;
        iconNumberDispGx(model, *(s32*)(work + 0x60) + *(s32*)(work + 0x54), 0, &color);
    }
}

/* Target-owned .sdata2 catalog: definitions intentionally after all consumers. */
const u32 dat_80424b68 = 0xFFFFFFFF;
const u32 dat_80424b6c = 0xFFFFFFFF;
const f32 float_neg200_80424b70 = -200.0f;
const f32 float_70_80424b74 = 70.0f;
const f32 float_1_80424b78 = 1.0f;
const f32 float_neg225_80424b7c = -225.0f;
const f32 float_neg175_80424b80 = -175.0f;
const f32 float_100_80424b84 = 100.0f;
const f32 float_0p02_80424b88 = 0.02f;
const f32 float_0p01_80424b8c = 0.01f;
const f32 float_neg288_80424b90 = -288.0f;
const f32 float_45_80424b94 = 45.0f;
const f32 float_108_80424b98 = 108.0f;
const f32 float_25_80424b9c = 25.0f;
const f32 float_0_80424ba0 = 0.0f;
const f32 float_neg300_80424ba4 = -300.0f;
const f32 float_30_80424ba8 = 30.0f;
const f32 float_0p4_80424bac = 0.4f;
const f32 float_1p5_80424bb0 = 1.5f;
