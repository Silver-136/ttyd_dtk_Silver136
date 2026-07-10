#include "battle/battle_stage.h"

extern void* wp;
extern void* _battleWorkPointer;

void BattleFree(void* ptr);

typedef struct BattleStageVec {
    f32 x;
    f32 y;
    f32 z;
} BattleStageVec;

extern BattleStageVec vec3_802f37b4[];
extern BattleStageVec vec3_802f37c0[];
extern BattleStageVec vec3_802f37cc[];
extern u32 vec3_802f37a8[];
extern u32 unk_804295c0;
extern u32 unk_804295c4;
extern u32 unk_804295c8;
extern u32 unk_804295cc;
extern u32 unk_804295d4;
extern u32 dat_80422ad0;
extern u32 dat_80422ad4;

void BattleStageOffLightEnemyInCommand(void) {
    ;
}

void BattleStageOffLightFriendInCommand(void) {
    ;
}

void BattleStageOffLightInCommand(void) {
    ;
}

void BattleStageOnLightEnemyInCommand(void) {
    ;
}

void BattleStageOnLightFriendInCommand(void) {
    ;
}

void BattleStageOnLightInCommand(void) {
    ;
}

void* BatSpotGetPtr(void) {
    return wp;
}

void* BattleStageGetPtr(void) {
    return (void*)((s32)_battleWorkPointer + 0x163FC);
}

void BattleStageSetDark(void* dark, s32 value, s32 frames, s32 type) {
    *(u8*)((s32)dark + 0x6) = *(u8*)((s32)dark + 0x4);
    *(u8*)((s32)dark + 0x8) = value;
    *(s32*)((s32)dark + 0xC) = frames;
    *(s32*)((s32)dark + 0x14) = frames;
    *(u8*)((s32)dark + 0x1C) = type;

    if (frames == 0) {
        *(u8*)((s32)dark + 0x4) = value;
    }
}

void BattleStageSetDarkBase(void* dark, s32 value, s32 frames, s32 type) {
    *(u8*)((s32)dark + 0x7) = *(u8*)((s32)dark + 0x5);
    *(u8*)((s32)dark + 0x9) = value;
    *(s32*)((s32)dark + 0x10) = frames;
    *(s32*)((s32)dark + 0x18) = frames;
    *(u8*)((s32)dark + 0x1D) = type;

    if (frames == 0) {
        *(u8*)((s32)dark + 0x5) = value;
    }
}

void BattleStageSetAudDark(s32 value, s32 frames, s32 type) {
    void* workHi = (void*)((s32)_battleWorkPointer + 0x10000);
    BattleStageSetDark((void*)((s32)workHi + 0x6F00), value, frames, type);
}

void BattleStageSetAudDarkBase(s32 value, s32 frames, s32 type) {
    void* workHi = (void*)((s32)_battleWorkPointer + 0x10000);
    BattleStageSetDarkBase((void*)((s32)workHi + 0x6F00), value, frames, type);
}

void BattleStageSetStgDark(s32 value, s32 frames, s32 type) {
    void* workHi = (void*)((s32)_battleWorkPointer + 0x10000);
    BattleStageSetDark((void*)((s32)workHi + 0x6EE0), value, frames, type);
}

void BattleStageSetStgDarkBase(s32 value, s32 frames, s32 type) {
    void* workHi = (void*)((s32)_battleWorkPointer + 0x10000);
    BattleStageSetDarkBase((void*)((s32)workHi + 0x6EE0), value, frames, type);
}

void BattleStageReturnAudDarkBase(s32 frames, s32 type) {
    void* workHi = (void*)((s32)_battleWorkPointer + 0x10000);
    void* dark = (void*)((s32)workHi + 0x6F00);
    BattleStageSetDark(dark, *(u8*)((s32)dark + 5), frames, type);
}

void BattleStageReturnStgDarkBase(s32 frames, s32 type) {
    void* workHi = (void*)((s32)_battleWorkPointer + 0x10000);
    void* dark = (void*)((s32)workHi + 0x6EE0);
    BattleStageSetDark(dark, *(u8*)((s32)dark + 5), frames, type);
}

void unk_80137fe4(void) {
    BattleFree(*(void**)((s32)wp + 0x4));
}

