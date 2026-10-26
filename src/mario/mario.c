#include "mario/mario.h"
#include "driver/animdrv.h"
#include "driver/hitdrv.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/shadowdrv.h"
#include "driver/seqdrv.h"
#include "mario/mario_cam.h"
#include "mario/mario_motion.h"
#include "mario/mario_party.h"
#include "mario/mario_pouch.h"
#include "mario/mario_sbr.h"
#include "mario/mariost.h"
#include "party/party_vivian.h"
#include "party.h"
#include "system.h"
#include "memory.h"
#include <string.h>
#include "libc_decls.h"
#include <math.h>
#include "party/party_kuribo.h"
#include "party/party_bomhei.h"
#include "mot/mot_plane.h"
#include "mot/mot_roll.h"
#include "mot/mot_ship.h"
#include "mot/mot_slit.h"
#include "party/party_yoshi.h"
#include "party/party_nokonoko.h"
#include "mario/mario_hit.h"
#include "mot/mot_damage.h"
#include "koopa.h"
#include "mario/mario_balloon.h"
#include "peach.h"
#include "driver/npcdrv.h"
#include "dolphin/mtx.h"
#include "party/party_move.h"
#include "driver/itemdrv.h"

#ifdef VERSION_G8ME01
#endif


// defined without a header prototype: marioGetMoveRate in mario_sbr.c (party_yoshi declares it with no parameter,
// and the shared prototype changes its code); N_dou10_yoko_yari3 in mario_hit.c takes no parameter but is called
// here with one
f32 marioGetMoveRate(f32 dir);
void N_dou10_yoko_yari3(void* p);

//.data
const char* dotMarioPose[4][6] = {
    {"Z_1", "S_1", "W_1", "R_1", "U_1", "J_1"},
    {"Z_15", "S_2", "W_15", "R_15", "U_2", "J_2"},
    {"Z_17", "S_3", "W_17", "R_17", "U_3", "J_3"},
    {"Z_18", "S_4", "W_18", "R_18", "U_4", "J_4"}
};

//.sdata
const char* marioAnimeGroupData[4][3] = {
    {"a_mario", "a_mario_r", "e_mario"},
    {"a_mario_l", "a_mario_lr", "e_mario_l"},
    {"a_mario_w", "a_mario_wr", "e_mario_w"},
    {"a_mario_wl", "a_mario_wlr", "e_mario_wl"}
};

//.bss
Vec blurPos[8];

//.sbss
s32 unk_804186EC;
MarioWork* mp;
Vec R_last_position;
s32 blurPosId;

//local prototypes
void marioDispBlurSub(Mtx mtx, s32 blurId);
const char* toRearPose(const char* name);
const char* toFrontPose(const char* name);

MarioWork* marioGetPtr(void) {
    return mp;
}

s32 marioGetColor(void) {
    return mp->colorId;
}


static inline void marioCharPoseRelease(MarioWork* wp) {
    if (wp->paperAnimGroupId[0] >= 0) {
        animPoseRelease(wp->paperAnimGroupId[0]);
        wp->paperAnimGroupId[0] = -1;
    }
    if (wp->paperAnimGroupId[1] >= 0) {
        animPoseRelease(wp->paperAnimGroupId[1]);
        wp->paperAnimGroupId[1] = -1;
    }
    if (wp->paperAnimGroupId[2] >= 0) {
        animPoseRelease(wp->paperAnimGroupId[2]);
        wp->paperAnimGroupId[2] = -1;
    }
}

static inline s32 marioBadgeColor(void) {
    s32 a = pouchEquipCheckBadge(ITEM_L_EMBLEM);
    s32 b = pouchEquipCheckBadge(ITEM_W_EMBLEM);

    if (a && !b) {
        return 1;
    } else if (!a && b) {
        return 2;
    } else if (a && b) {
        return 3;
    }
    return 0;
}

void marioSetCharMode(s32 mode) {
    MarioWork* wp = mp;
    u32 matFlag;
    u32 lightFlag;
    s32 color;

    if (mode == CHARACTER_PEACH) {
        wp->characterId = CHARACTER_PEACH;
        peachPreInit();
        marioCharPoseRelease(wp);
        if (!(wp->peachFlags & PEACH_FLAG_GUNDAN)) {
            wp->paperAnimGroupId[0] = animPoseEntry("c_peach", 2);
            wp->paperAnimGroupId[1] = animPoseEntry("c_peach_r", 2);
            npcSetMarioAutoTalkPose("P_S_1", "P_T_1");
        } else {
            wp->paperAnimGroupId[0] = animPoseEntry("c_zako_n", 2);
            wp->paperAnimGroupId[1] = animPoseEntry("c_peach_r", 2);
            npcSetMarioAutoTalkPose("S_1", "T_1");
            wp->wPlayerCollisionBox.x = wp->wPlayerCollisionBox.y = 30.0f;
        }
        marioSetSpec();
        wp->trigFlags |= MARIO_TRIG_FLAG_CHG_ANIM;
        return;
    }
    if (mode == CHARACTER_BOWSER) {
        kpaPreInit();
        marioCharPoseRelease(wp);
        wp->paperAnimGroupId[0] = animPoseEntry(kpaGetStageViewType() == 1 ? "c_koopa" : "c_koopa2", 2);
        wp->characterId = CHARACTER_BOWSER;
        npcSetMarioAutoTalkPose("KPA_S_1", "KPA_T_1");
        kpaSetLevel(1);
        wp->trigFlags |= MARIO_TRIG_FLAG_CHG_ANIM;
        return;
    }
    if (!(wp->flags & MARIO_FLAG_IS_8_BIT)) {
        matFlag = animPoseGetMaterialFlag(wp->paperAnimGroupId[marioAnimeId()]);
        lightFlag = animPoseGetMaterialLightFlag(wp->paperAnimGroupId[marioAnimeId()]);
        wp->characterId = CHARACTER_MARIO;
        if (!mp->characterId) {
            color = marioBadgeColor();
            if (!mp->characterId && mp->colorId != color) {
                mp->colorId = color;
                marioSetCharMode(CHARACTER_MARIO);
            }
        }
        marioCharPoseRelease(wp);
        wp->paperAnimGroupId[0] = animPoseEntry(marioAnimeGroupData[mp->colorId][0], 2);
        wp->paperAnimGroupId[1] = animPoseEntry(marioAnimeGroupData[mp->colorId][1], 2);
        wp->paperAnimGroupId[2] = animPoseEntry(marioAnimeGroupData[mp->colorId][2], 2);
        animPoseSetMaterialFlagOn(wp->paperAnimGroupId[0], matFlag);
        animPoseSetMaterialFlagOn(wp->paperAnimGroupId[1], matFlag);
        animPoseSetMaterialFlagOn(wp->paperAnimGroupId[2], matFlag);
        animPoseSetMaterialLightFlagOn(wp->paperAnimGroupId[0], lightFlag);
        animPoseSetMaterialLightFlagOn(wp->paperAnimGroupId[1], lightFlag);
        animPoseSetMaterialLightFlagOn(wp->paperAnimGroupId[2], lightFlag);
        npcSetMarioAutoTalkPose("M_S_1", "M_I_2");
        wp->trigFlags |= MARIO_TRIG_FLAG_CHG_ANIM;
    } else {
        marioSetFamicomMode(1);
    }
    if (!(wp->flags & MARIO_FLAG_BG_MODE)) {
        if (wp->currMotionId == MARIO_MOTION_PLANE) {
            marioForcePlaneAnime();
            return;
        } else if (wp->currMotionId == MARIO_MOTION_SHIP) {
            marioForceShipAnime();
            return;
        } else if (wp->currMotionId == MARIO_MOTION_ROLL) {
            marioForceRollAnime();
            return;
        } else if (wp->dispFlags & MARIO_DISP_FLAG_SLIT) {
            marioForceSlitAnime();
            return;
        } else if (wp->currMotionId == MARIO_MOTION_VIVIAN) {
            N_marioForceVivianAnime();
            return;
        } else if (wp->currMotionId == MARIO_MOTION_YOSHI) {
            marioChgPose("M_Y_2");
            marioChgPose("M_Y_1");
            return;
        }
    }
    if (wp->currMotionId != MARIO_MOTION_PLANE && wp->currMotionId != MARIO_MOTION_SHIP && wp->currMotionId != MARIO_MOTION_JABARA) {
        marioSetSpec();
    }
}

void marioSetFamicomMode(s32 mode) {
    MarioWork* wp = mp;

    if (mode == 1) {
        if (!(wp->flags & MARIO_FLAG_IS_8_BIT)) {
            wp->flags |= MARIO_FLAG_IS_8_BIT;
            marioChgMot(MARIO_MOTION_STAY);
            if (wp->paperAnimGroupId[0] >= 0) {
                animPoseRelease(wp->paperAnimGroupId[0]);
            }
            if (wp->paperAnimGroupId[1] >= 0) {
                animPoseRelease(wp->paperAnimGroupId[1]);
            }
            wp->paperAnimGroupId[0] = animPoseEntry("d_mario", 2);
            wp->paperAnimGroupId[1] = animPoseEntry(marioAnimeGroupData[mp->colorId][1], 2);
            wp->characterId = CHARACTER_MARIO;
            npcSetMarioAutoTalkPose("M_S_1", "M_I_2");
            wp->animName = NULL;
            marioChgPose("M_S_1");
        } else {
            wp->animName = NULL;
            marioChgPose("M_S_1");
        }
    } else {
        if (wp->flags & MARIO_FLAG_IS_8_BIT) {
            wp->flags &= ~MARIO_FLAG_IS_8_BIT;
            if (wp->paperAnimGroupId[0] >= 0) {
                animPoseRelease(wp->paperAnimGroupId[0]);
            }
            if (wp->paperAnimGroupId[1] >= 0) {
                animPoseRelease(wp->paperAnimGroupId[1]);
            }
            wp->paperAnimGroupId[0] = animPoseEntry(marioAnimeGroupData[mp->colorId][0], 2);
            wp->paperAnimGroupId[1] = animPoseEntry(marioAnimeGroupData[mp->colorId][1], 2);
            wp->characterId = CHARACTER_MARIO;
            npcSetMarioAutoTalkPose("M_S_1", "M_I_2");
            wp->animName = NULL;
            marioChgPose("M_S_1");
        } else {
            wp->animName = NULL;
            marioChgPose("M_S_1");
        }
    }
}

void marioSetSpec(void) {
    MarioWork* wp = mp;

    if (wp->characterId == CHARACTER_PEACH) {
        (wp->wPlayerCollisionBox).x = 20.0f;
        (wp->wPlayerCollisionBox).y = 47.0f;
        wp->mBaseWalkSpeed = 1.0f;
        wp->mBaseDashSpeed = 2.0f;
    } else if (wp->characterId == CHARACTER_BOWSER) {
        wp->mBaseWalkSpeed = 1.0f;
        wp->mBaseDashSpeed = 2.25f;
    } else {
        if (!(wp->flags & MARIO_FLAG_BG_MODE)) {
            (wp->wPlayerCollisionBox).x = 20.0f;
            (wp->wPlayerCollisionBox).y = 37.0f;
        } else {
            (wp->wPlayerCollisionBox).x = 10.0f;
            (wp->wPlayerCollisionBox).y = 25.9f;
        }
        wp->mBaseWalkSpeed = 1.0f;
        wp->mBaseDashSpeed = 2.25f;
    }
}

