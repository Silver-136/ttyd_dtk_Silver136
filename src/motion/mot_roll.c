#pragma defer_codegen on
#include "dolphin/types.h"
#include "driver/hitdrv.h"
#include "mario/mario.h"
#include "mario/mario_cam.h"
#include "mario/mario_motion.h"
#include "mario/mariost.h"
#include "memory.h"
#include "pmario_sound.h"
#include <string.h>
#include "libc_decls.h"
#include <math.h>
#include <dolphin/pad.h>
#include "sfx_name_word.h"
#include "mario/mario_sbr.h"
#include "system.h"
#include "mario/mario_hit.h"
#include "mot/mot_roll.h"

HitEntry* marioChkLandon(f32* outY, f32 y);
void marioLandOn(void);
void marioClearJumpPara(void);
void marioGetRubInit(void);
void allPartyRideShip(void);
void allPartyRideOff(void);
void allPartyForceRideOn(void);

#define CAMF() (camGetPtr((CameraId)8)->flags)

typedef struct MotRollWork {
    f32 spdX;           // 0x00: roll velocity x (marioResetRollSpd clears it)
    f32 spdZ;           // 0x04: roll velocity z
    s32 squashTrigger;  // 0x08: nonzero restarts the squash animation (-> squashState 1)
    s32 squashState;    // 0x0C: y-scale squash state machine (1.4 / back to 1.0)
    s32 field_0x10;     // 0x10
#ifdef VERSION_G8ME01
    s32 sfxId;          // 0x14: US only, roll loop (0x17B), -1 when off
#endif
} MotRollWork;          // size 0x14 JP, 0x18 US

#define RW() (marioGetPtr()->motRoll)

f32 roll_grx;
f32 roll_grz;
s32 roll_flyout_flag2;
static __declspec(section ".rodata") Vec rollOfs = {0.0f, 40.0f, 0.0f};

f32 rollData[] = {3.0f, -0.05f, -0.5f, 0.5f, 7.0f, 180.0f, 140.0f, 10.0f, 1.2f, 2.0f, 0.8f};

static inline f32 trunc10(f32 x) {
    s32 i = (s32)(10.0f * x);
    f32 f = (f32)i;
    return f / 10.0f;
}

static inline s32 angOutT(f32 center, f32 half, f32 x) {
    s32 w = 0;
    s32 r = w;
    f32 c = trunc10(center);
    f32 lo;
    f32 hi;
    lo = c - half;
    if (lo < 0.0f) {
        w = 1;
        lo += 360.0f;
    }
    hi = c + half;
    if (hi > 360.0f) {
        hi -= 360.0f;
        w = 2;
    }
    if (w == 1) {
        if (hi < x && x < lo) {
            r = 1;
        }
    } else if (w == 2) {
        if (hi < x && x < lo) {
            r = 1;
        }
    } else if (w == 0) {
        if (x < lo || hi < x) {
            r = 1;
        }
    }
    return r;
}

static inline s32 angOut(f32 center, f32 half, f32 x) {
    return angOutT(center, half, x);
}

/* repurposed unused inline: angOut on the mario stick angle, with everything read inside */
// FAKE: k025/rabs/rollRad/rollCos/rSin are invented one-line inline helpers. Their result temps get
// later inline numbering, which gives the target FPR colouring in mot_roll (fmadds operand order, makeVec c
// in f31) and marioRollCan (v26 in f26, fabs/sin/cos volatiles). Behaviour-identical to the expressions they wrap.
static inline f32 k025(void) { return 0.025f; }
static inline f32 rSin(f32 a) { return (f32)sin(a); }
static inline f32 rabs(f32 x) { return __fabsf(x); }
static inline f32 rollRad(f32 d) { return 3.1415927f * d / 180.0f; }
static inline f32 rollCos(f32 a) { return (f32)cos(a); }
static inline s32 makeVec(f32 half) {
    s32 w = 0;
    s32 r = w;
    MarioWork* m = marioGetPtr();
    f32 c = (f32)(s32)(10.0f * m->moveDirection) / 10.0f;
    f32 x = angleABf(0.0f, 0.0f, (f32)(s32)m->wStickDir1, (f32)(s32)m->wStickDir2);
    f32 lo;
    f32 hi;
    lo = c - half;
    if (lo < 0.0f) {
        w = 1;
        lo += 360.0f;
    }
    hi = c + half;
    if (hi > 360.0f) {
        hi -= 360.0f;
        w = 2;
    }
    if (w == 1) {
        if (hi < x && x < lo) {
            r = 1;
        }
    } else if (w == 2) {
        if (hi < x && x < lo) {
            r = 1;
        }
    } else if (w == 0) {
        if (x < lo || hi < x) {
            r = 1;
        }
    }
    return r;
}

#define DISPDIR_BODY(a, b, m2)                                                                    \
    if (angOut(b, 135.0f, a) == 1) {                                                              \
        b = revise360(180.0f + b);                                                                \
    }                                                                                             \
    {                                                                                             \
        f32 ty_ = trunc10((m2)->targetYaw); f32 dd = revise360(-(ty_ - trunc10(b))); \
        s32 sgn = (dd < 180.0f) ? 0 : 1;                                                          \
        if (sgn == 0) {                                                                           \
            if (__fabsf(b - a) > 90.0f) {                                                            \
                if (a > b) {                                                                      \
                    b += 360.0f;                                                                  \
                }                                                                                 \
            }                                                                                     \
        } else {                                                                                  \
            if (__fabsf(b - a) > 90.0f) {                                                            \
                if (a < b) {                                                                      \
                    a += 360.0f;                                                                  \
                }                                                                                 \
            }                                                                                     \
        }                                                                                         \
    }                                                                                             \
    if (angOut(b, 90.0f, a) == 0) {                                                               \
        if (__fabsf(b - a) > 180.0f) {                                                               \
            if (b > a) {                                                                          \
                b -= 360.0f;                                                                      \
            } else {                                                                              \
                a -= 360.0f;                                                                      \
            }                                                                                     \
        }                                                                                         \
    }                                                                                             \
    b = 1.2f * (a - b) + b;                                                                       \
    b = revise360(b);                                                                             \
    {                                                                                             \
        f32 md = toMovedir(mario->displayDirection);                                                            \
        if (mario->hammerDir) {                                                                           \
            md = revise360(180.0f + md);                                                          \
        }                                                                                         \
        if (angOut(b, 135.0f, md) == 1) {                                                         \
            mario->hammerDir = (mario->hammerDir + 1) & 1;                                                        \
        }                                                                                         \
    }                                                                                             \
    { f32 t_ = 270.0f + mario->targetYaw - b; mario->displayDirection = revise360(t_ + (mario->hammerDir ? -180.0f : 0.0f)); } \
    if (mario->displayDirection >= 200.0f) {                                                                    \
        mario->displayDirection = revise360(270.0f + mario->targetYaw - b + (mario->hammerDir ? -180.0f : 0.0f));              \
    }                                                                                             \
    if (mario->displayDirection < 170.0f) {                                                                     \
        mario->displayDirection = revise360(270.0f + mario->targetYaw - b + (mario->hammerDir ? -180.0f : 0.0f));              \
    }

