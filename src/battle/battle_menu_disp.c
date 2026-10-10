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

void SelectedItemCoordinateColorUpDate(void) {
    extern s8 seq_510;
    extern u8 seleItemCoordCol[4];

    switch (seq_510) {
        case 0:
            if (++seleItemCoordCol[1] == 0xFF) {
                seq_510 = 1;
            }
            break;
        case 1:
            if (--seleItemCoordCol[0] == 0xC0) {
                seq_510 = 2;
            }
            break;
        case 2:
            if (++seleItemCoordCol[2] == 0xFF) {
                seq_510 = 3;
            }
            break;
        case 3:
            if (--seleItemCoordCol[1] == 0xC0) {
                seq_510 = 4;
            }
            break;
        case 4:
            if (++seleItemCoordCol[0] == 0xFF) {
                seq_510 = 5;
            }
            break;
        case 5:
            if (--seleItemCoordCol[2] == 0xC0) {
                seq_510 = 0;
            }
            break;
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

void battleMenuDispEnd(void) {
    void* battleWork;

    battleWork = _battleWorkPointer;
    fileFree(*(void**)((s32)battleWork + 0x163F8));
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

void DrawMainIcon(s32 window, s32 actionTable, s32 matrices, s32 positions, s32 angles) {
    typedef struct { f32 x, y, z; } Position;
    typedef struct { u8 pad[0x20]; u32 flags[6]; } IconWindow;
    typedef struct { s32 pad[2]; s32 count; } Cursor;
    extern void* _battleWorkPointer;
    extern void* BattleGetUnitPtr(void*, s32);
    extern void DrawMainIconCircle(f64, s32, s32, s32, Position*);
    extern void DrawMainIconHukidasi(f32, s32, s32, s32);
    s32 order[8];
    f32 depth[6];
    Position pos;
    void* work;
    void* unit;
    Cursor* cursor;
    f32 tempDepth;
    s32 tempOrder;
    s32 i, j, index;
    u32 state;

    work = _battleWorkPointer;
    unit = BattleGetUnitPtr(work, *(s32*)((s32)work + 0x420));
    if (*(s32*)((s32)unit + 8) == 0xDE) {
        cursor = (Cursor*)((s32)work + 0x1BB4);
    } else {
        cursor = (Cursor*)((s32)work + 0x1BC0);
    }
    for (i = 0; i < cursor->count; i++) {
        order[i] = i;
        depth[i] = ((Position*)positions)[i].y;
    }
    for (i = 0; i < cursor->count - 1; i++) {
        for (j = i + 1; j < cursor->count; j++) {
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
    for (i = 0; i < cursor->count; i++) {
        index = order[i];
        state = ((IconWindow*)window)->flags[index];
        if (state & 1) {
            if (state & 2) {
                DrawMainIconHukidasi(((f32*)angles)[index], window,
                    actionTable + index * 0x14, matrices + index * 0x30);
            } else {
                pos = ((Position*)positions)[index];
                DrawMainIconCircle(((f32*)angles)[index], window,
                    actionTable + index * 0x14, matrices + index * 0x30, &pos);
            }
        }
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

void DrawMainIconCircle(f64 angle, s32 unused, void* iconInfo, f32 mtx[3][4], void* animInfo) {
    typedef f32 Mtx[3][4];

    extern void* _battleWorkPointer;
    extern const f32 float_0_8042242c;
    extern const f32 float_1_8042245c;
    extern const f32 float_1p3_80422508;
    extern const f32 float_1p5_804224a4;
    extern const f32 float_4_80422490;
    extern const f32 float_7p5_8042251c;
    extern const f32 float_8_80422510;
    extern const f32 float_180_80422438;
    extern const f32 float_360_80422444;
    extern const f32 float_deg2rad_80422448;
    extern const f32 float_neg4_80422514;
    extern const f32 float_neg3p0769_80422518;
    extern const f32 float_0p005_8042250c;
    extern const f32 float_0p66_80422520;
    extern u32 dat_8042240c;
    extern const u32 dat_80422410;
    extern const u32 dat_80422414;

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
    typedef struct { f32 values[32]; } QuadPoints;
    extern const f32 dat_802ef650[32], dat_802ef6d0[32];
    QuadPoints positions;
    QuadPoints texCoords;
    Mtx rotMtx;
    Mtx scaleMtx;
    Mtx transMtx;
    Mtx drawMtx;
    u32 texObj[8];
    u32 color;
    u8 shade;
    s32 fade;
    u16 iconId;
    f32 scale;
    f64 offset;
    f64 rad;
    s32 i;
    s32 j;

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

    scale = -(float_0p005_8042250c * *(f32*)((s32)animInfo + 8) - float_1p3_80422508);
    PSMTXScale(scaleMtx, scale, scale, float_1_8042245c);
    rad = (f64)float_deg2rad_80422448 * angle;
    PSMTXRotRad(rotMtx, 0x79, (f32)rad);
    PSMTXConcat(mtx, rotMtx, drawMtx);
    PSMTXConcat(drawMtx, scaleMtx, drawMtx);
    PSMTXConcat((void*)((s32)cam + 0x11C), drawMtx, drawMtx);
    GXLoadPosMtxImm(drawMtx, 0);

    TEXGetGXTexObjFromPalette(**(void***)((s32)*(void**)((s32)battleWork + 0x163F8) + 0xA0), texObj, 0);
    GXLoadTexObj(texObj, 0);

    offset = float_neg3p0769_80422518;
    if (angle >= float_0_8042242c && angle <= float_180_80422438) {
        offset = (float_4_80422490 - (f32)((f64)(f32)((f64)float_8_80422510 * angle) / (f64)float_180_80422438)) / float_1p3_80422508;
    }
    if (angle >= float_180_80422438 && angle <= float_360_80422444) {
        offset = (float_neg4_80422514 + (f32)((f64)(float_8_80422510 * (f32)(angle - float_180_80422438)) / (f64)float_180_80422438)) / float_1p3_80422508;
    }

    positions = *(const QuadPoints*)dat_802ef650;
    texCoords = *(const QuadPoints*)dat_802ef6d0;
    for (i = 0; i < 4; i++) {
        GXBegin(0x80, 0, 4);
        for (j = 0; j < 8; j += 2) {
            btlDispGXPoint2D((f32)(offset + positions.values[i * 8 + j]),
                float_neg3p0769_80422518 + positions.values[i * 8 + j + 1],
                texCoords.values[i * 8 + j], texCoords.values[i * 8 + j + 1], 0, 0, 0, 0x80);
        }
    }
    for (i = 0; i < 4; i++) {
        GXBegin(0x80, 0, 4);
        for (j = 0; j < 8; j += 2) {
            btlDispGXPoint2D(positions.values[i * 8 + j], positions.values[i * 8 + j + 1],
                texCoords.values[i * 8 + j], texCoords.values[i * 8 + j + 1], shade, shade, shade, 0xFF);
        }
    }

    PSMTXTrans(transMtx, float_0_8042242c, float_7p5_8042251c, float_0_8042242c);
    scale = float_0p66_80422520 * (-(float_0p005_8042250c * *(f32*)((s32)animInfo + 8) - float_1p3_80422508));
    PSMTXScale(scaleMtx, scale, scale, float_1_8042245c);
    PSMTXRotRad(rotMtx, 0x79, (f32)rad);
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

void DrawSubIcon(void* window, void* subMatrices, u32* enabled, u16* icons,
                 char** names, u32* costs, u32* spCosts, void* table, s32 flag) {
    extern void* _battleWorkPointer;
    extern void* BattleGetUnitPtr(void*, s32);
    extern void PSMTXTrans(void*, f32, f32, f32);
    extern void PSMTXConcat(void*, void*, void*);
    extern void DrawSubIconSub(void*, s32, void*, u32, u16, char*, u32, u32, void*, s32);
    extern const f32 float_24_80422480, float_100_804224f8, float_35_80422504, float_0_8042242c;
    f32 matrices[7][3][4];
    f32 translate[3][4];
    void* selection;
    f32 scroll;
    f32 offset;
    s32 base;
    s32 i;

    BattleGetUnitPtr(_battleWorkPointer, *(s32*)((s32)_battleWorkPointer + 0x420));
    scroll = *(f32*)((s32)window + 0x98);
    base = (s32)scroll;
    selection = *(void**)window;
    offset = float_24_80422480 * (scroll - (f32)base);
    for (i = 0; i < 7; i++) {
        if ((s32)*(f32*)((s32)window + 0x98) + i < *(s32*)((s32)selection + 8) &&
            (*(u32*)((s32)window + 0xA4 + i * 4) & 1)) {
            PSMTXTrans(translate, float_100_804224f8, -(float_24_80422480 * i - float_35_80422504) + offset, float_0_8042242c);
            PSMTXConcat(translate, (void*)((s32)subMatrices + i * 0x30), matrices[i]);
            DrawSubIconSub(window, i, matrices[i], enabled[i], icons[i], names[i],
                           costs[i], spCosts[i], table, flag);
        }
    }
}


void DrawSubIconSub(void* menu, s32 phase, f32 baseMtx[3][4], s32 enabled,
                    u16 iconId, char* name, s32 fpCost, s32 spCost,
                    s32 table, s32 fitText) {
    extern void* _battleWorkPointer;
    extern u8 seleItemCoordCol[4];
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
    extern char str_btl_cost_disp_FP_802ef7c0[];
    extern char str_btl_cost_disp_AP_802ef7d4[];
    extern char str_btl_disp_HP_802ef7e8[];
    extern const u32 dat_80422418;
    extern const u32 dat_8042241c;
    extern char str_btl_disp_slash_802ef7f4[];
    extern void* gpGlobals;
    extern const f32 float_255_804224b8, float_1_8042245c, float_144_804224bc, float_neg152_804224c0, float_24_80422480, float_neg136_804224c4, float_152_804224c8, float_136_804224cc, float_0_8042242c, float_neg119_804224d0, float_2_804224d4, float_0p5_804224b4, float_neg104_804224d8, float_22_804224dc, float_0p8_8042248c, float_120_804224e0, float_72_804224e4, float_4_80422490, float_25_804224e8, float_30_804224ec;
    extern const f32 float_26_804224f0, float_28_80422494, float_92_804224f4, float_100_804224f8;
    extern const f32 float_76_804224fc, float_78_80422500;
    void* battleWork;
    void* camera;
    s32* selection;
    f32 scale[3][4];
    f32 trans[3][4];
    f32 draw[3][4];
    u8 texObj[32];
    f32 scroll;
    f32 fraction;
    f32 textScale;
    typedef struct { u8 r, g, b, a; } MenuColor;
    MenuColor iconOnDraw, iconOn, iconOffDraw, iconOff;
    MenuColor fontOn, fontOff, fontChosen, color;
    extern u32 unk_80429598;
    extern const f32 float_3p0039_80422420;
    u32 alpha;
    u32 shade;
    u8 blue;
    u8 green;
    u8 red;
    s32 base;
    s32 hp;
    s32 maxHp;
    s32 fpValues[3];
    s32 fpOffsets[3];
    s32 spValues[3];
    s32 spOffsets[3];
    s32 hpValues[3];
    s32 hpOffsets[3];
    char fpDigit[4];
    char spDigit[4];
    char hpDigit[4];
    s32 digitCount;
    s32 i;

    battleWork = _battleWorkPointer;
    selection = *(s32**)menu;
    camera = camGetPtr(8);
    scroll = *(f32*)((s32)menu + 0x98);
    base = (s32)scroll;
    fraction = scroll - (f32)base;
    alpha = 0xFF;
    if (phase == 0) alpha = (s32)(float_255_804224b8 * (float_1_8042245c - fraction));
    if (phase == 6) alpha = (s32)(float_255_804224b8 * fraction);
    shade = alpha;
    if (base + phase == *selection) {
        red = seleItemCoordCol[0];
        green = seleItemCoordCol[1];
        blue = seleItemCoordCol[2];
    } else {
        shade = (s32)(float_144_804224bc * (f32)(alpha & 0xFF) / float_255_804224b8);
        red = 0xFF;
        green = 0xFF;
        blue = 0xFF;
    }

    btlDispGXInit2D();
    PSMTXConcat((void*)((s32)camera + 0x11C), baseMtx, draw);
    GXLoadPosMtxImm(draw, 0);
    TEXGetGXTexObjFromPalette(**(void***)((s32)*(void**)((s32)battleWork + 0x163F8) + 0xA0), texObj, 2);
    GXLoadTexObj(texObj, 0);
    btlDispGXQuads2D(float_neg152_804224c0, float_24_80422480, float_neg136_804224c4, float_0_8042242c, red, green, blue, shade);
    btlDispGXQuads2D(float_152_804224c8, float_24_80422480, float_136_804224cc, float_0_8042242c, red, green, blue, shade);
    TEXGetGXTexObjFromPalette(**(void***)((s32)*(void**)((s32)battleWork + 0x163F8) + 0xA0), texObj, 3);
    GXLoadTexObj(texObj, 0);
    btlDispGXQuads2D(float_neg136_804224c4, float_24_80422480, float_136_804224cc, float_0_8042242c, red, green, blue, shade);

    PSMTXTrans(trans, float_neg119_804224d0, float_2_804224d4, float_0_8042242c);
    PSMTXScale(scale, float_0p5_804224b4, float_0p5_804224b4, float_1_8042245c);
    PSMTXConcat(baseMtx, trans, draw);
    PSMTXConcat(draw, scale, draw);
    if (iconId == 0) iconId = 0x193;
    if (enabled == 1) {
        *(u32*)&iconOn = dat_80422418;
        iconOn.a = alpha;
        iconOnDraw = iconOn;
        iconDispGxCol(draw, 0x10, iconId, (u32*)&iconOnDraw);
    } else {
        *(u32*)&iconOff = dat_8042241c;
        iconOff.a = alpha;
        iconOffDraw = iconOff;
        iconDispGxCol(draw, 0x30, iconId, (u32*)&iconOffDraw);
    }

    FontDrawStart();
    if (enabled == 1) {
        *(u32*)&fontOn = unk_80429598;
        fontOn.a = alpha;
        fontChosen = fontOn;
    } else {
        *(u32*)&fontOff = *(u32*)&float_3p0039_80422420;
        fontOff.a = alpha;
        fontChosen = fontOff;
    }
    color = fontChosen;
    FontDrawColor((u32*)&color);
    PSMTXTrans(trans, float_neg104_804224d8, float_22_804224dc, float_0_8042242c);
    if (fitText == 1) {
        if (FontGetMessageWidth(name) > 150)
            textScale = float_120_804224e0 / FontGetMessageWidth(name);
        else
            textScale = float_0p8_8042248c;
        PSMTXScale(scale, textScale, float_0p8_8042248c, float_0p8_8042248c);
    } else {
        PSMTXScale(scale, float_0p8_8042248c, float_0p8_8042248c, float_0p8_8042248c);
    }
    PSMTXConcat(baseMtx, trans, draw);
    PSMTXConcat(draw, scale, draw);
    FontDrawStringMtx(draw, name);

    if (fpCost != 0) {
        fpValues[2] = fpCost / 100;
        fpValues[1] = (fpCost % 100) / 10;
        fpValues[0] = (fpCost % 100) % 10;
        if (fpValues[2] > 0) {
            fpOffsets[0] = 28; digitCount = 3; fpOffsets[1] = 14; fpOffsets[2] = 0;
        } else if (fpValues[1] > 0) {
            fpOffsets[0] = 21; digitCount = 2; fpOffsets[1] = 7; fpOffsets[2] = 0;
        } else {
            fpOffsets[0] = 14; digitCount = 1; fpOffsets[1] = 0; fpOffsets[2] = 0;
        }
        PSMTXScale(scale, float_1_8042245c, float_1_8042245c, float_1_8042245c);
        for (i = 0; i < digitCount; i++) {
            fpDigit[0] = fpValues[i] + '0';
            fpDigit[1] = 0;
            PSMTXTrans(trans, float_72_804224e4 + float_4_80422490 + (f32)fpOffsets[i], float_25_804224e8, float_0_8042242c);
            PSMTXConcat(baseMtx, trans, draw);
            PSMTXConcat(draw, scale, draw);
            FontDrawStringMtx(draw, fpDigit);
        }
        PSMTXTrans(trans, float_120_804224e0, float_22_804224dc, float_0_8042242c);
        PSMTXScale(scale, float_0p8_8042248c, float_0p8_8042248c, float_0p8_8042248c);
        PSMTXConcat(baseMtx, trans, draw);
        PSMTXConcat(draw, scale, draw);
        FontDrawStringMtx(draw, msgSearch(str_btl_cost_disp_FP_802ef7c0));
    }
    if (spCost != 0) {
        spValues[2] = spCost / 100;
        spValues[1] = (spCost % 100) / 10;
        spValues[0] = (spCost % 100) % 10;
        if (spValues[2] > 0) {
            spOffsets[0] = 28; digitCount = 3; spOffsets[1] = 14; spOffsets[2] = 0;
        } else if (spValues[1] > 0) {
            spOffsets[0] = 21; digitCount = 2; spOffsets[1] = 7; spOffsets[2] = 0;
        } else {
            spOffsets[0] = 14; digitCount = 1; spOffsets[1] = 0; spOffsets[2] = 0;
        }
        PSMTXScale(scale, float_1_8042245c, float_1_8042245c, float_1_8042245c);
        for (i = 0; i < digitCount; i++) {
            spDigit[0] = spValues[i] + '0';
            spDigit[1] = 0;
            PSMTXTrans(trans, float_72_804224e4 + float_4_80422490 + (f32)spOffsets[i], float_25_804224e8, float_0_8042242c);
            PSMTXConcat(baseMtx, trans, draw);
            PSMTXConcat(draw, scale, draw);
            FontDrawStringMtx(draw, spDigit);
        }
        PSMTXTrans(trans, float_120_804224e0, float_22_804224dc, float_0_8042242c);
        PSMTXScale(scale, float_0p8_8042248c, float_0p8_8042248c, float_0p8_8042248c);
        PSMTXConcat(baseMtx, trans, draw);
        PSMTXConcat(draw, scale, draw);
        FontDrawStringMtx(draw, msgSearch(str_btl_cost_disp_AP_802ef7d4));
    }
    if (table != 0) {
        table += ((s32)*(f32*)((s32)menu + 0x98) + phase) * 0x18;
        hp = *(s16*)(table + 0x14);
        maxHp = *(s16*)(table + 0x16);
        PSMTXTrans(trans, float_30_804224ec, float_22_804224dc, float_0_8042242c);
        PSMTXScale(scale, float_0p8_8042248c, float_0p8_8042248c, float_0p8_8042248c);
        PSMTXConcat(baseMtx, trans, draw);
        PSMTXConcat(draw, scale, draw);
        FontDrawStringMtx(draw, msgSearch(str_btl_disp_HP_802ef7e8));

        hpValues[2] = hp / 100;
        hp %= 100;
        hpValues[1] = hp / 10;
        hpValues[0] = hp % 10;
        if (hpValues[2] > 0) {
            hpOffsets[0] = 28; digitCount = 3; hpOffsets[1] = 14; hpOffsets[2] = 0;
        } else if (hpValues[1] > 0) {
            hpOffsets[0] = 21; digitCount = 2; hpOffsets[1] = 7; hpOffsets[2] = 0;
        } else {
            hpOffsets[0] = 14; digitCount = 1; hpOffsets[1] = 0; hpOffsets[2] = 0;
        }
        PSMTXScale(scale, float_1_8042245c, float_1_8042245c, float_1_8042245c);
        for (i = 0; i < digitCount; i++) {
            hpDigit[1] = 0;
            hpDigit[0] = hpValues[i] + '0';
            if (*(u32*)((s32)gpGlobals + 8) == 0)
                PSMTXTrans(trans, float_26_804224f0 + float_30_804224ec + (f32)hpOffsets[i], float_25_804224e8, float_0_8042242c);
            else
                PSMTXTrans(trans, float_28_80422494 + float_30_804224ec + (f32)hpOffsets[i], float_25_804224e8, float_0_8042242c);
            PSMTXConcat(baseMtx, trans, draw);
            PSMTXConcat(draw, scale, draw);
            FontDrawStringMtx(draw, hpDigit);
        }
        if (*(u32*)((s32)gpGlobals + 8) == 0)
            PSMTXTrans(trans, float_92_804224f4, float_22_804224dc, float_0_8042242c);
        else
            PSMTXTrans(trans, float_100_804224f8, float_22_804224dc, float_0_8042242c);
        PSMTXScale(scale, float_0p8_8042248c, float_0p8_8042248c, float_0p8_8042248c);
        PSMTXConcat(baseMtx, trans, draw);
        PSMTXConcat(draw, scale, draw);
        FontDrawStringMtx(draw, msgSearch(str_btl_disp_slash_802ef7f4));
        hpValues[2] = maxHp / 100;
        maxHp %= 100;
        hpValues[1] = maxHp / 10;
        hpValues[0] = maxHp % 10;
        if (hpValues[2] < 1) {
            if (hpValues[1] < 1) {
                hpOffsets[0] = 14; digitCount = 1; hpOffsets[1] = 0;
            } else {
                hpOffsets[0] = 21; digitCount = 2; hpOffsets[1] = 7;
            }
        } else {
            hpOffsets[0] = 28; digitCount = 3; hpOffsets[1] = 14;
        }
        hpOffsets[2] = 0;
        PSMTXScale(scale, float_1_8042245c, float_1_8042245c, float_1_8042245c);
        for (i = 0; i < digitCount; i++) {
            hpDigit[1] = 0;
            hpDigit[0] = hpValues[i] + '0';
            if (*(u32*)((s32)gpGlobals + 8) == 0)
                PSMTXTrans(trans, float_76_804224fc + float_30_804224ec + (f32)hpOffsets[i], float_25_804224e8, float_0_8042242c);
            else
                PSMTXTrans(trans, float_78_80422500 + float_30_804224ec + (f32)hpOffsets[i], float_25_804224e8, float_0_8042242c);
            PSMTXConcat(baseMtx, trans, draw);
            PSMTXConcat(draw, scale, draw);
            FontDrawStringMtx(draw, hpDigit);
        }
    }
}

void DrawMenuPartyChangeButton(void) {
    extern void* _battleWorkPointer;
    extern void camGetPtr(s32 id);
    extern void* BattleGetMarioPtr(void* battleWork);
    extern void* BattleGetPartyPtr(void* battleWork);
    extern void BattleAudience_GetItemOn(s32*, f32*, f32*, f32*, void*);
    extern void btlGetScreenPoint(f32* position, f32* screen);
    extern void DrawMenuPartyChangeButton_Sub(f32, f32, f32, s32);
    extern s32 BattleCommandCheckChangePositionEnable(void* battleWork);
    extern void psndSFXOn(char* name);
    extern char str_SFX_BTL_SYS_CAUTION_802ef7a4[];
    extern f32 float_90_80422440, float_0_8042242c, float_5_804224b0;
    extern f32 float_20_8042244c, float_0p5_804224b4;
    void* battleWork;
    void* window;
    void* mario;
    void* party;
    f32 marioScreen[3];
    f32 partyScreen[3];
    s32 itemId;
    f32 itemX;
    f32 itemY;
    f32 itemZ;
    f32 centerX;
    f32 centerY;

    battleWork = _battleWorkPointer;
    window = *(void**)((s32)battleWork + 0x1C78);
    camGetPtr(8);
    mario = BattleGetMarioPtr(battleWork);
    party = BattleGetPartyPtr(battleWork);
    if (party == 0) {
        BattleAudience_GetItemOn(&itemId, &itemX, &itemY, &itemZ, 0);
        if (itemId != -1) {
            if (*(f32*)((s32)window + 0x120) > float_0_8042242c) {
                *(f32*)((s32)window + 0x120) -= float_5_804224b0;
            } else {
                *(f32*)((s32)window + 0x120) = float_0_8042242c;
            }
            btlGetScreenPoint((f32*)((s32)mario + 0x3C), marioScreen);
            DrawMenuPartyChangeButton_Sub(marioScreen[0], marioScreen[1] - float_20_8042244c,
                                          *(f32*)((s32)window + 0x120), 5);
        } else {
            *(f32*)((s32)window + 0x120) = float_90_80422440;
        }
    } else {
        btlGetScreenPoint((f32*)((s32)mario + 0x3C), marioScreen);
        btlGetScreenPoint((f32*)((s32)party + 0x3C), partyScreen);
        centerY = marioScreen[1];
        centerX = float_0p5_804224b4 * (marioScreen[0] + partyScreen[0]);
        BattleAudience_GetItemOn(&itemId, &itemX, &itemY, &itemZ, 0);
        if (itemId != -1) {
            if (float_90_80422440 == *(f32*)((s32)window + 0x120)) {
                psndSFXOn(str_SFX_BTL_SYS_CAUTION_802ef7a4);
            }
            if (*(f32*)((s32)window + 0x120) > float_0_8042242c) {
                *(f32*)((s32)window + 0x120) -= float_5_804224b0;
            } else {
                *(f32*)((s32)window + 0x120) = float_0_8042242c;
            }
            DrawMenuPartyChangeButton_Sub(centerX, centerY - float_20_8042244c,
                                          *(f32*)((s32)window + 0x120), 5);
        } else {
            *(f32*)((s32)window + 0x120) = float_90_80422440;
            if (BattleCommandCheckChangePositionEnable(battleWork)) {
                DrawMenuPartyChangeButton_Sub(centerX, centerY - float_20_8042244c, float_0_8042242c, 4);
            }
        }
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

void N_DrawMenuMarioPinchMark(void) {
    extern void* _battleWorkPointer;
    extern void* gp;
    extern u16 danger_icon_tbl[6];
    extern u16 peril_icon_tbl[6];
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
    extern const f32 float_20_8042244c;
    extern const f32 float_60_804224a0;
    extern const f32 float_neg120_8042249c;
    extern const f32 float_0_8042242c;
    extern const f32 float_10_8042246c;
    extern const f32 float_1p5_804224a4;
    extern const f32 float_neg28_804224a8;
    extern const f32 float_12_80422430;
    extern const f32 float_28_80422494;
    extern const f32 float_neg12_804224ac;
    void* battleWork;
    void* camera;
    void* mario;
    f32 trans[3][4];
    f32 model[3][4];
    f32 scale[3][4];
    u8 texObj[32];
    f32 screen[3];
    s32 icon;
    u8 blue;
    u8 green;
    u8 red;
    u8 alpha;
    s16 hp;

    battleWork = _battleWorkPointer;
    camera = camGetPtr(8);
    mario = BattleGetMarioPtr(battleWork);
    hp = *(s16*)((s32)mario + 0x10C);
    if (hp <= 0 || hp >= *(s16*)((s32)mario + 0x108) || hp > 5) return;
    if (hp == 1) {
        icon = peril_icon_tbl[*(s32*)((s32)gp + 0x16C)];
        if (*(u32*)((s32)gp + 0x1C) % 50 <= 24) {
            blue = 0xFF; green = 0xE9; red = 0x40; alpha = 0xFF;
        } else {
            blue = 0xE9; green = 0x52; red = 0x2F; alpha = 0xFF;
        }
    } else {
        blue = 0xFF; green = 0xE9; red = 0x40; alpha = 0xFF;
        icon = danger_icon_tbl[*(s32*)((s32)gp + 0x16C)];
    }
    TEXGetGXTexObjFromPalette(**(void***)((s32)*(void**)((s32)battleWork + 0x163F8) + 0xA0), texObj, icon);
    GXLoadTexObj(texObj, 0);
    btlGetScreenPoint((f32*)((s32)mario + 0x3C), screen);
    if (*(f32*)((s32)mario + 0x3C) > float_neg120_8042249c)
        PSMTXTrans(trans, screen[0] - float_20_8042244c, float_60_804224a0 + screen[1], float_20_8042244c);
    else
        PSMTXTrans(trans, screen[0] - float_20_8042244c, float_60_804224a0 + screen[1], float_0_8042242c);
    PSMTXConcat((void*)((s32)camera + 0x11C), trans, model);
    if (*(u32*)((s32)gp + 0x16C) != 0) {
        PSMTXScale(scale, float_1p5_804224a4, float_1p5_804224a4, float_1p5_804224a4);
        PSMTXConcat(model, scale, model);
    }
    GXLoadPosMtxImm(model, 0);
    btlDispGXQuads2D(float_neg28_804224a8, float_12_80422430, float_28_80422494, float_neg12_804224ac, blue, green, red, alpha);
}

void N_DrawMenuPartyPinchMark(void) {
    extern void* _battleWorkPointer;
    extern void* gp;
    extern u16 danger_icon_tbl[6];
    extern u16 peril_icon_tbl[6];
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
    extern const f32 float_20_8042244c;
    extern const f32 float_60_804224a0;
    extern const f32 float_neg120_8042249c;
    extern const f32 float_0_8042242c;
    extern const f32 float_10_8042246c;
    extern const f32 float_1p5_804224a4;
    extern const f32 float_neg28_804224a8;
    extern const f32 float_12_80422430;
    extern const f32 float_28_80422494;
    extern const f32 float_neg12_804224ac;
    void* battleWork;
    void* camera;
    void* party;
    f32 trans[3][4];
    f32 model[3][4];
    f32 scale[3][4];
    u8 texObj[32];
    f32 screen[3];
    s32 icon;
    u8 blue;
    u8 green;
    u8 red;
    u8 alpha;
    s16 hp;

    battleWork = _battleWorkPointer;
    camera = camGetPtr(8);
    party = BattleGetPartyPtr(battleWork);
    if (party == 0) return;
    hp = *(s16*)((s32)party + 0x10C);
    if (hp <= 0 || hp >= *(s16*)((s32)party + 0x108) || hp > 5) return;
    if (hp == 1) {
        icon = peril_icon_tbl[*(s32*)((s32)gp + 0x16C)];
        if (*(u32*)((s32)gp + 0x1C) % 50 <= 24) {
            blue = 0xFF; green = 0xE9; red = 0x40; alpha = 0xFF;
        } else {
            blue = 0xE9; green = 0x52; red = 0x2F; alpha = 0xFF;
        }
    } else {
        blue = 0xFF; green = 0xE9; red = 0x40; alpha = 0xFF;
        icon = danger_icon_tbl[*(s32*)((s32)gp + 0x16C)];
    }
    TEXGetGXTexObjFromPalette(**(void***)((s32)*(void**)((s32)battleWork + 0x163F8) + 0xA0), texObj, icon);
    GXLoadTexObj(texObj, 0);
    btlGetScreenPoint((f32*)((s32)party + 0x3C), screen);
    if (*(f32*)((s32)party + 0x3C) > float_neg120_8042249c)
        PSMTXTrans(trans, screen[0] - float_20_8042244c, float_60_804224a0 + screen[1], float_10_8042246c);
    else
        PSMTXTrans(trans, screen[0] - float_20_8042244c, float_60_804224a0 + screen[1], float_0_8042242c);
    PSMTXConcat((void*)((s32)camera + 0x11C), trans, model);
    if (*(u32*)((s32)gp + 0x16C) != 0) {
        PSMTXScale(scale, float_1p5_804224a4, float_1p5_804224a4, float_1p5_804224a4);
        PSMTXConcat(model, scale, model);
    }
    GXLoadPosMtxImm(model, 0);
    btlDispGXQuads2D(float_neg28_804224a8, float_12_80422430, float_28_80422494, float_neg12_804224ac, blue, green, red, alpha);
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

void DrawMenuHelpWin(f32 x, f32 y, char* helpMsg) {
    extern void windowDispGX_Waku_col(double, double, double, double, double, u16, u32*);
    extern void FontDrawStart(void);
    extern void FontDrawColorIDX(s32);
    extern void FontDrawScale(f32);
    extern char* msgSearch(char*);
    extern void FontDrawString(f32, f32, char*);
    extern const u32 dat_80422428;
    extern const u8 str_no_description_802ef788[25];
    extern const f32 float_400_80422484, float_50_80422488, float_10_8042246c;
    extern const f32 float_0p8_8042248c, float_4_80422490, float_20_8042244c;
    extern const f32 float_28_80422494, float_52_80422498;
    signed char lines[4][48];
    signed char* source;
    signed char* dest;
    s32 offset;
    s32 i;
    u32 color = dat_80422428;

    windowDispGX_Waku_col(x, y, float_400_80422484, float_50_80422488, float_10_8042246c, 0, &color);
    FontDrawStart();
    FontDrawColorIDX(0);
    FontDrawScale(float_0p8_8042248c);
    source = (signed char*)msgSearch(helpMsg);
    if (source != 0) {
        for (i = 0; i < 4; i++) lines[i][0] = 0;
        dest = lines[0];
        offset = 0;
        for (;;) {
            if (*source == '<') {
                do { source++; } while (*source != '>');
                source++;
            }
            if (*source == 0) break;
            if (*source == '\n') {
                *dest = 0;
                offset += 48;
                dest = (signed char*)lines + offset;
                source++;
            } else {
                *dest++ = *source++;
            }
        }
        *dest = 0;
        if (lines[0][0] != 0) FontDrawScale(float_0p8_8042248c);
        x = float_20_8042244c + x;
        FontDrawString(x, y - float_4_80422490, (char*)lines[0]);
        if (lines[1][0] != 0) FontDrawScale(float_0p8_8042248c);
        FontDrawString(x, y - float_28_80422494, (char*)lines[1]);
        if (lines[2][0] != 0) FontDrawScale(float_0p8_8042248c);
        FontDrawString(x, y - float_52_80422498, (char*)lines[2]);
    } else {
        FontDrawString(float_10_8042246c + x, y - float_4_80422490, (char*)str_no_description_802ef788);
    }
}

u8 DrawMenuCursorAndScrollArrow(f32* pos) {
    extern void* gp;
    extern f32 intplGetValue(s32, s32, s32, f32, f32);
    extern void iconDispGx(f32*, s32, s32, f32);
    extern const f32 float_0_8042242c, float_10_8042246c, float_155_80422470;
    extern const f32 float_15_80422474, float_0p75_80422478, float_164_8042247c;
    extern const f32 float_24_80422480, float_20_8042244c, float_1_8042245c;
    typedef struct MenuCursor { s32 absolute, relative, count; } MenuCursor;
    typedef struct Vec3 { f32 x, y, z; } Vec3;
    void* window = *(void**)((s32)_battleWorkPointer + 0x1C78);
    MenuCursor* cursor = *(void**)window;
    Vec3 work;
    f32 value;
    f32 y;
    s32 visible;
    value = intplGetValue(4, *(s32*)((s32)window + 0x11C), 6,
        float_0_8042242c, float_10_8042246c);
    *(s32*)((s32)window + 0x11C) += 1;
    if (*(s32*)((s32)window + 0x11C) >= *(s32*)((s32)gp + 4) * 12 / 60)
        *(s32*)((s32)window + 0x11C) = 0;
    if (cursor->relative > 0) {
        work.y = float_15_80422474 + pos[1] + value;
        work.x = float_155_80422470 + pos[0];
        work.z = float_0_8042242c;
        iconDispGx((f32*)&work, 0x10, 0x1BD, float_0p75_80422478);
    }
    if (cursor->relative < cursor->count - 6) {
        work.y = float_20_8042244c + pos[1] - float_164_8042247c - value;
        work.x = float_155_80422470 + pos[0];
        work.z = float_0_8042242c;
        iconDispGx((f32*)&work, 0x10, 0x1BE, float_0p75_80422478);
    }
    visible = cursor->absolute - cursor->relative;
    if (visible < 0) visible = 0;
    if (visible >= 6) visible = 5;
    work.x = pos[0];
    work.y = -(float_24_80422480 * visible - pos[1]);
    work.z = float_0_8042242c;
    iconDispGx((f32*)&work, 0x14, 0x1F8, float_1_8042245c);
}

void InitSubMenuCommonProcess(void* outWindow_, void* outCursor_, void* cursor) {
    extern void* _battleWorkPointer;
    extern void BattleFree(void*);
    extern void* BattleAlloc(s32);
    extern void* BattleGetUnitPtr(void*, s32);
    extern const f32 float_0_8042242c;
    typedef struct MenuCursor { s32 absolute, relative, count; } MenuCursor;
    typedef struct MenuWindow {
        MenuCursor* cursor;
        u8 reserved4[8];
        s32 state, timer;
        u8 active;
        u8 reserved15[3];
        s32 previous;
        u8 reserved1c[0x7C];
        f32 scroll;
        u8 reserved9c[8];
        s32 flags[7], states[7], timers[7], slots[7];
    } MenuWindow;
    MenuWindow** outWindow = outWindow_;
    MenuCursor** outCursor = outCursor_;
    MenuCursor* cur = cursor;
    void* battleWork = _battleWorkPointer;
    void* window = *(void**)((s32)battleWork + 0x1C78);
    void* unit;
    s32 i;
    if (window != 0) BattleFree(window);
    window = BattleAlloc(0x124);
    *(void**)((s32)battleWork + 0x1C78) = window;
    *outWindow = *(void**)((s32)battleWork + 0x1C78);
    (*outWindow)->cursor = cur;
    (*outWindow)->state = 0;
    (*outWindow)->timer = 0;
    (*outWindow)->active = 0;
    (*outWindow)->previous = cur->absolute;
    unit = BattleGetUnitPtr(battleWork, *(s32*)((s32)battleWork + 0x420));
    if (*(s32*)((s32)unit + 8) == 0xDE)
        *outCursor = (void*)((s32)battleWork + 0x1BB4);
    else
        *outCursor = (void*)((s32)battleWork + 0x1BC0);
    (*outWindow)->scroll = float_0_8042242c;
    for (i = 0; i < 7; i++) {
        (*outWindow)->flags[i] = 0;
        (*outWindow)->states[i] = 1;
        (*outWindow)->timers[i] = -i;
        (*outWindow)->slots[i] = 0;
    }
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

void DrawSubMenuCommonProcessSub1(void* windowWork, void* cursor, u32 relativePos, int numOptions) {
    extern const f32 float_0_8042242c;
    typedef struct MenuCursor { s32 absolute, relative, count; } MenuCursor;
    typedef struct MenuWindow {
        u8 reserved0[0xC];
        s32 state, timer;
        u8 reserved14[0x88];
        f32 start, end;
        u8 reserveda4[0x1C];
        s32 states[7], timers[7];
    } MenuWindow;
    MenuWindow* window = windowWork;
    MenuCursor* cur = cursor;
    s32 i;
    if (numOptions == 6 && cur->count > 6 && relativePos == cur->count - 6 &&
        cur->relative == 0 && cur->absolute != 1) {
        window->state = 30;
        window->timer = 0;
        window->start = cur->count - 6;
        window->end = float_0_8042242c;
        for (i = 0; i < 7; i++) {
            window->states[i] = 30;
            window->timers[i] = 0;
        }
    } else if (numOptions == 6 && cur->count > 6 && relativePos == 0 &&
               cur->relative == cur->count - 6 && cur->absolute != 5) {
        window->state = 40;
        window->timer = 0;
        window->start = float_0_8042242c;
        window->end = cur->count - 6;
        for (i = 0; i < 7; i++) {
            window->states[i] = 40;
            window->timers[i] = 0;
        }
    } else if (cur->relative == relativePos + 1) {
        window->state = 10;
        window->timer = 0;
        window->start = (s32)relativePos;
        window->end = (s32)(relativePos + 1);
        for (i = 0; i < 7; i++) {
            window->states[i] = 10;
            window->timers[i] = 0;
        }
    } else if (cur->relative == relativePos - 1) {
        window->state = 20;
        window->timer = 0;
        window->start = (s32)relativePos;
        window->end = (s32)(relativePos - 1);
        for (i = 0; i < 7; i++) {
            window->states[i] = 20;
            window->timers[i] = 0;
        }
    }
}

void DrawSubMenuCommonProcess(f32* outX, f32* outY, void* matrices,
                              s32 positions, s32 angles, void* outMatrices) {
    extern void* _battleWorkPointer;
    extern void* gp;
    extern void* BattleGetUnitPtr(void*, s32);
    extern void DrawSubMenuCommonProcessSub1(void*, void*, u32, s32);
    extern void GetRingCenter(f32*);
    extern void GetRingOffset(f32*, f32*, f32);
    extern f32 intplGetValue(s32, s32, s32, f32, f32);
    extern void PSMTXTrans(void*, f32, f32, f32);
    extern void PSMTXScale(void*, f32, f32, f32);
    extern void PSMTXConcat(void*, void*, void*);
    extern void PSMTXRotRad(void*, char, f32);
    extern void PSMTXIdentity(void*);
    extern f32 kakutable[15];
    extern const f32 float_neg200_80422460;
    extern const f32 float_neg100_80422464;
    extern const f32 float_neg40_80422468;
    extern const f32 float_3p1416_80422434;
    extern const f32 float_180_80422438;
    extern const f32 float_360_80422444;
    extern const f32 float_deg2rad_80422448;
    extern const f32 float_0p1_80422458;
    extern const f32 float_1_8042245c;
    extern const f32 float_0_8042242c;
    extern const f32 float_55_80422454;
    extern const f32 float_20_8042244c;
    extern const f32 float_neg60_80422450;
    extern const f32 float_90_80422440;
    typedef struct MenuCursor { s32 absolute, relative, count; } MenuCursor;
    typedef struct MenuWindow {
        MenuCursor* cursor;
        void* table;
        s32 reserved8;
        s32 state;
        u8 reserved10[4];
        u8 active;
        u8 reserved15[3];
        s32 previous;
        s32 show;
        u32 flags[5];
        s32 states[5];
        s32 timers[5];
        s32 reserved5c[5];
        s32 starts[5];
        s32 slots[5];
        f32 scroll, scrollStart, scrollEnd;
        u32 subFlags[7];
        s32 subStates[7];
        s32 subTimers[7];
        s32 reservedf8[7];
        u8 icons[7];
    } MenuWindow;
    void* battleWork;
    MenuWindow* window;
    MenuCursor* cursor;
    MenuCursor* menuCursor;
    void* unit;
    f32 transMtx[3][4];
    f32 scaleMtx[3][4];
    f32 offsetX;
    f32 offsetY;
    f32* pos;
    f32 value;
    f32 targetAngle;
    s32 relative;
    s32 absolute;
    s32 count;
    s32 visible;
    s32 state;
    s32 ready;
    s32 settled;
    s32 duration;
    s32 i;
    s32 matrixOffset;
    s32 positionOffset;
    s32 itemOffset;

    battleWork = _battleWorkPointer;
    window = *(void**)((s32)battleWork + 0x1C78);
    cursor = window->cursor;
    unit = BattleGetUnitPtr(battleWork, *(s32*)((s32)battleWork + 0x420));
    if (*(s32*)((s32)unit + 8) == 0xDE)
        menuCursor = (void*)((s32)battleWork + 0x1BB4);
    else
        menuCursor = (void*)((s32)battleWork + 0x1BC0);
    visible = 6;
    if (cursor->count < 6)
        visible = cursor->count;
    relative = cursor->relative;
    absolute = cursor->absolute;
    if (relative < absolute) {
        if (relative + visible - 1 <= absolute) cursor->relative = absolute - visible + 2;
    } else {
        cursor->relative = absolute - 1;
    }
    if (cursor->relative < 0) cursor->relative = 0;
    if (cursor->relative >= cursor->count - visible)
        cursor->relative = cursor->count - visible;
    *outX = float_neg200_80422460;
    *outY = float_neg100_80422464;
    state = window->state;
    switch (state) {
    case 0:
        window->active = 1;
        window->scroll = (f32)cursor->relative;
        settled = 0;
        for (i = 0; i < menuCursor->count; i++) {
            if (window->states[i] == 1 ||
                window->states[i] == 11) settled++;
        }
        ready = 0;
        for (i = 0; i < 7; i++) {
            if (window->subStates[i] == 2) ready++;
        }
        if (settled == menuCursor->count && ready == 7)
            window->state = 2;
        DrawSubMenuCommonProcessSub1(window, cursor, relative, visible);
        break;
    case 2:
        window->active = 1;
        window->scroll = (f32)cursor->relative;
        DrawSubMenuCommonProcessSub1(window, cursor, relative, visible);
        break;
    case 10:
    case 20:
    case 30:
    case 40:
        if (state == 10 || state == 20) window->active = 1;
        ready = 0;
        for (i = 0; i < 7; i++) {
            if (window->subStates[i] == 2) ready++;
        }
        if (ready == 7) {
            window->state = 2;
            window->scroll = (f32)cursor->relative;
        }
        DrawSubMenuCommonProcessSub1(window, cursor, relative, visible);
        break;
    }

    window->show = 0;
    count = menuCursor->count;
    for (i = 0, matrixOffset = 0, positionOffset = 0, itemOffset = 0;
         i < menuCursor->count;
         i++, matrixOffset += 0x30, positionOffset += 0xC, itemOffset += 4) {
        state = window->states[i];
        pos = (f32*)(positions + positionOffset);
        switch (state) {
        case 10:
            window->flags[i] |= 1;
            window->flags[i] &= ~2;
            targetAngle = float_3p1416_80422434 * *(f32*)((s32)kakutable + menuCursor->count * 20 +
                window->slots[i] * 4 - 0x3C) / float_180_80422438;
            GetRingCenter(pos);
            GetRingOffset(&offsetX, &offsetY, targetAngle);
            pos[0] += offsetX;
            pos[1] += offsetY;
            pos[2] = offsetY;
            value = -(float_0p1_80422458 * window->timers[i] - float_1_8042245c);
            if (value <= float_0_8042242c) window->states[i] = 11;
            PSMTXScale(scaleMtx, value, value, float_1_8042245c);
            PSMTXTrans(transMtx, pos[0], pos[1], pos[2]);
            PSMTXConcat(transMtx, scaleMtx, (void*)((s32)matrices + matrixOffset));
            *(f32*)(angles + itemOffset) = float_0_8042242c;
            window->timers[i] += 1;
            break;
        case 1:
            window->flags[i] |= 1;
            window->flags[i] |= 2;
            PSMTXTrans((void*)((s32)matrices + matrixOffset), float_neg40_80422468, float_55_80422454, float_0_8042242c);
            *(f32*)(angles + itemOffset) = float_360_80422444;
            window->show = 1;
            break;
        case 0:
            window->flags[i] |= 1;
            window->flags[i] |= 2;
            GetRingCenter(pos);
            pos[0] = (f32)intplGetValue(4, window->timers[i], *(s32*)((s32)gp + 4) * 12 / 60, pos[0], float_neg60_80422450);
            pos[1] = (f32)intplGetValue(4, window->timers[i], *(s32*)((s32)gp + 4) * 12 / 60, pos[1], float_55_80422454);
            PSMTXTrans((void*)((s32)matrices + matrixOffset), pos[0] + float_20_8042244c, pos[1], pos[2]);
            *(f32*)(angles + itemOffset) = (f32)intplGetValue(0, window->timers[i], *(s32*)((s32)gp + 4) * 12 / 60, float_0_8042242c, float_360_80422444);
            if (*(f32*)(angles + itemOffset) > float_360_80422444)
                *(f32*)(angles + itemOffset) -= float_360_80422444;
            if (window->timers[i] > 2)
                window->show = 1;
            if (++window->timers[i] > *(s32*)((s32)gp + 4) * 12 / 60)
                window->states[i] = 1;
            break;
        case 11:
            window->flags[i] &= ~1;
            break;
        }
    }

    for (i = 0, itemOffset = 0, matrixOffset = 0; i < 7;
         i++, itemOffset += 4, matrixOffset += 0x30) {
        state = window->subStates[i];
        switch (state) {
        case 0:
            window->subFlags[i] &= ~1;
            window->icons[i] = 0xFF;
            break;
        case 1:
            window->subFlags[i] &= ~1;
            window->icons[i] = 0xFF;
            if (window->subTimers[i] >= 0 && i != 6) {
                f32 rotation = intplGetValue(0, window->subTimers[i], *(s32*)((s32)gp + 4) * 12 / 60, float_90_80422440, float_0_8042242c);
                window->subFlags[i] |= 1;
                PSMTXRotRad((void*)((s32)outMatrices + matrixOffset), 'y',
                    (f32)(float_deg2rad_80422448 * rotation));
            }
            window->subTimers[i] += 1;
            if (window->subTimers[i] > *(s32*)((s32)gp + 4) * 12 / 60)
                window->subStates[i] = 2;
            break;
        case 2:
            window->icons[i] = 0xFF;
            if (i == 6) window->subFlags[i] &= ~1;
            else {
                window->subFlags[i] |= 1;
                PSMTXIdentity((void*)((s32)outMatrices + matrixOffset));
            }
            break;
        case 10:
        case 20:
        case 30:
        case 40:
            if (state == 10 || state == 20) duration = 12;
            if (state == 30 || state == 40)
                duration = (cursor->count - 6) / 2 + 12;
            window->subFlags[i] |= 1;
            PSMTXIdentity((void*)((s32)outMatrices + matrixOffset));
            window->scroll = (f32)intplGetValue(4, window->subTimers[i], duration, window->scrollStart, window->scrollEnd);
            window->subTimers[i] += 1;
            if (window->subTimers[i] > duration) {
                window->subStates[i] = 2;
                window->subTimers[i] = 0;
            }
            break;
        }
    }
}

void BattleMenuDisp_ActSelect_Init(void* battleWork, void* cursor, void* actionTable, u32 flags) {
    extern void BattleFree(void* ptr);
    extern void* BattleAlloc(s32 size);
    extern void* gp;
    typedef struct RingCursor { s32 absolute, relative, count; } RingCursor;
    typedef struct RingWindow {
        RingCursor* cursor;
        void* table;
        s32 reserved8;
        s32 state;
        u8 reserved10[4];
        u8 active;
        u8 reserved15[3];
        s32 previous;
        s32 reserved1c;
        s32 flags[5];
        s32 states[5];
        s32 timers[5];
        s32 reserved5c[5];
        s32 starts[5];
        s32 slots[5];
    } RingWindow;
    RingWindow* window;
    RingCursor* cur = cursor;
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
        window->cursor = cur;
        window->table = actionTable;
        window->state = 10;
        window->active = 0;
        window->previous = cur->absolute;
        index = cur->absolute;
        for (i = 0; i < cur->count; i++) {
            if (index == cur->absolute) {
                window->states[index] = 10;
            } else {
                window->states[index] = 11;
            }
            window->timers[index] = 0;
            window->slots[index] =
                cur->count + i - 1;
            if (window->slots[index] >=
                cur->count) {
                window->slots[index] -=
                    cur->count;
            }
            window->flags[index] = 0;
            index++;
            if (index >= cur->count) {
                index = 0;
            }
        }
    } else {
        window = *(void**)((s32)battleWork + 0x1C78);
        window->cursor = cur;
        window->table = actionTable;
        window->state = 0;
        window->active = 0;
        window->previous = cur->absolute;
        index = cur->absolute;
        for (i = 0; i < cur->count; i++) {
            index++;
            if (index >= cur->count) {
                index = 0;
            }
            window->states[index] = 0;
            value = i * (*(s32*)((s32)gp + 4) * 10 / 60);
            window->timers[index] = value / 2;
            window->slots[index] = i;
            window->flags[index] = 0;
        }
    }
}

void BattleMenuDisp_ActSelect_Main(void) {
    ;
}

void BattleMenuDisp_ActSelect_Disp(void) {
    DrawMainMenu();
}

void BattleMenuDisp_ActSelect_End(void* work) {
    void* ptr;

    ptr = *(void**)((s32)work + 0x1C78);
    if (ptr != 0) {
        BattleFree(ptr);
        *(void**)((s32)work + 0x1C78) = 0;
    }
}

void DrawMainMenu(void) {
    extern void* _battleWorkPointer;
    extern void* gp;
    extern void camGetPtr(s32);
    extern void GetRingCenter(f32*);
    extern void GetRingOffset(f32*, f32*, f32);
    extern double intplGetValue(double, double, s32, s32, s32);
    extern void PSMTXRotRad(void*, char, f32);
    extern void PSMTXTrans(void*, f32, f32, f32);
    extern void PSMTXScale(void*, f32, f32, f32);
    extern void PSMTXConcat(void*, void*, void*);
    extern void DrawMainIcon(void*, void*, void*, void*, void*);
    extern void DrawMenuPartyChangeButton(void);
    extern void DrawMenuPinchMark(void);
    extern f32 kakutable[15];
    extern const f32 float_3p1416_80422434;
    extern const f32 float_180_80422438;
    extern const f32 float_12_80422430;
    extern const f32 float_270_8042243c;
    extern const f32 float_90_80422440;
    extern const f32 float_360_80422444;
    extern const f32 float_deg2rad_80422448;
    extern const f32 float_20_8042244c;
    extern const f32 float_0p1_80422458;
    extern const f32 float_1_8042245c;
    extern const f32 float_0_8042242c;
    extern const f32 float_neg60_80422450;
    extern const f32 float_55_80422454;
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
    switch (state) {
    case 0:
        settled = 0;
        count = *(s32*)((s32)cursor + 8);
        for (i = 0; i < *(s32*)((s32)cursor + 8); i++) {
            if (*(s32*)((s32)window + 0x34 + i * 4) == 3) settled++;
        }
        if (settled == count) *(s32*)((s32)window + 0xC) = 1;
        break;
    case 1:
        *(u8*)((s32)window + 0x14) = 1;
        current = **(s32**)window;
        previous = *(s32*)((s32)window + 0x18);
        count = *(s32*)((s32)cursor + 8);
        delta = current - previous;
        if (delta == 1 || delta == -(count - 1)) {
            *(s32*)((s32)window + 0xC) = 2;
            *(u8*)((s32)window + 0x14) = 0;
            *(s32*)((s32)window + 0x18) = **(s32**)window;
            for (i = 0; i < *(s32*)((s32)cursor + 8); i++) {
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
            *(s32*)((s32)window + 0x18) = **(s32**)window;
            for (i = 0; i < *(s32*)((s32)cursor + 8); i++) {
                *(s32*)((s32)window + 0x34 + i * 4) = 5;
                *(s32*)((s32)window + 0x48 + i * 4) = 0;
                *(s32*)((s32)window + 0x70 + i * 4) =
                    *(s32*)((s32)window + 0x84 + i * 4);
                if (++*(s32*)((s32)window + 0x84 + i * 4) == count)
                    *(s32*)((s32)window + 0x84 + i * 4) = 0;
            }
        }
        break;
    case 2:
        settled = 0;
        count = *(s32*)((s32)cursor + 8);
        for (i = 0; i < *(s32*)((s32)cursor + 8); i++) {
            if (*(s32*)((s32)window + 0x34 + i * 4) == 3) settled++;
        }
        if (settled == count) *(s32*)((s32)window + 0xC) = 1;
        break;
    case 10:
        *(s32*)((s32)window + 0xC) = 1;
        *(u8*)((s32)window + 0x14) = 1;
        break;
    }

    *(s32*)((s32)window + 0x1C) = 0;
    for (i = 0; i < *(s32*)((s32)cursor + 8); i++) {
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
            targetAngle = float_3p1416_80422434 * *(f32*)((s32)kakutable - 0x3C + *(s32*)((s32)cursor + 8) * 20 +
                *(s32*)((s32)window + 0x84 + i * 4) * 4) / float_180_80422438;
            currentAngle = *(s32*)((s32)window + 0x48 + i * 4) *
                           (float_12_80422430 / *(s32*)((s32)gp + 4));
            currentAngle += float_0_8042242c;
            if (targetAngle <= currentAngle) currentAngle = targetAngle;
            GetRingCenter(positions[i]);
            GetRingOffset(&offsetX, &offsetY, currentAngle);
            positions[i][0] += offsetX;
            positions[i][1] += offsetY;
            positions[i][2] = offsetY;
            if (i == *(s32*)cursor) {
                value = float_270_8042243c + float_90_80422440 * *(s32*)((s32)window + 0x48 + i * 4) /
                        (*(s32*)((s32)gp + 4) * 10 / 60);
                if (value >= float_360_80422444) value = float_0_8042242c;
                PSMTXRotRad(rot, 'x', value * float_deg2rad_80422448);
                PSMTXTrans(trans, positions[i][0] + float_20_8042244c,
                           positions[i][1], positions[i][2]);
                PSMTXConcat(trans, rot, matrices[i]);
                *(u32*)((s32)window + 0x20 + i * 4) |= 2;
            } else {
                value = float_270_8042243c + float_90_80422440 * *(s32*)((s32)window + 0x48 + i * 4) /
                        (*(s32*)((s32)gp + 4) * 10 / 60);
                if (value >= float_360_80422444) value = float_0_8042242c;
                PSMTXRotRad(rot, 'x', value * float_deg2rad_80422448);
                PSMTXTrans(trans, positions[i][0], positions[i][1], positions[i][2]);
                PSMTXConcat(trans, rot, matrices[i]);
            }
            angles[i] = float_0_8042242c;
            if (currentAngle >= targetAngle &&
                *(s32*)((s32)window + 0x48 + i * 4) >= *(s32*)((s32)gp + 4) * 10 / 60) {
                *(s32*)((s32)window + 0x34 + i * 4) = 3;
                *(s32*)((s32)window + 0x48 + i * 4) = -1;
            }
            *(s32*)((s32)window + 0x48 + i * 4) += 1;
            break;
        case 3:
            targetAngle = float_3p1416_80422434 * *(f32*)((s32)kakutable - 0x3C + *(s32*)((s32)cursor + 8) * 20 +
                *(s32*)((s32)window + 0x84 + i * 4) * 4) / float_180_80422438;
            GetRingCenter(positions[i]);
            GetRingOffset(&offsetX, &offsetY, targetAngle);
            positions[i][0] += offsetX;
            positions[i][1] += offsetY;
            positions[i][2] = offsetY;
            if (i == *(s32*)cursor) {
                PSMTXTrans(matrices[i], positions[i][0] + float_20_8042244c, positions[i][1], positions[i][2]);
                *(u32*)((s32)window + 0x20 + i * 4) |= 2;
            } else {
                PSMTXTrans(matrices[i], positions[i][0], positions[i][1], positions[i][2]);
            }
            angles[i] = float_0_8042242c;
            break;
        case 4:
            targetAngle = *(f32*)((s32)kakutable - 0x3C + *(s32*)((s32)cursor + 8) * 20 +
                *(s32*)((s32)window + 0x84 + i * 4) * 4);
            if (targetAngle == float_0_8042242c) {
                targetAngle = float_360_80422444;
            }
            targetAngle = float_3p1416_80422434 * targetAngle / float_180_80422438;
            currentAngle = *(s32*)((s32)window + 0x48 + i * 4) *
                           (float_12_80422430 / *(s32*)((s32)gp + 4));
            currentAngle += float_3p1416_80422434 * *(f32*)((s32)kakutable - 0x3C + *(s32*)((s32)cursor + 8) * 20 +
                *(s32*)((s32)window + 0x70 + i * 4) * 4) / float_180_80422438;
            if (currentAngle >= targetAngle) {
                *(s32*)((s32)window + 0x34 + i * 4) = 3;
                *(s32*)((s32)window + 0x48 + i * 4) = -1;
                currentAngle = targetAngle;
            }
            GetRingCenter(positions[i]);
            GetRingOffset(&offsetX, &offsetY, currentAngle);
            positions[i][0] += offsetX;
            positions[i][1] += offsetY;
            positions[i][2] = offsetY;
            PSMTXTrans(matrices[i], positions[i][0], positions[i][1], positions[i][2]);
            angles[i] = float_0_8042242c;
            *(u32*)((s32)window + 0x20 + i * 4) &= ~2;
            *(s32*)((s32)window + 0x48 + i * 4) += 1;
            break;
        case 5:
            currentAngle = *(f32*)((s32)kakutable - 0x3C + *(s32*)((s32)cursor + 8) * 20 +
                *(s32*)((s32)window + 0x70 + i * 4) * 4);
            if (currentAngle == float_0_8042242c) {
                currentAngle = float_360_80422444;
            }
            targetAngle = float_3p1416_80422434 * *(f32*)((s32)kakutable - 0x3C + *(s32*)((s32)cursor + 8) * 20 +
                *(s32*)((s32)window + 0x84 + i * 4) * 4) / float_180_80422438;
            currentAngle = -(*(s32*)((s32)window + 0x48 + i * 4) *
                           (float_12_80422430 / *(s32*)((s32)gp + 4)) -
                           float_3p1416_80422434 * currentAngle / float_180_80422438);
            if (currentAngle <= targetAngle) {
                *(s32*)((s32)window + 0x34 + i * 4) = 3;
                *(s32*)((s32)window + 0x48 + i * 4) = -1;
                currentAngle = targetAngle;
            }
            GetRingCenter(positions[i]);
            GetRingOffset(&offsetX, &offsetY, currentAngle);
            positions[i][0] += offsetX;
            positions[i][1] += offsetY;
            positions[i][2] = offsetY;
            PSMTXTrans(matrices[i], positions[i][0], positions[i][1], positions[i][2]);
            angles[i] = float_0_8042242c;
            *(u32*)((s32)window + 0x20 + i * 4) &= ~2;
            *(s32*)((s32)window + 0x48 + i * 4) += 1;
            break;
        case 10:
            *(u32*)((s32)window + 0x20 + i * 4) |= 1;
            *(u32*)((s32)window + 0x20 + i * 4) |= 2;
            GetRingCenter(positions[i]);
            GetRingOffset(&offsetX, &offsetY, float_0_8042242c);
            positions[i][0] += offsetX;
            positions[i][1] += offsetY;
            positions[i][2] = offsetY;
            positions[i][0] = (f32)intplGetValue(float_neg60_80422450, positions[i][0], 4,
                *(s32*)((s32)window + 0x48 + i * 4), *(s32*)((s32)gp + 4) * 12 / 60);
            positions[i][1] = (f32)intplGetValue(float_55_80422454, positions[i][1], 4,
                *(s32*)((s32)window + 0x48 + i * 4), *(s32*)((s32)gp + 4) * 12 / 60);
            PSMTXTrans(matrices[i], positions[i][0] + float_20_8042244c,
                       positions[i][1], positions[i][2]);
            angles[i] = (f32)intplGetValue(float_360_80422444, float_0_8042242c, 0,
                *(s32*)((s32)window + 0x48 + i * 4), *(s32*)((s32)gp + 4) * 12 / 60);
            if (++*(s32*)((s32)window + 0x48 + i * 4) > *(s32*)((s32)gp + 4) * 12 / 60)
                *(s32*)((s32)window + 0x34 + i * 4) = 3;
            break;
        case 11:
            *(u32*)((s32)window + 0x20 + i * 4) |= 1;
            *(u32*)((s32)window + 0x20 + i * 4) &= ~2;
            targetAngle = float_3p1416_80422434 * *(f32*)((s32)kakutable - 0x3C + *(s32*)((s32)cursor + 8) * 20 +
                *(s32*)((s32)window + 0x84 + i * 4) * 4) / float_180_80422438;
            GetRingCenter(positions[i]);
            GetRingOffset(&offsetX, &offsetY, targetAngle);
            positions[i][0] += offsetX;
            positions[i][1] += offsetY;
            positions[i][2] = offsetY;
            value = float_0p1_80422458 * *(s32*)((s32)window + 0x48 + i * 4);
            if (value >= float_1_8042245c) *(s32*)((s32)window + 0x34 + i * 4) = 3;
            PSMTXScale(scale, value, value, float_1_8042245c);
            PSMTXTrans(trans, positions[i][0], positions[i][1], positions[i][2]);
            PSMTXConcat(trans, scale, matrices[i]);
            angles[i] = float_0_8042242c;
            *(s32*)((s32)window + 0x48 + i * 4) += 1;
            break;
        }
    }
    DrawMainIcon(window, *(void**)((s32)window + 4), matrices, positions, angles);
    DrawMenuPartyChangeButton();
    DrawMenuPinchMark();
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

void BattleMenuDisp_WeaponSelect_Main(void) {
    ;
}

void BattleMenuDisp_WeaponSelect_Disp(void) {
    DrawWeaponWin();
}

void BattleMenuDisp_WeaponSelect_End(void* work) {
    void* ptr;

    ptr = *(void**)((s32)work + 0x1C78);
    if (ptr != 0) {
        BattleFree(ptr);
        *(void**)((s32)work + 0x1C78) = 0;
    }
}

void DrawWeaponWin(void) {
    extern void DrawSubMenuCommonProcess(f32*, f32*, void*, void*, void*, void*);
    extern void DrawSubIcon(void*, void*, u32*, u16*, char**, u32*, u32*, void*, s32);
    extern void DrawMainIcon(void*, void*, void*, void*, void*);
    extern u8 DrawMenuCursorAndScrollArrow(f32*);
    extern u32 BtlUnit_GetWeaponCost(void*, void*);
    extern void DrawMenuHelpWin(f32, f32, char*);
    extern const f32 vec3_802ef750[3];
    typedef struct CursorPosition { f32 x, y, z; } CursorPosition;
    extern u8 itemDataTable[];
    extern u8 ItemWeaponData_CookingItem[];
    void* battleWork = _battleWorkPointer;
    u8* window = *(u8**)((s32)battleWork + 0x1C78);
    s32* cursor = *(s32**)window;
    u8* weaponTable = (u8*)battleWork + 0x171C;
    void* unit = BattleGetUnitPtr(battleWork, *(s32*)((s32)battleWork + 0x420));
    f32 outX, outY;
    CursorPosition cursorPos;
    u16 icons[8];
    u32 spCosts[7], costs[7];
    char* names[7];
    u32 enabled[7];
    u8 angles[20], positions[60], mainMatrices[5 * 48], subMatrices[7 * 48];
    s32 i, index;
    void* selectedWeapon = *(void**)(*(u8**)(window + 0x8) + cursor[0] * 0x1C);
    char* helpMsg;

    DrawSubMenuCommonProcess(&outX, &outY, mainMatrices, positions, angles, subMatrices);
    for (i = 0; i < 7; i++) {
        index = (s32)*(f32*)(window + 0x98) + i;
        if (index < cursor[2]) {
            enabled[i] = *(u32*)(*(u8**)(window + 0x8) + index * 0x1C + 0x4);
            icons[i] = *(u16*)(weaponTable + index * 0x1C + 0x8C);
            names[i] = *(char**)(*(u8**)(window + 0x8) + index * 0x1C + 0x8);
            if (*(void**)(*(u8**)(window + 0x8) + index * 0x1C) != NULL) {
                costs[i] = BtlUnit_GetWeaponCost(unit, *(void**)(*(u8**)(window + 0x8) + index * 0x1C));
            } else {
                costs[i] = 0;
            }
            if (*(void**)(*(u8**)(window + 0x8) + index * 0x1C) != NULL) {
                spCosts[i] = *(u8*)(*(s32*)(*(u8**)(window + 0x8) + index * 0x1C) + 0x12);
            } else {
                spCosts[i] = 0;
            }
        }
    }
    DrawMainIcon(window, (u8*)battleWork + 0x1724, mainMatrices, positions, angles);
    DrawSubIcon(window, subMatrices, enabled, icons, names, costs, spCosts, NULL, 0);
    cursorPos = *(const CursorPosition*)vec3_802ef750;
    DrawMenuCursorAndScrollArrow((f32*)&cursorPos);
    if (selectedWeapon != NULL) {
        index = *(s32*)((s32)selectedWeapon + 8);
        if (index != 0) {
            helpMsg = *(char**)(itemDataTable + index * 0x28 + 8);
        } else
        if (selectedWeapon == ItemWeaponData_CookingItem) {
            index = *(s32*)(*(u8**)(window + 0x8) + cursor[0] * 0x1C + 0x14);
            helpMsg = *(char**)(itemDataTable + index * 0x28 + 8);
        } else {
            helpMsg = *(char**)((s32)selectedWeapon + 0xC);
        }
    } else {
        index = *(s32*)(*(u8**)(window + 0x8) + cursor[0] * 0x1C + 0x14);
        helpMsg = *(char**)(itemDataTable + index * 0x28 + 8);
    }
    if (helpMsg != NULL) {
        DrawMenuHelpWin(outX, outY, helpMsg);
    }
}

void BattleMenuDisp_Operation_Init(void* work, s32 flags) {
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

void BattleMenuDisp_Operation_Main(void) {
    ;
}

void BattleMenuDisp_Operation_Disp(void) {
    DrawOperationWin();
}

void BattleMenuDisp_Operation_End(void* work) {
    void* ptr;

    ptr = *(void**)((s32)work + 0x1C78);
    if (ptr != 0) {
        BattleFree(ptr);
        *(void**)((s32)work + 0x1C78) = 0;
    }
}

void DrawOperationWin(void) {
    extern void DrawSubMenuCommonProcess(f32*, f32*, void*, void*, void*, void*);
    extern void DrawMainIcon(void*, void*, void*, void*, void*);
    extern void DrawSubIcon(void*, void*, u32*, u16*, char**, u32*, u32*, void*, s32);
    extern const f32 vec3_802ef75c[3];
    typedef struct CursorPosition { f32 x, y, z; } CursorPosition;
    void* battleWork = _battleWorkPointer;
    u8* window = *(u8**)((s32)battleWork + 0x1C78);
    s32* cursor = *(s32**)window;
    u8* table;
    f32 outX;
    f32 outY;
    CursorPosition cursorPos;
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
    table = (u8*)battleWork + 0x19E8;
    DrawSubMenuCommonProcess(&outX, &outY, mainMatrices, positions, angles, subMatrices);
    for (i = 0; i < 7; i++) {
        index = (s32)*(f32*)(window + 0x98) + i;
        if (index < cursor[2]) {
            entry = table + index * 0x1C;
            enabled[i] = *(u32*)(entry + 0x4);
            if (*(u16*)(table + i * 0x1C + 0x10) != 0) icons[i] = *(u16*)(entry + 0x10);
            else icons[i] = 0x193;
            names[i] = *(char**)(entry + 0xC);
            costs[i] = *(u32*)(entry + 0x18);
            spCosts[i] = 0;
        }
    }
    DrawMainIcon(window, (u8*)battleWork + 0x1724, mainMatrices, positions, angles);
    DrawSubIcon(window, subMatrices, enabled, icons, names, costs, spCosts, NULL, 0);
    cursorPos = *(const CursorPosition*)vec3_802ef75c;
    DrawMenuCursorAndScrollArrow((f32*)&cursorPos);
    entry = table + cursor[0] * 0x1C;
    if (*(char**)(entry + 0x14) != NULL) {
        DrawMenuHelpWin(outX, outY, *(char**)(entry + 0x14));
    }
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

void BattleMenuDisp_ChangePartySelect_Main(void) {
    ;
}

void BattleMenuDisp_ChangePartySelect_Disp(void) {
    DrawChangePartyWin();
}

void BattleMenuDisp_ChangePartySelect_End(void* work) {
    void* ptr;

    ptr = *(void**)((s32)work + 0x1C78);
    if (ptr != 0) {
        BattleFree(ptr);
        *(void**)((s32)work + 0x1C78) = 0;
    }
}

void DrawChangePartyWin(void) {
    extern void DrawSubMenuCommonProcess(f32*, f32*, void*, void*, void*, void*);
    extern void DrawMainIcon(void*, void*, void*, void*, void*);
    extern void DrawSubIcon(void*, void*, u32*, u16*, char**, u32*, u32*, void*, s32);
    extern const f32 vec3_802ef768[3];
    typedef struct CursorPosition { f32 x, y, z; } CursorPosition;
    void* battleWork = _battleWorkPointer;
    u8* window = *(u8**)((s32)battleWork + 0x1C78);
    s32* cursor = *(s32**)window;
    u8* table;
    f32 outX;
    f32 outY;
    CursorPosition cursorPos;
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
    table = (u8*)battleWork + 0x1AAC;
    DrawSubMenuCommonProcess(&outX, &outY, mainMatrices, positions, angles, subMatrices);
    for (i = 0; i < 7; i++) {
        index = (s32)*(f32*)(window + 0x98) + i;
        if (index < cursor[2]) {
            u16 icon;
            entry = table + index * 0x18;
            enabled[i] = *(u32*)(entry + 0x4);
            icon = *(u16*)(entry + 0xC);
            if (icon != 0) icons[i] = icon;
            else icons[i] = 0x193;
            names[i] = *(char**)(entry + 0x8);
            costs[i] = 0;
            spCosts[i] = 0;
        }
    }
    DrawMainIcon(window, (u8*)battleWork + 0x1724, mainMatrices, positions, angles);
    DrawSubIcon(window, subMatrices, enabled, icons, names, costs, spCosts, table, 1);
    cursorPos = *(const CursorPosition*)vec3_802ef768;
    DrawMenuCursorAndScrollArrow((f32*)&cursorPos);
    entry = table + cursor[0] * 0x18;
    if (*(char**)(entry + 0x10) != NULL) {
        DrawMenuHelpWin(outX, outY, *(char**)(entry + 0x10));
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

void BattleMenuDisp_MultiItem_Main(void) {
    ;
}

void BattleMenuDisp_MultiItem_Disp(void) {
    DrawMultiItemWin();
}

void BattleMenuDisp_MultiItem_End(void* work) {
    void* ptr;

    ptr = *(void**)((s32)work + 0x1C78);
    if (ptr != 0) {
        BattleFree(ptr);
        *(void**)((s32)work + 0x1C78) = 0;
    }
}

void DrawMultiItemWin(void) {
    extern void DrawSubMenuCommonProcess(f32*, f32*, void*, void*, void*, void*);
    extern void DrawMainIcon(void*, void*, void*, void*, void*);
    extern void DrawSubIcon(void*, void*, u32*, u16*, char**, u32*, u32*, void*, s32);
    extern const f32 vec3_802ef774[3];
    typedef struct CursorPosition { f32 x, y, z; } CursorPosition;
    void* battleWork = _battleWorkPointer;
    u8* window = *(u8**)((s32)battleWork + 0x1C78);
    s32* cursor = *(s32**)window;
    u8* table;
    f32 outX;
    f32 outY;
    CursorPosition cursorPos;
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
    table = (u8*)battleWork + 0x1B6C;
    DrawSubMenuCommonProcess(&outX, &outY, mainMatrices, positions, angles, subMatrices);
    for (i = 0; i < 7; i++) {
        index = (s32)*(f32*)(window + 0x98) + i;
        if (index < cursor[2]) {
            u16 icon;
            entry = table + index * 0x18;
            enabled[i] = *(u32*)(entry + 0x0);
            icon = *(u16*)(entry + 0xC);
            if (icon != 0) icons[i] = icon;
            else icons[i] = 0x193;
            names[i] = *(char**)(entry + 0x4);
            costs[i] = *(u32*)(entry + 0x10);
            spCosts[i] = 0;
        }
    }
    DrawMainIcon(window, (u8*)battleWork + 0x1724, mainMatrices, positions, angles);
    DrawSubIcon(window, subMatrices, enabled, icons, names, costs, spCosts, NULL, 0);
    cursorPos = *(const CursorPosition*)vec3_802ef774;
    DrawMenuCursorAndScrollArrow((f32*)&cursorPos);
    entry = table + cursor[0] * 0x18;
    if (*(char**)(entry + 0x8) != NULL) {
        DrawMenuHelpWin(outX, outY, *(char**)(entry + 0x8));
    }
}

const f32 dat_802ef650[32] = {
    -20.0f, 40.0f, 0.0f, 40.0f, 0.0f, 20.0f, -20.0f, 20.0f,
    20.0f, 40.0f, 20.0f, 20.0f, 0.0f, 20.0f, 0.0f, 40.0f,
    -20.0f, 20.0f, 0.0f, 20.0f, 0.0f, 0.0f, -20.0f, 0.0f,
    0.0f, 20.0f, 20.0f, 20.0f, 20.0f, 0.0f, 0.0f, 0.0f,
};

const f32 dat_802ef6d0[32] = {
    0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
};

const f32 vec3_802ef750[3] = {-60.0f, 35.0f, 0.0f};
const f32 vec3_802ef75c[3] = {-60.0f, 35.0f, 0.0f};
const f32 vec3_802ef768[3] = {-60.0f, 35.0f, 0.0f};
const f32 vec3_802ef774[3] = {-60.0f, 35.0f, 0.0f};
const f64 double_to_int_802ef780 = 4503601774854144.0;
const u8 str_no_description_802ef788[25] = {
    0x82, 0xB9, 0x82, 0xC2, 0x82, 0xDF, 0x82, 0xA2,
    0x82, 0xCD, 0x81, 0x40, 0x82, 0xC8, 0x82, 0xA2,
    0x82, 0xCC, 0x82, 0xBE, 0x81, 0x49, 0x81, 0x49,
    0x00,
};
const char str_SFX_BTL_SYS_CAUTION_802ef7a4[] = "SFX_BTL_SYS_CAUTION_THROW1";
const char str_btl_cost_disp_FP_802ef7c0[] = "btl_cost_disp_FP";
const char str_btl_cost_disp_AP_802ef7d4[] = "btl_cost_disp_AP";
const char str_btl_disp_HP_802ef7e8[] = "btl_disp_HP";
const char str_btl_disp_slash_802ef7f4[] = "btl_disp_slash";
const f64 double_to_int_mask_802ef808 = 4503599627370496.0;
const char str_PCTs_battle_common_b_802ef810[] = "%s/battle/common/battle_common.tpl";

f32 kakutable[15] = {
    240.0f, 120.0f, 0.0f, 0.0f, 0.0f,
    270.0f, 180.0f, 90.0f, 0.0f, 0.0f,
    270.0f, 203.0f, 157.0f, 90.0f, 0.0f,
};

u8 seleItemCoordCol[4] = {0xFF, 0xC0, 0xC0, 0xCF};
u16 danger_icon_tbl[6] = {6, 7, 8, 7, 9, 10};
u16 peril_icon_tbl[6] = {11, 12, 13, 14, 15, 16};
s8 seq_510;

const u32 dat_80422400 = 0xE1E1E1FF;
const u32 dat_80422404 = 0x000000FF;
const u32 dat_80422408 = 0x808080FF;
const u32 dat_8042240c = 0xE1E1E1FF;
const u32 dat_80422410 = 0x000000FF;
const u32 dat_80422414 = 0x000000FF;
const u32 dat_80422418 = 0xFFFFFF00;
const u32 dat_8042241c = 0xFFFFFF00;
const f32 float_3p0039_80422420 = 3.00390625f;
const u32 dat_80422424 = 0xFFFFFFFF;
const u32 dat_80422428 = 0xFFFFFFCF;
const f32 float_0_8042242c = 0.0f;
const f32 float_12_80422430 = 12.0f;
const f32 float_3p1416_80422434 = 3.1415927410125732f;
const f32 float_180_80422438 = 180.0f;
const f32 float_270_8042243c = 270.0f;
const f32 float_90_80422440 = 90.0f;
const f32 float_360_80422444 = 360.0f;
const f32 float_deg2rad_80422448 = 0.01745329238474369f;
const f32 float_20_8042244c = 20.0f;
const f32 float_neg60_80422450 = -60.0f;
const f32 float_55_80422454 = 55.0f;
const f32 float_0p1_80422458 = 0.1f;
const f32 float_1_8042245c = 1.0f;
const f32 float_neg200_80422460 = -200.0f;
const f32 float_neg100_80422464 = -100.0f;
const f32 float_neg40_80422468 = -40.0f;
const f32 float_10_8042246c = 10.0f;
const f32 float_155_80422470 = 155.0f;
const f32 float_15_80422474 = 15.0f;
const f32 float_0p75_80422478 = 0.75f;
const f32 float_164_8042247c = 164.0f;
const f32 float_24_80422480 = 24.0f;
const f32 float_400_80422484 = 400.0f;
const f32 float_50_80422488 = 50.0f;
const f32 float_0p8_8042248c = 0.8f;
const f32 float_4_80422490 = 4.0f;
const f32 float_28_80422494 = 28.0f;
const f32 float_52_80422498 = 52.0f;
const f32 float_neg120_8042249c = -120.0f;
const f32 float_60_804224a0 = 60.0f;
const f32 float_1p5_804224a4 = 1.5f;
const f32 float_neg28_804224a8 = -28.0f;
const f32 float_neg12_804224ac = -12.0f;
const f32 float_5_804224b0 = 5.0f;
const f32 float_0p5_804224b4 = 0.5f;
const f32 float_255_804224b8 = 255.0f;
const f32 float_144_804224bc = 144.0f;
const f32 float_neg152_804224c0 = -152.0f;
const f32 float_neg136_804224c4 = -136.0f;
const f32 float_152_804224c8 = 152.0f;
const f32 float_136_804224cc = 136.0f;
const f32 float_neg119_804224d0 = -119.0f;
const f32 float_2_804224d4 = 2.0f;
const f32 float_neg104_804224d8 = -104.0f;
const f32 float_22_804224dc = 22.0f;
const f32 float_120_804224e0 = 120.0f;
const f32 float_72_804224e4 = 72.0f;
const f32 float_25_804224e8 = 25.0f;
const f32 float_30_804224ec = 30.0f;
const f32 float_26_804224f0 = 26.0f;
const f32 float_92_804224f4 = 92.0f;
const f32 float_100_804224f8 = 100.0f;
const f32 float_76_804224fc = 76.0f;
const f32 float_78_80422500 = 78.0f;
const f32 float_35_80422504 = 35.0f;
const f32 float_1p3_80422508 = 1.3f;
const f32 float_0p005_8042250c = 0.005f;
const f32 float_8_80422510 = 8.0f;
const f32 float_neg4_80422514 = -4.0f;
const f32 float_neg3p0769_80422518 = -3.076923131942749f;
const f32 float_7p5_8042251c = 7.5f;
const f32 float_0p66_80422520 = 0.66f;
const f32 float_neg5_80422524 = -5.0f;
const f32 float_64_80422528 = 64.0f;
const f32 float_neg64_8042252c = -64.0f;
const f32 float_80_80422530 = 80.0f;
const f32 float_neg67_80422534 = -67.0f;
const f32 float_52p5_80422538 = 52.5f;
const f32 float_0p9_8042253c = 0.9f;
const f32 float_4p7124_80422540 = 4.71238899230957f;
const f32 float_22p5_80422544 = 22.5f;
const f32 float_105_80422548 = 105.0f;
const f32 float_neg180_8042254c = -180.0f;
