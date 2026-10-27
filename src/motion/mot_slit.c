#include "dolphin/types.h"
#include "driver/hitdrv.h"
#include "mario/mario.h"
#include "mario/mario_motion.h"
#include "mot/mot_slit.h"
#include "pmario_sound.h"
#include "system.h"
#include <string.h>
#include <dolphin/pad.h>
#include "mario/mario_pouch.h"
#include "driver/camdrv.h"
#include "sfx_name_word.h"
#include "mario/mario_sbr.h"
#include "mario/mario_hit.h"
#include "party/party_motion.h"
#include "driver/mapdrv.h"


double fabs(double);
BOOL marioCreviceWallChk(const Vec* p);

int __abs(int);



void marioSlitReenter(void) {
    MarioWork* mario = marioGetPtr();
    marioChgMot2(MARIO_MOTION_SLIT);
    mario->slitState = 10;
    mario->flags &= ~(MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING | MARIO_FLAG_IS_STEPPING | MARIO_FLAG_HAS_INPUT_JUMP);
}

void N_marioSlitSetAfterLand(void) {
    MarioWork* mario = marioGetPtr();
    marioChgMot2(MARIO_MOTION_SLIT);
    mario->flags &= ~(MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING | MARIO_FLAG_IS_STEPPING | MARIO_FLAG_HAS_INPUT_JUMP);
}

BOOL marioSlitAbilityChk(void) {
    MarioWork* mario = marioGetPtr();
    if (mario->flags & MARIO_FLAG_SLIT_ABILITY_DISABLED) {
        return FALSE;
    }
    if (!pouchCheckItem(ITEM_PAPER_CURSE)) {
        return FALSE;
    }
    if (mario->flags & MARIO_FLAG_IS_8_BIT) {
        return FALSE;
    }
    return marioBgmodeChk() != 1;
}

s32 marioSlitButton(void) {
    MarioWork* mario = marioGetPtr();
    if (mario->trigFlags & MARIO_TRIG_FLAG_USE_PARTNER) {
        return 0;
    }
    if (marioKeyOffChk()) {
        return 0;
    }
    if (marioBgmodeChk() == 1) {
        return 0;
    }
    if (!(mario->dispFlags & MARIO_DISP_FLAG_SLIT)) {
        if (mario->buttonHold & PAD_TRIGGER_R) {
            return 1;
        }
    } else {
        if (mario->buttonHold & PAD_TRIGGER_R) {
            return 2;
        }
        if (mario->triggerR >= 50) {
            return 2;
        }
    }
    return 0;
}

BOOL marioSlitKeyDisable(void) {
    MarioWork* mario = marioGetPtr();
    s16 st;
    if (!(mario->flags & MARIO_FLAG_PAPER_MODE)) {
        return TRUE;
    }
    st = mario->slitState;
    if (st >= 0x50 && st <= 0x52) {
        return FALSE;
    }
    if (st >= 0x5a && st <= 0x5e) {
        return FALSE;
    }
    return TRUE;
}

BOOL U_marioSlitContinueChk(void) {
    MarioWork* mario = marioGetPtr();
    Vec pos;
    if (marioSlitButton()) {
        return TRUE;
    }
    if (marioSlitChkWallAround()) {
        return TRUE;
    }
    pos = mario->position;
    return marioCreviceWallChk(&pos) != 0;
}

