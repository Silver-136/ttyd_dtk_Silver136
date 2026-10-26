#include "party.h"
#include "driver/camdrv.h"
#include "driver/hitdrv.h"
#include "mario/mario.h"
#include "mario/mario_party.h"
#include "mario/mario_sbr.h"
#include "pmario_sound.h"
#include "system.h"
#include <dolphin/mtx.h>
#include <string.h>
#include "party/party_nokonoko.h"
#include "party/party_chuchu.h"
#include "party/party_hit.h"
#include "party/party_motion.h"
#include "party/party_bomhei.h"
#include "party/party_cloud.h"
#include "mot/mot_stay.h"
#include "mot/mot_damage.h"
#include "mot/mot_slit.h"
#include "party/party_move.h"
#include "driver/mobjdrv.h"
#include "libc_decls.h"
#include <math.h>
#include "sfx_name_word.h"
#include "driver/npcdrv.h"
#include "mario/mario_hit.h"
#include "mario/mariost.h"
#include "party/party_vivian.h"

s32 partyMoveMain(PartyEntry* party, s32 hosei);
f32 partyUpdMoveSpd(PartyEntry* party, f32 spd);
extern void party_set_behind_dir(PartyEntry* party);
static void flyMain(PartyEntry* party);

static inline void partyModeTail(PartyEntry* party) {
    Vec pos;

    party->moveDir = party->targetDir;
    if (party->trigFlags & PARTY_TRIG_SPLASH) {
        party->trigFlags &= ~PARTY_TRIG_SPLASH;
        pos = party->position;
        marioSetSplash(0, &pos);
        party->splashTimer = 20;
    }
    if (--party->splashTimer < 0) {
        party->splashTimer = 0;
    }
    if (party->runMode != 13) {
        if (party->hitobjStandOn && (hitGetAttr(party->hitobjStandOn) & HIT_ATTR_TOGE)) {
            partyChgRunMode(party, 13);
        } else if ((void*)party->hitobjFront && (hitGetAttr((void*)party->hitobjFront) & HIT_ATTR_TOGE)) {
            partyChgRunMode(party, 13);
        } else if ((void*)party->hitobjHead && (hitGetAttr((void*)party->hitobjHead) & HIT_ATTR_TOGE)) {
            partyChgRunMode(party, 13);
        }
    }
}

Footmark footmarkBuf[2][80];
s32 writeId[2];
s32 readId[2];

static const char* objNames[] = {
    "MOBJ_TreasureBox",
    "MOBJ_BigTreasureBox",
    "MOBJ_GrayTreasureBox",
    "MOBJ_BlackTreasureBox",
    "MOBJ_GoldenTreasureBox",
    "MOBJ_BlueJumpStand",
    "MOBJ_BlueJumpStand2",
    "MOBJ_RedJumpStand",
    "MOBJ_RedJumpStand2",
    "MOBJ_MapJumpStand",
    "MOBJ_SaveBlock",
    "MOBJ_RecoveryBlock",
    "MOBJ_CoinTakenRecoveryBlock",
    "MOBJ_eki_06_su1",
    "MOBJ_eki_06_su2",
    "MOBJ_eki_06_su3",
    "MOBJ_RedSwitch",
    "MOBJ_BlueSwitch",
    "MOBJ_WhiteSwitch",
    "MOBJ_BlackSwitch",
    "MOBJ_green_FlowBlockSwitch",
    "MOBJ_purple_FlowBlockSwitch",
    "MOBJ_orange_FlowBlockSwitch",
    "MOBJ_RedTimerSwitch",
    "MOBJ_BlueTimerSwitch",
    NULL,
};

Footmark* partyGetFootmarkPtr(s32 id) {
    return &footmarkBuf[id][readId[id]];
}

s32 partyGetFootmarkReadId(s32 id) {
    return readId[id];
}

s32 partyGetFootmarkWriteId(s32 id) {
    return writeId[id];
}

s32 partyGetFootmarkId(PartyEntry* party) {
    return (party->flags >> 2) & 1;
}

static inline void getTargetPos(s32 id, Vec* pos) {
    MarioWork* mario = marioGetPtr();
    BOOL far = FALSE;
    MarioWork* mario2 = marioGetPtr();
    PartyEntry* other = anotherPartyGetPtr(id != 0);

    if (other) {
        if (nokonokoGetStatus(other) == 3 ||
            PSVECDistance(&mario2->position, &other->position) >= 150.0f) {
            far = TRUE;
        }
    }
    if (far) {
        other = NULL;
    }
    if (other) {
        pos->x = other->position.x;
        pos->y = other->position.y;
        pos->z = other->position.z;
    } else {
        pos->x = mario->position.x;
        pos->y = mario->position.y;
        pos->z = mario->position.z;
    }
}

void L_getTargetPos(s32 id, Vec* pos) {
    MarioWork* mario2;
    MarioWork* mario;
    PartyEntry* other;
    BOOL far;

    mario = marioGetPtr();
    far = FALSE;
    mario2 = marioGetPtr();
    other = anotherPartyGetPtr(id != 0);

    if (other) {
        if (nokonokoGetStatus(other) == 3 ||
            PSVECDistance(&mario2->position, &other->position) >= 150.0f) {
            far = TRUE;
        }
    }
    if (far) {
        other = NULL;
    }
    if (other) {
        pos->x = other->position.x;
        pos->y = other->position.y;
        pos->z = other->position.z;
    } else {
        pos->x = mario->position.x;
        pos->y = mario->position.y;
        pos->z = mario->position.z;
    }
}

f32 partyGetTargetDir(s32 id, f32 x, f32 z) {
    MarioWork* mario = marioGetPtr();
    BOOL far = FALSE;
    PartyEntry* other = anotherPartyGetPtr(id != 0);
    Vec pos;

    if (other) {
        if (nokonokoGetStatus(other) == 3 ||
            PSVECDistance(&mario->position, &other->position) >= 150.0f) {
            far = TRUE;
        }
    }
    if (other && !far) {
        getTargetPos(id, &pos);
        return angleABf(x, z, pos.x, pos.z);
    }
    return angleABf(x, z, mario->position.x, mario->position.z);
}

f32 partyGetTargetDist(s32 id, f32 x, f32 z) {
    MarioWork* mario = marioGetPtr();
    BOOL far = FALSE;
    PartyEntry* other = anotherPartyGetPtr(id != 0);
    Vec pos;

    if (id == 0) {
        return distABf(x, z, mario->position.x, mario->position.z);
    }
    if (other) {
        if (nokonokoGetStatus(other) == 3 ||
            PSVECDistance(&mario->position, &other->position) >= 150.0f) {
            far = TRUE;
        }
    }
    if (other && !far) {
        getTargetPos(id, &pos);
        return distABf(x, z, pos.x, pos.z);
    }
    return distABf(x, z, mario->position.x, mario->position.z);
}

f32 partyGetTargetDist3D(s32 id, Vec* p) {
    MarioWork* mario = marioGetPtr();
    BOOL far = FALSE;
    PartyEntry* other = anotherPartyGetPtr(id != 0);
    Vec pos;

    if (id == 0) {
        return PSVECDistance(&marioGetPtr()->position, p);
    }
    if (other) {
        if (nokonokoGetStatus(other) == 3 ||
            PSVECDistance(&mario->position, &other->position) >= 150.0f) {
            far = TRUE;
        }
    }
    if (other && !far) {
        getTargetPos(id, &pos);
        return PSVECDistance(&pos, p);
    }
    return PSVECDistance(&marioGetPtr()->position, p);
}

f32 partyGetTargetDistY(s32 id, f32 y) {
    MarioWork* mario = marioGetPtr();
    BOOL far = FALSE;
    PartyEntry* other = anotherPartyGetPtr(id != 0);
    Vec pos;

    if (id == 0) {
        return __fabsf(mario->position.y - y);
    }
    if (other) {
        if (nokonokoGetStatus(other) == 3 ||
            PSVECDistance(&mario->position, &other->position) >= 150.0f) {
            far = TRUE;
        }
    }
    if (other && !far) {
        getTargetPos(id, &pos);
        return __fabsf(pos.y - y);
    }
    return __fabsf(mario->position.y - y);
}

f32 partyGetTargetMovespd(s32 id) {
    MarioWork* mario = marioGetPtr();
    BOOL far;
    PartyEntry* other;

    if (id == 0) {
        if (mario->baseSpeed > 0.0f) {
            return mario->baseSpeed;
        }
        return mario->mBaseDashSpeed;
    }
    far = FALSE;
    other = anotherPartyGetPtr(id != 0);
    if (other) {
        if (nokonokoGetStatus(other) == 3 ||
            PSVECDistance(&mario->position, &other->position) >= 150.0f) {
            far = TRUE;
        }
    }
    if (other && !far) {
        return other->moveSpeed;
    }
    mario = marioGetPtr();
    if (mario->baseSpeed > 0.0f) {
        return mario->baseSpeed;
    }
    return mario->mBaseDashSpeed;
}

