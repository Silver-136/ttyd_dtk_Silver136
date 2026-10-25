#include "romfont.h"
#include "dolphin/mtx.h"
#include "dolphin/types.h"
#include "memory.h"
#ifdef VERSION_G8ME01
#include "mario/mariost.h"
#endif
#include <dolphin/os.h>

//.sbss
static RomFontWork work;

//.sdata
static RomFontWork* wp = &work;

//.rodata string, .sdata table
#ifdef VERSION_G8ME01
const char* msg_jp[21] = {
    "\203v\203\215\203O\203\214\203b\203V\203u\203\202\201[\203h\202\305\225\\\216\246\202\265\202\334\202\267\202\251\201H",
    "\202\315\201@\202\242\201@\201@\201@\201@\201@\201@\202\242\202\242\202\246",
    "\201y\201@\201@\201@\201@\201z",
    "\211\346\226\312\225\\\216\246\203\202\201[\203h\202\315\203v\203\215\203O\203\214\203b\203V\203u\203\202\201[\203h\202\311\203Z\203b\203g\202\263\202\352\202\334\202\265\202\275\201B",
    "\211\346\226\312\225\\\216\246\203\202\201[\203h\202\315\203C\203\223\203^\201[\203\214\201[\203X\203\202\201[\203h\202\311\203Z\203b\203g\202\263\202\352\202\334\202\265\202\275\201B",
    "",
    "",
    "",
    "",
    "",
    "\203f\203B\203X\203N\203J\203o\201[\202\252\212J\202\242\202\304\202\242\202\334\202\267\201B\n\n\203Q\201[\203\200\202\360\221\261\202\257\202\351\217\352\215\207\202\315\201A\n\203f\203B\203X\203N\203J\203o\201[\202\360\225\302\202\337\202\304\202\255\202\276\202\263\202\242\201B",
    "\203y\201[\203p\201[\203}\203\212\203I\202q\202o\202f\202\314\n\203f\203B\203X\203N\202\360\203Z\203b\203g\202\265\202\304\202\255\202\276\202\263\202\242\201B",
    "\203f\203B\203X\203N\202\360\223\307\202\337\202\334\202\271\202\361\202\305\202\265\202\275\201B\n\n\202\255\202\355\202\265\202\255\202\315\201A\226{\221\314\202\314\216\346\210\265\220\340\226\276\217\221\202\360\n\202\250\223\307\202\335\202\255\202\276\202\263\202\242\201B",
    "\203G\203\211\201[\202\252\224\255\220\266\202\265\202\334\202\265\202\275\201B\n\n\226{\221\314\202\314\203p\203\217\201[\203{\203^\203\223\202\360\211\237\202\265\202\304\223d\214\271\202\360OFF\202\311\202\265\201A\n\226{\221\314\202\314\216\346\210\265\220\340\226\276\217\221\202\314\216w\216\246\202\311\217]\202\301\202\304\202\255\202\276\202\263\202\242\201B",
    "English",
    "Deutsch",
    "Fran\347ais",
    "Espa\361ol",
    "Italiano",
    "[           ]",
    NULL,
};

const char* msg_us[21] = {
    "Would you like to display\nin Progressive Mode?",
    "Yes              N o",
    "[    ]",
    "Progressive Mode\nhas been turned on.",
    "Progressive Mode\nhas been turned off.",
    "This game is compatible with 60Hz mode.\nPlease select mode.\n\nIf your television does not display\ncorrectly in 60Hz mode,\nplease select 50Hz mode.",
    "60Hz mode      50Hz mode",
    "[             ]",
    "Screen display has been set to\n60Hz mode.",
    "Screen display has been set to\n50Hz mode.",
    "The Disc Cover is open.\nIf you want to continue the game,\nplease close the Disc Cover.",
    "Please insert\nthe \"Paper Mario: The\nThousand-Year Door\"\nGame Disc.",
    "The Game Disc could not be read.\nPlease read the Nintendo GameCube\nInstruction Booklet\nfor more information.",
    "An error has occurred.\nTurn the power off and refer to\nthe Nintendo GameCube\nInstruction Booklet\nfor further instructions.",
    "English",
    "Deutsch",
    "Fran\347ais",
    "Espa\361ol",
    "Italiano",
    "[           ]",
    NULL,
};

