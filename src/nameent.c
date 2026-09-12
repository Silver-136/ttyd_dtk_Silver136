#include "nameent.h"

extern void* gp;
extern void* wp;
extern const char* lbl_802FE300[];
extern const char* lbl_802FE318[];
extern const char str_PCTs_w_PCTs_name_tpl_802fe4ac[];
extern char str___80426910[];
extern char str__80426d80[];
extern f32 float_0_80426ddc;
extern void* camGetPtr(s32);
extern void GXSetZCompLoc(s32);
extern void GXSetNumChans(s32);
extern void GXSetNumTevStages(s32);
extern void GXSetCullMode(s32);
extern void GXClearVtxDesc(void);
extern void GXSetCurrentMtx(s32);
extern void GXSetNumTexGens(s32);
extern void TEXGetGXTexObjFromPalette(s32, void*, s32);
extern void GXLoadTexObj(void*, s32);
extern void GXInitTexObjLOD(void*, s32, s32, f32, f32, f32, s32, s32, s32);
extern u16 GXGetTexObjWidth(void*);
extern u16 GXGetTexObjHeight(void*);
extern void GXSetChanCtrl(s32, s32, s32, s32, s32, s32, s32);
extern void GXSetBlendMode(s32, s32, s32, s32);
extern void GXSetAlphaCompare(s32, s32, s32, s32, s32);
extern void GXSetZMode(s32, s32, s32);
extern void GXSetTevOrder(s32, s32, s32, s32);
extern void GXSetVtxDesc(s32, s32);
extern void GXSetVtxAttrFmt(s32, s32, s32, s32, s32);
extern void GXLoadPosMtxImm(void*, s32);
extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
extern void GXBegin(s32, s32, s32);

char* getMarioStDvdRoot(void);
s32 fileAsyncf(s32 size, s32 flags, const char* fmt, ...);
void* fileAllocf(s32 size, const char* fmt, ...);
void fileFree(void* file);
void nameMain(void);
void nameEntDisp(void);
void dispEntry(s32 cameraId, s32 renderMode, void* callback, s32 param, f32 priority);
s32 strncmp(const char* s1, const char* s2, u32 n);

void nameEntInit(void) {
    extern void* memset(void* ptr, s32 value, u32 size);

    memset(wp, 0, 0x64);
}
void nameEntReInit(void) {

    *(s32*)((s32)wp + 4) = 3;
}
s32 nameEntPrepare(void) {
    const char* table[6];
    s32 result;

    table[0] = lbl_802FE300[0];
    table[1] = lbl_802FE300[1];
    table[2] = lbl_802FE300[2];
    table[3] = lbl_802FE300[3];
    table[4] = lbl_802FE300[4];
    table[5] = lbl_802FE300[5];
    result = fileAsyncf(4, 0, str_PCTs_w_PCTs_name_tpl_802fe4ac, getMarioStDvdRoot(), table[*(s32*)((s32)gp + 0x16C)]);
    return (u32)(-result | result) >> 31;
}

/* stub-fill: nameEntOn | missing_definition | ghidra_signature */
#pragma optimize_for_size off
void nameEntOn(int param_1) {
    extern char lbl_803AE790[];
    extern char str___80426910[];
    extern char str__80426d80[];
    extern char str_p_80426c8c[];
    extern char float_0_80426e94[];
    extern f32 float_neg106_80426e90;
    extern f32 float_135_80426e8c;
    extern f32 float_0_80426ddc;
    extern char* strcpy(char* dst, const char* src);
    extern char* strcat(char* dst, const char* src);
    s32 i;
    s32 col;
    s32 row;
    s32 quotient;
    f32 x;
    f32 y;
    f32 left;
    f32 right;
    char* fill;
    char* base = lbl_803AE790;

    *(u16*)wp = 0;
    *(s32*)((s32)wp + 0xC) = param_1;
    *(s32*)((s32)wp + 4) = 1;
    *(s32*)((s32)wp + 8) = 0;
    if (*(u32*)((s32)gp + 0x16C) == 0) {
        *(void**)((s32)wp + 0x50) = base + 0x40;
    } else {
        *(void**)((s32)wp + 0x50) = base + 0x430;
    }

    *(s32*)((s32)wp + 0x18) = 0;
    *(s32*)((s32)wp + 0x10) = 0;
    *(s32*)((s32)wp + 0x14) = 0;
    col = *(s32*)((s32)wp + 0x10);
    quotient = col / 5;
    x = (f32)(col * 0x18 + quotient * 0x1E - 0xBE);
    *(f32*)((s32)wp + 0x24) = x;
    *(f32*)((s32)wp + 0x1C) = x;
    row = *(s32*)((s32)wp + 0x14);
    y = (f32)(0x70 - row * 0x1C);
    *(f32*)((s32)wp + 0x28) = y;
    *(f32*)((s32)wp + 0x20) = y;
    left = float_neg106_80426e90;
    right = float_135_80426e8c;
    *(f32*)((s32)wp + 0x34) = left;
    *(f32*)((s32)wp + 0x2C) = left;
    *(f32*)((s32)wp + 0x38) = right;
    *(f32*)((s32)wp + 0x30) = right;

    strcpy((char*)((s32)wp + 0x3C), float_0_80426e94);
    i = 0;
    do {
        fill = str__80426d80;
        if (*(u32*)((s32)gp + 0x16C) == 0) {
            fill = str___80426910;
        }
        strcat((char*)((s32)wp + 0x3C), fill);
        i++;
    } while (i < 8);
    *(f32*)((s32)wp + 0x54) = float_0_80426ddc;

    if (*(u32*)((s32)gp + 0x16C) == 0) {
        if (param_1 == 2) {
            *(char**)(base + 0x64) = str___80426910;
            *(char**)(base + 0x25C) = str___80426910;
        } else {
            *(char**)(base + 0x64) = (char*)0x80426918;
            *(char**)(base + 0x25C) = (char*)0x80426AA8;
        }
    } else if (param_1 == 2) {
        *(char**)(base + 0x4EC) = str__80426d80;
    } else {
        *(char**)(base + 0x4EC) = str_p_80426c8c;
    }
}
#pragma optimize_for_size on


void nameEntOff(void) {

    *(s32*)((s32)wp + 4) = 3;
}

s32 nameEntWait(void) {
    s32 value;

    *(u16*)wp |= 1;
    value = *(s32*)((s32)wp + 8);
    return (u32)((0x65 - value) | (value - 0x65)) >> 31;
}

s32 nameEntIsCancel(void) {

    return (*(u16*)wp >> 1) & 1;
}

#pragma use_lmw_stmw off
char* nameEntGetName(void) {
    s32 count;
    s32 offset;
    s32 zero;

    if (*(s32*)((s32)wp + 8) != 0x65) {
        return NULL;
    }
    if (*(u32*)((s32)gp + 0x16C) == 0) {
        count = 7;
        offset = 0xE;
        zero = 0;
        do {
            if (strncmp((char*)((s32)wp + offset + 0x3C), str___80426910, 2) != 0) {
                break;
            }
            count--;
            *(u8*)((s32)wp + offset + 0x3C) = zero;
            *(u8*)((s32)wp + offset + 0x3D) = zero;
            offset -= 2;
        } while (count >= 1);
    } else {
        offset = 7;
        zero = 0;
        do {
            if (strncmp((char*)((s32)wp + offset + 0x3C), str__80426d80, 1) != 0) {
                break;
            }
            *(u8*)((s32)wp + offset + 0x3C) = zero;
            offset--;
        } while (offset >= 1);
    }
    return (char*)((s32)wp + 0x3C);
}
#pragma use_lmw_stmw on


void nameEntMain(void) {
    const char* table[6];
    s32 loaded;

    switch (*(s32*)((s32)wp + 4)) {
        case 0:
            break;
        case 1:
            table[0] = lbl_802FE318[0];
            table[1] = lbl_802FE318[1];
            table[2] = lbl_802FE318[2];
            table[3] = lbl_802FE318[3];
            table[4] = lbl_802FE318[4];
            table[5] = lbl_802FE318[5];
            if ((u32)fileAsyncf(4, 0, str_PCTs_w_PCTs_name_tpl_802fe4ac, getMarioStDvdRoot(), table[*(s32*)((s32)gp + 0x16C)]) != 0) {
                *(void**)((s32)wp + 0x60) = fileAllocf(4, str_PCTs_w_PCTs_name_tpl_802fe4ac, getMarioStDvdRoot(), table[*(s32*)((s32)gp + 0x16C)]);
                loaded = 1;
            } else {
                loaded = 0;
            }
            if (loaded != 0) {
                *(s32*)((s32)wp + 4) = *(s32*)((s32)wp + 4) + 1;
            }
        case 2:
            nameMain();
            break;
        case 3:
            if (*(void**)((s32)wp + 0x60) != NULL) {
                fileFree(*(void**)((s32)wp + 0x60));
            }
            *(void**)((s32)wp + 0x60) = NULL;
            *(s32*)((s32)wp + 4) = 0;
            break;
        default:
            break;
    }

    if (*(s32*)((s32)wp + 4) > 1) {
        dispEntry(4, 7, nameEntDisp, 0, float_0_80426ddc);
    }
}

