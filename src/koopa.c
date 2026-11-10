// Bowser (koopa) as the player in the Super Mario Bros. style side-scrolling interludes: work, level, camera,
// fire attack and jump parameters.

#include "version.h"
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

static void kpa2DMain(void);
static void kpa3DMain(void);
static void kpaFireAttack(void);
static void kpaFireMain(void);
static void kpaFall(void);
void fire_se_init(void);
const f32 vec3_802fc920[3] = {0.0f, 0.0f, 0.0f};
// FAKE: unsized f32 array view of mario_motion.c's MarioJumpData marioJumpData; reading marioJumpData.jump through
// the struct type changes the object.
extern f32 marioJumpData[];

/**
 * Bowser's size per power-up level (small, normal, big, giant): his width (kpaGetWidth) and model scale
 * (kpaSetLevel); then the 28 voice clips of which one plays at random when the pause menu opens as Bowser.
 */
static f32 kpaWidthDat[4] = {20.0f, 60.0f, 84.0f, 186.0f};
static f32 baseScaleTbl[4] = {0.3f, 1.0f, 1.4f, 3.1f};
static s32 kpaPauseSeTable[28] = {PSND_SFX_VOICE_KOOPA_ANGRY1_1, PSND_SFX_VOICE_KOOPA_ANGRY1_2, PSND_SFX_VOICE_KOOPA_ANGRY2_1, PSND_SFX_VOICE_KOOPA_ANGRY2_2, PSND_SFX_VOICE_KOOPA_HAPPY1_1, PSND_SFX_VOICE_KOOPA_HAPPY1_2, PSND_SFX_VOICE_KOOPA_HAPPY2_1, PSND_SFX_VOICE_KOOPA_HAPPY2_2, PSND_SFX_VOICE_KOOPA_LAUGH1_1, PSND_SFX_VOICE_KOOPA_LAUGH1_2,
                           PSND_SFX_VOICE_KOOPA_LAUGH2_1, PSND_SFX_VOICE_KOOPA_LAUGH2_2, PSND_SFX_VOICE_KOOPA_QUESTION1, PSND_SFX_VOICE_KOOPA_SATIATED1_2, PSND_SFX_VOICE_KOOPA_SATIATED2_1, PSND_SFX_VOICE_KOOPA_SATIATED2_2, PSND_SFX_VOICE_KOOPA_SURPRISED1_1, PSND_SFX_VOICE_KOOPA_SURPRISED1_2, PSND_SFX_VOICE_KOOPA_SURPRISED2_1, PSND_SFX_VOICE_KOOPA_SURPRISED2_2,
                           PSND_SFX_VOICE_KOOPA_THINK1_1, PSND_SFX_VOICE_KOOPA_THINK1_2, PSND_SFX_VOICE_KOOPA_THINK2_1, PSND_SFX_VOICE_KOOPA_THINK2_2, PSND_SFX_VOICE_KOOPA_VEXATIOUS1_1, PSND_SFX_VOICE_KOOPA_VEXATIOUS1_2, PSND_SFX_VOICE_KOOPA_VEXATIOUS2_1, PSND_SFX_VOICE_KOOPA_VEXATIOUS2_2};
static EffectEntry* fireefp;
// Unused 4-byte object, read by nothing; the original had it too (its storage is in the retail .sbss). Name invented.
static u8* fireefp_pad;

/**
 * Spawns Bowser's fire breath (the Hooktail breath effect) in front of his mouth if none is active:
 * offset by his size and facing, flame size by level (level 3 uses the big flame type),
 * both flame layers turned to his facing.
 */
