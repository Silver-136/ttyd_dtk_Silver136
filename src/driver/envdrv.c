#include "driver/envdrv.h"
#include "driver/arcdrv.h"
#include "driver/dispdrv.h"
#include "driver/mapdrv.h"
#include "mario/mariost.h"
#include "manager/dvdmgr.h"
#include "system.h"
#include "texPalette.h"
#include <dolphin/dvd.h>
#include <string.h>
#include "memory.h"
#include "sdk/texPalette.h"
#include "libc_decls.h"
#include <math.h>
#include "driver/camdrv.h"
#include "driver/fadedrv.h"

//.data

EnvironmentGlare envGlare0 = {
    GX_BM_BLEND, GX_BL_ONE, GX_BL_ONE, GX_TEV_KCSEL_7_8, GX_TEV_KCSEL_1, 5.25f, 16,
    {
        {-1.0f, 0.0f}, {-3.0f, 0.0f}, {-5.0f, 0.0f}, {-7.0f, 0.0f},
    {1.0f, 0.0f}, {3.0f, 0.0f}, {5.0f, 0.0f}, {7.0f, 0.0f},
    {0.0f, -1.0f}, {0.0f, -3.0f}, {0.0f, -5.0f}, {0.0f, -7.0f},
    {0.0f, 1.0f}, {0.0f, 3.0f}, {0.0f, 5.0f}, {0.0f, 7.0f},
    }
};

EnvironmentGlare envGlare1 = {
    GX_BM_BLEND, GX_BL_ONE, GX_BL_ONE, GX_TEV_KCSEL_7_8, GX_TEV_KCSEL_1_2, 3.5f, 8,
    {
        {-1.41419994831f, -1.41419994831f}, {0.0f, -1.0f}, {1.41419994831f, -1.41419994831f}, {-1.0f, 0.0f},
    {1.0f, 0.0f}, {-1.41419994831f, 1.41419994831f}, {0.0f, 1.0f}, {1.41419994831f, 1.41419994831f},
    }
};

EnvironmentGlare envGlare2 = {
    GX_BM_BLEND, GX_BL_ONE, GX_BL_ONE, GX_TEV_KCSEL_7_8, GX_TEV_KCSEL_1_4, 3.5f, 52,
    {
        {0.0f, -5.0f}, {-1.0f, -4.0f}, {0.0f, -4.0f}, {1.0f, -4.0f},
    {-2.0f, -3.0f}, {-1.0f, -3.0f}, {0.0f, -3.0f}, {1.0f, -3.0f},
    {2.0f, -3.0f}, {-3.0f, -2.0f}, {-2.0f, -2.0f}, {-1.0f, -2.0f},
    {0.0f, -2.0f}, {1.0f, -2.0f}, {2.0f, -2.0f}, {3.0f, -2.0f},
    {-3.0f, -1.0f}, {-2.0f, -1.0f}, {-1.0f, -1.0f}, {0.0f, -1.0f},
    {1.0f, -1.0f}, {2.0f, -1.0f}, {3.0f, -1.0f}, {-3.0f, 0.0f},
    {-2.0f, 0.0f}, {-1.0f, 0.0f}, {3.0f, 0.0f}, {2.0f, 0.0f},
    {1.0f, 0.0f}, {-3.0f, 1.0f}, {-2.0f, 1.0f}, {-1.0f, 1.0f},
    {0.0f, 1.0f}, {1.0f, 1.0f}, {2.0f, 1.0f}, {3.0f, 1.0f},
    {-3.0f, 2.0f}, {-2.0f, 2.0f}, {-1.0f, 2.0f}, {0.0f, 2.0f},
    {1.0f, 2.0f}, {2.0f, 2.0f}, {3.0f, 2.0f}, {-2.0f, 3.0f},
    {-1.0f, 3.0f}, {0.0f, 3.0f}, {1.0f, 3.0f}, {2.0f, 3.0f},
    {-1.0f, 4.0f}, {0.0f, 4.0f}, {1.0f, 4.0f}, {0.0f, 5.0f},
    }
};

EnvironmentGlare envGlare3 = {
    GX_BM_BLEND, GX_BL_ONE, GX_BL_ONE, GX_TEV_KCSEL_7_8, GX_TEV_KCSEL_1_2, 3.5f, 16,
    {
        {0.0f, -4.0f}, {-1.0f, -3.0f}, {1.0f, -3.0f}, {-2.0f, -2.0f},
    {2.0f, -2.0f}, {-3.0f, -1.0f}, {3.0f, -1.0f}, {-3.0f, 0.0f},
    {3.0f, 0.0f}, {-3.0f, 1.0f}, {3.0f, 1.0f}, {-2.0f, 2.0f},
    {2.0f, 2.0f}, {-1.0f, 3.0f}, {1.0f, 3.0f}, {0.0f, 4.0f},
    }
};

//.bss
static EnvironmentWork work[2];

//.sbss
TEXPalette* env_tpl = NULL;
s32 tpl_ok = 0;

//local prototypes
void envCapture(CameraId camId, void* param);
void envCapture2(CameraId camId, void* param);
void envDisp(CameraId camId, void* param);
void envDisp2(CameraId camId, void* param);
void envDisp_DepthOfField(void);
void envDisp_Blur(void);
void envDisp_FF(void);

//there is no function so we'll make a macro
#define envGetWork() (gp->inBattle ? &work[1] : &work[0])

static void _callback(s32 result, DVDFileInfo* fileInfo) {
    DVDEntry* handle = fileInfo->cb.userData;

    UnpackTexPalette(env_tpl);
    DVDMgrClose(handle);
    tpl_ok = TRUE;
}

void envInit(void) {
    memset(&work[0], 0, sizeof(EnvironmentWork));
    memset(&work[1], 0, sizeof(EnvironmentWork));
    env_tpl = NULL;
    tpl_ok = FALSE;
}

void envReInit(void) {
    EnvironmentWork* wp = envGetWork();
    memset(wp, 0, sizeof(EnvironmentWork));
}

void envTexSetup(void) {
    DVDEntry* entry;
    TEXPalette* tpl;
    char path[128];
    u32 size;

    env_tpl = NULL;
    tpl = arcOpen("env.tpl", 0, 0);
    env_tpl = tpl;
    if (!tpl) {
        sprintf(path, ("%s/env.tpl"), getMarioStDvdRoot());
        entry = DVDMgrOpen(path, 2, 0);
        size = DVDMgrGetLength(entry);
        env_tpl = __memAlloc(HEAP_DEFAULT, OSRoundUp32B(size));
        entry->info.cb.userData = entry;
        DVDMgrReadAsync(entry, env_tpl, size, 0, _callback);
    } else {
        UnpackTexPalette(tpl);
        tpl_ok = 1;
    }
}