void marioEntry(void) {
    MarioWork* wp = mp;

    wp->flags |= MARIO_FLAG_ACTIVE;
    marioChgPose("M_S_1");
    if (wp->flags & MARIO_FLAG_BG_MODE) {
        if (wp->currMotionId == MARIO_MOTION_SLIT) {
            wp->flags |= MARIO_FLAG_PAPER_MODE;
            marioSlitForceCancel();
        }
        marioChgMot(MARIO_MOTION_STAY);
    } else if (wp->currMotionId == MARIO_MOTION_SHIP) {
        marioReInit_ship();
    } else if (wp->currMotionId == MARIO_MOTION_ROLL) {
        marioReInit_roll();
    } else if (wp->currMotionId == MARIO_MOTION_SLIT) {
        marioReInit_slit();
    } else if (wp->currMotionId == MARIO_MOTION_YOSHI) {
        marioRideYoshi();
    } else {
        marioChgMot(MARIO_MOTION_STAY);
    }
}

void marioItemGetDisable(void) {
    MarioWork* wp = mp;
    wp->flags |= MARIO_FLAG_ITEM_GET_DISABLED;
    wp->itemGetDisableTimer = 0x14;
}

void marioItemGetChk(void) {
    MarioWork* work;
    s32 var;

    work = mp;
    var = mp->itemGetDisableTimer;
    if (var > 0) {
        var--;
        work->itemGetDisableTimer = (s16)var;
        if ((s16)var <= 0) {
            work->itemGetDisableTimer = 0;
            mp->flags &= ~0x4000u;
        }
    }
}

BOOL marioItemGetOk(void) {
    return (mp->flags & MARIO_FLAG_ITEM_GET_DISABLED) == 0;
}

BOOL marioCaseEventValidChk(void) {
    if (mp->currMotionId == MARIO_MOTION_PLANE) {
        return FALSE;
    } else {
        return vivianGetStatus() == 0;
    }
}

BOOL marioFrontActionValidChk(void) {
    if (mp->currMotionId == MARIO_MOTION_ROLL || mp->flags & MARIO_FLAG_PAPER_MODE) {
        return FALSE;
    }
    return TRUE;
}

static inline s32 marioChkCtrl2(void) { return mp->ctrlOffCount ? 0 : 1; }
static inline BOOL marioCtrlDisable(void) {
    return marioChkCtrl2() ? 0 : 1;
}

static inline BOOL marioKeyDisable(void) {
    if (!marioChkKey()) {
        return TRUE;
    }
    return FALSE;
}

static inline BOOL marioChkYoshiKey(void) {
    PartyEntry* party = partyGetPtr(marioGetPartyId());

    if (seqGetSeq() != SEQ_GAME) {
        return FALSE;
    }
    if (mp->keyOffCount) {
        return FALSE;
    }
    if (yoshiGetStatus() == 1) {
        return TRUE;
    }
    return FALSE;
}

BOOL marioCheckMenuDisable(void) {
    MarioWork* wp = mp;

    if (marioCtrlDisable()) {
        return TRUE;
    }
    if (!strncmp(gp->currentMapName, "yuu_", 4)) {
        if (marioCtrlDisable() || marioKeyDisable()) {
            return TRUE;
        }
        return FALSE;
    }
    if (wp->characterId == CHARACTER_PEACH) {
        return TRUE;
    }
    if (wp->characterId == CHARACTER_BOWSER && kpaGetStageViewType()) {
        return TRUE;
    }
    if (wp->currMotionId == MARIO_MOTION_HAMMER || wp->currMotionId == MARIO_MOTION_HAMMER_2) {
        return TRUE;
    }
    if (wp->currMotionId == MARIO_MOTION_JABARA) {
        return TRUE;
    }
    if (marioGetPlaneStatus() == 1) {
        return TRUE;
    }
    if (marioShipChanging() == 1 || marioShipChanging() == 2) {
        return TRUE;
    }
    if (wp->currMotionId == MARIO_MOTION_DAMAGE || wp->currMotionId == MARIO_MOTION_DAMAGE_TOGE) {
        return TRUE;
    }
    if ((s32)(u8)wp->characterId != CHARACTER_BOWSER && !marioGetPlaneStatus() && !wp->hitobjStandOn) {
        return TRUE;
    }
    if (!mario_bomhei_keychk()) {
        return TRUE;
    }
#ifdef VERSION_G8ME01
    if (vivianGetStatus()) {
        if (!(partyGetPtr(marioGetPartyId())->flags & 0x80000000)) {
            if (vivianGetStatus() == 2) {
                return FALSE;
            }
        } else {
            return TRUE;
        }
    }
#else
    if (vivianGetStatus() && vivianGetStatus() == 2) {
        return FALSE;
    }
#endif
    if (marioChkYoshiKey()) {
        return FALSE;
    }
    if (marioKeyDisable()) {
        return TRUE;
    }
    return FALSE;
}

BOOL marioChkKey(void) {
    PartyEntry* party = partyGetPtr(marioGetPartyId());

    if (seqGetSeq() != SEQ_GAME) {
        return FALSE;
    }
    if (mp->keyOffCount) {
        return FALSE;
    }
    if (marioShipChanging() == 1 || marioShipChanging() == 2) {
        return FALSE;
    }
    if (!marioRollKeyDisable()) {
        return FALSE;
    }
    if (!marioSlitKeyDisable()) {
        return FALSE;
    }
    if (!party) {
        return TRUE;
    }
    if (christineGetStatus() == 1) {
        return FALSE;
    }
    if (!mario_bomhei_keychk2()) {
        return FALSE;
    }
    if (yoshiGetStatus()) {
        return FALSE;
    }
    if (party->flags & PARTY_FLAG_IS_BEING_USED) {
        if (party->currentMemberId == PARTY_MEMBER_MS_MOWZ || party->currentMemberId == PARTY_MEMBER_FLURRIE || party->currentMemberId == PARTY_MEMBER_VIVIAN) {
            return FALSE;
        }
    }
    if (nokonoko_holdItem()) {
        return FALSE;
    }
    return TRUE;
}

BOOL marioChkCtrl(void) {
    return !mp->ctrlOffCount;
}

BOOL N_marioChkUseParty(void) {
    if ((u16)gp->buttonNew[0] & 0x100F) {
        return TRUE;
    }
    return mp->ctrlOffCount;
}

void marioFBattlePrepare(void) {
    MarioWork* wp = mp;

    R_last_position = wp->position;
    marioAdjustMoveDir();
    if (wp->currMotionId != MARIO_MOTION_JABARA) {
        wp->flags |= MARIO_FLAG_PRE_BATTLE_PHYSICS;
    }
    partyUsePost(partyGetPtr(marioGetPartyId()));
    partyCtrlOff();
}

void marioFBattlePost(void) {
    MarioWork* wp = mp;

    wp->flags &= ~MARIO_FLAG_PRE_BATTLE_PHYSICS;
    if (wp->currMotionId == MARIO_MOTION_JABARA) {
        wp->position = R_last_position;
    }
    if (wp->currMotionId == MARIO_MOTION_YOSHI) {
        marioYoshiForceCancel();
    }
#ifdef VERSION_G8ME01
    {
        PartyEntry* party = partyGetPtr(marioGetPartyId());
        if (party) {
            Vec pos;
            partyGetAppearPos(party, &pos);
            party->position = pos;
        }
    }
#endif
    partyCtrlOn();
    if (wp->currMotionId == MARIO_MOTION_HAMMER || wp->currMotionId == MARIO_MOTION_JABARA) {
        marioChgMot(MARIO_MOTION_STAY);
    }
}

s32 marioCtrlOffChk(void) {
    return mp->ctrlOffCount;
}

s32 marioKeyOffChk(void) {
    return mp->keyOffCount;
}

s32 marioCtrlOff(void) {
    MarioWork* wp = mp;

    wp->flags |= MARIO_FLAG_CTRL_OFF;
    wp->ctrlOffCount++;
    return wp->ctrlOffCount;
}

s32 marioCtrlOn(void) {
    MarioWork* wp = mp;

    wp->ctrlOffCount--;
    if (wp->ctrlOffCount == 0) {
        wp->flags &= ~MARIO_FLAG_CTRL_OFF;
    }
    return wp->ctrlOffCount;
}

s32 marioCtrlOff2(void) {
    MarioWork* wp = mp;

    wp->flags |= MARIO_FLAG_CTRL_OFF_2;
    wp->flags |= MARIO_FLAG_CTRL_OFF;
    wp->ctrlOffCount++;
    return wp->ctrlOffCount;
}

s32 marioCtrlOn2(void) {
    MarioWork* wp = mp;

    wp->flags &= ~MARIO_FLAG_CTRL_OFF_2;
    wp->ctrlOffCount--;
    if (wp->ctrlOffCount == 0) {
        wp->flags &= ~MARIO_FLAG_CTRL_OFF;
    }
    return wp->ctrlOffCount;
}

s32 marioKeyOff(void) {
    MarioWork* wp = mp;

    wp->flags |= MARIO_FLAG_KEY_OFF;
    wp->keyOffCount++;
    return wp->keyOffCount;
}

s32 marioKeyOn(void) {
    MarioWork* wp = mp;

    wp->keyOffCount--;
    if (wp->keyOffCount == 0) {
        wp->flags &= ~MARIO_FLAG_KEY_OFF;
    }
    return wp->keyOffCount;
}

s16 marioSetMutekiTime(s32 msec) {
    MarioWork* wp = mp;
    PartyEntry* party;

    wp->mutekiTimer = sysMsec2Frame(msec);
    party = partyGetPtr(marioGetPartyId());
    if (party) {
        party->mutekiTimer = wp->mutekiTimer;
    }
    return wp->mutekiTimer;
}

BOOL marioBgmodeChk(void) {
    return (mp->flags >> 25) & 1;
}

void marioBgmodeOn(void) {
    mp->flags |= MARIO_FLAG_BG_MODE;
    marioSetSpec();
    camFollowYOff();
}

void marioBgmodeOff(void) {
    mp->flags &= ~MARIO_FLAG_BG_MODE;
    marioSetSpec();
    camFollowYOn();
}

BOOL marioChkSts(s32 flags) {
    MarioWork* wp = mp;
    BOOL ret = FALSE;

    if (flags & 1 && wp->flags & MARIO_FLAG_IS_JUMPING) {
        ret = TRUE;
    }
    if (flags & 2 && wp->flags & MARIO_FLAG_IS_FALLING) {
        ret = TRUE;
    }
    if (flags & 4 && wp->flags & MARIO_FLAG_PAPER_MODE) {
        ret = TRUE;
    }
    if (flags & 0x20 && wp->currMotionId == MARIO_MOTION_KAZE) {
        ret = TRUE;
    }
    if (flags & 0x10 && wp->currMotionId == MARIO_MOTION_ROLL) {
        ret = TRUE;
    }
    if (flags & 8 && wp->currMotionId == MARIO_MOTION_JABARA) {
        ret = TRUE;
    }
    return ret;
}

void marioResetHitObj(const char* name) {
    MarioWork* wp = mp;

    if (wp->hitobjFrontA && !strcmp(hitGetName((HitEntry*)wp->hitobjFrontA), name)) {
        wp->hitobjFrontA = 0;
    }
    if (wp->hitobjFront && !strcmp(hitGetName((HitEntry*)wp->hitobjFront), name)) {
        wp->hitobjFront = 0;
    }
    if (wp->hitobjStandOn && !strcmp(hitGetName(wp->hitobjStandOn), name)) {
        wp->hitobjStandOn = NULL;
    }
    if (wp->hitobjJumpFrom && !strcmp(hitGetName(wp->hitobjJumpFrom), name)) {
        wp->hitobjJumpFrom = NULL;
    }
    if (wp->hitobjPush && !strcmp(hitGetName(wp->hitobjPush), name)) {
        wp->hitobjPush = NULL;
    }
    if (wp->hitobjWall && !strcmp(hitGetName((HitEntry*)wp->hitobjWall), name)) {
        wp->hitobjWall = 0;
    }
    if (wp->hitobjHead && !strcmp(hitGetName((HitEntry*)wp->hitobjHead), name)) {
        wp->hitobjHead = 0;
    }
    if (wp->hitobjUnder && !strcmp(hitGetName((HitEntry*)wp->hitobjUnder), name)) {
        wp->hitobjUnder = 0;
    }
    if (wp->hitobjHammer && !strcmp(hitGetName((HitEntry*)wp->hitobjHammer), name)) {
        wp->hitobjHammer = 0;
    }
}

