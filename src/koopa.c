#include "eff/eff_gonbaba_breath.h"
#include "mario/mario.h"
#include "koopa.h"
#include "mario/mariost.h"
#include "driver/animdrv.h"
#include "pmario_sound.h"
#include "system.h"
#include <dolphin/types.h>
#include <dolphin/mtx.h>
#include "memory.h"
#include <string.h>
#include "koopa_hit.h"
#include <dolphin/pad.h>
#include "driver/camdrv.h"
#include "sfx_name_word.h"
#include "koopa_motion.h"
#include "mario/mario_motion.h"
#include "mario/mario_cam.h"
#include "mot/mot_jump.h"
#include "mario/mario_sbr.h"
#include "mario/mario_balloon.h"
#include "party.h"
#include "mario/mario_hit.h"
#include "mario/mario_kemuri.h"
#include "driver/mapdrv.h"

void kpa2DMain(void);
void kpa3DMain(void);
void kpaFireAttack(void);
void kpaFireMain(void);
void kpaFall(void);
const f32 vec3_802fc920[3] = {0.0f, 0.0f, 0.0f};
extern f32 marioJumpData[];


f32 kpaWidthDat[4] = {20.0f, 60.0f, 84.0f, 186.0f};
f32 baseScaleTbl[4] = {0.3f, 1.0f, 1.4f, 3.1f};
s32 kpaPauseSeTable[28] = {0x10C, 0x10D, 0x10E, 0x10F, 0x111, 0x112, 0x113, 0x114, 0x116, 0x117,
                           0x118, 0x119, 0x11B, 0x11E, 0x11F, 0x120, 0x122, 0x123, 0x124, 0x125,
                           0x127, 0x128, 0x129, 0x12A, 0x12C, 0x12D, 0x12E, 0x12F};
static EffectEntry* fireefp;
// FAKE: never used; pads .sbss to the 8 bytes of the original section; removing it changes the object
static u8* fireefp_pad;

/**
 * Spawns Bowser's fire breath (the Hooktail breath effect) in front of his mouth if none is active:
 * offset by his size and facing, flame size by level (level 3 uses the big flame type),
 * both flame layers turned to his facing.
 */
static inline void kpaFireEntry2(Vec pos) {
    MarioWork* mp = marioGetPtr();
    if (fireefp == NULL) {
        s32 flag = 0;
        s32 ang;
        f32 dir;
        f32 scale;
        f32 x;
        f32 y;
        f32 z;
        f32 sz;
        s32 type;
        EffectEntry* child;
        s32 t;
        dir = (mp->displayDirection >= 90.0f && mp->displayDirection <= 270.0f) ? 1.0f : -1.0f;
        scale = kpaGetBaseScale(kpaGetLevel());
        x = 40.0f * dir * scale + pos.x;
        t = (marioGetPtr()->stageFlags & KPA_STAGE_FLAG_SIDE_SCROLLING) ^ 1;
        y = pos.y + (20.0f * scale + (t == 1 ? 6.0f : 0.0f));
        z = pos.z;
        type = 7;
        sz = 0.4f;
        switch (mp->kpaWork->level) {
        case 0:
            sz = 0.3f;
            break;
        case 1:
            sz = 1.0f;
            break;
        case 2:
            sz = 1.4f;
            break;
        case 3:
            sz = 1.5f;
            type = 0;
            break;
        }
        fireefp = effGonbabaBreathEntry(type, 0, x, y, z, sz);
        flag = 0;
        if (mp->displayDirectionTarget >= 90.0f && mp->displayDirectionTarget <= 270.0f) {
            flag = 1;
        }
        ((EffGonbabaBreathWork*)fireefp->userdata)->yaw = (f32)(flag ? 180 : 0);
        child = ((EffGonbabaBreathWork*)fireefp->userdata)->child;
        if (child) {
            flag = 0;
            if (mp->displayDirectionTarget >= 90.0f && mp->displayDirectionTarget <= 270.0f) {
                flag = 1;
            }
            ang = flag ? 180 : 0;
            ((EffGonbabaBreathWork*)child->userdata)->yaw = (f32)ang;
        }
    }
}

/** Spawns the fire breath at pos (see kpaFireEntry2). */
static void kpaFireEntry(Vec pos) {
    MarioWork* mp = marioGetPtr();
    kpaFireEntry2(pos);
}

/** Starts the looping fire breath sound plus a roar at Bowser's position, unless the loop is already playing. */
static inline void kpaFireSEOn(void) {
    MarioWork* mp = marioGetPtr();
    if (mp->kpaWork->fireSfxId == 0xFFFFFFFFU) {
        mp->kpaWork->fireSfxId = psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_KUPPA_FIRE_BREATH1), &mp->position);
        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_VOICE_KOOPA_FIRE1), &mp->position);
    }
}

