#include "mario/mario_hit.h"

s32 chkFilterAttr2(void* a, void* b) {
    u32 attr = *(u32*)((s32)b + 0x4);
    return ((attr & *(u32*)((s32)a + 0x4)) == 0 || (attr & *(u32*)((s32)a + 0x8)) != 0);
}

void clear_hitobj_ride(void) {
    extern void* marioGetPtr(void);
    extern void kpaClearHitobjRide(void);

    void* mario = marioGetPtr();
    *(s32*)((s32)mario + 0x1E8) = 0;
    if ((s32)*(u8*)((s32)mario + 0x3C) == 2) {
        kpaClearHitobjRide();
    }
}


void marioChkGnd(void) {
    typedef struct Vec { f32 x, y, z; } Vec;
    typedef struct HitCheckArg {
        u8 pad[0xC];
        Vec start;
        Vec end;
        Vec normal;
        Vec hitPos;
        f32 radius;
    } HitCheckArg;
    extern void* marioGetPtr(void);
    extern s32 vivianGetStatus(void);
    extern void kpaClearHitobjRide(void);
    extern void* marioSearchGround(f64, f64, f32*, f32*, f32*);
    extern void* marioSearchGroundRoll(f64, f64, f32*, f32*, f32*);
    extern void* kpaSearchGround(f64, f32*, f32*, f32*);
    extern s32 hitCheckVecFilter(HitCheckArg*, void*);
    extern s32 chkfilterVecVivian(s32, int);
    extern s32 chkfilterVecRoll(s32, int);
    extern s32 chkfilterVec(s32, int);
    extern u32 hitGetAttr(void* hit);
    extern s32 marioChkFrontStep(f32*, void*, void*);
    extern void marioChgMot(s32 motion);
    extern void marioChgMotSub(s32 motion, s32 param);
    extern void marioSetFallPara(void);
    extern s32 marioChkShipMoveMode(void);
    extern void marioShipChgFall(void);
    extern void roll_upstairs_on(void);
    extern void set_damage_root_ypos(f32 y);
    extern f32 float_0_804208ac;
    extern f32 float_0p5_804208cc;
    extern f32 float_5_804208c0;
    extern f32 float_11_804208b4;
    extern f32 float_30_80420928;
    extern f32 float_37_8042097c;
    extern f32 float_1000_804208a8;
    extern f32 float_5000_80420984;
    extern f32 float_neg3000_80420980;

    void* player = marioGetPtr();
    HitCheckArg under;
    Vec down = { 0.0f, -1.0f, 0.0f };
    void* hit;
    void* underHit;
    f32 groundY;
    f32 drop;
    f32 angle;
    f32 stepY;
    u8 stepA[4];
    u8 stepB[4];

#define CHECK_UNDER() do { \
        player = marioGetPtr(); \
        under.start = *(Vec*)((s32)player + 0x8C); \
        under.start.y += *(f32*)((s32)player + 0x1BC) * \
            ((*(u32*)player & 0x01000000) ? float_0p5_804208cc : 1.0f); \
        under.end = down; \
        under.radius = float_1000_804208a8; \
        if (*(u16*)((s32)player + 0x2E) == 0x1C) { \
            underHit = (void*)hitCheckVecFilter(&under, chkfilterVecVivian); \
        } else if ((*(u32*)player & 0x01000000) != 0) { \
            underHit = (void*)hitCheckVecFilter(&under, chkfilterVecRoll); \
        } else { \
            underHit = (void*)hitCheckVecFilter(&under, chkfilterVec); \
        } \
        *(void**)((s32)player + 0x1FC) = underHit; \
        *(f32*)((s32)player + 0x1C4) = underHit != 0 ? \
            under.hitPos.y : float_neg3000_80420980; \
    } while (0)

    if (vivianGetStatus() == 2 || vivianGetStatus() == 1 || vivianGetStatus() == 3) {
        hit = marioSearchGround(float_37_8042097c, float_0_804208ac, &groundY, &drop, &angle);
        *(void**)((s32)player + 0x1E8) = hit;
        CHECK_UNDER();
        return;
    }

    *(void**)((s32)player + 0x1E8) = 0;
    if (*(s8*)((s32)player + 0x3C) == 2) kpaClearHitobjRide();
    CHECK_UNDER();
    if ((*(u32*)player & 0x60000) != 0 || *(f32*)((s32)player + 0x90) >= float_5000_80420984) {
        return;
    }

    if ((*(u32*)player & 0x100) == 0) {
        if (*(s8*)((s32)player + 0x3C) == 2) {
            hit = kpaSearchGround(float_11_804208b4, &groundY, &drop, &angle);
        } else if ((*(u32*)player & 0x01000000) != 0) {
            hit = marioSearchGroundRoll(float_11_804208b4, *(f32*)((s32)player + 0x7C),
                                        &groundY, &drop, &angle);
        } else {
            hit = marioSearchGround(float_11_804208b4, *(f32*)((s32)player + 0x7C),
                                    &groundY, &drop, &angle);
        }
        if (hit != 0) {
            *(void**)((s32)player + 0x1E8) = hit;
            *(f32*)((s32)player + 0x1C4) = groundY;
        }
    } else {
        groundY = *(f32*)((s32)player + 0x90);
        hit = 0;
    }

    if (hit == 0) {
        if ((*(u32*)player & 0x40000) == 0) {
            marioChgMot(drop < float_0_804208ac ? 7 : 8);
            marioSetFallPara();
        }
        return;
    }

    if (*(u16*)((s32)player + 0x2E) == 0x13) {
        if (marioChkShipMoveMode() != 0) {
            if (*(void**)((s32)player + 0x1E8) == 0) {
                marioShipChgFall();
            } else {
                *(u32*)player &= ~0x40000;
            }
        }
        return;
    }

    if (*(u16*)((s32)player + 0x2E) != 0x1F &&
        (hitGetAttr(hit) & 0x200) != 0) {
        if (*(f32*)((s32)player + 0x7C) >= float_0_804208ac) {
            marioSetFallPara();
        }
        set_damage_root_ypos(*(f32*)((s32)player + 0x90));
        marioChgMot(0x1F);
        return;
    }

    if ((hitGetAttr(hit) & 0x800) != 0) {
        if (vivianGetStatus() != 2 && vivianGetStatus() != 1 &&
            vivianGetStatus() != 3) {
            set_damage_root_ypos(*(f32*)((s32)player + 0x90));
            marioChgMot(0x20);
        }
        return;
    }

    if (*(s16*)((s32)player + 0x50) == 0 &&
        marioChkFrontStep(&stepY, stepA, stepB) != 0) {
        f32 rise = stepY - *(f32*)((s32)player + 0x90);
        if (rise >= float_5_804208c0 && rise <= float_11_804208b4 &&
            *(f32*)((s32)player + 0x218) <= float_30_80420928 &&
            *(f32*)((s32)player + 0x180) != float_0_804208ac) {
            *(f32*)((s32)player + 0x1C8) = stepY;
            if ((*(u32*)player & 0x01000000) != 0) roll_upstairs_on();
            else marioChgMotSub(12, 0);
            return;
        }
    }

    *(f32*)((s32)player + 0x90) = groundY;

#undef CHECK_UNDER
}

s32 marioSearchGround(f64 velocityY, f64 maxDrop, f32* outY, f32* outDrop, f32* outAngle) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;
    typedef struct HitCheckArg {
        u8 pad[0xC];
        Vec start;
        Vec end;
        Vec normal;
        Vec hitPos;
        f32 radius;
    } HitCheckArg;

    extern void* marioGetPtr(void);
    extern f64 sin(f64 value);
    extern f64 cos(f64 value);
    extern f64 sqrt(f64 value);
    extern f64 __fabs(f64 value);
    extern f64 angleABf(f64 x1, f64 y1, f64 x2, f64 y2);
    extern f64 toMovedir(f64 angle);
    extern f64 revise360(f64 angle);
    extern s32 hitCheckVecFilter(HitCheckArg* hit, void* filter);
    extern s32 chkfilterVecVivian(s32, int);
    extern s32 chkfilterVecRoll(s32, int);
    extern s32 chkfilterVec(s32, int);
    extern void marioUpdateCamCollision(Vec* position);
    extern Vec vec3_802c3bd8;
    extern f32 float_0_804208ac;
    extern f32 float_0p375_80420978;
    extern f32 float_0p425_80420974;
    extern f32 float_0p5_804208cc;
    extern f32 float_neg0p5_804208d0;
    extern f32 float_neg1_804208e4;
    extern f32 float_2_804208f0;
    extern f32 float_3p1416_804208e8;
    extern f32 float_5_804208c0;
    extern f32 float_11_804208b4;
    extern f32 float_50_80420954;
    extern f32 float_75_80420950;
    extern f32 float_100_80420900;
    extern f32 float_180_804208d4;
    extern f32 float_1000_804208a8;
    extern f32 float_neg10000_80420968;

    void* player = marioGetPtr();
    HitCheckArg hit;
    Vec cameraPos;
    f32 offsetX[5];
    f32 offsetZ[5];
    f32 currentY;
    f32 bestY;
    f32 direction;
    f32 sideX;
    f32 sideZ;
    f32 bias;
    f32 nx;
    f32 ny;
    f32 nz;
    f32 flatLength;
    f32 slope;
    f32 hitY;
    f32 tolerance;
    s32 result;
    s32 bestResult;
    s32 i;

    *(f32*)((s32)player + 0x218) = float_0_804208ac;
    *(f32*)((s32)player + 0x21C) = float_0_804208ac;
    *(f32*)((s32)player + 0x220) = float_0_804208ac;
    *(f32*)((s32)player + 0x224) = float_0_804208ac;
    currentY = *(f32*)((s32)player + 0x90);
    *outY = currentY;
    *outDrop = float_neg1_804208e4;

    if ((*(u32*)player & 0x300) != 0) {
        return 0;
    }

    if (*(u16*)((s32)player + 0x2E) == 0x13) {
        direction = toMovedir(*(f32*)((s32)player + 0x2BC));
    } else if (*(f32*)((s32)player + 0x180) == float_0_804208ac) {
        direction = toMovedir(*(f32*)((s32)player + 0x1AC));
    } else {
        direction = revise360(*(f32*)((s32)player + 0x1A4) - *(f32*)((s32)player + 0x1A0));
    }

    direction = float_3p1416_804208e8 * direction / float_180_804208d4;
    if ((*(u32*)player & 0x01000000) == 0) {
        sideX = float_0p375_80420978 * *(f32*)((s32)player + 0x1B8) * (f32)sin(direction);
        sideZ = float_0p375_80420978 * *(f32*)((s32)player + 0x1B8) * -(f32)cos(direction);
    } else {
        sideX = float_0p425_80420974 * *(f32*)((s32)player + 0x1B8) * (f32)sin(direction);
        sideZ = float_0p425_80420974 * *(f32*)((s32)player + 0x1B8) * -(f32)cos(direction);
    }

    offsetX[0] = float_0_804208ac;
    offsetZ[0] = float_0_804208ac;
    offsetX[1] = sideX;
    offsetZ[1] = sideZ;
    offsetX[2] = -sideZ;
    offsetZ[2] = sideX;
    offsetX[3] = -sideX;
    offsetZ[3] = -sideZ;
    offsetX[4] = sideZ;
    offsetZ[4] = -sideX;

    tolerance = float_2_804208f0;
    bestY = float_neg10000_80420968;
    bestResult = 0;
    for (i = 0; i < 5; i++) {
        hit.start.x = *(f32*)((s32)player + 0x8C) + offsetX[i];
        hit.start.y = currentY + float_5_804208c0 + (f32)__fabs(maxDrop);
        hit.start.z = *(f32*)((s32)player + 0x94) + offsetZ[i];
        hit.end = vec3_802c3bd8;
        hit.radius = *(f32*)((s32)player + 0x1BC) + (f32)__fabs(velocityY) + float_5_804208c0;

        if (*(u16*)((s32)marioGetPtr() + 0x2E) == 0x1C) {
            result = hitCheckVecFilter(&hit, chkfilterVecVivian);
        } else if ((*(u32*)marioGetPtr() & 0x01000000) != 0) {
            result = hitCheckVecFilter(&hit, chkfilterVecRoll);
        } else {
            result = hitCheckVecFilter(&hit, chkfilterVec);
        }
        if (result == 0) {
            continue;
        }

        bias = hit.normal.x >= float_0_804208ac ? float_0p5_804208cc : float_neg0p5_804208d0;
        nx = (f32)(s32)(hit.normal.x * float_1000_804208a8 + bias) / float_1000_804208a8;
        bias = hit.normal.y >= float_0_804208ac ? float_0p5_804208cc : float_neg0p5_804208d0;
        ny = (f32)(s32)(hit.normal.y * float_1000_804208a8 + bias) / float_1000_804208a8;
        bias = hit.normal.z >= float_0_804208ac ? float_0p5_804208cc : float_neg0p5_804208d0;
        nz = (f32)(s32)(hit.normal.z * float_1000_804208a8 + bias) / float_1000_804208a8;

        *(f32*)((s32)player + 0x21C) = (f32)(angleABf(0.0, 0.0, float_100_80420900 * nz,
                                                       float_100_80420900 * ny) - float_180_804208d4);
        *(f32*)((s32)player + 0x220) = float_0_804208ac;
        *(f32*)((s32)player + 0x224) = (f32)(angleABf(0.0, 0.0, float_100_80420900 * nx,
                                                       float_100_80420900 * ny) - float_180_804208d4);
        flatLength = (f32)sqrt(nx * nx + nz * nz);
        slope = (f32)angleABf(0.0, 0.0, flatLength, -ny);
        if (slope < float_50_80420954 || slope > float_75_80420950) {
            *(f32*)((s32)player + 0x218) = slope;
        }

        hitY = hit.hitPos.y;
        if ((currentY <= hitY || __fabs(hitY - currentY) < tolerance) &&
            __fabs(hitY - currentY) <= float_11_804208b4 && hitY > bestY) {
            *outY = hitY;
            *outDrop = float_0_804208ac;
            *outAngle = (f32)angleABf(*(f32*)((s32)player + 0x8C), *(f32*)((s32)player + 0x94),
                                      hit.hitPos.x, hit.hitPos.z);
            cameraPos = hit.hitPos;
            marioUpdateCamCollision(&cameraPos);
            bestY = hitY;
            bestResult = result;
        } else if (__fabs(hitY - currentY) <= __fabs(maxDrop) && hitY > bestY) {
            *outY = hitY;
            *outDrop = float_0_804208ac;
            *outAngle = (f32)angleABf(*(f32*)((s32)player + 0x8C), *(f32*)((s32)player + 0x94),
                                      hit.hitPos.x, hit.hitPos.z);
            bestY = hitY;
            bestResult = result;
        } else {
            *outDrop = (f32)__fabs(currentY - hitY);
        }
    }
    return bestResult;
}

