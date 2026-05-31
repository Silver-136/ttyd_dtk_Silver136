#include "effect/n64/eff_n64.h"

extern void* wp;
extern void GXInitTexObj();
extern void TEXGetGXTexObjFromPalette();
extern void UnpackTexPalette();
extern void DVDMgrClose();

static u8 dummy_425[4];

u8 effTexSetupN64(void) {
    return 0;
}


u8 effSetVtxDescN64(u32 param_1) {
    return 0;
}


void* effGetSetN64(char* param_1) {
    return 0;
}


u8 effGetTexObjN64(int param_1, u32* param_2) {
    void* work = wp;

    if (*(s32*)((s32)work + 4) == 0) {
        GXInitTexObj(param_2, dummy_425, 1, 1, 0, 0, 0, 0);
    } else {
        TEXGetGXTexObjFromPalette(*(void**)work, param_1, param_2);
    }
}


u8 tri2(s16 param_1, s16 param_2, s16 param_3, s32 param_4, s16 param_5, s16 param_6, s16 param_7) {
    return 0;
}


void _callback_tpl(void* unused, void* fileInfo) {
    void* file = *(void**)((s32)fileInfo + 0x2C);

    UnpackTexPalette(*(void**)wp);
    DVDMgrClose(file);
    *(s32*)((s32)wp + 4) = 1;
}


int effTblRandN64(int param_1, int param_2) {
    return 0;
}


u8 effInit64(void) {
    memset(wp, 0, 8);
    *(s32*)((s32)wp + 4) = 0;
}


void tri1(s16 param_1, s16 param_2, s16 param_3) {
    volatile u16* fifo = (volatile u16*)0xCC008000;

    *fifo = param_1;
    *fifo = param_1;
    *fifo = param_1;
    *fifo = param_2;
    *fifo = param_2;
    *fifo = param_2;
    *fifo = param_3;
    *fifo = param_3;
    *fifo = param_3;
}