/** Stops the looping fire breath sound if it is playing. */
static inline void kpaFireSEOff(void) {
    MarioWork* mp = marioGetPtr();
    if (mp->kpaWork->fireSfxId != 0xFFFFFFFFU) {
        psndSFXOff(mp->kpaWork->fireSfxId);
        mp->kpaWork->fireSfxId = -1;
    }
}


/** Returns the active fire breath effect, or NULL. koopa_hit reads it to test the flame against objects. */
EffectEntry* getKoopaFireEfp(void) {
    return fireefp;
}

/** Returns the frames left in which enemies cannot hit Bowser (after taking damage). */
s32 kpaNoHitEnemyCheck(void) {
    return marioGetPtr()->kpaNoHitTimer;
}

/** TRUE while Bowser is invincible (muteki) from a star (KpaWork flag 0x10000). */
s32 kpaMutekiCheck(void) {
    return (marioGetPtr()->kpaWork->flags & KPA_WORK_FLAG_MUTEKI) != 0;
}

/** Makes Bowser invincible and turns on his highlight material. */
void kpaMutekiOn(void) {
    MarioWork* mp = marioGetPtr();
    animPoseSetMaterialFlagOn(mp->paperAnimGroupId[0], MAP_OBJ_FLAG_PULSE);
    mp->kpaWork->flags |= KPA_WORK_FLAG_MUTEKI;
}

/** Ends Bowser's invincibility and his highlight. */
void kpaMutekiOff(void) {
    MarioWork* mp = marioGetPtr();
    animPoseSetMaterialFlagOff(mp->paperAnimGroupId[0], MAP_OBJ_FLAG_PULSE);
    mp->kpaWork->flags &= ~KPA_WORK_FLAG_MUTEKI;
}

/**
 * Starts the 12-second star invincibility timer and clears the post-damage timer;
 * the side-scrolling update calls kpaMutekiOff when it runs out.
 */
void kpaMSetMutekiTimer(void) {
    MarioWork* mp = marioGetPtr();
    mp->kpaWork->mutekiTimer = sysMsec2Frame(12000);
    mp->kpaNoHitTimer = 0;
}

/** Sets Bowser's pose: a in the 3D maps, b in the side-scrolling stages (the KPA2_ poses). */
void kpaChgPose(const char* a, const char* b) {
    s32 v = (marioGetPtr()->stageFlags & KPA_STAGE_FLAG_SIDE_SCROLLING) ^ 1;
    marioChgPose(v == 1 ? a : b);
}

/** Bowser's power-up level (0 small, 1 normal, 2 big, 3 giant); 0 when the player is not Bowser. */
s32 kpaGetLevel(void) {
    s32 lv = 0;
    MarioWork* mp = marioGetPtr();
    if (mp->characterId == CHARACTER_BOWSER) {
        lv = mp->kpaWork->level;
    }
    return lv;
}

/**
 * Sets Bowser's power-up level and his matching scale (baseScaleTbl) and collision (kpaSetSpec).
 * Only while the player is Bowser.
 */
void kpaSetLevel(s32 lv) {
    MarioWork* mp = marioGetPtr();
    if (mp->characterId == CHARACTER_BOWSER && mp->kpaWork != NULL) {
        f32 s;
        mp->kpaWork->level = lv;
        s = baseScaleTbl[lv];
        mp->scale.x = mp->scale.y = s;
        mp->scale.z = 1.0f;
        kpaSetSpec();
    }
}

/** Bowser's width for power-up level lv (20, 60, 84, 186). */
f32 kpaGetWidth(s32 lv) {
    return kpaWidthDat[lv];
}

/**
 * Where Bowser is relative to the water in kpa_03 (surface at y 370): 0 above, 1 partly in, 2 fully under;
 * always 0 on other maps and -1 when the player is not Bowser. Underwater he moves and jumps slower.
 */
s32 kpaGetBodyStatus(void) {
    MarioWork* mp = marioGetPtr();
    s32 st;
    if (mp->characterId != CHARACTER_BOWSER) {
        return KPA_BODY_NOT_BOWSER;
    }
    st = KPA_BODY_DRY;
    if (strcmp(gp->currentMapName, "kpa_03") == 0) {
        if (mp->position.y > 370.0f) {
            return KPA_BODY_DRY;
        }
        if (mp->position.y + mp->wPlayerCollisionBox.y < 370.0f) {
            st = KPA_BODY_SUBMERGED;
        } else {
            st = KPA_BODY_PARTLY_SUBMERGED;
        }
    }
    return st;
}

/** 1 in kpa_03, the seaside stage with water (stageFlags bit 2), else 0. */
s32 kpaGetStageType(void) {
    return (marioGetPtr()->stageFlags >> 1) & 1;
}

#define JUMP_PARAM(m, i) ((&(m)->jumpVelocityY)[i]) // jumpVelocityY..jumpSnapY as f32[4]

static inline void kpaSetJumpParams(MarioWork* mp, f32 velocityY, f32 accelerationY, f32 jerkY, f32 snapY) {
    mp->jumpVelocityY = velocityY;
    mp->jumpAccelerationY = accelerationY;
    mp->jumpJerkY = jerkY;
    mp->jumpSnapY = snapY;
}

