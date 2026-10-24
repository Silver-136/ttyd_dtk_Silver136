#include "party.h"
#include "driver/hitdrv.h"
#include "driver/mobjdrv.h"
#include "mario/mario.h"
#include "system.h"
#include <dolphin/types.h>
#include <string.h>
#include <dolphin/mtx.h>
#include "party/party_yoshi.h"
#include "party/party_move.h"
#include "mario/mario_hit.h"
#include "party/party_hit.h"
#include "libc_decls.h"
#include "eff/eff_magic1_n64.h"


#include "mario/mario_party.h"
#include "mario/mario_sbr.h"
#ifdef VERSION_G8ME01
#include "mario/mariost.h"
#endif
#include <math.h>

HitEntry* partySearchGround(PartyEntry* party, f32 range, f32 depth);
HitEntry* partyChkFrontStep(PartyEntry* party, f32 range, f32* outY, f32* outDist, f32* outAng);
extern HitEntry* partyChkFrontStep(PartyEntry* party, f32 a, f32* x, f32* y, f32* z);

BOOL chkfilter(HitCheckQuery* query, HitEntry* entry) {
    MapObjectEntry* mobj;
    marioGetPtr();
    if (entry->attributes & 0x80080) {
        return TRUE;
    }
    if (entry->attributes & 0x800004) {
        return FALSE;
    }
    if (!(entry->attributes & 0x80000000)) {
        return TRUE;
    }
    mobj = mobjHitObjPtrToPtr(entry);
    if (!strcmp(mobj->animName, "MOBJ_HiddenHatenaBlock") || !strcmp(mobj->animName, "MOBJ_HiddenBadgeBlock") ||
        !strcmp(mobj->animName, "MOBJ_Hidden10CountBlock")) {
        mobjHitObjPtrToPtr(entry);
        return FALSE;
    }
    return TRUE;
}

BOOL chkfilterYoshi(HitCheckQuery* query, HitEntry* entry) {
    MapObjectEntry* mobj;
    marioGetPtr();
    if (entry->attributes & 0x80080) {
        return TRUE;
    }
    if (entry->attributes & 0x800000) {
        return TRUE;
    }
    if (entry->attributes & 0x4) {
        return FALSE;
    }
    if (!(entry->attributes & 0x80000000)) {
        return TRUE;
    }
    mobj = mobjHitObjPtrToPtr(entry);
    if (!strcmp(mobj->animName, "MOBJ_HiddenHatenaBlock") || !strcmp(mobj->animName, "MOBJ_HiddenBadgeBlock") ||
        !strcmp(mobj->animName, "MOBJ_Hidden10CountBlock")) {
        mobjHitObjPtrToPtr(entry);
        return FALSE;
    }
    return TRUE;
}

BOOL chkfilterNokotaro(HitCheckQuery* query, HitEntry* entry) {
    MapObjectEntry* mobj;
    marioGetPtr();
    if (entry->attributes & 0x800005) {
        return FALSE;
    }
    if (entry->attributes & 0x80080) {
        return TRUE;
    }
    if (!(entry->attributes & 0x80000000)) {
        return TRUE;
    }
    mobj = mobjHitObjPtrToPtr(entry);
    if (!strcmp(mobj->animName, "MOBJ_HiddenHatenaBlock") || !strcmp(mobj->animName, "MOBJ_HiddenBadgeBlock") ||
        !strcmp(mobj->animName, "MOBJ_Hidden10CountBlock")) {
        mobjHitObjPtrToPtr(entry);
        return FALSE;
    }
    return TRUE;
}

static inline f32 party_sumsq(f32 x, f32 z) { return x * x + z * z; }