const char* msg_ge[21] = {
    "",
    "",
    "",
    "",
    "",
    "Dieses Spiel unterst\374tzt den 60Hz-Modus.\nBitte Modus auswahlen.\n\nSollte das TV-Gerat im 60Hz-Modus kein\neinwandfreies Bild darstellen,\nbitte den 50Hz-Modus auswahlen.",
    "60Hz-Modus     50Hz-Modus",
    "[               ]",
    "Die Bildschirmdarstellung erfolgt im\n60Hz-Modus.",
    "Die Bildschirmdarstellung erfolgt im\n50Hz-Modus.",
    "Der Disc-Deckel ist ge\366ffnet.\nBitte den Disc-Deckel schlie\337en,\num mit dem Spiel fortzufahren.",
    "Bitte legen Sie die \"Paper Mario\" Disc ein.",
    "Diese Game Disc kann\nnicht gelesen werden.\nBitte lesen Sie die\nBedienungsanleitung des\nNintendo GameCube, um weitere\nInformationen zu erhalten.",
    "Ein Fehler ist aufgetreten.\nBitte schalten Sie den\nNintendo GameCube aus und lesen\nSie die Bedienungsanleitung,\num weitere Informationen zu erhalten.",
    "English",
    "Deutsch",
    "Fran\347ais",
    "Espa\361ol",
    "Italiano",
    "[           ]",
    NULL,
};

const char* msg_fr[21] = {
    "",
    "",
    "",
    "",
    "",
    "Ce jeu est compatible\navec le mode 60 Hz.\nVeuillez choisir un mode.\n\nSi votre televiseur n'affiche pas\ncorrectement le mode 60 Hz,\nchoisissez le mode 50 Hz.",
    "Mode 60 Hz     Mode 50 Hz",
    "[               ]",
    "L'affichage est en mode 60 Hz.",
    "L'affichage est en mode 50 Hz.",
    "Le couvercle est ouvert.\nPour continuer \340 jouer,\nveuillez fermer le couvercle.",
    "Veuillez ins\351rer le disque \"Paper Mario\".",
    "La lecture du disque a echoue.\nVeuillez vous r\351f\351rer\nau manuel d'instructions\nNintendo GameCube pour\nde plus amples informations.",
    "Une erreur est survenue.\nEteignez la console et r\351f\351rez-vous\nau manuel d'instructions\nNintendo GameCube pour\nde plus amples informations.",
    "English",
    "Deutsch",
    "Fran\347ais",
    "Espa\361ol",
    "Italiano",
    "[           ]",
    NULL,
};

const char* msg_sp[21] = {
    "",
    "",
    "",
    "",
    "",
    "Este juego es compatible\ncon el modo 60 Hz.\n\277Quieres verlo en ese modo?\n\nSi tu televisor no reproduce\ncorrectamente el modo 60 Hz,\nselecciona el modo 50 Hz.",
    "Modo 60 Hz     Modo 50 Hz",
    "[               ]",
    "La pantalla se ha configurado en\nmodo 60 Hz.",
    "La pantalla se ha configurado en\nmodo 50 Hz.",
    "La tapa est\341 abierta.\nSi quieres seguir jugando,\ndebes cerrar la tapa.",
    "Coloca el disco de \"Paper Mario\".",
    "No se puede leer el disco.\nConsulta el manual\nde instrucciones de\nNintendo GameCube para\nobtener mas informacion.",
    "Se ha producido un error.\nApaga la consola y consulta\nel manual de instrucciones de\nNintendo GameCube para\nobtener m\341s informaci\363n.",
    "English",
    "Deutsch",
    "Fran\347ais",
    "Espa\361ol",
    "Italiano",
    "[           ]",
    NULL,
};

const char* msg_it[21] = {
    "",
    "",
    "",
    "",
    "",
    "Questo gioco pu\362 essere\nvisualizzato in modalit\340 60Hz.\nSeleziona la modalit\340.\n\nSe il televisore non dovesse visualizzare\ncorrettamente la modalit\340 60Hz,\nseleziona la modalit\340 50Hz.",
    "Modalit\340 60Hz  Modalit\340 50Hz",
    "[                ]",
    "Visualizzazione impostata a 60Hz.",
    "Visualizzazione impostata a 50Hz.",
    "Il coperchio del disco \350 aperto.\nSe vuoi proseguire nel gioco,\nchiudi il coperchio del disco.",
    "Inserisci il disco di gioco \"Paper Mario\".",
    "Impossibile leggere il disco di gioco.\nPer ulteriori indicazioni consulta\nil manuale di istruzioni del\nNintendo GameCube.",
    "Si \350 verificato un errore.\nSpegni e consulta il manuale\ndi istruzioni del\nNintendo GameCube per\nulteriori indicazioni.",
    "English",
    "Deutsch",
    "Fran\347ais",
    "Espa\361ol",
    "Italiano",
    "[           ]",
    NULL,
};

