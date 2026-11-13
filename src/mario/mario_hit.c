// Mario's collision: ground, walls, ceiling and landing checks (marioChkLandon, marioChkOverhead,
// marioChkWallAround), slopes, steps and the hit objects he stands on or pushes.

#include "version.h"
#include "mario/mario.h"
#include "driver/hitdrv.h"
#include "driver/mobjdrv.h"
#include "party/party_vivian.h"
#include "system.h"
#include "mario/mariost.h"
#include "mario/mario_sbr.h"
#include "party.h"
#include <dolphin/types.h>
#include <string.h>
#include <dolphin/mtx.h>
#include "mario/mario_cam.h"
#include "mario/mario_party.h"
#include "mario/mario_balloon.h"
#include "mot/mot_damage.h"
#include "koopa_motion.h"
#include "mot/mot_jabara.h"
#include "mot/mot_roll.h"
#include "mot/mot_ship.h"
#include "koopa_hit.h"
#include "mot/mot_slit.h"
#include "mario/mario_hit.h"
#include "eff/eff_magic1_n64.h"
#include "libc_decls.h"
#include <math.h>
#include <dolphin/pad.h>
#include "sfx_name_word.h"
#include "koopa.h"
#include "pmario_sound.h"
static HitEntry* searchFrontWall(Vec* pos, f32* h, f32 dx, f32 dz, f32 w);
static HitEntry* marioChkFrontStep(f32* y, f32* h, f32* ang);

/** Zero vectors that start the ray position and direction of Mario's collision checks and landing ray. */
static const Vec zv0 = {0.0f, 0.0f, 0.0f};
static const Vec zv1 = {0.0f, 0.0f, 0.0f};

/** Inline copy of marioChkBero (TRUE = Mario passes through the hit), used by the roll, ship and wall-around checks. */
static inline BOOL marioChkBeroInl(HitEntry* hit) {
    s32 attr = hitGetAttr(hit);
    MarioWork* mario = marioGetPtr();
    if ((attr & HIT_ATTR_ROLL_THROUGH) && !(mario->flags & MARIO_FLAG_TUBE_MODE)) {
        return FALSE;
    }
    if (!(attr & HIT_ATTR_SHIP_ONLY)) {
        return (attr & HIT_ATTR_BERO) ? 1 : 0;
    }
    return mario->currMotionId == MARIO_MOTION_SHIP;
}

/** Mario's collision height, or the riding partner's height (partyGetHeight) while he is on Yoshi. */
// Original name from the JP demo's link map (UNUSED there): every call is inlined, so the linker strips it.
f32 marioGetHeight(void) {
    MarioWork* mario = marioGetPtr();
    f32 ph = mario->wPlayerCollisionBox.y;
    if (mario->currMotionId == MARIO_MOTION_YOSHI) {
        PartyEntry* p = partyGetPtr(marioGetPartyId());
        if (p) {
            ph = partyGetHeight(p);
        }
    }
    return ph;
}

/**
 * Eases Mario's collision width (wPlayerCollisionBox.z) by 1 per frame toward its target:
 * the full box width (18 in tube mode) normally, 6 while he is paper-thin.
 * Called every frame from mario.c before the hit checks.
 */
void marioHitCheckWidth(void) {
    MarioWork* mario = marioGetPtr();
    f32 v;
    if (!(mario->flags & MARIO_FLAG_PAPER_MODE)) {
        f32 lim = !(mario->flags & MARIO_FLAG_TUBE_MODE) ? mario->wPlayerCollisionBox.x : 18.0f;
        f32 s = (mario->wPlayerCollisionBox.z += 1.0f);
        if (s > lim) {
            mario->wPlayerCollisionBox.z = lim;
        }
    } else {
        v = mario->wPlayerCollisionBox.z;
        if (v > 6.0f) {
            f32 t = v - 1.0f;
            mario->wPlayerCollisionBox.z = t;
            if (t < 6.0f) {
                mario->wPlayerCollisionBox.z = 6.0f;
            }
        }
        v = mario->wPlayerCollisionBox.z;
        if (v < 6.0f) {
            v += 1.0f;
            mario->wPlayerCollisionBox.z = v;
            if (v > 6.0f) {
                mario->wPlayerCollisionBox.z = 6.0f;
            }
        }
    }
}

/**
 * Ray filter (TRUE = test this hit): tests hits that have none of query->user0's attribute bits
 * or any of user1's.
 * marioHitCheckVec2 passes 0x4 / 0x800000, so only 0x4 surfaces without 0x800000 are skipped.
 */
static BOOL chkFilterAttr2(HitCheckQuery* query, HitEntry* entry) {
    BOOL ret = FALSE;
    s32 attr = entry->attributes;
    if ((attr & query->user0) == 0 || (attr & query->user1) != 0) {
        ret = TRUE;
    }
    return ret;
}

/**
 * Mario's normal ray filter (TRUE = test this hit):
 * always tests 0x80 / 0x80000 hits, skips 0x4 hits unless they also have 0x800000,
 * and skips the map objects of hidden ? Blocks, hidden badge blocks and hidden 10-coin blocks.
 */
