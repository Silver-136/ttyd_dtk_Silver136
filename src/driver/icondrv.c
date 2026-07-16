#include "driver/icondrv.h"
#define ICON_SDA_U16_TABLE(base, index) (*(u16*)((s32)&(base) + ((index) * 2)))
extern s32 work;
extern void* gp;

extern s32 strcmp(const char* a, const char* b);
extern void* memset(void* dst, int value, unsigned long size);

extern void* camGetPtr(s32 cameraId);
extern void PSMTXTrans(void* mtx, f32 x, f32 y, f32 z);
extern void PSMTXScale(void* mtx, f32 x, f32 y, f32 z);
extern void PSMTXRotRad(void* mtx, s32 axis, f32 rad);
extern void PSMTXConcat(void* a, void* b, void* ab);
extern void iconGX(void* mtx, void* icon);
extern void iconDispGxAlpha(f32 scale, void* icon, s32 x, s32 y, s32 alpha);

extern void DVDMgrClose(s32 entry);
extern void UnpackTexPalette(void* palette);

extern s32 icon_bin_ok;
extern s32 icon_tpl_ok;
extern void* icon_tpl;
extern void* __memAlloc(s32 heap, u32 size);
extern const char* getMarioStDvdRoot(void);
extern s32 sprintf(char* buffer, const char* format, ...);
extern void* DVDMgrOpen(const char* path, s32 mode, s32 flags);
extern s32 DVDMgrGetLength(void* entry);
extern void DVDMgrReadAsync(void* entry, void* dst, s32 size, s32 offset, void* callback);

extern void _callback_tpl(void* unused, void* fileInfo);
extern void _callback_bin(void* unused, void* fileInfo);

extern const char str_PCTs_icon_tpl_802bf858[];
extern const char str_PCTs_icon_bin_802bf864[];
extern const f32 float_deg2rad_8041f8a0;

typedef struct IconVecRaw {
    u32 x;
    u32 y;
    u32 z;
} IconVecRaw;

extern const IconVecRaw vec3_802bf83c;
extern const IconVecRaw vec3_802bf824;
extern const IconVecRaw vec3_802bf818;

extern const f32 float_1_8041f88c;

extern u16 unk_80415040;
extern u16 unk_8041504c;

extern void* icon_bin;
extern void* icon_tpl;
extern s32 icon_bin_ok;
extern s32 icon_tpl_ok;

extern void* TEXGet(void* tpl, s32 id);

void iconTexSetup(void) {
    ;
}

void iconReInit(void) {
    void* workSet;

    workSet = &work;

    if (*(s32*)((s32)gp + 0x14) != 0) {
        workSet = (void*)((s32)workSet + 0x8);
    }

    memset(
        *(void**)((s32)workSet + 0x4),
        0,
        *(s32*)workSet * 0x38
    );
}

void* iconNameToPtr(const char* name) {
    void* workSet;
    void* icon;
    s32 i;

    workSet = &work;

    if (*(s32*)((s32)gp + 0x14) != 0) {
        workSet = (void*)((s32)workSet + 0x8);
    }

    icon = *(void**)((s32)workSet + 0x4);

    for (i = 0; i < *(s32*)workSet; i++, icon = (void*)((s32)icon + 0x38)) {
        if ((*(u16*)icon & 1) != 0) {
            if (strcmp((const char*)((s32)icon + 0x18), name) == 0) {
                return icon;
            }
        }
    }

    return NULL;
}

void iconFlagOff(const char* name, u32 flag) {
    void* workSet;
    s32 i;
    void* icon;

    workSet = &work;

    if (*(s32*)((s32)gp + 0x14) != 0) {
        workSet = (void*)((s32)workSet + 0x8);
    }

    icon = *(void**)((s32)workSet + 0x4);
    i = 0;
    goto check;

loop:
    if ((*(u16*)icon & 1) != 0) {
        if (strcmp((const char*)((s32)icon + 0x18), name) == 0) {
            goto found;
        }
    }

    i++;
    icon = (void*)((s32)icon + 0x38);

check:
    if (i < *(s32*)workSet) {
        goto loop;
    }

    icon = NULL;

found:
    if (icon != NULL) {
        *(u16*)icon &= ~flag;
    }
}

void iconFlagOn(const char* name, u32 flag) {
    void* workSet;
    s32 i;
    void* icon;

    workSet = &work;

    if (*(s32*)((s32)gp + 0x14) != 0) {
        workSet = (void*)((s32)workSet + 0x8);
    }

    icon = *(void**)((s32)workSet + 0x4);
    i = 0;
    goto check;

loop:
    if ((*(u16*)icon & 1) != 0) {
        if (strcmp((const char*)((s32)icon + 0x18), name) == 0) {
            goto found;
        }
    }

    i++;
    icon = (void*)((s32)icon + 0x38);

check:
    if (i < *(s32*)workSet) {
        goto loop;
    }

    icon = NULL;

found:
    if (icon != NULL) {
        *(u16*)icon |= flag;
    }
}

void iconSetAlpha(const char* name, s32 alpha) {
    void* workSet;
    s32 i;
    void* icon;
    u32 color;

    workSet = &work;

    if (*(s32*)((s32)gp + 0x14) != 0) {
        workSet = (void*)((s32)workSet + 0x8);
    }

    icon = *(void**)((s32)workSet + 0x4);
    i = 0;
    goto check;

loop:
    if ((*(u16*)icon & 1) != 0) {
        if (strcmp((const char*)((s32)icon + 0x18), name) == 0) {
            goto found;
        }
    }

    i++;
    icon = (void*)((s32)icon + 0x38);

check:
    if (i < *(s32*)workSet) {
        goto loop;
    }

    icon = NULL;

found:
    if (icon != NULL) {
        color = *(u32*)((s32)icon + 0x14);
        *((u8*)&color + 3) = alpha;
        *(u32*)((s32)icon + 0x14) = color;
    }
}

void iconSetScale(const char* name, f32 scale) {
    void* workSet;
    s32 i;
    void* icon;

    workSet = &work;

    if (*(s32*)((s32)gp + 0x14) != 0) {
        workSet = (void*)((s32)workSet + 0x8);
    }

    icon = *(void**)((s32)workSet + 0x4);
    i = 0;
    goto check;

loop:
    if ((*(u16*)icon & 1) != 0) {
        if (strcmp((const char*)((s32)icon + 0x18), name) == 0) {
            goto found;
        }
    }

    i++;
    icon = (void*)((s32)icon + 0x38);

check:
    if (i < *(s32*)workSet) {
        goto loop;
    }

    icon = NULL;

found:
    if (icon != NULL) {
        *(f32*)((s32)icon + 0x10) = scale;
    }
}