BOOL marioCreviceWallChk(const Vec* p) {
    f32 offX;
    f32 offZ;
    f32 angA;
    f32 angB;
    HitEntry* hitA;
    HitEntry* hitB;
    f32 s;
    f32 c;
    f32 dist;
    f32 h = 10.0f;
    Vec pos;
    Vec dir;
    Vec outPos;
    Vec outNrm;

    marioGetPtr();
    sincosf(&s, &c, toMovedirSimple(270.0f));
    offX = 0.5f * (20.0f * s);
    offZ = 0.5f * (20.0f * c);
    angA = 0.0f;
    angB = angA;
    sincosf(&s, &c, toMovedirSimple(180.0f));
    dist = 20.0f;
    pos = *p;
    pos.y += 10.0f;
    {
        Vec t1 = {0, 0, 0};
        t1.x = s;
        t1.z = c;
        dir = t1;
    }
    hitA = marioHitCheckVec(&pos, &dir, &outPos, &outNrm, &dist);
    if (hitA) {
        angA = angleABf(0.0f, 0.0f, outNrm.x, outNrm.z);
    }
    if (!hitA) {
        Vec t2 = {0, 0, 0};
        dist = 20.0f;
        t2.x = p->x + offX;
        t2.y = p->y + h;
        t2.z = p->z + offZ;
        pos = t2;
        hitA = marioHitCheckVec(&pos, &dir, &outPos, &outNrm, &dist);
        if (hitA) {
            angA = angleABf(0.0f, 0.0f, outNrm.x, outNrm.z);
        }
    }
    if (!hitA) {
        Vec t3 = {0, 0, 0};
        dist = 20.0f;
        t3.x = p->x - offX;
        t3.y = p->y + h;
        t3.z = p->z - offZ;
        pos = t3;
        hitA = marioHitCheckVec(&pos, &dir, &outPos, &outNrm, &dist);
        if (hitA) {
            angA = angleABf(0.0f, 0.0f, outNrm.x, outNrm.z);
        }
    }
    sincosf(&s, &c, toMovedirSimple(0.0f));
    dist = 20.0f;
    pos = *p;
    pos.y += 10.0f;
    {
        Vec u1 = {0, 0, 0};
        u1.x = s;
        u1.z = c;
        dir = u1;
    }
    hitB = marioHitCheckVec(&pos, &dir, &outPos, &outNrm, &dist);
    if (hitB) {
        angB = angleABf(0.0f, 0.0f, outNrm.x, outNrm.z);
    }
    if (!hitB) {
        Vec u2 = {0, 0, 0};
        dist = 20.0f;
        u2.x = p->x + offX;
        u2.y = p->y + h;
        u2.z = p->z + offZ;
        pos = u2;
        hitB = marioHitCheckVec(&pos, &dir, &outPos, &outNrm, &dist);
        if (hitB) {
            angB = angleABf(0.0f, 0.0f, outNrm.x, outNrm.z);
        }
    }
    if (!hitB) {
        Vec u3 = {0, 0, 0};
        dist = 20.0f;
        u3.x = p->x - offX;
        u3.y = p->y + h;
        u3.z = p->z - offZ;
        pos = u3;
        hitB = marioHitCheckVec(&pos, &dir, &outPos, &outNrm, &dist);
        if (hitB) {
            angB = angleABf(0.0f, 0.0f, outNrm.x, outNrm.z);
        }
    }
    if (hitA && hitB && __fabsf(180.0f - 0.5f * (angA + angB)) <= 10.0f) {
        return TRUE;
    }
    return FALSE;
}

void marioClearSlitFloor(void) {
    MarioWork* mario = marioGetPtr();
    mario->slitPosSet = 0;
}

BOOL marioGetSlitFloorPos(Vec* out) {
    MarioWork* mario = marioGetPtr();
    if (mario->slitPosSet) {
        *out = mario->slitPos;
        return TRUE;
    }
    return FALSE;
}

BOOL marioChkSlitThrouh(void) {
    MarioWork* mario = marioGetPtr();
    s16 st;
    if (mario->currMotionId != MARIO_MOTION_SLIT) {
        return FALSE;
    }
    st = mario->slitState;
    if (st >= 0x50 && st <= 0x5e) {
        return TRUE;
    }
    return FALSE;
}

BOOL marioChkSlitThrouhEnd(void) {
    MarioWork* mario = marioGetPtr();
    s16 st;
    if (mario->currMotionId != MARIO_MOTION_SLIT) {
        return FALSE;
    }
    st = mario->slitState;
    if (st == 0x52 || st == 0x5e) {
        return TRUE;
    }
    return FALSE;
}

void motSlitContinue(void) {
    s16 v;
    v = 0x14;
    marioChgMot2(MARIO_MOTION_SLIT);
    marioGetPtr()->slitState = v;
}

BOOL marioChkSlitEnd(void) {
    MarioWork* mario = marioGetPtr();
    if (mario->currMotionId != MARIO_MOTION_SLIT) {
        return FALSE;
    }
    return mario->slitState >= 0x64;
}

void motSlitCancel(void) {
    MarioWork* mario = marioGetPtr();
    if (mario->flags & MARIO_FLAG_PAPER_MODE) {
        mario->slitState = 0x64;
    }
}

void motSlitCancel2(void) {
    MarioWork* mario = marioGetPtr();
    if (mario->flags & MARIO_FLAG_PAPER_MODE) {
        allPartyForceSlitOff();
        mario->slitState = 0x6e;
    }
}

void motSlitCancel3(void) {
    MarioWork* mario = marioGetPtr();
    if (mario->flags & MARIO_FLAG_PAPER_MODE) {
        mario->slitState = 0x78;
    }
}

BOOL marioGetMotSlitCancel3(void) {
    s16 st;
    BOOL r = FALSE;
    MarioWork* mario = marioGetPtr();
    if (mario->currMotionId == MARIO_MOTION_SLIT) {
        st = mario->slitState;
        if ((st >= 0x78 && st <= 0x7a) || (st >= 0x6e && st <= 0x70)) {
            r = TRUE;
        }
    }
    return r;
}

#define CLR_DISP 0xFEFFF3FF
static inline void slitClearFlags(MarioWork* m) {
    m->flags &= ~MARIO_FLAG_PARTY_WALK_DISABLED;
    m->flags &= ~MARIO_FLAG_GONE_THROUGH_BARS;
    m->dispFlags &= CLR_DISP;
    m->dispFlags &= CLR_DISP;
}

