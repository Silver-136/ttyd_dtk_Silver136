#include "eff/eff_gonbaba_breath.h"
#include "driver/camdrv.h"
#include "driver/hitdrv.h"
#include "driver/mobjdrv.h"
#include "mario/mario.h"
#include "koopa.h"
#include "mario/mario_motion.h"
#include "system.h"
#include <dolphin/mtx.h>
#include <dolphin/types.h>
#include <string.h>
#include "mario/mario_sbr.h"
#include "koopa.h"
#include "libc_decls.h"
#include "eff/eff_magic1_n64.h"
#include "koopa_hit.h"
#include <math.h>
s32 kpaEnemyFireHitChk(f32 x, f32 y, f32 z, f32 h, f32 w);
BOOL kpaFlameHitFilter(HitCheckQuery* query, HitEntry* entry);




#define KPW (marioGetPtr()->kpaWork)
#define KHEAD(i) (KPW->headHit[i])
#define KHEADD(i) (KPW->headDist[i])
#define KRIDE(i) (KPW->rideHit[i])
#define KRIDED(i) (KPW->rideDist[i])

static inline f32 kpa_nrmToAngle(f32 x, f32 y, f32 z) {
    return angleABf(0.0f, 0.0f, sqrtf(x * x + z * z), -y);
}

static inline f32 kpa_nrmSet(MarioWork* m, f32 x, f32 y, f32 z) {
    m->groundRot.x = (s32)(angleABf(0.0f, 0.0f, 100.0f * z, 100.0f * y) - 180.0f);
    m->groundRot.y = 0.0f;
    m->groundRot.z = (s32)(angleABf(0.0f, 0.0f, 100.0f * x, 100.0f * y) - 180.0f);
    return kpa_nrmToAngle(x, y, z);
}

s32 kpaEnemyFireHitChk(f32 x, f32 y, f32 z, f32 h, f32 w) {
    Mtx m3;
    Mtx m1;
    Mtx m2;
    Mtx m4;
    Vec out;
    Vec b;
    Vec diff;
    Vec a;
    EffectEntry* efp;
    EffGonbabaBreathWork* e;
    f32 half;
    f32 r;
    f32 s;
    s32 i;
    efp = getKoopaFireEfp();
    camGetPtr(CAMERA_3D);
    {
        Vec zero0 = {0.0f, 0.0f, 0.0f};
        a = zero0;
    }
    a.x = x;
    a.y = 0.5f * h + y;
    a.z = z;
    b = a;
    if (!efp) {
        return 0;
    }
    e = (EffGonbabaBreathWork*)efp->userdata;
    s = e->scale;
    PSMTXTrans(m1, e->position.x, e->position.y, e->position.z);
    PSMTXRotRad(m2, 'y', 0.017453292f * (e->yaw - camGetPtr(CAMERA_3D)->viewYaw));
    if (e->yaw > 90.0f && e->yaw < 270.0f) {
        PSMTXScale(m3, e->scale, e->scale, -e->scale);
    } else {
        PSMTXScale(m3, e->scale, e->scale, e->scale);
    }
    PSMTXConcat(m1, m2, m1);
    PSMTXConcat(m1, m3, m4);
    half = 0.5f * w;
    e++;
    for (i = 1; i < efp->count; i++, e++) {
        if (e->delay == 0) {
            r = 7.0f * e->scale;
            PSMTXTrans(m1, e->position.x, e->position.y, e->position.z);
            PSMTXScale(m3, r, r, r);
            PSMTXConcat(m1, m3, m1);
            PSMTXConcat(m4, m1, m1);
            {
                Vec zero = {0.0f, 0.0f, 0.0f};
                PSMTXMultVec(m1, &zero, &out);
            }
            PSVECSubtract(&b, &out, &diff);
            if (PSVECMag(&diff) < r * s + half) {
                return 1;
            }
        }
    }
    return 0;
}

