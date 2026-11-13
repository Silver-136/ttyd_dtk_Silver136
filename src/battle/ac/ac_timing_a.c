// Action command: press A in time with the timing gauge (ac_timing_a); the standard attack timing prompt.

#include "version.h"
#include "battle/ac/ac_timing_a.h"
#include "battle/battle.h"
#include "battle/battle_ac.h"
#include "battle/battle_pad.h"
#include "battle/battle_unit.h"
#include "driver/camdrv.h"
#include "driver/icondrv.h"
#include "system.h"
#include <string.h>
#include "icon_ids.h"

typedef struct TimingAEntry {
    s32 frame;
    s32 value;
} TimingAEntry;

/**
 * Width of the A-press window for Mario's jumps and Goombella's Headbonk, one list per difficulty:
 * pairs of (up to this many bounces, frames). The first entry is the normal window;
 * Power Bounce and Multibonk walk the list, so it narrows as the bounces go on.
 */
static TimingAEntry _ac_timing_a_timing_lv_0[] = {
    {4, 10}, {5, 9}, {6, 8}, {7, 7}, {8, 6}, {9, 5}, {99999, 1},
};
static TimingAEntry _ac_timing_a_timing_lv_1[] = {
    {0, 10}, {1, 9}, {2, 8}, {3, 7}, {4, 6}, {5, 5}, {6, 4}, {7, 4}, {8, 3}, {9, 2}, {99999, 1},
};
static TimingAEntry _ac_timing_a_timing_lv_2[] = {
    {0, 9}, {1, 8}, {2, 7}, {3, 6}, {4, 5}, {5, 4}, {6, 3}, {7, 2}, {8, 1}, {9, 1}, {99999, 1},
};
static TimingAEntry _ac_timing_a_timing_lv_3[] = {
    {0, 8}, {1, 7}, {2, 6}, {3, 5}, {4, 4}, {5, 3}, {6, 2}, {8, 1}, {9, 1}, {99999, 1},
};
static TimingAEntry _ac_timing_a_timing_lv_4[] = {
    {0, 7}, {1, 6}, {2, 5}, {3, 4}, {4, 3}, {5, 2}, {8, 1}, {9, 1}, {99999, 1},
};
static TimingAEntry _ac_timing_a_timing_lv_5[] = {
    {0, 6}, {1, 5}, {2, 4}, {3, 3}, {4, 2}, {8, 1}, {9, 1}, {99999, 1},
};
static TimingAEntry _ac_timing_a_timing_lv_6[] = {
    {0, 5}, {1, 4}, {2, 3}, {3, 2}, {8, 1}, {9, 1}, {99999, 1},
};

/** The window lists above by difficulty, read by battleAcTimingA_GetSuccessFrame. */
static TimingAEntry* _ac_timing_a_timing_tbl[] = {
    _ac_timing_a_timing_lv_0, _ac_timing_a_timing_lv_1, _ac_timing_a_timing_lv_2,
    _ac_timing_a_timing_lv_3, _ac_timing_a_timing_lv_4, _ac_timing_a_timing_lv_5,
    _ac_timing_a_timing_lv_6,
};

/**
 * Draws the A icon, shown pressed while the jump script sets timing flag bit 0 (btlevtcmd_ac_timing_flag_onoff)
 * as the cue.
 */
static void actionCommandDisp(f32 x, f32 y) {
    BattleWork* work = _battleWorkPointer;
    BattleActions* ac = &work->actionCommands;
    s32* blinkTimer = &ac->disp.blinkTimer;
    camGetPtr(CAMERA_2D);
    if (((TimingAWork*)ac->work)->flags & AC_TIMING_A_ICON_PRESSED) {
        Vec pos = {0.0f, 0.0f, 0.0f};
        pos.x = -200.0f + x;
        pos.y = 50.0f + y;
        iconDispGx(&pos, ICON_FLAG_2D, ICON_BUTTON_A_PRESSED, 1.0f);
    } else {
        Vec pos = {0.0f, 0.0f, 0.0f};
        pos.x = -200.0f + x;
        pos.y = 50.0f + y;
        iconDispGx(&pos, ICON_FLAG_2D, ICON_BUTTON_A, 1.0f);
    }
    (*blinkTimer)++;
}

/** Called by btlevtcmd_StopAC: starts the 60-frame end wait. */
void battleAcDelete_TimingA(BattleWork* work) {
    work->actionCommands.acState = AC_STATE_RUN;
}

/**
 * Display callback: slides the A icon in from the left over 20 frames (JP and later snap it in place),
 * and once the command is stopped keeps it up for 40 frames before sliding it back out.
 */
void battleAcDisp_TimingA(CameraId cameraId, void* param) {
    BattleWork* work = (BattleWork*)param;
    BattleActions* ac = &work->actionCommands;
    BattleACDispParams* disp = &ac->disp;
    switch (ac->acState) {
        case AC_STATE_WAIT_START:
            disp->iconX = intplGetValue(EASING_QUADRATIC_OUT, -300.0f, 30.0f, 20 - disp->slideTimer, 20);

            // no break: in JP and later case 100 sets iconX to 30.0f, so the icon does not slide in
        case AC_STATE_START:
#if VERSION >= VERSION_JP
            disp->iconX = 30.0f;
#endif
            actionCommandDisp(disp->iconX, disp->iconY);
            if (disp->slideTimer > 0) {
                disp->slideTimer--;
            }
            break;
        case AC_STATE_RUN:
        case AC_STATE_RUN + 1:
            if (disp->slideTimer >= 40) {
                disp->iconX = intplGetValue(EASING_QUADRATIC_OUT, 30.0f, -300.0f, disp->slideTimer - 40, 20);
            } else {
                disp->iconX = 30.0f;
            }
            actionCommandDisp(disp->iconX, disp->iconY);
            if (disp->slideTimer < 60) {
                disp->slideTimer++;
            }
            break;
    }
}

