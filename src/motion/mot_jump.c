#include "motion/mot_jump.h"

extern void* gp;
extern char str_gon_10_802c40d0[];

void* marioGetPtr(void);
void psndSFXOff(s32 id);
s32 strcmp(const char* a, const char* b);
void camFollowYOn(void);

f32 marioGetJumpHeight(void) {
    void* mario = marioGetPtr();
    return *(f32*)((s32)mario + 0x2D0);
}

void mot_fall_post(void) {
    void* mario = marioGetPtr();
    s32 sound = *(s32*)((s32)mario + 0x284);
    if ((u32)(sound + 0x10000) != 0xFFFF) {
        psndSFXOff(sound);
        *(s32*)((s32)mario + 0x284) = -1;
    }
}

void N_mot_jumpNpc_post(void) {
    void* mario = marioGetPtr();
    if (strcmp((char*)((s32)gp + 0x12C), str_gon_10_802c40d0) == 0) {
        if (*(u32*)((s32)mario + 4) & 0x20000) {
            camFollowYOn();
        }
    }
}


void mot_jumpNpc(void) {
    extern void* marioGetPtr(void);
    extern void kpa_jumpNpc(void);
    extern void marioResetCamFollowRate(void);
    extern void camFollowYOff(void);
    extern void clear_hitobj_ride(void);
    extern void marioSetJumpPara(void);
    extern void marioMakeJumpPara(void);
    extern void marioSetPeakYpos(void);
    extern void marioChgPose(char*);
    extern u32 hitGetAttr(void*);
    extern void* mobjHitObjPtrToPtr(s32);
    extern s32 marioChkFront(f32, f32, f32*, void*);
    extern f32 revise360(f32);
    extern void sincosf(f32, f32*, f32*);
    extern f64 distABf(f64, f64, f64, f64);
    extern f32 float_0_80420cc8;
    extern f32 float_2p6_80420cd4;
    extern f32 float_neg0p06_80420cd8;
    extern f32 float_neg0p01_80420cdc;
    extern f32 float_neg0p002_80420ce0;
    extern f32 float_1_80420ce4;
    extern f32 float_neg0p04_80420ce8;
    extern f32 float_neg0p02_80420cec;
    extern f32 float_0p5_80420cf0;
    extern f32 float_5_80420cf4;
    extern f32 float_30_80420cf8;
    extern f32 float_180_80420cfc;
    extern f32 float_neg0p03_80420d00;
    extern f32 float_0p04_80420d04;
    extern char str_M_J_1B_802c40c8;
    extern char str_M_J_1C_802c40b8;

    typedef struct Vec { f32 x, y, z; } Vec;

    void* mario = marioGetPtr();
    void* work;
    Vec pos;
    Vec hitPos;
    f32 dist;
    f32 sx;
    f32 sz;
    f32 temp;

    if (*(s8*)((s32)mario + 0x3C) == 2) {
        kpa_jumpNpc();
        return;
    }

    if ((*(u32*)((s32)mario + 0xC) & 1) != 0) {
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
        *(u32*)((s32)work + 0x11C) = *(u32*)((s32)work + 0x8C);
        *(u32*)((s32)work + 0x120) = *(u32*)((s32)work + 0x90);
        *(u32*)((s32)work + 0x124) = *(u32*)((s32)work + 0x94);
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
        if ((*(u32*)((s32)mario + 0x0) & 0x10000000) == 0 && *(s8*)((s32)mario + 0x3C) == 0) {
            marioChgPose(&str_M_J_1B_802c40c8);
        }

        if (*(void**)((s32)mario + 0x1EC) == 0) {
            temp = revise360(float_180_80420cfc + *(f32*)((s32)mario + 0x1A4));
            *(f32*)((s32)mario + 0x1A4) = temp;
            *(f32*)((s32)mario + 0x1A0) = temp;
            *(s32*)((s32)mario + 0x48) = 0x14;
            sincosf(temp, &sx, &sz);
            dist = (f32)distABf(*(f32*)((s32)mario + 0x8C), *(f32*)((s32)mario + 0x94),
                                sx * float_30_80420cf8 + *(f32*)((s32)mario + 0x8C),
                                sz * float_30_80420cf8 + *(f32*)((s32)mario + 0x94));
            *(f32*)((s32)mario + 0x180) = dist / (f32)*(s32*)((s32)mario + 0x48);
        } else if ((hitGetAttr(*(void**)((s32)mario + 0x1EC)) & 1) != 0) {
            work = mobjHitObjPtrToPtr((s32)*(void**)((s32)mario + 0x1EC));
            *(f32*)((s32)mario + 0x2CC) = *(f32*)((s32)work + 0x38);
            *(f32*)((s32)mario + 0x2D0) = *(f32*)((s32)work + 0x40);
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
            sx = sx * float_30_80420cf8;
            *(s32*)((s32)mario + 0x48) = 0x14;
            dist = (f32)distABf(*(f32*)((s32)mario + 0x8C), *(f32*)((s32)mario + 0x94),
                                sx + *(f32*)((s32)work + 0x38),
                                sz * float_30_80420cf8 + *(f32*)((s32)work + 0x40));
            *(f32*)((s32)mario + 0x180) = dist / (f32)*(s32*)((s32)mario + 0x48);
        }
        *(s32*)((s32)mario + 0x44) = 0;
    }

    if (*(s32*)((s32)mario + 0x44) == 0x14) {
        *(s32*)((s32)mario + 0x48) -= 1;
        if (*(s32*)((s32)mario + 0x48) < 1) {
            *(f32*)((s32)mario + 0x180) = float_0_80420cc8;
            *(s32*)((s32)mario + 0x44) = 0x1E;
        }
    } else if (*(s32*)((s32)mario + 0x44) == 0) {
        *(s32*)((s32)mario + 0x48) -= 1;
        marioMakeJumpPara();
        if (*(f32*)((s32)mario + 0x7C) <= float_0_80420cc8) {
            if ((*(u32*)((s32)mario + 0x0) & 0x10000000) == 0 && *(s8*)((s32)mario + 0x3C) == 0) {
                marioChgPose(&str_M_J_1C_802c40b8);
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
    }
    *(s16*)((s32)mario + 0x50) += 1;
}

void mot_jumpSw(void) {
    extern void* marioGetPtr(void);
    extern void marioResetCamFollowRate(void);
    extern void camFollowYOff(void);
    extern void clear_hitobj_ride(void);
    extern void marioSetJumpPara(void);
    extern void marioMakeJumpPara(void);
    extern void marioSetPeakYpos(void);
    extern void marioChgPose(char*);
    extern u32 hitGetAttr(void*);
    extern void* mobjHitObjPtrToPtr(s32);
    extern s32 marioChkFront(f32, f32, f32*, void*);
    extern f32 revise360(f32);
    extern void sincosf(f32, f32*, f32*);
    extern f64 distABf(f64, f64, f64, f64);
    extern f32 marioJumpSwData[];
    extern f32 float_0_80420cc8;
    extern f32 float_0p5_80420cf0;
    extern f32 float_5_80420cf4;
    extern f32 float_30_80420cf8;
    extern f32 float_180_80420cfc;
    extern char str_M_J_1B_802c40c8;
    extern char str_M_J_1C_802c40b8;

    typedef struct Vec { f32 x, y, z; } Vec;

    void* mario = marioGetPtr();
    void* work;
    Vec pos;
    Vec hitPos;
    f32 dist;
    f32 sx;
    f32 sz;
    f32 dir;

    if ((*(u32*)((s32)mario + 0xC) & 1) != 0) {
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
        *(u32*)((s32)work + 0x11C) = *(u32*)((s32)work + 0x8C);
        *(u32*)((s32)work + 0x120) = *(u32*)((s32)work + 0x90);
        *(u32*)((s32)work + 0x124) = *(u32*)((s32)work + 0x94);
        *(f32*)((s32)work + 0x190) = *(f32*)((s32)work + 0x180);

        if ((*(u32*)((s32)mario + 0x0) & 0x10000000) == 0 && *(s8*)((s32)mario + 0x3C) == 0) {
            marioChgPose(&str_M_J_1B_802c40c8);
        }
        if (*(void**)((s32)mario + 0x1EC) == 0) {
            dir = revise360(float_180_80420cfc + *(f32*)((s32)mario + 0x1A4));
            sincosf(dir, &sx, &sz);
            *(f32*)((s32)mario + 0x1A4) = dir;
            *(f32*)((s32)mario + 0x1A0) = dir;
            *(s32*)((s32)mario + 0x48) = 0x14;
            dist = (f32)distABf(*(f32*)((s32)mario + 0x8C), *(f32*)((s32)mario + 0x94),
                                sx * float_30_80420cf8 + *(f32*)((s32)mario + 0x8C),
                                sz * float_30_80420cf8 + *(f32*)((s32)mario + 0x94));
            *(f32*)((s32)mario + 0x180) = dist / (f32)*(s32*)((s32)mario + 0x48);
        } else if ((hitGetAttr(*(void**)((s32)mario + 0x1EC)) & 1) != 0) {
            work = mobjHitObjPtrToPtr((s32)*(void**)((s32)mario + 0x1EC));
            *(f32*)((s32)mario + 0x2CC) = *(f32*)((s32)work + 0x38);
            *(f32*)((s32)mario + 0x2D0) = *(f32*)((s32)work + 0x40);
            pos.x = *(f32*)((s32)work + 0x38);
            pos.y = *(f32*)((s32)work + 0x3C) + *(f32*)((s32)mario + 0x1BC) * float_0p5_80420cf0;
            pos.z = *(f32*)((s32)work + 0x40);
            hitPos = pos;
            if (marioChkFront(*(f32*)((s32)mario + 0x1A4),
                              *(f32*)((s32)mario + 0x1B8) * float_5_80420cf4,
                              &dist, &hitPos) != 0 &&
                dist >= float_0_80420cc8 &&
                dist <= (*(f32*)((s32)mario + 0x1B8) * float_0p5_80420cf0 + float_30_80420cf8)) {
                dir = revise360(float_180_80420cfc + *(f32*)((s32)mario + 0x1A4));
                *(f32*)((s32)mario + 0x1A4) = dir;
                *(f32*)((s32)mario + 0x1A0) = dir;
            }
            sincosf(*(f32*)((s32)mario + 0x1A4), &sx, &sz);
            sx *= float_30_80420cf8;
            *(s32*)((s32)mario + 0x48) = 0x14;
            dist = (f32)distABf(*(f32*)((s32)mario + 0x8C), *(f32*)((s32)mario + 0x94),
                                sx + *(f32*)((s32)work + 0x38),
                                sz * float_30_80420cf8 + *(f32*)((s32)work + 0x40));
            *(f32*)((s32)mario + 0x180) = dist / (f32)*(s32*)((s32)mario + 0x48);
        }
        *(s32*)((s32)mario + 0x44) = 0;
    }

    if (*(s32*)((s32)mario + 0x44) == 0xA) {
        *(s32*)((s32)mario + 0x48) -= 1;
        if (*(s32*)((s32)mario + 0x48) < 1) {
            *(f32*)((s32)mario + 0x180) = float_0_80420cc8;
            *(s32*)((s32)mario + 0x44) = 0x14;
        }
    } else if (*(s32*)((s32)mario + 0x44) == 0) {
        *(s32*)((s32)mario + 0x48) -= 1;
        marioMakeJumpPara();
        if (*(f32*)((s32)mario + 0x7C) <= float_0_80420cc8) {
            if ((*(u32*)((s32)mario + 0x0) & 0x10000000) == 0 && *(s8*)((s32)mario + 0x3C) == 0) {
                marioChgPose(&str_M_J_1C_802c40b8);
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
    }
    *(s16*)((s32)mario + 0x50) += 1;
}

void mot_jumpStand(void) {
    extern void* marioGetPtr(void);
    extern void clear_hitobj_ride(void);
    extern void marioSetJumpPara(void);
    extern void marioMakeJumpPara(void);
    extern void marioSetPeakYpos(void);
    extern void marioChgPose(char*);
    extern s32 marioSlitButton(void);
    extern void marioSlitForceCancel(void);
    extern f32 marioJumpStandData[];
    extern f32 float_0_80420cc8;
    extern f32 float_neg1_80420d08;
    extern char str_M_J_1B_802c40c8;
    extern char str_M_J_1C_802c40b8;

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
        case 0:
            *(s32*)((s32)mario + 0x48) += 1;
            if (*(s32*)((s32)mario + 0x48) > 2) {
                *(s32*)((s32)mario + 0x44) = 4;
                *(f32*)((s32)mario + 0x7C) = marioJumpStandData[0];
                *(f32*)((s32)mario + 0x80) = marioJumpStandData[1];
                *(f32*)((s32)mario + 0x84) = marioJumpStandData[2];
                *(f32*)((s32)mario + 0x88) = marioJumpStandData[3];
                if ((*(u32*)((s32)mario + 0x0) & 0x10000000) == 0 && *(s8*)((s32)mario + 0x3C) == 0) {
                    marioChgPose(&str_M_J_1B_802c40c8);
                }
            }
            break;
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
                if ((*(u32*)((s32)mario + 0x0) & 0x10000000) == 0 && *(s8*)((s32)mario + 0x3C) == 0) {
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
                if ((*(u32*)((s32)mario + 0x0) & 0x10000000) == 0 && *(s8*)((s32)mario + 0x3C) == 0) {
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

void mot_fall(void) {
    extern void* marioGetPtr(void);
    extern void kpa_fall(void);
    extern void psndSFXOff(s32);
    extern void marioResetCamFollowRate(void);
    extern void kpaChgPose(char*, char*);
    extern void marioChgPose(char*);
    extern s32 marioBgmodeChk(void);
    extern void camFollowYOn(void);
    extern s32 marioPriCheckJabara(void);
    extern f32 marioGetCamFollowRate(void);
    extern void marioSetCamFollowRate(double);
    extern f32 float_0_80420cc8;
    extern f32 float_0p1_80420ccc;
    extern f32 float_0p8_80420cd0;
    extern char str_KPA_J_1B_802c40a0;
    extern char str_KPA2_J_1B_802c40ac;
    extern char str_M_J_1C_802c40b8;

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
        *(u32*)((s32)mario + 0xC) &= ~0x20000;
        *(u32*)((s32)mario + 0xC) &= ~0x100000;
        sound = *(s32*)((s32)mario + 0x284);
        if ((u32)(sound + 0x10000) != 0xFFFF) {
            psndSFXOff(sound);
            *(s32*)((s32)mario + 0x284) = -1;
        }
        marioResetCamFollowRate();
        *(s32*)((s32)mario + 0x44) = 0;
        if (*(u16*)((s32)mario + 0x2E) == 0x17) {
            *(u32*)mario |= 0x80;
        } else if (*(s8*)((s32)mario + 0x3C) == 2) {
            kpaChgPose(&str_KPA_J_1B_802c40a0, &str_KPA2_J_1B_802c40ac);
        } else if ((*(u32*)mario & 0x10000000) == 0 && *(s8*)((s32)mario + 0x3C) == 0) {
            marioChgPose(&str_M_J_1C_802c40b8);
        }
        if (*(s16*)((s32)mario + 0x50) == 0) {
            *(u32*)((s32)mario + 0xD4) = *(u32*)((s32)mario + 0x8C);
            *(u32*)((s32)mario + 0xD8) = *(u32*)((s32)mario + 0x90);
            *(u32*)((s32)mario + 0xDC) = *(u32*)((s32)mario + 0x94);
        }
    }
    if (*(s32*)((s32)mario + 0x44) == 0 &&
        *(f32*)((s32)mario + 0x90) <= *(f32*)((s32)mario + 0x11C)) {
        if (marioBgmodeChk() == 0) {
            camFollowYOn();
        }
        *(s32*)((s32)mario + 0x44) = 10;
    }
    *(s16*)((s32)mario + 0x50) += 1;
    *(u32*)((s32)mario + 0xC) &= ~0x40000;
    if (marioPriCheckJabara() != 0 && *(f32*)((s32)mario + 0x190) == float_0_80420cc8 &&
        *(s16*)((s32)mario + 0x52) != 0 && *(s16*)((s32)mario + 0x52) < *(s16*)((s32)mario + 0x50)) {
        if (*(s16*)((s32)mario + 0x50) < 0x12) {
            if (*(f32*)((s32)mario + 0x194) != float_0_80420cc8) {
                *(u32*)((s32)mario + 0xC) |= 0x20000;
            }
        } else {
            *(u32*)((s32)mario + 0xC) |= 0x40000;
            if ((*(u32*)((s32)mario + 0xC) & 0x20000) == 0 &&
                *(f32*)((s32)mario + 0x194) != float_0_80420cc8) {
                *(u32*)((s32)mario + 0xC) |= 0x100000;
            }
        }
    }
    if ((s32)*(f32*)((s32)mario + 0x16C) > 0x1AD) {
        f32 rate = marioGetCamFollowRate();
        rate += float_0p1_80420ccc;
        if (rate >= float_0p8_80420cd0) {
            rate = float_0p8_80420cd0;
        }
        marioSetCamFollowRate(rate);
    }
}

void mot_jump(void) {
    extern void* marioGetPtr(void);
    extern void kpa_jump(void);
    extern void marioResetCamFollowRate(void);
    extern void clear_hitobj_ride(void);
    extern void marioSetJumpPara(void);
    extern void camFollowYOn(void);
    extern void camFollowYOff(void);
    extern void marioChgPose(char*);
    extern s32 irand(s32);
    extern void psndSFXOn_3D(void*, void*);
    extern char str_M_J_1B_802c40c8;

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
        *(u32*)mario |= 0x40000;
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
        *(u32*)((s32)mario2 + 0x11C) = *(u32*)((s32)mario2 + 0x8C);
        *(u32*)((s32)mario2 + 0x120) = *(u32*)((s32)mario2 + 0x90);
        *(u32*)((s32)mario2 + 0x124) = *(u32*)((s32)mario2 + 0x94);
        *(f32*)((s32)mario2 + 0x190) = *(f32*)((s32)mario2 + 0x180);
        if (*(u16*)((s32)mario + 0x2E) == 0x16) {
            camFollowYOn();
        } else {
            camFollowYOff();
        }
        if ((*(u32*)mario & 0x10000000) == 0 && *(s8*)((s32)mario + 0x3C) == 0) {
            marioChgPose(&str_M_J_1B_802c40c8);
        }
        if (*(u16*)((s32)mario + 0x2E) != 0x16) {
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
            *(u32*)((s32)mario + 0xD4) = *(u32*)((s32)mario + 0x8C);
            *(u32*)((s32)mario + 0xD8) = *(u32*)((s32)mario + 0x90);
            *(u32*)((s32)mario + 0xDC) = *(u32*)((s32)mario + 0x94);
        }
        *(u32*)((s32)mario + 0xC) &= ~0x10000;
    }
    if (*(s32*)((s32)mario + 0x44) == 0 && --*(s32*)((s32)mario + 0x48) < 1) {
        if ((*(u32*)mario & 0x10000000) == 0 && *(s8*)((s32)mario + 0x3C) == 0) {
            marioChgPose(&str_M_J_1B_802c40c8);
        }
        *(s32*)((s32)mario + 0x44) = 10;
    }
    *(s16*)((s32)mario + 0x50) += 1;
}

void mot_jumpSmall(void) {
    extern void* marioGetPtr(void);
    extern void kpa_jump(void);
    extern void marioResetCamFollowRate(void);
    extern void clear_hitobj_ride(void);
    extern void marioSetJumpPara(void);
    extern void camFollowYOff(void);
    extern void marioChgPose(char*);
    extern s32 irand(s32);
    extern void psndSFXOn_3D(void*, void*);
    extern char str_M_J_1B_802c40c8;

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
        *(u32*)mario |= 0x40000;
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
        *(u32*)((s32)mario2 + 0x11C) = *(u32*)((s32)mario2 + 0x8C);
        *(u32*)((s32)mario2 + 0x120) = *(u32*)((s32)mario2 + 0x90);
        *(u32*)((s32)mario2 + 0x124) = *(u32*)((s32)mario2 + 0x94);
        *(f32*)((s32)mario2 + 0x190) = *(f32*)((s32)mario2 + 0x180);
        if (*(u16*)((s32)mario + 0x2E) != 0x16) {
            camFollowYOff();
        }
        if ((*(u32*)mario & 0x10000000) == 0 && *(s8*)((s32)mario + 0x3C) == 0) {
            marioChgPose(&str_M_J_1B_802c40c8);
        }
        if (*(u16*)((s32)mario + 0x2E) != 0x16) {
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
            *(u32*)((s32)mario + 0xD4) = *(u32*)((s32)mario + 0x8C);
            *(u32*)((s32)mario + 0xD8) = *(u32*)((s32)mario + 0x90);
            *(u32*)((s32)mario + 0xDC) = *(u32*)((s32)mario + 0x94);
        }
        *(u32*)((s32)mario + 0xC) &= ~0x10000;
    }
    if (*(s32*)((s32)mario + 0x44) == 0 && --*(s32*)((s32)mario + 0x48) < 1) {
        if ((*(u32*)mario & 0x10000000) == 0 && *(s8*)((s32)mario + 0x3C) == 0) {
            marioChgPose(&str_M_J_1B_802c40c8);
        }
        *(s32*)((s32)mario + 0x44) = 10;
    }
    *(s16*)((s32)mario + 0x50) += 1;
}

void mot_upstairs(void) {
    extern void* marioGetPtr(void);
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

void mot_land(void) {
    extern void* marioGetPtr(void);
    extern void marioResetCamFollowRate(void);
    extern void psndSFXOff(s32 id);
    extern void marioChgMot(s32 motion);
    extern f32 float_0_80420cc8;
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

u8 marioPreJump(void) {
    extern void clear_hitobj_ride(void);
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