f32 partyGetTargetMovespd2(PartyEntry* party) {
    MarioWork* mario = party->playerPtr;
    BOOL far;
    PartyEntry* other;

    if (!(party->flags & PARTY_FLAG_IS_PARTNER)) {
        if (mario->baseSpeed > 0.0f) {
            return mario->baseSpeed;
        }
        return mario->mBaseDashSpeed;
    }
    if (party->trigFlags & PARTY_TRIG_FOLLOW_CLOSE) {
        if (mario->baseSpeed > 0.0f) {
            return mario->baseSpeed;
        }
        return mario->mBaseDashSpeed;
    }
    far = FALSE;
    other = partyGetPtr(marioGetPartyId());
    if (other) {
        if (nokonokoGetStatus(other) == 3 ||
            PSVECDistance(&mario->position, &other->position) >= 150.0f) {
            far = TRUE;
        }
    }
    if (other && !far) {
        do { return other->moveSpeed; } while (0);
    }
    if (mario->baseSpeed > 0.0f) {
        return mario->baseSpeed;
    }
    return mario->mBaseDashSpeed;
}

void partyNextReadId(s32 id) {
    if (readId[id] != writeId[id]) {
        readId[id]++;
        if (readId[id] >= 80) {
            readId[id] = 0;
        }
    }
}

void partyGetReadId(s32 id, f32* x, f32* y, f32* z) {
    Footmark* fm = &footmarkBuf[id][readId[id]];
    *x = fm->pos.x;
    *y = fm->pos.y;
    *z = fm->pos.z;
}

void partyGetReadId2(s32 id, Vec* pos) {
    Footmark* fm = &footmarkBuf[id][readId[id]];
    pos->x = fm->pos.x;
    pos->y = fm->pos.y;
    pos->z = fm->pos.z;
}

void partyClearFootmark(void) {
    Vec pos = marioGetPtr()->position;
    u32 i;

    for (i = 0; i < 80; i++) {
        footmarkBuf[0][i].pos = pos;
        footmarkBuf[0][i].flag = 0;
    }
    writeId[0] = 0;
    readId[0] = 0;
}

void partyClearFootmark2(void) {
    MarioWork* mario = marioGetPtr();
    PartyEntry* other = partyGetPtr(marioGetPartyId());
    Vec pos;
    u32 i;
    Footmark* fm;

    if (other) {
        if (nokonokoGetStatus(other) == 3 ||
            PSVECDistance(&mario->position, &other->position) >= 150.0f) {
            pos = mario->position;
        } else {
            pos = other->position;
        }
    } else {
        pos = mario->position;
    }
    fm = footmarkBuf[1];
    for (i = 0; i < 80; i++) {
        fm->pos = pos;
        fm->flag = 0;
        fm++;
    }
    writeId[1] = 0;
    readId[1] = 0;
}

static inline BOOL partyChkMarioJumpFlag(void) {
    MarioWork* mario = marioGetPtr();
    BOOL ret = FALSE;
    if (mario->flags & MARIO_FLAG_IS_JUMPING) {
        ret = TRUE;
    }
    return ret;
}

static inline void partyPutFootmark(s32 id, Vec pos, u32 flags) {
    MarioWork* mario = marioGetPtr();
    Footmark* fm = &footmarkBuf[id][writeId[id]];

    if ((fm->flag & FOOTMARK_FLAG_JUMP) && (flags & FOOTMARK_FLAG_JUMP)) {
        return;
    }
    if (fm->pos.x == pos.x && fm->pos.y == pos.y && fm->pos.z == pos.z) {
        return;
    }
    if (readId[id] == writeId[id] + 1) {
        return;
    }
    writeId[id]++;
    if (writeId[id] >= 80) {
        writeId[id] = 0;
    }
    fm = &footmarkBuf[id][writeId[id]];
    fm->pos = pos;
    if (flags & FOOTMARK_FLAG_JUMP) {
        fm->flag |= FOOTMARK_FLAG_JUMP;
    } else {
        fm->flag &= ~FOOTMARK_FLAG_JUMP;
    }
    if (flags & FOOTMARK_FLAG_SLIT) {
        fm->flag |= FOOTMARK_FLAG_SLIT;
    } else {
        fm->flag &= ~FOOTMARK_FLAG_SLIT;
    }
}

static inline BOOL partyChkOtherFlag(PartyEntry* other) {
    BOOL ret = FALSE;
    if (other->flags & PARTY_FLAG_IS_JUMPING) {
        ret = TRUE;
    }
    return ret;
}

void partyRecordFootmark(PartyEntry* party) {
    MarioWork* mario = party->playerPtr;
    u32 flags = 0;
    PartyEntry* other;
    BOOL far;

    if (party->currentSlotId == 0) {
        if (partyChkMarioJumpFlag()) {
            flags |= FOOTMARK_FLAG_JUMP;
        }
        if (marioChkSlitThrouh()) {
            flags |= FOOTMARK_FLAG_SLIT;
        }
        partyPutFootmark(partyGetFootmarkId(party), mario->position, flags);
    } else {
        far = FALSE;
        other = partyGetPtr(marioGetPartyId());
        if (other && (nokonokoGetStatus(other) == 3 ||
                      PSVECDistance(&mario->position, &other->position) >= 150.0f)) {
            far = TRUE;
        }
        if (other && !far) {
            if (party->footmarkMode == 0) {
                party->footmarkMode = 1;
                partyClearFootmark();
            }
            if (partyChkOtherFlag(other)) {
                flags |= FOOTMARK_FLAG_JUMP;
            }
            partyPutFootmark(partyGetFootmarkId(party), other->position, flags);
        } else {
            if (party->footmarkMode == 1) {
                party->footmarkMode = 0;
                ((void (*)(void))partyClearFootmark2)();
            }
            if (partyChkMarioJumpFlag()) {
                flags |= 1;
            }
            if (marioChkSlitThrouh()) {
                flags |= 4;
            }
            partyPutFootmark(partyGetFootmarkId(party), mario->position, flags);
        }
    }
}

void partyClearJumpPara(PartyEntry* party) {
    party->flags &= ~(PARTY_FLAG_IS_JUMPING | PARTY_FLAG_IS_FALLING | PARTY_FLAG_IS_STEPPING);
    party->velY = 0.0f;
    party->accelY = 0.0f;
    party->gravity = 0.0f;
}

static inline f32 partySpdRate(PartyEntry* party) {
    return (party->flags & PARTY_FLAG_IN_SHALLOW_WATER) ? 0.75f : 1.0f;
}

static inline f32 partyUpdSpd(PartyEntry* party) {
    MarioWork* mario = party->playerPtr;
    f32 f;
    f32 target;
    f32 cur;

    cur = party->moveSpeed;
    if (mario->flags & MARIO_FLAG_IS_PUSHING) {
        target = mario->baseSpeed;
    } else {
        target = party->targetSpeed * mario->playerGravity * partySpdRate(party);
    }
    if (target > 0.0f) {
        f = cur + 0.05f * ((0.5f + target) - cur);
        if (f >= target) {
            f = target;
        }
        party->moveSpeed = f;
        return f;
    }
    f = cur + 0.1f * (-1.0f - cur);
    if (f <= 0.0f) {
        f = 0.0f;
    }
    party->moveSpeed = f;
    return f;
}

static inline f32 partyUpdSpd2(PartyEntry* party) {
    MarioWork* mario = party->playerPtr;
    f32 f;
    f32 cur;
    f32 target;

    cur = party->moveSpeed;
    if (mario->flags & MARIO_FLAG_IS_PUSHING) {
        target = mario->baseSpeed;
    } else {
        target = party->targetSpeed * mario->playerGravity * partySpdRate(party);
    }
    if (target > 0.0f) {
        f = cur + 0.05f * ((0.5f + target) - cur);
        if (f >= target) {
            f = target;
        }
        party->moveSpeed = f;
        return f;
    }
    f = cur + 0.1f * (-1.0f - cur);
    if (f <= 0.0f) {
        f = 0.0f;
    }
    party->moveSpeed = f;
    return f;
}

static inline f32 partyChkUpdSpd(PartyEntry* party) {
    if (party->forceMoveFrames != 0 || party->motionId == 2 || party->motionId == 3 || party->motionId == 6) {
        return party->moveSpeed;
    }
    return partyUpdSpd2(party);
}

