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

#define KPW(m) ((m)->kpaWork)

f32 kpaWidthDat[4] = {20.0f, 60.0f, 84.0f, 186.0f};
f32 baseScaleTbl[4] = {0.3f, 1.0f, 1.4f, 3.1f};
s32 kpaPauseSeTable[28] = {0x10C, 0x10D, 0x10E, 0x10F, 0x111, 0x112, 0x113, 0x114, 0x116, 0x117,
                           0x118, 0x119, 0x11B, 0x11E, 0x11F, 0x120, 0x122, 0x123, 0x124, 0x125,
                           0x127, 0x128, 0x129, 0x12A, 0x12C, 0x12D, 0x12E, 0x12F};
static EffectEntry* fireefp;
static u8* fireefp_pad;

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
        t = (marioGetPtr()->stageFlags & 1) ^ 1;
        y = pos.y + (20.0f * scale + (t == 1 ? 6.0f : 0.0f));
        z = pos.z;
        type = 7;
        sz = 0.4f;
        switch (KPW(mp)->level) {
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

static inline void kpaFireEntry(Vec pos) {
    MarioWork* mp = marioGetPtr();
    kpaFireEntry2(pos);
}

static inline void kpaFireSEOn(void) {
    MarioWork* mp = marioGetPtr();
    if (KPW(mp)->fireSfxId == 0xFFFFFFFFU) {
        KPW(mp)->fireSfxId = psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_KUPPA_FIRE_BREATH1), &mp->position);
        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_VOICE_KOOPA_FIRE1), &mp->position);
    }
}

static inline void kpaFireSEOff(void) {
    MarioWork* mp = marioGetPtr();
    if (KPW(mp)->fireSfxId != 0xFFFFFFFFU) {
        psndSFXOff(KPW(mp)->fireSfxId);
        KPW(mp)->fireSfxId = -1;
    }
}


EffectEntry* getKoopaFireEfp(void) {
    return fireefp;
}

s32 kpaNoHitEnemyCheck(void) {
    return marioGetPtr()->kpaNoHitTimer;
}

s32 kpaMutekiCheck(void) {
    return (KPW(marioGetPtr())->flags >> 16) & 1;
}

void kpaMutekiOn(void) {
    MarioWork* mp = marioGetPtr();
    animPoseSetMaterialFlagOn(mp->paperAnimGroupId[0], MAP_OBJ_FLAG_PULSE);
    KPW(mp)->flags |= KPA_WORK_FLAG_MUTEKI;
}

void kpaMutekiOff(void) {
    MarioWork* mp = marioGetPtr();
    animPoseSetMaterialFlagOff(mp->paperAnimGroupId[0], MAP_OBJ_FLAG_PULSE);
    KPW(mp)->flags &= ~KPA_WORK_FLAG_MUTEKI;
}

void kpaMSetMutekiTimer(void) {
    MarioWork* mp = marioGetPtr();
    KPW(mp)->mutekiTimer = sysMsec2Frame(12000);
    mp->kpaNoHitTimer = 0;
}

void kpaChgPose(const char* a, const char* b) {
    s32 v = (marioGetPtr()->stageFlags & 1) ^ 1;
    marioChgPose(v == 1 ? a : b);
}

s32 kpaGetLevel(void) {
    s32 lv = 0;
    MarioWork* mp = marioGetPtr();
    if (mp->characterId == CHARACTER_BOWSER) {
        lv = KPW(mp)->level;
    }
    return lv;
}

void kpaSetLevel(s32 lv) {
    MarioWork* mp = marioGetPtr();
    if (mp->characterId == CHARACTER_BOWSER && KPW(mp) != NULL) {
        f32 s;
        KPW(mp)->level = lv;
        s = baseScaleTbl[lv];
        mp->scale.x = mp->scale.y = s;
        mp->scale.z = 1.0f;
        kpaSetSpec();
    }
}

f32 kpaGetWidth(s32 lv) {
    return kpaWidthDat[lv];
}

s32 kpaGetBodyStatus(void) {
    MarioWork* mp = marioGetPtr();
    s32 st;
    if (mp->characterId != CHARACTER_BOWSER) {
        return -1;
    }
    st = 0;
    if (strcmp(gp->currentMapName, "kpa_03") == 0) {
        if (mp->position.y > 370.0f) {
            return 0;
        }
        if (mp->position.y + mp->wPlayerCollisionBox.y < 370.0f) {
            st = 2;
        } else {
            st = 1;
        }
    }
    return st;
}