/** 1 in the 3D maps, 0 in the side-scrolling stages kpa_02 to kpa_06 (stageFlags bit 1). */
s32 kpaGetStageViewType(void) {
    return !(marioGetPtr()->stageFlags & KPA_STAGE_FLAG_SIDE_SCROLLING);
}

/**
 * In the side-scrolling stages, lets the camera follow Bowser vertically once he is above y 76;
 * returns 0 and does nothing in 3D maps or in background mode.
 */
s32 kpaCamFollow(void) {
    MarioWork* mp = marioGetPtr();
    s32 v;
    if (marioBgmodeChk() == 1 || (v = (marioGetPtr()->stageFlags & KPA_STAGE_FLAG_SIDE_SCROLLING) ^ 1, v == 1)) {
        return 0;
    }
    if (mp->position.y > 76.0f) {
        camFollowYOn();
    }
    return 1;
}

/**
 * Prepares the player for becoming Bowser (marioSetCharMode 2):
 * allocates and clears the Bowser work at level 1,
 * and sets the stage flags for the side-scrolling (kpa_02 to kpa_06) and seaside (kpa_03) stages.
 */
void kpaPreInit(void) {
    s32* sfxIdPtr;
    MarioWork* mp = marioGetPtr();
    if (mp->kpaWork == NULL) {
        mp->kpaWork = (KpaWork*)__memAlloc(HEAP_DEFAULT, sizeof(KpaWork));
    }
    memset(mp->kpaWork, 0, sizeof(KpaWork));
    mp->kpaWork->level = 1;
    mp->kpaWork->walkRate = 1;
    mp->kpaWork->fireCount = 0;
    mp->kpaWork->fireRate = 1;
    sfxIdPtr = &marioGetPtr()->kpaWork->fireSfxId; // FAKE: address temp; direct store differs
    *sfxIdPtr = -1;
    mp = marioGetPtr();
    if (strcmp(gp->currentMapName, "kpa_02") == 0 || strcmp(gp->currentMapName, "kpa_03") == 0 ||
        strcmp(gp->currentMapName, "kpa_04") == 0 || strcmp(gp->currentMapName, "kpa_05") == 0 ||
        strcmp(gp->currentMapName, "kpa_06") == 0) {
        mp->stageFlags |= KPA_STAGE_FLAG_SIDE_SCROLLING;
    } else {
        mp->stageFlags &= ~KPA_STAGE_FLAG_SIDE_SCROLLING;
    }
    mp = marioGetPtr();
    if (strcmp(gp->currentMapName, "kpa_03") == 0) {
        mp->stageFlags |= KPA_STAGE_FLAG_KPA03;
    } else {
        mp->stageFlags &= ~KPA_STAGE_FLAG_KPA03;
    }
    fireefp = NULL;
}

f32 __fabsf(f32);

/** Per-frame player update while playing as Bowser: the side-scrolling or the 3D version. */
void kpaMain(void) {
    if (marioGetPtr()->stageFlags & KPA_STAGE_FLAG_SIDE_SCROLLING) {
        kpa2DMain();
    } else {
        kpa3DMain();
    }
}

/**
 * Side-scrolling Bowser update: hit checks, item pickup, the fire attack, the star timer,
 * horizontal movement limited by his dash speed and walls, jumping and falling, floors and ceilings,
 * then the shared player effects, balloon and camera.
 */
