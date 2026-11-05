// Yoshi: partner init, follow, the ride (Mario on Yoshi's back) and the flutter jump.

#include "version.h"
#include "dolphin/types.h"
#include "eff/eff_magic1_n64.h"
#include "driver/animdrv.h"
#include "driver/effdrv.h"
#include "eff/n64/eff_sweat_n64.h"
#include "driver/hitdrv.h"
#include "mario/mario_sbr.h"
#include "mario/mario.h"
#include "mario/mario_motion.h"
#include "mario/mario_party.h"
#include "memory.h"
#include "party.h"
#include "system.h"
#include <dolphin/mtx.h>
#include <string.h>
#include "party/party_hit.h"
#include "mario/mario_hit.h"
#include "mot/mot_damage.h"
#include "mot/mot_jump.h"
#include "mario/mario_cam.h"
#include <dolphin/pad.h>
#include "party/party_yoshi.h"
#include "pmario_sound.h"
#include "sfx_name_word.h"

typedef struct YoshiWork {
    u8 pad0[0x4];         // 0x0
    s32 dir;              // 0x4
    s32 count;            // 0x8
    f32 angle;            // 0xC
    s32 timer;            // 0x10
    EffectEntry* effect;  // 0x14
} YoshiWork;


extern HitEntry* dou10_yoko_yari2(void* out);

#if VERSION <= VERSION_DEMO_JP
static f32 yoshiSignTbl[16] = {
    -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f,
};
static f32 marioJumpStandData[10] = {1.52f, -0.188f, 2.925f, -0.847f, -0.1f, -0.4f, 0.024f, -0.004f, 0.125f, 0.7f};
#endif

f32 ofs_PYS_S_2[52] = {
    0.0f, -0.011f, -0.042f, -0.087f, -0.142f, -0.202f, -0.262f, -0.317f, -0.362f, -0.393f, -0.404f, -0.404f, -0.404f,
    -0.404f, -0.404f, -0.387f, -0.341f, -0.276f, -0.202f, -0.128f, -0.063f, -0.017f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, -0.011f, -0.042f, -0.087f, -0.142f, -0.202f, -0.262f, -0.317f, -0.362f, -0.393f, -0.404f, -0.404f, -0.404f,
    -0.404f, -0.404f, -0.387f, -0.341f, -0.276f, -0.202f, -0.128f, -0.063f, -0.017f, 0.0f, 0.0f, 0.0f, 0.0f,
};

#if VERSION >= VERSION_JP
static f32 marioJumpStandData[10] = {1.52f, -0.188f, 2.925f, -0.847f, -0.1f, -0.4f, 0.024f, -0.004f, 0.125f, 0.7f};
#endif

f32 ofs_PYS_R_2[10] = {0.0f, 0.0f, 0.366f, 0.555f, 0.38f, 0.0f, 0.0f, 0.366f, 0.555f, 0.38f};
f32 ofs_PYS_F_1[10] = {0.0f, 0.0f, 0.433f, 0.557f, 0.512f, 0.0f, 0.0f, 0.433f, 0.557f, 0.512f};

// Yoshi's work (PartyEntry.useStruct). A macro, not an inline function: as a function the object changes.
#define YOSHI_WORK(party) ((YoshiWork*)(party)->useStruct)

static inline void yoshiAddFloorMove(PartyEntry* party) {
    if (party->hitobjStandOn) {
        party->position.x += party->floorMove.x;
        party->position.y += party->floorMove.y;
        party->position.z += party->floorMove.z;
    }
}

// Mario rides Yoshi: he follows Yoshi's position and direction.
static inline void yoshiSyncMario(MarioWork* mario, PartyEntry* party) {
    mario->camTarget.x = party->position.x;
    mario->camTarget.z = party->position.z;
    mario->position.x = party->position.x;
    mario->position.z = party->position.z;
    mario->displayDirection = party->displayDirection;
    mario->displayDirectionTarget = party->displayDirectionTarget;
}

f32 evt_yoshi_spd;
s32 evt_yoshi_frm;
f32 jumpheight;

void inertia(PartyEntry* party);
void searchGround(PartyEntry* party);
void searchGround2(PartyEntry* party);
void checkWall(PartyEntry* party, f32 spd, f32 dir);
void getStick(PartyEntry* party, f32* angle, f32* speed);

#if VERSION <= VERSION_DEMO_JP
void yoshiFlyMove(PartyEntry* party, f32* angle);
#else
void yoshiFlyMove(PartyEntry* party);
#endif

/**
 * Scripted flutter for evt_party_yoshi_fly: stores the length (ms to frames) in evt_yoshi_frm.
 * Only its sign is read: 0 or more makes yoshi_use start a flutter in event mode, and landing resets it to -1.
 * The script command already converts its argument to frames, so the value is converted twice.
 */
void evt_set_yoshi_frm(s32 msec) {
    evt_yoshi_frm = sysMsec2Frame(msec);
}

/** Sets the speed Yoshi moves at while a script drives him (evt_party_yoshi_fly argument 1). */
void evt_set_yoshi_spd(f32 spd) {
    evt_yoshi_spd = spd;
}

/**
 * Returns evt_yoshi_frm; evt_party_yoshi_fly keeps waiting while it is 0
 * or more (the scripted flutter has not landed yet).
 */
s32 evt_get_yoshi_frm(void) {
    return evt_yoshi_frm;
}

/**
 * Records the hit object Yoshi stands on (NULL when airborne) and,
 * while Mario rides on the ground and is not jumping or falling,
 * makes it Mario's stand-on object too so moving floors carry both.
 */
void L_sweatEntry(PartyEntry* party, void* hit) {
    party->hitobjStandOn = hit;
    if (yoshiGetStatus() == YOSHI_STATUS_RIDING && !(party->playerPtr->flags & (MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING | MARIO_FLAG_IS_STEPPING | MARIO_FLAG_HAS_INPUT_JUMP))) {
        party->playerPtr->hitobjStandOn = hit;
    }
}

/**
 * Keeps the sweat-drop effect spawned during the flutter jump above Yoshi (18 up) and deletes it
 * when its 20-frame timer runs out. Runs every frame from yoshi_use.
 */
void sweatMain(PartyEntry* party) {
    if (party->useStruct && YOSHI_WORK(party)->effect) {
        f32 x = party->position.x;
        f32 y = party->position.y + 18.0f;
        f32 z = party->position.z;
        ((EffSweatN64Work*)YOSHI_WORK(party)->effect->userdata)->position.x = x;
        ((EffSweatN64Work*)YOSHI_WORK(party)->effect->userdata)->position.y = y;
        ((EffSweatN64Work*)YOSHI_WORK(party)->effect->userdata)->position.z = z;
        if (--YOSHI_WORK(party)->timer <= 0) {
            effDelete(YOSHI_WORK(party)->effect);
            YOSHI_WORK(party)->effect = NULL;
        }
    }
}

/**
 * Ride state from Yoshi's use motion: 0 not riding (or the partner is not Yoshi), 1 riding on the ground,
 * 2 flutter jump, 3 jump-stand bounce, 4 falling (after a flutter or a jump-stand bounce),
 * 5 anything else (mounting, getting off, step-up, water). The early demos have only 0, 1, 2 and 3 for everything else.
 */
s32 yoshiGetStatus(void) {
    PartyEntry* party = partyGetPtr(marioGetPartyId());
#if VERSION <= VERSION_DEMO_JP
    s32 status = 3;
#else
    s32 status = YOSHI_STATUS_OTHER;
#endif
    u8 motion;

    if (party == NULL || party->currentMemberId != PARTY_MEMBER_YOSHI || !(party->flags & PARTY_FLAG_IS_BEING_USED)) {
        return YOSHI_STATUS_INACTIVE;
    }
    motion = party->useMotionId;
    if (motion >= 10 && motion <= 13) {
        status = YOSHI_STATUS_RIDING;
    } else if (motion >= 20 && motion <= 22) {
        status = YOSHI_STATUS_FLUTTER;
    } else if (motion >= 100 && motion <= 101) {
#if VERSION >= VERSION_JP
        status = YOSHI_STATUS_OTHER;
    } else if (motion >= 60 && motion <= 65) {
#endif
        status = YOSHI_STATUS_STAND_JUMP;
#if VERSION >= VERSION_JP
    } else if (motion >= 50 && motion <= 52) {
        status = YOSHI_STATUS_FALL;
#endif
    }
    return status;
}

