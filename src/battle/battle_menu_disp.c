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
extern void btlGetScreenPoint(f32* pos, f32* out);

extern double cos(double);
extern double sin(double);

extern const char str_PCTs_battle_common_b_802ef810[];

void BattleMenuDisp_ActSelect_Main(void) {
    ;
}

void BattleMenuDisp_ChangePartySelect_Main(void) {
    ;
}

void BattleMenuDisp_MultiItem_Main(void) {
    ;
}

void BattleMenuDisp_Operation_Main(void) {
    ;
}

void BattleMenuDisp_WeaponSelect_Main(void) {
    ;
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
    extern double cos(double);
    extern double sin(double);
    extern f32 float_4p7124_80422540;
    extern f32 float_80_80422530;
    extern f32 float_35_80422504;
    f32 a;
    f32 value;

    a = float_4p7124_80422540 - angle;
    value = cos(a);
    *x = float_80_80422530 * value;
    value = sin(a);
    *y = float_35_80422504 * value;
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
    extern void InitSubMenuCommonProcess(void*, void*, void*);
    extern void InitSubMenuCommonProcess2(void*, void*);
    extern void InitSubMenuCommonProcess3(void*, void*);
    extern void* BattleGetUnitPtr(void*, s32);
    void* proc;
    void* common;
    void* unit;
    void* info;

    unit = BattleGetUnitPtr(work, *(s32*)((s32)work + 0x420));

    if (*(s32*)((s32)unit + 0x8) != 0xDE) {
        goto not_mario;
    }
    info = (void*)((s32)work + 0x1C14);
    goto got_info;
not_mario:
    info = (void*)((s32)work + 0x1C20);
got_info:
    InitSubMenuCommonProcess(&proc, &common, info);

    if (flags & 1) {
        InitSubMenuCommonProcess2(proc, common);
    } else {
        InitSubMenuCommonProcess3(proc, common);
    }
}


void DrawSubIconSub(void* menu, s32 phase, f32 baseMtx[3][4], s32 enabled,
                    u16 iconId, char* name, s32 fpCost, s32 spCost,
                    s32 table, s32 fitText) {
    extern void* _battleWorkPointer;
    extern u8 seleItemCoordCol[];
    extern void* camGetPtr(s32);
    extern void btlDispGXInit2D(void);
    extern void PSMTXTrans(void*, f32, f32, f32);
    extern void PSMTXScale(void*, f32, f32, f32);
    extern void PSMTXConcat(void*, void*, void*);
    extern void GXLoadPosMtxImm(void*, s32);
    extern void TEXGetGXTexObjFromPalette(void*, void*, s32);
    extern void GXLoadTexObj(void*, s32);
    extern void btlDispGXQuads2D(f32, f32, f32, f32, u8, u8, u8, u8);
    extern void iconDispGxCol(void*, s32, u16, u32*);
    extern void FontDrawStart(void);
    extern void FontDrawColor(u32*);
    extern void FontDrawStringMtx(f32 mtx[3][4], char* text);
    extern u16 FontGetMessageWidth(char* text);
    extern char* msgSearch(char* key);
    extern char str_btl_disp_HP_802ef7e8[];
    extern char str_btl_disp_slash_802ef7f4[];
    void* battleWork;
    void* camera;
    f32 trans[3][4];
    f32 scale[3][4];
    f32 draw[3][4];
    u8 texObj[32];
    f32 scroll;
    f32 fraction;
    f32 textScale;
    u32 color;
    u32 alpha;
    u32 shade;
    u8 red;
    u8 green;
    u8 blue;
    s32 base;
    s32 hp;
    s32 maxHp;
    char digits[4];

    battleWork = _battleWorkPointer;
    camera = camGetPtr(8);
    scroll = *(f32*)((s32)menu + 0x98);
    base = (s32)scroll;
    fraction = scroll - (f32)base;
    alpha = 0xFF;
    if (phase == 0) alpha = (u32)(255.0f * (1.0f - fraction));
    if (phase == 6) alpha = (u32)(255.0f * fraction);
    shade = alpha;
    red = seleItemCoordCol[0];
    green = seleItemCoordCol[1];
    blue = seleItemCoordCol[2];
    if (base + phase != *(s32*)*(void**)menu) {
        shade = (u32)(144.0f * (f32)(alpha & 0xFF) / 255.0f);
        red = 0xFF;
        green = 0xFF;
        blue = 0xFF;
    }

    btlDispGXInit2D();
    PSMTXConcat((void*)((s32)camera + 0x11C), baseMtx, draw);
    GXLoadPosMtxImm(draw, 0);
    TEXGetGXTexObjFromPalette(**(void***)((s32)*(void**)((s32)battleWork + 0x163F8) + 0xA0), texObj, 2);
    GXLoadTexObj(texObj, 0);
    btlDispGXQuads2D(-152.0f, 24.0f, -136.0f, 0.0f, red, green, blue, shade);
    btlDispGXQuads2D(152.0f, 24.0f, 136.0f, 0.0f, red, green, blue, shade);
    TEXGetGXTexObjFromPalette(**(void***)((s32)*(void**)((s32)battleWork + 0x163F8) + 0xA0), texObj, 3);
    GXLoadTexObj(texObj, 0);
    btlDispGXQuads2D(-136.0f, 24.0f, 136.0f, 0.0f, red, green, blue, shade);

    PSMTXTrans(trans, -119.0f, 2.0f, 0.0f);
    PSMTXScale(scale, 0.5f, 0.5f, 1.0f);
    PSMTXConcat(baseMtx, trans, draw);
    PSMTXConcat(draw, scale, draw);
    if (iconId == 0) iconId = 0x193;
    color = enabled == 1 ? (0xFFFFFF00 | (alpha & 0xFF)) : (0x80808000 | (alpha & 0xFF));
    iconDispGxCol(draw, enabled == 1 ? 0x10 : 0x30, iconId, &color);

    FontDrawStart();
    color = enabled == 1 ? (0x00000000 | (alpha & 0xFF)) : (0x80808000 | (alpha & 0xFF));
    FontDrawColor(&color);
    PSMTXTrans(trans, -104.0f, 22.0f, 0.0f);
    textScale = 0.8f;
    if (fitText == 1 && FontGetMessageWidth(name) > 150) {
        textScale = 120.0f / FontGetMessageWidth(name);
    }
    PSMTXScale(scale, textScale, 0.8f, 0.8f);
    PSMTXConcat(baseMtx, trans, draw);
    PSMTXConcat(draw, scale, draw);
    FontDrawStringMtx(draw, name);

    if (fpCost != 0) {
        digits[0] = '0' + (fpCost / 100) % 10;
        digits[1] = '0' + (fpCost / 10) % 10;
        digits[2] = '0' + fpCost % 10;
        digits[3] = 0;
        PSMTXTrans(trans, 76.0f, 25.0f, 0.0f);
        PSMTXScale(scale, 1.0f, 1.0f, 1.0f);
        PSMTXConcat(baseMtx, trans, draw);
        PSMTXConcat(draw, scale, draw);
        FontDrawStringMtx(draw, digits);
    }
    if (spCost != 0) {
        digits[0] = '0' + (spCost / 100) % 10;
        digits[1] = '0' + (spCost / 10) % 10;
        digits[2] = '0' + spCost % 10;
        digits[3] = 0;
        PSMTXTrans(trans, 76.0f, 25.0f, 0.0f);
        PSMTXConcat(baseMtx, trans, draw);
        FontDrawStringMtx(draw, digits);
    }
    if (table != 0) {
        table += (base + phase) * 0x18;
        hp = *(s16*)(table + 0x14);
        maxHp = *(s16*)(table + 0x16);
        PSMTXTrans(trans, 30.0f, 22.0f, 0.0f);
        PSMTXScale(scale, 0.8f, 0.8f, 0.8f);
        PSMTXConcat(baseMtx, trans, draw);
        PSMTXConcat(draw, scale, draw);
        FontDrawStringMtx(draw, msgSearch(str_btl_disp_HP_802ef7e8));

        digits[0] = '0' + (hp / 100) % 10;
        digits[1] = '0' + (hp / 10) % 10;
        digits[2] = '0' + hp % 10;
        digits[3] = 0;
        PSMTXTrans(trans, 56.0f, 25.0f, 0.0f);
        PSMTXScale(scale, 1.0f, 1.0f, 1.0f);
        PSMTXConcat(baseMtx, trans, draw);
        PSMTXConcat(draw, scale, draw);
        FontDrawStringMtx(draw, digits);

        PSMTXTrans(trans, 92.0f, 22.0f, 0.0f);
        PSMTXScale(scale, 0.8f, 0.8f, 0.8f);
        PSMTXConcat(baseMtx, trans, draw);
        PSMTXConcat(draw, scale, draw);
        FontDrawStringMtx(draw, msgSearch(str_btl_disp_slash_802ef7f4));

        digits[0] = '0' + (maxHp / 100) % 10;
        digits[1] = '0' + (maxHp / 10) % 10;
        digits[2] = '0' + maxHp % 10;
        digits[3] = 0;
        PSMTXTrans(trans, 106.0f, 25.0f, 0.0f);
        PSMTXScale(scale, 1.0f, 1.0f, 1.0f);
        PSMTXConcat(baseMtx, trans, draw);
        PSMTXConcat(draw, scale, draw);
        FontDrawStringMtx(draw, digits);
    }
}