static inline HitEntry* partyHitCheckInl(PartyEntry* party, Vec* pos, Vec* dir, Vec* outPos, Vec* outNrm, f32* dist) {
    HitCheckQuery query;
    Vec d = *dir;
    HitEntry* hit;
    f32 rf;

    query.targetPos = *pos;
    query.targetDir = d;
    query.targetDistance = *dist;
    if (yoshiGetStatus()) {
        hit = hitCheckVecFilter(&query, chkfilterYoshi);
    } else if (party->currentMemberId == PARTY_MEMBER_KOOPS && (party->flags & PARTY_FLAG_IS_BEING_USED)) {
        hit = hitCheckVecFilter(&query, chkfilterNokotaro);
    } else {
        hit = hitCheckVecFilter(&query, chkfilter);
    }
    if (!hit) {
        return hit;
    }
    if (query.targetDistance >= 0.0f) {
        rf = 0.5f;
    } else {
        rf = -0.5f;
    }
    *dist = (s32)((1000.0f * query.targetDistance) + rf) / 1000.0f;
    if (query.hitPos.x >= 0.0f) {
        rf = 0.5f;
    } else {
        rf = -0.5f;
    }
    outPos->x = (s32)((1000.0f * query.hitPos.x) + rf) / 1000.0f;
    if (query.hitPos.y >= 0.0f) {
        rf = 0.5f;
    } else {
        rf = -0.5f;
    }
    outPos->y = (s32)((1000.0f * query.hitPos.y) + rf) / 1000.0f;
    if (query.hitPos.z >= 0.0f) {
        rf = 0.5f;
    } else {
        rf = -0.5f;
    }
    outPos->z = (s32)((1000.0f * query.hitPos.z) + rf) / 1000.0f;
    outNrm->x = query.hitNormal.x;
    outNrm->y = query.hitNormal.y;
    outNrm->z = query.hitNormal.z;
    return hit;
}

HitEntry* partyHitCheck(PartyEntry* party, Vec* pos, Vec* dir, Vec* outPos, Vec* outNrm, f32* dist) {
    return partyHitCheckInl(party, pos, dir, outPos, outNrm, dist);
}

static inline f32 partyNrmTo(Vec* n) {
    return angleABf(0.0f, 0.0f, sqrtf(n->x * n->x + n->z * n->z), -n->y);
}

f32 partyNrmToAngle(Vec n) {
    return angleABf(0.0f, 0.0f, sqrtf(n.x * n.x + n.z * n.z), -n.y);
}

void partyChkGnd(PartyEntry* party) {
    s32 attr;
    HitEntry* hit;
    f32 a, b, c;

    if (!(party->flags & 0x2000000)) {
        attr = 0;
        hit = partySearchGround(party, 11.0f, party->velY);
        if (hit) {
            attr = hitGetAttr(hit);
            if (attr & 0x200) {
                hit = NULL;
            }
        }
        party->hitobjStandOn = hit;
        party->flags &= ~0x80;
        if (hit == NULL) {
            party->flags &= ~0x2;
            partyChgMot(party, 3);
            party->gravity = -1.0f;
        } else {
            party->flags |= 0x2;
            if (attr & 0x100) {
                party->flags |= 0x80;
            } else {
                party->flags &= ~0x80;
            }
        }
        party->position.y = party->groundY;
        if (!(party->flags & 0x30)) {
            partyChkFrontStep(party, 11.0f, &a, &b, &c);
            if (a - party->position.y >= 5.0f && party->groundSlope <= 30.0f) {
                party->stepY = a;
                if (party->moveSpeed) {
                    partyChgMot(party, 6);
                }
            }
        }
    }
}

static inline f32 partyNrmSet(PartyEntry* party, Vec n) {
    party->groundRot.x = (s32)(angleABf(0.0f, 0.0f, 100.0f * n.z, 100.0f * n.y) - 180.0f);
    party->groundRotY = 0.0f;
    party->groundRotZ = (s32)(angleABf(0.0f, 0.0f, 100.0f * n.x, 100.0f * n.y) - 180.0f);
    return partyNrmTo(&n);
}