void nameMain(void) {
    typedef struct NameEntWork {
        u16 flags;
        u16 pad02;
        s32 field04;
        s32 state;
        s32 lookupKey;
        s32 currentCursorCol;
        s32 currentCursorRow;
        s32 currentCharIndex;
        f32 currentCursorX;
        f32 currentCursorY;
        f32 targetCursorX;
        f32 targetCursorY;
        f32 currentNameX;
        f32 currentNameY;
        f32 targetNameX;
        f32 targetNameY;
        char nameStr[0x14];
        char** charTbl;
        f32 fade;
        s32 field58;
        s32 confirmChoice;
        void* file;
    } NameEntWork;

    extern f32 distABf(f32, f32, f32, f32);
    extern u32 keyGetButtonTrg(s32);
    extern u32 keyGetDirRep(s32);
    extern s32 psndSFXOn(s32);
    extern u32 strlen(const char*);
    extern char* strcpy(char*, const char*);
    extern char* msgSearch(const char*);
    extern f64 sin(f64);
    extern char* hiragana_tbl[];
    extern char* katakana_tbl[];
    extern char str_name_party3_802fe4a0[];
    extern char str___80426910[];
    extern char str__80426d80[];
    extern f32 float_1_80426de4;
    extern f32 float_2_80426e1c;
    extern f32 float_4_80426e80;
    extern f32 float_6_80426e70;
    extern f32 float_12_80426e74;
    extern f32 float_16_80426de0;
    extern f32 float_40_80426e34;
    extern f32 float_60_80426e84;
    extern f32 float_100_80426e50;
    extern f32 float_135_80426e8c;
    extern f32 float_neg60_80426e6c;
    extern f32 float_0p125_80426e88;

    f32 dist;
    s32 col;
    s32 row;
    s32 limit;
    s32 i;
    s32 count;
    s32 offset;
    u32 len;
    char* text;

    dist = distABf(((NameEntWork*)wp)->targetCursorX,
                   ((NameEntWork*)wp)->targetCursorY,
                   ((NameEntWork*)wp)->currentCursorX,
                   ((NameEntWork*)wp)->currentCursorY);

    switch (((NameEntWork*)wp)->state) {
        case 0:
            if ((((NameEntWork*)wp)->flags & 1) != 0 && *(s32*)((u8*)gp + 8) == 0) {
                if ((keyGetButtonTrg(0) & 0x100) != 0) {
                    col = ((NameEntWork*)wp)->currentCursorCol;
                    if (col >= 10) {
                        limit = 6;
                        if (*(u32*)((u8*)gp + 0x16C) == 0) {
                            limit = 4;
                        }

                        row = ((NameEntWork*)wp)->currentCursorRow;
                        if (row >= limit) {
                            switch (row) {
                                case 4:
                                    ((NameEntWork*)wp)->charTbl = hiragana_tbl;
                                    psndSFXOn(0x2000C);
                                    goto cursor_target_update;

                                case 5:
                                    ((NameEntWork*)wp)->charTbl = katakana_tbl;
                                    psndSFXOn(0x2000C);
                                    goto cursor_target_update;

                                case 6:
                                    if (((NameEntWork*)wp)->currentCharIndex > 0) {
                                        ((NameEntWork*)wp)->currentCharIndex--;
                                        psndSFXOn(0x2000B);
                                    }
                                    goto cursor_target_update;

                                case 7:
                                    if (*(u32*)((u8*)gp + 0x16C) == 0) {
                                        len = strlen(((NameEntWork*)wp)->nameStr) >> 1;
                                        count = 0;
                                        offset = 0;
                                        while (count < (s32)len) {
                                            if (strncmp(((NameEntWork*)wp)->nameStr + offset,
                                                        str___80426910, 2) != 0) {
                                                break;
                                            }
                                            offset += 2;
                                            count++;
                                        }

                                        if (count < (s32)len || ((NameEntWork*)wp)->lookupKey != 0) {
                                            ((NameEntWork*)wp)->confirmChoice = 1;
                                            ((NameEntWork*)wp)->state = 100;
                                            psndSFXOn(0x20012);
                                            if (((NameEntWork*)wp)->lookupKey == 1 &&
                                                count >= (s32)len) {
                                                text = msgSearch(str_name_party3_802fe4a0);
                                                strcpy(((NameEntWork*)wp)->nameStr, text);
                                                ((NameEntWork*)wp)->currentCharIndex =
                                                    strlen(((NameEntWork*)wp)->nameStr) >> 1;
                                            }
                                        } else {
                                            psndSFXOn(0x20014);
                                        }
                                    } else {
                                        len = strlen(((NameEntWork*)wp)->nameStr);
                                        count = 0;
                                        while (count < (s32)len) {
                                            if (strncmp(((NameEntWork*)wp)->nameStr + count,
                                                        str__80426d80, 1) != 0) {
                                                break;
                                            }
                                            count++;
                                        }

                                        if (count < (s32)len || ((NameEntWork*)wp)->lookupKey != 0) {
                                            ((NameEntWork*)wp)->confirmChoice = 1;
                                            ((NameEntWork*)wp)->state = 100;
                                            psndSFXOn(0x20012);
                                            if (((NameEntWork*)wp)->lookupKey == 1 &&
                                                count >= (s32)len) {
                                                text = msgSearch(str_name_party3_802fe4a0);
                                                strcpy(((NameEntWork*)wp)->nameStr, text);
                                                ((NameEntWork*)wp)->currentCharIndex =
                                                    strlen(((NameEntWork*)wp)->nameStr);
                                            }
                                        } else {
                                            psndSFXOn(0x20014);
                                        }
                                    }
                                    goto cursor_target_update;

                                default:
                                    goto cursor_target_update;
                            }
                        }
                    }

                    i = ((NameEntWork*)wp)->currentCharIndex;
                    if (i < 8) {
                        if (*(u32*)((u8*)gp + 0x16C) == 0) {
                            *(u16*)(((NameEntWork*)wp)->nameStr + i * 2) =
                                *(u16*)((NameEntWork*)wp)->charTbl[
                                    col + ((NameEntWork*)wp)->currentCursorRow * 14];
                        } else {
                            ((NameEntWork*)wp)->nameStr[i] =
                                *((NameEntWork*)wp)->charTbl[
                                    col + ((NameEntWork*)wp)->currentCursorRow * 14];
                        }

                        ((NameEntWork*)wp)->currentCharIndex++;
                        if (((NameEntWork*)wp)->currentCharIndex >= 8) {
                            ((NameEntWork*)wp)->currentCharIndex = 7;
                            ((NameEntWork*)wp)->currentCursorCol = 10;
                            ((NameEntWork*)wp)->currentCursorRow = 7;
                        }
                        psndSFXOn(0xA);
                    } else if (i == 8) {
                        ((NameEntWork*)wp)->currentCharIndex = 7;
                        if (*(u32*)((u8*)gp + 0x16C) == 0) {
                            *(u16*)(((NameEntWork*)wp)->nameStr +
                                    ((NameEntWork*)wp)->currentCharIndex * 2) =
                                *(u16*)((NameEntWork*)wp)->charTbl[
                                    ((NameEntWork*)wp)->currentCursorCol +
                                    ((NameEntWork*)wp)->currentCursorRow * 14];
                        } else {
                            ((NameEntWork*)wp)->nameStr[
                                ((NameEntWork*)wp)->currentCharIndex] =
                                *((NameEntWork*)wp)->charTbl[
                                    ((NameEntWork*)wp)->currentCursorCol +
                                    ((NameEntWork*)wp)->currentCursorRow * 14];
                        }
                        ((NameEntWork*)wp)->currentCursorCol = 10;
                        ((NameEntWork*)wp)->currentCursorRow = 7;
                        psndSFXOn(0xA);
                    }
                } else if ((keyGetButtonTrg(0) & 0x1000) != 0) {
                    if (*(u32*)((u8*)gp + 0x16C) == 0) {
                        len = strlen(((NameEntWork*)wp)->nameStr) >> 1;
                        count = 0;
                        offset = 0;
                        while (count < (s32)len) {
                            if (strncmp(((NameEntWork*)wp)->nameStr + offset,
                                        str___80426910, 2) != 0) {
                                break;
                            }
                            offset += 2;
                            count++;
                        }

                        if (count < (s32)len || ((NameEntWork*)wp)->lookupKey != 0) {
                            ((NameEntWork*)wp)->confirmChoice = 1;
                            ((NameEntWork*)wp)->state = 100;
                            psndSFXOn(0x12);
                            if (((NameEntWork*)wp)->lookupKey == 1 &&
                                count >= (s32)len) {
                                text = msgSearch(str_name_party3_802fe4a0);
                                strcpy(((NameEntWork*)wp)->nameStr, text);
                                ((NameEntWork*)wp)->currentCharIndex =
                                    strlen(((NameEntWork*)wp)->nameStr) >> 1;
                            }
                        } else {
                            psndSFXOn(0x20014);
                        }
                    } else {
                        len = strlen(((NameEntWork*)wp)->nameStr);
                        count = 0;
                        while (count < (s32)len) {
                            if (strncmp(((NameEntWork*)wp)->nameStr + count,
                                        str__80426d80, 1) != 0) {
                                break;
                            }
                            count++;
                        }

                        if (count < (s32)len || ((NameEntWork*)wp)->lookupKey != 0) {
                            ((NameEntWork*)wp)->confirmChoice = 1;
                            ((NameEntWork*)wp)->state = 100;
                            psndSFXOn(0x12);
                            if (((NameEntWork*)wp)->lookupKey == 1 &&
                                count >= (s32)len) {
                                text = msgSearch(str_name_party3_802fe4a0);
                                strcpy(((NameEntWork*)wp)->nameStr, text);
                                ((NameEntWork*)wp)->currentCharIndex =
                                    strlen(((NameEntWork*)wp)->nameStr);
                            }
                        } else {
                            psndSFXOn(0x20014);
                        }
                    }
                } else if ((keyGetButtonTrg(0) & 0x200) != 0) {
                    if (*(u32*)((u8*)gp + 0x16C) == 0) {
                        if (((NameEntWork*)wp)->currentCharIndex == 0 &&
                            ((NameEntWork*)wp)->lookupKey == 0 &&
                            strncmp(((NameEntWork*)wp)->nameStr, str___80426910, 2) == 0) {
                            psndSFXOn(0x13);
                            ((NameEntWork*)wp)->state = 0x65;
                            ((NameEntWork*)wp)->flags |= 2;
                        } else {
                            *(u16*)(((NameEntWork*)wp)->nameStr +
                                    ((NameEntWork*)wp)->currentCharIndex * 2) =
                                *(u16*)str___80426910;
                            ((NameEntWork*)wp)->currentCharIndex--;
                            if (((NameEntWork*)wp)->currentCharIndex < 0) {
                                ((NameEntWork*)wp)->currentCharIndex = 0;
                            }
                            psndSFXOn(0xB);
                        }
                    } else {
                        if (((NameEntWork*)wp)->currentCharIndex == 0 &&
                            ((NameEntWork*)wp)->lookupKey == 0 &&
                            strncmp(((NameEntWork*)wp)->nameStr, str__80426d80, 1) == 0) {
                            psndSFXOn(0x13);
                            ((NameEntWork*)wp)->state = 0x65;
                            ((NameEntWork*)wp)->flags |= 2;
                        } else {
                            ((NameEntWork*)wp)->nameStr[
                                ((NameEntWork*)wp)->currentCharIndex] = *str__80426d80;
                            ((NameEntWork*)wp)->currentCharIndex--;
                            if (((NameEntWork*)wp)->currentCharIndex < 0) {
                                ((NameEntWork*)wp)->currentCharIndex = 0;
                            }
                            psndSFXOn(0xB);
                        }
                    }
                } else if ((keyGetDirRep(0) & 0x4000) != 0) {
                    if (dist < 10.0f) {
                        ((NameEntWork*)wp)->currentCursorCol--;
                        limit = 5;
                        if (*(u32*)((u8*)gp + 0x16C) == 0) {
                            limit = 3;
                        }

                        if (((NameEntWork*)wp)->currentCursorRow > limit) {
                            if (((NameEntWork*)wp)->currentCursorCol < 0) {
                                ((NameEntWork*)wp)->currentCursorCol = 10;
                                if (((NameEntWork*)wp)->currentCursorRow > 7) {
                                    ((NameEntWork*)wp)->currentCursorRow = 7;
                                }
                            }
                        } else if (((NameEntWork*)wp)->currentCursorCol < 0) {
                            ((NameEntWork*)wp)->currentCursorCol = 13;
                        }
                        psndSFXOn(5);
                    }
                } else if ((keyGetDirRep(0) & 0x8000) != 0) {
                    if (dist < 10.0f) {
                        ((NameEntWork*)wp)->currentCursorCol++;
                        limit = 5;
                        if (*(u32*)((u8*)gp + 0x16C) == 0) {
                            limit = 3;
                        }

                        if (((NameEntWork*)wp)->currentCursorRow > limit) {
                            if (((NameEntWork*)wp)->currentCursorCol > 9 &&
                                ((NameEntWork*)wp)->currentCursorRow > 7) {
                                ((NameEntWork*)wp)->currentCursorRow = 7;
                            }
                            if (((NameEntWork*)wp)->currentCursorCol > 10) {
                                ((NameEntWork*)wp)->currentCursorCol = 0;
                            }
                        } else if (((NameEntWork*)wp)->currentCursorCol > 13) {
                            ((NameEntWork*)wp)->currentCursorCol = 0;
                        }
                        psndSFXOn(5);
                    }
                } else if ((keyGetDirRep(0) & 0x1000) != 0) {
                    if (dist < 10.0f) {
                        ((NameEntWork*)wp)->currentCursorRow--;
                        if (((NameEntWork*)wp)->currentCursorRow < 0) {
                            if (((NameEntWork*)wp)->currentCursorCol < 10) {
                                ((NameEntWork*)wp)->currentCursorRow = 8;
                            } else {
                                ((NameEntWork*)wp)->currentCursorRow = 7;
                                ((NameEntWork*)wp)->currentCursorCol = 10;
                            }
                        }
                        psndSFXOn(5);
                    }
                } else if ((keyGetDirRep(0) & 0x2000) != 0) {
                    if (dist < 10.0f) {
                        ((NameEntWork*)wp)->currentCursorRow++;
                        if (((NameEntWork*)wp)->currentCursorCol < 10) {
                            if (((NameEntWork*)wp)->currentCursorRow > 8) {
                                ((NameEntWork*)wp)->currentCursorRow = 0;
                            }
                        } else {
                            limit = 6;
                            if (*(u32*)((u8*)gp + 0x16C) == 0) {
                                limit = 4;
                            }
                            if (((NameEntWork*)wp)->currentCursorRow >= limit) {
                                ((NameEntWork*)wp)->currentCursorCol = 10;
                            }
                            if (((NameEntWork*)wp)->currentCursorRow > 7) {
                                ((NameEntWork*)wp)->currentCursorRow = 0;
                            }
                        }
                        psndSFXOn(5);
                    }
                }
            }

cursor_target_update:
            col = ((NameEntWork*)wp)->currentCursorCol;
            if (col > 9) {
                limit = 5;
                if (*(u32*)((u8*)gp + 0x16C) == 0) {
                    limit = 3;
                }

                if (((NameEntWork*)wp)->currentCursorRow > limit) {
                    ((NameEntWork*)wp)->targetCursorX = float_100_80426e50;
                    ((NameEntWork*)wp)->targetCursorY =
                        (f32)(-20 - (((NameEntWork*)wp)->currentCursorRow - 4) * 34);
                    ((NameEntWork*)wp)->targetCursorX -= float_6_80426e70;
                    ((NameEntWork*)wp)->targetCursorY += float_6_80426e70;
                    goto cursor_fade;
                }
            }

            ((NameEntWork*)wp)->targetCursorX =
                (f32)(col * 24 + (col / 5) * 30 - 190);
            ((NameEntWork*)wp)->targetCursorY =
                (f32)(112 - ((NameEntWork*)wp)->currentCursorRow * 28);
            ((NameEntWork*)wp)->targetCursorX -= float_16_80426de0;
            ((NameEntWork*)wp)->targetCursorY -= float_12_80426e74;

cursor_fade:
            ((NameEntWork*)wp)->fade -= 0.05f;
            if (((NameEntWork*)wp)->fade <= float_0_80426ddc) {
                ((NameEntWork*)wp)->fade = float_0_80426ddc;
            }
            break;

        case 100:
            if ((keyGetButtonTrg(0) & 0x100) != 0) {
                if (((NameEntWork*)wp)->confirmChoice == 0) {
                    ((NameEntWork*)wp)->state++;
                    psndSFXOn(0x12);
                } else {
                    ((NameEntWork*)wp)->state = 0;
                    psndSFXOn(0x13);
                }
            } else if ((keyGetButtonTrg(0) & 0x200) != 0) {
                ((NameEntWork*)wp)->state = 0;
                psndSFXOn(0x13);
            } else if ((keyGetDirRep(0) & 0x3000) != 0) {
                ((NameEntWork*)wp)->confirmChoice =
                    1 - ((NameEntWork*)wp)->confirmChoice;
                psndSFXOn(5);
            }

            ((NameEntWork*)wp)->targetCursorX = float_neg60_80426e6c;
            ((NameEntWork*)wp)->targetCursorY =
                (float_0_80426ddc - 30.0f) -
                float_40_80426e34 * (f32)((NameEntWork*)wp)->confirmChoice;
            ((NameEntWork*)wp)->fade += 0.05f;
            if (((NameEntWork*)wp)->fade >= float_1_80426de4) {
                ((NameEntWork*)wp)->fade = float_1_80426de4;
            }
            break;

        default:
            break;
    }

    ((NameEntWork*)wp)->currentCursorX +=
        (((NameEntWork*)wp)->targetCursorX -
         ((NameEntWork*)wp)->currentCursorX) /
        ((float_4_80426e80 * (f32)*(u32*)((u8*)gp + 4)) /
         float_60_80426e84);

    ((NameEntWork*)wp)->currentCursorY +=
        (((NameEntWork*)wp)->targetCursorY -
         ((NameEntWork*)wp)->currentCursorY) /
        ((float_4_80426e80 * (f32)*(u32*)((u8*)gp + 4)) /
         float_60_80426e84);

    ((NameEntWork*)wp)->currentCursorX +=
        __fabs(float_2_80426e1c *
               (f32)sin((f64)((f32)*(u32*)((u8*)gp + 0x1C) *
                              float_0p125_80426e88)));

    i = ((NameEntWork*)wp)->currentCharIndex;
    if (i > 7) {
        i--;
    }

    ((NameEntWork*)wp)->targetNameX = (f32)(i * 32 - 106);
    ((NameEntWork*)wp)->targetNameY = float_135_80426e8c;

    ((NameEntWork*)wp)->currentNameX +=
        (((NameEntWork*)wp)->targetNameX -
         ((NameEntWork*)wp)->currentNameX) /
        ((float_4_80426e80 * (f32)*(u32*)((u8*)gp + 4)) /
         float_60_80426e84);

    ((NameEntWork*)wp)->currentNameY +=
        (((NameEntWork*)wp)->targetNameY -
         ((NameEntWork*)wp)->currentNameY) /
        ((float_4_80426e80 * (f32)*(u32*)((u8*)gp + 4)) /
         float_60_80426e84);

    ((NameEntWork*)wp)->currentNameY +=
        __fabs(float_2_80426e1c *
               (f32)sin((f64)((f32)*(u32*)((u8*)gp + 0x1C) *
                              float_0p125_80426e88)));
}

