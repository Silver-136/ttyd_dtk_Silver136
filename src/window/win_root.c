#include "window/win_root.h"

extern f32 float_neg1000_804233a0;
void* sort_data[8];
void qqsort(void* base, s32 count, s32 size, void* compare);
s32 pouchGetHaveItemCnt(void);
s32 pouchGetHaveBadgeCnt(void);
s32 pouchHaveItem(s32 item);
s32 pouchHaveBadge(s32 badge);
void pouchSortItem(s32 type);
void winMakeSkipList(void* win);
void winMakeEquipList(void* win);
s32 N_compare_func4_1(void* a, void* b);
s32 N_compare_func4_1_r(void* a, void* b);
s32 N_compare_func4_2(void* a, void* b);
s32 N_compare_func4_2_r(void* a, void* b);
s32 compare_func4(void* a, void* b);
s32 compare_func4_r(void* a, void* b);
s32 compare_func5_1(void* a, void* b);
s32 compare_func5_1_r(void* a, void* b);
s32 compare_func5_2(void* a, void* b);
s32 compare_func5_2_r(void* a, void* b);
s32 compare_func6_1(void* a, void* b);
s32 compare_func6_1_r(void* a, void* b);
s32 compare_func6_2(void* a, void* b);
s32 compare_func6_2_r(void* a, void* b);
extern u8 itemDataTable[];
char* msgSearch(const char* msg);
void unk_800d48b0(char* msg, char* out);
s32 strcmp(const char* a, const char* b);
void FontGetMessageWidthLine(char* msg, u16* width);
char* strncpy(char* dest, const char* src, u32 n);
extern char _tmp_1042[];
void* battleGetUnitMonosiriPtr(s32 id);
s32 sprintf(char* str, const char* format, ...);
s32 evtGetValue(void* evt, s32 value);
extern char* tbl_1614[];
extern char str_menu_monosiri_PCTs_802f4d00[];
extern char str_btl_un_hatena_802f4ce0[];
extern char str_btl_un_rampell_802f4cf0[];

s32 winSortWait(void* win) {
    s32 value = *(s32*)((s32)win + 0x17C);
    return (u32)(-value | value) >> 31;
}

s32 compare_func1(void* a, void* b) {
    if (*(u8*)a > *(u8*)b) {
        return 1;
    }
    if (*(u8*)a < *(u8*)b) {
        return -1;
    }
    return 0;
}

s32 compare_func1_r(void* a, void* b) {
    if (*(u8*)a < *(u8*)b) {
        return 1;
    }
    if (*(u8*)a > *(u8*)b) {
        return -1;
    }
    return 0;
}

s32 compare_func5(void* a, void* b) {
    if (*(u8*)a > *(u8*)b) {
        return 1;
    }
    if (*(u8*)a < *(u8*)b) {
        return -1;
    }
    return 0;
}

s32 compare_func5_r(void* a, void* b) {
    if (*(u8*)a < *(u8*)b) {
        return 1;
    }
    if (*(u8*)a > *(u8*)b) {
        return -1;
    }
    return 0;
}

void winSortEntry(void* win, s32 type, f32 a, f32 b) {
    void* entry = sort_data[type];
    s32 index = 0;
    f32 neg = float_neg1000_804233a0;

    while (*(void**)entry != 0) {
        entry = (void*)((s32)entry + 8);
        index++;
    }

    *(s32*)((s32)win + 0x184) = type;
    *(s32*)((s32)win + 0x178) = index;
    *(s32*)((s32)win + 0x174) = 0;
    *(s32*)((s32)win + 0x180) = 0;
    *(f32*)((s32)win + 0x164) = neg;
    *(f32*)((s32)win + 0x16C) = a;
    *(f32*)((s32)win + 0x168) = b;
    *(f32*)((s32)win + 0x170) = b;
    *(s32*)((s32)win + 0x17C) = 1;
}


void winKirinukiGX(double param_1, double param_2, double param_3, double param_4, void* pWin, int param_6) {
    ;
}


u8 winMailGX(void) {
    return 0;
}


void winHakoGX(double param_1, double param_2, void* pWin, int param_4) {
    ;
}


void winBookGX(double param_1, double param_2, void* pWin, int param_4) {
    ;
}


u8 winWazaGX(void) {
    return 0;
}


s32 winRootMain(void* pWin) {
    return 0;
}


u8 winSortGX(void* pWin) {
    return 0;
}


u8 winMailDisp(void* pWin) {
    return 0;
}


u8 winNameGX(void) {
    return 0;
}


u8 winMsgDisp(double x, double y, void* pWin) {
    return 0;
}


u8 winBgGX(void) {
    return 0;
}


u8 winKageGX(void) {
    return 0;
}


u8 winBgMain(void* pWin) {
    return 0;
}


void winRootDisp(s32 cameraId, void* work) {
    ;
}


