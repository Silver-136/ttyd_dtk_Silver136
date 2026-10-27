#pragma defer_codegen off
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


TimingAEntry _ac_timing_a_timing_lv_0[] = {
    {4, 10}, {5, 9}, {6, 8}, {7, 7}, {8, 6}, {9, 5}, {99999, 1},
};
TimingAEntry _ac_timing_a_timing_lv_1[] = {
    {0, 10}, {1, 9}, {2, 8}, {3, 7}, {4, 6}, {5, 5}, {6, 4}, {7, 4}, {8, 3}, {9, 2}, {99999, 1},
};
TimingAEntry _ac_timing_a_timing_lv_2[] = {
    {0, 9}, {1, 8}, {2, 7}, {3, 6}, {4, 5}, {5, 4}, {6, 3}, {7, 2}, {8, 1}, {9, 1}, {99999, 1},
};
TimingAEntry _ac_timing_a_timing_lv_3[] = {
    {0, 8}, {1, 7}, {2, 6}, {3, 5}, {4, 4}, {5, 3}, {6, 2}, {8, 1}, {9, 1}, {99999, 1},
};
TimingAEntry _ac_timing_a_timing_lv_4[] = {
    {0, 7}, {1, 6}, {2, 5}, {3, 4}, {4, 3}, {5, 2}, {8, 1}, {9, 1}, {99999, 1},
};
TimingAEntry _ac_timing_a_timing_lv_5[] = {
    {0, 6}, {1, 5}, {2, 4}, {3, 3}, {4, 2}, {8, 1}, {9, 1}, {99999, 1},
};
TimingAEntry _ac_timing_a_timing_lv_6[] = {
    {0, 5}, {1, 4}, {2, 3}, {3, 2}, {8, 1}, {9, 1}, {99999, 1},
};

TimingAEntry* _ac_timing_a_timing_tbl[] = {
    _ac_timing_a_timing_lv_0, _ac_timing_a_timing_lv_1, _ac_timing_a_timing_lv_2,
    _ac_timing_a_timing_lv_3, _ac_timing_a_timing_lv_4, _ac_timing_a_timing_lv_5,
    _ac_timing_a_timing_lv_6,
};

