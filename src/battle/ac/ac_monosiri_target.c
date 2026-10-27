#include "battle/battle.h"
#include "battle/battle_ac.h"
#include "battle/battle_pad.h"
#include "battle/battle_unit.h"
#include "driver/icondrv.h"
#include "pmario_sound.h"
#include "system.h"
#include <string.h>
#include "version.h"
#include "battle/battle_disp.h"


s32 BattleACGetButtonIcon(s32 button, s32 pressed);

typedef struct MonosiriTargetWork {
    s32 frame;      // 0x0
    s32 maxFrames;  // 0x4
    Vec hitPos;     // 0x8
    Vec pos;        // 0x14
    Vec targetPos;  // 0x20
    f32 angle;      // 0x2C
    s32 inRange;    // 0x30
    s32 inRangeCnt; // 0x34
    u32 sfxId;      // 0x38
    s32 endTimer;   // 0x3C
} MonosiriTargetWork;


static void actionCommandDisp(f32 x, f32 y);

s32 _frame_table[] = {240, 180, 120, 90, 70, 40, 20};

s32 _getmaxcount(void) {
    return _frame_table[BattleActionCommandGetDifficulty(_battleWorkPointer)];
}

s32 battleAcMain_MonosiriTarget(BattleWork* work) {
    BattleActions* ac;
    MonosiriTargetWork* cw;
    BattleACDispParams* disp;
    BattleUnit* unit;
    BattleUnitPart* parts;
    BOOL autoCommand;

    ac = &work->actionCommands;
    autoCommand = FALSE;
    cw = (MonosiriTargetWork*)ac->work;
    disp = &ac->disp;
    if (work->actionCommands.mAcUnit->badgesEquipped.AutoCommand) {
        autoCommand = TRUE;
    }
    unit = BattleGetUnitPtr(work, work->actionCommands.params[0]);
    parts = BtlUnit_GetPartsPtr(unit, work->actionCommands.params[1]);

    switch (work->actionCommands.mAcState) {
        case 0:
            memset(disp, 0, sizeof(BattleACDispParams));
            disp->iconX = -300.0f;
            disp->iconY = 0.0f;
            disp->slideTimer = 20;
            cw->frame = 0;
            cw->maxFrames = _getmaxcount();
            BtlUnit_GetHitPos(unit, parts, &cw->hitPos.x, &cw->hitPos.y, &cw->hitPos.z);
            cw->pos = cw->hitPos;
            cw->targetPos = cw->hitPos;
            cw->inRange = 0;
            cw->angle = 0.0f;
            work->actionCommands.mAcState = 99;
            work->actionCommands.result = AC_RESULT_RUNNING;
            cw->sfxId = -1;
            break;
        case 100:
            cw->frame = 0;
            cw->sfxId = psndSFXOn("SFX_AC_CURSOR_MOVE1");
            work->actionCommands.mAcState = 1000;
        case 1000:
            cw->frame++;
            cw->pos.x = (cw->hitPos.x - 80.0f) + intplGetValue(EASING_LINEAR, 0.0f, 160.0f, cw->frame, cw->maxFrames);
            if (__fabs(cw->pos.x - cw->targetPos.x) < 28.0) {
                cw->inRange = 1;
            } else {
                cw->inRange = 0;
            }
            if (cw->inRange == 0) {
                cw->inRangeCnt = 0;
            } else {
                cw->inRangeCnt++;
            }
            if (cw->frame >= cw->maxFrames) {
                work->actionCommands.result = 0;
                work->actionCommands.mAcState = 1002;
                break;
            }
            if (BattlePadCheckTrigger(PAD_BUTTON_A) || autoCommand) {
                if (cw->inRange) {
                    work->actionCommands.result = AC_RESULT_SUCCESS;
                    work->actionCommands.mResultCount++;
                } else {
                    if (autoCommand) {
                        break;
                    }
                    work->actionCommands.result = 0;
                }
                work->actionCommands.mAcState = 1002;
            }
            break;
        case 1002:
            cw->endTimer = 60;
            work->actionCommands.mAcState = 1003;
            if (cw->sfxId != -1) {
                psndSFXOff(cw->sfxId);
            }
        case 1003:
            cw->endTimer--;
            if (cw->endTimer <= 0) {
                work->actionCommands.mAcState = 1004;
            }
            break;
        case 1004:
            work->actionCommands.maincb = NULL;
            work->actionCommands.dispcb = NULL;
            work->actionCommands.resultcb = NULL;
            work->actionCommands.deletecb = NULL;
            return 0;
    }
    return 1;
}

s32 battleAcResult_MonosiriTarget(BattleWork* work) {
    return work->actionCommands.result;
}

void battleAcDisp_MonosiriTarget(CameraId cameraId, void* param) {
    BattleWork* work = _battleWorkPointer;
    switch (work->actionCommands.mAcState) {
        case 1000:
            actionCommandDisp(0.0f, 0.0f);
            break;
    }
}

void battleAcDelete_MonosiriTarget(BattleWork* work) {
    work->actionCommands.mAcState = 1002;
}

static void actionCommandDisp(f32 x, f32 y) {
    BattleWork* work = _battleWorkPointer;
    BattleActions* ac = &work->actionCommands;
    MonosiriTargetWork* cw = (MonosiriTargetWork*)ac->work;
    f32 alpha = 0.0f;
    Vec screen;
    Vec screen2;
    s32 icon;
    s32 pressedIcon;

    btlGetScreenPoint(&cw->pos, &screen);
    btlDispTex4(BTL_TEX(0x42), screen, (Vec){1.0f, 1.0f, 1.0f}, (Vec){0.0f, 0.0f, 0.0f}, (GXColor){255, 255, 255, 255});
    screen.x += 4.0f;
    screen.y -= 4.0f;
    screen.z -= 2.0f;
    btlDispTex4(BTL_TEX(0x42), screen, (Vec){1.0f, 1.0f, 1.0f}, (Vec){0.0f, 0.0f, 0.0f}, (GXColor){128, 128, 128, 255});

    if (cw->inRange) {
        s32 t = cw->inRangeCnt % 30;
        if (t < 20) {
            alpha = 0.01f * intplGetValue(EASING_CUBIC_OUT, 100.0f, 0.0f, t, 20);
        }
    }

    btlGetScreenPoint(&cw->targetPos, &screen2);
    btlDispTex4(BTL_TEX(0x41), screen2, (Vec){1.0f, 1.0f, 1.0f}, (Vec){0.0f, 0.0f, cw->angle}, (GXColor){255, 255, 255, 255});
    if (alpha > 0.0f) {
        btlDispTex4(BTL_TEX(0x41), screen2, (Vec){alpha, alpha, 1.0f}, (Vec){0.0f, 0.0f, cw->angle}, (GXColor){255, 255, 255, 255});
    }
    screen2.x += 4.0f;
    screen2.y -= 4.0f;
    screen2.z -= 2.0f;
    btlDispTex4(BTL_TEX(0x41), screen2, (Vec){1.0f, 1.0f, 1.0f}, (Vec){0.0f, 0.0f, cw->angle}, (GXColor){128, 128, 128, 255});
    cw->angle += 2.0f;

    pressedIcon = BattleACGetButtonIcon(PAD_BUTTON_A, 1);
    icon = BattleACGetButtonIcon(PAD_BUTTON_A, 0);
    if (__fabs(cw->pos.x - cw->targetPos.x) < 15.0) {
        icon = pressedIcon;
    }
    ((void (*)(Vec*, s32, s32, f32))iconDispGx)(&(Vec){-200.0f + x, 50.0f + y, 0.0f}, 0x10, icon, 1.0f);
}

