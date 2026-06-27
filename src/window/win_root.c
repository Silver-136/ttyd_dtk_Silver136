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
    return 0;
}


u8 winMsgMain(void* pWin) {
    return 0;
}


u8 sort_4_3_func(void* pWin) {
    return 0;
}


u8 sort_5_3_func(void* pWin) {
    return 0;
}


u8 winZClear(void) {
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
    return 0;
}


s32 N_compare_func4_1(void* param_1, void* param_2) {
    return 0;
}


u8 sort_2_2_func(void* pWin) {
    return 0;
}


u8 sort_2_1_func(void* pWin) {
    return 0;
}


s32 compare_func4(void* param_1, void* param_2) {
    return 0;
}


s32 compare_func4_r(void* param_1, void* param_2) {
    return 0;
}


s32 compare_func6_2_r(void* param_1, void* param_2) {
    return 0;
}


s32 compare_func6_2(void* param_1, void* param_2) {
    return 0;
}


s32 compare_func5_2_r(void* param_1, void* param_2) {
    return 0;
}


s32 compare_func5_2(void* param_1, void* param_2) {
    return 0;
}


s32 N_compare_func4_2_r(void* param_1, void* param_2) {
    return 0;
}


s32 N_compare_func4_2(void* param_1, void* param_2) {
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
int unk_80152bdc() {
    return 0;
}
