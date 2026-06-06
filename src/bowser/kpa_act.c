#include "bowser/kpa_act.h"

s32 kpa_chg_pose(void* evt) {
    extern s32 evtGetValue(void* evt, s32 value);
    extern void marioChgPose(s32 pose);

    s32* args = *(s32**)((s32)evt + 0x18);
    marioChgPose(evtGetValue(evt, args[0]));
    return 2;
}

void kpaAddScore(s32 score) {
    extern void pouchAddKpaScore(s32 score);

    if (score > 10000) {
        score = 10000;
    }
    pouchAddKpaScore(score);
}

void kpaAddCoin(s32 coin) {
    extern s32 pouchAddKpaCoin(s32 coin);
    extern void pouchAddKpaScore(s32 score);

    if (pouchAddKpaCoin(coin) != 0) {
        pouchAddKpaScore(1000);
    }
    {
        s32 score = coin * 500;

        if (score > 10000) {
            score = 10000;
        }
        pouchAddKpaScore(score);
    }
}

s32 kpa_score_disp_main(void) {
    s32 i;
    s32 offset;

    extern void* ksdp;
    extern f32 float_1_804272d4;
    extern f32 float_0_804272d0;
    extern void kpaScoreDisp(void);
    extern void dispEntry(s32 cameraId, s32 renderMode, void* callback, s32 param, f32 priority);

    {
        f32 one = float_1_804272d4;

    offset = 0;
    for (i = 0; i < 25; i++) {
        void* entry = (void*)((s32)ksdp + offset);
        if (*(u8*)((s32)entry + 8) != 0) {
            *(f32*)((s32)entry + 0x10) += one;
            *(u8*)((s32)ksdp + offset + 8) -= 1;
        }
        offset += 0x18;
        entry = (void*)((s32)ksdp + offset);
        if (*(u8*)((s32)entry + 8) != 0) {
            *(f32*)((s32)entry + 0x10) += one;
            *(u8*)((s32)ksdp + offset + 8) -= 1;
        }
        offset += 0x18;
    }
    }

    dispEntry(4, 2, kpaScoreDisp, 0, float_0_804272d0);
    return 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void kpaScoreDisp(void) {
    u32 color;
    f32 trans[12];
    f32 scale[12];
    s32 offset;
    s32 i;

    extern void* ksdp;
    extern u32 dat_804272c0;
    extern f32 float_0p5_804272c4;
    extern f32 float_300_804272d8;
    extern void PSMTXScale(void* m, f32 x, f32 y, f32 z);
    extern void PSMTXTrans(void* m, f32 x, f32 y, f32 z);
    extern void PSMTXConcat(void* a, void* b, void* ab);
    extern void FontDrawStart(void);
    extern void FontDrawColor(void* color);
    extern void FontDrawStringMtx(void* mtx, void* str);

    PSMTXScale(scale, float_0p5_804272c4, float_0p5_804272c4, float_0p5_804272c4);
    FontDrawStart();
    color = dat_804272c0;
    FontDrawColor(&color);

    offset = 0;
    for (i = 0; i < 50; i++) {
        if (*(u8*)((s32)ksdp + offset + 8) != 0) {
            PSMTXTrans(trans, *(f32*)((s32)ksdp + offset + 0xC), *(f32*)((s32)ksdp + offset + 0x10), float_300_804272d8);
            PSMTXConcat(trans, scale, trans);
            FontDrawStringMtx(trans, (void*)((s32)ksdp + offset));
        }
        offset += 0x18;
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