void marioReset(void) {
    MarioWork* wp = mp;

    marioSetSpec();
    wp->flags &= ~MARIO_FLAG_PAPER_MODE;
    wp->flags &= ~MARIO_FLAG_TUBE_MODE;
    marioPaperOff();
    marioChgPaper(NULL);
    wp->dispFlags &= ~MARIO_DISP_FLAG_NO_DIR_FLIP;
    wp->dispFlags &= ~MARIO_DISP_FLAG_NO_MAKE_DISP_DIR;
    marioOfsRotReset();
    wp->baseSpeed = 0.0f;
    marioChgMot(MARIO_MOTION_STAY);
}

void marioOfsRotReset(void) {
    MarioWork* wp = mp;

    wp->rot = (Vec){0.0f, 0.0f, 0.0f};
    wp->rotOfs = (Vec){0.0f, 0.0f, 0.0f};
    wp->dispOfs = (Vec){0.0f, 0.0f, 0.0f};
    wp->dispOfs2 = (Vec){0.0f, 0.0f, 0.0f};
}

void marioSoundInit(void) {
    MarioWork* wp = mp;

    wp->soundId[0] = -1;
    wp->soundId[1] = -1;
    wp->soundId[2] = -1;
#ifdef VERSION_G8ME01
    wp->soundId[3] = -1;
    wp->soundId[4] = -1;
#endif
}

void marioPoseInit(void) {
    MarioWork* wp = mp;

    wp->paperAnimGroupId[0] = animPoseEntry(marioAnimeGroupData[mp->colorId][0], 2);
    wp->paperAnimGroupId[1] = animPoseEntry(marioAnimeGroupData[mp->colorId][1], 2);
    wp->paperAnimGroupId[2] = animPoseEntry(marioAnimeGroupData[mp->colorId][2], 2);
}

void marioInit(void) {
    mp = __memAlloc(HEAP_DEFAULT, sizeof(MarioWork));
    memset(mp, 0, sizeof(MarioWork));
    marioSoundInit();
    pouchInit();
    marioResetGravity();
    marioInitCamId();
    mp->colorId = 0;
    animGroupBaseAsync(marioAnimeGroupData[mp->colorId][0], 2, NULL);
    animGroupBaseAsync(marioAnimeGroupData[mp->colorId][1], 2, NULL);
    animGroupBaseAsync(marioAnimeGroupData[mp->colorId][2], 2, NULL);
    mp->paperAnimGroupId[0] = -1;
    mp->paperAnimGroupId[1] = -1;
    mp->paperAnimGroupId[2] = -1;
    mp->paperPoseId = -1;
    if (!mp->characterId) {
        mp->mBaseWalkSpeed = 1.0f;
        mp->mBaseDashSpeed = 2.25f;
    } else {
        mp->mBaseWalkSpeed = 1.0f;
        mp->mBaseDashSpeed = 2.0f;
    }
    mp->currPartySlotId[0] = -1;
    mp->currPartySlotId[1] = -1;
    mp->wPlayerCollisionBox.x = 20.0f;
    mp->wPlayerCollisionBox.y = 37.0f;
    mp->scale = (Vec){1.0f, 1.0f, 1.0f};
    mp->basePos = mp->position;
    mp->wPlayerCollisionBox.z = 20.0f;
    marioChgPose("M_S_1");
    mp->reserved_20 = NULL;
    mp->bottomlessResetPosition = (Vec){0.0f, 74.0f, 0.0f};
    marioChgMot(MARIO_MOTION_STAY);
    marioBaloonInit();
    marioGetRubInit();
    blurPosId = 0;
    blurPos[0] = mp->position;
    blurPos[1] = mp->position;
    blurPos[2] = mp->position;
    blurPos[3] = mp->position;
    blurPos[4] = mp->position;
    blurPos[5] = mp->position;
    blurPos[6] = mp->position;
    blurPos[7] = mp->position;
    marioResetCamShift();
    marioResetCamShiftRate();
    marioResetCamFollowRate();
    if (!pouchCheckItem(ITEM_HAMMER)) {
        pouchGetItem(ITEM_HAMMER);
    }
    partyInit();
}

void marioReInit(void) {
    CameraId camId;
    s32 slot0;
    s32 slot1;
    s32 anim1;
    s32 anim2;
    MarioWork* wp;
    s32 anim0;
    s32 trigFlags;
    s32 peachFlags;
    MarioMotion motion;
    s32 charId;
    s32 dotFlag;
    s32 poseId;
    s32 t;
    s8 prev0;
    s8 prev1;
    struct KpaWork* kpaWork;
    f32 f1A8;
    f32 f1AC;
    f32 f1A0;
    f32 f19C;

    dotFlag = (wp = mp)->flags & 0x80000000;
    camId = marioGetCamId();
    charId = wp->characterId;
    slot0 = wp->currPartySlotId[0];
    slot1 = wp->currPartySlotId[1];
    peachFlags = wp->peachFlags;
    kpaWork = wp->kpaWork;
    trigFlags = wp->trigFlags;
    motion = wp->currMotionId;
    f1A8 = wp->displayDirection;
    f1AC = wp->displayDirectionTarget;
    f1A0 = wp->directionView;
    f19C = wp->moveDirection;
    t = wp->paperPoseId;
    poseId = t;
    prev0 = wp->prevPartyMemberId[0];
    prev1 = wp->prevPartyMemberId[1];
    anim0 = wp->paperAnimGroupId[0];
    anim1 = wp->paperAnimGroupId[1];
    anim2 = wp->paperAnimGroupId[2];
    if (t >= 0) {
        animPoseSetPaperAnimGroup(anim0, NULL, FALSE);
        animPoseRelease(wp->paperPoseId);
        poseId = -1;
        wp->paperPoseId = poseId;
    }
    memset(wp, 0, sizeof(MarioWork));
    marioSoundInit();
    if (dotFlag) {
        wp->flags |= MARIO_FLAG_IS_8_BIT;
    }
    wp->peachFlags = peachFlags;
    wp->kpaWork = kpaWork;
    wp->camId = camId;
    wp->characterId = charId;
    wp->trigFlags = trigFlags;
    wp->currMotionId = motion;
    wp->displayDirection = f1A8;
    wp->displayDirectionTarget = f1AC;
    wp->directionView = f1A0;
    wp->moveDirection = f19C;
    wp->prevPartyMemberId[0] = prev0;
    wp->prevPartyMemberId[1] = prev1;
    wp->currPartySlotId[0] = slot0;
    wp->currPartySlotId[1] = slot1;
    wp->paperAnimGroupId[0] = anim0;
    wp->paperAnimGroupId[1] = anim1;
    wp->paperAnimGroupId[2] = anim2;
    wp->paperPoseId = poseId;
    if (!wp->characterId) {
        wp->mBaseWalkSpeed = 1.0f;
        wp->mBaseDashSpeed = 2.25f;
    } else {
        wp->mBaseWalkSpeed = 1.0f;
        wp->mBaseDashSpeed = 2.0f;
    }
    marioSetSpec();
    wp->flags &= ~MARIO_FLAG_ACTIVE;
    if (wp->paperAnimGroupId[0] >= 0) {
        animPoseSetMaterialFlagOff(wp->paperAnimGroupId[0], -1);
    }
    if (wp->paperAnimGroupId[1] >= 0) {
        animPoseSetMaterialFlagOff(wp->paperAnimGroupId[1], -1);
    }
    if (wp->paperAnimGroupId[2] >= 0) {
        animPoseSetMaterialFlagOff(wp->paperAnimGroupId[2], -1);
    }
    if (wp->paperAnimGroupId[0] >= 0) {
        animPoseSetMaterialLightFlagOn(wp->paperAnimGroupId[0], 2);
    }
    if (wp->paperAnimGroupId[1] >= 0) {
        animPoseSetMaterialLightFlagOn(wp->paperAnimGroupId[1], 2);
    }
    if (wp->paperAnimGroupId[2] >= 0) {
        animPoseSetMaterialLightFlagOn(wp->paperAnimGroupId[2], 2);
    }
    wp->scale = (Vec){1.0f, 1.0f, 1.0f};
    wp->bottomlessResetPosition = (Vec){0.0f, 74.0f, 0.0f};
    partyReInit();
    marioBaloonInit();
    marioGetRubInit();
    blurPosId = 0;
    blurPos[0] = mp->position;
    blurPos[1] = mp->position;
    blurPos[2] = mp->position;
    blurPos[3] = mp->position;
    blurPos[4] = mp->position;
    blurPos[5] = mp->position;
    blurPos[6] = mp->position;
    blurPos[7] = mp->position;
    if (!wp->characterId) {
        marioSetCharMode(CHARACTER_MARIO);
    }
    if (wp->characterId == CHARACTER_BOWSER) {
        marioSetCharMode(CHARACTER_BOWSER);
    }
    marioResetCamShift();
    marioResetCamShiftRate();
    marioResetCamFollowRate();
    if (!pouchCheckItem(ITEM_HAMMER)) {
        pouchGetItem(ITEM_HAMMER);
    }
    wp->returnPos = (Vec){0.0f, -10000.0f, 0.0f};
}

#ifdef VERSION_G8ME01
#define mario_kemuri_80130474 mario_kemuri // US: global name
#endif
void mario_kemuri_80130474(void);

static inline void marioMainBlur(MarioWork* wp) {
    if ((wp->frameCounter & 1) && (mp->dispFlags & MARIO_DISP_FLAG_BLUR)) {
        blurPos[blurPosId] = mp->position;
        if (++blurPosId >= 8) {
            blurPosId -= 8;
        }
    }
}

static inline void marioMainEnd(MarioWork* wp) {
    wp->prevAirTimer = wp->airTimer;
    wp->prevHitobjStandOn = wp->hitobjStandOn;
    if (pouchGetHP() <= 0) {
        seqSetSeq(SEQ_GAME_OVER, NULL, NULL);
    }
    wp->basePos = wp->position;
    wp->floorMove.x = 0.0f;
    wp->floorMove.y = 0.0f;
    wp->floorMove.z = 0.0f;
    wp->frameCounter++;
}

static inline void marioColorChk(void) {
    s32 color;

    if (!mp->characterId) {
        color = marioBadgeColor();
        if (!mp->characterId && mp->colorId != color) {
            mp->colorId = color;
            marioSetCharMode(CHARACTER_MARIO);
        }
    }
}

static inline void marioCoinDropSub(Vec pos) {
    itemCoinDrop(&pos);
}

static inline BOOL marioChkRollMot2(void) {
    BOOL ret = FALSE;
    if (mp->currMotionId == MARIO_MOTION_ROLL) {
        ret = TRUE;
    }
    return ret;
}

