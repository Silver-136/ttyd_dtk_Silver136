#include "manager/fontmgr.h"
#include "manager/dvdmgr.h"
#include "driver/camdrv.h"
#include "driver/icondrv.h"
#include "driver/msgdrv.h"
#include "mario/mariost.h"
#include "memory.h"
#include "system.h"
#include "driver/windowdrv.h"
#include "libc_decls.h"

typedef struct FontWidthEntry {
    u8 kerning; //0x0
    u8 width; //0x1
} FontWidthEntry;

typedef struct FontWidthBlock {
    u32 magic; //0x0, "WID1"
    u32 size; //0x4
    u16 firstcode; //0x8
    u16 lastcode; //0xA
    FontWidthEntry entries[]; //0xC
} FontWidthBlock;

//.sdata
GXColor fontColTbl[3] = {
    {0x1E, 0x14, 0x14, 0xFF},
    {0xDC, 0xDC, 0xDC, 0xFF},
    {0xB4, 0x78, 0x14, 0xFF}
};
#ifdef VERSION_G8ME01
s32 charspace[7] = {4, 2, 2, 2, 2, 2, 2};
const char* fontname[7] = {"papermarioset_JPN", "papermarioset_US", "papermarioset_EU", "papermarioset_EU",
    "papermarioset_EU", "papermarioset_EU", "papermarioset_US"};
#else
s32 charspace[2] = {4, 2};
const char* fontname[2] = {"papermarioset_JPN", "popjoyB22"};
#endif

//.sbss
Vec fontScale;
GXColor fontColor;
u8 fontAlpha;
BOOL fontEdge;
BinaryFontInfo* pfi; //pointer to font info
BinaryFontGlyph* blockGlyph;
GXTexObj fgTexObj;
BinaryFontHeader* pfh; //pointer to font header

// US passes single-byte characters to JUTFont_CodeToGlyph zero-extended
#ifdef VERSION_G8ME01
#define FONT_CH(c) ((u8)(c))
#else
#define FONT_CH(c) (c)
#endif

//local prototypes
void JUTFont_DrawStart(GXColor color);
void _JUTFont_DrawPos(u16 code, s16 x, s16 y);
u16 JUTFont_CodeToGlyph(u32 code);
GXColor HSV2RGB(GXColor hsv);
#ifdef VERSION_G8ME01
void FontDrawStringVecPitch(Vec pos, f32 pitch, const s8* msg);
#endif

static inline FontWidthEntry* fontGetWidth(u16 code) {
    FontWidthBlock* block = (FontWidthBlock*)&pfh[1];
    u32 i;

    for (i = 0; i < pfh->blockcount; i++) {
        switch (block->magic) {
            case 'WID1':
                if (code >= block->firstcode && code <= block->lastcode) {
                    return &block->entries[code - block->firstcode];
                }
                break;
        }
        block = (FontWidthBlock*)((u8*)block + block->size);
    }
    return NULL;
}

static inline void fontDrawCodeEdge(u16 code) {
    if (fontEdge) {
        GXColor save = fontColor;
        FontDrawColorIDX(0);
        _JUTFont_DrawPos(code, -2, 0);
        _JUTFont_DrawPos(code, 2, 0);
        _JUTFont_DrawPos(code, 0, -2);
        _JUTFont_DrawPos(code, 0, 2);
        FontDrawColor(save);
    }
    _JUTFont_DrawPos(code, 0, 0);
}

void fontmgrInit(void) {
    ; //stubbed in retail
}

void fontmgrTexSetup(void) {
#ifdef VERSION_G8ME01
    JUTFontSetup(gp->language);
#else
    JUTFontSetup(0);
#endif
}

void FontDrawStart(void) {
    FontDrawStart_alpha(0xFF);
}

void FontDrawStart_alpha(u8 alpha) {
    GXColor start;

    fontColor = start = fontColTbl[0];
    fontAlpha = alpha;
    start.a = (start.a * alpha) / 255;
    fontScale = (Vec){1.0f, 1.0f, 1.0f};
    fontEdge = FALSE;
    JUTFont_DrawStart(start);
}

void FontDrawEdge(void) {
    fontEdge = TRUE;
}

void FontDrawEdgeOff(void) {
    fontEdge = FALSE;
}

void FontDrawRainbowColor(void) {
    GXSetNumTevStages(2);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_CPREV, GX_CC_RASC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_APREV, GX_CA_RASA, GX_CA_ZERO);
}

void FontDrawRainbowColorOff(void) {
    GXSetNumTevStages(1);
}