void kpa2DMain(void) {
    MarioWork* mp = marioGetPtr();
    Vec diff;
    Vec saved;
    f32 spd;
    f32 sn;
    f32 cs;
    f32 t;
    kpaHitCheck();
    if (mp->kpaNoHitTimer) {
        mp->kpaNoHitTimer--;
    }
    mp->hitobjHead = NULL;
    mp->hitobjFront = NULL;
    mp->hitobjFrontA = NULL;
    mp->hitobjPush = NULL;
    mp->hitobjWall = NULL;
    marioItemGetChk();
    kpaFireAttack();
    kpaFireMain();
    {
        MarioWork* m2 = marioGetPtr();
        if (m2->kpaWork->mutekiTimer > 0) {
            if (--m2->kpaWork->mutekiTimer <= 0) {
                m2->kpaWork->mutekiTimer = 0;
                kpaMutekiOff();
            }
        }
    }
    if (mp->currMotionId == MARIO_MOTION_JUMP_3) {
        mp->basePos = mp->position;
        mp->floorMove.x = 0.0f;
        mp->floorMove.y = 0.0f;
        mp->floorMove.z = 0.0f;
        marioBottomless();
        marioPreDisp();
        mario_kemuri();
        marioCamMain();
        mp->frameCounter++;
        return;
    }
    saved = mp->position;
    spd = 0.0f;
    if (!(mp->flags & MARIO_FLAG_STOP_AIR_MOVEMENT) && mp->airTimer != 0) {
        spd = mp->controlStickSensitivity * mp->playerGravity;
        spd *= 0.015f;
        if (!(mp->flags & (MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING | MARIO_FLAG_IS_STEPPING))) {
            spd *= 0.25f;
        }
        sincosf(&sn, &cs, mp->controlStickAngle);
        spd *= sn;
    }
    sincosf(&sn, &cs, mp->directionView);
    t = mp->baseSpeed * sn + spd;
    spd = __fabsf(t);
    {
        f32 ang = angleABf(0.0f, 0.0f, t, 0.0f);
        f32 mx = mp->mBaseDashSpeed * mp->playerGravity;
        if (spd > mx) {
            spd = mx;
        }
        marioCheckWall(spd, ang);
    }
    mp->position.y += mp->floorMove.y;
    PSVECSubtract(&mp->position, &saved, &diff);
    {
        f32 mag = PSVECMag(&diff);
        if (mag > spd) {
            PSVECScale(&diff, &diff, spd / mag);
            PSVECAdd(&saved, &diff, &mp->position);
        }
    }
    if (mp->flags & MARIO_FLAG_IS_JUMPING) {
        marioChkOverhead();
        if (mp->airTimer > 5) {
            marioChkGnd2();
        }
    } else if (mp->flags & MARIO_FLAG_IS_FALLING) {
        kpaFall();
        marioChkOverhead();
        if (mp->currMotionId != MARIO_MOTION_HIP && mp->currMotionId != MARIO_MOTION_HIP_2 && mp->airTimer > 5) {
            marioChkGnd2();
        }
    } else {
        marioChkOverhead();
        marioChkGnd();
    }
    marioBoots();
    marioForceMoveMain();
    marioBottomless();
    marioPreDisp();
    marioBalloonMain();
    mario_kemuri();
    marioCamMain();
    mp->frameCounter++;
    mp->basePos = mp->position;
    mp->floorMove.x = 0.0f;
    mp->floorMove.y = 0.0f;
    mp->floorMove.z = 0.0f;
}

/**
 * 3D Bowser update (kpa_00, kpa_01 and other maps):
 * hit checks, item pickup, the fire attack, the star timer and the normal player movement,
 * floor and fall checks, then partner, effects and camera. Damage and plane motions skip the movement.
 */
void kpa3DMain(void) {
    MarioWork* mp = marioGetPtr();
    struct HitEntry* h = kpaHitCheck();
    if (h) {
        mp->hitobjHead = h;
    }
    if (mp->kpaNoHitTimer) {
        mp->kpaNoHitTimer--;
    }
    if (mp->currMotionId == MARIO_MOTION_PLANE || mp->currMotionId == MARIO_MOTION_DAMAGE || mp->currMotionId == MARIO_MOTION_DAMAGE_TOGE) {
        marioPreDisp();
        marioBottomless();
        mp->frameCounter++;
        partyMain();
        marioCamMain();
        mp->hitobjHead = NULL;
        mp->hitobjFront = NULL;
        mp->hitobjFrontA = NULL;
        mp->hitobjPush = NULL;
        mp->hitobjWall = NULL;
        return;
    }
    marioItemGetChk();
    kpaFireAttack();
    kpaFireMain();
    {
        MarioWork* m2 = marioGetPtr();
        if (m2->kpaWork->mutekiTimer > 0) {
            if (--m2->kpaWork->mutekiTimer <= 0) {
                m2->kpaWork->mutekiTimer = 0;
                kpaMutekiOff();
            }
        }
    }
    if (mp->currMotionId == MARIO_MOTION_JUMP_3) {
        mp->basePos = mp->position;
        mp->floorMove.x = 0.0f;
        mp->floorMove.y = 0.0f;
        mp->floorMove.z = 0.0f;
        marioBottomless();
        marioPreDisp();
        marioBalloonMain();
        mario_kemuri();
        partyMain();
        marioCamMain();
        mp->frameCounter++;
        return;
    }
    if (mp->flags & MARIO_FLAG_IS_STEPPING) {
        marioMove();
        marioChkOverhead();
        if (mp->airTimer > 100) {
            marioChgMot(MARIO_MOTION_FALL);
            marioSetFallPara();
        }
    } else if (mp->flags & MARIO_FLAG_IS_JUMPING) {
        marioMove();
        marioChkOverhead();
        if (mp->airTimer > 5) {
            marioChkGnd2();
        }
    } else if (mp->flags & MARIO_FLAG_IS_FALLING) {
        marioMove();
        kpaFall();
        kpaSearchFrontUnder();
        marioChkOverhead();
        if (mp->currMotionId != MARIO_MOTION_HIP && mp->currMotionId != MARIO_MOTION_HIP_2 && mp->airTimer > 5) {
            marioChkGnd2();
        }
    } else {
        marioMove();
        kpaSearchFrontUnder();
        marioChkOverhead();
        marioChkGnd();
    }
    marioBoots();
    marioForceMoveMain();
    marioBottomless();
    marioPreDisp();
    marioBalloonMain();
    mario_kemuri();
    partyMain();
    marioCamMain();
    mp->frameCounter++;
    mp->basePos = mp->position;
    mp->floorMove.x = 0.0f;
    mp->floorMove.y = 0.0f;
    mp->floorMove.z = 0.0f;
}

