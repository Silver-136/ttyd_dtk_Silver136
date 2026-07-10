#include "effect/eff_fukidashi.h"


u8 effFukidashiDisp(void) {
    return 0;
}


u8 calc_pos(int param_1, int param_2) {
    return 0;
}


u8 effFukidashiEntry(void) {
    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
u8 effFukidashiMain(void* effect) {
    typedef struct LocalVec3 {
        f32 x;
        f32 y;
        f32 z;
    } LocalVec3;
    extern void effDelete(void* effect);
    extern f32 dispCalcZ(LocalVec3* pos);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern u8 effFukidashiDisp(s32 camera, void* effect);
    extern u8 calc_pos(void* work, s32 flag);
    extern LocalVec3 vec3_802f3684;
    extern s8* anim_data_table[];
    extern s8* anim_data_table_2[];
    extern f32 float_22p5_80422940;
    extern f32 float_0_804228c0;
    extern f32 float_9_80422944;
    extern f32 float_neg90_80422948;
    void* work;
    LocalVec3 dispPos;
    LocalVec3 pos;
    s8* anim;
    s32 value;
    s32 frame;
    s32 kind;

    work = *(void**)((s32)effect + 0xC);
    pos = vec3_802f3684;
    pos.x = *(f32*)((s32)work + 0x4);
    pos.y = *(f32*)((s32)work + 0x8);
    pos.z = *(f32*)((s32)work + 0xC);
    dispPos = pos;
    kind = *(s32*)work;

    if (*(s32*)((s32)work + 0x40) == 0) {
        anim = anim_data_table[kind];
    } else {
        anim = anim_data_table_2[kind];
    }
    frame = *(s32*)((s32)work + 0x34);
    value = anim[frame / 2];
    *(s32*)((s32)work + 0x34) = frame + 1;
    *(s32*)((s32)work + 0x38) = value;

    if (value < 0) {
        if (value == -1) {
            effDelete(effect);
            return 0;
        }
        *(s32*)((s32)work + 0x34) = (-10 - value) * 2;
        if (*(s32*)((s32)work + 0x40) == 0) {
            anim = anim_data_table[kind];
        } else {
            anim = anim_data_table_2[kind];
        }
        frame = *(s32*)((s32)work + 0x34);
        value = anim[frame / 2];
        *(s32*)((s32)work + 0x34) = frame + 1;
        *(s32*)((s32)work + 0x38) = value;
    }

    if (kind == 6) {
        if (*(s32*)((s32)work + 0x54) == 0) {
            *(f32*)((s32)work + 0x24) += float_22p5_80422940;
            if (*(f32*)((s32)work + 0x24) >= float_0_804228c0) {
                *(f32*)((s32)work + 0x24) = float_0_804228c0;
                *(s32*)((s32)work + 0x54) += 1;
            }
        } else if (*(s32*)((s32)work + 0x54) == 0x78) {
            *(f32*)((s32)work + 0x24) -= float_9_80422944;
            if (*(f32*)((s32)work + 0x24) <= float_neg90_80422948) {
                effDelete(effect);
                return 0;
            }
        } else {
            *(s32*)((s32)work + 0x54) += 1;
        }
    } else if (*(s32*)((s32)work + 0x40) > 0) {
        if (*(s32*)((s32)work + 0x30) < 10000) {
            *(s32*)((s32)work + 0x30) -= 1;
        }
        if (*(s32*)((s32)work + 0x30) < 0) {
            effDelete(effect);
            return 0;
        }
    }

    if (*(void**)((s32)work + 0x44) != 0) {
        calc_pos(work, 1);
    }
    if (kind == 4) {
        dispEntry(*(s32*)((s32)work + 0x4C), 2, effFukidashiDisp, effect, dispCalcZ(&dispPos));
    } else {
        dispEntry(*(s32*)((s32)work + 0x4C), 8, effFukidashiDisp, effect, dispCalcZ(&dispPos));
    }
    return 0;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

