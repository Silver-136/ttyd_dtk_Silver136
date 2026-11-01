// Title sequence: the title screen and its "press start" prompt.

#include "version.h"
#include "seq/seq_title.h"
#include "driver/animdrv.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/fadedrv.h"
#include "mario/mario_pouch.h"
#include "mario/mariost.h"
#include "manager/fontmgr.h"
#include "pmario_sound.h"
#include "sound.h"
#include "sdk/texPalette.h"
#include "system.h"
#include "win/win_main.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <string.h>
#include "eff/eff_magic1_n64.h"
#include <dolphin/pad.h>
#include "sfx_name_word.h"
#include "seq/seq_e3.h"


#if VERSION >= VERSION_US
#define PSTATIC
#else
#define PSTATIC static
#endif

void FontDrawString(f32 x, f32 y, const char* msg);

TitleWork work2;
/**
 * Title screen state (logo animation, the press-start prompt, the debug flag);
 * wp2 is the pointer the code uses.
 */
TitleWork* wp2 = &work2;

#if VERSION >= VERSION_US
#define TITLE_FRAMES_PER_SEC (gp->framerate)
#else
#define TITLE_FRAMES_PER_SEC 60
#endif

/** Resting height of the title logo; the demos show it higher. */
#if VERSION <= VERSION_DEMO_US
#define TITLE_TOP_Y 130.0f
#else
#define TITLE_TOP_Y 120.0f
#endif

// local prototypes
void titleInit(SequenceWork* work);
void titleMain(SequenceWork* work);
void disp1(CameraId cameraId, void* param);
void disp2(CameraId cameraId, void* param);
static void titleLoadDemo(void);

/** Stub of the debug battle selector's message file hook: always NULL, so a debug battle loads gon_11's messages. */
const char* DbgBtlSel_GetMsgDataPtr(void) {
    return NULL;
}

#if VERSION > VERSION_DEMO_US
/** Later versions: titleInit does the setup, then the same flags are cleared. */
void seq_titleInit(SequenceWork* work) {
    titleInit(work);
    gp->flags &= ~GW_FLAG_NO_ENTRANCE;
    gp->flags &= ~0x40;
    gp->flags &= ~GW_FLAG_NO_SAVE_BLOCK;
    gp->flags &= ~GW_FLAG_E3;
}
#endif

/** Leaves the title sequence: frees mariost.tpl and fades out all sound unless going on to the file select. */
#if VERSION <= VERSION_DEMO_US
/** Later versions: titleInit does the setup, then the same flags are cleared. */
void seq_titleInit(SequenceWork* work) {
    memset(wp2, 0, sizeof(TitleWork));
    wp2->texture = fileAllocf(4, "%s/mariost.tpl", getMarioStDvdRoot());
    wp2->topPosX = 0.0f;
    wp2->topPosY = 1000.0f;
    wp2->bottomPosX = 0.0f;
    wp2->bottomPosY = -1000.0f;
    wp2->fadeAlpha = 0.0f;
    if (seqGetPrevSeq() != SEQ_LOAD && seqGetPrevSeq() != SEQ_MAP_CHANGE && seqGetPrevSeq() == SEQ_GAME_OVER) {
        psndStopAllFadeOut();
    }
    if (seqGetPrevSeq() != SEQ_LOAD) {
        if (seqGetPrevSeq() == SEQ_GAME_OVER) {
            psndBGMOn(PSND_FADE_OUT, "BGM_DIGEST1");
            if ((u32)psbgm[0].streamId != 0xFFFFFFFFU) {
                sound.streams[psbgm[0].streamId].flags |= 0x8000;
            }
        } else {
            psndBGMOn(PSND_FADE_OUT, "BGM_TITLE1");
            if ((u32)psbgm[0].streamId != 0xFFFFFFFFU) {
                sound.streams[psbgm[0].streamId].flags |= 0x8000;
            }
        }
    }
    camGetPtr(CAMERA_2D)->flags &= ~CAMERA_FLAG_LETTER_BOX;
    camGetPtr(CAMERA_2D)->flags |= CAMERA_FLAG_LETTER_BOX_INSTANT;
    gp->flags &= ~GW_FLAG_NO_ENTRANCE;
    gp->flags &= ~0x40;
    gp->flags &= ~GW_FLAG_NO_SAVE_BLOCK;
    gp->flags &= ~GW_FLAG_E3;
}
#endif
/** Leaves the title sequence: frees mariost.tpl and fades out all sound unless going on to the file select. */
void seq_titleExit(SequenceWork* work) {
    fileFree(wp2->texture);
    if (seqGetNextSeq() != SEQ_LOAD) {
        psndStopAllFadeOut();
    }
}