void DrawMainMenu(void) {
    extern void* _battleWorkPointer;
    extern void* gp;
    extern void camGetPtr(s32);
    extern void GetRingCenter(f32*);
    extern void GetRingOffset(f32, f32*, f32*);
    extern double intplGetValue(double, double, s32, s32, s32);
    extern void PSMTXRotRad(void*, char, f32);
    extern void PSMTXTrans(void*, f32, f32, f32);
    extern void PSMTXScale(void*, f32, f32, f32);
    extern void PSMTXConcat(void*, void*, void*);
    extern void DrawMainIcon(s32, s32, s32, s32, s32);
    extern void DrawMenuPartyChangeButton(void);
    extern void DrawMenuPinchMark(void);
    void* window;
    void* cursor;
    f32 matrices[5][3][4];
    f32 positions[5][3];
    f32 angles[5];
    f32 rot[3][4];
    f32 trans[3][4];
    f32 scale[3][4];
    f32 offsetX;
    f32 offsetY;
    f32 targetAngle;
    f32 currentAngle;
    f32 value;
    s32 count;
    s32 current;
    s32 previous;
    s32 delta;
    s32 duration;
    s32 settled;
    s32 state;
    s32 i;

    window = *(void**)((s32)_battleWorkPointer + 0x1C78);
    cursor = *(void**)window;
    camGetPtr(8);
    state = *(s32*)((s32)window + 0xC);
    if (state == 2 || state == 0) {
        settled = 0;
        count = *(s32*)((s32)cursor + 8);
        for (i = 0; i < count; i++) {
            if (*(s32*)((s32)window + 0x34 + i * 4) == 3) settled++;
        }
        if (settled == count) *(s32*)((s32)window + 0xC) = 1;
    } else if (state == 1) {
        *(u8*)((s32)window + 0x14) = 1;
        current = *(s32*)cursor;
        previous = *(s32*)((s32)window + 0x18);
        count = *(s32*)((s32)cursor + 8);
        delta = current - previous;
        if (delta == 1 || delta == -(count - 1)) {
            *(s32*)((s32)window + 0xC) = 2;
            *(u8*)((s32)window + 0x14) = 0;
            *(s32*)((s32)window + 0x18) = current;
            for (i = 0; i < count; i++) {
                *(s32*)((s32)window + 0x34 + i * 4) = 4;
                *(s32*)((s32)window + 0x48 + i * 4) = 0;
                *(s32*)((s32)window + 0x70 + i * 4) =
                    *(s32*)((s32)window + 0x84 + i * 4);
                if (--*(s32*)((s32)window + 0x84 + i * 4) == -1)
                    *(s32*)((s32)window + 0x84 + i * 4) = count - 1;
            }
        } else if (delta == -1 || delta == count - 1) {
            *(s32*)((s32)window + 0xC) = 2;
            *(u8*)((s32)window + 0x14) = 0;
            *(s32*)((s32)window + 0x18) = current;
            for (i = 0; i < count; i++) {
                *(s32*)((s32)window + 0x34 + i * 4) = 5;
                *(s32*)((s32)window + 0x48 + i * 4) = 0;
                *(s32*)((s32)window + 0x70 + i * 4) =
                    *(s32*)((s32)window + 0x84 + i * 4);
                if (++*(s32*)((s32)window + 0x84 + i * 4) == count)
                    *(s32*)((s32)window + 0x84 + i * 4) = 0;
            }
        }
    } else if (state == 10) {
        *(s32*)((s32)window + 0xC) = 1;
        *(u8*)((s32)window + 0x14) = 1;
    }

    *(s32*)((s32)window + 0x1C) = 0;
    count = *(s32*)((s32)cursor + 8);
    duration = *(s32*)((s32)gp + 4) * 12 / 60;
    for (i = 0; i < count; i++) {
        state = *(s32*)((s32)window + 0x34 + i * 4);
        switch (state) {
        case 0:
            if (--*(s32*)((s32)window + 0x48 + i * 4) < 1) {
                *(s32*)((s32)window + 0x34 + i * 4) = 1;
                *(s32*)((s32)window + 0x48 + i * 4) = 0;
            }
            break;
        case 1:
            *(u32*)((s32)window + 0x20 + i * 4) |= 1;
            targetAngle = 3.1416f * *(f32*)(0x8036359C + count * 20 +
                *(s32*)((s32)window + 0x84 + i * 4) * 4) / 180.0f;
            currentAngle = *(s32*)((s32)window + 0x48 + i * 4) *
                           (12.0f / *(s32*)((s32)gp + 4));
            if (currentAngle > targetAngle) currentAngle = targetAngle;
            GetRingCenter(positions[i]);
            GetRingOffset(currentAngle, &offsetX, &offsetY);
            positions[i][0] += offsetX;
            positions[i][1] += offsetY;
            positions[i][2] = offsetY;
            value = 270.0f + 90.0f * *(s32*)((s32)window + 0x48 + i * 4) /
                    (*(s32*)((s32)gp + 4) * 10 / 60);
            if (value >= 360.0f) value = 0.0f;
            PSMTXRotRad(rot, 'x', value * 0.01745329f);
            PSMTXTrans(trans, positions[i][0] + (i == *(s32*)cursor ? 20.0f : 0.0f),
                       positions[i][1], positions[i][2]);
            PSMTXConcat(trans, rot, matrices[i]);
            if (i == *(s32*)cursor) *(u32*)((s32)window + 0x20 + i * 4) |= 2;
            angles[i] = 0.0f;
            if (currentAngle >= targetAngle &&
                *(s32*)((s32)window + 0x48 + i * 4) >= *(s32*)((s32)gp + 4) * 10 / 60) {
                *(s32*)((s32)window + 0x34 + i * 4) = 3;
                *(s32*)((s32)window + 0x48 + i * 4) = -1;
            }
            *(s32*)((s32)window + 0x48 + i * 4) += 1;
            break;
        case 3:
            targetAngle = 3.1416f * *(f32*)(0x8036359C + count * 20 +
                *(s32*)((s32)window + 0x84 + i * 4) * 4) / 180.0f;
            GetRingCenter(positions[i]);
            GetRingOffset(targetAngle, &offsetX, &offsetY);
            positions[i][0] += offsetX;
            positions[i][1] += offsetY;
            positions[i][2] = offsetY;
            PSMTXTrans(matrices[i], positions[i][0] + (i == *(s32*)cursor ? 20.0f : 0.0f),
                       positions[i][1], positions[i][2]);
            if (i == *(s32*)cursor) *(u32*)((s32)window + 0x20 + i * 4) |= 2;
            angles[i] = 0.0f;
            break;
        case 10:
            *(u32*)((s32)window + 0x20 + i * 4) |= 3;
            GetRingCenter(positions[i]);
            GetRingOffset(0.0f, &offsetX, &offsetY);
            positions[i][0] += offsetX;
            positions[i][1] += offsetY;
            positions[i][2] = offsetY;
            positions[i][0] = (f32)intplGetValue(-60.0, positions[i][0], 4,
                *(s32*)((s32)window + 0x48 + i * 4), duration);
            positions[i][1] = (f32)intplGetValue(55.0, positions[i][1], 4,
                *(s32*)((s32)window + 0x48 + i * 4), duration);
            PSMTXTrans(matrices[i], positions[i][0] + 20.0f,
                       positions[i][1], positions[i][2]);
            angles[i] = (f32)intplGetValue(360.0, 0.0, 0,
                *(s32*)((s32)window + 0x48 + i * 4), duration);
            if (++*(s32*)((s32)window + 0x48 + i * 4) > duration)
                *(s32*)((s32)window + 0x34 + i * 4) = 3;
            break;
        case 11:
            *(u32*)((s32)window + 0x20 + i * 4) |= 1;
            *(u32*)((s32)window + 0x20 + i * 4) &= ~2;
            targetAngle = 3.1416f * *(f32*)(0x8036359C + count * 20 +
                *(s32*)((s32)window + 0x84 + i * 4) * 4) / 180.0f;
            GetRingCenter(positions[i]);
            GetRingOffset(targetAngle, &offsetX, &offsetY);
            positions[i][0] += offsetX;
            positions[i][1] += offsetY;
            positions[i][2] = offsetY;
            value = 0.1f * *(s32*)((s32)window + 0x48 + i * 4);
            if (value >= 1.0f) *(s32*)((s32)window + 0x34 + i * 4) = 3;
            PSMTXScale(scale, value, value, 1.0f);
            PSMTXTrans(trans, positions[i][0], positions[i][1], positions[i][2]);
            PSMTXConcat(trans, scale, matrices[i]);
            angles[i] = 0.0f;
            *(s32*)((s32)window + 0x48 + i * 4) += 1;
            break;
        }
    }
    DrawMainIcon((s32)window, *(s32*)((s32)window + 4), (s32)matrices,
                 (s32)positions, (s32)angles);
    DrawMenuPartyChangeButton();
    DrawMenuPinchMark();
}