void FontDrawNoise(void) {
    Mtx mtx;

    GXSetNumTexGens(2);
    GXSetNumTevStages(2);
    GXSetTevKAlphaSel(GX_TEVSTAGE1, GX_TEV_KASEL_1);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP2, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_CPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_APREV, GX_CA_TEXA, GX_CA_ZERO);
    GXLoadTexObj(&wakuTexObj[50], GX_TEXMAP2);
    PSMTXScale(mtx, 0.015625f, 0.015625f, 1.0f);
    GXLoadTexMtxImm(mtx, GX_TEXMTX1, GX_MTX2x4);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX1, GX_TEXMTX1, GX_FALSE, GX_PTIDENTITY);
}

void FontDrawNoiseOff(void) {
    GXSetNumTexGens(1);
    GXSetNumTevStages(1);
}

void FontDrawColorIDX(s32 idx) {
    FontDrawColor(fontColTbl[idx]);
}

void FontDrawColor(GXColor color) {
    fontColor = color;
    color.a = (color.a * fontAlpha) / 255;
    GXSetTevColor(GX_TEVREG0, color);
}

void FontDrawColor_(void) {
    FontDrawColor(fontColor);
}

GXColor FontGetDrawColor(void) {
    return fontColor;
}

void FontDrawScale(f32 scale) {
    fontScale.x = scale;
    fontScale.y = scale;
    fontScale.z = scale;
}

void FontDrawScaleVec(Vec scale) {
    fontScale = scale;
}

void FontDrawCode(f32 x, f32 y, u16 code) {
    Mtx mtx;

    PSMTXScale(mtx, fontScale.x, fontScale.y, fontScale.z);
    PSMTXTransApply(mtx, mtx, x, y, 0.0f);
    PSMTXConcat(camGetCurPtr()->view, mtx, mtx);
    GXLoadPosMtxImm(mtx, 0);
    GXSetCurrentMtx(0);
    fontDrawCodeEdge(code);
}

void FontDrawCodeMtx(Mtx mtx, u16 code) {
    PSMTXConcat(camGetCurPtr()->view, mtx, mtx);
    GXLoadPosMtxImm(mtx, 0);
    GXSetCurrentMtx(0);
    fontDrawCodeEdge(code);
}

static void FontDrawStringVec(Vec pos, const s8* str) {
    CameraEntry* camera;
    Mtx trans, scale;
    u8 width;
    const s8* msg;
    u16 code;
    f32 x;

    msg = str;
    camera = camGetCurPtr();
    x = pos.x;
    PSMTXScale(scale, fontScale.x, fontScale.y, fontScale.z);
    while (*msg) {
        if (_ismbblead(*msg)) {
            code = JUTFont_CodeToGlyph(*(u16*)msg);
            msg += 2;
        } else {
            code = JUTFont_CodeToGlyph(FONT_CH(*msg++));
        }
        width = fontGetWidth(code)->width;
        PSMTXTransApply(scale, trans, x, pos.y, pos.z);
        PSMTXConcat(camera->view, trans, trans);
        GXLoadPosMtxImm(trans, 0);
        fontDrawCodeEdge(code);
        x += fontScale.x * (width + charspace[gp->language]);
    }
}

#ifdef VERSION_G8ME01
// US: FontDrawString and FontDrawStringPitch are wrappers over FontDrawStringVecPitch
void FontDrawString(f32 x, f32 y, const char* msg) {
    FontDrawStringVecPitch((Vec){x, y, 0.0f}, -1.0f, (const s8*)msg);
}
#else
void FontDrawString(f32 x, f32 y, const char* msg) {
    FontDrawStringVec((Vec){x, y, 0.0f}, (const s8*)msg);
}
#endif

static inline void fontDrawStringVecPitch(Vec pos, f32 pitch, const s8* str) {
    CameraEntry* camera;
    Mtx trans, scale;
    u8 width;
    const s8* msg;
    u16 code;
    f32 x;

    msg = str;
    camera = camGetCurPtr();
    x = pos.x;
    PSMTXScale(scale, fontScale.x, fontScale.y, fontScale.z);
    while (*msg) {
        if (_ismbblead(*msg)) {
            code = JUTFont_CodeToGlyph(*(u16*)msg);
            msg += 2;
        } else {
            code = JUTFont_CodeToGlyph(FONT_CH(*msg++));
        }
        width = fontGetWidth(code)->width;
        PSMTXTransApply(scale, trans, pitch < 0.0f ? x : x + fontScale.x * ((pitch - width) * 0.5f), pos.y, pos.z);
        PSMTXConcat(camera->view, trans, trans);
        GXLoadPosMtxImm(trans, 0);
        fontDrawCodeEdge(code);
        if (pitch < 0.0f) {
            x += fontScale.x * (width + charspace[gp->language]);
        } else {
            x += pitch * fontScale.x;
        }
    }
}