/** Per-frame title sequence; all the work is in titleMain. */
void seq_titleMain(SequenceWork* work) {
    titleMain(work);
}

// Descriptive name (not in the symbol maps).
#if VERSION > VERSION_DEMO_US
s32 getDebugMode(void) {
    return wp2->debug;
}

/**
 * Later versions' title setup (called by seq_titleInit):
 * loads mariost.tpl, parks the logo off screen,
 * stops leftover field sound (BGM fades over 3 s after a map change), and starts the title BGM,
 * or the digest theme after a game over, held paused until the curtain opens.
 */
void titleInit(SequenceWork* work) {
    memset(wp2, 0, sizeof(TitleWork));
    wp2->texture = fileAllocf(FILE_TYPE_TPL, "%s/mariost.tpl", getMarioStDvdRoot());
    wp2->topPosX = 0.0f;
    wp2->topPosY = 1000.0f;
    wp2->bottomPosX = 0.0f;
    wp2->bottomPosY = -1000.0f;
    wp2->fadeAlpha = 0.0f;
    wp2->debug = 0;
    if (seqGetPrevSeq() == SEQ_LOAD) {
        psndSFXAllOff();
    } else if (seqGetPrevSeq() != SEQ_LOGO) {
        if (seqGetPrevSeq() == SEQ_MAP_CHANGE) {
            psndBGMOff_f_d(PSND_FADEOUT, 3000, 0);
            psndBGMOff_f_d(PSND_FADEOUT | PSND_SLOT(1), 3000, 0);
            psndSFXAllOff();
        } else if (seqGetPrevSeq() == SEQ_GAME_OVER) {
            psndStopAllFadeOut();
        }
    }
    if (seqGetPrevSeq() != SEQ_LOAD) {
        if (seqGetPrevSeq() == SEQ_GAME_OVER) {
            if (OSGetSoundMode() == OS_SOUND_MODE_MONO) {
                SoundSetOutputMode(SND_OUTPUTMODE_MONO);
            } else {
                SoundSetOutputMode(SND_OUTPUTMODE_STEREO);
            }
            psndBGMOn(PSND_FADEOUT, "BGM_DIGEST1");
            if ((u32)psbgm[0].streamId != 0xFFFFFFFFU) {
                sound.streams[psbgm[0].streamId].flags |= SOUND_STREAM_FLAG_PAUSE;
            }
        } else {
            psndBGMOn(PSND_FADEOUT, "BGM_TITLE1");
            if ((u32)psbgm[0].streamId != 0xFFFFFFFFU) {
                sound.streams[psbgm[0].streamId].flags |= SOUND_STREAM_FLAG_PAUSE;
            }
        }
    }
    camGetPtr(CAMERA_2D)->flags &= ~CAMERA_FLAG_LETTER_BOX;
    camGetPtr(CAMERA_2D)->flags |= CAMERA_FLAG_LETTER_BOX_INSTANT;
}
#endif


/**
 * Title screen state machine: waits for the BGM (and curtain animations), opens the curtain,
 * slides the logo halves in, then waits for A or Start (to the file select via SEQ_LOAD)
 * or about 19 s of idling (to the dmo_00 opening demo).
 * Queues the logo draws each frame; later versions also track the debug-mode button code.
 */
