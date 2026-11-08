// Action command: hold the stick left and the sub stick right (ac_lst_rc_keep).

#include "battle/ac/ac_table.h"
#include "battle/battle.h"
#include "battle/battle_ac.h"
#include "battle/battle_pad.h"
#include "battle/battle_unit.h"
#include "driver/camdrv.h"
#include "driver/icondrv.h"
#include "system.h"
#include <string.h>
#include "icon_ids.h"

typedef struct LstRcWork {
    s32 timer;    // 0x0
    s32 count;    // 0x4, steps elapsed (0..5)
    s16 stickLeftFrames;    // 0x8, control stick left: frames held (state 1000), then frames since release (1001)
    s16 substickRightFrames;    // 0xA, C-stick right: frames held (state 1000), then frames since release (1001)
    s32 endTimer; // 0xC
} LstRcWork;

/**
 * Release window of the unused two-stick hammer charge command, by difficulty (80 frames at the easiest
 * down to 5), and the names of the star icons it removes when it ends.
 */
s32 ac_lstrckeep_ok_frame[] = {0x50, 0x46, 0x3C, 0x28, 0x14, 0x0A, 0x05};
const char* ac_lstrckeep_iconname[] = {"_AcSkl1", "_AcSkl2", "_AcSkl3", "_AcSkl4"};

static void actionCommandDisp(f32 x, f32 y, s32 n);

/**
 * Per-frame driver of action command 4, which no battle script in the tree sets up: a two-stick hammer charge.
 * Tilt the Control Stick left and the C-Stick right within 120 frames,
 * keep holding while three stars light (step lengths params[0..3]),
 * then let go of both during the last step (params[4] frames) within a difficulty window of 80 down to 5 frames.
 * Letting go early, or for over 5 frames, or never, fails.
 */