void DrawMainIconCircle(f32 angle, s32 unused, void* iconInfo, f32 mtx[3][4], void* animInfo) {
    typedef f32 Mtx[3][4];

    extern void* _battleWorkPointer;
    extern f32 float_0_8042242c;
    extern f32 float_1_8042245c;
    extern f32 float_1p3_80422508;
    extern f32 float_1p5_804224a4;
    extern f32 float_4_80422490;
    extern f32 float_7p5_8042251c;
    extern f32 float_8_80422510;
    extern f32 float_180_80422438;
    extern f32 float_360_80422444;
    extern f32 float_deg2rad_80422448;
    extern f32 float_neg4_80422514;
    extern f32 float_neg3p0769_80422518;
    extern f32 float_0p005_8042250c;
    extern f32 float_0p66_80422520;
    extern u32 dat_8042240c;
    extern u8 dat_80422410;
    extern u8 dat_80422414;

    extern void* camGetPtr(s32 cameraId);
    extern void btlDispGXInit2D(void);
    extern void GXSetTevColor(s32 stage, void* color);
    extern void GXSetNumTevStages(s32 count);
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void PSMTXScale(Mtx m, f32 x, f32 y, f32 z);
    extern void PSMTXTrans(Mtx m, f32 x, f32 y, f32 z);
    extern void PSMTXRotRad(Mtx m, s32 axis, f32 rad);
    extern void PSMTXIdentity(Mtx m);
    extern void PSMTXConcat(Mtx a, Mtx b, Mtx out);
    extern void GXLoadPosMtxImm(Mtx m, s32 id);
    extern void TEXGetGXTexObjFromPalette(void* palette, void* texObj, s32 id);
    extern void GXLoadTexObj(void* texObj, s32 id);
    extern void GXBegin(s32 primitive, s32 vat, s32 vtxCount);
    extern void btlDispGXPoint2D(f32 x, f32 y, f32 u, f32 v, u8 r, u8 g, u8 b, u8 a);
    extern void iconDispGxCol(Mtx m, s32 flags, u16 iconId, void* color);

    void* battleWork = _battleWorkPointer;
    void* cam = camGetPtr(8);
    Mtx drawMtx;
    Mtx transMtx;
    Mtx scaleMtx;
    Mtx rotMtx;
    u32 texObj[8];
    u32 color;
    u8 shade;
    s32 fade;
    u16 iconId;
    f32 scale;
    f32 offset;
    f32 rad;
    s32 i;

    fade = (s32)(float_1p5_804224a4 * *(f32*)((s32)animInfo + 8));
    shade = (u8)(0xC0 - fade);

    btlDispGXInit2D();
    color = dat_8042240c;
    GXSetTevColor(1, &color);
    GXSetNumTevStages(1);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0xF, 0xA, 2, 0xF);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaIn(0, 7, 5, 4, 7);
    GXSetTevOrder(0, 0, 0, 4);

    scale = float_1p3_80422508 - (float_0p005_8042250c * *(f32*)((s32)animInfo + 8));
    PSMTXScale(scaleMtx, scale, scale, float_1_8042245c);
    rad = float_deg2rad_80422448 * angle;
    PSMTXRotRad(rotMtx, 0x79, rad);
    PSMTXConcat(mtx, rotMtx, drawMtx);
    PSMTXConcat(drawMtx, scaleMtx, drawMtx);
    PSMTXConcat((void*)((s32)cam + 0x11C), drawMtx, drawMtx);
    GXLoadPosMtxImm(drawMtx, 0);

    TEXGetGXTexObjFromPalette(**(void***)((s32)*(void**)((s32)battleWork + 0x163F8) + 0xA0), texObj, 0);
    GXLoadTexObj(texObj, 0);

    offset = float_neg3p0769_80422518;
    if (angle >= float_0_8042242c && angle <= float_180_80422438) {
        offset = (float_4_80422490 - ((float_8_80422510 * angle) / float_180_80422438)) / float_1p3_80422508;
    }
    if (angle >= float_180_80422438 && angle <= float_360_80422444) {
        offset = (float_neg4_80422514 + ((float_8_80422510 * (angle - float_180_80422438)) / float_180_80422438)) / float_1p3_80422508;
    }

    for (i = 0; i < 4; i++) {
        GXBegin(0x80, 0, 4);
        btlDispGXPoint2D(offset - 8.0f, float_neg3p0769_80422518 - 8.0f, 0.0f, 0.0f, 0, 0, 0, 0x80);
        btlDispGXPoint2D(offset + 8.0f, float_neg3p0769_80422518 - 8.0f, 1.0f, 0.0f, 0, 0, 0, 0x80);
        btlDispGXPoint2D(offset + 8.0f, float_neg3p0769_80422518 + 8.0f, 1.0f, 1.0f, 0, 0, 0, 0x80);
        btlDispGXPoint2D(offset - 8.0f, float_neg3p0769_80422518 + 8.0f, 0.0f, 1.0f, 0, 0, 0, 0x80);
    }

    for (i = 0; i < 4; i++) {
        GXBegin(0x80, 0, 4);
        btlDispGXPoint2D(-8.0f, -8.0f, 0.0f, 0.0f, shade, shade, shade, 0xFF);
        btlDispGXPoint2D(8.0f, -8.0f, 1.0f, 0.0f, shade, shade, shade, 0xFF);
        btlDispGXPoint2D(8.0f, 8.0f, 1.0f, 1.0f, shade, shade, shade, 0xFF);
        btlDispGXPoint2D(-8.0f, 8.0f, 0.0f, 1.0f, shade, shade, shade, 0xFF);
    }

    PSMTXTrans(transMtx, float_0_8042242c, float_7p5_8042251c, float_0_8042242c);
    scale = float_0p66_80422520 * (float_1p3_80422508 - (float_0p005_8042250c * *(f32*)((s32)animInfo + 8)));
    PSMTXScale(scaleMtx, scale, scale, float_1_8042245c);
    PSMTXRotRad(rotMtx, 0x79, rad);
    PSMTXIdentity(rotMtx);
    PSMTXConcat(mtx, rotMtx, drawMtx);
    PSMTXConcat(drawMtx, scaleMtx, drawMtx);
    PSMTXConcat(drawMtx, transMtx, drawMtx);

    iconId = *(u16*)((s32)iconInfo + 0xC);
    if (iconId == 0) {
        iconId = 0x193;
    }
    if (*(s32*)((s32)iconInfo + 4) == 1) {
        color = ((u32)shade << 24) | ((u32)shade << 16) | ((u32)shade << 8) | dat_80422410;
        iconDispGxCol(drawMtx, 0x10, iconId, &color);
    } else {
        color = ((u32)shade << 24) | ((u32)shade << 16) | ((u32)shade << 8) | dat_80422414;
        iconDispGxCol(drawMtx, 0x30, iconId, &color);
    }
}

void DrawMainIconHukidasi(f32 angle, void* bubbleInfo, void* iconInfo, f32 mtx[3][4]) {
    typedef f32 Mtx[3][4];

    extern void* _battleWorkPointer;
    extern f32 float_0_8042242c;
    extern f32 float_1_8042245c;
    extern f32 float_4_80422490;
    extern f32 float_8_80422510;
    extern f32 float_20_8042244c;
    extern f32 float_64_80422528;
    extern f32 float_76_804224fc;
    extern f32 float_80_80422530;
    extern f32 float_90_80422440;
    extern f32 float_180_80422438;
    extern f32 float_360_80422444;
    extern f32 float_deg2rad_80422448;
    extern f32 float_neg4_80422514;
    extern f32 float_neg5_80422524;
    extern f32 float_neg64_8042252c;
    extern f32 float_neg67_80422534;
    extern f32 float_52p5_80422538;
    extern f32 float_0p9_8042253c;
    extern u32 dat_80422400;
    extern u32 dat_80422404;
    extern u32 dat_80422408;

    extern void* camGetPtr(s32 cameraId);
    extern void btlDispGXInit2D(void);
    extern void GXSetTevColor(s32 stage, void* color);
    extern void GXSetNumTevStages(s32 count);
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void PSMTXScale(Mtx m, f32 x, f32 y, f32 z);
    extern void PSMTXTrans(Mtx m, f32 x, f32 y, f32 z);
    extern void PSMTXRotRad(Mtx m, s32 axis, f32 rad);
    extern void PSMTXConcat(Mtx a, Mtx b, Mtx out);
    extern void GXLoadPosMtxImm(Mtx m, s32 id);
    extern void TEXGetGXTexObjFromPalette(void* palette, void* texObj, s32 id);
    extern void GXLoadTexObj(void* texObj, s32 id);
    extern void btlDispGXQuads2D(f32 x1, f32 y1, f32 x2, f32 y2, u8 r, u8 g, u8 b, u8 a);
    extern void iconDispGx2(Mtx m, s32 flags, u16 iconId);
    extern u32 FontGetMessageWidth(char* msg);
    extern void FontDrawStart(void);
    extern void FontDrawColor(void* color);
    extern void FontDrawStringCenterMtx(Mtx m, char* msg);

    void* battleWork = _battleWorkPointer;
    void* cam = camGetPtr(8);
    Mtx drawMtx;
    Mtx transMtx;
    Mtx scaleMtx;
    Mtx rotMtx;
    u32 texObj[8];
    u32 color;
    char* msg;
    u16 iconId;
    f32 offset;
    f32 rad;
    f32 width;
    f32 scale;

    btlDispGXInit2D();
    color = dat_80422400;
    GXSetTevColor(1, &color);
    GXSetNumTevStages(1);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0xF, 0xA, 2, 0xF);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaIn(0, 7, 5, 4, 7);
    GXSetTevOrder(0, 0, 0, 4);

    PSMTXScale(scaleMtx, float_1_8042245c, float_1_8042245c, float_1_8042245c);
    PSMTXTrans(transMtx, float_0_8042242c, float_neg5_80422524, float_0_8042242c);
    rad = float_deg2rad_80422448 * angle;
    PSMTXRotRad(rotMtx, 0x79, rad);
    PSMTXConcat(mtx, rotMtx, drawMtx);
    PSMTXConcat(drawMtx, scaleMtx, drawMtx);
    PSMTXConcat(drawMtx, transMtx, drawMtx);
    PSMTXConcat((void*)((s32)cam + 0x11C), drawMtx, drawMtx);
    GXLoadPosMtxImm(drawMtx, 0);

    TEXGetGXTexObjFromPalette(**(void***)((s32)*(void**)((s32)battleWork + 0x163F8) + 0xA0), texObj, 1);
    GXLoadTexObj(texObj, 0);

    if (angle >= float_0_8042242c && angle <= float_180_80422438) {
        offset = float_4_80422490 - ((float_8_80422510 * angle) / float_180_80422438);
    }
    if (angle >= float_180_80422438 && angle <= float_360_80422444) {
        offset = float_neg4_80422514 + ((float_8_80422510 * (angle - float_180_80422438)) / float_180_80422438);
    }

    if (*(s32*)((s32)bubbleInfo + 0x1C) == 0) {
        btlDispGXQuads2D(float_64_80422528 + offset, float_76_804224fc,
                         float_neg64_8042252c + offset, float_neg4_80422514, 0, 0, 0, 0x80);
        btlDispGXQuads2D(float_64_80422528, float_80_80422530,
                         float_neg64_8042252c, float_0_8042242c, 0xFF, 0xFF, 0xFF, 0xFF);
    } else {
        btlDispGXQuads2D(float_neg64_8042252c + offset, float_76_804224fc,
                         float_64_80422528 + offset, float_neg4_80422514, 0, 0, 0, 0x80);
        btlDispGXQuads2D(float_neg64_8042252c, float_80_80422530,
                         float_64_80422528, float_0_8042242c, 0xFF, 0xFF, 0xFF, 0xFF);
    }

    PSMTXTrans(transMtx, float_neg67_80422534, float_20_8042244c, float_0_8042242c);
    PSMTXScale(scaleMtx, float_1_8042245c, float_1_8042245c, float_1_8042245c);
    PSMTXRotRad(rotMtx, 0x79, rad);
    PSMTXConcat(mtx, rotMtx, drawMtx);
    PSMTXConcat(drawMtx, transMtx, drawMtx);
    PSMTXConcat(drawMtx, scaleMtx, drawMtx);

    iconId = *(u16*)((s32)iconInfo + 0xC);
    if (iconId == 0) {
        iconId = 0x193;
    }
    if (*(s32*)((s32)iconInfo + 4) == 1) {
        iconDispGx2(drawMtx, 0x10, iconId);
    } else {
        iconDispGx2(drawMtx, 0x30, iconId);
    }

    msg = *(char**)((s32)iconInfo + 8);
    PSMTXTrans(transMtx, float_0_8042242c, float_52p5_80422538, float_0_8042242c);
    width = (f32)(FontGetMessageWidth(msg) & 0xFFFF);
    scale = float_0p9_8042253c;
    if (width > float_90_80422440) {
        scale = float_0p9_8042253c * (float_90_80422440 / width);
    }
    PSMTXScale(scaleMtx, scale, float_0p9_8042253c, float_0p9_8042253c);
    PSMTXRotRad(rotMtx, 0x79, rad);
    PSMTXConcat(mtx, rotMtx, drawMtx);
    PSMTXConcat(drawMtx, transMtx, drawMtx);
    PSMTXConcat(drawMtx, scaleMtx, drawMtx);
    FontDrawStart();
    if (*(s32*)((s32)iconInfo + 4) == 1) {
        color = dat_80422404;
    } else {
        color = dat_80422408;
    }
    FontDrawColor(&color);
    FontDrawStringCenterMtx(drawMtx, msg);
}

