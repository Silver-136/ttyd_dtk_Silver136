#include "romfont.h"

typedef struct RomFontWorkData {
    void* glyphs;
    s32 uniqueChars;
    s32 language;
} RomFontWorkData;

static RomFontWorkData work;
static void* wp = &work;

char* msg_jp[21] = {
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
    0,
};

char* msg_us[21] = {
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
    0,
};

char* msg_lang3[21] = {
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
    0,
};

char* msg_lang4[21] = {
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
    0,
};

char* msg_lang5[21] = {
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
    0,
};

char* msg_lang6[21] = {
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
    0,
};

static char** msg_tbl[6] = {
    msg_jp,
    msg_us,
    msg_lang3,
    msg_lang4,
    msg_lang5,
    msg_lang6,
};

void romFontMake(void);

char* romFontGetMessage(s32 messageId) {
    extern void* wp;
    extern void* gp;

    if (*(s32*)((s32)wp + 8) != 0 && *(u32*)((s32)gp + 0x16C) == 0) {
        return msg_tbl[1][messageId];
    }
    return msg_tbl[*(s32*)((s32)gp + 0x16C)][messageId];
}

void romFontInit(void) {
    extern void* wp;
    extern u16 OSGetFontEncode(void);

    *(s32*)((s32)wp + 0) = 0;
    *(s32*)((s32)wp + 4) = 0;
    if (OSGetFontEncode() == 1) {
        *(s32*)((s32)wp + 8) = 0;
    } else {
        *(s32*)((s32)wp + 8) = 1;
    }
    romFontMake();
}

/* stub-fill: romFontMake | prototype_only | source_prototype */
void romFontMake(void) {
    extern void* wp;
    extern void* __memAlloc(s32 heap, u32 size);
    extern void __memFree(s32 heap, void* memory);
    extern u32 OSGetFontEncode(void);
    extern s32 OSLoadFont(void* fontData, void* textureData);
    extern void* memset(void* dst, s32 value, u32 size);
    extern s32 OSGetFontTexel(const char* text, void* image,
                              s32 position, u32 stride, u32* width);
    extern void DCFlushRange(void* memory, u32 size);

    u16* characters = __memAlloc(0, 0x2000);
    s32 characterCount = 0;
    s32 tableIndex;
    s32 messageIndex;

    for (tableIndex = 0; tableIndex < 6; tableIndex++) {
        for (messageIndex = 0; msg_tbl[tableIndex][messageIndex] != NULL; messageIndex++) {
            char* message = msg_tbl[tableIndex][messageIndex];
            s32 offset = 0;

            if (*(s32*)((s32)wp + 8) != 0 && tableIndex == 0) {
                continue;
            }

            for (;;) {
                u8 byte = *(u8*)(message + offset);

                if (*(s32*)((s32)wp + 8) != 0) {
                    if (byte == 0) {
                        break;
                    }
                    characters[characterCount++] = byte;
                    offset++;
                } else if (byte >= 0x20 && byte < 0x80) {
                    characters[characterCount++] = byte;
                    offset++;
                } else if ((byte >= 0x80 && byte < 0xA0) || byte >= 0xE0) {
                    characters[characterCount++] = *(u16*)(message + offset);
                    offset += 2;
                } else {
                    if (byte == 0) {
                        break;
                    }
                    offset++;
                }
            }
        }
    }

    {
        s32 i;
        for (i = 0; i < characterCount - 1; i++) {
            s32 j;
            for (j = i; j < characterCount; j++) {
                if (characters[j] < characters[i]) {
                    u16 value = characters[i];
                    characters[i] = characters[j];
                    characters[j] = value;
                }
            }
        }
    }

    *(s32*)((s32)wp + 4) = 0;
    {
        u16* uniqueCharacters = __memAlloc(0, 0x2000);
        u16 previous = 0xFFFF;
        s32 remaining = characterCount;
        s32 i = 0;
        void* fontData;
        void* textureData;
        u8* glyphData;

        while (remaining > 0) {
            u16 character = characters[i++];
            if (character != previous) {
                s32 count = *(s32*)((s32)wp + 4);
                *(s32*)((s32)wp + 4) = count + 1;
                uniqueCharacters[count] = character;
                previous = character;
            }
            remaining--;
        }

        if ((OSGetFontEncode() & 0xFFFF) == 1) {
            fontData = __memAlloc(0, 0x90EE4);
            textureData = __memAlloc(0, 0x4D000);
        } else {
            fontData = __memAlloc(0, 0x10120);
            textureData = __memAlloc(0, 0x3000);
        }
        OSLoadFont(fontData, textureData);

        glyphData = __memAlloc(0, *(s32*)((s32)wp + 4) * 0x140);
        *(void**)wp = glyphData;
        memset(glyphData, 0, *(s32*)((s32)wp + 4) * 0x140);

        {
            u16* currentCharacter = uniqueCharacters;
            u8* glyph = glyphData;
        for (i = 0; i < *(s32*)((s32)wp + 4); i++) {
            u8 text[3];
            u32 width;
            u16 character = *currentCharacter;

            if (character < 0x100) {
                text[0] = (u8)character;
                text[1] = 0;
            } else {
                *(u16*)text = character;
                text[2] = 0;
            }
            OSGetFontTexel((const char*)text, glyph, 0, 6, &width);
            *(u16*)(glyph + 0x120) = character;
            *(s16*)(glyph + 0x122) = (s16)width;
            currentCharacter++;
            glyph += 0x140;
        }
        }

        DCFlushRange(glyphData, *(s32*)((s32)wp + 4) * 0x140);
        __memFree(0, textureData);
        __memFree(0, fontData);
        __memFree(0, uniqueCharacters);
    }
    __memFree(0, characters);
}

