// Partner entries (PartyEntry): creating, removing and drawing partners, poses and paper animations, and the
// appear positions.

#include "version.h"
#include "party.h"
#include "driver/animdrv.h"
#include "mario/mario_pouch.h"
#include "mario/mario_party.h"
#include "mario/mario_cam.h"
#include "memory.h"
#include "system.h"
#include "mario/mariost.h"
#include <string.h>
#include "data/party_data.h"
#include "pmario_sound.h"
#include "mario/mario.h"
#include "sfx_name_word.h"
#include "party/party_move.h"
#include "party/party_bomhei.h"
#include "eff/eff_magic1_n64.h"
#include "libc_decls.h"
#include "party/party_yoshi.h"
#include "party/party_vivian.h"
#include "party/party_motion.h"
#include "party/party_chuchu.h"
#include "driver/shadowdrv.h"
#include "mot/mot_damage.h"
#include "mot/mot_ship.h"
#include "driver/hitdrv.h"
#include "party/party_hit.h"
#include "mario/mario_sbr.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "mario/mario_kemuri.h"

#if VERSION <= VERSION_DEMO_US
#include "mario/mario_hit.h"
#endif

// Ride teardown clears these two known flags plus legacy bit 18.
// Keep 0x40000: no standalone setter/reader was established for that bit.
enum { PARTY_RIDE_CLEAR_MASK = PARTY_FLAG_IS_RIDING_PLANE | PARTY_FLAG_IS_RIDING_SHIP | 0x40000 };

PartyEntry* partyPtrTbl[2];
PartyData* party_dp;
s32 partyCtrlNo;

s32 partyEntryMain(s32 memberId);

/** Returns the partner entry in slot index (0 is the main partner, 1 the follower), or NULL for a negative index. */
PartyEntry* partyGetPtr(s32 index) {
    if (index >= 0) {
        return partyPtrTbl[index];
    }
    return NULL;
}

/**
 * Returns the entry in the other slot than index (the follower for the main partner and the other way round),
 * or NULL for a negative index.
 */
PartyEntry* anotherPartyGetPtr(s32 index) {
    if (index < 0) {
        return NULL;
    }
    return partyPtrTbl[(index + 1) & 1];
}

/**
 * Starts a paper animation group (p_roll, p_plane, p_slit, ...) on the partner, replacing any running one.
 * The model is hidden during it except for the roll, plane and pipe papers,
 * and for the paper slit only when the partner is Flurrie.
 */
BOOL partyPaperOn(PartyEntry* party, const char* animGroup) {
    BOOL flag;

    if (party->paperPoseId >= 0) {
        animPaperPoseRelease(party->paperPoseId);
        party->paperPoseId = -1;
    }
    party->flags2 |= PARTY_DISP_FLAG_PAPER;
    party->paperPoseId = animPaperPoseEntry(animGroup, 2);
    flag = (!strcmp(animGroup, "p_roll") || !strcmp(animGroup, "p_plane") ||
            !strcmp(animGroup, "p_dokan_x") || !strcmp(animGroup, "p_dokan_y")) ? FALSE : TRUE;
    if (!strcmp(animGroup, "p_slit")) {
        flag = party->currentMemberId == PARTY_MEMBER_FLURRIE;
    }
    animPoseSetPaperAnimGroup(party->paperAnimGroup, animGroup, flag);
    return TRUE;
}

/** Ends the partner's paper animation and releases its paper pose, if one is active. */
void partyPaperOff(PartyEntry* party) {
    s32 paperPoseId;

    if ((party->flags2 & PARTY_DISP_FLAG_PAPER) && party->paperAnimName) {
        animPoseSetPaperAnimGroup(party->paperAnimGroup, NULL, FALSE);
        party->paperAnimName = NULL;
        party->flags2 &= ~PARTY_DISP_FLAG_PAPER;
        paperPoseId = party->paperPoseId;
        if (paperPoseId >= 0) {
            animPaperPoseRelease(paperPoseId);
            party->paperPoseId = -1;
        }
    }
}

/**
 * Turns off paper-mode lighting on the partner's model (used by the partner motion code when leaving a paper
 * shape).
 */
void partyPaperLightOff(PartyEntry* party) {
    animPoseSetMaterialLightFlagOff(party->paperAnimGroup, MATERIAL_LIGHT_PAPER);
}

// a partner is in a pinch at this HP or below
#if VERSION <= VERSION_DEMO_US
#define PARTY_PINCH_HP 1
#else
#define PARTY_PINCH_HP 5
#endif

/**
 * TRUE when one of the seven partners is in a pinch:
 * HP at or below PARTY_PINCH_HP (1 in the demos, 5 in the full game), which switches her to the tired pose set.
 */
static inline BOOL partyIsPinch(PartyEntry* party) {
    BOOL pinch = FALSE;

    if (party->currentMemberId < MAIN_PARTY_MEMBER_MAX) {
        if (partyGetHp(party->currentMemberId) <= PARTY_PINCH_HP) {
            pinch = TRUE;
        }
    }
    return pinch;
}

// FAKE: same behaviour as partyChgPoseId (inlined), but with the pinch check written out in place instead of
// partyIsPinch(). Only partyReInit's first call uses it: that call needs this form while its other
// two calls need the partyIsPinch() form (see docs/FAKES.md).
static inline const char* partyChgPoseIdNoPinch_inl(PartyEntry* party, s32 index) {
    BOOL pinch;
    s32 poseIdx = index;
    const char* pose;
    const char* curPose;

    if ((party->flags2 & PARTY_DISP_FLAG_POSE_LOCK) != 0) {
        return party->animName;
    }
    pinch = FALSE;
    if (party->currentMemberId < MAIN_PARTY_MEMBER_MAX) {
        if (partyGetHp(party->currentMemberId) <= PARTY_PINCH_HP) {
            pinch = TRUE;
        }
    }
    if (pinch == TRUE) {
        if (index == PARTY_POSE_STAND) {
            poseIdx = PARTY_POSE_PINCH_STAND;
        } else if (index == PARTY_POSE_WALK) {
            poseIdx = PARTY_POSE_PINCH_WALK;
        } else if (index == PARTY_POSE_RUN) {
            poseIdx = PARTY_POSE_PINCH_RUN;
        }
    }
    if (!(party->flags2 & PARTY_DISP_FLAG_FAMICOM)) {
        pose = partyPoseTbl[party->currentMemberId][poseIdx];
    } else if (party->currentMemberId != PARTY_MEMBER_YOSHI) {
        pose = partyDotPoseTbl[party->currentMemberId][poseIdx];
    } else {
        pose = yoshiDotPoseTbl[pouchGetPartyColor(PARTNER_YOSHI)][poseIdx];
    }
    curPose = party->animName;
    if (curPose && !strcmp(curPose, pose)) {
        return pose;
    }
    party->animName = pose;
    party->poseTime = 0;
    party->trigFlags |= PARTY_TRIG_CHG_ANIM;
    return pose;
}

/**
 * Starts the partner's pose number index from partyPoseTbl (or the 8-bit table in the Famicom pipe,
 * or Yoshi's colour table) and returns the pose name.
 * Indexes 1-3 (stand, walk, run) switch to the pinch poses 12-14 when her HP is low.
 * Does nothing while the pose is locked.
 */
const char* partyChgPoseId(PartyEntry* party, s32 index) {
    const char* pose;
    const char* curPose;
    s32 poseIdx = index;

    if ((party->flags2 & PARTY_DISP_FLAG_POSE_LOCK) != 0) {
        return party->animName;
    }
    if (partyIsPinch(party) == TRUE) {
        if (index == PARTY_POSE_STAND) {
            poseIdx = PARTY_POSE_PINCH_STAND;
        } else if (index == PARTY_POSE_WALK) {
            poseIdx = PARTY_POSE_PINCH_WALK;
        } else if (index == PARTY_POSE_RUN) {
            poseIdx = PARTY_POSE_PINCH_RUN;
        }
    }
    if (!(party->flags2 & PARTY_DISP_FLAG_FAMICOM)) {
        pose = partyPoseTbl[party->currentMemberId][poseIdx];
    } else if (party->currentMemberId != PARTY_MEMBER_YOSHI) {
        pose = partyDotPoseTbl[party->currentMemberId][poseIdx];
    } else {
        pose = yoshiDotPoseTbl[pouchGetPartyColor(PARTNER_YOSHI)][poseIdx];
    }
    curPose = party->animName;
    if (curPose && !strcmp(curPose, pose)) {
        return pose;
    }
    party->animName = pose;
    party->poseTime = 0;
    party->trigFlags |= PARTY_TRIG_CHG_ANIM;
    return pose;
}

/** Starts the named pose on the partner unless the pose is locked, already playing, or she is in 8-bit form. */
void partyChgPose(PartyEntry* party, const char* pose) {
    if (!(party->flags2 & PARTY_DISP_FLAG_POSE_LOCK)) {
        if (party->animName == NULL || strcmp(party->animName, pose)) {
            if (!(party->flags2 & PARTY_DISP_FLAG_FAMICOM)) {
                party->animName = pose;
                party->poseTime = 0;
                party->trigFlags |= PARTY_TRIG_CHG_ANIM;
            }
        }
    }
}

/**
 * Starts the named paper animation on the partner (applied by partyPreDisp) unless the pose is locked
 * or it is already playing.
 */
void partyChgPaper(PartyEntry* party, char* paper) {
    if (!(party->flags2 & PARTY_DISP_FLAG_POSE_LOCK)) {
        if (party->paperAnimName == NULL || strcmp(party->paperAnimName, paper)) {
            party->paperAnimName = paper;
            party->poseTime = 0;
            party->trigFlags |= PARTY_TRIG_CHG_PAPER_ANIM;
        }
    }
}

/**
 * Freezes both partner slots (PARTY_FLAG_CTRL_OFF): driveParty only draws them.
 * Used by events, battles and windows while the partner must stand still.
 */
void partyCtrlOff(void) {
    PartyEntry* party = partyPtrTbl[0];
    if (party) {
        party->flags |= PARTY_FLAG_CTRL_OFF;
    }
    party = partyPtrTbl[1];
    if (party) {
        party->flags |= PARTY_FLAG_CTRL_OFF;
    }
}

/** Lets both partner slots move again after partyCtrlOff. */
void partyCtrlOn(void) {
    PartyEntry* party = partyPtrTbl[0];
    if (party) {
        party->flags &= ~PARTY_FLAG_CTRL_OFF;
    }
    party = partyPtrTbl[1];
    if (party) {
        party->flags &= ~PARTY_FLAG_CTRL_OFF;
    }
}

/** Switches a standing partner (run mode 0) to waiting/following mode 1 (evt_party_run). */
void partyRun(PartyEntry* party) {
    if (party->runMode == PARTY_RUN_MOVE_RESET) {
        partyChgRunMode(party, PARTY_RUN_WAIT);
    }
}

/** Puts the partner in run mode 0: she stops in place with speed 0 instead of following Mario (evt_party_stop). */
void partyStop(PartyEntry* party) {
    partyChgRunMode(party, PARTY_RUN_MOVE_RESET);
}

/**
 * Makes the partner walk for frames frames in direction dir (degrees) at speed (run mode 9, partyForceWalk);
 * used by evt_party_move.
 */
void partySetForceMove(PartyEntry* party, s32 frames, f32 dir, f32 speed) {
    party->targetDir = dir;
    party->forceMoveFrames = frames;
    party->moveSpeed = speed;
    partyChgRunMode(party, PARTY_RUN_FORCE_WALK);
    partyChgMot(party, PARTY_MOTION_WALK);
}

/** Creates the partner memberId in a free slot and has her pop out of Mario (run mode 4). Returns the slot or -1. */
s32 partyEntryHello(s32 memberId) {
    s32 id = partyEntryMain(memberId);
    partyChgRunMode(partyGetPtr(id), 4);
    return id;
}

/**
 * Creates the partner memberId in a free slot, already standing at her appear position next to Mario,
 * waiting to follow him (run mode 1). Returns the slot or -1.
 */
s32 partyEntry(s32 memberId) {
    PartyEntry* party;
    s32 id;
    MarioWork* player;

    id = partyEntryMain(memberId);
    party = partyGetPtr(id);
    party->scale = (Vec){1.0f, 1.0f, 1.0f};
    party->motionStep = 10;
    player = party->playerPtr;
    party->displayDirectionTarget = player->displayDirection;
    party->displayDirection = player->displayDirection;
    partyGetAppearPos2(party, &party->position);
    party->trailPos = party->position;
    partyChgPoseId(party, PARTY_POSE_STAND);
    party->runMode = PARTY_RUN_WAIT;
    return id;
}

/**
 * Creates the partner memberId standing still at x, y, z facing Mario's way (run mode 0),
 * as after a pipe or when an event places her. Returns the slot or -1.
 */
s32 partyEntryPos(s32 memberId, f32 x, f32 y, f32 z) {
    PartyEntry* party;
    s32 index;
    char* pose;
    s32 id;
    BOOL lowHp;
    MarioWork* player;

    id = partyEntryMain(memberId);
    party = partyGetPtr(id);
    party->scale = (Vec){1.0f, 1.0f, 1.0f};
    party->position = (Vec){x, y, z};
    player = party->playerPtr;
    party->scale = (Vec){1.0f, 1.0f, 1.0f};
    party->displayDirectionTarget = player->displayDirection;
    party->displayDirection = player->displayDirection;
    index = PARTY_POSE_STAND;
    if (!(party->flags2 & PARTY_DISP_FLAG_POSE_LOCK)) {
        lowHp = FALSE;
        if (party->currentMemberId < MAIN_PARTY_MEMBER_MAX && partyGetHp(party->currentMemberId) <= PARTY_PINCH_HP) {
            lowHp = TRUE;
        }
        if (lowHp == TRUE) {
            index = PARTY_POSE_PINCH_STAND;
        }
        if (!(party->flags2 & PARTY_DISP_FLAG_FAMICOM)) {
            pose = partyPoseTbl[party->currentMemberId][index];
        } else if (party->currentMemberId != PARTY_MEMBER_YOSHI) {
            pose = partyDotPoseTbl[party->currentMemberId][index];
        } else {
            pose = yoshiDotPoseTbl[pouchGetPartyColor(PARTNER_YOSHI)][index];
        }
        if (party->animName == NULL || strcmp(party->animName, pose)) {
            party->animName = pose;
            party->poseTime = 0;
            party->trigFlags |= PARTY_TRIG_CHG_ANIM;
        }
    }
    party->runMode = PARTY_RUN_MOVE_RESET;
    return id;
}

/** Sends the partner in slot id away with her goodbye animation (run mode 8). Returns FALSE for a negative id. */
BOOL partyGoodbye(s32 id) {
    BOOL ret = FALSE;

    if (id >= 0) {
        partyChgRunMode(partyGetPtr(id), PARTY_RUN_GOODBYE);
        ret = TRUE;
    }
    return ret;
}

/**
 * Removes the partner in slot id at once: runs her member's exit function (not while riding, in the full game),
 * remembers her as the previous member of that slot, clears the slot and frees the entry and model.
 */