s32 marioChkWallAround(f64 speed, f64 angle, f64 radius, f64 height,
                       void* position, s32 mode) {
    typedef struct VecLocal { f32 x, y, z; } VecLocal;
    typedef struct HitCheckArgLocal {
        u8 pad[0xC];
        VecLocal start;
        VecLocal end;
        VecLocal normal;
        VecLocal hitPos;
        f32 radius;
    } HitCheckArgLocal;
    extern void* marioGetPtr(void);
    extern s32 kpaGetStageViewType(void);
    extern void sincosf(f32, f32*, f32*);
    extern void* hitCheckVecFilter(void*, void*);
    extern s32 chkfilterVecVivian(s32, s32);
    extern s32 chkfilterVecRoll(s32, s32);
    extern s32 chkfilterVec(s32, s32);
    extern u32 hitGetAttr(void*);
    extern f32 PSVECMag(void*);
    extern void PSVECScale(void*, void*, f32);
    extern void PSVECAdd(void*, void*, void*);
    extern f32 PSVECDotProduct(void*, void*);
    extern f32 float_0_804208ac;
    extern f32 float_0p5_804208cc;
    extern f32 float_1_804208c4;
    extern f32 float_11_804208b4;
    extern f32 float_360_804208c8;
    extern f32 float_180_804208d4;
    extern f32 float_0p125_804208d8;
    extern f32 float_0p25_804208dc;
    extern f32 float_1000_804208a8;
    extern f32 float_neg0p5_804208d0;
    VecLocal* pos = position;
    VecLocal correction = { 0.0f, 0.0f, 0.0f };
    VecLocal firstCorrection;
    HitCheckArgLocal low;
    HitCheckArgLocal high;
    void* player = marioGetPtr();
    void* lowHit;
    void* highHit;
    f32 lowDist;
    f32 highDist;
    f32 sinv;
    f32 cosv;
    f32 step;
    f32 probeRadius;
    f32 limit;
    s32 count;
    s32 collided = 0;
    s32 i;

    count = kpaGetStageViewType() == 0 ? 2 : 8;
    step = float_360_804208c8 / (f32)count;
    probeRadius = (f32)radius * float_0p5_804208cc;

    for (i = 0; i < count; i++) {
        u32 attr;
        sincosf((f32)height, &sinv, &cosv);
        low.start.x = pos->x;
        low.start.y = pos->y + (f32)angle - float_1_804208c4;
        low.start.z = pos->z;
        low.end.x = sinv;
        low.end.y = float_0_804208ac;
        low.end.z = cosv;
        low.radius = probeRadius;
        high = low;
        high.start.y = pos->y + float_11_804208b4;

        if (*(u16*)((u8*)player + 0x2E) == 0x1C) {
            lowHit = hitCheckVecFilter(&low, chkfilterVecVivian);
            highHit = hitCheckVecFilter(&high, chkfilterVecVivian);
        } else if ((*(u32*)player & 0x01000000) != 0) {
            lowHit = hitCheckVecFilter(&low, chkfilterVecRoll);
            highHit = hitCheckVecFilter(&high, chkfilterVecRoll);
        } else {
            lowHit = hitCheckVecFilter(&low, chkfilterVec);
            highHit = hitCheckVecFilter(&high, chkfilterVec);
        }
        lowDist = lowHit != 0 ?
            (f32)(s32)(low.radius * float_1000_804208a8 +
                (low.radius >= float_0_804208ac ? float_0p5_804208cc : float_neg0p5_804208d0)) /
                float_1000_804208a8 : probeRadius;
        highDist = highHit != 0 ?
            (f32)(s32)(high.radius * float_1000_804208a8 +
                (high.radius >= float_0_804208ac ? float_0p5_804208cc : float_neg0p5_804208d0)) /
                float_1000_804208a8 : probeRadius;

        if (lowHit != 0) {
            attr = hitGetAttr(lowHit);
            if (((attr & 0x80000) != 0 && (*(u32*)player & 0x01000000) == 0) ||
                ((attr & 0x80) != 0 && *(u16*)((u8*)player + 0x2E) == 0x19) ||
                ((attr & 0x80) == 0 && ((attr >> 23) & 1) != 0)) {
                lowHit = 0;
            }
        }
        if (highHit != 0) {
            attr = hitGetAttr(highHit);
            if (((attr & 0x80000) != 0 && (*(u32*)player & 0x01000000) == 0) ||
                ((attr & 0x80) != 0 && *(u16*)((u8*)player + 0x2E) == 0x19) ||
                ((attr & 0x80) == 0 && ((attr >> 23) & 1) != 0)) {
                highHit = 0;
            }
        }
        if (lowHit == 0 || highHit == 0) {
            if (highHit != 0) {
                lowHit = highHit;
                lowDist = highDist;
            }
        } else if (highDist < lowDist) {
            lowHit = highHit;
            lowDist = highDist;
        }
        if (lowHit != 0) {
            f32 distance = probeRadius - lowDist;
            if (distance < float_0_804208ac) {
                distance = -distance;
            }
            sincosf(float_180_804208d4 + (f32)height, &sinv, &cosv);
            correction.x += sinv * distance;
            correction.z += cosv * distance;
            collided = 1;
        }
        height = (f32)height + step;
    }

    if (!collided) {
        return 0;
    }
    limit = float_0p125_804208d8 * (f32)radius;
    if (mode == 0 && (*(u16*)((u8*)player + 0x2E) == 0x0D ||
                      *(u16*)((u8*)player + 0x2E) == 0x0E)) {
        limit = float_0p25_804208dc * (f32)radius;
    }
    if (limit < float_1_804208c4) {
        limit = float_1_804208c4;
    }
    firstCorrection = correction;
    if (PSVECMag(&correction) > limit) {
        f32 magnitude = PSVECMag(&correction);
        PSVECScale(&correction, &correction, limit / magnitude);
    }
    PSVECAdd(pos, &correction, pos);

    correction.x = 0.0f;
    correction.y = 0.0f;
    correction.z = 0.0f;
    for (i = 0; i < count; i++) {
        sincosf((f32)height, &sinv, &cosv);
        low.start.x = pos->x;
        low.start.y = pos->y + float_11_804208b4;
        low.start.z = pos->z;
        low.end.x = sinv;
        low.end.y = float_0_804208ac;
        low.end.z = cosv;
        low.radius = probeRadius;
        if (*(u16*)((u8*)player + 0x2E) == 0x1C) {
            lowHit = hitCheckVecFilter(&low, chkfilterVecVivian);
        } else if ((*(u32*)player & 0x01000000) != 0) {
            lowHit = hitCheckVecFilter(&low, chkfilterVecRoll);
        } else {
            lowHit = hitCheckVecFilter(&low, chkfilterVec);
        }
        if (lowHit != 0) {
            f32 distance = probeRadius - low.radius;
            if (distance < 0.0f) distance = -distance;
            sincosf(float_180_804208d4 + (f32)height, &sinv, &cosv);
            correction.x += sinv * distance;
            correction.z += cosv * distance;
        }
        height = (f32)height + step;
    }
    if (PSVECDotProduct(&firstCorrection, &correction) < float_0_804208ac) {
        PSVECScale(&correction, &correction, float_0p5_804208cc);
        PSVECAdd(pos, &correction, pos);
    }
    return 1;
}

void* searchFrontWall(f64 speed, f64 angle, f64 radius, void* position, s32 outParam) {
    typedef struct Vec { f32 x, y, z; } Vec;
    typedef struct HitCheckArg {
        u8 pad[0xC];
        Vec start;
        Vec end;
        Vec normal;
        Vec hitPos;
        f32 radius;
    } HitCheckArg;
    extern void* marioGetPtr(void);
    extern s32 vivianGetStatus(void);
    extern void sincosf(f32, f32*, f32*);
    extern s32 marioChkSts(u32);
    extern s32 strcmp(const char*, const char*);
    extern s32 kpaGetLevel(void);
    extern s32 hitCheckVecFilter(HitCheckArg*, void*);
    extern s32 chkfilterVecVivian(s32, s32);
    extern s32 chkfilterVecRoll(s32, s32);
    extern s32 chkfilterVec(s32, s32);
    extern s32 chkBalloon(void*);
    extern void* gp;
    extern char str_eki_04_802c3d90[];
    extern f32 float_0_804208ac;
    extern f32 float_0p1_804208ec;
    extern f32 float_0p5_804208cc;
    extern f32 float_neg0p5_804208d0;
    extern f32 float_0p75_804208e0;
    extern f32 float_7_80420934;
    extern f32 float_11_804208b4;
    extern f32 float_12_80420930;
    extern f32 float_26p64_8042092c;
    extern f32 float_30_80420928;
    extern f32 float_45_804208bc;
    extern f32 float_1000_804208a8;
    u8* player = marioGetPtr();
    Vec* pos = position;
    HitCheckArg check;
    f32 heights[4];
    f32 sinv;
    f32 cosv;
    f32 rightSin;
    f32 rightCos;
    f32 leftSin;
    f32 leftCos;
    f32 bestDistance;
    f32 sideDistance;
    f32 probeRadius;
    f32 correctionX = 0.0f;
    f32 correctionZ = 0.0f;
    void* hit;
    void* bestHit = 0;
    s32 count = 2;
    s32 rightBlocked = 0;
    s32 leftBlocked = 0;
    s32 i;

    if (vivianGetStatus() == 2) {
        return 0;
    }
    sincosf((f32)angle, &sinv, &cosv);
    if (marioChkSts(0x10) != 0 &&
        strcmp((char*)gp + 0x12C, str_eki_04_802c3d90) == 0 &&
        (*(f32*)(player + 0x21C) >= float_45_804208bc ||
         *(f32*)(player + 0x21C) <= -float_45_804208bc) &&
        (*(f32*)(player + 0x224) >= float_45_804208bc ||
         *(f32*)(player + 0x224) <= -float_45_804208bc)) {
        pos->x += (f32)speed * sinv;
        pos->z += (f32)speed * cosv;
        return 0;
    }

    if (*(s8*)(player + 0x3C) == 2) {
        if ((*(u32*)(player + 0x14) & 1) == 0) {
            heights[0] = float_0p75_804208e0 * *(f32*)(player + 0x1BC);
            heights[1] = float_11_804208b4;
        } else {
            heights[0] = float_0p75_804208e0 * *(f32*)(player + 0x1BC);
            heights[1] = float_0p5_804208cc * *(f32*)(player + 0x1BC);
            heights[2] = float_11_804208b4;
            count = 3;
            if (kpaGetLevel() == 2) {
                heights[3] = float_30_80420928;
                count = 4;
            }
        }
    } else if ((*(u32*)player & 0x01000000) != 0) {
        heights[0] = float_12_80420930;
        heights[1] = float_11_804208b4;
        if (*(s16*)(player + 0x50) != 0) {
            heights[2] = float_7_80420934;
            count = 3;
        }
    } else {
        heights[0] = float_26p64_8042092c;
        heights[1] = float_11_804208b4;
        if (*(s16*)(player + 0x50) != 0) {
            heights[2] = float_0p1_804208ec;
            count = 3;
        }
    }

    probeRadius = float_0p5_804208cc * (f32)radius;
    bestDistance = (f32)speed + probeRadius;
    for (i = 0; i < count; i++) {
        check.start = *pos;
        check.start.y = pos->y + heights[i];
        check.end.x = sinv;
        check.end.y = float_0_804208ac;
        check.end.z = cosv;
        check.radius = bestDistance;
        if (*(u16*)(player + 0x2E) == 0x1C) {
            hit = (void*)hitCheckVecFilter(&check, chkfilterVecVivian);
        } else if ((*(u32*)player & 0x01000000) != 0) {
            hit = (void*)hitCheckVecFilter(&check, chkfilterVecRoll);
        } else {
            hit = (void*)hitCheckVecFilter(&check, chkfilterVec);
        }
        if (hit != 0) {
            f32 bias = check.radius >= float_0_804208ac ?
                       float_0p5_804208cc : float_neg0p5_804208d0;
            f32 distance = (f32)(s32)(check.radius * float_1000_804208a8 + bias) /
                           float_1000_804208a8;
            if (chkBalloon(hit) != 0) {
                *(void**)(player + 0x210) = hit;
            }
            if (bestHit == 0 || distance < bestDistance) {
                bestHit = hit;
                bestDistance = distance;
                correctionX = float_0p5_804208cc *
                    -((speed * sinv * check.hitPos.x + speed * cosv * check.hitPos.z) *
                      check.hitPos.x - speed * sinv);
                correctionZ = float_0p5_804208cc *
                    -((speed * sinv * check.hitPos.x + speed * cosv * check.hitPos.z) *
                      check.hitPos.z - speed * cosv);
            }
        }
    }

    sincosf((f32)angle + float_45_804208bc, &rightSin, &rightCos);
    sideDistance = probeRadius;
    for (i = 0; i < count; i++) {
        check.start = *pos;
        check.start.y = pos->y + heights[i];
        check.end.x = rightSin;
        check.end.y = float_0_804208ac;
        check.end.z = rightCos;
        check.radius = probeRadius;
        if (*(u16*)(player + 0x2E) == 0x1C) {
            rightBlocked = hitCheckVecFilter(&check, chkfilterVecVivian);
        } else if ((*(u32*)player & 0x01000000) != 0) {
            rightBlocked = hitCheckVecFilter(&check, chkfilterVecRoll);
        } else {
            rightBlocked = hitCheckVecFilter(&check, chkfilterVec);
        }
        if (rightBlocked != 0) {
            sideDistance = check.radius;
            break;
        }
    }
    if (rightBlocked != 0) {
        sincosf((f32)angle - float_45_804208bc, &leftSin, &leftCos);
        for (i = 0; i < count; i++) {
            check.start = *pos;
            check.start.y = pos->y + heights[i];
            check.end.x = leftSin;
            check.end.y = float_0_804208ac;
            check.end.z = leftCos;
            check.radius = probeRadius;
            if (*(u16*)(player + 0x2E) == 0x1C) {
                leftBlocked = hitCheckVecFilter(&check, chkfilterVecVivian);
            } else if ((*(u32*)player & 0x01000000) != 0) {
                leftBlocked = hitCheckVecFilter(&check, chkfilterVecRoll);
            } else {
                leftBlocked = hitCheckVecFilter(&check, chkfilterVec);
            }
        }
    }

    if (bestHit == 0) {
        if (leftBlocked == 0) {
            pos->x += (f32)speed * sinv;
            pos->z += (f32)speed * cosv;
        }
    } else if (leftBlocked == 0) {
        *(f32*)outParam = sideDistance;
        pos->x += correctionX;
        pos->z += correctionZ;
    } else {
        *(f32*)outParam = float_0_804208ac;
    }
    return bestHit;
}

s32 marioHitCheck2(void* start, void* end, f32* outNormal, void* outHitPos, f32* radius) {
    typedef struct Vec { f32 x, y, z; } Vec;
    typedef struct HitCheckArg {
        u32 flags;
        u32 attrMask;
        u32 pad;
        Vec start;
        Vec end;
        Vec normal;
        Vec hitPos;
        f32 radius;
    } HitCheckArg;
    extern void* marioGetPtr(void);
    extern s32 marioGetJabaraState(void);
    extern s32 hitCheckVecFilter(HitCheckArg*, void*);
    extern s32 chkFilterAttr2(s32, s32);
    extern s32 chkfilterVecVivian(s32, s32);
    extern s32 chkfilterVecRoll(s32, s32);
    extern s32 chkfilterVec(s32, s32);
    extern f32 float_0_804208ac;
    extern f32 float_0p5_804208cc;
    extern f32 float_neg0p5_804208d0;
    extern f32 float_1000_804208a8;
    u8* player = marioGetPtr();
    HitCheckArg hit;
    f32 value = *radius;
    f32 bias;
    s32 motion = *(u16*)(player + 0x2E);
    s32 useAttrFilter;
    s32 result;

    hit.start = *(Vec*)start;
    hit.end = *(Vec*)end;
    hit.radius = value;
    useAttrFilter = 0;
    if (motion == 0x14) {
        if (marioGetJabaraState() == 1) {
            useAttrFilter = 1;
        }
    } else if ((*(u32*)player & 0x00020000) == 0) {
        useAttrFilter = 1;
    }

    if (useAttrFilter) {
        hit.flags = 4;
        hit.attrMask = 0x800000;
        result = hitCheckVecFilter(&hit, chkFilterAttr2);
    } else if (motion == 0x1C) {
        result = hitCheckVecFilter(&hit, chkfilterVecVivian);
    } else if ((*(u32*)player & 0x01000000) != 0) {
        result = hitCheckVecFilter(&hit, chkfilterVecRoll);
    } else {
        result = hitCheckVecFilter(&hit, chkfilterVec);
    }

    if (result != 0) {
        value = hit.radius;
        bias = value >= float_0_804208ac ? float_0p5_804208cc : float_neg0p5_804208d0;
        value = (f32)(s32)(value * float_1000_804208a8 + bias) / float_1000_804208a8;
        if (outNormal != 0) {
            *(Vec*)outNormal = hit.normal;
            value = outNormal[0];
            bias = value >= float_0_804208ac ? float_0p5_804208cc : float_neg0p5_804208d0;
            outNormal[0] = (f32)(s32)(value * float_1000_804208a8 + bias) / float_1000_804208a8;
            value = outNormal[1];
            bias = value >= float_0_804208ac ? float_0p5_804208cc : float_neg0p5_804208d0;
            outNormal[1] = (f32)(s32)(value * float_1000_804208a8 + bias) / float_1000_804208a8;
            value = outNormal[2];
            bias = value >= float_0_804208ac ? float_0p5_804208cc : float_neg0p5_804208d0;
            outNormal[2] = (f32)(s32)(value * float_1000_804208a8 + bias) / float_1000_804208a8;
        }
        if (outHitPos != 0) {
            *(Vec*)outHitPos = hit.hitPos;
        }
    }
    *radius = value;
    return result;
}