f32 partySetTargetSpd(PartyEntry* party, f32 spd) {
    if (spd) {
        party->targetSpeed = spd;
        partyChkUpdSpd(party);
        return spd;
    }
    party->targetSpeed = 0.0f;
    if (spd) {
    }
    return spd;
}

f32 partyForceSetSpd(PartyEntry* party, f32 spd) {
    f32 t = spd * party->playerPtr->playerGravity;
    party->targetSpeed = t * partySpdRate(party);
    party->moveSpeed = party->targetSpeed;
    return spd;
}

f32 partyUpdMoveSpd(PartyEntry* party, f32 spd) {
    if (party->forceMoveFrames != 0 || party->motionId == 2 || party->motionId == 3 || party->motionId == 6) {
        return party->moveSpeed;
    }
    return partyUpdSpd(party);
}

void N_partyPlaySfxPartyLanding1(PartyEntry* party) {
    psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_PARTY_LANDING1), &party->position);
}

void partyWalkSe(PartyEntry* party, s32 arg) {
    s32 a;
    s32 b;

    party->footstepTimer++;
    if (party->currentMemberId == PARTY_MEMBER_FLAVIO) {
        if (arg == 0) {
            a = 20;
            b = 40;
        } else {
            a = 10;
            b = 20;
        }
        if (party->footstepTimer >= b) {
            party->footstepTimer = 0;
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_STG5_MARCO_MOVE1R), &party->position);
        } else if (party->footstepTimer == a) {
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_STG5_MARCO_MOVE1L), &party->position);
        }
    } else if (party->currentMemberId == PARTY_MEMBER_EGG) {
        if (party->footstepTimer >= 15) {
            party->footstepTimer = 0;
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_PARTY_YOSH_EGG1), &party->position);
        }
    } else if (party->currentMemberId == PARTY_MEMBER_PUNIO) {
        if (arg == 0) {
            a = 5;
            b = 10;
        } else {
            a = 5;
            b = 10;
        }
        if (party->footstepTimer >= b) {
            party->footstepTimer = 0;
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_STG0_PUNI_MOVE1), &party->position);
        } else if (party->footstepTimer == a) {
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_STG0_PUNI_MOVE1), &party->position);
        }
    } else if (party->moveKind == 1) {
        if (party->footstepTimer >= (!arg ? 40 : 20)) {
            party->footstepTimer = 0;
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_PARTY_MOVE2), &party->position);
        }
    } else {
        if (arg == 0) {
            a = 20;
            b = 40;
        } else {
            a = 10;
            b = 20;
        }
        if (party->footstepTimer >= b) {
            party->footstepTimer = 0;
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_PARTY_MOVE1R), &party->position);
        } else if (party->footstepTimer == a) {
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_PARTY_MOVE1L), &party->position);
        }
    }
}

void partyChgStayPose(PartyEntry* party) {
    if (party->runMode != 0) {
        if (marioChkDeepSleep() == TRUE || (party->flags & PARTY_FLAG_SLEEP_ON)) {
            partyChgPoseId(party, 6);
        } else if (party->currentMemberId != PARTY_MEMBER_MS_MOWZ) {
            partyChgPoseId(party, 1);
        } else if (!chuchu_searchObject()) {
            partyChgPoseId(party, 1);
        }
    }
}

void partyChgWalkPose(PartyEntry* party, f32 spd) {
    f32 a;
    f32 b;

    if (party->flags2 & PARTY_DISP_FLAG_SLIT) {
        partyChgPoseId(party, 3);
        partyWalkSe(party, 1);
        return;
    }
    a = (1.0f == party->playerPtr->playerGravity) ? 1.65f : 2.0f;
    b = (1.0f == party->playerPtr->playerGravity) ? 0.8f : 0.0f;
    if (a - b < spd && spd < spd + b) {
        if (N_marioGetMovementSpeed() == 1) {
            partyChgPoseId(party, 2);
            partyWalkSe(party, 0);
        } else {
            partyChgPoseId(party, 3);
            partyWalkSe(party, 1);
        }
    } else if (spd >= a) {
        partyChgPoseId(party, 3);
        partyWalkSe(party, 1);
    } else {
        partyChgPoseId(party, 2);
        partyWalkSe(party, 0);
    }
}

void partySetDispDirToMario(PartyEntry* party) {
    f32 dir = angleABf(party->position.x, party->position.z,
                       party->playerPtr->position.x, party->playerPtr->position.z);
    f32 a = revise360(camGetPtr(CAMERA_3D)->viewYaw + 270.0f - dir);
    BOOL b = FALSE;
    if (a >= 90.0f && a <= 270.0f) {
        b = TRUE;
    }
    party->displayDirectionTarget = b ? 180.0f : 0.0f;
}

BOOL partyChkNoStandMobj(HitEntry* hit) {
    MapObjectEntry* mobj;
    BOOL ret;
    const char** names;

    if (!hit) {
        return FALSE;
    }
    if (!(hitGetAttr(hit) & HIT_ATTR_MOBJ)) {
        return FALSE;
    }
    names = objNames;
    mobj = mobjHitObjPtrToPtr(hit);
    ret = FALSE;
    while (*names) {
        if (strcmp(mobj->animName, *names) == 0) {
            ret = TRUE;
            break;
        }
        names++;
    }
    return ret;
}

void partyMove(PartyEntry* party) {
    partyMoveMain(party, 1);
}

void partyMoveNoHosei(PartyEntry* party) {
    partyMoveMain(party, 0);
}

s32 partyMoveMain(PartyEntry* party, s32 hosei) {
    Vec pos;
    Vec pos2;
    f32 spd;
    f32 dir;
    f32 h;
    HitEntry* hit;
    f32 v;

    party->hitobjFront = NULL;
    party->hitobjPush = NULL;
    pos = party->position;
    spd = 0.0f;
    if (hosei) {
        partyGetMoveDirSpd(party, &dir, &spd);
    } else {
        dir = party->targetDir;
        spd = party->moveSpeed * ((party->flags & PARTY_FLAG_IN_SHALLOW_WATER) ? 0.75f : 1.0f);
    }
    if (!(party->flags & PARTY_FLAG_DISABLE_FRONT_WALL_CHK)) {
        hit = partySearchFrontWall(party, &pos, spd, dir);
        if (hit) {
            party->hitobjFront = hit;
            if (party->moveSpeed) {
                party->hitobjPush = hit;
            }
        }
        if (hit) {
            party->position.x = pos.x;
            party->position.z = pos.z;
        } else {
#ifdef VERSION_G8ME01
            partyChkWallAround(party, &pos, spd, dir, party->width, party->height);
#else
            marioChkWallAround(&pos, 1, spd, dir, party->width, party->height);
#endif
            party->position.x = pos.x;
            party->position.z = pos.z;
        }
    } else {
        movePos(&party->position.x, &party->position.z, spd, dir);
    }
    if (!(party->flags & PARTY_FLAG_FORCE_FALL)) {
        pos2 = party->position;
        h = party->height;
        hit = partySearchHead(party, &pos2, &h, party->displayDirection);
        if (hit) {
            party->hitobjHead = hit;
        }
    }
    if (party->flags & PARTY_FLAG_IS_JUMPING) {
        dir = toMovedirSimple(party->displayDirectionTarget);
        if (!(void*)party->hitobjHead) {
            party->position.y += party->velY;
        }
        party->velY += party->accelY;
        party->flags &= ~PARTY_FLAG_IS_GROUNDED;
        return 1;
    }
    hit = NULL;
    if (!(party->flags & PARTY_FLAG_FORCE_FALL) && party->velY <= 0.0f) {
        v = __fabsf(party->velY);
        hit = partySearchGround(party, v, v);
        pos.y = party->groundY;
        if (hit) {
            if (hitGetAttr(hit) & HIT_ATTR_WATER_HAZARD) {
                party->hitobjStandOn = hit;
                party->flags &= ~PARTY_FLAG_IS_GROUNDED;
                if (!(party->flags & PARTY_FLAG_IS_FALLING)) {
                    party->flags |= PARTY_FLAG_IS_FALLING;
                    if (!party->accelY) {
                        party->accelY = -1.4f;
                    }
                }
                party->position.y += party->velY;
                party->velY += party->accelY;
                return 2;
            }
            if (party->dropDepth != 0.0f) {
                hit = NULL;
            } else {
                party->position.y = pos.y;
            }
        }
    }
    party->hitobjStandOn = hit;
    if (!hit) {
        if (!(party->flags & PARTY_FLAG_DISABLE_FLOORS)) {
            party->flags &= ~PARTY_FLAG_IS_GROUNDED;
            if (!(party->flags & PARTY_FLAG_IS_FALLING)) {
                party->flags |= PARTY_FLAG_IS_FALLING;
                if (!party->accelY) {
                    party->accelY = -1.4f;
                }
            }
            party->position.y += party->velY;
            party->velY += party->accelY;
            return 2;
        }
    } else if (party->flags & PARTY_FLAG_IS_FALLING) {
        party->flags &= ~PARTY_FLAG_IS_FALLING;
        party->flags |= PARTY_FLAG_IS_GROUNDED;
        party->velY = 0.0f;
        party->accelY = 0.0f;
        party->position.y = pos.y;
        return 3;
    } else {
        party->position.y = pos.y;
    }
    return 0;
}