void* BattleGetUnitPtr(void* battleWork, s32 unitId);
void dispEntry(s32 cameraId, s32 priority, void* callback, f32 z, void* param);

void screen_capture(void);
void BattleStageDispDark(void);
void BattleStageDispLight(void);

void DarkByPoint(s32 alpha, f32 x, f32 y, f32 z) {
    volatile f32* fifoF;
    volatile u8* fifoB;

    fifoF = (volatile f32*)0xCC008000;
    fifoB = (volatile u8*)0xCC008000;

    *fifoF = x;
    *fifoF = y;
    *fifoF = z;
    *fifoB = 0;
    *fifoB = 0;
    *fifoB = 0;
    *fifoB = alpha;
}

s32 screen_evt_func(void) {
    dispEntry(8, 0, screen_capture, 1000.0f, 0);
    return 0;
}

void BattleStageDisp(void) {
    extern f32 float_5000_80422b34;
    extern f32 float_6000_80422b38;
    void* battleWork;

    battleWork = _battleWorkPointer;

    BattleGetUnitPtr(battleWork, *(s32*)((s32)battleWork + 0x420));

    dispEntry(4, 2, BattleStageDispDark, float_5000_80422b34, 0);
    dispEntry(4, 2, BattleStageDispLight, float_6000_80422b38, 0);
}

s32 strcmp(const char* s1, const char* s2);

void BattleStageLightSetWidth(s32 index, s32 width, s32 frames, s32 type) {
    void* workHi;
    void* stage;
    void* light;
    s32 offset;

    offset = index * 0xE8;
    workHi = (void*)((s32)_battleWorkPointer + 0x10000);
    stage = (void*)((s32)workHi + 0x63FC);
    light = (void*)((s32)stage + offset + 4);

    *(f32*)((s32)light + 0x40) = *(f32*)((s32)light + 0x3C);
    *(f32*)((s32)light + 0x44) = width;
    *(s32*)((s32)light + 0x48) = frames;
    *(s32*)((s32)light + 0x4C) = frames;
    *(u8*)((s32)light + 0x50) = type;

    if (frames == 0) {
        *(f32*)((s32)light + 0x3C) = width;
    }
}

s32 BattleStageGetLightNumberByName(const char* name) {
    s32 offset;
    s32 i;
    void* stage;
    void* light;

    i = 0;
    offset = 0;
    stage = (void*)((s32)_battleWorkPointer + 0x163FC);

    while (i < 0xC) {
        light = (void*)((s32)stage + offset + 4);

        if ((*(u32*)light & 1) != 0) {
            if (strcmp((const char*)((s32)light + 4), name) == 0) {
                return i;
            }
        }

        i++;
        offset += 0xE8;
    }

    return -1;
}


void BattleStageMain(void) {
    ;
}


u8 tou_gamen_screen_tev(int param_1) {
    return 0;
}


void BattleStageDispLight(void) {
    ;
}


