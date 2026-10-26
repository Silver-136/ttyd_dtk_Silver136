#include "battle/battle.h"
#include "battle/battle_ac.h"
#include "battle/battle_pad.h"
#include "battle/battle_unit.h"
#include "pmario_sound.h"
#include "system.h"
#include <string.h>
#include "version.h"
#include "battle/battle_sub.h"
#include "battle/battle_disp.h"
// US battle texture ids are 0x16 higher for these entries.
#define BTL_TEX(id) ((id) + VERSION_PICK(0, 0x16))



typedef struct ShotTargetWork {
    s32 frame;    // 0x0
    s32 endTimer; // 0x4
    Vec hitPos;   // 0x8
    Vec pos;      // 0x14
    s32 inRange;  // 0x20
    f32 angle;    // 0x24
    s16 periodX;  // 0x28
    s16 cntX;     // 0x2A
    f32 ampX;     // 0x2C
    s16 periodY;  // 0x30
    s16 cntY;     // 0x32
    f32 ampY;     // 0x34
    s32 sfxId;    // 0x38
} ShotTargetWork;

#define ST_WORK(ac) ((ShotTargetWork*)(ac)->work)
#define ST_DISP(ac) (&(ac)->disp)


static void _ac_disp_init(BattleWork* work);
static void actionCommandDisp(f32 x, f32 y);
void _ac_rumble_param_set(u32 difficulty, s16* period, s16* count, f32* amp);

s32 battleAcMain_ShotTarget(BattleWork* work) {
    const char* sfxName;
    BattleActions* ac;
    ShotTargetWork* cw;
    BOOL repeat;
    BOOL autoCommand;
    u32 difficulty;
    BattleUnit* unit;
    BattleUnitPart* parts;
    Vec hit;

    ac = &work->actionCommands;
    cw = ST_WORK(ac);
    autoCommand = FALSE;
    if (ac->mAcUnit->badgesEquipped.AutoCommand) {
        autoCommand = TRUE;
    }
    difficulty = work->actionCommands.params[3];
    do {
        sfxName = "SFX_AC_CURSOR_MOVE1";
        repeat = FALSE;
        switch (ac->mAcState) {
            case 0:
                _ac_disp_init(work);
                ac->result = 1;
                unit = BattleGetUnitPtr(work, work->actionCommands.params[0]);
                parts = BtlUnit_GetPartsPtr(unit, work->actionCommands.params[1]);
                BtlUnit_GetHitPos(unit, parts, &hit.x, &hit.y, &hit.z);
                cw->hitPos = hit;
                cw->inRange = 0;
                {
                    Vec p = {0.0f, 60.0f, 0.0f};
                    cw->pos = p;
                }
                cw->angle = 0.0f;
                _ac_rumble_param_set(difficulty, &cw->periodX, &cw->cntX, &cw->ampX);
                _ac_rumble_param_set(difficulty, &cw->periodY, &cw->cntY, &cw->ampY);
                cw->sfxId = -1;
                ac->mAcState = 99;
                break;
            case 99:
                break;
            case 100:
                cw->frame = work->actionCommands.params[2];
                ac->mAcState = 1000;
                cw->sfxId = psndSFXOn(sfxName);
            case 1000:
                if (work->actionCommands.countdown > 0) {
                    (work->actionCommands.countdown)--;
                    ac->result = 2;
                    repeat = TRUE;
                    ac->mResultCount++;
                    ac->mAcState = 1001;
                    break;
                }
                cw->cntX--;
                if (cw->cntX < 0) {
                    _ac_rumble_param_set(difficulty, &cw->periodX, &cw->cntX, &cw->ampX);
                }
                {
                    f32 s = cw->ampX * sinfd((cw->cntX + 1) * 180 / cw->periodX);
                    cw->pos.x += cw->ampX * sinfd(cw->cntX * 180 / cw->periodX) - s;
                }
                cw->cntY--;
                if (cw->cntY < 0) {
                    _ac_rumble_param_set(difficulty, &cw->periodY, &cw->cntY, &cw->ampY);
                }
                {
                    f32 s = cw->ampY * sinfd((cw->cntY + 1) * 180 / cw->periodY);
                    cw->pos.y += cw->ampY * sinfd(cw->cntY * 180 / cw->periodY) - s;
                }
                cw->pos.x += work->padWork[0].mPadData[0].stickX / 10.0f;
                cw->pos.y += work->padWork[0].mPadData[0].stickY / 10.0f;
                if (autoCommand) {
                    if (__fabs(cw->pos.x - cw->hitPos.x) < 3.0) {
                        cw->pos.x = cw->hitPos.x;
                    } else if (cw->pos.x > cw->hitPos.x) {
                        cw->pos.x -= 3.0f;
                    } else {
                        cw->pos.x += 3.0f;
                    }
                    if (__fabs(cw->pos.y - cw->hitPos.y) < 3.0) {
                        cw->pos.y = cw->hitPos.y;
                    } else if (cw->pos.y > cw->hitPos.y) {
                        cw->pos.y -= 3.0f;
                    } else {
                        cw->pos.y += 3.0f;
                    }
                }
                if (__fabs(distABf(cw->pos.x, cw->pos.y, cw->hitPos.x, cw->hitPos.y)) <= 28.0) {
                    cw->inRange = 1;
                } else {
                    cw->inRange = 0;
                }
                cw->frame--;
                if ((ac->setupFlags & 1) && ((u32)ac->mAcUnit->mTokenFlags & 0x10)) {
                    if (irand(100) >= 0) {
                        ac->result = 0;
                    } else {
                        ac->result = 2;
                        ac->mResultCount++;
                    }
                    ac->mAcState = 1001;
                    break;
                }
                if (cw->frame < 0) {
                    if (cw->inRange) {
                        ac->result = 2;
                        ac->mResultCount++;
                    } else {
                        ac->result = 0;
                    }
                    ac->mAcState = 1001;
                }
                break;
            case 1001:
                ac->mAcState = 1002;
                break;
            case 1002:
                break;
            case 1003:
                cw->endTimer = 60;
                ac->mAcState = 1004;
                if ((u32)cw->sfxId != 0xFFFFFFFF) {
                    psndSFXOff(cw->sfxId);
                }
                break;
            case 1004:
                cw->endTimer--;
                if (cw->endTimer <= 0) {
                    ac->mAcState = 1005;
                }
                break;
            case 1005:
                ac->maincb = NULL;
                ac->dispcb = NULL;
                ac->resultcb = NULL;
                ac->deletecb = NULL;
                break;
        }
    } while (repeat);
    return 1;
}

