// Mario's side of the partners: which partner is out, calling them (entry/hello/goodbye/kill), the partner
// button, and partner join/leave, HP and level helpers.

#include "version.h"
#include "mario/mario_party.h"
#include "mario/mario_pouch.h"
#include "mario/mario.h"
#include "party.h"
#include <dolphin/pad.h>
#include "driver/npcdrv.h"
#include "party/party_yoshi.h"


/** Removes the partner in Mario's first party slot (currPartySlotId[0]) and marks the slot empty (-1). */
static inline void marioPartyKillSlot0(void) {
    MarioWork* mario = marioGetPtr();
    if (mario->currPartySlotId[0] >= 0) {
        partyKill(mario->currPartySlotId[0]);
        mario->currPartySlotId[0] = -1;
    }
}

/** Removes the follower in Mario's second party slot (currPartySlotId[1], the partyEntry2 members) and empties it. */
static inline void marioPartyKillSlot1(void) {
    MarioWork* mario = marioGetPtr();
    if (mario->currPartySlotId[1] >= 0) {
        partyKill(mario->currPartySlotId[1]);
        mario->currPartySlotId[1] = -1;
    }
}

#if VERSION >= VERSION_JP
s32 marioUseParty(void) {
    MarioWork* mario = marioGetPtr();
    PartyEntry* party;

    if (N_marioChkUseParty()) {
        mario->partyUseDelay = 0;
        mario->trigFlags &= ~MARIO_TRIG_FLAG_USE_PARTNER;
        return 0;
    }
    if ((mario->trigFlags & MARIO_TRIG_FLAG_USE_PARTNER) && marioKeyOffChk()) {
        mario->partyUseDelay = 0;
        mario->trigFlags &= ~MARIO_TRIG_FLAG_USE_PARTNER;
        return 0;
    }
    party = partyGetPtr(marioGetPartyId());
    if (party != NULL && party->runMode == PARTY_RUN_USE) {
        return 0;
    }
    if (mario->currPartySlotId[0] < 0) {
        mario->trigFlags &= ~MARIO_TRIG_FLAG_USE_PARTNER;
        return 0;
    }
    if (mario->flags & MARIO_FLAG_IS_8_BIT) {
        return 0;
    }
    if (fbatPreventMarioEventChk()) {
        mario->partyUseDelay = 0;
        mario->trigFlags &= ~MARIO_TRIG_FLAG_USE_PARTNER;
        return 0;
    }
    if ((mario->flags & (MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING)) || mario->airTimer != 0) {
        mario->partyUseDelay = 0;
        mario->trigFlags &= ~MARIO_TRIG_FLAG_USE_PARTNER;
        return 0;
    }
    if (mario->currMotionId != MARIO_MOTION_STAY && mario->currMotionId != MARIO_MOTION_WALK && mario->currMotionId != MARIO_MOTION_DASH) {
        mario->trigFlags &= ~MARIO_TRIG_FLAG_USE_PARTNER;
        return 0;
    }

    // party can be NULL (marioGetPartyId may return -1 even with a partner slot set); it is dereferenced here before the NULL check below.
    if ((mario->dispFlags & MARIO_DISP_FLAG_SLIT) || party->slitOn == 1) {
        return 0;
    }
    if (partyGetPtr(marioGetPartyId())->flags & PARTY_FLAG_IS_BEING_USED) {
        return 0;
    }
    party = partyGetPtr(marioGetPartyId());
    if (party == NULL) {
        return 0;
    }

    // 4 = PARTY_YOSHI: the char-sized enum constant changes the s8 compare
    if (party->currentMemberId == PARTY_GOOMBELLA && !(party->flags & PARTY_FLAG_IS_GROUNDED)) {
        return 0;
    }
    if (party->currentMemberId == 4 && yoshiGetStatus()) {
        return 0;
    }
    if (!(mario->trigFlags & MARIO_TRIG_FLAG_USE_PARTNER) && (mario->buttonTrg & PAD_BUTTON_X)) {
        mario->trigFlags |= MARIO_TRIG_FLAG_USE_PARTNER;
        mario->partyUseDelay = 5;
    }
    return 0;
}
#endif

// Descriptive name (not in the symbol maps; docs/NAME_PROVENANCE.md).
#if VERSION >= VERSION_JP

/**
 * Runs from partyMain while a partner is out: after the partner button,
 * counts down partyUseDelay and then calls partyDoWork to use the partner's field ability (returns its result,
 * else 0). Losing control (marioKeyOffChk) cancels.
 */
s32 marioUsePartyMain(void) {
    MarioWork* mario = marioGetPtr();

    if (mario->trigFlags & MARIO_TRIG_FLAG_USE_PARTNER) {
        if (marioKeyOffChk()) {
            mario->partyUseDelay = 0;
            mario->trigFlags &= ~MARIO_TRIG_FLAG_USE_PARTNER;
            return 0;
        }
        if (--mario->partyUseDelay <= 0) {
            mario->partyUseDelay = 0;
            mario->trigFlags &= ~MARIO_TRIG_FLAG_USE_PARTNER;
            return partyDoWork();
        }
    }
    return 0;
}
#endif

