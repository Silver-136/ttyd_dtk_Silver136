#include "icon_ids.h"
#include "battle/battle.h"
#include "battle/battle_ac.h"
#include "battle/battle_pad.h"
#include "battle/battle_unit.h"
#include "driver/camdrv.h"
#include "driver/icondrv.h"
#include "system.h"
#include <string.h>

s32 BattleACGetButtonIcon(s32 button, s32 pressed);
void BattleAcDrawGauge(s32 x, s32 y, s32 width, f32 ratio, s32 level, s32 a, s32 b, s32 c, s32 flags);

typedef struct PGLv2Work {
    s32 timer;     // 0x0
    s32 maxFrames; // 0x4
    f32 gauge;     // 0x8
    s32 button;    // 0xC
    f32 rate;      // 0x10
    f32 dec;       // 0x14
    s32 full;      // 0x18
    s32 inZone;    // 0x1C
    s16 zones[3];  // 0x20
    u8 pad26[2];   // 0x26
    s32 endTimer;  // 0x28
} PGLv2Work;

#define AC_DISP(ac) (&(ac)->disp)
#define AC_WORK(ac) ((PGLv2Work*)(ac)->work)

static s32 _power_table[] = {300, 200, 150, 100, 80, 60, 40};
s32 _power_table2[] = {100, 100, 100, 100, 110, 120, 130};

static void actionCommandDisp(f32 x, f32 y);

s32 battleAcMain_PowerGaugeLv2(BattleWork* work) {
    BattleActions* ac = &work->actionCommands;
    PGLv2Work* g = AC_WORK(ac);
    BattleACDispParams* disp = AC_DISP(ac);
    s32 diff = BattleActionCommandGetDifficulty(work);
    s32 flag = 0;
    s32 pressed;
    s32 i;
    if (ac->mAcUnit->badgesEquipped.AutoCommand) {
        flag = 1;
    }
    switch (ac->mAcState) {
        case 0:
            memset(disp, 0, 0x2C);
            disp->iconX = -300.0f;
            disp->iconY = 0.0f;
            disp->slideTimer = 20;
            disp->gaugeRatio = 0.0f;
            disp->color = (GXColor){255, 255, 255, 255};
            disp->colorStep[0] = 0;
            disp->colorStep[1] = -16;
            disp->colorStep[2] = -16;
            disp->colorStep[3] = 0;
            g->timer = 0;
            ac->outputParams[0] = 0;
            ac->mAcState = 99;
            ac->result = 1;
            g->maxFrames = sysMsec2Frame(7000);
            g->gauge = 0.0f;
            g->rate = 0.01f * ac->params[1];
            g->dec = 0.01f * ac->params[2];
            g->full = 0;
            g->inZone = 0;
            g->button = 0x100;
            sysMsec2Frame(1000);
            for (i = 0; i < 3; i++) {
                g->zones[i] = irand(sysMsec2Frame(250)) + (i * 2 + 1) * sysMsec2Frame(1000);
            }
            break;
        case 99:
            break;
        case 100:
            if ((ac->setupFlags & 1) && ((u32)ac->mAcUnit->mTokenFlags & kConfusedAction)) {
                if (irand(100) >= 0) {
                    ac->setupParam = 30;
                    ac->outputParams[0] = 0;
                } else {
                    ac->outputParams[0] = irand(ac->params[3] - 1) + 1;
                    ac->setupParam = 30;
                }
                ac->mAcState = 1001;
                break;
            }
            BattleAcGaugeSeInit();
            ac->mAcState = 1000;
        case 1000:
            g->timer++;
            if (g->timer > g->maxFrames) {
                if ((s32)ac->outputParams[0] < ac->params[4]) {
                    ac->result = 0;
                    ac->result &= ~2;
                } else {
                    ac->result = 2;
                    ac->mResultCount++;
                }
                ac->mAcState = 1002;
                break;
            }
            pressed = BattlePadCheckNow(g->button) != 0;
            {
                s32 w = sysMsec2Frame(750);
                g->inZone = 1;
                for (i = 0; i < 3; i++) {
                    s32 z = g->zones[i];
                    if (g->timer >= z && g->timer < z + w) {
                        g->inZone = 0;
                        break;
                    }
                }
            }
            if (flag) {
                if (g->inZone) {
                    pressed = TRUE;
                } else {
                    pressed = FALSE;
                }
            }
            if (!g->full) {
                if (pressed) {
                    if (g->inZone) {
                        g->gauge += 0.01f * (g->rate * (f32)_power_table[diff]);
                    } else {
                        g->gauge -= 0.01f * (g->rate * (f32)_power_table2[diff]);
                    }
                }
                if (g->gauge >= 100.0f) {
                    g->gauge = 100.0f;
                    g->full = 1;
                } else {
                    g->gauge -= g->dec;
                }
                if (g->gauge <= 0.0f) {
                    g->gauge = 0.0f;
                }
            }
            BattleAcGaugeSeUpdate(g->gauge);
            ac->outputParams[0] = (s32)(g->gauge * (f32)ac->params[3] / 100.0f);
            if ((s32)ac->outputParams[0] >= ac->params[3]) {
                ac->outputParams[0] = ac->params[3] - 1;
            }
            if (!((s32)ac->acFlags & AC_POWER_GAUGE_LV2_FLAG_NO_FULL_BONUS_UNUSED)) {
                if (g->gauge >= 100.0f) {
                    ac->outputParams[0]++;
                }
            }
            break;
        case 1001:
            ac->setupParam--;
            if (ac->setupParam < 0) {
                if ((s32)ac->outputParams[0] >= ac->params[4]) {
                    ac->result = 2;
                    ac->mResultCount++;
                } else {
                    ac->result = 0;
                }
                return 0;
            }
            break;
        case 1002:
            ac->mAcState = 1003;
        case 1003:
            ac->doneFlags |= 1;
            BattleAcGaugeSeDelete();
            g->endTimer = 60;
            ac->mAcState = 1004;
        case 1004:
            g->endTimer--;
            if (g->endTimer <= 0) {
                ac->mAcState = 1005;
            }
            break;
        case 1005:
            ac->maincb = NULL;
            ac->dispcb = NULL;
            ac->resultcb = NULL;
            ac->deletecb = NULL;
            return 0;
    }
    return 1;
}

