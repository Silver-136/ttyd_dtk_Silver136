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
    return 0;
}

/* stub-fill: romFontPrintGX | missing_definition | ghidra_signature */
u8 romFontPrintGX(void) {
    return 0;
}

/* stub-fill: romFontMake | prototype_only | source_prototype */
void romFontMake(void) {
    return;
}