// FAKE: marioSetRollDispDir copy of DISPDIR_BODY: dd = tb_ - ty_ (was -(ty_ - tb_); differs only in the sign of zero, and dd is
// only compared with 180) and a dead int-guarded re-def of ty_ (COLOURING M) keep ty_ as a named value created before tb_, as the target fsubs f1,f0,f1
#define DISPDIR_BODY2(a, b, m2)                                                                    \
    if (angOut(b, 135.0f, a) == 1) {                                                              \
        b = revise360(180.0f + b);                                                                \
    }                                                                                             \
    {                                                                                             \
        s32 g_; f32 ty_; f32 dd; s32 sgn; f32 tb_; ty_ = trunc10((m2)->targetYaw); tb_ = trunc10(b); g_ = 0; if (g_ != 0) { ty_ = 0.0f; } dd = revise360(tb_ - ty_); sgn = (dd < 180.0f) ? 0 : 1;                                                          \
        if (sgn == 0) {                                                                           \
            if (__fabsf(b - a) > 90.0f) {                                                            \
                if (a > b) {                                                                      \
                    b += 360.0f;                                                                  \
                }                                                                                 \
            }                                                                                     \
        } else {                                                                                  \
            if (__fabsf(b - a) > 90.0f) {                                                            \
                if (a < b) {                                                                      \
                    a += 360.0f;                                                                  \
                }                                                                                 \
            }                                                                                     \
        }                                                                                         \
    }                                                                                             \
    if (angOut(b, 90.0f, a) == 0) {                                                               \
        if (__fabsf(b - a) > 180.0f) {                                                               \
            if (b > a) {                                                                          \
                b -= 360.0f;                                                                      \
            } else {                                                                              \
                a -= 360.0f;                                                                      \
            }                                                                                     \
        }                                                                                         \
    }                                                                                             \
    b = 1.2f * (a - b) + b;                                                                       \
    b = revise360(b);                                                                             \
    {                                                                                             \
        f32 md = toMovedir(mario->displayDirection);                                                            \
        if (mario->hammerDir) {                                                                           \
            md = revise360(180.0f + md);                                                          \
        }                                                                                         \
        if (angOut(b, 135.0f, md) == 1) {                                                         \
            mario->hammerDir = (mario->hammerDir + 1) & 1;                                                        \
        }                                                                                         \
    }                                                                                             \
    { f32 t_ = 270.0f + mario->targetYaw - b; mario->displayDirection = revise360(t_ + (mario->hammerDir ? -180.0f : 0.0f)); } \
    if (mario->displayDirection >= 200.0f) {                                                                    \
        mario->displayDirection = revise360(270.0f + mario->targetYaw - b + (mario->hammerDir ? -180.0f : 0.0f));              \
    }                                                                                             \
    if (mario->displayDirection < 170.0f) {                                                                     \
        mario->displayDirection = revise360(270.0f + mario->targetYaw - b + (mario->hammerDir ? -180.0f : 0.0f));              \
    }

void roll_upstairs(void);
static inline void marioRollCan(void);

#pragma inline_max_total_size(200000)

void marioRollFixOn(void) {
    MarioWork* mario = marioGetPtr();
    if (mario->currMotionId == MARIO_MOTION_ROLL) {
        mario->mot.roll.flags |= 0x200;
    }
}

s32 marioRollCancelDisable(void) {
    MarioWork* mario = marioGetPtr();
    if (mario->mot.roll.flags & 0x200) {
        return 1;
    }
    if ((mario->flags & MARIO_FLAG_IS_TOUCHING_CEILING) != 0) {
        return 1;
    }
    return !mario->hitobjStandOn ? 1 : (marioSlitChkWallAround() != 0);
}

s32 marioRollKeyDisable(void) {
    MarioWork* mario = marioGetPtr();
    if (mario->currMotionId != MARIO_MOTION_ROLL) {
        return 1;
    }
    {
        s32 sub = (s32)mario->currSubMotionId;
        if (sub >= 0 && sub <= 1) {
            return 0;
        }
        if (sub >= 0x14 && sub <= 0x16) {
            return 0;
        }
    }
    return 1;
}

void setRollEvtFlag(void) {
    MarioWork* mario = marioGetPtr();
    mario->mot.roll.flags |= 0x100;
}

void clrRollEvtFlag(void) {
    MarioWork* mario = marioGetPtr();
    mario->mot.roll.flags &= ~0x100;
}

s32 getRollEvtFlag(void) {
    s32 r = 0;
    MarioWork* mario = marioGetPtr();
    if (mario->flags & MARIO_FLAG_TUBE_MODE) {
        if (mario->mot.roll.flags & 0x100) {
            r = 1;
        }
    }
    return r;
}

void marioResetRollSpd(void) {
    MarioWork* mario = marioGetPtr();
    if (mario->currMotionId == MARIO_MOTION_ROLL) {
        if (marioGetPtr()->motStruct) {
            f32 a = 0.0f;
            f32 b;
            RW()->spdX = a;
            b = 0.0f;
            RW()->spdZ = b;
        }
        mario->baseSpeed = 0.0f;
    }
}