void FontDrawStringPitch(f32 x, f32 y, f32 pitch, const char* msg) {
#ifdef VERSION_G8ME01
    FontDrawStringVecPitch((Vec){x, y, 0.0f}, pitch, (const s8*)msg);
#else
    fontDrawStringVecPitch((Vec){x, y, 0.0f}, pitch, (const s8*)msg);
#endif
}

#ifdef VERSION_G8ME01
// FAKE: auto_inline off keeps the wrappers from inlining FontDrawStringVecPitch (the US wrappers call it)
#pragma auto_inline off
#endif
void FontDrawStringVecPitch(Vec pos, f32 pitch, const s8* msg) {
    CameraEntry* camera;
    Mtx trans, scale;
#ifdef VERSION_G8ME01
    char cmd[128];
    char arg[128];
#endif
    u8 width;
    u16 code;
    f32 x;
#ifdef VERSION_G8ME01
    s8 c;
#endif

    camera = camGetCurPtr();
    x = pos.x;
    PSMTXScale(scale, fontScale.x, fontScale.y, fontScale.z);
    while (*msg) {
#ifdef VERSION_G8ME01
        code = 0xFFFF;
#endif
        if (_ismbblead(*msg)) {
            code = JUTFont_CodeToGlyph(*(u16*)msg);
            msg += 2;
        } else {
#ifdef VERSION_G8ME01
            switch (c = *msg) {
                case '<':
                    msg = (const s8*)msgGetCommand((char*)(msg + 1), cmd, arg);
                    break;
                default:
                    msg++;
                    code = JUTFont_CodeToGlyph((u8)c);
                    break;
            }
        }
        if (code != 0xFFFF) {
            width = fontGetWidth(code)->width;
            PSMTXTransApply(scale, trans, pitch < 0.0f ? x : x + fontScale.x * ((pitch - width) * 0.5f), pos.y, pos.z);
            PSMTXConcat(camera->view, trans, trans);
            GXLoadPosMtxImm(trans, 0);
            fontDrawCodeEdge(code);
            if (pitch < 0.0f) {
                x += fontScale.x * (width + charspace[gp->language]);
            } else {
                x += pitch * fontScale.x;
            }
#else
            code = JUTFont_CodeToGlyph(FONT_CH(*msg++));
        }
        width = fontGetWidth(code)->width;
        PSMTXTransApply(scale, trans, pitch < 0.0f ? x : x + fontScale.x * ((pitch - width) * 0.5f), pos.y, pos.z);
        PSMTXConcat(camera->view, trans, trans);
        GXLoadPosMtxImm(trans, 0);
        fontDrawCodeEdge(code);
        if (pitch < 0.0f) {
            x += fontScale.x * (width + charspace[gp->language]);
        } else {
            x += pitch * fontScale.x;
#endif
        }
    }
}
#ifdef VERSION_G8ME01
#pragma auto_inline reset
#endif
static inline void fontDrawPairXI(u16 code, s16 x, s16 y) {
    s16 iy = y;
    s16 ix = x;
    _JUTFont_DrawPos(code, ix - 2, (s16)iy);
    _JUTFont_DrawPos(code, ix + 2, (s16)iy);
}

static inline void fontDrawPairYI(u16 code, s16 x, s16 y) {
    s16 iy = y;
    s16 ix = x;
    _JUTFont_DrawPos(code, ix, iy - 2);
    _JUTFont_DrawPos(code, ix, iy + 2);
}

static inline void fontDrawPairX(u16 code, f32 x, f32 y) {
    s16 ix = x;
    s16 iy = y;
    _JUTFont_DrawPos(code, ix - 2, (s16)iy);
    _JUTFont_DrawPos(code, ix + 2, (s16)iy);
}

static inline void fontDrawPairY(u16 code, f32 x, f32 y) {
    s16 ix = x;
    s16 iy = y;
    _JUTFont_DrawPos(code, (s16)ix, iy - 2);
    _JUTFont_DrawPos(code, (s16)ix, iy + 2);
}

static inline void fontDrawCodeEdgeXY(u16 code, f32 x, f32 y) {
    if (fontEdge) {
        GXColor save = fontColor;
        FontDrawColorIDX(0);
        fontDrawPairX(code, x, y);
        fontDrawPairY(code, x, y);
        FontDrawColor(save);
    }
    _JUTFont_DrawPos(code, x, y);
}

