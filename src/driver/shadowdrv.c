#include "driver/shadowdrv.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/hitdrv.h"
#include "driver/offscreendrv.h"
#include "mario/mariost.h"
#include "system.h"
#include <string.h>
#include "driver/mobjdrv.h"
#include "eff/eff_n64.h"


void shadowCharShadowDisp_Collision(CameraId cameraId, void* param);
void shadowCharShadowDisp_Projection(CameraId cameraId, void* param);
void shadowCharShadowDisp_Polygon(CameraId cameraId, void* param);
void shadowCharShadowDisp_Texture(CameraId cameraId, void* param);
void cylinderShadowDraw(CameraId cameraId, void* param);
void cylinder(ShadowEntry* entry);
void projShadowEnd(CameraId cameraId, void* param);
void depthShadowEnd(CameraId cameraId, void* param);
void shadowDisp(CameraId cameraId, void* param);
BOOL _filter(HitCheckQuery* query, HitEntry* hit);


static ShadowConfig shadowConfig[2] = {
    {0, {-100.0f, 256.0f, 335.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, -20.0f, 20.0f, 356.0f, 741.0f, 0, 3, 0, -4.47f},
    {1, {135.0f, 40.0f, 41.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, -240.0f, 240.0f, 80.0f, 10000.0f, 1, 3, 0, 0.0f},
};

static f32 testKagePosArray[18] ATTRIBUTE_ALIGN(32) = {
    0.0f, 0.0f, -1.0f,
    0.9f, 0.0f, -0.5f,
    -0.9f, 0.0f, -0.5f,
    0.9f, 0.0f, 0.5f,
    -0.9f, 0.0f, 0.5f,
    0.0f, 0.0f, 1.0f,
};

static u8 cylinder_dl_top[64] ATTRIBUTE_ALIGN(32) = {
    0xA0, 0x00, 0x10, 0x00, 0x00, 0x0F, 0x00, 0x0E, 0x00, 0x0D, 0x00, 0x0C, 0x00, 0x0B, 0x00, 0x0A,
    0x00, 0x09, 0x00, 0x08, 0x00, 0x07, 0x00, 0x06, 0x00, 0x05, 0x00, 0x04, 0x00, 0x03, 0x00, 0x02,
    0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static u8 cylinder_dl_bottom[64] ATTRIBUTE_ALIGN(32) = {
    0xA0, 0x00, 0x10, 0x10, 0x01, 0x11, 0x01, 0x12, 0x01, 0x13, 0x01, 0x14, 0x01, 0x15, 0x01, 0x16,
    0x01, 0x17, 0x01, 0x18, 0x01, 0x19, 0x01, 0x1A, 0x01, 0x1B, 0x01, 0x1C, 0x01, 0x1D, 0x01, 0x1E,
    0x01, 0x1F, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static u8 cylinder_dl_side[64] ATTRIBUTE_ALIGN(32) = {
    0x98, 0x00, 0x1E, 0x11, 0x01, 0x01, 0x02, 0x12, 0x01, 0x02, 0x02, 0x13, 0x01, 0x03, 0x02, 0x14,
    0x01, 0x04, 0x02, 0x15, 0x01, 0x05, 0x02, 0x16, 0x01, 0x06, 0x02, 0x17, 0x01, 0x07, 0x02, 0x18,
    0x01, 0x08, 0x02, 0x19, 0x01, 0x09, 0x02, 0x1A, 0x01, 0x0A, 0x02, 0x1B, 0x01, 0x0B, 0x02, 0x1C,
    0x01, 0x0C, 0x02, 0x1D, 0x01, 0x0D, 0x02, 0x1E, 0x01, 0x0E, 0x02, 0x1F, 0x01, 0x0F, 0x02, 0x00,
};

static u8 cylinder_dl_top_inside[64] ATTRIBUTE_ALIGN(32) = {
    0xA0, 0x00, 0x10, 0x20, 0x00, 0x2F, 0x00, 0x2E, 0x00, 0x2D, 0x00, 0x2C, 0x00, 0x2B, 0x00, 0x2A,
    0x00, 0x29, 0x00, 0x28, 0x00, 0x27, 0x00, 0x26, 0x00, 0x25, 0x00, 0x24, 0x00, 0x23, 0x00, 0x22,
    0x00, 0x21, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static u8 cylinder_dl_top_outside[64] ATTRIBUTE_ALIGN(32) = {
    0x98, 0x00, 0x1E, 0x01, 0x02, 0x21, 0x00, 0x02, 0x02, 0x22, 0x00, 0x03, 0x02, 0x23, 0x00, 0x04,
    0x02, 0x24, 0x00, 0x05, 0x02, 0x25, 0x00, 0x06, 0x02, 0x26, 0x00, 0x07, 0x02, 0x27, 0x00, 0x08,
    0x02, 0x28, 0x00, 0x09, 0x02, 0x29, 0x00, 0x0A, 0x02, 0x2A, 0x00, 0x0B, 0x02, 0x2B, 0x00, 0x0C,
    0x02, 0x2C, 0x00, 0x0D, 0x02, 0x2D, 0x00, 0x0E, 0x02, 0x2E, 0x00, 0x0F, 0x02, 0x2F, 0x00, 0x00,
};

static u8 cube_dl_top[64] ATTRIBUTE_ALIGN(32) = {
    0x80, 0x00, 0x14, 0x30, 0x00, 0x31, 0x00, 0x32, 0x00, 0x33, 0x00, 0x34, 0x02, 0x35, 0x02, 0x31,
    0x00, 0x30, 0x00, 0x31, 0x00, 0x35, 0x02, 0x36, 0x02, 0x32, 0x00, 0x33, 0x00, 0x32, 0x00, 0x36,
    0x02, 0x37, 0x02, 0x34, 0x02, 0x30, 0x00, 0x33, 0x00, 0x37, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static f32 vertex_tbl[180] ATTRIBUTE_ALIGN(32) = {
    0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f,
    0.433884f, 0.0f, 0.900969f,
    0.781832f, 0.0f, 0.62349f,
    0.974928f, 0.0f, 0.222521f,
    0.974928f, 0.0f, -0.222521f,
    0.781831f, 0.0f, -0.62349f,
    0.433884f, 0.0f, -0.900969f,
    -0.0f, 0.0f, -1.0f,
    -0.433884f, 0.0f, -0.900969f,
    -0.781832f, 0.0f, -0.62349f,
    -0.974928f, 0.0f, -0.222521f,
    -0.974928f, 0.0f, 0.222521f,
    -0.781832f, 0.0f, 0.62349f,
    -0.433884f, 0.0f, 0.900969f,
    0.0f, 0.0f, 1.0f,
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 1.0f,
    0.433884f, -1.0f, 0.900969f,
    0.781832f, -1.0f, 0.62349f,
    0.974928f, -1.0f, 0.222521f,
    0.974928f, -1.0f, -0.222521f,
    0.781831f, -1.0f, -0.62349f,
    0.433884f, -1.0f, -0.900969f,
    -0.0f, -1.0f, -1.0f,
    -0.433884f, -1.0f, -0.900969f,
    -0.781832f, -1.0f, -0.62349f,
    -0.974928f, -1.0f, -0.222521f,
    -0.974928f, -1.0f, 0.222521f,
    -0.781832f, -1.0f, 0.62349f,
    -0.433884f, -1.0f, 0.900969f,
    0.0f, -1.0f, 1.0f,
    0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.6f,
    0.2603304f, 0.0f, 0.54058146f,
    0.4690992f, 0.0f, 0.374094f,
    0.5849568f, 0.0f, 0.13351262f,
    0.5849568f, 0.0f, -0.13351262f,
    0.46909863f, 0.0f, -0.374094f,
    0.2603304f, 0.0f, -0.54058146f,
    0.0f, 0.0f, -0.6f,
    -0.2603304f, 0.0f, -0.54058146f,
    -0.4690992f, 0.0f, -0.374094f,
    -0.5849568f, 0.0f, -0.13351262f,
    -0.5849568f, 0.0f, 0.13351262f,
    -0.4690992f, 0.0f, 0.374094f,
    -0.2603304f, 0.0f, 0.54058146f,
    0.0f, 0.0f, 0.6f,
    -0.6f, 0.0f, 0.6f,
    -0.6f, 0.0f, -0.6f,
    0.6f, 0.0f, -0.6f,
    0.6f, 0.0f, 0.6f,
    -1.0f, 0.0f, 1.0f,
    -1.0f, 0.0f, -1.0f,
    1.0f, 0.0f, -1.0f,
    1.0f, 0.0f, 1.0f,
    -1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, 1.0f,
};


//.bss
CharShadowWork char_shadow_work;
ProjShadowWork proj_shadow_work;
DepthShadowWork depth_shadow_work;

//.sbss
GXTexObj rampTex8;
GXTexObj rampTex16;
f32 tmp_vp[6];
u32 tmp_sci[4];
Mtx tmp_view;

//.sdata
CharShadowWork* cswp = &char_shadow_work;
ProjShadowWork* pswp = &proj_shadow_work;
DepthShadowWork* dswp = &depth_shadow_work;
u16 ShadowSizeTbl[4] = {0x190, 0x100, 0x80, 0x40};
u8 cube_dl_bottom[32] ATTRIBUTE_ALIGN(32) = {
    0x80, 0x00, 0x04, 0x38, 0x01, 0x39, 0x01, 0x3A, 0x01, 0x3B, 0x01, 0x00,
};
u8 cube_dl_side[32] ATTRIBUTE_ALIGN(32) = {
    0x98, 0x00, 0x0A, 0x35, 0x02, 0x38, 0x01, 0x36, 0x02, 0x39, 0x01, 0x37,
    0x02, 0x3A, 0x01, 0x38, 0x02, 0x3B, 0x01, 0x39, 0x02, 0x38, 0x01, 0x00,
};
static GXColor color_tbl[3] ATTRIBUTE_ALIGN(32) = {
    {0x00, 0x00, 0x00, 0x6E}, {0x00, 0x00, 0x00, 0x00}, {0x00, 0x00, 0x00, 0x16},
};
GXColor color_tbl_nodecay[3] ATTRIBUTE_ALIGN(32) = {
    {0x00, 0x00, 0x00, 0x6E}, {0x00, 0x00, 0x00, 0x6E}, {0x00, 0x00, 0x00, 0x6E},
};
GXColor color_tbl_cube[3] ATTRIBUTE_ALIGN(32) = {
    {0x00, 0x00, 0x00, 0x8E}, {0x00, 0x00, 0x00, 0x00}, {0x00, 0x00, 0x00, 0x16},
};

#pragma explicit_zero_data on
#pragma explicit_zero_data reset





static inline void depthShadowInit(void);
static inline void projShadowInit(void);


void shadowInit(void) {
    u32 bufferSize;
    CharShadowWork* wp;
    void* ramp;
    u32 i;

    cswp->flags = 0;
    cswp->drawCount = 0;
    cswp->configIdx = 1;
    cswp->color = (GXColor){0, 0, 0, 0x6E};
    cswp->projection = 1;
    cswp->defaultMode = 2;
    cswp->defaultSizeY = 20.0f;
    cswp->defaultEnabled = 1;
    cswp->numEntries = 0;
    cswp->entries = __memAlloc(HEAP_DEFAULT, sizeof(ShadowEntry) * 64);
    memset(cswp->entries, 0, sizeof(ShadowEntry) * 64);
    wp = cswp;
    bufferSize = GXGetTexBufferSize(ShadowSizeTbl[0], ShadowSizeTbl[0], 2, 0, 0);
    wp->texBuffer = smartAlloc(bufferSize, 0);
    depthShadowInit();

    {
        bufferSize = GXGetTexBufferSize(0x10, 0x10, GX_TF_I8, GX_FALSE, 0);
        ramp = __memAlloc(HEAP_DEFAULT, GXGetTexBufferSize(0x10, 0x10, GX_TF_I8, GX_FALSE, 0));
        for (i = 0; i < 256; i++) {
            ((u8*)ramp)[(((i & 0x3) * 8) + (((i & 0xC) * 16) + (((i & 0x80) / 4) + ((i & 0x70) / 16))))] = i;
        }
        GXInitTexObj(&rampTex8, ramp, 0x10, 0x10, GX_TF_I8, GX_CLAMP, GX_REPEAT, GX_FALSE);
        GXInitTexObjLOD(&rampTex8, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
        DCFlushRange(ramp, bufferSize);
    }
    {
        u32 j;

        i = GXGetTexBufferSize(0x100, 0x100, GX_TF_IA8, GX_FALSE, 0);
        ramp = __memAlloc(HEAP_DEFAULT, GXGetTexBufferSize(0x100, 0x100, GX_TF_IA8, GX_FALSE, 0));
        for (j = 0; j < 0x10000; j++) {
            ((u16*)ramp)[(((j & 0x3) * 4) + (((j & 0xFC) * 256) + (((j & 0xFC00) / 64) + ((j & 0x300) / 256))))] = j;
        }
        GXInitTexObj(&rampTex16, ramp, 0x100, 0x100, GX_TF_IA8, GX_CLAMP, GX_REPEAT, GX_FALSE);
        GXInitTexObjLOD(&rampTex16, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
        DCFlushRange(ramp, i);
    }
    projShadowInit();
}

void shadowMain(void) {
    dispEntry(CAMERA_3D, 0.0f, 9, shadowDisp, NULL);
    dispEntry(CAMERA_OFFSCREEN2, 0.0f, 9, shadowDisp, NULL);
    dispEntry(CAMERA_3D_EFFECTA, 0.0f, 9, shadowDisp, NULL);
    dispEntry(CAMERA_3D_EFFECTB, 0.0f, 9, shadowDisp, NULL);
    if (cswp->projection) {
        dispEntry(CAMERA_SHADOW, 0.0f, 5, shadowDisp, NULL);
    }
    dispEntry(CAMERA_SHADOW, 0.0f, 0, shadowDisp, NULL);
    dispEntry(CAMERA_SHADOW, 0.0f, 2, shadowDisp, NULL);
    dispEntry(CAMERA_SHADOW, 0.0f, 7, shadowDisp, NULL);
    cswp->numEntries = 0;
    cswp->flags &= ~SHADOW_WORK_READY;
    cswp->drawCount = 0;
    pswp->flags &= ~SHADOW_WORK_READY;
    pswp->drawCount = 0;
    dswp->flags &= ~SHADOW_WORK_READY;
    dswp->drawCount = 0;
}

static inline void shadowSetupPolygon(void) {
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
    GXSetChanMatColor(GX_COLOR0A0, cswp->color);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_RASC);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_RASA, GX_CA_ZERO);
    GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, (GXColor){0, 0, 0, 0});
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
    GXSetZCompLoc(GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetCurrentMtx(GX_PNMTX0);
    GXSetCullMode(GX_CULL_NONE);
}

static inline void shadowBegin(ProjShadowWork* wp, BOOL depth) {
    f32 (*proj)[4];
    ShadowConfig* cfg;
    u16 size;
    Vec pos;

    GXInvalidateTexAll();
    size = ShadowSizeTbl[((ShadowConfig*)((u8*)shadowConfig + wp->configIdx * sizeof(ShadowConfig)))->sizeIdx];
    if (depth) {
        GXSetColorUpdate(GX_FALSE);
    }
    GXGetViewportv(tmp_vp);
    GXGetScissor(&tmp_sci[0], &tmp_sci[1], &tmp_sci[2], &tmp_sci[3]);
    GXSetViewport(0.0f, 0.0f, size, size, 0.0f, 1.0f);
    GXSetScissor(1, 1, size - 2, size - 2);
    // FAKE: comma expression `(0, X)`: X becomes a separate compiler temp; the plain X changes the object
    cfg = (0, &shadowConfig[wp->configIdx]);
    proj = wp->proj;
    pos = cfg->pos;
    C_MTXLookAt(wp->view, &pos, &cfg->up, &cfg->target);
    if (cfg->projType == 0) {
        C_MTXFrustum(proj, cfg->top, -cfg->top, cfg->left, -cfg->left, cfg->near, cfg->far);
    } else {
        C_MTXOrtho(proj, cfg->top, -cfg->top, cfg->left, -cfg->left, cfg->near, cfg->far);
    }
    GXSetProjection(wp->proj, ((ShadowConfig*)((u8*)shadowConfig + wp->configIdx * sizeof(ShadowConfig)))->projType);
    PSMTXCopy(camGetCurPtr()->view, tmp_view);
    PSMTXCopy(wp->view, camGetCurPtr()->view);
}


void shadowDisp(CameraId cameraId, void* param) {
    DispEntry* disp;
    CameraEntry* cam;

    disp = dispGetCurWork();

    if ((( cameraId == CAMERA_OFFSCREEN2 ) || (( (u32)(cameraId - CAMERA_3D) ) <= 1 )) || ( cameraId == CAMERA_3D_EFFECTB) ) {
        shadowCharShadowDisp_Polygon(cameraId, param);
        {
            f32 t;
            cam = camGetCurPtr();
            t = cam->cameraAt.y * 20.0f;
        }
        shadowSetupPolygon();
        GXSetArray(GX_VA_POS, testKagePosArray, 12);
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_INDEX8);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_INDEX8);
        GXSetVtxDesc(GX_VA_CLR0, GX_INDEX8);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
        GXSetArray(GX_VA_POS, vertex_tbl, 12);
        GXSetArray(GX_VA_CLR0, color_tbl, 4);
        cylinderShadowDraw(cameraId, param);
        shadowCharShadowDisp_Texture(cameraId, param);
    } else if (cameraId == CAMERA_SHADOW) {
        switch (disp->renderMode) {
            case 5: {
                ShadowConfig* cfg;
                shadowCharShadowDisp_Collision(cameraId, param);
                if ((cfg = &shadowConfig[cswp->configIdx])->follow == 1) {
                    cam->cameraAt.z = cam->cameraAt.z; // FAKE: self-assignment; removing it changes codegen
                    cam = camGetPtr(CAMERA_3D);
                    cfg->pos.x = cam->cameraAt.x;
                    cfg->pos.y = 1000.0f + cam->cameraAt.y;
                    cfg->pos.z = cam->cameraAt.z;
                    cfg->up.x = 0.0f;
                    cfg->up.y = 0.0f;
                    cfg->up.z = -1.0f;
                    cfg->target.x = cam->cameraAt.x;
                    cfg->target.y = cam->cameraAt.y;
                    cfg->target.z = cam->cameraAt.z;
                }
                shadowBegin((ProjShadowWork*)cswp, FALSE);
                shadowCharShadowDisp_Projection(cameraId, param);
                projShadowEnd(cameraId, cswp);
                break;
            }
            case 0:
                shadowBegin(pswp, FALSE);
                break;
            case 2:
                projShadowEnd(cameraId, pswp);
                shadowBegin((ProjShadowWork*)dswp, TRUE);
                break;
            case 7:
                depthShadowEnd(cameraId, dswp);
                break;
        }
    }
}

#pragma push
#pragma opt_dead_assignments off
BOOL _filter(HitCheckQuery* query, HitEntry* hit) {
    f32 t = 20.0f;

    if ((hitGetAttr(hit) & HIT_ATTR_MOBJ) && (*(u32*)mobjHitObjPtrToPtr(hit) & 0x800)) {
        return FALSE;
    }
    return TRUE;
}
#pragma pop



void shadowCharShadowDisp_Collision(CameraId cameraId, void* param) {
    ShadowEntry* entry = cswp->entries;
    s32 i;
    f32 (*m)[4];
    f32 size;
    HitCheckQuery* q;
    f32 sy;
    HitCheckQuery query;

    camGetCurPtr();
    for (i = 0; i < cswp->numEntries; i++, entry++) {
        m = entry->mtx;
        if (!entry->size) { continue;
        }
        if (entry->flags & SHADOW_ENTRY_PLACED) { continue;
        }
        if ( (entry->mode != 3 ) && (entry->flags & SHADOW_ENTRY_GROUND)) {
            query.targetPos = entry->pos;
            query.targetPos.y += 1.0f;
            query.targetDir = (Vec){0.0f, -1.0f, 0.0f};
            query.targetDistance = 150.0f;
            q = &query;
            if (!hitCheckVecFilter(&query, _filter)) { continue;
            }
            size = entry->size;
            sy = entry->sizeY;
            m[0][3] = q->hitPos.x + q->hitNormal.x;
            m[1][3] = q->hitPos.y + q->hitNormal.y;
            m[2][3] = q->hitPos.z + q->hitNormal.z;
            m[1][0] = query.hitNormal.x * size;
            m[1][1] = query.hitNormal.y * sy;
            m[1][2] = query.hitNormal.z * size;
            m[2][0] = 0.0f;
            m[2][1] = query.hitNormal.z * size;
            m[2][2] = ( -query.hitNormal.y ) * size;
            m[0][0] = size * ((( -query.hitNormal.y ) * query.hitNormal.y ) - ( query.hitNormal.z * query.hitNormal.z) );
            m[0][1] = size * (query.hitNormal.x * query.hitNormal.y);
            m[0][2] = size * (query.hitNormal.x * query.hitNormal.z);
        } else {
            PSMTXTrans(m, entry->pos.x, 1.5f + entry->pos.y, entry->pos.z);
            m[0][0] = ( m[2][2] = entry->size) ;
            m[1][1] = entry->sizeY;
        }
        entry->flags |= SHADOW_ENTRY_PLACED;
    }
}

void shadowCharShadowDisp_Projection(CameraId cameraId, void* param) {
    ShadowConfig* cfg;
    CharShadowWork* wp;
    ShadowEntry* entry;
    f32 (*view)[4];
    u8 alpha;
    s32 i;
    Mtx m;
    f32 w, h, fw, fh;
    f32 cx, cz;

    cfg = &shadowConfig[(wp = cswp)->configIdx];
    entry = wp->entries;
    view = camGetCurPtr()->view;
    alpha = cswp->color.a;
    shadowSetupPolygon();
    GXSetArray(GX_VA_POS, testKagePosArray, 12);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_INDEX8);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetBlendMode(GX_BM_NONE, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    if (gp->inBattle) {
        GXSetChanMatColor(GX_COLOR0A0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
    } else {
        GXSetChanMatColor(GX_COLOR0A0, (GXColor){alpha, alpha, alpha, 0xFF});
    }
    GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
    for (i = 0; i < cswp->numEntries; i++, entry++) {
        if (!entry->size) continue;
        if (entry->mode != 3) continue;
        if (!(entry->flags & SHADOW_ENTRY_PLACED)) continue;
        PSMTXConcat(view, entry->mtx, m);
        GXLoadPosMtxImm(m, GX_PNMTX0);
        GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, 6);
        GXPosition1x8(0);
        GXPosition1x8(1);
        GXPosition1x8(2);
        GXPosition1x8(3);
        GXPosition1x8(4);
        GXPosition1x8(5);
        wp->drawCount++;
    }
    if (wp->drawCount) {
        Mtx44 proj;
        Mtx look;
        C_MTXOrtho(proj, cfg->top, -cfg->top, cfg->left, -cfg->left, cfg->near, cfg->far);
        GXSetProjection(proj, GX_ORTHOGRAPHIC);
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
        GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
        GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
        {
            Vec pos = {0.0f, 1000.0f, 0.0f};
            Vec up = {0.0f, 0.0f, -1.0f};
            Vec target = {0.0f, 0.0f, 0.0f};
            C_MTXLookAt(look, &pos, &up, &target);
        }
        GXLoadPosMtxImm(look, GX_PNMTX0);
        w = -cfg->left;
        h = cfg->top;
        fh = 0.2f * h;
        fw = 0.2f * w;
        GXBegin(GX_QUADS, GX_VTXFMT0, 16);
        cx = 0.0f;
        cz = 0.0f;
        GXPosition3f32((cx - w), cx, (cz - h));
        GXColor4u8(0, 0, 0, 0xFF);
        GXPosition3f32((cx + w), cx, (cz - h));
        GXColor4u8(0, 0, 0, 0xFF);
        GXPosition3f32((cx + w), cx, fh + (cz - h));
        GXColor4u8(0, 0, 0, 0);
        GXPosition3f32((cx - w), cx, fh + (cz - h));
        GXColor4u8(0, 0, 0, 0);
        GXPosition3f32((cx - w), cx, (cz + h));
        GXColor4u8(0, 0, 0, 0xFF);
        GXPosition3f32((cx + w), cx, (cz + h));
        GXColor4u8(0, 0, 0, 0xFF);
        GXPosition3f32((cx + w), cx, (cz + h) - fh);
        GXColor4u8(0, 0, 0, 0);
        GXPosition3f32((cx - w), cx, (cz + h) - fh);
        GXColor4u8(0, 0, 0, 0);
        GXPosition3f32((cx - w), cx, (cz - h));
        GXColor4u8(0, 0, 0, 0xFF);
        GXPosition3f32((cx - w), cx, (cz + h));
        GXColor4u8(0, 0, 0, 0xFF);
        GXPosition3f32(fw + (cx - w), cx, (cz + h));
        GXColor4u8(0, 0, 0, 0);
        GXPosition3f32(fw + (cx - w), cx, (cz - h));
        GXColor4u8(0, 0, 0, 0);
        GXPosition3f32((cx + w), cx, (cz - h));
        GXColor4u8(0, 0, 0, 0xFF);
        GXPosition3f32((cx + w), cx, (cz + h));
        GXColor4u8(0, 0, 0, 0xFF);
        GXPosition3f32((cx + w) - fw, cx, (cz + h));
        GXColor4u8(0, 0, 0, 0);
        GXPosition3f32((cx + w) - fw, cx, (cz - h));
        GXColor4u8(0, 0, 0, 0);
    }
}

void shadowCharShadowDisp_Polygon(CameraId cameraId, void* param) {
    ShadowEntry* entry = cswp->entries;
    f32 (*view)[4];
    s32 i;
    Mtx m;
    u16 left, top, right, bottom;

    view = camGetCurPtr()->view;
    shadowSetupPolygon();
    GXSetArray(GX_VA_POS, testKagePosArray, 12);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_INDEX8);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    for (i = 0; i < cswp->numEntries; i++, entry++) {
        if (!entry->size) continue;
        if (entry->mode != 0) continue;
        if (!(entry->flags & SHADOW_ENTRY_PLACED)) continue;
        if (entry->camId != cameraId) continue;
        if (cameraId == CAMERA_OFFSCREEN2) {
            sysWaitDrawSync();
            GXClearBoundingBox();
        }
        PSMTXConcat(view, entry->mtx, m);
        GXLoadPosMtxImm(m, GX_PNMTX0);
        GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, 6);
        GXPosition1x8(0);
        GXPosition1x8(1);
        GXPosition1x8(2);
        GXPosition1x8(3);
        GXPosition1x8(4);
        GXPosition1x8(5);
        if (cameraId == CAMERA_OFFSCREEN2) {
            sysWaitDrawSync();
            GXReadBoundingBox(&left, &top, &right, &bottom);
            offscreenAddBoundingBox(entry->offscreenId, left, top, right, bottom);
        }
    }
}

static inline void shadowSetMat(void) {
    GXSetChanMatColor(GX_COLOR0A0, cswp->color);
}

void shadowCharShadowDisp_Texture(CameraId cameraId, void* param) {
    ShadowEntry* entry = cswp->entries;
    f32 (*view)[4];
    s32 i;
    f32 size;
    Mtx m;
    GXTexObj tex;
    u16 left, top, right, bottom;

    view = camGetCurPtr()->view;
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
    shadowSetMat();
    effGetTexObjN64(0x2C, &tex);
    GXLoadTexObj(&tex, GX_TEXMAP0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_RASC);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_RASA, GX_CA_ZERO);
    GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, (GXColor){0, 0, 0, 0});
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
    GXSetZCompLoc(GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetCurrentMtx(GX_PNMTX0);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    size = 2.5f;
    for (i = 0; i < cswp->numEntries; i++, entry++) {
        if (!entry->size) continue;
        if (entry->mode != 1) continue;
        if (!(entry->flags & SHADOW_ENTRY_PLACED)) continue;
        if (entry->camId != cameraId) continue;
        if (cameraId == CAMERA_OFFSCREEN2) {
            sysWaitDrawSync();
            GXClearBoundingBox();
        }
        PSMTXConcat(view, entry->mtx, m);
        GXLoadPosMtxImm(m, GX_PNMTX0);
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3f32(-size / 2.0f, 0.0f, size / 2.0f);
        GXTexCoord2f32(0.0f, 0.0f);
        GXPosition3f32(size / 2.0f, 0.0f, size / 2.0f);
        GXTexCoord2f32(1.0f, 0.0f);
        GXPosition3f32(size / 2.0f, 0.0f, -size / 2.0f);
        GXTexCoord2f32(1.0f, 1.0f);
        GXPosition3f32(-size / 2.0f, 0.0f, -size / 2.0f);
        GXTexCoord2f32(0.0f, 1.0f);
        if (cameraId == CAMERA_OFFSCREEN2) {
            sysWaitDrawSync();
            GXReadBoundingBox(&left, &top, &right, &bottom);
            offscreenAddBoundingBox(entry->offscreenId, left, top, right, bottom);
        }
    }
}

void cylinder(ShadowEntry* entry) {
    f32 inner;
    f32 a;
    f32 b;
    u8 alpha;

    if (entry->flags & SHADOW_ENTRY_NO_DECAY) {
        GXSetArray(GX_VA_CLR0, color_tbl_nodecay, 4);
        GXCallDisplayList(cylinder_dl_top, 0x40);
        GXCallDisplayList(cylinder_dl_bottom, 0x40);
        GXCallDisplayList(cylinder_dl_side, 0x40);
        return;
    }
    inner = 1.0f - entry->decay2;
    a = (cswp->color.a * entry->volume) / 255.0f;
    b = a * entry->decay;
    if (inner < 0.0f) {
        inner = 0.0f;
    } else if (inner > 1.0f) {
        inner = 1.0f;
    }
    if (a <= 0.0f) {
        return;
    }
    if (b <= 0.0f) {
        b = 0.0f;
    }
    if (b >= 1.0f) {
        b = 1.0f;
    }
    alpha = 255.0f * b;
    GXSetArray(GX_VA_CLR0, color_tbl, 4);
    if (inner != 0.0f) {
        GXCallDisplayList(cylinder_dl_top_inside, 0x40);
    }
    if (inner != 1.0f && alpha) {
        GXCallDisplayList(cylinder_dl_top_outside, 0x40);
    }
    GXCallDisplayList(cylinder_dl_bottom, 0x40);
    GXCallDisplayList(cylinder_dl_side, 0x40);
}

static inline void shadowDrawVolume(ShadowEntry* entry) {
    switch (entry->type) {
        case 1:
            GXSetArray(GX_VA_CLR0, color_tbl_cube, 4);
            GXCallDisplayList(cube_dl_top, 0x40);
            GXCallDisplayList(cube_dl_bottom, 0x20);
            GXCallDisplayList(cube_dl_side, 0x20);
            break;
        case 0:
        default:
            cylinder(entry);
            break;
    }
}

static inline void shadowSetTev(u8 a) {
    GXColor c = {0, 0, 0, 0};
    c.a = a;
    GXSetTevColor(GX_TEVREG0, c);
}

static inline void shadowSetMatZero(void) {
    GXSetChanMatColor(GX_COLOR0A0, (GXColor){0, 0, 0, 0});
}


void cylinderShadowDraw(CameraId cameraId, void* param) {
    ShadowEntry* entry = cswp->entries;
    f32 (*view)[4];
    s32 i;
    Mtx m;
    u16 left, top, right, bottom;

    view = camGetCurPtr()->view;
    for (i = 0; i < cswp->numEntries; i++, entry++) {
        if (!entry->size) continue;
        if (entry->mode != 2) continue;
        if (!(entry->flags & SHADOW_ENTRY_PLACED)) continue;
        if (entry->camId != cameraId) continue;
        if (cameraId == CAMERA_OFFSCREEN2) {
            sysWaitDrawSync();
            GXClearBoundingBox();
        }
        PSMTXConcat(view, entry->mtx, m);
        GXLoadPosMtxImm(m, GX_PNMTX0);
        shadowSetTev(entry->alpha);
        GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
        GXSetCullMode(GX_CULL_BACK);
        GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ONE, GX_LO_CLEAR);
        GXSetBlendMode(GX_BM_LOGIC, GX_BL_ONE, GX_BL_ONE, GX_LO_OR);
        GXSetAlphaUpdate(GX_TRUE);
        GXSetColorUpdate(GX_FALSE);
        GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
        shadowDrawVolume(entry);
        GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
        GXSetCullMode(GX_CULL_FRONT);
        GXSetBlendMode(GX_BM_NONE, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
        GXSetAlphaUpdate(GX_TRUE);
        GXSetColorUpdate(GX_FALSE);
        shadowSetMatZero();
        GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
        shadowDrawVolume(entry);
        GXSetCullMode(GX_CULL_BACK);
        GXSetBlendMode(GX_BM_BLEND, GX_BL_DSTALPHA, GX_BL_INVDSTALPHA, GX_LO_CLEAR);
        GXSetAlphaUpdate(GX_FALSE);
        GXSetColorUpdate(GX_TRUE);
        shadowSetMat();
        shadowDrawVolume(entry);
        if (cameraId == CAMERA_OFFSCREEN2) {
            sysWaitDrawSync();
            GXReadBoundingBox(&left, &top, &right, &bottom);
            offscreenAddBoundingBox(entry->offscreenId, left, top, right, bottom);
        }
    }
}

const Vec vec3_802c1ac0 = {0.0f, 0.0f, 0.0f};

s32 shadowEntry(f32 x, f32 y, f32 z, f32 size) {
    s32 id = cswp->numEntries;
    ShadowEntry* entry = &cswp->entries[id];

    entry->flags = 0;
    if (cswp->defaultEnabled) {
        entry->flags |= SHADOW_ENTRY_GROUND;
    }
    entry->pos = (Vec){x, y, z};
    entry->size = size;
    entry->sizeY = cswp->defaultSizeY;
    entry->mode = cswp->defaultMode;
    entry->type = 0;
    entry->offscreenId = -1;
    entry->camId = CAMERA_3D;
    entry->volume = 1.0f;
    entry->decay = 0.2f;
    entry->decay2 = 0.4f;
    entry->alpha = 0xFF;
    cswp->numEntries++;
    return id;
}

const Vec vec3_802c1ad8 = {0.0f, 0.0f, 0.0f};

s32 shadowEntryMode(s32 mode, f32 x, f32 y, f32 z, f32 size) {
    s32 id = cswp->numEntries;
    ShadowEntry* entry = &cswp->entries[id];

    entry->flags = 0;
    if (cswp->defaultEnabled) {
        entry->flags |= SHADOW_ENTRY_GROUND;
    }
    entry->pos = (Vec){x, y, z};
    entry->size = size;
    entry->sizeY = cswp->defaultSizeY;
    entry->mode = mode;
    entry->type = 0;
    entry->offscreenId = -1;
    entry->camId = CAMERA_3D;
    entry->volume = 1.0f;
    entry->decay = 0.2f;
    entry->decay2 = 0.4f;
    entry->alpha = 0xFF;
    cswp->numEntries++;
    return id;
}

const Vec vec3_802c1af0 = {0.0f, 0.0f, 0.0f};

void shadowSetType(s32 id, u8 type) {
    ShadowEntry* entry = &cswp->entries[id];
    entry->type = type;
}

void U_shadowSetMode(s32 id, u8 alpha) {
    ShadowEntry* entry = &cswp->entries[id];
    entry->alpha = alpha;
}

void shadowSetVolumeDecay(s32 id, f32 volume, f32 decay, f32 decay2) {
    ShadowEntry* entry = &cswp->entries[id];
    entry->volume = volume;
    entry->decay = decay;
    entry->decay2 = decay2;
}

void shadowSetSize(s32 id, f32 size) {
    ShadowEntry* entry = &cswp->entries[id];
    entry->size = size;
}

void shadowSetCamId(s32 id, s32 camId) {
    ShadowEntry* entry = &cswp->entries[id];
    entry->camId = camId;
}

void shadowSetCollision(s32 id, BOOL enable) {
    s32 off = id * sizeof(ShadowEntry);
    ShadowEntry* e = cswp->entries;
    if (enable) {
        ((u16*)((u8*)e + off))[0] |= 1;
    } else {
        ((u16*)((u8*)e + off))[0] &= ~1;
    }
}

void shadowFlagOn(s32 id, u16 flags) {
    cswp->entries[id].flags |= flags;
}

GXTexObj* shadowGetCharShadowTexObj(void) {
    if (cswp->flags & SHADOW_WORK_READY) {
        return smartTexObj(&cswp->texObj, (void**)cswp->texBuffer);
    }
    return NULL;
}

f32 (*shadowGetCharShadowProjTexMtx(void))[4] {
    return cswp->texMtx;
}

GXColor shadowGetCharShadowColor(void) {
    return cswp->color;
}

s32 shadowGetCharShadowProjection(void) {
    return cswp->projection;
}

GXTexObj* shadowGetRamp8TexObj(void) {
    return &rampTex8;
}

GXTexObj* shadowGetRamp16TexObj(void) {
    return &rampTex16;
}

void shadowDepthShadowDrawCountUp(void) {
    dswp->drawCount++;
}

GXTexObj* shadowGetDepthShadowTexObj(void) {
    if (dswp->flags & SHADOW_WORK_READY) {
        return smartTexObj(&dswp->texObj, (void**)dswp->texBuffer);
    }
    return NULL;
}

f32 (*shadowGetDepthShadowDepthMtx(void))[4] {
    return dswp->depthMtx;
}

f32 (*shadowGetDepthShadowProjTexMtx(void))[4] {
    return dswp->texMtx;
}

static inline void depthShadowInit(void) {
    dswp->flags = SHADOW_WORK_ALLOC;
    dswp->drawCount = 0;
    dswp->configIdx = 0;
    dswp->color = (GXColor){0, 0, 0, 0x40};
}

GXColor shadowGetDepthShadowColor(void) {
    GXColor c;
    c.r = ((0xFF - dswp->color.r) * dswp->color.a) / 0xFF;
    c.g = ((0xFF - dswp->color.g) * dswp->color.a) / 0xFF;
    c.b = ((0xFF - dswp->color.b) * dswp->color.a) / 0xFF;
    c.a = 0;
    return c;
}

ShadowConfig* shadowGetDepthShadowConfig(void) {
    return &shadowConfig[dswp->configIdx];
}

void shadowProjShadowDrawCountUp(void) {
    pswp->drawCount++;
}

GXTexObj* shadowGetProjShadowTexObj(void) {
    if (pswp->flags & SHADOW_WORK_READY) {
        return smartTexObj(&pswp->texObj, (void**)pswp->texBuffer);
    }
    return NULL;
}

f32 (*shadowGetProjShadowProjTexMtx(void))[4] {
    return pswp->texMtx;
}

static inline void projShadowInit(void) {
    pswp->flags = SHADOW_WORK_ALLOC;
    pswp->drawCount = 0;
    pswp->configIdx = 0;
    pswp->color = (GXColor){0, 0, 0, 0x40};
}

GXColor shadowGetProjShadowColor(void) {
    return pswp->color;
}

ShadowConfig* shadowGetProjShadowConfig(void) {
    return &shadowConfig[pswp->configIdx];
}

static inline void shadowCalcTexMtx(ProjShadowWork* wp, Mtx texMtx, Mtx depthMtx) {
    ShadowConfig* cfg = &shadowConfig[wp->configIdx];
    u32 is16 = cfg->format > 2;
    Mtx m;
    Mtx dm;
    f32 n, f, d, scale;

    if (texMtx) {
        if (cfg->projType == 0) {
            C_MTXLightFrustum(m, -cfg->top, cfg->top, cfg->left, -cfg->left, cfg->near, 0.5f, 0.5f, 0.5f, 0.5f);
        } else {
            C_MTXLightOrtho(m, -cfg->top, cfg->top, cfg->left, -cfg->left, 0.5f, 0.5f, 0.5f, 0.5f);
        }
        PSMTXConcat(m, wp->view, texMtx);
    }
    if (depthMtx) {
        n = cfg->near;
        f = cfg->far;
        d = f - n;
        PSMTXScale(dm, 0.0f, 0.0f, 0.0f);
        scale = is16 ? 256.0f : 16.0f;
        if (cfg->projType == 0) {
            dm[0][2] = f / d;
            dm[0][3] = (f * n) / d;
            dm[1][2] = dm[0][2] * scale;
            dm[1][3] = dm[0][3] * scale;
            dm[2][2] = 1.0f;
            dm[2][3] = cfg->offset;
        } else {
            dm[0][2] = -1.0f / d;
            dm[0][3] = -n / d;
            dm[1][2] = dm[0][2] * scale;
            dm[1][3] = dm[0][3] * scale;
            dm[2][3] = 1.0f + cfg->offset;
        }
        PSMTXConcat(dm, wp->view, depthMtx);
    }
}

static inline void shadowRestoreView(void) {
    PSMTXCopy(tmp_view, camGetCurPtr()->view);
}

void projShadowEnd(CameraId cameraId, void* param) {
    ProjShadowWork* wp = param;
    u16 size;
    ShadowConfig* cfg;

    shadowRestoreView();
    GXSetViewport(tmp_vp[0], tmp_vp[1], tmp_vp[2], tmp_vp[3], tmp_vp[4], tmp_vp[5]);
    GXSetScissor(tmp_sci[0], tmp_sci[1], tmp_sci[2], tmp_sci[3]);
    if (wp->drawCount == 0) {
        return;
    }
    cfg = &shadowConfig[wp->configIdx];
    size = ShadowSizeTbl[cfg->sizeIdx];
    if (wp->flags & SHADOW_WORK_ALLOC) {
        wp->texBuffer = smartAlloc((wp->flags & SHADOW_WORK_HALF_SIZE_UNUSED) ? GXGetTexBufferSize(size / 2, size / 2, GX_TF_IA4, GX_FALSE, 0)
                                              : GXGetTexBufferSize(size, size, GX_TF_IA4, GX_FALSE, 0), 3);
    }
    GXSetTexCopySrc(0, 0, size, size);
    if (wp->flags & SHADOW_WORK_HALF_SIZE_UNUSED) {
        GXSetTexCopyDst(size / 2, size / 2, GX_TF_IA4, GX_TRUE);
    } else {
        GXSetTexCopyDst(size, size, GX_TF_IA4, GX_FALSE);
    }
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
    GXCopyTex(wp->texBuffer->address, GX_TRUE);
    GXPixModeSync();
    if (wp->flags & SHADOW_WORK_HALF_SIZE_UNUSED) {
        GXInitTexObj(&wp->texObj, wp->texBuffer->address, size / 2, size / 2, GX_TF_IA4, GX_CLAMP, GX_CLAMP, GX_FALSE);
    } else {
        GXInitTexObj(&wp->texObj, wp->texBuffer->address, size, size, GX_TF_IA4, GX_CLAMP, GX_CLAMP, GX_FALSE);
    }
    GXInitTexObjLOD(&wp->texObj, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    shadowCalcTexMtx(wp, wp->texMtx, NULL);
    wp->flags |= SHADOW_WORK_READY;
}

void depthShadowEnd(CameraId cameraId, void* param) {
    ShadowConfig* cfg;
    u32 texFmt;
    u32 copyFmt;
    ProjShadowWork* wp = param;
    u16 size;

    shadowRestoreView();
    GXSetColorUpdate(GX_TRUE);
    GXSetViewport(tmp_vp[0], tmp_vp[1], tmp_vp[2], tmp_vp[3], tmp_vp[4], tmp_vp[5]);
    GXSetScissor(tmp_sci[0], tmp_sci[1], tmp_sci[2], tmp_sci[3]);
    if (wp->drawCount == 0) {
        return;
    }
    cfg = &shadowConfig[wp->configIdx];
    if (cfg->format <= 2) {
        copyFmt = GX_TF_Z8;
        texFmt = GX_TF_I8;
    } else {
        copyFmt = GX_TF_Z16;
        texFmt = GX_TF_IA8;
    }
    if (wp->flags & SHADOW_WORK_ALLOC) {
        size = ShadowSizeTbl[cfg->sizeIdx];
        wp->texBuffer = smartAlloc(GXGetTexBufferSize(size, size, texFmt, GX_FALSE, 0), 3);
    }
    GXSetTexCopySrc(0, 0, size, size);
    GXSetTexCopyDst(size, size, copyFmt, GX_FALSE);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
    GXCopyTex(wp->texBuffer->address, GX_TRUE);
    GXPixModeSync();
    GXInitTexObj(&wp->texObj, wp->texBuffer->address, size, size, texFmt, GX_CLAMP, GX_CLAMP, GX_FALSE);
    GXInitTexObjLOD(&wp->texObj, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    shadowCalcTexMtx(wp, wp->texMtx, wp->depthMtx);
    wp->flags |= SHADOW_WORK_READY;
}

// debug strings (from stripped debug code)
const char str_802C1AFC[] = "result scene (8bit depth)";
const char str_802C1B18[] = "actual depth from the light (8bit depth)";
const char str_802C1B44[] = "projected shadow depth map (8bit depth)";
const char str_802C1B6C[] = "result scene (16bit depth)";
const char str_802C1B88[] = "actual depth from the light (16bit depth)";
const char str_802C1BB4[] = "projected shadow depth map (16bit depth)";