void BattleStageDispDark(void) {
    extern void* camGetPtr(s32 cameraId);
    extern void DarkBySquare(s32 alpha, f32 x0, f32 y0, f32 x1, f32 y1, f32 z);
    extern void GXSetCullMode(s32 mode);
    extern void GXSetZCompLoc(s32 beforeTex);
    extern void GXSetAlphaCompare(s32 comp0, s32 ref0, s32 op, s32 comp1, s32 ref1);
    extern void GXSetBlendMode(s32 type, s32 srcFactor, s32 dstFactor, s32 op);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32 attr, s32 type);
    extern void GXSetVtxAttrFmt(s32 vtxfmt, s32 attr, s32 compCnt, s32 compType, s32 frac);
    extern void GXSetNumChans(s32 nChans);
    extern void GXSetChanCtrl(s32 chan, s32 enable, s32 ambSrc, s32 matSrc, s32 lightMask, s32 diffFn, s32 attnFn);
    extern void GXSetNumTexGens(s32 nTexGens);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXSetNumTevStages(s32 nStages);
    extern void GXSetTevColorOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevColorIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevAlphaOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevAlphaIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevOrder(s32 stage, s32 texCoord, s32 texMap, s32 color);
    extern void GXSetZMode(s32 enable, s32 func, s32 updateEnable);
    extern f32 float_neg640_80422b28;
    extern f32 float_640_80422b2c;
    extern f32 float_105_80422b30;
    void* cam;
    void* battleHi;
    void* dark;

    battleHi = (void*)((s32)_battleWorkPointer + 0x10000);
    cam = camGetPtr(4);
    GXSetCullMode(2);
    GXSetZCompLoc(1);
    GXSetAlphaCompare(7, 0, 0, 7, 0);
    GXSetBlendMode(1, 4, 5, 5);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(0xB, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 0xB, 1, 5, 0);
    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 1, 1, 0, 2, 2);
    GXSetNumTexGens(0);
    GXLoadPosMtxImm((void*)((s32)cam + 0x11C), 0);
    GXSetNumTevStages(1);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0xF, 0xF, 0xF, 0xA);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaIn(0, 7, 7, 7, 5);
    GXSetTevOrder(0, 0xFF, 0xFF, 4);

    dark = (void*)((s32)battleHi + 0x6F00);
    GXSetZMode(1, 6, 0);
    DarkBySquare(*(u8*)((s32)dark + 4),
                 float_neg640_80422b28,
                 float_neg640_80422b28,
                 float_640_80422b2c,
                 float_640_80422b2c,
                 float_105_80422b30);
    GXSetZMode(1, 1, 0);
    DarkBySquare(*(u8*)((s32)dark + 4),
                 float_640_80422b2c,
                 float_640_80422b2c,
                 float_neg640_80422b28,
                 float_neg640_80422b28,
                 float_105_80422b30);
    GXSetZMode(1, 1, 0);
    DarkBySquare(*(u8*)((s32)battleHi + 0x6EE4),
                 float_neg640_80422b28,
                 float_neg640_80422b28,
                 float_640_80422b2c,
                 float_640_80422b2c,
                 float_105_80422b30);
}

void* batSpotEntry(void) {
    void* work = wp;
    void* spot = *(void**)((s32)work + 4);
    s32 count = *(s32*)work;
    u32* data = vec3_802f37a8;

    while (count > 0) {
        if ((*(u16*)spot & 1) == 0) {
            break;
        }
        spot = (void*)((s32)spot + 0x2C);
        count--;
    }

    *(u32*)((s32)spot + 0x4) = data[0x54 / 4];
    *(u32*)((s32)spot + 0x8) = data[0x58 / 4];
    *(u32*)((s32)spot + 0xC) = data[0x5C / 4];
    *(u32*)((s32)spot + 0x1C) = data[0x60 / 4];
    *(u32*)((s32)spot + 0x20) = data[0x64 / 4];
    *(u32*)((s32)spot + 0x24) = data[0x68 / 4];
    *(u32*)((s32)spot + 0x10) = data[0x6C / 4];
    *(u32*)((s32)spot + 0x14) = data[0x70 / 4];
    *(u32*)((s32)spot + 0x18) = data[0x74 / 4];
    *(u32*)((s32)spot + 0x28) = unk_804295d4;
    *(u16*)spot |= 1;
    return spot;
}


void batSpotInit(void) {
    extern void* BattleAlloc(s32 size);
    extern void* memset(void* dst, s32 value, u32 size);
    extern f32 float_neg100_80422b18;
    extern f32 float_50_80422b1c;
    u32* data = vec3_802f37a8;
    s32 i;

    *(s32*)wp = 5;
    *(void**)((s32)wp + 4) = BattleAlloc(*(s32*)wp * 0x2C);
    memset(*(void**)((s32)wp + 4), 0, *(s32*)wp * 0x2C);

    for (i = 0; i < 5; i++) {
        void* spot = batSpotEntry();
        BattleStageVec pos;

        pos.x = *(f32*)&data[0x30 / 4];
        pos.y = *(f32*)&data[0x34 / 4];
        pos.z = *(f32*)&data[0x38 / 4];
        pos.x = float_neg100_80422b18 + (float_50_80422b1c * i);
        *(BattleStageVec*)((s32)spot + 0x4) = pos;
        *(u32*)((s32)spot + 0x10) = data[0x3C / 4];
        *(u32*)((s32)spot + 0x14) = data[0x40 / 4];
        *(u32*)((s32)spot + 0x18) = data[0x44 / 4];
        *(u32*)((s32)spot + 0x1C) = data[0x48 / 4];
        *(u32*)((s32)spot + 0x20) = data[0x4C / 4];
        *(u32*)((s32)spot + 0x24) = data[0x50 / 4];
        *(u32*)((s32)spot + 0x28) = dat_80422ad4;
    }
}


