// Font manager: loads the font file and draws text (strings, messages with tags, centred and rotated
// text, edges, colour effects) glyph by glyph.

#include "version.h"
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
s32 charspace[VERSION_PICK(2, 7)] = {
    4, 2,
#if VERSION >= VERSION_US
    2, 2, 2, 2, 2
#endif
};

#if VERSION >= VERSION_US
const char* fontname[7] = {"papermarioset_JPN", "papermarioset_US", "papermarioset_EU", "papermarioset_EU",
    "papermarioset_EU", "papermarioset_EU", "papermarioset_US"};
#else
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

// US and later pass single-byte characters to JUTFont_CodeToGlyph zero-extended
#if VERSION >= VERSION_US
#define FONT_CH(c) ((u8)(c))
#else
#define FONT_CH(c) (c)
#endif


//local prototypes
void JUTFont_DrawStart(GXColor color);
static void _JUTFont_DrawPos(u16 code, s16 x, s16 y);
u16 JUTFont_CodeToGlyph(u32 code);

/** Glyph of a single-byte character. */
static inline u16 fontByteGlyph(s8 c) {
    return JUTFont_CodeToGlyph(FONT_CH(c));
}

#if VERSION >= VERSION_US
void FontDrawStringVecPitch(Vec pos, f32 pitch, const s8* msg);
#endif

/**
 * The width entry (kerning and advance width in pixels) of a glyph, from the font's WID1 block that covers it;
 * NULL if no block does (callers do not check).
 */
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

/**
 * Draws one glyph at the current position matrix origin;
 * with FontDrawEdge on, first draws it four times in the outline colour (fontColTbl[0]) offset 2 px left, right,
 * down and up, then restores the colour.
 */
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

/**
 * Font manager start-up, called once from the boot code in mariost.c.
 * Empty: the font is loaded later by fontmgrTexSetup.
 */
void fontmgrInit(void) {
}

/**
 * Loads the font selected by gp->language in US and later (the September demos too), index 1 in the June US
 * demo, and index 0 in JP and the JP demo. Does nothing once a font is loaded.
 */
void fontmgrTexSetup(void) {
#if VERSION == VERSION_DEMO_US_1
    JUTFontSetup(1);
#elif VERSION >= VERSION_US
    JUTFontSetup(gp->language);
#else
    JUTFontSetup(0);
#endif
}

/** Starts a block of text drawing at full opacity; see FontDrawStart_alpha. */
void FontDrawStart(void) {
    FontDrawStart_alpha(0xFF);
}

/**
 * Starts a block of text drawing: resets the colour to the default dark text colour,
 * the scale to 1 and the outline off, remembers alpha (0-255) as the multiplier for every later colour,
 * and sets up GX for glyphs.
 */
void FontDrawStart_alpha(u8 alpha) {
    GXColor start;

    fontColor = start = fontColTbl[0];
    fontAlpha = alpha;
    start.a = (start.a * alpha) / 255;
    fontScale = (Vec){1.0f, 1.0f, 1.0f};
    fontEdge = FALSE;
    JUTFont_DrawStart(start);
}

/** Turns on the outline: following glyphs get a 2 px border in fontColTbl[0] drawn under them. */
void FontDrawEdge(void) {
    fontEdge = TRUE;
}

/** Turns the glyph outline back off. */
void FontDrawEdgeOff(void) {
    fontEdge = FALSE;
}

/**
 * Adds a second TEV stage that multiplies the text by the vertex colour,
 * which _JUTFont_DrawPos always sets to a hue cycling with x and time;
 * msgdrv uses it for rainbow-coloured message glyphs.
 */
void FontDrawRainbowColor(void) {
    GXSetNumTevStages(2);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_CPREV, GX_CC_RASC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_APREV, GX_CA_RASA, GX_CA_ZERO);
}

/** Drops back to the single TEV stage, ending FontDrawRainbowColor. */
void FontDrawRainbowColorOff(void) {
    GXSetNumTevStages(1);
}

/**
 * Adds a second TEV stage that multiplies the glyph alpha by a tiling noise texture (window texture 50);
 * msgdrv uses it for the <noise> message tag, which makes the text look grainy.
 */
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

/** Ends FontDrawNoise: back to one texture coordinate and one TEV stage. */
void FontDrawNoiseOff(void) {
    GXSetNumTexGens(1);
    GXSetNumTevStages(1);
}

/** Sets the text colour from fontColTbl: 0 dark default (also the outline colour), 1 near white, 2 orange. */
void FontDrawColorIDX(s32 idx) {
    FontDrawColor(fontColTbl[idx]);
}