void marioMain(void) {
    MarioWork* wp = mp;
    MarioWork* p;
    u32 flags;
    f32 y;
    f32 c, b, a;

    if (!(wp->flags & MARIO_FLAG_ACTIVE)) {
        return;
    }
    if (N_marioChkUseParty()) {
        marioColorChk();
        p = mp;
        if (pouchEquipCheckBadge(ITEM_SLOW_GO)) {
            if (p->currMotionId == MARIO_MOTION_STAY) {
                p->trigFlags |= MARIO_TRIG_FLAG_CHG_ANIM;
            } else if (p->currMotionId == MARIO_MOTION_WALK) {
                p->trigFlags |= MARIO_TRIG_FLAG_CHG_ANIM;
            } else if (p->currMotionId == MARIO_MOTION_DASH) {
                marioChgMot(MARIO_MOTION_WALK);
                p->trigFlags |= MARIO_TRIG_FLAG_CHG_ANIM;
            }
        }
    }
    marioSetDirEventMain();
    wp->playerGravity = 1.0f;
    if (!strcmp(gp->currentMapName, "moo_00") || !strcmp(gp->currentMapName, "moo_01") ||
        !strcmp(gp->currentMapName, "moo_02") || !strcmp(gp->currentMapName, "moo_05") ||
        !strcmp(gp->currentMapName, "moo_06") || !strcmp(gp->currentMapName, "moo_07")) {
        wp->playerGravity = 0.5f;
    }
    marioPreCamera();
    marioHitCheckWidth();
    if (wp->mutekiTimer > 0) {
        wp->mutekiTimer--;
    }
    if (wp->coinDropTimer > 0) {
        wp->coinDropTimer--;
        if (wp->coinDropTimer % 30 == 0 && wp->coinDropTimer > 0) {
            Vec drop;
            Vec pos = {0.0f, 0.0f, 0.0f};
            pos.x = wp->position.x;
            pos.y = 30.0f + wp->position.y;
            pos.z = wp->position.z;
            drop = pos;
            itemCoinDrop(&drop);
        }
    }
    if (wp->zoomOffTimer > 0) {
        if (--wp->zoomOffTimer <= 0) {
            wp->dispFlags &= ~MARIO_DISP_FLAG_ZOOM;
        }
    }
    if (wp->currMotionId == MARIO_MOTION_DOKAN) {
        marioMotion();
        mp->hitobjStandOn = marioSearchGround(&a, &b, &c, 0.0f, 0.0f);
        marioPreDisp();
        partyMain();
        marioCamMain();
        wp = mp;
        if ((wp->frameCounter & 1) && (wp->dispFlags & MARIO_DISP_FLAG_BLUR)) {
            blurPos[blurPosId] = wp->position;
            if (++blurPosId >= 8) {
                blurPosId -= 8;
            }
        }
        marioMainEnd(wp);
        return;
    }
    if (wp->flags & MARIO_FLAG_CTRL_OFF) {
        if (wp->flags & MARIO_FLAG_CTRL_OFF_2) {
            marioCtrlOff2Main();
        }
    } else {
        marioUpdateKeyData();
        marioGetStick(&wp->controlStickAngle, &wp->controlStickSensitivity);
        marioUseParty();
        marioMotion();
        if (wp->characterId == CHARACTER_BOWSER) {
            kpaMain();
            marioMainEnd(mp);
            return;
        }
        if (wp->characterId == CHARACTER_PEACH) {
            peachMain();
            return;
        }
        if (wp->currMotionId == MARIO_MOTION_PLANE || wp->currMotionId == MARIO_MOTION_DAMAGE || wp->currMotionId == MARIO_MOTION_DAMAGE_TOGE || wp->currMotionId == MARIO_MOTION_BOTTOMLESS) {
            marioSearchUnder();
            marioPreDisp();
            marioBottomless();
            partyMain();
            marioCamMain();
            wp->hitobjHead = NULL;
            wp->hitobjFront = NULL;
            wp->hitobjFrontA = NULL;
            wp->hitobjPush = NULL;
            wp->hitobjWall = NULL;
            marioMainEnd(mp);
            return;
        }
        if (wp->currMotionId == MARIO_MOTION_VIVIAN) {
            marioChkGnd();
            marioChkToge();
            marioPreDisp();
            marioBalloonMain();
            marioBottomless();
            partyMain();
            marioCamMain();
            marioMainEnd(mp);
            return;
        }
        if (wp->currMotionId == MARIO_MOTION_JABARA) {
            marioMove();
            marioChkToge();
            marioPreDisp();
            marioBalloonMain();
            marioBottomless();
            partyMain();
            marioCamMain();
            marioMainEnd(mp);
            return;
        }
        if (wp->currMotionId == MARIO_MOTION_YOSHI) {
            marioChkToge();
            marioBottomless();
            marioPreDisp();
            marioBalloonMain();
            mario_kemuri_80130474();
            partyMain();
            marioCamMain();
            marioMainEnd(mp);
            return;
        }
        if (wp->currMotionId == MARIO_MOTION_JUMP_3) {
            marioBottomless();
            marioPreDisp();
            marioBalloonMain();
            mario_kemuri_80130474();
            partyMain();
            marioCamMain();
            marioMainBlur(wp);
            marioMainEnd(mp);
            return;
        }
        flags = wp->flags;
        if (flags & 0x40000) {
            marioMove();
            marioChkOverhead();
            if (wp->airTimer > 100) {
                marioChgMot(MARIO_MOTION_FALL);
                marioSetFallPara();
            }
        } else if (flags & 0x10000) {
            marioJump();
            marioChkOverhead();
            marioMove();
            if (!marioChkRollMot2() && wp->airTimer > 5) {
                marioChkGnd2();
            }
        } else if (flags & 0x20000) {
            if (wp->currMotionId != MARIO_MOTION_SHIP) {
                marioMove();
                if (wp->currMotionId == MARIO_MOTION_HIP) {
                    y = marioChkOverhead();
                    if (wp->hitobjHead) {
                        wp->position.y = y;
                    }
                } else {
                    marioChkOverhead();
                }
                marioFall();
                if (wp->currMotionId != MARIO_MOTION_HIP && wp->currMotionId != MARIO_MOTION_HIP_2) {
                    if (!marioChkRollMot2() && wp->airTimer > 5) {
                        marioChkGnd2();
                    }
                } else {
                    marioChkGnd2();
                }
            }
        } else {
            marioMove();
            marioChkOverhead();
            marioChkGnd();
        }
        marioBoots();
    }
    marioForceMoveMain();
    marioBottomless();
    marioPreDisp();
    marioBalloonMain();
    mario_kemuri_80130474();
    partyMain();
    marioCamMain();
    marioMainBlur(wp);
    marioMainEnd(mp);
}

static inline BOOL marioChkRollMot(void) {
    BOOL ret = FALSE;
    if (mp->currMotionId == MARIO_MOTION_ROLL) {
        ret = TRUE;
    }
    return ret;
}

void marioCtrlOff2Main(void) {
    MarioWork* wp = mp;
    MarioMotion mot = wp->currMotionId;
    f32 y;

    if (mot == MARIO_MOTION_PLANE) return;
    if (mot == MARIO_MOTION_SHIP) return;
    if (mot == MARIO_MOTION_DAMAGE || mot == MARIO_MOTION_DAMAGE_TOGE || mot == MARIO_MOTION_BOTTOMLESS ||
        mot == MARIO_MOTION_JABARA || mot == MARIO_MOTION_YOSHI ||
        mot == MARIO_MOTION_JUMP_3 || mot == MARIO_MOTION_JUMP_STAND) {
        return;
    }
    if (vivianGetStatus()) {
        return;
    }
    marioMotion();
    if (wp->flags & MARIO_FLAG_IS_STEPPING) {
        marioMove();
        marioChkOverhead();
        if (wp->airTimer > 100) {
            marioChgMot(MARIO_MOTION_FALL);
            marioSetFallPara();
        }
    } else if (wp->flags & MARIO_FLAG_IS_JUMPING) {
        marioJump();
        marioChkOverhead();
        marioMove();
        if (!marioChkRollMot() && wp->airTimer > 5) {
            marioChkGnd2();
        }
    } else if (wp->flags & MARIO_FLAG_IS_FALLING) {
        if (wp->currMotionId == MARIO_MOTION_SHIP) {
            return;
        }
        marioMove();
        if (wp->currMotionId == MARIO_MOTION_HIP) {
            y = marioChkOverhead();
            if (wp->hitobjHead) {
                wp->position.y = y;
            }
        } else {
            marioChkOverhead();
        }
        marioFall();
        if (wp->currMotionId != MARIO_MOTION_HIP && wp->currMotionId != MARIO_MOTION_HIP_2) {
            if (!marioChkRollMot() && wp->airTimer > 5) {
                marioChkGnd2();
            }
        } else {
            marioChkGnd2();
        }
        marioChkWallAround(&wp->position, 0, 0.0f, 0.0f, wp->wPlayerCollisionBox.z, wp->wPlayerCollisionBox.y);
    } else {
        marioChkGnd();
    }
}


static inline void marioMove_screenPos(void) {
    MarioWork* p = mp;
    f32 x1, y1, z1;
    marioGetScreenPos(p->position, &x1, &y1, &z1);
    p->screenX = x1;
    p->screenY = y1;
    p->screenZ = z1;
}

void marioMove(void) {
    MarioWork* wp = mp;
    Vec prev;
    f32 x2, y2, z2;
    s32 stick;

    prev = wp->position;
    marioMoveMain();
    marioMove_screenPos();
    if (wp->dispFlags & MARIO_DISP_FLAG_ZOOM) {
        if (wp->controlStickSensitivity || !(wp->dispFlags & MARIO_DISP_FLAG_SLIT)) {
            marioCamZoomOff();
        }
    }
    if (wp->flags & MARIO_FLAG_BG_MODE) {
        if (strcmp(gp->currentMapName, "aji_13")) {
            if (wp->controlStickSensitivity && !(wp->dispFlags & MARIO_DISP_FLAG_CAM_FOLLOW_Y_OFF)) {
                camFollowYOff();
            }
        } else {
            marioGetScreenPos(wp->position, &x2, &y2, &z2);
            if (y2 >= 400.0f) {
                camFollowYOn();
            }
        }
        if (wp->currMotionId == MARIO_MOTION_WALK || wp->currMotionId == MARIO_MOTION_DASH) {
            if (0.0f != prev.x - wp->position.x) {
                if (PSVECDistance(&prev, &wp->position) <= 1.0f) {
                    marioChgPose("M_W_1");
                } else {
                    marioChgPose("M_R_1");
                }
            } else {
                stick = (s8)wp->wStickDir1;
                stick = stick * stick;
                if (stick < 25) {
                    marioChgPose("M_Z_1");
                } else if (stick < 3025) {
                    marioChgPose("M_W_1");
                } else {
                    marioChgPose("M_R_1");
                }
            }
        }
    }
}