s32 tou_gamen_screen_tev_init(void* event) {
    extern s32 GXGetTexBufferSize(s32 width, s32 height, s32 format, s32 mipmap, s32 maxLod);
    extern void* _mapAlloc(void* heap, u32 size);
    extern s32 evtGetValue(void* event, s32 value);
    extern void* evtEntry(void* script, s32 priority, s32 flags);
    extern void* mapalloc_base_ptr;
    extern void* screen_wp;
    extern s32 screen_evt;
    extern f32 float_0_80422ad8;
    extern f32 float_1_80422adc;
    extern f32 float_neg1_80422b00;
    u32 texSize = GXGetTexBufferSize(0x260, 0x1E0, 0, 0, 0);

    screen_wp = _mapAlloc(mapalloc_base_ptr, 0x2C);
    *(void**)screen_wp = _mapAlloc(mapalloc_base_ptr, texSize);
    *(s32*)((s32)screen_wp + 0x4) = 0;
    *(f32*)((s32)screen_wp + 0xC) = float_1_80422adc;
    *(f32*)((s32)screen_wp + 0x8) = float_1_80422adc;
    *(f32*)((s32)screen_wp + 0x14) = float_0_80422ad8;
    *(f32*)((s32)screen_wp + 0x10) = float_0_80422ad8;
    *(f32*)((s32)screen_wp + 0x1C) = float_0_80422ad8;
    *(f32*)((s32)screen_wp + 0x18) = float_0_80422ad8;
    *(s32*)((s32)screen_wp + 0x20) = 0;
    *(f32*)((s32)screen_wp + 0x28) = float_neg1_80422b00;
    if (evtGetValue(event, 0xF5DE0180) == 0xA3) {
        *(s32*)((s32)screen_wp + 0x24) = 1;
    }
    evtEntry(&screen_evt, 0, 0);
    return 2;
}


void BattleStageInit(void) {
    extern void* memset(void* dst, s32 value, u32 size);
    extern void BattleStageObjectInit(void);
    extern f32 float_290_80422b3c;
    extern f32 float_200_80422b40;
    extern f32 float_90_80422b44;
    extern f32 float_neg50_80422b48;
    void* stage = (void*)((s32)_battleWorkPointer + 0x163FC);
    s32 i;
    s32 offset = 0;

    memset(stage, 0, 0xB3C);
    *(u32*)stage = 0;
    for (i = 0; i < 12; i++) {
        *(u32*)((s32)stage + offset + 0x4) = 0;
        *(u32*)((s32)stage + offset + 0xE8) = 0;
        offset += 0xE8;
    }
    *(s32*)((s32)stage + 0xAE4) = 1;
    *(u8*)((s32)stage + 0xAE9) = 0;
    *(u8*)((s32)stage + 0xAE8) = 0;
    *(u8*)((s32)stage + 0xAEA) = 0;
    *(u8*)((s32)stage + 0xAEC) = 0;
    *(s32*)((s32)stage + 0xAF0) = 0;
    *(s32*)((s32)stage + 0xAF8) = 0;
    *(s32*)((s32)stage + 0xB04) = 1;
    *(u8*)((s32)stage + 0xB09) = 0x8C;
    *(u8*)((s32)stage + 0xB08) = 0x8C;
    *(u8*)((s32)stage + 0xB0A) = 0;
    *(u8*)((s32)stage + 0xB0C) = 0;
    *(s32*)((s32)stage + 0xB10) = 0;
    *(s32*)((s32)stage + 0xB18) = 0;
    *(f32*)((s32)stage + 0xB24) = float_290_80422b3c;
    *(f32*)((s32)stage + 0xB28) = float_200_80422b40;
    *(f32*)((s32)stage + 0xB2C) = float_90_80422b44;
    *(f32*)((s32)stage + 0xB30) = *(f32*)((s32)stage + 0xB24);
    *(f32*)((s32)stage + 0xB34) = float_neg50_80422b48;
    *(f32*)((s32)stage + 0xB38) = float_200_80422b40;
    BattleStageObjectInit();
    batSpotInit();
}


void BattleStageLightSetTarget(s32 lightId, f32 x, f32 y, f32 z, s32 unk1, s32 unk2);

