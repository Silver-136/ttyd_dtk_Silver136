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
extern void iconDispGxAlpha(void* icon, s32 x, s32 y, s32 alpha);

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

void iconDispGx(void* icon, s32 x, s32 y) {
    iconDispGxAlpha(icon, x, y, 0xFF);
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

    if (strcmp((const char*)((s32)icon + 0x18), name) == 0) {
} else {
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

    if (strcmp((const char*)((s32)icon + 0x18), name) == 0) {
} else {
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