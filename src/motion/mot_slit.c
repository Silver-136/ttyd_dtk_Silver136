#include "motion/mot_slit.h"

void* marioGetPtr(void);
void marioChgMot2(s32 motionId);

typedef struct SlitVecCopy {
    s32 x;
    s32 y;
    s32 z;
} SlitVecCopy;

void unk_800a6ac8(void) {
    void* mario = marioGetPtr();

    marioChgMot2(0x15);
    *(s16*)((s32)mario + 0x2F0) = 0xA;
    *(u32*)mario &= ~0xF0000;
}

void N_marioSlitSetAfterLand(void) {
    void* mario = marioGetPtr();
    marioChgMot2(0x15);
    *(u32*)mario &= ~0xF0000;
}

u32 marioSlitAbilityChk(void) {
    extern void* marioGetPtr(void);
    extern s32 pouchCheckItem(s32 item);
    extern s32 marioBgmodeChk(void);
    void* mario = marioGetPtr();
    s32 mode;

    if (*(u32*)mario & 0x800000) {
        return 0;
    }
    if (pouchCheckItem(2) == 0) {
        return 0;
    }
    if (*(u32*)mario & 0x80000000) {
        return 0;
    }
    mode = marioBgmodeChk();
    return (u32)((1 - mode) | (mode - 1)) >> 31;
}

s32 marioSlitButton(void) {
    extern void* marioGetPtr(void);
    extern s32 marioKeyOffChk(void);
    extern s32 marioBgmodeChk(void);
    void* mario = marioGetPtr();
    if (*(u32*)((s32)mario + 0xC) & 2) {
        return 0;
    }
    if (marioKeyOffChk() != 0) {
        return 0;
    }
    if (marioBgmodeChk() == 1) {
        return 0;
    }
    if (!(*(u32*)((s32)mario + 0x4) & 0x1000000)) {
        if (*(u16*)((s32)mario + 0x24A) & 0x20) {
            return 1;
        }
    } else {
        if (*(u16*)((s32)mario + 0x24A) & 0x20) {
            return 2;
        }
        if (*(u8*)((s32)mario + 0x257) >= 0x32) {
            return 2;
        }
    }
    return 0;
}

