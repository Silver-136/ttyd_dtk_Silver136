#define _MATH_H_
double fabs(double x);
#include "party/party_cloud.h"
#include "dolphin/types.h"
#include "musyx/musyx.h"
#include "party.h"
#include "mario/mario.h"
#include "mario/mario_sbr.h"
#include "mario/mario_party.h"
#include "mario/mario_cam.h"
#include "system.h"
#include <dolphin/pad.h>
#include "sfx_name_word.h"
#include "eff/eff_magic1_n64.h"
#include "libc_decls.h"
#include "party/party_move.h"

Vec cloud_at;


#include "driver/hitdrv.h"
#include "driver/effdrv.h"
#include "memory.h"
#include "pmario_sound.h"
#include "mario/mario_motion.h"
#include "eff/n64/eff_kumokumo_n64.h"
void N__cloud_use(PartyEntry* party);
void getHitBreatheout2(PartyEntry* party, f32 angle);

// Flurrie's work (PartyEntry.useStruct). A macro, not an inline function: as a function the object changes (2026-10-27).
#define CLOUD_WORK(p) ((PartyCloud*)(p)->useStruct)
#define BREATH_PARTICLES(p) ((EffKumokumoN64Particle*)(p)->work.cloud.breathEffect->userdata)

static inline void cloudSweep(PartyEntry* party, f32 sign, s32 i) {
    PartyCloud* cl = CLOUD_WORK(party);
    if (cl->sweep[i].reverse == 0) {
        cl->sweep[i].angle += 1.0f;
        cl = CLOUD_WORK(party);
        if (cl->sweep[i].angle >= 20.0f) {
            cl->sweep[i].angle = 20.0f;
            CLOUD_WORK(party)->sweep[i].reverse = 1;
        }
    } else {
        cl->sweep[i].angle -= 1.0f;
        cl = CLOUD_WORK(party);
        if (cl->sweep[i].angle <= -20.0f) {
            cl->sweep[i].angle = -20.0f;
            CLOUD_WORK(party)->sweep[i].reverse = 0;
        }
    }
}

// Flurrie's move target: beside Mario (3.3 units to his left or right of the facing direction), 10 units up
static inline void cloudCalcPos(PartyEntry* party) {
    MarioWork* mario = party->playerPtr;
    f32 dir;

    party->moveTarget = mario->position;
    dir = mario->displayDirectionTarget;
    dir = toMovedir(dir + ((dir < 90.0f || dir >= 270.0f) ? -90.0f : 90.0f));
    movePos(&party->moveTarget.x, &party->moveTarget.z, 3.3f, dir);
    party->moveTarget.y += 10.0f;
}

void mot_cloud() {
    MarioWork* mario = marioGetPtr();
    
    if((mario->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) != 0) {
        mario->trigFlags &= ~MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
        mario->flags &= ~(MARIO_FLAG_HAS_INPUT_JUMP|MARIO_FLAG_IS_STEPPING|MARIO_FLAG_IS_FALLING|MARIO_FLAG_IS_JUMPING);
        mario->multiTimer = 0;
        mario->airTimer = 0;
        mario->currSubMotionId = 0;
    }
}

void cloudResetAt(void) {
    MarioWork* mario = marioGetPtr();
    u32 partyId = marioGetPartyId();
    PartyEntry* party = partyGetPtr(partyId);

    if(party != NULL && party->currentMemberId == PARTY_MEMBER_FLURRIE) {
        cloud_at.x = camGetPtr(CAMERA_3D)->cameraAt.x;
        cloud_at.y = camGetPtr(CAMERA_3D)->cameraAt.y;
        cloud_at.z = camGetPtr(CAMERA_3D)->cameraAt.z;

        marioCamZoomOffReq2(600);
    }

    mario->dispFlags &= ~(MARIO_DISP_FLAG_HIDE << 8); //kDisableMakeDispDir
    mario->dispFlags &= ~(MARIO_DISP_FLAG_HIDE << 2); //kForceFrontPose
    marioAdjustMoveDir();
    mario->camShiftRate.x = 0.01f;
    mario->camShiftTargetX = 0.0f;
}

