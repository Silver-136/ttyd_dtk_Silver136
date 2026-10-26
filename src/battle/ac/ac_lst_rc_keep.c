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
    s32 timer;  // 0x2BC
    s32 count;  // 0x2C0
    s16 held0;  // 0x2C4
    s16 held1;  // 0x2C6
    s32 endTimer; // 0x2C8
} LstRcWork;

#define LR_WORK(ac) ((LstRcWork*)(ac)->work)

s32 ac_lstrckeep_ok_frame[] = {0x50, 0x46, 0x3C, 0x28, 0x14, 0x0A, 0x05};
const char* ac_lstrckeep_iconname[] = {"_AcSkl1", "_AcSkl2", "_AcSkl3", "_AcSkl4"};

static void actionCommandDisp(f32 x, f32 y, s32 n);

s32 battleAcMain_LstRcKeep(BattleWork* work) {
    BattleActions* ac = &work->actionCommands;
    LstRcWork* lw = LR_WORK(ac);
    BattleACDispParams* disp = (BattleACDispParams*)&ac->disp;
    s32 loop;
    s32 off;
    s32 autoCmd = 0;
    s32 ok;
    s32 d;
    s32 tm;
    s32 n;
    s32 i;
    if (ac->mAcUnit->badgesEquipped.AutoCommand) {
        autoCmd = 1;
    }
    do {
        loop = 0;
        switch (ac->mAcState) {
            case 0:
                ac->result = 1;
                lw->count = 0;
                memset(disp, 0, sizeof(BattleACDispParams));
                disp->iconX = -300.0f;
                disp->iconY = 0.0f;
                disp->slideTimer = 20;
                ac->mAcState = 99;
                break;
            case 99:
                break;
            case 100:
                lw->timer = 120;
                ac->mAcState = 1000;
                lw->held0 = 0;
                lw->held1 = 0;
            case 1000:
                if ((ac->setupFlags & 1) && (ac->mAcUnit->mTokenFlags & kConfusedAction)) {
                    if (ac->setupFlags & 2) {
                        ac->setupParam--;
                        if (ac->setupParam >= 0) {
                            break;
                        }
                    }
                    if (irand(100) >= 0) {
                        ac->result = 0;
                    } else {
                        ac->result = 2;
                        ac->mResultCount++;
                    }
                    return 0;
                }
                if (lw->held0 == 0) {
                    if (BattlePadCheckTrigger(BATTLE_PAD_STICK_LEFT)) {
                        lw->held0 = 1;
                    }
                } else {
                    lw->held0++;
                    if (BattlePadCheckNow(BATTLE_PAD_STICK_LEFT) == 0) {
                        lw->held0 = 0;
                    }
                }
                if (lw->held1 == 0) {
                    if (BattlePadCheckTrigger(BATTLE_PAD_SUBSTICK_RIGHT)) {
                        lw->held1 = 1;
                    }
                } else {
                    lw->held1++;
                    if (BattlePadCheckNow(BATTLE_PAD_SUBSTICK_RIGHT) == 0) {
                        lw->held1 = 0;
                    }
                }
                if ((lw->held0 != 0 && lw->held1 != 0) || ac->countdown > 0 || autoCmd) {
                    ac->mAcState = 1001;
                    lw->timer = ac->params[0];
                    lw->held1 = (lw->held0 = (lw->count = 0));
                    break;
                }
                lw->timer--;
                if (lw->timer <= 0) {
                    ac->result = 0;
                    loop = 1;
                    ac->mAcState = 1003;
                }
                break;
            case 1001:
                if (ac->countdown > 0) {
                    ac->countdown--;
                    ac->result = 2;
                    loop = 1;
                    ac->mAcState = 1003;
                }
                lw->timer--;
                if (lw->held0 > 0) {
                    lw->held0++;
                } else if (BattlePadCheckNow(BATTLE_PAD_STICK_LEFT) == 0) {
                    lw->held0 = 1;
                }
                if (lw->held1 > 0) {
                    lw->held1++;
                } else if (BattlePadCheckNow(BATTLE_PAD_SUBSTICK_RIGHT) == 0) {
                    lw->held1 = 1;
                }
                if (!autoCmd) {
                    if (lw->held0 > 5 || lw->held1 > 5) {
                        ac->result &= ~1;
                        loop = 1;
                        ac->mAcState = 1003;
                        break;
                    }
                }
                tm = lw->timer;
                if (tm < 0) {
                    lw->count++;
                    if (lw->count >= 5) {
                        ac->result &= ~1;
                        loop = 1;
                        ac->mAcState = 1003;
                        break;
                    }
                    lw->timer = ac->params[lw->count];
                    break;
                }
                if (!(lw->held0 != 0 && lw->held1 != 0) && !autoCmd) {
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
                    ac->result |= 2;
                    ac->result &= ~1;
                    loop = 1;
                    ac->mAcState = 1003;
                    break;
                }
                if (ok) {
                    ac->result |= 2;
                    ac->result &= ~1;
                    loop = 1;
                    ac->mAcState = 1003;
                    break;
                }
                ac->result &= ~1;
                loop = 1;
                ac->mAcState = 1003;
                break;
            case 1003:
                n = lw->count - 1; if (n >= 0) {
                for (i = 0, off = 0; i <= n; i++, off += 4) { iconDelete(ac_lstrckeep_iconname[off >> 2]);
                } }
                ac->mAcState = 1004;
            case 1004:
            case 1005:
                lw->endTimer = 60;
                ac->mAcState = 1006;
                break;
            case 1006:
                lw->endTimer--;
                if (lw->endTimer <= 0) {
                    ac->mAcState = 1007;
                }
                break;
            case 1007:
                ac->maincb = NULL;
                ac->dispcb = NULL;
                ac->resultcb = NULL;
                ac->deletecb = NULL;
                break;
        }
    } while (loop);
    return 1;
}