void DrawWeaponWin(void) {
    extern void DrawSubMenuCommonProcess(f32*, f32*, void*, s32, s32, void*);
    extern void DrawSubIcon(void*, void*, u32*, u16*, char**, u32*, u32*, void*, s32);
    extern u8 DrawMainIcon(s32, s32, s32, s32, s32);
    extern u8 DrawMenuCursorAndScrollArrow(f32*);
    extern u32 BtlUnit_GetWeaponCost(void*, void*);
    extern f32 vec3_802ef750[3];
    void* battleWork = _battleWorkPointer;
    u8* window = *(u8**)((s32)battleWork + 0x1C78);
    s32* cursor = *(s32**)window;
    u8* weaponTable = (u8*)battleWork + 0x171C;
    u8* entries = *(u8**)(window + 0x8);
    void* unit = BattleGetUnitPtr(battleWork, *(s32*)((s32)battleWork + 0x420));
    f32 outY, outX, cursorPos[3];
    u16 icons[8];
    u32 spCosts[7], costs[7];
    char* names[7];
    u32 enabled[7];
    u8 angles[20], positions[60], mainMatrices[5 * 48], subMatrices[7 * 48];
    s32 i, index;
    u8* entry;
    void* weapon;

    DrawSubMenuCommonProcess(&outX, &outY, mainMatrices, (s32)positions, (s32)angles, subMatrices);
    for (i = 0; i < 7; i++) {
        index = (s32)*(f32*)(window + 0x98) + i;
        if (index < cursor[2]) {
            entry = entries + index * 0x1C;
            weapon = *(void**)entry;
            enabled[i] = *(u32*)(entry + 0x4);
            icons[i] = *(u16*)(weaponTable + index * 0x1C + 0x8C);
            names[i] = *(char**)(entry + 0x8);
            costs[i] = weapon != NULL ? BtlUnit_GetWeaponCost(unit, weapon) : 0;
            spCosts[i] = weapon != NULL ? *(u8*)((s32)weapon + 0x12) : 0;
        }
    }
    DrawMainIcon((s32)window, (s32)battleWork + 0x1724, (s32)mainMatrices, (s32)positions, (s32)angles);
    DrawSubIcon(window, subMatrices, enabled, icons, names, costs, spCosts, NULL, 0);
    cursorPos[0] = vec3_802ef750[0]; cursorPos[1] = vec3_802ef750[1]; cursorPos[2] = vec3_802ef750[2];
    DrawMenuCursorAndScrollArrow(cursorPos);
}

void N_DrawMenuPartyPinchMark(void) {
    extern void* _battleWorkPointer;
    extern void* gp;
    extern u16 danger_icon_tbl[];
    extern u16 peril_icon_tbl[];
    extern void* camGetPtr(s32 id);
    extern void* BattleGetPartyPtr(void*);
    extern void TEXGetGXTexObjFromPalette(void*, void*, s32);
    extern void GXLoadTexObj(void*, s32);
    extern void btlGetScreenPoint(f32*, f32*);
    extern void PSMTXTrans(void*, f32, f32, f32);
    extern void PSMTXConcat(void*, void*, void*);
    extern void PSMTXScale(void*, f32, f32, f32);
    extern void GXLoadPosMtxImm(void*, s32);
    extern void btlDispGXQuads2D(f32, f32, f32, f32, u8, u8, u8, u8);
    void* battleWork;
    void* camera;
    void* party;
    u8 texObj[32];
    f32 screen[3];
    f32 trans[3][4];
    f32 model[3][4];
    f32 scale[3][4];
    s32 icon;
    u8 red;
    u8 green;
    u8 blue;
    s16 hp;

    battleWork = _battleWorkPointer;
    camera = camGetPtr(8);
    party = BattleGetPartyPtr(battleWork);
    if (party == 0) return;
    hp = *(s16*)((s32)party + 0x10C);
    if (hp <= 0 || hp >= *(s16*)((s32)party + 0x108) || hp >= 6) return;
    if (hp == 1) {
        icon = peril_icon_tbl[*(s32*)((s32)gp + 0x16C)];
        if (*(u32*)((s32)gp + 0x1C) % 50 < 25) {
            red = 0x40; green = 0xE9; blue = 0xFF;
        } else {
            red = 0x2F; green = 0x52; blue = 0xE9;
        }
    } else {
        red = 0x40; green = 0xE9; blue = 0xFF;
        icon = danger_icon_tbl[*(s32*)((s32)gp + 0x16C)];
    }
    TEXGetGXTexObjFromPalette(**(void***)((s32)*(void**)((s32)battleWork + 0x163F8) + 0xA0), texObj, icon);
    GXLoadTexObj(texObj, 0);
    btlGetScreenPoint((f32*)((s32)party + 0x3C), screen);
    PSMTXTrans(trans, screen[0] - 20.0f, screen[1] + 60.0f,
               *(f32*)((s32)party + 0x3C) <= -120.0f ? 0.0f : 10.0f);
    PSMTXConcat((void*)((s32)camera + 0x11C), trans, model);
    if (*(s32*)((s32)gp + 0x16C) != 0) {
        PSMTXScale(scale, 1.5f, 1.5f, 1.5f);
        PSMTXConcat(model, scale, model);
    }
    GXLoadPosMtxImm(model, 0);
    btlDispGXQuads2D(-28.0f, 12.0f, 28.0f, -12.0f, blue, green, red, 0xFF);
}

void N_DrawMenuMarioPinchMark(void) {
    extern void* _battleWorkPointer;
    extern void* gp;
    extern u16 danger_icon_tbl[];
    extern u16 peril_icon_tbl[];
    extern void* camGetPtr(s32 id);
    extern void* BattleGetMarioPtr(void*);
    extern void TEXGetGXTexObjFromPalette(void*, void*, s32);
    extern void GXLoadTexObj(void*, s32);
    extern void btlGetScreenPoint(f32*, f32*);
    extern void PSMTXTrans(void*, f32, f32, f32);
    extern void PSMTXConcat(void*, void*, void*);
    extern void PSMTXScale(void*, f32, f32, f32);
    extern void GXLoadPosMtxImm(void*, s32);
    extern void btlDispGXQuads2D(f32, f32, f32, f32, u8, u8, u8, u8);
    void* battleWork;
    void* camera;
    void* mario;
    u8 texObj[32];
    f32 screen[3];
    f32 trans[3][4];
    f32 model[3][4];
    f32 scale[3][4];
    s32 icon;
    u8 red;
    u8 green;
    u8 blue;
    s16 hp;

    battleWork = _battleWorkPointer;
    camera = camGetPtr(8);
    mario = BattleGetMarioPtr(battleWork);
    hp = *(s16*)((s32)mario + 0x10C);
    if (hp <= 0 || hp >= *(s16*)((s32)mario + 0x108) || hp >= 6) return;
    if (hp == 1) {
        icon = peril_icon_tbl[*(s32*)((s32)gp + 0x16C)];
        if (*(u32*)((s32)gp + 0x1C) % 50 < 25) {
            red = 0x40; green = 0xE9; blue = 0xFF;
        } else {
            red = 0x2F; green = 0x52; blue = 0xE9;
        }
    } else {
        red = 0x40; green = 0xE9; blue = 0xFF;
        icon = danger_icon_tbl[*(s32*)((s32)gp + 0x16C)];
    }
    TEXGetGXTexObjFromPalette(**(void***)((s32)*(void**)((s32)battleWork + 0x163F8) + 0xA0), texObj, icon);
    GXLoadTexObj(texObj, 0);
    btlGetScreenPoint((f32*)((s32)mario + 0x3C), screen);
    PSMTXTrans(trans, screen[0] - 20.0f, screen[1] + 60.0f,
               *(f32*)((s32)mario + 0x3C) <= -120.0f ? 0.0f : 20.0f);
    PSMTXConcat((void*)((s32)camera + 0x11C), trans, model);
    if (*(s32*)((s32)gp + 0x16C) != 0) {
        PSMTXScale(scale, 1.5f, 1.5f, 1.5f);
        PSMTXConcat(model, scale, model);
    }
    GXLoadPosMtxImm(model, 0);
    btlDispGXQuads2D(-28.0f, 12.0f, 28.0f, -12.0f, blue, green, red, 0xFF);
}

