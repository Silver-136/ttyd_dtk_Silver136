#include "seq/seq_logo.h"
#include "libc_decls.h"
#include "driver/arcdrv.h"
#include "driver/effdrv.h"
#include "driver/envdrv.h"
#include "driver/fadedrv.h"
#include "driver/icondrv.h"
#include "driver/seqdrv.h"
#include "driver/windowdrv.h"
#include "mario/mario.h"
#include "mario/mariost.h"
#include "manager/filemgr.h"
#include "manager/fontmgr.h"
#include "pmario_sound.h"
#include "romfont.h"
#include "system.h"
#include <dolphin/pad.h>
#include <demo/DEMOPad.h>
#include <dolphin/vi.h>
#include <string.h>

#ifdef VERSION_G8ME01
static inline s32* logoStoryTime(void);
extern u32 seq_logo_zero_color;
#endif
//.sbss
static LogoWork work;

//.sdata
static LogoWork* wp = &work;

#define OSIsRestart() ((OSGetResetCode() & 0x80000000) ? TRUE : FALSE)

//local prototypes
void logoDisp(CameraId camId, void* param);
void dummy_draw(CameraId camId, void* param);
void progDisp(CameraId camId, void* param);


#ifndef VERSION_G8ME01
static s32 logoAsyncFirst(void);
#endif