HitEntry* partySearchGround(PartyEntry* party, f32 range, f32 depth) {
    HitEntry* hit;
    HitEntry* best;
    s32 i;
    f32 probeY, nsz, nsx, len, bestY, sx, sz, px, baseY, pz, gy, ox, oz, rad, d;
    Vec pos;
    Vec dir;
    Vec outPos, outNrm;
    f32 dist;

    if (party->flags & 0x2000000) {
        return NULL;
    }
    {
        Vec zero = {0.0f, 0.0f, 0.0f};
        party->groundRot = zero;
    }
    party->groundSlope = 0.0f;
    range = __fabsf(range); range += 0.1f;
    if (marioGetPtr()->currMotionId == 0x1A) {
        d = 5.0f;
    } else {
        d = 0.75f * partyGetHeight(party);
    }
    party->groundY = party->position.y;
    party->dropDepth = -1.0f;
    if (party->moveSpeed) {
        rad = party->targetDir;
    } else {
        rad = partyToMovedir(party, party->displayDirection);
    }
    rad = 3.1415927f * rad / 180.0f;
    nsz = (f32)sin(rad); sx = 0.375f * (party->width * nsz); nsz = (f32)cos(rad); sz = 0.375f * (party->width * -nsz);
    nsx = -sx;
    nsz = -sz;
    baseY = party->position.y;
    probeY = baseY + d;
    len = d + range;
    {
    {Vec d0 = {0.0f, -1.0f, 0.0f}; dir = d0;}
    best = NULL;
    px = party->position.x;
    pz = party->position.z;
    bestY = -10000.0f;
    for (i = 0; i < 5; i++) {
        if (i == 0) {
            ox = 0.0f;
            oz = 0.0f;
        } else if (i == 1) {
            ox = sx;
            oz = sz;
        } else if (i == 2) {
            ox = nsz;
            oz = sx;
        } else if (i == 3) {
            ox = sz;
            oz = nsx;
        } else if (i == 4) {
            ox = nsx;
            oz = nsz;
        }
        pos.x = px + ox;
        pos.y = probeY;
        pos.z = pz + oz;
        dist = len;
        hit = partyHitCheckInl(party, &pos, &dir, &outPos, &outNrm, &dist);
        gy = outPos.y;
        if (hit) {
            if (partyNrmSet(party, outNrm) >= 50.0f) {
                hit = NULL;
            }
        }
        if (hit) {
            party->groundSlope = partyNrmSet(party, outNrm);
            if (gy < baseY && __fabsf(gy - baseY) >= 2.0f) {
                if (__fabsf(gy - baseY) > __fabsf(depth)) {
                    party->dropDepth = __fabsf(baseY - gy);
                } else if (gy > bestY) {
                    bestY = gy;
                    best = hit;
                }
            } else if (__fabsf(gy - baseY) <= 11.0f) {
                if (gy > bestY) {
                    bestY = gy;
                    best = hit;
                }
            }
        }
    }
    }
    if (best) {
        party->groundY = bestY;
        party->dropDepth = 0.0f;
        return best;
    }
    return NULL;
}

HitEntry* partyChkFrontStep(PartyEntry* party, f32 range, f32* outY, f32* outDist, f32* outAng) {
    HitEntry* hit;
    f32 dx, sn, cs, h, ang, ox, oy, oz, gy;
    Vec pos, outPos, outNrm;
    f32 dist, rad;

    range += 0.01f;
    {
        Vec zero = {0.0f, 0.0f, 0.0f};
        party->groundRot = zero;
    }
    party->groundSlope = 0.0f;
    h = partyGetHeight(party);
    *outY = party->position.y;
    *outDist = -1.0f;
    if (party->moveSpeed) {
        ang = party->targetDir;
    } else {
        ang = toMovedir(party->displayDirection);
    }
    ang = revise360((s32)(0.5f + ang));
    dx = 0.265f * party->width;
    rad = 3.1415927f * ang / 180.0f;
    sn = (f32)sin(rad);
    cs = (f32)cos(rad);
    oy = party->position.y;
    ox = party->position.x;
    oz = party->position.z;
    {
        Vec p = {0.0f, 0.0f, 0.0f};
        Vec dir = {0.0f, -1.0f, 0.0f};
        f32 mx;
        f32 mz;
        mx = dx * sn;
        mz = dx * -cs;
        p.x = ox + mx;
        p.y = oy + h;
        p.z = oz + mz;
        pos = p;
        dist = h + range;
        hit = partyHitCheckInl(party, &pos, &dir, &outPos, &outNrm, &dist);
    }
    if (hit) {
        Vec n = outNrm;
        party->groundRot.x = (s32)(angleABf(0.0f, 0.0f, 100.0f * n.z, 100.0f * n.y) - 180.0f);
        party->groundRotY = 0.0f;
        party->groundRotZ = (s32)(angleABf(0.0f, 0.0f, 100.0f * n.x, 100.0f * n.y) - 180.0f);
        party->groundSlope = partyNrmTo(&n);
        gy = outPos.y;
        if (gy < oy && __fabsf(gy - oy) >= 2.0f) {
            *outDist = oy - gy;
            *outAng = angleABf(ox, oz, outPos.x, outPos.z);
        } else {
            *outY = gy;
            *outDist = 0.0f;
            *outAng = angleABf(ox, oz, outNrm.x, outNrm.z);
            return hit;
        }
    }
    return NULL;
}

