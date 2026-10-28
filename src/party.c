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

PartyEntry* partyPtrTbl[2];
PartyData* party_dp;
s32 partyCtrlNo;

s32 partyEntryMain(s32 memberId);

PartyEntry* partyGetPtr(s32 index) {
    if (index >= 0) {
        return partyPtrTbl[index];
    }
    return NULL;
}

PartyEntry* anotherPartyGetPtr(s32 index) {
    if (index < 0) {
        return NULL;
    }
    return partyPtrTbl[(index + 1) & 1];
}

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

void partyPaperLightOff(PartyEntry* party) {
    animPoseSetMaterialLightFlagOff(party->paperAnimGroup, MATERIAL_LIGHT_PAPER);
}

static inline BOOL partyIsPinch(PartyEntry* party) {
    BOOL pinch = FALSE;

    if (party->currentMemberId < MAIN_PARTY_MEMBER_MAX) {
        if (partyGetHp(party->currentMemberId) <= 5) {
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
    if (party->currentMemberId < 8) {
        if (partyGetHp(party->currentMemberId) <= 5) {
            pinch = TRUE;
        }
    }
    if (pinch == TRUE) {
        if (index == 1) {
            poseIdx = 12;
        } else if (index == 2) {
            poseIdx = 13;
        } else if (index == 3) {
            poseIdx = 14;
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

const char* partyChgPoseId(PartyEntry* party, s32 index) {
    const char* pose;
    const char* curPose;
    s32 poseIdx = index;

    if ((party->flags2 & PARTY_DISP_FLAG_POSE_LOCK) != 0) {
        return party->animName;
    }
    if (partyIsPinch(party) == TRUE) {
        if (index == 1) {
            poseIdx = 12;
        } else if (index == 2) {
            poseIdx = 13;
        } else if (index == 3) {
            poseIdx = 14;
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

void partyChgPaper(PartyEntry* party, char* paper) {
    if (!(party->flags2 & PARTY_DISP_FLAG_POSE_LOCK)) {
        if (party->paperAnimName == NULL || strcmp(party->paperAnimName, paper)) {
            party->paperAnimName = paper;
            party->poseTime = 0;
            party->trigFlags |= PARTY_TRIG_CHG_PAPER_ANIM;
        }
    }
}

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

void partyRun(PartyEntry* party) {
    if (party->runMode == 0) {
        partyChgRunMode(party, 1);
    }
}

void partyStop(PartyEntry* party) {
    partyChgRunMode(party, 0);
}

void partySetForceMove(PartyEntry* party, s32 frames, f32 dir, f32 speed) {
    party->targetDir = dir;
    party->forceMoveFrames = frames;
    party->moveSpeed = speed;
    partyChgRunMode(party, 9);
    partyChgMot(party, 1);
}

s32 partyEntryHello(s32 memberId) {
    s32 id = partyEntryMain(memberId);
    partyChgRunMode(partyGetPtr(id), 4);
    return id;
}

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
    partyChgPoseId(party, 1);
    party->runMode = 1;
    return id;
}

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
    index = 1;
    if (!(party->flags2 & PARTY_DISP_FLAG_POSE_LOCK)) {
        lowHp = FALSE;
        if (party->currentMemberId < MAIN_PARTY_MEMBER_MAX && partyGetHp(party->currentMemberId) <= 5) {
            lowHp = TRUE;
        }
        if (lowHp == TRUE) {
            index = 12;
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
    party->runMode = 0;
    return id;
}

BOOL partyGoodbye(s32 id) {
    BOOL ret = FALSE;

    if (id >= 0) {
        partyChgRunMode(partyGetPtr(id), 8);
        ret = TRUE;
    }
    return ret;
}

s32 partyKill(s32 id) {
    PartyEntry* party;
    MarioWork* player;

    party = partyGetPtr(id);
    if (party == NULL) {
        return 0;
    }
    player = party->playerPtr;
    party_dp = &partyDataTbl[partyGetPtr(player->currPartySlotId[id])->currentMemberId];
    if (party_dp->exit_func && party->runMode != 11) {
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

s32 partyKill2(s32 id) {
    PartyEntry* party;
    MarioWork* player;

    party = partyGetPtr(id);
    if (party == NULL) {
        return 0;
    }
    player = party->playerPtr;
    party_dp = &partyDataTbl[partyGetPtr(player->currPartySlotId[id])->currentMemberId];
    if (party_dp->exit_func && party->runMode != 11) {
        party_dp->exit_func(party);
    }
    player->prevPartyMemberId[id] = 0;
    player->currPartySlotId[id] = -1;
    partyPtrTbl[id] = NULL;
    animPoseRelease(party->paperAnimGroup);
    __memFree(HEAP_DEFAULT, party);
    return 0;
}

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

f32 partyGetHeight(PartyEntry* party) {
    if (party->currentMemberId == PARTY_MEMBER_YOSHI && (party->flags & PARTY_FLAG_IS_BEING_USED)) {
        return 18.5f + party->height;
    }
    return party->height;
}

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

static void setAnimPoseMaterialFlag(PartyEntry* party) {
    MarioWork* player = party->playerPtr;

    animPoseSetMaterialFlagOn(party->paperAnimGroup, animPoseGetMaterialFlag(player->paperAnimGroupId[marioAnimeId()]));
    animPoseSetMaterialLightFlagOn(party->paperAnimGroup,
                                   animPoseGetMaterialLightFlag(player->paperAnimGroupId[marioAnimeId()]));
}

static inline s32 partyGetFreeSlot2(void) {
    if (partyPtrTbl[1] == NULL) {
        memset(partyPtrTbl[1] = __memAlloc(HEAP_DEFAULT, sizeof(PartyEntry)), 0, sizeof(PartyEntry));
        return 1;
    }
    return -1;
}

char* yoshigroup[7] = {
    "c_babyyoshi",
    "c_babyyoshi2",
    "c_babyyoshi3",
    "c_babyyoshi4",
    "c_babyyoshi5",
    "c_babyyoshi6",
    "c_babyyoshi7"
};

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
    partyChgPose(party, partyChgPoseId(party, 1));
    party->paperAnimName = NULL;
    party_dp->init_func(party);
    return slot;
}

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
    partyChgPose(party, partyChgPoseId(party, 1));
    party->paperAnimName = NULL;
    party_dp->init_func(party);
    party->position = player->position;
    party->trailPos = party->position;
    party->scale = (Vec){1.0f, 1.0f, 1.0f};
    party->runMode = 0;
    return slot;
}

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
    partyChgPose(party, partyChgPoseId(party, 1));
    party->paperAnimName = NULL;
    party_dp->init_func(party);
    partyChgRunMode(party, 4);
    return slot;
}

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
    partyChgPose(party, partyChgPoseId(party, 1));
    party->paperAnimName = NULL;
    party_dp->init_func(party);
    partyChgRunMode(party, 2);
    party->position = (Vec){x, y, z};
    party->trailPos = party->position;
    party->scale.x = 1.0f;
    party->scale.y = 1.0f;
    party->scale.z = 1.0f;
    return slot;
}

void partyVivianTailStart(PartyEntry* party);
void driveParty(PartyEntry* party);
s32 partyHelloMain(PartyEntry* party);
static void partyShadowOn(PartyEntry* party) {
    if (party) {
        party->flags2 &= ~PARTY_DISP_FLAG_SHADOW_OFF;
    }
}

void partyInit(void) {
    memset(partyPtrTbl, 0, sizeof(partyPtrTbl));
}

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
        partyChgRunMode(party, 2);
        partyChgMot(party, 0);
        party->yoshiPsndSFXId = -1;
        party->scale = (Vec){1.0f, 1.0f, 1.0f};
    }
    party = partyGetPtr(marioGetPartyId());
    if (party) {
        dotted = party->flags2 & PARTY_DISP_FLAG_FAMICOM;
        party->flags = PARTY_FLAG_IS_ACTIVE;
        party->flags2 = 0;
        party->slitOn = 0;
        party->camId = marioGetCamId();
        if (party) {
            poseId = party->paperAnimGroup;
            if (poseId >= 0) {
                animPoseSetMaterialFlagOff(poseId, -1);
                animPoseSetMaterialLightFlagOn(poseId, MATERIAL_LIGHT_CHARA);
            }
            if (party->paperAnimName) {
                animPoseSetPaperAnimGroup(party->paperAnimGroup, NULL, FALSE);
                party->paperAnimName = NULL;
                party->flags2 &= ~PARTY_DISP_FLAG_PAPER;
                if (party->paperPoseId >= 0) {
                    animPaperPoseRelease(party->paperPoseId);
                    party->paperPoseId = -1;
                }
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
        if (party->currentMemberId == PARTY_MEMBER_YOSHI && party->runMode == 3) {
            return;
        }
        if (!(marioGetPtr()->flags & MARIO_FLAG_PARTY_WALK_DISABLED)) {
            pose = partyChgPoseId(party, 1);
            partyChgPose(party, pose);
            partyChgRunMode(party, 2);
            partyChgMot(party, 0);
        } else {
            pose = partyChgPoseId(party, 1);
            partyChgPose(party, pose);
            partyChgRunMode(party, 2);
            party->moveMode = 6;
            party->motionStep = 0;
            party->flags |= 0x8000;
        }
        if (party->currentMemberId == PARTY_MEMBER_MS_MOWZ) {
            chuchu_reinit(party);
        }
        partyResetTouchWallCt();
    }
}
void partyUsePost(PartyEntry* party) {
    PartyFunc func;
    if (party && party->runMode == 3) {
        func = partyDataTbl[party->currentMemberId].post_func;
        if (func) {
            func(party);
        }
    }
}

static inline void partyChgRunM(PartyEntry* party);
static inline void partyChgRunMo(PartyEntry* party);
void N__partyChgRunMode(PartyEntry* party, s32 prev, s32 mode) {
    MarioWork* mario;
    PartyFunc func;

    switch (prev) {
        case 11:
            if (mode != 8) {
                mario = party->playerPtr;
                partyChgRunMo(party);
                party->moveDir = mario->moveDirection;
                party->targetDir = mario->directionView;
                party->displayDirectionTarget = mario->displayDirectionTarget;
                party->flags &= ~0x70000;
                partyClearJumpPara(party);
                partyChgPoseId(party, 1);
                party->flags2 &= ~PARTY_DISP_FLAG_NO_MAKE_DISP_DIR;
                party->flags2 &= ~PARTY_DISP_FLAG_HIDE;
                partyShadowOn(party);
            }
            break;
        case 4:
            mario = party->playerPtr;
            partyChgRunM(party);
            party->flags2 &= ~PARTY_DISP_FLAG_NO_MAKE_DISP_DIR;
            party->moveDir = mario->moveDirection;
            party->targetDir = mario->directionView;
            party->displayDirectionTarget = mario->displayDirectionTarget;
            party->displayDirection = mario->displayDirection;
            partyClearJumpPara(party);
            partyChgPoseId(party, 1);
            party->flags2 &= ~PARTY_DISP_FLAG_NO_MAKE_DISP_DIR;
            party->runMode = 1;
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
void partyChgRunMode(PartyEntry* party, s32 mode) {
    if (party->runMode != 9 && party->runMode != 3) {
        party->prevRunMode = party->runMode;
    }
    N__partyChgRunMode(party, party->runMode, mode);
    party->runMode = mode;
    party->trigFlags |= PARTY_TRIG_CHG_RUN_MODE;
}
#pragma dont_inline reset

#pragma dont_inline on
void N_partyForceChgRunMode(PartyEntry* party, s32 mode, s32 sub) {
    if (party->runMode != 9 && party->runMode != 3) {
        party->prevRunMode = party->runMode;
    }
    N__partyChgRunMode(party, party->runMode, mode);
    party->runMode = mode;
    party->trigFlags &= ~PARTY_TRIG_CHG_RUN_MODE;
    party->motionStep = sub;
}
#pragma dont_inline reset

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
        partyChgPoseId(party, 1);
        party->flags2 &= ~PARTY_DISP_FLAG_NO_MAKE_DISP_DIR;
        party->runMode = 1;
        partyUsePost(party);
        partyChgMot(party, 0);
    } else if (party->runMode == 11) {
        MarioWork* mario = party->playerPtr;
        partyChgRunMo(party);
        party->moveDir = mario->moveDirection;
        party->targetDir = mario->directionView;
        party->displayDirectionTarget = mario->displayDirectionTarget;
        party->flags &= ~0x70000;
        partyClearJumpPara(party);
        partyChgPoseId(party, 1);
        party->flags2 &= ~PARTY_DISP_FLAG_NO_MAKE_DISP_DIR;
        party->flags2 &= ~PARTY_DISP_FLAG_HIDE;
        partyShadowOn(party);
        partyUsePost(party);
        partyChgMot(party, 0);
    } else {
        if (party->runMode == 12) {
            partyShadowOn(party);
        }
        partyUsePost(party);
        party->runMode = party->prevRunMode;
        party->trigFlags |= PARTY_TRIG_CHG_RUN_MODE;
        partyChgMot(party, 0);
    }
}

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
void partyMain(void) {
    s32 i;
    PartyEntry* party;

    marioGetPtr();
    if (partyCountActive()) {
        marioUsePartyMain();
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

void driveParty(PartyEntry* party) {
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
            if (vivianGetStatus() != 2) {
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
                partyChgPoseId(party, 1);
                party->moveSpeed = 0.0f;
                party->flags &= ~PARTY_FLAG_IS_BEING_USED;
                partyChgMot(party, 0);
            }
            party_dp = &partyDataTbl[party->currentMemberId];
            party_dp->move_func(party);
            break;
        case 1:
            if ((party->flags & PARTY_FLAG_CTRL_OFF) && !(party->flags & PARTY_FLAG_RUN_OFF)) {
                break;
            }
            party_move_mode_wait(party);
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
                partyForceSlitOff(party);
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

BOOL partyDoWork(void) {
    PartyEntry* party = partyGetPtr(marioGetPtr()->currPartySlotId[0]);
    if (!party) {
        return FALSE;
    }
    if (party->runMode != 0 && party->runMode != 1 && party->runMode != 2) {
        return FALSE;
    }
    marioGetPtr()->baseSpeed = 0.0f;
    partyChgRunMode(party, 3);
    return TRUE;
}

void partyHello(PartyEntry* party) {
    partyHello_inl(party);
}


s32 partyHelloMain(PartyEntry* party) {
    s32 ret = 0;
    u8* mario = (u8*)party->playerPtr;

    switch (party->motionStep) {
        case 0: {
                f32 x, y, z, h, dy;
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
                if (party->displayDirectionTarget >= 90.0f && party->displayDirectionTarget <= 270.0f) {
                    party->moveDir = partyToMovedir(party, 180.0f);
                    party->targetDir = party->moveDir;
                    party->displayDirectionTarget = party->displayDirection = 180.0f;
                } else {
                    party->moveDir = partyToMovedir(party, 0.0f);
                    party->targetDir = party->moveDir;
                    party->displayDirectionTarget = party->displayDirection = 0.0f;
                }
                partyClearJumpPara(party);
                partyChgPoseId(party, 1);
                party->flags2 &= ~PARTY_DISP_FLAG_NO_MAKE_DISP_DIR;
                ret = 1;
                party->runMode = 1;
                psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_PARTY_OUT1 | PSND_SFX_BYPASS_MUTE), &((MarioWork*)mario)->position);
                break;
        }
        case 10:
            partyGetAppearPos2(party, &party->position);
            party->trailPos = party->position;
            partyChgPoseId(party, 1);
            party->runMode = 1;
            ret = 1;
            break;
        case 20:
            partyChgPoseId(party, 1);
            party->runMode = 0;
            ret = 1;
            break;
        case 30:
            allPartyForceRideOn();
            ret = 1;
            break;
    }
    return ret;
}

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

s32 partyGoodbyeMain(PartyEntry* party) {
    MarioWork* mario = marioGetPtr();
    f32 tx, ty, tz, px, py, pz;

    switch (party->motionStep) {
        case 0:
            party_dp = &partyDataTbl[party->currentMemberId];
            party_dp->use_func(party);
            if (party->currentMemberId == PARTY_MEMBER_VIVIAN) {
                if (vivianGetStatus() == 0) {
                    party->motionStep = 10;
                    party->runMode = 8;
                }
            } else if (party->currentMemberId == PARTY_MEMBER_YOSHI) {
                if (yoshiGetStatus() == 0) {
                    party->motionStep = 10;
                    party->runMode = 8;
                }
            } else {
                party->motionStep = 10;
                party->runMode = 8;
            }
            break;
        case 10:
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_PARTY_IN1 | PSND_SFX_BYPASS_MUTE), &mario->position);
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
            partyChgPoseId(party, 4);
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
            partyChgPoseId(party, 1);
            party->velY = 0.0f;
            party->position.y = party->moveTarget.y;
            partyChgRunMode(party, 14);
            return 1;
        case 20:
            partyChgRunMode(party, 14);
            return 1;
    }
    return 0;
}

void partyVivianTailStop(PartyEntry* party);

static inline void partyChgRunM(PartyEntry* party) {
    party->scale = (Vec){1.0f, 1.0f, 1.0f};
}
s32 partyDokanInit(PartyEntry* party) {
    MarioWork* mario = party->playerPtr;

#ifdef VERSION_G8ME01
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
#ifdef VERSION_G8ME01
            partyChgPaper(party, "PM_D_1A");
            partyChgPoseId(party, 0);
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
#ifdef VERSION_G8ME01
            party->flags2 |= PARTY_DISP_FLAG_NO_FLIP;
            party->flags2 |= PARTY_DISP_FLAG_NO_MAKE_DISP_DIR;
            animPoseSetMaterialLightFlagOff(party->paperAnimGroup, MATERIAL_LIGHT_PAPER);
        case 1:
            partyPaperOn(party, "p_dokan_y");
            partyChgPaper(party, "PM_D_1A");
            partyChgPoseId(party, 0);
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
#ifdef VERSION_G8ME01
            partyChgPaper(party, "PM_D_1A");
            partyChgPoseId(party, 0);
#endif
            party->spinSpeed = 26.0f;
            party->motionStep = 10;
            party->rotOfsY = 10.0f;
#ifdef VERSION_G8ME01
            party->flags2 |= PARTY_DISP_FLAG_NO_FLIP;
            party->flags2 |= PARTY_DISP_FLAG_NO_MAKE_DISP_DIR;
            animPoseSetMaterialLightFlagOff(party->paperAnimGroup, MATERIAL_LIGHT_PAPER);
            break;
        case 4:
            party->displayDirectionTarget = mario->displayDirection;
            partyPaperOn(party, "p_dokan_x");
            partyChgPaper(party, "PM_D_1A");
            partyChgPoseId(party, 0);
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
    partyChgPoseId(party, 0);
    party->flags2 |= PARTY_DISP_FLAG_NO_FLIP;
    party->flags2 |= PARTY_DISP_FLAG_NO_MAKE_DISP_DIR;
    animPoseSetMaterialLightFlagOff(party->paperAnimGroup, MATERIAL_LIGHT_PAPER);
    party->displayDirectionTarget = revise360(180.0f + mario->displayDirection);
    party->displayDirection = mario->displayDirection;
    party->moveDir = mario->moveDirection;
    party->position = mario->position;
#endif
    partyVivianTailStop(party);
    return 0;
}

s32 partyDokanMain(PartyEntry* party) {
    MarioWork* mario = party->playerPtr;
    s32 n;
#ifdef VERSION_G8ME01
    f32 s;
    f32 c;
#endif

    switch (party->motionStep) {
        case 0:
            party->position = mario->position;
            party->displayDirection = revise360(party->displayDirection + party->spinSpeed);
#ifndef VERSION_G8ME01
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
#ifdef VERSION_G8ME01
            party->position = mario->position;
            sincosf(&s, &c, mario->targetYaw);
            party->position.x = mario->position.x + -5.0f * s;
            party->position.z = mario->position.z + 5.0f * c;
#endif
            break;
        case 100:
            switch (party->dirKind) {
                case 0:
                case 1:
                    partyChgPaper(party, "PM_D_1B");
                    party->moveFrames = 18;
                    party->motionStep = 101;
#ifdef VERSION_G8ME01
                    party->flags2 |= PARTY_DISP_FLAG_NO_FLIP;
                    party->flags2 |= PARTY_DISP_FLAG_NO_MAKE_DISP_DIR;
                    animPoseSetMaterialLightFlagOff(party->paperAnimGroup, MATERIAL_LIGHT_PAPER);
                    break;
                case 4:
                    party->rot.x = 0.0f;
                    party->flags2 &= ~PARTY_DISP_FLAG_NO_FLIP;
                    party->displayDirectionTarget = party->displayDirection;
#else
                    break;
                case 4:
#endif
                    partyChgPaper(party, "PM_D_1B");
                    party->moveFrames = 18;
                    party->motionStep = 101;
                    break;
            }
            break;
        case 101:
            switch (party->dirKind) {
                case 0:
                case 1:
#ifdef VERSION_G8ME01
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
            animPoseSetMaterialLightFlagOn(party->paperAnimGroup, MATERIAL_LIGHT_PAPER);
            party->flags2 &= ~PARTY_DISP_FLAG_NO_FLIP;
            party->flags2 &= ~PARTY_DISP_FLAG_NO_MAKE_DISP_DIR;
            party->flags &= ~PARTY_FLAG_DISABLE_FLOORS;
            party->flags &= ~PARTY_FLAG_FORCE_FALL;
            partyShadowOn(party);
            partyVivianTailStart(party);
            party->trailPos = party->position;
            partyChgRunMode(party, 1);
            n = 0;
            if (partyGetPtr(marioGetPartyId())) {
                n = 1;
            }
            if (partyGetPtr(marioGetExtraPartyId())) {
                n++;
            }
            if (party->dirKind == 4) {
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
            if (party->dirKind == 1) {
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
            break;
    }
    return 0;
}

void partyDokanEnd(s32 id, s32 dir) {
    PartyEntry* party = partyGetPtr(id);
    if (party && party->runMode == 10) {
        party->motionStep = 100;
        party->dirKind = dir;
        party->flags2 &= ~PARTY_DISP_FLAG_NO_FLIP;
        party->flags2 &= ~PARTY_DISP_FLAG_NO_MAKE_DISP_DIR;
        party->flags2 &= ~(PARTY_DISP_FLAG_TURN_LEFT_UNUSED | PARTY_DISP_FLAG_TURN_RIGHT_UNUSED);
        party->flags &= ~PARTY_FLAG_DISABLE_FLOORS;
        party->flags &= ~PARTY_FLAG_FORCE_FALL;
        partyShadowOn(party);
        party->trailPos = party->position;
        partyVivianTailStart(party);
    }
}

void allPartyRidePlane(void) {
    PartyEntry* party = partyGetPtr(marioGetPartyId());
    if (party) {
        bomhei_use_cancel();
        party->flags |= PARTY_FLAG_IS_RIDING_PLANE;
        partyChgRunMode(party, 11);
    }
    party = partyPtrTbl[1];
    if (party) {
        party->flags |= PARTY_FLAG_IS_RIDING_PLANE;
        partyChgRunMode(party, 11);
    }
}

void allPartyRideShip(void) {
    PartyEntry* party = partyGetPtr(marioGetPartyId());
    if (party) {
        bomhei_use_cancel();
        party->flags |= PARTY_FLAG_IS_RIDING_SHIP;
        partyChgRunMode(party, 11);
    }
    party = partyPtrTbl[1];
    if (party) {
        party->flags |= PARTY_FLAG_IS_RIDING_SHIP;
        partyChgRunMode(party, 11);
    }
}

void getRidePos(PartyEntry* party, Vec* pos) {
    MarioWork* mario = party->playerPtr;
    PartyEntry* ride;
    f32 angle;
    f32 s;
    f32 c;

    if (mario->currMotionId == MARIO_MOTION_YOSHI) {
        angle = 3.1415927f * toMovedir(mario->displayDirectionTarget) / 180.0f;
        if (yoshiGetStatus() != 2) {
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
            if (yoshiGetStatus() != 2) {
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
    } else if (mario->currMotionId == MARIO_MOTION_SHIP) {
        sincosf(&s, &c, toMovedirSimple(N_marioShipGetDir() ? 0.0f : 180.0f));
        pos->x = mario->position.x + 22.0f * s;
        pos->z = mario->position.z + 22.0f * c;
        pos->y = mario->position.y;
    } else {
        pos->x = mario->position.x;
        pos->z = mario->position.z;
        pos->y = 20.0f + mario->position.y;
    }
}

#pragma dont_inline on
void allPartyForceRideOn(void) {
    PartyEntry* party = partyGetPtr(marioGetPartyId());
    if (party) {
        N_partyForceChgRunMode(party, 11, 11);
        party->flags2 |= PARTY_DISP_FLAG_HIDE;
        partyShadowOff(party);
    }
    party = partyGetPtr(marioGetExtraPartyId());
    if (party) {
        N_partyForceChgRunMode(party, 11, 11);
        party->flags2 |= PARTY_DISP_FLAG_HIDE;
        partyShadowOff(party);
    }
}
#pragma dont_inline reset

void L_allPartyRideOn(void) {
    PartyEntry* party = partyGetPtr(marioGetPartyId());
    if (party) {
        partyChgRunMode(party, 11);
    }
    party = partyGetPtr(marioGetExtraPartyId());
    if (party) {
        partyChgRunMode(party, 11);
    }
}

void allPartyRideOff(void) {
    PartyEntry* party = partyGetPtr(marioGetPartyId());
    if (party && party->runMode == 11) {
#ifdef VERSION_G8ME01
        if (!(party->trigFlags & PARTY_TRIG_CHG_RUN_MODE)) {
            party->motionStep = 100;
        } else {
            party->runMode = 1;
        }
#else
        party->motionStep = 100;
#endif
    }
    party = partyGetPtr(marioGetExtraPartyId());
    if (party && party->runMode == 11) {
#ifdef VERSION_G8ME01
        if (!(party->trigFlags & PARTY_TRIG_CHG_RUN_MODE)) {
            party->motionStep = 100;
        } else {
            party->runMode = 1;
        }
#else
        party->motionStep = 100;
#endif
    }
}

void allPartyRideOff2(void) {
    PartyEntry* party = partyGetPtr(marioGetPartyId());
    if (party && party->runMode == 11) {
#ifdef VERSION_G8ME01
        if (!(party->trigFlags & PARTY_TRIG_CHG_RUN_MODE)) {
            party->motionStep = 101;
        } else {
            party->runMode = 1;
        }
#else
        party->motionStep = 101;
#endif
    }
    party = partyGetPtr(marioGetExtraPartyId());
    if (party && party->runMode == 11) {
#ifdef VERSION_G8ME01
        if (!(party->trigFlags & PARTY_TRIG_CHG_RUN_MODE)) {
            party->motionStep = 101;
        } else {
            party->runMode = 1;
        }
#else
        party->motionStep = 101;
#endif
    }
}

s32 partyRideChk(PartyEntry* party) {
    s32 ret = 0;
    if (party == NULL) {
        return 0;
    }
    if (party->runMode == 11) {
        ret = 1;
        if (party->motionStep >= 2) {
            ret = 2;
        }
    }
    return ret;
}

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
            partyChgPoseId(party, 4);
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
                partyChgPoseId(party, 1);
                party->velY = 0.0f;
                party->position.y = party->moveTarget.y;
                party->scale = (Vec){0.0f, 0.0f, 0.0f};
                party->flags2 |= PARTY_DISP_FLAG_HIDE;
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
                partyChgPoseId(party, 1);
                party->flags2 |= PARTY_DISP_FLAG_HIDE;
                partyShadowOff(party);
                getRidePos(party, &pos);
                party->scale = (Vec){0.0f, 0.0f, 0.0f};
                party->position = pos;
                party->velY = 0.0f;
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
            party->flags &= ~0x70000;
            party->dispOfs.y = 0.0f;
            party->scale.x = 1.0f;
            party->scale.y = party->scale.x;
            party->scale.z = party->scale.x;
            partyShadowOn(party);
            partyChgRunMode(party, 1);
            break;

        case 100:

        case 101: {
                f32 x;
                f32 y;
                f32 z;
                f32 h;
                f32 dy;
                party->flags2 &= ~PARTY_DISP_FLAG_HIDE;
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
                party->flags2 &= ~PARTY_DISP_FLAG_HIDE;
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
                ((PartyEntry*)party)->moveFrames = 0;
                party->displayDirectionTarget = ((MarioWork*)mario)->displayDirectionTarget;
                party->displayDirection = ((MarioWork*)mario)->displayDirectionTarget;
                party->moveDir = toMovedirSimple(party->displayDirection);
                party->targetDir = party->moveDir;
                partyClearJumpPara(party);
                partyChgPoseId(party, 1);
                party->flags2 &= ~PARTY_DISP_FLAG_NO_MAKE_DISP_DIR;
                party->flags &= ~0x70000;
                party->dispOfs.y = 0.0f;
                party->scale.x = 1.0f;
                party->scale.y = party->scale.x;
                party->scale.z = party->scale.x;
                party->flags2 &= ~PARTY_DISP_FLAG_HIDE;
                partyShadowOn(party);
                partyChgRunMode(party, 1);
                break;
            }

    }

    party->trailPos = party->position;
}
static inline void partyChgRunMo(PartyEntry* party) {
    party->scale = (Vec){1.0f, 1.0f, 1.0f};
    party->dispOfs = (Vec){0.0f, 0.0f, 0.0f};
}

void party_force_ride_yoshi(PartyEntry* party) {
    Vec pos;

    N_partyForceChgRunMode(party, 12, 10);
    party->flags &= ~PARTY_FLAG_IS_BEING_USED;
    getRidePos(party, &pos);
    party->position = pos;
    partyChgPoseId(party, 1);
    party->velY = 0.0f;
    partyShadowOff(party);
    party->displayDirectionTarget = party->playerPtr->displayDirectionTarget;
    party->moveDir = toMovedir(party->displayDirectionTarget);
}

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
            partyChgPoseId(party, 4);
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
            partyChgPoseId(party, 1);
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
                partyChgRunMode(party, 2);
                partyShadowOn(party);
                party->dispOfs = (Vec){0.0f, 0.0f, 0.0f};
            }
            break;
    }
    return 0;
}

void party_damage_return(PartyEntry* party) {
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
            partyChgPoseId(party, 8);
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
            marioGetScreenPos(party->position, &x, &y, &z);
#ifdef VERSION_G8ME01
            if (y > -100.0f) {
                party->position.y += 20.0f;
            }
#else
            if (y <= -100.0f) {
                party->position.y += 1.0f;
            }
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
            marioGetScreenPos(party->position, &x, &y, &z);
            if (y > 240.0f) {
                party->flags &= ~PARTY_FLAG_FORCE_FALL;
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
                partyChgPoseId(party, 9);
                party->motionStep = 23;
                party->moveFrames = 30;
            }
            break;
        case 23:
            if (--party->moveFrames > 0) {
                break;
            }
            partyChgRunMode(party, 2);
            party->moveMode = 0;
            party->flags2 &= ~PARTY_DISP_FLAG_NO_MAKE_DISP_DIR;
            break;
    }
    if (vivianGetStatus() != 2) {
        vivianShadow(party);
    }
}

void partyInitCamId(PartyEntry* party) {
    party->camId = marioGetCamId();
}

void partySetCamId(PartyEntry* party, s32 cameraId) {
    party->camId = cameraId;
}

BOOL N_vivianDisplayTailCheck(PartyEntry* party);
void partyDisp(CameraId cam, void* param);

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

void partyPreDisp(PartyEntry* party) {
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
            dispEntry((CameraId)party->camId, z, 1, partyDisp, party);
        }
    } else {
        dispEntry((CameraId)party->camId, z, 1, partyDisp, party);
    }
    mode = 2;
    size = 13.333333f;
    if (party->currentMemberId != PARTY_MEMBER_VIVIAN) {
        party->trailPos = party->position;
    } else {
        if (party->runMode == 4 || (party->runMode == 8 && !(party->flags & PARTY_FLAG_IS_BEING_USED))) {
            party->trailPos = party->position;
        }
        if (party->flags & PARTY_FLAG_IS_BEING_USED) {
            size = party->work.vivian.shadowSize;
            if (party->flags & PARTY_FLAG_IS_EVT_USE) {
                mode = 1;
            }
        }
    }
    if (N_vivianDisplayTailCheck(party)) {
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

void partyDisp(CameraId cam, void* param) {
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
    PSMTXTrans(t, party->position.x + party->dispOfs.x, yoff + (party->position.y + party->dispOfs.y),
               party->position.z + party->dispOfs.z);
    PSMTXConcat(t, m, m);
    animPoseDrawMtx(party->paperAnimGroup, m, 2, 0.0f, 1.0f);
    animPoseDrawMtx(party->paperAnimGroup, m, 1, 0.0f, 1.0f);
    animPoseSetLocalTimeRate(party->paperAnimGroup, (party->flags & PARTY_FLAG_IN_SHALLOW_WATER) ? 0.75f : 1.0f);
}

void partyKeyOn(void) {
}

void partyKeyOff(void) {
}

void partyClearCont(PartyEntry* party) {
    party->stickX = 0;
    party->stickY = 0;
    party->subStickX = 0;
    party->subStickY = 0;
    party->triggerL = 0;
    party->triggerR = 0;
    party->reserved_15E = 0;
    party->reserved_15F = 0;
    party->buttonHold = 0;
    party->buttonTrg = 0;
    party->buttonRepeat = 0;
    party->buttonUp = 0;
}

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
    party->reserved_15E = 0;
    party->reserved_15F = 0;
    party->buttonHold = gp->button[port];
    party->buttonTrg = gp->buttonNew[port];
    party->buttonRepeat = gp->buttonRepeat[port];
    party->buttonUp = gp->buttonUp[port];
}

void partyShadowOff(PartyEntry* party) {
    if (party) {
        party->flags2 |= PARTY_DISP_FLAG_SHADOW_OFF;
    }
}

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

void partySetFamicomMode(PartyEntry* party, s32 mode) {
    if (mode == 1) {
        if (party->flags2 & PARTY_DISP_FLAG_FAMICOM) {
            return;
        }
        partyForceSlitOff(party);
        party->flags2 |= PARTY_DISP_FLAG_FAMICOM;
        partyPoseEntryF(party);
        partyChgPose(party, partyChgPoseId(party, 1));
    } else {
        if (!(party->flags2 & PARTY_DISP_FLAG_FAMICOM)) {
            return;
        }
        party->flags2 &= ~PARTY_DISP_FLAG_FAMICOM;
        partyPoseEntryF(party);
        partyChgPose(party, partyChgPoseId(party, 1));
    }
}