void marioMoveMain(void) {
    MarioWork* wp = mp;
    PartyEntry* party;
    f32 vy;
    f32 sn;
    f32 spd;
    f32 vz;
    f32 cs;
    f32 dist;
    f32 vx;
    f32 vz2;

    if (wp->currMotionId != MARIO_MOTION_SHIP) {
        if (wp->flags & MARIO_FLAG_TUBE_MODE) {
            wp->hitobjHead = NULL;
        }
        wp->hitobjFront = NULL;
        wp->hitobjFrontA = NULL;
        wp->hitobjPush = NULL;
        wp->hitobjWall = NULL;
    }
    if (wp->flags & MARIO_FLAG_CTRL_OFF) {
        return;
    }
    if (wp->currMotionId == MARIO_MOTION_JUMP_3) {
        return;
    }
    if (getRollEvtFlag()) {
        return;
    }
    if (wp->flags & MARIO_FLAG_FORCED_MOVEMENT) {
        if (wp->currMotionId == MARIO_MOTION_YOSHI) {
            party = partyGetPtr(marioGetPartyId());
            wp->moveDirection = wp->directionView;
            movePos(&party->position.x, &party->position.z, wp->baseSpeed, wp->directionView);
            wp->position.x = party->position.x;
            wp->position.z = party->position.z;
        } else if (wp->currMotionId == MARIO_MOTION_ROLL) {
            marioSetRollDispDir();
            movePos(&wp->position.x, &wp->position.z, wp->baseSpeed, wp->directionView);
        } else if (wp->currMotionId == MARIO_MOTION_SHIP) {
            marioShipSetDispDir();
            movePos(&wp->position.x, &wp->position.z, wp->baseSpeed, wp->directionView);
        } else if (wp->currMotionId != MARIO_MOTION_SLIT) {
            wp->moveDirection = wp->directionView;
            movePos(&wp->position.x, &wp->position.z, wp->baseSpeed, wp->directionView);
        } else {
            movePos(&wp->position.x, &wp->position.z, wp->baseSpeed, wp->directionView);
        }
        return;
    }
    if (wp->currMotionId == MARIO_MOTION_SHIP) {
        return;
    }
    if (L_marioChkRub()) {
        return;
    }
    vx = 0.0f;
    vz = vx;
    if (!(wp->flags & MARIO_FLAG_STOP_AIR_MOVEMENT) && wp->airTimer) {
        spd = wp->controlStickSensitivity * wp->playerGravity;
        if (wp->flags & MARIO_FLAG_PAPER_MODE) {
            if (wp->currMotionId != MARIO_MOTION_JUMP_STAND) {
                spd = vx;
            } else {
                spd *= 0.015f;
            }
        } else if (pouchEquipCheckBadge(ITEM_SLOW_GO)) {
            spd *= 0.01f;
        } else if (marioChkRollMot() == TRUE) {
            spd *= 0.01f;
        } else {
            spd *= 0.015f;
            if (!(wp->flags & (MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING | MARIO_FLAG_IS_STEPPING))) {
                spd *= 0.25f;
            }
            if (wp->flags & MARIO_FLAG_BG_MODE) {
                spd *= 0.4f;
            }
        }
        sincosf(&sn, &cs, wp->controlStickAngle);
        vx = spd * sn;
        vz = spd * cs;
    }
    spd = wp->baseSpeed;
    if (wp->hitobjStandOn) {
        if (hitGetAttr(wp->hitobjStandOn) & HIT_ATTR_DAMAGE_RETURN) {
            wp->damageHit = wp->hitobjStandOn;
        }
        if (hitGetAttr(wp->hitobjStandOn) & HIT_ATTR_SHALLOW_WATER) {
            wp->flags |= MARIO_FLAG_IN_SHALLOW_WATER;
            spd *= 0.75f;
        } else {
            wp->flags &= ~MARIO_FLAG_IN_SHALLOW_WATER;
        }
    } else {
        wp->flags &= ~MARIO_FLAG_IN_SHALLOW_WATER;
    }
    sincosf(&sn, &cs, wp->directionView);
    vx += spd * sn;
    vz2 = vz + spd * cs;
    if (!(wp->flags & (MARIO_FLAG_HAS_INPUT_JUMP | MARIO_FLAG_BG_MODE))) {
        if (wp->groundRot.z != 0.0f) {
            vx *= (f32)cos(3.1415927f * wp->groundSlope / 180.0f);
        }
        if (wp->groundRot.x != 0.0f) {
            vz2 *= (f32)cos(3.1415927f * wp->groundSlope / 180.0f);
        }
    }
    vx += wp->floorMove.x;
    vz2 += wp->floorMove.z;
    vy = 0.0f;
    wp->position.y += vy + wp->floorMove.y;
    dist = sqrtf(vx * vx + vz2 * vz2);
    if (dist != vy) {
        spd = angleABf(0.0f, 0.0f, vx, vz2);
    } else {
        spd = toMovedir(wp->displayDirection);
    }
    if (!(wp->flags & MARIO_FLAG_PAPER_MODE) && (wp->flags & (MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING))) {
        vx = wp->mBaseDashSpeed * wp->playerGravity;
        if (dist > vx) {
            dist = vx;
        }
    }
    if (!(wp->flags & MARIO_FLAG_TUBE_MODE)) {
        dist *= marioGetMoveRate(wp->moveDirection);
    }
    if (wp->flags & MARIO_FLAG_PRE_BATTLE_PHYSICS) {
        dist = 0.0f;
    }
    wp->moveDist = dist;
    if (wp->flags & MARIO_FLAG_USE_FORCE_DIRECTION) {
        spd = wp->forceDir;
        wp->directionView = spd;
    } else if (wp->flags & MARIO_FLAG_WALLS_DISABLED) {
        spd = wp->directionView;
    }
    if (wp->flags & MARIO_FLAG_WALLS_DISABLED) {
        movePos(&wp->position.x, &wp->position.z, wp->baseSpeed, spd);
        return;
    }
    if (!(wp->flags & MARIO_FLAG_TUBE_MODE)) {
        marioCheckWall(dist, spd);
    } else {
        if (!(wp->flags & MARIO_FLAG_IS_JUMPING)) {
            marioCheckWall(dist, spd);
        } else {
            marioCheckWall_roll(dist, spd);
        }
        N_dou10_yoko_yari3(&wp->frontWallDist);
        if (wp->hitobjFront && (hitGetAttr(wp->hitobjFront) & HIT_ATTR_TOGE)) {
            set_damage_root_ypos(wp->position.y);
            marioChgMot(MARIO_MOTION_DAMAGE_TOGE);
            return;
        }
    }
    marioChkToge();
}

BOOL marioChkPushAnime(void) {
    MarioWork* wp = mp;
    BOOL valid = FALSE;
    if (!strcmp(wp->animName, "M_O_1") || !strcmp(wp->animName, "M_O_2") || !strcmp(wp->animName, "M_O_2R")) {
        valid = TRUE;
    }
    return valid;
}

s32 marioAnimeId(void) {
    MarioWork* wp = mp;
    s32 id;
    if (wp->dispFlags & MARIO_DISP_FLAG_EVT_POSE) {
        id = 2;
    } else if (wp->dispFlags & MARIO_DISP_FLAG_REAR_POSE) {
        id = 1;
    } else {
        id = 0;
    }
    return id;
}

void marioChgDamagePose(void) {
    MarioWork* wp = mp;
    MarioMotion mot = wp->currMotionId;

    if (mot != MARIO_MOTION_SHIP && mot != MARIO_MOTION_ROLL && mot != MARIO_MOTION_PLANE && mot != MARIO_MOTION_JABARA && mot != MARIO_MOTION_YOSHI) {
        switch (wp->dispFlags & MARIO_DISP_FLAG_SLIT) {
        case 0:
            marioChgPose("M_D_1");
            break;
        }
    }
}

void marioChgEvtPose(const char* name) {
    MarioWork* wp = mp;
    const char* animName;

    if (!(mp->dispFlags & MARIO_DISP_FLAG_POSE_LOCK) && !(mp->flags & MARIO_FLAG_IS_8_BIT)) {
        animName = mp->animName;
        if (!animName || strcmp(animName, name)) {
            wp->animName = name;
            wp->dispFlags &= ~0x20000000;
            wp->dispFlags |= MARIO_DISP_FLAG_EVT_POSE;
            wp->trigFlags |= MARIO_TRIG_FLAG_CHG_ANIM;
        }
    }
}

const char* toDotMarioPose(const char* name) {
    const char* dot = NULL;

    if (!strcmp("M_Z_1", name)) {
        dot = dotMarioPose[mp->colorId][0];
    } else if (!strcmp("M_S_1", name)) {
        dot = dotMarioPose[mp->colorId][1];
    } else if (!strcmp("M_W_1", name)) {
        dot = dotMarioPose[mp->colorId][2];
    } else if (!strcmp("M_R_1", name)) {
        dot = dotMarioPose[mp->colorId][3];
    } else if (!strcmp("M_J_1B", name)) {
        dot = dotMarioPose[mp->colorId][5];
    } else if (!strcmp("M_J_1C", name)) {
        dot = dotMarioPose[mp->colorId][5];
    }

    return dot;
}

void marioChgPose(const char* name) {
    MarioWork* wp = mp;
    const char* dot;

    if (!(wp->dispFlags & MARIO_DISP_FLAG_POSE_LOCK)) {
        if (!wp->animName || strcmp(wp->animName, name)) {
            if (!(wp->flags & MARIO_FLAG_IS_8_BIT)) {
                wp->animName = name;
                wp->poseTime = 0;
                wp->dispFlags &= ~0x30000000;
                wp->trigFlags |= MARIO_TRIG_FLAG_CHG_ANIM;
            } else {
                dot = toDotMarioPose(name);
                if (dot) {
                    wp->animName = dot;
                    wp->poseTime = 0;
                    wp->dispFlags &= ~0x30000000;
                    wp->trigFlags |= MARIO_TRIG_FLAG_CHG_ANIM;
                }
            }
        }
    }
}

void marioChgPoseTime(const char* name, s16 frames) {
    MarioWork* wp = mp;
    const char* animName;

    if (!(mp->dispFlags & MARIO_DISP_FLAG_POSE_LOCK) && !(mp->flags & MARIO_FLAG_IS_8_BIT)) {
        animName = mp->animName;
        if (!animName || strcmp(animName, name)) {
            wp->animName = name;
            wp->poseTime = frames;
            wp->dispFlags &= ~0x30000000;
            wp->trigFlags |= MARIO_TRIG_FLAG_CHG_ANIM;
        }
    }
}

void marioChgPaper(const char* name) {
    MarioWork* wp = mp;
    s32 flags = wp->dispFlags;
    const char* animName;

    if (flags & 0x40000000 && !(flags & 2) && !(mp->flags & MARIO_FLAG_IS_8_BIT)) {
        animName = mp->paperAnimName;
        if (!animName || strcmp(animName, name)) { // only when the animation changes
            wp->paperAnimName = name;
            wp->poseTime = 0;
            wp->trigFlags |= MARIO_TRIG_FLAG_CHG_PAPER_ANIM;
        }
    }
}

void marioSetPaperAnimeLocalTime(s32 frames) {
    MarioWork* wp = mp;

    wp->poseTime = frames;
    animPoseSetLocalTime(wp->paperPoseId, frames);
    animPoseSetLocalTime(wp->paperAnimGroupId[marioAnimeId()], frames);
}

inline void marioPaperRelease(void) {
    MarioWork* wp = mp;
    s32 poseId = wp->paperPoseId;

    if (poseId >= 0) {
        animPaperPoseRelease(poseId);
        wp->paperPoseId = -1;
    }
}

BOOL marioPaperOn(const char* name) {
    BOOL flag;
    MarioWork* wp = mp;

    if (wp->paperPoseId >= 0) {
        animPaperPoseRelease(wp->paperPoseId);
        wp->paperPoseId = -1;
    }
    wp->dispFlags |= MARIO_DISP_FLAG_PAPER; //paper on
    wp->paperPoseId = animPaperPoseEntry(name, 2);
    flag = (!strcmp(name, "p_roll")
        || !strcmp(name, "p_plane")
        || !strcmp(name, "p_hune")
        || !strcmp(name, "p_jyabara")
        || !strcmp(name, "p_dokan_x")
        || !strcmp(name, "p_dokan_y"));
    animPoseSetPaperAnimGroup(wp->paperAnimGroupId[0], name, flag == 0);
    return TRUE;
}

void marioPaperOff(void) {
    MarioWork* wp = mp;

    if (wp->dispFlags & MARIO_DISP_FLAG_PAPER) {
        if (wp->paperAnimName) {
            animPoseSetPaperAnimGroup(mp->paperAnimGroupId[0], NULL, FALSE);
            wp->paperAnimName = 0;
            wp->dispFlags &= ~MARIO_DISP_FLAG_PAPER;
            marioPaperRelease();
        }
    }
}