static inline void partyWallSlide(Vec* out, Vec n, f32 d, f32 sn, f32 cs) {
    Vec unused = {0.0f, 0.0f, 0.0f};
    f32 mx = sn * d;
    f32 mz = cs * d;
    f32 dot = mx * n.x + mz * n.z;
    out->x += 0.5f * (mx - dot * n.x);
    out->z += 0.5f * (mz - dot * n.z);
}

#ifdef VERSION_G8ME01
HitEntry* partySearchWallFront(PartyEntry* party, Vec* pos, f32 dist, f32 ang) {
    Vec qpos;
    Vec qdir;
    Vec outPos;
    Vec bestNrm;
    Vec nrm;
    Vec v = {0.0f, 0.0f, 0.0f};
    f32 arr[4];
    f32 sn, cs, sn2, cs2;
    f32 bestD, d;
    f32 rd, maxD;
    HitEntry* best;
    s32 count;
    HitEntry* hit;
    s32 i;

    sincosf(&sn, &cs, ang);
    count = 3;
    arr[0] = 0.75f * partyGetHeight(party);
    arr[1] = 0.5f * partyGetHeight(party);
    if ((party->flags & 0x30) || yoshiGetStatus() == 2) {
        arr[2] = 1.0f;
    } else {
        arr[2] = 11.0f;
        if (!strcmp(gp->currentMapName, "gor_02") && party->position.y >= 120.0f && party->position.x <= 350.0f) {
            arr[3] = 1.0f;
            count = 4;
        }
    }
    rd = 0.5f * party->width;
    best = NULL;
    maxD = rd + dist;
    bestD = maxD;
    do {
    qpos = *pos;
    {
    Vec dir1 = {0.0f, 0.0f, 0.0f};
    dir1.x = sn;
    dir1.z = cs;
    qdir = dir1;
    }
    for (i = 0; i < count; i++) {
        d = maxD;
        qpos.y = pos->y + arr[i];
        hit = marioHitCheckVec(&qpos, &qdir, &outPos, &nrm, &d);
        if (hit) {
            if (best) {
                if (d < bestD) {
                    best = hit;
                    bestD = d;
                    bestNrm = nrm;
                }
            } else {
                best = hit;
                bestD = d;
                bestNrm = nrm;
            }
        }
    }
    if (best) {
        Vec nn;
        f32 dot;
        f32 mx, mz;
        f32 nz;
        nn = bestNrm;
        mx = sn * dist;
        mz = cs * dist;
        nz = nn.z;
        dot = mx * nn.x + mz * nz;
        v.x += 0.5f * (mx - dot * nn.x);
        v.z += 0.5f * (mz - dot * nz);
    }
    hit = NULL;
    sincosf(&sn2, &cs2, ang + 45.0f);
    bestD = rd;
    qpos = *pos;
    {
    Vec dir2 = {0.0f, 0.0f, 0.0f};
    dir2.x = sn2;
    dir2.z = cs2;
    qdir = dir2;
    }
    for (i = 0; i < count; i++) {
        bestD = rd;
        qpos.y = pos->y + arr[i];
        hit = marioHitCheckVec(&qpos, &qdir, NULL, NULL, &bestD);
        if (hit) {
            break;
        }
    }
    if (hit) {
        hit = NULL;
        sincosf(&sn2, &cs2, ang - 45.0f);
        for (i = 0; i < count; i++) {
            bestD = rd;
            qpos.y = pos->y + arr[i];
            {
            Vec dd = {0.0f, 0.0f, 0.0f};
            dd.x = sn2;
            dd.z = cs2;
            qdir = dd;
            }
            hit = marioHitCheckVec(&qpos, &qdir, NULL, NULL, &bestD);
        }
    }
    if (best) {
        if (!hit) {
            pos->x += v.x;
            pos->z += v.z;
        }
        return best;
    }
    if (!hit) {
        pos->x += dist * sn;
        pos->z += dist * cs;
    }
    return NULL;
    } while (0);
}

