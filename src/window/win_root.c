#include "window/win_root.h"

extern f32 float_neg1000_804233a0;
void* sort_data[8];

s32 winSortWait(void* win) {
    s32 value = *(s32*)((s32)win + 0x17C);
    return (u32)(-value | value) >> 31;
}

s32 compare_func1(void* a, void* b) {
    if (*(u8*)a > *(u8*)b) {
        return 1;
    }
    if (*(u8*)a < *(u8*)b) {
        return -1;
    }
    return 0;
}

s32 compare_func1_r(void* a, void* b) {
    if (*(u8*)a < *(u8*)b) {
        return 1;
    }
    if (*(u8*)a > *(u8*)b) {
        return -1;
    }
    return 0;
}

s32 compare_func5(void* a, void* b) {
    if (*(u8*)a > *(u8*)b) {
        return 1;
    }
    if (*(u8*)a < *(u8*)b) {
        return -1;
    }
    return 0;
}

s32 compare_func5_r(void* a, void* b) {
    if (*(u8*)a < *(u8*)b) {
        return 1;
    }
    if (*(u8*)a > *(u8*)b) {
        return -1;
    }
    return 0;
}

void winSortEntry(void* win, s32 type, f32 a, f32 b) {
    void* entry = sort_data[type];
    s32 index = 0;
    f32 neg = float_neg1000_804233a0;

    while (*(void**)entry != 0) {
        entry = (void*)((s32)entry + 8);
        index++;
    }

    *(s32*)((s32)win + 0x184) = type;
    *(s32*)((s32)win + 0x178) = index;
    *(s32*)((s32)win + 0x174) = 0;
    *(s32*)((s32)win + 0x180) = 0;
    *(f32*)((s32)win + 0x164) = neg;
    *(f32*)((s32)win + 0x16C) = a;
    *(f32*)((s32)win + 0x168) = b;
    *(f32*)((s32)win + 0x170) = b;
    *(s32*)((s32)win + 0x17C) = 1;
}


void winKirinukiGX(double param_1, double param_2, double param_3, double param_4, void* pWin, int param_6) {
}


u8 winMailGX(void) {
    return 0;
}


void winHakoGX(double param_1, double param_2, void* pWin, int param_4) {
}


void winBookGX(double param_1, double param_2, void* pWin, int param_4) {
}


u8 winWazaGX(void) {
    return 0;
}


s32 winRootMain(void* pWin) {
    return 0;
}


u8 winSortGX(void* pWin) {
    return 0;
}


u8 winMailDisp(void* pWin) {
    return 0;
}


u8 winNameGX(void) {
    return 0;
}


u8 winMsgDisp(double x, double y, void* pWin) {
    return 0;
}


u8 winBgGX(void) {
    return 0;
}


u8 winKageGX(void) {
    return 0;
}


u8 winBgMain(void* pWin) {
    return 0;
}


void winRootDisp(s32 cameraId, void* work) {
}


u8 winSortMain(void* pWin) {
    return 0;
}


u8 winMsgMain(void* pWin) {
    return 0;
}


u8 sort_4_3_func(void* pWin) {
    return 0;
}


u8 sort_5_3_func(void* pWin) {
    return 0;
}


u8 winZClear(void) {
    return 0;
}


u8 compare_func3_r(int param_1, int param_2) {
    return 0;
}


u8 compare_func3(int param_1, int param_2) {
    return 0;
}


int compare_func2_r(int param_1, int param_2) {
    return 0;
}


int compare_func2(int param_1, int param_2) {
    return 0;
}


u8 sort_7_3_func(void* pWin) {
    return 0;
}


u8 sort_7_2_func(void* pWin) {
    return 0;
}


u8 winMsgEntry(void* pWin, s32 param_2, char* param_3, s32 param_4) {
    return 0;
}


u8 sort_6_2_func(void* pWin) {
    return 0;
}


u8 sort_6_1_func(void* pWin) {
    return 0;
}


u8 sort_5_2_func(void* pWin) {
    return 0;
}


u8 sort_5_1_func(void* pWin) {
    return 0;
}


u8 sort_8_2_func(void* pWin) {
    return 0;
}


u8 sort_8_1_func(void* pWin) {
    return 0;
}


u8 sort_3_3_func(void* pWin) {
    return 0;
}


u8 sort_1_2_func(void* pWin) {
    return 0;
}


u8 sort_1_1_func(void* pWin) {
    return 0;
}


u8 sort_7_1_func(void* pWin) {
    return 0;
}


u8 sort_4_2_func(void* pWin) {
    return 0;
}


u8 sort_4_1_func(void* pWin) {
    return 0;
}


u8 compare_func6_1_r(short* param_1, short* param_2) {
    return 0;
}


u8 compare_func6_1(short* param_1, short* param_2) {
    return 0;
}


u8 compare_func5_1_r(short* param_1, short* param_2) {
    return 0;
}


u8 compare_func5_1(short* param_1, short* param_2) {
    return 0;
}


u8 sort_3_2_func(void* pWin) {
    return 0;
}


u8 sort_3_1_func(void* pWin) {
    return 0;
}


u8 N_compare_func4_1_r(int param_1, int param_2) {
    return 0;
}


u8 N_compare_func4_1(int param_1, int param_2) {
    return 0;
}


u8 sort_2_2_func(void* pWin) {
    return 0;
}


u8 sort_2_1_func(void* pWin) {
    return 0;
}


u8 compare_func4(int param_1, int param_2) {
    return 0;
}


u8 compare_func4_r(int param_1, int param_2) {
    return 0;
}


int compare_func6_2_r(short* param_1, short* param_2) {
    return 0;
}


int compare_func6_2(short* param_1, short* param_2) {
    return 0;
}


int compare_func5_2_r(short* param_1, short* param_2) {
    return 0;
}


int compare_func5_2(short* param_1, short* param_2) {
    return 0;
}


int N_compare_func4_2_r(int param_1, int param_2) {
    return 0;
}


int N_compare_func4_2(int param_1, int param_2) {
    return 0;
}


u8 winHalfBookGX(double x, double y, void* file, int param_4) {
    return 0;
}