void marioForceRollAnime(void) {
    MarioWork* mario = marioGetPtr();
    marioPaperOff();
    if (mario) {
    }
    marioChgPaper(0);
    marioPaperOn("p_roll");
    marioChgPose("M_I_U");
    marioChgPaper("PM_R_1B");
    mario->dispFlags |= 8;
    mario->dispFlags |= 0x100;
}

s32 marioRollTryCancel(void) {
    s32 ret = 0;
    MarioWork* mario = marioGetPtr();
    if (!(mario->flags & MARIO_FLAG_TUBE_MODE)) {
        return 0;
    }
    if (mario->mot.roll.flags & 0x200) {
        return 0;
    }
    {
        s32 r;
        MarioWork* m = marioGetPtr();
        if (m->mot.roll.flags & 0x200) {
            r = 1;
        } else if (m->flags & MARIO_FLAG_IS_TOUCHING_CEILING) {
            r = 1;
        } else if (!m->hitobjStandOn) {
            r = 1;
        } else if (marioSlitChkWallAround()) {
            r = 1;
        } else {
            r = 0;
        }
        if (!r) {
            ret = 1;
            mario->currSubMotionId = 0x14;
            CAMF() |= 0x200;
        }
    }
    return ret;
}

s32 marioRollCancel(void) {
    MarioWork* mario = marioGetPtr();
    if (!(mario->flags & MARIO_FLAG_TUBE_MODE)) {
        return 0;
    }
    if (mario->mot.roll.flags & 0x200) {
        return 0;
    }
    mario->currSubMotionId = 0x14;
    CAMF() |= 0x200;
    return 1;
}

void mot_roll_post(void) {
    MarioWork* mario = marioGetPtr();
    MarioWork* m = marioGetPtr();
#ifdef VERSION_G8ME01
    if (marioGetPtr()->motStruct && (u32)RW()->sfxId != (u32)-1) {
        psndSFXOff(RW()->sfxId);
        RW()->sfxId = -1;
    }
#else
    if ((u32)m->mot.roll.sfxId != (u32)-1) {
        psndSFXOff(m->mot.roll.sfxId);
        m->mot.roll.sfxId = -1;
    }
#endif
    m = marioGetPtr();
    m->camShiftTargetZ = 0.0f;
    m->camShiftRate.z = 0.004f;
    {
        f32 d = revise360(mario->directionView - mario->targetYaw);
        do { if (d >= 180.0f) { mario->displayDirection = 0.0f; mario->displayDirectionTarget = 0.0f; mario->moveDirection = 270.0f; mario->directionView = 270.0f; } else { mario->displayDirection = 180.0f; mario->displayDirectionTarget = 180.0f; mario->moveDirection = 90.0f; mario->directionView = 90.0f; } } while (0);
    }
    marioSetSpec();
    mario->rollFlags = 0;
    mario->rotOfs = (Vec){0.0f, 0.0f, 0.0f};
    mario->dispOfs = (Vec){0.0f, 0.0f, 0.0f};
    mario->rot = (Vec){0.0f, 0.0f, 0.0f};
    mario->dispFlags &= ~0x100;
    mario->dispFlags &= ~8;
    mario->flags &= ~MARIO_FLAG_TUBE_MODE;
    marioPaperOff();
    marioAdjustMoveDir();
    marioChgPose("M_S_1");
    if (marioGetPtr()->motStruct) {
        __memFree(HEAP_DEFAULT, marioGetPtr()->motStruct);
        mario = 0;
        marioGetPtr()->motStruct = (void*)mario;
    }
    allPartyRideOff();
}

void marioRollForceCancel(void) {
    MarioWork* mario = marioGetPtr();
    if (mario->currMotionId == MARIO_MOTION_ROLL) {
        MarioWork* m = marioGetPtr();
#ifdef VERSION_G8ME01
        if (marioGetPtr()->motStruct && (u32)RW()->sfxId != (u32)-1) {
            psndSFXOff(RW()->sfxId);
            RW()->sfxId = -1;
        }
#else
        if ((u32)m->mot.roll.sfxId != (u32)-1) {
            psndSFXOff(m->mot.roll.sfxId);
            m->mot.roll.sfxId = -1;
        }
#endif
        m = marioGetPtr();
        m->camShiftTargetZ = 0.0f;
        m->camShiftRate.z = 0.004f;
        {
            f32 d = revise360(mario->directionView - mario->targetYaw);
            if (d >= 180.0f) {
                mario->displayDirection = 0.0f;
                mario->displayDirectionTarget = 0.0f;
                mario->moveDirection = 270.0f;
                do { mario->directionView = 270.0f; } while (0);
            } else {
                mario->displayDirection = 180.0f;
                mario->displayDirectionTarget = 180.0f;
                mario->moveDirection = 90.0f;
                mario->directionView = 90.0f;
            }
        }
        marioSetSpec();
        mario->rollFlags = 0;
        mario->rotOfs = (Vec){0.0f, 0.0f, 0.0f};
        mario->dispOfs = (Vec){0.0f, 0.0f, 0.0f};
        mario->rot = (Vec){0.0f, 0.0f, 0.0f};
        mario->dispFlags &= ~0x100;
        mario->dispFlags &= ~8;
        mario->flags &= ~MARIO_FLAG_TUBE_MODE;
        marioPaperOff();
        marioAdjustMoveDir();
        marioChgPose("M_S_1");
        if (marioGetPtr()->motStruct) {
            __memFree(HEAP_DEFAULT, marioGetPtr()->motStruct);
            mario = 0;
            marioGetPtr()->motStruct = (void*)mario;
        }
        allPartyRideOff();
        marioChgMotSub(MARIO_MOTION_STAY, 0);
    }
}

void marioSetRollDispDir(void) {
    MarioWork* mario = marioGetPtr();
    f32 b, a;
    MarioWork* m2;
    a = mario->directionView;
    b = mario->moveDirection;
    m2 = marioGetPtr();
    DISPDIR_BODY2(a, b, m2)
}