s32 partyKill(s32 id) {
    PartyEntry* party;
    MarioWork* player;

    party = partyGetPtr(id);
    if (party == NULL) {
        return 0;
    }
    player = party->playerPtr;
    party_dp = &partyDataTbl[partyGetPtr(player->currPartySlotId[id])->currentMemberId];
#if VERSION <= VERSION_DEMO_US
    if (party_dp->exit_func) {
#else
    if (party_dp->exit_func && party->runMode != PARTY_RUN_RIDE) {
#endif
        party_dp->exit_func(party);
    }
    if (partyGetPtr(player->currPartySlotId[id])) {
        player->prevPartyMemberId[id] = partyGetPtr(player->currPartySlotId[id])->currentMemberId;
    } else {
        player->prevPartyMemberId[id] = 0;
    }
    player->currPartySlotId[id] = -1;
    partyPtrTbl[id] = NULL;
    animPoseRelease(party->paperAnimGroup);
    __memFree(HEAP_DEFAULT, party);
    return 0;
}

/**
 * Like partyKill, but forgets the slot's previous member instead of remembering it;
 * evt_mario uses it for the follower slot.
 */
s32 partyKill2(s32 id) {
    PartyEntry* party;
    MarioWork* player;

    party = partyGetPtr(id);
    if (party == NULL) {
        return 0;
    }
    player = party->playerPtr;
    party_dp = &partyDataTbl[partyGetPtr(player->currPartySlotId[id])->currentMemberId];
#if VERSION <= VERSION_DEMO_US
    if (party_dp->exit_func) {
#else
    if (party_dp->exit_func && party->runMode != PARTY_RUN_RIDE) {
#endif
        party_dp->exit_func(party);
    }
    player->prevPartyMemberId[id] = 0;
    player->currPartySlotId[id] = -1;
    partyPtrTbl[id] = NULL;
    animPoseRelease(party->paperAnimGroup);
    __memFree(HEAP_DEFAULT, party);
    return 0;
}

/**
 * Picks Yoshi's colour at random when he hatches in Glitzville (stg3_12_2_init):
 * the first six colours 15% each, the seventh 10%. Stores it in the pouch and returns it.
 */
s32 yoshiSetColor(void) {
    s32 color;
    s32 r = irand(1000);

    if (r < 150) {
        color = 0;
    } else if (r < 300) {
        color = 1;
    } else if (r < 450) {
        color = 2;
    } else if (r < 600) {
        color = 3;
    } else if (r < 750) {
        color = 4;
    } else if (r < 900) {
        color = 5;
    } else {
        color = 6;
    }
    pouchSetPartyColor(PARTNER_YOSHI, color);
    return color;
}

/** The partner's height for collision and riding; Yoshi counts 18.5 more while Mario rides him. */
f32 partyGetHeight(PartyEntry* party) {
    if (party->currentMemberId == PARTY_MEMBER_YOSHI && (party->flags & PARTY_FLAG_IS_BEING_USED)) {
        return 18.5f + party->height;
    }
    return party->height;
}

/**
 * Finds the first empty partner slot from start, allocates and zeroes a new entry there and returns its index,
 * or -1 if both are taken.
 */
static inline s32 partyGetFreeSlot(s32 start) {
    s32 i;

    for (i = start; i < 2; i++) {
        if (partyPtrTbl[i] == NULL) {
            memset(partyPtrTbl[i] = __memAlloc(HEAP_DEFAULT, sizeof(PartyEntry)), 0, sizeof(PartyEntry));
            return i;
        }
    }
    return -1;
}

/**
 * Loads the partner's model: her member's animation group, Yoshi's group for his colour,
 * or the 8-bit d_mario group in the Famicom pipe.
 */
static void partyPoseEntry(PartyEntry* party) {
    char* animName;

    if (party->currentMemberId != PARTY_MEMBER_YOSHI) {
        if (!(party->flags2 & PARTY_DISP_FLAG_FAMICOM)) {
            animName = party_dp->name;
        } else {
            animName = "d_mario";
        }
    } else {
        animName = yoshigroup[pouchGetPartyColor(PARTNER_YOSHI)];
        if (party->flags2 & PARTY_DISP_FLAG_FAMICOM) {
            animName = "d_mario";
        }
    }
    if (party->paperAnimGroup >= 0) {
        animPoseRelease(party->paperAnimGroup);
    }
    party->paperAnimGroup = animPoseEntry(animName, 2);
}

/** Copies Mario's current material and light flags to the partner's model, so she is lit and tinted like him. */
static void setAnimPoseMaterialFlag(PartyEntry* party) {
    MarioWork* player = party->playerPtr;

    animPoseSetMaterialFlagOn(party->paperAnimGroup, animPoseGetMaterialFlag(player->paperAnimGroupId[marioAnimeId()]));
    animPoseSetMaterialLightFlagOn(party->paperAnimGroup,
                                   animPoseGetMaterialLightFlag(player->paperAnimGroupId[marioAnimeId()]));
}

/** Allocates and zeroes the follower slot (1) if it is empty; returns 1, or -1 if taken. */
static inline s32 partyGetFreeSlot2(void) {
    if (partyPtrTbl[1] == NULL) {
        memset(partyPtrTbl[1] = __memAlloc(HEAP_DEFAULT, sizeof(PartyEntry)), 0, sizeof(PartyEntry));
        return 1;
    }
    return -1;
}

/**
 * Animation group names for Yoshi's seven colours,
 * indexed by the colour he hatched with (pouchGetPartyColor).
 */
char* yoshigroup[7] = {
    "c_babyyoshi",
    "c_babyyoshi2",
    "c_babyyoshi3",
    "c_babyyoshi4",
    "c_babyyoshi5",
    "c_babyyoshi6",
    "c_babyyoshi7"
};

/**
 * Shared setup of partyEntry*: allocates a free slot,
 * fills in the entry for memberId (camera, Mario, follower flag for slot 1, 8-bit form),
 * loads the model and standing pose and calls the member's init function. Returns the slot or -1.
 */
s32 partyEntryMain(s32 memberId) {
    MarioWork* player;
    s32 slot;
    PartyEntry* party;

    player = marioGetPtr();
    slot = partyGetFreeSlot(0);
    if (slot < 0) {
        return -1;
    }
    party = partyGetPtr(slot);
    memset(party, 0, sizeof(PartyEntry));
    party->camId = marioGetCamId();
    party->paperAnimGroup = -1;
    party->reserved_10 = -1;
    party->paperPoseId = -1;
    party->flags = PARTY_FLAG_IS_ACTIVE;
    party->currentSlotId = slot;
    party->playerPtr = player;
    party->currentMemberId = memberId;
    if (player->prevPartyMemberId[0] == 0) {
        player->prevPartyMemberId[0] = party->currentMemberId;
    }
    if (slot == 1) {
        party->flags |= PARTY_FLAG_IS_FOLLOWER;
    }
    party_dp = &partyDataTbl[memberId];
    if (player->flags & MARIO_FLAG_IS_8_BIT) {
        party->flags2 |= PARTY_DISP_FLAG_FAMICOM;
    }
    partyPoseEntry(party);
    animPosePeraOff(party->paperAnimGroup);
    setAnimPoseMaterialFlag(party);
    partyChgPose(party, partyChgPoseId(party, PARTY_POSE_STAND));
    party->paperAnimName = NULL;
    party_dp->init_func(party);
    return slot;
}

/**
 * Creates follower memberId (Flavio, Frankly, a partner tagging along, ...) in slot 1 at Mario's position,
 * standing still (run mode 0). Returns the slot or -1 if the slot is taken.
 */
s32 partyEntry2(s32 memberId) {
    MarioWork* player;
    s32 slot;
    PartyEntry* party;

    player = marioGetPtr();
    slot = partyGetFreeSlot2();
    if (slot < 0) {
        return -1;
    }
    party = partyGetPtr(slot);
    memset(party, 0, sizeof(PartyEntry));
    party->camId = marioGetCamId();
    party->paperAnimGroup = -1;
    party->reserved_10 = -1;
    party->paperPoseId = -1;
    party->flags = PARTY_FLAG_IS_ACTIVE | PARTY_FLAG_IS_PARTNER;
    party->currentSlotId = slot;
    party->playerPtr = player;
    party->currentMemberId = memberId;
    if (slot == 1) {
        party->flags |= PARTY_FLAG_IS_FOLLOWER;
    }
    party_dp = &partyDataTbl[memberId];
    partyPoseEntry(party);
    animPosePeraOff(party->paperAnimGroup);
    setAnimPoseMaterialFlag(party);
    partyChgPose(party, partyChgPoseId(party, PARTY_POSE_STAND));
    party->paperAnimName = NULL;
    party_dp->init_func(party);
    party->position = player->position;
    party->trailPos = party->position;
    party->scale = (Vec){1.0f, 1.0f, 1.0f};
    party->runMode = PARTY_RUN_MOVE_RESET;
    return slot;
}

/** Creates follower memberId in slot 1 popping out of Mario (run mode 4). Returns the slot or -1. */
s32 partyEntry2Hello(s32 memberId) {
    MarioWork* player;
    s32 slot;
    PartyEntry* party;

    player = marioGetPtr();
    slot = partyGetFreeSlot2();
    if (slot < 0) {
        return -1;
    }
    party = partyGetPtr(slot);
    memset(party, 0, sizeof(PartyEntry));
    party->camId = marioGetCamId();
    party->paperAnimGroup = -1;
    party->reserved_10 = -1;
    party->paperPoseId = -1;
    party->flags = PARTY_FLAG_IS_ACTIVE | PARTY_FLAG_IS_PARTNER;
    party->currentSlotId = slot;
    party->playerPtr = player;
    party->currentMemberId = memberId;
    if (slot == 1) {
        party->flags |= PARTY_FLAG_IS_FOLLOWER;
    }
    party_dp = &partyDataTbl[memberId];
    partyPoseEntry(party);
    animPosePeraOff(party->paperAnimGroup);
    setAnimPoseMaterialFlag(party);
    partyChgPose(party, partyChgPoseId(party, PARTY_POSE_STAND));
    party->paperAnimName = NULL;
    party_dp->init_func(party);
    partyChgRunMode(party, 4);
    return slot;
}

/** Creates follower memberId in slot 1 at x, y, z, following Mario (run mode 2). Returns the slot or -1. */
s32 partyEntry2Pos(s32 memberId, f32 x, f32 y, f32 z) {
    MarioWork* player;
    s32 slot;
    PartyEntry* party;

    player = marioGetPtr();
    slot = partyGetFreeSlot2();
    if (slot < 0) {
        return -1;
    }
    party = partyGetPtr(slot);
    memset(party, 0, sizeof(PartyEntry));
    party->camId = marioGetCamId();
    party->paperAnimGroup = -1;
    party->reserved_10 = -1;
    party->paperPoseId = -1;
    party->flags = PARTY_FLAG_IS_ACTIVE | PARTY_FLAG_IS_PARTNER;
    party->currentSlotId = slot;
    party->playerPtr = player;
    party->currentMemberId = memberId;
    if (slot == 1) {
        party->flags |= PARTY_FLAG_IS_FOLLOWER;
    }
    party_dp = &partyDataTbl[memberId];
    partyPoseEntry(party);
    animPosePeraOff(party->paperAnimGroup);
    setAnimPoseMaterialFlag(party);
    partyChgPose(party, partyChgPoseId(party, PARTY_POSE_STAND));
    party->paperAnimName = NULL;
    party_dp->init_func(party);
    partyChgRunMode(party, PARTY_RUN_MOVE);
    party->position = (Vec){x, y, z};
    party->trailPos = party->position;
    party->scale.x = 1.0f;
    party->scale.y = 1.0f;
    party->scale.z = 1.0f;
    return slot;
}

void driveParty(PartyEntry* party);
s32 partyHelloMain(PartyEntry* party);
/** Lets partyPreDisp draw the partner's shadow again. */
static void partyShadowOn(PartyEntry* party) {
    if (party) {
        party->flags2 &= ~PARTY_DISP_FLAG_SHADOW_OFF;
    }
}

/** Clears both partner slots at game start. */
void partyInit(void) {
    memset(partyPtrTbl, 0, sizeof(partyPtrTbl));
}

/**
 * Resets both partners when Mario is reinitialised on a map change (marioReInit):
 * clears their flags, paper animations, hit objects and scale and puts them back in following mode,
 * or in the walk-disabled wait when Mario's partner walking is off.
 * Yoshi being ridden is left alone; Ms. Mowz gets chuchu_reinit.
 */
void partyReInit(void) {
    s32 poseId;
    s32 dotted;
    PartyEntry* party;
    const char* pose;

    party = partyGetPtr(marioGetExtraPartyId());
    if (party) {
        party->flags = PARTY_FLAG_IS_ACTIVE | PARTY_FLAG_IS_FOLLOWER | PARTY_FLAG_IS_PARTNER;
        party->flags2 = 0;
        poseId = party->paperAnimGroup;
        if (poseId >= 0) {
            animPoseSetMaterialFlagOff(poseId, -1);
            animPoseSetMaterialLightFlagOn(poseId, MATERIAL_LIGHT_CHARA);
#if VERSION > VERSION_DEMO_US
        }
        party->slitOn = 0;
        if (party->paperAnimName) {
            animPoseSetPaperAnimGroup(party->paperAnimGroup, NULL, FALSE);
            party->paperAnimName = NULL;
            party->flags2 &= ~PARTY_DISP_FLAG_PAPER;
            if (party->paperPoseId >= 0) {
                animPaperPoseRelease(party->paperPoseId);
                party->paperPoseId = -1;
            }
#endif
        }
        party->hitobjAction = NULL;
        party->hitobjFront = NULL;
        party->reserved_148 = 0;
        party->hitobjStandOn = NULL;
        party->hitobjJumpFrom = NULL;
        party->hitobjPush = NULL;
        party->hitobjHead = NULL;
        party->hitobjUnder = NULL;
        partyChgPose(party, partyChgPoseIdNoPinch_inl(party, 1)); // FAKE: see partyChgPoseIdNoPinch_inl
        partyChgRunMode(party, PARTY_RUN_MOVE);
        partyChgMot(party, PARTY_MOTION_STAY);
        party->yoshiPsndSFXId = -1;
        party->scale = (Vec){1.0f, 1.0f, 1.0f};
    }
    party = partyGetPtr(marioGetPartyId());
    if (party) {
        dotted = party->flags2 & PARTY_DISP_FLAG_FAMICOM;
        party->flags = PARTY_FLAG_IS_ACTIVE;
        party->flags2 = 0;
#if VERSION > VERSION_DEMO_US
        party->slitOn = 0;
#endif
        party->camId = marioGetCamId();
        if (party) {
            poseId = party->paperAnimGroup;
            if (poseId >= 0) {
                animPoseSetMaterialFlagOff(poseId, -1);
                animPoseSetMaterialLightFlagOn(poseId, MATERIAL_LIGHT_CHARA);
#if VERSION > VERSION_DEMO_US
            }
            if (party->paperAnimName) {
                animPoseSetPaperAnimGroup(party->paperAnimGroup, NULL, FALSE);
                party->paperAnimName = NULL;
                party->flags2 &= ~PARTY_DISP_FLAG_PAPER;
                if (party->paperPoseId >= 0) {
                    animPaperPoseRelease(party->paperPoseId);
                    party->paperPoseId = -1;
                }
#endif
            }
            party->hitobjAction = NULL;
            party->hitobjFront = NULL;
            party->reserved_148 = 0;
            party->hitobjStandOn = NULL;
            party->hitobjJumpFrom = NULL;
            party->hitobjPush = NULL;
            party->hitobjHead = NULL;
            party->hitobjUnder = NULL;
            party->scale = (Vec){1.0f, 1.0f, 1.0f};
        }
        partyClearFootmark();
        if (dotted) {
            partyPoseEntry(party);
        }
        if (party->currentMemberId == PARTY_MEMBER_YOSHI && party->runMode == PARTY_RUN_USE) {
            return;
        }
        if (!(marioGetPtr()->flags & MARIO_FLAG_PARTY_WALK_DISABLED)) {
            pose = partyChgPoseId(party, PARTY_POSE_STAND);
            partyChgPose(party, pose);
            partyChgRunMode(party, PARTY_RUN_MOVE);
            partyChgMot(party, PARTY_MOTION_STAY);
        } else {
            pose = partyChgPoseId(party, PARTY_POSE_STAND);
            partyChgPose(party, pose);
            partyChgRunMode(party, PARTY_RUN_MOVE);
            party->moveMode = PARTY_MOVE_MODE_CONTINUE_BEHIND_MARIO;
            party->motionStep = 0;
            party->flags |= PARTY_FLAG_CONTINUE_FOLLOW_MARKER;
        }
        if (party->currentMemberId == PARTY_MEMBER_MS_MOWZ) {
            chuchu_reinit(party);
        }
#if VERSION > VERSION_DEMO_US
        partyResetTouchWallCt();
#endif
    }
}

/**
 * Runs the partner's member post function if she is in the middle of her field ability (run mode 3),
 * to end it cleanly (from Mario's battle and map-exit code).
 */
void partyUsePost(PartyEntry* party) {
    PartyFunc func;
    if (party && party->runMode == PARTY_RUN_USE) {
        func = partyDataTbl[party->currentMemberId].post_func;
        if (func) {
            func(party);
        }
    }
}

static inline void partyChgRunM(PartyEntry* party);
static inline void partyChgRunMo(PartyEntry* party);
/**
 * Cleanup when the partner leaves run mode prev for mode:
 * gets off a ride (11) unless she is leaving, ends the hello pop-out (4) back into following,
 * shows the shadow again after riding Yoshi (12), ends her ability (3),
 * and drops the paper slit from waiting/following.
 */
// Descriptive name (not in the symbol maps).
#if VERSION <= VERSION_DEMO_US
/** the demo only leaves the hello state (4) and runs the post function (3) on a run-mode change */
static inline void partyLeaveRunMode(PartyEntry* party) {
#else
/**
 * Cleanup when the partner leaves run mode prev for mode:
 * gets off a ride (11) unless she is leaving, ends the hello pop-out (4) back into following,
 * shows the shadow again after riding Yoshi (12), ends her ability (3),
 * and drops the paper slit from waiting/following.
 */
void _partyChgRunMode(PartyEntry* party, s32 prev, s32 mode) {
#endif
    MarioWork* mario;
    PartyFunc func;

#if VERSION <= VERSION_DEMO_US
    if (party->runMode != PARTY_RUN_FORCE_WALK && party->runMode != PARTY_RUN_USE) {
        party->prevRunMode = party->runMode;
    }
    if (party->runMode == 4) {
#else
    switch (prev) {
        case 11:
            if (mode != 8) {
                mario = party->playerPtr;
                partyChgRunMo(party);
                party->moveDir = mario->moveDirection;
                party->targetDir = mario->directionView;
                party->displayDirectionTarget = mario->displayDirectionTarget;
                party->flags &= ~PARTY_RIDE_CLEAR_MASK;
                partyClearJumpPara(party);
                partyChgPoseId(party, PARTY_POSE_STAND);
                party->flags2 &= ~PARTY_DISP_FLAG_NO_MAKE_DISP_DIR;
                party->flags2 &= ~PARTY_DISP_FLAG_HIDE;
                partyShadowOn(party);
            }
            break;
        case 4:
#endif
            mario = party->playerPtr;
            partyChgRunM(party);
            party->flags2 &= ~PARTY_DISP_FLAG_NO_MAKE_DISP_DIR;
            party->moveDir = mario->moveDirection;
            party->targetDir = mario->directionView;
            party->displayDirectionTarget = mario->displayDirectionTarget;
            party->displayDirection = mario->displayDirection;
            partyClearJumpPara(party);
            partyChgPoseId(party, PARTY_POSE_STAND);
            party->flags2 &= ~PARTY_DISP_FLAG_NO_MAKE_DISP_DIR;
            party->runMode = PARTY_RUN_WAIT;
#if VERSION > VERSION_DEMO_US
            break;
        case 12:
            partyShadowOn(party);
            break;
        case 3:
            func = partyDataTbl[party->currentMemberId].post_func;
            if (func) {
                func(party);
            }
            break;
        case 1:
        case 2:
            partyForceSlitOff(party);
            break;
    }
}

#pragma dont_inline on
#else
        }
        partyUsePost(party);
    }
#endif

/**
 * Switches the partner to run mode mode (0 stand, 1 wait, 2 follow, 3 ability, 4-7 appear, 8 goodbye,
 * 9 forced walk, 10 pipe, 11 ride, 12 ride Yoshi, 13 return after damage, 14 delete),
 * remembering the old mode for partyPrevRunMode. The demo only ends the appear and ability modes.
 */
void partyChgRunMode(PartyEntry* party, s32 mode) {
#if VERSION <= VERSION_DEMO_US
    partyLeaveRunMode(party);
#else
    if (party->runMode != PARTY_RUN_FORCE_WALK && party->runMode != PARTY_RUN_USE) {
        party->prevRunMode = party->runMode;
    }
    _partyChgRunMode(party, party->runMode, mode);
#endif
    party->runMode = mode;
    party->trigFlags |= PARTY_TRIG_CHG_RUN_MODE;
}
#pragma dont_inline reset

#if VERSION > VERSION_DEMO_US
#pragma dont_inline on
#endif
/**
 * Like partyChgRunMode but enters mode already at motion step sub,
 * without the mode's start-up (the change trigger is cleared). Unused in C.
 */
// Descriptive name (not in the symbol maps).
void partyForceChgRunMode(PartyEntry* party, s32 mode, s32 sub) {
#if VERSION <= VERSION_DEMO_US
    partyLeaveRunMode(party);
#else
    if (party->runMode != PARTY_RUN_FORCE_WALK && party->runMode != PARTY_RUN_USE) {
        party->prevRunMode = party->runMode;
    }
    _partyChgRunMode(party, party->runMode, mode);
    party->runMode = mode;
#endif
    party->trigFlags &= ~PARTY_TRIG_CHG_RUN_MODE;
#if VERSION <= VERSION_DEMO_US
    party->runMode = mode;
#endif
    party->motionStep = sub;
}

/**
 * Returns the partner to the run mode she had before the current one,
 * ending her ability first (evt_party and party_move).
 */
#if VERSION <= VERSION_DEMO_US
void partyPrevRunMode(PartyEntry* party) {
    if (party->runMode == party->prevRunMode) {
        return;
    }
    partyUsePost(party);
    party->runMode = party->prevRunMode;
    party->trigFlags |= PARTY_TRIG_CHG_RUN_MODE;
    partyChgMot(party, PARTY_MOTION_STAY);
}
#endif
#pragma dont_inline reset

#if VERSION > VERSION_DEMO_US
void partyPrevRunMode(PartyEntry* party) {
    if (party->runMode == party->prevRunMode) {
        return;
    }
    if (party->runMode == 4) {
        MarioWork* mario = party->playerPtr;
        partyChgRunM(party);
        party->flags2 &= ~PARTY_DISP_FLAG_NO_MAKE_DISP_DIR;
        party->moveDir = mario->moveDirection;
        party->targetDir = mario->directionView;
        party->displayDirectionTarget = mario->displayDirectionTarget;
        party->displayDirection = mario->displayDirection;
        partyClearJumpPara(party);
        partyChgPoseId(party, PARTY_POSE_STAND);
        party->flags2 &= ~PARTY_DISP_FLAG_NO_MAKE_DISP_DIR;
        party->runMode = PARTY_RUN_WAIT;
        partyUsePost(party);
        partyChgMot(party, PARTY_MOTION_STAY);
    } else if (party->runMode == PARTY_RUN_RIDE) {
        MarioWork* mario = party->playerPtr;
        partyChgRunMo(party);
        party->moveDir = mario->moveDirection;
        party->targetDir = mario->directionView;
        party->displayDirectionTarget = mario->displayDirectionTarget;
        party->flags &= ~PARTY_RIDE_CLEAR_MASK;
        partyClearJumpPara(party);
        partyChgPoseId(party, PARTY_POSE_STAND);
        party->flags2 &= ~PARTY_DISP_FLAG_NO_MAKE_DISP_DIR;
        party->flags2 &= ~PARTY_DISP_FLAG_HIDE;
        partyShadowOn(party);
        partyUsePost(party);
        partyChgMot(party, PARTY_MOTION_STAY);
    } else {
        if (party->runMode == PARTY_RUN_RIDE_YOSHI) {
            partyShadowOn(party);
        }
        partyUsePost(party);
        party->runMode = party->prevRunMode;
        party->trigFlags |= PARTY_TRIG_CHG_RUN_MODE;
        partyChgMot(party, PARTY_MOTION_STAY);
    }
}
#endif

/** Counts the active partner slots. */
static inline s32 partyCountActive(void) {
    s32 count = 0;
    s32 i;
    for (i = 0; i < 2; i++) {
        if (partyPtrTbl[i] && (partyGetPtr(i)->flags & PARTY_FLAG_IS_ACTIVE)) {
            count++;
        }
    }
    return count;
}

/**
 * Per-frame partner update from Mario's main loop:
 * when any partner is active (the full game also runs marioUsePartyMain first),
 * drives each active slot unless Mario is in background mode.
 */
void partyMain(void) {
    s32 i;
    PartyEntry* party;
#if VERSION > VERSION_DEMO_US
    marioGetPtr();
#endif
    if (partyCountActive()) {
#if VERSION > VERSION_DEMO_US
        marioUsePartyMain();
#endif
        for (i = 0; i < 2; i++) {
            party = partyPtrTbl[i];
            if (party && (party->flags & PARTY_FLAG_IS_ACTIVE) && marioBgmodeChk() != 1) {
                driveParty(party);
            }
        }
    }
}

void partyPreDisp(PartyEntry* party);
void partyRideMain(PartyEntry* party);
s32 partyRideYoshiMain(PartyEntry* party);
void party_damage_return(PartyEntry* party);
s32 partyDokanInit(PartyEntry* party);
s32 partyDokanMain(PartyEntry* party);

/**
 * Runs one partner's frame by run mode (see partyChgRunMode):
 * her move, ability, appear, goodbye, pipe, ride and damage handlers, or frees her in mode 14.
 * A frozen partner only gets drawn. Afterwards updates dust, the off-screen flag and her display.
 */
#if VERSION <= VERSION_DEMO_US
static void driveParty(PartyEntry* party) {
#else
void driveParty(PartyEntry* party) {
#endif
    MarioWork* mario;
    f32 x, y, z;

    mario = party->playerPtr;
    party->camDir = mario->targetYaw;
    if (party->mutekiTimer > 0) {
        party->mutekiTimer--;
    }
    switch (party->runMode) {
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 14:
            break;
        default:
            if (!(party->flags & PARTY_FLAG_CTRL_OFF)) {
                break;
            }
            if (vivianGetStatus() != VIVIAN_STATUS_HIDDEN) {
                vivianShadow(party);
            }
            partyPreDisp(party);
            return;
    }
    if (mario->flags & MARIO_FLAG_BG_MODE) {
        party->flags |= PARTY_FLAG_BG_MODE;
    } else {
        party->flags &= ~PARTY_FLAG_BG_MODE;
    }
    party->hitobjStandOn = NULL;
    party->hitobjFront = NULL;
    party->hitobjHead = NULL;
    party->hitobjPush = NULL;
    party->hitobjAction = NULL;
    switch (party->runMode) {
        case 0:
            if ((party->flags & PARTY_FLAG_CTRL_OFF) && !(party->flags & PARTY_FLAG_RUN_OFF)) {
                break;
            }
            if (party->trigFlags & PARTY_TRIG_CHG_RUN_MODE) {
                party->trigFlags &= ~PARTY_TRIG_CHG_RUN_MODE;
                partyChgPoseId(party, PARTY_POSE_STAND);
                party->moveSpeed = 0.0f;
                party->flags &= ~PARTY_FLAG_IS_BEING_USED;
                partyChgMot(party, PARTY_MOTION_STAY);
            }
            party_dp = &partyDataTbl[party->currentMemberId];
            party_dp->move_func(party);
            break;
        case 1:
#if VERSION <= VERSION_DEMO_US
            if ((party->flags & PARTY_FLAG_CTRL_OFF) && !(party->flags & PARTY_FLAG_RUN_OFF))
                break;
            if (party->trigFlags & PARTY_TRIG_CHG_RUN_MODE) {
                party->trigFlags &= ~PARTY_TRIG_CHG_RUN_MODE;
                partyChgPoseId(party, PARTY_POSE_STAND);
                party->flags &= ~PARTY_FLAG_IS_BEING_USED;
#else
            if ((party->flags & PARTY_FLAG_CTRL_OFF) && !(party->flags & PARTY_FLAG_RUN_OFF)) {
                break;
#endif
            }
#if VERSION <= VERSION_DEMO_US
            if (!(party->flags & PARTY_FLAG_DISABLE_FRONT_WALL_CHK)) {
                partyChkWall(party);
            }
            partyChkGnd(party);
            if (distABf(party->position.x, party->position.z, mario->position.x, mario->position.z) > 31.0f) {
                partyChgRunMode(party, PARTY_RUN_MOVE);
            }
#else
            party_move_mode_wait(party);
#endif
            break;
        case 2:
            if ((party->flags & PARTY_FLAG_CTRL_OFF) && !(party->flags & PARTY_FLAG_RUN_OFF)) {
                break;
            }
            party_dp = &partyDataTbl[party->currentMemberId];
            party_dp->move_func(party);
            break;
        case 9:
            if ((party->flags & PARTY_FLAG_CTRL_OFF) && !(party->flags & PARTY_FLAG_RUN_OFF)) {
                break;
            }
            partyForceWalk(party);
            break;
        case 3:
            if ((party->flags & PARTY_FLAG_CTRL_OFF) && !(party->flags & PARTY_FLAG_RUN_OFF)) {
                break;
            }
            {
                PartyFunc fn;
                party_dp = &partyDataTbl[party->currentMemberId];
                fn = party_dp->use_func;
                party->flags |= PARTY_FLAG_IS_BEING_USED;
                fn(party);
            }
            break;
        case 4:
        case 5:
        case 6:
        case 7:
            partyHello(party);
            break;
        case 8:
            party_dp = &partyDataTbl[party->currentMemberId];
            party_dp->bye_func(party);
            break;
        case 10:
            if (party->trigFlags & PARTY_TRIG_CHG_RUN_MODE) {
                party->trigFlags &= ~PARTY_TRIG_CHG_RUN_MODE;
                partyDokanInit(party);
                party->flags &= ~PARTY_FLAG_IS_BEING_USED;
            }
            partyDokanMain(party);
            break;
        case 11:
            if ((party->flags & PARTY_FLAG_CTRL_OFF) && !(party->flags & PARTY_FLAG_RUN_OFF)) {
                break;
            }
            if (party->trigFlags & PARTY_TRIG_CHG_RUN_MODE) {
                party->trigFlags &= ~PARTY_TRIG_CHG_RUN_MODE;
                party->motionStep = 0;
                party->flags &= ~PARTY_FLAG_IS_BEING_USED;
#if VERSION > VERSION_DEMO_US
                partyForceSlitOff(party);
#endif
            }
            partyRideMain(party);
            break;
        case 12:
            if ((party->flags & PARTY_FLAG_CTRL_OFF) && !(party->flags & PARTY_FLAG_RUN_OFF)) {
                break;
            }
            if (party->trigFlags & PARTY_TRIG_CHG_RUN_MODE) {
                party->trigFlags &= ~PARTY_TRIG_CHG_RUN_MODE;
                party->motionStep = 0;
                party->flags &= ~PARTY_FLAG_IS_BEING_USED;
            }
            partyRideYoshiMain(party);
            break;
        case 13:
            if ((party->flags & PARTY_FLAG_CTRL_OFF) && !(party->flags & PARTY_FLAG_RUN_OFF)) {
                break;
            }
            party_damage_return(party);
            break;
        case 14:
            if (party->trigFlags & PARTY_TRIG_CHG_RUN_MODE) {
                party->trigFlags &= ~PARTY_TRIG_CHG_RUN_MODE;
            }
            mario->prevPartyMemberId[party->currentSlotId] = party->currentMemberId;
            mario->currPartySlotId[party->currentSlotId] = -1;
            partyPtrTbl[party->currentSlotId] = NULL;
            animPoseRelease(party->paperAnimGroup);
            __memFree(HEAP_DEFAULT, party);
            return;
    }
    party_kemuri(party);
    marioGetScreenPos(party->position, &x, &y, &z);
    if (marioChkInScreen(x, y)) {
        party->flags2 &= ~PARTY_DISP_FLAG_OUT_OF_SCREEN;
    } else {
        party->flags2 |= PARTY_DISP_FLAG_OUT_OF_SCREEN;
    }
    party->floorMove = (Vec){0.0f, 0.0f, 0.0f};
    partyPreDisp(party);
}

/**
 * Starts and runs the partner's appear (run modes 4-7):
 * modes 4 and 5 pop her out of Mario from scale 0, mode 7 starts later in the sequence, and if Mario is rolling,
 * flying or sailing she joins at the end.
 */
static inline void partyHello_inl(PartyEntry* party) {
    MarioWork* mario;

    if (party->trigFlags & PARTY_TRIG_CHG_RUN_MODE) {
        party->trigFlags &= ~PARTY_TRIG_CHG_RUN_MODE;
        mario = party->playerPtr;
        if (mario->currMotionId == MARIO_MOTION_ROLL || mario->currMotionId == MARIO_MOTION_PLANE || mario->currMotionId == MARIO_MOTION_SHIP) {
            party->motionStep = 30;
        } else if (party->runMode == 4 || party->runMode == 5) {
            party->flags2 |= PARTY_DISP_FLAG_NO_MAKE_DISP_DIR;
            party->scale = (Vec){0.0f, 0.0f, 0.0f};
            party->motionStep = 0;
            party->displayDirectionTarget = mario->displayDirection;
            party->displayDirection = mario->displayDirection;
            party->position = mario->position;
            party->trailPos = party->position;
        } else if (party->runMode == 7) {
            party->scale = (Vec){1.0f, 1.0f, 1.0f};
            party->motionStep = 20;
            party->displayDirectionTarget = mario->displayDirection;
            party->displayDirection = mario->displayDirection;
        } else {
            party->scale = (Vec){1.0f, 1.0f, 1.0f};
            party->motionStep = 10;
            party->displayDirectionTarget = mario->displayDirection;
            party->displayDirection = mario->displayDirection;
            party->position = mario->position;
            party->trailPos = party->position;
        }
        party->flags &= ~PARTY_FLAG_IS_BEING_USED;
    }
    partyHelloMain(party);
}

/**
 * Starts the main partner's field ability (run mode 3) when Mario presses the partner button:
 * only from standing, waiting or following. Stops Mario and returns TRUE if it started.
 */
BOOL partyDoWork(void) {
    PartyEntry* party = partyGetPtr(marioGetPtr()->currPartySlotId[0]);
#if VERSION > VERSION_DEMO_US
    if (!party) {
        return FALSE;
    }
#endif
    if (party->runMode != PARTY_RUN_MOVE_RESET && party->runMode != PARTY_RUN_WAIT && party->runMode != PARTY_RUN_MOVE) {
        return FALSE;
    }
    marioGetPtr()->baseSpeed = 0.0f;
    partyChgRunMode(party, PARTY_RUN_USE);
    return TRUE;
}

/** Runs the partner appear handler (partyHello_inl) for mario_party. */
void partyHello(PartyEntry* party) {
    partyHello_inl(party);
}


/**
 * Runs the partner's appear: step 0 places her at Mario's middle
 * and launches a 16-frame hop to her appear position while she grows from nothing (with the appear sound),
 * step 2 settles her facing left or right and switches to waiting.
 * Step 10 places her at once, 20 just stands her still, 30 puts her on Mario's ride. Returns 1 when done.
 */
s32 partyHelloMain(PartyEntry* party) {
    s32 ret = 0;
    u8* mario = (u8*)party->playerPtr;

    switch (party->motionStep) {
        case 0: {
                f32 x, y, z, h, dy;
#if VERSION <= VERSION_DEMO_US
                psndSFXOn_3D((const char*)PSND_SFX_PARTY_OUT1, &party->position);
#endif
                x = ((MarioWork*)mario)->position.x;
                y = ((MarioWork*)mario)->position.y;
                z = ((MarioWork*)mario)->position.z;
                party->position.x = x;
                party->position.y = y + 0.5f * ((MarioWork*)mario)->wPlayerCollisionBox.y;
                party->position.z = z;
                partyGetAppearPos(party, &party->moveTarget);
                party->moveFrames = 16;
                party->moveDistance = distABf(x, z, party->moveTarget.x, party->moveTarget.z);
                party->targetDir = angleABf(x, z, party->moveTarget.x, party->moveTarget.z);
                party->moveSpeed = party->moveDistance / (f32)party->moveFrames;
                h = 0.5f * (f32)party->moveFrames;
                dy = party->moveTarget.y - party->position.y;
                if (__fabsf(dy) < 1.0f) {
                    dy = 10.0f;
                } else if (dy >= 0.0f) {
                    dy += 0.5f * party->height;
                } else {
                    dy = 10.0f;
                }
                dy = (dy + dy) / (h * h - h);
                party->velY = dy * h;
                party->accelY = -dy;
                party->flags |= PARTY_FLAG_IS_JUMPING;
                party->motionStep = 1;
                break;
        }
        case 1:
            partyMoveNoHosei(party);
            if (party->velY <= 0.0f) {
                party->flags &= ~PARTY_FLAG_IS_JUMPING;
                party->accelY = -1.6f;
            }
            if (party->position.y < -2000.0f) {
                party->moveFrames = 0;
                party->motionStep = 2;
                break;
            }
            if (party->hitobjStandOn == NULL) {
                // FAKE: no-op (PartyEntry*) cast on the shrink-timer reads/stores of moveFrames; the plain field access reschedules the int->float conversion
                s32 n = 16 - ((PartyEntry*)party)->moveFrames;
                f32 v = (f32)n / 16.0f;
                Vec s;
                s = (Vec){0.0f, 0.0f, 0.0f};
                s.x = s.y = s.z = v;
                party->scale = s;
                if (--party->moveFrames <= 0) {
                    ((PartyEntry*)party)->moveFrames = 0;
                }
                break;
            }
            party->motionStep = 2;
        case 2: {
                f32 v = (f32)(16 - ((PartyEntry*)party)->moveFrames) / 16.0f;
                Vec s;
                s = (Vec){0.0f, 0.0f, 0.0f};
                s.x = s.y = s.z = v;
                party->scale = s;
                if (--party->moveFrames > 0) {
                    break;
                }
                ((PartyEntry*)party)->moveFrames = 0;
#if VERSION <= VERSION_DEMO_US
                party->moveDir = ((MarioWork*)mario)->moveDirection;
                party->targetDir = ((MarioWork*)mario)->directionView;
                party->displayDirectionTarget = ((MarioWork*)mario)->displayDirectionTarget;
                party->displayDirection = ((MarioWork*)mario)->displayDirection;
#else
                if (party->displayDirectionTarget >= 90.0f && party->displayDirectionTarget <= 270.0f) {
                    party->moveDir = partyToMovedir(party, 180.0f);
                    party->targetDir = party->moveDir;
                    party->displayDirectionTarget = party->displayDirection = 180.0f;
                } else {
                    party->moveDir = partyToMovedir(party, 0.0f);
                    party->targetDir = party->moveDir;
                    party->displayDirectionTarget = party->displayDirection = 0.0f;
                }
#endif
                partyClearJumpPara(party);
                partyChgPoseId(party, PARTY_POSE_STAND);
                party->flags2 &= ~PARTY_DISP_FLAG_NO_MAKE_DISP_DIR;
                ret = 1;
                party->runMode = PARTY_RUN_WAIT;
#if VERSION > VERSION_DEMO_US
                psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_PARTY_OUT1 | PSND_SFX_BYPASS_MUTE), &((MarioWork*)mario)->position);
#endif
                break;
        }
        case 10:
            partyGetAppearPos2(party, &party->position);
            party->trailPos = party->position;
            partyChgPoseId(party, PARTY_POSE_STAND);
            party->runMode = PARTY_RUN_WAIT;
            ret = 1;
            break;
        case 20:
            partyChgPoseId(party, PARTY_POSE_STAND);
            party->runMode = PARTY_RUN_MOVE_RESET;
            ret = 1;
            break;
        case 30:
            allPartyForceRideOn();
            ret = 1;
            break;
    }
    return ret;
}

/**
 * Starts the partner's goodbye (run mode 8): runs the member's init function,
 * has Vivian let go of Mario or Mario get off Yoshi first if her ability is active,
 * and skips the animation when Mario is rolling, flying or sailing.
 */
s32 partyGoodbyeInit(PartyEntry* party) {
    MarioWork* mario;
    party->motionStep = 10;
    party_dp = &partyDataTbl[party->currentMemberId];
    if (party_dp->init_func) {
        party_dp->init_func(party);
    }
    if (party->flags & PARTY_FLAG_IS_BEING_USED) {
        if (party->currentMemberId == PARTY_MEMBER_VIVIAN) {
            vivianUnhold(party);
            party->motionStep = 0;
        }
        if (party->currentMemberId == PARTY_MEMBER_YOSHI) {
            marioGetoffYoshi();
            party->motionStep = 0;
        }
    }
    mario = party->playerPtr;
    if (mario->currMotionId == MARIO_MOTION_ROLL || mario->currMotionId == MARIO_MOTION_PLANE || mario->currMotionId == MARIO_MOTION_SHIP) {
        party->motionStep = 20;
    }
    return 0;
}

/**
 * Runs the partner's goodbye: finishes Vivian's or Yoshi's ability first,
 * then she hops back into Mario over 15 frames, spinning and shrinking, and is freed (run mode 14).
 * Returns 1 when done.
 */
s32 partyGoodbyeMain(PartyEntry* party) {
    MarioWork* mario = marioGetPtr();
    f32 tx, ty, tz, px, py, pz;

    switch (party->motionStep) {
        case 0:
            party_dp = &partyDataTbl[party->currentMemberId];
            party_dp->use_func(party);
            if (party->currentMemberId == PARTY_MEMBER_VIVIAN) {
                if (vivianGetStatus() == VIVIAN_STATUS_INACTIVE) {
                    party->motionStep = 10;
                    party->runMode = PARTY_RUN_GOODBYE;
                }
            } else if (party->currentMemberId == PARTY_MEMBER_YOSHI) {
                if (yoshiGetStatus() == YOSHI_STATUS_INACTIVE) {
                    party->motionStep = 10;
                    party->runMode = PARTY_RUN_GOODBYE;
                }
            } else {
                party->motionStep = 10;
                party->runMode = PARTY_RUN_GOODBYE;
            }
            break;
        case 10:
#if VERSION <= VERSION_DEMO_US
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_PARTY_IN1), &party->position);
#else
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_PARTY_IN1 | PSND_SFX_BYPASS_MUTE), &mario->position);
#endif
            tx = party->moveTarget.x = mario->position.x;
            ty = party->moveTarget.y = 0.5f * mario->wPlayerCollisionBox.y + mario->position.y;
            tz = party->moveTarget.z = mario->position.z;
            px = party->position.x;
            pz = party->position.z;
            py = party->position.y;
            party->moveSpeed = 4.0f;
            party->gravity = 1.6f;
            party->moveDistance = distABf(px, pz, tx, tz);
            party->targetDir = angleABf(px, pz, tx, tz);
            ty -= py;
            party->moveFrames = 15;
            party->moveSpeed = party->moveDistance / (f32)party->moveFrames;
            party->velY = (0.5f * ((f32)party->moveFrames * (party->gravity * (f32)party->moveFrames)) + ty) / (f32)party->moveFrames;
            partyChgPoseId(party, PARTY_POSE_JUMP);
            party->motionStep = 11;
            break;
        case 11: {
                f32 t;
                party->velY -= party->gravity;
                party->position.y += party->velY;
                movePos(&party->position.x, &party->position.z, party->moveSpeed, party->targetDir);
                party->displayDirection = revise360(50.0f + party->displayDirection);
                t = party->moveFrames;
                if (t > 10.0f) {
                    t = 10.0f;
                }
                party->scale.x = t / 10.0f;
                party->scale.y = party->scale.x;
                party->scale.z = party->scale.x;
                if (--party->moveFrames < 0) {
                    party->motionStep = 12;
                }
                break;
        }
        case 12:
            partyChgPoseId(party, PARTY_POSE_STAND);
            party->velY = 0.0f;
            party->position.y = party->moveTarget.y;
            partyChgRunMode(party, PARTY_RUN_DELETE);
            return 1;
        case 20:
            partyChgRunMode(party, PARTY_RUN_DELETE);
            return 1;
    }
    return 0;
}


