#include "manager/fontmgr.h"

extern const char str_papermarioset_JPN_802c2b30[];
extern const char str_papermarioset_US_802c2b44[];
extern const char str_papermarioset_EU_802c2b58[];
extern Vec vec3_802c2b6c;
extern const char str_PCTs_f_PCTs_bfn_802c2ba0[];
extern const char str_PCTs_font_PCTs_bfn_802c2bac[];
extern const char str_PCTdPCTfPCTdPCTdPCTd_802c2bbc[];
extern const char str_PCTsPCTfPCTdPCTdPCTd_802c2bc8[];
extern const volatile u32 dat_804203d8;
extern const volatile u32 dat_804203dc;
extern const volatile u32 dat_804203e0;
extern const f32 float_1_804203e4;
extern const f32 float_2_804203e8;
extern const f32 float_0p0019531_804203ec;
extern const f32 float_0p00097656_804203f0;
extern const f32 float_0p0039062_804203f4;
extern const f32 float_0_804203f8;
extern const f32 float_255_804203fc;
extern const f32 float_360_80420400;
extern const f32 float_60_80420404;
extern const char str_icon_80420408[];
extern const char str_red_80420410[];
extern const char str_red_80420414[];
extern const f32 float_0p5_8042041c;
extern const char str_col_80420420[];
extern const char str_PCTx_80420424[];
extern const char str_col_80420428[];
extern const f32 float_neg1_80420430;
extern const f32 float_28_80420434;
extern const f32 float_0p015625_80420438;

extern u32 fontColTbl[3];
extern s32 charspace[7];
extern char* fontname[7];

Vec fontScale;
u32 fontColor;
u8 fontAlpha;
s32 fontEdge;
void* pfi;
void* blockGlyph;
u8 fgTexObj[32];
void* pfh;

extern void* gp;
extern u32 unk_80429578;

s32 JUTFont_CodeToGlyph(s32 code);
u16 FontGetMessageWidthLine(const char* msg, s16* lines);
void GXSetNumTexGens(s32 count);
void GXSetNumTevStages(s32 count);
void GXSetTevColor(s32 reg, void* color);
void JUTFontSetup(s32 language);
void _JUTFont_DrawPos(u16 glyph, s16 x, s16 y);
extern void* camGetCurPtr(void);
extern s32 sscanf(const char* str, const char* fmt, ...);
extern s32 msgIconStr2ID(char* name);
extern s32 strcmp(const char* a, const char* b);
extern s32 irand(s32 max);
extern void iconDispGxAlpha(f32 scale, void* pos, s32 flags, s32 iconId, s32 alpha);

void fontmgrInit(void) {
    ;
}

void fontmgrTexSetup(void) {
    JUTFontSetup((u16)*(s32*)((s32)gp + 0x16C));
}

void FontDrawStart(void) {






    extern void JUTFont_DrawStart(void* color);
    u32 color;
    u32 gxColor;
    s32 product;
    s32 quotient;

    color = fontColTbl[0];
    fontScale = vec3_802c2b6c;
    fontEdge = 0;
    fontColor = color;
    fontAlpha = 0xFF;
    product = ((u8*)&color)[3] * 0xFF;
    quotient = __mulhw(0x80808081, product);
    quotient = (quotient + product) >> 7;
    quotient += (u32)quotient >> 31;
    ((u8*)&color)[3] = quotient;
    gxColor = color;
    JUTFont_DrawStart(&gxColor);
}

void FontDrawStart_alpha(u8 alpha) {






    extern void JUTFont_DrawStart(void* color);
    u32 color;
    u32 gxColor;
    s32 product;
    s32 quotient;

    color = fontColTbl[0];
    fontScale = vec3_802c2b6c;
    fontAlpha = alpha;
    fontEdge = 0;
    fontColor = color;
    product = ((u8*)&color)[3] * alpha;
    quotient = __mulhw(0x80808081, product);
    quotient = (quotient + product) >> 7;
    quotient += (u32)quotient >> 31;
    ((u8*)&color)[3] = quotient;
    gxColor = color;
    JUTFont_DrawStart(&gxColor);
}

void FontDrawEdge(void) {
    fontEdge = 1;
}

