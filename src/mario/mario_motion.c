// Mario's motion table and motion changes (marioChgMot), jump and fall parameters, and the checks that start
// jumps, hammer, spin and the paper transformations from the controls.

#include "version.h"
#include "dolphin/types.h"
#include "mario/mario.h"
#include "mario/mariost.h"
#include "mario/mario_motion.h"
#include <string.h>
#include "mot/mot_jump.h"
#include "party.h"
#include "driver/npcdrv.h"
#include "driver/mobjdrv.h"
#include <dolphin/pad.h>
#include "driver/camdrv.h"
#include "motion/mot_yoshi.h"
#include "party/party_cloud.h"
#include "mario/mario_party.h"
#include "system.h"
#include "mario/mario_sbr.h"
#include "mario/mario_pouch.h"
#include "mario/mario_cam.h"
#include "koopa.h"
#include "mot/mot_damage.h"
#include "mot/mot_slit.h"
#include "koopa_motion.h"
#include "mario/mario_hit.h"
#include "mot/mot_jabara.h"
#include "mot/mot_ship.h"
#include "mot/mot_roll.h"
#include "mot/mot_walk.h"
#include "mot/mot_plane.h"
#include "mot/mot_stay.h"
#include "peach.h"
#include "party/party_yoshi.h"
#include "party/party_vivian.h"
#include "mot/mot_hammer.h"
#include "mot/mot_hip.h"
#include "mot/mot_dokan.h"
#include "koopa_hit.h"
#include "eff/eff_kemuri.h"
#include "pmario_sound.h"
#include "sfx_name_word.h"
#include "driver/casedrv.h"
#include "driver/hitdrv.h"


typedef struct MarioJumpData {
    f32 jump[4];   // 0x0
    f32 fall[4];   // 0x10
    f32 unk20;     // 0x20
    f32 minFall;   // 0x24
} MarioJumpData;

typedef struct MarioMotTblEntry {
    void (*main)(void);
    void (*unk)(void);
    void (*post)(void);
} MarioMotTblEntry;

void mot_talk(void);
void mot_getItem(void);
void mot_kaze(void);
#if VERSION <= VERSION_DEMO_US
static void mot_bottomless(void);
static void mot_cloud_post(void);
#else
void mot_bottomless(void);
#endif
void mot_forceReset(void);
void mot_shadow(void);
void mot_partyUse(void);
void mot_dummy(void);


BOOL marioCreviceWallChk(Vec pos);
void* fbatNpcTalkCheck(void);

/**
 * Mario's motions by motion id (standing, walking, dashing, jumping, ... and Bowser's): the function that runs
 * each frame, an unused slot and the cleanup run when Mario leaves the motion; marioMotion calls through it.
 */
MarioMotTblEntry marioMotTbl[] = {
    {mot_stay, NULL, mot_stay_post},
    {mot_walk, NULL, NULL},
    {mot_dash, NULL, NULL},
    {mot_jump, NULL, NULL},
#if VERSION >= VERSION_US
    {mot_jumpNpc, NULL, mot_jumpNpc_post},
#else
    {mot_jumpNpc, NULL, NULL},
#endif
    {mot_jumpSw, NULL, NULL},
    {mot_jumpStand, NULL, NULL},
    {mot_jump, NULL, NULL},
    {mot_jump, NULL, NULL},
    {mot_jumpSmall, NULL, NULL},
    {mot_fall, NULL, mot_fall_post},
    {mot_fall, NULL, NULL},
    {mot_upstairs, NULL, NULL},
    {mot_land, NULL, NULL},
    {mot_talk, NULL, NULL},
    {mot_getItem, NULL, NULL},
    {mot_hip, NULL, mot_hip_post},
    {mot_hip, NULL, mot_hip_post},
    {mot_hammer, NULL, NULL},
    {mot_hammer2, NULL, mot_ham2_post},
    {mot_jabara, NULL, mot_jabara_post},
    {mot_slit, NULL, mot_slit_post},
    {mot_roll, NULL, mot_roll_post},
    {mot_kaze, NULL, NULL},
    {mot_plane, NULL, mot_plane_post},
    {mot_ship, NULL, mot_ship_post},
    {mot_yoshi, NULL, mot_yoshi_post},
#if VERSION <= VERSION_DEMO_US
    {mot_cloud, NULL, mot_cloud_post},
#else
    {mot_cloud, NULL, cloudResetAt},
#endif
    {mot_vivian, NULL, mot_vivian_post},
    {mot_dokan, NULL, NULL},
    {mot_grasp, NULL, NULL},
    {mot_damage, NULL, NULL},
    {mot_damageToge, NULL, NULL},
    {mot_bottomless, NULL, NULL},
    {mot_forceReset, NULL, NULL},
    {mot_shadow, NULL, NULL},
    {mot_partyUse, NULL, NULL},
    {kpa_swim, NULL, NULL},
    {kpa_powup, NULL, NULL},
    {kpa_powdown, NULL, NULL},
    {mot_dummy, NULL, NULL},
};

/**
 * Mario's normal jump physics: rising speed, acceleration, jerk and snap; the same for falling; an unread height
 * and the fastest fall speed. Bowser's jump also starts from the rising values.
 */
MarioJumpData marioJumpData = {
    {8.0f, -0.84f, 0.08f, -0.02f},
    {-0.1f, -0.2f, 0.024f, -0.004f},
    32.0f,
    -18.0f,
};

/**
 * Puts Mario back into stay/walk/dash from the field battle code (fbatBattleMode state 4).
 * The plane is cancelled, the tube makes the partners ride on (not in the demo),
 * and Yoshi (and, US and later, the damage motions) are left alone.
 * With no speed it switches to stay but then still to walk, as there is no return.
 */
void marioChgMoveMotion(void) {
    MarioWork* mario = marioGetPtr();
    if (mario->currMotionId == MARIO_MOTION_PLANE) {
        mario_plane_cancel();
        return;
    }
#if VERSION > VERSION_DEMO_US
    if (mario->currMotionId == MARIO_MOTION_ROLL) {
        allPartyForceRideOn();
        return;
    }
#if VERSION >= VERSION_US
    if ((u16)(mario->currMotionId - MARIO_MOTION_DAMAGE) <= 1) {
        return;
    }
#endif
#endif
    if (mario->currMotionId == MARIO_MOTION_YOSHI) {
        return;
    }
    // At zero speed this calls the stay transition and then continues into the walk/dash selection below (no return).
    if (!mario->baseSpeed) {
        marioChgStayMotion();
    }
    if (mario->baseSpeed > mario->baseWalkSpeed) {
        marioChgMot(MARIO_MOTION_DASH);
    } else {
        marioChgMot(MARIO_MOTION_WALK);
    }
}

/**
 * Stops Mario in the standing motion unless he is riding the boat or Yoshi,
 * or is in the paper slit or tube motion. Called by events, item pickups, hit checks and the lecture maps.
 */
void marioChgStayMotion(void) {
    MarioWork* mario = marioGetPtr();
    if (mario->currMotionId != MARIO_MOTION_SHIP && mario->currMotionId != MARIO_MOTION_YOSHI &&
        mario->currMotionId != MARIO_MOTION_SLIT && mario->currMotionId != MARIO_MOTION_ROLL) {
        marioChgMot(MARIO_MOTION_STAY);
    }
}

