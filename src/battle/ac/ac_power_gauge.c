#include "icon_ids.h"
#include "battle/battle.h"
#include "battle/battle_ac.h"
#include "battle/battle_pad.h"
#include "battle/battle_unit.h"
#include "driver/camdrv.h"
#include "driver/icondrv.h"
#include "system.h"
#include <string.h>

void BattleAcDrawGauge(s32 x, s32 y, s32 width, f32 ratio, s32 level, s32 a, s32 b, s32 c, s32 flags);
s32 BattleACGetButtonIcon(s32 button, s32 pressed);

typedef struct PowerGaugeWork {
    s32 frame;     // 0x0
    f32 gauge;     // 0x4
    s32 frame2;    // 0x8
    s32 maxFrames; // 0xC
    s8 autoFail;   // 0x10
    s8 phase;      // 0x11
    s8 mode;       // 0x12
    u8 pad13;      // 0x13
    u32 button;    // 0x14
    s32 endTimer;  // 0x18
} PowerGaugeWork;


// iconDispGx called through a cast: its callers pass different argument types and no single prototype fits
// them all (docs/CLEANUP_BACKLOG.md, function-pointer casts).
#define ICONGX ((void (*)(Vec*, s32, s32, f32))iconDispGx)
static void actionCommandDisp(f32 x, f32 y);

s32 battleAcMain_PowerGauge(BattleWork* work) {
    BattleActions* ac = &work->actionCommands;
    PowerGaugeWork* pw = (PowerGaugeWork*)ac->work;
    BOOL isAuto = FALSE;
    BattleACDispParams* disp = &ac->disp;

    if (ac->mAcUnit->badgesEquipped.AutoCommand) {
        isAuto = TRUE;
    }

    switch (ac->mAcState) {
        case 0:
            memset(disp, 0, 0x2C);
            disp->iconX = -300.0f;
            disp->iconY = 0.0f;
            disp->slideTimer = 20;
            disp->gaugeRatio = 0.0f;
            disp->color = (GXColor){255, 255, 255, 255};
            pw->frame = 0;
            pw->frame2 = 0;
            ac->outputParams[0] = 0;
            ac->mAcState = 99;
            ac->result = 1;
            pw->maxFrames = ac->params[1] + ac->params[2];
            pw->gauge = 0.0f;
            switch (ac->params[0]) {
                case 0:
                    pw->mode = 1;
                    pw->button = 0x60;
                    pw->phase = 1;
                    ac->outputParams[0] = 1;
                    break;
                case 1:
                    pw->mode = 1;
                    pw->button = 0x100;
                    pw->phase = 1;
                    ac->outputParams[0] = 1;
                    break;
                case 2:
                    pw->mode = 0;
                    pw->button = 0x100;
                    pw->phase = 0;
                    break;
                case 3:
                    pw->mode = 0;
                    pw->button = 0x20;
                    pw->phase = 0;
                    break;
                default:
                    pw->mode = 1;
                    pw->button = 0x100;
                    pw->phase = 1;
                    ac->outputParams[0] = 1;
                    break;
            }
            break;
        case 99:
            break;
        case 100:
            if ((ac->setupFlags & 1) && (ac->mAcUnit->mTokenFlags & kConfusedAction)) {
                if (irand(100) >= 0) {
                    ac->setupParam = 30;
                    pw->autoFail = 0;
                } else {
                    pw->autoFail = 1;
                    ac->setupParam = 30;
                }
                ac->outputParams[0] = 1;
                ac->mAcState = 1002;
                break;
            }
            if (pw->mode) {
                ac->mAcState = 1001;
            } else {
                ac->mAcState = 1000;
            }
            if (pw->mode) {
                BattleAcGaugeSeInit();
            }
        case 1000:
        case 1001:
            pw->frame++;
            switch (pw->phase) {
                case 0:
                    if (!pw->mode) {
                        if (pw->button == BattlePadGetNow() || isAuto) {
                            pw->phase = 1;
                            ac->mAcState = 1001;
                            pw->frame = 0;
                            ac->outputParams[0] = 1;
                            BattleAcGaugeSeInit();
                        }
                    }
                    break;
                case 1:
                    if (pw->mode) {
                        if (pw->button == BattlePadGetNow()) {
                            if (BattlePadCheckTrigger(pw->button)) {
                                pw->phase = 2;
                            }
                        }
                    } else {
                        if (!BattlePadCheckNow(pw->button)) {
                            if (BattlePadCheckUp(pw->button)) {
                                pw->phase = 2;
                            }
                        }
                    }
                    break;
            }
            if (!(ac->acFlags & AC_POWER_GAUGE_FLAG_FILL_WHILE_HELD)) {
                pw->frame2++;
            } else if (pw->phase == 1) {
                pw->frame2++;
            }
            if (ac->mAcState == 1000) {
                if (pw->frame > 120) {
                    pw->frame = 0;
                    ac->result = 0;
                    pw->gauge = 0.0f;
                    ac->mAcState = 1003;
                    ac->outputParams[0] = 2;
                    break;
                }
            } else if (pw->frame > pw->maxFrames) {
                pw->frame = 0;
                ac->result = 0;
                pw->gauge = 101.0f;
                ac->mAcState = 1003;
                break;
            }
            if (pw->frame <= ac->params[1]) {
                pw->gauge = intplGetValue(ac->params[3], 0.0f, 100.0f, pw->frame2, ac->params[1]);
#ifdef VERSION_G8ME01
                if (pw->gauge > 100.0f) {
                    pw->gauge = 100.0f;
                }
#endif
            } else {
                pw->gauge = 100.0f;
            }
            BattleAcGaugeSeUpdate(pw->gauge);
            if (isAuto) {
                if (pw->gauge >= (f32)ac->params[4]) {
                    pw->phase = 2;
                } else {
                    pw->phase = 1;
                }
            }
            if (pw->phase == 2) {
                if (pw->gauge < (f32)ac->params[4]) {
                    ac->result = 0;
                    ac->result &= ~2;
                } else {
                    ac->result = 2;
                    ac->mResultCount++;
                }
                ac->mAcState = 1003;
            }
            break;
        case 1002:
            ac->setupParam--;
            if (ac->setupParam < 0) {
                if (pw->autoFail) {
                    ac->result = 2;
                    ac->mResultCount++;
                } else {
                    ac->result = 0;
                }
                ac->doneFlags |= 1;
                pw->endTimer = 60;
                ac->mAcState = 1005;
                return 0;
            }
            break;
        case 1003:
            ac->mAcState = 1004;
        case 1004:
            ac->doneFlags |= 1;
            BattleAcGaugeSeDelete();
            pw->endTimer = 60;
            ac->mAcState = 1005;
        case 1005:
            pw->endTimer--;
            if (pw->endTimer <= 0) {
                ac->mAcState = 1006;
            }
            break;
        case 1006:
            ac->maincb = NULL;
            ac->dispcb = NULL;
            ac->resultcb = NULL;
            ac->deletecb = NULL;
            return 0;
    }
    return 1;
}