static BOOL chkfilterVec(HitCheckQuery* query, HitEntry* entry) {
    MapObjectEntry* mobj;
    if (entry->attributes & (HIT_ATTR_SHIP_ONLY | HIT_ATTR_ROLL_THROUGH)) {
        return TRUE;
    }
    if (!(entry->attributes & HIT_ATTR_BERO) && (entry->attributes & HIT_ATTR_NPC_ONLY)) {
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
 * chkfilterVec for tube mode: 0x80000 hits are skipped like 0x4 ones (unless 0x800000),
 * so the tube rolls through them; hidden blocks are skipped as well.
 */
static BOOL chkfilterVecRoll(HitCheckQuery* query, HitEntry* entry) {
    MapObjectEntry* mobj;
    if (entry->attributes & HIT_ATTR_SHIP_ONLY) {
        return TRUE;
    }
    if (!(entry->attributes & HIT_ATTR_BERO) && (entry->attributes & (HIT_ATTR_NPC_ONLY | HIT_ATTR_ROLL_THROUGH))) {
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
 * chkfilterVec while Mario hides in Vivian's shadow:
 * 0x4, 0x800 (spike) and 0x80000 hits are skipped unless they have 0x800000; hidden blocks are skipped as well.
 */
static BOOL chkfilterVecVivian(HitCheckQuery* query, HitEntry* entry) {
    MapObjectEntry* mobj;
    if ((entry->attributes & (HIT_ATTR_NPC_ONLY | HIT_ATTR_SPIKES | HIT_ATTR_ROLL_THROUGH)) && !(entry->attributes & HIT_ATTR_BERO)) {
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
 * Casts a ray for Mario with the filter for his current state (Vivian's shadow, tube mode or normal).
 * On a hit, *dist, outPos (rounded to 0.001) and outNrm are filled in; returns the hit or NULL.
 * The core of every check in this file.
 */
static inline HitEntry* marioHitCheckVecInl(Vec* pos, Vec* dir, Vec* outPos, Vec* outNrm, f32* dist) {
    HitCheckQuery query;
    HitEntry* hit;
    f32 rf;

    query.targetPos = *pos;
    query.targetDir = *dir;
    query.targetDistance = *dist;
    if (marioGetPtr()->currMotionId == MARIO_MOTION_VIVIAN) {
        hit = hitCheckVecFilter(&query, chkfilterVecVivian);
    } else if (marioGetPtr()->flags & MARIO_FLAG_TUBE_MODE) {
        hit = hitCheckVecFilter(&query, chkfilterVecRoll);
    } else {
        hit = hitCheckVecFilter(&query, chkfilterVec);
    }
    if (!hit) {
        return NULL;
    }
    if (query.targetDistance >= 0.0f) {
        rf = 0.5f;
    } else {
        rf = -0.5f;
    }
    *dist = (s32)((1000.0f * query.targetDistance) + rf) / 1000.0f;
    if (outPos) {
        *outPos = query.hitPos;
        if (outPos->x >= 0.0f) {
            rf = 0.5f;
        } else {
            rf = -0.5f;
        }
        outPos->x = (s32)((1000.0f * outPos->x) + rf) / 1000.0f;
        if (outPos->y >= 0.0f) {
            rf = 0.5f;
        } else {
            rf = -0.5f;
        }
        outPos->y = (s32)((1000.0f * outPos->y) + rf) / 1000.0f;
        if (outPos->z >= 0.0f) {
            rf = 0.5f;
        } else {
            rf = -0.5f;
        }
        outPos->z = (s32)((1000.0f * outPos->z) + rf) / 1000.0f;
    }
    if (outNrm) {
        *outNrm = query.hitNormal;
    }
    return hit;
}

/**
 * Out-of-line marioHitCheckVecInl for the plane, slit and tube motions,
 * Bowser (koopa_hit.c) and partners (party_hit.c).
 */
HitEntry* marioHitCheckVec(Vec* pos, Vec* dir, Vec* outPos, Vec* outNrm, f32* dist) {
    return marioHitCheckVecInl(pos, dir, outPos, outNrm, dist);
}

/** marioHitCheckVecInl with chkFilterAttr2: skips 0x4 surfaces that lack 0x800000. */
// Original name from the JP demo's link map (UNUSED there): every call is inlined, so the linker strips it.
// It keeps `inline`: as a plain function -inline auto leaves it out of line here (the link changes).
inline HitEntry* marioHitCheckVec2(Vec* pos, Vec* dir, Vec* outPos, Vec* outNrm, f32* dist) {
    HitCheckQuery query;
    HitEntry* hit;
    f32 rf;

    query.targetPos = *pos;
    query.targetDir = *dir;
    query.targetDistance = *dist;
    query.user0 = HIT_ATTR_NPC_ONLY;
    query.user1 = HIT_ATTR_BERO;
    hit = hitCheckVecFilter(&query, chkFilterAttr2);
    if (!hit) {
        return NULL;
    }
    if (query.targetDistance >= 0.0f) {
        rf = 0.5f;
    } else {
        rf = -0.5f;
    }
    *dist = (s32)((1000.0f * query.targetDistance) + rf) / 1000.0f;
    if (outPos) {
        *outPos = query.hitPos;
        if (outPos->x >= 0.0f) {
            rf = 0.5f;
        } else {
            rf = -0.5f;
        }
        outPos->x = (s32)((1000.0f * outPos->x) + rf) / 1000.0f;
        if (outPos->y >= 0.0f) {
            rf = 0.5f;
        } else {
            rf = -0.5f;
        }
        outPos->y = (s32)((1000.0f * outPos->y) + rf) / 1000.0f;
        if (outPos->z >= 0.0f) {
            rf = 0.5f;
        } else {
            rf = -0.5f;
        }
        outPos->z = (s32)((1000.0f * outPos->z) + rf) / 1000.0f;
    }
    if (outNrm) {
        *outNrm = query.hitNormal;
    }
    return hit;
}

/**
 * Scalar wrapper of marioHitCheckVecInl: casts Mario's ray from (x, y, z) along (dx, dy, dz) for up to *dist
 * and returns the hit position, normal and distance through the pointers. Used by the plane and roll motions.
 */
HitEntry* marioHitCheck(f32* ox, f32* oy, f32* oz, f32* dist, f32* nx, f32* ny, f32* nz, f32 x, f32 y, f32 z, f32 dx,
                        f32 dy, f32 dz) {
    Vec outPos, outNrm;
    Vec pos = zv0;
    Vec dir = zv1;
    f32 d;
    HitEntry* hit;
    d = *dist;
    pos.x = x;
    pos.y = y;
    pos.z = z;
    dir.x = dx;
    dir.y = dy;
    dir.z = dz;
    hit = marioHitCheckVecInl(&pos, &dir, &outPos, &outNrm, &d);
    *dist = d;
    *ox = outPos.x;
    *oy = outPos.y;
    *oz = outPos.z;
    *nx = outNrm.x;
    *ny = outNrm.y;
    *nz = outNrm.z;
    return hit;
}

/**
 * The ceiling ray of marioSearchHead and marioSearchHead_jabara:
 * while Mario is not falling (or in the Spring Jump's state 1) it uses marioHitCheckVec2,
 * which tests every surface except 0x4 ones without 0x800000; unlike the normal filter (chkfilterVec)
 * it does not skip the hidden ? / badge / 10-coin blocks, so his head finds them on the way up.
 * Otherwise it uses the normal filters.
 */
static HitEntry* marioHitCheck2(Vec* pos, Vec* dir, Vec* outPos, Vec* outNrm, f32* dist) {
    HitEntry* hit;
    MarioWork* mario = marioGetPtr();
    f32 d = *dist;
    if (mario->currMotionId != MARIO_MOTION_JABARA) {
        if (!(mario->flags & MARIO_FLAG_IS_FALLING)) {
            hit = marioHitCheckVec2(pos, dir, outPos, outNrm, &d);
        } else {
            hit = marioHitCheckVecInl(pos, dir, outPos, outNrm, &d);
        }
    } else if (marioGetJabaraState() == JABARA_STATE_RISING) {
        hit = marioHitCheckVec2(pos, dir, outPos, outNrm, &d);
    } else {
        hit = marioHitCheckVecInl(pos, dir, outPos, outNrm, &d);
    }
    *dist = d;
    return hit;
}

/**
 * Forgets the hit object Mario stands on (hitobjStandOn), and Bowser's ride object when playing as him.
 * Called when a jump starts (mot_jump.c).
 */
void clear_hitobj_ride(void) {
    MarioWork* mario = marioGetPtr();
    mario->hitobjStandOn = NULL;
    if (mario->characterId == CHARACTER_BOWSER) {
        kpaClearHitobjRide();
    }
}

/**
 * TRUE if Mario passes through this hit instead of being stopped:
 * 0x800000 hits, 0x80 hits only in boat motion, and 0x80000 hits never outside tube mode.
 * Used by the wall, tube, balloon and Yoshi checks; the front-wall check keeps moving Mario into such a hit
 * and records it as the wall he pushes.
 * Named after bero (map exits), whose walls Mario walks into: dou_03 gives its exit wall s_bero 0x80,
 * so only the boat passes.
 */
BOOL marioChkBero(HitEntry* hit) {
    s32 attr = hitGetAttr(hit);
    MarioWork* mario = marioGetPtr();
    if ((attr & HIT_ATTR_ROLL_THROUGH) && !(mario->flags & MARIO_FLAG_TUBE_MODE)) {
        return FALSE;
    }
    if (!(attr & HIT_ATTR_SHIP_ONLY)) {
        return (attr & HIT_ATTR_BERO) ? 1 : 0;
    }
    return mario->currMotionId == MARIO_MOTION_SHIP;
}

/** Slides a move (vx, vz) along a wall with normal (nx, nz): the move minus its part into the wall, at half speed. */
// Original name from the JP demo's link map (UNUSED there): every call is inlined, so the linker strips it.
static void _reflect(f32 vx, f32 nx, f32 vz, f32 nz, f32* mx, f32* mz) {
    f32 dot = vx * nx + vz * nz;
    *mx = 0.5f * (vx - dot * nx);
    *mz = 0.5f * (vz - dot * nz);
}

/** _ABCtoAngleY for a normal vector. */
static f32 ABCtoAngleY(Vec* nrm) {
    MarioWork* mario = marioGetPtr();
    mario->groundRot.x = (s32)(100.0f * (angleABf(0.0f, 0.0f, 100.0f * nrm->z, 100.0f * nrm->y) - 180.0f) + 0.5f) / 100.0f;
    mario->groundRot.y = 0.0f;
    mario->groundRot.z = (s32)(100.0f * (angleABf(0.0f, 0.0f, 100.0f * nrm->x, 100.0f * nrm->y) - 180.0f) + 0.5f) / 100.0f;
    return angleABf(0.0f, 0.0f, sqrtf(nrm->x * nrm->x + nrm->z * nrm->z), -nrm->y);
}

/**
 * Turns a surface normal (nx, ny, nz) into Mario's groundRot tilt (x and z, degrees rounded to 0.01)
 * and returns the surface's slope in degrees from horizontal (0 = flat floor, 90 = wall, 180 = flat ceiling).
 */
// Original names and sizes (_ABCtoAngleY 0x234, ABCtoAngleY 0x210) from the JP demo map (UNUSED there: every
// call is inlined).
static f32 _ABCtoAngleY(f32 nx, f32 ny, f32 nz) {
    MarioWork* mario = marioGetPtr();
    mario->groundRot.x = (s32)(100.0f * (angleABf(0.0f, 0.0f, 100.0f * nz, 100.0f * ny) - 180.0f) + 0.5f) / 100.0f;
    mario->groundRot.y = 0.0f;
    mario->groundRot.z = (s32)(100.0f * (angleABf(0.0f, 0.0f, 100.0f * nx, 100.0f * ny) - 180.0f) + 0.5f) / 100.0f;
    return angleABf(0.0f, 0.0f, sqrtf(nx * nx + nz * nz), -ny);
}

// FAKE: inline size limit raised for the rest of the file; without it the object changes
#pragma inline_max_size(1000)

/**
 * Tube mode on the ground: casts down from 10 above Mario
 * and stores the floor's tilt in roll_grx / roll_grz for the rolling tube (mot_roll.c).
 * Inlined into marioChkGnd.
 */
static inline void marioChkGndRoll(MarioWork* mario) {
    HitEntry* hit;
    f32 gz, gx;
    Vec pos, outPos, outNrm;
    f32 dist = 47.0f;
    mario->baseHeight = mario->basePos.y - mario->position.y;
    pos = mario->position;
    pos.y += 10.0f;
    {
        Vec dir = {0.0f, -1.0f, 0.0f};
        hit = marioHitCheckVecInl(&pos, &dir, &outPos, &outNrm, &dist);
    }
    if (hit) {
        gx = (s32)(100.0f * (angleABf(0.0f, 0.0f, 100.0f * outNrm.z, 100.0f * outNrm.y) - 180.0f) + 0.5f) / 100.0f;
        gz = (s32)(100.0f * (angleABf(0.0f, 0.0f, 100.0f * outNrm.x, 100.0f * outNrm.y) - 180.0f) + 0.5f) / 100.0f;
        angleABf(0.0f, 0.0f, sqrtf(outNrm.x * outNrm.x + outNrm.z * outNrm.z), -outNrm.y);
    }
    if (hit) {
        roll_grx = gx;
        roll_grz = gz;
    }
}

/**
 * Casts a ray straight down from Mario's head (half height in tube mode) for up to 1000 units
 * and stores the floor's height in groundY (-3000 if none).
 * Returns the floor hit, which callers keep in hitobjUnder.
 */
HitEntry* marioSearchUnder(void) {
    Vec pos, outPos, outNrm;
    f32 d = 1000.0f;
    HitEntry* hit;
    f32 h;
    MarioWork* mario = marioGetPtr();
    h = mario->wPlayerCollisionBox.y;
    if (mario->flags & MARIO_FLAG_TUBE_MODE) {
        h *= 0.5f;
    }
    pos = mario->position;
    pos.y += h;
    {
        Vec dir = {0.0f, -1.0f, 0.0f};
        hit = marioHitCheckVecInl(&pos, &dir, &outPos, &outNrm, &d);
    }
    mario->groundY = hit ? outPos.y : -3000.0f;
    return hit;
}

/**
 * Finds the floor below a point: casts down from 37 above (x, y, z) for up to 1037 units,
 * writes its height to *out (-1 if none) and returns the hit.
 * Used by evt_mario.c and evt_party.c to place Mario and partners on the ground.
 */
HitEntry* searchUnder2(f32* out, f32 x, f32 y, f32 z) {
    Vec pos2, outPos, outNrm;
    HitEntry* hit;
    Vec pos = {0.0f, 0.0f, 0.0f};
    f32 d = 1037.0f;
    pos.y = y;
    pos.x = x;
    pos.z = z;
    pos2 = pos;
    pos2.y += 37.0f;
    {
        Vec dir = {0.0f, -1.0f, 0.0f};
        hit = marioHitCheckVecInl(&pos2, &dir, &outPos, &outNrm, &d);
    }
    *out = hit ? outPos.y : -1.0f;
    return hit;
}

/** FALSE when floors are disabled for Mario or he is at y >= 5000 (out of the map), so marioChkGnd skips the floor. */
static inline BOOL marioChkGndOk(void) {
    MarioWork* mario = marioGetPtr();
    if (mario->flags & MARIO_FLAG_FLOORS_DISABLED) {
        return FALSE;
    }
    if (mario->position.y >= 5000.0f) {
        return FALSE;
    }
    return TRUE;
}

/**
 * Mario's per-frame ground check while on the ground (mario.c, koopa.c, peach.c, the Spring Jump):
 * finds the floor he stands on, starts a fall when it is gone,
 * hurts him on 0x200 (water hazard) and 0x800 (spike) floors, steps him up 5-11 unit steps in front,
 * and handles the boat, Bowser and tube-mode variants. Skipped while he is in Vivian's shadow.
 */
void marioChkGnd(void) {
    f32 b, y, c;
    f32 sy, sb, sc;
    f32 d;
    HitEntry* hit;
    MarioWork* mario = marioGetPtr();
    if (vivianGetStatus() == VIVIAN_STATUS_HIDDEN || vivianGetStatus() == VIVIAN_STATUS_SINKING || vivianGetStatus() == VIVIAN_STATUS_RISING) {
        mario->hitobjStandOn = marioSearchGround(&y, &b, &c, 37.0f, 0.0f);
        mario->hitobjUnder = marioSearchUnder();
        return;
    }
    mario->hitobjStandOn = NULL;
    if (mario->characterId == CHARACTER_BOWSER) {
        kpaClearHitobjRide();
    }
    mario->hitobjUnder = marioSearchUnder();
    if (mario->flags & (MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING)) {
        return;
    }
    if (!marioChkGndOk()) {
        return;
    }
    if (!(mario->flags & MARIO_FLAG_FORCE_WALL)) {
        if (mario->flags & MARIO_FLAG_TUBE_MODE) {
            hit = marioSearchGroundRoll(&y, &b, &c, 11.0f, mario->jumpVelocityY);
            if (hit) {
                mario->hitobjStandOn = hit;
                mario->groundY = (s32)(100.0f * y + 0.5f) / 100.0f;
            }
        } else {
            if (mario->characterId == CHARACTER_BOWSER) {
                hit = kpaSearchGround(11.0f, &y, &b, &c);
            } else {
                hit = marioSearchGround(&y, &b, &c, 11.0f, mario->jumpVelocityY);
            }
            if (hit) {
                mario->hitobjStandOn = hit;
                mario->groundY = (s32)(100.0f * y + 0.5f) / 100.0f;
            }
        }
    } else {
        y = mario->position.y;
        if (mario->characterId == CHARACTER_BOWSER) {
            if (mario->airTimer == 0) {
                marioChgMot(MARIO_MOTION_FALL);
                marioSetFallPara();
                return;
            }
        } else {
            if (mario->airTimer == 0) {
                marioChgMot(MARIO_MOTION_FALL);
                marioSetFallPara();
                return;
            }
        }
    }
    if (mario->currMotionId == MARIO_MOTION_SHIP) {
        if (marioChkShipMoveMode()) {
            if (!mario->hitobjStandOn) {
                marioShipChgFall();
            } else {
                mario->flags &= ~MARIO_FLAG_IS_FALLING;
            }
        }
        return;
    }
    if (mario->currMotionId != MARIO_MOTION_DAMAGE && mario->hitobjStandOn && (hitGetAttr(mario->hitobjStandOn) & HIT_ATTR_WATER_HAZARD)) {
#if VERSION <= VERSION_DEMO_JP
        if (mario->jumpVelocityY == 0.0f) {
#else
        if (mario->jumpVelocityY >= 0.0f) {
#endif
            marioSetFallPara();
        }
        set_damage_root_ypos(mario->position.y);
        marioChgMot(MARIO_MOTION_DAMAGE);
        return;
    }
    if (mario->hitobjStandOn && (hitGetAttr(mario->hitobjStandOn) & HIT_ATTR_SPIKES)) {
        if (vivianGetStatus() != VIVIAN_STATUS_HIDDEN && vivianGetStatus() != VIVIAN_STATUS_SINKING && vivianGetStatus() != VIVIAN_STATUS_RISING) {
            set_damage_root_ypos(mario->position.y);
            marioChgMot(MARIO_MOTION_DAMAGE_TOGE);
        }
        return;
    }
    if (mario->airTimer == 0) {
        if (!(mario->flags & MARIO_FLAG_TUBE_MODE)) {
            hit = marioChkFrontStep(&sy, &sb, &sc);
            d = sy - mario->position.y;
            if (hit && d <= 11.0f && d >= 5.0f && mario->groundSlope <= 30.0f) {
                mario->stepUpY = sy;
                if (mario->baseSpeed) {
                    if (!(mario->dispFlags & MARIO_DISP_FLAG_SLIT)) {
                        marioChgMot(MARIO_MOTION_UPSTAIRS);
                    } else {
                        marioChgMotSub(MARIO_MOTION_UPSTAIRS, 0);
                    }
                    return;
                }
            }
        } else {
            hit = marioChkFrontStep(&sy, &sb, &sc);
            d = sy - mario->position.y;
            if (hit && d <= 11.0f && d >= 5.0f && mario->groundSlope <= 30.0f) {
                mario->stepUpY = sy;
                if (mario->baseSpeed) {
                    roll_upstairs_on();
                    return;
                }
            }
        }
    }
    mario->position.y = y;
    if (mario->characterId == CHARACTER_BOWSER) {
        hit = kpaChkHitobjRide();
    } else {
        hit = mario->hitobjStandOn;
    }
    if (hit) {
        if (mario->flags & MARIO_FLAG_TUBE_MODE) {
            marioChkGndRoll(mario);
        }
#if VERSION <= VERSION_DEMO_JP
        mario->trigFlags |= MARIO_TRIG_FLAG_SHIP_PLANE_EVT_ON;
#endif
        return;
    }
#if VERSION <= VERSION_DEMO_JP
    if (!(mario->flags & (MARIO_FLAG_PAPER_MODE | MARIO_FLAG_TUBE_MODE))) {
#else
    if (!(mario->flags & MARIO_FLAG_TUBE_MODE)) {
#endif
        if (mario->flags & MARIO_FLAG_IS_FALLING) {
            return;
        }
        if (b < 0.0f) {
            marioChgMot(MARIO_MOTION_FALL);
            marioSetFallPara();
        }
        if (b > 0.0f) {
            marioChgMot(MARIO_MOTION_FALL_2);
            marioSetFallPara();
        }
    } else {
        if (mario->flags & MARIO_FLAG_IS_FALLING) {
            return;
        }
        mario->flags &= ~MARIO_FLAG_IS_JUMPING;
        mario->flags |= MARIO_FLAG_IS_FALLING;
        mario->jumpVelocityY = -0.4f;
        mario->jumpAccelerationY = -0.06f;
        mario->jumpJerkY = -0.04f;
        mario->jumpSnapY = -0.02f;
    }
}

/**
 * Catches Mario on a floor during a jump or fall (mario.c, koopa.c, peach.c, after 5 airborne frames):
 * if the floor is up to 10 units above his feet he snaps onto it,
 * his air state is cleared and he changes to standing (Bowser to walking or dashing, with a landing quake),
 * and the camera follows him again.
 */
void marioChkGnd2(void) {
    f32 b, y, c;
    HitEntry* hit;
    f32 d;
    MarioWork* mario = marioGetPtr();
    if (!(mario->flags & MARIO_FLAG_IS_JUMPING)) {
        if (mario->characterId != CHARACTER_BOWSER) {
            hit = marioSearchGround(&y, &b, &c, 0.1f, mario->jumpVelocityY);
        } else {
            hit = kpaSearchGround(0.1f, &y, &b, &c);
        }
        if (hit) {
            d = mario->position.y - y;
            if (d < 0.0f && d >= -10.0f) {
                mario->hitobjStandOn = hit;
                mario->position.y = y;
                mario->flags &= ~(MARIO_FLAG_USE_FORCE_DIRECTION | MARIO_FLAG_STOP_AIR_MOVEMENT);
                mario->airTimer = 0;
                mario->jumpPeakAirTime = 0;
                if (mario->characterId == CHARACTER_BOWSER) {
                    if (mario->controlStickSensitivity || !kpaFireAttackCheck()) {
                        s8 sx = mario->wStickDir1;
                        s8 sy = mario->wStickDir2;
                        if (sx * sx + sy * sy > (55 * 55)) {
                            marioChgMot(MARIO_MOTION_DASH);
                        } else {
                            marioChgMot(MARIO_MOTION_WALK);
                        }
                    } else {
                        marioChgMot(MARIO_MOTION_STAY);
                    }
                    quake_kpaLandOn();
                } else {
                    marioChgMot(MARIO_MOTION_STAY);
                }
                if (mario->characterId != CHARACTER_BOWSER) {
                    if (!marioBgmodeChk()) {
                        camFollowYOn();
                    }
                } else {
                    kpaCamFollow();
                }
            }
        }
    }
}

/**
 * Five-point ground search: the early demos probe at 0.375 of the collision width and return the first accepted floor.
 * JP and later use 0.425 in paper mode (0.375 otherwise) and keep the highest accepted floor.
 * The early demos allow slopes below 75 degrees in paper mode (50 otherwise); JP and later allow 75 in slit display
 * or the hip-drop motion instead (50 otherwise).
 */
HitEntry* marioSearchGround(f32* y, f32* b, f32* c, f32 spd, f32 h) {
    f32 dist;
#if VERSION <= VERSION_DEMO_JP
    f32 yl, sx, sz, x0, y0, z0, a;
    f32 x, z;
    f32 lim;
    f32 sn;
    HitEntry* hit;
    s32 i;
#else
    f32 best, yl, sx, sz, x0, y0, z0, a;
    f32 x, z;
    f32 lim;
    f32 dir;
    f32 sn;
    HitEntry* found;
    s32 i;
    HitEntry* hit;
#endif
    MarioWork* mario = marioGetPtr();
    mario->groundRot.x = 0.0f;
    mario->groundRot.y = 0.0f;
    mario->groundRot.z = 0.0f;
    mario->groundSlope = 0.0f;
    if (mario->flags & MARIO_FLAG_FORCE_WALL) {
        *y = mario->position.y;
        return NULL;
    }
    if (mario->flags & MARIO_FLAG_FLOORS_DISABLED) {
        *y = mario->position.y;
        return NULL;
    }
    *y = mario->position.y;
    spd = __fabsf(spd);
    spd += 0.01f;
    yl = 5.0f + __fabsf(h);
    *b = -1.0f;
#if VERSION <= VERSION_DEMO_JP
    if (mario->baseSpeed) {
        a = revise360(mario->directionView - mario->targetYaw);
    } else {
        a = toMovedir(mario->displayDirection);
    }
    a = (3.1415927f * a) / 180.0f;
    sn = (f32)sin(a);
    sx = 0.375f * (mario->wPlayerCollisionBox.x * sn);
    sn = (f32)cos(a);
    sz = 0.375f * (mario->wPlayerCollisionBox.x * -sn);
    y0 = mario->position.y;
    {
        Vec dir;
#else
#if VERSION >= VERSION_US
    if (mario->currMotionId != MARIO_MOTION_HAMMER_2) {
        if (mario->baseSpeed) {
            a = revise360(mario->directionView - mario->targetYaw);
        } else {
            a = toMovedir(mario->displayDirection);
        }
    } else {
        a = toMovedir(mario->savedDisplayDirection);
    }
#else
    if (mario->baseSpeed) {
        a = revise360(mario->directionView - mario->targetYaw);
    } else {
        a = toMovedir(mario->displayDirection);
    }
#endif
    if (mario->flags & MARIO_FLAG_PAPER_MODE) {
        a = (3.1415927f * a) / 180.0f;
        sn = (f32)sin(a);
        sx = 0.425f * (mario->wPlayerCollisionBox.x * sn);
        sn = (f32)cos(a);
        sz = 0.425f * (mario->wPlayerCollisionBox.x * -sn);
    } else {
        a = (3.1415927f * a) / 180.0f;
        sn = (f32)sin(a);
        sx = 0.375f * (mario->wPlayerCollisionBox.x * sn);
        sn = (f32)cos(a);
        sz = 0.375f * (mario->wPlayerCollisionBox.x * -sn);
    }
    y0 = mario->position.y;
    {
    Vec dir = {0.0f, -1.0f, 0.0f};
#endif
    Vec outPos;
    Vec outNrm;
    x0 = mario->position.x;
#if VERSION <= VERSION_DEMO_JP
    z0 = mario->position.z;
#else
    found = NULL;
    z0 = mario->position.z;
    best = -10000.0f;
#endif
    for (i = 0; i < 5; i++) {
        if (i == 0) {
            z = x = 0.0f;
        } else if (i == 1) {
            x = sx;
            z = sz;
        } else if (i == 2) {
            x = -sz;
            z = sx;
        } else if (i == 3) {
            x = sz;
            z = -sx;
        } else if (i == 4) {
            x = -sx;
            z = -sz;
        }
        {
            Vec v = {0.0f, 0.0f, 0.0f};
#if VERSION <= VERSION_DEMO_JP
            dir = (Vec){0.0f, -1.0f, 0.0f};
#endif
            v.x = x0 + x;
            v.y = y0 + yl;
            v.z = z0 + z;
            dist = yl + spd;
            hit = marioHitCheckVecInl(&v, &dir, &outPos, &outNrm, &dist);
        }
        if (hit) {
#if VERSION <= VERSION_DEMO_JP
            if (mario->flags & MARIO_FLAG_PAPER_MODE) {
#else
            if (mario->dispFlags & MARIO_DISP_FLAG_SLIT) {
                lim = 75.0f;
            } else if (mario->currMotionId == MARIO_MOTION_HIP) {
#endif
                lim = 75.0f;
            } else {
                lim = 50.0f;
            }
            if (ABCtoAngleY(&outNrm) >= lim) {
                hit = NULL;
            }
        }
        if (hit) {
            mario->groundSlope = ABCtoAngleY(&outNrm);
            if (outPos.y < y0 && __fabsf(outPos.y - y0) >= 2.0f) {
                if (__fabsf(outPos.y - y0) > __fabsf(h)) {
                    *b = __fabsf(y0 - outPos.y);
                    *c = angleABf(x0, z0, outPos.x, outPos.z);
#if VERSION <= VERSION_DEMO_JP
                } else {
                    *y = outPos.y;
#else
                } else if (outPos.y > best) {
                    *y = outPos.y;
                    best = outPos.y;
#endif
                    *b = 0.0f;
                    *c = angleABf(x0, z0, outPos.x, outPos.z);
                    marioUpdateCamCollision(outPos);
#if VERSION <= VERSION_DEMO_JP
                    return hit;
                }
            } else if (__fabsf(outPos.y - y0) <= 11.0f) {
                *y = outPos.y;
#else
                    found = hit;
                }
            } else if (__fabsf(outPos.y - y0) <= 11.0f && outPos.y > best) {
                *y = outPos.y;
                best = outPos.y;
#endif
                *b = 0.0f;
                *c = angleABf(x0, z0, outPos.x, outPos.z);
                marioUpdateCamCollision(outPos);
#if VERSION <= VERSION_DEMO_JP
                return hit;
#else
                found = hit;
#endif
            }
        }
    }
    }
#if VERSION >= VERSION_JP
    if (found) {
        return found;
    }
#endif
    return NULL;
}

typedef struct LandonRay {
    Vec pos;
    Vec dir;
} LandonRay;

/** marioHitCheckVecInl taking the ray as a LandonRay (position and direction), for the landing checks. */
static inline HitEntry* landonHitCheck(LandonRay* ray, Vec* outPos, Vec* outNrm, f32* dist) {
    HitCheckQuery query;
    HitEntry* hit;
    f32 rf;

    query.targetPos = ray->pos;
    query.targetDir = ray->dir;
    query.targetDistance = *dist;
    if (marioGetPtr()->currMotionId == MARIO_MOTION_VIVIAN) {
        hit = hitCheckVecFilter(&query, chkfilterVecVivian);
    } else if (marioGetPtr()->flags & MARIO_FLAG_TUBE_MODE) {
        hit = hitCheckVecFilter(&query, chkfilterVecRoll);
    } else {
        hit = hitCheckVecFilter(&query, chkfilterVec);
    }
    if (!hit) {
        return NULL;
    }
    if (query.targetDistance >= 0.0f) {
        rf = 0.5f;
    } else {
        rf = -0.5f;
    }
    *dist = (s32)((1000.0f * query.targetDistance) + rf) / 1000.0f;
    if (outPos) {
        *outPos = query.hitPos;
        if (outPos->x >= 0.0f) {
            rf = 0.5f;
        } else {
            rf = -0.5f;
        }
        outPos->x = (s32)((1000.0f * outPos->x) + rf) / 1000.0f;
        if (outPos->y >= 0.0f) {
            rf = 0.5f;
        } else {
            rf = -0.5f;
        }
        outPos->y = (s32)((1000.0f * outPos->y) + rf) / 1000.0f;
        if (outPos->z >= 0.0f) {
            rf = 0.5f;
        } else {
            rf = -0.5f;
        }
        outPos->z = (s32)((1000.0f * outPos->z) + rf) / 1000.0f;
    }
    if (outNrm) {
        *outNrm = query.hitNormal;
    }
    return hit;
}

/**
 * Tube ground search: the early demos test three points and return the first accepted floor;
 * on flat ground the centre stays put and the two side probes move 3 units ahead.
 * JP and later test four points on nearly flat ground or three on slopes, skip slopes of 75 to 105 degrees,
 * and keep the highest accepted floor.
 */
HitEntry* marioSearchGroundRoll(f32* y, f32* b, f32* c, f32 spd, f32 h) {
#if VERSION <= VERSION_DEMO_JP
    LandonRay ray;
    Vec nrm;
    Vec hp;
    f32 dist;
    f32 lim, hh, x0, y0, z0, a;
    f32 ox, oy, oz, nx, ny, nz;
    f32 x, z;
    f32 sn;
    s32 end;
    f32 rz, rx;
    s32 i;
#else
    f32 dist;
    f32 lim, best, hh, x0, y0, z0, a, r;
    f32 sn;
    HitEntry* found;
    s32 i;
    s32 end;
#endif
    HitEntry* hit;
    MarioWork* mario = marioGetPtr();
    s32 gz;
    mario->groundRot.x = 0.0f;
    mario->groundRot.y = 0.0f;
    mario->groundRot.z = 0.0f;
    if (mario->flags & MARIO_FLAG_FORCE_WALL) {
        *y = mario->position.y;
        return NULL;
    }
    if (mario->flags & MARIO_FLAG_FLOORS_DISABLED) {
        *y = mario->position.y;
        return NULL;
    }
#if VERSION <= VERSION_DEMO_JP
    spd = __fabsf(spd);
    spd += 0.01f;
    hh = mario->wPlayerCollisionBox.y;
    if (mario->groundSlope) {
        lim = 10.0f;
    } else {
        lim = 2.0f;
    }
    *y = mario->position.y;
    *b = -1.0f;
    y0 = mario->position.y;
    x0 = mario->position.x;
    z0 = mario->position.z;
    if (mario->groundSlope) {
        end = 3;
    } else {
        end = 6;
    }
    if (mario->groundSlope) {
        i = 0;
    } else {
        i = 3;
    }
    for (; i < end; i++) {
        dist = hh + spd;
        switch (i) {
            case 0:
                z = x = 0.0f;
                break;
            case 1:
                a = (3.1415927f * (90.0f + mario->directionView)) / 180.0f;
                x = 9.0f * (f32)sin(a);
                sn = (f32)cos(a);
                z = 9.0f * -sn;
                break;
            case 2:
                a = (3.1415927f * (mario->directionView - 90.0f)) / 180.0f;
                x = 9.0f * (f32)sin(a);
                sn = (f32)cos(a);
                z = 9.0f * -sn;
                break;
            case 3:
                z = x = 0.0f;
                break;
            case 4:
                a = (3.1415927f * (90.0f + mario->directionView)) / 180.0f;
                x = 9.0f * (f32)sin(a);
                sn = (f32)cos(a);
                z = 9.0f * -sn;
                movePos(&x, &z, 3.0f, mario->directionView);
                break;
            case 5:
                a = (3.1415927f * (mario->directionView - 90.0f)) / 180.0f;
                x = 9.0f * (f32)sin(a);
                sn = (f32)cos(a);
                z = 9.0f * -sn;
                movePos(&x, &z, 3.0f, mario->directionView);
                break;
            case 6:
                a = (3.1415927f * (90.0f + mario->directionView)) / 180.0f;
                x = 9.0f * (f32)sin(a);
                sn = (f32)cos(a);
                z = 9.0f * -sn;
                movePos(&x, &z, 3.0f, 180.0f + mario->directionView);
                break;
            case 7:
                a = (3.1415927f * (mario->directionView - 90.0f)) / 180.0f;
                x = 9.0f * (f32)sin(a);
                sn = (f32)cos(a);
                z = 9.0f * -sn;
                movePos(&x, &z, 3.0f, 180.0f + mario->directionView);
                break;
        }

        // FAKE: the dead guard keeps rx/rz their own webs, so x and z load before the volatile template copy
        // (rz declared before rx so the loads colour f1/f0 as in the demo)
        rx = x;
        rz = z;
        gz = 0;
        if (gz != 0) {
            rx = 0.0f;
            rz = 0.0f;
        }
        ray = *(volatile LandonRay*)&zv0; // FAKE: same 24-byte template copy as marioLandonChk_old
        ray.pos.x = x0 + rx;
        ray.pos.y = y0 + hh;
        ray.pos.z = z0 + rz;
        ray.dir.x = 0.0f;
        ray.dir.y = -1.0f;
        ray.dir.z = 0.0f;
        hit = landonHitCheck(&ray, &hp, &nrm, &dist);
        ox = hp.x;
        oy = hp.y;
        oz = hp.z;
        nx = nrm.x;
        ny = nrm.y;
        nz = nrm.z;
        if (hit) {
            mario->groundSlope = _ABCtoAngleY(nx, ny, nz);
            if (oy < y0 && __fabsf(oy - y0) >= lim) {
                if (__fabsf(oy - y0) > __fabsf(h)) {
                    *b = __fabsf(y0 - oy);
                    *c = angleABf(x0, z0, ox, oz);
                } else {
                    *y = oy;
                    *b = 0.0f;
                    *c = angleABf(x0, z0, ox, oz);
                    return hit;
                }
            } else if (__fabsf(oy - y0) <= 11.0f) {
                *y = oy;
                *b = 0.0f;
                *c = angleABf(x0, z0, ox, oz);
                return hit;
            }
        }
    }
#else
    hh = mario->wPlayerCollisionBox.y;
    spd = __fabsf(spd);
    found = NULL;
    *y = mario->position.y;
    *b = -1.0f;
    spd += 0.01f;
    best = -10000.0f;
    {
        Vec dir = {0.0f, -1.0f, 0.0f};
        Vec outPos;
        Vec outNrm;
        f32 x, z;
        y0 = mario->position.y;
        x0 = mario->position.x;
        z0 = mario->position.z;
        if (__fabsf(mario->groundSlope) < 0.01f) {
            lim = 2.0f;
            i = 0;
            end = 4;
        } else {
            lim = 10.0f;
            i = 4;
            end = 7;
        }
        for (; i < end; i++) {
            switch (i) {
                case 0:
                    z = x = 0.0f;
                    break;
                case 1:
                    a = (3.1415927f * (90.0f + mario->directionView)) / 180.0f;
                    x = 8.6399994f * (f32)sin(a);
                    sn = (f32)cos(a);
                    z = 8.6399994f * -sn;
                    break;
                case 2:
                    a = (3.1415927f * (mario->directionView - 90.0f)) / 180.0f;
                    x = 8.6399994f * (f32)sin(a);
                    sn = (f32)cos(a);
                    z = 8.6399994f * -sn;
                    break;
                case 3:
                    z = x = 0.0f;
                    movePos(&x, &z, 3.0f, mario->directionView);
                    break;
                case 4:
                    z = x = 0.0f;
                    break;
                case 5:
                    a = (3.1415927f * (90.0f + mario->directionView)) / 180.0f;
                    x = 8.6399994f * (f32)sin(a);
                    sn = (f32)cos(a);
                    z = 8.6399994f * -sn;
                    movePos(&x, &z, 3.0f, mario->directionView);
                    break;
                case 6:
                    a = (3.1415927f * (mario->directionView - 90.0f)) / 180.0f;
                    x = 8.6399994f * (f32)sin(a);
                    sn = (f32)cos(a);
                    z = 8.6399994f * -sn;
                    movePos(&x, &z, 3.0f, mario->directionView);
                    break;
            }
            {
                Vec v = {0.0f, 0.0f, 0.0f};
                v.x = x0 + x;
                v.y = y0 + hh;
                v.z = z0 + z;
                dist = hh + spd;
                hit = marioHitCheckVecInl(&v, &dir, &outPos, &outNrm, &dist);
            }
            if (hit) {
                r = ABCtoAngleY(&outNrm);
                if (r <= 105.0f && r >= 75.0f) {
                    continue;
                }
                mario->groundSlope = r;
                if (outPos.y < y0 && __fabsf(outPos.y - y0) >= lim) {
                    if (__fabsf(outPos.y - y0) > __fabsf(h)) {
                        *b = __fabsf(y0 - outPos.y);
                        *c = angleABf(x0, z0, outPos.x, outPos.z);
                    } else if (outPos.y > best) {
                        *y = outPos.y;
                        best = outPos.y;
                        *b = 0.0f;
                        *c = angleABf(x0, z0, outPos.x, outPos.z);
                        found = hit;
                    }
                } else if (__fabsf(outPos.y - y0) <= 11.0f && outPos.y > best) {
                    *y = outPos.y;
                    best = outPos.y;
                    *b = 0.0f;
                    *c = angleABf(x0, z0, outPos.x, outPos.z);
                    found = hit;
                }
            }
        }
    }
    if (found) {
        return found;
    }
#endif
    return NULL;
}

/**
 * Hurts Mario on spikes (toge): if the wall in front (within half his width),
 * the wall that stopped him or the ceiling above has attribute 0x800, he changes to the spike damage motion.
 * Called each frame from mario.c; not in Vivian's shadow.
 */
void marioChkToge(void) {
    MarioWork* mario = marioGetPtr();
    s32 attr;
    if (vivianGetStatus() == VIVIAN_STATUS_HIDDEN || vivianGetStatus() == VIVIAN_STATUS_SINKING || vivianGetStatus() == VIVIAN_STATUS_RISING) {
        return;
    }
    if (mario->hitobjFront != NULL) {
        attr = hitGetAttr(mario->hitobjFront);
        if (mario->currMotionId != MARIO_MOTION_DAMAGE_TOGE && (attr & HIT_ATTR_SPIKES) && mario->frontWallDist < 0.5f * mario->wPlayerCollisionBox.x) {
            set_damage_root_ypos(mario->position.y);
            marioChgMot(MARIO_MOTION_DAMAGE_TOGE);
        }
    }
    if (mario->hitobjWall != NULL) {
        attr = hitGetAttr(mario->hitobjWall);
        if (mario->currMotionId != MARIO_MOTION_DAMAGE_TOGE && (attr & HIT_ATTR_SPIKES)) {
            set_damage_root_ypos(mario->position.y);
            marioChgMot(MARIO_MOTION_DAMAGE_TOGE);
        }
    }
    if (mario->hitobjHead != NULL) {
        attr = hitGetAttr(mario->hitobjHead);
        if (mario->currMotionId != MARIO_MOTION_DAMAGE_TOGE && (attr & HIT_ATTR_SPIKES)) {
            set_damage_root_ypos(mario->position.y);
            marioChgMot(MARIO_MOTION_DAMAGE_TOGE);
        }
    }
}

/**
 * Landing check used by the jump, fall, damage, ship and Bowser motions:
 * searches for the floor within d below Mario (the tube's own search in tube mode,
 * marioLandonChk_old for Bowser), writes its height to *a and returns the hit.
 */
HitEntry* marioChkLandon(f32* a, f32 d) {
    f32 b, c;
    HitEntry* ret;
    MarioWork* mario = marioGetPtr();
    if (mario->characterId != CHARACTER_BOWSER) {
        if (!(mario->flags & MARIO_FLAG_TUBE_MODE)) {
            ret = marioSearchGround(a, &b, &c, d, d);
        } else {
            ret = marioSearchGroundRoll(a, &b, &c, d, d);
        }
    } else {
        ret = marioLandonChk_old(a, d);
    }
    return ret;
}

/**
 * The older landing check, still used for Bowser (where it hands over to kpaSearchGround):
 * five rays down from 3/4 of Mario's height, accepting a floor only
 * where it lies at the ray's reported distance; *outY gets its height.
 */
HitEntry* marioLandonChk_old(f32* outY, f32 spd) {
    LandonRay ray;
    Vec nrm;
    Vec hp;
    f32 hy;
    f32 nx;
    f32 ny;
    f32 nz;
    f32 dx;
    f32 dz;
    f32 mone;
    f32 zero;
    f32 d;
    f32 dist;
    f32 noz;
    f32 nox;
    f32 top;
    f32 ystart;
    f32 px;
    f32 ox;
    f32 oz;
    f32 hh;
    f32 range;
    f32 sn;
    f32 pz;
    f32 py;
    s32 i;
    HitEntry* hit;
    MarioWork* mario = marioGetPtr();
    f32 a;

    mario->groundRot.x = 0.0f;
    mario->groundRot.y = 0.0f;
    mario->groundRot.z = 0.0f;
    mario->groundSlope = 0.0f;
    if (mario->flags & MARIO_FLAG_FORCE_WALL) {
        *outY = mario->position.y;
        return NULL;
    }
    if (mario->flags & MARIO_FLAG_FLOORS_DISABLED) {
        *outY = mario->position.y;
        return NULL;
    }
    range = 0.01f + __fabsf(spd);
    hh = 0.75f * mario->wPlayerCollisionBox.y;
    if (mario->baseSpeed) {
        a = revise360(mario->directionView - mario->targetYaw);
    } else {
        a = toMovedir(mario->displayDirection);
    }
    a = (3.1415927f * a) / 180.0f;
    sn = (f32)sin(a);
    ox = 0.25f * (mario->wPlayerCollisionBox.x * sn);
    sn = (f32)cos(a);
    oz = 0.25f * (mario->wPlayerCollisionBox.x * -sn);
    px = mario->position.x;
    py = mario->position.y;
    pz = mario->position.z;
    if (mario->characterId == CHARACTER_BOWSER) {
        f32 gy, gb, gc;
        hit = kpaSearchGround(11.0f, &gy, &gb, &gc);
        *outY = gy;
        return hit;
    }
    noz = -oz;
    nox = -ox;
    zero = 0.0f;
    mone = -1.0f;

    // FAKE: dead re-definition (i is 0) gives zero/mone two reaching defs in IRO pass 0, so they stay named vregs instead of being constant-propagated
    i = 0;
    if (i != 0) {
        zero = 0.0f;
        mone = -1.0f;
    }
    top = hh + range;
    ystart = py + hh;
    for (i = 0; i < 5; i++) {
        if (i == 0) {
            dx = 0.0f;
            dz = dx;
        } else if (i == 1) {
            dx = ox;
            dz = oz;
        } else if (i == 2) {
            // Probe 2 uses (-oz, oz), reusing the z offset; the head/floor search rings use (-z, x) here.
            dx = noz;
            dz = oz;
        } else if (i == 3) {
            dx = oz;
            dz = nox;
        } else if (i == 4) {
            dx = nox;
            dz = noz;
        }
        ray = *(volatile LandonRay*)&zv0; // FAKE: copies a 24-byte zero template; a real (LandonRay){0} adds a .rodata object and shifts every pool
        ray.pos.x = px + dx;
        ray.pos.y = ystart;
        ray.pos.z = pz + dz;
        ray.dir.x = zero;
        ray.dir.y = mone;
        ray.dir.z = zero;
        dist = top;
        hit = landonHitCheck(&ray, &hp, &nrm, &dist);
        hy = hp.y;
        nx = nrm.x;
        ny = nrm.y;
        nz = nrm.z;
        if (hit) {
            mario->groundSlope = _ABCtoAngleY(nx, ny, nz);
            d = (s32)(100.0f * ((ystart - hy) - dist) + 0.5f) / 100.0f;
            if (__fabsf(d) < 0.1f) {
                *outY = (s32)(100.0f * hy + 0.5f) / 100.0f;
                return hit;
            }
        }
    }
    return NULL;
}

/** Five-point landing probe with a vertical ofs: the early demos use the older LandonRay form; JP and later use marioHitCheckVecInl. */
HitEntry* marioChkLandon2(f32* y, f32 spd, f32 ofs) {
#if VERSION <= VERSION_DEMO_JP
    LandonRay ray;
    Vec nrm;
    Vec hp;
    f32 hy;
    f32 nx;
    f32 ny;
    f32 nz;
    f32 dx;
    f32 dz;
    f32 mone;
    f32 zero;
    f32 d;
    f32 dist;
    f32 noz;
    f32 nox;
    f32 top;
    f32 ystart;
    f32 ox;
    f32 oz;
    f32 px;
    f32 hh;
    f32 range;
    f32 sn;
    f32 pz;
    f32 py;
    f32 x, z;
    s32 i;
    HitEntry* hit;
    MarioWork* mario = marioGetPtr();
    f32 a;
#else
    Vec dir;
    Vec outPos;
    Vec outNrm;
    f32 nsx;
    f32 d1;
    f32 pyl;
    f32 yl;
    f32 d0;
    f32 sx;
    f32 sz;
    f32 x0;
    f32 z0;
    f32 a;
    f32 sn;
    f32 dist;
    f32 py;
    f32 x, z;
    s32 i;
    HitEntry* hit;
    MarioWork* mario = marioGetPtr();
#endif
    mario->groundRot.x = 0.0f;
    mario->groundRot.y = 0.0f;
    mario->groundRot.z = 0.0f;
    mario->groundSlope = 0.0f;
    if (mario->flags & MARIO_FLAG_FORCE_WALL) {
        *y = mario->position.y;
        return NULL;
    }
    if (mario->flags & MARIO_FLAG_FLOORS_DISABLED) {
        *y = mario->position.y;
        return NULL;
    }
#if VERSION <= VERSION_DEMO_JP
    range = 0.1f + __fabsf(spd);
    hh = 0.5f * mario->wPlayerCollisionBox.y;
#else
    d0 = 0.1f + __fabsf(spd);
    yl = 0.5f * mario->wPlayerCollisionBox.y;
#endif
    if (mario->baseSpeed) {
        a = mario->directionView;
    } else {
        a = toMovedir(mario->displayDirection);
    }
    a = (3.1415927f * a) / 180.0f;
    sn = (f32)sin(a);
#if VERSION <= VERSION_DEMO_JP
    ox = 0.25f * (mario->wPlayerCollisionBox.x * sn);
    sn = (f32)cos(a);
    oz = 0.25f * (mario->wPlayerCollisionBox.x * -sn);
    nox = -ox;
    top = hh + range;
    py = mario->position.y + ofs;
    ystart = py + hh;
    px = mario->position.x;
    pz = mario->position.z;
    noz = -oz;
    zero = 0.0f;
    mone = -1.0f;
    for (i = 0; i < 5; i++) {
        if (i == 0) {
            dx = 0.0f;
            dz = dx;
        } else if (i == 1) {
            dx = ox;
            dz = oz;
        } else if (i == 2) {
            dx = noz;
            dz = oz;
        } else if (i == 3) {
            dx = oz;
            dz = nox;
        } else if (i == 4) {
            dx = nox;
            dz = noz;
        }
        ray = *(volatile LandonRay*)&zv0; // FAKE: copies a 24-byte zero template; a real (LandonRay){0} adds a .rodata object and shifts every pool
        ray.pos.x = px + dx;
        ray.pos.y = ystart;
        ray.pos.z = pz + dz;
        ray.dir.x = zero;
        ray.dir.y = mone;
        ray.dir.z = zero;
        dist = top;
        hit = landonHitCheck(&ray, &hp, &nrm, &dist);
        hy = hp.y;
        nx = nrm.x;
        ny = nrm.y;
        nz = nrm.z;
        if (hit) {
            mario->groundSlope = _ABCtoAngleY(nx, ny, nz);
            d = (s32)(100.0f * ((ystart - hy) - dist) + 0.5f) / 100.0f;
            if (__fabsf(d) < 0.1f) {
                *y = (s32)(100.0f * hy + 0.5f) / 100.0f;
                return hit;
            }
        }

        // FAKE: dead re-definition (gz is 0) at the loop end gives zero/mone two reaching defs in IRO pass 0, so they stay named
        // vregs (callee-saved order f27/f28) instead of being constant-propagated; the same guard before the loop shifts the
        // prepass schedule so box.x*-sn lands in f1 instead of f0
        {
            s32 gz = 0;
            if (gz != 0) {
                zero = 0.0f;
                mone = -1.0f;
#else
    sx = 0.25f * (mario->wPlayerCollisionBox.x * sn);
    sn = (f32)cos(a);
    sz = 0.25f * (mario->wPlayerCollisionBox.x * -sn);
    nsx = -sx;
    d1 = yl + d0;
    py = mario->position.y + ofs;
    pyl = py + yl;
    x0 = mario->position.x;
    z0 = mario->position.z;
    {
        dir = (Vec){0.0f, -1.0f, 0.0f};
        for (i = 0; i < 5; i++) {
            if (i == 0) {
                z = x = 0.0f;
            } else if (i == 1) {
                x = sx;
                z = sz;
            } else if (i == 2) {
                x = -sz;
                z = sz;
            } else if (i == 3) {
                x = sz;
                z = nsx;
            } else if (i == 4) {
                x = nsx;
                z = -sz;
            }
            {
                Vec v = {0.0f, 0.0f, 0.0f};
                v.x = x0 + x;
                v.y = pyl;
                v.z = z0 + z;
                dist = d1;
                hit = marioHitCheckVecInl(&v, &dir, &outPos, &outNrm, &dist);
            }
            if (hit) {
                mario->groundSlope = ABCtoAngleY(&outNrm);
                if (__fabsf((s32)(100.0f * ((pyl - outPos.y) - dist) + 0.5f) / 100.0f) < 0.1f) {
                    *y = outPos.y;
                    return hit;
                }
#endif
        }
    }
    }
    return NULL;
}

/**
 * TRUE if the ceiling hit is a Super Block (MOBJ_PowerUpBlock), Save Block or Heart Block (free or paid):
 * bumping one stops Mario's air movement in marioChkOverhead.
 */
// Original name from the JP demo's link map (UNUSED there): every call is inlined, so the linker strips it.
BOOL chk_special_mobj(HitEntry* hit) {
    MapObjectEntry* mobj;
    if (!hit) {
        return FALSE;
    }
    if (!(hitGetAttr(hit) & HIT_ATTR_MOBJ)) {
        return FALSE;
    }
    mobj = mobjHitObjPtrToPtr(hit);
    if (!strcmp(mobj->animName, "MOBJ_PowerUpBlock") || !strcmp(mobj->animName, "MOBJ_SaveBlock") ||
        !strcmp(mobj->animName, "MOBJ_RecoveryBlock") || !strcmp(mobj->animName, "MOBJ_CoinTakenRecoveryBlock")) {
        return TRUE;
    }
    return FALSE;
}

// FAKE: stand-in that nothing calls (the linker strips it); it only places 18.0f in this object's constant pool
#if VERSION >= VERSION_JP
void marioHitStrippedPool2(f32* f) {
    f[0] = 18.0f;
}
#endif

f32 marioChkOverhead(void) {
#if VERSION >= VERSION_JP
    // pa..pd are never read, in the original too: their stack slots and initializer templates are in the retail code
    Vec pa = {0.0f, 0.0f, 0.0f};
    Vec pb = {0.0f, -1.0f, 0.0f};
    Vec pc = {0.0f, 0.0f, 0.0f};
    Vec pd = {0.0f, 0.0f, 0.0f};
#endif
    Vec pos;
    s32 hj;
    f32 h;
#if VERSION <= VERSION_DEMO_JP
    f32 d;
#endif
    MarioWork* mario = marioGetPtr();
    if (mario->currMotionId == MARIO_MOTION_JUMP_3) {
        return mario->position.y;
    }
    if (mario->currMotionId == MARIO_MOTION_JABARA) {
        pos = mario->position;
        h = mario->wPlayerCollisionBox.y;
        if (!jabaraNoHitChk()) {
            h += 5.0f;
            mario->hitobjHead = marioSearchHead_jabara(&pos, &h, &hj, 0.0f);
            if (!mario->hitobjHead) {
#if VERSION <= VERSION_DEMO_JP
                mario->hitobjHead = marioSearchHead(&pos, &h, d);
#else
                mario->hitobjHead = marioSearchHead(&pos, &h, (mario->flags & MARIO_FLAG_IS_JUMPING) ? mario->jumpVelocityY : 10.0f);
#endif
            }
        }
    } else if (mario->flags & MARIO_FLAG_TUBE_MODE) {
        pos = mario->position;
        mario->hitobjHead = marioSearchHead_roll(&pos, &h, 40.0f);
    } else {
        f32 d;
        pos = mario->position;
        h = mario->wPlayerCollisionBox.y;
        d = (mario->flags & MARIO_FLAG_IS_JUMPING) ? mario->jumpVelocityY : 10.0f;
        if (mario->characterId == CHARACTER_BOWSER) {
            mario->hitobjHead = kpaSearchHead(d, &pos, &h);
        } else {
            mario->hitobjHead = marioSearchHead(&pos, &h, d);
        }
    }
    mario->position.x = pos.x;
    if (mario->hitobjHead) {
        if (chk_special_mobj(mario->hitobjHead)) {
            mario->baseSpeed = 0.0f;
            mario->flags |= MARIO_FLAG_STOP_AIR_MOVEMENT;
        }
        mario->flags |= MARIO_FLAG_IS_TOUCHING_CEILING;
        if (mario->characterId == CHARACTER_BOWSER) {
            marioItemGetDisable();
            if (!kpaGetBodyStatus()) {
                marioClearJumpPara();
            }
        }
        pos.y = (s32)(10.0f * (pos.y - marioGetHeight()) + 0.5f) / 10.0f - 1.0f;
        if (!(mario->flags & MARIO_FLAG_IS_FALLING) && pos.y < mario->lastGroundPos.y) {
            pos.y = mario->lastGroundPos.y;
#if VERSION <= VERSION_DEMO_JP
            if (!(mario->flags & MARIO_FLAG_TUBE_MODE) && mario->currMotionId != MARIO_MOTION_SHIP && mario->currMotionId != MARIO_MOTION_VIVIAN && mario->characterId != CHARACTER_BOWSER) {
                if (mario->baseSpeed == 0.0f) {
                    marioChgStayMotion();
                } else {
                    marioChgMot(mario->baseSpeed > mario->baseWalkSpeed ? 2 : 1);
#else
            if (!(mario->flags & MARIO_FLAG_TUBE_MODE) && mario->currMotionId != MARIO_MOTION_SHIP && mario->currMotionId != MARIO_MOTION_PLANE &&
                mario->currMotionId != MARIO_MOTION_VIVIAN && mario->characterId != CHARACTER_BOWSER && !marioChkSlitEnd()) {
                if (mario->baseSpeed == 0.0f) {
                    if (mario->currMotionId != MARIO_MOTION_STAY) {
                        marioChgStayMotion();
                    }
                } else {
#if VERSION >= VERSION_US
                    {
                        /* The second test is redundant, but both compares (a double beq) are in the retail code */
                        s32 m = mario->baseSpeed > mario->baseWalkSpeed ? 2 : 1;
                        if (m != mario->prevMotionId && m != mario->prevMotionId) {
                            marioChgMot(mario->baseSpeed > mario->baseWalkSpeed ? 2 : 1);
                        }
                    }
#else
                    marioChgMot(mario->baseSpeed > mario->baseWalkSpeed ? 2 : 1);
#endif
#endif
                }
            }
        }
        if (mario->currMotionId == MARIO_MOTION_JUMP || mario->currMotionId == MARIO_MOTION_JUMP_STAND) {
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MARIO_HEAD1), &mario->position);
            marioClearJumpPara();
            mario->flags &= ~(MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING | MARIO_FLAG_IS_STEPPING | MARIO_FLAG_HAS_INPUT_JUMP);
            if (mario->characterId != CHARACTER_BOWSER) {
                marioChgMot(MARIO_MOTION_FALL);
                marioSetFallPara();
            }
            mario->position.y = pos.y;
        }
    } else if (!(mario->flags & MARIO_FLAG_TUBE_MODE)) {
        if (h >= 0.0f && h <= 5.0f) {
            mario->flags |= MARIO_FLAG_IS_TOUCHING_CEILING;
        } else {
            mario->flags &= ~MARIO_FLAG_IS_TOUCHING_CEILING;
        }
    } else {
        if (h >= 0.0f && h + mario->wPlayerCollisionBox.y <= 47.0f) {
            mario->flags |= MARIO_FLAG_IS_TOUCHING_CEILING;
        } else {
            mario->flags &= ~MARIO_FLAG_IS_TOUCHING_CEILING;
        }
    }
    return pos.y;
}

/**
 * Ceiling check for the Spring Jump (mot_jabara.c):
 * *sideOut gets which side of Mario the ceiling was hit (-1, 0, 1)
 * and *distOut the gap to a ceiling not yet reached (-1 if none); returns the y Mario must stay under.
 */
f32 marioChkOverheadJabara(s32* sideOut, f32* distOut) {
    Vec pos;
    f32 margin = 5.0f;
    f32 h;
    MarioWork* mario;
    mario = marioGetPtr();
    *sideOut = 0;
    *distOut = -1.0f;
    pos = mario->position;
    h = mario->wPlayerCollisionBox.y;
    if (mario->currMotionId == MARIO_MOTION_JUMP_3) {
        return pos.y;
    }
    if (!jabaraNoHitChk()) {
        h += margin;
        mario->hitobjHead = marioSearchHead_jabara(&pos, &h, sideOut, 0.0f);
        if (!mario->hitobjHead) {
            h = mario->wPlayerCollisionBox.y + margin;
            mario->hitobjHead = marioSearchHead(&pos, &h, 0.0f);
        }
    }
    mario->position.x = pos.x;
    if (mario->hitobjHead) {
        mario->flags |= MARIO_FLAG_IS_TOUCHING_CEILING;
        pos.y = pos.y - mario->wPlayerCollisionBox.y - 0.1f;
        if (mario->currMotionId == MARIO_MOTION_JUMP) {
            marioChgMot(MARIO_MOTION_STAY);
        }
    } else if (h > 0.0f) {
        *distOut = h;
        mario->flags |= MARIO_FLAG_IS_TOUCHING_CEILING;
    } else {
        mario->flags &= ~MARIO_FLAG_IS_TOUCHING_CEILING;
    }
    return pos.y;
}

/** Offset of half of Mario's width (less 0.5) in the direction deg (degrees), for marioSearchHead's side rays. */
static inline void marioSearchHeadOfs(MarioWork* mario, f32 deg, f32* px, f32* pz) {
    f32 r = (3.1415927f * deg) / 180.0f;
    f32 t;
    t = sin(r);
    *px = 0.5f * ((mario->wPlayerCollisionBox.x - 0.5f) * t);
    t = cos(r);
    *pz = 0.5f * ((mario->wPlayerCollisionBox.x - 0.5f) * -t);
}

/**
 * Finds a ceiling over pos: five rays up from half of *h (Mario's height) for up to *h + dd,
 * ignoring walls outside tube mode (slopes of 50-130 degrees normally, 75-105 in paper/slit mode).
 * If his head reaches it, pos->y is set to the ceiling, *h to 0 and the hit is returned;
 * otherwise *h gets the gap (-1 if none).
 * US and later also ignore non-tube ceilings while falling faster than 10 per frame.
 */
HitEntry* marioSearchHead(Vec* pos, f32* h, f32 dd) {
    Vec p;
    Vec d;
    Vec outPos, outNrm;
    f32 dist;
    f32 hh;
    f32 sx, sz;
    f32 a;
    f32 top;
    f32 x, z;
    HitEntry* hit;
    s32 i;
    MarioWork* mario = marioGetPtr();
    if (mario->baseSpeed) {
        hh = mario->directionView;
    } else {
        hh = toMovedir(mario->displayDirection);
    }
    marioSearchHeadOfs(mario, hh, &sx, &sz);
    a = *h;
    hh = 0.5f * a;
    top = a + dd;
    *h = -1.0f;
    for (i = 0; i < 5; i++) {
        if (i == 0) {
            z = x = 0.0f;
        } else if (i == 1) {
            x = sx;
            z = sz;
        } else if (i == 2) {
            x = -sz;
            z = sx;
        } else if (i == 3) {
            x = sz;
            z = -sx;
        } else if (i == 4) {
            x = -sx;
            z = -sz;
        }
        {
            Vec v = {0.0f, 0.0f, 0.0f};
            Vec dir = {0.0f, 1.0f, 0.0f};
            dist = top;
            v.x = pos->x + x;
            v.y = pos->y + hh;
            v.z = pos->z + z;

            // v and dir are copied into p and d before the call (the copies are in the retail code)
            p = v;
            d = dir;
            hit = marioHitCheck2(&p, &d, &outPos, &outNrm, &dist);
        }
        if (hit) {
            f32 r = _ABCtoAngleY(outNrm.x, outNrm.y, outNrm.z);
            if (!(mario->flags & MARIO_FLAG_TUBE_MODE)) {
#if VERSION <= VERSION_DEMO_JP
                f32 lim = (mario->flags & MARIO_FLAG_PAPER_MODE) ? 75.0f : 50.0f;
#else
                f32 lim = (mario->dispFlags & MARIO_DISP_FLAG_SLIT) ? 75.0f : 50.0f;
#endif
                if (r <= 180.0f - lim && r >= lim) {
                    hit = NULL;
                }
            }
        }
        if (mario->flags & MARIO_FLAG_TUBE_MODE) {
            if (hit) {
                outPos.y = (s32)(100.0f * outPos.y + hh) / 100.0f;
                outPos.y = outPos.y - 0.05f * a;
                if (pos->y + a >= outPos.y) {
                    pos->y = outPos.y;
                    *h = 0.0f;
                    return hit;
                }
                *h = outPos.y - pos->y - a;
                return NULL;
            }
        } else {
            if (hit) {
                outPos.y = (s32)(100.0f * outPos.y + hh) / 100.0f;
#if VERSION >= VERSION_US
                if (mario->jumpVelocityY < -10.0f) {
                    *h = outPos.y - pos->y - a;
                    return NULL;
                }
#endif
                if (pos->y + a >= outPos.y) {
                    pos->y = outPos.y;
                    *h = 0.0f;
                    return hit;
                }
                *h = outPos.y - pos->y - a;
                return NULL;
            }
        }
    }
    return NULL;
}

/** cos(a) as f32, for marioSearchHead_roll's side offsets. */
// FAKE: one-purpose cos helper; (f32)cos(a) at the four sites changes the object
inline f32 marioSearchHead_roll_cos(f32 a) {
    return (f32)cos(a);
}

/**
 * Ceiling check for the tube: five rays up from 1 above pos (centre and the tube's front and back corners).
 * On contact Mario is pushed down under the ceiling, *h is 0 and the hit is returned;
 * otherwise *h gets the smallest gap.
 */
HitEntry* marioSearchHead_roll(Vec* pos, f32* h, f32 d) {
    f32 a;
    Vec outPos, outNrm;
    f32 x, z;
    f32 dist;
    f32 base, top;
    Vec* pdir;
    s32 i;
    HitEntry* hit;
    MarioWork* mario = marioGetPtr();
    f32 t;
    *h = -1.0f;
    base = mario->wPlayerCollisionBox.y;
    i = 0;
    top = base + d;
    for (; i < 5; i++) {
        switch (i) {
            case 0:
                z = x = 0.0f;
                break;
            case 1:
                a = (3.1415927f * (90.0f + mario->directionView)) / 180.0f;
                x = 9.0f * (f32)sin(a);
                z = 9.0f * -marioSearchHead_roll_cos(a);
                movePos(&x, &z, 3.0f, mario->directionView);
                break;
            case 2:
                a = (3.1415927f * (mario->directionView - 90.0f)) / 180.0f;
                x = 9.0f * (f32)sin(a);
                z = 9.0f * -marioSearchHead_roll_cos(a);
                movePos(&x, &z, 3.0f, mario->directionView);
                break;
            case 3:
                a = (3.1415927f * (90.0f + mario->directionView)) / 180.0f;
                x = 9.0f * (f32)sin(a);
                z = 9.0f * -marioSearchHead_roll_cos(a);
                movePos(&x, &z, 3.0f, 180.0f + mario->directionView);
                break;
            case 4:
                a = (3.1415927f * (mario->directionView - 90.0f)) / 180.0f;
                x = 9.0f * (f32)sin(a);
                z = 9.0f * -marioSearchHead_roll_cos(a);
                movePos(&x, &z, 3.0f, 180.0f + mario->directionView);
                break;
        }
        {
            Vec v = {0.0f, 0.0f, 0.0f};
            Vec dir = {0.0f, 1.0f, 0.0f};
            v.x = pos->x + x;

            // FAKE: spare pointer to dir; passing &dir directly changes the object
            pdir = &dir;
            v.y = 1.0f + pos->y;
            v.z = pos->z + z;
            dist = top;
            hit = marioHitCheckVecInl(&v, pdir, &outPos, &outNrm, &dist);
        }
        if (hit) {
            if (pos->y + base >= outPos.y) {
                pos->y = outPos.y - base - 1.0f;
                *h = 0.0f;
                return hit;
            }
            t = outPos.y - pos->y - base;
            if (*h < 0.0f) {
                *h = t;
            } else if (*h > t) {
                *h = t;
            }
        }
    }
    return NULL;
}

/**
 * Ceiling check for the Spring Jump: eight rays up along a line across Mario.
 * A 0x40000 hit counts on any ray and *side says which side it was on;
 * any other ceiling only counts on the middle ray. Outputs as marioSearchHead.
 * 0x40000 marks the ceilings Mario can hang from at full charge (mot_jabara).
 */
HitEntry* marioSearchHead_jabara(Vec* pos, f32* h, s32* side, f32 d) {
    Vec v, dir, outPos, outNrm;
    f32 dist;
    f32 base, top, off;
    HitEntry* hit;
    s32 i;
    MarioWork* mario = marioGetPtr();
    *side = 0;
    i = 0;
    base = *h;
    off = -23.7f;
    *h = -1.0f;
    top = base + d;
    dir = (Vec){0.0f, 1.0f, 0.0f};
    for (; i < 8; i++, off += 5.8f) {
        dist = top;
        v.x = pos->x + off * (f32)sin((3.1415927f * mario->targetYaw) / 180.0f);
        v.z = pos->z - off * (f32)cos((3.1415927f * mario->targetYaw) / 180.0f);
        v.y = 0.5f + pos->y;
        hit = marioHitCheck2(&v, &dir, &outPos, &outNrm, &dist);
        if (hit) {
            if (hitGetAttr(hit) & HIT_ATTR_JABARA) {
                if (pos->y + base >= outPos.y) {
                    pos->y = outPos.y;
                    *h = 0.0f;
                    *side = (v.z - pos->z < 0.0f) ? -1 : 1;
                    return hit;
                }
                *h = outPos.y - pos->y - base;
            } else if (i == 3) {
                if (pos->y + base >= outPos.y) {
                    pos->y = outPos.y;
                    *h = 0.0f;
                    *side = 0;
                    return hit;
                }
                *h = outPos.y - pos->y - base;
            }
        }
    }
    return NULL;
}

/**
 * Moves Mario dx units (his speed) in direction dz (degrees) and stops him at walls,
 * from mario.c and koopa.c each frame.
 * Sets hitobjFront / hitobjWall, and hitobjPush once he has pushed a wall for 20 frames (10 for pipes, "dokan",
 * from the US release on). Ends with marioChkWallAround and the Pirate's Grotto spear check (dou10_yoko_yari).
 */
void marioCheckWall(f32 dx, f32 dz) {
    Vec pos;
    f32 h;
    HitEntry* hit;
    MarioWork* mario = marioGetPtr();
    mario->frontWallDist = 1000.0f;
    if (dx && mario->airTimer == 0) {
        if (mario->wallPushDist < dx) {
            mario->wallPushDist = dx;
            mario->wallPushDir = dz;
            mario->wallPushHold = 14;
        } else {
            if (--mario->wallPushHold <= 0) {
                mario->wallPushHold = 0;
                mario->wallPushDist = dx;
                mario->wallPushDir = dz;
            }
        }
        mario->moveDirection = mario->wallPushDir;
    } else {
        mario->wallPushDist = 0.0f;
    }
    if (!dx) {
        marioSearchFrontWall_staying(dz);
        dou10_yoko_yari(&mario->frontWallDist);
        return;
    }
    pos = mario->position;
    mario->hitobjFrontWall = NULL;
    hit = searchFrontWall(&pos, &h, dx, dz, mario->wPlayerCollisionBox.z);
    if (hit) {
        if (mario->hitobjFrontWall) {
            mario->hitobjFront = mario->hitobjFrontWall;
        } else {
            mario->hitobjFront = hit;
        }
        if (marioChkBero(hit)) {
            if (dx) {
                mario->hitobjPush = hit;
                mario->hitobjWall = hit;
            }
            movePos(&mario->position.x, &mario->position.z, dx, dz);
        } else {
            mario->frontWallDist = h;
            mario->hitobjWall = hit;
#if VERSION >= VERSION_US
            {
                s32 lim = strncmp(hitGetName(hit), "dokan", 5) != 0 ? 20 : 10;
                if (++mario->wallPushTimer >= lim) {
                    mario->wallPushTimer = lim;
                    if (!(hitGetAttr(hit) & HIT_ATTR_SHIP_ONLY) ? TRUE : marioGetPtr()->currMotionId == MARIO_MOTION_SHIP) {
                        mario->hitobjPush = hit;
                    }
#else
            if (++mario->wallPushTimer >= 20) {
                mario->wallPushTimer = 20;
                if (!(hitGetAttr(hit) & HIT_ATTR_SHIP_ONLY) ? TRUE : marioGetPtr()->currMotionId == MARIO_MOTION_SHIP) {
                    mario->hitobjPush = hit;
#endif
                }
            }
            mario->position = pos;
        }
    } else {
        mario->position = pos;
    }
#if VERSION <= VERSION_DEMO_JP
    marioChkWallAround(&mario->position, dx, dz, mario->wPlayerCollisionBox.z, mario->wPlayerCollisionBox.y);
#else
    marioChkWallAround(&mario->position, 0, dx, dz, mario->wPlayerCollisionBox.z, mario->wPlayerCollisionBox.y);
#endif
    dou10_yoko_yari(&mario->frontWallDist);
}

/**
 * Wall check while Mario stands still: finds the wall he faces (hitobjFront, frontWallDist;
 * none during a hammer swing), the wall in front on an A press (hitobjFrontA) and on dou_10 a wall behind him,
 * then pushes him out of nearby walls with marioChkWallAround.
 */
void marioSearchFrontWall_staying(f32 dz) {
    Vec pos;
    f32 h;
    f32 h2;
    f32 ang;
    f32 r;
    HitEntry* hit;
    MarioWork* mario = marioGetPtr();
    mario->wallPushTimer = 0;
    r = 0.5f * mario->wPlayerCollisionBox.x;
    if (mario->flags & MARIO_FLAG_PAPER_MODE) {
        r = 0.0f;
    }
    pos = mario->position;
    hit = searchFrontWall(&pos, &h, r, mario->directionView - mario->targetYaw, mario->wPlayerCollisionBox.z);
    if (mario->currMotionId != MARIO_MOTION_HAMMER_2) {
        mario->hitobjFront = hit;
    } else {
        mario->hitobjFront = NULL;
    }
    if (!hit) {
        f32 a = toMovedir(mario->displayDirection);
        pos = mario->position;
        hit = searchFrontWall(&pos, &h, r, a, mario->wPlayerCollisionBox.z);
        if (mario->currMotionId != MARIO_MOTION_HAMMER_2) {
            mario->hitobjFront = hit;
        } else {
            mario->hitobjFront = NULL;
        }
    } else if (!strcmp(gp->currentMapName, "dou_10") && vivianGetStatus() != VIVIAN_STATUS_HIDDEN && vivianGetStatus() != VIVIAN_STATUS_SINKING && vivianGetStatus() != VIVIAN_STATUS_RISING) {
        ang = toMovedir(mario->displayDirection);
        pos = mario->position;
        movePos(&pos.x, &pos.z, 0.5f * mario->wPlayerCollisionBox.x, ang);
        ang += 180.0f;
        hit = searchFrontWall(&pos, &h2, mario->wPlayerCollisionBox.x, ang, mario->wPlayerCollisionBox.z);
        if (hit && mario->wPlayerCollisionBox.x >= h2) {
            mario->hitobjFront = hit;
            h = 0.5f * h2;
        }
    }
    if (mario->hitobjFront) {
        mario->frontWallDist = h;
    }
    if (mario->buttonTrg & PAD_BUTTON_A) {
        mario->hitobjFrontA = NULL;
        pos = mario->position;
        if (searchFrontWall(&pos, &h, r, mario->directionView, mario->wPlayerCollisionBox.z)) {
            mario->hitobjFrontA = mario->hitobjFront;
        }
    }
    pos = mario->position;
#if VERSION <= VERSION_DEMO_JP
    marioChkWallAround(&pos, 0.0f, dz, mario->wPlayerCollisionBox.z, mario->wPlayerCollisionBox.y);
#else
    marioChkWallAround(&pos, 0, 0.0f, dz, mario->wPlayerCollisionBox.z, mario->wPlayerCollisionBox.y);
#endif
    mario->position.x = pos.x;
    mario->position.z = pos.z;
}

/**
 * marioCheckWall for a jumping tube: also stops the jump against a ceiling within 16.8 above,
 * then moves the tube dx in direction dz against walls and pushes it out of nearby ones.
 */
void marioCheckWall_roll(f32 dx, f32 dz) {
    Vec pos;
    Vec outPos;
    Vec outNrm;
    f32 h;
    HitEntry* hit;
    MarioWork* mario = marioGetPtr();
    mario->frontWallDist = 1000.0f;
    if (dx && mario->airTimer == 0) {
        if (mario->wallPushDist < dx) {
            mario->wallPushDist = dx;
            mario->wallPushDir = dz;
            mario->wallPushHold = 14;
        } else {
            if (--mario->wallPushHold <= 0) {
                mario->wallPushHold = 0;
                mario->wallPushDist = dx;
                mario->wallPushDir = dz;
            }
        }
    } else {
        mario->wallPushDist = 0.0f;
    }
    if (dx && mario->airTimer == 0) {
        mario->moveDirection = mario->wallPushDir;
    }
    if (mario->flags & MARIO_FLAG_IS_JUMPING) {
        f32 dist = 30.0f;
        Vec dir = {0.0f, 1.0f, 0.0f};
        hit = marioHitCheckVecInl(&mario->position, &dir, &outPos, &outNrm, &dist);
        if (hit && dist > 16.8f) {
            hit = NULL;
        }
        mario->hitobjHead = hit;
        if (hit) {
            mario->position.y = outPos.y - mario->wPlayerCollisionBox.y;
        }
    }
    if (!dx) {
        marioSearchFrontWall_staying(dz);
        return;
    }
    if (mario->hitobjHead) {
        mario->baseSpeed = dx = 0.0f;
        mario->jumpVelocityY = 0.0f;
        mario->jumpAccelerationY = 0.0f;
        mario->jumpJerkY = 0.0f;
        mario->jumpSnapY = 0.0f;
    }
    pos = mario->position;
    hit = searchFrontWall(&pos, &h, dx, dz, mario->wPlayerCollisionBox.z);
    if (hit) {
        mario->hitobjFront = hit;
        if (marioChkBeroInl(hit)) {
            if (dx) {
                mario->hitobjPush = hit;
                mario->hitobjWall = hit;
            }
            movePos(&mario->position.x, &mario->position.z, dx, dz);
        } else {
            mario->frontWallDist = h;
            mario->hitobjWall = hit;
            if (++mario->wallPushTimer >= 20) {
                mario->wallPushTimer = 20;
                if (!(hitGetAttr(hit) & HIT_ATTR_SHIP_ONLY) ? 1 : marioGetPtr()->currMotionId == MARIO_MOTION_SHIP) {
                    mario->hitobjPush = hit;
                }
            }
            mario->position = pos;
        }
    } else {
        mario->position = pos;
    }
    pos = mario->position;
#if VERSION <= VERSION_DEMO_JP
    marioChkWallAround(&pos, dx, dz, mario->wPlayerCollisionBox.z, mario->wPlayerCollisionBox.y);
#else
    marioChkWallAround(&pos, 0, dx, dz, mario->wPlayerCollisionBox.z, mario->wPlayerCollisionBox.y);
#endif
    mario->position.x = pos.x;
    mario->position.z = pos.z;
}

/** marioHitCheckVecInl with the direction built after the position copy (the target schedules the pos loads first) */
// marioHitCheckVecInl for a horizontal ray: the direction is (s, 0, c)
static inline HitEntry* marioChkFrontInl(const Vec* pos, f32 s, f32 c, Vec* outPos, Vec* outNrm, f32* dist) {
    HitCheckQuery query;
    HitEntry* hit;
    f32 rf;

    {
        Vec dir = {0.0f, 0.0f, 0.0f};
        dir.x = s;
        dir.z = c;
        query.targetPos = *pos;
        query.targetDir = dir;
    }
    query.targetDistance = *dist;
    if (marioGetPtr()->currMotionId == MARIO_MOTION_VIVIAN) {
        hit = hitCheckVecFilter(&query, chkfilterVecVivian);
    } else if (marioGetPtr()->flags & MARIO_FLAG_TUBE_MODE) {
        hit = hitCheckVecFilter(&query, chkfilterVecRoll);
    } else {
        hit = hitCheckVecFilter(&query, chkfilterVec);
    }
    if (!hit) {
        return NULL;
    }
    if (query.targetDistance >= 0.0f) {
        rf = 0.5f;
    } else {
        rf = -0.5f;
    }
    *dist = (s32)((1000.0f * query.targetDistance) + rf) / 1000.0f;
    if (outPos) {
        *outPos = query.hitPos;
        if (outPos->x >= 0.0f) {
            rf = 0.5f;
        } else {
            rf = -0.5f;
        }
        outPos->x = (s32)((1000.0f * outPos->x) + rf) / 1000.0f;
        if (outPos->y >= 0.0f) {
            rf = 0.5f;
        } else {
            rf = -0.5f;
        }
        outPos->y = (s32)((1000.0f * outPos->y) + rf) / 1000.0f;
        if (outPos->z >= 0.0f) {
            rf = 0.5f;
        } else {
            rf = -0.5f;
        }
        outPos->z = (s32)((1000.0f * outPos->z) + rf) / 1000.0f;
    }
    if (outNrm) {
        *outNrm = query.hitNormal;
    }
    return hit;
}

/**
 * Casts a horizontal ray from pos in direction angle (degrees) for up to dist.
 * Returns the hit and its flat distance in *out, or NULL with *out = -1.
 * Used by the jump motions (mot_jump.c) to look for a wall ahead.
 */
HitEntry* marioChkFront(f32* out, const Vec* pos, f32 angle, f32 dist) {
    Vec outPos, outNrm;
    f32 s, c;
    f32 d;
    HitEntry* hit;
    sincosf(&s, &c, angle);
    d = dist;
    hit = marioChkFrontInl(pos, s, c, &outPos, &outNrm, &d);
    if (hit && d <= dist) {
        *out = distABf(pos->x, pos->z, outPos.x, outPos.z);
        return hit;
    }
    *out = -1.0f;
    return NULL;
}

/** Copies v into *d (helper of sfwVecSetNested for searchFrontWall). */
// FAKE: one-use inline helper; writing the call out changes the object in both versions
static inline void sfwVecSet(Vec* d, Vec v) {
    *d = v;
}

// FAKE: two-level wrapper so dir2 is allocated as a nested-inline local (lowest stack slot, COLOURING C2)
static inline void sfwVecSetNested(Vec* d, Vec v) {
    sfwVecSet(d, v);
}

/**
 * Moves pos dx in direction dz (degrees) unless a wall blocks it:
 * rays at two to four heights (by character and mode) reach half the width w ahead, plus 45-degree side rays.
 * On a wall pos slides along it, *h gets the distance and the wall is returned;
 * balloon walls (chkBalloon) also go in hitobjFrontWall.
 * On Riverside Station's eki_04, in marioChkSts state 0x10 on a slope of 45 degrees or more,
 * pos just moves with no wall check.
 */
static HitEntry* searchFrontWall(Vec* pos, f32* h, f32 dx, f32 dz, f32 w) {
    Vec p;
    Vec dir;
    Vec hp;
    Vec bestNrm;
    Vec nrm;
    f32 heights[4];
    f32 s, c;
    f32 s2, c2;
    f32 half;
    f32 range;
    f32 mx, mz;
    f32 bestDist;
    f32 dist;
    HitEntry* hit2;
    s32 i;
    s32 count;
    HitEntry* best;
    HitEntry* hit;
    MarioWork* mario;

    mario = marioGetPtr();
#if VERSION <= VERSION_DEMO_JP
    if (mario->characterId == 2 && (mario->stageFlags & KPA_STAGE_FLAG_SIDE_SCROLLING)) {
        return kpaSearchWallFront(pos, h, dx, dz);
    }
#endif
    if (vivianGetStatus() == VIVIAN_STATUS_HIDDEN) {
        return NULL;
    }
    sincosf(&s, &c, dz);
    if (marioChkSts(MARIO_STS_ROLL) && !strcmp(gp->currentMapName, "eki_04") &&
        (__fabsf(mario->groundRot.x) >= 45.0f || __fabsf(mario->groundRot.z) >= 45.0f)) {
        pos->x += dx * s;
        pos->z += dx * c;
        return NULL;
    }
    count = 2;
    if (mario->characterId == CHARACTER_BOWSER) {
#if VERSION <= VERSION_DEMO_JP
        heights[0] = 0.75f * mario->wPlayerCollisionBox.y;
        heights[1] = 11.0f;
#else
        if (mario->stageFlags & KPA_STAGE_FLAG_SIDE_SCROLLING) {
            count = 3;
            heights[0] = 0.75f * mario->wPlayerCollisionBox.y;
            heights[1] = 0.5f * mario->wPlayerCollisionBox.y;
            heights[2] = 11.0f;
#if VERSION >= VERSION_US
            if (kpaGetLevel() == 2) {
                count = 4;
                heights[3] = 30.0f;
            }
#endif
        } else {
            heights[0] = 0.75f * mario->wPlayerCollisionBox.y;
            heights[1] = 11.0f;
        }
#endif
    } else if (mario->currMotionId == MARIO_MOTION_YOSHI) {
        heights[0] = 0.75f * marioGetHeight();
        heights[1] = 11.0f;
    } else if (!(mario->flags & MARIO_FLAG_TUBE_MODE)) {
        heights[0] = VERSION_PICK(27.75f, 26.640001f);
        heights[1] = 11.0f;
        if (mario->airTimer) {
            count = 3;
            heights[2] = 0.1f;
        }
    } else {
        heights[0] = 12.0f;
        heights[1] = 11.0f;
#if VERSION >= VERSION_JP
        if (mario->airTimer) {
            count = 3;
            heights[2] = 7.0f;
        }
#endif
    }
    half = 0.5f * w;
    best = NULL;
    range = half + dx;
    bestDist = range;
    p = *pos;
    dir = (Vec){s, 0.0f, c};
    for (i = 0; i < count; i++) {
        p.y = pos->y + heights[i];
        dist = range;
        hit = marioHitCheckVecInl(&p, &dir, &hp, &nrm, &dist);
        if (hit) {
            if (chkBalloon(hit)) {
                mario->hitobjFrontWall = hit;
            }
            if (best) {
                if (dist < bestDist) {
                    bestDist = dist;
                    best = hit;
                    bestNrm = nrm;
                }
            } else {
                bestDist = dist;
                best = hit;
                bestNrm = nrm;
            }
        }
    }
    if (best) {
        _reflect(s * dx, bestNrm.x, c * dx, bestNrm.z, &mx, &mz);
    }
    hit2 = NULL;
    sincosf(&s2, &c2, dz + 45.0f);
    {
        Vec dir2;
        p = *pos;
        sfwVecSetNested(&dir2, (Vec){s2, 0.0f, c2});
        dist = half;
        for (i = 0; i < count; i++) {
            p.y = pos->y + heights[i];
            dist = half;
            hit2 = marioHitCheckVecInl(&p, &dir2, NULL, NULL, &dist);
            if (hit2) {
                break;
            }
        }
    }
    if (hit2) {
        hit2 = NULL;
        sincosf(&s2, &c2, dz - 45.0f);
        for (i = 0; i < count; i++) {
            p.y = pos->y + heights[i];
            dist = half;
            hit2 = marioHitCheckVecInl(&p, &(Vec){s2, 0.0f, c2}, NULL, NULL, &dist);
        }
    }
    if (best) {
        if (!hit2) {
            *h = dist;
            pos->x += mx;
            pos->z += mz;
        } else {
            *h = 0.0f;
        }
        return best;
    }
    if (!hit2) {
        pos->x += dx * s;
        pos->z += dx * c;
    }
    return NULL;
}

/**
 * The sideways spears (yoko yari) of Pirate's Grotto dou_10:
 * when no wall is in front and Mario is not a tube,
 * looks for a spike (0x800) below him and by +z rays from him and 8 units (2 paper-thin) to either side;
 * a spike becomes hitobjFront with *out = 0.8, so marioChkToge hurts him.
 */
HitEntry* dou10_yoko_yari(f32* out) {
    Vec pos, pos2, outPos, outNrm;
    f32 h;
    f32 d;
    HitEntry* hit;
    f32 step;
    MarioWork* mario = marioGetPtr();
    if (mario->currMotionId == MARIO_MOTION_ROLL) {
        return NULL;
    }
    toMovedir(mario->displayDirection);
    hit = mario->hitobjFront;
    if (hit) {
        return hit;
    }
    pos = mario->position;
    if (!strcmp(gp->currentMapName, "dou_10") && vivianGetStatus() != VIVIAN_STATUS_HIDDEN && vivianGetStatus() != VIVIAN_STATUS_SINKING && vivianGetStatus() != VIVIAN_STATUS_RISING && !hit) {
        if (mario->flags & MARIO_FLAG_PAPER_MODE) {
            step = 2.0f;
        } else {
            step = 8.0f;
        }
        d = 100.0f;
        pos2 = pos;
        pos2.y += 60.0f;
        {
            Vec dir = {0.0f, -1.0f, 0.0f};
            hit = marioHitCheckVecInl(&pos2, &dir, &outPos, &outNrm, &d);
        }
        if (hit && (hitGetAttr(hit) & HIT_ATTR_SPIKES)) {
            mario->hitobjFront = hit;
            *out = 0.8f;
        }
        pos = mario->position;
        hit = searchFrontWall(&pos, &h, 20.0f, 0.0f, 20.0f);
        if (hit && (hitGetAttr(hit) & HIT_ATTR_SPIKES)) {
            mario->hitobjFront = hit;
            *out = 0.8f;
        }
        pos = mario->position;
        movePos(&pos.x, &pos.z, step, 270.0f);
        hit = searchFrontWall(&pos, &h, 20.0f, 0.0f, 20.0f);
        if (hit && (hitGetAttr(hit) & HIT_ATTR_SPIKES)) {
            mario->hitobjFront = hit;
            *out = 0.8f;
        }
        pos = mario->position;
        movePos(&pos.x, &pos.z, step, 90.0f);
        hit = searchFrontWall(&pos, &h, 20.0f, 0.0f, 20.0f);
        if (hit && (hitGetAttr(hit) & HIT_ATTR_SPIKES)) {
            mario->hitobjFront = hit;
            *out = 0.8f;
        }
    }
    return hit;
}

/** dou10_yoko_yari for Yoshi (party_yoshi.c): the same spike search on dou_10, run even when a wall is in front. */
HitEntry* dou10_yoko_yari2(f32* out) {
    Vec pos, pos2, outPos, outNrm;
    f32 h;
    f32 d;
    HitEntry* hit;
    MarioWork* mario = marioGetPtr();
    toMovedir(mario->displayDirection);
    pos = mario->position;
    if (strcmp(gp->currentMapName, "dou_10")) {
        return NULL;
    }
    d = 100.0f;
    pos2 = pos;
    pos2.y += 60.0f;
    {
        Vec dir = {0.0f, -1.0f, 0.0f};
        hit = marioHitCheckVecInl(&pos2, &dir, &outPos, &outNrm, &d);
    }
    if (hit && (hitGetAttr(hit) & HIT_ATTR_SPIKES)) {
        mario->hitobjFront = hit;
        *out = 0.8f;
    }
    pos = mario->position;
    hit = searchFrontWall(&pos, &h, 20.0f, 0.0f, 20.0f);
    if (hit && (hitGetAttr(hit) & HIT_ATTR_SPIKES)) {
        mario->hitobjFront = hit;
        *out = 0.8f;
    }
    pos = mario->position;
    movePos(&pos.x, &pos.z, 8.0f, 270.0f);
    hit = searchFrontWall(&pos, &h, 20.0f, 0.0f, 20.0f);
    if (hit && (hitGetAttr(hit) & HIT_ATTR_SPIKES)) {
        mario->hitobjFront = hit;
        *out = 0.8f;
    }
    pos = mario->position;
    movePos(&pos.x, &pos.z, 8.0f, 90.0f);
    hit = searchFrontWall(&pos, &h, 20.0f, 0.0f, 20.0f);
    if (hit && (hitGetAttr(hit) & HIT_ATTR_SPIKES)) {
        mario->hitobjFront = hit;
        *out = 0.8f;
    }
    return hit;
}

/**
 * Spear check on dou_10 for tube mode (mario.c): unfiltered rays 40 along +z from 10 ahead of and behind Mario;
 * a hit becomes hitobjFront, and mario.c hurts him if it has the spike attribute 0x800.
 * Descriptive name (not in the symbol maps).
 */
#if VERSION >= VERSION_JP

HitEntry* dou10_yoko_yari3(void) {
    HitCheckQuery query;
    f32 s, c;
    f32 x, z;
    f32 y;
    HitEntry* hit;
    MarioWork* mario = marioGetPtr();
    if (strcmp(gp->currentMapName, "dou_10")) {
        return NULL;
    }
    x = mario->position.x;
    y = mario->position.y;
    z = mario->position.z;
    movePos(&x, &z, 10.0f, mario->directionView);
    {
        Vec pos = {0.0f, 0.0f, 0.0f};
        pos.y = y;
        pos.x = x;
        pos.z = z;
        query.targetPos = pos;
        query.targetPos.y += 5.0f;
    }
    sincosf(&s, &c, 0.0f);
    {
        Vec dir = {0.0f, 0.0f, 0.0f};
        dir.x = s;
        dir.z = c;
        query.targetDir = dir;
    }
    query.targetDistance = 40.0f;
    hit = hitCheckVecFilter(&query, NULL);
    if (hit) {
        mario->hitobjFront = hit;
        return hit;
    }
    x = mario->position.x;
    y = mario->position.y;
    z = mario->position.z;
    movePos(&x, &z, -10.0f, mario->directionView);
    {
        Vec pos = {0.0f, 0.0f, 0.0f};
        pos.y = y;
        pos.x = x;
        pos.z = z;
        query.targetPos = pos;
        query.targetPos.y += 5.0f;
    }
    sincosf(&s, &c, 0.0f);
    {
        Vec dir = {0.0f, 0.0f, 0.0f};
        dir.x = s;
        dir.z = c;
        query.targetDir = dir;
    }
    query.targetDistance = 40.0f;
    hit = hitCheckVecFilter(&query, NULL);
    if (hit) {
        mario->hitobjFront = hit;
    }
    return hit;
}
#endif

/** marioHitCheckVecInl that stores the hit in *out and returns whether there was one. */
static inline BOOL marioHitCheckVecChk(Vec* p, Vec* d, Vec* op, Vec* on, f32* di, HitEntry** out) {
    *out = marioHitCheckVecInl(p, d, op, on, di);
    return *out != NULL;
}

/**
 * Looks for a step just ahead of Mario (or the tube): one ray down from half his height.
 * A floor above his feet or less than 2 below gives its y in *y and is returned,
 * which marioChkGnd uses to climb 5-11 unit steps; a lower floor gives the drop in *h and its direction in *ang.
 */
static HitEntry* marioChkFrontStep(f32* y, f32* h, f32* ang) {
    Vec outPos;
    Vec outNrm;
    f32 x0;
    f32 y0;
    f32 z0;
    f32 a;
    f32 hh;
    f32 s;
    f32 r;
    f32 dist;
    HitEntry* hit;
    BOOL ok;
    MarioWork* mario = marioGetPtr();
    f32 c;
    f32 rad;
    mario->groundRot.x = 0.0f;
    mario->groundRot.y = 0.0f;
    mario->groundRot.z = 0.0f;
    mario->groundSlope = 0.0f;
    hh = (mario->flags & MARIO_FLAG_TUBE_MODE) ? 18.5f : 0.5f * mario->wPlayerCollisionBox.y;
    *y = mario->position.y;
    *h = -1.0f;
    if (mario->baseSpeed) {
        a = mario->directionView;
    } else {
        a = toMovedir(mario->displayDirection);
    }
    a = revise360((s32)(0.5f + a));
    if (mario->flags & MARIO_FLAG_TUBE_MODE) {
        r = 3.0f;
    } else {
        r = 0.265f * mario->wPlayerCollisionBox.x;
    }
    rad = (3.1415927f * a) / 180.0f;
    s = (f32)sin(rad);
    c = (f32)cos(rad);
    s = r * s;
    a = r * -c;
    y0 = mario->position.y;
    x0 = mario->position.x;
    z0 = mario->position.z;
    {
        Vec v = {0.0f, 0.0f, 0.0f};
        Vec dir = {0.0f, -1.0f, 0.0f};
        v.x = x0 + s;
        v.y = y0 + hh;
        v.z = z0 + a;
        dist = 0.1f + (11.0f + hh);
        ok = marioHitCheckVecChk(&v, &dir, &outPos, &outNrm, &dist, &hit);
    }
    if (ok) {
        mario->groundSlope = ABCtoAngleY(&outNrm);
        if (outPos.y < y0 && __fabsf(outPos.y - y0) >= 2.0f) {
            *h = y0 - outPos.y;
            *ang = angleABf(x0, z0, outPos.x, outPos.z);
        } else {
            *y = outPos.y;
            *h = 0.0f;
            *ang = angleABf(x0, z0, outPos.x, outPos.z);
            return hit;
        }
    }
    return NULL;
}

// FAKE: stand-in that nothing calls (the linker strips it); it only places its five literals in this object's constant pool
void marioHitStrippedPool(f32* f) {
    f[0] = 0.75f;
    f[1] = -1.0f;
    f[2] = 3.1415927f;
    f[3] = 0.1f;
    f[4] = 2.0f;
}

/** Sets up marioChkWallAround's two ray origins (11 above pos and h - 1 above it) and the ray length (half of w). */
static inline void wallBase(Vec* pos, Vec* hi, Vec* lo, f32 h, f32* half, f32 w) {
    Vec base = *pos;
    *hi = base;
    hi->y += 11.0f;
    *half = 0.5f * w;
    *lo = base;
    lo->y += h - 1.0f;
}

/** Copies *in to *out through a temporary, for marioChkWallAround's second pass. */
static inline void wallCopy(Vec* out, Vec* in) {
    Vec t;
    t = *in;
    *out = t;
}

#if VERSION <= VERSION_DEMO_JP

/** The early demos have no mode argument: the push is always limited to 1/8 of the width. */
BOOL marioChkWallAround(Vec* pos, f32 unused, f32 dir, f32 w, f32 h) {
#else
BOOL marioChkWallAround(Vec* pos, s32 mode, f32 unused, f32 dir, f32 w, f32 h) {
#endif
    Vec hi;
    Vec lo;
    Vec d;
    Vec acc;
    Vec saved;
    f32 s, c;
    f32 s2, c2;
    f32 half;
    f32 a;
    f32 a2;
    f32 d1, d2;
    Vec dd;
    f32 lim;
    f32 step;
    s32 i;
    s32 n;
    BOOL hitAny;
    HitEntry* hit1;
    HitEntry* hit2;

    n = 8;
    hitAny = FALSE;
    if (kpaGetStageViewType() == KPA_VIEW_SIDE) {
        n = 2;
    }
    step = 360.0f / n;
    a = dir;
    wallBase(pos, &hi, &lo, h, &half, w);
    acc = (Vec){0.0f, 0.0f, 0.0f};
    for (i = 0; i < n; i++) {
        sincosf(&s, &c, a);
        d2 = d1 = half;
        d = (Vec){s, 0.0f, c};
        hit1 = marioHitCheckVecInl(&hi, &d, NULL, NULL, &d1);
        hit2 = marioHitCheckVecInl(&lo, &d, NULL, NULL, &d2);
        if (marioChkBeroInl(hit1)) {
            hit1 = NULL;
        }
        if (marioChkBeroInl(hit2)) {
            hit2 = NULL;
        }
        if (hit1 && hit2) {
            if (d1 > d2) {
                d1 = d2;
                hit1 = hit2;
            }
        } else if (hit2) {
            hit1 = hit2;
            d1 = d2;
        }
        if (hit1) {
            sincosf(&s2, &c2, 180.0f + a);
            hitAny = TRUE;
            acc.x += s2 * __fabsf(half - d1);
            acc.z += c2 * __fabsf(half - d1);
        }
        a += step;
    }
    if (!hitAny) {
        return hitAny;
    }
    lim = 0.125f * w;
#if VERSION >= VERSION_JP
    if (mode == 0) {
        MarioWork* mario = marioGetPtr();
        if (mario->currMotionId == MARIO_MOTION_FALL || mario->currMotionId == MARIO_MOTION_FALL_2) {
            lim = 0.25f * w;
        }
    }
    if (lim < 1.0f) {
        lim = 1.0f;
    }
#endif
    saved = acc;
    {
        f32 mag = PSVECMag(&acc);
        if (mag > lim) {
            PSVECScale(&acc, &acc, lim / mag);
        }
    }
    PSVECAdd(pos, &acc, pos);

    a2 = dir;
    wallBase(pos, &hi, &lo, h, &half, w);
    acc = (Vec){0.0f, 0.0f, 0.0f};
    for (i = 0; i < n; i++) {
        sincosf(&s, &c, a2);
        d2 = d1 = half;
        wallCopy(&dd, &(Vec){s, 0.0f, c});
        hit1 = marioHitCheckVecInl(&hi, &dd, NULL, NULL, &d1);
        hit2 = marioHitCheckVecInl(&lo, &dd, NULL, NULL, &d2);
        if (marioChkBeroInl(hit1)) {
            hit1 = NULL;
        }
        if (marioChkBeroInl(hit2)) {
            hit2 = NULL;
        }
        if (hit1 && hit2) {
            if (d1 > d2) {
                d1 = d2;
                hit1 = hit2;
            }
        } else if (hit2) {
            hit1 = hit2;
            d1 = d2;
        }
        if (hit1) {
            sincosf(&s2, &c2, 180.0f + a2);
            hitAny = TRUE;
            acc.x += s2 * __fabsf(half - d1);
            acc.z += c2 * __fabsf(half - d1);
        }
        a2 += step;
    }
    if (!hitAny) {
        return TRUE;
    }
    if (PSVECDotProduct(&saved, &acc) >= 0.0f) {
        return TRUE;
    }
    PSVECScale(&acc, &acc, 0.5f);
    PSVECAdd(pos, &acc, pos);
    return TRUE;
}

/**
 * TRUE when walls within 13 units (at three heights) surround Mario
 * so that no three neighbouring directions of eight are all open.
 * While it is TRUE the paper (slit) and tube motions keep Mario in that form
 * and starting the tube is refused, so he cannot change form in a gap.
 */
BOOL marioSlitChkWallAround(void) {
    s32 flags[8];
    Vec pos;
    s32 i;
    f32 a;
    f32 s, c;
    HitEntry* hit;
    MarioWork* mario = marioGetPtr();
    f32 k;
    pos = mario->position;
    pos.y += 11.0f;
    for (i = 0, a = 0.0f; i < 8; i++, a += 45.0f) {
        Vec dir;
        f32 dist;
        flags[i] = 0;
        sincosf(&s, &c, a);
        dir = (Vec){s, 0.0f, c};
        dist = 13.0f;
        hit = marioHitCheckVecInl(&pos, &dir, NULL, NULL, &dist);
        if (hit && !marioChkBeroInl(hit)) {
            flags[i] = 1;
        }
    }

    // FAKE: spare temporary k; the plain `pos.y = mario->position.y + 5.0f;` changes the object
    k = 5.0f;
    pos.y = mario->position.y + k;
    for (i = 0, a = 0.0f; i < 8; i++, a += 45.0f) {
        if (flags[i] != 1) {
            Vec dir;
            f32 dist;
            sincosf(&s, &c, a);
            dir = (Vec){s, 0.0f, c};
            dist = 13.0f;
            hit = marioHitCheckVecInl(&pos, &dir, NULL, NULL, &dist);
            if (hit && !marioChkBeroInl(hit)) {
                flags[i] = 1;
            }
        }
    }
    pos.y = mario->position.y + (mario->wPlayerCollisionBox.y - 1.0f);
    for (i = 0, a = 0.0f; i < 8; i++, a += 45.0f) {
        if (flags[i] != 1) {
            Vec dir;
            f32 dist;
            sincosf(&s, &c, a);
            dir = (Vec){s, 0.0f, c};
            dist = 13.0f;
            hit = marioHitCheckVecInl(&pos, &dir, NULL, NULL, &dist);
            if (hit && !marioChkBeroInl(hit)) {
                flags[i] = 1;
            }
        }
    }
    for (i = 0; i < 8; i++) {
        if (!flags[i] && !flags[(i + 1) & 7] && !flags[(i + 2) & 7]) {
            return FALSE;
        }
    }
    return TRUE;
}

/**
 * marioCheckWall for the paper boat (mot_ship.c):
 * moves it dx in direction dz against walls, pushes it out of nearby ones at a height of 4,
 * and caps the wall-around correction at the boat's dash speed.
 */
void marioCheckWallShip(f32 dx, f32 dz) {
    Vec pos;
    Vec old;
    Vec diff;
    Vec tmp;
    f32 h;
    f32 max;
    f32 mag;
    HitEntry* hit;
    MarioWork* mario = marioGetPtr();
    mario->frontWallDist = 1000.0f;
    if (dx && mario->airTimer == 0) {
        if (mario->wallPushDist < dx) {
            mario->wallPushDist = dx;
            mario->wallPushDir = dz;
            mario->wallPushHold = 14;
        } else {
            if (--mario->wallPushHold <= 0) {
                mario->wallPushHold = 0;
                mario->wallPushDist = dx;
                mario->wallPushDir = dz;
            }
        }
    } else {
        mario->wallPushDist = 0.0f;
    }
    if (dx && mario->airTimer == 0) {
        mario->moveDirection = mario->wallPushDir;
    }
    if (!dx) {
        marioSearchFrontWall_staying(dz);
        return;
    }
    pos = mario->position;
    hit = searchFrontWall(&pos, &h, dx, dz, mario->wPlayerCollisionBox.z);
    if (hit) {
        mario->hitobjFront = hit;
        if (marioChkBeroInl(hit)) {
            if (dx) {
                mario->hitobjPush = hit;
                mario->hitobjWall = hit;
            }
            movePos(&mario->position.x, &mario->position.z, dx, dz);

            // pos is not read again; the retail code still copies the position before the return
            pos = mario->position;
            return;
        }
        if (!dx) {
            mario->wallPushTimer = 0;
        } else {
            mario->frontWallDist = h;
            mario->hitobjWall = hit;
            if (++mario->wallPushTimer >= 20) {
                s32 attr;
                BOOL r;
                mario->wallPushTimer = 20;
                attr = hitGetAttr(hit);
                r = !(attr & HIT_ATTR_SHIP_ONLY) ? TRUE : marioGetPtr()->currMotionId == MARIO_MOTION_SHIP;
                if (r) {
                    mario->hitobjPush = hit;
                }
            }
        }
        mario->position = pos;
    }
    old = pos;
    mario->position = pos;
#if VERSION <= VERSION_DEMO_JP
    tmp = pos;
    marioChkWallAround(&mario->position, dx, dz, mario->wPlayerCollisionBox.z, 4.0f);
#else
    marioChkWallAround(&mario->position, 0, dx, dz, mario->wPlayerCollisionBox.z, 4.0f);
#endif
    max = mario->baseDashSpeed;
    PSVECSubtract(&mario->position, &old, &diff);
    mag = PSVECMag(&diff);
    if (mag > max) {
        PSVECScale(&diff, &diff, max / mag);
        PSVECAdd(&old, &diff, &mario->position);
    }
}