s32 marioSearchGroundRoll(f64 velocityY, f64 maxDrop, f32* outY, f32* outDrop, f32* outAngle) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;
    typedef struct HitCheckArg {
        u8 pad[0xC];
        Vec start;
        Vec end;
        Vec normal;
        Vec hitPos;
        f32 radius;
    } HitCheckArg;

    extern void* marioGetPtr(void);
    extern f64 sqrt(f64 value);
    extern f64 __fabs(f64 value);
    extern f64 angleABf(f64 x1, f64 y1, f64 x2, f64 y2);
    extern s32 hitCheckVecFilter(HitCheckArg* hit, void* filter);
    extern s32 chkfilterVecVivian(s32, int);
    extern s32 chkfilterVecRoll(s32, int);
    extern s32 chkfilterVec(s32, int);
    extern Vec vec3_802c3bd8;
    extern f32 float_0_804208ac;
    extern f32 float_0p01_80420964;
    extern f32 float_0p5_804208cc;
    extern f32 float_neg0p5_804208d0;
    extern f32 float_neg1_804208e4;
    extern f32 float_2_804208f0;
    extern f32 float_10_80420904;
    extern f32 float_11_804208b4;
    extern f32 float_75_80420950;
    extern f32 float_100_80420900;
    extern f32 float_105_80420970;
    extern f32 float_180_804208d4;
    extern f32 float_1000_804208a8;
    extern f32 float_neg10000_80420968;

    void* player = marioGetPtr();
    HitCheckArg hit;
    f32 offsetX[7];
    f32 offsetZ[7];
    f32 radiusX;
    f32 radiusZ;
    f32 currentY;
    f32 tolerance;
    f32 bestY;
    f32 bias;
    f32 nx;
    f32 ny;
    f32 nz;
    f32 flatLength;
    f32 slope;
    f32 hitY;
    s32 first;
    s32 last;
    s32 result;
    s32 bestResult;
    s32 i;

    *(f32*)((s32)player + 0x21C) = float_0_804208ac;
    *(f32*)((s32)player + 0x220) = float_0_804208ac;
    *(f32*)((s32)player + 0x224) = float_0_804208ac;
    currentY = *(f32*)((s32)player + 0x90);
    *outY = currentY;
    *outDrop = float_neg1_804208e4;

    if (*(u32*)player & 0x300) {
        return 0;
    }

    radiusX = *(f32*)((s32)player + 0x1B8) * float_0p5_804208cc;
    radiusZ = radiusX;
    offsetX[0] = 0.0f;
    offsetZ[0] = 0.0f;
    offsetX[1] = radiusX;
    offsetZ[1] = 0.0f;
    offsetX[2] = -radiusX;
    offsetZ[2] = 0.0f;
    offsetX[3] = 0.0f;
    offsetZ[3] = radiusZ;
    offsetX[4] = 0.0f;
    offsetZ[4] = -radiusZ;
    offsetX[5] = radiusX;
    offsetZ[5] = radiusZ;
    offsetX[6] = -radiusX;
    offsetZ[6] = -radiusZ;

    if (__fabs(*(f32*)((s32)player + 0x218)) >= float_0p01_80420964) {
        first = 4;
        last = 7;
        tolerance = float_10_80420904;
    } else {
        first = 0;
        last = 4;
        tolerance = float_2_804208f0;
    }

    bestY = float_neg10000_80420968;
    bestResult = 0;
    for (i = first; i < last; i++) {
        hit.start.x = *(f32*)((s32)player + 0x8C) + offsetX[i];
        hit.start.y = currentY + *(f32*)((s32)player + 0x1BC);
        hit.start.z = *(f32*)((s32)player + 0x94) + offsetZ[i];
        hit.end = vec3_802c3bd8;
        hit.radius = *(f32*)((s32)player + 0x1BC) +
                     (f32)__fabs(velocityY) + float_0p01_80420964;

        if (*(u16*)((s32)marioGetPtr() + 0x2E) == 0x1C) {
            result = hitCheckVecFilter(&hit, chkfilterVecVivian);
        } else if (*(u32*)marioGetPtr() & 0x01000000) {
            result = hitCheckVecFilter(&hit, chkfilterVecRoll);
        } else {
            result = hitCheckVecFilter(&hit, chkfilterVec);
        }
        if (result == 0) {
            continue;
        }

        bias = hit.normal.x >= 0.0f ? float_0p5_804208cc : float_neg0p5_804208d0;
        nx = (f32)(s32)(hit.normal.x * float_1000_804208a8 + bias) / float_1000_804208a8;
        bias = hit.normal.y >= 0.0f ? float_0p5_804208cc : float_neg0p5_804208d0;
        ny = (f32)(s32)(hit.normal.y * float_1000_804208a8 + bias) / float_1000_804208a8;
        bias = hit.normal.z >= 0.0f ? float_0p5_804208cc : float_neg0p5_804208d0;
        nz = (f32)(s32)(hit.normal.z * float_1000_804208a8 + bias) / float_1000_804208a8;

        *(f32*)((s32)player + 0x21C) =
            (f32)(angleABf(0.0, 0.0, float_100_80420900 * nz,
                           float_100_80420900 * ny) - float_180_804208d4);
        *(f32*)((s32)player + 0x220) = 0.0f;
        *(f32*)((s32)player + 0x224) =
            (f32)(angleABf(0.0, 0.0, float_100_80420900 * nx,
                           float_100_80420900 * ny) - float_180_804208d4);
        flatLength = (f32)sqrt(nx * nx + nz * nz);
        slope = (f32)angleABf(0.0, 0.0, flatLength, -ny);

        if (slope > float_105_80420970 || slope < float_75_80420950) {
            *(f32*)((s32)player + 0x218) = slope;
            hitY = hit.hitPos.y;
            if (currentY <= hitY || __fabs(hitY - currentY) < tolerance) {
                if (__fabs(hitY - currentY) <= float_11_804208b4 && hitY > bestY) {
                    *outY = hitY;
                    *outDrop = float_0_804208ac;
                    *outAngle = (f32)angleABf(*(f32*)((s32)player + 0x8C),
                                              *(f32*)((s32)player + 0x94),
                                              hit.hitPos.x, hit.hitPos.z);
                    bestY = hitY;
                    bestResult = result;
                }
            } else if (__fabs(hitY - currentY) <= __fabs(maxDrop)) {
                if (hitY > bestY) {
                    *outY = hitY;
                    *outDrop = float_0_804208ac;
                    *outAngle = (f32)angleABf(*(f32*)((s32)player + 0x8C),
                                              *(f32*)((s32)player + 0x94),
                                              hit.hitPos.x, hit.hitPos.z);
                    bestY = hitY;
                    bestResult = result;
                }
            } else {
                *outDrop = (f32)__fabs(currentY - hitY);
                *outAngle = (f32)angleABf(*(f32*)((s32)player + 0x8C),
                                          *(f32*)((s32)player + 0x94),
                                          hit.hitPos.x, hit.hitPos.z);
            }
        }
    }
    return bestResult;
}

s32 marioLandonChk_old(f64 velocityY, f32* outY) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;
    typedef struct HitCheckArg {
        u8 pad[0xC];
        Vec start;
        Vec end;
        Vec normal;
        Vec hitPos;
        f32 radius;
    } HitCheckArg;

    extern void* marioGetPtr(void);
    extern f32 toMovedir(f64 angle);
    extern f64 revise360(f64 angle);
    extern s32 kpaSearchGround(f64 length, f32* outY, void* outA, void* outB);
    extern f64 sin(f64 angle);
    extern f64 cos(f64 angle);
    extern f64 sqrt(f64 value);
    extern f64 __fabs(f64 value);
    extern f64 angleABf(f64 x1, f64 y1, f64 x2, f64 y2);
    extern s32 hitCheckVecFilter(HitCheckArg* hit, void* filter);
    extern s32 chkfilterVecVivian(s32, int);
    extern s32 chkfilterVecRoll(s32, int);
    extern s32 chkfilterVec(s32, int);
    extern Vec vec3_802c3bf0;
    extern f32 float_0_804208ac;
    extern f32 float_0p1_804208ec;
    extern f32 float_0p01_80420964;
    extern f32 float_0p75_804208e0;
    extern f32 float_11_804208b4;
    extern f32 float_0p25_804208dc;
    extern f32 float_0p5_804208cc;
    extern f32 float_neg0p5_804208d0;
    extern f32 float_3p1416_804208e8;
    extern f32 float_100_80420900;
    extern f32 float_180_804208d4;
    extern f32 float_1000_804208a8;

    void* player = marioGetPtr();
    HitCheckArg hit;
    f32 offsetX[5];
    f32 offsetZ[5];
    f32 dir;
    f32 radians;
    f32 sideX;
    f32 sideZ;
    f32 height;
    f32 reach;
    f32 bias;
    f32 nx;
    f32 ny;
    f32 nz;
    f32 flatLength;
    f32 hitY;
    s32 result;
    s32 i;

    *(f32*)((s32)player + 0x218) = float_0_804208ac;
    *(f32*)((s32)player + 0x21C) = float_0_804208ac;
    *(f32*)((s32)player + 0x220) = float_0_804208ac;
    *(f32*)((s32)player + 0x224) = float_0_804208ac;

    if (*(u32*)player & 0x300) {
        *outY = *(f32*)((s32)player + 0x90);
        return 0;
    }

    height = float_0p75_804208e0 * *(f32*)((s32)player + 0x1BC);
    if (*(f32*)((s32)player + 0x180) == float_0_804208ac) {
        dir = toMovedir(*(f32*)((s32)player + 0x1AC));
    } else {
        dir = (f32)revise360(*(f32*)((s32)player + 0x1A4) - *(f32*)((s32)player + 0x19C));
    }
    radians = (float_3p1416_804208e8 * dir) / float_180_804208d4;
    reach = float_0p25_804208dc * *(f32*)((s32)player + 0x1B8);
    sideX = reach * (f32)sin(radians);
    sideZ = reach * -(f32)cos(radians);

    offsetX[0] = 0.0f;
    offsetZ[0] = 0.0f;
    offsetX[1] = sideX;
    offsetZ[1] = sideZ;
    offsetX[2] = -sideZ;
    offsetZ[2] = sideX;
    offsetX[3] = sideZ;
    offsetZ[3] = -sideX;
    offsetX[4] = -sideX;
    offsetZ[4] = -sideZ;

    for (i = 0; i < 5; i++) {
        hit.start.x = *(f32*)((s32)player + 0x8C) + offsetX[i];
        hit.start.y = *(f32*)((s32)player + 0x90) + height;
        hit.start.z = *(f32*)((s32)player + 0x94) + offsetZ[i];
        hit.end = vec3_802c3bf0;
        hit.radius = height + (f32)__fabs(velocityY) + float_0p01_80420964;

        if (*(u16*)((s32)marioGetPtr() + 0x2E) == 0x1C) {
            result = hitCheckVecFilter(&hit, chkfilterVecVivian);
        } else if (*(u32*)marioGetPtr() & 0x01000000) {
            result = hitCheckVecFilter(&hit, chkfilterVecRoll);
        } else {
            result = hitCheckVecFilter(&hit, chkfilterVec);
        }
        if (result == 0) {
            continue;
        }

        bias = hit.radius >= 0.0f ? float_0p5_804208cc : float_neg0p5_804208d0;
        hit.radius = (f32)(s32)(hit.radius * float_1000_804208a8 + bias) / float_1000_804208a8;
        bias = hit.normal.x >= 0.0f ? float_0p5_804208cc : float_neg0p5_804208d0;
        nx = (f32)(s32)(hit.normal.x * float_1000_804208a8 + bias) / float_1000_804208a8;
        bias = hit.normal.y >= 0.0f ? float_0p5_804208cc : float_neg0p5_804208d0;
        ny = (f32)(s32)(hit.normal.y * float_1000_804208a8 + bias) / float_1000_804208a8;
        bias = hit.normal.z >= 0.0f ? float_0p5_804208cc : float_neg0p5_804208d0;
        nz = (f32)(s32)(hit.normal.z * float_1000_804208a8 + bias) / float_1000_804208a8;

        *(f32*)((s32)player + 0x21C) =
            (f32)(angleABf(0.0, 0.0, float_100_80420900 * nz,
                           float_100_80420900 * ny) - float_180_804208d4);
        *(f32*)((s32)player + 0x220) = 0.0f;
        *(f32*)((s32)player + 0x224) =
            (f32)(angleABf(0.0, 0.0, float_100_80420900 * nx,
                           float_100_80420900 * ny) - float_180_804208d4);
        flatLength = (f32)sqrt(nx * nx + nz * nz);
        *(f32*)((s32)player + 0x218) =
            (f32)angleABf(0.0, 0.0, flatLength, -ny);

        hitY = hit.start.y - hit.radius;
        if (__fabs(hitY - hit.hitPos.y) < float_0p1_804208ec) {
            *outY = hit.hitPos.y;
            return result;
        }
    }

    return 0;
}

s32 marioChkLandon2(f64 velocityY, f64 yOffset, f32* outY) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;
    typedef struct HitCheckArg {
        u8 pad[0xC];
        Vec start;
        Vec end;
        Vec normal;
        Vec hitPos;
        f32 radius;
    } HitCheckArg;

    extern void* marioGetPtr(void);
    extern f32 toMovedir(f64 angle);
    extern f64 sin(f64 angle);
    extern f64 cos(f64 angle);
    extern f64 sqrt(f64 value);
    extern f64 __fabs(f64 value);
    extern f64 angleABf(f64 x1, f64 y1, f64 x2, f64 y2);
    extern s32 hitCheckVecFilter(HitCheckArg* hit, void* filter);
    extern s32 chkfilterVecVivian(s32, int);
    extern s32 chkfilterVecRoll(s32, int);
    extern s32 chkfilterVec(s32, int);
    extern Vec vec3_802c3bf0;
    extern f32 float_0_804208ac;
    extern f32 float_0p1_804208ec;
    extern f32 float_0p25_804208dc;
    extern f32 float_0p5_804208cc;
    extern f32 float_neg0p5_804208d0;
    extern f32 float_3p1416_804208e8;
    extern f32 float_100_80420900;
    extern f32 float_180_804208d4;
    extern f32 float_1000_804208a8;

    void* player = marioGetPtr();
    HitCheckArg hit;
    f32 offsetX[5];
    f32 offsetZ[5];
    f32 dir;
    f32 radians;
    f32 sideX;
    f32 sideZ;
    f32 height;
    f32 reach;
    f32 bias;
    f32 nx;
    f32 ny;
    f32 nz;
    f32 flatLength;
    f32 hitY;
    s32 result;
    s32 i;

    *(f32*)((s32)player + 0x218) = float_0_804208ac;
    *(f32*)((s32)player + 0x21C) = float_0_804208ac;
    *(f32*)((s32)player + 0x220) = float_0_804208ac;
    *(f32*)((s32)player + 0x224) = float_0_804208ac;

    if (*(u32*)player & 0x300) {
        *outY = *(f32*)((s32)player + 0x90);
        return 0;
    }

    height = float_0p5_804208cc * *(f32*)((s32)player + 0x1BC);
    if (*(f32*)((s32)player + 0x180) == float_0_804208ac) {
        dir = toMovedir(*(f32*)((s32)player + 0x1AC));
    } else {
        dir = *(f32*)((s32)player + 0x1A4);
    }
    radians = (float_3p1416_804208e8 * dir) / float_180_804208d4;
    reach = float_0p25_804208dc * *(f32*)((s32)player + 0x1B8);
    sideX = reach * (f32)sin(radians);
    sideZ = reach * -(f32)cos(radians);

    offsetX[0] = 0.0f;
    offsetZ[0] = 0.0f;
    offsetX[1] = sideX;
    offsetZ[1] = sideZ;
    offsetX[2] = -sideZ;
    offsetZ[2] = sideX;
    offsetX[3] = sideZ;
    offsetZ[3] = -sideX;
    offsetX[4] = -sideX;
    offsetZ[4] = -sideZ;

    for (i = 0; i < 5; i++) {
        hit.start.x = *(f32*)((s32)player + 0x8C) + offsetX[i];
        hit.start.y = *(f32*)((s32)player + 0x90) + (f32)yOffset + height;
        hit.start.z = *(f32*)((s32)player + 0x94) + offsetZ[i];
        hit.end = vec3_802c3bf0;
        hit.radius = height + (f32)__fabs(velocityY) + float_0p1_804208ec;

        if (*(u16*)((s32)marioGetPtr() + 0x2E) == 0x1C) {
            result = hitCheckVecFilter(&hit, chkfilterVecVivian);
        } else if (*(u32*)marioGetPtr() & 0x01000000) {
            result = hitCheckVecFilter(&hit, chkfilterVecRoll);
        } else {
            result = hitCheckVecFilter(&hit, chkfilterVec);
        }
        if (result == 0) {
            continue;
        }

        bias = hit.radius >= 0.0f ? float_0p5_804208cc : float_neg0p5_804208d0;
        hit.radius = (f32)(s32)(hit.radius * float_1000_804208a8 + bias) / float_1000_804208a8;
        bias = hit.normal.x >= 0.0f ? float_0p5_804208cc : float_neg0p5_804208d0;
        nx = (f32)(s32)(hit.normal.x * float_1000_804208a8 + bias) / float_1000_804208a8;
        bias = hit.normal.y >= 0.0f ? float_0p5_804208cc : float_neg0p5_804208d0;
        ny = (f32)(s32)(hit.normal.y * float_1000_804208a8 + bias) / float_1000_804208a8;
        bias = hit.normal.z >= 0.0f ? float_0p5_804208cc : float_neg0p5_804208d0;
        nz = (f32)(s32)(hit.normal.z * float_1000_804208a8 + bias) / float_1000_804208a8;

        *(f32*)((s32)player + 0x21C) =
            (f32)(angleABf(0.0, 0.0, float_100_80420900 * nz,
                           float_100_80420900 * ny) - float_180_804208d4);
        *(f32*)((s32)player + 0x220) = 0.0f;
        *(f32*)((s32)player + 0x224) =
            (f32)(angleABf(0.0, 0.0, float_100_80420900 * nx,
                           float_100_80420900 * ny) - float_180_804208d4);
        flatLength = (f32)sqrt(nx * nx + nz * nz);
        *(f32*)((s32)player + 0x218) =
            (f32)angleABf(0.0, 0.0, flatLength, -ny);

        hitY = hit.start.y - hit.radius;
        if (__fabs(hitY - hit.hitPos.y) < float_0p1_804208ec) {
            *outY = hit.hitPos.y;
            return result;
        }
    }

    return 0;
}

