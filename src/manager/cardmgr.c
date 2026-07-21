#include "manager/cardmgr.h"

extern void* wp;

void* cardGetFilePtr(void) {
    return *(void**)((s32)wp + 0xA8);
}

void cardBufReset(void) {
    extern void CARDCancel(void* fileInfo);
    extern void* memset(void* ptr, s32 value, u32 size);
    extern char* strcpy(char* dst, const char* src);
    extern s64 OSGetTime(void);
    extern void OSTicksToCalendarTime(u32 hi, u32 lo, void* calendar);
    extern s32 sprintf(char* buf, const char* fmt, ...);
    extern void* memcpy(void* dst, const void* src, u32 size);
    extern u8 _mariost_icon_tex[];
    extern void* gp;
    extern void* version;

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
    char* buf;
    char* dataBuf;
    s64 time;
    s32 i;
    s32 slot;
    s32 checksum;
    u8* icon = _mariost_icon_tex;
    s32 count;

    if ((*(u16*)wp & 1) != 0) {
        CARDCancel((void*)((s32)wp + 0x1C));
        count = 1000000;
        while (((*(u16*)wp & 1) != 0) && (count != 0)) {
            count--;
        }
        *(u16*)wp &= ~1;
    }

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

    for (slot = 0; slot < 4; slot++) {
        dataBuf = (char*)(*(s32*)((s32)wp + 0xA8) + 0x2000 + slot * 0x4000);
        memset(dataBuf, 0, 0x2260);
        *(u16*)dataBuf |= 1;
        strcpy(dataBuf + 0x3FF0, version);
        *(s32*)(dataBuf + 0x3FF4) = 0x2260;
        *(s32*)(dataBuf + 0x3FF8) = 0;
        *(s32*)(dataBuf + 0x3FFC) = -1;
        checksum = 0;
        for (i = 0; i < 0x2260;) {
            checksum += *(u8*)(dataBuf + i + 0);
            checksum += *(u8*)(dataBuf + i + 1);
            checksum += *(u8*)(dataBuf + i + 2);
            checksum += *(u8*)(dataBuf + i + 3);
            checksum += *(u8*)(dataBuf + i + 4);
            checksum += *(u8*)(dataBuf + i + 5);
            checksum += *(u8*)(dataBuf + i + 6);
            checksum += *(u8*)(dataBuf + i + 7);
            i += 8;
            checksum += *(u8*)(dataBuf + i + 0);
            checksum += *(u8*)(dataBuf + i + 1);
            checksum += *(u8*)(dataBuf + i + 2);
            checksum += *(u8*)(dataBuf + i + 3);
            checksum += *(u8*)(dataBuf + i + 4);
            checksum += *(u8*)(dataBuf + i + 5);
            checksum += *(u8*)(dataBuf + i + 6);
            checksum += *(u8*)(dataBuf + i + 7);
            i += 8;
        }
        *(u32*)(dataBuf + 0x3FF8) = checksum;
        *(u32*)(dataBuf + 0x3FFC) = ~checksum;
    }

    *(u16*)((s32)wp + 0x08) = 0xFFFF;
    *(u16*)((s32)wp + 0x0A) = 0xFFFF;
    *(u16*)((s32)wp + 0x0C) = 0xFFFF;
    *(u16*)((s32)wp + 0x0E) = 0xFFFF;
    *(u32*)((s32)wp + 0x10) = 0;
    *(u32*)((s32)wp + 0x14) = 0;
    *(u32*)((s32)wp + 0xE0) = 0;
    *(u32*)((s32)wp + 0x9C) = 0xFFFFFFFD;
    *(u16*)wp = 0;
}

u8 cardInit(void) {
    typedef struct CalendarTime {
        s32 sec, min, hour, mday, mon, year, wday, yday, msec, usec;
    } CalendarTime;
    extern void* __memAlloc(s32, u32);
    extern void* memset(void*, s32, u32);
    extern void* memcpy(void*, const void*, u32);
    extern char* strcpy(char*, const char*);
    extern s32 sprintf(char*, const char*, ...);
    extern u64 OSGetTime(void);
    extern void OSTicksToCalendarTime(u64, CalendarTime*);
    extern void CARDInit(void);
    extern void* gp;
    extern char str_Paper_Mario_802cb0f4[];
    extern char str_PCT2d_PCT2d_save_dat_802cb100[];
    extern u8 _mariost_banner_tex[];
    extern u8 _mariost_icon_tex[];
    extern u8 _mariost_icon_tlut[];
    extern char version[];
    CalendarTime calendar;
    u8* header;
    u8* save;
    u32 sum;
    s32 offset;
    s32 count;
    s32 slot;

    wp = __memAlloc(0, 0xF0);
    memset(wp, 0, 0xF0);
    *(void**)((s32)wp + 4) = __memAlloc(0, 0xA000);
    memset(*(void**)((s32)wp + 4), 0, 0xA000);
    *(void**)((s32)wp + 0xA8) = __memAlloc(0, 0x12000);
    memset(*(void**)((s32)wp + 0xA8), 0, 0x12000);
    *(void**)((s32)wp + 0xAC) = __memAlloc(0, 0x4000);
    memset(*(void**)((s32)wp + 0xAC), 0, 0x4000);
    *(void**)((s32)wp + 0xB0) = __memAlloc(0, 0x4000);
    memset(*(void**)((s32)wp + 0xB0), 0, 0x4000);

    header = *(u8**)((s32)wp + 0xA8);
    memset(header, 0, 0x1E46);
    strcpy((char*)header, str_Paper_Mario_802cb0f4);
    OSTicksToCalendarTime(OSGetTime(), &calendar);
    sprintf((char*)header + 0x20, str_PCT2d_PCT2d_save_dat_802cb100,
            calendar.mon + 1, calendar.mday);
    memcpy(header + 0x40, _mariost_banner_tex, 0x1800);
    memcpy(header + 0x1840, _mariost_icon_tex, 0x400);
    memcpy(header + 0x1C40, _mariost_icon_tlut, 0x200);
    *(u16*)(header + 0x1E40) = (*(s32*)((s32)gp + 0x1294) != 0);
    *(u16*)(header + 0x1E42) = *(u16*)((s32)gp + 0x1274);
    *(u16*)(header + 0x1E44) = *(u16*)((s32)gp + 0x11B8);
    *(u32*)(header + 0x1FF8) = 0;
    *(u32*)(header + 0x1FFC) = 0xFFFFFFFF;
    sum = 0;
    offset = 0;
    count = 0x1E4;
    do {
        u8* p = header + offset;
        offset += 0x10;
        sum += p[0] + p[1] + p[2] + p[3] + p[4] + p[5] + p[6] + p[7]
             + p[8] + p[9] + p[10] + p[11] + p[12] + p[13] + p[14] + p[15];
    } while (--count != 0);
    if (offset < 0x1E46) {
        count = 0x1E46 - offset;
        do {
            sum += header[offset++];
        } while (--count != 0);
    }
    *(u32*)(header + 0x1FF8) = sum;
    *(u32*)(header + 0x1FFC) = ~sum;

    offset = 0;
    for (slot = 0; slot < 4; slot++, offset += 0x4000) {
        if (slot == 0) {
            save = *(u8**)((s32)wp + 0xB0);
        } else {
            save = *(u8**)((s32)wp + 0xA8) + offset + 0x2000;
        }
        memset(save, 0, 0x2260);
        *(u16*)save |= 1;
        strcpy((char*)save + 0x3FF0, version);
        *(u32*)(save + 0x3FF4) = 0x2260;
        *(u32*)(save + 0x3FF8) = 0;
        *(u32*)(save + 0x3FFC) = 0xFFFFFFFF;
        sum = 0;
        for (count = 0; count < 0x2260; count++) {
            sum += save[count];
        }
        *(u32*)(save + 0x3FF8) = sum;
        *(u32*)(save + 0x3FFC) = ~sum;
    }
    *(s32*)((s32)wp + 8) = -1;
    *(s32*)((s32)wp + 0xC) = -1;
    *(s32*)((s32)wp + 0x10) = 0;
    *(s32*)((s32)wp + 0x14) = 0;
    *(s32*)((s32)wp + 0xE0) = 0;
    *(s32*)((s32)wp + 0x9C) = -3;
    *(u16*)wp = 0;
    CARDInit();
    return 0;
}

