// Mario's paper slit motion (side-step through bars): turning sideways, sliding along walls and turning back.

#include "version.h"
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

// MarioWork.slitState: the Paper Mode motion's steps (mot_slit). From READY on Mario is fully paper.
typedef enum SlitState {
    SLIT_STATE_INIT = 0x0,
    SLIT_STATE_CHANGING = 0x2,
    SLIT_STATE_SIDEWAYS = 0x4,
    SLIT_STATE_READY = 0xA,
    SLIT_STATE_MOVE = 0x14,
    SLIT_STATE_MOVE_PM_S_2 = 0x15,
    SLIT_STATE_GAP_ENTER = 0x50,
    SLIT_STATE_GAP_PASS = 0x51,
    SLIT_STATE_GAP_EXIT = 0x52,
    SLIT_STATE_CRACK_1 = 0x5A,
    SLIT_STATE_CRACK_2 = 0x5B,
    SLIT_STATE_CRACK_3 = 0x5C,
    SLIT_STATE_CRACK_4 = 0x5D,
    SLIT_STATE_CRACK_5 = 0x5E,
    SLIT_STATE_END_1 = 0x64,
    SLIT_STATE_END_2 = 0x65,
    SLIT_STATE_END_3 = 0x66,
    SLIT_STATE_SCRIPT_END_1 = 0x6E,
    SLIT_STATE_SCRIPT_END_2 = 0x6F,
    SLIT_STATE_SCRIPT_END_3 = 0x70,
    SLIT_STATE_LECTURE_END_1 = 0x78,
    SLIT_STATE_LECTURE_END_2 = 0x79,
    SLIT_STATE_LECTURE_END_3 = 0x7A,
} SlitState;

#if VERSION <= VERSION_DEMO_JP
#define SLIT_FLAG_RESUME 0x10 // mot.slit.flags: resumeState holds the state motSlitContinue2 returns to
#endif


BOOL marioCreviceWallChk(const Vec* p);

int __abs(int);


// Descriptive name (not in the symbol maps); placeholder unk_800a4e40, JP 0x800A4E40. See docs/NAME_PROVENANCE.md.
#if VERSION >= VERSION_JP
/**
 * Puts Mario straight back into Paper Mode's sideways standing state (slit state 10) after landing from a jump
 * or fall while still paper; called from the landing code in mario_motion.c. JP and later.
 */
void marioSlitReenter(void) {
    MarioWork* mario = marioGetPtr();
    marioChgMot2(MARIO_MOTION_SLIT);
    mario->slitState = SLIT_STATE_READY;
    mario->flags &= ~(MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING | MARIO_FLAG_IS_STEPPING | MARIO_FLAG_HAS_INPUT_JUMP);
}
#endif

// Descriptive name (not in the symbol maps).
#if VERSION >= VERSION_JP
/**
 * Returns Mario to the slit motion after landing while moving in Paper Mode, keeping its current state.
 * JP and later.
 */
void marioSlitSetAfterLand(void) {
    MarioWork* mario = marioGetPtr();
    marioChgMot2(MARIO_MOTION_SLIT);
    mario->flags &= ~(MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING | MARIO_FLAG_IS_STEPPING | MARIO_FLAG_HAS_INPUT_JUMP);
}
#endif

/**
 * TRUE when Mario may turn into Paper Mode: he has the paper curse (ITEM_PAPER_CURSE), it is not disabled,
 * he is not in 8-bit form, and the map is not in background mode. Checked by the stay and walk motions.
 */
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

/**
 * Paper Mode button state: 1 when R is held while not paper (turn sideways),
 * 2 while already paper and R is still held (or the analog R trigger reads 50 or more),
 * 0 otherwise or while control is off, in background mode, or (JP and later) a partner action is starting.
 */
