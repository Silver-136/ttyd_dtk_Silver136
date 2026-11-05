// Action command: hold the control stick to the left (ac_stick_keep_left) for the required duration.

#include "battle/battle.h"
#include "battle/battle_ac.h"
#include "battle/battle_pad.h"
#include "battle/battle_unit.h"
#include "driver/camdrv.h"
#include "driver/icondrv.h"
#include "mario/mariost.h"
#include "system.h"
#include <string.h>
#include "icon_ids.h"
#include "pmario_sound.h"

typedef struct SklWork {
    s32 timer;    // 0x0
    s32 idx;      // 0x4
    s32 endTimer; // 0x8
    u32 sfx[12];  // 0xC
} SklWork;



// BattleActions.params of this command
enum {
    STICK_KEEP_PARAM_LIGHTS = 0,    // lights to hold through
    STICK_KEEP_PARAM_INTERVAL = 1,  // frames per light
    STICK_KEEP_PARAM_PAUSE = 2,     // nonzero pauses the command
    STICK_KEEP_PARAM_PAUSED = 3,    // pause display while PARAM_PAUSE is set
    STICK_KEEP_PARAM_LAST_SIGNAL = 4, // 1 once the last signal sound played
};

// outputParams[2]
enum {
    STICK_KEEP_RESULT_EARLY = 1,   // let go before the last light, or after its release window
    STICK_KEEP_RESULT_LATE = 2,    // held too long
    STICK_KEEP_RESULT_SUCCESS = 3,
};

/**
 * Frames allowed to let go of the stick after the last light, by difficulty (80 at the easiest down to 5).
 * Used by Mario's hammer attacks and Frankly's battle lesson.
 */
s32 ac_stickkeepleft_ok_frame[7] = {80, 70, 60, 40, 20, 10, 5};

static void actionCommandDisp(f32 x, f32 y, s32 unused);

/**
 * Per-frame driver of action command 3, used by Mario's hammer attacks and Frankly's battle lesson.
 * Tilt the stick left within 120 frames and hold it while params[0] lights come on, one every params[1] frames,
 * then let go within a difficulty window (80 down to 5 frames) after the last, big one lights.
 * outputParams[2]: 1 let go early or after the window, 2 held through the last step, 3 success.
 * acFlags bit 1 ignores early releases and passes a held-too-long. A non-zero params[2] freezes.
 */
