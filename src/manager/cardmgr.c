#include "manager/cardmgr.h"

extern void* wp;

void mountDetachCallback(void) {
}

void* cardGetCode(void) {
    return *(void**)((s32)wp + 0x9C);
}

void* cardGetFilePtr(void) {
    return *(void**)((s32)wp + 0xA8);
}

void checkCallback(s32 chan, s32 result) {
    *(s32*)((s32)wp + 0x9C) = result;
    *(u16*)wp &= ~1;
}

void createCallback(s32 chan, s32 result) {
    *(s32*)((s32)wp + 0x9C) = result;
    *(u16*)wp &= ~1;
}

void formatCallback(s32 chan, s32 result) {
    *(s32*)((s32)wp + 0x9C) = result;
    *(u16*)wp &= ~1;
}

void readCallback(s32 chan, s32 result) {
    *(s32*)((s32)wp + 0x9C) = result;
    *(u16*)wp &= ~1;
}

void statusCallback(s32 chan, s32 result) {
    *(s32*)((s32)wp + 0x9C) = result;
    *(u16*)wp &= ~1;
}

void writeCallback(s32 chan, s32 result) {
    *(s32*)((s32)wp + 0x9C) = result;
    *(u16*)wp &= ~1;
}

void mountAttachCallback(s32 chan, s32 result) {
    *(s32*)((s32)wp + 0x9C) = result;
    if (result == -6 || result == -13) {
        *(s32*)((s32)wp + 0x9C) = 0;
    }
    *(u16*)wp &= ~1;
}

void unk_800b2c08(void) {
    *(u16*)wp &= 0x7FFF;
    *(u16*)wp &= 0x3FFF;
}

s32 unk_800b2bdc(void) {
    volatile u16* flags = (volatile u16*)wp;

    if ((*flags & 0x8000) != 0 && (*flags & 0x4000) != 0) {
        return 1;
    }
    return 0;
}

s32 cardIsExec(void) {
    void* work = wp;

    if (work == 0) {
        return 0;
    }
    if ((*(volatile u16*)work & 2) != 0) {
        return 1;
    }
    if ((*(volatile u16*)work & 1) != 0) {
        return 1;
    }
    return ((u32)(-*(s32*)((s32)work + 0xEC)) | (u32)*(s32*)((s32)work + 0xEC)) >> 31;
}

void unk_800b2c2c(void) {
    *(s32*)((s32)wp + 0xE4) = *(s32*)((s32)wp + 8);
    *(s32*)((s32)wp + 0xEC) = 10;
    *(u16*)wp |= 0x8000;
}
