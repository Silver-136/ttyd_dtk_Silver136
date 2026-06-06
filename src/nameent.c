#include "nameent.h"

extern void* gp;
extern void* wp;
extern const char* lbl_802FE300[];
extern const char* lbl_802FE318[];
extern const char str_PCTs_w_PCTs_name_tpl_802fe4ac[];
extern char str___80426910[];
extern char str__80426d80[];
extern f32 float_0_80426ddc;

char* getMarioStDvdRoot(void);
s32 fileAsyncf(s32 size, s32 flags, const char* fmt, ...);
void* fileAllocf(s32 size, const char* fmt, ...);
void fileFree(void* file);
void nameMain(void);
void nameEntDisp(void);
void dispEntry(s32 cameraId, s32 renderMode, void* callback, s32 param, f32 priority);
s32 strncmp(const char* s1, const char* s2, u32 n);

void nameEntOff(void) {
    extern void* wp;

    *(s32*)((s32)wp + 4) = 3;
}

s32 nameEntIsCancel(void) {
    extern void* wp;

    return (*(u16*)wp >> 1) & 1;
}

void nameEntReInit(void) {
    extern void* wp;

    *(s32*)((s32)wp + 4) = 3;
}

void nameEntInit(void) {
    extern void* wp;
    extern void* memset(void* ptr, s32 value, u32 size);

    memset(wp, 0, 0x64);
}

s32 nameEntWait(void) {
    extern void* wp;
    s32 value;

    *(u16*)wp |= 1;
    value = *(s32*)((s32)wp + 8);
    return (u32)((0x65 - value) | (value - 0x65)) >> 31;
}

void nameEntMain(void) {
    const char* table[6];
    s32 loaded;

    switch (*(s32*)((s32)wp + 4)) {
        case 1:
            table[0] = lbl_802FE318[0];
            table[1] = lbl_802FE318[1];
            table[2] = lbl_802FE318[2];
            table[3] = lbl_802FE318[3];
            table[4] = lbl_802FE318[4];
            table[5] = lbl_802FE318[5];
            if (fileAsyncf(4, 0, str_PCTs_w_PCTs_name_tpl_802fe4ac, getMarioStDvdRoot(), table[*(s32*)((s32)gp + 0x16C)]) != 0) {
                *(void**)((s32)wp + 0x60) = fileAllocf(4, str_PCTs_w_PCTs_name_tpl_802fe4ac, getMarioStDvdRoot(), table[*(s32*)((s32)gp + 0x16C)]);
                loaded = 1;
            } else {
                loaded = 0;
            }
            if (loaded != 0) {
                *(s32*)((s32)wp + 4) = *(s32*)((s32)wp + 4) + 1;
            }
            /* fallthrough */
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
        while (strncmp((char*)((s32)wp + offset + 0x3C), str___80426910, 2) == 0) {
            count--;
            *(u8*)((s32)wp + offset + 0x3C) = zero;
            *(u8*)((s32)wp + offset + 0x3D) = zero;
            offset -= 2;
            if (count < 1) {
                break;
            }
        }
    } else {
        offset = 7;
        zero = 0;
        while (strncmp((char*)((s32)wp + offset + 0x3C), str__80426d80, 1) == 0) {
            *(u8*)((s32)wp + offset + 0x3C) = zero;
            offset--;
            if (offset < 1) {
                break;
            }
        }
    }
    return (char*)((s32)wp + 0x3C);
}
#pragma use_lmw_stmw on

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