void marioPaperLightOn(void) {
    animPoseSetMaterialLightFlagOn(mp->paperAnimGroupId[0], 1);
}

void marioPaperLightOff(void) {
    animPoseSetMaterialLightFlagOff(mp->paperAnimGroupId[0], 1);
}

void marioGetScreenPos(Vec position, f32* x, f32* y, f32* z) {
    CameraEntry* camera;
    f32 proj[7];
    f32 view[6];

    camera = camGetPtr(CAMERA_3D);
    GXSetProjection(camera->projection, camera->projType);
    GXGetProjectionv(proj);
    GXGetViewportv(view);
    GXProject(position.x, position.y, position.z, camera->view, proj, view, x, y, z);
}

BOOL marioChkInScreen(s32 x, s32 y) {
    BOOL valid = FALSE;
    if (x >= 0 && x <= 600 && y >= 0 && y <= 535) {
        valid = TRUE;
    }
    return valid != FALSE; //why
}

f32 marioGetScale(void) {
    return mp->flags & MARIO_FLAG_BG_MODE ? 1.2f : 2.0f;
}

void marioMakeDispDir(void) {
    MarioWork* wp = mp;
    f32 diff;
    f32 tgt;
    f32 step;
    f32 c30 = 30.0f;
    f32 c20 = 20.0f;

    if (wp->dispFlags & MARIO_DISP_FLAG_SLIT) {
        return;
    }
    if (!(wp->dispFlags & MARIO_DISP_FLAG_NO_MAKE_DISP_DIR)) {
        diff = revise360(wp->moveDirection - wp->targetYaw);
        tgt = (s32)(0.5f + wp->displayDirectionTarget);
        if (!(wp->dispFlags & MARIO_DISP_FLAG_SLIT)) {
            if (290.0f == tgt || 250.0f == tgt) {
                tgt = 180.0f;
                if (diff >= tgt) {
                    tgt = 0.0f;
                }
            }
        }
        if (wp->baseSpeed) {
            if (diff >= 190.0f && diff <= 350.0f) {
                tgt = 0.0f;
            } else if (diff >= 10.0f && diff <= 170.0f) {
                tgt = 180.0f;
            }
        }
        wp->displayDirectionTarget = tgt;
        if (wp->dispFlags & MARIO_DISP_FLAG_DISP_DIR_SNAP) {
            wp->displayDirection = wp->displayDirectionTarget;
        }
        if (wp->displayDirection == wp->displayDirectionTarget) {
            return;
        }
    }
    if (wp->currMotionId == MARIO_MOTION_HAMMER) {
        if (wp->displayDirection <= 180.0f) {
            if (0.0f == wp->displayDirectionTarget) {
                wp->displayDirection = 0.0f;
                wp->trigFlags &= ~MARIO_TRIG_FLAG_DISP_DIR_CORRECTION;
            } else if (180.0f == wp->displayDirectionTarget) {
                wp->displayDirection = 180.0f;
                wp->trigFlags &= ~MARIO_TRIG_FLAG_DISP_DIR_CORRECTION;
            }
        } else if (wp->displayDirection > 180.0f) {
            if (0.0f == wp->displayDirectionTarget) {
                wp->displayDirection = 0.0f;
                wp->trigFlags &= ~MARIO_TRIG_FLAG_DISP_DIR_CORRECTION;
            } else {
                wp->displayDirection = 180.0f;
                wp->trigFlags &= ~MARIO_TRIG_FLAG_DISP_DIR_CORRECTION;
            }
        }
        return;
    }
    if (wp->displayDirection <= 180.0f) {
        tgt = (s32)wp->displayDirectionTarget;
        if (0.0f == tgt) {
            if (wp->trigFlags & MARIO_TRIG_FLAG_DISP_DIR_RIGHT) {
                wp->trigFlags &= ~MARIO_TRIG_FLAG_DISP_DIR_RIGHT;
                wp->trigFlags &= ~MARIO_TRIG_FLAG_DISP_DIR_CORRECTION;
            }
            wp->trigFlags |= MARIO_TRIG_FLAG_DISP_DIR_LEFT;
            if (wp->trigFlags & MARIO_TRIG_FLAG_DISP_DIR_CORRECTION) {
                step = 0.3f * (10.0f - wp->displayDirection);
            } else {
                step = 0.125f * (-c30 - wp->displayDirection);
            }
            wp->displayDirection += step;
            if (!(wp->trigFlags & MARIO_TRIG_FLAG_DISP_DIR_CORRECTION)) {
                if (wp->displayDirection < -c20) {
                    wp->trigFlags |= MARIO_TRIG_FLAG_DISP_DIR_CORRECTION;
                }
            } else if (wp->displayDirection >= 0.0f) {
                wp->displayDirection = 0.0f;
                wp->trigFlags &= ~MARIO_TRIG_FLAG_DISP_DIR_CORRECTION;
            }
        } else if (180.0f == tgt) {
            if (wp->trigFlags & MARIO_TRIG_FLAG_DISP_DIR_LEFT) {
                wp->trigFlags &= ~MARIO_TRIG_FLAG_DISP_DIR_LEFT;
                wp->trigFlags &= ~MARIO_TRIG_FLAG_DISP_DIR_CORRECTION;
            }
            wp->trigFlags |= MARIO_TRIG_FLAG_DISP_DIR_RIGHT;
            if (wp->trigFlags & MARIO_TRIG_FLAG_DISP_DIR_CORRECTION) {
                step = 0.3f * (170.0f - wp->displayDirection);
            } else {
                step = 0.125f * (210.0f - wp->displayDirection);
            }
            wp->displayDirection += step;
            if (!(wp->trigFlags & MARIO_TRIG_FLAG_DISP_DIR_CORRECTION)) {
                if (wp->displayDirection > 200.0f) {
                    wp->trigFlags |= MARIO_TRIG_FLAG_DISP_DIR_CORRECTION;
                }
            } else if (wp->displayDirection <= 180.0f) {
                wp->displayDirection = 180.0f;
                wp->trigFlags &= ~MARIO_TRIG_FLAG_DISP_DIR_CORRECTION;
            }
        }
    } else if (wp->displayDirection > 180.0f) {
        if (0.0f == wp->displayDirectionTarget) {
            if (wp->trigFlags & MARIO_TRIG_FLAG_DISP_DIR_RIGHT) {
                wp->trigFlags &= ~MARIO_TRIG_FLAG_DISP_DIR_RIGHT;
                wp->trigFlags &= ~MARIO_TRIG_FLAG_DISP_DIR_CORRECTION;
            }
            wp->trigFlags |= MARIO_TRIG_FLAG_DISP_DIR_LEFT;
            if (wp->trigFlags & MARIO_TRIG_FLAG_DISP_DIR_CORRECTION) {
                step = 0.3f * (10.0f - wp->displayDirection);
            } else {
                step = 0.125f * (-c30 - wp->displayDirection);
            }
            wp->displayDirection += step;
            if (!(wp->trigFlags & MARIO_TRIG_FLAG_DISP_DIR_CORRECTION)) {
                if (wp->displayDirection < -c20) {
                    wp->trigFlags |= MARIO_TRIG_FLAG_DISP_DIR_CORRECTION;
                }
            } else if (wp->displayDirection >= 0.0f) {
                wp->displayDirection = 0.0f;
                wp->trigFlags &= ~MARIO_TRIG_FLAG_DISP_DIR_CORRECTION;
            }
        } else if (180.0f == wp->displayDirectionTarget) {
            if (wp->trigFlags & MARIO_TRIG_FLAG_DISP_DIR_LEFT) {
                wp->trigFlags &= ~MARIO_TRIG_FLAG_DISP_DIR_LEFT;
                wp->trigFlags &= ~MARIO_TRIG_FLAG_DISP_DIR_CORRECTION;
            }
            wp->trigFlags |= MARIO_TRIG_FLAG_DISP_DIR_RIGHT;
            if (wp->trigFlags & MARIO_TRIG_FLAG_DISP_DIR_CORRECTION) {
                step = 0.3f * (170.0f - wp->displayDirection);
            } else {
                step = 0.125f * (210.0f - wp->displayDirection);
            }
            wp->displayDirection += step;
            if (!(wp->trigFlags & MARIO_TRIG_FLAG_DISP_DIR_CORRECTION)) {
                if (wp->displayDirection > 200.0f) {
                    wp->trigFlags |= MARIO_TRIG_FLAG_DISP_DIR_CORRECTION;
                } else if (wp->displayDirection <= 180.0f) {
                    wp->displayDirection = 180.0f;
                    wp->trigFlags &= ~MARIO_TRIG_FLAG_DISP_DIR_CORRECTION;
                }
            }
        }
    }
}

void marioRearAnime(void) {
    MarioWork* wp = mp;
    const char* name;
    f32 diff;

    if (!(wp->dispFlags & 0x70000004) && !(wp->flags & MARIO_FLAG_IS_CARRYING_BOBBERY)) {
        diff = revise360(wp->moveDirection - wp->targetYaw);
        if (diff >= 290.0f || diff <= 70.0f) {
            name = toRearPose(wp->animName);
            if (name) {
                wp->dispFlags |= MARIO_DISP_FLAG_REAR_POSE;
                marioChgPose(name);
            } else if (!toFrontPose(wp->animName)) {
                wp->dispFlags &= ~MARIO_DISP_FLAG_REAR_POSE;
            }
        } else {
            wp->dispFlags &= ~MARIO_DISP_FLAG_REAR_POSE;
            name = toFrontPose(wp->animName);
            if (name) {
                marioChgPose(name);
            }
        }
    } else if (wp->dispFlags & MARIO_DISP_FLAG_REAR_POSE) {
        wp->dispFlags &= ~MARIO_DISP_FLAG_REAR_POSE;
        name = toFrontPose(wp->animName);
        if (name) {
            marioChgPose(name);
        }
    }
}