void iconDelete(const char* name) {
    void* workSet;
    void* icon;
    s32 i;

    workSet = &work;

    if (*(s32*)((s32)gp + 0x14) != 0) {
        workSet = (void*)((s32)workSet + 0x8);
    }

    icon = *(void**)((s32)workSet + 0x4);

    for (i = 0; i < *(s32*)workSet; i++, icon = (void*)((s32)icon + 0x38)) {
        if ((*(u16*)icon & 1) != 0) {
            if (strcmp((const char*)((s32)icon + 0x18), name) == 0) {
                *(u16*)icon &= ~1;
                break;
            }
        }
    }
}

void iconDispGx(f32 scale, void* icon, s32 x, s32 y) {
    iconDispGxAlpha(scale, icon, x, y, 0xFF);
}

void iconDisp(s32 cameraId, void* icon) {
    void* cam;
    f32 trans[3][4];
    f32 scale[3][4];
    f32 rot[3][4];
    f32 mtx[3][4];

    cam = camGetPtr(cameraId);

    PSMTXTrans(
        trans,
        *(f32*)((s32)icon + 0x4),
        *(f32*)((s32)icon + 0x8),
        *(f32*)((s32)icon + 0xC)
    );

    PSMTXScale(
        scale,
        *(f32*)((s32)icon + 0x10),
        *(f32*)((s32)icon + 0x10),
        *(f32*)((s32)icon + 0x10)
    );

    PSMTXRotRad(
        rot,
        0x79,
        float_deg2rad_8041f8a0 * -*(f32*)((s32)cam + 0x114)
    );

    PSMTXConcat(trans, rot, mtx);
    PSMTXConcat(mtx, scale, mtx);
    PSMTXConcat((void*)((s32)cam + 0x11C), mtx, mtx);

    iconGX(mtx, icon);
}

void _callback_bin(void* unused, void* fileInfo) {
    DVDMgrClose(*(s32*)((s32)fileInfo + 0x2C));
    icon_bin_ok = 1;
}

void _callback_tpl(void* unused, void* fileInfo) {
    s32 entry;

    entry = *(s32*)((s32)fileInfo + 0x2C);

    UnpackTexPalette(icon_tpl);
    DVDMgrClose(entry);

    icon_tpl_ok = 1;
}

void iconSetPos(const char* name, f32 x, f32 y, f32 z) {
    void* workSet;
    s32 i;
    void* icon;
    IconVecRaw pos;

    workSet = &work;

    if (*(s32*)((s32)gp + 0x14) != 0) {
        workSet = (void*)((s32)workSet + 0x8);
    }

    icon = *(void**)((s32)workSet + 0x4);
    i = 0;

    goto check;

loop:
    if ((*(u16*)icon & 1) == 0) {
        goto next;
    }

    if (strcmp((const char*)((s32)icon + 0x18), name) != 0) {
        goto next;
    }
    goto found;

next:
    i++;
    icon = (void*)((s32)icon + 0x38);

check:
    if (i < *(s32*)workSet) {
        goto loop;
    }

    icon = NULL;

found:
    if (icon != NULL) {
        pos = vec3_802bf83c;

        *(f32*)&pos.x = x;
        *(f32*)&pos.y = y;
        *(f32*)&pos.z = z;

        *(u32*)((s32)icon + 0x4) = pos.x;
        *(u32*)((s32)icon + 0x8) = pos.y;
        *(u32*)((s32)icon + 0xC) = pos.z;
    }
}

void iconGetWidthHight(u16* width, u16* height, s32 iconId) {
    void* bin;
    void* entry;
    void* tex;
    s32 id;

    id = iconId;
    bin = icon_bin;

    if (icon_tpl_ok == 0 || icon_bin_ok == 0) {
        entry = NULL;
    } else {
        id = (u16)id;

        if (id != 0x67) {
            if (id < 0x67) {
                if (id == 0x49) {
                    id = ICON_SDA_U16_TABLE(unk_80415040, *(s32*)((s32)gp + 0x16C));
                }
            }
        } else {
            id = ICON_SDA_U16_TABLE(unk_8041504c, *(s32*)((s32)gp + 0x16C));
        }

        entry = (void*)((s32)bin + (((u16)id) << 1));
        entry = (void*)((s32)bin + *(u16*)((s32)entry + 2));
    }

    *width = *(u16*)((s32)entry + 0x2);
    *height = *(u16*)((s32)entry + 0x4);

    if (*width == 0 && *height == 0) {
        tex = TEXGet(icon_tpl, (u16)iconId);
        *width = *(u16*)(*(s32*)tex + 0x2);
        *height = *(u16*)(*(s32*)tex + 0x0);
    }
}

void iconChange(const char* name, s32 iconId) {
    void* workSet;
    void* icon;
    void* bin;
    void* entry;
    s32 i;
    s32 id;

    workSet = &work;

    if (*(s32*)((s32)gp + 0x14) != 0) {
        workSet = (void*)((s32)workSet + 0x8);
    }

    icon = *(void**)((s32)workSet + 0x4);
    i = 0;

    goto change_check;

change_loop:
    if ((*(u16*)icon & 1) == 0) {
        goto change_next;
    }

    if (strcmp((const char*)((s32)icon + 0x18), name) != 0) {
        goto change_next;
    }
    goto change_found;

change_next:
    i++;
    icon = (void*)((s32)icon + 0x38);

change_check:
    if (i < *(s32*)workSet) {
        goto change_loop;
    }

    icon = NULL;

change_found:
    *(u16*)((s32)icon + 0x28) = iconId;

    id = *(u16*)((s32)icon + 0x28);
    bin = icon_bin;

    if (icon_tpl_ok == 0 || icon_bin_ok == 0) {
        entry = NULL;
    } else {
        id = (u16)id;

        if (id != 0x67) {
            if (id < 0x67) {
                if (id == 0x49) {
                    id = ICON_SDA_U16_TABLE(unk_80415040, *(s32*)((s32)gp + 0x16C));
                }
            }
        } else {
            id = ICON_SDA_U16_TABLE(unk_8041504c, *(s32*)((s32)gp + 0x16C));
        }

        entry = (void*)((s32)bin + (((u16)id) << 1));
        entry = (void*)((s32)bin + *(u16*)((s32)entry + 2));
    }

    *(u16*)((s32)icon + 0x2A) = *(u16*)((s32)entry + 0x2);
    *(u16*)((s32)icon + 0x2C) = *(u16*)((s32)entry + 0x4);
    *(u16*)((s32)icon + 0x2E) = *(u16*)((s32)entry + 0x6);
    *(u16*)((s32)icon + 0x30) = 0;
    *(u16*)((s32)icon + 0x32) = *(u16*)((s32)entry + 0x8);
    *(u16*)((s32)icon + 0x34) = *(u16*)((s32)entry + 0xA);

    if ((*(u16*)entry & 1) != 0) {
        *(u16*)icon |= 0x1000;
    }

    *(u16*)icon |= 1;
}

