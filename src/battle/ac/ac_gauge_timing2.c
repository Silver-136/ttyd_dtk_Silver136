#include "version.h"
#include "battle/ac/ac_table.h"
#include "icon_ids.h"
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


typedef struct GT2Work {
    s32 frame;       // 0x0
    f32 cursor;      // 0x4
    s32 frame2;      // 0x8
    s32 totalFrames; // 0xC
    s32 hit;         // 0x10
    s32 button;      // 0x14
    f32 targets[8];  // 0x18
    s8 state[8];     // 0x38
    s8 flag[8];      // 0x40
    u32 sfx[8];      // 0x48
    u16 width;       // 0x68
    s8 miss;         // 0x6A
    u8 pad;
    s32 endTimer; // 0x6C
} GT2Work;

// iconDispGx called through a cast: its callers pass different argument types and no single prototype fits
// them all (docs/CLEANUP_BACKLOG.md, function-pointer casts).


static void actionCommandDisp(f32 x, f32 y);

/**
 * Per-frame driver of action command 20, used by Yoshi's Mini-Egg.
 * A bar fills over params[1] frames past params[2] evenly spaced targets;
 * each A press within 6 of an unresolved target scores it (outputParams[0] counts hits),
 * and pressing too early marks that target so it fails. Succeeds with at least one hit.
 * The releases also wait until every target is hit or missed and stop at the first target a press reaches;
 * the demos end on the time limit.
 */