s32 marioChkFrontStep(f32* outY, f32* outDrop, f32* outAngle) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;
    typedef struct HitCheckArg {
        u8 pad[0xC];
        Vec start;
        Vec end;
        Vec normal;
        Vec hitPos;
        f32 radius;
    } HitCheckArg;

    extern void* marioGetPtr(void);
    extern f32 toMovedir(f64);
    extern f64 revise360(f64);
    extern f64 sin(f64);
    extern f64 cos(f64);
    extern f64 angleABf(f64, f64, f64, f64);
    extern s32 hitCheckVecFilter(HitCheckArg*, void*);
    extern s32 chkfilterVecVivian(s32, int);
    extern s32 chkfilterVecRoll(s32, int);
    extern s32 chkfilterVec(s32, int);
    extern Vec vec3_802c3d04;
    extern f32 float_0_804208ac;
    extern f32 float_0p1_804208ec;
    extern f32 float_0p265_804208fc;
    extern f32 float_0p5_804208cc;
    extern f32 float_neg0p5_804208d0;
    extern f32 float_neg1_804208e4;
    extern f32 float_2_804208f0;
    extern f32 float_3_804208f8;
    extern f32 float_3p1416_804208e8;
    extern f32 float_11_804208b4;
    extern f32 float_18p5_804208f4;
    extern f32 float_100_80420900;
    extern f32 float_180_804208d4;
    extern f32 float_1000_804208a8;

    void* player;
    HitCheckArg hit;
    f32 height;
    f32 reach;
    f32 dir;
    f32 rad;
    f32 sx;
    f32 cz;
    f32 bias;
    f32 nx;
    f32 ny;
    f32 nz;
    f32 hx;
    f32 hy;
    f32 hz;
    f32 flatLen;
    s32 result;

    player = marioGetPtr();
    *(f32*)((s32)player + 0x21C) = float_0_804208ac;
    *(f32*)((s32)player + 0x220) = float_0_804208ac;
    *(f32*)((s32)player + 0x224) = float_0_804208ac;
    *(f32*)((s32)player + 0x218) = float_0_804208ac;

    height = float_0p5_804208cc * *(f32*)((s32)player + 0x1BC);
    if ((*(u32*)player & 0x01000000) != 0) {
        height = float_18p5_804208f4;
    }

    *outY = *(f32*)((s32)player + 0x90);
    *outDrop = float_neg1_804208e4;

    if (*(f32*)((s32)player + 0x180) == float_0_804208ac) {
        dir = toMovedir(*(f32*)((s32)player + 0x1AC));
    } else {
        dir = *(f32*)((s32)player + 0x1A4);
    }
    dir = (f32)revise360((f32)(s32)(dir + float_0p5_804208cc));

    reach = float_3_804208f8;
    if ((*(u32*)player & 0x01000000) == 0) {
        reach = float_0p265_804208fc * *(f32*)((s32)player + 0x1B8);
    }

    rad = (float_3p1416_804208e8 * dir) / float_180_804208d4;
    sx = (f32)sin(rad);
    cz = (f32)cos(rad);

    hit.start.x = *(f32*)((s32)player + 0x8C) + reach * sx;
    hit.start.y = *(f32*)((s32)player + 0x90) + height;
    hit.start.z = *(f32*)((s32)player + 0x94) + reach * -cz;
    hit.end = vec3_802c3d04;
    hit.radius = float_0p1_804208ec + float_11_804208b4 + height;

    if (*(u16*)((s32)marioGetPtr() + 0x2E) == 0x1C) {
        result = hitCheckVecFilter(&hit, chkfilterVecVivian);
    } else if ((*(u32*)marioGetPtr() & 0x01000000) != 0) {
        result = hitCheckVecFilter(&hit, chkfilterVecRoll);
    } else {
        result = hitCheckVecFilter(&hit, chkfilterVec);
    }

    if (result == 0) {
        return 0;
    }

    bias = float_neg0p5_804208d0;
    if (hit.radius >= float_0_804208ac) {
        bias = float_0p5_804208cc;
    }
    hit.radius = (f32)(s32)(hit.radius * float_1000_804208a8 + bias) / float_1000_804208a8;

    bias = float_neg0p5_804208d0;
    if (hit.normal.x >= float_0_804208ac) {
        bias = float_0p5_804208cc;
    }
    nx = (f32)(s32)(hit.normal.x * float_1000_804208a8 + bias) / float_1000_804208a8;

    bias = float_neg0p5_804208d0;
    if (hit.normal.y >= float_0_804208ac) {
        bias = float_0p5_804208cc;
    }
    ny = (f32)(s32)(hit.normal.y * float_1000_804208a8 + bias) / float_1000_804208a8;

    bias = float_neg0p5_804208d0;
    if (hit.normal.z >= float_0_804208ac) {
        bias = float_0p5_804208cc;
    }
    nz = (f32)(s32)(hit.normal.z * float_1000_804208a8 + bias) / float_1000_804208a8;

    hx = hit.hitPos.x;
    hy = hit.hitPos.y;
    hz = hit.hitPos.z;

    *(f32*)((s32)player + 0x21C) = (f32)(s32)(float_100_80420900 *
        ((f32)angleABf(float_0_804208ac, float_0_804208ac, float_100_80420900 * nz, float_100_80420900 * ny) - float_180_804208d4) +
        float_0p5_804208cc) / float_100_80420900;
    *(f32*)((s32)player + 0x220) = float_0_804208ac;
    *(f32*)((s32)player + 0x224) = (f32)(s32)(float_100_80420900 *
        ((f32)angleABf(float_0_804208ac, float_0_804208ac, float_100_80420900 * nx, float_100_80420900 * ny) - float_180_804208d4) +
        float_0p5_804208cc) / float_100_80420900;

    flatLen = nx * nx + nz * nz;
    *(f32*)((s32)player + 0x218) = (f32)angleABf(float_0_804208ac, float_0_804208ac, flatLen, -ny);

    if (*(f32*)((s32)player + 0x90) <= hy ||
        ((hy - *(f32*)((s32)player + 0x90) < float_2_804208f0) &&
         (*(f32*)((s32)player + 0x90) - hy < float_2_804208f0))) {
        *outY = hy;
        *outDrop = float_0_804208ac;
        *outAngle = (f32)angleABf(*(f32*)((s32)player + 0x8C), *(f32*)((s32)player + 0x94), hx, hz);
        return result;
    }

    *outDrop = *(f32*)((s32)player + 0x90) - hy;
    *outAngle = (f32)angleABf(*(f32*)((s32)player + 0x8C), *(f32*)((s32)player + 0x94), hx, hz);
    return 0;
}

s32 marioSearchHead(f64 velocityY, f32* pos, f32* outHeight) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;
    extern void* marioGetPtr(void);
    extern f32 toMovedir(f64);
    extern f64 sin(f64);
    extern f64 cos(f64);
    extern f64 angleABf(f64, f64, f64, f64);
    extern u8 marioHitCheck2(void);
    extern Vec vec3_802c3c44;
    extern f32 float_0_804208ac;
    extern f32 float_0p5_804208cc;
    extern f32 float_neg1_804208e4;
    extern f32 float_neg10_8042090c;
    extern f32 float_3p1416_804208e8;
    extern f32 float_180_804208d4;
    extern f32 float_50_80420954;
    extern f32 float_75_80420950;
    extern f32 float_100_80420900;
    extern f32 float_0p05_80420958;

    void* mario;
    Vec checkPos;
    Vec vec;
    Vec normal;
    Vec hitPos;
    f32 radius;
    f32 dir;
    f32 rad;
    f32 sideX;
    f32 sideZ;
    f32 yHalf;
    f32 offX;
    f32 offZ;
    f32 hitY;
    f32 angle;
    s32 i;
    s32 result;

    mario = marioGetPtr();
    if (*(f32*)((s32)mario + 0x180) == float_0_804208ac) {
        dir = toMovedir(*(f32*)((s32)mario + 0x1AC));
    } else {
        dir = *(f32*)((s32)mario + 0x1A4);
    }
    rad = (float_3p1416_804208e8 * dir) / float_180_804208d4;
    sideX = float_0p5_804208cc * ((*(f32*)((s32)mario + 0x1B8) - float_0p5_804208cc) * (f32)sin(rad));
    sideZ = float_0p5_804208cc * ((*(f32*)((s32)mario + 0x1B8) - float_0p5_804208cc) * -(f32)cos(rad));
    yHalf = float_0p5_804208cc * *outHeight;
    *outHeight = float_neg1_804208e4;

    for (i = 0; i < 5; i++) {
        if (i == 0) {
            offX = float_0_804208ac;
            offZ = float_0_804208ac;
        } else if (i == 1) {
            offX = sideX;
            offZ = sideZ;
        } else if (i == 2) {
            offX = -sideZ;
            offZ = sideX;
        } else if (i == 3) {
            offX = sideZ;
            offZ = -sideX;
        } else {
            offX = -sideX;
            offZ = -sideZ;
        }

        checkPos.x = pos[0] + offX;
        checkPos.y = pos[1] + yHalf;
        checkPos.z = pos[2] + offZ;
        vec = vec3_802c3c44;
        radius = *outHeight + (f32)velocityY;
        result = ((s32 (*)(void*, void*, void*, void*, void*))marioHitCheck2)(&checkPos, &vec, &normal, &hitPos, &radius);
        if (result != 0) {
            angle = (f32)angleABf(float_0_804208ac, float_0_804208ac,
                                  float_100_80420900 * normal.x,
                                  float_100_80420900 * normal.z);
            *(f32*)((s32)mario + 0x21C) = (f32)(s32)(float_100_80420900 * (angle - float_180_804208d4) + float_0p5_804208cc) / float_100_80420900;
            *(f32*)((s32)mario + 0x220) = float_0_804208ac;
            *(f32*)((s32)mario + 0x224) = *(f32*)((s32)mario + 0x21C);

            if (!(*(u32*)mario & 0x01000000)) {
                f32 limit = float_50_80420954;
                if (*(u32*)((s32)mario + 4) & 0x01000000) {
                    limit = float_75_80420950;
                }
                if (angle <= float_180_804208d4 - limit && limit <= angle) {
                    result = 0;
                }
            }
        }

        if (!(*(u32*)mario & 0x01000000)) {
            if (result != 0) {
                hitY = (f32)(s32)(float_100_80420900 * hitPos.y + yHalf) / float_100_80420900;
                if (*(f32*)((s32)mario + 0x7C) < float_neg10_8042090c) {
                    *outHeight = (hitY - pos[1]) - *outHeight;
                    return 0;
                }
                if (hitY <= pos[1] + *outHeight) {
                    pos[1] = hitY;
                    *outHeight = float_0_804208ac;
                    return result;
                }
                *outHeight = (hitY - pos[1]) - *outHeight;
                return 0;
            }
        } else if (result != 0) {
            hitY = -((float_0p05_80420958 * *outHeight) - (f32)(s32)(float_100_80420900 * hitPos.y + yHalf) / float_100_80420900);
            if (hitY <= pos[1] + *outHeight) {
                pos[1] = hitY;
                *outHeight = float_0_804208ac;
                return result;
            }
            *outHeight = (hitY - pos[1]) - *outHeight;
            return 0;
        }
    }
    return 0;
}

s32 marioSlitChkWallAround(void) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;
    typedef struct HitCheckArg {
        u8 pad[0xC];
        Vec start;
        Vec end;
        Vec normal;
        Vec hitPos;
        f32 radius;
    } HitCheckArg;

    extern void* marioGetPtr(void);
    extern void sincosf(f32, f32*, f32*);
    extern s32 hitCheckVecFilter(HitCheckArg*, void*);
    extern s32 chkfilterVecVivian(s32, int);
    extern s32 chkfilterVecRoll(s32, int);
    extern s32 chkfilterVec(s32, int);
    extern u32 hitGetAttr(void*);
    extern Vec vec3_802c3b78;
    extern f32 float_0_804208ac;
    extern f32 float_1_804208c4;
    extern f32 float_5_804208c0;
    extern f32 float_11_804208b4;
    extern f32 float_13_804208b8;
    extern f32 float_45_804208bc;

    void* player;
    HitCheckArg check;
    s32 blocked[8];
    f32 s;
    f32 c;
    f32 angle;
    f32 x;
    f32 y;
    f32 z;
    void* hit;
    u32 attr;
    s32 i;

    player = marioGetPtr();
    x = *(f32*)((s32)player + 0x8C);
    y = *(f32*)((s32)player + 0x90) + float_11_804208b4;
    z = *(f32*)((s32)player + 0x94);

    angle = float_0_804208ac;
    for (i = 0; i < 8; i++, angle += float_45_804208bc) {
        blocked[i] = 0;
        sincosf(angle, &s, &c);
        check.start.x = x;
        check.start.y = y;
        check.start.z = z;
        check.end = vec3_802c3b78;
        check.end.x = s;
        check.end.z = c;
        check.radius = float_13_804208b8;

        if (*(u16*)((s32)marioGetPtr() + 0x2E) == 0x1C) {
            hit = (void*)hitCheckVecFilter(&check, chkfilterVecVivian);
        } else if ((*(u32*)marioGetPtr() & 0x01000000) != 0) {
            hit = (void*)hitCheckVecFilter(&check, chkfilterVecRoll);
        } else {
            hit = (void*)hitCheckVecFilter(&check, chkfilterVec);
        }

        if (hit != 0) {
            attr = hitGetAttr(hit);
            if ((attr & 0x80000) == 0 || (*(u32*)marioGetPtr() & 0x01000000) != 0) {
                if ((attr & 0x80) == 0) {
                    attr = (attr >> 0x17) & 1;
                } else {
                    attr = (*(u16*)((s32)marioGetPtr() + 0x2E) == 0x19);
                }
            } else {
                attr = 0;
            }
            if (attr == 0) {
                blocked[i] = 1;
            }
        }
    }

    y = *(f32*)((s32)player + 0x90) + float_5_804208c0;
    angle = float_0_804208ac;
    for (i = 0; i < 8; i++, angle += float_45_804208bc) {
        if (blocked[i] == 0) {
            sincosf(angle, &s, &c);
            check.start.x = x;
            check.start.y = y;
            check.start.z = z;
            check.end = vec3_802c3b78;
            check.end.x = s;
            check.end.z = c;
            check.radius = float_13_804208b8;

            if (*(u16*)((s32)marioGetPtr() + 0x2E) == 0x1C) {
                hit = (void*)hitCheckVecFilter(&check, chkfilterVecVivian);
            } else if ((*(u32*)marioGetPtr() & 0x01000000) != 0) {
                hit = (void*)hitCheckVecFilter(&check, chkfilterVecRoll);
            } else {
                hit = (void*)hitCheckVecFilter(&check, chkfilterVec);
            }

            if (hit != 0) {
                attr = hitGetAttr(hit);
                if ((attr & 0x80000) == 0 || (*(u32*)marioGetPtr() & 0x01000000) != 0) {
                    if ((attr & 0x80) == 0) {
                        attr = (attr >> 0x17) & 1;
                    } else {
                        attr = (*(u16*)((s32)marioGetPtr() + 0x2E) == 0x19);
                    }
                } else {
                    attr = 0;
                }
                if (attr == 0) {
                    blocked[i] = 1;
                }
            }
        }
    }

    y = *(f32*)((s32)player + 0x90) + (*(f32*)((s32)player + 0x1BC) - float_1_804208c4);
    angle = float_0_804208ac;
    for (i = 0; i < 8; i++, angle += float_45_804208bc) {
        if (blocked[i] == 0) {
            sincosf(angle, &s, &c);
            check.start.x = x;
            check.start.y = y;
            check.start.z = z;
            check.end = vec3_802c3b78;
            check.end.x = s;
            check.end.z = c;
            check.radius = float_13_804208b8;

            if (*(u16*)((s32)marioGetPtr() + 0x2E) == 0x1C) {
                hit = (void*)hitCheckVecFilter(&check, chkfilterVecVivian);
            } else if ((*(u32*)marioGetPtr() & 0x01000000) != 0) {
                hit = (void*)hitCheckVecFilter(&check, chkfilterVecRoll);
            } else {
                hit = (void*)hitCheckVecFilter(&check, chkfilterVec);
            }

            if (hit != 0) {
                attr = hitGetAttr(hit);
                if ((attr & 0x80000) == 0 || (*(u32*)marioGetPtr() & 0x01000000) != 0) {
                    if ((attr & 0x80) == 0) {
                        attr = (attr >> 0x17) & 1;
                    } else {
                        attr = (*(u16*)((s32)marioGetPtr() + 0x2E) == 0x19);
                    }
                } else {
                    attr = 0;
                }
                if (attr == 0) {
                    blocked[i] = 1;
                }
            }
        }
    }

    for (i = 0; i < 8; i++) {
        if (blocked[i] == 0 &&
            blocked[(i + 1) & 7] == 0 &&
            blocked[(i + 2) & 7] == 0) {
            return 0;
        }
    }

    return 1;
}

