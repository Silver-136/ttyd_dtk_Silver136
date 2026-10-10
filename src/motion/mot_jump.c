#include "motion/mot_jump.h"
#include "bowser/koopa_motion.h"

extern void* gp;
extern void clear_hitobj_ride(void);
extern void marioSetJumpPara(void);
extern void marioMakeJumpPara(void);
extern void marioSetPeakYpos(void);
extern void marioChgPose(char*);
extern void marioResetCamFollowRate(void);
extern void camFollowYOff(void);
extern u32 hitGetAttr(void*);
extern void* mobjHitObjPtrToPtr(s32);
extern u32 marioChkFront(f32, f32, f32*, void*);
extern f32 revise360(f32);
extern void sincosf(f32, f32*, f32*);
extern f64 distABf(f64, f64, f64, f64);
extern s32 irand(s32);
extern void psndSFXOn_3D(void*, void*);

void* marioGetPtr(void);
void psndSFXOff(s32 id);
s32 strcmp(const char* a, const char* b);
void camFollowYOn(void);

u8 marioPreJump(void) {
    extern u8 marioSetJumpPara(void);
    void* mario = marioGetPtr();
    s32 x;
    s32 y;

    *(u32*)mario &= ~0x20000;
    *(u32*)mario |= 0x10000;
    *(s16*)((s32)mario + 0x50) = 0;
    *(s16*)((s32)mario + 0x52) = 0;
    *(s32*)((s32)mario + 0x1EC) = *(s32*)((s32)mario + 0x1E8);
    clear_hitobj_ride();
    marioSetJumpPara();
    x = *(s32*)((s32)mario + 0x8C);
    y = *(s32*)((s32)mario + 0x90);
    *(s32*)((s32)mario + 0x11C) = x;
    *(s32*)((s32)mario + 0x120) = y;
    *(s32*)((s32)mario + 0x124) = *(s32*)((s32)mario + 0x94);
    *(f32*)((s32)mario + 0x190) = *(f32*)((s32)mario + 0x180);
}

