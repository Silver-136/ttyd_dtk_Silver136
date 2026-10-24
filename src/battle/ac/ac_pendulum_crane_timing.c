#include "battle/battle.h"
#include "battle/battle_ac.h"
#include "battle/battle_disp.h"
#include "battle/battle_pad.h"
#include "battle/battle_unit.h"
#include "driver/camdrv.h"
#include "driver/icondrv.h"
#include "pmario_sound.h"
#include "system.h"
#include <string.h>

#pragma optimize_for_size on
f32 sinfd(f32 degrees);
#include <stdlib.h>
#include "icon_ids.h"

typedef struct PendulumParams {
    u8 field_0x0[0x3C - 0x0];
    s32 unitId;    // 0x3C
    s32 partsId;   // 0x40
    s32 rotOffset; // 0x44
    s32 period;    // 0x48
    s32 maxFrames; // 0x4C
} PendulumParams;

typedef struct PendulumWork {
    s32 button;   // 0x0
    s32 frame;    // 0x4
    Vec basePos;  // 0x8
    Vec pos;      // 0x14
    f32 unk20;    // 0x20
    s32 unk24;    // 0x24
    f32 force;    // 0x28
    f32 angle;    // 0x2C
    f32 ang360;   // 0x30
    s32 cur;      // 0x34
    s32 vel;      // 0x38
    s32 flag;     // 0x3C
    s32 rate;     // 0x40
    s32 sfxId;    // 0x44
    s32 zone;     // 0x48
    s32 endTimer; // 0x4C
} PendulumWork;

#define CALC_DIR(d)                                   \
    if (cw->ang360 >= 90.0f && cw->ang360 < 270.0f) { \
        d = -1.0f;                                    \
    } else {                                          \
        d = 1.0f;                                     \
    }

#define PC_PARAMS(work) ((PendulumParams*)&(work)->actionCommands)

#pragma dont_inline on
u16 _hp_tbl[] = {20, 2, 35, 4, 45, 6, 55, 8, 65, 6, 80, 4, 100, 2};

s32 _get_angle_hp(s32 hp);
s32 _get_angle_rate(f32 angle);