/* stub-fill: nameEntDisp | prototype_only | source_prototype */
void nameEntDisp(void) {
    typedef f32 Mtx[3][4];
    typedef struct Vec { f32 x, y, z; } Vec;
    extern void* wp;
    extern void* camGetCurPtr(void);
    extern void* camGetPtr(s32);
    extern void GXGetProjectionv(f32*);
    extern void GXGetViewportv(f32*);
    extern void GXSetProjection(void*, s32);
    extern void GXSetProjectionv(f32*);
    extern void GXSetViewport(f32, f32, f32, f32, f32, f32);
    extern void winTexInit(s32);
    extern void winTexSet(s32, Vec*, Vec*, void*);
    extern void winTexInit_x2(s32);
    extern void winTexSet_x2(s32, s32, Vec*, Vec*, void*);
    extern void nameBG(void);
    extern void nameKirinukiGX(f64, f64, f64, f64);
    extern void nameWinGX(f64, f64, f64, f64, s32, s32);
    extern void FontDrawStart(void);
    extern void FontDrawString(f32, f32, const char*);
    extern void FontDrawColor(void*);
    extern void FontDrawEdge(void);
    extern void FontDrawEdgeOff(void);
    extern u16 FontGetMessageWidth(const char*);
    extern void* memset(void*, s32, u32);
    extern char* strncpy(char*, const char*, u32);
    extern u32 strlen(const char*);
    extern char* msgSearch(const char*);
    extern void windowDispGX_Waku_col(f64, f64, f64, f64, f64, s32, void*);
    extern void nameMaskGX(f64);
    extern char* msg_tbl[];
    extern char str_msg_nameent_ok_802fe450[];
    extern char str_msg_nameent_yes_802fe460[];
    extern char str_msg_nameent_no_802fe470[];
    extern char str_msg_nameent_ok_01_802fe480[];
    extern void* gp;
    extern u32 dat_80426db4;
    extern Vec vec3_802fe378;
    extern Vec vec3_802fe384;
    extern Vec vec3_802fe390;
    extern Vec vec3_802fe39c;
    extern Vec vec3_802fe3a8;
    extern Vec vec3_802fe3b4;
    u8* work = wp;
    f32 projection[7];
    f32 viewport[6];
    u32 savedCamera[0x98];
    Vec pos, scale;
    u32 color = 0xFFFFFFFF;
    u32 messageColor = 0xFFFFFFFF;
    char glyph[16];
    s32 file = ***(s32***)(work + 0x60);
    s32 i;
    s32 row;
    s32 col;
    u32* src;
    u32* dst;

    GXGetProjectionv(projection);
    GXGetViewportv(viewport);
    src = (u32*)camGetCurPtr();
    dst = savedCamera;
    for (i = 0; i < 0x4C; i++) {
        *dst++ = *src++;
        *dst++ = *src++;
    }
    src = (u32*)camGetPtr(1);
    dst = (u32*)camGetCurPtr();
    for (i = 0; i < 0x4C; i++) {
        *dst++ = *src++;
        *dst++ = *src++;
    }
    GXSetProjection((u8*)camGetPtr(1) + 0x160, *(s32*)((u8*)camGetPtr(1) + 0x1A0));

    nameBG();
    winTexInit(file);
    pos.x = -240.0f; pos.y = 120.0f; pos.z = 0.0f;
    scale.x = 480.0f; scale.y = 270.0f; scale.z = 1.0f;
    winTexSet(0x2A, &pos, &scale, &color);
    for (i = 0; i < 8; i++) {
        pos.x = -250.0f + 40.0f * (f32)i;
        pos.y = 20.0f;
        scale.x = 28.0f; scale.y = 28.0f; scale.z = 1.0f;
        winTexSet(0x29, &pos, &scale, &color);
    }
    pos.x = -160.0f; pos.y = -160.0f;
    scale.x = 500.0f; scale.y = 40.0f;
    winTexSet(0x2B, &pos, &scale, &color);
    nameKirinukiGX(-240.0, 120.0, 480.0, 270.0);

    FontDrawStart();
    i = 0;
    for (row = 0; row < 9; row++) {
        for (col = 0; col < 14; col++, i++) {
            const char* entry = (*(char***)(work + 0x50))[i];
            if (entry != 0) {
                u16 width = FontGetMessageWidth(entry);
                FontDrawString((f32)(col * 24 + (col / 5) * 30 - 190 +
                                     (24 - (s32)width) / 2),
                               (f32)(112 - row * 28), entry);
            }
        }
    }

    for (i = (*(u32*)((u8*)gp + 0x16C) == 0) ? 0 : 2; i < 4; i++) {
        if (*(s32*)(work + 0x10) < 10 || *(s32*)(work + 0x14) - 4 != i) {
            winTexInit_x2(file);
            pos = vec3_802fe3a8;
            pos.y = (f32)(-20 - i * 34);
            scale = vec3_802fe3b4;
            winTexSet_x2(i + 0x25, 0x37, &pos, &scale, &color);
        } else {
            winTexInit(file);
            pos = vec3_802fe378;
            pos.y = (f32)(-20 - i * 34);
            scale = vec3_802fe384;
            winTexSet(0x37, &pos, &scale, &dat_80426db4);
            winTexInit_x2(file);
            pos = vec3_802fe390;
            pos.y = (f32)(-14 - i * 34);
            scale = vec3_802fe39c;
            winTexSet_x2(i + 0x25, 0x37, &pos, &scale, &color);
        }
    }

    FontDrawStart();
    for (i = 0; i < 8; i++) {
        memset(glyph, 0, sizeof(glyph));
        if (*(u8*)(work + 0x3C + i) >= 0x80) {
            strncpy(glyph, (char*)work + 0x3C + i, 2);
            i++;
        } else {
            strncpy(glyph, (char*)work + 0x3C + i, 1);
        }
        if (strlen(glyph) != 0) {
            FontDrawString(-220.0f + 40.0f * (f32)i, -100.0f, glyph);
        }
    }

    {
        const char* message = msgSearch(msg_tbl[*(s32*)(work + 0x58)]);
        f32 messageWidth = (f32)FontGetMessageWidth(message);
        windowDispGX_Waku_col(-250.0, -160.0, 500.0, 40.0, 20.0, 0, &messageColor);
        FontDrawStart();
        FontDrawString((240.0f - messageWidth) * 0.5f - 120.0f, -168.0f, message);
        nameMaskGX((f64)*(f32*)(work + 0x54));
    }
    if (*(s32*)(work + 8) == 100) {
        const char* ok = msgSearch(str_msg_nameent_ok_802fe450);
        const char* yes = msgSearch(str_msg_nameent_yes_802fe460);
        const char* no = msgSearch(str_msg_nameent_no_802fe470);
        u32 textColor = 0xFFFFFFFF;
        if (*(s32*)(work + 0x58) == 4) {
            ok = msgSearch(str_msg_nameent_ok_01_802fe480);
        }
        nameWinGX(-180.0, 130.0, 360.0, 100.0, file, 1);
        nameWinGX(-80.0, 10.0, 160.0, 120.0, file, 1);
        FontDrawStart();
        FontDrawColor(&textColor);
        FontDrawEdge();
        FontDrawString(-(f32)FontGetMessageWidth(ok) * 0.5f, 110.0f, ok);
        FontDrawString(-(f32)FontGetMessageWidth((char*)work + 0x3C) * 0.5f,
                       70.0f, (char*)work + 0x3C);
        FontDrawString(-(f32)FontGetMessageWidth(no) * 0.5f, -20.0f, yes);
        FontDrawString(-(f32)FontGetMessageWidth(no) * 0.5f, -60.0f, no);
        FontDrawEdgeOff();
    }
    winTexInit(file);
    pos.x = *(f32*)(work + 0x1C) + 2.0f;
    pos.y = *(f32*)(work + 0x20) - 2.0f;
    pos.z = 0.0f;
    scale.x = 32.0f;
    scale.y = 32.0f;
    scale.z = 1.0f;
    winTexSet(0, &pos, &scale, &color);
    pos.x = *(f32*)(work + 0x1C);
    pos.y = *(f32*)(work + 0x20);
    winTexSet(0, &pos, &scale, &color);
    src = savedCamera;
    dst = (u32*)camGetCurPtr();
    for (i = 0; i < 0x4C; i++) {
        *dst++ = *src++;
        *dst++ = *src++;
    }
    GXSetProjectionv(projection);
    GXSetViewport(viewport[0], viewport[1], viewport[2], viewport[3], viewport[4], viewport[5]);
}

