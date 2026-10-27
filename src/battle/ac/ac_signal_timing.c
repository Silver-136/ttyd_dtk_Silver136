#include "battle/battle_disp.h"
#include "battle/battle.h"
#include <dolphin/pad.h>
#include "battle/battle_ac.h"
#include "battle/battle_pad.h"
#include "battle/battle_unit.h"
#include "driver/camdrv.h"
#include "driver/icondrv.h"
#include "pmario_sound.h"
#include "system.h"
#include <string.h>
#include "icon_ids.h"
#include "version.h"
// US battle texture ids are 0x16 higher for these entries.
#define BTL_TEX(id) ((id) + VERSION_PICK(0, 0x16))

u16 BattleACGetButtonIcon(s32 button, s32 pressed);

typedef struct SignalTimingWork {
    s32 hits;       // 0x0
    s32 misses;     // 0x4
    s32 index;      // 0x8
    s32 next;       // 0xC
    s32 frame;      // 0x10
    s32 rangeStart; // 0x14
    s32 rangeEnd;   // 0x18
    s32 okStart;    // 0x1C
    s32 okEnd;      // 0x20
    s32 pressed;    // 0x24
    s32 nextRangeEnd;      // 0x28
    s32 difficulty; // 0x2C
    s32 okFlag;     // 0x30
    u32 button;     // 0x34
    u32 buttons[5]; // 0x38
    s8 results[8];  // 0x4C
    s32 endTimer;   // 0x54
} SignalTimingWork;

#define ST_COUNT(w) ((w)->actionCommands.params[0])    /* number of signals */
#define ST_MODE(w) ((w)->actionCommands.params[1])     /* 1: use the wider ok-frame table */
#define ST_STEP(w) ((w)->actionCommands.params[2])     /* frames per signal */
#define ST_NEEDED(w) ((w)->actionCommands.params[3])   /* hits needed for success */
#define ST_BUTTONS(w) ((w)->actionCommands.params[4])  /* 1: d-pad directions, 2: A/B/X/Y */
#define ST_EXTRA(w) ((w)->actionCommands.params[5])    /* added to hits for the displayed count */
#define ST_LEAD(w) ((w)->actionCommands.params[6])     /* lead-in signals before the first button */
#define ST_OKBASE(w) ((w)->actionCommands.params[7])   /* base ok frame range */
#define ST_OUT_HITS(w) ((w)->actionCommands.outputParams[0])
#define ST_OUT_RESULT(w) ((w)->actionCommands.outputParams[1])

u32 _button_table_4dir[4] = {0x10000, 0x20000, 0x40000, 0x80000};
u32 _button_table_abxy[4] = {PAD_BUTTON_A, PAD_BUTTON_B, PAD_BUTTON_X, PAD_BUTTON_Y};
s32 ac_signaltiming_ok_frame_range[7] = {20, 16, 12, 10, 6, 4, 2};
s32 ac_signaltiming_ok_frame_range_2[7] = {30, 30, 30, 30, 25, 20, 15};

static void actionCommandDisp(f32 x, f32 y);
s32 _get_ok_frame_range(s32 difficulty);

s32 _get_ok_frame_range(s32 difficulty) {
    BattleWork* work = _battleWorkPointer;
    switch (ST_MODE(work)) {
        case 1:
            return ST_OKBASE(work) + ac_signaltiming_ok_frame_range_2[difficulty];
    }
    return ST_OKBASE(work) + ac_signaltiming_ok_frame_range[difficulty];
}