s32 kpaGetStageType(void) {
    return (marioGetPtr()->stageFlags >> 1) & 1;
}

#define JP(m, i) ((&(m)->jumpVelocityY)[i]) // jumpVelocityY..jumpSnapY as f32[4]
#define SETP(m, a, b, c, d)     do { JP(m, 0) = (a); JP(m, 1) = (b); JP(m, 2) = (c); JP(m, 3) = (d); } while (0)

s32 kpaGetStageViewType(void) {
    return !(marioGetPtr()->stageFlags & 1);
}

s32 kpaCamFollow(void) {
    MarioWork* mp = marioGetPtr();
    s32 v;
    if (marioBgmodeChk() == 1 || (v = (marioGetPtr()->stageFlags & 1) ^ 1, (s32)v == 1)) {
        return 0;
    }
    if (mp->position.y > 76.0f) {
        camFollowYOn();
    }
    return 1;
}

void kpaPreInit(void) {
    s32* sfxIdPtr;
    MarioWork* mp = marioGetPtr();
    if (KPW(mp) == NULL) {
        KPW(mp) = (KpaWork*)__memAlloc(HEAP_DEFAULT, sizeof(KpaWork));
    }
    memset(KPW(mp), 0, sizeof(KpaWork));
    KPW(mp)->level = 1;
    KPW(mp)->walkRate = 1;
    KPW(mp)->fireCount = 0;
    KPW(mp)->fireRate = 1;
    sfxIdPtr = &KPW(marioGetPtr())->fireSfxId; // FAKE: address temp; direct store differs
    *sfxIdPtr = -1;
    mp = marioGetPtr();
    if (strcmp(gp->currentMapName, "kpa_02") == 0 || strcmp(gp->currentMapName, "kpa_03") == 0 ||
        strcmp(gp->currentMapName, "kpa_04") == 0 || strcmp(gp->currentMapName, "kpa_05") == 0 ||
        strcmp(gp->currentMapName, "kpa_06") == 0) {
        mp->stageFlags |= 1;
    } else {
        mp->stageFlags &= ~1;
    }
    mp = marioGetPtr();
    if (strcmp(gp->currentMapName, "kpa_03") == 0) {
        mp->stageFlags |= 2;
    } else {
        mp->stageFlags &= ~2;
    }
    fireefp = 0;
}

f32 __fabsf(f32);

