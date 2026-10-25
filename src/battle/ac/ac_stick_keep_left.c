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

#define SKL_P(work) (&(work)->actionCommands) // params: 0 count, 1 frames, 2 confused, 3 end flag, 4 sfx played
#define SKL_W(w) ((SklWork*)(w)->actionCommands.work)
#define SKL_D(w) (&(w)->actionCommands.disp)


s32 ac_stickkeepleft_ok_frame[7] = {80, 70, 60, 40, 20, 10, 5};

static void actionCommandDisp(f32 x, f32 y, s32 unused);

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

    autoCommand = 0;
    cw = SKL_W(work);
    disp = SKL_D(work);
    if (work->actionCommands.mAcUnit->badgesEquipped.AutoCommand) {
        autoCommand = 1;
    }
    SKL_P(work)->params[3] = 0;
    if (work->actionCommands.mAcState != 0) {
        if (SKL_P(work)->params[2] != 0) {
            SKL_P(work)->params[3] = 1;
        } else {
            SKL_P(work)->params[3] = 0;
        }
    }
    sfx[0] = "SFX_AC_PI1";
    sfx[1] = "SFX_AC_PONE1";
    do {
        loop = 0;
        if (SKL_P(work)->params[3] != 0) {
            break;
        }
        switch (work->actionCommands.mAcState) {
            case 0:
                work->actionCommands.result = 1;
                cw->idx = 0;
                memset(disp, 0, sizeof(BattleACDispParams));
                disp->iconX = -300.0f;
                disp->iconY = 0.0f;
                disp->slideTimer = 20;
                work->actionCommands.outputParams[0] = 0;
                SKL_P(work)->params[2] = 0;
                SKL_P(work)->params[3] = 0;
                SKL_P(work)->params[4] = 0;
                work->actionCommands.outputParams[2] = 0;
                for (i = 0; i < 12; i++) {
                    cw->sfx[i] = -1;
                }
                work->actionCommands.mAcState = 99;
                break;
            case 99:
                break;
            case 100:
                cw->timer = 120;
                work->actionCommands.mAcState = 1000;
                // fallthrough
            case 1000:
                if ((work->actionCommands.setupFlags & 1) && (work->actionCommands.mAcUnit->mTokenFlags & 0x10)) {
                    if (work->actionCommands.setupFlags & 2) {
                        work->actionCommands.setupParam--;
                        if (work->actionCommands.setupParam >= 0) {
                            break;
                        }
                    }
                    if (irand(100) >= 0) {
                        work->actionCommands.result = 0;
                    } else {
                        work->actionCommands.result = 2;
                        work->actionCommands.mResultCount++;
                    }
                    return 0;
                }
                if (!BattlePadCheckNow(BATTLE_PAD_STICK_LEFT) && (&(work)->actionCommands)->countdown <= 0 && autoCommand == 0) {
                    if (--cw->timer <= 0) {
                        work->actionCommands.result = 0;
                        loop = 1;
                        work->actionCommands.mAcState = 1003;
                    }
                } else {
                    work->actionCommands.outputParams[0] = 1;
                    work->actionCommands.mAcState = 1001;
                    cw->timer = 1;
                    cw->idx = 1;
                    work->actionCommands.outputParams[1] = 0;
                }
                break;
            case 1001:
                if ((&(work)->actionCommands)->countdown > 0) {
                    (&(work)->actionCommands)->countdown--;
                    loop = 1;
                    work->actionCommands.result = 2;
                    work->actionCommands.mAcState = 1003;
                }
                cw->timer--;
                if (cw->timer < 0) {
                    cw->idx++;
                    work->actionCommands.outputParams[1]++;
                    if (cw->idx >= SKL_P(work)->params[0] + 2) {
                        work->actionCommands.result &= ~1;
                        work->actionCommands.mAcState = 1003;
                        work->actionCommands.outputParams[2] = 2;
                        if (SKL_P(work)->acFlags & 2) {
                            work->actionCommands.result |= 2;
                            work->actionCommands.result &= ~1;
                        }
                        loop = 1;
                        break;
                    }
                    j = cw->idx - 2;
                    if (j < SKL_P(work)->params[0] - 1) {
                        if (cw->sfx[j] == -1) {
                            cw->sfx[j] = psndSFXOn(sfx[0]);
                        }
                    } else {
                        if (cw->sfx[j] == -1) {
                            cw->sfx[j] = psndSFXOn(sfx[1]);
                            SKL_P(work)->params[4] = 1;
                        }
                    }
                    cw->timer = SKL_P(work)->params[1];
                } else {
                    if (BattlePadCheckNow(BATTLE_PAD_STICK_LEFT) && autoCommand == 0) {
                        break;
                    }
                    ok = 0;
                    if (cw->idx != SKL_P(work)->params[0] + 1) {
                        if (!(SKL_P(work)->acFlags & 2)) {
                            work->actionCommands.outputParams[2] = 1;
                        }
                    } else {
                        diff = SKL_P(work)->params[1] - cw->timer;
                        if (diff > ac_stickkeepleft_ok_frame[BattleActionCommandGetDifficulty(work)]) {
                            if (!(SKL_P(work)->acFlags & 2)) {
                                work->actionCommands.outputParams[2] = 1;
                            }
                        } else {
                            work->actionCommands.outputParams[2] = 3;
                            ok = 1;
                        }
                    }
                    if (autoCommand) {
                        if (!ok) {
                            break;
                        }
                        work->actionCommands.result |= 2;
                        work->actionCommands.result &= ~1;
                        loop = 1;
                        work->actionCommands.mAcState = 1003;
                    } else if (ok) {
                        work->actionCommands.result |= 2;
                        work->actionCommands.result &= ~1;
                        loop = 1;
                        work->actionCommands.mAcState = 1003;
                    } else if (!(SKL_P(work)->acFlags & 2)) {
                        work->actionCommands.result &= ~1;
                        loop = 1;
                        work->actionCommands.mAcState = 1003;
                    }
                }
                break;
            case 1003:
                {
                    s32 k;
                    j = cw->idx - 2; if (j >= 0) for (k = 0; k <= j; k++) {
                    }
                }
                work->actionCommands.mAcState = 1004;
                break;
            case 1005:
                cw->endTimer = 60;
                work->actionCommands.mAcState = 1006;
                break;
            case 1006:
                cw->endTimer--;
                if (cw->endTimer <= 0) {
                    work->actionCommands.mAcState = 1007;
                }
                break;
            case 1007:
                work->actionCommands.maincb = NULL;
                work->actionCommands.dispcb = NULL;
                work->actionCommands.resultcb = NULL;
                work->actionCommands.deletecb = NULL;
                break;
        }
    } while (loop);
    return 1;
}

