#include "effect/n64/eff_n64.h"

extern void* wp;
extern void* gp;
extern void GXInitTexObj();
extern void TEXGetGXTexObjFromPalette();
extern void UnpackTexPalette();
extern void DVDMgrClose();
extern void GXClearVtxDesc(void);
extern void GXSetVtxDesc(s32 attr, s32 type);
extern void GXSetVtxAttrFmt(s32 vtxFmt, s32 attr, s32 compCnt, s32 compType, s32 frac);
extern void GXSetArray(s32 attr, void* base, s32 stride);
extern s32 strcmp(const char* s1, const char* s2);
extern void* arcOpen(const char* name, s32 unused0, s32 unused1);
extern char* getMarioStDvdRoot(void);
extern int sprintf(char* str, const char* format, ...);
extern s32 DVDMgrOpen(const char* path, s32 a, s32 b);
extern s32 DVDMgrGetLength(s32 entry);
extern void* __memAlloc(s32 heap, u32 size);
extern void DVDMgrReadAsync(s32 entry, void* dst, s32 size, s32 offset, void* callback);
void _callback_tpl(void* unused, void* fileInfo);

typedef struct EffSetN64 {
    s16 id;
    s16 pad;
    const char* name;
} EffSetN64;

extern EffSetN64 eff_set_n64[];
extern const char str_effect_n64_tpl_802fbafc[];
extern const char str_PCTs_e_PCTs_effect_n_802fbb0c[];
extern const char str_jp_80425b08[3];
extern const char str_us_80425b0c[3];

static u8 dummy_425[4];

void _callback_tpl(void* unused, void* fileInfo) {
    void* file = *(void**)((s32)fileInfo + 0x2C);

    UnpackTexPalette(*(void**)wp);
    DVDMgrClose(file);
    *(s32*)((s32)wp + 4) = 1;
}

u8 effInit64(void) {
    memset(wp, 0, 8);
    *(s32*)((s32)wp + 4) = 0;
}

#pragma use_lmw_stmw off
void effTexSetupN64(void) {
    char path[0x80];
    char* language;
    s32 file;
    s32 size;

    *(void**)wp = NULL;
    *(s32*)((s32)wp + 4) = 0;
    *(void**)wp = arcOpen(str_effect_n64_tpl_802fbafc, 0, 0);

    if (*(void**)wp == NULL) {
        language = (char*)str_us_80425b0c;
        if (*(u32*)((s32)gp + 0x16C) == 0) {
            language = (char*)str_jp_80425b08;
        }

        sprintf(path, str_PCTs_e_PCTs_effect_n_802fbb0c, getMarioStDvdRoot(), language);
        file = DVDMgrOpen(path, 2, 0);
        size = (DVDMgrGetLength(file) + 0x1F) & ~0x1F;
        *(void**)wp = __memAlloc(0, size);
        *(s32*)((s32)file + 0x6C) = file;
        DVDMgrReadAsync(file, *(void**)wp, size, 0, _callback_tpl);
    } else {
        UnpackTexPalette(*(void**)wp);
        *(s32*)((s32)wp + 4) = 1;
    }
}
#pragma use_lmw_stmw on


u8 effGetTexObjN64(int param_1, u32* param_2) {
    void* work = wp;

    if (*(s32*)((s32)work + 4) == 0) {
        GXInitTexObj(param_2, dummy_425, 1, 1, 0, 0, 0, 0);
    } else {
        TEXGetGXTexObjFromPalette(*(void**)work, param_2, param_1);
    }
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effGetSetN64(char* name) {
    EffSetN64* set;
    s32 i;

    set = eff_set_n64;
    i = 0;
    while (set->id != -1) {
        if (strcmp(set->name, name) == 0) {
            return &eff_set_n64[i];
        }
        set++;
        i++;
    }
    return NULL;
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off



int effTblRandN64(int param_1, int param_2) {
    extern s32 _rand_table[];
    s32 value;

    value = _rand_table[param_2 % 128];
    if (value < 0) {
        value = -value;
    }
    return value % (param_1 + 1);
}

void effSetVtxDescN64(void* data) {
    GXClearVtxDesc();
    GXSetVtxDesc(9, 3);
    GXSetVtxDesc(0xB, 3);
    GXSetVtxDesc(0xD, 3);
    GXSetVtxAttrFmt(0, 9, 1, 3, 0);
    GXSetVtxAttrFmt(0, 0xB, 1, 5, 0);
    GXSetVtxAttrFmt(0, 0xD, 1, 3, 5);
    GXSetArray(9, data, 0xE);
    GXSetArray(0xB, (void*)((s32)data + 0xA), 0xE);
    GXSetArray(0xD, (void*)((s32)data + 0x6), 0xE);
}


void tri1(s16 param_1, s16 param_2, s16 param_3) {
    volatile u16* fifo = (volatile u16*)0xCC008000;

    *fifo = param_1;
    *fifo = param_1;
    *fifo = param_1;
    *fifo = param_2;
    *fifo = param_2;
    *fifo = param_2;
    *fifo = param_3;
    *fifo = param_3;
    *fifo = param_3;
}


u8 tri2(s16 param_1, s16 param_2, s16 param_3, s32 param_4, s16 param_5, s16 param_6, s16 param_7) {
    volatile u16* fifo = (volatile u16*)0xCC008000;

    *fifo = param_1;
    *fifo = param_1;
    *fifo = param_1;
    *fifo = param_2;
    *fifo = param_2;
    *fifo = param_2;
    *fifo = param_3;
    *fifo = param_3;
    *fifo = param_3;
    *fifo = param_5;
    *fifo = param_5;
    *fifo = param_5;
    *fifo = param_6;
    *fifo = param_6;
    *fifo = param_6;
    *fifo = param_7;
    *fifo = param_7;
    *fifo = param_7;
}