s32 battleAcResult_PowerGauge(BattleWork* work) {
    return work->actionCommands.result;
}


void battleAcDisp_PowerGauge(CameraId cameraId, BattleWork* work) {
    BattleActions* ac = &work->actionCommands;
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

void battleAcDelete_PowerGauge(BattleWork* work) {
    if (!(work->actionCommands.doneFlags & 1)) {
        work->actionCommands.mAcState = 1004;
    }
}

static void actionCommandDisp(f32 x, f32 y) {
    BattleWork* work = _battleWorkPointer;
    BattleActions* ac = &work->actionCommands;
    PowerGaugeWork* pw;
    s32 a, b, c, d;
    BattleACDispParams* disp = &ac->disp;
    f32 ratio;
    u32 flags;

    camGetPtr(CAMERA_2D);
    pw = (PowerGaugeWork*)ac->work;
    ratio = pw->gauge / 100.0f;
    if (ratio > 1.0f) {
        ratio = 1.0f;
    }
    disp->gaugeRatio = ratio;

    if (ac->acFlags & AC_POWER_GAUGE_FLAG_PLAIN_GAUGE) {
        BattleAcDrawGauge((s32)x, (s32)y, 0xB2, ratio, 1, 100, 100, 100, 0);
    } else {
        BattleAcDrawGauge((s32)x, (s32)y, 0xB2, ratio, 2, ac->params[4], 100, 100, 0);
    }

    flags = ac->acFlags;
    if (!(flags & AC_POWER_GAUGE_FLAG_SHOW_ZONE_LAMP)) {
        Vec pos = {0.0f, 0.0f, 0.0f};
        pos.x = -288.0f + x + (f32)(ac->params[4] * 0xB0) / 100.0f;
        pos.y = 45.0f + y;
        ICONGX(&pos, 0x10, ICON_AC_GAUGE_MARKER, 1.0f);
    } else {
        s32 icon;
        if (pw->gauge >= (f32)ac->params[4] && pw->gauge <= 100.0f) {
            icon = ICON_AC_ON;
        } else {
            icon = ICON_AC_OFF;
        }
        if (flags & AC_POWER_GAUGE_FLAG_LAMP_SHIFT_LEFT) {
            Vec pos = {0.0f, 0.0f, 0.0f};
            pos.x = 120.0f + (-200.0f + x) - 12.0f;
            pos.y = 18.0f + y;
            ICONGX(&pos, 0x10, icon, 1.0f);
        } else {
            Vec pos = {0.0f, 0.0f, 0.0f};
            pos.x = 120.0f + (-200.0f + x);
            pos.y = 18.0f + y;
            ICONGX(&pos, 0x10, icon, 1.0f);
        }
    }

    switch (ac->params[0]) {
        case 0:
            a = ICON_TRIGGER_L_PRESSED;
            b = ICON_TRIGGER_L;
            c = ICON_TRIGGER_R_PRESSED;
            d = ICON_TRIGGER_R;
            break;
        case 1:
            a = BattleACGetButtonIcon(0x100, 1);
            b = BattleACGetButtonIcon(0x100, 0);
            c = 0;
            d = 0;
            break;
        case 2:
            a = BattleACGetButtonIcon(0x100, 1);
            b = BattleACGetButtonIcon(0x100, 0);
            c = 0;
            d = 0;
            break;
        case 3:
            a = BattleACGetButtonIcon(0x20, 1);
            b = BattleACGetButtonIcon(0x20, 0);
            c = 0;
            d = 0;
            break;
    }

    switch (ac->mAcState) {
        case 99:
        case 100:
        case 1003:
        case 1004:
        case 1005:
            if ((u16)c) {
                Vec pos = {0.0f, 0.0f, 0.0f};
                f32 px = -225.0f + x;
                f32 py = 80.0f + y;
                pos.x = px;
                pos.y = py;
                ICONGX(&pos, 0x10, b, 1.0f);
                {
                    Vec pos2 = {0.0f, 0.0f, 0.0f};
                    pos2.x = 56.0f + px;
                    pos2.y = py;
                    ICONGX(&pos2, 0x10, d, 1.0f);
                }
            } else {
                Vec pos = {0.0f, 0.0f, 0.0f};
                pos.x = 28.0f + (-225.0f + x);
                pos.y = 80.0f + y;
                ICONGX(&pos, 0x10, b, 1.0f);
            }
            break;
        case 1000:
        case 1001:
            if (pw->mode) {
                if (pw->gauge >= (f32)ac->params[4] && pw->gauge <= 100.0f) {
                    if ((u16)c) {
                        Vec pos = {0.0f, 0.0f, 0.0f};
                        f32 px = -225.0f + x;
                        f32 py = 80.0f + y;
                        pos.x = px;
                        pos.y = py;
                        ICONGX(&pos, 0x10, a, 1.0f);
                        {
                            Vec pos2 = {0.0f, 0.0f, 0.0f};
                            pos2.x = 56.0f + px;
                            pos2.y = py;
                            ICONGX(&pos2, 0x10, c, 1.0f);
                        }
                    } else {
                        Vec pos = {0.0f, 0.0f, 0.0f};
                        pos.x = 28.0f + (-225.0f + x);
                        pos.y = 80.0f + y;
                        ICONGX(&pos, 0x10, a, 1.0f);
                    }
                } else {
                    if ((u16)c) {
                        Vec pos = {0.0f, 0.0f, 0.0f};
                        f32 px = -225.0f + x;
                        f32 py = 80.0f + y;
                        pos.x = px;
                        pos.y = py;
                        ICONGX(&pos, 0x10, b, 1.0f);
                        {
                            Vec pos2 = {0.0f, 0.0f, 0.0f};
                            pos2.x = 56.0f + px;
                            pos2.y = py;
                            ICONGX(&pos2, 0x10, d, 1.0f);
                        }
                    } else {
                        Vec pos = {0.0f, 0.0f, 0.0f};
                        pos.x = 28.0f + (-225.0f + x);
                        pos.y = 80.0f + y;
                        ICONGX(&pos, 0x10, b, 1.0f);
                    }
                }
            } else {
                if ((pw->gauge >= (f32)ac->params[4] && pw->gauge <= 100.0f) || (s32)ac->outputParams[0] == 0) {
                    if ((u16)c) {
                        Vec pos = {0.0f, 0.0f, 0.0f};
                        f32 px = -225.0f + x;
                        f32 py = 80.0f + y;
                        pos.x = px;
                        pos.y = py;
                        ICONGX(&pos, 0x10, b, 1.0f);
                        {
                            Vec pos2 = {0.0f, 0.0f, 0.0f};
                            pos2.x = 56.0f + px;
                            pos2.y = py;
                            ICONGX(&pos2, 0x10, d, 1.0f);
                        }
                    } else {
                        Vec pos = {0.0f, 0.0f, 0.0f};
                        pos.x = 28.0f + (-225.0f + x);
                        pos.y = 80.0f + y;
                        ICONGX(&pos, 0x10, b, 1.0f);
                    }
                } else {
                    if ((u16)c) {
                        Vec pos = {0.0f, 0.0f, 0.0f};
                        f32 px = -225.0f + x;
                        f32 py = 80.0f + y;
                        pos.x = px;
                        pos.y = py;
                        ICONGX(&pos, 0x10, a, 1.0f);
                        {
                            Vec pos2 = {0.0f, 0.0f, 0.0f};
                            pos2.x = 56.0f + px;
                            pos2.y = py;
                            ICONGX(&pos2, 0x10, c, 1.0f);
                        }
                    } else {
                        Vec pos = {0.0f, 0.0f, 0.0f};
                        pos.x = 28.0f + (-225.0f + x);
                        pos.y = 80.0f + y;
                        ICONGX(&pos, 0x10, a, 1.0f);
                    }
                }
            }
            break;
    }
}