u8 cloudGetStatus(void) {
    u32 partyId = marioGetPartyId();
    PartyEntry* party = partyGetPtr(partyId);
    u8 status = 2;

    if (party == NULL || (party->currentMemberId == PARTY_MEMBER_FLURRIE && (party->flags & PARTY_FLAG_IS_BEING_USED) == 0)) {
        status = 0;
        return status;
    } else if (party->useMotionId == 10 && party->useMotionId < 20) {
        status = 1;
    }

    return status;
}

f32 cloudGetBreathDir() {
    PartyEntry* party = partyGetPtr(PARTY_SLOT_PARTY);
    f32 breathDir;

    if (party == NULL) {
        return 0.0f;
    }

    if ((party->flags & PARTY_FLAG_IS_BEING_USED) != 0 && (party->useMotionId < MARIO_MOTION_FALL)){
        breathDir = -1.0f;
    } else if (party->useStruct == NULL) {
        breathDir = 0.0f;
    } else {
        PartyCloud* cloud = (PartyCloud*)party->useStruct;
        breathDir = toMovedir(cloud->breathDir);
    }
    
    return breathDir;
}

f32 cloudGetBreathPower(f32 size, Vec* pos) {
    PartyEntry* party = partyGetPtr(PARTY_SLOT_PARTY);
    f32 breathPower;
    f32 dist;
    f32 angle;
    f32 reach;
    f32 val;

    breathPower = 0.0f;
    if (party == NULL)  {
        return breathPower;
    }

    val = 10.0f;
    if(
        party->currentMemberId == PARTY_MEMBER_FLURRIE 
        && (party->flags & PARTY_FLAG_IS_BEING_USED) != 0 
        && party->useMotionId >= MARIO_MOTION_FALL
        && ((__fabs(party->position.y - pos->y - 18.0f)) <= val)
    ) {
        PartyCloud* cloud = (PartyCloud*)party->useStruct;
        reach = cloud->breathDist;
        if (-((0.5f * size) - distABf(party->position.x, party->position.z, pos->x, pos->z)) <= reach) {
            PartyCloud* cloud = (PartyCloud*)party->useStruct;
            dist = toMovedir(cloud->breathDir);
            angle = angleABf(party->position.x, party->position.z, pos->x, pos->z);
            dist = revise360(angle - dist);
            
            if (dist <= 30.0f || dist >= 330.0f) {
                breathPower = cos((3.1415927f * dist) / 180.0f);
                val = 100.0f;
                breathPower = val * __fabsf(breathPower);
            }
        }
    }

    return breathPower;
}

struct HitEntry* cloudGetHitBreathout(s32 which) {
    u32 partyId = marioGetPartyId();
    PartyEntry* party = partyGetPtr(partyId);
    struct HitEntry* hitBreathout;

    if (party == NULL) {
        return 0;
    }

    if (
        party->currentMemberId != PARTY_MEMBER_FLURRIE 
        || (party->flags & PARTY_FLAG_IS_BEING_USED) == 0
        || party->useMotionId < MARIO_MOTION_FALL
    ){
            return 0;
    }

    hitBreathout = NULL;
    if (which == 0) {
        hitBreathout = ((PartyCloud*)party->useStruct)->hitFront;
    } else if (which == 1) {
        hitBreathout = ((PartyCloud*)party->useStruct)->hitDown;
    } else if (which == 2) {
        hitBreathout = ((PartyCloud*)party->useStruct)->hitSweep[0];
    } else if (which == 3) {
        hitBreathout = ((PartyCloud*)party->useStruct)->hitSweep[1];
    }
    
    return hitBreathout;
}

f32 cloudGetBreathDist() {
    u32 partyId = marioGetPartyId();
    PartyEntry* party = partyGetPtr(partyId);
    f32 breathDist;

    if (party == NULL) {
        breathDist = 0.0f;
        return breathDist;
    }

    if ((party->currentMemberId != PARTY_MEMBER_FLURRIE || (party->flags & PARTY_FLAG_IS_BEING_USED) == 0) || party->useMotionId < MARIO_MOTION_FALL) {
        breathDist = 0.0f;
    } else {
        PartyCloud* cloud = (PartyCloud*)party->useStruct;
        breathDist = cloud->breathDist;
    }

    return breathDist;
}

