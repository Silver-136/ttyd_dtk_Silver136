#include "party/party_hit.h"
extern void* marioGetPtr(void);
extern const char str_MOBJ_HiddenHatenaBlo_802cb8a0[];
extern const char str_MOBJ_HiddenBadgeBloc_802cb8b8[];
extern const char str_MOBJ_Hidden10CountBl_802cb8d0[];
extern f64 sin(f64 x);
extern f64 cos(f64 x);
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 chkfilter(s32 param_1, int param_2) {
    typedef struct MobjName {
        u8 pad[0x15];
        char name[16];
    } MobjName;
    extern MobjName* mobjHitObjPtrToPtr(int hit);
    extern s32 strcmp(char* a, const char* b);
    u32 attr;
    MobjName* mobj;

    marioGetPtr();
    attr = *(u32*)(param_2 + 4);
    if ((attr & 0x80080) != 0) {
        return 1;
    }
    if ((attr & 0x800004) != 0) {
        return 0;
    }
    if ((attr & 0x80000000) == 0) {
        return 1;
    }
    mobj = mobjHitObjPtrToPtr(param_2);
    if (strcmp(mobj->name, str_MOBJ_HiddenHatenaBlo_802cb8a0) == 0 ||
        strcmp(mobj->name, str_MOBJ_HiddenBadgeBloc_802cb8b8) == 0 ||
        strcmp(mobj->name, str_MOBJ_Hidden10CountBl_802cb8d0) == 0) {
        mobjHitObjPtrToPtr(param_2);
        return 0;
    }
    return 1;
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 chkfilterYoshi(s32 param_1, int param_2) {
    typedef struct MobjName {
        u8 pad[0x15];
        char name[16];
    } MobjName;
    extern MobjName* mobjHitObjPtrToPtr(int hit);
    extern s32 strcmp(char* a, const char* b);
    u32 attr;
    MobjName* mobj;

    marioGetPtr();
    attr = *(u32*)(param_2 + 4);
    if ((attr & 0x80080) != 0) {
        return 1;
    }
    if ((attr & 0x800000) != 0) {
        return 1;
    }
    if ((attr & 4) != 0) {
        return 0;
    }
    if ((attr & 0x80000000) == 0) {
        return 1;
    }
    mobj = mobjHitObjPtrToPtr(param_2);
    if (strcmp(mobj->name, str_MOBJ_HiddenHatenaBlo_802cb8a0) == 0 ||
        strcmp(mobj->name, str_MOBJ_HiddenBadgeBloc_802cb8b8) == 0 ||
        strcmp(mobj->name, str_MOBJ_Hidden10CountBl_802cb8d0) == 0) {
        mobjHitObjPtrToPtr(param_2);
        return 0;
    }
    return 1;
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 chkfilterNokotaro(s32 param_1, int param_2) {
    typedef struct MobjName {
        u8 pad[0x15];
        char name[16];
    } MobjName;
    extern MobjName* mobjHitObjPtrToPtr(int hit);
    extern s32 strcmp(char* a, const char* b);
    u32 attr;
    MobjName* mobj;

    marioGetPtr();
    attr = *(u32*)(param_2 + 4);
    if ((attr & 0x800005) != 0) {
        return 0;
    }
    if ((attr & 0x80080) != 0) {
        return 1;
    }
    if ((attr & 0x80000000) == 0) {
        return 1;
    }
    mobj = mobjHitObjPtrToPtr(param_2);
    if (strcmp(mobj->name, str_MOBJ_HiddenHatenaBlo_802cb8a0) == 0 ||
        strcmp(mobj->name, str_MOBJ_HiddenBadgeBloc_802cb8b8) == 0 ||
        strcmp(mobj->name, str_MOBJ_Hidden10CountBl_802cb8d0) == 0) {
        mobjHitObjPtrToPtr(param_2);
        return 0;
    }
    return 1;
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void partyHitCheck(void* pParty, unsigned int* param_2, unsigned int* param_3, float* param_4, float* param_5, float* param_6) {
    typedef struct RawVec {
        unsigned int x;
        unsigned int y;
        unsigned int z;
    } RawVec;
    typedef struct Vec {
        float x;
        float y;
        float z;
    } Vec;
    typedef struct HitWork {
        char unk0[0xC];
        RawVec start;
        RawVec end;
        Vec hitPos;
        Vec hitNormal;
        float dist;
    } HitWork;
    extern int yoshiGetStatus(void);
    extern int hitCheckVecFilter(void* work, void* filter);
    extern int chkfilterYoshi(int param_1, int param_2);
    extern int chkfilterNokotaro(int param_1, int param_2);
    extern int chkfilter(int param_1, int param_2);

    volatile RawVec inputCopy;
    HitWork work;
    float value;
    float bias;
    int result;

    inputCopy.x = param_3[0];
    inputCopy.y = param_3[1];
    inputCopy.z = param_3[2];
    work.start.x = param_2[0];
    work.start.y = param_2[1];
    work.start.z = param_2[2];
    work.end.x = inputCopy.x;
    work.end.y = inputCopy.y;
    work.end.z = inputCopy.z;
    work.dist = *param_6;

    if (yoshiGetStatus() != 0) {
        result = hitCheckVecFilter(&work, chkfilterYoshi);
    } else if (*(signed char*)((int)pParty + 0x31) == 2 && (*(unsigned int*)pParty & 0x100) != 0) {
        result = hitCheckVecFilter(&work, chkfilterNokotaro);
    } else {
        result = hitCheckVecFilter(&work, chkfilter);
    }

    if ((unsigned int)result != 0) {
        value = work.dist;
        bias = -0.5f;
        if (value >= 0.0f) {
            bias = 0.5f;
        }
        *param_6 = (float)(int)(value * 1000.0f + bias) / 1000.0f;

        value = work.hitPos.x;
        bias = -0.5f;
        if (value >= 0.0f) {
            bias = 0.5f;
        }
        param_4[0] = (float)(int)(value * 1000.0f + bias) / 1000.0f;

        value = work.hitPos.y;
        bias = -0.5f;
        if (value >= 0.0f) {
            bias = 0.5f;
        }
        param_4[1] = (float)(int)(value * 1000.0f + bias) / 1000.0f;

        value = work.hitPos.z;
        bias = -0.5f;
        if (value >= 0.0f) {
            bias = 0.5f;
        }
        param_4[2] = (float)(int)(value * 1000.0f + bias) / 1000.0f;

        param_5[0] = work.hitNormal.x;
        param_5[1] = work.hitNormal.y;
        param_5[2] = work.hitNormal.z;
    }
}
extern f64 __frsqrte(f64 value);
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset


u8 partyNrmToAngle(float* normal) {
    typedef struct VecBits {
        u32 x;
        u32 y;
        u32 z;
    } VecBits;
    extern f32 angleABf(f32 ax, f32 az, f32 bx, f32 bz);
    extern f32 float_0_80421508;
    extern f32 __float_nan[];
    extern const VecBits vec3_802cb770;
    union FloatBits {
        f32 value;
        u32 bits;
    } view;
    f32 normalX;
    f32 normalZ;
    f32 square;
    f32 length;
    f64 value;
    f64 inv;
    u32 exponent;
    s32 kind;
    register const u8* constants;

    normalX = normal[0];
    constants = (const u8*)&vec3_802cb770;
    normalZ = normal[2];
    square = normalX * normalX + normalZ * normalZ;
    value = (f64)square;

    if (value > (f64)float_0_80421508) {
        inv = __frsqrte(value);
        inv = *(const f64*)(constants + 0x110) * inv *
              -((value * inv * inv) - *(const f64*)(constants + 0x118));
        inv = *(const f64*)(constants + 0x110) * inv *
              -((value * inv * inv) - *(const f64*)(constants + 0x118));
        length = (f32)(value * *(const f64*)(constants + 0x110) * inv *
                       -((value * inv * inv) - *(const f64*)(constants + 0x118)));
        goto have_length;
    }

    if (value < *(const f64*)(constants + 0x120)) {
        length = __float_nan[0];
        goto have_length;
    }

    view.value = square;
    exponent = view.bits & 0x7F800000;
    length = square;

    if (exponent == 0x7F800000) {
        kind = (view.bits & 0x7FFFFF) != 0 ? 1 : 2;
    } else if (exponent < 0x7F800000 && exponent == 0) {
        kind = (view.bits & 0x7FFFFF) != 0 ? 5 : 3;
    } else {
        kind = 4;
    }
    if (kind == 1) {
        length = __float_nan[0];
    }

have_length:
    angleABf(
        float_0_80421508,
        float_0_80421508,
        length,
        -normal[1]);
}

u8 partyChkGnd(void* pParty) {
    extern s32 partySearchGround(f32 rise, f32 fall, void* party);
    extern u32 hitGetAttr(s32 hit);
    extern void partyChgMot(void* party, s32 motion);
    extern s32 partyChkFrontStep(f32 height, void* party, f32* outY, f32* outDiff, f32* outAngle);
    extern f32 float_11_80421534;
    extern f32 float_neg1_8042153c;
    extern f32 float_5_8042155c;
    extern f32 float_30_8042156c;
    extern f32 float_0_80421508;
    s32 hit;
    u32 attr;
    f32 y;
    f32 diff;
    f32 angle;

    if ((*(u32*)pParty & 0x2000000) == 0) {
        attr = 0;
        hit = partySearchGround(float_11_80421534, *(f32*)((s32)pParty + 0x114), pParty);
        if (hit != 0) {
            attr = hitGetAttr(hit);
            if ((attr & 0x200) != 0) {
                hit = 0;
            }
        }
        *(s32*)((s32)pParty + 0x138) = hit;
        *(u32*)pParty &= ~0x80;
        if (hit == 0) {
            *(u32*)pParty &= ~2;
            partyChgMot(pParty, 3);
            *(f32*)((s32)pParty + 0x11C) = float_neg1_8042153c;
        } else {
            *(u32*)pParty |= 2;
            if ((attr & 0x100) == 0) {
                *(u32*)pParty &= ~0x80;
            } else {
                *(u32*)pParty |= 0x80;
            }
        }
        *(f32*)((s32)pParty + 0x5C) = *(f32*)((s32)pParty + 0xE4);
        if ((*(u32*)pParty & 0x30) == 0) {
            partyChkFrontStep(float_11_80421534, pParty, &y, &diff, &angle);
            if (float_5_8042155c <= y - *(f32*)((s32)pParty + 0x5C) &&
                *(f32*)((s32)pParty + 0xE0) <= float_30_8042156c) {
                *(f32*)((s32)pParty + 0x120) = y;
                if (*(f32*)((s32)pParty + 0x104) != float_0_80421508) {
                    partyChgMot(pParty, 6);
                }
            }
        }
    }
}


#pragma no_register_save_helpers off
#pragma use_lmw_stmw on
s32 partySearchGround(f32 rise, f32 fall, void* party) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    typedef struct VecBits {
        u32 x;
        u32 y;
        u32 z;
    } VecBits;
    typedef struct HitWork {
        u8 pad[0xC];
        Vec3 start;
        Vec3 dir;
        Vec3 hitPos;
        Vec3 normal;
        f32 dist;
    } HitWork;
    union FloatBits {
        f32 value;
        u32 bits;
    };

    extern f32 partyGetHeight(void* party);
    extern f32 partyToMovedir(f32 direction, void* party);
    extern f32 angleABf(f32 x0, f32 z0, f32 x1, f32 z1);
    extern s32 yoshiGetStatus(void);
    extern void* hitCheckVecFilter(void* work, void* filter);
    extern s32 chkfilter(s32, s32);
    extern s32 chkfilterNokotaro(s32, s32);
    extern s32 chkfilterYoshi(s32, s32);
    extern f32 __fabsf(f32);
    extern f64 __frsqrte(f64);
    extern f32 __float_nan;
    extern const VecBits vec3_802cb770;
    extern const f32 float_0_80421508;
    extern const f32 float_0p1_80421558;
    extern const f32 float_0p375_80421560;
    extern const f32 float_0p5_80421504;
    extern const f32 float_0p75_80421528;
    extern const f32 float_2_80421554;
    extern const f32 float_3p1416_80421550;
    extern const f32 float_5_8042155c;
    extern const f32 float_11_80421534;
    extern const f32 float_50_80421568;
    extern const f32 float_100_80421520;
    extern const f32 float_1000_80421530;
    extern const f32 float_180_8042150c;
    extern const f32 float_neg0p5_8042152c;
    extern const f32 float_neg1_8042153c;
    extern const f32 float_neg10000_80421564;

    register const VecBits* vecCatalog = &vec3_802cb770;
    HitWork work;
    Vec3 startPos;
    Vec3 normalA;
    Vec3 normalB;
    Vec3 normalC;
    VecBits direction;
    void* mario;
    void* hit;
    void* bestHit;
    f32* pos;
    f32 riseAbs;
    f32 height;
    f32 angle;
    f32 sideX;
    f32 sideZ;
    f32 baseX;
    f32 baseY;
    f32 baseZ;
    f32 startY;
    f32 totalDist;
    f32 best;
    f32 hitY;
    f32 slopeAngle;
    f32 offX;
    f32 offZ;
    s32 i;

    pos = (f32*)((u8*)party + 0x58);
    bestHit = 0;

    if ((*(u32*)party & 0x02000000) != 0) {
        return 0;
    }

    /*
     * Target interleaves this fixed-vector copy with fabs(rise)+0.1.
     * Keeping the catalog base live also exposes vec3_802cb77c as entry 1.
     */
    *(VecBits*)((u8*)party + 0xC4) = vecCatalog[0];
    riseAbs = __fabsf(rise) + float_0p1_80421558;
    *(f32*)((u8*)party + 0xE0) = float_0_80421508;

    mario = marioGetPtr();
    if (*(u16*)((u8*)mario + 0x2E) == 0x1A) {
        height = float_5_8042155c;
    } else {
        height = float_0p75_80421528 * partyGetHeight(party);
    }

    baseY = pos[1];
    *(f32*)((u8*)party + 0xE4) = baseY;
    *(f32*)((u8*)party + 0xE8) = float_neg1_8042153c;

    if (*(f32*)((u8*)party + 0x104) != float_0_80421508) {
        angle = *(f32*)((u8*)party + 0x100);
    } else {
        angle = partyToMovedir(*(f32*)((u8*)party + 0x10C), party);
    }

    angle = float_3p1416_80421550 * angle / float_180_8042150c;
    sideX = float_0p375_80421560 * *(f32*)((u8*)party + 0xF4) * (f32)sin(angle);
    sideZ = float_0p375_80421560 * *(f32*)((u8*)party + 0xF4) * -(f32)cos(angle);

    direction = vecCatalog[1];
    baseX = pos[0];
    baseZ = pos[2];
    startY = baseY + height;
    totalDist = height + riseAbs;
    best = float_neg10000_80421564;

    for (i = 0; i < 5; i++) {
        if (i == 0) {
            offX = float_0_80421508;
            offZ = float_0_80421508;
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

        startPos.x = baseX + offX;
        startPos.y = startY;
        startPos.z = baseZ + offZ;
        work.start = startPos;
        *(VecBits*)&work.dir = direction;
        work.dist = totalDist;

        if (yoshiGetStatus() != 0) {
            hit = hitCheckVecFilter(&work, chkfilterYoshi);
        } else if (*(u8*)((u8*)party + 0x31) == 2 &&
                   (*(u32*)party & 0x100) != 0) {
            hit = hitCheckVecFilter(&work, chkfilterNokotaro);
        } else {
            hit = hitCheckVecFilter(&work, chkfilter);
        }

        /*
         * Target has a first hit block that only rounds the hit Y and copies
         * the normal into a distinct stack Vec, followed by a second test.
         */
        if (hit != 0) {
            f32 bias;
            s32 rounded;

            hitY = work.hitPos.y;
            if (hitY >= float_0_80421508) {
                bias = float_0p5_80421504;
            } else {
                bias = float_neg0p5_8042152c;
            }
            rounded = (s32)(float_1000_80421530 * hitY + bias);
            hitY = (f32)rounded / float_1000_80421530;
            normalA = work.normal;
        }

        if (hit != 0) {
            union FloatBits view;
            f32 square;
            f32 length;
            f64 value;
            f64 inv;
            u32 exponent;
            s32 kind;

            normalB = normalA;

            *(f32*)((u8*)party + 0xC4) =
                (f32)(s32)(angleABf(
                    float_0_80421508,
                    float_0_80421508,
                    float_100_80421520 * normalB.z,
                    float_100_80421520 * normalB.y) -
                           float_180_8042150c);
            *(f32*)((u8*)party + 0xC8) = float_0_80421508;
            *(f32*)((u8*)party + 0xCC) =
                (f32)(s32)(angleABf(
                    float_0_80421508,
                    float_0_80421508,
                    float_100_80421520 * normalB.x,
                    float_100_80421520 * normalB.y) -
                           float_180_8042150c);

            square = normalB.x * normalB.x + normalB.z * normalB.z;
            value = (f64)square;

            if (value > (f64)float_0_80421508) {
                inv = __frsqrte(value);
                inv = 0.5 * inv * -((value * inv * inv) - 3.0);
                inv = 0.5 * inv * -((value * inv * inv) - 3.0);
                length = (f32)(value * 0.5 * inv *
                               -((value * inv * inv) - 3.0));
            } else if (value < 0.0) {
                length = __float_nan;
            } else {
                view.value = square;
                exponent = view.bits & 0x7F800000;
                if (exponent == 0x7F800000) {
                    kind = (view.bits & 0x7FFFFF) == 0 ? 2 : 1;
                } else if (exponent < 0x7F800000 && exponent == 0) {
                    kind = (view.bits & 0x7FFFFF) == 0 ? 3 : 5;
                } else {
                    kind = 4;
                }
                length = kind == 1 ? __float_nan : square;
            }

            slopeAngle = angleABf(
                float_0_80421508,
                float_0_80421508,
                length,
                -normalB.y);

            if (slopeAngle >= float_50_80421568) {
                hit = 0;
            }
        }

        /*
         * Do not reuse slopeAngle here.  The target performs a third normal
         * copy and deliberately repeats the entire length/slope calculation.
         */
        if (hit != 0) {
            union FloatBits view;
            f32 square;
            f32 length;
            f32 diff;
            f32 absDiff;
            f32 fallLimit;
            f64 value;
            f64 inv;
            u32 exponent;
            s32 kind;

            normalC = normalA;

            *(f32*)((u8*)party + 0xC4) =
                (f32)(s32)(angleABf(
                    float_0_80421508,
                    float_0_80421508,
                    float_100_80421520 * normalC.z,
                    float_100_80421520 * normalC.y) -
                           float_180_8042150c);
            *(f32*)((u8*)party + 0xC8) = float_0_80421508;
            *(f32*)((u8*)party + 0xCC) =
                (f32)(s32)(angleABf(
                    float_0_80421508,
                    float_0_80421508,
                    float_100_80421520 * normalC.x,
                    float_100_80421520 * normalC.y) -
                           float_180_8042150c);

            square = normalC.x * normalC.x + normalC.z * normalC.z;
            value = (f64)square;

            if (value > (f64)float_0_80421508) {
                inv = __frsqrte(value);
                inv = 0.5 * inv * -((value * inv * inv) - 3.0);
                inv = 0.5 * inv * -((value * inv * inv) - 3.0);
                length = (f32)(value * 0.5 * inv *
                               -((value * inv * inv) - 3.0));
            } else if (value < 0.0) {
                length = __float_nan;
            } else {
                view.value = square;
                exponent = view.bits & 0x7F800000;
                if (exponent == 0x7F800000) {
                    kind = (view.bits & 0x7FFFFF) == 0 ? 2 : 1;
                } else if (exponent < 0x7F800000 && exponent == 0) {
                    kind = (view.bits & 0x7FFFFF) == 0 ? 3 : 5;
                } else {
                    kind = 4;
                }
                length = kind == 1 ? __float_nan : square;
            }

            slopeAngle = angleABf(
                float_0_80421508,
                float_0_80421508,
                length,
                -normalC.y);
            *(f32*)((u8*)party + 0xE0) = slopeAngle;

            diff = hitY - baseY;
            absDiff = __fabsf(diff);

            if (hitY >= baseY || absDiff < float_2_80421554) {
                if (absDiff <= float_11_80421534 && hitY > best) {
                    best = hitY;
                    bestHit = hit;
                }
            } else {
                fallLimit = __fabsf(fall);
                if (absDiff <= fallLimit) {
                    if (hitY > best) {
                        best = hitY;
                        bestHit = hit;
                    }
                } else {
                    *(f32*)((u8*)party + 0xE8) =
                        __fabsf(baseY - hitY);
                }
            }
        }
    }

    if (bestHit != 0) {
        *(f32*)((u8*)party + 0xE4) = best;
        *(f32*)((u8*)party + 0xE8) = float_0_80421508;
        return (s32)bestHit;
    }

    return 0;
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

s32 partyChkFrontStep(f32 heightAdd, void* pParty, f32* outY, f32* outDiff, f32* outAngle) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    typedef struct VecBits {
        u32 x;
        u32 y;
        u32 z;
    } VecBits;
    typedef struct HitWork {
        u8 pad[0xC];
        Vec3 start;
        Vec3 dir;
        Vec3 hitPos;
        Vec3 normal;
        f32 dist;
    } HitWork;
    union FloatBits {
        f32 value;
        u32 bits;
    };

    extern f32 partyGetHeight(void* party);
    extern f32 toMovedir(f32 dir);
    extern f32 revise360(f32 dir);
    extern s32 yoshiGetStatus(void);
    extern void* hitCheckVecFilter(void* work, void* filter);
    extern s32 chkfilterYoshi(s32 a, s32 b);
    extern s32 chkfilterNokotaro(s32 a, s32 b);
    extern s32 chkfilter(s32 a, s32 b);
    extern f32 angleABf(f32 ax, f32 az, f32 bx, f32 bz);
    extern f32 __fabsf(f32);
    extern f64 __frsqrte(f64);
    extern f32 __float_nan;
    extern const VecBits vec3_802cb770;
    extern f32 float_0_80421508;
    extern f32 float_0p01_80421548;
    extern f32 float_0p5_80421504;
    extern f32 float_neg0p5_8042152c;
    extern f32 float_neg1_8042153c;
    extern f32 float_2_80421554;
    extern f32 float_3p1416_80421550;
    extern f32 float_100_80421520;
    extern f32 float_180_8042150c;
    extern f32 float_1000_80421530;
    extern f32 float_0p265_8042154c;

    register const VecBits* vecCatalog = &vec3_802cb770;
    HitWork work;
    Vec3 startPos;
    Vec3 normalA;
    Vec3 normalB;
    void* hit;
    f32 partyHeight;
    f32 direction;
    f32 radians;
    f32 sinDir;
    f32 cosDir;
    f32 widthOffset;
    f32 partyX;
    f32 partyY;
    f32 partyZ;
    f32 roundedX;
    f32 roundedY;
    f32 roundedZ;
    f32 bias;
    f32 square;
    f32 length;
    f32 yDiffAbs;

    *(VecBits*)((u8*)pParty + 0xC4) = vecCatalog[2];
    *(f32*)((u8*)pParty + 0xE0) = float_0_80421508;

    partyHeight = partyGetHeight(pParty);
    partyY = *(f32*)((u8*)pParty + 0x5C);
    partyX = *(f32*)((u8*)pParty + 0x58);
    *outY = partyY;
    *outDiff = float_neg1_8042153c;

    if (*(f32*)((u8*)pParty + 0x104) != float_0_80421508) {
        direction = *(f32*)((u8*)pParty + 0x100);
    } else {
        direction = toMovedir(*(f32*)((u8*)pParty + 0x10C));
    }

    direction = revise360((f32)(s32)(direction + float_0p5_80421504));
    radians = float_3p1416_80421550 * direction / float_180_8042150c;
    widthOffset = float_0p265_8042154c * *(f32*)((u8*)pParty + 0xF4);
    sinDir = (f32)sin(radians);
    cosDir = (f32)cos(radians);
    partyZ = *(f32*)((u8*)pParty + 0x60);

    *(VecBits*)&startPos = vecCatalog[3];
    startPos.x = partyX + widthOffset * sinDir;
    startPos.y = partyY + partyHeight;
    startPos.z = partyZ + widthOffset * -cosDir;
    work.start = startPos;
    *(VecBits*)&work.dir = vecCatalog[4];
    work.dist = partyHeight + (heightAdd + float_0p01_80421548);

    if (yoshiGetStatus() != 0) {
        hit = hitCheckVecFilter(&work, chkfilterYoshi);
    } else if (*(s8*)((u8*)pParty + 0x31) == 2 &&
               (*(u32*)pParty & 0x100) != 0) {
        hit = hitCheckVecFilter(&work, chkfilterNokotaro);
    } else {
        hit = hitCheckVecFilter(&work, chkfilter);
    }

    if (hit == 0) {
        return 0;
    }

    bias = work.hitPos.x >= float_0_80421508
               ? float_0p5_80421504
               : float_neg0p5_8042152c;
    roundedX =
        (f32)(s32)(work.hitPos.x * float_1000_80421530 + bias) /
        float_1000_80421530;

    bias = work.hitPos.y >= float_0_80421508
               ? float_0p5_80421504
               : float_neg0p5_8042152c;
    roundedY =
        (f32)(s32)(work.hitPos.y * float_1000_80421530 + bias) /
        float_1000_80421530;

    bias = work.hitPos.z >= float_0_80421508
               ? float_0p5_80421504
               : float_neg0p5_8042152c;
    roundedZ =
        (f32)(s32)(work.hitPos.z * float_1000_80421530 + bias) /
        float_1000_80421530;

    normalA = work.normal;
    normalB = normalA;

    *(f32*)((u8*)pParty + 0xC4) =
        (f32)(s32)(angleABf(
            float_0_80421508,
            float_0_80421508,
            float_100_80421520 * normalB.z,
            float_100_80421520 * normalB.y) -
                   float_180_8042150c);
    *(f32*)((u8*)pParty + 0xC8) = float_0_80421508;
    *(f32*)((u8*)pParty + 0xCC) =
        (f32)(s32)(angleABf(
            float_0_80421508,
            float_0_80421508,
            float_100_80421520 * normalB.x,
            float_100_80421520 * normalB.y) -
                   float_180_8042150c);

    square = normalB.x * normalB.x + normalB.z * normalB.z;
    {
        union FloatBits view;
        f64 value = (f64)square;
        f64 inv;
        u32 exponent;
        s32 kind;

        if (value > 0.0) {
            inv = __frsqrte(value);
            inv = 0.5 * inv * -((value * inv * inv) - 3.0);
            inv = 0.5 * inv * -((value * inv * inv) - 3.0);
            length = (f32)(value * 0.5 * inv *
                           -((value * inv * inv) - 3.0));
        } else if (value < 0.0) {
            length = __float_nan;
        } else {
            view.value = square;
            exponent = view.bits & 0x7F800000;
            if (exponent == 0x7F800000) {
                kind = (view.bits & 0x7FFFFF) == 0 ? 2 : 1;
            } else if (exponent < 0x7F800000 && exponent == 0) {
                kind = (view.bits & 0x7FFFFF) == 0 ? 3 : 5;
            } else {
                kind = 4;
            }
            length = kind == 1 ? __float_nan : square;
        }
    }

    *(f32*)((u8*)pParty + 0xE0) =
        angleABf(
            float_0_80421508,
            float_0_80421508,
            length,
            -normalB.y);

    yDiffAbs = __fabsf(roundedY - partyY);

    if (roundedY < partyY && yDiffAbs < float_2_80421554) {
        *outDiff = partyY - roundedY;
        *outAngle = angleABf(partyX, partyZ, roundedX, roundedZ);
        return 0;
    }

    *outY = roundedY;
    *outDiff = float_0_80421508;
    *outAngle = angleABf(
        partyX,
        partyZ,
        work.hitPos.x,
        work.hitPos.z);
    return (s32)hit;
}

#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

void* partySearchWallFront(f32 distance, f32 direction, void* pParty, f32* pos) {
    extern f32 float_1_8042151c;
    extern f32 float_45_80421538;
    typedef struct VecLocal {
        f32 x;
        f32 y;
        f32 z;
    } VecLocal;
    typedef struct VecBits {
        u32 x;
        u32 y;
        u32 z;
    } VecBits;
    extern void sincosf(f32 angle, f32* cosOut, f32* sinOut);
    extern f32 partyGetHeight(void* party);
    extern s32 yoshiGetStatus(void);
    extern s32 strcmp(const char* a, const char* b);
    extern void* marioHitCheckVec(void* pos, void* dir, void* outNorm, void* outHitPos, f32* dist);
    extern void* gp;
    extern f32 float_0p5_80421504;
    extern f32 float_0p75_80421528;
    extern f32 float_11_80421534;
    extern f32 float_120_80421540;
    extern f32 float_350_80421544;
    extern const VecBits vec3_802cb770;

    register const VecBits* vecCatalog = &vec3_802cb770;
    f32 heights[4];
    VecLocal dir;
    VecLocal sideDir;
    VecLocal posWork;
    VecLocal callPos;
    VecLocal callDir;
    VecLocal normal;
    VecLocal bestNormal;
    VecLocal hitPos;
    VecLocal push;
    void* hit;
    void* bestHit;
    f32 width;
    volatile f32 bestDist;
    f32 dist;
    f32 dot;
    f32 dirX;
    f32 dirZ;
    f32* height;
    s32 count;
    s32 i;
    void* sideHit;
    void* secondSideHit;

    *(VecBits*)&push = vecCatalog[6];
    sincosf(direction, &dirX, &dirZ);
    *(VecBits*)&dir = vecCatalog[7];
    dir.x = dirX;
    dir.z = dirZ;
    count = 3;
    heights[0] = float_0p75_80421528 * partyGetHeight(pParty);
    heights[1] = float_0p5_80421504 * partyGetHeight(pParty);
    if ((*(u32*)pParty & 0x30) != 0) {
        heights[2] = float_1_8042151c;
    } else if (yoshiGetStatus() == 2) {
        heights[2] = float_1_8042151c;
    } else {
        heights[2] = float_11_80421534;
        if (strcmp((const char*)((u8*)gp + 0x12C),
                   (const char*)((u8*)vecCatalog + 0x128)) == 0 &&
            *(f32*)((u8*)pParty + 0x5C) >= float_120_80421540 &&
            *(f32*)((u8*)pParty + 0x58) <= float_350_80421544) {
            count = 4;
            heights[3] = float_1_8042151c;
        }
    }

    width = float_0p5_80421504 * *(f32*)((s32)pParty + 0xF4);
    bestDist = width + distance;
    bestHit = 0;
    posWork.x = pos[0];
    posWork.y = pos[1];
    posWork.z = pos[2];
    height = heights;
    for (i = 0; i < count; i++, height++) {
        posWork.y = pos[1] + *height;
        dist = width + distance;
        hit = marioHitCheckVec(&posWork, &dir, &hitPos, &normal, &dist);
        if (hit != 0) {
            if (bestHit == 0) {
                bestDist = dist;
                bestNormal = normal;
                bestHit = hit;
            } else if (dist < bestDist) {
                bestDist = dist;
                bestNormal = normal;
                bestHit = hit;
            }
        }
    }
    if (bestHit != 0) {
        normal = bestNormal;
        dot = (distance * dir.x) * normal.x + (distance * dir.z) * normal.z;
        push.x += float_0p5_80421504 * -(dot * normal.x - (distance * dir.x));
        push.z += float_0p5_80421504 * -(dot * normal.z - (distance * dir.z));
    }

    sideHit = 0;
    sincosf(direction + float_45_80421538, &dirX, &dirZ);
    *(VecBits*)&sideDir = vecCatalog[8];
    sideDir.x = dirX;
    sideDir.z = dirZ;
    posWork.x = pos[0];
    posWork.y = pos[1];
    posWork.z = pos[2];
    height = heights;
    for (i = 0; i < count; i++, height++) {
        posWork.y = pos[1] + *height;
        callPos = posWork;
        callDir = sideDir;
        dist = width;
        sideHit = marioHitCheckVec(&callPos, &callDir, 0, 0, &dist);
        if (sideHit != 0) {
            break;
        }
    }
    secondSideHit = 0;
    if (sideHit != 0) {
        sincosf(direction - float_45_80421538, &dirX, &dirZ);
        height = heights;
        for (i = 0; i < count; i++, height++) {
            *(VecBits*)&sideDir = vecCatalog[9];
            sideDir.x = dirX;
            sideDir.z = dirZ;
            posWork.y = pos[1] + *height;
            callPos = posWork;
            callDir = sideDir;
            dist = width;
            secondSideHit = marioHitCheckVec(&callPos, &callDir, 0, 0, &dist);
        }
    }

    if (bestHit != 0) {
        if (secondSideHit == 0) {
            pos[0] += push.x;
            pos[2] += push.z;
        }
        return bestHit;
    }

    if (secondSideHit == 0) {
        pos[0] += distance * dir.x;
        pos[2] += distance * dir.z;
    }
    return 0;
}

#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 partySearchFrontWall(f32 distance, f32 angle, void* pParty, f32* pos) {
    typedef struct HitWork {
        u8 pad[0xC];
        f32 start[3];
        f32 dir[3];
        f32 hitPos[3];
        f32 normal[3];
        f32 dist;
    } HitWork;
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    typedef struct VecBits {
        u32 x;
        u32 y;
        u32 z;
    } VecBits;
    union FloatBits {
        f32 value;
        u32 bits;
    };

    extern void sincosf(f32, f32*, f32*);
    extern f32 partyGetHeight(void*);
    extern s32 yoshiGetStatus(void);
    extern s32 hitCheckVecFilter(void*, void*);
    extern s32 chkfilterYoshi(s32, s32), chkfilterNokotaro(s32, s32), chkfilter(s32, s32);
    extern f32 angleABf(f32, f32, f32, f32);
    extern f64 __frsqrte(f64);
    extern f32 __float_nan;
    extern const VecBits vec3_802cb770;
    extern f32 float_0_80421508, float_0p5_80421504, float_0p75_80421528;
    extern f32 float_1_8042151c, float_6_80421518, float_11_80421534;
    extern f32 float_45_80421538, float_1000_80421530;
    extern f32 float_neg0p5_8042152c, float_neg1_8042153c;

    HitWork work1;
    HitWork work2;
    Vec3 startPos;
    Vec3 normal;
    Vec3 direction;
    Vec3 response1;
    Vec3 response2;
    f32 c;
    f32 s;
    f32 radius;
    f32 roundedDist;
    f32 bias;
    f32 dot;
    register const VecBits* vecCatalog = &vec3_802cb770;
    s32 hit;

    sincosf(angle, &c, &s);

    radius = *(f32*)((u8*)pParty + 0xF4);
    if ((*(u32*)pParty & 0x1000) != 0) {
        radius = float_6_80421518;
    }
    radius = float_0p5_80421504 * radius + distance;
    roundedDist = radius;

    {
        f32 height = partyGetHeight(pParty);
        startPos = *(Vec3*)((u8*)pParty + 0x58);
        startPos.y += float_0p75_80421528 * height;
    }

    *(VecBits*)&direction = vecCatalog[10];
    direction.x = c;
    direction.z = s;

    *(Vec3*)work1.start = startPos;
    *(Vec3*)work1.dir = direction;
    work1.dist = radius;

    if (yoshiGetStatus() != 0) {
        hit = hitCheckVecFilter(&work1, chkfilterYoshi);
    } else if (*(s8*)((u8*)pParty + 0x31) == 2 &&
               (*(u32*)pParty & 0x100) != 0) {
        hit = hitCheckVecFilter(&work1, chkfilterNokotaro);
    } else {
        hit = hitCheckVecFilter(&work1, chkfilter);
    }

    if (hit != 0) {
        s32 rounded;

        bias = float_neg0p5_8042152c;
        if (work1.dist >= float_0_80421508) {
            bias = float_0p5_80421504;
        }
        rounded = (s32)(float_1000_80421530 * work1.dist + bias);
        roundedDist = (f32)rounded / float_1000_80421530;
        normal = *(Vec3*)work1.normal;

        *(f32*)((u8*)pParty + 0xEC) = roundedDist;
        response1 = normal;
        dot = (distance * c) * response1.x +
              (distance * s) * response1.z;
        pos[0] += float_0p5_80421504 *
                  -(dot * response1.x - distance * c);
        pos[2] += float_0p5_80421504 *
                  -(dot * response1.z - distance * s);
        return hit;
    }

    if (hit == 0) {
        f32 height = float_11_80421534;

        if (*(s8*)((u8*)pParty + 0x31) == 3 &&
            (*(u32*)pParty & 0x100) != 0) {
            height = float_1_8042151c;
        }

        startPos = *(Vec3*)((u8*)pParty + 0x58);
        startPos.y += height;

        *(Vec3*)work2.start = startPos;
        *(Vec3*)work2.dir = direction;
        work2.dist = radius;

        if (yoshiGetStatus() != 0) {
            hit = hitCheckVecFilter(&work2, chkfilterYoshi);
        } else if (*(s8*)((u8*)pParty + 0x31) == 2 &&
                   (*(u32*)pParty & 0x100) != 0) {
            hit = hitCheckVecFilter(&work2, chkfilterNokotaro);
        } else {
            hit = hitCheckVecFilter(&work2, chkfilter);
        }

        if (hit != 0) {
            s32 rounded;

            bias = float_neg0p5_8042152c;
            if (work2.dist >= float_0_80421508) {
                bias = float_0p5_80421504;
            }
            rounded = (s32)(float_1000_80421530 * work2.dist + bias);
            roundedDist = (f32)rounded / float_1000_80421530;
            normal = *(Vec3*)work2.normal;
        }

        if (hit != 0) {
            union FloatBits view;
            f32 square;
            f32 length;
            f64 value;
            f64 inv;
            u32 exponent;
            s32 kind;

            square = normal.x * normal.x + normal.z * normal.z;
            value = (f64)square;

            if (value > (f64)float_0_80421508) {
                inv = __frsqrte(value);
                inv = 0.5 * inv * -((value * inv * inv) - 3.0);
                inv = 0.5 * inv * -((value * inv * inv) - 3.0);
                length = (f32)(value * 0.5 * inv *
                               -((value * inv * inv) - 3.0));
            } else if (value < 0.0) {
                length = __float_nan;
            } else {
                view.value = square;
                exponent = view.bits & 0x7F800000;
                if (exponent == 0x7F800000) {
                    kind = (view.bits & 0x7FFFFF) == 0 ? 2 : 1;
                } else if (exponent < 0x7F800000 && exponent == 0) {
                    kind = (view.bits & 0x7FFFFF) == 0 ? 3 : 5;
                } else {
                    kind = 4;
                }
                length = kind == 1 ? __float_nan : square;
            }

            if (angleABf(float_0_80421508, float_0_80421508,
                         length, -normal.y) >= float_45_80421538) {
                *(f32*)((u8*)pParty + 0xEC) = roundedDist;
                response2 = normal;
                dot = (distance * c) * response2.x +
                      (distance * s) * response2.z;
                pos[0] += float_0p5_80421504 *
                          -(dot * response2.x - distance * c);
                pos[2] += float_0p5_80421504 *
                          -(dot * response2.z - distance * s);
                return hit;
            }
        }

        *(f32*)((u8*)pParty + 0xEC) = float_neg1_8042153c;
        pos[0] += distance * c;
        pos[2] += distance * s;
        return 0;
    }

    return hit;
}

#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void partySearchHead(f32 angle, void* pParty, f32* pos, f32* dist) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;

    extern f32 float_1_8042151c;
    extern void sincosf(f32 angle, f32* cosOut, f32* sinOut);
    extern u32 hitCheckFilter(f64 x, f64 y, f64 z, f64 vx, f64 vy, f64 vz, s32 flags,
                              void* out0, void* out1, void* out2, f32* outDist,
                              void* out4, void* out5, void* out6);
    extern f32 float_0_80421508;
    extern f32 float_10_80421510;
    extern f32 float_0p3_80421524;
    extern const Vec3 vec3_802cb770[];

    /*
     * Declaration order mirrors the target stack:
     *   callPos             0xBC
     *   probe0..probe3      0xB0..0x8C
     *   cos/sin             0x88/0x84
     *   original            0x78
     *   four 7-float output groups 0x5C..0x08
     */
    Vec3 callPos;
    Vec3 probe0;
    Vec3 probe1;
    Vec3 probe2;
    Vec3 probe3;
    volatile struct {
        Vec3 original;
        f32 s;
        f32 c;
    } local;

    f32 d0;
    f32 o06;
    f32 o05;
    f32 o04;
    f32 o02;
    f32 o01;
    f32 o00;

    f32 d1;
    f32 o16;
    f32 o15;
    f32 o14;
    f32 o12;
    f32 o11;
    f32 o10;

    f32 d2;
    f32 o26;
    f32 o25;
    f32 o24;
    f32 o22;
    f32 o21;
    f32 o20;

    f32 d3;
    f32 o36;
    f32 o35;
    f32 o34;
    f32 o32;
    f32 o31;
    f32 o30;

    const Vec3* catalog;
    f32 width;
    f32 best;
    f32 dx;
    f32 dz;
    f32 x;
    f32 y;
    f32 z;
    u32 hit;

    width = *(f32*)((s32)pParty + 0xF4);
    catalog = vec3_802cb770;
    width = float_0p3_80421524 * width;

    sincosf(angle, (f32*)&local.c, (f32*)&local.s);

    local.original.x = pos[0];
    local.original.y = pos[1];
    local.original.z = pos[2];

    best = *dist;
    dx = width * local.c;
    dz = -width * local.s;

    probe0 = catalog[12];
    x = local.original.x + dx;
    y = local.original.y;
    z = local.original.z + dz;
    probe0.x = x;
    probe0.y = y;
    probe0.z = z;
    callPos = probe0;
    d0 = best;
    hit = hitCheckFilter(
        callPos.x, callPos.y + float_10_80421510, callPos.z,
        float_0_80421508, float_1_8042151c, float_0_80421508,
        0, &o00, &o01, &o02, &d0, &o04, &o05, &o06);
    if (hit != 0 && best >= d0) {
        best = d0;
    }
    if (hit != 0) {
        goto done;
    }

    probe1 = catalog[13];
    x = local.original.x - dx;
    y = local.original.y;
    z = local.original.z - dz;
    probe1.x = x;
    probe1.y = y;
    probe1.z = z;
    callPos = probe1;
    d1 = best;
    hit = hitCheckFilter(
        callPos.x, callPos.y + float_10_80421510, callPos.z,
        float_0_80421508, float_1_8042151c, float_0_80421508,
        0, &o10, &o11, &o12, &d1, &o14, &o15, &o16);
    if (hit != 0 && best >= d1) {
        best = d1;
    }
    if (hit != 0) {
        goto done;
    }

    probe2 = catalog[14];
    x = local.original.x + dz;
    y = local.original.y;
    z = local.original.z + dx;
    probe2.x = x;
    probe2.y = y;
    probe2.z = z;
    callPos = probe2;
    d2 = best;
    hit = hitCheckFilter(
        callPos.x, callPos.y + float_10_80421510, callPos.z,
        float_0_80421508, float_1_8042151c, float_0_80421508,
        0, &o20, &o21, &o22, &d2, &o24, &o25, &o26);
    if (hit != 0 && best >= d2) {
        best = d2;
    }
    if (hit != 0) {
        goto done;
    }

    probe3 = catalog[15];
    x = local.original.x - dz;
    y = local.original.y;
    z = local.original.z - dx;
    probe3.x = x;
    probe3.y = y;
    probe3.z = z;
    callPos = probe3;
    d3 = best;
    hit = hitCheckFilter(
        callPos.x, callPos.y + float_10_80421510, callPos.z,
        float_0_80421508, float_1_8042151c, float_0_80421508,
        0, &o30, &o31, &o32, &d3, &o34, &o35, &o36);
    if (hit != 0 && best >= d3) {
        best = d3;
    }

done:
    if (hit != 0) {
        pos[0] = x;
        pos[1] = y;
        pos[2] = z;
        *dist = best;
    }
    if (hit == 0) {
        pos[0] = x;
        pos[1] = y;
        pos[2] = z;
        *dist = float_0_80421508;
    }
}

#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset


s32 unk_800c27c0(void* flags, void* mario) {
    extern f32 float_6_80421518;
    extern s32 marioChkWallAround(void* mario, s32 arg, f32 a, f32 b, f32 radius);
    f32 a;
    f32 b;
    f32 radius;

    if (*(u32*)flags & 0x1000) {
        radius = float_6_80421518;
    }
    return marioChkWallAround(mario, 1, a, b, radius);
}
/* CHATGPT FALLBACK MISSING STUBS: main/party/party_hit 20260624_191429 */

/* fallback stub-fill: map=unk_800c2010 addr=0x800c2010 size=0x000007b0 */
extern f32 __fabsf(f32 value);
s32 unk_800c2010(void* party, f32* pos) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    typedef struct VecBits {
        u32 x;
        u32 y;
        u32 z;
    } VecBits;
    typedef struct HitWork {
        u8 pad[0xC];
        Vec3 start;
        Vec3 dir;
        Vec3 hitPos;
        Vec3 normal;
        f32 dist;
    } HitWork;

    extern f32 toMovedirSimple(f32 angle);
    extern void sincosf(f32 angle, f32* sinOut, f32* cosOut);
    extern f32 angleABf(f32, f32, f32, f32);
    extern s32 yoshiGetStatus(void);
    extern void* hitCheckVecFilter(void* work, void* filter);
    extern s32 chkfilter(s32, s32);
    extern s32 chkfilterNokotaro(s32, s32);
    extern s32 chkfilterYoshi(s32, s32);
    extern const VecBits vec3_802cb770;
    extern const f32 float_0_80421508;
    extern const f32 float_0p5_80421504;
    extern const f32 float_10_80421510;
    extern const f32 float_20_80421514;
    extern const f32 float_180_8042150c;
    extern const f32 float_270_80421500;

    register const VecBits* catalog = &vec3_802cb770;

    /*
     * Declaration order mirrors the six target HitWork frames. MWCC allocates
     * these locals downward, producing center1 at the high end of the stack
     * and left2 at the low end, as in the target.
     */
    HitWork center1;
    HitWork right1;
    HitWork left1;
    HitWork center2;
    HitWork right2;
    HitWork left2;

    /*
     * The target owns seven distinct scratch Vecs in addition to the HitWorks:
     * one center start Vec reused by both passes, one direction Vec per pass,
     * and four side-start Vecs. Together with the two sin/cos scalars these
     * account for the 0x70 local-frame gap in the retained body.
     */
    Vec3 startCenter;
    Vec3 dir1;
    Vec3 startRight1;
    Vec3 startLeft1;
    Vec3 dir2;
    Vec3 startRight2;
    Vec3 startLeft2;

    f32 sinOut;
    f32 cosOut;
    f32 radius;
    f32 sideX;
    f32 sideZ;
    f32 angle1;
    f32 angle2;
    f32 delta;
    void* hit1;
    void* hit2;

#define RUN_HIT(_work) \
    (yoshiGetStatus() != 0 \
         ? hitCheckVecFilter(&(_work), chkfilterYoshi) \
         : ((*(s8*)((u8*)party + 0x31) == 2 && (*(u32*)party & 0x100) != 0) \
                ? hitCheckVecFilter(&(_work), chkfilterNokotaro) \
                : hitCheckVecFilter(&(_work), chkfilter)))

    radius = *(f32*)((u8*)party + 0xF4);

    /*
     * Side offset used by both opposing probe passes.
     */
    sincosf(toMovedirSimple(float_270_80421500), &sinOut, &cosOut);
    sideX = float_0p5_80421504 * (sinOut * radius);
    sideZ = float_0p5_80421504 * (cosOut * radius);

    angle1 = float_0_80421508;
    angle2 = float_0_80421508;

    /*
     * Pass 1: forward direction. The target seeds dir1 from catalog entry 16,
     * then overwrites x/z with the sin/cos outputs.
     */
    sincosf(toMovedirSimple(float_180_8042150c), &sinOut, &cosOut);
    *(VecBits*)&dir1 = catalog[16];
    dir1.x = sinOut;
    dir1.z = cosOut;

    *(VecBits*)&startCenter = *(VecBits*)pos;
    startCenter.y = startCenter.y + float_10_80421510;
    center1.start = startCenter;
    center1.dir = dir1;
    center1.dist = float_20_80421514;

    hit1 = RUN_HIT(center1);
    if (hit1 != 0) {
        angle1 = angleABf(
            float_0_80421508,
            float_0_80421508,
            center1.normal.x,
            center1.normal.z);
    }

    if (hit1 == 0) {
        *(VecBits*)&startRight1 = catalog[17];
        startRight1.x = pos[0] + sideX;
        startRight1.y = pos[1] + float_10_80421510;
        startRight1.z = pos[2] + sideZ;
        right1.start = startRight1;
        right1.dir = dir1;
        right1.dist = float_20_80421514;

        hit1 = RUN_HIT(right1);
        if (hit1 != 0) {
            angle1 = angleABf(
                float_0_80421508,
                float_0_80421508,
                right1.normal.x,
                right1.normal.z);
        }
    }

    if (hit1 == 0) {
        *(VecBits*)&startLeft1 = catalog[18];
        startLeft1.x = pos[0] - sideX;
        startLeft1.y = pos[1] + float_10_80421510;
        startLeft1.z = pos[2] - sideZ;
        left1.start = startLeft1;
        left1.dir = dir1;
        left1.dist = float_20_80421514;

        hit1 = RUN_HIT(left1);
        if (hit1 != 0) {
            angle1 = angleABf(
                float_0_80421508,
                float_0_80421508,
                left1.normal.x,
                left1.normal.z);
        }
    }

    /*
     * Pass 2: opposite direction. Target reuses startCenter but has a distinct
     * direction scratch Vec and distinct side-start Vecs.
     */
    sincosf(toMovedirSimple(float_0_80421508), &sinOut, &cosOut);
    *(VecBits*)&dir2 = catalog[19];
    dir2.x = sinOut;
    dir2.z = cosOut;

    *(VecBits*)&startCenter = *(VecBits*)pos;
    startCenter.y = startCenter.y + float_10_80421510;
    center2.start = startCenter;
    center2.dir = dir2;
    center2.dist = float_20_80421514;

    hit2 = RUN_HIT(center2);
    if (hit2 != 0) {
        angle2 = angleABf(
            float_0_80421508,
            float_0_80421508,
            center2.normal.x,
            center2.normal.z);
    }

    if (hit2 == 0) {
        *(VecBits*)&startRight2 = catalog[20];
        startRight2.x = pos[0] + sideX;
        startRight2.y = pos[1] + float_10_80421510;
        startRight2.z = pos[2] + sideZ;
        right2.start = startRight2;
        right2.dir = dir2;
        right2.dist = float_20_80421514;

        hit2 = RUN_HIT(right2);
        if (hit2 != 0) {
            angle2 = angleABf(
                float_0_80421508,
                float_0_80421508,
                right2.normal.x,
                right2.normal.z);
        }
    }

    if (hit2 == 0) {
        *(VecBits*)&startLeft2 = catalog[21];
        startLeft2.x = pos[0] - sideX;
        startLeft2.y = pos[1] + float_10_80421510;
        startLeft2.z = pos[2] - sideZ;
        left2.start = startLeft2;
        left2.dir = dir2;
        left2.dist = float_20_80421514;

        hit2 = RUN_HIT(left2);
        if (hit2 != 0) {
            angle2 = angleABf(
                float_0_80421508,
                float_0_80421508,
                left2.normal.x,
                left2.normal.z);
        }
    }

#undef RUN_HIT

    if (hit1 == 0 || hit2 == 0) {
        return 0;
    }

    delta =
        -(float_0p5_80421504 * (angle1 + angle2) - float_180_8042150c);
    delta = __fabsf(delta);

    if (delta <= float_10_80421510) {
        return 1;
    }
    return 0;
}