void partyChgMoveMode(PartyEntry* party, s32 mode) {
    // The self-copy in case 4/5 keeps the arm alive through switch lowering
    // (the target's cmpwi 4; bge range check) and then coalesces to nothing.
    PartyEntry* p = party;
    switch (p->moveMode) {
        case 6:
            p->flags &= ~0x8000;
            break;
        case 4:
        case 5:
            p = party;
            break;
    }
    party = p;
    party->moveMode = mode;
    party->trigFlags |= PARTY_TRIG_CHG_MOVE_MODE;
}

void partyChgMot(PartyEntry* party, s32 mot) {
    if (party->motionId == 3) {
        party->flags &= ~(PARTY_FLAG_IS_JUMPING | PARTY_FLAG_IS_FALLING | PARTY_FLAG_IS_STEPPING);
        party->flags &= ~(PARTY_FLAG_DISABLE_FRONT_WALL_CHK | PARTY_FLAG_FORCE_FALL);
        party->flags &= ~PARTY_FLAG_DISABLE_FLOORS;
    }
    party->motionId = mot;
    party->trigFlags |= PARTY_TRIG_CHG_MOT;
}

void partySetDir(PartyEntry* party, f32 x, f32 z) {
    party->targetDir = angleABf(party->position.x, party->position.z, x, z);
}

void partyMoveBehindMario(PartyEntry* party, s32 arg) {
    if (party->currentMemberId == PARTY_MEMBER_YOSHI && party->runMode == 3) {
        return;
    }
    party->moveArg = arg;
    partyChgMoveMode(party, 2);
    partyChgMot(party, 1);
}

void partyMoveBehindMario3(PartyEntry* party, s32 arg) {
    if (party->currentMemberId == PARTY_MEMBER_YOSHI && party->runMode == 3) {
        return;
    }
    party->trigFlags |= PARTY_TRIG_BEHIND_DIR_SET;
    party->trigFlags |= PARTY_TRIG_HALF_PUSH;
    party_set_behind_dir(party);
    party->moveArg = arg;
    partyChgMoveMode(party, 2);
    partyChgMot(party, 1);
}

void partyMoveBesideMario(PartyEntry* party, s32 arg) {
    if (party->currentMemberId == PARTY_MEMBER_YOSHI && party->runMode == 3) {
        return;
    }
    party->moveArg = arg;
    partyChgMoveMode(party, 3);
    partyChgMot(party, 1);
}

void partyMoveCheckWall(PartyEntry* party) {
    f32 spd;
    f32 dir;
    Vec pos;
    HitEntry* hit;

    pos = party->position;
    partyGetMoveDirSpd(party, &dir, &spd);
    hit = partySearchFrontWall(party, &pos, spd, dir);
    if (hit) {
        party->hitobjFront = hit;
        if (party->moveSpeed) {
            party->hitobjPush = hit;
        }
    }
    party->position.x = pos.x;
    party->position.z = pos.z;
    hit = partySearchGround(party, 10.0f, 10.0f);
    pos.y = party->groundY;
    if (hit && (hitGetAttr(hit) & HIT_ATTR_WATER_HAZARD)) {
        hit = NULL;
    }
    party->hitobjStandOn = hit;
    if (!hit) {
        party->position.y += party->velY;
        if (party->accelY == 0.0f) {
            party->accelY = -1.4f;
        }
        party->velY += party->accelY;
    }
}

s32 partyMarioHosei(PartyEntry* party);

void partyGetMoveDirSpd(PartyEntry* party, f32* dir, f32* spd) {
    MarioWork* mario = party->playerPtr;
    s32 hosei;
    BOOL far;
    PartyEntry* p;
    f32 s;
    f32 c;
    f32 x;
    f32 z;
    f32 mul;

    hosei = partyMarioHosei(party);
    if (hosei && party->moveMode != 2) {
        if (!(party->flags & PARTY_FLAG_IS_PARTNER)) {
            partyClearFootmark();
        } else {
            far = FALSE;
            p = anotherPartyGetPtr(1);
            if (p && (nokonokoGetStatus(p) == 3 ||
                      PSVECDistance(&mario->position, &p->position) >= 150.0f)) {
                far = TRUE;
            }
            if (far) {
                partyClearFootmark2();
            }
        }
    }
    if (party->trigFlags & PARTY_TRIG_DOOR_MOVE) {
        hosei = 0;
    }
    if (hosei >= 30) {
        party->pushCount = 0;
        partyMoveBehindMario(party, 0);
    }
    sincosf(&s, &c, party->targetDir);
    x = party->floorMoveX + party->pushVelX + party->moveSpeed * s;
    z = party->floorMoveZ + party->pushVelZ + party->moveSpeed * c;
    *dir = angleABf(0.0f, 0.0f, x, z);
    mul = (party->flags & PARTY_FLAG_IN_SHALLOW_WATER) ? 0.75f : 1.0f;
    *spd = mul * sqrtf(x * x + z * z);
}

s32 partyMarioHosei(PartyEntry* party) {
    MarioWork* mario = party->playerPtr;
    f32 kx;
    f32 kz;
    f32 ky;
    f32 w;
    f32 h;
    f32 a;
    f32 dir;
    f32 scale;
    f32 d;

    if (!(party->trigFlags & PARTY_TRIG_HALF_PUSH)) {
        kx = 0.4f;
        kz = 0.3f;
        ky = kx;
    } else {
        kx = 0.2f;
        kz = 0.15f;
        ky = kx;
    }
    if (&party->position) {
    }
    if (mario->flags & MARIO_FLAG_IS_PUSHING) {
        kx = 0.2f;
        kz = 0.15f;
        ky = kx;
    }
    w = party->width + mario->wPlayerCollisionBox.x;
    h = mario->wPlayerCollisionBox.y + party->height;
    party->pushVelX = 0.0f;
    kx *= w;
    kz *= w;
    party->pushVelZ = 0.0f;
    ky *= h;
    if (__fabs(party->position.x - mario->position.x) <= kx &&
        __fabs(party->position.z - mario->position.z) <= kz &&
        __fabs(party->position.y - mario->position.y) <= ky) {
        dir = angleABf(mario->position.x, mario->position.z, party->position.x, party->position.z);
        if (mario->baseSpeed) {
            d = dir - mario->directionView;
            if (__fabsf(d) <= 4.0f) {
                if (dir - mario->directionView >= 0.0f) {
                    dir += 90.0f;
                } else {
                    dir -= 90.0f;
                }
            }
            if (++party->pushCount > 100) {
                party->pushCount = 100;
            }
            scale = 1.0f;
        } else {
            dir = angleABf(mario->position.x, mario->position.z, party->position.x, party->position.z);
            scale = 4.0f;
        }
        if (mario->baseSpeed > 1.0f) {
            scale = 0.8f * mario->baseSpeed;
        }
        a = (3.14159265f * dir) / 180.0f;
        party->pushVelX = scale * (f32)sin(a);
        party->pushVelZ = scale * -(f32)cos(a);
    } else if (party->pushCount > 0) {
        party->pushCount--;
    }
    return party->pushCount;
}

void partyChkWall(PartyEntry* party) {
    f32 spd;
    f32 dir;
    Vec pos;
    HitEntry* hit;

    pos = party->position;
    party->moveSpeed = 0.0f;
    partyGetMoveDirSpd(party, &dir, &spd);
    hit = partySearchFrontWall(party, &pos, spd, dir);
    if (hit) {
        party->hitobjFront = hit;
        if (party->moveSpeed) {
            party->hitobjPush = hit;
        }
    }
    party->position.x = pos.x;
    party->position.z = pos.z;
}