#ifdef VERSION_G8ME01
void N_cloudLockAnimationsOff(PartyEntry* party) {
    party->work.cloud.lockAnims = 0;
    partyChgPose(party, "PWD_S_1");
}

void N_cloudLockAnimationsOn(PartyEntry* party) {
    party->work.cloud.lockAnims = 1;
    partyChgPose(party, "PWD_Z_1");
}
#endif

void cloud_hover_init(PartyEntry* party) {
    party->work.cloud.hoverAngle = 0.0f;
    party->work.cloud.hoverSpeed = 4.25f;
}

void cloud_hover_main(PartyEntry* party) {
    f32 a;
    party->work.cloud.hoverAngle = revise360(party->work.cloud.hoverAngle - party->work.cloud.hoverSpeed);
    a = sin((3.1415927f * party->work.cloud.hoverAngle) / 180.0f);
    party->dispOfs.y = 3.0f + 3.0f * a;
}

void cloud_move(PartyEntry* party) {
#ifdef VERSION_G8ME01
    f32 a;

#endif
    if (party->trigFlags & PARTY_TRIG_CHG_RUN_MODE) {
        party->trigFlags &= ~PARTY_TRIG_CHG_RUN_MODE;
        partyMoveFlyInit(party, 1);
#ifdef VERSION_G8ME01
        party->work.cloud.hoverAngle = 0.0f;
        party->work.cloud.hoverSpeed = 4.25f;
    }
    if (party->work.cloud.lockAnims == 0) {
        partyFlyMain(party);
        party->work.cloud.hoverAngle = revise360(party->work.cloud.hoverAngle - party->work.cloud.hoverSpeed);
        a = sin((3.1415927f * party->work.cloud.hoverAngle) / 180.0f);
        party->dispOfs.y = 3.0f + 3.0f * a;
        if (party->moveMode != 2) {
            party->trigFlags &= ~PARTY_TRIG_HALF_PUSH;
        }
#else
        cloud_hover_init(party);
    }
    partyFlyMain(party);
    cloud_hover_main(party);
    if (party->moveMode != 2) {
        party->trigFlags &= ~PARTY_TRIG_HALF_PUSH;
#endif
    }
}

void cloud_init(PartyEntry* party) {
    party->memberKind = 5;
    party->moveKind = 1;
    party->height = 38.0f;
    party->width = 24.0f;
    party->work.cloud.reserved_174 = 0;
    party->work.cloud.breathEffect = NULL;
    party->work.cloud.hoverAngleBits = 0;
    party->work.cloud.hoverSpeedBits = 0;
#ifdef VERSION_G8ME01
    party->work.cloud.lockAnims = 0;
#endif
    party->useStruct = NULL;
    party->work.cloud.breathEffect = NULL;
}

void cloud_use(PartyEntry* party) {
    MarioWork* mario = party->playerPtr;

    N__cloud_use(party);
    if (mario->currMotionId == MARIO_MOTION_CLOUD || mario->currMotionId <= 1 || mario->currMotionId == MARIO_MOTION_DASH) {
        return;
    }
    party->flags &= ~PARTY_FLAG_IS_BEING_USED;
    if (CLOUD_WORK(party) != NULL && (u32)CLOUD_WORK(party)->sfxId != 0xFFFFFFFF) {
        psndSFXOff(CLOUD_WORK(party)->sfxId);
        CLOUD_WORK(party)->sfxId = -1;
    }
    if (party->work.cloud.breathEffect != NULL) {
        effSoftDelete(party->work.cloud.breathEffect);
        party->work.cloud.breathEffect = NULL;
    }
    cloudCalcPos(party);
    party->position = party->moveTarget;
    party->flags2 |= PARTY_DISP_FLAG_NO_MAKE_DISP_DIR;
    party->flags &= ~PARTY_FLAG_IS_EVT_USE;
    if (party->useStruct != NULL) {
        __memFree(HEAP_DEFAULT, party->useStruct);
        party->useStruct = NULL;
    }
    if (mario->currMotionId == MARIO_MOTION_DAMAGE || mario->currMotionId == MARIO_MOTION_DAMAGE_TOGE) {
        partyChgRunMode(party, 13);
    } else {
        partyChgPose(party, partyChgPoseId(party, 1));
        partyChgRunMode(party, 2);
        partyChgMot(party, 0);
    }
}