#else
HitEntry* partySearchWallFront(PartyEntry* party, Vec* pos, f32 dist, f32 ang) {
    Vec qpos;
    Vec qdir;
    Vec outPos;
    Vec bestNrm;
    Vec nrm;
    Vec v = {0.0f, 0.0f, 0.0f};
    f32 arr[4];
    f32 sn, cs, sn2, cs2;
    f32 bestD, d;
    f32 rd, maxD;
    HitEntry* best;
    HitEntry* hit;
    s32 i;

    sincosf(&sn, &cs, ang);
    arr[0] = 0.75f * partyGetHeight(party);
    arr[1] = 0.5f * partyGetHeight(party);
    if ((party->flags & 0x30) || yoshiGetStatus() == 2) {
        arr[2] = 1.0f;
    } else {
        arr[2] = 11.0f;
    }
    rd = 0.5f * party->width;
    best = NULL;
    maxD = rd + dist;
    bestD = maxD;
    do {
    qpos = *pos;
    {
    Vec dir1 = {0.0f, 0.0f, 0.0f};
    dir1.x = sn;
    dir1.z = cs;
    qdir = dir1;
    }
    for (i = 0; i < 3; i++) {
        d = maxD;
        qpos.y = pos->y + arr[i];
        hit = marioHitCheckVec(&qpos, &qdir, &outPos, &nrm, &d);
        if (hit) {
            if (best) {
                if (d < bestD) {
                    best = hit;
                    bestD = d;
                    bestNrm = nrm;
                }
            } else {
                best = hit;
                bestD = d;
                bestNrm = nrm;
            }
        }
    }
    if (best) {
        Vec nn;
        f32 dot;
        f32 mx, mz;
        f32 nz;
        nn = bestNrm;
        mx = sn * dist;
        mz = cs * dist;
        nz = nn.z;
        dot = mx * nn.x + mz * nz;
        v.x += 0.5f * (mx - dot * nn.x);
        v.z += 0.5f * (mz - dot * nz);
    }
    sincosf(&sn2, &cs2, ang + 45.0f);
    bestD = rd;
    qpos = *pos;
    {
    Vec dir2 = {0.0f, 0.0f, 0.0f};
    dir2.x = sn2;
    dir2.z = cs2;
    qdir = dir2;
    }
    for (i = 0; i < 3; i++) {
        bestD = rd;
        qpos.y = pos->y + arr[i];
        hit = marioHitCheckVec(&qpos, &qdir, NULL, NULL, &bestD);
        if (hit) {
            break;
        }
    }
    if (hit) {
        sincosf(&sn2, &cs2, ang - 45.0f);
        for (i = 0; i < 3; i++) {
            bestD = rd;
            qpos.y = pos->y + arr[i];
            {
            Vec dd = {0.0f, 0.0f, 0.0f};
            dd.x = sn2;
            dd.z = cs2;
            qdir = dd;
            }
            hit = marioHitCheckVec(&qpos, &qdir, NULL, NULL, &bestD);
        }
    }
    if (best) {
        if (!hit) {
            pos->x += v.x;
            pos->z += v.z;
        }
        return best;
    }
    if (!hit) {
        pos->x += dist * sn;
        pos->z += dist * cs;
    }
    return NULL;
    } while (0);
}

#endif