void DrawMenuHelpWin(double x, double y, char* helpMsg) {
    extern void windowDispGX_Waku_col(double, double, double, double, double, u16, u32*);
    extern void FontDrawStart(void);
    extern void FontDrawColorIDX(s32);
    extern void FontDrawScale(f32);
    extern char* msgSearch(char*);
    extern void FontDrawString(double, double, char*);
    char lines[4][48];
    char* source;
    char* dest;
    s32 line;
    s32 i;
    u32 color;

    color = 0xFFFFFFFF;
    windowDispGX_Waku_col(x, y, 400.0, 50.0, 10.0, 0, &color);
    FontDrawStart();
    FontDrawColorIDX(0);
    FontDrawScale(0.8f);
    source = msgSearch(helpMsg);
    if (source == 0) {
        FontDrawString((f32)(x + 10.0), (f32)(y - 4.0), "message missing");
        return;
    }
    for (i = 0; i < 4; i++) {
        lines[i][0] = 0;
    }
    line = 0;
    dest = lines[0];
    while (*source != 0) {
        if (*source == '<') {
            while (*source != '>') {
                source++;
            }
            source++;
        }
        if (*source == 0) {
            break;
        }
        if (*source == '\n') {
            *dest = 0;
            line++;
            dest = lines[line];
            source++;
        } else {
            *dest++ = *source++;
        }
    }
    *dest = 0;
    if (lines[0][0] != 0) FontDrawScale(0.8f);
    FontDrawString((f32)(x + 20.0), (f32)(y - 4.0), lines[0]);
    if (lines[1][0] != 0) FontDrawScale(0.8f);
    FontDrawString((f32)(x + 20.0), (f32)(y - 28.0), lines[1]);
    if (lines[2][0] != 0) FontDrawScale(0.8f);
    FontDrawString((f32)(x + 20.0), (f32)(y - 52.0), lines[2]);
}

void DrawMenuPartyChangeButton(void) {
    extern void* _battleWorkPointer;
    extern void camGetPtr(s32 id);
    extern void* BattleGetMarioPtr(void* battleWork);
    extern void* BattleGetPartyPtr(void* battleWork);
    extern void BattleAudience_GetItemOn(s32*, f32*, f32*, f32*, void*);
    extern void btlGetScreenPoint(f32* position, f32* screen);
    extern void DrawMenuPartyChangeButton_Sub(f32, f32, f32, s32);
    extern u32 BattleCommandCheckChangePositionEnable(void* battleWork);
    extern void psndSFXOn(char* name);
    extern char str_SFX_BTL_SYS_CAUTION_802ef7a4[];
    void* battleWork;
    void* window;
    void* mario;
    void* party;
    f32 marioScreen[3];
    f32 partyScreen[3];
    f32 itemX;
    f32 itemY;
    f32 itemZ;
    f32 centerX;
    s32 itemId;

    battleWork = _battleWorkPointer;
    window = *(void**)((s32)battleWork + 0x1C78);
    camGetPtr(4);
    mario = BattleGetMarioPtr(battleWork);
    party = BattleGetPartyPtr(battleWork);
    if (party == 0) {
        BattleAudience_GetItemOn(&itemId, &itemX, &itemY, &itemZ, 0);
        if (itemId == -1) {
            *(f32*)((s32)window + 0x120) = 90.0f;
        } else {
            if (*(f32*)((s32)window + 0x120) <= 0.0f) {
                *(f32*)((s32)window + 0x120) = 0.0f;
            } else {
                *(f32*)((s32)window + 0x120) -= 5.0f;
            }
            btlGetScreenPoint((f32*)((s32)mario + 0x3C), marioScreen);
            DrawMenuPartyChangeButton_Sub(marioScreen[0], marioScreen[1] - 20.0f,
                                          *(f32*)((s32)window + 0x120), 5);
        }
    } else {
        btlGetScreenPoint((f32*)((s32)mario + 0x3C), marioScreen);
        btlGetScreenPoint((f32*)((s32)party + 0x3C), partyScreen);
        centerX = (marioScreen[0] + partyScreen[0]) * 0.5f;
        BattleAudience_GetItemOn(&itemId, &itemX, &itemY, &itemZ, 0);
        if (itemId == -1) {
            *(f32*)((s32)window + 0x120) = 90.0f;
            if (BattleCommandCheckChangePositionEnable(battleWork)) {
                DrawMenuPartyChangeButton_Sub(centerX, marioScreen[1] - 20.0f, 0.0f, 4);
            }
        } else {
            if (*(f32*)((s32)window + 0x120) == 90.0f) {
                psndSFXOn(str_SFX_BTL_SYS_CAUTION_802ef7a4);
            }
            if (*(f32*)((s32)window + 0x120) <= 0.0f) {
                *(f32*)((s32)window + 0x120) = 0.0f;
            } else {
                *(f32*)((s32)window + 0x120) -= 5.0f;
            }
            DrawMenuPartyChangeButton_Sub(centerX, marioScreen[1] - 20.0f,
                                          *(f32*)((s32)window + 0x120), 5);
        }
    }
}

void DrawMainIcon(s32 window, s32 actionTable, s32 matrices, s32 positions, s32 angles) {
    extern void* _battleWorkPointer;
    extern void* BattleGetUnitPtr(void*, s32);
    extern void DrawMainIconCircle(f32, s32, s32, s32, f32*);
    extern void DrawMainIconHukidasi(f32, s32, s32, s32);
    void* unit;
    void* cursor;
    f32 depth[6];
    s32 order[8];
    f32 pos[3];
    f32 tempDepth;
    s32 tempOrder;
    s32 i;
    s32 j;
    s32 index;
    u32 state;

    unit = BattleGetUnitPtr(_battleWorkPointer, *(s32*)((s32)_battleWorkPointer + 0x420));
    if (*(s32*)((s32)unit + 8) == 0xDE) {
        cursor = (void*)((s32)_battleWorkPointer + 0x1BB4);
    } else {
        cursor = (void*)((s32)_battleWorkPointer + 0x1BC0);
    }
    for (i = 0; i < *(s32*)((s32)cursor + 8); i++) {
        order[i] = i;
        depth[i] = *(f32*)(positions + i * 12 + 4);
    }
    for (i = 0; i < *(s32*)((s32)cursor + 8) - 1; i++) {
        for (j = i + 1; j < *(s32*)((s32)cursor + 8); j++) {
            if (depth[i] < depth[j]) {
                tempDepth = depth[i];
                tempOrder = order[i];
                depth[i] = depth[j];
                order[i] = order[j];
                depth[j] = tempDepth;
                order[j] = tempOrder;
            }
        }
    }
    for (i = 0; i < *(s32*)((s32)cursor + 8); i++) {
        index = order[i];
        state = *(u32*)(window + 0x20 + index * 4);
        if (state & 1) {
            if ((state & 2) == 0) {
                pos[0] = *(f32*)(positions + index * 12);
                pos[1] = *(f32*)(positions + index * 12 + 4);
                pos[2] = *(f32*)(positions + index * 12 + 8);
                DrawMainIconCircle(*(f32*)(angles + index * 4), window,
                                   actionTable + index * 0x14,
                                   matrices + index * 0x30, pos);
            } else {
                DrawMainIconHukidasi(*(f32*)(angles + index * 4), window,
                                     actionTable + index * 0x14,
                                     matrices + index * 0x30);
            }
        }
    }
}

