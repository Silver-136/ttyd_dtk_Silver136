#include "effect/eff_number.h"


/* CHATGPT STUB FILL: main/effect/eff_number 20260624_184929 */

/* stub-fill: effNumberGX | missing_definition | ghidra_signature */
void effNumberGX(f32 matrix[3][4], s32 type, s32 value, u8 alpha, s32 layers) {
    typedef u32 GXTexObj[8];
    extern void* camGetCurPtr(void);
    extern void effGetTexObj(s32 id, GXTexObj obj);
    extern void GXLoadTexObj(GXTexObj obj, s32 map);
    extern void GXSetNumChans(s32 n);
    extern void GXSetChanCtrl(s32, s32, s32, s32, s32, s32, s32);
    extern void GXSetNumTexGens(s32 n);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern void GXSetNumTevStages(s32 n);
    extern void GXSetTevOrder(s32 stage, s32 coord, s32 map, s32 color);
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
    extern void GXSetCullMode(s32 mode);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32, s32);
    extern void GXSetVtxAttrFmt(s32, s32, s32, s32, s32);
    extern void GXSetTevSwapMode(s32 stage, s32 ras, s32 tex);
    extern void GXSetTevSwapModeTable(s32 table, s32 r, s32 g, s32 b, s32 a);
    extern void PSMTXTrans(f32 m[3][4], f32 x, f32 y, f32 z);
    extern void PSMTXScale(f32 m[3][4], f32 x, f32 y, f32 z);
    extern void PSMTXConcat(f32 a[3][4], f32 b[3][4], f32 out[3][4]);
    extern void GXLoadPosMtxImm(f32 m[3][4], s32 id);
    extern void GXSetCurrentMtx(s32 id);
    extern void GXLoadTexMtxImm(f32 m[3][4], s32 id, s32 type);
    extern void GXBegin(s32 primitive, s32 format, s32 count);
    extern s32 sprintf(char* out, const char* fmt, ...);
    extern u32 strlen(const char* text);
    char text[32];
    GXTexObj tex;
    f32 trans[3][4];
    f32 scale[3][4];
    f32 model[3][4];
    s32 length;
    s32 layer;
    s32 i;
    s32 layerOffset = 0;

    camGetCurPtr();
    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 1, 0, 0, 2);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(0xB, 1);
    GXSetVtxDesc(0xD, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 0xB, 1, 5, 0);
    GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);

    sprintf(text, "%d", value);
    length = strlen(text);
    for (layer = 0; layer < layers + 1; layer++) {
        if (layer == 0) {
            effGetTexObj(1, tex);
            GXLoadTexObj(tex, 0);
            GXSetNumTevStages(1);
            GXSetTevOrder(0, 0, 0, 4);
            GXSetTevColorOp(0, 0, 0, 0, 1, 0);
            GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
            GXSetTevColorIn(0, 0xF, 0xF, 0xF, 0xF);
            GXSetTevAlphaIn(0, 7, 5, 4, 7);
            GXSetTevSwapMode(0, 0, 0);
        } else {
            effGetTexObj(0x6C, tex);
            GXLoadTexObj(tex, 0);
            GXSetNumTevStages(3);
            GXSetTevOrder(0, 0, 0, 0xFF);
            GXSetTevColorOp(0, 0xE, 0, 0, 1, 0);
            GXSetTevAlphaOp(0, 0xE, 0, 0, 1, 0);
            GXSetTevColorIn(0, 8, 0xF, 8, 0xF);
            GXSetTevAlphaIn(0, 4, 7, 4, 7);
            GXSetTevSwapModeTable(1, 0, 1, 2, 0);
            GXSetTevSwapMode(0, 1, 1);
        }
        for (i = 0; i < length; i++) {
            s32 index = length - 1 - i;
            s32 digit = text[index] == '-' ? 10 : text[index] - '0';
            PSMTXTrans(trans, (length / 2 - i) * 32.0f - (f32)layerOffset,
                       (f32)layerOffset, 0.0f);
            PSMTXConcat(matrix, trans, model);
            GXLoadPosMtxImm(model, 0);
            GXSetCurrentMtx(0);
            PSMTXScale(scale, 1.0f / 11.0f, 1.0f, 1.0f);
            PSMTXTrans(trans, (f32)digit, 0.0f, 0.0f);
            PSMTXConcat(scale, trans, model);
            GXLoadTexMtxImm(model, 0x1E, 1);
            GXBegin(0x80, 0, 4);
        }
        layerOffset += 2;
    }
    GXSetTevSwapModeTable(1, 0, 1, 2, 3);
    GXSetTevSwapMode(0, 0, 0);
}