void FontDrawStringMtx(Mtx mtx, const char* msg) {
    CameraEntry* camera;
    Mtx m;
    s8 tag[0x80];
    s8 value[0x80];
    u16 code;
    f32 x, y;

    camera = camGetCurPtr();
    x = 0.0f;
    y = x;
    PSMTXConcat(camera->view, mtx, m);
    GXLoadPosMtxImm(m, 0);
    while (*(const s8*)msg) {
        code = 0xFFFF;
        if (_ismbblead(*(const s8*)msg)) {
            code = JUTFont_CodeToGlyph(*(u16*)msg);
            msg += 2;
        } else {
            switch (*(const s8*)msg) {
                case '\n':
                    msg++;
                    x = 0.0f;
                    y -= 28.0f;
                    break;
                case '<':
                    msg = msgGetCommand((char*)(msg + 1), (char*)tag, (char*)value);
                    if (tag[0] == 'R' || (tag[0] == '/' && tag[0] == (u32)'/r')) {
                        break;
                    }
                    if (*(const s8*)msg == '\n') {
                        msg++;
                    }
                    break;
                default:
                    code = JUTFont_CodeToGlyph(FONT_CH(*((const s8*)msg)++));
                    break;
            }
        }
        if (code != 0xFFFF) {
            fontDrawCodeEdgeXY(code, x, y);
            x += fontGetWidth(code)->width + charspace[gp->language];
        }
    }
}

static inline void fontDrawCodeEdgeX(u16 code, f32 x) {
    if (fontEdge) {
        GXColor save = fontColor;
        FontDrawColorIDX(0);
        _JUTFont_DrawPos(code, (s16)x - 2, 0);
        _JUTFont_DrawPos(code, (s16)x + 2, 0);
        _JUTFont_DrawPos(code, x, -2);
        _JUTFont_DrawPos(code, x, 2);
        FontDrawColor(save);
    }
    _JUTFont_DrawPos(code, x, 0);
}

void FontDrawStringCenterMtx(Mtx mtx, const s8* msg) {
    f32 x;
    u16 code;
    CameraEntry* camera;
    Mtx m;

    camera = camGetCurPtr();
    x = -(f32)FontGetMessageWidthLine((const char*)msg, NULL) * 0.5f;
    PSMTXConcat(camera->view, mtx, m);
    GXLoadPosMtxImm(m, 0);
    while (*msg) {
        if (_ismbblead(*msg)) {
            code = JUTFont_CodeToGlyph(*(u16*)msg);
            msg += 2;
        } else {
            code = JUTFont_CodeToGlyph(FONT_CH(*msg++));
        }
        fontDrawCodeEdgeXY(code, x, 0.0f);
        x += fontGetWidth(code)->width + charspace[gp->language];
    }
}

inline u8 fontGetGlyphWidth(u16 code) {
    return fontGetWidth(code)->width;
}

void FontDrawStringShake(f32 x, f32 y, const s8* msg) {
    CameraEntry* camera;
    Mtx trans, scale;
    u8 width;
    u16 code;
    f32 sx, sy;

#ifdef VERSION_G8ME01
    // FAKE: dead guard (folded after register allocation, emits no code). It only creates the -1.0f
    // .sdata2 literal at this point, which puts it before the 28.0f of FontDrawStringMtx like the target.
    {
        s32 gz = 0;
        if (gz != 0) {
            x = -1.0f;
        }
    }
#endif
    camera = camGetCurPtr();
    PSMTXScale(scale, fontScale.x, fontScale.y, fontScale.z);
    while (*msg) {
        if (_ismbblead(*msg)) {
            code = JUTFont_CodeToGlyph(*(u16*)msg);
            msg += 2;
        } else {
            code = JUTFont_CodeToGlyph(FONT_CH(*msg++));
        }
        sx = x + fontScale.x * (irand(10000) % 3);
        sy = y + fontScale.y * (irand(10000) % 3);
        width = fontGetGlyphWidth(code);
        PSMTXTransApply(scale, trans, sx, sy, 0.0f);
        PSMTXConcat(camera->view, trans, trans);
        GXLoadPosMtxImm(trans, 0);
        fontDrawCodeEdge(code);
        x += fontScale.x * (width + charspace[gp->language]);
    }
}

static inline void fontDrawCodeEdgeI(u16 code, s16 x, s16 y) {
    if (fontEdge) {
        GXColor save = fontColor;
        FontDrawColorIDX(0);
        fontDrawPairXI(code, x, y);
        fontDrawPairYI(code, x, y);
        FontDrawColor(save);
    }
    _JUTFont_DrawPos(code, x, y);
}