void _ac_rumble_param_set(u32 difficulty, s16* period, s16* count, f32* amp) {
    *period = irand(120) + 120;
    *count = *period - 1;
    *amp = 25.0f;
    if (irand(100) < 50) {
        *amp = -1.0f * *amp;
    }
    switch (difficulty) {
        case 0:
            *amp = 0.6f * (0.25f * *amp);
            break;
        case 1:
            *amp = 0.6f * (0.5f * *amp);
            break;
        case 2:
            *amp = 0.6f * (0.8f * *amp);
            break;
        case 3:
            *amp = 0.6f * *amp;
            break;
        case 4:
            *amp = 0.6f * (1.5f * *amp);
            break;
        case 5:
            *amp = 0.6f * (2.0f * *amp);
            break;
        case 6:
        default:
            *amp = 0.6f * (3.0f * *amp);
            break;
    }
}

s32 battleAcResult_ShotTarget(BattleWork* work) {
    return work->actionCommands.result;
}

void battleAcDisp_ShotTarget(CameraId cameraId, void* param) {
    BattleWork* work = param;
    BattleActions* ac = &work->actionCommands;
    BattleACDispParams* d = ST_DISP(ac);

    switch (ac->mAcState) {
        case 99:
        case 100:
        case 1000:
        case 1001:
        case 1002:
            d->iconX = intplGetValue(EASING_QUADRATIC_OUT, -200.0f, 30.0f, 20 - d->slideTimer, 20);
            actionCommandDisp(d->iconX, d->iconY);
            if (d->slideTimer > 0) {
                d->slideTimer--;
            }
            break;
        case 1003:
        case 1004:
            if (d->slideTimer >= 40) {
                d->iconX = intplGetValue(EASING_QUADRATIC_OUT, 30.0f, -200.0f, d->slideTimer - 40, 20);
            } else {
                d->iconX = 30.0f;
            }
            actionCommandDisp(d->iconX, d->iconY);
            if (d->slideTimer < 60) {
                d->slideTimer++;
            }
            break;
    }
}

void battleAcDelete_ShotTarget(BattleWork* work) {
    work->actionCommands.mAcState = 1003;
}

static void _ac_disp_init(BattleWork* work) {
    BattleACDispParams* d = ST_DISP(&work->actionCommands);
    memset(d, 0, 0x2C);
    d->iconX = -200.0f;
    d->iconY = 0.0f;
    d->slideTimer = 20;
    d->color = (GXColor){255, 255, 255, 255};
    d->colorStep[0] = 0;
    d->colorStep[1] = -16;
    d->colorStep[2] = -16;
    d->colorStep[3] = 0;
    d->gaugeRatio = 0.0f;
}

static void actionCommandDisp(f32 x, f32 y) {
    BattleWork* work = _battleWorkPointer;
    BattleActions* ac = &work->actionCommands;
    ShotTargetWork* cw = ST_WORK(ac);
    Vec screen;
    Vec screen2;

    switch (ac->mAcState) {
        case 100:
        case 1000:
        case 1001:
        case 1002:
            cw->angle += 2.0f;
            btlGetScreenPoint(&cw->pos, &screen);
            btlDispTex4(BTL_TEX(0x42), screen, (Vec){1.0f, 1.0f, 1.0f}, (Vec){0.0f, 0.0f, 0.0f}, (GXColor){255, 255, 255, 255});
            screen.x += 4.0f;
            screen.y -= 4.0f;
            screen.z -= 2.0f;
            btlDispTex4(BTL_TEX(0x42), screen, (Vec){1.0f, 1.0f, 1.0f}, (Vec){0.0f, 0.0f, 0.0f}, (GXColor){128, 128, 128, 255});
            btlGetScreenPoint(&cw->hitPos, &screen2);
            btlDispTex4(BTL_TEX(0x41), screen2, (Vec){1.0f, 1.0f, 1.0f}, (Vec){0.0f, 0.0f, cw->angle}, (GXColor){255, 255, 255, 255});
            screen2.x += 4.0f;
            screen2.y -= 4.0f;
            screen2.z -= 2.0f;
            btlDispTex4(BTL_TEX(0x41), screen2, (Vec){1.0f, 1.0f, 1.0f}, (Vec){0.0f, 0.0f, cw->angle}, (GXColor){128, 128, 128, 255});
            break;
    }
}