void BattleStageLightSetTargetByUnit(s32 lightId, f32 x, f32 y, f32 z, s32 unitId, s32 fadeFrames, s32 unk) {
    extern void BtlUnit_GetPos(void* unit, f32* x, f32* y, f32* z);
    f32 unitX;
    f32 unitY;
    f32 unitZ;
    void* unit = BattleGetUnitPtr(_battleWorkPointer, unitId);

    BtlUnit_GetPos(unit, &unitX, &unitY, &unitZ);
    unitX += x;
    unitY += y;
    unitZ += z;
    BattleStageLightSetTarget(lightId, unitX, unitY, unitZ, fadeFrames, unk);
}


s32 BattleStageSysLightEntry(char* name) {
    extern void strcpy(char* dst, const char* src);
    s32 i;
    void* workHi = (void*)((s32)_battleWorkPointer + 0x10000);
    void* stage = (void*)((s32)workHi + 0x63FC);
    s32 offset;

    offset = 0;
    for (i = 0; i <= 6; i++) {
        void* light = (void*)((s32)stage + offset + 4);

        if ((*(u32*)light & 1) != 0) {
            strcmp((char*)((s32)light + 4), name);
        }
        offset += 0xE8;
    }

    offset = 0;
    for (i = 0; i < 7; i++) {
        void* light = (void*)((s32)stage + offset + 4);

        if ((*(u32*)light & 1) == 0) {
            *(u32*)light = 1;
            strcpy((char*)((s32)light + 4), name);
            break;
        }
        offset += 0xE8;
    }
    return i;
}


f32 getSpline(f32* a, f32* b, f32* c, f32* d, s32* time, s32 count, s32 value) {
    s32 i;
    f32 t;

    for (i = count - 1; i >= 0; i--) {
        t = value - time[i];
        if (t >= 0.0f) {
            break;
        }
    }
    t = value - time[i];
    return a[i] + (t * (b[i] + (t * (c[i] + (t * d[i])))));
}


void BattleStageLightSetTarget(s32 lightId, f32 x, f32 y, f32 z, s32 unk1, s32 unk2) {
    s32 offset = lightId * 0xE8;
    void* workHi = (void*)((s32)_battleWorkPointer + 0x10000);
    void* stage = (void*)((s32)workHi + 0x63FC);
    void* light = (void*)((s32)stage + offset + 4);
    BattleStageVec vec;

    *(f32*)((s32)light + 0x90) = *(f32*)((s32)light + 0x84);
    *(f32*)((s32)light + 0x94) = *(f32*)((s32)light + 0x88);
    *(f32*)((s32)light + 0x98) = *(f32*)((s32)light + 0x8C);
    *(f32*)((s32)light + 0x9C) = x;
    *(f32*)((s32)light + 0xA0) = y;
    *(f32*)((s32)light + 0xA4) = z;
    *(s32*)((s32)light + 0xA8) = unk1;
    *(s32*)((s32)light + 0xAC) = unk1;
    *(u8*)((s32)light + 0xB0) = unk2;
    *(u32*)light &= ~4;

    if (unk1 == 0) {
        vec = vec3_802f37c0[0];
        vec.x = x;
        vec.y = y;
        vec.z = z;
        *(BattleStageVec*)((s32)light + 0x84) = vec;
    }
}


void DarkBySquare(s32 alpha, f32 x0, f32 y0, f32 x1, f32 y1, f32 z) {
    extern void GXBegin(s32 primitive, s32 vtxfmt, s32 nverts);

    GXBegin(0x80, 0, 4);
    DarkByPoint(alpha, x0, y0, z);
    DarkByPoint(alpha, x1, y0, z);
    DarkByPoint(alpha, x1, y1, z);
    DarkByPoint(alpha, x0, y1, z);
}


