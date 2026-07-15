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


u8 DrawSubIconSub(void* menu, s32 phase, f32 baseMtx[3][4], s32 enabled,
                  u16 iconId, char* name, s32 fpCost, s32 spCost,
                  s32 selected, s32 fitText) {
    extern void FontDrawStart(void);
    extern void FontDrawStringMtx(f32 mtx[3][4], char* text);
    extern u16 FontGetMessageWidth(char* text);
    extern void PSMTXTrans(f32 mtx[3][4], f32 x, f32 y, f32 z);
    extern void PSMTXScale(f32 mtx[3][4], f32 x, f32 y, f32 z);
    extern void PSMTXConcat(f32 a[3][4], f32 b[3][4], f32 out[3][4]);
    f32 trans[3][4];
    f32 scale[3][4];
    f32 draw[3][4];
    f32 textScale;
    u32 alpha;
    char digits[4];

    alpha = 0xFF;
    if (phase == 0) alpha = 0x20;
    if (phase == 6) alpha = 0xD0;
    if (iconId == 0) iconId = 0x193;

    FontDrawStart();
    textScale = 0.8f;
    if (fitText != 0 && FontGetMessageWidth(name) > 150) {
        textScale = 120.0f / FontGetMessageWidth(name);
    }
    PSMTXTrans(trans, -104.0f, 22.0f, 0.0f);
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
        PSMTXConcat(baseMtx, trans, draw);
        FontDrawStringMtx(draw, digits);
    }
    return (u8)alpha;
}

void DrawMainMenu(void) {
    void* command;
    void* window;
    void* cursor;
    s32 state;
    s32 count;
    s32 current;
    s32 previous;
    s32 delta;
    s32 i;

    command = (void*)((s32)_battleWorkPointer + 0x171C);
    window = *(void**)((s32)command + 0x55C);
    if (window == 0) {
        return;
    }
    cursor = *(void**)window;
    if (cursor == 0) {
        return;
    }
    state = *(s32*)((s32)window + 0xC);
    count = *(s32*)((s32)cursor + 8);
    current = *(s32*)cursor;
    previous = *(s32*)((s32)window + 0x18);

    if (state == 0 || state == 2) {
        s32 settled = 0;
        for (i = 0; i < count; i++) {
            if (*(s32*)((s32)window + 0x34 + i * 4) == 3) {
                settled++;
            }
        }
        if (settled == count) {
            *(s32*)((s32)window + 0xC) = 1;
        }
    } else if (state == 1) {
        *(s32*)((s32)window + 0x14) = 1;
        delta = current - previous;
        if (delta == 1 || delta == -(count - 1)) {
            *(s32*)((s32)window + 0xC) = 2;
            *(s32*)((s32)window + 0x14) = 0;
            *(s32*)((s32)window + 0x18) = current;
            for (i = 0; i < count; i++) {
                *(s32*)((s32)window + 0x34 + i * 4) = 4;
                *(s32*)((s32)window + 0x48 + i * 4) = 0;
                *(s32*)((s32)window + 0x70 + i * 4) =
                    *(s32*)((s32)window + 0x84 + i * 4);
                if (--*(s32*)((s32)window + 0x84 + i * 4) < 0) {
                    *(s32*)((s32)window + 0x84 + i * 4) = count - 1;
                }
            }
        } else if (delta == -1 || delta == count - 1) {
            *(s32*)((s32)window + 0xC) = 2;
            *(s32*)((s32)window + 0x14) = 0;
            *(s32*)((s32)window + 0x18) = current;
            for (i = 0; i < count; i++) {
                *(s32*)((s32)window + 0x34 + i * 4) = 5;
                *(s32*)((s32)window + 0x48 + i * 4) = 0;
                *(s32*)((s32)window + 0x70 + i * 4) =
                    *(s32*)((s32)window + 0x84 + i * 4);
                if (++*(s32*)((s32)window + 0x84 + i * 4) == count) {
                    *(s32*)((s32)window + 0x84 + i * 4) = 0;
                }
            }
        }
    } else if (state == 10) {
        *(s32*)((s32)window + 0xC) = 1;
        *(s32*)((s32)window + 0x14) = 1;
    }
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
    ;
}


u8 N_DrawMenuPartyPinchMark(void) {
    return 0;
}


u8 N_DrawMenuMarioPinchMark(void) {
    return 0;
}


