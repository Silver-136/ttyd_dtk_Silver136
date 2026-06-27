#include "manager/cardmgr.h"

extern void* wp;

void mountDetachCallback(void) {
    ;
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
    ;
}


u8 memcard_open(void) {
    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

void cardWrite(s32 fileNo) {
    extern void* memset(void* ptr, s32 value, u32 size);
    extern void* memcpy(void* dst, const void* src, u32 size);
    extern char* strcpy(char* dst, const char* src);
    extern void* marioGetPtr(void);
    extern s32 marioGetPartyId(void);
    extern s32 marioGetExtraPartyId(void);
    extern void* partyGetPtr(s32 id);
    extern s64 OSGetTime(void);
    extern void* pouchGetPtr(void);
    extern void* fbatGetPointer(void);
    extern void* gp;
    extern void* bdsw;
    extern void* yuwp;
    extern void _jdt;
    extern void* version;

    char* buf;
    void* mario;
    void* party;
    void* globals;
    s32 partyId;
    s64 time;
    s32 i;
    s32 checksum;

    if ((*(u16*)wp & 2) == 0) {
        *(s32*)((s32)wp + 0xE0) = 1;
        *(s32*)((s32)wp + 0xA0) = fileNo;
        *(s32*)((s32)wp + 0xD8) = 0;
        *(s32*)((s32)wp + 0x9C) = 0;
        *(u16*)wp |= 2;
        *(u16*)wp &= ~0x300;
        *(u16*)wp &= ~0x800;
        buf = (char*)(*(s32*)((s32)wp + 0xA8) + fileNo * 0x4000 + 0x2000);
        mario = marioGetPtr();
        memset(buf, 0, 0x4000);
        globals = gp;
        *(s32*)((s32)globals + 0x11D4) = *(s32*)((s32)mario + 0x8C);
        *(s32*)((s32)globals + 0x11D8) = *(s32*)((s32)mario + 0x90);
        *(s32*)((s32)globals + 0x11DC) = *(s32*)((s32)mario + 0x94);
        party = partyGetPtr(marioGetPartyId());
        if (party != 0) {
            partyId = *(u8*)((s32)party + 0x31);
        } else {
            partyId = *(u8*)((s32)mario + 0x247);
        }
        *(s32*)((s32)gp + 0x11E0) = (s8)partyId;
        party = partyGetPtr(marioGetExtraPartyId());
        if (party != 0) {
            partyId = *(u8*)((s32)party + 0x31);
        } else {
            partyId = *(u8*)((s32)mario + 0x248);
        }
        *(s32*)((s32)gp + 0x11E4) = (s8)partyId;
        time = OSGetTime();
        *(s32*)((s32)gp + 0x11EC) = (s32)time;
        *(s32*)((s32)gp + 0x11E8) = (s32)(time >> 32);
        *(s32*)((s32)gp + 0x11F0) += 1;
        *(u32*)gp &= ~1;
        *(u32*)gp &= ~2;
        *(s32*)((s32)gp + 0x1278) = 0;
        *(s32*)((s32)gp + 0x127C) = 0;
        *(s32*)((s32)gp + 0x1280) = 0;
        *(s32*)((s32)gp + 0x10) = 0;
        memcpy(buf + 8, gp, 0x13D8);
        memcpy(buf + 0x13E0, pouchGetPtr(), 0x5D4);
        memcpy(buf + 0x19B4, (void*)((s32)fbatGetPointer() + 0x4C), 0x500);
        memcpy(buf + 0x1EB4, bdsw, 0x124);
        memcpy(buf + 0x1FD8, yuwp, 0x258);
        memcpy(buf + 0x2230, (void*)((s32)&_jdt + 0x14), 0x2B);
        *(u16*)buf &= ~1;
        *(u16*)buf &= ~2;
        strcpy(buf + 0x3FF0, version);
        *(s32*)(buf + 0x3FF4) = 0x2260;
        *(s32*)(buf + 0x3FF8) = 0;
        *(s32*)(buf + 0x3FFC) = -1;
        checksum = 0;
        for (i = 0; i < 0x2260;) {
            checksum += *(u8*)(buf + i + 0);
            checksum += *(u8*)(buf + i + 1);
            checksum += *(u8*)(buf + i + 2);
            checksum += *(u8*)(buf + i + 3);
            checksum += *(u8*)(buf + i + 4);
            checksum += *(u8*)(buf + i + 5);
            checksum += *(u8*)(buf + i + 6);
            checksum += *(u8*)(buf + i + 7);
            i += 8;
            checksum += *(u8*)(buf + i + 0);
            checksum += *(u8*)(buf + i + 1);
            checksum += *(u8*)(buf + i + 2);
            checksum += *(u8*)(buf + i + 3);
            checksum += *(u8*)(buf + i + 4);
            checksum += *(u8*)(buf + i + 5);
            checksum += *(u8*)(buf + i + 6);
            checksum += *(u8*)(buf + i + 7);
            i += 8;
        }
        *(u32*)(buf + 0x3FF8) = checksum;
        *(u32*)(buf + 0x3FFC) = ~checksum;
    }
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

void cardWriteHeader(void) {
    extern void* memset(void* ptr, s32 value, u32 size);
    extern char* strcpy(char* dst, const char* src);
    extern s32 sprintf(char* buf, const char* fmt, ...);
    extern void* memcpy(void* dst, const void* src, u32 size);
    extern s64 OSGetTime(void);
    extern void OSTicksToCalendarTime(u32 hi, u32 lo, void* calendar);
    extern void* gp;
    extern u8 _mariost_icon_tex[];

    struct Calendar {
        s32 sec;
        s32 min;
        s32 hour;
        s32 mday;
        s32 mon;
        s32 year;
        s32 wday;
        s32 yday;
        s32 msec;
        s32 usec;
    } calendar;
    register u8* icon;
    register char* buf;
    s64 time;
    s32 checksum;
    s32 i;

    icon = _mariost_icon_tex;
    if ((*(u16*)wp & 2) == 0) {
        *(s32*)((s32)wp + 0xE0) = 2;
        *(s32*)((s32)wp + 0xA0) = 0;
        *(s32*)((s32)wp + 0xD8) = 0;
        *(s32*)((s32)wp + 0x9C) = 0;
        *(u16*)wp |= 2;
        *(u16*)wp &= ~0x300;
        *(u16*)wp &= ~0x800;
        buf = *(char**)((s32)wp + 0xA8);
        memset(buf, 0, 0x1E46);
        strcpy(buf, (char*)(icon + 0x1E14));
        time = OSGetTime();
        OSTicksToCalendarTime((u32)(time >> 32), (u32)time, &calendar);
        sprintf(buf + 0x20, (char*)(icon + 0x1E20), calendar.mon + 1, calendar.mday);
        memcpy(buf + 0x40, icon + 0x600, 0x1800);
        memcpy(buf + 0x1840, icon, 0x400);
        memcpy(buf + 0x1C40, icon + 0x400, 0x200);
        *(u16*)(buf + 0x1E40) = ((u32)(-*(s32*)((s32)gp + 0x1294)) | (u32)*(s32*)((s32)gp + 0x1294)) >> 31;
        *(s16*)(buf + 0x1E42) = *(s32*)((s32)gp + 0x1274);
        *(s16*)(buf + 0x1E44) = *(s32*)((s32)gp + 0x11B8);
        *(s32*)(buf + 0x1FF8) = 0;
        *(s32*)(buf + 0x1FFC) = -1;
        checksum = 0;
        for (i = 0; i < 0x1E40; i += 0x10) {
            checksum += *(u8*)(buf + i + 0);
            checksum += *(u8*)(buf + i + 1);
            checksum += *(u8*)(buf + i + 2);
            checksum += *(u8*)(buf + i + 3);
            checksum += *(u8*)(buf + i + 4);
            checksum += *(u8*)(buf + i + 5);
            checksum += *(u8*)(buf + i + 6);
            checksum += *(u8*)(buf + i + 7);
            checksum += *(u8*)(buf + i + 8);
            checksum += *(u8*)(buf + i + 9);
            checksum += *(u8*)(buf + i + 10);
            checksum += *(u8*)(buf + i + 11);
            checksum += *(u8*)(buf + i + 12);
            checksum += *(u8*)(buf + i + 13);
            checksum += *(u8*)(buf + i + 14);
            checksum += *(u8*)(buf + i + 15);
        }
        for (; i < 0x1E46; i++) {
            checksum += *(u8*)(buf + i);
        }
        *(u32*)(buf + 0x1FF8) = checksum;
        *(u32*)(buf + 0x1FFC) = ~checksum;
    }
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

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
    ;
}


u8 cardReadAll(void) {
    return 0;
}


void cardFormat(void) {
    ;
}