/** Sets the text colour; its alpha is scaled by the alpha given to FontDrawStart_alpha before it is loaded. */
void FontDrawColor(GXColor color) {
    fontColor = color;
    color.a = (color.a * fontAlpha) / 255;
    GXSetTevColor(GX_TEVREG0, color);
}

/** Reloads the current text colour into TEV register 0 (msgdrv calls it after other drawing changed the register). */
void FontDrawColor_(void) {
    FontDrawColor(fontColor);
}

/** The current text colour as last set, before the alpha scaling. */
GXColor FontGetDrawColor(void) {
    return fontColor;
}

/** Sets a uniform text scale for the following FontDrawCode/FontDrawString* calls (reset to 1 by FontDrawStart). */
void FontDrawScale(f32 scale) {
    fontScale.x = scale;
    fontScale.y = scale;
    fontScale.z = scale;
}

/** Sets a per-axis text scale; see FontDrawScale. */
void FontDrawScaleVec(Vec scale) {
    fontScale = scale;
}

/**
 * Draws one glyph (an index from JUTFont_CodeToGlyph) at x, y with the current scale and outline;
 * msgdrv draws message windows glyph by glyph through this.
 */
void FontDrawCode(f32 x, f32 y, u16 code) {
    Mtx mtx;

    PSMTXScale(mtx, fontScale.x, fontScale.y, fontScale.z);
    PSMTXTransApply(mtx, mtx, x, y, 0.0f);
    PSMTXConcat(camGetCurPtr()->view, mtx, mtx);
    GXLoadPosMtxImm(mtx, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    fontDrawCodeEdge(code);
}

/**
 * Draws one glyph through the given model matrix (changed in place: the camera view is multiplied in);
 * msgdrv uses it for rotated or scaled message glyphs.
 */
void FontDrawCodeMtx(Mtx mtx, u16 code) {
    PSMTXConcat(camGetCurPtr()->view, mtx, mtx);
    GXLoadPosMtxImm(mtx, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    fontDrawCodeEdge(code);
}

/**
 * JP and early-demo body of FontDrawString: draws a plain string (no tags, no line breaks) left to right from pos with
 * the current scale and outline, Shift-JIS pairs as one glyph.
 */
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
        GXLoadPosMtxImm(trans, GX_PNMTX0);
        fontDrawCodeEdge(code);
        x += fontScale.x * (width + charspace[gp->language]);
    }
}

/**
 * Draws a single-line string at x, y with proportional spacing;
 * message tags are drawn as text in JP and the early demos and skipped in US and later.
 */
// US and later: FontDrawString and FontDrawStringPitch are wrappers over FontDrawStringVecPitch
void FontDrawString(f32 x, f32 y, const char* msg) {
#if VERSION >= VERSION_US
    // US and later: FontDrawString and FontDrawStringPitch are wrappers over FontDrawStringVecPitch
    FontDrawStringVecPitch((Vec){x, y, 0.0f}, -1.0f, (const s8*)msg);
#else
    FontDrawStringVec((Vec){x, y, 0.0f}, (const s8*)msg);
#endif
}

/**
 * JP and the early demos: FontDrawStringPitch's copy of FontDrawStringVecPitch.
 * It walks the string through a local copy (msg = str);
 * calling FontDrawStringVecPitch itself, or giving it the same copy, changes the registers.
 */
static inline void fontDrawStringVecPitch(Vec pos, f32 pitch, const s8* str) {
    CameraEntry* camera;
    Mtx trans, scale;
    u8 width;
    const s8* msg;
    u16 code;
    f32 x;

    // FAKE: walks the string through a local copy (msg = str); using str directly or calling FontDrawStringVecPitch changes the registers
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
        GXLoadPosMtxImm(trans, GX_PNMTX0);
        fontDrawCodeEdge(code);
        if (pitch < 0.0f) {
            x += fontScale.x * (width + charspace[gp->language]);
        } else {
            x += pitch * fontScale.x;
        }
    }
}

/**
 * Draws a single-line string at x, y with every glyph centred in a fixed cell pitch pixels wide (monospaced,
 * e.g. for numbers); a negative pitch means proportional spacing like FontDrawString.
 */
void FontDrawStringPitch(f32 x, f32 y, f32 pitch, const char* msg) {
#if VERSION >= VERSION_US
    FontDrawStringVecPitch((Vec){x, y, 0.0f}, pitch, (const s8*)msg);
#else
    fontDrawStringVecPitch((Vec){x, y, 0.0f}, pitch, (const s8*)msg);
#endif
}

