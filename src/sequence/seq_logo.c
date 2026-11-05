// Logo sequence: the boot logos and the progressive-scan prompt before the title.

#include "version.h"
#include "seq/seq_logo.h"
#include "libc_decls.h"
#include "driver/arcdrv.h"

#if VERSION <= VERSION_DEMO_JP
#include "driver/animdrv.h"
#endif

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

#if VERSION >= VERSION_US
static inline s32* logoStoryTime(void);
#endif

//.sbss
static LogoWork work;

//.sdata
static LogoWork* wp = &work;

#if VERSION >= VERSION_DEMO_EU
// PAL: per-language logo screen textures {logo, story text}
static s32 logoTexTbl[6][2] = {{24, 25}, {24, 25}, {26, 27}, {28, 29}, {30, 31}, {32, 33}};
#endif

#define OSIsRestart() ((OSGetResetCode() & 0x80000000) ? TRUE : FALSE)

//local prototypes
static void logoDisp(CameraId camId, void* param);
static void dummy_draw(CameraId camId, void* param);
static void progDisp(CameraId camId, void* param);


#if VERSION < VERSION_US
static s32 logoAsyncFirst(void);
#endif

#if VERSION >= VERSION_JP
/**
 * Releases: on a cold start, NTSC asks for progressive scan when the cable allows it (always when it is saved,
 * or with B held); PAL always asks 50 Hz or 60 Hz, preselecting the saved EURGB60 choice. The September demos
 * ask nothing: they pick 60 Hz or progressive scan themselves when they can, and then wait 2 s for the TV.
 */