/** Starts the talk motion (mot_talk), from npcdrv when Mario speaks to an NPC and from evt_mario. */
void marioChgTalkMotion(void) {
    marioChgMot(MARIO_MOTION_TALK);
}

/**
 * Starts the item-pickup pose (mot_getItem) from itemdrv and evt_sub.
 * In tube mode it only resets the roll speed, in the slit or the boat it stops Mario,
 * and on Yoshi nothing changes.
 */
void marioChgGetItemMotion(void) {
    MarioWork* mario = marioGetPtr();
    if (mario->currMotionId == MARIO_MOTION_ROLL) {
        marioResetRollSpd();
    } else if (mario->currMotionId == MARIO_MOTION_SLIT) {
        mario->baseSpeed = 0.0f;
    } else if (mario->currMotionId == MARIO_MOTION_SHIP) {
        mario->baseSpeed = 0.0f;
    } else if (mario->currMotionId != MARIO_MOTION_YOSHI) {
        marioChgMot(MARIO_MOTION_GET_ITEM);
    }
}

/**
 * Re-initialises the boat motion (marioReInit_ship);
 * the Pianta Parlor boat game (yuu_01) and the E3 sequence use it.
 */
void marioChgShipMotion(void) {
    marioReInit_ship();
}

/**
 * Puts Mario straight into the tube (roll) motion at its sub-state 50,
 * without running the old motion's post function or the first-frame setup.
 * The Pianta Parlor tube game (yuu_02) uses it; landing in tube mode too.
 */
void marioChgRollMotion(void) {
    MarioWork* mario = marioGetPtr();
    mario->currSubMotionId = 50;
    mario->currMotionId = MARIO_MOTION_ROLL;
}

/**
 * Launches Mario off a spring (jump stand) with v as the jump height:
 * from evt_mobj and on landing on MOBJ_BlueJumpStand (100).
 * In tube mode only the height is stored (retail also puffs smoke and jumps); on Yoshi, yoshi_jumpStand jumps.
 */
void marioChgJumpStandMotion(f32 v) {
    MarioWork* mario = marioGetPtr();
#if VERSION > VERSION_DEMO_US
    camFollowYOn();
#endif
    if (marioChkSts(MARIO_STS_ROLL)) {
        mario->mot.jump.standHeight = v;
#if VERSION <= VERSION_DEMO_US
        } else
    if (mario->currMotionId == MARIO_MOTION_YOSHI) {
#else
        effKemuriEntry(0, mario->position.x, mario->position.y, mario->position.z, 0.0f);
        marioChgMot(MARIO_MOTION_JUMP_STAND);
    } else if (mario->currMotionId == MARIO_MOTION_YOSHI) {
#endif
        mario->mot.jump.standHeight = v;
        yoshi_jumpStand();
    } else {
        mario->mot.jump.standHeight = v;
        marioChgMot(MARIO_MOTION_JUMP_STAND);
    }
}

/**
 * A small fixed hop (start speed 6, no air steering), used by the Koops,
 * Vivian and Yoshi partner code and mot_jabara.
 */
void marioChgSmallJumpMotion(void) {
    MarioWork* mario = marioGetPtr();
    mario->trigFlags |= MARIO_TRIG_FLAG_SET_JUMP_PARAMS;
    marioChgMot(MARIO_MOTION_JUMP);
    mario->flags |= MARIO_FLAG_STOP_AIR_MOVEMENT;
    mario->hitobjJumpFrom = mario->hitobjStandOn;
    mario->hitobjStandOn = NULL;
    mario->jumpVelocityY = 6.0f;
    mario->jumpAccelerationY = -0.5f;
    mario->jumpJerkY = 0.0f;
    mario->jumpSnapY = 0.0f;
}

/**
 * TRUE when Mario can take an item pickup now: standing, walking, dashing, on Yoshi,
 * in the tube or the slit and not starting a new motion, or on the boat once its transformation has finished.
 * itemdrv checks it before pickups.
 */
BOOL marioChkItemGetMotion(void) {
    BOOL ret = FALSE;
    MarioWork* mario = marioGetPtr();
    MarioMotion cur = mario->currMotionId;
    if (!cur || cur == MARIO_MOTION_WALK || cur == MARIO_MOTION_DASH ||
        cur == MARIO_MOTION_YOSHI || cur == MARIO_MOTION_ROLL || cur == MARIO_MOTION_SLIT) {
        if (!(mario->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION)) {
            ret = TRUE;
        }
    } else if (cur == MARIO_MOTION_SHIP) {
        if (marioShipChanging() == 3) {
            ret = TRUE;
        }
    }
    return ret;
}

/**
 * TRUE when Mario may start talking to an NPC: standing, walking or dashing, not in background mode or the slit,
 * and not while Yoshi is being ridden or Vivian is hiding him. npcdrv checks it before choosing a talk target.
 */
BOOL marioChkTalkable(void) {
    MarioWork* mario = marioGetPtr();
    BOOL ret = FALSE;
    PartyEntry* party = partyGetPtr(marioGetPartyId());
    if (party != NULL && party->currentMemberId == PARTY_MEMBER_YOSHI && (party->flags & PARTY_FLAG_IS_BEING_USED)) {
        return FALSE;
    }
    switch (mario->currMotionId) {
        case MARIO_MOTION_STAY:
        case MARIO_MOTION_WALK:
        case MARIO_MOTION_DASH:
            if (!(mario->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) && marioBgmodeChk() != TRUE && !(mario->dispFlags & MARIO_DISP_FLAG_SLIT)) {
                ret = TRUE;
            }
            break;
    }
    if (vivianGetStatus()) {
        ret = FALSE;
    }
    return ret;
}

/**
 * Per-frame air check (from marioMain, Peach's and Bowser's main) that starts the Spin Jump (MARIO_MOTION_HIP)
 * on A: Mario needs a jump input and marioChkHipAttack;
 * Bowser instead needs kpaHipAttackOk and 6 frames to his peak.
 * Never in 8-bit or paper mode; the PAL release also skips it in plane mode.
 */
void marioBoots(void) {
    MarioWork* mario = marioGetPtr();
    u32 flags = mario->flags;
    if (flags & MARIO_FLAG_IS_8_BIT) {
        return;
    }
    if (flags & MARIO_FLAG_PAPER_MODE) {
        return;
    }
    if (mario->characterId == CHARACTER_BOWSER) {
        if (mario->jumpPeakAirTime < 6) {
            return;
        }
        if (!kpaHipAttackOk()) {
            return;
        }
        if (!(mario->buttonTrg & PAD_BUTTON_A)) {
            return;
        }
        marioChgMot(MARIO_MOTION_HIP);
        return;
    }
    if (!(flags & MARIO_FLAG_HAS_INPUT_JUMP)) {
#if VERSION == VERSION_EU
        return;
    }
    if (mario->currMotionId == MARIO_MOTION_PLANE) {
#endif
        return;
    }
    if (!marioChkHipAttack()) {
        return;
    }
    marioChgMot(MARIO_MOTION_HIP);
}