s32 battleAcMain_LstRcKeep(BattleWork* work) {
    BattleActions* ac = &work->actionCommands;
    LstRcWork* lw = (LstRcWork*)ac->work;
    BattleACDispParams* disp = &ac->disp;
    s32 loop;
    s32 off;
    s32 autoCmd = 0;
    s32 ok;
    s32 d;
    s32 tm;
    s32 n;
    s32 i;
    if (ac->acUnit->badgesEquipped.AutoCommand) {
        autoCmd = 1;
    }
    do {
        loop = 0;
        switch (ac->acState) {
            case AC_STATE_INIT:
                ac->result = AC_RESULT_RUNNING;
                lw->count = 0;
                memset(disp, 0, sizeof(BattleACDispParams));
                disp->iconX = -300.0f;
                disp->iconY = 0.0f;
                disp->slideTimer = 20;
                ac->acState = AC_STATE_WAIT_START;
                break;
            case AC_STATE_WAIT_START:
                break;
            case AC_STATE_START:
                lw->timer = 120;
                ac->acState = AC_STATE_RUN;
                lw->stickLeftFrames = 0;
                lw->substickRightFrames = 0;
            case AC_STATE_RUN:
                if ((ac->setupFlags & AC_SETUP_FLAG_CONFUSED_AUTO) && (ac->acUnit->tokenFlags & kConfusedAction)) {
                    if (ac->setupFlags & AC_SETUP_FLAG_RETRIES) {
                        ac->setupParam--;
                        if (ac->setupParam >= 0) {
                            break;
                        }
                    }

                    // irand(100) is never negative: a confused auto-play always fails
                    if (irand(100) >= 0) {
                        ac->result = 0;
                    } else {
                        ac->result = AC_RESULT_SUCCESS;
                        ac->resultCount++;
                    }
                    return 0;
                }
                if (lw->stickLeftFrames == 0) {
                    if (BattlePadCheckTrigger(BATTLE_PAD_STICK_LEFT)) {
                        lw->stickLeftFrames = 1;
                    }
                } else {
                    lw->stickLeftFrames++;
                    if (BattlePadCheckNow(BATTLE_PAD_STICK_LEFT) == 0) {
                        lw->stickLeftFrames = 0;
                    }
                }
                if (lw->substickRightFrames == 0) {
                    if (BattlePadCheckTrigger(BATTLE_PAD_SUBSTICK_RIGHT)) {
                        lw->substickRightFrames = 1;
                    }
                } else {
                    lw->substickRightFrames++;
                    if (BattlePadCheckNow(BATTLE_PAD_SUBSTICK_RIGHT) == 0) {
                        lw->substickRightFrames = 0;
                    }
                }
                if ((lw->stickLeftFrames != 0 && lw->substickRightFrames != 0) || ac->countdown > 0 || autoCmd) {
                    ac->acState = AC_STATE_RUN + 1;
                    lw->timer = ac->params[0];
                    lw->substickRightFrames = lw->stickLeftFrames = lw->count = 0;
                    break;
                }
                lw->timer--;
                if (lw->timer <= 0) {
                    ac->result = 0;
                    loop = 1;
                    ac->acState = AC_STATE_RUN + 3;
                }
                break;
            case AC_STATE_RUN + 1:
                if (ac->countdown > 0) {
                    ac->countdown--;
                    ac->result = AC_RESULT_SUCCESS;
                    loop = 1;
                    ac->acState = AC_STATE_RUN + 3;
                }
                lw->timer--;
                if (lw->stickLeftFrames > 0) {
                    lw->stickLeftFrames++;
                } else if (BattlePadCheckNow(BATTLE_PAD_STICK_LEFT) == 0) {
                    lw->stickLeftFrames = 1;
                }
                if (lw->substickRightFrames > 0) {
                    lw->substickRightFrames++;
                } else if (BattlePadCheckNow(BATTLE_PAD_SUBSTICK_RIGHT) == 0) {
                    lw->substickRightFrames = 1;
                }
                if (!autoCmd) {
                    if (lw->stickLeftFrames > 5 || lw->substickRightFrames > 5) {
                        ac->result &= ~AC_RESULT_RUNNING;
                        loop = 1;
                        ac->acState = AC_STATE_RUN + 3;
                        break;
                    }
                }
                tm = lw->timer;
                if (tm < 0) {
                    lw->count++;
                    if (lw->count >= 5) {
                        ac->result &= ~AC_RESULT_RUNNING;
                        loop = 1;
                        ac->acState = AC_STATE_RUN + 3;
                        break;
                    }
                    lw->timer = ac->params[lw->count];
                    break;
                }
                if (!(lw->stickLeftFrames != 0 && lw->substickRightFrames != 0) && !autoCmd) {
                    break;
                }
                ok = 0;
                if (lw->count == 4) {
                    d = ac->params[4] - tm;
                    if (d <= ac_lstrckeep_ok_frame[BattleActionCommandGetDifficulty(work)]) {
                        ok = 1;
                    }
                }
                if (autoCmd) {
                    if (!ok) {
                        break;
                    }
                    ac->result |= AC_RESULT_SUCCESS;
                    ac->result &= ~AC_RESULT_RUNNING;
                    loop = 1;
                    ac->acState = AC_STATE_RUN + 3;
                    break;
                }
                if (ok) {
                    ac->result |= AC_RESULT_SUCCESS;
                    ac->result &= ~AC_RESULT_RUNNING;
                    loop = 1;
                    ac->acState = AC_STATE_RUN + 3;
                    break;
                }
                ac->result &= ~AC_RESULT_RUNNING;
                loop = 1;
                ac->acState = AC_STATE_RUN + 3;
                break;
            case AC_STATE_RUN + 3:
                // FAKE: byte-offset index (off >> 2) and the n >= 0 guard; the plain loop over
                // ac_lstrckeep_iconname[i] changes the object
                n = lw->count - 1;
                if (n >= 0) {
                    for (i = 0, off = 0; i <= n; i++, off += 4) {
                        iconDelete(ac_lstrckeep_iconname[off >> 2]);
                    }
                }
                ac->acState = AC_STATE_RUN + 4;
            case AC_STATE_RUN + 4:
            case AC_STATE_RUN + 5:
                lw->endTimer = 60;
                ac->acState = AC_STATE_RUN + 6;
                break;
            case AC_STATE_RUN + 6:
                lw->endTimer--;
                if (lw->endTimer <= 0) {
                    ac->acState = AC_STATE_RUN + 7;
                }
                break;
            case AC_STATE_RUN + 7:
                ac->maincb = NULL;
                ac->dispcb = NULL;
                ac->resultcb = NULL;
                ac->deletecb = NULL;
                break;
        }
    } while (loop);
    return 1;
}

/** Returns the result word: 1 while running, 2 on success, 0 on failure. */
s32 battleAcResult_LstRcKeep(BattleWork* work) {
    return work->actionCommands.result;
}

/**
 * Display callback: slides the prompt in from the left over 20 frames, lights the stars as the steps pass,
 * and after the command is stopped keeps it up for 40 frames before sliding it back out.
 */