s32 kpaEnemyHitChk(f32 x, f32 y, f32 z, f32 h, f32 w) {
    MarioWork* m;
    f32 dh, dw, dx, dy;

    m = marioGetPtr();

    if (kpaEnemyFireHitChk(x, y, z, h, w)) {
        return 2;
    }
    // FAKE: no-op self-assignment; fixes the scheduling of the position.y load
    m->wPlayerCollisionBox.y = m->wPlayerCollisionBox.y;
    dh = 0.5f * (m->wPlayerCollisionBox.y + h);
    dw = 0.5f * (m->wPlayerCollisionBox.x + w);
    dx = __fabsf(m->position.x - x);
    dy = __fabsf(m->position.y + 0.5f * m->wPlayerCollisionBox.y - y - 0.5f * h);
    if (dy <= dh && dx <= dw) {
        if ((m->flags & MARIO_FLAG_IS_FALLING) && m->position.y > y) {
            if (kpaGetLevel() == 0 && kpaGetStageType() != 1 && m->mutekiTimer == 0 && z < 50.0f) {
                marioChgMot(MARIO_MOTION_JUMP_NPC);
            }
            return 1;
        }
        if (kpaMutekiCheck() == 1) {
            return 3;
        }
        if (m->mutekiTimer != 0) {
            return 3;
        }
        if (m->kpaNoHitTimer == 0) {
            return 4;
        }
    }
    return 0;
}

static inline void kpa_headClear(void) {
    s32 i;
    for (i = 0; i < 10; i++) {
        KHEAD(i) = NULL;
        KHEADD(i) = 1000.0f;
    }
}

HitEntry* kpaSearchGround(f32 range, f32* outY, f32* outDepth, f32* outAng) {
    s32 n, i;
    MarioWork* m;
    MarioWork* m2;
    HitEntry* hit;
    HitEntry* best;
    Vec zero = {0.0f, 0.0f, 0.0f};
    f32 hx, hy, hz, dist, nx, ny, nz, nv;
    f32 h, deg, ex, ez, fx, fz, ang;
    f32 rad, r2b, r3, bd, yh, r2, px, py, pz, ox, oz;

    m = marioGetPtr();
    best = NULL;
    m->groundRot.x = 0.0f;
    m->groundRot.y = 0.0f;
    m->groundRot.z = 0.0f;
    m->groundSlope = 0.0f;
    if (m->flags & MARIO_FLAG_FORCE_WALL) {
        *outY = m->position.y;
        return NULL;
    }
    range += 0.01f;
    if (kpaGetLevel() == 0) {
        h = m->wPlayerCollisionBox.y;
    } else {
        h = 0.25f * m->wPlayerCollisionBox.y;
    }
    *outY = m->position.y;
    *outDepth = -1.0f;
    if (m->baseSpeed) {
        deg = m->directionView;
    } else {
        deg = toMovedir(m->displayDirection);
    }
    px = m->position.x;
    py = m->position.y;
    pz = m->position.z;
    for (i = 0; i < 10; i++) {
        KRIDE(i) = NULL;
        KRIDED(i) = 1000.0f;
    }
    switch (kpaGetLevel()) {
    case 0:
    case 1:
    case 2:
        n = 3;
        r2 = 0.6f * (0.5 * m->wPlayerCollisionBox.x);
        break;
    case 3:
        n = 7;
        r2 = 0.6f * (0.5f * m->wPlayerCollisionBox.x / 3.0f);
        break;
    }
    r2b = 2.0f * r2 - 0.05f;
    rad = 3.1415927f * deg;
    bd = h + range;
    yh = py + h;
    r3 = 3.0f * r2 - 0.05f;
    for (i = 0; i < n; i++) {
        switch (i) {
        case 0:
            ox = 0.0f;
            oz = ox;
            break;
        case 1:
            ang = rad / 180.0f;
            ox = (r2 - 0.05f) * (f32)sin(ang);
            nv = (f32)cos(ang); oz = (r2 - 0.05f) * -nv;
            break;
        case 2:
            ang = rad / 180.0f;
            nv = (f32)sin(ang); ox = (r2 - 0.05f) * -nv;
            nv = (f32)cos(ang); oz = (r2 - 0.05f) * -nv;
            break;
        case 3:
            ang = rad / 180.0f;
            ox = r2b * (f32)sin(ang);
            oz = r2b * -(f32)cos(ang);
            break;
        case 4:
            ang = rad / 180.0f;
            ox = r2b * -(f32)sin(ang);
            oz = r2b * -(f32)cos(ang);
            break;
        case 5:
            ang = rad / 180.0f;
            ox = r3 * (f32)sin(ang);
            oz = r3 * -(f32)cos(ang);
            break;
        case 6:
            ang = rad / 180.0f;
            ox = r3 * -(f32)sin(ang);
            oz = r3 * -(f32)cos(ang);
            break;
        }
        dist = bd;
        hit = hitCheckFilter(NULL, &hx, &hy, &hz, &dist, &nx, &ny, &nz, px + ox, yh, pz + oz, 0.0f, -1.0f, 0.0f);
        if (hit) {
            if (kpa_nrmSet(marioGetPtr(), nx, ny, nz) >= 50.0f) {
                hit = NULL;
            }
        }
        hy = (s32)(100.0f * hy + 0.5f) / 100.0f;
        if (hit) {
            m->groundSlope = kpa_nrmSet(marioGetPtr(), nx, ny, nz);
            if (hy < py && __fabsf(hy - py) >= 2.0f) {
                *outDepth = py - hy;
                *outAng = angleABf(px, pz, hx, hz);
                hit = NULL;
            } else {
                if (!nx && !nz) {
                    hy = (s32)(100.0f * hy + 0.5f) / 100.0f;
                }
                *outY = hy;
                *outDepth = 0.0f;
                *outAng = angleABf(px, pz, hx, hz);
                best = hit;
            }
        }
        KRIDE(i) = hit;
        KRIDED(i) = ox;
    }
    return best;
}