s32 battleAcResult_StickKeepLeft(BattleWork* work) {
    return work->actionCommands.result;
}

void battleAcDisp_StickKeepLeft(CameraId camId, void* param) {
    BattleWork* work = (BattleWork*)param;
    BattleActions* ac = &work->actionCommands;
    SklWork* cw = (SklWork*)ac->work;
    BattleACDispParams* disp = &ac->disp;

    switch (work->actionCommands.mAcState) {
        case 0x63:
        case 0x64:
        case 0x3E8:
            disp->iconStep = -1;
            disp->iconX = intplGetValue(4, -300.0f, 30.0f, 20 - disp->slideTimer, 20);
            actionCommandDisp(disp->iconX, disp->iconY, disp->iconStep);
            if (SKL_P(work)->params[3] == 0 && disp->slideTimer > 0) {
                disp->slideTimer--;
            }
            break;
        case 0x3E9:
            disp->iconStep = cw->idx - 1;
            disp->iconX = intplGetValue(4, -300.0f, 30.0f, 20 - disp->slideTimer, 20);
            actionCommandDisp(disp->iconX, disp->iconY, disp->iconStep);
            if (SKL_P(work)->params[3] == 0 && disp->slideTimer > 0) {
                disp->slideTimer--;
            }
            break;
        case 0x3EA:
        case 0x3EB:
        case 0x3EC:
            actionCommandDisp(30.0f, 0.0f, disp->iconStep);
            break;
        case 0x3ED:
        case 0x3EE:
            if (disp->slideTimer >= 40) {
                disp->iconX = intplGetValue(4, 30.0f, -300.0f, disp->slideTimer - 40, 20);
            } else {
                disp->iconX = 30.0f;
            }
            actionCommandDisp(disp->iconX, disp->iconY, disp->iconStep);
            if (SKL_P(work)->params[3] == 0 && disp->slideTimer < 60) {
                disp->slideTimer++;
            }
            break;
    }
}