static const char** msg_tbl[6] = { msg_jp, msg_us, msg_ge, msg_fr, msg_sp, msg_it };
#else
const char* msg_jp[10] = {
    "プログレッシブモードで表示しますか？",
    "は　い　　　　　　いいえ",
    "【　　　　】",
    "画面表示モードはプログレッシブモードにセットされました。",
    "画面表示モードはインターレースモードにセットされました。",
    "ディスクカバーが開いています。\n\nゲームを続ける場合は、\nディスクカバーを閉めてください。",
    "ペーパーマリオＲＰＧの\nディスクをセットしてください。",
    "ディスクを読めませんでした。\n\nくわしくは、本体の取扱説明書を\nお読みください。",
    "エラーが発生しました。\n\n本体のパワーボタンを押して電源をOFFにし、\n本体の取扱説明書の指示に従ってください。",
    NULL
};

const char* msg_us[9] = {
    "Would you like to display in progressive scan mode?",
    "Yes              N o",
    "[    ]",
    "Progressive scan mode has been turned on.",
    "Progressive scan mode has been turned off.",
    "The Disc Cover is open.\nIf you want to continue the game,\nplease close the Disc Cover.",
    "Please insert\nthe \"Paper Mario 2\" Game Disc.",
    "The Game Disc could not be read.\nPlease read the\nNintendo GameCube Instruction Booklet\nfor more information.",
    NULL
};

static const char** msg_tbl[2] = { msg_jp, msg_us };
#endif

// local prototypes
void romFontMake(void);

const char* romFontGetMessage(s32 msg) {
#ifdef VERSION_G8ME01
    if (wp->language != 0 && (u32)gp->language == 0) {
        return msg_tbl[1][msg];
    }
    return msg_tbl[gp->language][msg];
#else
    return msg_tbl[0][msg]; // has eng but always returns JP
#endif
}

void romFontInit(void) {
    wp->glyphs = 0;
    wp->uniqueChars = 0;
    if (OSGetFontEncode() == OS_FONT_ENCODE_SJIS) {
        wp->language = 0;
    } else {
        wp->language = 1;
    }
    romFontMake();
}