void N__cloud_use(PartyEntry* party) {
    MarioWork* mario = party->playerPtr;
    f32 ofsX, ofsZ;
    f32 nz, ny, nx, dist, hz, hy, hx;
    f32 yc, ys, dirCos, dirSin;
    HitEntry* hit;
    s32 flag;
    f32 dir;
    PartyCloud* cl;

    if (party->trigFlags & PARTY_TRIG_CHG_RUN_MODE) {
        party->trigFlags &= ~PARTY_TRIG_CHG_RUN_MODE;
        party->work.cloud.hoverAngleBits = 0;
        party->work.cloud.hoverSpeedBits = 0;
        marioAdjustMoveDir();
        party->useMotionId = 0;
        party->useStruct = __memAlloc(HEAP_DEFAULT, sizeof(PartyCloud));
        memset(party->useStruct, 0, sizeof(PartyCloud));
        CLOUD_WORK(party)->reserved_0 = 100.0f;
        CLOUD_WORK(party)->breathDistMax = 120.0f;
        CLOUD_WORK(party)->sweep[0].angle = 0.0f;
        CLOUD_WORK(party)->sweep[0].reverse = 0;
        CLOUD_WORK(party)->sweep[1].angle = 0.0f;
        CLOUD_WORK(party)->sweep[1].reverse = 1;
        party->work.cloud.breathEffect = NULL;
        if (CLOUD_WORK(party) != NULL) {
            CLOUD_WORK(party)->sfxId = -1;
        }
        if (party->flags & PARTY_FLAG_IS_EVT_USE) {
            CLOUD_WORK(party)->autoTimer = sysMsec2Frame(party->actionTime);
        }
    }

    switch (party->useMotionId) {
    case 0:
        mario->dispFlags |= MARIO_DISP_FLAG_NO_MAKE_DISP_DIR;
        party->moveFrames = 4;
        party->useMotionId = 1;
    case 1:
        cloudCalcPos(party);
        party->displayDirection = mario->displayDirection;
        party->displayDirectionTarget = mario->displayDirectionTarget;
        party->position.x += (party->moveTarget.x - party->position.x) / (f32)party->moveFrames;
        party->position.y += (party->moveTarget.y - party->position.y) / (f32)party->moveFrames;
        party->position.z += (party->moveTarget.z - party->position.z) / (f32)party->moveFrames;
        if (--party->moveFrames > 0) {
            break;
        }
        party->useMotionId = 10;
    case 10:
        cloudCalcPos(party);
        party->position = party->moveTarget;
        if (party->work.cloud.breathEffect == NULL) {
            party->work.cloud.breathEffect = effKumokumoN64Entry(0, 0, 0.0f, 0.0f, 0.0f, 0.0f, -2.0f, 0.0f);
            BREATH_PARTICLES(party)->camera = party->camId;
            BREATH_PARTICLES(party)->r1 = 0x4C;
            BREATH_PARTICLES(party)->g1 = 0xA7;
            BREATH_PARTICLES(party)->b1 = 0xCA;
            BREATH_PARTICLES(party)->r2 = 0x7D;
            BREATH_PARTICLES(party)->g2 = 0x1B;
            BREATH_PARTICLES(party)->b2 = 0xF5;
        }
        marioChgMot(MARIO_MOTION_CLOUD);
        marioChgPose("M_A_2A");
        partyChgPose(party, "PWD_A_5");
        party->moveFrames = 16;
        party->useMotionId = 11;
        mario->dispFlags |= MARIO_DISP_FLAG_NO_REAR_POSE;
        mario->dispFlags |= MARIO_DISP_FLAG_NO_MAKE_DISP_DIR;
        party->flags2 |= PARTY_DISP_FLAG_NO_MAKE_DISP_DIR;
        CLOUD_WORK(party)->breathDist = 0.0f;
        CLOUD_WORK(party)->breathDir = mario->displayDirection;
        if (!(party->flags & PARTY_FLAG_IS_EVT_USE)) {
            mario->camShiftRate.x = 0.03f;
            mario->camShiftTargetX = 70.0f * (revise360(270.0f - mario->displayDirectionTarget) <= 180.0f ? -1.0f : 1.0f);
        }
        if (CLOUD_WORK(party) != NULL && (u32)CLOUD_WORK(party)->sfxId == 0xFFFFFFFF) {
            CLOUD_WORK(party)->sfxId = psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_PARTY_CLAUD_WIND1), &party->playerPtr->position);
        }
    case 11:
        cloudCalcPos(party);
        party->position = party->moveTarget;
        if (--party->moveFrames > 0) {
            break;
        }
        partyChgPose(party, "PWD_A_6");