/* stub-fill: nameBG | missing_definition | ghidra_signature */
void nameBG(void) {
    extern void GXSetBlendMode(s32,s32,s32,s32);
    extern void GXSetAlphaCompare(s32,s32,s32,s32,s32);
    extern void GXSetZMode(s32,s32,s32);
    extern void GXSetFog(s32,f32,f32,f32,f32,void*);
    extern void GXSetChanCtrl(s32,s32,s32,s32,s32,s32,s32);
    extern void GXSetTevOrder(s32,s32,s32,s32);
    extern void GXSetTevOp(s32,s32);
    extern void GXSetVtxDesc(s32,s32);
    extern void GXSetVtxAttrFmt(s32,s32,s32,s32,s32);
    extern void GXLoadPosMtxImm(void*,s32);
    extern void TEXGetGXTexObjFromPalette(void*,void*,s32);
    extern void GXInitTexObjLOD(void*,s32,s32,f32,f32,f32,s32,s32,s32);
    extern void GXLoadTexObj(void*,s32);
    extern void GXSetTexCoordGen2(s32,s32,s32,s32,s32,s32);
    extern u32 GXGetTexObjHeight(void*);
    extern u32 GXGetTexObjWidth(void*);
    extern void PSMTXScale(void*,f32,f32,f32);
    extern void GXLoadTexMtxImm(void*,s32,s32);
    extern void GXBegin(s32,s32,s32);
    f32 m0[3][4], m1[3][4], m2[3][4];
    u8 t0[0x20], t1[0x20], t2[0x20];
    u32 fog=0;
    void* camera;
    void* palette;
    volatile f32* fifo=(volatile f32*)0xCC008000;
    f32 left[3]={-304.0f,-272.0f,272.0f};
    f32 right[3]={-272.0f,272.0f,304.0f};
    s32 indices[3]={0x22,0x21,0x23};
    f32 widths[3]={32.0f,544.0f,32.0f};
    f32 sx,sy;
#define DRAW_PANEL(obj,mtx,id,width,left,right) \
    TEXGetGXTexObjFromPalette(palette,obj,id); \
    GXInitTexObjLOD(obj,1,1,0.0f,0.0f,0.0f,0,0,0); \
    GXLoadTexObj(obj,0); GXSetNumTexGens(1); GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D); \
    sx=width/(f32)(GXGetTexObjWidth(obj)&0xFFFF); sy=480.0f/(f32)(GXGetTexObjHeight(obj)&0xFFFF); \
    PSMTXScale(mtx,sx,sy,1.0f); GXLoadTexMtxImm(mtx,0x1E,1); GXBegin(0x80,0,4); \
    *fifo=left; *fifo=240.0f; *fifo=0.0f; *fifo=0.0f; *fifo=0.0f; \
    *fifo=left; *fifo=-240.0f; *fifo=0.0f; *fifo=0.0f; *fifo=1.0f; \
    *fifo=right; *fifo=-240.0f; *fifo=0.0f; *fifo=1.0f; *fifo=1.0f; \
    *fifo=right; *fifo=240.0f; *fifo=0.0f; *fifo=1.0f; *fifo=0.0f
    GXSetBlendMode(0,1,0,0); GXSetZCompLoc(0); GXSetAlphaCompare(6,0x80,1,0,0);
    GXSetZMode(0,7,0); GXSetFog(0,0.0f,0.0f,0.0f,0.0f,&fog); GXSetNumChans(0);
    GXSetChanCtrl(4,0,0,0,0,0,2); GXSetNumTevStages(1); GXSetTevOrder(0,0,0,0xFF);
    GXSetTevOp(0,3); GXSetCullMode(0); GXClearVtxDesc(); GXSetVtxDesc(9,1); GXSetVtxDesc(13,1);
    GXSetVtxAttrFmt(0,9,1,4,0); GXSetVtxAttrFmt(0,13,1,4,0);
    camera=camGetPtr(1); GXLoadPosMtxImm((u8*)camera+0x11C,0); GXSetCurrentMtx(0);
    palette=*(void**)(*(s32*)((s32)wp+0x60));
    DRAW_PANEL(t0,m0,indices[0],widths[0],left[0],right[0]);
    DRAW_PANEL(t1,m1,indices[1],widths[1],left[1],right[1]);
    DRAW_PANEL(t2,m2,indices[2],widths[2],left[2],right[2]);
