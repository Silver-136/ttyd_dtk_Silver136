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

s32 kpa_score_disp_init(s32 evt) {
    extern void* evtEntryType(void* script, s32 priority, s32 flags, s32 typeMask);
    extern void* _mapAlloc(void* heap, u32 size);
    extern s32 kpa_score_disp_evt;
    extern void* mapalloc_base_ptr;
    extern volatile void* ksdp;
    extern volatile u32 vec3_802feb58[];

    void* entry;
    volatile u32* vec;
    u32 temp;
    u32* pos;
    s32 base;
    s32 count;
    s32 outer;
    s32 rem;

    entry = evtEntryType(&kpa_score_disp_evt, *(u8*)(evt + 0xB), 0, *(u8*)(evt + 0xC));

    *(u32*)((s32)entry + 0x160) = *(u32*)(evt + 0x160);
    *(u32*)((s32)entry + 0x170) = *(u32*)(evt + 0x170);
    *(u32*)((s32)entry + 0x09C) = *(u32*)(evt + 0x09C);
    *(u32*)((s32)entry + 0x0A0) = *(u32*)(evt + 0x0A0);
    *(u32*)((s32)entry + 0x0A4) = *(u32*)(evt + 0x0A4);
    *(u32*)((s32)entry + 0x0A8) = *(u32*)(evt + 0x0A8);
    *(u32*)((s32)entry + 0x0AC) = *(u32*)(evt + 0x0AC);
    *(u32*)((s32)entry + 0x0B0) = *(u32*)(evt + 0x0B0);
    *(u32*)((s32)entry + 0x0B4) = *(u32*)(evt + 0x0B4);
    *(u32*)((s32)entry + 0x0B8) = *(u32*)(evt + 0x0B8);
    *(u32*)((s32)entry + 0x0BC) = *(u32*)(evt + 0x0BC);
    *(u32*)((s32)entry + 0x0C0) = *(u32*)(evt + 0x0C0);
    *(u32*)((s32)entry + 0x0C4) = *(u32*)(evt + 0x0C4);
    *(u32*)((s32)entry + 0x0C8) = *(u32*)(evt + 0x0C8);
    *(u32*)((s32)entry + 0x0CC) = *(u32*)(evt + 0x0CC);
    *(u32*)((s32)entry + 0x0D0) = *(u32*)(evt + 0x0D0);
    *(u32*)((s32)entry + 0x0D4) = *(u32*)(evt + 0x0D4);
    *(u32*)((s32)entry + 0x0D8) = *(u32*)(evt + 0x0D8);
    *(u32*)((s32)entry + 0x0DC) = *(u32*)(evt + 0x0DC);
    *(u32*)((s32)entry + 0x0E0) = *(u32*)(evt + 0x0E0);
    *(u32*)((s32)entry + 0x0E4) = *(u32*)(evt + 0x0E4);
    *(u32*)((s32)entry + 0x154) = *(u32*)(evt + 0x154);
    *(u32*)((s32)entry + 0x158) = *(u32*)(evt + 0x158);

    ksdp = _mapAlloc(mapalloc_base_ptr, 0x4B0);
    vec = vec3_802feb58;

    count = 0;
    base = 0;
    outer = 6;
    do {
        *(u8*)((s32)ksdp + base) = 0;
        *(u8*)((s32)ksdp + base + 0x08) = 0;
        temp = vec[1];
        pos = (u32*)((s32)ksdp + base + 0x0C);
        pos[0] = vec[0];
        pos[1] = temp;
        pos[2] = vec[2];

        *(u8*)((s32)ksdp + base + 0x18) = 0;
        *(u8*)((s32)ksdp + base + 0x20) = 0;
        temp = vec[1];
        pos = (u32*)((s32)ksdp + base + 0x24);
        pos[0] = vec[0];
        pos[1] = temp;
        pos[2] = vec[2];

        *(u8*)((s32)ksdp + base + 0x30) = 0;
        *(u8*)((s32)ksdp + base + 0x38) = 0;
        temp = vec[1];
        pos = (u32*)((s32)ksdp + base + 0x3C);
        pos[0] = vec[0];
        pos[1] = temp;
        pos[2] = vec[2];

        *(u8*)((s32)ksdp + base + 0x48) = 0;
        *(u8*)((s32)ksdp + base + 0x50) = 0;
        temp = vec[1];
        pos = (u32*)((s32)ksdp + base + 0x54);
        pos[0] = vec[0];
        pos[1] = temp;
        pos[2] = vec[2];

        *(u8*)((s32)ksdp + base + 0x60) = 0;
        *(u8*)((s32)ksdp + base + 0x68) = 0;
        temp = vec[1];
        pos = (u32*)((s32)ksdp + base + 0x6C);
        pos[0] = vec[0];
        pos[1] = temp;
        pos[2] = vec[2];

        *(u8*)((s32)ksdp + base + 0x78) = 0;
        *(u8*)((s32)ksdp + base + 0x80) = 0;
        temp = vec[1];
        pos = (u32*)((s32)ksdp + base + 0x84);
        pos[0] = vec[0];
        pos[1] = temp;
        pos[2] = vec[2];

        *(u8*)((s32)ksdp + base + 0x90) = 0;
        *(u8*)((s32)ksdp + base + 0x98) = 0;
        temp = vec[1];
        pos = (u32*)((s32)ksdp + base + 0x9C);
        pos[0] = vec[0];
        pos[1] = temp;
        pos[2] = vec[2];

        *(u8*)((s32)ksdp + base + 0xA8) = 0;
        *(u8*)((s32)ksdp + base + 0xB0) = 0;
        temp = vec[1];
        pos = (u32*)((s32)ksdp + base + 0xB4);
        pos[0] = vec[0];
        pos[1] = temp;
        pos[2] = vec[2];

        base += 0xC0;
        count += 8;
        outer--;
    } while (outer != 0);

    rem = 0x32 - count;
    base = count * 0x18;
    if (count < 0x32) {
        do {
            *(u8*)((s32)ksdp + base) = 0;
            *(u8*)((s32)ksdp + base + 0x08) = 0;
            temp = vec[1];
            pos = (u32*)((s32)ksdp + base + 0x0C);
            base += 0x18;
            pos[0] = vec[0];
            pos[1] = temp;
            pos[2] = vec[2];
            rem--;
        } while (rem != 0);
    }

    return 2;
}



/* CHATGPT STUB FILL: main/bowser/kpa_act 20260624_184128 */

/* stub-fill: kpaAddScorePos | missing_definition | ghidra_signature */
u8 kpaAddScorePos(int param_1, float* param_2) {
    return 0;
}

/* stub-fill: kpaAddCoinPos | missing_definition | ghidra_signature */
u8 kpaAddCoinPos(int param_1, float* param_2) {
    return 0;
}
