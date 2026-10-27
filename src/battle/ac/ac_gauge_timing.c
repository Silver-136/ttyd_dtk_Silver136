#include "icon_ids.h"
#include "battle/battle.h"
#include "battle/battle_ac.h"
#include "battle/battle_pad.h"
#include "battle/battle_unit.h"
#include "driver/camdrv.h"
#include "driver/icondrv.h"
#include "pmario_sound.h"
#include "system.h"
#include <string.h>
#include "battle/battle_disp.h"

u16 BattleACGetButtonIcon(s32 button, s32 pressed);
void BattleAcDrawGauge(s32 x, s32 y, s32 width, f32 ratio, s32 level, s32 a, s32 b, s32 c, s32 flags);

static void actionCommandDisp(f32 x, f32 y);

// BattleActions.params slots this command reads (btlevtcmd_AcSetParamAll)
typedef enum GaugeTimingParam {
    GAUGE_PARAM_TYPE = 0,
    GAUGE_PARAM_MAX_FRAMES = 1,
    GAUGE_PARAM_MODE = 2,
    GAUGE_PARAM_TARGET = 3,
    GAUGE_PARAM_PERIOD = 4,
} GaugeTimingParam;

typedef struct GaugeWork {
    s32 frame;     // 0x0
    f32 gauge;     // 0x4
    s32 cnt;       // 0x8
    s32 maxFrames; // 0xC
    s32 inRange;   // 0x10
    s8 passed;     // 0x14
    s8 phase;      // 0x15
    s8 hold;       // 0x16
    u8 pad17;      // 0x17
    u32 button;   // 0x18
    s32 timer;    // 0x1C
    s32 hits;     // 0x20
    u32 sfxId;    // 0x24
    s32 endTimer; // 0x28
} GaugeWork;