void marioCheckWall_roll(f64 speed, f64 angle) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;
    typedef struct HitCheckArg {
        u8 pad[0xC];
        Vec start;
        Vec end;
        Vec normal;
        Vec hitPos;
        f32 radius;
    } HitCheckArg;

    extern void* marioGetPtr(void);
    extern s32 hitCheckVecFilter(HitCheckArg*, void*);
    extern s32 chkfilterVecVivian(s32, int);
    extern s32 chkfilterVecRoll(s32, int);
    extern s32 chkfilterVec(s32, int);
    extern u8 marioSearchFrontWall_staying(void);
    extern u8 searchFrontWall(s64, s64, s64, void*, s32);
    extern u32 hitGetAttr(void*);
    extern void movePos(f64, f64, void*, void*);
    extern s32 marioChkWallAround(void*, s32, f32, f32, f32);
    extern Vec vec3_802c3c74;
    extern f32 float_0_804208ac;
    extern f32 float_1000_804208a8;
    extern f32 float_30_80420928;
    extern f32 float_16p8_80420938;
    extern f32 float_0p5_804208cc;
    extern f32 float_neg0p5_804208d0;

    void* mario;
    void* hit;
    HitCheckArg check;
    Vec pos;
    f32 rounded;
    f32 bias;
    u32 attr;

    mario = marioGetPtr();
    *(f32*)((s32)mario + 0x214) = float_1000_804208a8;

    if (speed == float_0_804208ac || *(s16*)((s32)mario + 0x50) != 0) {
        *(f32*)((s32)mario + 0x1CC) = float_0_804208ac;
    } else if (*(f32*)((s32)mario + 0x1CC) >= (f32)speed) {
        *(s32*)((s32)mario + 0x1D4) = *(s32*)((s32)mario + 0x1D4) - 1;
        if (*(s32*)((s32)mario + 0x1D4) < 1) {
            *(s32*)((s32)mario + 0x1D4) = 0;
            *(f32*)((s32)mario + 0x1CC) = (f32)speed;
            *(f32*)((s32)mario + 0x1D0) = (f32)angle;
        }
    } else {
        *(f32*)((s32)mario + 0x1CC) = (f32)speed;
        *(f32*)((s32)mario + 0x1D0) = (f32)angle;
        *(s32*)((s32)mario + 0x1D4) = 0xE;
    }

    if (speed != float_0_804208ac && *(s16*)((s32)mario + 0x50) == 0) {
        *(f32*)((s32)mario + 0x1A0) = *(f32*)((s32)mario + 0x1D0);
    }

    if ((*(u32*)mario & 0x10000) != 0) {
        check.start = *(Vec*)((s32)mario + 0x8C);
        check.end = vec3_802c3c74;
        check.radius = float_30_80420928;

        if (*(u16*)((s32)marioGetPtr() + 0x2E) == 0x1C) {
            hit = (void*)hitCheckVecFilter(&check, chkfilterVecVivian);
        } else if ((*(u32*)marioGetPtr() & 0x01000000) != 0) {
            hit = (void*)hitCheckVecFilter(&check, chkfilterVecRoll);
        } else {
            hit = (void*)hitCheckVecFilter(&check, chkfilterVec);
        }

        if (hit != 0) {
            bias = float_neg0p5_804208d0;
            if (check.radius >= float_0_804208ac) {
                bias = float_0p5_804208cc;
            }
            rounded = (f32)(s32)(check.radius * float_1000_804208a8 + bias) / float_1000_804208a8;

            bias = float_neg0p5_804208d0;
            if (check.normal.x >= float_0_804208ac) {
                bias = float_0p5_804208cc;
            }
            check.normal.x = (f32)(s32)(check.normal.x * float_1000_804208a8 + bias) / float_1000_804208a8;

            bias = float_neg0p5_804208d0;
            if (check.normal.y >= float_0_804208ac) {
                bias = float_0p5_804208cc;
            }
            check.normal.y = (f32)(s32)(check.normal.y * float_1000_804208a8 + bias) / float_1000_804208a8;

            bias = float_neg0p5_804208d0;
            if (check.normal.z >= float_0_804208ac) {
                bias = float_0p5_804208cc;
            }
            check.normal.z = (f32)(s32)(check.normal.z * float_1000_804208a8 + bias) / float_1000_804208a8;

            if (rounded > float_16p8_80420938) {
                hit = 0;
            }
        }

        *(void**)((s32)mario + 0x1F8) = hit;
        if (hit != 0) {
            *(f32*)((s32)mario + 0x90) = check.normal.y - *(f32*)((s32)mario + 0x1BC);
        }
    }

    if (speed == float_0_804208ac) {
        ((void (*)(f64))marioSearchFrontWall_staying)(angle);
        return;
    }

    if (*(void**)((s32)mario + 0x1F8) != 0) {
        *(f32*)((s32)mario + 0x180) = float_0_804208ac;
        *(f32*)((s32)mario + 0x7C) = float_0_804208ac;
        *(f32*)((s32)mario + 0x80) = float_0_804208ac;
        *(f32*)((s32)mario + 0x84) = float_0_804208ac;
        *(f32*)((s32)mario + 0x88) = float_0_804208ac;
        speed = float_0_804208ac;
    }

    pos = *(Vec*)((s32)mario + 0x8C);
    rounded = float_0_804208ac;
    hit = ((void* (*)(f64, f64, f64, void*, void*))searchFrontWall)(speed, angle, angle, &pos, &rounded);
    if (hit == 0) {
        *(Vec*)((s32)mario + 0x8C) = pos;
    } else {
        *(void**)((s32)mario + 0x1E4) = hit;
        attr = hitGetAttr(hit);
        if ((attr & 0x80000) == 0 || (*(u32*)marioGetPtr() & 0x01000000) != 0) {
            if ((attr & 0x80) == 0) {
                attr = (attr >> 0x17) & 1;
            } else {
                attr = (*(u16*)((s32)mario + 0x2E) == 0x19);
            }
        } else {
            attr = 0;
        }

        if (attr == 0) {
            *(f32*)((s32)mario + 0x214) = rounded;
            *(void**)((s32)mario + 0x1FC) = hit;
            *(s8*)((s32)mario + 0x3F) = *(s8*)((s32)mario + 0x3F) + 1;
            if (*(s8*)((s32)mario + 0x3F) > 0x13) {
                *(s8*)((s32)mario + 0x3F) = 0x14;
                attr = hitGetAttr(hit);
                if ((attr & 0x80) == 0 || *(u16*)((s32)marioGetPtr() + 0x2E) == 0x19) {
                    *(void**)((s32)mario + 0x1F0) = hit;
                }
            }
            *(Vec*)((s32)mario + 0x8C) = pos;
        } else {
            if (speed != float_0_804208ac) {
                *(void**)((s32)mario + 0x1F0) = hit;
                *(void**)((s32)mario + 0x1FC) = hit;
            }
            movePos(speed, angle, (void*)((s32)mario + 0x8C), (void*)((s32)mario + 0x94));
        }
    }

    pos = *(Vec*)((s32)mario + 0x8C);
    ((s32 (*)(f64, f64, f64, f64, void*, s32))marioChkWallAround)(speed, angle, speed, angle, &pos, 0);
    *(f32*)((s32)mario + 0x8C) = pos.x;
    *(f32*)((s32)mario + 0x94) = pos.z;
}

s32 marioSearchHead_roll(f64 heightAdd, f32* pos, f32* outHeight) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;
    typedef struct HitCheckArg {
        u8 pad[0xC];
        Vec start;
        Vec end;
        Vec normal;
        Vec hitPos;
        f32 radius;
    } HitCheckArg;
    extern void* marioGetPtr(void);
    extern f64 sin(f64);
    extern f64 cos(f64);
    extern void movePos(f64, f64, void*, void*);
    extern s32 hitCheckVecFilter(HitCheckArg*, void*);
    extern s32 chkfilterVecVivian(s32, int);
    extern s32 chkfilterVecRoll(s32, int);
    extern s32 chkfilterVec(s32, int);
    extern Vec vec3_802c3c50;
    extern Vec vec3_802c3c5c;
    extern f32 float_neg1_804208e4;
    extern f32 float_1_804208c4;
    extern f32 float_0_804208ac;
    extern f32 float_3p1416_804208e8;
    extern f32 float_180_804208d4;
    extern f32 float_90_80420924;
    extern f32 float_9_8042094c;
    extern f32 float_3_804208f8;
    extern f32 float_1000_804208a8;
    extern f32 float_0p5_804208cc;
    extern f32 float_neg0p5_804208d0;

    void* mario;
    HitCheckArg hit;
    Vec start;
    Vec offset;
    f32 playerHeight;
    f32 maxY;
    f32 delta;
    f32 bias;
    f64 rad;
    s32 i;
    s32 result;

    mario = marioGetPtr();
    *outHeight = float_neg1_804208e4;
    playerHeight = *(f32*)((s32)mario + 0x1BC);
    maxY = playerHeight + (f32)heightAdd;

    for (i = 0; i < 5; i++) {
        if (i == 0) {
            offset.x = float_0_804208ac;
            offset.z = float_0_804208ac;
        } else {
            f32 angle;
            if (i == 1 || i == 3) {
                angle = float_90_80420924 + *(f32*)((s32)mario + 0x1A4);
            } else {
                angle = *(f32*)((s32)mario + 0x1A4) - float_90_80420924;
            }
            rad = (f64)((float_3p1416_804208e8 * angle) / float_180_804208d4);
            offset.x = float_9_8042094c * (f32)sin(rad);
            offset.z = -(float_9_8042094c * (f32)cos(rad));
            if (i >= 3) {
                movePos(float_3_804208f8, float_180_804208d4 + *(f32*)((s32)mario + 0x1A4), &offset.x, &offset.z);
            } else {
                movePos(float_3_804208f8, *(f32*)((s32)mario + 0x1A4), &offset.x, &offset.z);
            }
        }

        start.x = pos[0] + offset.x;
        start.y = pos[1] + float_1_804208c4;
        start.z = pos[2] + offset.z;
        hit.start = start;
        hit.end = vec3_802c3c5c;
        hit.radius = maxY;

        if (*(u16*)((s32)marioGetPtr() + 0x2E) == 0x1C) {
            result = hitCheckVecFilter(&hit, chkfilterVecVivian);
        } else if (*(u32*)marioGetPtr() & 0x01000000) {
            result = hitCheckVecFilter(&hit, chkfilterVecRoll);
        } else {
            result = hitCheckVecFilter(&hit, chkfilterVec);
        }

        if (result != 0) {
            bias = float_neg0p5_804208d0;
            if (hit.normal.y >= float_0_804208ac) {
                bias = float_0p5_804208cc;
            }
            hit.normal.y = (f32)(s32)(hit.normal.y * float_1000_804208a8 + bias) / float_1000_804208a8;
            if (hit.normal.y <= pos[1] + playerHeight) {
                pos[1] = (hit.normal.y - playerHeight) - float_1_804208c4;
                *outHeight = float_0_804208ac;
                return result;
            }
            delta = (hit.normal.y - pos[1]) - playerHeight;
            if (*outHeight >= float_0_804208ac) {
                if (delta < *outHeight) {
                    *outHeight = delta;
                }
            } else {
                *outHeight = delta;
            }
        }
    }
    return 0;
}

f64 marioChkOverhead(void) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;
    extern void* marioGetPtr(void);
    extern s32 jabaraNoHitChk(void);
    extern u8 marioSearchHead_jabara(void);
    extern u8 marioSearchHead(void);
    extern u8 marioSearchHead_roll(void);
    extern void* kpaSearchHead(void*, void*);
    extern u32 hitGetAttr(void*);
    extern void* mobjHitObjPtrToPtr(s32);
    extern s32 strcmp(char*, char*);
    extern void marioItemGetDisable(void);
    extern s32 kpaGetBodyStatus(void);
    extern void marioClearJumpPara(void);
    extern s32 marioGetPartyId(void);
    extern void* partyGetPtr(s32);
    extern f64 partyGetHeight(void*);
    extern s32 marioChkSlitEnd(void);
    extern void marioChgStayMotion(void);
    extern void marioChgMot(s32);
    extern void psndSFXOn_3D(s32, void*);
    extern void marioSetFallPara(void);
    extern char str_MOBJ_PowerUpBlock_802c3d98[];
    extern char str_MOBJ_SaveBlock_802c3dac[];
    extern char str_MOBJ_RecoveryBlock_802c3dbc[];
    extern char str_MOBJ_CoinTakenRecove_802c3dd0[];
    extern f32 float_0_804208ac;
    extern f32 float_5_804208c0;
    extern f32 float_1_804208c4;
    extern f32 float_10_80420904;
    extern f32 float_40_80420908;
    extern f32 float_47_8042095c;
    extern f32 float_0p5_804208cc;

    void* player;
    void* hit;
    void* mobj;
    void* party;
    Vec pos;
    f32 height;
    f32 newY;
    f32 vel;
    s32 powerBlock;
    s32 motion;
    u8 temp[4];

    player = marioGetPtr();
    if (*(u16*)((s32)player + 0x2E) == 8) {
        return *(f32*)((s32)player + 0x90);
    }

    if (*(u16*)((s32)player + 0x2E) == 0x14) {
        pos = *(Vec*)((s32)player + 0x8C);
        height = *(f32*)((s32)player + 0x1BC);
        if (jabaraNoHitChk() == 0) {
            height += float_5_804208c0;
            hit = ((void* (*)(f64, void*, void*, void*))marioSearchHead_jabara)(float_0_804208ac, &pos, &height, temp);
            *(void**)((s32)player + 0x1F8) = hit;
            if (*(void**)((s32)player + 0x1F8) == 0) {
                if (*(u32*)player & 0x10000) {
                    vel = *(f32*)((s32)player + 0x7C);
                } else {
                    vel = float_10_80420904;
                }
                hit = ((void* (*)(f64, void*, void*))marioSearchHead)(vel, &pos, &height);
                *(void**)((s32)player + 0x1F8) = hit;
            }
        }
    } else if (*(u32*)player & 0x01000000) {
        pos = *(Vec*)((s32)player + 0x8C);
        hit = ((void* (*)(f64, void*, void*))marioSearchHead_roll)(float_40_80420908, &pos, &height);
        *(void**)((s32)player + 0x1F8) = hit;
    } else {
        pos = *(Vec*)((s32)player + 0x8C);
        height = *(f32*)((s32)player + 0x1BC);
        if (*(u32*)player & 0x10000) {
            vel = *(f32*)((s32)player + 0x7C);
        } else {
            vel = float_10_80420904;
        }
        if (*(s8*)((s32)player + 0x3C) == 2) {
            hit = kpaSearchHead(&pos, &height);
        } else {
            hit = ((void* (*)(f64, void*, void*))marioSearchHead)(vel, &pos, &height);
        }
        *(void**)((s32)player + 0x1F8) = hit;
    }

    *(f32*)((s32)player + 0x8C) = pos.x;
    hit = *(void**)((s32)player + 0x1F8);
    if (hit == 0) {
        if (!(*(u32*)player & 0x01000000)) {
            if (height >= float_0_804208ac && height <= float_5_804208c0) {
                *(u32*)player |= 0x08000000;
            } else {
                *(u32*)player &= ~0x08000000;
            }
        } else if (height >= float_0_804208ac &&
                   (height + *(f32*)((s32)player + 0x1BC)) <= float_47_8042095c) {
            *(u32*)player |= 0x08000000;
        } else {
            *(u32*)player &= ~0x08000000;
        }
        return pos.y;
    }

    powerBlock = 0;
    if (hitGetAttr(hit) & 0x80000000) {
        mobj = mobjHitObjPtrToPtr((s32)hit);
        if (strcmp((char*)((s32)mobj + 0x15), str_MOBJ_PowerUpBlock_802c3d98) == 0 ||
            strcmp((char*)((s32)mobj + 0x15), str_MOBJ_SaveBlock_802c3dac) == 0 ||
            strcmp((char*)((s32)mobj + 0x15), str_MOBJ_RecoveryBlock_802c3dbc) == 0 ||
            strcmp((char*)((s32)mobj + 0x15), str_MOBJ_CoinTakenRecove_802c3dd0) == 0) {
            powerBlock = 1;
        }
    }
    if (powerBlock) {
        *(f32*)((s32)player + 0x180) = float_0_804208ac;
        *(u32*)player |= 0x80;
    }
    *(u32*)player |= 0x08000000;

    if (*(s8*)((s32)player + 0x3C) == 2) {
        marioItemGetDisable();
        if (kpaGetBodyStatus() == 0) {
            marioClearJumpPara();
        }
    }

    player = marioGetPtr();
    height = *(f32*)((s32)player + 0x1BC);
    if (*(u16*)((s32)player + 0x2E) == 0x1A) {
        party = partyGetPtr(marioGetPartyId());
        if (party != 0) {
            height = (f32)partyGetHeight(party);
        }
    }
    newY = (f32)(s32)((pos.y - height) * float_10_80420904 + float_0p5_804208cc) / float_10_80420904 - float_1_804208c4;
    if (!(*(u32*)player & 0x20000) && newY < *(f32*)((s32)player + 0x120)) {
        newY = *(f32*)((s32)player + 0x120);
        if (!(*(u32*)player & 0x01000000) && *(u16*)((s32)player + 0x2E) != 0x19 &&
            *(u16*)((s32)player + 0x2E) != 0x18 && *(u16*)((s32)player + 0x2E) != 0x1C &&
            *(s8*)((s32)player + 0x3C) != 2 && marioChkSlitEnd() == 0) {
            if (*(f32*)((s32)player + 0x180) == float_0_804208ac) {
                if (*(u16*)((s32)player + 0x2E) != 0) {
                    marioChgStayMotion();
                }
            } else {
                if (*(f32*)((s32)player + 0x180) <= *(f32*)((s32)player + 0x184)) {
                    motion = 1;
                } else {
                    motion = 2;
                }
                if (motion != *(u16*)((s32)player + 0x30)) {
                    marioChgMot(motion);
                }
            }
        }
    }
    if (*(u16*)((s32)player + 0x2E) == 3 || *(u16*)((s32)player + 0x2E) == 6) {
        psndSFXOn_3D(0x156, (void*)((s32)player + 0x8C));
        marioClearJumpPara();
        *(u32*)player &= 0xF87FFFFF;
        if (*(s8*)((s32)player + 0x3C) != 2) {
            marioChgMot(10);
            marioSetFallPara();
        }
        *(f32*)((s32)player + 0x90) = newY;
    }
    return newY;
}