void marioPreDisp(void) {
    MarioWork* wp = marioGetPtr();
    Vec pos;
    f32 size;
    f32 z;
    f32 sz;
    f32 xo;
    u32 bit;
    f32 zo;
    f32 dy;
    s32 t;

    if (!wp->characterId || wp->characterId == CHARACTER_PEACH) {
        marioRearAnime();
    }
    if (!wp->ctrlOffCount && !(wp->dispFlags & MARIO_DISP_FLAG_NO_MAKE_DISP_DIR)) {
        marioMakeDispDir();
    }
    if (wp->trigFlags & (MARIO_TRIG_FLAG_CHG_ANIM | MARIO_TRIG_FLAG_CHG_PAPER_ANIM)) {
        if (wp->trigFlags & MARIO_TRIG_FLAG_CHG_ANIM) {
            wp->trigFlags &= ~MARIO_TRIG_FLAG_CHG_ANIM;
        }
        if (wp->trigFlags & MARIO_TRIG_FLAG_CHG_PAPER_ANIM) {
            wp->trigFlags &= ~MARIO_TRIG_FLAG_CHG_PAPER_ANIM;
        }
        animPoseSetAnim(wp->paperAnimGroupId[marioAnimeId()], wp->animName, TRUE);
        if (wp->paperAnimName) {
            animPoseSetPaperAnim(wp->paperAnimGroupId[0], wp->paperAnimName, TRUE);
        }
        animPoseSetLocalTime(wp->paperAnimGroupId[marioAnimeId()], wp->poseTime);
    }
    animPoseMain(wp->paperAnimGroupId[marioAnimeId()]);
    if (wp->dispFlags & MARIO_DISP_FLAG_HIDE) {
        return;
    }
    pos = wp->position;
    PSMTX44MultVec(camGetPtr(CAMERA_3D)->projection, &pos, &pos);
    z = 5000.0f * pos.z + 5000.0f;
    if (z < 0.0f) {
        z = 0.0f;
    } else if (z > 10000.0f) {
        z = 10000.0f;
    }
    z *= -1.0f;
    if (!(wp->dispFlags & MARIO_DISP_FLAG_BLUR) || (wp->flags & MARIO_FLAG_IS_SHADOW_MARIO)) {
        if (wp->characterId == CHARACTER_BOWSER) {
            t = kpaNoHitEnemyCheck();
        } else {
            t = wp->mutekiTimer;
        }
        if (t > 0) {
            if (!(t & 2)) {
                dispEntry(marioGetCamId(), z, 1, marioDisp, wp);
            }
        } else {
            dispEntry(marioGetCamId(), z, 1, marioDisp, wp);
        }
    } else {
        dispEntry(marioGetCamId(), z, 2, marioDispBlur, wp);
    }
    if (wp->dispFlags & MARIO_DISP_FLAG_SHADOW_OFF) {
        return;
    }
    if (wp->flags & MARIO_FLAG_BG_MODE) {
        return;
    }
    if (wp->currMotionId == MARIO_MOTION_PLANE) {
        shadowEntryMode(2, wp->position.x, wp->groundY, wp->position.z, 13.333333f);
        return;
    }
    if (wp->characterId == CHARACTER_BOWSER) {
        size = 30.0f;
        xo = 14.0f * (f32)sin(3.1415927f * wp->targetYaw / 180.0f);
        dy = (f32)cos(3.1415927f * wp->targetYaw / 180.0f);
        zo = 14.0f * -dy;
    }
#ifdef VERSION_G8ME01
    else if (wp->characterId == CHARACTER_PEACH) {
        size = 13.333333f;
        xo = 6.0f * (f32)sin(3.1415927f * wp->targetYaw / 180.0f);
        dy = (f32)cos(3.1415927f * wp->targetYaw / 180.0f);
        zo = 6.0f * -dy;
    }
#endif
    else {
        xo = 0.0f;
        size = 13.333333f;
        zo = xo;
    }
    if (wp->hitobjStandOn) {
        bit = wp->flags & MARIO_FLAG_BG_MODE;
        if (bit) {
            sz = 0.6f * size;
        } else {
            sz = size;
        }
        t = 2; if (bit) { t = 0; }
        shadowEntryMode(t, wp->position.x + xo, wp->position.y, wp->position.z + zo, sz);
    } else {
        dy = wp->position.y - wp->groundY;
        if (dy >= -2.0f) {
            size -= dy / 13.333333f;
            if (size < 0.1f) {
                size = 0.0f;
            }
            if (size) {
                t = 2;
                if (wp->flags & MARIO_FLAG_BG_MODE) {
                    t = 0;
                }
                shadowEntryMode(t, wp->position.x + xo, wp->position.y, wp->position.z + zo, size);
            }
        }
    }
}

inline BOOL marioChkRevDir(f32 angle) {
    BOOL ret = FALSE;

    if (angle >= 90.0f && angle <= 270.0f) {
        ret = TRUE;
    }
    return ret;
}

void marioDispBlurSub(Mtx unused, s32 blurId) {
    MarioWork* wp = mp;
    Mtx mtx;
    Mtx rotMtx;
    Mtx scaleMtx;
    f32 scale;
    f32 angle;
    f32 yofs = 0.0f;

    scale = marioGetScale();
    MTXScale(mtx, wp->scale.x * scale, wp->scale.y * scale, wp->scale.z * scale);
    if (!(wp->dispFlags & MARIO_DISP_FLAG_NO_DIR_FLIP) && wp->currMotionId == MARIO_MOTION_JABARA) {
        MTXScale(scaleMtx, 1.0f, 1.0f, marioChkRevDir(revise360(wp->rot.x)) ? -1.0f : 1.0f);
        MTXConcat(scaleMtx, mtx, mtx);
    }
    if (!(wp->dispFlags & (MARIO_DISP_FLAG_LEFT_ONLY | MARIO_DISP_FLAG_RIGHT_ONLY))) {
        if (!(wp->dispFlags & MARIO_DISP_FLAG_NO_DIR_FLIP)) {
            MTXScale(scaleMtx, 1.0f, 1.0f, marioChkRevDir(revise360(wp->displayDirection)) ? -1.0f : 1.0f);
            MTXConcat(scaleMtx, mtx, mtx);
        }
    } else {
        if (wp->dispFlags & MARIO_DISP_FLAG_LEFT_ONLY) {
            MTXScale(scaleMtx, 1.0f, 1.0f, 1.0f);
        } else {
            MTXScale(scaleMtx, 1.0f, 1.0f, -1.0f);
        }
        MTXConcat(scaleMtx, mtx, mtx);
    }
    MTXTrans(rotMtx, -wp->rotOfs.x, -wp->rotOfs.y, -wp->rotOfs.z);
    MTXConcat(rotMtx, mtx, mtx);
    if (wp->currMotionId == MARIO_MOTION_PLANE) {
        MTXRotRad(rotMtx, 'x', 0.017453292f * wp->rot.x);
        MTXConcat(rotMtx, mtx, mtx);
        MTXRotRad(rotMtx, 'z', 0.017453292f * wp->rot.z);
        MTXConcat(rotMtx, mtx, mtx);
    } else {
        MTXRotRad(rotMtx, 'z', 0.017453292f * wp->rot.z);
        MTXConcat(rotMtx, mtx, mtx);
        MTXRotRad(rotMtx, 'x', 0.017453292f * wp->rot.x);
        MTXConcat(rotMtx, mtx, mtx);
    }
    MTXRotRad(rotMtx, 'y', 0.017453292f * revise360(wp->displayDirection - wp->targetYaw));
    MTXConcat(rotMtx, mtx, mtx);
    if (wp->dispFlags & (MARIO_DISP_FLAG_LEFT_ONLY | MARIO_DISP_FLAG_RIGHT_ONLY)) {
        angle = revise360(wp->displayDirection);
        if (angle >= 90.0f && angle <= 270.0f) {
            MTXRotRad(rotMtx, 'y', 0.017453292f * ((wp->dispFlags & MARIO_DISP_FLAG_LEFT_ONLY) ? 180.0f : 0.0f));
            MTXConcat(rotMtx, mtx, mtx);
        } else {
            MTXRotRad(rotMtx, 'y', 0.017453292f * ((wp->dispFlags & MARIO_DISP_FLAG_LEFT_ONLY) ? 0.0f : 180.0f));
            MTXConcat(rotMtx, mtx, mtx);
        }
    }
    MTXTrans(rotMtx, wp->rotOfs.x, wp->rotOfs.y, wp->rotOfs.z);
    MTXConcat(rotMtx, mtx, mtx);
    if (wp->flags & MARIO_FLAG_TUBE_MODE) {
        MTXScale(scaleMtx, 1.0f, wp->tubeScaleY, 1.0f);
        MTXConcat(scaleMtx, mtx, mtx);
    }
    if (wp->currMotionId == MARIO_MOTION_YOSHI) {
        MTXRotRad(rotMtx, 'z', 0.017453292f * wp->mot.yoshi.rotZ);
        MTXConcat(rotMtx, mtx, mtx);
    }
    MTXTrans(rotMtx,
        wp->dispOfs2.x + (wp->dispOfs.x + blurPos[blurId].x),
        yofs + (wp->dispOfs2.y + (wp->dispOfs.y + blurPos[blurId].y)),
        wp->dispOfs2.z + (wp->dispOfs.z + blurPos[blurId].z));
    MTXConcat(rotMtx, mtx, mtx);
    animPoseSetMaterialFlagOn(wp->paperAnimGroupId[marioAnimeId()], 0x40);
    animPoseSetMaterialEvtColor(wp->paperAnimGroupId[marioAnimeId()], (GXColor){0xFF, 0xFF, 0xFF, 0x80 - (blurId << 4)});
    animPoseDrawMtx(wp->paperAnimGroupId[marioAnimeId()], mtx, 1, 0.0f, 1.0f);
    animPoseDrawMtx(wp->paperAnimGroupId[marioAnimeId()], mtx, 2, 0.0f, 1.0f);
    animPoseDrawMtx(wp->paperAnimGroupId[marioAnimeId()], mtx, 3, 0.0f, 1.0f);
}

#pragma opt_dead_assignments off
void marioDispBlur(CameraId camId, void* param) {
    MarioWork* wp = param;
    s32 i;
    Mtx mtx;
    u32 flag;
    f32 s, c, sg, cg, sy;
    f32 angle;
    flag = animPoseGetMaterialFlag(wp->paperAnimGroupId[marioAnimeId()]);
    angle = wp->targetYaw * 6.2831855f / 360.0f;
    if (angle <= 3.1415927f) {
        if (angle <= 1.5707964f) {
            s = angle;
            sg = 1.0f;
            sy = s * s;
        } else {
            s = angle - 1.5707964f;
            sg = 1.0f;
            sy = s * s;
        }
    } else if (angle < 4.712389f) {
        s = angle - 3.1415927f;
        sg = -1.0f;
        sy = s * s;
    } else {
        s = angle - 4.712389f;
        sg = -1.0f;
        sy = s * s;
    }
    if (angle <= 3.1415927f) {
        if (angle <= 1.5707964f) {
            c = angle;
            cg = 1.0f;
        } else {
            c = angle - 1.5707964f;
            cg = 1.0f;
        }
    } else if (angle < 4.712389f) {
        c = angle - 3.1415927f;
        cg = -1.0f;
    } else {
        c = angle - 4.712389f;
        cg = -1.0f;
    }
    s = s * sg * c * cg * sy;
    for (i = 7; i > 0; i--) {
        marioDispBlurSub(mtx, i);
    }
    animPoseSetMaterialFlagOff(wp->paperAnimGroupId[marioAnimeId()], 0x40);
    animPoseSetMaterialFlagOn(wp->paperAnimGroupId[marioAnimeId()], flag);
}
#pragma opt_dead_assignments reset

