#include "event/evt_lottery.h"
#include "event/evt_cmd.h"

extern void* gp;
s32 evtGetValue(EventEntry* event, s32 value);
s32 evtSetValue(EventEntry* event, s32 target, s32 value);

void* lotteryGetPtr(void) {
    return (void*)((s32)gp + 0xA8);
}

USER_FUNC(evt_lottery_todaynum) {
    evtSetValue(event, event->args[0], *(s16*)((s32)gp + 0xB8));
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_lottery_flag) {
    evtSetValue(event, event->args[0], *(u16*)((s32)gp + 0xA8));
    return EVT_RETURN_DONE;
}



/* CHATGPT STUB FILL: main/event/evt_lottery 20260624_184929 */

/* stub-fill: evt_lottery_error_check | missing_definition | ghidra_signature */
s32 evt_lottery_error_check(int param_1) {
    extern void* gp;
    extern s64 OSGetTime(void);
    extern void OSTicksToCalendarTime(s64 ticks, void* calendar);
    extern s32 evtSetValue(void* event, s32 target, s32 value);

    void* work;
    s32* args;
    s64 now;
    s64 t1;
    s64 t2;
    s64 t3;
    s64 t4;
    u16 flags;
    s32 error;
    char calA[0x28];
    char calB[0x28];

    work = gp;
    args = *(s32**)(param_1 + 0x18);
    now = OSGetTime();
    flags = *(u16*)((s32)work + 0xA8);
    error = 0;

    if (flags & 4) {
        error = 1;
    }

    t1 = ((s64)*(s32*)((s32)work + 0xC0) << 32) | *(u32*)((s32)work + 0xC4);
    if ((flags & 2) && t1 < now) {
        error = 1;
    }

    t2 = ((s64)*(s32*)((s32)work + 0xD8) << 32) | *(u32*)((s32)work + 0xDC);
    if (t2 < now) {
        error = 1;
    }

    t3 = ((s64)*(s32*)((s32)work + 0xE0) << 32) | *(u32*)((s32)work + 0xE4);
    if (t3 < now) {
        error = 1;
    }

    t4 = ((s64)*(s32*)((s32)work + 0xB0) << 32) | *(u32*)((s32)work + 0xB4);
    if (t4 < now) {
        error = 1;
    }

    if (flags & 8) {
        if (t1 > now) {
            OSTicksToCalendarTime(t1, calA);
            OSTicksToCalendarTime(now, calB);
            if (*(s32*)(calA + 0x14) != *(s32*)(calB + 0x14) ||
                *(s32*)(calA + 0x10) != *(s32*)(calB + 0x10) ||
                *(s32*)(calA + 0xC) != *(s32*)(calB + 0xC)) {
                error = 0;
            }
        }
    }

    evtSetValue((void*)param_1, args[0], error);
    return 2;
}

/* stub-fill: evt_lottery_buy | missing_definition | ghidra_signature */
#pragma optimize_for_size off
s32 evt_lottery_buy(void* pEvt) {
    extern void* gp;
    extern s64 OSGetTime(void);
    extern int rand(void);
    extern s32 evtSetValue(void* event, s32 target, s32 value);

    s32* args;
    s64 time;
    s32 today;
    s32 number;
    s32 old;
    void* work;

    args = *(s32**)((s32)pEvt + 0x18);
    time = OSGetTime();
    work = gp;

    do {
        number = rand() % 10000;
        *(s16*)((s32)work + 0xC8) = number;
        number = *(s16*)((s32)work + 0xC8);
        old = *(s16*)((s32)work + 0xB8);
    } while (number == old || number % 1000 == old % 1000 ||
             number % 100 == old % 100 || number % 10 == old % 10);

    *(u32*)((s32)work + 0xC4) = (u32)time;
    *(u32*)((s32)work + 0xC0) = (u32)(time >> 32);

    *(s16*)((s32)work + 0xCA) = 0x18B - (rand() % 60);
    *(s16*)((s32)work + 0xCC) = 0x73 - (rand() % 30);
    *(s16*)((s32)work + 0xCE) = 0x23 - (rand() % 10);
    *(s16*)((s32)work + 0xD0) = 0xA - (rand() % 6);

    if (*(u16*)((s32)work + 0xA8) & 4) {
        *(u16*)((s32)work + 0xA8) |= 8;
        *(u32*)((s32)work + 0xB4) = (u32)time;
        *(u32*)((s32)work + 0xB0) = (u32)(time >> 32);
        *(u32*)((s32)work + 0xDC) = (u32)time;
        *(u32*)((s32)work + 0xD8) = (u32)(time >> 32);
        *(u32*)((s32)work + 0xE4) = (u32)time;
        *(u32*)((s32)work + 0xE0) = (u32)(time >> 32);
    }
    if (*(u16*)((s32)work + 0xA8) & 2) {
        *(u16*)((s32)work + 0xA8) |= 8;
        *(u32*)((s32)work + 0xB4) = (u32)time;
        *(u32*)((s32)work + 0xB0) = (u32)(time >> 32);
        *(u32*)((s32)work + 0xDC) = (u32)time;
        *(u32*)((s32)work + 0xD8) = (u32)(time >> 32);
        *(u32*)((s32)work + 0xE4) = (u32)time;
        *(u32*)((s32)work + 0xE0) = (u32)(time >> 32);
    }

    *(u16*)((s32)work + 0xA8) |= 2;
    today = *(s16*)((s32)work + 0xC8);
    evtSetValue(pEvt, args[0], today);
    return 2;
}
#pragma optimize_for_size on