f32 marioGetJumpHeight(void) {
    void* mario = marioGetPtr();
    return *(f32*)((s32)mario + 0x2D0);
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void mot_jumpStand(void) {
    extern f32 float_0_80420cc8;
    extern char str_M_J_1B_802c40c8[];
    extern char str_M_J_1C_802c40b8;
    extern s32 marioSlitButton(void);
    extern void marioSlitForceCancel(void);
    extern f32 marioJumpStandData[10];
    extern f32 float_neg1_80420d08;

    void* mario = marioGetPtr();
    void* work;
    f32 oldVel;

    if ((*(u32*)((s32)mario + 0xC) & 1) != 0) {
        *(u32*)((s32)mario + 0xC) &= ~1;
        *(f32*)((s32)mario + 0x180) = float_0_80420cc8;
        work = marioGetPtr();
        *(u32*)((s32)work + 0x0) &= ~0x20000;
        *(u32*)((s32)work + 0x0) |= 0x10000;
        *(s16*)((s32)work + 0x50) = 0;
        *(s16*)((s32)work + 0x52) = 0;
        *(s32*)((s32)work + 0x1EC) = *(s32*)((s32)work + 0x1E8);
        clear_hitobj_ride();
        marioSetJumpPara();
        *(u32*)((s32)work + 0x11C) = *(u32*)((s32)work + 0x8C);
        *(u32*)((s32)work + 0x120) = *(u32*)((s32)work + 0x90);
        *(u32*)((s32)work + 0x124) = *(u32*)((s32)work + 0x94);
        *(f32*)((s32)work + 0x190) = *(f32*)((s32)work + 0x180);
        *(f32*)((s32)mario + 0x2C8) = float_0_80420cc8;
        *(s32*)((s32)mario + 0x44) = 0;
        *(s32*)((s32)mario + 0x48) = 0;
    }

    switch (*(s32*)((s32)mario + 0x44)) {
        case 0: {
            s32 count = *(s32*)((s32)mario + 0x48) + 1;
            *(s32*)((s32)mario + 0x48) = count;
            if (count >= 3) {
                *(s32*)((s32)mario + 0x44) = 4;
                *(f32*)((s32)mario + 0x7C) = marioJumpStandData[0];
                *(f32*)((s32)mario + 0x80) = marioJumpStandData[1];
                *(f32*)((s32)mario + 0x84) = marioJumpStandData[2];
                *(f32*)((s32)mario + 0x88) = marioJumpStandData[3];
                if ((*(u32*)((s32)mario + 0x0) & 0x2000) == 0 && *(s8*)((s32)mario + 0x3C) == 0) {
                    marioChgPose(str_M_J_1B_802c40c8);
                }
            }
            break;
        }
        case 4:
            oldVel = *(f32*)((s32)mario + 0x7C);
            marioMakeJumpPara();
            if (*(f32*)((s32)mario + 0x7C) <= oldVel) {
                *(s32*)((s32)mario + 0x44) = 6;
            }
            *(f32*)((s32)mario + 0x90) += *(f32*)((s32)mario + 0x7C);
            *(f32*)((s32)mario + 0x2C8) += *(f32*)((s32)mario + 0x7C);
            break;
        case 6:
            *(f32*)((s32)mario + 0x90) += *(f32*)((s32)mario + 0x7C);
            *(f32*)((s32)mario + 0x2C8) += *(f32*)((s32)mario + 0x7C);
            if (*(f32*)((s32)mario + 0x2D0) * marioJumpStandData[9] <= *(f32*)((s32)mario + 0x2C8)) {
                *(s32*)((s32)mario + 0x44) = 8;
            }
            if (*(f32*)((s32)mario + 0x7C) <= float_0_80420cc8) {
                *(s32*)((s32)mario + 0x44) = 8;
            }
            break;
        case 8:
            *(f32*)((s32)mario + 0x7C) =
                (float_neg1_80420d08 - *(f32*)((s32)mario + 0x7C)) * marioJumpStandData[8] +
                *(f32*)((s32)mario + 0x7C);
            *(f32*)((s32)mario + 0x90) += *(f32*)((s32)mario + 0x7C);
            *(f32*)((s32)mario + 0x2C8) += *(f32*)((s32)mario + 0x7C);
            if (*(f32*)((s32)mario + 0x7C) <= float_0_80420cc8) {
                *(s32*)((s32)mario + 0x44) = 0xA;
                if ((*(u32*)((s32)mario + 0x0) & 0x2000) == 0 && *(s8*)((s32)mario + 0x3C) == 0) {
                    marioChgPose(&str_M_J_1C_802c40b8);
                }
                marioSetPeakYpos();
                *(f32*)((s32)mario + 0x7C) = marioJumpStandData[4];
                *(f32*)((s32)mario + 0x80) = marioJumpStandData[5];
                *(f32*)((s32)mario + 0x84) = marioJumpStandData[6];
                *(f32*)((s32)mario + 0x88) = marioJumpStandData[7];
                *(u32*)((s32)mario + 0x0) &= ~0x10000;
                *(u32*)((s32)mario + 0x0) |= 0x20000;
                *(s32*)((s32)mario + 0x44) = 0x14;
            }
            break;
        case 0xA:
            marioMakeJumpPara();
            if (*(f32*)((s32)mario + 0x7C) <= float_0_80420cc8) {
                if ((*(u32*)((s32)mario + 0x0) & 0x2000) == 0 && *(s8*)((s32)mario + 0x3C) == 0) {
                    marioChgPose(&str_M_J_1C_802c40b8);
                }
                marioSetPeakYpos();
                *(f32*)((s32)mario + 0x7C) = marioJumpStandData[4];
                *(f32*)((s32)mario + 0x80) = marioJumpStandData[5];
                *(f32*)((s32)mario + 0x84) = marioJumpStandData[6];
                *(f32*)((s32)mario + 0x88) = marioJumpStandData[7];
                *(u32*)((s32)mario + 0x0) &= ~0x10000;
                *(u32*)((s32)mario + 0x0) |= 0x20000;
                *(s32*)((s32)mario + 0x44) = 0x14;
            }
            *(f32*)((s32)mario + 0x90) += *(f32*)((s32)mario + 0x7C);
            break;
        case 0x14:
            *(s32*)((s32)mario + 0x48) -= 1;
            if (*(s32*)((s32)mario + 0x48) < 1) {
                *(f32*)((s32)mario + 0x180) = float_0_80420cc8;
                *(s32*)((s32)mario + 0x44) = 0x1E;
            }
            break;
    }
    *(s16*)((s32)mario + 0x50) += 1;
    if (marioSlitButton() != 2) {
        marioSlitForceCancel();
    }
}

#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset
void mot_jumpSw(void) {
    extern f32 float_0_80420cc8;
    extern f32 float_0p5_80420cf0;
    extern f32 float_5_80420cf4;
    extern f32 float_30_80420cf8;
    extern f32 float_180_80420cfc;
    extern char str_M_J_1B_802c40c8[];
    extern char str_M_J_1C_802c40b8[];
    extern f32 marioJumpSwData[8];
    typedef struct Vec { f32 x, y, z; } Vec;
    extern const Vec vec3_802c4088;

    char* poseBase = (char*)&vec3_802c4088;
    void* mario = marioGetPtr();

    if ((*(u32*)((s32)mario + 0xC) & 1) != 0) {
        void* work;

        *(u32*)((s32)mario + 0xC) &= ~1;
        marioResetCamFollowRate();
        *(u32*)((s32)mario + 0x0) |= 0x80;
        camFollowYOff();
        work = marioGetPtr();
        *(u32*)((s32)work + 0x0) &= ~0x20000;
        *(u32*)((s32)work + 0x0) |= 0x10000;
        *(s16*)((s32)work + 0x50) = 0;
        *(s16*)((s32)work + 0x52) = 0;
        *(s32*)((s32)work + 0x1EC) = *(s32*)((s32)work + 0x1E8);
        clear_hitobj_ride();
        marioSetJumpPara();
        *(Vec*)((s32)work + 0x11C) = *(Vec*)((s32)work + 0x8C);
        *(f32*)((s32)work + 0x190) = *(f32*)((s32)work + 0x180);

        if ((*(u32*)((s32)mario + 0x0) & 0x2000) == 0 && *(s8*)((s32)mario + 0x3C) == 0) {
            marioChgPose(poseBase + 0x40);
        }
        if (*(void**)((s32)mario + 0x1EC) != 0) {
            if ((hitGetAttr(*(void**)((s32)mario + 0x1EC)) & 0x80000000) != 0) {
                Vec hitPos;
                Vec pos;
                f32 dist;
                f32 sx;
                f32 sz;
                f32 angle;
                f64 value;

                work = mobjHitObjPtrToPtr((s32)*(void**)((s32)mario + 0x1EC));
                *(f32*)((s32)mario + 0x2CC) = *(f32*)((s32)work + 0x38);
                *(f32*)((s32)mario + 0x2D0) = *(f32*)((s32)work + 0x40);
                pos = ((const Vec*)poseBase)[0];
                pos.x = *(f32*)((s32)work + 0x38);
                pos.y = *(f32*)((s32)work + 0x3C) + *(f32*)((s32)mario + 0x1BC) * float_0p5_80420cf0;
                pos.z = *(f32*)((s32)work + 0x40);
                hitPos = pos;
                if (marioChkFront(*(f32*)((s32)mario + 0x1A4),
                                  *(f32*)((s32)mario + 0x1B8) * float_5_80420cf4,
                                  &dist, &hitPos) != 0 &&
                    dist >= float_0_80420cc8 &&
                    dist <= (*(f32*)((s32)mario + 0x1B8) * float_0p5_80420cf0 + float_30_80420cf8)) {
                    angle = revise360(float_180_80420cfc + *(f32*)((s32)mario + 0x1A4));
                    *(f32*)((s32)mario + 0x1A4) = angle;
                    *(f32*)((s32)mario + 0x1A0) = angle;
                }
                sincosf(*(f32*)((s32)mario + 0x1A4), &sx, &sz);
                *(s32*)((s32)mario + 0x48) = 0x14;
                value = distABf(*(f32*)((s32)mario + 0x8C), *(f32*)((s32)mario + 0x94),
                                sx * float_30_80420cf8 + *(f32*)((s32)work + 0x38),
                                sz * float_30_80420cf8 + *(f32*)((s32)work + 0x40));
                *(f32*)((s32)mario + 0x180) = (f32)value / (f32)*(s32*)((s32)mario + 0x48);
            }
        } else {
            f32 sx;
            f32 sz;
            f32 angle;
            f64 value;

            angle = revise360(float_180_80420cfc + *(f32*)((s32)mario + 0x1A4));
            sincosf(angle, &sx, &sz);
            *(f32*)((s32)mario + 0x1A4) = angle;
            *(f32*)((s32)mario + 0x1A0) = angle;
            *(s32*)((s32)mario + 0x48) = 0x14;
            value = distABf(*(f32*)((s32)mario + 0x8C), *(f32*)((s32)mario + 0x94),
                            sx * float_30_80420cf8 + *(f32*)((s32)mario + 0x8C),
                            sz * float_30_80420cf8 + *(f32*)((s32)mario + 0x94));
            *(f32*)((s32)mario + 0x180) = (f32)value / (f32)*(s32*)((s32)mario + 0x48);
        }
        *(s32*)((s32)mario + 0x44) = 0;
    }

    switch (*(s32*)((s32)mario + 0x44)) {
        case 0:
            *(s32*)((s32)mario + 0x48) -= 1;
            marioMakeJumpPara();
            if (*(f32*)((s32)mario + 0x7C) <= float_0_80420cc8) {
                if ((*(u32*)((s32)mario + 0x0) & 0x2000) == 0 && *(s8*)((s32)mario + 0x3C) == 0) {
                    marioChgPose(poseBase + 0x30);
                }
                marioSetPeakYpos();
                *(f32*)((s32)mario + 0x7C) = marioJumpSwData[4];
                *(f32*)((s32)mario + 0x80) = marioJumpSwData[5];
                *(f32*)((s32)mario + 0x84) = marioJumpSwData[6];
                *(f32*)((s32)mario + 0x88) = marioJumpSwData[7];
                *(u32*)((s32)mario + 0x0) &= ~0x10000;
                *(u32*)((s32)mario + 0x0) |= 0x20000;
                *(s32*)((s32)mario + 0x44) = 0xA;
            }
            *(f32*)((s32)mario + 0x90) += *(f32*)((s32)mario + 0x7C);
            break;
        case 0xA:
            if (--*(s32*)((s32)mario + 0x48) <= 0) {
                *(f32*)((s32)mario + 0x180) = float_0_80420cc8;
                *(s32*)((s32)mario + 0x44) = 0x14;
            }
            break;
    }
    *(s16*)((s32)mario + 0x50) += 1;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void mot_jumpNpc(void) {
    extern f32 float_0_80420cc8;
    extern f32 float_0p5_80420cf0;
    extern f32 float_5_80420cf4;
    extern f32 float_30_80420cf8;
    extern f32 float_180_80420cfc;
    extern char str_M_J_1B_802c40c8[];
    extern char str_M_J_1C_802c40b8[];
    extern void kpa_jumpNpc(void);
    extern f32 float_2p6_80420cd4;
    extern f32 float_neg0p06_80420cd8;
    extern f32 float_neg0p01_80420cdc;
    extern f32 float_neg0p002_80420ce0;
    extern f32 float_1_80420ce4;
    extern f32 float_neg0p04_80420ce8;
    extern f32 float_neg0p02_80420cec;
    extern f32 float_neg0p03_80420d00;
    extern f32 float_0p04_80420d04;

    typedef struct Vec { f32 x, y, z; } Vec;
    extern const Vec vec3_802c4088;

    char* poseBase = (char*)&vec3_802c4088;
    void* mario = marioGetPtr();

    if (*(s8*)((s32)mario + 0x3C) == 2) {
        kpa_jumpNpc();
        return;
    }

    if ((*(u32*)((s32)mario + 0xC) & 1) != 0) {
        void* work;

        *(u32*)((s32)mario + 0xC) &= ~1;
        marioResetCamFollowRate();
        if (*(s8*)((s32)mario + 0x3C) != 2) {
            *(u32*)((s32)mario + 0x0) |= 0x80;
        }
        camFollowYOff();

        work = marioGetPtr();
        *(u32*)((s32)work + 0x0) &= ~0x20000;
        *(u32*)((s32)work + 0x0) |= 0x10000;
        *(s16*)((s32)work + 0x50) = 0;
        *(s16*)((s32)work + 0x52) = 0;
        *(s32*)((s32)work + 0x1EC) = *(s32*)((s32)work + 0x1E8);
        clear_hitobj_ride();
        marioSetJumpPara();
        *(Vec*)((s32)work + 0x11C) = *(Vec*)((s32)work + 0x8C);
        *(f32*)((s32)work + 0x190) = *(f32*)((s32)work + 0x180);

        if (*(s8*)((s32)mario + 0x3C) == 2) {
            *(f32*)((s32)mario + 0x7C) = float_2p6_80420cd4;
            *(f32*)((s32)mario + 0x80) = float_neg0p06_80420cd8;
            *(f32*)((s32)mario + 0x84) = float_neg0p01_80420cdc;
            *(f32*)((s32)mario + 0x88) = float_neg0p002_80420ce0;
        } else {
            *(f32*)((s32)mario + 0x7C) = float_1_80420ce4;
            *(f32*)((s32)mario + 0x80) = float_neg0p04_80420ce8;
            *(f32*)((s32)mario + 0x84) = float_neg0p01_80420cdc;
            *(f32*)((s32)mario + 0x88) = float_neg0p02_80420cec;
        }
        if ((*(u32*)((s32)mario + 0x0) & 0x2000) == 0 && *(s8*)((s32)mario + 0x3C) == 0) {
            marioChgPose(poseBase + 0x40);
        }

        if (*(void**)((s32)mario + 0x1EC) != 0) {
            if ((hitGetAttr(*(void**)((s32)mario + 0x1EC)) & 0x80000000) != 0) {
                Vec pos;
                Vec hitPos;
                f32 dist;
                f32 sx;
                f32 sz;
                f32 temp;
                f64 value;

                work = mobjHitObjPtrToPtr((s32)*(void**)((s32)mario + 0x1EC));
                *(f32*)((s32)mario + 0x2CC) = *(f32*)((s32)work + 0x38);
                *(f32*)((s32)mario + 0x2D0) = *(f32*)((s32)work + 0x40);
                pos = ((const Vec*)poseBase)[1];
                pos.x = *(f32*)((s32)work + 0x38);
                pos.y = *(f32*)((s32)work + 0x3C) + *(f32*)((s32)mario + 0x1BC) * float_0p5_80420cf0;
                pos.z = *(f32*)((s32)work + 0x40);
                hitPos = pos;
                if (marioChkFront(*(f32*)((s32)mario + 0x1A4),
                                  *(f32*)((s32)mario + 0x1B8) * float_5_80420cf4,
                                  &dist, &hitPos) != 0 &&
                    dist >= float_0_80420cc8 &&
                    dist <= (*(f32*)((s32)mario + 0x1B8) * float_0p5_80420cf0 + float_30_80420cf8)) {
                    temp = revise360(float_180_80420cfc + *(f32*)((s32)mario + 0x1A4));
                    *(f32*)((s32)mario + 0x1A4) = temp;
                    *(f32*)((s32)mario + 0x1A0) = temp;
                }
                sincosf(*(f32*)((s32)mario + 0x1A4), &sx, &sz);
                *(s32*)((s32)mario + 0x48) = 0x14;
                value = distABf(*(f32*)((s32)mario + 0x8C), *(f32*)((s32)mario + 0x94),
                                sx * float_30_80420cf8 + *(f32*)((s32)work + 0x38),
                                sz * float_30_80420cf8 + *(f32*)((s32)work + 0x40));
                *(f32*)((s32)mario + 0x180) =
                    (f32)(value / (f64)(f32)*(s32*)((s32)mario + 0x48));
            }
        } else {
            f32 sx;
            f32 sz;
            f32 angle;
            f64 value;

            revise360(float_180_80420cfc + *(f32*)((s32)mario + 0x1A4));
            angle = *(f32*)((s32)mario + 0x1A4);
            sincosf(angle, &sx, &sz);
            *(f32*)((s32)mario + 0x1A4) = angle;
            *(f32*)((s32)mario + 0x1A0) = angle;
            *(s32*)((s32)mario + 0x48) = 0x14;
            value = distABf(*(f32*)((s32)mario + 0x8C), *(f32*)((s32)mario + 0x94),
                            sx * float_30_80420cf8 + *(f32*)((s32)mario + 0x8C),
                            sz * float_30_80420cf8 + *(f32*)((s32)mario + 0x94));
            *(f32*)((s32)mario + 0x180) =
                (f32)(value / (f64)(f32)*(s32*)((s32)mario + 0x48));
        }
        *(s32*)((s32)mario + 0x44) = 0;
    }

    switch (*(s32*)((s32)mario + 0x44)) {
        case 0:
            *(s32*)((s32)mario + 0x48) -= 1;
            marioMakeJumpPara();
            if (*(f32*)((s32)mario + 0x7C) <= float_0_80420cc8) {
                if ((*(u32*)((s32)mario + 0x0) & 0x2000) == 0 && *(s8*)((s32)mario + 0x3C) == 0) {
                    marioChgPose(poseBase + 0x30);
                }
                marioSetPeakYpos();
                *(f32*)((s32)mario + 0x7C) = float_neg0p03_80420d00;
                *(f32*)((s32)mario + 0x80) = float_neg0p02_80420cec;
                *(f32*)((s32)mario + 0x84) = float_0p04_80420d04;
                *(f32*)((s32)mario + 0x88) = float_0_80420cc8;
                *(u32*)((s32)mario + 0x0) &= ~0x10000;
                *(u32*)((s32)mario + 0x0) |= 0x20000;
                *(s32*)((s32)mario + 0x44) = 0x14;
            }
            *(f32*)((s32)mario + 0x90) += *(f32*)((s32)mario + 0x7C);
            break;
        case 0x14:
            if (--*(s32*)((s32)mario + 0x48) <= 0) {
                *(f32*)((s32)mario + 0x180) = float_0_80420cc8;
                *(s32*)((s32)mario + 0x44) = 0x1E;
            }
            break;
    }
    *(s16*)((s32)mario + 0x50) += 1;
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

void N_mot_jumpNpc_post(void) {
    extern char str_gon_10_802c40d0[];
    void* mario = marioGetPtr();
    if (strcmp((char*)((s32)gp + 0x12C), str_gon_10_802c40d0) == 0) {
        if (*(u32*)((s32)mario + 4) & 0x20000) {
            camFollowYOn();
        }
    }
}

typedef struct VecData {
    f32 x;
    f32 y;
    f32 z;
} VecData;

#pragma use_lmw_stmw off
#pragma opt_common_subs off
void mot_jump(void) {
    extern char str_M_J_1B_802c40c8[];

    void* mario = marioGetPtr();
    if (*(s8*)((s32)mario + 0x3C) == 2) {
        kpa_jump();
        return;
    }
    if ((*(u32*)((s32)mario + 0xC) & 1) != 0) {
        void* mario2;
        s32 rnd;
        void* id;

        *(u32*)((s32)mario + 0xC) &= ~1;
        marioResetCamFollowRate();
        *(u32*)mario |= 0x80000;
        *(s32*)((s32)mario + 0x48) = 2;
        *(s32*)((s32)mario + 0x44) = 0;
        mario2 = marioGetPtr();
        *(u32*)mario2 &= ~0x20000;
        *(u32*)mario2 |= 0x10000;
        *(s16*)((s32)mario2 + 0x50) = 0;
        *(s16*)((s32)mario2 + 0x52) = 0;
        *(s32*)((s32)mario2 + 0x1EC) = *(s32*)((s32)mario2 + 0x1E8);
        clear_hitobj_ride();
        marioSetJumpPara();
        *(VecData*)((s32)mario2 + 0x11C) = *(VecData*)((s32)mario2 + 0x8C);
        *(f32*)((s32)mario2 + 0x190) = *(f32*)((s32)mario2 + 0x180);
        if (*(u16*)((s32)mario + 0x2E) == 8) {
            camFollowYOn();
        } else {
            camFollowYOff();
        }
        if ((*(u32*)mario & 0x2000) == 0 && *(s8*)((s32)mario + 0x3C) == 0) {
            marioChgPose(str_M_J_1B_802c40c8);
        }
        if (*(u16*)((s32)mario + 0x2E) != 8) {
            rnd = irand(10000);
            if (rnd < 2000) {
                id = (void*)0x9D;
            } else if (rnd < 4000) {
                id = (void*)0x9E;
            } else if (rnd < 6000) {
                id = (void*)0x9F;
            } else if (rnd < 8000) {
                id = (void*)0xA0;
            } else {
                id = (void*)0xA1;
            }
            psndSFXOn_3D(id, (void*)((s32)mario + 0x8C));
        }
        psndSFXOn_3D((void*)0x14D, (void*)((s32)mario + 0x8C));
        if (*(s16*)((s32)mario + 0x50) == 0) {
            *(VecData*)((s32)mario + 0xD4) = *(VecData*)((s32)mario + 0x8C);
        }
        *(u32*)((s32)mario + 0xC) &= ~0x10000;
    }
    switch (*(s32*)((s32)mario + 0x44)) {
        case 0:
            if (--*(s32*)((s32)mario + 0x48) <= 0) {
                if ((*(u32*)mario & 0x2000) == 0 && *(s8*)((s32)mario + 0x3C) == 0) {
                    marioChgPose(str_M_J_1B_802c40c8);
                }
                *(s32*)((s32)mario + 0x44) = 10;
            }
            break;
    }
    *(s16*)((s32)mario + 0x50) += 1;
}
#pragma opt_common_subs reset
#pragma use_lmw_stmw reset

#pragma use_lmw_stmw off
#pragma opt_common_subs off
void mot_jumpSmall(void) {
    extern char str_M_J_1B_802c40c8[];

    void* mario = marioGetPtr();
    if (*(s8*)((s32)mario + 0x3C) == 2) {
        kpa_jump();
        return;
    }
    if ((*(u32*)((s32)mario + 0xC) & 1) != 0) {
        void* mario2;
        s32 rnd;
        void* id;

        *(u32*)((s32)mario + 0xC) &= ~1;
        marioResetCamFollowRate();
        *(u32*)mario |= 0x80000;
        *(s32*)((s32)mario + 0x48) = 2;
        *(s32*)((s32)mario + 0x44) = 0;
        mario2 = marioGetPtr();
        *(u32*)mario2 &= ~0x20000;
        *(u32*)mario2 |= 0x10000;
        *(s16*)((s32)mario2 + 0x50) = 0;
        *(s16*)((s32)mario2 + 0x52) = 0;
        *(s32*)((s32)mario2 + 0x1EC) = *(s32*)((s32)mario2 + 0x1E8);
        clear_hitobj_ride();
        marioSetJumpPara();
        *(VecData*)((s32)mario2 + 0x11C) = *(VecData*)((s32)mario2 + 0x8C);
        *(f32*)((s32)mario2 + 0x190) = *(f32*)((s32)mario2 + 0x180);
        if (*(u16*)((s32)mario + 0x2E) != 8) {
            camFollowYOff();
        }
        if ((*(u32*)mario & 0x2000) == 0 && *(s8*)((s32)mario + 0x3C) == 0) {
            marioChgPose(str_M_J_1B_802c40c8);
        }
        if (*(u16*)((s32)mario + 0x2E) != 8) {
            rnd = irand(10000);
            if (rnd < 2000) {
                id = (void*)0x9D;
            } else if (rnd < 4000) {
                id = (void*)0x9E;
            } else if (rnd < 6000) {
                id = (void*)0x9F;
            } else if (rnd < 8000) {
                id = (void*)0xA0;
            } else {
                id = (void*)0xA1;
            }
            psndSFXOn_3D(id, (void*)((s32)mario + 0x8C));
        }
        psndSFXOn_3D((void*)0x14D, (void*)((s32)mario + 0x8C));
        if (*(s16*)((s32)mario + 0x50) == 0) {
            *(VecData*)((s32)mario + 0xD4) = *(VecData*)((s32)mario + 0x8C);
        }
        *(u32*)((s32)mario + 0xC) &= ~0x10000;
    }
    switch (*(s32*)((s32)mario + 0x44)) {
        case 0:
            if (--*(s32*)((s32)mario + 0x48) <= 0) {
                if ((*(u32*)mario & 0x2000) == 0 && *(s8*)((s32)mario + 0x3C) == 0) {
                    marioChgPose(str_M_J_1B_802c40c8);
                }
                *(s32*)((s32)mario + 0x44) = 10;
            }
            break;
    }
    *(s16*)((s32)mario + 0x50) += 1;
}
#pragma opt_common_subs reset
#pragma use_lmw_stmw reset

void mot_upstairs(void) {
    extern char str_M_J_1B_802c40c8;
    extern void marioClearJumpPara(void);
    extern void motSlitContinue(void);
    extern void marioChgMot2(s32 motion);
    extern void marioChgMotAuto(void);
    void* mario = marioGetPtr();
    u32 flags = *(u32*)((s32)mario + 0xC);
    if (flags & 1) {
        *(u32*)((s32)mario + 0xC) = flags & ~1;
        *(u32*)((s32)mario + 0x0) |= 0x40000;
        *(u32*)((s32)mario + 0x0) |= 0x80;
        marioClearJumpPara();
        *(s32*)((s32)mario + 0x48) = 3;
        *(f32*)((s32)mario + 0x7C) = (*(f32*)((s32)mario + 0x1C8) - *(f32*)((s32)mario + 0x90)) / (f32)*(s32*)((s32)mario + 0x48);
        *(s32*)((s32)mario + 0x44) = 0;
    }
    switch (*(s32*)((s32)mario + 0x44)) {
        case 0:
            break;
        default:
            return;
    }
    *(f32*)((s32)mario + 0x90) += *(f32*)((s32)mario + 0x7C);
    if (--*(s32*)((s32)mario + 0x48) <= 0) {
            *(u32*)((s32)mario + 0x0) &= ~0x80;
            *(u32*)((s32)mario + 0x0) &= ~0x40;
            *(u32*)((s32)mario + 0x0) &= ~0x40000;
            if (*(u32*)((s32)mario + 0x0) & 0x100000) {
                motSlitContinue();
            } else if (*(u32*)((s32)mario + 0x4) & 0x1000000) {
                marioChgMot2(0x15);
            } else {
                marioChgMotAuto();
            }
    }
}

#pragma use_lmw_stmw off
void mot_fall(void) {
    extern f32 float_0_80420cc8;
    extern char str_M_J_1C_802c40b8[];
    extern void psndSFXOff(s32);
    extern void kpaChgPose(char*, char*);
    extern s32 marioBgmodeChk(void);
    extern s32 marioPriCheckJabara(void);
    extern f32 marioGetCamFollowRate(void);
    extern void marioSetCamFollowRate(f32);
    extern f32 float_0p1_80420ccc;
    extern f32 float_0p8_80420cd0;
    extern const VecData vec3_802c4088;

    char* poseBase = (char*)&vec3_802c4088;
    void* mario = marioGetPtr();
    if (*(s8*)((s32)mario + 0x3C) == 2) {
        kpa_fall();
        return;
    }
    if ((*(u32*)((s32)mario + 0xC) & 1) != 0) {
        s32 sound;
        *(u32*)((s32)mario + 0xC) &= ~1;
        *(u32*)mario &= ~0x10000;
        *(u32*)mario |= 0x20000;
        *(u32*)((s32)mario + 0xC) &= ~0x200000;
        *(u32*)((s32)mario + 0xC) &= ~0x400000;
        sound = *(s32*)((s32)mario + 0x284);
        if ((u32)(sound + 0x10000) != 0xFFFF) {
            psndSFXOff(sound);
            *(s32*)((s32)mario + 0x284) = -1;
        }
        marioResetCamFollowRate();
        *(s32*)((s32)mario + 0x44) = 0;
        if (*(u16*)((s32)mario + 0x2E) != 0xB) {
            if (*(s8*)((s32)mario + 0x3C) == 2) {
                kpaChgPose(poseBase + 0x18, poseBase + 0x24);
            } else if ((*(u32*)mario & 0x2000) == 0 && *(s8*)((s32)mario + 0x3C) == 0) {
                marioChgPose(poseBase + 0x30);
            }
        } else {
            *(u32*)mario |= 0x80;
        }
        if (*(s16*)((s32)mario + 0x50) == 0) {
            *(u32*)((s32)mario + 0xD4) = *(u32*)((s32)mario + 0x8C);
            *(u32*)((s32)mario + 0xD8) = *(u32*)((s32)mario + 0x90);
            *(u32*)((s32)mario + 0xDC) = *(u32*)((s32)mario + 0x94);
        }
    }
    if (*(s32*)((s32)mario + 0x44) == 0 &&
        *(f32*)((s32)mario + 0x90) <= *(f32*)((s32)mario + 0x120)) {
        if (marioBgmodeChk() == 0) {
            camFollowYOn();
        }
        *(s32*)((s32)mario + 0x44) = 10;
    }
    *(s16*)((s32)mario + 0x50) += 1;
    *(u32*)((s32)mario + 0xC) &= ~0x100000;
    if (marioPriCheckJabara() != 0 && float_0_80420cc8 == *(f32*)((s32)mario + 0x190) &&
        *(s16*)((s32)mario + 0x52) != 0 && *(s16*)((s32)mario + 0x52) < *(s16*)((s32)mario + 0x50)) {
        if (*(s16*)((s32)mario + 0x50) < 0x12) {
            if (*(f32*)((s32)mario + 0x194) != float_0_80420cc8) {
                *(u32*)((s32)mario + 0xC) |= 0x200000;
            }
        } else {
            *(u32*)((s32)mario + 0xC) |= 0x100000;
            if ((*(u32*)((s32)mario + 0xC) & 0x200000) == 0 &&
                *(f32*)((s32)mario + 0x194) != float_0_80420cc8) {
                *(u32*)((s32)mario + 0xC) |= 0x400000;
            }
        }
    }
    if (*(s32*)((s32)mario + 0x16C) >= 0x1AE) {
        f32 rate = marioGetCamFollowRate();
        rate += float_0p1_80420ccc;
        if (rate >= float_0p8_80420cd0) {
            rate = float_0p8_80420cd0;
        }
        marioSetCamFollowRate(rate);
    }
}
#pragma use_lmw_stmw reset

void mot_fall_post(void) {
    void* mario = marioGetPtr();
    s32 sound = *(s32*)((s32)mario + 0x284);
    if ((u32)(sound + 0x10000) != 0xFFFF) {
        psndSFXOff(sound);
        *(s32*)((s32)mario + 0x284) = -1;
    }
}

void mot_land(void) {
    extern f32 float_0_80420cc8;
    extern void psndSFXOff(s32 id);
    extern void marioChgMot(s32 motion);
    void* mario = marioGetPtr();
    u32 flags = *(u32*)((s32)mario + 0xC);
    if (flags & 1) {
        s32 sound;
        *(u32*)((s32)mario + 0xC) = flags & ~1;
        {
            f32 zero = float_0_80420cc8;
        *(u32*)((s32)mario + 0x0) &= ~0xF0000;
        *(s16*)((s32)mario + 0x50) = 0;
        *(s16*)((s32)mario + 0x52) = 0;
        *(s32*)((s32)mario + 0x44) = 0;
        *(f32*)((s32)mario + 0x180) = zero;
        }
        marioResetCamFollowRate();
        sound = *(s32*)((s32)mario + 0x284);
        if ((u32)(sound + 0x10000) != 0xFFFF) {
            psndSFXOff(sound);
            *(s32*)((s32)mario + 0x284) = -1;
        }
    }
    switch (*(s32*)((s32)mario + 0x44)) {
        case 0:
            marioChgMot(0);
            break;
        case 1:
            if (--*(s32*)((s32)mario + 0x48) <= 0) {
                marioChgMot(0);
            }
            break;
        case 10:
            if (--*(s32*)((s32)mario + 0x48) <= 0) {
                marioChgMot(0);
            }
            break;
    }
}

const VecData vec3_802c4088 = { 0.0f, 0.0f, 0.0f };
const VecData vec3_802c4094 = { 0.0f, 0.0f, 0.0f };
const char str_KPA_J_1B_802c40a0[] = "KPA_J_1B";
const char str_KPA2_J_1B_802c40ac[] = "KPA2_J_1B";
const char str_M_J_1C_802c40b8[] = "M_J_1C";
const char str_M_J_1B_802c40c8[] = "M_J_1B";
const char str_gon_10_802c40d0[] = "gon_10";

f32 marioJumpStandData[] = {
    1.52f, -0.188f, 2.925f, -0.847f, -0.1f,
    -0.4f, 0.024f, -0.004f, 0.125f, 0.7f,
};

f32 marioJumpSwData[] = {
    2.5f, -0.1f, 0.02f, -0.01f,
    -0.08f, -0.4f, 0.0f, -0.002f,
};

const f32 float_0_80420cc8 = 0.0f;
const f32 float_0p1_80420ccc = 0.1f;
const f32 float_0p8_80420cd0 = 0.8f;
const f32 float_2p6_80420cd4 = 2.6f;
const f32 float_neg0p06_80420cd8 = -0.06f;
const f32 float_neg0p01_80420cdc = -0.01f;
const f32 float_neg0p002_80420ce0 = -0.002f;
const f32 float_1_80420ce4 = 1.0f;
const f32 float_neg0p04_80420ce8 = -0.04f;
const f32 float_neg0p02_80420cec = -0.02f;
const f32 float_0p5_80420cf0 = 0.5f;
const f32 float_5_80420cf4 = 5.0f;
const f32 float_30_80420cf8 = 30.0f;
const f32 float_180_80420cfc = 180.0f;
const f32 float_neg0p03_80420d00 = -0.03f;
const f32 float_0p04_80420d04 = 0.04f;
const f32 float_neg1_80420d08 = -1.0f;