void BattleStageLightSetLightColor(s32 lightId, u8 r, u8 g, u8 b, s32 fadeFrames, s32 unk) {
    s32 offset = lightId * 0xE8;
    void* workHi = (void*)((s32)_battleWorkPointer + 0x10000);
    void* stage = (void*)((s32)workHi + 0x63FC);
    void* light = (void*)((s32)stage + offset + 4);
    u32 color;

    color = unk_804295c8;
    ((u8*)&color)[0] = *(u8*)((s32)light + 0x24);
    ((u8*)&color)[1] = *(u8*)((s32)light + 0x25);
    ((u8*)&color)[2] = *(u8*)((s32)light + 0x26);
    *(u32*)((s32)light + 0x28) = color;

    color = unk_804295cc;
    ((u8*)&color)[0] = r;
    ((u8*)&color)[1] = g;
    ((u8*)&color)[2] = b;
    *(u32*)((s32)light + 0x2C) = color;
    *(s32*)((s32)light + 0x30) = fadeFrames;
    *(s32*)((s32)light + 0x34) = fadeFrames;
    *(u8*)((s32)light + 0x38) = unk;

    if (fadeFrames == 0) {
        color = dat_80422ad0;
        ((u8*)&color)[0] = *(u8*)((s32)light + 0x24);
        ((u8*)&color)[1] = *(u8*)((s32)light + 0x25);
        ((u8*)&color)[2] = *(u8*)((s32)light + 0x26);
        *(u32*)((s32)light + 0x24) = color;
    }
}


void BattleStageLightSetUpVec(s32 lightId, f32 x, f32 y, f32 z, s32 unk1, s32 unk2) {
    s32 offset = lightId * 0xE8;
    void* workHi = (void*)((s32)_battleWorkPointer + 0x10000);
    void* stage = (void*)((s32)workHi + 0x63FC);
    void* light = (void*)((s32)stage + offset + 4);
    BattleStageVec vec;

    *(f32*)((s32)light + 0xC0) = *(f32*)((s32)light + 0xB4);
    *(f32*)((s32)light + 0xC4) = *(f32*)((s32)light + 0xB8);
    *(f32*)((s32)light + 0xC8) = *(f32*)((s32)light + 0xBC);
    *(f32*)((s32)light + 0xCC) = x;
    *(f32*)((s32)light + 0xD0) = y;
    *(f32*)((s32)light + 0xD4) = z;
    *(s32*)((s32)light + 0xD8) = unk1;
    *(s32*)((s32)light + 0xDC) = unk1;
    *(u8*)((s32)light + 0xE0) = unk2;

    if (unk1 == 0) {
        vec = vec3_802f37cc[0];
        vec.x = x;
        vec.y = y;
        vec.z = z;
        *(BattleStageVec*)((s32)light + 0xB4) = vec;
    }
}


void BattleStageLightSetPosition(s32 lightId, f32 x, f32 y, f32 z, s32 unk1, s32 unk2) {
    s32 offset = lightId * 0xE8;
    void* workHi = (void*)((s32)_battleWorkPointer + 0x10000);
    void* stage = (void*)((s32)workHi + 0x63FC);
    void* light = (void*)((s32)stage + offset + 4);
    BattleStageVec vec;

    *(f32*)((s32)light + 0x60) = *(f32*)((s32)light + 0x54);
    *(f32*)((s32)light + 0x64) = *(f32*)((s32)light + 0x58);
    *(f32*)((s32)light + 0x68) = *(f32*)((s32)light + 0x5C);
    *(f32*)((s32)light + 0x6C) = x;
    *(f32*)((s32)light + 0x70) = y;
    *(f32*)((s32)light + 0x74) = z;
    *(s32*)((s32)light + 0x78) = unk1;
    *(s32*)((s32)light + 0x7C) = unk1;
    *(u8*)((s32)light + 0x80) = unk2;

    if (unk1 == 0) {
        vec = vec3_802f37b4[0];
        vec.x = x;
        vec.y = y;
        vec.z = z;
        *(BattleStageVec*)((s32)light + 0x54) = vec;
    }
}


void BattleStageLightRelease(s32 lightId, s32 fadeFrames, s32 unk) {
    s32 offset = lightId * 0xE8;
    void* workHi = (void*)((s32)_battleWorkPointer + 0x10000);
    void* stage = (void*)((s32)workHi + 0x63FC);
    void* light = (void*)((s32)stage + offset + 4);
    u32 color;

    if ((*(u32*)light & 1) != 0) {
        if (fadeFrames == 0) {
            *(u32*)light = 0;
        } else {
            *(u32*)light |= 2;
            color = unk_804295c0;
            ((u8*)&color)[0] = *(u8*)((s32)light + 0x24);
            ((u8*)&color)[1] = *(u8*)((s32)light + 0x25);
            ((u8*)&color)[2] = *(u8*)((s32)light + 0x26);
            *(u32*)((s32)light + 0x28) = color;
            *(u32*)((s32)light + 0x2C) = unk_804295c4;
            *(s32*)((s32)light + 0x30) = fadeFrames;
            *(s32*)((s32)light + 0x34) = fadeFrames;
            *(u8*)((s32)light + 0x38) = unk;
        }
    }
}