/**
 * Runs the current motion's main function from marioMain each frame, again while a motion change is pending,
 * so a new motion's first frame runs in the same frame. Clears the hammer's hit object first.
 */
void marioMotion(void) {
    MarioWork* mario = marioGetPtr();
    mario->hitobjHammer = NULL;
    do {
        marioMotTbl[mario->currMotionId].main();
    } while (mario->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION);
}

/**
 * The usual motion change: runs the old motion's post function
 * and flags the new one to do its first-frame setup.
 * Ignored while MARIO_FLAG_CHG_MOT_DISABLED; after a Spin Jump it clears the blur and front pose.
 */
void marioChgMot(MarioMotion id) {
    MarioWork* mario = marioGetPtr();
    if (mario->flags & MARIO_FLAG_CHG_MOT_DISABLED) {
        return;
    }
    if (mario->prevMotionId == MARIO_MOTION_HIP || mario->prevMotionId == MARIO_MOTION_HIP_2) {
        if (mario->dispFlags & MARIO_DISP_FLAG_BLUR) {
            mario->dispFlags &= ~MARIO_DISP_FLAG_BLUR;
        }
        if (mario->dispFlags & MARIO_DISP_FLAG_NO_REAR_POSE) {
            mario->dispFlags &= ~MARIO_DISP_FLAG_NO_REAR_POSE;
        }
    }
    mario->prevMotionId = mario->currMotionId;
    mario->currMotionId = id;
    if (marioMotTbl[mario->prevMotionId].post) {
        marioMotTbl[mario->prevMotionId].post();
    }
    mario->trigFlags |= MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
}

/**
 * marioChgMot with a choice: callPost FALSE skips the old motion's post function (retail still runs it when
 * leaving the jabara motion). Used by motion code that has already cleaned up itself.
 */
void marioChgMotSub(MarioMotion id, s32 callPost) {
    MarioWork* mario = marioGetPtr();
    if (mario->flags & MARIO_FLAG_CHG_MOT_DISABLED) {
        return;
    }
    if (mario->prevMotionId == MARIO_MOTION_HIP || mario->prevMotionId == MARIO_MOTION_HIP_2) {
        if (mario->dispFlags & MARIO_DISP_FLAG_BLUR) {
            mario->dispFlags &= ~MARIO_DISP_FLAG_BLUR;
        }
        if (mario->dispFlags & MARIO_DISP_FLAG_NO_REAR_POSE) {
            mario->dispFlags &= ~MARIO_DISP_FLAG_NO_REAR_POSE;
        }
    }
    mario->prevMotionId = mario->currMotionId;
    mario->currMotionId = id;
    if (callPost) {
        if (marioMotTbl[mario->prevMotionId].post) {
            marioMotTbl[mario->prevMotionId].post();
        }
#if VERSION > VERSION_DEMO_US
    } else if (mario->prevMotionId == MARIO_MOTION_JABARA) {
        if (marioMotTbl[mario->prevMotionId].post) {
            marioMotTbl[mario->prevMotionId].post();
        }
#endif
    }
    mario->trigFlags |= MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
}

/**
 * Changes motion without the new motion's first-frame setup (the start flag is cleared, not set),
 * then runs the old motion's post function.
 * Used by mot_jump, mot_stay, mot_slit and Bowser's motions to switch mid-move.
 */
void marioChgMot2(MarioMotion id) {
    MarioWork* mario = marioGetPtr();
    if (mario->flags & MARIO_FLAG_CHG_MOT_DISABLED) {
        return;
    }
    mario->prevMotionId = mario->currMotionId;
    mario->currMotionId = id;
    mario->trigFlags &= ~MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
    if (marioMotTbl[mario->prevMotionId].post) {
        marioMotTbl[mario->prevMotionId].post();
    }
}

/** Zeroes Mario's vertical motion (jump velocity, acceleration, jerk and snap), e.g. on landing. */
void marioClearJumpPara(void) {
    MarioWork* mario = marioGetPtr();
    mario->jumpVelocityY = 0.0f;
    mario->jumpAccelerationY = 0.0f;
    mario->jumpJerkY = 0.0f;
    mario->jumpSnapY = 0.0f;
}

/**
 * Loads the starting vertical speed curve for the current jump motion from marioJumpData (or marioJumpSwData for
 * the red switch).
 * The pre-battle slow motion halves it, the Moon's low gravity swaps in a floatier set,
 * and Bowser uses kpaSetJumpPara. Called by mot_jump at the start of a jump.
 */
void marioSetJumpPara(void) {
    MarioWork* mario = marioGetPtr();
    f32* jd;
    switch (mario->currMotionId) {
        case MARIO_MOTION_JUMP_SW:
            mario->jumpVelocityY = marioJumpSwData[0];
            mario->jumpAccelerationY = marioJumpSwData[1];
            mario->jumpJerkY = marioJumpSwData[2];
            mario->jumpSnapY = marioJumpSwData[3];
            break;
        case MARIO_MOTION_JUMP:
        case MARIO_MOTION_UPSTAIRS:
        case MARIO_MOTION_JABARA:
        case MARIO_MOTION_PLANE:
        case MARIO_MOTION_SHIP:
            if (mario->trigFlags & MARIO_TRIG_FLAG_SET_JUMP_PARAMS) {
                jd = marioJumpData.jump;
                if (mario->flags & MARIO_FLAG_PRE_BATTLE_PHYSICS) {
                    mario->jumpVelocityY = 0.5f * jd[0];
                    mario->jumpAccelerationY = 0.5f * jd[1];
                    mario->jumpJerkY = 0.5f * jd[2];
                    mario->jumpSnapY = 0.5f * jd[3];
                }
            } else {
                jd = marioJumpData.jump;
                if (!(mario->flags & MARIO_FLAG_PRE_BATTLE_PHYSICS)) {
                    mario->jumpVelocityY = jd[0];
                    mario->jumpAccelerationY = jd[1];
                    mario->jumpJerkY = jd[2];
                    mario->jumpSnapY = jd[3];
                } else {
                    mario->jumpVelocityY = 0.5f * jd[0];
                    mario->jumpAccelerationY = 0.5f * jd[1];
                    mario->jumpJerkY = 0.5f * jd[2];
                    mario->jumpSnapY = 0.5f * jd[3];
                }
                if (1.0f != mario->playerGravity) {
                    mario->jumpVelocityY = 2.0f;
                    mario->jumpAccelerationY = -0.003f;
                    mario->jumpJerkY = 0.029f;
                    mario->jumpSnapY = -0.003f;
                }
            }
            break;
        case MARIO_MOTION_JUMP_NPC:
            mario->jumpVelocityY = 2.0f;
            mario->jumpAccelerationY = -0.04f;
            mario->jumpJerkY = -0.01f;
            mario->jumpSnapY = -0.003f;
            break;
        case MARIO_MOTION_ROLL:
            mario->jumpVelocityY = (jd = marioJumpData.jump)[0];
            mario->jumpAccelerationY = jd[1];
            mario->jumpJerkY = jd[2];
            mario->jumpSnapY = jd[3];
            if (1.0f != mario->playerGravity) {
                mario->jumpVelocityY = 2.7f;
                mario->jumpAccelerationY = -0.002f;
                mario->jumpJerkY = 0.016f;
                mario->jumpSnapY = -0.002f;
            }
            break;
    }
    if (mario->currMotionId != MARIO_MOTION_JUMP_3 && mario->characterId == CHARACTER_BOWSER) {
        kpaSetJumpPara();
    }
}

