// Small GX helpers: queen_uv (texture offsets extdrv scrolls), and in the early demos the 2D textured-quad
// helpers seq_demo draws with.

#include "version.h"
#include "gxsub.h"

//.sdata
#if VERSION <= VERSION_DEMO_JP
#include "driver/camdrv.h"
f32 queen_uv[4]
= {0.0f, -0.0016000001f, 0.0f, -0.00080000004f};
static s32 lastGX ATTRIBUTE_ALIGN(32) = 8;
static BOOL fillTexEdgeOn;
static BOOL fillTexAlphaOff;
static GXTexObj* pFillTexObj;

/**
 * Early demos only.
 * Records mode as the last applied GX setup; returns TRUE when it differs from the previous one,
 * so the caller must set up GX again. -1 forces the next call to return TRUE.
 */
BOOL gxsubMiscSetGX(s32 mode) {
    if (mode != lastGX) {
        lastGX = mode;
        return TRUE;
    }
    return FALSE;
}

/**
 * Early demos only.
 * Draws a w x h quad at (x, y) in the current camera,
 * textured with the texture set by gxsubFillSetTexObj (or a plain white texel) and tinted by color;
 * height flips for the 2D camera.
 * seq_demo uses it to draw mariost.tpl texture 5 over the demo movie:
 * the title logo's top half, which the demo's title screen draws at the logo's top position (seq_title.c).
 */
void gxsubFillGX(s32 x, s32 y, s32 w, s32 h, GXColor* color) {
    static u8 filltex[4] ATTRIBUTE_ALIGN(32) = {0xFF, 0xFF, 0x00, 0x00}; // 1x1 I4 texel used when no texture is set
    Mtx mtx;
    GXTexObj obj;
    GXColor c;
    CameraEntry* cam = camGetCurPtr();

    PSMTXIdentity(mtx);
    PSMTXConcat(cam->view, mtx, mtx);
    GXLoadPosMtxImm(mtx, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    if (gxsubMiscSetGX(5)) {
        GXSetNumChans(0);
        GXSetNumTexGens(1);
        GXSetNumTevStages(1);
        GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_C0, GX_CC_TEXC, GX_CC_ZERO);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
        GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
        GXSetZCompLoc(GX_TRUE);
        GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_OR);
        GXSetCullMode(GX_CULL_NONE);
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    }
    if (pFillTexObj) {
        GXLoadTexObj(pFillTexObj, GX_TEXMAP0);
    } else {
        GXInitTexObj(&obj, filltex, 1, 1, GX_TF_I4, GX_CLAMP, GX_CLAMP, GX_FALSE);
        GXLoadTexObj(&obj, GX_TEXMAP0);
    }
    if (fillTexAlphaOff) {
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A0);
    }
    if (fillTexEdgeOn) {
        GXSetZCompLoc(GX_FALSE);
        GXSetAlphaCompare(GX_GEQUAL, 0x80, GX_AOP_OR, GX_NEVER, 0);
        GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_CLEAR);
    }
    c = *color;
    GXSetTevColor(GX_TEVREG0, c);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    if (camGetCurPtr() == camGetPtr(CAMERA_2D)) {
        h *= -1;
    }
    GXPosition3s16(x, y, 0);
    GXTexCoord2f32(0.0f, 0.0f);
    GXPosition3s16(x + w, y, 0);
    GXTexCoord2f32(1.0f, 0.0f);
    GXPosition3s16(x + w, y + h, 0);
    GXTexCoord2f32(1.0f, 1.0f);
    GXPosition3s16(x, y + h, 0);
    GXTexCoord2f32(0.0f, 1.0f);
}

/** Early demos only. Sets the texture gxsubFillGX draws with; NULL goes back to a plain fill. */
void gxsubFillSetTexObj(GXTexObj* obj) {
    pFillTexObj = obj;
}
#else
f32 queen_uv[4] = {0.0f, -0.0016000001f, 0.0f, -0.00080000004f};
#endif
