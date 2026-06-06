#include "manager/fontmgr.h"

typedef struct Vec {
    f32 x;
    f32 y;
    f32 z;
} Vec;

u32 fontColor;
u8 fontAlpha;
u32 fontColTbl[8];
s32 fontEdge;
Vec fontScale;
void* pfh;
extern void* gp;

s32 JUTFont_CodeToGlyph(s32 code);
f32 FontGetMessageWidthLine(const char* msg, s32 line);
void GXSetNumTexGens(s32 count);
void GXSetNumTevStages(s32 count);
void GXSetTevColor(s32 reg, void* color);
void JUTFontSetup(s32 language);

void fontmgrInit(void) {
}

u32 FontGetDrawColor(void) {
    return fontColor;
}

void FontDrawColor_(void) {
    u32 color;
    u32 oldColor;
    u32 gxColor;

    color = fontColor;
    oldColor = color;
    ((u8*)&color)[3] = (((u8*)&color)[3] * fontAlpha) / 255;
    fontColor = oldColor;
    gxColor = color;
    GXSetTevColor(1, &gxColor);
}

void FontDrawColor(void* colorPtr) {
    u32 color;

    fontColor = *(u32*)colorPtr;
    color = *(u32*)colorPtr;
    ((u8*)&color)[3] = (((u8*)&color)[3] * fontAlpha) / 255;
    GXSetTevColor(1, &color);
}

void FontDrawColorIDX(s32 idx) {
    u32 color;
    u32 gxColor;

    color = fontColTbl[idx];
    fontColor = color;
    ((u8*)&color)[3] = (((u8*)&color)[3] * fontAlpha) / 255;
    gxColor = color;
    GXSetTevColor(1, &gxColor);
}

void FontDrawEdgeOff(void) {
    fontEdge = 0;
}

void FontDrawEdge(void) {
    fontEdge = 1;
}

void FontDrawScaleVec(Vec* scale) {
    fontScale = *scale;
}

void FontDrawScale(f32 scale) {
    fontScale.x = scale;
    fontScale.y = scale;
    fontScale.z = scale;
}

s32 kanjiSearch(s32 code) {
    return JUTFont_CodeToGlyph(code);
}

s32 hankakuSearch(s32 code) {
    return JUTFont_CodeToGlyph((u8)code);
}

f32 FontGetMessageWidth(const char* msg) {
    return FontGetMessageWidthLine(msg, 0);
}

void FontDrawNoiseOff(void) {
    GXSetNumTexGens(1);
    GXSetNumTevStages(1);
}

void FontDrawRainbowColorOff(void) {
    GXSetNumTevStages(1);
}

void fontmgrTexSetup(void) {
    JUTFontSetup((u16)*(s32*)((s32)gp + 0x16C));
}

u8 kanjiGetWidth(u16 code) {
    void* entry;
    void* found;
    u32 magic;
    u32 count;
    s32 i;

    entry = (void*)((s32)pfh + 0x20);
    magic = 0x57494431;
    count = *(s32*)((s32)pfh + 0xC);
    found = 0;
    for (i = 0; i < count; i++) {
        if (*(u32*)entry == magic) {
            u16 start = *(u16*)((s32)entry + 8);
            if (code >= start && code <= *(u16*)((s32)entry + 0xA)) {
                found = (void*)((s32)entry + ((code - start) * 2) + 0xC);
                break;
            }
        }
        entry = (void*)((s32)entry + *(s32*)((s32)entry + 4));
    }
    return *(u8*)((s32)found + 1);
}


u8 FontDrawMessage(u32 param_1, u32 param_2, char* param_3) {
    return 0;
}


u8 FontDrawMessageMtx(s32 param_1, char* param_2) {
    return 0;
}


u8 _JUTFont_DrawPos(u16 param_1, s16 param_2, s16 param_3) {
    return 0;
}


void FontDrawStringMtx(f32* mtx, void* string) {
}


u8 FontDrawStringVecPitch(void) {
    return 0;
}


u8 FontDrawStringShake(void) {
    return 0;
}


f32 FontGetMessageWidthLine(const char* msg, s32 line) {
    return 0.0f;
}


void JUTFontSetup(s32 language) {
}


u8 FontDrawStringCenterMtx(void) {
    return 0;
}


u8 JUTFont_DrawStart(u32* param_1) {
    return 0;
}