s32 battleAcMain_StickKeepLeft(BattleWork* work) {
    const char* sfx[2];
    SklWork* cw;
    BattleACDispParams* disp;
    s32 loop;
    s32 autoCommand;
    s32 ok;
    s32 i;
    s32 diff;
    s32 j;
    s32 k;

    autoCommand = 0;
    cw = (SklWork*)work->actionCommands.work;
    disp = &work->actionCommands.disp;
    if (work->actionCommands.acUnit->badgesEquipped.AutoCommand) {
        autoCommand = 1;
    }
    work->actionCommands.params[STICK_KEEP_PARAM_PAUSED] = 0;
    if (work->actionCommands.acState != AC_STATE_INIT) {
        if (work->actionCommands.params[STICK_KEEP_PARAM_PAUSE] != 0) {
            work->actionCommands.params[STICK_KEEP_PARAM_PAUSED] = 1;
        } else {
            work->actionCommands.params[STICK_KEEP_PARAM_PAUSED] = 0;
        }
    }
    sfx[0] = "SFX_AC_PI1";
    sfx[1] = "SFX_AC_PONE1";
    do {
        loop = 0;
        if (work->actionCommands.params[STICK_KEEP_PARAM_PAUSED] != 0) {
            break;
        }
        switch (work->actionCommands.acState) {
            case AC_STATE_INIT:
                work->actionCommands.result = AC_RESULT_RUNNING;
                cw->idx = 0;
                memset(disp, 0, sizeof(BattleACDispParams));
                disp->iconX = -300.0f;
                disp->iconY = 0.0f;
                disp->slideTimer = 20;
                work->actionCommands.outputParams[0] = 0;
                work->actionCommands.params[STICK_KEEP_PARAM_PAUSE] = 0;
                work->actionCommands.params[STICK_KEEP_PARAM_PAUSED] = 0;
                work->actionCommands.params[STICK_KEEP_PARAM_LAST_SIGNAL] = 0;
                work->actionCommands.outputParams[2] = 0;
                for (i = 0; i < 12; i++) {
                    cw->sfx[i] = -1;
                }
                work->actionCommands.acState = AC_STATE_WAIT_START;
                break;
            case AC_STATE_WAIT_START:
                break;
            case AC_STATE_START:
                cw->timer = 120;
                work->actionCommands.acState = AC_STATE_RUN;
                // fallthrough
            case AC_STATE_RUN:
                if ((work->actionCommands.setupFlags & AC_SETUP_FLAG_CONFUSED_AUTO) && (work->actionCommands.acUnit->tokenFlags & kConfusedAction)) {
                    if (work->actionCommands.setupFlags & AC_SETUP_FLAG_RETRIES) {
                        work->actionCommands.setupParam--;
                        if (work->actionCommands.setupParam >= 0) {
                            break;
                        }
                    }

                    // irand(100) is never negative: a confused auto-play always fails
                    if (irand(100) >= 0) {
                        work->actionCommands.result = 0;
                    } else {
                        work->actionCommands.result = AC_RESULT_SUCCESS;
                        work->actionCommands.resultCount++;
                    }
                    return 0;
                }
                if (!BattlePadCheckNow(BATTLE_PAD_STICK_LEFT) && work->actionCommands.countdown <= 0 && autoCommand == 0) {
                    if (--cw->timer <= 0) {
                        work->actionCommands.result = 0;
                        loop = 1;
                        work->actionCommands.acState = AC_STATE_RUN + 3;
                    }
                } else {
                    work->actionCommands.outputParams[0] = 1;
                    work->actionCommands.acState = AC_STATE_RUN + 1;
                    cw->timer = 1;
                    cw->idx = 1;
                    work->actionCommands.outputParams[1] = 0;
                }
                break;
            case AC_STATE_RUN + 1:
                if (work->actionCommands.countdown > 0) {
                    work->actionCommands.countdown--;
                    loop = 1;
                    work->actionCommands.result = AC_RESULT_SUCCESS;
                    work->actionCommands.acState = AC_STATE_RUN + 3;
                }
                cw->timer--;
                if (cw->timer < 0) {
                    cw->idx++;
                    work->actionCommands.outputParams[1]++;
                    if (cw->idx >= work->actionCommands.params[STICK_KEEP_PARAM_LIGHTS] + 2) {
                        work->actionCommands.result &= ~AC_RESULT_RUNNING;
                        work->actionCommands.acState = AC_STATE_RUN + 3;
                        work->actionCommands.outputParams[2] = STICK_KEEP_RESULT_LATE;
                        if (work->actionCommands.acFlags & AC_STICK_KEEP_LEFT_FLAG_NO_EARLY_RELEASE_FAIL) {
                            work->actionCommands.result |= AC_RESULT_SUCCESS;
                            work->actionCommands.result &= ~AC_RESULT_RUNNING;
                        }
                        loop = 1;
                        break;
                    }
                    j = cw->idx - 2;
                    if (j < work->actionCommands.params[STICK_KEEP_PARAM_LIGHTS] - 1) {
                        if (cw->sfx[j] == -1) {
                            cw->sfx[j] = psndSFXOn(sfx[0]);
                        }
                    } else {
                        if (cw->sfx[j] == -1) {
                            cw->sfx[j] = psndSFXOn(sfx[1]);
                            work->actionCommands.params[STICK_KEEP_PARAM_LAST_SIGNAL] = 1;
                        }
                    }
                    cw->timer = work->actionCommands.params[STICK_KEEP_PARAM_INTERVAL];
                } else {
                    if (BattlePadCheckNow(BATTLE_PAD_STICK_LEFT) && autoCommand == 0) {
                        break;
                    }
                    ok = 0;
                    if (cw->idx != work->actionCommands.params[STICK_KEEP_PARAM_LIGHTS] + 1) {
                        if (!(work->actionCommands.acFlags & AC_STICK_KEEP_LEFT_FLAG_NO_EARLY_RELEASE_FAIL)) {
                            work->actionCommands.outputParams[2] = STICK_KEEP_RESULT_EARLY;
                        }
                    } else {
                        diff = work->actionCommands.params[STICK_KEEP_PARAM_INTERVAL] - cw->timer;
                        if (diff > ac_stickkeepleft_ok_frame[BattleActionCommandGetDifficulty(work)]) {
                            if (!(work->actionCommands.acFlags & AC_STICK_KEEP_LEFT_FLAG_NO_EARLY_RELEASE_FAIL)) {
                                work->actionCommands.outputParams[2] = STICK_KEEP_RESULT_EARLY;
                            }
                        } else {
                            work->actionCommands.outputParams[2] = STICK_KEEP_RESULT_SUCCESS;
                            ok = 1;
                        }
                    }
                    if (autoCommand) {
                        if (!ok) {
                            break;
                        }
                        work->actionCommands.result |= AC_RESULT_SUCCESS;
                        work->actionCommands.result &= ~AC_RESULT_RUNNING;
                        loop = 1;
                        work->actionCommands.acState = AC_STATE_RUN + 3;
                    } else if (ok) {
                        work->actionCommands.result |= AC_RESULT_SUCCESS;
                        work->actionCommands.result &= ~AC_RESULT_RUNNING;
                        loop = 1;
                        work->actionCommands.acState = AC_STATE_RUN + 3;
                    } else if (!(work->actionCommands.acFlags & AC_STICK_KEEP_LEFT_FLAG_NO_EARLY_RELEASE_FAIL)) {
                        work->actionCommands.result &= ~AC_RESULT_RUNNING;
                        loop = 1;
                        work->actionCommands.acState = AC_STATE_RUN + 3;
                    }
                }
                break;
            case AC_STATE_RUN + 3:
            // retail runs this empty loop over the signal sound slots 0..idx-2
            j = cw->idx - 2;
            if (j >= 0) {
                for (k = 0; k <= j; k++) {
                }
            }
                work->actionCommands.acState = AC_STATE_RUN + 4;
                break;
            case AC_STATE_RUN + 5:
                cw->endTimer = 60;
                work->actionCommands.acState = AC_STATE_RUN + 6;
                break;
            case AC_STATE_RUN + 6:
                cw->endTimer--;
                if (cw->endTimer <= 0) {
                    work->actionCommands.acState = AC_STATE_RUN + 7;
                }
                break;
            case AC_STATE_RUN + 7:
                work->actionCommands.maincb = NULL;
                work->actionCommands.dispcb = NULL;
                work->actionCommands.resultcb = NULL;
                work->actionCommands.deletecb = NULL;
                break;
        }
    } while (loop);
    return 1;
}

