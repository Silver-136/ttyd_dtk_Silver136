#include "evt/evt_lottery.h"
#include "evt/evt_cmd.h"
#include "mario/mariost.h"
#include <dolphin/os.h>
#include "libc_decls.h"
#include "manager/evtmgr_cmd.h"

//.sbss
BOOL dbg_lotteryinfo;
static s32 lotteryUnused;

//local prototypes
USER_FUNC(evt_lottery);
USER_FUNC(evt_lottery_buy);
USER_FUNC(evt_lottery_flag);
USER_FUNC(evt_lottery_todaynum);
USER_FUNC(evt_lottery_error_check);

lottery_info* lotteryGetPtr(void) {
    return &gp->mLotteryInfo;
}

USER_FUNC(evt_lottery) {
    lottery_info* lot;
    s32 day;
    s32 v;
    OSCalendarTime a1, a2, b1, b2, c1, c2, d1, d2, e;
    EvtWord* args = event->args;
    s32 idx = evtGetValue(event, args[0]);

    OSTime now = OSGetTime();

    dbg_lotteryinfo = TRUE;
    lot = &gp->mLotteryInfo;

    lot->flags &= ~0x1000;
    lot->flags &= ~0x2000;
    lot->flags &= ~0x4000;
    lot->flags &= ~0x8000;

    if (lot->flags & 8) {
        if (lot->buyTime < now) {
            OSTicksToCalendarTime(lot->buyTime, &a1);
            OSTicksToCalendarTime(now, &a2);
            if (a1.year != a2.year || a1.mon != a2.mon || a1.mday != a2.mday) {
                lot->flags &= ~4;
                lot->flags &= ~8;
            }
        }
        else {
            lot->flags |= 4;
            lot->flags &= ~8;
        }
    }

    if (lot->flags & 2) {
        if (lot->times[idx] < now) {
            OSTicksToCalendarTime(lot->times[idx], &b1);
            OSTicksToCalendarTime(now, &b2);
            if (b1.year == b2.year && b1.mon == b2.mon && b1.mday == b2.mday) {
                lot->flags &= ~0x10;
            }
            else {
                if (!(lot->flags & 8)) {
                    lot->flags |= 0x10;
                }
                lot->times[idx] = now;
            }
        }
        else {
            OSTicksToCalendarTime(lot->times[idx], &c1);
            OSTicksToCalendarTime(now, &c2);
            if (c1.year == c2.year && c1.mon == c2.mon && c1.mday == c2.mday) {
                lot->flags &= ~0x10;
            }
            else {
                lot->flags |= 0x10;
                lot->times[idx] = now;
            }
            lot->flags |= 4;
        }
    }
    else {
        lot->times[idx] = 0;
    }

    if (!(lot->flags & 1)) {
        lot->flags |= 1;
        lot->lastTime = now;
        lot->todayNum = rand() % 10000;
    }
    else if (lot->lastTime < now) {
        OSTicksToCalendarTime(lot->lastTime, &d1);
        OSTicksToCalendarTime(now, &d2);
        if (d1.year != d2.year || d1.mon != d2.mon || d1.mday != d2.mday) {
            if (idx == 1) {
                lot->lastTime = now;
            }
            if (!(lot->flags & 2)) {
                lot->todayNum = rand() % 10000;
            }
            else if (!(now < lot->buyTime)) {
                day = (now - lot->buyTime) / OS_TIMER_CLOCK / 60 / 60 / 24;
                v = lot->days[0];
                if (day > v) {
                    while (day > v) {
                        v += 395 - rand() % 60;
                    }
                    lot->days[0] = v - day;
                }
                v = lot->days[1];
                if (day > v) {
                    while (day > v) {
                        v += 115 - rand() % 30;
                    }
                    lot->days[1] = v - day;
                }
                v = lot->days[2];
                if (day > v) {
                    while (day > v) {
                        v += 35 - rand() % 10;
                    }
                    lot->days[2] = v - day;
                }
                v = lot->days[3];
                if (day > v) {
                    while (day > v) {
                        v += 10 - rand() % 6;
                    }
                    lot->days[3] = v - day;
                }
                if (day == lot->days[0]) {
                    lot->todayNum = lot->number;
                    lot->flags |= 0x1000;
                }
                else if (day == lot->days[1]) {
                    do {
                        lot->todayNum = lot->number % 1000 + rand() % 10 * 1000;
                    } while (lot->todayNum == lot->number);
                    lot->flags |= 0x2000;
                }
                else if (day == lot->days[2]) {
                    do {
                        lot->todayNum = lot->number % 100 + rand() % 100 * 100;
                    } while (lot->todayNum == lot->number || lot->todayNum % 1000 == lot->number % 1000);
                    lot->flags |= 0x4000;
                }
                else if (day == lot->days[3]) {
                    do {
                        lot->todayNum = lot->number % 10 + rand() % 1000 * 10;
                    } while (lot->todayNum == lot->number || lot->todayNum % 1000 == lot->number % 1000 ||
                        lot->todayNum % 100 == lot->number % 100);
                    lot->flags |= 0x8000;
                }
                else {
                    do {
                        lot->todayNum = rand() % 10000;
                    } while (lot->todayNum == lot->number || lot->number % 1000 == lot->todayNum % 1000 ||
                        lot->number % 100 == lot->todayNum % 100 || lot->number % 10 == lot->todayNum % 10);
                }
            }
            else {
                lot->flags |= 4;
                lot->todayNum = -1;
            }
        }
    }
    else {
        lot->flags |= 4;
        lot->todayNum = -1;
        lot->lastTime = now;
    }

    evtSetValue(event, args[1], lot->flags);
    evtSetValue(event, args[2], lot->todayNum);
    OSTicksToCalendarTime(now, &e);
    if (lot->flags & 2) {
        OSTime bt;
        OSTicksToCalendarTime(lot->buyTime, &e);
        bt = lot->buyTime; OSTicksToCalendarTime(bt + (OSTime)OS_TIMER_CLOCK * 86400 * lot->days[0], &e);
        bt = lot->buyTime; OSTicksToCalendarTime(bt + (OSTime)OS_TIMER_CLOCK * 86400 * lot->days[1], &e);
        bt = lot->buyTime; bt += (OSTime)OS_TIMER_CLOCK * 86400 * lot->days[2]; OSTicksToCalendarTime(bt, &e);
        bt = lot->buyTime; bt += (OSTime)OS_TIMER_CLOCK * 86400 * lot->days[3]; OSTicksToCalendarTime(bt, &e);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_lottery_buy) {
    EvtWord* args = event->args;
    OSTime now = OSGetTime();
    lottery_info* lot = &gp->mLotteryInfo;

    do {
        lot->number = rand() % 10000;
    } while (lot->number == lot->todayNum || lot->number % 1000 == lot->todayNum % 1000 ||
        lot->number % 100 == lot->todayNum % 100 || lot->number % 10 == lot->todayNum % 10);
    lot->buyTime = now;
    lot->days[0] = 395 - rand() % 60;
    lot->days[1] = 115 - rand() % 30;
    lot->days[2] = 35 - rand() % 10;
    lot->days[3] = 10 - rand() % 6;
    if (lot->flags & 4) {
        lot->flags |= 8;
        lot->lastTime = now;
        lot->times[0] = now;
        lot->times[1] = now;
    }
    if (lot->flags & 2) {
        lot->flags |= 8;
        lot->lastTime = now;
        lot->times[0] = now;
        lot->times[1] = now;
    }
    lot->flags |= 2;
    evtSetValue(event, args[0], lot->number);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_lottery_flag) {
    evtSetValue(event, event->args[0], (&gp->mLotteryInfo)->flags);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_lottery_todaynum) {
    evtSetValue(event, event->args[0], (&gp->mLotteryInfo)->todayNum);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_lottery_error_check) {
    OSCalendarTime a, b;
    GlobalWork* g = gp;
    EvtWord* args = event->args;
    OSTime now = OSGetTime();
    lottery_info* lot = &g->mLotteryInfo;
    u16 flags = lot->flags;
    BOOL err = FALSE;

    if (flags & 4) {
        err = TRUE;
    }
    if ((flags & 2) && now < lot->buyTime) {
        err = TRUE;
    }
    if (now < lot->times[0]) {
        err = TRUE;
    }
    if (now < lot->times[1]) {
        err = TRUE;
    }
    if (now < lot->lastTime) {
        err = TRUE;
    }
    if ((flags & 8) && lot->buyTime < now) {
        OSTicksToCalendarTime(lot->buyTime, &a);
        OSTicksToCalendarTime(now, &b);
        if (a.year != b.year || a.mon != b.mon || a.mday != b.mday) {
            err = FALSE;
        }
    }
    evtSetValue(event, args[0], err);
    return EVT_RETURN_DONE;
}
