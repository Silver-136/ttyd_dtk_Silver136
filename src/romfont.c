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
u8 romFontPrintGX(void) {
    return 0;
}

/* stub-fill: romFontMake | prototype_only | source_prototype */
void romFontMake(void) {
    return;
}