void rollEvtJumpSetup(void) {
    MarioWork* mario = marioGetPtr();
    camFollowYOff();
    mario->flags |= MARIO_FLAG_IS_JUMPING;
    mario->airTimer = 1;
}

void marioReInit_roll(void) {
    void* p;
    MarioWork* mario = marioGetPtr();
    f32 z0;
    f32 z1;
    p = __memAlloc(HEAP_DEFAULT, sizeof(MotRollWork));
    marioGetPtr()->motStruct = p;
    memset(marioGetPtr()->motStruct, 0, sizeof(MotRollWork));
    {
        MarioWork* t = marioGetPtr();
#ifdef VERSION_G8ME01
        if (marioGetPtr()->motStruct) RW()->sfxId = -1;
#else
        t->mot.roll.sfxId = -1;
#endif
    }
    mario->trigFlags &= ~MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
    mario->currMotionId = MARIO_MOTION_ROLL;
    mario->flags |= MARIO_FLAG_TUBE_MODE;
    mario->rollFlags = 0x10;
    marioPaperOn("p_roll");
    marioChgPose("M_I_U");
    marioChgPaper("PM_R_1B");
    mario->dispFlags |= 8;
    mario->dispFlags |= 0x100;
    mario->currSubMotionId = 0x1e;
    mario = marioGetPtr();
    z0 = 0.0f;
    mario->wPlayerCollisionBox.x = 14.8f;
    mario->wPlayerCollisionBox.y = 14.8f;
    mario->mBaseDashSpeed = rollData[0];
    RW()->spdX = z0;
    z1 = 0.0f;
    RW()->spdZ = z1;
    mario->rotOfs.x = -6.0f;
    mario->rotOfs.y = 6.0f;
    mario->dispOfs.x = 6.0f;
    mario->dispOfs.y = 0.0f;
    mario->scale.x = 1.0f;
    mario->scale.y = 1.0f;
    mario->scale.z = 1.0f;
    mario->tubeScaleY = 1.0f;
    allPartyForceRideOn();
}

static inline s32 rollChk(void) {
    s32 r;
    MarioWork* m = marioGetPtr();
    if (m->mot.roll.flags & 0x200) {
        r = 1;
    } else if (m->flags & MARIO_FLAG_IS_TOUCHING_CEILING) {
        r = 1;
    } else if (!m->hitobjStandOn) {
        r = 1;
    } else if (marioSlitChkWallAround()) {
        r = 1;
    } else {
        r = 0;
    }
    return r;
}