void seq_logoInit(SequenceWork* work) {
    memset(wp, 0, sizeof(LogoWork));
#ifdef VERSION_G8ME01
    wp->page = -1;
#endif
    wp->texture = fileAllocf(FILE_TYPE_TPL, "%s/mariost.tpl", getMarioStDvdRoot());
    if (!OSIsRestart() || OSGetResetCode() & OS_RESET_SHUTDOWN) {
        if (VIGetDTVStatus()) {
            if (OSGetProgressiveMode() == 1) {
                wp->progressive = TRUE;
            } else {
                if (keyGetButton(0) & PAD_BUTTON_B) {
                    wp->progressive = TRUE;
                } else {
                    wp->progressive = FALSE;
                }
            }
        } else {
            if (OSGetProgressiveMode() == 1) {
                OSSetProgressiveMode(0);
            }
            wp->progressive = FALSE;
        }
    }
#ifdef VERSION_G8ME01
    if (!OSIsRestart() || OSGetResetCode() & OS_RESET_SHUTDOWN) {
        VIConfigure(&sRMObjHReso);
#else
    logoAsyncFirst();
    if (!OSIsRestart() || OSGetResetCode() & OS_RESET_SHUTDOWN) {
        VIConfigure(&sRMObjHReso);
        if (!wp && !wp && !wp) {
        }
#endif
        VIFlush();
        VIWaitForRetrace();
    } else {
        switch (OSGetResetCode() & 1) {
            case OS_RESET_RESTART:
                VIConfigure(&sRMObjHReso);
                VIFlush();
                VIWaitForRetrace();
                break;

            case OS_RESET_HOTRESET:
                VIConfigure(&sRMObjHReso_prog);
                VIFlush();
                VIWaitForRetrace();
                VIWaitForRetrace();
                gp->flags |= GW_FLAG_PROGRESSIVE;
                break;
        }
        gp->flags |= GW_FLAG_VIDEO_MODE_SET;
    }
}


void seq_logoExit(SequenceWork* work) {
    fontmgrTexSetup();
    windowTexSetup();
    fadeTexSetup();
    iconTexSetup();
    envTexSetup();
    effTexSetup();
    marioPoseInit();
    fileFree(wp->texture);
}

void seq_logoMain(SequenceWork* work) {
#ifdef VERSION_G8ME01
    static OSTime time;
#else
    static OSTime time; //time instance 1
    u32 size;
    u8* decode;
    FileEntry* first;
    s32 ret;
    int ok;
#endif

    switch (work->state) {
        case 0:
            fadeEntry(FADE_IN_BLACK, 500, (GXColor){0, 0, 0, 0xFF});
            DVDMgrSetupCallback(psndPushGroup);
            work->state++;
            break;

        case 1:
#ifdef VERSION_G8ME01
            work->state++;
            break;

        case 2:
            if (fadeIsFinish()) {
                work->state++;
            }
            break;

        case 3:
            if (!wp->progressive) {
                gp->flags |= GW_FLAG_VIDEO_MODE_SET;
                time = gp->renderTime;
                work->state++;
            }
            break;

        case 4:
            fadeEntry(FADE_IN_BLACK, 500, (GXColor){0, 0, 0, 0xFF});
            work->state++;
            wp->page = 0;
            break;

        case 5:
            if (OSTicksToMilliseconds(gp->renderTime - time) > 2000) {
                work->state++;
                wp->page = 1;
            }
            break;

        case 6:
            if (keyGetButtonTrg(0)) {
                work->state++;
            } else if (OSTicksToMilliseconds(gp->renderTime - time) > 59000) {
                work->state++;
            }
            break;

        case 7:
            fadeEntry(FADE_OUT_BLACK, 500, (GXColor){0, 0, 0, 0xFF});
            work->state++;
            break;

        case 8:
            if (fadeIsFinish()) {
                fadeEntry(FADE_IN_BLACK, 500, (GXColor){0, 0, 0, 0xFF});
                work->state++;
                wp->page = 2;
            }
            break;

        case 9:
            if (fadeIsFinish()) {
                time = gp->renderTime;
                work->state++;
            }
            break;

        case 10:
#else
            if (fadeIsFinish() && !wp->progressive) {
                work->state++;
                time = gp->renderTime;
                gp->flags |= GW_FLAG_VIDEO_MODE_SET;
            }
            break;

        case 2:
#endif
            if (OSTicksToMilliseconds(gp->renderTime - time) > 1000) {
                fadeEntry(FADE_OUT_BLACK, 500, (GXColor){0, 0, 0, 0xFF});
                work->state++;
            }
            break;

#ifdef VERSION_G8ME01
        case 11:
#else
        case 3:
#endif
            if (fadeIsFinish()) {
                fadeEntry(FADE_IN_BLACK, 500, (GXColor){0, 0, 0, 0xFF});
                work->state++;
#ifdef VERSION_G8ME01
                wp->page = 3;
#else
                wp->logoId = 1;
#endif
            }
            break;

#ifdef VERSION_G8ME01
        case 12:
#else
        case 4:
#endif
            if (fadeIsFinish()) {
                work->state++;
                time = gp->renderTime;
            }
            break;

#ifdef VERSION_G8ME01
        case 13:
#else
        case 5:
#endif
            if (OSTicksToMilliseconds(gp->renderTime - time) > 1000) {
                fadeEntry(FADE_OUT_BLACK, 500, (GXColor){0, 0, 0, 0xFF});
                work->state++;
            }
            break;

#ifdef VERSION_G8ME01
        case 14:
#else
        case 6:
#endif
            if (fadeIsFinish()) {
                fadeEntry(FADE_IN_BLACK, 500, (GXColor){0, 0, 0, 0xFF});
                work->state++;
#ifdef VERSION_G8ME01
                wp->page = 4;
#else
                wp->logoId = 2;
#endif
            }
            break;

#ifdef VERSION_G8ME01
        case 15:
#else
        case 7:
#endif
            if (fadeIsFinish()) {
                work->state++;
                time = gp->renderTime;
            }
            break;

#ifdef VERSION_G8ME01
        case 16:
#else
        case 8:
#endif
            if (OSTicksToMilliseconds(gp->renderTime - time) > 1000) {
                fadeEntry(FADE_OUT_BLACK, 500, (GXColor){0, 0, 0, 0xFF});
                wp->skipStory = FALSE;
                work->state++;
            }
            break;

#ifdef VERSION_G8ME01
        case 17:
#else
        case 9:
#endif
            if (keyGetButtonTrg(0) & (PAD_BUTTON_START | PAD_BUTTON_A)) {
                wp->skipStory = TRUE;
            }
            if ((u32)psndPushGroup() == 0U) {
                DVDMgrSetupCallback(NULL);
                if (fadeIsFinish()) {
                    if (!(gp->flags & GW_FLAG_PROGRESSIVE)) {
                        VIConfigure(&GXNtsc480IntDfMarioSt);
                        VIFlush();
                    } else {
                        VIConfigure(&GXNtsc480ProgMarioSt);
                        VIFlush();
                    }
                    VISetBlack(TRUE);
                    dispEntry(CAMERA_BACKGROUND, 0.0f, 0, dummy_draw, NULL);
                    work->state++;
                }
            }
            break;

#ifdef VERSION_G8ME01
        case 18:
            VISetBlack(FALSE);
            VIWaitForRetrace();
            if (!(gp->flags & GW_FLAG_E3)) {
                if (!wp->skipStory) {
#else
        case 10:
            VISetBlack(FALSE);
            VIWaitForRetrace();
            ret = logoAsyncFirst();
            if (ret == 0U) {
                ok = 0;
            } else if (ret == -1) {
                ok = 1;
            } else {
                ok = 1;
            }
            if (!ok) {
                break;
            }
            first = fileAllocf(FILE_TYPE_RAW, "%s/b/first", getMarioStDvdRoot());
            if (first) {
                decode = *first->data;
                size = *(u32*)decode;
                size = ((size << 16) & 0xFF0000) | (size & 0xFF00) | ((size >> 16) & 0xFF);
                LZ77Decode(*first->data, decode = __memAlloc(HEAP_DEFAULT, size));
                arcEntry(0, decode, size);
                fileFree(first);
            }
            if (!(gp->flags & GW_FLAG_E3)) {
                if (!wp->skipStory) { //play story demo
#endif
                    seqSetSeq(SEQ_MAP_CHANGE, "dmo_00", NULL);
                    return;
                }
#ifdef VERSION_G8ME01
                else {
#else
                else { //continue to title screen
#endif
                    fadeEntry(FADE_OUT_WHITE, 500, (GXColor){0, 0, 0, 0xFF});
                    work->state++;
                    return;
                }
            } else {
                seqSetSeq(SEQ_E3, NULL, NULL);
                return;
            }
            break;

#ifdef VERSION_G8ME01
        case 19:
#else
        case 11:
#endif
            if (fadeIsFinish()) {
                seqSetSeq(SEQ_TITLE, NULL, NULL);
            }
            break;
    }

#ifdef VERSION_G8ME01
    if (work->state >= 2 && work->state < 4) {
        dispEntry(CAMERA_2D, 0.0f, 0, progDisp, NULL);
    }
    if (work->state >= 4 && work->state < 18) {
        dispEntry(CAMERA_2D, 1.0f, 0, logoDisp, work);
#else
    if (work->state < 10) {
        dispEntry(CAMERA_2D, 1.0f, 0, logoDisp, NULL);
        dispEntry(CAMERA_2D, 0.0f, 0, progDisp, NULL);
#endif
    }
}


//param is unused
void logoDisp(CameraId camId, void* param) {
    GXRenderModeObj* rmode;
    f32 width, height, x, y;

    //stack variables
    Mtx44 ortho; //stack + 0xAC
    Mtx position; //stack + 0x7C
    f32 projection[GX_PROJECTION_SZ]; //stack + 0x60
    f32 viewport[GX_VIEWPORT_SZ]; //stack + 0x48
    GXTexObj texObj; //stack + 0x28
    u32 sciLeft, sciTop, sciWidth, sciHeight; //tack + 0x24

    if (wp->texture) {
        GXGetProjectionv(projection);
        GXGetViewportv(viewport);
        GXGetScissor(&sciLeft, &sciTop, &sciWidth, &sciHeight);
        if (OSGetProgressiveMode() != 1) {
            rmode = &sRMObjHReso;
        } else {
            rmode = &sRMObjHReso_prog;
        }
        GXSetPixelFmt(GX_PF_RGB8_Z24, GX_ZC_LINEAR);
        GXSetDispCopySrc(0, 0, rmode->fbWidth, rmode->efbHeight);
        GXSetDispCopyDst(rmode->fbWidth, rmode->efbHeight);
        GXSetDispCopyYScale((f32)rmode->xfbHeight / (f32)rmode->efbHeight);
        GXSetDispCopyGamma(GX_GM_1_0);
        GXSetScissorBoxOffset(0, 0);
        GXSetScissor(0, 0, 608, 448);
        GXSetViewport(0.0f, 0.0f, 608.0f, 448.0f, -1.0f, 1.0f);

#ifdef VERSION_G8ME01
        switch (wp->page) {
            case -1:
                break;

            case 0:
            case 1:
                TEXGetGXTexObjFromPalette(*wp->texture->data, &texObj, 7);
                GXLoadTexObj(&texObj, GX_TEXMAP0);
                GXSetNumChans(0);
                GXSetTevColor(GX_TEVREG0, (GXColor){0, 0, 0, 0xFF});
                GXSetTevColor(GX_TEVREG1, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
                GXSetNumTexGens(1);
                GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3C);
                GXSetNumTevStages(1);
                GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
                GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C0, GX_CC_C1, GX_CC_TEXC, GX_CC_ZERO);
                GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_A0, GX_CA_ZERO);
                GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
                GXSetZCompLoc(GX_TRUE);
                GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
                GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
                MTXOrtho(ortho, -23.0f, 448.0f, 0.0f, 608.0f, -100.0f, 100.0f);
                GXSetProjection(ortho, GX_ORTHOGRAPHIC);
                MTXIdentity(position);
                GXLoadPosMtxImm(position, GX_PNMTX0);
                GXSetCurrentMtx(GX_PNMTX0);
                GXSetCullMode(GX_CULL_NONE);
                GXClearVtxDesc();
                GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
                GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
                GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
                GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
                GXBegin(GX_QUADS, GX_VTXFMT0, 4);

                GXPosition3f32(0.0f, 0.0f, 0.0f);
                GXTexCoord2f32(0.0f, 0.0f);

                GXPosition3f32(608.0f, 0.0f, 0.0f);
                GXTexCoord2f32(1.0f, 0.0f);

                GXPosition3f32(608.0f, 448.0f, 0.0f);
                GXTexCoord2f32(1.0f, 1.0f);

                GXPosition3f32(0.0f, 448.0f, 0.0f);
                GXTexCoord2f32(0.0f, 1.0f);
                if (wp->page == 1) {
                    s32 alpha;
                    f32 c;

                    c = cos(1.5707964f * (f32)*logoStoryTime() / (f32)gp->framerate);
                    alpha = (s32)(255.0 * (1.0 - c) / 2.0);
                    *logoStoryTime() += 2;
                    if (alpha > 255) {
                        alpha = 255;
                    }
                    GXSetTevColor(GX_TEVREG0, (GXColor){0, 0, 0, alpha});
                    TEXGetGXTexObjFromPalette(*wp->texture->data, &texObj, 8);
                    GXLoadTexObj(&texObj, GX_TEXMAP0);
                    GXBegin(GX_QUADS, GX_VTXFMT0, 4);

                    GXPosition3f32(0.0f, 359.0f, 0.0f);
                    GXTexCoord2f32(0.0f, 0.0f);

                    GXPosition3f32(608.0f, 359.0f, 0.0f);
                    GXTexCoord2f32(1.0f, 0.0f);

                    GXPosition3f32(608.0f, 407.0f, 0.0f);
                    GXTexCoord2f32(1.0f, 1.0f);

                    GXPosition3f32(0.0f, 407.0f, 0.0f);
                    GXTexCoord2f32(0.0f, 1.0f);
                }
                break;


            case 2:
#else
        switch (wp->logoId) {
            case 0:
#endif
                TEXGetGXTexObjFromPalette(*wp->texture->data, &texObj, 0);
                GXLoadTexObj(&texObj, GX_TEXMAP0);
                GXSetNumChans(0);
                GXSetTevColor(GX_TEVREG0, (GXColor){0, 0, 0, 0xFF});
                if (gp->language == 0U) { //blue for JP
                    GXSetTevColor(GX_TEVREG1, (GXColor){0, 0x46, 0xFF, 0xFF});
                } else { //red for US
                    GXSetTevColor(GX_TEVREG1, (GXColor){0xDC, 0, 0, 0xFF});
                }
                GXSetNumTexGens(1);
                GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3C);
                GXSetNumTevStages(1);
                GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
                GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C0, GX_CC_C1, GX_CC_TEXC, GX_CC_ZERO);
                GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_TEXA, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
                GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
                GXSetZCompLoc(GX_TRUE);
                GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
                GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
                MTXOrtho(ortho, -23.0f, 448.0f, 0.0f, 608.0f, -100.0f, 100.0f);
                GXSetProjection(ortho, GX_ORTHOGRAPHIC);
                MTXIdentity(position);
                GXLoadPosMtxImm(position, GX_PNMTX0);
                GXSetCurrentMtx(GX_PNMTX0);
                GXSetCullMode(GX_CULL_NONE);
                GXClearVtxDesc();
                GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
                GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
                GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
                GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
                GXBegin(GX_QUADS, GX_VTXFMT0, 4);

                GXPosition3f32(117.0f, 154.0f, 0.0f);
                GXTexCoord2f32(0.0f, 0.0f);

                GXPosition3f32(493.0f, 154.0f, 0.0f);
                GXTexCoord2f32(1.0f, 0.0f);

                GXPosition3f32(493.0f, 258.0f, 0.0f);
                GXTexCoord2f32(1.0f, 1.0f);

                GXPosition3f32(117.0f, 258.0f, 0.0f);
                GXTexCoord2f32(0.0f, 1.0f);
                break;

#ifdef VERSION_G8ME01
            case 3:
#else
            case 1:
#endif
                TEXGetGXTexObjFromPalette(*wp->texture->data, &texObj, 1);
                GXLoadTexObj(&texObj, GX_TEXMAP0);
                GXSetNumChans(0);
                GXSetNumTexGens(1);
                GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3C);
                GXSetNumTevStages(1);
                GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
                GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
                GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
                GXSetZCompLoc(GX_TRUE);
                GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
                GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
                MTXOrtho(ortho, 0.0f, 448.0f, 0.0f, 608.0f, -100.0f, 100.0f);
                GXSetProjection(ortho, GX_ORTHOGRAPHIC);
                MTXIdentity(position);
                GXLoadPosMtxImm(position, GX_PNMTX0);
                GXSetCurrentMtx(GX_PNMTX0);
                GXSetCullMode(GX_CULL_NONE);
                GXClearVtxDesc();
                GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
                GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
                GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
                GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
                GXBegin(GX_QUADS, GX_VTXFMT0, 4);
                width = (f32)GXGetTexObjWidth(&texObj);
                height = (f32)GXGetTexObjHeight(&texObj);
                x = (608.0f - width) / 2.0f;
                y = (448.0f - height) / 2.0f - 20.0f;
                height = y + height;

                GXPosition3f32(x, y, 0.0f);
                GXTexCoord2f32(0.0f, 0.0f);

                GXPosition3f32(x + width, y, 0.0f);
                GXTexCoord2f32(1.0f, 0.0f);

                GXPosition3f32(x + width, height, 0.0f);
                GXTexCoord2f32(1.0f, 1.0f);

                GXPosition3f32(x, height, 0.0f);
                GXTexCoord2f32(0.0f, 1.0f);
                break;

#ifdef VERSION_G8ME01
            case 4:
#else
            case 2:
#endif
                TEXGetGXTexObjFromPalette(*wp->texture->data, &texObj, 2);
                GXLoadTexObj(&texObj, GX_TEXMAP0);
                GXSetNumChans(1u);
                GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE, GX_AF_NONE);
                GXSetChanMatColor(GX_COLOR0A0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
                GXSetNumTexGens(1);
                GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3C);
                GXSetNumTevStages(1);
                GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
                GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_RASC, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
                GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_TEXA, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
                GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
                GXSetZCompLoc(GX_TRUE);
                GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
                GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
                MTXOrtho(ortho, 0.0f, 448.0f, 0.0f, 608.0f, -100.0f, 100.0f);
                GXSetProjection(ortho, GX_ORTHOGRAPHIC);
                MTXIdentity(position);
                GXLoadPosMtxImm(position, GX_PNMTX0);
                GXSetCurrentMtx(GX_PNMTX0);
                GXSetCullMode(GX_CULL_NONE);
                GXClearVtxDesc();
                GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
                GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
                GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
                GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
                GXBegin(GX_QUADS, GX_VTXFMT0, 4);

                GXPosition3f32(189.0f, 150.0f, 0.0f);
                GXTexCoord2f32(0.0f, 0.0f);

                GXPosition3f32(421.0f, 150.0f, 0.0f);
                GXTexCoord2f32(1.0f, 0.0f);

                GXPosition3f32(421.0f, 262.0f, 0.0f);
                GXTexCoord2f32(1.0f, 1.0f);

                GXPosition3f32(189.0f, 262.0f, 0.0f);
                GXTexCoord2f32(0.0f, 1.0f);
                break;
        }
        GXSetProjectionv(projection);
        GXSetViewport(viewport[0], viewport[1], viewport[2], viewport[3], viewport[4], viewport[5]);
        GXSetScissor(sciLeft, sciTop, sciWidth, sciHeight);
    }
}