/** Resets the partner's scale to 1 (leaving the appear mode). */
static inline void partyChgRunM(PartyEntry* party) {
    party->scale = (Vec){1.0f, 1.0f, 1.0f};
}

/**
 * Puts the partner in the pipe with Mario (run mode 10):
 * she takes Mario's position facing backwards and turns into the spinning pipe paper,
 * p_dokan_y for the vertical pipe kinds (Mario's dokanType 0-3) and p_dokan_x for the sideways ones (4, 5).
 * Ends Vivian's tail effect.
 */
s32 partyDokanInit(PartyEntry* party) {
    MarioWork* mario = party->playerPtr;

#if VERSION >= VERSION_US
    party->displayDirectionTarget = revise360(180.0f + mario->displayDirection);
    party->displayDirection = mario->displayDirection;
    party->moveDir = mario->moveDirection;
    party->position = mario->position;
#endif
    party->flags2 |= PARTY_DISP_FLAG_NO_MAKE_DISP_DIR;
    switch (mario->dokanType) {
        case 2:
        case 3:
            partyPaperOn(party, "p_dokan_y");
#if VERSION >= VERSION_US
            partyChgPaper(party, "PM_D_1A");
            partyChgPoseId(party, PARTY_POSE_PIPE);
            party->spinSpeed = 30.0f;
            party->motionStep = 0;
            party->flags2 |= PARTY_DISP_FLAG_NO_FLIP;
            party->flags2 |= PARTY_DISP_FLAG_NO_MAKE_DISP_DIR;
            animPoseSetMaterialLightFlagOff(party->paperAnimGroup, MATERIAL_LIGHT_PAPER);
#else
            party->spinSpeed = 30.0f;
            party->motionStep = 0;
#endif
            break;
        case 0:
            party->scale = (Vec){0.0f, 0.0f, 0.0f};
#if VERSION >= VERSION_US
            party->flags2 |= PARTY_DISP_FLAG_NO_FLIP;
            party->flags2 |= PARTY_DISP_FLAG_NO_MAKE_DISP_DIR;
            animPoseSetMaterialLightFlagOff(party->paperAnimGroup, MATERIAL_LIGHT_PAPER);
        case 1:
            partyPaperOn(party, "p_dokan_y");
            partyChgPaper(party, "PM_D_1A");
            partyChgPoseId(party, PARTY_POSE_PIPE);
            party->spinSpeed = 30.0f;
            party->motionStep = 0;
            party->position = mario->position;
            party->scale = (Vec){1.0f, 1.0f, 1.0f};
            party->flags2 |= PARTY_DISP_FLAG_NO_FLIP;
            party->flags2 |= PARTY_DISP_FLAG_NO_MAKE_DISP_DIR;
            animPoseSetMaterialLightFlagOff(party->paperAnimGroup, MATERIAL_LIGHT_PAPER);
#else
        case 1:
            partyPaperOn(party, "p_dokan_y");
            party->spinSpeed = 30.0f;
            party->motionStep = 0;
#endif
            break;
        case 5:
            partyPaperOn(party, "p_dokan_x");
#if VERSION >= VERSION_US
            partyChgPaper(party, "PM_D_1A");
            partyChgPoseId(party, PARTY_POSE_PIPE);
#endif
            party->spinSpeed = 26.0f;
            party->motionStep = 10;
            party->rotOfsY = 10.0f;
#if VERSION <= VERSION_DEMO_US
            break;
        case 4:
            partyPaperOn(party, "p_dokan_x");
            party->spinSpeed = -26.0f;
            party->motionStep = 10;
            party->rotOfsY = 10.0f;
            party->rot.x = revise360(180.0f + mario->rot.x);
            break;
    }
    if (!(party->flags2 & PARTY_DISP_FLAG_POSE_LOCK)) {
        if (party->paperAnimName == NULL || strcmp(party->paperAnimName, "PM_D_1A")) {
            party->paperAnimName = "PM_D_1A";
            party->poseTime = 0;
            party->trigFlags |= PARTY_TRIG_CHG_PAPER_ANIM;
        }
    }
    partyChgPoseId(party, PARTY_POSE_PIPE);
    party->flags2 |= PARTY_DISP_FLAG_NO_FLIP;
    party->flags2 |= PARTY_DISP_FLAG_NO_MAKE_DISP_DIR;
    animPoseSetMaterialLightFlagOff(party->paperAnimGroup, MATERIAL_LIGHT_PAPER);
    party->displayDirectionTarget = revise360(180.0f + mario->displayDirection);
    party->displayDirection = mario->displayDirection;
    party->moveDir = mario->moveDirection;
    party->position = mario->position;
#else
#if VERSION >= VERSION_US
            party->flags2 |= PARTY_DISP_FLAG_NO_FLIP;
            party->flags2 |= PARTY_DISP_FLAG_NO_MAKE_DISP_DIR;
            animPoseSetMaterialLightFlagOff(party->paperAnimGroup, MATERIAL_LIGHT_PAPER);
            break;
        case 4:
            party->displayDirectionTarget = mario->displayDirection;
            partyPaperOn(party, "p_dokan_x");
            partyChgPaper(party, "PM_D_1A");
            partyChgPoseId(party, PARTY_POSE_PIPE);
            party->motionStep = 10;
            party->rotOfsY = 10.0f;
            if (mario->displayDirectionTarget >= 90.0f && mario->displayDirectionTarget <= 270.0f) {
                party->rot.x = revise360(180.0f + mario->rot.x);
                party->displayDirectionTarget = mario->displayDirectionTarget;
                party->displayDirection = mario->displayDirection;
                party->spinSpeed = 30.0f;
            } else {
                party->rot.x = revise360(mario->rot.x - 180.0f);
                party->displayDirectionTarget = mario->displayDirectionTarget;
                party->displayDirection = mario->displayDirection;
                party->flags2 |= PARTY_DISP_FLAG_NO_FLIP;
                party->spinSpeed = -30.0f;
                party->position = mario->position;
                party->dispOfs.z = -4.0f;
            }
            party->flags2 |= PARTY_DISP_FLAG_NO_MAKE_DISP_DIR;
            animPoseSetMaterialLightFlagOff(party->paperAnimGroup, MATERIAL_LIGHT_PAPER);
            break;
    }