/**
 * Puts Mario back on Yoshi without the mount animation (motion 11) when Mario's motion is re-initialised
 * while riding (mario.c). Allocates Yoshi's work area if needed and makes the extra partner follow the ride.
 */
void marioRideYoshi(void) {
    PartyEntry* party = partyGetPtr(marioGetPartyId());
    PartyEntry* extra;

    party->trigFlags &= ~PARTY_TRIG_CHG_RUN_MODE;
    party->trigFlags &= ~PARTY_TRIG_CHG_MOVE_MODE;
    party->trigFlags &= ~PARTY_TRIG_CHG_MOT;
    party->flags |= PARTY_FLAG_IS_BEING_USED;
    party->runMode = PARTY_RUN_USE;
    party->useMotionId = 11;
    if (party->useStruct == NULL) {
        party->useStruct = __memAlloc(HEAP_DEFAULT, sizeof(YoshiWork));
        memset(party->useStruct, 0, sizeof(YoshiWork));
        YOSHI_WORK(party)->effect = NULL;
    }
    extra = partyGetPtr(marioGetExtraPartyId());
    if (extra) {
        party_force_ride_yoshi(extra);
    }
}

/**
 * Starts the dismount (motion 100) when Mario is riding on the ground.
 * Returns TRUE if it started; used by the cancel in evt_mario.c,
 * the ride script command in evt_party.c and party.c.
 */
s32 marioGetoffYoshi(void) {
    PartyEntry* party = partyGetPtr(marioGetPartyId());

    if (!(party->flags & PARTY_FLAG_IS_BEING_USED)) {
        return 0;
    }
    if (yoshiGetStatus() == YOSHI_STATUS_RIDING) {
        party->useMotionId = 100;
        return 1;
    }
    return 0;
}

/** Clears Mario's and the extra partner's display offsets (the bob added while riding). */
// Descriptive name (inline helper; not in the symbol maps).
static inline void resetOfs(PartyEntry* party) {
    PartyEntry* extra;

    MarioWork* player = party->playerPtr;

    player->dispOfs = (Vec){0.0f, 0.0f, 0.0f};
    player->dispOfs2 = (Vec){0.0f, 0.0f, 0.0f};
    extra = partyGetPtr(marioGetExtraPartyId());
    if (extra) {
        extra->dispOfs = (Vec){0.0f, 0.0f, 0.0f};
    }
}

/** Switches Yoshi's use motion unless he is still moving under Mario to mount (motion 5); FALSE if refused. */
// Descriptive name (inline helper; not in the symbol maps).
static inline u32 chgMotion(PartyEntry* party, u8 motion) {
    if (party->useMotionId == 5) {
        return FALSE;
    }
    party->useMotionId = motion;
    return TRUE;
}

/**
 * Applies one frame of ride gravity: starts the fall curve the first time,
 * then integrates the jerk and acceleration into party->gravity, capped at -15 per frame.
 */
// Descriptive name (inline helper; not in the symbol maps).
static inline void fall(PartyEntry* party) {
    if (!(party->flags & PARTY_FLAG_IS_FALLING)) {
        party->flags |= PARTY_FLAG_IS_FALLING;
        party->gravity = -0.4f;
        party->work.common.accelY = -0.4f;
        party->work.yoshi.gravityAccel = 0.0f;
        party->accelY = 0.0f;
    }
    party->work.yoshi.gravityAccel += party->accelY;
    party->work.common.accelY += party->work.yoshi.gravityAccel;
    party->gravity += party->work.common.accelY;
    if (party->gravity < -15.0f) {
        party->gravity = -15.0f;
    }
}

/**
 * Ends the ride at once (no dismount animation):
 * resets the offsets and Mario's pose, frees Yoshi's work area and returns him to follow mode.
 * Called on damage, after a field battle (marioFBattlePost) and from yoshi_exit.
 * In JP and later a spike hit (MARIO_MOTION_DAMAGE_TOGE) instead puts Yoshi
 * and the extra partner in run mode 0xd.
 */
void marioYoshiForceCancel(void) {
    MarioWork* mario = marioGetPtr();
    PartyEntry* party = partyGetPtr(marioGetPartyId());
    PartyEntry* extra;

    if (yoshiGetStatus() == YOSHI_STATUS_INACTIVE) {
        return;
    }
    resetOfs(party);
    marioChgPose("M_S_1");
    if (party->flags & PARTY_FLAG_IS_BEING_USED) {
        party->flags &= ~PARTY_FLAG_IS_BEING_USED;
        if (party->useStruct) {
            __memFree(HEAP_DEFAULT, party->useStruct);
            party->useStruct = NULL;
        }
#if VERSION >= VERSION_JP
        if (mario->currMotionId == MARIO_MOTION_DAMAGE_TOGE) {
            partyChgRunMode(party, PARTY_RUN_DAMAGE_RETURN);
            extra = partyGetPtr(marioGetExtraPartyId());
            if (extra) {
                partyChgRunMode(extra, PARTY_RUN_DAMAGE_RETURN);
            }
            return;
        }
#endif
    }
    if (party->runMode != PARTY_RUN_GOODBYE && party->runMode != PARTY_RUN_DAMAGE_RETURN) {
        partyChgRunMode(party, PARTY_RUN_MOVE);
        party->moveMode = PARTY_MOVE_MODE_STAY;
    }
}

/**
 * Yoshi's goodbye handler (party_data): if Mario is riding, dismounts first and keeps running the ride code;
 * otherwise plays the partner goodbye and moves to run mode 0xe when it ends.
 */
void yoshi_bye(PartyEntry* party) {
    if (party->trigFlags & PARTY_TRIG_CHG_RUN_MODE) {
        party->trigFlags &= ~PARTY_TRIG_CHG_RUN_MODE;
        partyGoodbyeInit(party);
    }
    if (party->flags & PARTY_FLAG_IS_BEING_USED) {
        if (yoshiGetStatus() == YOSHI_STATUS_RIDING) {
            party->useMotionId = 100;
        }
        yoshi_use(party);
    } else if (partyGoodbyeMain(party)) {
        partyChgRunMode(party, PARTY_RUN_DELETE);
    }
}

/** Yoshi's exit handler (party_data, partner Yoshi's entry only): cancels the ride and puts Mario back to standing. */
void yoshi_exit(PartyEntry* party) {
    party->playerPtr->flags &= ~MARIO_FLAG_CHG_MOT_DISABLED;
    marioYoshiForceCancel();
    marioChgMot(MARIO_MOTION_STAY);
}

/**
 * Bounce off a jump stand while riding: marioChgJumpStandMotion calls this instead of Mario's own jump-stand
 * motion; starts Yoshi's jump-stand sequence (motions 60-66) unless one is already running.
 */
void yoshi_jumpStand(void) {
    PartyEntry* party = partyGetPtr(marioGetPartyId());

    if (party == NULL || party->currentMemberId != PARTY_MEMBER_YOSHI || !(party->flags & PARTY_FLAG_IS_BEING_USED)) {
        return;
    }
    if (!(party->useMotionId >= 60 && party->useMotionId <= 66)) {
        party->useMotionId = 60;
    }
}

/**
 * Spike hit while riding: stops Mario's ride velocity,
 * plays the hurt motion (MARIO_MOTION_DAMAGE_TOGE) with the hurt poses and puts Yoshi
 * and the extra partner in run mode 0xd.
 */