static inline void kpaFireEntry2(Vec pos) {
    MarioWork* mario = marioGetPtr();
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
        dir = (mario->displayDirection >= 90.0f && mario->displayDirection <= 270.0f) ? 1.0f : -1.0f;
        scale = kpaGetBaseScale(kpaGetLevel());
        x = 40.0f * dir * scale + pos.x;
        t = (marioGetPtr()->stageFlags & KPA_STAGE_FLAG_SIDE_SCROLLING) ^ 1;
        y = pos.y + (20.0f * scale + (t == 1 ? 6.0f : 0.0f));
        z = pos.z;
        type = 7;
        sz = 0.4f;
        switch (mario->kpaWork->level) {
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
        if (mario->displayDirectionTarget >= 90.0f && mario->displayDirectionTarget <= 270.0f) {
            flag = 1;
        }
        ((EffGonbabaBreathWork*)fireefp->userdata)->yaw = (f32)(flag ? 180 : 0);
        child = ((EffGonbabaBreathWork*)fireefp->userdata)->child;
        if (child) {
            flag = 0;
            if (mario->displayDirectionTarget >= 90.0f && mario->displayDirectionTarget <= 270.0f) {
                flag = 1;
            }
            ang = flag ? 180 : 0;
            ((EffGonbabaBreathWork*)child->userdata)->yaw = (f32)ang;
        }
    }
}

/** Spawns the fire breath at pos (see kpaFireEntry2). */
static void kpaFireEntry(Vec pos) {
    // the result of this marioGetPtr() call is never used (the call is in the retail code)
    MarioWork* mp = marioGetPtr();
    kpaFireEntry2(pos);
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
    MarioWork* mario = marioGetPtr();
    animPoseSetMaterialFlagOn(mario->paperAnimGroupId[0], MAP_OBJ_FLAG_PULSE);
    mario->kpaWork->flags |= KPA_WORK_FLAG_MUTEKI;
}

/** Ends Bowser's invincibility and his highlight. */
void kpaMutekiOff(void) {
    MarioWork* mario = marioGetPtr();
    animPoseSetMaterialFlagOff(mario->paperAnimGroupId[0], MAP_OBJ_FLAG_PULSE);
    mario->kpaWork->flags &= ~KPA_WORK_FLAG_MUTEKI;
}

/**
 * Starts the 12-second star invincibility timer and clears the post-damage timer;
 * the player update (kpa2DMain or kpa3DMain) calls kpaMutekiOff when it runs out.
 */
void kpaMSetMutekiTimer(void) {
    MarioWork* mario = marioGetPtr();
    mario->kpaWork->mutekiTimer = sysMsec2Frame(12000);
    mario->kpaNoHitTimer = 0;
}

/** Sets Bowser's pose: a in the 3D maps, b in the side-scrolling stages (the KPA2_ poses). */
void kpaChgPose(const char* a, const char* b) {
    s32 v = (marioGetPtr()->stageFlags & KPA_STAGE_FLAG_SIDE_SCROLLING) ^ 1;
    marioChgPose(v == 1 ? a : b);
}

/** Bowser's power-up level (0 small, 1 normal, 2 big, 3 giant); 0 when the player is not Bowser. */
s32 kpaGetLevel(void) {
    s32 lv = 0;
    MarioWork* mario = marioGetPtr();
    if (mario->characterId == CHARACTER_BOWSER) {
        lv = mario->kpaWork->level;
    }
    return lv;
}

/**
 * Sets Bowser's power-up level and his matching scale (baseScaleTbl) and collision (kpaSetSpec).
 * Only while the player is Bowser.
 */
