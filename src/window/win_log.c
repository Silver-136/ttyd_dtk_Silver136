#include "window/win_log.h"

void monosiriClose(void* work) {
    if (work != NULL) {
        *(s32*)work = 2;
    }
}

s32 monosiriCloseWait(void* work) {
    extern f32 float_500_80423b40;
    extern f32 float_neg300_80423b44;

    if (work == NULL) {
        return 1;
    }
    if (*(f32*)((s32)work + 0x10) > float_500_80423b40 &&
        *(f32*)((s32)work + 0x14) < float_neg300_80423b44) {
        return 1;
    }
    return 0;
}


u8 winLogMain(s32 pWin) {
    return 0;
}


u8 monoshiriGX(void) {
    return 0;
}


u8 capture(void) {
    return 0;
}


char* winGetMapTplName(void) {
    return 0;
}


u8 monosiriExit(int param_1) {
    return 0;
}


u8 monosiriInit(void) {
    return 0;
}


u8 winLogInit(void* pWin) {
    return 0;
}


u8 mapGX(s64 param_1, s64 param_2, void* pWin) {
    return 0;
}


u8 winLogDisp(s32 param_1, void* pWin, s32 param_3) {
    return 0;
}


void monosiri_disp(s32 param_1, void* monosiriWork) {
}


u8 winLogMain2(void* pWin) {
    return 0;
}


u8 monosiriMain(void* param_1) {
    return 0;
}


u8 winLogInit2(void* pWin) {
    return 0;
}


u8 winLogExit(void* pWin) {
    return 0;
}