#define SETPV(m, v, a, b, c, d) \
    do {                        \
        JUMP_PARAM(m, v) = (a); \
        JUMP_PARAM(m, 1) = (b); \
        JUMP_PARAM(m, 2) = (c); \
        JUMP_PARAM(m, 3) = (d); \
    } while (v)
/**
 * Loads Bowser's jump physics (initial speed, acceleration, jerk, snap) for his level:
 * 7.0, 8.1, 8.4 or 10.5 units per frame upward. Returns the initial speed.
 */
f32 kpaSetJumpPara(void) {
    int slot;
    f32* jd = marioJumpData;
    MarioWork* mp = marioGetPtr();
    slot = 0; // FAKE: register-held 0 for SETPV/JUMP_PARAM index and the dead empty if below; literal 0 differs
    SETPV(mp, slot, jd[slot], jd[1], jd[2], jd[3]);
    switch (mp->kpaWork->level) {
    case 0:
        SETPV(mp, slot, 7.0f, -0.4f, 0.08f, -0.02f);
        break;
    case 1:
        SETPV(mp, slot, 8.1f, -0.3f, 0.08f, -0.02f);
        break;
    case 2:
        SETPV(mp, slot, 8.4f, -0.1f, 0.08f, -0.018f);
        break;
        if (!slot) {
        }
    case 3:
        SETPV(mp, slot, 10.5f, -0.12f, 0.08f, -0.018f);
        break;
    }
    return JUMP_PARAM(mp, slot);
}

/**
 * Sets Bowser's fall physics: a slower, floatier fall while A is held.
 * No return statement: the original falls off the end (callers ignore the result).
 * Declared void, the constants load into other float registers, so the f32 return type stays.
 */
f32 kpaSetFallPara(void) {
    MarioWork* mp = marioGetPtr();
    if (mp->buttonHold & PAD_BUTTON_A) {
        kpaSetJumpParams(mp, -0.05f, -0.08f, -0.006f, -0.002f);
    } else {
        kpaSetJumpParams(mp, -0.1f, -0.2f, -0.024f, -0.01f);
    }
}

/**
 * Player motion for being bounced off an NPC: a small fixed jump (2.6 up) that turns into a fall at the peak.
 * Mario also gets his jump poses. Run from mot_jump.
 */
void kpa_jumpNpc(void) {
    MarioWork* mp = marioGetPtr();
    if (mp->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) {
        mp->trigFlags &= ~MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
        marioResetCamFollowRate();
        mp->flags &= ~MARIO_FLAG_STOP_AIR_MOVEMENT;
        camFollowYOff();
        marioPreJump();
        kpaSetJumpParams(mp, 2.6f, -0.06f, -0.01f, -0.002f);
        if (!(mp->flags & MARIO_FLAG_IS_CARRYING_BOBBERY) && mp->characterId == CHARACTER_MARIO) {
            marioChgPose("M_J_1B");
        }
        mp->flags |= MARIO_FLAG_IS_JUMPING;
        mp->currSubMotionId = 0;
    }
    switch ((s32)mp->currSubMotionId) {
    case 0:
        marioMakeJumpPara();
        if (mp->jumpVelocityY <= 0.0f) {
            if (!(mp->flags & MARIO_FLAG_IS_CARRYING_BOBBERY) && mp->characterId == CHARACTER_MARIO) {
                marioChgPose("M_J_1C");
            }
            marioSetPeakYpos();
            mp->jumpAccelerationY = -0.06f;
            mp->jumpJerkY = -0.004f;
            mp->jumpSnapY = -0.002f;
            mp->flags &= ~MARIO_FLAG_IS_JUMPING;
            mp->flags |= MARIO_FLAG_IS_FALLING;
        }
        break;
    }
    mp->position.y += mp->jumpVelocityY;
    mp->airTimer++;
}

/**
 * Falling for Bowser: in kpa_03 a simple capped fall onto platforms with a landing sound;
 * elsewhere also slows his forward speed and floats while A is held,
 * landing on floors and ride objects (and passing through hits with attribute 0x200).
 */