HitEntry* kpaSearchHead(f32 hgt, f32* outA, f32* outB) {
    static s32 _ofs0[] = {3, 12, 16, 1, 0, 16, 0, -9, 16, 0};
    static s32 _ofs1[] = {4, 25, 74, 0, 50, 65, 1, 0, 67, 0, -25, 62, 0};
    static s32 _ofs2[] = {6, 50, 102, 0, 76, 90, 1, 25, 100, 0, 0, 94, 0, -25, 90, 0, -45, 68, 0};
    static s32 _ofs3[] = {13, 170, 194, 1, 145, 204, 0, 120, 232, 0, 95, 227, 0, 70, 232, 0, 55, 232, 0,
                          30, 194, 0, 5, 187, 0, -20, 210, 0, -45, 201, 0, -70, 181, 0, -95, 153, 0, -120, 56, 0};
    static s32* headchkofs[] = {_ofs0, _ofs1, _ofs2, _ofs3};
    s32 i, j, n;
    s32* tbl;
    HitEntry* hit;
    HitEntry* best;
    MarioWork* m;
    f32 deg, rad, off, c, s_, x, y, z, hx, hy, hz, nx, ny, nz, d;
    f32 oy, oz, mode;
    s32 md;

    m = marioGetPtr();
    if (m->flags & MARIO_FLAG_IS_FALLING) {
        return NULL;
    }
    best = NULL;
    if (m->baseSpeed) {
        deg = m->directionView;
    } else {
        deg = toMovedir(m->displayDirection);
    }
    off = *outB;
    *outB = -1.0f;
    kpa_headClear();
    tbl = headchkofs[kpaGetLevel()];
    n = *tbl++;
    if (!(m->flags & MARIO_FLAG_IS_JUMPING)) {
        switch (kpaGetLevel()) {
        case 0:
            off = 2.0f;
            break;
        case 1:
            off = 0.0f;
            break;
        case 2:
            off = 0.0f;
            break;
        case 3:
            off = 35.0f;
            break;
        }
    } else {
        switch (kpaGetLevel()) {
        case 0:
            if (m->currMotionId == MARIO_MOTION_KPA_SWIM) {
                off = 10.0f;
            } else {
                off = hgt;
            }
            break;
        default:
            off = hgt;
            break;
        }
    }
    rad = 3.1415927f * deg / 180.0f;
    for (i = 0; i < n; i++) {
        s_ = tbl[0] * (f32)sin(rad);
        c = (f32)cos(rad);
        oz = tbl[0] * -c;
        oy = tbl[1];
        md = tbl[2];
        tbl += 3;
        if (md == 0 && !(m->flags & MARIO_FLAG_IS_JUMPING)) {
            continue;
        }
        x = m->position.x + s_;
        y = m->position.y + oy;
        z = m->position.z + oz;
        d = off;
        hit = hitCheckFilter(NULL, &hx, &hy, &hz, &d, &nx, &ny, &nz, x, y, z, 0.0f, 1.0f, 0.0f);
        if (hit) {
            hy = (s32)(100.0f * hy + 0.5f) / 100.0f;
            if (y + off >= hy) {
                outA[1] = hy;
                best = hit;
                *outB = 0.0f;
                KHEAD(i) = hit;
            } else {
                *outB = hy - outA[1] - off;
            }
        }
    }
    return best;
}