const u32 vec3_802cb770[] = { 0x00000000, 0x00000000, 0x00000000 };
const u32 vec3_802cb77c[] = { 0x00000000, 0xBF800000, 0x00000000 };
const u32 vec3_802cb788[] = { 0x00000000, 0x00000000, 0x00000000 };
const u32 vec3_802cb794[] = { 0x00000000, 0xBF800000, 0x00000000 };
const u32 vec3_802cb7a0[] = { 0x00000000, 0x00000000, 0x00000000 };
const u32 vec3_802cb7ac[] = { 0x00000000, 0x00000000, 0x00000000 };
const u32 vec3_802cb7b8[] = { 0x00000000, 0x00000000, 0x00000000 };
const u32 vec3_802cb7c4[] = { 0x00000000, 0x00000000, 0x00000000 };
const u32 vec3_802cb7d0[] = { 0x00000000, 0x00000000, 0x00000000 };
const u32 vec3_802cb7dc[] = { 0x00000000, 0x00000000, 0x00000000 };
const u32 vec3_802cb7e8[] = { 0x00000000, 0x00000000, 0x00000000 };
const u32 vec3_802cb7f4[] = { 0x00000000, 0x00000000, 0x00000000 };
const u32 vec3_802cb800[] = { 0x00000000, 0x00000000, 0x00000000 };
const u32 vec3_802cb80c[] = { 0x00000000, 0x00000000, 0x00000000 };
const u32 vec3_802cb818[] = { 0x00000000, 0x00000000, 0x00000000 };
const u32 vec3_802cb824[] = { 0x00000000, 0x00000000, 0x00000000 };
const u32 vec3_802cb830[] = { 0x00000000, 0x00000000, 0x00000000 };
const u32 vec3_802cb83c[] = { 0x00000000, 0x00000000, 0x00000000 };
const u32 vec3_802cb848[] = { 0x00000000, 0x00000000, 0x00000000 };
const u32 vec3_802cb854[] = { 0x00000000, 0x00000000, 0x00000000 };
const u32 vec3_802cb860[] = { 0x00000000, 0x00000000, 0x00000000 };
const u32 vec3_802cb86c[] = { 0x00000000, 0x00000000, 0x00000000 };
const char str_gor_02_802cb898[] = "gor_02";
const char str_MOBJ_HiddenHatenaBlo_802cb8a0[] = "MOBJ_HiddenHatenaBlock";
const char str_MOBJ_HiddenBadgeBloc_802cb8b8[] = "MOBJ_HiddenBadgeBlock";
const char str_MOBJ_Hidden10CountBl_802cb8d0[] = "MOBJ_Hidden10CountBlock";