// Descriptive name (not in the symbol maps); placeholder unk_800b474c, JP 0x800B474C. See docs/NAME_PROVENANCE.md.
void yoshi_damageToge(PartyEntry* party) {
    MarioWork* mario = marioGetPtr();
    PartyEntry* extra;

    L_sweatEntry(party, NULL);
    marioChgMot(MARIO_MOTION_DAMAGE_TOGE);
    party->useMotionId = 10;
    partyChgPose(party, "PYS_S_2");
    marioChgPose("M_Y_2");
    mario->mot.yoshi.velX = 0.0f;
    mario->mot.yoshi.velZ = 0.0f;
    partyChgRunMode(party, PARTY_RUN_DAMAGE_RETURN);
    extra = partyGetPtr(marioGetExtraPartyId());
    if (extra) {
        partyChgRunMode(extra, PARTY_RUN_DAMAGE_RETURN);
    }
}

void yoshiAnimeCtrl(PartyEntry* party) {
    MarioWork* mario = party->playerPtr;

    if (!strcmp(party->animName, "PYS_S_2")) {
        if (++mario->poseTime >= 52) {
            mario->poseTime = 0;
        }
        party->poseTime = mario->poseTime;
        animPoseSetLocalTime(party->paperAnimGroup, mario->poseTime);
    } else if (!strcmp(party->animName, "PYS_R_2")) {
        if (++mario->poseTime >= 10) {
            mario->poseTime = 0;
        }
        party->poseTime = mario->poseTime;
        animPoseSetLocalTime(party->paperAnimGroup, mario->poseTime);
    } else if (!strcmp(party->animName, "PYS_F_1")) {
        if (++mario->poseTime >= 10) {
            mario->poseTime = 0;
        }
        party->poseTime = mario->poseTime;
        animPoseSetLocalTime(party->paperAnimGroup, mario->poseTime);
    }
}

/** Yoshi's init handler (party_data): sets his member kind and size (26 tall; 28 wide in the early demos, 20 in JP and later). */
void yoshi_init(PartyEntry* party) {
    party->memberKind = PARTY_MEMBER_YOSHI;
    party->moveKind = 0;
    party->height = 26.0f;
#if VERSION <= VERSION_DEMO_JP
    party->width = 28.0f;
#else
    party->width = 20.0f;
#endif
    party->yoshiPsndSFXId = -1;
}

// Descriptive name (inline helper; not in the symbol maps).
static void searchHead(PartyEntry* party);

/**
 * Mario's vertical ride bob this frame: twice the bob table entry for Yoshi's pose (PYS_S_2, PYS_R_2 or
 * PYS_F_1) at the shared pose time.
 */
// Descriptive name (inline helper; not in the symbol maps).
static inline f32 getOfs(PartyEntry* party) {
    MarioWork* mario = party->playerPtr;
    f32 ofs = 0.0f;

    if (!strcmp(party->animName, "PYS_S_2")) {
        ofs += 2.0f * ofs_PYS_S_2[mario->poseTime];
    } else if (!strcmp(party->animName, "PYS_R_2")) {
        ofs += 2.0f * ofs_PYS_R_2[mario->poseTime];
    } else if (!strcmp(party->animName, "PYS_F_1")) {
        ofs += 2.0f * ofs_PYS_F_1[mario->poseTime];
    }
    return ofs;
}

/**
 * Runs Yoshi's ride: mounting, ground movement, flutter and jump-stand motion, dismounting and water handling.
 * The early demos omit the head search and add the fly bob speed directly to the height;
 * JP and later accumulate a bob offset around a ground/base height. The early demos also scale ordinary ride movement
 * to 3/4 while party bit 0x80 (shallow water) is set.
 */
void yoshi_use(PartyEntry* party) {
    MarioWork* mario = party->playerPtr;
    PartyEntry* extra;
    f32 dir;
    f32 spd;
    HitEntry* hit;
    s32 attr;
    f32 angle;
    f32 speed;
    f32 ox;
    f32 oz;
    MarioWork* m;
    f32 prev;
    f32 y;
    f32 x;
    f32 z;

    if (!mario->keyOffCount) {
        partyUpdateKeyData(party);
#if VERSION >= VERSION_JP
    } else {
        partyClearCont(party);
#endif
    }
    if (party->trigFlags & PARTY_TRIG_CHG_RUN_MODE) {
        party->trigFlags &= ~PARTY_TRIG_CHG_RUN_MODE;
#if VERSION >= VERSION_JP
        party->flags &= ~(PARTY_FLAG_IS_JUMPING | PARTY_FLAG_IS_FALLING | PARTY_FLAG_IS_STEPPING);
        party->flags &= ~(PARTY_FLAG_DISABLE_FRONT_WALL_CHK | PARTY_FLAG_FORCE_FALL);
        party->flags &= ~PARTY_FLAG_DISABLE_FLOORS;
#endif
        party->useStruct = __memAlloc(HEAP_DEFAULT, sizeof(YoshiWork));
        memset(party->useStruct, 0, sizeof(YoshiWork));
        YOSHI_WORK(party)->effect = NULL;
        if (party->flags & PARTY_FLAG_IS_EVT_USE) {
            evt_yoshi_spd = 1.0f;
        } else {
            evt_yoshi_frm = -1;
            evt_yoshi_spd = 0.0f;
        }
        if (!(party->flags & PARTY_FLAG_IS_EVT_USE)) {
            party->useMotionId = 0;
        } else {
            party->useMotionId = 4;
        }
        party->moveFrames = 3;
        mario->baseSpeed = 0.0f;
        mario->mot.yoshi.bobOfs = 0.0f;
        mario->mot.yoshi.bobSpeed = 0.0f;
        mario->mot.yoshi.velX = 0.0f;
        mario->mot.yoshi.velZ = 0.0f;
        YOSHI_WORK(party)->dir = 0;
    }

    switch (party->useMotionId) {
        case 0:
            if (--party->moveFrames > 0) {
                if (!marioKeyOffChk() && mario->hitobjStandOn) {
                    return;
                }
                if (party->flags & PARTY_FLAG_IS_BEING_USED) {
                    party->flags &= ~PARTY_FLAG_IS_BEING_USED;
                    if (party->useStruct) {
                        __memFree(HEAP_DEFAULT, party->useStruct);
                        party->useStruct = NULL;
                    }
                }
                if (party->runMode != PARTY_RUN_GOODBYE && party->runMode != PARTY_RUN_DAMAGE_RETURN) {
                    partyChgRunMode(party, PARTY_RUN_MOVE);
                    party->moveMode = PARTY_MOVE_MODE_STAY;
                }
                return;
            }
            party->useMotionId = 4;
        case 4:
            mario->directionView = mario->directionView <= 180.0f ? 90.0f : 270.0f;
            mario->moveDirection = mario->directionView;
            party->moveTarget = mario->position;
            party->moveFrames = 4;
            party->velY = 0.0f;
#if VERSION >= VERSION_JP
            party->gravity = 0.0f;
            party->work.common.accelY = 0.0f;
            party->work.yoshi.gravityAccel = 0.0f;
            party->accelY = 0.0f;
#endif
            party->targetDir = angleABf(party->position.x, party->position.z, party->moveTarget.x, party->moveTarget.z);
            party->moveSpeed = distABf(party->position.x, party->position.z, party->moveTarget.x, party->moveTarget.z) / party->moveFrames;
            party->useMotionId = 5;
        case 5:
            party->moveTarget = mario->position;
            party->position.x += (party->moveTarget.x - party->position.x) / party->moveFrames;
            party->position.y += (party->moveTarget.y - party->position.y) / party->moveFrames;
            party->position.z += (party->moveTarget.z - party->position.z) / party->moveFrames;
            if (--party->moveFrames != 0) {
                searchGround(party);
                yoshiAddFloorMove(party);
                return;
            }
            mario->trigFlags |= MARIO_TRIG_FLAG_DISABLE_JUMP_KEMURI;
            marioChgSmallJumpMotion();
            mario->baseSpeed = 0.0f;
            party->moveDir = mario->moveDirection;
            party->targetDir = mario->directionView;
            party->displayDirectionTarget = mario->displayDirectionTarget;
            party->displayDirection = mario->displayDirection;
            party->moveSpeed = 0.0f;
            party->useMotionId = 7;
            yoshiAddFloorMove(party);
            partyChkGnd(party);
            return;
        case 7:
            if (!(party->flags & PARTY_FLAG_IS_EVT_USE)) {
#if VERSION <= VERSION_DEMO_JP
                if (marioKeyOffChk() || mario->lastGroundPos.y - 5.0f > mario->position.y) {
#else
                if (marioKeyOffChk()) {
                    party->useMotionId = 101;
                    break;
                }
                if (!(party->flags & PARTY_FLAG_IS_GROUNDED)) {
#endif
                    party->useMotionId = 101;
                    break;
                }
            }
            if (mario->hitobjStandOn == NULL) {
                yoshiAddFloorMove(party);
                partyChkGnd(party);
                return;
            }
            party->useMotionId = 10;
            extra = partyGetPtr(marioGetExtraPartyId());
            if (extra) {
                partyChgRunMode(extra, PARTY_RUN_RIDE_YOSHI);
            }
            partyChkGnd(party);
            break;
    }

    switch (party->useMotionId) {
        case 10:
#if VERSION <= VERSION_DEMO_JP
            psndSFXOn_3D((const char*)PSND_SFX_PARTY_YOSH_RIDE1, &party->position);
            case 11:
            partyChgPose(party, "PYS_H_1");
            marioChgMot(MARIO_MOTION_YOSHI);
#else
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_PARTY_YOSH_RIDE1), &party->position);
            party->flags &= ~PARTY_FLAG_FORCE_FALL;
        case 11:
            marioChgMot(MARIO_MOTION_YOSHI);
            partyChgPose(party, "PYS_H_1");
#endif
            marioChgPose("M_Y_1");
            party->moveSpeed = 0.0f;
            party->moveDistance = 0.0f;
            party->position.x = mario->position.x;
#if VERSION >= VERSION_JP
            party->position.y = mario->position.y;
#endif
            party->position.z = mario->position.z;
            mario->camTarget.x = party->position.x;
            mario->camTarget.z = party->position.z;
            party->useMotionId = 12;
            mario->lastGroundPos = party->position;
            break;
        case 12:
            party->playerPtr->poseTime = 0;
            party->useMotionId = 13;
        case 13:
            if (mario->flags & MARIO_FLAG_FORCED_MOVEMENT) {
                marioChgPose("M_Y_2");
                partyChgPose(party, "PYS_R_2");
                party->hitobjFront = NULL;
                party->hitobjPush = NULL;
#if VERSION >= VERSION_JP
                party->hitobjHead = NULL;
#endif
                mario->hitobjFront = NULL;
                mario->hitobjPush = NULL;
                mario->hitobjWall = NULL;
#if VERSION <= VERSION_DEMO_JP
                checkWall(party, mario->baseSpeed, mario->directionView);
#else
                mario->hitobjHead = NULL;
                dir = mario->directionView;
                spd = mario->baseSpeed;
                searchHead(party);
                checkWall(party, spd, dir);
#endif
                yoshiSyncMario(mario, party);
                party_ride_yoshi_force_move();
                mario->lastGroundPos = party->position;
                searchGround(party);
                if (--mario->forceMoveTimer <= 0) {
                    mario->forceMoveTimer = 0;
                    mario->flags &= ~MARIO_FLAG_FORCED_MOVEMENT;
                }
#if VERSION >= VERSION_JP
                YOSHI_WORK(party)->count++;
                if (YOSHI_WORK(party)->count % 10 == 0) {
                    psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_PARTY_YOSH_MOVE1), &party->position);
                }