void DrawMenuHelpWin(double x, double y, char* helpMsg) {
    ;
}


u8 DrawMenuPartyChangeButton(void) {
    return 0;
}


u8 DrawMainIcon(int param_1, int param_2, int param_3, int param_4, int param_5) {
    return 0;
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
    ;
}


void DrawMultiItemWin(void) {
    ;
}


void DrawChangePartyWin(void) {
    ;
}


u8 DrawSubIcon(u8 param_1, u8 param_2, u8 param_3, u8 param_4, u8 param_5, u8 param_6, u8 param_7, u8 param_8, s32 param_9) {
    return 0;
}


u8 SelectedItemCoordinateColorUpDate(void) {
    return 0;
}


void InitSubMenuCommonProcess(void*, void*, void*) {
    ;
}


u8 DrawMenuPartyChangeButton_Sub(void) {
    return 0;
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
    return 0;
}


void DrawSubMenuCommonProcess(f32* outX, f32* outY, void* matrices,
                              s32 positions, s32 angles, void* outMatrices) {
    extern void* _battleWorkPointer;
    extern u8 DrawSubMenuCommonProcessSub1(void*, void*, u32, int);
    void* battleWork = _battleWorkPointer;
    void* window = *(void**)((s32)battleWork + 0x1C78);
    void* cursor = *(void**)window;
    s32 relative = *(s32*)((s32)cursor + 4);
    s32 absolute = *(s32*)cursor;
    s32 count = *(s32*)((s32)cursor + 8);
    s32 visible = count < 6 ? count : 6;
    s32 state;
    s32 i;
    s32 ready;

    (void)matrices;
    (void)positions;
    (void)angles;
    (void)outMatrices;
    if (relative < absolute) {
        if (relative + visible - 1 <= absolute) {
            relative = absolute - visible + 2;
        }
    } else {
        relative = absolute - 1;
    }
    if (relative < 0) {
        relative = 0;
    }
    if (relative >= count - visible) {
        relative = count - visible;
    }
    *(s32*)((s32)cursor + 4) = relative;
    *outX = -200.0f;
    *outY = -100.0f;
    state = *(s32*)((s32)window + 0xC);
    switch (state) {
        case 0:
            *(s32*)((s32)window + 0x14) = 1;
            *(f32*)((s32)window + 0x98) = (f32)relative;
            ready = 0;
            for (i = 0; i < 7; i++) {
                if (*(s32*)((s32)window + 0xC0 + i * 4) == 2) {
                    ready++;
                }
            }
            if (ready == 7) {
                *(s32*)((s32)window + 0xC) = 2;
            }
            DrawSubMenuCommonProcessSub1(window, cursor, relative, visible);
            break;
        case 2:
            *(s32*)((s32)window + 0x14) = 1;
            *(f32*)((s32)window + 0x98) = (f32)relative;
            DrawSubMenuCommonProcessSub1(window, cursor, relative, visible);
            break;
        case 10:
        case 20:
        case 30:
        case 40:
            if (state == 10 || state == 20) {
                *(s32*)((s32)window + 0x14) = 1;
            }
            ready = 0;
            for (i = 0; i < 7; i++) {
                if (*(s32*)((s32)window + 0xC0 + i * 4) == 2) {
                    ready++;
                }
            }
            if (ready == 7) {
                *(s32*)((s32)window + 0xC) = 2;
                *(f32*)((s32)window + 0x98) = (f32)relative;
            }
            DrawSubMenuCommonProcessSub1(window, cursor, relative, visible);
            break;
    }
    *(s32*)((s32)window + 0x1C) = 0;
    for (i = 0; i < count; i++) {
        s32 itemState = *(s32*)((s32)window + 0x34 + i * 4);
        if (itemState == 1) {
            *(u32*)((s32)window + 0x20 + i * 4) |= 3;
            *(s32*)((s32)window + 0x1C) = 1;
        } else if (itemState == 10) {
            *(u32*)((s32)window + 0x20 + i * 4) |= 1;
            *(u32*)((s32)window + 0x20 + i * 4) &= ~2;
            *(s32*)((s32)window + 0x48 + i * 4) += 1;
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

void BattleMenuDisp_ActSelect_Init(void* battleWork, void* cursor, void* actionTable, u32 param_4) {
    ;
}