static inline void slitSetDir(MarioWork* m, f32 a) {
    if (revise360(m->directionView - m->targetYaw) <= 180.0f) {
        m->displayDirection = 260.0f;
    } else {
        m->displayDirection = 280.0f;
    }
}

void marioSlitForceCancel(void) {
    MarioWork* m = marioGetPtr();
    if (m->flags & MARIO_FLAG_PAPER_MODE) {
        m->flags &= ~MARIO_FLAG_PAPER_MODE;
        if (m->directionView >= 180.0f) {
            m->displayDirection = 0.0f;
            m->displayDirectionTarget = m->displayDirection;
        } else {
            m->displayDirection = 180.0f;
            m->displayDirectionTarget = m->displayDirection;
        }
        m->dispFlags &= CLR_DISP;
        m->flags &= ~MARIO_FLAG_PARTY_WALK_DISABLED;
        m->flags &= ~MARIO_FLAG_GONE_THROUGH_BARS;
        m->wPlayerCollisionBox.x = 10.0f;
        m->dispFlags &= CLR_DISP;
        marioPaperOff();
        mapResetPaperAmbColor();
        m->dispFlags &= ~MARIO_DISP_FLAG_NO_REAR_POSE;
        marioAdjustMoveDir();
        {
            MarioWork* p = marioGetPtr();
            p->camShiftTargetZ = 0.0f;
            p->camShiftRate.z = 0.004f;
        }
        allPartyForceSlitOff();
    }
}