void kpaSetLevel(s32 lv) {
    MarioWork* mario = marioGetPtr();
    if (mario->characterId == CHARACTER_BOWSER && mario->kpaWork != NULL) {
        f32 s;
        mario->kpaWork->level = lv;
        s = baseScaleTbl[lv];
        mario->scale.x = mario->scale.y = s;
        mario->scale.z = 1.0f;
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
    MarioWork* mario = marioGetPtr();
    s32 st;
    if (mario->characterId != CHARACTER_BOWSER) {
        return KPA_BODY_NOT_BOWSER;
    }
    st = KPA_BODY_DRY;
    if (strcmp(gp->currentMapName, "kpa_03") == 0) {
        if (mario->position.y > 370.0f) {
            return KPA_BODY_DRY;
        }
        if (mario->position.y + mario->wPlayerCollisionBox.y < 370.0f) {
            st = KPA_BODY_SUBMERGED;
        } else {
            st = KPA_BODY_PARTLY_SUBMERGED;
        }
    }
    return st;
}

/** 1 in kpa_03, the seaside stage with water (KPA_STAGE_FLAG_KPA03, stageFlags 0x2), else 0. */
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

/** 1 in the 3D maps, 0 in the side-scrolling stages kpa_02 to kpa_06 (KPA_STAGE_FLAG_SIDE_SCROLLING, stageFlags 0x1). */
s32 kpaGetStageViewType(void) {
    return !(marioGetPtr()->stageFlags & KPA_STAGE_FLAG_SIDE_SCROLLING);
}

/**
 * In the side-scrolling stages, lets the camera follow Bowser vertically once he is above y 76;
 * returns 0 and does nothing in 3D maps or in background mode.
 */
s32 kpaCamFollow(void) {
    MarioWork* mario = marioGetPtr();
    s32 v;

    // FAKE: comma expression re-testing the stage flag; !(stageFlags & KPA_STAGE_FLAG_SIDE_SCROLLING) changes the object
    if (marioBgmodeChk() == 1 || (v = (marioGetPtr()->stageFlags & KPA_STAGE_FLAG_SIDE_SCROLLING) ^ 1, v == 1)) {
        return 0;
    }
    if (mario->position.y > 76.0f) {
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
    MarioWork* mario = marioGetPtr();
    if (mario->kpaWork == NULL) {
        mario->kpaWork = (KpaWork*)__memAlloc(HEAP_DEFAULT, sizeof(KpaWork));
    }
    memset(mario->kpaWork, 0, sizeof(KpaWork));
    mario->kpaWork->level = 1;
    mario->kpaWork->walkRate = 1;
    mario->kpaWork->fireCount = 0;
    mario->kpaWork->fireRate = 1;
    fire_se_init();
    mario = marioGetPtr();
    if (strcmp(gp->currentMapName, "kpa_02") == 0 || strcmp(gp->currentMapName, "kpa_03") == 0 ||
        strcmp(gp->currentMapName, "kpa_04") == 0 || strcmp(gp->currentMapName, "kpa_05") == 0 ||
        strcmp(gp->currentMapName, "kpa_06") == 0) {
        mario->stageFlags |= KPA_STAGE_FLAG_SIDE_SCROLLING;
    } else {
        mario->stageFlags &= ~KPA_STAGE_FLAG_SIDE_SCROLLING;
    }
    mario = marioGetPtr();
    if (strcmp(gp->currentMapName, "kpa_03") == 0) {
        mario->stageFlags |= KPA_STAGE_FLAG_KPA03;
    } else {
        mario->stageFlags &= ~KPA_STAGE_FLAG_KPA03;
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
static void kpa2DMain(void) {
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
#if VERSION >= VERSION_JP
    mp->hitobjHead = NULL;
    mp->hitobjFront = NULL;
    mp->hitobjFrontA = NULL;
    mp->hitobjPush = NULL;
    mp->hitobjWall = NULL;
#endif
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
        f32 mx = mp->baseDashSpeed * mp->playerGravity;
        if (spd > mx) {
            spd = mx;
        }
        marioCheckWall(spd, ang);
    }
#if VERSION >= VERSION_JP
    mp->position.y += mp->floorMove.y;
#endif
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
static void kpa3DMain(void) {
    MarioWork* mp = marioGetPtr();
    struct HitEntry* h = kpaHitCheck();
    MarioWork* m2;
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
    m2 = marioGetPtr();
    if (m2->kpaWork->mutekiTimer > 0) {
        if (--m2->kpaWork->mutekiTimer <= 0) {
            m2->kpaWork->mutekiTimer = 0;
            kpaMutekiOff();
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
    MarioWork* mario = marioGetPtr();
    slot = 0; // FAKE: register-held 0 for SETPV/JUMP_PARAM index and the dead empty if below; literal 0 differs
    SETPV(mario, slot, jd[slot], jd[1], jd[2], jd[3]);
    switch (mario->kpaWork->level) {
        case 0:
            SETPV(mario, slot, 7.0f, -0.4f, 0.08f, -0.02f);
            break;
        case 1:
            SETPV(mario, slot, 8.1f, -0.3f, 0.08f, -0.02f);
            break;
        case 2:
            SETPV(mario, slot, 8.4f, -0.1f, 0.08f, -0.018f);
            break;
            if (!slot) {
            }
        case 3:
            SETPV(mario, slot, 10.5f, -0.12f, 0.08f, -0.018f);
            break;
    }
    return JUMP_PARAM(mario, slot);
}

/**
 * Sets Bowser's fall physics: a slower, floatier fall while A is held.
 * No return statement: the original falls off the end (callers ignore the result).
 * Declared void, the constants load into other float registers, so the f32 return type stays.
 */
f32 kpaSetFallPara(void) {
    MarioWork* mario = marioGetPtr();
    if (mario->buttonHold & PAD_BUTTON_A) {
        kpaSetJumpParams(mario, -0.05f, -0.08f, -0.006f, -0.002f);
    } else {
        kpaSetJumpParams(mario, -0.1f, -0.2f, -0.024f, -0.01f);
    }
}

/**
 * Player motion for being bounced off an NPC: a small fixed jump (2.6 up) that turns into a fall at the peak.
 * The M_J_ poses apply only to Mario, but mot_jumpNpc (mot_jump) calls this only for Bowser.
 */
void kpa_jumpNpc(void) {
    MarioWork* mario = marioGetPtr();
    if (mario->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) {
        mario->trigFlags &= ~MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
        marioResetCamFollowRate();
        mario->flags &= ~MARIO_FLAG_STOP_AIR_MOVEMENT;
        camFollowYOff();
        marioPreJump();
        kpaSetJumpParams(mario, 2.6f, -0.06f, -0.01f, -0.002f);
        if (!(mario->flags & MARIO_FLAG_IS_CARRYING_BOBBERY) && mario->characterId == CHARACTER_MARIO) {
            marioChgPose("M_J_1B");
        }
        mario->flags |= MARIO_FLAG_IS_JUMPING;
        mario->currSubMotionId = 0;
    }
    switch ((s32)mario->currSubMotionId) {
        case 0:
            marioMakeJumpPara();
            if (mario->jumpVelocityY <= 0.0f) {
                if (!(mario->flags & MARIO_FLAG_IS_CARRYING_BOBBERY) && mario->characterId == CHARACTER_MARIO) {
                    marioChgPose("M_J_1C");
                }
                marioSetPeakYpos();
                mario->jumpAccelerationY = -0.06f;
                mario->jumpJerkY = -0.004f;
                mario->jumpSnapY = -0.002f;
                mario->flags &= ~MARIO_FLAG_IS_JUMPING;
                mario->flags |= MARIO_FLAG_IS_FALLING;
            }
            break;
    }
    mario->position.y += mario->jumpVelocityY;
    mario->airTimer++;
}

/**
 * Falling for Bowser: in kpa_03 a simple capped fall onto platforms with a landing sound;
 * elsewhere also slows his forward speed and floats while A is held,
 * landing on floors and ride objects (and passing through hits with attribute 0x200).
 */
static void kpaFall(void) {
    MarioWork* mario = marioGetPtr();
    f32 fs;
    f32 ly;
    if (strcmp(gp->currentMapName, "kpa_03") == 0) {
        void* h;
        mario->hitobjStandOn = NULL;
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
            mario->hitobjStandOn = h;
            mario->hitobjJumpFrom = NULL;
            mario->position.y = ly;
        } else {
            mario->position.y += fs;
        }
        if (mario->hitobjStandOn) {
            marioLandOn();
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_KUPPA_LANDING1), &mario->position);
        }
    } else {
        void* h;
        if (mario->currMotionId == MARIO_MOTION_HIP || mario->currMotionId == MARIO_MOTION_HIP_2) {
            return;
        }
        if (!marioKeyOffChk() || !marioCtrlOffChk()) {
            if (!(mario->flags & MARIO_FLAG_FORCED_MOVEMENT)) {
                f32 bs = mario->baseSpeed;
                bs += 0.01f * (-0.5f - bs);
                if (bs < 0.0f) {
                    bs = 0.0f;
                }
                mario->baseSpeed = bs;
            }
        }
        if (mario->currMotionId != MARIO_MOTION_JUMP_NPC) {
            if (mario->buttonHold & PAD_BUTTON_A) {
                mario->jumpSnapY = -0.001f;
            } else {
                mario->jumpSnapY = -0.01f;
            }
        }
        mario->hitobjStandOn = NULL;
        kpaClearHitobjRide();
        fs = marioGetFallSpd();
        if (fs < -20.0f) {
            fs = -20.0f;
        }
        h = marioChkLandon(&ly, fs);
        if (!h) {
            mario->position.y += fs;
        } else {
            mario->hitobjStandOn = h;
            mario->hitobjJumpFrom = NULL;
            if (hitGetAttr(h) & HIT_ATTR_WATER_HAZARD) {
                mario->position.y += fs;
            } else {
                mario->position.y = ly;
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

/** Starts the looping fire breath sound plus a roar at Bowser's position, unless the loop is already playing. */
// Original name from the JP demo's link map (UNUSED there): every call is inlined, so the linker strips it.
void fire_se_on(void) {
    MarioWork* mario = marioGetPtr();
    if (mario->kpaWork->fireSfxId == 0xFFFFFFFFU) {
        mario->kpaWork->fireSfxId = psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_KUPPA_FIRE_BREATH1), &mario->position);
        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_VOICE_KOOPA_FIRE1), &mario->position);
    }
}

/** Stops the looping fire breath sound if it is playing. */
// Original name from the JP demo's link map (UNUSED there): every call is inlined, so the linker strips it.
void fire_se_off(void) {
    MarioWork* mp = marioGetPtr();
    if (mp->kpaWork->fireSfxId != 0xFFFFFFFFU) {
        psndSFXOff(mp->kpaWork->fireSfxId);
        mp->kpaWork->fireSfxId = -1;
    }
}

/** Marks the fire breath loop as not playing (fireSfxId -1). */
// Original name from the JP demo's link map (UNUSED there): every call is inlined, so the linker strips it.
// It keeps `inline`: kpaPreInit calls it before this definition, and as a plain function it stays out of line there.
inline void fire_se_init(void) {
    MarioWork* mario = marioGetPtr();
    mario->kpaWork->fireSfxId = -1;
}

/**
 * TRUE while Bowser's fire breath is coming out (attack states 20-23 or 40-43) and the player is active.
 * Used for flame hits and to block other actions.
 */
s32 kpaFireAttackCheck(void) {
    s32 m;
    MarioWork* mario = marioGetPtr();
    if (!(mario->flags & MARIO_FLAG_ACTIVE)) {
        return 0;
    }
    m = mario->kpaWork->attackState;
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
    if (mp->kpaWork->attackState == KPA_ATTACK_NONE) {
        marioChgMot(MARIO_MOTION_STAY);
        mp->kpaWork->attackState = KPA_ATTACK_START;
    }
}

/** Starts the fire breath attack without changing motion (state 30), used while swimming (kpa_swim). */
void kpaAttackStart2(void) {
    KpaWork* w = marioGetPtr()->kpaWork;
    if (w->attackState == KPA_ATTACK_NONE) {
        w->attackState = KPA_ATTACK_SCRIPTED_START;
    }
}

/**
 * Runs the fire breath attack: wind-up pose, then flames every frame for at least 400 ms and for as long as B
 * or fireHold is held, then the closing pose and back to the motion's normal pose.
 * States 1-24 are the standing attack, 30-44 the swimming one (its end picks the pose of the current motion);
 * the looping sound plays while flames come out.
 */
static void kpaFireAttack(void) {
    MarioWork* mario = marioGetPtr();
    switch (mario->kpaWork->attackState) {
        case KPA_ATTACK_START:
            mario->flags &= ~(MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING | MARIO_FLAG_IS_STEPPING | MARIO_FLAG_HAS_INPUT_JUMP);
            mario->airTimer = 0;
            mario->kpaWork->fireRate = mario->kpaWork->level == 0 ? 12 : 8;
            mario->kpaWork->fireCount = 0;
            mario->kpaWork->attackTimer = 0;
            mario->kpaWork->attackState = KPA_ATTACK_WINDUP_POSE;
            if (!(mario->flags & MARIO_FLAG_FORCED_MOVEMENT)) {
                mario->baseSpeed = 0.0f;
                mario->kpaSpeed = 0.0f;
            }
        case KPA_ATTACK_WINDUP_POSE:
            kpaChgPose("KPA_A_1A", "KPA2_A_1A");
            mario->kpaWork->attackTimer = 6;
            mario->kpaWork->attackState = KPA_ATTACK_WINDUP;
            break;
        case KPA_ATTACK_WINDUP:
            if (--mario->kpaWork->attackTimer > 0) {
                break;
            }
            kpaChgPose("KPA_A_1B", "KPA2_A_1B");
            mario->kpaWork->attackTimer = 8;
            mario->kpaWork->attackState = KPA_ATTACK_FIRE_START;
            fire_se_on();
            break;
        case KPA_ATTACK_FIRE_START:
            kpaFireEntry(mario->position);
            if (--mario->kpaWork->attackTimer > 0) {
                break;
            }
            mario->kpaWork->attackTimer = sysMsec2Frame(400);
            kpaChgPose("KPA_A_1C", "KPA2_A_1C");
            mario->kpaWork->attackState = KPA_ATTACK_FIRE_HOLD;
            break;
        case KPA_ATTACK_FIRE_HOLD:
            kpaFireEntry(mario->position);
            if (--mario->kpaWork->attackTimer > 0) {
                break;
            }
            if (mario->buttonHold & PAD_BUTTON_B) {
                break;
            }
            if (mario->kpaWork->fireHold != 0) {
                break;
            }
            kpaChgPose("KPA_A_1D", "KPA2_A_1D");
            mario->kpaWork->attackState = KPA_ATTACK_FINISH;
            mario->kpaWork->attackTimer = 6;
            fire_se_off();
            break;
        case KPA_ATTACK_FINISH:
            if (--mario->kpaWork->attackTimer > 0) {
                break;
            }
            mario->kpaWork->attackState = KPA_ATTACK_NONE;
            kpaChgPose("KPA_S_1", "KPA2_S_1");
            break;
    }
    switch (mario->kpaWork->attackState) {
        case KPA_ATTACK_SCRIPTED_START:
            mario->kpaWork->fireRate = mario->kpaWork->level == 0 ? 12 : 8;
            mario->kpaWork->fireCount = 0;
            mario->kpaWork->attackTimer = 0;
            mario->kpaWork->attackState = KPA_ATTACK_SCRIPTED_WINDUP_POSE;
            if (!(mario->flags & MARIO_FLAG_FORCED_MOVEMENT)) {
                mario->baseSpeed = 0.0f;
                mario->kpaSpeed = 0.0f;
            }
        case KPA_ATTACK_SCRIPTED_WINDUP_POSE:
            kpaChgPose("KPA_A_1A", "KPA2_A_1A");
            mario->kpaWork->attackTimer = 6;
            mario->kpaWork->attackState = KPA_ATTACK_SCRIPTED_WINDUP;
            break;
        case KPA_ATTACK_SCRIPTED_WINDUP:
            if (--mario->kpaWork->attackTimer > 0) {
                break;
            }
            kpaChgPose("KPA_A_1B", "KPA2_A_1B");
            mario->kpaWork->attackTimer = 8;
            mario->kpaWork->attackState = KPA_ATTACK_SCRIPTED_FIRE_START;
            fire_se_on();
            break;
        case KPA_ATTACK_SCRIPTED_FIRE_START:
            kpaFireEntry(mario->position);
            if (--mario->kpaWork->attackTimer > 0) {
                break;
            }
            mario->kpaWork->attackTimer = sysMsec2Frame(400);
            kpaChgPose("KPA_A_1C", "KPA2_A_1C");
            mario->kpaWork->attackState = KPA_ATTACK_SCRIPTED_FIRE_HOLD;
            break;
        case KPA_ATTACK_SCRIPTED_FIRE_HOLD:
            kpaFireEntry(mario->position);
            if (--mario->kpaWork->attackTimer > 0) {
                break;
            }
            if (mario->buttonHold & PAD_BUTTON_B) {
                break;
            }
            if (mario->kpaWork->fireHold != 0) {
                break;
            }
            kpaChgPose("KPA_A_1D", "KPA2_A_1D");
            mario->kpaWork->attackState = KPA_ATTACK_SCRIPTED_FINISH;
            mario->kpaWork->attackTimer = 6;
            fire_se_off();
            break;
        case KPA_ATTACK_SCRIPTED_FINISH:
            if (--mario->kpaWork->attackTimer > 0) {
                break;
            }
            mario->kpaWork->attackState = KPA_ATTACK_NONE;
            switch ((u16)mario->currMotionId) {
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
    w->attackState = KPA_ATTACK_NONE;
    fire_se_off();
}

/** TRUE while any fire attack state is active. */
s32 kpaGetAttackMode(void) {
    return marioGetPtr()->kpaWork->attackState != KPA_ATTACK_NONE;
}

/** Removes the fire breath effect and stops its sound. Called by the kpa map scripts. */
// Descriptive name (not in the symbol maps); placeholder unk_801f8cd0, JP 0x801F8CD0. See docs/NAME_PROVENANCE.md.
void kpaFireDelete(void) {
    if (fireefp) {
        effSoftDelete(fireefp);
        fireefp = NULL;
        fire_se_off();
    }
}

// FAKE: stand-in, never called (the linker strips it); its int-to-float conversion only puts the conversion
// constant into the pool; removing it changes the object
void kpaStrippedPool(f32* f, s32 i) {
    *f = (f32)i;
}

/**
 * Keeps the fire breath effect at Bowser's mouth
 * and tests every fourth live flame puff against objects (kpaFlameHitCheck), storing the first hit in flameHit.
 * JP and later skip the test at level 0 and for puffs smaller than 7 units; the early demos test them all.
 * Deletes the effect when the attack ends.
 */
static void kpaFireMain(void) {
    MarioWork* mario = marioGetPtr();
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
    mario->kpaWork->flameHit = NULL;
    if (fireefp) {
        f32 dir;
        f32 scale;
        f32 y;
        f32 z;
        s32 t;
        dir = (mario->displayDirection >= 90.0f && mario->displayDirection <= 270.0f) ? 1.0f : -1.0f;
        scale = kpaGetBaseScale(kpaGetLevel());
        f30 = 40.0f * dir * scale + mario->position.x;
        t = (marioGetPtr()->stageFlags & KPA_STAGE_FLAG_SIDE_SCROLLING) ^ 1;
        y = mario->position.y + (20.0f * scale + (t == 1 ? 6.0f : 0.0f));
        z = mario->position.z;
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
#if VERSION <= VERSION_DEMO_JP
        {
            f32 sc;
            f32 base;
#else
        lv = kpaGetLevel();
        if (lv != 0) {
#endif
            fe = fireefp;
            camGetPtr(CAMERA_3D);
            w = (EffGonbabaBreathWork*)fe->userdata;
#if VERSION <= VERSION_DEMO_JP
            base = w->scale;
#else
            headScale = w->scale;
#endif
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
#if VERSION <= VERSION_DEMO_JP
                    sc = 7.0f * w->scale;
                    PSMTXTrans(m80, w->position.x, w->position.y, w->position.z);
                    PSMTXScale(mb0, sc, sc, sc);
                    PSMTXConcat(m80, mb0, m80);
                    PSMTXConcat(m20, m80, m80);
                    zero = *(Vec*)vec3_802fc920;
                    PSMTXMultVec(m80, &zero, &out);
                    hit = kpaFlameHitCheck(out.x, out.y, out.z, sc * base);
                    if (hit) {
                        mario->kpaWork->flameHit = hit;
                        break;
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
#else
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
                            mario->kpaWork->flameHit = hit;
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
#endif

/** Bowser's scale for power-up level lv (0.3, 1.0, 1.4, 3.1). */
f32 kpaGetBaseScale(s32 lv) {
    return baseScaleTbl[lv];
}

/** TRUE while the player is Bowser doing a ground pound (MARIO_MOTION_HIP_2). Used to break blocks (mobjdrv). */
s32 kpaHipAttackCheck(void) {
    s32 r = 0;
    MarioWork* mario = marioGetPtr();
    if (mario->characterId != CHARACTER_BOWSER) {
        return 0;
    }
    if (mario->currMotionId == MARIO_MOTION_HIP_2) {
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
