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


#ifdef VERSION_G8ME01
#define PSTATIC
#else
#define PSTATIC static
#endif

void FontDrawString(f32 x, f32 y, const char* msg);

TitleWork work2;
TitleWork* wp2 = &work2;

//local prototypes
#ifdef VERSION_G8ME01
#define TITLE_FRAMES_PER_SEC (gp->framerate)
#else
#define TITLE_FRAMES_PER_SEC 60
#endif
void L_titleInit(SequenceWork* work);
void titleMain(SequenceWork* work);
void disp1(CameraId cameraId, void* param);
void disp2(CameraId cameraId, void* param);
static void titleLoadDemo(void);

const char* DbgBtlSel_GetMsgDataPtr(void) {
    return NULL;
}

void seq_titleInit(SequenceWork* work) {
    L_titleInit(work);
    gp->flags &= ~GW_FLAG_NO_ENTRANCE;
    gp->flags &= ~0x40;
    gp->flags &= ~GW_FLAG_NO_SAVE_BLOCK;
    gp->flags &= ~GW_FLAG_E3;
}

void seq_titleExit(SequenceWork* work) {
    fileFree(wp2->texture);
    if (seqGetNextSeq() != SEQ_LOAD) {
        psndStopAllFadeOut();
    }
}

void seq_titleMain(SequenceWork* work) {
    titleMain(work);
}

s32 N_getDebugMode(void) {
    return wp2->debug;
}