static void actionCommandDisp(f32 x, f32 y) {
    BattleWork* work = _battleWorkPointer;
    BattleActions* ac = &work->actionCommands;
    s32* blinkTimer = &ac->disp.blinkTimer;
    camGetPtr(CAMERA_2D);
    if (((TimingAWork*)ac->work)->flags & 1) {
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

void battleAcDelete_TimingA(BattleWork* work) {
    work->actionCommands.mAcState = 1000;
}

void battleAcDisp_TimingA(CameraId cameraId, void* param) {
    BattleWork* work = (BattleWork*)param;
    BattleACDispParams* disp = &work->actionCommands.disp;
    int iconSlot;
    switch (work->actionCommands.mAcState) {
        case 99:
            disp->iconX = intplGetValue(EASING_QUADRATIC_OUT, -300.0f, 30.0f, 20 - disp->slideTimer, 20);
            // no break: case 100 sets iconX to 30.0f, so the icon does not slide in
        case 100:
            disp->iconX = 30.0f;
            actionCommandDisp(disp->iconX, disp->iconY);
            if (disp->slideTimer > 0) {
                disp->slideTimer--;
            }
            break;
        case 1000:
        case 1001:
            if (disp->slideTimer >= 40) {
                iconSlot = 4; // FAKE: register-held index; also read uninitialized on the else path below. Literal 4 / slot = 4 at decl both differ
                disp->iconX = intplGetValue(iconSlot, 30.0f, -300.0f, AC_SLIDE_TIMER_AT(disp, iconSlot) - 40, 20);
            } else {
                disp->iconX = 30.0f;
            }
            actionCommandDisp(disp->iconX, disp->iconY);
            if (AC_SLIDE_TIMER_AT(disp, iconSlot) < 60) {
                disp->slideTimer++;
            }
            break;
    }
}


s32 battleAcResult_TimingA(BattleWork* work) {
    s32 successFrame = battleAcTimingA_GetSuccessFrame(work);
    s32 count = 0;
    s32 i;
    if ((work->actionCommands.setupFlags & 1) && (work->actionCommands.mAcUnit->mTokenFlags & kConfusedAction)) {
        // irand(100) is never negative: a confused auto-play always fails
        if (irand(100) >= 0) {
            work->actionCommands.result = 0;
        } else {
            work->actionCommands.result = AC_RESULT_SUCCESS;
            work->actionCommands.mResultCount++;
        }
        return work->actionCommands.result;
    }
    if (work->actionCommands.mAcUnit->badgesEquipped.AutoCommand) {
        work->actionCommands.result = AC_RESULT_SUCCESS;
        work->actionCommands.mResultCount++;
        return work->actionCommands.result;
    }
    work->actionCommands.result = 0;
    if (work->flags & BATTLE_FLAG_LECTURE_JUMP) {
        work->actionCommands.outputParams[0] = 3;
        work->actionCommands.result = AC_RESULT_SUCCESS;
        work->actionCommands.mResultCount++;
        return work->actionCommands.result;
    }
    for (i = 0; i < 20; i++) {
        if (BattlePadCheckRecordTrigger(i, PAD_BUTTON_A)) {
            count++;
        }
    }
    if (count >= 2) {
        work->actionCommands.outputParams[0] = 1;
        return work->actionCommands.result;
    }
    for (i = 0; i < successFrame; i++) {
        if (BattlePadCheckRecordTrigger(i, PAD_BUTTON_A)) {
            break;
        }
    }
    if (i >= successFrame) {
        if (count > 0) {
            work->actionCommands.outputParams[0] = 1;
        } else {
            work->actionCommands.outputParams[0] = 2;
        }
        return work->actionCommands.result;
    }
    work->actionCommands.outputParams[0] = 3;
    work->actionCommands.result = AC_RESULT_SUCCESS;
    work->actionCommands.mResultCount++;
    return work->actionCommands.result;
}

s32 battleAcTimingA_GetSuccessFrame(BattleWork* work) {
    s32 diff = BattleActionCommandGetDifficulty(work);
    TimingAEntry* e = _ac_timing_a_timing_tbl[diff];
    s32 ret = e->value;
    s32 count = work->actionCommands.mResultCount;
    if (((TimingAWork*)work->actionCommands.work)->flags & 2) {
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

s32 battleAcMain_TimingA(BattleWork* work) {
    BattleActions* ac = &work->actionCommands;
    TimingAWork* tw = (TimingAWork*)ac->work;
    BattleACDispParams* disp = &ac->disp;

    switch (work->actionCommands.mAcState) {
        case 0:
            memset(disp, 0, sizeof(BattleACDispParams));
            disp->iconX = -300.0f;
            disp->iconY = 0.0f;
            disp->slideTimer = 20;
            work->actionCommands.mAcState = 99;
            // FAKE: flags reached through a second cast of the work buffer instead of tw; tw->flags changes the object
            ((TimingAWork*)work->actionCommands.work)->flags &= ~1;
            work->actionCommands.outputParams[0] = 0;
            break;
        case 100:
            break;
        case 1000:
            tw->endTimer = 60;
            work->actionCommands.mAcState = 1001;
        case 1001:
            tw->endTimer--;
            if (tw->endTimer <= 0) {
                work->actionCommands.mAcState = 1002;
            }
            break;
        case 1002:
            work->actionCommands.maincb = NULL;
            work->actionCommands.dispcb = NULL;
            work->actionCommands.resultcb = NULL;
            work->actionCommands.deletecb = NULL;
            return 0;
    }
    return 1;
}