s32 battleAcMain_GaugeTiming(BattleWork* work) {
    GaugeWork* cw;
    BOOL autoCommand;
    BattleACDispParams* disp;
    BattleActions* ac;

    ac = &work->actionCommands;
    autoCommand = FALSE;
    cw = (GaugeWork*)ac->work;
    disp = &ac->disp;
    if (work->actionCommands.mAcUnit->badgesEquipped.AutoCommand) {
        autoCommand = TRUE;
    }

    switch (work->actionCommands.mAcState) {
        case 0:
            memset(disp, 0, sizeof(BattleACDispParams));
            disp->iconX = -300.0f;
            disp->iconY = 0.0f;
            disp->slideTimer = 20;
            disp->gaugeRatio = 0.0f;
            disp->color = (GXColor){255, 255, 255, 255};
            cw->frame = 0;
            cw->cnt = 0;
            work->actionCommands.outputParams[0] = 0;
            work->actionCommands.mAcState = 99;
            work->actionCommands.result = AC_RESULT_RUNNING;
            cw->maxFrames = work->actionCommands.params[GAUGE_PARAM_MAX_FRAMES];
            cw->inRange = 0;
            cw->timer = 0;
            cw->hits = 0;
            cw->sfxId = -1;
            switch (work->actionCommands.params[GAUGE_PARAM_TYPE]) {
                case 0:
                    cw->hold = 1;
                    cw->button = PAD_TRIGGER_R;
                    cw->phase = 1;
                    work->actionCommands.outputParams[0] = 1;
                    break;
                case 2:
                    cw->hold = 0;
                    cw->button = BATTLE_PAD_STICK_LEFT;
                    cw->phase = 0;
                    break;
                case 1:
                default:
                    cw->hold = 1;
                    cw->button = PAD_BUTTON_A;
                    cw->phase = 1;
                    work->actionCommands.outputParams[0] = 1;
                    break;
            }
            switch (work->actionCommands.params[GAUGE_PARAM_MODE]) {
                case 1:
                    cw->gauge = 0.0f;
                    work->actionCommands.params[GAUGE_PARAM_PERIOD] &= ~1;
                    break;
                default:
                    cw->gauge = 50.0f;
                    break;
            }
            break;
        case 99:
            break;
        case 100:
            if ((work->actionCommands.setupFlags & 1) && (work->actionCommands.mAcUnit->mTokenFlags & kConfusedAction)) {
                if (irand(100) >= 0) {
                    work->actionCommands.setupParam = 30;
                    cw->passed = 0;
                } else {
                    cw->passed = 1;
                    work->actionCommands.setupParam = 30;
                }
                work->actionCommands.mAcState = 1002;
                break;
            }
            if (cw->hold) {
                work->actionCommands.mAcState = 1001;
            } else {
                work->actionCommands.mAcState = 1000;
            }
        case 1000:
        case 1001:
            cw->frame++;
            switch (cw->phase) {
                case 0:
                    if (!cw->hold && BattlePadCheckNow(cw->button)) {
                        cw->phase = 1;
                        work->actionCommands.mAcState = 1001;
                        cw->frame = 0;
                        work->actionCommands.outputParams[0] = 1;
                    }
                    break;
                case 1:
                    if (cw->hold) {
                        if (cw->button == BattlePadGetNow() && BattlePadCheckTrigger(cw->button)) {
                            cw->phase = 2;
                        }
                    } else {
                        if (!BattlePadCheckNow(cw->button) && BattlePadCheckUp(cw->button)) {
                            cw->phase = 2;
                        }
                    }
                    break;
            }
            if (work->actionCommands.mAcState == 1000) {
                if (cw->frame > 120) {
                    work->actionCommands.result = 0;
                    cw->gauge = 0.0f;
                    work->actionCommands.mAcState = 1003;
                    work->actionCommands.outputParams[0] = 2;
                    break;
                }
            } else {
                if (cw->frame > cw->maxFrames) {
                    work->actionCommands.result = 0;
                    work->actionCommands.mAcState = 1003;
                    break;
                }
            }
            if (work->actionCommands.mAcState == 1000) {
                break;
            }
            if (cw->timer <= 0) {
                cw->cnt++;
            }
            switch (work->actionCommands.params[GAUGE_PARAM_MODE]) {
            case 1: {
                f32 angle = reviseAngle((f32)(cw->cnt * 360 / work->actionCommands.params[GAUGE_PARAM_PERIOD]));
                if (angle <= 90.0f) {
                    cw->gauge = intplGetValue(EASING_CUBIC_OUT, 0.0f, 100.0f, (s32)angle, 90);
                } else if (angle <= 180.0f) {
                    cw->gauge = intplGetValue(EASING_CUBIC_IN, 100.0f, 0.0f, (s32)(angle - 90.0f), 90);
                } else if (angle <= 270.0f) {
                    cw->gauge = intplGetValue(EASING_CUBIC_OUT, 0.0f, 100.0f, (s32)(angle - 180.0f), 90);
                } else {
                    cw->gauge = intplGetValue(EASING_CUBIC_IN, 100.0f, 0.0f, (s32)(angle - 270.0f), 90);
                }
                if (cw->timer > 0) {
                    cw->timer--;
                    if (cw->timer > 0) {
                        cw->gauge = 0.0f;
                        cw->inRange = 0;
                        goto done;
                    }
                } else if ((work->actionCommands.acFlags & AC_GAUGE_TIMING_FLAG_CYCLE_PAUSE) || (work->actionCommands.acFlags & AC_GAUGE_TIMING_FLAG_CYCLE_PAUSE)) {
                    // BUG: the same bit is tested twice, so the 30-frame else branch below can never run. Same at the
                    // wrap test below.
                    if ((s32)cw->gauge <= 0) {
                        cw->hits++;
                        cw->gauge = 0.0f;
                        cw->inRange = 0;
                        if (work->actionCommands.acFlags & AC_GAUGE_TIMING_FLAG_CYCLE_PAUSE) {
                            cw->timer = 15;
                        } else {
                            cw->timer = 30;
                        }
                        goto done;
                    }
                }
                if (cw->gauge >= (f32)work->actionCommands.params[GAUGE_PARAM_TARGET]) {
                    cw->inRange = 1;
                } else {
                    cw->inRange = 0;
                }
                break;
            }
            default: {
                if (cw->timer > 0) {
                    cw->timer--;
                    if (cw->timer > 0) {
                        cw->gauge = 0.0f;
                        cw->inRange = 0;
                        goto done;
                    }
                }
                cw->gauge = (f32)work->actionCommands.params[GAUGE_PARAM_PERIOD] / 100.0f * (f32)cw->cnt + 50.0f;
                if (work->actionCommands.acFlags & AC_GAUGE_TIMING_FLAG_WRAP) {
                    s32 g = (s32)cw->gauge;
                    if ((work->actionCommands.acFlags & AC_GAUGE_TIMING_FLAG_CYCLE_PAUSE) || (work->actionCommands.acFlags & AC_GAUGE_TIMING_FLAG_CYCLE_PAUSE)) {
                        s32 q = g / 100;
                        if (q > cw->hits) {
                            cw->hits = q;
                            cw->gauge = 0.0f;
                            cw->inRange = 0;
                            if (work->actionCommands.acFlags & AC_GAUGE_TIMING_FLAG_CYCLE_PAUSE) {
                                cw->timer = 15;
                            } else {
                                cw->timer = 30;
                            }
                            goto done;
                        }
                    }
                    g %= 100;
                    cw->gauge = (f32)g;
                }
                if (__fabs(cw->gauge - 50.0f) <= 12.0) {
                    cw->inRange = 1;
                } else {
                    cw->inRange = 0;
                }
                break;
            }
            }
        done:
            if (autoCommand && cw->inRange) {
                cw->phase = 2;
            }
            if (cw->phase == 2) {
                if (cw->inRange) {
                    work->actionCommands.result = AC_RESULT_SUCCESS;
                    work->actionCommands.mResultCount++;
                } else {
                    work->actionCommands.result = 0;
                    work->actionCommands.result &= ~AC_RESULT_SUCCESS;
                }
                work->actionCommands.mAcState = 1003;
            }
            break;
        case 1002:
            work->actionCommands.setupParam--;
            if (work->actionCommands.setupParam < 0) {
                if (cw->passed) {
                    work->actionCommands.result = AC_RESULT_SUCCESS;
                    work->actionCommands.mResultCount++;
                } else {
                    work->actionCommands.result = 0;
                }
                work->actionCommands.doneFlags |= 1;
                return 0;
            }
            break;
        case 1003:
            work->actionCommands.mAcState = 1004;
        case 1004:
            work->actionCommands.doneFlags |= 1;
            cw->endTimer = 60;
            work->actionCommands.mAcState = 1005;
            if (cw->sfxId != -1) {
                psndSFXOff(cw->sfxId);
            }
        case 1005:
            cw->endTimer--;
            if (cw->endTimer <= 0) {
                work->actionCommands.mAcState = 1006;
            }
            break;
        case 1006:
            work->actionCommands.maincb = NULL;
            work->actionCommands.dispcb = NULL;
            work->actionCommands.resultcb = NULL;
            work->actionCommands.deletecb = NULL;
            return 0;
    }
    return 1;
}