/** Returns the result word: 1 while running, 2 on success, 0 on failure. */
s32 battleAcResult_StickKeepLeft(BattleWork* work) {
    return work->actionCommands.result;
}

/**
 * Display callback: slides the prompt in from the left over 20 frames, lights it step by step,
 * and once the command is stopped keeps it up for 40 frames before sliding it back out;
 * the slide pauses while params[2] freezes it.
 */
void battleAcDisp_StickKeepLeft(CameraId camId, void* param) {
    BattleWork* work = (BattleWork*)param;
    BattleActions* ac = &work->actionCommands;
    SklWork* cw = (SklWork*)ac->work;
    BattleACDispParams* disp = &ac->disp;

    switch (work->actionCommands.acState) {
        case AC_STATE_WAIT_START:
        case AC_STATE_START:
        case AC_STATE_RUN:
            disp->iconStep = -1;
            disp->iconX = intplGetValue(EASING_QUADRATIC_OUT, -300.0f, 30.0f, 20 - disp->slideTimer, 20);
            actionCommandDisp(disp->iconX, disp->iconY, disp->iconStep);
            if (work->actionCommands.params[STICK_KEEP_PARAM_PAUSED] == 0 && disp->slideTimer > 0) {
                disp->slideTimer--;
            }
            break;
        case AC_STATE_RUN + 1:
            disp->iconStep = cw->idx - 1;
            disp->iconX = intplGetValue(EASING_QUADRATIC_OUT, -300.0f, 30.0f, 20 - disp->slideTimer, 20);
            actionCommandDisp(disp->iconX, disp->iconY, disp->iconStep);
            if (work->actionCommands.params[STICK_KEEP_PARAM_PAUSED] == 0 && disp->slideTimer > 0) {
                disp->slideTimer--;
            }
            break;
        case AC_STATE_RUN + 2:
        case AC_STATE_RUN + 3:
        case AC_STATE_RUN + 4:
            actionCommandDisp(30.0f, 0.0f, disp->iconStep);
            break;
        case AC_STATE_RUN + 5:
        case AC_STATE_RUN + 6:
            if (disp->slideTimer >= 40) {
                disp->iconX = intplGetValue(EASING_QUADRATIC_OUT, 30.0f, -300.0f, disp->slideTimer - 40, 20);
            } else {
                disp->iconX = 30.0f;
            }
            actionCommandDisp(disp->iconX, disp->iconY, disp->iconStep);
            if (work->actionCommands.params[STICK_KEEP_PARAM_PAUSED] == 0 && disp->slideTimer < 60) {
                disp->slideTimer++;
            }
            break;
    }
}