/**
 * Loads the falling speed curve from marioJumpData.fall (the red switch jump has its own),
 * or a slow one under the Moon's low gravity.
 * marioJump calls it at a jump's peak; mario.c and several motions (damage, plane, boat) too.
 */
void marioSetFallPara(void) {
    MarioWork* mario = marioGetPtr();
    f32* fall = marioJumpData.fall;
    if (mario->currMotionId == MARIO_MOTION_JUMP_SW) {
        fall = &marioJumpSwData[4];
    }
    mario->jumpVelocityY = fall[0];
    mario->jumpAccelerationY = fall[1];
    mario->jumpJerkY = fall[2];
    mario->jumpSnapY = fall[3];
    if (1.0f != mario->playerGravity) {
        mario->jumpVelocityY = 0.34f;
        mario->jumpAccelerationY = -0.002f;
        mario->jumpJerkY = -0.01f;
        mario->jumpSnapY = 0.0f;
    }
}

/**
 * Steps the fall curve one frame and returns the new vertical speed (units per frame),
 * capped at marioJumpData.minFall (-18); the jerk is not allowed to turn positive.
 */
f32 marioGetFallSpd(void) {
    MarioWork* mario = marioGetPtr();
    MarioJumpData* jd;
    f32 min;
    mario->jumpJerkY = mario->jumpJerkY + mario->jumpSnapY;
    if (mario->jumpJerkY > 0.0f) {
        mario->jumpJerkY = 0.0f;
    }
    jd = &marioJumpData;
    mario->jumpAccelerationY = mario->jumpAccelerationY + mario->jumpJerkY;
    mario->jumpVelocityY = mario->jumpVelocityY + mario->jumpAccelerationY;
    if (mario->jumpVelocityY <= (min = jd->minFall)) {
        mario->jumpVelocityY = min;
        mario->jumpAccelerationY = 0.0f;
        mario->jumpJerkY = 0.0f;
        mario->jumpSnapY = 0.0f;
    }
    return mario->jumpVelocityY;
}

/**
 * Steps the jump curve one frame (snap into jerk into acceleration into velocity)
 * and returns the new vertical speed. Everything is divided by 1.7 during the pre-battle slow motion.
 */
f32 marioMakeJumpPara(void) {
    MarioWork* mario = marioGetPtr();
    f32 div = (mario->flags & MARIO_FLAG_PRE_BATTLE_PHYSICS) ? 1.7f : 1.0f;
    mario->jumpJerkY = mario->jumpJerkY + mario->jumpSnapY / div;
    mario->jumpAccelerationY = mario->jumpAccelerationY + mario->jumpJerkY / div;
    mario->jumpVelocityY = mario->jumpVelocityY + mario->jumpAccelerationY / div;
    return mario->jumpVelocityY;
}

void marioJump(void) {
    MarioWork* mario = marioGetPtr();
    f32 prev;
    if (getRollEvtFlag()) {
        return;
    }
    switch (mario->currMotionId) {
        case MARIO_MOTION_JUMP_NPC:
            break;
        case MARIO_MOTION_JUMP_SW:
        case MARIO_MOTION_JUMP_STAND:
        case MARIO_MOTION_JUMP_2:
        case MARIO_MOTION_JUMP_3:
            return;
    }
    if (mario->currSubMotionId < 10) {
        return;
    }
    prev = mario->jumpVelocityY;
    marioMakeJumpPara();
    if (mario->jumpVelocityY > 0.0f && mario->airTimer >= 4 && !(mario->buttonHold & PAD_BUTTON_A)) {
        mario->jumpVelocityY += -0.526f;
    }
    if (mario->jumpVelocityY <= 0.0f) {
        if (prev > 0.0f) {
            mario->jumpPeakAirTime = mario->airTimer;
        }
        marioSetPeakYpos();
        marioSetFallPara();
        marioMakeJumpPara();
        if (!marioChkSts(MARIO_STS_ROLL)) {
            marioChgMot(MARIO_MOTION_FALL);
        } else {
            mario->jumpVelocityY = -0.03f;
            mario->jumpAccelerationY = -0.02f;
            mario->jumpJerkY = -0.02f;
            mario->jumpSnapY = -0.002f;
            mario->flags &= ~MARIO_FLAG_IS_JUMPING;
            mario->flags |= MARIO_FLAG_IS_FALLING;
        }
    }
    mario->position.y += mario->jumpVelocityY;
}

/**
 * Per-frame fall (marioMain and Peach): slows air drift, stops it off screen,
 * steps the fall speed and lands Mario on the floor below (marioLandOn);
 * a passing jabara check (marioPriCheckJabara, stick rub) turns the landing into the jabara motion.
 * On kpa_03 Bowser just falls and lands; the tube uses mot_fall_roll.
 */
