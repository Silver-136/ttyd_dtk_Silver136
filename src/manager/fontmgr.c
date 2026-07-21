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
void _JUTFont_DrawPos(u16 glyph, s16 x, s16 y);

void fontmgrInit(void) {
    ;
}

void fontmgrTexSetup(void) {
    JUTFontSetup((u16)*(s32*)((s32)gp + 0x16C));
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

void FontDrawNoiseOff(void) {
    GXSetNumTexGens(1);
    GXSetNumTevStages(1);
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

void FontDrawColor(void* colorPtr) {
    extern void GXSetTevColor(s32 reg, void* color);
    u32 color;

    fontColor = *(u32*)colorPtr;
    ((u8*)colorPtr)[3] = (((u8*)colorPtr)[3] * fontAlpha) / 255;
    color = *(u32*)colorPtr;
    GXSetTevColor(1, &color);
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

u32 FontGetDrawColor(void) {
    return fontColor;
}

void FontDrawScale(f32 scale) {
    fontScale.x = scale;
    fontScale.y = scale;
    fontScale.z = scale;
}

void FontDrawScaleVec(Vec* scale) {
    fontScale = *scale;
}

void FontDrawCode(f64 x, f64 y, u16 code) {
    extern void PSMTXScale(void*, f32, f32, f32);
    extern void PSMTXTransApply(void*, void*, f32, f32, f32);
    extern void* camGetCurPtr(void);
    extern void PSMTXConcat(void*, void*, void*);
    extern void GXLoadPosMtxImm(void*, s32);
    extern void GXSetCurrentMtx(s32);
    extern void GXSetTevColor(s32, void*);
    extern void _JUTFont_DrawPos(u16, s16, s16);
    extern f32 float_0_804203f8;
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

void FontDrawCodeMtx(void* mtx, u16 code) {
    extern void* camGetCurPtr(void);
    extern void PSMTXConcat(void*, void*, void*);
    extern void GXLoadPosMtxImm(void*, s32);
    extern void GXSetCurrentMtx(s32);
    extern void GXSetTevColor(s32, void*);
    extern void _JUTFont_DrawPos(u16, s16, s16);
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

void FontDrawStringVecPitch(Vec* pos, char* msg, f32 pitch) {
    extern void* camGetCurPtr(void);
    extern void PSMTXScale(void*, f32, f32, f32);
    extern void PSMTXTransApply(void*, void*, f32, f32, f32);
    extern void PSMTXConcat(void*, void*, void*);
    extern void GXLoadPosMtxImm(void*, s32);
    extern s32 _ismbblead(s32);
    extern char* msgGetCommand(char*, char*, char*);
    extern void GXSetTevColor(s32, void*);
    extern void _JUTFont_DrawPos(u16, s16, s16);
    extern s32 charspace[];
    f32 scaleMtx[3][4];
    f32 drawMtx[3][4];
    char command[128];
    char argument[128];
    f32 x;
    s32 glyph;
    void* entry;
    s32 count;
    u8 width;
    u32 oldColor;
    u32 edgeColor;
    u32 gxColor;

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
            glyph = JUTFont_CodeToGlyph((u8)*msg);
            msg++;
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
                break;
            }
            entry = (u8*)entry + *(s32*)((u8*)entry + 4);
            count--;
        }
        width = *(u8*)((u8*)entry + 1);

        PSMTXTransApply(scaleMtx, drawMtx, x, pos->y, pos->z);
        PSMTXConcat((u8*)camGetCurPtr() + 0x11C, drawMtx, drawMtx);
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
        if (pitch < 0.0f) {
            x += (f32)(width + charspace[*(s32*)((u8*)gp + 0x16C)]);
        } else {
            x += pitch;
        }
    }
}

void FontDrawStringMtx(f32* mtx, void* string) {
    extern void* camGetCurPtr(void);
    extern void PSMTXConcat(void* a, void* b, void* out);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern s32 _ismbblead(s32 c);
    extern char* msgGetCommand(char* in, char* cmd, char* arg);
    extern void GXSetTevColor(s32 reg, void* color);
    extern const f32 float_0_804203f8;
    extern const f32 float_28_80420434;
    extern s32 charspace[];
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
    s32 ix;
    s32 iy;

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
            glyph = JUTFont_CodeToGlyph((u8)*p);
            p++;
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

            ix = (s32)x;
            iy = (s32)y;
            _JUTFont_DrawPos((u16)glyph, (s16)(ix - 2), (s16)iy);
            _JUTFont_DrawPos((u16)glyph, (s16)(ix + 2), (s16)iy);
            _JUTFont_DrawPos((u16)glyph, (s16)ix, (s16)(iy - 2));
            _JUTFont_DrawPos((u16)glyph, (s16)ix, (s16)(iy + 2));

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

void FontDrawStringCenterMtx(void* mtx, char* msg) {
    extern void* camGetCurPtr(void);
    extern void PSMTXConcat(void*, void*, void*);
    extern void GXLoadPosMtxImm(void*, s32);
    extern s32 _ismbblead(s32);
    extern void GXSetTevColor(s32, void*);
    extern void _JUTFont_DrawPos(u16, s16, s16);
    extern s32 charspace[];
    extern f32 float_0p5_8042041c;
    f32 drawMtx[3][4];
    f32 x;
    s32 glyph;
    void* entry;
    s32 count;
    u32 oldColor;
    u32 edgeColor;
    u32 gxColor;
    s32 ix;

    x = -float_0p5_8042041c * FontGetMessageWidthLine(msg, 0);
    PSMTXConcat((u8*)camGetCurPtr() + 0x11C, mtx, drawMtx);
    GXLoadPosMtxImm(drawMtx, 0);

    while (*msg != 0) {
        if (_ismbblead(*msg) != 0) {
            glyph = JUTFont_CodeToGlyph(*(u16*)msg);
            msg += 2;
        } else {
            glyph = JUTFont_CodeToGlyph((u8)*msg);
            msg++;
        }

        ix = (s32)x;
        if (fontEdge != 0) {
            edgeColor = fontColTbl[0];
            oldColor = fontColor;
            fontColor = edgeColor;
            ((u8*)&edgeColor)[3] = (((u8*)&edgeColor)[3] * fontAlpha) / 255;
            gxColor = edgeColor;
            GXSetTevColor(1, &gxColor);
            _JUTFont_DrawPos((u16)glyph, (s16)(ix - 2), 0);
            _JUTFont_DrawPos((u16)glyph, (s16)(ix + 2), 0);
            _JUTFont_DrawPos((u16)glyph, (s16)ix, -2);
            _JUTFont_DrawPos((u16)glyph, (s16)ix, 2);
            fontColor = oldColor;
            ((u8*)&oldColor)[3] = (((u8*)&oldColor)[3] * fontAlpha) / 255;
            gxColor = oldColor;
            GXSetTevColor(1, &gxColor);
        }
        _JUTFont_DrawPos((u16)glyph, (s16)ix, 0);

        entry = (u8*)pfh + 0x20;
        count = *(s32*)((u8*)pfh + 0xC);
        while (count > 0) {
            if (*(u32*)entry == 0x57494431 &&
                (u16)glyph >= *(u16*)((u8*)entry + 8) &&
                (u16)glyph <= *(u16*)((u8*)entry + 0xA)) {
                entry = (u8*)entry + (((u16)glyph - *(u16*)((u8*)entry + 8)) * 2) + 0xC;
                break;
            }
            entry = (u8*)entry + *(s32*)((u8*)entry + 4);
            count--;
        }
        x += (f32)(*(u8*)((u8*)entry + 1) + charspace[*(s32*)((u8*)gp + 0x16C)]);
    }
}

void FontDrawStringShake(double xIn, double yIn, char* str) {
    extern void* camGetCurPtr(void);
    extern void PSMTXScale(f32 mtx[3][4], f32 x, f32 y, f32 z);
    extern void PSMTXTransApply(f32 src[3][4], f32 dst[3][4], f32 x, f32 y, f32 z);
    extern void PSMTXConcat(void* a, void* b, void* out);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern s32 _ismbblead(s32 c);
    extern s32 irand(s32 max);
    extern void GXSetTevColor(s32 reg, void* color);
    extern const f32 float_0_804203f8;
    extern s32 charspace[];
    f32 scaleMtx[3][4];
    f32 drawMtx[3][4];
    char* p;
    f32 x;
    f32 y;
    s32 glyph;
    s32 count;
    void* entry;
    u32 edgeColor;
    u32 oldColor;
    u32 gxColor;
    s32 jitterX;
    s32 jitterY;

    p = str;
    x = (f32)xIn;
    y = (f32)yIn;
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
        jitterY = irand(10000) % 3;

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

        PSMTXTransApply(scaleMtx, drawMtx, x + (f32)jitterX, y + (f32)jitterY, float_0_804203f8);
        PSMTXConcat((void*)((s32)camGetCurPtr() + 0x11C), drawMtx, drawMtx);
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
        x += (f32)(*(u8*)((s32)entry + 1) + charspace[*(s32*)((s32)gp + 0x16C)]);
    }
}


void FontDrawMessage(s32 x0, s32 y0, char* msg) {
    extern void* camGetCurPtr(void);
    extern u32 JUTFont_CodeToGlyph(u32 code);
    extern void _JUTFont_DrawPos(u16 glyph, s16 x, s16 y);
    extern s32 _ismbblead(s32 c);
    extern char* msgGetCommand(char* in, char* cmd, char* arg);
    extern s32 strcmp(const char* a, const char* b);
    extern s32 sscanf(const char* str, const char* fmt, ...);
    extern s32 msgIconStr2ID(char* name);
    extern void iconGetWidthHight(u16* w, u16* h, s32 iconId);
    extern void iconDispGxAlpha(f32 scale, void* pos, s32 flags, s32 iconId, s32 alpha);
    extern void GXSetTevColor(s32 reg, void* color);
    extern void JUTFont_DrawStart(void* color);
    extern s32 charspace[];
    extern u32 dat_804203d8;
    extern const f32 float_0_804203f8;
    extern const f32 float_0p5_8042041c;
    extern const char str_red_80420410[4];
    extern const char str_red_80420414[5];
    extern const char str_col_80420420[4];
    extern const char str_col_80420428[5];
    extern const char str_PCTx_80420424[3];
    extern const char str_icon_80420408[5];
    extern const char str_papermarioset_JPN_802c2b30[];

    u16 iconH;
    u16 iconW;
    u32 glyph;
    u32 colorA;
    u32 colorB;
    u32 colorC;
    u32 colorD;
    u32 colorE;
    u32 colorF;
    u32 colorG;
    u32 gxColorA;
    u32 gxColorB;
    u32 gxColorC;
    u32 gxColorD;
    u32 gxColorE;
    u32 gxColorF;
    u32 gxColorG;
    u32 savedColor;
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
                    gxColorA = colorA;
                    ((u8*)&gxColorA)[3] = (u8)((((u8*)&gxColorA)[3] * fontAlpha) / 255);
                    GXSetTevColor(1, &gxColorA);
                } else if (strcmp(cmd, str_red_80420414) == 0) {
                    colorB = fontColTbl[0];
                    fontColor = colorB;
                    gxColorB = colorB;
                    ((u8*)&gxColorB)[3] = (u8)((((u8*)&gxColorB)[3] * fontAlpha) / 255);
                    GXSetTevColor(1, &gxColorB);
                } else if (strcmp(cmd, str_col_80420420) == 0) {
                    sscanf(arg, str_PCTx_80420424, &colorC);
                    fontColor = colorC;
                    gxColorC = colorC;
                    ((u8*)&gxColorC)[3] = (u8)((((u8*)&gxColorC)[3] * fontAlpha) / 255);
                    GXSetTevColor(1, &gxColorC);
                } else if (strcmp(cmd, str_col_80420428) == 0) {
                    colorD = fontColTbl[0];
                    fontColor = colorD;
                    gxColorD = colorD;
                    ((u8*)&gxColorD)[3] = (u8)((((u8*)&gxColorD)[3] * fontAlpha) / 255);
                    GXSetTevColor(1, &gxColorD);
                } else if (strcmp(cmd, str_icon_80420408) == 0) {
                    camGetCurPtr();

                    iconScale = 1.0f;
                    iconDx = 0;
                    iconDy = 0;
                    iconAdd = 0;
                    iconId = 0;

                    if (arg[0] >= '0' && arg[0] <= '9') {
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
                    fontScale.x = *(f32*)(paper + 0x3C);
                    fontScale.y = *(f32*)(paper + 0x40);
                    fontScale.z = *(f32*)(paper + 0x44);

                    colorE = fontColTbl[0];
                    gxColorE = colorE;
                    ((u8*)&gxColorE)[3] = (u8)((((u8*)&gxColorE)[3] * fontAlpha) / 255);
                    JUTFont_DrawStart(&gxColorE);

                    colorF = fontColor;
                    gxColorF = colorF;
                    ((u8*)&gxColorF)[3] = (u8)((((u8*)&gxColorF)[3] * fontAlpha) / 255);
                    GXSetTevColor(1, &gxColorF);

                    x = (s32)((f32)x + ((f32)iconW * iconScale) + (f32)iconAdd);
                } else {
                    if (cmd[0] == 'R') {
                        ;
                    } else if (cmd[0] == '/' && cmd[1] == 'r') {
                        ;
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

        if ((u16)glyph != 0xFFFF) {
            void* entry;
            s32 count;

            if (fontEdge != 0) {
                colorG = fontColTbl[0];
                savedColor = fontColor;
                fontColor = colorG;
                gxColorG = colorG;
                ((u8*)&gxColorG)[3] = (u8)((((u8*)&gxColorG)[3] * fontAlpha) / 255);
                GXSetTevColor(1, &gxColorG);

                _JUTFont_DrawPos((u16)glyph, (s16)(x - 2), (s16)y);
                _JUTFont_DrawPos((u16)glyph, (s16)(x + 2), (s16)y);
                _JUTFont_DrawPos((u16)glyph, (s16)x, (s16)(y - 2));
                _JUTFont_DrawPos((u16)glyph, (s16)x, (s16)(y + 2));

                fontColor = savedColor;
                gxColorG = savedColor;
                ((u8*)&gxColorG)[3] = (u8)((((u8*)&gxColorG)[3] * fontAlpha) / 255);
                GXSetTevColor(1, &gxColorG);
            }

            _JUTFont_DrawPos((u16)glyph, (s16)x, (s16)y);

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

void FontDrawMessageMtx(f32 mtx[3][4], char* msg) {
    extern void* camGetCurPtr(void);
    extern u32 JUTFont_CodeToGlyph(u32 code);
    extern void _JUTFont_DrawPos(u16 glyph, s16 x, s16 y);
    extern s32 _ismbblead(s32 c);
    extern char* msgGetCommand(char* in, char* cmd, char* arg);
    extern s32 strcmp(const char* a, const char* b);
    extern s32 sscanf(const char* str, const char* fmt, ...);
    extern s32 msgIconStr2ID(char* name);
    extern void iconGetWidthHight(u16* w, u16* h, s32 iconId);
    extern void iconDispGxAlpha(f32 scale, void* pos, s32 flags, s32 iconId, s32 alpha);
    extern void PSMTXMultVec(void* mtx, Vec* src, Vec* dst);
    extern void PSMTXTrans(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXConcat(void* a, void* b, void* out);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXSetTevColor(s32 reg, void* color);
    extern void JUTFont_DrawStart(void* color);
    extern s32 charspace[];
    extern u32 dat_804203dc;
    extern const f32 float_0_804203f8;
    extern const f32 float_0p5_8042041c;
    extern const char str_red_80420410[];
    extern const char str_red_80420414[];
    extern const char str_icon_80420408[];
    extern const char str_papermarioset_JPN_802c2b30[];

    u16 iconH;
    u16 iconW;
    u32 glyph;
    u32 colorA;
    u32 colorB;
    u32 colorC;
    u32 colorD;
    u32 colorE;
    u32 gxColorA;
    u32 gxColorB;
    u32 gxColorC;
    u32 gxColorD;
    u32 gxColorE;
    u32 savedColor;
    s32 iconId;
    s32 iconDx;
    s32 iconDy;
    s32 iconAdd;
    f32 iconScale;
    Vec iconPos;
    char iconName[80];
    f32 transMtx[3][4];
    f32 workMtx[3][4];
    char cmd[128];
    char arg[128];
    void* cam;
    char* p;
    s32 x;
    s32 y;

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
                    gxColorA = colorA;
                    ((u8*)&gxColorA)[3] = (u8)((((u8*)&gxColorA)[3] * fontAlpha) / 255);
                    GXSetTevColor(1, &gxColorA);
                } else if (strcmp(cmd, str_red_80420414) == 0) {
                    colorB = fontColTbl[0];
                    fontColor = colorB;
                    gxColorB = colorB;
                    ((u8*)&gxColorB)[3] = (u8)((((u8*)&gxColorB)[3] * fontAlpha) / 255);
                    GXSetTevColor(1, &gxColorB);
                } else if (strcmp(cmd, str_icon_80420408) == 0) {
                    camGetCurPtr();

                    iconScale = 1.0f;
                    iconDx = 0;
                    iconDy = 0;
                    iconAdd = 0;
                    iconId = 0;

                    if (arg[0] >= '0' && arg[0] <= '9') {
                        sscanf(arg, str_papermarioset_JPN_802c2b30 + 0x8C, &iconId, &iconScale, &iconDx, &iconDy, &iconAdd);
                    } else {
                        sscanf(arg, str_papermarioset_JPN_802c2b30 + 0x98, iconName + 0x30, &iconScale, &iconDx, &iconDy, &iconAdd);
                        iconId = msgIconStr2ID(iconName + 0x30);
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
                    fontScale.x = *(f32*)(str_papermarioset_JPN_802c2b30 + 0x3C);
                    fontScale.y = *(f32*)(str_papermarioset_JPN_802c2b30 + 0x40);
                    fontScale.z = *(f32*)(str_papermarioset_JPN_802c2b30 + 0x44);

                    colorC = fontColTbl[0];
                    gxColorC = colorC;
                    ((u8*)&gxColorC)[3] = (u8)((((u8*)&gxColorC)[3] * fontAlpha) / 255);
                    JUTFont_DrawStart(&gxColorC);

                    colorD = fontColor;
                    gxColorD = colorD;
                    ((u8*)&gxColorD)[3] = (u8)((((u8*)&gxColorD)[3] * fontAlpha) / 255);
                    GXSetTevColor(1, &gxColorD);

                    x = (s32)((f32)x + ((f32)iconW * iconScale) + (f32)iconAdd);
                } else {
                    if (cmd[0] == 'R') {
                        ;
                    } else if (cmd[0] == '/' && cmd[1] == 'r') {
                        ;
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
                gxColorE = colorE;
                ((u8*)&gxColorE)[3] = (u8)((((u8*)&gxColorE)[3] * fontAlpha) / 255);
                GXSetTevColor(1, &gxColorE);

                _JUTFont_DrawPos((u16)glyph, -2, 0);
                _JUTFont_DrawPos((u16)glyph, 2, 0);
                _JUTFont_DrawPos((u16)glyph, 0, -2);
                _JUTFont_DrawPos((u16)glyph, 0, 2);

                fontColor = savedColor;
                gxColorE = savedColor;
                ((u8*)&gxColorE)[3] = (u8)((((u8*)&gxColorE)[3] * fontAlpha) / 255);
                GXSetTevColor(1, &gxColorE);
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

f32 FontGetMessageWidthLine(const char* msg, s32 line) {
    extern s32 _ismbblead(s32 c);
    extern char* msgGetCommand(char* in, char* cmd, char* arg);
    extern s32 strcmp(const char* a, const char* b);
    extern s32 sscanf(const char* str, const char* fmt, ...);
    extern void* camGetCurPtr(void);
    extern s32 msgIconStr2ID(char* name);
    extern void iconGetWidthHight(s16* w, s16* h, u16 id);
    extern s32 charspace[];
    const char* p;
    s32 glyph;
    u32 width;
    u32 maxWidth;
    s16 lines;
    char cmd[128];
    char arg[128];
    char iconName[32];
    f32 scale;
    s32 a;
    s32 b;
    s32 add;
    s32 iconId;
    s16 iconW;
    s16 iconH;
    void* entry;
    s32 count;

    p = msg;
    width = 0;
    maxWidth = 0;
    lines = 0;

    while (*p != 0) {
        glyph = 0xFFFF;
        if (_ismbblead(*p) != 0) {
            glyph = JUTFont_CodeToGlyph(*(u16*)p);
            p += 2;
        } else if (*p == '<') {
            p = msgGetCommand((char*)p + 1, cmd, arg);
            if (strcmp(cmd, "icon") == 0) {
                camGetCurPtr();
                scale = 1.0f;
                a = 0;
                b = 0;
                add = 0;
                if (arg[0] < '0' || arg[0] > '9') {
                    sscanf(arg, "%s%f%d%d%d", iconName, &scale, &a, &b, &add);
                    iconId = msgIconStr2ID(iconName);
                    if (iconId < 0) {
                        iconId = 0;
                    }
                } else {
                    sscanf(arg, "%d%f%d%d%d", &iconId, &scale, &a, &b, &add);
                }
                iconGetWidthHight(&iconW, &iconH, (u16)iconId);
                width = (u16)((f32)(u16)width + ((f32)(u16)iconW * scale) + (f32)add);
            } else if (cmd[0] == 'R') {
                glyph = 0xFFFF;
            }
            if (*p == '\n') {
                p++;
            }
        } else if (*p == '\n') {
            if ((u16)maxWidth < (u16)width) {
                maxWidth = width;
            }
            width = 0;
            lines++;
            p++;
        } else {
            glyph = JUTFont_CodeToGlyph((u8)*p);
            p++;
        }

        if ((u16)glyph == 0xFFFF) {
            continue;
        }

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
        width = (u16)(width + *(u8*)((s32)entry + 1) + charspace[*(s32*)((s32)gp + 0x16C)]);
    }

    if ((u16)maxWidth < (u16)width) {
        maxWidth = width;
    }
    if ((u16)maxWidth != 0) {
        maxWidth = (u16)(maxWidth - charspace[*(s32*)((s32)gp + 0x16C)]);
    }
    if (line != 0) {
        *(s16*)line = lines;
    }
    return (f32)(u16)maxWidth;
}

f32 FontGetMessageWidth(const char* msg) {
    return FontGetMessageWidthLine(msg, 0);
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
    extern void* pfi;
    extern void* blockGlyph;
    extern u8 fgTexObj[];
    extern char* fontname[];
    extern const char str_PCTs_f_PCTs_bfn_802c2ba0[];
    extern const char str_PCTs_font_PCTs_bfn_802c2bac[];
    extern f32 float_0_804203f8;
    char path[140];
    void* dvd;
    void* check;
    void* entry;
    void* widthEntry;
    s32 length;
    s32 maxLength;
    s32 checkLength;
    s32 i;
    u16 glyph;

    if (pfh != 0) {
        return;
    }

    sprintf(path, str_PCTs_f_PCTs_bfn_802c2ba0,
            getMarioStDvdRoot(), fontname[language]);
    dvd = DVDMgrOpen(path, 2, 0);
    length = DVDMgrGetLength(dvd);
    maxLength = length;
    for (i = 0; i < 7; i++) {
        sprintf(path, str_PCTs_font_PCTs_bfn_802c2bac,
                getMarioStDvdRoot(), fontname[i]);
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
        if (*(u32*)entry == 0x494E4631) {
            pfi = entry;
        } else if (*(u32*)entry == 0x474C5931) {
            blockGlyph = entry;
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
        if (*(u32*)entry == 0x57494431 &&
            glyph >= *(u16*)((u8*)entry + 8) &&
            glyph <= *(u16*)((u8*)entry + 0xA)) {
            widthEntry = (u8*)entry + (glyph - *(u16*)((u8*)entry + 8)) * 2 + 0xC;
            break;
        }
        entry = (u8*)entry + *(s32*)((u8*)entry + 4);
    }
    *(u8*)((u8*)widthEntry + 1) = 7;

    glyph = (u16)JUTFont_CodeToGlyph(0x8140);
    entry = (u8*)pfh + 0x20;
    widthEntry = 0;
    for (i = *(s32*)((u8*)pfh + 0xC); i != 0; i--) {
        if (*(u32*)entry == 0x57494431 &&
            glyph >= *(u16*)((u8*)entry + 8) &&
            glyph <= *(u16*)((u8*)entry + 0xA)) {
            widthEntry = (u8*)entry + (glyph - *(u16*)((u8*)entry + 8)) * 2 + 0xC;
            break;
        }
        entry = (u8*)entry + *(s32*)((u8*)entry + 4);
    }
    *(u8*)((u8*)widthEntry + 1) = 9;
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

void JUTFont_DrawStart(u32* color) {
    extern void* camGetCurPtr(void);
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
    extern void* fgTexObj;
    extern u32 unk_80429578;
    extern void* gp;
    extern f32 float_0p0019531_804203ec;
    extern f32 float_0p00097656_804203f0;
    extern f32 float_0p0039062_804203f4;
    extern f32 float_1_804203e4;
    extern f32 float_0_804203f8;
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
    extern s32 irand(s32 max);
    extern u32 HSV2RGB(void* hsv);
    extern void* blockGlyph;
    extern u32 dat_804203e0;
    extern const f32 float_1_804203e4;
    extern const f32 float_2_804203e8;
    u32 magic;
    u8* widthEntry;
    u8* entry;
    u16 firstCode;
    s32 count;
    s32 code;
    s32 col;
    s32 row;
    s32 page;
    f32 u0;
    f32 v0;
    f32 u1;
    f32 v1;
    s32 x0;
    s32 y0;
    f32 x1;
    f32 y1;
    s32 rndU;
    s32 rndV;
    u32 color;
    u32 hsv;
    f32 cellW;
    f32 cellH;
    f32 glyphW;
    f32 glyphH;
    volatile u16* fifo16;
    volatile u8* fifo8;

    entry = (u8*)((s32)pfh + 0x20);
    magic = 0x57494431;
    count = *(s32*)((s32)pfh + 0xC);
    widthEntry = 0;
    code = param_1;
    for (;;) {
        if (count == 0) {
            return;
        }
        if (*(u32*)entry == magic) {
            firstCode = *(u16*)((s32)entry + 8);
            if ((u32)code >= firstCode && (u32)code <= *(u16*)((s32)entry + 0xA)) {
                widthEntry = (u8*)((s32)entry + ((code - firstCode) * 2) + 0xC);
                break;
            }
        }
        entry = (u8*)((s32)entry + *(s32*)((s32)entry + 4));
        count--;
    }

    code -= *(u16*)((s32)blockGlyph + 8);
    col = code / *(u16*)((s32)blockGlyph + 0x16);
    row = col / *(u16*)((s32)blockGlyph + 0x18);
    page = col - row * *(u16*)((s32)blockGlyph + 0x18);
    cellW = (f32)*(u16*)((s32)blockGlyph + 0xC);
    cellH = (f32)*(u16*)((s32)blockGlyph + 0xE);
    glyphW = ((f32)*(u16*)((s32)blockGlyph + 0xE) - float_2_804203e8) * float_1_804203e4;
    glyphH = (f32)widthEntry[1] * float_1_804203e4;
    u0 = (f32)(code - col * *(u16*)((s32)blockGlyph + 0x16)) * cellW + (f32)widthEntry[0];
    v0 = float_2_804203e8 + (f32)page * cellH;
    u1 = u0 + (f32)widthEntry[1];
    v1 = v0 + (cellH - float_2_804203e8);
    x0 = param_2;
    y0 = param_3 - 2;
    x1 = (f32)param_2 + glyphH;
    y1 = (f32)(param_3 - 2) - glyphW;
    fifo16 = (volatile u16*)0xCC008000;
    fifo8 = (volatile u8*)0xCC008000;

    GXBegin(0x80, 0, 4);
    rndU = irand(0x40) & 0xFF;
    rndV = irand(0x40) & 0xFF;

    *fifo16 = (u16)x0;
    *fifo16 = (u16)y0;
    *fifo16 = 0;
    hsv = dat_804203e0;
    ((u8*)&hsv)[0] = (u8)(param_2 + ((s32)(u8)param_2 - ((*(s32*)((s32)gp + 0x1C) & 0xFF) << 1)));
    color = HSV2RGB(&hsv);
    *fifo8 = ((u8*)&color)[0];
    *fifo8 = ((u8*)&color)[1];
    *fifo8 = ((u8*)&color)[2];
    *fifo8 = ((u8*)&color)[3];
    *fifo16 = (u16)u0;
    *fifo16 = (u16)v0;
    *fifo8 = (u8)rndU;
    *fifo8 = (u8)rndV;

    *fifo16 = (u16)x1;
    *fifo16 = (u16)y0;
    *fifo16 = 0;
    hsv = dat_804203e0;
    ((u8*)&hsv)[0] = (u8)((s32)x1 + ((s32)(u8)param_2 - ((*(s32*)((s32)gp + 0x1C) & 0xFF) << 1)));
    color = HSV2RGB(&hsv);
    *fifo8 = ((u8*)&color)[0];
    *fifo8 = ((u8*)&color)[1];
    *fifo8 = ((u8*)&color)[2];
    *fifo8 = ((u8*)&color)[3];
    *fifo16 = (u16)u1;
    *fifo16 = (u16)v0;
    *fifo8 = (u8)(rndU + glyphH);
    *fifo8 = (u8)rndV;

    *fifo16 = (u16)x1;
    *fifo16 = (u16)y1;
    *fifo16 = 0;
    hsv = dat_804203e0;
    ((u8*)&hsv)[0] = (u8)((s32)x1 + ((s32)(u8)param_2 - ((*(s32*)((s32)gp + 0x1C) & 0xFF) << 1)));
    color = HSV2RGB(&hsv);
    *fifo8 = ((u8*)&color)[0];
    *fifo8 = ((u8*)&color)[1];
    *fifo8 = ((u8*)&color)[2];
    *fifo8 = ((u8*)&color)[3];
    *fifo16 = (u16)u1;
    *fifo16 = (u16)v1;
    *fifo8 = (u8)(rndU + glyphH);
    *fifo8 = (u8)(rndV + glyphW);

    *fifo16 = (u16)x0;
    *fifo16 = (u16)y1;
    *fifo16 = 0;
    hsv = dat_804203e0;
    ((u8*)&hsv)[0] = (u8)(param_2 + ((s32)(u8)param_2 - ((*(s32*)((s32)gp + 0x1C) & 0xFF) << 1)));
    color = HSV2RGB(&hsv);
    *fifo8 = ((u8*)&color)[0];
    *fifo8 = ((u8*)&color)[1];
    *fifo8 = ((u8*)&color)[2];
    *fifo8 = ((u8*)&color)[3];
    *fifo16 = (u16)u0;
    *fifo16 = (u16)v1;
    *fifo8 = (u8)rndU;
    *fifo8 = (u8)(rndV + glyphW);
    return;
}
