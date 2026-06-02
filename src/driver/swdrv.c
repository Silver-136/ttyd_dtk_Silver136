#include "driver/swdrv.h"
extern void* gp;
extern void* memset(void* dst, int value, unsigned long size);


u8 _swByteGet(s32 index) {
    index = (s32)gp + index;
    return *(u8*)(index + 0xDB8);
}

void _swByteSet(s32 index, u8 value) {
    index = (s32)gp + index;
    *(u8*)(index + 0xDB8) = value;
}

void _swClear(s32 index) {
    s32 word;
    s32 bit;

    word = index / 32;
    bit = index % 32;

    *(u32*)((s32)gp + 0xD78 + word * 4) &= ~(1 << bit);
}

s32 _swGet(s32 index) {
    s32 word;
    s32 bit;
    s32 value;

    word = index / 32;
    bit = index % 32;

    value = *(s32*)((s32)gp + 0xD78 + word * 4);
    value &= 1 << bit;

    return value != 0;
}

void _swSet(s32 index) {
    s32 word;
    s32 bit;
    u32 mask;
    void* globals;

    word = index / 32;
    bit = index % 32;
    globals = gp;

    mask = 1 << bit;
    *(u32*)((s32)globals + 0xD78 + word * 4) |= mask;
}
s32 swByteGet(s32 index) {
    if (index == 0) {
        return *(s32*)((s32)gp + 0x174);
    }

    return *((u8*)gp + index + 0x578);
}

void swByteSet(s32 index, s32 value) {
    if (index == 0) {
        *(s32*)((s32)gp + 0x174) = value;
        return;
    }

    *((u8*)gp + index + 0x578) = value;
}

void swClear(s32 index) {
    s32 word;
    s32 bit;

    word = index / 32;
    bit = index % 32;

    *(u32*)((s32)gp + 0x178 + word * 4) &= ~(1 << bit);
}

s32 swGet(s32 index) {
    s32 word;
    s32 bit;
    s32 value;

    word = index / 32;
    bit = index % 32;

    value = *(s32*)((s32)gp + 0x178 + word * 4);
    value &= 1 << bit;

    return value != 0;
}

void swSet(s32 index) {
    s32 word;
    s32 bit;
    u32 mask;
    void* globals;

    word = index / 32;
    bit = index % 32;
    globals = gp;

    mask = 1 << bit;
    *(u32*)((s32)globals + 0x178 + word * 4) |= mask;
}

void swReInit(void) {
    memset((void*)((s32)gp + 0xD78), 0, 0x40);
    memset((void*)((s32)gp + 0xDB8), 0, 0x400);
}

void swInit(void) {
    memset((void*)((s32)gp + 0x178), 0, 0x400);
    memset((void*)((s32)gp + 0x578), 0, 0x800);
    memset((void*)((s32)gp + 0xD78), 0, 0x40);
    memset((void*)((s32)gp + 0xDB8), 0, 0x400);

    *(s32*)((s32)gp + 0x174) = 0;
}