void party_move_mode_wait(PartyEntry* party) {
    if (party->trigFlags & PARTY_TRIG_CHG_MOVE_MODE) {
        party->trigFlags &= ~PARTY_TRIG_CHG_MOVE_MODE;
        partyChgPoseId(party, 1);
        party->flags &= ~PARTY_FLAG_IS_BEING_USED;
        if (party->currentMemberId == PARTY_MEMBER_FLURRIE) {
            cloud_hover_init(party);
        }
    }
    switch (party->motionId) {
        case 0:
            party_motion_wait(party);
            if (party->currentMemberId == PARTY_MEMBER_FLURRIE) {
                cloud_hover_main(party);
            }
            break;
        case 1:
        case 2:
        case 6:
            partyChgMot(party, 0);
            break;
        case 3:
            party_motion_fall(party);
            break;
    }
    partyModeTail(party);
    vivianShadow(party);
}

void partyWalkInit(PartyEntry* party) {
    u8* p114;
    MarioWork* mario = party->playerPtr;

    party->flags |= PARTY_FLAG_IS_GROUNDED;
    if (!(party->flags & PARTY_FLAG_IS_PARTNER)) {
        partyClearFootmark();
    } else {
        partyClearFootmark2();
    }
    party->moveArg = 0;
    party->reserved_44 = 0x10;
    party->homingDist = 50.0f;
    p114 = (u8*)&party->velY;
    party->reserved_3E = 0;
    party->accelY = 1.8f;
    *(f32*)p114 = 0.0f;
    if (!(mario->flags & MARIO_FLAG_PARTY_WALK_DISABLED)) {
        partyChgPoseId(party, 1);
        partyChgMoveMode(party, 1);
        partyChgMot(party, 0);
    }
}

void partyMoveWalk(PartyEntry* party) {
    if (party->trigFlags & PARTY_TRIG_CHG_RUN_MODE) {
        party->trigFlags &= ~PARTY_TRIG_CHG_RUN_MODE;
        party->flags &= ~PARTY_FLAG_IS_BEING_USED;
        partyWalkInit(party);
    }
    if (party->forceMoveFrames == 0 && party->motionId != 2 && party->motionId != 3 && party->motionId != 6) {
        f32 f;
        f32 target;
        MarioWork* mario = party->playerPtr;
        f32 cur = party->moveSpeed;
        if (mario->flags & MARIO_FLAG_IS_PUSHING) {
            target = mario->baseSpeed;
        } else {
            target = party->targetSpeed * mario->playerGravity * partySpdRate(party);
        }
        if (target > 0.0f) {
            f = cur + 0.05f * ((0.5f + target) - cur);
            if (f >= target) {
                f = target;
            }
            party->moveSpeed = f;
        } else {
            f = cur + 0.1f * (-1.0f - cur);
            if (f <= 0.0f) {
                f = 0.0f;
            }
            party->moveSpeed = f;
        }
    }
    partyWalkMain(party);
}

void partyWalkMain(PartyEntry* party) {
    PartyEntry* other;
    u32 flags = 0;
    MarioWork* mario = party->playerPtr;

    if (party->currentSlotId == 0) {
        if (partyChkMarioJumpFlag()) {
            flags |= 1;
        }
        if (marioChkSlitThrouh()) {
            flags |= 4;
        }
        partyPutFootmark(0, mario->position, flags);
    } else {
        other = anotherPartyGetPtr(party->currentSlotId);
        if (other) {
            if ((other->currentMemberId == PARTY_MEMBER_BOBBERY && other->runMode == 3) || nokonokoGetStatus(other) == 3 ||
                PSVECDistance(&mario->position, &other->position) >= 150.0f) {
                flags = partyChkMarioJumpFlag();
                partyPutFootmark(1, marioGetPtr()->position, flags);
            } else {
                flags = FALSE;
                if (other->flags & PARTY_FLAG_IS_JUMPING) {
                    flags = TRUE;
                }
                partyPutFootmark(1, other->position, flags);
            }
        } else {
            flags = partyChkMarioJumpFlag();
            partyPutFootmark(1, marioGetPtr()->position, flags);
        }
    }
    walkMain(party);
}

void party_move_mode_stay(PartyEntry* party) {
    if (party->trigFlags & PARTY_TRIG_CHG_MOVE_MODE) {
        party->trigFlags &= ~PARTY_TRIG_CHG_MOVE_MODE;
        partyChgPoseId(party, 1);
        if (party->currentMemberId == PARTY_MEMBER_FLURRIE) {
            cloud_hover_init(party);
        }
    }
    switch (party->motionId) {
        case 0:
            party_motion_stay(party);
            if (party->currentMemberId == PARTY_MEMBER_FLURRIE) {
                cloud_hover_main(party);
            }
            break;
        case 1:
        case 2:
        case 6:
            partyChgMot(party, 0);
            break;
        case 3:
            party_motion_fall(party);
            break;
    }
    partyModeTail(party);
}

void party_move_mode_walk(PartyEntry* party) {
    if (party->trigFlags & PARTY_TRIG_CHG_MOVE_MODE) {
        party->trigFlags &= ~PARTY_TRIG_CHG_MOVE_MODE;
    }
    switch (party->motionId) {
        case 0:
            party_motion_stay(party);
            break;
        case 1:
            if (party->moveKind == 0) {
                party_motion_homing_walk(party);
            } else {
                party_motion_homing_fly(party);
            }
            break;
        case 2:
            party_motion_jump(party);
            break;
        case 3:
            party_motion_fall(party);
            break;
        case 6:
            party_motion_upstairs(party);
            break;
        case 7:
            party_motion_damage(party);
            break;
    }
    partyModeTail(party);
}

void party_move_mode_behind_mario(PartyEntry* party) {
    if (party->trigFlags & PARTY_TRIG_CHG_MOVE_MODE) {
        party->trigFlags &= ~PARTY_TRIG_CHG_MOVE_MODE;
    }
    switch (party->motionId) {
        case 0:
        case 1:
            if (party->moveKind == 0) {
                party_motion_behind_mario(party);
            } else {
                party_motion_behind_mario_fly(party);
            }
            break;
        case 2:
            party_motion_jump2(party);
            break;
        case 3:
            party_motion_fall(party);
            break;
        case 6:
            party_motion_upstairs(party);
            break;
        case 7:
            party_motion_damage(party);
            break;
    }
    party->moveDir = party->targetDir;
}

void party_move_mode_beside_mario(PartyEntry* party) {
    if (party->trigFlags & PARTY_TRIG_CHG_MOVE_MODE) {
        party->trigFlags &= ~PARTY_TRIG_CHG_MOVE_MODE;
    }
    switch (party->motionId) {
        case 0:
        case 1:
            party_motion_beside_mario(party);
            break;
        case 2:
            party_motion_jump2(party);
            break;
        case 3:
            party_motion_fall(party);
            break;
        case 6:
            party_motion_upstairs(party);
            break;
        case 7:
            party_motion_damage(party);
            break;
    }
    party->moveDir = party->targetDir;
}

void partyForceWalk(PartyEntry* party) {
    Vec pos;
    HitEntry* hit;
    s32 attr;

    if (party->trigFlags & PARTY_TRIG_CHG_RUN_MODE) {
        party->trigFlags &= ~PARTY_TRIG_CHG_RUN_MODE;
        if (party->flags & PARTY_FLAG_IS_BEING_USED) {
            if (party->currentMemberId == PARTY_MEMBER_BOBBERY) {
                bomhei_use_cancel();
            }
            if (party->currentMemberId == PARTY_MEMBER_KOOPS) {
                nokonoko_finish(party);
            }
            party->flags &= ~PARTY_FLAG_IS_BEING_USED;
        }
    }
    switch (party->motionId) {
        case 0:
        case 1:
            if (party->forceMoveFrames <= 0) {
                party->forceMoveFrames = 0;
                partyPrevRunMode(party);
                break;
            }
            partyChgWalkPose(party, party->moveSpeed);
            pos = party->position;
            if (partySearchFrontWall(party, &pos, party->moveSpeed, party->targetDir)) {
                partyChgMot(party, 2);
            } else {
                party->position.x = pos.x;
                party->position.z = pos.z;
            }
            if (--party->forceMoveFrames <= 0) {
                party->forceMoveFrames = 0;
                partyPrevRunMode(party);
                break;
            }
            party->flags &= ~PARTY_FLAG_IN_SHALLOW_WATER;
            hit = partySearchGround(party, 10.0f, 10.0f);
            attr = 0;
            if (hit && ((attr = hitGetAttr(hit)) & 0x200)) {
                hit = NULL;
            }
            party->hitobjStandOn = hit;
            if (!hit) {
                partyChgMot(party, 3);
                party->accelY = -1.0f;
            } else if (attr & 0x100) {
                party->flags |= PARTY_FLAG_IN_SHALLOW_WATER;
            } else {
                party->flags &= ~PARTY_FLAG_IN_SHALLOW_WATER;
            }
            party->position.y = party->groundY;
            break;
        case 2:
            if (--party->forceMoveFrames <= 0) {
                party->forceMoveFrames = 0;
            }
            party_motion_jump(party);
            break;
        case 3:
            if (--party->forceMoveFrames <= 0) {
                party->forceMoveFrames = 0;
            }
            party_motion_fall(party);
            break;
    }
    party->moveDir = party->targetDir;
    if (party->currentMemberId == PARTY_MEMBER_VIVIAN) {
        vivianShadow(party);
    }
}