#if VERSION >= VERSION_US
// FAKE: auto_inline off keeps the wrappers from inlining FontDrawStringVecPitch (the US-and-later wrappers call it)
#pragma auto_inline off
#endif

/**
 * Draws a single-line string from pos, fixed pitch or (pitch < 0) proportional;
 * the shared body of FontDrawString and FontDrawStringPitch in US and later,
 * where it also skips <tag>s.
 */
void FontDrawStringVecPitch(Vec pos, f32 pitch, const s8* msg) {
    CameraEntry* camera;
    Mtx trans, scale;
#if VERSION >= VERSION_US
    char cmd[128];
    char arg[128];
#endif
    u8 width;
    u16 code;
    f32 x;
#if VERSION >= VERSION_US
    s8 c;
#endif

    camera = camGetCurPtr();
    x = pos.x;
    PSMTXScale(scale, fontScale.x, fontScale.y, fontScale.z);
    while (*msg) {
#if VERSION >= VERSION_US
        code = 0xFFFF;
#endif
        if (_ismbblead(*msg)) {
            code = JUTFont_CodeToGlyph(*(u16*)msg);
            msg += 2;
        } else {
#if VERSION >= VERSION_US
            // US and later: skips <command> tags instead of drawing them
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
            GXLoadPosMtxImm(trans, GX_PNMTX0);
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
        GXLoadPosMtxImm(trans, GX_PNMTX0);
        fontDrawCodeEdge(code);
        if (pitch < 0.0f) {
            x += fontScale.x * (width + charspace[gp->language]);
        } else {
            x += pitch * fontScale.x;
#endif
        }
    }
}

#if VERSION >= VERSION_US
#pragma auto_inline reset
#endif

/** The left/right pair of a glyph's four outline copies, 2 px either side of an integer x, y. */
static inline void fontDrawPairXI(u16 code, s16 x, s16 y) {
    // FAKE: spare s16 copies of the s16 parameters (and a no-op cast); using x and y directly changes the object
    s16 iy = y;
    s16 ix = x;
    _JUTFont_DrawPos(code, ix - 2, (s16)iy);
    _JUTFont_DrawPos(code, ix + 2, (s16)iy);
}

/** The down/up pair of outline copies for an integer position; see fontDrawPairXI. */
static inline void fontDrawPairYI(u16 code, s16 x, s16 y) {
    // FAKE: spare s16 copies of the s16 parameters; using x and y directly changes the object
    s16 iy = y;
    s16 ix = x;
    _JUTFont_DrawPos(code, ix, iy - 2);
    _JUTFont_DrawPos(code, ix, iy + 2);
}

/** The left/right pair of outline copies for a float position (truncated to pixels). */
static inline void fontDrawPairX(u16 code, f32 x, f32 y) {
    s16 ix = x;
    s16 iy = y;
    // FAKE: no-op (s16) casts on iy; the plain argument changes the object
    _JUTFont_DrawPos(code, ix - 2, (s16)iy);
    _JUTFont_DrawPos(code, ix + 2, (s16)iy);
}

/** The down/up pair of outline copies for a float position (truncated to pixels). */
static inline void fontDrawPairY(u16 code, f32 x, f32 y) {
    s16 ix = x;
    s16 iy = y;
    // FAKE: no-op (s16) casts on ix; the plain argument changes the object
    _JUTFont_DrawPos(code, (s16)ix, iy - 2);
    _JUTFont_DrawPos(code, (s16)ix, iy + 2);
}

/** Draws one glyph at x, y in the already-loaded matrix, with the outline first when FontDrawEdge is on. */
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

/**
 * Draws a multi-line string through a model matrix, line height 28; <tags> are skipped, not interpreted.
 * Used for the speech-bubble effects, battle menu text, telops and block messages.
 */
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
    GXLoadPosMtxImm(m, GX_PNMTX0);
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
                    // BUG: '/r' is a multi-character constant (0x2F72), so the closing ruby tag test is always false
                    if (tag[0] == 'R' || (tag[0] == '/' && tag[0] == (u32)'/r')) {
                        break;
                    }
                    if (*(const s8*)msg == '\n') {
                        msg++;
                    }
                    break;
                default:
                    code = fontByteGlyph(*msg++);
                    break;
            }
        }
        if (code != 0xFFFF) {
            fontDrawCodeEdgeXY(code, x, y);
            x += fontGetWidth(code)->width + charspace[gp->language];
        }
    }
}