#endif
                break;
            }
            if (party->runMode == PARTY_RUN_GOODBYE) {
                break;
#if VERSION >= VERSION_JP
            }
            if (yoshiGetStatus() == YOSHI_STATUS_RIDING) {
                party->floorMoveX = mario->floorMove.x;
                party->floorMoveY = mario->floorMove.y;
                party->floorMoveZ = mario->floorMove.z;
#endif
            }
            if (party->flags & PARTY_FLAG_IS_GROUNDED) {
                if (!(party->flags & PARTY_FLAG_IS_EVT_USE)) {
                    if (party->buttonTrg & PAD_BUTTON_A) {
                        party->useMotionId = 20;
                        break;
                    }
                } else if (evt_yoshi_frm >= 0) {
                    party->useMotionId = 20;
                    break;
                }
            }
            if (party->flags & PARTY_FLAG_IS_EVT_USE) {
                angle = mario->directionView;
                speed = evt_yoshi_spd;
            } else {
                getStick(party, &angle, &speed);
            }
            if (speed > 0.0f) {
                party->moveSpeed = speed;
                party->targetDir = angle;
                party->moveDir = angle;
                marioChgPose("M_Y_2");
                partyChgPose(party, "PYS_R_2");
                YOSHI_WORK(party)->count++;
                if (YOSHI_WORK(party)->count % 10 == 0) {
                    psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_PARTY_YOSH_MOVE1), &party->position);
                }
            } else {
                if (strcmp(party->animName, "PYS_S_2")) {
                    mario->poseTime = 0;
                }
                partyChgPose(party, "PYS_S_2");
                party->moveSpeed = 0.0f;
                YOSHI_WORK(party)->count = 0;
            }
            inertia(party);
            party->position.x = mario->position.x;
            party->position.z = mario->position.z;
            party->hitobjFront = NULL;
            party->hitobjPush = NULL;
            mario->hitobjFront = NULL;
            mario->hitobjPush = NULL;
            mario->hitobjWall = NULL;
            dir = party->targetDir;
#if VERSION <= VERSION_DEMO_JP
            spd = party->moveSpeed * ((party->flags & PARTY_FLAG_IN_SHALLOW_WATER) ? 0.75f : 1.0f);
#else
            spd = party->moveSpeed;
            searchHead(party);
#endif
            checkWall(party, spd, dir);
            yoshiSyncMario(mario, party);
            if (party->buttonTrg & (PAD_BUTTON_B | PAD_BUTTON_X)) {
                party->useMotionId = 100;
            }
            searchGround(party);
            mario->moveDist = spd;
            break;
    }

    switch (party->useMotionId) {
        case 100:
            resetOfs(party);
            marioChgSmallJumpMotion();
            mario->baseSpeed = 0.0f;
            partyChgPose(party, "PYS_H_2");
            party->moveSpeed = 0.0f;
            party->useMotionId = 101;
            yoshiAddFloorMove(party);
            partyChkGnd(party);
            return;
        case 101:
            if (mario->hitobjStandOn == NULL) {
                yoshiAddFloorMove(party);
                partyChkGnd(party);
                return;
            }
            party->flags &= ~PARTY_FLAG_IS_BEING_USED;
            if (party->runMode != PARTY_RUN_GOODBYE) {
                partyChgRunMode(party, PARTY_RUN_MOVE);
                party->moveMode = PARTY_MOVE_MODE_STAY;
            }
            yoshiAddFloorMove(party);
            partyChkGnd(party);
#if VERSION >= VERSION_US
            movePos(&party->position.x, &party->position.z, mario->hitobjFront ? -0.1f : 0.1f, toMovedir(mario->displayDirection));
#endif
            if (party->useStruct) {
                __memFree(HEAP_DEFAULT, party->useStruct);
                party->useStruct = NULL;
            }
            return;
    }

    switch (party->useMotionId) {
        case 40:
            party->flags |= PARTY_FLAG_IS_STEPPING;
            party->velY = 0.0f;
            party->accelY = 0.0f;
            party->gravity = 0.0f;
            party->moveFrames = 3;
            party->velY = (party->stepY - party->position.y) / party->moveFrames;
            party->useMotionId = 41;
        case 41:
            party->position.y += party->velY;
            if (--party->moveFrames <= 0) {
                party->flags &= ~PARTY_FLAG_IS_STEPPING;
                party->useMotionId = 13;
            }
            break;
    }

    switch (party->useMotionId) {
        case 20:
#if VERSION <= VERSION_DEMO_JP
            if (mario->displayDirection >= 90.0f && mario->displayDirection <= 270.0f) {
                YOSHI_WORK(party)->dir = 1;
            } else {
                YOSHI_WORK(party)->dir = -1;
            }
#else
#if VERSION >= VERSION_US
            YOSHI_WORK(party)->dir = (party->displayDirectionTarget >= 90.0f && party->displayDirectionTarget <= 270.0f) ? 1 : -1;
#else
            YOSHI_WORK(party)->dir = (mario->displayDirection >= 90.0f && mario->displayDirection <= 270.0f) ? 1 : -1;
#endif
#endif
            mario->hitobjJumpFrom = mario->hitobjStandOn;
            mario->hitobjStandOn = NULL;
            mario->lastGroundPos = party->position;
            partyChgPose(party, "PYS_F_1");
            marioChgPose("M_Y_3");
            mario->airTimer = 0;
            party->useMotionId = 21;
            party->moveSpeed += -0.01f;
            if (party->moveSpeed <= 0.0f) {
                party->moveSpeed = 0.0f;
            }
            YOSHI_WORK(party)->angle = 1.0f;
            mario->mot.yoshi.bobSpeed = sin(3.141592653589793f * YOSHI_WORK(party)->angle / 180.0f);
            mario->mot.yoshi.bobOfs = 0.0f;
#if VERSION <= VERSION_DEMO_JP
            party->yoshiPsndSFXId = psndSFXOn_3D((const char*)PSND_SFX_PARTY_YOSH_JUMP1, &party->position);
#else
            mario->mot.yoshi.baseY = party->position.y;
            party->yoshiPsndSFXId = psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_PARTY_YOSH_JUMP1), &party->position);