void marioDisp(CameraId camId, void* param) {
    MarioWork* wp = mp;
    Mtx mtx;
    Mtx rotMtx;
    Mtx scaleMtx;
    f32 scale;
    f32 angle;

    scale = marioGetScale();
    MTXScale(mtx, wp->scale.x * scale, wp->scale.y * scale, wp->scale.z * scale);
    if (!(wp->dispFlags & MARIO_DISP_FLAG_NO_DIR_FLIP) && wp->currMotionId == MARIO_MOTION_JABARA) {
        MTXScale(scaleMtx, 1.0f, 1.0f, marioChkRevDir(revise360(wp->rot.x)) ? -1.0f : 1.0f);
        MTXConcat(scaleMtx, mtx, mtx);
    }
    if (!(wp->dispFlags & (MARIO_DISP_FLAG_LEFT_ONLY | MARIO_DISP_FLAG_RIGHT_ONLY))) {
        if (wp->dispFlags & MARIO_DISP_FLAG_FLIP) {
            MTXScale(scaleMtx, 1.0f, 1.0f, -1.0f);
            MTXConcat(scaleMtx, mtx, mtx);
        } else if (!(wp->dispFlags & MARIO_DISP_FLAG_NO_DIR_FLIP)) {
            MTXScale(scaleMtx, 1.0f, 1.0f, marioChkRevDir(revise360(wp->displayDirection)) ? -1.0f : 1.0f);
            MTXConcat(scaleMtx, mtx, mtx);
        }
    } else {
        if (wp->dispFlags & MARIO_DISP_FLAG_LEFT_ONLY) {
            MTXScale(scaleMtx, 1.0f, 1.0f, 1.0f);
        } else {
            MTXScale(scaleMtx, 1.0f, 1.0f, -1.0f);
        }
        MTXConcat(scaleMtx, mtx, mtx);
    }
    MTXTrans(rotMtx, -wp->rotOfs.x, -wp->rotOfs.y, -wp->rotOfs.z);
    MTXConcat(rotMtx, mtx, mtx);
    if (wp->currMotionId == MARIO_MOTION_PLANE) {
        MTXRotRad(rotMtx, 'x', 0.017453292f * wp->rot.x);
        MTXConcat(rotMtx, mtx, mtx);
        MTXRotRad(rotMtx, 'z', 0.017453292f * wp->rot.z);
        MTXConcat(rotMtx, mtx, mtx);
    } else {
        MTXRotRad(rotMtx, 'z', 0.017453292f * wp->rot.z);
        MTXConcat(rotMtx, mtx, mtx);
        MTXRotRad(rotMtx, 'x', 0.017453292f * wp->rot.x);
        MTXConcat(rotMtx, mtx, mtx);
    }
    MTXRotRad(rotMtx, 'y', 0.017453292f * revise360(wp->displayDirection - wp->targetYaw));
    MTXConcat(rotMtx, mtx, mtx);
    if (wp->dispFlags & (MARIO_DISP_FLAG_LEFT_ONLY | MARIO_DISP_FLAG_RIGHT_ONLY)) {
        angle = revise360(wp->displayDirection);
        if (angle >= 90.0f && angle <= 270.0f) {
            MTXRotRad(rotMtx, 'y', 0.017453292f * ((wp->dispFlags & MARIO_DISP_FLAG_LEFT_ONLY) ? 180.0f : 0.0f));
            MTXConcat(rotMtx, mtx, mtx);
        } else {
            MTXRotRad(rotMtx, 'y', 0.017453292f * ((wp->dispFlags & MARIO_DISP_FLAG_LEFT_ONLY) ? 0.0f : 180.0f));
            MTXConcat(rotMtx, mtx, mtx);
        }
    }
    MTXTrans(rotMtx, wp->rotOfs.x, wp->rotOfs.y, wp->rotOfs.z);
    MTXConcat(rotMtx, mtx, mtx);
    if (wp->flags & MARIO_FLAG_TUBE_MODE) {
        MTXScale(scaleMtx, 1.0f, wp->tubeScaleY, 1.0f);
        MTXConcat(scaleMtx, mtx, mtx);
    }
    MTXTrans(rotMtx,
        wp->dispOfs2.x + (wp->position.x + wp->dispOfs.x),
        wp->dispOfs2.y + (wp->position.y + wp->dispOfs.y),
        wp->dispOfs2.z + (wp->position.z + wp->dispOfs.z));
    MTXConcat(rotMtx, mtx, mtx);
    animPoseDrawMtx(wp->paperAnimGroupId[marioAnimeId()], mtx, 2, 0.0f, 1.0f);
    animPoseDrawMtx(wp->paperAnimGroupId[marioAnimeId()], mtx, 1, 0.0f, 1.0f);
    animPoseSetLocalTimeRate(wp->paperAnimGroupId[marioAnimeId()], (wp->flags & MARIO_FLAG_IN_SHALLOW_WATER) ? 0.75f : 1.0f);
}

const char* toRearPose(const char* name) {
    const char* rear = NULL;
    if (mp->characterId == CHARACTER_PEACH && !(mp->peachFlags & PEACH_FLAG_GUNDAN)) {
        if (!strcmp(name, "P_Z_1")) {
            rear = "P_Z_1R";
        } else if (!strcmp(name, "P_S_1")) {
            rear = "P_S_1R";
        } else if (!strcmp(name, "P_W_1")) {
            rear = "P_W_1R";
        } else if (!strcmp(name, "P_R_1")) {
            rear = "P_R_1R";
        } else if (!strcmp(name, "P_T_1")) {
            rear = "P_T_1R";
        } else if (!strcmp(name, "P_Z_2")) {
            rear = "P_Z_2R";
        } else if (!strcmp(name, "P_S_2")) {
            rear = "P_S_2R";
        } else if (!strcmp(name, "P_W_2")) {
            rear = "P_W_2R";
        } else if (!strcmp(name, "P_R_2")) {
            rear = "P_R_2R";
        } else if (!strcmp(name, "P_T_2")) {
            rear = "P_T_2R";
        } else if (!strcmp(name, "P_Z_3")) {
            rear = "P_Z_3R";
        } else if (!strcmp(name, "P_S_3")) {
            rear = "P_S_3R";
        } else if (!strcmp(name, "P_W_3")) {
            rear = "P_W_3R";
        } else if (!strcmp(name, "P_R_3")) {
            rear = "P_R_3R";
        } else if (!strcmp(name, "P_T_3")) {
            rear = "P_T_3R";
        } else if (!strcmp(name, "P_O_Z_1")) {
            rear = "P_O_Z_1R";
        } else if (!strcmp(name, "P_O_S_1")) {
            rear = "P_O_S_1R";
        } else if (!strcmp(name, "P_O_F_1")) {
            rear = "P_O_F_1R";
        } else if (!strcmp(name, "P_O_R_1")) {
            rear = "P_O_R_1R";
        } else if (!strcmp(name, "P_O_N_1")) {
            rear = "P_O_N_1R";
        } else if (!strcmp(name, "P_O_S_2")) {
            rear = "P_O_S_2R";
        } else if (!strcmp(name, "P_O_F_2")) {
            rear = "P_O_F_2R";
        } else if (!strcmp(name, "P_O_R_2")) {
            rear = "P_O_R_2R";
        } else if (!strcmp(name, "P_O_N_2")) {
            rear = "P_O_N_2R";
        } else if (!strcmp(name, "P_O_D_1")) {
            rear = "P_O_D_1R";
        } else if (!strcmp(name, "P_O_D_2")) {
            rear = "P_O_D_2R";
        } else if (!strcmp(name, "P_A_1")) {
            rear = "P_A_1R";
        }
        return rear;
    } else {
        if (!strcmp(name, "M_Z_1")) {
            rear = "M_Z_1R";
        } else if (!strcmp(name, "M_S_1")) {
            rear = "M_S_1R";
        } else if (!strcmp(name, "M_W_1")) {
            rear = "M_W_1R";
        } else if (!strcmp(name, "M_R_1")) {
            rear = "M_R_1R";
        } else if (!strcmp(name, "M_J_1B")) {
            rear = "M_J_1BR";
        } else if (!strcmp(name, "M_J_1C")) {
            rear = "M_J_1CR";
        } else if (!strcmp(name, "M_H_1")) {
            rear = "M_H_1R";
        } else if (!strcmp(name, "M_H_2")) {
            rear = "M_H_2R";
        } else if (!strcmp(name, "M_H_4")) {
            rear = "M_H_4R";
        } else if (!strcmp(name, "M_H_5")) {
            rear = "M_H_5R";
        } else if (!strcmp(name, "M_H_7")) {
            rear = "M_H_7R";
        } else if (!strcmp(name, "M_H_8")) {
            rear = "M_H_8R";
        } else if (!strcmp(name, "M_O_1")) {
            rear = "M_O_2R";
        } else if (!strcmp(name, "M_I_2")) {
            rear = "M_I_2R";
        } else if (!strcmp(name, "M_U_2")) {
            rear = "M_U_2R";
        } else if (!strcmp(name, "M_C_1")) {
            rear = "M_C_1R";
        }
        return rear;
    }
}

const char* toFrontPose(const char* name) {
    const char* front = NULL;
    if (mp->characterId == CHARACTER_PEACH && !(mp->peachFlags & PEACH_FLAG_GUNDAN)) {
        if (!strcmp(name, "P_Z_1R")) {
            front = "P_Z_1";
        } else if (!strcmp(name, "P_S_1R")) {
            front = "P_S_1";
        } else if (!strcmp(name, "P_W_1R")) {
            front = "P_W_1";
        } else if (!strcmp(name, "P_R_1R")) {
            front = "P_R_1";
        } else if (!strcmp(name, "P_T_1R")) {
            front = "P_T_1";
        } else if (!strcmp(name, "P_Z_2R")) {
            front = "P_Z_2";
        } else if (!strcmp(name, "P_S_2R")) {
            front = "P_S_2";
        } else if (!strcmp(name, "P_W_2R")) {
            front = "P_W_2";
        } else if (!strcmp(name, "P_R_2R")) {
            front = "P_R_2";
        } else if (!strcmp(name, "P_T_2R")) {
            front = "P_T_2";
        } else if (!strcmp(name, "P_Z_3R")) {
            front = "P_Z_3";
        } else if (!strcmp(name, "P_S_3R")) {
            front = "P_S_3";
        } else if (!strcmp(name, "P_W_3R")) {
            front = "P_W_3";
        } else if (!strcmp(name, "P_R_3R")) {
            front = "P_R_3";
        } else if (!strcmp(name, "P_T_3R")) {
            front = "P_T_3";
        } else if (!strcmp(name, "P_O_Z_1R")) {
            front = "P_O_Z_1";
        } else if (!strcmp(name, "P_O_S_1R")) {
            front = "P_O_S_1";
        } else if (!strcmp(name, "P_O_F_1R")) {
            front = "P_O_F_1";
        } else if (!strcmp(name, "P_O_R_1R")) {
            front = "P_O_R_1";
        } else if (!strcmp(name, "P_O_N_1R")) {
            front = "P_O_N_1";
        } else if (!strcmp(name, "P_O_S_2R")) {
            front = "P_O_S_2";
        } else if (!strcmp(name, "P_O_F_2R")) {
            front = "P_O_F_2";
        } else if (!strcmp(name, "P_O_R_2R")) {
            front = "P_O_R_2";
        } else if (!strcmp(name, "P_O_N_2R")) {
            front = "P_O_N_2";
        } else if (!strcmp(name, "P_O_D_1R")) {
            front = "P_O_D_1";
        } else if (!strcmp(name, "P_O_D_2R")) {
            front = "P_O_D_2";
        } else if (!strcmp(name, "P_A_1R")) {
            front = "P_A_1";
        }
        return front;
    } else {
        if (!strcmp(name, "M_Z_1R")) {
            front = "M_Z_1";
        } else if (!strcmp(name, "M_S_1R")) {
            front = "M_S_1";
        } else if (!strcmp(name, "M_W_1R")) {
            front = "M_W_1";
        } else if (!strcmp(name, "M_R_1R")) {
            front = "M_R_1";
        } else if (!strcmp(name, "M_J_1BR")) {
            front = "M_J_1B";
        } else if (!strcmp(name, "M_J_1CR")) {
            front = "M_J_1C";
        } else if (!strcmp(name, "M_H_1R")) {
            front = "M_H_1";
        } else if (!strcmp(name, "M_H_2R")) {
            front = "M_H_2";
        } else if (!strcmp(name, "M_H_4R")) {
            front = "M_H_4";
        } else if (!strcmp(name, "M_H_5R")) {
            front = "M_H_5";
        } else if (!strcmp(name, "M_H_7R")) {
            front = "M_H_7";
        } else if (!strcmp(name, "M_H_8R")) {
            front = "M_H_8";
        } else if (!strcmp(name, "M_O_2R")) {
            front = "M_O_1";
        } else if (!strcmp(name, "M_I_2R")) {
            front = "M_I_2";
        } else if (!strcmp(name, "M_U_2R")) {
            front = "M_U_2";
        } else if (!strcmp(name, "M_C_1R")) {
            front = "M_C_1";
        }
        return front;
    }
}