#else
        break;
        case 4:
            partyPaperOn(party, "p_dokan_x");
            party->spinSpeed = -26.0f;
            party->motionStep = 10;
            party->rotOfsY = 10.0f;
            party->rot.x = revise360(180.0f + mario->rot.x);
            break;
    }
    if (!(party->flags2 & PARTY_DISP_FLAG_POSE_LOCK)) {
        if (party->paperAnimName == NULL || strcmp(party->paperAnimName, "PM_D_1A")) {
            party->paperAnimName = "PM_D_1A";
            party->poseTime = 0;
            party->trigFlags |= PARTY_TRIG_CHG_PAPER_ANIM;
        }
    }
    partyChgPoseId(party, PARTY_POSE_PIPE);
    party->flags2 |= PARTY_DISP_FLAG_NO_FLIP;
    party->flags2 |= PARTY_DISP_FLAG_NO_MAKE_DISP_DIR;
    animPoseSetMaterialLightFlagOff(party->paperAnimGroup, MATERIAL_LIGHT_PAPER);
    party->displayDirectionTarget = revise360(180.0f + mario->displayDirection);
    party->displayDirection = mario->displayDirection;
    party->moveDir = mario->moveDirection;
    party->position = mario->position;
#endif
    partyVivianTailStop(party);