void marioFall(void) {
    MarioWork* mario = marioGetPtr();
    s32 rub;
    f32 spd;
    s32 b;
    s32 a;
    f32 c;
    f32 y;
    void* hit;

    if (!strcmp(gp->currentMapName, "kpa_03") && mario->characterId == CHARACTER_BOWSER) {
        mario->hitobjStandOn = NULL;
        kpaClearHitobjRide();
        spd = marioGetFallSpd();
        hit = marioChkLandon(&y, spd);
        if (hit) {
            mario->hitobjStandOn = hit;
            mario->hitobjJumpFrom = NULL;
            mario->position.y = y;
        } else {
            mario->position.y += spd;
        }
        if (mario->hitobjStandOn) {
            marioLandOn();
        }
        return;
    }
    if (!(mario->flags & MARIO_FLAG_FORCED_MOVEMENT) && !marioChkInScreen(mario->screenX, mario->screenY)) {
        mario->flags |= MARIO_FLAG_STOP_AIR_MOVEMENT;
        mario->baseSpeed += 0.1f * -mario->baseSpeed;
    }
    if (getRollEvtFlag()) {
        return;
    }
    if (mario->currMotionId == MARIO_MOTION_HIP || mario->currMotionId == MARIO_MOTION_HIP_2) {
        return;
    }
    if ((!marioKeyOffChk() || !marioCtrlOffChk()) && !(mario->flags & MARIO_FLAG_FORCED_MOVEMENT)) {
        f32 s = mario->baseSpeed;
        s = s + 0.01f * (-0.5f - s);
        if (s < 0.0f) {
            s = 0.0f;
        }
        mario->baseSpeed = s;
    }
    mario->hitobjStandOn = NULL;
    if (mario->characterId == CHARACTER_BOWSER) {
        kpaClearHitobjRide();
    }
    if (marioChkSts(MARIO_STS_ROLL) == TRUE) {
        mot_fall_roll();
        return;
    }
#if VERSION >= VERSION_US
    if (!(mario->trigFlags & MARIO_TRIG_FLAG_JABARA_MOVE_CHK) && (mario->trigFlags & MARIO_TRIG_FLAG_JABARA_CHK)) {
        rub = marioGetRub(PAD_BUTTON_A, &a, &b, &c);
        if (mario->buttonHold & PAD_BUTTON_A) {
            mario->trigFlags |= MARIO_TRIG_FLAG_START_JABARA_SPIN;
        }
    }
#else
    rub = marioGetRub(PAD_BUTTON_A, &a, &b, &c);
#endif
    spd = marioGetFallSpd();
    hit = marioChkLandon(&y, spd);
    if (hit) {
        mario->hitobjStandOn = hit;
        mario->hitobjJumpFrom = NULL;
        if (hitGetAttr(hit) & HIT_ATTR_WATER_HAZARD) {
            mario->position.y += spd;
        } else {
            mario->position.y = y;
#if VERSION <= VERSION_DEMO_US
            if (marioPriCheckJabara() && a != -1 && rub >= 0) {
#else
#if VERSION >= VERSION_US
            if (!(mario->trigFlags & MARIO_TRIG_FLAG_JABARA_MOVE_CHK) && (mario->trigFlags & MARIO_TRIG_FLAG_JABARA_CHK) && marioPriCheckJabara() && a != -1 && rub >= 0) {
#else
            if (marioPriCheckJabara() && a != -1 && rub >= 0) {
#endif
                camFollowYOn();
#endif
                marioChgMot(MARIO_MOTION_JABARA);
                return;
            }
        }
    } else {
        mario->position.y += spd;
    }
    if (mario->characterId != CHARACTER_BOWSER) {
        if (mario->hitobjStandOn) {
            marioLandOn();
        }
    } else if (kpaChkHitobjRide()) {
        marioLandOn();
    }
}

/**
 * Mario touches down: plays the landing sound for the floor type, clears the jump state,
 * and picks the next motion: damage on hazard or spike floors, the red switch or blue spring objects,
 * walk or stand from the stick, Bowser's quake, re-entering the slit in paper mode.
 * Bowser and the tube also call it.
 */
void marioLandOn(void) {
    MarioWork* mario = marioGetPtr();
    MapObjectEntry* mobj;
    Vec pos;
    s32 attr;
    u32 flags;
    s32 sfx;
    void* stand;

    if (!(hitGetAttr(mario->hitobjStandOn) & HIT_ATTR_WATER_HAZARD)) {
        marioClearJumpPara();
    }
    mario->flags &= ~MARIO_FLAG_STOP_AIR_MOVEMENT;
    mario->flags &= ~MARIO_FLAG_USE_FORCE_DIRECTION;
    mario->airTimer = 0;
    mario->jumpPeakAirTime = 0;
    if (mario->characterId != CHARACTER_BOWSER) {
        if (!marioBgmodeChk()) {
            camFollowYOn();
        }
    } else {
        kpaCamFollow();
    }
    stand = mario->hitobjStandOn;
    pos = mario->position;
    if (stand) {
        attr = hitGetAttr(stand);
        if (attr & HIT_ATTR_SHALLOW_WATER) {
            sfx = 0x150;
        } else if (attr & HIT_ATTR_WOOD) {
            sfx = 0x151;
        } else if (attr & HIT_ATTR_SNOW) {
            sfx = 0x152;
        } else if (attr & HIT_ATTR_GRASS) {
            sfx = 0x153;
        } else if (attr & 0x400000) {
            sfx = 0x154;
        } else {
            sfx = 0x14f;
        }
        psndSFXOn_3D(SFX_NAME_WORD(sfx), &pos);
    }
    if (mario->flags & MARIO_FLAG_TUBE_MODE) {
        mario->flags &= ~(MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING);
        mario->hitobjJumpFrom = NULL;
        return;
    }
    mario->mot.jump.dist = __fabsf(distABf(mario->position.x, mario->position.z, mario->lastGroundPos.x, mario->lastGroundPos.z));
    attr = hitGetAttr(mario->hitobjStandOn);
    if (mario->currMotionId != MARIO_MOTION_DAMAGE && (attr & HIT_ATTR_WATER_HAZARD)) {
        set_damage_root_ypos(mario->position.y);
        marioChgMot(MARIO_MOTION_DAMAGE);
        return;
    }
    if (mario->currMotionId != MARIO_MOTION_DAMAGE_TOGE && (attr & HIT_ATTR_TOGE)) {
        set_damage_root_ypos(mario->position.y);
        marioChgMot(MARIO_MOTION_DAMAGE_TOGE);
        return;
    }
    if (hitGetAttr(mario->hitobjStandOn) & HIT_ATTR_MOBJ) {
        mobj = mobjHitObjPtrToPtr(mario->hitobjStandOn);
        if (!strcmp(mobj->animName, "MOBJ_RedSwitch")) {
            marioChgMot(MARIO_MOTION_JUMP_SW);
            return;
        }
        if (!strcmp(mobj->animName, "MOBJ_BlueJumpStand")) {
            marioChgJumpStandMotion(100.0f);
            return;
        }
        if (mario->characterId == CHARACTER_BOWSER) {
            marioChgMot(MARIO_MOTION_STAY);
            return;
        }
        if (mario->currMotionId != MARIO_MOTION_SLIT) {
            marioChgMot(MARIO_MOTION_STAY);
            return;
        }
        if (mario->flags & MARIO_FLAG_PAPER_MODE) {
#if VERSION <= VERSION_DEMO_US
            marioChgMotSub(MARIO_MOTION_STAY, FALSE);
#else
            marioSlitReenter();
#endif
            return;
        }
        if ((mario->dispFlags & MARIO_DISP_FLAG_SLIT) && !marioChkSlitEnd()) {
            motSlitCancel();
        }
        return;
    }
#if VERSION <= VERSION_DEMO_US
    if (mario->controlStickSensitivity > 0.0f && !(mario->flags & MARIO_FLAG_FORCED_MOVEMENT)) {
#else
#if VERSION >= VERSION_US
    if (mario->controlStickSensitivity > 0.0f && !(mario->trigFlags & MARIO_TRIG_FLAG_START_JABARA_SPIN) && !(mario->flags & MARIO_FLAG_FORCED_MOVEMENT)) {
#else
    if (mario->controlStickSensitivity > 0.0f && !(mario->flags & MARIO_FLAG_FORCED_MOVEMENT)) {
#endif
#endif
        if (mario->characterId == CHARACTER_BOWSER) {
            if (kpaFireAttackCheck()) {
                marioChgMot(MARIO_MOTION_STAY);
            } else {
                marioChgMot(MARIO_MOTION_WALK);
            }
            quake_kpaLandOn();
            return;
        }
#if VERSION <= VERSION_DEMO_US
        marioChgMot(MARIO_MOTION_WALK); // the demo does not stay in the slit after landing
#else
        if (!(mario->dispFlags & MARIO_DISP_FLAG_SLIT)) {
#if VERSION >= VERSION_US
            if (marioPriCheckJabara()) {
                if (mario->buttonHold & PAD_BUTTON_A) {
                    marioChgMot(MARIO_MOTION_STAY);
                } else {
                    marioChgMot(MARIO_MOTION_WALK);
                }
            } else {
                marioChgMot(MARIO_MOTION_WALK);
            }
#else
            marioChgMot(MARIO_MOTION_WALK);
#endif
            return;
        }
        marioSlitSetAfterLand();
#endif
        return;
    }
    flags = mario->flags;
    if (flags & MARIO_FLAG_TUBE_MODE) {
        marioChgRollMotion();
        return;
    }
    if (mario->characterId == CHARACTER_BOWSER) {
        marioChgMot(MARIO_MOTION_STAY);
        quake_kpaLandOn();
        return;
    }
    if (mario->currMotionId != MARIO_MOTION_SLIT) {
        if (!(flags & MARIO_FLAG_FORCED_MOVEMENT)) {
            marioChgMot(MARIO_MOTION_STAY);
            return;
        }
        marioChgMot(mario->baseSpeed > mario->baseWalkSpeed ? MARIO_MOTION_DASH : MARIO_MOTION_WALK);
        return;
    }
    if (flags & MARIO_FLAG_PAPER_MODE) {
#if VERSION <= VERSION_DEMO_US
        marioChgMotSub(MARIO_MOTION_STAY, FALSE);
#else
        marioSlitReenter();
#endif
        return;
    }
    if ((mario->dispFlags & MARIO_DISP_FLAG_SLIT) && !marioChkSlitEnd()) {
        motSlitCancel();
    }
}

/**
 * Talk motion: turns Mario left or right to face the NPC he touched and takes that NPC's talk pose;
 * the M_I_2 pose (M_I_3 in the PAL release) returns to M_S_1 after 60 frames.
 * Peach uses peach_talk, Bowser his own pose.
 */
#if VERSION <= VERSION_DEMO_US
static f32 marioDemoHalf(f32 x) {
    if (x < 0.0) {
        return x;
    }
    return 0.5 * x * (3.0 - x);
}
#endif
void mot_talk(void) {
    MarioWork* mario = marioGetPtr();
    NpcEntry* npc;
    f32 camYaw;
    f32 angle;
    NpcEntry* fbat;

    if (mario->characterId == CHARACTER_PEACH) {
        peach_talk();
        return;
    }
    if (mario->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) {
        mario->trigFlags &= ~MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
        mario->multiTimer = 0;
        mario->airTimer = 0;
        mario->currSubMotionId = 0;
        mario->baseSpeed = 0.0f;
        npc = marioGetTouchNpcPtr();
        if (npc == NULL) {
            marioAdjustMoveDir();
        } else {
            camYaw = camGetPtr(CAMERA_3D)->viewYaw;
            angle = revise360(270.0f - (s32)(10.0f * (angleABf(mario->position.x, mario->position.z,
                ((NpcEntry*)npc)->position.x, ((NpcEntry*)npc)->position.z) - camYaw) + 0.5f) / 10.0f);
            if (angle >= 90.0f && angle <= 270.0f) {
                mario->moveDirection = toMovedir(90.0f);
                mario->directionView = mario->moveDirection;
                mario->displayDirectionTarget = 180.0f;
            } else {
                mario->moveDirection = toMovedir(90.0f);
                mario->directionView = mario->moveDirection;
                mario->displayDirectionTarget = 0.0f;
            }
        }
        if (mario->characterId == CHARACTER_BOWSER) {
            kpaChgPose("KPA_S_1", "KPA2_S_1");
            return;
        }
        // BUG: fbatGetPointer()->npc is whichever NPC Mario last talked to or battled, from any map. If that
        // NPC slot is unused on the current map, checkPose is not valid and marioChgPose can crash.
        fbat = fbatGetPointer()->npc;
        if (fbat != NULL) {
            marioChgPose(((NpcEntry*)fbat)->checkPose);
        }
#if VERSION == VERSION_EU
        if (!strcmp(mario->animName, "M_I_3")) {
#else
        if (!strcmp(mario->animName, "M_I_2")) {
#endif
            mario->multiTimer = 60;
            mario->currSubMotionId = 10;
        }
    }
    switch (mario->currSubMotionId) {
        case 0:
            return;
        case 10:
            if (--mario->multiTimer > 0) {
                break;
            }
            marioChgPose("M_S_1");
            mario->currSubMotionId = 11;
            break;
    }
}

/** Item pickup motion: stops Mario and shows the item-up pose M_I_U until the item code changes the motion. */
void mot_getItem(void) {
    MarioWork* mario = marioGetPtr();
    if (mario->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) {
        mario->trigFlags &= ~MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
        mario->multiTimer = 0;
        mario->airTimer = 0;
        mario->currSubMotionId = 0;
        mario->baseSpeed = 0.0f;
        marioAdjustMoveDir();
        marioChgPose("M_I_U");
    }
}

/**
 * Starts a jump on A from stand, walk or the slit unless A belongs to something else:
 * an object hint or A-button case in front or underfoot, an NPC in talk range, or Yoshi.
 * PAL also refuses on a damage floor. TRUE if Mario jumped.
 */
BOOL marioChkJump(void) {
    MarioWork* mario = marioGetPtr();
    CaseEntry* ce;
    if (!(mario->buttonTrg & PAD_BUTTON_A)) {
        return FALSE;
    }
    if (mario->currMotionId == MARIO_MOTION_YOSHI) {
        return FALSE;
    }
    if (mario->hitobjFrontA) {
        if ((hitGetAttr(mario->hitobjFrontA) & HIT_ATTR_MOBJ) && mobjGetHint(mobjHitObjPtrToPtr(mario->hitobjFrontA)) == 1) {
            return FALSE;
        }
        ce = caseCheckHitObj(mario->hitobjFrontA);
        if (ce && (ce->conditionId == CASE_FRONT_A || ce->conditionId == CASE_JUMP_FROM_A || ce->conditionId == CASE_PUSH_A)) {
            return FALSE;
        }
    }
    if (mario->hitobjFront) {
        if ((hitGetAttr(mario->hitobjFront) & HIT_ATTR_MOBJ) && mobjGetHint(mobjHitObjPtrToPtr(mario->hitobjFront)) == 1) {
            return FALSE;
        }
        ce = caseCheckHitObj(mario->hitobjFront);
        if (ce && (ce->conditionId == CASE_FRONT_A || ce->conditionId == CASE_JUMP_FROM_A || ce->conditionId == CASE_PUSH_A)) {
            return FALSE;
        }
    }
    if (mario->hitobjStandOn) {
        if ((hitGetAttr(mario->hitobjStandOn) & HIT_ATTR_MOBJ) && mobjGetHint(mobjHitObjPtrToPtr(mario->hitobjStandOn)) == 1) {
            return FALSE;
        }
        ce = caseCheckHitObj(mario->hitobjStandOn);
        if (ce && ce->conditionId == CASE_STAND_ON_A) {
            return FALSE;
        }
    }
    if (fbatNpcTalkCheck()) {
#if VERSION == VERSION_EU
        return FALSE;
    }
    if (mario->hitobjStandOn && (hitGetAttr(mario->hitobjStandOn) & HIT_ATTR_WATER_HAZARD)) {
#endif
        return FALSE;
    }
    marioChgMot(MARIO_MOTION_JUMP);
    return TRUE;
}

/**
 * On Y, turns Mario into the boat or the plane when he stands on a matching panel and has the Boat
 * or Plane Curse (retail: not while a partner use is pending).
 * Returns TRUE only when Y was pressed and no panel took it; callers (mot_stay, mot_walk) ignore it.
 */
BOOL marioChkTransform(void) {
    MarioWork* mario = marioGetPtr();
    if (!(mario->buttonTrg & PAD_BUTTON_Y)) {
#if VERSION > VERSION_DEMO_US
        return FALSE;
    }
    if (!(mario->trigFlags & MARIO_TRIG_FLAG_SHIP_PLANE_EVT_ON) && mario->partyUseDelay) {
#endif
        return FALSE;
    }
    if (mario->currMotionId == MARIO_MOTION_YOSHI) {
        return FALSE;
    }
    if (pouchCheckItem(ITEM_BOAT_CURSE) > 0 && marioChkShipPlace(mario->hitobjStandOn)) {
        marioChgMot(MARIO_MOTION_SHIP);
        return FALSE;
    }
    if (pouchCheckItem(ITEM_PLANE_CURSE) > 0 && marioChkPlanePlace(mario->hitobjStandOn)) {
        marioChgMot(MARIO_MOTION_PLANE);
        return FALSE;
    }
    return TRUE;
}

/**
 * Starts a hammer swing from stand or walk: the charged spin swing when marioChkHammer2 says so,
 * otherwise a swing on B (not in paper or tube mode). Needs a hammer and normal (not 8-bit) Mario.
 * TRUE if a swing started.
 */
BOOL marioChkItemMotion(void) {
    MarioWork* mario = marioGetPtr();
    u32 trig;
    if (mario->flags & MARIO_FLAG_IS_8_BIT) {
        return FALSE;
    }
    if (!marioGetHammerLevel()) {
        return FALSE;
    }
    if (marioChkHammer2()) {
        marioChgMot(MARIO_MOTION_HAMMER_2);
        return TRUE;
    }
    trig = mario->buttonTrg & PAD_BUTTON_B;
    if (!trig) {
        return FALSE;
    }
    if (mario->flags & (MARIO_FLAG_PAPER_MODE | MARIO_FLAG_TUBE_MODE)) {
        return FALSE;
    }
    // FAKE: repeated test of trig (already known nonzero); removing it changes the object
    if (!trig) {
        return FALSE;
    }
    marioChgMot(MARIO_MOTION_HAMMER);
    return TRUE;
}

/** Always 0: a left-over stick-rub check; marioMain's air movement would skip the stick while it returned nonzero. */
s32 L_marioChkRub(void) {
#if VERSION <= VERSION_DEMO_US
    MarioWork* mario = marioGetPtr();
#endif
    return 0;
}

/**
 * From the slit motion: rolls Mario into a tube when R is held, the stick-rub gesture counts 3,
 * he has the Tube Curse and no wall or crevice is around him. Not in 8-bit mode.
 * Returns MARIO_MOTION_ROLL when it switched, else 0.
 */
s32 marioRollChgChk(void) {
    MarioWork* mario = marioGetPtr();
    s32 b;
    s32 a;
    f32 c;
    if (mario->flags & MARIO_FLAG_IS_8_BIT) {
        return 0;
    }
    if (!(mario->buttonHold & PAD_TRIGGER_R)) {
        return 0;
    }
    if (marioGetRub(PAD_TRIGGER_R, &a, &b, &c) != TRUE) {
        return 0;
    }
    if (b < 3) {
        return 0;
    }
    if (!pouchCheckItem(ITEM_TUBE_CURSE)) {
        return 0;
    }
    if (marioSlitChkWallAround() || marioCreviceWallChk(mario->position)) {
        return 0;
    }
    if (mario->buttonHold & PAD_TRIGGER_R) {
        marioChgMot(MARIO_MOTION_ROLL);
        return MARIO_MOTION_ROLL;
    }
    return 0;
}

/**
 * Bounces Mario off an enemy he jumped on in the field (npcdrv first-strike hits) with the NPC jump motion;
 * not for Bowser in body status 2 or (retail) in the tube.
 */
void marioChgMotJump2(void) {
    MarioWork* mario = marioGetPtr();
#if VERSION <= VERSION_DEMO_US
    if (kpaGetBodyStatus() != KPA_BODY_SUBMERGED) {
#else
    if (kpaGetBodyStatus() != KPA_BODY_SUBMERGED && mario->currMotionId != MARIO_MOTION_ROLL) {
#endif
        marioChgMot(MARIO_MOTION_JUMP_NPC);
    }
}

/** TRUE for display angles strictly between 90 and 270 degrees, so mot_kaze ends facing 180 rather than 0. */
#if VERSION <= VERSION_DEMO_US
/**
 * In the demo the Flurrie motion lives here: party_cloud.c has no mot_cloud yet,
 * and the motion's post function resets the camera target.
 */
static void mot_cloud(void) {
    MarioWork* mario = marioGetPtr();

    if (mario->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) {
        mario->trigFlags &= ~MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
        mario->flags &= ~(MARIO_FLAG_HAS_INPUT_JUMP | MARIO_FLAG_IS_STEPPING | MARIO_FLAG_IS_FALLING | MARIO_FLAG_IS_JUMPING);
        mario->multiTimer = 0;
        mario->airTimer = 0;
        mario->currSubMotionId = 0;
    }
}
/** Leaving the demo's Flurrie motion: puts the camera target back on Mario (cloudResetAt). */
static void mot_cloud_post(void) {
    cloudResetAt();
}
#endif
/** TRUE for display angles strictly between 90 and 270 degrees, so mot_kaze ends facing 180 rather than 0. */
static inline BOOL marioChkBackAngle(f32 a) {
    BOOL ret = FALSE;
    if (a > 90.0f && a < 270.0f) {
        ret = TRUE;
    }
    return ret;
}

/**
 * Unused spin: turns Mario in pose M_P_2 up to 60 degrees a frame while Z is held,
 * then slows him to a stop and stands.
 * No code in the tree enters MARIO_MOTION_KAZE; marioChkSts 0x20 only tests for it.
 * Guess: a whirlwind, from the name ("kaze", wind).
 */
void mot_kaze(void) {
    MarioWork* mario = marioGetPtr();
    if (mario->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) {
        mario->trigFlags &= ~MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
        mario->flags &= ~(MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING | MARIO_FLAG_IS_STEPPING | MARIO_FLAG_HAS_INPUT_JUMP);
        mario->dispFlags |= MARIO_DISP_FLAG_NO_MAKE_DISP_DIR;
        mario->multiTimer = 0;
        mario->currSubMotionId = 0;
        mario->baseSpeed = 0.0f;
        mario->mot.kaze.spinSpeed = 0.0f;
        marioChgPose("M_P_2");
    }
    switch (mario->currSubMotionId) {
        case 0:
            mario->mot.kaze.spinSpeed += 5.0f;
            if (mario->mot.kaze.spinSpeed > 60.0f) {
                mario->mot.kaze.spinSpeed = 60.0f;
            }
            mario->displayDirection = revise360(mario->displayDirection + mario->mot.kaze.spinSpeed);
            if (mario->buttonHold & PAD_TRIGGER_Z) {
                break;
            }
            mario->displayDirectionTarget = marioChkBackAngle(mario->displayDirection) ? 180.0f : 0.0f;
            mario->currSubMotionId = 100;
            mario->multiTimer = 10;
            break;
        case 100:
            mario->mot.kaze.spinSpeed -= 6.0f;
            if (mario->mot.kaze.spinSpeed < 0.0f) {
                mario->mot.kaze.spinSpeed = 0.0f;
                mario->dispFlags &= ~MARIO_DISP_FLAG_NO_MAKE_DISP_DIR;
                marioChgMot(MARIO_MOTION_STAY);
            }
            mario->displayDirection = revise360(mario->displayDirection + mario->mot.kaze.spinSpeed);
            mario->displayDirectionTarget = marioChkBackAngle(mario->displayDirection) ? 180.0f : 0.0f;
            break;
    }
}

/**
 * Unused: lays Mario flat (rot.x 90, pose M_S_1, slit display flag cleared) until B stands him back up.
 * No code in the tree enters MARIO_MOTION_SHADOW. Guess: lying flat as a shadow, from the name.
 */
void mot_shadow(void) {
    MarioWork* mario = marioGetPtr();
    if (mario->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) {
        mario->trigFlags &= ~MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
        mario->flags &= ~(MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING | MARIO_FLAG_IS_STEPPING | MARIO_FLAG_HAS_INPUT_JUMP);
        mario->dispFlags &= ~MARIO_DISP_FLAG_SLIT;
        mario->multiTimer = 0;
        mario->currSubMotionId = 0;
        marioChgPose("M_S_1");
        mario->rot.x = 90.0f;
        mario->rot.y = 0.0f;
        mario->rot.z = 0.0f;
    }
    switch (mario->currSubMotionId) {
        case 0:
            if (mario->buttonTrg & PAD_BUTTON_B) {
                mario->rot.x = 0.0f;
                mario->rot.z = 0.0f;
                mario->multiTimer = 0;
                marioChgMot(MARIO_MOTION_STAY);
            }
            break;
    }
}

/** Empty motion slot MARIO_MOTION_PARTY_USE; nothing in the tree enters it. */
void mot_partyUse(void) {
    marioGetPtr();
}

/** Empty last motion slot (MARIO_MOTION_DUMMY). */
void mot_dummy(void) {
    marioGetPtr();
}

/**
 * Falling into a pit in background mode (marioBottomless):
 * Mario drops from above the screen, glides back to where he entered (returnPos; fixed spots on win_01,
 * win_03 and tik_01 when unset), lands with a thud, loses 1 HP and stands.
 */
void mot_bottomless(void) {
    MarioWork* mario = marioGetPtr();
    Vec pos;
    f32 y;
    f32 sx, sy, sz;
    f32 x;
    f32 step;
    f32 z;
    void* hit;

    if (mario->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) {
        mario->trigFlags &= ~MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
        mario->baseSpeed = 0.0f;
        mario->airTimer = 0;
        mario->currSubMotionId = 0;
        // x, y and z are set below only on win_03, win_01 and tik_01, then used on every map.
        if (mario->returnPos.y < -2000.0f) {
            if (!strcmp(gp->currentMapName, "win_03")) {
                x = 250.0f;
                y = 13.0f;
                z = mario->position.z;
            } else if (!strcmp(gp->currentMapName, "win_01")) {
                x = 220.0f;
                y = 1.0f;
                z = mario->position.z;
            } else if (!strcmp(gp->currentMapName, "tik_01")) {
                x = 100.0f;
                y = 120.0f;
                z = mario->position.z;
            }
        } else {
            x = mario->returnPos.x;
            y = mario->returnPos.y;
            z = mario->returnPos.z;
        }
        pos = (Vec){x, 300.0f + y, z};
        step = 10.0f;
        while (TRUE) {
            marioGetScreenPos(pos, &sx, &sy, &sz);
            if (!marioChkInScreen(sx, sy)) {
                break;
            }
            pos.y += step;
        }
        mario->position.y = pos.y;
        mario->mot.bottomless.speed = 8.0f;
        mario->multiTimer = (s16)(distABf(x, z, mario->position.x, mario->position.z) / mario->mot.bottomless.speed);
        mario->mot.bottomless.angle = angleABf(mario->position.x, mario->position.z, x, z);
    }
    switch (mario->currSubMotionId) {
        case 0:
            movePos(&mario->position.x, &mario->position.z, mario->mot.bottomless.speed, mario->mot.bottomless.angle);
            mario->camTarget.x = mario->position.x;
            mario->camTarget.z = mario->position.z;
            if (--mario->multiTimer > 0) {
                break;
            }
            mario->currSubMotionId = 2;
            mario->flags |= MARIO_FLAG_STOP_AIR_MOVEMENT;
            marioSetFallPara();
            break;
        case 2:
            mario->hitobjStandOn = NULL;
            step = marioGetFallSpd();
            if (step < -10.0f) {
                step = -10.0f;
            }
            hit = marioChkLandon(&y, step);
            if (hit == NULL) {
                mario->position.y += step;
            } else {
                psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_VOICE_MARIO_DAMAGED1), &mario->position);
                marioChgPose("M_D_2");
                psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MARIO_FALL_CRUSH1), &mario->position);
                marioDmgRetQuakeOn();
                mario->hitobjStandOn = hit;
                mario->hitobjJumpFrom = NULL;
                mario->position.y = y;
            }
            if (mario->hitobjStandOn == NULL) {
                break;
            }
        case 10:
            mario->multiTimer = 50;
            mario->currSubMotionId = 11;
        case 11:
            if (--mario->multiTimer > 0) {
                break;
            }
            pouchAddHP(-1);
            if (pouchGetHP() <= 0) {
                mario->currSubMotionId = 13;
                marioUpdateCamPos();
                break;
            }
            marioChgPose("M_U_3");
            mario->multiTimer = 12;
            mario->currSubMotionId = 12;
        case 12:
            if (--mario->multiTimer > 0) {
                break;
            }
            marioChgMot(MARIO_MOTION_STAY);
            marioUpdateCamPos();
            break;
    }
}

/**
 * After marioBottomless moves Mario out of a pit: cancels the slit and stands him.
 * Its second step (marioReset, partner and follower moved to Mario) only runs
 * if MARIO_FLAG_CHG_MOT_DISABLED blocked that stand change.
 */
void mot_forceReset(void) {
    MarioWork* mario = marioGetPtr();
    PartyEntry* party;
    if (mario->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) {
        mario->trigFlags &= ~MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
        mario->currSubMotionId = 0;
    }
    switch (mario->currSubMotionId) {
        case 0:
            marioSlitForceCancel();
            marioChgMot(MARIO_MOTION_STAY);
            mario->currSubMotionId = 1;
            break;
        case 1:
            marioReset();
            party = partyGetPtr(marioGetPartyId());
            if (party) {
                party->position = mario->position;
            }
            party = partyGetPtr(marioGetExtraPartyId());
            if (party) {
                party->position = mario->position;
            }
            break;
    }
}