void seq_logoInit(SequenceWork* work) {
#if VERSION == VERSION_DEMO_US2 || VERSION == VERSION_DEMO_EU
    s32 i;
#endif
    memset(wp, 0, sizeof(LogoWork));
#if VERSION >= VERSION_US
    wp->page = -1;
#endif
    wp->texture = fileAllocf(FILE_TYPE_TPL, "%s/mariost.tpl", getMarioStDvdRoot());
#if VERSION == VERSION_DEMO_US2 || VERSION == VERSION_DEMO_EU
    wp->progressive = FALSE;
#endif
    if (!OSIsRestart() || OSGetResetCode() & OS_RESET_SHUTDOWN) {
#if VERSION == VERSION_DEMO_US2 || VERSION == VERSION_DEMO_EU
        // A PAL TV is switched to 60 Hz (EURGB60), an NTSC TV on a component cable to progressive scan unless B
        // is held. On a PAL TV sRMObjHReso_prog is the EURGB60 mode (the September US demo's marioStInit copied
        // the PAL modes into the NTSC ones).
        if (VIGetTvFormat() != VI_NTSC || (VIGetDTVStatus() && !(keyGetButton(0) & PAD_BUTTON_B))) {
            if (VIGetTvFormat() != VI_NTSC) {
                OSSetEuRgb60Mode(1);
            } else {
                OSSetProgressiveMode(1);
            }
            VISetBlack(TRUE);
            VIConfigure(&sRMObjHReso_prog);
            VIFlush();
            VIWaitForRetrace();
            VIWaitForRetrace();
            gp->flags |= GW_FLAG_PROGRESSIVE;
            // give the TV 2 s to settle on the new mode
            for (i = 0; i < 120; i++) {
                VIWaitForRetrace();
            }
        } else {
            if (VIGetTvFormat() != VI_NTSC) {
                OSSetEuRgb60Mode(0);
            } else {
                OSSetProgressiveMode(0);
            }
            VISetBlack(TRUE);
            VIConfigure(&sRMObjHReso);
            VIFlush();
            VIWaitForRetrace();
        }
        wp->progressive = FALSE;
        gp->flags |= GW_FLAG_VIDEO_MODE_SET;
#elif VERSION >= VERSION_DEMO_EU
        // PAL: always ask for 50Hz / 60Hz; preselect 60Hz when EURGB60 is saved
        wp->progressive = TRUE;
        wp->toggle = OSGetEuRgb60Mode() != 1;
#else
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
#endif
    }
#if VERSION == VERSION_DEMO_US2 || VERSION == VERSION_DEMO_EU
    else {
#else
#if VERSION < VERSION_US
    logoAsyncFirst();
#endif
    if (!OSIsRestart() || OSGetResetCode() & OS_RESET_SHUTDOWN) {
        VIConfigure(&sRMObjHReso);
        VIFlush();
        VIWaitForRetrace();
    } else {
#endif
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
#endif



#if VERSION <= VERSION_DEMO_JP

/**
 * The early demos always start interlaced; the US demo also starts reading the first data archive
 * and preloads the E3 demo's models and textures.
 */
void seq_logoInit(SequenceWork* work) {
    memset(wp, 0, sizeof(LogoWork));
#if VERSION == VERSION_DEMO_JP
    wp->texture = fileAllocf(4, "%s/monthry.tpl", getMarioStDvdRoot());
    wp->progressive = FALSE;
#else
    wp->texture = fileAllocf(4, "%s/mariost.tpl", getMarioStDvdRoot());
    wp->progressive = FALSE;
    work->state = 0;
    logoAsyncFirst();
#endif
    VIConfigure(&sRMObjHReso);
    VIFlush();
    VIWaitForRetrace();
#if VERSION != VERSION_DEMO_JP
    animGroupBaseAsync("c_pkr", 0, NULL);
    animGroupBaseAsync("c_pnk", 0, NULL);
    animGroupBaseAsync("c_windy", 0, NULL);
    animGroupBaseAsync("OFF_d_meku", 0, NULL);
    animGroupBaseAsync("EFF_Mugi", 0, NULL);
    fileAsyncf(4, 0, "%s/e3.tpl", getMarioStDvdRoot());
#endif
}
#endif

/**
 * Leaves the logo sequence: sets up the font, window, fade, icon,
 * environment and effect textures and Mario's poses for the rest of the game, and frees the logo texture.
 */
void seq_logoExit(SequenceWork* work) {
    fontmgrTexSetup();
    windowTexSetup();
    fadeTexSetup();
    iconTexSetup();
    envTexSetup();
    effTexSetup();
    marioPoseInit();
    fileFree(wp->texture);
#if VERSION <= VERSION_DEMO_JP
    gp->flags |= GW_FLAG_VIDEO_MODE_SET;
#endif
}

/**
 * Logo sequence: the three logos (1 s each, 500 ms fades) while sound groups load,
 * then the dmo_00 opening (skipped with A or Start to the title), or the kiosk menu on kiosk builds.
 * US and PAL first show the story screen (the September demos skip it); JP and the early demos read and decode the first data archive before leaving.
 */
void seq_logoMain(SequenceWork* work) {
    static OSTime time;
#if VERSION <= VERSION_DEMO_JP
    u32 size;
    u8* decode;
    FileEntry* first;
    FileEntry* ret;
    int ok;
#else
#if VERSION < VERSION_US
    u32 size;
    u8* decode;
    FileEntry* first;
    s32 ret;
    int ok;
#endif
#endif

    switch (work->state) {
        case 0:
            fadeEntry(FADE_IN_BLACK, 500, (GXColor){0, 0, 0, 0xFF});
            DVDMgrSetupCallback(psndPushGroup);
            work->state++;
            break;

        case 1:
#if VERSION <= VERSION_DEMO_JP
            if (fadeIsFinish() && !wp->progressive) {
                work->state++;
                time = gp->renderTime;
            }
            break;

        case 2:
#else
#if VERSION >= VERSION_US
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
#if VERSION == VERSION_DEMO_US2 || VERSION == VERSION_DEMO_EU
                work->state = 8; // the September demos skip the story screen
#else
                work->state++;
#endif
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
#if VERSION >= VERSION_DEMO_EU
                work->state++;
                time = gp->renderTime;
#else
                time = gp->renderTime;
                work->state++;
#endif
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
#endif
            if (OSTicksToMilliseconds(gp->renderTime - time) > 1000) {
                fadeEntry(FADE_OUT_BLACK, 500, (GXColor){0, 0, 0, 0xFF});
                work->state++;
            }
            break;

#if VERSION >= VERSION_US
        case 11:
#else
        case 3:
#endif
            if (fadeIsFinish()) {
                fadeEntry(FADE_IN_BLACK, 500, (GXColor){0, 0, 0, 0xFF});
                work->state++;
#if VERSION >= VERSION_US
                wp->page = 3;
#else
                wp->logoId = 1;
#endif
            }
            break;

#if VERSION >= VERSION_US
        case 12:
#else
        case 4:
#endif
            if (fadeIsFinish()) {
                work->state++;
                time = gp->renderTime;
            }
            break;

#if VERSION >= VERSION_US
        case 13:
#else
        case 5:
#endif
            if (OSTicksToMilliseconds(gp->renderTime - time) > 1000) {
                fadeEntry(FADE_OUT_BLACK, 500, (GXColor){0, 0, 0, 0xFF});
                work->state++;
            }
            break;

#if VERSION >= VERSION_US
        case 14:
#else
        case 6:
#endif
            if (fadeIsFinish()) {
                fadeEntry(FADE_IN_BLACK, 500, (GXColor){0, 0, 0, 0xFF});
                work->state++;
#if VERSION >= VERSION_US
                wp->page = 4;
#else
                wp->logoId = 2;
#endif
            }
            break;

#if VERSION >= VERSION_US
        case 15:
#else
        case 7:
#endif
            if (fadeIsFinish()) {
                work->state++;
                time = gp->renderTime;
            }
            break;

#if VERSION >= VERSION_US
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

#if VERSION >= VERSION_US
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
                    dispEntry(CAMERA_BACKGROUND, 0.0f, DISP_MODE_OPAQUE, dummy_draw, NULL);
                    work->state++;
                }
            }
            break;

#if VERSION <= VERSION_DEMO_JP
        case 10:
            VISetBlack(FALSE);
            VIWaitForRetrace();
            ret = (FileEntry*)logoAsyncFirst();
            if (ret == 0U) {
                ok = 0;
            } else if ((s32)ret == -1) {
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
#else
#if VERSION >= VERSION_US
        case 18:
            VISetBlack(FALSE);
            VIWaitForRetrace();
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
#endif
#endif
            if (!(gp->flags & GW_FLAG_E3)) {
                if (!wp->skipStory) { //play story demo
                    seqSetSeq(SEQ_MAP_CHANGE, "dmo_00", NULL);
                    return;
                }
                else {
                    fadeEntry(FADE_OUT_WHITE, 500, (GXColor){0, 0, 0, 0xFF});
                    work->state++;
                    return;
                }
            } else {
                seqSetSeq(SEQ_E3, NULL, NULL);
                return;
            }
            break;

#if VERSION >= VERSION_US
        case 19:
#else
        case 11:
#endif
            if (fadeIsFinish()) {
                seqSetSeq(SEQ_TITLE, NULL, NULL);
            }
            break;
    }

#if VERSION >= VERSION_US
    if (work->state >= 2 && work->state < 4) {
        dispEntry(CAMERA_2D, 0.0f, DISP_MODE_OPAQUE, progDisp, NULL);
    }
    if (work->state >= 4 && work->state < 18) {
        dispEntry(CAMERA_2D, 1.0f, DISP_MODE_OPAQUE, logoDisp, work);
#else
    if (work->state < 10) {
        dispEntry(CAMERA_2D, 1.0f, DISP_MODE_OPAQUE, logoDisp, NULL);
        dispEntry(CAMERA_2D, 0.0f, DISP_MODE_OPAQUE, progDisp, NULL);
#endif
    }
}


static void logoDisp(CameraId camId, void* param) {
    GXRenderModeObj* rmode;
    f32 width, height, x, y;
    // stack variables
    Mtx44 ortho;
    Mtx position;
    f32 projection[GX_PROJECTION_SZ];
    f32 viewport[GX_VIEWPORT_SZ];
    GXTexObj texObj;
    u32 sciLeft, sciTop, sciWidth, sciHeight;

    if (wp->texture) {
        GXGetProjectionv(projection);
        GXGetViewportv(viewport);
        GXGetScissor(&sciLeft, &sciTop, &sciWidth, &sciHeight);
#if VERSION >= VERSION_DEMO_EU
        // PAL and the PAL demo: 50 Hz on a cold start before state 3, otherwise the saved 50/60 Hz mode
        if ((!OSIsRestart() || OSGetResetCode() & OS_RESET_SHUTDOWN) && ((SequenceWork*)param)->state < 3) {
            rmode = &sRMObjHReso;
        } else if (OSGetEuRgb60Mode() != 1) {
            rmode = &sRMObjHReso;
        } else {
            rmode = &sRMObjHReso_prog;
        }
#elif VERSION == VERSION_DEMO_US2
        // on a PAL TV the NTSC modes hold the PAL ones (marioStInit), _prog the EURGB60 one
        if (VIGetTvFormat() != VI_NTSC) {
            if (OSGetEuRgb60Mode() != 1) {
                rmode = &sRMObjHReso;
            } else {
                rmode = &sRMObjHReso_prog;
            }
        } else {
            if (OSGetProgressiveMode() != 1) {
                rmode = &sRMObjHReso;
            } else {
                rmode = &sRMObjHReso_prog;
            }
        }
#else
        if (OSGetProgressiveMode() != 1) {
            rmode = &sRMObjHReso;
        } else {
            rmode = &sRMObjHReso_prog;
        }
#endif
        GXSetPixelFmt(GX_PF_RGB8_Z24, GX_ZC_LINEAR);
        GXSetDispCopySrc(0, 0, rmode->fbWidth, rmode->efbHeight);
        GXSetDispCopyDst(rmode->fbWidth, rmode->efbHeight);
        GXSetDispCopyYScale((f32)rmode->xfbHeight / (f32)rmode->efbHeight);
        GXSetDispCopyGamma(GX_GM_1_0);
        GXSetScissorBoxOffset(0, 0);
        GXSetScissor(0, 0, 608, 448);
        GXSetViewport(0.0f, 0.0f, 608.0f, 448.0f, -1.0f, 1.0f);

#if VERSION >= VERSION_US
        switch (wp->page) {
            case -1:
                break;

            case 0:
            case 1:
#if VERSION >= VERSION_DEMO_EU
                TEXGetGXTexObjFromPalette(*wp->texture->data, &texObj, logoTexTbl[gp->language][0]);
#else
                TEXGetGXTexObjFromPalette(*wp->texture->data, &texObj, 7);
#endif
                GXLoadTexObj(&texObj, GX_TEXMAP0);
                GXSetNumChans(0);
                GXSetTevColor(GX_TEVREG0, (GXColor){0, 0, 0, 0xFF});
                GXSetTevColor(GX_TEVREG1, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
                GXSetNumTexGens(1);
                GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
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
#if VERSION >= VERSION_DEMO_EU
                    TEXGetGXTexObjFromPalette(*wp->texture->data, &texObj, logoTexTbl[gp->language][1]);
#else
                    TEXGetGXTexObjFromPalette(*wp->texture->data, &texObj, 8);
#endif
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
                if (gp->language == LANGUAGE_JP) { // blue for Japanese
                    GXSetTevColor(GX_TEVREG1, (GXColor){0, 0x46, 0xFF, 0xFF});
                } else { // red for every other language
                    GXSetTevColor(GX_TEVREG1, (GXColor){0xDC, 0, 0, 0xFF});
                }
                GXSetNumTexGens(1);
                GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
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

#if VERSION >= VERSION_US
            case 3:
#else
            case 1:
#endif
                TEXGetGXTexObjFromPalette(*wp->texture->data, &texObj, 1);
                GXLoadTexObj(&texObj, GX_TEXMAP0);
                GXSetNumChans(0);
                GXSetNumTexGens(1);
                GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
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

#if VERSION >= VERSION_US
            case 4:
#else
            case 2:
#endif
                TEXGetGXTexObjFromPalette(*wp->texture->data, &texObj, 2);
                GXLoadTexObj(&texObj, GX_TEXMAP0);
                GXSetNumChans(1u);
                GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
                GXSetChanMatColor(GX_COLOR0A0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
                GXSetNumTexGens(1);
                GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
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


#if VERSION < VERSION_US

/**
 * JP and the early demos read the first data file asynchronously; JP and the US demo start it in seq_logoInit,
 * so it loads while the logos show, the JP demo only after the logos.
 */
static s32 logoAsyncFirst(void) {
    return fileAsyncf(FILE_TYPE_RAW, 0, "%s/b/first", getMarioStDvdRoot());
}
#endif

/**
 * Background draw callback queued for the frame the video mode switches:
 * sets the display copy and a 608x480 viewport and scissor for the interlaced
 * or progressive (PAL: 50 or 60 Hz) render mode.
 */
static void dummy_draw(CameraId camId, void* param) {
    GXRenderModeObj* rmode;
    f32 scale;
    u16 height;

#if VERSION == VERSION_DEMO_US2
    // as in logoDisp: on a PAL TV the NTSC modes hold the PAL and EURGB60 ones
    if (VIGetTvFormat() != VI_NTSC) {
        if (OSGetEuRgb60Mode() != 1) {
            rmode = &GXNtsc480IntDfMarioSt;
        } else {
            rmode = &GXNtsc480ProgMarioSt;
        }
    } else {
        if (OSGetProgressiveMode() != 1) {
            rmode = &GXNtsc480IntDfMarioSt;
        } else {
            rmode = &GXNtsc480ProgMarioSt;
        }
    }
#else
#if VERSION >= VERSION_DEMO_EU
    if (OSGetEuRgb60Mode() != 1) {
#else
    if (OSGetProgressiveMode() != 1) {
#endif
        rmode = &GXNtsc480IntDfMarioSt;
    } else {
        rmode = &GXNtsc480ProgMarioSt;
    }
#endif
    scale = GXGetYScaleFactor(rmode->efbHeight, rmode->xfbHeight);
    height = (u16)GXSetDispCopyYScale(scale);
    GXSetDispCopySrc(0, 0, rmode->fbWidth, rmode->efbHeight);
    GXSetDispCopyDst(rmode->fbWidth, height);
    GXSetPixelFmt(GX_PF_RGBA6_Z24, GX_ZC_LINEAR);
    GXSetScissor(0, 0, 608, 480);
    GXSetViewport(0.0f, 0.0f, 608.0f, 480.0f, 0.0f, 1.0f);
}

/**
 * Video-mode prompt: left/right toggles, A confirms, and 10 s idle picks the current choice.
 * PAL selects 50 or 60 Hz; the other versions select interlaced or progressive scan.
 * JP, the early demos and PAL show the result for 2 s before switching the video mode; US and the
 * September US demo switch first and show it after.
 * param is unused.
 */
#if VERSION < VERSION_US
#if VERSION <= VERSION_DEMO_JP
static void progDisp(CameraId camId, void* param) {
#else
static void progDisp(CameraId camId, void* param) {
#endif
#else
static void progDisp(CameraId camId, void* param) {
#endif
    static OSTime time;
    const char* msg;
    f32 width;
    u32 button;
    s32 type;

    camGetPtr(camId);
    if (wp->progressive) {
#if VERSION < VERSION_US
        switch (wp->promptStep) {
            case 0:
                wp->lastInputTime = OSGetTime();
                wp->promptStep++;
#else
        switch (wp->progState) {
            case 0:
                wp->lastInput = OSGetTime();
                wp->progState++;
#endif
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
#if VERSION < VERSION_US
                    wp->lastInputTime = OSGetTime();
                } else {
                    if (OSTicksToSeconds(OSGetTime() - wp->lastInputTime) > 10) {
#else
                    wp->lastInput = OSGetTime();
                } else {
                    if (OSTicksToSeconds(OSGetTime() - wp->lastInput) > 10) {
#endif
                        button = PAD_BUTTON_A;
                    }
                }
                if (button & (PAD_BUTTON_LEFT | PAD_BUTTON_RIGHT) || keyGetDirTrg(0) & (DEMO_STICK_LEFT | DEMO_STICK_RIGHT)) {
                    wp->toggle = 1 - wp->toggle;
                } else if (button & PAD_BUTTON_A) {
#if VERSION < VERSION_US
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
#else
#if VERSION >= VERSION_DEMO_EU
                    time = gp->renderTime;
#endif
                    wp->progState++;
                }

#if VERSION >= VERSION_DEMO_EU
                msg = romFontGetMessage(5);
#else
                msg = romFontGetMessage(0);
#endif
                width = (f32)romFontGetWidth(msg);
                romFontPrintGX(
                    -width / 2.0f,
#if VERSION >= VERSION_DEMO_EU
                    30.0f,
#else
                    -80.0f,
#endif
                    1.0f,
                    &(GXColor){0xFF, 0xFF, 0xFF, 0xFF},
                    msg);

#if VERSION >= VERSION_DEMO_EU
                    msg = romFontGetMessage(6);
#else
                    msg = romFontGetMessage(1);
#endif
                width = (f32)romFontGetWidth(msg);
                romFontPrintGX(
                    -width / 2.0f,
                    -160.0f,
                    1.0f,
                    &(GXColor){0xFF, 0xFF, 0xFF, 0xFF},
                    msg);

#if VERSION >= VERSION_DEMO_EU
                    msg = romFontGetMessage(7);
#else
                    msg = romFontGetMessage(2);
#endif
                width = (f32)romFontGetWidth(msg);
                romFontPrintGX(
#if VERSION >= VERSION_DEMO_EU
                    (f32)(108 * ((2 * wp->toggle) - 1)) - width / 2.0f,
#else
                    (f32)(106 * ((2 * wp->toggle) - 1)) - width / 2.0f,
#endif
                    -160.0f,
                    1.0f,
#endif
                    &(GXColor){0xFF, 0xFF, 0xFF, 0xFF},
                    msg);
                break;

            case 2:
#if VERSION < VERSION_US
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
                    -(0.7f * width) / 2.0f,
                    -145.0f,
                    0.7f,
                    &(GXColor){0xFF, 0xFF, 0xFF, 0xFF},
                    msg);
                break;

            case 3:
                if (!wp->toggle) {
                    VISetBlack(TRUE);
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
                    wp->progressive = FALSE;
                    gp->flags &= ~GW_FLAG_PROGRESSIVE;
                }
#if VERSION >= VERSION_JP
                gp->flags |= GW_FLAG_VIDEO_MODE_SET;
#endif
                break;

            case 4:
                if (OSTicksToMilliseconds(gp->renderTime - time) > 2000) {
                    VISetBlack(FALSE);
                    wp->progressive = FALSE;
                }
#else
#if VERSION >= VERSION_DEMO_EU
                if (OSTicksToMilliseconds(gp->renderTime - time) > 2000) {
#else
                if (!wp->toggle) {
                    VISetBlack(TRUE);
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
                    wp->progressive = FALSE;
#endif
                    wp->progState++;
                }
#if VERSION >= VERSION_DEMO_EU
                type = 8;
#else
                type = 3;
#endif
                if (wp->toggle) {
#if VERSION >= VERSION_DEMO_EU
                    type = 9;
#else
                    type = 4;
#endif
                }
                msg = romFontGetMessage(type);
                width = (f32)romFontGetWidth(msg);
                romFontPrintGX(
                    -width / 2.0f,
                    -100.0f,
                    1.0f,
                    &(GXColor){0xFF, 0xFF, 0xFF, 0xFF},
                    msg);
#if VERSION >= VERSION_DEMO_EU
                break;

            case 3:
                if (!wp->toggle) {
                    VISetBlack(1);
                    OSSetEuRgb60Mode(1);
                    gp->framerate = 60;
                    VIConfigure(&sRMObjHReso_prog);
                    VIFlush();
                    VIWaitForRetrace();
                    VIWaitForRetrace();
                    time = gp->renderTime;
                    wp->progState++;
                    gp->flags |= GW_FLAG_PROGRESSIVE;
                } else {
                    OSSetEuRgb60Mode(0);
                    gp->framerate = 50;
                    wp->progressive = FALSE;
                    gp->flags &= ~GW_FLAG_PROGRESSIVE;
                }
                gp->flags |= GW_FLAG_VIDEO_MODE_SET;
                break;

            case 4:
                if (OSTicksToMilliseconds(gp->renderTime - time) > 2000) {
                    VISetBlack(FALSE);
                    wp->progressive = FALSE;
                }
#endif
#endif
                break;
        }
    }
}

#if VERSION >= VERSION_US
/* FAKE: the US build places this static first in .sbss, ahead of progDisp's; a static inside a
   deferred-inline helper defined last gives that order */
static inline s32* logoStoryTime(void) {
    static s32 time;
    return &time;
}
#endif
