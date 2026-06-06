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


u8 write_main(void) {
    return 0;
}


u8 read_all_main(void) {
    return 0;
}


u8 read_main(void) {
    return 0;
}


u8 create_main(void) {
    return 0;
}


u8 cardMain(void) {
    return 0;
}


u8 cardInit(void) {
    return 0;
}


u8 write_header_main(void) {
    return 0;
}


void cardBufReset(void) {
}


u8 memcard_open(void) {
    return 0;
}


u8 cardWrite(int param_1) {
    return 0;
}


void cardWriteHeader(void) {
}


void cardErase(s32 fileNo) {
    extern void* memset(void* ptr, s32 value, u32 size);
    extern char* strcpy(char* dst, const char* src);
    extern void* version;
    void* buf;
    s32 i;
    u32 checksum;

    if ((*(u16*)wp & 2) == 0) {
        *(s32*)((s32)wp + 0xE0) = 1;
        *(s32*)((s32)wp + 0xA0) = fileNo;
        *(s32*)((s32)wp + 0xD8) = 0;
        *(s32*)((s32)wp + 0x9C) = 0;
        *(u16*)wp |= 2;
        *(u16*)wp &= ~0x300;
        *(u16*)wp |= 0x800;
        buf = *(void**)((s32)wp + 0xB0);
        memset(buf, 0, 0x2260);
        *(u16*)buf |= 1;
        strcpy((char*)((s32)buf + 0x3FF0), version);
        *(s32*)((s32)buf + 0x3FF4) = 0x2260;
        *(s32*)((s32)buf + 0x3FF8) = 0;
        *(s32*)((s32)buf + 0x3FFC) = -1;
        checksum = 0;
        for (i = 0; i < 0x2260; i++) {
            checksum += *(u8*)((s32)buf + i);
        }
        *(u32*)((s32)buf + 0x3FF8) = checksum;
        *(u32*)((s32)buf + 0x3FFC) = ~checksum;
    }
}

void cardCopy2Main(s32 fileNo) {
    extern void* gp;
    extern void* bdsw;
    extern void* yuwp;
    extern void _jdt;
    extern char str_123_80421134[];
    extern void* memcpy(void* dst, const void* src, u32 size);
    extern char* strcpy(char* dst, const char* src);
    extern void* pouchGetPtr(void);
    extern void* fbatGetPointer(void);
    extern s64 OSGetTime(void);
    void* src;
    s32 flag;
    s32 old168;
    s32 old160;
    s32 old16c;
    s32 old1274;
    s32 old1294;
    s32 old11b8;
    s64 time;

    src = (void*)(*(s32*)((s32)wp + 0xA8) + (fileNo << 14) + 0x2000);
    flag = *(u32*)gp & 8;
    old168 = *(s32*)((s32)gp + 0x168);
    old160 = *(s32*)((s32)gp + 0x160);
    old16c = *(s32*)((s32)gp + 0x16C);
    old1274 = *(s32*)((s32)gp + 0x1274);
    old1294 = *(s32*)((s32)gp + 0x1294);
    old11b8 = *(s32*)((s32)gp + 0x11B8);
    memcpy(gp, (void*)((s32)src + 8), 0x13D8);
    *(s32*)((s32)gp + 0x168) = old168;
    memcpy(pouchGetPtr(), (void*)((s32)src + 0x13E0), 0x5D4);
    memcpy((void*)((s32)fbatGetPointer() + 0x4C), (void*)((s32)src + 0x19B4), 0x500);
    memcpy(bdsw, (void*)((s32)src + 0x1EB4), 0x124);
    memcpy(yuwp, (void*)((s32)src + 0x1FD8), 0x258);
    memcpy((void*)((s32)&_jdt + 0x14), (void*)((s32)src + 0x2230), 0x2B);
    *(s32*)((s32)gp + 0x1278) = 0;
    *(s32*)((s32)gp + 0x127C) = 0;
    *(s32*)((s32)gp + 0x1280) = 0;
    *(s32*)((s32)gp + 0x10) = 0;
    time = OSGetTime();
    *(s32*)((s32)gp + 0x34) = (s32)time;
    *(s32*)((s32)gp + 0x30) = (s32)(time >> 32);
    strcpy((char*)((s32)gp + 0x13C), str_123_80421134);
    *(s32*)((s32)gp + 0x15C) = 0;
    *(s32*)((s32)gp + 0x1274) = old1274;
    *(s32*)((s32)gp + 0x1294) = old1294;
    *(s32*)((s32)gp + 0x11B8) = old11b8;
    *(s32*)((s32)gp + 0x160) = old160;
    *(s32*)((s32)gp + 8) = 0;
    *(s32*)((s32)gp + 0xC) = 0;
    if (flag != 0) {
        *(u32*)gp |= 8;
    } else {
        *(u32*)gp &= ~8;
    }
    *(s32*)((s32)gp + 0x16C) = old16c;
}

void cardCopy(s32 fileNo, s32 dataNo) {
    extern void* memcpy(void* dst, const void* src, u32 size);

    if ((*(u16*)wp & 2) == 0) {
        *(s32*)((s32)wp + 0xE0) = 1;
        *(s32*)((s32)wp + 0xA0) = dataNo;
        *(s32*)((s32)wp + 0xD8) = 0;
        *(s32*)((s32)wp + 0x9C) = 0;
        *(u16*)wp |= 2;
        *(u16*)wp &= ~0x300;
        *(u16*)wp |= 0x800;
        memcpy(*(void**)((s32)wp + 0xB0), (void*)(*(s32*)((s32)wp + 0xA8) + (fileNo << 14) + 0x2000), 0x4000);
    }
}

void cardCreate(void) {
}


u8 cardReadAll(void) {
    return 0;
}


void cardFormat(void) {
}