#endif
        case 21: {
                f32 s = sin(3.141592653589793f * YOSHI_WORK(party)->angle / 180.0f);
                mario->mot.yoshi.bobSpeed = s * (YOSHI_WORK(party)->angle <= 180.0f ? 0.5f : 0.37f);
            }
#if VERSION >= VERSION_JP
            mario->mot.yoshi.bobOfs += mario->mot.yoshi.bobSpeed;
#endif
            YOSHI_WORK(party)->angle += 7.0f;
            if (YOSHI_WORK(party)->angle >= 360.0f) {
                YOSHI_WORK(party)->angle = 360.0f;
                party->useMotionId = 22;
            }
        case 22:
            y = 18.0f + party->position.y;
            x = party->position.x;
            z = party->position.z;
            if (YOSHI_WORK(party)->effect == NULL) {
                YOSHI_WORK(party)->effect = effSweatN64Entry(1, x, y, z, 1.0f, 70.0f * (party->displayDirectionTarget == 180.0f ? -1.0f : 1.0f), 0x16);
                YOSHI_WORK(party)->timer = 20;
            }
#if VERSION <= VERSION_DEMO_JP
            party->position.y += mario->mot.yoshi.bobSpeed;
#else
            hit = partySearchGround(party, 11.0f, 11.0f);
            if (hit == NULL) {
                party->position.y = mario->mot.yoshi.baseY + mario->mot.yoshi.bobOfs;
            } else {
                attr = hitGetAttr(hit);
                if (attr & HIT_ATTR_WATER_HAZARD) {
                    if (party->position.y <= party->groundY) {
                        party->useMotionId = 70;
                    }
                } else if (attr & (HIT_ATTR_DAMAGE_LIQUID | HIT_ATTR_SPIKES)) {
                    if (party->position.y <= party->groundY) {
                        yoshi_damageToge(party);
                        return;
                    }
                }
                L_sweatEntry(party, NULL);
                mario->mot.yoshi.baseY = party->groundY;
                party->position.y = party->groundY + mario->mot.yoshi.bobOfs;
            }
#endif
            party->position.x = mario->position.x;
            party->position.z = mario->position.z;
            if (party->flags & PARTY_FLAG_IS_EVT_USE) {
#if VERSION <= VERSION_DEMO_JP
                party->moveSpeed = evt_yoshi_spd;
            } else {
                getStick(party, &angle, &speed);
                yoshiFlyMove(party, &angle);
#else
                f32 s = evt_yoshi_spd;
                angle = mario->directionView;
                speed = s;
                party->moveSpeed = s;
            } else {
                getStick(party, &angle, &speed);
                yoshiFlyMove(party);
#endif
            }
            party->hitobjFront = NULL;
            party->hitobjPush = NULL;
            dir = party->targetDir;
#if VERSION <= VERSION_DEMO_JP
            checkWall(party, party->moveSpeed * ((party->flags & PARTY_FLAG_IN_SHALLOW_WATER) ? 0.75f : 1.0f), dir);
#else
            spd = party->moveSpeed;
            searchHead(party);
            checkWall(party, spd, dir);
#endif
            if (party->hitobjStandOn == NULL && mario->airTimer >= 10) {
                searchGround2(party);
                if (party->hitobjStandOn) {
                    mario->hitobjJumpFrom = NULL;
#if VERSION >= VERSION_JP
                    if (party->yoshiPsndSFXId != -1) {
                        psndSFXOff(party->yoshiPsndSFXId);
                        party->yoshiPsndSFXId = -1;
                    }
#endif
                    party->useMotionId = 10;
                    partyChgPose(party, "PYS_S_2");
                    marioChgPose("M_Y_2");
                    mario->mot.yoshi.velX = 0.0f;
                    mario->mot.yoshi.velZ = 0.0f;
                    if (party->flags & PARTY_FLAG_IS_EVT_USE) {
                        evt_yoshi_spd = 0.0f;
                        evt_yoshi_frm = -1;
                        party->moveSpeed = 0.0f;
                        party->useMotionId = 52;
#if VERSION <= VERSION_DEMO_JP
                        break; // the early demos only break out here in the evt mode
                    }
#else
                    }
                    break;
#endif
                }
                YOSHI_WORK(party)->dir = 0;
            }
            yoshiSyncMario(mario, party);
            mario->airTimer++;
            if (mario->airTimer > 14) {
                if (party->flags & PARTY_FLAG_IS_EVT_USE) {
                    if (mario->airTimer > 68) {
                        evt_yoshi_frm = 0;
                        party->velY = 0.0f;
                        party->gravity = -1.0f;
                        party->useMotionId = 50;
                    }
                } else if (mario->airTimer > 68 || !(party->buttonHold & PAD_BUTTON_A) || marioKeyOffChk()) {
                    party->velY = 0.0f;
                    party->gravity = -1.0f;
                    party->useMotionId = 50;
                }
            }
            break;
    }

    yoshiAnimeCtrl(party);
