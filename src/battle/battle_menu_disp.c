#include "battle/battle_menu_disp.h"
extern void* _battleWorkPointer;

extern void DrawMainMenu(void);
extern void DrawChangePartyWin(void);
extern void DrawMultiItemWin(void);
extern void DrawOperationWin(void);
extern void DrawWeaponWin(void);

extern void BattleFree(void*);
extern void fileFree(void*);
extern void* fileAlloc(const char*, s32);

extern char* getMarioStDvdRoot(void);
extern int sprintf(char*, const char*, ...);

extern void InitSubMenuCommonProcess(void*, void*, void*);
extern void InitSubMenuCommonProcess2(void*, void*);
extern void InitSubMenuCommonProcess3(void*, void*);

extern void* BattleGetUnitPtr(void*, s32);

extern double cos(double);
extern double sin(double);

extern const char str_PCTs_battle_common_b_802ef810[];

void BattleMenuDisp_ActSelect_Main(void) {
}

void BattleMenuDisp_ChangePartySelect_Main(void) {
}

void BattleMenuDisp_MultiItem_Main(void) {
}

void BattleMenuDisp_Operation_Main(void) {
}

void BattleMenuDisp_WeaponSelect_Main(void) {
}

void BattleMenuDisp_ActSelect_Disp(void) {
    DrawMainMenu();
}

void BattleMenuDisp_ChangePartySelect_Disp(void) {
    DrawChangePartyWin();
}

void BattleMenuDisp_MultiItem_Disp(void) {
    DrawMultiItemWin();
}

void BattleMenuDisp_Operation_Disp(void) {
    DrawOperationWin();
}

void BattleMenuDisp_WeaponSelect_Disp(void) {
    DrawWeaponWin();
}

void battleMenuDispEnd(void) {
    void* battleWork;

    battleWork = _battleWorkPointer;
    fileFree(*(void**)((s32)battleWork + 0x163F8));
}

void BattleMenuDisp_ActSelect_End(void* work) {
    void* ptr;

    ptr = *(void**)((s32)work + 0x1C78);
    if (ptr != 0) {
        BattleFree(ptr);
        *(void**)((s32)work + 0x1C78) = 0;
    }
}

void BattleMenuDisp_ChangePartySelect_End(void* work) {
    void* ptr;

    ptr = *(void**)((s32)work + 0x1C78);
    if (ptr != 0) {
        BattleFree(ptr);
        *(void**)((s32)work + 0x1C78) = 0;
    }
}

void BattleMenuDisp_MultiItem_End(void* work) {
    void* ptr;

    ptr = *(void**)((s32)work + 0x1C78);
    if (ptr != 0) {
        BattleFree(ptr);
        *(void**)((s32)work + 0x1C78) = 0;
    }
}

void BattleMenuDisp_Operation_End(void* work) {
    void* ptr;

    ptr = *(void**)((s32)work + 0x1C78);
    if (ptr != 0) {
        BattleFree(ptr);
        *(void**)((s32)work + 0x1C78) = 0;
    }
}

void BattleMenuDisp_WeaponSelect_End(void* work) {
    void* ptr;

    ptr = *(void**)((s32)work + 0x1C78);
    if (ptr != 0) {
        BattleFree(ptr);
        *(void**)((s32)work + 0x1C78) = 0;
    }
}

s32 BattleMenuKeyOKInACT(void* work) {
    void* ptr;

    if (*(u8*)((s32)work + 0x1C74) == 0) {
        ptr = *(void**)((s32)work + 0x1C78);
        if (*(u8*)((s32)ptr + 0x14) == 0) {
            return 0;
        }
    }

    return 1;
}

void battleMenuDispInit(void) {
    char path[256];
    void* battleWork;

    battleWork = _battleWorkPointer;
    sprintf(path, str_PCTs_battle_common_b_802ef810, getMarioStDvdRoot());
    *(void**)((s32)battleWork + 0x163F8) = fileAlloc(path, 4);
}

