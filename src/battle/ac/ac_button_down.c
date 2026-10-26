#include "battle/battle.h"
#include <dolphin/pad.h>
#include "battle/battle_ac.h"
#include "battle/battle_pad.h"
#include "battle/battle_unit.h"
#include "driver/icondrv.h"
#include "pmario_sound.h"
#include "system.h"
#include <string.h>
#include "icon_ids.h"

typedef struct BDWork {
    s32 timer;
    s32 sndCount;
    u32 button[13];
    s32 pressed[13];
    s32 endTimer;
} BDWork;


static u32 _button_table[7] = {PAD_BUTTON_A, PAD_BUTTON_B, PAD_BUTTON_X, PAD_BUTTON_Y, PAD_TRIGGER_L, PAD_TRIGGER_R, PAD_TRIGGER_Z};

static void actionCommandDisp(s32 unused, f32 x, f32 y);

// FAKE: the button[]/pressed[] indexes are cast (u32) (i, j >= 0 there, same value);
// the cast stops IR strength reduction so the backend emits the target addi/lwzx
// and the shared offset IV. allDone is declared first for the register order.
s32 battleAcMain_ButtonDown(BattleWork *work) {
    BattleActions *ac = &work->actionCommands;
    BDWork *p = (BDWork*)work->actionCommands.work;
    BattleACDispParams *disp = &work->actionCommands.disp;
    s32 allDone;
    s32 i;
    s32 done;
    s32 flag = 0;
    const char *sfx;
    if (ac->mAcUnit->badgesEquipped.AutoCommand) {
        flag = 1;
    }
    // FAKE: comma expression `(0, X)`: X becomes a separate compiler temp; the plain X changes the object
    sfx = (0, "SFX_AC_BUTTON_PUSH1");
    do {
        allDone = 0;
        switch (ac->mAcState) {
            case 0: {
                    ac->result = 1;
                    memset(disp, 0, 0x2c);
                    disp->iconX = -300.0f;
                    disp->iconY = 0.0f;
                    disp->slideTimer = 20;
                    memset(p->button, 0, 0x34);
                    memset(p->pressed, 1, 0x34);
                    for (i = 0; i < (work->actionCommands.params[2]); i++) {
                        switch (work->actionCommands.params[3]) {
                            case -2:
                                p->button[(u32)i] = (irand(100) < 50) ? (0x100) : (0x200);
                                p->pressed[(u32)i] = 0;
                                break;

                            case -4:
                                p->button[(u32)i] = (irand(100) < 50) ? (0x200) : (0x400);
                                p->pressed[(u32)i] = 0;
                                break;

                            case -3:
                                if ((work->actionCommands.acFlags) & AC_BUTTON_DOWN_FLAG_ALLOW_REPEATS) {
                                    p->button[(u32)i] = _button_table[irand(4)];
                                    p->pressed[(u32)i] = 0;
                                } else {
                                    s32 found = 0;
                                    u32 cand;
                                    while (!found) {
                                        s32 j;
                                        s32 ok;
                                        cand = _button_table[irand(4)];
                                        ok = 1;
                                        for (j = i - 1; j >= 0; j--) {
                                            if (p->button[(u32)j] == cand) {
                                                ok = 0;
                                            }
                                        }

                                        if (ok) {
                                            found = 1;
                                        }
                                    }

                                    p->button[(u32)i] = cand;
                                    p->pressed[(u32)i] = 0;
                                }
                                break;

                            default: {
                                    s32 found = 0;
                                    u32 cand;
                                    while (!found) {
                                        s32 j;
                                        s32 ok;
                                        cand = _button_table[irand(7)];
                                        ok = 1;
                                        for (j = i - 1; j >= 0; j--) {
                                            if (p->button[(u32)j] == cand) {
                                                ok = 0;
                                            }
                                        }

                                        if (ok) {
                                            found = 1;
                                        }
                                    }

                                    p->button[(u32)i] = cand;
                                    p->pressed[(u32)i] = 0;
                                    break;
                                }

                        }

                    }

                    ac->mAcState = 99;
                    break;
                }

            case 99:
                break;

            case 100:
                ac->mAcState = 1000;
                p->sndCount = 0;
                p->timer = work->actionCommands.params[0];

            case 1000:
                if ((ac->setupFlags & 1) && (((u32)ac->mAcUnit->mTokenFlags) & 0x10)) {
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
                if ((work->actionCommands.countdown) > 0) {
                    (work->actionCommands.countdown)--;
                    ac->result = 2;
                    allDone = 1;
                    ac->mAcState = 1001;
                    break;
                }
                *((s32*)(&ac->outputParams[0])) = 0;
                done = 1;
                for (i = 0; i < (work->actionCommands.params[2]); i++) {
                    if (p->button[(u32)i]) {
                        if (p->pressed[(u32)i]) {
                            (*((s32*)(&ac->outputParams[0])))++;
                        } else
                            if (((p->button[(u32)i]) == BattlePadGetTrigger()) || flag) {
                            p->pressed[(u32)i] = 1;
                            (*((s32*)(&ac->outputParams[0])))++;
                            if ((work->actionCommands.params[1]) == 1) {
                                if ((i + 1) < (work->actionCommands.params[2])) {
                                    done = 0;
                                }
                                break;
                            }
                        } else
                            if (BattlePadGetTrigger()) {
                            ac->result = 0;
                            ac->mAcState = 1001;
                            if (((work->actionCommands.acFlags) & AC_BUTTON_DOWN_FLAG_PARTIAL_SUCCESS) && ((*((s32*)(&ac->outputParams[0]))) > 0)) {
                                allDone = 1;
                                ac->result |= 2;
                                ac->result &= ~1;
                                ac->mAcState = 1001;
                                break;
                            }
                            return 1;
                        } else {
                            done = 0;
                            if ((work->actionCommands.params[1]) == 1) {
                                break;
                            }
                        }
                    }
                }

                if (p->sndCount != (*((s32*)(&ac->outputParams[0])))) {
                    psndSFXOn(sfx);
                    p->sndCount = *((s32*)(&ac->outputParams[0]));
                }
                if (done) {
                    ac->result |= 2;
                    allDone = 1;
                    ac->result &= ~1;
                    ac->mAcState = 1001;
                    break;
                }
                p->timer--;
                if (p->timer < 0) {
                    ac->result = 0;
                    allDone = 1;
                    ac->mAcState = 1001;
                    if (((work->actionCommands.acFlags) & AC_BUTTON_DOWN_FLAG_PARTIAL_SUCCESS) && ((*((s32*)(&ac->outputParams[0]))) > 0)) {
                        ac->result |= 2;
                        ac->result &= ~1;
                        ac->mAcState = 1001;
                    }
                }
                break;

            case 1001:
                ac->mAcState = 1002;

            case 1002:
                return 0;

            case 1003:
                p->endTimer = 60;
                ac->mAcState = 1004;
                break;

            case 1004:
                p->endTimer--;
                if (p->endTimer <= 0) {
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

    }
    while (allDone);
    return 1;
}

s32 battleAcResult_ButtonDown(BattleWork* work) {
    return work->actionCommands.result;
}

void battleAcDisp_ButtonDown(CameraId cameraId, void* param) {
    BattleWork* work = (BattleWork*)param;
    BattleActions* ac = &work->actionCommands;
    BattleACDispParams* disp;
    f32 x, y;
    disp = &ac->disp;
    switch (ac->mAcState) {
        case 99:
        case 0:
            if (disp->slideTimer > 0) {
                disp->slideTimer--;
            }
            x = intplGetValue(EASING_QUADRATIC_OUT, -300.0f, 0.0f, 20 - disp->slideTimer, 20);
            y = 0.0f;
            break;
        default:
            x = y = 0.0f;
            break;
    }
    actionCommandDisp(0, x, y);
}

void battleAcDelete_ButtonDown(BattleWork* work) {
    work->actionCommands.mAcState = 1003;
}

static void actionCommandDisp(s32 unused, f32 x, f32 y) {
    BattleWork *work = _battleWorkPointer;
    BDWork *p = (BDWork*)work->actionCommands.work;
    s32 k;
    s32 end = ((work->actionCommands.params[2]) * 3) + 2;
    int size;
    s32 i;
    s32 count;
    s32 off;
    s32 j;
    f32 bx = (-200.0f) + x;
    f32 by;
    f32 lx = bx - 40.0f;
    Vec pos = {0.0f, 0.0f, -2.0f};
    by = 70.0f + y;
    pos.x = lx;
    pos.y = by;
    iconDispGx(&pos, 16, ICON_08F, 1.0f);
    lx += 16.0f;
    for (k = 1; k < end; k++) {
        Vec pos2 = {0.0f, 0.0f, -2.0f};
        pos2.x = lx;
        pos2.y = by;
        size = 16;
        iconDispGx(&pos2, size, ICON_08E, 1.0f);
        lx += 16.0f;
    }

    {
        Vec pos3 = {0.0f, 0.0f, -2.0f};
        pos3.x = lx;
        pos3.y = by;
        iconDispGx(&pos3, 16, ICON_090, 1.0f);
    }
    if ((work->actionCommands.acFlags) & AC_BUTTON_DOWN_FLAG_REVEAL_ONE_BY_ONE) {
        for (count = 0, j = 0; count < (work->actionCommands.params[2]); count++, j += 4) {
            if (!p->pressed[j >> 2]) {
                break;
            }
        }

        if (count < (work->actionCommands.params[2])) {
            count++;
        }
    } else {
        count = work->actionCommands.params[2];
    }
    for (i = 0, off = 0, j = 0; i < count; i++, off += 0x30, j += 4) {
        s32 a;
        s32 b;
        if (p->button[j >> 2] == 0U) {
            continue;
        }
        switch (p->button[(j >> 1) >> 1]) {
            case PAD_BUTTON_A:
                a = ICON_BUTTON_A_PRESSED;
                b = ICON_BUTTON_A;
                break;

            case PAD_BUTTON_B:
                a = ICON_BUTTON_B_PRESSED;
                b = ICON_BUTTON_B;
                break;

            case PAD_BUTTON_X:
                a = ICON_BUTTON_X_PRESSED;
                b = ICON_BUTTON_X;
                break;

            case PAD_BUTTON_Y:
                a = ICON_BUTTON_Y_PRESSED;
                b = ICON_BUTTON_Y;
                break;

            case PAD_TRIGGER_L:
                a = ICON_TRIGGER_L_PRESSED;
                b = ICON_TRIGGER_L;
                break;

            case PAD_TRIGGER_R:
                a = ICON_TRIGGER_R_PRESSED;
                b = ICON_TRIGGER_R;
                break;

            case PAD_TRIGGER_Z:
                a = ICON_TRIGGER_Z_PRESSED;
                b = ICON_TRIGGER_Z;
                break;

            case BATTLE_PAD_STICK_LEFT:

            default:
                a = ICON_STICK_LEFT;
                b = ICON_STICK_NEUTRAL;
                break;

        }

        if (p->pressed[j >> 2]) {
            Vec pos4 = {0.0f, 0.0f, 0.0f};
            pos4.x = bx + ((f32) off);
            pos4.y = by;
            ((void (*)(Vec *, s32, s32, f32)) iconDispGx)(&pos4, size, a, 1.0f);
        } else {
            Vec pos5 = {0.0f, 0.0f, 0.0f};
            pos5.x = bx + ((f32) off);
            pos5.y = by;
            ((void (*)(Vec *, s32, s32, f32)) iconDispGx)(&pos5, size, b, 1.0f);
        }
    }

}