/**
 * Puts a party member out at once, replacing whoever held the slot:
 * ids below 8 are partners (must have joined, else -1) and go to slot 0,
 * higher ids are followers (partyEntry2) in slot 1.
 * Used on save load, after battles and in the E3 sequence; returns the party slot or -1.
 */
s32 marioPartyEntry(s32 id) {
    MarioWork* mario = marioGetPtr();
    s32 ret;

    if (id < PARTNER_COUNT) {
        if (!partyChkJoin(id)) {
            return -1;
        }
        if (mario->currPartySlotId[0] >= 0) {
            marioPartyKillSlot0();
        }
        ret = partyEntry(id);
        if (ret >= 0) {
            mario->currPartySlotId[0] = ret;
            partyHello(partyGetPtr(ret));
        }
    } else {
        if (mario->currPartySlotId[1] >= 0) {
            marioPartyKillSlot1();
        }
        ret = partyEntry2(id);
        if (ret >= 0) {
            mario->currPartySlotId[1] = ret;
            partyHello(partyGetPtr(ret));
        }
    }
    return ret;
}

/**
 * Calls a partner out with the arrival animation (partyEntryHello) if slot 0 is empty,
 * or a follower into slot 1.
 * The partner menu (win_main) uses it after marioPartyGoodbye when switching partners. Returns the slot or -1.
 */
s32 marioPartyHello(s32 id) {
    s32 ret = -1;
    MarioWork* mario = marioGetPtr();

    if (id < PARTNER_COUNT) {
        if (!partyChkJoin(id)) {
            return -1;
        }
        if (mario->currPartySlotId[0] < 0) {
            ret = partyEntryHello(id);
            if (ret >= 0) {
                mario->currPartySlotId[0] = ret;
            }
        }
    } else {
        ret = partyEntry2Hello(id);
        if (ret >= 0) {
            mario->currPartySlotId[1] = ret;
        }
    }
    return ret;
}

/**
 * Sends the current partner in slot 0 away with the leaving animation (partyGoodbye)
 * when switching partners in the partner menu. TRUE if there was a partner to send away.
 */
BOOL marioPartyGoodbye(void) {
    BOOL ret = FALSE;
    s8 slot = marioGetPtr()->currPartySlotId[0];
    if (slot >= 0) {
        partyGoodbye(slot);
        ret = TRUE;
    }
    return ret;
}

/** Removes both the partner and the follower at once with no animation (save load, end of battle, E3 sequence). */
void marioPartyKill(void) {
    marioPartyKillSlot0();
    marioPartyKillSlot1();
}

#if VERSION <= VERSION_DEMO_JP

/**
 * Early-demo partner button check: X arms MARIO_TRIG_FLAG_JABARA_CHK with a 5-frame partyUseDelay
 * while Mario stands, walks or dashes with a usable partner.
 * Subsequent calls count down the delay and return partyDoWork's result when it expires.
 * Losing control clears the trigger; jumping, falling and airTimer also block or cancel it.
 */
s32 marioUseParty(void) {
    MarioWork* mario = marioGetPtr();
    PartyEntry* party;

    if ((mario->trigFlags & MARIO_TRIG_FLAG_JABARA_CHK) && marioKeyOffChk()) {
        mario->trigFlags &= ~MARIO_TRIG_FLAG_JABARA_CHK;
        return 0;
    }
    if (mario->currPartySlotId[0] < 0) {
        mario->trigFlags &= ~MARIO_TRIG_FLAG_JABARA_CHK;
        return 0;
    }
    if (mario->flags & MARIO_FLAG_IS_8_BIT) {
        return 0;
    }
    if ((mario->flags & (MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING)) || mario->airTimer != 0) {
        mario->trigFlags &= ~MARIO_TRIG_FLAG_JABARA_CHK;
        return 0;
    }
    if (mario->currMotionId != MARIO_MOTION_STAY && mario->currMotionId != MARIO_MOTION_WALK && mario->currMotionId != MARIO_MOTION_DASH) {
        mario->trigFlags &= ~MARIO_TRIG_FLAG_JABARA_CHK;
        return 0;
    }
    if (mario->dispFlags & MARIO_DISP_FLAG_SLIT) {
        return 0;
    }
    if (partyGetPtr(marioGetPartyId())->flags & PARTY_FLAG_IS_BEING_USED) {
        return 0;
    }
    party = partyGetPtr(marioGetPartyId());
    if (party == NULL) {
        return 0;
    }

    // 1 = PARTY_GOOMBELLA, 4 = PARTY_YOSHI: the char-sized enum constants change the s8 compares
    if (party->currentMemberId == 1 && !(party->flags & PARTY_FLAG_IS_GROUNDED)) {
        return 0;
    }
    if (party->currentMemberId == 4 && yoshiGetStatus()) {
        return 0;
    }
    if (!(mario->trigFlags & MARIO_TRIG_FLAG_JABARA_CHK)) {
        if (mario->buttonTrg & PAD_BUTTON_X) {
            mario->trigFlags |= MARIO_TRIG_FLAG_JABARA_CHK;
            mario->partyUseDelay = 5;
        }
    } else if (--mario->partyUseDelay <= 0) {
        mario->partyUseDelay = 0;
        mario->trigFlags &= ~MARIO_TRIG_FLAG_JABARA_CHK;
        return partyDoWork();
    }
    return 0;
}
#endif

