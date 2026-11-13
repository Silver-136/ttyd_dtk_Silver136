// Partner collision: ground, wall and ceiling checks for the partners.

#include "version.h"
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

#if VERSION >= VERSION_US
#include "mario/mariost.h"
#endif

#include <math.h>

static BOOL chkfilter(HitCheckQuery* query, HitEntry* entry) {
    MapObjectEntry* mobj;
    marioGetPtr();
    if (entry->attributes & (HIT_ATTR_SHIP_ONLY | HIT_ATTR_ROLL_THROUGH)) {
        return TRUE;
    }
    if (entry->attributes & (HIT_ATTR_NPC_ONLY | HIT_ATTR_BERO)) {
        return FALSE;
    }
    if (!(entry->attributes & HIT_ATTR_MOBJ)) {
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

/**
 * Partner ray filter used while yoshiGetStatus() is nonzero:
 * like chkfilter, but surfaces with 0x800000 still collide and only attribute 0x4 is skipped.
 * Hidden blocks are passed through.
 */
static BOOL chkfilterYoshi(HitCheckQuery* query, HitEntry* entry) {
    MapObjectEntry* mobj;
    marioGetPtr();
    if (entry->attributes & (HIT_ATTR_SHIP_ONLY | HIT_ATTR_ROLL_THROUGH)) {
        return TRUE;
    }
    if (entry->attributes & HIT_ATTR_BERO) {
        return TRUE;
    }
    if (entry->attributes & HIT_ATTR_NPC_ONLY) {
        return FALSE;
    }
    if (!(entry->attributes & HIT_ATTR_MOBJ)) {
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

/**
 * Partner ray filter used while Koops is in his shell ability:
 * skips any surface with attribute 0x800005 (checked before the 0x80080 override, unlike chkfilter)
 * and passes through hidden blocks.
 */
static BOOL chkfilterNokotaro(HitCheckQuery* query, HitEntry* entry) {
    MapObjectEntry* mobj;
    marioGetPtr();
    if (entry->attributes & (HIT_ATTR_SHELL_THROUGH | HIT_ATTR_NPC_ONLY | HIT_ATTR_BERO)) {
        return FALSE;
    }
    if (entry->attributes & (HIT_ATTR_SHIP_ONLY | HIT_ATTR_ROLL_THROUGH)) {
        return TRUE;
    }
    if (!(entry->attributes & HIT_ATTR_MOBJ)) {
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

/**
 * Casts a ray with the filter for the current partner state (Yoshi, Koops' shell, or the default)
 * and on a hit returns the hit object with the distance,
 * hit position (both rounded to 0.001) and surface normal; NULL on a miss.
 */
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
    } else if (party->currentMemberId == PARTY_KOOPS && (party->flags & PARTY_FLAG_IS_BEING_USED)) {
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

/**
 * Exported partyHitCheckInl: a partner-filtered ray cast returning the hit object,
 * rounded distance and position, and normal. Used by party.c, party_motion.c, party_move.c and evt_party.c.
 */
HitEntry* partyHitCheck(PartyEntry* party, Vec* pos, Vec* dir, Vec* outPos, Vec* outNrm, f32* dist) {
    return partyHitCheckInl(party, pos, dir, outPos, outNrm, dist);
}

/** Slope in degrees of the surface with normal n (0 for a flat floor). */
static inline f32 partyNrmTo(Vec* n) {
    return angleABf(0.0f, 0.0f, sqrtf(n->x * n->x + n->z * n->z), -n->y);
}

/** Slope in degrees of a surface normal, by value (party.c and party_move.c use it for appear spots). */
f32 partyNrmToAngle(Vec n) {
    return angleABf(0.0f, 0.0f, sqrtf(n.x * n.x + n.z * n.z), -n.y);
}

/** Stores the floor tilt from normal n in the partner's groundRot (x and z, in degrees) and returns the slope. */
// Original name and size (0x1f0) from the JP demo map (UNUSED there: every call is inlined).
static f32 ABCtoAngleY(PartyEntry* party, Vec n) {
    party->groundRot.x = (s32)(angleABf(0.0f, 0.0f, 100.0f * n.z, 100.0f * n.y) - 180.0f);
    party->groundRotY = 0.0f;
    party->groundRotZ = (s32)(angleABf(0.0f, 0.0f, 100.0f * n.x, 100.0f * n.y) - 180.0f);
    return partyNrmTo(&n);
}

/**
 * Per-frame ground check for a walking partner (the Goombella, Bobbery, Ms. Mowz and Yoshi ability code,
 * party.c and party_motion.c call it):
 * snaps her to the floor, sets the grounded and shallow-water flags (attribute 0x100),
 * starts a fall when there is no floor (attribute 0x200 counts as none),
 * and starts the step-up motion when the floor just ahead is 5 or more units higher on a gentle slope.
 */
void partyChkGnd(PartyEntry* party) {
    s32 attr;
    HitEntry* hit;
    f32 stepY, stepDrop, stepAngle;

    if (!(party->flags & PARTY_FLAG_FORCE_FALL)) {
        attr = 0;
        hit = partySearchGround(party, 11.0f, party->velY);
        if (hit) {
            attr = hitGetAttr(hit);
            if (attr & HIT_ATTR_WATER_HAZARD) {
                hit = NULL;
            }
        }
        party->hitobjStandOn = hit;
        party->flags &= ~PARTY_FLAG_IN_SHALLOW_WATER;
        if (hit == NULL) {
            party->flags &= ~PARTY_FLAG_IS_GROUNDED;
            partyChgMot(party, PARTY_MOTION_FALL);
            party->gravity = -1.0f;
        } else {
            party->flags |= PARTY_FLAG_IS_GROUNDED;
            if (attr & HIT_ATTR_SHALLOW_WATER) {
                party->flags |= PARTY_FLAG_IN_SHALLOW_WATER;
            } else {
                party->flags &= ~PARTY_FLAG_IN_SHALLOW_WATER;
            }
        }
        party->position.y = party->groundY;
        if (!(party->flags & (PARTY_FLAG_IS_JUMPING | PARTY_FLAG_IS_FALLING))) {
            partyChkFrontStep(party, 11.0f, &stepY, &stepDrop, &stepAngle);
            if (stepY - party->position.y >= 5.0f && party->groundSlope <= 30.0f) {
                party->stepY = stepY;
                if (party->moveSpeed) {
                    partyChgMot(party, PARTY_MOTION_UPSTAIRS);
                }
            }
        }
    }
}

#if VERSION <= VERSION_DEMO_JP
static inline HitEntry* partyHitCheckGnd(PartyEntry* party, Vec* pos, Vec* dir, f32 dist, f32* outY, Vec* outNrm) {
    HitCheckQuery query;
    Vec d = *dir;
    HitEntry* hit;
    f32 rf;

    query.targetPos = *pos;
    query.targetDir = d;
    query.targetDistance = dist;
    if (yoshiGetStatus()) {
        hit = hitCheckVecFilter(&query, chkfilterYoshi);
    } else if (party->currentMemberId == PARTY_KOOPS && (party->flags & PARTY_FLAG_IS_BEING_USED)) {
        hit = hitCheckVecFilter(&query, chkfilterNokotaro);
    } else {
        hit = hitCheckVecFilter(&query, chkfilter);
    }
    if (!hit) {
        return hit;
    }
    if (query.hitPos.y >= 0.0f) {
        rf = 0.5f;
    } else {
        rf = -0.5f;
    }
    *outY = (s32)((1000.0f * query.hitPos.y) + rf) / 1000.0f;
    outNrm->x = query.hitNormal.x;
    outNrm->y = query.hitNormal.y;
    outNrm->z = query.hitNormal.z;
    return hit;
}

/* partyChkGnd's ground search: writes the party's own groundY/dropDepth */
HitEntry* partySearchGround(PartyEntry* party, f32 range, f32 depth) {
    HitEntry* hit;
    s32 i;
    f32 nsx, probeY, nsz, len, sx, sz, px, baseY, pz, ox, oz, gy, rad, d;
    Vec pos;
    Vec dir;
    Vec outNrm;
#else
HitEntry* partySearchGround(PartyEntry* party, f32 range, f32 depth) {
    HitEntry* hit;
    HitEntry* best;
    s32 i;
    f32 probeY, nsz, nsx, len, bestY, sx, sz, px, baseY, pz, gy, ox, oz, rad, d;
    Vec pos;
    Vec dir;
    Vec outPos, outNrm;
    f32 dist;
#endif

    if (party->flags & PARTY_FLAG_FORCE_FALL) {
        return NULL;
    }
    {
        Vec zero = {0.0f, 0.0f, 0.0f};
        party->groundRot = zero;
    }
    party->groundSlope = 0.0f;
    range = __fabsf(range);
    range += 0.1f;
#if VERSION <= VERSION_DEMO_JP
    d = 0.75f * partyGetHeight(party);
#else
    if (marioGetPtr()->currMotionId == MARIO_MOTION_YOSHI) {
        d = 5.0f;
    } else {
        d = 0.75f * partyGetHeight(party);
    }
#endif
    party->groundY = party->position.y;
    party->dropDepth = -1.0f;
    if (party->moveSpeed) {
        rad = party->targetDir;
    } else {
        rad = partyToMovedir(party, party->displayDirection);
    }
    rad = 3.1415927f * rad / 180.0f;
    nsz = (f32)sin(rad);
#if VERSION <= VERSION_DEMO_JP
    sx = 0.5f * ((party->width - 0.05f) * nsz);
    nsz = (f32)cos(rad);
    sz = 0.5f * ((party->width - 0.05f) * -nsz);
#else
    sx = 0.375f * (party->width * nsz);
    nsz = (f32)cos(rad);
    sz = 0.375f * (party->width * -nsz);
#endif
    nsx = -sx;
    nsz = -sz;
    baseY = party->position.y;
    probeY = baseY + d;
    len = d + range;
    {
        Vec d0 = {0.0f, -1.0f, 0.0f};
        dir = d0;
    }
#if VERSION <= VERSION_DEMO_JP
    px = party->position.x;
    pz = party->position.z;
#else
    best = NULL;
    px = party->position.x;
    pz = party->position.z;
    bestY = -10000.0f;
#endif
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
#if VERSION <= VERSION_DEMO_JP
        hit = partyHitCheckGnd(party, &pos, &dir, len, &gy, &outNrm);
#else
        dist = len;
        hit = partyHitCheckInl(party, &pos, &dir, &outPos, &outNrm, &dist);
        gy = outPos.y;
#endif
        if (hit) {
            if (ABCtoAngleY(party, outNrm) >= 50.0f) {
                hit = NULL;
            }
        }
        if (hit) {
            party->groundSlope = ABCtoAngleY(party, outNrm);
            if (gy < baseY && __fabsf(gy - baseY) >= 2.0f) {
                if (__fabsf(gy - baseY) > __fabsf(depth)) {
                    party->dropDepth = __fabsf(baseY - gy);
#if VERSION <= VERSION_DEMO_JP
                } else {
                    party->groundY = gy;
                    party->dropDepth = 0.0f;
                    return hit;
                }
            } else if (__fabsf(gy - baseY) <= 11.0f) {
                party->groundY = gy;
                party->dropDepth = 0.0f;
                return hit;
            }
        }
    }
    return NULL;
}

/* the early demos' second ground search reports the floor height and drop depth through out-pointers and takes the first
   acceptable probe instead of the highest one */
HitEntry* _partySearchGround(PartyEntry* party, f32* outY, f32* outDepth, f32 range, f32 depth) {
    HitEntry* hit;
    s32 i;
    f32 nsx, probeY, nsz, len, sx, sz, px, baseY, pz, ox, oz, gy, rad, d;
    Vec pos;
    Vec dir;
    Vec outNrm;

    if (party->flags & PARTY_FLAG_FORCE_FALL) {
        return NULL;
    }
    {
        Vec zero = {0.0f, 0.0f, 0.0f};
        party->groundRot = zero;
    }
    party->groundSlope = 0.0f;
    range = __fabsf(range);
    range += 0.1f;
    d = 0.75f * partyGetHeight(party);
    *outY = party->position.y;
    *outDepth = -1.0f;
    if (party->moveSpeed) {
        rad = party->targetDir;
    } else {
        rad = partyToMovedir(party, party->displayDirection);
    }
    rad = 3.1415927f * rad / 180.0f;
    nsz = (f32)sin(rad);
    sx = 0.5f * ((party->width - 0.05f) * nsz);
    nsz = (f32)cos(rad);
    sz = 0.5f * ((party->width - 0.05f) * -nsz);
    nsx = -sx;
    nsz = -sz;
    baseY = party->position.y;
    probeY = baseY + d;
    len = d + range;
    {
        Vec d0 = {0.0f, -1.0f, 0.0f};
        dir = d0;
    }
    px = party->position.x;
    pz = party->position.z;
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
        hit = partyHitCheckGnd(party, &pos, &dir, len, &gy, &outNrm);
        if (hit) {
            if (ABCtoAngleY(party, outNrm) >= 50.0f) {
                hit = NULL;
            }
        }
        if (hit) {
            party->groundSlope = ABCtoAngleY(party, outNrm);
            if (gy < baseY && __fabsf(gy - baseY) >= 2.0f) {
                if (__fabsf(gy - baseY) > __fabsf(depth)) {
                    *outDepth = __fabsf(baseY - gy);
                } else {
                    *outY = gy;
                    *outDepth = 0.0f;
                    return hit;
                }
            } else if (__fabsf(gy - baseY) <= 11.0f) {
                *outY = gy;
                *outDepth = 0.0f;
                return hit;
            }
        }
#else
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
    if (best) {
        party->groundY = bestY;
        party->dropDepth = 0.0f;
        return best;
#endif
    }
    return NULL;
}

/**
 * Probes the floor 0.265 of the partner's width ahead, from her height above (half her height in the early demos).
 * On a floor less than 2 units lower, or a higher floor, returns it with *outY = floor height and *outDist = 0;
 * on a lower floor returns NULL with *outDist = the drop. Also sets her floor tilt and slope.
 */
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
#if VERSION <= VERSION_DEMO_JP
    h = 0.5f * partyGetHeight(party);
#else
    h = partyGetHeight(party);
#endif
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
        // p is built first and copied into pos (the copy is in the retail code).
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

/**
 * Moves *out by half of the movement (vx, vz) with its component into the wall normal n removed,
 * so a partner pushing into a wall slides along it.
 * Original name and size (0x38) from the JP demo map (UNUSED there: every call is inlined).
 */
static void reflect2(Vec* out, Vec n, f32 vx, f32 vz) {
#if VERSION >= VERSION_JP
    // Never read; unused in the original too: its stack slot and initializer template are in the retail code
    Vec unused = {0.0f, 0.0f, 0.0f};
#endif
    f32 dot = vx * n.x + vz * n.z;
    out->x += 0.5f * (vx - dot * n.x);
    out->z += 0.5f * (vz - dot * n.z);
}

/**
 * Wall probe with Mario's filter (marioHitCheckVec) at 0.75 and 0.5 of the height and step height (11,
 * or 1 when airborne or yoshiGetStatus() is 2; US and later add a 1-unit ray in one corner of gor_02):
 * slides *pos along the nearest wall, or moves it by dist; it stays put when both diagonals are blocked.
 */
#if VERSION >= VERSION_JP
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
#if VERSION >= VERSION_US
    s32 count;
#endif
    HitEntry* hit;
    s32 i;

    sincosf(&sn, &cs, ang);
#if VERSION >= VERSION_US
    count = 3;
#endif
    arr[0] = 0.75f * partyGetHeight(party);
    arr[1] = 0.5f * partyGetHeight(party);
    if ((party->flags & (PARTY_FLAG_IS_JUMPING | PARTY_FLAG_IS_FALLING)) || yoshiGetStatus() == YOSHI_STATUS_FLUTTER) {
        arr[2] = 1.0f;
    } else {
        arr[2] = 11.0f;
#if VERSION >= VERSION_US
        if (!strcmp(gp->currentMapName, "gor_02") && party->position.y >= 120.0f && party->position.x <= 350.0f) {
            arr[3] = 1.0f;
            count = 4;
        }
#endif
    }
    rd = 0.5f * party->width;
    best = NULL;
    maxD = rd + dist;
    bestD = maxD;
    qpos = *pos;
    {
        Vec dir1 = {0.0f, 0.0f, 0.0f};
        dir1.x = sn;
        dir1.z = cs;
        qdir = dir1;
    }
#if VERSION >= VERSION_US
    for (i = 0; i < count; i++) {
#else
    for (i = 0; i < 3; i++) {
#endif
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

        // nn copies bestNrm for the wall slide (the copy is in the retail code).
        nn = bestNrm;
        mx = sn * dist;
        mz = cs * dist;
        nz = nn.z;
        dot = mx * nn.x + mz * nz;
        v.x += 0.5f * (mx - dot * nn.x);
        v.z += 0.5f * (mz - dot * nz);
    }
#if VERSION >= VERSION_US
    // Dead store (both loops below assign hit when count >= 1); its li is in the retail code.
    hit = NULL;
#endif
    sincosf(&sn2, &cs2, ang + 45.0f);

    // Dead first store (the loop sets bestD at the top of each pass); the store is in the retail code.
    bestD = rd;
    qpos = *pos;
    {
        Vec dir2 = {0.0f, 0.0f, 0.0f};
        dir2.x = sn2;
        dir2.z = cs2;
        qdir = dir2;
    }
#if VERSION >= VERSION_US
    for (i = 0; i < count; i++) {
#else
    for (i = 0; i < 3; i++) {
#endif
        bestD = rd;
        qpos.y = pos->y + arr[i];
        hit = marioHitCheckVec(&qpos, &qdir, NULL, NULL, &bestD);
        if (hit) {
            break;
        }
    }

    // This probe loop has no break, so hit keeps the last height's result (the +45 loop above stops at its first hit).
    if (hit) {
#if VERSION >= VERSION_US
        hit = NULL;
        sincosf(&sn2, &cs2, ang - 45.0f);
        for (i = 0; i < count; i++) {
#else
        sincosf(&sn2, &cs2, ang - 45.0f);
        for (i = 0; i < 3; i++) {
#endif
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
}
#endif

#if VERSION <= VERSION_DEMO_JP

/** Early demos only: partner-filtered ray that returns the hit object, the rounded hit distance in *dist and the normal. */
static inline HitEntry* partyHitCheckDistNrm(PartyEntry* party, Vec* pos, Vec* dir, Vec* outNrm, f32* dist) {
    HitCheckQuery query;
    HitEntry* hit;
    f32 rf;

    query.targetPos = *pos;
    query.targetDir = *dir;
    query.targetDistance = *dist;
    if (yoshiGetStatus()) {
        hit = hitCheckVecFilter(&query, chkfilterYoshi);
    } else if (party->currentMemberId == PARTY_KOOPS && (party->flags & PARTY_FLAG_IS_BEING_USED)) {
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
    outNrm->x = query.hitNormal.x;
    outNrm->y = query.hitNormal.y;
    outNrm->z = query.hitNormal.z;
    return hit;
}

/** Early demos only: partner-filtered ray of fixed length that returns the hit object and the surface normal. */
static inline HitEntry* partyHitCheckNrm(PartyEntry* party, Vec* pos, Vec* dir, Vec* outNrm, f32 dist) {
    HitCheckQuery query;
    HitEntry* hit;

    query.targetPos = *pos;
    query.targetDir = *dir;
    query.targetDistance = dist;
    if (yoshiGetStatus()) {
        hit = hitCheckVecFilter(&query, chkfilterYoshi);
    } else if (party->currentMemberId == PARTY_KOOPS && (party->flags & PARTY_FLAG_IS_BEING_USED)) {
        hit = hitCheckVecFilter(&query, chkfilterNokotaro);
    } else {
        hit = hitCheckVecFilter(&query, chkfilter);
    }
    if (!hit) {
        return hit;
    }
    outNrm->x = query.hitNormal.x;
    outNrm->y = query.hitNormal.y;
    outNrm->z = query.hitNormal.z;
    return hit;
}

/**
 * Early-demo wall probe with the current partner filter: tries 0.75 of the height,
 * adds a half-height probe for Yoshi in use, then tries step height (1 airborne, otherwise 11).
 * Stops at the first qualifying hit and slides *pos along it; otherwise moves *pos by dist.
 * Unlike JP and later, it does not run diagonal probes. Used by Yoshi's wall check.
 */
HitEntry* partySearchWallFront(PartyEntry* party, Vec* pos, f32 dist, f32 ang) {
    f32 sn, cs;
    f32 d, dy;
    HitEntry* hit;
    Vec p, nrm;

    marioGetPtr();
    sincosf(&sn, &cs, ang);
    d = 0.5f * party->width + dist;
    dy = 0.75f * partyGetHeight(party);
    {
        Vec dir = {0.0f, 0.0f, 0.0f};
        dir.x = sn;
        dir.z = cs;
        p.x = pos->x;
        p.z = pos->z;
        p.y = pos->y + dy;
        hit = partyHitCheckDistNrm(party, &p, &dir, &nrm, &d);
        if (hit) {
            reflect2(pos, nrm, sn * dist, cs * dist);
            return hit;
        }
        if (party->currentMemberId == PARTY_YOSHI && (party->flags & PARTY_FLAG_IS_BEING_USED)) {
            dy = 0.5f * partyGetHeight(party);
            p.y = pos->y + dy;
            hit = partyHitCheckNrm(party, &p, &dir, &nrm, d);
            if (hit) {
                reflect2(pos, nrm, sn * dist, cs * dist);
                return hit;
            }
        }
        if (party->flags & (PARTY_FLAG_IS_JUMPING | PARTY_FLAG_IS_FALLING)) {
            dy = 1.0f;
        } else {
            dy = 11.0f;
        }
        d = 0.5f * party->width + dist;
        p.y = pos->y + dy;
        hit = partyHitCheckNrm(party, &p, &dir, &nrm, d);
        if (hit) {
            if (partyNrmTo(&nrm) >= 45.0f) {
                reflect2(pos, nrm, sn * dist, cs * dist);
                return hit;
            }
        }
    }
    pos->x += dist * sn;
    pos->z += dist * cs;
    return NULL;
}
#endif

/**
 * The partners' main wall check: a ray ahead at 0.75 of the height,
 * then one at step height (11, or 1 for Bobbery in use) that only counts walls of 45 degrees or more.
 * On a hit slides *pos along the wall and returns it; otherwise moves *pos by dist.
 * JP and later store frontWallDist (-1 none); US and later narrow paper-mode width to 6.
 */
HitEntry* partySearchFrontWall(PartyEntry* party, Vec* pos, f32 dist, f32 ang) {
    f32 sn, cs;
    f32 d, dy, hh;
#if VERSION >= VERSION_US
    f32 w, maxD;
#endif
    HitEntry* hit;
    Vec p, nrm;

    sincosf(&sn, &cs, ang);
#if VERSION >= VERSION_US
    // US and later: in paper mode (PARTY_FLAG_PAPER_MODE) the partner checks walls with a width of 6
    w = party->width;
    if (party->flags & PARTY_FLAG_PAPER_MODE) {
        w = 6.0f;
    }
    d = maxD = 0.5f * w + dist;
#else
    d = 0.5f * party->width + dist;
#endif
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
#if VERSION >= VERSION_JP
            party->frontWallDist = d;
#endif
            reflect2(pos, nrm, sn * dist, cs * dist);
            return hit;
        }
        dy = 11.0f;
        if (party->currentMemberId == PARTY_BOBBERY && (party->flags & PARTY_FLAG_IS_BEING_USED)) {
            dy = 1.0f;
        }
        p = party->position;
#if VERSION >= VERSION_US
        d = maxD;
#else
        d = 0.5f * party->width + dist;
#endif
        p.y += dy;
        hit = partyHitCheckInl(party, &p, &dir, &outPos, &nrm, &d);
        if (hit) {
            if (partyNrmTo(&nrm) >= 45.0f) {
#if VERSION >= VERSION_JP
                party->frontWallDist = d;
#endif
                reflect2(pos, nrm, sn * dist, cs * dist);
                return hit;
            }
        }
    }
#if VERSION >= VERSION_JP
    party->frontWallDist = -1.0f;
#endif
    pos->x += dist * sn;
    pos->z += dist * cs;
    return NULL;
}

/**
 * Ceiling check: casts upward rays from 10 units above four points 0.3 of the width around *pos,
 * stopping at the first that hits within *dist.
 * Leaves *pos at the last probe point and *dist at the ceiling distance (0 if none).
 */
HitEntry* partySearchHead(PartyEntry* party, Vec* pos, f32* dist, f32 angle) {
    f32 lx;
    f32 ly;
    f32 lz;
    f32 best;
    f32 oz;
    f32 ox;
    Vec t;
    f32 s;
    f32 c;

    // FAKE: volatile probe coordinates (plain f32 locals change the object in both versions)
    volatile f32 px;
    volatile f32 py;
    volatile f32 pz;
    f32 d0;
    f32 nz0;
    f32 ny0;
    f32 nx0;
    f32 probeHeight;

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
    HitEntry* hit;
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
    lx = px + oz;
    lz = pz + ox;
    ly = py;
    t = (Vec){lx, ly, lz};
    ty = t.y;
    d0 = best;

    // FAKE: empty barrier (no code) steers scheduling of the 10.0f + t.y argument
    if ((t.z && t.z) && t.z) {
    }
    probeHeight = 10.0f;
    hit = hitCheckFilter(NULL, &hx0, &hy0, &hz0, &d0, &nx0, &ny0, &nz0, t.x, probeHeight + ty, t.z, zero, one, zero);
    if (hit && (best >= d0)) {
        best = d0;
    }
    if (!hit) {
        lx = px - oz;
        ly = py;
        lz = pz - ox;
        t = (Vec){lx, ly, lz};
        ty = t.y;

        // FAKE: empty barrier (no code) steers scheduling of the 10.0f + t.y argument
        if ((t.z && t.z) && t.z) { }
        d1 = best;
        hit = hitCheckFilter(NULL, &hx1, &hy1, &hz1, &d1, &nx1, &ny1, &nz1, t.x, probeHeight + ty, t.z, zero, one, zero);
        if (hit && (best >= d1)) {
            best = d1;
        }
        if (!hit) {
            lx = px + ox;
            ly = py;
            lz = pz + oz;
            t = (Vec){lx, ly, lz};
            ty = t.y;

            // FAKE: empty barrier (no code) steers scheduling of the 10.0f + t.y argument
            if ((t.z && t.z) && t.z) { }
            d2 = best;
            hit = hitCheckFilter(NULL, &hx2, &hy2, &hz2, &d2, &nx2, &ny2, &nz2, t.x, probeHeight + ty, t.z, zero, one, zero);
            if (hit && (best >= d2)) {
                best = d2;
            }
            if (!hit) {
                lx = px - ox;
                ly = py;
                lz = pz - oz;
                t = (Vec){lx, ly, lz};
                ty = t.y;

                // FAKE: empty barrier (no code) steers scheduling of the 10.0f + t.y argument
                if ((t.z && t.z) && t.z) { }
                d3 = best;
                hit = hitCheckFilter(NULL, &hx3, &hy3, &hz3, &d3, &nx3, &ny3, &nz3, t.x, probeHeight + ty, t.z, zero, one, zero);
                if (hit && (best >= d3)) {
                    best = d3;
                }
            }
        }
    }
    if (hit) {
        pos->x = lx;
        pos->y = ly;
        pos->z = lz;
        *dist = best;
    }
    if (!hit) {
        pos->x = lx;
        pos->y = ly;
        pos->z = lz;
        *dist = 0.0f;
    }
    return hit;
}

// FAKE: stand-in that nothing calls; it only places these literals and the int-to-float conversion constant in
// this object's constant pool
void partyHitStrippedPool(f32* f, s32 i) {
    f[0] = 10.0f;
    f[1] = 0.0f;
    f[2] = 1.0f;
    f[3] = 0.5f;
    f[4] = 100.0f;
    f[5] = (f32)i;
}

#if VERSION >= VERSION_US

/** 20-unit partner-filtered ray from *pos along *dir; returns the hit object and its normal in *nrm. */
static inline HitEntry* partyHitNrm(PartyEntry* party, Vec* pos, Vec* dir, Vec* nrm) {
    HitCheckQuery query;
    HitEntry* hit;

    query.targetPos = *pos;
    query.targetDir = *dir;
    query.targetDistance = 20.0f;
    if (yoshiGetStatus()) {
        hit = hitCheckVecFilter(&query, chkfilterYoshi);
    } else if (party->currentMemberId == PARTY_KOOPS && (party->flags & PARTY_FLAG_IS_BEING_USED)) {
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

/**
 * Pushes *pos out of nearby walls with Mario's marioChkWallAround, using the partner's width (6 in paper mode).
 * Built in US and later only.
 */
void partyChkWallAround(PartyEntry* party, Vec* pos, f32 spd, f32 dir, f32 width, f32 height) {
    if (party->flags & PARTY_FLAG_PAPER_MODE) {
        width = 6.0f;
    }
    marioChkWallAround(pos, 1, spd, dir, width, height);
}

/**
 * TRUE when 20-unit rays 10 units up hit walls on both sides of pos (directions 180 and 0,
 * from the centre or half a width aside) whose normals face each other (average angle within 10 of 180).
 * Used by party_motion.c's slit code.
 */
BOOL partyChkBetweenWalls(PartyEntry* party, Vec pos) {
    f32 r = party->width;
    f32 sn, cs;
    f32 ox, oz;
    f32 ang1, ang2;
    f32 rayHeight = 10.0f;
    Vec p;
    Vec n;
    HitEntry* hit1;
    HitEntry* hit2;
    Vec d;

    sincosf(&sn, &cs, toMovedirSimple(270.0f));
    ox = 0.5f * (sn * r);
    oz = 0.5f * (cs * r);
    ang1 = 0.0f;
    ang2 = ang1;
    sincosf(&sn, &cs, toMovedirSimple(180.0f));
    p = pos;
    p.y += rayHeight;
    d = (Vec){sn, 0.0f, cs};
    hit1 = partyHitNrm(party, &p, &d, &n);
    if (hit1) {
        ang1 = angleABf(0.0f, 0.0f, n.x, n.z);
    }
    if (!hit1) {
        hit1 = partyHitNrm(party, &(Vec){pos.x + ox, pos.y + rayHeight, pos.z + oz}, &d, &n);
        if (hit1) {
            ang1 = angleABf(0.0f, 0.0f, n.x, n.z);
        }
    }
    if (!hit1) {
        hit1 = partyHitNrm(party, &(Vec){pos.x - ox, pos.y + rayHeight, pos.z - oz}, &d, &n);
        if (hit1) {
            ang1 = angleABf(0.0f, 0.0f, n.x, n.z);
        }
    }
    sincosf(&sn, &cs, toMovedirSimple(0.0f));
    p = pos;
    p.y += rayHeight;
    d = (Vec){sn, 0.0f, cs};
    hit2 = partyHitNrm(party, &p, &d, &n);
    if (hit2) {
        ang2 = angleABf(0.0f, 0.0f, n.x, n.z);
    }
    if (!hit2) {
        hit2 = partyHitNrm(party, &(Vec){pos.x + ox, pos.y + rayHeight, pos.z + oz}, &d, &n);
        if (hit2) {
            ang2 = angleABf(0.0f, 0.0f, n.x, n.z);
        }
    }
    if (!hit2) {
        hit2 = partyHitNrm(party, &(Vec){pos.x - ox, pos.y + rayHeight, pos.z - oz}, &d, &n);
        if (hit2) {
            ang2 = angleABf(0.0f, 0.0f, n.x, n.z);
        }
    }
    if (hit1 && hit2 && __fabsf(180.0f - 0.5f * (ang1 + ang2)) <= 10.0f) {
        return TRUE;
    }
    return FALSE;
}
#endif
