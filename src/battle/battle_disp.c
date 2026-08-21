#include "battle/battle_disp.h"
#include "battle/battle_camera.h"

extern void* _battleWorkPointer;
extern f32 float_901_80422238;
void _btlStockExpDisp(void);
void dispEntry(s32 cameraId, s32 priority, void* callback, f32 z, void* param);
extern f32 float_0_80422240;
extern f32 float_0p5_80422234;
extern f32 float_1_8042224c;
extern f32 float_deg2rad_8042223c;
extern u8 itemDataTable[];
extern f32 float_2_80422250;
extern f32 float_4_80422224;
extern f64 double_to_int_802ee3f8;
extern s32 _status_pose_table[8];
extern void* camGetCurPtr(void);
extern void PSMTXConcat(void* a, void* b, void* out);
extern void GXLoadPosMtxImm(void* mtx, s32 id);
extern char* strcpy(char* dst, const char* src);
void animPoseSetLocalTimeRate(void* pose, f32 rate);
void animPoseSetEffectAnim(void* pose, s32 value, s32 flag);
void animPoseSetEffect(void* pose, s32 value, s32 flag);
void animPoseSetMaterialLightFlagOn(void* pose, s32 flag);
typedef struct BattleWorkUnitPartBlur {
    u32 flags;
    f32 orientation[3][4];
    f32 rotation;
    u32 baseColor;
    u32 blurColors[2];
} BattleWorkUnitPartBlur;

extern void PSMTXCopy(void* src, void* dst);
void _partsBlurControl(f64 rotation, void* part, u32 flags, void* orientation, void* color);
void _pose_def(void* part);
void _pose_one_pattern(void* part);
void _pose_two_pattern(void* part);
void battleMenuDispInit(void);
f32 sinfd(f64);
void gravityOffsetControl(void* part);
void floatOffsetControl(void* part);
void statusPoseControl(void* part);
f32 getFloatDispOffset(void* part);
f32 getGravityDispOffset(void* part);
void BattleStockExpDisp(void);
void _btlDispTex4(s32 texId, f32* trans0, f32* scale0, f32* rot, f32* trans1, f32* scale1, void* color);
void TEXGetGXTexObjFromPalette(void* palette, void* texObj, s32 id);
u16 GXGetTexObjWidth(void* texObj);
u16 GXGetTexObjHeight(void* texObj);

void* searchPoseTbl(void* tbl, s32 type) {
    s32 i;
    if (tbl == 0) {
        return 0;
    }
    for (i = 0; i < 0x1C; i++) {
        s32 value = *(s32*)tbl;
        if (value == type || value == 0x1C) {
            return *(void**)((s32)tbl + 4);
        }
        tbl = (void*)((s32)tbl + 8);
    }
    return 0;
}

void btlDispGXInit2DSub(void) {
    extern void GXSetCullMode(s32 mode);
    extern void GXSetZCompLoc(s32 beforeTex);
    extern void GXSetAlphaCompare(s32 comp0, s32 ref0, s32 op, s32 comp1, s32 ref1);
    extern void GXSetBlendMode(s32 type, s32 srcFactor, s32 dstFactor, s32 op);
    extern void GXSetZMode(s32 enable, s32 func, s32 updateEnable);
    extern void GXClearVtxDesc(void);
    extern void GXSetNumChans(s32 nChans);
    extern void GXSetChanCtrl(s32 chan, s32 enable, s32 ambSrc, s32 matSrc, s32 lightMask, s32 diffFn, s32 attnFn);

    GXSetCullMode(0);
    GXSetZCompLoc(1);
    GXSetAlphaCompare(7, 0, 0, 7, 0);
    GXSetBlendMode(1, 4, 5, 5);
    GXSetZMode(1, 7, 0);
    GXClearVtxDesc();
    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 1, 1, 0, 2, 2);
}

void btlDispGXInit2DRasta(void) {
    extern void GXSetVtxDesc(s32 attr, s32 type);
    extern void GXSetVtxAttrFmt(s32 vtxFmt, s32 attr, s32 compCnt, s32 compType, s32 frac);
    extern void GXSetNumTexGens(s32 nTexGens);
    extern void GXSetNumTevStages(s32 nStages);
    extern void GXSetTevColorOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevColorIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevAlphaOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevAlphaIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevOrder(s32 stage, s32 texCoord, s32 texMap, s32 chan);

    btlDispGXInit2DSub();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(0xB, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 0xB, 1, 5, 0);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0xF, 0xF, 0xF, 0xA);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaIn(0, 7, 7, 7, 5);
    GXSetTevOrder(0, 0xFF, 0xFF, 4);
}

void btlDispGXInit2D(void) {
    extern void GXSetVtxDesc(s32 attr, s32 type);
    extern void GXSetVtxAttrFmt(s32 vtxFmt, s32 attr, s32 compCnt, s32 compType, s32 frac);
    extern void GXSetNumTexGens(s32 nTexGens);
    extern void GXSetTexCoordGen2(s32 dstCoord, s32 func, s32 srcParam, s32 mtx, s32 normalize, s32 postMtx);
    extern void GXSetNumTevStages(s32 nStages);
    extern void GXSetTevColorOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevColorIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevAlphaOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevAlphaIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevOrder(s32 stage, s32 texCoord, s32 texMap, s32 chan);

    btlDispGXInit2DSub();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(0xB, 1);
    GXSetVtxDesc(0xD, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 0xB, 1, 5, 0);
    GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXSetNumTevStages(1);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0xF, 0xA, 8, 0xF);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaIn(0, 7, 5, 4, 7);
    GXSetTevOrder(0, 0, 0, 4);
}

void btlDispGXPoint2DRasta(u8 r, u8 g, u8 b, u8 a, f32 x, f32 y) {
    volatile f32* fifo_f = (volatile f32*)0xCC008000;
    volatile u8* fifo_b = (volatile u8*)0xCC008000;
    fifo_f[0] = x;
    fifo_f[0] = y;
    fifo_f[0] = float_0_80422240;
    fifo_b[0] = r;
    fifo_b[0] = g;
    fifo_b[0] = b;
    fifo_b[0] = a;
}

void btlDispGXQuads2DRasta(f32 x1, f32 y1, f32 x2, f32 y2, u8 r, u8 g, u8 b, u8 a) {
    extern void GXBegin(s32 primitive, s32 vtxFmt, s32 nVerts);

    GXBegin(0x80, 0, 4);
    btlDispGXPoint2DRasta(r, g, b, a, x1, y1);
    btlDispGXPoint2DRasta(r, g, b, a, x2, y1);
    btlDispGXPoint2DRasta(r, g, b, a, x2, y2);
    btlDispGXPoint2DRasta(r, g, b, a, x1, y2);
}

void btlDispGXPoint2D(u8 r, u8 g, u8 b, u8 a, f32 x, f32 y, f32 s, f32 t) {
    volatile f32* fifo_f = (volatile f32*)0xCC008000;
    volatile u8* fifo_b = (volatile u8*)0xCC008000;
    fifo_f[0] = x;
    fifo_f[0] = y;
    fifo_f[0] = float_0_80422240;
    fifo_b[0] = r;
    fifo_b[0] = g;
    fifo_b[0] = b;
    fifo_b[0] = a;
    fifo_f[0] = s;
    fifo_f[0] = t;
}

void btlDispGXQuads2D(f32 x1, f32 y1, f32 x2, f32 y2, u8 r, u8 g, u8 b, u8 a) {
    extern void GXBegin(s32 primitive, s32 vtxFmt, s32 nVerts);

    GXBegin(0x80, 0, 4);
    btlDispGXPoint2D(r, g, b, a, x1, y1, float_0_80422240, float_0_80422240);
    btlDispGXPoint2D(r, g, b, a, x2, y1, float_1_8042224c, float_0_80422240);
    btlDispGXPoint2D(r, g, b, a, x2, y2, float_1_8042224c, float_1_8042224c);
    btlDispGXPoint2D(r, g, b, a, x1, y2, float_0_80422240, float_1_8042224c);
}

void btlDispTexPlaneInit(void) {
    extern void GXSetCullMode(s32 mode);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32 attr, s32 type);
    extern void GXSetVtxAttrFmt(s32 vtxFmt, s32 attr, s32 compCnt, s32 compType, s32 frac);
    extern void GXSetNumChans(s32 nChans);
    extern void GXSetChanCtrl(s32 chan, s32 enable, s32 ambSrc, s32 matSrc, s32 lightMask, s32 diffFn, s32 attnFn);
    extern void GXSetNumTexGens(s32 nTexGens);
    extern void GXSetTexCoordGen2(s32 dstCoord, s32 func, s32 srcParam, s32 mtx, s32 normalize, s32 postMtx);

    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(0xB, 1);
    GXSetVtxDesc(0xD, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 0xB, 1, 5, 0);
    GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);
    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 1, 0, 2, 2);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
}

void btlDispTexPlainGX(s32 texId, u8* color0, u8* color1, u8* color2, u8* color3) {
    extern void GXLoadTexObj(void*, s32);
    extern s32 GXGetTexObjFmt(void*);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevOp(s32, s32);
    extern void GXBegin(s32, s32, s32);
    u8 texObj[32];
    volatile f32* fifoF = (volatile f32*)0xCC008000;
    volatile u8* fifoB = (volatile u8*)0xCC008000;
    void* work = _battleWorkPointer;
    f32 halfWidth;
    f32 halfHeight;

    TEXGetGXTexObjFromPalette(**(void***)((s32)work + 0x163F8 + 0xA0), texObj, texId);
    GXLoadTexObj(texObj, 0);
    if (GXGetTexObjFmt(texObj) == 0) {
        GXSetNumTevStages(1);
        GXSetTevColorOp(0, 0, 0, 0, 1, 0);
        GXSetTevColorIn(0, 0xF, 0xF, 0xF, 0xA);
        GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
        GXSetTevAlphaIn(0, 7, 5, 4, 7);
        GXSetTevOrder(0, 0, 0, 4);
    } else {
        GXSetNumTevStages(1);
        GXSetTevColorOp(0, 0, 0, 0, 1, 0);
        GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
        GXSetTevOrder(0, 0, 0, 4);
        GXSetTevOp(0, 0);
    }

    halfWidth = (f32)GXGetTexObjWidth(texObj) * float_0p5_80422234;
    halfHeight = (f32)GXGetTexObjHeight(texObj) * float_0p5_80422234;
    GXBegin(0x80, 0, 4);

#define EMIT_VERTEX(px, py, color, ts, tt) \
    fifoF[0] = (px); fifoF[0] = (py); fifoF[0] = float_0_80422240; \
    fifoB[0] = (color)[0]; fifoB[0] = (color)[1]; fifoB[0] = (color)[2]; fifoB[0] = (color)[3]; \
    fifoF[0] = (ts); fifoF[0] = (tt)
    EMIT_VERTEX(-halfWidth, halfHeight, color0, float_0_80422240, float_0_80422240);
    EMIT_VERTEX(halfWidth, halfHeight, color1, float_1_8042224c, float_0_80422240);
    EMIT_VERTEX(halfWidth, -halfHeight, color2, float_1_8042224c, float_1_8042224c);
    EMIT_VERTEX(-halfWidth, -halfHeight, color3, float_0_80422240, float_1_8042224c);
#undef EMIT_VERTEX
}