s32 battleAcMain_SignalTiming(BattleWork *work) {
    const char *sfxPi;
    const char *sfxPone;
    const char *sfxOk;
    const char *sfxNg;
    const char *sfxNg2; // FAKE: second name for the NG sound; the chained assignment puts its address first in the pre-loop block
    s32 j;
    s32 i;
    s32 k;
    BOOL unique;
    BattleActions *ac = &work->actionCommands;
    SignalTimingWork *w = (SignalTimingWork*)ac->work;
    BattleACDispParams *disp = (BattleACDispParams*)&ac->disp;
    BOOL loop;
    BOOL autoCommand = 0;
    BOOL got;
    BOOL good;
    u32 btn;
    BOOL done;
    if (ac->mAcUnit->badgesEquipped.AutoCommand) {
        autoCommand = 1;
    }
    sfxPi = "SFX_AC_PI1";
    sfxPone = "SFX_AC_PONE1";
    sfxOk = "SFX_AC_COMMAND_OK1";
    sfxNg2 = sfxNg = "SFX_AC_COMMAND_NG1";
    do {
        loop = 0;
        got = 0;
        switch (ac->mAcState) {
            case 0:
                ac->result = 1;
                memset(disp, 0, sizeof(BattleACDispParams));
                disp->iconX = -300.0f;
                disp->iconY = 0.0f;
                *(&disp->slideTimer) = 20;
                w->hits = 0;
                w->misses = 0;
                w->frame = 0;
                w->index = 0;
                w->next = 0;
                w->difficulty = BattleActionCommandGetDifficulty(work);
                w->okFlag = 0;
                ST_OUT_RESULT(work) = 0;
                for (k = 0; k < 5; k++) {
                    w->results[k] = 0;
                }

                switch ST_BUTTONS(work) {
                    case 1:

                    case 2:
                        for (i = 0; i < ST_COUNT(work); i++) {
                            done = 0;
                            while (!done) {
                                if (ST_BUTTONS(work) == 1) {
                                    btn = _button_table_4dir[irand(4)];
                                } else {
                                    btn = _button_table_abxy[irand(4)];
                                }
                                unique = 1;
                                for (j = i - 1; j >= 0; j--) {
                                    if (w->buttons[j] == btn) {
                                        unique = 0;
                                    }
                                }

                                if (unique) {
                                    done = 1;
                                }
                            }

                            w->buttons[i] = btn;
                        }

                        w->button = w->buttons[0];
                        break;

                    default:
                        w->button = 0x100;
                        for (i = 0; i < ST_COUNT(work); i++) {
                            w->buttons[i] = 0x100;
                        }

                        w->button = w->buttons[0];

                }

                ac->mAcState = 99;
                break;

            case 99:
                break;

            case 100:
                if (work->actionCommands.countdown > 0) {
                    work->actionCommands.countdown--;
                    ac->result = 2;
                    loop = 1;
                    ac->mAcState = 1002;
                    break;
                }
                w->rangeStart = w->index * ST_STEP(work);
                w->rangeEnd = ((w->index + 1) * ST_STEP(work)) - 1;
                w->okStart = w->rangeStart;
                j = _get_ok_frame_range(w->difficulty) - 1;
                w->okEnd = w->okStart + j;
                ac->mAcState = 1000;

            case 1000:
                w->nextRangeEnd = ((w->next + 1) * ST_STEP(work)) - 1;
                w->next++;
                if (w->next < (ST_LEAD(work) + 1)) {
                    if (!((work->actionCommands.setupFlags & 1) && (work->actionCommands.mAcUnit->mTokenFlags & kConfusedAction))) {
                        psndSFXOn(sfxPi);
                    }
                } else
                  if (!((work->actionCommands.setupFlags & 1) && (work->actionCommands.mAcUnit->mTokenFlags & kConfusedAction))) {
                    psndSFXOn(sfxPone);
                }
                w->pressed = 0;
                ac->mAcState = 1001;

            case 1001:
                w->frame++;
                if (w->pressed) {
                    w->okFlag = 0;
                } else
                  if (w->index >= ST_LEAD(work)) {
                    if ((w->frame >= w->okStart) && (w->frame <= w->okEnd)) {
                        w->okFlag = 1;
                    } else {
                        w->okFlag = 0;
                    }
                } else {
                    w->okFlag = 0;
                }
                good = 0;
                if (BattlePadCheckTrigger(w->button)) {
                    got = 1;
                }
                if (got) {
                    switch ST_BUTTONS(work) {
                        case 2:
                            if (w->button == BattlePadGetTrigger()) {
                                good = 1;
                            } else {
                                good = 0;
                            }
                            break;

                        default:
                            good = 1;

                    }

                }
                if (autoCommand && w->okFlag) {
                    got = 1;
                    good = 1;
                }
                if ((w->hits + w->misses) >= ST_COUNT(work)) {
                    got = 0;
                }
                if (((w->index >= ST_LEAD(work)) && got) && (!w->pressed)) {
                    w->pressed = 1;
                    if (((w->frame >= w->okStart) && (w->frame <= w->okEnd)) && good) {
                        w->hits++;
                        ST_OUT_RESULT(work) = 1;
                        if ((w->index >= ST_LEAD(work)) && (w->index < (ST_COUNT(work) + ST_LEAD(work)))) {
                            w->results[w->index - ST_LEAD(work)] = 1;
                            if (!((work->actionCommands.setupFlags & 1) && (work->actionCommands.mAcUnit->mTokenFlags & kConfusedAction))) {
                                psndSFXOn(sfxOk);
                            }
                        }
                        if (work->actionCommands.acFlags & AC_SIGNAL_TIMING_FLAG_END_ON_HIT) {
                            ST_OUT_HITS(work) = w->hits;
                            ac->result &= ~1;
                            if (w->hits >= ST_NEEDED(work)) {
                                ac->result |= 2;
                            }
                            loop = 1;
                            ac->mAcState = 1002;
                        }
                    } else
                      if ((w->index >= ST_LEAD(work)) && (w->index < (ST_COUNT(work) + ST_LEAD(work)))) {
                        w->misses++;
                        if (w->frame >= w->rangeStart) {
                            w->results[w->index - ST_LEAD(work)] = -1;
                            if (!((work->actionCommands.setupFlags & 1) && (work->actionCommands.mAcUnit->mTokenFlags & kConfusedAction))) {
                                psndSFXOn(sfxNg2);
                            }
                            ST_OUT_RESULT(work) = -1;
                        } else {
                            w->pressed = 0;
                        }
                    }
                }
                if ((!w->pressed) && (w->frame > w->okEnd)) {
                    w->pressed = 1;
                    ST_OUT_RESULT(work) = -1;
                    if ((w->index >= ST_LEAD(work)) && (w->index < (ST_COUNT(work) + ST_LEAD(work)))) {
                        w->misses++;
                        w->results[w->index - ST_LEAD(work)] = -1;
                        if (!((work->actionCommands.setupFlags & 1) && (work->actionCommands.mAcUnit->mTokenFlags & kConfusedAction))) {
                            psndSFXOn(sfxNg);
                        }
                    }
                }
                if (w->pressed && (w->index < (ST_COUNT(work) + ST_LEAD(work)))) {
                    w->index++;
                    if (w->index < (ST_COUNT(work) + ST_LEAD(work))) {
                        w->rangeStart = (w->index * ST_STEP(work)) - 10;
                        w->rangeEnd = ((w->index + 1) * ST_STEP(work)) - 1;
                        w->okStart = w->index * ST_STEP(work);
                        j = _get_ok_frame_range(w->difficulty) - 1;
                        w->okEnd = w->okStart + j;
                        w->pressed = 0;
                        if ((w->index >= ST_LEAD(work)) && (w->index < (ST_COUNT(work) + ST_LEAD(work)))) {
                            w->button = w->buttons[w->index - ST_LEAD(work)];
                        }
                    }
                }
                if (w->frame >= (w->next * ST_STEP(work))) {
                    if (w->next < (ST_COUNT(work) + ST_LEAD(work))) {
                        ac->mAcState = 1000;
                    } else {
                        ST_OUT_HITS(work) = w->hits;
                        ac->result &= ~1;
                        if (w->hits >= ST_NEEDED(work)) {
                            ac->result |= 2;
                        }
                        loop = 1;
                        ac->mAcState = 1002;
                    }
                }
                break;

            case 1002:
                ac->mAcState = 1003;
                break;

            case 1003:
                break;

            case 1004:
                work->actionCommands.doneFlags |= 1;
                w->okFlag = 0;
                w->endTimer = 60;
                ac->mAcState = 1005;
                break;

            case 1005:
                w->endTimer--;
                if (w->endTimer <= 0) {
                    ac->mAcState = 1006;
                }
                break;

            case 1006:
                ac->maincb = NULL;
                ac->dispcb = NULL;
                ac->resultcb = NULL;
                ac->deletecb = NULL;
                break;

        }

    }
    while (loop);
    return 1;
}