void iconInit(void) {
    char path[0x80];
    void* entry;
    void* workPtr;
    s32 size;
    s32 count;

    count = 0x20;
    work = count;

    workPtr = __memAlloc(0, count * 0x38);
    *(void**)(&work + 1) = workPtr;
    memset(workPtr, 0, work * 0x38);

    count = 0x20;
    *(s32*)((s32)&work + 0x8) = count;

    workPtr = __memAlloc(0, count * 0x38);
    *(void**)((s32)&work + 0xC) = workPtr;
    memset(workPtr, 0, *(s32*)((s32)&work + 0x8) * 0x38);

    icon_tpl = NULL;
    icon_bin = NULL;
    icon_tpl_ok = 0;
    icon_bin_ok = 0;

    sprintf(path, str_PCTs_icon_tpl_802bf858, getMarioStDvdRoot());
    entry = DVDMgrOpen(path, 2, 0);

    if (entry != NULL) {
        size = (DVDMgrGetLength(entry) + 0x1F) & ~0x1F;
        icon_tpl = __memAlloc(0, size);
        *(void**)((s32)entry + 0x6C) = entry;
        DVDMgrReadAsync(entry, icon_tpl, size, 0, _callback_tpl);
    }

    sprintf(path, str_PCTs_icon_bin_802bf864, getMarioStDvdRoot());
    entry = DVDMgrOpen(path, 2, 0);

    if (entry != NULL) {
        size = (DVDMgrGetLength(entry) + 0x1F) & ~0x1F;
        icon_bin = __memAlloc(0, size);
        *(void**)((s32)entry + 0x6C) = entry;
        DVDMgrReadAsync(entry, icon_bin, size, 0, _callback_bin);
    }
}


void iconGX(void* mtx, void* icon) {
    typedef f32 Mtx[3][4];
    typedef u32 TexObj[8];
    extern void* icon_tpl;
    extern s32 icon_tpl_ok, icon_bin_ok;
    extern void* camGetPtr(s32 id);
    extern void* camGetCurPtr(void);
    extern void mapSetMaterialLight(u32 flags, void* pos);
    extern void mapSetMaterialFog(void);
    extern void TEXGetGXTexObjFromPalette(void* tpl, void* obj, u32 id);
    extern void GXInitTexObj(void*, void*, u16, u16, s32, s32, s32, u8);
    extern void GXInitTexObjLOD(void*, s32, s32, f32, f32, f32, u8, u8, s32);
    extern void GXLoadTexObj(void*, s32);
    extern void PSMTXScale(Mtx, f32, f32, f32);
    extern void PSMTXConcat(Mtx, Mtx, Mtx);
    extern void GXLoadPosMtxImm(Mtx, s32);
    extern void GXSetCurrentMtx(s32);
    extern void GXSetNumChans(s32);
    extern void GXSetChanMatColor(s32, void*);
    extern void GXSetChanCtrl(s32, s32, s32, s32, s32, s32, s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevOp(s32, s32);
    extern void GXSetTevSwapMode(s32, s32, s32);
    extern void GXSetTevSwapModeTable(s32, s32, s32, s32, s32);
    extern void GXSetTevColor(s32, void*);
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
    extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern void GXSetBlendMode(s32, s32, s32, s32);
    extern void GXSetZCompLoc(s32);
    extern void GXSetAlphaCompare(s32, s32, s32, s32, s32);
    extern void GXSetZMode(s32, s32, s32);
    extern void GXSetCullMode(s32);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32, s32);
    extern void GXSetVtxAttrFmt(s32, s32, s32, s32, s32);
    extern u32 GXGetTexObjWidth(void*);
    extern u32 GXGetTexObjHeight(void*);
    extern void GXBegin(s32, s32, s32);
    extern u32 dat_8041f868;
    extern u32 dat_8041f86c;
    extern u8 dummy_672;
    u16* data;
    Mtx scale;
    Mtx model;
    TexObj tex;
    f32 pos[3];
    u32 color;
    u32 white;
    u32 width;
    u32 height;
    f32 left;
    f32 top;
    f32 right;
    f32 bottom;
    f32 sx;
    f32 sy;
    volatile f32* fifo;

    data = icon;
    pos[0] = ((f32(*)[4])mtx)[0][3];
    pos[1] = ((f32(*)[4])mtx)[1][3];
    pos[2] = ((f32(*)[4])mtx)[2][3];
    sy = ((*data & 8) != 0) ? -1.0f : 1.0f;
    sx = ((*data & 4) != 0) ? -1.0f : 1.0f;

    PSMTXScale(scale, sx, sy, 1.0f);
    PSMTXConcat(mtx, scale, model);
    if (camGetCurPtr() != camGetPtr(8) && ((*data & 0x10) != 0)) {
        PSMTXScale(scale, 1.0f, -1.0f, 1.0f);
        PSMTXConcat(model, scale, model);
    }
    GXLoadPosMtxImm(model, 0);
    GXSetCurrentMtx(0);

    if (icon_tpl_ok == 0 || icon_bin_ok == 0) {
        GXInitTexObj(tex, &dummy_672, 1, 1, 0, 0, 0, 0);
    } else {
        TEXGetGXTexObjFromPalette(icon_tpl, tex, data[0x19]);
        GXInitTexObjLOD(tex, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    }
    GXLoadTexObj(tex, 0);

    if ((*data & 0x10) == 0) {
        mapSetMaterialLight(4, pos);
    } else {
        GXSetNumChans(1);
        white = dat_8041f868;
        GXSetChanMatColor(4, &white);
        GXSetChanCtrl(0, 0, 0, 0, 0, 0, 2);
    }

    color = *(u32*)(data + 10);
    if ((*data & 0x20) == 0) {
        GXSetNumTevStages(2);
        GXSetTevOrder(0, 0, 0, 0);
        GXSetTevOp(0, 0);
        GXSetTevSwapMode(0, 0, 0);
        GXSetTevColor(1, &color);
        GXSetTevOrder(1, 0xFF, 0xFF, 0xFF);
        GXSetTevColorOp(1, 0, 0, 0, 1, 0);
        GXSetTevAlphaOp(1, 0, 0, 0, 1, 0);
        GXSetTevColorIn(1, 0xF, 2, 0, 0xF);
        GXSetTevAlphaIn(1, 7, 1, 0, 7);
    } else {
        GXSetNumTevStages(3);
        GXSetTevSwapModeTable(1, 0, 0, 0, 3);
        GXSetTevSwapModeTable(2, 1, 1, 1, 3);
        GXSetTevSwapModeTable(3, 2, 2, 2, 3);
        GXSetTevSwapMode(0, 0, 1);
        GXSetTevSwapMode(1, 0, 2);
        GXSetTevSwapMode(2, 0, 3);
        white = dat_8041f86c;
        GXSetTevColor(1, &white);
        GXSetTevColor(2, &color);

        GXSetTevOrder(0, 0, 0, 0xFF);
        GXSetTevColorOp(0, 0, 0, 0, 1, 0);
        GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
        GXSetTevColorIn(0, 0xF, 2, 8, 0xF);
        GXSetTevAlphaIn(0, 7, 7, 7, 4);
        GXSetTevOrder(1, 0, 0, 0xFF);
        GXSetTevColorOp(1, 0, 0, 0, 1, 0);
        GXSetTevAlphaOp(1, 0, 0, 0, 1, 0);
        GXSetTevColorIn(1, 0xF, 2, 8, 0);
        GXSetTevAlphaIn(1, 7, 7, 7, 4);
        GXSetTevOrder(2, 0, 0, 0xFF);
        GXSetTevColorOp(2, 0, 0, 0, 1, 0);
        GXSetTevAlphaOp(2, 0, 0, 0, 1, 0);
        GXSetTevColorIn(2, 0xF, 2, 8, 0);
        GXSetTevAlphaIn(2, 7, 1, 4, 7);
    }
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);

    if ((*data & 0x10) == 0) {
        mapSetMaterialFog();
    }

    if (*(s8*)((u8*)data + 0x17) == -1) {
        GXSetBlendMode(0, 4, 5, 0);
        GXSetZCompLoc(0);
        GXSetAlphaCompare(6, 0x80, 1, 0, 0);
        GXSetZMode(1, 3, 1);
    } else {
        GXSetBlendMode(1, 4, 5, 0);
        GXSetZCompLoc(1);
        GXSetAlphaCompare(7, 0, 0, 7, 0);
        GXSetZMode(1, 3, 0);
    }
    if ((*data & 0x10) != 0) {
        GXSetZMode(0, 3, 0);
    }
    GXSetCullMode((*data & 0x400) ? 2 : ((*data & 0x800) ? 1 : 0));

    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(13, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 13, 1, 4, 0);

    width = data[0x15];
    height = data[0x16];
    if (width == 0 && height == 0) {
        width = GXGetTexObjWidth(tex);
        height = GXGetTexObjHeight(tex);
    }
    left = -(f32)width * 0.5f;
    top = (f32)height;
    right = left + (f32)width;
    bottom = 0.0f;
    if ((*data & 0x10) == 0) {
        left *= 0.6f;
        top *= 0.6f;
        right *= 0.6f;
        bottom *= 0.6f;
        if (data[0x14] == 0x193) {
            left *= 0.8f;
            top *= 0.8f;
            right *= 0.8f;
        } else if (data[0x14] == 0x195) {
            left *= 0.9f;
            top *= 0.9f;
            right *= 0.9f;
        }
    }

    fifo = (volatile f32*)0xCC008000;
    GXBegin(0x80, 0, 4);
    *fifo = left;  *fifo = top;    *fifo = 0.0f; *fifo = 0.0f; *fifo = 0.0f;
    *fifo = right; *fifo = top;    *fifo = 0.0f; *fifo = 1.0f; *fifo = 0.0f;
    *fifo = right; *fifo = bottom; *fifo = 0.0f; *fifo = 1.0f; *fifo = 1.0f;
    *fifo = left;  *fifo = bottom; *fifo = 0.0f; *fifo = 0.0f; *fifo = 1.0f;

    if ((*data & 0x20) != 0) {
        GXSetTevSwapMode(0, 0, 0);
        GXSetTevSwapMode(1, 0, 0);
        GXSetTevSwapMode(2, 0, 0);
    }
}

