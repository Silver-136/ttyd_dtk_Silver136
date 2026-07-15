#include "effect/eff_number.h"


/* CHATGPT STUB FILL: main/effect/eff_number 20260624_184929 */

/* stub-fill: effNumberGX | missing_definition | ghidra_signature */
void effNumberGX(f32 matrix[3][4], s32 type, s32 value, u8 alpha, s32 layers) {
    typedef u32 GXTexObj[8];
    extern void* camGetCurPtr(void);
    extern void effGetTexObj(s32 id, GXTexObj obj);
    extern void GXLoadTexObj(GXTexObj obj, s32 map);
    extern void GXSetNumChans(s32 n);
    extern void GXSetNumTexGens(s32 n);
    extern void GXSetNumTevStages(s32 n);
    extern void GXSetTevOrder(s32 stage, s32 coord, s32 map, s32 color);
    extern void GXSetCullMode(s32 mode);
    extern void GXSetTevSwapMode(s32 stage, s32 ras, s32 tex);
    extern void GXSetTevSwapModeTable(s32 table, s32 r, s32 g, s32 b, s32 a);
    extern void PSMTXTrans(f32 m[3][4], f32 x, f32 y, f32 z);
    extern void PSMTXScale(f32 m[3][4], f32 x, f32 y, f32 z);
    extern void PSMTXConcat(f32 a[3][4], f32 b[3][4], f32 out[3][4]);
    extern void GXLoadPosMtxImm(f32 m[3][4], s32 id);
    extern void GXLoadTexMtxImm(f32 m[3][4], s32 id, s32 type);
    extern void GXBegin(s32 primitive, s32 format, s32 count);
    char text[32];
    GXTexObj tex;
    f32 trans[3][4], scale[3][4], model[3][4];
    s32 length, layer, i;
    extern s32 sprintf(char* out, const char* fmt, ...);
    extern u32 strlen(const char* text);
    sprintf(text, "%d", value);
    length = strlen(text);
    GXSetNumChans(1);
    GXSetNumTexGens(1);
    GXSetCullMode(0);
    for (layer = 0; layer <= layers; layer++) {
        effGetTexObj(layer == 0 ? 1 : 0x6C, tex);
        GXLoadTexObj(tex, 0);
        GXSetNumTevStages(layer == 0 ? 1 : 3);
        GXSetTevOrder(0, 0, 0, layer == 0 ? 0 : 0xFF);
        for (i = 0; i < length; i++) {
            s32 digit = text[length - 1 - i] == '-' ? 10 : text[length - 1 - i] - '0';
            PSMTXTrans(trans, (length / 2 - i) * 32.0f - layer * 2.0f, layer * 2.0f, 0.0f);
            PSMTXConcat(matrix, trans, model);
            GXLoadPosMtxImm(model, 0);
            PSMTXScale(scale, 1.0f / 11.0f, 1.0f, 1.0f);
            PSMTXTrans(trans, (f32)digit, 0.0f, 0.0f);
            PSMTXConcat(scale, trans, model);
            GXLoadTexMtxImm(model, 0x1E, 1);
            GXBegin(0x80, 0, 4);
            if (alpha == 0) break;
        }
    }
    GXSetTevSwapModeTable(1, 0, 1, 2, 3);
    GXSetTevSwapMode(0, 0, 0);
}