void FontDrawEdgeOff(void) {
    fontEdge = 0;
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

void FontDrawRainbowColorOff(void) {
    GXSetNumTevStages(1);
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

void FontDrawNoiseOff(void) {
    GXSetNumTexGens(1);
    GXSetNumTevStages(1);
}

void FontDrawColorIDX(s32 idx) {
    u32 color;
    u32 gxColor;
    s32 product;
    s32 quotient;

    color = fontColTbl[idx];
    fontColor = color;
    product = ((u8*)&color)[3] * fontAlpha;
    quotient = __mulhw(0x80808081, product);
    quotient = (quotient + product) >> 7;
    quotient += (u32)quotient >> 31;
    ((u8*)&color)[3] = quotient;
    gxColor = color;
    GXSetTevColor(1, &gxColor);
}

#pragma optimize_for_size off
void FontDrawColor(void* colorPtr) {
    extern void GXSetTevColor(s32 reg, void* color);
    u32 color;

    fontColor = *(u32*)colorPtr;
    ((u8*)colorPtr)[3] = (((u8*)colorPtr)[3] * fontAlpha) / 255;
    color = *(u32*)colorPtr;
    GXSetTevColor(1, &color);
}
#pragma optimize_for_size reset

void FontDrawColor_(void) {
    typedef struct Color {
        u8 r, g, b, a;
    } Color;
    Color color;
    Color oldColor;
    Color gxColor;
    s32 product;
    s32 quotient;

    color = *(Color*)&fontColor;
    oldColor = color;
    product = color.a * fontAlpha;
    quotient = __mulhw(0x80808081, product);
    quotient = (quotient + product) >> 7;
    quotient += (u32)quotient >> 31;
    color.a = quotient;
    fontColor = *(u32*)&oldColor;
    gxColor = color;
    GXSetTevColor(1, &gxColor);
}

u32 FontGetDrawColor(void) {
    return fontColor;
}

void FontDrawScale(f32 scale) {
    fontScale.x = scale;
    fontScale.y = scale;
    fontScale.z = scale;
}

void FontDrawScaleVec(Vec* __restrict scale) {
    fontScale = *scale;
}

#pragma optimize_for_size off
void FontDrawCode(f64 x, f64 y, u16 code) {
    extern void PSMTXScale(void*, f32, f32, f32);
    extern void PSMTXTransApply(void*, void*, f32, f32, f32);

    extern void PSMTXConcat(void*, void*, void*);
    extern void GXLoadPosMtxImm(void*, s32);
    extern void GXSetCurrentMtx(s32);
    extern void GXSetTevColor(s32, void*);
    f32 mtx[3][4];
    u32 oldColor;
    u32 edgeColor;
    u32 gxColor;

    PSMTXScale(mtx, fontScale.x, fontScale.y, fontScale.z);
    PSMTXTransApply(mtx, mtx, (f32)x, (f32)y, float_0_804203f8);
    PSMTXConcat((u8*)camGetCurPtr() + 0x11C, mtx, mtx);
    GXLoadPosMtxImm(mtx, 0);
    GXSetCurrentMtx(0);
    if (fontEdge != 0) {
        edgeColor = fontColTbl[0];
        oldColor = fontColor;
        fontColor = edgeColor;
        ((u8*)&edgeColor)[3] = (((u8*)&edgeColor)[3] * fontAlpha) / 255;
        gxColor = edgeColor;
        GXSetTevColor(1, &gxColor);
        _JUTFont_DrawPos(code, -2, 0);
        _JUTFont_DrawPos(code, 2, 0);
        _JUTFont_DrawPos(code, 0, -2);
        _JUTFont_DrawPos(code, 0, 2);
        fontColor = oldColor;
        ((u8*)&oldColor)[3] = (((u8*)&oldColor)[3] * fontAlpha) / 255;
        gxColor = oldColor;
        GXSetTevColor(1, &gxColor);
    }
    _JUTFont_DrawPos(code, 0, 0);
}
#pragma optimize_for_size reset

#pragma optimize_for_size off
void FontDrawCodeMtx(void* mtx, u16 code) {

    extern void PSMTXConcat(void*, void*, void*);
    extern void GXLoadPosMtxImm(void*, s32);
    extern void GXSetCurrentMtx(s32);
    extern void GXSetTevColor(s32, void*);
    u32 oldColor;
    u32 edgeColor;
    u32 gxColor;

    PSMTXConcat((u8*)camGetCurPtr() + 0x11C, mtx, mtx);
    GXLoadPosMtxImm(mtx, 0);
    GXSetCurrentMtx(0);
    if (fontEdge != 0) {
        edgeColor = fontColTbl[0];
        oldColor = fontColor;
        fontColor = edgeColor;
        ((u8*)&edgeColor)[3] = (((u8*)&edgeColor)[3] * fontAlpha) / 255;
        gxColor = edgeColor;
        GXSetTevColor(1, &gxColor);
        _JUTFont_DrawPos(code, -2, 0);
        _JUTFont_DrawPos(code, 2, 0);
        _JUTFont_DrawPos(code, 0, -2);
        _JUTFont_DrawPos(code, 0, 2);
        fontColor = oldColor;
        ((u8*)&oldColor)[3] = (((u8*)&oldColor)[3] * fontAlpha) / 255;
        gxColor = oldColor;
        GXSetTevColor(1, &gxColor);
    }
    _JUTFont_DrawPos(code, 0, 0);
}
#pragma optimize_for_size reset

void FontDrawString(f32 x, f32 y, s32 str) {
    extern void FontDrawStringVecPitch(void* pos, s32 str, f32 pitch);
    extern const u32 vec3_802c2b78[3];
    u32 out[3];
    u32 vec[3];

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

void FontDrawStringPitch(f32 x, f32 y, s32 str, f32 pitch) {
    extern void FontDrawStringVecPitch(void* pos, s32 str, f32 pitch);
    extern const u32 vec3_802c2b84[3];
    u32 out[3];
    u32 vec[3];

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

#pragma optimize_for_size off
void FontDrawStringVecPitch(Vec* pos, char* msg, f32 pitch) {

    extern void PSMTXScale(void*, f32, f32, f32);
    extern void PSMTXTransApply(void*, void*, f32, f32, f32);
    extern void PSMTXConcat(void*, void*, void*);
    extern void GXLoadPosMtxImm(void*, s32);
    extern s32 _ismbblead(s8);
    extern char* msgGetCommand(char*, char*, char*);
    extern void GXSetTevColor(s32, void*);

    f32 drawMtx[3][4];
    f32 scaleMtx[3][4];
    char command[128];
    char argument[128];
    f32 x;
    f32 drawX;
    s32 glyph;
    void* entry;
    u32 count;
    u8 width;
    u32 oldColor;
    u32 edgeColor;
    u32 gxColor;
    void* cam;

    cam = camGetCurPtr();
    x = pos->x;
    PSMTXScale(scaleMtx, fontScale.x, fontScale.y, fontScale.z);
    while (*msg != 0) {
        glyph = 0xFFFF;
        if (_ismbblead(*msg) != 0) {
            glyph = JUTFont_CodeToGlyph(*(u16*)msg);
            msg += 2;
        } else if (*msg == '<') {
            msg = msgGetCommand(msg + 1, command, argument);
        } else {
            glyph = JUTFont_CodeToGlyph((u8)*msg++);
        }
        if ((u16)glyph == 0xFFFF) {
            continue;
        }

        entry = (u8*)pfh + 0x20;
        count = *(s32*)((u8*)pfh + 0xC);
        while (count > 0) {
            if (*(u32*)entry == 0x57494431 &&
                (u16)glyph >= *(u16*)((u8*)entry + 8) &&
                (u16)glyph <= *(u16*)((u8*)entry + 0xA)) {
                entry = (u8*)entry + (((u16)glyph - *(u16*)((u8*)entry + 8)) * 2) + 0xC;
                goto width_found;
            }
            entry = (u8*)entry + *(s32*)((u8*)entry + 4);
            count--;
        }
        entry = NULL;

    width_found:
        width = *(u8*)((u8*)entry + 1);

        if (pitch < float_0_804203f8) {
            drawX = x;
        } else {
            drawX = x + fontScale.x * (float_0p5_8042041c * (pitch - (f32)width));
        }
        PSMTXTransApply(scaleMtx, drawMtx, drawX, pos->y, pos->z);
        PSMTXConcat((u8*)cam + 0x11C, drawMtx, drawMtx);
        GXLoadPosMtxImm(drawMtx, 0);
        if (fontEdge != 0) {
            edgeColor = fontColTbl[0];
            oldColor = fontColor;
            fontColor = edgeColor;
            ((u8*)&edgeColor)[3] = (((u8*)&edgeColor)[3] * fontAlpha) / 255;
            gxColor = edgeColor;
            GXSetTevColor(1, &gxColor);
            _JUTFont_DrawPos((u16)glyph, -2, 0);
            _JUTFont_DrawPos((u16)glyph, 2, 0);
            _JUTFont_DrawPos((u16)glyph, 0, -2);
            _JUTFont_DrawPos((u16)glyph, 0, 2);
            fontColor = oldColor;
            ((u8*)&oldColor)[3] = (((u8*)&oldColor)[3] * fontAlpha) / 255;
            gxColor = oldColor;
            GXSetTevColor(1, &gxColor);
        }
        _JUTFont_DrawPos((u16)glyph, 0, 0);
        if (pitch < float_0_804203f8) {
            x += (f32)(width + charspace[*(s32*)((u8*)gp + 0x16C)]);
        } else {
            x += pitch;
        }
    }
}
#pragma optimize_for_size reset

#pragma optimize_for_size off
void FontDrawStringMtx(f32* mtx, void* string) {

    extern void PSMTXConcat(void* a, void* b, void* out);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern s32 _ismbblead(s32 c);
    extern char* msgGetCommand(char* in, char* cmd, char* arg);
    extern void GXSetTevColor(s32 reg, void* color);

    f32 localMtx[3][4];
    char arg[128];
    char cmd[128];
    char* p;
    f32 x;
    f32 y;
    s32 glyph;
    s32 count;
    void* entry;
    u32 edgeColor;
    u32 oldColor;
    u32 gxColor;

    PSMTXConcat((void*)((s32)camGetCurPtr() + 0x11C), mtx, localMtx);
    GXLoadPosMtxImm(localMtx, 0);
    p = (char*)string;
    x = float_0_804203f8;
    y = float_0_804203f8;

    while (*p != 0) {
        glyph = 0xFFFF;
        if (_ismbblead(*p) != 0) {
            glyph = JUTFont_CodeToGlyph(*(u16*)p);
            p += 2;
        } else if (*p == '<') {
            p = msgGetCommand(p + 1, cmd, arg);
            if (cmd[0] != 'R') {
                if (*p == '\n') {
                    p++;
                }
            }
        } else if (*p == '\n') {
            p++;
            y -= float_28_80420434;
            x = float_0_804203f8;
        } else {
            glyph = JUTFont_CodeToGlyph((u8)*p++);
        }

        if ((u16)glyph == 0xFFFF) {
            continue;
        }

        if (fontEdge != 0) {
            edgeColor = fontColTbl[0];
            oldColor = fontColor;
            fontColor = edgeColor;
            ((u8*)&edgeColor)[3] = (((u8*)&edgeColor)[3] * fontAlpha) / 255;
            gxColor = edgeColor;
            GXSetTevColor(1, &gxColor);

            _JUTFont_DrawPos((u16)glyph, (s16)((s32)x - 2), (s16)(s32)y);
            _JUTFont_DrawPos((u16)glyph, (s16)((s32)x + 2), (s16)(s32)y);
            _JUTFont_DrawPos((u16)glyph, (s16)(s32)x, (s16)((s32)y - 2));
            _JUTFont_DrawPos((u16)glyph, (s16)(s32)x, (s16)((s32)y + 2));

            fontColor = oldColor;
            ((u8*)&oldColor)[3] = (((u8*)&oldColor)[3] * fontAlpha) / 255;
            gxColor = oldColor;
            GXSetTevColor(1, &gxColor);
        }

        _JUTFont_DrawPos((u16)glyph, (s16)((s32)x), (s16)((s32)y));

        entry = (void*)((s32)pfh + 0x20);
        count = *(s32*)((s32)pfh + 0xC);
        while (count > 0) {
            if (*(u32*)entry == 0x57494431 &&
                (u16)glyph >= *(u16*)((s32)entry + 8) &&
                (u16)glyph <= *(u16*)((s32)entry + 0xA)) {
                entry = (void*)((s32)entry + (((u16)glyph - *(u16*)((s32)entry + 8)) * 2) + 0xC);
                break;
            }
            entry = (void*)((s32)entry + *(s32*)((s32)entry + 4));
            count--;
        }
        x += (f32)(*(u8*)((s32)entry + 1) + charspace[*(s32*)((s32)gp + 0x16C)]);
    }
}
#pragma optimize_for_size reset

#pragma optimize_for_size off
void FontDrawStringCenterMtx(void* mtx, char* msg) {

    extern void PSMTXConcat(void*, void*, void*);
    extern void GXLoadPosMtxImm(void*, s32);
    extern s32 _ismbblead(s8);
    extern void GXSetTevColor(s32, void*);

    f32 drawMtx[3][4];
    f32 x;
    s32 glyph;
    void* entry;
    s32 count;
    s32 magic;
    u32 oldColor;
    u32 edgeColor;
    u32 gxColor;
    void* cam;

    cam = camGetCurPtr();
    x = -float_0p5_8042041c * FontGetMessageWidthLine(msg, 0);
    PSMTXConcat((u8*)cam + 0x11C, mtx, drawMtx);
    GXLoadPosMtxImm(drawMtx, 0);

    magic = 0x57494431;
    while (*msg != 0) {
        if (_ismbblead(*msg) != 0) {
            glyph = JUTFont_CodeToGlyph(*(u16*)msg);
            msg += 2;
        } else {
            glyph = JUTFont_CodeToGlyph((u8)*msg++);
        }

        if (fontEdge != 0) {
            edgeColor = fontColTbl[0];
            oldColor = fontColor;
            fontColor = edgeColor;
            ((u8*)&edgeColor)[3] = (((u8*)&edgeColor)[3] * fontAlpha) / 255;
            gxColor = edgeColor;
            GXSetTevColor(1, &gxColor);
            _JUTFont_DrawPos((u16)glyph, (s16)((s32)x - 2), 0);
            _JUTFont_DrawPos((u16)glyph, (s16)((s32)x + 2), 0);
            _JUTFont_DrawPos((u16)glyph, (s16)(s32)x, -2);
            _JUTFont_DrawPos((u16)glyph, (s16)(s32)x, 2);
            fontColor = oldColor;
            ((u8*)&oldColor)[3] = (((u8*)&oldColor)[3] * fontAlpha) / 255;
            gxColor = oldColor;
            GXSetTevColor(1, &gxColor);
        }
        _JUTFont_DrawPos((u16)glyph, (s16)(s32)x, 0);

        entry = (u8*)pfh + 0x20;
        count = *(s32*)((u8*)pfh + 0xC);
        while (count > 0) {
            if (*(u32*)entry == magic &&
                (u16)glyph >= *(u16*)((u8*)entry + 8) &&
                (u16)glyph <= *(u16*)((u8*)entry + 0xA)) {
                entry = (u8*)entry + (((u16)glyph - *(u16*)((u8*)entry + 8)) * 2) + 0xC;
                goto width_found;
            }
            entry = (u8*)entry + *(s32*)((u8*)entry + 4);
            count--;
        }

        entry = NULL;

    width_found:
        x += (f32)(*(u8*)((u8*)entry + 1) + charspace[*(s32*)((u8*)gp + 0x16C)]);
    }
}
#pragma optimize_for_size reset

#pragma optimize_for_size off
void FontDrawStringShake(double xIn, double yIn, char* str) {

    extern void PSMTXScale(f32 mtx[3][4], f32 x, f32 y, f32 z);
    extern void PSMTXTransApply(f32 src[3][4], f32 dst[3][4], f32 x, f32 y, f32 z);
    extern void PSMTXConcat(void* a, void* b, void* out);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern s32 _ismbblead(s32 c);

    extern void GXSetTevColor(s32 reg, void* color);

    f32 scaleMtx[3][4];
    f32 drawMtx[3][4];
    char* p;
    f32 drawX;
    f32 drawY;
    s32 glyph;
    u32 count;
    void* entry;
    u32 magic;
    u32 edgeColor;
    u32 oldColor;
    volatile u32 alphaEdgeColor;
    volatile u32 alphaOldColor;
    volatile u32 gxEdgeColor;
    volatile u32 gxOldColor;
    s32 jitterX;
    s32 jitterY;
    void* cam;

    p = str;
    cam = camGetCurPtr();
    PSMTXScale(scaleMtx, fontScale.x, fontScale.y, fontScale.z);

    while (*p != 0) {
        if (_ismbblead(*p) != 0) {
            glyph = JUTFont_CodeToGlyph(*(u16*)p);
            p += 2;
        } else {
            glyph = JUTFont_CodeToGlyph((u8)*p);
            p++;
        }

        jitterX = irand(10000) % 3;
        drawX = (f32)(xIn + (fontScale.x * (f32)jitterX));
        jitterY = irand(10000) % 3;
        drawY = (f32)(yIn + (fontScale.y * (f32)jitterY));

        entry = (void*)((s32)pfh + 0x20);
        magic = 0x57494431;
        count = *(s32*)((s32)pfh + 0xC);
        while (count != 0) {
            if (*(u32*)entry == magic &&
                (u16)glyph >= *(u16*)((s32)entry + 8) &&
                (u16)glyph <= *(u16*)((s32)entry + 0xA)) {
                entry = (void*)((s32)entry + (((u16)glyph - *(u16*)((s32)entry + 8)) * 2) + 0xC);
                goto width_found;
            }
            entry = (void*)((s32)entry + *(s32*)((s32)entry + 4));
            count--;
        }

        entry = NULL;

    width_found:
        PSMTXTransApply(scaleMtx, drawMtx,
                        drawX,
                        drawY,
                        float_0_804203f8);
        PSMTXConcat((void*)((s32)cam + 0x11C), drawMtx, drawMtx);
        GXLoadPosMtxImm(drawMtx, 0);

        if (fontEdge != 0) {
            edgeColor = fontColTbl[0];
            oldColor = fontColor;
            fontColor = edgeColor;
            alphaEdgeColor = edgeColor;
            ((u8*)&alphaEdgeColor)[3] = (((u8*)&alphaEdgeColor)[3] * fontAlpha) / 255;
            gxEdgeColor = alphaEdgeColor;
            GXSetTevColor(1, (void*)&gxEdgeColor);

            _JUTFont_DrawPos(glyph, -2, 0);
            _JUTFont_DrawPos(glyph, 2, 0);
            _JUTFont_DrawPos(glyph, 0, -2);
            _JUTFont_DrawPos(glyph, 0, 2);

            fontColor = oldColor;
            alphaOldColor = oldColor;
            ((u8*)&alphaOldColor)[3] = (((u8*)&alphaOldColor)[3] * fontAlpha) / 255;
            gxOldColor = alphaOldColor;
            GXSetTevColor(1, (void*)&gxOldColor);
        }

        _JUTFont_DrawPos(glyph, 0, 0);
        xIn += fontScale.x * (f32)(*(u8*)((s32)entry + 1) + charspace[*(s32*)((s32)gp + 0x16C)]);
    }
}
#pragma optimize_for_size reset

#pragma optimize_for_size off
void FontDrawMessage(s32 x0, s32 y0, char* msg) {

    extern u32 JUTFont_CodeToGlyph(u32 code);
    extern s32 _ismbblead(s32 c);
    extern char* msgGetCommand(char* in, char* cmd, char* arg);



    extern void iconGetWidthHight(u16* w, u16* h, s32 iconId);

    extern void GXSetTevColor(s32 reg, void* color);
    extern void JUTFont_DrawStart(void* color);

    u16 iconH;
    u16 iconW;
    u32 glyph;
    volatile u32 colorA;
    volatile u32 colorB;
    volatile u32 colorC;
    volatile u32 colorCSource;
    volatile u32 colorD;
    volatile u32 colorE;
    volatile u32 colorF;
    volatile u32 colorG;
    volatile u32 savedColor;
    volatile u32 alphaColorA;
    volatile u32 alphaColorB;
    volatile u32 alphaColorC;
    volatile u32 alphaColorD;
    volatile u32 alphaColorF;
    volatile u32 alphaColorG;
    volatile u32 restoredAlphaColor;
    volatile u32 gxColorA;
    volatile u32 gxColorB;
    volatile u32 gxColorC;
    volatile u32 gxColorD;
    volatile u32 gxColorE;
    volatile u32 gxColorF;
    volatile u32 gxColorG;
    volatile u32 restoredGxColor;
    s32 iconId;
    s32 iconDx;
    s32 iconDy;
    s32 iconAdd;
    f32 iconScale;
    Vec iconPos;
    char iconName[32];
    char cmd[128];
    char arg[128];
    const char* paper;
    char* p;
    s32 x;
    s32 y;

    paper = str_papermarioset_JPN_802c2b30;
    x = x0;
    y = y0;
    p = msg;

    while (*p != 0) {
        glyph = 0xFFFF;

        if (_ismbblead((s32)(s8)*p) != 0) {
            glyph = JUTFont_CodeToGlyph(*(u16*)p);
            p += 2;
        } else {
            s32 ch = (s32)(s8)*p;

            if (ch == '<') {
                p = msgGetCommand(p + 1, cmd, arg);

                if (strcmp(cmd, str_red_80420410) == 0) {
                    colorA = dat_804203d8;
                    fontColor = colorA;
                    alphaColorA = colorA;
                    ((u8*)&alphaColorA)[3] = (u8)((((u8*)&alphaColorA)[3] * fontAlpha) / 255);
                    gxColorA = alphaColorA;
                    GXSetTevColor(1, (void*)&gxColorA);
                } else if (strcmp(cmd, str_red_80420414) == 0) {
                    colorB = fontColTbl[0];
                    fontColor = colorB;
                    alphaColorB = colorB;
                    ((u8*)&alphaColorB)[3] = (u8)((((u8*)&alphaColorB)[3] * fontAlpha) / 255);
                    gxColorB = alphaColorB;
                    GXSetTevColor(1, (void*)&gxColorB);
                } else if (strcmp(cmd, str_col_80420420) == 0) {
                    sscanf(arg, str_PCTx_80420424, &colorC);
                    colorCSource = colorC;
                    fontColor = colorCSource;
                    alphaColorC = colorCSource;
                    ((u8*)&alphaColorC)[3] = (u8)((((u8*)&alphaColorC)[3] * fontAlpha) / 255);
                    gxColorC = alphaColorC;
                    GXSetTevColor(1, (void*)&gxColorC);
                } else if (strcmp(cmd, str_col_80420428) == 0) {
                    colorD = fontColTbl[0];
                    fontColor = colorD;
                    alphaColorD = colorD;
                    ((u8*)&alphaColorD)[3] = (u8)((((u8*)&alphaColorD)[3] * fontAlpha) / 255);
                    gxColorD = alphaColorD;
                    GXSetTevColor(1, (void*)&gxColorD);
                } else if (strcmp(cmd, str_icon_80420408) == 0) {
                    camGetCurPtr();

                    iconScale = 1.0f;

                    if ((s8)arg[0] >= '0' && (s8)arg[0] <= '9') {
                        sscanf(arg, paper + 0x8C, &iconId, &iconScale, &iconDx, &iconDy, &iconAdd);
                    } else {
                        sscanf(arg, paper + 0x98, iconName, &iconScale, &iconDx, &iconDy, &iconAdd);
                        iconId = msgIconStr2ID(iconName);
                        if (iconId < 0) {
                            iconId = 0;
                        }
                    }

                    iconGetWidthHight(&iconW, &iconH, (u16)iconId);

                    iconPos.x = ((f32)x + ((f32)iconW * float_0p5_8042041c * iconScale)) + (f32)iconDx;
                    iconPos.y = (f32)iconDy + -(((f32)iconH * iconScale) - (f32)y);
                    iconPos.z = float_0_804203f8;
                    iconDispGxAlpha(iconScale, &iconPos, 0x10, (u16)iconId, fontAlpha);

                    fontEdge = 0;
                    fontColor = fontColTbl[0];
                    fontScale = vec3_802c2b6c;

                    colorE = fontColTbl[0];
                    ((u8*)&colorE)[3] = (u8)((((u8*)&colorE)[3] * fontAlpha) / 255);
                    gxColorE = colorE;
                    JUTFont_DrawStart((void*)&gxColorE);

                    colorF = fontColor;
                    alphaColorF = colorF;
                    ((u8*)&alphaColorF)[3] = (u8)((((u8*)&alphaColorF)[3] * fontAlpha) / 255);
                    gxColorF = alphaColorF;
                    GXSetTevColor(1, (void*)&gxColorF);

                    x = (s32)((f32)x + ((f32)iconW * iconScale) + (f32)iconAdd);
                } else {
                    if (cmd[0] == 'R') {
                        goto message_glyph;
                    } else if (cmd[0] == '/' && *(u16*)cmd == 0x2F72) {
                        goto message_glyph;
                    }
                }

                if (*p == '\n') {
                    p++;
                }
            } else if (ch == '\n') {
                y -= 0x1C;
                p++;
                x = x0;
            } else {
                glyph = JUTFont_CodeToGlyph((u8)*p);
                p++;
            }
        }

message_glyph:
        if ((u16)glyph != 0xFFFF) {
            void* entry;
            u32 count;
            u32 magic;
            u16 drawGlyph;
            s16 drawX;
            s16 drawY;

            drawGlyph = (u16)glyph;
            drawX = (s16)x;
            drawY = (s16)y;

            if (fontEdge != 0) {
                colorG = fontColTbl[0];
                savedColor = fontColor;
                fontColor = colorG;
                alphaColorG = colorG;
                ((u8*)&alphaColorG)[3] = (u8)((((u8*)&alphaColorG)[3] * fontAlpha) / 255);
                gxColorG = alphaColorG;
                GXSetTevColor(1, (void*)&gxColorG);

                _JUTFont_DrawPos(drawGlyph, drawX - 2, drawY);
                _JUTFont_DrawPos(drawGlyph, drawX + 2, drawY);
                _JUTFont_DrawPos(drawGlyph, drawX, drawY - 2);
                _JUTFont_DrawPos(drawGlyph, drawX, drawY + 2);

                fontColor = savedColor;
                restoredAlphaColor = savedColor;
                ((u8*)&restoredAlphaColor)[3] = (u8)((((u8*)&restoredAlphaColor)[3] * fontAlpha) / 255);
                restoredGxColor = restoredAlphaColor;
                GXSetTevColor(1, (void*)&restoredGxColor);
            }

            _JUTFont_DrawPos(drawGlyph, drawX, drawY);

            entry = (void*)((s32)pfh + 0x20);
            magic = 0x57494431;
            count = *(s32*)((s32)pfh + 0xC);
            while (count != 0) {
                if (*(u32*)entry == magic &&
                    drawGlyph >= *(u16*)((s32)entry + 8) &&
                    drawGlyph <= *(u16*)((s32)entry + 0xA)) {
                    entry = (void*)((s32)entry + ((drawGlyph - *(u16*)((s32)entry + 8)) * 2) + 0xC);
                    goto width_found;
                }
                entry = (void*)((s32)entry + *(s32*)((s32)entry + 4));
                count--;
            }

            entry = NULL;

        width_found:
            x += *(u8*)((s32)entry + 1) + charspace[*(s32*)((s32)gp + 0x16C)];
        }
    }
}
#pragma optimize_for_size reset

void FontDrawMessageMtx(f32 mtx[3][4], char* msg) {
    extern u32 JUTFont_CodeToGlyph(u32 code);
    extern s32 _ismbblead(s32 c);
    extern char* msgGetCommand(char* in, char* cmd, char* arg);
    extern void iconGetWidthHight(u16* w, u16* h, s32 iconId);
    extern void PSMTXMultVec(void* mtx, Vec* src, Vec* dst);
    extern void PSMTXTrans(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXConcat(void* a, void* b, void* out);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXSetTevColor(s32 reg, void* color);
    extern void JUTFont_DrawStart(void* color);

    u16 iconH;
    u16 iconW;
    u32 glyph;
    volatile u32 colorA;
    volatile u32 colorB;
    volatile u32 colorC;
    volatile u32 colorD;
    volatile u32 colorE;
    volatile u32 savedColor;
    volatile u32 alphaColorA;
    volatile u32 alphaColorB;
    volatile u32 alphaColorC;
    volatile u32 alphaColorD;
    volatile u32 alphaColorE;
    volatile u32 restoredAlphaColor;
    volatile u32 gxColorA;
    volatile u32 gxColorB;
    volatile u32 gxColorC;
    volatile u32 gxColorD;
    volatile u32 gxColorE;
    s32 iconId;
    s32 iconDx;
    s32 iconDy;
    s32 iconAdd;
    f32 iconScale;
    Vec iconPos;
    char iconName[32];
    f32 transMtx[3][4];
    f32 workMtx[3][4];
    char cmd[128];
    char arg[128];
    void* cam;
    const u8* rodata;
    char* p;
    s32 x;
    s32 y;

    rodata = (const u8*)str_papermarioset_JPN_802c2b30;
    cam = camGetCurPtr();
    p = msg;
    x = (s32)float_0_804203f8;
    y = 0;

    while (*p != 0) {
        glyph = 0xFFFF;

        if (_ismbblead((s32)(s8)*p) != 0) {
            glyph = JUTFont_CodeToGlyph(*(u16*)p);
            p += 2;
        } else {
            s32 ch = (s32)(s8)*p;

            if (ch == '<') {
                p = msgGetCommand(p + 1, cmd, arg);

                if (strcmp(cmd, str_red_80420410) == 0) {
                    colorA = dat_804203dc;
                    fontColor = colorA;
                    alphaColorA = colorA;
                    ((u8*)&alphaColorA)[3] = (u8)((((u8*)&alphaColorA)[3] * fontAlpha) / 255);
                    gxColorA = alphaColorA;
                    GXSetTevColor(1, (void*)&gxColorA);
                } else if (strcmp(cmd, str_red_80420414) == 0) {
                    colorB = fontColTbl[0];
                    fontColor = colorB;
                    alphaColorB = colorB;
                    ((u8*)&alphaColorB)[3] = (u8)((((u8*)&alphaColorB)[3] * fontAlpha) / 255);
                    gxColorB = alphaColorB;
                    GXSetTevColor(1, (void*)&gxColorB);
                } else if (strcmp(cmd, str_icon_80420408) == 0) {
                    camGetCurPtr();

                    if ((s8)arg[0] >= '0' && (s8)arg[0] <= '9') {
                        sscanf(arg, (const char*)(rodata + 0x8C), &iconId, &iconScale, &iconDx, &iconDy, &iconAdd);
                    } else {
                        sscanf(arg, (const char*)(rodata + 0x98), iconName, &iconScale, &iconDx, &iconDy, &iconAdd);
                        iconId = msgIconStr2ID(iconName);
                        if (iconId < 0) {
                            iconId = 0;
                        }
                    }

                    iconGetWidthHight(&iconW, &iconH, (u16)iconId);

                    iconPos.x = ((f32)x + ((f32)iconW * float_0p5_8042041c * iconScale)) + (f32)iconDx;
                    iconPos.y = (f32)iconDy + -(((f32)iconH * iconScale) - (f32)y);
                    iconPos.z = float_0_804203f8;
                    PSMTXMultVec(mtx, &iconPos, &iconPos);
                    iconDispGxAlpha(iconScale, &iconPos, 0x10, (u16)iconId, fontAlpha);

                    fontEdge = 0;
                    fontColor = fontColTbl[0];
                    fontScale = *(Vec*)(rodata + 0x3C);

                    colorC = fontColTbl[0];
                    alphaColorC = colorC;
                    ((u8*)&alphaColorC)[3] = (u8)((((u8*)&alphaColorC)[3] * fontAlpha) / 255);
                    gxColorC = alphaColorC;
                    JUTFont_DrawStart((void*)&gxColorC);

                    colorD = fontColor;
                    alphaColorD = colorD;
                    ((u8*)&alphaColorD)[3] = (u8)((((u8*)&alphaColorD)[3] * fontAlpha) / 255);
                    gxColorD = alphaColorD;
                    GXSetTevColor(1, (void*)&gxColorD);

                    x = (s32)((f32)x + ((f32)iconW * iconScale) + (f32)iconAdd);
                } else {
                    if (cmd[0] == 'R') {
                        goto message_mtx_glyph;
                    } else if (cmd[0] == '/' && *(u16*)cmd == 0x2F72) {
                        goto message_mtx_glyph;
                    }
                }

                if (*p == '\n') {
                    p++;
                }
            } else if (ch == '\n') {
                y -= 0x1C;
                p++;
                x = (s32)float_0_804203f8;
            } else {
                glyph = JUTFont_CodeToGlyph((u8)*p);
                p++;
            }
        }

message_mtx_glyph:
        if ((u16)glyph != 0xFFFF) {
            void* entry;
            s32 count;

            PSMTXTrans(transMtx, (f32)x, (f32)y, float_0_804203f8);
            PSMTXConcat((void*)((s32)cam + 0x11C), mtx, workMtx);
            PSMTXConcat(workMtx, transMtx, workMtx);
            GXLoadPosMtxImm(workMtx, 0);

            if (fontEdge != 0) {
                colorE = fontColTbl[0];
                savedColor = fontColor;
                fontColor = colorE;
                alphaColorE = colorE;
                ((u8*)&alphaColorE)[3] = (u8)((((u8*)&alphaColorE)[3] * fontAlpha) / 255);
                gxColorE = alphaColorE;
                GXSetTevColor(1, (void*)&gxColorE);

                _JUTFont_DrawPos((u16)glyph, -2, 0);
                _JUTFont_DrawPos((u16)glyph, 2, 0);
                _JUTFont_DrawPos((u16)glyph, 0, -2);
                _JUTFont_DrawPos((u16)glyph, 0, 2);

                fontColor = savedColor;
                restoredAlphaColor = savedColor;
                ((u8*)&restoredAlphaColor)[3] = (u8)((((u8*)&restoredAlphaColor)[3] * fontAlpha) / 255);
                gxColorE = restoredAlphaColor;
                GXSetTevColor(1, (void*)&gxColorE);
            }

            _JUTFont_DrawPos((u16)glyph, 0, 0);

            entry = (void*)((s32)pfh + 0x20);
            count = *(s32*)((s32)pfh + 0xC);
            while (count > 0) {
                if (*(u32*)entry == 0x57494431 &&
                    (u16)glyph >= *(u16*)((s32)entry + 8) &&
                    (u16)glyph <= *(u16*)((s32)entry + 0xA)) {
                    entry = (void*)((s32)entry + (((u16)glyph - *(u16*)((s32)entry + 8)) * 2) + 0xC);
                    break;
                }
                entry = (void*)((s32)entry + *(s32*)((s32)entry + 4));
                count--;
            }

            x += *(u8*)((s32)entry + 1) + charspace[*(s32*)((s32)gp + 0x16C)];
        }
    }
}

s32 hankakuSearch(s32 code) {
    return JUTFont_CodeToGlyph((u8)code);
}

s32 kanjiSearch(s32 code) {
    return JUTFont_CodeToGlyph(code);
}

u8 kanjiGetWidth(u16 code) {
    void* entry;
    u32 count;
    u32 i;

    entry = (u8*)pfh + 0x20;
    count = *(u32*)((u8*)pfh + 0xC);
    for (i = 0; i < count; i++) {
        switch (*(u32*)entry) {
            case 0x57494431: {
                u16 start = *(u16*)((u8*)entry + 8);
                if (code >= start && code <= *(u16*)((u8*)entry + 0xA)) {
                    entry = (u8*)entry + ((code - start) * 2) + 0xC;
                    goto found;
                }
                break;
            }
            default:
                break;
        }
        entry = (u8*)entry + *(s32*)((u8*)entry + 4);
    }
    entry = 0;
found:
    return *(u8*)((u8*)entry + 1);
}

u16 FontGetMessageWidthLine(const char* msg, s16* line) {
    extern s32 _ismbblead(s8 c);
    extern char* msgGetCommand(char* in, char* cmd, char* arg);
    extern void iconGetWidthHight(u16* w, u16* h, u16 id);

    s32 glyph;
    u16 maxWidth;
    u16 width;
    s16 lines;
    char cmd[128];
    char arg[128];
    char iconName[32];
    s32 iconId;
    s32 a;
    s32 b;
    s32 add;
    f32 scale;
    u16 iconW;
    u16 iconH;
    void* entry;
    u32 count;
    u32 i;

    maxWidth = 0;
    width = 0;
    lines = 0;

    while (*msg != 0) {
        glyph = 0xFFFF;
        if (_ismbblead(*msg) != 0) {
            glyph = JUTFont_CodeToGlyph(*(u16*)msg);
            msg += 2;
        } else {
            switch ((s8)*msg) {
                case '\n':
                    if ((u16)maxWidth < (u16)width) {
                        maxWidth = width;
                    }
                    width = 0;
                    lines++;
                    msg++;
                    break;

                case '<':
                    msg = msgGetCommand((char*)msg + 1, cmd, arg);
                    if (strcmp(cmd, str_icon_80420408) == 0) {
                        camGetCurPtr();
                        if ((s8)arg[0] >= '0' && (s8)arg[0] <= '9') {
                            sscanf(arg, str_PCTdPCTfPCTdPCTdPCTd_802c2bbc,
                                   &iconId, &scale, &a, &b, &add);
                        } else {
                            sscanf(arg, str_PCTsPCTfPCTdPCTdPCTd_802c2bc8,
                                   iconName, &scale, &a, &b, &add);
                            iconId = msgIconStr2ID(iconName);
                            if (iconId < 0) {
                                iconId = 0;
                            }
                        }
                        iconGetWidthHight(&iconW, &iconH, (u16)iconId);
                        width = (u16)(((f32)(u16)iconW * scale) + (f32)add +
                                      (f32)(u16)width);
                    } else if ((s8)cmd[0] == 'R' ||
                               ((s8)cmd[0] == '/' && *(u16*)cmd == 0x2F72)) {
                        break;
                    }
                    if ((s8)*msg == '\n') {
                        msg++;
                    }
                    break;

                default:
                    glyph = JUTFont_CodeToGlyph((u8)*msg++);
                    break;
            }
        }

        if ((u16)glyph == 0xFFFF) {
            continue;
        }

        entry = (u8*)pfh + 0x20;
        count = *(u32*)((u8*)pfh + 0xC);
        for (i = 0; i < count; i++) {
            switch (*(u32*)entry) {
                case 0x57494431:
                    if ((u16)glyph >= *(u16*)((u8*)entry + 8) &&
                        (u16)glyph <= *(u16*)((u8*)entry + 0xA)) {
                        entry = (u8*)entry +
                                (((u16)glyph - *(u16*)((u8*)entry + 8)) * 2) + 0xC;
                        goto width_found;
                    }
                    break;
                default:
                    break;
            }
            entry = (u8*)entry + *(s32*)((u8*)entry + 4);
        }
        entry = 0;
width_found:
        width = (u16)(*(u8*)((u8*)entry + 1) + width +
                      charspace[*(s32*)((u8*)gp + 0x16C)]);
    }

    if ((u16)maxWidth < (u16)width) {
        maxWidth = width;
    }
    if ((u16)maxWidth != 0) {
        maxWidth = (u16)(maxWidth - charspace[*(s32*)((u8*)gp + 0x16C)]);
    }
    if (line != 0) {
        *line = lines;
    }
    return (u16)maxWidth;
}

u16 FontGetMessageWidth(const char* msg) {
    return FontGetMessageWidthLine(msg, 0);
}

u32 HSV2RGB(u8* hsv) {
    extern f64 floor(f64 x);
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

void JUTFontSetup(s32 language) {
    extern s32 getMarioStDvdRoot(void);
    extern s32 sprintf(char*, const char*, ...);
    extern void* DVDMgrOpen(const char*, s32, s32);
    extern s32 DVDMgrGetLength(void*);
    extern void DVDMgrClose(void*);
    extern void* __memAlloc(s32, u32);
    extern void DVDMgrRead(void*, void*, u32, s32);
    extern void GXInitTexObj(void*, void*, u16, u16, s32, s32, s32, s32);
    extern void GXInitTexObjLOD(void*, s32, s32, f32, f32, f32, s32, s32, s32);

    char path[140];
    void* dvd;
    void* check;
    void* entry;
    void* widthEntry;
    s32 length;
    s32 maxLength;
    s32 checkLength;
    u32 i;
    u16 glyph;
    char** name;

    if (pfh != 0) {
        return;
    }

    sprintf(path, str_PCTs_f_PCTs_bfn_802c2ba0,
            getMarioStDvdRoot(), fontname[(u16)language]);
    dvd = DVDMgrOpen(path, 2, 0);
    length = DVDMgrGetLength(dvd);
    maxLength = length;
    name = fontname;
    for (i = 0; i < 7; i++, name++) {
        sprintf(path, str_PCTs_font_PCTs_bfn_802c2bac,
                getMarioStDvdRoot(), *name);
        check = DVDMgrOpen(path, 2, 0);
        if (check != 0) {
            checkLength = DVDMgrGetLength(check);
            if (maxLength < checkLength) {
                maxLength = checkLength;
            }
            DVDMgrClose(check);
        }
    }

    pfh = __memAlloc(0, (maxLength + 0x1F) & ~0x1F);
    DVDMgrRead(dvd, pfh, (length + 0x1F) & ~0x1F, 0);
    DVDMgrClose(dvd);

    entry = (u8*)pfh + 0x20;
    for (i = 0; i < *(s32*)((u8*)pfh + 0xC); i++) {
        switch (*(u32*)entry) {
            case 0x494E4631:
                pfi = entry;
                break;
            case 0x474C5931:
                blockGlyph = entry;
                break;
            case 0x4D415031:
            case 0x57494431:
            default:
                break;
        }
        entry = (u8*)entry + *(s32*)((u8*)entry + 4);
    }

    GXInitTexObj(fgTexObj, (u8*)blockGlyph + 0x20,
                 *(u16*)((u8*)blockGlyph + 0x1A),
                 *(u16*)((u8*)blockGlyph + 0x1C), 0, 0, 0, 0);
    GXInitTexObjLOD(fgTexObj, 1, 1, float_0_804203f8,
                    float_0_804203f8, float_0_804203f8, 0, 0, 0);

    glyph = (u16)JUTFont_CodeToGlyph(0x20);
    entry = (u8*)pfh + 0x20;
    widthEntry = 0;
    for (i = *(s32*)((u8*)pfh + 0xC); i != 0; i--) {
        switch (*(u32*)entry) {
            case 0x57494431:
                if (glyph >= *(u16*)((u8*)entry + 8) &&
                    glyph <= *(u16*)((u8*)entry + 0xA)) {
                    widthEntry = (u8*)entry + (glyph - *(u16*)((u8*)entry + 8)) * 2 + 0xC;
                    goto width_20_found;
                }
                break;
            default:
                break;
        }
        entry = (u8*)entry + *(s32*)((u8*)entry + 4);
    }
width_20_found:
    *(u8*)((u8*)widthEntry + 1) = 7;

    glyph = (u16)JUTFont_CodeToGlyph(0x8140);
    entry = (u8*)pfh + 0x20;
    widthEntry = 0;
    for (i = *(s32*)((u8*)pfh + 0xC); i != 0; i--) {
        switch (*(u32*)entry) {
            case 0x57494431:
                if (glyph >= *(u16*)((u8*)entry + 8) &&
                    glyph <= *(u16*)((u8*)entry + 0xA)) {
                    widthEntry = (u8*)entry + (glyph - *(u16*)((u8*)entry + 8)) * 2 + 0xC;
                    goto width_8140_found;
                }
                break;
            default:
                break;
        }
        entry = (u8*)entry + *(s32*)((u8*)entry + 4);
    }
width_8140_found:
    *(u8*)((u8*)widthEntry + 1) = 9;
}

s32 JUTFont_CodeToGlyph(s32 code) {
    typedef struct BinaryFontMapLocal {
        u32 magic;
        u32 size;
        u16 mapping;
        u16 firstchar;
        u16 lastchar;
        u16 count;
        u16 table[1];
    } BinaryFontMapLocal;
    extern void* pfh;

    BinaryFontMapLocal* map;
    u32 blockcount;
    u16 glyph, basecode;
    u8 sjis[2];
    u32 lead, index;
    s32 max, min;
    u32 i;

    map = (BinaryFontMapLocal*)((u8*)pfh + 0x20);
    glyph = *(u16*)((u8*)pfi + 0x12);
    blockcount = *(u32*)((u8*)pfh + 0xC);
    for (i = 0; i < blockcount; i++) {
        if (map->magic == 0x4D415031) {
            basecode = map->firstchar;
            if ((u16)code >= basecode && (u16)code <= map->lastchar) {
                switch (map->mapping) {
                    case 0:
                        glyph = (u16)(code - basecode);
                        break;
                    case 1:
                        *(u16*)sjis = (u16)code;
                        lead = sjis[0];
                        index = sjis[1];
                        if (sjis[1] >= 0x80) {
                            index -= 65;
                        } else {
                            index -= 64;
                        }
                        if (!map->count) {
                            glyph = (u16)(((lead - 136) * 188) + index + 702);
                        } else {
                            glyph = (u16)(((lead - 136) * 188) + index - 94 + map->table[0]);
                        }
                        break;
                    case 2:
                        glyph = map->table[(u16)code - basecode];
                        break;
                    case 3:
                        max = map->count;
                        min = 0;
                        while (max >= min) {
                            s32 middle = (min + max) / 2;
                            if ((u16)code == map->table[middle * 2]) {
                                glyph = map->table[middle * 2 + 1];
                                break;
                            }
                            if ((u16)code < map->table[middle * 2]) {
                                max = middle - 1;
                            } else {
                                min = middle + 1;
                            }
                        }
                        break;
                }
                break;
            }
        }
        map = (BinaryFontMapLocal*)((u8*)map + map->size);
    }
    return glyph;
}

void JUTFont_DrawStart(u32* color) {

    extern void __GXSetIndirectMask(u32 mask);
    extern void GXSetCullMode(s32 mode);
    extern void GXSetZCompLoc(s32 beforeTex);
    extern void GXSetAlphaCompare(s32, s32, s32, s32, s32);
    extern void GXSetBlendMode(s32, s32, s32, s32);
    extern void GXSetZMode(s32, s32, s32);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32, s32);
    extern void GXSetVtxAttrFmt(s32, s32, s32, s32, s32);
    extern void GXSetChanCtrl(s32, s32, s32, s32, s32, s32, s32);
    extern void GXSetNumChans(s32);
    extern void GXSetNumTexGens(s32);
    extern void GXSetTevColor(s32, void*);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
    extern void GXLoadTexObj(void*, s32);
    extern void PSMTXScale(void*, f32, f32, f32);
    extern void GXLoadTexMtxImm(void*, s32, s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern void GXLoadPosMtxImm(void*, s32);
    extern void GXSetCurrentMtx(s32);
    extern void GXSetFog(s32, f32, f32, f32, f32, void*);
    extern void* gp;
    f32 mtx[3][4];
    u32 tevColor = *color;
    u32 fogColor = unk_80429578;
    void* cam = camGetCurPtr();

    __GXSetIndirectMask(0);
    GXSetCullMode(0);
    GXSetZCompLoc(1);
    GXSetAlphaCompare(7, 0, 0, 7, 0);
    GXSetBlendMode(1, 4, 5, 7);
    GXSetZMode(1, 7, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(11, 1);
    GXSetVtxDesc(13, 1);
    GXSetVtxDesc(14, 1);
    GXSetVtxAttrFmt(0, 9, 1, 3, 0);
    GXSetVtxAttrFmt(0, 11, 1, 5, 0);
    GXSetVtxAttrFmt(0, 13, 1, 2, 0);
    GXSetVtxAttrFmt(0, 14, 1, 0, 0);
    GXSetChanCtrl(4, 0, 1, 1, 0, 0, 2);
    GXSetNumChans(1);
    GXSetNumTexGens(1);
    GXSetTevColor(1, &tevColor);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 15, 2, 12, 15);
    GXSetTevAlphaIn(0, 7, 1, 4, 7);
    GXLoadTexObj(&fgTexObj, 0);
    if (*(s32*)((s32)gp + 0x170) == 0) {
        PSMTXScale(mtx, float_0p0019531_804203ec, float_0p00097656_804203f0, float_1_804203e4);
    } else {
        PSMTXScale(mtx, float_0p0019531_804203ec, float_0p0039062_804203f4, float_1_804203e4);
    }
    GXLoadTexMtxImm(mtx, 0x1E, 1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    GXLoadPosMtxImm((void*)((s32)cam + 0x11C), 0);
    GXSetCurrentMtx(0);
    GXSetFog(0, float_0_804203f8, float_0_804203f8,
             float_0_804203f8, float_0_804203f8, &fogColor);
}

void _JUTFont_DrawPos(u16 param_1, s16 param_2, s16 param_3) {
    extern void GXBegin(s32 primitive, s32 vtxFmt, s32 nVerts);
    extern u32 HSV2RGB(void* hsv);

    u8* widthEntry;
    u8* entry;
    u16 firstCode;
    u32 count;
    s32 i;
    s32 code;
    s32 col;
    s32 row;
    s32 page;
    s32 u0;
    s32 v0;
    s32 u1;
    s32 v1;
    s32 x1;
    s32 y0;
    s32 y1;
    s32 rndU;
    u32 widthByte;
    s32 rndURight;
    s32 rndVBottom;
    u32 color;
    u32 hsvBase;
    u32 hsv0;
    u32 hsv1;
    u32 hsv2;
    u32 hsv3;
    u8 hueOffset;
    u8 hueLeft;
    u8 hueRight;
    f32 cellW;
    f32 cellH;
    f32 glyphW;
    f32 glyphH;
    f32 texU;
    register f32 texWidth;
    volatile u16* fifo16;
    volatile u8* fifo8;

    count = *(u32*)((s32)pfh + 0xC);
    entry = (u8*)((s32)pfh + 0x20);
    for (i = 0; i < count; i++) {
        switch (*(s32*)entry) {
            case 0x57494431:
                firstCode = *(u16*)((s32)entry + 8);
                if ((u32)param_1 < firstCode) {
                    break;
                }
                if ((u32)param_1 > *(u16*)((s32)entry + 0xA)) {
                    break;
                }
                widthEntry = (u8*)((s32)entry + (((u32)param_1 - firstCode) * 2) + 0xC);
                goto width_found;
            default:
                break;
        }
        entry = (u8*)((s32)entry + *(s32*)((s32)entry + 4));
    }
    widthEntry = 0;

width_found:
    code = (u16)param_1 - *(u16*)((s32)blockGlyph + 8);
    cellH = (f32)*(u16*)((s32)blockGlyph + 0xE);
    col = code / *(u16*)((s32)blockGlyph + 0x16);
    row = col / *(u16*)((s32)blockGlyph + 0x18);
    cellW = (f32)*(u16*)((s32)blockGlyph + 0xC);
    glyphW = (cellH - float_2_804203e8) * float_1_804203e4;
    texU = (f32)(code - col * *(u16*)((s32)blockGlyph + 0x16)) * cellW + (f32)widthEntry[0];
    widthByte = widthEntry[1];
    glyphH = (f32)widthByte * float_1_804203e4;
    texWidth = (f32)widthByte;
    fifo16 = (volatile u16*)0xCC008000;
    fifo8 = (volatile u8*)0xCC008000;
    GXBegin(0x80, 0, 4);
    rndU = irand(0x40) & 0xFF;
    widthByte = irand(0x40) & 0xFF;

    page = col - row * *(u16*)((s32)blockGlyph + 0x18);
    v0 = (s32)(float_2_804203e8 + (f32)page * cellH);
    x1 = (s32)((f32)param_2 + glyphH);
    y0 = param_3 - 2;
    hueOffset = (u8)param_2 - (u8)(*(s32*)((s32)gp + 0x1C) << 1);
    hueLeft = (u8)((s32)param_2 + (s32)hueOffset);

    *fifo16 = (u16)param_2;
    *fifo16 = (u16)y0;
    *fifo16 = 0;
    hsvBase = dat_804203e0;
    ((u8*)&hsvBase)[0] = hueLeft;
    hsv0 = hsvBase;
    color = HSV2RGB(&hsv0);
    u0 = (s32)texU;
    *fifo8 = ((u8*)&color)[0];
    *fifo8 = ((u8*)&color)[1];
    *fifo8 = ((u8*)&color)[2];
    *fifo8 = ((u8*)&color)[3];
    *fifo16 = (u16)u0;
    *fifo16 = (u16)v0;
    *fifo8 = (u8)rndU;
    *fifo8 = (u8)widthByte;

    *fifo16 = (u16)x1;
    *fifo16 = (u16)y0;
    *fifo16 = 0;
    hueRight = (u8)((s32)x1 + (s32)hueOffset);
    ((u8*)&hsvBase)[0] = hueRight;
    hsv1 = hsvBase;
    color = HSV2RGB(&hsv1);
    u1 = (s32)(texU + texWidth);
    rndURight = (s32)((f32)(u8)rndU + glyphH);
    *fifo8 = ((u8*)&color)[0];
    *fifo8 = ((u8*)&color)[1];
    *fifo8 = ((u8*)&color)[2];
    *fifo8 = ((u8*)&color)[3];
    *fifo16 = (u16)u1;
    *fifo16 = (u16)v0;
    *fifo8 = (u8)rndURight;
    *fifo8 = (u8)widthByte;

    y1 = (s32)((f32)y0 - glyphW);
    *fifo16 = (u16)x1;
    *fifo16 = (u16)y1;
    *fifo16 = 0;
    ((u8*)&hsvBase)[0] = hueRight;
    hsv2 = hsvBase;
    color = HSV2RGB(&hsv2);
    v1 = (s32)((f32)(u16)v0 + (cellH - float_2_804203e8));
    rndVBottom = (s32)((f32)(u8)widthByte + glyphW);
    *fifo8 = ((u8*)&color)[0];
    *fifo8 = ((u8*)&color)[1];
    *fifo8 = ((u8*)&color)[2];
    *fifo8 = ((u8*)&color)[3];
    *fifo16 = (u16)u1;
    *fifo16 = (u16)v1;
    *fifo8 = (u8)rndURight;
    *fifo8 = (u8)rndVBottom;

    *fifo16 = (u16)param_2;
    *fifo16 = (u16)y1;
    *fifo16 = 0;
    ((u8*)&hsvBase)[0] = hueLeft;
    hsv3 = hsvBase;
    color = HSV2RGB(&hsv3);
    *fifo8 = ((u8*)&color)[0];
    *fifo8 = ((u8*)&color)[1];
    *fifo8 = ((u8*)&color)[2];
    *fifo8 = ((u8*)&color)[3];
    *fifo16 = (u16)u0;
    *fifo16 = (u16)v1;
    *fifo8 = (u8)rndU;
    *fifo8 = (u8)rndVBottom;
}

u32 fontColTbl[3] = {0x1E1414FF, 0xDCDCDCFF, 0xB47814FF};
s32 charspace[7] = {4, 2, 2, 2, 2, 2, 2};
char* fontname[7] = {
    (char*)str_papermarioset_JPN_802c2b30,
    (char*)str_papermarioset_US_802c2b44,
    (char*)str_papermarioset_EU_802c2b58,
    (char*)str_papermarioset_EU_802c2b58,
    (char*)str_papermarioset_EU_802c2b58,
    (char*)str_papermarioset_EU_802c2b58,
    (char*)str_papermarioset_US_802c2b44,
};

const char str_papermarioset_JPN_802c2b30[] = "papermarioset_JPN";
const char str_papermarioset_US_802c2b44[] = "papermarioset_US";
const char str_papermarioset_EU_802c2b58[] = "papermarioset_EU";
const char str_PCTs_f_PCTs_bfn_802c2ba0[] = "%s/f/%s.bfn";
const char str_PCTs_font_PCTs_bfn_802c2bac[] = "%s/font/%s.bfn";
const char str_PCTdPCTfPCTdPCTdPCTd_802c2bbc[] = "%d%f%d%d%d";
const char str_PCTsPCTfPCTdPCTdPCTd_802c2bc8[] = "%s%f%d%d%d";

const volatile u32 dat_804203d8 = 0xE00000FF;
const volatile u32 dat_804203dc = 0xE00000FF;
const volatile u32 dat_804203e0 = 0x00AAFFFF;
const f32 float_1_804203e4 = 1.0f;
const f32 float_2_804203e8 = 2.0f;
const f32 float_0p0019531_804203ec = 1.0f / 512.0f;
const f32 float_0p00097656_804203f0 = 1.0f / 1024.0f;
const f32 float_0p0039062_804203f4 = 1.0f / 256.0f;
const f32 float_0_804203f8 = 0.0f;
const f32 float_255_804203fc = 255.0f;
const f32 float_360_80420400 = 360.0f;
const f32 float_60_80420404 = 60.0f;
const char str_icon_80420408[] = "icon";
const char str_red_80420410[] = "red";
const char str_red_80420414[] = "/red";
const f32 float_0p5_8042041c = 0.5f;
const char str_col_80420420[] = "col";
const char str_PCTx_80420424[] = "%x";
const char str_col_80420428[] = "/col";
const f32 float_neg1_80420430 = -1.0f;
const f32 float_28_80420434 = 28.0f;
const f32 float_0p015625_80420438 = 0.015625f;
