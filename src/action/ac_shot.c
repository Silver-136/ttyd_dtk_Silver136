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
extern AcShotVec vec3_802f9b68[];

s32 battleAcResult_Shot(void* wp) {
    return *(s32*)((s32)wp + 0x1CB8);
}

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


s32 battleAcMain_Shot(void* battleWork) {
    return 0;
}


void actionCommandDisp(f32 x, f32 y) {
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