/**
 * Draws one line of text centred on the matrix origin (half of FontGetMessageWidthLine to the left);
 * the battle menu uses it for names.
 */
void FontDrawStringCenterMtx(Mtx mtx, const s8* msg) {
    f32 x;
    u16 code;
    CameraEntry* camera;
    Mtx m;

    camera = camGetCurPtr();
    x = -(f32)FontGetMessageWidthLine((const char*)msg, NULL) * 0.5f;
    PSMTXConcat(camera->view, mtx, m);
    GXLoadPosMtxImm(m, GX_PNMTX0);
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

/** A glyph's advance width in pixels from the WID1 block. */
inline u8 fontGetGlyphWidth(u16 code) {
    return fontGetWidth(code)->width;
}

/**
 * Draws a single-line string with every glyph jittered 0-2 px at random each frame (shaking text);
 * npcdrv uses it for the first-strike bubble text.
 */
void FontDrawStringShake(f32 x, f32 y, const s8* msg) {
    CameraEntry* camera;
    Mtx trans, scale;
    u8 width;
    u16 code;
    f32 sx, sy;
    s32 gz;

#if VERSION >= VERSION_US
    // FAKE: dead guard (folded after register allocation, emits no code). It only creates the -1.0f
    // .sdata2 literal at this point, which puts it before the 28.0f of FontDrawStringMtx like the target.
    gz = 0;
    if (gz != 0) {
        x = -1.0f;
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
        GXLoadPosMtxImm(trans, GX_PNMTX0);
        fontDrawCodeEdge(code);
        x += fontScale.x * (width + charspace[gp->language]);
    }
}

/** Draws one glyph at integer x, y in the loaded matrix, outline first when FontDrawEdge is on. */
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

/**
 * Draws a message at screen position x, y (line height 28), handling the <red>, <col xxxxxxxx> and <icon> tags;
 * other tags are skipped. Used for help windows, battle messages and the list windows.
 */
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
    s32 offsetX, offsetY, spacing;
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
                            sscanf((char*)value, "%d%f%d%d%d", &id, &scale, &offsetX, &offsetY, &spacing);
                        } else {
                            sscanf((char*)value, "%s%f%d%d%d", name, &scale, &offsetX, &offsetY, &spacing);
                            id = msgIconStr2ID(name);
                            if (id < 0) {
                                id = 0;
                            }
                        }
                        iconGetWidthHight(&w, &h, (u16)id);
                        pos.x = offsetX + (cx + 0.5f * w * scale);
                        pos.y = offsetY + (cy - h * scale);
                        pos.z = 0.0f;
                        iconDispGxAlpha(&pos, ICON_FLAG_2D, id, fontAlpha, scale);
                        FontDrawStart_alpha(fontAlpha);
                        FontDrawColor(fontColor);
                        cx += w * scale + spacing;
                    // BUG: '/r' is a multi-character constant (0x2F72), so the closing ruby tag test is always false
                    } else if (tag[0] == 'R' || (tag[0] == '/' && tag[0] == (u32)'/r')) {
                        break;
                    }
                    if (*(const s8*)msg == '\n') {
                        msg++;
                    }
                    break;
                default:
                    code = fontByteGlyph(*msg++);
                    break;
            }
        }
        if (code != 0xFFFF) {
            fontDrawCodeEdgeI(code, cx, cy);
            cx += fontGetWidth(code)->width + charspace[gp->language];
        }
    }
}

