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
}


u8 iconMain(void) {
    return 0;
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