void FontDrawMessage(s32 x, s32 y, const char* msg) {
    s32 cx;
    s32 cy;
    u16 code;
    s8 tag[0x80];
    s8 value[0x80];
    char name[0x20];
    Vec pos;
    GXColor color;
    s32 id;
    s32 d1, d2, d3;
    f32 scale;
    u16 w, h;

    cx = x;
    cy = y;
    while (*(const s8*)msg) {
        code = 0xFFFF;
        if (_ismbblead(*(const s8*)msg)) {
            code = JUTFont_CodeToGlyph(*(u16*)msg);
            msg += 2;
        } else {
            switch (*(const s8*)msg) {
                case '\n':
                    cx = x;
                    cy -= 28;
                    msg++;
                    break;
                case '<':
                    msg = msgGetCommand((char*)(msg + 1), (char*)tag, (char*)value);
                    if (!strcmp((char*)tag, "red")) {
                        FontDrawColor((GXColor){0xE0, 0x00, 0x00, 0xFF});
                    } else if (!strcmp((char*)tag, "/red")) {
                        FontDrawColor(fontColTbl[0]);
                    } else if (!strcmp((char*)tag, "col")) {
                        sscanf((char*)value, "%x", &color);
                        FontDrawColor(color);
                    } else if (!strcmp((char*)tag, "/col")) {
                        FontDrawColor(fontColTbl[0]);
                    } else if (!strcmp((char*)tag, "icon")) {
                        camGetCurPtr();
                        if (value[0] >= '0' && value[0] <= '9') {
                            sscanf((char*)value, "%d%f%d%d%d", &id, &scale, &d1, &d2, &d3);
                        } else {
                            sscanf((char*)value, "%s%f%d%d%d", name, &scale, &d1, &d2, &d3);
                            id = msgIconStr2ID(name);
                            if (id < 0) {
                                id = 0;
                            }
                        }
                        iconGetWidthHight(&w, &h, (u16)id);
                        pos.x = d1 + (cx + 0.5f * w * scale);
                        pos.y = d2 + (cy - h * scale);
                        pos.z = 0.0f;
                        iconDispGxAlpha(&pos, 0x10, id, fontAlpha, scale);
                        FontDrawStart_alpha(fontAlpha);
                        FontDrawColor(fontColor);
                        cx += w * scale + d3;
                    } else if (tag[0] == 'R' || (tag[0] == '/' && tag[0] == (u32)'/r')) {
                        break;
                    }
                    if (*(const s8*)msg == '\n') {
                        msg++;
                    }
                    break;
                default:
                    code = JUTFont_CodeToGlyph(FONT_CH(*((const s8*)msg)++));
                    break;
            }
        }
        if (code != 0xFFFF) {
            fontDrawCodeEdgeI(code, cx, cy);
            cx += fontGetWidth(code)->width + charspace[gp->language];
        }
    }
}

void FontDrawMessageMtx(Mtx mtx, const char* msg) {
    CameraEntry* camera;
    u16 code;
    s32 cx;
    s32 cy;
    f32 x;
    s8 tag[0x80];
    s8 value[0x80];
    Mtx m2;
    Mtx m;
    char name[0x20];
    Vec pos;
    s32 id;
    s32 d1, d2, d3;
    f32 scale;
    u16 w, h;

    camera = camGetCurPtr();
    cx = 0;
    cy = 0;
    x = 0.0f;
    while (*(const s8*)msg) {
        code = 0xFFFF;
        if (_ismbblead(*(const s8*)msg)) {
            code = JUTFont_CodeToGlyph(*(u16*)msg);
            msg += 2;
        } else {
            switch (*(const s8*)msg) {
                case '\n':
                    cx = x;
                    cy -= 28;
                    msg++;
                    break;
                case '<':
                    msg = msgGetCommand((char*)(msg + 1), (char*)tag, (char*)value);
                    if (!strcmp((char*)tag, "red")) {
                        FontDrawColor((GXColor){0xE0, 0x00, 0x00, 0xFF});
                    } else if (!strcmp((char*)tag, "/red")) {
                        FontDrawColor(fontColTbl[0]);
                    } else if (!strcmp((char*)tag, "icon")) {
                        camGetCurPtr();
                        if (value[0] >= '0' && value[0] <= '9') {
                            sscanf((char*)value, "%d%f%d%d%d", &id, &scale, &d1, &d2, &d3);
                        } else {
                            sscanf((char*)value, "%s%f%d%d%d", name, &scale, &d1, &d2, &d3);
                            id = msgIconStr2ID(name);
                            if (id < 0) {
                                id = 0;
                            }
                        }
                        iconGetWidthHight(&w, &h, (u16)id);
                        pos.x = d1 + (cx + 0.5f * w * scale);
                        pos.y = d2 + (cy - h * scale);
                        pos.z = 0.0f;
                        PSMTXMultVec(mtx, &pos, &pos);
                        // FAKE: no-op self-assignment before the icon draw; without it the scale load moves
                        scale = scale;
                        iconDispGxAlpha(&pos, 0x10, id, fontAlpha, scale);
                        FontDrawStart_alpha(fontAlpha);
                        FontDrawColor(fontColor);
                        cx += w * scale + d3;
                    } else if (tag[0] == 'R' || (tag[0] == '/' && tag[0] == (u32)'/r')) {
                        break;
                    }
                    if (*(const s8*)msg == '\n') {
                        msg++;
                    }
                    break;
                default:
                    code = JUTFont_CodeToGlyph(FONT_CH(*((const s8*)msg)++));
                    break;
            }
        }
        if (code != 0xFFFF) {
            PSMTXTrans(m, cx, cy, 0.0f);
            PSMTXConcat(camera->view, mtx, m2);
            PSMTXConcat(m2, m, m2);
            GXLoadPosMtxImm(m2, 0);
            fontDrawCodeEdge(code);
            cx += fontGetWidth(code)->width + charspace[gp->language];
        }
    }
}