#ifdef VERSION_G8ME01
        if (mario->currMotionId == MARIO_MOTION_CLOUD)
#endif
        marioChgPose("M_A_2B");
        party->useMotionId = 12;
    case 12:
        cloudCalcPos(party);
        party->position = party->moveTarget;
        partyUpdateKeyData(party);
        mario->displayDirection = CLOUD_WORK(party)->breathDir;
        mario->displayDirectionTarget = CLOUD_WORK(party)->breathDir;
        party->displayDirection = CLOUD_WORK(party)->breathDir;
        party->displayDirectionTarget = CLOUD_WORK(party)->breathDir;
        ofsX = mario->position.x;
        ofsZ = mario->position.z;
        flag = 0;
        dir = mario->displayDirection;
        if (dir < 90.0f || dir >= 270.0f) {
            flag = 1;
        }
        movePos(&ofsX, &ofsZ, 3.3f, toMovedir(dir + (flag ? -90.0f : 90.0f)));
        party->position.x = ofsX;
        party->position.z = ofsZ;
        party->position.y = 10.0f + mario->position.y;
        {
            Vec zero = {0.0f, 0.0f, 0.0f};
            party->dispOfs = zero;
        }
        CLOUD_WORK(party)->breathDist += 2.0f;
        cl = CLOUD_WORK(party);
        mario = party->playerPtr;
        if (cl->breathDist >= cl->breathDistMax) {
            cl->breathDist = cl->breathDistMax;
        }
        sincosf(&dirSin, &dirCos, toMovedir(CLOUD_WORK(party)->breathDir));
        sincosf(&ys, &yc, -45.0f);
        dist = CLOUD_WORK(party)->breathDist;
        hit = hitCheckFilter(NULL, &hx, &hy, &hz, &dist, &nx, &ny, &nz,
                             party->position.x, 18.0f + mario->position.y, party->position.z,
                             dirSin, ys, dirCos);
        if (hit != NULL && (hitGetAttr(hit) & HIT_ATTR_NPC_ONLY)) {
            hit = NULL;
        }
        CLOUD_WORK(party)->hitDown = hit;
        getHitBreatheout2(party, 0.0f);
        if (++CLOUD_WORK(party)->breathLevel > 2) {
            CLOUD_WORK(party)->breathLevel = 0;
        }
        if (!(party->flags & PARTY_FLAG_IS_EVT_USE)) {
            if (party->buttonHold & PAD_BUTTON_X) {
                if (marioKeyOffChk() == 0) {
                    break;
                }
            }
        } else if (--CLOUD_WORK(party)->autoTimer > 0) {
            break;
        }
        CLOUD_WORK(party)->hitFront = NULL;
        CLOUD_WORK(party)->hitDown = NULL;
        CLOUD_WORK(party)->hitSweep[0] = NULL;
        CLOUD_WORK(party)->hitSweep[1] = NULL;
        party->moveFrames = sysMsec2Frame(200);
        party->useMotionId = 13;
        if (party->flags & PARTY_FLAG_IS_EVT_USE) {
            party->useMotionId = 20;
        }
        if (CLOUD_WORK(party) != NULL && (u32)CLOUD_WORK(party)->sfxId != 0xFFFFFFFF) {
            psndSFXOff(CLOUD_WORK(party)->sfxId);
            CLOUD_WORK(party)->sfxId = -1;
        }
        break;
    case 13:
        cloudCalcPos(party);
        party->position = party->moveTarget;
        partyUpdateKeyData(party);
        if (party->buttonTrg & PAD_BUTTON_X) {
            party->useMotionId = 10;
        }
        if (--party->moveFrames <= 0) {
            party->useMotionId = 20;
            break;
        }
        if (party->stickX == 0 && party->stickY == 0) {
            break;
        }
        party->useMotionId = 20;
    case 20:
        party->flags &= ~PARTY_FLAG_IS_BEING_USED;
        if (CLOUD_WORK(party) != NULL && (u32)CLOUD_WORK(party)->sfxId != 0xFFFFFFFF) {
            psndSFXOff(CLOUD_WORK(party)->sfxId);
            CLOUD_WORK(party)->sfxId = -1;
        }
        if (party->work.cloud.breathEffect != NULL) {
            effSoftDelete(party->work.cloud.breathEffect);
            party->work.cloud.breathEffect = NULL;
        }
        cloudCalcPos(party);
        party->position = party->moveTarget;
        party->flags2 &= ~PARTY_DISP_FLAG_NO_MAKE_DISP_DIR;
        if (mario->currMotionId == MARIO_MOTION_CLOUD) {
            mario->dispFlags &= ~MARIO_DISP_FLAG_NO_MAKE_DISP_DIR;
            mario->dispFlags &= ~MARIO_DISP_FLAG_NO_REAR_POSE;
            marioAdjustMoveDir();
            party->trigFlags |= PARTY_TRIG_HALF_PUSH;
            mario->dispFlags &= ~MARIO_DISP_FLAG_NO_MAKE_DISP_DIR;
            marioChgMot(MARIO_MOTION_STAY);
        }
        party->flags2 |= PARTY_DISP_FLAG_NO_MAKE_DISP_DIR;
        partyChgRunMode(party, 2);
        party->moveArg = 0;
        partyChgMoveMode(party, 2);
        partyChgMot(party, 1);
        if (!(party->flags & PARTY_FLAG_IS_EVT_USE)) {
            mario->camShiftRate.x = 0.01f;
            mario->camShiftTargetX = 0.0f;
        }
        party->flags &= ~PARTY_FLAG_IS_EVT_USE;
        if (party->useStruct != NULL) {
            __memFree(HEAP_DEFAULT, party->useStruct);
            party->useStruct = NULL;
        }
        break;
    }
}