/**
 * FontDrawMessage through a model matrix (glyphs and icons transformed by mtx);
 * handles <red> and <icon> but not <col>. Used by the shop, the select windows and the pause menu.
 */
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
    s32 offsetX, offsetY, spacing;
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
                            sscanf((char*)value, "%d%f%d%d%d", &id, &scale, &offsetX, &offsetY, &spacing);
                        } else {
                            sscanf((char*)value, "%s%f%d%d%d", name, &scale, &offsetX, &offsetY, &spacing);
                            id = msgIconStr2ID(name);
                            if (id < 0) {
                                id = 0;
                            }
                        }
                        iconGetWidthHight(&w, &h, (u16)id);
                        pos.x = offsetX + (cx + 0.5f * w * scale);
                        pos.y = offsetY + (cy - h * scale);
                        pos.z = 0.0f;
                        PSMTXMultVec(mtx, &pos, &pos);
                        // FAKE: no-op self-assignment before the icon draw; without it the scale load moves
                        scale = scale;
                        iconDispGxAlpha(&pos, ICON_FLAG_2D, id, fontAlpha, scale);
                        FontDrawStart_alpha(fontAlpha);
                        FontDrawColor(fontColor);
                        cx += w * scale + spacing;
                    // BUG: '/r' is a multi-character constant (0x2F72), so the closing ruby tag test is always false
                    } else if (tag[0] == 'R' || (tag[0] == '/' && tag[0] == (u32)'/r')) {
                        break;
                    }
                    if (*(const s8*)msg == '\n') {
                        msg++;
                    }
                    break;
                default:
                    code = fontByteGlyph(*msg++);
                    break;
            }
        }
        if (code != 0xFFFF) {
            PSMTXTrans(m, cx, cy, 0.0f);
            PSMTXConcat(camera->view, mtx, m2);
            PSMTXConcat(m2, m, m2);
            GXLoadPosMtxImm(m2, GX_PNMTX0);
            fontDrawCodeEdge(code);
            cx += fontGetWidth(code)->width + charspace[gp->language];
        }
    }
}

/** The glyph index of a single-byte (hankaku, half-width) character; msgdrv's entry point to the font map. */
u16 hankakuSearch(s8 c) {
    return JUTFont_CodeToGlyph(FONT_CH(c));
}

/** The glyph index of a two-byte Shift-JIS (kanji or full-width) character code. */
u16 kanjiSearch(u32 code) {
    return JUTFont_CodeToGlyph(code);
}

/** A glyph's advance width in pixels (code is a glyph index, not a character code). */
u16 kanjiGetWidth(u16 code) {
    return fontGetWidth(code)->width;
}

/**
 * The width in pixels of the widest line of a message as FontDrawMessage would lay it out (glyphs,
 * spacing and <icon> widths; other tags take no room), unscaled.
 * If lines is given it receives the number of line breaks (lines minus one).
 */
u16 FontGetMessageWidthLine(const char* msg, u16* lines) {
    u16 code;
    u16 maxWidth = 0;
    u16 width = 0;
    s32 lineCount = 0;
    s8 tag[0x80];
    s8 value[0x80];
    char name[0x20];
    s32 id;
    s32 offsetX, offsetY, spacing;
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
                            sscanf((char*)value, "%d%f%d%d%d", &id, &scale, &offsetX, &offsetY, &spacing);
                        } else {
                            sscanf((char*)value, "%s%f%d%d%d", name, &scale, &offsetX, &offsetY, &spacing);
                            id = msgIconStr2ID(name);
                            if (id < 0) {
                                id = 0;
                            }
                        }
                        iconGetWidthHight(&w, &h, (u16)id);
                        width += w * scale + spacing;
                    // BUG: '/r' is a multi-character constant (0x2F72), so the closing ruby tag test is always false
                    } else if (tag[0] == 'R' || (tag[0] == '/' && tag[0] == (u32)'/r')) {
                        break;
                    }
                    if (*(const s8*)msg == '\n') {
                        msg++;
                    }
                    break;
                default:
                    code = fontByteGlyph(*msg++);
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
        // FAKE: empty dead test of code; removing it changes the object
        if (code && code) {
        }
        maxWidth -= charspace[gp->language];
    }
    if (lines) {
        *lines = lineCount;
    }
    return maxWidth;
}

/** The width in pixels of the widest line of a message; see FontGetMessageWidthLine. */
u16 FontGetMessageWidth(const char* message) {
    return FontGetMessageWidthLine(message, NULL);
}

/**
 * Converts a colour packed as HSV (r = hue, 0-255 for a full circle; g = saturation; b = value; a kept) to RGB.
 * Used for the rainbow glyph vertex colours and by Art Attack (sac_scissor).
 */
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
    out.a = hsv.a;
    out.r = 255.0f * r;
    out.g = 255.0f * g;
    out.b = 255.0f * b;
    return out;
}

/**
 * Loads font file fontname[font] (.bfn) from the disc into the default heap, finds its INF1 and GLY1 blocks,
 * sets up the glyph sheet texture and forces the widths of the space (7) and full-width space (9).
 * Does nothing once a font is loaded.
 * US and later try to size the buffer for the largest of the seven font files, but that loop opens
 * "<root>/font/" while the fonts are in "<root>/f/", so no file opens and the buffer fits the loaded font.
 */
