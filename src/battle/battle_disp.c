#include "battle/battle_disp.h"

extern void* _battleWorkPointer;
extern f32 float_901_80422238;
void _btlStockExpDisp(void);
void dispEntry(s32 cameraId, s32 priority, void* callback, f32 z, void* param);
extern f32 float_0_80422240;
extern f32 float_2_80422250;
extern f32 float_4_80422224;
extern f64 double_to_int_802ee3f8;
extern s32 _status_pose_table[8];
void animPoseSetLocalTimeRate(void* pose, f32 rate);
void animPoseSetEffectAnim(void* pose, s32 value, s32 flag);
void animPoseSetEffect(void* pose, s32 value, s32 flag);
void animPoseSetMaterialLightFlagOn(void* pose, s32 flag);
void _partsBlurControl(void* part, s32 enable, s32 a3, void* color);
void _pose_one_pattern(void* part);
void _pose_two_pattern(void* part);
void battleCameraInit(void);
void battleMenuDispInit(void);
f32 sinfd(f64);
void TEXGetGXTexObjFromPalette(void* palette, void* texObj, s32 id);
u16 GXGetTexObjWidth(void* texObj);
u16 GXGetTexObjHeight(void* texObj);

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

void btlDispAnimeSpeed(void* part, f32 speed) {
    *(f32*)((s32)part + 0x208) = speed;
    animPoseSetLocalTimeRate(*(void**)((s32)part + 0x1C0),
                             *(f32*)((s32)part + 0x208) * *(f32*)((s32)part + 0x20C));
}

void btlDispPoseAnime(void* part) {
    *(u32*)((s32)part + 0x204) &= ~0x12;
    *(u32*)((s32)part + 0x204) |= 1;
    *(u32*)((s32)part + 0x204) &= ~0x80;
    *(u32*)((s32)part + 0x204) &= ~4;
}

void btlDispChangePeraAnime(void* part) {
    s32 animId;
    *(u32*)((s32)part + 0x204) |= 4;
    *(u8*)((s32)part + 0x215) = 0;
    animPoseSetEffectAnim(*(void**)((s32)part + 0x1C0), animId, 1);
}

void btlDispInit(void) {
    void* work = _battleWorkPointer;
    battleCameraInit();
    battleMenuDispInit();
    *(u32*)((s32)work + 0xEF8) |= 2;
}

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

void btlUnitPartsBlurControl(void* part, s32 a2, s32 a3, s32* color) {
    s32 local;
    if ((*(u32*)((s32)part + 0x1AC) & 0x04000000) != 0) {
        local = *color;
        _partsBlurControl(part, 1, a3, &local);
    } else {
        local = *color;
        _partsBlurControl(part, 0, a3, &local);
    }
}

void btlDispGetTexSize(s32 id, u16* width, u16* height) {
    u8 texObj[0x20];
    void* work = _battleWorkPointer;
    void* palette = *(void**)(*(s32*)((s32)work + 0x163F8) + 0xA0);
    TEXGetGXTexObjFromPalette(*(void**)palette, texObj, id);
    *width = GXGetTexObjWidth(texObj);
    *height = GXGetTexObjHeight(texObj);
}

f32 getFloatDispOffset(void* part) {
    s32 value = ((*(u16*)((s32)part + 0x1A8) << 2) + 0x10E) % 0x168;
    f32 sin = sinfd((f32)value);
    return float_2_80422250 + float_2_80422250 * sin;
}

