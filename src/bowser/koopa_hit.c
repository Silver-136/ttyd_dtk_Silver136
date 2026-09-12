#include "bowser/koopa_hit.h"

typedef struct KpaVec3Data {
    f32 x;
    f32 y;
    f32 z;
} KpaVec3Data;


s32 kpaFlameHitFilter(void* unused, void* obj);

void* marioGetPtr(void);
void* hitCheckSphereFilter(void* filter, f32 x, f32 y, f32 z, f32 radius);

/* stub-fill: kpaEnemyFireHitChk | missing_definition | ghidra_signature */
u8 kpaEnemyFireHitChk(f32 x, f32 y, f32 z, f32 height, f32 width) {
    typedef KpaVec3Data VecLocal;

    extern void* getKoopaFireEfp(void);
    extern void* camGetPtr(s32 cameraId);
    extern void PSMTXTrans(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXRotRad(void* mtx, s32 axis, f32 rad);
    extern void PSMTXScale(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXConcat(void* a, void* b, void* out);
    extern void PSMTXMultVec(void* mtx, void* src, void* dst);
    extern void PSVECSubtract(void* a, void* b, void* out);
    extern f32 PSVECMag(void* v);
    extern const f32 float_0p5_804273b0;
    extern const f32 float_deg2rad_804273f4;
    extern const f32 float_90_804273f8;
    extern const f32 float_270_804273fc;
    extern const f32 float_7_80427400;
    extern const KpaVec3Data vec3_802fec78;
    extern const KpaVec3Data vec3_802fec84;

    f32 transMtx[3][4];
    f32 rotMtx[3][4];
    f32 scaleMtx[3][4];
    f32 concatMtx[3][4];
    VecLocal src;
    VecLocal target;
    VecLocal pos;
    VecLocal diff;
    void* effect;
    void* work;
    s32 i;
    f32 baseScale;
    f32 radius;

    effect = getKoopaFireEfp();
    camGetPtr(4);
    src = vec3_802fec78;
    src.x = x;
    src.y = y + (float_0p5_804273b0 * height);
    src.z = z;
    target = src;

    if (effect == 0) {
        return 0;
    }

    work = *(void**)((s32)effect + 0xC);
    baseScale = *(f32*)((s32)work + 0x40);
    PSMTXTrans(transMtx, *(f32*)((s32)work + 4), *(f32*)((s32)work + 8), *(f32*)((s32)work + 0xC));
    camGetPtr(4);
    PSMTXRotRad(rotMtx, 0x79, float_deg2rad_804273f4 * (*(f32*)((s32)work + 0x48) - *(f32*)((s32)camGetPtr(4) + 0x114)));

    if (*(f32*)((s32)work + 0x48) > float_90_804273f8 && *(f32*)((s32)work + 0x48) < float_270_804273fc) {
        PSMTXScale(scaleMtx, *(f32*)((s32)work + 0x40), *(f32*)((s32)work + 0x40), -*(f32*)((s32)work + 0x40));
    } else {
        PSMTXScale(scaleMtx, *(f32*)((s32)work + 0x40), *(f32*)((s32)work + 0x40), *(f32*)((s32)work + 0x40));
    }

    PSMTXConcat(transMtx, rotMtx, transMtx);
    PSMTXConcat(transMtx, scaleMtx, concatMtx);

    radius = float_0p5_804273b0 * width;
    work = (void*)((s32)work + 0x50);
    for (i = 1; i < *(s32*)((s32)effect + 8); i++, work = (void*)((s32)work + 0x50)) {
        if (*(s32*)((s32)work + 0x44) == 0) {
            PSMTXTrans(transMtx, *(f32*)((s32)work + 4), *(f32*)((s32)work + 8), *(f32*)((s32)work + 0xC));
            baseScale = float_7_80427400 * *(f32*)((s32)work + 0x40);
            PSMTXScale(scaleMtx, baseScale, baseScale, baseScale);
            PSMTXConcat(transMtx, scaleMtx, transMtx);
            PSMTXConcat(concatMtx, transMtx, transMtx);
            pos = vec3_802fec84;
            PSMTXMultVec(transMtx, &pos, &pos);
            PSVECSubtract(&target, &pos, &diff);
            if (PSVECMag(&diff) < ((baseScale * baseScale) + radius)) {
                return 1;
            }
        }
    }

    return 0;
}

/* stub-fill: kpaEnemyHitChk | missing_definition | ghidra_signature */
u8 kpaEnemyHitChk(f32 x, f32 y, f32 z, f32 height, f32 width) {
    extern void* marioGetPtr(void);
    extern s32 kpaEnemyFireHitChk(f32 x, f32 y, f32 z, f32 height, f32 width);
    extern s32 kpaGetLevel(void);
    extern s32 kpaGetStageType(void);
    extern s32 kpaMutekiCheck(void);
    extern void marioChgMot(s32 mot);
    extern const f32 float_0p5_804273b0;
    extern const f32 float_50_804273f0;

    void* mario = marioGetPtr();
    f32 ySize;
    f32 xSize;
    f32 yDiff;
    f32 xDiff;

    if (kpaEnemyFireHitChk(x, y, z, height, width) != 0) {
        return 2;
    }

    ySize = *(f32*)((s32)mario + 0x1BC) + height;
    xSize = *(f32*)((s32)mario + 0x1B8) + width;
    yDiff = ((*(f32*)((s32)mario + 0x90) + (float_0p5_804273b0 * *(f32*)((s32)mario + 0x1BC))) - y) -
            (float_0p5_804273b0 * height);
    xDiff = *(f32*)((s32)mario + 0x8C) - x;

    if (__fabsf(yDiff) <= (float_0p5_804273b0 * ySize) &&
        __fabsf(xDiff) <= (float_0p5_804273b0 * xSize)) {
        if ((*(u32*)((s32)mario + 0x0) & 0x00020000) != 0) {
            if (*(f32*)((s32)mario + 0x90) > y) {
                if (kpaGetLevel() == 0 && kpaGetStageType() != 1 &&
                    *(s16*)((s32)mario + 0x4C) == 0 && z < float_50_804273f0) {
                    marioChgMot(4);
                }
                return 1;
            }
        } else {
            if (kpaMutekiCheck() == 1) {
                return 3;
            }
            if (*(s16*)((s32)mario + 0x4C) != 0) {
                return 3;
            }
            if (*(s16*)((s32)mario + 0x78) == 0) {
                return 4;
            }
        }
    }

    return 0;
}

/* stub-fill: kpaSearchGround | missing_definition | ghidra_signature */
s32 kpaSearchGround(f64 height, f32* outY, f32* outDelta, f32* outAngle) {
    extern void* marioGetPtr(void);
    extern s32 kpaGetLevel(void);
    extern f32 toMovedir(f64);
    extern s32 hitCheckFilter(f64, f64, f64, f64, f64, f64, s32,
                              f32*, f32*, f32*, f32*, f32*, f32*, f32*);
    extern f32 angleABf(f32, f32, f32, f32);
    extern f64 sin(f64);
    extern f64 cos(f64);
    extern f64 sqrt(f64);
    extern const f32 float_0p5_804273b0;
    extern const f32 float_1000_804273b4;
    extern const f32 float_3p1416_804273b8;
    extern const f32 float_180_804273bc;
    extern const f32 float_0_804273c0;
    extern const f32 float_neg1_804273c4;
    extern const f32 float_2_804273c8;
    extern const f32 float_100_804273d8;
    extern const f32 float_0p01_804273dc;
    extern const f32 float_0p25_804273e0;
    extern const f32 float_0p6_804273e4;
    extern const f32 float_3_804273e8;
    extern const f32 float_0p05_804273ec;
    extern const f32 float_50_804273f0;
    u8* mario = marioGetPtr();
    u8* work;
    f32 groundOffset;
    f32 direction;
    f32 baseRadius;
    f32 twoRadius;
    f32 threeRadius;
    f32 angleNumerator;
    f32 currentX;
    f32 currentY;
    f32 currentZ;
    s32 level;
    s32 rays;
    s32 i;
    s32 found = 0;
    f32 hitData[2];
    f32 hitY;
    f32 hitZ;
    f32 hitDistance;
    f32 normalX;
    f32 normalY;
    f32 normalZ;
    f32 horizontal;
    s32 hit;
    s32 converted;

    *(f32*)(mario + 0x21C) = float_0_804273c0;
    *(f32*)(mario + 0x220) = float_0_804273c0;
    *(f32*)(mario + 0x224) = float_0_804273c0;
    *(f32*)(mario + 0x218) = float_0_804273c0;
    if (*(u32*)mario & 0x200) {
        *outY = *(f32*)(mario + 0x90);
        return 0;
    }

    height += float_0p01_804273dc;
    level = kpaGetLevel();
    if (level == 0) {
        groundOffset = *(f32*)(mario + 0x1BC);
    } else {
        groundOffset = float_0p25_804273e0 * *(f32*)(mario + 0x1BC);
    }

    *outY = *(f32*)(mario + 0x90);
    *outDelta = float_neg1_804273c4;

    if (*(f32*)(mario + 0x180) != float_0_804273c0) {
        direction = *(f32*)(mario + 0x1A4);
    } else {
        direction = toMovedir(*(f32*)(mario + 0x1AC));
    }

    currentX = *(f32*)(mario + 0x8C);
    currentY = *(f32*)(mario + 0x90);
    currentZ = *(f32*)(mario + 0x94);

    for (i = 0; i < 10; i++) {
        work = *(u8**)((u8*)marioGetPtr() + 0x298);
        *(s32*)(work + 0x4C + i * 4) = 0;
        work = *(u8**)((u8*)marioGetPtr() + 0x298);
        *(f32*)(work + 0xA4 + i * 4) = float_1000_804273b4;
    }

    level = kpaGetLevel();
    if (level == 3) {
        rays = 7;
        baseRadius = float_0p6_804273e4 *
                     ((float_0p5_804273b0 * *(f32*)(mario + 0x1B8)) / float_3_804273e8);
    } else if (level < 3 && level > -1) {
        rays = 3;
        baseRadius = (f32)((f64)0.6f *
                           (0.5 * (f64)*(f32*)(mario + 0x1B8)));
    }

    angleNumerator = float_3p1416_804273b8 * direction;
    twoRadius = float_2_804273c8 * baseRadius - float_0p05_804273ec;
    threeRadius = float_3_804273e8 * baseRadius - float_0p05_804273ec;

    for (i = 0; i < rays; i++) {
        f32 offsetX;
        f32 offsetZ;
        f32 radius;
        f32 angle;
        f32 x;
        f32 z;

        switch (i) {
        case 0:
            offsetX = float_0_804273c0;
            offsetZ = float_0_804273c0;
            break;

        case 1:
            angle = angleNumerator / float_180_804273bc;
            radius = baseRadius - float_0p05_804273ec;
            offsetX = radius * (f32)sin(angle);
            offsetZ = -radius * (f32)cos(angle);
            break;

        case 2:
            angle = angleNumerator / float_180_804273bc;
            radius = baseRadius - float_0p05_804273ec;
            offsetX = -radius * (f32)sin(angle);
            offsetZ = -radius * (f32)cos(angle);
            break;

        case 3:
            angle = angleNumerator / float_180_804273bc;
            offsetX = twoRadius * (f32)sin(angle);
            offsetZ = -twoRadius * (f32)cos(angle);
            break;

        case 4:
            angle = angleNumerator / float_180_804273bc;
            offsetX = -twoRadius * (f32)sin(angle);
            offsetZ = -twoRadius * (f32)cos(angle);
            break;

        case 5:
            angle = angleNumerator / float_180_804273bc;
            offsetX = threeRadius * (f32)sin(angle);
            offsetZ = -threeRadius * (f32)cos(angle);
            break;

        default:
            angle = angleNumerator / float_180_804273bc;
            offsetX = -threeRadius * (f32)sin(angle);
            offsetZ = -threeRadius * (f32)cos(angle);
            break;
        }

        x = currentX + offsetX;
        z = currentZ + offsetZ;
        hitDistance = groundOffset + (f32)height;
        hit = hitCheckFilter((f64)x, (f64)(currentY + groundOffset), (f64)z,
                             (f64)float_0_804273c0, (f64)float_neg1_804273c4,
                             (f64)float_0_804273c0, 0, hitData,
                             &hitY, &hitZ, &hitDistance,
                             &normalX, &normalY, &normalZ);

        if (hit != 0) {
            mario = marioGetPtr();

            converted = (s32)(angleABf(
                float_0_804273c0,
                float_0_804273c0,
                float_100_804273d8 * normalY,
                float_100_804273d8 * normalZ) - float_180_804273bc);
            *(f32*)(mario + 0x21C) = (f32)converted;
            *(f32*)(mario + 0x220) = float_0_804273c0;

            converted = (s32)(angleABf(
                float_0_804273c0,
                float_0_804273c0,
                float_100_804273d8 * normalX,
                float_100_804273d8 * normalY) - float_180_804273bc);
            *(f32*)(mario + 0x224) = (f32)converted;

            horizontal = (f32)sqrt((f64)(normalX * normalX + normalZ * normalZ));
            if (angleABf(float_0_804273c0, float_0_804273c0, horizontal, -normalY) >=
                float_50_804273f0) {
                hit = 0;
            }
        }

        converted = (s32)(float_100_804273d8 * hitY + float_0p5_804273b0);
        hitY = (f32)converted / float_100_804273d8;

        if (hit != 0) {
            mario = marioGetPtr();

            converted = (s32)(angleABf(
                float_0_804273c0,
                float_0_804273c0,
                float_100_804273d8 * normalY,
                float_100_804273d8 * normalZ) - float_180_804273bc);
            *(f32*)(mario + 0x21C) = (f32)converted;
            *(f32*)(mario + 0x220) = float_0_804273c0;

            converted = (s32)(angleABf(
                float_0_804273c0,
                float_0_804273c0,
                float_100_804273d8 * normalX,
                float_100_804273d8 * normalY) - float_180_804273bc);
            *(f32*)(mario + 0x224) = (f32)converted;

            *(f32*)(mario + 0x218) = angleABf(
                float_0_804273c0,
                float_0_804273c0,
                (f32)sqrt((f64)(normalX * normalX + normalZ * normalZ)),
                -normalY);

            if (currentY <= hitY || __fabsf(hitY - currentY) < float_2_804273c8) {
                if (normalX == float_0_804273c0 && normalZ == float_0_804273c0) {
                    converted = (s32)(float_100_804273d8 * hitY + float_0p5_804273b0);
                    hitY = (f32)converted / float_100_804273d8;
                }
                *outY = hitY;
                *outDelta = float_0_804273c0;
                *outAngle = angleABf(currentX, currentZ, hitData[0], hitZ);
                found = hit;
            } else {
                *outDelta = currentY - hitY;
                *outAngle = angleABf(currentX, currentZ, hitData[0], hitZ);
                hit = 0;
            }
        }

        work = *(u8**)((u8*)marioGetPtr() + 0x298);
        *(s32*)(work + 0x4C + i * 4) = hit;
        work = *(u8**)((u8*)marioGetPtr() + 0x298);
        *(f32*)(work + 0xA4 + i * 4) = offsetX;
    }

    return found;
}

s32 kpaSearchHead(void* param2, f32* outValue, f32 height) {
    extern f32 toMovedir(f32 angle);
    extern s32 kpaGetLevel(void);
    extern f64 sin(f64 x);
    extern f64 cos(f64 x);
    extern s32 hitCheckFilter(f32 x, f32 y, f32 z, f32 ax, f32 ay, f32 az, s32 flags,
                              f32* outZ, f32* outY, f32* outX, f32* outDist,
                              void* outA, void* outB, void* outC);
    extern s32* headchkofs_581[4];
    extern const f32 float_0_804273c0;
    extern const f32 float_neg1_804273c4;
    extern const f32 float_1000_804273b4;
    extern const f32 float_2_804273c8;
    extern const f32 float_35_804273cc;
    extern const f32 float_10_804273d0;
    extern const f32 float_3p1416_804273b8;
    extern const f32 float_180_804273bc;
    extern const f32 float_1_804273d4;
    extern const f32 float_100_804273d8;
    extern const f32 float_0p5_804273b0;

    void* mario;
    f32 direction;
    s32 result;
    f32 checkHeight;
    s32 i;
    s32 offset;
    s32 zero;
    f32 resetValue;
    void* kpaWork;
    s32* headData;
    s32 count;
    f32 angleRad;
    s32 hitOffset;
    f32 outX;
    f32 outY;
    f32 outZ;
    f32 dist;
    f32 scratchA;
    f32 scratchB;
    f32 scratchC;

    mario = marioGetPtr();
    if ((*(u32*)((s32)mario + 0x0) & 0x00020000) != 0) {
        return 0;
    }

    if (*(f32*)((s32)mario + 0x180) != float_0_804273c0) {
        direction = *(f32*)((s32)mario + 0x1A4);
    } else {
        direction = toMovedir(*(f32*)((s32)mario + 0x1AC));
    }

    result = 0;
    checkHeight = *outValue;
    i = 0;
    offset = 0;
    zero = 0;
    resetValue = float_1000_804273b4;
    *outValue = float_neg1_804273c4;

    do {
        kpaWork = *(void**)((s32)marioGetPtr() + 0x298);
        *(s32*)((s32)kpaWork + offset + 0x20) = zero;
        kpaWork = *(void**)((s32)marioGetPtr() + 0x298);
        i++;
        *(f32*)((s32)kpaWork + offset + 0x78) = resetValue;
        offset += 4;
    } while (i < 10);

    headData = headchkofs_581[kpaGetLevel()];
    count = *headData;
    headData++;

    if ((*(u32*)((s32)mario + 0x0) & 0x00010000) == 0) {
        switch (kpaGetLevel()) {
            case 0:
                checkHeight = float_2_804273c8;
                break;
            case 1:
                checkHeight = float_0_804273c0;
                break;
            case 2:
                checkHeight = float_0_804273c0;
                break;
            case 3:
                checkHeight = float_35_804273cc;
                break;
        }
    } else {
        if (kpaGetLevel() == 0) {
            if (*(u16*)((s32)mario + 0x2E) == 0x25) {
                checkHeight = float_10_804273d0;
            } else {
                checkHeight = height;
            }
        } else {
            checkHeight = height;
        }
    }

    angleRad = (float_3p1416_804273b8 * direction) / float_180_804273bc;
    hitOffset = 0;

    for (i = 0; i < count; i++, hitOffset += 4) {
        s32 xOff;
        s32 yOff;
        s32 require;
        f32 sinValue;
        f32 xDelta;
        f32 cosValue;
        f32 yDelta;
        f32 zDelta;
        f32 checkX;
        f32 checkY;
        f32 checkZ;
        s32 hit;
        s32 rounded;

        sinValue = (f32)sin(angleRad);
        xOff = headData[0];
        xDelta = (f32)xOff * sinValue;

        cosValue = (f32)cos(angleRad);
        xOff = headData[0];
        yOff = headData[1];
        require = headData[2];
        headData += 3;
        zDelta = (f32)xOff * -cosValue;
        yDelta = (f32)yOff;

        if (require != 0 || ((*(u32*)((s32)mario + 0x0) & 0x00010000) != 0)) {
            checkX = *(f32*)((s32)mario + 0x8C) + xDelta;
            checkY = *(f32*)((s32)mario + 0x90) + yDelta;
            checkZ = *(f32*)((s32)mario + 0x94) + zDelta;
            dist = checkHeight;
            hit = hitCheckFilter(checkX, checkY, checkZ,
                                 float_0_804273c0, float_1_804273d4, float_0_804273c0,
                                 0, &outZ, &outY, &outX, &dist, &scratchA, &scratchB, &scratchC);
            if (hit != 0) {
                rounded = (s32)(float_100_804273d8 * outY + float_0p5_804273b0);
                outY = (f32)rounded / float_100_804273d8;
                if ((checkY + checkHeight) >= outY) {
                    *(f32*)((s32)param2 + 4) = outY;
                    result = hit;
                    *outValue = float_0_804273c0;
                    kpaWork = *(void**)((s32)marioGetPtr() + 0x298);
                    *(s32*)((s32)kpaWork + hitOffset + 0x20) = hit;
                } else {
                    *outValue = outY - *(f32*)((s32)param2 + 4) - checkHeight;
                }
            }
        }
    }

    return result;
}

s32 kpaSearchFrontUnder(void) {
    return 0;
}

void* kpaGetHitobjHead(s32 index) {
    s32 offset = index * 4;
    void* base = *(void**)((s32)marioGetPtr() + 0x298);
    return *(void**)((s32)base + offset + 0x20);
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* kpaGetHitobjHead2(void) {
    s32 offset = 0;
    s32 i = 0;
    void* obj = 0;
    for (; i < 10; i++, offset += 4) {
        void* base = *(void**)((s32)marioGetPtr() + 0x298);
        s32 index = offset + 0x20;
        obj = *(void**)((s32)base + index);
        if (obj != 0) {
            break;
        }
    }
    return obj;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


void* kpaGetHitobjRide(s32 index) {
    s32 offset = index * 4;
    void* base = *(void**)((s32)marioGetPtr() + 0x298);
    return *(void**)((s32)base + offset + 0x4C);
}

void* kpaChkHitobjRide(void) {
    void* work = *(void**)((s32)marioGetPtr() + 0x298);
    void* obj;

    obj = *(void**)((s32)work + 0x4C);
    if (obj == 0) {
        obj = *(void**)((s32)work + 0x50);
        if (obj == 0) {
            obj = *(void**)((s32)work + 0x54);
            if (obj == 0) {
                obj = *(void**)((s32)work + 0x58);
                if (obj == 0) {
                    obj = *(void**)((s32)work + 0x5C);
                    if (obj == 0) {
                        obj = *(void**)((s32)work + 0x60);
                        if (obj == 0) {
                            obj = *(void**)((s32)work + 0x64);
                            if (obj == 0) {
                                obj = *(void**)((s32)work + 0x68);
                                if (obj == 0) {
                                    obj = *(void**)((s32)work + 0x6C);
                                    if (obj == 0) {
                                        obj = *(void**)((s32)work + 0x70);
                                        if (obj != 0) {
                                            return obj;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return obj;
}
void kpaClearHitobjRide(void) {
    void* mario = marioGetPtr();

    *(void**)(*(s32*)((s32)mario + 0x298) + 0x4C) = 0;
    *(void**)(*(s32*)((s32)mario + 0x298) + 0x50) = 0;
    *(void**)(*(s32*)((s32)mario + 0x298) + 0x54) = 0;
    *(void**)(*(s32*)((s32)mario + 0x298) + 0x58) = 0;
    *(void**)(*(s32*)((s32)mario + 0x298) + 0x5C) = 0;
    *(void**)(*(s32*)((s32)mario + 0x298) + 0x60) = 0;
    *(void**)(*(s32*)((s32)mario + 0x298) + 0x64) = 0;
    *(void**)(*(s32*)((s32)mario + 0x298) + 0x68) = 0;
    *(void**)(*(s32*)((s32)mario + 0x298) + 0x6C) = 0;
    *(void**)(*(s32*)((s32)mario + 0x298) + 0x70) = 0;
}

void* kpaGetHitobj(void) {
    void* mario = marioGetPtr();
    if (*(s8*)((s32)mario + 0x3C) != 2) {
        return 0;
    }
    return *(void**)(*(s32*)((s32)mario + 0x298) + 0x1C);
}

void* kpaGetHitFlame(void) {
    void* mario = marioGetPtr();
    if (*(s8*)((s32)mario + 0x3C) != 2) {
        return 0;
    }
    return *(void**)(*(s32*)((s32)mario + 0x298) + 0x18);
}


/* CHATGPT STUB FILL: main/bowser/koopa_hit 20260624_184128 */

/* stub-fill: kpaHitCheck | missing_definition | ghidra_signature */
void* kpaHitCheck(void) {
    extern void* marioGetPtr(void);
    extern s32 kpaFireAttackCheck(void);
    extern s32 kpaGetLevel(void);
    extern f64 sin(f64 x);
    extern f64 cos(f64 x);
    extern void* hitCheckSphereFilter(void* filter, f32 x, f32 y, f32 z, f32 radius);
    extern u32 hitGetAttr(void* hitObj);
    extern void* mobjHitObjPtrToPtr(void* hitObj);
    extern s32* head_sphere_data[4];
    extern const f32 float_1000_804273b4;
    extern const f32 float_3p1416_804273b8;
    extern const f32 float_180_804273bc;

    void* result = 0;
    void* mario = marioGetPtr();
    void* hitObj;
    void* mobj;
    s32 i;
    s32 offset;
    s32* data;
    s32 count;
    f32 angle;
    f32 x;
    f32 y;
    f32 z;
    f32 radius;
    f32 sinValue;
    f32 cosValue;
    s32 xOff;
    s32 yOff;
    s32 zOff;
    s32 type;

    if (*(s8*)((s32)mario + 0x3C) != 2) {
        return 0;
    }

    *(void**)(*(s32*)((s32)mario + 0x298) + 0x1C) = 0;
    for (i = 0, offset = 0; i < 10; i++, offset += 4) {
        *(void**)(*(s32*)((s32)marioGetPtr() + 0x298) + offset + 0x20) = 0;
        *(f32*)(*(s32*)((s32)marioGetPtr() + 0x298) + offset + 0x78) = float_1000_804273b4;
    }

    if (kpaFireAttackCheck() == 1) {
        return 0;
    }

    data = head_sphere_data[kpaGetLevel()];
    count = *data++;
    angle = (float_3p1416_804273b8 * *(f32*)((s32)mario + 0x1A4)) / float_180_804273bc;
    for (i = 0, offset = 0; i < count; i++, offset += 4) {
        sinValue = (f32)sin(angle);
        xOff = data[0];
        x = *(f32*)((s32)mario + 0x8C) + ((f32)xOff * sinValue);
        cosValue = (f32)cos(angle);
        xOff = data[0];
        yOff = data[1];
        zOff = data[2];
        type = data[3];
        data += 4;
        z = *(f32*)((s32)mario + 0x94) - ((f32)xOff * cosValue);
        y = *(f32*)((s32)mario + 0x90) + (f32)yOff;
        radius = (f32)zOff;

        if (type == 1) {
            if ((*(u32*)((s32)mario + 0x0) & 0x00010000) == 0) {
                continue;
            }
        } else if (type == 2) {
            if ((*(u32*)((s32)mario + 0x0) & 0x00010000) != 0) {
                continue;
            }
        } else if (type == 3) {
            if ((*(u32*)((s32)mario + 0x0) & 0x00030000) != 0) {
                continue;
            }
        }

        hitObj = hitCheckSphereFilter(0, x, y, z, radius);
        if ((*(u32*)((s32)mario + 0x0) & 0x00010000) != 0) {
            if ((hitGetAttr(hitObj) & 0xFFFFFFFE) != 0) {
                mobj = mobjHitObjPtrToPtr(hitObj);
                if (*(f32*)((s32)mobj + 0x3C) > y) {
                    *(void**)(*(s32*)((s32)marioGetPtr() + 0x298) + offset + 0x20) = hitObj;
                }
            }
        }
        if (hitObj != 0) {
            result = hitObj;
        }
    }

    *(void**)(*(s32*)((s32)mario + 0x298) + 0x1C) = result;
    return result;
}

s32 kpaFlameHitFilter(void* unused, void* obj) {
    typedef struct KpaMobjName {
        u8 pad[0x15];
        char name[1];
    } KpaMobjName;
    extern KpaMobjName* mobjHitObjPtrToPtr(void* hitObj);
    extern s32 strcmp(const char* str1, const char* str2);
    extern const char str_MOBJ_HiddenHatenaBlo_802fec9c[];
    extern const char str_MOBJ_Block_802fecb4[];

    KpaMobjName* mobj = mobjHitObjPtrToPtr(obj);
    s32 cmp;

    if (strcmp(mobj->name, str_MOBJ_HiddenHatenaBlo_802fec9c) == 0) {
        return 0;
    }
    cmp = strcmp(mobj->name, str_MOBJ_Block_802fecb4);
    return ((u32)(-cmp) | (u32)cmp) >> 31;
}

void* kpaFlameHitCheck(f32 x, f32 y, f32 z, f32 radius) {
    marioGetPtr();
    return hitCheckSphereFilter(kpaFlameHitFilter, x, y, z, radius);
}

/* Target-owned koopa_hit read-only catalogs used by the reconstructed collision paths. */
const KpaVec3Data vec3_802fec78 = { 0.0f, 0.0f, 0.0f };
const KpaVec3Data vec3_802fec84 = { 0.0f, 0.0f, 0.0f };
const KpaVec3Data vec3_802fec90 = { 0.0f, 0.0f, 0.0f };
const char str_MOBJ_HiddenHatenaBlo_802fec9c[] = "MOBJ_HiddenHatenaBlock";
const char str_MOBJ_Block_802fecb4[] = "MOBJ_Block";

const f32 float_0p5_804273b0 = 0.5f;
const f32 float_1000_804273b4 = 1000.0f;
const f32 float_3p1416_804273b8 = 3.1415927f;
const f32 float_180_804273bc = 180.0f;
const f32 float_0_804273c0 = 0.0f;
const f32 float_neg1_804273c4 = -1.0f;
const f32 float_2_804273c8 = 2.0f;
const f32 float_35_804273cc = 35.0f;
const f32 float_10_804273d0 = 10.0f;
const f32 float_1_804273d4 = 1.0f;
const f32 float_100_804273d8 = 100.0f;
const f32 float_0p01_804273dc = 0.01f;
const f32 float_0p25_804273e0 = 0.25f;
const f32 float_0p6_804273e4 = 0.6f;
const f32 float_3_804273e8 = 3.0f;
const f32 float_0p05_804273ec = 0.05f;
const f32 float_50_804273f0 = 50.0f;
const f32 float_deg2rad_804273f4 = 0.017453292f;
const f32 float_90_804273f8 = 90.0f;
const f32 float_270_804273fc = 270.0f;
const f32 float_7_80427400 = 7.0f;
