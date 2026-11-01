// Partner movement: the footmark trail behind Mario, target speed and direction, walking and the move modes.

#include "version.h"
#include "party.h"
#include "mot/mot_plane.h"
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
static void flyMain(PartyEntry* party);

Footmark footmarkBuf[2][80];
s32 writeId[2];
s32 readId[2];

/**
 * Map objects a partner must not stand on when following Mario:
 * chests, jump stands, Save and Heart blocks, switches and the eki_06 objects.
 */
#if VERSION > VERSION_DEMO_US
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
#endif

// Movement helper; definitions below retain their original order.
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
    if (party->runMode != PARTY_RUN_DAMAGE_RETURN) {
        if (party->hitobjStandOn && (hitGetAttr(party->hitobjStandOn) & HIT_ATTR_TOGE)) {
            partyChgRunMode(party, PARTY_RUN_DAMAGE_RETURN);
        } else if (party->hitobjFront && (hitGetAttr(party->hitobjFront) & HIT_ATTR_TOGE)) {
            partyChgRunMode(party, PARTY_RUN_DAMAGE_RETURN);
        } else if (party->hitobjHead && (hitGetAttr(party->hitobjHead) & HIT_ATTR_TOGE)) {
            partyChgRunMode(party, PARTY_RUN_DAMAGE_RETURN);
        }
    }
}

/**
 * The footmark at the read position of trail id (0 = the trail the partner follows, 1 = the follower's trail);
 * party_motion.c steers toward it.
 */
Footmark* partyGetFootmarkPtr(s32 id) {
    return &footmarkBuf[id][readId[id]];
}

/** Read position (0-79) of footmark trail id. */
s32 partyGetFootmarkReadId(s32 id) {
    return readId[id];
}

/** Write position (0-79) of footmark trail id, the newest mark; read == write means the trail is used up. */
s32 partyGetFootmarkWriteId(s32 id) {
    return writeId[id];
}

/** Which footmark trail this party entry follows: 1 for a PARTY_FLAG_IS_FOLLOWER entry, else 0. */
s32 partyGetFootmarkId(PartyEntry* party) {
    return (party->flags >> 2) & 1;
}

/**
 * Where trail id is chasing: the other party member (anotherPartyGetPtr) when it exists,
 * Koops's nokonokoGetStatus is not 3 and it is within 150 units of Mario, else Mario.
 */
#if VERSION <= VERSION_DEMO_US
static inline void partyTargetPos(PartyEntry* other, BOOL far, Vec* pos) {
    if (other && !far) {
#else
static void getTargetPos(s32 id, Vec* pos) {
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
#endif
        pos->x = other->position.x;
        pos->y = other->position.y;
        pos->z = other->position.z;
    } else {
#if VERSION <= VERSION_DEMO_US
        pos->x = marioGetPtr()->position.x;
        pos->y = marioGetPtr()->position.y;
        pos->z = marioGetPtr()->position.z;
#else
        pos->x = mario->position.x;
        pos->y = mario->position.y;
        pos->z = mario->position.z;
#endif
    }
}

/**
 * Where trail id is chasing: the other party member (anotherPartyGetPtr) when it exists,
 * Koops's nokonokoGetStatus is not 3 and it is within 150 units of Mario, else Mario.
 */
#if VERSION > VERSION_DEMO_US
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

/**
 * Demo: direction in degrees from (x, z) to what trail id chases: Mario for trail 0;
 * for trail 1 the partner in slot 0, or Mario while Koops's nokonokoGetStatus is 3.
 */
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

/**
 * Demo: horizontal distance from (x, z) to what trail id chases (Mario, or the slot 0 partner for trail 1;
 * Mario while Koops's nokonokoGetStatus is 3).
 */
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
#else
/**
 * Demo: direction in degrees from (x, z) to what trail id chases: Mario for trail 0;
 * for trail 1 the partner in slot 0, or Mario while Koops's nokonokoGetStatus is 3.
 */
f32 partyGetTargetDir(s32 id, f32 x, f32 z) {
    MarioWork* mario;
    PartyEntry* other;
    BOOL far;
    Vec pos;

    if (id == 0) {
        mario = marioGetPtr();
        return angleABf(x, z, mario->position.x, mario->position.z);
    }
    far = FALSE;
    other = anotherPartyGetPtr(id != 0);
    if (other) {
        if (nokonokoGetStatus(other) == 3) {
#endif
            far = TRUE;
        }
    }
#if VERSION <= VERSION_DEMO_US
    partyTargetPos(other, far, &pos);
    return angleABf(x, z, pos.x, pos.z);
}

/**
* Demo: horizontal distance from (x, z) to what trail id chases (Mario, or the slot 0 partner for trail 1;
* Mario while Koops's nokonokoGetStatus is 3).
*/
f32 partyGetTargetDist(s32 id, f32 x, f32 z) {
    MarioWork* mario;
    PartyEntry* other;
    BOOL far;
    Vec pos;

    if (id == 0) {
        mario = marioGetPtr();
        return distABf(x, z, mario->position.x, mario->position.z);
    }
    far = FALSE;
    other = anotherPartyGetPtr(id != 0);
    if (other) {
        if (nokonokoGetStatus(other) == 3) {
#else
    if (other && !far) {
        getTargetPos(id, &pos);
        return distABf(x, z, pos.x, pos.z);
    }
    return distABf(x, z, mario->position.x, mario->position.z);
}

/** Demo: 3D distance from p to what trail id chases, picked as in partyGetTargetDir. */
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
#endif
            far = TRUE;
        }
    }
#if VERSION <= VERSION_DEMO_US
    partyTargetPos(other, far, &pos);
    return distABf(x, z, pos.x, pos.z);
}

/** Demo: 3D distance from p to what trail id chases, picked as in partyGetTargetDir. */
f32 partyGetTargetDist3D(s32 id, Vec* p) {
    PartyEntry* other;
    BOOL far;
    Vec pos;

    if (id == 0) {
        return PSVECDistance(&marioGetPtr()->position, p);
    }
    far = FALSE;
    other = anotherPartyGetPtr(id != 0);
    if (other) {
        if (nokonokoGetStatus(other) == 3) {
#else
    if (other && !far) {
        getTargetPos(id, &pos);
        return PSVECDistance(&pos, p);
    }
    return PSVECDistance(&marioGetPtr()->position, p);
}

/** Demo: vertical distance from height y to what trail id chases, picked as in partyGetTargetDir. */
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
#endif
            far = TRUE;
        }
    }