void* dou10_yoko_yari(float* outSpeed) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;
    typedef struct HitCheckArg {
        u8 pad[0xC];
        Vec start;
        Vec end;
        Vec normal;
        Vec hitPos;
        f32 radius;
    } HitCheckArg;
    extern void* marioGetPtr(void);
    extern f32 toMovedir(f64);
    extern s32 strcmp(char*, char*);
    extern s32 vivianGetStatus(void);
    extern s32 hitCheckVecFilter(HitCheckArg*, void*);
    extern s32 chkfilterVecVivian(s32, int);
    extern s32 chkfilterVecRoll(s32, int);
    extern s32 chkfilterVec(s32, int);
    extern u32 hitGetAttr(void*);
    extern void movePos(f64, f64, void*, void*);
    extern void* gp;
    extern char str_dou_10_802c3d88[];
    extern Vec vec3_802c3cb0;
    extern f32 float_2_804208f0;
    extern f32 float_60_80420910;
    extern f32 float_100_80420900;
    extern f32 float_0p8_80420914;
    extern f32 float_20_80420918;
    extern f32 float_8_8042091c;
    extern f32 float_270_80420920;
    extern f32 float_90_80420924;
    extern f32 float_0_804208ac;

    void* mario;
    void* hit;
    HitCheckArg check;
    Vec pos;
    f32 sideOffset;
    u8 temp[4];

    mario = marioGetPtr();
    if (*(u16*)((s32)mario + 0x2E) == 0x16) {
        return 0;
    }
    toMovedir(*(f32*)((s32)mario + 0x1AC));
    hit = *(void**)((s32)mario + 0x1E4);
    if (hit != 0) {
        return hit;
    }

    pos = *(Vec*)((s32)mario + 0x8C);
    if (strcmp((char*)((s32)gp + 0x12C), str_dou_10_802c3d88) != 0) {
        return hit;
    }
    if (vivianGetStatus() == 2 || vivianGetStatus() == 1 || vivianGetStatus() == 3) {
        return hit;
    }

    if (*(u32*)mario & 0x100000) {
        sideOffset = float_2_804208f0;
    } else {
        sideOffset = float_8_8042091c;
    }

    check.start = pos;
    check.start.y += float_60_80420910;
    check.end = vec3_802c3cb0;
    check.radius = float_100_80420900;

    if (*(u16*)((s32)marioGetPtr() + 0x2E) == 0x1C) {
        hit = (void*)hitCheckVecFilter(&check, chkfilterVecVivian);
    } else if (*(u32*)marioGetPtr() & 0x01000000) {
        hit = (void*)hitCheckVecFilter(&check, chkfilterVecRoll);
    } else {
        hit = (void*)hitCheckVecFilter(&check, chkfilterVec);
    }
    if (hit != 0 && (hitGetAttr(hit) & 0x800)) {
        *(void**)((s32)mario + 0x1E4) = hit;
        *outSpeed = float_0p8_80420914;
    }

    pos = *(Vec*)((s32)mario + 0x8C);
    hit = (void*)searchFrontWall(float_20_80420918, float_0_804208ac, float_20_80420918, &pos, (s32)temp);
    if (hit != 0 && (hitGetAttr(hit) & 0x800)) {
        *(void**)((s32)mario + 0x1E4) = hit;
        *outSpeed = float_0p8_80420914;
    }

    pos = *(Vec*)((s32)mario + 0x8C);
    movePos(sideOffset, float_270_80420920, &pos, &pos.z);
    hit = (void*)searchFrontWall(float_20_80420918, float_0_804208ac, float_20_80420918, &pos, (s32)temp);
    if (hit != 0 && (hitGetAttr(hit) & 0x800)) {
        *(void**)((s32)mario + 0x1E4) = hit;
        *outSpeed = float_0p8_80420914;
    }

    pos = *(Vec*)((s32)mario + 0x8C);
    movePos(sideOffset, float_90_80420924, &pos, &pos.z);
    hit = (void*)searchFrontWall(float_20_80420918, float_0_804208ac, float_20_80420918, &pos, (s32)temp);
    if (hit != 0 && (hitGetAttr(hit) & 0x800)) {
        *(void**)((s32)mario + 0x1E4) = hit;
        *outSpeed = float_0p8_80420914;
    }

    return hit;
}

int dou10_yoko_yari2(float* outSpeed) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;
    typedef struct HitCheckArg {
        u8 pad[0xC];
        Vec start;
        Vec end;
        Vec normal;
        Vec hitPos;
        f32 radius;
    } HitCheckArg;
    extern void* marioGetPtr(void);
    extern f32 toMovedir(f64);
    extern s32 strcmp(char*, char*);
    extern s32 hitCheckVecFilter(HitCheckArg*, void*);
    extern s32 chkfilterVecVivian(s32, int);
    extern s32 chkfilterVecRoll(s32, int);
    extern s32 chkfilterVec(s32, int);
    extern u32 hitGetAttr(void*);
    extern void movePos(f64, f64, void*, void*);
    extern void* gp;
    extern char str_dou_10_802c3d88[];
    extern Vec vec3_802c3cbc;
    extern f32 float_60_80420910;
    extern f32 float_100_80420900;
    extern f32 float_0p8_80420914;
    extern f32 float_20_80420918;
    extern f32 float_8_8042091c;
    extern f32 float_270_80420920;
    extern f32 float_90_80420924;
    extern f32 float_0_804208ac;

    void* mario;
    void* hit;
    HitCheckArg check;
    Vec pos;
    u8 temp[4];

    mario = marioGetPtr();
    toMovedir(*(f32*)((s32)mario + 0x1AC));
    pos = *(Vec*)((s32)mario + 0x8C);
    if (strcmp((char*)((s32)gp + 0x12C), str_dou_10_802c3d88) != 0) {
        return 0;
    }

    check.start = pos;
    check.start.y += float_60_80420910;
    check.end = vec3_802c3cbc;
    check.radius = float_100_80420900;

    if (*(u16*)((s32)marioGetPtr() + 0x2E) == 0x1C) {
        hit = (void*)hitCheckVecFilter(&check, chkfilterVecVivian);
    } else if (*(u32*)marioGetPtr() & 0x01000000) {
        hit = (void*)hitCheckVecFilter(&check, chkfilterVecRoll);
    } else {
        hit = (void*)hitCheckVecFilter(&check, chkfilterVec);
    }
    if (hit != 0 && (hitGetAttr(hit) & 0x800)) {
        *(void**)((s32)mario + 0x1E4) = hit;
        *outSpeed = float_0p8_80420914;
    }

    pos = *(Vec*)((s32)mario + 0x8C);
    hit = (void*)searchFrontWall(float_20_80420918, float_0_804208ac, float_20_80420918, &pos, (s32)temp);
    if (hit != 0 && (hitGetAttr(hit) & 0x800)) {
        *(void**)((s32)mario + 0x1E4) = hit;
        *outSpeed = float_0p8_80420914;
    }

    pos = *(Vec*)((s32)mario + 0x8C);
    movePos(float_8_8042091c, float_270_80420920, &pos, &pos.z);
    hit = (void*)searchFrontWall(float_20_80420918, float_0_804208ac, float_20_80420918, &pos, (s32)temp);
    if (hit != 0 && (hitGetAttr(hit) & 0x800)) {
        *(void**)((s32)mario + 0x1E4) = hit;
        *outSpeed = float_0p8_80420914;
    }

    pos = *(Vec*)((s32)mario + 0x8C);
    movePos(float_8_8042091c, float_90_80420924, &pos, &pos.z);
    hit = (void*)searchFrontWall(float_20_80420918, float_0_804208ac, float_20_80420918, &pos, (s32)temp);
    if (hit != 0 && (hitGetAttr(hit) & 0x800)) {
        *(void**)((s32)mario + 0x1E4) = hit;
        *outSpeed = float_0p8_80420914;
    }

    return (s32)hit;
}

s32 marioChkFront(f64 angle, f64 maxDist, f32* outDist, f32* pos) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;
    typedef struct HitCheckArg {
        u8 pad[0xC];
        Vec start;
        Vec end;
        Vec normal;
        Vec hitPos;
        f32 radius;
    } HitCheckArg;
    extern void sincosf(f32, f32*, f32*);
    extern void* marioGetPtr(void);
    extern s32 hitCheckVecFilter(HitCheckArg*, void*);
    extern s32 chkfilterVecVivian(s32, int);
    extern s32 chkfilterVecRoll(s32, int);
    extern s32 chkfilterVec(s32, int);
    extern f64 distABf(f64, f64, f64, f64);
    extern Vec vec3_802c3c80;
    extern f32 float_0_804208ac;
    extern f32 float_1000_804208a8;
    extern f32 float_0p5_804208cc;
    extern f32 float_neg0p5_804208d0;
    extern f32 float_neg1_804208e4;

    HitCheckArg hit;
    Vec hitPos;
    f32 s;
    f32 c;
    f32 roundedDist;
    f32 roundBias;
    s32 result;

    sincosf((f32)angle, &c, &s);
    hit.start = *(Vec*)pos;
    hit.end = vec3_802c3c80;
    hit.end.x = c;
    hit.end.z = s;
    hit.radius = (f32)maxDist;

    if (*(u16*)((s32)marioGetPtr() + 0x2E) == 0x1C) {
        result = hitCheckVecFilter(&hit, chkfilterVecVivian);
    } else if (*(u32*)marioGetPtr() & 0x01000000) {
        result = hitCheckVecFilter(&hit, chkfilterVecRoll);
    } else {
        result = hitCheckVecFilter(&hit, chkfilterVec);
    }

    if (result != 0) {
        roundedDist = hit.radius;
        roundBias = float_neg0p5_804208d0;
        if (roundedDist >= float_0_804208ac) {
            roundBias = float_0p5_804208cc;
        }
        roundedDist = (f32)(s32)(roundedDist * float_1000_804208a8 + roundBias) / float_1000_804208a8;

        roundBias = float_neg0p5_804208d0;
        if (hit.normal.x >= float_0_804208ac) {
            roundBias = float_0p5_804208cc;
        }
        hit.normal.x = (f32)(s32)(hit.normal.x * float_1000_804208a8 + roundBias) / float_1000_804208a8;

        roundBias = float_neg0p5_804208d0;
        if (hit.normal.y >= float_0_804208ac) {
            roundBias = float_0p5_804208cc;
        }
        hit.normal.y = (f32)(s32)(hit.normal.y * float_1000_804208a8 + roundBias) / float_1000_804208a8;

        roundBias = float_neg0p5_804208d0;
        if (hit.normal.z >= float_0_804208ac) {
            roundBias = float_0p5_804208cc;
        }
        hit.normal.z = (f32)(s32)(hit.normal.z * float_1000_804208a8 + roundBias) / float_1000_804208a8;
        hitPos = hit.hitPos;
    }

    if (result == 0 || maxDist < roundedDist) {
        *outDist = float_neg1_804208e4;
        return 0;
    }

    *outDist = (f32)distABf(pos[0], pos[2], hit.normal.x, hit.normal.z);
    return result;
}

s32 marioHitCheck(f64 x0, f64 y0, f64 z0, f64 x1, f64 y1, f64 z1, f32* outNx, f32* outNy, f32* outNz,
                  f32* radius, f32* outHx, f32* outHy, f32* outHz) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;
    typedef struct HitCheckArg {
        u8 pad[0xC];
        Vec start;
        Vec end;
        Vec normal;
        Vec hitPos;
        f32 radius;
    } HitCheckArg;
    extern void* marioGetPtr(void);
    extern s32 hitCheckVecFilter(HitCheckArg*, void*);
    extern s32 chkfilterVecVivian(s32, int);
    extern s32 chkfilterVecRoll(s32, int);
    extern s32 chkfilterVec(s32, int);
    extern Vec vec3_802c3b78;
    extern Vec vec3_802c3b84;
    extern f32 float_0_804208ac;
    extern f32 float_1000_804208a8;
    extern f32 float_0p5_804208cc;
    extern f32 float_neg0p5_804208d0;

    HitCheckArg hit;
    Vec roundedHit;
    f32 r;
    f32 roundBias;
    s32 result;

    hit.start = vec3_802c3b78;
    hit.end = vec3_802c3b84;
    hit.start.x = (f32)x0;
    hit.start.y = (f32)y0;
    hit.start.z = (f32)z0;
    hit.end.x = (f32)x1;
    hit.end.y = (f32)y1;
    hit.end.z = (f32)z1;
    r = *radius;
    hit.radius = r;

    if (*(u16*)((s32)marioGetPtr() + 0x2E) == 0x1C) {
        result = hitCheckVecFilter(&hit, chkfilterVecVivian);
    } else if (*(u32*)marioGetPtr() & 0x01000000) {
        result = hitCheckVecFilter(&hit, chkfilterVecRoll);
    } else {
        result = hitCheckVecFilter(&hit, chkfilterVec);
    }

    if (result != 0) {
        roundBias = float_neg0p5_804208d0;
        if (r >= float_0_804208ac) {
            roundBias = float_0p5_804208cc;
        }
        r = (f32)(s32)(r * float_1000_804208a8 + roundBias) / float_1000_804208a8;

        roundBias = float_neg0p5_804208d0;
        if (hit.normal.x >= float_0_804208ac) {
            roundBias = float_0p5_804208cc;
        }
        hit.normal.x = (f32)(s32)(hit.normal.x * float_1000_804208a8 + roundBias) / float_1000_804208a8;

        roundBias = float_neg0p5_804208d0;
        if (hit.normal.y >= float_0_804208ac) {
            roundBias = float_0p5_804208cc;
        }
        hit.normal.y = (f32)(s32)(hit.normal.y * float_1000_804208a8 + roundBias) / float_1000_804208a8;

        roundBias = float_neg0p5_804208d0;
        if (hit.normal.z >= float_0_804208ac) {
            roundBias = float_0p5_804208cc;
        }
        hit.normal.z = (f32)(s32)(hit.normal.z * float_1000_804208a8 + roundBias) / float_1000_804208a8;
        roundedHit = hit.hitPos;
    }

    *radius = r;
    *outNx = hit.normal.x;
    *outNy = hit.normal.y;
    *outNz = hit.normal.z;
    *outHx = roundedHit.x;
    *outHy = roundedHit.y;
    *outHz = roundedHit.z;
    return result;
}

void marioCheckWallShip(f64 speed, f64 angle) {
    typedef struct VecLocal { f32 x, y, z; } VecLocal;
    extern void* marioGetPtr(void);
    extern void marioSearchFrontWall_staying(f64);
    extern void* searchFrontWall(f64, f64, f64, void*, f32*);
    extern u32 hitGetAttr(void*);
    extern void movePos(f64, f64, f32*, f32*);
    extern s32 marioChkWallAround(f64, f64, f64, f64, void*, s32);
    extern void PSVECSubtract(void*, void*, void*);
    extern f32 PSVECMag(void*);
    extern void PSVECScale(void*, void*, f32);
    extern void PSVECAdd(void*, void*, void*);
    extern f32 float_0_804208ac;
    extern f32 float_1000_804208a8;
    u8* player = marioGetPtr();
    VecLocal oldPos;
    VecLocal delta;
    f32 wallDistance;
    void* hit;
    u32 attr;
    s32 passable;

    *(f32*)(player + 0x214) = float_1000_804208a8;
    if (speed == float_0_804208ac || *(s16*)(player + 0x50) != 0) {
        *(f32*)(player + 0x1CC) = float_0_804208ac;
    } else if (speed > *(f32*)(player + 0x1CC)) {
        *(f32*)(player + 0x1CC) = (f32)speed;
        *(f32*)(player + 0x1D0) = (f32)angle;
        *(s32*)(player + 0x1D4) = 14;
    } else {
        *(s32*)(player + 0x1D4) -= 1;
        if (*(s32*)(player + 0x1D4) < 1) {
            *(s32*)(player + 0x1D4) = 0;
            *(f32*)(player + 0x1CC) = (f32)speed;
            *(f32*)(player + 0x1D0) = (f32)angle;
        }
    }
    if (speed != float_0_804208ac && *(s16*)(player + 0x50) == 0) {
        *(f32*)(player + 0x1A0) = *(f32*)(player + 0x1D0);
    }
    if (speed == float_0_804208ac) {
        marioSearchFrontWall_staying(angle);
        return;
    }

    oldPos = *(VecLocal*)(player + 0x8C);
    hit = searchFrontWall(speed, angle, *(f32*)(player + 0x1C0), &oldPos, &wallDistance);
    if (hit != 0) {
        *(void**)(player + 0x1E4) = hit;
        attr = hitGetAttr(hit);
        passable = 0;
        if ((attr & 0x80000) == 0 || (*(u32*)player & 0x01000000) != 0) {
            if ((attr & 0x80) == 0) {
                passable = (attr >> 23) & 1;
            } else {
                passable = (*(u16*)(player + 0x2E) == 0x19);
            }
        }
        if (passable != 0) {
            *(void**)(player + 0x1F0) = hit;
            *(void**)(player + 0x1F4) = hit;
            movePos(speed, angle, (f32*)(player + 0x8C), (f32*)(player + 0x94));
            oldPos = *(VecLocal*)(player + 0x8C);
            return;
        }
        if (speed == float_0_804208ac) {
            *(s8*)(player + 0x3F) = 0;
        } else {
            *(f32*)(player + 0x214) = wallDistance;
            *(void**)(player + 0x1F4) = hit;
            *(s8*)(player + 0x3F) += 1;
            if (*(s8*)(player + 0x3F) > 19) {
                *(s8*)(player + 0x3F) = 20;
                attr = hitGetAttr(hit);
                if ((attr & 0x80) == 0 || *(u16*)(player + 0x2E) == 0x19) {
                    *(void**)(player + 0x1F0) = hit;
                }
            }
        }
        *(VecLocal*)(player + 0x8C) = oldPos;
    }

    *(VecLocal*)(player + 0x8C) = oldPos;
    marioChkWallAround(speed, angle, *(f32*)(player + 0x1C0), angle,
                       player + 0x8C, 0);
    PSVECSubtract(player + 0x8C, &oldPos, &delta);
    if (PSVECMag(&delta) > *(f32*)(player + 0x188)) {
        f32 magnitude = PSVECMag(&delta);
        PSVECScale(&delta, &delta, *(f32*)(player + 0x188) / magnitude);
        PSVECAdd(&oldPos, &delta, player + 0x8C);
    }
}