s32 battleAcMain_PendulumCraneTiming(BattleWork* work) {
    PendulumWork* cw;
    BattleUnit* unit;
    BOOL autoCommand;
    BattleACDispParams* disp;
    BOOL inRange;
    BattleActions* ac;
    f32 dir;
    f32 dir2;
    u8* stick;
    f32 accel;
    f32 mag;
    s32 zone;
    s32 rate;

    ac = &work->actionCommands;
    autoCommand = FALSE;
    cw = (PendulumWork*)ac->work;
    disp = (BattleACDispParams*)&ac->disp;
    if (work->actionCommands.mAcUnit->badgesEquipped.AutoCommand) {
        autoCommand = TRUE;
    }
    unit = BattleGetUnitPtr(work, PC_PARAMS(work)->unitId);
    BtlUnit_GetPartsPtr(unit, PC_PARAMS(work)->partsId);

    switch (work->actionCommands.mAcState) {
        case 0:
            memset(disp, 0, sizeof(BattleACDispParams));
            disp->iconX = -300.0f;
            disp->iconY = 0.0f;
            disp->slideTimer = 20;
            cw->frame = 0;
            cw->angle = 0.0f;
            cw->cur = PC_PARAMS(work)->period;
            cw->vel = 0;
            cw->force = 3.0f;
            cw->zone = -1;
            cw->button = 0x100;
            work->actionCommands.outputParams[0] = 0;
            work->actionCommands.outputParams[1] = _get_angle_hp(50);
            work->actionCommands.outputParams[2] = _get_angle_hp(0);
            cw->rate = 50;
            work->actionCommands.mAcState = 99;
            work->actionCommands.result = 1;
            cw->sfxId = -1;
            break;
        case 100:
            BtlUnit_SetRotateOffset(unit, 0.0f, (f32)PC_PARAMS(work)->rotOffset, 0.0f);
            unit = BattleGetUnitPtr(work, PC_PARAMS(work)->unitId);
            BtlUnit_GetPartsPtr(unit, PC_PARAMS(work)->partsId);
            BtlUnit_GetPos(unit, &cw->pos.x, &cw->pos.y, &cw->pos.z);
            cw->basePos = cw->pos;
            cw->frame = 0;
            cw->unk20 = 0.5f;
            cw->flag = 1;
            work->actionCommands.mAcState = 1000;
        case 1000:
            cw->frame++;
            cw->ang360 = (f32)(((cw->cur + cw->vel) % (PC_PARAMS(work)->period * 2)) * 360 /
                               (PC_PARAMS(work)->period * 2));
            cw->angle = reviseAngle(45.0f * sinfd(cw->ang360));
            stick = (u8*)_battleWorkPointer;
            accel = 0.0f;
            CALC_DIR(dir)
            if (ac->acFlags & 1) {
                if (stick[0xF33] > 10 && stick[0xF32] < 10) {
                    if (dir < 0.0f) {
                        mag = stick[0xF33] / 128.0f;
                        if (mag > 1.0f) {
                            mag = 1.0f;
                        }
                        accel = 0.95f * -cw->force * mag;
                    }
                } else if (stick[0xF33] < 10) {
                    if (stick[0xF32] > 10) {
                        if (dir > 0.0f) {
                            mag = stick[0xF32] / 128.0f;
                            if (mag > 1.0f) {
                                mag = 1.0f;
                            }
                            accel = 0.95f * -cw->force * mag;
                        }
                    }
                }
            }
            cw->vel = (s32)((f32)cw->vel + accel);
            CALC_DIR(dir)
            cw->cur = (s32)((f32)cw->cur + cw->force);
            cw->ang360 = (f32)(((cw->cur + cw->vel) % (PC_PARAMS(work)->period * 2)) * 360 /
                               (PC_PARAMS(work)->period * 2));
            cw->angle = reviseAngle(45.0f * sinfd(cw->ang360));
            BtlUnit_SetRotate(unit, 0.0f, 0.0f, cw->angle);
            CALC_DIR(dir2)
            if (dir != dir2) {
                cw->flag = 0;
            }
            rate = _get_angle_rate(cw->angle);
            work->actionCommands.outputParams[1] = _get_angle_hp(rate);
            cw->rate = rate;
            rate = _get_angle_rate(cw->angle);
            zone = -1;
            if (rate <= 5) {
                zone = 0;
            } else if (rate >= 95) {
                zone = 1;
            }
            if (zone != -1 && zone != cw->zone) {
                psndSFXOn("SFX_AC_GAUGE_MOVE1");
                cw->zone = zone;
            }
            inRange = FALSE;
            if (autoCommand && (s32)work->actionCommands.outputParams[1] >= _get_angle_hp(50)) {
                inRange = TRUE;
            }
            if (BattlePadCheckTrigger(cw->button) || inRange) {
                work->actionCommands.outputParams[0] = 1;
                work->actionCommands.result = 2;
                work->actionCommands.mResultCount++;
                work->actionCommands.mAcState = 1002;
                break;
            }
            if (cw->frame > PC_PARAMS(work)->maxFrames) {
                work->actionCommands.result = 0;
                work->actionCommands.mAcState = 1002;
            }
            break;
        case 1002:
            ac->doneFlags |= 1;
            cw->endTimer = 60;
            if (cw->sfxId != 0xFFFFFFFF) {
                psndSFXOff(cw->sfxId);
            }
            work->actionCommands.mAcState = 1003;
        case 1003:
            cw->endTimer--;
            if (cw->endTimer > 0) {
                break;
            }
            work->actionCommands.mAcState = 1004;
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

s32 battleAcResult_PendulumCraneTiming(BattleWork* work) {
    return work->actionCommands.result;
}

static void actionCommandDisp(f32 x, f32 y);

void battleAcDisp_PendulumCraneTiming(CameraId cameraId, void* param) {
    BattleACDispParams* disp;
    BattleWork* w = _battleWorkPointer;
    BattleActions* ac = &w->actionCommands;
    disp = &ac->disp;
    switch (ac->mAcState) {
        case 99:
            disp->iconX = intplGetValue(4, 20 - disp->slideTimer, 20, -300.0f, 30.0f);
        case 100:
        case 1000:
            actionCommandDisp(disp->iconX, disp->iconY);
            if (disp->slideTimer > 0) {
                disp->slideTimer--;
            }
            break;
        case 1002:
        case 1003:
            if (disp->slideTimer >= 40) {
                disp->iconX = intplGetValue(4, disp->slideTimer - 40, 20, 30.0f, -300.0f);
            } else {
                disp->iconX = 30.0f;
            }
            actionCommandDisp(disp->iconX, disp->iconY);
            if (disp->slideTimer < 60) {
                (disp->slideTimer)++;
            }
            break;
    }
}

void battleAcDelete_PendulumCraneTiming(BattleWork* work) {
    if ((u32)work->actionCommands.doneFlags & 1) {
        return;
    }
    work->actionCommands.mAcState = 1002;
}

static void actionCommandDisp(f32 x, f32 y) {
    BattleWork* work = _battleWorkPointer;
    CameraEntry* cam;
    f32 baseX;
    f32 baseB;
    f32 px;
    f32 baseY;
    s32 i;

    camGetPtr(8);
    {
        Vec pos = {0.0f, 0.0f, 0.0f};
        pos.x = px = -200.0f + x;
        pos.y = 25.0f + y;
        iconDispGx(&pos, 0x10, ICON_08C, 1.0f);
    }
    cam = camGetPtr(8);
    btlDispGXInit2DRasta();
    GXLoadPosMtxImm(cam->view, 0);
    baseX = px - 88.0f;
    baseY = 50.0f + y;
    baseB = baseY - 18.0f;
    for (i = 0; i < 7; i++) {
        btlDispGXQuads2DRasta(i == 0 ? baseX : baseX + (f32)(_hp_tbl[i * 2 - 2] * 175) / 100.0f, baseY, baseX + (f32)(_hp_tbl[i * 2] * 175) / 100.0f, baseB, (3 - abs(3 - i)) * 60, 0,
                              180 - (3 - abs(3 - i)) * 60, 255);
    }
    {
        Vec pos2 = {0.0f, 0.0f, 0.0f};
        pos2.x = (-288.0f + x) + (f32)(((PendulumWork*)work->actionCommands.work)->rate * 176) / 100.0f;
        pos2.y = 45.0f + y;
        iconDispGx(&pos2, 0x10, ICON_096, 1.0f);
    }
}

s32 _get_angle_rate(f32 angle) {
    s32 r;
    f32 a;
    angle = compAngle(0.0f, angle);
    angle += 45.0f;
    a = 100.0f * angle;
    r = a / 90.0f;

    if (r < 0) {
        r = 0;
    }
    if (r > 100) {
        r = 100;
    }
    return r;
}

s32 _get_angle_hp(s32 hp) {
    u16* p = _hp_tbl;
    while (hp > p[0]) {
        p += 2;
    }
    return p[1];
}