#if VERSION <= VERSION_DEMO_US
static void titleMain(SequenceWork* work) {
    switch (wp2->state) {
        case 0:
        if (psndBGMStartCheck(0) != 0U) {
#else
void titleMain(SequenceWork* work) {
    switch (wp2->state) {
        case 0:
            if (psndBGMStartCheck(0) != 0U
                && animGroupBaseAsync("OFF_d_maku_left_right", 2, 0)
                && animGroupBaseAsync("OFF_d_maku_center", 2, 0)) {
#endif
                if (seqGetPrevSeq() == SEQ_LOAD) {
                    fadeEntry(FADE_CURTAIN_SIDES_A2_TOP, 0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
                    fadeEntry(FADE_CURTAIN_CENTER_Z2_3D, 0, (GXColor){0x00, 0x00, 0x00, 0xFF});
#if VERSION > VERSION_DEMO_US
                    wp2->topPosY = TITLE_TOP_Y;
                    wp2->bottomPosY = -170.0f;
#endif
                    wp2->timer = 0;
                } else if (seqGetPrevSeq() == SEQ_GAME_OVER) {
                    fadeEntry(FADE_OUT_WHITE_3D, 0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
                    fadeEntry(FADE_CURTAIN_CENTER_A1_3D, 0, (GXColor){0x00, 0x00, 0x00, 0xFF});
                    wp2->timer = 78;
                } else {
                    fadeEntry(FADE_OUT_WHITE_3D, 0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
                    fadeEntry(FADE_CURTAIN_ENTER_3D, 0, (GXColor){0x00, 0x00, 0x00, 0xFF});
                    fadeSetNarrowFast();
#if VERSION <= VERSION_DEMO_US
                    wp2->timer = 38;
#else
                    wp2->timer = 23;
#endif
                }
                wp2->state++;
            }
            break;
        case 1:
            wp2->timer--;
            if (wp2->timer == 0) {
                if ((u32)psbgm[0].streamId != 0xFFFFFFFFU) {
                    sound.streams[psbgm[0].streamId].flags &= ~SOUND_STREAM_FLAG_PAUSE;
                }
            }
            if (fadeIsFinish()) {
                if (seqGetPrevSeq() != SEQ_LOAD) {
                    fadeEntry(FADE_IN_WHITE_3D, 500, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
                    wp2->state++;
                } else {
                    wp2->topPosY = TITLE_TOP_Y;
                    wp2->bottomPosY = -170.0f;
#if VERSION > VERSION_DEMO_US
#if VERSION >= VERSION_US
                    wp2->counter = 0;
#endif
                    wp2->frame = 0;
#endif
                    wp2->state = 3;
                }
            }
            break;
        case 2:
            wp2->topPosX = 0.0f;
            wp2->topPosY = intplGetValue(EASING_BOUNCE_SHRINKING, 300.0f, TITLE_TOP_Y, wp2->counter, TITLE_FRAMES_PER_SEC);
            wp2->timer--;
            if (wp2->timer == 0) {
                if ((u32)psbgm[0].streamId != 0xFFFFFFFFU) {
                    sound.streams[psbgm[0].streamId].flags &= ~SOUND_STREAM_FLAG_PAUSE;
                }
                if (seqGetPrevSeq() == SEQ_GAME_OVER) {
                    psndSFXOn(SFX_NAME_WORD(PSND_SFX_BTL_CLAUD_ATTACK2));
                }
            }
            wp2->bottomPosX = 0.0f;
#if VERSION <= VERSION_DEMO_US
            wp2->bottomPosY = intplGetValue(EASING_SINE_IN_OUT, -300.0f, -170.0f, wp2->counter, VERSION_PICK(60, gp->framerate));
            wp2->counter++;
            if (wp2->counter > VERSION_PICK(60, gp->framerate)) {
                titleLoadDemo();
#else
            wp2->bottomPosY = intplGetValue(EASING_SINE_IN_OUT, -300.0f, -170.0f, wp2->counter, TITLE_FRAMES_PER_SEC);
            wp2->counter++;
            if (wp2->counter > TITLE_FRAMES_PER_SEC) {
                titleLoadDemo();
#if VERSION == VERSION_US
                wp2->counter = 0;
#endif
                wp2->frame = 0;
#endif
                wp2->state++;
            }
            break;
        case 3:
#if VERSION <= VERSION_DEMO_US
            wp2->fadeAlpha += 0.1f;
                    if (wp2->fadeAlpha > 1.0f) {
                        wp2->fadeAlpha = 1.0f;
                    }
                    if (keyGetButtonTrg(0) & (PAD_BUTTON_A | PAD_BUTTON_START)) {
                        psndSFXOn((const char*)0x12);
                        wp2->state = 10;
                    } else if (++wp2->idle > VERSION_PICK(60, gp->framerate) * 20 - VERSION_PICK(60, gp->framerate)) {
                        wp2->state = 20;
                    }
#else
#if VERSION >= VERSION_US
            wp2->counter++;
            if (wp2->counter > TITLE_FRAMES_PER_SEC) {
#endif
            wp2->fadeAlpha += 0.1f;
            if (wp2->fadeAlpha > 1.0f) {
                wp2->fadeAlpha = 1.0f;
            }
            if (keyGetButtonTrg(0) & (PAD_BUTTON_A | PAD_BUTTON_START)) {
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_PRESS_START1));
                wp2->state = 10;
            } else if (++wp2->idle > TITLE_FRAMES_PER_SEC * 20 - TITLE_FRAMES_PER_SEC) {
                wp2->state = 20;
            }
#if VERSION >= VERSION_US
            }
#endif
#endif
            break;
        case 10:
            fadeEntry(FADE_CURTAIN_SIDES_A1_TOP, 0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
            wp2->state++;
        case 11:
            if (fadeIsFinish()) {
                fadeEntry(FADE_CURTAIN_SIDES_Z1_3D, 0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
                wp2->state++;
            }
            break;
        case 12:
            if (fadeIsFinish()) {
                fadeReset(4);
                seqSetSeq(SEQ_LOAD, NULL, NULL);
            }
            break;
        case 20:
            fadeEntry(FADE_CURTAIN_LEAVE_3D, 0, (GXColor){0x00, 0x00, 0x00, 0xFF});
#if VERSION <= VERSION_DEMO_US
            fadeEntry(FADE_OUT_WHITE, 1000, (GXColor){0x00, 0x00, 0x00, 0xFF});
#else
            fadeEntry(FADE_OUT_BLACK, 1000, (GXColor){0x00, 0x00, 0x00, 0xFF});
#endif
            psndBGMOff_f_d(PSND_FADEOUT, 1000, 0);
            wp2->counter = 0;
            wp2->state++;
            break;
        case 21:
            wp2->topPosY = intplGetValue(EASING_SINE_IN_OUT, TITLE_TOP_Y, 400.0f, wp2->counter, TITLE_FRAMES_PER_SEC);
            wp2->bottomPosY = intplGetValue(EASING_SINE_IN_OUT, -170.0f, -400.0f, wp2->counter, TITLE_FRAMES_PER_SEC);
            wp2->counter++;
            if (wp2->counter > TITLE_FRAMES_PER_SEC) {
                wp2->counter = TITLE_FRAMES_PER_SEC;
            }
            wp2->fadeAlpha -= 0.1f;
            if (wp2->fadeAlpha < 0.0f) {
                wp2->fadeAlpha = 0.0f;
            }
            if (fadeIsFinish()) {
                fadeReset(1);
#if VERSION <= VERSION_DEMO_US
                gp->areaFadeInType = FADE_OUT_WHITE;
#else
                if (pouchGetHP() <= 0) {
                    pouchSetHP(1);
                }
                gp->areaFadeInType = FADE_OUT_BLACK;
#endif
                gp->areaFadeInDuration = 0;
                seqSetSeq(SEQ_MAP_CHANGE, "dmo_00", NULL);
            }
            break;
    }
    if (wp2->state != 12 && work->state < 14) {
        dispEntry(CAMERA_3D, 0.0f, 0, disp1, NULL);
        dispEntry(CAMERA_3D_EFFECTB, 0.0f, 0, disp2, NULL);
#if VERSION > VERSION_DEMO_US
        // a button sequence on the title screen turns on debug mode (getDebugMode)
        switch (wp2->debug) {
            case 0:
                if (keyGetButtonTrg(0) & ~PAD_BUTTON_X) {
                    wp2->debug = 0;
                } else if (keyGetButtonTrg(0) & PAD_BUTTON_X) {
                    wp2->debug++;
                }
                break;
            case 1:
                if (keyGetButtonTrg(0) & ~PAD_BUTTON_B) {
                    wp2->debug = 0;
                } else if (keyGetButtonTrg(0) & PAD_BUTTON_B) {
                    wp2->debug++;
                }
                break;
            case 2:
                if (keyGetButtonTrg(0) & ~PAD_TRIGGER_R) {
                    wp2->debug = 0;
                } else if (keyGetButtonTrg(0) & PAD_TRIGGER_R) {
                    wp2->debug++;
                }
                break;
            case 3:
                if (keyGetButtonTrg(0) & ~PAD_BUTTON_B) {
                    wp2->debug = 0;
                } else if (keyGetButtonTrg(0) & PAD_BUTTON_B) {
                    wp2->debug++;
                }
                break;
            case 4:
                if (keyGetButtonTrg(0) & ~PAD_BUTTON_Y) {
                    wp2->debug = 0;
                } else if (keyGetButtonTrg(0) & PAD_BUTTON_Y) {
                    wp2->debug++;
                }
                break;
            case 5:
                if (keyGetButtonTrg(0) & ~PAD_TRIGGER_L) {
                    wp2->debug = 0;
                } else if (keyGetButtonTrg(0) & PAD_TRIGGER_L) {
                    wp2->debug = -1;
                }
                break;
        }
#endif
    }
}

/** 3D-camera draw callback: draws the title background image from mariost.tpl through the 2D camera. */
void disp1(CameraId cameraId, void* param) {
    CameraEntry* cam = camGetPtr(CAMERA_2D);
    CameraEntry saved;
    f32 proj[GX_PROJECTION_SZ];
    f32 vp[GX_VIEWPORT_SZ];

    saved = *camGetPtr(cameraId);
    *camGetPtr(cameraId) = *cam;
    GXGetProjectionv(proj);
    GXGetViewportv(vp);
    GXSetProjection(cam->projection, cam->projType);
    GXSetViewport(cam->viewport[0], cam->viewport[1], cam->viewport[2],
                  cam->viewport[3], cam->viewport[4], cam->viewport[5]);
    winTexInit(*wp2->texture->data);
#if VERSION >= VERSION_US
    winTexSet(5, (Vec){0.0f, 0.0f, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
#else
    winTexSet(7, (Vec){0.0f, 0.0f, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
#endif
    *camGetPtr(cameraId) = saved;
    GXSetProjectionv(proj);
    GXSetViewport(vp[0], vp[1], vp[2], vp[3], vp[4], vp[5]);
}

/**
 * Effect-layer draw callback: draws the two logo halves at their animated positions (per-language images in US
 * and PAL) and the pulsing "press start" prompt.
 * Later versions show the build date and time while Z is held in debug mode.
 */
#if VERSION <= VERSION_DEMO_US
static void disp2(CameraId cameraId, void* param) {
#else
void disp2(CameraId cameraId, void* param) {
#endif
    CameraEntry* cam = camGetPtr(CAMERA_2D);
    CameraEntry saved;
    f32 proj[GX_PROJECTION_SZ];
    f32 vp[GX_VIEWPORT_SZ];
    s32 alpha;
    f32 s;
#if VERSION >= VERSION_US
#if VERSION == VERSION_EU
    s32 tbl[6] = {19, 19, 20, 21, 22, 23};
#else
    s32 tbl[6] = {21, 21, 22, 23, 24, 25};
#endif
#endif

    saved = *camGetPtr(cameraId);
    *camGetPtr(cameraId) = *cam;
    GXGetProjectionv(proj);
    GXGetViewportv(vp);
    GXSetProjection(cam->projection, cam->projType);
    GXSetViewport(cam->viewport[0], cam->viewport[1], cam->viewport[2],
                  cam->viewport[3], cam->viewport[4], cam->viewport[5]);
    winTexInit(*wp2->texture->data);
#if VERSION <= VERSION_DEMO_US
    winTexSet(5, (Vec){wp2->topPosX, wp2->topPosY, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
    winTexSet(6, (Vec){wp2->bottomPosX, wp2->bottomPosY, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
    s = (f32)sin((f32)gp->retraceCount / 30.0f);
#else
#if VERSION >= VERSION_US
    winTexSetNearest(tbl[gp->language], (Vec){wp2->topPosX, wp2->topPosY - 20.0f, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
#if VERSION == VERSION_EU
    winTexSet(gp->language + 7, (Vec){wp2->bottomPosX, wp2->bottomPosY, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
#else
    winTexSet(gp->language + 9, (Vec){wp2->bottomPosX, wp2->bottomPosY, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
#endif
#else
    winTexSetNearest(5, (Vec){wp2->topPosX, wp2->topPosY, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
    winTexSet(6, (Vec){wp2->bottomPosX, wp2->bottomPosY, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
#endif
    wp2->frame++;
    s = (f32)sin(6.2831855f * (f32)wp2->frame / 120.0f);
#endif
    alpha = (s32)(wp2->fadeAlpha * (459.0f * s));
    if (alpha < 0) {
        alpha = 0;
    }
    if (alpha > 255) {
        alpha = 255;
    }
#if VERSION <= VERSION_DEMO_US
    pressStartGX(alpha, 0.0f, -110.0f);
#else
#if VERSION >= VERSION_US
    pressStartGX(alpha, 0.0f, -122.0f);
#else
    pressStartGX(alpha, 0.0f, -110.0f);
#endif
    if (wp2->debug < 0 && (keyGetButton(0) & PAD_TRIGGER_Z)) {
        FontDrawStart();
        FontDrawEdge();
        FontDrawColor((GXColor){0xFA, 0xFA, 0xFA, 0xFF});
        FontDrawScale(0.8f);
        FontDrawString(60.0f, 220.0f, build_date_ptr);
        FontDrawString(200.0f, 220.0f, build_time_ptr);
    }
#endif
    *camGetPtr(cameraId) = saved;
    GXSetProjectionv(proj);
    GXSetViewport(vp[0], vp[1], vp[2], vp[3], vp[4], vp[5]);
}

PSTATIC void pressStartGX(s32 alpha, f32 x, f32 y) {
    CameraEntry* camera = camGetCurPtr();
    GXTexObj startText, gradient;
    Mtx mtx, scaleMtx;

#if VERSION >= VERSION_US
#if VERSION == VERSION_EU
    TEXGetGXTexObjFromPalette(*wp2->texture->data, &startText, gp->language + 13);
#else
    TEXGetGXTexObjFromPalette(*wp2->texture->data, &startText, gp->language + 15);
#endif
    GXLoadTexObj(&startText, GX_TEXMAP0);
    TEXGetGXTexObjFromPalette(*wp2->texture->data, &gradient, 6);
#else
    TEXGetGXTexObjFromPalette(*wp2->texture->data, &startText, 8);
    GXLoadTexObj(&startText, GX_TEXMAP0);
    TEXGetGXTexObjFromPalette(*wp2->texture->data, &gradient, 9);
#endif
    GXLoadTexObj(&gradient, GX_TEXMAP1);
    GXSetNumTevStages(2);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_CPREV, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_APREV, GX_CA_A0, GX_CA_ZERO);
    GXSetTevColor(GX_TEVREG0, (GXColor){0x00, 0x00, 0x00, alpha});
    GXSetNumChans(0);
    GXSetNumTexGens(2);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
    GXSetTexCoordGen(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0);
    MTXScale(scaleMtx, 1.0f, 1.0f, 1.0f);
#if VERSION <= VERSION_DEMO_US
    MTXTrans(mtx, (f32)gp->retraceCount / -200.0f, 0.0f, 0.0f);
#else
    MTXTrans(mtx, (f32)wp2->frame / -200.0f, 0.0f, 0.0f);
#endif
    MTXConcat(scaleMtx, mtx, mtx);
    GXLoadTexMtxImm(mtx, GX_TEXMTX0, GX_MTX2x4);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetZCompLoc(GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetZMode(GX_DISABLE, GX_LEQUAL, GX_DISABLE);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    MTXTrans(mtx, x, y, 0.0f);
    MTXScale(scaleMtx, (f32)GXGetTexObjWidth(&startText), (f32)GXGetTexObjHeight(&startText), 1.0f);
    MTXConcat(mtx, scaleMtx, mtx);
    MTXConcat(camera->view, mtx, mtx);
    GXLoadPosMtxImm(mtx, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(-0.5f, -0.5f, 0.0f);
    GXTexCoord2f32(0.0f, 1.0f);
    GXPosition3f32(0.5f, -0.5f, 0.0f);
    GXTexCoord2f32(1.0f, 1.0f);
    GXPosition3f32(0.5f, 0.5f, 0.0f);
    GXTexCoord2f32(1.0f, 0.0f);
    GXPosition3f32(-0.5f, 0.5f, 0.0f);
    GXTexCoord2f32(0.0f, 0.0f);
    GXSetTevSwapModeTable(GX_TEV_SWAP1, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
}

/**
 * Prefetches demo.tpl and the animations used by the dmo_00 opening demo (Peach, the treasure chest,
 * a Goomba and the other characters of the opening) once the logo has landed, so the idle demo starts quickly.
 */
static void titleLoadDemo(void) {
    fileAsyncf(FILE_TYPE_TPL, 0, "%s/demo.tpl", getMarioStDvdRoot());
    animGroupBaseAsync("c_peach", 0, 0);
    animGroupBaseAsync("c_nazo", 0, 0);
    animGroupBaseAsync("MOBJ_GoldenTreasureBox", 0, 0);
#if VERSION > VERSION_DEMO_US
    animGroupBaseAsync("MOBJ_BigTreasureBox", 0, 0);
#endif
    animGroupBaseAsync("c_roten", 0, 0);
    animGroupBaseAsync("c_kuribo_n", 0, 0);
    animGroupBaseAsync("c_tyusan", 0, 0);
}
