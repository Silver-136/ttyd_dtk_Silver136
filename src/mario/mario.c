// Mario's work (MarioWork): init, per-frame main, the key/control locks, pose and paper changes, display and
// shadow, and the per-map start-up hooks.

#include "version.h"
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
#include "mario/mario_kemuri.h"
#include <dolphin/pad.h>
#include "driver/mapdrv.h"
static void marioDisp(CameraId camId, void* param);
static void marioDispBlur(CameraId camId, void* param);
static void marioMoveMain(void);
static void marioCtrlOff2Main(void);

// defined in mario_hit.c with no parameter, but called here with one
void dou10_yoko_yari3(void* p);

//.data
#if VERSION >= VERSION_JP
const char* dotMarioPose[4][6] = {
    {"Z_1", "S_1", "W_1", "R_1", "U_1", "J_1"},
    {"Z_15", "S_2", "W_15", "R_15", "U_2", "J_2"},
    {"Z_17", "S_3", "W_17", "R_17", "U_3", "J_3"},
    {"Z_18", "S_4", "W_18", "R_18", "U_4", "J_4"}
};
#endif

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
// FAKE: unused 4-byte .sbss filler in no symbol map; it only fills the slot the original had here. Removing it changes the object.
s32 unk_804186EC;
MarioWork* mp;

#if VERSION >= VERSION_JP
Vec R_last_position;
#endif

s32 blurPosId;

//local prototypes
static const char* toRearPose(const char* name);
static const char* toFrontPose(const char* name);

/** Returns Mario's work (MarioWork), allocated once by marioInit; the main way the rest of the game reaches Mario. */
MarioWork* marioGetPtr(void) {
    return mp;
}

/**
 * Returns Mario's outfit colour (0 normal, 1 L Emblem, 2 W Emblem, 3 both),
 * which picks the row of marioAnimeGroupData.
 */
s32 marioGetColor(void) {
    return mp->colorId;
}

/**
 * Releases the up to three animation pose sets of the current character (front, back, event)
 * and marks the slots free; used before loading another character's poses.
 */
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

/**
 * Works out the outfit colour from the equipped badges:
 * 1 with only the L Emblem (Luigi's colours), 2 with only the W Emblem (Wario's colours), 3 with both,
 * 0 otherwise.
 */
static inline s32 marioBadgeColor(void) {
    s32 lEmblem = pouchEquipCheckBadge(ITEM_L_EMBLEM);
    s32 wEmblem = pouchEquipCheckBadge(ITEM_W_EMBLEM);

    if (lEmblem && !wEmblem) {
        return 1;
    } else if (!lEmblem && wEmblem) {
        return 2;
    } else if (lEmblem && wEmblem) {
        return 3;
    }
    return 0;
}

#if VERSION >= VERSION_DEMO_EU
#define MARIO_TALK_POSE "M_I_3"
#else
#define MARIO_TALK_POSE "M_I_2"
#endif

/**
 * Switches the player character: 0 Mario (in his badge outfit colour, or 8-bit Mario if that mode is on),
 * 1 Princess Peach (the X-Naut disguise when peachFlags 0x2000 is set), 2 Bowser.
 * Loads the character's pose sets and the poses used when Mario talks to NPCs,
 * re-forces the paper form of the current motion (plane, boat, tube, sideways, Vivian's shadow, Yoshi ride)
 * and sets the collision box and speeds.
 * Called from peach.c, event.c and an evt_mario script call;
 * from the Japanese release on the re-force is skipped in background mode.
 */
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
        wp->paperAnimGroupId[0] = animPoseEntry(kpaGetStageViewType() == KPA_VIEW_3D ? "c_koopa" : "c_koopa2", 2);
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
#if VERSION >= VERSION_JP
        if (!mp->characterId) {
            color = marioBadgeColor();
            if (!mp->characterId && mp->colorId != color) {
                mp->colorId = color;
                marioSetCharMode(CHARACTER_MARIO);
            }
        }
#endif
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
        npcSetMarioAutoTalkPose("M_S_1", MARIO_TALK_POSE);
        wp->trigFlags |= MARIO_TRIG_FLAG_CHG_ANIM;
    } else {
        marioSetFamicomMode(1);
    }
#if VERSION >= VERSION_JP
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
            marioForceVivianAnime();
            return;
        } else if (wp->currMotionId == MARIO_MOTION_YOSHI) {
            marioChgPose("M_Y_2");
            marioChgPose("M_Y_1");
            return;
        }
    }
#else
    if (wp->currMotionId == MARIO_MOTION_PLANE) {
        marioForcePlaneAnime();
        return;
    } else if (wp->currMotionId == MARIO_MOTION_SHIP) {
        marioForceShipAnime();
        return;
    } else if (wp->currMotionId == MARIO_MOTION_ROLL) {
        marioForceRollAnime();
        return;
    } else if (wp->currMotionId == MARIO_MOTION_SLIT) {
        marioForceSlitAnime();
        return;
    } else if (wp->currMotionId == MARIO_MOTION_YOSHI) {
        marioChgPose("M_Y_2");
        marioChgPose("M_Y_1");
        return;
    }
#endif
    if (wp->currMotionId != MARIO_MOTION_PLANE && wp->currMotionId != MARIO_MOTION_SHIP && wp->currMotionId != MARIO_MOTION_JABARA) {
        marioSetSpec();
    }
}

/**
 * Turns 8-bit ("Famicom") Mario on (mode 1) or off (mode 0):
 * swaps the front pose set for d_mario or back to the normal outfit and resets him to standing.
 * Turned on by the dot_mario script call in aji_09 (X-Naut Fortress) and off again
 * when the aji module unloads (its _epilog) if GSWF_AJI_09_DOT_MARIO_SEEN is set.
 * From the Japanese release on, calling it with the mode already set still resets the pose.
 */
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
            npcSetMarioAutoTalkPose("M_S_1", MARIO_TALK_POSE);
            wp->animName = NULL;
            marioChgPose("M_S_1");
#if VERSION >= VERSION_JP
        } else {
            wp->animName = NULL;
            marioChgPose("M_S_1");
#endif
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
            npcSetMarioAutoTalkPose("M_S_1", MARIO_TALK_POSE);
            wp->animName = NULL;
            marioChgPose("M_S_1");
#if VERSION >= VERSION_JP
        } else {
            wp->animName = NULL;
            marioChgPose("M_S_1");
#endif
        }
    }
}

/**
 * Sets the collision box and walk/dash base speeds for the current character:
 * Peach 20x47 with dash 2.0, Bowser dash 2.25, Mario 20x37 (10x25.9 in background mode) with dash 2.25.
 */