s32 battleAcResult_LstRcKeep(BattleWork* work) {
    return work->actionCommands.result;
}

void battleAcDisp_LstRcKeep(CameraId cameraId, void* param) {
    BattleWork* work = (BattleWork*)param;
    BattleActions* ac = &work->actionCommands;
    BattleACDispParams* disp = &ac->disp;
    LstRcWork* lw = (LstRcWork*)ac->work;
    switch (ac->mAcState) {
        case 99:
        case 100:
        case 1000:
            disp->iconStep = -1;
            disp->iconX = intplGetValue(EASING_QUADRATIC_OUT, -300.0f, 30.0f, 20 - disp->slideTimer, 20);
            actionCommandDisp(disp->iconX, disp->iconY, disp->iconStep);
            if (disp->slideTimer > 0) {
                (disp->slideTimer)--;
            }
            break;
        case 1001:
            disp->iconStep = lw->count - 1;
            disp->iconX = intplGetValue(EASING_QUADRATIC_OUT, -300.0f, 30.0f, 20 - disp->slideTimer, 20);
            actionCommandDisp(disp->iconX, disp->iconY, disp->iconStep);
            if (disp->slideTimer > 0) {
                (disp->slideTimer)--;
            }
            break;
        case 1002:
        case 1003:
        case 1004:
            actionCommandDisp(30.0f, 0.0f, disp->iconStep);
            break;
        case 1005:
        case 1006:
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

void battleAcDelete_LstRcKeep(BattleWork* work) {
    work->actionCommands.mAcState = 1005;
}

#pragma optimize_for_size on
static void actionCommandDisp(f32 x, f32 y, s32 n) {
    s32 off;
    s32 i;
    camGetPtr(CAMERA_2D);
    if (n < 3) {
        Vec pos = {0.0f, 0.0f, 0.0f};
        pos.x = -168.0f + x;
        pos.y = 70.0f + y;
        iconDispGx(&pos, 0x10, ICON_071, 1.0f);
    } else {
        Vec pos = {0.0f, 0.0f, 0.0f};
        pos.x = -168.0f + x;
        pos.y = 70.0f + y;
        iconDispGx(&pos, 0x10, ICON_070, 1.0f);
    }
    if (n < 3) {
        Vec pos = {0.0f, 0.0f, 0.0f};
        pos.x = -232.0f + x;
        pos.y = 70.0f + y;
        iconDispGx(&pos, 0x10, ICON_STICK_LEFT, 1.0f);
    } else {
        Vec pos = {0.0f, 0.0f, 0.0f};
        pos.x = -232.0f + x;
        pos.y = 70.0f + y;
        iconDispGx(&pos, 0x10, ICON_STICK_NEUTRAL, 1.0f);
    }
    {
        Vec pos = {0.0f, 0.0f, 0.0f};
        pos.x = -200.0f + x;
        y = 25.0f + y;
        pos.y = y;
        iconDispGx(&pos, 0x10, ICON_08E, 1.0f);
    }
    i = 0;
    off = 0;
    for (; i < 3; i++, off += 0x21) {
        if (i <= n) {
            Vec pos = {0.0f, 0.0f, 0.0f};
            pos.x = x + (f32)(off - 0x102);
            pos.y = y;
            iconDispGx(&pos, 0x10, ICON_092 + i, 1.0f);
        } else {
            Vec pos = {0.0f, 0.0f, 0.0f};
            pos.x = x + (f32)(off - 0x102);
            pos.y = y;
            iconDispGx(&pos, 0x10, ICON_091, 0.5f);
        }
    }
    if (n < 3) {
        Vec pos = {0.0f, 0.0f, 0.0f};
        pos.x = -151.0f + x;
        pos.y = y;
        iconDispGx(&pos, 0x10, ICON_091, 1.0f);
    } else {
        Vec pos = {0.0f, 0.0f, 0.0f};
        pos.x = -151.0f + x;
        pos.y = y;
        iconDispGx(&pos, 0x10, ICON_095, 1.0f);
    }
}
#pragma optimize_for_size reset