#ifdef VERSION_G8ME01
HitEntry* partySearchFrontWall(PartyEntry* party, Vec* pos, f32 dist, f32 ang) {
    f32 sn, cs;
    f32 d, dy, hh;
    f32 w, maxD;
    HitEntry* hit;
    Vec p, nrm;

    sincosf(&sn, &cs, ang);
    w = party->width;
    if (party->flags & 0x1000) {
        w = 6.0f;
    }
    d = maxD = 0.5f * w + dist;
    hh = 0.75f * partyGetHeight(party);
    p = party->position;
    p.y += hh;
    {
    Vec dir = {0.0f, 0.0f, 0.0f};
    Vec outPos = {0.0f, 0.0f, 0.0f};
    dir.x = sn;
    dir.z = cs;
    hit = partyHitCheckInl(party, &p, &dir, &outPos, &nrm, &d);
    if (hit) {
        party->frontWallDist = d;
        partyWallSlide(pos, nrm, dist, sn, cs);
        return hit;
    }
    dy = 11.0f;
    if (party->currentMemberId == PARTY_MEMBER_BOBBERY && (party->flags & PARTY_FLAG_IS_BEING_USED)) {
        dy = 1.0f;
    }
    p = party->position;
    d = maxD;
    p.y += dy;
    hit = partyHitCheckInl(party, &p, &dir, &outPos, &nrm, &d);
    if (hit) {
        if (partyNrmTo(&nrm) >= 45.0f) {
            party->frontWallDist = d;
            partyWallSlide(pos, nrm, dist, sn, cs);
            return hit;
        }
    }
    }
    party->frontWallDist = -1.0f;
    pos->x += dist * sn;
    pos->z += dist * cs;
    return NULL;
}

#else
HitEntry* partySearchFrontWall(PartyEntry* party, Vec* pos, f32 dist, f32 ang) {
    f32 sn, cs;
    f32 d, dy, hh;
    HitEntry* hit;
    Vec p, nrm;

    sincosf(&sn, &cs, ang);
    d = 0.5f * party->width + dist;
    hh = 0.75f * partyGetHeight(party);
    p = party->position;
    p.y += hh;
    {
    Vec dir = {0.0f, 0.0f, 0.0f};
    Vec outPos = {0.0f, 0.0f, 0.0f};
    dir.x = sn;
    dir.z = cs;
    hit = partyHitCheckInl(party, &p, &dir, &outPos, &nrm, &d);
    if (hit) {
        party->frontWallDist = d;
        partyWallSlide(pos, nrm, dist, sn, cs);
        return hit;
    }
    dy = 11.0f;
    if (party->currentMemberId == PARTY_MEMBER_BOBBERY && (party->flags & PARTY_FLAG_IS_BEING_USED)) {
        dy = 1.0f;
    }
    p = party->position;
    d = 0.5f * party->width + dist;
    p.y += dy;
    hit = partyHitCheckInl(party, &p, &dir, &outPos, &nrm, &d);
    if (hit) {
        if (partyNrmTo(&nrm) >= 45.0f) {
            party->frontWallDist = d;
            partyWallSlide(pos, nrm, dist, sn, cs);
            return hit;
        }
    }
    }
    party->frontWallDist = -1.0f;
    pos->x += dist * sn;
    pos->z += dist * cs;
    return NULL;
}

#endif

