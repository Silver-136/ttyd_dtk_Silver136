#include "event/evt_lottery.h"
#include "event/evt_cmd.h"

extern void* gp;
extern s64 OSGetTime(void);

void* lotteryGetPtr(void) {
    return (void*)((s32)gp + 0xA8);
}
#pragma optimize_for_size on

/* stub-fill: evt_lottery | missing_definition | ghidra_signature */
s32 evt_lottery(void* event) {
    extern void* gp;
    extern s64 OSGetTime(void);
    extern void OSTicksToCalendarTime(s64,void*);
    extern s32 rand(void);
    extern s64 __div2i(s64, s64);
    extern s32 dbg_lotteryinfo;
    s32* args=*(s32**)((u8*)event+0x18);
    s32 mode=evtGetValue(event,args[0]);
    s64 now=OSGetTime();
    u8* global=(u8*)gp;
    u8* work=global+0xA8;
    u16* flags=(u16*)work;
    s32 calPickOld[10], calPickNow[10];
    s32 calModePastOld[10], calModePastNow[10];
    s32 calModeFutureOld[10], calModeFutureNow[10];
    s32 calSignOld[10], calSignNow[10];
    s32 calOutput[10];
    dbg_lotteryinfo=1;
    *flags&=~0x1000;
    *flags&=~0x2000;
    *flags&=~0x4000;
    *flags&=~0x8000;
    if(*flags&8) {
        if(*(s64*)(work+0x18)<now) {
            OSTicksToCalendarTime(*(s64*)(work+0x18),calPickOld);
            OSTicksToCalendarTime(now,calPickNow);
            if(calPickOld[0]!=calPickNow[0]||calPickOld[1]!=calPickNow[1]||calPickOld[2]!=calPickNow[2]) {
                *flags&=~4;
                *flags&=~8;
            }
        } else {
            *flags|=4;
            *flags&=~8;
        }
    }
    if((*flags&2)==0) {
        *(s64*)(work+0x30+mode*8)=0;
    } else {
        if (*(s64*)(work+0x30+mode*8) < now) {
            OSTicksToCalendarTime(*(s64*)(work+0x30+mode*8),calModePastOld);
            OSTicksToCalendarTime(now,calModePastNow);
            if(calModePastOld[0]==calModePastNow[0]&&calModePastOld[1]==calModePastNow[1]&&calModePastOld[2]==calModePastNow[2]) {
                *flags&=~0x10;
            } else {
                if((*flags&8)==0) *flags|=0x10;
                *(s64*)(work+0x30+mode*8)=now;
            }
        } else {
            OSTicksToCalendarTime(*(s64*)(work+0x30+mode*8),calModeFutureOld);
            OSTicksToCalendarTime(now,calModeFutureNow);
            if(calModeFutureOld[0]==calModeFutureNow[0]&&calModeFutureOld[1]==calModeFutureNow[1]&&calModeFutureOld[2]==calModeFutureNow[2]) {
                *flags&=~0x10;
            } else {
                *flags|=0x10;
                *(s64*)(work+0x30+mode*8)=now;
            }
            *flags|=4;
        }
    }
    if((*flags&1)==0) {
        *flags|=1;
        *(s64*)(work+8)=now;
        *(s16*)(work+0x10)=(s16)(rand()%10000);
    } else {
        OSTicksToCalendarTime(*(s64*)(work+8),calSignOld);
        OSTicksToCalendarTime(now,calSignNow);
        if(calSignOld[0]!=calSignNow[0]||calSignOld[1]!=calSignNow[1]||calSignOld[2]!=calSignNow[2]) {
            if(mode==1) *(s64*)(work+8)=now;
            if((*flags&2)==0) *(s16*)(work+0x10)=(s16)(rand()%10000);
            else if(now<*(s64*)(work+0x18)) { *flags|=4; *(s16*)(work+0x10)=-1; }
            else {
                s64 elapsed = now - *(s64*)(work + 0x18);
                s32 days;
                s32 value;
                elapsed = __div2i(elapsed, (*(u32*)0x800000F8 >> 2));
                elapsed = __div2i(elapsed, 60);
                elapsed = __div2i(elapsed, 60);
                elapsed = __div2i(elapsed, 24);
                days = (s32)elapsed;
                value = *(s16*)(work + 0x22);
                while (value < days) value += 395 - rand() % 60;
                *(s16*)(work + 0x22) = (s16)(value - days);
                value = *(s16*)(work + 0x24);
                while (value < days) value += 115 - rand() % 30;
                *(s16*)(work + 0x24) = (s16)(value - days);
                value = *(s16*)(work + 0x26);
                while (value < days) value += 35 - rand() % 10;
                *(s16*)(work + 0x26) = (s16)(value - days);
                value = *(s16*)(work + 0x28);
                while (value < days) value += 10 - rand() % 6;
                *(s16*)(work + 0x28) = (s16)(value - days);
                if (days == *(s16*)(work + 0x22)) {
                    *(s16*)(work+0x10) = *(s16*)(work + 0x20);
                    *flags |= 0x1000;
                } else if (days == *(s16*)(work + 0x24)) {
                    do {
                        *(s16*)(work+0x10) = *(s16*)(work + 0x20) % 1000 + (rand() % 10) * 1000;
                    } while (*(s16*)(work+0x10) == *(s16*)(work + 0x20));
                    *flags |= 0x2000;
                } else if (days == *(s16*)(work + 0x26)) {
                    do {
                        do {
                            *(s16*)(work+0x10) = *(s16*)(work + 0x20) % 100 + (rand() % 100) * 100;
                        } while (*(s16*)(work+0x10) == *(s16*)(work + 0x20));
                    } while (*(s16*)(work+0x10) % 1000 == *(s16*)(work + 0x20) % 1000);
                    *flags |= 0x4000;
                } else if (days == *(s16*)(work + 0x28)) {
                    do {
                        do {
                            *(s16*)(work+0x10) = *(s16*)(work + 0x20) % 10 + (rand() % 1000) * 10;
                        } while (*(s16*)(work+0x10) == *(s16*)(work + 0x20));
                    } while (*(s16*)(work+0x10) % 1000 == *(s16*)(work + 0x20) % 1000 ||
                             *(s16*)(work+0x10) % 100 == *(s16*)(work + 0x20) % 100);
                    *flags |= 0x8000;
                } else {
                    do {
                        do {
                            *(s16*)(work+0x10) = rand() % 10000;
                        } while (*(s16*)(work+0x10) == *(s16*)(work + 0x20));
                    } while (*(s16*)(work+0x10) % 1000 == *(s16*)(work + 0x20) % 1000 ||
                             *(s16*)(work+0x10) % 100 == *(s16*)(work + 0x20) % 100 ||
                             *(s16*)(work+0x10) % 10 == *(s16*)(work + 0x20) % 10);
                }
            }
        } else {
            *flags |= 4;
            *(s16*)(work+0x10) = -1;
            *(s64*)(work+8) = now;
        }
    }
    evtSetValue(event, args[1], *flags);
    evtSetValue(event, args[2], *(s16*)(work+0x10));
    OSTicksToCalendarTime(now, calOutput);
    if (*flags & 2) {
        s64 baseTime = *(s64*)(work + 0x18);
        OSTicksToCalendarTime(baseTime, calOutput);
        OSTicksToCalendarTime(baseTime + (s64)(*(u32*)0x800000F8 >> 2) * 60 * 60 * 24 * *(s16*)(work + 0x22), calOutput);
        OSTicksToCalendarTime(baseTime + (s64)(*(u32*)0x800000F8 >> 2) * 60 * 60 * 24 * *(s16*)(work + 0x24), calOutput);
        OSTicksToCalendarTime(baseTime + (s64)(*(u32*)0x800000F8 >> 2) * 60 * 60 * 24 * *(s16*)(work + 0x26), calOutput);
        OSTicksToCalendarTime(baseTime + (s64)(*(u32*)0x800000F8 >> 2) * 60 * 60 * 24 * *(s16*)(work + 0x28), calOutput);
    }
    return 2;
}

/* stub-fill: evt_lottery_buy | missing_definition | ghidra_signature */
#pragma optimize_for_size off
s32 evt_lottery_buy(void* pEvt) {
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

USER_FUNC(evt_lottery_flag) {
    evtSetValue(event, event->args[0], *(u16*)((s32)gp + 0xA8));
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_lottery_todaynum) {
    evtSetValue(event, event->args[0], *(s16*)((s32)gp + 0xB8));
    return EVT_RETURN_DONE;
}



/* CHATGPT STUB FILL: main/event/evt_lottery 20260624_184929 */

/* stub-fill: evt_lottery_error_check | missing_definition | ghidra_signature */
s32 evt_lottery_error_check(int param_1) {
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

s32 dbg_lotteryinfo;
