#include "window/win_log.h"

extern void* mapalloc_base_ptr;
extern const char str_msg_menu_kiroku_map_802f6708[];
extern const f64 double_to_int_802f6b70;
extern f32 float_neg128_80423c34;
extern f32 float_500_80423b40;
extern f32 float_neg300_80423b44;
extern f32 float_45_80423bdc;
extern f32 float_0p5_80423af8;
extern f32 float_8_80423b04;
extern f32 float_320_80423c0c;
extern f32 float_neg240_80423bf0;
extern f32 float_neg200_80423c30;

void fileFree(void* ptr);
void animPoseRelease(s32 poseId);
void smartFree(void* ptr);
void _mapFree(void* heap, void* ptr);
void* _mapAlloc(void* heap, u32 size);
void* memset(void* ptr, int value, u32 size);
s32 swByteGet(s32 index);
void winMsgEntry(void* pWin, s32 param_2, char* msg, s32 param_4);

typedef struct MonosiriWork {
    s32 state;
    s32 param;
    void* file0;
    void* file1;
    f32 closeX;
    f32 closeY;
    f32 x;
    f32 y;
    s32 pose0;
    s32 pose1;
    void* buffer;
    s32 flags;
} MonosiriWork;

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
    char* base = (char*)str_msg_menu_kiroku_map_802f6708;

    if (swByteGet(0) < 0x13) {
        return base + 0x4F0;
    }
    if (swByteGet(0) < 0x41) {
        return base + 0x4FC;
    }
    if (swByteGet(0) < 0x79) {
        return base + 0x508;
    }
    if (swByteGet(0) < 0xAF) {
        return base + 0x514;
    }
    if (swByteGet(0) < 0xDE) {
        return base + 0x520;
    }
    if (swByteGet(0) < 0x110) {
        return base + 0x52C;
    }
    if (swByteGet(0) >= 0x15C) {
        return base + 0x544;
    }
    return base + 0x538;
}


void monosiriExit(void* work) {
    if (work != NULL) {
        if (*(void**)((s32)work + 0x8) != NULL) {
            fileFree(*(void**)((s32)work + 0x8));
            *(void**)((s32)work + 0x8) = NULL;
        }
        if (*(void**)((s32)work + 0xC) != NULL) {
            fileFree(*(void**)((s32)work + 0xC));
            *(void**)((s32)work + 0xC) = NULL;
        }
        if (*(s32*)((s32)work + 0x20) != -1) {
            animPoseRelease(*(s32*)((s32)work + 0x20));
            *(s32*)((s32)work + 0x20) = -1;
        }
        if (*(s32*)((s32)work + 0x24) != -1) {
            animPoseRelease(*(s32*)((s32)work + 0x24));
            *(s32*)((s32)work + 0x24) = -1;
        }
        if (*(void**)((s32)work + 0x28) != NULL) {
            smartFree(*(void**)((s32)work + 0x28));
            *(void**)((s32)work + 0x28) = NULL;
        }
        _mapFree(mapalloc_base_ptr, work);
    }
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
MonosiriWork* monosiriInit(s32 param, f32 x, f32 y) {
    MonosiriWork* work = _mapAlloc(mapalloc_base_ptr, sizeof(MonosiriWork));

    memset(work, 0, sizeof(MonosiriWork));
    work->state = 0;
    work->param = param;
    work->x = x;
    work->y = y;
    work->closeX = float_500_80423b40;
    work->closeY = float_neg300_80423b44;
    work->flags = 0;
    work->pose0 = -1;
    work->pose1 = -1;
    return work;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


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


void winLogInit2(void* pWin) {
    f32 f45;
    f32 f05;
    f32 f8;
    f32 f320;
    f32 f240;
    f32 f300;
    f32 f200;
    f32 a;
    f32 b;
    f32 value;

    *(f32*)((s32)pWin + 0x158) = float_neg128_80423c34;
    f45 = float_45_80423bdc;
    a = f45 * (f32)*(s32*)((s32)pWin + 0xD70);
    b = f45 * (f32)*(s32*)((s32)pWin + 0xD6C);
    f05 = float_0p5_80423af8;
    f8 = float_8_80423b04;
    f320 = float_320_80423c0c;
    f240 = float_neg240_80423bf0;
    f300 = float_neg300_80423b44;
    f200 = float_neg200_80423c30;
    value = (a * f05) - b;
    *(f32*)((s32)pWin + 0x15C) = f8 + value;

    *(f32*)((s32)pWin + 0xE88) = f320;
    *(f32*)((s32)pWin + 0xE84) = f320;
    *(f32*)((s32)pWin + 0xE90) = f240;
    *(f32*)((s32)pWin + 0xE8C) = f240;
    value = (f32)(*(s32*)((s32)pWin + 0xEA0) * 0xE8);
    *(f32*)((s32)pWin + 0xEA8) = value;
    *(f32*)((s32)pWin + 0xEA4) = value;
    *(f32*)((s32)pWin + 0xEB0) = f200;
    *(f32*)((s32)pWin + 0xEAC) = f200;
    *(f32*)((s32)pWin + 0xEB8) = f300;
    *(f32*)((s32)pWin + 0xEB4) = f300;

    *(f32*)((s32)pWin + 0xF88) = f320;
    *(f32*)((s32)pWin + 0xF84) = f320;
    *(f32*)((s32)pWin + 0xF90) = f240;
    *(f32*)((s32)pWin + 0xF8C) = f240;
    value = (f32)(*(s32*)((s32)pWin + 0xFA0) * 0xE8);
    *(f32*)((s32)pWin + 0xFA8) = value;
    *(f32*)((s32)pWin + 0xFA4) = value;
    *(f32*)((s32)pWin + 0xFB0) = f200;
    *(f32*)((s32)pWin + 0xFAC) = f200;
    *(f32*)((s32)pWin + 0xFB8) = f300;
    *(f32*)((s32)pWin + 0xFB4) = f300;
    winMsgEntry(pWin, 0, *(char**)((s32)pWin + 0xD90 + (*(s32*)((s32)pWin + 0xD6C) << 5)), 0);
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
u8 winLogExit(void* pWin) {
    extern void effSoftDelete(s32 id);
    s32 i;

    if (*(void**)((s32)pWin + 0x30) != NULL) {
        fileFree(*(void**)((s32)pWin + 0x30));
        *(void**)((s32)pWin + 0x30) = NULL;
    }
    if (*(s32*)((s32)pWin + 0xE1C) != -1) {
        animPoseRelease(*(s32*)((s32)pWin + 0xE1C));
        *(s32*)((s32)pWin + 0xE1C) = -1;
    }
    for (i = 0; i < 7; i++) {
        effSoftDelete(*(s32*)((s32)pWin + 0xE44));
        pWin = (void*)((s32)pWin + 4);
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