u16 hankakuSearch(s8 c) {
    return JUTFont_CodeToGlyph(FONT_CH(c));
}

u16 kanjiSearch(u32 code) {
    return JUTFont_CodeToGlyph(code);
}

u16 kanjiGetWidth(u16 code) {
    return fontGetWidth(code)->width;
}

u16 FontGetMessageWidthLine(const char* msg, u16* lines) {
    u16 code;
    u16 maxWidth = 0;
    u16 width = 0;
    s32 lineCount = 0;
    s8 tag[0x80];
    s8 value[0x80];
    char name[0x20];
    s32 id;
    s32 d1, d2, d3;
    f32 scale;
    u16 w, h;

    while (*(const s8*)msg) {
        code = 0xFFFF;
        if (_ismbblead(*(const s8*)msg)) {
            code = JUTFont_CodeToGlyph(*(u16*)msg);
            msg += 2;
        } else {
            switch (*(const s8*)msg) {
                case '\n':
                    if (maxWidth < width) {
                        maxWidth = width;
                    }
                    width = 0;
                    lineCount++;
                    msg++;
                    break;
                case '<':
                    msg = msgGetCommand((char*)(msg + 1), (char*)tag, (char*)value);
                    if (!strcmp((char*)tag, "icon")) {
                        camGetCurPtr();
                        if (value[0] >= '0' && value[0] <= '9') {
                            sscanf((char*)value, "%d%f%d%d%d", &id, &scale, &d1, &d2, &d3);
                        } else {
                            sscanf((char*)value, "%s%f%d%d%d", name, &scale, &d1, &d2, &d3);
                            id = msgIconStr2ID(name);
                            if (id < 0) {
                                id = 0;
                            }
                        }
                        iconGetWidthHight(&w, &h, (u16)id);
                        width += w * scale + d3;
                    } else if (tag[0] == 'R' || (tag[0] == '/' && tag[0] == (u32)'/r')) {
                        break;
                    }
                    if (*(const s8*)msg == '\n') {
                        msg++;
                    }
                    break;
                default:
                    code = JUTFont_CodeToGlyph(FONT_CH(*((const s8*)msg)++));
                    break;
            }
        }
        if (code != 0xFFFF) {
            width += fontGetWidth(code)->width + charspace[gp->language];
        }
    }
    if (maxWidth < width) {
        maxWidth = width;
    }
    if (maxWidth) {
        if (code && code) {
        }
        maxWidth -= charspace[gp->language];
    }
    if (lines) {
        *lines = lineCount;
    }
    return maxWidth;
}

u16 FontGetMessageWidth(const char* message) {
    return FontGetMessageWidthLine(message, NULL);
}

GXColor HSV2RGB(GXColor hsv) {
    f32 h, s, v, f, p, q, r, g, b;
    s32 i;
    GXColor out;

    s = hsv.g / 255.0f;
    v = hsv.b / 255.0f;
    h = (360.0f * hsv.r) / 256.0f / 60.0f;
    i = floor(h);
    f = h - i;
    if (!(i & 1)) {
        f = 1.0f - f;
    }
    p = v * (1.0f - s);
    q = v * (1.0f - s * f);
    switch (i) {
        case 0: r = v; g = q; b = p; break;
        case 1: r = q; g = v; b = p; break;
        case 2: r = p; g = v; b = q; break;
        case 3: r = p; g = q; b = v; break;
        case 4: r = q; g = p; b = v; break;
        case 5: r = v; g = p; b = q; break;
    }
    out.a = hsv.a;
    out.r = 255.0f * r;
    out.g = 255.0f * g;
    out.b = 255.0f * b;
    return out;
}