#if VERSION <= VERSION_DEMO_US
    partyTargetPos(other, far, &pos);
    return PSVECDistance(&pos, p);
}
#else
    if (other && !far) {
        getTargetPos(id, &pos);
        return __fabsf(pos.y - y);
    }
    return __fabsf(mario->position.y - y);
}
#endif

/**
 * The speed trail id should move at: Mario's walk speed (baseSpeed, else baseDashSpeed) for trail 0 or
 * when the other party member is away, else that member's moveSpeed.
 */
#if VERSION <= VERSION_DEMO_US
/** Demo: vertical distance from height y to what trail id chases, picked as in partyGetTargetDir. */
f32 partyGetTargetDistY(s32 id, f32 y) {
#else
f32 partyGetTargetMovespd(s32 id) {
    MarioWork* mario = marioGetPtr();
    BOOL far;
#endif
    PartyEntry* other;
#if VERSION <= VERSION_DEMO_US
    BOOL far;
    Vec pos;
#endif

    if (id == 0) {
#if VERSION <= VERSION_DEMO_US
        return __fabsf(marioGetPtr()->position.y - y);
#else
        if (mario->baseSpeed > 0.0f) {
            return mario->baseSpeed;
        }
        return mario->baseDashSpeed;
#endif
    }
    far = FALSE;
    other = anotherPartyGetPtr(id != 0);
    if (other) {
#if VERSION <= VERSION_DEMO_US
        if (nokonokoGetStatus(other) == 3) {
#else
        if (nokonokoGetStatus(other) == 3 ||
            PSVECDistance(&mario->position, &other->position) >= 150.0f) {
#endif
            far = TRUE;
        }
    }
#if VERSION <= VERSION_DEMO_US
    partyTargetPos(other, far, &pos);
    return __fabsf(pos.y - y);
}
#else
    if (other && !far) {
        return other->moveSpeed;
    }
    mario = marioGetPtr();
    if (mario->baseSpeed > 0.0f) {
        return mario->baseSpeed;
    }
    return mario->baseDashSpeed;
}
#endif

/**
 * Demo: the speed the entry walks at to keep up.
 * Without PARTY_FLAG_IS_PARTNER, or while following close (doors),
 * it is Mario's walk speed (baseSpeed, else baseDashSpeed);
 * otherwise the active partner's moveSpeed unless Koops's nokonokoGetStatus is 3.
 */
f32 partyGetTargetMovespd2(PartyEntry* party) {
    MarioWork* mario = party->playerPtr;
    BOOL far;
    PartyEntry* other;

    if (!(party->flags & PARTY_FLAG_IS_PARTNER)) {
        if (mario->baseSpeed > 0.0f) {
            return mario->baseSpeed;
        }
        return mario->baseDashSpeed;
    }
    if (party->trigFlags & PARTY_TRIG_FOLLOW_CLOSE) {
        if (mario->baseSpeed > 0.0f) {
            return mario->baseSpeed;
        }
        return mario->baseDashSpeed;
    }
    far = FALSE;
    other = partyGetPtr(marioGetPartyId());
    if (other) {
#if VERSION <= VERSION_DEMO_US
        if (nokonokoGetStatus(other) == 3) {
#else
        if (nokonokoGetStatus(other) == 3 ||
            PSVECDistance(&mario->position, &other->position) >= 150.0f) {
#endif
            far = TRUE;
        }
    }
    if (other && !far) {
        return other->moveSpeed;
    }
    if (mario->baseSpeed > 0.0f) {
        return mario->baseSpeed;
    }
    return mario->baseDashSpeed;
}

/**
 * Demo: moves trail id's read position on by one,
 * wrapping at 80 (unlike the later versions it can pass the write position).
 */
void partyNextReadId(s32 id) {
#if VERSION > VERSION_DEMO_US
    if (readId[id] != writeId[id]) {
        readId[id]++;
        if (readId[id] >= 80) {
            readId[id] = 0;
        }
    }
}
#else
    readId[id]++;
    if (readId[id] >= 80) {
        readId[id] = 0;
    }
}
#endif

/** Copies the position of the footmark at trail id's read position into x, y, z. */
void partyGetReadId(s32 id, f32* x, f32* y, f32* z) {
    Footmark* fm = &footmarkBuf[id][readId[id]];
    *x = fm->pos.x;
    *y = fm->pos.y;
    *z = fm->pos.z;
}

/** Copies the position of the footmark at trail id's read position into pos. */
void partyGetReadId2(s32 id, Vec* pos) {
    Footmark* fm = &footmarkBuf[id][readId[id]];
    pos->x = fm->pos.x;
    pos->y = fm->pos.y;
    pos->z = fm->pos.z;
}

/** Restarts footmark trail 0 (the one the partner follows) at Mario's position. */
void partyClearFootmark(void) {
#if VERSION <= VERSION_DEMO_US
    Vec mpos = marioGetPtr()->position;
    PartyEntry* other = partyGetPtr(marioGetPartyId());
    Vec ppos = *(other ? &other->position : &mpos);
    Footmark* a = footmarkBuf[0];
    Footmark* b = footmarkBuf[1];
#else
    Vec pos = marioGetPtr()->position;
#endif
    u32 i;

    for (i = 0; i < 80; i++) {
#if VERSION <= VERSION_DEMO_US
        a->pos = mpos;
        b->pos = ppos;
        a->flag = 0;
        b->flag = 0;
        a++;
        b++;
#else
        footmarkBuf[0][i].pos = pos;
        footmarkBuf[0][i].flag = 0;
#endif
    }
    writeId[0] = 0;
    readId[0] = 0;
#if VERSION <= VERSION_DEMO_US
    writeId[1] = 0;
    readId[1] = 0;
#endif
}

/** Demo: restarts both footmark trails at Mario's position (the active partner is looked up but not used). */
void partyClearFootmark2(void) {
#if VERSION <= VERSION_DEMO_US
    Vec pos = marioGetPtr()->position;
    PartyEntry* other = partyGetPtr(marioGetPartyId());
    Footmark* a = footmarkBuf[0];
    Footmark* b = footmarkBuf[1];
    u32 i;
    Footmark* fm;

    for (i = 0; i < 80; i++) {
        a->pos = pos;
        b->pos = pos;
        a->flag = 0;
        b->flag = 0;
        a++;
        b++;
    }
    writeId[0] = 0;
    readId[0] = 0;
#else
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
#endif
    writeId[1] = 0;
    readId[1] = 0;
}

/** TRUE while Mario is in a jump (MARIO_FLAG_IS_JUMPING); footmarks laid then get the jump bit. */
static inline BOOL partyChkMarioJumpFlag(void) {
    MarioWork* mario = marioGetPtr();
    BOOL ret = FALSE;
    if (mario->flags & MARIO_FLAG_IS_JUMPING) {
        ret = TRUE;
    }
    return ret;
}

/**
 * Demo: appends pos to trail id.
 * Skipped when the last mark and this one are both jump marks, when pos equals the last mark,
 * or when the trail is full (write would reach read).
 * Argument bit 1 sets the jump bit (1), bit 2 the paper-slit bit (4).
 */
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
#if VERSION <= VERSION_DEMO_US
    if (flags & PARTY_FOOTMARK_DEMO_ARG_SLIT) {
#else
    if (flags & FOOTMARK_FLAG_SLIT) {
#endif
        fm->flag |= FOOTMARK_FLAG_SLIT;
    } else {
        fm->flag &= ~FOOTMARK_FLAG_SLIT;
    }
}

/** TRUE while the other party member is jumping (PARTY_FLAG_IS_JUMPING); its footmarks then get the jump bit. */
static inline BOOL partyChkOtherFlag(PartyEntry* other) {
    BOOL ret = FALSE;
    if (other->flags & PARTY_FLAG_IS_JUMPING) {
        ret = TRUE;
    }
    return ret;
}

/**
 * Demo: lays this frame's footmark for Koops's walk (party_nokonoko.c).
 * The slot 0 entry tracks Mario; the other tracks the active partner,
 * or Mario while Koops's nokonokoGetStatus is 3; switching between the two restarts the trail (footmarkMode).
 */
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
#if VERSION <= VERSION_DEMO_US
            flags |= PARTY_FOOTMARK_DEMO_ARG_SLIT;
#else
            flags |= FOOTMARK_FLAG_SLIT;
#endif
        }
