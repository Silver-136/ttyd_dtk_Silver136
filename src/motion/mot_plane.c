#include "motion/mot_plane.h"

s32 marioPlaneGetFlyingDir(void) {
    extern void* marioGetPtr(void);

    void* mario = marioGetPtr();
    if (*(u16*)((s32)mario + 0x2E) != 0x18) {
        return 0;
    }

    if (*(u32*)*(s32*)((s32)mario + 0x294) & 1) {
        return 1;
    }
    return -1;
}

s32 marioGetPlaneStatus(void) {
    extern void* marioGetPtr(void);

    void* mario = marioGetPtr();
    s32 status;

    if (*(u16*)((s32)mario + 0x2E) != 0x18) {
        return 0;
    }

    status = *(s32*)((s32)mario + 0x44);
    if (status >= 0xC && status < 0x14) {
        return 2;
    }
    return 1;
}


u8 mot_plane(void) {
    extern void* marioGetPtr(void);
    extern void* __memAlloc(s32, s32);
    extern void* memset(void*, s32, u32);
    extern void marioPreJump(void);
    extern void marioChgPose(char*);
    extern void marioMakeJumpPara(void);
    extern s32 marioGetColor(void);
    extern void marioPaperOn(char*);
    extern void marioChgPaper(char*);
    extern void allPartyRidePlane(void);
    extern u32 psndSFXOn_3D(s32, void*);
    extern u32 hitGetAttr(void*);
    extern void flyMain(void);
    extern char* paper_plane[];
    extern char str_M_J_1B_802c4170[];
    extern char str_PM_P_1A_802c4178[];
    extern char str_PM_P_1B_802c4180[];
    void* player = marioGetPtr();
    void* work;
    s32 state;
    f32 direction;
    f32 target;

    if ((*(u32*)((s32)player + 4) & 0x1000) != 0) {
        *(u32*)((s32)player + 4) &= ~0x1000;
        work = __memAlloc(0, 0x48);
        *(void**)((s32)player + 0x234) = work;
        memset(work, 0, 0x48);
        *(s32*)((s32)work + 0x28) = -1;
        *(s32*)((s32)player + 0x44) = 0;
    }
    work = *(void**)((s32)player + 0x234);
    state = *(s32*)((s32)player + 0x44);
    if (state == 0) {
        marioPreJump();
        marioChgPose(str_M_J_1B_802c4170);
        *(u32*)((s32)player + 8) |= 0x400;
        *(s32*)((s32)player + 0x44) = 1;
        *(s32*)((s32)player + 0x48) = 0;
        *(f32*)((s32)player + 0x180) = 0.0f;
        *(f32*)((s32)player + 0x178) = 0.0f;
        *(f32*)((s32)player + 0x17C) = *(f32*)((s32)player + 0x90);
        *(u32*)work = (hitGetAttr(*(void**)((s32)player + 0x1E8)) >> 4) & 1;
        psndSFXOn_3D(0x17D, (void*)((s32)player + 0x8C));
        psndSFXOn_3D(0xAF, (void*)((s32)player + 0x8C));
        state = 1;
    }
    if (state == 1) {
        marioMakeJumpPara();
        target = (*(u32*)work & 1) ? 30.0f : -30.0f;
        direction = *(f32*)((s32)player + 0x178);
        direction += 0.125f * (target - direction);
        *(f32*)((s32)player + 0x178) = direction;
        *(f32*)((s32)player + 0x1AC) += direction;
        if (((*(u32*)work & 1) == 0 && *(f32*)((s32)player + 0x1AC) <= -630.0f) ||
            ((*(u32*)work & 1) != 0 && *(f32*)((s32)player + 0x1AC) >= 630.0f)) {
            *(s32*)((s32)player + 0x44) = 2;
            *(s32*)((s32)player + 0x170) = 20;
            marioPaperOn(paper_plane[marioGetColor()]);
            marioChgPaper(str_PM_P_1A_802c4178);
        }
    } else if (state == 2) {
        target = (*(u32*)work & 1) ? 30.0f : -30.0f;
        direction = *(f32*)((s32)player + 0x178);
        direction += 0.125f * (target - direction);
        *(f32*)((s32)player + 0x178) = direction;
        *(f32*)((s32)player + 0x1AC) += direction;
        if (((*(u32*)work & 1) == 0 && *(f32*)((s32)player + 0x1AC) <= -1080.0f) ||
            ((*(u32*)work & 1) != 0 && *(f32*)((s32)player + 0x1AC) >= 1260.0f)) {
            *(s32*)((s32)player + 0x170) = 60;
            *(s32*)((s32)player + 0x44) = 3;
        }
    } else if (state == 3) {
        if (--*(s32*)((s32)player + 0x170) == 40) allPartyRidePlane();
        if (*(s32*)((s32)player + 0x170) < 1) {
            marioChgPaper(str_PM_P_1B_802c4180);
            *(s32*)((s32)player + 0x44) = 12;
            *(f32*)((s32)player + 0x180) = 1.0f;
        }
    }
    flyMain();
    return 0;
}