u8 winSortMain(void* pWin) {
    extern void psndSFXOn(s32 soundId);
    extern char* msgSearch(char* msg);
    extern void FontGetMessageWidthLine(char* msg, u16* width);
    extern s32 strcmp(char* a, char* b);
    extern char* strncpy(char* dst, char* src, u32 n);
    extern void* sort_data[8];
    extern char _tmp_1042[];
    extern f32 float_neg600_804233d0;
    extern f32 float_30_804233d4;
    extern f32 float_40_804233bc;
    extern f32 float_0p125_804233d8;
    u32 buttons;
    u32 dirs;
    char* msg;
    s32 changed;
    u16 width[4];

    buttons = *(u32*)((s32)pWin + 4);
    dirs = *(u32*)((s32)pWin + 0x10);
    if (*(s32*)((s32)pWin + 0x17C) == 0) {
        buttons = 0;
        dirs = 0;
    }
    if ((buttons & 0x1000) != 0) {
        *(s32*)((s32)pWin + 0x17C) = 0;
        *(f32*)((s32)pWin + 0x16C) = float_neg600_804233d0;
    } else if ((buttons & 0x200) != 0) {
        psndSFXOn(0x20013);
        *(s32*)((s32)pWin + 0x17C) = 0;
        *(f32*)((s32)pWin + 0x16C) = float_neg600_804233d0;
    } else if ((dirs & 0x1000) != 0) {
        psndSFXOn(0x20005);
        (*(s32*)((s32)pWin + 0x174))--;
        if (*(s32*)((s32)pWin + 0x174) < 0) {
            *(s32*)((s32)pWin + 0x174) = *(s32*)((s32)pWin + 0x178) - 1;
        }
    } else if ((dirs & 0x2000) != 0) {
        psndSFXOn(0x20005);
        (*(s32*)((s32)pWin + 0x174))++;
        if (*(s32*)((s32)pWin + 0x174) >= *(s32*)((s32)pWin + 0x178)) {
            *(s32*)((s32)pWin + 0x174) = 0;
        }
    } else if ((buttons & 0x100) != 0) {
        void (**entry)(void*);
        psndSFXOn(0x20012);
        entry = (void (**)(void*))((s32)sort_data[*(s32*)((s32)pWin + 0x184)] + (*(s32*)((s32)pWin + 0x174) << 3));
        entry[1](pWin);
    }

    if (*(s32*)((s32)pWin + 0x17C) != 0) {
        *(f32*)((s32)pWin + 0x158) = *(f32*)((s32)pWin + 0x16C) + float_30_804233d4;
        *(f32*)((s32)pWin + 0x15C) = (*(f32*)((s32)pWin + 0x170) - float_40_804233bc) - (f32)(*(s32*)((s32)pWin + 0x174) * 0x28);
        msg = *(char**)((s32)sort_data[*(s32*)((s32)pWin + 0x184)] + (*(s32*)((s32)pWin + 0x174) << 3));
        if (msg != 0) {
            changed = 0;
            if (*(char**)((s32)pWin + 0x144) != msg) {
                changed = 1;
            } else if (strcmp(_tmp_1042, msg) != 0) {
                changed = 1;
            }
            if (changed != 0) {
                *(s32*)((s32)pWin + 0x138) = 0;
                *(char**)((s32)pWin + 0x13C) = msg;
                *(s32*)((s32)pWin + 0x140) = 0;
                FontGetMessageWidthLine(msgSearch(*(char**)((s32)pWin + 0x13C)), width);
                *(s32*)((s32)pWin + 0x148) = 0;
                *(s32*)((s32)pWin + 0x14C) = width[0] + 1;
                *(char**)((s32)pWin + 0x144) = msg;
                strncpy(_tmp_1042, msg, 0x3C);
            }
        }
    }

    *(f32*)((s32)pWin + 0x164) = ((*(f32*)((s32)pWin + 0x16C) - *(f32*)((s32)pWin + 0x164)) * float_0p125_804233d8) + *(f32*)((s32)pWin + 0x164);
    *(f32*)((s32)pWin + 0x168) = ((*(f32*)((s32)pWin + 0x170) - *(f32*)((s32)pWin + 0x168)) * float_0p125_804233d8) + *(f32*)((s32)pWin + 0x168);
    return 0;
}

u8 winMsgMain(void* pWin) {
    extern f32 intplGetValue(f32 start, f32 end, s32 type, s32 time, s32 duration);
    extern u32 keyGetDirTrg(s32 controller);
    extern void psndSFXOn(s32 soundId);
    extern void* gp;
    extern f32 float_neg800_8042347c;
    extern f32 float_neg170_80423480;
    extern f32 float_0_80423338;
    s32 duration;
    s32 timer;
    s32 line;
    s32 max;

    switch (*(s32*)((s32)pWin + 0x130)) {
        case 0:
            *(f32*)((s32)pWin + 0x128) = float_neg800_8042347c;
            *(f32*)((s32)pWin + 0x12C) = float_neg170_80423480;
            *(s32*)((s32)pWin + 0x134) = 0;
            (*(s32*)((s32)pWin + 0x130))++;
            break;
        case 1:
            timer = *(s32*)((s32)pWin + 0x134);
            (*(s32*)((s32)pWin + 0x134))++;
            duration = (*(s32*)((s32)gp + 4) << 4) / 60;
            *(f32*)((s32)pWin + 0x128) = intplGetValue(float_neg800_8042347c, float_0_80423338, 0xB, timer, duration);
            duration = (*(s32*)((s32)gp + 4) << 4) / 60;
            if (*(s32*)((s32)pWin + 0x134) > duration) {
                (*(s32*)((s32)pWin + 0x130))++;
            }
            break;
        case 2:
            if (*(s32*)((s32)pWin + 0x20C) != 0xCA) {
                if ((keyGetDirTrg(0) & 0x100) != 0) {
                    *(s32*)((s32)pWin + 0x148) -= 2;
                    if (*(s32*)((s32)pWin + 0x148) < 0) {
                        *(s32*)((s32)pWin + 0x148) = 0;
                    } else {
                        psndSFXOn(0x20006);
                    }
                } else if ((keyGetDirTrg(0) & 0x200) != 0) {
                    line = *(s32*)((s32)pWin + 0x148);
                    max = *(s32*)((s32)pWin + 0x14C);
                    if (line / 2 < ((max + 1) / 2) - 1) {
                        *(s32*)((s32)pWin + 0x148) = line + 2;
                        psndSFXOn(0x20006);
                    }
                }
            }
            break;
        case 10:
            *(s32*)((s32)pWin + 0x134) = 0;
            (*(s32*)((s32)pWin + 0x130))++;
            break;
        case 11:
            timer = *(s32*)((s32)pWin + 0x134);
            (*(s32*)((s32)pWin + 0x134))++;
            duration = (*(s32*)((s32)gp + 4) << 4) / 60;
            *(f32*)((s32)pWin + 0x128) = intplGetValue(float_0_80423338, float_neg800_8042347c, 0xB, timer, duration);
            duration = (*(s32*)((s32)gp + 4) << 4) / 60;
            if (*(s32*)((s32)pWin + 0x134) > duration) {
                (*(s32*)((s32)pWin + 0x130))++;
            }
            break;
    }
    return 0;
}