const f32 float_270_80421500 = 270.0f;
const f32 float_0p5_80421504 = 0.5f;
const f32 float_0_80421508 = 0.0f;
const f32 float_180_8042150c = 180.0f;
const f32 float_10_80421510 = 10.0f;
const f32 float_20_80421514 = 20.0f;
const f32 float_6_80421518 = 6.0f;
const f32 float_1_8042151c = 1.0f;
const f32 float_100_80421520 = 100.0f;
const f32 float_0p3_80421524 = 0.3f;
const f32 float_0p75_80421528 = 0.75f;
const f32 float_neg0p5_8042152c = -0.5f;
const f32 float_1000_80421530 = 1000.0f;
const f32 float_11_80421534 = 11.0f;
const f32 float_45_80421538 = 45.0f;
const f32 float_neg1_8042153c = -1.0f;
const f32 float_120_80421540 = 120.0f;
const f32 float_350_80421544 = 350.0f;
const f32 float_0p01_80421548 = 0.01f;
const f32 float_0p265_8042154c = 0.265f;
const f32 float_3p1416_80421550 = 3.1415927f;
const f32 float_2_80421554 = 2.0f;
const f32 float_0p1_80421558 = 0.1f;
const f32 float_5_8042155c = 5.0f;
const f32 float_0p375_80421560 = 0.375f;
const f32 float_neg10000_80421564 = -10000.0f;
const f32 float_50_80421568 = 50.0f;
const f32 float_30_8042156c = 30.0f;
