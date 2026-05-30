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

    color = fontColor;
    ((u8*)&color)[3] = (((u8*)&color)[3] * fontAlpha) / 255;
    GXSetTevColor(1, &color);
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

    color = fontColTbl[idx];
    fontColor = color;
    ((u8*)&color)[3] = (((u8*)&color)[3] * fontAlpha) / 255;
    GXSetTevColor(1, &color);
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
    s32 count;
    s32 i;

    entry = (void*)((s32)pfh + 0x20);
    count = *(s32*)((s32)pfh + 0xC);
    for (i = 0; i < count; i++) {
        if (*(u32*)entry == 0x57494431 && code >= *(u16*)((s32)entry + 8) && code <= *(u16*)((s32)entry + 0xA)) {
            return *(u8*)((s32)entry + ((code - *(u16*)((s32)entry + 8)) * 2) + 0xD);
        }
        entry = (void*)((s32)entry + *(s32*)((s32)entry + 4));
    }
    return *(u8*)1;
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


u8 FontDrawNoise(void) {
    return 0;
}


u8 FontDrawRainbowColor(void) {
    return 0;
}


void FontDrawStart(void) {
}


u8 FontDrawStart_alpha(u8 param_1) {
    return 0;
}


u8 FontDrawStringPitch(double param_1, double param_2, s64 param_3, s32 param_4) {
    return 0;
}


void FontDrawString(f32 x, f32 y, s32 str) {
}
