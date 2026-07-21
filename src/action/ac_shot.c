#include "action/ac_shot.h"

extern f32 float_neg200_80424840;
extern f32 float_0_80424828;
extern f32 float_1_8042482c;
extern f32 float_60_80424830;
extern f32 float_30_80424834;
extern f32 float_255_80424838;
extern f32 float_neg1_8042483c;
extern u32 dat_80424818;
extern u32 dat_8042481c;
extern u32 dat_80424820;
extern u32 dat_80424824;

typedef struct AcShotVec {
    f32 x;
    f32 y;
    f32 z;
} AcShotVec;

typedef struct AcShotDisp {
    u8 unk0[0x14];
    f32 x;
    f32 y;
    u8 unk1[4];
    s32 timer;
} AcShotDisp;

void* memset(void* dst, int value, u32 size);
f32 sinfd(f32 angle);
f32 cosfd(f32 angle);
void btlGetScreenPoint(AcShotVec* pos, AcShotVec* out);
f32 intplGetValue(s32 type, s32 current, f32 start, f32 end, s32 max);
f32 reviseAngle(f32 angle);
void btlDispTex4(s32 texture, AcShotVec* pos, AcShotVec* scale, AcShotVec* rot, u32* color);
void _ac_disp_init(void* wp);
void actionCommandDisp(f32 x, f32 y);
extern AcShotVec vec3_802f9b68[];


s32 battleAcMain_Shot(void* battleWork) {
    extern s32 BattlePadCheckNow(u32);
    extern s32 BattlePadCheckTrigger(u32);
    extern s32 irand(s32);
    extern s32 psndSFXOn(const char*);
    extern void psndSFXOff(s32);
    extern void dispEntry(s32, s32, void*, void*, f32);
    extern void _disp_target_mark(s32, void*);
    extern void _disp_target_mark_afterimage(s32, void*);
    extern void* BattleGetUnitPtr(void* battle, s32 index);
    extern void BtlUnit_GetPos(void* unit, f32* x, f32* y, f32* z);
    extern s32 BtlUnit_GetHeight(void* unit);
    extern const char str_SFX_AC_CURSOR_MOVE1_802f9bb0[];
    u8* bw = battleWork;
    u8* disp = bw + 0x1F20;
    s32 state = *(s32*)(bw + 0x1C9C);
    s32 done = 0;
    void* unit;

    switch (state) {
        case 0:
            _ac_disp_init(battleWork);
            *(s32*)(bw + 0x1CB8) = 1;
            *(f32*)(disp + 0x0C) = (f32)*(s32*)(bw + 0x1CD8);
            *(f32*)(disp + 0x10) = (f32)*(s32*)(bw + 0x1CDC);
            *(f32*)(disp + 0x14) = (f32)*(s32*)(bw + 0x1CE0);
            *(f32*)(disp + 0x18) = (f32)*(s32*)(bw + 0x1CE4);
            *(f32*)(disp + 0x24) =
                (f32)irand((s32)(*(f32*)(disp + 0x18) - *(f32*)(disp + 0x14))) +
                *(f32*)(disp + 0x14);
            *(f32*)(disp + 0x28) = (f32)*(s32*)(bw + 0x1CE8) * 0.1f;
            *(s32*)(disp + 0x08) = 0;
            unit = BattleGetUnitPtr(battleWork, *(s32*)(bw + 0x1CD0));
            BtlUnit_GetPos(unit, (f32*)(disp + 0x30),
                          (f32*)(disp + 0x34), (f32*)(disp + 0x38));
            *(f32*)(disp + 0x34) +=
                *(f32*)((s32)unit + 0x114) *
                (f32)BtlUnit_GetHeight(unit) * 0.5f;
            *(s32*)(disp + 0x64) = -1;
            *(s32*)(bw + 0x1C9C) = 99;
            break;

        case 99:
            *(s32*)disp = 0x168;
            *(s32*)(bw + 0x1C9C) = 1000;
            *(s32*)(disp + 0x64) = psndSFXOn(str_SFX_AC_CURSOR_MOVE1_802f9bb0);
            break;

        case 1000:
            if (BattlePadCheckTrigger(0x100) == 0) {
                *(s32*)disp -= 1;
                if (*(s32*)disp < 1) {
                    *(s32*)(bw + 0x1CB8) = 0;
                    *(s32*)(bw + 0x1C9C) = 1002;
                    done = 1;
                }
            } else {
                *(s32*)(bw + 0x1C9C) = 1001;
            }
            *(f32*)(disp + 0x24) += *(f32*)(disp + 0x28);
            *(f32*)(disp + 0x2C) += 8.0f;
            break;

        case 1001:
            if (BattlePadCheckNow(0x100) == 0) {
                *(s32*)(bw + 0x1CB8) = 2;
                *(s32*)(bw + 0x1C9C) = 1002;
                done = 1;
            }
            break;

        case 1002:
            *(s32*)(bw + 0x1C9C) = 1003;
            break;

        case 1003:
            break;

        case 1004:
            *(s32*)(disp + 0x3C) = 0x3C;
            *(s32*)(bw + 0x1C9C) = 1005;
            if (*(s32*)(disp + 0x64) != -1) {
                psndSFXOff(*(s32*)(disp + 0x64));
            }
            break;

        case 1005:
            *(s32*)(disp + 0x3C) -= 1;
            if (*(s32*)(disp + 0x3C) < 1) {
                *(s32*)(bw + 0x1C9C) = 1006;
            }
            break;

        case 1006:
            *(void**)(bw + 0x1CA0) = 0;
            *(void**)(bw + 0x1CA4) = 0;
            *(void**)(bw + 0x1CA8) = 0;
            *(void**)(bw + 0x1CAC) = 0;
            break;
    }

    if (!done) {
        state = *(s32*)(bw + 0x1C9C);
        if (state == 1000 || state == 1001) {
            dispEntry(2, 1, _disp_target_mark, battleWork, 900.0f);
        }
        dispEntry(2, 1, _disp_target_mark_afterimage, battleWork, 900.0f);
        return 1;
    }
    return 0;
}