u8 DrawMenuCursorAndScrollArrow(f32* pos) {
    extern void* gp;
    extern f32 intplGetValue(s32, s32, f32, f32, s32);
    extern void iconDispGx(f32*, s32, s32, f32);
    void* battleWork;
    void* cursor;
    void* common;
    f32 work[3];
    f32 value;
    s32 limit;
    s32 visible;

    battleWork = _battleWorkPointer;
    cursor = *(void**)((s32)battleWork + 0x1C78);
    common = *(void**)cursor;
    value = intplGetValue(4, *(s32*)((s32)cursor + 0x11C), 0.0f, 10.0f, 6);
    *(s32*)((s32)cursor + 0x11C) += 1;

    limit = *(s32*)((s32)gp + 4) * 12 / 60;
    if (*(s32*)((s32)cursor + 0x11C) >= limit) {
        *(s32*)((s32)cursor + 0x11C) = 0;
    }

    if (*(s32*)((s32)common + 4) > 0) {
        work[0] = pos[0] + 155.0f;
        work[1] = pos[1] + 15.0f + value;
        work[2] = 0.0f;
        iconDispGx(work, 0x10, 0x1BD, 0.75f);
    }

    if (*(s32*)((s32)common + 4) < *(s32*)((s32)common + 8) - 6) {
        work[0] = pos[0] + 155.0f;
        work[1] = pos[1] + 20.0f - 164.0f - value;
        work[2] = 0.0f;
        iconDispGx(work, 0x10, 0x1BE, 0.75f);
    }

    visible = *(s32*)common - *(s32*)((s32)common + 4);
    if (visible < 0) {
        visible = 0;
    }
    if (visible >= 6) {
        visible = 5;
    }

    work[0] = pos[0];
    work[1] = pos[1] - 24.0f * visible;
    work[2] = 0.0f;
    iconDispGx(work, 0x14, 0x1F8, 1.0f);
}

void DrawOperationWin(void) {
    extern void DrawSubMenuCommonProcess(f32*, f32*, void*, s32, s32, void*);
    extern void DrawSubIcon(void*, void*, u32*, u16*, char**, u32*, u32*, void*, s32);
    extern f32 vec3_802ef75c[3];
    void* battleWork = _battleWorkPointer;
    u8* window = *(u8**)((s32)battleWork + 0x1C78);
    s32* cursor = *(s32**)window;
    u8* table = (u8*)battleWork + 0x19E8;
    f32 outY, outX, cursorPos[3];
    u16 icons[8];
    u32 spCosts[7], costs[7];
    char* names[7];
    u32 enabled[7];
    u8 angles[20], positions[60], mainMatrices[5 * 48], subMatrices[7 * 48];
    s32 i, index;
    u8* entry;

    BattleGetUnitPtr(battleWork, *(s32*)((s32)battleWork + 0x420));
    DrawSubMenuCommonProcess(&outX, &outY, mainMatrices, (s32)positions, (s32)angles, subMatrices);
    for (i = 0; i < 7; i++) {
        index = (s32)*(f32*)(window + 0x98) + i;
        if (index < cursor[2]) {
            entry = table + index * 0x1C;
            enabled[i] = *(u32*)(entry + 0x4);
            icons[i] = *(u16*)(entry + 0x10);
            if (icons[i] == 0) icons[i] = 0x193;
            names[i] = *(char**)(entry + 0xC);
            costs[i] = *(u32*)(entry + 0x18);
            spCosts[i] = 0;
        }
    }
    DrawMainIcon((s32)window, (s32)battleWork + 0x1724, (s32)mainMatrices, (s32)positions, (s32)angles);
    DrawSubIcon(window, subMatrices, enabled, icons, names, costs, spCosts, NULL, 0);
    cursorPos[0] = vec3_802ef75c[0]; cursorPos[1] = vec3_802ef75c[1]; cursorPos[2] = vec3_802ef75c[2];
    DrawMenuCursorAndScrollArrow(cursorPos);
    entry = table + cursor[0] * 0x1C;
    if (*(char**)(entry + 0x14) != NULL) DrawMenuHelpWin(outX, outY, *(char**)(entry + 0x14));
}

void DrawMultiItemWin(void) {
    extern void DrawSubMenuCommonProcess(f32*, f32*, void*, s32, s32, void*);
    extern void DrawSubIcon(void*, void*, u32*, u16*, char**, u32*, u32*, void*, s32);
    extern f32 vec3_802ef774[3];
    void* battleWork = _battleWorkPointer;
    u8* window = *(u8**)((s32)battleWork + 0x1C78);
    s32* cursor = *(s32**)window;
    u8* table = (u8*)battleWork + 0x1B6C;
    f32 outY;
    f32 outX;
    f32 cursorPos[3];
    u16 icons[8];
    u32 spCosts[7];
    u32 costs[7];
    char* names[7];
    u32 enabled[7];
    u8 angles[20];
    u8 positions[60];
    u8 mainMatrices[5 * 48];
    u8 subMatrices[7 * 48];
    s32 i;
    s32 index;
    u8* entry;

    BattleGetUnitPtr(battleWork, *(s32*)((s32)battleWork + 0x420));
    DrawSubMenuCommonProcess(&outX, &outY, mainMatrices, (s32)positions, (s32)angles, subMatrices);
    for (i = 0; i < 7; i++) {
        index = (s32)*(f32*)(window + 0x98) + i;
        if (index < cursor[2]) {
            entry = table + index * 0x18;
            enabled[i] = *(u32*)(entry + 0x0);
            icons[i] = *(u16*)(entry + 0xC);
            if (icons[i] == 0) {
                icons[i] = 0x193;
            }
            names[i] = *(char**)(entry + 0x4);
            costs[i] = *(u32*)(entry + 0x10);
            spCosts[i] = 0;
        }
    }
    DrawMainIcon((s32)window, (s32)battleWork + 0x1724, (s32)mainMatrices, (s32)positions, (s32)angles);
    DrawSubIcon(window, subMatrices, enabled, icons, names, costs, spCosts, NULL, 0);
    cursorPos[0] = vec3_802ef774[0];
    cursorPos[1] = vec3_802ef774[1];
    cursorPos[2] = vec3_802ef774[2];
    DrawMenuCursorAndScrollArrow(cursorPos);
    entry = table + cursor[0] * 0x18;
    if (*(char**)(entry + 0x8) != NULL) {
        DrawMenuHelpWin(outX, outY, *(char**)(entry + 0x8));
    }
}

void DrawChangePartyWin(void) {
    extern void DrawSubMenuCommonProcess(f32*, f32*, void*, s32, s32, void*);
    extern void DrawSubIcon(void*, void*, u32*, u16*, char**, u32*, u32*, void*, s32);
    extern f32 vec3_802ef768[3];
    void* battleWork = _battleWorkPointer;
    u8* window = *(u8**)((s32)battleWork + 0x1C78);
    s32* cursor = *(s32**)window;
    u8* table = (u8*)battleWork + 0x1AAC;
    f32 outY, outX, cursorPos[3];
    u16 icons[8];
    u32 spCosts[7], costs[7];
    char* names[7];
    u32 enabled[7];
    u8 angles[20], positions[60], mainMatrices[5 * 48], subMatrices[7 * 48];
    s32 i, index;
    u8* entry;

    BattleGetUnitPtr(battleWork, *(s32*)((s32)battleWork + 0x420));
    DrawSubMenuCommonProcess(&outX, &outY, mainMatrices, (s32)positions, (s32)angles, subMatrices);
    for (i = 0; i < 7; i++) {
        index = (s32)*(f32*)(window + 0x98) + i;
        if (index < cursor[2]) {
            entry = table + index * 0x18;
            enabled[i] = *(u32*)(entry + 0x4);
            icons[i] = *(u16*)(entry + 0xC);
            if (icons[i] == 0) icons[i] = 0x193;
            names[i] = *(char**)(entry + 0x8);
            costs[i] = 0;
            spCosts[i] = 0;
        }
    }
    DrawMainIcon((s32)window, (s32)battleWork + 0x1724, (s32)mainMatrices, (s32)positions, (s32)angles);
    DrawSubIcon(window, subMatrices, enabled, icons, names, costs, spCosts, table, 1);
    cursorPos[0] = vec3_802ef768[0]; cursorPos[1] = vec3_802ef768[1]; cursorPos[2] = vec3_802ef768[2];
    DrawMenuCursorAndScrollArrow(cursorPos);
    entry = table + cursor[0] * 0x18;
    if (*(char**)(entry + 0x10) != NULL) DrawMenuHelpWin(outX, outY, *(char**)(entry + 0x10));
}

void DrawSubIcon(void* window, void* subMatrices, u32* enabled, u16* icons,
                 char** names, u32* costs, u32* spCosts, void* table, s32 flag) {
    extern void* _battleWorkPointer;
    extern void* BattleGetUnitPtr(void*, s32);
    extern void PSMTXTrans(void*, f32, f32, f32);
    extern void PSMTXConcat(void*, void*, void*);
    extern void DrawSubIconSub(void*, s32, void*, u32, u16, char*, u32, u32, void*, s32);
    f32 translate[3][4];
    f32 matrices[7][3][4];
    f32 scroll;
    f32 offset;
    s32 base;
    s32 i;

    BattleGetUnitPtr(_battleWorkPointer, *(s32*)((s32)_battleWorkPointer + 0x420));
    scroll = *(f32*)((s32)window + 0x98);
    base = (s32)scroll;
    offset = 24.0f * (scroll - (f32)base);
    for (i = 0; i < 7; i++) {
        if (base + i < *(s32*)((s32)*(void**)window + 8) &&
            (*(u32*)((s32)window + 0xA4 + i * 4) & 1)) {
            PSMTXTrans(translate, 100.0f, -(24.0f * i - 35.0f) + offset, 0.0f);
            PSMTXConcat(translate, (void*)((s32)subMatrices + i * 0x30), matrices[i]);
            DrawSubIconSub(window, i, matrices[i], enabled[i], icons[i], names[i],
                           costs[i], spCosts[i], table, flag);
        }
    }
}