void marioSearchFrontWall_staying(f64 angle) {
    typedef struct Vec { f32 x, y, z; } Vec;
    extern void* marioGetPtr(void);
    extern void* searchFrontWall(f64, f64, f64, Vec*, f32*);
    extern f64 toMovedir(f64);
    extern void movePos(f64, f64, f32*, f32*);
    extern s32 strcmp(const char*, const char*);
    extern s32 vivianGetStatus(void);
    extern s32 marioChkWallAround(f64, f64, f64, f64, Vec*, s32);
    extern void* gp;
    extern char str_dou_10_802c3d88[];
    extern f32 float_0_804208ac;
    extern f32 float_0p5_804208cc;
    extern f32 float_180_804208d4;
    u8* player = marioGetPtr();
    Vec pos;
    void* hit;
    f32 distance;
    f32 radius;
    f32 direction;

    *(s8*)(player + 0x3F) = 0;
    radius = float_0p5_804208cc * *(f32*)(player + 0x1B8);
    if ((*(u32*)player & 0x00100000) != 0) {
        radius = float_0_804208ac;
    }

    pos = *(Vec*)(player + 0x8C);
    hit = searchFrontWall(radius,
                          *(f32*)(player + 0x1A4) - *(f32*)(player + 0x19C),
                          *(f32*)(player + 0x1C0), &pos, &distance);
    *(void**)(player + 0x1E4) = *(u16*)(player + 0x2E) == 0x13 ? 0 : hit;

    if (hit == 0) {
        direction = (f32)toMovedir(*(f32*)(player + 0x1AC));
        pos = *(Vec*)(player + 0x8C);
        hit = searchFrontWall(radius, direction, *(f32*)(player + 0x1C0), &pos, &distance);
        *(void**)(player + 0x1E4) = *(u16*)(player + 0x2E) == 0x13 ? 0 : hit;
    } else if (strcmp((char*)gp + 0x12C, str_dou_10_802c3d88) == 0 &&
               vivianGetStatus() != 2 && vivianGetStatus() != 1 &&
               vivianGetStatus() != 3) {
        direction = (f32)toMovedir(*(f32*)(player + 0x1AC));
        pos = *(Vec*)(player + 0x8C);
        movePos(float_0p5_804208cc * *(f32*)(player + 0x1B8), direction,
                &pos.x, &pos.z);
        hit = searchFrontWall(*(f32*)(player + 0x1B8),
                              direction + float_180_804208d4,
                              *(f32*)(player + 0x1C0), &pos, &radius);
        if (hit != 0 && radius <= *(f32*)(player + 0x1B8)) {
            *(void**)(player + 0x1E4) = hit;
            distance = float_0p5_804208cc * radius;
        }
    }

    if (*(void**)(player + 0x1E4) != 0) {
        *(f32*)(player + 0x214) = distance;
    }
    if ((*(u16*)(player + 0x24C) & 0x100) != 0) {
        *(void**)(player + 0x1E0) = 0;
        pos = *(Vec*)(player + 0x8C);
        if (searchFrontWall(radius, *(f32*)(player + 0x1A4),
                            *(f32*)(player + 0x1C0), &pos, &distance) != 0) {
            *(void**)(player + 0x1E0) = *(void**)(player + 0x1E4);
        }
    }

    pos = *(Vec*)(player + 0x8C);
    marioChkWallAround(float_0_804208ac, angle, *(f32*)(player + 0x1C0),
                       *(f32*)(player + 0x1BC), &pos, 0);
    *(f32*)(player + 0x8C) = pos.x;
    *(f32*)(player + 0x94) = pos.z;
}

void marioCheckWall(f64 speed, f64 angle) {
    typedef struct Vec { f32 x, y, z; } Vec;
    extern void* marioGetPtr(void);
    extern void marioSearchFrontWall_staying(f64);
    extern void dou10_yoko_yari(f32*);
    extern void* searchFrontWall(f64, f64, f64, Vec*, f32*);
    extern u32 hitGetAttr(void*);
    extern char* hitGetName(void*);
    extern s32 strncmp(const char*, const char*, u32);
    extern void movePos(f64, f64, f32*, f32*);
    extern s32 marioChkWallAround(f64, f64, f64, f64, Vec*, s32);
    extern char str_dokan_8042093c[];
    extern f32 float_0_804208ac;
    extern f32 float_1000_804208a8;
    u8* player = marioGetPtr();
    Vec pos;
    void* hit;
    u32 attr;
    f32 distance;
    s32 limit;

    *(f32*)(player + 0x214) = float_1000_804208a8;
    if (speed == float_0_804208ac || *(s16*)(player + 0x50) != 0) {
        *(f32*)(player + 0x1CC) = float_0_804208ac;
    } else {
        if (*(f32*)(player + 0x1CC) < (f32)speed) {
            *(f32*)(player + 0x1CC) = (f32)speed;
            *(f32*)(player + 0x1D0) = (f32)angle;
            *(s32*)(player + 0x1D4) = 14;
        } else if (--*(s32*)(player + 0x1D4) < 1) {
            *(s32*)(player + 0x1D4) = 0;
            *(f32*)(player + 0x1CC) = (f32)speed;
            *(f32*)(player + 0x1D0) = (f32)angle;
        }
        *(f32*)(player + 0x1A0) = *(f32*)(player + 0x1D0);
    }

    if (speed == float_0_804208ac) {
        marioSearchFrontWall_staying(angle);
        dou10_yoko_yari((f32*)(player + 0x214));
        return;
    }

    pos = *(Vec*)(player + 0x8C);
    *(void**)(player + 0x210) = 0;
    hit = searchFrontWall(speed, angle, *(f32*)(player + 0x1C0), &pos, &distance);
    if (hit == 0) {
        *(Vec*)(player + 0x8C) = pos;
    } else {
        *(void**)(player + 0x1E4) = *(void**)(player + 0x210) != 0 ?
                                    *(void**)(player + 0x210) : hit;
        attr = hitGetAttr(hit);
        if (((attr & 0x80000) != 0 && (*(u32*)player & 0x01000000) == 0) ||
            ((attr & 0x80) != 0 && *(u16*)(player + 0x2E) != 0x19) ||
            ((attr & 0x80) == 0 && ((attr >> 23) & 1) == 0)) {
            if (speed != float_0_804208ac) {
                *(void**)(player + 0x1F0) = hit;
                *(void**)(player + 0x1F4) = hit;
            }
            movePos(speed, angle, (f32*)(player + 0x8C), (f32*)(player + 0x94));
        } else {
            *(f32*)(player + 0x214) = distance;
            *(void**)(player + 0x1F4) = hit;
            limit = strncmp(hitGetName(hit), str_dokan_8042093c, 5) == 0 ? 10 : 20;
            *(s8*)(player + 0x3F) += 1;
            if (*(s8*)(player + 0x3F) >= limit) {
                *(s8*)(player + 0x3F) = (s8)limit;
                attr = hitGetAttr(hit);
                if ((attr & 0x80) == 0 || *(u16*)(player + 0x2E) == 0x19) {
                    *(void**)(player + 0x1F0) = hit;
                }
            }
            *(Vec*)(player + 0x8C) = pos;
        }
    }
    marioChkWallAround(speed, angle, *(f32*)(player + 0x1C0),
                       *(f32*)(player + 0x1BC), (Vec*)(player + 0x8C), 0);
    dou10_yoko_yari((f32*)(player + 0x214));
}

int marioHitCheckVec(void* param_1, void* param_2, float* param_3, void* param_4, float* param_5) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;
    typedef struct HitCheckArg {
        u8 pad[0xC];
        Vec start;
        Vec end;
        Vec normal;
        Vec hitPos;
        f32 radius;
    } HitCheckArg;
    extern void* marioGetPtr(void);
    extern s32 hitCheckVecFilter(HitCheckArg*, void*);
    extern s32 chkfilterVecVivian(s32, int);
    extern s32 chkfilterVecRoll(s32, int);
    extern s32 chkfilterVec(s32, int);
    extern f32 float_0p5_804208cc;
    extern f32 float_neg0p5_804208d0;
    extern f32 float_1000_804208a8;

    HitCheckArg hit;
    s32 result;

    hit.start = *(Vec*)param_1;
    hit.end = *(Vec*)param_2;
    hit.radius = *param_5;

    if (*(u16*)((s32)marioGetPtr() + 0x2E) == 0x1C) {
        result = hitCheckVecFilter(&hit, chkfilterVecVivian);
    } else if ((*(u32*)marioGetPtr() & 0x01000000) != 0) {
        result = hitCheckVecFilter(&hit, chkfilterVecRoll);
    } else {
        result = hitCheckVecFilter(&hit, chkfilterVec);
    }

    if (result != 0) {
        f32 radius = *param_5;
        *param_5 = (f32)(s32)(radius * float_1000_804208a8 + (radius >= 0.0f ? float_0p5_804208cc : float_neg0p5_804208d0)) / float_1000_804208a8;
        if (param_3 != 0) {
            f32 x;
            f32 y;
            f32 z;

            *(Vec*)param_3 = hit.normal;
            x = param_3[0];
            param_3[0] = (f32)(s32)(x * float_1000_804208a8 + (x >= 0.0f ? float_0p5_804208cc : float_neg0p5_804208d0)) / float_1000_804208a8;
            y = param_3[1];
            param_3[1] = (f32)(s32)(y * float_1000_804208a8 + (y >= 0.0f ? float_0p5_804208cc : float_neg0p5_804208d0)) / float_1000_804208a8;
            z = param_3[2];
            param_3[2] = (f32)(s32)(z * float_1000_804208a8 + (z >= 0.0f ? float_0p5_804208cc : float_neg0p5_804208d0)) / float_1000_804208a8;
        }
        if (param_4 != 0) {
            *(Vec*)param_4 = hit.hitPos;
        }
    }
    return result;
}

u8 searchUnder2(double param_1, double param_2, double param_3, float* param_4) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;
    typedef struct HitCheckArg {
        u8 pad[0xC];
        Vec start;
        Vec end;
        Vec normal;
        Vec hitPos;
        f32 radius;
    } HitCheckArg;
    extern void* marioGetPtr(void);
    extern s32 hitCheckVecFilter(HitCheckArg*, void*);
    extern s32 chkfilterVecVivian(s32, int);
    extern s32 chkfilterVecRoll(s32, int);
    extern s32 chkfilterVec(s32, int);
    extern Vec vec3_802c3ba8;
    extern Vec vec3_802c3bb4;
    extern f32 float_37_8042097c;
    extern f32 float_1037_80420998;
    extern f32 float_0p5_804208cc;
    extern f32 float_neg0p5_804208d0;
    extern f32 float_1000_804208a8;
    extern f32 float_neg1_804208e4;

    Vec pos = vec3_802c3ba8;
    HitCheckArg hit;
    s32 result;

    pos.x = (f32)param_1;
    pos.y = (f32)param_2;
    pos.z = (f32)param_3;
    pos.y += float_37_8042097c;
    hit.start = pos;
    hit.end = vec3_802c3bb4;
    hit.radius = float_1037_80420998;

    if (*(u16*)((s32)marioGetPtr() + 0x2E) == 0x1C) {
        result = hitCheckVecFilter(&hit, chkfilterVecVivian);
    } else if ((*(u32*)marioGetPtr() & 0x01000000) != 0) {
        result = hitCheckVecFilter(&hit, chkfilterVecRoll);
    } else {
        result = hitCheckVecFilter(&hit, chkfilterVec);
    }

    if (result != 0) {
        f32 x = hit.normal.x;
        f32 y = hit.normal.y;
        f32 z = hit.normal.z;

        hit.normal.x = (f32)(s32)(x * float_1000_804208a8 + (x >= 0.0f ? float_0p5_804208cc : float_neg0p5_804208d0)) / float_1000_804208a8;
        hit.normal.y = (f32)(s32)(y * float_1000_804208a8 + (y >= 0.0f ? float_0p5_804208cc : float_neg0p5_804208d0)) / float_1000_804208a8;
        hit.normal.z = (f32)(s32)(z * float_1000_804208a8 + (z >= 0.0f ? float_0p5_804208cc : float_neg0p5_804208d0)) / float_1000_804208a8;
    }

    if (result != 0) {
        *param_4 = hit.normal.y;
    } else {
        *param_4 = float_neg1_804208e4;
    }
    return result;
}


u8 marioSearchUnder(void) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;
    typedef struct HitCheckArg {
        u8 pad[0xC];
        Vec start;
        Vec end;
        Vec normal;
        Vec hitPos;
        f32 radius;
    } HitCheckArg;
    extern void* marioGetPtr(void);
    extern s32 hitCheckVecFilter(HitCheckArg*, void*);
    extern s32 chkfilterVecVivian(s32, int);
    extern s32 chkfilterVecRoll(s32, int);
    extern s32 chkfilterVec(s32, int);
    extern Vec vec3_802c3b9c;
    extern f32 float_0p5_804208cc;
    extern f32 float_1000_804208a8;
    extern f32 float_neg0p5_804208d0;
    extern f32 float_neg3000_80420980;

    void* mario = marioGetPtr();
    f32 height = *(f32*)((s32)mario + 0x1BC);
    HitCheckArg hit;
    Vec pos;
    s32 result;

    if ((*(u32*)mario & 0x01000000) != 0) {
        height *= float_0p5_804208cc;
    }

    pos.x = *(f32*)((s32)mario + 0x8C);
    pos.y = *(f32*)((s32)mario + 0x90) + height;
    pos.z = *(f32*)((s32)mario + 0x94);
    hit.start = pos;
    hit.end = vec3_802c3b9c;
    hit.radius = float_1000_804208a8;

    if (*(u16*)((s32)marioGetPtr() + 0x2E) == 0x1C) {
        result = hitCheckVecFilter(&hit, chkfilterVecVivian);
    } else if ((*(u32*)marioGetPtr() & 0x01000000) != 0) {
        result = hitCheckVecFilter(&hit, chkfilterVecRoll);
    } else {
        result = hitCheckVecFilter(&hit, chkfilterVec);
    }

    if (result != 0) {
        f32 x = hit.normal.x;
        f32 y = hit.normal.y;
        f32 z = hit.normal.z;

        hit.normal.x = (f32)(s32)(x * float_1000_804208a8 + (x >= 0.0f ? float_0p5_804208cc : float_neg0p5_804208d0)) / float_1000_804208a8;
        hit.normal.y = (f32)(s32)(y * float_1000_804208a8 + (y >= 0.0f ? float_0p5_804208cc : float_neg0p5_804208d0)) / float_1000_804208a8;
        hit.normal.z = (f32)(s32)(z * float_1000_804208a8 + (z >= 0.0f ? float_0p5_804208cc : float_neg0p5_804208d0)) / float_1000_804208a8;
    }

    *(f32*)((s32)mario + 0x1C4) = result != 0 ? hit.normal.y : float_neg3000_80420980;
    return result;
}