void partyMoveFlyInit(PartyEntry* party, s32 arg) {
    MarioWork* mario = marioGetPtr();

    party->flags |= PARTY_FLAG_IS_GROUNDED;
    if (!(party->flags & PARTY_FLAG_IS_PARTNER)) {
        partyClearFootmark();
    } else {
        partyClearFootmark2();
    }
    if (party->moveMode == 2) {
        party->homingDist = 50.0f;
        return;
    }
    party->moveArg = arg == 0 ? 0 : 2;
    partyChgMoveMode(party, 0);
    if (party->flyRequest == 1 || party->flyRequest == 3) {
        party->flyRequest = 0;
        party->moveArg = 0;
        partyChgMoveMode(party, 1);
    } else if (party->flyRequest == 2) {
        party->moveArg = 0;
        partyChgMoveMode(party, 0x3C);
    }
    party->reserved_44 = 0x10;
    party->homingDist = 50.0f;
    party->reserved_3E = 0;
    partyChgPoseId(party, 1);
    partyChgMoveMode(party, 1);
    partyChgMot(party, 0);
}

void partyFlyMain(PartyEntry* party) {
    u32 flags;
    PartyEntry* p;

    if (party->currentSlotId == 0) {
        flags = partyChkMarioJumpFlag();
        partyPutFootmark(0, marioGetPtr()->position, flags);
    } else {
        anotherPartyGetPtr(party->currentSlotId);
        p = partyGetPtr(0);
        if (p) {
            flags = FALSE;
            if (p->flags & PARTY_FLAG_IS_JUMPING) {
                flags = TRUE;
            }
            partyPutFootmark(1, p->position, flags);
        } else {
            flags = partyChkMarioJumpFlag();
            partyPutFootmark(0, marioGetPtr()->position, flags);
        }
    }
    flyMain(party);
}

static inline void party_move_mode_behind_mario_continue(PartyEntry* party) {
    if (party->trigFlags & PARTY_TRIG_CHG_MOVE_MODE) {
        party->trigFlags &= ~PARTY_TRIG_CHG_MOVE_MODE;
    }
    switch (party->motionId) {
        case 0:
        case 1:
            if (party->moveKind == 0) {
                party_motion_behind_mario_continue(party);
            } else {
                party_motion_behind_mario_fly(party);
            }
            break;
        case 2:
            party_motion_jump2(party);
            break;
        case 3:
            party_motion_fall(party);
            break;
        case 6:
            party_motion_upstairs(party);
            break;
        case 7:
            party_motion_damage(party);
            break;
    }
    party->moveDir = party->targetDir;
}

void walkMain(PartyEntry* party) {
    switch (party->moveMode) {
        case 0:
            party_move_mode_stay(party);
            break;
        case 1:
            party_move_mode_walk(party);
            break;
        case 2:
            party_move_mode_behind_mario(party);
            break;
        case 3:
            party_move_mode_beside_mario(party);
            break;
        case 6:
            party_move_mode_behind_mario_continue(party);
            break;
    }
}

static void flyMain(PartyEntry* party) {
    switch (party->moveMode) {
        case 0:
            party_move_mode_stay(party);
            break;
        case 1:
            party_move_mode_walk(party);
            break;
        case 2:
            party_move_mode_behind_mario(party);
            break;
        case 3:
            party_move_mode_beside_mario(party);
            break;
        case 6:
            party_move_mode_behind_mario_continue(party);
            break;
    }
}

extern f32 partyNrmToAngle(Vec* n);
extern s32 marioPlaneGetFlyingDir(void);

static inline BOOL partyAppearChkFloor(HitEntry* hit) {
    if (!hit) {
        return FALSE;
    }
    if (hitGetAttr(hit) & (HIT_ATTR_WATER_HAZARD | HIT_ATTR_TOGE)) {
        return FALSE;
    }
    if (partyChkNoStandMobj(hit)) {
        return FALSE;
    }
    return TRUE;
}

static inline f32 partyAppearNrmAngle(Vec n) {
    return partyNrmToAngle(&n);
}

static inline void partyAppearMoveBack(Vec* out, f32 d, f32 ang) {
    movePos(&out->x, &out->z, -d, ang);
}