void mot_roll(void) {
    MarioWork* mario = marioGetPtr();
    f32 v31;
    f32 v29;
    f32 v28;
    f32 v30;
    f32 v27;
    f32 v26;
    f32 tcA;
    f32 nx;
    f32 nz;
    f32 a31;
    f32 b31;
    f32 s;
    f32 rd;
    if (mario->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) {
        mario->trigFlags &= ~MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
        mario->flags &= ~(MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING | MARIO_FLAG_IS_STEPPING | MARIO_FLAG_HAS_INPUT_JUMP);
        mario->mot.roll.flags = 0;
        mario->mot.word[2] = 0;
        mario->mot.word[3] = 0;
        mario->mot.word[4] = 0;
        mario->mot.word[5] = 0;
        {
            void* p = __memAlloc(HEAP_DEFAULT, sizeof(MotRollWork));
            marioGetPtr()->motStruct = p;
        }
        memset(marioGetPtr()->motStruct, 0, sizeof(MotRollWork));
        {
            MarioWork* t = marioGetPtr();
#ifdef VERSION_G8ME01
            if (marioGetPtr()->motStruct) RW()->sfxId = -1;
#else
            t->mot.roll.sfxId = -1;
#endif
        }
        mario->currSubMotionId = 0;
    }
    switch (mario->currSubMotionId) {
    case 0: {
        MarioWork* m;
        s32 z0 = 0;
        s32 z1;
        CAMF() |= 0x200;
        marioAdjustMoveDir();
        v31 = mario->displayDirection;
        if (v31 < 90.0f || v31 > 270.0f) {
            mario->hammerDir = 1;
            mario->directionView = revise360(270.0f - mario->targetYaw);
            mario->moveDirection = 270.0f;
        } else {
            mario->hammerDir = 0;
            mario->directionView = revise360(90.0f - mario->targetYaw);
            mario->moveDirection = 90.0f;
        }
        marioGetRubInit();
        mario->flags |= MARIO_FLAG_TUBE_MODE;
        mario->rollFlags = 0x10;
        mario->dispFlags |= 8;
        mario->dispFlags |= 0x100;
        marioPaperOn("p_roll");
        marioChgPose("M_I_U");
        marioChgPaper("PM_R_1A");
        mario->multiTimer = 0x41;
        m = marioGetPtr();
        v28 = 0.0f;
        m->wPlayerCollisionBox.x = 14.8f;
        m->wPlayerCollisionBox.y = 14.8f;
        m->mBaseDashSpeed = rollData[0];
        RW()->spdX = v28;
        v27 = 0.0f;
        RW()->spdZ = v27;
        m->rotOfs.x = -6.0f;
        m->rotOfs.y = 6.0f;
        m->dispOfs.x = 6.0f;
        m->dispOfs.y = 0.0f;
        m->scale.x = 1.0f;
        m->scale.y = 1.0f;
        m->scale.z = 1.0f;
        m->tubeScaleY = 1.0f;
        mario->airTimer = 0;
        if (mario->forceMoveTimer == 0) {
            mario->baseSpeed = 0.0f;
        }
        m = marioGetPtr();
        m->camShiftTargetZ = 0.4f;
        m->camShiftRate.z = 0.01f;
        m->camZoom = 2.0f;
        ((MarioWork*)m)->camZoomOfs = rollOfs;
        RW()->squashTrigger = z0;
        z1 = 0;
        RW()->squashState = z1;
        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MARIO_CHANGE_ROLL1), &mario->position);
        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_VOICE_MARIO_FLD_JUMP3), &mario->position);
        mario->currSubMotionId = 1;
        break;
    }
    case 1:
        if (mario->multiTimer == 0x36) {
            allPartyRideShip();
        }
        mario->dispOfs.y += 0.1f;
        if (mario->dispOfs.y > 1.0f) {
            mario->dispOfs.y = 1.0f;
        }
        if (--mario->multiTimer <= 0) {
            mario->currSubMotionId = 0x1e;
            marioChgPose("M_I_U");
            marioChgPaper("PM_R_1B");
            CAMF() &= ~0x200;
        }
        break;
    case 0x14:
        mario->baseSpeed = 0.0f;
        mario->rot.x = 0.0f;
        mario->rot.z = 0.0f;
        mario->rotOfs.x = 0.0f;
        mario->rotOfs.y = 0.0f;
        v31 = revise360(mario->directionView - mario->targetYaw);
        mario->dispOfs.x = (v31 < 180.0f) ? 3.0f : -3.0f;
        mario->dispOfs.y = 0.0f;
        mario->multiTimer = 0x41;
        mario->currSubMotionId = 0x15;
        mario->displayDirection = 0.0f;
        {
            MarioWork* m = marioGetPtr();
#ifdef VERSION_G8ME01
            if (marioGetPtr()->motStruct && (u32)RW()->sfxId != (u32)-1) {
                psndSFXOff(RW()->sfxId);
                RW()->sfxId = -1;
            }
#else
            if ((u32)m->mot.roll.sfxId != (u32)-1) {
                psndSFXOff(m->mot.roll.sfxId);
                m->mot.roll.sfxId = -1;
            }
#endif
        }
        {
            MarioWork* m = marioGetPtr();
            m->camShiftTargetZ = 0.0f;
            m->camShiftRate.z = 0.004f;
        }
        marioChgPaper("PM_R_1C");
        marioPaperLightOff();
        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MARIO_CHANGE_ROLL2), &mario->position);
    case 0x15:
        if (--mario->multiTimer > 0) {
            v31 = revise360(mario->directionView - mario->targetYaw);
            if (v31 < 180.0f) {
                f32 t = 90.0f;
                f32 diff = t - mario->displayDirection;
                f32 n = 0.1f * diff;
                n += mario->displayDirection;
                if (__fabsf((f32)((t - n) < 1.0f))) {
                    n = t;
                }
                mario->displayDirection = n;
                mario->displayDirectionTarget = n;
            } else {
                f32 t = 270.0f;
                f32 diff = t - mario->displayDirection;
                f32 n = 0.1f * diff;
                n += mario->displayDirection;
                if (__fabsf((f32)((t - n) < 1.0f))) {
                    n = t;
                }
                mario->displayDirection = n;
                mario->displayDirectionTarget = n;
            }
        } else {
            v31 = revise360(mario->directionView - mario->targetYaw);
            if (v31 >= 180.0f) {
                mario->displayDirection = 0.0f;
                mario->displayDirectionTarget = 0.0f;
                mario->moveDirection = 270.0f;
                mario->directionView = 270.0f;
            } else {
                mario->displayDirection = 180.0f;
                mario->displayDirectionTarget = 180.0f;
                mario->moveDirection = 90.0f;
                mario->directionView = 90.0f;
            }
            marioSetSpec();
            mario->rollFlags = 0;
            marioOfsRotReset();
            mario->dispFlags &= ~0x100;
            mario->dispFlags &= ~8;
            mario->flags &= ~MARIO_FLAG_TUBE_MODE;
            marioPaperOff();
            marioAdjustMoveDir();
            marioChgPose("M_S_1");
            if (marioGetPtr()->motStruct) {
                void* z;
                __memFree(HEAP_DEFAULT, marioGetPtr()->motStruct);
                z = 0;
                marioGetPtr()->motStruct = z;
            }
            allPartyRideOff();
            CAMF() &= ~0x200;
            mario->currSubMotionId = 0x16;
        }
        break;
    case 0x16:
        marioChgMotSub(MARIO_MOTION_STAY, 0);
        break;
    }
    if (mario->currSubMotionId < 0x1e) {
        return;
    }
    if (mario->flags & MARIO_FLAG_IS_STEPPING) {
        roll_upstairs();
        return;
    }
    if (mario->buttonTrg & PAD_BUTTON_B) {
        if (!rollChk()) {
            mario->currSubMotionId = 0x14;
            CAMF() |= 0x200;
        } else {
            mario->buttonTrg |= 0x100;
        }
    }
    v26 = mario->tubeScaleY;
    if (RW()->squashTrigger) {
        s32 z = 0;
        s32 one;
        RW()->squashTrigger = z;
        one = 1;
        RW()->squashState = one;
        v26 = 1.0f;
    }
    switch (RW()->squashState) {
    case 1:
        if (v26 < 1.4f) {
            { f32 k = 1.0f; v26 = k * (1.4499999f - v26) + v26; }
            if (v26 >= 1.4f) {
                v26 = 1.4f;
            }
        }
        if (mario->airTimer == 0) {
            s32 t = 2;
            RW()->squashState = t;
        }
        break;
    case 2:
        if (v26 > 0.4f) {
            v26 = 0.4f * (0.35f - v26) + v26;
            if (v26 <= 0.4f) {
                s32 t = 3;
                v26 = 0.4f;
                RW()->squashState = t;
            }
        }
        break;
    case 3:
        if (v26 < 1.0f) {
            v26 = 0.5f * (1.05f - v26) + v26;
            if (v26 >= 1.0f) {
                s32 t = 0;
                RW()->squashState = t;
                v26 = 1.0f;
            }
        }
        break;
    }
    mario->tubeScaleY = v26;
    if (!mario->hitobjStandOn && mario->hitobjFront && !marioChkBero(mario->hitobjFront)) {
        v26 = RW()->spdX;
        v27 = RW()->spdZ;
        v26 += 0.2f * -v26;
        v27 += 0.2f * -v27;
        if (__fabsf(v26) < 1.0f) {
            v26 = 0.0f;
        }
        if (__fabsf(v27) < 1.0f) {
            v27 = 0.0f;
        }
        RW()->spdX = v26;
        RW()->spdZ = v27;
    }
    if (mario->hitobjStandOn) {
        if (!(hitGetAttr(mario->hitobjStandOn) & 0xa00) && mario->airTimer == 0 && (mario->buttonTrg & PAD_BUTTON_A)) {
            if (!(strcmp(gp->currentMapName, "eki_04") == 0 && __fabsf(mario->groundSlope) >= 40.0f)) {
                MarioWork* m = marioGetPtr();
                camFollowYOff();
                m->jumpVelocityY = 2.85f;
                m->jumpAccelerationY = -0.14f;
                m->jumpJerkY = 0.08f;
                m->jumpSnapY = -0.02f;
                m->flags |= MARIO_FLAG_IS_JUMPING;
                m->airTimer = 1;
                psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MARIO_ROLL_JUMP1), &marioGetPtr()->position);
                mario->hitobjJumpFrom = mario->hitobjStandOn;
                mario->hitobjStandOn = 0;
                mario->fallStartPos = mario->position;
                mario->lastGroundPos = mario->position;
                camFollowYOff();
            }
        }
        marioUpdateCamCollision(mario->position);
        mario->airTimer = 0;
    }
    if (mario->flags & (MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING)) {
        if (mario->hitobjHead) {
            if (mario->jumpVelocityY >= 0.0f) {
                mario->jumpVelocityY = -0.03f;
                mario->jumpAccelerationY = -0.02f;
                mario->jumpJerkY = -0.02f;
                mario->jumpSnapY = -0.002f;
            }
            mario->flags |= MARIO_FLAG_IS_FALLING;
        }
        if (mario->airTimer == 0 && mario->jumpVelocityY < 0.0f) {
            mario->fallStartPos = mario->position;
            camFollowYOn();
        }
        if (mario->position.y < mario->lastGroundPos.y) {
            camFollowYOn();
        }
        mario->airTimer++;
    }
    v29 = RW()->spdX;
    v28 = RW()->spdZ;
    nx = 0.0f;
    nz = nx;
    if (!(mario->flags & (MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING))) {
        f32 a26, a30;
        a26 = (f32)(s32)(s8)mario->wStickDir1;
        a30 = (f32)(s32)(s8)mario->wStickDir2;
        nz = distABf(nx, nx, a26, a30);
        v31 = angleABf(0.0f, 0.0f, a26, a30);
        v31 = revise360(mario->targetYaw + v31);
        v26 = 3.1415927f * v31 / 180.0f;
        nx = nz * (f32)sin(v26);
        nz = nz * -(f32)cos(v26);
        v31 = angleABf(0.0f, 0.0f, nx, nz);
        // FAKE: no-op (MarioWork*) cast on the directionView/groundRot reads below (were raw MWOFF offsets); the plain field reorders a const load
        tcA = ((MarioWork*)mario)->directionView;
        if (angOut(tcA, 135.0f, v31) == 1) {
            nz = distABf(0.0f, 0.0f, nx, nz);
            nx = -nz * (f32)sin(3.1415927f * mario->directionView / 180.0f);
            nz = nz * (f32)cos(3.1415927f * ((MarioWork*)mario)->directionView / 180.0f);
        }
    }
    if (!(mario->flags & (MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING)) && !(mario->mot.roll.flags & 0x20)) {
        v31 = angleABf(0.0f, 0.0f, v29, -v28);
        if (((MarioWork*)mario)->groundRot.z > 0.05f) {
            v26 = (v31 > 180.0f) ? rollData[5] : rollData[6];
            {
                f32 t = (f32)sin(3.1415927f * ((MarioWork*)mario)->groundRot.z / 180.0f);
                t = t * v26;
                nx = nx - t;
            }
            if (nx <= -100.0f) {
                nx = -100.0f;
            }
        } else if (mario->groundRot.z < -0.05f) {
            v26 = (v31 < 180.0f) ? rollData[5] : rollData[6];
            {
                f32 t = (f32)sin(3.1415927f * mario->groundRot.z / 180.0f);
                t = t * v26;
                nx = nx - t;
            }
            if (nx >= 100.0f) {
                nx = 100.0f;
            }
        }
        if (((MarioWork*)mario)->groundRot.x > 0.05f) {
            v26 = (v31 > 180.0f) ? rollData[5] : rollData[6];
            {
                f32 t = (f32)sin(3.1415927f * ((MarioWork*)mario)->groundRot.x / 180.0f);
                t = t * v26;
                nz = nz - t;
            }
            if (nz <= -100.0f) {
                nz = -100.0f;
            }
        } else if (mario->groundRot.x < -0.05f) {
            v26 = (v31 < 180.0f) ? rollData[5] : rollData[6];
            {
                f32 t = (f32)sin(3.1415927f * mario->groundRot.x / 180.0f);
                t = t * v26;
                nz = nz - t;
            }
            if (nz >= 100.0f) {
                nz = 100.0f;
            }
        }
        v31 = angleABf(0.0f, 0.0f, nx, nz);
        // FAKE: no-op (MarioWork*) cast on the directionView/groundRot reads below (were raw MWOFF offsets); the plain field reorders a const load
        tcA = ((MarioWork*)mario)->directionView;
        if (angOut(tcA, 135.0f, v31) == 1) {
            nz = distABf(0.0f, 0.0f, nx, -nz);
            nx = -nz * (f32)sin(3.1415927f * mario->directionView / 180.0f);
            nz = nz * (f32)cos(3.1415927f * ((MarioWork*)mario)->directionView / 180.0f);
        }
    }
    if (mario->targetNpc) {
        if (!marioChkBero(mario->hitobjFront)) {
            v29 = 0.05f * -v29 + v29;
            v28 = 0.05f * -v28 + v28;
            if (__fabsf(v29) < 0.1f) {
                v29 = 0.0f;
            }
            if (__fabsf(v28) < 0.1f) {
                v28 = 0.0f;
            }
        }
    }
    if (mario->hitobjFront && !mario->wStickDir1 && !mario->wStickDir2 && !marioChkBero(mario->hitobjFront)) {
        v29 = 0.6f * -v29 + v29;
        v28 = 0.6f * -v28 + v28;
        if (__fabsf(v29) < 0.1f) {
            v29 = 0.0f;
        }
        if (__fabsf(v28) < 0.1f) {
            v28 = 0.0f;
        }
    }
    if (mario->forceMoveTimer == 0 && !nx && !nz) {
        if (mario->hitobjWall) {
            if (!marioChkBero(mario->hitobjWall)) {
                f32 k = rollData[2];
                v29 = v29 * k + v29;
                v28 = v28 * k + v28;
            }
        } else if (mario->hitobjStandOn && mario->airTimer == 0) {
            if (__fabsf((f32)(mario->groundSlope <= 1.0f))) {
                f32 k = rollData[1];
                v29 = v29 * k + v29;
                v28 = v28 * k + v28;
            } else {
                f32 k = rollData[1];
                f32 a = v29 * k;
                f32 b = v28 * k;
                v29 = 6.4f * a + v29;
                v28 = 6.4f * b + v28;
            }
        }
        {
            f32 l = rollData[3];
            if (__fabsf(v29) < l) {
                v29 = 0.0f;
            }
        }
        {
            f32 l = rollData[3];
            if (__fabsf(v28) < l) {
                v28 = 0.0f;
            }
        }
    } else {  // correctness: target runs the speed clamp only on this path (first branch jumps past it)
        if ((mario->mot.roll.flags & 0x10) && mario->groundRot.z == 0.0f) {
            if (makeVec(135.0f) == 1) {
                v29 = 0.2f * -v29 + v29;
                v28 = 0.2f * -v28 + v28;
                if (__fabsf(v29) < 1.0f) {
                    v29 = 0.0f;
                }
                if (__fabsf(v28) < 1.0f) {
                    v28 = 0.0f;
                }
                if (v29 == 0.0f && v28 == 0.0f) {
                    mario->moveDirection = revise360(180.0f + mario->moveDirection);
                    mario->directionView = mario->moveDirection;
                }
            }
        }
        a31 = rollData[0] / 0.05f;
        v26 = 1.0f;
        if (mario->forceMoveTimer == 0 && __fabsf(mario->groundSlope) >= v26) {
            f32 ang = angleABf(0.0f, 0.0f, v29, -v28);
            f32 sp = mario->groundRot.z;
            if (sp > 0.0f) {
                if (ang > 180.0f) {
                    v26 = rollData[9];
                } else {
                    v26 = 1.0f;
                }
            } else if (sp < 0.0f) {
                if (ang < 180.0f) {
                    v26 = rollData[9];
                } else {
                    v26 = 1.0f;
                }
            }
        }
        a31 = a31 * v26;
        v26 = v28 * v28;
        if (v29 * v29 + v26 <= a31 * a31) {
            v29 = nx * k025() + v29;
        } else {
            f32 n = -a31;
            v29 = nx * k025() + v29;
            if (v29 < n) {
                v29 = n;
            }
            if (v29 > a31) {
                v29 = a31;
            }
        }
        b31 = rollData[0] / 0.05f;
        v30 = 1.0f;
        if (mario->forceMoveTimer == 0 && __fabsf(mario->groundSlope) >= v30) {
            f32 ang = angleABf(0.0f, 0.0f, v29, -v28);
            f32 sp = mario->groundRot.x;
            if (sp > 0.0f) {
                if (ang > 180.0f) {
                    v30 = rollData[9];
                } else {
                    v30 = 1.0f;
                }
            } else if (sp < 0.0f) {
                if (ang < 180.0f) {
                    v30 = rollData[9];
                } else {
                    v30 = 1.0f;
                }
            }
        }
        b31 = b31 * v30;
        if (v29 * v29 + v26 <= b31 * b31) {
            v28 = v28 - nz * k025();
        } else {
            f32 n = -b31;
            v28 = v28 - nz * k025();
            if (v28 < n) {
                v28 = n;
            }
            if (v28 > b31) {
                v28 = b31;
            }
        }
    }
    RW()->spdX = v29;
    RW()->spdZ = v28;
    v31 = -v28;
    v30 = angleABf(0.0f, 0.0f, v29, v31);
    v27 = 3.1415927f * v30 / 180.0f;
    v26 = rollCos(v27);
    v27 = distABf(0.0f, 0.0f, v29 * (f32)sin(v27), v28 * v26);
    s = 0.05f * v27;
    if (!getRollEvtFlag() && mario->forceMoveTimer == 0) {
        v30 = angleABf(0.0f, 0.0f, v29, v31);
        if (!(mario->mot.roll.flags & 0x10000)) {
            s = s * mario->playerGravity;
            if (s < 0.05f) {
                s = 0.0f;
            }
            mario->baseSpeed = s;
            if (s) {
                mario->directionView = v30;
            }
        }
    }
    if (mario->baseSpeed) {
        MarioWork* m = marioGetPtr();
#ifdef VERSION_G8ME01
        if (marioGetPtr()->motStruct && (u32)RW()->sfxId == (u32)-1) {
            RW()->sfxId = (s32)psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MARIO_ROLL_MOVE1), &m->position);
            m->mot.roll.flags |= 0x1000;
        }