void cloud_exit(PartyEntry* party) {
    MarioWork* mario = party->playerPtr;

    if (CLOUD_WORK(party) != NULL && (u32)CLOUD_WORK(party)->sfxId != 0xFFFFFFFF) {
        psndSFXOff(CLOUD_WORK(party)->sfxId);
        CLOUD_WORK(party)->sfxId = -1;
    }
    if (party->work.cloud.breathEffect != NULL) {
        effSoftDelete(party->work.cloud.breathEffect);
        party->work.cloud.breathEffect = NULL;
    }
    if (party->useStruct != NULL) {
        __memFree(HEAP_DEFAULT, party->useStruct);
        party->useStruct = NULL;
    }
    mario->dispFlags &= ~MARIO_DISP_FLAG_NO_MAKE_DISP_DIR;
    mario->dispFlags &= ~MARIO_DISP_FLAG_NO_REAR_POSE;
    marioAdjustMoveDir();
    marioChgMot(MARIO_MOTION_STAY);
    mario->camShiftRate.x = 0.01f;
    mario->camShiftTargetX = 0.0f;
}

void getHitBreatheout2(PartyEntry* party, f32 angle) {
    f32 dirSin, dirCos, ys, yc;
    f32 hx, hy, hz, dist, nx, ny, nz;
    f32 ofsZ, ofsX;
    f32 dir, sign, height;
    s16 kind;
    HitEntry* hit;
    PartyCloud* cl = CLOUD_WORK(party);
    MarioWork* mario = party->playerPtr;

    kind = cl->breathLevel;

    if (kind == 0) {
        height = 8.0f;
    } else if (kind == 1) {
        height = 20.0f;
    } else {
        if (kind == 2) {
            height = 40.0f;
        }
    }
    if (cl->breathDist >= cl->breathDistMax) {
        cl->breathDist = cl->breathDistMax;
    }
    sincosf(&ys, &yc, angle);
    dir = toMovedir(CLOUD_WORK(party)->breathDir);
    sincosf(&dirSin, &dirCos, dir);
    dist = CLOUD_WORK(party)->breathDist;
    hit = hitCheckFilter(NULL, &hx, &hy, &hz, &dist, &nx, &ny, &nz,
                         party->position.x, mario->position.y + height, party->position.z,
                         dirSin, ys, dirCos);
    if (hit != NULL && (hitGetAttr(hit) & HIT_ATTR_NPC_ONLY)) {
        hit = NULL;
    }
    CLOUD_WORK(party)->hitFront = hit;
    ofsX = 0.0f;
    ofsZ = 0.0f;
    if (party->work.cloud.breathEffect != NULL) {
        movePos(&ofsX, &ofsZ, 24.0f, dir);
        BREATH_PARTICLES(party)->x = party->position.x + ofsX;
        BREATH_PARTICLES(party)->y = 18.0f + party->position.y;
        BREATH_PARTICLES(party)->z = party->position.z + ofsZ;
        cl = CLOUD_WORK(party);
        BREATH_PARTICLES(party)->scale = 0.6f * (cl->breathDist / cl->breathDistMax);
        cl = CLOUD_WORK(party);
        if (cl->breathDir >= 90.0f && cl->breathDir <= 270.0f) {
            BREATH_PARTICLES(party)->rot = -25.0f;
        } else {
            BREATH_PARTICLES(party)->rot = 205.0f;
        }
    }
    cl = CLOUD_WORK(party);
    if (cl->breathDir >= 90.0f && cl->breathDir <= 270.0f) {
        sign = 1.0f;
    } else {
        sign = -1.0f;
    }

    cloudSweep(party, sign, 0);
    cl = CLOUD_WORK(party);
    sincosf(&dirSin, &dirCos, toMovedir(cl->sweep[0].angle * sign + cl->breathDir));
    dist = CLOUD_WORK(party)->breathDist;
    hit = hitCheckFilter(NULL, &hx, &hy, &hz, &dist, &nx, &ny, &nz,
                         party->position.x, mario->position.y + height, party->position.z,
                         dirSin, ys, dirCos);
    if (hit != NULL && (hitGetAttr(hit) & HIT_ATTR_NPC_ONLY)) {
        hit = NULL;
    }
    CLOUD_WORK(party)->hitSweep[0] = hit;

    cloudSweep(party, sign, 1);
    cl = CLOUD_WORK(party);
    sincosf(&dirSin, &dirCos, toMovedir(cl->sweep[1].angle * sign + cl->breathDir));
    dist = CLOUD_WORK(party)->breathDist;
    hit = hitCheckFilter(NULL, &hx, &hy, &hz, &dist, &nx, &ny, &nz,
                         party->position.x, mario->position.y + height, party->position.z,
                         dirSin, ys, dirCos);
    if (hit != NULL && (hitGetAttr(hit) & HIT_ATTR_NPC_ONLY)) {
        hit = NULL;
    }
    CLOUD_WORK(party)->hitSweep[1] = hit;
}

void cloudGetAt(Vec* position) {
    MarioWork* mario = marioGetPtr();
    u32 partyId = marioGetPartyId();
    PartyEntry* party = partyGetPtr(partyId);

    if (party == NULL || (party->currentMemberId == PARTY_MEMBER_FLURRIE && (party->flags & PARTY_FLAG_IS_BEING_USED) == 0)) {
        return;
    }

    if ((mario->dispFlags & (MARIO_DISP_FLAG_HIDE << 20)) == 0) {
        CameraEntry* camera = camGetPtr(CAMERA_3D);

        if(camera->stateA == 0) {
            cloud_at = *position;
        } else {
            *position = cloud_at;
        }
    }
}

void partyCloudStrippedPool(f32* f, s32 i) {
    f[0] = 0.0f;
    f[1] = 70.0f;
    f[2] = 180.0f;
    f[3] = (f32)i;
}
