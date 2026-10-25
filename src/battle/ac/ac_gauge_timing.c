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

#pragma optimize_for_size on

void btlDispGXInit2DRasta(void);
void btlDispGXQuads2DRasta(f32 x0, f32 y0, f32 x1, f32 y1, u8 r, u8 g, u8 b, u8 a);
u16 BattleACGetButtonIcon(s32 button, s32 pressed);
void BattleAcDrawGauge(s32 x, s32 y, s32 icon, f32 ratio, s32 a, s32 b, s32 c, s32 d, s32 e);

static void actionCommandDisp(f32 x, f32 y);

typedef struct GaugeAC {
    struct BattleUnit* unit; // 0x0
    s32 field_0x4;           // 0x4
    s32 field_0x8;           // 0x8
    s32 state;               // 0xC
    void* maincb;            // 0x10
    void* resultcb;          // 0x14
    void* dispcb;            // 0x18
    void* deletecb;          // 0x1C
    s32 field_0x20;          // 0x20
    s32 resultCount;         // 0x24
    s32 result;              // 0x28
    u8 field_0x2C[4];        // 0x2C
    u32 flags;               // 0x30
    u32 opts;                // 0x34
    s32 type;                // 0x38
    s32 maxFrames;           // 0x3C
    s32 mode;                // 0x40
    s32 target;              // 0x44
    s32 period;              // 0x48
    u8 field_0x4C[0x58 - 0x4C];
    u32 out0; // 0x58
} GaugeAC;

typedef struct GaugeWork {
    s32 frame;     // 0x0
    f32 gauge;     // 0x4
    s32 cnt;       // 0x8
    s32 maxFrames; // 0xC
    s32 inRange;   // 0x10
    s8 passed;     // 0x14
    s8 phase;      // 0x15
    s8 hold;       // 0x16
    u8 field_0x17;
    u32 button;   // 0x18
    s32 timer;    // 0x1C
    s32 hits;     // 0x20
    u32 sfxId;    // 0x24
    s32 endTimer; // 0x28
} GaugeWork;