void mot_slit_post(void) {
    MarioWork* m = marioGetPtr();
#ifdef VERSION_G8ME01
    u16 id;
    if (m->slitState >= 10) {
        id = m->currMotionId;
        if (id == 0x15 || (u16)(id - 0xb) <= 1 || m->currMotionId <= 7 || m->currMotionId == MARIO_MOTION_JUMP_3) {
            return;
        }
#else
    u16 id = m->currMotionId;
    if (id == 0x15 || (u16)(id - 0xb) <= 1 || m->currMotionId <= 7 || m->currMotionId == MARIO_MOTION_JUMP_3) {
        return;
#endif
    }
    if (!(m->flags & MARIO_FLAG_PAPER_MODE)) {
        if (m->dispFlags & MARIO_DISP_FLAG_SLIT) {
            m->wPlayerCollisionBox.x = 20.0f;
            if (m->directionView >= 180.0f) {
                m->displayDirection = 0.0f;
                m->displayDirectionTarget = m->displayDirection;
            } else {
                m->displayDirection = 180.0f;
                m->displayDirectionTarget = m->displayDirection;
            }
            slitClearFlags(m);
            marioPaperOff();
            mapResetPaperAmbColor();
            m->dispFlags &= ~MARIO_DISP_FLAG_NO_REAR_POSE;
            marioAdjustMoveDir();
            allPartyForceSlitOff();
        }
    } else if (m->currMotionId != MARIO_MOTION_FALL) {
        m->flags &= ~MARIO_FLAG_PAPER_MODE;
        m->wPlayerCollisionBox.x = 20.0f;
        if (m->directionView >= 180.0f) {
            m->displayDirection = 0.0f;
            m->displayDirectionTarget = m->displayDirection;
        } else {
            m->displayDirection = 180.0f;
            m->displayDirectionTarget = m->displayDirection;
        }
        m->flags &= ~MARIO_FLAG_PARTY_WALK_DISABLED;
        m->flags &= ~MARIO_FLAG_GONE_THROUGH_BARS;
        m->dispFlags &= ~(MARIO_DISP_FLAG_LEFT_ONLY | MARIO_DISP_FLAG_RIGHT_ONLY | MARIO_DISP_FLAG_SLIT);
        m->dispFlags &= CLR_DISP;
        marioPaperOff();
        mapResetPaperAmbColor();
        m->dispFlags &= ~MARIO_DISP_FLAG_NO_REAR_POSE;
        marioAdjustMoveDir();
#ifdef VERSION_G8ME01
        if (m->currMotionId != MARIO_MOTION_ROLL) {
            allPartyForceSlitOff();
        } else {
            U_allPartySlitOffReq();
        }
#else
        allPartyForceSlitOff();
#endif
    }
}

void marioForceSlitAnime(void) {
    MarioWork* m = marioGetPtr();
    marioPaperOff();
    marioChgPaper(NULL);
    marioPaperOn("p_slit");
    marioChgPose("M_S_1");
    marioChgPaper("PM_S_1A");
    m->dispFlags |= MARIO_DISP_FLAG_NO_REAR_POSE;
}

static inline void slitInit(void) {
    MarioWork* m = marioGetPtr();
    m->mot.slit.reserved_0 = 0.0f;
    m->mot.slit.counter = 0;
    m->mot.slit.count = 0;
    m->mot.slit.timer = 0;
    m->mot.slit.side = 0;
    m->trigFlags &= ~MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
    m->flags &= ~(MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING | MARIO_FLAG_IS_STEPPING | MARIO_FLAG_HAS_INPUT_JUMP);
    m->dispFlags |= MARIO_DISP_FLAG_NO_REAR_POSE;
    m->dispFlags |= MARIO_DISP_FLAG_SLIT;
    if (m->paperPoseId == -1) {
        GXColor tmp;
        GXColor col = {0, 0, 0, 255};
        col.r = 0xa8;
        col.g = 0xa8;
        col.b = 0xa8;
        tmp = col;
        mapSetPaperAmbColor(&tmp);
        marioPaperOn("p_slit");
    }
    if (!m->controlStickSensitivity) {
        marioChgPose("M_S_1");
    }
    marioChgPaper("PM_S_1A");
}

void marioReInit_slit(void) {
    MarioWork* m = marioGetPtr();
    slitInit();
    m->trigFlags &= ~MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
    m->currMotionId = MARIO_MOTION_SLIT;
    m->slitState = 10;
    m->displayDirection = 270.0f;
    m->displayDirectionTarget = m->displayDirection;
    marioChgPose("M_S_1");
    marioChgPaper("PM_S_1B");
    m->slitState = 10;
    m->mot.slit.count = 0;
    m->wPlayerCollisionBox.x = 6.0f;
    m->flags |= MARIO_FLAG_PAPER_MODE;
    m->dispFlags |= MARIO_DISP_FLAG_SLIT;
    allPartySlitOn();
}

static inline BOOL slitContinueChk(void) {
    MarioWork* mario = marioGetPtr();
    Vec pos;
    if (marioSlitButton()) {
        return TRUE;
    }
    if (marioSlitChkWallAround()) {
        return TRUE;
    }
    pos = mario->position;
    if (marioCreviceWallChk(&pos)) {
        return TRUE;
    }
    return FALSE;
}

#ifdef VERSION_G8ME01
#define SPEEDCHK_COND(m) ((m)->baseSpeed >= (m)->mBaseDashSpeed || (m)->controlStickSensitivity)
#else
#define SPEEDCHK_COND(m) ((m)->baseSpeed >= (m)->mBaseDashSpeed)
#endif

// Back from dashing to walking speed.
static inline void slitSpeedDown(MarioWork* m) {
    m->baseSpeed = m->mBaseWalkSpeed;
    if (!(m->trigFlags & 0x8000)) {
        marioChgPose("M_R_1");
    }
    m->mot.slit.flags |= 1;
}

static inline f32 slitWalkSpeed(void) {
    MarioWork* p = marioGetPtr();
    f32 spd = p->mBaseWalkSpeed;
    if (p->flags & MARIO_FLAG_PAPER_MODE) {
        if (p->mot.slit.timer != 0) return spd; {
            int a = p->wStickDir1;
            int b = p->wStickDir2;
            if (a * a + b * b <= 0xbd1) {
                spd *= 0.5f;
            }

        }
    } else if (p->flags & MARIO_FLAG_BG_MODE) {
        spd *= 0.5f;
    }
    return spd;
}

static inline void slitSetDisplayDir(MarioWork* m) {
    m->displayDirection = (revise360(m->directionView - m->targetYaw) <= 180.0f) ? 260.0f : 280.0f;
}

static inline void slitSetSide(MarioWork* m) {
    if (revise360(m->directionView - m->targetYaw) <= 180.0f) {
        m->mot.slit.side = 1;
        m->displayDirection = 260.0f;
    } else {
        m->mot.slit.side = 2;
        m->displayDirection = 280.0f;
    }
}

static inline void slitCamFix(void) {
    MarioWork* p = marioGetPtr();
    p->camShiftTargetZ = 0.0f;
    p->camShiftRate.z = 0.004f;
}

#ifdef VERSION_G8ME01
#define SLITSTOP(m)               \
    if (!marioChkKey()) {         \
        (m)->baseSpeed = 0.0f;    \
        m->slitState = 10;        \
        m->mot.slit.count = 0;    \
        marioChgPose("M_S_1");    \
        marioChgPaper("PM_S_1B"); \
        break;                    \
    }
#define SLITSTOP2(m)              \
    if (!marioChkKey()) {         \
        m->mot.slit.timer = 0;    \
        (m)->baseSpeed = 0.0f;    \
        m->slitState = 10;        \
        m->mot.slit.count = 0;    \
        marioChgPose("M_S_1");    \
        marioChgPaper("PM_S_1B"); \
        break;                    \
    }
#else
#define SLITSTOP(m)
#define SLITSTOP2(m)
#endif

void mot_slit(void) {
    MarioWork* m = marioGetPtr();
    f32 d;
    s16 st;
    if (!m->controlStickSensitivity) {
        m->trigFlags &= ~0x8000;
    }
    if (m->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) {
        slitInit();
        m->slitState = 0;
        m->airTimer = 0;
    }
    if (marioRollChgChk()) {
        if (revise360(m->directionView - m->targetYaw) >= 180.0f) {
            m->displayDirection = 0.0f;
            m->displayDirectionTarget = m->displayDirection;
        } else {
            m->displayDirection = 180.0f;
            m->displayDirectionTarget = m->displayDirection;
        }
        m->flags &= ~MARIO_FLAG_PARTY_WALK_DISABLED;
        m->flags &= ~MARIO_FLAG_GONE_THROUGH_BARS;
        m->dispFlags &= CLR_DISP;
        marioPaperOff();
        m->flags &= ~MARIO_FLAG_PAPER_MODE;
        return;
    }
    switch (m->slitState) {
    case 0:
        m->slitTimer = 30;
        m->mot.slit.counter = 0;
        m->slitState = 2;
        m->mot.slit.lastY = m->position.y;
        if (SPEEDCHK_COND(m)) {
            slitSpeedDown(m);
        }
        marioGetPtr();
        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MARIO_CHANGE_SLIT1), &m->position);
        break;
    case 2:
        if (!m->controlStickSensitivity) {
            m->baseSpeed = 0.0f;
            marioChgPose("M_S_1");
#ifdef VERSION_G8ME01
        } else {
            if (SPEEDCHK_COND(m)) {
                slitSpeedDown(m);
            }
            m->directionView = m->controlStickAngle;
#endif
        }
        if (--m->slitTimer <= 0) {
            m->flags |= MARIO_FLAG_PAPER_MODE;
            m->slitState = 4;
            m->wPlayerCollisionBox.x = 6.0f;
        }
        if (m->slitTimer == 0x14) {
            allPartySlitOn();
        }
        break;
    case 4:
        if (marioChkKey()) {
            if (!m->controlStickSensitivity) {
                m->baseSpeed = 0.0f;
                marioChgPose("M_S_1");
            }
            m->directionView = toMovedirSimple(m->displayDirection);
        }
        if (m->baseSpeed >= m->mBaseDashSpeed) {
            slitSpeedDown(m);
        }
        m->displayDirection = 270.0f;
        m->displayDirectionTarget = m->displayDirection;
        if (!m->controlStickSensitivity) {
            marioChgPose("M_S_1");
        }
        marioChgPaper("PM_S_1B");
        m->slitState = 10;
        m->mot.slit.count = 0;
    case 10:
        if (m->position.y != m->mot.slit.lastY) {
            slitCamFix();
        }
        m->mot.slit.lastY = m->position.y;
        if (m->forceMoveTimer != 0) {
            if (!(m->trigFlags & 0x8000)) {
                marioChgPose("M_W_1");
            }
            marioChgPaper("PM_S_1D");
            if (revise360(m->directionView - m->targetYaw) <= 180.0f) {
                m->mot.slit.side = 1;
                m->displayDirection = 260.0f;
            } else {
                m->mot.slit.side = 2;
                m->displayDirection = 280.0f;
            }
            break;
        }
        if (m->baseSpeed >= m->mBaseDashSpeed) {
            slitSpeedDown(m);
        }
        if (marioChkKey()) {
            if (slitContinueChk() != TRUE) {
                m->directionView = m->controlStickAngle;
                m->slitState = 0x64;
                break;
            }
        }
        if (marioChkJump()) {
            return;
        }
        if (!m->controlStickSensitivity) {
            m->baseSpeed = 0.0f;
            if (!m->mot.slit.counter) {
                marioChgPose("M_S_1");
                marioChgPaper("PM_S_1B");
            }
            break;
        }
        if (strcmp(m->animName, "M_I_Y") == 0) {
            marioChgPose("M_W_1");
        }
        if (!(m->mot.slit.flags & 1)) {
            slitCamFix();
        } else {
            m->mot.slit.flags &= ~1;
            slitCamFix();
        }
        m->baseSpeed = slitWalkSpeed();
        m->directionView = m->controlStickAngle;
        if (!(m->trigFlags & 0x8000)) {
            marioChgPose("M_W_1");
        }
        d = revise360(m->controlStickAngle - m->targetYaw);
        if (d >= 315.0f || d <= 45.0f || (d >= 135.0f && d <= 225.0f)) {
            marioChgPaper("PM_S_1E");
        } else {
            marioChgPaper("PM_S_1D");
            slitSetSide(m);
        }
        m->slitState = 0x14;
        break;
    }
    switch (m->slitState) {
    case 0x14:
        if (m->mot.slit.timer > 0) {
            m->mot.slit.timer = 0;
            marioChgPaper("PM_S_1D");
            slitSetDisplayDir(m);
            SLITSTOP(m)
        }
        if (marioChkKey()) {
            if (slitContinueChk() != TRUE) {
                m->slitState = 0x64;
            }
        }
        if (!(m->flags & (MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING))) {
            marioChkJump();
        }
        if (marioChkKey()) {
            if (!m->controlStickSensitivity) {
                m->baseSpeed = 0.0f;
                if (++m->mot.slit.count < 6) {
                    if (slitContinueChk() != TRUE) {
                        m->slitState = 0x64;
                    }
                    break;
                } else {
                    m->slitState = 4;
                    break;
                }
            } else {
                s32 sum;
                m->mot.slit.count = 0;
                sum = __abs((s8)m->wStickDir1) + __abs((s8)m->wStickDir2);
                if (!(m->trigFlags & 0x8000)) {
                    marioChgPose((sum < 0x44) ? "M_W_1" : "M_R_1");
                }
            }
        }
        if (marioChkKey()) {
            m->directionView = m->controlStickAngle;
            m->baseSpeed = slitWalkSpeed();
        }
        d = revise360(m->controlStickAngle - m->targetYaw);
        if (d >= 315.0f || d <= 45.0f || (d >= 135.0f && d <= 225.0f)) {
            marioChgPaper("PM_S_1E");
            m->mot.slit.count = 6;
            m->mot.slit.side = 0;
            slitSetDisplayDir(m);
            break;
        } else {
            marioChgPaper("PM_S_1D");
            d = revise360(m->directionView - m->targetYaw);
            if (m->mot.slit.side == 0) {
                m->mot.slit.side = (d >= 180.0f) ? 2 : 1;
            }
            if (m->mot.slit.side == 1) {
                if (d >= 180.0f) {
                    marioChgPaper("PM_S_2");
                    m->mot.slit.timer = 6;
                    m->slitState = 0x15;
                    m->mot.slit.side = 2;
                    m->baseSpeed = slitWalkSpeed();
                    m->displayDirection = 260.0f;
                    SLITSTOP2(m)
                }
            } else if (m->mot.slit.side == 2) {
                if (d <= 180.0f) {
                    marioChgPaper("PM_S_2");
                    m->mot.slit.timer = 6;
                    m->slitState = 0x15;
                    m->mot.slit.side = 1;
                    m->baseSpeed = slitWalkSpeed();
                    m->displayDirection = 280.0f;
                    SLITSTOP2(m)
                }
            }
        }
        break;
    case 0x15:
        if (--m->mot.slit.timer > 0) {
            SLITSTOP2(m)
            break;
        }
        marioChgPaper("PM_S_1D");
        m->slitState = 0x14;
        m->displayDirection = (m->mot.slit.side == 1) ? 260.0f : 280.0f;
        break;
    }
    if (m->flags & MARIO_FLAG_PAPER_MODE) {
        if (m->hitobjPush != NULL) {
            if (m->hitobjStandOn != NULL && m->currMotionId == MARIO_MOTION_SLIT) {
                if (hitGetAttr(m->hitobjPush) & HIT_ATTR_SLIT) {
                    HitCheckQuery q;
                    HitCheckQuery* qp;
                    f32 sn, cs;
                    d = revise360(m->controlStickAngle - m->targetYaw);
                    if (d > 90.0f && d < 270.0f) {
                        m->mot.slit.pushDir = 180.0f;
                    } else {
                        m->mot.slit.pushDir = 0.0f;
                    }
                    if (++m->mot.slit.counter >= sysMsec2Frame(50)) {
                        qp = &q;
                        m->directionView = revise360(m->mot.slit.pushDir + m->targetYaw);
                        m->mot.slit.counter = 0;
                        sincosf(&sn, &cs, m->directionView);
                        { Vec tmp = {0, 0, 0};
                        q.targetPos.x = m->position.x;
                        q.targetPos.y = 11.0f + m->position.y;
                        q.targetPos.z = m->position.z;
                        tmp.x = sn;
                        tmp.z = cs;
                        q.targetDir = tmp; }
                        q.targetDistance = 20.0f;
                        if (hitCheckVecHitObjXZ(qp, m->hitobjPush)) {
                            q.targetPos.x = m->position.x;
                            q.targetPos.z = m->position.z;
                            q.targetDistance = 20.0f;
                            movePos(&qp->targetPos.x, &qp->targetPos.z, 1.0f, reviseAngle(90.0f + m->moveDirection));
                            if (hitCheckVecHitObjXZ(qp, m->hitobjPush)) {
                                q.targetPos.x = m->position.x;
                                q.targetPos.z = m->position.z;
                                q.targetDistance = 20.0f;
                                movePos(&qp->targetPos.x, &qp->targetPos.z, 1.0f, reviseAngle(m->moveDirection - 90.0f));
                                if (hitCheckVecHitObjXZ(qp, m->hitobjPush)) {
                                    m->slitState = 0x50;
                                }
                            }
                        }
                    }
                } else {
                    m->mot.slit.counter = 0;
                }
            }
        } else {
            st = m->slitState;
            if (!(st >= 0x5a && st <= 0x5e) && m->hitobjStandOn != NULL && (hitGetAttr(m->hitobjStandOn) & HIT_ATTR_SLIT)) {
                m->mot.slit.counter += 3;
                marioChgPaper("PM_S_1H");
                if (m->mot.slit.counter >= 0xf && m->currMotionId == MARIO_MOTION_SLIT) {
                    m->slitState = 0x5a;
                }
            } else {
                m->mot.slit.counter--;
                if (m->mot.slit.counter < 0) {
                    m->mot.slit.counter = 0;
                }
            }
        }
    }
    switch (m->slitState) {
    case 0x50:
        camGetPtr(CAMERA_2D)->flags |= CAMERA_FLAG_LETTER_BOX_OFF;
        slitCamFix();
        m->flags |= MARIO_FLAG_CHG_MOT_DISABLED;
        if (0.0f == m->mot.slit.pushDir) {
            marioChgPaper("PM_S_1F");
        } else {
            marioChgPaper("PM_S_1G");
        }
        m->flags |= MARIO_FLAG_WALLS_DISABLED;
        m->slitTimer = 20;
        m->baseSpeed = (2.0f * m->wPlayerCollisionBox.x) / (f32)m->slitTimer;
        m->slitState = 0x51;
        m->flags |= MARIO_FLAG_GONE_THROUGH_BARS;
        break;
    case 0x51:
        if (--m->slitTimer <= 0) {
            m->slitState = 0x52;
        }
        break;
    case 0x52:
        camGetPtr(CAMERA_2D)->flags &= ~CAMERA_FLAG_LETTER_BOX_OFF;
        m->flags &= ~MARIO_FLAG_WALLS_DISABLED;
        m->slitState = 0x14;
        m->flags &= ~MARIO_FLAG_CHG_MOT_DISABLED;
        break;
    }
    switch (m->slitState) {
    case 0x5a:
        camGetPtr(CAMERA_2D)->flags |= CAMERA_FLAG_LETTER_BOX_OFF;
        slitCamFix();
        m->flags |= MARIO_FLAG_CHG_MOT_DISABLED;
        marioChgPaper("PM_S_1H");
        m->slitTimer = sysMsec2Frame(500);
        m->baseSpeed = 0.0f;
        m->slitState = 0x5b;
        {
            MarioWork* p = marioGetPtr();
            p->slitPosSet = 1;
            // copy
            p->slitPos = p->position;
        }
    case 0x5b:
        m->slitTimer = 15;
        m->slitState = 0x5c;
        m->flags |= MARIO_FLAG_FLOORS_DISABLED;
        marioSetFallPara();
        m->flags &= ~MARIO_FLAG_IS_JUMPING;
        m->flags |= (MARIO_FLAG_IS_FALLING | MARIO_FLAG_HAS_INPUT_JUMP);
    case 0x5c:
        slitSetDisplayDir(m);
        if (--m->slitTimer > 0) {
            break;
        }
        m->slitState = 0x5d;
        m->flags &= ~MARIO_FLAG_FLOORS_DISABLED;
        m->flags &= ~MARIO_FLAG_CHG_MOT_DISABLED;
    case 0x5d:
        if (m->hitobjStandOn == NULL) {
            break;
        }
        m->flags &= ~(MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING | MARIO_FLAG_IS_STEPPING | MARIO_FLAG_HAS_INPUT_JUMP);
        m->slitState = 0x5e;
    case 0x5e:
        camGetPtr(CAMERA_2D)->flags &= ~CAMERA_FLAG_LETTER_BOX_OFF;
        m->slitState = 0x14;
        break;
    }
    switch (m->slitState) {
    case 0x64:
        m->flags &= ~MARIO_FLAG_GONE_THROUGH_BARS;
        m->slitTimer2 = 0;
        m->flags &= ~MARIO_FLAG_PAPER_MODE;
        m->wPlayerCollisionBox.x = 20.0f;
#ifdef VERSION_G8ME01
        if (m->controlStickSensitivity) {
            slitSpeedDown(m);
        }
#endif
        if (m->baseSpeed == 0.0f) {
            marioChgPose("M_S_1");
        }
        marioChgPaper("PM_S_1C");
        m->slitTimer2 = 30;
        m->slitState = 0x65;
        if (revise360(m->directionView - m->targetYaw) >= 180.0f) {
            m->displayDirection = 0.0f;
            m->displayDirectionTarget = m->displayDirection;
            if (!m->controlStickSensitivity) {
                m->directionView = toMovedirSimple(m->displayDirection);
            }
        } else {
            m->displayDirection = 180.0f;
            m->displayDirectionTarget = m->displayDirection;
            if (!m->controlStickSensitivity) {
                m->directionView = toMovedirSimple(m->displayDirection);
            }
        }
        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MARIO_CHANGE_SLIT2), &m->position);
    case 0x65:
        if (!m->controlStickSensitivity) {
            m->baseSpeed = 0.0f;
            marioChgPose("M_S_1");
#ifdef VERSION_G8ME01
        } else {
            slitSpeedDown(m);
            m->directionView = m->controlStickAngle;
#endif
        }
        if (--m->slitTimer2 > 0) {
            break;
        }
        m->slitState = 0x66;
        break;
    case 0x66:
        if (!m->controlStickSensitivity) {
            m->baseSpeed = 0.0f;
            marioChgPose("M_S_1");
#ifdef VERSION_G8ME01
        } else {
            slitSpeedDown(m);
#endif
        }
        m->flags &= ~MARIO_FLAG_PARTY_WALK_DISABLED;
        m->flags &= ~MARIO_FLAG_GONE_THROUGH_BARS;
        m->dispFlags &= CLR_DISP;
        marioPaperOff();
        mapResetPaperAmbColor();
        m->dispFlags &= ~MARIO_DISP_FLAG_NO_REAR_POSE;
        marioAdjustMoveDir();