u8 SelectedItemCoordinateColorUpDate(void) {
    extern s8 seq_510;
    extern u8 seleItemCoordCol[];

    if (seq_510 == 3) {
        seleItemCoordCol[1]--;
        if (seleItemCoordCol[1] == 0xC0) {
            seq_510 = 4;
        }
    } else if (seq_510 > 2) {
        if (seq_510 == 5) {
            seleItemCoordCol[2]--;
            if (seleItemCoordCol[2] == 0xC0) {
                seq_510 = 0;
            }
        } else if (seq_510 == 4) {
            seleItemCoordCol[0]++;
            if (seleItemCoordCol[0] == 0xFF) {
                seq_510 = 5;
            }
        }
    } else if (seq_510 == 1) {
        seleItemCoordCol[0]--;
        if (seleItemCoordCol[0] == 0xC0) {
            seq_510 = 2;
        }
    } else if (seq_510 < 1) {
        if (seq_510 >= 0) {
            seleItemCoordCol[1]++;
            if (seleItemCoordCol[1] == 0xFF) {
                seq_510 = 1;
            }
        }
    } else {
        seleItemCoordCol[2]++;
        if (seleItemCoordCol[2] == 0xFF) {
            seq_510 = 3;
        }
    }
}

void InitSubMenuCommonProcess(void* outWindow_, void* outCursor_, void* cursor) {
    extern void* _battleWorkPointer;
    extern void BattleFree(void* ptr);
    extern void* BattleAlloc(s32 size);
    extern void* BattleGetUnitPtr(void* battleWork, s32 unitId);
    void** outWindow;
    void** outCursor;
    void* battleWork;
    void* window;
    void* unit;
    s32 i;

    outWindow = outWindow_;
    outCursor = outCursor_;
    battleWork = _battleWorkPointer;
    window = *(void**)((s32)battleWork + 0x1C78);
    if (window != 0) {
        BattleFree(window);
    }
    window = BattleAlloc(0x124);
    *(void**)((s32)battleWork + 0x1C78) = window;
    *outWindow = window;
    *(void**)window = cursor;
    *(s32*)((s32)window + 0xC) = 0;
    *(s32*)((s32)window + 0x10) = 0;
    *(s32*)((s32)window + 0x14) = 0;
    *(s32*)((s32)window + 0x18) = *(s32*)cursor;
    unit = BattleGetUnitPtr(battleWork, *(s32*)((s32)battleWork + 0x420));
    if (*(s32*)((s32)unit + 8) == 0xDE) {
        *outCursor = (void*)((s32)battleWork + 0x1BB4);
    } else {
        *outCursor = (void*)((s32)battleWork + 0x1BC0);
    }
    *(f32*)((s32)window + 0x98) = 0.0f;
    for (i = 0; i < 7; i++) {
        *(s32*)((s32)window + 0xA4 + i * 4) = 0;
        *(s32*)((s32)window + 0xC0 + i * 4) = 1;
        *(s32*)((s32)window + 0xDC + i * 4) = -i;
        *(s32*)((s32)window + 0xF8 + i * 4) = 0;
    }
}

void DrawMenuPartyChangeButton_Sub(f32 x, f32 y, f32 angle, s32 texture) {
    extern void PSMTXRotRad(void* matrix, char axis, f32 radians);
    extern void PSMTXTransApply(void* source, void* dest, f32 x, f32 y, f32 z);
    extern void btlDispTexPlane2(void* matrix, s32 texture, u32* color, s32 mode);
    f32 matrix[3][4];
    u32 color;

    PSMTXRotRad(matrix, 'y', angle * 0.01745329f);
    PSMTXTransApply(matrix, matrix, x, y, 0.0f);
    color = 0xFFFFFFFF;
    btlDispTexPlane2(matrix, texture, &color, 0);
}

void GetRingCenter(f32* out) {
    void* battleWork;
    void* unit;

    battleWork = _battleWorkPointer;
    unit = BattleGetUnitPtr(battleWork, *(s32*)((s32)battleWork + 0x420));
    btlGetScreenPoint((f32*)((s32)unit + 0x3C), out);
    out[0] += 22.5f;
    out[1] += 105.0f;
    out[2] = 0.0f;
    if (out[0] <= -180.0f) {
        out[0] = -180.0f;
    }
}


u8 DrawMenuPinchMark(void) {
    extern void* _battleWorkPointer;
    extern void* BattleGetMarioPtr(void* battleWork);
    extern void btlDispGXInit2D(void);
    extern void N_DrawMenuMarioPinchMark(void);
    extern void N_DrawMenuPartyPinchMark(void);
    void* mario;

    mario = BattleGetMarioPtr(_battleWorkPointer);
    btlDispGXInit2D();
    if (*(f32*)((s32)mario + 0x3C) <= -120.0f) {
        N_DrawMenuMarioPinchMark();
        N_DrawMenuPartyPinchMark();
    } else {
        N_DrawMenuPartyPinchMark();
        N_DrawMenuMarioPinchMark();
    }
}

void DrawSubMenuCommonProcess(f32* outX, f32* outY, void* matrices,
                              s32 positions, s32 angles, void* outMatrices) {
    extern void* _battleWorkPointer;
    extern void* gp;
    extern void* BattleGetUnitPtr(void*, s32);
    extern void DrawSubMenuCommonProcessSub1(void*, void*, u32, s32);
    extern void GetRingCenter(f32*);
    extern void GetRingOffset(f32, f32*, f32*);
    extern double intplGetValue(double, double, s32, s32, s32);
    extern void PSMTXTrans(void*, f32, f32, f32);
    extern void PSMTXScale(void*, f32, f32, f32);
    extern void PSMTXConcat(void*, void*, void*);
    extern void PSMTXRotRad(void*, char, f32);
    extern void PSMTXIdentity(void*);
    void* battleWork;
    void* window;
    void* cursor;
    void* menuCursor;
    void* unit;
    f32 scaleMtx[3][4];
    f32 transMtx[3][4];
    f32 offsetX;
    f32 offsetY;
    f32* pos;
    f32 value;
    s32 relative;
    s32 absolute;
    s32 count;
    s32 visible;
    s32 state;
    s32 ready;
    s32 settled;
    s32 duration;
    s32 i;

    battleWork = _battleWorkPointer;
    window = *(void**)((s32)battleWork + 0x1C78);
    cursor = *(void**)window;
    unit = BattleGetUnitPtr(battleWork, *(s32*)((s32)battleWork + 0x420));
    menuCursor = (void*)((s32)battleWork +
                         (*(s32*)((s32)unit + 8) == 0xDE ? 0x1BB4 : 0x1BC0));
    visible = *(s32*)((s32)cursor + 8) < 6 ? *(s32*)((s32)cursor + 8) : 6;
    relative = *(s32*)((s32)cursor + 4);
    absolute = *(s32*)cursor;
    if (relative < absolute) {
        if (relative + visible - 1 <= absolute) relative = absolute - visible + 2;
    } else {
        relative = absolute - 1;
    }
    if (relative < 0) relative = 0;
    if (relative >= *(s32*)((s32)cursor + 8) - visible)
        relative = *(s32*)((s32)cursor + 8) - visible;
    *(s32*)((s32)cursor + 4) = relative;
    *outX = -200.0f;
    *outY = -100.0f;
    state = *(s32*)((s32)window + 0xC);
    switch (state) {
    case 0:
        *(u8*)((s32)window + 0x14) = 1;
        *(f32*)((s32)window + 0x98) = (f32)relative;
        settled = 0;
        for (i = 0; i < *(s32*)((s32)menuCursor + 8); i++) {
            if (*(s32*)((s32)window + 0x34 + i * 4) == 1 ||
                *(s32*)((s32)window + 0x34 + i * 4) == 11) settled++;
        }
        ready = 0;
        for (i = 0; i < 7; i++) {
            if (*(s32*)((s32)window + 0xC0 + i * 4) == 2) ready++;
        }
        if (settled == *(s32*)((s32)menuCursor + 8) && ready == 7)
            *(s32*)((s32)window + 0xC) = 2;
        DrawSubMenuCommonProcessSub1(window, cursor, *(u32*)((s32)cursor + 4), visible);
        break;
    case 2:
        *(u8*)((s32)window + 0x14) = 1;
        *(f32*)((s32)window + 0x98) = (f32)relative;
        DrawSubMenuCommonProcessSub1(window, cursor, *(u32*)((s32)cursor + 4), visible);
        break;
    case 10:
    case 20:
    case 30:
    case 40:
        if (state == 10 || state == 20) *(u8*)((s32)window + 0x14) = 1;
        ready = 0;
        for (i = 0; i < 7; i++) {
            if (*(s32*)((s32)window + 0xC0 + i * 4) == 2) ready++;
        }
        if (ready == 7) {
            *(s32*)((s32)window + 0xC) = 2;
            *(f32*)((s32)window + 0x98) = (f32)relative;
        }
        DrawSubMenuCommonProcessSub1(window, cursor, *(u32*)((s32)cursor + 4), visible);
        break;
    }

    *(s32*)((s32)window + 0x1C) = 0;
    count = *(s32*)((s32)menuCursor + 8);
    for (i = 0; i < count; i++) {
        state = *(s32*)((s32)window + 0x34 + i * 4);
        pos = (f32*)(positions + i * 12);
        if (state == 10) {
            *(u32*)((s32)window + 0x20 + i * 4) |= 1;
            *(u32*)((s32)window + 0x20 + i * 4) &= ~2;
            GetRingCenter(pos);
            GetRingOffset(3.1416f * *(f32*)(0x8036359C + count * 20 +
                          *(s32*)((s32)window + 0x84 + i * 4) * 4) / 180.0f,
                          &offsetX, &offsetY);
            pos[0] += offsetX;
            pos[1] += offsetY;
            pos[2] = offsetY;
            value = 1.0f - 0.1f * *(s32*)((s32)window + 0x48 + i * 4);
            if (value <= 0.0f) *(s32*)((s32)window + 0x34 + i * 4) = 11;
            PSMTXScale(scaleMtx, value, value, 1.0f);
            PSMTXTrans(transMtx, pos[0], pos[1], pos[2]);
            PSMTXConcat(transMtx, scaleMtx, (void*)((s32)matrices + i * 0x30));
            *(f32*)(angles + i * 4) = 0.0f;
            *(s32*)((s32)window + 0x48 + i * 4) += 1;
        } else if (state == 1) {
            *(u32*)((s32)window + 0x20 + i * 4) |= 3;
            PSMTXTrans((void*)((s32)matrices + i * 0x30), -40.0f, 55.0f, 0.0f);
            *(f32*)(angles + i * 4) = 360.0f;
            *(s32*)((s32)window + 0x1C) = 1;
        } else if (state == 0) {
            *(u32*)((s32)window + 0x20 + i * 4) |= 3;
            GetRingCenter(pos);
            duration = *(s32*)((s32)gp + 4) * 12 / 60;
            pos[0] = (f32)intplGetValue(pos[0], -60.0, 4,
                                       *(s32*)((s32)window + 0x48 + i * 4), duration);
            pos[1] = (f32)intplGetValue(pos[1], 55.0, 4,
                                       *(s32*)((s32)window + 0x48 + i * 4), duration);
            PSMTXTrans((void*)((s32)matrices + i * 0x30), pos[0] + 20.0f, pos[1], pos[2]);
            *(f32*)(angles + i * 4) = (f32)intplGetValue(0.0, 360.0, 0,
                                       *(s32*)((s32)window + 0x48 + i * 4), duration);
            if (*(s32*)((s32)window + 0x48 + i * 4) > 2)
                *(s32*)((s32)window + 0x1C) = 1;
            if (++*(s32*)((s32)window + 0x48 + i * 4) > duration)
                *(s32*)((s32)window + 0x34 + i * 4) = 1;
        } else if (state == 11) {
            *(u32*)((s32)window + 0x20 + i * 4) &= ~1;
        }
    }

    for (i = 0; i < 7; i++) {
        state = *(s32*)((s32)window + 0xC0 + i * 4);
        if (state == 0) {
            *(u32*)((s32)window + 0xA4 + i * 4) &= ~1;
            *(u8*)((s32)window + 0x114 + i) = 0xFF;
        } else if (state == 1) {
            *(u32*)((s32)window + 0xA4 + i * 4) &= ~1;
            *(u8*)((s32)window + 0x114 + i) = 0xFF;
            if (*(s32*)((s32)window + 0xDC + i * 4) >= 0 && i != 6) {
                duration = *(s32*)((s32)gp + 4) * 12 / 60;
                value = (f32)intplGetValue(90.0, 0.0, 0,
                            *(s32*)((s32)window + 0xDC + i * 4), duration);
                *(u32*)((s32)window + 0xA4 + i * 4) |= 1;
                PSMTXRotRad((void*)((s32)outMatrices + i * 0x30), 'y', value * 0.01745329f);
            }
            duration = *(s32*)((s32)gp + 4) * 12 / 60;
            if (++*(s32*)((s32)window + 0xDC + i * 4) > duration)
                *(s32*)((s32)window + 0xC0 + i * 4) = 2;
        } else if (state == 2) {
            *(u8*)((s32)window + 0x114 + i) = 0xFF;
            if (i == 6) *(u32*)((s32)window + 0xA4 + i * 4) &= ~1;
            else {
                *(u32*)((s32)window + 0xA4 + i * 4) |= 1;
                PSMTXIdentity((void*)((s32)outMatrices + i * 0x30));
            }
        } else if (state == 10 || state == 20 || state == 30 || state == 40) {
            if (state == 10 || state == 20) {
                duration = 12;
            } else {
                duration = (*(s32*)((s32)cursor + 8) - 6) / 2 + 12;
            }
            *(u32*)((s32)window + 0xA4 + i * 4) |= 1;
            PSMTXIdentity((void*)((s32)outMatrices + i * 0x30));
            *(f32*)((s32)window + 0x98) = (f32)intplGetValue(
                *(f32*)((s32)window + 0x9C), *(f32*)((s32)window + 0xA0), 4,
                *(s32*)((s32)window + 0xDC + i * 4), duration);
            if (++*(s32*)((s32)window + 0xDC + i * 4) > duration) {
                *(s32*)((s32)window + 0xC0 + i * 4) = 2;
                *(s32*)((s32)window + 0xDC + i * 4) = 0;
            }
        }
    }
}