#if VERSION <= VERSION_DEMO_US
        partyPutFootmark(partyGetFootmarkId(party), marioGetPtr()->position, flags);
#else
        partyPutFootmark(partyGetFootmarkId(party), mario->position, flags);
#endif
    } else {
        far = FALSE;
        other = partyGetPtr(marioGetPartyId());
#if VERSION <= VERSION_DEMO_US
        if (other) {
            if (nokonokoGetStatus(other) == 3) {
                far = TRUE;
            }
#else
        if (other && (nokonokoGetStatus(other) == 3 ||
                      PSVECDistance(&mario->position, &other->position) >= 150.0f)) {
            far = TRUE;
#endif
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
                // FAKE: call through a no-op function-pointer cast; the plain call changes the object
                ((void (*)(void))partyClearFootmark2)();
            }
            if (partyChkMarioJumpFlag()) {
                flags |= FOOTMARK_FLAG_JUMP;
            }
            if (marioChkSlitThrouh()) {
#if VERSION <= VERSION_DEMO_US
                flags |= PARTY_FOOTMARK_DEMO_ARG_SLIT;
#else
                flags |= FOOTMARK_FLAG_SLIT;
#endif
            }
#if VERSION <= VERSION_DEMO_US
            partyPutFootmark(partyGetFootmarkId(party), marioGetPtr()->position, flags);
#else
            partyPutFootmark(partyGetFootmarkId(party), mario->position, flags);
#endif
        }
    }
}

/**
 * Ends any jump, fall or step: clears those flags and zeroes vertical speed, acceleration and gravity.
 * Used by party.c and Koops.
 */
void partyClearJumpPara(PartyEntry* party) {
    party->flags &= ~(PARTY_FLAG_IS_JUMPING | PARTY_FLAG_IS_FALLING | PARTY_FLAG_IS_STEPPING);
    party->velY = 0.0f;
    party->accelY = 0.0f;
    party->gravity = 0.0f;
}

static inline f32 partySpdRate(PartyEntry* party) {
    return (party->flags & PARTY_FLAG_IN_SHALLOW_WATER) ? 0.75f : 1.0f;
}

/**
 * Eases moveSpeed toward the target speed (targetSpeed scaled by Mario's playerGravity, 0.75 in shallow water;
 * Mario's own speed while he pushes) and returns it; with no target it slows to a stop.
 */
#if VERSION > VERSION_DEMO_US
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
#endif

/** partyUpdSpd again with its locals declared in another order (as found; the order can change register allocation). */
// FAKE: copy of partyUpdSpd with its locals declared in another order; using partyUpdSpd in partyChkUpdSpd changes the object
#if VERSION > VERSION_DEMO_US
/** partyUpdSpd again with its locals declared in another order (as found; the order can change register allocation). */
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
#endif

/**
 * partyUpdSpd, except while forced to move (forceMoveFrames) or jumping,
 * falling or climbing a step (motions 2, 3, 6), when moveSpeed is left as it is.
 */
static inline f32 partyChkUpdSpd(PartyEntry* party) {
    if (party->forceMoveFrames != 0 || party->motionId == PARTY_MOTION_JUMP || party->motionId == PARTY_MOTION_FALL || party->motionId == PARTY_MOTION_UPSTAIRS) {
        return party->moveSpeed;
    }
    return partyUpdSpd2(party);
}

/** Sets the speed the partner eases toward (0 = stop) and, when nonzero, takes one easing step at once. Returns spd. */
// Descriptive name (not in the symbol maps); placeholder unk_800c93d0, JP 0x800C93D0. See docs/NAME_PROVENANCE.md.
#if VERSION > VERSION_DEMO_US
/** Sets the speed the partner eases toward (0 = stop) and, when nonzero, takes one easing step at once. Returns spd. */
f32 partySetTargetSpd(PartyEntry* party, f32 spd) {
    if (spd) {
        party->targetSpeed = spd;
        partyChkUpdSpd(party);
        return spd;
    }
    party->targetSpeed = 0.0f;
    return spd;
}
#endif

