#include "nameent.h"

void nameEntOff(void) {
    extern void* wp;

    *(s32*)((s32)wp + 4) = 3;
}

s32 nameEntIsCancel(void) {
    extern void* wp;

    return (*(u16*)wp >> 1) & 1;
}

void nameEntReInit(void) {
    extern void* wp;

    *(s32*)((s32)wp + 4) = 3;
}

void nameEntInit(void) {
    extern void* wp;
    extern void* memset(void* ptr, s32 value, u32 size);

    memset(wp, 0, 0x64);
}

s32 nameEntWait(void) {
    extern void* wp;
    s32 value;

    *(u16*)wp |= 1;
    value = *(s32*)((s32)wp + 8);
    return (u32)((0x65 - value) | (value - 0x65)) >> 31;
}
