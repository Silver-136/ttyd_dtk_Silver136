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