void JUTFontSetup(u16 font) {
#ifdef VERSION_G8ME01
    DVDEntry* dvd;
    u32 size;
#else
    u32 size;
    DVDEntry* dvd;
#endif
    BinaryFontBlock* ptr;
    char path[128];
    u32 i;
#ifdef VERSION_G8ME01
    s32 maxsize;
    s32 j;
    DVDEntry* other;
    s32 len;
#endif

    if (!pfh) {
        sprintf(path, "%s/f/%s.bfn", getMarioStDvdRoot(), fontname[font]);
        dvd = DVDMgrOpen(path, 2, 0);
        size = DVDMgrGetLength(dvd);
#ifdef VERSION_G8ME01
        maxsize = size;
        for (j = 0; j < 7; j++) {
            sprintf(path, "%s/font/%s.bfn", getMarioStDvdRoot(), fontname[j]);
            other = DVDMgrOpen(path, 2, 0);
            if (other) {
                len = DVDMgrGetLength(other);
                if (maxsize < len) {
                    maxsize = len;
                }
                DVDMgrClose(other);
            }
        }
        pfh = __memAlloc(HEAP_DEFAULT, OSRoundUp32B(maxsize));
        DVDMgrRead(dvd, pfh, OSRoundUp32B(size), 0);
#else
        pfh = __memAlloc(HEAP_DEFAULT, size = OSRoundUp32B(size));
        DVDMgrRead(dvd, pfh, size, 0);
#endif
        DVDMgrClose(dvd);
        ptr = (BinaryFontBlock*)(pfh + 1);
        for (i = 0; i < pfh->blockcount; i++) {
            switch (ptr->magic) {
                case 'INF1':
                    pfi = (BinaryFontInfo*)ptr;
                    break;
                case 'GLY1':
                    blockGlyph = (BinaryFontGlyph*)ptr;
                    break;
                case 'MAP1':
                case 'WID1':
                default:
                    break;
            }
            ptr = (BinaryFontBlock*)((u8*)ptr + ptr->size);
        }
        GXInitTexObj(&fgTexObj, &blockGlyph[1], blockGlyph->sheetwidth, blockGlyph->sheetheight,
            GX_TF_I4, GX_CLAMP, GX_CLAMP, GX_FALSE);
        GXInitTexObjLOD(&fgTexObj, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
        fontGetWidth(JUTFont_CodeToGlyph(' '))->width = 7;
        fontGetWidth(JUTFont_CodeToGlyph(0x8140))->width = 9;
    }
}

#ifndef VERSION_G8ME01
void JUTFont_Free(void) {
    if (pfh) {
        __memFree(HEAP_DEFAULT, pfh);
        pfh = NULL;
    }
}
#endif

u16 JUTFont_CodeToGlyph(u32 code) {
    BinaryFontMap* map;
    u32 blockcount;
    u16 glyph, basecode;
    u8 sjis[2];
    u32 lead, index;
    s32 max, min;
    u32 i;

    map = (BinaryFontMap*)&pfh[1];
    glyph = pfi->replace;
    blockcount = pfh->blockcount;
    for (i = 0; i < blockcount; i++) {
        if (map->magic == 'MAP1') { //search for a correct mapping
            basecode = map->firstchar;
            if (((u16)code) >= basecode && ((u16)code) <= map->lastchar) {
                switch (map->mapping) {
                    case LINEAR_CHARACTER_MAP:
                        glyph = (u16)(code - basecode);
                        break;

                    case SJIS_CHARACTER_MAP:
                        *(u16*)sjis = (u16)code;
                        lead = sjis[0];
                        index = sjis[1];
                        if (sjis[1] >= 0x80) {
                            index -= 65;
                        } else {
                            index -= 64;
                        }

                        if (!map->count) {
                            glyph = (u16)((((lead - 136) * 188) + index) + 702);
                        } else {
                            glyph = (u16)(((lead - 136) * 188) + index - 94 + map->table[0]);
                        }
                    break;

                    case TABLE_CHARACTER_MAP:
                        glyph = map->table[(u16)code - basecode];
                        break;

                    case MAP_CHARACTER_MAP:
                        max = map->count;
                        min = 0;

                        while (max >= min) {
                            s32 index = (min + max) / 2;
                            if ((u16)code == map->table[index * 2]) {
                                glyph = map->table[2 * index + 1];
                                break;
                            }
                            if ((u16)code < map->table[2 * index]) {
                                max = index - 1;
                            } else {
                                min = index + 1;
                            }
                        }
                    break;
                }
                break;
            }
        }
        map = (BinaryFontMap*)((u8*)map + map->size);
    }
    return glyph;
}

void JUTFont_DrawStart(GXColor color) {
    CameraEntry* camera;
    Mtx mtx;

    camera = camGetCurPtr();
    __GXSetIndirectMask(0);
    GXSetCullMode(GX_CULL_NONE);
    GXSetZCompLoc(GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_OR);
    GXSetZMode(GX_TRUE, GX_ALWAYS, GX_FALSE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX1, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_U16, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX1, GX_TEX_ST, GX_U8, 0);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_VTX, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumChans(1);
    GXSetNumTexGens(1);
    GXSetTevColor(GX_TEVREG0, color);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_C0, GX_CC_ONE, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
    GXLoadTexObj(&fgTexObj, GX_TEXMAP0);
#ifdef VERSION_G8ME01
    if ((u32)gp->language == LANGUAGE_JP) {
        PSMTXScale(mtx, 0.001953125f, 0.0009765625f, 1.0f);
    } else {
        PSMTXScale(mtx, 0.001953125f, 0.00390625f, 1.0f);
    }
#else
    PSMTXScale(mtx, 0.001953125f, 0.0009765625f, 1.0f);
#endif
    GXLoadTexMtxImm(mtx, GX_TEXMTX0, GX_MTX2x4);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    GXLoadPosMtxImm(camera->view, 0);
    GXSetCurrentMtx(0);
    GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, (GXColor){0, 0, 0, 0});
}