/**
 * Sets both targetSpeed and moveSpeed at once to spd scaled by Mario's playerGravity (0.75 in shallow water),
 * with no easing. Returns spd.
 */
// Descriptive name (not in the symbol maps); placeholder unk_800c9398, JP 0x800C9398. See docs/NAME_PROVENANCE.md.
#if VERSION > VERSION_DEMO_US
/**
 * Sets both targetSpeed and moveSpeed at once to spd scaled by Mario's playerGravity (0.75 in shallow water),
 * with no easing. Returns spd.
 */
f32 partyForceSetSpd(PartyEntry* party, f32 spd) {
    f32 t = spd * party->playerPtr->playerGravity;
    party->targetSpeed = t * partySpdRate(party);
    party->moveSpeed = party->targetSpeed;
    return spd;
}
#endif

/**
 * One speed-easing step for this frame (partyUpdSpd) unless the partner is forced to move or is jumping,
 * falling or climbing a step; returns moveSpeed. Koops and Ms. Mowz call it from their walks.
 */
// Descriptive name (not in the symbol maps); placeholder unk_800c92c4, JP 0x800C92C4. See docs/NAME_PROVENANCE.md.
#if VERSION > VERSION_DEMO_US
/**
 * One speed-easing step for this frame (partyUpdSpd) unless the partner is forced to move or is jumping,
 * falling or climbing a step; returns moveSpeed. Koops and Ms. Mowz call it from their walks.
 */
f32 partyUpdMoveSpd(PartyEntry* party) {
    if (party->forceMoveFrames != 0 || party->motionId == PARTY_MOTION_JUMP || party->motionId == PARTY_MOTION_FALL || party->motionId == PARTY_MOTION_UPSTAIRS) {
        return party->moveSpeed;
    }
    return partyUpdSpd(party);
}
#endif

/** Plays the partner landing sound (0x928) at the partner, when a jump or fall ends (party_motion.c). */
// Descriptive name (not in the symbol maps).
#if VERSION > VERSION_DEMO_US
/** Plays the partner landing sound (0x928) at the partner, when a jump or fall ends (party_motion.c). */
void partyPlaySfxPartyLanding1(PartyEntry* party) {
    psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_PARTY_LANDING1), &party->position);
}
#endif

/**
 * Footstep sounds, counted on footstepTimer; arg 0 walking, 1 running (steps twice as often).
 * Flavio, the Yoshi egg and Punio have their own sounds and timing, flying partners (moveKind 1) a single sound;
 * the rest alternate two steps.
 */
#if VERSION > VERSION_DEMO_US
void partyWalkSe(PartyEntry* party, s32 arg) {
    s32 leftStepFrame;
    s32 stepFrames;

    party->footstepTimer++;
    if (party->currentMemberId == PARTY_MEMBER_FLAVIO) {
        if (arg == 0) {
            leftStepFrame = 20;
            stepFrames = 40;
        } else {
            leftStepFrame = 10;
            stepFrames = 20;
        }
        if (party->footstepTimer >= stepFrames) {
            party->footstepTimer = 0;
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_STG5_MARCO_MOVE1R), &party->position);
        } else if (party->footstepTimer == leftStepFrame) {
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_STG5_MARCO_MOVE1L), &party->position);
        }
    } else if (party->currentMemberId == PARTY_MEMBER_EGG) {
        if (party->footstepTimer >= 15) {
            party->footstepTimer = 0;
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_PARTY_YOSH_EGG1), &party->position);
        }
    } else if (party->currentMemberId == PARTY_MEMBER_PUNIO) {
        if (arg == 0) {
            leftStepFrame = 5;
            stepFrames = 10;
        } else {
            leftStepFrame = 5;
            stepFrames = 10;
        }
        if (party->footstepTimer >= stepFrames) {
            party->footstepTimer = 0;
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_STG0_PUNI_MOVE1), &party->position);
        } else if (party->footstepTimer == leftStepFrame) {
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_STG0_PUNI_MOVE1), &party->position);
        }
    } else if (party->moveKind == 1) {
        if (party->footstepTimer >= (!arg ? 40 : 20)) {
            party->footstepTimer = 0;
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_PARTY_MOVE2), &party->position);
        }
    } else {
        if (arg == 0) {
            leftStepFrame = 20;
            stepFrames = 40;
        } else {
            leftStepFrame = 10;
            stepFrames = 20;
        }
        if (party->footstepTimer >= stepFrames) {
            party->footstepTimer = 0;
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_PARTY_MOVE1R), &party->position);
        } else if (party->footstepTimer == leftStepFrame) {
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_PARTY_MOVE1L), &party->position);
        }
    }
}
#endif

/**
 * Puts the partner in its standing pose (1), or the sleep pose (6) while Mario is deeply asleep
 * or PARTY_FLAG_SLEEP_ON is set. Ms. Mowz keeps her pose while chuchu_searchObject finds something to sniff out.
 * Does nothing in run mode 0.
 */
// Descriptive name (not in the symbol maps); placeholder unk_800c9044, JP 0x800C9044. See docs/NAME_PROVENANCE.md.
#if VERSION > VERSION_DEMO_US
/**
 * Puts the partner in its standing pose (1), or the sleep pose (6) while Mario is deeply asleep
 * or PARTY_FLAG_SLEEP_ON is set. Ms. Mowz keeps her pose while chuchu_searchObject finds something to sniff out.
 * Does nothing in run mode 0.
 */
void partyChgStayPose(PartyEntry* party) {
    if (party->runMode != PARTY_RUN_MOVE_RESET) {
        if (marioChkDeepSleep() == TRUE || (party->flags & PARTY_FLAG_SLEEP_ON)) {
            partyChgPoseId(party, PARTY_POSE_SLEEP);
        } else if (party->currentMemberId != PARTY_MEMBER_MS_MOWZ) {
            partyChgPoseId(party, PARTY_POSE_STAND);
        } else if (!chuchu_searchObject()) {
            partyChgPoseId(party, PARTY_POSE_STAND);
        }
    }
}
#endif

