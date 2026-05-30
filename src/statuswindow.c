#include "statuswindow.h"

void* wp;
s32 valueCheck(void);
void statusGetValue(void* out);

void statusWinDispOff(void) {
    *(u16*)((s32)wp + 4) |= 0x1000;
}

void statusWinDispOn(void) {
    *(u16*)((s32)wp + 4) &= ~0x1000;
}

void N_statusClearBlink(void) {
    *(s32*)((s32)wp + 0x80) = 0;
}

void statusWinForceCloseClear(void) {
    *(u16*)((s32)wp + 4) &= ~0xA0;
}

void statusFPBlink(s32 value) {
    *(u32*)((s32)wp + 0x80) |= 2;
    *(s32*)((s32)wp + 0x88) = value;
}

void statusMarioHPBlink(s32 value) {
    *(u32*)((s32)wp + 0x80) |= 1;
    *(s32*)((s32)wp + 0x88) = value;
}

void statusPartyHPBlink(s32 value) {
    *(u32*)((s32)wp + 0x80) |= 8;
    *(s32*)((s32)wp + 0x88) = value;
}

s32 statusWinCheck(void) {
    s32 value = *(u16*)((s32)wp + 4) & 0x11;
    return ((u32)(-value) | (u32)value) >> 31;
}

s32 statusWinCheckUpdate(void) {
    s32 value = valueCheck();
    return ((u32)(-value) | (u32)value) >> 31;
}

void statusAPBlink(s32 value) {
    *(s32*)((s32)wp + 0x80) = 0;
    *(u32*)((s32)wp + 0x80) |= 4;
    *(s32*)((s32)wp + 0x88) = value;
}

void statusWinClose(void) {
    *(u16*)((s32)wp + 4) |= 0xA;
    *(u16*)((s32)wp + 4) &= ~5;
}

void statusWinOpen(void) {
    *(u16*)((s32)wp + 4) &= ~0xA;
    *(u16*)((s32)wp + 4) |= 5;
}

void statusWinForceUpdateCoin(void) {
    u8 values[0x14];
    statusGetValue(values);
    *(s16*)((s32)wp + 0x60) = *(s16*)(values + 0x10);
}

void statusWinForceClose(void) {
    *(u16*)((s32)wp + 4) &= ~5;
    *(u16*)((s32)wp + 4) &= ~0x50;
    *(u16*)((s32)wp + 4) |= 0xA0;
}

void statusWinForceOpen(void) {
    *(u16*)((s32)wp + 4) &= ~0xA;
    *(u16*)((s32)wp + 4) &= ~0xA0;
    *(u16*)((s32)wp + 4) |= 0x50;
}


u8 statusWinDisp(void) {
    return 0;
}


u8 statusWinMain(void) {
    return 0;
}


u8 valueUpdate(void) {
    return 0;
}


u8 gaugeDisp(void) {
    return 0;
}


s32 valueCheck(void) {
    return 0;
}


void statusGetValue(void* out) {
}


void statusWinReInit(void) {
}


void statusWinForceOff(void) {
}


void statusWinForceUpdate(void) {
}


u8 statusWinInit(void) {
    return 0;
}


u8 statusGetApPos(float* param_1) {
    return 0;
}