double randf(double min, double max) {
    extern s32 rand(void);
    extern f32 float_32767_80422b04;

    return (f32)(((f32)rand() / float_32767_80422b04) * (f32)(max - min) + (f32)min);
}

void BattleStageEnd(void) {
    ;
}


void screen_capture(void) {
    extern void GXSetTexCopySrc(u32 left, u32 top, u32 width, u32 height);
    extern void GXSetTexCopyDst(u16 width, u16 height, u32 format, u32 mipmap);
    extern void GXCopyTex(void* dest, s32 clear);
    extern void GXPixModeSync(void);
    extern void* screen_wp;

    GXSetTexCopySrc(0, 0, 0x260, 0x1E0);
    GXSetTexCopyDst(0x130, 0xF0, 4, 1);
    GXCopyTex(*(void**)screen_wp, 0);
    GXPixModeSync();
    *(s32*)((s32)screen_wp + 4) = 1;
}

void batSpotMain(void) {
    extern f32 float_neg1000_80422b14;
    extern u8 batSpotDisp(s32 cameraId);
    s32 count;

    count = *(s32*)wp;
    while (count > 0) {
        count--;
    }

    if (*(s32*)((s32)wp + 8) > 0) {
        dispEntry(6, 0, batSpotDisp, float_neg1000_80422b14, 0);
    }
}

u8 batSpotDisp(s32 cameraId) {
    extern void* wp;
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32 attr, s32 type);
    extern void GXSetVtxAttrFmt(s32 vtxfmt, s32 attr, s32 compCnt, s32 compType, s32 frac);
    extern void GXSetNumTexGens(s32 nTexGens);
    extern void GXSetNumTevStages(s32 nStages);
    extern void GXSetTevOrder(s32 stage, s32 texCoord, s32 texMap, s32 color);
    extern void GXSetTevColorOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevAlphaOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevColorIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevAlphaIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetZCompLoc(s32 beforeTex);
    extern void GXSetAlphaCompare(s32 comp0, s32 ref0, s32 op, s32 comp1, s32 ref1);
    extern void GXSetNumChans(s32 nChans);
    extern void GXSetCullMode(s32 mode);
    extern void GXSetAlphaUpdate(s32 update);
    extern void GXSetColorUpdate(s32 update);
    extern void GXSetZMode(s32 enable, s32 func, s32 updateEnable);
    extern void GXSetBlendMode(s32 type, s32 srcFactor, s32 dstFactor, s32 op);
    extern void GXSetChanCtrl(s32 chan, s32 enable, s32 ambSrc, s32 matSrc, s32 lightMask, s32 diffFn, s32 attnFn);
    extern void GXSetChanMatColor(s32 chan, void* color);
    extern u8 drawCone(s32 cameraId, void* spot);
    extern u32 unk_804295d0;
    void* spot;
    s32 i;
    u32 color;

    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(0xB, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 0xB, 1, 5, 0);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0xFF, 0xFF, 4);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0xF, 0xF, 0xF, 0xA);
    GXSetTevAlphaIn(0, 7, 7, 7, 5);
    GXSetZCompLoc(1);
    GXSetAlphaCompare(7, 0, 0, 7, 0);
    GXSetNumChans(1);

    spot = *(void**)((s32)wp + 4);
    for (i = 0; i < *(s32*)wp; i++, spot = (void*)((s32)spot + 0x2C)) {
        if (((*(u16*)spot & 1) != 0) && ((*(u16*)spot & 2) != 0)) {
            GXSetCullMode(2);
            GXSetAlphaUpdate(1);
            GXSetColorUpdate(0);
            GXSetZMode(1, 3, 0);
            GXSetBlendMode(0, 4, 5, 0);
            GXSetChanCtrl(4, 0, 0, 1, 0, 2, 2);
            drawCone(cameraId, spot);

            GXSetCullMode(1);
            GXSetAlphaUpdate(1);
            GXSetColorUpdate(0);
            GXSetZMode(1, 3, 0);
            GXSetBlendMode(0, 4, 5, 0);
            GXSetChanCtrl(4, 0, 0, 0, 0, 2, 2);
            color = unk_804295d0;
            GXSetChanMatColor(4, &color);
            drawCone(cameraId, spot);

            GXSetCullMode(2);
            GXSetAlphaUpdate(0);
            GXSetColorUpdate(1);
            GXSetZMode(1, 3, 0);
            GXSetBlendMode(1, 6, 7, 0);
            GXSetChanCtrl(4, 0, 0, 0, 0, 2, 2);
            color = *(u32*)((s32)spot + 0x28);
            GXSetChanMatColor(4, &color);
            drawCone(cameraId, spot);
        }
    }
    return 0;
}