u8 DrawSubMenuCommonProcessSub1(void* windowWork, void* cursor, u32 relativePos, int numOptions) {
    s32 count;
    s32 current;
    s32 value;
    s32 i;

    if (numOptions == 6) {
        count = *(s32*)((s32)cursor + 8);
        if (count > 6 && relativePos == count - 6 && *(s32*)((s32)cursor + 4) == 0 &&
            *(s32*)cursor != 1) {
            *(s32*)((s32)windowWork + 0xC) = 0x1E;
            *(s32*)((s32)windowWork + 0x10) = 0;
            *(f32*)((s32)windowWork + 0x9C) = count - 6;
            *(f32*)((s32)windowWork + 0xA0) = 0.0f;
            for (i = 0; i < 7; i++) {
                *(s32*)((s32)windowWork + 0xC0 + i * 4) = 0x1E;
                *(s32*)((s32)windowWork + 0xDC + i * 4) = 0;
            }
        } else if (count > 6 && relativePos == 0 && *(s32*)((s32)cursor + 4) == count - 6 &&
                   *(s32*)cursor != 5) {
            *(s32*)((s32)windowWork + 0xC) = 0x28;
            *(s32*)((s32)windowWork + 0x10) = 0;
            *(f32*)((s32)windowWork + 0x9C) = 0.0f;
            *(f32*)((s32)windowWork + 0xA0) = count - 6;
            for (i = 0; i < 7; i++) {
                *(s32*)((s32)windowWork + 0xC0 + i * 4) = 0x28;
                *(s32*)((s32)windowWork + 0xDC + i * 4) = 0;
            }
        }
    }

    current = *(s32*)((s32)cursor + 4);
    value = relativePos + 1;
    if (current == value) {
        *(s32*)((s32)windowWork + 0xC) = 0xA;
        *(s32*)((s32)windowWork + 0x10) = 0;
        *(f32*)((s32)windowWork + 0x9C) = relativePos;
        *(f32*)((s32)windowWork + 0xA0) = value;
        for (i = 0; i < 7; i++) {
            *(s32*)((s32)windowWork + 0xC0 + i * 4) = 0xA;
            *(s32*)((s32)windowWork + 0xDC + i * 4) = 0;
        }
    } else {
        value = relativePos - 1;
        if (current == value) {
            *(s32*)((s32)windowWork + 0xC) = 0x14;
            *(s32*)((s32)windowWork + 0x10) = 0;
            *(f32*)((s32)windowWork + 0x9C) = relativePos;
            *(f32*)((s32)windowWork + 0xA0) = value;
            for (i = 0; i < 7; i++) {
                *(s32*)((s32)windowWork + 0xC0 + i * 4) = 0x14;
                *(s32*)((s32)windowWork + 0xDC + i * 4) = 0;
            }
        }
    }
}

void BattleMenuDisp_ActSelect_Init(void* battleWork, void* cursor, void* actionTable, u32 flags) {
    extern void BattleFree(void* ptr);
    extern void* BattleAlloc(s32 size);
    extern void* gp;
    void* window;
    s32 index;
    s32 i;
    s32 value;

    window = *(void**)((s32)battleWork + 0x1C78);
    if (window != 0) {
        BattleFree(window);
    }
    window = BattleAlloc(0x124);
    *(void**)((s32)battleWork + 0x1C78) = window;
    if (flags & 1) {
        window = *(void**)((s32)battleWork + 0x1C78);
        *(void**)window = cursor;
        *(void**)((s32)window + 4) = actionTable;
        *(s32*)((s32)window + 0xC) = 10;
        *(u8*)((s32)window + 0x14) = 0;
        *(s32*)((s32)window + 0x18) = *(s32*)cursor;
        index = *(s32*)cursor;
        for (i = 0; i < *(s32*)((s32)cursor + 8); i++) {
            if (index == *(s32*)cursor) {
                *(s32*)((s32)window + 0x34 + index * 4) = 10;
            } else {
                *(s32*)((s32)window + 0x34 + index * 4) = 11;
            }
            *(s32*)((s32)window + 0x48 + index * 4) = 0;
            *(s32*)((s32)window + 0x84 + index * 4) =
                *(s32*)((s32)cursor + 8) + i - 1;
            if (*(s32*)((s32)window + 0x84 + index * 4) >=
                *(s32*)((s32)cursor + 8)) {
                *(s32*)((s32)window + 0x84 + index * 4) -=
                    *(s32*)((s32)cursor + 8);
            }
            index++;
            *(s32*)((s32)window + 0x20 + (index - 1) * 4) = 0;
            if (index >= *(s32*)((s32)cursor + 8)) {
                index = 0;
            }
        }
    } else {
        window = *(void**)((s32)battleWork + 0x1C78);
        *(void**)window = cursor;
        *(void**)((s32)window + 4) = actionTable;
        *(s32*)((s32)window + 0xC) = 0;
        *(u8*)((s32)window + 0x14) = 0;
        *(s32*)((s32)window + 0x18) = *(s32*)cursor;
        index = *(s32*)cursor;
        for (i = 0; i < *(s32*)((s32)cursor + 8); i++) {
            index++;
            if (index >= *(s32*)((s32)cursor + 8)) {
                index = 0;
            }
            *(s32*)((s32)window + 0x34 + index * 4) = 0;
            value = i * (*(s32*)((s32)gp + 4) * 10 / 60);
            *(s32*)((s32)window + 0x48 + index * 4) = value / 2;
            *(s32*)((s32)window + 0x84 + index * 4) = i;
            *(s32*)((s32)window + 0x20 + index * 4) = 0;
        }
    }
}