void kpaFall(void) {
    MarioWork* mp = marioGetPtr();
    f32 fs;
    f32 ly;
    if (strcmp(gp->currentMapName, "kpa_03") == 0) {
        void* h;
        mp->hitobjStandOn = NULL;
        kpaClearHitobjRide();
        fs = marioGetFallSpd();
        if (fs < -20.0f) {
            fs = -20.0f;
        }
        if (!fs) {
            kpaSetFallPara();
            fs = marioGetFallSpd();
        }
        h = marioChkLandon(&ly, fs);
        if (h) {
            mp->hitobjStandOn = h;
            mp->hitobjJumpFrom = NULL;
            mp->position.y = ly;
        } else {
            mp->position.y += fs;
        }
        if (mp->hitobjStandOn) {
            marioLandOn();
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_KUPPA_LANDING1), &mp->position);
        }
    } else {
        void* h;
        if (mp->currMotionId == MARIO_MOTION_HIP || mp->currMotionId == MARIO_MOTION_HIP_2) {
            return;
        }
        if (!marioKeyOffChk() || !marioCtrlOffChk()) {
            if (!(mp->flags & MARIO_FLAG_FORCED_MOVEMENT)) {
                f32 bs = mp->baseSpeed;
                bs += 0.01f * (-0.5f - bs);
                if (bs < 0.0f) {
                    bs = 0.0f;
                }
                mp->baseSpeed = bs;
            }
        }
        if (mp->currMotionId != MARIO_MOTION_JUMP_NPC) {
            if (mp->buttonHold & PAD_BUTTON_A) {
                mp->jumpSnapY = -0.001f;
            } else {
                mp->jumpSnapY = -0.01f;
            }
        }
        mp->hitobjStandOn = NULL;
        kpaClearHitobjRide();
        fs = marioGetFallSpd();
        if (fs < -20.0f) {
            fs = -20.0f;
        }
        h = marioChkLandon(&ly, fs);
        if (!h) {
            mp->position.y += fs;
        } else {
            mp->hitobjStandOn = h;
            mp->hitobjJumpFrom = NULL;
            if (hitGetAttr(h) & HIT_ATTR_WATER_HAZARD) {
                mp->position.y += fs;
            } else {
                mp->position.y = ly;
            }
        }
        if (h || kpaChkHitobjRide()) {
            marioLandOn();
        }
    }
}

// FAKE: stand-in, never called (the linker strips it); it only puts kpaSetFallPara's eight literals into the
// .sdata2 pool; removing it changes the object
void kpaStrippedFallPool(f32* f) {
    f[0] = -0.05f;
    f[1] = -0.08f;
    f[2] = -0.006f;
    f[3] = -0.002f;
    f[4] = -0.1f;
    f[5] = -0.2f;
    f[6] = -0.024f;
    f[7] = -0.01f;
}

/**
 * TRUE while Bowser's fire breath is coming out (attack states 20-23 or 40-43) and the player is active.
 * Used for flame hits and to block other actions.
 */
s32 kpaFireAttackCheck(void) {
    s32 m;
    MarioWork* mp = marioGetPtr();
    if (!(mp->flags & MARIO_FLAG_ACTIVE)) {
        return 0;
    }
    m = mp->kpaWork->attackState;
    if ((m >= 20 && m <= 23) || (m >= 40 && m <= 43)) {
        return 1;
    }
    return 0;
}

/**
 * Starts the standing fire breath attack (state 1) if Bowser is not already attacking:
 * stops his movement and switches him to standing.
 */
void kpaAttackStart(void) {
    MarioWork* mp = marioGetPtr();
    if (mp->kpaWork->attackState == 0) {
        marioChgMot(MARIO_MOTION_STAY);
        mp->kpaWork->attackState = 1;
    }
}

/** Starts the fire breath attack without changing motion (state 30), used while moving or jumping. */
void kpaAttackStart2(void) {
    KpaWork* w = marioGetPtr()->kpaWork;
    if (w->attackState == 0) {
        w->attackState = 30;
    }
}

/**
 * Runs the fire breath attack: wind-up pose, then flames every frame for at least 400 ms and for as long as B
 * or fireHold is held, then the closing pose and back to the motion's normal pose.
 * States 1-24 are the standing attack, 30-44 the moving one; the looping sound plays while flames come out.
 */
