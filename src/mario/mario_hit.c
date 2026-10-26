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
#include "version.h"
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

static const Vec zv0 = {0.0f, 0.0f, 0.0f};
static const Vec zv1 = {0.0f, 0.0f, 0.0f};

extern HitEntry* kpaSearchGround(f32* a, f32* b, f32* c, f32 d);
extern HitEntry* marioSearchHead(Vec* pos, f32* h, f32 d);
#ifdef VERSION_G8ME01
#endif

static inline BOOL marioChkBeroSub(s32 attr) {
    BOOL r = TRUE;
    if (attr & 0x80) {
        r = FALSE;
        if (marioGetPtr()->currMotionId == MARIO_MOTION_SHIP) {
            r = TRUE;
        }
    }
    return r;
}

static inline BOOL marioChkBeroInl(HitEntry* hit) {
    s32 attr = hitGetAttr(hit);
    MarioWork* w = marioGetPtr();
    if ((attr & 0x80000) && !(w->flags & MARIO_FLAG_TUBE_MODE)) {
        return FALSE;
    }
    if (!(attr & 0x80)) {
        return (attr & 0x800000) ? 1 : 0;
    }
    return w->currMotionId == MARIO_MOTION_SHIP;
}

static inline BOOL marioChkBeroSub2(s32 attr) {
    if (!(attr & 0x80)) {
        return TRUE;
    }
    return marioGetPtr()->currMotionId == MARIO_MOTION_SHIP;
}

static inline BOOL marioChkBeroSub3(s32 attr) {
    return (attr & 0x80) ? (marioGetPtr()->currMotionId == MARIO_MOTION_SHIP) : TRUE;
}

static inline BOOL marioChkBeroSub4(s32 attr) {
    BOOL r = TRUE;
    if (attr & 0x80) {
        r = marioGetPtr()->currMotionId == MARIO_MOTION_SHIP;
    }
    return r;
}

static inline BOOL marioChkBeroSub5(HitEntry* hit) {
    s32 attr = hitGetAttr(hit);
    if (!(attr & 0x80)) {
        return TRUE;
    }
    return marioGetPtr()->currMotionId == MARIO_MOTION_SHIP;
}

static inline BOOL marioChkBeroSub6(s32 attr) {
    if (attr & 0x80) {
        return marioGetPtr()->currMotionId == MARIO_MOTION_SHIP;
    } else {
        return TRUE;
    }
}

static inline f32 marioHitNrmAngle(Vec* nrm) {
    f32 nz = nrm->z;
    f32 ny = nrm->y;
    f32 nx = nrm->x;
    MarioWork* mp = marioGetPtr();
    f32 y100 = 100.0f * ny;
    mp->groundRot.x = (s32)(100.0f * (angleABf(0.0f, 0.0f, 100.0f * nz, y100) - 180.0f) + 0.5f) / 100.0f;
    mp->groundRot.y = 0.0f;
    mp->groundRot.z = (s32)(100.0f * (angleABf(0.0f, 0.0f, 100.0f * nx, y100) - 180.0f) + 0.5f) / 100.0f;
    return angleABf(0.0f, 0.0f, sqrtf(nx * nx + nz * nz), -ny);
}

static inline f32 marioHitNrmAnglePtr(Vec* nrm) {
    MarioWork* mp = marioGetPtr();
    mp->groundRot.x = (s32)(100.0f * (angleABf(0.0f, 0.0f, 100.0f * nrm->z, 100.0f * nrm->y) - 180.0f) + 0.5f) / 100.0f;
    mp->groundRot.y = 0.0f;
    mp->groundRot.z = (s32)(100.0f * (angleABf(0.0f, 0.0f, 100.0f * nrm->x, 100.0f * nrm->y) - 180.0f) + 0.5f) / 100.0f;
    return angleABf(0.0f, 0.0f, sqrtf(nrm->x * nrm->x + nrm->z * nrm->z), -nrm->y);
}

void marioHitCheckWidth(void) {
    MarioWork* w = marioGetPtr();
    f32 v;
    if (!(w->flags & MARIO_FLAG_PAPER_MODE)) {
        f32 lim = !(w->flags & MARIO_FLAG_TUBE_MODE) ? w->wPlayerCollisionBox.x : 18.0f;
        f32 s = (w->wPlayerCollisionBox.z += 1.0f);
        if (s > lim) {
            w->wPlayerCollisionBox.z = lim;
        }
    } else {
        v = w->wPlayerCollisionBox.z;
        if (v > 6.0f) {
            f32 t = v - 1.0f;
            w->wPlayerCollisionBox.z = t;
            if (t < 6.0f) {
                w->wPlayerCollisionBox.z = 6.0f;
            }
        }
        v = w->wPlayerCollisionBox.z;
        if (v < 6.0f) {
            v += 1.0f;
            w->wPlayerCollisionBox.z = v;
            if (v > 6.0f) {
                w->wPlayerCollisionBox.z = 6.0f;
            }
        }
    }
}

BOOL chkFilterAttr2(HitCheckQuery* query, HitEntry* entry) {
    s32 ret = 0;
    s32 attr = entry->attributes;
    if ((attr & query->user0) == 0 || (attr & query->user1) != 0) {
        ret = 1;
    }
    return ret;
}

