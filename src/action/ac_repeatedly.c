#include "action/ac_repeatedly.h"

#include "system.h"

void BattleAcGaugeSeDelete(void* wp);
static void actionCommandDisp(f32 x, f32 y);

void _init_param(void* battleWork) {
    char* battle;
    char* extra;
    char* disp;
    s32 type;
    s32 value;

    battle = (char*)battleWork;
    extra = battle + 0x1F4C;
    disp = battle + 0x1F20;
    extra[0x2C] = 0;
    *(s32*)(extra + 0x0C) = 0;
    type = *(s32*)(battle + 0x1CC8);

    switch (type) {
        case 2: value = 0x200; goto set_same;
        case 3: value = 0x400; goto set_same;
        case 4: value = 0x800; goto set_same;
        case 5: value = 0x20; goto set_same;
        case 8:
            *(s32*)(extra + 0x0C) = 0x100;
        case 6:
            value = 0x40000;
            goto set_same;
        case 10:
            *(u32*)(battle + 0xEF4) |= 0x20000000;
            break;
        case 0x0B:
            *(s32*)(extra + 0x00) = 0x100;
            *(s32*)(extra + 0x04) = 0x100;
            *(s32*)(extra + 0x08) = 0x200;
            break;
        case 0x0C:
            *(s32*)(extra + 0x00) = 0x400;
            *(s32*)(extra + 0x04) = 0x400;
            *(s32*)(extra + 0x08) = 0x800;
            break;
        case 0x10:
            *(s32*)(extra + 0x00) = 0x100;
            *(s32*)(extra + 0x04) = 0x100;
            *(s32*)(extra + 0x08) = 0x200;
            *(s16*)(extra + 0x22) = irand(0x1E) + 0x3C;
            break;
        default:
            value = 0x100;
set_same:
            *(s32*)(extra + 0x00) = value;
            *(s32*)(extra + 0x04) = value;
            *(s32*)(extra + 0x08) = value;
            break;
    }

    *(s16*)(extra + 0x28) = *(s32*)(battle + 0x1CCC);
    *(s16*)(extra + 0x26) = *(s32*)(battle + 0x1CCC);
    *(f32*)(extra + 0x10) = (f32)*(s32*)(battle + 0x1CD0);
    *(f32*)(extra + 0x14) = (f32)*(s32*)(battle + 0x1CD4) / 10.0f;
    if (*(f32*)(extra + 0x14) != 0.0f) {
        *(f32*)(extra + 0x10) = (f32)irand(100);
    }
    *(f32*)(extra + 0x18) = (f32)*(s32*)(battle + 0x1CE4);
    *(f32*)(disp + 0x28) = 0.01f * *(f32*)(extra + 0x18);
    *(s16*)(extra + 0x20) = 0;
    *(s16*)(extra + 0x24) = *(s32*)(battle + 0x1CDC);
    *(f32*)(extra + 0x1C) = 0.01f * (f32)*(s32*)(battle + 0x1CE0);
    extra[0x2A] = 0xB2;
    if (*(s16*)(battle + 0x1D18) > 0) {
        *(s16*)(battle + 0x1D18) -= 1;
        extra[0x2B] = 1;
    } else {
        extra[0x2B] = 0;
    }
    *(s32*)(battle + 0x1CE8) = 0;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 battleAcMain_Repeatedly(register void* battleWork) {
    extern u32 BattleActionCommandGetDifficulty(void* battleWork);
    extern s32 BattlePadCheckNow(u32 buttons);
    extern void BattleAcGaugeSeInit(void);
    extern u32 OSGetResetButtonState(void);
    extern s32 SIProbe(u32 chan);
    extern s32 BattlePadCheckTrigger(u32 buttons);
    extern void BattleAcGaugeSeUpdate(f32 value);
    extern s32 _power_table[];
    extern u32 dat_80422a90;
    extern f32 float_neg300_80422ab8;
    extern f32 float_0_80422ac0;
    extern f32 float_0p01_80422ac4;
    extern f32 float_neg1_80422ac8;
    extern f32 float_100_80422aa4;

    register void* disp;
    register void* extra;
    void* unit;
    register u32 difficulty;
    register s32 forced;
    register s32 pressed;
    s32 state;
    s32 acParam;
    s16 count;

    extra = (void*)((s32)battleWork + 0x1F4C);
    disp = (void*)((s32)battleWork + 0x1F20);
    difficulty = BattleActionCommandGetDifficulty(battleWork);
    unit = *(void**)((s32)battleWork + 0x1C90);
    forced = 0;
    if (*(u8*)((s32)unit + 0x307) != 0) {
        forced = 1;
    }
    state = *(s32*)((s32)battleWork + 0x1C9C);
    pressed = 0;

    if (state == 1000) {
        goto state_1000;
    }
    if (state >= 1000) {
        if (state == 1003) {
            goto state_1003;
        }
        if (state >= 1003) {
            if (state >= 1005) {
                goto done;
            }
            goto state_1004;
        }
        if (state >= 1002) {
            goto state_1002;
        }
        goto done;
    }
    if (state == 99) {
        goto done;
    }
    if (state >= 99) {
        if (state >= 101) {
            goto done;
        }
        goto state_100;
    }
    if (state != 0) {
        goto done;
    }

state_0:
            memset(disp, 0, 0x2C);
            *(f32*)((s32)disp + 0x14) = float_neg300_80422ab8;
            *(f32*)((s32)disp + 0x18) = float_0_80422ac0;
            *(s32*)((s32)disp + 0x20) = 0x14;
            *(u32*)((s32)disp + 0x0C) = dat_80422a90;
            *(u8*)((s32)disp + 0x10) = 0;
            *(s8*)((s32)disp + 0x11) = -0x10;
            *(s8*)((s32)disp + 0x12) = -0x10;
            *(u8*)((s32)disp + 0x13) = 0;
            *(f32*)((s32)disp + 0x28) = float_0_80422ac0;
            *(s32*)((s32)battleWork + 0x1CB8) = 1;
            *(s16*)((s32)extra + 0x28) = 0;
            _init_param(battleWork);
            *(s32*)((s32)battleWork + 0x1C9C) = 99;
            goto done;
state_100:
            if (((*(u32*)((s32)battleWork + 0x1C94) & 1) == 0 ||
                 (*(u32*)((s32)unit + 0x27C) & 0x10) == 0) &&
                *(u32*)((s32)extra + 0x0C) != 0 &&
                BattlePadCheckNow(*(u32*)((s32)extra + 0x0C)) == 0) {
                goto done;
            }
            BattleAcGaugeSeInit();
            *(s32*)((s32)battleWork + 0x1C9C) = 1000;
            *(s32*)((s32)battleWork + 0x1CE8) = 1;
            goto state_1000;
state_1002:
            *(s32*)((s32)extra + 0x30) = 0x3C;
            *(s32*)((s32)battleWork + 0x1C9C) = 1003;
            goto done;
state_1003:
            *(s32*)((s32)extra + 0x30) -= 1;
            if (*(s32*)((s32)extra + 0x30) <= 0) {
                *(s32*)((s32)battleWork + 0x1C9C) = 1004;
            }
            goto done;
state_1004:
            *(void**)((s32)battleWork + 0x1CA0) = 0;
            *(void**)((s32)battleWork + 0x1CA4) = 0;
            *(void**)((s32)battleWork + 0x1CA8) = 0;
            *(void**)((s32)battleWork + 0x1CAC) = 0;
            goto done;

state_1000:
    acParam = *(s32*)((s32)battleWork + 0x1CC8);
    switch (acParam) {
    case 10:
        if (*(u8*)((s32)extra + 0x2C) != 0) {
            if (OSGetResetButtonState() == 0) {
                pressed = 1;
                *(u8*)((s32)extra + 0x2C) = 0;
                *(s16*)((s32)extra + 0x20) += 1;
            }
        } else if (OSGetResetButtonState() != 0) {
            pressed = 1;
            *(u8*)((s32)extra + 0x2C) = 1;
            *(s16*)((s32)extra + 0x20) += 1;
        }
        break;
    case 9:
        if (*(u8*)((s32)extra + 0x2C) != 0) {
            if (SIProbe(0) == 8) {
                pressed = 1;
                *(u8*)((s32)extra + 0x2C) = 0;
                *(s16*)((s32)extra + 0x20) += 1;
            }
        } else if ((SIProbe(0) != 8) && (SIProbe(0) != 0x80)) {
            pressed = 1;
            *(u8*)((s32)extra + 0x2C) = 1;
            *(s16*)((s32)extra + 0x20) += 1;
        }
        break;
    case 11:
    case 12:
        if ((*(u16*)((s32)extra + 0x20) & 1) == 0) {
            *(u32*)extra = *(u32*)((s32)extra + 4);
        } else {
            *(u32*)extra = *(u32*)((s32)extra + 8);
        }
        if (BattlePadCheckTrigger(*(u32*)extra) != 0) {
            pressed = 1;
            *(s16*)((s32)extra + 0x20) += 1;
        }
        break;
    case 0x10:
        *(u16*)((s32)extra + 0x22) -= 1;
        if (*(u16*)((s32)extra + 0x22) == 0) {
            *(u16*)((s32)extra + 0x22) = (u16)irand(0x1E) + 0x3C;
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
        break;
    default:
        if (BattlePadCheckTrigger(*(u32*)extra) != 0) {
            pressed = 1;
            *(s16*)((s32)extra + 0x20) += 1;
        }
        break;
    }

    if (forced && !pressed) {
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
done:
    return 1;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

s32 _power_table[7] = {
    55, 50, 45, 40, 35, 30, 25,
};


s32 battleAcResult_Repeatedly(void* wp) {
    return *(s32*)((s32)wp + 0x1CB8);
}
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void battleAcDisp_Repeatedly(s32 param_1, void* battleWork) {
    typedef struct AcRepeatedlyDispWork {
        u8 pad_00[0x14];
        f32 x;
        f32 y;
        u8 pad_1C[0x4];
        s32 timer;
    } AcRepeatedlyDispWork;
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
static void actionCommandDisp(f32 x, f32 y) {
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
    extern Vec vec3_802f3760[];
    extern f32 float_neg200_80422a94;
    extern f32 float_70_80422a98;
    extern f32 float_1_80422a9c;
    extern f32 float_0p5_80422aa0;
    extern f32 float_100_80422aa4;
    extern f32 float_0p0125_80422aa8;
    extern f32 float_neg288_80422aac;
    extern f32 float_176_80422ab0;
    extern f32 float_45_80422ab4;

    register void* battleWork;
    register Vec* templates;
    void* disp;
    void* extra;
    Vec gaugePos;
    Vec dimPos;
    Vec selectedPos;
    Vec offPos;
    Vec onPos;
    f32 target;
    f32 ratio;
    f32 baseX;
    f32 baseY;
    s32 iconOn;
    s32 iconOff;
    s32 flags;
    s32 percent;
    s32 i;
    s32 offset;
    s32 selected;

    templates = vec3_802f3760;
    battleWork = _battleWorkPointer;
    camGetPtr(8);
    disp = (void*)((s32)battleWork + 0x1F20);
    extra = (void*)((s32)battleWork + 0x1F4C);

    if (*(s32*)((s32)battleWork + 0x1CC8) != 0x10) {
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
        if (((*(s32*)disp / 7) & 1) == 1) {
            onPos = templates[0];
            onPos.x = float_neg200_80422a94 + x;
            onPos.y = float_70_80422a98 + y;
            iconDispGx(&onPos, 0x10, iconOff, float_1_80422a9c);
        } else {
            offPos = templates[1];
            offPos.x = float_neg200_80422a94 + x;
            offPos.y = float_70_80422a98 + y;
            iconDispGx(&offPos, 0x10, iconOn, float_1_80422a9c);
        }
    } else {
        i = 0;
        baseX = float_neg200_80422a94 + x;
        baseY = float_70_80422a98 + y;
        offset = 0;
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
            if (selected) {
                if (((*(s32*)disp / 7) & 1) == 1) {
                    iconOn = iconOff;
                }
                selectedPos = templates[2];
                selectedPos.x = baseX + (f32)offset;
                selectedPos.y = baseY;
                iconDispGx(&selectedPos, 0x10, iconOn, float_1_80422a9c);
            } else {
                dimPos = templates[3];
                dimPos.x = baseX + (f32)offset;
                dimPos.y = baseY;
                iconDispGx(&dimPos, 0x10, iconOff, float_0p5_80422aa0);
            }
            i++;
            offset += 0x28;
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

    if ((*(u32*)((s32)battleWork + 0x1CC4) & 2) == 0) {
        percent = (s32)*(f32*)((s32)extra + 0x10);
    } else {
        percent = 100;
    }
    BattleAcDrawGauge(ratio, (s32)x, (s32)y, *(u8*)((s32)extra + 0x2A), 2, percent, 100, 100, flags);

    gaugePos = templates[4];
    gaugePos.x = (float_neg288_80422aac + x) +
            (float_176_80422ab0 * *(f32*)((s32)extra + 0x10)) / float_100_80422aa4;
    gaugePos.y = float_45_80422ab4 + y;
    iconDispGx(&gaugePos, 0x10, 0x9E, float_1_80422a9c);

    *(s32*)disp += 1;
    *(s32*)((s32)disp + 4) += 1;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on