void romFontMake(void) {
    char c[16];
    s32 width;
    OSFontHeader* fontData;
    void* fontTmp;
    RomFontChar* glyph;
    u16* chars;
    u16* uniqueChars;
    s32 numChars;
    s32 i;
    const char* str;
    u16 prev;
    s32 t;

#ifdef VERSION_G8ME01
    chars = __memAlloc(HEAP_DEFAULT, 0x2000);
#else
    chars = __memAlloc(HEAP_DEFAULT, 0x800);
#endif
    numChars = 0;

    /* 1. Collect every printable code from the system message strings. */
#ifdef VERSION_G8ME01
    for (t = 0; t < 6; t++) {
#else
    for (t = 0; t < 2; t++) {
#endif
        for (i = 0; (str = msg_tbl[t][i]) != NULL; i++) {
            /* ASCII mode (language != 0): only scan the English table (t != 0). */
            s32 k = 0;
            if (wp->language == 0 || t != 0) {
                for (k;;) {
                    u32 b = str[k];
                    u16 w = *(u16*)(str + k);
#ifdef VERSION_G8ME01
                    if (wp->language != 0) {
                        if (b == 0) {
                            break;
                        }
                        chars[numChars++] = b;
                        k++;
                    } else
#endif
                    if (b >= 0x20 && b < 0x80) {
                        chars[numChars++] = b;
                        k++;
                    } else if ((b >= 0x80 && b < 0xA0) || b >= 0xE0) {
                        chars[numChars++] = w;
                        k += 2;
                    } else if (b != 0) {
                        k++;
                    } else {
                        break;
                    }
                }
            }
        }
    }

    /* 2. Selection sort ascending. */
    {
        s32 j = 0;
        s32 i = 0;
        for (i = 0; i < numChars - 1; i++) {
            for (j = i; j < numChars; j++) {
                if (chars[i] > chars[j]) {
                    u16 tmp = chars[i];
                    chars[i] = chars[j];
                    chars[j] = tmp;
                }
            }
        }
    }

    /* 3. Deduplicate the sorted list; wp->unk4 = unique count. */
    wp->uniqueChars = 0;
#ifdef VERSION_G8ME01
    uniqueChars = __memAlloc(HEAP_DEFAULT, 0x2000);
#else
    uniqueChars = __memAlloc(HEAP_DEFAULT, 0x800);
#endif
    prev = 0xFFFF;
    for (i = 0; i < numChars; i++) {
        if (chars[i] != prev) {
            prev = chars[i];
            uniqueChars[wp->uniqueChars++] = chars[i];
        }
    }

    /* 4. Expand the ROM font into RAM. */
    if (OSGetFontEncode() == OS_FONT_ENCODE_SJIS) {
        fontData = __memAlloc(HEAP_DEFAULT, 0x90EE4);              // OS_FONT_SIZE_SJIS (this SDK)
        fontTmp = __memAlloc(HEAP_DEFAULT, OS_FONT_ROM_SIZE_SJIS); // 0x4D000
    } else {
        fontData = __memAlloc(HEAP_DEFAULT, 0x10120);              // OS_FONT_SIZE_ANSI (this SDK)
        fontTmp = __memAlloc(HEAP_DEFAULT, OS_FONT_ROM_SIZE_ANSI); // 0x3000
    }
    OSLoadFont(fontData, fontTmp);

    /* 5. Bake each unique glyph into the cache. */
    wp->glyphs = __memAlloc(HEAP_DEFAULT, wp->uniqueChars * sizeof(RomFontChar));
    memset(wp->glyphs, 0, wp->uniqueChars * sizeof(RomFontChar));
    glyph = wp->glyphs;
    for (i = 0; i < (s32)wp->uniqueChars; i++) {
        if (uniqueChars[i] < 0x100) {
            c[0] = (s8)uniqueChars[i];
            c[1] = '\0';
        } else {
            *(u16*)c = uniqueChars[i];
            c[2] = '\0';
        }
        OSGetFontTexel(c, glyph, 0, 6, &width);
        glyph->code = uniqueChars[i];
        glyph->width = (s16)width;
        glyph++;
    }

    DCFlushRange(wp->glyphs, wp->uniqueChars * sizeof(RomFontChar));

    __memFree(HEAP_DEFAULT, fontTmp);
    __memFree(HEAP_DEFAULT, fontData);
    __memFree(HEAP_DEFAULT, uniqueChars);
    __memFree(HEAP_DEFAULT, chars);
}

double tan(double);

static inline RomFontChar* romFontFindChar(u16 code) {
    RomFontWork* w = wp;
    RomFontChar* table = w->glyphs;
    RomFontChar* cur = w->glyphs;
    s32 n;
    for (n = 0; n < w->uniqueChars; cur++, n++) {
        if (cur->code == code)
            return cur;
    }
    return table; // not found
}

void romFontPrintGX(f32 x, f32 y, f32 scale, GXColor* color, const char* msg, ...) {
    char buf[1024];  // 0x158
    Mtx mtx;         // 0x128
    Mtx scaleMtx;    // 0x0F8
    Mtx camMtx;      // 0x0C8
    GXTexObj texObj; // 0x0A8
    Point3d target;  // 0x09C
    Point3d up;      // 0x090
    Point3d camPos;  // 0x084
    va_list ap;
    s32 i = 0;
    s32 curX = 0;
    s16 curY = 0;
    RomFontChar* glyph;

    // --- camera: place the eye so 1 unit == 1 px at the target plane ---
    target = (Vec){ 0.0f, 0.0f, 0.0f };
    up = (Vec){ 0.0f, 1.0f, 0.0f };
    camPos = (Vec){ 0.0f, 0.0f, 240.0f / (f32)tan(0.21816612780094147) };

    // --- format the message (this is where the va_list lives) ---
    va_start(ap, msg);
    vsprintf(buf, msg, ap); // no va_end in the original

    C_MTXLookAt(camMtx, &camPos, &up, &target);

    // --- render state: alpha-blended, no Z, colored I4 glyphs ---
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_S16, 0);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetZCompLoc(GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetZMode(GX_DISABLE, GX_LEQUAL, GX_DISABLE);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE, GX_AF_NONE);
    GXSetChanMatColor(GX_COLOR0A0, *color);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);

    // --- model matrix = cam * translate(x,y,0) * scale(s,s,s) ---
    PSMTXTrans(mtx, x, y, 0.0f);
    PSMTXScale(scaleMtx, scale, scale, scale);
    PSMTXConcat(mtx, scaleMtx, mtx); // mtx = T * S
    PSMTXConcat(camMtx, mtx, mtx);   // mtx = cam * (T * S)
    GXLoadPosMtxImm(mtx, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);

    // --- walk the formatted string, one 24x24 quad per glyph ---
    for (i;;) {
        u32 c = buf[i];
        u16 c16 = *(u16*)&buf[i]; // big-endian lead<<8 | trail, for double-byte

#ifdef VERSION_G8ME01
        if (wp->language != 0) {
            if (c == 0x0A) {
                curX = 0;
                curY -= 24;
                i += 1;
                continue;
            } else if (c == 0x00) {
                break;
            } else {
                glyph = romFontFindChar(c & 0xFFFF);
                i += 1;
            }
        } else
#endif
        if (c >= 0x20 && c < 0x80) { // ASCII
            glyph = romFontFindChar(c & 0xFFFF);
            i += 1;
        } else if ((c >= 0x80 && c < 0xA0) || c >= 0xE0) { // SJIS lead byte
            glyph = romFontFindChar(c16);
            i += 2;
        } else if (c == 0x00) { // NUL -> done
            break;
        } else if (c == 0x0A) { // '\n'
            curX = 0;
            curY -= 24; // line height = cell height
            i += 1;
            continue; // no glyph drawn
        } else {      // ctrl / 0xA0-0xDF -> .notdef
            glyph = wp->glyphs;
            i += 1;
        }

        GXInitTexObj(&texObj, glyph->texel, 24, 24, GX_TF_I4, GX_CLAMP, GX_CLAMP, GX_FALSE);
        GXInitTexObjLOD(&texObj, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
        GXLoadTexObj(&texObj, GX_TEXMAP0);

        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        { s16 x2 = (s16)curX + 24; s16 y2 = (s16)curY + 24;
        GXPosition3s16(curX, y2, 0);
        GXTexCoord2s16(0, 0);
        GXPosition3s16(x2, y2, 0);
        GXTexCoord2s16(1, 0);
        GXPosition3s16(x2, curY, 0);
        GXTexCoord2s16(1, 1);
        GXPosition3s16(curX, curY, 0);
        GXTexCoord2s16(0, 1); }
        GXEnd();

        curX += glyph->width;
    }
}

