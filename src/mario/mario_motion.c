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

typedef struct MotCaseEntry {
    s32 unk0;
    s32 type;
} MotCaseEntry;

void mot_stay(void);
void mot_stay_post(void);
void mot_walk(void);
void mot_dash(void);
void mot_jump(void);
void mot_jumpNpc(void);
#ifdef VERSION_G8ME01
void N_mot_jumpNpc_post(void);
#endif
void mot_jumpSw(void);
void mot_jumpStand(void);
void mot_jumpSmall(void);
void mot_fall(void);
void mot_fall_post(void);
void mot_upstairs(void);
void mot_land(void);
void mot_talk(void);
void mot_getItem(void);
void mot_hip(void);
void mot_hip_post(void);
void mot_hammer(void);
void mot_hammer2(void);
void mot_ham2_post(void);
void mot_jabara(void);
void mot_jabara_post(void);
void mot_slit(void);
void mot_slit_post(void);
void mot_roll(void);
void mot_roll_post(void);
void mot_kaze(void);
void mot_plane(void);
void mot_plane_post(void);
void mot_ship(void);
void mot_ship_post(void);
void mot_cloud(void);
void mot_vivian(void);
void mot_vivian_post(void);
void mot_dokan(void);
void mot_grasp(void);
void mot_damage(void);
void mot_damageToge(void);
void mot_bottomless(void);
void mot_forceReset(void);
void mot_shadow(void);
void mot_partyUse(void);
void kpa_swim(void);
void kpa_powup(void);
void kpa_powdown(void);
void mot_dummy(void);
void mot_fall_roll(void);

void marioSlitForceCancel(void);
void movePos(f32* x, f32* z, f32 speed, f32 angle);
s32 psndSFXOn_3D(s32 id, Vec* position);
void* marioChkLandon(f32* y, f32 spd);
#define PCHK(x) pouchCheckItem(x)
void marioDmgRetQuakeOn(void);
BOOL marioGetRub(s32 mask, s32* a, s32* b, s32* c);
BOOL marioCreviceWallChk(Vec pos);
s32 marioGetHammerLevel(void);
BOOL marioChkHammer2(void);
BOOL marioChkShipPlace(void* hit);
BOOL marioChkPlanePlace(void* hit);
MotCaseEntry* caseCheckHitObj(void* hit);
void* fbatNpcTalkCheck(void);
void peach_talk(void);
void effKemuriEntry(s32 type, f32 x, f32 y, f32 z, f32 w);
void yoshi_jumpStand(void);
void marioSlitReenter(void);
void motSlitCancel(void);
void N_marioSlitSetAfterLand(void);
void kpaClearHitobjRide(void);
void* kpaChkHitobjRide(void);
BOOL getRollEvtFlag(void);
BOOL marioPriCheckJabara(void);
BOOL kpaHipAttackOk(void);
BOOL marioChkHipAttack(void);
BOOL vivianGetStatus(void);
s32 marioShipChanging(void);
void marioReInit_ship(void);
void marioResetRollSpd(void);
void mario_plane_cancel(void);
void allPartyForceRideOn(void);
void marioLandOn(void);
void marioClearJumpPara(void);
void marioChgRollMotion(void);

