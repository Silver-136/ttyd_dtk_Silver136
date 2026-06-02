#include "countdown.h"

extern void* gp;
void* wp;

void* memset(void* dst, int value, u32 size);
void psndSFXOff(s32 id);

void countDownSetRestartTime(s32 time) {
    *(s32*)((s32)wp + 0x1C) = time;
}

void countDownFlagOff(u16 flag) {
    *(u16*)wp &= ~flag;
}

void countDownFlagOn(u16 flag) {
    *(u16*)wp |= flag;
}

void countDownStop(void) {
    *(u16*)wp |= 8;
    psndSFXOff(*(s32*)((s32)wp + 0x38));
}

void countDownEnd(void) {
    if (*(u16*)wp & 1) {
        *(s32*)((s32)wp + 0x30) = 2;
    }
}

s32 countDownGetStatus(void) {
    u32 flags = *(u16*)wp;
    if (flags & 1) {
        goto active;
    }
    return -1;
active:
    return (flags >> 1) & 1;
}

void countDownInit(void) {
    wp = (void*)((s32)gp + 0x68);
    memset(wp, 0, 0x40);
}
