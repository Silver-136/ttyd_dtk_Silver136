#include "countdown.h"

#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/icondrv.h"
#include "evt/evt_cmd.h"
#include "manager/evtmgr_cmd.h"
#include "mario/mariost.h"
#include "system.h"
#include <dolphin/mtx.h>
#include <dolphin/os.h>
#include <string.h>
#include "pmario_sound.h"
#include "icon_ids.h"


//.sdata2
static const GXColor sWhite = {255, 255, 255, 255};
static const GXColor sShadow = {255, 255, 255, 153};

//.sbss
static CountdownWork* wp;

static void countDownDisp(CameraId cameraId, void* param);

void countDownInit(void) {
    wp = &gp->countdown;
    memset(&gp->countdown, 0, sizeof(gp->countdown));
}

void countDownSaveReStart(void) {
    CountdownWork* work = wp;
    if ((work->flags & 1) != 0) {
        u32 ticks = OS_TIMER_CLOCK / 1000;
        s64 ms = work->elapsed / ticks;
#ifdef VERSION_G8ME01
        s32 rem = work->limit - ms;
        if (rem < 0) {
            rem = 0;
        }
        if ((u32)rem <= work->restart) {
#else
        if ((u32)(work->limit - ms) <= work->restart) {
#endif
            work->elapsed = (s64)ticks * (u32)(work->limit - work->restart);
        }
        wp->prevTime = gp->renderTime;
        if (evtGetValue(NULL, GSW_STORY_PROGRESS) < 0x160) {
            wp->sfxId = psndSFXOn("SFX_STG2_BOMB_TIMER1");
        } else {
            wp->sfxId = psndSFXOn("SFX_STG7_ALART_WARNING1");
        }
#ifdef VERSION_G8ME01
        wp->flags &= ~2;
#endif
    }
}

void countDownStart(u32 limit, u32 restart) {
    wp->limit = limit;
    wp->restart = restart;
    wp->prevTime = gp->renderTime;
    wp->elapsed = 0;
    wp->state = 0;
    wp->counter = 0;
    wp->offX = 0.0f;
    wp->offY = 0.0f;
    wp->x = 180.0f;
    wp->y = -180.0f;
    wp->flags = 0;
    wp->flags |= 1;
    if (evtGetValue(NULL, GSW_STORY_PROGRESS) < 0x160) {
        wp->sfxId = psndSFXOn("SFX_STG2_BOMB_TIMER1");
    } else {
        wp->sfxId = psndSFXOn("SFX_STG7_ALART_WARNING1");
    }
}

void countDownSetRestartTime(u32 restart) {
    wp->restart = restart;
}

void countDownMain(void) {
    if ((wp->flags & 1) != 0) {
        switch (wp->state) {
        case 0:
            if (++wp->counter < 30) {
                wp->offX = intplGetValue(EASING_SINE_IN_OUT, 300.0f, 0.0f, wp->counter, 30);
            } else {
                wp->counter = 0;
                wp->state++;
            }
            break;
        case 1:
            if ((wp->flags & 2) != 0) {
                psndSFXOff(wp->sfxId);
                wp->sfxId = -1;
            }
            break;
        case 2:
            if (++wp->counter < 30) {
                wp->offX = intplGetValue(EASING_SINE_IN_OUT, 0.0f, 300.0f, wp->counter, 30);
            } else {
                wp->counter = 0;
                wp->state++;
                wp->flags &= ~1;
                psndSFXOff(wp->sfxId);
            }
            break;
        }
        if ((wp->flags & 8) == 0 && marioStGetSystemLevel() == 0) {
            wp->elapsed += OSDiffTick(gp->renderTime, wp->prevTime);
        }
        wp->prevTime = gp->renderTime;
        {
            CountdownWork* work = wp;
            if (OSTicksToMilliseconds(work->elapsed) >= work->limit) {
                work->flags |= 2;
            }
        }
        dispEntry(CAMERA_2D, 1000.0f, 0, countDownDisp, NULL);
    }
}

static void countDownDisp(CameraId cameraId, void* param) {
    GXColor white = sWhite;
    GXColor white2;
    GXColor white3;
    CountdownWork* work = wp;
    s32 remaining;
    u16 flags;
    Mtx mtx;
    Mtx scale;
    Mtx base;
    white3 = sWhite;
    white2 = sWhite;
    flags = work->flags;
    remaining = work->limit - OSTicksToMilliseconds(work->elapsed);
    if ((flags & 4) == 0) {
        s32 sec, msec;
        s32 digits[3];
        s32 i;
        if ((flags & 2) != 0) {
            remaining = 0;
        }
        sec = remaining / 1000;
        msec = remaining % 1000;
        PSMTXTrans(base, work->x + work->offX, work->y + work->offY, 0.0f);
        PSMTXScale(scale, 2.5f, 1.0f, 1.0f);
        PSMTXConcat(base, scale, mtx);
        {
            GXColor shadow = sShadow;
            iconDispGxCol(mtx, 0x10, ICON_COUNTER_BG, shadow);
        }
        digits[0] = sec / 100;
        sec %= 100;
        digits[1] = sec / 10;
        digits[2] = sec % 10;
        for (i = 0; i < 3; i++) {
            PSMTXTrans(mtx, i * 30 - 50, 8.0f, 0.0f);
            PSMTXConcat(base, mtx, mtx);
            iconDispGxCol(mtx, 0x10, (u16)(digits[i] + ICON_COUNTER_DIGIT_0), white2);
        }
        PSMTXTrans(mtx, 28.0f, 10.0f, 0.0f);
        PSMTXConcat(base, mtx, mtx);
        iconDispGxCol(mtx, 0x10, ICON_COUNTER_COLON, white3);
        digits[0] = msec / 100;
        digits[1] = (msec % 100) / 10;
        for (i = 0; i < 2; i++) {
            PSMTXTrans(mtx, i * 15 + 40, 8.0f, 0.0f);
            PSMTXScale(scale, 0.5f, 0.5f, 0.5f);
            PSMTXConcat(mtx, scale, mtx);
            PSMTXConcat(base, mtx, mtx);
            iconDispGxCol(mtx, 0x10, (u16)(digits[i] + ICON_COUNTER_DIGIT_0), white2);
        }
    }
}

s32 countDownGetStatus(void) {
    CountdownWork* work = wp;
    return !(work->flags & 1) ? -1 : (work->flags >> 1) & 1;
}

void countDownEnd(void) {
    if ((wp->flags & 1) != 0) {
        wp->state = 2;
    }
}

void countDownStop(void) {
    wp->flags |= 8;
    psndSFXOff(wp->sfxId);
}

void countDownFlagOn(u16 flag) {
    wp->flags |= flag;
}

void countDownFlagOff(u16 flag) {
    wp->flags &= ~flag;
}