void marioSetSpec(void) {
    MarioWork* wp = mp;

    if (wp->characterId == CHARACTER_PEACH) {
        wp->wPlayerCollisionBox.x = 20.0f;
        wp->wPlayerCollisionBox.y = 47.0f;
        wp->baseWalkSpeed = 1.0f;
        wp->baseDashSpeed = 2.0f;
    } else if (wp->characterId == CHARACTER_BOWSER) {
        wp->baseWalkSpeed = 1.0f;
        wp->baseDashSpeed = 2.25f;
    } else {
        if (!(wp->flags & MARIO_FLAG_BG_MODE)) {
            wp->wPlayerCollisionBox.x = 20.0f;
            wp->wPlayerCollisionBox.y = 37.0f;
        } else {
            wp->wPlayerCollisionBox.x = 10.0f;
            wp->wPlayerCollisionBox.y = 25.9f;
        }
        wp->baseWalkSpeed = 1.0f;
        wp->baseDashSpeed = 2.25f;
    }
}

/**
 * Activates Mario as a map starts (seq_mapchange) or after a game over continue:
 * sets the standing pose and resumes the motion he was in (boat, tube, sideways, Yoshi ride),
 * otherwise plain standing; background mode always stands.
 */
void marioEntry(void) {
    MarioWork* wp = mp;

    wp->flags |= MARIO_FLAG_ACTIVE;
    marioChgPose("M_S_1");
    if (wp->flags & MARIO_FLAG_BG_MODE) {
#if VERSION >= VERSION_JP
        if (wp->currMotionId == MARIO_MOTION_SLIT) {
            wp->flags |= MARIO_FLAG_PAPER_MODE;
            marioSlitForceCancel();
        }
#endif
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

/**
 * Stops Mario collecting field items for 20 frames; mario_hit calls it when Bowser bumps his head on a ceiling.
 * marioItemGetChk counts the timer down.
 */
void marioItemGetDisable(void) {
    MarioWork* wp = mp;
    wp->flags |= MARIO_FLAG_ITEM_GET_DISABLED;
    wp->itemGetDisableTimer = 20;
}

/**
 * Counts down the item pick-up lock set by marioItemGetDisable and clears it at 0;
 * run each frame from Bowser's main loop (koopa.c).
 */
void marioItemGetChk(void) {
    MarioWork* work;
    s32 timer;

    work = mp;
    timer = mp->itemGetDisableTimer;
    if (timer > 0) {
        timer--;
        work->itemGetDisableTimer = (s16)timer;
        if ((s16)timer <= 0) {
            work->itemGetDisableTimer = 0;
            mp->flags &= ~MARIO_FLAG_ITEM_GET_DISABLED;
        }
    }
}

/**
 * TRUE when Mario may collect field items, i.e. the marioItemGetDisable lock is off;
 * itemdrv checks it before a pick-up.
 */
BOOL marioItemGetOk(void) {
    return (mp->flags & MARIO_FLAG_ITEM_GET_DISABLED) == 0;
}

/**
 * TRUE when case events (touch and check zones) may fire for Mario:
 * not while he is a paper airplane and not while hidden in Vivian's shadow. Checked by casedrv.
 */
BOOL marioCaseEventValidChk(void) {
    if (mp->currMotionId == MARIO_MOTION_PLANE) {
        return FALSE;
    } else {
        return vivianGetStatus() == VIVIAN_STATUS_INACTIVE;
    }
}

// Descriptive name (not in the symbol maps); placeholder unk_8005bb80, JP 0x8005BB80. See docs/NAME_PROVENANCE.md.
#if VERSION >= VERSION_JP
BOOL marioFrontActionValidChk(void) {
    if (mp->currMotionId == MARIO_MOTION_ROLL || mp->flags & MARIO_FLAG_PAPER_MODE) {
        return FALSE;
    }
    return TRUE;
}
#endif

/** 1 when Mario's control is on (no marioCtrlOff outstanding). */
static inline s32 marioChkCtrl2(void) {
    return mp->ctrlOffCount ? 0 : 1;
}

/** TRUE while a marioCtrlOff is outstanding. */
static inline BOOL marioCtrlDisable(void) {
    return marioChkCtrl2() ? 0 : 1;
}

/** TRUE when marioChkKey refuses button input. */
static inline BOOL marioKeyDisable(void) {
    if (!marioChkKey()) {
        return TRUE;
    }
    return FALSE;
}

/**
 * TRUE while Mario rides Yoshi on the ground (yoshiGetStatus 1), in the normal game sequence with keys on;
 * marioCheckMenuDisable then lets the pause menu open even though Yoshi's ride holds the keys.
 */
static inline BOOL marioChkYoshiKey(void) {
    PartyEntry* party = partyGetPtr(marioGetPartyId());

    if (seqGetSeq() != SEQ_GAME) {
        return FALSE;
    }
    if (mp->keyOffCount) {
        return FALSE;
    }
    if (yoshiGetStatus() == YOSHI_STATUS_RIDING) {
        return TRUE;
    }
    return FALSE;
}

/**
 * TRUE when the pause menu may not open now: control off, playing Peach or Bowser in the side view, mid-hammer,
 * accordion, plane or boat changing form, taking damage, in the air, Bobbery busy,
 * or a partner action holding the keys.
 * In the Pianta Parlor game rooms (yuu_) only the control and key locks count.
 * Called by win_main and a dou_04 script call; with Vivian in use,
 * US and later also block it while her party entry has flag 0x80000000.
 */
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
    if (marioGetPlaneStatus() == PLANE_STATUS_TRANSITION) {
        return TRUE;
    }
    if (marioShipChanging() == SHIP_STATUS_FOLDING || marioShipChanging() == SHIP_STATUS_SETTLING) {
        return TRUE;
    }
    if (wp->currMotionId == MARIO_MOTION_DAMAGE || wp->currMotionId == MARIO_MOTION_DAMAGE_TOGE) {
        return TRUE;
    }
    if (wp->characterId != CHARACTER_BOWSER && !marioGetPlaneStatus() && !wp->hitobjStandOn) {
        return TRUE;
    }
    if (!mario_bomhei_keychk()) {
        return TRUE;
    }
#if VERSION >= VERSION_US
    if (vivianGetStatus()) {
        if (!(partyGetPtr(marioGetPartyId())->flags & PARTY_FLAG_IS_EVT_USE)) {
            if (vivianGetStatus() == VIVIAN_STATUS_HIDDEN) {
                return FALSE;
            }
        } else {
            return TRUE;
        }
    }
#else
    if (vivianGetStatus() && vivianGetStatus() == VIVIAN_STATUS_HIDDEN) {
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

/**
 * TRUE when Mario accepts button input this frame:
 * normal game sequence, no marioKeyOff outstanding,
 * and no partner action holding the keys (boat changing, tube, sideways, Flurrie, Ms. Mowz or Vivian in use,
 * Bobbery, Yoshi, Koops holding an item).
 */
BOOL marioChkKey(void) {
    PartyEntry* party = partyGetPtr(marioGetPartyId());

    if (seqGetSeq() != SEQ_GAME) {
        return FALSE;
    }
    if (mp->keyOffCount) {
        return FALSE;
    }
    if (marioShipChanging() == SHIP_STATUS_FOLDING || marioShipChanging() == SHIP_STATUS_SETTLING) {
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

/** TRUE when Mario's control is on, i.e. no marioCtrlOff is outstanding. */
BOOL marioChkCtrl(void) {
    return !mp->ctrlOffCount;
}

#if VERSION >= VERSION_JP
BOOL N_marioChkUseParty(void) {
    // FAKE: (u16) cast on the u32 buttonNew (all tested bits are in the low half); the plain test changes the object
    if ((u16)gp->buttonNew[0] & (PAD_BUTTON_LEFT | PAD_BUTTON_RIGHT | PAD_BUTTON_DOWN | PAD_BUTTON_UP | PAD_BUTTON_START)) {
        return TRUE;
    }
    return mp->ctrlOffCount;
}
#endif

/**
 * Readies Mario for a field battle start (first strike or enemy contact, from npcdrv and evt_npc):
 * freezes his field movement, ends the partner's action and takes the partner's control away.
 * From the Japanese release on it also saves his position for the accordion (jabara) motion.
 */
void marioFBattlePrepare(void) {
    MarioWork* wp = mp;
#if VERSION >= VERSION_JP
    R_last_position = wp->position;
#endif
    marioAdjustMoveDir();
#if VERSION >= VERSION_JP
    if (wp->currMotionId != MARIO_MOTION_JABARA) {
        wp->flags |= MARIO_FLAG_PRE_BATTLE_PHYSICS;
    }
#else
    wp->flags |= MARIO_FLAG_PRE_BATTLE_PHYSICS;
#endif
    partyUsePost(partyGetPtr(marioGetPartyId()));
    partyCtrlOff();
}

/**
 * Undoes marioFBattlePrepare once the battle start is over or cancelled:
 * restores movement, puts Mario back after the accordion motion, gets him off Yoshi,
 * gives the partner control back and ends a hammer swing.
 * US and later also move the partner back to its spot beside Mario.
 */
void marioFBattlePost(void) {
    MarioWork* wp = mp;
    PartyEntry* party;

    wp->flags &= ~MARIO_FLAG_PRE_BATTLE_PHYSICS;
#if VERSION >= VERSION_JP
    if (wp->currMotionId == MARIO_MOTION_JABARA) {
        wp->position = R_last_position;
    }
#endif
    if (wp->currMotionId == MARIO_MOTION_YOSHI) {
        marioYoshiForceCancel();
    }
#if VERSION <= VERSION_DEMO_JP
    partyCtrlOn();
    if (wp->currMotionId == MARIO_MOTION_HAMMER) {
#else
#if VERSION >= VERSION_US
    party = partyGetPtr(marioGetPartyId());
    if (party) {
        Vec pos;
        partyGetAppearPos(party, &pos);
        party->position = pos;
    }
#endif
    partyCtrlOn();
    if (wp->currMotionId == MARIO_MOTION_HAMMER || wp->currMotionId == MARIO_MOTION_JABARA) {
#endif
        marioChgMot(MARIO_MOTION_STAY);
    }
}

/** Returns how many marioCtrlOff calls are outstanding (0 means Mario has control). */
s32 marioCtrlOffChk(void) {
    return mp->ctrlOffCount;
}

/** Returns how many marioKeyOff calls are outstanding (0 means button input is on). */
s32 marioKeyOffChk(void) {
    return mp->keyOffCount;
}

/**
 * Takes control away from the player (counted: each call needs a marioCtrlOn) and returns the new count;
 * marioMain then skips input and motion.
 */
s32 marioCtrlOff(void) {
    MarioWork* wp = mp;

    wp->flags |= MARIO_FLAG_CTRL_OFF;
    wp->ctrlOffCount++;
    return wp->ctrlOffCount;
}

/** Releases one marioCtrlOff; the player has control again when the count reaches 0. Returns the remaining count. */
s32 marioCtrlOn(void) {
    MarioWork* wp = mp;

    wp->ctrlOffCount--;
    if (wp->ctrlOffCount == 0) {
        wp->flags &= ~MARIO_FLAG_CTRL_OFF;
    }
    return wp->ctrlOffCount;
}

/**
 * Like marioCtrlOff, but marioMain keeps running Mario's motion and physics (marioCtrlOff2Main) so a jump
 * or fall can finish; used when the pause menu opens (win_main).
 */
s32 marioCtrlOff2(void) {
    MarioWork* wp = mp;

    wp->flags |= MARIO_FLAG_CTRL_OFF_2;
    wp->flags |= MARIO_FLAG_CTRL_OFF;
    wp->ctrlOffCount++;
    return wp->ctrlOffCount;
}

/**
 * Releases a marioCtrlOff2: clears the keep-moving flag and drops one control-off count.
 * Returns the remaining count.
 */
s32 marioCtrlOn2(void) {
    MarioWork* wp = mp;

    wp->flags &= ~MARIO_FLAG_CTRL_OFF_2;
    wp->ctrlOffCount--;
    if (wp->ctrlOffCount == 0) {
        wp->flags &= ~MARIO_FLAG_CTRL_OFF;
    }
    return wp->ctrlOffCount;
}

/**
 * Blocks button input for Mario (counted like marioCtrlOff, but his motion and physics keep running)
 * and returns the new count.
 */
s32 marioKeyOff(void) {
    MarioWork* wp = mp;

    wp->flags |= MARIO_FLAG_KEY_OFF;
    wp->keyOffCount++;
    return wp->keyOffCount;
}

/** Releases one marioKeyOff; input returns when the count reaches 0. Returns the remaining count. */
s32 marioKeyOn(void) {
    MarioWork* wp = mp;

    wp->keyOffCount--;
    if (wp->keyOffCount == 0) {
        wp->flags &= ~MARIO_FLAG_KEY_OFF;
    }
    return wp->keyOffCount;
}

/**
 * Makes Mario and the current partner invincible for msec milliseconds (converted to frames);
 * Mario blinks while it lasts. npcdrv sets 3 s after an enemy hit. Returns the frame count.
 */
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

/** TRUE while Mario is in background mode (MARIO_FLAG_BG_MODE). */
BOOL marioBgmodeChk(void) {
    return (mp->flags >> 25) & 1;
}

/**
 * Puts Mario in background mode: smaller collision box, drawn at scale 1.2 instead of 2.0, no shadow,
 * and the camera stops following his height. Started by an evt_mario script call.
 */
void marioBgmodeOn(void) {
    mp->flags |= MARIO_FLAG_BG_MODE;
    marioSetSpec();
    camFollowYOff();
}

/** Ends background mode: normal collision box, and the camera follows Mario's height again. */
void marioBgmodeOff(void) {
    mp->flags &= ~MARIO_FLAG_BG_MODE;
    marioSetSpec();
    camFollowYOn();
}

/**
 * TRUE if Mario is in any of the asked states: 1 jumping, 2 falling, 4 paper mode, 8 accordion (jabara),
 * 0x10 rolling as a paper tube, 0x20 blown by wind (kaze). Used by scripts, hit checks and motions.
 */
BOOL marioChkSts(s32 flags) {
    MarioWork* wp = mp;
    BOOL ret = FALSE;

    if (flags & MARIO_STS_JUMPING && wp->flags & MARIO_FLAG_IS_JUMPING) {
        ret = TRUE;
    }
    if (flags & MARIO_STS_FALLING && wp->flags & MARIO_FLAG_IS_FALLING) {
        ret = TRUE;
    }
    if (flags & MARIO_STS_PAPER && wp->flags & MARIO_FLAG_PAPER_MODE) {
        ret = TRUE;
    }
    if (flags & MARIO_STS_KAZE && wp->currMotionId == MARIO_MOTION_KAZE) {
        ret = TRUE;
    }
    if (flags & MARIO_STS_ROLL && wp->currMotionId == MARIO_MOTION_ROLL) {
        ret = TRUE;
    }
    if (flags & MARIO_STS_JABARA && wp->currMotionId == MARIO_MOTION_JABARA) {
        ret = TRUE;
    }
    return ret;
}

/**
 * Clears every hit-object reference in MarioWork (stand-on, front, wall, head, hammer, ...) that points to the
 * named collision object; mobjdrv calls it before deleting a field object's hit so Mario keeps no stale pointer.
 */
void marioResetHitObj(const char* name) {
    MarioWork* wp = mp;

    if (wp->hitobjFrontA && !strcmp(hitGetName(wp->hitobjFrontA), name)) {
        wp->hitobjFrontA = NULL;
    }
    if (wp->hitobjFront && !strcmp(hitGetName(wp->hitobjFront), name)) {
        wp->hitobjFront = NULL;
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
    if (wp->hitobjWall && !strcmp(hitGetName(wp->hitobjWall), name)) {
        wp->hitobjWall = NULL;
    }
    if (wp->hitobjHead && !strcmp(hitGetName(wp->hitobjHead), name)) {
        wp->hitobjHead = NULL;
    }
    if (wp->hitobjUnder && !strcmp(hitGetName(wp->hitobjUnder), name)) {
        wp->hitobjUnder = NULL;
    }
    if (wp->hitobjHammer && !strcmp(hitGetName(wp->hitobjHammer), name)) {
        wp->hitobjHammer = NULL;
    }
}

/**
 * Puts Mario back to plain standing: normal spec, paper and tube modes off, no paper animation,
 * mirror and facing locks cleared, draw offsets zeroed, speed 0. Used by mario_motion and after a game over.
 */
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

/**
 * Zeroes Mario's draw rotation and draw offsets;
 * the damage, pipe, ground pound, tube and boat motions call it when they end.
 */
void marioOfsRotReset(void) {
    MarioWork* wp = mp;

    wp->rot = (Vec){0.0f, 0.0f, 0.0f};
    wp->rotOfs = (Vec){0.0f, 0.0f, 0.0f};
    wp->dispOfs = (Vec){0.0f, 0.0f, 0.0f};
    wp->dispOfs2 = (Vec){0.0f, 0.0f, 0.0f};
}

/** Marks Mario's sound slots (and from the US release the ground pound sound slots) as not playing. */
void marioSoundInit(void) {
    MarioWork* wp = mp;

    wp->soundId[0] = -1;
    wp->soundId[1] = -1;
    wp->soundId[2] = -1;
#if VERSION >= VERSION_US
    wp->hipSfxId[0] = -1;
    wp->hipSfxId[1] = -1;
#endif
}

/** Loads Mario's three pose sets for the current outfit colour; called once by seq_logo as the logo sequence ends. */
void marioPoseInit(void) {
    MarioWork* wp = mp;

    wp->paperAnimGroupId[0] = animPoseEntry(marioAnimeGroupData[mp->colorId][0], 2);
    wp->paperAnimGroupId[1] = animPoseEntry(marioAnimeGroupData[mp->colorId][1], 2);
    wp->paperAnimGroupId[2] = animPoseEntry(marioAnimeGroupData[mp->colorId][2], 2);
}

/**
 * Allocates and sets up MarioWork at boot (mariost):
 * pouch, gravity, camera, preloads the pose groups, standing pose, balloon, motion-blur trail and camera shift,
 * gives Mario the Hammer if the pouch lacks it, then sets up the partners.
 */
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
        mp->baseWalkSpeed = 1.0f;
        mp->baseDashSpeed = 2.25f;
    } else {
        mp->baseWalkSpeed = 1.0f;
        mp->baseDashSpeed = 2.0f;
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
#if VERSION >= VERSION_JP
    mp->bottomlessResetPosition = (Vec){0.0f, 74.0f, 0.0f};
#endif
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

/**
 * Resets MarioWork on a map change (seq_mapchange) but keeps the character, pose sets, current motion, facing,
 * partners, camera and 8-bit mode; the rest is cleared and the partners, balloon,
 * blur trail and camera shift are reset. The early demos carry flag 0x200000 over as the party-walk lock.
 */
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
#if VERSION <= VERSION_DEMO_JP
    BOOL flag22;
#endif
    s32 poseId;
    s32 t;
    s8 prev0;
    s8 prev1;
    struct KpaWork* kpaWork;
    f32 dispDir;
    f32 dispDirTarget;
    f32 dirView;
    f32 moveDir;
#if VERSION <= VERSION_DEMO_JP
    flag22 = FALSE;
    if ((wp = mp)->flags & 0x200000) {
        flag22 = TRUE;
    }
    dotFlag = wp->flags & MARIO_FLAG_IS_8_BIT;
#else
    // FAKE: assignment inside the expression; wp = mp; on its own line changes the object
    dotFlag = (wp = mp)->flags & MARIO_FLAG_IS_8_BIT;
#endif
    camId = marioGetCamId();
    charId = wp->characterId;
    slot0 = wp->currPartySlotId[0];
    slot1 = wp->currPartySlotId[1];
    peachFlags = wp->peachFlags;
    kpaWork = wp->kpaWork;
    trigFlags = wp->trigFlags;
    motion = wp->currMotionId;
    dispDir = wp->displayDirection;
    dispDirTarget = wp->displayDirectionTarget;
    dirView = wp->directionView;
    moveDir = wp->moveDirection;
    // FAKE: spare copy t of paperPoseId; testing poseId directly changes the object
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
#if VERSION <= VERSION_DEMO_JP
    if (flag22) {
        wp->flags |= MARIO_FLAG_PARTY_WALK_DISABLED;
    }
#endif
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
    wp->displayDirection = dispDir;
    wp->displayDirectionTarget = dispDirTarget;
    wp->directionView = dirView;
    wp->moveDirection = moveDir;
    wp->prevPartyMemberId[0] = prev0;
    wp->prevPartyMemberId[1] = prev1;
    wp->currPartySlotId[0] = slot0;
    wp->currPartySlotId[1] = slot1;
    wp->paperAnimGroupId[0] = anim0;
    wp->paperAnimGroupId[1] = anim1;
    wp->paperAnimGroupId[2] = anim2;
    wp->paperPoseId = poseId;
    if (!wp->characterId) {
        wp->baseWalkSpeed = 1.0f;
        wp->baseDashSpeed = 2.25f;
    } else {
        wp->baseWalkSpeed = 1.0f;
        wp->baseDashSpeed = 2.0f;
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
        animPoseSetMaterialLightFlagOn(wp->paperAnimGroupId[0], MATERIAL_LIGHT_CHARA);
    }
    if (wp->paperAnimGroupId[1] >= 0) {
        animPoseSetMaterialLightFlagOn(wp->paperAnimGroupId[1], MATERIAL_LIGHT_CHARA);
    }
    if (wp->paperAnimGroupId[2] >= 0) {
        animPoseSetMaterialLightFlagOn(wp->paperAnimGroupId[2], MATERIAL_LIGHT_CHARA);
    }
    wp->scale = (Vec){1.0f, 1.0f, 1.0f};
#if VERSION >= VERSION_JP
    wp->bottomlessResetPosition = (Vec){0.0f, 74.0f, 0.0f};
#endif
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

/** Every other frame while motion blur is on, records Mario's position in the 8-entry blur trail. */
static inline void marioMainBlur(MarioWork* wp) {
    if ((wp->frameCounter & 1) && (mp->dispFlags & MARIO_DISP_FLAG_BLUR)) {
        blurPos[blurPosId] = mp->position;
        if (++blurPosId >= 8) {
            blurPosId -= 8;
        }
    }
}

/**
 * End-of-frame bookkeeping for marioMain: remembers the air timer and floor,
 * starts the game over sequence when HP is 0, clears the moving-floor push and counts the frame.
 */
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

/** Switches Mario's outfit when the colour from the equipped L Emblem/W Emblem changed. */
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

/** TRUE while Mario rolls as a paper tube; a copy of marioChkRollMot defined before marioMain. */
static inline BOOL marioChkRollMot2(void) {
    BOOL ret = FALSE;
    if (mp->currMotionId == MARIO_MOTION_ROLL) {
        ret = TRUE;
    }
    return ret;
}

/**
 * Mario's per-frame update, run from mariost while he is active:
 * outfit and Slow Go checks, the Moon's low gravity (moo maps), invincibility and coin-drop timers, input,
 * partner use and motion, then the movement, ground and wall checks for his state, bottomless pits,
 * display set-up, balloon, partner and camera. Bowser and Peach hand off to kpaMain/peachMain.
 * The early demos always recheck the outfit and have no Slow Go walk forcing here.
 */
void marioMain(void) {
    MarioWork* wp = mp;
    MarioWork* p;
    u32 flags;
    f32 y;
    f32 c, b, a;

    if (!(wp->flags & MARIO_FLAG_ACTIVE)) {
        return;
    }
#if VERSION >= VERSION_JP
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
#else
    marioColorChk();
#endif
    marioSetDirEventMain();
#if VERSION >= VERSION_JP
    wp->playerGravity = 1.0f;
#endif
    if (!strcmp(gp->currentMapName, "moo_00") || !strcmp(gp->currentMapName, "moo_01") ||
        !strcmp(gp->currentMapName, "moo_02") || !strcmp(gp->currentMapName, "moo_05") ||
        !strcmp(gp->currentMapName, "moo_06") || !strcmp(gp->currentMapName, "moo_07")) {
        wp->playerGravity = 0.5f;
#if VERSION <= VERSION_DEMO_JP
    } else {
        wp->playerGravity = 1.0f;
#endif
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
            // FAKE: spare copy drop of pos; passing &pos changes the object
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
#if VERSION <= VERSION_DEMO_JP
            marioMove();
#else
            marioChkGnd();
#endif
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
            mario_kemuri();
            partyMain();
            marioCamMain();
            marioMainEnd(mp);
            return;
        }
        if (wp->currMotionId == MARIO_MOTION_JUMP_3) {
            marioBottomless();
            marioPreDisp();
            marioBalloonMain();
            mario_kemuri();
            partyMain();
            marioCamMain();
            marioMainBlur(wp);
            marioMainEnd(mp);
            return;
        }
        flags = wp->flags;
        if (flags & MARIO_FLAG_IS_STEPPING) {
            marioMove();
            marioChkOverhead();
            if (wp->airTimer > 100) {
                marioChgMot(MARIO_MOTION_FALL);
                marioSetFallPara();
            }
        } else if (flags & MARIO_FLAG_IS_JUMPING) {
            marioJump();
            marioChkOverhead();
            marioMove();
#if VERSION <= VERSION_DEMO_JP
            if (wp->airTimer > 5) {
#else
            if (!marioChkRollMot2() && wp->airTimer > 5) {
#endif
                marioChkGnd2();
            }
        } else if (flags & MARIO_FLAG_IS_FALLING) {
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
#if VERSION <= VERSION_DEMO_JP
                    if (wp->airTimer > 5) {
#else
                    if (!marioChkRollMot2() && wp->airTimer > 5) {
#endif
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
    mario_kemuri();
    partyMain();
    marioCamMain();
    marioMainBlur(wp);
    marioMainEnd(mp);
}

/** TRUE while Mario rolls as a paper tube. */
static inline BOOL marioChkRollMot(void) {
    BOOL ret = FALSE;
    if (mp->currMotionId == MARIO_MOTION_ROLL) {
        ret = TRUE;
    }
    return ret;
}

/**
 * Runs Mario's motion and physics while control is off through marioCtrlOff2 (the pause menu opening),
 * so a jump or fall finishes; skipped for plane, boat, damage, pits, accordion, Yoshi and Vivian's shadow.
 */
static void marioCtrlOff2Main(void) {
    MarioWork* wp = mp;
    MarioMotion mot = wp->currMotionId;
    f32 y;

    if (mot == MARIO_MOTION_PLANE) {
        return;
    }
    if (mot == MARIO_MOTION_SHIP) {
        return;
    }
    if (mot == MARIO_MOTION_DAMAGE || mot == MARIO_MOTION_DAMAGE_TOGE || mot == MARIO_MOTION_BOTTOMLESS ||
        mot == MARIO_MOTION_JABARA || mot == MARIO_MOTION_YOSHI ||
#if VERSION <= VERSION_DEMO_JP
        mot == MARIO_MOTION_JUMP_3) {
#else
        mot == MARIO_MOTION_JUMP_3 || mot == MARIO_MOTION_JUMP_STAND) {
#endif
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
#if VERSION <= VERSION_DEMO_JP
        if (wp->airTimer > 5) {
#else
        if (!marioChkRollMot() && wp->airTimer > 5) {
#endif
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
#if VERSION <= VERSION_DEMO_JP
            if (wp->airTimer > 5) {
#else
            if (!marioChkRollMot() && wp->airTimer > 5) {
#endif
                marioChkGnd2();
            }
        } else {
            marioChkGnd2();
        }
#if VERSION >= VERSION_JP
        marioChkWallAround(&wp->position, 0, 0.0f, 0.0f, wp->wPlayerCollisionBox.z, wp->wPlayerCollisionBox.y);
    } else {
        marioChkGnd();
#endif
    }
}

/** Stores Mario's current screen position in MarioWork. */
static inline void marioMove_screenPos(void) {
    MarioWork* p = mp;
    f32 x1, y1, z1;
    marioGetScreenPos(p->position, &x1, &y1, &z1);
    p->screenX = x1;
    p->screenY = y1;
    p->screenZ = z1;
}

/**
 * Moves Mario for the frame (marioMoveMain) and updates his screen position.
 * In background mode it also handles the camera height and picks the walk or run pose from his movement;
 * from the Japanese release on, aji_13 (X-Naut Fortress) instead turns the camera's height follow back on
 * once he is low on screen (screen y 400 or more).
 */
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
#if VERSION >= VERSION_JP
        if (strcmp(gp->currentMapName, "aji_13")) {
            if (wp->controlStickSensitivity && !(wp->dispFlags & MARIO_DISP_FLAG_CAM_FOLLOW_Y_OFF)) {
                camFollowYOff();
            }
        } else {
            marioGetScreenPos(wp->position, &x2, &y2, &z2);
            if (y2 >= 400.0f) {
                camFollowYOn();
            }
#else
        if (wp->controlStickSensitivity && !(wp->dispFlags & MARIO_DISP_FLAG_CAM_FOLLOW_Y_OFF)) {
            camFollowYOff();
#endif
        }
        if (wp->currMotionId == MARIO_MOTION_WALK || wp->currMotionId == MARIO_MOTION_DASH) {
            if (0.0f != prev.x - wp->position.x) {
                if (PSVECDistance(&prev, &wp->position) <= 1.0f) {
                    marioChgPose("M_W_1");
                } else {
                    marioChgPose("M_R_1");
                }
            } else {
                stick = wp->wStickDir1;
                stick = stick * stick;
                if (stick < 25) {
                    marioChgPose("M_Z_1");
                } else if (stick < STICK_DASH_MIN_SQ) {
                    marioChgPose("M_W_1");
                } else {
                    marioChgPose("M_R_1");
                }
            }
        }
    }
}

/**
 * Mario's horizontal movement: forced moves (Yoshi, tube, boat),
 * air steering from the stick (weaker with Slow Go or as a tube), shallow-water slowdown, damage floors,
 * slopes and moving floors, the air speed cap, then the wall checks (plus the dou_10 side spears while a tube,
 * from the Japanese release on) and spikes.
 */
static void marioMoveMain(void) {
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
                spd = 0.0f;
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
    // FAKE: vy is a zero temporary (added to position.y, compared with dist); the literal changes the object
    vy = 0.0f;
    wp->position.y += vy + wp->floorMove.y;
    dist = sqrtf(vx * vx + vz2 * vz2);
    if (dist != vy) {
        spd = angleABf(0.0f, 0.0f, vx, vz2);
    } else {
        spd = toMovedir(wp->displayDirection);
    }
    if (!(wp->flags & MARIO_FLAG_PAPER_MODE) && (wp->flags & (MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING))) {
        vx = wp->baseDashSpeed * wp->playerGravity;
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
#if VERSION >= VERSION_JP
        dou10_yoko_yari3(&wp->frontWallDist);
        if (wp->hitobjFront && (hitGetAttr(wp->hitobjFront) & HIT_ATTR_SPIKES)) {
            set_damage_root_ypos(wp->position.y);
            marioChgMot(MARIO_MOTION_DAMAGE_TOGE);
            return;
        }
#endif
    }
    marioChkToge();
}

/** TRUE when Mario is showing a pushing pose (M_O_1, M_O_2 or M_O_2R); used by mot_walk. */
BOOL marioChkPushAnime(void) {
    MarioWork* wp = mp;
    BOOL valid = FALSE;
    if (!strcmp(wp->animName, "M_O_1") || !strcmp(wp->animName, "M_O_2") || !strcmp(wp->animName, "M_O_2R")) {
        valid = TRUE;
    }
    return valid;
}

/** Returns which of Mario's three pose sets is in use: 2 the event set, 1 the back-facing set, 0 the front set. */
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

// Descriptive name (not in the symbol maps); placeholder unk_80058374, JP 0x80058374. See docs/NAME_PROVENANCE.md.
#if VERSION >= VERSION_JP
void marioChgDamagePose(void) {
    MarioWork* wp = mp;
    MarioMotion mot = wp->currMotionId;

    if (mot != MARIO_MOTION_SHIP && mot != MARIO_MOTION_ROLL && mot != MARIO_MOTION_PLANE && mot != MARIO_MOTION_JABARA && mot != MARIO_MOTION_YOSHI) {
        // FAKE: switch with a single case 0 for the slit test; the plain if changes the object
        switch (wp->dispFlags & MARIO_DISP_FLAG_SLIT) {
        case 0:
            marioChgPose("M_D_1");
            break;
        }
    }
}
#endif

/**
 * Shows a pose from Mario's event pose set (e_mario); used by scripts and some motions.
 * Ignored while the pose is locked or as 8-bit Mario.
 */
void marioChgEvtPose(const char* name) {
    MarioWork* wp = mp;
    const char* animName;

    if (!(mp->dispFlags & MARIO_DISP_FLAG_POSE_LOCK) && !(mp->flags & MARIO_FLAG_IS_8_BIT)) {
        animName = mp->animName;
        if (!animName || strcmp(animName, name)) {
            wp->animName = name;
            wp->dispFlags &= ~MARIO_DISP_FLAG_POSE_UNUSED;
            wp->dispFlags |= MARIO_DISP_FLAG_EVT_POSE;
            wp->trigFlags |= MARIO_TRIG_FLAG_CHG_ANIM;
        }
    }
}

#if VERSION <= VERSION_DEMO_JP
const char* dotMarioPose[4][6] = {
    {"Z_1", "S_1", "W_1", "R_1", "U_1", "J_1"},
    {"Z_15", "S_2", "W_15", "R_15", "U_2", "J_2"},
    {"Z_17", "S_3", "W_17", "R_17", "U_3", "J_3"},
    {"Z_18", "S_4", "W_18", "R_18", "U_4", "J_4"}
};
#endif

/**
 * Maps a normal Mario pose (idle, stand, walk, run, jump) to the matching 8-bit pose for his outfit colour;
 * NULL for poses 8-bit Mario lacks.
 */
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

/**
 * Changes Mario's pose by name from frame 0 unless it is already showing; ignored while the pose is locked.
 * As 8-bit Mario the name goes through toDotMarioPose and poses with no 8-bit version are skipped.
 */
void marioChgPose(const char* name) {
    MarioWork* wp = mp;
    const char* dot;

    if (!(wp->dispFlags & MARIO_DISP_FLAG_POSE_LOCK)) {
        if (!wp->animName || strcmp(wp->animName, name)) {
            if (!(wp->flags & MARIO_FLAG_IS_8_BIT)) {
                wp->animName = name;
                wp->poseTime = 0;
                wp->dispFlags &= ~(MARIO_DISP_FLAG_EVT_POSE | MARIO_DISP_FLAG_POSE_UNUSED);
                wp->trigFlags |= MARIO_TRIG_FLAG_CHG_ANIM;
            } else {
                dot = toDotMarioPose(name);
                if (dot) {
                    wp->animName = dot;
                    wp->poseTime = 0;
                    wp->dispFlags &= ~(MARIO_DISP_FLAG_EVT_POSE | MARIO_DISP_FLAG_POSE_UNUSED);
                    wp->trigFlags |= MARIO_TRIG_FLAG_CHG_ANIM;
                }
            }
        }
    }
}

/** Like marioChgPose but starts the new pose at the given frame; not for 8-bit Mario. Used by mot_stay and aji_17. */
void marioChgPoseTime(const char* name, s16 frames) {
    MarioWork* wp = mp;
    const char* animName;

    if (!(mp->dispFlags & MARIO_DISP_FLAG_POSE_LOCK) && !(mp->flags & MARIO_FLAG_IS_8_BIT)) {
        animName = mp->animName;
        if (!animName || strcmp(animName, name)) {
            wp->animName = name;
            wp->poseTime = frames;
            wp->dispFlags &= ~(MARIO_DISP_FLAG_EVT_POSE | MARIO_DISP_FLAG_POSE_UNUSED);
            wp->trigFlags |= MARIO_TRIG_FLAG_CHG_ANIM;
        }
    }
}

/**
 * Changes the paper animation of Mario's active paper form (set up by marioPaperOn) when the name differs;
 * ignored while the pose is locked or as 8-bit Mario.
 */
void marioChgPaper(const char* name) {
    MarioWork* wp = mp;
    s32 flags = wp->dispFlags;
    const char* animName;

    if (flags & MARIO_DISP_FLAG_PAPER && !(flags & MARIO_DISP_FLAG_POSE_LOCK) && !(mp->flags & MARIO_FLAG_IS_8_BIT)) {
        animName = mp->paperAnimName;
        if (!animName || strcmp(animName, name)) { // only when the animation changes
            wp->paperAnimName = name;
            wp->poseTime = 0;
            wp->trigFlags |= MARIO_TRIG_FLAG_CHG_PAPER_ANIM;
        }
    }
}

/** Sets the current frame of Mario's paper animation and of his pose; used by mot_ship for the boat. */
void marioSetPaperAnimeLocalTime(s32 frames) {
    MarioWork* wp = mp;

    wp->poseTime = frames;
    animPoseSetLocalTime(wp->paperPoseId, frames);
    animPoseSetLocalTime(wp->paperAnimGroupId[marioAnimeId()], frames);
}

/** Frees Mario's paper pose if one is loaded. */
static void marioPaperRelease(void) {
    MarioWork* wp = mp;
    s32 poseId = wp->paperPoseId;

    if (poseId >= 0) {
        animPaperPoseRelease(poseId);
        wp->paperPoseId = -1;
    }
}

/**
 * Loads a paper form animation and attaches it to Mario's model;
 * for the tube, plane, boat, accordion and pipe forms (p_roll, p_plane, p_hune, p_jyabara, p_dokan_x/y) the
 * paper flag passed is FALSE, for others TRUE. Always returns TRUE.
 */
BOOL marioPaperOn(const char* name) {
    BOOL flag;
    MarioWork* wp = mp;

    if (wp->paperPoseId >= 0) {
        animPaperPoseRelease(wp->paperPoseId);
        wp->paperPoseId = -1;
    }
    wp->dispFlags |= MARIO_DISP_FLAG_PAPER;
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

/** Detaches and frees Mario's paper form animation, returning him to his normal model. */
void marioPaperOff(void) {
    MarioWork* wp = mp;

    if (wp->dispFlags & MARIO_DISP_FLAG_PAPER) {
        if (wp->paperAnimName) {
            animPoseSetPaperAnimGroup(mp->paperAnimGroupId[0], NULL, FALSE);
            wp->paperAnimName = NULL;
            wp->dispFlags &= ~MARIO_DISP_FLAG_PAPER;
            marioPaperRelease();
        }
    }
}

/** Turns on the paper lighting material flag on Mario's front pose set; used by mot_hammer. */
void marioPaperLightOn(void) {
    animPoseSetMaterialLightFlagOn(mp->paperAnimGroupId[0], MATERIAL_LIGHT_PAPER);
}

/** Turns off the paper lighting material flag that marioPaperLightOn set. */
void marioPaperLightOff(void) {
    animPoseSetMaterialLightFlagOff(mp->paperAnimGroupId[0], MATERIAL_LIGHT_PAPER);
}

/** Projects a world position through the 3D camera to screen coordinates (x, y in pixels, z depth). */
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

/** TRUE if screen point (x, y) lies in the 0-600 by 0-535 area treated as on screen. */
BOOL marioChkInScreen(s32 x, s32 y) {
    BOOL valid = FALSE;
    if (x >= 0 && x <= 600 && y >= 0 && y <= 535) {
        valid = TRUE;
    }

    // FAKE: redundant compare with FALSE; return valid changes the object
    return valid != FALSE;
}

/** Mario's draw scale: 1.2 in background mode, 2.0 otherwise. */
f32 marioGetScale(void) {
    return mp->flags & MARIO_FLAG_BG_MODE ? 1.2f : 2.0f;
}

/**
 * Turns Mario's displayed facing (the paper flip) towards left
 * or right from his move direction relative to the camera,
 * overshooting and settling back for the paper-turn look (degrees); the hammer snaps instead.
 * Skipped while sideways.
 */
static void marioMakeDispDir(void) {
    MarioWork* wp = mp;
    f32 diff;
    f32 tgt;
    f32 step;
    // FAKE: c30 and c20 hold 30.0f and 20.0f for the negated uses below; the literals -30.0f and -20.0f change the object
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

/**
 * Switches Mario's or Peach's pose to its back-facing version while he moves away from the camera,
 * and back to the front version otherwise.
 */
static void marioRearAnime(void) {
    MarioWork* wp = mp;
    const char* name;
    f32 diff;

    if (!(wp->dispFlags & (MARIO_DISP_FLAG_PAPER | MARIO_DISP_FLAG_POSE_UNUSED | MARIO_DISP_FLAG_EVT_POSE | MARIO_DISP_FLAG_NO_REAR_POSE)) &&
        !(wp->flags & MARIO_FLAG_IS_CARRYING_BOBBERY)) {
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

/**
 * Per-frame display set-up: back-facing pose, facing turn, applies pending pose changes and steps the animation,
 * then queues marioDisp (blinking while invincible) or marioDispBlur and the shadow.
 * Bowser's and Peach's shadows are offset along the camera yaw (Peach's from the US release on).
 */
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
                dispEntry(marioGetCamId(), z, DISP_MODE_ALPHA_TEST, marioDisp, wp);
            }
        } else {
            dispEntry(marioGetCamId(), z, DISP_MODE_ALPHA_TEST, marioDisp, wp);
        }
    } else {
        dispEntry(marioGetCamId(), z, DISP_MODE_XLU, marioDispBlur, wp);
    }
    if (wp->dispFlags & MARIO_DISP_FLAG_SHADOW_OFF) {
        return;
    }
    if (wp->flags & MARIO_FLAG_BG_MODE) {
        return;
    }
    if (wp->currMotionId == MARIO_MOTION_PLANE) {
        shadowEntryMode(SHADOW_MODE_CYLINDER, wp->position.x, wp->groundY, wp->position.z, 13.333333f);
        return;
    }
    if (wp->characterId == CHARACTER_BOWSER) {
        size = 30.0f;
        xo = 14.0f * (f32)sin(3.1415927f * wp->targetYaw / 180.0f);
        dy = (f32)cos(3.1415927f * wp->targetYaw / 180.0f);
        zo = 14.0f * -dy;
    }
#if VERSION >= VERSION_US
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
        t = SHADOW_MODE_CYLINDER;
        if (bit) {
            t = SHADOW_MODE_POLYGON;
        }
        shadowEntryMode(t, wp->position.x + xo, wp->position.y, wp->position.z + zo, sz);
    } else {
        dy = wp->position.y - wp->groundY;
        if (dy >= -2.0f) {
            size -= dy / 13.333333f;
            if (size < 0.1f) {
                size = 0.0f;
            }
            if (size) {
                t = SHADOW_MODE_CYLINDER;
                if (wp->flags & MARIO_FLAG_BG_MODE) {
                    t = SHADOW_MODE_POLYGON;
                }
                shadowEntryMode(t, wp->position.x + xo, wp->position.y, wp->position.z + zo, size);
            }
        }
    }
}

/** TRUE when an angle in degrees lies from 90 to 270, i.e. the model is drawn mirrored. */
inline BOOL marioChkRevDir(f32 angle) {
    BOOL ret = FALSE;

    if (angle >= 90.0f && angle <= 270.0f) {
        ret = TRUE;
    }
    return ret;
}

/**
 * Draws one ghost of Mario's motion-blur trail at recorded position blurId, more transparent for a higher slot index;
 * used by marioDispBlur.
 */
static void marioDispBlurSub(Mtx unused, s32 blurId) {
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
    animPoseSetMaterialFlagOn(wp->paperAnimGroupId[marioAnimeId()], MAP_OBJ_FLAG_COLOR);
    animPoseSetMaterialEvtColor(wp->paperAnimGroupId[marioAnimeId()], (GXColor){0xFF, 0xFF, 0xFF, 0x80 - (blurId << 4)});
    animPoseDrawMtx(wp->paperAnimGroupId[marioAnimeId()], mtx, 1, 0.0f, 1.0f);
    animPoseDrawMtx(wp->paperAnimGroupId[marioAnimeId()], mtx, 2, 0.0f, 1.0f);
    animPoseDrawMtx(wp->paperAnimGroupId[marioAnimeId()], mtx, 3, 0.0f, 1.0f);
}

// FAKE: opt_dead_assignments off keeps the never-used sin/cos quadrant computation (s, c, sg, cg, sy from targetYaw) in marioDispBlur; removing the pragma or the code changes the object
#pragma opt_dead_assignments off

/**
 * Draw callback for Mario with motion blur: draws trail slots 7 through 1 as fading ghosts through
 * marioDispBlurSub. The angle values it computes are never used.
 */
static void marioDispBlur(CameraId camId, void* param) {
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
    animPoseSetMaterialFlagOff(wp->paperAnimGroupId[marioAnimeId()], MAP_OBJ_FLAG_COLOR);
    animPoseSetMaterialFlagOn(wp->paperAnimGroupId[marioAnimeId()], flag);
}
#pragma opt_dead_assignments reset

/**
 * Draw callback for Mario: builds his model matrix from scale, the facing mirror, rotation,
 * offsets and tube squash, draws the pose and runs the animation at 75% speed in shallow water.
 */
static void marioDisp(CameraId camId, void* param) {
    MarioWork* wp = mp;
    Mtx mtx;
    Mtx rotMtx;
    Mtx scaleMtx;
    f32 scale;
    f32 angle;
#if VERSION <= VERSION_DEMO_JP
    f32 angle2;
    f32 yofs = 0.0f;
#endif
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
#if VERSION <= VERSION_DEMO_JP
        yofs + (wp->dispOfs2.y + (wp->position.y + wp->dispOfs.y)),
#else
        wp->dispOfs2.y + (wp->position.y + wp->dispOfs.y),
#endif
        wp->dispOfs2.z + (wp->position.z + wp->dispOfs.z));
    MTXConcat(rotMtx, mtx, mtx);
    animPoseDrawMtx(wp->paperAnimGroupId[marioAnimeId()], mtx, 2, 0.0f, 1.0f);
    animPoseDrawMtx(wp->paperAnimGroupId[marioAnimeId()], mtx, 1, 0.0f, 1.0f);
    animPoseSetLocalTimeRate(wp->paperAnimGroupId[marioAnimeId()], (wp->flags & MARIO_FLAG_IN_SHALLOW_WATER) ? 0.75f : 1.0f);
}

/** Returns the back-facing version of a Mario or Peach pose name, or NULL if it has none; the PAL demo and PAL add M_I_3. */
static const char* toRearPose(const char* name) {
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
#if VERSION >= VERSION_DEMO_EU
        }
        else if (!strcmp(name, "M_I_3")) {
            rear = "M_I_3R";
        }
        else if (!strcmp(name, "M_U_2")) {
#else
        } else if (!strcmp(name, "M_U_2")) {
#endif
            rear = "M_U_2R";
        } else if (!strcmp(name, "M_C_1")) {
            rear = "M_C_1R";
        }
        return rear;
    }
}

/** Returns the front-facing version of a back-facing pose name (the reverse of toRearPose), or NULL. */
static const char* toFrontPose(const char* name) {
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
#if VERSION >= VERSION_DEMO_EU
        }
        else if (!strcmp(name, "M_I_3R")) {
            front = "M_I_3";
        }
        else if (!strcmp(name, "M_U_2R")) {
#else
        } else if (!strcmp(name, "M_U_2R")) {
#endif
            front = "M_U_2";
        } else if (!strcmp(name, "M_C_1R")) {
            front = "M_C_1";
        }
        return front;
    }
}