#if VERSION <= VERSION_DEMO_JP
    if (yoshiGetStatus() == YOSHI_STATUS_RIDING || yoshiGetStatus() == YOSHI_STATUS_FLUTTER) {
#else
    if (party->useMotionId == 13 || yoshiGetStatus() == YOSHI_STATUS_STAND_JUMP || yoshiGetStatus() == YOSHI_STATUS_FALL || yoshiGetStatus() == YOSHI_STATUS_FLUTTER) {
#endif
        ox = 0.0f;
        oz = 0.0f;
        movePos(&ox, &oz, -6.0f, toMovedir(party->displayDirection));
        mario->dispOfs.x = ox;
        mario->dispOfs.z = oz;
        mario->dispOfs2.y = 8.0f + getOfs(party);
        extra = partyGetPtr(marioGetExtraPartyId());
        if (extra) {
            extra->dispOfs.y = getOfs(party);
        }
    }

    switch (party->useMotionId) {
        case 60:
            party->moveSpeed = 0.0f;
            party->flags &= ~PARTY_FLAG_IS_FALLING;
            party->flags |= PARTY_FLAG_IS_JUMPING;
            mario->airTimer = 0;
            mario->jumpPeakAirTime = 0;
            party->hitobjJumpFrom = party->hitobjStandOn;
            L_sweatEntry(party, NULL);
            mario->hitobjStandOn = NULL;
            mario->lastGroundPos = mario->position;
            jumpheight = 100.0f;
            mario->jumpDistance = 0.0f;
            party->useMotionId = 61;
            party->moveFrames = 0;
            party->gravity = marioJumpStandData[0];
            party->work.common.accelY = marioJumpStandData[1];
            party->work.yoshi.gravityAccel = marioJumpStandData[2];
            party->accelY = marioJumpStandData[3];
        case 61:
            prev = party->gravity;
            party->work.yoshi.gravityAccel += party->accelY;
            party->work.common.accelY += party->work.yoshi.gravityAccel;
            party->gravity += party->work.common.accelY;
            if (prev >= party->gravity) {
                party->useMotionId = 62;
            }
            party->position.y += party->gravity;
            mario->jumpDistance += party->gravity;
            break;
        case 62:
            party->position.y += party->gravity;
            mario->jumpDistance += party->gravity;
            if (mario->jumpDistance >= 0.7f * marioGetJumpHeight()) {
                party->useMotionId = 63;
            }
            break;
        case 63:
            party->gravity += 0.125f * (-1.0f - party->gravity);
            party->position.y += party->gravity;
            mario->jumpDistance += party->gravity;
            if (party->gravity <= 0.0f) {
                party->useMotionId = 64;
                marioSetPeakYpos();
                party->gravity = -0.4f;
                party->work.common.accelY = -0.4f;
                party->work.yoshi.gravityAccel = 0.0f;
                party->accelY = 0.0f;
                party->flags &= ~PARTY_FLAG_IS_JUMPING;
                party->flags |= PARTY_FLAG_IS_FALLING;
                party->useMotionId = 64;
            }
            break;
        case 64:
            party->work.yoshi.gravityAccel += party->accelY;
            party->work.common.accelY += party->work.yoshi.gravityAccel;
            party->gravity += party->work.common.accelY;
            if (party->gravity <= 0.0f) {
                marioSetPeakYpos();
                party->gravity = -0.4f;
                party->work.common.accelY = -0.4f;
                party->work.yoshi.gravityAccel = 0.0f;
                party->accelY = 0.0f;
                party->flags &= ~PARTY_FLAG_IS_JUMPING;
                party->flags |= PARTY_FLAG_IS_FALLING;
                party->useMotionId = 65;
            }
            party->position.y += party->gravity;
            break;
        case 65:
            if (--party->moveFrames <= 0) {
                party->moveSpeed = 0.0f;
                party->velY = 0.0f;
                party->gravity = -1.0f;
                party->useMotionId = 50;
            }
            break;
    }

    if (party->useMotionId >= 64 && party->useMotionId <= 65) {
        mario->airTimer++;
        searchGround(party);
        if (party->hitobjStandOn) {
            mario->hitobjJumpFrom = NULL;
            party->velY = 0.0f;
            party->gravity = -1.0f;
            party->useMotionId = 50;
        }
    }

    switch (party->useMotionId) {
        case 50:
        case 51:
            party->position.x = mario->position.x;
            party->position.z = mario->position.z;
            if (party->flags & PARTY_FLAG_IS_EVT_USE) {
#if VERSION <= VERSION_DEMO_JP
                party->moveSpeed = evt_yoshi_spd;
            } else {
                getStick(party, &angle, &speed);
                yoshiFlyMove(party, &angle);
#else
                f32 s = evt_yoshi_spd;
                angle = mario->directionView;
                speed = s;
                party->moveSpeed = s;
            } else {
                getStick(party, &angle, &speed);
                yoshiFlyMove(party);
#endif
            }
            if (!(party->buttonHold & PAD_BUTTON_A) || marioKeyOffChk()) {
                if (party->useMotionId == 50) {
                    party->useMotionId = 51;
                }
                m = marioGetPtr();
                m->mot.yoshi.bobSpeed += m->mot.yoshi.bobOfs;
            }
            if (mario->mot.yoshi.bobSpeed <= -10.0f) {
                mario->mot.yoshi.bobSpeed = -10.0f;
            }
            party->hitobjFront = NULL;
            party->hitobjPush = NULL;
            mario->hitobjFront = NULL;
            mario->hitobjPush = NULL;
            mario->hitobjWall = NULL;
            dir = party->targetDir;
#if VERSION <= VERSION_DEMO_JP
            checkWall(party, party->moveSpeed * ((party->flags & PARTY_FLAG_IN_SHALLOW_WATER) ? 0.75f : 1.0f), dir);
#else
            spd = party->moveSpeed;
            searchHead(party);
            checkWall(party, spd, dir);
#endif
            searchGround(party);
            if (party->hitobjStandOn) {
                mario->hitobjJumpFrom = NULL;
                party->useMotionId = 13;
                partyChgPose(party, "PYS_S_2");
                marioChgPose("M_Y_2");
                mario->mot.yoshi.velX = 0.0f;
                mario->mot.yoshi.velZ = 0.0f;
                YOSHI_WORK(party)->dir = 0;
                if (party->flags & PARTY_FLAG_IS_EVT_USE) {
                    evt_yoshi_spd = 0.0f;
                    evt_yoshi_frm = -1;
                    party->moveSpeed = 0.0f;
                    party->useMotionId = 52;
                }
            }
            break;
        case 52:
            if (!(party->flags & PARTY_FLAG_IS_EVT_USE)) {
                party->useMotionId = 13;
            }
            break;
    }

    sweatMain(party);
    if (party->useMotionId == 70) {
        if (marioChkDamageSink()) {
            party->position = party->playerPtr->position;
            return;
        }
        party->useMotionId = 71;
        return;
    }
    if (party->useMotionId == 71) {
        party->flags &= ~PARTY_FLAG_IS_BEING_USED;
        if (party->useStruct) {
            __memFree(HEAP_DEFAULT, party->useStruct);
            party->useStruct = NULL;
        }
#if VERSION >= VERSION_JP
        marioSetFallPara();
        marioChgMot(MARIO_MOTION_DAMAGE);
#endif
        partyChgRunMode(party, PARTY_RUN_DAMAGE_RETURN);
        extra = partyGetPtr(marioGetExtraPartyId());
        if (extra) {
            partyChgRunMode(extra, PARTY_RUN_DAMAGE_RETURN);
        }
        return;
    }
#if VERSION <= VERSION_DEMO_JP
    yoshiAddFloorMove(party);
#endif
    if (party->useMotionId >= 10 && party->useMotionId <= 13 && mario->currMotionId != MARIO_MOTION_YOSHI) {
        party->flags &= ~PARTY_FLAG_IS_BEING_USED;
        if (party->useStruct) {
            __memFree(HEAP_DEFAULT, party->useStruct);
            party->useStruct = NULL;
        }
        partyChgRunMode(party, PARTY_RUN_MOVE);
        party->moveMode = PARTY_MOVE_MODE_STAY;
        resetOfs(party);
        return;
    }
#if VERSION >= VERSION_JP
    yoshiAddFloorMove(party);
#endif
    mario->camTarget.x = party->position.x;
    mario->camTarget.z = party->position.z;
    mario->displayDirection = party->displayDirection;
    mario->displayDirectionTarget = party->displayDirectionTarget;
    mario->position = party->position;
    marioUpdateCamCollision(party->position);
}

#if VERSION <= VERSION_DEMO_JP