void JUTFontSetup(u16 font) {
#if VERSION >= VERSION_US
    DVDEntry* dvd;
    u32 size;
#else
    u32 size;
    DVDEntry* dvd;
#endif
    BinaryFontBlock* ptr;
    char path[128];
    u32 i;
#if VERSION >= VERSION_US
    s32 maxsize;
    s32 j;
    DVDEntry* other;
    s32 len;
#endif

    if (!pfh) {
#if VERSION <= VERSION_DEMO_JP
        sprintf(path, "%s/font/%s.bfn", getMarioStDvdRoot(), fontname[font]);
#else
        sprintf(path, "%s/f/%s.bfn", getMarioStDvdRoot(), fontname[font]);
#endif
        dvd = DVDMgrOpen(path, 2, 0);
        size = DVDMgrGetLength(dvd);
#if VERSION >= VERSION_US
        maxsize = size;
        for (j = 0; j < (s32)(sizeof(fontname) / sizeof(fontname[0])); j++) {
            // BUG (US and later): opens <root>/font/ while the fonts are loaded from <root>/f/ above, so no file
            // opens and maxsize stays the size of the loaded font.
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

#if VERSION < VERSION_US
void JUTFont_Free(void) {
    if (pfh) {
        __memFree(HEAP_DEFAULT, pfh);
        pfh = NULL;
    }
}
#endif

/**
 * Maps a character code (one byte, or a Shift-JIS pair read as u16) to a glyph index through the font's MAP1
 * blocks (linear, Shift-JIS formula, table or binary-searched pairs);
 * unmapped codes give the font's replacement glyph.
 */
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
                            s32 mid = (min + max) / 2;
                            if ((u16)code == map->table[mid * 2]) {
                                glyph = map->table[2 * mid + 1];
                                break;
                            }
                            if ((u16)code < map->table[2 * mid]) {
                                max = mid - 1;
                            } else {
                                min = mid + 1;
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

/**
 * Sets up GX for textured glyph quads: alpha blending, no depth test or fog,
 * direct s16 positions with colour and two texture coordinates, the glyph sheet texture and its texel scale,
 * colour in TEV register 0.
 */
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
#if VERSION >= VERSION_US
    if (gp->language == LANGUAGE_JP) {
        PSMTXScale(mtx, 0.001953125f, 0.0009765625f, 1.0f);
    } else {
        PSMTXScale(mtx, 0.001953125f, 0.00390625f, 1.0f);
    }
#else
    PSMTXScale(mtx, 0.001953125f, 0.0009765625f, 1.0f);
#endif
    GXLoadTexMtxImm(mtx, GX_TEXMTX0, GX_MTX2x4);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    GXLoadPosMtxImm(camera->view, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, (GXColor){0, 0, 0, 0});
}

// FAKE: fontHue/fontHueX/fontPos are invented inline helpers. Their params become @ temps created at the vertex 3/4
// call sites (hr1 = vertex 3 hue, hr0 = vertex 4 hue, x1s = vertex 3 x), which is the register numbering the target needs.
static inline void fontHue(GXColor* hsv, s32 hr) {
    hsv->r = hr;
}

/** Sets the vertex hue from its x and the frame hue (see fontHue). */
static inline void fontHueX(GXColor* hsv, const s16 x, u8 hue) {
    fontHue(hsv, x + hue);
}

/** Emits a vertex position and sets the hue for that vertex's colour. */
static inline void fontPos(const s16 x, s16 y, GXColor* hsv, u8 hue) {
    GXPosition3s16(x, y, 0);
    fontHueX(hsv, x, hue);
}

/**
 * Emits one glyph as a quad at x, y in the current matrix:
 * texture coordinates of its cell in the glyph sheet,
 * a rainbow vertex colour whose hue moves with x and the frame count,
 * and random noise texture coordinates (used only when FontDrawRainbowColor or FontDrawNoise add their stage).
 * US and later trim 2 px off the cell.
 */
static void _JUTFont_DrawPos(u16 code, s16 x, s16 y) {
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
#if VERSION >= VERSION_US
    h = (cellh - 2.0f) * scale;
#else
    h = cellh * scale;
#endif

    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    n0 = irand(64);
    n1 = irand(64);
    x0 = x;
#if VERSION >= VERSION_US
    y0 = y - 2;
#else
    y0 = y - 1;
#endif
    x1 = x0 + w;
    hue = x0 - (u8)gp->retraceCount * 2;
    hsv = (GXColor){0x00, 0xAA, 0xFF, 0xFF};
    tv = row * cellh;
#if VERSION >= VERSION_US
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
#if VERSION >= VERSION_US
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