s32 battleAcResult_PowerGaugeLv2(BattleWork* work) {
    return work->actionCommands.result;
}

void battleAcDisp_PowerGaugeLv2(CameraId cameraId, void* param) {
    BattleWork* work = (BattleWork*)param;
    BattleActions* ac = &work->actionCommands;
    BattleACDispParams* disp = AC_DISP(ac);
    switch (ac->mAcState) {
        case 99:
            disp->iconX = intplGetValue(EASING_QUADRATIC_OUT, -300.0f, 30.0f, 20 - disp->slideTimer, 20);
        case 100:
        case 1000:
        case 1002:
            actionCommandDisp(disp->iconX, disp->iconY);
            if (disp->slideTimer > 0) {
                disp->slideTimer--;
            }
            break;
        case 1003:
        case 1004:
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

void battleAcDelete_PowerGaugeLv2(BattleWork* work) {
    if (!(work->actionCommands.doneFlags & 1)) {
        work->actionCommands.mAcState = 1003;
    }
}

#define ICON ((void (*)(Vec*, s32, s32, f32))iconDispGx)
static void actionCommandDisp(f32 x, f32 y) {
    BattleWork* work = _battleWorkPointer;
    BattleActions* ac = &work->actionCommands;
    f32 ratio;
    s32 full;
    s32 tmp[4];
    s32 cnt;
    s32 i;
    s32 pressedIcon;
    s32 icon;
    f32 px;
    f32 py;
    camGetPtr(CAMERA_2D);
    px = -200.0f + x;
    py = 25.0f + y;
    {
        Vec pos = {0.0f, 0.0f, 0.0f};
        pos.x = px;
        pos.y = py;
        ICON(&pos, 0x10, ICON_AC_GAUGE_FRAME, 1.0f);
    }
    full = 0;
    ratio = AC_WORK(ac)->gauge / 100.0f;
    if (ratio > 1.0f) {
        ratio = 1.0f;
    }
    ac->disp.gaugeRatio = ratio;
    if (AC_WORK(ac)->full) {
        ac->disp.color.g += ac->disp.colorStep[1];
        ac->disp.color.b += ac->disp.colorStep[2];
        if (ac->disp.gaugeRatio >= 1.0f) {
            full |= 1;
        }
        if (ac->disp.color.g <= 0x80 && ac->disp.colorStep[1] < 0) {
            ac->disp.colorStep[1] = ac->disp.colorStep[2] = 0x10;
        }
        if (ac->disp.color.g >= 0xF5 && ac->disp.colorStep[1] > 0) {
            ac->disp.colorStep[1] = ac->disp.colorStep[2] = -0x10;
        }
    }
    cnt = 0;
    for (i = 0; i < 4; i++) {
        tmp[i] = 100;
    }
    for (i = 0; i < 4; i++, cnt++) {
        tmp[i] = ac->gaugeParams[i];
        if (i != 0 && !(tmp[i] - tmp[i - 1] > 0)) {
            break;
        }
    }
    BattleAcDrawGauge((s32)x, (s32)y, 0xB2, ac->disp.gaugeRatio, cnt, tmp[0], tmp[1], tmp[2], full);
    pressedIcon = BattleACGetButtonIcon(AC_WORK(ac)->button, 1);
    icon = BattleACGetButtonIcon(AC_WORK(ac)->button, 0);
    switch (ac->mAcState) {
        case 99:
        case 100:
        case 1002:
        case 1003:
        case 1004: {
            Vec pos = {0.0f, 0.0f, 0.0f};
            pos.x = px;
            pos.y = 70.0f + y;
            ICON(&pos, 0x10, icon, 1.0f);
            break;
        }
        case 1000:
            if (AC_WORK(ac)->inZone) {
                Vec pos = {0.0f, 0.0f, 0.0f};
                pos.x = px;
                pos.y = 70.0f + y;
                ICON(&pos, 0x10, pressedIcon, 1.0f);
            } else {
                Vec pos = {0.0f, 0.0f, 0.0f};
                pos.x = px;
                pos.y = 70.0f + y;
                ICON(&pos, 0x10, icon, 1.0f);
            }
            break;
    }
    {
        s32 id = ICON_AC_OFF;
        Vec pos;
        if (AC_WORK(ac)->inZone) {
            id = ICON_AC_ON;
        }
        {
            Vec zero = {0.0f, 0.0f, 0.0f};
            pos = zero;
        }
        pos.x = 120.0f + px;
        pos.y = py;
        ICON(&pos, 0x10, id, 1.0f);
    }
}