#endif
    return 0;
}

/**
 * The partner's frames in the pipe (run mode 10):
 * she spins at Mario's position, growing back to full size on the vertical kinds;
 * once partyDokanEnd sets step 100 she plays the PM_D_1B exit paper for 18 frames,
 * then returns to normal and waits beside Mario, offset so two partners do not overlap.
 */
s32 partyDokanMain(PartyEntry* party) {
    MarioWork* mario = party->playerPtr;
    s32 n;
#if VERSION >= VERSION_US
    f32 s;
    f32 c;
#endif

    switch (party->motionStep) {
        case 0:
            party->position = mario->position;
            party->displayDirection = revise360(party->displayDirection + party->spinSpeed);
#if VERSION < VERSION_US
            party->scale.x += 0.05f;
            if (party->scale.x >= 1.0f) {
                party->scale.x = 1.0f;
            }
            party->scale.y = party->scale.x;
            party->scale.z = party->scale.x;
#endif
            break;
        case 10:
            party->rot.x += party->spinSpeed;
#if VERSION >= VERSION_US
            party->position = mario->position;
            sincosf(&s, &c, mario->targetYaw);
            party->position.x = mario->position.x + -5.0f * s;
            party->position.z = mario->position.z + 5.0f * c;
#endif
            break;
        case 100:
            switch (party->dirKind) {
                case DOKAN_TATE_DOWNIN:
                case DOKAN_TATE_UPIN:
                    partyChgPaper(party, "PM_D_1B");
                    party->moveFrames = 18;
                    party->motionStep = 101;
#if VERSION >= VERSION_US
                    party->flags2 |= PARTY_DISP_FLAG_NO_FLIP;
                    party->flags2 |= PARTY_DISP_FLAG_NO_MAKE_DISP_DIR;
                    animPoseSetMaterialLightFlagOff(party->paperAnimGroup, MATERIAL_LIGHT_PAPER);
                    break;
                case DOKAN_YOKO_IN:
                    party->rot.x = 0.0f;
                    party->flags2 &= ~PARTY_DISP_FLAG_NO_FLIP;
                    party->displayDirectionTarget = party->displayDirection;
#else
                    break;
                case DOKAN_YOKO_IN:
#endif
                    partyChgPaper(party, "PM_D_1B");
                    party->moveFrames = 18;
                    party->motionStep = 101;
                    break;
            }
            break;
        case 101:
            switch (party->dirKind) {
                case DOKAN_TATE_DOWNIN:
                case DOKAN_TATE_UPIN:
#if VERSION >= VERSION_US
                    party->position.z -= 0.2f;
#else
                    party->position.z += -0.2f;
#endif
                    break;
            }
            if (--party->moveFrames <= 0) {
                party->motionStep = 102;
            }
            break;
        case 102:
            party->scale = (Vec){1.0f, 1.0f, 1.0f};
            party->rot = (Vec){0.0f, 0.0f, 0.0f};
            party->rotOfs = (Vec){0.0f, 0.0f, 0.0f};
            party->dispOfs = (Vec){0.0f, 0.0f, 0.0f};
            party->displayDirectionTarget = mario->displayDirectionTarget;
            party->displayDirection = party->displayDirectionTarget;
            party->targetDir = mario->directionView;
            party->moveDir = mario->directionView;
            partyPaperOff(party);
#if VERSION <= VERSION_DEMO_US
            party->flags2 &= ~PARTY_DISP_FLAG_NO_FLIP;
            party->flags2 &= ~PARTY_DISP_FLAG_NO_MAKE_DISP_DIR;
            animPoseSetMaterialLightFlagOn(party->paperAnimGroup, MATERIAL_LIGHT_PAPER);
#else
            animPoseSetMaterialLightFlagOn(party->paperAnimGroup, MATERIAL_LIGHT_PAPER);
            party->flags2 &= ~PARTY_DISP_FLAG_NO_FLIP;
            party->flags2 &= ~PARTY_DISP_FLAG_NO_MAKE_DISP_DIR;
#endif
            party->flags &= ~PARTY_FLAG_DISABLE_FLOORS;
            party->flags &= ~PARTY_FLAG_FORCE_FALL;
            partyShadowOn(party);
#if VERSION <= VERSION_DEMO_US
            party->trailPos = party->position;
            partyChgRunMode(party, PARTY_RUN_WAIT);
            if ((s8)party->dirKind == DOKAN_YOKO_IN) {
                party->position.z += -4.0f;
            }
            if (party->currentSlotId == 1) {
                party->position.z += -4.0f;
#else
            partyVivianTailStart(party);
            party->trailPos = party->position;
            partyChgRunMode(party, PARTY_RUN_WAIT);
            n = 0;
            if (partyGetPtr(marioGetPartyId())) {
                n = 1;
            }
            if (partyGetPtr(marioGetExtraPartyId())) {
                n++;
            }
            if (party->dirKind == DOKAN_YOKO_IN) {
                if (n == 1) {
                    party->position.z += -4.0f;
                } else if (!(party->flags & PARTY_FLAG_IS_PARTNER)) {
                    party->position.z += -4.0f;
                    party->position.x += 1.5f;
                } else {
                    party->position.z += -3.0f;
                    party->position.x += -1.5f;
                }
            }
            if (party->dirKind == DOKAN_TATE_UPIN) {
                if (n == 1) {
                    party->position.z += -4.0f;
                } else if (!(party->flags & PARTY_FLAG_IS_PARTNER)) {
                    party->position.z += -4.0f;
                    party->position.x += 1.5f;
                } else {
                    party->position.z += -3.0f;
                    party->position.x += -1.5f;
                }
#endif
            }
            break;
    }
    return 0;
}

/**
 * Called by mot_dokan as Mario leaves the pipe:
 * makes a partner in pipe mode play her exit (step 100) for pipe kind dir and shows her shadow again.
 */
void partyDokanEnd(s32 id, s32 dir) {
    PartyEntry* party = partyGetPtr(id);
    if (party && party->runMode == PARTY_RUN_PIPE) {
        party->motionStep = 100;
        party->dirKind = dir;
#if VERSION > VERSION_DEMO_US
        party->flags2 &= ~PARTY_DISP_FLAG_NO_FLIP;
        party->flags2 &= ~PARTY_DISP_FLAG_NO_MAKE_DISP_DIR;
        party->flags2 &= ~(PARTY_DISP_FLAG_TURN_LEFT_UNUSED | PARTY_DISP_FLAG_TURN_RIGHT_UNUSED);
        party->flags &= ~PARTY_FLAG_DISABLE_FLOORS;
        party->flags &= ~PARTY_FLAG_FORCE_FALL;
#endif
        partyShadowOn(party);
#if VERSION <= VERSION_DEMO_US
        party->flags2 |= PARTY_DISP_FLAG_DISP_DIR_SNAP;
        party->flags2 &= ~(PARTY_DISP_FLAG_TURN_LEFT_UNUSED | PARTY_DISP_FLAG_TURN_RIGHT_UNUSED);
#else
        party->trailPos = party->position;
        partyVivianTailStart(party);
#endif
    }
}

/** Puts both partners on Mario's plane-mode flight (run mode 11), cancelling Bobbery's ability first (mot_plane). */
void allPartyRidePlane(void) {
    PartyEntry* party = partyGetPtr(marioGetPartyId());
    if (party) {
        bomhei_use_cancel();
        party->flags |= PARTY_FLAG_IS_RIDING_PLANE;
        partyChgRunMode(party, PARTY_RUN_RIDE);
    }
    party = partyPtrTbl[1];
    if (party) {
        party->flags |= PARTY_FLAG_IS_RIDING_PLANE;
        partyChgRunMode(party, PARTY_RUN_RIDE);
    }
}

/** Puts both partners on Mario's boat-mode ride (run mode 11), cancelling Bobbery's ability first (mot_ship). */
void allPartyRideShip(void) {
    PartyEntry* party = partyGetPtr(marioGetPartyId());
    if (party) {
        bomhei_use_cancel();
        party->flags |= PARTY_FLAG_IS_RIDING_SHIP;
        partyChgRunMode(party, PARTY_RUN_RIDE);
    }
    party = partyPtrTbl[1];
    if (party) {
        party->flags |= PARTY_FLAG_IS_RIDING_SHIP;
        partyChgRunMode(party, PARTY_RUN_RIDE);
    }
}

#if VERSION <= VERSION_DEMO_US
void allPartyRideJabara(void) {
    PartyEntry* party = partyGetPtr(marioGetPartyId());
    if (party) {
        bomhei_use_cancel();
        party->flags |= PARTY_FLAG_IS_RIDING_JABARA;
        partyChgRunMode(party, PARTY_RUN_RIDE);
    }
    party = partyPtrTbl[1];
    if (party) {
        party->flags |= PARTY_FLAG_IS_RIDING_JABARA;
        partyChgRunMode(party, PARTY_RUN_RIDE);
    }
}
#endif

/**
 * Where the partner rides with Mario: behind him on Yoshi (the follower behind the main partner),
 * above him in plane mode, in front of the boat in boat mode (not in the demos), otherwise 20 above him.
 */
#if VERSION <= VERSION_DEMO_US
static void getRidePos(PartyEntry* party, Vec* pos) {
#else
void getRidePos(PartyEntry* party, Vec* pos) {
#endif
    MarioWork* mario = party->playerPtr;
    PartyEntry* ride;
    f32 angle;
    f32 s;
    f32 c;

    if (mario->currMotionId == MARIO_MOTION_YOSHI) {
        angle = 3.1415927f * toMovedir(mario->displayDirectionTarget) / 180.0f;
        if (yoshiGetStatus() != YOSHI_STATUS_FLUTTER) {
            pos->x = mario->position.x - 10.0f * (f32)sin(angle);
            pos->z = mario->position.z - 10.0f * (f32)cos(angle);
            pos->y = 20.0f + mario->position.y;
        } else {
            pos->x = mario->position.x - 20.0f * (f32)sin(angle);
            pos->z = mario->position.z - 20.0f * (f32)cos(angle);
            pos->y = 16.0f + mario->position.y;
        }
        if (party->currentSlotId) {
            ride = partyGetPtr(marioGetPartyId());
            sincosf(&s, &c, toMovedir2(mario->displayDirectionTarget, 10.0f));
            if (yoshiGetStatus() != YOSHI_STATUS_FLUTTER) {
                pos->x = ride->position.x - 12.0f * s;
                pos->z = ride->position.z - 12.0f * c;
                pos->y = 20.0f + ride->position.y;
            } else {
                pos->x = ride->position.x - 22.0f * s;
                pos->z = ride->position.z - 22.0f * c;
                pos->y = 16.0f + ride->position.y;
            }
        }
    } else if (mario->currMotionId == MARIO_MOTION_PLANE) {
        pos->x = mario->position.x;
        pos->z = mario->position.z;
        pos->y = 40.0f + mario->position.y;
#if VERSION > VERSION_DEMO_US
    } else if (mario->currMotionId == MARIO_MOTION_SHIP) {
        sincosf(&s, &c, toMovedirSimple(marioShipGetDir() ? 0.0f : 180.0f));
        pos->x = mario->position.x + 22.0f * s;
        pos->z = mario->position.z + 22.0f * c;
        pos->y = mario->position.y;
#endif
    } else {
        pos->x = mario->position.x;
        pos->z = mario->position.z;
        pos->y = 20.0f + mario->position.y;
    }
}

#pragma dont_inline on
/**
 * Puts both partners straight into the riding-along state (run mode 11, step 11) hidden and without shadow,
 * e.g. when Mario is already rolling, flying or sailing as they appear.
 */
void allPartyForceRideOn(void) {
    PartyEntry* party = partyGetPtr(marioGetPartyId());
    if (party) {
        partyForceChgRunMode(party, 11, 11);
#if VERSION > VERSION_DEMO_US
        party->flags2 |= PARTY_DISP_FLAG_HIDE;
#endif
        partyShadowOff(party);
    }
    party = partyGetPtr(marioGetExtraPartyId());
    if (party) {
        partyForceChgRunMode(party, 11, 11);
#if VERSION > VERSION_DEMO_US
        party->flags2 |= PARTY_DISP_FLAG_HIDE;
#endif
        partyShadowOff(party);
    }
}
#pragma dont_inline reset

/**
 * Puts both partners in the ride mode (11) to hop onto Mario (mot_roll for the rolling paper mode,
 * and the retail bellows motion).
 */
void L_allPartyRideOn(void) {
    PartyEntry* party = partyGetPtr(marioGetPartyId());
    if (party) {
        partyChgRunMode(party, PARTY_RUN_RIDE);
    }
    party = partyGetPtr(marioGetExtraPartyId());
    if (party) {
        partyChgRunMode(party, PARTY_RUN_RIDE);
    }
}

/**
 * Makes both riding partners hop off Mario to their appear position (step 100) when he leaves the rolling,
 * bellows or boat mode.
 * In the US and PAL releases, a partner that has not started riding yet just goes back to waiting.
 */
void allPartyRideOff(void) {
    PartyEntry* party = partyGetPtr(marioGetPartyId());
    if (party && party->runMode == PARTY_RUN_RIDE) {
#if VERSION >= VERSION_US
        if (!(party->trigFlags & PARTY_TRIG_CHG_RUN_MODE)) {
            party->motionStep = 100;
        } else {
            party->runMode = PARTY_RUN_WAIT;
        }
#else
        party->motionStep = 100;
#endif
    }
    party = partyGetPtr(marioGetExtraPartyId());
    if (party && party->runMode == PARTY_RUN_RIDE) {
#if VERSION >= VERSION_US
        if (!(party->trigFlags & PARTY_TRIG_CHG_RUN_MODE)) {
            party->motionStep = 100;
        } else {
            party->runMode = PARTY_RUN_WAIT;
        }
#else
        party->motionStep = 100;
#endif
    }
}

/**
 * Like allPartyRideOff but the partners land at the partyGetAppearPos2 spot (step 101);
 * used when Mario lands from plane mode.
 */
void allPartyRideOff2(void) {
    PartyEntry* party = partyGetPtr(marioGetPartyId());
    if (party && party->runMode == PARTY_RUN_RIDE) {
#if VERSION >= VERSION_US
        if (!(party->trigFlags & PARTY_TRIG_CHG_RUN_MODE)) {
            party->motionStep = 101;
        } else {
            party->runMode = PARTY_RUN_WAIT;
        }
#else
        party->motionStep = 101;
#endif
    }
    party = partyGetPtr(marioGetExtraPartyId());
    if (party && party->runMode == PARTY_RUN_RIDE) {
#if VERSION >= VERSION_US
        if (!(party->trigFlags & PARTY_TRIG_CHG_RUN_MODE)) {
            party->motionStep = 101;
        } else {
            party->runMode = PARTY_RUN_WAIT;
        }
#else
        party->motionStep = 101;
#endif
    }
}

/**
 * Ride state of a partner: 0 not riding, 1 hopping on, 2 riding along.
 * evt_mario waits on it before and after Mario's paper modes.
 */
s32 partyRideChk(PartyEntry* party) {
    s32 ret = 0;
    if (party == NULL) {
        return 0;
    }
    if (party->runMode == PARTY_RUN_RIDE) {
        ret = 1;
        if (party->motionStep >= 2) {
            ret = 2;
        }
    }
    return ret;
}

/**
 * The partner's ride mode (11): hops onto Mario over 30 frames, spinning and shrinking (steps 0-2),
 * then rides along hidden at his position (10, 11).
 * Steps 20-22 (not entered from C) throw her up and let her fall, 200 (also not entered) drops her at once;
 * 100/101 hop her back out next to Mario and return her to waiting.
 */
void partyRideMain(PartyEntry* party) {
    Vec pos;
    f32 shrink; // FAKE: one function-scope temp for (f32)(16 - timer), shared by cases 110 and 111 (see docs/FAKES.md)
    Vec s;
    f32 dy0;
    u8* mario = (u8*)party->playerPtr;
    switch (party->motionStep) {
        case 0:
            getRidePos(party, &pos);
            party->moveTarget = pos;
            party->moveDistance = distABf(party->position.x, party->position.z, party->moveTarget.x, party->moveTarget.z);
            party->targetDir = angleABf(party->position.x, party->position.z, party->moveTarget.x, party->moveTarget.z);
            party->moveFrames = 30;
            party->moveSpeed = party->moveDistance / (f32)party->moveFrames;
            dy0 = party->moveTarget.y - party->position.y;
            party->gravity = 0.4f;
            party->velY = (dy0 + 0.5f * ((f32)party->moveFrames * (party->gravity * (f32)party->moveFrames))) / (f32)party->moveFrames;
            party->spinSpeed = 1.0f;
            party->flags2 |= PARTY_DISP_FLAG_NO_MAKE_DISP_DIR;
            partyChgPoseId(party, PARTY_POSE_JUMP);
            party->motionStep = 1;
            break;

        case 1: {
                f32 t;
                party->velY = party->velY - party->gravity;
                party->position.y = party->position.y + party->velY;
                movePos(&party->position.x, &party->position.z, party->moveSpeed, party->targetDir);
                party->displayDirection = revise360(party->displayDirection + party->spinSpeed);
                party->spinSpeed += 2.0f;
                if (party->spinSpeed >= 30.0f) {
                    party->spinSpeed = 30.0f;
                }
                t = (f32)party->moveFrames;
                if (t > 10.0f) {
                    t = 10.0f;
                }
                party->scale.x = t / 10.0f;
                party->scale.y = party->scale.x;
                party->scale.z = party->scale.x;
                if (--party->moveFrames < 0) {
                    party->motionStep = 2;
                }
                break;
            }

        case 2: {
                partyChgPoseId(party, PARTY_POSE_STAND);
                party->velY = 0.0f;
                party->position.y = party->moveTarget.y;
                party->scale = (Vec){0.0f, 0.0f, 0.0f};
#if VERSION > VERSION_DEMO_US
                party->flags2 |= PARTY_DISP_FLAG_HIDE;
#endif
                partyShadowOff(party);
                party->motionStep = 10;
                break;
            }

        case 10:
            party->position = ((MarioWork*)mario)->position;
            party->displayDirectionTarget = ((MarioWork*)mario)->displayDirectionTarget;
            party->moveDir = toMovedir(party->displayDirectionTarget);
            break;

        case 11: {
                partyChgPoseId(party, PARTY_POSE_STAND);
#if VERSION <= VERSION_DEMO_US
                party->velY = 0.0f;
                party->scale = (Vec){0.0f, 0.0f, 0.0f};
#else
                party->flags2 |= PARTY_DISP_FLAG_HIDE;
#endif
                partyShadowOff(party);
                getRidePos(party, &pos);
#if VERSION > VERSION_DEMO_US
                party->scale = (Vec){0.0f, 0.0f, 0.0f};
#endif
                party->position = pos;
#if VERSION > VERSION_DEMO_US
                party->velY = 0.0f;
#endif
                party->displayDirectionTarget = party->playerPtr->displayDirectionTarget;
                party->moveDir = toMovedir(party->displayDirectionTarget);
                break;
            }

        case 20:
            party->motionStep = 21;
            party->moveFrames = 16;
            party->velY = 16.0f;
            party->work.common.accelY = 0.5f;

        case 21:
            party->position.y = party->position.y + party->velY;
            party->velY = party->velY + party->work.common.accelY;
            if (--party->moveFrames > 0) {
                break;
            }
            party->motionStep = 22;
            party->flags |= PARTY_FLAG_FORCE_FALL;
            break;

        case 22:
            break;

        case 200:
            party->flags &= ~PARTY_RIDE_CLEAR_MASK;
            party->dispOfs.y = 0.0f;
            party->scale.x = 1.0f;
            party->scale.y = party->scale.x;
            party->scale.z = party->scale.x;
            partyShadowOn(party);
            partyChgRunMode(party, PARTY_RUN_WAIT);
            break;

        case 100:

        case 101: {
                f32 x;
                f32 y;
                f32 z;
                f32 h;
                f32 dy;
#if VERSION > VERSION_DEMO_US
                party->flags2 &= ~PARTY_DISP_FLAG_HIDE;
#endif
                x = ((MarioWork*)mario)->position.x;
                y = ((MarioWork*)mario)->position.y;
                z = ((MarioWork*)mario)->position.z;
                party->position.x = x;
                party->position.y = y + 0.5f * ((MarioWork*)mario)->wPlayerCollisionBox.y;
                party->position.z = z;
                if (party->motionStep == 100) {
                    partyGetAppearPos(party, &party->moveTarget);
                } else {
                    partyGetAppearPos2(party, &party->moveTarget);
                }
                party->displayDirectionTarget = ((MarioWork*)mario)->displayDirectionTarget;
                party->moveFrames = 16;
                party->moveDistance = distABf(x, z, party->moveTarget.x, party->moveTarget.z);
                party->targetDir = angleABf(x, z, party->moveTarget.x, party->moveTarget.z);
                party->moveSpeed = party->moveDistance / (f32)party->moveFrames;
                h = 0.5f * (f32)party->moveFrames;
                dy = party->moveTarget.y - party->position.y;
                if (__fabsf(dy) < 1.0f) {
                    dy = 10.0f;
                } else if (dy >= 0.0f) {
                    dy += 0.5f * party->height;
                } else {
                    dy = 10.0f;
                }
                dy = (dy + dy) / (h * h - h);
                party->velY = dy * h;
                party->accelY = -dy;
                party->flags |= PARTY_FLAG_IS_JUMPING;
#if VERSION > VERSION_DEMO_US
                party->flags2 &= ~PARTY_DISP_FLAG_HIDE;
#endif
                partyShadowOn(party);
                party->motionStep = 110;
                break;
            }

        case 110:
            partyMoveNoHosei(party);
            if (party->velY <= 0.0f) {
                party->flags &= ~PARTY_FLAG_IS_JUMPING;
                party->accelY = -1.6f;
            }
            if (party->position.y < -2000.0f) {
                party->moveFrames = 0;
                party->motionStep = 111;
                break;
            }
            if (party->hitobjStandOn == NULL) {
                // FAKE: no-op (PartyEntry*) cast on the shrink-timer reads/stores of moveFrames; the plain field access reschedules the int->float conversion
                shrink = (f32)(16 - ((PartyEntry*)party)->moveFrames);
                s = (Vec){0.0f, 0.0f, 0.0f};
                s.x = s.y = s.z = shrink / 16.0f;
                party->scale = s;
                if (--party->moveFrames <= 0) {
                    ((PartyEntry*)party)->moveFrames = 0;
                }
                break;
            }
            party->motionStep = 111;

        case 111:
            shrink = (f32)(16 - ((PartyEntry*)party)->moveFrames);
            {
                s = (Vec){0.0f, 0.0f, 0.0f};
                s.x = s.y = s.z = shrink / 16.0f;
                party->scale = s;
                if (--party->moveFrames > 0) {
                    break;
                }
#if VERSION <= VERSION_DEMO_US
                (((PartyEntry*)party)->moveFrames) = 0;
                party->moveDir = ((MarioWork*)mario)->moveDirection;
                party->targetDir = ((MarioWork*)mario)->directionView;
#else
                ((PartyEntry*)party)->moveFrames = 0;
#endif
                party->displayDirectionTarget = ((MarioWork*)mario)->displayDirectionTarget;
#if VERSION > VERSION_DEMO_US
                party->displayDirection = ((MarioWork*)mario)->displayDirectionTarget;
                party->moveDir = toMovedirSimple(party->displayDirection);
                party->targetDir = party->moveDir;
#endif
                partyClearJumpPara(party);
                partyChgPoseId(party, PARTY_POSE_STAND);
                party->flags2 &= ~PARTY_DISP_FLAG_NO_MAKE_DISP_DIR;
                party->flags &= ~PARTY_RIDE_CLEAR_MASK;
                party->dispOfs.y = 0.0f;
                party->scale.x = 1.0f;
                party->scale.y = party->scale.x;
                party->scale.z = party->scale.x;
#if VERSION > VERSION_DEMO_US
                party->flags2 &= ~PARTY_DISP_FLAG_HIDE;
#endif
                partyShadowOn(party);
                partyChgRunMode(party, PARTY_RUN_WAIT);
                break;
            }
    }

    party->trailPos = party->position;
}

/** Resets the partner's scale and display offset when she leaves the ride mode. */
#if VERSION > VERSION_DEMO_US
static inline void partyChgRunMo(PartyEntry* party) {
    party->scale = (Vec){1.0f, 1.0f, 1.0f};
    party->dispOfs = (Vec){0.0f, 0.0f, 0.0f};
}
#endif

#if VERSION <= VERSION_DEMO_US
#include "driver/hitdrv.h"
#include "driver/npcdrv.h"
#include "mot/mot_plane.h"
extern f32 partyNrmToAngle(Vec* n);
/** TRUE when hit is a floor the partner may stand on (not flagged with hit attributes 0x200 or 0x800). */
static inline BOOL partyAppearChkFloor(HitEntry* hit) {
    return hit == NULL ? FALSE : (hitGetAttr(hit) & (HIT_ATTR_WATER_HAZARD | HIT_ATTR_SPIKES)) == 0;
}

/** Slope angle in degrees of the floor normal n (partyNrmToAngle on a copy). */
static inline f32 partyAppearNrmAngle(Vec n) {
    return partyNrmToAngle(&n);
}

/** Moves out by d units against direction ang (behind it). */
static inline void partyAppearMoveBack(Vec* out, f32 d, f32 ang) {
    movePos(&out->x, &out->z, -d, ang);
}

/**
 * Demo copy of the appear-position search (retail has it in party_move.c):
 * finds a free spot with floor 30 units from Mario for the partner to land on, trying behind him first,
 * then his sides and front; with a follower present the two take mirrored offsets.
 * In plane mode only checks behind. Returns FALSE if no spot has floor within 100 below.
 */
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

/**
 * Demo copy of the spot search for a partner placed directly (partyEntry, landing from plane mode):
 * 30 units behind Mario, or beside him when a follower is present, needing floor within one step below.
 * Returns FALSE if none fits.
 */
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

/**
 * Demo copy of the search for a free spot around Mario (Goombella's and Ms. Mowz's moves,
 * the partner motion code): tries nine directions starting diagonally behind him, needing open space,
 * a flat enough walkable floor (60 degrees at most) and no NPC in the way, then pushes the spot off walls.
 * Returns FALSE if none fits.
 */
BOOL partyGetAppearPos3(PartyEntry* party, Vec* out) {
    HitCheckQuery query;
    s32 angles[9] = {45, 135, -135, 315, -315, 180, -180, 90, -90};
    f32 s;
    f32 c;
    HitEntry* hit;
    u32 i;
    MarioWork* mario;
    f32 dir;
    f32 dist;
    f32 ang;

    mario = party->playerPtr;
    dist = 30.0f + party->width;
    dir = mario->displayDirectionTarget;
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
        if (partyAppearNrmAngle(query.hitNormal) > 60.0f) {
            continue;
        }
        if (!partyAppearChkFloor(hit)) {
            continue;
        }
        if (npcNearDistCheck(out->x, out->y, out->z, party->width)) {
            continue;
        }
        out->y = query.hitPos.y;
        marioChkWallAround(out, 2.0f, 0.0f, party->width, party->height);
        return TRUE;
    }
    return FALSE;
}

/**
 * Demo copy of the search for a spot near the partner's home direction (partner motion code):
 * starts from Mario, or from the other partner for a follower, and tries eleven directions around it;
 * falls back to partyGetAppearPos. Returns TRUE only if one of its own directions fits.
 */
BOOL partyGetAppearPos4(PartyEntry* party, Vec* out) {
    PartyEntry* other;
    MarioWork* mario = party->playerPtr;
    HitCheckQuery query;
    s32 angA[11] = {180, 150, -150, 120, -120, 90, -90, 60, -60, 30, -30};
    s32 angB[11] = {150, -150, 120, -120, 90, -90, 60, -60, 30, -30, 180};
    s32 angC[11] = {-150, 150, -120, 120, -90, 90, -60, 60, -30, 30, -180};
    f32 s;
    f32 c;
    s32 i;
    HitEntry* hit;
    f32 back;
    f32 dist;
    f32 ang;

    dist = 30.0f + party->width;
    back = revise360(180.0f + (270.0f - party->homeDir));
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
    for (i = 0; i <= sizeof(angA) / sizeof(angA[0]); i++) {
        if (i == 0) {
            ang = party->homeDir;
        } else if (!other) {
            ang = toMovedir2(back, angA[i]);
        } else if (!(party->flags & PARTY_FLAG_IS_PARTNER)) {
            ang = toMovedir2(back, angB[i]);
        } else {
            ang = toMovedir2(back, angC[i]);
        }
        sincosf(&s, &c, ang);
        query.targetDistance = dist;
        query.targetPos = (Vec){out->x, 37.0f + out->y, out->z};
        query.targetDir = (Vec){s, 0.0f, c};
        if (hitCheckVecFilter(&query, NULL)) {
            continue;
        }
        movePos(&out->x, &out->z, 30.0f, ang);
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
        marioChkWallAround(out, 4.0f, 0.0f, 1.2f * party->width, party->height);
        return TRUE;
    }
    partyGetAppearPos(party, out);
    return FALSE;
}
const Vec party_unused_zero_vec0 = {0.0f, 0.0f, 0.0f};
const Vec party_unused_zero_vec1 = {0.0f, 0.0f, 0.0f};
#endif
/**
 * Seats the follower on Yoshi with Mario straight away (ride-Yoshi mode 12 at step 10), without shadow;
 * used by party_yoshi when Mario mounts Yoshi.
 */
void party_force_ride_yoshi(PartyEntry* party) {
    Vec pos;

    partyForceChgRunMode(party, 12, 10);
    party->flags &= ~PARTY_FLAG_IS_BEING_USED;
    getRidePos(party, &pos);
    party->position = pos;
    partyChgPoseId(party, PARTY_POSE_STAND);
    party->velY = 0.0f;
    partyShadowOff(party);
    party->displayDirectionTarget = party->playerPtr->displayDirectionTarget;
    party->moveDir = toMovedir(party->displayDirectionTarget);
}

/** Snaps the follower to her seat behind Mario on Yoshi (party_yoshi, while Yoshi carries Mario). */
void party_ride_yoshi_force_move(void) {
    Vec pos;
    PartyEntry* party = partyGetPtr(marioGetExtraPartyId());
    if (party) {
        getRidePos(party, &pos);
        party->position = pos;
        party->displayDirectionTarget = party->playerPtr->displayDirectionTarget;
        party->moveDir = toMovedir(party->displayDirectionTarget);
    }
}

/**
 * The follower's ride-Yoshi mode (12): hops onto Yoshi behind Mario over 15 frames,
 * then rides there without shadow until Mario gets off, when she drops to her appear position and follows again.
 */
s32 partyRideYoshiMain(PartyEntry* party) {
    Vec pos;
    Vec pos2;
    MarioWork* mario = party->playerPtr;

    switch (party->motionStep) {
        case 0:
            party->gravity = 1.6f;
            getRidePos(party, &pos);
            party->moveTarget = pos;
            party->moveDistance = distABf(party->position.x, party->position.z, party->moveTarget.x, party->moveTarget.z);
            party->targetDir = angleABf(party->position.x, party->position.z, party->moveTarget.x, party->moveTarget.z);
            party->moveFrames = 15;
            party->moveSpeed = party->moveDistance / (f32)party->moveFrames;
            party->velY = (0.5f * ((f32)party->moveFrames * (party->gravity * (f32)party->moveFrames)) + (party->moveTarget.y - party->position.y)) / (f32)party->moveFrames;
            partyChgPoseId(party, PARTY_POSE_JUMP);
            party->motionStep = 1;
            break;
        case 1:
            getRidePos(party, &pos);
            party->moveTarget = pos;
            party->moveDistance = distABf(party->position.x, party->position.z, party->moveTarget.x, party->moveTarget.z);
            party->targetDir = angleABf(party->position.x, party->position.z, party->moveTarget.x, party->moveTarget.z);
            party->moveSpeed = party->moveDistance / (f32)party->moveFrames;
            party->velY -= party->gravity;
            party->position.y += party->velY;
            movePos(&party->position.x, &party->position.z, party->moveSpeed, party->targetDir);
            if (--party->moveFrames <= 0) {
                party->motionStep = 2;
            }
            break;
        case 2:
            partyChgPoseId(party, PARTY_POSE_STAND);
            party->velY = 0.0f;
            party->position.y = party->moveTarget.y;
            party->motionStep = 10;
            partyShadowOff(party);
            break;
        case 10:
            getRidePos(party, &pos);
            party->position = pos;
            party->displayDirectionTarget = mario->displayDirectionTarget;
            party->moveDir = toMovedir(party->displayDirectionTarget);
            if (mario->currMotionId != MARIO_MOTION_YOSHI) {
                partyGetAppearPos(party, &pos2);
                party->position.x = pos2.x;
                party->position.z = pos2.z;
                partyChgRunMode(party, PARTY_RUN_MOVE);
                partyShadowOn(party);
                party->dispOfs = (Vec){0.0f, 0.0f, 0.0f};
            }
            break;
    }
    return 0;
}

/**
 * Brings the partner back after Mario takes damage from a pit or spikes (run mode 13):
 * after a splash it waits 100 frames, plays her hurt pose, flies her up off screen,
 * waits for Mario's return fall, then drops her in from above next to him and resumes following.
 */
#if VERSION <= VERSION_DEMO_US
static void party_damage_return(PartyEntry* party) {
#else
void party_damage_return(PartyEntry* party) {
#endif
    MarioWork* mario = party->playerPtr;
    f32 x, y, z;
    f32 mx, my, mz;
    f32 lim;
    Vec pos;

    if (party->trigFlags & PARTY_TRIG_CHG_RUN_MODE) {
        party->trigFlags &= ~PARTY_TRIG_CHG_RUN_MODE;
        party->flags &= ~PARTY_FLAG_IS_BEING_USED;
        if (!(party->trigFlags & PARTY_TRIG_SPLASH)) {
            party->motionStep = 10;
        } else {
            party->motionStep = 0;
            party->moveFrames = 100;
        }
    }
    switch (party->motionStep) {
        case 0:
            if (--party->moveFrames > 0) {
                break;
            }
            party->motionStep = 10;
        case 10:
#if VERSION <= VERSION_DEMO_US
            partyChgPoseId(party, 9);
#else
            partyChgPoseId(party, 8);
#endif
            party->moveFrames = sysMsec2Frame(200);
            party->motionStep = 11;
        case 11:
            if (--party->moveFrames > 0) {
                break;
            }
            party->velY = 15.0f;
            party->accelY = 0.0f;
            party->motionStep = 12;
        case 12:
            party->position.y += party->velY;
            marioGetScreenPos(party->position, &x, &y, &z);
            marioGetScreenPos(mario->position, &mx, &my, &mz);
            lim = (my < -100.0f) ? my : -100.0f;
            if (y <= lim) {
                party->moveFrames = 32;
                party->motionStep = 20;
            }
            break;
        case 20:
#if VERSION > VERSION_DEMO_US
            marioGetScreenPos(party->position, &x, &y, &z);
#if VERSION >= VERSION_US
            if (y > -100.0f) {
                party->position.y += 20.0f;
            }
#else
            if (y <= -100.0f) {
                party->position.y += 1.0f;
            }
#endif
#endif
            if (mario->currMotionId == MARIO_MOTION_DAMAGE) {
                if (!marioWaitDamageReturnFall()) {
                    break;
                }
            } else if (mario->currMotionId == MARIO_MOTION_DAMAGE_TOGE) {
                if (!marioWaitDamageTogeReturnFall()) {
                    break;
                }
            }
            party->motionStep = 21;
        case 21:
            if (--party->moveFrames > 0) {
                break;
            }
            partyGetAppearPos(party, &pos);
            party->position.x = pos.x;
            party->position.z = pos.z;
            party->velY = 0.0f;
            party->accelY = -1.0f;
            party->moveSpeed = 0.0f;
            party->flags |= PARTY_FLAG_IS_FALLING;
            party->flags |= PARTY_FLAG_FORCE_FALL;
            party->motionStep = 22;
            break;
        case 22:
#if VERSION <= VERSION_DEMO_US
            {
                f32 sx, sy, sz;
                marioGetScreenPos(party->position, &sx, &sy, &sz);
                if (sy > 240.0f) {
                    party->flags &= ~PARTY_FLAG_FORCE_FALL;
                }
#else
            marioGetScreenPos(party->position, &x, &y, &z);
            if (y > 240.0f) {
                party->flags &= ~PARTY_FLAG_FORCE_FALL;
#endif
            }
            partyMove(party);
            if (party->velY <= -6.0f) {
                party->accelY = 0.0f;
                party->velY = -6.0f;
            }
            if (party->position.y < -2000.0f) {
                party->position = mario->position;
                party->position.y += 200.0f;
                party->flags &= ~(PARTY_FLAG_IS_JUMPING | PARTY_FLAG_IS_FALLING | PARTY_FLAG_IS_STEPPING);
            }

            // bit 9 of the attributes is HIT_ATTR_WATER_HAZARD
            if (party->hitobjStandOn && !((hitGetAttr(party->hitobjStandOn) >> 9) & 1)) {
#if VERSION > VERSION_DEMO_US
                partyChgPoseId(party, 9);
#endif
                party->motionStep = 23;
                party->moveFrames = 30;
            }
            break;
        case 23:
            if (--party->moveFrames > 0) {
                break;
            }
            partyChgRunMode(party, PARTY_RUN_MOVE);
            party->moveMode = PARTY_MOVE_MODE_STAY;
            party->flags2 &= ~PARTY_DISP_FLAG_NO_MAKE_DISP_DIR;
            break;
    }
#if VERSION > VERSION_DEMO_US
    if (vivianGetStatus() != VIVIAN_STATUS_HIDDEN) {
        vivianShadow(party);
    }
#endif
}

/** Puts the partner back on Mario's camera (evt_party). */
void partyInitCamId(PartyEntry* party) {
    party->camId = marioGetCamId();
}

/** Draws the partner with camera cameraId instead of Mario's (evt_party). */
void partySetCamId(PartyEntry* party, s32 cameraId) {
    party->camId = cameraId;
}

BOOL vivianDisplayTailCheck(PartyEntry* party);
void partyDisp(CameraId cam, void* param);

/**
 * Turns the partner to face left (180) or right (0) of the screen as she moves,
 * with Paper Mario's overshooting flip: eases past the target by about 40 degrees and back.
 * Leaves her alone in the paper slit or when her direction is locked.
 */
void partyMakeDispDir(PartyEntry* party) {
    f32 diff;
    f32 tgt;
    f32 cur;
    f32 step;

    if (party->flags2 & PARTY_DISP_FLAG_SLIT) {
        return;
    }
    if (!(party->flags2 & PARTY_DISP_FLAG_NO_MAKE_DISP_DIR)) {
        diff = revise360(party->moveDir - party->camDir);
        tgt = party->displayDirectionTarget;
        if (!(party->flags2 & PARTY_DISP_FLAG_SLIT)) {
            if (290.0f == tgt || 250.0f == tgt) {
                tgt = 180.0f;
                if (diff >= tgt) {
                    tgt = 0.0f;
                }
            }
        }
        if (party->moveSpeed) {
            if (diff >= 190.0f && diff <= 350.0f) {
                tgt = 0.0f;
            } else if (diff >= 10.0f && diff <= 170.0f) {
                tgt = 180.0f;
            }
        }
        party->displayDirectionTarget = tgt;
        if (party->flags2 & PARTY_DISP_FLAG_DISP_DIR_SNAP) {
            party->displayDirection = party->displayDirectionTarget;
        }
        if (party->displayDirection == party->displayDirectionTarget) {
            return;
        }
    }
    if ((cur = party->displayDirection) <= 180.0f) {
        if (0.0f == party->displayDirectionTarget) {
            step = 0.125f * (((party->trigFlags & PARTY_TRIG_DISP_DIR_CORRECTION) ? 20.0f : -40.0f) - party->displayDirection);
            party->displayDirection += step;
            if (!(party->trigFlags & PARTY_TRIG_DISP_DIR_CORRECTION)) {
                if (party->displayDirection < -30.0f) {
                    party->trigFlags |= PARTY_TRIG_DISP_DIR_CORRECTION;
                }
            } else if (party->displayDirection >= 0.0f) {
                party->displayDirection = 0.0f;
                party->trigFlags &= ~PARTY_TRIG_DISP_DIR_CORRECTION;
            }
        } else if (180.0f == party->displayDirectionTarget) {
            step = 0.125f * (((party->trigFlags & PARTY_TRIG_DISP_DIR_CORRECTION) ? 170.0f : 220.0f) - party->displayDirection);
            party->displayDirection += step;
            if (!(party->trigFlags & PARTY_TRIG_DISP_DIR_CORRECTION)) {
                if (party->displayDirection > 210.0f) {
                    party->trigFlags |= PARTY_TRIG_DISP_DIR_CORRECTION;
                }
            } else if (party->displayDirection <= 180.0f) {
                party->displayDirection = 180.0f;
                party->trigFlags &= ~PARTY_TRIG_DISP_DIR_CORRECTION;
            }
        }
    } else if (cur > 180.0f) {
        if (0.0f == party->displayDirectionTarget) {
            step = 0.125f * (((party->trigFlags & PARTY_TRIG_DISP_DIR_CORRECTION) ? 20.0f : -40.0f) - party->displayDirection);
            party->displayDirection += step;
            if (!(party->trigFlags & PARTY_TRIG_DISP_DIR_CORRECTION)) {
                if (party->displayDirection < -30.0f) {
                    party->trigFlags |= PARTY_TRIG_DISP_DIR_CORRECTION;
                }
            } else if (party->displayDirection >= 0.0f) {
                party->displayDirection = 0.0f;
                party->trigFlags &= ~PARTY_TRIG_DISP_DIR_CORRECTION;
            }
        } else if (180.0f == party->displayDirectionTarget) {
            step = 0.125f * (((party->trigFlags & PARTY_TRIG_DISP_DIR_CORRECTION) ? 170.0f : 220.0f) - party->displayDirection);
            party->displayDirection += step;
            if (!(party->trigFlags & PARTY_TRIG_DISP_DIR_CORRECTION)) {
                if (party->displayDirection > 210.0f) {
                    party->trigFlags |= PARTY_TRIG_DISP_DIR_CORRECTION;
                } else if (party->displayDirection <= 180.0f) {
                    party->displayDirection = 180.0f;
                    party->trigFlags &= ~PARTY_TRIG_DISP_DIR_CORRECTION;
                }
            }
        }
    }
}

/**
 * Applies the partner's pose and paper animation changes,
 * advances her model and queues her draw at her screen depth (flickering while invincible after damage),
 * then draws her shadow, following Vivian's shadow tail when she is out or used.
 */
#if VERSION <= VERSION_DEMO_US
static void partyPreDisp(PartyEntry* party) {
#else
void partyPreDisp(PartyEntry* party) {
#endif
    Vec screen;
    f32 z;
    f32 size;
    s32 mode;
    s32 mutekiTimer;

    if (!(party->flags2 & PARTY_DISP_FLAG_NO_MAKE_DISP_DIR) && !(party->flags & PARTY_FLAG_KEEP_POSE_UNUSED)) {
        partyMakeDispDir(party);
    }
    if (party->trigFlags & (PARTY_TRIG_CHG_ANIM | PARTY_TRIG_CHG_PAPER_ANIM)) {
        if (party->trigFlags & PARTY_TRIG_CHG_ANIM) {
            party->trigFlags &= ~PARTY_TRIG_CHG_ANIM;
        }
        if (party->trigFlags & PARTY_TRIG_CHG_PAPER_ANIM) {
            party->trigFlags &= ~PARTY_TRIG_CHG_PAPER_ANIM;
        }
        animPoseSetAnim(party->paperAnimGroup, party->animName, TRUE);
        if (party->paperAnimName) {
            animPoseSetPaperAnim(party->paperAnimGroup, party->paperAnimName, TRUE);
        }
        animPoseSetLocalTime(party->paperAnimGroup, party->poseTime);
    }
    animPoseMain(party->paperAnimGroup);
    if (party->flags2 & PARTY_DISP_FLAG_HIDE) {
        return;
    }
    screen = party->position;
    PSMTX44MultVec(camGetPtr(CAMERA_3D)->projection, &screen, &screen);
    z = 5000.0f * screen.z + 5000.0f;
    if (z < 0.0f) {
        z = 0.0f;
    } else if (z > 10000.0f) {
        z = 10000.0f;
    }
    z *= -1.0f;
    mutekiTimer = party->mutekiTimer;
    if (mutekiTimer > 0) {
        if (!(mutekiTimer & 2)) {
            dispEntry((CameraId)party->camId, z, DISP_MODE_ALPHA_TEST, partyDisp, party);
        }
    } else {
        dispEntry((CameraId)party->camId, z, DISP_MODE_ALPHA_TEST, partyDisp, party);
    }
    mode = 2;
    size = 13.333333f;
    if (party->currentMemberId != PARTY_MEMBER_VIVIAN) {
        party->trailPos = party->position;
    } else {
        if (party->runMode == 4 || (party->runMode == PARTY_RUN_GOODBYE && !(party->flags & PARTY_FLAG_IS_BEING_USED))) {
            party->trailPos = party->position;
        }
        if (party->flags & PARTY_FLAG_IS_BEING_USED) {
            size = party->work.vivian.shadowSize;
            if (party->flags & PARTY_FLAG_IS_EVT_USE) {
                mode = 1;
            }
        }
    }
#if VERSION <= VERSION_DEMO_US
    if (party->currentMemberId == PARTY_MEMBER_VIVIAN && !(party->work.vivian.tailFlags & 1)) {
#else
    if (vivianDisplayTailCheck(party)) {
#endif
        Vec pos;
        Vec dir;
        Vec hitPos;
        Vec hitNrm;
        f32 dist;

        dist = 1000.0f;
        pos = party->position;
        pos.y += 10.0f;
        dir = (Vec){0.0f, -1.0f, 0.0f};
        party->trailPos.y = partyHitCheck(party, &pos, &dir, &hitPos, &hitNrm, &dist) ? hitPos.y : party->position.y;
        animPoseVivianMain(party->paperAnimGroup, &party->trailPos);
    }
    if (!(party->flags2 & PARTY_DISP_FLAG_SHADOW_OFF)) {
        mode = shadowEntryMode(mode, party->trailPos.x, party->trailPos.y, party->trailPos.z, size);
        if (party->camId != CAMERA_3D) {
            shadowSetCamId(mode, party->camId);
        }
        if (party->flags2 & PARTY_DISP_FLAG_SHADOW_NODECAY) {
            shadowFlagOn(mode, SHADOW_ENTRY_NO_DECAY);
        }
    }
}

/**
 * Draw callback for the partner: builds her model matrix (scale 2, or 1.2 in background mode; left/right mirror;
 * rotations; offsets; a small per-member height fix for paper animations outside the demos) and draws the model,
 * slowed to 0.75 in shallow water.
 */
#if VERSION <= VERSION_DEMO_US
static void partyDisp(CameraId cam, void* param) {
#else
void partyDisp(CameraId cam, void* param) {
#endif
    PartyEntry* party = param;
    Mtx m;
    Mtx t;
    Mtx s;
    f32 scale;
    f32 dir;
    f32 yoff;
    BOOL flip;

    if (!partyCountActive()) {
        return;
    }
    if (party->flags & PARTY_FLAG_KEEP_POSE_UNUSED) {
        party->rotOfs.x = -6.8f;
        party->rotOfsY = 6.8f;
    }
    scale = (party->flags & PARTY_FLAG_BG_MODE) ? 1.2f : 2.0f;
    PSMTXScale(m, party->scale.x * scale, party->scale.y * scale, party->scale.z * scale);
    if (!(party->flags2 & PARTY_DISP_FLAG_NO_FLIP)) {
        dir = revise360(party->displayDirection);
        flip = FALSE;
        if (dir >= 90.0f && dir <= 270.0f) {
            flip = TRUE;
        }
        PSMTXScale(s, 1.0f, 1.0f, flip ? -1.0f : 1.0f);
        PSMTXConcat(s, m, m);
    }
    PSMTXTrans(t, -party->rotOfs.x, -party->rotOfsY, -party->rotOfsZ);
    PSMTXConcat(t, m, m);
    PSMTXRotRad(t, 'z', 0.017453292f * party->rotation);
    PSMTXConcat(t, m, m);
    PSMTXRotRad(t, 'x', 0.017453292f * party->rot.x);
    PSMTXConcat(t, m, m);
    PSMTXRotRad(t, 'y', 0.017453292f * revise360(party->displayDirection - party->camDir));
    PSMTXConcat(t, m, m);
    if (party->flags2 & (PARTY_DISP_FLAG_TURN_LEFT_UNUSED | PARTY_DISP_FLAG_TURN_RIGHT_UNUSED)) {
        dir = revise360(party->displayDirection);
        if (dir >= 90.0f && dir <= 270.0f) {
            PSMTXRotRad(t, 'y', 0.017453292f * ((party->flags2 & PARTY_DISP_FLAG_TURN_LEFT_UNUSED) ? 180.0f : 0.0f));
            PSMTXConcat(t, m, m);
        } else {
            PSMTXRotRad(t, 'y', 0.017453292f * ((party->flags2 & PARTY_DISP_FLAG_TURN_LEFT_UNUSED) ? 0.0f : 180.0f));
            PSMTXConcat(t, m, m);
        }
    }
    PSMTXTrans(t, party->rotOfs.x, party->rotOfsY, party->rotOfsZ);
    PSMTXConcat(t, m, m);
    yoff = 0.0f;
#if VERSION > VERSION_DEMO_US
    if (party->paperAnimName) {
        if (party->currentMemberId == PARTY_MEMBER_YOSHI) {
            yoff = 1.0f;
        } else if (party->currentMemberId == PARTY_MEMBER_BOBBERY) {
            yoff = 2.0f;
        } else if (party->currentMemberId == PARTY_MEMBER_MS_MOWZ) {
            yoff = 2.0f;
        } else if (party->currentMemberId == PARTY_MEMBER_GOOMBELLA) {
            yoff = 2.0f;
        } else if (party->currentMemberId == PARTY_MEMBER_FRANKLY) {
            yoff = 3.0f;
        } else if (party->currentMemberId == PARTY_MEMBER_EGG) {
            yoff = 3.5f;
        } else if (party->currentMemberId == PARTY_MEMBER_PUNIO) {
            yoff = 4.0f;
        } else if (party->currentMemberId == PARTY_MEMBER_FLAVIO) {
            yoff = -1.0f;
        }
    }
#endif
    PSMTXTrans(t, party->position.x + party->dispOfs.x, yoff + (party->position.y + party->dispOfs.y),
               party->position.z + party->dispOfs.z);
    PSMTXConcat(t, m, m);
    animPoseDrawMtx(party->paperAnimGroup, m, 2, 0.0f, 1.0f);
    animPoseDrawMtx(party->paperAnimGroup, m, 1, 0.0f, 1.0f);
    animPoseSetLocalTimeRate(party->paperAnimGroup, (party->flags & PARTY_FLAG_IN_SHALLOW_WATER) ? 0.75f : 1.0f);
}

/** Empty in the shipped game; npcdrv, evt_mario and evt_mobj call it where Mario's controls are switched back on. */
void partyKeyOn(void) {
}

/** Empty in the shipped game; npcdrv, evt_mario and evt_mobj call it where Mario's controls are switched off. */
void partyKeyOff(void) {
}

/** Clears the partner's copy of the controller state. */
void partyClearCont(PartyEntry* party) {
    party->stickX = 0;
    party->stickY = 0;
    party->subStickX = 0;
    party->subStickY = 0;
    party->triggerL = 0;
    party->triggerR = 0;
    party->analogA = 0;
    party->analogB = 0;
    party->buttonHold = 0;
    party->buttonTrg = 0;
    party->buttonRepeat = 0;
    party->buttonUp = 0;
}

/**
 * Copies the controller state of the partner's pad port into the entry,
 * or clears it while the partner is frozen; read by the abilities that steer the partner (Flurrie, Vivian,
 * Yoshi).
 */
void partyUpdateKeyData(PartyEntry* party) {
    s32 port = party->port;
    if (party->flags & (PARTY_FLAG_CTRL_OFF | PARTY_FLAG_CTRL_OFF_2)) {
        partyClearCont(party);
        return;
    }
    party->stickX = gp->stickX[port];
    party->stickY = -gp->stickY[port];
    party->subStickX = gp->substickX[port];
    party->subStickY = -gp->substickY[port];
    party->triggerL = gp->triggerLeft[port];
    party->triggerR = gp->triggerRight[port];
    party->analogA = 0;
    party->analogB = 0;
    party->buttonHold = gp->button[port];
    party->buttonTrg = gp->buttonNew[port];
    party->buttonRepeat = gp->buttonRepeat[port];
    party->buttonUp = gp->buttonUp[port];
}

/** Hides the partner's shadow (pipes, riding, events); NULL is ignored. */
void partyShadowOff(PartyEntry* party) {
    if (party) {
        party->flags2 |= PARTY_DISP_FLAG_SHADOW_OFF;
    }
}

/** Reloads the partner's model for her normal or 8-bit form (see partyPoseEntry). */
static inline void partyPoseEntryF(PartyEntry* party) {
    char* animName;

    if (party->currentMemberId != PARTY_MEMBER_YOSHI) {
        if (!(party->flags2 & PARTY_DISP_FLAG_FAMICOM)) {
            animName = party_dp->name;
        } else {
            animName = "d_mario";
        }
    } else {
        animName = yoshigroup[pouchGetPartyColor(PARTNER_YOSHI)];
        if (party->flags2 & PARTY_DISP_FLAG_FAMICOM) {
            animName = "d_mario";
        }
    }
    if (party->paperAnimGroup >= 0) {
        animPoseRelease(party->paperAnimGroup);
    }
    party->paperAnimGroup = animPoseEntry(animName, 2);
}

/**
 * Switches the partner into (mode 1) or out of the 8-bit Famicom form, reloading her model and standing pose.
 * Unused in C.
 */
void partySetFamicomMode(PartyEntry* party, s32 mode) {
    if (mode == 1) {
        if (party->flags2 & PARTY_DISP_FLAG_FAMICOM) {
            return;
        }
#if VERSION > VERSION_DEMO_US
        partyForceSlitOff(party);
#endif
        party->flags2 |= PARTY_DISP_FLAG_FAMICOM;
        partyPoseEntryF(party);
        partyChgPose(party, partyChgPoseId(party, PARTY_POSE_STAND));
    } else {
        if (!(party->flags2 & PARTY_DISP_FLAG_FAMICOM)) {
            return;
        }
        party->flags2 &= ~PARTY_DISP_FLAG_FAMICOM;
        partyPoseEntryF(party);
        partyChgPose(party, partyChgPoseId(party, PARTY_POSE_STAND));
    }
}