/* stub-fill: romFontPrintGX | missing_definition | ghidra_signature */
void romFontPrintGX(f32 x, f32 y, f32 scale, u32* color, const char* format, ...) {
    typedef struct {
        u32 regInfo;
        void* inputArgArea;
        void* regSaveArea;
    } RomFontVaList;

    extern void* wp;
    extern f64 tan(f64 value);
    extern s32 vsprintf(char* buffer, const char* format, void* args);
    extern void C_MTXLookAt(f32 matrix[3][4], const f32* camera,
                            const f32* up, const f32* target);
    extern void GXSetCullMode(s32 mode);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32 attr, s32 type);
    extern void GXSetVtxAttrFmt(s32 vtxfmt, s32 attr, s32 type, s32 frac, s32 shift);
    extern void GXSetBlendMode(s32 type, s32 src, s32 dst, s32 op);
    extern void GXSetZCompLoc(s32 beforeTexture);
    extern void GXSetAlphaCompare(s32 comp0, s32 ref0, s32 op, s32 comp1, s32 ref1);
    extern void GXSetZMode(s32 enable, s32 func, s32 update);
    extern void GXSetNumChans(s32 count);
    extern void GXSetChanCtrl(s32 channel, s32 enable, s32 ambSrc,
                              s32 matSrc, s32 lights, s32 diffFn, s32 attnFn);
    extern void GXSetChanMatColor(s32 channel, const u32* color);
    extern void GXSetNumTevStages(s32 count);
    extern void GXSetTevOrder(s32 stage, s32 coord, s32 map, s32 color);
    extern void GXSetTevOp(s32 stage, s32 mode);
    extern void GXSetNumTexGens(s32 count);
    extern void GXSetTexCoordGen2(s32 coord, s32 func, s32 source,
                                  s32 matrix, s32 normalize, s32 postMatrix);
    extern void PSMTXTrans(f32 matrix[3][4], f32 x, f32 y, f32 z);
    extern void PSMTXScale(f32 matrix[3][4], f32 x, f32 y, f32 z);
    extern void PSMTXConcat(f32 a[3][4], f32 b[3][4], f32 out[3][4]);
    extern void GXLoadPosMtxImm(f32 matrix[3][4], s32 id);
    extern void GXSetCurrentMtx(s32 id);
    extern void GXInitTexObj(void* object, void* image, u16 width, u16 height,
                             s32 format, s32 wrapS, s32 wrapT, s32 mipmap);
    extern void GXInitTexObjLOD(void* object, s32 minFilter, s32 magFilter,
                                f32 minLod, f32 maxLod, f32 lodBias,
                                s32 biasClamp, s32 edgeLod, s32 anisotropy);
    extern void GXLoadTexObj(void* object, s32 mapId);
    extern void GXBegin(s32 primitive, s32 vtxfmt, s32 count);
    extern u32 vec3_803025d8[];
    extern u32 vec3_803025e4[];
    extern u32 vec3_803025f0[];

    char text[1024];
    f32 translation[3][4];
    f32 scaling[3][4];
    f32 lookAt[3][4];
    u8 textureObject[32];
    f32 target[3];
    f32 up[3];
    f32 camera[3];
    RomFontVaList args;
    u32 materialColor;
    volatile u16* fifo = (volatile u16*)0xCC008000;
    s16 penX = 0;
    s16 penY = 0;
    s32 index = 0;

    up[0] = *(f32*)&vec3_803025d8[0];
    up[1] = *(f32*)&vec3_803025d8[1];
    up[2] = *(f32*)&vec3_803025d8[2];
    target[0] = *(f32*)&vec3_803025d8[3];
    target[1] = *(f32*)&vec3_803025d8[4];
    target[2] = *(f32*)&vec3_803025d8[5];
    camera[0] = *(f32*)&vec3_803025d8[6];
    camera[1] = *(f32*)&vec3_803025d8[7];
    camera[2] = 240.0f / (f32)tan(0.2181661564992912);

    args.regInfo = 0x02030000;
    args.inputArgArea = (u8*)&args + 0x30;
    args.regSaveArea = (u8*)&args - 0x60;
    vsprintf(text, format, &args);

    C_MTXLookAt(lookAt, camera, target, up);
    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(13, 1);
    GXSetVtxAttrFmt(0, 9, 1, 3, 0);
    GXSetVtxAttrFmt(0, 13, 1, 3, 0);
    GXSetBlendMode(1, 4, 5, 0);
    GXSetZCompLoc(1);
    GXSetAlphaCompare(7, 0, 0, 7, 0);
    GXSetZMode(0, 3, 0);
    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2);
    materialColor = *color;
    GXSetChanMatColor(4, &materialColor);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 4);
    GXSetTevOp(0, 0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    PSMTXTrans(translation, x, y, 0.0f);
    PSMTXScale(scaling, scale, scale, scale);
    PSMTXConcat(translation, scaling, translation);
    PSMTXConcat(lookAt, translation, translation);
    GXLoadPosMtxImm(translation, 0);
    GXSetCurrentMtx(0);

    for (;;) {
        u8 byte = (u8)text[index];
        u16 wide = *(u16*)&text[index];
        u8* glyph;

        if (*(s32*)((s32)wp + 8) != 0) {
            if (byte == '\n') {
                penX = 0;
                penY -= 0x18;
                index++;
                continue;
            }
            if (byte == 0) {
                return;
            }
            {
                u8* first = *(u8**)wp;
                s32 count = *(s32*)((s32)wp + 4);
                glyph = first;
                if (count > 0) {
                    do {
                        if (*(u16*)(glyph + 0x120) == (u16)byte) {
                            goto glyph_ready;
                        }
                        glyph += 0x140;
                    } while (--count != 0);
                }
                glyph = first;
            }
            index++;
        } else if (byte >= 0x20 && byte < 0x80) {
            u8* first = *(u8**)wp;
            s32 count = *(s32*)((s32)wp + 4);
            glyph = first;
            if (count > 0) {
                do {
                    if (*(u16*)(glyph + 0x120) == (u16)byte) {
                        goto glyph_ready;
                    }
                    glyph += 0x140;
                } while (--count != 0);
            }
            glyph = first;
            index++;
        } else if ((byte >= 0x80 && byte < 0xA0) || byte >= 0xE0) {
            u8* first = *(u8**)wp;
            s32 count = *(s32*)((s32)wp + 4);
            glyph = first;
            if (count > 0) {
                do {
                    if (*(u16*)(glyph + 0x120) == wide) {
                        goto glyph_ready;
                    }
                    glyph += 0x140;
                } while (--count != 0);
            }
            glyph = first;
            index += 2;
        } else {
            if (byte == 0) {
                return;
            }
            if (byte == '\n') {
                penX = 0;
                penY -= 0x18;
                index++;
                continue;
            } else {
                glyph = *(u8**)wp;
                index++;
            }
        }

glyph_ready:
        {
            /* Keep the renderer body after the encoding-specific lookup. */
        GXInitTexObj(textureObject, glyph, 0x18, 0x18, 0, 0, 0, 0);
        GXInitTexObjLOD(textureObject, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
        GXLoadTexObj(textureObject, 0);
        GXBegin(0x80, 0, 4);

        *fifo = (u16)penX;
        *fifo = (u16)(penY + 0x18);
        *fifo = 0;
        *fifo = 0;
        *fifo = 0;

        *fifo = (u16)(penX + 0x18);
        *fifo = (u16)(penY + 0x18);
        *fifo = 0;
        *fifo = 1;
        *fifo = 0;

        *fifo = (u16)(penX + 0x18);
        *fifo = (u16)penY;
        *fifo = 0;
        *fifo = 1;
        *fifo = 1;

        *fifo = (u16)penX;
        *fifo = (u16)penY;
        *fifo = 0;
        *fifo = 0;
        *fifo = 1;

        penX += *(s16*)(glyph + 0x122);
        }
    }
}


/* CHATGPT STUB FILL: main/romfont 20260624_184929 */

/* stub-fill: romFontGetWidth | missing_definition | ghidra_signature */
int romFontGetWidth(s32 message, s32 entry) {
    extern void* wp;
    u8* font = wp;
    s32 width = 0;
    s32 index = 0;
    s32 maximum = 0;

    for (;;) {
        u8 character = *(u8*)(message + index);
        u16 wideCharacter = *(u16*)(message + index);

        if (*(s32*)(font + 8) != 0) {
            if (character == 0) {
                if (maximum < width) {
                    maximum = width;
                }
                break;
            }
            if (character == '\n') {
                if (maximum < width) {
                    maximum = width;
                }
                width = 0;
                index++;
            } else {
                u8* first = *(u8**)font;
                u8* current = first;
                s32 count = *(s32*)(font + 4);

                while (count-- > 0) {
                    if (*(u16*)(current + 0x120) == (u16)character) {
                        goto narrow_found;
                    }
                    current += 0x140;
                }
                current = first;
narrow_found:
                entry = (s32)current;
                index++;
            }
        } else if (character >= 0x20 && character < 0x80) {
            u8* first = *(u8**)font;
            u8* current = first;
            s32 count = *(s32*)(font + 4);

            while (count-- > 0) {
                if (*(u16*)(current + 0x120) == (u16)character) {
                    goto ascii_found;
                }
                current += 0x140;
            }
            current = first;
ascii_found:
            entry = (s32)current;
            index++;
        } else if ((character >= 0x80 && character < 0xA0) || character >= 0xE0) {
            u8* first = *(u8**)font;
            u8* current = first;
            s32 count = *(s32*)(font + 4);

            while (count-- > 0) {
                if (*(u16*)(current + 0x120) == wideCharacter) {
                    goto wide_found;
                }
                current += 0x140;
            }
            current = first;
wide_found:
            entry = (s32)current;
            index += 2;
        } else {
            if (character == 0) {
                if (maximum < width) {
                    maximum = width;
                }
                break;
            }
            if (character == '\n') {
                if (maximum < width) {
                    maximum = width;
                }
                width = 0;
                index++;
            } else {
                index++;
            }
        }
        width += *(u16*)(entry + 0x122);
    }
    return maximum;
}

