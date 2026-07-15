#include "action/ac_repeatedly.h"

void BattleAcGaugeSeDelete(void* wp);

s32 battleAcResult_Repeatedly(void* wp) {
    return *(s32*)((s32)wp + 0x1CB8);
}

void battleAcDelete_Repeatedly(void* wp) {
    void* callArg;

    switch (*(s32*)((s32)wp + 0x1CC8)) {
        case 10:
            *(u32*)((s32)wp + 0xEF4) &= ~0x20000000;
            break;
        default:
            break;
    }
    BattleAcGaugeSeDelete(callArg);
    *(s32*)((s32)wp + 0x1C9C) = 1002;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 battleAcMain_Repeatedly(void* battleWork) {
    extern u32 BattleActionCommandGetDifficulty(void* battleWork);
    extern void* memset(void* dst, s32 val, u32 size);
    extern void _init_param(void* battleWork);
    extern u32 BattlePadCheckNow(u32 buttons);
    extern void BattleAcGaugeSeInit(void);
    extern u32 OSGetResetButtonState(void);
    extern s32 SIProbe(u32 chan);
    extern s32 irand(s32 max);
    extern u32 BattlePadCheckTrigger(u32 buttons);
    extern void BattleAcGaugeSeUpdate(f32 value);
    extern void battleAcDelete_Repeatedly(void* battleWork);
    extern s32 _power_table[];
    extern u32 dat_80422a90;
    extern f32 float_neg300_80422ab8;
    extern f32 float_0_80422ac0;
    extern f32 float_0p01_80422ac4;
    extern f32 float_neg1_80422ac8;
    extern f32 float_100_80422aa4;

    void* disp;
    void* extra;
    void* unit;
    u32 difficulty;
    s32 pressed;
    s32 state;
    s32 acParam;
    s16 count;

    difficulty = BattleActionCommandGetDifficulty(battleWork);
    unit = *(void**)((s32)battleWork + 0x1C90);
    pressed = 0;
    if (*(u8*)((s32)unit + 0x307) != 0) {
        pressed = 1;
    }
    disp = (void*)((s32)battleWork + 0x1F20);
    extra = (void*)((s32)battleWork + 0x1F4C);
    state = *(s32*)((s32)battleWork + 0x1C9C);

    switch (state) {
        case 0:
            memset(disp, 0, 0x2C);
            *(f32*)((s32)disp + 0x14) = float_neg300_80422ab8;
            *(f32*)((s32)disp + 0x18) = float_0_80422ac0;
            *(s32*)((s32)disp + 0x20) = 0x14;
            *(u32*)((s32)disp + 0x0C) = dat_80422a90;
            *(u8*)((s32)disp + 0x10) = 0;
            *(u8*)((s32)disp + 0x11) = -0x10;
            *(u8*)((s32)disp + 0x12) = -0x10;
            *(u8*)((s32)disp + 0x13) = 0;
            *(f32*)((s32)disp + 0x28) = float_0_80422ac0;
            *(s32*)((s32)battleWork + 0x1CB8) = 1;
            *(s16*)((s32)extra + 0x28) = 0;
            _init_param(battleWork);
            *(s32*)((s32)battleWork + 0x1C9C) = 99;
            return 1;
        case 99:
            return 1;
        case 100:
            if (((*(u32*)((s32)battleWork + 0x1C94) & 1) == 0 ||
                 (*(u32*)((s32)unit + 0x27C) & 0x10) == 0) &&
                *(u32*)((s32)extra + 0x0C) != 0 &&
                BattlePadCheckNow(*(u32*)((s32)extra + 0x0C)) == 0) {
                return 1;
            }
            BattleAcGaugeSeInit();
            *(s32*)((s32)battleWork + 0x1C9C) = 1000;
            *(s32*)((s32)battleWork + 0x1CE8) = 1;
            break;
        case 1002:
            *(s32*)((s32)extra + 0x30) = 0x3C;
            *(s32*)((s32)battleWork + 0x1C9C) = 1003;
            return 1;
        case 1003:
            *(s32*)((s32)extra + 0x30) -= 1;
            if (*(s32*)((s32)extra + 0x30) <= 0) {
                *(s32*)((s32)battleWork + 0x1C9C) = 1004;
            }
            return 1;
        case 1004:
            *(void**)((s32)battleWork + 0x1CA0) = 0;
            *(void**)((s32)battleWork + 0x1CA4) = 0;
            *(void**)((s32)battleWork + 0x1CA8) = 0;
            *(void**)((s32)battleWork + 0x1CAC) = 0;
            return 1;
        default:
            if (state != 1000) {
                return 1;
            }
            break;
    }

    acParam = *(s32*)((s32)battleWork + 0x1CC8);
    if (acParam == 10) {
        if (*(u8*)((s32)extra + 0x2C) == 0) {
            if (OSGetResetButtonState() != 0) {
                pressed = 1;
                *(u8*)((s32)extra + 0x2C) = 1;
                *(s16*)((s32)extra + 0x20) += 1;
            }
        } else if (OSGetResetButtonState() == 0) {
            pressed = 1;
            *(u8*)((s32)extra + 0x2C) = 0;
            *(s16*)((s32)extra + 0x20) += 1;
        }
    } else if (acParam == 9) {
        if (*(u8*)((s32)extra + 0x2C) == 0) {
            if ((SIProbe(0) != 8) && (SIProbe(0) != 0x80)) {
                pressed = 1;
                *(u8*)((s32)extra + 0x2C) = 1;
                *(s16*)((s32)extra + 0x20) += 1;
            }
        } else if (SIProbe(0) == 8) {
            pressed = 1;
            *(u8*)((s32)extra + 0x2C) = 0;
            *(s16*)((s32)extra + 0x20) += 1;
        }
    } else if ((acParam >= 11) && (acParam < 13)) {
        if ((*(u16*)((s32)extra + 0x20) & 1) == 0) {
            *(u32*)extra = *(u32*)((s32)extra + 4);
        } else {
            *(u32*)extra = *(u32*)((s32)extra + 8);
        }
        if (BattlePadCheckTrigger(*(u32*)extra) != 0) {
            pressed = 1;
            *(s16*)((s32)extra + 0x20) += 1;
        }
    } else if (acParam == 0x10) {
        *(s16*)((s32)extra + 0x22) -= 1;
        if (*(s16*)((s32)extra + 0x22) == 0) {
            *(s16*)((s32)extra + 0x22) = (s16)irand(0x1E) + 0x3C;
            if (*(u32*)extra == *(u32*)((s32)extra + 4)) {
                *(u32*)extra = *(u32*)((s32)extra + 8);
            } else {
                *(u32*)extra = *(u32*)((s32)extra + 4);
            }
        }
        if (BattlePadCheckTrigger(*(u32*)extra) != 0) {
            pressed = 1;
            *(s16*)((s32)extra + 0x20) += 1;
        }
    } else if (BattlePadCheckTrigger(*(u32*)extra) != 0) {
        pressed = 1;
        *(s16*)((s32)extra + 0x20) += 1;
    }

    if ((*(u8*)((s32)unit + 0x307) != 0) && !pressed) {
        pressed = 1;
        *(s16*)((s32)extra + 0x20) += 1;
    }

    if (*(s8*)((s32)extra + 0x2B) == 0) {
        if (pressed) {
            *(f32*)((s32)extra + 0x18) +=
                float_0p01_80422ac4 * (f32)*(s16*)((s32)extra + 0x24) * (f32)_power_table[difficulty];
        }
        *(f32*)((s32)extra + 0x18) -= *(f32*)((s32)extra + 0x1C);
        if (*(f32*)((s32)extra + 0x18) <= float_0_80422ac0) {
            *(f32*)((s32)extra + 0x18) = float_0_80422ac0;
        }
        if (float_100_80422aa4 <= *(f32*)((s32)extra + 0x18)) {
            *(f32*)((s32)extra + 0x18) = float_100_80422aa4;
            *(s8*)((s32)extra + 0x2B) = 1;
        }
    }

    if (*(f32*)((s32)extra + 0x14) != float_0_80422ac0) {
        *(f32*)((s32)extra + 0x10) += *(f32*)((s32)extra + 0x14);
        if (*(f32*)((s32)extra + 0x10) <= float_0_80422ac0) {
            *(f32*)((s32)extra + 0x10) = float_0_80422ac0;
            *(f32*)((s32)extra + 0x14) *= float_neg1_80422ac8;
        }
        if (float_100_80422aa4 <= *(f32*)((s32)extra + 0x10)) {
            *(f32*)((s32)extra + 0x10) = float_100_80422aa4;
            *(f32*)((s32)extra + 0x14) *= float_neg1_80422ac8;
        }
    }

    if (*(u32*)((s32)extra + 0x0C) != 0 && BattlePadCheckNow(*(u32*)((s32)extra + 0x0C)) == 0) {
        *(s16*)((s32)extra + 0x28) = 0;
    }

    BattleAcGaugeSeUpdate(*(f32*)((s32)extra + 0x18));
    count = *(s16*)((s32)extra + 0x28);
    if (count < 1) {
        *(u32*)((s32)battleWork + 0x1CB8) &= ~1;
        if ((*(f32*)((s32)extra + 0x10) <= *(f32*)((s32)extra + 0x18)) ||
            (*(s8*)((s32)extra + 0x2B) != 0)) {
            *(u32*)((s32)battleWork + 0x1CB8) |= 2;
        }
        battleAcDelete_Repeatedly(battleWork);
        *(s32*)((s32)battleWork + 0x1C9C) = 1002;
    } else {
        *(s16*)((s32)extra + 0x28) = count - 1;
    }
    return 1;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void actionCommandDisp(f32 x, f32 y) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;

    extern void* camGetPtr(s32 cameraId);
    extern void iconDispGx(Vec* pos, s32 flags, s32 iconId, f32 scale);
    extern void BattleAcDrawGauge(f32 ratio, s32 x, s32 y, s32 innerBarWidth, s32 param5,
                                  s32 bar1, s32 bar2, s32 bar3, s32 flags);
    extern void* _battleWorkPointer;
    extern Vec vec3_802f3760;
    extern f32 float_neg200_80422a94;
    extern f32 float_70_80422a98;
    extern f32 float_1_80422a9c;
    extern f32 float_0p5_80422aa0;
    extern f32 float_100_80422aa4;
    extern f32 float_0p0125_80422aa8;
    extern f32 float_neg288_80422aac;
    extern f32 float_176_80422ab0;
    extern f32 float_45_80422ab4;

    void* battleWork;
    void* disp;
    void* extra;
    Vec pos;
    f32 target;
    f32 ratio;
    s32 iconOn;
    s32 iconOff;
    s32 flags;
    s32 percent;
    s32 i;
    s32 selected;

    battleWork = _battleWorkPointer;
    camGetPtr(8);
    disp = (void*)((s32)battleWork + 0x1F20);
    extra = (void*)((s32)battleWork + 0x1F4C);

    if (*(s32*)((s32)battleWork + 0x1CC8) == 0x10) {
        i = 0;
        while (i < 2) {
            if (i == 0) {
                iconOn = 0x6D;
                iconOff = 0x6C;
                selected = (*(s32*)extra == *(s32*)((s32)extra + 4));
            } else {
                iconOn = 0x6F;
                iconOff = 0x6E;
                selected = (*(s32*)extra == *(s32*)((s32)extra + 8));
            }
            pos = vec3_802f3760;
            pos.x = (float_neg200_80422a94 + x) + (f32)(i * 0x28);
            pos.y = float_70_80422a98 + y;
            if (selected) {
                if (((*(s32*)disp / 7) & 1) != 0) {
                    iconOn = iconOff;
                }
                iconDispGx(&pos, 0x10, iconOn, float_1_80422a9c);
            } else {
                iconDispGx(&pos, 0x10, iconOff, float_0p5_80422aa0);
            }
            i++;
        }
    } else {
        switch (*(s32*)extra) {
            case 0x100:
                iconOn = 0x6D;
                iconOff = 0x6C;
                break;
            case 0x200:
                iconOn = 0x6F;
                iconOff = 0x6E;
                break;
            case 0x400:
                iconOn = 0x71;
                iconOff = 0x70;
                break;
            case 0x800:
                iconOn = 0x73;
                iconOff = 0x72;
                break;
            default:
                iconOn = 0x80;
                iconOff = 0x81;
                break;
        }
        pos = vec3_802f3760;
        pos.x = float_neg200_80422a94 + x;
        pos.y = float_70_80422a98 + y;
        if (((*(s32*)disp / 7) & 1) != 0) {
            iconDispGx(&pos, 0x10, iconOff, float_1_80422a9c);
        } else {
            iconDispGx(&pos, 0x10, iconOn, float_1_80422a9c);
        }
    }

    ratio = *(f32*)((s32)disp + 0x28);
    target = *(f32*)((s32)extra + 0x18) / float_100_80422aa4;
    if (target < ratio) {
        ratio -= float_0p0125_80422aa8;
        if (ratio < target) {
            ratio = target;
        }
    } else {
        ratio += float_0p0125_80422aa8;
        if (target < ratio) {
            ratio = target;
        }
    }
    *(f32*)((s32)disp + 0x28) = ratio;

    flags = 0;
    if ((*(s8*)((s32)extra + 0x2B) == 1) && (float_1_80422a9c <= ratio)) {
        flags = 1;
        *(u8*)((s32)disp + 0x0D) += *(s8*)((s32)disp + 0x11);
        *(u8*)((s32)disp + 0x0E) += *(s8*)((s32)disp + 0x12);
        if ((*(u8*)((s32)disp + 0x0D) < 0x81) && (*(s8*)((s32)disp + 0x11) < 0)) {
            *(u8*)((s32)disp + 0x12) = 0x10;
            *(u8*)((s32)disp + 0x11) = 0x10;
        }
        if ((*(u8*)((s32)disp + 0x0D) > 0xF4) && (*(s8*)((s32)disp + 0x11) > 0)) {
            *(u8*)((s32)disp + 0x12) = 0xF0;
            *(u8*)((s32)disp + 0x11) = 0xF0;
        }
    }
    if (*(s8*)((s32)extra + 0x2B) == -1) {
        flags |= 2;
    }

    if ((*(u32*)((s32)battleWork + 0x1C94) & 2) == 0) {
        percent = (s32)*(f32*)((s32)extra + 0x10);
    } else {
        percent = 100;
    }
    BattleAcDrawGauge(ratio, (s32)x, (s32)y, *(u8*)((s32)extra + 0x2A), 2, percent, 100, 100, flags);

    pos = vec3_802f3760;
    pos.x = (float_neg288_80422aac + x) +
            (float_176_80422ab0 * *(f32*)((s32)extra + 0x10)) / float_100_80422aa4;
    pos.y = float_45_80422ab4 + y;
    iconDispGx(&pos, 0x10, 0x9E, float_1_80422a9c);

    *(s32*)disp += 1;
    *(s32*)((s32)disp + 4) += 1;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

typedef struct AcRepeatedlyDispWork {
    u8 pad_00[0x14];
    f32 x;
    f32 y;
    u8 pad_1C[0x4];
    s32 timer;
} AcRepeatedlyDispWork;

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void battleAcDisp_Repeatedly(s32 param_1, void* battleWork) {
    extern f32 intplGetValue(s32 type, f32 start, f32 end, s32 time, s32 duration);
    extern f32 float_neg300_80422ab8;
    extern f32 float_30_80422abc;
    register AcRepeatedlyDispWork* work = (AcRepeatedlyDispWork*)((s32)battleWork + 0x1F20);
    s32 status = *(s32*)((s32)battleWork + 0x1C9C);
    s32 timer;

    if (status < 1000) {
        if (status >= 101) {
            return;
        }
        if (status >= 99) {
            goto opening;
        }
        return;
    } else {
        if (status >= 1004) {
            return;
        }
        if (status >= 1002) {
            goto closing;
        }
    }

opening:
    work->x = intplGetValue(
        4,
        float_neg300_80422ab8,
        float_30_80422abc,
        20 - work->timer,
        20
    );
    actionCommandDisp(work->x, work->y);
    timer = work->timer;
    if (timer > 0) {
        work->timer = timer - 1;
    }
    return;

closing:
    timer = work->timer;
    if (timer >= 40) {
        work->x = intplGetValue(
            4,
            float_30_80422abc,
            float_neg300_80422ab8,
            timer - 40,
            20
        );
    } else {
        work->x = float_30_80422abc;
    }
    actionCommandDisp(work->x, work->y);
    timer = work->timer;
    if (timer < 60) {
        work->timer = timer + 1;
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


/* CHATGPT STUB FILL: main/action/ac_repeatedly 20260624_184008 */

/* stub-fill: _init_param | missing_definition | ghidra_signature */
u8 _init_param(int param_1) {
    return 0;
}