void flyMain(void) {
    extern void* marioGetPtr(void);
    extern f64 revise360(f64 angle);
    extern f64 sin(f64 angle);
    extern void sincosf(f32 angle, f32* sinOut, f32* cosOut);
    extern u32 psndSFXOn_3D(s32 id, void* position);
    extern void* marioSearchUnder(void);
    extern s32 chkCollision(s32* attr);
    extern u32 hitGetAttr(void* hit);
    extern s32 sysMsec2Frame(s32 msec);
    extern f32 planeData[];
    extern f32 float_0_80420d1c;
    extern f32 float_0p1_80420d40;
    extern f32 float_0p5_80420d5c;
    extern f32 float_1_80420d2c;
    extern f32 float_2_80420d28;
    extern f32 float_6_80420d60;
    extern f32 float_60_80420d3c;
    extern f32 float_90_80420d14;
    extern f32 float_150_80420d44;
    extern f32 float_180_80420d54;
    extern f32 float_210_80420d4c;
    extern f32 float_270_80420d30;
    extern f32 float_330_80420d48;
    extern f32 float_3p1416_80420d50;
    extern f32 float_neg0p001_80420d58;
    extern f32 float_neg22_80420d68;

    void* player = marioGetPtr();
    f32* work = *(f32**)((s32)player + 0x234);
    f32 sinDir;
    f32 cosDir;
    f32 sinPitch;
    f32 cosPitch;
    f32 pitch;
    f32 speed;
    f32 lift;
    f32 input;
    void* hit;
    s32 attr;

#define SUBMOTION (*(s32*)((s32)player + 0x44))
#define BUTTONS (*(u32*)((s32)player + 0x14))
#define STICK (*(s8*)((s32)player + 0x3A))
#define POS ((f32*)((s32)player + 0x8C))
#define MODEL_Y (*(f32*)((s32)player + 0x120))
#define BASE_SPEED (*(f32*)((s32)player + 0x180))
#define DISP_DIR (*(f32*)((s32)player + 0x1AC))
#define VIEW_DIR (*(f32*)((s32)player + 0x1A4))

    if (SUBMOTION == 12) {
        if (POS[1] <= *(f32*)((s32)player + 0x16C)) {
            SUBMOTION = 14;
        }
        if (*(s32*)((s32)work + 0x28) == -1) {
            *(s32*)((s32)work + 0x28) = psndSFXOn_3D(0x17F, POS);
        }
    }
    if (SUBMOTION == 13 || SUBMOTION == 14) {
        if (MODEL_Y <= *(f32*)0x803104C0) {
            SUBMOTION = 15;
        } else {
            MODEL_Y -= float_2_80420d28;
            *(f32*)((s32)player + 0x104) += float_2_80420d28;
            POS[1] += float_2_80420d28;
        }
    }
    if (SUBMOTION != 15) {
        return;
    }

    if ((BUTTONS & 0x300) != 0) {
        SUBMOTION = 51;
        *(f32*)((s32)player + 0x174) = POS[1];
        return;
    }

    work[2] = (f32)revise360(work[3] * *(f32*)0x80310460 + work[2]);
    if ((((u32*)work)[0] & 1) == 0) {
        if (work[2] < float_210_80420d4c) {
            work[2] += float_0p1_80420d40 * (float_210_80420d4c - work[2]);
        }
        if (work[1] < float_330_80420d48) {
            work[1] += float_0p1_80420d40 * (float_330_80420d48 - work[1]);
        }
    } else {
        if (work[2] > float_150_80420d44) {
            work[2] += float_0p1_80420d40 * (float_150_80420d44 - work[2]);
        }
        if (work[1] < float_60_80420d3c) {
            work[1] += float_0p1_80420d40 * (float_60_80420d3c - work[1]);
        }
    }

    pitch = work[2];
    if (pitch >= float_90_80420d14 && pitch <= float_270_80420d30) {
        DISP_DIR = (f32)revise360(-pitch - float_90_80420d14);
    } else {
        DISP_DIR = (f32)revise360(float_270_80420d30 - pitch);
    }

    input = (f32)STICK;
    if (STICK == 0) {
        speed = planeData[0] - BASE_SPEED;
        if (speed < float_0_80420d1c) speed = float_0_80420d1c;
        lift = (f32)sin(float_3p1416_80420d50 * speed *
                        (float_90_80420d14 / planeData[0]) / float_180_80420d54);
        work[7] = lift;
        work[1] += lift;
        if (work[1] < *(f32*)0x80310438) work[1] = *(f32*)0x80310438;
        if (work[1] >= float_6_80420d60) work[5] = float_0_80420d1c;
    } else if (BASE_SPEED != float_0_80420d1c) {
        if ((((u32*)work)[0] & 1) == 0) {
            if (STICK > 0) work[1] += float_0p1_80420d40 * input * *(f32*)0x8031043C;
            if (STICK < 0) work[1] -= float_0p1_80420d40 * -input * *(f32*)0x80310440;
        } else {
            if (STICK < 0) work[1] += float_0p1_80420d40 * -input * *(f32*)0x8031043C;
            if (STICK > 0) work[1] -= float_0p1_80420d40 * input * *(f32*)0x80310440;
        }
    }

    BASE_SPEED += BASE_SPEED * *(f32*)0x8031042C;
    if (work[1] >= float_0_80420d1c) {
        ((u32*)work)[0] |= 0x10;
        BASE_SPEED += BASE_SPEED * *(f32*)0x80310448 *
                      (f32)sin(float_3p1416_80420d50 * work[1] * *(f32*)0x80310444 /
                               float_180_80420d54);
    } else {
        ((u32*)work)[0] &= ~0x10;
        work[4] = *(f32*)0x80310450 *
                  (f32)sin(float_3p1416_80420d50 * *(f32*)0x8031044C * -work[1] /
                           float_180_80420d54);
        BASE_SPEED += work[4];
        work[9] = float_neg22_80420d68;
    }
    if (BASE_SPEED < float_0_80420d1c) BASE_SPEED = float_0_80420d1c;
    if (BASE_SPEED > planeData[0]) BASE_SPEED = planeData[0];

    POS[1] += work[6] - *(f32*)0x8031049C;
    *(f32*)((s32)player + 0x10C) = (f32)revise360(-work[1]);
    VIEW_DIR = work[2];
    sincosf(VIEW_DIR, &sinDir, &cosDir);
    POS[2] += BASE_SPEED * cosDir;
    sincosf(float_90_80420d14 + work[1], &sinPitch, &cosPitch);
    POS[1] += BASE_SPEED * cosPitch * (cosPitch >= float_0_80420d1c ? *(f32*)0x803104A4 : *(f32*)0x803104A8);
    POS[0] += sinPitch * BASE_SPEED * sinDir;

    hit = marioSearchUnder();
    *(void**)((s32)player + 0x1F4) = hit;
    if (chkCollision(&attr) != 0) {
        if (*(void**)((s32)player + 0x1F0) != 0) {
            SUBMOTION = 40;
        } else {
            SUBMOTION = 50;
            hit = *(void**)((s32)player + 0x200);
            if (hit == 0) hit = *(void**)((s32)player + 0x1E8);
            if (hit != 0 && (hitGetAttr(hit) & 0x200) != 0) SUBMOTION = 70;
        }
    }

#undef VIEW_DIR
#undef DISP_DIR
#undef BASE_SPEED
#undef MODEL_Y
#undef POS
#undef STICK
#undef BUTTONS
#undef SUBMOTION
}