void iconMain(void) {
    extern void shadowEntry(f32 x, f32 y, f32 z, f32 size);
    extern f32 dispCalcZ(void* pos);
    extern void dispEntry(s32 prio, s32 mode, void* disp, f32 z, void* data);
    extern void iconDisp(s32 cameraId, void* icon);
    extern f32 float_10_8041f874;
    extern f32 float_200_8041f8a8;

    void* workSet;
    u16* icon;
    void* entry;
    void* bin;
    s32 i;
    s32 id;
    u32 flags;
    u16 count;
    u16 frame;

    workSet = &work;
    if (*(s32*)((s32)gp + 0x14) != 0) {
        workSet = (void*)((s32)workSet + 0x8);
    }

    if (icon_tpl_ok == 0) {
        return;
    }
    if (icon_bin_ok == 0) {
        return;
    }

    icon = *(u16**)((s32)workSet + 0x4);
    i = 0;
    while (i < *(s32*)workSet) {
        flags = icon[0];
        if ((flags & 1) != 0) {
            if ((flags & 0x2000) != 0) {
                icon[0] = flags & ~0x2000;

                id = icon[0x14];
                bin = icon_bin;
                if (icon_tpl_ok == 0 || icon_bin_ok == 0) {
                    entry = NULL;
                } else {
                    id = (u16)id;
                    if (id == 0x67) {
                        id = ICON_SDA_U16_TABLE(unk_8041504c, *(s32*)((s32)gp + 0x16C));
                    } else if (id < 0x67 && id == 0x49) {
                        id = ICON_SDA_U16_TABLE(unk_80415040, *(s32*)((s32)gp + 0x16C));
                    }
                    entry = (void*)((s32)bin + (((u16)id) << 1));
                    entry = (void*)((s32)bin + *(u16*)((s32)entry + 2));
                }

                icon[0x15] = *(u16*)((s32)entry + 0x2);
                icon[0x16] = *(u16*)((s32)entry + 0x4);
                icon[0x17] = *(u16*)((s32)entry + 0x6);
                icon[0x18] = 0;
                icon[0x19] = *(u16*)((s32)entry + 0x8);
                icon[0x1A] = *(u16*)((s32)entry + 0xA);
                if ((*(u16*)entry & 1) != 0) {
                    icon[0] |= 0x1000;
                }
            }

            flags = icon[0];
            if ((flags & 0x1000) != 0) {
                if (icon[0x1A] != 0) {
                    icon[0x1A]--;
                }
                if (icon[0x1A] == 0) {
                    icon[0x18]++;
                    if (icon[0x18] >= icon[0x17]) {
                        icon[0x18] = 0;
                    }

                    id = icon[0x14];
                    bin = icon_bin;
                    if (icon_tpl_ok == 0 || icon_bin_ok == 0) {
                        entry = NULL;
                    } else {
                        id = (u16)id;
                        if (id == 0x67) {
                            id = ICON_SDA_U16_TABLE(unk_8041504c, *(s32*)((s32)gp + 0x16C));
                        } else if (id < 0x67 && id == 0x49) {
                            id = ICON_SDA_U16_TABLE(unk_80415040, *(s32*)((s32)gp + 0x16C));
                        }
                        entry = (void*)((s32)bin + (((u16)id) << 1));
                        entry = (void*)((s32)bin + *(u16*)((s32)entry + 2));
                    }

                    frame = icon[0x18];
                    icon[0x19] = *(u16*)((s32)entry + (frame * 4) + 0x8);
                    icon[0x1A] = *(u16*)((s32)entry + (frame * 4) + 0xA);
                }
            }

            flags = icon[0];
            if ((flags & 2) == 0) {
                if ((flags & 0x10) != 0) {
                    dispEntry(8, 1, iconDisp, float_200_8041f8a8, icon);
                } else if ((flags & 0x100) != 0) {
                    shadowEntry(*(f32*)((s32)icon + 0x4), *(f32*)((s32)icon + 0x8), *(f32*)((s32)icon + 0xC), float_10_8041f874);
                    dispEntry(5, 1, iconDisp, dispCalcZ((void*)((s32)icon + 0x4)), icon);
                } else if ((flags & 0x200) != 0) {
                    shadowEntry(*(f32*)((s32)icon + 0x4), *(f32*)((s32)icon + 0x8), *(f32*)((s32)icon + 0xC), float_10_8041f874);
                    dispEntry(7, 1, iconDisp, dispCalcZ((void*)((s32)icon + 0x4)), icon);
                } else {
                    shadowEntry(*(f32*)((s32)icon + 0x4), *(f32*)((s32)icon + 0x8), *(f32*)((s32)icon + 0xC), float_10_8041f874);
                    if (*(u8*)((s32)icon + 0x17) != 0xFF) {
                        dispEntry(4, 2, iconDisp, dispCalcZ((void*)((s32)icon + 0x4)), icon);
                    } else {
                        dispEntry(4, 1, iconDisp, dispCalcZ((void*)((s32)icon + 0x4)), icon);
                    }
                }
            }
        }
        i++;
        icon = (u16*)((s32)icon + 0x38);
    }
}

