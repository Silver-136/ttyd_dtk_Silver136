#include "romfont.h"

void romFontInit(void) {
    extern void* wp;
    extern u16 OSGetFontEncode(void);
    extern void romFontMake(void);

    *(s32*)((s32)wp + 0) = 0;
    *(s32*)((s32)wp + 4) = 0;
    if (OSGetFontEncode() == 1) {
        *(s32*)((s32)wp + 8) = 0;
    } else {
        *(s32*)((s32)wp + 8) = 1;
    }
    romFontMake();
}

char* romFontGetMessage(s32 messageId) {
    extern void* wp;
    extern void* gp;
    extern char** msg_tbl[6];

    if (*(s32*)((s32)wp + 8) != 0 && *(u32*)((s32)gp + 0x16C) == 0) {
        return msg_tbl[1][messageId];
    }
    return msg_tbl[*(s32*)((s32)gp + 0x16C)][messageId];
}


/* CHATGPT STUB FILL: main/romfont 20260624_184929 */

/* stub-fill: romFontGetWidth | missing_definition | ghidra_signature */
int romFontGetWidth(int param_1, int param_2) {
    extern void* wp;

    void* font;
    void* entry;
    s32 width;
    s32 maxWidth;
    s32 index;
    s32 count;
    u8 c;
    u16 code;

    width = 0;
    maxWidth = 0;
    index = 0;
    font = wp;

    while (1) {
        if (*(s32*)((s32)font + 8) != 0) {
            c = *(u8*)(param_1 + index);
            code = *(u16*)(param_1 + index);
            if (c == 0) {
                if (maxWidth < width) {
                    maxWidth = width;
                }
                break;
            }
            if (c == '\n') {
                if (maxWidth < width) {
                    maxWidth = width;
                }
                width = 0;
                index++;
                continue;
            }
            entry = *(void**)font;
            count = *(s32*)((s32)font + 4);
            while (count > 0) {
                if (*(u16*)((s32)entry + 0x120) == (u16)c) {
                    break;
                }
                entry = (void*)((s32)entry + 0x140);
                count--;
            }
            if (count <= 0) {
                entry = *(void**)font;
            }
            index++;
        } else {
            c = *(u8*)(param_1 + index);
            code = *(u16*)(param_1 + index);
            if (c >= 0x20 && c < 0x80) {
                entry = *(void**)font;
                count = *(s32*)((s32)font + 4);
                while (count > 0) {
                    if (*(u16*)((s32)entry + 0x120) == (u16)c) {
                        break;
                    }
                    entry = (void*)((s32)entry + 0x140);
                    count--;
                }
                if (count <= 0) {
                    entry = *(void**)font;
                }
                index++;
            } else if ((c >= 0x80 && c < 0xA0) || c >= 0xE0) {
                entry = *(void**)font;
                count = *(s32*)((s32)font + 4);
                while (count > 0) {
                    if (*(u16*)((s32)entry + 0x120) == code) {
                        break;
                    }
                    entry = (void*)((s32)entry + 0x140);
                    count--;
                }
                if (count <= 0) {
                    entry = *(void**)font;
                }
                index += 2;
            } else if (c == 0) {
                if (maxWidth < width) {
                    maxWidth = width;
                }
                break;
            } else if (c == '\n') {
                if (maxWidth < width) {
                    maxWidth = width;
                }
                width = 0;
                index++;
                continue;
            } else {
                index++;
                continue;
            }
        }
        width += *(u16*)((s32)entry + 0x122);
    }
    return maxWidth;
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

    RomFontVaList args;
    char text[1036];
    u8 textureObject[32];
    f32 lookAt[3][4];
    f32 translation[3][4];
    f32 scaling[3][4];
    f32 up[3];
    f32 target[3];
    f32 camera[3];
    volatile u16* fifo = (volatile u16*)0xCC008000;
    s16 penX = 0;
    s16 penY = 0;
    s32 index = 0;

    up[0] = *(f32*)&vec3_803025d8[0];
    up[1] = *(f32*)&vec3_803025d8[1];
    up[2] = *(f32*)&vec3_803025d8[2];
    target[0] = *(f32*)&vec3_803025e4[0];
    target[1] = *(f32*)&vec3_803025e4[1];
    target[2] = *(f32*)&vec3_803025e4[2];
    camera[0] = *(f32*)&vec3_803025f0[0];
    camera[1] = *(f32*)&vec3_803025f0[1];
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
    GXSetChanMatColor(4, color);
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
        u8* glyph;
        s32 count;
        u16 code;

        if (*(s32*)((s32)wp + 8) != 0) {
            if (byte == 0) {
                return;
            }
            if (byte == '\n') {
                penX = 0;
                penY -= 0x18;
                index++;
                continue;
            }
            code = byte;
            index++;
        } else {
            if (byte >= 0x20 && byte < 0x80) {
                code = byte;
                index++;
            } else if ((byte >= 0x80 && byte < 0xA0) || byte >= 0xE0) {
                code = *(u16*)&text[index];
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
                }
                code = byte;
                index++;
            }
        }

        glyph = *(u8**)wp;
        count = *(s32*)((s32)wp + 4);
        while (count > 0) {
            if (*(u16*)(glyph + 0x120) == code) {
                break;
            }
            glyph += 0x140;
            count--;
        }
        if (count <= 0) {
            glyph = *(u8**)wp;
        }

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

/* stub-fill: romFontMake | prototype_only | source_prototype */
void romFontMake(void) {
    extern void* wp;
    extern char** msg_tbl[6];
    extern void* __memAlloc(s32 heap, u32 size);
    extern void __memFree(s32 heap, void* memory);
    extern u16 OSGetFontEncode(void);
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

        if (OSGetFontEncode() == 1) {
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

        for (i = 0; i < *(s32*)((s32)wp + 4); i++) {
            struct {
                u16 code;
                u8 terminator;
            } text;
            u32 width;
            u16 character = uniqueCharacters[i];
            u8* glyph = glyphData + i * 0x140;

            text.code = character;
            if (text.code < 0x100) {
                text.code <<= 8;
            } else {
                text.terminator = 0;
            }
            OSGetFontTexel((const char*)&text.code, glyph, 0, 6, &width);
            *(u16*)(glyph + 0x120) = character;
            *(s16*)(glyph + 0x122) = (s16)width;
        }

        DCFlushRange(glyphData, *(s32*)((s32)wp + 4) * 0x140);
        __memFree(0, textureData);
        __memFree(0, fontData);
        __memFree(0, uniqueCharacters);
    }
    __memFree(0, characters);
}