HitEntry* kpaSearchFrontUnder(void) {
    return NULL;
}

HitEntry* kpaGetHitobjHead(s32 i) {
    HitEntry** e = &KPW->headHit[i];
    return *e;
}

HitEntry* kpaGetHitobjHead2(void) {
#ifdef VERSION_G8ME01
    s32 i;
    HitEntry* hit;
    HitEntry* ret;
    for (i = 0; i < 10; i++) {
        hit = KHEAD(i);
        ret = hit;
        if (hit) {
            break;
        }
    }
    return ret;
#else
    return KPW->headHit[0];
#endif
}

HitEntry* kpaGetHitobjRide(s32 i) {
    HitEntry** e = &KPW->rideHit[i];
    return *e;
}

HitEntry* kpaChkHitobjRide(void) {
KpaWork* w = KPW;
HitEntry* r;
r = w->rideHit[0];
if (!r) {
r = w->rideHit[1];
if (!r) {
r = w->rideHit[2];
if (!r) {
r = w->rideHit[3];
if (!r) {
r = w->rideHit[4];
if (!r) {
r = w->rideHit[5];
if (!r) {
r = w->rideHit[6];
if (!r) {
r = w->rideHit[7];
if (!r) {
r = w->rideHit[8];
if (!r) {
r = w->rideHit[9];
if (r) { return r; } else { return r; }
}
}
}
}
}
}
}
}
}
return r;
}

long kpaClearHitobjRide(void) {
    MarioWork* m = marioGetPtr();
    m->kpaWork->rideHit[0] = NULL;
    m->kpaWork->rideHit[1] = NULL;
    m->kpaWork->rideHit[2] = NULL;
    m->kpaWork->rideHit[3] = NULL;
    m->kpaWork->rideHit[4] = NULL;
    m->kpaWork->rideHit[5] = NULL;
    m->kpaWork->rideHit[6] = NULL;
    m->kpaWork->rideHit[7] = NULL;
    m->kpaWork->rideHit[8] = NULL;
    m->kpaWork->rideHit[9] = NULL;
}

HitEntry* kpaGetHitobj(void) {
    MarioWork* m = marioGetPtr();
    if (m->characterId != CHARACTER_BOWSER) {
        return NULL;
    }
    return m->kpaWork->objHit;
}

HitEntry* kpaGetHitFlame(void) {
    MarioWork* m = marioGetPtr();
    if (m->characterId != CHARACTER_BOWSER) {
        return NULL;
    }
    return m->kpaWork->flameHit;
}