/** Called by btlevtcmd_StopAC: starts the slide-out and the 60-frame end wait. */
void battleAcDelete_StickKeepLeft(BattleWork* work) {
    work->actionCommands.acState = AC_STATE_RUN + 5;
}

/**
 * Draws the stick icon (flashing until it is tilted, released once the last light is on), the bar,
 * the numbered lights up to the current step and the final star;
 * acFlags bit 0 packs them closer (and lengthens the bar for four or more lights).
 */
static void actionCommandDisp(f32 x, f32 y, s32 unused) {
    s32 count;
    s32 idx;
    s32 j;
    BattleActions* ac;
    s32 i;
    s32 n;
    s32 m;
    f32 spacing;
    f32 yy;
    f32 stepX;
    char n1[8] = "_AcSkl1";
    char n2[8] = "_AcSkl2";
    char n3[8] = "_AcSkl3";
    char n4[8] = "_AcSkl4";
    char n5[8] = "_AcSkl5";

    ac = &_battleWorkPointer->actionCommands;
    camGetPtr(CAMERA_2D);
    count = ac->params[0];
    idx = ((SklWork*)ac->work)->idx - 1;
    switch (ac->acState) {
        case AC_STATE_INIT:
        case AC_STATE_WAIT_START:
        case AC_STATE_START: {
            Vec p = {0.0f, 0.0f, 0.0f};
            p.x = -200.0f + x;
            p.y = 70.0f + y;
            iconDispGx(&p, ICON_FLAG_2D, ICON_STICK_NEUTRAL, 1.0f);
            break;
        }
        case AC_STATE_RUN:
            if (gp->retraceCount & 8) {
                Vec p = {0.0f, 0.0f, 0.0f};
                p.x = -200.0f + x;
                p.y = 70.0f + y;
                iconDispGx(&p, ICON_FLAG_2D, ICON_STICK_LEFT, 1.0f);
            } else {
                Vec p = {0.0f, 0.0f, 0.0f};
                p.x = -200.0f + x;
                p.y = 70.0f + y;
                iconDispGx(&p, ICON_FLAG_2D, ICON_STICK_NEUTRAL, 1.0f);
            }
            break;
        default:
            if (idx < count) {
                Vec p = {0.0f, 0.0f, 0.0f};
                p.x = -200.0f + x;
                p.y = 70.0f + y;
                iconDispGx(&p, ICON_FLAG_2D, ICON_STICK_LEFT, 1.0f);
            } else {
                Vec p = {0.0f, 0.0f, 0.0f};
                p.x = -200.0f + x;
                p.y = 70.0f + y;
                iconDispGx(&p, ICON_FLAG_2D, ICON_STICK_NEUTRAL, 1.0f);
            }
            break;
    }

    n = 10;
    if (count >= 4) {
        n = count * 2 + 3;
        if (ac->acFlags & AC_STICK_KEEP_LEFT_FLAG_COMPACT_GAUGE) {
            n += 2;
        }
    }
    spacing = 16.0f;
    if (ac->acFlags & AC_STICK_KEEP_LEFT_FLAG_COMPACT_GAUGE) {
        spacing = 12.0f;
    }
    {
        Vec p = {0.0f, 0.0f, 0.0f};
        p.x = -288.0f + x;
        yy = 25.0f + y;
        p.y = yy;
        iconDispGx(&p, ICON_FLAG_2D, ICON_AC_BAR_LEFT, 1.0f);
    }
    for (i = 1; i < n; i++) {
        Vec p = {0.0f, 0.0f, 0.0f};
        p.x = x + (spacing * (f32)i + -288.0f);
        p.y = yy;
        iconDispGx(&p, ICON_FLAG_2D, ICON_AC_BAR_MIDDLE, 1.0f);
    }
    {
        Vec p = {0.0f, 0.0f, 0.0f};
        p.x = x + (spacing * (f32)i + -288.0f);
        p.y = yy;
        iconDispGx(&p, ICON_FLAG_2D, ICON_AC_BAR_RIGHT, 1.0f);
    }
    stepX = 33.0f;
    if (ac->acFlags & AC_STICK_KEEP_LEFT_FLAG_COMPACT_GAUGE) {
        stepX = 24.75f;
    }
    m = count - 1;
    for (j = 0; j < m; j++) {
        if (j < idx) {
            if (j < 3) {
                Vec p = {0.0f, 0.0f, 0.0f};
                p.x = x + (stepX * (f32)j + -258.0f);
                p.y = yy;
                iconDispGx(&p, ICON_FLAG_2D, (u16)(ICON_AC_LAMP_1 + j), 1.0f);
            } else {
                Vec p = {0.0f, 0.0f, 0.0f};
                p.x = x + (stepX * (f32)j + -258.0f);
                p.y = yy;
                iconDispGx(&p, ICON_FLAG_2D, ICON_AC_LAMP_3, 1.0f);
            }
        } else {
            Vec p = {0.0f, 0.0f, 0.0f};
            p.x = x + (stepX * (f32)j + -258.0f);
            p.y = yy;
            iconDispGx(&p, ICON_FLAG_2D, ICON_AC_OFF, 0.5f);
        }
    }
    if (idx < count) {
        Vec p = {0.0f, 0.0f, 0.0f};
        p.x = x + (stepX * (f32)m + -250.0f);
        p.y = yy;
        iconDispGx(&p, ICON_FLAG_2D, ICON_AC_OFF, 1.0f);
    } else {
        Vec p = {0.0f, 0.0f, 0.0f};
        p.x = x + (stepX * (f32)m + -250.0f);
        p.y = yy;
        iconDispGx(&p, ICON_FLAG_2D, ICON_AC_ON, 1.0f);
    }
}