void iconDispGxAlpha(f32 scale, void* pos, s32 someBitfield, s32 iconId, s32 alpha) {
    extern void* camGetCurPtr(void);
    extern u32 dat_8041f860;
    u16 icon[0x1C];
    f32 trans[3][4];
    f32 scaleMtx[3][4];
    f32 rot[3][4];
    f32 concatMtx[3][4];
    void* cam;
    u16* iconPtr;
    void* entry;
    void* bin;
    s32 id;
    u32 color;
    s32 count;
    s32 total;
    s32 used;
    s32 i;
    void* scan;
    s32 retrace;
    f32 angle;

    cam = camGetCurPtr();
    iconPtr = icon;

    if (icon_tpl_ok != 0 && icon_bin_ok != 0) {
        PSMTXTrans(
            trans,
            *(f32*)((s32)pos + 0x0),
            *(f32*)((s32)pos + 0x4),
            *(f32*)((s32)pos + 0x8)
        );
        PSMTXScale(scaleMtx, scale, scale, scale);
        angle = -*(f32*)((s32)cam + 0x114);
        PSMTXRotRad(rot, 0x79, float_deg2rad_8041f8a0 * angle);
        PSMTXConcat(trans, rot, concatMtx);
        PSMTXConcat(concatMtx, scaleMtx, concatMtx);
        PSMTXConcat((void*)((s32)cam + 0x11C), concatMtx, concatMtx);

        color = dat_8041f860;
        *((u8*)&color + 3) = (u8)alpha;
        iconPtr[0] = someBitfield;
        *(u32*)((s32)iconPtr + 0x14) = color;
        iconPtr[0x14] = iconId;

        id = (u16)iconId;
        bin = icon_bin;
        if (icon_tpl_ok == 0 || icon_bin_ok == 0) {
            entry = NULL;
        } else {
            if (id != 0x67) {
                if (id < 0x67) {
                    if (id == 0x49) {
                        id = ICON_SDA_U16_TABLE(unk_80415040, *(s32*)((s32)gp + 0x16C));
                    }
                }
            } else {
                id = ICON_SDA_U16_TABLE(unk_8041504c, *(s32*)((s32)gp + 0x16C));
            }

            entry = (void*)((s32)bin + (((u16)id) << 1));
            entry = (void*)((s32)bin + *(u16*)((s32)entry + 2));
        }

        iconPtr[0x15] = *(u16*)((s32)entry + 0x2);
        iconPtr[0x16] = *(u16*)((s32)entry + 0x4);
        iconPtr[0x19] = *(u16*)((s32)entry + 0x8);
        iconPtr[0x1A] = *(u16*)((s32)entry + 0xA);

        count = *(u16*)((s32)entry + 0x6);
        if (count > 1) {
            total = 0;
            used = 0;
            if (count != 0) {
                if (count > 8 && (s32)(count - 8) > 0) {
                    scan = entry;
                    i = (count - 1) >> 3;
                    do {
                        used += 8;
                        total += *(u16*)((s32)scan + 0xA);
                        total += *(u16*)((s32)scan + 0xE);
                        total += *(u16*)((s32)scan + 0x12);
                        total += *(u16*)((s32)scan + 0x16);
                        total += *(u16*)((s32)scan + 0x1A);
                        total += *(u16*)((s32)scan + 0x1E);
                        total += *(u16*)((s32)scan + 0x22);
                        total += *(u16*)((s32)scan + 0x26);
                        scan = (void*)((s32)scan + 0x20);
                        i--;
                    } while (i != 0);
                }

                scan = (void*)((s32)entry + (used * 4));
                i = count - used;
                if (used < (s32)count) {
                    do {
                        total += *(u16*)((s32)scan + 0xA);
                        scan = (void*)((s32)scan + 4);
                        i--;
                    } while (i != 0);
                }
            }

            retrace = *(s32*)((s32)gp + 0x1C);
            retrace = retrace - ((retrace / total) * total);
            scan = entry;
            i = 0;
            while (count != 0) {
                if (retrace < *(u16*)((s32)scan + 0xA)) {
                    iconPtr[0x19] = *(u16*)((s32)entry + (i * 4) + 0x8);
                    break;
                }
                retrace -= *(u16*)((s32)scan + 0xA);
                scan = (void*)((s32)scan + 4);
                i++;
                count--;
            }
        }

        iconGX(concatMtx, iconPtr);
    }
}

u32 iconNumberDispGx3D(void* mtx, s32 number, s32 small, u32* color) {
    extern void PSMTXTrans(void* mtx, f32 x, f32 y, f32 z);
    extern void iconDispGxCol(s32 mtx, s16 someBitfield, u16 iconId, void* color);
    extern f32 float_0_8041f87c;
    extern f32 float_8_8041f878;
    extern f32 float_10_8041f874;
    extern u16 suuji[];
    extern u16 suuji_s[];
    s32 digits[5];
    s32 count;
    s32 tenThousands;
    s32 thousands;
    s32 hundreds;
    s32 tens;
    s32 hundredThousands;
    s32 i;
    u32 x;
    f32 step;
    f32 trans[3][4];
    u32 localColor;
    u16 iconId;

    step = float_8_8041f878;
    if (small == 0) {
        step = float_10_8041f874;
    }

    hundredThousands = number / 100000;
    number = number - (hundredThousands * 100000);
    tenThousands = number / 10000;
    digits[4] = tenThousands;
    number = number - (tenThousands * 10000);
    thousands = number / 1000;
    digits[3] = thousands;
    number = number - (thousands * 1000);
    hundreds = number / 100;
    digits[2] = hundreds;
    number = number - (hundreds * 100);
    tens = number / 10;
    digits[1] = tens;
    digits[0] = number - (tens * 10);

    if (hundredThousands == 0) {
        if (digits[4] == 0) {
            if (digits[3] == 0) {
                if (digits[2] == 0) {
                    if (digits[1] == 0) {
                        count = 1;
                    } else {
                        count = 2;
                    }
                } else {
                    count = 3;
                }
            } else {
                count = 4;
            }
        } else {
            count = 5;
        }
    } else {
        count = 5;
    }

    x = 0;
    for (i = 0; i < count; i++) {
        PSMTXTrans(trans, (f32)(-i) * step, float_0_8041f87c, float_0_8041f87c);
        PSMTXConcat(mtx, trans, trans);
        localColor = *color;
        if (small != 0) {
            iconId = suuji_s[digits[i]];
        } else {
            iconId = suuji[digits[i]];
        }
        iconDispGxCol((s32)trans, 0, iconId, &localColor);
        x = (u32)((f32)x + step);
    }

    return x;
}