s32 marioSlitButton(void) {
    MarioWork* mario = marioGetPtr();
#if VERSION >= VERSION_JP
    if (mario->trigFlags & MARIO_TRIG_FLAG_USE_PARTNER) {
        return 0;
    }
#endif
    if (marioKeyOffChk()) {
        return 0;
    }
    if (marioBgmodeChk() == 1) {
        return 0;
    }
#if VERSION <= VERSION_DEMO_JP
    if (!(mario->flags & MARIO_FLAG_PAPER_MODE)) {
#else
    if (!(mario->dispFlags & MARIO_DISP_FLAG_SLIT)) {
#endif
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

/**
 * FALSE while Mario is slipping through a gap or dropping through a floor crack in Paper Mode (slit states 0x50-0x52,
 * 0x5A-0x5E), TRUE otherwise; marioChkKey treats FALSE as no control.
 */
BOOL marioSlitKeyDisable(void) {
    MarioWork* mario = marioGetPtr();
#if VERSION <= VERSION_DEMO_JP
    s32 st;
#else
    s16 st;
#endif
    if (!(mario->flags & MARIO_FLAG_PAPER_MODE)) {
        return TRUE;
    }
    st = mario->slitState;
    if (st >= SLIT_STATE_GAP_ENTER && st <= SLIT_STATE_GAP_EXIT) {
        return FALSE;
    }
    if (st >= SLIT_STATE_CRACK_1 && st <= SLIT_STATE_CRACK_5) {
        return FALSE;
    }
    return TRUE;
}

/** Eases the camera's forward shift back to 0 (rate 0.004). */
// Original name from the JP demo's link map (UNUSED there): every call is inlined, so the linker strips it.
static void zoomOff(void) {
    MarioWork* mario = marioGetPtr();
    mario->camShiftTargetZ = 0.0f;
    mario->camShiftRate.z = 0.004f;
}

/**
 * Walking speed in Paper Mode or background mode:
 * in Paper Mode half the walk speed when the stick is only tilted a little and mario->mot.slit.timer is 0;
 * in background mode always half.
 */
// Original name from the JP demo's link map (UNUSED there): every call is inlined, so the linker strips it.
static f32 _getWalkSpd(void) {
    MarioWork* mario = marioGetPtr();
    f32 spd = mario->baseWalkSpeed;
    int a;
    int b;
    if (mario->flags & MARIO_FLAG_PAPER_MODE) {
        if (mario->mot.slit.timer != 0) {
            return spd;
        }
        a = mario->wStickDir1;
        b = mario->wStickDir2;
        if (a * a + b * b <= (55 * 55)) {
            spd *= 0.5f;
        }

    } else if (mario->flags & MARIO_FLAG_BG_MODE) {
        spd *= 0.5f;
    }
    return spd;
}

#if VERSION >= VERSION_JP
/**
 * TRUE when Mario must stay in Paper Mode: R is held, walls are too close around him,
 * or he stands in a narrow crack (marioCreviceWallChk). The item menu refuses items then. JP and later.
 */
BOOL marioSlitContinueChk(void) {
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
#endif

/**
 * TRUE when position p is inside a narrow crack:
 * rays 20 units to both sides (each tried from the centre and from points 10 units ahead and behind) hit walls
 * whose two normal angles average to within 10 degrees of 180.
 */
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

/**
 * Forgets the floor position Mario last stood on in Paper Mode (mario->slitPosSet),
 * for example when he takes damage.
 */
void marioClearSlitFloor(void) {
    MarioWork* mario = marioGetPtr();
    mario->slitPosSet = 0;
}

/** Saves Mario's position as the Paper Mode floor position (mario->slitPos) the partners follow. */
// Original name from the JP demo's link map (UNUSED there): every call is inlined, so the linker strips it.
void marioSetSlitFloorPos(void) {
    MarioWork* mario = marioGetPtr();
    mario->slitPosSet = 1;
    mario->slitPos = mario->position;
}

/**
 * Copies the floor position saved in Paper Mode to *out and returns TRUE, or FALSE if none is saved;
 * the partners use it to follow Mario through gaps.
 */
BOOL marioGetSlitFloorPos(Vec* out) {
    MarioWork* mario = marioGetPtr();
    if (mario->slitPosSet) {
        *out = mario->slitPos;
        return TRUE;
    }
    return FALSE;
}

/**
 * TRUE while Mario is passing through a gap in Paper Mode (slit states 0x50-0x5E);
 * the partners check it.
 */
BOOL marioChkSlitThrouh(void) {
    MarioWork* mario = marioGetPtr();
#if VERSION <= VERSION_DEMO_JP
    s32 st;
#else
    s16 st;
#endif
    if (mario->currMotionId != MARIO_MOTION_SLIT) {
        return FALSE;
    }
    st = mario->slitState;
    if (st >= SLIT_STATE_GAP_ENTER && st <= SLIT_STATE_CRACK_5) {
        return TRUE;
    }
    return FALSE;
}

/** TRUE on the last frame of a pass through a gap (slit state 0x52 or 0x5E). */
BOOL marioChkSlitThrouhEnd(void) {
    MarioWork* mario = marioGetPtr();
#if VERSION <= VERSION_DEMO_JP
    s32 st;
#else
    s16 st;
#endif
    if (mario->currMotionId != MARIO_MOTION_SLIT) {
        return FALSE;
    }
    st = mario->slitState;
    if (st == SLIT_STATE_GAP_EXIT || st == SLIT_STATE_CRACK_5) {
        return TRUE;
    }
    return FALSE;
}

/**
 * Switches back to the slit motion in its moving state (0x14), from the stay,
 * walk and step-up motions when Mario is still paper.
 */
void motSlitContinue(void) {
    marioChgMot2(MARIO_MOTION_SLIT);
    marioGetPtr()->slitState = SLIT_STATE_MOVE;
}

#if VERSION <= VERSION_DEMO_JP
/**
 * The early demos only: returns to the slit motion at the state saved
 * before a step-up (or ends it, state 0x64, if none was saved).
 */
void motSlitContinue2(void) {
    MarioWork* mario = marioGetPtr();
    marioChgMot2(MARIO_MOTION_SLIT);
    if (!(mario->mot.slit.flags & SLIT_FLAG_RESUME)) {
        mario->slitState = SLIT_STATE_END_1;
    } else {
        mario->slitState = mario->mot.slit.resumeState;
    }
}
#endif
/** TRUE once the slit motion has reached its ending states (0x64 and later). */
BOOL marioChkSlitEnd(void) {
    MarioWork* mario = marioGetPtr();
    if (mario->currMotionId != MARIO_MOTION_SLIT) {
        return FALSE;
    }
    return mario->slitState >= SLIT_STATE_END_1;
}

/** Starts turning Mario back from Paper Mode (state 0x64) if he is paper. */
void motSlitCancel(void) {
    MarioWork* mario = marioGetPtr();
    if (mario->flags & MARIO_FLAG_PAPER_MODE) {
        mario->slitState = SLIT_STATE_END_1;
    }
}

/**
 * Turns Mario back from Paper Mode for a script (state 0x6E),
 * first pulling the partners out of their paper form (JP and later); called from evt_mario.
 */
void motSlitCancel2(void) {
    MarioWork* mario = marioGetPtr();
    if (mario->flags & MARIO_FLAG_PAPER_MODE) {
#if VERSION >= VERSION_JP
        allPartyForceSlitOff();
#endif
        mario->slitState = SLIT_STATE_SCRIPT_END_1;
    }
}

/**
 * Turns Mario back from Paper Mode with the 0x78 ending,
 * used by the paper genie lecture scripts (evt_lect_*) and yuu_03 before they take control.
 */
void motSlitCancel3(void) {
    MarioWork* mario = marioGetPtr();
    if (mario->flags & MARIO_FLAG_PAPER_MODE) {
        mario->slitState = SLIT_STATE_LECTURE_END_1;
    }
}

/**
 * TRUE while either script-driven Paper Mode ending (0x6E-0x70 or 0x78-0x7A) is playing;
 * evt_mario and mario_sbr wait on it.
 */
BOOL marioGetMotSlitCancel3(void) {
#if VERSION <= VERSION_DEMO_JP
    s32 st;
#else
    s16 st;
#endif
    BOOL r = FALSE;
    MarioWork* mario = marioGetPtr();
    if (mario->currMotionId == MARIO_MOTION_SLIT) {
        st = mario->slitState;
        if ((st >= SLIT_STATE_LECTURE_END_1 && st <= SLIT_STATE_LECTURE_END_3) || (st >= SLIT_STATE_SCRIPT_END_1 && st <= SLIT_STATE_SCRIPT_END_3)) {
            r = TRUE;
        }
    }
    return r;
}

static inline void slitClearFlags(MarioWork* m) {
    m->flags &= ~MARIO_FLAG_PARTY_WALK_DISABLED;
    m->flags &= ~MARIO_FLAG_GONE_THROUGH_BARS;
    m->dispFlags &= ~(MARIO_DISP_FLAG_LEFT_ONLY | MARIO_DISP_FLAG_RIGHT_ONLY | MARIO_DISP_FLAG_SLIT);
    m->dispFlags &= ~(MARIO_DISP_FLAG_LEFT_ONLY | MARIO_DISP_FLAG_RIGHT_ONLY | MARIO_DISP_FLAG_SLIT); // the game clears these bits twice (both stores are in the retail code)
}

/**
 * Instantly turns Mario back from Paper Mode with no animation:
 * faces him left or right, clears the paper flags, sets his collision width (20 in the early demos, 10 in JP and later),
 * turns the paper look and map paper tint off, recentres the camera and returns the partners to normal.
 * Used before damage, pipes and other forced motions.
 */
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
        m->dispFlags &= ~(MARIO_DISP_FLAG_LEFT_ONLY | MARIO_DISP_FLAG_RIGHT_ONLY | MARIO_DISP_FLAG_SLIT);
        m->flags &= ~MARIO_FLAG_PARTY_WALK_DISABLED;
        m->flags &= ~MARIO_FLAG_GONE_THROUGH_BARS;
#if VERSION <= VERSION_DEMO_JP
        m->wPlayerCollisionBox.x = 20.0f;
        m->dispFlags &= ~(MARIO_DISP_FLAG_LEFT_ONLY | MARIO_DISP_FLAG_RIGHT_ONLY | MARIO_DISP_FLAG_SLIT);
#else
        m->wPlayerCollisionBox.x = 10.0f;
        m->dispFlags &= ~(MARIO_DISP_FLAG_LEFT_ONLY | MARIO_DISP_FLAG_RIGHT_ONLY | MARIO_DISP_FLAG_SLIT);
#endif
        marioPaperOff();
        mapResetPaperAmbColor();
        m->dispFlags &= ~MARIO_DISP_FLAG_NO_REAR_POSE;
        marioAdjustMoveDir();
        zoomOff();
        allPartyForceSlitOff();
    }
}

/**
 * Cleanup when the slit motion ends: nothing if the next motion keeps Mario paper (walk, dash, jumps, falls,
 * step-up; in US and later only once the turn into paper has finished); otherwise restores him and the partners
 * like marioSlitForceCancel (in US and later, when switching to Tube Mode the partners turn back by request).
 */
void mot_slit_post(void) {
    MarioWork* mario = marioGetPtr();
#if VERSION <= VERSION_DEMO_JP
    u16 id;
#elif VERSION >= VERSION_US
    u16 id;
    if (mario->slitState >= SLIT_STATE_READY) {
        id = mario->currMotionId;
        if (id == MARIO_MOTION_SLIT || (id == MARIO_MOTION_FALL_2 || id == MARIO_MOTION_UPSTAIRS) || mario->currMotionId <= MARIO_MOTION_JUMP_2 ||
            mario->currMotionId == MARIO_MOTION_JUMP_3) {
            return;
        }
    }
#else
    u16 id = mario->currMotionId;
    if (id == MARIO_MOTION_SLIT || (id == MARIO_MOTION_FALL_2 || id == MARIO_MOTION_UPSTAIRS) || mario->currMotionId <= MARIO_MOTION_JUMP_2 ||
        mario->currMotionId == MARIO_MOTION_JUMP_3) {
        return;
    }
#endif
    if (!(mario->flags & MARIO_FLAG_PAPER_MODE)) {
        if (mario->dispFlags & MARIO_DISP_FLAG_SLIT) {
            mario->wPlayerCollisionBox.x = 20.0f;
            if (mario->directionView >= 180.0f) {
                mario->displayDirection = 0.0f;
                mario->displayDirectionTarget = mario->displayDirection;
            } else {
                mario->displayDirection = 180.0f;
                mario->displayDirectionTarget = mario->displayDirection;
            }
#if VERSION <= VERSION_DEMO_JP
            mario->flags &= ~MARIO_FLAG_PARTY_WALK_DISABLED;
            mario->flags &= ~MARIO_FLAG_GONE_THROUGH_BARS;
            mario->dispFlags &= ~(MARIO_DISP_FLAG_LEFT_ONLY | MARIO_DISP_FLAG_RIGHT_ONLY | MARIO_DISP_FLAG_SLIT);
            mario->dispFlags &= ~(MARIO_DISP_FLAG_LEFT_ONLY | MARIO_DISP_FLAG_RIGHT_ONLY | MARIO_DISP_FLAG_SLIT);
#else
            slitClearFlags(mario);
#endif
            marioPaperOff();
            mapResetPaperAmbColor();
            mario->dispFlags &= ~MARIO_DISP_FLAG_NO_REAR_POSE;
#if VERSION == VERSION_EU
            if (!(mario->flags & MARIO_FLAG_FORCED_MOVEMENT)) {
                marioAdjustMoveDir();
            }
#else
            marioAdjustMoveDir();
#endif
            allPartyForceSlitOff();
        }
#if VERSION <= VERSION_DEMO_JP
    } else {
        id = mario->currMotionId;
        if (id == MARIO_MOTION_WALK || (u16)(id - MARIO_MOTION_DASH) <= MARIO_MOTION_JUMP_2 - MARIO_MOTION_DASH || mario->currMotionId == MARIO_MOTION_JUMP_3) {
            return;
        }
#else
    } else if (mario->currMotionId != MARIO_MOTION_FALL) {
#endif
        mario->flags &= ~MARIO_FLAG_PAPER_MODE;
        mario->wPlayerCollisionBox.x = 20.0f;
        if (mario->directionView >= 180.0f) {
            mario->displayDirection = 0.0f;
            mario->displayDirectionTarget = mario->displayDirection;
        } else {
            mario->displayDirection = 180.0f;
            mario->displayDirectionTarget = mario->displayDirection;
        }
        mario->flags &= ~MARIO_FLAG_PARTY_WALK_DISABLED;
        mario->flags &= ~MARIO_FLAG_GONE_THROUGH_BARS;
        mario->dispFlags &= ~(MARIO_DISP_FLAG_LEFT_ONLY | MARIO_DISP_FLAG_RIGHT_ONLY | MARIO_DISP_FLAG_SLIT);
        mario->dispFlags &= ~(MARIO_DISP_FLAG_LEFT_ONLY | MARIO_DISP_FLAG_RIGHT_ONLY | MARIO_DISP_FLAG_SLIT);
        marioPaperOff();
        mapResetPaperAmbColor();
        mario->dispFlags &= ~MARIO_DISP_FLAG_NO_REAR_POSE;
        marioAdjustMoveDir();
#if VERSION >= VERSION_US
        if (mario->currMotionId != MARIO_MOTION_ROLL) {
            allPartyForceSlitOff();
        } else {
            allPartySlitOffReq();
        }
#else
        allPartyForceSlitOff();
#endif
    }
}

/** Re-applies the Paper Mode animation (p_slit, PM_S_1A) when mario.c rebuilds Mario's look while he is paper. */
void marioForceSlitAnime(void) {
    MarioWork* mario = marioGetPtr();
    marioPaperOff();
    marioChgPaper(NULL);
    marioPaperOn("p_slit");
    marioChgPose("M_S_1");
    marioChgPaper("PM_S_1A");
    mario->dispFlags |= MARIO_DISP_FLAG_NO_REAR_POSE;
}

static inline void slitInit(void) {
    MarioWork* mario = marioGetPtr();
    mario->mot.slit.reserved_0 = 0.0f;
    mario->mot.slit.counter = 0;
    mario->mot.slit.count = 0;
    mario->mot.slit.timer = 0;
    mario->mot.slit.side = 0;
    mario->trigFlags &= ~MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
    mario->flags &= ~(MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING | MARIO_FLAG_IS_STEPPING | MARIO_FLAG_HAS_INPUT_JUMP);
    mario->dispFlags |= MARIO_DISP_FLAG_NO_REAR_POSE;
    mario->dispFlags |= MARIO_DISP_FLAG_SLIT;
    if (mario->paperPoseId == -1) {
        GXColor tmp;
        GXColor col = {0, 0, 0, 255};
        col.r = 0xa8;
        col.g = 0xa8;
        col.b = 0xa8;
        tmp = col;
        mapSetPaperAmbColor(&tmp);
        marioPaperOn("p_slit");
    }
    if (!mario->controlStickSensitivity) {
        marioChgPose("M_S_1");
    }
    marioChgPaper("PM_S_1A");
}

/**
 * Restores Paper Mode when Mario is re-entered on a new map while paper (marioEntry):
 * sideways standing state, 6-unit collision width, partners turned paper too.
 */
void marioReInit_slit(void) {
    MarioWork* mario = marioGetPtr();
    slitInit();
    mario->trigFlags &= ~MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
    mario->currMotionId = MARIO_MOTION_SLIT;
    mario->slitState = SLIT_STATE_READY;
    mario->displayDirection = 270.0f;
    mario->displayDirectionTarget = mario->displayDirection;
    marioChgPose("M_S_1");
    marioChgPaper("PM_S_1B");
    mario->slitState = SLIT_STATE_READY;
    mario->mot.slit.count = 0;
    mario->wPlayerCollisionBox.x = 6.0f;
    mario->flags |= MARIO_FLAG_PAPER_MODE;
    mario->dispFlags |= MARIO_DISP_FLAG_SLIT;
    allPartySlitOn();
}

/**
 * TRUE when Mario should stay paper: the paper button is held (early demos: held while already paper),
 * walls are close around him, or he is in a narrow crack.
 */
static inline BOOL slitContinueChk(void) {
    MarioWork* mario = marioGetPtr();
    Vec pos;
#if VERSION <= VERSION_DEMO_JP
    if (marioSlitButton() == 2) {
#else
    if (marioSlitButton()) {
#endif
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

#if VERSION >= VERSION_US
#define SPEEDCHK_COND(m) ((m)->baseSpeed >= (m)->baseDashSpeed || (m)->controlStickSensitivity)
#else
#define SPEEDCHK_COND(m) ((m)->baseSpeed >= (m)->baseDashSpeed)
#endif

// Back from dashing to walking speed.
static inline void slitSpeedDown(MarioWork* m) {
    m->baseSpeed = m->baseWalkSpeed;
    if (!(m->trigFlags & MARIO_TRIG_FLAG_SLIT_KEEP_POSE)) {
        marioChgPose("M_R_1");
    }
    m->mot.slit.flags |= 1;
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

#if VERSION >= VERSION_US
#define SLITSTOP(m)               \
    if (!marioChkKey()) {         \
        (m)->baseSpeed = 0.0f;    \
        m->slitState = SLIT_STATE_READY;        \
        m->mot.slit.count = 0;    \
        marioChgPose("M_S_1");    \
        marioChgPaper("PM_S_1B"); \
        break;                    \
    }
#define SLITSTOP2(m)              \
    if (!marioChkKey()) {         \
        m->mot.slit.timer = 0;    \
        (m)->baseSpeed = 0.0f;    \
        m->slitState = SLIT_STATE_READY;        \
        m->mot.slit.count = 0;    \
        marioChgPose("M_S_1");    \
        marioChgPaper("PM_S_1B"); \
        break;                    \
    }
#else
#define SLITSTOP(m)
#define SLITSTOP2(m)
#endif

/**
 * Paper Mode motion (paper curse, hold R): Mario turns sideways and thin, walks and runs slowly,
 * slips through narrow gaps and bars (states 0x50-0x52, walls off while he passes),
 * drops through floor cracks (0x5A-0x5E), and turns back when R is released and there is room.
 * Scripts end it with their own animations (0x6E, 0x78).
 */
void mot_slit(void) {
    MarioWork* m = marioGetPtr();
    f32 d;
    s32 st;
    if (!m->controlStickSensitivity) {
        m->trigFlags &= ~MARIO_TRIG_FLAG_SLIT_KEEP_POSE;
    }
    if (m->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) {
        slitInit();
        m->slitState = SLIT_STATE_INIT;
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
        m->dispFlags &= ~(MARIO_DISP_FLAG_LEFT_ONLY | MARIO_DISP_FLAG_RIGHT_ONLY | MARIO_DISP_FLAG_SLIT);
        marioPaperOff();
        m->flags &= ~MARIO_FLAG_PAPER_MODE;
        return;
    }
    switch (m->slitState) {
    case SLIT_STATE_INIT:
        m->slitTimer = 30;
        m->mot.slit.counter = 0;
        m->slitState = SLIT_STATE_CHANGING;
        m->mot.slit.lastY = m->position.y;
        if (SPEEDCHK_COND(m)) {
            slitSpeedDown(m);
        }
        marioGetPtr();
        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MARIO_CHANGE_SLIT1), &m->position);
        break;
    case SLIT_STATE_CHANGING:
#if VERSION == VERSION_EU
        if (m->flags & MARIO_FLAG_FORCED_MOVEMENT) {
            break;
        }
#endif
        if (!m->controlStickSensitivity) {
            m->baseSpeed = 0.0f;
            marioChgPose("M_S_1");
#if VERSION >= VERSION_US
        } else {
            if (SPEEDCHK_COND(m)) {
                slitSpeedDown(m);
            }
            m->directionView = m->controlStickAngle;
#endif
        }
        if (--m->slitTimer <= 0) {
            m->flags |= MARIO_FLAG_PAPER_MODE;
            m->slitState = SLIT_STATE_SIDEWAYS;
            m->wPlayerCollisionBox.x = 6.0f;
        }
        if (m->slitTimer == 0x14) {
            allPartySlitOn();
        }
        break;
    case SLIT_STATE_SIDEWAYS:
        if (marioChkKey()) {
            if (!m->controlStickSensitivity) {
                m->baseSpeed = 0.0f;
                marioChgPose("M_S_1");
            }
            m->directionView = toMovedirSimple(m->displayDirection);
        }
#if VERSION == VERSION_EU
        if (!(m->flags & MARIO_FLAG_FORCED_MOVEMENT) && m->baseSpeed >= m->baseDashSpeed) {
#else
        if (m->baseSpeed >= m->baseDashSpeed) {
#endif
            slitSpeedDown(m);
        }
        m->displayDirection = 270.0f;
        m->displayDirectionTarget = m->displayDirection;
        if (!m->controlStickSensitivity) {
            marioChgPose("M_S_1");
        }
        marioChgPaper("PM_S_1B");
        m->slitState = SLIT_STATE_READY;
        m->mot.slit.count = 0;
    case SLIT_STATE_READY:
        if (m->position.y != m->mot.slit.lastY) {
            zoomOff();
        }
        m->mot.slit.lastY = m->position.y;
        if (m->forceMoveTimer != 0) {
            if (!(m->trigFlags & MARIO_TRIG_FLAG_SLIT_KEEP_POSE)) {
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
        if (m->baseSpeed >= m->baseDashSpeed) {
            slitSpeedDown(m);
        }
        if (marioChkKey()) {
            if (slitContinueChk() != TRUE) {
#if VERSION >= VERSION_JP
                m->directionView = m->controlStickAngle;
#endif
                m->slitState = SLIT_STATE_END_1;
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
            zoomOff();
        } else {
            m->mot.slit.flags &= ~1;
            zoomOff();
        }
        m->baseSpeed = _getWalkSpd();
        m->directionView = m->controlStickAngle;
        if (!(m->trigFlags & MARIO_TRIG_FLAG_SLIT_KEEP_POSE)) {
            marioChgPose("M_W_1");
        }
        d = revise360(m->controlStickAngle - m->targetYaw);
        if (d >= 315.0f || d <= 45.0f || (d >= 135.0f && d <= 225.0f)) {
            marioChgPaper("PM_S_1E");
        } else {
            marioChgPaper("PM_S_1D");
#if VERSION <= VERSION_DEMO_JP
            if (revise360(m->directionView - m->targetYaw) <= 180.0f) {
                m->mot.slit.side = 1;
                m->displayDirection = 260.0f;
            } else {
                m->mot.slit.side = 2;
                m->displayDirection = 280.0f;
            }
#else
            slitSetSide(m);
#endif
        }
        m->slitState = SLIT_STATE_MOVE;
        break;
    }
    switch (m->slitState) {
    case SLIT_STATE_MOVE:
#if VERSION >= VERSION_JP
        if (m->mot.slit.timer > 0) {
            m->mot.slit.timer = 0;
            marioChgPaper("PM_S_1D");
            slitSetDisplayDir(m);
            SLITSTOP(m)
        }
#endif
        if (marioChkKey()) {
            if (slitContinueChk() != TRUE) {
                m->slitState = SLIT_STATE_END_1;
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
                        m->slitState = SLIT_STATE_END_1;
                    }
                    break;
                } else {
                    m->slitState = SLIT_STATE_SIDEWAYS;
                    break;
                }
            } else {
                s32 sum;
                m->mot.slit.count = 0;
#if VERSION <= VERSION_DEMO_JP
                sum = __abs((s8)m->wStickDir1) + __abs((s8)m->wStickDir2);
#else
                sum = __abs(m->wStickDir1) + __abs(m->wStickDir2);
#endif
                if (!(m->trigFlags & MARIO_TRIG_FLAG_SLIT_KEEP_POSE)) {
                    marioChgPose((sum < 0x44) ? "M_W_1" : "M_R_1");
                }
            }
        }
        if (marioChkKey()) {
            m->directionView = m->controlStickAngle;
            m->baseSpeed = _getWalkSpd();
        }
        d = revise360(m->controlStickAngle - m->targetYaw);
        if (d >= 315.0f || d <= 45.0f || (d >= 135.0f && d <= 225.0f)) {
            marioChgPaper("PM_S_1E");
            m->mot.slit.count = 6;
            m->mot.slit.side = 0;
#if VERSION <= VERSION_DEMO_JP
            m->displayDirection = (revise360(m->directionView - m->targetYaw) <= 180.0f) ? 260.0f : 280.0f;
#else
            slitSetDisplayDir(m);
#endif
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
                    m->slitState = SLIT_STATE_MOVE_PM_S_2;
                    m->mot.slit.side = 2;
                    m->baseSpeed = _getWalkSpd();
#if VERSION >= VERSION_JP
                    m->displayDirection = 260.0f;
                    SLITSTOP2(m)
#endif
                }
            } else if (m->mot.slit.side == 2) {
                if (d <= 180.0f) {
                    marioChgPaper("PM_S_2");
                    m->mot.slit.timer = 6;
                    m->slitState = SLIT_STATE_MOVE_PM_S_2;
                    m->mot.slit.side = 1;
                    m->baseSpeed = _getWalkSpd();
#if VERSION >= VERSION_JP
                    m->displayDirection = 280.0f;
                    SLITSTOP2(m)
#endif
                }
            }
        }
        break;
    case SLIT_STATE_MOVE_PM_S_2:
        if (--m->mot.slit.timer > 0) {
#if VERSION >= VERSION_JP
            SLITSTOP2(m)
#endif
            break;
        }
        marioChgPaper("PM_S_1D");
        m->slitState = SLIT_STATE_MOVE;
#if VERSION <= VERSION_DEMO_JP
        m->displayDirection = (revise360(m->directionView - m->targetYaw) <= 180.0f) ? 260.0f : 280.0f;
#else
        m->displayDirection = (m->mot.slit.side == 1) ? 260.0f : 280.0f;
#endif
        break;
    }
    if (m->flags & MARIO_FLAG_PAPER_MODE) {
        if (m->hitobjPush != NULL) {
#if VERSION <= VERSION_DEMO_JP
            if (hitGetAttr(m->hitobjPush) & HIT_ATTR_SLIT) {
                HitCheckQuery q;
                f32 sn, cs;
                d = revise360(m->controlStickAngle - m->targetYaw);
                if (d > 90.0f && d < 270.0f) {
                    m->mot.slit.pushDir = 180.0f;
                } else {
                    m->mot.slit.pushDir = 0.0f;
                }
                if (++m->mot.slit.counter >= sysMsec2Frame(50)) {
                    m->directionView = revise360(m->mot.slit.pushDir + m->targetYaw);
                    sincosf(&sn, &cs, m->directionView);
                    q.targetPos = m->position;
                    q.targetPos.y += 11.0f;
                    {
                        Vec tmp = {0, 0, 0};
                        tmp.x = sn;
                        tmp.z = cs;
                        q.targetDir = tmp;
                    }
                    q.targetDistance = 20.0f;
                    if (hitCheckVecHitObjXZ(&q, m->hitobjPush)) {
                        m->slitState = SLIT_STATE_GAP_ENTER;
                    }
                }
            } else {
                m->mot.slit.counter = 0;
#else
            if (m->hitobjStandOn != NULL && m->currMotionId == MARIO_MOTION_SLIT) {
                if (hitGetAttr(m->hitobjPush) & HIT_ATTR_SLIT) {
                    HitCheckQuery q;

                    // FAKE: qp is a second name for &q; using &q changes the object
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
                        {
                            Vec tmp = {0, 0, 0};
                            q.targetPos.x = m->position.x;
                            q.targetPos.y = 11.0f + m->position.y;
                            q.targetPos.z = m->position.z;
                            tmp.x = sn;
                            tmp.z = cs;
                            q.targetDir = tmp;
                        }
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
                                    m->slitState = SLIT_STATE_GAP_ENTER;
                                }
                            }
                        }
                    }
                } else {
                    m->mot.slit.counter = 0;
                }
#endif
            }
        } else {
            st = m->slitState;
            if (!(st >= SLIT_STATE_CRACK_1 && st <= SLIT_STATE_CRACK_5) && m->hitobjStandOn != NULL && (hitGetAttr(m->hitobjStandOn) & HIT_ATTR_SLIT)) {
                m->mot.slit.counter += 3;
                marioChgPaper("PM_S_1H");
#if VERSION <= VERSION_DEMO_JP
                if (m->mot.slit.counter >= 0xf) {
#else
                if (m->mot.slit.counter >= 0xf && m->currMotionId == MARIO_MOTION_SLIT) {
#endif
                    m->slitState = SLIT_STATE_CRACK_1;
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
    case SLIT_STATE_GAP_ENTER:
        camGetPtr(CAMERA_2D)->flags |= CAMERA_FLAG_LETTER_BOX_OFF;
        zoomOff();
        m->flags |= MARIO_FLAG_CHG_MOT_DISABLED;
        if (0.0f == m->mot.slit.pushDir) {
            marioChgPaper("PM_S_1F");
        } else {
            marioChgPaper("PM_S_1G");
        }
        m->flags |= MARIO_FLAG_WALLS_DISABLED;
        m->slitTimer = 20;
        m->baseSpeed = (2.0f * m->wPlayerCollisionBox.x) / (f32)m->slitTimer;
        m->slitState = SLIT_STATE_GAP_PASS;
        m->flags |= MARIO_FLAG_GONE_THROUGH_BARS;
        break;
    case SLIT_STATE_GAP_PASS:
        if (--m->slitTimer <= 0) {
            m->slitState = SLIT_STATE_GAP_EXIT;
        }
        break;
    case SLIT_STATE_GAP_EXIT:
        camGetPtr(CAMERA_2D)->flags &= ~CAMERA_FLAG_LETTER_BOX_OFF;
        m->flags &= ~MARIO_FLAG_WALLS_DISABLED;
        m->slitState = SLIT_STATE_MOVE;
        m->flags &= ~MARIO_FLAG_CHG_MOT_DISABLED;
        break;
    }
    switch (m->slitState) {
    case SLIT_STATE_CRACK_1:
        camGetPtr(CAMERA_2D)->flags |= CAMERA_FLAG_LETTER_BOX_OFF;
        zoomOff();
        m->flags |= MARIO_FLAG_CHG_MOT_DISABLED;
        marioChgPaper("PM_S_1H");
        m->slitTimer = sysMsec2Frame(500);
        m->baseSpeed = 0.0f;
        m->slitState = SLIT_STATE_CRACK_2;
        marioSetSlitFloorPos();
    case SLIT_STATE_CRACK_2:
#if VERSION <= VERSION_DEMO_JP
        m->slitTimer = 20;
#else
        m->slitTimer = 15;
#endif
        m->slitState = SLIT_STATE_CRACK_3;
        m->flags |= MARIO_FLAG_FLOORS_DISABLED;
        marioSetFallPara();
        m->flags &= ~MARIO_FLAG_IS_JUMPING;
        m->flags |= (MARIO_FLAG_IS_FALLING | MARIO_FLAG_HAS_INPUT_JUMP);
    case SLIT_STATE_CRACK_3:
#if VERSION <= VERSION_DEMO_JP
        m->displayDirection = (revise360(m->directionView - m->targetYaw) <= 180.0f) ? 260.0f : 280.0f;
#else
        slitSetDisplayDir(m);
#endif
        if (--m->slitTimer > 0) {
            break;
        }
        m->slitState = SLIT_STATE_CRACK_4;
        m->flags &= ~MARIO_FLAG_FLOORS_DISABLED;
#if VERSION >= VERSION_JP
        m->flags &= ~MARIO_FLAG_CHG_MOT_DISABLED;
#endif
    case SLIT_STATE_CRACK_4:
        if (m->hitobjStandOn == NULL) {
            break;
        }
        m->flags &= ~(MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING | MARIO_FLAG_IS_STEPPING | MARIO_FLAG_HAS_INPUT_JUMP);
        m->slitState = SLIT_STATE_CRACK_5;
    case SLIT_STATE_CRACK_5:
        camGetPtr(CAMERA_2D)->flags &= ~CAMERA_FLAG_LETTER_BOX_OFF;
        m->slitState = SLIT_STATE_MOVE;
#if VERSION <= VERSION_DEMO_JP
        m->flags &= ~MARIO_FLAG_CHG_MOT_DISABLED;
#endif
        break;
    }
    switch (m->slitState) {
    case SLIT_STATE_END_1:
        m->flags &= ~MARIO_FLAG_GONE_THROUGH_BARS;
#if VERSION <= VERSION_DEMO_JP
        m->mot.slit.resumeState = m->slitState;
        m->mot.slit.flags |= SLIT_FLAG_RESUME;
#endif
        m->slitTimer2 = 0;
        m->flags &= ~MARIO_FLAG_PAPER_MODE;
        m->wPlayerCollisionBox.x = 20.0f;
#if VERSION >= VERSION_US
        if (m->controlStickSensitivity) {
            slitSpeedDown(m);
        }
#endif
        if (m->baseSpeed == 0.0f) {
            marioChgPose("M_S_1");
        }
        marioChgPaper("PM_S_1C");
        m->slitTimer2 = 30;
        m->slitState = SLIT_STATE_END_2;
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
#if VERSION <= VERSION_DEMO_JP
        psndSFXOn_3D((char*)PSND_SFX_MARIO_CHANGE_SLIT2, &m->position);
    case SLIT_STATE_END_2:
        m->mot.slit.resumeState = m->slitState;
#else
        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MARIO_CHANGE_SLIT2), &m->position);
    case SLIT_STATE_END_2:
#if VERSION == VERSION_EU
        if (m->flags & MARIO_FLAG_FORCED_MOVEMENT) {
            break;
        }
#endif
#endif
        if (!m->controlStickSensitivity) {
            m->baseSpeed = 0.0f;
            marioChgPose("M_S_1");
#if VERSION >= VERSION_US
        } else {
            slitSpeedDown(m);
            m->directionView = m->controlStickAngle;
#endif
        }
        if (--m->slitTimer2 > 0) {
            break;
        }
        m->slitState = SLIT_STATE_END_3;
        break;
    case SLIT_STATE_END_3:
#if VERSION <= VERSION_DEMO_JP
        m->mot.slit.resumeState = m->slitState;
        if (!m->controlStickSensitivity) {
            m->baseSpeed = 0.0f;
            marioChgPose("M_S_1");
#else
#if VERSION == VERSION_EU
        if (!(m->flags & MARIO_FLAG_FORCED_MOVEMENT)) {
            if (!m->controlStickSensitivity) {
                m->baseSpeed = 0.0f;
                marioChgPose("M_S_1");
            } else {
                slitSpeedDown(m);
            }
#else
        if (!m->controlStickSensitivity) {
            m->baseSpeed = 0.0f;
            marioChgPose("M_S_1");
#if VERSION >= VERSION_US
        } else {
            slitSpeedDown(m);
#endif
#endif
#endif
        }
        m->flags &= ~MARIO_FLAG_PARTY_WALK_DISABLED;
        m->flags &= ~MARIO_FLAG_GONE_THROUGH_BARS;
        m->dispFlags &= ~(MARIO_DISP_FLAG_LEFT_ONLY | MARIO_DISP_FLAG_RIGHT_ONLY | MARIO_DISP_FLAG_SLIT);
        marioPaperOff();
        mapResetPaperAmbColor();
        m->dispFlags &= ~MARIO_DISP_FLAG_NO_REAR_POSE;
#if VERSION <= VERSION_DEMO_JP
        marioAdjustMoveDir();
        allPartySlitOff();
#else
#if VERSION == VERSION_EU
        if (!(m->flags & MARIO_FLAG_FORCED_MOVEMENT)) {
            marioAdjustMoveDir();
        }
#else
        marioAdjustMoveDir();
#endif
#if VERSION < VERSION_US
        allPartySlitOff();
#endif
#endif
        zoomOff();
        marioChgMotSub(MARIO_MOTION_STAY, 0);
        break;
    case SLIT_STATE_SCRIPT_END_1:
        m->flags &= ~MARIO_FLAG_PARTY_WALK_DISABLED;
        m->flags &= ~MARIO_FLAG_GONE_THROUGH_BARS;
        m->flags &= ~MARIO_FLAG_CHG_MOT_DISABLED;
        m->flags &= ~MARIO_FLAG_WALLS_DISABLED;
#if VERSION <= VERSION_DEMO_JP
        m->mot.slit.resumeState = m->slitState;
        m->mot.slit.flags |= SLIT_FLAG_RESUME;
#endif
        m->slitTimer2 = 0;
        m->flags &= ~MARIO_FLAG_PAPER_MODE;
        m->wPlayerCollisionBox.x = 20.0f;
        if (!m->controlStickSensitivity) {
            marioChgPose("M_S_1");
        }
        marioChgPaper("PM_S_1C");
        m->slitTimer2 = 30;
        m->slitState = SLIT_STATE_SCRIPT_END_2;
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
#if VERSION <= VERSION_DEMO_JP
        psndSFXOn_3D((char*)PSND_SFX_MARIO_CHANGE_SLIT2, &m->position);
    case SLIT_STATE_SCRIPT_END_2:
        m->mot.slit.resumeState = m->slitState;
#else
        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MARIO_CHANGE_SLIT2), &m->position);
    case SLIT_STATE_SCRIPT_END_2:
#endif
        if (--m->slitTimer2 > 0) {
            break;
        }
        m->slitState = SLIT_STATE_SCRIPT_END_3;
        break;
    case SLIT_STATE_SCRIPT_END_3:
#if VERSION <= VERSION_DEMO_JP
        m->mot.slit.resumeState = m->slitState;
        m->flags &= ~MARIO_FLAG_PARTY_WALK_DISABLED;
        m->flags &= ~MARIO_FLAG_GONE_THROUGH_BARS;
        m->dispFlags &= ~(MARIO_DISP_FLAG_LEFT_ONLY | MARIO_DISP_FLAG_RIGHT_ONLY | MARIO_DISP_FLAG_SLIT);
#else
        m->flags &= ~MARIO_FLAG_PARTY_WALK_DISABLED;
        m->flags &= ~MARIO_FLAG_GONE_THROUGH_BARS;
        m->dispFlags &= ~(MARIO_DISP_FLAG_LEFT_ONLY | MARIO_DISP_FLAG_RIGHT_ONLY | MARIO_DISP_FLAG_SLIT);
#endif
        marioPaperOff();
        mapResetPaperAmbColor();
        m->dispFlags &= ~MARIO_DISP_FLAG_NO_REAR_POSE;
        marioAdjustMoveDir();
        allPartySlitOff();
        zoomOff();
        marioChgMot(MARIO_MOTION_STAY);
        break;
    case SLIT_STATE_LECTURE_END_1:
#if VERSION <= VERSION_DEMO_JP
        m->mot.slit.resumeState = m->slitState;
        m->mot.slit.flags |= SLIT_FLAG_RESUME;
#endif
        m->slitTimer2 = 0;
        m->flags &= ~MARIO_FLAG_PAPER_MODE;
        m->wPlayerCollisionBox.x = 20.0f;
        if (!m->controlStickSensitivity) {
            marioChgPose("M_S_1");
        }
        marioChgPaper("PM_S_1C");
        m->slitTimer2 = 30;
        m->slitState = SLIT_STATE_LECTURE_END_2;
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
#if VERSION <= VERSION_DEMO_JP
        psndSFXOn_3D((char*)PSND_SFX_MARIO_CHANGE_SLIT2, &m->position);
    case SLIT_STATE_LECTURE_END_2:
        m->mot.slit.resumeState = m->slitState;
#else
        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MARIO_CHANGE_SLIT2), &m->position);
    case SLIT_STATE_LECTURE_END_2:
#endif
        if (--m->slitTimer2 > 0) {
            break;
        }
        m->slitState = SLIT_STATE_LECTURE_END_3;
        break;
    case SLIT_STATE_LECTURE_END_3:
#if VERSION <= VERSION_DEMO_JP
        m->mot.slit.resumeState = m->slitState;
#endif
        m->flags &= ~MARIO_FLAG_PARTY_WALK_DISABLED;
        m->flags &= ~MARIO_FLAG_GONE_THROUGH_BARS;
        m->flags &= ~MARIO_FLAG_WALLS_DISABLED;
        m->dispFlags &= ~(MARIO_DISP_FLAG_LEFT_ONLY | MARIO_DISP_FLAG_RIGHT_ONLY | MARIO_DISP_FLAG_SLIT);
        marioPaperOff();
        mapResetPaperAmbColor();
        m->dispFlags &= ~MARIO_DISP_FLAG_NO_REAR_POSE;
        marioAdjustMoveDir();
        allPartySlitOff();
        zoomOff();
        marioChgMot(MARIO_MOTION_STAY);
        break;
    }
}