u8 HSV2RGB(void) {
    return 0;
}


u8 FontDrawCode(void) {
    return 0;
}


s32 JUTFont_CodeToGlyph(s32 code) {
    return 0;
}


u8 FontDrawCodeMtx(s32 param_1, s32 param_2) {
    return 0;
}


void FontDrawNoise(void) {
    extern void GXSetNumTexGens(s32 count);
    extern void GXSetNumTevStages(s32 count);
    extern void GXSetTevKAlphaSel(s32 stage, s32 sel);
    extern void GXSetTevOrder(s32 stage, s32 texCoord, s32 texMap, s32 color);
    extern void GXSetTevColorOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevAlphaOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevColorIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevAlphaIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXLoadTexObj(void* obj, s32 mapId);
    extern void PSMTXScale(void* mtx, f32 x, f32 y, f32 z);
    extern void GXLoadTexMtxImm(void* mtx, s32 id, s32 type);
    extern void GXSetTexCoordGen2(s32 coord, s32 type, s32 src, s32 mtx, s32 normalize, s32 postMtx);
    extern void* wakuTexObj;
    extern f32 float_0p015625_80420438;
    extern f32 float_1_804203e4;
    f32 mtx[12];

    GXSetNumTexGens(2);
    GXSetNumTevStages(2);
    GXSetTevKAlphaSel(1, 0);
    GXSetTevOrder(1, 1, 2, 0xFF);
    GXSetTevColorOp(1, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(1, 0, 0, 0, 1, 0);
    GXSetTevColorIn(1, 0xF, 0xF, 0xF, 0);
    GXSetTevAlphaIn(1, 7, 0, 4, 7);
    GXLoadTexObj((void*)((s32)wakuTexObj + 0x640), 2);
    PSMTXScale(mtx, float_0p015625_80420438, float_0p015625_80420438, float_1_804203e4);
    GXLoadTexMtxImm(mtx, 0x21, 1);
    GXSetTexCoordGen2(1, 1, 5, 0x21, 0, 0x7D);
}

void FontDrawRainbowColor(void) {
    extern void GXSetNumTevStages(s32 count);
    extern void GXSetTevOrder(s32 stage, s32 texCoord, s32 texMap, s32 color);
    extern void GXSetTevColorOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevAlphaOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevColorIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevAlphaIn(s32 stage, s32 a, s32 b, s32 c, s32 d);

    GXSetNumTevStages(2);
    GXSetTevOrder(1, 0xFF, 0xFF, 4);
    GXSetTevColorOp(1, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(1, 0, 0, 0, 1, 0);
    GXSetTevColorIn(1, 0xF, 0, 0xA, 0xF);
    GXSetTevAlphaIn(1, 7, 0, 5, 7);
}

void FontDrawStart(void) {
    extern u32 fontColor;
    extern u8 fontAlpha;
    extern u32 fontColTbl[8];
    extern s32 fontEdge;
    extern Vec fontScale;
    extern Vec vec3_802c2b6c;
    extern void JUTFont_DrawStart(void* color);
    u32 color;
    u32 gxColor;

    color = fontColTbl[0];
    fontScale = vec3_802c2b6c;
    fontEdge = 0;
    fontColor = color;
    fontAlpha = 0xFF;
    ((u8*)&color)[3] = (((u8*)&color)[3] * 0xFF) / 255;
    gxColor = color;
    JUTFont_DrawStart(&gxColor);
}

void FontDrawStart_alpha(u8 alpha) {
    extern u32 fontColor;
    extern u8 fontAlpha;
    extern u32 fontColTbl[8];
    extern s32 fontEdge;
    extern Vec fontScale;
    extern Vec vec3_802c2b6c;
    extern void JUTFont_DrawStart(void* color);
    u32 color;
    u32 gxColor;

    color = fontColTbl[0];
    fontScale = vec3_802c2b6c;
    fontAlpha = alpha;
    fontEdge = 0;
    fontColor = color;
    ((u8*)&color)[3] = (((u8*)&color)[3] * alpha) / 255;
    gxColor = color;
    JUTFont_DrawStart(&gxColor);
}

u8 FontDrawStringPitch(double param_1, double param_2, s64 param_3, s32 param_4) {
    return 0;
}


void FontDrawString(f32 x, f32 y, s32 str) {
}
