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
    ;
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
    extern void GXSetTevColor(s32 reg, void* color);
    u32 color;

    fontColor = *(u32*)colorPtr;
    ((u8*)colorPtr)[3] = (((u8*)colorPtr)[3] * fontAlpha) / 255;
    color = *(u32*)colorPtr;
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
    ;
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
    ;
}


u8 FontDrawStringCenterMtx(void) {
    return 0;
}


u8 JUTFont_DrawStart(u32* param_1) {
    return 0;
}


u32 HSV2RGB(u8* hsv) {
    extern f64 floor(f64 x);
    extern const f32 float_360_80420400;
    extern const f32 float_0p0039062_804203f4;
    extern const f32 float_60_80420404;
    extern const f32 float_255_804203fc;
    extern const f32 float_1_804203e4;
    f32 h;
    f32 s;
    f32 v;
    f32 f;
    f32 p;
    f32 q;
    f32 r;
    f32 g;
    f32 b;
    s32 i;
    u32 color;
    u8* out;

    h = ((float_360_80420400 * hsv[0]) * float_0p0039062_804203f4) / float_60_80420404;
    s = hsv[1] / float_255_804203fc;
    v = hsv[2] / float_255_804203fc;
    i = (s32)floor(h);
    f = h - (f32)i;
    if ((i & 1) == 0) {
        f = float_1_804203e4 - f;
    }
    p = v * (float_1_804203e4 - s);
    q = v * (float_1_804203e4 - (s * f));

    switch (i) {
        case 0:
            r = v;
            g = q;
            b = p;
            break;
        case 1:
            r = q;
            g = v;
            b = p;
            break;
        case 2:
            r = p;
            g = v;
            b = q;
            break;
        case 3:
            r = p;
            g = q;
            b = v;
            break;
        case 4:
            r = q;
            g = p;
            b = v;
            break;
        case 5:
            r = v;
            g = p;
            b = q;
            break;
    }

    out = (u8*)&color;
    out[0] = (u8)(r * float_255_804203fc);
    out[1] = (u8)(g * float_255_804203fc);
    out[2] = (u8)(b * float_255_804203fc);
    out[3] = hsv[3];
    return color;
}

u8 FontDrawCode(void) {
    return 0;
}


s32 JUTFont_CodeToGlyph(s32 code) {
    extern void* pfh;
    extern void* pfi;
    void* entry;
    u16 c;
    s32 count;
    s32 glyph;

    c = code;
    entry = (void*)((s32)pfh + 0x20);
    count = *(s32*)((s32)pfh + 0xC);
    glyph = *(u16*)((s32)pfi + 0x12);
    while (count > 0) {
        if (*(u32*)entry == 0x4D415031 &&
            c >= *(u16*)((s32)entry + 0xA) &&
            c <= *(u16*)((s32)entry + 0xC)) {
            u16 kind = *(u16*)((s32)entry + 8);
            switch (kind) {
                case 0:
                    glyph = (u16)(code - *(u16*)((s32)entry + 0xA));
                    break;
                case 1: {
                    u16 hi;
                    u16 lo;
                    s32 offset;

                    *(u16*)((s32)&lo) = code;
                    hi = ((u8*)&lo)[1];
                    lo = ((u8*)&lo)[0];
                    if (hi >= 0x80) {
                        offset = hi - 0x41;
                    } else {
                        offset = hi - 0x40;
                    }
                    if (*(u16*)((s32)entry + 0xE) == 0) {
                        glyph = (u16)((lo - 0x88) * 0xBC + 0x2BE + offset);
                    } else {
                        glyph = (u16)((lo - 0x88) * 0xBC - 0x5E + *(u16*)((s32)entry + 0x10) + offset);
                    }
                    break;
                }
                case 2:
                    glyph = *(u16*)((s32)entry + 0x10 + ((c - *(u16*)((s32)entry + 0xA)) * 2));
                    break;
                case 3: {
                    s32 low = 0;
                    s32 high = *(u16*)((s32)entry + 0xE);
                    while (high >= low) {
                        s32 mid = (low + high) / 2;
                        u16 value = *(u16*)((s32)entry + 0x10 + mid * 4);
                        if (c == value) {
                            glyph = *(u16*)((s32)entry + 0x12 + mid * 4);
                            break;
                        }
                        if (c < value) {
                            high = mid - 1;
                        } else {
                            low = mid + 1;
                        }
                    }
                    break;
                }
            }
            break;
        }
        entry = (void*)((s32)entry + *(s32*)((s32)entry + 4));
        count--;
    }
    return glyph;
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

void FontDrawStringPitch(f32 x, f32 y, s32 str, f32 pitch) {
    extern void FontDrawStringVecPitch(void* pos, s32 str, f32 pitch);
    extern const u32 vec3_802c2b84[3];
    u32 vec[3];
    u32 out[3];

    vec[0] = vec3_802c2b84[0];
    vec[1] = vec3_802c2b84[1];
    vec[2] = vec3_802c2b84[2];
    *(f32*)&vec[0] = x;
    *(f32*)&vec[1] = y;
    out[0] = vec[0];
    out[1] = vec[1];
    out[2] = vec[2];
    FontDrawStringVecPitch(out, str, pitch);
}

void FontDrawString(f32 x, f32 y, s32 str) {
    extern void FontDrawStringVecPitch(void* pos, s32 str, f32 pitch);
    extern const u32 vec3_802c2b78[3];
    extern const f32 float_neg1_80420430;
    u32 vec[3];
    u32 out[3];

    vec[0] = vec3_802c2b78[0];
    vec[1] = vec3_802c2b78[1];
    vec[2] = vec3_802c2b78[2];
    *(f32*)&vec[0] = x;
    *(f32*)&vec[1] = y;
    out[0] = vec[0];
    out[1] = vec[1];
    out[2] = vec[2];
    FontDrawStringVecPitch(out, str, float_neg1_80420430);
}