u8 sort_4_3_func(void* pWin) {
    extern u8 itemDataTable[];
    s32 i;
    s32 j;
    s32* a;
    s32* b;
    s32 t0;
    s32 t1;
    s32 t2;
    s32 swap;
    s32 idA;
    s32 idB;

    if (*(s32*)((s32)pWin + 0x180) == 0) {
        for (i = 0; i < *(s32*)((s32)pWin + 0xD64) - 1; i++) {
            a = (s32*)((s32)pWin + 0x404 + (i * 0xC));
            b = a + 3;
            for (j = i + 1; j < *(s32*)((s32)pWin + 0xD64); j++) {
                swap = 0;
                idA = (s16)a[2];
                idB = (s16)b[2];
                if (*(s8*)&itemDataTable[idA * 0x28 + 0x1C] > *(s8*)&itemDataTable[idB * 0x28 + 0x1C]) {
                    swap = 1;
                } else if (*(s8*)&itemDataTable[idA * 0x28 + 0x1C] == *(s8*)&itemDataTable[idB * 0x28 + 0x1C] &&
                           *(s16*)&itemDataTable[idA * 0x28 + 0x12] > *(s16*)&itemDataTable[idB * 0x28 + 0x12]) {
                    swap = 1;
                }
                if (swap != 0) {
                    t0 = a[0];
                    t1 = a[1];
                    t2 = a[2];
                    a[0] = b[0];
                    a[1] = b[1];
                    a[2] = b[2];
                    b[0] = t0;
                    b[1] = t1;
                    b[2] = t2;
                }
                b += 3;
            }
        }
    } else {
        for (i = 0; i < *(s32*)((s32)pWin + 0xD64) - 1; i++) {
            a = (s32*)((s32)pWin + 0x404 + (i * 0xC));
            b = a + 3;
            for (j = i + 1; j < *(s32*)((s32)pWin + 0xD64); j++) {
                swap = 0;
                idA = (s16)a[2];
                idB = (s16)b[2];
                if (*(s8*)&itemDataTable[idA * 0x28 + 0x1C] < *(s8*)&itemDataTable[idB * 0x28 + 0x1C]) {
                    swap = 1;
                } else if (*(s8*)&itemDataTable[idA * 0x28 + 0x1C] == *(s8*)&itemDataTable[idB * 0x28 + 0x1C] &&
                           *(s16*)&itemDataTable[idA * 0x28 + 0x12] < *(s16*)&itemDataTable[idB * 0x28 + 0x12]) {
                    swap = 1;
                }
                if (swap != 0) {
                    t0 = a[0];
                    t1 = a[1];
                    t2 = a[2];
                    a[0] = b[0];
                    a[1] = b[1];
                    a[2] = b[2];
                    b[0] = t0;
                    b[1] = t1;
                    b[2] = t2;
                }
                b += 3;
            }
        }
    }
    *(s32*)((s32)pWin + 0x180) = 1 - *(s32*)((s32)pWin + 0x180);
    *(s32*)((s32)pWin + 0xD68) = 0;
    *(s32*)((s32)pWin + 0xD6C) = *(s32*)((s32)pWin + 0xD68) / 8;
    return 0;
}

u8 sort_5_3_func(void* pWin) {
    extern u8 itemDataTable[];
    s32 i;
    s32 j;
    s16* a;
    s16* b;
    s16 tmp;
    s32 swap;
    s32 idA;
    s32 idB;

    if (*(s32*)((s32)pWin + 0x180) == 0) {
        for (i = 0; i < *(s32*)((s32)pWin + 0xE94) - 1; i++) {
            a = (s16*)((s32)pWin + 0xEBC + (i * 2));
            b = a + 1;
            for (j = i + 1; j < *(s32*)((s32)pWin + 0xE94); j++) {
                tmp = *a;
                swap = 0;
                idA = (s16)(tmp + 0xF0);
                idB = (s16)(*b + 0xF0);
                if (*(s8*)&itemDataTable[idA * 0x28 + 0x1C] > *(s8*)&itemDataTable[idB * 0x28 + 0x1C]) {
                    swap = 1;
                } else if (*(s8*)&itemDataTable[idA * 0x28 + 0x1C] == *(s8*)&itemDataTable[idB * 0x28 + 0x1C] &&
                           *(s16*)&itemDataTable[idA * 0x28 + 0x12] > *(s16*)&itemDataTable[idB * 0x28 + 0x12]) {
                    swap = 1;
                }
                if (swap != 0) {
                    *a = *b;
                    *b = tmp;
                }
                b++;
            }
        }
    } else {
        for (i = 0; i < *(s32*)((s32)pWin + 0xE94) - 1; i++) {
            a = (s16*)((s32)pWin + 0xEBC + (i * 2));
            b = a + 1;
            for (j = i + 1; j < *(s32*)((s32)pWin + 0xE94); j++) {
                tmp = *a;
                swap = 0;
                idA = (s16)(tmp + 0xF0);
                idB = (s16)(*b + 0xF0);
                if (*(s8*)&itemDataTable[idA * 0x28 + 0x1C] < *(s8*)&itemDataTable[idB * 0x28 + 0x1C]) {
                    swap = 1;
                } else if (*(s8*)&itemDataTable[idA * 0x28 + 0x1C] == *(s8*)&itemDataTable[idB * 0x28 + 0x1C] &&
                           *(s16*)&itemDataTable[idA * 0x28 + 0x12] < *(s16*)&itemDataTable[idB * 0x28 + 0x12]) {
                    swap = 1;
                }
                if (swap != 0) {
                    *a = *b;
                    *b = tmp;
                }
                b++;
            }
        }
    }
    *(s32*)((s32)pWin + 0x180) = 1 - *(s32*)((s32)pWin + 0x180);
    *(s32*)((s32)pWin + 0xE9C) = 0;
    *(s32*)((s32)pWin + 0xEA0) = *(s32*)((s32)pWin + 0xE9C) / 28;
    return 0;
}