void envCapture(CameraId camId, void* param) {
    EnvironmentWork* wp = envGetWork();
    u16 var0, var1, var2, var3;
    u32 size;

    var0 = gp->fbWidth;
    var1 = gp->efbHeight;
    var2 = (u16)(((u32)var0 & 0xFFFF) / 2);
    var3 = (u16)(((u32)var1 & 0xFFFF) / 2);
    if (wp->flags & ENV_FLAG_CAPTURE_COLOR_UNUSED) {
        GXSetTexCopySrc(0, 0, gp->fbWidth, gp->efbHeight);
        GXSetTexCopyDst(var2, var3, GX_TF_RGBA8, GX_TRUE);
        size = GXGetTexBufferSize(var2, var3, GX_TF_RGBA8, GX_FALSE, 0);
        wp->capture = smartAlloc(size, 3);
        GXCopyTex(wp->capture->address, GX_FALSE);
        GXPixModeSync();
        GXInitTexObj(&wp->captureObj, wp->capture->address, var2, var3, GX_TF_RGBA8, GX_CLAMP, GX_CLAMP, GX_FALSE);
        GXInitTexObjLOD(&wp->captureObj, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    }
    if (wp->flags & ENV_FLAG_CAPTURE_DEPTH_UNUSED) {
        GXSetTexCopySrc(0, 0, var0, var1);
        GXSetTexCopyDst(var2, var3, GX_CTF_Z8M, GX_TRUE);
        size = GXGetTexBufferSize(var2, var3, GX_CTF_Z8M, GX_FALSE, 0);
        wp->capture2 = smartAlloc(size, 3);
        GXCopyTex(wp->capture2->address, 0);
        GXPixModeSync();
        GXInitTexObj(&wp->captureObj2, wp->capture2->address, var2, var3, GX_TF_I8, GX_CLAMP, GX_CLAMP, GX_FALSE);
        GXInitTexObjLOD(&wp->captureObj2, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    }
}

void envCapture2(CameraId camId, void* param) {
    EnvironmentWork* wp = envGetWork();
    u16 var0, var1;

    var0 = (u16)(((u32)gp->fbWidth & 0xFFFF) / 2);
    var1 = (u16)(((u32)gp->efbHeight & 0xFFFF) / 2);
    if (wp->flags & ENV_FLAG_BLUR_CAPTURE) {
        GXSetTexCopySrc(0, 0, gp->fbWidth, gp->efbHeight);
        GXSetTexCopyDst(var0, var1, GX_TF_RGB565, GX_TRUE);
        GXCopyTex(wp->envBlur, GX_FALSE);
        GXPixModeSync();
        GXInitTexObj(&wp->envBlurObj, wp->envBlur, var0, var1, GX_TF_RGB565, GX_CLAMP, GX_CLAMP, GX_FALSE);
        GXInitTexObjLOD(&wp->envBlurObj, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    }
    if (wp->flags & ENV_FLAG_BLUR) {
        if ((wp->blur.flags & 1) && (wp->flags & ENV_FLAG_BLUR_CAPTURED)) {
            wp->flags &= ~ENV_FLAG_BLUR_CAPTURE;
        }
        wp->flags |= ENV_FLAG_BLUR_CAPTURED;
    }
}

void envDisp(CameraId camId, void *param) {
    f32 vp[GX_VIEWPORT_SZ];
    f32 pm[GX_PROJECTION_SZ];

    GXGetViewportv(vp);
    GXGetProjectionv(pm);
    envDisp_DepthOfField();
    envDisp_Blur();
    envDisp_FF();
    GXSetViewport(vp[0], vp[1], vp[2], vp[3], vp[4], vp[5]);
    GXSetProjectionv(pm);
}

void envDisp2(CameraId camId, void *param) {
    f32 vp[GX_VIEWPORT_SZ];
    f32 pm[GX_PROJECTION_SZ];
    EnvironmentWork* wp;
    EnvironmentGlareFilter* filter;

    GXGetViewportv(vp);
    GXGetProjectionv(pm);
    wp = envGetWork();
    filter = &wp->filter; //CodeWarrior is very good compiler, yesyes
    if (wp->flags & ENV_FLAG_GLARE_FILTER) {
        switch (filter->type) {
            case 0:
                break;
            case 1:
                envGlare(&envGlare0);
                break;
            case 2:
                envGlare(&envGlare1);
                break;
            case 3:
                envGlare(&envGlare2);
                break;
            case 4:
                envGlare(&envGlare3);
                break;
        }
    }
    GXSetViewport(vp[0], vp[1], vp[2], vp[3], vp[4], vp[5]);
    GXSetProjectionv(pm);
}

void envMain(void) {
    dispEntry(CAMERA_3D, 20.0f, 7, envCapture, NULL);
    dispEntry(CAMERA_3D, 10.0f, 7, envDisp, NULL);
    dispEntry(CAMERA_3D, 0.0f, 7, envCapture2, NULL);
    dispEntry(CAMERA_3D, 5000.0f, 8, envDisp2, NULL);
}

static const s32 texmtx_tbl[] = {30, 33, 36, 39, 42, 45, 48, 51, 54, 57};

static inline f32
env_sin(f32 x) {
    f32 a = 0.00761f;
    f32 b = 0.16605f;
    f32 c = 1.0f;
    f32 y;
    f32 x2;

    if (x <= 3.1415927f) {
        if (x <= 1.5707964f) {
            x2 = x * x;
            a = a * x2 - b;
            a = a * x2 + c;
            a = a * x;
        } else {
            y = 1.5707964f - (x - 1.5707964f);
            x2 = y * y;
            a = a * x2 - b;
            a = a * x2 + c;
            a = a * y;
        }
    } else if (x < 4.712389f) {
        y = x - 3.1415927f;
        x2 = y * y;
        a = a * x2 - b;
        a = a * x2 + c;
        a = a * y;
        a = -a;
    } else {
        y = 1.5707964f - (x - 4.712389f);
        x2 = y * y;
        a = a * x2 - b;
        a = a * x2 + c;
        a = a * y;
        a = -a;
    }
    return a;
}

static inline f32
env_cos(f32 x) {
    f32 a = 0.03705f;
    f32 b = 0.4967f;
    f32 c = 1.0f;
    f32 y;
    f32 x2;

    if (x <= 3.1415927f) {
        if (x <= 1.5707964f) {
            x2 = x * x;
            a = a * x2 - b;
            a = a * x2 + c;
        } else {
            y = 1.5707964f - (x - 1.5707964f);
            x2 = y * y;
            a = a * x2 - b;
            a = a * x2 + c;
            a = -a;
        }
    } else if (x < 4.712389f) {
        y = x - 3.1415927f;
        x2 = y * y;
        a = a * x2 - b;
        a = a * x2 + c;
        a = -a;
    } else {
        y = 1.5707964f - (x - 4.712389f);
        x2 = y * y;
        a = a * x2 - b;
        a = a * x2 + c;
    }
    return a;
}

// FAKE: the first vertex's y goes through an inline return temp, so in envDisp_DepthOfField (opt_propagation off)
// it is numbered after the x product and x keeps f0 as in the target. Same value; envDisp_Blur is unaffected.
static inline f32 dofDiv(f32 a, f32 b) { return a / b; }
#define DOF_QUAD(g, half)                                                              \
    GXPosition3f32(-(s32)(g)->fbWidth / (half), dofDiv((g)->efbHeight, (half)), 0.0f); \
    GXTexCoord2f32(0.0f, 0.0f);                                                        \
    GXPosition3f32((g)->fbWidth / (half), (g)->efbHeight / (half), 0.0f);              \
    GXTexCoord2f32(1.0f, 0.0f);                                                        \
    GXPosition3f32((g)->fbWidth / (half), -(s32)(g)->efbHeight / (half), 0.0f);        \
    GXTexCoord2f32(1.0f, 1.0f);                                                        \
    GXPosition3f32(-(s32)(g)->fbWidth / (half), -(s32)(g)->efbHeight / (half), 0.0f);  \
    GXTexCoord2f32(0.0f, 1.0f)

static const GXColor white = {0xFF, 0xFF, 0xFF, 0xFF};

#pragma opt_propagation off
#pragma opt_lifetimes off
// FAKE: tbl, tbl2, coord2 and stage2 are one-element local arrays. The front end scalarizes them into @ temps,
// which sit in the coalesce window: `tbl = texmtx_tbl` then loads straight into the pointer register (a named
// pointer under opt_propagation off gives addi r0 + mr), and loop 2's zero inits become the target's li/mr copies.
// i2 is an `int` array: an s32 (long) element is not scalarized (the int-typed loop compares count as a different
// type), so it would stay in memory. The `(s32)` cast in `(s32)i2[0] == 3` stops CSE with the `3 == i2[0]` below.
void envDisp_DepthOfField(void) {
    GXTevColorArg cin;
    s32 first2[1];
    s32 coord2[1];
    EnvironmentWork* wp = envGetWork();
    const s32* tbl2[1];
    s32 stage2[1];
    s32 coord;
    s32 stage;
    int i2[1];
    s32 first;
    const s32* tbl[1];
    s32 tc;
    s32 tc2;
    EnvironmentDof* dof;
    CameraEntry* cam[1];
    GXColor color;
    Mtx mtx;
    s32 i;
    Mtx mtx2;
    f32 ang;
    f32 ang2;
    s32 gz;
    int k;
    s32 gz2;
    s32 hz;
    s32 mg[1];

    /* FAKE: dead early stores (removed by the front end). They only set the first-reference order that numbers
       the scalarized array temps: tbl, then mg, then cam, then the loop 2 arrays. */
    tbl[0] = texmtx_tbl;
    mg[0] = 0x43300000;
    dof = &wp->dof;
    if (wp->flags & ENV_FLAG_DOF_UNUSED) {
        /* FAKE: cam is a one-element array so it becomes an @ temp numbered between mg and tbl2 */
        cam[0] = camGetPtr(CAMERA_2D);
        // FAKE: dead guard (gz is 0) gives cam a second def, so it stays a named web instead of the call-result temp
        gz = 0;
        if (gz) cam[0] = (CameraEntry*)wp;
        GXSetProjection(camGetPtr(CAMERA_2D)->projection, cam[0]->projType);
        GXLoadPosMtxImm(camGetPtr(CAMERA_2D)->view, 0);
        GXSetCurrentMtx(0);
        GXLoadTexObj(smartTexObj(&wp->captureObj, (void**)wp->capture), GX_TEXMAP0);
        GXLoadTexObj(smartTexObj(&wp->captureObj2, (void**)wp->capture2), GX_TEXMAP1);
        GXSetNumChans(0);
        GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
        GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_OR);
        GXSetZCompLoc(GX_FALSE);
        GXSetAlphaCompare(GX_GEQUAL, 0x80, GX_AOP_OR, GX_NEVER, 0);
        GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
        /* FAKE: a one-trip loop. The back end unlinks it, which enables its array-to-register pass, so first2
           (kept in memory by the front end, see below) is promoted late and numbered as a codegen temp. */
        for (k = 0; k < 1; k++) {
            GXSetCullMode(GX_CULL_NONE);
        }
        if ((dof->flags & ENV_DOF_PASS1) && dof->radius1) {
            tbl[0] = texmtx_tbl;
            coord = 0;
            stage = 0;
            for (i = 0; i < 4; i++, coord++, tbl[0]++, stage++) {
                ang = 6.2831855f * i / 4.0f;
                PSMTXTrans(mtx, dof->radius1 * env_sin(ang) / 100.0f, dof->radius1 * env_cos(ang) / 100.0f, 0.0f);
                GXLoadTexMtxImm(mtx, *tbl[0], GX_MTX2x4);
                GXSetTexCoordGen2((GXTexCoordID)coord, GX_TG_MTX2x4, GX_TG_TEX0, *tbl[0], GX_FALSE, GX_PTIDENTITY);
                GXSetTevKColorSel((GXTevStageID)stage, GX_TEV_KCSEL_1_4);
                GXSetTevKAlphaSel((GXTevStageID)stage, GX_TEV_KASEL_1_4);
                GXSetTevOrder((GXTevStageID)stage, (GXTexCoordID)coord, GX_TEXMAP0, GX_COLOR_NULL);
                GXSetTevColorOp((GXTevStageID)stage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, (GXTevRegID)(i == 3));
                GXSetTevAlphaOp((GXTevStageID)stage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, (GXTevRegID)(3 == i));
                cin = GX_CC_CPREV;
                if (first = (i == 0)) {
                    cin = GX_CC_ZERO;
                }
                GXSetTevColorIn((GXTevStageID)stage, GX_CC_ZERO, GX_CC_TEXC, GX_CC_KONST, cin);
                GXSetTevAlphaIn((GXTevStageID)stage, GX_CA_ZERO, GX_CA_TEXA, GX_CA_KONST, first ? GX_CA_ZERO : GX_CA_APREV);
                GXSetTevSwapMode((GXTevStageID)stage, GX_TEV_SWAP0, GX_TEV_SWAP0);
            }
            color.r = dof->level1;
            color.a = 0xFF;
            GXSetTevKColor(GX_KCOLOR0, color);
            GXSetTexCoordGen2((GXTexCoordID)coord, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
            tc = coord++;
            GXSetTevKColorSel((GXTevStageID)stage, GX_TEV_KCSEL_K0);
            GXSetTevKAlphaSel((GXTevStageID)stage, GX_TEV_KASEL_K0_A);
            GXSetTevOrder((GXTevStageID)stage, (GXTexCoordID)tc, GX_TEXMAP1, GX_COLOR_NULL);
            GXSetTevColorOp((GXTevStageID)stage, GX_TEV_COMP_R8_GT, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevAlphaOp((GXTevStageID)stage, GX_TEV_COMP_R8_GT, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevColorIn((GXTevStageID)stage, GX_CC_TEXC, GX_CC_KONST, GX_CC_C0, GX_CC_ZERO);
            GXSetTevAlphaIn((GXTevStageID)stage, GX_CA_TEXA, GX_CA_KONST, GX_CA_A0, GX_CA_ZERO);
            GXSetTevSwapMode((GXTevStageID)stage, GX_TEV_SWAP0, GX_TEV_SWAP0);
            stage++;
            GXSetNumTexGens((u8)coord);
            GXSetNumTevStages((u8)stage);
            GXBegin(GX_QUADS, GX_VTXFMT0, 4);
            DOF_QUAD(gp, 2.0f);
        }
        if ((dof->flags & ENV_DOF_PASS2) && dof->radius2) {
            /* FAKE: dead read of i2 with a different type. It stops the first front end pass from scalarizing i2;
               the read is then deleted and the second pass scalarizes it, so its @ temp is created after the
               CSE temps and numbers below them. */
            hz = *(s16*)i2;
            /* FAKE: mg holds the int to float magic 0x43300000. The back end value-numbers the hoisted loop 2 lis
               into mg, so the constant gets mg @ temp number. i2 is initialised last so it does not overlap the
               tbl2 address temp. */
            for (coord2[0] = 0, stage2[0] = 0, mg[0] = 0x43300000, tbl2[0] = texmtx_tbl, i2[0] = 0; i2[0] < 4; i2[0]++, coord2[0]++, tbl2[0]++, stage2[0]++) {
                ang2 = 6.2831855f * i2[0] / 4.0f;
                PSMTXTrans(mtx2, dof->radius2 * env_sin(ang2) / 100.0f, dof->radius2 * env_cos(ang2) / 100.0f, 0.0f);
                GXLoadTexMtxImm(mtx2, *tbl2[0], GX_MTX2x4);
                GXSetTexCoordGen2((GXTexCoordID)coord2[0], GX_TG_MTX2x4, GX_TG_TEX0, *tbl2[0], GX_FALSE, GX_PTIDENTITY);
                GXSetTevKColorSel((GXTevStageID)stage2[0], GX_TEV_KCSEL_1_4);
                GXSetTevKAlphaSel((GXTevStageID)stage2[0], GX_TEV_KASEL_1_4);
                GXSetTevOrder((GXTevStageID)stage2[0], (GXTexCoordID)coord2[0], GX_TEXMAP0, GX_COLOR_NULL);
                GXSetTevColorOp((GXTevStageID)stage2[0], GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, (GXTevRegID)((s32)i2[0] == 3));
                GXSetTevAlphaOp((GXTevStageID)stage2[0], GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, (GXTevRegID)(3 == i2[0]));
                cin = GX_CC_CPREV;
                /* FAKE: first2 is an s32 array stored through an int pointer: the type mismatch keeps it in memory
                   through the front end (see the one-trip loop above) */
                if (*(int*)first2 = (i2[0] == 0)) {
                    cin = GX_CC_ZERO;
                }
                GXSetTevColorIn((GXTevStageID)stage2[0], GX_CC_ZERO, GX_CC_TEXC, GX_CC_KONST, cin);
                GXSetTevAlphaIn((GXTevStageID)stage2[0], GX_CA_ZERO, GX_CA_TEXA, GX_CA_KONST, first2[0] ? GX_CA_ZERO : GX_CA_APREV);
                GXSetTevSwapMode((GXTevStageID)stage2[0], GX_TEV_SWAP0, GX_TEV_SWAP0);
            }
            /* FAKE: dead guard (gz2 is 0) that keeps mg alive through the front end */
            gz2 = 0;
            if (gz2) coord2[0] = mg[0];
            color.r = dof->level2;
            color.a = 0xFF;
            GXSetTevKColor(GX_KCOLOR0, color);
            GXSetTexCoordGen2((GXTexCoordID)coord2[0], GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
            tc2 = coord2[0]++;
            GXSetTevColor(GX_TEVREG1, white);
            GXSetTevKColorSel((GXTevStageID)stage2[0], GX_TEV_KCSEL_K0);
            GXSetTevKAlphaSel((GXTevStageID)stage2[0], GX_TEV_KASEL_K0_A);
            GXSetTevOrder((GXTevStageID)stage2[0], (GXTexCoordID)tc2, GX_TEXMAP1, GX_COLOR_NULL);
            GXSetTevColorOp((GXTevStageID)stage2[0], GX_TEV_COMP_R8_GT, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevAlphaOp((GXTevStageID)stage2[0], GX_TEV_COMP_R8_GT, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevColorIn((GXTevStageID)stage2[0], GX_CC_TEXC, GX_CC_KONST, GX_CC_ONE, GX_CC_ZERO);
            GXSetTevAlphaIn((GXTevStageID)stage2[0], GX_CA_TEXA, GX_CA_KONST, GX_CA_A1, GX_CA_ZERO);
            GXSetTevSwapMode((GXTevStageID)stage2[0], GX_TEV_SWAP0, GX_TEV_SWAP0);
            GXSetTevOrder((GXTevStageID)(stage2[0] + 1), GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
            GXSetTevColorOp((GXTevStageID)(stage2[0] + 1), GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevAlphaOp((GXTevStageID)(stage2[0] + 1), GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevColorIn((GXTevStageID)(stage2[0] + 1), GX_CC_C0, GX_CC_ZERO, GX_CC_APREV, GX_CC_ZERO);
            GXSetTevAlphaIn((GXTevStageID)(stage2[0] + 1), GX_CA_A0, GX_CA_ZERO, GX_CA_APREV, GX_CA_ZERO);
            GXSetTevSwapMode((GXTevStageID)(stage2[0] + 1), GX_TEV_SWAP0, GX_TEV_SWAP0);
            stage2[0] += 2;
            GXSetNumTexGens((u8)coord2[0]);
            GXSetNumTevStages((u8)stage2[0]);
            GXBegin(GX_QUADS, GX_VTXFMT0, 4);
            DOF_QUAD(gp, 2.0f);
        }
    }
}

#pragma opt_propagation reset
#pragma opt_lifetimes reset

void envDisp_Blur(void) {
    EnvironmentWork* wp = envGetWork();
    EnvBlurWork* blur = &wp->blur;
    CameraEntry* cam;
    s32 msec;
    f32 t;
    u8 alpha;

    if ((wp->flags & ENV_FLAG_BLUR) && (wp->flags & ENV_FLAG_BLUR_CAPTURED)) {
        cam = camGetPtr(CAMERA_2D);
        GXSetProjection(camGetPtr(CAMERA_2D)->projection, cam->projType);
        GXLoadPosMtxImm(camGetPtr(CAMERA_2D)->view, 0);
        GXSetCurrentMtx(0);
        GXSetNumChans(0);
        GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
        GXSetNumTevStages(1);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_KONST, GX_CA_TEXA, GX_CA_ZERO);
        GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
        GXSetTevKColorSel(GX_TEVSTAGE0, GX_TEV_KCSEL_K0);
        GXSetTevKAlphaSel(GX_TEVSTAGE0, GX_TEV_KASEL_K0_A);
        msec = blur->msec;
        if (msec) {
            t = (f32)OSTicksToMilliseconds((u64)(gp->renderTime - blur->time)) / (f32)msec;
            if (t > 1.0f) {
                t = 1.0f;
            }
            if (blur->flags & 1) {
                alpha = 255.0f * (1.0f - t);
            } else {
                alpha = 120.0f * (1.0f - t) + 128.0f;
            }
            GXSetTevKColor(GX_KCOLOR0, (GXColor){0xFF, 0xFF, 0xFF, alpha});
        } else {
            GXSetTevKColor(GX_KCOLOR0, (GXColor){0xFF, 0xFF, 0xFF, 0xA0});
        }
        GXLoadTexObj(&wp->envBlurObj, GX_TEXMAP0);
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
        GXSetZCompLoc(GX_TRUE);
        GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
        GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        DOF_QUAD(gp, 2.0f);
    }
}

void envBlurOn(s32 type, s32 msec) {
    EnvironmentWork* wp = envGetWork();
    EnvBlurWork* blur = &wp->blur;

    wp->flags |= ENV_FLAG_BLUR;
    wp->flags |= ENV_FLAG_BLUR_CAPTURE;
    wp->flags &= ~ENV_FLAG_BLUR_CAPTURED;
    memset(blur, 0, sizeof(EnvBlurWork));
    blur->time = gp->renderTime;
    blur->msec = msec;
    if (type) {
        blur->flags |= 1;
    }
    wp->envBlur = _mapAlloc(GXGetTexBufferSize(gp->fbWidth / 2,
        gp->efbHeight / 2, GX_TF_RGB565, GX_FALSE, 0));
}

void envBlurOff(void) {
    EnvironmentWork* wp = envGetWork();

    wp->flags &= ~ENV_FLAG_BLUR_CAPTURE;
    wp->flags &= ~ENV_FLAG_BLUR;
    _mapFree(wp->envBlur);
}

void envGlare(EnvironmentGlare* glare) {
    static f32 frame_cnt = 0.0f;
    EnvironmentWork* envWork = envGetWork();
    s16 sx;
    EnvironmentGlareFilter* glareFilter;
    u32 texH;
    u32 texW;
    EnvironmentGlare* glareIter;
    EnvironmentGlare* glareCur;
    s32 width;
    s32 y1v;

    s32 x1v;
    s32 glareCount;
    u32 texMtxId;
    s16 sy;
    s32 doneCount;
    s32 batchCount;
    void* texBuf;
    u16 y0;
    u16 x0;
    u16 height;

    u32 size;
    Mtx44 projMtx;
    Mtx posMtx;
    Mtx texMtx;
    GXTexObj texObj;

    int i;

    frame_cnt += 1.0f;
    glareFilter = &envWork->filter;
    y0 = envWork->filter.y0;
    x0 = envWork->filter.x0;
    width = (u16)(envWork->filter.x1 - x0);
    height = (u16)(envWork->filter.y1 - y0);
    texH = height >> 1;
    texW = width >> 1;
    size = GXGetTexBufferSize((u16)texW, (u16)texH, GX_TF_RGB565, GX_FALSE, 0);
    texBuf = smartAlloc(size, 3)->address;
    GXSetTexCopySrc(x0, y0, width, height);
    GXSetTexCopyDst((u16)texW, (u16)texH, GX_TF_RGB565, GX_TRUE);
    GXCopyTex(texBuf, GX_FALSE);
    GXPixModeSync();
    GXInitTexObj(&texObj, texBuf, (u16)texW, (u16)texH, GX_TF_RGB565, GX_CLAMP, GX_CLAMP, GX_FALSE);
    GXInitTexObjLOD(&texObj, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    glareCount = glare->count;
    doneCount = 0;
    texW = glare->kcolorSel0;
    texH = glare->kcolorSel1;
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_SUB, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVREG1);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_KONST, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
    GXSetTevKColorSel(GX_TEVSTAGE0, texW);
    GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_C1, GX_CC_KONST, GX_CC_ZERO);
    GXSetTevKColorSel(GX_TEVSTAGE1, texH);
    GXSetTevSwapMode(GX_TEVSTAGE1, GX_TEV_SWAP0, GX_TEV_SWAP0);

    for (i = 0; i < 7; i++) {
        GXSetTevOrder(i * 2 + 2, i + 1, GX_TEXMAP0, GX_COLOR_NULL);
        GXSetTevColorOp(i * 2 + 2, GX_TEV_SUB, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVREG1);
        GXSetTevColorIn(i * 2 + 2, GX_CC_KONST, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
        GXSetTevKColorSel(i * 2 + 2, texW);
        GXSetTevSwapMode(i * 2 + 2, GX_TEV_SWAP0, GX_TEV_SWAP0);
        GXSetTevOrder(i * 2 + 3, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
        GXSetTevColorOp(i * 2 + 3, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
        GXSetTevColorIn(i * 2 + 3, GX_CC_ZERO, GX_CC_C1, GX_CC_KONST, GX_CC_CPREV);
        GXSetTevKColorSel(i * 2 + 3, texH);
        GXSetTevSwapMode(i * 2 + 3, GX_TEV_SWAP0, GX_TEV_SWAP0);
    }

    {
        int j;
        for (j = 0; j < 8; j++) {
            GXSetTexCoordGen(j, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0 + j * 3);
        }
    }

    if (glareFilter->flags & ENV_GLARE_FILTER_Z) {
        GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
    } else {
        GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
    }
    GXSetBlendMode(glare->mode, glare->srcFactor, glare->dstFactor, GX_LO_CLEAR);
    GXSetNumChans(0);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_CLAMP, GX_AF_NONE);
    GXSetViewport(0.0f, 0.0f, (f32)gp->fbWidth, (f32)gp->efbHeight, 0.0f, 1.0f);
    GXSetScissor(0, 0, gp->fbWidth, gp->efbHeight);
    MTXOrtho(projMtx, 0.0f, (f32)gp->efbHeight, 0.0f, (f32)gp->fbWidth, 0.0f, -32767.0f);
    GXSetProjection(projMtx, GX_ORTHOGRAPHIC);
    PSMTXIdentity(posMtx);
    GXLoadPosMtxImm(posMtx, 0);
    GXSetCurrentMtx(0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT7, GX_VA_POS, GX_POS_XYZ, GX_S16, 0);
    GXSetVtxAttrFmt(GX_VTXFMT7, GX_VA_TEX0, GX_TEX_ST, GX_U16, 0);
    GXSetCullMode(GX_CULL_BACK);
    GXSetZScaleOffset(1.0f, glareFilter->zscale);
    y1v = y0 + height;
    x1v = x0 + width;
    sx = x0;
    sy = y0;
    glareIter = glare;
    while (doneCount < glareCount) {
        glareCur = glareIter;
        batchCount = 0;
        texMtxId = 0x1E;
        while ((batchCount < 8) && (doneCount < glareCount)) {
            MTXTrans(texMtx, (glare->coordScale * glareCur->coords[0].x) / (f32) gp->fbWidth, (glare->coordScale * glareCur->coords[0].y) / (f32) gp->efbHeight, 0.0f);
            GXLoadTexMtxImm(texMtx, texMtxId, GX_MTX2x4);
            texMtxId += 3;
            batchCount += 1;
            glareCur = (EnvironmentGlare*)((EnvironmentGlareTexCoord*)glareCur + 1);
            glareIter = (EnvironmentGlare*)((EnvironmentGlareTexCoord*)glareIter + 1);
            doneCount += 1;
        }
        GXSetNumTevStages((u8)(batchCount * 2));
        GXSetNumTexGens((u8)batchCount);
        GXBegin(GX_QUADS, GX_VTXFMT7, 4);
        GXPosition3s16(sx, sy, 0);
        GXTexCoord2u16(0, 0);
        GXPosition3s16((s16)x1v, sy, 0);
        GXTexCoord2u16(1, 0);
        GXPosition3s16((s16)x1v, (s16)y1v, 0);
        GXTexCoord2u16(1, 1);
        GXPosition3s16(sx, (s16)y1v, 0);
        GXTexCoord2u16(0, 1);
    }
}

void envGlareFilter(s32 type, s32 y0, s32 x0, s32 y1, s32 x1) {
    EnvironmentWork* wp = envGetWork();
    EnvironmentGlareFilter* f = &wp->filter;
    s16 sx1, sy1, sx0, sy0;

    if (type == 0) {
        f->type = 0;
        f->x1 = 0;
        f->y1 = 0;
        f->x0 = 0;
        f->y0 = 0;
        wp->flags &= ~ENV_FLAG_GLARE_FILTER;
        return;
    }
    sx0 = x0 - (x0 & 1);
    sy0 = y0 - (y0 & 1);
    sx1 = x1 + (x1 & 1);
    sy1 = y1 + (y1 & 1);
    if (sx0 < 0) {
        sx0 = 0;
    }
    if (sy0 < 0) {
        sy0 = 0;
    }
    if (sx1 > gp->fbWidth) {
        sx1 = gp->fbWidth;
    }
    if (sy1 > gp->efbHeight) {
        sy1 = gp->efbHeight;
    }
    f->type = type;
    f->y0 = sy0;
    f->x0 = sx0;
    f->y1 = sy1;
    f->x1 = sx1;
    f->zscale = 0.0f;
    f->flags &= ~ENV_GLARE_FILTER_Z;
    wp->flags |= ENV_FLAG_GLARE_FILTER;
}

void envGlareFilterZ(f32 zscale) {
    EnvironmentWork* wp = envGetWork();
    EnvironmentGlareFilter* f = &wp->filter;

    f->zscale = zscale;
    f->flags |= ENV_GLARE_FILTER_Z;
}

#pragma opt_propagation off
#pragma opt_lifetimes off
void envDisp_FF(void) {
    EnvironmentWork* wp = envGetWork();
    f32 r;
    f32 angle;
    f32 c;
    f32 s;
    f32 ofs;
    s32 i;

    if (wp->flags & ENV_FLAG_FF_UNUSED) {
        GXSetNumChans(0);
        GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
        GXSetNumTevStages(1);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
        GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
        GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
        GXLoadTexObj(&wp->captureObj, GX_TEXMAP0);
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
        GXSetZCompLoc(GX_TRUE);
        GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
        GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
        GXSetLineWidth(16, GX_TO_ZERO);
        GXLoadPosMtxImm(camGetPtr(CAMERA_2D)->view, 0);
        GXSetCurrentMtx(0);
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
        r = sqrtf(gp->fbWidth / 2 * gp->fbWidth / 2 + gp->efbHeight / 2 * gp->efbHeight / 2);
        GXBegin(GX_LINES, GX_VTXFMT0, 2000);
        for (i = 0; i < 1000; i++) {
            angle = 6.2831855f * i / 1000.0f;
            c = env_cos(angle);
            s = env_sin(angle);
            ofs = (rand() % 10) / 100.0f;
            GXPosition3f32(s * r, c * r, 0.0f);
            GXTexCoord2f32(s / 2.0f + 0.5f + ofs, -c / 2.0f + 0.5f + ofs);
            GXPosition3f32(0.0f, 0.0f, 0.0f);
            GXTexCoord2f32(0.5f, 0.5f);
        }
    }
}

#pragma opt_propagation reset
#pragma opt_lifetimes reset

double acos(double);

void envTevLoadTexMtxImm(s32 type, GXTexMtx id, GXTexMtx id2, GXTexMtxType mtxType) {
    static f32 a = 0.0f;
    EnvironmentWork* wp = envGetWork();
    Mtx m0, m2, m1, m3, m4, m5, m6, m7;
    Mtx44 m9;
    Mtx m10, m11, m8;
    f32 dot;

    switch (type) {
        case 0:
            PSMTXRotRad(m0, 'Y', 0.017453292f * wp->texMtxWork.rotY);
            PSMTXRotRad(m1, 'Z', 0.017453292f * -wp->texMtxWork.rotX);
            PSMTXConcat(m0, m1, m0);
            PSMTXInvXpose(m0, m0);
            PSVECNormalize((Vec*)m0[0], (Vec*)m0[0]);
            PSVECNormalize((Vec*)m0[1], (Vec*)m0[1]);
            PSVECNormalize((Vec*)m0[2], (Vec*)m0[2]);
            PSMTXScale(m2, 0.5f, 0.5f, 1.0f);
            PSMTXConcat(m2, m0, m0);
            PSMTXTrans(m2, 0.5f, 0.5f, 0.0f);
            PSMTXConcat(m2, m0, m0);
            GXLoadTexMtxImm(m0, id, mtxType);
            break;
        case 1:
            PSMTXRotRad(m0, 'Y', (-1.5707964f));
            PSMTXConcat(m0, wp->texMtxWork.model, m0);
            PSMTXInvXpose(m0, m0);
            PSVECNormalize((Vec*)m0[0], (Vec*)m0[0]);
            PSVECNormalize((Vec*)m0[1], (Vec*)m0[1]);
            PSVECNormalize((Vec*)m0[2], (Vec*)m0[2]);
            PSMTXScale(m3, 0.5f - 0.25f * wp->texMtxWork.scale, 0.5f, 1.0f);
            PSMTXConcat(m3, m0, m0);
            PSMTXTrans(m3, 0.5f, 0.5f, 0.0f);
            PSMTXConcat(m3, m0, m0);
            GXLoadTexMtxImm(m0, id, mtxType);
            break;
        case 2: {
                Vec v = {0.0f, 0.0f, -1.0f};
                Vec light;
                Vec half;
                f32 ry2, rx2, s2;

                ry2 = 3.1415927f * wp->texMtxWork.rotY / 180.0f;
                rx2 = 3.1415927f * wp->texMtxWork.rotX / 180.0f;
                light.x = -(f32)cos(rx2) * (f32)sin(ry2);
                light.y = -(f32)sin(rx2);
                light.z = -(f32)cos(rx2) * (f32)cos(ry2);
                PSMTXMultVecSR(wp->texMtxWork.view, &light, &light);
                C_VECHalfAngle(&light, &v, &half);
                PSMTXInvXpose(wp->texMtxWork.model, m0);
                PSVECNormalize((Vec*)m0[0], (Vec*)m0[0]);
                PSVECNormalize((Vec*)m0[1], (Vec*)m0[1]);
                PSVECNormalize((Vec*)m0[2], (Vec*)m0[2]);
                PSMTXIdentity(m4);
                m4[0][0] = half.x;
                m4[0][1] = half.y;
                m4[0][2] = half.z;
                PSMTXConcat(m4, m0, m0);
                s2 = 20.0f * wp->texMtxWork.scale;
                PSMTXScale(m4, s2, 1.0f, 1.0f);
                PSMTXConcat(m4, m0, m0);
                PSMTXTrans(m4, 1.0f - s2, 0.0f, 0.0f);
                PSMTXConcat(m4, m0, m0);
                GXLoadTexMtxImm(m0, id, mtxType);
                break;
        }
        case 3: {
                Vec light2;
                Vec h;
                Vec axis;
                Vec v2 = {0.0f, 0.0f, 1.0f};
                f32 ry, rx;

                ry = 3.1415927f * wp->texMtxWork.rotY / 180.0f;
                rx = 3.1415927f * wp->texMtxWork.rotX / 180.0f;
                light2.x = (f32)cos(rx) * (f32)sin(ry);
                light2.y = (f32)sin(rx);
                light2.z = (f32)cos(rx) * (f32)cos(ry);
                PSMTXMultVecSR(wp->texMtxWork.view, &light2, &light2);
                dot = PSVECDotProduct(&light2, &v2);
                if (dot == (-1.0f)) {
                    PSMTXScale(m0, 0.0f, 0.0f, 0.0f);
                    GXLoadTexMtxImm(m0, id, GX_MTX2x4);
                    break;
                }
                C_VECHalfAngle(&light2, &v2, &h);
                h.x = -h.x;
                h.y = -h.y;
                h.z = -h.z;
                PSMTXInvXpose(wp->texMtxWork.model, m5);
                PSVECNormalize((Vec*)m5[0], (Vec*)m5[0]);
                PSVECNormalize((Vec*)m5[1], (Vec*)m5[1]);
                PSVECNormalize((Vec*)m5[2], (Vec*)m5[2]);
                if (dot == 1.0f) {
                    PSMTXIdentity(m6);
                } else {
                    PSVECCrossProduct(&h, &v2, &axis);
                    PSMTXRotAxisRad(m6, &axis, (f32)acos(PSVECDotProduct(&v2, &h)));
                }
                PSMTXConcat(m6, m5, m0);
                PSMTXScale(m6, 2.0f * wp->texMtxWork.scale, 2.0f * wp->texMtxWork.scale, 1.0f);
                PSMTXConcat(m6, m0, m0);
                PSMTXTrans(m6, 0.5f, 0.5f, 0.0f);
                PSMTXConcat(m6, m0, m0);
                GXLoadTexMtxImm(m0, id, mtxType);
                break;
        }
        case 4:
            PSMTXInvXpose(wp->texMtxWork.model, m0);
            PSVECNormalize((Vec*)m0[0], (Vec*)m0[0]);
            PSVECNormalize((Vec*)m0[1], (Vec*)m0[1]);
            PSVECNormalize((Vec*)m0[2], (Vec*)m0[2]);
            GXLoadTexMtxImm(m0, id, GX_MTX2x4);
            PSMTXScale(m7, 0.5f, 0.5f, 0.0f);
            PSMTXTrans(m0, 0.5f, 0.5f, 1.0f);
            PSMTXConcat(m0, m7, m0);
            GXLoadTexMtxImm(m0, id2, mtxType);
            break;
        case 5: {
                Vec camPos;
                Vec target;
                Vec up;

                a += 0.0001f;
                camPos = (Vec){10.0f * (f32)sin(a), 100.0f, 10.0f * (f32)cos(a)};
                target = (Vec){10.0f * (f32)sin(a), 0.0f, 10.0f * (f32)cos(a)};
                up = (Vec){0.0f, 0.0f, (-1.0f)};
                C_MTXLookAt(m8, &camPos, &up, &target);
                C_MTXLightFrustum((MtxPtr)m9, (-32.0f), 32.0f, (-32.0f), 32.0f, 300.0f, 0.5f, 0.5f, 0.5f, 0.5f);
                PSMTXInverse(wp->texMtxWork.view, m10);
                PSMTXConcat(m8, m10, m11);
                PSMTXConcat((MtxPtr)m9, m11, m0);
                GXLoadTexMtxImm(m0, id2, mtxType);
                break;
        }
    }
}

static inline void
envTexObj(TEXPalette* pal, GXTexObj* to, u32 id) {
    GXBool mipMap;
    TEXDescriptor* tdp;

    tdp = TEXGet(pal, id);
    if (tdp->textureHeader->minLOD == tdp->textureHeader->maxLOD) {
        mipMap = GX_FALSE;
    } else {
        mipMap = GX_TRUE;
    }
    GXInitTexObj(to, tdp->textureHeader->data, tdp->textureHeader->width, tdp->textureHeader->height, (GXTexFmt)tdp->textureHeader->format, GX_CLAMP, GX_CLAMP, mipMap);
    GXInitTexObjLOD(to, tdp->textureHeader->minFilter, tdp->textureHeader->magFilter, tdp->textureHeader->minLOD, tdp->textureHeader->maxLOD, tdp->textureHeader->LODBias, GX_DISABLE, tdp->textureHeader->edgeLODEnable, GX_ANISO_1);
}

static inline void
envTexObjCI(TEXPalette* pal, GXTexObj* to, GXTlutObj* tlo, GXTlut tluts, u32 id) {
    GXBool mipMap;
    TEXDescriptor* tdp;

    tdp = TEXGet(pal, id);
    if (tdp->textureHeader->minLOD == tdp->textureHeader->maxLOD) {
        mipMap = GX_FALSE;
    } else {
        mipMap = GX_TRUE;
    }
    GXInitTlutObj(tlo, tdp->CLUTHeader->data, tdp->CLUTHeader->format, tdp->CLUTHeader->numEntries);
    GXInitTexObjCI(to, tdp->textureHeader->data, tdp->textureHeader->width, tdp->textureHeader->height, (GXCITexFmt)tdp->textureHeader->format, GX_CLAMP, GX_CLAMP, mipMap, tluts);
    GXInitTexObjLOD(to, tdp->textureHeader->minFilter, tdp->textureHeader->magFilter, tdp->textureHeader->minLOD, tdp->textureHeader->maxLOD, tdp->textureHeader->LODBias, GX_DISABLE, tdp->textureHeader->edgeLODEnable, GX_ANISO_1);
}

static inline void
envLoadTex(u32 id, s32 map) {
    GXTexObj obj;
    GXTlutObj tlut;

    if (TEXGet(env_tpl, id)->CLUTHeader) {
        envTexObjCI(env_tpl, &obj, &tlut, GX_TLUT0, id);
        GXLoadTlut(&tlut, GX_TLUT0);
        GXLoadTexObj(&obj, (GXTexMapID)map);
    } else {
        envTexObj(env_tpl, &obj, id);
        GXLoadTexObj(&obj, (GXTexMapID)map);
    }
}

void envAddTev(s32 type, MapTevCallbackInfo* info) {
    s32 stage;
    int map;
    int coord;
    EnvironmentWork *wp = (gp->inBattle) ? (&work[1]) : (&work[0]);
    s32 indStage;
    Mtx mtx;
    stage = info->tevStage;
    map = info->texMap;
    coord = info->texCoord;
    indStage = info->indStage;
    if (type == 0) {
        return;
    }
    PSMTXConcat(camGetCurPtr()->view, info->mtx, mtx);
    wp->texMtxWork.view = camGetCurPtr()->view;
    wp->texMtxWork.mtx = info->mtx;
    wp->texMtxWork.model = mtx;
    GXSetTevOrder((GXTevStageID) stage,  coord, (GXTexMapID) map, GX_COLOR_NULL);
    GXSetTevColorOp((GXTevStageID) stage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp((GXTevStageID) stage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevSwapMode((GXTevStageID) stage, GX_TEV_SWAP0, GX_TEV_SWAP0);
    switch (type) {
        case 0:
            break;

        case 1:
            GXSetTevColorIn((GXTevStageID) stage, GX_CC_ZERO, GX_CC_CPREV, GX_CC_TEXC, GX_CC_ZERO);
            GXSetTevAlphaIn((GXTevStageID) stage, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
            wp->texMtxWork.rotY = 10.0f;
            wp->texMtxWork.rotX = gp->retraceCount + 45;
            wp->texMtxWork.scale = 0.5f;
            envLoadTex(6, map);
            GXSetTexCoordGen2((GXTexCoordID) coord, GX_TG_MTX2x4, GX_TG_NRM, texmtx_tbl[map], GX_FALSE, GX_PTIDENTITY);
            envTevLoadTexMtxImm(0, (GXTexMtx) texmtx_tbl[map], GX_PTTEXMTX0, GX_MTX2x4);
            stage++;
            coord++;
            map++;
            break;

        case 2:
            GXSetTevColorIn((GXTevStageID) stage, GX_CC_ZERO, GX_CC_CPREV, GX_CC_TEXC, GX_CC_TEXA);
            GXSetTevAlphaIn((GXTevStageID) stage, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
            wp->texMtxWork.rotY = 10.0f;
            wp->texMtxWork.rotX = gp->retraceCount + 45;
            wp->texMtxWork.scale = 0.5f;
            envLoadTex(8, map);
            GXSetTexCoordGen2((GXTexCoordID) coord, GX_TG_MTX2x4, GX_TG_NRM, texmtx_tbl[map], GX_FALSE, GX_PTIDENTITY);
            envTevLoadTexMtxImm(0, (GXTexMtx) texmtx_tbl[map], GX_PTTEXMTX0, GX_MTX2x4);
            stage++;
            coord++;
            map++;
            break;

        case 3:
            GXSetTevColorIn((GXTevStageID) stage, GX_CC_CPREV, GX_CC_ZERO, GX_CC_TEXC, GX_CC_ZERO);
            GXSetTevAlphaIn((GXTevStageID)stage, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
            wp->texMtxWork.scale = 0.5f;
            envLoadTex(0, map);
            GXSetTexCoordGen2((GXTexCoordID) coord, GX_TG_MTX2x4, GX_TG_NRM, texmtx_tbl[map], GX_FALSE, GX_PTIDENTITY);
            envTevLoadTexMtxImm(1, (GXTexMtx) texmtx_tbl[map], GX_PTTEXMTX0, GX_MTX2x4);
            stage++;
            map++;
            coord++;
            break;

        case 4:
            GXSetTevColorIn((GXTevStageID) stage, GX_CC_CPREV, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
            GXSetTevAlphaIn((GXTevStageID) stage, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
            wp->texMtxWork.scale = 0.5f;
            wp->texMtxWork.rotY = 45.0f;
            wp->texMtxWork.rotX = 45.0f;
            envLoadTex(1, map);
            GXSetTexCoordGen2((GXTexCoordID) coord, GX_TG_MTX2x4, GX_TG_NRM, texmtx_tbl[map], GX_FALSE, GX_PTIDENTITY);
            envTevLoadTexMtxImm(2, (GXTexMtx) texmtx_tbl[map], GX_PTTEXMTX0, GX_MTX2x4);
            stage++;
            map++;
            coord++;
            break;

        case 5:
            GXSetTevColorIn((GXTevStageID) stage, GX_CC_CPREV, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
            GXSetTevAlphaIn((GXTevStageID) stage, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
            wp->texMtxWork.scale = 0.5f;
            wp->texMtxWork.rotY = 45.0f;
            wp->texMtxWork.rotX = 45.0f;
            envLoadTex(2, map);
            GXSetTexCoordGen2((GXTexCoordID) coord, GX_TG_MTX2x4, GX_TG_NRM, texmtx_tbl[map], GX_FALSE, GX_PTIDENTITY);
            envTevLoadTexMtxImm(3, (GXTexMtx) texmtx_tbl[map], GX_PTTEXMTX0, GX_MTX2x4);
            stage++;
            map++;
            coord++;
            break;

        case 6:
            GXSetTevColorIn((GXTevStageID) stage, GX_CC_CPREV, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
            GXSetTevAlphaIn((GXTevStageID) stage, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
            wp->texMtxWork.scale = 0.15f;
            wp->texMtxWork.rotY = 0.0f;
            wp->texMtxWork.rotX = 90.0f;
            envLoadTex(4, map);
            GXSetTexCoordGen2((GXTexCoordID) coord, GX_TG_MTX2x4, GX_TG_NRM, texmtx_tbl[map], GX_FALSE, GX_PTIDENTITY);
            envTevLoadTexMtxImm(2, (GXTexMtx) texmtx_tbl[map], GX_PTTEXMTX0, GX_MTX2x4);
            stage++;
            map++;
            coord++;
            break;

        case 7:
            GXSetTevColorIn((GXTevStageID) stage, GX_CC_CPREV, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
            GXSetTevAlphaIn((GXTevStageID) stage, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
            wp->texMtxWork.scale = 0.3f;
            wp->texMtxWork.rotY = 0.0f;
            wp->texMtxWork.rotX = 90.0f;
            envLoadTex(5, map);
            GXSetTexCoordGen2((GXTexCoordID) coord, GX_TG_MTX2x4, GX_TG_NRM, texmtx_tbl[map], GX_FALSE, GX_PTIDENTITY);
            envTevLoadTexMtxImm(3, (GXTexMtx) texmtx_tbl[map], GX_PTTEXMTX0, GX_MTX2x4);
            stage++;
            map++;
            coord++;
            break;

        case 8:
            GXSetTevColorIn((GXTevStageID) stage, GX_CC_ZERO, GX_CC_CPREV, GX_CC_TEXC, GX_CC_ZERO);
            GXSetTevAlphaIn((GXTevStageID) stage, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
            wp->texMtxWork.scale = 0.5f;
            wp->texMtxWork.rotY = 10.0f;
            wp->texMtxWork.rotX = 45.0f;
            envLoadTex(10, map);
            GXSetTexCoordGen2((GXTexCoordID) coord, GX_TG_MTX2x4, GX_TG_NRM, texmtx_tbl[map], GX_FALSE, GX_PTIDENTITY);
            envTevLoadTexMtxImm(0, (GXTexMtx) texmtx_tbl[map], GX_PTTEXMTX0, GX_MTX2x4);
            stage++;
            map++;
            coord++;
            break;

        case 9:
            break;

        case 10:
            GXSetTevColorIn((GXTevStageID) stage, GX_CC_CPREV, GX_CC_TEXC, GX_CC_KONST, GX_CC_ZERO);
            GXSetTevAlphaIn((GXTevStageID) stage, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
            GXSetTevKColorSel((GXTevStageID) stage, GX_TEV_KCSEL_K1);
            GXSetTevKColor(GX_KCOLOR1, (GXColor){0x7F, 0x7F, 0x7F, 0x00});
            envLoadTex(9, map);
            GXSetTexCoordGen2((GXTexCoordID) coord, GX_TG_MTX3x4, GX_TG_NRM, texmtx_tbl[map], GX_FALSE, GX_PTTEXMTX0);
            envTevLoadTexMtxImm(4, (GXTexMtx) texmtx_tbl[map], GX_PTTEXMTX0, GX_MTX3x4);
            stage++;
            map++;
            coord++;
            break;

    }

    info->tevStage = stage;
    info->texCoord = coord;
    info->indStage = indStage;
    info->texMap = map;
}

#undef ENV_TEX_TAIL

// FAKE: one-use inline helper; writing the call out changes the object in both versions
static inline MapObject* envWaterObj(MapTevCallbackInfo* info) {
    return info->mapObj;
}
static inline MapObject* envWaterParent(MapObject* o) { return o->parent; }
void envSetWater(MapTevCallbackInfo* info) {
    MapObject* obj;
    GXTevStageID stage;
    GXTexMapID map;
    GXTexCoordID coord;
    GXIndTexStageID ind;
    u8* buf2;
    s32 i;
    s32 j;
    s32 h;
    s32 w;
    s32 hh;
    s32 hw;
    void* buf;
    f32 scale;
    f32 t;
    Mtx mtx;
    GXTexObj obj1;
    GXTexObj obj2;
    f32 indMtx[2][3];
    Vec pos;
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;

    obj = envWaterObj(info);
    stage = info->tevStage;
    map = info->texMap;
    coord = info->texCoord;
    ind = info->indStage;
    if (obj->flags & MAP_OBJ_FLAG_GROUP_MEMBER) {
        while ((obj = envWaterParent(obj)) != NULL) {
            if (obj->flags & MAP_OBJ_FLAG_GROUP) {
                if (!(obj->flags & MAP_OBJ_FLAG_SCREEN_RECT)) {
                    obj->flags |= MAP_OBJ_FLAG_SCREEN_RECT;
                    return;
                }
                x0 = obj->screenRect[0];
                y0 = obj->screenRect[1];
                x1 = obj->screenRect[2];
                y1 = obj->screenRect[3];
                break;
            }
        }
    } else {
        if (!(obj->flags & MAP_OBJ_FLAG_SCREEN_RECT)) {
            obj->flags |= MAP_OBJ_FLAG_SCREEN_RECT;
            return;
        }
        x0 = obj->screenRect[0];
        y0 = obj->screenRect[1];
        x1 = obj->screenRect[2];
        y1 = obj->screenRect[3];
    }
    if (x0 == 0x3FF || y0 == 0x3FF || x1 == 0x3FF || y1 == 0x3FF) {
        return;
    }
    if (x0 == 0 && y0 == 0 && x1 == 0 && y1 == 0) {
        return;
    }
    x0 -= x0 & 1;
    y0 -= y0 & 1;
    x1 += x1 & 1;
    y1 += y1 & 1;
    if (x0 < 0) {
        x0 = 0;
    }
    if (y0 < 0) {
        y0 = 0;
    }
    if (x1 > gp->fbWidth) {
        x1 = gp->fbWidth;
    }
    if (y1 > gp->efbHeight) {
        y1 = gp->efbHeight;
    }
    h = y1 - y0;
    w = x1 - x0;
    hw = w / 2;
    hh = h / 2;
    buf = smartAlloc(GXGetTexBufferSize((u16)hw, (u16)hh, GX_TF_RGB565, GX_FALSE, 0), 3)->address;
    GXSetTexCopySrc((u16)x0, (u16)y0, (u16)w, (u16)h);
    GXSetTexCopyDst((u16)hw, (u16)hh, GX_TF_RGB565, GX_TRUE);
    GXCopyTex(buf, GX_FALSE);
    GXPixModeSync();
    GXTexModeSync();
    GXInitTexObj(&obj1, buf, (u16)hw, (u16)hh, GX_TF_RGB565, GX_CLAMP, GX_CLAMP, GX_FALSE);
    GXInitTexObjLOD(&obj1, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    PSMTXMultVec(info->mtx, &info->mapObj->center, &pos);
    scale = 1000.0f / PSVECDistance(&camGetCurPtr()->cameraPos, &pos);
    if (scale < 0.0f) {
        scale = 0.0f;
    }
    if (scale > 127.0f) {
        scale = 127.0f;
    }
    buf2 = smartAlloc(GXGetTexBufferSize(64, 64, GX_TF_IA8, GX_FALSE, 0), 3)->address;
    for (i = 0; i < 64; i++) {
        for (j = 0; j < 64; j++) {
            f32 s = sin((f32)gp->retraceCount / 10.0f + (18.849552f * j) / 64.0f);
            s32 q;
            s32 o;
            u8* p;
            q = i / 4;
            o = q * 32 + (j / 4) * 512;
            q = i % 4;
            o += ((j % 4) * 4 + q) * 2;
            p = buf2 + o;
            p[0] = 0.6f + (scale * s + 128.0f);
            p[1] = 0x80;
        }
    }
    DCFlushRange(buf2, 0x2000);
    GXInitTexObj(&obj2, buf2, 64, 64, GX_TF_IA8, GX_REPEAT, GX_REPEAT, GX_FALSE);
    GXInitTexObjLOD(&obj2, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    t = tan(6.2831855f * (camGetPtr(CAMERA_3D)->fovY / 2.0f) / 360.0f);
    C_MTXLightFrustum(mtx, gp->efbHeight / 2 - y1, gp->efbHeight / 2 - y0, x0 - gp->fbWidth / 2, x1 - gp->fbWidth / 2,
                      gp->efbHeight / 2.0f / t, 0.5f, 0.5f, 0.5f, 0.5f);
    PSMTXConcat(mtx, camGetCurPtr()->view, mtx);
    PSMTXConcat(mtx, info->mtx, mtx);
    GXSetTexCoordGen2((GXTexCoordID)coord, GX_TG_MTX3x4, GX_TG_POS, texmtx_tbl[map], GX_FALSE, GX_PTIDENTITY);
    GXLoadTexMtxImm(mtx, texmtx_tbl[map], GX_MTX3x4);
    GXLoadTexObj(&obj1, (GXTexMapID)map);
    GXLoadTexObj(&obj2, (GXTexMapID)(map + 1U));
    GXSetTevOrder((GXTevStageID)stage, (GXTexCoordID)coord, (GXTexMapID)map, GX_COLOR_NULL);
    GXSetTevColorOp((GXTevStageID)stage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp((GXTevStageID)stage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn((GXTevStageID)stage, GX_CC_TEXC, GX_CC_CPREV, GX_CC_APREV, GX_CC_ZERO);
    GXSetTevAlphaIn((GXTevStageID)stage, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
    GXSetTevSwapMode((GXTevStageID)stage, GX_TEV_SWAP0, GX_TEV_SWAP0);
    indMtx[0][0] = 0.5f;
    indMtx[0][1] = 0.0f;
    indMtx[0][2] = 0.0f;
    indMtx[1][0] = 0.0f;
    indMtx[1][1] = 0.5f;
    indMtx[1][2] = 0.0f;
    GXSetIndTexMtx(GX_ITM_0, indMtx, 1);
    GXSetIndTexOrder((GXIndTexStageID)ind, (GXTexCoordID)coord, (GXTexMapID)(1 + map));
    GXSetIndTexCoordScale((GXIndTexStageID)ind, GX_ITS_1, GX_ITS_1);
    GXSetTevIndWarp((GXTevStageID)stage, (GXIndTexStageID)ind, GX_TRUE, GX_FALSE, GX_ITM_0);
    stage++;
    coord++;
    ind++;
    map += 2;
    info->tevStage = stage;
    info->texCoord = coord;
    info->indStage = ind;
    info->texMap = map;
}

void envSetYamiView(Mtx view) {
    EnvironmentWork* wp = envGetWork();

    PSMTXCopy(view, wp->yamiView);
}

void envSetYami(MapTevCallbackInfo* info) {
    static f32 u1 = 0.0f;
    static f32 u2 = 0.0f;
    static u32 v_count = 0;
    EnvironmentWork* wp;
    GXTevStageID stage;
    GXTexMapID map;
    GXTexCoordID coord;
    s32 unk18;
    GXTexObj obj;
    Mtx mtx;
    u32 rc;

    wp = envGetWork();
    stage = info->tevStage;
    map = info->texMap;
    coord = info->texCoord;
    unk18 = info->indStage;

    if (v_count != (rc = gp->retraceCount)) {
        u1 += 0.004f;
        u2 -= 0.004f;
        v_count = rc;
        if (u1 > 10.0f) {
            u1 -= 10.0f;
        }
        if (u2 < 10.0f) {
            u2 += 10.0f;
        }
    }
    GXSetTevColorOp(stage - 1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVREG0);
    GXSetTevAlphaOp(stage - 1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVREG0);
    C_MTXLightOrtho(mtx, 72.0f, (-72.0f), (-96.0f), 96.0f, 0.5f, 0.5f, u1, 0.5f);
    PSMTXConcat(mtx, wp->yamiView, mtx);
    PSMTXConcat(mtx, info->mtx, mtx);
    GXLoadTexMtxImm(mtx, texmtx_tbl[map], GX_MTX3x4);
    GXSetTexCoordGen2(coord, GX_TG_MTX3x4, GX_TG_POS, texmtx_tbl[map], GX_FALSE, GX_PTIDENTITY);
    C_MTXLightOrtho(mtx, 72.0f, (-72.0f), (-96.0f), 96.0f, 0.5f, 0.5f, u2, 0.5f);
    PSMTXConcat(mtx, wp->yamiView, mtx);
    PSMTXConcat(mtx, info->mtx, mtx);
    GXLoadTexMtxImm(mtx, texmtx_tbl[map + 1], GX_MTX3x4);
    GXSetTexCoordGen2(coord + 1, GX_TG_MTX3x4, GX_TG_POS, texmtx_tbl[map + 1], GX_FALSE, GX_PTIDENTITY);
    TEXGetGXTexObjFromPalette(env_tpl, &obj, 9);
    GXLoadTexObj(&obj, map);
    TEXGetGXTexObjFromPalette(env_tpl, &obj, 10);
    GXLoadTexObj(&obj, map + 1);

    GXSetTevOrder(stage, coord, map, GX_COLOR_NULL);
    GXSetTevColorOp(stage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(stage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(stage, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
    GXSetTevAlphaIn(stage, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
    GXSetTevSwapMode(stage, GX_TEV_SWAP0, GX_TEV_SWAP0);

    GXSetTevOrder(stage + 1, coord + 1, map + 1, GX_COLOR_NULL);
    GXSetTevColorOp(stage + 1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(stage + 1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(stage + 1, GX_CC_ZERO, GX_CC_CPREV, GX_CC_TEXA, GX_CC_ZERO);
    GXSetTevAlphaIn(stage + 1, GX_CA_ZERO, GX_CA_APREV, GX_CA_TEXA, GX_CA_ZERO);
    GXSetTevSwapMode(stage + 1, GX_TEV_SWAP0, GX_TEV_SWAP0);

    GXSetTevOrder(stage + 2, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevColorOp(stage + 2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(stage + 2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(stage + 2, GX_CC_C0, GX_CC_ZERO, GX_CC_APREV, GX_CC_ZERO);
    GXSetTevAlphaIn(stage + 2, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A0);
    GXSetTevSwapMode(stage + 2, GX_TEV_SWAP0, GX_TEV_SWAP0);

    stage += 3;
    coord += 2;
    map += 2;
    info->tevStage = stage;
    info->texCoord = coord;
    info->indStage = unk18;
    info->texMap = map;
}