MarioMotTblEntry marioMotTbl[] = {
    {mot_stay, NULL, mot_stay_post},
    {mot_walk, NULL, NULL},
    {mot_dash, NULL, NULL},
    {mot_jump, NULL, NULL},
#ifdef VERSION_G8ME01
    {mot_jumpNpc, NULL, N_mot_jumpNpc_post},
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
    {mot_cloud, NULL, cloudResetAt},
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

MarioJumpData marioJumpData = {
    {8.0f, -0.84f, 0.08f, -0.02f},
    {-0.1f, -0.2f, 0.024f, -0.004f},
    32.0f,
    -18.0f,
};

void marioChgMoveMotion(void) {
    MarioWork* mario = marioGetPtr();
    if (mario->currMotionId == MARIO_MOTION_PLANE) {
        mario_plane_cancel();
        return;
    }
    if (mario->currMotionId == MARIO_MOTION_ROLL) {
        allPartyForceRideOn();
        return;
    }
#ifdef VERSION_G8ME01
    if ((u16)(mario->currMotionId - MARIO_MOTION_DAMAGE) <= 1) {
        return;
    }
#endif
    if (mario->currMotionId == MARIO_MOTION_YOSHI) {
        return;
    }
    if (!mario->baseSpeed) {
        marioChgStayMotion();
    }
    if (mario->baseSpeed > mario->mBaseWalkSpeed) {
        marioChgMot(MARIO_MOTION_DASH);
    } else {
        marioChgMot(MARIO_MOTION_WALK);
    }
}

void marioChgStayMotion(void) {
    MarioWork* mario = marioGetPtr();
    if (mario->currMotionId != MARIO_MOTION_SHIP && mario->currMotionId != MARIO_MOTION_YOSHI &&
        mario->currMotionId != MARIO_MOTION_SLIT && mario->currMotionId != MARIO_MOTION_ROLL) {
        marioChgMot(MARIO_MOTION_STAY);
    }
}

void marioChgTalkMotion(void) {
    marioChgMot(MARIO_MOTION_TALK);
}

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

void marioChgShipMotion(void) {
    marioReInit_ship();
}

void marioChgRollMotion(void) {
    MarioWork* mario = marioGetPtr();
    mario->currSubMotionId = 50;
    mario->currMotionId = MARIO_MOTION_ROLL;
}

void marioChgJumpStandMotion(f32 v) {
    MarioWork* mario = marioGetPtr();
    camFollowYOn();
    if (marioChkSts(0x10)) {
        mario->mot.jump.standHeight = v;
        effKemuriEntry(0, mario->position.x, mario->position.y, mario->position.z, 0.0f);
        marioChgMot(MARIO_MOTION_JUMP_STAND);
    } else if (mario->currMotionId == MARIO_MOTION_YOSHI) {
        mario->mot.jump.standHeight = v;
        yoshi_jumpStand();
    } else {
        mario->mot.jump.standHeight = v;
        marioChgMot(MARIO_MOTION_JUMP_STAND);
    }
}

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

BOOL marioChkTalkable(void) {
    MarioWork* mario = marioGetPtr();
    BOOL ret = FALSE;
    PartyEntry* party = partyGetPtr(marioGetPartyId());
    if (party != NULL && (s32)(u8)party->currentMemberId == 4 && (party->flags & 0x100)) {
        return FALSE;
    }
    switch (mario->currMotionId) {
        case MARIO_MOTION_STAY:
        case MARIO_MOTION_WALK:
        case MARIO_MOTION_DASH:
            if (!(mario->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) && marioBgmodeChk() != TRUE && !(mario->dispFlags & 0x1000000)) {
                ret = TRUE;
            }
            break;
    }
    if (vivianGetStatus()) {
        ret = FALSE;
    }
    return ret;
}

void marioBoots(void) {
    MarioWork* mario = marioGetPtr();
    u32 flags = mario->flags;
    if (flags & 0x80000000) {
        return;
    }
    if (flags & 0x100000) {
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
    if (!(flags & 0x80000)) {
        return;
    }
    if (!marioChkHipAttack()) {
        return;
    }
    marioChgMot(MARIO_MOTION_HIP);
}

void marioMotion(void) {
    MarioWork* mario = marioGetPtr();
    mario->hitobjHammer = NULL;
    do {
        marioMotTbl[mario->currMotionId].main();
    } while (mario->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION);
}

void marioChgMot(MarioMotion id) {
    MarioWork* mario = marioGetPtr();
    if (mario->flags & MARIO_FLAG_CHG_MOT_DISABLED) {
        return;
    }
    if (mario->prevMotionId == MARIO_MOTION_HIP || mario->prevMotionId == MARIO_MOTION_HIP_2) {
        if (mario->dispFlags & 0x8000000) {
            mario->dispFlags &= ~0x8000000;
        }
        if (mario->dispFlags & 4) {
            mario->dispFlags &= ~4;
        }
    }
    mario->prevMotionId = mario->currMotionId;
    mario->currMotionId = id;
    if (marioMotTbl[mario->prevMotionId].post) {
        marioMotTbl[mario->prevMotionId].post();
    }
    mario->trigFlags |= MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
}

void marioChgMotSub(MarioMotion id, s32 callPost) {
    MarioWork* mario = marioGetPtr();
    if (mario->flags & MARIO_FLAG_CHG_MOT_DISABLED) {
        return;
    }
    if (mario->prevMotionId == MARIO_MOTION_HIP || mario->prevMotionId == MARIO_MOTION_HIP_2) {
        if (mario->dispFlags & 0x8000000) {
            mario->dispFlags &= ~0x8000000;
        }
        if (mario->dispFlags & 4) {
            mario->dispFlags &= ~4;
        }
    }
    mario->prevMotionId = mario->currMotionId;
    mario->currMotionId = id;
    if (callPost) {
        if (marioMotTbl[mario->prevMotionId].post) {
            marioMotTbl[mario->prevMotionId].post();
        }
    } else if (mario->prevMotionId == MARIO_MOTION_JABARA) {
        if (marioMotTbl[mario->prevMotionId].post) {
            marioMotTbl[mario->prevMotionId].post();
        }
    }
    mario->trigFlags |= MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
}

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

void marioClearJumpPara(void) {
    MarioWork* mario = marioGetPtr();
    mario->jumpVelocityY = 0.0f;
    mario->jumpAccelerationY = 0.0f;
    mario->jumpJerkY = 0.0f;
    mario->jumpSnapY = 0.0f;
}

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
    if ((s32)mario->currSubMotionId < 10) {
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
        if (!marioChkSts(0x10)) {
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

void marioFall(void) {
    MarioWork* mario = marioGetPtr();
    s32 rub;
    f32 spd;
    s32 b;
    s32 a;
    s32 c;
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
    if (marioChkSts(0x10) == TRUE) {
        mot_fall_roll();
        return;
    }
#ifdef VERSION_G8ME01
    if (!(mario->trigFlags & MARIO_TRIG_FLAG_JABARA_MOVE_CHK) && (mario->trigFlags & MARIO_TRIG_FLAG_JABARA_CHK)) {
        rub = marioGetRub(0x100, &a, &b, &c);
        if (mario->buttonHold & PAD_BUTTON_A) {
            mario->trigFlags |= MARIO_TRIG_FLAG_START_JABARA_SPIN;
        }
    }
#else
    rub = marioGetRub(0x100, &a, &b, &c);
#endif
    spd = marioGetFallSpd();
    hit = marioChkLandon(&y, spd);
    if (hit) {
        mario->hitobjStandOn = hit;
        mario->hitobjJumpFrom = NULL;
        if (hitGetAttr(hit) & 0x200) {
            mario->position.y += spd;
        } else {
            mario->position.y = y;
#ifdef VERSION_G8ME01
            if (!(mario->trigFlags & MARIO_TRIG_FLAG_JABARA_MOVE_CHK) && (mario->trigFlags & MARIO_TRIG_FLAG_JABARA_CHK) && marioPriCheckJabara() && a != -1 && rub >= 0) {
#else
            if (marioPriCheckJabara() && a != -1 && rub >= 0) {
#endif
                camFollowYOn();
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

void marioLandOn(void) {
    MarioWork* mario = marioGetPtr();
    MapObjectEntry* mobj;
    Vec pos;
    s32 attr;
    u32 flags;
    s32 sfx;
    void* stand;

    if (!(hitGetAttr(mario->hitobjStandOn) & 0x200)) {
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
        if (attr & 0x100) {
            sfx = 0x150;
        } else if (attr & 0x200000) {
            sfx = 0x151;
        } else if (attr & 0x100000) {
            sfx = 0x152;
        } else if (attr & 0x1000) {
            sfx = 0x153;
        } else if (attr & 0x400000) {
            sfx = 0x154;
        } else {
            sfx = 0x14f;
        }
        psndSFXOn_3D(sfx, &pos);
    }
    if (mario->flags & MARIO_FLAG_TUBE_MODE) {
        mario->flags &= ~(MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING);
        mario->hitobjJumpFrom = NULL;
        return;
    }
    mario->mot.jump.dist = __fabsf(distABf(mario->position.x, mario->position.z, mario->lastGroundPos.x, mario->lastGroundPos.z));
    attr = hitGetAttr(mario->hitobjStandOn);
    if (mario->currMotionId != MARIO_MOTION_DAMAGE && (attr & 0x200)) {
        set_damage_root_ypos(mario->position.y);
        marioChgMot(MARIO_MOTION_DAMAGE);
        return;
    }
    if (mario->currMotionId != MARIO_MOTION_DAMAGE_TOGE && (attr & 0x800)) {
        set_damage_root_ypos(mario->position.y);
        marioChgMot(MARIO_MOTION_DAMAGE_TOGE);
        return;
    }
    if (hitGetAttr(mario->hitobjStandOn) & 0x80000000) {
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
            marioSlitReenter();
            return;
        }
        if ((mario->dispFlags & 0x1000000) && !marioChkSlitEnd()) {
            motSlitCancel();
        }
        return;
    }
#ifdef VERSION_G8ME01
    if (mario->controlStickSensitivity > 0.0f && !(mario->trigFlags & MARIO_TRIG_FLAG_START_JABARA_SPIN) && !(mario->flags & MARIO_FLAG_FORCED_MOVEMENT)) {
#else
    if (mario->controlStickSensitivity > 0.0f && !(mario->flags & MARIO_FLAG_FORCED_MOVEMENT)) {
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
        if (!(mario->dispFlags & 0x1000000)) {
#ifdef VERSION_G8ME01
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
        N_marioSlitSetAfterLand();
        return;
    }
    flags = mario->flags;
    if (flags & 0x1000000) {
        marioChgRollMotion();
        return;
    }
    if (mario->characterId == CHARACTER_BOWSER) {
        marioChgMot(MARIO_MOTION_STAY);
        quake_kpaLandOn();
        return;
    }
    if (mario->currMotionId != MARIO_MOTION_SLIT) {
        if (!(flags & 0x20)) {
            marioChgMot(MARIO_MOTION_STAY);
            return;
        }
        marioChgMot(mario->baseSpeed > mario->mBaseWalkSpeed ? MARIO_MOTION_DASH : MARIO_MOTION_WALK);
        return;
    }
    if (flags & 0x100000) {
        marioSlitReenter();
        return;
    }
    if ((mario->dispFlags & 0x1000000) && !marioChkSlitEnd()) {
        motSlitCancel();
    }
}

void mot_talk(void) {
    MarioWork* mario = marioGetPtr();
    void* npc;
    f32 camYaw;
    f32 angle;
    void* fbat;

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
        fbat = fbatGetPointer()->npc;
        if (fbat != NULL) {
            marioChgPose(((NpcEntry*)fbat)->checkPose);
        }
        if (!strcmp(mario->animName, "M_I_2")) {
            mario->multiTimer = 60;
            mario->currSubMotionId = 10;
        }
    }
    switch ((s32)mario->currSubMotionId) {
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

BOOL marioChkJump(void) {
    MarioWork* mario = marioGetPtr();
    MotCaseEntry* ce;
    if (!(mario->buttonTrg & PAD_BUTTON_A)) {
        return FALSE;
    }
    if (mario->currMotionId == MARIO_MOTION_YOSHI) {
        return FALSE;
    }
    if (mario->hitobjFrontA) {
        if ((hitGetAttr(mario->hitobjFrontA) & 0x80000000) && mobjGetHint(mobjHitObjPtrToPtr(mario->hitobjFrontA)) == 1) {
            return FALSE;
        }
        ce = caseCheckHitObj(mario->hitobjFrontA);
        if (ce && (ce->type == 9 || ce->type == 4 || ce->type == 12)) {
            return FALSE;
        }
    }
    if (mario->hitobjFront) {
        if ((hitGetAttr(mario->hitobjFront) & 0x80000000) && mobjGetHint(mobjHitObjPtrToPtr(mario->hitobjFront)) == 1) {
            return FALSE;
        }
        ce = caseCheckHitObj(mario->hitobjFront);
        if (ce && (ce->type == 9 || ce->type == 4 || ce->type == 12)) {
            return FALSE;
        }
    }
    if (mario->hitobjStandOn) {
        if ((hitGetAttr(mario->hitobjStandOn) & 0x80000000) && mobjGetHint(mobjHitObjPtrToPtr(mario->hitobjStandOn)) == 1) {
            return FALSE;
        }
        ce = caseCheckHitObj(mario->hitobjStandOn);
        if (ce && ce->type == 5) {
            return FALSE;
        }
    }
    if (fbatNpcTalkCheck()) {
        return FALSE;
    }
    marioChgMot(MARIO_MOTION_JUMP);
    return TRUE;
}

BOOL marioChkTransform(void) {
    MarioWork* mario = marioGetPtr();
    if (!(mario->buttonTrg & PAD_BUTTON_Y)) {
        return FALSE;
    }
    if (!(mario->trigFlags & MARIO_TRIG_FLAG_SHIP_PLANE_EVT_ON) && mario->partyUseDelay) {
        return FALSE;
    }
    if (mario->currMotionId == MARIO_MOTION_YOSHI) {
        return FALSE;
    }
    if (PCHK(5) > 0 && marioChkShipPlace(mario->hitobjStandOn)) {
        marioChgMot(MARIO_MOTION_SHIP);
        return FALSE;
    }
    if (PCHK(4) > 0 && marioChkPlanePlace(mario->hitobjStandOn)) {
        marioChgMot(MARIO_MOTION_PLANE);
        return FALSE;
    }
    return TRUE;
}

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
    if (!trig) {
        return FALSE;
    }
    marioChgMot(MARIO_MOTION_HAMMER);
    return TRUE;
}

s32 L_marioChkRub(void) {
    return 0;
}

s32 marioRollChgChk(void) {
    MarioWork* mario = marioGetPtr();
    s32 b;
    s32 a;
    s32 c;
    if (mario->flags & MARIO_FLAG_IS_8_BIT) {
        return 0;
    }
    if (!(mario->buttonHold & PAD_TRIGGER_R)) {
        return 0;
    }
    if (marioGetRub(0x20, &a, &b, &c) != TRUE) {
        return 0;
    }
    if (b < 3) {
        return 0;
    }
    if (!PCHK(3)) {
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

void marioChgMotJump2(void) {
    MarioWork* mario = marioGetPtr();
    if (kpaGetBodyStatus() != 2 && mario->currMotionId != MARIO_MOTION_ROLL) {
        marioChgMot(MARIO_MOTION_JUMP_NPC);
    }
}

static inline BOOL marioChkBackAngle(f32 a) {
    BOOL ret = FALSE;
    if (a > 90.0f && a < 270.0f) {
        ret = TRUE;
    }
    return ret;
}

void mot_kaze(void) {
    MarioWork* mario = marioGetPtr();
    if (mario->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) {
        mario->trigFlags &= ~MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
        mario->flags &= ~(MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING | MARIO_FLAG_IS_STEPPING | MARIO_FLAG_HAS_INPUT_JUMP);
        mario->dispFlags |= 0x100;
        mario->multiTimer = 0;
        mario->currSubMotionId = 0;
        mario->baseSpeed = 0.0f;
        mario->mot.kaze.spinSpeed = 0.0f;
        marioChgPose("M_P_2");
    }
    switch ((s32)mario->currSubMotionId) {
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
                mario->dispFlags &= ~0x100;
                marioChgMot(MARIO_MOTION_STAY);
            }
            mario->displayDirection = revise360(mario->displayDirection + mario->mot.kaze.spinSpeed);
            mario->displayDirectionTarget = marioChkBackAngle(mario->displayDirection) ? 180.0f : 0.0f;
            break;
    }
}

void mot_shadow(void) {
    MarioWork* mario = marioGetPtr();
    if (mario->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) {
        mario->trigFlags &= ~MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
        mario->flags &= ~(MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING | MARIO_FLAG_IS_STEPPING | MARIO_FLAG_HAS_INPUT_JUMP);
        mario->dispFlags &= ~0x1000000;
        mario->multiTimer = 0;
        mario->currSubMotionId = 0;
        marioChgPose("M_S_1");
        mario->rot.x = 90.0f;
        mario->rot.y = 0.0f;
        mario->rot.z = 0.0f;
    }
    switch ((s32)mario->currSubMotionId) {
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

void mot_partyUse(void) {
    marioGetPtr();
}

void mot_dummy(void) {
    marioGetPtr();
}

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
    switch ((s32)mario->currSubMotionId) {
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
                psndSFXOn_3D(0xb9, &mario->position);
                marioChgPose("M_D_2");
                psndSFXOn_3D(0x197, &mario->position);
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

void mot_forceReset(void) {
    MarioWork* mario = marioGetPtr();
    PartyEntry* party;
    if (mario->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) {
        mario->trigFlags &= ~MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
        mario->currSubMotionId = 0;
    }
    switch ((s32)mario->currSubMotionId) {
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