BOOL chkfilterVec(HitCheckQuery* query, HitEntry* entry) {
    MapObjectEntry* mobj;
    if (entry->attributes & 0x80080) {
        return TRUE;
    }
    if (!(entry->attributes & 0x800000) && (entry->attributes & 0x4)) {
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

BOOL chkfilterVecRoll(HitCheckQuery* query, HitEntry* entry) {
    MapObjectEntry* mobj;
    if (entry->attributes & 0x80) {
        return TRUE;
    }
    if (!(entry->attributes & 0x800000) && (entry->attributes & 0x80004)) {
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

BOOL chkfilterVecVivian(HitCheckQuery* query, HitEntry* entry) {
    MapObjectEntry* mobj;
    if ((entry->attributes & 0x80804) && !(entry->attributes & 0x800000)) {
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

HitEntry* marioHitCheckVec(Vec* pos, Vec* dir, Vec* outPos, Vec* outNrm, f32* dist) {
    return marioHitCheckVecInl(pos, dir, outPos, outNrm, dist);
}

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

static inline HitEntry* marioHitCheckAttr2Inl(Vec* pos, Vec* dir, Vec* outPos, Vec* outNrm, f32* dist) {
    HitCheckQuery query;
    HitEntry* hit;
    f32 rf;

    query.targetPos = *pos;
    query.targetDir = *dir;
    query.targetDistance = *dist;
    query.user0 = 4;
    query.user1 = 0x800000;
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

HitEntry* marioHitCheck2(Vec* pos, Vec* dir, Vec* outPos, Vec* outNrm, f32* dist) {
    HitEntry* hit;
    MarioWork* w = marioGetPtr();
    f32 d = *dist;
    if (w->currMotionId != MARIO_MOTION_JABARA) {
        if (!(w->flags & MARIO_FLAG_IS_FALLING)) {
            hit = marioHitCheckAttr2Inl(pos, dir, outPos, outNrm, &d);
        } else {
            hit = marioHitCheckVecInl(pos, dir, outPos, outNrm, &d);
        }
    } else if (marioGetJabaraState() == 1) {
        hit = marioHitCheckAttr2Inl(pos, dir, outPos, outNrm, &d);
    } else {
        hit = marioHitCheckVecInl(pos, dir, outPos, outNrm, &d);
    }
    *dist = d;
    return hit;
}

void clear_hitobj_ride(void) {
    MarioWork* w = marioGetPtr();
    w->hitobjStandOn = NULL;
    if (w->characterId == CHARACTER_BOWSER) {
        kpaClearHitobjRide();
    }
}

BOOL marioChkBero(HitEntry* hit) {
    s32 attr = hitGetAttr(hit);
    MarioWork* w = marioGetPtr();
    if ((attr & 0x80000) && !(w->flags & MARIO_FLAG_TUBE_MODE)) {
        return FALSE;
    }
    if (!(attr & 0x80)) {
        return (attr & 0x800000) ? 1 : 0;
    }
    return w->currMotionId == MARIO_MOTION_SHIP;
}

#pragma inline_max_size(1000)
static inline void marioChkGndRoll(MarioWork* w) {
            HitEntry* hit;
            f32 gz, gx;
            Vec pos, outPos, outNrm;
            f32 dist = 47.0f;
            w->baseHeight = w->basePos.y - w->position.y;
            pos = w->position;
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

HitEntry* marioSearchUnder(void) {
    Vec pos, outPos, outNrm;
    f32 d = 1000.0f;
    HitEntry* hit;
    f32 h;
    MarioWork* w = marioGetPtr();
    h = w->wPlayerCollisionBox.y;
    if (w->flags & MARIO_FLAG_TUBE_MODE) {
        h *= 0.5f;
    }
    pos = w->position;
    pos.y += h;
    {
    Vec dir = {0.0f, -1.0f, 0.0f};
    hit = marioHitCheckVecInl(&pos, &dir, &outPos, &outNrm, &d);
    }
    w->groundY = hit ? outPos.y : -3000.0f;
    return hit;
}

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
    if (x) {
    }
    {
    Vec dir = {0.0f, -1.0f, 0.0f};
    hit = marioHitCheckVecInl(&pos2, &dir, &outPos, &outNrm, &d);
    }
    *out = hit ? outPos.y : -1.0f;
    return hit;
}

extern void marioSetFallPara(void);
extern void marioChgMotSub(MarioMotion id, s32 callPost);

static inline BOOL marioChkGndOk(void) {
    MarioWork* w = marioGetPtr();
    if (w->flags & MARIO_FLAG_FLOORS_DISABLED) {
        return FALSE;
    }
    if (w->position.y >= 5000.0f) {
        return FALSE;
    }
    return TRUE;
}

void marioChkGnd(void) {
    f32 b, y, c;
    f32 sy, sb, sc;
    f32 d;
    HitEntry* hit;
    MarioWork* w = marioGetPtr();
    if (vivianGetStatus() == 2 || vivianGetStatus() == 1 || vivianGetStatus() == 3) {
        w->hitobjStandOn = marioSearchGround(&y, &b, &c, 37.0f, 0.0f);
        w->hitobjUnder = marioSearchUnder();
        return;
    }
    w->hitobjStandOn = NULL;
    if (w->characterId == CHARACTER_BOWSER) {
        kpaClearHitobjRide();
    }
    w->hitobjUnder = marioSearchUnder();
    if (w->flags & (MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING)) {
        return;
    }
    if (!marioChkGndOk()) {
        return;
    }
    if (!(w->flags & MARIO_FLAG_FORCE_WALL)) {
        if (w->flags & MARIO_FLAG_TUBE_MODE) {
            hit = marioSearchGroundRoll(&y, &b, &c, 11.0f, w->jumpVelocityY);
            if (hit) {
                w->hitobjStandOn = hit;
                w->groundY = (s32)(100.0f * y + 0.5f) / 100.0f;
            }
        } else {
            if (w->characterId == CHARACTER_BOWSER) {
                hit = kpaSearchGround(&y, &b, &c, 11.0f);
            } else {
                hit = marioSearchGround(&y, &b, &c, 11.0f, w->jumpVelocityY);
            }
            if (hit) {
                w->hitobjStandOn = hit;
                w->groundY = (s32)(100.0f * y + 0.5f) / 100.0f;
            }
        }
    } else {
        y = w->position.y;
        if (w->characterId == CHARACTER_BOWSER) {
            if (w->airTimer == 0) {
                marioChgMot(MARIO_MOTION_FALL);
                marioSetFallPara();
                return;
            }
        } else {
            if (w->airTimer == 0) {
                marioChgMot(MARIO_MOTION_FALL);
                marioSetFallPara();
                return;
            }
        }
    }
    if (w->currMotionId == MARIO_MOTION_SHIP) {
        if (marioChkShipMoveMode()) {
            if (!w->hitobjStandOn) {
                marioShipChgFall();
            } else {
                w->flags &= ~MARIO_FLAG_IS_FALLING;
            }
        }
        return;
    }
    if (w->currMotionId != MARIO_MOTION_DAMAGE && w->hitobjStandOn && (hitGetAttr(w->hitobjStandOn) & 0x200)) {
        if (w->jumpVelocityY >= 0.0f) {
            marioSetFallPara();
        }
        set_damage_root_ypos(w->position.y);
        marioChgMot(MARIO_MOTION_DAMAGE);
        return;
    }
    if (w->hitobjStandOn && (hitGetAttr(w->hitobjStandOn) & 0x800)) {
        if (vivianGetStatus() != 2 && vivianGetStatus() != 1 && vivianGetStatus() != 3) {
            set_damage_root_ypos(w->position.y);
            marioChgMot(MARIO_MOTION_DAMAGE_TOGE);
        }
        return;
    }
    if (w->airTimer == 0) {
        if (!(w->flags & MARIO_FLAG_TUBE_MODE)) {
            hit = marioChkFrontStep(&sy, &sb, &sc);
            d = sy - w->position.y;
            if (hit && d <= 11.0f && d >= 5.0f && w->groundSlope <= 30.0f) {
                w->stepUpY = sy;
                if (w->baseSpeed) {
                    if (!((u32)w->dispFlags & MARIO_DISP_FLAG_SLIT)) {
                        marioChgMot(MARIO_MOTION_UPSTAIRS);
                    } else {
                        marioChgMotSub(MARIO_MOTION_UPSTAIRS, 0);
                    }
                    return;
                }
            }
        } else {
            hit = marioChkFrontStep(&sy, &sb, &sc);
            d = sy - w->position.y;
            if (hit && d <= 11.0f && d >= 5.0f && w->groundSlope <= 30.0f) {
                w->stepUpY = sy;
                if (w->baseSpeed) {
                    roll_upstairs_on();
                    return;
                }
            }
        }
    }
    w->position.y = y;
    if (w->characterId == CHARACTER_BOWSER) {
        hit = kpaChkHitobjRide();
    } else {
        hit = w->hitobjStandOn;
    }
    if (hit) {
        if (w->flags & MARIO_FLAG_TUBE_MODE) {
            marioChkGndRoll(w);
        }
        return;
    }
    if (!(w->flags & MARIO_FLAG_TUBE_MODE)) {
        if (w->flags & MARIO_FLAG_IS_FALLING) {
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
        if (w->flags & MARIO_FLAG_IS_FALLING) {
            return;
        }
        w->flags &= ~MARIO_FLAG_IS_JUMPING;
        w->flags |= MARIO_FLAG_IS_FALLING;
        w->jumpVelocityY = -0.4f;
        w->jumpAccelerationY = -0.06f;
        w->jumpJerkY = -0.04f;
        w->jumpSnapY = -0.02f;
    }
}

void marioChkGnd2(void) {
    f32 b, y, c;
    HitEntry* hit;
    f32 d;
    MarioWork* w = marioGetPtr();
    if (!(w->flags & MARIO_FLAG_IS_JUMPING)) {
        if (w->characterId != CHARACTER_BOWSER) {
            hit = marioSearchGround(&y, &b, &c, 0.1f, w->jumpVelocityY);
        } else {
            hit = kpaSearchGround(&y, &b, &c, 0.1f);
        }
        if (hit) {
            d = w->position.y - y;
            if (d < 0.0f && d >= -10.0f) {
                w->hitobjStandOn = hit;
                w->position.y = y;
                w->flags &= ~(MARIO_FLAG_USE_FORCE_DIRECTION | MARIO_FLAG_STOP_AIR_MOVEMENT);
                w->airTimer = 0;
                w->jumpPeakAirTime = 0;
                if (w->characterId == CHARACTER_BOWSER) {
                    if (w->controlStickSensitivity || !kpaFireAttackCheck()) {
                        s32 sx = (s8)w->wStickDir1;
                        s32 sy = (s8)w->wStickDir2;
                        if (sx * sx + sy * sy > 0xbd1) {
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
                if (w->characterId != CHARACTER_BOWSER) {
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


HitEntry* marioSearchGround(f32* y, f32* b, f32* c, f32 spd, f32 h) {
    f32 dist;
    f32 best, yl, sx, sz, x0, y0, z0, a;
    f32 x, z;
    f32 lim;
    f32 dir;
    f32 sn;
    HitEntry* found;
    s32 i;
    HitEntry* hit;
    MarioWork* w = marioGetPtr();
    w->groundRot.x = 0.0f;
    w->groundRot.y = 0.0f;
    w->groundRot.z = 0.0f;
    w->groundSlope = 0.0f;
    if (w->flags & MARIO_FLAG_FORCE_WALL) {
        *y = w->position.y;
        return NULL;
        if (b) {
        }
    }
    if (w->flags & MARIO_FLAG_FLOORS_DISABLED) {
        *y = w->position.y;
        return NULL;
    }
    *y = w->position.y;
    spd = __fabsf(spd);
    spd += 0.01f;
    yl = 5.0f + __fabsf(h);
    *b = -1.0f;
#ifdef VERSION_G8ME01
    if (w->currMotionId != MARIO_MOTION_HAMMER_2) {
        if (w->baseSpeed) {
            a = revise360(w->directionView - w->targetYaw);
        } else {
            a = toMovedir(w->displayDirection);
        }
    } else {
        a = toMovedir(w->savedDisplayDirection);
    }
#else
    if (w->baseSpeed) {
        a = revise360(w->directionView - w->targetYaw);
    } else {
        a = toMovedir(w->displayDirection);
    }
#endif
    if (w->flags & MARIO_FLAG_PAPER_MODE) {
        a = (3.1415927f * a) / 180.0f;
        sn = (f32)sin(a);
        sx = 0.425f * (w->wPlayerCollisionBox.x * sn);
        sn = (f32)cos(a);
        sz = 0.425f * (w->wPlayerCollisionBox.x * -sn);
    } else {
        a = (3.1415927f * a) / 180.0f;
        sn = (f32)sin(a);
        sx = 0.375f * (w->wPlayerCollisionBox.x * sn);
        sn = (f32)cos(a);
        sz = 0.375f * (w->wPlayerCollisionBox.x * -sn);
    }
    y0 = w->position.y;
    {
    Vec dir = {0.0f, -1.0f, 0.0f};
    Vec outPos;
    Vec outNrm;
    x0 = w->position.x;
    found = NULL;
    z0 = w->position.z;
    best = -10000.0f;
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
            v.x = x0 + x;
            v.y = y0 + yl;
            v.z = z0 + z;
            dist = yl + spd;
            hit = marioHitCheckVecInl(&v, &dir, &outPos, &outNrm, &dist);
        }
        if (hit) {
            if ((u32)w->dispFlags & MARIO_DISP_FLAG_SLIT) {
                lim = 75.0f;
            } else if (w->currMotionId == MARIO_MOTION_HIP) {
                lim = 75.0f;
            } else {
                lim = 50.0f;
            }
            if (marioHitNrmAnglePtr(&outNrm) >= lim) {
                hit = NULL;
            }
        }
        if (hit) {
            w->groundSlope = marioHitNrmAnglePtr(&outNrm);
            if (outPos.y < y0 && __fabsf(outPos.y - y0) >= 2.0f) {
                if (__fabsf(outPos.y - y0) > __fabsf(h)) {
                    *b = __fabsf(y0 - outPos.y);
                    *c = angleABf(x0, z0, outPos.x, outPos.z);
                } else if (outPos.y > best) {
                    *y = outPos.y;
                    best = outPos.y;
                    *b = 0.0f;
                    *c = angleABf(x0, z0, outPos.x, outPos.z);
                    marioUpdateCamCollision(outPos);
                    found = hit;
                }
            } else if (__fabsf(outPos.y - y0) <= 11.0f && outPos.y > best) {
                *y = outPos.y;
                best = outPos.y;
                *b = 0.0f;
                *c = angleABf(x0, z0, outPos.x, outPos.z);
                marioUpdateCamCollision(outPos);
                found = hit;
            }
        }
    }
    }
    if (found) {
        return found;
    }
    return NULL;
}

HitEntry* marioSearchGroundRoll(f32* y, f32* b, f32* c, f32 spd, f32 h) {
    f32 dist;
    f32 lim, best, hh, x0, y0, z0, a, r;
    f32 sn;
    HitEntry* found;
    s32 i;
    s32 end;
    HitEntry* hit;
    MarioWork* w = marioGetPtr();
    w->groundRot.x = 0.0f;
    w->groundRot.y = 0.0f;
    w->groundRot.z = 0.0f;
    if (w->flags & MARIO_FLAG_FORCE_WALL) {
        *y = w->position.y;
        return NULL;
        if (b) {
        }
    }
    if (w->flags & MARIO_FLAG_FLOORS_DISABLED) {
        *y = w->position.y;
        return NULL;
    }
    hh = w->wPlayerCollisionBox.y;
    spd = __fabsf(spd);
    found = NULL;
    *y = w->position.y;
    *b = -1.0f;
    spd += 0.01f;
    best = -10000.0f;
    {
    Vec dir = {0.0f, -1.0f, 0.0f};
    Vec outPos;
    Vec outNrm;
    f32 x, z;
    y0 = w->position.y;
    x0 = w->position.x;
    z0 = w->position.z;
    if (__fabsf(w->groundSlope) < 0.01f) {
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
            a = (3.1415927f * (90.0f + w->directionView)) / 180.0f;
            x = 8.6399994f * (f32)sin(a);
            sn = (f32)cos(a);
            z = 8.6399994f * -sn;
            break;
        case 2:
            a = (3.1415927f * (w->directionView - 90.0f)) / 180.0f;
            x = 8.6399994f * (f32)sin(a);
            sn = (f32)cos(a);
            z = 8.6399994f * -sn;
            break;
        case 3:
            z = x = 0.0f;
            movePos(&x, &z, 3.0f, w->directionView);
            break;
        case 4:
            z = x = 0.0f;
            break;
        case 5:
            a = (3.1415927f * (90.0f + w->directionView)) / 180.0f;
            x = 8.6399994f * (f32)sin(a);
            sn = (f32)cos(a);
            z = 8.6399994f * -sn;
            movePos(&x, &z, 3.0f, w->directionView);
            break;
        case 6:
            a = (3.1415927f * (w->directionView - 90.0f)) / 180.0f;
            x = 8.6399994f * (f32)sin(a);
            sn = (f32)cos(a);
            z = 8.6399994f * -sn;
            movePos(&x, &z, 3.0f, w->directionView);
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
            r = marioHitNrmAnglePtr(&outNrm);
            if (r <= 105.0f && r >= 75.0f) {
                continue;
            }
            w->groundSlope = r;
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
    return NULL;
}

void marioChkToge(void) {
    MarioWork* w = marioGetPtr();
    s32 attr;
    if (vivianGetStatus() == 2 || vivianGetStatus() == 1 || vivianGetStatus() == 3) {
        return;
    }
    if (w->hitobjFront != NULL) {
        attr = hitGetAttr(w->hitobjFront);
        if (w->currMotionId != MARIO_MOTION_DAMAGE_TOGE && (attr & 0x800) && w->frontWallDist < 0.5f * w->wPlayerCollisionBox.x) {
            set_damage_root_ypos(w->position.y);
            marioChgMot(MARIO_MOTION_DAMAGE_TOGE);
        }
    }
    if (w->hitobjWall != NULL) {
        attr = hitGetAttr(w->hitobjWall);
        if (w->currMotionId != MARIO_MOTION_DAMAGE_TOGE && (attr & 0x800)) {
            set_damage_root_ypos(w->position.y);
            marioChgMot(MARIO_MOTION_DAMAGE_TOGE);
        }
    }
    if (w->hitobjHead != NULL) {
        attr = hitGetAttr(w->hitobjHead);
        if (w->currMotionId != MARIO_MOTION_DAMAGE_TOGE && (attr & 0x800)) {
            set_damage_root_ypos(w->position.y);
            marioChgMot(MARIO_MOTION_DAMAGE_TOGE);
        }
    }
}

HitEntry* marioChkLandon(f32* a, f32 d) {
    f32 b, c;
    HitEntry* ret;
    MarioWork* w = marioGetPtr();
    if (w->characterId != CHARACTER_BOWSER) {
        if (!(w->flags & MARIO_FLAG_TUBE_MODE)) {
            ret = marioSearchGround(a, &b, &c, d, d);
        } else {
            ret = marioSearchGroundRoll(a, &b, &c, d, d);
        }
    } else {
        ret = marioLandonChk_old(a, d);
    }
    return ret;
}

typedef struct LandonRay {
    Vec pos;
    Vec dir;
} LandonRay;

static inline f32 landonNrmAngle(f32 nx, f32 ny, f32 nz) {
    MarioWork* mp = marioGetPtr();
    f32 y100 = 100.0f * ny;
    mp->groundRot.x = (s32)(100.0f * (angleABf(0.0f, 0.0f, 100.0f * nz, y100) - 180.0f) + 0.5f) / 100.0f;
    mp->groundRot.y = 0.0f;
    mp->groundRot.z = (s32)(100.0f * (angleABf(0.0f, 0.0f, 100.0f * nx, y100) - 180.0f) + 0.5f) / 100.0f;
    return angleABf(0.0f, 0.0f, sqrtf(nx * nx + nz * nz), -ny);
}

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
    MarioWork* w = marioGetPtr();
    f32 a;

    w->groundRot.x = 0.0f;
    w->groundRot.y = 0.0f;
    w->groundRot.z = 0.0f;
    w->groundSlope = 0.0f;
    if (w->flags & MARIO_FLAG_FORCE_WALL) {
        *outY = w->position.y;
        return NULL;
    }
    if (w->flags & MARIO_FLAG_FLOORS_DISABLED) {
        *outY = w->position.y;
        return NULL;
    }
    range = 0.01f + __fabsf(spd);
    hh = 0.75f * w->wPlayerCollisionBox.y;
    if (w->baseSpeed) {
        a = revise360(w->directionView - w->targetYaw);
    } else {
        a = toMovedir(w->displayDirection);
    }
    a = (3.1415927f * a) / 180.0f;
    sn = (f32)sin(a);
    ox = 0.25f * (w->wPlayerCollisionBox.x * sn);
    sn = (f32)cos(a);
    oz = 0.25f * (w->wPlayerCollisionBox.x * -sn);
    px = w->position.x;
    py = w->position.y;
    pz = w->position.z;
    if (w->characterId == CHARACTER_BOWSER) {
        f32 gy, gb, gc;
        hit = kpaSearchGround(&gy, &gb, &gc, 11.0f);
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
            w->groundSlope = landonNrmAngle(nx, ny, nz);
            d = (s32)(100.0f * ((ystart - hy) - dist) + 0.5f) / 100.0f;
            if (__fabsf(d) < 0.1f) {
                *outY = (s32)(100.0f * hy + 0.5f) / 100.0f;
                return hit;
            }
        }
    }
    return NULL;
}

HitEntry* marioChkLandon2(f32* y, f32 spd, f32 ofs) {
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
    void* vp;
    MarioWork* w = marioGetPtr();
    vp = (void*)y;
    w->groundRot.x = 0.0f;
    w->groundRot.y = 0.0f;
    w->groundRot.z = 0.0f;
    w->groundSlope = 0.0f;
    if (w->flags & MARIO_FLAG_FORCE_WALL) {
        *((f32*)vp) = w->position.y;
        return NULL;
    }
    if (w->flags & MARIO_FLAG_FLOORS_DISABLED) {
        *((f32*)vp) = w->position.y;
        return NULL;
    }
    d0 = 0.1f + __fabsf(spd);
    yl = 0.5f * w->wPlayerCollisionBox.y;
    if (w->baseSpeed) {
        a = w->directionView;
    } else {
        a = toMovedir(w->displayDirection);
    }
    a = (3.1415927f * a) / 180.0f;
    sn = (f32)sin(a);
    sx = 0.25f * (w->wPlayerCollisionBox.x * sn);
    sn = (f32)cos(a);
    sz = 0.25f * (w->wPlayerCollisionBox.x * -sn);
    nsx = -sx;
    d1 = yl + d0;
    py = w->position.y + ofs;
    pyl = py + yl;
    x0 = w->position.x;
    z0 = w->position.z;
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
            w->groundSlope = marioHitNrmAnglePtr(&outNrm);
            if (__fabsf((s32)(100.0f * ((pyl - outPos.y) - dist) + 0.5f) / 100.0f) < 0.1f) {
                *((f32*)vp) = outPos.y;
                return hit;
            }
        }
    }
    }
    return NULL;
}

extern HitEntry* kpaSearchHead(Vec* pos, f32* h, f32 d);
extern void marioClearJumpPara(void);
extern void marioChgStayMotion(void);

typedef struct { u8 pad[0x15]; char name[16]; } MobjNameView;

static inline BOOL marioChkHeadBlock(HitEntry* hit) {
    MobjNameView* mobj;
    char* name;
    if (!hit) {
        return FALSE;
    }
    if (!(hitGetAttr(hit) & 0x80000000)) {
        return FALSE;
    }
    mobj = (MobjNameView*)mobjHitObjPtrToPtr(hit);
    if (!strcmp(mobj->name, "MOBJ_PowerUpBlock") || !strcmp(mobj->name, "MOBJ_SaveBlock") ||
        !strcmp(mobj->name, "MOBJ_RecoveryBlock") || !strcmp(mobj->name, "MOBJ_CoinTakenRecoveryBlock")) {
        return TRUE;
    }
    return FALSE;
}

static inline f32 marioGetHeightInl(void) {
    MarioWork* m = marioGetPtr();
    f32 ph = m->wPlayerCollisionBox.y;
    if (m->currMotionId == MARIO_MOTION_YOSHI) {
        PartyEntry* p = partyGetPtr(marioGetPartyId());
        if (p) {
            ph = partyGetHeight(p);
        }
    }
    return ph;
}

void marioHitStrippedPool2(f32* f) {
    f[0] = 18.0f;
}

f32 marioChkOverhead(void) {
    Vec pa = {0.0f, 0.0f, 0.0f};
    Vec pb = {0.0f, -1.0f, 0.0f};
    Vec pc = {0.0f, 0.0f, 0.0f};
    Vec pd = {0.0f, 0.0f, 0.0f};
    Vec pos;
    s32 hj;
    f32 h;
    MarioWork* w = marioGetPtr();
    if (w->currMotionId == MARIO_MOTION_JUMP_3) {
        return w->position.y;
    }
    if (w->currMotionId == MARIO_MOTION_JABARA) {
        pos = w->position;
        h = w->wPlayerCollisionBox.y;
        if (!jabaraNoHitChk()) {
            h += 5.0f;
            w->hitobjHead = marioSearchHead_jabara(&pos, &h, &hj, 0.0f);
            if (!w->hitobjHead) {
                w->hitobjHead = marioSearchHead(&pos, &h, (w->flags & MARIO_FLAG_IS_JUMPING) ? w->jumpVelocityY : 10.0f);
            }
        }
    } else if (w->flags & MARIO_FLAG_TUBE_MODE) {
        pos = w->position;
        w->hitobjHead = marioSearchHead_roll(&pos, &h, 40.0f);
    } else {
        f32 d;
        pos = w->position;
        h = w->wPlayerCollisionBox.y;
        d = (w->flags & MARIO_FLAG_IS_JUMPING) ? w->jumpVelocityY : 10.0f;
        if (w->characterId == CHARACTER_BOWSER) {
            w->hitobjHead = kpaSearchHead(&pos, &h, d);
        } else {
            w->hitobjHead = marioSearchHead(&pos, &h, d);
        }
    }
    w->position.x = pos.x;
    if (w->hitobjHead) {
        if (marioChkHeadBlock(w->hitobjHead)) {
            w->baseSpeed = 0.0f;
            w->flags |= MARIO_FLAG_STOP_AIR_MOVEMENT;
        }
        w->flags |= MARIO_FLAG_IS_TOUCHING_CEILING;
        if (w->characterId == CHARACTER_BOWSER) {
            marioItemGetDisable();
            if (!kpaGetBodyStatus()) {
                marioClearJumpPara();
            }
        }
        pos.y = (s32)(10.0f * (pos.y - marioGetHeightInl()) + 0.5f) / 10.0f - 1.0f;
        if (!(w->flags & MARIO_FLAG_IS_FALLING) && pos.y < w->lastGroundPos.y) {
            pos.y = w->lastGroundPos.y;
            if (!(w->flags & MARIO_FLAG_TUBE_MODE) && w->currMotionId != MARIO_MOTION_SHIP && w->currMotionId != MARIO_MOTION_PLANE &&
                w->currMotionId != MARIO_MOTION_VIVIAN && w->characterId != CHARACTER_BOWSER && !marioChkSlitEnd()) {
                if (w->baseSpeed == 0.0f) {
                    if (w->currMotionId != MARIO_MOTION_STAY) {
                        marioChgStayMotion();
                    }
                } else {
#ifdef VERSION_G8ME01
                    {
                        /* FAKE: redundant second test reproduces the target's double beq */
                        s32 m = w->baseSpeed > w->mBaseWalkSpeed ? 2 : 1;
                        if (m != w->prevMotionId && m != w->prevMotionId) {
                            marioChgMot(w->baseSpeed > w->mBaseWalkSpeed ? 2 : 1);
                        }
                    }
#else
                    marioChgMot(w->baseSpeed > w->mBaseWalkSpeed ? 2 : 1);
#endif
                }
            }
        }
        if (w->currMotionId == MARIO_MOTION_JUMP || w->currMotionId == MARIO_MOTION_JUMP_STAND) {
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MARIO_HEAD1), &w->position);
            marioClearJumpPara();
            w->flags &= ~(MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING | MARIO_FLAG_IS_STEPPING | MARIO_FLAG_HAS_INPUT_JUMP);
            if (w->characterId != CHARACTER_BOWSER) {
                marioChgMot(MARIO_MOTION_FALL);
                marioSetFallPara();
            }
            w->position.y = pos.y;
        }
    } else if (!(w->flags & MARIO_FLAG_TUBE_MODE)) {
        if (h >= 0.0f && h <= 5.0f) {
            w->flags |= MARIO_FLAG_IS_TOUCHING_CEILING;
        } else {
            w->flags &= ~MARIO_FLAG_IS_TOUCHING_CEILING;
        }
    } else {
        if (h >= 0.0f && h + w->wPlayerCollisionBox.y <= 47.0f) {
            w->flags |= MARIO_FLAG_IS_TOUCHING_CEILING;
        } else {
            w->flags &= ~MARIO_FLAG_IS_TOUCHING_CEILING;
        }
    }
    return pos.y;
}

f32 marioChkOverheadJabara(s32* sideOut, f32* distOut) {
    Vec pos;
    f32 margin = 5.0f;
    f32 h;
    MarioWork* w;
    w = marioGetPtr();
    *sideOut = 0;
    *distOut = -1.0f;
    pos = w->position;
    h = w->wPlayerCollisionBox.y;
    if (w->currMotionId == MARIO_MOTION_JUMP_3) {
        return pos.y;
    }
    if (!jabaraNoHitChk()) {
        h += margin;
        w->hitobjHead = marioSearchHead_jabara(&pos, &h, sideOut, 0.0f);
        if (!w->hitobjHead) {
            h = w->wPlayerCollisionBox.y + margin;
            w->hitobjHead = marioSearchHead(&pos, &h, 0.0f);
        }
    }
    w->position.x = pos.x;
    if (w->hitobjHead) {
        w->flags |= MARIO_FLAG_IS_TOUCHING_CEILING;
        pos.y = pos.y - w->wPlayerCollisionBox.y - 0.1f;
        if (w->currMotionId == MARIO_MOTION_JUMP) {
            marioChgMot(MARIO_MOTION_STAY);
        }
    } else if (h > 0.0f) {
        *distOut = h;
        w->flags |= MARIO_FLAG_IS_TOUCHING_CEILING;
    } else {
        w->flags &= ~MARIO_FLAG_IS_TOUCHING_CEILING;
    }
    return pos.y;
}

static inline HitEntry* marioHitCheck2Val(Vec p, Vec d, Vec* op, Vec* on, f32* dist) {
    return marioHitCheck2(&p, &d, op, on, dist);
}

static inline void marioSearchHeadOfs(MarioWork* w, f32 deg, f32* px, f32* pz) {
    f32 r = (3.1415927f * deg) / 180.0f;
    f32 t;
    t = sin(r);
    *px = 0.5f * ((w->wPlayerCollisionBox.x - 0.5f) * t);
    t = cos(r);
    *pz = 0.5f * ((w->wPlayerCollisionBox.x - 0.5f) * -t);
}

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
    void* vp;
    MarioWork* w = marioGetPtr();
    vp = (void*)h;
    if (w->baseSpeed) {
        hh = w->directionView;
    } else {
        hh = toMovedir(w->displayDirection);
    }
    marioSearchHeadOfs(w, hh, &sx, &sz);
    a = *((f32*)vp);
    hh = 0.5f * a;
    top = a + dd;
    *((f32*)vp) = -1.0f;
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
            p = v;
            d = dir;
            hit = marioHitCheck2(&p, &d, &outPos, &outNrm, &dist);
        }
        if (hit) {
            f32 r = marioHitNrmAngle(&outNrm);
            if (!(w->flags & MARIO_FLAG_TUBE_MODE)) {
                f32 lim = ((u32)w->dispFlags & MARIO_DISP_FLAG_SLIT) ? 75.0f : 50.0f;
                if (r <= 180.0f - lim && r >= lim) {
                    hit = NULL;
                }
            }
        }
        if (w->flags & MARIO_FLAG_TUBE_MODE) {
            if (hit) {
                outPos.y = (s32)(100.0f * outPos.y + hh) / 100.0f;
                outPos.y = outPos.y - 0.05f * a;
                if (pos->y + a >= outPos.y) {
                    pos->y = outPos.y;
                    *((f32*)vp) = 0.0f;
                    return hit;
                }
                *((f32*)vp) = outPos.y - pos->y - a;
                return NULL;
            }
        } else {
            if (hit) {
                outPos.y = (s32)(100.0f * outPos.y + hh) / 100.0f;
#ifdef VERSION_G8ME01
                if (w->jumpVelocityY < -10.0f) {
                    *((f32*)vp) = outPos.y - pos->y - a;
                    return NULL;
                }
#endif
                if (pos->y + a >= outPos.y) {
                    pos->y = outPos.y;
                    *((f32*)vp) = 0.0f;
                    return hit;
                }
                *((f32*)vp) = outPos.y - pos->y - a;
                return NULL;
            }
        }
    }
    return NULL;
}

inline f32 marioSearchHead_roll_cos(f32 a) {
    return (f32)cos(a);
}

HitEntry* marioSearchHead_roll(Vec* pos, f32* h, f32 d) {
    f32 a;
    Vec outPos, outNrm;
    f32 x, z;
    f32 dist;
    f32 base, top;
    Vec* pdir;
    s32 i;
    HitEntry* hit;
    MarioWork* w = marioGetPtr();
    *h = -1.0f;
    base = w->wPlayerCollisionBox.y;
    i = 0;
    top = base + d;
    for (; i < 5; i++) {
        switch (i) {
        case 0:
            z = x = 0.0f;
            break;
        case 1:
            a = (3.1415927f * (90.0f + w->directionView)) / 180.0f;
            x = 9.0f * (f32)sin(a);
            z = 9.0f * -marioSearchHead_roll_cos(a);
            movePos(&x, &z, 3.0f, w->directionView);
            break;
        case 2:
            a = (3.1415927f * (w->directionView - 90.0f)) / 180.0f;
            x = 9.0f * (f32)sin(a);
            z = 9.0f * -marioSearchHead_roll_cos(a);
            movePos(&x, &z, 3.0f, w->directionView);
            break;
        case 3:
            a = (3.1415927f * (90.0f + w->directionView)) / 180.0f;
            x = 9.0f * (f32)sin(a);
            z = 9.0f * -marioSearchHead_roll_cos(a);
            movePos(&x, &z, 3.0f, 180.0f + w->directionView);
            break;
        case 4:
            a = (3.1415927f * (w->directionView - 90.0f)) / 180.0f;
            x = 9.0f * (f32)sin(a);
            z = 9.0f * -marioSearchHead_roll_cos(a);
            movePos(&x, &z, 3.0f, 180.0f + w->directionView);
            break;
        }
        {
            Vec v = {0.0f, 0.0f, 0.0f};
            Vec dir = {0.0f, 1.0f, 0.0f};
            v.x = pos->x + x;
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
            {
                f32 t = outPos.y - pos->y - base;
                if (*h < 0.0f) {
                    *h = t;
                } else if (*h > t) {
                    *h = t;
                }
            }
        }
    }
    return NULL;
}

HitEntry* marioSearchHead_jabara(Vec* pos, f32* h, s32* side, f32 d) {
    Vec v, dir, outPos, outNrm;
    f32 dist;
    f32 base, top, off;
    HitEntry* hit;
    s32 i;
    MarioWork* w = marioGetPtr();
    *side = 0;
    i = 0;
    base = *h;
    off = -23.7f;
    *h = -1.0f;
    top = base + d;
    {
    dir = (Vec){0.0f, 1.0f, 0.0f};
    for (; i < 8; i++, off += 5.8f) {
        dist = top;
        v.x = pos->x + off * (f32)sin((3.1415927f * w->targetYaw) / 180.0f);
        v.z = pos->z - off * (f32)cos((3.1415927f * w->targetYaw) / 180.0f);
        v.y = 0.5f + pos->y;
        hit = marioHitCheck2(&v, &dir, &outPos, &outNrm, &dist);
        if (hit) {
            if (hitGetAttr(hit) & 0x40000) {
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
    }
    return NULL;
}

void marioCheckWall(f32 dx, f32 dz) {
    Vec pos;
    f32 h;
    HitEntry* hit;
    MarioWork* w = marioGetPtr();
    w->frontWallDist = 1000.0f;
    if (dx && w->airTimer == 0) {
        if (w->wallPushDist < dx) {
            w->wallPushDist = dx;
            w->wallPushDir = dz;
            w->wallPushHold = 14;
        } else {
            if (--w->wallPushHold <= 0) {
                w->wallPushHold = 0;
                w->wallPushDist = dx;
                w->wallPushDir = dz;
            }
        }
        w->moveDirection = w->wallPushDir;
    } else {
        w->wallPushDist = 0.0f;
    }
    if (!dx) {
        marioSearchFrontWall_staying(dz);
        dou10_yoko_yari(&w->frontWallDist);
        return;
    }
    pos = w->position;
    w->hitobjFrontWall = NULL;
    hit = searchFrontWall(&pos, &h, dx, dz, w->wPlayerCollisionBox.z);
    if (hit) {
        if (w->hitobjFrontWall) {
            w->hitobjFront = w->hitobjFrontWall;
        } else {
            w->hitobjFront = hit;
        }
        if (marioChkBero(hit)) {
            if (dx) {
                w->hitobjPush = hit;
                w->hitobjWall = hit;
            }
            movePos(&w->position.x, &w->position.z, dx, dz);
        } else {
            w->frontWallDist = h;
            w->hitobjWall = hit;
#ifdef VERSION_G8ME01
            {
                s32 lim = strncmp(hitGetName(hit), "dokan", 5) != 0 ? 20 : 10;
                if (++w->wallPushTimer >= lim) {
                    w->wallPushTimer = lim;
                    if (!(hitGetAttr(hit) & 0x80) ? TRUE : marioGetPtr()->currMotionId == MARIO_MOTION_SHIP) {
                        w->hitobjPush = hit;
                    }
#else
            if (++w->wallPushTimer >= 20) {
                w->wallPushTimer = 20;
                if (!(hitGetAttr(hit) & 0x80) ? TRUE : marioGetPtr()->currMotionId == MARIO_MOTION_SHIP) {
                    w->hitobjPush = hit;
#endif
                }
            }
            w->position = pos;
        }
    } else {
        w->position = pos;
    }
    marioChkWallAround(&w->position, 0, dx, dz, w->wPlayerCollisionBox.z, w->wPlayerCollisionBox.y);
    dou10_yoko_yari(&w->frontWallDist);
}

void marioSearchFrontWall_staying(f32 dz) {
    Vec pos;
    f32 h;
    f32 h2;
    f32 ang;
    f32 r;
    HitEntry* hit;
    MarioWork* w = marioGetPtr();
    w->wallPushTimer = 0;
    r = 0.5f * w->wPlayerCollisionBox.x;
    if (w->flags & MARIO_FLAG_PAPER_MODE) {
        r = 0.0f;
    }
    pos = w->position;
    hit = searchFrontWall(&pos, &h, r, w->directionView - w->targetYaw, w->wPlayerCollisionBox.z);
    if (w->currMotionId != MARIO_MOTION_HAMMER_2) {
        w->hitobjFront = hit;
    } else {
        w->hitobjFront = NULL;
    }
    if (!hit) {
        f32 a = toMovedir(w->displayDirection);
        pos = w->position;
        hit = searchFrontWall(&pos, &h, r, a, w->wPlayerCollisionBox.z);
        if (w->currMotionId != MARIO_MOTION_HAMMER_2) {
            w->hitobjFront = hit;
        } else {
            w->hitobjFront = NULL;
        }
    } else if (!strcmp(gp->currentMapName, "dou_10") && vivianGetStatus() != 2 && vivianGetStatus() != 1 && vivianGetStatus() != 3) {
        ang = toMovedir(w->displayDirection);
        pos = w->position;
        movePos(&pos.x, &pos.z, 0.5f * w->wPlayerCollisionBox.x, ang);
        ang += 180.0f;
        hit = searchFrontWall(&pos, &h2, w->wPlayerCollisionBox.x, ang, w->wPlayerCollisionBox.z);
        if (hit && w->wPlayerCollisionBox.x >= h2) {
            w->hitobjFront = hit;
            h = 0.5f * h2;
        }
    }
    if (w->hitobjFront) {
        w->frontWallDist = h;
    }
    if (w->buttonTrg & PAD_BUTTON_A) {
        w->hitobjFrontA = NULL;
        pos = w->position;
        if (searchFrontWall(&pos, &h, r, w->directionView, w->wPlayerCollisionBox.z)) {
            w->hitobjFrontA = w->hitobjFront;
        }
    }
    pos = w->position;
    marioChkWallAround(&pos, 0, 0.0f, dz, w->wPlayerCollisionBox.z, w->wPlayerCollisionBox.y);
    w->position.x = pos.x;
    w->position.z = pos.z;
}

void marioCheckWall_roll(f32 dx, f32 dz) {
    Vec pos;
    Vec outPos;
    Vec outNrm;
    f32 h;
    HitEntry* hit;
    MarioWork* w = marioGetPtr();
    w->frontWallDist = 1000.0f;
    if (dx && w->airTimer == 0) {
        if (w->wallPushDist < dx) {
            w->wallPushDist = dx;
            w->wallPushDir = dz;
            w->wallPushHold = 14;
        } else {
            if (--w->wallPushHold <= 0) {
                w->wallPushHold = 0;
                w->wallPushDist = dx;
                w->wallPushDir = dz;
            }
        }
    } else {
        w->wallPushDist = 0.0f;
    }
    if (dx && w->airTimer == 0) {
        w->moveDirection = w->wallPushDir;
    }
    if (w->flags & MARIO_FLAG_IS_JUMPING) {
        f32 dist = 30.0f;
        Vec dir = {0.0f, 1.0f, 0.0f};
        hit = marioHitCheckVecInl(&w->position, &dir, &outPos, &outNrm, &dist);
        if (hit && dist > 16.8f) {
            hit = NULL;
        }
        w->hitobjHead = hit;
        if (hit) {
            w->position.y = outPos.y - w->wPlayerCollisionBox.y;
        }
    }
    if (!dx) {
        marioSearchFrontWall_staying(dz);
        return;
    }
    if (w->hitobjHead) {
        w->baseSpeed = dx = 0.0f;
        w->jumpVelocityY = 0.0f;
        w->jumpAccelerationY = 0.0f;
        w->jumpJerkY = 0.0f;
        w->jumpSnapY = 0.0f;
    }
    pos = w->position;
    hit = searchFrontWall(&pos, &h, dx, dz, w->wPlayerCollisionBox.z);
    if (hit) {
        w->hitobjFront = hit;
        if (marioChkBeroInl(hit)) {
            if (dx) {
                w->hitobjPush = hit;
                w->hitobjWall = hit;
            }
            movePos(&w->position.x, &w->position.z, dx, dz);
        } else {
            w->frontWallDist = h;
            w->hitobjWall = hit;
            if (++w->wallPushTimer >= 20) {
                w->wallPushTimer = 20;
                if (!(hitGetAttr(hit) & 0x80) ? 1 : marioGetPtr()->currMotionId == MARIO_MOTION_SHIP) {
                    w->hitobjPush = hit;
                }
            }
            w->position = pos;
        }
    } else {
        w->position = pos;
    }
    pos = w->position;
    marioChkWallAround(&pos, 0, dx, dz, w->wPlayerCollisionBox.z, w->wPlayerCollisionBox.y);
    w->position.x = pos.x;
    w->position.z = pos.z;
}

// marioHitCheckVecInl with the direction built after the position copy (the target schedules the pos loads first)
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

static inline void sfwVecSet(Vec* d, Vec v) {
    *d = v;
}

// wall-slide offset: half of the move vector minus its component along the wall normal
static inline void sfwSlide(Vec* n, f32 vx, f32 vz, f32* mx, f32* mz) {
    f32 dot = vx * n->x + vz * n->z;
    *mx = 0.5f * (vx - dot * n->x);
    *mz = 0.5f * (vz - dot * n->z);
}

// FAKE: two-level wrapper so dir2 is allocated as a nested-inline local (lowest stack slot, COLOURING C2)
static inline void sfwVecSetNested(Vec* d, Vec v) {
    sfwVecSet(d, v);
}

HitEntry* searchFrontWall(Vec* pos, f32* h, f32 dx, f32 dz, f32 w) {
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
    MarioWork* mp;

    mp = marioGetPtr();
    if (vivianGetStatus() == 2) {
        return NULL;
    }
    sincosf(&s, &c, dz);
    if (marioChkSts(0x10) && !strcmp(gp->currentMapName, "eki_04") &&
        (__fabsf(mp->groundRot.x) >= 45.0f || __fabsf(mp->groundRot.z) >= 45.0f)) {
        pos->x += dx * s;
        pos->z += dx * c;
        return NULL;
    }
    count = 2;
    if (mp->characterId == CHARACTER_BOWSER) {
        if (mp->stageFlags & 1) {
            count = 3;
            heights[0] = 0.75f * mp->wPlayerCollisionBox.y;
            heights[1] = 0.5f * mp->wPlayerCollisionBox.y;
            heights[2] = 11.0f;
#ifdef VERSION_G8ME01
            if (kpaGetLevel() == 2) {
                count = 4;
                heights[3] = 30.0f;
            }
#endif
        } else {
            heights[0] = 0.75f * mp->wPlayerCollisionBox.y;
            heights[1] = 11.0f;
        }
    } else if (mp->currMotionId == MARIO_MOTION_YOSHI) {
        heights[0] = 0.75f * marioGetHeightInl();
        heights[1] = 11.0f;
    } else if (!(mp->flags & MARIO_FLAG_TUBE_MODE)) {
        heights[0] = VERSION_PICK(27.75f, 26.640001f);
        heights[1] = 11.0f;
        if (mp->airTimer) {
            count = 3;
            heights[2] = 0.1f;
        }
    } else {
        heights[0] = 12.0f;
        heights[1] = 11.0f;
        if (mp->airTimer) {
            count = 3;
            heights[2] = 7.0f;
        }
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
                mp->hitobjFrontWall = hit;
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
        sfwSlide(&bestNrm, s * dx, c * dx, &mx, &mz);
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

HitEntry* dou10_yoko_yari(f32* out) {
    Vec pos, pos2, outPos, outNrm;
    f32 h;
    f32 d;
    HitEntry* hit;
    f32 step;
    MarioWork* w = marioGetPtr();
    if (w->currMotionId == MARIO_MOTION_ROLL) {
        return NULL;
    }
    toMovedir(w->displayDirection);
    hit = w->hitobjFront;
    if (hit) {
        return hit;
    }
    pos = w->position;
    if (!strcmp(gp->currentMapName, "dou_10") && vivianGetStatus() != 2 && vivianGetStatus() != 1 && vivianGetStatus() != 3 && !hit) {
        if (w->flags & MARIO_FLAG_PAPER_MODE) {
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
        if (hit && (hitGetAttr(hit) & 0x800)) {
            w->hitobjFront = hit;
            *out = 0.8f;
        }
        pos = w->position;
        hit = searchFrontWall(&pos, &h, 20.0f, 0.0f, 20.0f);
        if (hit && (hitGetAttr(hit) & 0x800)) {
            w->hitobjFront = hit;
            *out = 0.8f;
        }
        pos = w->position;
        movePos(&pos.x, &pos.z, step, 270.0f);
        hit = searchFrontWall(&pos, &h, 20.0f, 0.0f, 20.0f);
        if (hit && (hitGetAttr(hit) & 0x800)) {
            w->hitobjFront = hit;
            *out = 0.8f;
        }
        if ((!w->flags) && (!w->flags)) {
        }
        pos = w->position;
        movePos(&pos.x, &pos.z, step, 90.0f);
        hit = searchFrontWall(&pos, &h, 20.0f, 0.0f, 20.0f);
        if (hit && (hitGetAttr(hit) & 0x800)) {
            w->hitobjFront = hit;
            *out = 0.8f;
        }
    }
    return hit;
}

HitEntry* dou10_yoko_yari2(f32* out) {
    Vec pos, pos2, outPos, outNrm;
    f32 h;
    f32 d;
    HitEntry* hit;
    MarioWork* w = marioGetPtr();
    toMovedir(w->displayDirection);
    pos = w->position;
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
    if (hit && (hitGetAttr(hit) & 0x800)) {
        w->hitobjFront = hit;
        *out = 0.8f;
    }
    pos = w->position;
    hit = searchFrontWall(&pos, &h, 20.0f, 0.0f, 20.0f);
    if (hit && (hitGetAttr(hit) & 0x800)) {
        w->hitobjFront = hit;
        *out = 0.8f;
    }
    pos = w->position;
    movePos(&pos.x, &pos.z, 8.0f, 270.0f);
    hit = searchFrontWall(&pos, &h, 20.0f, 0.0f, 20.0f);
    if (hit && (hitGetAttr(hit) & 0x800)) {
        w->hitobjFront = hit;
        *out = 0.8f;
    }
    pos = w->position;
    movePos(&pos.x, &pos.z, 8.0f, 90.0f);
    hit = searchFrontWall(&pos, &h, 20.0f, 0.0f, 20.0f);
    if (hit && (hitGetAttr(hit) & 0x800)) {
        w->hitobjFront = hit;
        *out = 0.8f;
    }
    return hit;
}

HitEntry* N_dou10_yoko_yari3(void) {
    HitCheckQuery query;
    f32 s, c;
    f32 x, z;
    f32 y;
    HitEntry* hit;
    MarioWork* w = marioGetPtr();
    if (strcmp(gp->currentMapName, "dou_10")) {
        return NULL;
    }
    x = w->position.x;
    y = w->position.y;
    z = w->position.z;
    movePos(&x, &z, 10.0f, w->directionView);
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
        w->hitobjFront = hit;
        {
            return hit;
        }
    }
    x = w->position.x;
    y = w->position.y;
    z = w->position.z;
    movePos(&x, &z, -10.0f, w->directionView);
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
        w->hitobjFront = hit;
    }
    return hit;
}

static inline BOOL marioHitCheckVecChk(Vec* p, Vec* d, Vec* op, Vec* on, f32* di, HitEntry** out) {
    *out = marioHitCheckVecInl(p, d, op, on, di);
    return *out != NULL;
}

HitEntry* marioChkFrontStep(f32* y, f32* h, f32* ang) {
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
    MarioWork* w = marioGetPtr();
    w->groundRot.x = 0.0f;
    w->groundRot.y = 0.0f;
    w->groundRot.z = 0.0f;
    w->groundSlope = 0.0f;
    hh = (w->flags & MARIO_FLAG_TUBE_MODE) ? 18.5f : 0.5f * w->wPlayerCollisionBox.y;
    *y = w->position.y;
    *h = -1.0f;
    if (w->baseSpeed) {
        a = w->directionView;
    } else {
        a = toMovedir(w->displayDirection);
    }
    a = revise360((s32)(0.5f + a));
    if (w->flags & MARIO_FLAG_TUBE_MODE) {
        r = 3.0f;
    } else {
        r = 0.265f * w->wPlayerCollisionBox.x;
    }
    {f32 c; f32 rad = (3.1415927f * a) / 180.0f;
    s = (f32)sin(rad);
    c = (f32)cos(rad);
    s = r * s;
    a = r * -c;}
    y0 = w->position.y;
    x0 = w->position.x;
    z0 = w->position.z;
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
        w->groundSlope = marioHitNrmAnglePtr(&outNrm);
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

void marioHitStrippedPool(f32* f) {
    f[0] = 0.75f;
    f[1] = -1.0f;
    f[2] = 3.1415927f;
    f[3] = 0.1f;
    f[4] = 2.0f;
}

static inline void wallBase(Vec* pos, Vec* hi, Vec* lo, f32 h, f32* half, f32 w) {
    Vec base = *pos;
    *hi = base;
    hi->y += 11.0f;
    *half = 0.5f * w;
    *lo = base;
    lo->y += h - 1.0f;
}

static inline void wallCopy(Vec* out, Vec* in) {
    Vec t;
    t = *in;
    *out = t;
}

BOOL marioChkWallAround(Vec* pos, s32 mode, f32 unused, f32 dir, f32 w, f32 h) {
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
    if (kpaGetStageViewType() == 0) {
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
    if (mode == 0) {
        MarioWork* m = marioGetPtr();
        if (m->currMotionId == MARIO_MOTION_FALL || m->currMotionId == MARIO_MOTION_FALL_2) {
            lim = 0.25f * w;
        }
    }
    if (lim < 1.0f) {
        lim = 1.0f;
    }
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

BOOL marioSlitChkWallAround(void) {
    s32 flags[8];
    Vec pos;
    s32 i;
    f32 a;
    f32 s, c;
    HitEntry* hit;
    MarioWork* w = marioGetPtr();
    pos = w->position;
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
    { f32 k = 5.0f; pos.y = w->position.y + k; }
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
    pos.y = w->position.y + (w->wPlayerCollisionBox.y - 1.0f);
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

void marioCheckWallShip(f32 dx, f32 dz) {
    Vec pos;
    Vec old;
    Vec diff;
    Vec tmp;
    f32 h;
    f32 max;
    f32 mag;
    HitEntry* hit;
    MarioWork* w = marioGetPtr();
    w->frontWallDist = 1000.0f;
    if (dx && w->airTimer == 0) {
        if (w->wallPushDist < dx) {
            w->wallPushDist = dx;
            w->wallPushDir = dz;
            w->wallPushHold = 14;
        } else {
            if (--w->wallPushHold <= 0) {
                w->wallPushHold = 0;
                w->wallPushDist = dx;
                w->wallPushDir = dz;
            }
        }
    } else {
        w->wallPushDist = 0.0f;
    }
    if (dx && w->airTimer == 0) {
        w->moveDirection = w->wallPushDir;
    }
    if (!dx) {
        marioSearchFrontWall_staying(dz);
        return;
    }
    pos = w->position;
    hit = searchFrontWall(&pos, &h, dx, dz, w->wPlayerCollisionBox.z);
    if (hit) {
        w->hitobjFront = hit;
        if (marioChkBeroInl(hit)) {
            if (dx) {
                w->hitobjPush = hit;
                w->hitobjWall = hit;
            }
            movePos(&w->position.x, &w->position.z, dx, dz);
            pos = w->position;
            return;
        }
        if (!dx) {
            w->wallPushTimer = 0;
        } else {
            w->frontWallDist = h;
            w->hitobjWall = hit;
            if (++w->wallPushTimer >= 20) {
                s32 attr;
                BOOL r;
                w->wallPushTimer = 20;
                attr = hitGetAttr(hit);
                r = !(attr & 0x80) ? TRUE : marioGetPtr()->currMotionId == MARIO_MOTION_SHIP;
                if (r) w->hitobjPush = hit;
            }
        }
        w->position = pos;
    }
    old = pos;
    w->position = pos;
    tmp = pos;
    marioChkWallAround(&w->position, 0, dx, dz, w->wPlayerCollisionBox.z, 4.0f);
    max = w->mBaseDashSpeed;
    PSVECSubtract(&w->position, &old, &diff);
    mag = PSVECMag(&diff);
    if (mag > max) {
        PSVECScale(&diff, &diff, max / mag);
        PSVECAdd(&old, &diff, &w->position);
    }
}

// forward declarations: these functions are defined further down