#ifndef VERSION_G8ME01
        allPartySlitOff();
#endif
        slitCamFix();
        marioChgMotSub(MARIO_MOTION_STAY, 0);
        break;
    case 0x6e:
        m->flags &= ~MARIO_FLAG_PARTY_WALK_DISABLED;
        m->flags &= ~MARIO_FLAG_GONE_THROUGH_BARS;
        m->flags &= ~MARIO_FLAG_CHG_MOT_DISABLED;
        m->flags &= ~MARIO_FLAG_WALLS_DISABLED;
        m->slitTimer2 = 0;
        m->flags &= ~MARIO_FLAG_PAPER_MODE;
        m->wPlayerCollisionBox.x = 20.0f;
        if (!m->controlStickSensitivity) {
            marioChgPose("M_S_1");
        }
        marioChgPaper("PM_S_1C");
        m->slitTimer2 = 30;
        m->slitState = 0x6f;
        m->baseSpeed = 0.0f;
        if (revise360(m->directionView - m->targetYaw) >= 180.0f) {
            m->displayDirection = 0.0f;
            m->displayDirectionTarget = m->displayDirection;
            m->directionView = toMovedirSimple(m->displayDirection);
        } else {
            m->displayDirection = 180.0f;
            m->displayDirectionTarget = m->displayDirection;
            m->directionView = toMovedirSimple(m->displayDirection);
        }
        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MARIO_CHANGE_SLIT2), &m->position);
    case 0x6f:
        if (--m->slitTimer2 > 0) {
            break;
        }
        m->slitState = 0x70;
        break;
    case 0x70:
        m->flags &= ~MARIO_FLAG_PARTY_WALK_DISABLED;
        m->flags &= ~MARIO_FLAG_GONE_THROUGH_BARS;
        m->dispFlags &= CLR_DISP;
        marioPaperOff();
        mapResetPaperAmbColor();
        m->dispFlags &= ~MARIO_DISP_FLAG_NO_REAR_POSE;
        marioAdjustMoveDir();
        allPartySlitOff();
        slitCamFix();
        marioChgMot(MARIO_MOTION_STAY);
        break;
    case 0x78:
        m->slitTimer2 = 0;
        m->flags &= ~MARIO_FLAG_PAPER_MODE;
        m->wPlayerCollisionBox.x = 20.0f;
        if (!m->controlStickSensitivity) {
            marioChgPose("M_S_1");
        }
        marioChgPaper("PM_S_1C");
        m->slitTimer2 = 30;
        m->slitState = 0x79;
        m->baseSpeed = 0.0f;
        if (revise360(m->directionView - m->targetYaw) >= 180.0f) {
            m->displayDirection = 0.0f;
            m->displayDirectionTarget = m->displayDirection;
            m->directionView = toMovedirSimple(m->displayDirection);
        } else {
            m->displayDirection = 180.0f;
            m->displayDirectionTarget = m->displayDirection;
            m->directionView = toMovedirSimple(m->displayDirection);
        }
        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MARIO_CHANGE_SLIT2), &m->position);
    case 0x79:
        if (--m->slitTimer2 > 0) {
            break;
        }
        m->slitState = 0x7a;
        break;
    case 0x7a:
        m->flags &= ~MARIO_FLAG_PARTY_WALK_DISABLED;
        m->flags &= ~MARIO_FLAG_GONE_THROUGH_BARS;
        m->flags &= ~MARIO_FLAG_WALLS_DISABLED;
        m->dispFlags &= CLR_DISP;
        marioPaperOff();
        mapResetPaperAmbColor();
        m->dispFlags &= ~MARIO_DISP_FLAG_NO_REAR_POSE;
        marioAdjustMoveDir();
        allPartySlitOff();
        slitCamFix();
        marioChgMot(MARIO_MOTION_STAY);
        break;
    }
}
