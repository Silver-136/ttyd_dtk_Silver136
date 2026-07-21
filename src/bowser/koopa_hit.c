#include "bowser/koopa_hit.h"

s32 kpaFlameHitFilter(void* unused, void* obj);

void* marioGetPtr(void);
void* hitCheckSphereFilter(void* filter, f32 x, f32 y, f32 z, f32 radius);

/* stub-fill: kpaEnemyFireHitChk | missing_definition | ghidra_signature */
u8 kpaEnemyFireHitChk(f32 x, f32 y, f32 z, f32 height, f32 width) {
    typedef struct VecLocal {
        f32 x;
        f32 y;
        f32 z;
    } VecLocal;

    extern void* getKoopaFireEfp(void);
    extern void* camGetPtr(s32 cameraId);
    extern void PSMTXTrans(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXRotRad(void* mtx, s32 axis, f32 rad);
    extern void PSMTXScale(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXConcat(void* a, void* b, void* out);
    extern void PSMTXMultVec(void* mtx, void* src, void* dst);
    extern void PSVECSubtract(void* a, void* b, void* out);
    extern f32 PSVECMag(void* v);
    extern f32 float_0p5_804273b0;
    extern f32 float_deg2rad_804273f4;
    extern f32 float_90_804273f8;
    extern f32 float_270_804273fc;
    extern f32 float_7_80427400;
    extern VecLocal vec3_802fec78;
    extern VecLocal vec3_802fec84;

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
    extern f32 float_0p5_804273b0;
    extern f32 float_50_804273f0;

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
    extern void* kpaGetKpaWork(void);
    extern s32 kpaGetLevel(void);
    extern void toMovedir(f64);
    extern s32 hitCheckFilter(f64, f64, f64, f64, f64, f64, s32,
                              f32*, f32*, f32*, f32*, f32*, f32*, f32*);
    extern f32 angleABf(f32, f32, f32, f32);
    extern f64 sin(f64);
    extern f64 cos(f64);
    extern f64 sqrt(f64);
    u8* mario = marioGetPtr();
    u8* work = kpaGetKpaWork();
    f32 start[3];
    f32 end[3];
    f32 best = -100000.0f;
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

    *(f32*)(mario + 0x218) = 0.0f;
    *(f32*)(mario + 0x21C) = 0.0f;
    *(f32*)(mario + 0x220) = 0.0f;
    *(f32*)(mario + 0x224) = 0.0f;
    *outY = *(f32*)(mario + 0x90);
    if (*(u32*)mario & 0x200) {
        return 0;
    }

    height += 0.01;
    *outDelta = -1.0f;
    *outAngle = 0.0f;
    if (*(f32*)(mario + 0x180) == 0.0f) {
        toMovedir(*(f32*)(mario + 0x1AC));
    }
    for (i = 0; i < 10; i++) {
        *(s32*)(work + 0x4C + i * 4) = 0;
        *(f32*)(work + 0xA4 + i * 4) = 1000.0f;
    }
    rays = kpaGetLevel() == 3 ? 7 : 3;

    for (i = 0; i < rays; i++) {
        f32 offsetX;
        f32 offsetZ;
        f32 radius;
        f64 angle = (f64)*(f32*)(mario + 0x1AC) / 180.0;
        switch (i) {
        case 0:
            offsetX = 0.0f;
            offsetZ = 0.0f;
            break;
        case 1:
            radius = 19.95f;
            offsetX = radius * (f32)sin(angle);
            offsetZ = -radius * (f32)cos(angle);
            break;
        case 2:
            radius = 19.95f;
            offsetX = -radius * (f32)sin(angle);
            offsetZ = -radius * (f32)cos(angle);
            break;
        case 3:
            radius = 39.95f;
            offsetX = radius * (f32)sin(angle);
            offsetZ = -radius * (f32)cos(angle);
            break;
        case 4:
            radius = 39.95f;
            offsetX = -radius * (f32)sin(angle);
            offsetZ = -radius * (f32)cos(angle);
            break;
        case 5:
            radius = 59.95f;
            offsetX = radius * (f32)sin(angle);
            offsetZ = -radius * (f32)cos(angle);
            break;
        default:
            radius = 59.95f;
            offsetX = -radius * (f32)sin(angle);
            offsetZ = -radius * (f32)cos(angle);
            break;
        }
        {
        f32 x = *(f32*)(mario + 0x8C) + offsetX;
        f32 z = *(f32*)(mario + 0x94) + offsetZ;
        start[0] = x;
        start[1] = *(f32*)(mario + 0x90) + (f32)height;
        start[2] = z;
        end[0] = x;
        end[1] = *(f32*)(mario + 0x90) - (f32)height;
        end[2] = z;
        hitDistance = (f32)height + 0.01f;
        {
        s32 hit = hitCheckFilter((f64)x, (f64)start[1], (f64)z,
                                 0.0, -1.0, 0.0, 0, hitData,
                                 &hitY, &hitZ, &hitDistance,
                                 &normalX, &normalY, &normalZ);
        if (hit != 0) {
            f32 horizontal = (f32)sqrt((f64)(normalX * normalX + normalZ * normalZ));
            if (angleABf(0.0f, 0.0f, horizontal, -normalY) >= 50.0f) {
                hit = 0;
            }
        }
        hitY = (f32)(s32)(100.0f * hitY + 0.5f) / 100.0f;
        if (hit != 0 && hitY > best) {
                best = hitY;
                *outY = best;
                *outDelta = *(f32*)(mario + 0x90) - best;
                *outAngle = angleABf(0.0f, 0.0f, normalX, normalZ);
                found = 1;
            }
        }
        }
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
    extern f32 float_0_804273c0;
    extern f32 float_neg1_804273c4;
    extern f32 float_1000_804273b4;
    extern f32 float_2_804273c8;
    extern f32 float_35_804273cc;
    extern f32 float_10_804273d0;
    extern f32 float_3p1416_804273b8;
    extern f32 float_180_804273bc;
    extern f32 float_1_804273d4;
    extern f32 float_100_804273d8;
    extern f32 float_0p5_804273b0;

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
    extern f32 float_1000_804273b4;
    extern f32 float_3p1416_804273b8;
    extern f32 float_180_804273bc;

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