// FAKE: fontHue/fontHueX/fontPos are invented inline helpers. Their params become @ temps created at the vertex 3/4
// call sites (hr1 = vertex 3 hue, hr0 = vertex 4 hue, x1s = vertex 3 x), which is the register numbering the target needs.
static inline void fontHue(GXColor* hsv, s32 hr) {
    hsv->r = hr;
}

static inline void fontHueX(GXColor* hsv, const s16 x, u8 hue) {
    fontHue(hsv, x + hue);
}

static inline void fontPos(const s16 x, s16 y, GXColor* hsv, u8 hue) {
    GXPosition3s16(x, y, 0);
    fontHueX(hsv, x, hue);
}

void _JUTFont_DrawPos(u16 code, s16 x, s16 y) {
    s32 row;
    u8 hue;
    s16 x1;
    u8 n0;
    s16 y0;
    u8 n1;
    s32 x0;
    s16 y1;
    u16 v0;
    GXColor hsv;
    s32 col;
    BinaryFontGlyph* glyph;
    f32 cellh, cellw, tu, w, h, tw;
    FontWidthEntry* entry;
    GXColor c;
    u16 u0, u1, v1;
    u8 t0, t1;
    f32 tv;
    f32 scale;
    u8 kern;
    entry = fontGetWidth(code);
    kern = entry->kerning;
    glyph = blockGlyph;
    row = (code - glyph->firstcode) / glyph->sheetrows;
    col = (code - glyph->firstcode) % glyph->sheetrows;
    row = row % glyph->sheetcolumns;
    cellh = glyph->cellheight;
    cellw = glyph->cellwidth;
    tu = col * cellw + kern;
    tw = entry->width;
    scale = 1.0f;
    w = entry->width * scale;
#ifdef VERSION_G8ME01
    h = (cellh - 2.0f) * scale;
#else
    h = cellh * scale;
#endif

    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    n0 = irand(64);
    n1 = irand(64);
    x0 = x;
#ifdef VERSION_G8ME01
    y0 = y - 2;
#else
    y0 = y - 1;
#endif
    x1 = x0 + w;
    hue = x0 - (u8)gp->retraceCount * 2;
    hsv = (GXColor){0x00, 0xAA, 0xFF, 0xFF};
    tv = row * cellh;
#ifdef VERSION_G8ME01
    v0 = 2.0f + tv;
#else
    v0 = 1.0f + tv;
#endif
    GXPosition3s16(x, y0, 0);
    fontHue(&hsv, x0 + hue);
    c = HSV2RGB(hsv);
    u0 = tu;
    GXColor4u8(c.r, c.g, c.b, c.a);
    GXTexCoord2u16(u0, v0);
    GXTexCoord2u8(n0, n1);

    // FAKE: vertex 2 x written as u16 (same 16 bits) so its first extsh is the vertex 3 fontPos param, not a store temp
    GXWGFifo.u16 = x1;
    GXWGFifo.s16 = y0;
    GXWGFifo.s16 = 0;
    fontHueX(&hsv, x1, hue);
    c = HSV2RGB(hsv);
    u1 = tu + tw;
    GXColor4u8(c.r, c.g, c.b, c.a);
    t0 = n0 + w;
    GXTexCoord2u16(u1, v0);
    GXTexCoord2u8(t0, n1);

    y1 = (s32)(y0 - h);
    fontPos(x1, y1, &hsv, hue);
    c = HSV2RGB(hsv);
    GXColor4u8(c.r, c.g, c.b, c.a);
#ifdef VERSION_G8ME01
    v1 = v0 + (cellh - 2.0f);
#else
    v1 = v0 + cellh;
#endif
    t1 = n1 + h;
    GXTexCoord2u16(u1, v1);
    GXTexCoord2u8(t0, t1);

    GXPosition3s16(x0, y1, 0);
    fontHue(&hsv, x0 + hue);
    c = HSV2RGB(hsv);
    GXColor4u8(c.r, c.g, c.b, c.a);
    GXTexCoord2u16(u0, v1);
    GXTexCoord2u8(n0, t1);
}