s32 chkCollision(s32* outAttr) {
    extern void* marioGetPtr(void);
    extern void sincosf(f32, f32*, f32*);
    extern void* marioHitCheck(f64, f64, f64, f64, f64, f64, void*, s32*, void*, f32*, void*, void*, void*);
    extern void PSVECSubtract(void*, void*, void*);
    extern f32 PSVECMag(void*);
    extern void PSVECNormalize(void*, void*);
    extern void* marioHitCheckVec(void*, void*, f32*, void*, f32*);
    extern f32 planeData[];
    extern f32 float_0_80420d1c;
    extern f32 float_1_80420d2c;
    extern f32 float_2_80420d28;
    extern f32 float_4_80420d10;
    extern f32 float_10_80420d18;
    extern f32 float_10p1_80420d20;
    extern f32 float_90_80420d14;
    extern f32 float_neg1_80420d24;

    typedef struct Vec { f32 x, y, z; } Vec;

    void* mario;
    void* plane;
    void* hit;
    Vec start;
    Vec end;
    Vec delta;
    f32 sx1, cx1;
    f32 sx2, cx2;
    f32 sx3, cx3;
    f32 radius;
    f32 scratchA[3];
    f32 scratchB[3];
    f32 scratchC[3];
    s32 attr;
    s32 hitAny;
    f32 x0;
    f32 y0;
    f32 z0;
    f32 px;
    f32 py;
    f32 pz;
    f32 f28;

    mario = marioGetPtr();
    plane = *(void**)((s32)mario + 0x294);
    hitAny = 0;

    sincosf(float_90_80420d14 + *(f32*)((s32)plane + 4), &sx1, &cx1);
    sincosf(*(f32*)((s32)plane + 8), &sx2, &cx2);
    end.x = float_4_80420d10 * sx2 * cx1 + *(f32*)((s32)mario + 0x8C) +
            *(f32*)((s32)mario + 0x98) + *(f32*)((s32)mario + 0xB0);
    end.y = -sx1 * float_4_80420d10 + *(f32*)((s32)mario + 0x90) +
            *(f32*)((s32)mario + 0x9C) + *(f32*)((s32)mario + 0xB4);
    end.z = float_4_80420d10 * cx2 * cx1 + *(f32*)((s32)mario + 0x94) +
            *(f32*)((s32)mario + 0xA0) + *(f32*)((s32)mario + 0xB8);
    *(Vec*)((s32)plane + 0x3C) = end;

    sincosf(*(f32*)((s32)plane + 0xC), &sx3, &cx3);
    sincosf(*(f32*)((s32)plane + 4), &sx1, &cx1);
    *(void**)((s32)mario + 0x1E4) = 0;
    sincosf(*(f32*)((s32)plane + 8), &sx2, &cx2);
    px = *(f32*)((s32)mario + 0x8C) + *(f32*)((s32)mario + 0x98) + *(f32*)((s32)mario + 0xB0);
    py = *(f32*)((s32)mario + 0x90) + *(f32*)((s32)mario + 0x9C) + *(f32*)((s32)mario + 0xB4);
    pz = *(f32*)((s32)mario + 0x94) + *(f32*)((s32)mario + 0xA0) + *(f32*)((s32)mario + 0xB8);
    f28 = planeData[0xA0 / 4];
    sincosf(*(f32*)((s32)plane + 4) - float_90_80420d14, &sx1, &cx1);
    start.x = f28 * (sx2 * -cx1) + px + (planeData[0x9C / 4] * cx1 * sx2 - planeData[0x9C / 4] * sx3 * cx2);
    start.y = planeData[0x9C / 4] * sx1 * cx3 + sx1 * f28 + py;
    start.z = f28 * (cx2 * -cx1) + pz + (planeData[0x9C / 4] * cx1 * cx2 - planeData[0x9C / 4] * sx3 * sx2);
    *(Vec*)((s32)plane + 0x30) = start;

    radius = float_10_80420d18 + *(f32*)((s32)mario + 0x180);
    hit = marioHitCheck(-(float_10_80420d18 * sx2 - start.x), start.y,
                        -(float_10_80420d18 * cx2 - start.z), sx2,
                        float_0_80420d1c, cx2, scratchA, &attr, scratchB,
                        &radius, scratchC, scratchA, scratchB);
    if (hit != 0) {
        *(void**)((s32)mario + 0x1E4) = hit;
        *outAttr = attr;
        hitAny = 1;
    }

    radius = *(f32*)((s32)mario + 0x180);
    hit = marioHitCheck(end.x, end.y, end.z, sx2, float_0_80420d1c, cx2,
                        scratchA, &attr, scratchB, &radius, scratchC, scratchA, scratchB);
    if (hit != 0) {
        *(void**)((s32)mario + 0x1E4) = hit;
        *outAttr = attr;
        hitAny = 1;
    }

    *(void**)((s32)mario + 0x1E8) = 0;
    radius = float_10p1_80420d20;
    hit = marioHitCheck(start.x, start.y + float_10_80420d18, start.z,
                        float_0_80420d1c, float_neg1_80420d24, float_0_80420d1c,
                        scratchA, &attr, scratchB, &radius, scratchC, scratchA, scratchB);
    if (hit != 0) {
        *(void**)((s32)mario + 0x1E8) = hit;
        *outAttr = attr;
        hitAny = 1;
    } else if (!hitAny) {
        radius = float_10p1_80420d20;
        hit = marioHitCheck(end.x, end.y + float_10_80420d18, end.z,
                            float_0_80420d1c, float_neg1_80420d24, float_0_80420d1c,
                            scratchA, &attr, scratchB, &radius, scratchC, scratchA, scratchB);
        if (hit != 0) {
            *(void**)((s32)mario + 0x1E8) = hit;
            *outAttr = attr;
            hitAny = 1;
        }
    }

    if (!hitAny) {
        PSVECSubtract(&end, &start, &delta);
        radius = PSVECMag(&delta);
        PSVECNormalize(&delta, &delta);
        hit = marioHitCheckVec(&start, &delta, &x0, 0, &radius);
        if (hit != 0) {
            *(void**)((s32)mario + 0x1E8) = hit;
            *outAttr = *(s32*)((s32)&x0 - 4);
            hitAny = 1;
        }
    }

    *(void**)((s32)mario + 0x1F0) = 0;
    if (*(void**)((s32)mario + 0x1E8) == 0) {
        radius = float_2_80420d28;
        hit = marioHitCheck(start.x, start.y, start.z,
                            float_0_80420d1c, float_1_80420d2c, float_0_80420d1c,
                            scratchA, &attr, scratchB, &radius, scratchC, scratchA, scratchB);
        if (hit != 0) {
            *(void**)((s32)mario + 0x1F0) = hit;
            *outAttr = attr;
            hitAny = 1;
        }
    }

    return hitAny;
}

