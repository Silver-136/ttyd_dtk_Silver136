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


void btlUnitPartsDisp(s32 param_1, void* part) {
}


void btlDispMain(void) {
}


u8 btlDispTex4(void) {
    return 0;
}


u8 btlDispTexPlainGX(void) {
    return 0;
}


void _pose_two_pattern(void* part) {
}


void _btlStockExpDisp(void) {
}


void btlUnitPartsBlurDisp(s32 param_1, void* part) {
}


u8 _btlDispTex4(s32 param_1, float* param_2, float* param_3, float* param_4, float* param_5, float* param_6, void* param_7) {
    return 0;
}


u8 btlDispGXInit2D(void) {
    return 0;
}


u8 btlDispGXQuads2D(void) {
    return 0;
}


void btlDispTexPlane(s32 tpl, void* color, s32 flags, f32 x, f32 y, f32 z, f32 rx, f32 scale) {
}


void btlUnitItemDisp(s32 param_1, void* unit) {
}


u8 btlDispGXInit2DRasta(void) {
    return 0;
}


u8 btlDispTexPlaneInit(void) {
    return 0;
}


void _pose_one_pattern(void* part) {
}


u8 btlDispGXQuads2DRasta(double x1, double y1, double x2, double y2, s32 r, s32 g, u8 b, u8 a) {
    return 0;
}


void _partsBlurControl(void* part, s32 enable, s32 a3, void* color) {
}


u8 floatOffsetControl(void* part) {
    return 0;
}


void btlUnitStolenItemDisp(s32 param_1, void* unit) {
}


u8 btlDispTexPlane3(s32 param_1, s32 param_2, void* param_3, void* param_4, void* param_5, void* param_6) {
    return 0;
}


void btlGetScreenPoint(void* inPos, void* outScreenSpacePos) {
}


u8 gravityOffsetControl(void* part) {
    return 0;
}


u8 btlDispGXInit2DSub(void) {
    return 0;
}


u8 btlDispEntAnime(void* unit) {
    return 0;
}


void btlUnitItemDisp2(s32 param_1, void* unit) {
}


u8 btlDispTexPlane2(s32 param_1, s32 param_2, void* param_3) {
    return 0;
}


void btlDispChangeAnime(BattleWorkUnitPart* part, const char*name, BOOL a3) {
}