void partySearchHead(PartyEntry *party, Vec *pos, f32 *dist, f32 angle)
{
  f32 lx;
  f32 ly;
  f32 lz;
  f32 best;
  f32 oz;
  f32 ox;
  Vec t;
  f32 s;
  f32 c;
  volatile f32 px;
  volatile f32 py;
  volatile f32 pz;
  f32 d0;
  f32 nz0;
  f32 ny0;
  f32 nx0;
  float probeHeight;
  // FAKE: t.y read into a reused local right after each t store, so the load numbers below the 10.0f
  // (the add is written probeHeight + ty, as in the target operand order)
  f32 ty;
  f32 hz0;
  f32 hy0;
  f32 hx0;
  f32 d1;
  f32 nz1;
  f32 ny1;
  f32 nx1;
  f32 hz1;
  f32 hy1;
  f32 hx1;
  f32 d2;
  f32 nz2;
  f32 ny2;
  f32 nx2;
  f32 hz2;
  f32 hy2;
  f32 hx2;
  f32 d3;
  f32 nz3;
  f32 ny3;
  f32 nx3;
  f32 hz3;
  f32 hy3;
  f32 hx3;
  HitEntry *hit;
  f32 zero = 0.0f;
  f32 one = 1.0f;
  oz = party->width * 0.3f;
  sincosf(&s, &c, angle);
  px = pos->x;
  ox = (-oz) * c;
  oz = oz * s;
  py = pos->y;
  pz = pos->z;
  best = *dist;
  {
    lx = px + oz;
    lz = pz + ox;
    ly = py;
    t = (Vec){lx, ly, lz};
    ty = t.y;
    d0 = best;
    // FAKE: empty barrier (no code) steers scheduling of the 10.0f + t.y argument
    if ((t.z && t.z) && t.z)
    {
    }
    // FAKE: probeHeight reloaded with the same 10.0f (permuter idiom)
    probeHeight = 10.0f;
    hit = hitCheckFilter((void *) 0, &hx0, &hy0, &hz0, &d0, &nx0, &ny0, &nz0, t.x, probeHeight + ty, t.z, zero, one, zero);
    if (hit && (best >= d0))
    {
      best = d0;
    }
  }
  if (!hit)
  {
    lx = px - oz;
    ly = py;
    lz = pz - ox;
    t = (Vec){lx, ly, lz};
    ty = t.y;
    // FAKE: empty barrier (no code) steers scheduling of the 10.0f + t.y argument
    if ((t.z && t.z) && t.z) { }
    d1 = best;
    // FAKE: probeHeight reloaded with the same 10.0f (permuter idiom)
    probeHeight = 10.0f;
    hit = hitCheckFilter((void *) 0, &hx1, &hy1, &hz1, &d1, &nx1, &ny1, &nz1, t.x, probeHeight + ty, t.z, zero, one, zero);
    if (hit && (best >= d1))
    {
      best = d1;
    }
    if (!hit)
    {
      lx = px + ox;
      ly = py;
      lz = pz + oz;
      t = (Vec){lx, ly, lz};
      ty = t.y;
      // FAKE: empty barrier (no code) steers scheduling of the 10.0f + t.y argument
      if ((t.z && t.z) && t.z) { }
      d2 = best;
      // FAKE: probeHeight reloaded with the same 10.0f (permuter idiom)
      probeHeight = 10.0f;
      hit = hitCheckFilter((void *) 0, &hx2, &hy2, &hz2, &d2, &nx2, &ny2, &nz2, t.x, probeHeight + ty, t.z, zero, one, zero);
      if (hit && (best >= d2))
      {
        best = d2;
      }
      if (!hit)
      {
        lx = px - ox;
        ly = py;
        lz = pz - oz;
        t = (Vec){lx, ly, lz};
        ty = t.y;
        // FAKE: empty barrier (no code) steers scheduling of the 10.0f + t.y argument
        if ((t.z && t.z) && t.z) { }
        d3 = best;
        // FAKE: probeHeight reloaded with the same 10.0f (permuter idiom)
        probeHeight = 10.0f;
        hit = hitCheckFilter((void *) 0, &hx3, &hy3, &hz3, &d3, &nx3, &ny3, &nz3, t.x, probeHeight + ty, t.z, zero, one, zero);
        if (hit && (best >= d3))
        {
          best = d3;
        }
      }
    }
  }
  if (hit)
  {
    pos->x = lx;
    pos->y = ly;
    pos->z = lz;
    *dist = best;
  }
  if (!hit)
  {
    pos->x = lx;
    pos->y = ly;
    pos->z = lz;
    *dist = 0.0f;
  }
}

void partyHitStrippedPool(f32* f, s32 i) {
    f[0] = 10.0f;
    f[1] = 0.0f;
    f[2] = 1.0f;
    f[3] = 0.5f;
    f[4] = 100.0f;
    f[5] = (f32)i;
}