u32 iconNumberDispGx(void* mtx, s32 number, s32 small, u32* color) {
    extern void PSMTXTrans(void* mtx, f32 x, f32 y, f32 z);
    extern void iconDispGxCol(s32 mtx, s16 someBitfield, u16 iconId, void* color);
    extern f32 float_0_8041f87c;
    extern f32 float_16_8041f884;
    extern f32 float_20_8041f880;
    extern u16 suuji[];
    extern u16 suuji_s[];
    s32 digits[5];
    s32 count;
    s32 tenThousands;
    s32 thousands;
    s32 hundreds;
    s32 tens;
    s32 hundredThousands;
    s32 i;
    u32 x;
    f32 step;
    f32 trans[3][4];
    f32 concat[3][4];
    u32 localColor;
    u16 iconId;

    step = float_16_8041f884;
    if (small == 0) {
        step = float_20_8041f880;
    }

    hundredThousands = number / 100000;
    number = number - (hundredThousands * 100000);
    tenThousands = number / 10000;
    digits[4] = tenThousands;
    number = number - (tenThousands * 10000);
    thousands = number / 1000;
    digits[3] = thousands;
    number = number - (thousands * 1000);
    hundreds = number / 100;
    digits[2] = hundreds;
    number = number - (hundreds * 100);
    tens = number / 10;
    digits[1] = tens;
    digits[0] = number - (tens * 10);

    if (hundredThousands == 0) {
        if (digits[4] == 0) {
            if (digits[3] == 0) {
                if (digits[2] == 0) {
                    if (digits[1] == 0) {
                        count = 1;
                    } else {
                        count = 2;
                    }
                } else {
                    count = 3;
                }
            } else {
                count = 4;
            }
        } else {
            count = 5;
        }
    } else {
        count = 5;
    }

    x = 0;
    for (i = 0; i < count; i++) {
        PSMTXTrans(trans, (f32)(-i) * step, float_0_8041f87c, float_0_8041f87c);
        PSMTXConcat(mtx, trans, concat);
        localColor = *color;
        if (small != 0) {
            iconId = suuji_s[digits[i]];
        } else {
            iconId = suuji[digits[i]];
        }
        iconDispGxCol((s32)concat, 0x10, iconId, &localColor);
        x = (u32)((f32)x + step);
    }

    return x;
}

void iconEntry(char* name, u16 iconId) {
    extern void strcpy(char* dst, const char* src);
    extern s32 strncmp(const char* a, const char* b, u32 n);
    extern void* mapGetWork(void);
    extern s32 evtGetValue(void* event, s32 value);
    extern u32 dat_8041f858;
    extern const char str_aji_8041f8a4[4];
    void* workSet;
    void* icon;
    void* entry;
    void* bin;
    void* mapWork;
    s32 i;
    s32 id;

    workSet = &work;

    if (*(s32*)((s32)gp + 0x14) != 0) {
        workSet = (void*)((s32)workSet + 0x8);
    }

    icon = *(void**)((s32)workSet + 0x4);
    i = 0;
    while (i < *(s32*)workSet) {
        if ((*(u16*)icon & 1) != 0) {
            if (strcmp((const char*)((s32)icon + 0x18), name) == 0) {
                break;
            }
        }
        i++;
        icon = (void*)((s32)icon + 0x38);
    }

    i = *(s32*)workSet;
    icon = *(void**)((s32)workSet + 0x4);
    if (i > 0) {
        for (; i != 0; i--) {
            if ((*(u16*)icon & 1) == 0) {
                break;
            }
            icon = (void*)((s32)icon + 0x38);
        }
    }

    memset(icon, 0, 0x38);

    *(u32*)((s32)icon + 0x4) = vec3_802bf818.x;
    *(u32*)((s32)icon + 0x8) = vec3_802bf818.y;
    *(u32*)((s32)icon + 0xC) = vec3_802bf818.z;
    *(f32*)((s32)icon + 0x10) = float_1_8041f88c;
    *(u32*)((s32)icon + 0x14) = dat_8041f858;

    strcpy((char*)((s32)icon + 0x18), name);
    *(u16*)((s32)icon + 0x28) = iconId;

    id = *(u16*)((s32)icon + 0x28);
    bin = icon_bin;

    if (icon_tpl_ok == 0 || icon_bin_ok == 0) {
        entry = NULL;
    } else {
        id = (u16)id;

        if (id != 0x67) {
            if (id < 0x67) {
                if (id == 0x49) {
                    id = ICON_SDA_U16_TABLE(unk_80415040, *(s32*)((s32)gp + 0x16C));
                }
            }
        } else {
            id = ICON_SDA_U16_TABLE(unk_8041504c, *(s32*)((s32)gp + 0x16C));
        }

        entry = (void*)((s32)bin + (((u16)id) << 1));
        entry = (void*)((s32)bin + *(u16*)((s32)entry + 2));
    }

    if (entry != NULL) {
        *(u16*)((s32)icon + 0x2A) = *(u16*)((s32)entry + 0x2);
        *(u16*)((s32)icon + 0x2C) = *(u16*)((s32)entry + 0x4);
        *(u16*)((s32)icon + 0x2E) = *(u16*)((s32)entry + 0x6);
        *(u16*)((s32)icon + 0x30) = 0;
        *(u16*)((s32)icon + 0x32) = *(u16*)((s32)entry + 0x8);
        *(u16*)((s32)icon + 0x34) = *(u16*)((s32)entry + 0xA);

        if ((*(u16*)entry & 1) != 0) {
            *(u16*)icon |= 0x1000;
        }
    } else {
        *(u16*)icon |= 0x2000;
    }

    *(u16*)icon |= 1;
    mapWork = mapGetWork();
    if ((*(u16*)((s32)mapWork + 0x4) & 2) != 0) {
        if (strncmp((const char*)((s32)gp + 0x12C), str_aji_8041f8a4, 3) == 0
            && evtGetValue(NULL, -170000000) == 0x178) {
            *(u16*)icon &= ~0x40;
        } else {
            *(u16*)icon |= 0x40;
        }
    }
}