// Descriptive name (not in the symbol maps); placeholder unk_800c8f24, JP 0x800C8F24. See docs/NAME_PROVENANCE.md.
#if VERSION > VERSION_DEMO_US
void partyChgWalkPose(PartyEntry* party, f32 spd) {
    f32 a;
    f32 b;

    if (party->flags2 & PARTY_DISP_FLAG_SLIT) {
        partyChgPoseId(party, PARTY_POSE_RUN);
        partyWalkSe(party, 1);
        return;
    }
    a = (1.0f == party->playerPtr->playerGravity) ? 1.65f : 2.0f;
    b = (1.0f == party->playerPtr->playerGravity) ? 0.8f : 0.0f;
    if (a - b < spd && spd < spd + b) {
        if (marioGetMovementSpeed() == 1) {
            partyChgPoseId(party, PARTY_POSE_WALK);
            partyWalkSe(party, 0);
        } else {
            partyChgPoseId(party, PARTY_POSE_RUN);
            partyWalkSe(party, 1);
        }
    } else if (spd >= a) {
        partyChgPoseId(party, PARTY_POSE_RUN);
        partyWalkSe(party, 1);
    } else {
        partyChgPoseId(party, PARTY_POSE_WALK);
        partyWalkSe(party, 0);
    }
}
#endif

/**
 * Turns the partner's sprite to face Mario on screen:
 * displayDirectionTarget 180 when Mario is on its left in the camera's view, else 0.
 * Used by evt_mario.c and party_motion.c.
 */
// Descriptive name (not in the symbol maps); placeholder unk_800c8e74, JP 0x800C8E74. See docs/NAME_PROVENANCE.md.
#if VERSION > VERSION_DEMO_US
/**
 * Turns the partner's sprite to face Mario on screen:
 * displayDirectionTarget 180 when Mario is on its left in the camera's view, else 0.
 * Used by evt_mario.c and party_motion.c.
 */
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
#endif

/**
 * TRUE when hit belongs to a map object a partner must not stand on (the objNames list: chests, jump stands,
 * Save and Heart Blocks, switches). party_motion.c then hops the partner off it (partyGetAppearPos5).
 */
// Descriptive name (not in the symbol maps); placeholder unk_800c8dd0, JP 0x800C8DD0. See docs/NAME_PROVENANCE.md.
#if VERSION > VERSION_DEMO_US
/**
 * TRUE when hit belongs to a map object a partner must not stand on (the objNames list: chests, jump stands,
 * Save and Heart Blocks, switches). party_motion.c then hops the partner off it (partyGetAppearPos5).
 */
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
#endif

/**
 * One frame of movement with corrections: the speed
 * and direction come from partyGetMoveDirSpd (push away from Mario, moving floors).
 */
void partyMove(PartyEntry* party) {
    partyMoveMain(party, 1);
}

/**
 * One frame of movement straight along targetDir at moveSpeed, without the push and floor corrections.
 * Used by party.c, Koops and Bobbery.
 */
void partyMoveNoHosei(PartyEntry* party) {
    partyMoveMain(party, 0);
}

/**
 * Moves the partner one frame: walls in front (or a straight move with wall checks off), the ceiling,
 * then a jump or the ground search and falling. Returns 0 on the ground, 1 jumping, 2 falling, 3 just landed.
 * hosei 1 adds the corrections of partyGetMoveDirSpd.
 */