#ifndef VERSION_G8ME01
//param is unused
static s32 logoAsyncFirst(void) {
    return fileAsyncf(FILE_TYPE_RAW, 0, "%s/b/first", getMarioStDvdRoot());
}
#endif
void dummy_draw(CameraId camId, void* param) {
    GXRenderModeObj* rmode;
    f32 scale;
    u16 height;

    if (OSGetProgressiveMode() != 1) {
        rmode = &GXNtsc480IntDfMarioSt;
    } else {
        rmode = &GXNtsc480ProgMarioSt;
    }
    scale = GXGetYScaleFactor(rmode->efbHeight, rmode->xfbHeight);
    height = (u16)GXSetDispCopyYScale(scale);
    GXSetDispCopySrc(0, 0, rmode->fbWidth, rmode->efbHeight);
    GXSetDispCopyDst(rmode->fbWidth, height);
    GXSetPixelFmt(GX_PF_RGBA6_Z24, GX_ZC_LINEAR);
    GXSetScissor(0, 0, 608, 480);
    GXSetViewport(0.0f, 0.0f, 608.0f, 480.0f, 0.0f, 1.0f);
}

#ifndef VERSION_G8ME01
//param is unused
void progDisp(CameraId camId, void* param) {
    static OSTime time; //time instance 2
    const char* msg;
    f32 width;
    u32 button;
    s32 type;

    camGetPtr(camId); //unused
    if (wp->progressive) {
        switch (wp->promptStep) {
            case 0:
                wp->lastInputTime = OSGetTime();
                wp->promptStep++;
                break;

            case 1:
                button = keyGetButtonTrg(0);
                if (gp->dirs[0] ||
                    gp->button[0] ||
                    gp->stickX[0] ||
                    gp->stickY[0] ||
                    gp->substickX[0] ||
                    gp->substickY[0] ||
                    gp->triggerLeft[0] ||
                    gp->triggerRight[0]) {
                    wp->lastInputTime = OSGetTime();
                } else {
                    if (OSTicksToSeconds(OSGetTime() - wp->lastInputTime) > 10) {
                        button = 0x100;
                    }
                }
                if (button & (PAD_BUTTON_LEFT | PAD_BUTTON_RIGHT) || keyGetDirTrg(0) & (DEMO_STICK_LEFT | DEMO_STICK_RIGHT)) {
                    wp->toggle = 1 - wp->toggle;
                } else if (button & PAD_BUTTON_A) {
                    time = gp->renderTime;
                    wp->promptStep++;
                }

                msg = romFontGetMessage(0); //Display Progressive Scan?
                width = (f32)romFontGetWidth(msg);
                romFontPrintGX(
                    -(0.7f * width) / 2.0f,
                    -130.0f,
                    0.7f,
                    &(GXColor){0xFF, 0xFF, 0xFF, 0xFF},
                    msg);

                msg = romFontGetMessage(1); //yes/no
                width = (f32)romFontGetWidth(msg);
                romFontPrintGX(
                    -(0.7f * width) / 2.0f,
                    -160.0f,
                    0.7f,
                    &(GXColor){0xFF, 0xFF, 0xFF, 0xFF},
                    msg);

                msg = romFontGetMessage(2); //selection box
                width = 0.7f * (f32)romFontGetWidth(msg);
                romFontPrintGX(
                    -((width) / 2.0f - (f32)(75 * ((2 * wp->toggle) - 1))),
                    -160.0f,
                    0.7f,
                    &(GXColor){0xFF, 0xFF, 0xFF, 0xFF},
                    msg);
                break;

            case 2:
                if (OSTicksToMilliseconds(gp->renderTime - time) > 2000) {
                    wp->promptStep++;
                }
                type = 3; //progressive turned on
                if (wp->toggle) {
                    type = 4; //progressive turned off
                }
                msg = romFontGetMessage(type);
                width = (f32)romFontGetWidth(msg);
                romFontPrintGX(
                    -(0.7f * (f32)width) / 2.0f,
                    -145.0f,
                    0.7f,
                    &(GXColor){0xFF, 0xFF, 0xFF, 0xFF},
                    msg);
                break;

            case 3:
                if (!wp->toggle) {
                    VISetBlack(1);
                    OSSetProgressiveMode(1);
                    VIConfigure(&sRMObjHReso_prog);
                    VIFlush();
                    VIWaitForRetrace();
                    VIWaitForRetrace();
                    time = gp->renderTime;
                    wp->promptStep++;
                    gp->flags |= GW_FLAG_PROGRESSIVE;
                } else {
                    OSSetProgressiveMode(0);
                    wp->progressive = 0;
                    gp->flags &= ~GW_FLAG_PROGRESSIVE;
                }
                gp->flags |= GW_FLAG_VIDEO_MODE_SET;
                break;

            case 4:
                if (OSTicksToMilliseconds(gp->renderTime - time) > 2000) {
                    VISetBlack(FALSE);
                    wp->progressive = 0;
                }
                break;
        }
    }
}
#else
void progDisp(CameraId camId, void* param) {
    static OSTime time;
    const char* msg;
    f32 width;
    u32 button;
    s32 type;

    camGetPtr(camId);
    if (wp->progressive) {
        switch (wp->progState) {
            case 0:
                wp->lastInput = OSGetTime();
                wp->progState++;
                break;

            case 1:
                button = keyGetButtonTrg(0);
                if (gp->dirs[0] ||
                    gp->button[0] ||
                    gp->stickX[0] ||
                    gp->stickY[0] ||
                    gp->substickX[0] ||
                    gp->substickY[0] ||
                    gp->triggerLeft[0] ||
                    gp->triggerRight[0]) {
                    wp->lastInput = OSGetTime();
                } else {
                    if (OSTicksToSeconds(OSGetTime() - wp->lastInput) > 10) {
                        button = 0x100;
                    }
                }
                if (button & (PAD_BUTTON_LEFT | PAD_BUTTON_RIGHT) || keyGetDirTrg(0) & (DEMO_STICK_LEFT | DEMO_STICK_RIGHT)) {
                    wp->toggle = 1 - wp->toggle;
                } else if (button & PAD_BUTTON_A) {
                    wp->progState++;
                }

                msg = romFontGetMessage(0);
                width = (f32)romFontGetWidth(msg);
                romFontPrintGX(
                    -width / 2.0f,
                    -80.0f,
                    1.0f,
                    &(GXColor){0xFF, 0xFF, 0xFF, 0xFF},
                    msg);

                msg = romFontGetMessage(1);
                width = (f32)romFontGetWidth(msg);
                romFontPrintGX(
                    -width / 2.0f,
                    -160.0f,
                    1.0f,
                    &(GXColor){0xFF, 0xFF, 0xFF, 0xFF},
                    msg);

                msg = romFontGetMessage(2);
                width = (f32)romFontGetWidth(msg);
                romFontPrintGX(
                    (f32)(106 * ((2 * wp->toggle) - 1)) - width / 2.0f,
                    -160.0f,
                    1.0f,
                    &(GXColor){0xFF, 0xFF, 0xFF, 0xFF},
                    msg);
                break;

            case 2:
                if (!wp->toggle) {
                    VISetBlack(1);
                    OSSetProgressiveMode(1);
                    VIConfigure(&sRMObjHReso_prog);
                    VIFlush();
                    VIWaitForRetrace();
                    VIWaitForRetrace();
                    time = gp->renderTime;
                    wp->progState++;
                    gp->flags |= GW_FLAG_PROGRESSIVE;
                } else {
                    OSSetProgressiveMode(0);
                    time = gp->renderTime;
                    wp->progState += 2;
                    gp->flags &= ~GW_FLAG_PROGRESSIVE;
                }
                gp->flags |= GW_FLAG_VIDEO_MODE_SET;
                break;

            case 3:
                if (OSTicksToMilliseconds(gp->renderTime - time) > 2000) {
                    VISetBlack(FALSE);
                    VIFlush();
                    time = gp->renderTime;
                    wp->progState++;
                }
                break;

            case 4:
                if (OSTicksToMilliseconds(gp->renderTime - time) > 2000) {
                    wp->progressive = 0;
                    wp->progState++;
                }
                type = 3;
                if (wp->toggle) {
                    type = 4;
                }
                msg = romFontGetMessage(type);
                width = (f32)romFontGetWidth(msg);
                romFontPrintGX(
                    -width / 2.0f,
                    -100.0f,
                    1.0f,
                    &(GXColor){0xFF, 0xFF, 0xFF, 0xFF},
                    msg);
                break;
        }
    }
}

/* FAKE: the US build places this static first in .sbss, ahead of progDisp's; a static inside a
   deferred-inline helper defined last gives that order */
static inline s32* logoStoryTime(void) {
    static s32 time;
    return &time;
}
#endif