u8 winZClear(void) {
    extern void PSMTXTrans(void* mtx, f32 x, f32 y, f32 z);
    extern void* camGetCurPtr(void);
    extern void PSMTXConcat(void* a, void* b, void* out);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXSetCurrentMtx(s32 id);
    extern void GXSetAlphaUpdate(s32 update);
    extern void GXSetColorUpdate(s32 update);
    extern void GXSetNumChans(s32 count);
    extern void GXSetChanCtrl(s32 chan, s32 enable, s32 ambSrc, s32 matSrc, s32 lightMask, s32 diffFn, s32 attnFn);
    extern void GXSetNumTevStages(s32 count);
    extern void GXSetTevOrder(s32 stage, s32 texCoord, s32 texMap, s32 colorChan);
    extern void GXSetTevOp(s32 stage, s32 mode);
    extern void GXSetNumTexGens(s32 count);
    extern void GXSetZMode(s32 enable, s32 func, s32 update);
    extern void GXSetBlendMode(s32 type, s32 srcFact, s32 dstFact, s32 op);
    extern void GXSetZCompLoc(s32 beforeTex);
    extern void GXSetAlphaCompare(s32 comp0, s32 ref0, s32 op, s32 comp1, s32 ref1);
    extern void GXSetCullMode(s32 mode);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxAttrFmt(s32 vtxFmt, s32 attr, s32 compCnt, s32 compType, s32 frac);
    extern void GXSetVtxDesc(s32 attr, s32 type);
    extern void GXBegin(s32 primitive, s32 vtxFmt, s32 nVerts);
    extern f32 float_0_80423338;
    extern f32 float_neg1000_804233a0;
    extern f32 float_neg500_804233a4;
    extern f32 float_300_804233a8;
    extern f32 float_neg100_804233ac;
    volatile f32* fifo = (volatile f32*)0xCC008000;
    f32 mtx[3][4];

    PSMTXTrans(mtx, float_0_80423338, float_0_80423338, float_neg1000_804233a0);
    PSMTXConcat((void*)((s32)camGetCurPtr() + 0x11C), mtx, mtx);
    GXLoadPosMtxImm(mtx, 0);
    GXSetCurrentMtx(0);
    GXSetAlphaUpdate(0);
    GXSetColorUpdate(0);
    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 0, 0, 2, 2);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0xFF, 0xFF, 4);
    GXSetTevOp(0, 4);
    GXSetNumTexGens(0);
    GXSetZMode(1, 7, 1);
    GXSetBlendMode(0, 4, 5, 0);
    GXSetZCompLoc(1);
    GXSetAlphaCompare(7, 0, 0, 7, 0);
    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxDesc(9, 1);
    GXBegin(0x80, 0, 4);
    *fifo = float_neg500_804233a4;
    *fifo = float_300_804233a8;
    *fifo = float_0_80423338;
    *fifo = float_0_80423338;
    *fifo = float_300_804233a8;
    *fifo = float_0_80423338;
    *fifo = float_0_80423338;
    *fifo = float_neg100_804233ac;
    *fifo = float_0_80423338;
    *fifo = float_neg500_804233a4;
    *fifo = float_neg100_804233ac;
    *fifo = float_0_80423338;
    GXSetColorUpdate(1);
    return 0;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 compare_func3_r(void* param_1, void* param_2) {
    char bufA[0x80];
    char bufB[0x80];
    void* unitA;
    void* unitB;
    void* ptrB = param_2;
    void* ptrA = param_1;

    unitA = battleGetUnitMonosiriPtr(*(u8*)((s32)ptrA + 1));
    unitB = battleGetUnitMonosiriPtr(*(u8*)((s32)ptrB + 1));
    unk_800d48b0(msgSearch(*(char**)unitA), bufA);
    unk_800d48b0(msgSearch(*(char**)unitB), bufB);
    if (*(u8*)((s32)ptrA + 1) == 0x4D || *(u8*)((s32)ptrA + 1) == 0x87) {
        if (evtGetValue(NULL, 0xF5DE0180) < 0xD2) {
            unk_800d48b0(msgSearch(str_btl_un_hatena_802f4ce0), bufA);
        } else {
            unk_800d48b0(msgSearch(str_btl_un_rampell_802f4cf0), bufA);
        }
    }
    if (*(u8*)((s32)ptrB + 1) == 0x4D || *(u8*)((s32)ptrB + 1) == 0x87) {
        if (evtGetValue(NULL, 0xF5DE0180) < 0xD2) {
            unk_800d48b0(msgSearch(str_btl_un_hatena_802f4ce0), bufB);
        } else {
            unk_800d48b0(msgSearch(str_btl_un_rampell_802f4cf0), bufB);
        }
    }
    return strcmp(bufB, bufA);
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 compare_func3(void* param_1, void* param_2) {
    char bufA[0x80];
    char bufB[0x80];
    void* unitA;
    void* unitB;
    void* ptrB = param_2;
    void* ptrA = param_1;

    unitA = battleGetUnitMonosiriPtr(*(u8*)((s32)ptrA + 1));
    unitB = battleGetUnitMonosiriPtr(*(u8*)((s32)ptrB + 1));
    unk_800d48b0(msgSearch(*(char**)unitA), bufA);
    unk_800d48b0(msgSearch(*(char**)unitB), bufB);
    if (*(u8*)((s32)ptrA + 1) == 0x4D || *(u8*)((s32)ptrA + 1) == 0x87) {
        if (evtGetValue(NULL, 0xF5DE0180) < 0xD2) {
            unk_800d48b0(msgSearch(str_btl_un_hatena_802f4ce0), bufA);
        } else {
            unk_800d48b0(msgSearch(str_btl_un_rampell_802f4cf0), bufA);
        }
    }
    if (*(u8*)((s32)ptrB + 1) == 0x4D || *(u8*)((s32)ptrB + 1) == 0x87) {
        if (evtGetValue(NULL, 0xF5DE0180) < 0xD2) {
            unk_800d48b0(msgSearch(str_btl_un_hatena_802f4ce0), bufB);
        } else {
            unk_800d48b0(msgSearch(str_btl_un_rampell_802f4cf0), bufB);
        }
    }
    return strcmp(bufA, bufB);
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

s32 compare_func2_r(void* param_1, void* param_2) {
    char bufA[0x40];
    char bufB[0x40];
    void* unitA;
    void* unitB;
    char** tblA;
    char** tblB;
    char* nameA;
    char* nameB;
    u32 idxA;
    u32 idxB;

    unitA = battleGetUnitMonosiriPtr(*(u8*)((s32)param_1 + 1));
    unitB = battleGetUnitMonosiriPtr(*(u8*)((s32)param_2 + 1));
    nameA = *(char**)((s32)unitA + 0x14);
    unitA = unitB;
    tblA = tbl_1614;
    tblB = tblA;
    idxA = 0;
    while (1) {
        sprintf(bufA, str_menu_monosiri_PCTs_802f4d00, *tblB);
        if (strcmp(bufA, nameA) == 0) {
            break;
        }
        idxA++;
        tblB++;
        if (idxA >= 0x1A) {
            break;
        }
    }
    if (idxA >= 0x1A) {
        idxA = 0;
    }

    nameB = *(char**)((s32)unitA + 0x14);
    idxB = 0;
    while (1) {
        sprintf(bufB, str_menu_monosiri_PCTs_802f4d00, *tblA);
        if (strcmp(bufB, nameB) == 0) {
            break;
        }
        idxB++;
        tblA++;
        if (idxB >= 0x1A) {
            break;
        }
    }
    if (idxB >= 0x1A) {
        idxB = 0;
    }
    if (idxA < idxB) {
        return 1;
    }
    if (idxA > idxB) {
        return -1;
    }
    return 0;
}


s32 compare_func2(void* param_1, void* param_2) {
    char bufA[0x40];
    char bufB[0x40];
    void* unitA;
    void* unitB;
    char** tblA;
    char** tblB;
    char* nameA;
    char* nameB;
    u32 idxA;
    u32 idxB;

    unitA = battleGetUnitMonosiriPtr(*(u8*)((s32)param_1 + 1));
    unitB = battleGetUnitMonosiriPtr(*(u8*)((s32)param_2 + 1));
    nameA = *(char**)((s32)unitA + 0x14);
    unitA = unitB;
    tblA = tbl_1614;
    tblB = tblA;
    idxA = 0;
    while (1) {
        sprintf(bufA, str_menu_monosiri_PCTs_802f4d00, *tblB);
        if (strcmp(bufA, nameA) == 0) {
            break;
        }
        idxA++;
        tblB++;
        if (idxA >= 0x1A) {
            break;
        }
    }
    if (idxA >= 0x1A) {
        idxA = 0;
    }

    nameB = *(char**)((s32)unitA + 0x14);
    idxB = 0;
    while (1) {
        sprintf(bufB, str_menu_monosiri_PCTs_802f4d00, *tblA);
        if (strcmp(bufB, nameB) == 0) {
            break;
        }
        idxB++;
        tblA++;
        if (idxB >= 0x1A) {
            break;
        }
    }
    if (idxB >= 0x1A) {
        idxB = 0;
    }
    if (idxA > idxB) {
        return 1;
    }
    if (idxA < idxB) {
        return -1;
    }
    return 0;
}


void sort_7_3_func(void* pWin) {
    if (*(s32*)((s32)pWin + 0x180) == 0) {
        qqsort((void*)((s32)pWin + 0x1058), *(s32*)((s32)pWin + 0x1040), 2, compare_func1);
        qqsort((void*)((s32)pWin + 0x1058), *(s32*)((s32)pWin + 0x1040), 2, compare_func3);
    } else {
        qqsort((void*)((s32)pWin + 0x1058), *(s32*)((s32)pWin + 0x1040), 2, compare_func1_r);
        qqsort((void*)((s32)pWin + 0x1058), *(s32*)((s32)pWin + 0x1040), 2, compare_func3_r);
    }
    *(s32*)((s32)pWin + 0x180) = 1 - *(s32*)((s32)pWin + 0x180);
    *(s32*)((s32)pWin + 0x1048) = 0;
    *(s32*)((s32)pWin + 0x104C) = *(s32*)((s32)pWin + 0x1048) / 16;
}


void sort_7_2_func(void* pWin) {
    if (*(s32*)((s32)pWin + 0x180) == 0) {
        qqsort((void*)((s32)pWin + 0x1058), *(s32*)((s32)pWin + 0x1040), 2, compare_func1);
        qqsort((void*)((s32)pWin + 0x1058), *(s32*)((s32)pWin + 0x1040), 2, compare_func2);
    } else {
        qqsort((void*)((s32)pWin + 0x1058), *(s32*)((s32)pWin + 0x1040), 2, compare_func1_r);
        qqsort((void*)((s32)pWin + 0x1058), *(s32*)((s32)pWin + 0x1040), 2, compare_func2_r);
    }
    *(s32*)((s32)pWin + 0x180) = 1 - *(s32*)((s32)pWin + 0x180);
    *(s32*)((s32)pWin + 0x1048) = 0;
    *(s32*)((s32)pWin + 0x104C) = *(s32*)((s32)pWin + 0x1048) / 16;
}


void winMsgEntry(void* pWin, s32 param_2, char* param_3, s32 param_4) {
    u16 width;
    s32 changed = 0;

    if (param_3 != NULL) {
        if (*(char**)((s32)pWin + 0x144) != param_3) {
            changed = 1;
        } else if (strcmp(_tmp_1042, param_3) != 0) {
            changed = 1;
        }

        if (changed != 0) {
            *(s32*)((s32)pWin + 0x138) = param_2;
            *(char**)((s32)pWin + 0x13C) = param_3;
            *(s32*)((s32)pWin + 0x140) = param_4;
            FontGetMessageWidthLine(msgSearch(*(char**)((s32)pWin + 0x13C)), &width);
            *(s32*)((s32)pWin + 0x148) = 0;
            *(s32*)((s32)pWin + 0x14C) = width + 1;
            *(char**)((s32)pWin + 0x144) = param_3;
            strncpy(_tmp_1042, param_3, 0x3C);
        }
    }
}


void sort_6_2_func(void* pWin) {
    if (*(s32*)((s32)pWin + 0x180) == 0) {
        qqsort((void*)((s32)pWin + 0xFBC), *(s32*)((s32)pWin + 0xF94), 2, compare_func6_2);
    } else {
        qqsort((void*)((s32)pWin + 0xFBC), *(s32*)((s32)pWin + 0xF94), 2, compare_func6_2_r);
    }
    *(s32*)((s32)pWin + 0x180) = 1 - *(s32*)((s32)pWin + 0x180);
    *(s32*)((s32)pWin + 0xF9C) = 0;
    *(s32*)((s32)pWin + 0xFA0) = *(s32*)((s32)pWin + 0xF9C) / 28;
}


void sort_6_1_func(void* pWin) {
    if (*(s32*)((s32)pWin + 0x180) == 0) {
        qqsort((void*)((s32)pWin + 0xFBC), *(s32*)((s32)pWin + 0xF94), 2, compare_func6_1);
    } else {
        qqsort((void*)((s32)pWin + 0xFBC), *(s32*)((s32)pWin + 0xF94), 2, compare_func6_1_r);
    }
    *(s32*)((s32)pWin + 0x180) = 1 - *(s32*)((s32)pWin + 0x180);
    *(s32*)((s32)pWin + 0xF9C) = 0;
    *(s32*)((s32)pWin + 0xFA0) = *(s32*)((s32)pWin + 0xF9C) / 28;
}


void sort_5_2_func(void* pWin) {
    if (*(s32*)((s32)pWin + 0x180) == 0) {
        qqsort((void*)((s32)pWin + 0xEBC), *(s32*)((s32)pWin + 0xE94), 2, compare_func5_2);
    } else {
        qqsort((void*)((s32)pWin + 0xEBC), *(s32*)((s32)pWin + 0xE94), 2, compare_func5_2_r);
    }
    *(s32*)((s32)pWin + 0x180) = 1 - *(s32*)((s32)pWin + 0x180);
    *(s32*)((s32)pWin + 0xE9C) = 0;
    *(s32*)((s32)pWin + 0xEA0) = *(s32*)((s32)pWin + 0xE9C) / 28;
}


void sort_5_1_func(void* pWin) {
    if (*(s32*)((s32)pWin + 0x180) == 0) {
        qqsort((void*)((s32)pWin + 0xEBC), *(s32*)((s32)pWin + 0xE94), 2, compare_func5_1);
    } else {
        qqsort((void*)((s32)pWin + 0xEBC), *(s32*)((s32)pWin + 0xE94), 2, compare_func5_1_r);
    }
    *(s32*)((s32)pWin + 0x180) = 1 - *(s32*)((s32)pWin + 0x180);
    *(s32*)((s32)pWin + 0xE9C) = 0;
    *(s32*)((s32)pWin + 0xEA0) = *(s32*)((s32)pWin + 0xE9C) / 28;
}


void sort_8_2_func(void* pWin) {
    if (*(s32*)((s32)pWin + 0x180) == 0) {
        qqsort((void*)((s32)pWin + 0x26E), *(s32*)((s32)pWin + 0x254), 2, compare_func5);
    } else {
        qqsort((void*)((s32)pWin + 0x26E), *(s32*)((s32)pWin + 0x254), 2, compare_func5_r);
    }
    *(s32*)((s32)pWin + 0x180) = 1 - *(s32*)((s32)pWin + 0x180);
    *(s32*)((s32)pWin + 0x258) = 0;
    *(s32*)((s32)pWin + 0x25C) = *(s32*)((s32)pWin + 0x258) / 5;
}


void sort_8_1_func(void* pWin) {
    if (*(s32*)((s32)pWin + 0x180) == 0) {
        qqsort((void*)((s32)pWin + 0x26E), *(s32*)((s32)pWin + 0x254), 2, compare_func4);
    } else {
        qqsort((void*)((s32)pWin + 0x26E), *(s32*)((s32)pWin + 0x254), 2, compare_func4_r);
    }
    *(s32*)((s32)pWin + 0x180) = 1 - *(s32*)((s32)pWin + 0x180);
    *(s32*)((s32)pWin + 0x258) = 0;
    *(s32*)((s32)pWin + 0x25C) = *(s32*)((s32)pWin + 0x258) / 5;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void sort_3_3_func(void* pWin) {
    s32 badge = *(s32*)((s32)pWin + 0x3E4);
    s32 type;
    pouchGetHaveBadgeCnt();
    pouchHaveBadge(badge);
    type = 0xD;
    if (*(s32*)((s32)pWin + 0x180) == 0) {
        type = 6;
    }
    pouchSortItem(type);
    type = 0xD;
    if (*(s32*)((s32)pWin + 0x180) == 0) {
        type = 6;
    }
    pouchSortItem(type);
    *(s32*)((s32)pWin + 0x180) = 1 - *(s32*)((s32)pWin + 0x180);
    winMakeEquipList(pWin);
    *(s32*)((s32)pWin + 0x3E4) = 0;
    *(s32*)((s32)pWin + 0x3EC) = *(s32*)((s32)pWin + 0x3E4) / 8;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void sort_1_2_func(void* pWin) {
    s32 item = *(s32*)((s32)pWin + 0x214);
    s32 type;
    pouchGetHaveItemCnt();
    pouchHaveItem(item);
    type = 8;
    if (*(s32*)((s32)pWin + 0x180) == 0) {
        type = 1;
    }
    pouchSortItem(type);
    *(s32*)((s32)pWin + 0x180) = 1 - *(s32*)((s32)pWin + 0x180);
    winMakeSkipList(pWin);
    *(s32*)((s32)pWin + 0x214) = 0;
    *(s32*)((s32)pWin + 0x21C) = *(s32*)((s32)pWin + 0x214) / 10;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void sort_1_1_func(void* pWin) {
    s32 item = *(s32*)((s32)pWin + 0x214);
    s32 type;
    pouchGetHaveItemCnt();
    pouchHaveItem(item);
    type = 7;
    if (*(s32*)((s32)pWin + 0x180) == 0) {
        type = 0;
    }
    pouchSortItem(type);
    *(s32*)((s32)pWin + 0x180) = 1 - *(s32*)((s32)pWin + 0x180);
    winMakeSkipList(pWin);
    *(s32*)((s32)pWin + 0x214) = 0;
    *(s32*)((s32)pWin + 0x21C) = *(s32*)((s32)pWin + 0x214) / 10;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


void sort_7_1_func(void* pWin) {
    if (*(s32*)((s32)pWin + 0x180) == 0) {
        qqsort((void*)((s32)pWin + 0x1058), *(s32*)((s32)pWin + 0x1040), 2, compare_func1);
    } else {
        qqsort((void*)((s32)pWin + 0x1058), *(s32*)((s32)pWin + 0x1040), 2, compare_func1_r);
    }
    *(s32*)((s32)pWin + 0x180) = 1 - *(s32*)((s32)pWin + 0x180);
    *(s32*)((s32)pWin + 0x1048) = 0;
    *(s32*)((s32)pWin + 0x104C) = *(s32*)((s32)pWin + 0x1048) / 16;
}


void sort_4_2_func(void* pWin) {
    if (*(s32*)((s32)pWin + 0x180) == 0) {
        qqsort((void*)((s32)pWin + 0x404), *(s32*)((s32)pWin + 0xD64), 0xC, N_compare_func4_2);
    } else {
        qqsort((void*)((s32)pWin + 0x404), *(s32*)((s32)pWin + 0xD64), 0xC, N_compare_func4_2_r);
    }
    *(s32*)((s32)pWin + 0x180) = 1 - *(s32*)((s32)pWin + 0x180);
    *(s32*)((s32)pWin + 0x3E8) = 0;
    *(s32*)((s32)pWin + 0x3F0) = *(s32*)((s32)pWin + 0x3E8) / 8;
}


void sort_4_1_func(void* pWin) {
    if (*(s32*)((s32)pWin + 0x180) == 0) {
        qqsort((void*)((s32)pWin + 0x404), *(s32*)((s32)pWin + 0xD64), 0xC, N_compare_func4_1);
    } else {
        qqsort((void*)((s32)pWin + 0x404), *(s32*)((s32)pWin + 0xD64), 0xC, N_compare_func4_1_r);
    }
    *(s32*)((s32)pWin + 0x180) = 1 - *(s32*)((s32)pWin + 0x180);
    *(s32*)((s32)pWin + 0x3E8) = 0;
    *(s32*)((s32)pWin + 0x3F0) = *(s32*)((s32)pWin + 0x3E8) / 8;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 compare_func6_1_r(void* param_1, void* param_2) {
    char bufA[0x80];
    char bufB[0x80];
    s32 idxA = (s16)*(s16*)param_1 + 0xB3;
    s32 idxB = (s16)*(s16*)param_2 + 0xB3;
    char** names = (char**)(itemDataTable + 4);

    unk_800d48b0(msgSearch(names[idxA * 0xA]), bufA);
    unk_800d48b0(msgSearch(names[idxB * 0xA]), bufB);
    return strcmp(bufB, bufA);
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 compare_func6_1(void* param_1, void* param_2) {
    char bufA[0x80];
    char bufB[0x80];
    s32 idxA = (s16)*(s16*)param_1 + 0xB3;
    s32 idxB = (s16)*(s16*)param_2 + 0xB3;
    char** names = (char**)(itemDataTable + 4);

    unk_800d48b0(msgSearch(names[idxA * 0xA]), bufA);
    unk_800d48b0(msgSearch(names[idxB * 0xA]), bufB);
    return strcmp(bufA, bufB);
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 compare_func5_1_r(void* param_1, void* param_2) {
    char bufA[0x80];
    char bufB[0x80];
    s32 idxA = (s16)*(s16*)param_1 + 0xF0;
    s32 idxB = (s16)*(s16*)param_2 + 0xF0;
    char** names = (char**)(itemDataTable + 4);

    unk_800d48b0(msgSearch(names[idxA * 0xA]), bufA);
    unk_800d48b0(msgSearch(names[idxB * 0xA]), bufB);
    return strcmp(bufB, bufA);
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 compare_func5_1(void* param_1, void* param_2) {
    char bufA[0x80];
    char bufB[0x80];
    s32 idxA = (s16)*(s16*)param_1 + 0xF0;
    s32 idxB = (s16)*(s16*)param_2 + 0xF0;
    char** names = (char**)(itemDataTable + 4);

    unk_800d48b0(msgSearch(names[idxA * 0xA]), bufA);
    unk_800d48b0(msgSearch(names[idxB * 0xA]), bufB);
    return strcmp(bufA, bufB);
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void sort_3_2_func(void* pWin) {
    s32 badge = *(s32*)((s32)pWin + 0x3E4);
    s32 type;
    pouchGetHaveBadgeCnt();
    pouchHaveBadge(badge);
    type = 0xC;
    if (*(s32*)((s32)pWin + 0x180) == 0) {
        type = 5;
    }
    pouchSortItem(type);
    *(s32*)((s32)pWin + 0x180) = 1 - *(s32*)((s32)pWin + 0x180);
    winMakeEquipList(pWin);
    *(s32*)((s32)pWin + 0x3E4) = 0;
    *(s32*)((s32)pWin + 0x3EC) = *(s32*)((s32)pWin + 0x3E4) / 8;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void sort_3_1_func(void* pWin) {
    s32 badge = *(s32*)((s32)pWin + 0x3E4);
    s32 type;
    pouchGetHaveBadgeCnt();
    pouchHaveBadge(badge);
    type = 0xB;
    if (*(s32*)((s32)pWin + 0x180) == 0) {
        type = 4;
    }
    pouchSortItem(type);
    *(s32*)((s32)pWin + 0x180) = 1 - *(s32*)((s32)pWin + 0x180);
    winMakeEquipList(pWin);
    *(s32*)((s32)pWin + 0x3E4) = 0;
    *(s32*)((s32)pWin + 0x3EC) = *(s32*)((s32)pWin + 0x3E4) / 8;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


s32 N_compare_func4_1_r(void* param_1, void* param_2) {
    char bufA[0x80];
    char bufB[0x80];
    s32 idxA = *(s32*)((s32)param_1 + 8);
    s32 idxB = *(s32*)((s32)param_2 + 8);
    char** names = (char**)(itemDataTable + 4);

    unk_800d48b0(msgSearch(names[(s16)idxA * 0xA]), bufA);
    unk_800d48b0(msgSearch(names[(s16)idxB * 0xA]), bufB);
    return strcmp(bufB, bufA);
}

s32 N_compare_func4_1(void* param_1, void* param_2) {
    char bufA[0x80];
    char bufB[0x80];
    s32 idxA = *(s32*)((s32)param_1 + 8);
    s32 idxB = *(s32*)((s32)param_2 + 8);
    char** names = (char**)(itemDataTable + 4);

    unk_800d48b0(msgSearch(names[(s16)idxA * 0xA]), bufA);
    unk_800d48b0(msgSearch(names[(s16)idxB * 0xA]), bufB);
    return strcmp(bufA, bufB);
}

u8 sort_2_2_func(void* pWin) {
    return 0;
}


u8 sort_2_1_func(void* pWin) {
    return 0;
}


s32 compare_func4(void* param_1, void* param_2) {
    extern void* mailGetPtr(s32 id);
    void* mailA;
    void* mailB;
    char* msgA;
    char* msgB;

    mailA = mailGetPtr(*(u8*)((s32)param_1 + 1));
    mailB = mailGetPtr(*(u8*)((s32)param_2 + 1));
    msgA = msgSearch(*(char**)((s32)mailA + 8));
    msgB = msgSearch(*(char**)((s32)mailB + 8));
    return strcmp(msgA, msgB);
}

s32 compare_func4_r(void* param_1, void* param_2) {
    extern void* mailGetPtr(s32 id);
    void* mailA;
    void* mailB;
    char* msgA;
    char* msgB;

    mailA = mailGetPtr(*(u8*)((s32)param_1 + 1));
    mailB = mailGetPtr(*(u8*)((s32)param_2 + 1));
    msgA = msgSearch(*(char**)((s32)mailA + 8));
    msgB = msgSearch(*(char**)((s32)mailB + 8));
    return strcmp(msgB, msgA);
}

s32 compare_func6_2_r(void* param_1, void* param_2) {
    s32 idxA = (s16)((s16)*(s16*)param_1 + 0xB3);
    s32 idxB = (s16)((s16)*(s16*)param_2 + 0xB3);
    s16* values = (s16*)(itemDataTable + 0x12);
    s32 valueA = values[idxA * 0x14];
    s32 valueB = values[idxB * 0x14];

    if (valueA < valueB) {
        return 1;
    }
    if (valueA > valueB) {
        return -1;
    }
    return 0;
}

s32 compare_func6_2(void* param_1, void* param_2) {
    s32 idxA = (s16)((s16)*(s16*)param_1 + 0xB3);
    s32 idxB = (s16)((s16)*(s16*)param_2 + 0xB3);
    s16* values = (s16*)(itemDataTable + 0x12);
    s32 valueA = values[idxA * 0x14];
    s32 valueB = values[idxB * 0x14];

    if (valueA > valueB) {
        return 1;
    }
    if (valueA < valueB) {
        return -1;
    }
    return 0;
}

s32 compare_func5_2_r(void* param_1, void* param_2) {
    s32 idxA = (s16)((s16)*(s16*)param_1 + 0xF0);
    s32 idxB = (s16)((s16)*(s16*)param_2 + 0xF0);
    s16* values = (s16*)(itemDataTable + 0x12);
    s32 valueA = values[idxA * 0x14];
    s32 valueB = values[idxB * 0x14];

    if (valueA < valueB) {
        return 1;
    }
    if (valueA > valueB) {
        return -1;
    }
    return 0;
}

s32 compare_func5_2(void* param_1, void* param_2) {
    s32 idxA = (s16)((s16)*(s16*)param_1 + 0xF0);
    s32 idxB = (s16)((s16)*(s16*)param_2 + 0xF0);
    s16* values = (s16*)(itemDataTable + 0x12);
    s32 valueA = values[idxA * 0x14];
    s32 valueB = values[idxB * 0x14];

    if (valueA > valueB) {
        return 1;
    }
    if (valueA < valueB) {
        return -1;
    }
    return 0;
}

s32 N_compare_func4_2_r(void* param_1, void* param_2) {
    s32 idxA = *(s32*)((s32)param_1 + 8);
    s32 idxB = *(s32*)((s32)param_2 + 8);
    s16* values = (s16*)(itemDataTable + 0x12);
    s32 valueA = values[(s16)idxA * 0x14];
    s32 valueB = values[(s16)idxB * 0x14];

    if (valueA < valueB) {
        return 1;
    }
    if (valueA > valueB) {
        return -1;
    }
    return 0;
}

s32 N_compare_func4_2(void* param_1, void* param_2) {
    s32 idxA = *(s32*)((s32)param_1 + 8);
    s32 idxB = *(s32*)((s32)param_2 + 8);
    s16* values = (s16*)(itemDataTable + 0x12);
    s32 valueA = values[(s16)idxA * 0x14];
    s32 valueB = values[(s16)idxB * 0x14];

    if (valueA > valueB) {
        return 1;
    }
    if (valueA < valueB) {
        return -1;
    }
    return 0;
}

u8 winHalfBookGX(double x, double y, void* file, int param_4) {
    return 0;
}

void unk_80152e80(s32 cameraId, void* win) {
    extern void* gp;
    extern void GXSetTexCopySrc(s32 x, s32 y, s32 width, s32 height);
    extern void GXSetTexCopyDst(s32 width, s32 height, s32 format, s32 mipmap);
    extern void GXCopyTex(void* dst, s32 clear);
    extern void GXTexModeSync(void);
    extern void N_mapDispOff(void);
    extern void L_camDispOff(s32 cameraId);
    void* gpWork;

    gpWork = gp;
    GXSetTexCopySrc(0, 0, *(u16*)((s32)gpWork + 0x170), *(u16*)((s32)gpWork + 0x172));
    gpWork = gp;
    GXSetTexCopyDst(*(u16*)((s32)gpWork + 0x170), *(u16*)((s32)gpWork + 0x172), 4, 0);
    GXCopyTex(**(void***)((s32)win + 0x34), 0);
    GXTexModeSync();
    *(s32*)((s32)win + 0x38) = 1;
    N_mapDispOff();
    L_camDispOff(4);
}


/* CHATGPT FALLBACK MISSING STUBS: main/window/win_root 20260624_191429 */

/* fallback stub-fill: map=unk_80152bdc addr=0x80152bdc size=0x000002a4 */
void unk_80152bdc(s32 cameraId, void* win) {
    extern void GXSetFog(s32 type, f32 startz, f32 endz, f32 nearz, f32 farz, void* color);
    extern void GXSetNumChans(s32 count);
    extern void GXSetChanCtrl(s32 chan, s32 enable, s32 ambSrc, s32 matSrc, s32 lightMask, s32 diffFn, s32 attnFn);
    extern void GXSetNumTevStages(s32 count);
    extern void GXSetTevOrder(s32 stage, s32 texCoord, s32 texMap, s32 colorChan);
    extern void GXSetTevOp(s32 stage, s32 mode);
    extern void GXSetNumTexGens(s32 count);
    extern void GXSetTexCoordGen2(s32 texCoord, s32 func, s32 srcParam, s32 mtx, s32 normalize, s32 postMtx);
    extern void GXSetBlendMode(s32 type, s32 srcFact, s32 dstFact, s32 op);
    extern void GXSetZCompLoc(s32 beforeTex);
    extern void GXSetAlphaCompare(s32 comp0, s32 ref0, s32 op, s32 comp1, s32 ref1);
    extern void GXSetZMode(s32 enable, s32 func, s32 update);
    extern void* camGetPtr(s32 cameraId);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXSetCurrentMtx(s32 id);
    extern void GXInitTexObj(void* texObj, void* image, u16 width, u16 height, s32 fmt, s32 wrapS, s32 wrapT, s32 mipmap);
    extern void GXLoadTexObj(void* texObj, s32 mapId);
    extern void GXSetCullMode(s32 mode);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32 attr, s32 type);
    extern void GXSetVtxAttrFmt(s32 vtxFmt, s32 attr, s32 compCnt, s32 compType, s32 frac);
    extern void GXBegin(s32 primitive, s32 vtxFmt, s32 nVerts);
    extern void* gp;
    extern u32 dat_80423334;
    extern f32 float_0_80423338;
    extern f32 float_0p5_8042333c;
    extern f32 float_1_80423340;
    volatile f32* fifo = (volatile f32*)0xCC008000;
    u32 color;
    s32 texObj[8];
    f32 halfW;
    f32 halfH;

    if (*(s32*)((s32)win + 0x38) != 0) {
        color = dat_80423334;
        GXSetFog(0, float_0_80423338, float_0_80423338, float_0_80423338, float_0_80423338, &color);
        GXSetNumChans(0);
        GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2);
        GXSetNumTevStages(1);
        GXSetTevOrder(0, 0, 0, 0xFF);
        GXSetTevOp(0, 3);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
        GXSetBlendMode(1, 4, 5, 0);
        GXSetZCompLoc(1);
        GXSetAlphaCompare(7, 0, 0, 7, 0);
        GXSetZMode(0, 7, 0);
        GXLoadPosMtxImm((void*)((s32)camGetPtr(8) + 0x11C), 0);
        GXSetCurrentMtx(0);
        GXInitTexObj(texObj, **(void***)((s32)win + 0x34), *(u16*)((s32)gp + 0x170), *(u16*)((s32)gp + 0x172), 4, 0, 0, 0);
        GXLoadTexObj(texObj, 0);
        GXSetCullMode(0);
        GXClearVtxDesc();
        GXSetVtxDesc(9, 1);
        GXSetVtxDesc(0xD, 1);
        GXSetVtxAttrFmt(0, 9, 1, 4, 0);
        GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);
        GXBegin(0x80, 0, 4);
        halfW = (f32)*(u16*)((s32)gp + 0x170) * float_0p5_8042333c;
        halfH = (f32)*(u16*)((s32)gp + 0x172) * float_0p5_8042333c;
        *fifo = -halfW;
        *fifo = -halfH;
        *fifo = float_0_80423338;
        *fifo = float_0_80423338;
        *fifo = float_1_80423340;
        *fifo = halfW;
        *fifo = -halfH;
        *fifo = float_0_80423338;
        *fifo = float_1_80423340;
        *fifo = float_1_80423340;
        *fifo = halfW;
        *fifo = halfH;
        *fifo = float_0_80423338;
        *fifo = float_1_80423340;
        *fifo = float_0_80423338;
        *fifo = -halfW;
        *fifo = halfH;
        *fifo = float_0_80423338;
        *fifo = float_0_80423338;
        *fifo = float_0_80423338;
    }
}