f32 getGravityDispOffset(void* part) {
    s32 value = (*(u16*)((s32)part + 0x1A6) * 3 + 0x10E) % 0x168;
    f32 sin = sinfd((f32)value);
    return float_4_80422224 + float_4_80422224 * sin;
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


void btlUnitPartsDisp(s32 cameraId, void* part) {
    extern void animPoseSetLocalTimeRate(void*, f32);
    extern void animPoseSetEffect(void*, s32, s32);
    extern void animPoseSetEffectAnim(void*, s32, s32);
    extern void _partsBlurControl(void*, s32, s32, void*);
    s32 poseId;
    u32 flags;

    if (part == 0) {
        return;
    }
    poseId = *(s32*)((s32)part + 0x1D4);
    if (poseId < 0) {
        return;
    }
    flags = *(u32*)((s32)part + 0x204);
    if ((flags & 0x10) != 0) {
        animPoseSetLocalTimeRate((void*)poseId, 0.5f);
    } else {
        animPoseSetLocalTimeRate((void*)poseId, 1.0f);
    }
    if ((flags & 0x40) != 0) {
        animPoseSetEffect((void*)poseId, 1, 0);
    }
    if ((flags & 0x80) != 0) {
        animPoseSetEffectAnim((void*)poseId, 1, 0);
    }
    _partsBlurControl(part, (flags & 0x100) != 0, 0, (void*)((s32)part + 0x1F0));
}

void btlDispMain(void) {
    extern void* _battleWorkPointer;
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
    void* unit;
    s32 i;
    u8 shown;
    u8 target;

    for (i = 0; i < 64; i++) {
        unit = BattleGetUnitPtr(_battleWorkPointer, i);
        if (unit == 0) {
            continue;
        }
        shown = *(u8*)((s32)unit + 0x304);
        target = *(u8*)((s32)unit + 0x303);
        BtlUnit_OffUnitFlag(unit, 2);
        if (shown != target) {
            BtlUnit_OnUnitFlag(unit, 2);
            if (shown < target) {
                shown += 12;
                if (shown > target) shown = target;
            } else {
                shown -= 2;
                if (shown < target) shown = target;
            }
            *(u8*)((s32)unit + 0x304) = shown;
        }
        BtlUnit_HpGaugeMain(unit);
        *(f32*)((s32)unit + 0x34) = 1.0f;
        BattleStatusEffectMain(unit);
        BattleStatusIconMain(unit);
    }
    BattleStockExpDisp();
    BattleStageDisp();
    BattleCommandDisplay(_battleWorkPointer);
    BattleAudience_Disp();
    BattleBreakSlot_Disp();
}

void btlDispTex4(s32 texId, f32* trans, f32* scale, f32* rot, u32* color) {
    extern void btlDispGetTexSize(s32 texId, u16* width, u16* height);
    extern void _btlDispTex4(s32 texId, f32* trans0, f32* scale0, f32* rot, f32* trans1, f32* scale1, void* color);
    extern f32 float_0_80422240;
    extern f32 float_0p5_80422234;
    extern u32 vec3_802ee3a0[];
    u16 height;
    u16 width;
    u32 color0;
    u32 color1;
    u32 color2;
    u32 color3;
    u32 base0[3];
    u32 size0[3];
    u32 rot0[3];
    u32 off0[3];
    u32 one0[3];
    u32 base1[3];
    u32 size1[3];
    u32 rot1[3];
    u32 off1[3];
    u32 one1[3];
    u32 base2[3];
    u32 size2[3];
    u32 rot2[3];
    u32 off2[3];
    u32 one2[3];
    u32 base3[3];
    u32 size3[3];
    u32 rot3[3];
    u32 off3[3];
    u32 one3[3];
    f32 zero = float_0_80422240;
    f32 halfW;
    f32 halfH;
    f32 negW;
    f32 negH;

    btlDispGetTexSize(texId, &width, &height);
    color0 = *color;
    halfW = float_0p5_80422234 * (f32)width * scale[0];
    halfH = float_0p5_80422234 * (f32)height * scale[1];
    negW = -halfW;
    negH = -halfH;

    base0[0] = *(u32*)&trans[0];
    base0[1] = *(u32*)&trans[1];
    base0[2] = *(u32*)&trans[2];
    size0[0] = *(u32*)&scale[0];
    size0[1] = *(u32*)&scale[1];
    size0[2] = *(u32*)&scale[2];
    rot0[0] = *(u32*)&rot[0];
    rot0[1] = *(u32*)&rot[1];
    rot0[2] = *(u32*)&rot[2];
    *(f32*)&off0[0] = negW;
    *(f32*)&off0[1] = halfH;
    *(f32*)&off0[2] = zero;
    one0[0] = vec3_802ee3a0[3];
    one0[1] = vec3_802ee3a0[4];
    one0[2] = vec3_802ee3a0[5];
    _btlDispTex4(texId, (f32*)base0, (f32*)size0, (f32*)rot0, (f32*)off0, (f32*)one0, &color0);

    color1 = *color;
    base1[0] = *(u32*)&trans[0];
    base1[1] = *(u32*)&trans[1];
    base1[2] = *(u32*)&trans[2];
    size1[0] = *(u32*)&scale[0];
    size1[1] = *(u32*)&scale[1];
    size1[2] = *(u32*)&scale[2];
    rot1[0] = *(u32*)&rot[0];
    rot1[1] = *(u32*)&rot[1];
    rot1[2] = *(u32*)&rot[2];
    *(f32*)&off1[0] = halfW;
    *(f32*)&off1[1] = halfH;
    *(f32*)&off1[2] = zero;
    one1[0] = vec3_802ee3a0[6];
    one1[1] = vec3_802ee3a0[7];
    one1[2] = vec3_802ee3a0[8];
    _btlDispTex4(texId, (f32*)base1, (f32*)size1, (f32*)rot1, (f32*)off1, (f32*)one1, &color1);

    color2 = *color;
    base2[0] = *(u32*)&trans[0];
    base2[1] = *(u32*)&trans[1];
    base2[2] = *(u32*)&trans[2];
    size2[0] = *(u32*)&scale[0];
    size2[1] = *(u32*)&scale[1];
    size2[2] = *(u32*)&scale[2];
    rot2[0] = *(u32*)&rot[0];
    rot2[1] = *(u32*)&rot[1];
    rot2[2] = *(u32*)&rot[2];
    *(f32*)&off2[0] = halfW;
    *(f32*)&off2[1] = negH;
    *(f32*)&off2[2] = zero;
    one2[0] = vec3_802ee3a0[9];
    one2[1] = vec3_802ee3a0[10];
    one2[2] = vec3_802ee3a0[11];
    _btlDispTex4(texId, (f32*)base2, (f32*)size2, (f32*)rot2, (f32*)off2, (f32*)one2, &color2);

    color3 = *color;
    base3[0] = *(u32*)&trans[0];
    base3[1] = *(u32*)&trans[1];
    base3[2] = *(u32*)&trans[2];
    size3[0] = *(u32*)&scale[0];
    size3[1] = *(u32*)&scale[1];
    size3[2] = *(u32*)&scale[2];
    rot3[0] = *(u32*)&rot[0];
    rot3[1] = *(u32*)&rot[1];
    rot3[2] = *(u32*)&rot[2];
    *(f32*)&off3[0] = negW;
    *(f32*)&off3[1] = negH;
    *(f32*)&off3[2] = zero;
    one3[0] = vec3_802ee3a0[12];
    one3[1] = vec3_802ee3a0[13];
    one3[2] = vec3_802ee3a0[14];
    _btlDispTex4(texId, (f32*)base3, (f32*)size3, (f32*)rot3, (f32*)off3, (f32*)one3, &color3);
}

u8 btlDispTexPlainGX(void) {
    return 0;
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

void _btlStockExpDisp(void) {
    ;
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

void _btlDispTex4(s32 texId, f32* trans0, f32* scale0, f32* rot, f32* trans1, f32* scale1, void* color) {
    extern f32 float_deg2rad_8042223c;
    extern void PSMTXIdentity(void* mtx);
    extern void PSMTXTrans(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXScale(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXRotRad(void* mtx, s32 axis, f32 radians);
    extern void PSMTXConcat(void* a, void* b, void* ab);
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

void btlDispGXInit2D(void) {
    extern void btlDispGXInit2DSub(void);
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

void btlDispGXQuads2D(f32 x1, f32 y1, f32 x2, f32 y2, u8 r, u8 g, u8 b, u8 a) {
    extern f32 float_0_80422240;
    extern f32 float_1_8042224c;
    extern void GXBegin(s32 primitive, s32 vtxFmt, s32 nVerts);
    extern void btlDispGXPoint2D(u8 r, u8 g, u8 b, u8 a, f32 x, f32 y, f32 s, f32 t);

    GXBegin(0x80, 0, 4);
    btlDispGXPoint2D(r, g, b, a, x1, y1, float_0_80422240, float_0_80422240);
    btlDispGXPoint2D(r, g, b, a, x2, y1, float_1_8042224c, float_0_80422240);
    btlDispGXPoint2D(r, g, b, a, x2, y2, float_1_8042224c, float_1_8042224c);
    btlDispGXPoint2D(r, g, b, a, x1, y2, float_0_80422240, float_1_8042224c);
}

void btlDispTexPlane(s32 tpl, void* color, s32 flags, f32 x, f32 y, f32 z, f32 scaleX, f32 scaleY) {
    extern f32 float_1_8042224c;
    extern void btlDispTexPlaneInit(void);
    extern void* camGetCurPtr(void);
    extern void PSMTXScale(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXTrans(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXConcat(void* a, void* b, void* ab);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
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

void btlUnitItemDisp(s32 param_1, void* unit) {
    ;
}


void btlDispGXInit2DRasta(void) {
    extern void btlDispGXInit2DSub(void);
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

void btlDispGXQuads2DRasta(f32 x1, f32 y1, f32 x2, f32 y2, u8 r, u8 g, u8 b, u8 a) {
    extern void GXBegin(s32 primitive, s32 vtxFmt, s32 nVerts);
    extern void btlDispGXPoint2DRasta(u8 r, u8 g, u8 b, u8 a, f32 x, f32 y);

    GXBegin(0x80, 0, 4);
    btlDispGXPoint2DRasta(r, g, b, a, x1, y1);
    btlDispGXPoint2DRasta(r, g, b, a, x2, y1);
    btlDispGXPoint2DRasta(r, g, b, a, x2, y2);
    btlDispGXPoint2DRasta(r, g, b, a, x1, y2);
}

void _partsBlurControl(void* part, s32 enable, s32 a3, void* color) {
    ;
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

void btlUnitStolenItemDisp(s32 param_1, void* unit) {
    ;
}


void btlDispTexPlane3(void* mtx, s32 texId, void* color0, void* color1, void* color2, void* color3) {
    extern void btlDispTexPlaneInit(void);
    extern void PSMTXCopy(void* src, void* dst);
    extern void* camGetCurPtr(void);
    extern void PSMTXConcat(void* a, void* b, void* ab);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
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

void btlGetScreenPoint(void* inPos, void* outScreenSpacePos) {
    extern f32 float_0_80422240;
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

void btlDispEntAnime(void* unit) {
    extern s32 animPoseEntry(void*, s32);
    extern char* strcpy(char*, const char*);
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

void btlDispTexPlane2(void* mtx, s32 texId, void* color) {
    extern void btlDispTexPlaneInit(void);
    extern void PSMTXCopy(void* src, void* dst);
    extern void* camGetCurPtr(void);
    extern void PSMTXConcat(void* a, void* b, void* ab);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
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

void btlDispChangeAnime(BattleWorkUnitPart* part, const char*name, BOOL a3) {
    extern char* strcpy(char*, const char*);

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