#else
        if ((u32)m->mot.roll.sfxId == (u32)-1) {
            m->mot.roll.sfxId = (s32)psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MARIO_ROLL_MOVE1), &m->position);
            m->mot.roll.flags |= 0x1000;
        }
#endif
    } else {
        MarioWork* m = marioGetPtr();
#ifdef VERSION_G8ME01
        if (marioGetPtr()->motStruct && (u32)RW()->sfxId != (u32)-1) {
            psndSFXOff(RW()->sfxId);
            RW()->sfxId = -1;
        }
#else
        if ((u32)m->mot.roll.sfxId != (u32)-1) {
            psndSFXOff(m->mot.roll.sfxId);
            m->mot.roll.sfxId = -1;
        }
#endif
    }
    rd = rollData[4];
    {
        s32 flip;
        f32 sgn;
        f32 mv = toMovedir(mario->displayDirection);
        f32 pre;
        // FAKE: no-op (MarioWork*) cast on the directionView/groundRot reads below (were raw MWOFF offsets); the plain field reorders a const load
        tcA = ((MarioWork*)mario)->directionView;
        flip = angOut(tcA, 90.0f, mv);
        pre = mario->baseSpeed * rd;
        sgn = (flip == 1) ? -1.0f : 1.0f;
        mario->rot.z = revise360(pre * sgn + mario->rot.z);
    }
    if (mario->forceMoveTimer == 0 && !getRollEvtFlag() && s) {
        v31 = mario->moveDirection;
        {
            MarioWork* m2 = marioGetPtr();
            DISPDIR_BODY2(v30, v31, m2)
        }
        ((MarioWork*)mario)->directionView = v31;
        mario->moveDirection = v31;
    }
    if (mario->groundRot.z != 0.0f) {
        mario->rot.x = revise360(-mario->groundRot.z * (f32)sin(3.1415927f * mario->displayDirection / 180.0f));
    } else {
        mario->rot.x = 0.0f;
    }
    if (mario->wStickDir1 || mario->wStickDir2) {
        mario->mot.roll.flags |= 0x4000;
    }
    if (mario->baseSpeed) {
        MarioWork* m = marioGetPtr();
        m->camShiftTargetZ = 0.0f;
        m->camShiftRate.z = 0.004f;
    }
    marioRollCan();
}