void kpaFireAttack(void) {
    MarioWork* mp = marioGetPtr();
    switch (mp->kpaWork->attackState) {
    case 1:
        mp->flags &= ~(MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING | MARIO_FLAG_IS_STEPPING | MARIO_FLAG_HAS_INPUT_JUMP);
        mp->airTimer = 0;
        mp->kpaWork->fireRate = mp->kpaWork->level == 0 ? 12 : 8;
        mp->kpaWork->fireCount = 0;
        mp->kpaWork->attackTimer = 0;
        mp->kpaWork->attackState = 20;
        if (!(mp->flags & MARIO_FLAG_FORCED_MOVEMENT)) {
            mp->baseSpeed = 0.0f;
            mp->kpaSpeed = 0.0f;
        }
    case 20:
        kpaChgPose("KPA_A_1A", "KPA2_A_1A");
        mp->kpaWork->attackTimer = 6;
        mp->kpaWork->attackState = 21;
        break;
    case 21:
        if (--mp->kpaWork->attackTimer > 0) {
            break;
        }
        kpaChgPose("KPA_A_1B", "KPA2_A_1B");
        mp->kpaWork->attackTimer = 8;
        mp->kpaWork->attackState = 22;
        kpaFireSEOn();
        break;
    case 22:
        kpaFireEntry(mp->position);
        if (--mp->kpaWork->attackTimer > 0) {
            break;
        }
        mp->kpaWork->attackTimer = sysMsec2Frame(400);
        kpaChgPose("KPA_A_1C", "KPA2_A_1C");
        mp->kpaWork->attackState = 23;
        break;
    case 23:
        kpaFireEntry(mp->position);
        if (--mp->kpaWork->attackTimer > 0) {
            break;
        }
        if (mp->buttonHold & PAD_BUTTON_B) {
            break;
        }
        if (mp->kpaWork->fireHold != 0) {
            break;
        }
        kpaChgPose("KPA_A_1D", "KPA2_A_1D");
        mp->kpaWork->attackState = 24;
        mp->kpaWork->attackTimer = 6;
        kpaFireSEOff();
        break;
    case 24:
        if (--mp->kpaWork->attackTimer > 0) {
            break;
        }
        mp->kpaWork->attackState = 0;
        kpaChgPose("KPA_S_1", "KPA2_S_1");
        break;
    }
    switch (mp->kpaWork->attackState) {
    case 30:
        mp->kpaWork->fireRate = mp->kpaWork->level == 0 ? 12 : 8;
        mp->kpaWork->fireCount = 0;
        mp->kpaWork->attackTimer = 0;
        mp->kpaWork->attackState = 40;
        if (!(mp->flags & MARIO_FLAG_FORCED_MOVEMENT)) {
            mp->baseSpeed = 0.0f;
            mp->kpaSpeed = 0.0f;
        }
    case 40:
        kpaChgPose("KPA_A_1A", "KPA2_A_1A");
        mp->kpaWork->attackTimer = 6;
        mp->kpaWork->attackState = 41;
        break;
    case 41:
        if (--mp->kpaWork->attackTimer > 0) {
            break;
        }
        kpaChgPose("KPA_A_1B", "KPA2_A_1B");
        mp->kpaWork->attackTimer = 8;
        mp->kpaWork->attackState = 42;
        kpaFireSEOn();
        break;
    case 42:
        kpaFireEntry(mp->position);
        if (--mp->kpaWork->attackTimer > 0) {
            break;
        }
        mp->kpaWork->attackTimer = sysMsec2Frame(400);
        kpaChgPose("KPA_A_1C", "KPA2_A_1C");
        mp->kpaWork->attackState = 43;
        break;
    case 43:
        kpaFireEntry(mp->position);
        if (--mp->kpaWork->attackTimer > 0) {
            break;
        }
        if (mp->buttonHold & PAD_BUTTON_B) {
            break;
        }
        if (mp->kpaWork->fireHold != 0) {
            break;
        }
        kpaChgPose("KPA_A_1D", "KPA2_A_1D");
        mp->kpaWork->attackState = 44;
        mp->kpaWork->attackTimer = 6;
        kpaFireSEOff();
        break;
    case 44:
        if (--mp->kpaWork->attackTimer > 0) {
            break;
        }
        mp->kpaWork->attackState = 0;
        switch ((u16)mp->currMotionId) {
        case MARIO_MOTION_STAY:
            kpaChgPose("KPA_S_1", "KPA2_S_1");
            break;
        case MARIO_MOTION_WALK:
            kpaChgPose("KPA_W_1", "KPA2_W_1");
            break;
        case MARIO_MOTION_DASH:
            kpaChgPose("KPA_R_1", "KPA2_R_1");
            break;
        case MARIO_MOTION_KPA_SWIM:
            kpaChgPose("KPA_S_1", "KPA2_Z_2");
            break;
        }
        break;
    }
}

/** Ends any fire attack at once and stops its sound. */
void kpaCancelAttackMode(void) {
    KpaWork* w = marioGetPtr()->kpaWork;
    w->attackState = 0;
    kpaFireSEOff();
}

/** TRUE while any fire attack state is active. */
s32 kpaGetAttackMode(void) {
    return marioGetPtr()->kpaWork->attackState != 0;
}

/** Removes the fire breath effect and stops its sound. Called by the kpa map scripts. */
// Descriptive name (not in the symbol maps); placeholder unk_801f8cd0, JP 0x801F8CD0. See docs/NAME_PROVENANCE.md.
void kpaFireDelete(void) {
    if (fireefp) {
        MarioWork* mp;
        effSoftDelete(fireefp);
        fireefp = NULL;
        mp = marioGetPtr();
        if (mp->kpaWork->fireSfxId != 0xFFFFFFFFU) {
            psndSFXOff(mp->kpaWork->fireSfxId);
            mp->kpaWork->fireSfxId = -1;
        }
    }
}

// FAKE: stand-in, never called (the linker strips it); its int-to-float conversion only puts the conversion
// constant into the pool; removing it changes the object
void kpaStrippedPool(f32* f, s32 i) {
    *f = (f32)i;
}