#define GAC(work) ((GaugeAC*)&(work)->actionCommands)

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
            GAC(work)->out0 = 0;
            work->actionCommands.mAcState = 99;
            work->actionCommands.result = 1;
            cw->maxFrames = GAC(work)->maxFrames;
            cw->inRange = 0;
            cw->timer = 0;
            cw->hits = 0;
            cw->sfxId = -1;
            switch (GAC(work)->type) {
                case 0:
                    cw->hold = 1;
                    cw->button = 0x20;
                    cw->phase = 1;
                    GAC(work)->out0 = 1;
                    break;
                case 2:
                    cw->hold = 0;
                    cw->button = 0x40000;
                    cw->phase = 0;
                    break;
                case 1:
                default:
                    cw->hold = 1;
                    cw->button = 0x100;
                    cw->phase = 1;
                    GAC(work)->out0 = 1;
                    break;
            }
            switch (GAC(work)->mode) {
                case 1:
                    cw->gauge = 0.0f;
                    GAC(work)->period &= ~1;
                    break;
                default:
                    cw->gauge = 50.0f;
                    break;
            }
            break;
        case 99:
            break;
        case 100:
            if ((GAC(work)->field_0x4 & 1) && ((u32)work->actionCommands.mAcUnit->mTokenFlags & 0x10)) {
                if (irand(100) >= 0) {
                    GAC(work)->field_0x8 = 30;
                    cw->passed = 0;
                } else {
                    cw->passed = 1;
                    GAC(work)->field_0x8 = 30;
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
                        GAC(work)->out0 = 1;
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
                    GAC(work)->out0 = 2;
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
            switch (GAC(work)->mode) {
            case 1: {
                f32 angle = reviseAngle((f32)(cw->cnt * 360 / GAC(work)->period));
                if (angle <= 90.0f) {
                    cw->gauge = intplGetValue(5, (s32)angle, 90, 0.0f, 100.0f);
                } else if (angle <= 180.0f) {
                    cw->gauge = intplGetValue(2, (s32)(angle - 90.0f), 90, 100.0f, 0.0f);
                } else if (angle <= 270.0f) {
                    cw->gauge = intplGetValue(5, (s32)(angle - 180.0f), 90, 0.0f, 100.0f);
                } else {
                    cw->gauge = intplGetValue(2, (s32)(angle - 270.0f), 90, 100.0f, 0.0f);
                }
                if (cw->timer > 0) {
                    cw->timer--;
                    if (cw->timer > 0) {
                        cw->gauge = 0.0f;
                        cw->inRange = 0;
                        goto done;
                    }
                } else if ((GAC(work)->opts & 2) || (GAC(work)->opts & 2)) {
                    if ((s32)cw->gauge <= 0) {
                        cw->hits++;
                        cw->gauge = 0.0f;
                        cw->inRange = 0;
                        if (GAC(work)->opts & 2) {
                            cw->timer = 15;
                        } else {
                            cw->timer = 30;
                        }
                        goto done;
                    }
                }
                if (cw->gauge >= (f32)GAC(work)->target) {
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
                cw->gauge = (f32)GAC(work)->period / 100.0f * (f32)cw->cnt + 50.0f;
                if (GAC(work)->opts & 1) {
                    s32 g = (s32)cw->gauge;
                    if ((GAC(work)->opts & 2) || (GAC(work)->opts & 2)) {
                        s32 q = g / 100;
                        if (q > cw->hits) {
                            cw->hits = q;
                            cw->gauge = 0.0f;
                            cw->inRange = 0;
                            if (GAC(work)->opts & 2) {
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
                    work->actionCommands.result = 2;
                    work->actionCommands.mResultCount++;
                } else {
                    work->actionCommands.result = 0;
                    work->actionCommands.result &= ~2;
                }
                work->actionCommands.mAcState = 1003;
            }
            break;
        case 1002:
            GAC(work)->field_0x8--;
            if (GAC(work)->field_0x8 < 0) {
                if (cw->passed) {
                    work->actionCommands.result = 2;
                    work->actionCommands.mResultCount++;
                } else {
                    work->actionCommands.result = 0;
                }
                GAC(work)->flags |= 1;
                return 0;
            }
            break;
        case 1003:
            work->actionCommands.mAcState = 1004;
        case 1004:
            GAC(work)->flags |= 1;
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
            disp->iconX = intplGetValue(4, 20 - disp->slideTimer, 20, -300.0f, 30.0f);
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
                disp->iconX = intplGetValue(4, disp->slideTimer - 40, 20, 30.0f, -300.0f);
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
    if (!(GAC(work)->flags & 1)) {
        work->actionCommands.mAcState = 1004;
    }
}

static void actionCommandDisp(f32 x, f32 y) {
    BattleWork* work = _battleWorkPointer;
    BattleActions* ac = &work->actionCommands;
    BattleACDispParams* disp = &ac->disp;
    GaugeAC* g;
    GaugeWork* cw;
    f32 rx;
    f32 ry;
    f32 bx;
    f32 by;
    f32 step;
    f32 t;
    f32 ratio;
    s32 i;
    s32 n;
    u16 pressed;
    u16 unpressed;
    u16 pressed2;
    u16 unpressed2;

    camGetPtr(CAMERA_2D);
    g = (GaugeAC*)ac;
    cw = (GaugeWork*)ac->work;
    ratio = cw->gauge / 100.0f;
    if (ratio > 1.0f) {
        ratio = 1.0f;
    }
    disp->gaugeRatio = ratio;
    switch (g->mode) {
    case 1: {
        u16 icon;
        t = 100.0f * disp->gaugeRatio / (f32)g->target;
        if (t > 100.0f) {
            t = 100.0f;
        }
        if (cw->timer > 0) {
            BattleAcDrawGauge((s32)x, (s32)y, 0xB2, -1.0f, 100, 100, 100, 100, 0);
        } else {
            BattleAcDrawGauge((s32)x, (s32)y, 0xB2, t, 100, 100, 100, 100, 0);
        }
        if (cw->gauge >= (f32)g->target) {
            icon = ICON_095;
        } else {
            icon = ICON_091;
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
            GXLoadPosMtxImm(cam->view, 0);
            btlDispGXQuads2DRasta(rx, ry, rx + 10.0f, ry - 16.0f, 0, 255, 0, 255);
        }
        n = g->target;
        bx = -288.0f + x;
        by = 20.0f + y;
        for (i = 0; i < n; i++) {
            u16 icon = ICON_091;
            f32 step;
            step = 100.0f / (f32)(n + 1) * (f32)(i + 1);
            if (cw->inRange) {
                icon = ICON_095;
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

    switch (g->type) {
        case 0:
            pressed = BattleACGetButtonIcon(0x20, 1);
            unpressed = BattleACGetButtonIcon(0x20, 0);
            pressed2 = 0;
            unpressed2 = 0;
            break;
        case 2:
            pressed = BattleACGetButtonIcon(0x40000, 1);
            unpressed = BattleACGetButtonIcon(0x40000, 0);
            pressed2 = 0;
            unpressed2 = 0;
            break;
        case 1:
        default:
            pressed = BattleACGetButtonIcon(0x100, 1);
            unpressed = BattleACGetButtonIcon(0x100, 0);
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