void mot_plane_post(void) {
    extern void* marioGetPtr(void);
    extern void camFollowYOn(void);
    extern void psndSFXOff(s32 id);
    extern void marioPaperOff(void);
    extern void marioChgPaper(s32 mode);
    extern void marioAdjustMoveDir(void);
    extern void __memFree(s32 heap, void* ptr);
    extern void allPartyRideOff2(void);
    extern u8 str_SFX_MARIO_AIRPLANE1_802c40d8[];
    extern f32 float_20_80420d34;
    extern f32 float_37_80420d38;
    extern f32 float_0_80420d1c;
    typedef struct Vec { u32 x, y, z; } Vec;
    u8* data = str_SFX_MARIO_AIRPLANE1_802c40d8;
    void* mario = marioGetPtr();
    void* mario2;
    void* plane;

    camFollowYOn();
    mario2 = marioGetPtr();
    plane = *(void**)((s32)mario2 + 0x294);
    if (plane != 0) {
        s32 sound = *(s32*)((s32)plane + 0x28);
        if ((u32)(sound + 0x10000) != 0xFFFF) {
            psndSFXOff(sound);
            *(s32*)((s32)*(void**)((s32)mario2 + 0x294) + 0x28) = -1;
        }
    }
    mario2 = marioGetPtr();
    *(f32*)((s32)mario2 + 0x1B8) = float_20_80420d34;
    *(f32*)((s32)mario2 + 0x1BC) = float_37_80420d38;
    *(Vec*)((s32)mario2 + 0xBC) = *(Vec*)(data + 0x50);
    *(Vec*)((s32)mario2 + 0xB0) = *(Vec*)(data + 0x5C);
    *(Vec*)((s32)mario2 + 0x98) = *(Vec*)(data + 0x68);
    marioPaperOff();
    marioChgPaper(0);
    *(u32*)((s32)mario2 + 0x4) &= ~0x8;
    *(u32*)((s32)mario2 + 0x0) |= 0x80;
    marioAdjustMoveDir();
    *(u32*)((s32)mario2 + 0x4) &= ~0x100;
    *(f32*)((s32)mario + 0x138) = float_0_80420d1c;
    *(f32*)((s32)mario + 0x140) = float_0_80420d1c;
    plane = *(void**)((s32)mario + 0x294);
    if (plane != 0) {
        __memFree(0, plane);
        *(void**)((s32)mario + 0x294) = 0;
    }
    allPartyRideOff2();
}