s32 romFontGetWidth(const char* message) {
#ifdef VERSION_G8ME01
    s32 width = 0;
    s32 i = 0;
    s32 max = 0;
    u16 c16;
    RomFontChar* glyph;
    u32 c;
#else
    u16 c16;
    RomFontChar* glyph;
    u32 c;
    s32 width = 0;
    s32 i = 0;
    s32 max = 0;
#endif

    for (;;) {
        c = message[i];
        c16 = *(u16*)&message[i];
#ifdef VERSION_G8ME01
        if (wp->language != 0) {
            if (c == 0x00) {
                if (max < width) {
                    max = width;
                }
                break;
            } else if (c == 0x0A) {
                if (max < width) {
                    max = width;
                }
                width = 0;
                i += 1;
            } else {
                glyph = romFontFindChar(c & 0xFFFF);
                i += 1;
            }
        } else
#endif
        if (c >= 0x20 && c < 0x80) {
            glyph = romFontFindChar(c & 0xFFFF);
            i += 1;
        } else if ((c >= 0x80 && c < 0xA0) || c >= 0xE0) {
            glyph = romFontFindChar(c16);
            i += 2;
        } else if (c == 0x00) {
            if (max < width) {
                max = width;
            }
            break;
        } else if (c == 0x0A) {
            if (max < width) {
                max = width;
            }
            width = 0;
            i += 1;
        } else {
            i += 1;
        }
        width += glyph->width;
    }
    return max;
}