void iconDispGx2(s32 mtx, s16 param_2, u16 iconId) {
    extern void* camGetCurPtr(void);
    extern u32 dat_8041f864;
    void* cam;
    void* entry;
    void* bin;
    f32 concatMtx[3][4];
    u16 icon[0x1C];
    u32 count;
    u32 id;
    s32 total;
    s32 used;
    s32 i;
    void* scan;
    s32 retrace;

    cam = camGetCurPtr();

    if (icon_tpl_ok != 0 && icon_bin_ok != 0) {
        icon[0] = param_2;
        *(u32*)((s32)icon + 0x14) = dat_8041f864;
        icon[0x14] = iconId;

        bin = icon_bin;
        id = iconId;
        if (icon_tpl_ok == 0 || icon_bin_ok == 0) {
            entry = NULL;
        } else {
            id = (u16)id;
            if (id != 0x67) {
                if (id < 0x67) {
                    if (id == 0x49) {
                        id = ICON_SDA_U16_TABLE(unk_80415040, *(s32*)((s32)gp + 0x16C));
                    }
                }
            } else {
                id = ICON_SDA_U16_TABLE(unk_8041504c, *(s32*)((s32)gp + 0x16C));
            }

            entry = (void*)((s32)bin + (((u16)id) << 1));
            entry = (void*)((s32)bin + *(u16*)((s32)entry + 2));
        }

        icon[0x15] = *(u16*)((s32)entry + 0x2);
        icon[0x16] = *(u16*)((s32)entry + 0x4);
        icon[0x19] = *(u16*)((s32)entry + 0x8);
        icon[0x1A] = *(u16*)((s32)entry + 0xA);

        count = *(u16*)((s32)entry + 0x6);
        if (count > 1) {
            total = 0;
            used = 0;
            if (count != 0) {
                if (count > 8 && (s32)(count - 8) > 0) {
                    scan = entry;
                    i = (count - 1) >> 3;
                    do {
                        used += 8;
                        total += *(u16*)((s32)scan + 0xA);
                        total += *(u16*)((s32)scan + 0xE);
                        total += *(u16*)((s32)scan + 0x12);
                        total += *(u16*)((s32)scan + 0x16);
                        total += *(u16*)((s32)scan + 0x1A);
                        total += *(u16*)((s32)scan + 0x1E);
                        total += *(u16*)((s32)scan + 0x22);
                        total += *(u16*)((s32)scan + 0x26);
                        scan = (void*)((s32)scan + 0x20);
                        i--;
                    } while (i != 0);
                }

                scan = (void*)((s32)entry + (used * 4));
                i = count - used;
                if (used < (s32)count) {
                    do {
                        total += *(u16*)((s32)scan + 0xA);
                        scan = (void*)((s32)scan + 4);
                        i--;
                    } while (i != 0);
                }
            }

            retrace = *(s32*)((s32)gp + 0x1C);
            retrace = retrace - ((retrace / total) * total);
            scan = entry;
            i = 0;
            while (count != 0) {
                if (retrace < *(u16*)((s32)scan + 0xA)) {
                    icon[0x19] = *(u16*)((s32)entry + (i * 4) + 0x8);
                    break;
                }
                retrace -= *(u16*)((s32)scan + 0xA);
                scan = (void*)((s32)scan + 4);
                i++;
                count--;
            }
        }

        PSMTXConcat((void*)((s32)cam + 0x11C), (void*)mtx, concatMtx);
        iconGX(concatMtx, icon);
    }
}

void iconDispGxCol(s32 mtx, s16 someBitfield, u16 iconId, void* color) {
    extern void* camGetCurPtr(void);
    void* cam;
    void* entry;
    void* bin;
    f32 concatMtx[3][4];
    u16 icon[0x1C];
    u32 count;
    u32 id;
    s32 total;
    s32 used;
    s32 i;
    void* scan;
    s32 retrace;

    cam = camGetCurPtr();

    if (icon_tpl_ok != 0 && icon_bin_ok != 0) {
        icon[0] = someBitfield;
        *(u32*)((s32)icon + 0x14) = *(u32*)color;
        icon[0x14] = iconId;

        bin = icon_bin;
        id = iconId;
        if (icon_tpl_ok == 0 || icon_bin_ok == 0) {
            entry = NULL;
        } else {
            id = (u16)id;
            if (id != 0x67) {
                if (id < 0x67) {
                    if (id == 0x49) {
                        id = ICON_SDA_U16_TABLE(unk_80415040, *(s32*)((s32)gp + 0x16C));
                    }
                }
            } else {
                id = ICON_SDA_U16_TABLE(unk_8041504c, *(s32*)((s32)gp + 0x16C));
            }

            entry = (void*)((s32)bin + (((u16)id) << 1));
            entry = (void*)((s32)bin + *(u16*)((s32)entry + 2));
        }

        icon[0x15] = *(u16*)((s32)entry + 0x2);
        icon[0x16] = *(u16*)((s32)entry + 0x4);
        icon[0x19] = *(u16*)((s32)entry + 0x8);
        icon[0x1A] = *(u16*)((s32)entry + 0xA);

        count = *(u16*)((s32)entry + 0x6);
        if (count > 1) {
            total = 0;
            used = 0;
            if (count != 0) {
                if (count > 8 && (s32)(count - 8) > 0) {
                    scan = entry;
                    i = (count - 1) >> 3;
                    do {
                        used += 8;
                        total += *(u16*)((s32)scan + 0xA);
                        total += *(u16*)((s32)scan + 0xE);
                        total += *(u16*)((s32)scan + 0x12);
                        total += *(u16*)((s32)scan + 0x16);
                        total += *(u16*)((s32)scan + 0x1A);
                        total += *(u16*)((s32)scan + 0x1E);
                        total += *(u16*)((s32)scan + 0x22);
                        total += *(u16*)((s32)scan + 0x26);
                        scan = (void*)((s32)scan + 0x20);
                        i--;
                    } while (i != 0);
                }

                scan = (void*)((s32)entry + (used * 4));
                i = count - used;
                if (used < (s32)count) {
                    do {
                        total += *(u16*)((s32)scan + 0xA);
                        scan = (void*)((s32)scan + 4);
                        i--;
                    } while (i != 0);
                }
            }

            retrace = *(s32*)((s32)gp + 0x1C);
            retrace = retrace - ((retrace / total) * total);
            scan = entry;
            i = 0;
            while (count != 0) {
                if (retrace < *(u16*)((s32)scan + 0xA)) {
                    icon[0x19] = *(u16*)((s32)entry + (i * 4) + 0x8);
                    break;
                }
                retrace -= *(u16*)((s32)scan + 0xA);
                scan = (void*)((s32)scan + 4);
                i++;
                count--;
            }
        }

        PSMTXConcat((void*)((s32)cam + 0x11C), (void*)mtx, concatMtx);
        iconGX(concatMtx, icon);
    }
}