u8 marioForcePlaneAnime(void) {
    extern void* marioGetPtr(void);
    extern void marioPaperOff(void);
    extern void marioChgPaper(void* paper);
    extern s32 marioGetColor(void);
    extern void marioPaperOn(void* paper);
    extern void marioChgPose(void* pose);
    extern void* paper_plane[4];
    extern char str_PM_P_1B_802c4180[];
    extern char str_M_Z_1_80420d88[6];
    void* mario = marioGetPtr();

    marioPaperOff();
    marioChgPaper(0);
    marioPaperOn(paper_plane[marioGetColor()]);
    marioChgPaper(str_PM_P_1B_802c4180);
    marioChgPose(str_M_Z_1_80420d88);
    *(u32*)((s32)mario + 4) |= 8;
}

s32 mario_plane_cancel(void) {
    extern void* marioGetPtr(void);
    void* mario = marioGetPtr();
    s32 state;
    s32 result;

    if (*(u16*)((s32)mario + 0x2E) != 0x18) {
        result = 0;
    } else {
        state = *(s32*)((s32)mario + 0x44);
        if (state >= 0xC && state < 0x14) {
            *(s32*)((s32)mario + 0x44) = 0x32;
            *(f32*)((s32)mario + 0x2C4) = *(f32*)((s32)mario + 0x90);
            result = 1;
        } else {
            result = 0;
        }
    }
    return result;
}