#undef DRAW_PANEL
}

void nameKirinukiGX(f64 x, f64 y, f64 width, f64 height) {
    typedef f32 Mtx[3][4];
    extern void GXSetTevOp(s32 stage, s32 mode);
    extern void GXSetFog(s32 type, f32 startz, f32 endz, f32 nearz, f32 farz, void* color);
    extern void PSMTXScale(Mtx mtx, f32 x, f32 y, f32 z);
    extern void GXLoadTexMtxImm(Mtx mtx, s32 id, s32 type);
    extern u32 dat_80426dcc;
    extern f32 float_1_80426de4;
    extern f32 float_8_80426df4;
    extern f64 double_8_802fe430;

    u32 fogColor;
    u8 tex0[0x20];
    u8 tex1[0x20];
    u8 tex2[0x20];
    u8 tex3[0x20];
    u8 tex4[0x20];
    u8 tex5[0x20];
    u8 tex6[0x20];
    u8 tex7[0x20];
    Mtx mtx0;
    Mtx mtx1;
    Mtx mtx2;
    Mtx mtx3;
    Mtx mtx4;
    Mtx mtx5;
    Mtx mtx6;
    Mtx mtx7;
    s32 textureIds[8] = {
        0x2F, 0x2D, 0x31, 0x2E, 0x32, 0x30, 0x34, 0x33
    };
    volatile f32* fifo = (volatile f32*)0xCC008000;
    f32 texWidth;
    f32 texHeight;
    f32 innerWidth;
    f32 innerHeight;
    f32 x8;
    f32 xRight8;
    f32 xRight;
    f32 y8;
    f32 yBottom8;
    f32 yBottom;

    GXSetBlendMode(0, 1, 0, 0);
    GXSetZCompLoc(0);
    GXSetAlphaCompare(6, 0x80, 1, 0, 0);
    GXSetZMode(0, 7, 0);

    fogColor = dat_80426dcc;
    GXSetFog(0, float_0_80426ddc, float_0_80426ddc,
             float_0_80426ddc, float_0_80426ddc, &fogColor);

    GXSetNumChans(0);
    GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevOp(0, 3);
    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(13, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 13, 1, 4, 0);
    GXLoadPosMtxImm((u8*)camGetPtr(8) + 0x11C, 0);
    GXSetCurrentMtx(0);

    innerWidth = (f32)((f64)(f32)(width - (f64)float_8_80426df4) -
                       (f64)float_8_80426df4);
    innerHeight = (f32)((f64)(f32)(height - (f64)float_8_80426df4) -
                        (f64)float_8_80426df4);
    x8 = (f32)x + 8.0f;
    xRight8 = (f32)((f64)(f32)(x + width) - (f64)float_8_80426df4);
    xRight = (f32)((f64)xRight8 + (f64)float_8_80426df4);
    y8 = (f32)(y - (f64)float_8_80426df4);
    yBottom8 = y8 - innerHeight;
    yBottom = (f32)((f64)yBottom8 - (f64)float_8_80426df4);

    TEXGetGXTexObjFromPalette(
        *(s32*)(*(s32*)(*(s32*)((s32)wp + 0x60) + 0xA0)), tex0, textureIds[0]);
    GXInitTexObjLOD(tex0, 1, 1, float_0_80426ddc, float_0_80426ddc,
                    float_0_80426ddc, 0, 0, 0);
    GXLoadTexObj(tex0, 0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    texHeight = (f32)GXGetTexObjHeight(tex0);
    texWidth = (f32)GXGetTexObjWidth(tex0);
    PSMTXScale(mtx0, (f32)__fabs(double_8_802fe430) / texWidth,
               (f32)__fabs(double_8_802fe430) / texHeight,
               float_1_80426de4);
    GXLoadTexMtxImm(mtx0, 0x1E, 1);
    GXBegin(0x80, 0, 4);
    *fifo = (f32)x; *fifo = (f32)y; *fifo = float_0_80426ddc; *fifo = float_0_80426ddc; *fifo = float_0_80426ddc;
    *fifo = (f32)x; *fifo = y8;     *fifo = float_0_80426ddc; *fifo = float_0_80426ddc; *fifo = float_1_80426de4;
    *fifo = x8;     *fifo = y8;     *fifo = float_0_80426ddc; *fifo = float_1_80426de4; *fifo = float_1_80426de4;
    *fifo = x8;     *fifo = (f32)y; *fifo = float_0_80426ddc; *fifo = float_1_80426de4; *fifo = float_0_80426ddc;

    TEXGetGXTexObjFromPalette(
        *(s32*)(*(s32*)(*(s32*)((s32)wp + 0x60) + 0xA0)), tex1, textureIds[1]);
    GXInitTexObjLOD(tex1, 1, 1, float_0_80426ddc, float_0_80426ddc,
                    float_0_80426ddc, 0, 0, 0);
    GXLoadTexObj(tex1, 0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    texHeight = (f32)GXGetTexObjHeight(tex1);
    texWidth = (f32)GXGetTexObjWidth(tex1);
    PSMTXScale(mtx1, __fabs(innerWidth) / texWidth,
               (f32)__fabs(double_8_802fe430) / texHeight,
               float_1_80426de4);
    GXLoadTexMtxImm(mtx1, 0x1E, 1);
    GXBegin(0x80, 0, 4);
    *fifo = x8;      *fifo = (f32)y; *fifo = float_0_80426ddc; *fifo = float_0_80426ddc; *fifo = float_0_80426ddc;
    *fifo = x8;      *fifo = y8;     *fifo = float_0_80426ddc; *fifo = float_0_80426ddc; *fifo = float_1_80426de4;
    *fifo = x8 + innerWidth; *fifo = y8;     *fifo = float_0_80426ddc; *fifo = float_1_80426de4; *fifo = float_1_80426de4;
    *fifo = x8 + innerWidth; *fifo = (f32)y; *fifo = float_0_80426ddc; *fifo = float_1_80426de4; *fifo = float_0_80426ddc;

    TEXGetGXTexObjFromPalette(
        *(s32*)(*(s32*)(*(s32*)((s32)wp + 0x60) + 0xA0)), tex2, textureIds[2]);
    GXInitTexObjLOD(tex2, 1, 1, float_0_80426ddc, float_0_80426ddc,
                    float_0_80426ddc, 0, 0, 0);
    GXLoadTexObj(tex2, 0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    texHeight = (f32)GXGetTexObjHeight(tex2);
    texWidth = (f32)GXGetTexObjWidth(tex2);
    PSMTXScale(mtx2, (f32)__fabs(double_8_802fe430) / texWidth,
               (f32)__fabs(double_8_802fe430) / texHeight,
               float_1_80426de4);
    GXLoadTexMtxImm(mtx2, 0x1E, 1);
    GXBegin(0x80, 0, 4);
    *fifo = xRight8; *fifo = (f32)y; *fifo = float_0_80426ddc; *fifo = float_0_80426ddc; *fifo = float_0_80426ddc;
    *fifo = xRight8; *fifo = y8;     *fifo = float_0_80426ddc; *fifo = float_0_80426ddc; *fifo = float_1_80426de4;
    *fifo = xRight;  *fifo = y8;     *fifo = float_0_80426ddc; *fifo = float_1_80426de4; *fifo = float_1_80426de4;
    *fifo = xRight;  *fifo = (f32)y; *fifo = float_0_80426ddc; *fifo = float_1_80426de4; *fifo = float_0_80426ddc;

    TEXGetGXTexObjFromPalette(
        *(s32*)(*(s32*)(*(s32*)((s32)wp + 0x60) + 0xA0)), tex3, textureIds[3]);
    GXInitTexObjLOD(tex3, 1, 1, float_0_80426ddc, float_0_80426ddc,
                    float_0_80426ddc, 0, 0, 0);
    GXLoadTexObj(tex3, 0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    texHeight = (f32)GXGetTexObjHeight(tex3);
    texWidth = (f32)GXGetTexObjWidth(tex3);
    PSMTXScale(mtx3, (f32)__fabs(double_8_802fe430) / texWidth,
               __fabs(innerHeight) / texHeight, float_1_80426de4);
    GXLoadTexMtxImm(mtx3, 0x1E, 1);
    GXBegin(0x80, 0, 4);
    *fifo = (f32)x; *fifo = y8;       *fifo = float_0_80426ddc; *fifo = float_0_80426ddc; *fifo = float_0_80426ddc;
    *fifo = (f32)x; *fifo = yBottom8; *fifo = float_0_80426ddc; *fifo = float_0_80426ddc; *fifo = float_1_80426de4;
    *fifo = x8;     *fifo = yBottom8; *fifo = float_0_80426ddc; *fifo = float_1_80426de4; *fifo = float_1_80426de4;
    *fifo = x8;     *fifo = y8;       *fifo = float_0_80426ddc; *fifo = float_1_80426de4; *fifo = float_0_80426ddc;

    TEXGetGXTexObjFromPalette(
        *(s32*)(*(s32*)(*(s32*)((s32)wp + 0x60) + 0xA0)), tex4, 0x2C);
    GXInitTexObjLOD(tex4, 1, 1, float_0_80426ddc, float_0_80426ddc,
                    float_0_80426ddc, 0, 0, 0);
    GXLoadTexObj(tex4, 0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    texHeight = (f32)GXGetTexObjHeight(tex4);
    texWidth = (f32)GXGetTexObjWidth(tex4);
    PSMTXScale(mtx4, __fabs((f32)(width - (f64)float_8_80426df4)) / texWidth,
               __fabs(innerHeight) / texHeight, float_1_80426de4);
    GXLoadTexMtxImm(mtx4, 0x1E, 1);
    GXBegin(0x80, 0, 4);
    *fifo = x8; *fifo = y8;       *fifo = float_0_80426ddc; *fifo = float_0_80426ddc; *fifo = float_0_80426ddc;
    *fifo = x8; *fifo = yBottom8; *fifo = float_0_80426ddc; *fifo = float_0_80426ddc; *fifo = float_1_80426de4;
    *fifo = x8 + (f32)(width - (f64)float_8_80426df4); *fifo = yBottom8; *fifo = float_0_80426ddc; *fifo = float_1_80426de4; *fifo = float_1_80426de4;
    *fifo = x8 + (f32)(width - (f64)float_8_80426df4); *fifo = y8;       *fifo = float_0_80426ddc; *fifo = float_1_80426de4; *fifo = float_0_80426ddc;

    TEXGetGXTexObjFromPalette(
        *(s32*)(*(s32*)(*(s32*)((s32)wp + 0x60) + 0xA0)), tex5, 0x30);
    GXInitTexObjLOD(tex5, 1, 1, float_0_80426ddc, float_0_80426ddc,
                    float_0_80426ddc, 0, 0, 0);
    GXLoadTexObj(tex5, 0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    texHeight = (f32)GXGetTexObjHeight(tex5);
    texWidth = (f32)GXGetTexObjWidth(tex5);
    PSMTXScale(mtx5, (f32)__fabs(double_8_802fe430) / texWidth,
               (f32)__fabs(double_8_802fe430) / texHeight,
               float_1_80426de4);
    GXLoadTexMtxImm(mtx5, 0x1E, 1);
    GXBegin(0x80, 0, 4);
    *fifo = (f32)x; *fifo = yBottom8; *fifo = float_0_80426ddc; *fifo = float_0_80426ddc; *fifo = float_0_80426ddc;
    *fifo = (f32)x; *fifo = yBottom;  *fifo = float_0_80426ddc; *fifo = float_0_80426ddc; *fifo = float_1_80426de4;
    *fifo = x8;     *fifo = yBottom;  *fifo = float_0_80426ddc; *fifo = float_1_80426de4; *fifo = float_1_80426de4;
    *fifo = x8;     *fifo = yBottom8; *fifo = float_0_80426ddc; *fifo = float_1_80426de4; *fifo = float_0_80426ddc;

    TEXGetGXTexObjFromPalette(
        *(s32*)(*(s32*)(*(s32*)((s32)wp + 0x60) + 0xA0)), tex6, 0x2C);
    GXInitTexObjLOD(tex6, 1, 1, float_0_80426ddc, float_0_80426ddc,
                    float_0_80426ddc, 0, 0, 0);
    GXLoadTexObj(tex6, 0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    texHeight = (f32)GXGetTexObjHeight(tex6);
    texWidth = (f32)GXGetTexObjWidth(tex6);
    PSMTXScale(mtx6, __fabs(innerWidth) / texWidth,
               (f32)__fabs(double_8_802fe430) / texHeight,
               float_1_80426de4);
    GXLoadTexMtxImm(mtx6, 0x1E, 1);
    GXBegin(0x80, 0, 4);
    *fifo = x8;      *fifo = yBottom8; *fifo = float_0_80426ddc; *fifo = float_0_80426ddc; *fifo = float_0_80426ddc;
    *fifo = x8;      *fifo = yBottom;  *fifo = float_0_80426ddc; *fifo = float_0_80426ddc; *fifo = float_1_80426de4;
    *fifo = x8 + innerWidth; *fifo = yBottom;  *fifo = float_0_80426ddc; *fifo = float_1_80426de4; *fifo = float_1_80426de4;
    *fifo = x8 + innerWidth; *fifo = yBottom8; *fifo = float_0_80426ddc; *fifo = float_1_80426de4; *fifo = float_0_80426ddc;

    TEXGetGXTexObjFromPalette(
        *(s32*)(*(s32*)(*(s32*)((s32)wp + 0x60) + 0xA0)), tex7, 0x32);
    GXInitTexObjLOD(tex7, 1, 1, float_0_80426ddc, float_0_80426ddc,
                    float_0_80426ddc, 0, 0, 0);
    GXLoadTexObj(tex7, 0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    texHeight = (f32)GXGetTexObjHeight(tex7);
    texWidth = (f32)GXGetTexObjWidth(tex7);
    PSMTXScale(mtx7, (f32)__fabs(double_8_802fe430) / texWidth,
               (f32)__fabs(double_8_802fe430) / texHeight,
               float_1_80426de4);
    GXLoadTexMtxImm(mtx7, 0x1E, 1);
    GXBegin(0x80, 0, 4);
    *fifo = xRight8; *fifo = yBottom8; *fifo = float_0_80426ddc; *fifo = float_0_80426ddc; *fifo = float_0_80426ddc;
    *fifo = xRight8; *fifo = yBottom;  *fifo = float_0_80426ddc; *fifo = float_0_80426ddc; *fifo = float_1_80426de4;
    *fifo = xRight;  *fifo = yBottom;  *fifo = float_0_80426ddc; *fifo = float_1_80426de4; *fifo = float_1_80426de4;
    *fifo = xRight;  *fifo = yBottom8; *fifo = float_0_80426ddc; *fifo = float_1_80426de4; *fifo = float_0_80426ddc;
}

void nameMaskGX(f32 scale) {
    typedef f32 Mtx[3][4];

    extern f32 float_1000_80426de8;
    extern f32 float_0_80426ddc;
    extern f32 float_1_80426de4;
    extern f32 float_0p5_80426dec;
    extern f32 float_neg0p5_80426df0;
    extern u32 dat_80426dd0;

    extern void GXSetZCompLoc(s32 beforeTex);
    extern void GXSetAlphaCompare(s32 comp0, s32 ref0, s32 op, s32 comp1, s32 ref1);
    extern void GXSetBlendMode(s32 type, s32 srcFactor, s32 dstFactor, s32 op);
    extern void GXSetZMode(s32 compareEnable, s32 func, s32 updateEnable);
    extern void GXSetNumChans(s32 num);
    extern void GXSetNumTevStages(s32 num);
    extern void GXSetTevOrder(s32 stage, s32 texCoord, s32 texMap, s32 colorChan);
    extern void GXSetTevColorOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevAlphaOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevColorIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevAlphaIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevColor(s32 reg, void* color);
    extern s32 fadeGetTpl(void);
    extern void TEXGetGXTexObjFromPalette(s32 tpl, void* texObj, s32 index);
    extern void GXInitTexObjLOD(void* texObj, s32 minFilt, s32 magFilt, f32 minLod, f32 maxLod, f32 lodBias, s32 biasClamp, s32 doEdgeLod, s32 maxAniso);
    extern void GXLoadTexObj(void* texObj, s32 mapId);
    extern void GXSetNumTexGens(s32 num);
    extern void PSMTXTrans(Mtx m, double x, double y, double z);
    extern void PSMTXScale(Mtx m, f32 x, f32 y, f32 z);
    extern void PSMTXConcat(Mtx a, Mtx b, Mtx out);
    extern void GXLoadTexMtxImm(Mtx m, s32 id, s32 type);
    extern void GXSetTexCoordGen2(s32 dstCoord, s32 func, s32 srcParam, s32 mtx, s32 normalize, s32 postMtx);
    extern void GXSetCullMode(s32 mode);
    extern void GXSetVtxDesc(s32 attr, s32 type);
    extern void GXSetVtxAttrFmt(s32 vtxfmt, s32 attr, s32 compCnt, s32 compType, s32 frac);
    extern void* camGetPtr(s32 cameraId);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXSetCurrentMtx(s32 id);
    extern void GXBegin(s32 prim, s32 vtxfmt, s32 nverts);

    u32 color;
    u8 texObj[0x20];
    Mtx transA;
    Mtx scaleMtx;
    Mtx transB;
    f32 amount;
    f32 zero;
    f32 one;
    volatile u16* fifo16;
    volatile f32* fifoF;
    void* cam;

    amount = float_1000_80426de8 * scale;
    amount = amount * scale;
    amount = scale * amount;
    amount = scale * amount;
    amount = scale * amount;

    GXSetZCompLoc(1);
    GXSetAlphaCompare(7, 0, 0, 7, 0);
    GXSetBlendMode(1, 4, 5, 7);
    GXSetZMode(0, 7, 0);
    GXSetNumChans(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0xF, 2, 8, 0xF);
    GXSetTevAlphaIn(0, 7, 1, 4, 7);

    color = dat_80426dd0;
    GXSetTevColor(1, &color);

    TEXGetGXTexObjFromPalette(fadeGetTpl(), texObj, 1);

    zero = float_0_80426ddc;
    GXInitTexObjLOD(texObj, 1, 1, zero, zero, zero, 0, 0, 0);
    GXLoadTexObj(texObj, 0);
    GXSetNumTexGens(1);

    PSMTXTrans(transA, (double)float_0p5_80426dec, (double)float_0p5_80426dec, (double)zero);
    PSMTXScale(scaleMtx, amount, amount, zero);
    PSMTXTrans(transB, (double)float_neg0p5_80426df0, (double)float_neg0p5_80426df0, (double)zero);
    PSMTXConcat(transA, scaleMtx, transA);
    PSMTXConcat(transA, transB, transA);

    GXLoadTexMtxImm(transA, 0x1E, 1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);

    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(0xD, 1);
    GXSetVtxAttrFmt(0, 9, 1, 3, 0);
    GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);

    cam = camGetPtr(8);
    GXLoadPosMtxImm((void*)((s32)cam + 0x11C), 0);
    GXSetCurrentMtx(0);

    GXBegin(0x80, 0, 4);

    fifo16 = (volatile u16*)0xCC008000;
    fifoF = (volatile f32*)0xCC008000;
    one = float_1_80426de4;

    *fifo16 = (u16)-0x130;
    *fifo16 = 0xF0;
    *fifo16 = 0;
    *fifoF = zero;
    *fifoF = zero;

    *fifo16 = 0x130;
    *fifo16 = 0xF0;
    *fifo16 = 0;
    *fifoF = one;
    *fifoF = zero;

    *fifo16 = 0x130;
    *fifo16 = (u16)-0xF0;
    *fifo16 = 0;
    *fifoF = one;
    *fifoF = one;

    *fifo16 = (u16)-0x130;
    *fifo16 = (u16)-0xF0;
    *fifo16 = 0;
    *fifoF = zero;
    *fifoF = one;
}


/* CHATGPT STUB FILL: main/nameent 20260624_184008 */

/* stub-fill: nameWinGX | missing_definition | ghidra_signature */
void nameWinGX(f64 x, f64 y, f64 width, f64 height, s32 palette, s32 texture) {
    typedef f32 Mtx[3][4];
    extern void GXSetTevSwapMode(s32, s32, s32);
    extern void GXSetFog(s32, f32, f32, f32, f32, void*);
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
    extern void GXSetTevColor(s32, void*);
    extern void GXSetTevKAlphaSel(s32, s32);
    extern void PSMTXScale(Mtx, f32, f32, f32);
    extern void GXLoadTexMtxImm(Mtx, s32, s32);
    u8 texObj0[0x20];
    u8 texObj1[0x20];
    u8 texObj2[0x20];
    u8 texObj3[0x20];
    Mtx texMtx0;
    Mtx texMtx1;
    Mtx texMtx2;
    Mtx texMtx3;
    volatile f32* fifo = (volatile f32*)0xCC008000;
    u32 fogColor = 0xFFFFFFFF;
    u32 tevColor = 0x00000080;
    f32 zero = 0.0f;
    f32 sixteen = 16.0f;
    f32 one = 1.0f;
    f64 sixteenD = 16.0;
    s32 ids[4] = { 4, 7, 6, 3 };

    GXSetZCompLoc(1);
    GXSetAlphaCompare(7, 0, 0, 7, 0);
    GXSetBlendMode(1, 4, 5, 7);
    GXSetZMode(0, 7, 0);
    GXSetFog(0, zero, zero, zero, zero, &fogColor);
    GXSetNumChans(0);
    GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2);
    GXSetNumTevStages(3);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0, 0, 0, 8);
    GXSetTevAlphaIn(0, 0, 0, 0, 7);
    GXSetTevSwapMode(0, 0, 0);
    GXSetTevOrder(1, 1, 1, 0xFF);
    GXSetTevColorOp(1, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(1, 0, 0, 0, 1, 0);
    GXSetTevColorIn(1, 0, 0xF, 10, 0);
    GXSetTevAlphaIn(1, 0, 4, 4, 7);
    GXSetTevColor(1, &tevColor);
    GXSetTevSwapMode(1, 0, 0);
    GXSetTevOrder(2, 1, 2, 0xFF);
    GXSetTevColorOp(2, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(2, 0, 0, 0, 1, 0);
    GXSetTevColorIn(2, 0xF, 8, 10, 0);
    GXSetTevAlphaIn(2, 7, 6, 4, 7);
    GXSetTevKAlphaSel(2, 0);
    GXSetTevSwapMode(2, 0, 0);
    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(13, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 13, 1, 4, 0);
    GXLoadPosMtxImm((u8*)camGetPtr(1) + 0x118, 0);
    GXSetCurrentMtx(0);

    {
        f32 rightEdge = (f32)(x + width - sixteenD);
        f32 bottomEdge = (f32)(y - height) + sixteen;

#define DRAW_NAME_REGION(obj, mtx, left, top, right, bottom, w, h, detail, mask) \
    GXSetNumTexGens(2); \
    TEXGetGXTexObjFromPalette(palette, obj, texture + ids[0]); \
    GXInitTexObjLOD(obj, 1, 1, zero, zero, zero, 0, 0, 0); \
    GXLoadTexObj(obj, 0); \
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D); \
    PSMTXScale(mtx, __fabs(w) / (f32)GXGetTexObjWidth(obj), \
                  __fabs(h) / (f32)GXGetTexObjHeight(obj), one); \
    GXLoadTexMtxImm(mtx, 0x1E, 1); \
    TEXGetGXTexObjFromPalette(palette, obj, texture + detail); \
    GXInitTexObjLOD(obj, 1, 1, zero, zero, zero, 0, 0, 0); \
    GXLoadTexObj(obj, 1); \
    TEXGetGXTexObjFromPalette(palette, obj, texture + mask); \
    GXInitTexObjLOD(obj, 1, 1, zero, zero, zero, 0, 0, 0); \
    GXLoadTexObj(obj, 2); \
    GXSetTexCoordGen2(1, 1, 4, 0x21, 0, 0x7D); \
    PSMTXScale(mtx, __fabs(w) / (f32)GXGetTexObjWidth(obj), \
                  __fabs(h) / (f32)GXGetTexObjHeight(obj), one); \
    GXLoadTexMtxImm(mtx, 0x21, 1); \
    GXBegin(0x80, 0, 4); \
    *fifo = left;  *fifo = top;    *fifo = zero; *fifo = zero; *fifo = zero; \
    *fifo = left;  *fifo = bottom; *fifo = zero; *fifo = zero; *fifo = one;  \
    *fifo = right; *fifo = bottom; *fifo = zero; *fifo = one;  *fifo = one;  \
    *fifo = right; *fifo = top;    *fifo = zero; *fifo = one;  *fifo = zero

    {
        f32 left = (f32)x;
        f32 top = (f32)y;
        f32 w = (f32)(width - sixteenD);
        f32 h = (f32)(height - sixteenD);
        f32 right = (f32)(x + w);
        f32 bottom = (f32)(y - h);
        DRAW_NAME_REGION(texObj0, texMtx0, left, top, right, bottom, w, h, 5, 0);
    }
    {
        f32 left = rightEdge;
        f32 top = (f32)y;
        f32 w = sixteen;
        f32 h = (f32)(height - sixteenD);
        f32 right = left + sixteen;
        f32 bottom = (f32)(y - h);
        DRAW_NAME_REGION(texObj1, texMtx1, left, top, right, bottom, w, h, ids[1], 2);
    }
    {
        f32 left = (f32)x;
        f32 top = bottomEdge;
        f32 w = (f32)(width - sixteenD);
        f32 h = sixteen;
        f32 right = (f32)(x + w);
        f32 bottom = top - sixteen;
        DRAW_NAME_REGION(texObj2, texMtx2, left, top, right, bottom, w, h, ids[2], 1);
    }
    {
        f32 left = rightEdge;
        f32 top = bottomEdge;
        f32 w = sixteen;
        f32 h = sixteen;
        f32 right = left + sixteen;
        f32 bottom = top - sixteen;
        DRAW_NAME_REGION(texObj3, texMtx3, left, top, right, bottom, w, h, 8, ids[3]);
    }
#undef DRAW_NAME_REGION
    }
}