void unk_800b2c2c(void) {
    *(s32*)((s32)wp + 0xE4) = *(s32*)((s32)wp + 8);
    *(s32*)((s32)wp + 0xEC) = 10;
    *(u16*)wp |= 0x8000;
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

void* cardGetCode(void) {
    return *(void**)((s32)wp + 0x9C);
}

u8 cardMain(void) {
    extern void* wp;
    extern s32 CARDProbe(s32);
    extern s32 CARDProbeEx(s32,u32*,s32*);
    extern s32 CARDSetFastMode(s32);
    extern s32 CARDMountAsync(s32,s32,void*,void*);
    extern s32 CARDCheckAsync(s32,void*);
    extern s32 CARDFormatAsync(s32,void*);
    extern s32 CARDUnmount(s32);
    extern s32 CARDDelete(s32,char*);
    extern s32 CARDGetResultCode(s32);
    extern void write_main(void); extern void write_header_main(void); extern void read_main(void); extern void read_all_main(void); extern void create_main(void);
    extern void mountDetachCallback(void); extern void mountAttachCallback(s32,s32); extern void checkCallback(s32,s32); extern void formatCallback(s32,s32);
    extern char str_mariost_save_file_802cb0e0;
    s32 command,state,result,retry,sector; u32 memSize;
    if((*(u16*)wp&0x8000)!=0){result=CARDProbe(0);if(*(s32*)((s32)wp+0xE4)!=-(result==0))*(u16*)wp|=0x4000;*(s32*)((s32)wp+0xE4)=-(result==0);if(*(s32*)((s32)wp+0xEC)>0)(*(s32*)((s32)wp+0xEC))--;}
    command=*(s32*)((s32)wp+0xE0);
    switch(command){case 1:write_main();return 0;case 2:write_header_main();return 0;case 3:read_main();return 0;case 4:read_all_main();return 0;case 5:create_main();return 0;}
    if(command!=6&&command!=7)return 0;
    if(*(s32*)((s32)wp+0x9C)!=0)goto fail;
    if((*(u16*)wp&1)!=0){result=CARDGetResultCode(*(s32*)((s32)wp+8));if(result!=-1&&result!=*(s32*)((s32)wp+0x9C)){*(s32*)((s32)wp+0x9C)=result;*(u16*)wp&=~1;}return 0;}
    state=*(s32*)((s32)wp+0xD8);
    if(state==0){*(u16*)wp|=1;for(retry=0;retry<=1000000;retry++){result=CARDProbeEx(0,&memSize,&sector);if(result!=-1)break;}if(retry>1000000)result=-0x80;if(result==0&&sector!=0x2000)result=-2;*(s32*)((s32)wp+0x9C)=result;*(s32*)((s32)wp+8)=result==0||result==-2?0:-1;*(u16*)wp&=~1;}
    else if(state==1){CARDSetFastMode(1);*(u16*)wp|=1;CARDMountAsync(*(s32*)((s32)wp+8),*(s32*)((s32)wp+4),mountDetachCallback,mountAttachCallback);}
    else if(state==2){*(u16*)wp|=1;if(command==6)CARDFormatAsync(*(s32*)((s32)wp+8),formatCallback);else CARDCheckAsync(*(s32*)((s32)wp+8),checkCallback);}
    else if(state==3){*(u16*)wp|=1;if(command==7){result=CARDDelete(*(s32*)((s32)wp+8),&str_mariost_save_file_802cb0e0);if(result==-4)result=0;}else result=CARDUnmount(*(s32*)((s32)wp+8));*(s32*)((s32)wp+0x9C)=result;*(u16*)wp&=~1;}
    else if((command==6&&state==4)||(command==7&&state==5)){*(s32*)((s32)wp+0xE0)=0;*(s32*)((s32)wp+0xE4)=0;*(u16*)wp|=0x100;*(u16*)wp&=~2;return 0;}
    else{*(u16*)wp|=1;result=CARDUnmount(*(s32*)((s32)wp+8));*(s32*)((s32)wp+0x9C)=result;*(u16*)wp&=~1;}
    *(s32*)((s32)wp+0xD8)=state+1;return 0;
fail:*(s32*)((s32)wp+0xE0)=8;*(u16*)wp|=0x200;*(u16*)wp&=~2;return 0;
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
void cardErase(s32 fileNo) {
    extern void* memset(void* ptr, s32 value, u32 size);
    extern char* strcpy(char* dst, const char* src);
    extern void* version;

    char* buf;
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
        buf = *(char**)((s32)wp + 0xB0);
        memset(buf, 0, 0x2260);
        *(u16*)buf |= 1;
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

void write_main(void) {
    extern void* wp;
    extern void* gp;
    extern s32 CARDProbeEx(s32 chan, u32* memSize, s32* sectorSize);
    extern s32 CARDSetFastMode(s32 enable);
    extern s32 CARDMountAsync(s32 chan, s32 workArea, void* detachCb, void* attachCb);
    extern s32 CARDCheckAsync(s32 chan, void* cb);
    extern s32 CARDReadAsync(void* fileInfo, u32 addr, u32 length, u32 offset, void* cb);
    extern s32 CARDWriteAsync(void* fileInfo, u32 addr, u32 length, u32 offset, void* cb);
    extern s32 CARDGetSerialNo(s32 chan, u32* serial);
    extern s32 CARDClose(void* fileInfo);
    extern s32 CARDUnmount(s32 chan);
    extern s32 CARDGetResultCode(s32 chan);
    extern void memcard_open(void);
    extern void mountDetachCallback(void);
    extern void mountAttachCallback(s32 chan, s32 result);
    extern void checkCallback(s32 chan, s32 result);
    extern void readCallback(s32 chan, s32 result);
    extern void writeCallback(s32 chan, s32 result);
    extern void* memset(void* dst, s32 value, u32 size);
    extern void* memcpy(void* dst, const void* src, u32 size);
    extern char* strcpy(char* dst, const char* src);
    extern s32 strcmp(const char* a, const char* b);
    extern s32 sprintf(char* dst, const char* fmt, ...);
    extern s64 OSGetTime(void);
    extern void OSTicksToCalendarTime(u32 hi, u32 lo, void* calendar);
    extern u8 _mariost_icon_tex[];
    extern char version[];

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

    u8* icon;
    u8* buffer;
    u8* p;
    s32 state;
    s32 result;
    s32 retry;
    s32 sectorSize;
    u32 memSize;
    u32 sum;
    s32 i;
    s32 valid;
    s64 time;

    icon = _mariost_icon_tex;

    if (*(s32*)((s32)wp + 0x9C) != 0) {
        *(s32*)((s32)wp + 0xE0) = 8;
        *(u16*)wp |= 0x200;
        *(u16*)wp &= ~2;
        return;
    }

    if ((*(u16*)wp & 1) != 0) {
        result = CARDGetResultCode(*(s32*)((s32)wp + 8));
        if ((result != -1) && (result != *(s32*)((s32)wp + 0x9C))) {
            *(s32*)((s32)wp + 0x9C) = CARDGetResultCode(*(s32*)((s32)wp + 8));
            *(u16*)wp &= ~1;
        }
        return;
    }

    state = *(s32*)((s32)wp + 0xD8);
    switch (state) {
        case 0:
            *(u16*)wp |= 1;
            for (retry = 0; retry <= 1000000; retry++) {
                result = CARDProbeEx(0, &memSize, &sectorSize);
                if (result != -1) {
                    break;
                }
            }
            if (retry > 1000000) {
                result = -0x80;
            }
            if ((result == 0) && (sectorSize != 0x2000)) {
                result = -2;
            }
            if (result == 0) {
                *(s32*)((s32)wp + 0x9C) = 0;
                *(s32*)((s32)wp + 8) = 0;
            } else if (result == -2) {
                *(s32*)((s32)wp + 0x9C) = -2;
                *(s32*)((s32)wp + 8) = 0;
            } else {
                *(s32*)((s32)wp + 0x9C) = result;
                *(s32*)((s32)wp + 8) = -1;
            }
            *(u16*)wp &= ~1;
            break;

        case 1:
            CARDSetFastMode(1);
            *(u16*)wp |= 1;
            CARDMountAsync(*(s32*)((s32)wp + 8), *(s32*)((s32)wp + 4), mountDetachCallback, mountAttachCallback);
            break;

        case 2:
            *(u16*)wp |= 1;
            CARDCheckAsync(*(s32*)((s32)wp + 8), checkCallback);
            break;

        case 3:
            memcard_open();
            break;

        case 4:
            *(u16*)wp |= 1;
            CARDReadAsync((void*)((s32)wp + 0x1C), *(u32*)((s32)wp + 0xAC), 0x2000, 0, readCallback);
            break;

        case 5:
            buffer = *(u8**)((s32)wp + 0xAC);
            valid = 0;
            if (*(u32*)(buffer + 0x1FFC) == ~*(u32*)(buffer + 0x1FF8)) {
                sum = 0;
                for (i = 0; i < 0x1E40; i += 0x10) {
                    sum += buffer[i + 0];
                    sum += buffer[i + 1];
                    sum += buffer[i + 2];
                    sum += buffer[i + 3];
                    sum += buffer[i + 4];
                    sum += buffer[i + 5];
                    sum += buffer[i + 6];
                    sum += buffer[i + 7];
                    sum += buffer[i + 8];
                    sum += buffer[i + 9];
                    sum += buffer[i + 10];
                    sum += buffer[i + 11];
                    sum += buffer[i + 12];
                    sum += buffer[i + 13];
                    sum += buffer[i + 14];
                    sum += buffer[i + 15];
                }
                for (; i < 0x1E46; i++) {
                    sum += buffer[i];
                }
                if (*(u32*)(buffer + 0x1FF8) == sum) {
                    valid = 1;
                }
            }
            if (valid == 0) {
                buffer = *(u8**)((s32)wp + 0xA8);
                memset(buffer, 0, 0x1E46);
                strcpy((char*)buffer, (char*)(icon + 0x1E14));
                time = OSGetTime();
                OSTicksToCalendarTime((u32)(time >> 32), (u32)time, &calendar);
                sprintf((char*)buffer + 0x20, (char*)(icon + 0x1E20), calendar.mon + 1, calendar.mday);
                memcpy(buffer + 0x40, icon + 0x600, 0x1800);
                memcpy(buffer + 0x1840, icon, 0x400);
                memcpy(buffer + 0x1C40, icon + 0x400, 0x200);
                *(u16*)(buffer + 0x1E40) = ((u32)(-*(s32*)((s32)gp + 0x1294)) | (u32)*(s32*)((s32)gp + 0x1294)) >> 31;
                *(s16*)(buffer + 0x1E42) = *(s32*)((s32)gp + 0x1274);
                *(s16*)(buffer + 0x1E44) = *(s32*)((s32)gp + 0x11B8);
                *(s32*)(buffer + 0x1FF8) = 0;
                *(s32*)(buffer + 0x1FFC) = -1;
                sum = 0;
                for (i = 0; i < 0x1E40; i += 0x10) {
                    sum += buffer[i + 0];
                    sum += buffer[i + 1];
                    sum += buffer[i + 2];
                    sum += buffer[i + 3];
                    sum += buffer[i + 4];
                    sum += buffer[i + 5];
                    sum += buffer[i + 6];
                    sum += buffer[i + 7];
                    sum += buffer[i + 8];
                    sum += buffer[i + 9];
                    sum += buffer[i + 10];
                    sum += buffer[i + 11];
                    sum += buffer[i + 12];
                    sum += buffer[i + 13];
                    sum += buffer[i + 14];
                    sum += buffer[i + 15];
                }
                for (; i < 0x1E46; i++) {
                    sum += buffer[i];
                }
                *(u32*)(buffer + 0x1FF8) = sum;
                *(u32*)(buffer + 0x1FFC) = ~sum;
                *(u16*)wp |= 1;
                CARDWriteAsync((void*)((s32)wp + 0x1C), *(u32*)((s32)wp + 0xA8), 0x2000, 0, writeCallback);
            }
            break;

        case 6:
            *(u16*)wp |= 1;
            CARDReadAsync((void*)((s32)wp + 0x1C), *(u32*)((s32)wp + 0xAC), 0x4000,
                          *(s32*)((s32)wp + 0xA0) * 0x4000 + 0x2000, readCallback);
            break;

        case 7:
            buffer = *(u8**)((s32)wp + 0xAC);
            valid = 0;
            if (*(u32*)(buffer + 0x3FF4) == 0x2260) {
                if (strcmp((char*)buffer + 0x3FF0, version) == 0) {
                    if (*(u32*)(buffer + 0x3FFC) == ~*(u32*)(buffer + 0x3FF8)) {
                        sum = 0;
                        for (i = 0; i < 0x2260; i += 0x10) {
                            sum += buffer[i + 0];
                            sum += buffer[i + 1];
                            sum += buffer[i + 2];
                            sum += buffer[i + 3];
                            sum += buffer[i + 4];
                            sum += buffer[i + 5];
                            sum += buffer[i + 6];
                            sum += buffer[i + 7];
                            sum += buffer[i + 8];
                            sum += buffer[i + 9];
                            sum += buffer[i + 10];
                            sum += buffer[i + 11];
                            sum += buffer[i + 12];
                            sum += buffer[i + 13];
                            sum += buffer[i + 14];
                            sum += buffer[i + 15];
                        }
                        if (*(u32*)(buffer + 0x3FF8) == sum) {
                            valid = 1;
                        }
                    }
                }
            }
            *(s32*)((s32)wp + 0xB4) = valid;
            break;

        case 8:
            *(u16*)wp |= 1;
            CARDReadAsync((void*)((s32)wp + 0x1C), *(u32*)((s32)wp + 0xAC), 0x4000,
                          *(s32*)((s32)wp + 0xA0) * 0x4000 + 0x12000, readCallback);
            break;

        case 9:
            buffer = *(u8**)((s32)wp + 0xAC);
            valid = 0;
            if (*(u32*)(buffer + 0x3FF4) == 0x2260) {
                if (strcmp((char*)buffer + 0x3FF0, version) == 0) {
                    if (*(u32*)(buffer + 0x3FFC) == ~*(u32*)(buffer + 0x3FF8)) {
                        sum = 0;
                        for (i = 0; i < 0x2260; i += 0x10) {
                            sum += buffer[i + 0];
                            sum += buffer[i + 1];
                            sum += buffer[i + 2];
                            sum += buffer[i + 3];
                            sum += buffer[i + 4];
                            sum += buffer[i + 5];
                            sum += buffer[i + 6];
                            sum += buffer[i + 7];
                            sum += buffer[i + 8];
                            sum += buffer[i + 9];
                            sum += buffer[i + 10];
                            sum += buffer[i + 11];
                            sum += buffer[i + 12];
                            sum += buffer[i + 13];
                            sum += buffer[i + 14];
                            sum += buffer[i + 15];
                        }
                        if (*(u32*)(buffer + 0x3FF8) == sum) {
                            valid = 1;
                        }
                    }
                }
            }
            *(s32*)((s32)wp + 0xB8) = valid;
            break;

        case 10:
            if ((*(s32*)((s32)wp + 0xB4) == 0) || (*(s32*)((s32)wp + 0xB8) != 0)) {
                *(s32*)((s32)wp + 0xBC) = 0;
                *(s32*)((s32)wp + 0xC0) = 1;
            } else {
                *(s32*)((s32)wp + 0xBC) = 1;
                *(s32*)((s32)wp + 0xC0) = 0;
            }
            *(u16*)wp |= 1;
            if ((*(u16*)wp & 0x800) != 0) {
                CARDWriteAsync((void*)((s32)wp + 0x1C), *(u32*)((s32)wp + 0xB0), 0x4000,
                               *(s32*)((s32)wp + 0xA0) * 0x4000 + *(s32*)((s32)wp + 0xBC) * 0x10000 + 0x2000,
                               writeCallback);
            } else {
                CARDWriteAsync((void*)((s32)wp + 0x1C),
                               *(u32*)((s32)wp + 0xA8) + *(s32*)((s32)wp + 0xA0) * 0x4000 + 0x2000,
                               0x4000,
                               *(s32*)((s32)wp + 0xA0) * 0x4000 + *(s32*)((s32)wp + 0xBC) * 0x10000 + 0x2000,
                               writeCallback);
            }
            break;

        case 11:
            *(u16*)wp |= 1;
            if ((*(u16*)wp & 0x800) != 0) {
                CARDWriteAsync((void*)((s32)wp + 0x1C), *(u32*)((s32)wp + 0xB0), 0x4000,
                               *(s32*)((s32)wp + 0xA0) * 0x4000 + *(s32*)((s32)wp + 0xC0) * 0x10000 + 0x2000,
                               writeCallback);
            } else {
                CARDWriteAsync((void*)((s32)wp + 0x1C),
                               *(u32*)((s32)wp + 0xA8) + *(s32*)((s32)wp + 0xA0) * 0x4000 + 0x2000,
                               0x4000,
                               *(s32*)((s32)wp + 0xA0) * 0x4000 + *(s32*)((s32)wp + 0xC0) * 0x10000 + 0x2000,
                               writeCallback);
            }
            break;

        case 12:
            *(u16*)wp |= 1;
            for (retry = 0; retry <= 1000000; retry++) {
                result = CARDGetSerialNo(*(s32*)((s32)wp + 8), (u32*)((s32)wp + 0x10));
                if (result != -1) {
                    break;
                }
            }
            if (retry > 1000000) {
                result = -0x80;
            }
            *(s32*)((s32)wp + 0x9C) = result;
            *(u16*)wp &= ~1;
            break;

        case 13:
            *(u16*)wp |= 1;
            for (retry = 0; retry <= 1000000; retry++) {
                result = CARDClose((void*)((s32)wp + 0x1C));
                if (result != -1) {
                    break;
                }
            }
            if (retry > 1000000) {
                result = -0x80;
            }
            *(s32*)((s32)wp + 0x9C) = result;
            *(u16*)wp &= ~1;
            break;

        case 14:
            *(u16*)wp |= 1;
            for (retry = 0; retry <= 1000000; retry++) {
                result = CARDUnmount(*(s32*)((s32)wp + 8));
                if (result != -1) {
                    break;
                }
            }
            if (retry > 1000000) {
                result = -0x80;
            }
            *(s32*)((s32)wp + 0x9C) = result;
            *(u16*)wp &= ~1;
            break;

        case 15:
            *(s32*)((s32)wp + 0xE0) = 0;
            *(u16*)wp |= 0x100;
            *(u16*)wp |= 0x2000;
            *(u16*)wp &= ~2;
            if ((*(u16*)wp & 0x800) != 0) {
                *(u16*)wp &= ~0x800;
                memcpy((void*)(*(s32*)((s32)wp + 0xA8) + *(s32*)((s32)wp + 0xA0) * 0x4000 + 0x2000),
                       *(void**)((s32)wp + 0xB0), 0x4000);
            }
            break;
    }

    *(s32*)((s32)wp + 0xD8) = state + 1;
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


u8 write_header_main(void) {
    extern s32 CARDProbeEx(s32 chan, u32* memSize, s32* sectorSize);
    extern s32 CARDSetFastMode(s32 enable);
    extern s32 CARDMountAsync(s32 chan, s32 workArea, void* detachCb, void* attachCb);
    extern s32 CARDCheckAsync(s32 chan, void* cb);
    extern s32 CARDWriteAsync(void* fileInfo, u32 addr, u32 length, u32 offset, void* cb);
    extern s32 CARDGetSerialNo(s32 chan, u32* serial);
    extern s32 CARDClose(void* fileInfo);
    extern s32 CARDUnmount(s32 chan);
    extern s32 CARDGetResultCode(s32 chan);
    extern void mountDetachCallback(void);
    extern void mountAttachCallback(s32 chan, s32 result);
    extern void checkCallback(s32 chan, s32 result);
    extern void writeCallback(s32 chan, s32 result);
    extern void memcard_open(void);

    s32 result;
    s32 retry;
    u32 memSize;
    s32 sectorSize;

    if (*(s32*)((s32)wp + 0x9C) != 0) {
        *(s32*)((s32)wp + 0xE0) = 8;
        *(u16*)wp |= 0x200;
        *(u16*)wp &= ~2;
        return 0;
    }

    if ((*(u16*)wp & 1) == 0) {
        switch (*(s32*)((s32)wp + 0xD8)) {
            case 0:
                *(u16*)wp |= 1;
                for (retry = 0; retry <= 1000000; retry++) {
                    result = CARDProbeEx(0, &memSize, &sectorSize);
                    if (result != -1) {
                        break;
                    }
                }
                if (retry > 1000000) {
                    result = -0x80;
                }
                if ((result == 0) && (sectorSize != 0x2000)) {
                    result = -2;
                }
                if (result == 0) {
                    *(s32*)((s32)wp + 0x9C) = 0;
                    *(s32*)((s32)wp + 0x08) = 0;
                } else if (result == -2) {
                    *(s32*)((s32)wp + 0x9C) = -2;
                    *(s32*)((s32)wp + 0x08) = 0;
                } else {
                    *(s32*)((s32)wp + 0x9C) = result;
                    *(s32*)((s32)wp + 0x08) = -1;
                }
                *(u16*)wp &= ~1;
                break;
            case 1:
                CARDSetFastMode(1);
                *(u16*)wp |= 1;
                CARDMountAsync(*(s32*)((s32)wp + 4), *(s32*)((s32)wp + 8), mountDetachCallback, mountAttachCallback);
                break;
            case 2:
                *(u16*)wp |= 1;
                CARDCheckAsync(*(s32*)((s32)wp + 4), checkCallback);
                break;
            case 3:
                memcard_open();
                break;
            case 4:
                *(u16*)wp |= 1;
                CARDWriteAsync((void*)((s32)wp + 0x1C), *(u32*)((s32)wp + 0xA8), 0x2000, 0, writeCallback);
                break;
            case 5:
                *(u16*)wp |= 1;
                for (retry = 0; retry <= 1000000; retry++) {
                    result = CARDGetSerialNo(*(s32*)((s32)wp + 4), (u32*)((s32)wp + 0x10));
                    if (result != -1) {
                        break;
                    }
                }
                if (retry > 1000000) {
                    result = -0x80;
                }
                *(s32*)((s32)wp + 0x9C) = result;
                *(u16*)wp &= ~1;
                break;
            case 6:
                *(u16*)wp |= 1;
                for (retry = 0; retry <= 1000000; retry++) {
                    result = CARDClose((void*)((s32)wp + 0x1C));
                    if (result != -1) {
                        break;
                    }
                }
                if (retry > 1000000) {
                    result = -0x80;
                }
                *(s32*)((s32)wp + 0x9C) = result;
                *(u16*)wp &= ~1;
                break;
            case 7:
                *(u16*)wp |= 1;
                for (retry = 0; retry <= 1000000; retry++) {
                    result = CARDUnmount(*(s32*)((s32)wp + 4));
                    if (result != -1) {
                        break;
                    }
                }
                if (retry > 1000000) {
                    result = -0x80;
                }
                *(s32*)((s32)wp + 0x9C) = result;
                *(u16*)wp &= ~1;
                break;
            case 8:
                *(s32*)((s32)wp + 0xE0) = 0;
                *(u16*)wp |= 0x100;
                *(u16*)wp |= 0x2000;
                *(u16*)wp &= ~2;
                break;
        }
        *(s32*)((s32)wp + 0xD8) += 1;
    } else {
        result = CARDGetResultCode(*(s32*)((s32)wp + 4));
        if ((result != -1) && (*(s32*)((s32)wp + 0x9C) != CARDGetResultCode(*(s32*)((s32)wp + 4)))) {
            *(s32*)((s32)wp + 0x9C) = CARDGetResultCode(*(s32*)((s32)wp + 4));
            *(u16*)wp &= ~1;
        }
    }

    return 0;
}
u8 read_main(void) {
    extern void* wp;
    extern s32 CARDProbeEx(s32, u32*, s32*);
    extern s32 CARDSetFastMode(s32);
    extern s32 CARDMountAsync(s32, s32, void*, void*);
    extern s32 CARDCheckAsync(s32, void*);
    extern s32 CARDReadAsync(void*, u32, u32, u32, void*);
    extern s32 CARDGetResultCode(s32);
    extern void mountDetachCallback(void);
    extern void mountAttachCallback(s32, s32);
    extern void checkCallback(s32, s32);
    extern void readCallback(s32, s32);
    extern void memcard_open(void);
    extern s32 strcmp(char*, char*);
    extern char version[];
    s32 state, result, retry, sector, i;
    u32 memSize, sum;
    u8* buffer;
    s32 valid;

    if (*(s32*)((s32)wp + 0x9C) != 0) {
        *(s32*)((s32)wp + 0xE0) = 8;
        *(u16*)wp |= 0x200;
        *(u16*)wp &= ~2;
        return 0;
    }
    if ((*(u16*)wp & 1) != 0) goto pending;
    state = *(s32*)((s32)wp + 0xD8);
    switch (state) {
    case 0:
        *(u16*)wp |= 1;
        for (retry = 0; retry <= 1000000; retry++) {
            result = CARDProbeEx(0, &memSize, &sector);
            if (result != -1) break;
        }
        if (retry > 1000000) result = -0x80;
        if (result == 0 && sector != 0x2000) result = -2;
        *(s32*)((s32)wp + 0x9C) = result;
        *(s32*)((s32)wp + 8) = (result == 0 || result == -2) ? 0 : -1;
        *(u16*)wp &= ~1;
        break;
    case 1:
        CARDSetFastMode(1);
        *(u16*)wp |= 1;
        CARDMountAsync(*(s32*)((s32)wp + 8), *(s32*)((s32)wp + 4),
                       mountDetachCallback, mountAttachCallback);
        break;
    case 2:
        *(u16*)wp |= 1;
        CARDCheckAsync(*(s32*)((s32)wp + 8), checkCallback);
        break;
    case 3:
        memcard_open();
        break;
    case 4:
        *(u16*)wp |= 1;
        CARDReadAsync((void*)((s32)wp + 0x1C), (u32)*(void**)((s32)wp + 0xAC),
                      0x2000, 0, readCallback);
        break;
    case 5:
        buffer = *(u8**)((s32)wp + 0xAC);
        sum = 0;
        for (i = 0; i < 0x1E46; i++) sum += buffer[i];
        if (*(u32*)(buffer + 0x1FFC) != ~*(u32*)(buffer + 0x1FF8) ||
            *(u32*)(buffer + 0x1FF8) != sum) {
            *(u16*)wp |= 0x400;
        }
        break;
    case 6:
        *(u16*)wp |= 1;
        CARDReadAsync((void*)((s32)wp + 0x1C), (u32)*(void**)((s32)wp + 0xAC), 0x4000,
                      *(s32*)((s32)wp + 0xA4) * 0x4000 + 0x2000, readCallback);
        break;
    case 7:
        buffer = *(u8**)((s32)wp + 0xAC);
        valid = 0;
        if (*(u32*)(buffer + 0x3FF4) == 0x2260 && strcmp((char*)buffer + 0x3FF0, version) == 0) {
            sum = 0;
            for (i = 0; i < 0x2260; i++) sum += buffer[i];
            if (*(u32*)(buffer + 0x3FF8) == sum && *(u32*)(buffer + 0x3FFC) == ~sum) valid = 1;
        }
        *(s32*)((s32)wp + 0xB4) = valid;
        *(u32*)((s32)wp + 0xC8) = *(u32*)(buffer + 0x23E0);
        *(u32*)((s32)wp + 0xCC) = *(u32*)(buffer + 0x23E4);
        break;
    case 8:
        *(u16*)wp |= 1;
        CARDReadAsync((void*)((s32)wp + 0x1C), (u32)*(void**)((s32)wp + 0xAC), 0x4000,
                      *(s32*)((s32)wp + 0xA4) * 0x4000 + 0x12000, readCallback);
        break;
    case 9:
        buffer = *(u8**)((s32)wp + 0xAC);
        valid = 0;
        if (*(u32*)(buffer + 0x3FF4) == 0x2260 && strcmp((char*)buffer + 0x3FF0, version) == 0) {
            sum = 0;
            for (i = 0; i < 0x2260; i++) sum += buffer[i];
            if (*(u32*)(buffer + 0x3FF8) == sum && *(u32*)(buffer + 0x3FFC) == ~sum) valid = 1;
        }
        *(s32*)((s32)wp + 0xB8) = valid;
        *(u32*)((s32)wp + 0xD0) = *(u32*)(buffer + 0x23E0);
        *(u32*)((s32)wp + 0xD4) = *(u32*)(buffer + 0x23E4);
        break;
    case 10:
        *(s32*)((s32)wp + 0xE0) = 0;
        *(u16*)wp |= 0x100;
        *(u16*)wp &= ~2;
        return 0;
    }
    *(s32*)((s32)wp + 0xD8) = state + 1;
    return 0;

pending:
    result = CARDGetResultCode(*(s32*)((s32)wp + 8));
    if (result != -1 && result != *(s32*)((s32)wp + 0x9C)) {
        *(s32*)((s32)wp + 0x9C) = result;
        *(u16*)wp &= ~1;
    }
    return 0;
}

void cardReadAll(void) {
    if ((*(u16*)wp & 2) == 0) {
        *(s32*)((s32)wp + 0xE0) = 4;
        *(s32*)((s32)wp + 0xA4) = 0;
        *(s32*)((s32)wp + 0xD8) = 0;
        *(s32*)((s32)wp + 0x9C) = 0;
        *(u16*)wp |= 2;
        *(u16*)wp &= ~0x700;
        *(u16*)wp &= ~0x2000;
    }
}
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

void read_all_main(void) {
    typedef struct CalendarTime {
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
    } CalendarTime;

    extern void* wp;
    extern void* gp;
    extern s32 CARDProbeEx(s32 chan, u32* memSize, s32* sectorSize);
    extern s32 CARDSetFastMode(s32 mode);
    extern s32 CARDMountAsync(s32 chan, s32 workArea, void* detachCb, void* attachCb);
    extern s32 CARDCheckAsync(s32 chan, void* cb);
    extern s32 CARDReadAsync(void* fileInfo, u32 addr, u32 length, u32 offset, void* cb);
    extern s32 CARDGetResultCode(s32 chan);
    extern s32 CARDGetSerialNo(s32 chan, u32* serial);
    extern s32 CARDClose(void* fileInfo);
    extern s32 CARDUnmount(s32 chan);
    extern void memcard_open(void);
    extern void mountDetachCallback(void);
    extern void mountAttachCallback(s32 chan, s32 result);
    extern void checkCallback(s32 chan, s32 result);
    extern void readCallback(s32 chan, s32 result);
    extern void* memset(void* dst, s32 value, u32 size);
    extern void* memcpy(void* dst, const void* src, u32 size);
    extern char* strcpy(char* dst, const char* src);
    extern s32 sprintf(char* dst, const char* fmt, ...);
    extern u64 OSGetTime(void);
    extern void OSTicksToCalendarTime(u64 ticks, CalendarTime* calendar);
    extern s32 strcmp(char* a, char* b);
    extern char str_Paper_Mario_802cb0f4[];
    extern char str_PCT2d_PCT2d_save_dat_802cb100[];
    extern u8 _mariost_banner_tex[];
    extern u8 _mariost_icon_tex[];
    extern u8 _mariost_icon_tlut[];
    extern char version[];

    u16* flags = (u16*)wp;
    s32 state;
    s32 result;
    s32 retry;
    s32 sector;
    s32 i;
    s32 count;
    s32 valid;
    s32 choice;
    s32 slot;
    u32 memSize;
    u32 sum;
    u32 hi0;
    u32 hi1;
    u32 lo0;
    u32 lo1;
    u8* buffer;
    u8* p;
    u64 ticks;
    CalendarTime calendar;

    if (*(s32*)((s32)wp + 0x9C) != 0) {
        *(s32*)((s32)wp + 0xE0) = 8;
        *flags |= 0x200;
        *flags &= ~2;
        return;
    }

    if ((*flags & 1) != 0) {
        result = CARDGetResultCode(*(s32*)((s32)wp + 8));
        if (result != -1 && result != *(s32*)((s32)wp + 0x9C)) {
            *(s32*)((s32)wp + 0x9C) = result;
            *flags &= ~1;
        }
        return;
    }

    state = *(s32*)((s32)wp + 0xD8);
    switch (state) {
    case 0:
        *flags |= 1;
        retry = 0;
        while (retry <= 1000000) {
            result = CARDProbeEx(0, &memSize, &sector);
            if (result != -1) {
                break;
            }
            retry++;
        }
        if (retry > 1000000) {
            result = -0x80;
        }
        if (result == 0 && sector != 0x2000) {
            result = -2;
        }
        *(s32*)((s32)wp + 0x9C) = result;
        if (result == 0 || result == -2) {
            *(s32*)((s32)wp + 8) = 0;
        } else {
            *(s32*)((s32)wp + 8) = -1;
        }
        *flags &= ~1;
        break;

    case 1:
        CARDSetFastMode(1);
        *flags |= 1;
        CARDMountAsync(*(s32*)((s32)wp + 8), *(s32*)((s32)wp + 4), mountDetachCallback, mountAttachCallback);
        break;

    case 2:
        *flags |= 1;
        CARDCheckAsync(*(s32*)((s32)wp + 8), checkCallback);
        break;

    case 3:
        memcard_open();
        break;

    case 4:
        *flags |= 1;
        CARDReadAsync((void*)((s32)wp + 0x1C), *(u32*)((s32)wp + 0xA8), 0x2000, 0, readCallback);
        break;

    case 5:
        buffer = *(u8**)((s32)wp + 0xA8);
        valid = 0;
        if (*(u32*)(buffer + 0x1FFC) == ~*(u32*)(buffer + 0x1FF8)) {
            sum = 0;
            for (i = 0; i < 0x1E46; i++) {
                sum += buffer[i];
            }
            if (*(u32*)(buffer + 0x1FF8) == sum) {
                valid = 1;
            }
        }
        if (valid == 0) {
            memset(buffer, 0, 0x1E46);
            strcpy((char*)buffer, str_Paper_Mario_802cb0f4);
            ticks = OSGetTime();
            OSTicksToCalendarTime(ticks, &calendar);
            sprintf((char*)buffer + 0x20, str_PCT2d_PCT2d_save_dat_802cb100, calendar.mon + 1, calendar.mday);
            memcpy(buffer + 0x40, _mariost_banner_tex, 0x1800);
            memcpy(buffer + 0x1840, _mariost_icon_tex, 0x400);
            memcpy(buffer + 0x1C40, _mariost_icon_tlut, 0x200);
            *(u16*)(buffer + 0x1E40) = (*(s32*)((s32)gp + 0x1294) != 0);
            *(u16*)(buffer + 0x1E42) = *(u16*)((s32)gp + 0x1274);
            *(u16*)(buffer + 0x1E44) = *(u16*)((s32)gp + 0x11B8);
            *(u32*)(buffer + 0x1FF8) = 0;
            *(u32*)(buffer + 0x1FFC) = 0xFFFFFFFF;
            sum = 0;
            for (i = 0; i < 0x1E46; i++) {
                sum += buffer[i];
            }
            *(u32*)(buffer + 0x1FF8) = sum;
            *(u32*)(buffer + 0x1FFC) = ~sum;
            *flags |= 0x400;
        }
        *(s32*)((s32)gp + 0x1274) = *(u16*)(buffer + 0x1E42);
        *(s32*)((s32)gp + 0x1294) = *(u16*)(buffer + 0x1E40);
        *(s32*)((s32)gp + 0x11B8) = *(u16*)(buffer + 0x1E44);
        break;

    case 6:
        *flags |= 1;
        CARDReadAsync((void*)((s32)wp + 0x1C), *(u32*)((s32)wp + 0xAC), 0x4000,
                      *(s32*)((s32)wp + 0xA4) * 0x4000 + 0x2000, readCallback);
        break;

    case 7:
        buffer = *(u8**)((s32)wp + 0xAC);
        valid = 0;
        if (*(u32*)(buffer + 0x3FF4) == 0x2260 && strcmp((char*)buffer + 0x3FF0, version) == 0) {
            if (*(u32*)(buffer + 0x3FFC) == ~*(u32*)(buffer + 0x3FF8)) {
                sum = 0;
                for (i = 0; i < 0x2260; i++) {
                    sum += buffer[i];
                }
                if (*(u32*)(buffer + 0x3FF8) == sum) {
                    valid = 1;
                }
            }
        }
        *(s32*)((s32)wp + 0xB4) = valid;
        *(u32*)((s32)wp + 0xC8) = *(u32*)(buffer + 0x11F0);
        *(u32*)((s32)wp + 0xCC) = *(u32*)(buffer + 0x11F4);
        break;

    case 8:
        *flags |= 1;
        CARDReadAsync((void*)((s32)wp + 0x1C), *(u32*)((s32)wp + 0xAC), 0x4000,
                      *(s32*)((s32)wp + 0xA4) * 0x4000 + 0x12000, readCallback);
        break;

    case 9:
        buffer = *(u8**)((s32)wp + 0xAC);
        valid = 0;
        if (*(u32*)(buffer + 0x3FF4) == 0x2260 && strcmp((char*)buffer + 0x3FF0, version) == 0) {
            if (*(u32*)(buffer + 0x3FFC) == ~*(u32*)(buffer + 0x3FF8)) {
                sum = 0;
                for (i = 0; i < 0x2260; i++) {
                    sum += buffer[i];
                }
                if (*(u32*)(buffer + 0x3FF8) == sum) {
                    valid = 1;
                }
            }
        }
        *(s32*)((s32)wp + 0xB8) = valid;
        *(u32*)((s32)wp + 0xD0) = *(u32*)(buffer + 0x11F0);
        *(u32*)((s32)wp + 0xD4) = *(u32*)(buffer + 0x11F4);
        break;

    case 10:
        choice = *(s32*)((s32)wp + 0xB4);
        if (choice != 0 && *(s32*)((s32)wp + 0xB8) != 0) {
            hi0 = *(u32*)((s32)wp + 0xC8);
            lo0 = *(u32*)((s32)wp + 0xCC);
            hi1 = *(u32*)((s32)wp + 0xD0);
            lo1 = *(u32*)((s32)wp + 0xD4);
            if ((hi1 ^ 0x80000000U) < (hi0 ^ 0x80000000U) ||
                ((hi1 ^ 0x80000000U) == (hi0 ^ 0x80000000U) && lo1 < lo0)) {
                choice = 0;
            } else {
                choice = 1;
            }
        } else if (choice == 0 && *(s32*)((s32)wp + 0xB8) != 0) {
            choice = 1;
        } else if (choice != 0 && *(s32*)((s32)wp + 0xB8) == 0) {
            choice = 0;
        } else {
            choice = -1;
        }

        slot = *(s32*)((s32)wp + 0xA4);
        if (choice == -1) {
            p = *(u8**)((s32)wp + 0xA8) + slot * 0x4000;
            *(u16*)(p + 0x2000) |= 2;
        } else {
            *flags |= 1;
            CARDReadAsync((void*)((s32)wp + 0x1C),
                          (u32)(*(u8**)((s32)wp + 0xA8) + slot * 0x4000 + 0x2000),
                          0x4000,
                          slot * 0x4000 + choice * 0x10000 + 0x2000,
                          readCallback);
        }
        *(s32*)((s32)wp + 0xA4) = slot + 1;
        if (*(s32*)((s32)wp + 0xA4) < 4) {
            *(s32*)((s32)wp + 0xD8) = 3;
        }
        break;

    case 11:
        *flags |= 1;
        retry = 0;
        while (retry <= 1000000) {
            result = CARDGetSerialNo(*(s32*)((s32)wp + 8), (u32*)((s32)wp + 0x10));
            if (result != -1) {
                break;
            }
            retry++;
        }
        if (retry > 1000000) {
            result = -0x80;
        }
        *(s32*)((s32)wp + 0x9C) = result;
        *flags &= ~1;
        break;

    case 12:
        *flags |= 1;
        retry = 0;
        while (retry <= 1000000) {
            result = CARDClose((void*)((s32)wp + 0x1C));
            if (result != -1) {
                break;
            }
            retry++;
        }
        if (retry > 1000000) {
            result = -0x80;
        }
        *(s32*)((s32)wp + 0x9C) = result;
        *flags &= ~1;
        break;

    case 13:
        *flags |= 1;
        retry = 0;
        while (retry <= 1000000) {
            result = CARDUnmount(*(s32*)((s32)wp + 8));
            if (result != -1) {
                break;
            }
            retry++;
        }
        if (retry > 1000000) {
            result = -0x80;
        }
        *(s32*)((s32)wp + 0x9C) = result;
        *flags &= ~1;
        break;

    case 14:
        *(s32*)((s32)wp + 0xE0) = 0;
        *flags |= 0x100;
        *flags |= 0x2000;
        *flags &= ~2;
        break;
    }

    *(s32*)((s32)wp + 0xD8) = *(s32*)((s32)wp + 0xD8) + 1;
    return;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


void cardCreate(void) {
    if ((*(u16*)wp & 2) == 0) {
        *(s32*)((s32)wp + 0xE0) = 5;
        *(s32*)((s32)wp + 0xD8) = 0;
        *(s32*)((s32)wp + 0x9C) = 0;
        *(u16*)wp |= 2;
        *(u16*)wp &= ~0x700;
        *(u16*)wp &= ~0x2000;
        *(u16*)wp &= ~0x800;
    }
}

u8 create_main(void) {
    extern void* wp;
    extern s32 CARDProbeEx(s32, u32*, s32*);
    extern s32 CARDSetFastMode(s32);
    extern s32 CARDMountAsync(s32, s32, void*, void*);
    extern s32 CARDCheckAsync(s32, void*);
    extern s32 CARDDelete(s32, char*);
    extern s32 CARDCreateAsync(s32, char*, u32, void*, void*);
    extern s32 CARDWriteAsync(void*, u32, u32, u32, void*);
    extern s32 CARDGetStatus(s32, s32, void*);
    extern s32 CARDSetStatusAsync(s32, s32, void*, void*);
    extern s32 CARDGetSerialNo(s32, u32*);
    extern s32 CARDUnmount(s32);
    extern s32 CARDGetResultCode(s32);
    extern void mountDetachCallback(void);
    extern void mountAttachCallback(s32, s32);
    extern void checkCallback(s32, s32);
    extern void createCallback(s32, s32);
    extern void writeCallback(s32, s32);
    extern void statusCallback(s32, s32);
    extern void* memset(void*, s32, u32);
    extern char* strcpy(char*, char*);
    extern char str_mariost_save_file_802cb0e0;
    extern char str_Paper_Mario_802cb0f4;
    extern char version[];
    s32 state, result, retry, i, j;
    u32 memSize, sum;
    s32 sectorSize;
    u8* buffer;

    if (*(s32*)((s32)wp + 0x9C) != 0) goto fail;
    if ((*(u16*)wp & 1) != 0) {
        result = CARDGetResultCode(*(s32*)((s32)wp + 8));
        if (result != -1 && result != *(s32*)((s32)wp + 0x9C)) {
            *(s32*)((s32)wp + 0x9C) = result;
            *(u16*)wp &= ~1;
        }
        return 0;
    }
    state = *(s32*)((s32)wp + 0xD8);
    switch (state) {
        case 0:
            *(u16*)wp |= 1;
            for (retry = 0; retry <= 1000000; retry++) { result = CARDProbeEx(0, &memSize, &sectorSize); if (result != -1) break; }
            if (retry > 1000000) result = -0x80;
            if (result == 0 && sectorSize != 0x2000) result = -2;
            *(s32*)((s32)wp + 0x9C) = result;
            *(s32*)((s32)wp + 8) = result == 0 || result == -2 ? 0 : -1;
            *(u16*)wp &= ~1;
            break;
        case 1: CARDSetFastMode(1); *(u16*)wp |= 1; CARDMountAsync(*(s32*)((s32)wp + 8), *(s32*)((s32)wp + 4), mountDetachCallback, mountAttachCallback); break;
        case 2: *(u16*)wp |= 1; CARDCheckAsync(*(s32*)((s32)wp + 8), checkCallback); break;
        case 3:
            *(u16*)wp |= 1; result = CARDDelete(*(s32*)((s32)wp + 8), &str_mariost_save_file_802cb0e0); if (result == -4) result = 0; *(s32*)((s32)wp + 0x9C) = result; *(u16*)wp &= ~1; break;
        case 4: *(u16*)wp |= 1; CARDCreateAsync(*(s32*)((s32)wp + 8), &str_mariost_save_file_802cb0e0, 0x22000, (void*)((s32)wp + 0x1C), createCallback); break;
        case 5:
            buffer = *(u8**)((s32)wp + 0xA8);
            memset(buffer, 0, 0x1E46); strcpy((char*)buffer, &str_Paper_Mario_802cb0f4);
            buffer[0x1FFC] = 0xFF; buffer[0x1FFD] = 0xFF; buffer[0x1FFE] = 0xFF; buffer[0x1FFF] = 0xFF;
            sum = 0; for (i = 0; i < 0x1E46; i++) sum += buffer[i];
            *(u32*)(buffer + 0x1FF8) = sum; *(u32*)(buffer + 0x1FFC) = ~sum;
            for (j = 0; j < 4; j++) {
                u8* save = j == 0 ? *(u8**)((s32)wp + 0xB0) : buffer + 0x2000 + (j - 1) * 0x4000;
                memset(save, 0, 0x2260); *(u16*)save |= 1; strcpy((char*)(save + 0x3FF0), version);
                *(u32*)(save + 0x3FF4) = 0x2260; sum = 0; for (i = 0; i < 0x2260; i++) sum += save[i];
                *(u32*)(save + 0x3FF8) = sum; *(u32*)(save + 0x3FFC) = ~sum;
            }
            *(u16*)wp |= 1; CARDWriteAsync((void*)((s32)wp + 0x1C), (u32)buffer, 0x12000, 0, writeCallback); break;
        case 6: *(u16*)wp |= 1; CARDWriteAsync((void*)((s32)wp + 0x1C), (u32)(*(u8**)((s32)wp + 0xA8) + 0x2000), 0x10000, 0x12000, writeCallback); break;
        case 7: *(u16*)wp |= 1; result = CARDGetStatus(*(s32*)((s32)wp + 8), *(s32*)((s32)wp + 0x20), (void*)((s32)wp + 0x30)); *(s32*)((s32)wp + 0x9C) = result; *(u16*)wp &= ~1; break;
        case 8: *(u32*)((s32)wp + 0x68) = 0; *(u32*)((s32)wp + 0x60) = 0x40; break;
        case 9: *(u16*)wp |= 1; CARDSetStatusAsync(*(s32*)((s32)wp + 8), *(s32*)((s32)wp + 0x20), (void*)((s32)wp + 0x30), statusCallback); break;
        case 10: *(u16*)wp |= 1; result = CARDGetSerialNo(*(s32*)((s32)wp + 8), (u32*)((s32)wp + 0x10)); *(s32*)((s32)wp + 0x9C) = result; *(u16*)wp &= ~1; break;
        case 11: *(u16*)wp |= 1; result = CARDUnmount(*(s32*)((s32)wp + 8)); *(s32*)((s32)wp + 0x9C) = result; *(u16*)wp &= ~1; break;
        case 12: *(s32*)((s32)wp + 0xE0) = 0; *(u16*)wp |= 0x100; *(u16*)wp &= ~2; return 0;
    }
    *(s32*)((s32)wp + 0xD8) = state + 1;
    return 0;
fail:
    *(s32*)((s32)wp + 0xE0) = 8; *(u16*)wp |= 0x200; *(u16*)wp &= ~2; return 0;
}

void cardFormat(void) {
    if ((*(u16*)wp & 2) == 0) {
        *(s32*)((s32)wp + 0xE0) = 6;
        *(s32*)((s32)wp + 0xD8) = 0;
        *(s32*)((s32)wp + 0x9C) = 0;
        *(u16*)wp |= 2;
        *(u16*)wp &= ~0x700;
        *(u16*)wp &= ~0x2000;
    }
}

void mountDetachCallback(void) {
    ;
}

void mountAttachCallback(s32 chan, s32 result) {
    *(s32*)((s32)wp + 0x9C) = result;
    if (result == -6 || result == -13) {
        *(s32*)((s32)wp + 0x9C) = 0;
    }
    *(u16*)wp &= ~1;
}

void checkCallback(s32 chan, s32 result) {
    *(s32*)((s32)wp + 0x9C) = result;
    *(u16*)wp &= ~1;
}

void formatCallback(s32 chan, s32 result) {
    *(s32*)((s32)wp + 0x9C) = result;
    *(u16*)wp &= ~1;
}

void createCallback(s32 chan, s32 result) {
    *(s32*)((s32)wp + 0x9C) = result;
    *(u16*)wp &= ~1;
}

void writeCallback(s32 chan, s32 result) {
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

u8 memcard_open(void) {
    extern s32 CARDGetSerialNo(s32 chan, u32* serial);
    extern s32 CARDGetStatus(s32 chan, s32 fileNo, void* stat);
    extern s32 CARDOpen(s32 chan, const char* fileName, void* fileInfo);
    extern s32 CARDFreeBlocks(s32 chan, s32* bytesNotUsed, s32* filesNotUsed);
    extern s32 strcmp(const char* a, const char* b);
    extern char str_mariost_save_file_802cb0e0[];

    s32 result;
    s32 retry;
    s32 fileNo;
    u32 serialHi = 0;
    u32 serialLo = 0;
    s32 bytesNotUsed;
    s32 filesNotUsed;
    char stat[0x80];

    *(u16*)wp |= 1;

    if ((*(u16*)wp & 0x2000) != 0) {
        for (retry = 0; retry <= 1000000; retry++) {
            result = CARDGetSerialNo(*(s32*)((s32)wp + 8), &serialLo);
            if (result != -1) {
                break;
            }
        }
        if (retry > 1000000) {
            result = -0x80;
        }
        if (result != 0) {
            *(s32*)((s32)wp + 0x9C) = result;
            *(u16*)wp &= ~1;
            return 0;
        }
        if ((serialHi != *(u32*)((s32)wp + 0x10)) || (serialLo != *(u32*)((s32)wp + 0x14))) {
            *(s32*)((s32)wp + 0x9C) = -10;
            *(u16*)wp &= ~1;
            return 0;
        }
    }

    for (fileNo = 0; fileNo < 0x7F; fileNo++) {
        for (retry = 0; retry <= 1000000; retry++) {
            result = CARDGetStatus(*(s32*)((s32)wp + 8), fileNo, stat);
            if (result != -1) {
                break;
            }
        }
        if (retry > 1000000) {
            result = -0x80;
        }
        if ((result == -4) || (result == -10)) {
            continue;
        }
        if (result == 0) {
            if (strcmp(stat, str_mariost_save_file_802cb0e0) == 0) {
                if ((*(s32*)(stat + 0x38) != 0) ||
                    (*(s32*)(stat + 0x30) != 0x40) ||
                    (*(s32*)(stat + 0x20) != 0x22000)) {
                    *(s32*)((s32)wp + 0x18) = fileNo;
                    *(s32*)((s32)wp + 0x9C) = -4;
                    *(u16*)wp &= ~1;
                    return 0;
                }
                *(s32*)((s32)wp + 0x18) = fileNo;
                break;
            }
        } else if ((result == -0x80) || (result == -3)) {
            *(s32*)((s32)wp + 0x9C) = result;
            *(u16*)wp &= ~1;
            return 0;
        }
    }

    if (fileNo < 0x7F) {
        for (retry = 0; retry <= 1000000; retry++) {
            result = CARDOpen(*(s32*)((s32)wp + 8), str_mariost_save_file_802cb0e0, (void*)((s32)wp + 0x1C));
            if (result != -1) {
                break;
            }
        }
        if (retry > 1000000) {
            result = -0x80;
        }
        *(s32*)((s32)wp + 0x9C) = result;
        *(u16*)wp &= ~1;
    } else {
        for (retry = 0; retry <= 1000000; retry++) {
            result = CARDFreeBlocks(*(s32*)((s32)wp + 8), &bytesNotUsed, &filesNotUsed);
            if (result != -1) {
                break;
            }
        }
        if (retry > 1000000) {
            result = -0x80;
        }
        if (result == 0) {
            if (bytesNotUsed < 0x22000) {
                *(s32*)((s32)wp + 0x9C) = -9;
            } else if (filesNotUsed < 1) {
                *(s32*)((s32)wp + 0x9C) = -8;
            } else {
                *(s32*)((s32)wp + 0x9C) = -4;
            }
        } else {
            *(s32*)((s32)wp + 0x9C) = result;
        }
        *(u16*)wp &= ~1;
    }
    return 0;
}