void iconGetTexObj(u32* texObj, u16 iconId) {
    extern void GXInitTexObj(void* obj, void* image_ptr, u16 width, u16 height, s32 format, s32 wrap_s, s32 wrap_t, u8 mipmap);
    extern void TEXGetGXTexObjFromPalette(void* tpl, void* obj, u32 id);
    extern void GXInitTexObjLOD(void* obj, s32 minFilt, s32 magFilt, f32 minLod, f32 maxLod, f32 lodBias, u8 biasClamp, u8 doEdgeLod, s32 maxAniso);
    extern u8 dummy_651;
    extern f32 float_0_8041f87c;
    void* entry;
    void* bin;
    u32 id;
    u32 count;
    u32 texId;
    s32 total;
    s32 used;
    s32 i;
    void* scan;
    s32 retrace;

    id = iconId;

    if (icon_tpl_ok == 0 || icon_bin_ok == 0) {
        GXInitTexObj(texObj, &dummy_651, 1, 1, 0, 0, 0, 0);
        return;
    }

    bin = icon_bin;

    if (icon_tpl_ok == 0 || icon_bin_ok == 0) {
        entry = NULL;
    } else {
        id = (u16)id;

        if (id == 0x67) {
            id = ICON_SDA_U16_TABLE(unk_8041504c, *(s32*)((s32)gp + 0x16C));
        } else if (id < 0x67 && id == 0x49) {
            id = ICON_SDA_U16_TABLE(unk_80415040, *(s32*)((s32)gp + 0x16C));
        }

        entry = (void*)((s32)bin + (((u16)id) << 1));
        entry = (void*)((s32)bin + *(u16*)((s32)entry + 2));
    }

    count = *(u16*)((s32)entry + 0x6);
    texId = *(u16*)((s32)entry + 0x8);

    if (count > 1) {
        total = 0;
        used = 0;

        if (count != 0) {
            if (count > 8 && (s32)(count - 8) > 0) {
                scan = entry;
                i = (count - 1) >> 3;
                do {
                    used += 8;
                    total += *(u16*)((s32)scan + 0xA);
                    total += *(u16*)((s32)scan + 0xE);
                    total += *(u16*)((s32)scan + 0x12);
                    total += *(u16*)((s32)scan + 0x16);
                    total += *(u16*)((s32)scan + 0x1A);
                    total += *(u16*)((s32)scan + 0x1E);
                    total += *(u16*)((s32)scan + 0x22);
                    total += *(u16*)((s32)scan + 0x26);
                    scan = (void*)((s32)scan + 0x20);
                    i--;
                } while (i != 0);
            }

            scan = (void*)((s32)entry + (used * 4));
            i = count - used;
            if (used < (s32)count) {
                do {
                    total += *(u16*)((s32)scan + 0xA);
                    scan = (void*)((s32)scan + 4);
                    i--;
                } while (i != 0);
            }
        }

        retrace = *(s32*)((s32)gp + 0x1C);
        retrace = retrace - ((retrace / total) * total);
        scan = entry;
        i = 0;
        while (count != 0) {
            if (retrace < *(u16*)((s32)scan + 0xA)) {
                texId = *(u16*)((s32)entry + (i * 4) + 0x8);
                break;
            }
            retrace -= *(u16*)((s32)scan + 0xA);
            scan = (void*)((s32)scan + 4);
            i++;
            count--;
        }
    }

    TEXGetGXTexObjFromPalette(icon_tpl, texObj, texId);
    GXInitTexObjLOD(texObj, 1, 1, float_0_8041f87c, float_0_8041f87c, float_0_8041f87c, 0, 0, 0);
}

void iconEntry2D(char* name, u16 iconId) {
    extern void strcpy(char* dst, const char* src);
    extern u32 dat_8041f85c;
    void* workSet;
    void* icon;
    void* entry;
    void* bin;
    s32 i;
    s32 id;

    workSet = &work;

    if (*(s32*)((s32)gp + 0x14) != 0) {
        workSet = (void*)((s32)workSet + 0x8);
    }

    icon = *(void**)((s32)workSet + 0x4);
    i = 0;
    while (i < *(s32*)workSet) {
        if ((*(u16*)icon & 1) != 0) {
            if (strcmp((const char*)((s32)icon + 0x18), name) == 0) {
                break;
            }
        }
        i++;
        icon = (void*)((s32)icon + 0x38);
    }

    i = *(s32*)workSet;
    icon = *(void**)((s32)workSet + 0x4);
    if (i > 0) {
        for (; i != 0; i--) {
            if ((*(u16*)icon & 1) == 0) {
                break;
            }
            icon = (void*)((s32)icon + 0x38);
        }
    }

    memset(icon, 0, 0x38);

    *(u32*)((s32)icon + 0x4) = vec3_802bf824.x;
    *(u32*)((s32)icon + 0x8) = vec3_802bf824.y;
    *(u32*)((s32)icon + 0xC) = vec3_802bf824.z;
    *(f32*)((s32)icon + 0x10) = float_1_8041f88c;
    *(u32*)((s32)icon + 0x14) = dat_8041f85c;

    strcpy((char*)((s32)icon + 0x18), name);
    *(u16*)((s32)icon + 0x28) = iconId;

    id = *(u16*)((s32)icon + 0x28);
    bin = icon_bin;

    if (icon_tpl_ok == 0 || icon_bin_ok == 0) {
        entry = NULL;
    } else {
        id = (u16)id;

        if (id != 0x67) {
            if (id < 0x67) {
                if (id == 0x49) {
                    id = ICON_SDA_U16_TABLE(unk_80415040, *(s32*)((s32)gp + 0x16C));
                }
            }
        } else {
            id = ICON_SDA_U16_TABLE(unk_8041504c, *(s32*)((s32)gp + 0x16C));
        }

        entry = (void*)((s32)bin + (((u16)id) << 1));
        entry = (void*)((s32)bin + *(u16*)((s32)entry + 2));
    }

    if (entry != NULL) {
        *(u16*)((s32)icon + 0x2A) = *(u16*)((s32)entry + 0x2);
        *(u16*)((s32)icon + 0x2C) = *(u16*)((s32)entry + 0x4);
        *(u16*)((s32)icon + 0x2E) = *(u16*)((s32)entry + 0x6);
        *(u16*)((s32)icon + 0x30) = 0;
        *(u16*)((s32)icon + 0x32) = *(u16*)((s32)entry + 0x8);
        *(u16*)((s32)icon + 0x34) = *(u16*)((s32)entry + 0xA);

        if ((*(u16*)entry & 1) != 0) {
            *(u16*)icon |= 0x1000;
        }
    } else {
        *(u16*)icon |= 0x2000;
    }

    *(u16*)icon |= 0x11;
}