s32 battleAcResult_SignalTiming(BattleWork* work) {
    return work->actionCommands.result;
}

void battleAcDisp_SignalTiming(CameraId cameraId, BattleWork* work) {
    BattleActions* ac = &work->actionCommands;
    BattleACDispParams* disp = (BattleACDispParams*)&ac->disp;

    switch (ac->mAcState) {
        case 99:
        case 100:
            disp->iconX = intplGetValue(EASING_QUADRATIC_OUT, -300.0f, 30.0f, 20 - disp->slideTimer, 20);
            actionCommandDisp(disp->iconX, disp->iconY);
            if (disp->slideTimer > 0) {
                disp->slideTimer--;
            }
            break;
        case 1000:
        case 1001:
        case 1002:
        case 1003:
            actionCommandDisp(30.0f, 0.0f);
            break;
        case 1004:
        case 1005:
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

void battleAcDelete_SignalTiming(BattleWork* work) {
    work->actionCommands.mAcState = 1004;
}

static void actionCommandDisp(f32 x, f32 y) {
    BattleActions* ac = &_battleWorkPointer->actionCommands;
    // FAKE: work recovered from ac (container-of through offsetof) so its web is a codegen temp numbered above the
    // rodata base; then work takes r31 and the base r30, as in the target
    BattleWork* work = (BattleWork*)((u8*)ac - offsetof(BattleWork, actionCommands));
    SignalTimingWork* w;
    s32 i;
    s32 count;
    s32 next;
    s32 n;
    s32 num;
    f32 yb;
    f32 yd;
    f32 fx;
    f32 yc;
    Mtx mOut;
    Mtx mTrans;
    Mtx mScale;

    camGetPtr(CAMERA_2D);
    w = (SignalTimingWork*)ac->work;
    count = ST_COUNT(work);
    next = w->next;
    if (!(work->actionCommands.acFlags & AC_SIGNAL_TIMING_FLAG_SHOW_BUTTONS)) {
        if (w->okFlag) {
            Vec pos = {0.0f, 0.0f, 0.0f};
            pos.x = -200.0f + x;
            pos.y = 70.0f + y;
            iconDispGx(&pos, 0x10, ICON_BUTTON_A_PRESSED, 1.0f);
        } else {
            Vec pos = {0.0f, 0.0f, 0.0f};
            pos.x = -200.0f + x;
            pos.y = 70.0f + y;
            iconDispGx(&pos, 0x10, ICON_BUTTON_A, 1.0f);
        }
    }

    switch (count) {
        case 1:
            n = ST_LEAD(work) * 2 + 5;
            break;
        case 3:
            n = ST_LEAD(work) * 2 + 11;
            break;
        case 4:
            n = ST_LEAD(work) * 2 + 13;
            break;
        case 5:
            n = ST_LEAD(work) * 2 + 15;
            break;
        default:
            n = ST_LEAD(work) * 2 + 11;
            break;
    }

    {
        Vec pos = {0.0f, 0.0f, 0.0f};
        pos.x = -288.0f + x;
        pos.y = yb = 25.0f + y;
        iconDispGx(&pos, 0x10, ICON_AC_BAR_LEFT, 1.0f);
    }
    for (i = 1; i < n; i++) {
        Vec pos = {0.0f, 0.0f, 0.0f};
        pos.x = x + (f32)(i * 16 - 288);
        pos.y = yb;
        iconDispGx(&pos, 0x10, ICON_AC_BAR_MIDDLE, 1.0f);
    }
    {
        Vec pos = {0.0f, 0.0f, 0.0f};
        pos.x = x + (f32)(i * 16 - 288);
        pos.y = yb;
        iconDispGx(&pos, 0x10, ICON_AC_BAR_RIGHT, 1.0f);
    }

    if (!(work->actionCommands.acFlags & AC_SIGNAL_TIMING_FLAG_HIDE_HIT_COUNT)) {
        fx = -200.0f + x;
        num = w->hits + ST_EXTRA(work);
        // Both arms of the SHOW_BUTTONS test are the same (retail code): the counter's position does not depend on it.
        PSMTXTrans(mTrans, fx + (f32)((work->actionCommands.acFlags & AC_SIGNAL_TIMING_FLAG_SHOW_BUTTONS) ? count * 0x30 - 26 + ST_LEAD(work) * 32 : count * 0x30 - 26 + ST_LEAD(work) * 32), yb, 0.0f);
        if (work->actionCommands.acFlags & AC_SIGNAL_TIMING_FLAG_LARGE_HIT_COUNT) {
            PSMTXScale(mScale, 1.5f, 1.5f, 1.5f);
        } else {
            PSMTXScale(mScale, 1.0f, 1.0f, 1.0f);
        }
        PSMTXConcat(mTrans, mScale, mOut);
        iconNumberDispGx(mOut, num, 0, (GXColor){255, 255, 255, 255});
    }

    for (i = 0; i < ST_LEAD(work); i++) {
        if (i < w->next) {
            if (i < ST_LEAD(work)) {
                Vec pos = {0.0f, 0.0f, 0.0f};
                pos.x = x + (f32)(i * 33 - 258);
                pos.y = yb;
                iconDispGx(&pos, 0x10, (u16)(ICON_AC_LAMP_1 + i), 1.0f);
            } else {
                Vec pos = {0.0f, 0.0f, 0.0f};
                pos.x = x + (f32)(i * 33 - 258);
                pos.y = yb;
                iconDispGx(&pos, 0x10, ICON_AC_LAMP_3, 1.0f);
            }
        } else {
            Vec pos = {0.0f, 0.0f, 0.0f};
            pos.x = x + (f32)(i * 33 - 258);
            pos.y = yb;
            iconDispGx(&pos, 0x10, ICON_AC_OFF, 0.5f);
        }
    }

    if (!(work->actionCommands.acFlags & AC_SIGNAL_TIMING_FLAG_SHOW_BUTTONS)) {
        for (i = 0; i < count; i++) {
            if (i + ST_LEAD(work) < next) {
                Vec pos = {0.0f, 0.0f, 0.0f};
                pos.x = x + (f32)(i * 48 + (ST_LEAD(work) * 33 - 250));
                pos.y = yb;
                iconDispGx(&pos, 0x10, ICON_AC_ON, 1.0f);
            } else {
                Vec pos = {0.0f, 0.0f, 0.0f};
                pos.x = x + (f32)(i * 48 + (ST_LEAD(work) * 33 - 250));
                pos.y = yb;
                iconDispGx(&pos, 0x10, ICON_AC_OFF, 1.0f);
            }
        }
    } else if (!(work->actionCommands.acFlags & AC_SIGNAL_TIMING_FLAG_HIDE_UPCOMING_BUTTONS)) {
        yc = 15.0f + y;
        for (i = 0; i < count; i++) {
            f32 scale = 0.8f;
            if (w->next == (i + ST_LEAD(work)) && w->frame >= ST_STEP(work) * (i + ST_LEAD(work)) - 5 && w->frame < ST_STEP(work) * (i + ST_LEAD(work))) {
                Vec pos = {0.0f, 0.0f, 0.0f};
                pos.x = x + (f32)(i * 48 + (ST_LEAD(work) * 33 - 250));
                pos.y = yc;
                iconDispGx(&pos, 0x10, BattleACGetButtonIcon(w->buttons[i], 0), 0.8f);
            } else {
                s32 dy;
                if (w->next <= (i + ST_LEAD(work))) {
                    scale = 0.6f;
                    dy = -2;
                } else {
                    dy = -10;
                }
                {
                    Vec pos = {0.0f, 0.0f, 0.0f};
                    pos.x = x + (f32)(i * 48 + (ST_LEAD(work) * 33 - 250));
                    pos.y = (f32)(dy + 25) + y;
                    iconDispGx(&pos, 0x10, BattleACGetButtonIcon(w->buttons[i], 1), scale);
                }
            }
        }
    } else {
        yc = 20.800001f + yb;
        for (i = 0; i < count; i++) {
            if (w->next == (i + ST_LEAD(work)) && w->frame >= ST_STEP(work) * (i + ST_LEAD(work)) - 20 && w->frame <= ST_STEP(work) * (i + ST_LEAD(work))) {
                Vec pos = {0.0f, 0.0f, 0.0f};
                pos.x = x + (f32)(i * 48 + (ST_LEAD(work) * 33 - 250));
                pos.y = yb;
                iconDispGx(&pos, 0x10, BattleACGetButtonIcon(w->buttons[i], 0), 0.8f);
            } else if (w->next <= (i + ST_LEAD(work))) {
                if (i >= count - 1) {
                    btlDispTexPlane(x + (f32)(i * 48 + (ST_LEAD(work) * 33 - 250)), yc, 0.0f, 0.8f, 0.8f, BTL_TEX(0x40), (GXColor){255, 255, 255, 255}, 0);
                }
            } else {
                Vec pos = {0.0f, 0.0f, 0.0f};
                pos.x = x + (f32)(i * 48 + (ST_LEAD(work) * 33 - 250));
                pos.y = yb;
                iconDispGx(&pos, 0x10, BattleACGetButtonIcon(w->buttons[i], 1), 0.8f);
            }
        }
    }

    yd = yb - 25.0f;
    for (i = 0; i < count; i++) {
        if (w->results[i] == 1) {
            btlDispTexPlane(x + (f32)(i * 48 + (ST_LEAD(work) * 33 - 250)), yd, 0.0f, 1.0f, 1.0f, BTL_TEX(0x3E), (GXColor){255, 255, 255, 255}, 0);
        } else if (w->results[i] == -1) {
            btlDispTexPlane(x + (f32)(i * 48 + (ST_LEAD(work) * 33 - 250)), yd, 0.0f, 1.0f, 1.0f, BTL_TEX(0x3F), (GXColor){255, 255, 255, 255}, 0);
        }
    }
}