BOOL partyGetAppearPos(PartyEntry* party, Vec* out) {
    HitCheckQuery query;
    f32 s;
    f32 c;
    MarioWork* mario;
    HitEntry* hit;
    f32 dir;
    f32 dist;
    f32 back;
    f32 ofs = 30.0f;

    mario = party->playerPtr;
    dist = 30.0f + party->width;
    if (mario->currMotionId == MARIO_MOTION_PLANE) {
        if (marioPlaneGetFlyingDir() == 1) {
            back = 0.0f;
            dir = 180.0f;
        } else {
            back = 180.0f;
            dir = 0.0f;
        }
    } else {
        back = revise360(180.0f + mario->displayDirection);
        dir = mario->displayDirection;
    }
    out->x = mario->position.x;
    out->y = mario->position.y;
    out->z = mario->position.z;
    if (mario->currPartySlotId[0] >= 0) {
        if (mario->currPartySlotId[1] < 0) {
            sincosf(&s, &c, toMovedir(back));
            query.targetDistance = dist;
            query.targetPos = (Vec){out->x, 37.0f + out->y, out->z};
            query.targetDir = (Vec){s, 0.0f, c};
            hit = hitCheckVecFilter(&query, NULL);
            if (mario->currMotionId == MARIO_MOTION_PLANE) {
                return TRUE;
            }
            if (!hit) {
                partyAppearMoveBack(out, ofs, toMovedir(dir));
                query.targetDistance = 150.0f;
                query.targetPos = (Vec){out->x, 37.0f + out->y, out->z};
                query.targetDir = (Vec){0.0f, -1.0f, 0.0f};
                hit = hitCheckVecFilter(&query, NULL);
                if (partyAppearChkFloor(hit) && query.targetDistance <= 100.0f) {
                    out->y = query.hitPos.y;
                    return TRUE;
                }
            }
        } else {
            sincosf(&s, &c, toMovedir(back));
            query.targetDistance = dist;
            query.targetPos = (Vec){out->x, 37.0f + out->y, out->z};
            query.targetDir = (Vec){s, 0.0f, c};
            hit = hitCheckVecFilter(&query, NULL);
            if (mario->currMotionId == MARIO_MOTION_PLANE) {
                return TRUE;
            }
            if (!hit) {
                partyAppearMoveBack(out, ofs, toMovedir2(dir, party->currentSlotId == 0 ? -30.0f : 30.0f));
                query.targetDistance = 150.0f;
                query.targetPos = (Vec){out->x, 37.0f + out->y, out->z};
                query.targetDir = (Vec){0.0f, -1.0f, 0.0f};
                hit = hitCheckVecFilter(&query, NULL);
                if (partyAppearChkFloor(hit) && query.targetDistance <= 100.0f) {
                    out->y = query.hitPos.y;
                    return TRUE;
                }
            }
        }
    }
    if (mario->currPartySlotId[0] >= 0) {
        if (mario->currPartySlotId[1] < 0) {
            sincosf(&s, &c, toMovedir2(dir, -90.0f));
            query.targetDistance = dist;
            query.targetPos = (Vec){mario->position.x, 37.0f + mario->position.y, mario->position.z};
            query.targetDir = (Vec){s, 0.0f, c};
            if (!hitCheckVecFilter(&query, NULL)) {
                out->x = mario->position.x;
                out->y = mario->position.y;
                out->z = mario->position.z;
                partyAppearMoveBack(out, ofs, toMovedir2(dir, 90.0f));
                query.targetDistance = 150.0f;
                query.targetPos = (Vec){out->x, 37.0f + out->y, out->z};
                query.targetDir = (Vec){0.0f, -1.0f, 0.0f};
                hit = hitCheckVecFilter(&query, NULL);
                if (partyAppearChkFloor(hit) && query.targetDistance <= 100.0f) {
                    out->y = query.hitPos.y;
                    return TRUE;
                }
            }
            sincosf(&s, &c, toMovedir2(dir, 90.0f));
            query.targetDistance = dist;
            query.targetPos = (Vec){mario->position.x, 37.0f + mario->position.y, mario->position.z};
            query.targetDir = (Vec){s, 0.0f, c};
            if (!hitCheckVecFilter(&query, NULL)) {
                out->x = mario->position.x;
                out->y = mario->position.y;
                out->z = mario->position.z;
                partyAppearMoveBack(out, ofs, toMovedir2(dir, -90.0f));
                query.targetDistance = 150.0f;
                query.targetPos = (Vec){out->x, 37.0f + out->y, out->z};
                query.targetDir = (Vec){0.0f, -1.0f, 0.0f};
                hit = hitCheckVecFilter(&query, NULL);
                if (partyAppearChkFloor(hit) && query.targetDistance <= 100.0f) {
                    out->y = query.hitPos.y;
                    return TRUE;
                }
            }
            sincosf(&s, &c, toMovedir2(dir, 0.0f));
            query.targetDistance = dist;
            query.targetPos = (Vec){mario->position.x, 37.0f + mario->position.y, mario->position.z};
            query.targetDir = (Vec){s, 0.0f, c};
            if (!hitCheckVecFilter(&query, NULL)) {
                out->x = mario->position.x;
                out->y = mario->position.y;
                out->z = mario->position.z;
                movePos(&out->x, &out->z, ofs, toMovedir2(dir, 0.0f));
                query.targetDistance = 150.0f;
                query.targetPos = (Vec){out->x, 37.0f + out->y, out->z};
                query.targetDir = (Vec){0.0f, -1.0f, 0.0f};
                hit = hitCheckVecFilter(&query, NULL);
                if (partyAppearChkFloor(hit) && query.targetDistance <= 100.0f) {
                    out->y = query.hitPos.y;
                    return TRUE;
                }
            }
        } else {
            out->x = mario->position.x;
            out->y = mario->position.y;
            out->z = mario->position.z;
            sincosf(&s, &c, toMovedir2(dir, -90.0f));
            query.targetDistance = dist;
            query.targetPos = (Vec){mario->position.x, 37.0f + mario->position.y, mario->position.z};
            query.targetDir = (Vec){s, 0.0f, c};
            if (!hitCheckVecFilter(&query, NULL)) {
                out->x = mario->position.x;
                out->y = mario->position.y;
                out->z = mario->position.z;
                partyAppearMoveBack(out, ofs, toMovedir2(dir, party->currentSlotId == 0 ? 70.0f : 110.0f));
                query.targetDistance = 150.0f;
                query.targetPos = (Vec){out->x, 37.0f + out->y, out->z};
                query.targetDir = (Vec){0.0f, -1.0f, 0.0f};
                hit = hitCheckVecFilter(&query, NULL);
                if (partyAppearChkFloor(hit) && query.targetDistance <= 100.0f) {
                    out->y = query.hitPos.y;
                    return TRUE;
                }
            }
            sincosf(&s, &c, toMovedir2(dir, 90.0f));
            query.targetDistance = dist;
            query.targetPos = (Vec){mario->position.x, 37.0f + mario->position.y, mario->position.z};
            query.targetDir = (Vec){s, 0.0f, c};
            if (!hitCheckVecFilter(&query, NULL)) {
                out->x = mario->position.x;
                out->y = mario->position.y;
                out->z = mario->position.z;
                partyAppearMoveBack(out, ofs, toMovedir2(dir, party->currentSlotId == 0 ? -110.0f : -70.0f));
                query.targetDistance = 150.0f;
                query.targetPos = (Vec){out->x, 37.0f + out->y, out->z};
                query.targetDir = (Vec){0.0f, -1.0f, 0.0f};
                hit = hitCheckVecFilter(&query, NULL);
                if (partyAppearChkFloor(hit) && query.targetDistance <= 100.0f) {
                    out->y = query.hitPos.y;
                    return TRUE;
                }
            }
            sincosf(&s, &c, toMovedir2(dir, 0.0f));
            query.targetDistance = dist;
            query.targetPos = (Vec){mario->position.x, 37.0f + mario->position.y, mario->position.z};
            query.targetDir = (Vec){s, 0.0f, c};
            if (!hitCheckVecFilter(&query, NULL)) {
                out->x = mario->position.x;
                out->y = mario->position.y;
                out->z = mario->position.z;
                partyAppearMoveBack(out, ofs, toMovedir2(dir, party->currentSlotId == 0 ? 70.0f : -70.0f));
                query.targetDistance = 150.0f;
                query.targetPos = (Vec){out->x, 37.0f + out->y, out->z};
                query.targetDir = (Vec){0.0f, -1.0f, 0.0f};
                hit = hitCheckVecFilter(&query, NULL);
                if (partyAppearChkFloor(hit) && query.targetDistance <= 100.0f) {
                    out->y = query.hitPos.y;
                    return TRUE;
                }
            }
        }
    }
    return FALSE;
}

BOOL partyGetAppearPos2(PartyEntry* party, Vec* out) {
    HitCheckQuery query;
    f32 s;
    f32 c;
    MarioWork* mario;
    f32 dir;
    f32 dist;
    f32 back;
    f32 ofs = 30.0f;
    f32 a90 = 90.0f;

    mario = party->playerPtr;
    dist = 30.0f + party->width;
    back = revise360(180.0f + mario->displayDirection);
    dir = mario->displayDirection;
    out->x = mario->position.x;
    out->y = mario->position.y;
    out->z = mario->position.z;
    if (mario->currPartySlotId[1] < 0) {
        sincosf(&s, &c, toMovedir(180.0f + mario->displayDirection));
        query.targetDistance = dist;
        query.targetPos = (Vec){out->x, 37.0f + out->y, out->z};
        query.targetDir = (Vec){s, 0.0f, c};
        if (!hitCheckVecFilter(&query, NULL)) {
            partyAppearMoveBack(out, ofs, toMovedir(mario->displayDirection));
            query.targetDistance = 38.0f;
            query.targetPos = (Vec){out->x, 37.0f + out->y, out->z};
            query.targetDir = (Vec){0.0f, -1.0f, 0.0f};
            if (hitCheckVecFilter(&query, NULL)) {
                out->y = query.hitPos.y;
                return TRUE;
            }
        }
    } else {
        out->x = mario->position.x;
        out->y = mario->position.y;
        out->z = mario->position.z;
        sincosf(&s, &c, toMovedir2(dir, -90.0f));
        query.targetDistance = dist;
        query.targetPos = (Vec){mario->position.x, 37.0f + mario->position.y, mario->position.z};
        query.targetDir = (Vec){s, 0.0f, c};
        if (!hitCheckVecFilter(&query, NULL)) {
            out->x = mario->position.x;
            out->y = mario->position.y;
            out->z = mario->position.z;
            partyAppearMoveBack(out, ofs, toMovedir2(dir, party->currentSlotId == 0 ? 20.0f : -20.0f));
            query.targetDistance = 38.0f;
            query.targetPos = (Vec){out->x, 37.0f + out->y, out->z};
            query.targetDir = (Vec){0.0f, -1.0f, 0.0f};
            if (hitCheckVecFilter(&query, NULL)) {
                out->y = query.hitPos.y;
                return TRUE;
            }
        }
        sincosf(&s, &c, toMovedir2(dir, 90.0f));
        query.targetDistance = dist;
        query.targetPos = (Vec){mario->position.x, 37.0f + mario->position.y, mario->position.z};
        query.targetDir = (Vec){s, 0.0f, c};
        if (!hitCheckVecFilter(&query, NULL)) {
            out->x = mario->position.x;
            out->y = mario->position.y;
            out->z = mario->position.z;
            partyAppearMoveBack(out, ofs, toMovedir2(dir, party->currentSlotId == 0 ? -20.0f : 20.0f));
            query.targetDistance = 38.0f;
            query.targetPos = (Vec){out->x, 37.0f + out->y, out->z};
            query.targetDir = (Vec){0.0f, -1.0f, 0.0f};
            if (hitCheckVecFilter(&query, NULL)) {
                out->y = query.hitPos.y;
                return TRUE;
            }
        }
    }
    return FALSE;
}