void BattleMenuDisp_ChangePartySelect_Init(void* work, s32 flags) {
    void* proc;
    void* common;

    InitSubMenuCommonProcess(&proc, &common, (void*)((s32)work + 0x1C2C));

    if (flags & 1) {
        InitSubMenuCommonProcess3(proc, common);
    } else {
        InitSubMenuCommonProcess2(proc, common);
    }
}

void BattleMenuDisp_MultiItem_Init(void* work, s32 flags) {
    void* proc;
    void* common;

    InitSubMenuCommonProcess(&proc, &common, (void*)((s32)work + 0x1C44));

    if (flags & 1) {
        InitSubMenuCommonProcess2(proc, common);
    } else {
        InitSubMenuCommonProcess3(proc, common);
    }
}

void BattleMenuDisp_WeaponSelect_Init(void* work, void* commonInfo, s32 value, s32 flags) {
    void* proc;
    void* common;

    proc = 0;
    common = 0;
    InitSubMenuCommonProcess(&proc, &common, commonInfo);

    *(s32*)((s32)proc + 0x8) = value;

    if (flags & 1) {
        InitSubMenuCommonProcess2(proc, common);
    } else {
        InitSubMenuCommonProcess3(proc, common);
    }
}

void GetRingOffset(f32* x, f32* y, f32 angle) {
    f32 a;
    f32 value;

    a = 4.7124f - angle;

    value = cos(a);
    *x = 80.0f * value;

    value = sin(a);
    *y = 35.0f * value;
}

void InitSubMenuCommonProcess2(void* proc, void* common) {
    s32 i;
    s32 index;
    s32 count;
    void* entry;

    index = *(s32*)((s32)common + 0x0);
    i = 0;

    while (i < *(s32*)((s32)common + 0x8)) {
        index++;
        count = *(s32*)((s32)common + 0x8);
        if (index >= count) {
            index = 0;
        }

        entry = (void*)((s32)proc + index * 4);
        *(s32*)((s32)entry + 0x48) = 0;
        *(s32*)((s32)entry + 0x84) = i;
        *(s32*)((s32)entry + 0x20) = 0;
        *(s32*)((s32)entry + 0x20) |= 1;

        if (i == *(s32*)((s32)common + 0x8) - 1) {
            *(s32*)((s32)entry + 0x34) = 0;
        } else {
            *(s32*)((s32)entry + 0x34) = 10;
        }

        i++;
    }
}

void InitSubMenuCommonProcess3(void* proc, void* common) {
    s32 i;
    s32 index;
    s32 count;
    void* entry;

    index = *(s32*)((s32)common + 0x0);
    i = 0;

    while (i < *(s32*)((s32)common + 0x8)) {
        index++;
        count = *(s32*)((s32)common + 0x8);
        if (index >= count) {
            index = 0;
        }

        entry = (void*)((s32)proc + index * 4);
        *(s32*)((s32)entry + 0x48) = 0;
        *(s32*)((s32)entry + 0x84) = i;
        *(s32*)((s32)entry + 0x20) = 0;
        *(s32*)((s32)entry + 0x20) |= 1;

        if (i == *(s32*)((s32)common + 0x8) - 1) {
            *(s32*)((s32)entry + 0x34) = 1;
        } else {
            *(s32*)((s32)entry + 0x34) = 11;
        }

        i++;
    }
}

void BattleMenuDisp_Operation_Init(void* work, s32 flags) {
    void* proc;
    void* common;
    void* unit;
    void* info;

    unit = BattleGetUnitPtr(work, *(s32*)((s32)work + 0x420));

    if (*(s32*)((s32)unit + 0x8) == 0xDE) {
        info = (void*)((s32)work + 0x1C14);
    } else {
        info = (void*)((s32)work + 0x1C20);
    }

    InitSubMenuCommonProcess(&proc, &common, info);

    if (flags & 1) {
        InitSubMenuCommonProcess2(proc, common);
    } else {
        InitSubMenuCommonProcess3(proc, common);
    }
}