/* stub-fill: evt_lottery | missing_definition | ghidra_signature */
s32 evt_lottery(void* event) {
    extern void* gp;
    extern s64 OSGetTime(void);
    extern void OSTicksToCalendarTime(s64,void*);
    extern s32 rand(void);
    extern s32 dbg_lotteryinfo;
    s32* args=*(s32**)((u8*)event+0x18);
    s32 mode=evtGetValue(event,args[0]);
    s64 now=OSGetTime();
    u8* global=(u8*)gp;
    u8* work=global+0xA8;
    u16* flags=(u16*)work;
    s64* pickTimes=(s64*)(work+0x30);
    s64* signTime=(s64*)(work+8);
    s16* number=(s16*)(work+0x10);
    s32 calOld[10],calNow[10];
    dbg_lotteryinfo=1;
    *flags&=~0x1000;
    *flags&=~0x2000;
    *flags&=~0x4000;
    *flags&=~0x8000;
    if(*flags&8) {
        if(*(s64*)(work+0x18)<now) {
            OSTicksToCalendarTime(*(s64*)(work+0x18),calOld);
            OSTicksToCalendarTime(now,calNow);
            if(calOld[0]!=calNow[0]||calOld[1]!=calNow[1]||calOld[2]!=calNow[2]) {
                *flags&=~4;
                *flags&=~8;
            }
        } else {
            *flags|=4;
            *flags&=~8;
        }
    }
    if((*flags&2)==0) {
        pickTimes[mode]=0;
    } else {
        OSTicksToCalendarTime(pickTimes[mode],calOld);
        OSTicksToCalendarTime(now,calNow);
        if(calOld[0]==calNow[0]&&calOld[1]==calNow[1]&&calOld[2]==calNow[2]) {
            *flags&=~0x10;
        } else {
            if((*flags&8)==0) *flags|=0x10;
            pickTimes[mode]=now;
        }
    }
    if((*flags&1)==0) {
        *flags|=1;
        *signTime=now;
        *number=(s16)(rand()%10000);
    } else {
        OSTicksToCalendarTime(*signTime,calOld);
        OSTicksToCalendarTime(now,calNow);
        if(calOld[0]!=calNow[0]||calOld[1]!=calNow[1]||calOld[2]!=calNow[2]) {
            if(mode==1) *signTime=now;
            if((*flags&2)==0) *number=(s16)(rand()%10000);
            else if(now<*(s64*)(work+0x18)) { *flags|=4; *number=-1; }
            else {
                s64 ticksPerDay = (s64)(*(u32*)0x800000F8 >> 2) * 60 * 60 * 24;
                s32 days = (s32)((now - *(s64*)(work + 0x18)) / ticksPerDay);
                s16* grand = (s16*)(work + 0x22);
                s16* second = (s16*)(work + 0x24);
                s16* third = (s16*)(work + 0x26);
                s16* fourth = (s16*)(work + 0x28);
                s32 value;
                value = *grand;
                while (value < days) value += 395 - rand() % 60;
                *grand = (s16)(value - days);
                value = *second;
                while (value < days) value += 115 - rand() % 30;
                *second = (s16)(value - days);
                value = *third;
                while (value < days) value += 35 - rand() % 10;
                *third = (s16)(value - days);
                value = *fourth;
                while (value < days) value += 10 - rand() % 6;
                *fourth = (s16)(value - days);
                if (days == *grand) {
                    *number = *(s16*)(work + 0x20);
                    *flags |= 0x1000;
                } else if (days == *second) {
                    s32 candidate;
                    do {
                        candidate = *(s16*)(work + 0x20) % 1000 + (rand() % 10) * 1000;
                    } while (candidate == *(s16*)(work + 0x20));
                    *number = (s16)candidate;
                    *flags |= 0x2000;
                }
            }
        }
    }
    evtSetValue(event, args[1], *flags);
    evtSetValue(event, args[2], *number);
    OSTicksToCalendarTime(now, calNow);
    return 2;
}

