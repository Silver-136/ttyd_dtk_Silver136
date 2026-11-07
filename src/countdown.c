// Countdown timer: the on-screen time limit that timed events start, save, restart and end.

#include "version.h"
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

/** Points the countdown at gp->countdown and clears it. Called during game initialisation. */
void countDownInit(void) {
    wp = &gp->countdown;
    memset(&gp->countdown, 0, sizeof(gp->countdown));
}

/**
 * Resumes a countdown that was running when the game was saved (called by seq_load):
 * if less than the restart time remains, sets the clock back so exactly the restart time (ms) remains,
 * then restarts the ticking sound (the bomb timer before chapter 7, the alarm from chapter 7 on).
 * US and later clamp the remaining time at zero first and clear the time-up flag.
 */
void countDownSaveReStart(void) {
    CountdownWork* work = wp;
    if ((work->flags & COUNTDOWN_FLAG_RUNNING) != 0) {
        u32 ticks = OS_TIMER_CLOCK / 1000;
        s64 ms = work->elapsed / ticks;
#if VERSION >= VERSION_US
        s32 rem = work->limit - ms;
        if (rem < 0) {
            rem = 0;
        }
        if ((u32)rem <= work->restart) {
#else
        // BUG: past the limit, limit - ms is negative and the unsigned compare sees a huge value, so the timer is not
        // set back; US and later clamp the remainder at 0 first.
        if ((u32)(work->limit - ms) <= work->restart) {
#endif
            work->elapsed = (s64)ticks * (u32)(work->limit - work->restart);
        }
        wp->prevTime = gp->renderTime;
        if (evtGetValue(NULL, GSW_STORY_PROGRESS) < STG7_00_START) {
            wp->sfxId = psndSFXOn("SFX_STG2_BOMB_TIMER1");
        } else {
            wp->sfxId = psndSFXOn("SFX_STG7_ALART_WARNING1");
        }
#if VERSION >= VERSION_US
        wp->flags &= ~COUNTDOWN_FLAG_TIME_UP;
#endif
    }
}

/**
 * Starts a countdown of limit milliseconds (restart is the minimum time a resumed save gets back),
 * slides the display in from the right at (180, -180)
 * and starts the ticking sound (bomb timer before chapter 7, alarm after).
 */
void countDownStart(u32 limit, u32 restart) {
    wp->limit = limit;
    wp->restart = restart;
    wp->prevTime = gp->renderTime;
    wp->elapsed = 0;
    wp->state = COUNTDOWN_STATE_SLIDE_IN;
    wp->counter = 0;
    wp->offX = 0.0f;
    wp->offY = 0.0f;
    wp->x = 180.0f;
    wp->y = -180.0f;
    wp->flags = 0;
    wp->flags |= COUNTDOWN_FLAG_RUNNING;
    if (evtGetValue(NULL, GSW_STORY_PROGRESS) < STG7_00_START) {
        wp->sfxId = psndSFXOn("SFX_STG2_BOMB_TIMER1");
    } else {
        wp->sfxId = psndSFXOn("SFX_STG7_ALART_WARNING1");
    }
}

/** Changes the minimum time (ms) a countdown gets back when a save is loaded mid-countdown. */
void countDownSetRestartTime(u32 restart) {
    wp->restart = restart;
}

/**
 * Per frame: slides the display in or out over 30 frames,
 * adds the elapsed render time unless stopped (flag 8) or the game is paused,
 * sets the time-up flag (2) once the limit is reached and queues the draw.
 * JP and later also stop the ticking sound at time-up.
 */
void countDownMain(void) {
    if ((wp->flags & COUNTDOWN_FLAG_RUNNING) != 0) {
        switch (wp->state) {
        case COUNTDOWN_STATE_SLIDE_IN:
            if (++wp->counter < 30) {
                wp->offX = intplGetValue(EASING_SINE_IN_OUT, 300.0f, 0.0f, wp->counter, 30);
            } else {
                wp->counter = 0;
                wp->state++;
            }
            break;
        case COUNTDOWN_STATE_RUN:
#if VERSION >= VERSION_JP
            if ((wp->flags & COUNTDOWN_FLAG_TIME_UP) != 0) {
                psndSFXOff(wp->sfxId);
                wp->sfxId = -1;
            }
#endif
            break;
        case COUNTDOWN_STATE_SLIDE_OUT:
            if (++wp->counter < 30) {
                wp->offX = intplGetValue(EASING_SINE_IN_OUT, 0.0f, 300.0f, wp->counter, 30);
            } else {
                wp->counter = 0;
                wp->state++;
                wp->flags &= ~COUNTDOWN_FLAG_RUNNING;
                psndSFXOff(wp->sfxId);
            }
            break;
        }
        if ((wp->flags & COUNTDOWN_FLAG_STOPPED) == 0 && marioStGetSystemLevel() == 0) {
            wp->elapsed += OSDiffTick(gp->renderTime, wp->prevTime);
        }
        wp->prevTime = gp->renderTime;
        if (OSTicksToMilliseconds(wp->elapsed) >= wp->limit) {
            wp->flags |= COUNTDOWN_FLAG_TIME_UP;
        }
        dispEntry(CAMERA_2D, 1000.0f, DISP_MODE_OPAQUE, countDownDisp, NULL);
    }
}

/**
 * Draws the timer in the 2D camera: a shadow bar, three second digits,
 * a separator and two smaller hundredths digits, all from the icon set (US and later icon ids are 7 higher).
 * Hidden while flag 4 is set.
 */
static void countDownDisp(CameraId cameraId, void* param) {
    GXColor digitColor;
    GXColor colonColor;
    CountdownWork* work = wp;
    s32 remaining;
    u16 flags;
    Mtx mtx;
    Mtx scale;
    Mtx base;
    GXColor shadow;
    colonColor = sWhite;
    digitColor = sWhite;
    flags = work->flags;
    remaining = work->limit - OSTicksToMilliseconds(work->elapsed);
    if ((flags & COUNTDOWN_FLAG_HIDDEN) == 0) {
        s32 sec, msec;
        s32 digits[3];
        s32 i;
        if ((flags & COUNTDOWN_FLAG_TIME_UP) != 0) {
            remaining = 0;
        }
        sec = remaining / 1000;
        msec = remaining % 1000;
        PSMTXTrans(base, work->x + work->offX, work->y + work->offY, 0.0f);
        PSMTXScale(scale, 2.5f, 1.0f, 1.0f);
        PSMTXConcat(base, scale, mtx);
        shadow = sShadow;
        iconDispGxCol(mtx, ICON_FLAG_2D, ICON_COUNTER_BG, shadow);
        digits[0] = sec / 100;
        sec %= 100;
        digits[1] = sec / 10;
        digits[2] = sec % 10;
        for (i = 0; i < 3; i++) {
            PSMTXTrans(mtx, i * 30 - 50, 8.0f, 0.0f);
            PSMTXConcat(base, mtx, mtx);

            // the icon id is truncated to u16 for the s32 parameter (here and below; the clrlwi is in the retail code)
            iconDispGxCol(mtx, ICON_FLAG_2D, (u16)(digits[i] + ICON_COUNTER_DIGIT_0), digitColor);
        }
        PSMTXTrans(mtx, 28.0f, 10.0f, 0.0f);
        PSMTXConcat(base, mtx, mtx);
        iconDispGxCol(mtx, ICON_FLAG_2D, ICON_COUNTER_COLON, colonColor);
        digits[0] = msec / 100;
        digits[1] = (msec % 100) / 10;
        for (i = 0; i < 2; i++) {
            PSMTXTrans(mtx, i * 15 + 40, 8.0f, 0.0f);
            PSMTXScale(scale, 0.5f, 0.5f, 0.5f);
            PSMTXConcat(mtx, scale, mtx);
            PSMTXConcat(base, mtx, mtx);
            iconDispGxCol(mtx, ICON_FLAG_2D, (u16)(digits[i] + ICON_COUNTER_DIGIT_0), digitColor);
        }
    }
}

/** Returns -1 when no countdown is running, 1 when its time is up and 0 while it is still counting. */
s32 countDownGetStatus(void) {
    CountdownWork* work = wp;
    return !(work->flags & COUNTDOWN_FLAG_RUNNING) ? -1 : (work->flags & COUNTDOWN_FLAG_TIME_UP) != 0;
}

/** Slides a running countdown off screen and stops its sound (state 2 of countDownMain). */
void countDownEnd(void) {
    if ((wp->flags & COUNTDOWN_FLAG_RUNNING) != 0) {
        wp->state = COUNTDOWN_STATE_SLIDE_OUT;
    }
}

/** Freezes the countdown (flag 8) and stops its sound; the display stays up. */
void countDownStop(void) {
    wp->flags |= COUNTDOWN_FLAG_STOPPED;
    psndSFXOff(wp->sfxId);
}

/** Sets countdown flags (1 running, 2 time up, 4 hidden, 8 frozen). */
void countDownFlagOn(u16 flag) {
    wp->flags |= flag;
}

/** Clears countdown flags (1 running, 2 time up, 4 hidden, 8 frozen). */
void countDownFlagOff(u16 flag) {
    wp->flags &= ~flag;
}