void kpaMain(void) {
    if (marioGetPtr()->stageFlags & 1) {
        kpa2DMain();
    } else {
        kpa3DMain();
    }
}

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
        if (KPW(m2)->mutekiTimer > 0) {
            if (--KPW(m2)->mutekiTimer <= 0) {
                KPW(m2)->mutekiTimer = 0;
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
        if (KPW(m2)->mutekiTimer > 0) {
            if (--KPW(m2)->mutekiTimer <= 0) {
                KPW(m2)->mutekiTimer = 0;
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

#define SETPV(m, v, a, b, c, d) do { JP(m, v) = (a); JP(m, 1) = (b); JP(m, 2) = (c); JP(m, 3) = (d); } while (v)
f32 kpaSetJumpPara(void) {
    int slot;
    f32* jd = marioJumpData;
    MarioWork* mp = marioGetPtr();
    slot = 0; // FAKE: register-held 0 for SETPV/JP index and the dead empty if below; literal 0 differs
    SETPV(mp, slot, jd[slot], jd[1], jd[2], jd[3]);
    switch (KPW(mp)->level) {
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
    return JP(mp, slot);
}

f32 kpaSetFallPara(void) {
    MarioWork* mp = marioGetPtr();
    if ((mp->buttonHold & PAD_BUTTON_A)) {
        SETP(mp, -0.05f, -0.08f, -0.006f, -0.002f);
    } else {
        SETP(mp, -0.1f, -0.2f, -0.024f, -0.01f);
    }
}

void kpa_jumpNpc(void) {
    MarioWork* mp = marioGetPtr();
    if (mp->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) {
        mp->trigFlags &= ~MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
        marioResetCamFollowRate();
        mp->flags &= ~MARIO_FLAG_STOP_AIR_MOVEMENT;
        camFollowYOff();
        marioPreJump();
        SETP(mp, 2.6f, -0.06f, -0.01f, -0.002f);
        if (!(mp->flags & MARIO_FLAG_IS_CARRYING_BOBBERY) && mp->characterId == CHARACTER_MARIO) {
            marioChgPose("M_J_1B");
        }
        mp->flags |= MARIO_FLAG_IS_JUMPING;
        mp->currSubMotionId = 0;
    }
    switch ((s32)mp->currSubMotionId) { case 0: {
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
    } break; }
    mp->position.y += mp->jumpVelocityY;
    mp->airTimer++;
}

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
        {
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
}

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

s32 kpaFireAttackCheck(void) {
    s32 m;
    MarioWork* mp = marioGetPtr();
    if (!(mp->flags & MARIO_FLAG_ACTIVE)) {
        return 0;
    }
    m = KPW(mp)->attackState;
    if ((m >= 20 && m <= 23) || (m >= 40 && m <= 43)) {
        return 1;
    }
    return 0;
}

void kpaAttackStart(void) {
    MarioWork* mp = marioGetPtr();
    if (KPW(mp)->attackState == 0) {
        marioChgMot(MARIO_MOTION_STAY);
        KPW(mp)->attackState = 1;
    }
}

void kpaAttackStart2(void) {
    KpaWork* w = KPW(marioGetPtr());
    if (w->attackState == 0) {
        w->attackState = 30;
    }
}

void kpaFireAttack(void) {
    MarioWork* mp = marioGetPtr();
    switch (KPW(mp)->attackState) {
    case 1:
        mp->flags &= ~(MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING | MARIO_FLAG_IS_STEPPING | MARIO_FLAG_HAS_INPUT_JUMP);
        mp->airTimer = 0;
        KPW(mp)->fireRate = KPW(mp)->level == 0 ? 12 : 8;
        KPW(mp)->fireCount = 0;
        KPW(mp)->attackTimer = 0;
        KPW(mp)->attackState = 20;
        if (!(mp->flags & MARIO_FLAG_FORCED_MOVEMENT)) {
            mp->baseSpeed = 0.0f;
            mp->kpaSpeed = 0.0f;
        }
    case 20:
        kpaChgPose("KPA_A_1A", "KPA2_A_1A");
        KPW(mp)->attackTimer = 6;
        KPW(mp)->attackState = 21;
        break;
    case 21:
        if (--KPW(mp)->attackTimer > 0) {
            break;
        }
        kpaChgPose("KPA_A_1B", "KPA2_A_1B");
        KPW(mp)->attackTimer = 8;
        KPW(mp)->attackState = 22;
        kpaFireSEOn();
        break;
    case 22:
        kpaFireEntry(mp->position);
        if (--KPW(mp)->attackTimer > 0) {
            break;
        }
        KPW(mp)->attackTimer = sysMsec2Frame(400);
        kpaChgPose("KPA_A_1C", "KPA2_A_1C");
        KPW(mp)->attackState = 23;
        break;
    case 23:
        kpaFireEntry(mp->position);
        if (--KPW(mp)->attackTimer > 0) {
            break;
        }
        if (mp->buttonHold & PAD_BUTTON_B) {
            break;
        }
        if (KPW(mp)->fireHold != 0) {
            break;
        }
        kpaChgPose("KPA_A_1D", "KPA2_A_1D");
        KPW(mp)->attackState = 24;
        KPW(mp)->attackTimer = 6;
        kpaFireSEOff();
        break;
    case 24:
        if (--KPW(mp)->attackTimer > 0) {
            break;
        }
        KPW(mp)->attackState = 0;
        kpaChgPose("KPA_S_1", "KPA2_S_1");
        break;
    }
    switch (KPW(mp)->attackState) {
    case 30:
        KPW(mp)->fireRate = KPW(mp)->level == 0 ? 12 : 8;
        KPW(mp)->fireCount = 0;
        KPW(mp)->attackTimer = 0;
        KPW(mp)->attackState = 40;
        if (!(mp->flags & MARIO_FLAG_FORCED_MOVEMENT)) {
            mp->baseSpeed = 0.0f;
            mp->kpaSpeed = 0.0f;
        }
    case 40:
        kpaChgPose("KPA_A_1A", "KPA2_A_1A");
        KPW(mp)->attackTimer = 6;
        KPW(mp)->attackState = 41;
        break;
    case 41:
        if (--KPW(mp)->attackTimer > 0) {
            break;
        }
        kpaChgPose("KPA_A_1B", "KPA2_A_1B");
        KPW(mp)->attackTimer = 8;
        KPW(mp)->attackState = 42;
        kpaFireSEOn();
        break;
    case 42:
        kpaFireEntry(mp->position);
        if (--KPW(mp)->attackTimer > 0) {
            break;
        }
        KPW(mp)->attackTimer = sysMsec2Frame(400);
        kpaChgPose("KPA_A_1C", "KPA2_A_1C");
        KPW(mp)->attackState = 43;
        break;
    case 43:
        kpaFireEntry(mp->position);
        if (--KPW(mp)->attackTimer > 0) {
            break;
        }
        if (mp->buttonHold & PAD_BUTTON_B) {
            break;
        }
        if (KPW(mp)->fireHold != 0) {
            break;
        }
        kpaChgPose("KPA_A_1D", "KPA2_A_1D");
        KPW(mp)->attackState = 44;
        KPW(mp)->attackTimer = 6;
        kpaFireSEOff();
        break;
    case 44:
        if (--KPW(mp)->attackTimer > 0) {
            break;
        }
        KPW(mp)->attackState = 0;
        switch ((u16)mp->currMotionId) {
        case 0:
            kpaChgPose("KPA_S_1", "KPA2_S_1");
            break;
        case 1:
            kpaChgPose("KPA_W_1", "KPA2_W_1");
            break;
        case 2:
            kpaChgPose("KPA_R_1", "KPA2_R_1");
            break;
        case 0x25:
            kpaChgPose("KPA_S_1", "KPA2_Z_2");
            break;
        }
        break;
    }
}

void kpaCancelAttackMode(void) {
    MarioWork* mp;
    KpaWork* w = KPW(marioGetPtr());
    w->attackState = 0;
    mp = marioGetPtr();
    if (KPW(mp)->fireSfxId != 0xFFFFFFFFU) {
        psndSFXOff(KPW(mp)->fireSfxId);
        KPW(mp)->fireSfxId = -1;
    }
}

s32 kpaGetAttackMode(void) {
    return KPW(marioGetPtr())->attackState != 0;
}

void kpaFireDelete(void) {
    if (fireefp) {
        MarioWork* mp;
        effSoftDelete((void*)fireefp);
        fireefp = 0;
        mp = marioGetPtr();
        if (KPW(mp)->fireSfxId != 0xFFFFFFFFU) {
            psndSFXOff(KPW(mp)->fireSfxId);
            KPW(mp)->fireSfxId = -1;
        }
    }
}

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
    s32 flag;
    s32 lv;
    EffectEntry* fe;
    EffGonbabaBreathWork* w;
    s32 i;
    f32 f31;
    f32 f30;
    f32 f29;
    KPW(mp)->flameHit = NULL;
    flag = 0;
    if (fireefp && (f32)flag == 0.0f) {
        f32 dir;
        f32 scale;
        f32 y;
        f32 z;
        s32 t;
        dir = (mp->displayDirection >= 90.0f && mp->displayDirection <= 270.0f) ? 1.0f : -1.0f;
        scale = kpaGetBaseScale(kpaGetLevel());
        f30 = 40.0f * dir * scale + mp->position.x;
        t = (marioGetPtr()->stageFlags & 1) ^ 1;
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
            f29 = w->scale;
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
                    f31 = 7.0f * w->scale;
                    f30 = f31 * f29;
                    if (!(f30 < 7.0f)) {
                        PSMTXTrans(m80, w->position.x, w->position.y, w->position.z);
                        PSMTXScale(mb0, f31, f31, f31);
                        PSMTXConcat(m80, mb0, m80);
                        PSMTXConcat(m20, m80, m80);
                        zero = *(Vec*)vec3_802fc920;
                        PSMTXMultVec(m80, &zero, &out);
                        hit = kpaFlameHitCheck(out.x, out.y, out.z, f30);
                        if (hit) {
                            KPW(mp)->flameHit = hit;
                            break;
                        }
                    }
                }
            }
        }
    }
    {
        if (kpaFireAttackCheck() == 0) {
            if (fireefp) {
                effSoftDelete((void*)fireefp);
                fireefp = 0;
            }
        }
    }
}

f32 kpaGetBaseScale(s32 lv) {
    return baseScaleTbl[lv];
}

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

s32 kpaJumpChk(void) {
    return ((MarioWork*)marioGetPtr())->airTimer != 0;
}

void kpaPauseSE2(void) {
    psndSFXOn(SFX_NAME_WORD(kpaPauseSeTable[irand(28)] | PSND_SFX_BYPASS_MUTE));
}
