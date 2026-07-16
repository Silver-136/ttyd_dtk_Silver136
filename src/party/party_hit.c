#include "party/party_hit.h"

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


s32 partySearchGround(f64 rise, f64 fall, void* party) {
    typedef struct HitWork {
        u8 pad[0xC];
        f32 start[3];
        f32 dir[3];
        f32 hitPos[3];
        f32 normal[3];
        f32 dist;
    } HitWork;
    extern void* marioGetPtr(void);
    extern f64 partyGetHeight(void* party);
    extern f64 partyToMovedir(f64 direction, void* party);
    extern f64 sin(f64 x);
    extern f64 cos(f64 x);
    extern s32 yoshiGetStatus(void);
    extern void* hitCheckVecFilter(void* work, void* filter);
    extern s32 chkfilter(s32, s32);
    extern s32 chkfilterNokotaro(s32, s32);
    extern s32 chkfilterYoshi(s32, s32);
    extern const f32 float_0_80421508;
    extern const f32 float_0p1_80421558;
    extern const f32 float_0p375_80421560;
    extern const f32 float_0p75_80421528;
    extern const f32 float_3p1416_80421550;
    extern const f32 float_5_8042155c;
    extern const f32 float_11_80421534;
    extern const f32 float_180_8042150c;
    extern const f32 float_neg1_8042153c;
    extern const f32 float_neg10000_80421564;
    void* mario;
    f32* pos = (f32*)((s32)party + 0x58);
    f32 height;
    f32 angle;
    f32 sideX;
    f32 sideZ;
    f32 best = float_neg10000_80421564;
    void* bestHit = 0;
    s32 i;

    if ((*(u32*)party & 0x20000000) != 0) {
        return 0;
    }
    *(f32*)((s32)party + 0xC4) = float_0_80421508;
    *(f32*)((s32)party + 0xC8) = float_0_80421508;
    *(f32*)((s32)party + 0xCC) = float_0_80421508;
    *(f32*)((s32)party + 0xE0) = float_0_80421508;
    *(f32*)((s32)party + 0xE4) = pos[1];
    *(f32*)((s32)party + 0xE8) = float_neg1_8042153c;
    mario = marioGetPtr();
    height = *(u8*)((s32)mario + 0x2F) == 6 ? float_5_8042155c :
             float_0p75_80421528 * (f32)partyGetHeight(party);
    angle = *(f32*)((s32)party + 0x104) == float_0_80421508 ?
            (f32)partyToMovedir(*(f32*)((s32)party + 0x108), party) :
            *(f32*)((s32)party + 0x100);
    angle = float_3p1416_80421550 * angle / float_180_8042150c;
    sideX = float_0p375_80421560 * *(f32*)((s32)party + 0xF4) * (f32)sin(angle);
    sideZ = float_0p375_80421560 * *(f32*)((s32)party + 0xF4) * -(f32)cos(angle);
    for (i = 0; i < 5; i++) {
        HitWork work;
        f32 offX = 0.0f;
        f32 offZ = 0.0f;
        void* filter = chkfilter;
        void* hit;
        if (i == 1) { offX = sideX; offZ = sideZ; }
        if (i == 2) { offX = sideZ; offZ = -sideX; }
        if (i == 3) { offX = -sideZ; offZ = sideX; }
        if (i == 4) { offX = -sideX; offZ = -sideZ; }
        work.start[0] = pos[0] + offX;
        work.start[1] = pos[1] + height;
        work.start[2] = pos[2] + offZ;
        work.dir[0] = float_0_80421508;
        work.dir[1] = float_neg1_8042153c;
        work.dir[2] = float_0_80421508;
        work.dist = height + (f32)(rise < 0.0 ? -rise : rise) + float_0p1_80421558;
        if (yoshiGetStatus() != 0) {
            filter = chkfilterYoshi;
        } else if (*(s8*)((s32)party + 0x31) == 2 && (*(u32*)party & 0x100)) {
            filter = chkfilterNokotaro;
        }
        hit = hitCheckVecFilter(&work, filter);
        if (hit != 0) {
            f32 delta = work.hitPos[1] - pos[1];
            f32 limit = fall < 0.0 ? (f32)-fall : (f32)fall;
            if ((delta <= float_11_80421534 || delta < limit) && work.hitPos[1] > best) {
                best = work.hitPos[1];
                bestHit = hit;
                *(f32*)((s32)party + 0xC4) = work.normal[0];
                *(f32*)((s32)party + 0xC8) = work.normal[1];
                *(f32*)((s32)party + 0xCC) = work.normal[2];
            }
        }
    }
    if (bestHit != 0) {
        *(f32*)((s32)party + 0xE4) = best;
        *(f32*)((s32)party + 0xE8) = float_0_80421508;
    }
    return (s32)bestHit;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 partyChkFrontStep(f64 heightAdd, void* pParty, f32* outY, f32* outDiff, f32* outAngle) {
    typedef struct VecLocal {
        f32 x;
        f32 y;
        f32 z;
    } VecLocal;
    typedef struct HitWork {
        char pad0[0xC];
        VecLocal start;
        VecLocal dir;
        VecLocal hitPos;
        VecLocal normal;
        f32 dist;
    } HitWork;
    extern f32 partyGetHeight(void* party);
    extern f32 toMovedir(f32 dir);
    extern f32 revise360(f32 dir);
    extern f64 sin(f64 x);
    extern f64 cos(f64 x);
    extern s32 yoshiGetStatus(void);
    extern s32 hitCheckVecFilter(void* work, void* filter);
    extern s32 chkfilterYoshi(s32 a, s32 b);
    extern s32 chkfilterNokotaro(s32 a, s32 b);
    extern s32 chkfilter(s32 a, s32 b);
    extern f32 angleABf(f32 ax, f32 az, f32 bx, f32 bz);
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
    extern u32 vec3_802cb788[];
    extern u32 vec3_802cb7a0[];

    HitWork work;
    f32 partyHeight;
    f32 direction;
    f32 radians;
    f32 sinDir;
    f32 cosDir;
    f32 widthOffset;
    f32 partyX;
    f32 partyY;
    f32 partyZ;
    f32 roundedDist;
    f32 roundedX;
    f32 roundedY;
    f32 roundedZ;
    f32 bias;
    f32 normalXZ;
    f32 yDiffAbs;
    s32 hit;

    *(u32*)((s32)pParty + 0xC4) = vec3_802cb788[0];
    *(u32*)((s32)pParty + 0xC8) = vec3_802cb788[1];
    *(u32*)((s32)pParty + 0xCC) = vec3_802cb788[2];
    *(f32*)((s32)pParty + 0xE0) = float_0_80421508;

    partyHeight = partyGetHeight(pParty);
    partyX = *(f32*)((s32)pParty + 0x58);
    partyY = *(f32*)((s32)pParty + 0x5C);
    partyZ = *(f32*)((s32)pParty + 0x60);
    *outY = partyY;
    *outDiff = float_neg1_8042153c;

    if (*(f32*)((s32)pParty + 0x104) == float_0_80421508) {
        direction = toMovedir(*(f32*)((s32)pParty + 0x10C));
    } else {
        direction = *(f32*)((s32)pParty + 0x100);
    }
    direction = revise360((f32)(s32)(direction + float_0p5_80421504));
    radians = (float_3p1416_80421550 * direction) / float_180_8042150c;
    sinDir = (f32)sin(radians);
    cosDir = (f32)cos(radians);
    widthOffset = float_0p265_8042154c * *(f32*)((s32)pParty + 0xF4);

    work.start.x = partyX + widthOffset * sinDir;
    work.start.y = partyY + partyHeight;
    work.start.z = partyZ + widthOffset * -cosDir;
    work.dir.x = ((f32*)vec3_802cb7a0)[0];
    work.dir.y = ((f32*)vec3_802cb7a0)[1];
    work.dir.z = ((f32*)vec3_802cb7a0)[2];
    work.dist = partyHeight + ((f32)heightAdd + float_0p01_80421548);

    if (yoshiGetStatus() != 0) {
        hit = hitCheckVecFilter(&work, chkfilterYoshi);
    } else if (*(s8*)((s32)pParty + 0x31) == 2 && (*(u32*)pParty & 0x100) != 0) {
        hit = hitCheckVecFilter(&work, chkfilterNokotaro);
    } else {
        hit = hitCheckVecFilter(&work, chkfilter);
    }

    if (hit == 0) {
        return 0;
    }

    bias = float_neg0p5_8042152c;
    if (work.dist >= float_0_80421508) {
        bias = float_0p5_80421504;
    }
    roundedDist = (f32)(s32)(work.dist * float_1000_80421530 + bias) / float_1000_80421530;

    bias = float_neg0p5_8042152c;
    if (work.hitPos.x >= float_0_80421508) {
        bias = float_0p5_80421504;
    }
    roundedX = (f32)(s32)(work.hitPos.x * float_1000_80421530 + bias) / float_1000_80421530;

    bias = float_neg0p5_8042152c;
    if (work.hitPos.y >= float_0_80421508) {
        bias = float_0p5_80421504;
    }
    roundedY = (f32)(s32)(work.hitPos.y * float_1000_80421530 + bias) / float_1000_80421530;

    bias = float_neg0p5_8042152c;
    if (work.hitPos.z >= float_0_80421508) {
        bias = float_0p5_80421504;
    }
    roundedZ = (f32)(s32)(work.hitPos.z * float_1000_80421530 + bias) / float_1000_80421530;

    *(f32*)((s32)pParty + 0xC4) =
        (f32)(s32)(angleABf(float_0_80421508, float_0_80421508,
                            float_100_80421520 * work.normal.z,
                            float_100_80421520 * work.normal.y) - float_180_8042150c);
    *(f32*)((s32)pParty + 0xC8) = float_0_80421508;
    *(f32*)((s32)pParty + 0xCC) =
        (f32)(s32)(angleABf(float_0_80421508, float_0_80421508,
                            float_100_80421520 * work.normal.x,
                            float_100_80421520 * work.normal.y) - float_180_8042150c);
    normalXZ = work.normal.x * work.normal.x + work.normal.z * work.normal.z;
    *(f32*)((s32)pParty + 0xE0) = angleABf(float_0_80421508, float_0_80421508, normalXZ, -work.normal.y);

    yDiffAbs = roundedY - partyY;
    if (yDiffAbs < float_0_80421508) {
        yDiffAbs = -yDiffAbs;
    }
    if (roundedY < partyY && yDiffAbs < float_2_80421554) {
        *outDiff = partyY - roundedY;
        *outAngle = angleABf(partyX, partyZ, roundedX, roundedZ);
        return 0;
    }

    *outY = roundedY;
    *outDiff = float_0_80421508;
    *outAngle = angleABf(partyX, partyZ, work.hitPos.x, work.hitPos.z);
    return hit;
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 partySearchFrontWall(f64 distance, f64 angle, void* pParty, f32* pos) {
    typedef struct VecLocal {
        f32 x;
        f32 y;
        f32 z;
    } VecLocal;
    typedef struct HitWork {
        VecLocal start;
        VecLocal dir;
        VecLocal hitPos;
        VecLocal normal;
        f32 dist;
    } HitWork;
    extern void sincosf(f32 angle, f32* cosOut, f32* sinOut);
    extern f32 partyGetHeight(void* party);
    extern s32 yoshiGetStatus(void);
    extern s32 hitCheckVecFilter(void* work, void* filter);
    extern s32 chkfilterYoshi(s32 a, s32 b);
    extern s32 chkfilterNokotaro(s32 a, s32 b);
    extern s32 chkfilter(s32 a, s32 b);
    extern f32 angleABf(f32 ax, f32 az, f32 bx, f32 bz);
    extern f32 float_0_80421508;
    extern f32 float_0p5_80421504;
    extern f32 float_0p75_80421528;
    extern f32 float_1_8042151c;
    extern f32 float_6_80421518;
    extern f32 float_11_80421534;
    extern f32 float_1000_80421530;
    extern f32 float_neg0p5_8042152c;
    extern f32 float_45_80421538;
    extern f32 float_neg1_8042153c;

    HitWork work;
    f32 s;
    f32 c;
    f32 radius;
    f32 dist;
    f32 bias;
    f32 dot;
    s32 hit;

    sincosf((f32)angle, &c, &s);
    radius = *(f32*)((s32)pParty + 0xF4);
    if ((*(u32*)pParty & 0x100) != 0) {
        radius = float_6_80421518;
    }
    radius = (f32)(float_0p5_80421504 * radius + distance);
    dist = radius;

    work.start.x = *(f32*)((s32)pParty + 0x58);
    work.start.y = *(f32*)((s32)pParty + 0x5C) + float_0p75_80421528 * partyGetHeight(pParty);
    work.start.z = *(f32*)((s32)pParty + 0x60);
    work.dir.x = c;
    work.dir.y = float_0_80421508;
    work.dir.z = s;
    work.dist = radius;
    if (yoshiGetStatus() != 0) {
        hit = hitCheckVecFilter(&work, chkfilterYoshi);
    } else if (*(s8*)((s32)pParty + 0x31) == 2 && (*(u32*)pParty & 0x100) != 0) {
        hit = hitCheckVecFilter(&work, chkfilterNokotaro);
    } else {
        hit = hitCheckVecFilter(&work, chkfilter);
    }
    if (hit != 0) {
        bias = float_neg0p5_8042152c;
        if (work.dist >= float_0_80421508) {
            bias = float_0p5_80421504;
        }
        dist = (f32)(s32)(float_1000_80421530 * work.dist + bias) / float_1000_80421530;
    }

    if (hit == 0) {
        work.start.x = *(f32*)((s32)pParty + 0x58);
        work.start.y = *(f32*)((s32)pParty + 0x5C) + float_11_80421534;
        if (*(s8*)((s32)pParty + 0x31) == 3 && (*(u32*)pParty & 0x100) != 0) {
            work.start.y = *(f32*)((s32)pParty + 0x5C) + float_1_8042151c;
        }
        work.start.z = *(f32*)((s32)pParty + 0x60);
        work.dir.x = c;
        work.dir.y = float_0_80421508;
        work.dir.z = s;
        work.dist = radius;
        if (yoshiGetStatus() != 0) {
            hit = hitCheckVecFilter(&work, chkfilterYoshi);
        } else if (*(s8*)((s32)pParty + 0x31) == 2 && (*(u32*)pParty & 0x100) != 0) {
            hit = hitCheckVecFilter(&work, chkfilterNokotaro);
        } else {
            hit = hitCheckVecFilter(&work, chkfilter);
        }
        if (hit != 0) {
            bias = float_neg0p5_8042152c;
            if (work.dist >= float_0_80421508) {
                bias = float_0p5_80421504;
            }
            dist = (f32)(s32)(float_1000_80421530 * work.dist + bias) / float_1000_80421530;
            if (angleABf(float_0_80421508, float_0_80421508, work.normal.x * work.normal.x + work.normal.z * work.normal.z, -work.normal.y) >= float_45_80421538) {
                *(f32*)((s32)pParty + 0xEC) = dist;
                dot = ((f32)distance * c) * work.normal.x + ((f32)distance * s) * work.normal.z;
                pos[0] = float_0p5_80421504 * -(dot * work.normal.x - ((f32)distance * c)) + pos[0];
                pos[2] = float_0p5_80421504 * -(dot * work.normal.z - ((f32)distance * s)) + pos[2];
                return hit;
            }
        }
        hit = 0;
        *(f32*)((s32)pParty + 0xEC) = float_neg1_8042153c;
        pos[0] += (f32)distance * c;
        pos[2] += (f32)distance * s;
    } else {
        *(f32*)((s32)pParty + 0xEC) = dist;
        dot = ((f32)distance * c) * work.normal.x + ((f32)distance * s) * work.normal.z;
        pos[0] = float_0p5_80421504 * -(dot * work.normal.x - ((f32)distance * c)) + pos[0];
        pos[2] = float_0p5_80421504 * -(dot * work.normal.z - ((f32)distance * s)) + pos[2];
    }

    return hit;
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void partySearchHead(f64 angle, void* pParty, f32* pos, f32* dist) {
    extern void sincosf(f32 angle, f32* cosOut, f32* sinOut);
    extern s32 hitCheckFilter(f64 x, f64 y, f64 z, f64 vx, f64 vy, f64 vz, s32 flags,
                              void* out0, void* out1, void* out2, f32* outDist,
                              void* out4, void* out5, void* out6);
    extern f32 float_0_80421508;
    extern f32 float_10_80421510;
    extern f32 float_1_8042151c;
    extern f32 float_0p3_80421524;

    f32 out0[3];
    f32 out1[3];
    f32 out2[3];
    f32 out4[3];
    f32 out5[3];
    f32 out6[3];
    f32 x;
    f32 y;
    f32 z;
    f32 best;
    f32 width;
    f32 s;
    f32 c;
    f32 dx;
    f32 dz;
    s32 hit;

    width = float_0p3_80421524 * *(f32*)((s32)pParty + 0xF4);
    sincosf((f32)angle, &c, &s);
    x = pos[0];
    y = pos[1];
    z = pos[2];
    best = *dist;
    dx = width * c;
    dz = -width * s;

    pos[0] = x + dx;
    pos[1] = y;
    pos[2] = z + dz;
    hit = hitCheckFilter(pos[0], y + float_10_80421510, pos[2],
                         float_0_80421508, float_1_8042151c, float_0_80421508,
                         0, out0, out1, out2, dist, out4, out5, out6);
    if (hit != 0 && *dist <= best) {
        best = *dist;
    }
    if (hit == 0) {
        *dist = best;
        pos[0] = x - dx;
        pos[1] = y;
        pos[2] = z - dz;
        hit = hitCheckFilter(pos[0], y + float_10_80421510, pos[2],
                             float_0_80421508, float_1_8042151c, float_0_80421508,
                             0, out0, out1, out2, dist, out4, out5, out6);
        if (hit != 0 && *dist <= best) {
            best = *dist;
        }
        if (hit == 0) {
            *dist = best;
            pos[0] = x + dz;
            pos[1] = y;
            pos[2] = z + dx;
            hit = hitCheckFilter(pos[0], y + float_10_80421510, pos[2],
                                 float_0_80421508, float_1_8042151c, float_0_80421508,
                                 0, out0, out1, out2, dist, out4, out5, out6);
            if (hit != 0 && *dist <= best) {
                best = *dist;
            }
            if (hit == 0) {
                *dist = best;
                pos[0] = x - dz;
                pos[1] = y;
                pos[2] = z - dx;
                hit = hitCheckFilter(pos[0], y + float_10_80421510, pos[2],
                                     float_0_80421508, float_1_8042151c, float_0_80421508,
                                     0, out0, out1, out2, dist, out4, out5, out6);
                if (hit != 0 && *dist <= best) {
                    best = *dist;
                }
            }
        }
    }

    if (hit == 0) {
        *dist = float_0_80421508;
    } else {
        *dist = best;
    }
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
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

u8 partyChkGnd(void* pParty) {
    extern s32 partySearchGround(f64 rise, f64 fall, void* party);
    extern u32 hitGetAttr(s32 hit);
    extern void partyChgMot(void* party, s32 motion);
    extern s32 partyChkFrontStep(f64 height, void* party, f32* outY, f32* outDiff, f32* outAngle);
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

u8 partyNrmToAngle(float* normal) {
    extern f64 __frsqrte(f64 value);
    extern f32 angleABf(f32 ax, f32 az, f32 bx, f32 bz);
    extern f32 float_0_80421508;
    extern f32 __float_nan;
    union FloatBits {
        f32 value;
        u32 bits;
    } view;
    f32 square;
    f32 length;
    f64 value;
    f64 inv;
    u32 exponent;
    s32 kind;

    square = normal[0] * normal[0] + normal[2] * normal[2];
    value = (f64)square;
    if (value > (f64)float_0_80421508) {
        inv = __frsqrte(value);
        inv = 0.5 * inv * -((value * inv * inv) - 3.0);
        inv = 0.5 * inv * -((value * inv * inv) - 3.0);
        length = (f32)(value * 0.5 * inv * -((value * inv * inv) - 3.0));
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
    angleABf(float_0_80421508, float_0_80421508, length, -normal[1]);
}

s32 chkfilterYoshi(s32 param_1, int param_2) {
    typedef struct MobjName {
        u8 pad[0x15];
        char name[16];
    } MobjName;
    extern void* marioGetPtr(void);
    extern MobjName* mobjHitObjPtrToPtr(int hit);
    extern s32 strcmp(char* a, char* b);
    extern char str_MOBJ_HiddenHatenaBlo_802cb8a0[];
    extern char str_MOBJ_HiddenBadgeBloc_802cb8b8[];
    extern char str_MOBJ_Hidden10CountBl_802cb8d0[];
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

s32 chkfilterNokotaro(s32 param_1, int param_2) {
    typedef struct MobjName {
        u8 pad[0x15];
        char name[16];
    } MobjName;
    extern void* marioGetPtr(void);
    extern MobjName* mobjHitObjPtrToPtr(int hit);
    extern s32 strcmp(char* a, char* b);
    extern char str_MOBJ_HiddenHatenaBlo_802cb8a0[];
    extern char str_MOBJ_HiddenBadgeBloc_802cb8b8[];
    extern char str_MOBJ_Hidden10CountBl_802cb8d0[];
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

s32 chkfilter(s32 param_1, int param_2) {
    typedef struct MobjName {
        u8 pad[0x15];
        char name[16];
    } MobjName;
    extern void* marioGetPtr(void);
    extern MobjName* mobjHitObjPtrToPtr(int hit);
    extern s32 strcmp(char* a, char* b);
    extern char str_MOBJ_HiddenHatenaBlo_802cb8a0[];
    extern char str_MOBJ_HiddenBadgeBloc_802cb8b8[];
    extern char str_MOBJ_Hidden10CountBl_802cb8d0[];
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

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* partySearchWallFront(f64 distance, f64 direction, void* pParty, f32* pos) {
    typedef struct VecLocal {
        f32 x;
        f32 y;
        f32 z;
    } VecLocal;
    extern void sincosf(f32 angle, f32* cosOut, f32* sinOut);
    extern f32 partyGetHeight(void* party);
    extern s32 yoshiGetStatus(void);
    extern s32 strcmp(const char* a, const char* b);
    extern void* marioHitCheckVec(void* pos, void* dir, void* outNorm, void* outHitPos, f32* dist);
    extern void* gp;
    extern char str_gor_02_802cb898[];
    extern f32 float_0p5_80421504;
    extern f32 float_0p75_80421528;
    extern f32 float_1_8042151c;
    extern f32 float_11_80421534;
    extern f32 float_45_80421538;
    extern f32 float_120_80421540;
    extern f32 float_350_80421544;

    f32 heights[4];
    VecLocal dir;
    VecLocal sideDir;
    VecLocal posWork;
    VecLocal normal;
    VecLocal bestNormal;
    VecLocal hitPos;
    VecLocal push;
    void* hit;
    void* bestHit;
    f32 width;
    f32 bestDist;
    f32 dist;
    f32 dot;
    s32 count;
    s32 i;
    s32 blocked;

    push.x = 0.0f;
    push.y = 0.0f;
    push.z = 0.0f;
    sincosf(direction, &dir.x, &dir.z);
    dir.y = 0.0f;
    count = 3;
    heights[0] = float_0p75_80421528 * partyGetHeight(pParty);
    heights[1] = float_0p5_80421504 * partyGetHeight(pParty);
    if (((*(u32*)pParty & 0x60) == 0) && yoshiGetStatus() != 2) {
        heights[2] = float_11_80421534;
        if (strcmp(*(char**)((s32)gp + 0), str_gor_02_802cb898) == 0 &&
            *(f32*)((s32)pParty + 0x5C) >= float_120_80421540 &&
            *(f32*)((s32)pParty + 0x58) <= float_350_80421544) {
            count = 4;
            heights[3] = float_1_8042151c;
        }
    } else {
        heights[2] = float_1_8042151c;
    }

    width = float_0p5_80421504 * *(f32*)((s32)pParty + 0xF4);
    bestDist = width + (f32)distance;
    bestHit = 0;
    for (i = 0; i < count; i++) {
        posWork.x = pos[0];
        posWork.y = pos[1] + heights[i];
        posWork.z = pos[2];
        dist = width + (f32)distance;
        hit = marioHitCheckVec(&posWork, &dir, &normal, &hitPos, &dist);
        if (hit != 0) {
            if (bestHit == 0 || dist < bestDist) {
                bestDist = dist;
                bestNormal = normal;
                bestHit = hit;
            }
        }
    }
    if (bestHit != 0) {
        dot = ((f32)distance * dir.x) * bestNormal.x + ((f32)distance * dir.z) * bestNormal.z;
        push.x = float_0p5_80421504 * -(dot * bestNormal.x - ((f32)distance * dir.x));
        push.z = float_0p5_80421504 * -(dot * bestNormal.z - ((f32)distance * dir.z));
    }

    blocked = 0;
    sincosf(direction + float_45_80421538, &sideDir.x, &sideDir.z);
    sideDir.y = 0.0f;
    for (i = 0; i < count; i++) {
        posWork.x = pos[0];
        posWork.y = pos[1] + heights[i];
        posWork.z = pos[2];
        dist = width;
        if (marioHitCheckVec(&posWork, &sideDir, 0, 0, &dist) != 0) {
            blocked = 1;
            break;
        }
    }
    if (blocked != 0) {
        sincosf(direction - float_45_80421538, &sideDir.x, &sideDir.z);
        sideDir.y = 0.0f;
        blocked = 0;
        for (i = 0; i < count; i++) {
            posWork.x = pos[0];
            posWork.y = pos[1] + heights[i];
            posWork.z = pos[2];
            dist = width;
            if (marioHitCheckVec(&posWork, &sideDir, 0, 0, &dist) != 0) {
                blocked = 1;
            }
        }
    }

    if (bestHit == 0) {
        if (blocked == 0) {
            pos[0] += (f32)distance * dir.x;
            pos[2] += (f32)distance * dir.z;
        }
        return 0;
    }
    if (blocked == 0) {
        pos[0] += push.x;
        pos[2] += push.z;
    }
    return bestHit;
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

/* CHATGPT FALLBACK MISSING STUBS: main/party/party_hit 20260624_191429 */

/* fallback stub-fill: map=unk_800c2010 addr=0x800c2010 size=0x000007b0 */
s32 unk_800c2010(void* party, f32* pos) {
    typedef struct HitWork {
        u8 pad[0xC];
        f32 start[3];
        f32 dir[3];
        f32 hitPos[3];
        f32 normal[3];
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
    extern const f32 float_0_80421508;
    extern const f32 float_0p5_80421504;
    extern const f32 float_10_80421510;
    extern const f32 float_20_80421514;
    extern const f32 float_180_8042150c;
    extern const f32 float_270_80421500;
    f32 width = *(f32*)((s32)party + 0xF4);
    f32 sideSin;
    f32 sideCos;
    f32 angles[2];
    s32 found[2];
    s32 pass;

    sincosf((f32)toMovedirSimple(float_270_80421500), &sideSin, &sideCos);
    for (pass = 0; pass < 2; pass++) {
        f32 dirSin;
        f32 dirCos;
        s32 side;
        found[pass] = 0;
        angles[pass] = float_0_80421508;
        sincosf((f32)toMovedirSimple(pass == 0 ? float_180_8042150c : float_0_80421508),
                 &dirSin, &dirCos);
        for (side = 0; side < 3; side++) {
            HitWork work;
            f32 offset;
            void* filter;
            void* hit;
            if (side == 0) {
                offset = float_0_80421508;
            } else if (side == 1) {
                offset = float_0p5_80421504;
            } else {
                offset = -float_0p5_80421504;
            }
            work.start[0] = pos[0] + offset * sideSin * width;
            work.start[1] = pos[1] + float_10_80421510;
            work.start[2] = pos[2] + offset * sideCos * width;
            work.dir[0] = dirSin;
            work.dir[1] = float_0_80421508;
            work.dir[2] = dirCos;
            work.dist = float_20_80421514;
            if (yoshiGetStatus() != 0) {
                filter = chkfilterYoshi;
            } else if (*(s8*)((s32)party + 0x31) == 2 && (*(u32*)party & 0x100) != 0) {
                filter = chkfilterNokotaro;
            } else {
                filter = chkfilter;
            }
            hit = hitCheckVecFilter(&work, filter);
            if (hit != 0) {
                found[pass] = 1;
                angles[pass] = (f32)angleABf(float_0_80421508, float_0_80421508,
                                              work.normal[0], work.normal[2]);
                break;
            }
        }
    }
    if (found[0] == 0 || found[1] == 0) {
        return 0;
    }
    {
        f32 delta = -(float_0p5_80421504 * (angles[0] + angles[1]) - float_180_8042150c);
        if (delta < 0.0f) {
            delta = -delta;
        }
        return delta <= float_10_80421510;
    }
}