#if VERSION <= VERSION_DEMO_US
static s32 partyMoveMain(PartyEntry* party, s32 hosei) {
#else
s32 partyMoveMain(PartyEntry* party, s32 hosei) {
#endif
    Vec pos;
    Vec pos2;
    f32 spd;
    f32 dir;
#if VERSION <= VERSION_DEMO_US
    f32 depth;
#endif
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
#if VERSION <= VERSION_DEMO_US
            marioChkWallAround(&pos, spd, dir, party->width, party->height);
#else
#if VERSION >= VERSION_US
            partyChkWallAround(party, &pos, spd, dir, party->width, party->height);
#else
            marioChkWallAround(&pos, 1, spd, dir, party->width, party->height);
#endif
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
        if (!party->hitobjHead) {
            party->position.y += party->velY;
        }
        party->velY += party->accelY;
        party->flags &= ~PARTY_FLAG_IS_GROUNDED;
        return 1;
    }
    hit = NULL;
    if (!(party->flags & PARTY_FLAG_FORCE_FALL) && party->velY <= 0.0f) {
        v = __fabsf(party->velY);
#if VERSION <= VERSION_DEMO_US
        hit = _partySearchGround(party, &pos.y, &depth, v, v);
#else
        hit = partySearchGround(party, v, v);
        pos.y = party->groundY;
#endif
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
#if VERSION <= VERSION_DEMO_US
            if (depth != 0.0f) {
#else
            if (party->dropDepth != 0.0f) {
#endif
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
    // FAKE: the copy p and its self-assignment in case 4/5 keep the empty case 4/5 arm; the plain switch changes the object
    PartyEntry* p = party;
    switch (p->moveMode) {
        case PARTY_MOVE_MODE_CONTINUE_BEHIND_MARIO:
            p->flags &= ~PARTY_FLAG_CONTINUE_FOLLOW_MARKER;
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
#if VERSION > VERSION_DEMO_US
    if (party->motionId == PARTY_MOTION_FALL) {
        party->flags &= ~(PARTY_FLAG_IS_JUMPING | PARTY_FLAG_IS_FALLING | PARTY_FLAG_IS_STEPPING);
        party->flags &= ~(PARTY_FLAG_DISABLE_FRONT_WALL_CHK | PARTY_FLAG_FORCE_FALL);
        party->flags &= ~PARTY_FLAG_DISABLE_FLOORS;
    }
#endif
    party->motionId = mot;
    party->trigFlags |= PARTY_TRIG_CHG_MOT;
}

/** Turns targetDir toward the point (x, z). */
void partySetDir(PartyEntry* party, f32 x, f32 z) {
    party->targetDir = angleABf(party->position.x, party->position.z, x, z);
}

/**
 * Sends the partner to its spot behind Mario (move mode 2). arg goes to moveArg.
 * Ignored while Yoshi is in run mode 3 (being used).
 */
void partyMoveBehindMario(PartyEntry* party, s32 arg) {
    if (party->currentMemberId == PARTY_MEMBER_YOSHI && party->runMode == PARTY_RUN_USE) {
        return;
    }
    party->moveArg = arg;
    partyChgMoveMode(party, PARTY_MOVE_MODE_BEHIND_MARIO);
    partyChgMot(party, PARTY_MOTION_WALK);
}

/**
 * partyMoveBehindMario for evt_mario.c's scripted moves:
 * also keeps a set homeDir (party_set_behind_dir) and uses the smaller push box around Mario.
 */
void partyMoveBehindMario3(PartyEntry* party, s32 arg, f32 dir) {
    if (party->currentMemberId == PARTY_MEMBER_YOSHI && party->runMode == PARTY_RUN_USE) {
        return;
    }
    party->trigFlags |= PARTY_TRIG_BEHIND_DIR_SET;
    party->trigFlags |= PARTY_TRIG_HALF_PUSH;
    party_set_behind_dir(party, dir);
    party->moveArg = arg;
    partyChgMoveMode(party, PARTY_MOVE_MODE_BEHIND_MARIO);
    partyChgMot(party, PARTY_MOTION_WALK);
}

/**
 * Sends the partner to its spot beside Mario (move mode 3), from evt_party.c. arg goes to moveArg.
 * Ignored while Yoshi is in run mode 3.
 */
void partyMoveBesideMario(PartyEntry* party, s32 arg) {
    if (party->currentMemberId == PARTY_MEMBER_YOSHI && party->runMode == PARTY_RUN_USE) {
        return;
    }
    party->moveArg = arg;
    partyChgMoveMode(party, PARTY_MOVE_MODE_BESIDE_MARIO);
    partyChgMot(party, PARTY_MOTION_WALK);
}

/**
 * Wall and floor check for a frame where the partner is moved some other way (party_motion.c's behind-Mario
 * motions): stops it at walls and lets it fall when there is no floor within 10 units.
 */
void partyMoveCheckWall(PartyEntry* party) {
    f32 spd;
    f32 dir;
    Vec pos;
    HitEntry* hit;
#if VERSION <= VERSION_DEMO_US
    f32 depth;
#endif
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
#if VERSION <= VERSION_DEMO_US
    hit = _partySearchGround(party, &pos.y, &depth, 10.0f, 10.0f);
#else
    hit = partySearchGround(party, 10.0f, 10.0f);
    pos.y = party->groundY;
#endif
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

/**
 * This frame's movement direction and speed: moveSpeed along targetDir plus moving-floor motion
 * and the push away from Mario (partyMarioHosei), times 0.75 in shallow water.
 * While being pushed it restarts the footmark trail,
 * and after 30 frames of pushing it sends the partner behind Mario.
 * JP, US and PAL ignore the push during door walks.
 */
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
    if (hosei && party->moveMode != PARTY_MOVE_MODE_BEHIND_MARIO) {
        if (!(party->flags & PARTY_FLAG_IS_PARTNER)) {
            partyClearFootmark();
        } else {
            far = FALSE;
            p = anotherPartyGetPtr(1);
#if VERSION <= VERSION_DEMO_US
            if (p) {
                if (nokonokoGetStatus(p) == 3) {
                    far = TRUE;
                }
#else
            if (p && (nokonokoGetStatus(p) == 3 ||
                      PSVECDistance(&mario->position, &p->position) >= 150.0f)) {
                far = TRUE;
#endif
            }
            if (far) {
                partyClearFootmark2();
#if VERSION <= VERSION_DEMO_US
            } else {
                partyClearFootmark();
#endif
            }
        }
#if VERSION > VERSION_DEMO_US
    }
    if (party->trigFlags & PARTY_TRIG_DOOR_MOVE) {
        hosei = 0;
#endif
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
#if VERSION <= VERSION_DEMO_US
        kx = 0.1f;
#else
        kx = 0.2f;
        kz = 0.15f;
#endif
        ky = kx;
#if VERSION <= VERSION_DEMO_US
        kz = kx;
#else
    }
    if (mario->flags & MARIO_FLAG_IS_PUSHING) {
        kx = 0.2f;
        kz = 0.15f;
        ky = kx;
#endif
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

/**
 * Stops the partner at walls in front of it with moveSpeed set to 0,
 * so only the push from Mario and moving floors count.
 * Used for a standing partner (party.c in the demo, party_motion.c).
 */
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

// Descriptive name (not in the symbol maps); placeholder unk_800c7c98, JP 0x800C7C98. See docs/NAME_PROVENANCE.md.
#if VERSION > VERSION_DEMO_US
/**
 * Run mode 1 in JP, US and PAL: the partner stands and waits (stand pose, Flurrie's hover, falling),
 * with the usual end-of-frame checks and Vivian's shadow.
 */
void party_move_mode_wait(PartyEntry* party) {
    if (party->trigFlags & PARTY_TRIG_CHG_MOVE_MODE) {
        party->trigFlags &= ~PARTY_TRIG_CHG_MOVE_MODE;
        partyChgPoseId(party, PARTY_POSE_STAND);
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
            partyChgMot(party, PARTY_MOTION_STAY);
            break;
        case 3:
            party_motion_fall(party);
            break;
    }
    partyModeTail(party);
    vivianShadow(party);
}
#endif

/**
 * Readies a walking partner: grounded, footmark trail restarted at whatever it follows, homing distance 50,
 * then stand pose, move mode 1 and motion 0 unless Mario has partner walking disabled.
 * The PAL release also ends any jump.
 */
void partyWalkInit(PartyEntry* party) {
#if VERSION <= VERSION_DEMO_US
    u8* p114;
    MarioWork* mario = party->playerPtr;

    party->flags |= PARTY_FLAG_IS_GROUNDED;
    partyClearFootmark();
#else
    MarioWork* mario = party->playerPtr;

    party->flags |= PARTY_FLAG_IS_GROUNDED;
#if VERSION == VERSION_EU
    party->flags &= ~(PARTY_FLAG_IS_JUMPING | PARTY_FLAG_IS_FALLING | PARTY_FLAG_IS_STEPPING);
#endif
    if (!(party->flags & PARTY_FLAG_IS_PARTNER)) {
        partyClearFootmark();
    } else {
        partyClearFootmark2();
    }
#endif
    party->moveArg = 0;
    party->reserved_44 = 0x10;
    party->homingDist = 50.0f;
    party->reserved_3E = 0;
    party->accelY = 1.8f;
    party->velY = 0.0f;
    if (!(mario->flags & MARIO_FLAG_PARTY_WALK_DISABLED)) {
        partyChgPoseId(party, PARTY_POSE_STAND);
        partyChgMoveMode(party, PARTY_MOVE_MODE_WALK);
        partyChgMot(party, PARTY_MOTION_STAY);
    }
}

/**
 * move_func of the walking partners (partyDataTbl):
 * reinitialises on a run-mode change, eases the speed (JP, US and PAL), then runs partyWalkMain.
 */
void partyMoveWalk(PartyEntry* party) {
    if (party->trigFlags & PARTY_TRIG_CHG_RUN_MODE) {
        party->trigFlags &= ~PARTY_TRIG_CHG_RUN_MODE;
        party->flags &= ~PARTY_FLAG_IS_BEING_USED;
        partyWalkInit(party);
    }
#if VERSION > VERSION_DEMO_US
    partyChkUpdSpd(party);
#endif
    partyWalkMain(party);
}

/**
 * One walking frame: lays this frame's footmark (Mario for the slot 0 entry; the other entry for slot 1,
 * or Mario when it is Bobbery in use, Koops's status 3 or far away) and runs the move mode.
 */
void partyWalkMain(PartyEntry* party) {
    PartyEntry* other;
    u32 flags = 0;
    MarioWork* mario = party->playerPtr;
    if (party->currentSlotId == 0) {
        if (partyChkMarioJumpFlag()) {
            flags |= FOOTMARK_FLAG_JUMP;
        }
        if (marioChkSlitThrouh()) {
#if VERSION <= VERSION_DEMO_US
            flags |= PARTY_FOOTMARK_DEMO_ARG_SLIT;
#else
            flags |= FOOTMARK_FLAG_SLIT;
#endif
        }
#if VERSION <= VERSION_DEMO_US
        partyPutFootmark(0, marioGetPtr()->position, flags);
#else
        partyPutFootmark(0, mario->position, flags);
#endif
    } else {
        other = anotherPartyGetPtr(party->currentSlotId);
        if (other) {
#if VERSION <= VERSION_DEMO_US
            if ((other->currentMemberId == PARTY_MEMBER_BOBBERY && other->runMode == PARTY_RUN_USE) || nokonokoGetStatus(other) == 3) {
#else
            if ((other->currentMemberId == PARTY_MEMBER_BOBBERY && other->runMode == PARTY_RUN_USE) || nokonokoGetStatus(other) == 3 ||
                PSVECDistance(&mario->position, &other->position) >= 150.0f) {
#endif
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

/**
 * Move mode 0: the partner stays where it is (stand motion, falls; JP,
 * US and PAL also run Flurrie's hover and end other motions).
 */
void party_move_mode_stay(PartyEntry* party) {
    if (party->trigFlags & PARTY_TRIG_CHG_MOVE_MODE) {
        party->trigFlags &= ~PARTY_TRIG_CHG_MOVE_MODE;
        partyChgPoseId(party, PARTY_POSE_STAND);
#if VERSION > VERSION_DEMO_US
        if (party->currentMemberId == PARTY_MEMBER_FLURRIE) {
            cloud_hover_init(party);
        }
#endif
    }
    switch (party->motionId) {
        case 0:
            party_motion_stay(party);
#if VERSION > VERSION_DEMO_US
            if (party->currentMemberId == PARTY_MEMBER_FLURRIE) {
                cloud_hover_main(party);
            }
            break;
        case 1:
        case 2:
        case 6:
            partyChgMot(party, PARTY_MOTION_STAY);
#endif
            break;
        case 3:
            party_motion_fall(party);
            break;
    }
    partyModeTail(party);
}

/**
 * Move mode 1: the partner follows its footmark trail (walking or flying homing motion), with jump, fall,
 * step and damage motions.
 */
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

/**
 * Move mode 2: the partner heads for its spot behind Mario (walking or flying), with jump, fall,
 * step and damage motions.
 */
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

/** Move mode 3: the partner heads for its spot beside Mario, with jump, fall, step and damage motions. */
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

/**
 * Run mode 9 (partySetForceMove, evt_party.c): walks straight along targetDir at moveSpeed for forceMoveFrames
 * frames, then back to the previous run mode.
 * Entering it cancels Bobbery's or Koops's field move; a wall in front starts a jump (motion 2).
 */
void partyForceWalk(PartyEntry* party) {
    Vec pos;
    HitEntry* hit;
    s32 attr;
#if VERSION <= VERSION_DEMO_US
    f32 gy;
    f32 depth;
#endif
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
                partyChgMot(party, PARTY_MOTION_JUMP);
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
#if VERSION <= VERSION_DEMO_US
            hit = _partySearchGround(party, &gy, &depth, 10.0f, 10.0f);
            if (hit && (hitGetAttr(hit) & HIT_ATTR_WATER_HAZARD)) {
#else
            hit = partySearchGround(party, 10.0f, 10.0f);
            attr = 0;
            if (hit && ((attr = hitGetAttr(hit)) & HIT_ATTR_WATER_HAZARD)) {
#endif
                hit = NULL;
            }
            party->hitobjStandOn = hit;
            if (!hit) {
                partyChgMot(party, PARTY_MOTION_FALL);
                party->accelY = -1.0f;
#if VERSION <= VERSION_DEMO_US
            } else if (hitGetAttr(hit) & HIT_ATTR_SHALLOW_WATER) {
#else
            } else if (attr & HIT_ATTR_SHALLOW_WATER) {
#endif
                party->flags |= PARTY_FLAG_IN_SHALLOW_WATER;
            } else {
                party->flags &= ~PARTY_FLAG_IN_SHALLOW_WATER;
            }
#if VERSION <= VERSION_DEMO_US
            party->position.y = gy;
#else
            party->position.y = party->groundY;
#endif
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

/**
 * Readies a flying partner (Flurrie, Vivian): grounded, footmark trail restarted; arg picks moveArg 0 or 2.
 * In move mode 2 it only resets the homing distance. Ends in the stand pose, move mode 1, motion 0.
 */
void partyMoveFlyInit(PartyEntry* party, s32 arg) {
    MarioWork* mario = marioGetPtr();

    party->flags |= PARTY_FLAG_IS_GROUNDED;
#if VERSION > VERSION_DEMO_US
    if (!(party->flags & PARTY_FLAG_IS_PARTNER)) {
        partyClearFootmark();
    } else {
        partyClearFootmark2();
    }
#else
    partyClearFootmark();
#endif
    if (party->moveMode == PARTY_MOVE_MODE_BEHIND_MARIO) {
        party->homingDist = 50.0f;
        return;
    }
    party->moveArg = arg == 0 ? 0 : 2;
    partyChgMoveMode(party, PARTY_MOVE_MODE_STAY);
    if (party->flyRequest == 1 || party->flyRequest == 3) {
        party->flyRequest = 0;
        party->moveArg = 0;
        partyChgMoveMode(party, PARTY_MOVE_MODE_WALK);
    } else if (party->flyRequest == 2) {
        party->moveArg = 0;
        partyChgMoveMode(party, 0x3C);
    }
    party->reserved_44 = 0x10;
    party->homingDist = 50.0f;
    party->reserved_3E = 0;
    partyChgPoseId(party, PARTY_POSE_STAND);
    partyChgMoveMode(party, PARTY_MOVE_MODE_WALK);
    partyChgMot(party, PARTY_MOTION_STAY);
}

/**
 * One flying frame for Flurrie and Vivian: lays this frame's footmark and runs the move mode.
 * For slot 1 it follows the slot 0 entry, falling back to Mario on trail 0.
 */
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

/**
 * Move mode 6: behind Mario, continued from move mode 2 (party_motion_behind_mario_continue), with jump, fall,
 * step and damage motions.
 */
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

/** Runs the current move mode for a walking partner. */
void walkMain(PartyEntry* party) {
    switch (party->moveMode) {
        case PARTY_MOVE_MODE_STAY:
            party_move_mode_stay(party);
            break;
        case PARTY_MOVE_MODE_WALK:
            party_move_mode_walk(party);
            break;
        case PARTY_MOVE_MODE_BEHIND_MARIO:
            party_move_mode_behind_mario(party);
            break;
        case PARTY_MOVE_MODE_BESIDE_MARIO:
            party_move_mode_beside_mario(party);
            break;
        case PARTY_MOVE_MODE_CONTINUE_BEHIND_MARIO:
            party_move_mode_behind_mario_continue(party);
            break;
    }
}

/** Runs the current move mode for a flying partner (the same modes as walkMain). */
static void flyMain(PartyEntry* party) {
    switch (party->moveMode) {
        case PARTY_MOVE_MODE_STAY:
            party_move_mode_stay(party);
            break;
        case PARTY_MOVE_MODE_WALK:
            party_move_mode_walk(party);
            break;
        case PARTY_MOVE_MODE_BEHIND_MARIO:
            party_move_mode_behind_mario(party);
            break;
        case PARTY_MOVE_MODE_BESIDE_MARIO:
            party_move_mode_beside_mario(party);
            break;
        case PARTY_MOVE_MODE_CONTINUE_BEHIND_MARIO:
            party_move_mode_behind_mario_continue(party);
            break;
    }
}

extern f32 partyNrmToAngle(Vec* n);

/**
 * TRUE when hit is floor a partner may appear on:
 * not water or a hazard (attr 0x200 / 0x800) and not a map object from objNames.
 */
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

/** Slope of a floor normal in degrees (partyNrmToAngle), taking the normal by value. */
// FAKE: one-use inline helper; writing the call out changes the object in both versions
/** Slope of a floor normal in degrees (partyNrmToAngle), taking the normal by value. */
static inline f32 partyAppearNrmAngle(Vec n) {
    return partyNrmToAngle(&n);
}

/** Moves point out d units back from direction ang, in the horizontal plane. */
static inline void partyAppearMoveBack(Vec* out, f32 d, f32 ang) {
    movePos(&out->x, &out->z, -d, ang);
}

/**
 * Where the partner appears next to Mario: 30 units behind him (behind and to the side when he has two party
 * members), else to his sides or front, the first spot with no wall in the way and floor within 100 units below.
 * Returns FALSE if none; out starts at Mario's position. On the plane it keeps Mario's position.
 */
#if VERSION > VERSION_DEMO_US
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
#endif

/**
 * Like partyGetAppearPos but checks only behind or beside Mario, with floor within 38 units below;
 * places a partner as it joins or appears (party.c).
 */
#if VERSION > VERSION_DEMO_US
BOOL partyGetAppearPos2(PartyEntry* party, Vec* out) {
    HitCheckQuery query;
    f32 s;
    f32 c;
    MarioWork* mario;
    f32 dir;
    f32 dist;
    f32 back;
    f32 ofs = 30.0f;

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
#endif

/**
 * Finds a free spot 30 units from Mario in nine directions around his facing:
 * no wall, floor within 87 units at under 60 degrees, no NPC close.
 * If every spot has an NPC it takes the one farthest from it.
 * Used by Goombella, Ms. Mowz and party_motion.c to put the partner back by Mario.
 */
#if VERSION > VERSION_DEMO_US
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
#if VERSION >= VERSION_US
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
#endif

/** Flying partners appear at least 5 units above Mario's feet. */
static inline void partyAppearAdjustY(PartyEntry* party, MarioWork* mario, Vec* out) {
    if (party->moveKind == 1 && out->y < mario->position.y) {
        out->y = 5.0f + mario->position.y;
    }
}

/**
 * The partner's follow spot: radius units (26, 28 on door walks, 32 for Flurrie) from Mario,
 * or from the other party member, in direction homeDir, else fanned around it;
 * door walks in Twilight Town's usu_00 use their own fan. Falls back to partyGetAppearPos and returns FALSE.
 */
#if VERSION > VERSION_DEMO_US
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

    // BUG: i runs up to n inclusive, so the last pass reads one element past the end of the angle table
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
#if VERSION >= VERSION_US
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
#endif

/** Stand-in for a function the original linker dead-stripped; it created 90.0f and 20.0f in the .sdata2 pool here. */
// Stand-in for a function the original linker dead-stripped; it created 90.0f and 20.0f in the .sdata2 pool here.
static void partyMoveStrippedPool(f32* f) {
#if VERSION <= VERSION_DEMO_US
    f[0] = 0.0f;
#else
    f[0] = 90.0f;
    f[1] = 20.0f;
#endif
}

#if VERSION > VERSION_DEMO_US
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
    mobj = &(mobjHitObjPtrToPtr(hit))->position;
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
#endif