/**
 * Judges the press when the script asks: succeeds
 * if A was pressed in the last battleAcTimingA_GetSuccessFrame frames of the pad record
 * and only once in the last 20. outputParams[0]: 1 pressed too often or early, 2 not pressed, 3 success.
 * Auto Command and Frankly's jump lesson always succeed; confused units fail.
 */
s32 battleAcResult_TimingA(BattleWork* work) {
    s32 successFrame = battleAcTimingA_GetSuccessFrame(work);
    s32 count = 0;
    s32 i;
    if ((work->actionCommands.setupFlags & AC_SETUP_FLAG_CONFUSED_AUTO) && (work->actionCommands.acUnit->tokenFlags & kConfusedAction)) {
        // irand(100) is never negative: a confused auto-play always fails
        if (irand(100) >= 0) {
            work->actionCommands.result = 0;
        } else {
            work->actionCommands.result = AC_RESULT_SUCCESS;
            work->actionCommands.resultCount++;
        }
        return work->actionCommands.result;
    }
    if (work->actionCommands.acUnit->badgesEquipped.AutoCommand) {
        work->actionCommands.result = AC_RESULT_SUCCESS;
        work->actionCommands.resultCount++;
        return work->actionCommands.result;
    }
    work->actionCommands.result = 0;
    if (work->flags & BATTLE_FLAG_LECTURE_JUMP) {
        work->actionCommands.outputParams[0] = AC_TIMING_A_RESULT_SUCCESS;
        work->actionCommands.result = AC_RESULT_SUCCESS;
        work->actionCommands.resultCount++;
        return work->actionCommands.result;
    }
    for (i = 0; i < 20; i++) {
        if (BattlePadCheckRecordTrigger(i, PAD_BUTTON_A)) {
            count++;
        }
    }
    if (count >= 2) {
        work->actionCommands.outputParams[0] = AC_TIMING_A_RESULT_TOO_EARLY;
        return work->actionCommands.result;
    }
    for (i = 0; i < successFrame; i++) {
        if (BattlePadCheckRecordTrigger(i, PAD_BUTTON_A)) {
            break;
        }
    }
    if (i >= successFrame) {
        if (count > 0) {
            work->actionCommands.outputParams[0] = AC_TIMING_A_RESULT_TOO_EARLY;
        } else {
            work->actionCommands.outputParams[0] = AC_TIMING_A_RESULT_NO_PRESS;
        }
        return work->actionCommands.result;
    }
    work->actionCommands.outputParams[0] = AC_TIMING_A_RESULT_SUCCESS;
    work->actionCommands.result = AC_RESULT_SUCCESS;
    work->actionCommands.resultCount++;
    return work->actionCommands.result;
}

/**
 * Width in frames of the A window by difficulty (10 at the easiest down to 5);
 * with timing flag bit 1 it narrows as resultCount grows, as in Power Bounce and Multibonk,
 * until 100000 bounces make it 0.
 */
s32 battleAcTimingA_GetSuccessFrame(BattleWork* work) {
    s32 diff = BattleActionCommandGetDifficulty(work);
    TimingAEntry* e = _ac_timing_a_timing_tbl[diff];
    s32 ret = e->value;
    s32 count = work->actionCommands.resultCount;
    if (((TimingAWork*)work->actionCommands.work)->flags & AC_TIMING_A_WINDOW_BY_COUNT) {
        if (count >= 100000) {
            return 0;
        }
        while (TRUE) {
            if (count <= e->frame) {
                break;
            }
            e++;
        }
        ret = e->value;
    }
    return ret;
}

/**
 * Per-frame driver of action command 1, used by Mario's jumps,
 * Goombella's Headbonk and Multibonk and Frankly's lesson.
 * Does no judging itself (see battleAcResult_TimingA); it only sets up the display and runs the end wait.
 */
s32 battleAcMain_TimingA(BattleWork* work) {
    BattleActions* ac = &work->actionCommands;
    TimingAWork* tw = (TimingAWork*)ac->work;
    BattleACDispParams* disp = &ac->disp;

    switch (work->actionCommands.acState) {
        case AC_STATE_INIT:
            memset(disp, 0, sizeof(BattleACDispParams));
            disp->iconX = -300.0f;
            disp->iconY = 0.0f;
            disp->slideTimer = 20;
            work->actionCommands.acState = AC_STATE_WAIT_START;

            // FAKE: flags reached through a second cast of the work buffer instead of tw; tw->flags changes the object
            ((TimingAWork*)work->actionCommands.work)->flags &= ~AC_TIMING_A_ICON_PRESSED;
            work->actionCommands.outputParams[0] = 0;
            break;
        case AC_STATE_START:
            break;
        case AC_STATE_RUN:
            tw->endTimer = 60;
            work->actionCommands.acState = AC_STATE_RUN + 1;
        case AC_STATE_RUN + 1:
            tw->endTimer--;
            if (tw->endTimer <= 0) {
                work->actionCommands.acState = AC_STATE_RUN + 2;
            }
            break;
        case AC_STATE_RUN + 2:
            work->actionCommands.maincb = NULL;
            work->actionCommands.dispcb = NULL;
            work->actionCommands.resultcb = NULL;
            work->actionCommands.deletecb = NULL;
            return 0;
    }
    return 1;
}