u8 drawCone(s32 cameraId, void* spot) {
    typedef f32 MtxLocal[3][4];
    extern void* camGetPtr(s32 cameraId);
    extern void PSMTXTrans(void* m, f32 x, f32 y, f32 z);
    extern void PSMTXScale(void* m, f32 x, f32 y, f32 z);
    extern void PSMTXRotRad(void* m, s32 axis, f32 rad);
    extern void PSMTXConcat(void* a, void* b, void* out);
    extern void GXLoadPosMtxImm(void* m, s32 id);
    extern void GXSetCurrentMtx(s32 id);
    extern void GXBegin(s32 primitive, s32 vtxfmt, s32 nverts);
    extern double sin(double x);
    extern double cos(double x);
    extern f32 float_deg2rad_80422b08;
    extern f32 float_0_80422ad8;
    extern f32 float_neg1_80422b00;
    extern f32 float_6p2832_80422b0c;
    extern f32 float_30_80422b10;
    MtxLocal trans;
    MtxLocal scale;
    MtxLocal rx;
    MtxLocal ry;
    MtxLocal rz;
    MtxLocal rot;
    MtxLocal mtx;
    void* cam;
    volatile f32* fifoF;
    volatile u8* fifoB;
    s32 i;
    f32 rad;
    f32 s;
    f32 c;
    u8 alpha;

    cam = camGetPtr(cameraId);
    PSMTXTrans(trans, *(f32*)((s32)spot + 0x4), *(f32*)((s32)spot + 0x8), *(f32*)((s32)spot + 0xC));
    PSMTXScale(scale, *(f32*)((s32)spot + 0x1C), *(f32*)((s32)spot + 0x20), *(f32*)((s32)spot + 0x24));
    PSMTXRotRad(rx, 'x', float_deg2rad_80422b08 * *(f32*)((s32)spot + 0x10));
    PSMTXRotRad(ry, 'y', float_deg2rad_80422b08 * *(f32*)((s32)spot + 0x14));
    PSMTXRotRad(rz, 'z', float_deg2rad_80422b08 * *(f32*)((s32)spot + 0x18));
    PSMTXConcat(rz, ry, ry);
    PSMTXConcat(ry, rx, rot);
    PSMTXConcat((void*)((s32)cam + 0x11C), trans, trans);
    PSMTXConcat(trans, rot, rot);
    PSMTXConcat(rot, scale, rot);
    GXLoadPosMtxImm(rot, 0);
    GXSetCurrentMtx(0);
    GXBegin(0xA0, 0, 0x20);

    fifoF = (volatile f32*)0xCC008000;
    fifoB = (volatile u8*)0xCC008000;
    alpha = *(u8*)((s32)spot + 0x2B);
    *fifoF = float_0_80422ad8;
    *fifoF = float_0_80422ad8;
    *fifoF = float_0_80422ad8;
    *fifoB = 0;
    *fifoB = 0;
    *fifoB = 0;
    *fifoB = alpha;

    for (i = 0; i < 31; i++) {
        if (i == 30) {
            rad = float_0_80422ad8;
        } else {
            rad = (float_6p2832_80422b0c * (f32)-i) / float_30_80422b10;
        }
        s = (f32)sin(rad);
        c = (f32)cos(rad);
        *fifoF = c;
        *fifoF = s;
        *fifoF = float_neg1_80422b00;
        *fifoB = 0;
        *fifoB = 0;
        *fifoB = 0;
        *fifoB = alpha / 3;
    }
    return 0;
}