void kpaFireMain(void) {
    MarioWork* mp = marioGetPtr();
    Mtx mb0;
    Mtx m80;
    Mtx m50;
    Mtx m20;
    Vec out;
    Vec zero;
    s32 lv;
    EffectEntry* fe;
    EffGonbabaBreathWork* w;
    s32 i;
    f32 ballScale;
    f32 f30;
    f32 headScale;
    mp->kpaWork->flameHit = NULL;
    if (fireefp) {
        f32 dir;
        f32 scale;
        f32 y;
        f32 z;
        s32 t;
        dir = (mp->displayDirection >= 90.0f && mp->displayDirection <= 270.0f) ? 1.0f : -1.0f;
        scale = kpaGetBaseScale(kpaGetLevel());
        f30 = 40.0f * dir * scale + mp->position.x;
        t = (marioGetPtr()->stageFlags & KPA_STAGE_FLAG_SIDE_SCROLLING) ^ 1;
        y = mp->position.y + (20.0f * scale + (t == 1 ? 6.0f : 0.0f));
        z = mp->position.z;
        ((EffGonbabaBreathWork*)fireefp->userdata)->position.x = f30;
        ((EffGonbabaBreathWork*)fireefp->userdata)->position.y = y;
        ((EffGonbabaBreathWork*)fireefp->userdata)->position.z = z;
        {
            EffectEntry* w2 = ((EffGonbabaBreathWork*)fireefp->userdata)->child;
            if (w2) {
                ((EffGonbabaBreathWork*)w2->userdata)->position.x = f30;
                ((EffGonbabaBreathWork*)((EffGonbabaBreathWork*)fireefp->userdata)->child->userdata)->position.y = y;
                ((EffGonbabaBreathWork*)((EffGonbabaBreathWork*)fireefp->userdata)->child->userdata)->position.z = z;
            }
        }
        lv = kpaGetLevel();
        if (lv != 0) {
            fe = fireefp;
            camGetPtr(CAMERA_3D);
            w = (EffGonbabaBreathWork*)fe->userdata;
            headScale = w->scale;
            PSMTXTrans(m80, w->position.x, w->position.y, w->position.z);
            PSMTXRotRad(m50, 'y', 0.017453292f * (w->yaw - camGetPtr(CAMERA_3D)->viewYaw));
            if (w->yaw > 90.0f && w->yaw < 270.0f) {
                PSMTXScale(mb0, w->scale, w->scale, -w->scale);
            } else {
                PSMTXScale(mb0, w->scale, w->scale, w->scale);
            }
            PSMTXConcat(m80, m50, m80);
            PSMTXConcat(m80, mb0, m20);
            w = w + 1;
            for (i = 1; i < fe->count; i++, w++) {
                if (((i - 1) & 3) == 0 && w->delay == 0 && w->life > 0x10) {
                    void* hit;
                    ballScale = 7.0f * w->scale;
                    f30 = ballScale * headScale;
                    if (!(f30 < 7.0f)) {
                        PSMTXTrans(m80, w->position.x, w->position.y, w->position.z);
                        PSMTXScale(mb0, ballScale, ballScale, ballScale);
                        PSMTXConcat(m80, mb0, m80);
                        PSMTXConcat(m20, m80, m80);
                        zero = *(Vec*)vec3_802fc920;
                        PSMTXMultVec(m80, &zero, &out);
                        hit = kpaFlameHitCheck(out.x, out.y, out.z, f30);
                        if (hit) {
                            mp->kpaWork->flameHit = hit;
                            break;
                        }
                    }
                }
            }
        }
    }
    if (kpaFireAttackCheck() == 0) {
        if (fireefp) {
            effSoftDelete(fireefp);
            fireefp = NULL;
        }
    }
}

/** Bowser's scale for power-up level lv (0.3, 1.0, 1.4, 3.1). */
f32 kpaGetBaseScale(s32 lv) {
    return baseScaleTbl[lv];
}

/** TRUE while the player is Bowser doing a ground pound (MARIO_MOTION_HIP_2). Used to break blocks (mobjdrv). */
s32 kpaHipAttackCheck(void) {
    s32 r = 0;
    MarioWork* mp = marioGetPtr();
    if (mp->characterId != CHARACTER_BOWSER) {
        return 0;
    }
    if (mp->currMotionId == MARIO_MOTION_HIP_2) {
        r = 1;
    }
    return r;
}

/** TRUE while Bowser is in the air (airTimer not zero). */
s32 kpaJumpChk(void) {
    return marioGetPtr()->airTimer != 0;
}

/**
 * Plays one of Bowser's 28 random voice clips (kpaPauseSeTable) when the pause menu is opened as Bowser and
 * when Start is pressed in it.
 */
void kpaPauseSE2(void) {
    psndSFXOn(SFX_NAME_WORD(kpaPauseSeTable[irand(28)] | PSND_SFX_BYPASS_MUTE));
}