#ifdef VERSION_G8ME01

/* FAKE: inline add keeps the original fadds operand order (pos.y, 10.0f) */
static inline f32 party_addf(f32 a, f32 b) { return a + b; }

static inline HitEntry* partyHitNrm(PartyEntry* party, Vec* pos, Vec* dir, Vec* nrm) {
    HitCheckQuery query;
    HitEntry* hit;

    query.targetPos = *pos;
    query.targetDir = *dir;
    query.targetDistance = 20.0f;
    if (yoshiGetStatus()) {
        hit = hitCheckVecFilter(&query, chkfilterYoshi);
    } else if (party->currentMemberId == PARTY_MEMBER_KOOPS && (party->flags & PARTY_FLAG_IS_BEING_USED)) {
        hit = hitCheckVecFilter(&query, chkfilterNokotaro);
    } else {
        hit = hitCheckVecFilter(&query, chkfilter);
    }
    if (!hit) {
        return hit;
    }
    nrm->x = query.hitNormal.x;
    nrm->y = query.hitNormal.y;
    nrm->z = query.hitNormal.z;
    return hit;
}

void partyChkWallAround(PartyEntry* party, Vec* pos, f32 spd, f32 dir, f32 width, f32 height) {
    if (party->flags & 0x1000) {
        width = 6.0f;
    }
    marioChkWallAround(pos, 1, spd, dir, width, height);
}

BOOL partyChkBetweenWalls(PartyEntry* party, Vec pos) {
    f32 r = party->width;
    f32 sn, cs;
    f32 ox, oz;
    f32 ang1, ang2;
    Vec p;
    Vec n;
    HitEntry* hit1;
    HitEntry* hit2;

    sincosf(&sn, &cs, toMovedirSimple(270.0f));
    ox = 0.5f * (sn * r);
    oz = 0.5f * (cs * r);
    ang1 = 0.0f;
    ang2 = ang1;
    sincosf(&sn, &cs, toMovedirSimple(180.0f));
    {
        Vec d;
        p = pos;
        p.y += 10.0f;
        d = (Vec){sn, 0.0f, cs};
        hit1 = partyHitNrm(party, &p, &d, &n);
        if (hit1) {
            ang1 = angleABf(0.0f, 0.0f, n.x, n.z);
        }
        if (!hit1) {
            hit1 = partyHitNrm(party, &(Vec){pos.x + ox, party_addf(pos.y, 10.0f), pos.z + oz}, &d, &n);
            if (hit1) {
                ang1 = angleABf(0.0f, 0.0f, n.x, n.z);
            }
        }
        if (!hit1) {
            hit1 = partyHitNrm(party, &(Vec){pos.x - ox, party_addf(pos.y, 10.0f), pos.z - oz}, &d, &n);
            if (hit1) {
                ang1 = angleABf(0.0f, 0.0f, n.x, n.z);
            }
        }
    }
    sincosf(&sn, &cs, toMovedirSimple(0.0f));
    {
        Vec d;
        p = pos;
        p.y += 10.0f;
        d = (Vec){sn, 0.0f, cs};
        hit2 = partyHitNrm(party, &p, &d, &n);
        if (hit2) {
            ang2 = angleABf(0.0f, 0.0f, n.x, n.z);
        }
        if (!hit2) {
            hit2 = partyHitNrm(party, &(Vec){pos.x + ox, party_addf(pos.y, 10.0f), pos.z + oz}, &d, &n);
            if (hit2) {
                ang2 = angleABf(0.0f, 0.0f, n.x, n.z);
            }
        }
        if (!hit2) {
            hit2 = partyHitNrm(party, &(Vec){pos.x - ox, party_addf(pos.y, 10.0f), pos.z - oz}, &d, &n);
            if (hit2) {
                ang2 = angleABf(0.0f, 0.0f, n.x, n.z);
            }
        }
    }
    if (hit1 && hit2 && __fabsf(180.0f - 0.5f * (ang1 + ang2)) <= 10.0f) {
        return TRUE;
    }
    return FALSE;
}
#endif