s32 marioSlitKeyDisable(void) {
    void* mario = marioGetPtr();
    if ((*(u32*)mario & 0x100000) == 0) {
        return 1;
    }
    {
        s32 timer = *(s16*)((s32)mario + 0x2F0);
        if (timer >= 0x50 && timer <= 0x52) {
            return 0;
        }
        if (timer >= 0x5A && timer <= 0x5E) {
            return 0;
        }
    }
    return 1;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
u32 U_marioSlitContinueChk(void) {
    extern void* marioGetPtr(void);
    extern s32 marioKeyOffChk(void);
    extern s32 marioBgmodeChk(void);
    extern s32 marioSlitChkWallAround(void);
    extern s32 marioCreviceWallChk(void* pos);
    SlitVecCopy pos;
    void* mario = marioGetPtr();
    void* mario2 = marioGetPtr();
    s32 button;

    if (*(u32*)((s32)mario2 + 0xC) & 2) {
        button = 0;
    } else if (marioKeyOffChk() != 0) {
        button = 0;
    } else if (marioBgmodeChk() == 1) {
        button = 0;
    } else if (!(*(u32*)((s32)mario2 + 0x4) & 0x1000000)) {
        button = (*(u16*)((s32)mario2 + 0x24A) & 0x20) ? 1 : 0;
    } else if (*(u16*)((s32)mario2 + 0x24A) & 0x20) {
        button = 2;
    } else if (*(u8*)((s32)mario2 + 0x257) >= 0x32) {
        button = 2;
    } else {
        button = 0;
    }
    if (button != 0) {
        return 1;
    }
    if (marioSlitChkWallAround() != 0) {
        return 1;
    }
    pos.x = *(s32*)((s32)mario + 0x8C);
    pos.y = *(s32*)((s32)mario + 0x90);
    pos.z = *(s32*)((s32)mario + 0x94);
    button = marioCreviceWallChk(&pos);
    return ((u32)(-button) | (u32)button) >> 31;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

s32 marioCreviceWallChk(f32* pos) {
    extern void* marioGetPtr(void);
    extern f32 toMovedirSimple(f32);
    extern void sincosf(f32, f32*, f32*);
    extern s32 marioHitCheckVec(void*, void*, f32*, void*, f32*);
    extern f64 angleABf(f64, f64, f64, f64);
    extern f32 __fabsf(f32);
    extern f32 float_0_80420f9c;
    extern f32 float_0p5_80420fd8;
    extern f32 float_10_80421000;
    extern f32 float_20_80420ff4;
    extern f32 float_180_80420fa8;
    extern f32 float_270_80420fb8;

    typedef struct Vec { f32 x, y, z; } Vec;

    Vec checkPos;
    Vec dir;
    f32 outA[3];
    f32 outB[3];
    f32 radius;
    f32 sx;
    f32 sz;
    f32 offX;
    f32 offZ;
    f32 angA;
    f32 angB;
    s32 hitA;
    s32 hitB;

    marioGetPtr();
    sincosf(toMovedirSimple(float_270_80420fb8), &sx, &sz);
    offX = float_0p5_80420fd8 * float_20_80420ff4 * sx;
    offZ = float_0p5_80420fd8 * float_20_80420ff4 * sz;

    sincosf(toMovedirSimple(float_180_80420fa8), &sx, &sz);
    dir.x = sx;
    dir.y = float_0_80420f9c;
    dir.z = sz;
    checkPos.x = pos[0];
    checkPos.y = pos[1] + float_10_80421000;
    checkPos.z = pos[2];
    radius = float_20_80420ff4;
    hitA = marioHitCheckVec(&checkPos, &dir, outA, outB, &radius);
    angA = float_0_80420f9c;
    if (hitA != 0) {
        angA = (f32)angleABf(float_0_80420f9c, float_0_80420f9c, outB[0], outB[2]);
    } else {
        checkPos.x = pos[0] + offX;
        checkPos.y = pos[1] + float_10_80421000;
        checkPos.z = pos[2] + offZ;
        radius = float_20_80420ff4;
        hitA = marioHitCheckVec(&checkPos, &dir, outA, outB, &radius);
        if (hitA != 0) {
            angA = (f32)angleABf(float_0_80420f9c, float_0_80420f9c, outB[0], outB[2]);
        }
    }
    if (hitA == 0) {
        checkPos.x = pos[0] - offX;
        checkPos.y = pos[1] + float_10_80421000;
        checkPos.z = pos[2] - offZ;
        radius = float_20_80420ff4;
        hitA = marioHitCheckVec(&checkPos, &dir, outA, outB, &radius);
        if (hitA != 0) {
            angA = (f32)angleABf(float_0_80420f9c, float_0_80420f9c, outB[0], outB[2]);
        }
    }

    sincosf(toMovedirSimple(float_0_80420f9c), &sx, &sz);
    dir.x = sx;
    dir.y = float_0_80420f9c;
    dir.z = sz;
    checkPos.x = pos[0];
    checkPos.y = pos[1] + float_10_80421000;
    checkPos.z = pos[2];
    radius = float_20_80420ff4;
    hitB = marioHitCheckVec(&checkPos, &dir, outA, outB, &radius);
    angB = float_0_80420f9c;
    if (hitB != 0) {
        angB = (f32)angleABf(float_0_80420f9c, float_0_80420f9c, outB[0], outB[2]);
    } else {
        checkPos.x = pos[0] + offX;
        checkPos.y = pos[1] + float_10_80421000;
        checkPos.z = pos[2] + offZ;
        radius = float_20_80420ff4;
        hitB = marioHitCheckVec(&checkPos, &dir, outA, outB, &radius);
        if (hitB != 0) {
            angB = (f32)angleABf(float_0_80420f9c, float_0_80420f9c, outB[0], outB[2]);
        }
    }
    if (hitB == 0) {
        checkPos.x = pos[0] - offX;
        checkPos.y = pos[1] + float_10_80421000;
        checkPos.z = pos[2] - offZ;
        radius = float_20_80420ff4;
        hitB = marioHitCheckVec(&checkPos, &dir, outA, outB, &radius);
        if (hitB != 0) {
            angB = (f32)angleABf(float_0_80420f9c, float_0_80420f9c, outB[0], outB[2]);
        }
    }

    if (hitA == 0 || hitB == 0 ||
        float_10_80421000 < __fabsf(-(float_0p5_80420fd8 * (angA + angB) - float_180_80420fa8))) {
        return 0;
    }
    return 1;
}

void marioClearSlitFloor(void) {
    void* mario = marioGetPtr();
    *(s32*)((s32)mario + 0x26C) = 0;
}

s32 marioGetSlitFloorPos(SlitVecCopy* out) {
    void* mario = marioGetPtr();
    if (*(s32*)((s32)mario + 0x26C) != 0) {
        s32 x = *(s32*)((s32)mario + 0x270);
        s32 y = *(s32*)((s32)mario + 0x274);
        s32 z;
        out->x = x;
        out->y = y;
        z = *(s32*)((s32)mario + 0x278);
        out->z = z;
        return 1;
    }
    return 0;
}

s32 marioChkSlitThrouh(void) {
    extern void* marioGetPtr(void);
    void* mario = marioGetPtr();
    s32 timer;

    if (*(u16*)((s32)mario + 0x2E) != 0x15) {
        return 0;
    }
    timer = *(s16*)((s32)mario + 0x2F0);
    if (timer >= 0x50 && timer <= 0x5E) {
        return 1;
    }
    return 0;
}

s32 marioChkSlitThrouhEnd(void) {
    extern void* marioGetPtr(void);
    void* mario = marioGetPtr();
    s32 timer;

    if (*(u16*)((s32)mario + 0x2E) != 0x15) {
        return 0;
    }
    timer = *(s16*)((s32)mario + 0x2F0);
    if (timer == 0x52 || timer == 0x5E) {
        return 1;
    }
    return 0;
}

void motSlitContinue(void) {
    s32 timer;
    marioChgMot2(0x15);
    timer = 0x14;
    *(s16*)((s32)marioGetPtr() + 0x2F0) = timer;
}

int marioChkSlitEnd(void) {
    extern void* marioGetPtr(void);
    void* mario = marioGetPtr();

    if (*(u16*)((s32)mario + 0x2E) != 0x15) {
        return 0;
    }
    return *(s16*)((s32)mario + 0x2F0) >= 0x64;
}

void motSlitCancel(void) {
    void* mario = marioGetPtr();
    if (*(u32*)mario & 0x100000) {
        *(s16*)((s32)mario + 0x2F0) = 0x64;
    }
}

u8 motSlitCancel2(void) {
    extern void* marioGetPtr(void);
    extern void allPartyForceSlitOff(void);
    void* mario = marioGetPtr();

    if (*(u32*)mario & 0x100000) {
        allPartyForceSlitOff();
        *(s16*)((s32)mario + 0x2F0) = 0x6E;
    }
}

void motSlitCancel3(void) {
    void* mario = marioGetPtr();
    if (*(u32*)mario & 0x100000) {
        *(s16*)((s32)mario + 0x2F0) = 0x78;
    }
}

s32 marioGetMotSlitCancel3(void) {
    register s32 ret = 0;
    void* mario = marioGetPtr();
    if (*(u16*)((s32)mario + 0x2E) == 0x15) {
        s32 timer = *(s16*)((s32)mario + 0x2F0);
        if ((timer >= 0x78 && timer <= 0x7A) || (timer >= 0x6E && timer <= 0x70)) {
            ret = 1;
        }
    }
    return ret;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

void marioSlitForceCancel(void) {
    extern void* marioGetPtr(void);
    extern void marioPaperOff(void);
    extern void mapResetPaperAmbColor(void);
    extern void marioAdjustMoveDir(void);
    extern void allPartyForceSlitOff(void);
    extern f32 float_180_80420fa8;
    extern f32 float_0_80420f9c;
    extern f32 float_10_80421000;
    extern f32 float_0p004_80420fbc;
    void* mario = marioGetPtr();
    u32 flags = *(u32*)mario;
    if (flags & 0x100000) {
        *(u32*)mario = flags & ~0x200000;
        if (*(f32*)((s32)mario + 0x1A4) >= float_180_80420fa8) {
            *(f32*)((s32)mario + 0x1AC) = float_0_80420f9c;
        } else {
            *(f32*)((s32)mario + 0x1AC) = float_180_80420fa8;
        }
        *(f32*)((s32)mario + 0x1B0) = *(f32*)((s32)mario + 0x1AC);
        *(u32*)((s32)mario + 0x4) &= 0xFEFFFFFE;
        *(u32*)mario &= ~0x400000;
        *(u32*)mario &= ~0x100000;
        *(f32*)((s32)mario + 0x1B8) = float_10_80421000;
        *(u32*)((s32)mario + 0x4) &= 0xFEFFFFFE;
        marioPaperOff();
        mapResetPaperAmbColor();
        *(u32*)((s32)mario + 0x4) &= ~0x4;
        marioAdjustMoveDir();
        mario = marioGetPtr();
        *(f32*)((s32)mario + 0x148) = float_0_80420f9c;
        *(f32*)((s32)mario + 0x158) = float_0p004_80420fbc;
        allPartyForceSlitOff();
    }
}

void mot_slit_post(void) {
    extern void* marioGetPtr(void);
    extern void marioPaperOff(void);
    extern void mapResetPaperAmbColor(void);
    extern void marioAdjustMoveDir(void);
    extern void allPartyForceSlitOff(void);
    extern void U_allPartySlitOffReq(void);
    extern f32 float_20_80420ff4;
    extern f32 float_180_80420fa8;
    extern f32 float_0_80420f9c;

    void* mario = marioGetPtr();
    u16 motion;

    motion = *(u16*)((s32)mario + 0x2E);
    if (*(s16*)((s32)mario + 0x2F0) >= 10 &&
        (motion == 0x15 || ((u16)(motion - 0x17) <= 1) || motion <= 0x14 || motion == 0x16)) {
        return;
    }
    if ((*(u32*)mario & 0x100000) == 0) {
        if ((*(u32*)((s32)mario + 4) & 0x1000000) == 0) {
            return;
        }
        *(f32*)((s32)mario + 0x1B4) = float_20_80420ff4;
        if (*(f32*)((s32)mario + 0x1A4) < float_180_80420fa8) {
            *(f32*)((s32)mario + 0x1AC) = float_180_80420fa8;
        } else {
            *(f32*)((s32)mario + 0x1AC) = float_0_80420f9c;
        }
        *(f32*)((s32)mario + 0x1B0) = *(f32*)((s32)mario + 0x1AC);
        *(u32*)mario &= ~0x200000;
        *(u32*)mario &= ~0x400000;
        *(u32*)((s32)mario + 4) &= 0xFEFFF3FF;
        *(u32*)((s32)mario + 4) &= 0xFEFFF3FF;
        marioPaperOff();
        mapResetPaperAmbColor();
        *(u32*)((s32)mario + 4) &= ~4;
        marioAdjustMoveDir();
        allPartyForceSlitOff();
    } else if (*(u16*)((s32)mario + 0x2E) != 2) {
        *(u32*)mario &= ~0x100000;
        *(f32*)((s32)mario + 0x1B4) = float_20_80420ff4;
        if (*(f32*)((s32)mario + 0x1A4) < float_180_80420fa8) {
            *(f32*)((s32)mario + 0x1AC) = float_180_80420fa8;
        } else {
            *(f32*)((s32)mario + 0x1AC) = float_0_80420f9c;
        }
        *(f32*)((s32)mario + 0x1B0) = *(f32*)((s32)mario + 0x1AC);
        *(u32*)mario &= ~0x200000;
        *(u32*)mario &= ~0x400000;
        *(u32*)((s32)mario + 4) &= 0xFEFFF3FF;
        *(u32*)((s32)mario + 4) &= 0xFEFFF3FF;
        marioPaperOff();
        mapResetPaperAmbColor();
        *(u32*)((s32)mario + 4) &= ~4;
        marioAdjustMoveDir();
        if (*(u16*)((s32)mario + 0x2E) == 0x16) {
            U_allPartySlitOffReq();
        } else {
            allPartyForceSlitOff();
        }
    }
}

u8 marioForceSlitAnime(void) {
    extern void* marioGetPtr(void);
    extern void marioPaperOff(void);
    extern void marioChgPaper(void* paper);
    extern void marioPaperOn(void* paper);
    extern void marioChgPose(void* pose);
    extern char str_p_slit_802c433c[];
    extern char str_M_S_1_80420fa0[6];
    extern char str_PM_S_1A_802c4344[];
    void* mario = marioGetPtr();

    marioPaperOff();
    marioChgPaper(0);
    marioPaperOn(str_p_slit_802c433c);
    marioChgPose(str_M_S_1_80420fa0);
    marioChgPaper(str_PM_S_1A_802c4344);
    *(u32*)((s32)mario + 4) |= 4;
}

u8 marioReInit_slit(void) {
    extern void* marioGetPtr(void);
    extern void mapSetPaperAmbColor(void* color);
    extern void marioPaperOn(void* paper);
    extern void marioChgPose(void* pose);
    extern void marioChgPaper(void* paper);
    extern void allPartySlitOn(void);
    extern f32 float_0_80420f9c;
    extern f32 float_270_80420fb8;
    extern f32 float_6_80420fb4;
    extern u32 dat_80420f98;
    extern char str_M_S_1_80420fa0[6];
    extern char vec3_802c42e8[];

    void* mario = marioGetPtr();
    void* mario2 = marioGetPtr();
    u32 color;

    *(f32*)((s32)mario2 + 0x2B8) = float_0_80420f9c;
    *(s32*)((s32)mario2 + 0x2BC) = 0;
    *(s16*)((s32)mario2 + 0x2CE) = 0;
    *(s16*)((s32)mario2 + 0x2CC) = 0;
    *(s16*)((s32)mario2 + 0x2D2) = 0;
    *(u32*)((s32)mario2 + 0xC) &= ~1;
    *(u32*)mario2 &= ~0xF0000;
    *(u32*)((s32)mario2 + 0x4) |= 4;
    *(u32*)((s32)mario2 + 0x4) |= 0x1000000;

    if (*(s32*)((s32)mario2 + 0x240) == -1) {
        color = dat_80420f98;
        color = (color & 0xFF000000) | 0x00A8A8A8;
        mapSetPaperAmbColor(&color);
        marioPaperOn(vec3_802c42e8 + 0x54);
    }

    if (*(f32*)((s32)mario2 + 0x194) == float_0_80420f9c) {
        marioChgPose(str_M_S_1_80420fa0);
    }
    marioChgPaper(vec3_802c42e8 + 0x5C);

    *(u32*)((s32)mario + 0xC) &= ~1;
    *(u16*)((s32)mario + 0x2E) = 0x15;
    *(s16*)((s32)mario + 0x2F0) = 0xA;
    *(f32*)((s32)mario + 0x1AC) = float_270_80420fb8;
    *(f32*)((s32)mario + 0x1B0) = *(f32*)((s32)mario + 0x1AC);
    marioChgPose(str_M_S_1_80420fa0);
    marioChgPaper(vec3_802c42e8 + 0x64);
    *(s16*)((s32)mario + 0x2F0) = 0xA;
    *(s16*)((s32)mario + 0x2CE) = 0;
    *(f32*)((s32)mario + 0x1B8) = float_6_80420fb4;
    *(u32*)mario |= 0x100000;
    *(u32*)((s32)mario + 0x4) |= 0x1000000;
    allPartySlitOn();
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

void mot_slit(void) {
    extern void* marioGetPtr(void);
    extern void mapSetPaperAmbColor(void* color);
    extern void mapResetPaperAmbColor(void);
    extern void marioPaperOn(char* name);
    extern void marioPaperOff(void);
    extern void marioChgPose(char* pose);
    extern void marioChgPaper(char* paper);
    extern s32 marioRollChgChk(void);
    extern f64 revise360(f64 angle);
    extern f64 reviseAngle(f64 angle);
    extern f64 toMovedirSimple(f64 angle);
    extern u32 marioChkKey(void);
    extern s32 marioKeyOffChk(void);
    extern u32 marioBgmodeChk(void);
    extern s32 marioChkJump(void);
    extern s32 marioSlitChkWallAround(void);
    extern s32 marioCreviceWallChk(f32* pos);
    extern void marioSetFallPara(void);
    extern void allPartySlitOn(void);
    extern void allPartySlitOff(void);
    extern void marioAdjustMoveDir(void);
    extern void marioChgMotSub(s32 motion, s32 arg);
    extern void marioChgMot(s32 motion);
    extern u32 psndSFXOn_3D(s32 id, void* pos);
    extern u32 hitGetAttr(void* hit);
    extern s32 sysMsec2Frame(s32 msec);
    extern void sincosf(f32 angle, f32* s, f32* c);
    extern void movePos(f64 dist, f64 angle, f32* x, f32* z);
    extern s32 hitCheckVecHitObjXZ(void* arg, void* hit);
    extern void* camGetPtr(s32 id);
    extern s32 strcmp(char* a, char* b);

    extern u32 dat_80420f98;
    extern char vec3_802c42e8[];
    extern char str_M_S_1_80420fa0[];
    extern char str_M_R_1_80420fac[];
    extern char str_M_W_1_80420fc0[];
    extern char str_M_I_Y_80420fd0[];

    extern f32 float_0_80420f9c;
    extern f32 float_1_80420ff8;
    extern f32 float_2_80420ffc;
    extern f32 float_6_80420fb4;
    extern f32 float_10_80421000;
    extern f32 float_11_80420ff0;
    extern f32 float_20_80420ff4;
    extern f32 float_45_80420fe0;
    extern f32 float_90_80420fec;
    extern f32 float_135_80420fe4;
    extern f32 float_180_80420fa8;
    extern f32 float_225_80420fe8;
    extern f32 float_260_80420fc8;
    extern f32 float_270_80420fb8;
    extern f32 float_280_80420fcc;
    extern f32 float_315_80420fdc;
    extern f32 float_0p004_80420fbc;
    extern f32 float_0p5_80420fd8;

    void* player;
    void* p2;
    void* cam;
    void* hit;
    s32 state;
    s32 blocked;
    s32 t;
    s32 key;
    s32 side;
    s32 color;
    s16 s;
    f32 angle;
    f32 speed;
    f32 ftmp;
    f32 pos0[3];
    f32 pos1[3];
    f32 pos2[3];
    f32 sx;
    f32 cz;
    f32 hitArg[12];

#define P8(o) (*(s8*)((s32)player + (o)))
#define PU8(o) (*(u8*)((s32)player + (o)))
#define P16(o) (*(s16*)((s32)player + (o)))
#define PU16(o) (*(u16*)((s32)player + (o)))
#define P32(o) (*(s32*)((s32)player + (o)))
#define PU32(o) (*(u32*)((s32)player + (o)))
#define PF(o) (*(f32*)((s32)player + (o)))
#define PP(o) (*(void**)((s32)player + (o)))
#define CHG_IDLE() do { PF(0x180) = float_0_80420f9c; marioChgPose(str_M_S_1_80420fa0); } while (0)
#define CHG_RUN() do { PF(0x180) = PF(0x184); if ((PU32(0x0C) & 0x8000U) == 0) { marioChgPose(str_M_R_1_80420fac); } PU16(0x2D0) = (u16)(PU16(0x2D0) | 1); } while (0)
#define CAM_RESET() do { PF(0x148) = float_0_80420f9c; PF(0x158) = float_0p004_80420fbc; } while (0)
#define RESET_TO_SLIT_WAIT() do { P16(0x2CC) = 0; PF(0x180) = float_0_80420f9c; P16(0x2F0) = 10; P16(0x2CE) = 0; marioChgPose(str_M_S_1_80420fa0); marioChgPaper(vec3_802c42e8 + 0x64); } while (0)

    player = marioGetPtr();

    if (PF(0x194) == float_0_80420f9c) {
        PU32(0x0C) &= ~0x8000U;
    }

    if ((PU32(0x0C) & 1U) != 0) {
        p2 = marioGetPtr();
        player = p2;
        PF(0x2B8) = float_0_80420f9c;
        P32(0x2BC) = 0;
        P16(0x2CE) = 0;
        P16(0x2CC) = 0;
        P16(0x2D2) = 0;
        PU32(0x0C) &= ~1U;
        PU32(0x00) &= ~0x000F0000U;
        PU32(0x04) |= 4U;
        PU32(0x04) |= 0x01000000U;

        if (P32(0x240) == -1) {
            color = dat_80420f98;
            color = (color & 0xFF000000) | 0x00A8A8A8;
            mapSetPaperAmbColor(&color);
            marioPaperOn(vec3_802c42e8 + 0x54);
        }
        if (PF(0x194) == float_0_80420f9c) {
            marioChgPose(str_M_S_1_80420fa0);
        }
        marioChgPaper(vec3_802c42e8 + 0x5C);
        P16(0x2F0) = 0;
        P16(0x50) = 0;
    }

    if (marioRollChgChk() != 0) {
        angle = (f32)revise360((f64)(PF(0x1A4) - PF(0x19C)));
        if (angle < float_180_80420fa8) {
            PF(0x1AC) = float_180_80420fa8;
        } else {
            PF(0x1AC) = float_0_80420f9c;
        }
        PF(0x1B0) = PF(0x1AC);
        PU32(0x00) &= ~0x00800000U;
        PU32(0x00) &= ~0x01000000U;
        PU32(0x04) &= 0xFEFFF3FFU;
        marioPaperOff();
        PU32(0x00) &= ~0x08000000U;
        goto done;
    }

    state = P16(0x2F0);
    switch (state) {
    case 0:
        P32(0x2F4) = 30;
        P32(0x2BC) = 0;
        P16(0x2F0) = 2;
        PF(0x2C4) = PF(0x90);
        if ((PF(0x180) >= PF(0x188)) || (PF(0x194) != float_0_80420f9c)) {
            CHG_RUN();
        }
        marioGetPtr();
        psndSFXOn_3D(0x177, (void*)((s32)player + 0x8C));
        break;

    case 2:
        if (PF(0x194) == float_0_80420f9c) {
            CHG_IDLE();
        } else {
            if ((PF(0x180) >= PF(0x188)) || (PF(0x194) != float_0_80420f9c)) {
                CHG_RUN();
            }
            PF(0x1A4) = PF(0x198);
        }
        t = P32(0x2F4) - 1;
        P32(0x2F4) = t;
        if (t < 1) {
            PU32(0x00) |= 0x08000000U;
            P16(0x2F0) = 4;
            PF(0x1B8) = float_6_80420fb4;
        }
        if (P32(0x2F4) == 20) {
            allPartySlitOn();
        }
        break;

    case 4:
        if (marioChkKey() != 0) {
            PF(0x1A4) = (f32)toMovedirSimple((f64)PF(0x1AC));
        }
        if (PF(0x180) >= PF(0x188)) {
            CHG_RUN();
        }
        PF(0x1AC) = float_270_80420fb8;
        PF(0x1B0) = PF(0x1AC);
        if (PF(0x194) == float_0_80420f9c) {
            marioChgPose(str_M_S_1_80420fa0);
        }
        marioChgPaper(vec3_802c42e8 + 0x64);
        P16(0x2F0) = 10;
        P16(0x2CE) = 0;
        /* fall through */

    case 10:
        if (PF(0x90) != PF(0x2C4)) {
            p2 = marioGetPtr();
            *(f32*)((s32)p2 + 0x148) = float_0_80420f9c;
            *(f32*)((s32)p2 + 0x158) = float_0p004_80420fbc;
        }
        PF(0x2C4) = PF(0x90);

        if (P16(0x4E) == 0) {
            if (PF(0x180) >= PF(0x188)) {
                CHG_RUN();
            }

            if (marioChkKey() != 0) {
                blocked = 0;
                if ((PU32(0x0C) & 8U) != 0) {
                    blocked = 1;
                } else if (marioKeyOffChk() != 0) {
                    blocked = 1;
                } else if (marioBgmodeChk() == 1) {
                    blocked = 1;
                } else if (marioSlitChkWallAround() != 0) {
                    blocked = 1;
                } else {
                    pos0[0] = PF(0x8C);
                    pos0[1] = PF(0x90);
                    pos0[2] = PF(0x94);
                    if (marioCreviceWallChk(pos0) != 0) {
                        blocked = 1;
                    }
                }
                if (!blocked) {
                    PF(0x1A4) = PF(0x198);
                    P16(0x2F0) = 100;
                    break;
                }
            }

            if (marioChkJump() != 0) {
                goto done;
            }

            if (PF(0x194) == float_0_80420f9c) {
                PF(0x180) = float_0_80420f9c;
                if (P32(0x2BC) == 0) {
                    marioChgPose(str_M_S_1_80420fa0);
                    marioChgPaper(vec3_802c42e8 + 0x64);
                }
            } else {
                if (strcmp(*(char**)((s32)player + 0x18), str_M_I_Y_80420fd0) == 0) {
                    marioChgPose(str_M_W_1_80420fc0);
                }
                CAM_RESET();

                speed = PF(0x184);
                if ((PU32(0x00) & 0x08000000U) == 0) {
                    if ((PU32(0x00) & 0x80000000U) != 0) {
                        speed *= float_0p5_80420fd8;
                    }
                } else if ((P16(0x2CC) == 0) &&
                           ((P8(0x245) * P8(0x245) + P8(0x246) * P8(0x246)) < 0xBD2)) {
                    speed *= float_0p5_80420fd8;
                }
                PF(0x180) = speed;
                PF(0x1A4) = PF(0x198);
                if ((PU32(0x0C) & 0x8000U) == 0) {
                    marioChgPose(str_M_W_1_80420fc0);
                }

                angle = (f32)revise360((f64)(PF(0x198) - PF(0x19C)));
                if ((float_315_80420fdc <= angle) || (angle <= float_45_80420fe0) ||
                    ((float_135_80420fe4 <= angle) && (angle <= float_225_80420fe8))) {
                    marioChgPaper(vec3_802c42e8 + 0x74);
                } else {
                    marioChgPaper(vec3_802c42e8 + 0x6C);
                    angle = (f32)revise360((f64)(PF(0x1A4) - PF(0x19C)));
                    if (float_180_80420fa8 < angle) {
                        P16(0x2D2) = 2;
                        PF(0x1AC) = float_280_80420fcc;
                    } else {
                        P16(0x2D2) = 1;
                        PF(0x1AC) = float_260_80420fc8;
                    }
                }
                P16(0x2F0) = 0x14;
            }
        } else {
            if ((PU32(0x0C) & 0x8000U) == 0) {
                marioChgPose(str_M_W_1_80420fc0);
            }
            marioChgPaper(vec3_802c42e8 + 0x6C);
            angle = (f32)revise360((f64)(PF(0x1A4) - PF(0x19C)));
            if (float_180_80420fa8 < angle) {
                P16(0x2D2) = 2;
                PF(0x1AC) = float_280_80420fcc;
            } else {
                P16(0x2D2) = 1;
                PF(0x1AC) = float_260_80420fc8;
            }
        }
        break;
    }

    state = P16(0x2F0);
    if (state == 0x15) {
        s = P16(0x2CC) - 1;
        P16(0x2CC) = s;
        if (s < 1) {
            marioChgPaper(vec3_802c42e8 + 0x6C);
            P16(0x2F0) = 0x14;
            if (P16(0x2D2) == 1) {
                PF(0x1AC) = float_260_80420fc8;
            } else {
                PF(0x1AC) = float_280_80420fcc;
            }
        } else if (marioChkKey() == 0) {
            RESET_TO_SLIT_WAIT();
        }
    } else if (state == 0x14) {
        if (P16(0x2CC) > 0) {
            P16(0x2CC) = 0;
            marioChgPaper(vec3_802c42e8 + 0x6C);
            angle = (f32)revise360((f64)(PF(0x1A4) - PF(0x19C)));
            PF(0x1AC) = (angle <= float_180_80420fa8) ? float_260_80420fc8 : float_280_80420fcc;
            if (marioChkKey() == 0) {
                RESET_TO_SLIT_WAIT();
                goto after_walk_state;
            }
        }

        if (marioChkKey() != 0) {
            blocked = 0;
            if (marioKeyOffChk() != 0 || marioBgmodeChk() == 1 || marioSlitChkWallAround() != 0) {
                blocked = 1;
            } else {
                pos1[0] = PF(0x8C);
                pos1[1] = PF(0x90);
                pos1[2] = PF(0x94);
                if (marioCreviceWallChk(pos1) != 0) {
                    blocked = 1;
                }
            }
            if (!blocked) {
                P16(0x2F0) = 100;
            }
        }

        if ((PU32(0x00) & 0x00060000U) == 0) {
            marioChkJump();
        }

        if (marioChkKey() != 0) {
            if (PF(0x194) == float_0_80420f9c) {
                PF(0x180) = float_0_80420f9c;
                P16(0x2CE) = P16(0x2CE) + 1;
                if (P16(0x2CE) >= 6) {
                    P16(0x2F0) = 4;
                }
            } else {
                P16(0x2CE) = 0;
                if ((PU32(0x0C) & 0x8000U) == 0) {
                    if (((P8(0x245) < 0 ? -P8(0x245) : P8(0x245)) +
                         (P8(0x246) < 0 ? -P8(0x246) : P8(0x246))) < 0x44) {
                        marioChgPose(str_M_W_1_80420fc0);
                    } else {
                        marioChgPose(str_M_R_1_80420fac);
                    }
                }
            }
        }

        if (marioChkKey() != 0) {
            PF(0x1A4) = PF(0x198);
            speed = PF(0x184);
            if ((PU32(0x00) & 0x08000000U) == 0) {
                if ((PU32(0x00) & 0x80000000U) != 0) {
                    speed *= float_0p5_80420fd8;
                }
            } else if ((P16(0x2CC) == 0) &&
                       ((P8(0x245) * P8(0x245) + P8(0x246) * P8(0x246)) < 0xBD2)) {
                speed *= float_0p5_80420fd8;
            }
            PF(0x180) = speed;
        }

        angle = (f32)revise360((f64)(PF(0x198) - PF(0x19C)));
        if ((float_315_80420fdc <= angle) || (angle <= float_45_80420fe0) ||
            ((float_135_80420fe4 <= angle) && (angle <= float_225_80420fe8))) {
            marioChgPaper(vec3_802c42e8 + 0x74);
            P16(0x2CE) = 6;
            P16(0x2D2) = 0;
            angle = (f32)revise360((f64)(PF(0x1A4) - PF(0x19C)));
            PF(0x1AC) = (angle <= float_180_80420fa8) ? float_260_80420fc8 : float_280_80420fcc;
        } else {
            marioChgPaper(vec3_802c42e8 + 0x6C);
            angle = (f32)revise360((f64)(PF(0x1A4) - PF(0x19C)));
            if (P16(0x2D2) == 0) {
                P16(0x2D2) = (angle < float_180_80420fa8) ? 1 : 2;
            }
            if (P16(0x2D2) == 1) {
                if (float_180_80420fa8 <= angle) {
                    marioChgPaper(vec3_802c42e8 + 0x7C);
                    P16(0x2CC) = 6;
                    P16(0x2F0) = 0x15;
                    P16(0x2D2) = 2;
                    PF(0x180) = PF(0x184);
                    PF(0x1AC) = float_260_80420fc8;
                    if (marioChkKey() == 0) {
                        RESET_TO_SLIT_WAIT();
                    }
                }
            } else if (P16(0x2D2) == 2 && angle <= float_180_80420fa8) {
                marioChgPaper(vec3_802c42e8 + 0x7C);
                P16(0x2CC) = 6;
                P16(0x2F0) = 0x15;
                P16(0x2D2) = 1;
                PF(0x180) = PF(0x184);
                PF(0x1AC) = float_280_80420fcc;
                if (marioChkKey() == 0) {
                    RESET_TO_SLIT_WAIT();
                }
            }
        }
    }

after_walk_state:
    if ((PU32(0x00) & 0x08000000U) != 0) {
        hit = PP(0x1E8);
        if (hit == 0) {
            if ((P16(0x2F0) < 0x5A || P16(0x2F0) > 0x5E) && PP(0x1EC) != 0) {
                if ((hitGetAttr(PP(0x1EC)) & 0x2000) != 0) {
                    P32(0x2BC) += 3;
                    marioChgPaper(vec3_802c42e8 + 0x84);
                    if (P32(0x2BC) > 0xE && PU16(0x2E) == 0x15) {
                        P16(0x2F0) = 0x5A;
                    }
                } else {
                    P32(0x2BC) -= 1;
                    if (P32(0x2BC) < 0) {
                        P32(0x2BC) = 0;
                    }
                }
            }
        } else if (PP(0x1EC) != 0 && PU16(0x2E) == 0x15) {
            if ((hitGetAttr(hit) & 0x2000) == 0) {
                P32(0x2BC) = 0;
            } else {
                angle = (f32)revise360((f64)(PF(0x198) - PF(0x19C)));
                PF(0x2C0) = ((angle <= float_90_80420fec) || (float_270_80420fb8 <= angle)) ? float_0_80420f9c : float_180_80420fa8;
                P32(0x2BC) += 1;
                if (P32(0x2BC) >= sysMsec2Frame(0x32)) {
                    PF(0x1A4) = (f32)revise360((f64)(PF(0x2C0) + PF(0x19C)));
                    P32(0x2BC) = 0;
                    sincosf(PF(0x1A4), &sx, &cz);
                    hitArg[0] = PF(0x8C);
                    hitArg[1] = PF(0x90) + float_11_80420ff0;
                    hitArg[2] = PF(0x94);
                    hitArg[6] = sx;
                    hitArg[8] = cz;
                    hitArg[9] = float_20_80420ff4;
                    if (hitCheckVecHitObjXZ(hitArg, hit) != 0) {
                        hitArg[0] = PF(0x8C);
                        hitArg[2] = PF(0x94);
                        hitArg[9] = float_20_80420ff4;
                        angle = (f32)reviseAngle((f64)(float_90_80420fec + PF(0x1A0)));
                        movePos((f64)float_1_80420ff8, (f64)angle, &hitArg[0], &hitArg[2]);
                        if (hitCheckVecHitObjXZ(hitArg, hit) != 0) {
                            hitArg[0] = PF(0x8C);
                            hitArg[2] = PF(0x94);
                            hitArg[9] = float_20_80420ff4;
                            angle = (f32)reviseAngle((f64)(PF(0x1A0) - float_90_80420fec));
                            movePos((f64)float_1_80420ff8, (f64)angle, &hitArg[0], &hitArg[2]);
                            if (hitCheckVecHitObjXZ(hitArg, hit) != 0) {
                                P16(0x2F0) = 0x50;
                            }
                        }
                    }
                }
            }
        }
    }

    state = P16(0x2F0);
    if (state == 0x50) {
        cam = camGetPtr(2);
        if (cam != 0) {
            *(u32*)((s32)cam + 0x0C) |= 0x200;
        }
        CAM_RESET();
        PU32(0x00) |= 0x00400000U;
        if (PF(0x2C0) == float_0_80420f9c) {
            marioChgPaper(vec3_802c42e8 + 0x8C);
        } else {
            marioChgPaper(vec3_802c42e8 + 0x94);
        }
        P32(0x2F4) = 20;
        PF(0x180) = (float_2_80420ffc * PF(0x1B8)) / (f32)P32(0x2F4);
        P16(0x2F0) = 0x51;
        PU32(0x00) |= 0x01000000U;
    } else if (state == 0x51) {
        t = P32(0x2F4) - 1;
        P32(0x2F4) = t;
        if (t < 1) {
            P16(0x2F0) = 0x52;
        }
    } else if (state == 0x52) {
        cam = camGetPtr(2);
        if (cam != 0) {
            *(u32*)((s32)cam + 0x0C) &= ~0x200U;
        }
        PU32(0x00) &= ~0x02000000U;
        P16(0x2F0) = 0x14;
        PU32(0x00) &= ~0x00400000U;
    }

    state = P16(0x2F0);
    if (state == 0x5A) {
        cam = camGetPtr(2);
        if (cam != 0) {
            *(u32*)((s32)cam + 0x0C) |= 0x200;
        }
        CAM_RESET();
        PU32(0x00) |= 0x00400000U;
        marioChgPaper(vec3_802c42e8 + 0x84);
        P32(0x2F4) = sysMsec2Frame(500);
        PF(0x180) = float_0_80420f9c;
        P16(0x2F0) = 0x5B;
        p2 = marioGetPtr();
        *(s32*)((s32)p2 + 0x26C) = 1;
        *(u32*)((s32)p2 + 0x270) = *(u32*)((s32)p2 + 0x8C);
        *(u32*)((s32)p2 + 0x274) = *(u32*)((s32)p2 + 0x90);
        *(u32*)((s32)p2 + 0x278) = *(u32*)((s32)p2 + 0x94);
        P32(0x2F4) = 15;
        P16(0x2F0) = 0x5C;
        PU32(0x00) |= 0x00200000U;
        marioSetFallPara();
        PU32(0x00) &= ~0x00040000U;
        PU32(0x00) |= 0x00090000U;
    } else if (state == 0x5B) {
        P32(0x2F4) = 15;
        P16(0x2F0) = 0x5C;
        PU32(0x00) |= 0x00200000U;
        marioSetFallPara();
        PU32(0x00) &= ~0x00040000U;
        PU32(0x00) |= 0x00090000U;
    }

    state = P16(0x2F0);
    if (state == 0x5C) {
        angle = (f32)revise360((f64)(PF(0x1A4) - PF(0x19C)));
        PF(0x1AC) = (angle <= float_180_80420fa8) ? float_260_80420fc8 : float_280_80420fcc;
        t = P32(0x2F4) - 1;
        P32(0x2F4) = t;
        if (t < 1) {
            P16(0x2F0) = 0x5D;
            PU32(0x00) &= ~0x00200000U;
            PU32(0x00) &= ~0x00400000U;
        }
    }
    state = P16(0x2F0);
    if (state == 0x5D) {
        if (PP(0x1EC) != 0) {
            PU32(0x00) &= ~0x000F0000U;
            P16(0x2F0) = 0x5E;
        }
    } else if (state == 0x5E) {
        cam = camGetPtr(2);
        if (cam != 0) {
            *(u32*)((s32)cam + 0x0C) &= ~0x200U;
        }
        P16(0x2F0) = 0x14;
    }

    switch (P16(0x2F0)) {
    case 100:
        PU32(0x00) &= ~0x01000000U;
        P16(0x2F2) = 0;
        PU32(0x00) &= ~0x08000000U;
        PF(0x1B8) = float_20_80420ff4;
        if (PF(0x194) != float_0_80420f9c) {
            CHG_RUN();
        }
        if (PF(0x180) == float_0_80420f9c) {
            marioChgPose(str_M_S_1_80420fa0);
        }
        marioChgPaper(vec3_802c42e8 + 0x9C);
        P16(0x2F2) = 30;
        P16(0x2F0) = 0x65;
        angle = (f32)revise360((f64)(PF(0x1A4) - PF(0x19C)));
        if (angle < float_180_80420fa8) {
            PF(0x1AC) = float_180_80420fa8;
        } else {
            PF(0x1AC) = float_0_80420f9c;
        }
        PF(0x1B0) = PF(0x1AC);
        if (PF(0x194) == float_0_80420f9c) {
            PF(0x1A4) = (f32)toMovedirSimple((f64)PF(0x1AC));
        }
        psndSFXOn_3D(0x178, (void*)((s32)player + 0x8C));
        /* fall through */
    case 0x65:
        if (PF(0x194) == float_0_80420f9c) {
            CHG_IDLE();
        } else {
            CHG_RUN();
            PF(0x1A4) = PF(0x198);
        }
        s = P16(0x2F2) - 1;
        P16(0x2F2) = s;
        if (s < 1) {
            P16(0x2F0) = 0x66;
        }
        break;

    case 0x66:
        if (PF(0x194) == float_0_80420f9c) {
            CHG_IDLE();
        } else {
            CHG_RUN();
        }
        PU32(0x00) &= ~0x00800000U;
        PU32(0x00) &= ~0x01000000U;
        PU32(0x04) &= 0xFEFFF3FFU;
        marioPaperOff();
        mapResetPaperAmbColor();
        PU32(0x04) &= ~4U;
        marioAdjustMoveDir();
        CAM_RESET();
        marioChgMotSub(0, 0);
        break;

    case 0x6E:
        PU32(0x00) &= ~0x00800000U;
        PU32(0x00) &= ~0x01000000U;
        PU32(0x00) &= ~0x00400000U;
        PU32(0x00) &= ~0x02000000U;
        P16(0x2F2) = 0;
        PU32(0x00) &= ~0x08000000U;
        PF(0x1B8) = float_20_80420ff4;
        if (PF(0x194) == float_0_80420f9c) {
            marioChgPose(str_M_S_1_80420fa0);
        }
        marioChgPaper(vec3_802c42e8 + 0x9C);
        P16(0x2F2) = 30;
        P16(0x2F0) = 0x6F;
        PF(0x180) = float_0_80420f9c;
        angle = (f32)revise360((f64)(PF(0x1A4) - PF(0x19C)));
        if (angle < float_180_80420fa8) {
            PF(0x1AC) = float_180_80420fa8;
        } else {
            PF(0x1AC) = float_0_80420f9c;
        }
        PF(0x1B0) = PF(0x1AC);
        PF(0x1A4) = (f32)toMovedirSimple((f64)PF(0x1AC));
        psndSFXOn_3D(0x178, (void*)((s32)player + 0x8C));
        /* fall through */
    case 0x6F:
        s = P16(0x2F2) - 1;
        P16(0x2F2) = s;
        if (s < 1) {
            P16(0x2F0) = 0x70;
        }
        break;

    case 0x70:
        PU32(0x00) &= ~0x00800000U;
        PU32(0x00) &= ~0x01000000U;
        PU32(0x04) &= 0xFEFFF3FFU;
        marioPaperOff();
        mapResetPaperAmbColor();
        PU32(0x04) &= ~4U;
        marioAdjustMoveDir();
        allPartySlitOff();
        CAM_RESET();
        marioChgMot(0);
        break;

    case 0x78:
        P16(0x2F2) = 0;
        PU32(0x00) &= ~0x08000000U;
        PF(0x1B8) = float_20_80420ff4;
        if (PF(0x194) == float_0_80420f9c) {
            marioChgPose(str_M_S_1_80420fa0);
        }
        marioChgPaper(vec3_802c42e8 + 0x9C);
        P16(0x2F2) = 30;
        P16(0x2F0) = 0x79;
        PF(0x180) = float_0_80420f9c;
        angle = (f32)revise360((f64)(PF(0x1A4) - PF(0x19C)));
        if (angle < float_180_80420fa8) {
            PF(0x1AC) = float_180_80420fa8;
        } else {
            PF(0x1AC) = float_0_80420f9c;
        }
        PF(0x1B0) = PF(0x1AC);
        PF(0x1A4) = (f32)toMovedirSimple((f64)PF(0x1AC));
        psndSFXOn_3D(0x178, (void*)((s32)player + 0x8C));
        /* fall through */
    case 0x79:
        s = P16(0x2F2) - 1;
        P16(0x2F2) = s;
        if (s < 1) {
            P16(0x2F0) = 0x7A;
        }
        break;

    case 0x7A:
        PU32(0x00) &= ~0x00800000U;
        PU32(0x00) &= ~0x01000000U;
        PU32(0x00) &= ~0x02000000U;
        PU32(0x04) &= 0xFEFFF3FFU;
        marioPaperOff();
        mapResetPaperAmbColor();
        PU32(0x04) &= ~4U;
        marioAdjustMoveDir();
        allPartySlitOff();
        CAM_RESET();
        marioChgMot(0);
        break;
    }

done:
#undef P8
#undef PU8
#undef P16
#undef PU16
#undef P32
#undef PU32
#undef PF
#undef PP
#undef CHG_IDLE
#undef CHG_RUN
#undef CAM_RESET
#undef RESET_TO_SLIT_WAIT
    return;
}

#pragma no_register_save_helpers reset
#pragma use_lmw_stmw reset