BOOL partyGetAppearPos3(PartyEntry* party, Vec* out) {
    HitCheckQuery query;
    s32 angles[9] = {45, 135, -135, 315, -315, 180, -180, 90, -90};
    f32 s;
    f32 c;
    HitEntry* hit;
    u32 i;
    MarioWork* mario;
    NpcEntry* npc;
    f32 bx;
    f32 by;
    f32 bz;
    f32 dir;
    f32 dist;
    f32 best;
    f32 ang;

    mario = party->playerPtr;
    dist = 30.0f + party->width;
    dir = mario->displayDirectionTarget;
    best = -1.0f;
    out->x = mario->position.x;
    out->y = mario->position.y;
    out->z = mario->position.z;
    for (i = 0; i < 9; i++) {
        ang = toMovedir2(dir, angles[i]);
        sincosf(&s, &c, ang);
        query.targetDistance = dist;
        query.targetPos = (Vec){mario->position.x, 37.0f + mario->position.y, mario->position.z};
        query.targetDir = (Vec){s, 0.0f, c};
        if (hitCheckVecFilter(&query, NULL)) {
            continue;
        }
        out->x = mario->position.x;
        out->y = mario->position.y;
        out->z = mario->position.z;
        movePos(&out->x, &out->z, 30.0f, ang);
        query.targetDistance = 87.0f;
        query.targetPos = (Vec){out->x, 37.0f + out->y, out->z};
        query.targetDir = (Vec){0.0f, -1.0f, 0.0f};
        hit = hitCheckVecFilter(&query, NULL);
        if (!hit) {
            continue;
        }
        if (partyChkNoStandMobj(hit)) {
            continue;
        }
        if (partyAppearNrmAngle(query.hitNormal) > 60.0f) {
            continue;
        }
        if (!partyAppearChkFloor(hit)) {
            continue;
        }
        npc = npcNearDistCheck(out->x, out->y, out->z, party->width);
        if (npc) {
            if (best < PSVECDistance(&npc->position, out)) {
                best = PSVECDistance(&npc->position, out);
                bx = out->x;
                by = out->y;
                bz = out->z;
            }
        } else {
            out->y = query.hitPos.y;
#ifdef VERSION_G8ME01
            partyChkWallAround(party, out, 2.0f, 0.0f, party->width, party->height);
#else
            marioChkWallAround(out, 1, 2.0f, 0.0f, party->width, party->height);
#endif
            return TRUE;
        }
    }
    if (best >= 0.0f) {
        out->x = bx;
        out->y = by;
        out->z = bz;
        return TRUE;
    }
    return FALSE;
}

static inline void partyAppearAdjustY(PartyEntry* party, MarioWork* mario, Vec* out) {
    if (party->moveKind == 1 && out->y < mario->position.y) {
        out->y = 5.0f + mario->position.y;
    }
}

BOOL partyGetAppearPos4(PartyEntry* party, Vec* out) {
    PartyEntry* other = NULL;
    MarioWork* mario = party->playerPtr;
    HitCheckQuery query;
    s32 angA[11] = {180, 150, -150, 120, -120, 90, -90, 60, -60, 30, -30};
    s32 angB[11] = {150, -150, 120, -120, 90, -90, 60, -60, 30, -30, 180};
    s32 angC[11] = {-150, 150, -120, 120, -90, 90, -60, 60, -30, 30, -180};
    s32 angD[13] = {180, 180, 150, -150, 120, -120, 90, -90, 60, -60, 30, -30, 0};
    s32 angE[13] = {45, 60, 15, 75, 0, 90, -15, 105, -30, 120, -45, 135, -60};
    f32 s;
    f32 c;
    s32 n;
    HitEntry* hit;
    s32 i;
    f32 back;
    f32 dist;
    f32 radius;
    f32 ang;

    if (party->currentMemberId == PARTY_MEMBER_FLURRIE) {
        radius = 32.0f;
    } else if (party->trigFlags & PARTY_TRIG_DOOR_MOVE) {
        radius = 28.0f;
    } else {
        radius = 26.0f;
    }
    dist = radius + party->width;
    back = revise360(180.0f + party->homeDir);
    if (!(party->flags & PARTY_FLAG_IS_PARTNER)) {
        out->x = mario->position.x;
        out->y = mario->position.y;
        out->z = mario->position.z;
    } else {
        other = anotherPartyGetPtr(party->currentSlotId);
        if (other) {
            out->x = other->position.x;
            out->z = other->position.z;
            out->y = other->position.y;
        } else {
            out->x = mario->position.x;
            out->z = mario->position.z;
            out->y = mario->position.y;
        }
    }
    n = (party->trigFlags & PARTY_TRIG_DOOR_MOVE) ? 13 : 11;

    for (i = 0; i <= n; i++) {
        if (i == 0) {
            query.targetDistance = dist;
            ang = party->homeDir;
        } else {
            query.targetDistance = dist;
            if (party->trigFlags & PARTY_TRIG_DOOR_MOVE) {
            if (strcmp(gp->currentMapName, "usu_00") == 0) {
                ang = toMovedir2(party->homeDir, angE[i]);
            } else {
                ang = toMovedir2(party->homeDir, angD[i]);
            }
        } else if (!other) {
            ang = toMovedir2(back, angA[i]);
        } else if (!(party->flags & PARTY_FLAG_IS_PARTNER)) {
            ang = toMovedir2(back, angB[i]);
        } else {
            ang = toMovedir2(back, angC[i]);
        }
        }
        sincosf(&s, &c, ang);
        query.targetPos = (Vec){out->x, 37.0f + out->y, out->z};
        query.targetDir = (Vec){s, 0.0f, c};
        if (hitCheckVecFilter(&query, NULL)) {
            continue;
        }
        movePos(&out->x, &out->z, radius, ang);
        query.targetDistance = 87.0f;
        query.targetPos = (Vec){out->x, 37.0f + out->y, out->z};
        query.targetDir = (Vec){0.0f, -1.0f, 0.0f};
        hit = hitCheckVecFilter(&query, NULL);
        if (!hit) {
            continue;
        }
        if (!partyAppearChkFloor(hit)) {
            continue;
        }
        if (npcNearDistCheck(out->x, out->y, out->z, party->width)) {
            continue;
        }
        out->y = query.hitPos.y;
#ifdef VERSION_G8ME01
        partyChkWallAround(party, out, 0.0f, 0.0f, 28.0f, 5.0f);
#else
        marioChkWallAround(out, 1, 0.0f, 0.0f, 28.0f, 5.0f);
#endif
        partyAppearAdjustY(party, mario, out);
        return TRUE;
    }
    partyGetAppearPos(party, out);
    partyAppearAdjustY(party, mario, out);
    return FALSE;
}

const Vec vec3_802ce0ac = {0.0f, 0.0f, 0.0f};
const Vec vec3_802ce0b8 = {0.0f, 0.0f, 0.0f};

// Stand-in for a function the original linker dead-stripped; it created 90.0f and 20.0f in the .sdata2 pool here.
static void partyMoveStrippedPool(f32* f) {
    f[0] = 90.0f;
    f[1] = 20.0f;
}

BOOL partyGetAppearPos5(PartyEntry* party, HitEntry* hit, Vec* out) {
    s32 angles[12] = {0, 30, -30, 60, -60, 90, -90, 120, -120, 150, -150, 180};
    Vec pos;
    Vec dir;
    Vec hitPos;
    Vec hitNrm;
    f32 dist;
    f32 s;
    f32 c;
    Vec* mobj;
    u32 i;

    if (!hit) {
        return FALSE;
    }
    if (!(hitGetAttr(hit) & HIT_ATTR_MOBJ)) {
        return FALSE;
    }
    mobj = &((MapObjectEntry*)mobjHitObjPtrToPtr(hit))->position;
    pos.x = mobj->x;
    pos.z = mobj->z;
    pos.y = 50.0f + mobj->y;
    for (i = 0; i < 12; i++) {
        f32 ang = party->targetDir + angles[i];
        dist = 50.0f;
        sincosf(&s, &c, ang);
        dir = (Vec){s, 0.0f, c};
        if (!partyHitCheck(party, &pos, &dir, &hitPos, &hitNrm, &dist)) {
            HitEntry* floor;
            f32 r;
            dist = 150.0f;
            r = 50.0f;
            pos.x = s * r + mobj->x;
            pos.z = c * r + mobj->z;
            dir = (Vec){0.0f, -1.0f, 0.0f};
            floor = partyHitCheck(party, &pos, &dir, &hitPos, &hitNrm, &dist);
            if (!floor || !(hitGetAttr(floor) & HIT_ATTR_WATER_HAZARD)) {
                break;
            }
        }
    }
    if (i >= 12) {
        return FALSE;
    }
    out->x = pos.x;
    out->y = pos.y;
    out->z = pos.z;
    return TRUE;
}