typedef struct HeadSphere {
    s32 forward;
    s32 y;
    s32 radius;
    s32 mode;
} HeadSphere; // mode: 0 always, 1 grounded only, 2 airborne only, 3 neither flag
typedef struct HeadSphereSet1 { s32 count; HeadSphere spheres[1]; } HeadSphereSet1;
typedef struct HeadSphereSet3 { s32 count; HeadSphere spheres[3]; } HeadSphereSet3;
typedef struct HeadSphereSet4 { s32 count; HeadSphere spheres[4]; } HeadSphereSet4;
static HeadSphereSet3 head_sphere_data_lv1 = {
    3,
    {
    {32, 55, 24, 0},
    {26, 30, 16, 2},
    {-10, 44, 25, 1},
    },
};
static HeadSphereSet3 head_sphere_data_lv2 = {
    3,
    {
    {45, 75, 34, 0},
    {36, 30, 16, 2},
    {-15, 59, 36, 1},
    },
};
static HeadSphereSet4 head_sphere_data_lv3 = {
    4,
    {
    {100, 162, 80, 0},
    {90, 75, 30, 2},
    {50, 40, 30, 3},
    {-43, 142, 74, 1},
    },
};
static HeadSphereSet1 head_sphere_data_lv0 = {
    1,
    {
    {10, 15, 6, 0},
    },
};
static s32* head_sphere_data[] = {(s32*)&head_sphere_data_lv0, (s32*)&head_sphere_data_lv1, (s32*)&head_sphere_data_lv2, (s32*)&head_sphere_data_lv3};

HitEntry* kpaHitCheck(void) {
    s32* p;
    s32 j;
    s32 i;
    HitEntry* hit;
    s32 n;
    MarioWork* m;
    HitEntry* best;
    f32 rad, x, y, z, r, sn;

    best = NULL;
    m = marioGetPtr();
    if (m->characterId != CHARACTER_BOWSER) {
        return NULL;
    }
    m->kpaWork->objHit = NULL;
    kpa_headClear();
    if (kpaFireAttackCheck() == 1) {
        return NULL;
    }
    p = head_sphere_data[kpaGetLevel()];
    n = *p++;
    for (j = 0; j < n; j++) {
        sn = (f32)sin(3.1415927f * m->directionView / 180.0f);
        x = m->position.x + p[0] * sn;
        sn = (f32)cos(3.1415927f * m->directionView / 180.0f);
        z = m->position.z - p[0] * sn;
        y = m->position.y + p[1];
        r = p[2];
        i = p[3];
        p += 4;
        switch (i) {
        case 1:
            if (!(m->flags & MARIO_FLAG_IS_JUMPING)) {
                continue;
            }
            break;
        case 2:
            if (m->flags & MARIO_FLAG_IS_JUMPING) {
                continue;
            }
            break;
        case 3:
            if (m->flags & (MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING)) {
                continue;
            }
            break;
        }
        hit = hitCheckSphereFilter(NULL, x, y, z, r);
        if ((m->flags & MARIO_FLAG_IS_JUMPING) && (hitGetAttr(hit) & 0x80000000) && mobjHitObjPtrToPtr(hit)->position.y > y) {
            KHEAD(j) = hit;
        }
        if (hit) {
            {
            }
            best = hit;
        }
    }
    m->kpaWork->objHit = best;
    return best;
}

BOOL kpaFlameHitFilter(HitCheckQuery* query, HitEntry* entry) {
    MapObjectEntry* mobj = mobjHitObjPtrToPtr(entry);
    if (!strcmp(mobj->animName, "MOBJ_HiddenHatenaBlock")) {
        return FALSE;
    }
    if (!strcmp(mobj->animName, "MOBJ_Block")) {
        return FALSE;
    }
    return TRUE;
}

HitEntry* kpaFlameHitCheck(f32 x, f32 y, f32 z, f32 r) {
    marioGetPtr();
    return hitCheckSphereFilter(kpaFlameHitFilter, x, y, z, r);
}

// FAKE: stand-in for a function the linker stripped from the original object; its 0.5f is the first
// literal in the target .sdata2 pool (codegen runs bottom-up, so it must sit last in the file). The JP demo
// map shows the head of koopa_hit.o as kpaSearchWallFront (kept there, 0x320 bytes) and the stripped
// _reflect2/_reflect; the demo kpaSearchWallFront's literals are 45, 0.75, 0.25, 0.5, so the US original
// (whose only literal left is 0.5) was a different version and cannot be taken from the demo.
static f32 kpaHalf(f32 v) {
    return 0.5f * v;
}
