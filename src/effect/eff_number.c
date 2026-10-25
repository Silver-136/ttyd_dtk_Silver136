#include "driver/camdrv.h"
#include "driver/effdrv.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <stdio.h>
#include <string.h>
#include "version.h"
// US effect texture ids are 0x1C lower for these entries.
#define EFF_TEX(id) ((id) - VERSION_PICK(0, 0x1C))


static const f32 padz = 0.0f;

void effNumberGX(Mtx mtx, s32 type, s32 number, u8 alpha, s32 extra) {
    CameraEntry* cam;
    GXColor cols[2];
    GXColor tevColor;
    GXTexObj texObj;
    Mtx posMtx;
    Mtx scaleMtx;
    s8 buf[32];
    s32 len;
    s32 offset;
    s32 pass;
    f32 lo;
    f32 hi;
    s32 half;
    s32 i;

    cam = camGetCurPtr();
    switch (type) {
    case 0:
        cols[1] = (GXColor){ 0xDE, 0x0B, 0x16, alpha };
        cols[0] = (GXColor){ 0xFF, 0xF0, 0xF1, alpha };
        break;
    case 1:
        cols[1] = (GXColor){ 0x68, 0xC0, 0x03, alpha };
        cols[0] = (GXColor){ 0xEB, 0xFE, 0x6F, alpha };
        break;
    case 2:
        cols[1] = (GXColor){ 0xFF, 0x56, 0x27, alpha };
        cols[0] = (GXColor){ 0xFF, 0xC5, 0x18, alpha };
        break;
    case 3:
    default:
        cols[1] = (GXColor){ 0xFF, 0xFF, 0xFF, alpha };
        cols[0] = (GXColor){ 0xFF, 0xFF, 0xFF, alpha };
        break;
    }

    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);

    lo = -24.0f;
    hi = 24.0f;
    sprintf((char*)buf, "%d", number);
    len = strlen((char*)buf);
    half = len / 2;
    for (pass = 0, offset = 0; pass < extra + 1; offset += 2, pass++) {
        if (pass == 0) {
            effGetTexObj(1, &texObj);
            GXLoadTexObj(&texObj, GX_TEXMAP0);
            GXSetNumTevStages(1);
            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
            GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_RASA, GX_CA_TEXA, GX_CA_ZERO);
            GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
        } else {
            effGetTexObj(EFF_TEX(0x88), &texObj);
            GXLoadTexObj(&texObj, GX_TEXMAP0);
            GXSetNumTevStages(3);
            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
            GXSetTevColorOp(GX_TEVSTAGE0, (GXTevOp)0xE, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevAlphaOp(GX_TEVSTAGE0, (GXTevOp)0xE, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_TEXC, GX_CC_ZERO, GX_CC_TEXC, GX_CC_ZERO);
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_TEXA, GX_CA_ZERO, GX_CA_TEXA, GX_CA_ZERO);
            GXSetTevSwapModeTable(GX_TEV_SWAP1, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_RED);
            GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP1, GX_TEV_SWAP1);
            GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
            GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_CPREV, GX_CC_RASC, GX_CC_ZERO);
            GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
            GXSetTevSwapMode(GX_TEVSTAGE1, GX_TEV_SWAP0, GX_TEV_SWAP0);
            GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
            GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_C0, GX_CC_CPREV, GX_CC_APREV, GX_CC_ZERO);
            GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_TEXA, GX_CA_RASA, GX_CA_ZERO);
            tevColor = (GXColor){ 0x3C, 0x3C, 0x3C, 0xFF };
            GXSetTevColor(GX_TEVREG0, tevColor);
            GXSetTevSwapMode(GX_TEVSTAGE2, GX_TEV_SWAP0, GX_TEV_SWAP0);
        }
        for (i = 0; i < len; i++) {
            PSMTXTrans(posMtx, 32.0f * (f32)(half - i) - (f32)offset, (f32)offset, 0.0f);
            PSMTXConcat(mtx, posMtx, posMtx);
            PSMTXConcat(cam->view, posMtx, posMtx);
            GXLoadPosMtxImm(posMtx, GX_PNMTX0);
            GXSetCurrentMtx(GX_PNMTX0);
            if (buf[len - 1 - i] == '-') {
                PSMTXScale(scaleMtx, 1.0f / 11.0f, 1.0f, 1.0f);
                PSMTXTrans(posMtx, 10.0f, 0.0f, 0.0f);
                PSMTXConcat(scaleMtx, posMtx, posMtx);
                GXLoadTexMtxImm(posMtx, GX_TEXMTX0, GX_MTX2x4);
            } else {
                PSMTXScale(scaleMtx, 1.0f / 11.0f, 1.0f, 1.0f);
                PSMTXTrans(posMtx, (f32)(buf[len - 1 - i] - '0'), 0.0f, 0.0f);
                PSMTXConcat(scaleMtx, posMtx, posMtx);
                GXLoadTexMtxImm(posMtx, GX_TEXMTX0, GX_MTX2x4);
            }
            GXBegin(GX_QUADS, GX_VTXFMT0, 4);
            GXPosition3f32(lo, hi, 0.0f);
            GXColor4u8(cols[1].r, cols[1].g, cols[1].b, cols[1].a);
            GXTexCoord2f32(0.0f, 0.0f);
            GXPosition3f32(hi, hi, 0.0f);
            GXColor4u8(cols[1].r, cols[1].g, cols[1].b, cols[1].a);
            GXTexCoord2f32(1.0f, 0.0f);
            GXPosition3f32(hi, lo, 0.0f);
            GXColor4u8(cols[0].r, cols[0].g, cols[0].b, cols[0].a);
            GXTexCoord2f32(1.0f, 1.0f);
            GXPosition3f32(lo, lo, 0.0f);
            GXColor4u8(cols[0].r, cols[0].g, cols[0].b, cols[0].a);
            GXTexCoord2f32(0.0f, 1.0f);
        }
    }
    GXSetTevSwapModeTable(GX_TEV_SWAP1, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
    GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
}