s32 battleAcResult_GaugeTiming(BattleWork* work) {
    return work->actionCommands.result;
}

void battleAcDisp_GaugeTiming(CameraId cameraId, void* param) {
    BattleActions* ac = &((BattleWork*)param)->actionCommands;
    BattleACDispParams* disp = &ac->disp;

    switch (ac->mAcState) {
        case 99:
            disp->iconX = intplGetValue(EASING_QUADRATIC_OUT, -300.0f, 30.0f, 20 - disp->slideTimer, 20);
        case 100:
        case 1000:
        case 1001:
        case 1003:
            actionCommandDisp(disp->iconX, disp->iconY);
            if (disp->slideTimer > 0) {
                disp->slideTimer--;
            }
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

void battleAcDelete_GaugeTiming(BattleWork* work) {
    if (!(work->actionCommands.doneFlags & 1)) {
        work->actionCommands.mAcState = 1004;
    }
}

static void actionCommandDisp(f32 x, f32 y) {
    BattleWork* work = _battleWorkPointer;
    BattleActions* ac = &work->actionCommands;
    BattleACDispParams* disp = &ac->disp;
    GaugeWork* cw;
    f32 rx;
    f32 ry;
    f32 bx;
    f32 by;
    f32 t;
    f32 ratio;
    s32 i;
    s32 n;
    u16 pressed;
    u16 unpressed;
    u16 pressed2;
    u16 unpressed2;

    camGetPtr(CAMERA_2D);
    cw = (GaugeWork*)ac->work;
    ratio = cw->gauge / 100.0f;
    if (ratio > 1.0f) {
        ratio = 1.0f;
    }
    disp->gaugeRatio = ratio;
    switch (ac->params[GAUGE_PARAM_MODE]) {
    case 1: {
        u16 icon;
        t = 100.0f * disp->gaugeRatio / (f32)ac->params[GAUGE_PARAM_TARGET];
        if (t > 100.0f) {
            t = 100.0f;
        }
        if (cw->timer > 0) {
            BattleAcDrawGauge((s32)x, (s32)y, 0xB2, -1.0f, 100, 100, 100, 100, 0);
        } else {
            BattleAcDrawGauge((s32)x, (s32)y, 0xB2, t, 100, 100, 100, 100, 0);
        }
        if (cw->gauge >= (f32)ac->params[GAUGE_PARAM_TARGET]) {
            icon = ICON_AC_ON;
        } else {
            icon = ICON_AC_OFF;
        }
        {
            Vec pos = {0.0f, 0.0f, 0.0f};
            pos.x = 16.0f + (176.0f + (-288.0f + x));
            pos.y = 18.0f + y;
            iconDispGx(&pos, 0x10, icon, 1.0f);
        }
        break;
    }
    default: {
        BattleAcDrawGauge((s32)x, (s32)y, 0xB2, 0.0f, 100, 100, 100, 100, 0);
        if (cw->timer <= 0) {
            CameraEntry* cam = camGetPtr(CAMERA_2D);
            rx = (-200.0f + x - 88.0f) + 173.0f * cw->gauge / 100.0f;
            ry = 49.0f + y;
            btlDispGXInit2DRasta();
            GXLoadPosMtxImm(cam->view, GX_PNMTX0);
            btlDispGXQuads2DRasta(rx, ry, rx + 10.0f, ry - 16.0f, 0, 255, 0, 255);
        }
        n = ac->params[GAUGE_PARAM_TARGET];
        bx = -288.0f + x;
        by = 20.0f + y;
        for (i = 0; i < n; i++) {
            u16 icon = ICON_AC_OFF;
            f32 step;
            step = 100.0f / (f32)(n + 1) * (f32)(i + 1);
            if (cw->inRange) {
                icon = ICON_AC_ON;
            }
            {
                Vec pos = {0.0f, 0.0f, 0.0f};
                pos.x = bx + 176.0f * step / 100.0f;
                pos.y = by;
                iconDispGx(&pos, 0x10, icon, 0.8f);
            }
        }
        break;
    }
    }

    switch (ac->params[GAUGE_PARAM_TYPE]) {
        case 0:
            pressed = BattleACGetButtonIcon(PAD_TRIGGER_R, 1);
            unpressed = BattleACGetButtonIcon(PAD_TRIGGER_R, 0);
            pressed2 = 0;
            unpressed2 = 0;
            break;
        case 2:
            pressed = BattleACGetButtonIcon(BATTLE_PAD_STICK_LEFT, 1);
            unpressed = BattleACGetButtonIcon(BATTLE_PAD_STICK_LEFT, 0);
            pressed2 = 0;
            unpressed2 = 0;
            break;
        case 1:
        default:
            pressed = BattleACGetButtonIcon(PAD_BUTTON_A, 1);
            unpressed = BattleACGetButtonIcon(PAD_BUTTON_A, 0);
            pressed2 = 0;
            unpressed2 = 0;
            break;
    }

    switch (ac->mAcState) {
        case 99:
        case 100:
        case 1003:
        case 1004:
        case 1005:
            if (pressed2) {
                Vec pos = {0.0f, 0.0f, 0.0f};
                f32 rx, ry;
                rx = -228.0f + x;
                ry = 55.0f + y;
                pos.x = rx;
                pos.y = ry;
                iconDispGx(&pos, 0x10, unpressed, 1.0f);
                {
                Vec pos2 = {0.0f, 0.0f, 0.0f};
                pos2.x = 56.0f + rx;
                pos2.y = ry;
                iconDispGx(&pos2, 0x10, unpressed2, 1.0f);
                }
            } else {
                Vec pos = {0.0f, 0.0f, 0.0f};
                pos.x = 28.0f + (-228.0f + x);
                pos.y = 55.0f + y;
                iconDispGx(&pos, 0x10, unpressed, 1.0f);
            }
            break;
        case 1000:
        case 1001:
            if (cw->hold) {
                if (cw->inRange) {
                    if (pressed2) {
                        Vec pos = {0.0f, 0.0f, 0.0f};
                        f32 rx, ry;
                        rx = -228.0f + x;
                        ry = 55.0f + y;
                        pos.x = rx;
                        pos.y = ry;
                        iconDispGx(&pos, 0x10, pressed, 1.0f);
                        {
                        Vec pos2 = {0.0f, 0.0f, 0.0f};
                        pos2.x = 56.0f + rx;
                        pos2.y = ry;
                        iconDispGx(&pos2, 0x10, pressed2, 1.0f);
                        }
                    } else {
                        Vec pos = {0.0f, 0.0f, 0.0f};
                        pos.x = 28.0f + (-228.0f + x);
                        pos.y = 55.0f + y;
                        iconDispGx(&pos, 0x10, pressed, 1.0f);
                    }
                } else {
                    if (pressed2) {
                        Vec pos = {0.0f, 0.0f, 0.0f};
                        f32 rx, ry;
                        rx = -228.0f + x;
                        ry = 55.0f + y;
                        pos.x = rx;
                        pos.y = ry;
                        iconDispGx(&pos, 0x10, unpressed, 1.0f);
                        {
                        Vec pos2 = {0.0f, 0.0f, 0.0f};
                        pos2.x = 56.0f + rx;
                        pos2.y = ry;
                        iconDispGx(&pos2, 0x10, unpressed2, 1.0f);
                        }
                    } else {
                        Vec pos = {0.0f, 0.0f, 0.0f};
                        pos.x = 28.0f + (-228.0f + x);
                        pos.y = 55.0f + y;
                        iconDispGx(&pos, 0x10, unpressed, 1.0f);
                    }
                }
            } else {
                if (cw->inRange) {
                    if (pressed2) {
                        Vec pos = {0.0f, 0.0f, 0.0f};
                        f32 rx, ry;
                        rx = -228.0f + x;
                        ry = 55.0f + y;
                        pos.x = rx;
                        pos.y = ry;
                        iconDispGx(&pos, 0x10, unpressed, 1.0f);
                        {
                        Vec pos2 = {0.0f, 0.0f, 0.0f};
                        pos2.x = 56.0f + rx;
                        pos2.y = ry;
                        iconDispGx(&pos2, 0x10, unpressed2, 1.0f);
                        }
                    } else {
                        Vec pos = {0.0f, 0.0f, 0.0f};
                        pos.x = 28.0f + (-228.0f + x);
                        pos.y = 55.0f + y;
                        iconDispGx(&pos, 0x10, unpressed, 1.0f);
                    }
                } else {
                    if (pressed2) {
                        Vec pos = {0.0f, 0.0f, 0.0f};
                        f32 rx, ry;
                        rx = -228.0f + x;
                        ry = 55.0f + y;
                        pos.x = rx;
                        pos.y = ry;
                        iconDispGx(&pos, 0x10, pressed, 1.0f);
                        {
                        Vec pos2 = {0.0f, 0.0f, 0.0f};
                        pos2.x = 56.0f + rx;
                        pos2.y = ry;
                        iconDispGx(&pos2, 0x10, pressed2, 1.0f);
                        }
                    } else {
                        Vec pos = {0.0f, 0.0f, 0.0f};
                        pos.x = 28.0f + (-228.0f + x);
                        pos.y = 55.0f + y;
                        iconDispGx(&pos, 0x10, pressed, 1.0f);
                    }
                }
            }
            break;
    }
}