void battleAcDelete_StickKeepLeft(BattleWork* work) {
    work->actionCommands.mAcState = 0x3ED;
}

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

    ac = SKL_P(_battleWorkPointer);
    camGetPtr(CAMERA_2D);
    count = ac->params[0];
    idx = ((SklWork*)ac->work)->idx - 1;
    switch (ac->mAcState) {
        case 0:
        case 0x63:
        case 0x64: {
            Vec p = {0.0f, 0.0f, 0.0f};
            p.x = -200.0f + x;
            p.y = 70.0f + y;
            iconDispGx(&p, 0x10, ICON_079, 1.0f);
            break;
        }
        case 0x3E8:
            if (gp->retraceCount & 8) {
                Vec p = {0.0f, 0.0f, 0.0f};
                p.x = -200.0f + x;
                p.y = 70.0f + y;
                iconDispGx(&p, 0x10, ICON_078, 1.0f);
            } else {
                Vec p = {0.0f, 0.0f, 0.0f};
                p.x = -200.0f + x;
                p.y = 70.0f + y;
                iconDispGx(&p, 0x10, ICON_079, 1.0f);
            }
            break;
        default:
            if (idx < count) {
                Vec p = {0.0f, 0.0f, 0.0f};
                p.x = -200.0f + x;
                p.y = 70.0f + y;
                iconDispGx(&p, 0x10, ICON_078, 1.0f);
            } else {
                Vec p = {0.0f, 0.0f, 0.0f};
                p.x = -200.0f + x;
                p.y = 70.0f + y;
                iconDispGx(&p, 0x10, ICON_079, 1.0f);
            }
            break;
    }

    n = 10;
    if (count >= 4) {
        n = count * 2 + 3;
        if (ac->acFlags & 1) {
            n += 2;
        }
    }
    spacing = 16.0f;
    if (ac->acFlags & 1) {
        spacing = 12.0f;
    }
    {
        Vec p = {0.0f, 0.0f, 0.0f};
        p.x = -288.0f + x;
        yy = 25.0f + y;
        p.y = yy;
        iconDispGx(&p, 0x10, ICON_08F, 1.0f);
    }
    for (i = 1; i < n; i++) {
        Vec p = {0.0f, 0.0f, 0.0f};
        p.x = x + (spacing * (f32)i + -288.0f);
        p.y = yy;
        iconDispGx(&p, 0x10, ICON_08E, 1.0f);
    }
    {
        Vec p = {0.0f, 0.0f, 0.0f};
        p.x = x + (spacing * (f32)i + -288.0f);
        p.y = yy;
        iconDispGx(&p, 0x10, ICON_090, 1.0f);
    }
    stepX = 33.0f;
    if (ac->acFlags & 1) {
        stepX = 24.75f;
    }
    m = count - 1;
    for (j = 0; j < m; j++) {
        if (j < idx) {
            if (j < 3) {
                Vec p = {0.0f, 0.0f, 0.0f};
                p.x = x + (stepX * (f32)j + -258.0f);
                p.y = yy;
                iconDispGx(&p, 0x10, (u16)(ICON_092 + j), 1.0f);
            } else {
                Vec p = {0.0f, 0.0f, 0.0f};
                p.x = x + (stepX * (f32)j + -258.0f);
                p.y = yy;
                iconDispGx(&p, 0x10, ICON_094, 1.0f);
            }
        } else {
            Vec p = {0.0f, 0.0f, 0.0f};
            p.x = x + (stepX * (f32)j + -258.0f);
            p.y = yy;
            iconDispGx(&p, 0x10, ICON_091, 0.5f);
        }
    }
    if (idx < count) {
        Vec p = {0.0f, 0.0f, 0.0f};
        p.x = x + (stepX * (f32)m + -250.0f);
        p.y = yy;
        iconDispGx(&p, 0x10, ICON_091, 1.0f);
    } else {
        Vec p = {0.0f, 0.0f, 0.0f};
        p.x = x + (stepX * (f32)m + -250.0f);
        p.y = yy;
        iconDispGx(&p, 0x10, ICON_095, 1.0f);
    }
}