void btlDispTexPlane(s32 tpl, void* color, s32 flags, f32 x, f32 y, f32 z, f32 scaleX, f32 scaleY) {
    extern void PSMTXScale(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXTrans(void* mtx, f32 x, f32 y, f32 z);
    extern void btlDispTexPlainGX(s32 texId, void* color0, void* color1, void* color2, void* color3);
    f32 scaleMtx[3][4];
    f32 transMtx[3][4];
    f32 outMtx[3][4];
    u32 colorCopy0;
    u32 colorCopy1;
    u32 colorCopy2;
    u32 colorCopy3;
    void* cam;

    btlDispTexPlaneInit();
    cam = camGetCurPtr();
    PSMTXScale(scaleMtx, scaleX, scaleY, float_1_8042224c);
    PSMTXTrans(transMtx, x, y, z);
    PSMTXConcat(transMtx, scaleMtx, outMtx);
    PSMTXConcat((void*)((s32)cam + 0x11C), outMtx, outMtx);
    GXLoadPosMtxImm(outMtx, 0);

    colorCopy3 = *(u32*)color;
    colorCopy2 = colorCopy3;
    colorCopy1 = colorCopy3;
    colorCopy0 = colorCopy3;

    btlDispTexPlainGX(tpl, &colorCopy0, &colorCopy1, &colorCopy2, &colorCopy3);
}

void btlDispTexPlane2(void* mtx, s32 texId, void* color) {
    extern void PSMTXCopy(void* src, void* dst);
    extern void btlDispTexPlainGX(s32 texId, void* color0, void* color1, void* color2, void* color3);
    u32 colorCopy0;
    u32 colorCopy1;
    u32 colorCopy2;
    u32 colorCopy3;
    f32 localMtx[3][4];

    btlDispTexPlaneInit();
    PSMTXCopy(mtx, localMtx);
    PSMTXConcat((void*)((s32)camGetCurPtr() + 0x11C), localMtx, localMtx);
    GXLoadPosMtxImm(localMtx, 0);

    colorCopy3 = *(u32*)color;
    colorCopy2 = colorCopy3;
    colorCopy1 = colorCopy3;
    colorCopy0 = colorCopy3;

    btlDispTexPlainGX(texId, &colorCopy0, &colorCopy1, &colorCopy2, &colorCopy3);
}

void btlDispTexPlane3(void* mtx, s32 texId, void* color0, void* color1, void* color2, void* color3) {
    extern void PSMTXCopy(void* src, void* dst);
    extern void btlDispTexPlainGX(s32 texId, void* color0, void* color1, void* color2, void* color3);
    f32 localMtx[3][4];
    u32 local0;
    u32 local1;
    u32 local2;
    u32 local3;
    u32 value3;
    u32 value2;
    u32 value1;
    u32 value0;

    btlDispTexPlaneInit();
    PSMTXCopy(mtx, localMtx);
    PSMTXConcat((void*)((s32)camGetCurPtr() + 0x11C), localMtx, localMtx);
    GXLoadPosMtxImm(localMtx, 0);

    value3 = *(u32*)color3;
    value2 = *(u32*)color2;
    value1 = *(u32*)color1;
    value0 = *(u32*)color0;

    local3 = value3;
    local2 = value2;
    local1 = value1;
    local0 = value0;

    btlDispTexPlainGX(texId, &local0, &local1, &local2, &local3);
}

void btlDispGetTexSize(s32 id, u16* width, u16* height) {
    u8 texObj[0x20];
    void* work = _battleWorkPointer;
    void* palette = *(void**)(*(s32*)((s32)work + 0x163F8) + 0xA0);
    TEXGetGXTexObjFromPalette(*(void**)palette, texObj, id);
    *width = GXGetTexObjWidth(texObj);
    *height = GXGetTexObjHeight(texObj);
}

void btlDispInit(void) {
    void* work = _battleWorkPointer;
    battleCameraInit();
    battleMenuDispInit();
    *(u32*)((s32)work + 0xEF8) |= 2;
}

s32 _GetStatusPoseType(BattleWorkUnit* unit) {
    s32* table = _status_pose_table;
    s32 status;
    if ((*(u32*)((s32)unit + 0x104) & 0x20000) != 0) {
        return 0x1B;
    }
    for (;;) {
        status = *table;
        if (status == -1) {
            break;
        }
        if (BtlUnit_CheckStatus(unit, (s8)status)) {
            return status;
        }
        table++;
    }
    return 0x1C;
}

void btlDispMain(void) {
    extern s32 evtCheckID(s32);
    extern void BtlUnit_GetPos(void*, f32*, f32*, f32*);
    extern f32 BattleGetFloorHeight(void*, f32, f32, f32);
    extern void BtlUnit_SetPos(void*, f32, f32, f32);
    extern void BtlUnit_GetStatus(void*, s32, s8*, s8*);
    extern void btlUnitPartsDisp(void*, void*);
    extern void btlUnitPartsBlurDisp(void*);
    extern void animPoseMain(s32);
    extern void gravityOffsetControl(void*);
    extern void floatOffsetControl(void*);
    extern void statusPoseControl(void*);
    extern void BtlUnit_GetPartsWorldPos(void*, f32*, f32*, f32*);
    extern s32 animPoseTestXLU(s32);
    extern s32 _GetStatusPoseType(void*);
    extern s32 BtlUnit_CheckStatus(void*, s32);
    extern s32 BtlUnit_CheckStatusFlag(void*, u32);
    extern s32 BtlUnit_CanActStatus(void*);
    extern const char* BtlUnit_GetPoseNameFromType(void*, s32);
    extern void animPoseSetAnim(s32, const char*, s32);
    extern void* searchPoseTbl(void*, s32);
    extern void btlDispAnimeSpeed(f32, void*);
    extern void* BattleGetUnitPtr(void*, s32);
    extern void BtlUnit_OffUnitFlag(void*, u32);
    extern void BtlUnit_OnUnitFlag(void*, u32);
    extern void BtlUnit_HpGaugeMain(void*);
    extern void BattleStatusEffectMain(void*);
    extern void BattleStatusIconMain(void*);
    extern void BattleStockExpDisp(void);
    extern void BattleStageDisp(void);
    extern void BattleCommandDisplay(void*);
    extern void BattleAudience_Disp(void);
    extern void BattleBreakSlot_Disp(void);
    void* battleWork = _battleWorkPointer;
    u8* unit;
    u8* owner;
    u8* part;
    s32 i;
    s32 poseType;
    s32 unitKind;
    s32 flags;
    s32 effectFlag;
    const char* poseName;
    u8 shown, target;
    s8 turns, strength;
    f32 x, y, z, ox, oy, oz, floor;
    f32 worldX, worldY, worldZ;

    for (i = 0; i < 64; i++) {
        unit = BattleGetUnitPtr(battleWork, i);
        if (unit == NULL) continue;

        if (*(s32*)(unit + 8) == 0xDF && *(s32*)(unit + 0x220) != 0) {
            owner = BattleGetUnitPtr(battleWork, *(s32*)(unit + 0x218));
            if (owner != NULL &&
                (*(s32*)(owner + 0x2A8) == 0 || !evtCheckID(*(s32*)(owner + 0x2A8)) || (*(u32*)(owner + 0x2AC) & 4)) &&
                (*(s32*)(owner + 0x2B4) == 0 || !evtCheckID(*(s32*)(owner + 0x2B4)))) {
                BtlUnit_GetPos(unit, &x, &y, &z);
                BtlUnit_GetPos(owner, &ox, &oy, &oz);
                oz += 7.0f;
                floor = BattleGetFloorHeight(battleWork, ox, oy, oz);
                if (*(s32*)(unit + 0x220) == 3) {
                    if (__fabs(x - ox) >= 1.0f) x += ox <= x ? -1.0f : 1.0f;
                    else { x = ox; *(s32*)(unit + 0x220) = 1; }
                    BtlUnit_SetPos(unit, x, floor, oz);
                } else if (*(s32*)(unit + 0x220) == 2) {
                    if (__fabs(x - ox) > 10.0f) x += ox <= x ? -((x - ox) - 10.0f) : ((ox - x) - 10.0f);
                    BtlUnit_SetPos(unit, x, oy, oz);
                } else {
                    BtlUnit_SetPos(unit, ox, floor, oz);
                }
            }
        }

        shown = unit[0x304];
        target = unit[0x303];
        BtlUnit_OffUnitFlag(unit, 2);
        if (shown != target) {
            BtlUnit_OnUnitFlag(unit, 2);
            if (shown < target) { shown += 12; if (shown > target) shown = target; }
            else { shown -= 2; if (shown < target) shown = target; }
        }
        unit[0x304] = shown;
        BtlUnit_HpGaugeMain(unit);
        *(f32*)(unit + 0x114) = 1.0f;
        BtlUnit_GetStatus(unit, 11, &turns, &strength);
        if (turns > 0 && strength < 0) {
            f32 scale = 1.0f + 0.5f * strength;
            if (scale < 0.5f) scale = 0.5f;
            *(f32*)(unit + 0x114) *= scale;
        }
        BtlUnit_GetStatus(unit, 10, &turns, &strength);
        if (turns > 0 && strength > 0) {
            f32 scale = 1.0f + strength;
            if (scale > 2.0f) scale = 2.0f;
            *(f32*)(unit + 0x114) *= scale;
        }
        BattleStatusEffectMain(unit);
        BattleStatusIconMain(unit);
        for (part = *(u8**)(unit + 0x14); part != NULL; part = *(u8**)part) {
            if (*(s32*)(part + 0x1C0) != -1) {
                if ((*(u32*)(part + 0x204) & 2) == 0) {
                    poseType = _GetStatusPoseType(unit);
                    unitKind = *(s32*)(unit + 8);
                    if (unitKind < 1 || unitKind >= 0xD8) {
                        if (poseType == 0x1C && !BtlUnit_CheckStatus(unit, 0x16) &&
                            (*(u32*)(unit + 0x138) & 0x20000000)) {
                            poseName = BtlUnit_GetPoseNameFromType(part, 0x39);
                            if (poseName == NULL) poseName = searchPoseTbl(*(void**)(part + 0x1BC), 0x1C);
                        } else if (poseType == 0x1C && !BtlUnit_CheckStatus(unit, 0x16) &&
                                   (*(u32*)(unit + 0x138) & 0x10000000)) {
                            poseName = BtlUnit_GetPoseNameFromType(part, 0x38);
                            if (poseName == NULL) poseName = searchPoseTbl(*(void**)(part + 0x1BC), 0x1C);
                        } else {
                            poseName = searchPoseTbl(*(void**)(part + 0x1BC), poseType);
                        }
                        if (BtlUnit_CheckStatusFlag(unit, 0x1000000) && BtlUnit_CanActStatus(unit))
                            poseName = BtlUnit_GetPoseNameFromType(part, 0x2D);
                    } else {
                        poseName = searchPoseTbl(*(void**)(part + 0x1BC), poseType);
                    }
                    if (BtlUnit_CheckStatus(unit, 0x13) != 0) {
                        btlDispAnimeSpeed(2.0f, part);
                    } else if (BtlUnit_CheckStatus(unit, 0x14) != 0) {
                        btlDispAnimeSpeed(0.5f, part);
                    } else {
                        btlDispAnimeSpeed(1.0f, part);
                    }
                } else {
                    poseName = (const char*)(part + 0x1C4);
                    btlDispAnimeSpeed(1.0f, part);
                }
                if (*(u32*)(part + 0x204) & 1) {
                    effectFlag = 0;
                    flags = *(u32*)(part + 0x204) & ~0x201;
                    *(u32*)(part + 0x204) = flags;
                    animPoseSetAnim(*(s32*)(part + 0x1C0), poseName, flags != 0);
                    part[0x214] = 0;
                    if (!(flags & 4)) {
                        part[0x215] = 1;
                        if (BtlUnit_CheckStatus(unit, 0x12) || part[0x4F3] != 0xFF) {
                            effectFlag = 1;
                            part[0x215] = 7;
                        }
                    } else {
                        part[0x215] = 0;
                    }
                    if (!(flags & 2) || (flags & 8) || (flags & 0x80)) {
                        part[0x215] = 1;
                        if (BtlUnit_CheckStatus(unit, 0x12) || part[0x4F3] != 0xFF) {
                            effectFlag = 1;
                            part[0x215] = 7;
                        }
                        if (BtlUnit_CheckStatus(unit, 7)) {
                            effectFlag = 1;
                            part[0x215] = 4;
                        }
                    }
                    if (effectFlag && (*(u32*)(part + 0x1AC) & 0x8000)) {
                        effectFlag = 0;
                        part[0x215] = 0;
                    }
                    flags = *(u32*)(part + 0x204);
                    if (!(flags & 4)) {
                        if (effectFlag && !(flags & 0x40)) *(u32*)(part + 0x204) = flags | 0x40;
                        else if (!effectFlag && (flags & 0x40)) *(u32*)(part + 0x204) = flags & ~0x40;
                    }
                }
                gravityOffsetControl(part);
                floatOffsetControl(part);
                statusPoseControl(part);
                BtlUnit_GetPartsWorldPos(part, &worldX, &worldY, &worldZ);
                animPoseMain(*(s32*)(part + 0x1C0));
                if ((*(u32*)(unit + 0x104) & 0x1000000) == 0 &&
                    (*(u32*)(part + 0x1AC) & 0x1000000) == 0 &&
                    (*(u32*)(part + 0x204) & 0x100) == 0) {
                    if (part[0x1F3] == 0xFF && animPoseTestXLU(*(s32*)(part + 0x1C0)) == 0) {
                        dispEntry(1, 1, btlUnitPartsDisp, 0.0f, part);
                    } else {
                        dispEntry(1, 2, btlUnitPartsDisp, 0.0f, part);
                    }
                }
                if ((*(u32*)(part + 0x1AC) & 0x4000000) != 0) {
                    dispEntry(1, 2, btlUnitPartsBlurDisp, 0.0f, part);
                }
            }
        }
    }
    BattleStockExpDisp();
    BattleStageDisp();
    BattleCommandDisplay(battleWork);
    BattleAudience_Disp();
    BattleBreakSlot_Disp();
    if ((*(u32*)battleWork & 0x10000) != 0) {
        *(u32*)battleWork &= ~0x10000;
    }
    if ((*(u32*)battleWork & 0x20000) != 0) {
        *(u32*)battleWork &= ~0x20000;
    }
}

void btlUnitPartsDisp(s32 cameraId, void* part) {
    extern void PSMTXIdentity(void*);
    extern void PSMTXTrans(void*, f32, f32, f32);
    extern void PSMTXScale(void*, f32, f32, f32);
    extern void PSMTXRotRad(void*, s32, f32);
    extern void PSMTXConcat(void*, void*, void*);
    extern void PSMTXCopy(void*, void*);
    extern void animPoseSetMaterialEvtColor(s32, void*);
    extern void animPoseSetMaterialFlagOn(s32, u32);
    extern void animPoseSetMaterialFlagOff(s32, u32);
    extern void animPoseDrawMtx(s32, void*, s32, f32, f32);
    extern s32 BtlUnit_CheckStatus(void*, s32);
    extern void btlUnitPartsBlurControl(f64, void*, s32, void*, void*);
    extern f32 angleABf(f32, f32, f32, f32);
    extern f32 reviseAngle(f32);
    extern f32 float_270_80422258;
    extern f32 float_90_8042225c;
    extern f32 float_neg1_80422260;
    extern f32 float_180_80422264;
    extern f32 float_neg5_80422268;

    /*
     * The target frame contains 29 distinct 0x30-byte matrices occupying
     * r1+0x10 through r1+0x550.  MWCC allocates these aggregates downward;
     * this declaration order mirrors the target slots from high to low.
     */
    f32 unitResult[3][4];       /* target r1+0x550 */
    f32 result[3][4];           /* target r1+0x520 */
    f32 altPartScale[3][4];     /* target r1+0x4F0 */
    f32 blurMtx[3][4];          /* target r1+0x4C0 */
    f32 work[3][4];             /* target r1+0x490 */
    f32 altPartTranslate[3][4]; /* target r1+0x460 */
    f32 altPartTranslateNeg[3][4]; /* target r1+0x430 */
    f32 altPartTranslatePos[3][4]; /* target r1+0x400 */
    f32 altPartRotateX[3][4];   /* target r1+0x3D0 */
    f32 altPartRotateY[3][4];   /* target r1+0x3A0 */
    f32 altPartRotateZ[3][4];   /* target r1+0x370 */
    f32 altWork[3][4];          /* target r1+0x340 */
    f32 unitWorld[3][4];        /* target r1+0x310 */
    f32 unitDisp[3][4];         /* target r1+0x2E0 */
    f32 unitRotateX[3][4];      /* target r1+0x2B0 */
    f32 unitRotateY[3][4];      /* target r1+0x280 */
    f32 unitRotateZ[3][4];      /* target r1+0x250 */
    f32 unitTranslateNeg[3][4]; /* target r1+0x220 */
    f32 unitTranslatePos[3][4]; /* target r1+0x1F0 */
    f32 unitScale[3][4];        /* target r1+0x1C0 */
    f32 partTranslate[3][4];    /* target r1+0x190 */
    f32 partDisp[3][4];         /* target r1+0x160 */
    f32 partRotateX[3][4];      /* target r1+0x130 */
    f32 partRotateY[3][4];      /* target r1+0x100 */
    f32 partRotateZ[3][4];      /* target r1+0x0D0 */
    f32 partTranslateNeg[3][4]; /* target r1+0x0A0 */
    f32 partTranslatePos[3][4]; /* target r1+0x070 */
    f32 partScale[3][4];        /* target r1+0x040 */
    f32 zMtx[3][4];             /* target r1+0x010 */

    u32 materialColor;
    u32 blurColor;
    u8* p;
    u8* unit;
    u8* battleWork;
    u8* camera;
    void (*callback)(void*, s32);
    f32 zSign;
    f32 faceAngle;
    f32 drawRot;
    f32 cameraCorrection;
    f32 cameraAngle;
    f32 angle;
    f32 x;
    f32 y;
    f32 z;
    f32 floatOffset;
    f32 gravityOffset;
    f32 side;
    f32 faceValue;
    s32 order;

    p = part;
    battleWork = (u8*)_battleWorkPointer;
    zSign = float_1_8042224c;
    unit = *(u8**)(p + 0x4EC);

    if (*(s32*)(p + 0x1C0) < 0) {
        return;
    }

    /*
     * Common unit transform.  unitWorld is intentionally materialized even
     * though the final target paths do not consume it.
     */
    PSMTXTrans(
        unitTranslateNeg,
        -*(f32*)(unit + 0x78),
        -*(f32*)(unit + 0x7C),
        -*(f32*)(unit + 0x80));
    PSMTXTrans(
        unitTranslatePos,
        *(f32*)(unit + 0x78),
        *(f32*)(unit + 0x7C),
        *(f32*)(unit + 0x80));
    PSMTXTrans(
        unitDisp,
        *(f32*)(unit + 0x54),
        *(f32*)(unit + 0x58),
        *(f32*)(unit + 0x5C));
    PSMTXTrans(
        unitWorld,
        *(f32*)(unit + 0x3C),
        *(f32*)(unit + 0x40),
        *(f32*)(unit + 0x44));

    PSMTXRotRad(
        unitRotateX,
        0x78,
        float_deg2rad_8042223c *
            (*(f32*)(unit + 0x6C) + *(f32*)(unit + 0x60)));
    PSMTXRotRad(
        unitRotateY,
        0x79,
        float_deg2rad_8042223c *
            (*(f32*)(unit + 0x70) + *(f32*)(unit + 0x64)));
    PSMTXRotRad(
        unitRotateZ,
        0x7A,
        float_deg2rad_8042223c *
            (*(f32*)(unit + 0x74) + *(f32*)(unit + 0x68)));

    PSMTXScale(
        unitScale,
        *(f32*)(unit + 0x114) *
            *(f32*)(unit + 0x9C) *
            *(f32*)(unit + 0x90),
        *(f32*)(unit + 0x114) *
            *(f32*)(unit + 0xA0) *
            *(f32*)(unit + 0x94),
        *(f32*)(unit + 0xA4) *
            *(f32*)(unit + 0x98));

    PSMTXIdentity(work);
    PSMTXConcat(unitScale, work, work);
    PSMTXConcat(unitTranslateNeg, work, work);
    PSMTXConcat(unitRotateY, work, work);
    PSMTXConcat(unitRotateZ, work, work);
    PSMTXConcat(unitRotateX, work, work);
    PSMTXConcat(unitTranslatePos, work, unitResult);
    PSMTXConcat(unitDisp, unitResult, unitResult);

    /*
     * Target lays the 0x10000000 path out ahead of the branch-on-zero path.
     * The zero case enters the normal owner-relative path.
     */
    if ((*(u32*)(p + 0x1AC) & 0x10000000) != 0) {
        faceAngle = float_270_80422258;
        if (*(s8*)(p + 0xBD) >= 0) {
            faceAngle = float_90_8042225c;
        }

        x = *(f32*)(p + 0x18);
        floatOffset = getFloatDispOffset(part);
        gravityOffset = getGravityDispOffset(part);
        y = *(f32*)(p + 0x1C) + gravityOffset + floatOffset;
        z = *(f32*)(p + 0x20);

        camera = (u8*)camGetCurPtr();
        cameraAngle = angleABf(
            *(f32*)(camera + 0x0C),
            *(f32*)(camera + 0x14),
            *(f32*)(camera + 0x18),
            *(f32*)(camera + 0x20));
        angle = angleABf(
            *(f32*)(camera + 0x0C),
            *(f32*)(camera + 0x14),
            x,
            z);
        cameraCorrection = reviseAngle(cameraAngle - angle);

        PSMTXTrans(
            altPartTranslateNeg,
            -*(f32*)(p + 0x54),
            -*(f32*)(p + 0x58),
            -*(f32*)(p + 0x5C));
        PSMTXTrans(
            altPartTranslatePos,
            *(f32*)(p + 0x54),
            *(f32*)(p + 0x58),
            *(f32*)(p + 0x5C));

        PSMTXRotRad(
            altPartRotateX,
            0x78,
            float_deg2rad_8042223c *
                (*(f32*)(p + 0x48) + *(f32*)(p + 0x3C)));
        PSMTXRotRad(
            altPartRotateY,
            0x79,
            float_deg2rad_8042223c *
                (cameraCorrection +
                 (*(f32*)(p + 0x4C) + *(f32*)(p + 0x40))));
        PSMTXRotRad(
            altPartRotateZ,
            0x7A,
            float_deg2rad_8042223c *
                (*(f32*)(p + 0x50) + *(f32*)(p + 0x44)));

        PSMTXTrans(altPartTranslate, x, y, z);

        drawRot =
            reviseAngle(float_neg1_80422260 * faceAngle -
                        float_90_8042225c);

        angle = reviseAngle(
            *(f32*)(p + 0x40) +
            drawRot +
            *(f32*)(p + 0x4C));

        camera = (u8*)camGetCurPtr();
        cameraAngle = angleABf(
            *(f32*)(camera + 0x0C),
            *(f32*)(camera + 0x14),
            *(f32*)(camera + 0x18),
            *(f32*)(camera + 0x20));
        angle = reviseAngle(angle + cameraAngle);

        if (angle >= float_90_8042225c &&
            angle <= float_270_80422258) {
            zSign *= float_neg1_80422260;
            PSMTXRotRad(
                altPartRotateY,
                0x79,
                float_deg2rad_8042223c *
                    -(float_2_80422250 *
                          (float_180_80422264 -
                           (angle - cameraAngle)) -
                      (cameraCorrection +
                       (*(f32*)(p + 0x4C) +
                        *(f32*)(p + 0x40)))));
        }

        if ((*(u32*)(p + 0x1AC) & 0x08000000) == 0) {
            PSMTXScale(
                altPartScale,
                *(f32*)(unit + 0x114) *
                    *(f32*)(p + 0x6C) *
                    *(f32*)(p + 0x60),
                *(f32*)(unit + 0x114) *
                    *(f32*)(p + 0x70) *
                    *(f32*)(p + 0x64),
                zSign *
                    *(f32*)(p + 0x74) *
                    *(f32*)(p + 0x68));
        } else {
            PSMTXScale(
                altPartScale,
                *(f32*)(p + 0x6C) *
                    *(f32*)(p + 0x60),
                *(f32*)(p + 0x70) *
                    *(f32*)(p + 0x64),
                zSign *
                    *(f32*)(p + 0x74) *
                    *(f32*)(p + 0x68));
        }

        PSMTXIdentity(altWork);
        PSMTXConcat(altPartScale, altWork, altWork);
        PSMTXConcat(altPartTranslateNeg, altWork, altWork);
        PSMTXConcat(altPartRotateZ, altWork, altWork);
        PSMTXConcat(altPartRotateX, altWork, altWork);
        PSMTXConcat(altPartRotateY, altWork, altWork);
        PSMTXConcat(altPartTranslatePos, altWork, altWork);
        PSMTXConcat(altPartTranslate, altWork, result);

        PSMTXCopy(result, blurMtx);
        PSMTXTrans(
            zMtx,
            float_0_80422240,
            float_0_80422240,
            float_neg5_80422268);
        PSMTXConcat(zMtx, blurMtx, blurMtx);
    } else {
        /*
         * Normal owner-relative path.  A zero part face direction falls back
         * to the alliance attack direction exactly as target PPC does.
         */
        faceAngle = float_270_80422258;
        if (*(s8*)(unit + 0x189) >= 0) {
            faceAngle = float_90_8042225c;
        }

        faceValue = (f32)*(s8*)(p + 0xBD);
        side = float_1_8042224c;
        if (faceValue <= float_0_80422240) {
            side = float_neg1_80422260;
            if (faceValue >= float_0_80422240) {
                side = (f32)*(s8*)(
                    battleWork +
                    ((s8)*(s8*)(unit + 0x0C) * 8) +
                    0x0A);
            }
        }

        x = *(f32*)(unit + 0x3C) +
            *(f32*)(p + 0x24) * side;
        floatOffset = getFloatDispOffset(part);
        gravityOffset = getGravityDispOffset(part);
        y = *(f32*)(unit + 0x40) +
            *(f32*)(p + 0x28) +
            gravityOffset +
            floatOffset;
        z = *(f32*)(unit + 0x44) +
            *(f32*)(p + 0x2C);

        cameraCorrection = float_0_80422240;
        if ((*(u32*)(*(u8**)(p + 0x4EC) + 0x104) &
             0x04000000) != 0) {
            camera = (u8*)camGetCurPtr();
            cameraAngle = angleABf(
                *(f32*)(camera + 0x0C),
                *(f32*)(camera + 0x14),
                *(f32*)(camera + 0x18),
                *(f32*)(camera + 0x20));
            angle = angleABf(
                *(f32*)(camera + 0x0C),
                *(f32*)(camera + 0x14),
                x,
                z);
            cameraCorrection =
                reviseAngle(cameraAngle - angle);
        }

        PSMTXTrans(
            partTranslateNeg,
            -*(f32*)(p + 0x54),
            -*(f32*)(p + 0x58),
            -*(f32*)(p + 0x5C));
        PSMTXTrans(
            partTranslatePos,
            *(f32*)(p + 0x54),
            *(f32*)(p + 0x58),
            *(f32*)(p + 0x5C));

        PSMTXRotRad(
            partRotateX,
            0x78,
            float_deg2rad_8042223c *
                (*(f32*)(p + 0x48) + *(f32*)(p + 0x3C)));
        PSMTXRotRad(
            partRotateY,
            0x79,
            float_deg2rad_8042223c *
                (cameraCorrection +
                 (*(f32*)(p + 0x4C) + *(f32*)(p + 0x40))));
        PSMTXRotRad(
            partRotateZ,
            0x7A,
            float_deg2rad_8042223c *
                (*(f32*)(p + 0x50) + *(f32*)(p + 0x44)));

        PSMTXTrans(partTranslate, x, y, z);
        PSMTXTrans(
            partDisp,
            *(f32*)(p + 0x30),
            *(f32*)(p + 0x34),
            *(f32*)(p + 0x38));

        drawRot =
            reviseAngle(float_neg1_80422260 * faceAngle -
                        float_90_8042225c);

        /*
         * When the unit itself is not locked against facing adjustment, the
         * target may rebuild the unit matrix with a mirrored Z scale and a
         * part-selected rotation concatenation order.
         */
        if ((*(u32*)(*(u8**)(p + 0x4EC) + 0x1C) &
             0x01000000) == 0) {
            angle = reviseAngle(
                *(f32*)(p + 0x40) +
                *(f32*)(unit + 0x64) +
                *(f32*)(p + 0x4C) +
                drawRot +
                *(f32*)(unit + 0x70));

            camera = (u8*)camGetCurPtr();
            cameraAngle = angleABf(
                *(f32*)(camera + 0x0C),
                *(f32*)(camera + 0x14),
                *(f32*)(camera + 0x18),
                *(f32*)(camera + 0x20));
            angle = reviseAngle(angle + cameraAngle);

            if (angle >= float_90_8042225c &&
                angle <= float_270_80422258) {
                PSMTXScale(
                    unitScale,
                    *(f32*)(unit + 0x114) *
                        *(f32*)(unit + 0x9C) *
                        *(f32*)(unit + 0x90),
                    *(f32*)(unit + 0x114) *
                        *(f32*)(unit + 0xA0) *
                        *(f32*)(unit + 0x94),
                    float_neg1_80422260 *
                        *(f32*)(unit + 0xA4) *
                        *(f32*)(unit + 0x98));

                PSMTXIdentity(work);
                PSMTXConcat(unitScale, work, work);
                PSMTXConcat(unitTranslateNeg, work, work);

                order = *(s8*)(p + 0x4FC);
                switch (order) {
                    case 0:
                        PSMTXConcat(unitRotateY, work, work);
                        PSMTXConcat(unitRotateZ, work, work);
                        PSMTXConcat(unitRotateX, work, work);
                        break;
                    case 1:
                        PSMTXConcat(unitRotateY, work, work);
                        PSMTXConcat(unitRotateX, work, work);
                        PSMTXConcat(unitRotateZ, work, work);
                        break;
                    case 2:
                        PSMTXConcat(unitRotateX, work, work);
                        PSMTXConcat(unitRotateY, work, work);
                        PSMTXConcat(unitRotateZ, work, work);
                        break;
                    case 3:
                        PSMTXConcat(unitRotateX, work, work);
                        PSMTXConcat(unitRotateZ, work, work);
                        PSMTXConcat(unitRotateY, work, work);
                        break;
                    case 4:
                        PSMTXConcat(unitRotateZ, work, work);
                        PSMTXConcat(unitRotateX, work, work);
                        PSMTXConcat(unitRotateY, work, work);
                        break;
                    case 5:
                        PSMTXConcat(unitRotateZ, work, work);
                        PSMTXConcat(unitRotateY, work, work);
                        PSMTXConcat(unitRotateX, work, work);
                        break;
                }

                PSMTXConcat(
                    unitTranslatePos,
                    work,
                    unitResult);
                PSMTXConcat(
                    unitDisp,
                    unitResult,
                    unitResult);
            }
        }

        PSMTXScale(
            partScale,
            *(f32*)(p + 0x6C) *
                *(f32*)(p + 0x60),
            *(f32*)(p + 0x70) *
                *(f32*)(p + 0x64),
            float_1_8042224c *
                *(f32*)(p + 0x74) *
                *(f32*)(p + 0x68));

        PSMTXIdentity(work);
        PSMTXConcat(unitResult, work, work);
        PSMTXConcat(partScale, work, work);
        PSMTXConcat(partTranslateNeg, work, work);

        order = *(s8*)(p + 0x4FC);
        switch (order) {
            case 0:
                PSMTXConcat(partRotateY, work, work);
                PSMTXConcat(partRotateZ, work, work);
                PSMTXConcat(partRotateX, work, work);
                break;
            case 1:
                PSMTXConcat(partRotateY, work, work);
                PSMTXConcat(partRotateX, work, work);
                PSMTXConcat(partRotateZ, work, work);
                break;
            case 2:
                PSMTXConcat(partRotateX, work, work);
                PSMTXConcat(partRotateY, work, work);
                PSMTXConcat(partRotateZ, work, work);
                break;
            case 3:
                PSMTXConcat(partRotateX, work, work);
                PSMTXConcat(partRotateZ, work, work);
                PSMTXConcat(partRotateY, work, work);
                break;
            case 4:
                PSMTXConcat(partRotateZ, work, work);
                PSMTXConcat(partRotateX, work, work);
                PSMTXConcat(partRotateY, work, work);
                break;
            case 5:
                PSMTXConcat(partRotateZ, work, work);
                PSMTXConcat(partRotateY, work, work);
                PSMTXConcat(partRotateX, work, work);
                break;
        }

        PSMTXConcat(partTranslatePos, work, work);
        PSMTXConcat(partTranslate, work, result);
        PSMTXConcat(partDisp, result, result);

        PSMTXCopy(result, blurMtx);
        PSMTXTrans(
            zMtx,
            float_0_80422240,
            float_0_80422240,
            float_neg5_80422268);
        PSMTXConcat(zMtx, blurMtx, blurMtx);
    }

    /*
     * Rendering/material tail.  Target uses two distinct 4-byte color locals
     * and reloads the callback pointer at both call sites.
     */
    *(u32*)(p + 0x218) = *(u32*)(p + 0x4F4);

    if ((*(u32*)(p + 0x204) & 0x10) != 0) {
        p[0x218] = 0;
        p[0x219] = 0;
        p[0x21A] = 0;
    }

    if ((*(u32*)(p + 0x204) & 0x20) != 0) {
        p[0x218] = 0;
        p[0x219] = 0;
        p[0x21A] = 0;
    }

    if (p[0x218] == 0xFF &&
        p[0x219] == 0xFF &&
        p[0x21A] == 0xFF) {
        if (BtlUnit_CheckStatus(unit, 4) != 0) {
            p[0x218] =
                (p[0x218] * 0xA0) / 0xFF;
            p[0x21A] =
                (p[0x218] * 0xA0) / 0xFF;
        }

        p[0x218] =
            (p[0x218] * unit[0x312]) / 0xFF;
        p[0x219] =
            (p[0x219] * unit[0x312]) / 0xFF;
        p[0x21A] =
            (p[0x21A] * unit[0x312]) / 0xFF;
    }

    materialColor = *(u32*)(p + 0x218);
    animPoseSetMaterialEvtColor(
        *(s32*)(p + 0x1C0),
        &materialColor);
    animPoseSetMaterialFlagOn(
        *(s32*)(p + 0x1C0),
        0x40);

    callback =
        *(void (**)(void*, s32))(p + 0x210);
    if (callback != 0) {
        callback(part, 0);
    }

    if (BtlUnit_CheckStatus(unit, 0x10) != 0) {
        animPoseSetMaterialFlagOn(
            *(s32*)(p + 0x1C0),
            0x800000);
    } else {
        animPoseSetMaterialFlagOff(
            *(s32*)(p + 0x1C0),
            0x800000);
    }

    animPoseDrawMtx(
        *(s32*)(p + 0x1C0),
        result,
        1,
        drawRot,
        float_2_80422250);
    animPoseDrawMtx(
        *(s32*)(p + 0x1C0),
        result,
        2,
        drawRot,
        float_2_80422250);
    animPoseDrawMtx(
        *(s32*)(p + 0x1C0),
        result,
        3,
        drawRot,
        float_2_80422250);

    callback =
        *(void (**)(void*, s32))(p + 0x210);
    if (callback != 0) {
        callback(part, 1);
    }

    blurColor = *(u32*)(p + 0x218);
    btlUnitPartsBlurControl(
        (f64)drawRot,
        part,
        0,
        blurMtx,
        &blurColor);
}

void btlUnitItemDisp(s32 param_1, void* unit) {
    extern s32 BtlUnit_GetHeight(void* unit);
    extern s32 BtlUnit_EnemyItemCanUseCheck(s32 item);
    extern void BtlUnit_GetPos(void* unit, f32* x, f32* y, f32* z);
    extern void iconDispGx(f64 scale, f32* pos, u16 flags, u16 iconId);
    s32 iconItem;
    s32 height;
    f32 pos[3];

    height = BtlUnit_GetHeight(unit);
    iconItem = BtlUnit_EnemyItemCanUseCheck(*(s32*)((s32)unit + 0x308));
    if ((*(u32*)((s32)_battleWorkPointer + 0xEF4) & 0x20000) != 0 &&
        iconItem == 0) {
        iconItem = 0x94;
    }

    if (iconItem != 0) {
        BtlUnit_GetPos(unit, &pos[0], &pos[1], &pos[2]);
        pos[0] += *(f32*)((s32)unit + 0xB4);
        pos[1] += *(f32*)((s32)unit + 0xB8);
        pos[2] += *(f32*)((s32)unit + 0xBC);
        pos[0] += *(f32*)((s32)unit + 0xC0);
        pos[1] += *(f32*)((s32)unit + 0xC4);
        pos[2] += *(f32*)((s32)unit + 0xC8);
        iconDispGx(float_1_8042224c, pos, 0,
                   *(u16*)(itemDataTable + iconItem * 0x28 + 0x20));
    }
}

void btlUnitItemDisp2(s32 param_1, void* unit) {
    void* battleWork;
    s32 type;
    u32 flags;

    type = *(s32*)((s32)unit + 8);
    battleWork = _battleWorkPointer;
    if (type >= 1 && type < 0xD8) {
        goto display;
    }
    if (type >= 0xDE && type < 0xE0) {
        goto display;
    }
    if (type >= 0xE0) {
        if (type < 0xE7) {
            goto display;
        }
        goto done;
    }
    goto done;
display:
    flags = *(u32*)((s32)battleWork + 0xEF4);
    if ((flags & 0x20000) != 0) {
        btlUnitItemDisp(param_1, unit);
    } else {
        *(u32*)((s32)battleWork + 0xEF4) = flags | 0x20000;
        btlUnitItemDisp(param_1, unit);
        *(u32*)((s32)battleWork + 0xEF4) &= ~0x20000;
    }
done:
    return;
}

void btlUnitStolenItemDisp(s32 param_1, void* unit) {
    extern f32 float_5_80422254;
    extern void BtlUnit_GetPos(void* unit, f32* x, f32* y, f32* z);
    extern s32 BtlUnit_GetHeight(void* unit);
    extern void iconDispGx(f64 scale, f32* pos, u16 flags, u16 iconId);
    s32 item;
    s32 height;
    f32 pos[3];

    item = *(s32*)((s32)unit + 0x308);
    BtlUnit_GetPos(unit, &pos[0], &pos[1], &pos[2]);
    height = BtlUnit_GetHeight(unit);
    pos[1] += *(f32*)((s32)unit + 0x114) * (f32)height + float_5_80422254;
    iconDispGx(float_1_8042224c, pos, 0,
               *(u16*)(itemDataTable + item * 0x28 + 0x20));
}

void btlUnitPartsBlurControl(f64 rotation, void* part, s32 a3, void* orientation, s32* color) {
    s32 local;
    if ((*(u32*)((s32)part + 0x1AC) & 0x04000000) != 0) {
        local = *color;
        _partsBlurControl(rotation, part, 1, orientation, &local);
    } else {
        local = *color;
        _partsBlurControl(rotation, part, 0, orientation, &local);
    }
}

void _partsBlurControl(f64 rotation, void* part, u32 flags, void* orientation, void* color) {
    BattleWorkUnitPartBlur* blurWork;
    u32* blurFlags;
    s32 i;

    blurFlags = (u32*)((u8*)part + 0x21C);
    blurWork = (BattleWorkUnitPartBlur*)((u8*)part + 0x228);
    for (i = 9; i > 0; i--) {
        blurWork[i] = blurWork[i - 1];
    }
    blurWork[0].flags = flags;
    PSMTXCopy(orientation, blurWork[0].orientation);
    blurWork[0].rotation = (f32)rotation;
    blurWork[0].baseColor = *(u32*)color;
    blurWork[0].blurColors[0] = *(u32*)((u8*)part + 0x220);
    blurWork[0].blurColors[1] = *(u32*)((u8*)part + 0x224);
    if (((*blurFlags & 1) != 0) && ((blurWork[1].flags & 1) != 0)) {
        blurWork[0].flags &= ~1;
    }
}

void btlUnitPartsBlurDisp(s32 param_1, void* part) {
    extern void animPoseSetMaterialEvtColor(s32 poseId, void* color);
    extern void animPoseSetMaterialFlagOn(s32 poseId, u32 flag);
    extern void animPoseDrawMtx(s32 poseId, void* mtx, s32 mode, f32 rot, f32 scale);
    s32 i;
    s32 offset;
    s32 base;
    s32 entry;
    u32 color;
    u32 colorCopy;
    u8* colorBytes;
    s32 inv;
    s32 r;
    s32 g;
    s32 b;
    s32 a;

    base = (s32)part + 0x21C;
    for (i = 0, offset = 0; i < 10; i++, offset += 0x44) {
        entry = base + offset + 0xC;
        color = *(u32*)(entry + 0x38);
        if ((*(u32*)entry & 1) != 0) {
            inv = 10 - i;
            colorBytes = (u8*)&color;

            r = (((*(u8*)(entry + 0x3C) * inv) / 10) + ((*(u8*)(entry + 0x40) * i) / 10)) & 0xFF;
            colorBytes[0] = (u8)((colorBytes[0] * r) / 255);

            g = (((*(u8*)(entry + 0x3D) * inv) / 10) + ((*(u8*)(entry + 0x41) * i) / 10)) & 0xFF;
            colorBytes[1] = (u8)((colorBytes[1] * g) / 255);

            a = (((*(u8*)(entry + 0x3F) * inv) / 10) + ((*(u8*)(entry + 0x43) * i) / 10)) & 0xFF;
            colorBytes[3] = (u8)((colorBytes[3] * a) / 255);

            b = (((*(u8*)(entry + 0x3E) * inv) / 10) + ((*(u8*)(entry + 0x42) * i) / 10)) & 0xFF;
            colorBytes[2] = (u8)((colorBytes[2] * b) / 255);

            if (colorBytes[3] != 0) {
                colorCopy = color;
                animPoseSetMaterialEvtColor(*(s32*)((s32)part + 0x1C0), &colorCopy);
                animPoseSetMaterialFlagOn(*(s32*)((s32)part + 0x1C0), 0x40);
                animPoseDrawMtx(*(s32*)((s32)part + 0x1C0), (void*)(entry + 4), 1, *(f32*)(entry + 0x34), float_2_80422250);
                animPoseDrawMtx(*(s32*)((s32)part + 0x1C0), (void*)(entry + 4), 2, *(f32*)(entry + 0x34), float_2_80422250);
                animPoseDrawMtx(*(s32*)((s32)part + 0x1C0), (void*)(entry + 4), 3, *(f32*)(entry + 0x34), float_2_80422250);
            }
        }
    }
}

void btlDispEntAnime(void* unit) {
    extern s32 animPoseEntry(void*, s32);
    void* part;
    void* poseTable;
    void* animData;
    void* poseName;

    part = *(void**)((s32)unit + 0x14);
    while (part != NULL) {
        poseTable = *(void**)((s32)part + 0x1BC);
        if (poseTable == NULL) {
            *(s32*)((s32)part + 0x1C0) = -1;
        } else {
            animData = *(void**)(*(s32*)((s32)part + 4) + 8);
            if (animData == NULL) {
                *(s32*)((s32)part + 0x1C0) = -1;
            } else {
                strcpy((char*)((s32)part + 0x1C4), searchPoseTbl(poseTable, 0x1C));
                *(s32*)((s32)part + 0x1C0) = animPoseEntry(animData, 1);
            }
        }
        part = *(void**)part;
    }
}

void btlDispChangeAnime(BattleWorkUnitPart* part, const char*name, BOOL a3) {

    strcpy((char*)((s32)part + 0x1C4), name);
    *(u32*)((s32)part + 0x204) |= 2;
    *(u32*)((s32)part + 0x204) |= 1;
    *(u32*)((s32)part + 0x204) &= ~0x80;
    *(u32*)((s32)part + 0x204) &= ~0x200;
    *(u32*)((s32)part + 0x204) &= ~0x10;
    if (a3 != 0) {
        *(u32*)((s32)part + 0x204) |= 8;
    } else {
        *(u32*)((s32)part + 0x204) &= ~8;
    }
}

void btlDispChangePeraAnime(void* part) {
    s32 animId;
    *(u32*)((s32)part + 0x204) |= 4;
    *(u8*)((s32)part + 0x215) = 0;
    animPoseSetEffectAnim(*(void**)((s32)part + 0x1C0), animId, 1);
}

void btlDispPoseAnime(void* part) {
    *(u32*)((s32)part + 0x204) &= ~0x12;
    *(u32*)((s32)part + 0x204) |= 1;
    *(u32*)((s32)part + 0x204) &= ~0x80;
    *(u32*)((s32)part + 0x204) &= ~4;
}

void gravityOffsetControl(void* part) {
    s32 active;
    void* unit;
    void* battleWork;
    u16 value;

    battleWork = _battleWorkPointer;
    active = 1;
    unit = *(void**)((s32)part + 0x4EC);
    if ((*(u32*)((s32)battleWork + 0xEF4) & 0x40000000) == 0) {
        active = 0;
    } else if ((*(u32*)((s32)unit + 0x104) & 0x1004) != 0) {
        active = 0;
    }

    if (active != 0) {
        *(u16*)((s32)part + 0x1A6) = *(u16*)((s32)part + 0x1A6) + 1;
        return;
    }

    value = *(u16*)((s32)part + 0x1A6);
    if (value != 0) {
        *(u16*)((s32)part + 0x1A6) = value % 120;
        value = *(u16*)((s32)part + 0x1A6);
        if (value >= 60) {
            *(u16*)((s32)part + 0x1A6) = value + 1;
            return;
        }
        if (value != 0) {
            *(u16*)((s32)part + 0x1A6) = value - 1;
        }
    }
}

f32 getGravityDispOffset(void* part) {
    s32 value = (*(u16*)((s32)part + 0x1A6) * 3 + 0x10E) % 0x168;
    f32 sin = sinfd((f32)value);
    return float_4_80422224 + float_4_80422224 * sin;
}


void floatOffsetControl(void* part) {
    s32 active;
    u16 value;
    void* unit;

    active = 1;
    unit = *(void**)((s32)part + 0x4EC);
    if ((*(u32*)((s32)unit + 0x1C) & 0x100000) == 0) {
        active = 0;
    } else if ((*(u32*)((s32)unit + 0x1C) & 0x200000) == 0) {
        active = 0;
    } else if (BtlUnit_CheckStatus(unit, 0x1B) != 0) {
        active = 0;
    }

    if (active != 0) {
        *(u16*)((s32)part + 0x1A8) = *(u16*)((s32)part + 0x1A8) + 1;
    } else {
        value = *(u16*)((s32)part + 0x1A8);
        if (value != 0) {
            *(u16*)((s32)part + 0x1A8) = value % 90;
            value = *(u16*)((s32)part + 0x1A8);
            if (value >= 45) {
                *(u16*)((s32)part + 0x1A8) = value + 1;
            } else if (value != 0) {
                *(u16*)((s32)part + 0x1A8) = value - 1;
            }
        }
    }
}

f32 getFloatDispOffset(void* part) {
    s32 value = ((*(u16*)((s32)part + 0x1A8) << 2) + 0x10E) % 0x168;
    f32 sin = sinfd((f32)value);
    return float_2_80422250 + float_2_80422250 * sin;
}

void statusPoseControl(void* part) {
    switch (*(u8*)((s32)part + 0x215)) {
        case 0:
            break;
        case 2:
        case 3:
        case 7:
            _pose_one_pattern(part);
            break;
        case 4:
        case 5:
        case 6:
            _pose_two_pattern(part);
            break;
        default:
            _pose_def(part);
            break;
    }
}

void _pose_def(void* part) {
    if ((s32)*(u8*)((s32)part + 0x214) == 0) {
        goto set_default;
    }
    goto done;
set_default:
    *(u8*)((s32)part + 0x214) = 1;
    animPoseSetEffect(*(void**)((s32)part + 0x1C0), 0, 0);
    if ((*(u32*)((s32)*(void**)((s32)part + 0x4EC) + 0x1C) & 0x01000000) != 0) {
        animPoseSetMaterialLightFlagOn(*(void**)((s32)part + 0x1C0), 8);
    }
done:
    return;
}

void _pose_one_pattern(void* part) {
    extern u8 vec3_802ee3a0[];
    u8* base;

    base = vec3_802ee3a0;
    if ((s32)*(u8*)((s32)part + 0x214) == 0) {
        goto body;
    }
    goto done;
body:
    *(u8*)((s32)part + 0x214) = 1;
    switch (*(u8*)((s32)part + 0x215)) {
        case 2:
            animPoseSetEffect(*(void**)((s32)part + 0x1C0), (s32)(base + 0x68), 1);
            animPoseSetEffectAnim(*(void**)((s32)part + 0x1C0), (s32)(base + 0x78), 1);
            break;
        case 3:
            animPoseSetEffect(*(void**)((s32)part + 0x1C0), (s32)(base + 0x68), 1);
            animPoseSetEffectAnim(*(void**)((s32)part + 0x1C0), (s32)(base + 0x98), 1);
            break;
        case 7:
            animPoseSetEffect(*(void**)((s32)part + 0x1C0), (s32)(base + 0x68), 1);
            animPoseSetEffectAnim(*(void**)((s32)part + 0x1C0), (s32)(base + 0xA0), 1);
            break;
    }
done:
    return;
}

void _pose_two_pattern(void* part) {
    extern u8 vec3_802ee3a0[];
    extern s32 sysMsec2Frame(s32 msec);
    extern s32 irand(s32 max);
    u8* base;
    s32 frame;

    base = vec3_802ee3a0;
    switch (*(u8*)((s32)part + 0x214)) {
        case 0:
            *(u8*)((s32)part + 0x214) = 1;
            switch (*(u8*)((s32)part + 0x215)) {
                case 4:
                    animPoseSetEffect(*(void**)((s32)part + 0x1C0), (s32)(base + 0x68), 1);
                    animPoseSetEffectAnim(*(void**)((s32)part + 0x1C0), (s32)(base + 0x70), 1);
                    *(u16*)((s32)part + 0x216) = sysMsec2Frame(0x898);
                    break;
                case 5:
                    animPoseSetEffect(*(void**)((s32)part + 0x1C0), (s32)(base + 0x68), 1);
                    animPoseSetEffectAnim(*(void**)((s32)part + 0x1C0), (s32)(base + 0x78), 1);
                    frame = sysMsec2Frame(0x898);
                    *(u16*)((s32)part + 0x216) = (irand(4) + 6) * frame;
                    break;
                case 6:
                    animPoseSetEffect(*(void**)((s32)part + 0x1C0), (s32)(base + 0x68), 1);
                    animPoseSetEffectAnim(*(void**)((s32)part + 0x1C0), (s32)(base + 0x80), 1);
                    *(u16*)((s32)part + 0x216) = (irand(4) + 7) * 0x16;
                    break;
            }
            break;
        case 1:
            switch (*(u8*)((s32)part + 0x215)) {
                case 4:
                    *(u16*)((s32)part + 0x216) = *(u16*)((s32)part + 0x216) - 1;
                    if (*(u16*)((s32)part + 0x216) == 0) {
                        animPoseSetEffectAnim(*(void**)((s32)part + 0x1C0), (s32)(base + 0x70), 1);
                        *(u16*)((s32)part + 0x216) = sysMsec2Frame(0x898);
                    }
                    break;
                case 5:
                    *(u16*)((s32)part + 0x216) = *(u16*)((s32)part + 0x216) - 1;
                    if (*(u16*)((s32)part + 0x216) == 0) {
                        *(u8*)((s32)part + 0x214) = 2;
                        animPoseSetEffectAnim(*(void**)((s32)part + 0x1C0), (s32)(base + 0x88), 1);
                        *(u16*)((s32)part + 0x216) = 0x16;
                    }
                    break;
                case 6:
                    *(u16*)((s32)part + 0x216) = *(u16*)((s32)part + 0x216) - 1;
                    if (*(u16*)((s32)part + 0x216) == 0) {
                        *(u8*)((s32)part + 0x214) = 2;
                        animPoseSetEffectAnim(*(void**)((s32)part + 0x1C0), (s32)(base + 0x90), 1);
                        *(u16*)((s32)part + 0x216) = 0x16;
                    }
                    break;
            }
            break;
        case 2:
            switch (*(u8*)((s32)part + 0x215)) {
                case 4:
                case 5:
                case 6:
                    *(u16*)((s32)part + 0x216) = *(u16*)((s32)part + 0x216) - 1;
                    if (*(u16*)((s32)part + 0x216) == 0) {
                        *(u8*)((s32)part + 0x214) = 0;
                        _pose_two_pattern(part);
                    }
                    break;
            }
            break;
    }
}

void btlDispAnimeSpeed(void* part, f32 speed) {
    *(f32*)((s32)part + 0x208) = speed;
    animPoseSetLocalTimeRate(*(void**)((s32)part + 0x1C0),
                             *(f32*)((s32)part + 0x208) * *(f32*)((s32)part + 0x20C));
}

void btlGetScreenPoint(void* inPos, void* outScreenSpacePos) {
    extern f32 float_240_80422248;
    extern f32 float_304_80422244;
    extern void* camGetPtr(s32 id);
    extern void PSMTXMultVec(void* mtx, void* src, void* dst);
    extern void PSMTX44MultVec(void* mtx, void* src, void* dst);
    void* cam;
    f32 pos[3];
    f32 xScale;
    f32 yScale;
    f32 zero;
    f32 x;
    f32 y;

    cam = camGetPtr(4);
    pos[0] = *(f32*)((s32)inPos + 0);
    pos[1] = *(f32*)((s32)inPos + 4);
    pos[2] = *(f32*)((s32)inPos + 8);
    PSMTXMultVec((void*)((s32)cam + 0x11C), pos, pos);
    PSMTX44MultVec((void*)((s32)cam + 0x15C), pos, pos);

    xScale = float_304_80422244;
    x = pos[0];
    yScale = float_240_80422248;
    xScale = xScale * x;
    zero = float_0_80422240;
    *(f32*)((s32)outScreenSpacePos + 0) = xScale;
    y = pos[1];
    y = yScale * y;
    *(f32*)((s32)outScreenSpacePos + 4) = y;
    *(f32*)((s32)outScreenSpacePos + 8) = zero;
}

void btlDispTex4(s32 texId, f32* trans, f32* scale, f32* rot, u32* color) {
    typedef struct VecBits {
        u32 x;
        u32 y;
        u32 z;
    } VecBits;
    typedef union DoubleWords {
        f64 value;
        struct {
            u32 hi;
            u32 lo;
        } words;
    } DoubleWords;

    extern const VecBits vec3_802ee3a0[];

    register const VecBits* catalog = vec3_802ee3a0;

    u16 width;
    u16 height;

    u32 color0;
    u32 color1;
    u32 color2;
    u32 color3;

    VecBits offset;

    VecBits base0;
    VecBits size0;
    VecBits rot0;
    VecBits off0;
    VecBits one0;

    VecBits base1;
    VecBits size1;
    VecBits rot1;
    VecBits off1;
    VecBits one1;

    VecBits base2;
    VecBits size2;
    VecBits rot2;
    VecBits off2;
    VecBits one2;

    VecBits base3;
    VecBits size3;
    VecBits rot3;
    VecBits off3;
    VecBits one3;

    DoubleWords widthConv;
    DoubleWords heightConv;

    f32 halfW;
    f32 halfH;
    f32 negW;

    /*
     * These sixteen scalar temporaries mirror the target's call setup:
     * each call loads color + four Vec3 values completely before any of the
     * stack destinations are written.  The target can therefore keep twelve
     * of them in r14-r25 and use r0/r10-r12 for the remaining four.
     */
    register u32 v0;
    register u32 v1;
    register u32 v2;
    register u32 v3;
    register u32 v4;
    register u32 v5;
    register u32 v6;
    register u32 v7;
    register u32 v8;
    register u32 v9;
    register u32 v10;
    register u32 v11;
    register u32 v12;
    register u32 v13;
    register u32 v14;
    register u32 v15;

    /*
     * Target initializes the persistent offset Z before the texture-size
     * call, then reuses that scratch Vec throughout all four call blocks.
     */
    *(f32*)&offset.z = float_0_80422240;

    btlDispGetTexSize(texId, &width, &height);

    /*
     * Reconstruct MWCC's unsigned-u16 conversion scratch explicitly.  The
     * target uses the conversion double at vec3_802ee3a0 + 0x60, allowing
     * the same rodata base register to remain live across the function.
     */
    widthConv.words.hi = 0x43300000;
    widthConv.words.lo = width;
    heightConv.words.hi = 0x43300000;
    heightConv.words.lo = height;

    halfW =
        float_0p5_80422234 *
        (f32)(widthConv.value -
              *(const f64*)((const u8*)catalog + 0x60)) *
        scale[0];
    halfH =
        float_0p5_80422234 *
        (f32)(heightConv.value -
              *(const f64*)((const u8*)catalog + 0x60)) *
        scale[1];
    negW = -halfW;

    *(f32*)&offset.x = negW;
    *(f32*)&offset.y = halfH;

    v0 = *color;
    v1 = catalog[1].x;
    v2 = catalog[1].y;
    v3 = catalog[1].z;
    v4 = offset.x;
    v5 = offset.y;
    v6 = offset.z;
    v7 = ((VecBits*)rot)->x;
    v8 = ((VecBits*)rot)->y;
    v9 = ((VecBits*)rot)->z;
    v10 = ((VecBits*)scale)->x;
    v11 = ((VecBits*)scale)->y;
    v12 = ((VecBits*)scale)->z;
    v13 = ((VecBits*)trans)->x;
    v14 = ((VecBits*)trans)->y;
    v15 = ((VecBits*)trans)->z;

    color0 = v0;
    one0.x = v1;
    one0.y = v2;
    one0.z = v3;
    off0.x = v4;
    off0.y = v5;
    off0.z = v6;
    rot0.x = v7;
    rot0.y = v8;
    rot0.z = v9;
    size0.x = v10;
    size0.y = v11;
    size0.z = v12;
    base0.x = v13;
    base0.y = v14;
    base0.z = v15;

    _btlDispTex4(
        texId,
        (f32*)&base0,
        (f32*)&size0,
        (f32*)&rot0,
        (f32*)&off0,
        (f32*)&one0,
        &color0);

    *(f32*)&offset.x = halfW;

    v0 = *color;
    v1 = catalog[2].x;
    v2 = catalog[2].y;
    v3 = catalog[2].z;
    v4 = offset.x;
    v5 = offset.y;
    v6 = offset.z;
    v7 = ((VecBits*)rot)->x;
    v8 = ((VecBits*)rot)->y;
    v9 = ((VecBits*)rot)->z;
    v10 = ((VecBits*)scale)->x;
    v11 = ((VecBits*)scale)->y;
    v12 = ((VecBits*)scale)->z;
    v13 = ((VecBits*)trans)->x;
    v14 = ((VecBits*)trans)->y;
    v15 = ((VecBits*)trans)->z;

    color1 = v0;
    one1.x = v1;
    one1.y = v2;
    one1.z = v3;
    off1.x = v4;
    off1.y = v5;
    off1.z = v6;
    rot1.x = v7;
    rot1.y = v8;
    rot1.z = v9;
    size1.x = v10;
    size1.y = v11;
    size1.z = v12;
    base1.x = v13;
    base1.y = v14;
    base1.z = v15;

    _btlDispTex4(
        texId,
        (f32*)&base1,
        (f32*)&size1,
        (f32*)&rot1,
        (f32*)&off1,
        (f32*)&one1,
        &color1);

    halfH = -halfH;
    *(f32*)&offset.y = halfH;
    *(f32*)&offset.x = halfW;

    v0 = *color;
    v1 = catalog[3].x;
    v2 = catalog[3].y;
    v3 = catalog[3].z;
    v4 = offset.x;
    v5 = offset.y;
    v6 = offset.z;
    v7 = ((VecBits*)rot)->x;
    v8 = ((VecBits*)rot)->y;
    v9 = ((VecBits*)rot)->z;
    v10 = ((VecBits*)scale)->x;
    v11 = ((VecBits*)scale)->y;
    v12 = ((VecBits*)scale)->z;
    v13 = ((VecBits*)trans)->x;
    v14 = ((VecBits*)trans)->y;
    v15 = ((VecBits*)trans)->z;

    color2 = v0;
    one2.x = v1;
    one2.y = v2;
    one2.z = v3;
    off2.x = v4;
    off2.y = v5;
    off2.z = v6;
    rot2.x = v7;
    rot2.y = v8;
    rot2.z = v9;
    size2.x = v10;
    size2.y = v11;
    size2.z = v12;
    base2.x = v13;
    base2.y = v14;
    base2.z = v15;

    _btlDispTex4(
        texId,
        (f32*)&base2,
        (f32*)&size2,
        (f32*)&rot2,
        (f32*)&off2,
        (f32*)&one2,
        &color2);

    *(f32*)&offset.x = negW;

    v0 = *color;
    v1 = catalog[4].x;
    v2 = catalog[4].y;
    v3 = catalog[4].z;
    v4 = offset.x;
    v5 = offset.y;
    v6 = offset.z;
    v7 = ((VecBits*)rot)->x;
    v8 = ((VecBits*)rot)->y;
    v9 = ((VecBits*)rot)->z;
    v10 = ((VecBits*)scale)->x;
    v11 = ((VecBits*)scale)->y;
    v12 = ((VecBits*)scale)->z;
    v13 = ((VecBits*)trans)->x;
    v14 = ((VecBits*)trans)->y;
    v15 = ((VecBits*)trans)->z;

    color3 = v0;
    one3.x = v1;
    one3.y = v2;
    one3.z = v3;
    off3.x = v4;
    off3.y = v5;
    off3.z = v6;
    rot3.x = v7;
    rot3.y = v8;
    rot3.z = v9;
    size3.x = v10;
    size3.y = v11;
    size3.z = v12;
    base3.x = v13;
    base3.y = v14;
    base3.z = v15;

    _btlDispTex4(
        texId,
        (f32*)&base3,
        (f32*)&size3,
        (f32*)&rot3,
        (f32*)&off3,
        (f32*)&one3,
        &color3);
}

void _btlDispTex4(s32 texId, f32* trans0, f32* scale0, f32* rot, f32* trans1, f32* scale1, void* color) {
    extern void PSMTXIdentity(void* mtx);
    extern void PSMTXTrans(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXScale(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXRotRad(void* mtx, s32 axis, f32 radians);
    extern void btlDispTexPlane2(void* mtx, s32 texId, void* color, s32 flags);
    f32 mtx[3][4];
    f32 trans0Mtx[3][4];
    f32 scale0Mtx[3][4];
    f32 rotXMtx[3][4];
    f32 rotYMtx[3][4];
    f32 rotZMtx[3][4];
    f32 trans1Mtx[3][4];
    f32 scale1Mtx[3][4];
    u32 colorCopy;

    PSMTXIdentity(mtx);
    PSMTXTrans(trans0Mtx, trans0[0], trans0[1], trans0[2]);
    PSMTXTrans(trans1Mtx, trans1[0], trans1[1], trans1[2]);
    PSMTXScale(scale0Mtx, scale0[0], scale0[1], scale0[2]);
    PSMTXScale(scale1Mtx, scale1[0], scale1[1], scale1[2]);
    PSMTXRotRad(rotXMtx, 0x78, float_deg2rad_8042223c * rot[0]);
    PSMTXRotRad(rotYMtx, 0x79, float_deg2rad_8042223c * rot[1]);
    PSMTXRotRad(rotZMtx, 0x7A, float_deg2rad_8042223c * rot[2]);
    PSMTXConcat(scale0Mtx, mtx, mtx);
    PSMTXConcat(scale1Mtx, mtx, mtx);
    PSMTXConcat(trans1Mtx, mtx, mtx);
    PSMTXConcat(rotZMtx, mtx, mtx);
    PSMTXConcat(rotXMtx, mtx, mtx);
    PSMTXConcat(rotYMtx, mtx, mtx);
    PSMTXConcat(trans0Mtx, mtx, mtx);
    colorCopy = *(u32*)color;
    btlDispTexPlane2(mtx, texId, &colorCopy, 0);
}

void BattleStockExpDisp(void) {
    void* work = _battleWorkPointer;
    if (*(s32*)((s32)work + 0xF00) > *(s32*)((s32)work + 0xF04)) {
        *(s32*)((s32)work + 0xF08) += 1;
        if (*(s32*)((s32)work + 0xF08) >= 4) {
            *(s32*)((s32)work + 0xF04) += 1;
        }
    }
    if ((*(u32*)((s32)work + 0xEF8) & 2) != 0) {
        dispEntry(8, 1, _btlStockExpDisp, float_901_80422238, 0);
    }
}

void _btlStockExpDisp(void) {
    extern f32 float_13_80422218;
    extern f32 float_308_80422210;
    extern f32 float_3p5_80422214;
    extern f32 float_220_8042221c;
    extern f32 float_0p35_80422220;
    extern f32 float_20_8042222c;
    extern f32 float_10_80422228;
    extern f32 float_16_80422230;
    extern f32 float_0p5_80422234;
    extern f32 float_4_80422224;
    extern f32 vec3_802ee3dc[3];
    extern f32 vec3_802ee3e8[3];
    extern void iconDispGx(f64 scale, f32* pos, u16 flags, u16 iconId);
    s32 value;
    s32 tens;
    s32 ones;
    s32 i;
    Vec pos;
    static f32 x;
    static f32 y;
    f32 c13;
    f32 c308;
    f32 c3p5;
    f32 c220;
    f32 c4;
    f32 c20;
    f32 c10;
    f32 c16;

    value = *(s32*)((s32)_battleWorkPointer + 0xF04);
    if (value > 0) {
        if (value > 100) {
            value = 100;
        }
        tens = value / 10;
        ones = value - tens * 10;
        c13 = float_13_80422218;
        c308 = float_308_80422210;
        c3p5 = float_3p5_80422214;
        c220 = float_220_8042221c;
        for (i = 0; i < ones; i++) {
            pos = *(Vec*)vec3_802ee3dc;
            pos.x = (c308 + x - c3p5) - c13 * (f32)i;
            pos.y = y - c220;
            iconDispGx(float_0p35_80422220, (f32*)&pos, 0x10, 0x194);
        }
        c4 = float_4_80422224;
        c20 = float_20_8042222c;
        c10 = float_10_80422228;
        c16 = float_16_80422230;
        for (i = 0; i < tens; i++) {
            pos = *(Vec*)vec3_802ee3e8;
            pos.x = c4 + (c308 + x - c10) - c20 * (f32)i;
            pos.y = c16 + (y - c220);
            iconDispGx(float_0p5_80422234, (f32*)&pos, 0x10, 0x194);
        }
    }
}

const u32 vec3_802ee3a0[3] = {
    0x00000000, 0x00000000, 0x00000000,
};

const u32 vec3_802ee3ac[3] = {
    0x3F800000, 0x3F800000, 0x3F800000,
};

const u32 vec3_802ee3b8[3] = {
    0xBF800000, 0x3F800000, 0x3F800000,
};

const u32 vec3_802ee3c4[3] = {
    0xBF800000, 0xBF800000, 0x3F800000,
};

const u32 vec3_802ee3d0[3] = {
    0x3F800000, 0xBF800000, 0x3F800000,
};

const u32 vec3_802ee3dc[3] = {
    0x00000000, 0x00000000, 0xC2C80000,
};

const u32 vec3_802ee3e8[3] = {
    0x00000000, 0x00000000, 0xC2C80000,
};

const char str_p_b_st_802ee408[] = "p_b_st";
const char str_PM_Y_1B_802ee410[] = "PM_Y_1B";
const char str_PM_C_1A_802ee418[] = "PM_C_1A";
const char str_PM_F_2_802ee420[] = "PM_F_2";
const char str_PM_C_1B_802ee428[] = "PM_C_1B";
const char str_PM_F_1B_802ee430[] = "PM_F_1B";
const char str_PM_F_1A_802ee438[] = "PM_F_1A";
const char str_PM_Y_1A_802ee440[] = "PM_Y_1A";

s32 _status_pose_table[8] = {
    0x1B, 2, 9, 1, 4, 5, 3, -1,
};