s32 battleAcResult_Shot(void* wp) {
    return *(s32*)((s32)wp + 0x1CB8);
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void battleAcDisp_Shot(s32 unused, AcShotDisp* disp) {
    AcShotDisp* work;
    s32 state;
    s32 timer;

    work = (AcShotDisp*)((s32)disp + 0x1F20);
    state = *(s32*)((s32)disp + 0x1C9C);
    if (state >= 1000) {
        goto ge_1000;
    }
    if (state >= 101) {
        return;
    }
    if (state >= 99) {
        goto disp_in;
    }
    return;

ge_1000:
    if (state >= 1006) {
        return;
    }
    if (state >= 1004) {
        goto disp_out;
    }
disp_in:
    timer = work->timer;
    work->x = intplGetValue(4, 0x14 - timer, float_neg200_80424840, float_30_80424834, 0x14);
    actionCommandDisp(work->x, work->y);
    timer = work->timer;
    if (timer > 0) {
        work->timer = timer - 1;
    }
    return;

disp_out:
    timer = work->timer;
    if (timer >= 40) {
        work->x = intplGetValue(4, timer - 40, float_30_80424834, float_neg200_80424840, 0x14);
    } else {
        work->x = float_30_80424834;
    }
    actionCommandDisp(work->x, work->y);
    timer = work->timer;
    if (timer < 60) {
        work->timer = timer + 1;
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

void battleAcDelete_Shot(void* wp) {
    *(s32*)((s32)wp + 0x1C9C) = 1004;
}

void _ac_disp_init(void* wp) {
    void* disp = (void*)((s32)wp + 0x1F20);
    f32 y;
    f32 zero;
    s8 alpha;

    memset(disp, 0, 0x2C);

    y = float_neg200_80424840;
    zero = float_0_80424828;

    *(f32*)((s32)disp + 0x14) = y;

    alpha = -0x10;

    *(f32*)((s32)disp + 0x18) = zero;
    *(s32*)((s32)disp + 0x20) = 0x14;
    *(u32*)((s32)disp + 0xC) = dat_80424818;
    *(u8*)((s32)disp + 0x10) = 0;
    *(s8*)((s32)disp + 0x11) = alpha;
    *(s8*)((s32)disp + 0x12) = alpha;
    *(u8*)((s32)disp + 0x13) = 0;
    *(f32*)((s32)disp + 0x28) = zero;
}

void actionCommandDisp(f32 x, f32 y) {
    typedef f32 Mtx[3][4];
    extern void* _battleWorkPointer;
    extern void iconDispGx(void* pos, s32 icon, s32 pal, f32 scale);
    extern void iconDispGxCol(void* mtx, s32 icon, s32 pal, u32* color);
    extern void PSMTXTrans(Mtx m, f32 x, f32 y, f32 z);
    extern void PSMTXScale(Mtx m, f32 x, f32 y, f32 z);
    extern void PSMTXConcat(Mtx a, Mtx b, Mtx out);
    extern f32 float_25_80424844;
    extern f32 float_100_80424848;
    extern f32 float_0p01_8042484c;
    extern f32 float_88_80424850;
    extern f32 float_33_80424854;
    extern u32 unk_80429620;
    void* battleWork;
    u8* aimWork;
    u8* disp;
    AcShotVec pos;
    Mtx trans;
    Mtx scale;
    Mtx mtx;
    u32 color;
    f32 drawX;
    f32 cur;
    f32 target;

    battleWork = _battleWorkPointer;
    drawX = float_neg200_80424840 + x;
    aimWork = (u8*)((s32)battleWork + 0x1F4C);
    disp = (u8*)((s32)battleWork + 0x1F20);
    if ((*(u32*)((s32)battleWork + 0x1CC4) & 1) == 0) {
        pos = vec3_802f9b68[0];
        pos.x = drawX;
        pos.y = float_25_80424844 + y;
        iconDispGx(&pos, 0x10, 0x94, float_1_8042482c);

        target = *(f32*)(aimWork + 0x1C) / float_100_80424848;
        cur = *(f32*)(disp + 0x28);
        if (cur <= target) {
            cur += float_0p01_8042484c;
            *(f32*)(disp + 0x28) = cur;
            if (*(f32*)(disp + 0x28) > target) {
                *(f32*)(disp + 0x28) = target;
            }
        } else {
            cur -= float_0p01_8042484c;
            *(f32*)(disp + 0x28) = cur;
            if (*(f32*)(disp + 0x28) < target) {
                *(f32*)(disp + 0x28) = target;
            }
        }

        if (*(f32*)(disp + 0x28) >= float_1_8042482c) {
            disp[0xD] += disp[0x11];
            disp[0xE] += disp[0x12];
            if (disp[0xD] <= 0x80 && *(s8*)(disp + 0x11) < 0) {
                disp[0x11] = 0x10;
                disp[0x12] = 0x10;
            }
            if (disp[0xD] >= 0xF5 && *(s8*)(disp + 0x11) > 0) {
                disp[0x11] = -0x10;
                disp[0x12] = -0x10;
            }
        }

        PSMTXTrans(trans, drawX - (float_88_80424850 * (float_1_8042482c - *(f32*)(disp + 0x28))), float_33_80424854 + y, float_0_80424828);
        PSMTXScale(scale, *(f32*)(disp + 0x28), float_1_8042482c, float_1_8042482c);
        PSMTXConcat(trans, scale, mtx);
        color = unk_80429620;
        ((u8*)&color)[0] = disp[0xC];
        ((u8*)&color)[1] = disp[0xD];
        ((u8*)&color)[2] = disp[0xE];
        ((u8*)&color)[3] = disp[0xF];
        iconDispGxCol(mtx, 0x10, 0x95, &color);
    }
}

void _draw_target_mark(u32* color, f32 x, f32 y, f32 z, f32 angle) {
    AcShotVec pos;
    AcShotVec scale;
    AcShotVec rot;
    u32 color1;
    u32 color2;

    color1 = *color;
    pos = vec3_802f9b68[1];
    pos.x = x;
    pos.y = y;
    pos.z = z;
    rot = vec3_802f9b68[3];
    rot.z = reviseAngle(-angle);
    scale = vec3_802f9b68[2];
    btlDispTex4(0x58, &pos, &scale, &rot, &color1);

    color2 = *color;
    rot = vec3_802f9b68[5];
    rot.z = reviseAngle(angle);
    scale = vec3_802f9b68[4];
    btlDispTex4(0x57, &pos, &scale, &rot, &color2);
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void _disp_target_mark(s32 unused, void* battleWork) {
    AcShotVec pos;
    AcShotVec screen;
    u32 color;

    pos.x = *(f32*)((s32)battleWork + 0x1F7C) + (float_60_80424830 * sinfd(*(f32*)((s32)battleWork + 0x1F70)));
    pos.y = *(f32*)((s32)battleWork + 0x1F80) + (float_60_80424830 * cosfd(*(f32*)((s32)battleWork + 0x1F70)));
    pos.z = *(f32*)((s32)battleWork + 0x1F84);
    btlGetScreenPoint(&pos, &screen);
    color = dat_8042481c;
    _draw_target_mark(&color, screen.x, screen.y, screen.z, *(f32*)((s32)battleWork + 0x1F78));
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma no_register_save_helpers on
#pragma use_lmw_stmw on
void _disp_target_mark_afterimage(s32 unused, void* battleWork) {
    s32 ofs;
    s32 i;
    void* base;
    f32 zero;
    AcShotVec pos;
    AcShotVec screen;

    zero = float_0_80424828;
    base = (void*)((s32)battleWork + 0x1F4C);
    for (i = 0, ofs = 0; i < 3; i++, ofs += 4) {
        void* entry = (void*)((s32)base + ofs);

        if (*(f32*)((s32)entry + 0x40) >= zero && *(f32*)((s32)entry + 0x58) > zero) {
            *(f32*)((s32)entry + 0x58) -= float_1_8042482c;
            pos.x = *(f32*)((s32)base + 0x30) + (float_60_80424830 * sinfd(*(f32*)((s32)entry + 0x40)));
            pos.y = *(f32*)((s32)base + 0x34) + (float_60_80424830 * cosfd(*(f32*)((s32)entry + 0x40)));
            pos.z = *(f32*)((s32)base + 0x38);
            btlGetScreenPoint(&pos, &screen);
            if (*(f32*)((s32)entry + 0x58) >= float_30_80424834) {
                u32 color = dat_80424820;
                _draw_target_mark(&color, screen.x, screen.y, screen.z, *(f32*)((s32)entry + 0x4C));
            } else {
                u32 color = dat_80424824;
                s32 alpha = intplGetValue(0, (s32)(float_30_80424834 - *(f32*)((s32)entry + 0x58)), float_255_80424838, zero, 0x1E);
                ((u8*)&color)[3] = alpha;
                _draw_target_mark(&color, screen.x, screen.y, screen.z, *(f32*)((s32)entry + 0x4C));
            }
            if (*(f32*)((s32)entry + 0x58) <= zero) {
                *(f32*)((s32)entry + 0x40) = float_neg1_8042483c;
            }
        }
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on