void L_titleInit(SequenceWork* work) {
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
            if (OSGetSoundMode() == 0) {
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


void titleMain(SequenceWork* work) {
    switch (wp2->state) {
        case 0:
            if (psndBGMStartCheck(0) != 0U
                && animGroupBaseAsync("OFF_d_maku_left_right", 2, 0)
                && animGroupBaseAsync("OFF_d_maku_center", 2, 0)) {
                if (seqGetPrevSeq() == SEQ_LOAD) {
                    fadeEntry(0x40, 0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
                    fadeEntry(0x1F, 0, (GXColor){0x00, 0x00, 0x00, 0xFF});
                    wp2->topPosY = 120.0f;
                    wp2->bottomPosY = -170.0f;
                    wp2->timer = 0;
                } else if (seqGetPrevSeq() == SEQ_GAME_OVER) {
                    fadeEntry(0x15, 0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
                    fadeEntry(0x1D, 0, (GXColor){0x00, 0x00, 0x00, 0xFF});
                    wp2->timer = 78;
                } else {
                    fadeEntry(0x15, 0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
                    fadeEntry(0x22, 0, (GXColor){0x00, 0x00, 0x00, 0xFF});
                    fadeSetNarrowFast();
                    wp2->timer = 23;
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
                    fadeEntry(0x14, 500, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
                    wp2->state++;
                } else {
                    wp2->topPosY = 120.0f;
                    wp2->bottomPosY = -170.0f;
#ifdef VERSION_G8ME01
                    wp2->counter = 0;
#endif
                    wp2->frame = 0;
                    wp2->state = 3;
                }
            }
            break;
        case 2:
            wp2->topPosX = 0.0f;
            wp2->topPosY = intplGetValue(EASING_BOUNCE_SHRINKING, 300.0f, 120.0f, wp2->counter, TITLE_FRAMES_PER_SEC);
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
            wp2->bottomPosY = intplGetValue(EASING_SINE_IN_OUT, -300.0f, -170.0f, wp2->counter, TITLE_FRAMES_PER_SEC);
            wp2->counter++;
            if (wp2->counter > TITLE_FRAMES_PER_SEC) {
                titleLoadDemo();
#ifdef VERSION_G8ME01
                wp2->counter = 0;
#endif
                wp2->frame = 0;
                wp2->state++;
            }
            break;
        case 3:
#ifdef VERSION_G8ME01
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
#ifdef VERSION_G8ME01
            }
#endif
            break;
        case 10:
            fadeEntry(0x3F, 0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
            wp2->state++;
        case 11:
            if (fadeIsFinish()) {
                fadeEntry(0x1A, 0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
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
            fadeEntry(0x21, 0, (GXColor){0x00, 0x00, 0x00, 0xFF});
            fadeEntry(FADE_OUT_BLACK, 1000, (GXColor){0x00, 0x00, 0x00, 0xFF});
            psndBGMOff_f_d(PSND_FADEOUT, 1000, 0);
            wp2->counter = 0;
            wp2->state++;
            break;
        case 21:
            wp2->topPosY = intplGetValue(EASING_SINE_IN_OUT, 120.0f, 400.0f, wp2->counter, TITLE_FRAMES_PER_SEC);
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
                if (pouchGetHP() <= 0) {
                    pouchSetHP(1);
                }
                gp->areaFadeInType = FADE_OUT_BLACK;
                gp->areaFadeInDuration = 0;
                seqSetSeq(SEQ_MAP_CHANGE, "dmo_00", NULL);
            }
            break;
    }
    if (wp2->state != 12 && work->state < 14) {
        dispEntry(CAMERA_3D, 0.0f, 0, disp1, NULL);
        dispEntry(CAMERA_3D_EFFECTB, 0.0f, 0, disp2, NULL);
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
    }
}

void disp1(CameraId cameraId, void* param) {
    CameraEntry* cam = camGetPtr(CAMERA_2D);
    CameraEntry saved;
    f32 proj[7];
    f32 vp[6];

    saved = *camGetPtr(cameraId);
    *camGetPtr(cameraId) = *cam;
    GXGetProjectionv(proj);
    GXGetViewportv(vp);
    GXSetProjection(cam->projection, cam->projType);
    GXSetViewport(cam->viewport[0], cam->viewport[1], cam->viewport[2],
                  cam->viewport[3], cam->viewport[4], cam->viewport[5]);
    winTexInit(*wp2->texture->data);
#ifdef VERSION_G8ME01
    winTexSet(5, (Vec){0.0f, 0.0f, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
#else
    winTexSet(7, (Vec){0.0f, 0.0f, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
#endif
    *camGetPtr(cameraId) = saved;
    GXSetProjectionv(proj);
    GXSetViewport(vp[0], vp[1], vp[2], vp[3], vp[4], vp[5]);
}

void disp2(CameraId cameraId, void* param) {
    CameraEntry* cam = camGetPtr(CAMERA_2D);
    CameraEntry saved;
    f32 proj[7];
    f32 vp[6];
    s32 alpha;
    f32 s;
#ifdef VERSION_G8ME01
    s32 tbl[6] = { 21, 21, 22, 23, 24, 25 };
#endif

    saved = *camGetPtr(cameraId);
    *camGetPtr(cameraId) = *cam;
    GXGetProjectionv(proj);
    GXGetViewportv(vp);
    GXSetProjection(cam->projection, cam->projType);
    GXSetViewport(cam->viewport[0], cam->viewport[1], cam->viewport[2],
                  cam->viewport[3], cam->viewport[4], cam->viewport[5]);
    winTexInit(*wp2->texture->data);
#ifdef VERSION_G8ME01
    winTexSetNearest(tbl[gp->language], (Vec){wp2->topPosX, wp2->topPosY - 20.0f, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
    winTexSet(gp->language + 9, (Vec){wp2->bottomPosX, wp2->bottomPosY, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
#else
    winTexSetNearest(5, (Vec){wp2->topPosX, wp2->topPosY, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
    winTexSet(6, (Vec){wp2->bottomPosX, wp2->bottomPosY, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
#endif
    wp2->frame++;
    s = (f32)sin(6.2831855f * (f32)wp2->frame / 120.0f);
    alpha = (s32)(wp2->fadeAlpha * (459.0f * s));
    if (alpha < 0) {
        alpha = 0;
    }
    if (alpha > 255) {
        alpha = 255;
    }
#ifdef VERSION_G8ME01
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
    *camGetPtr(cameraId) = saved;
    GXSetProjectionv(proj);
    GXSetViewport(vp[0], vp[1], vp[2], vp[3], vp[4], vp[5]);
}

PSTATIC void pressStartGX(s32 alpha, f32 x, f32 y) {
    CameraEntry* camera = camGetCurPtr();
    GXTexObj startText, gradient;
    Mtx v17, v16;

#ifdef VERSION_G8ME01
    TEXGetGXTexObjFromPalette(*wp2->texture->data, &startText, gp->language + 15);
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
    GXSetTevColor(GX_TEVREG0, (GXColor){ 0x00, 0x00, 0x00, alpha });
    GXSetNumChans(0);
    GXSetNumTexGens(2);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 60);
    GXSetTexCoordGen(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, 30);
    MTXScale(v16, 1.0f, 1.0f, 1.0f);
    MTXTrans(v17, (f32)wp2->frame / -200.0f, 0.0f, 0.0f);
    MTXConcat(v16, v17, v17);
    GXLoadTexMtxImm(v17, 30, GX_MTX2x4);
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
    MTXTrans(v17, x, y, 0.0f);
    MTXScale(v16, (f32)GXGetTexObjWidth(&startText), (f32)GXGetTexObjHeight(&startText), 1.0f);
    MTXConcat(v17, v16, v17);
    MTXConcat(camera->view, v17, v17);
    GXLoadPosMtxImm(v17, 0);
    GXSetCurrentMtx(0);
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

static void titleLoadDemo(void) {
    fileAsyncf(FILE_TYPE_TPL, 0, "%s/demo.tpl", getMarioStDvdRoot());
    animGroupBaseAsync("c_peach", 0, 0);
    animGroupBaseAsync("c_nazo", 0, 0);
    animGroupBaseAsync("MOBJ_GoldenTreasureBox", 0, 0);
    animGroupBaseAsync("MOBJ_BigTreasureBox", 0, 0);
    animGroupBaseAsync("c_roten", 0, 0);
    animGroupBaseAsync("c_kuribo_n", 0, 0);
    animGroupBaseAsync("c_tyusan", 0, 0);
}