void battleAcDisp_LstRcKeep(CameraId cameraId, void* param) {
    BattleWork* work = (BattleWork*)param;
    BattleActions* ac = &work->actionCommands;
    BattleACDispParams* disp = &ac->disp;
    LstRcWork* lw = (LstRcWork*)ac->work;
    switch (ac->acState) {
        case AC_STATE_WAIT_START:
        case AC_STATE_START:
        case AC_STATE_RUN:
            disp->iconStep = -1;
            disp->iconX = intplGetValue(EASING_QUADRATIC_OUT, -300.0f, 30.0f, 20 - disp->slideTimer, 20);
            actionCommandDisp(disp->iconX, disp->iconY, disp->iconStep);
            if (disp->slideTimer > 0) {
                (disp->slideTimer)--;
            }
            break;
        case AC_STATE_RUN + 1:
            disp->iconStep = lw->count - 1;
            disp->iconX = intplGetValue(EASING_QUADRATIC_OUT, -300.0f, 30.0f, 20 - disp->slideTimer, 20);
            actionCommandDisp(disp->iconX, disp->iconY, disp->iconStep);
            if (disp->slideTimer > 0) {
                (disp->slideTimer)--;
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
            if (disp->slideTimer < 60) {
                (disp->slideTimer)++;
            }
            break;
    }
}

/** Called by btlevtcmd_StopAC: starts the 60-frame wait after which the command clears its callbacks. */
void battleAcDelete_LstRcKeep(BattleWork* work) {
    work->actionCommands.acState = AC_STATE_RUN + 5;
}

// actionCommandDisp size-optimised, as in the original (the object differs without it)
#pragma optimize_for_size on
/**
 * Draws the two stick icons (shown tilted until the last step),
 * the bar and its three numbered stars lit up to step n, and the final star, lit on the last step.
 */
static void actionCommandDisp(f32 x, f32 y, s32 n) {
    s32 off;
    s32 i;
    camGetPtr(CAMERA_2D);
    if (n < 3) {
        Vec pos = {0.0f, 0.0f, 0.0f};
        pos.x = -168.0f + x;
        pos.y = 70.0f + y;
        iconDispGx(&pos, ICON_FLAG_2D, ICON_SUBSTICK_RIGHT, 1.0f);
    } else {
        Vec pos = {0.0f, 0.0f, 0.0f};
        pos.x = -168.0f + x;
        pos.y = 70.0f + y;
        iconDispGx(&pos, ICON_FLAG_2D, ICON_SUBSTICK_NEUTRAL, 1.0f);
    }
    if (n < 3) {
        Vec pos = {0.0f, 0.0f, 0.0f};
        pos.x = -232.0f + x;
        pos.y = 70.0f + y;
        iconDispGx(&pos, ICON_FLAG_2D, ICON_STICK_LEFT, 1.0f);
    } else {
        Vec pos = {0.0f, 0.0f, 0.0f};
        pos.x = -232.0f + x;
        pos.y = 70.0f + y;
        iconDispGx(&pos, ICON_FLAG_2D, ICON_STICK_NEUTRAL, 1.0f);
    }
    {
        Vec pos = {0.0f, 0.0f, 0.0f};
        pos.x = -200.0f + x;
        y = 25.0f + y;
        pos.y = y;
        iconDispGx(&pos, ICON_FLAG_2D, ICON_AC_BAR_MIDDLE, 1.0f);
    }
    i = 0;
    off = 0;
    for (; i < 3; i++, off += 0x21) {
        if (i <= n) {
            Vec pos = {0.0f, 0.0f, 0.0f};
            pos.x = x + (f32)(off - 0x102);
            pos.y = y;
            iconDispGx(&pos, ICON_FLAG_2D, ICON_AC_LAMP_1 + i, 1.0f);
        } else {
            Vec pos = {0.0f, 0.0f, 0.0f};
            pos.x = x + (f32)(off - 0x102);
            pos.y = y;
            iconDispGx(&pos, ICON_FLAG_2D, ICON_AC_OFF, 0.5f);
        }
    }
    if (n < 3) {
        Vec pos = {0.0f, 0.0f, 0.0f};
        pos.x = -151.0f + x;
        pos.y = y;
        iconDispGx(&pos, ICON_FLAG_2D, ICON_AC_OFF, 1.0f);
    } else {
        Vec pos = {0.0f, 0.0f, 0.0f};
        pos.x = -151.0f + x;
        pos.y = y;
        iconDispGx(&pos, ICON_FLAG_2D, ICON_AC_ON, 1.0f);
    }
}
#pragma optimize_for_size reset