void* N_dou10_yoko_yari3(void) {
    typedef struct VecLocal { f32 x, y, z; } VecLocal;
    typedef struct HitCheckArgLocal {
        u8 pad[0xC];
        VecLocal start;
        VecLocal end;
        VecLocal normal;
        VecLocal hitPos;
        f32 radius;
    } HitCheckArgLocal;
    extern void* marioGetPtr(void);
    extern void* gp;
    extern s32 strcmp(const char*, const char*);
    extern char str_dou_10_802c3d88[];
    extern void movePos(f64, f64, f32*, f32*);
    extern void sincosf(f32, f32*, f32*);
    extern void* hitCheckVecFilter(void*, void*);
    extern f32 float_10_80420904;
    extern f32 float_neg10_8042090c;
    extern f32 float_5_804208c0;
    extern f32 float_0_804208ac;
    extern f32 float_40_80420908;
    u8* player = marioGetPtr();
    HitCheckArgLocal check;
    f32 x;
    f32 z;
    f32 sinv;
    f32 cosv;
    void* hit;

    if (strcmp((char*)gp + 0x12C, str_dou_10_802c3d88) != 0) {
        return 0;
    }
    x = *(f32*)(player + 0x8C);
    z = *(f32*)(player + 0x94);
    movePos(float_10_80420904, *(f32*)(player + 0x1A4), &x, &z);
    check.start.x = x;
    check.start.y = *(f32*)(player + 0x90) + float_5_804208c0;
    check.start.z = z;
    sincosf(float_0_804208ac, &sinv, &cosv);
    check.end.x = sinv;
    check.end.y = float_0_804208ac;
    check.end.z = cosv;
    check.radius = float_40_80420908;
    hit = hitCheckVecFilter(&check, 0);
    if (hit == 0) {
        x = *(f32*)(player + 0x8C);
        z = *(f32*)(player + 0x94);
        movePos(float_neg10_8042090c, *(f32*)(player + 0x1A4), &x, &z);
        check.start.x = x;
        check.start.y = *(f32*)(player + 0x90) + float_5_804208c0;
        check.start.z = z;
        sincosf(float_0_804208ac, &sinv, &cosv);
        check.end.x = sinv;
        check.end.y = float_0_804208ac;
        check.end.z = cosv;
        check.radius = float_40_80420908;
        hit = hitCheckVecFilter(&check, 0);
    }
    if (hit != 0) {
        *(void**)(player + 0x1E4) = hit;
    }
    return hit;
}

void* marioSearchHead_jabara(f64 height, f32* position, f32* distance, s32* side) {
    typedef struct Vec { f32 x, y, z; } Vec;
    extern void* marioGetPtr(void);
    extern f64 sin(f64);
    extern f64 cos(f64);
    extern s32 marioHitCheck2(void*, void*, f32*, void*, f32*);
    extern u32 hitGetAttr(void*);
    extern Vec vec3_802c3c68;
    extern f32 float_0_804208ac;
    extern f32 float_0p5_804208cc;
    extern f32 float_neg1_804208e4;
    extern f32 float_3p1416_804208e8;
    extern f32 float_180_804208d4;
    extern f32 float_neg23p7_80420944;
    extern f32 float_5p8_80420948;
    u8* player = marioGetPtr();
    Vec start;
    Vec end = vec3_802c3c68;
    Vec normal;
    Vec hitPos;
    void* hit;
    f32 radius;
    f32 offset = float_neg23p7_80420944;
    f32 baseDistance = *distance;
    s32 i;

    *side = 0;
    *distance = float_neg1_804208e4;
    for (i = 0; i < 8; i++, offset += float_5p8_80420948) {
        radius = baseDistance + (f32)height;
        start.x = position[0] + offset * (f32)sin(float_3p1416_804208e8 *
                                                   *(f32*)(player + 0x1AC) /
                                                   float_180_804208d4);
        start.y = position[1] + float_0p5_804208cc;
        start.z = position[2] - offset * (f32)cos(float_3p1416_804208e8 *
                                                   *(f32*)(player + 0x1AC) /
                                                   float_180_804208d4);
        hit = (void*)marioHitCheck2(&start, &end, (f32*)&normal, &hitPos, &radius);
        if (hit != 0) {
            u32 attr = hitGetAttr(hit);
            if ((attr & 0x40000) != 0 || i == 3) {
                if (hitPos.y <= position[1] + baseDistance) {
                    position[1] = hitPos.y;
                    *distance = float_0_804208ac;
                    if ((attr & 0x40000) != 0) {
                        *side = start.z - position[2] >= float_0_804208ac ? 1 : -1;
                    } else {
                        *side = 0;
                    }
                    return hit;
                }
                *distance = (hitPos.y - position[1]) - baseDistance;
            }
        }
    }
    return 0;
}

double marioChkOverheadJabara(void* param_1, float* param_2) {
    typedef struct Vec { f32 x, y, z; } Vec;
    extern void* marioGetPtr(void);
    extern s32 jabaraNoHitChk(void);
    extern void* marioSearchHead_jabara(f64, f32*, f32*, s32*);
    extern void* marioSearchHead(f64, f32*, f32*);
    extern void marioChgMot(s32);
    extern f32 float_0_804208ac;
    extern f32 float_0p1_804208ec;
    extern f32 float_neg1_804208e4;
    extern f32 float_5_804208c0;
    u8* player = marioGetPtr();
    Vec pos = *(Vec*)(player + 0x8C);
    f32 height = *(f32*)(player + 0x1BC);
    s32* side = param_1;

    *side = 0;
    *param_2 = float_neg1_804208e4;
    if (*(u16*)(player + 0x2E) != 0x15) {
        if (jabaraNoHitChk() == 0) {
            height += float_5_804208c0;
            *(void**)(player + 0x1EC) = marioSearchHead_jabara(
                float_0_804208ac, &pos.x, &height, side);
            if (*(void**)(player + 0x1EC) == 0) {
                height = *(f32*)(player + 0x1BC) + float_5_804208c0;
                *(void**)(player + 0x1EC) = marioSearchHead(
                    float_0_804208ac, &pos.x, &height);
            }
        }
        *(f32*)(player + 0x8C) = pos.x;
        if (*(void**)(player + 0x1EC) == 0) {
            if (height <= float_0_804208ac) {
                *(u32*)player &= ~0x200;
            } else {
                *param_2 = height;
                *(u32*)player |= 0x200;
            }
        } else {
            *(u32*)player |= 0x200;
            pos.y = (pos.y - *(f32*)(player + 0x1BC)) - float_0p1_804208ec;
            if (*(u16*)(player + 0x2E) == 6) {
                marioChgMot(0);
            }
        }
    }
    return (double)pos.y;
}

void marioChkGnd2(void) {
    extern void* marioGetPtr(void);
    extern void* marioSearchGround(f32 param_1, void* yOut, f32 velocityY, void* param_4, void* param_5);
    extern void* kpaSearchGround(f32 param_1, void* yOut, void* param_3, void* param_4);
    extern s32 kpaFireAttackCheck(void);
    extern void marioChgMot(s32 motion);
    extern void quake_kpaLandOn(void);
    extern s32 marioBgmodeChk(void);
    extern void camFollowYOn(void);
    extern void kpaCamFollow(void);
    extern f32 float_0p1_804208ec;
    extern f32 float_0_804208ac;
    extern f32 float_neg10_8042090c;
    void* mario;
    void* hit;
    u8 stack_18[4];
    f32 groundY;
    u8 stack_10[12];
    f32 diff;
    s32 x;
    s32 y;

    mario = marioGetPtr();
    if ((*(u32*)mario & 0x10000) == 0) {
        if (*(s8*)((s32)mario + 0x3C) == 2) {
            hit = kpaSearchGround(float_0p1_804208ec, &groundY, stack_10, stack_18);
        } else {
            hit = marioSearchGround(float_0p1_804208ec, &groundY, *(f32*)((s32)mario + 0x7C), stack_10, stack_18);
        }
        if (hit != 0) {
            diff = *(f32*)((s32)mario + 0x90) - groundY;
            if (diff < float_0_804208ac && diff >= float_neg10_8042090c) {
                *(void**)((s32)mario + 0x1E8) = hit;
                *(f32*)((s32)mario + 0x90) = groundY;
                *(u32*)mario &= ~0xC0;
                *(u16*)((s32)mario + 0x50) = 0;
                *(u16*)((s32)mario + 0x52) = 0;
                if (*(s8*)((s32)mario + 0x3C) == 2) {
                    if (*(f32*)((s32)mario + 0x194) == float_0_804208ac && kpaFireAttackCheck() != 0) {
                        marioChgMot(0);
                    } else {
                        x = *(s8*)((s32)mario + 0x252);
                        y = *(s8*)((s32)mario + 0x253);
                        if (x * x + y * y <= 0xBD1) {
                            marioChgMot(1);
                        } else {
                            marioChgMot(2);
                        }
                    }
                    quake_kpaLandOn();
                } else {
                    marioChgMot(0);
                }
                if (*(s8*)((s32)mario + 0x3C) == 2) {
                    kpaCamFollow();
                } else if (marioBgmodeChk() == 0) {
                    camFollowYOn();
                }
            }
        }
    }
}

void marioChkToge(void) {
    extern void* marioGetPtr(void);
    extern s32 vivianGetStatus(void);
    extern u32 hitGetAttr(void* hit);
    extern void set_damage_root_ypos(f64 y);
    extern void marioChgMot(s32 motion);
    extern f32 float_0p5_804208cc;
    void* mario;
    void* hit;
    u32 attr;
    s32 status;

    mario = marioGetPtr();
    status = vivianGetStatus();
    if (status != 2 && vivianGetStatus() != 1) {
        if (vivianGetStatus() == 3) {
            return;
        }
        hit = *(void**)((s32)mario + 0x1E4);
        if (hit != 0) {
            attr = hitGetAttr(hit);
            if (*(u16*)((s32)mario + 0x2E) != 0x20 &&
                (attr & 0x800) != 0 &&
                *(f32*)((s32)mario + 0x214) < float_0p5_804208cc * *(f32*)((s32)mario + 0x1B8)) {
                set_damage_root_ypos(*(f32*)((s32)mario + 0x90));
                marioChgMot(0x20);
            }
        }
        hit = *(void**)((s32)mario + 0x1F4);
        if (hit != 0) {
            attr = hitGetAttr(hit);
            if (*(u16*)((s32)mario + 0x2E) != 0x20 && (attr & 0x800) != 0) {
                set_damage_root_ypos(*(f32*)((s32)mario + 0x90));
                marioChgMot(0x20);
            }
        }
        hit = *(void**)((s32)mario + 0x1F8);
        if (hit != 0) {
            attr = hitGetAttr(hit);
            if (*(u16*)((s32)mario + 0x2E) != 0x20 && (attr & 0x800) != 0) {
                set_damage_root_ypos(*(f32*)((s32)mario + 0x90));
                marioChgMot(0x20);
            }
        }
    }
}

s32 chkfilterVecRoll(s32 param_1, int param_2) {
    typedef struct MobjEntry {
        u8 pad[0x15];
        char mAnimPoseName[16];
    } MobjEntry;
    extern MobjEntry* mobjHitObjPtrToPtr(int hit);
    extern s32 strcmp(char* a, char* b);
    extern char str_MOBJ_HiddenHatenaBlo_802c3dec[];
    extern char str_MOBJ_HiddenBadgeBloc_802c3e04[];
    extern char str_MOBJ_Hidden10CountBl_802c3e1c[];
    u32 attr;
    MobjEntry* mobj;

    attr = *(u32*)(param_2 + 4);
    if ((attr & 0x80) != 0) {
        return 1;
    }
    if ((attr & 0x800000) == 0 && (attr & 0x80004) != 0) {
        return 0;
    }
    if ((attr & 0x80000000) == 0) {
        return 1;
    }
    mobj = mobjHitObjPtrToPtr(param_2);
    if (strcmp(mobj->mAnimPoseName, str_MOBJ_HiddenHatenaBlo_802c3dec) == 0 ||
        strcmp(mobj->mAnimPoseName, str_MOBJ_HiddenBadgeBloc_802c3e04) == 0 ||
        strcmp(mobj->mAnimPoseName, str_MOBJ_Hidden10CountBl_802c3e1c) == 0) {
        mobjHitObjPtrToPtr(param_2);
        return 0;
    }
    return 1;
}

s32 chkfilterVec(s32 param_1, int param_2) {
    typedef struct MobjEntry {
        u8 pad[0x15];
        char mAnimPoseName[16];
    } MobjEntry;
    extern MobjEntry* mobjHitObjPtrToPtr(int hit);
    extern s32 strcmp(char* a, char* b);
    extern char str_MOBJ_HiddenHatenaBlo_802c3dec[];
    extern char str_MOBJ_HiddenBadgeBloc_802c3e04[];
    extern char str_MOBJ_Hidden10CountBl_802c3e1c[];
    u32 attr;
    MobjEntry* mobj;

    attr = *(u32*)(param_2 + 4);
    if ((attr & 0x80080) != 0) {
        return 1;
    }
    if ((attr & 0x800000) == 0 && (attr & 4) != 0) {
        return 0;
    }
    if ((attr & 0x80000000) == 0) {
        return 1;
    }
    mobj = mobjHitObjPtrToPtr(param_2);
    if (strcmp(mobj->mAnimPoseName, str_MOBJ_HiddenHatenaBlo_802c3dec) == 0 ||
        strcmp(mobj->mAnimPoseName, str_MOBJ_HiddenBadgeBloc_802c3e04) == 0 ||
        strcmp(mobj->mAnimPoseName, str_MOBJ_Hidden10CountBl_802c3e1c) == 0) {
        mobjHitObjPtrToPtr(param_2);
        return 0;
    }
    return 1;
}

s32 chkfilterVecVivian(s32 param_1, int param_2) {
    typedef struct MobjEntry {
        u8 pad[0x15];
        char mAnimPoseName[16];
    } MobjEntry;
    extern MobjEntry* mobjHitObjPtrToPtr(int hit);
    extern s32 strcmp(char* a, char* b);
    extern char str_MOBJ_HiddenHatenaBlo_802c3dec[];
    extern char str_MOBJ_HiddenBadgeBloc_802c3e04[];
    extern char str_MOBJ_Hidden10CountBl_802c3e1c[];
    u32 attr;
    MobjEntry* mobj;

    attr = *(u32*)(param_2 + 4);
    if ((attr & 0x80804) != 0 && (attr & 0x800000) == 0) {
        return 0;
    }
    if ((attr & 0x80000000) == 0) {
        return 1;
    }
    mobj = mobjHitObjPtrToPtr(param_2);
    if (strcmp(mobj->mAnimPoseName, str_MOBJ_HiddenHatenaBlo_802c3dec) == 0 ||
        strcmp(mobj->mAnimPoseName, str_MOBJ_HiddenBadgeBloc_802c3e04) == 0 ||
        strcmp(mobj->mAnimPoseName, str_MOBJ_Hidden10CountBl_802c3e1c) == 0) {
        mobjHitObjPtrToPtr(param_2);
        return 0;
    }
    return 1;
}

void marioHitCheckWidth(void) {
    extern void* marioGetPtr(void);
    extern f32 float_18_80420960;
    extern f32 float_1_804208c4;
    extern f32 float_6_8042099c;
    void* mario = marioGetPtr();
    u32 flags = *(u32*)mario;
    f32 target;
    f32 width;

    if (!(flags & 0x100000)) {
        if (flags & 0x1000000) {
            target = float_18_80420960;
        } else {
            target = *(f32*)((s32)mario + 0x1B8);
        }
        width = *(f32*)((s32)mario + 0x1C0) + float_1_804208c4;
        *(f32*)((s32)mario + 0x1C0) = width;
        if (width > target) {
            *(f32*)((s32)mario + 0x1C0) = target;
        }
    } else {
        width = *(f32*)((s32)mario + 0x1C0);
        if (width > float_6_8042099c) {
            width -= float_1_804208c4;
            *(f32*)((s32)mario + 0x1C0) = width;
            if (width < float_6_8042099c) {
                *(f32*)((s32)mario + 0x1C0) = float_6_8042099c;
            }
        }
        width = *(f32*)((s32)mario + 0x1C0);
        if (width < float_6_8042099c) {
            width += float_1_804208c4;
            *(f32*)((s32)mario + 0x1C0) = width;
            if (width > float_6_8042099c) {
                *(f32*)((s32)mario + 0x1C0) = float_6_8042099c;
            }
        }
    }
}

void marioChkLandon(f32 velocityY, s32 param_2) {
    extern void* marioGetPtr(void);
    extern void marioLandonChk_old(f32 velocityY, s32 param_1);
    extern void marioSearchGround(s32 param_1, f32 velocityY1, f32 velocityY2, void* param_4, void* param_5);
    extern void marioSearchGroundRoll(s32 param_1, f32 velocityY1, f32 velocityY2, void* param_4, void* param_5);
    void* mario;
    u8 stack_18[4];
    u8 stack_14[8];

    mario = marioGetPtr();
    if (*(s8*)((s32)mario + 0x3C) != 2) {
        if ((*(u32*)mario & 0x1000000) == 0) {
            marioSearchGround(param_2, velocityY, velocityY, stack_14, stack_18);
        } else {
            marioSearchGroundRoll(param_2, velocityY, velocityY, stack_14, stack_18);
        }
    } else {
        marioLandonChk_old(velocityY, param_2);
    }
}

u32 marioChkBero(int param_1) {
    extern u32 hitGetAttr(void* hit);
    extern void* marioGetPtr(void);
    void* mario;
    u32 attr;

    attr = hitGetAttr((void*)param_1);
    mario = marioGetPtr();
    if ((attr & 0x80000) != 0 && (*(u32*)mario & 0x1000000) == 0) {
        return 0;
    }
    if ((attr & 0x80) == 0) {
        return (attr >> 23) & 1;
    }
    return *(u16*)((s32)mario + 0x2E) == 0x19;
}

