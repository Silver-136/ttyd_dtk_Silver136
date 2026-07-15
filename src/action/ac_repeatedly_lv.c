#include "action/ac_repeatedly_lv.h"

void BattleAcGaugeSeDelete(void* wp);
f32 intplGetValue(s32 type, f32 start, f32 end, s32 time, s32 duration);
extern f32 float_neg300_80424ba4;
extern f32 float_30_80424ba8;

s32 battleAcResult_RepeatedlyLv(void* wp) {
    return *(s32*)((s32)wp + 0x1CB8);
}

void battleAcDelete_RepeatedlyLv(void* wp) {
    *(s32*)((s32)wp + 0x1C9C) = 1002;
    BattleAcGaugeSeDelete(wp);
}


void actionCommandDisp(f32 x,f32 y) {
    extern void* g_BattleWork;extern void iconDispGx(void*,s32,f32,f32,f32,u32);extern void BattleAcDrawGauge(void*);
    u8* battle=g_BattleWork;u8* disp=battle+0x1F20;u8* extra=battle+0x1F4C;u32 gauge[4];f32 target=*(f32*)(extra+0x14)*0.01f;f32* ratio=(f32*)(disp+0x28);s32 icon=*(u8*)(extra+0x25);
    (*(s32*)(disp+0x20))++;
    if((*ratio)<target){*ratio+=(*(u32*)(extra+8)&4)?0.02f:0.01f;if(*ratio>target)*ratio=target;}else if(*ratio>target){*ratio-=0.01f;if(*ratio<target)*ratio=target;}
    if(*(u32*)(extra+8)&0x10)*ratio=target;
    if((*(s32*)(disp+0x20)&8)==0){iconDispGx(battle,icon,x,y,1.0f,0xFFFFFFFF);if(*(u32*)(extra+8)&2)iconDispGx(battle,icon+1,x+36.0f,y,1.0f,0xFFFFFFFF);}
    gauge[0]=*(u32*)(disp+0xC);gauge[1]=*(u32*)(disp+0x10);gauge[2]=*(u32*)ratio;gauge[3]=*(u32*)(extra+8);BattleAcDrawGauge(gauge);
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 battleAcMain_RepeatedlyLv(void* battleWork) {
    extern void* memset(void* dst, s32 value, u32 size);
    extern void _init_param(void* battleWork);
    extern u32 BattlePadCheckNow(u32 buttons);
    extern u32 BattlePadCheckTrigger(u32 buttons);
    extern void BattlePadCheckUp(u32 buttons);
    extern void BattleAcGaugeSeInit(void);
    extern void BattleAcGaugeSeUpdate(f32 value);
    extern void battleAcDelete_RepeatedlyLv(void* battleWork);
    extern s32 irand(s32 max);
    extern u32 dat_80424b68;
    extern f32 float_neg300_80424ba4;
    extern f32 float_0_80424ba0;
    extern f32 float_100_80424b84;
    extern f32 float_0p4_80424bac;
    extern f32 float_1p5_80424bb0;

    void* unit;
    void* disp;
    void* extra;
    s32 autoCommand;
    s32 pressed;
    s32 state;
    s32 param;
    s32 value;
    s16 count;

    pressed = 0;
    unit = *(void**)((s32)battleWork + 0x1C90);
    autoCommand = 0;
    if (*(u8*)((s32)unit + 0x307) != 0) {
        autoCommand = 1;
    }

    disp = (void*)((s32)battleWork + 0x1F20);
    extra = (void*)((s32)battleWork + 0x1F4C);
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

    if (((*(u32*)((s32)battleWork + 0x1C94) & 1) == 0) ||
        ((*(u32*)((s32)unit + 0x27C) & 0x10) == 0)) {
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
        } else if ((*(u32*)((s32)battleWork + 0x1C94) & 0x20) == 0) {
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
                if ((*(u32*)((s32)battleWork + 0x1C94) & 0x20) != 0) {
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
    } else if (((*(u32*)((s32)battleWork + 0x1C94) & 2) == 0) ||
               (--*(s32*)((s32)battleWork + 0x1C98) < 0)) {
        *(u32*)((s32)battleWork + 0x1CB8) &= ~1;
        *(u32*)((s32)battleWork + 0x1CB8) |= 2;
        value = irand(100);
        *(s16*)((s32)extra + 0x18) = 0;
        while (value < 0) {
            value -= *(u8*)((s32)extra + 0x26);
            if (value <= -1) {
                break;
            }
            *(s16*)((s32)extra + 0x18) += 1;
        }
        *(s32*)((s32)battleWork + 0x1CF0) = *(s16*)((s32)extra + 0x18);
        *(s32*)((s32)battleWork + 0x1CF4) += 1;
        battleAcDelete_RepeatedlyLv(battleWork);
        return 0;
    }

    return 1;
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

typedef struct AcRepeatedlyLvDispWork {
    u8 pad_00[0x14];
    f32 x;
    f32 y;
    u8 pad_1C[0x4];
    s32 timer;
} AcRepeatedlyLvDispWork;

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void battleAcDisp_RepeatedlyLv(s32 unused, void* battleWork) {
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


/* CHATGPT STUB FILL: main/action/ac_repeatedly_lv 20260624_184008 */

/* stub-fill: _init_param | missing_definition | ghidra_signature */
u8 _init_param(int param_1) {
    return 0;
}