s32 battleAcMain_GaugeTiming2(BattleWork *work) {
    BOOL pressed;
    const char *sfxNg;
    BOOL allDone;
    const char *sfxNames[2]; // FAKE: register-promoted array of the two SFX names; separate scalars swap regs
    BOOL found;
    s32 i3;
    s32 i4;
    GT2Work *gw = (GT2Work*)work->actionCommands.work;
    BOOL autoCommand = 0;
    BattleACDispParams* disp = &work->actionCommands.disp;
    s32 i;
    BOOL doAuto;
    const char *sfxOk;
    if (work->actionCommands.acUnit->badgesEquipped.AutoCommand) {
        autoCommand = 1;
    }
    switch (work->actionCommands.acState) {
        case 0:
#if VERSION <= VERSION_DEMO_US
            memset(disp, 0, 0x2C);
#else
            memset(disp, 0, sizeof(BattleACDispParams));
#endif
            disp->iconX = -300.0f;
            disp->iconY = 0.0f;
            disp->slideTimer = 20;
            disp->gaugeRatio = 0.0f;
            disp->color = (GXColor){255, 255, 255, 255};
            gw->frame = 0;
            gw->frame2 = 0;
            work->actionCommands.acState = 99;
            work->actionCommands.result = AC_RESULT_RUNNING;
            gw->totalFrames = work->actionCommands.params[1];
            gw->hit = 0;
            gw->cursor = 0.0f;
            work->actionCommands.outputParams[0] = 0;
            gw->width = work->actionCommands.params[2] * 60;
            for (i = 0; i < work->actionCommands.params[2]; i++) {
                gw->targets[i] = (f32) ((100 / work->actionCommands.params[2]) * (i + 1));
                gw->targets[i] -= 4.0f;
                gw->state[i] = 0;
                gw->flag[i] = 0;
                gw->sfx[i] = -1;
            }

            gw->button = PAD_BUTTON_A;
            work->actionCommands.outputParams[1] = 0;
            break;

        case 99:
            break;

        case 100:
            if ((work->actionCommands.setupFlags & AC_SETUP_FLAG_CONFUSED_AUTO) && (work->actionCommands.acUnit->tokenFlags & kConfusedAction)) {
                if (irand(100) >= 0) {
                    work->actionCommands.setupParam = 30;
                    gw->miss = 0;
                } else {
                    gw->miss = 1;
                    work->actionCommands.setupParam = 30;
                }
                work->actionCommands.acState = 1001;
                break;
            }
            work->actionCommands.acState = 1000;

        case 1000:
            gw->frame++;
            pressed = 0;
            if (BattlePadCheckTrigger(gw->button)) {
                pressed = 1;
            }
#if VERSION <= VERSION_DEMO_US
if (gw->frame > gw->totalFrames)
{
                if (((s32)work->actionCommands.outputParams[0]) > 0) {
#else
            allDone = 1;
            for (i = 0; i < work->actionCommands.params[2]; i++) {
                if (!gw->state[i]) {
                    allDone = 0;
                    break;
                }
            }

            if ((gw->frame > gw->totalFrames) && allDone) {
                if (work->actionCommands.outputParams[0] > 0) {
#endif
                    work->actionCommands.result = AC_RESULT_SUCCESS;
                    work->actionCommands.resultCount++;
                } else {
                    work->actionCommands.result = 0;
                }
                work->actionCommands.acState = 1002;
                break;
            }
            gw->frame2++;
            gw->cursor = (100.0f * ((f32) gw->frame2)) / ((f32) gw->totalFrames);
            {
                sfxNames[0] = "SFX_AC_COMMAND_NG1";
                for (i3 = 0; i3 < work->actionCommands.params[2]; i3++) {
                    if ((!gw->state[i3]) && (gw->cursor > (6.0f + gw->targets[i3]))) {
                        gw->state[i3] = -1;
                        psndSFXOn(sfxNames[0]);
                    }
                }
#if VERSION <= VERSION_DEMO_US
#endif
            }
            gw->hit = 0;
            {
                sfxNames[1] = "SFX_AC_PONE1";
                for (i = 0; i < work->actionCommands.params[2]; i++) {
                    if ((!gw->state[i]) && (__fabs(gw->cursor - gw->targets[i]) <= 6.0)) {
                        gw->hit = 1;
                        if (gw->sfx[i] == (-1)) {
                            gw->sfx[i] = psndSFXOn(sfxNames[1]);
                        }
                    }
                }
#if VERSION <= VERSION_DEMO_US
#endif
            }
            doAuto = 0;
            if (autoCommand) {
                BOOL hit2 = 0;
                s32 j;
                for (j = 0; j < work->actionCommands.params[2]; j++) {
                    if ((!gw->state[j]) && (__fabs(gw->cursor - gw->targets[j]) <= 6.0)) {
                        hit2 = 1;
                        break;
                    }
                }

                if (hit2) {
                    doAuto = 1;
                }
            }
            if (pressed || doAuto) {
                sfxNg = "SFX_AC_COMMAND_NG1";
                sfxOk = "SFX_AC_COMMAND_OK1";
#if VERSION > VERSION_DEMO_US
                found = 0;
#endif
                for (i4 = 0; i4 < work->actionCommands.params[2]; i4++) {
#if VERSION <= VERSION_DEMO_US
                    if (!gw->state[i4]) {
#else
                    if ((!gw->state[i4]) && (!found)) {
#endif
                        f64 d = __fabs(gw->cursor - gw->targets[i4]);
                        if (d <= 6.0) {
                            if ((!doAuto) && gw->flag[i4]) {
                                gw->state[i4] = -1;
                                psndSFXOn(sfxNg);
#if VERSION > VERSION_DEMO_US
                                found = 1;
#endif
                            } else {
                                gw->state[i4] = 1;
                                work->actionCommands.outputParams[0]++;
                                psndSFXOn(sfxOk);
#if VERSION > VERSION_DEMO_US
                                found = 1;
#endif
                            }
                        } else if (d < __fabs(gw->targets[1] - gw->targets[0])) {
                            gw->flag[i4] = 1;
                            if (i4 == (work->actionCommands.params[2] - 1)) {
                                gw->state[i4] = -1;
                                psndSFXOn(sfxNg);
#if VERSION > VERSION_DEMO_US
                                found = 1;
#endif
                            }
                        }
                    }
                }
#if VERSION <= VERSION_DEMO_US
#endif
            }
            break;

        case 1001:
            work->actionCommands.setupParam--;
            if (work->actionCommands.setupParam < 0) {
                if (gw->miss) {
                    work->actionCommands.result = AC_RESULT_SUCCESS;
                    work->actionCommands.resultCount++;
                    work->actionCommands.outputParams[0] = irand(work->actionCommands.params[2]) + 1;
                } else {
                    work->actionCommands.result = 0;
                    work->actionCommands.outputParams[0] = 0;
                }
                work->actionCommands.doneFlags |= AC_DONE_FLAG_FINISHED;
                return 0;
            }
            break;

        case 1002:
            work->actionCommands.acState = 1003;

        case 1003:
            work->actionCommands.outputParams[1] = 1;
            work->actionCommands.doneFlags |= AC_DONE_FLAG_FINISHED;
            gw->endTimer = 60;
            work->actionCommands.acState = 1004;

        case 1004:
            gw->endTimer--;
            if (gw->endTimer <= 0) {
                work->actionCommands.acState = 1005;
            }
            break;

        case 1005:
#if VERSION <= VERSION_DEMO_US
            work->actionCommands.maincb = (void*)0;
            work->actionCommands.dispcb = (void*)0;
            work->actionCommands.resultcb = (void*)0;
            work->actionCommands.deletecb = (void*)0;
#else
            work->actionCommands.maincb = NULL;
            work->actionCommands.dispcb = NULL;
            work->actionCommands.resultcb = NULL;
            work->actionCommands.deletecb = NULL;
#endif
            return 0;
#if VERSION <= VERSION_DEMO_US
#endif
    }

    return 1;
}

/** Returns the result word: 1 while running, 2 when at least one target was hit, 0 otherwise. */
s32 battleAcResult_GaugeTiming2(BattleWork* work) {
    return work->actionCommands.result;
}

/**
 * Display callback: slides the bar in from the left over 20 frames,
 * and once the command ends keeps it up for 40 frames before sliding it back out.
 */
void battleAcDisp_GaugeTiming2(CameraId cameraId, void* param) {
    BattleWork* work = param;
    BattleActions* ac = &work->actionCommands;
    BattleACDispParams* disp = &ac->disp;
    switch (ac->acState) {
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

/** Called by btlevtcmd_StopAC: unless the command already finished, starts the 60-frame end wait. */
void battleAcDelete_GaugeTiming2(BattleWork* work) {
    if (!(work->actionCommands.doneFlags & AC_DONE_FLAG_FINISHED)) {
        work->actionCommands.acState = 1003;
    }
}

/**
 * Draws the bar frame (params[2] x 60 wide), its red fill, the target stars (lit once reached),
 * an OK or miss mark on each resolved target, the A icon (pressed while a target is in reach)
 * and the count hits + params[3].
 */
static void actionCommandDisp(f32 x, f32 y) {
    BattleActions* ac = &_battleWorkPointer->actionCommands;
    BattleACDispParams* disp = &ac->disp;
    GT2Work* gw;
    CameraEntry* cam;
    f32 x0;
    f32 y0;
    f32 y1;
    f32 x1;
    f32 y2;
    f32 y3;
    f32 ratio;
    f32 t;
    s32 q;
    s32 r;
    s32 v;
    s32 i;
    u16 icon;
    u16 pressedIcon;
    s32 id;
    s32 cnt;
    Vec pos;
    Vec d0;
    Vec d1;

    camGetPtr(CAMERA_2D);
    gw = (GT2Work*)ac->work;
    cam = camGetPtr(CAMERA_2D);
    v = gw->width - 16;
    x0 = (-288.0f) + x;
    y0 = 49.0f + y;
    y1 = y0 - 8.0f;
    q = 0;
    r = 0;
    if (v > 0) {
        q = (v + 3) / 4;
        r = v - q * 4;
    }
    btlDispGXInit2DRasta();
    GXLoadPosMtxImm(cam->view, GX_PNMTX0);
    btlDispTexPlane(x0, y1, 0.0f, 1.0f, 1.0f, BTL_TEX(0x43), (GXColor){255, 255, 255, 255}, 0);
    t = 2.0f + (8.0f + x0);
    for (i = 0; i < q; i++) {
        if (i != q - 1) {
            btlDispTexPlane(t + (f32)(i * 4), y1, 0.0f, 1.0f, 1.0f, BTL_TEX(0x44), (GXColor){255, 255, 255, 255}, 0);
        } else {
            btlDispTexPlane((f32)r + (t + (f32)(i * 4)), y1, 0.0f, 1.0f, 1.0f, BTL_TEX(0x44), (GXColor){255, 255, 255, 255}, 0);
        }
    }
    btlDispTexPlane(16.0f + x0 + (f32)v, y1, 0.0f, -1.0f, 1.0f, BTL_TEX(0x43), (GXColor){255, 255, 255, 255}, 0);
    ratio = gw->cursor / 100.0f;
    if (ratio > 1.0f) {
        ratio = 1.0f;
    }
    disp->gaugeRatio = ratio;

    cam = camGetPtr(CAMERA_2D);
    y2 = y0 - 16.0f;
    x1 = x0 + (((f32)gw->width) * gw->cursor) / 100.0f;
    if (x1 >= x0) {
        btlDispGXInit2DRasta();
        GXLoadPosMtxImm(cam->view, GX_PNMTX0);
        btlDispGXQuads2DRasta(x0, y0, x1 - 1.0f, y2, 255, 0, 0, 255);
    }

    y3 = 23.0f + y;
    cnt = ac->params[2];
    for (i = 0; i < cnt; i++) {
        u16 iconId;
        f32 target = gw->targets[i];
        if (target <= gw->cursor || __fabs(gw->cursor - target) <= 6.0) {
            iconId = ICON_AC_ON;
        } else {
            iconId = ICON_AC_OFF;
        }
        pos = (Vec){0.0f, 0.0f, 0.0f};
        // FAKE: d0 and d1 are never read; removing them changes the object
        d0 = (Vec){0.0f, 0.0f, 0.0f};
        d1 = (Vec){0.0f, 0.0f, 0.0f};
        pos.x = (-288.0f + x) + ((f32)gw->width) * target / 100.0f;
        pos.y = y3;
        iconDispGx(&pos, ICON_FLAG_2D, iconId, 0.75f);
    }

    pressedIcon = BattleACGetButtonIcon(PAD_BUTTON_A, 1);
    icon = BattleACGetButtonIcon(PAD_BUTTON_A, 0);
    switch (ac->acState) {
        case 99:
        case 100:
        case 1002:
        case 1003:
        case 1004: {
                Vec p = {0.0f, 0.0f, 0.0f};
                // FAKE: e0 and e1 are never used; removing them changes the object
                Vec e0 = {0.0f, 0.0f, 0.0f};
                Vec e1 = {0.0f, 0.0f, 0.0f};
                p.x = 28.0f + (-225.0f + x);
                p.y = 55.0f + y;
                iconDispGx(&p, ICON_FLAG_2D, icon, 1.0f);
                break;
        }
        case 1000:
            if (gw->hit) {
                Vec p = {0.0f, 0.0f, 0.0f};
                // FAKE: e0 and e1 are never used; removing them changes the object
                Vec e0 = {0.0f, 0.0f, 0.0f};
                Vec e1 = {0.0f, 0.0f, 0.0f};
                p.x = 28.0f + (-225.0f + x);
                p.y = 55.0f + y;
                iconDispGx(&p, ICON_FLAG_2D, pressedIcon, 1.0f);
            } else {
                Vec p = {0.0f, 0.0f, 0.0f};
                p.x = 28.0f + (-225.0f + x);
                p.y = 55.0f + y;
                iconDispGx(&p, ICON_FLAG_2D, icon, 1.0f);
            }
            break;
    }

    y1 = 25.0f + y;
    {
        f32 y4 = y1 - 25.0f;
        for (i = 0; i < ac->params[2]; i++) {
            switch (gw->state[i]) {
                case 1:
                    id = BTL_TEX(0x3E);
                    break;
                case -1:
                    id = BTL_TEX(0x3F);
                    break;
                default:
                    id = BTL_TEX(0x45);
                    break;
            }
            if ((u16)id != BTL_TEX(0x45)) {
                btlDispTexPlane(-288.0f + x + (((f32)gw->width) * gw->targets[i]) / 100.0f, y4, 0.0f, 1.0f, 1.0f, (u16)id, (GXColor){255, 255, 255, 255}, 0);
            }
        }
    }

    {
        Mtx m3;
        Mtx m1;
        Mtx m2;
        s32 num = ac->outputParams[0] + ac->params[3];
        PSMTXTrans(m1, -200.0f + x + ((f32)gw->width) - 60.0f, y1, 0.0f);
        PSMTXScale(m2, 1.0f, 1.0f, 1.0f);
        PSMTXConcat(m1, m2, m3);
        iconNumberDispGx(m3, num, FALSE, (GXColor){255, 255, 255, 255});
    }
}