/** The early demos' version works the fly angle in the caller's variable (passed by pointer). */
void yoshiFlyMove(PartyEntry* party, f32* angle) {
#else
void yoshiFlyMove(PartyEntry* party) {
#endif
    MarioWork* mario = marioGetPtr();
    f32 z;
    f32 x;
    f32 spd;
    f32 max;
    f32 one = 1.0f;

    inertia(party);
#if VERSION <= VERSION_DEMO_JP
    *angle = -mario->mot.yoshi.velZ;
#else
    z = -mario->mot.yoshi.velZ;
#endif
    x = mario->mot.yoshi.velX;
#if VERSION <= VERSION_DEMO_JP
    spd = 0.1f * distABf(0.0f, 0.0f, x, *angle);
#else
    spd = 0.1f * distABf(0.0f, 0.0f, x, z);
#endif
    max = mario->baseDashSpeed * one;
    if (spd >= max) {
        spd = max;
    }
#if VERSION <= VERSION_DEMO_JP
    *angle = angleABf(0.0f, 0.0f, x, *angle);
#else
    z = angleABf(0.0f, 0.0f, x, z);
#endif
    if (spd < 0.01f) {
        spd = 0.0f;
    }
    party->moveSpeed = spd;
    if (spd) {
#if VERSION <= VERSION_DEMO_JP
        party->targetDir = revise360(*angle + party->camDir);
        party->moveDir = revise360(*angle + party->camDir);
#else
        party->targetDir = revise360(z + party->camDir);
        party->moveDir = revise360(z + party->camDir);
#endif
    }
    mario->moveDist = spd;
}

/**
 * Ride velocity from the stick X axis: accelerates velX,
 * stops on a reversal against the flutter direction and clamps to 16.
 * With the stick released it brakes (harder against a wall);
 * in US and later it brakes only during the flutter and stops at once otherwise.
 */
void inertia(PartyEntry* party) {
    MarioWork* mario = party->playerPtr;
    f32 sx = party->stickX;
    f32 vx = mario->mot.yoshi.velX;
    f32 vz = mario->mot.yoshi.velZ;
    f32 k;
    f32 lim = 16.0f;
    f32 acc = 0.01f;
    f32 zero = 0.0f;
    s32 dir;

    if (!sx) {
#if VERSION >= VERSION_US
        if (yoshiGetStatus() != YOSHI_STATUS_FLUTTER) {
            vz = vx = 0.0f;
        } else {
            k = -0.07f;
            if (mario->hitobjWall) {
                k = -0.5f;
            }
            vx = vx * k + vx;
            vz = vz * k + vz;
            if (__fabsf(vx) < 0.01f) {
                vx = 0.0f;
            }
            if (__fabsf(vz) < 0.01f) {
                vz = 0.0f;
            }
        }
#else
        k = -0.07f;
        if (mario->hitobjWall) {
            k = -0.5f;
        }
        vx = vx * k + vx;
        vz = vz * k + vz;
        if (__fabsf(vx) < 0.01f) {
            vx = 0.0f;
        }
        if (__fabsf(vz) < 0.01f) {
            vz = 0.0f;
        }
#endif
    } else if (vx * vx + vz * vz <= 256.0f) {
        dir = YOSHI_WORK(party)->dir;
        vx += sx * acc;
        if (dir > 0 && vx <= 0.0f) {
            vx = 0.0f;
        }
        if (dir < 0 && vx >= 0.0f) {
            vx = 0.0f;
        }
        vz -= zero;
    } else {
        vz -= zero;
        vx += sx * acc;
        if (vx < -lim) {
            vx = -lim;
        }
        if (vx > lim) {
            vx = lim;
        }
        if (vz < -lim) {
            vz = -lim;
        }
        if (vz > lim) {
            vz = lim;
        }
    }
    mario->mot.yoshi.velX = vx;
    mario->mot.yoshi.velZ = vz;
}

#if VERSION <= VERSION_DEMO_JP

/** The early demos search 11 up into locals and has the older landing and step code. */
static void searchGround(PartyEntry* party) {
#else
void searchGround(PartyEntry* party) {
#endif
    HitEntry* hit;
    MarioWork* mario;
    s32 attr;
    f32 groundY;
    f32 range;
#if VERSION <= VERSION_DEMO_JP
    Vec ground; // the search fills only .y
#endif
    Vec pos;
#if VERSION <= VERSION_DEMO_JP
    f32 depth;
#endif
    f32 stepY;
    f32 stepA;
    f32 stepB;
    f32 diff;
    HitEntry* step;

    mario = party->playerPtr;
    hit = NULL;
    if (!(party->flags & PARTY_FLAG_FORCE_FALL)) {
        range = __fabsf(party->gravity);
        if (range < 1.0f) {
            range = 1.0f;
        }
#if VERSION <= VERSION_DEMO_JP
        hit = _partySearchGround(party, &ground.y, &depth, 11.0f, range);
#else
        hit = partySearchGround(party, range, range);
        groundY = party->groundY;
#endif
    }
    if (hit) {
        attr = hitGetAttr(hit);
        if (party->yoshiPsndSFXId != -1) {
            psndSFXOff(party->yoshiPsndSFXId);
            party->yoshiPsndSFXId = -1;
        }
        if (attr & HIT_ATTR_WATER_HAZARD) {
#if VERSION >= VERSION_JP
            if (!chgMotion(party, 70)) {
                return;
            }
#endif
            pos = party->position;
            marioSetSplash(0, &pos);
            L_sweatEntry(party, NULL);
            fall(party);
            party->position.y += party->gravity;
            set_damage_root_ypos(mario->position.y - 8.0f);
            resetOfs(party);
            party->flags |= PARTY_FLAG_FORCE_FALL;
            if (party->gravity <= -6.0f) {
                party->gravity = -6.0f;
                party->work.common.accelY = 0.0f;
                party->work.yoshi.gravityAccel = 0.0f;
                party->accelY = 0.0f;
            }
            mario->jumpVelocityY = party->gravity;
            mario->jumpAccelerationY = party->work.common.accelY;
            mario->jumpJerkY = party->work.yoshi.gravityAccel;
            mario->jumpSnapY = party->accelY;
            marioChgMot(MARIO_MOTION_DAMAGE);
#if VERSION <= VERSION_DEMO_JP
            party->useMotionId = 70;
#endif
            return;
        }
#if VERSION <= VERSION_DEMO_JP
        if (attr & HIT_ATTR_SPIKES) {
#else
        if (attr & (HIT_ATTR_DAMAGE_LIQUID | HIT_ATTR_SPIKES)) {
#endif
            yoshi_damageToge(party);
#if VERSION >= VERSION_JP
            partyChgRunMode(party, PARTY_RUN_DAMAGE_RETURN);
            {
                PartyEntry* extra = partyGetPtr(marioGetExtraPartyId());
                if (extra) {
                    partyChgRunMode(extra, PARTY_RUN_DAMAGE_RETURN);
                }
            }
#endif
            return;
        }
        if (hit && (hitGetAttr(hit) & HIT_ATTR_DAMAGE_RETURN)) {
            mario->damageHit = hit;
        }
        if (!(party->flags & (PARTY_FLAG_IS_JUMPING | PARTY_FLAG_IS_FALLING))) {
            step = partyChkFrontStep(party, 11.0f, &stepY, &stepA, &stepB);
            diff = stepY - party->position.y;
            if (step && diff <= 11.0f && diff >= 5.0f && party->groundSlope <= 30.0f) {
                party->stepY = stepY;
                if (party->moveSpeed) {
#if VERSION <= VERSION_DEMO_JP
                    party->useMotionId = 40;
                    return;
#else
                    if (chgMotion(party, 40)) {
                        return;
                    }
#endif
                }
            }
        }
    }
    L_sweatEntry(party, hit);
    if (hit == NULL) {
        party->flags &= ~PARTY_FLAG_IS_GROUNDED;
        fall(party);
        party->position.y += party->gravity;
    } else {
        party->flags |= PARTY_FLAG_IS_GROUNDED;
        if (party->flags & PARTY_FLAG_IS_FALLING) {
            party->flags &= ~(PARTY_FLAG_IS_JUMPING | PARTY_FLAG_IS_FALLING | PARTY_FLAG_IS_STEPPING);
            mario->flags &= ~(MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING | MARIO_FLAG_IS_STEPPING | MARIO_FLAG_HAS_INPUT_JUMP);
            party->velY = 0.0f;
#if VERSION <= VERSION_DEMO_JP
            party->position.y = ground.y;
#else
            party->position.y = groundY;
#endif
            if (party->yoshiPsndSFXId != -1) {
                psndSFXOff(party->yoshiPsndSFXId);
                party->yoshiPsndSFXId = -1;
            }
        } else {
#if VERSION <= VERSION_DEMO_JP
            party->position.y = ground.y;
#else
            party->position.y = groundY;
#endif
        }
    }
}

#if VERSION <= VERSION_DEMO_JP

/** The early demos search into locals, always record the hit object, and stops the flutter sound on landing. */
static void searchGround2(PartyEntry* party) {
#else
void searchGround2(PartyEntry* party) {
#endif
    HitEntry* hit = NULL;
    MarioWork* mario = party->playerPtr;
    s32 attr;
#if VERSION <= VERSION_DEMO_JP
    Vec ground; // the search fills only .y
    Vec pos;
    f32 depth;

    if (!(party->flags & PARTY_FLAG_FORCE_FALL)) {
        hit = _partySearchGround(party, &ground.y, &depth, 11.0f, 0.0f);
    }
    L_sweatEntry(party, hit);
    if (hit == NULL) {
        return;
    }
    if (party->yoshiPsndSFXId != -1) {
        psndSFXOff(party->yoshiPsndSFXId);
        party->yoshiPsndSFXId = -1;
    }
#else
    if (!(party->flags & PARTY_FLAG_FORCE_FALL)) {
        hit = partySearchGround(party, 11.0f, 0.0f);
    }
    if (hit == NULL) {
        L_sweatEntry(party, hit);
        return;
    }
#endif
    attr = hitGetAttr(hit);
    if (attr & HIT_ATTR_WATER_HAZARD) {
        Vec pos = party->position;
        marioSetSplash(0, &pos);
        L_sweatEntry(party, NULL);
        party->gravity = -0.4f;
        party->work.common.accelY = -0.4f;
        party->work.yoshi.gravityAccel = 0.0f;
        party->accelY = 0.0f;
        party->work.yoshi.gravityAccel += party->accelY;
        party->work.common.accelY += party->work.yoshi.gravityAccel;
        party->gravity += party->work.common.accelY;
        if (party->gravity < -15.0f) {
            party->gravity = -15.0f;
        }
        party->position.y += party->gravity;
        party->useMotionId = 70;
        return;
    }
    if (attr & HIT_ATTR_SPIKES) {
        yoshi_damageToge(party);
    }
    party->flags |= PARTY_FLAG_IS_GROUNDED;
    party->velY = 0.0f;
#if VERSION <= VERSION_DEMO_JP
    party->position.y = ground.y;
#else
    party->position.y = party->groundY;
#endif
    party->flags &= ~(PARTY_FLAG_IS_JUMPING | PARTY_FLAG_IS_FALLING | PARTY_FLAG_IS_STEPPING);
    mario->flags &= ~(MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING | MARIO_FLAG_IS_STEPPING | MARIO_FLAG_HAS_INPUT_JUMP);
}

/** Looks for a ceiling above Yoshi (10 units over his height) and stores it in Mario's hitobjHead. */
#if VERSION >= VERSION_JP
static void searchHead(PartyEntry* party) {
    MarioWork* mario = party->playerPtr;
    Vec pos = party->position;
    f32 height = partyGetHeight(party);

    mario->hitobjHead = marioSearchHead(&pos, &height, 10.0f);
}
#endif

/**
 * Wall check while riding: moves Yoshi by spd in direction dir up to a wall, records the front,
 * push and A-button hit objects for Mario, takes spike damage from walls, then pushes Yoshi out of nearby walls.
 */
#if VERSION <= VERSION_DEMO_JP
static void checkWall(PartyEntry* party, f32 spd, f32 dir) {
#else
void checkWall(PartyEntry* party, f32 spd, f32 dir) {
#endif
    MarioWork* mario = marioGetPtr();
    HitEntry* hit;
    Vec pos;
    s32 work1;
    s32 work2;
    s32 attr;

    pos = party->position;
    hit = partySearchWallFront(party, &pos, spd, dir);
    if (hit) {
        party->hitobjFront = hit;
        mario->hitobjFront = hit;
        if (spd) {
            if ((party->flags & PARTY_FLAG_IS_BEING_USED) && marioChkBero(hit) == TRUE) {
                party->hitobjPush = hit;
                mario->hitobjPush = hit;
                mario->hitobjWall = hit;
            } else {
                party->hitobjPush = NULL;
                mario->hitobjPush = NULL;
                mario->hitobjWall = NULL;
            }
        }
        if (hitGetAttr(hit) & HIT_ATTR_SPIKES) {
            yoshi_damageToge(party);
        }
        attr = hitGetAttr(hit);
        if ((attr & (HIT_ATTR_NPC_ONLY | HIT_ATTR_BERO)) && !(attr & (HIT_ATTR_SHIP_ONLY | HIT_ATTR_ROLL_THROUGH))) {
            movePos(&party->position.x, &party->position.z, spd, dir);
            return;
        }
        party->position = pos;
    } else if (!spd) {
#if VERSION <= VERSION_DEMO_JP
        f32 spd;
        spd = 0.5f * party->width;
#endif
        pos = party->position;
        hit = partySearchWallFront(party, &pos, spd, party->targetDir);
        party->hitobjFront = hit;
        mario->hitobjFront = hit;
        if (hit == NULL) {
            f32 ang = toMovedir(party->displayDirection);
            pos = party->position;
            hit = partySearchWallFront(party, &pos, spd, ang);
            party->hitobjFront = hit;
            mario->hitobjFront = hit;
        }
        if (party->buttonTrg & PAD_BUTTON_A) {
            pos = party->position;
            hit = partySearchWallFront(party, &pos, spd, party->targetDir);
            if (hit == NULL) {
                party->hitobjAction = hit;
                mario->hitobjFrontA = hit;
            } else {
                party->hitobjAction = party->hitobjFront;
                mario->hitobjFrontA = party->hitobjFront;
            }
        }
        hit = dou10_yoko_yari2(&work1);
        if (hit && (hitGetAttr(hit) & HIT_ATTR_SPIKES)) {
            yoshi_damageToge(party);
        }
#if VERSION <= VERSION_DEMO_JP
        return;
#endif
    } else {
        hit = dou10_yoko_yari2(&work2);
        if (hit && (hitGetAttr(hit) & HIT_ATTR_SPIKES)) {
            yoshi_damageToge(party);
        }
    }
#if VERSION <= VERSION_DEMO_JP
    marioChkWallAround(&pos, spd, dir, party->width, partyGetHeight(party));
#else
#if VERSION >= VERSION_US
    partyChkWallAround(party, &pos, spd, dir, party->width, mario->wPlayerCollisionBox.y);
#else
    marioChkWallAround(&pos, 1, spd, dir, party->width, mario->wPlayerCollisionBox.y);
#endif
#endif
    party->position.x = pos.x;
    party->position.z = pos.z;
}

/**
 * Converts the stick into a camera-relative ride direction and speed:
 * dash or walk speed by stick strength, times 1.5, cut to 3/4 in shallow water.
 * Zero speed while Mario's keys are off.
 */
void getStick(PartyEntry* party, f32* angle, f32* speed) {
    s32 x;
    s32 y;
    MarioWork* mario = party->playerPtr;
    f32 dist;
    f32 ang;

    if (mario->keyOffCount) {
        *speed = 0.0f;
        *angle = party->moveDir;
        return;
    }
    y = -party->stickY;
    x = party->stickX;
    dist = distABf(0.0f, 0.0f, x, -y);
    if (dist >= 70.0f) {
        dist = 70.0f;
    }
    ang = angleABf(0.0f, 0.0f, x, -y);
    if (!dist) {
        ang = party->moveDir;
    }
    ang = revise360(ang + party->camDir);
    *angle = ang;
    if (dist > 0.0f) {
        if (x * x + y * y > STICK_DASH_MIN_SQ) {
            dist = 1.5f * (mario->baseDashSpeed * marioGetMoveRate(ang));
        } else {
            dist = 1.5f * (mario->baseWalkSpeed * marioGetMoveRate(ang));
        }
        if (hitGetAttr(mario->hitobjStandOn) & HIT_ATTR_SHALLOW_WATER) {
            mario->flags |= MARIO_FLAG_IN_SHALLOW_WATER;
            dist *= 0.75f;
        } else {
            mario->flags &= ~MARIO_FLAG_IN_SHALLOW_WATER;
        }
        dist *= mario->playerGravity;
    }
    *speed = dist;
}