/**
 * The PartyMember kind of the partner out with Mario (PARTY_GOOMBELLA, ...), or 0 when none.
 * Used by the pause menu, item menu and status window.
 */
s32 marioGetParty(void) {
    s32 id;

    id = marioGetPartyId();
    if (id >= 0) {
        return partyGetPtr(id)->currentMemberId;
    } else {
        return 0;
    }
}

/**
 * The party slot of Mario's partner: the first of his two slots whose entry is not a follower
 * (PARTY_FLAG_IS_PARTNER is set by partyEntry2 on followers). -1 when no partner is out. Pass it to partyGetPtr.
 */
s32 marioGetPartyId(void) {
    MarioWork* mario = marioGetPtr();
    PartyEntry* party;
    s32 partyId;

    partyId = mario->currPartySlotId[0];
    if (partyId >= 0) {
        party = partyGetPtr(partyId);
        if ((party != NULL) && !(party->flags & PARTY_FLAG_IS_PARTNER)) {
            return partyId;
        }
    }
    partyId = mario->currPartySlotId[1];
    if (partyId >= 0) {
        party = partyGetPtr(partyId);
        if ((party != NULL) && !(party->flags & PARTY_FLAG_IS_PARTNER)) {
            return partyId;
        }
    }
    return -1;
}

/**
 * The party slot of the follower walking with Mario (a member added by partyEntry2,
 * flagged PARTY_FLAG_IS_PARTNER), or -1. The counterpart of marioGetPartyId.
 */
s32 marioGetExtraPartyId(void) {
    MarioWork* mario = marioGetPtr();
    PartyEntry* party;
    s32 partyId;

    partyId = mario->currPartySlotId[0];
    if (partyId >= 0) {
        party = partyGetPtr(partyId);
        if ((party != NULL) && (party->flags & PARTY_FLAG_IS_PARTNER)) {
            return partyId;
        }
    }

    partyId = mario->currPartySlotId[1];
    if (partyId >= 0) {
        party = partyGetPtr(partyId);
        if ((party != NULL) && (party->flags & PARTY_FLAG_IS_PARTNER)) {
            return partyId;
        }
    }
    return -1;
}

/**
 * Marks a partner as recruited in the pouch (partyData flag 1), so it can be called out.
 * event.c's story-stage setup calls it for each partner's join point (Goombella, Koops, Flurrie, Yoshi, Vivian,
 * Bobbery, Ms. Mowz).
 */
void partyJoin(s32 partnerId) {
    if (partnerId < PARTNER_COUNT) {
        pouchGetPtr()->partyData[partnerId].flags |= POUCH_PARTY_FLAG_JOINED; // obtained
    }
}

/**
 * Clears a partner's recruited flag in the pouch.
 * event.c's chapter 4 stage setup clears Goombella, Koops, Flurrie and Yoshi while Doopliss has them,
 * and partyJoin restores them a few stages later.
 */
void partyLeft(s32 partnerId) {
    if (partnerId < PARTNER_COUNT) {
        pouchGetPtr()->partyData[partnerId].flags &= ~POUCH_PARTY_FLAG_JOINED;
    }
}

/** TRUE when the partner has been recruited (pouch partyData flag 1); FALSE for ids 8 and up (followers). */
BOOL partyChkJoin(s32 partnerId) {
    BOOL ret = FALSE;
    if (partnerId < PARTNER_COUNT) {
        u32 joined = pouchGetPtr()->partyData[partnerId].flags & POUCH_PARTY_FLAG_JOINED;
        ret = joined;
    }
    return ret;
}

/**
 * The partner's current HP from the pouch (0 for invalid ids);
 * party.c compares it with PARTY_PINCH_HP for the low-HP animations.
 */
s32 partyGetHp(s32 partnerId) {
    s32 ret = 0;
    if (partnerId < PARTNER_COUNT) {
        ret = pouchGetPtr()->partyData[partnerId].currentHP;
    }
    return ret;
}

/**
 * The partner's rank (pouch techLevel: 0 unranked, 1 and 2 the rank-ups Merlon sells for Shine Sprites in
 * gor_02, which unitwin_setpartylank applies to HP, attack and tech levels together).
 * The partner battle units read it to pick their moves; 0 for invalid ids.
 */
s32 partyGetTechLv(s32 partnerId) {
    s32 ret = 0;
    if (partnerId < PARTNER_COUNT) {
        ret = pouchGetPtr()->partyData[partnerId].techLevel;
    }
    return ret;
}