void roll_upstairs_on(void) {
    MarioWork* mario = marioGetPtr();
    mario->flags |= MARIO_FLAG_IS_STEPPING;
    mario->flags |= MARIO_FLAG_STOP_AIR_MOVEMENT;
    marioClearJumpPara();
    mario->multiTimer = 3;
    mario->jumpVelocityY = (mario->stepUpY - mario->position.y) / (f32)(s32)mario->multiTimer;
    mario->jumpVelocityY = 4.0f;
    mario->jumpAccelerationY = -0.42f;
    mario->jumpJerkY = 0.04f;
    mario->jumpSnapY = -0.01f;
}

#pragma dont_inline on
void roll_upstairs(void) {
    MarioWork* mario = marioGetPtr();
    mario->position.y = mario->position.y + mario->jumpVelocityY;
    if ((s32)--mario->multiTimer <= 0) {
        mario->flags &= ~MARIO_FLAG_STOP_AIR_MOVEMENT;
        mario->flags &= ~MARIO_FLAG_USE_FORCE_DIRECTION;
        mario->flags &= ~MARIO_FLAG_IS_STEPPING;
    }
}

#pragma dont_inline reset

void mot_fall_roll(void) {
    MarioWork* mario = marioGetPtr();
    f32 spd = marioGetFallSpd();
    f32 y;
    HitEntry* h;
    if (spd < -10.0f) {
        spd = -10.0f;
    }
    h = marioChkLandon(&y, spd);
    if (h) {
        mario->position.y = y;
        mario->hitobjStandOn = h;
        mario->hitobjJumpFrom = 0;
    } else {
        mario->position.y += spd;
    }
    if (mario->hitobjStandOn) {
        marioLandOn();
        roll_flyout_flag2 = 0;
    }
}

static inline void marioRollCan(void) {
    MarioWork* mario = marioGetPtr();
    f32 v26;
    {
        f32 s;
        f32 c;
        f32 dist;
        Vec pos;
        Vec dir;
        Vec out;
        Vec nrm;
        Vec d2;
        dist = 100.0f;
        pos = mario->position;
        pos.y = pos.y + mario->wPlayerCollisionBox.y;
        dir = (Vec){0.0f, -1.0f, 0.0f};
        if (marioHitCheckVec(&pos, &dir, &out, &nrm, &dist)) {
            f32 sq = sqrtf(nrm.x * nrm.x + nrm.z * nrm.z);
            f32 ang = angleABf(0.0f, 0.0f, sq, -nrm.y);
            f32 r = (f32)(s32)(0.5f + ang);
            if (rabs(ang) < 45.0) {
                v26 = rollRad(r);
                s = rSin(v26);
                c = rollCos(v26);
                {
                    f32 t1 = s * 5.0f;
                    f32 t2 = 5.0f * c;
                    mario->dispOfs.x = 6.0f + t1;
                    mario->dispOfs.y = -4.0f + t2;
                }
            } else {
                sincosf(&s, &c, 90.0f);
                dist = 100.0f;
                pos = mario->position;
                d2 = (Vec){0.0f, 0.0f, 0.0f};
                d2.x = s;
                d2.z = c;
                dir = d2;
                marioHitCheckVec(&pos, &dir, &out, &nrm, &dist);
            }
        }
    }
}

void marioRollStrippedPool(void) {
    MarioWork* mario = marioGetPtr();
    mario->dispOfs.x = 100.0f;
    mario->dispOfs.y = 0.0f;
    mario->dispOfs.z = 0.5f;
    mario->dispOfs2.x = 3.1415927f;
    mario->dispOfs2.y = 180.0f;
    mario->dispOfs2.z = 6.0f;
    mario->rotOfs.x = 5.0f;
    mario->rotOfs.y = -4.0f;
    mario->rotOfs.z = 90.0f;
    mario->rot.x = 270.0f;
    mario->rot.y = 360.0f;
    mario->rot.z = 10.0f;
    mario->scale.x = 135.0f;
    mario->scale.y = 1.2f;
    mario->scale.z = sqrtf(mario->fallStartPos.x);
    mario->fallStartPos.y = __fabsf(mario->fallStartPos.z) < 45.0;
}

// prototypes for asm stopgap forward references
void mot_fall_roll(void);
void roll_upstairs(void);
void mot_roll(void);
void marioReInit_roll(void);
void rollEvtJumpSetup(void);
void marioSetRollDispDir(void);
void marioRollForceCancel(void);
void mot_roll_post(void);
s32 marioRollCancel(void);
s32 marioRollTryCancel(void);
void marioForceRollAnime(void);
void marioResetRollSpd(void);
s32 getRollEvtFlag(void);
void clrRollEvtFlag(void);
void setRollEvtFlag(void);
s32 marioRollKeyDisable(void);
s32 marioRollCancelDisable(void);
void marioRollFixOn(void);
