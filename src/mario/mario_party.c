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

/**
 * JP and later partner button check: X arms MARIO_TRIG_FLAG_USE_PARTNER with a 5-frame partyUseDelay (counted down by
 * marioUsePartyMain) while Mario stands, walks or dashes on the ground with a partner that can act (not in Paper Mode
 * or already in use, Goombella grounded, Yoshi idle; not as 8-bit Mario). Clears the trigger on a D-pad or Start press,
 * while control or (once armed) keys are off, while fbatPreventMarioEventChk is set, in the air, in other motions or
 * with no partner slot. Returns 0.
 */
#if VERSION >= VERSION_JP
s32 marioUseParty(void) {
    MarioWork* mario = marioGetPtr();
    PartyEntry* party;

    if (marioChkUseParty()) {
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
    if (party->currentMemberId == PARTY_YOSHI && yoshiGetStatus()) {
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
            marioPartyKillOne(0);
        }
        ret = partyEntry(id);
        if (ret >= 0) {
            mario->currPartySlotId[0] = ret;
            partyHello(partyGetPtr(ret));
        }
    } else {
        if (mario->currPartySlotId[1] >= 0) {
            marioPartyKillOne(1);
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

/** Sends the follower in slot 1 away with the leaving animation. TRUE if there was one. */
// Original name and size (0x48) from the JP demo map (UNUSED there: nothing calls it); marioPartyGoodbye for the
// second party slot.
BOOL marioParty2Goodbye(void) {
    BOOL ret = FALSE;
    s8 slot = marioGetPtr()->currPartySlotId[1];
    if (slot >= 0) {
        partyGoodbye(slot);
        ret = TRUE;
    }
    return ret;
}

/** Removes both the partner and the follower at once with no animation (save load, end of battle, E3 sequence). */
void marioPartyKill(void) {
    marioPartyKillOne(0);
    marioPartyKillOne(1);
}

/**
 * Removes the member in Mario's party slot (0 the partner, 1 the follower) with no animation and marks the
 * slot empty (-1); TRUE if the slot held one. Original name from the JP demo's link map (UNUSED there, 0x60):
 * every call is inlined, so the linker strips it. No caller reads the result: the BOOL return follows the sibling
 * marioPartyGoodbye; the bytes allow returning the removed slot id or -1 too.
 */
BOOL marioPartyKillOne(s32 slot) {
    BOOL ret = FALSE;
    MarioWork* mario = marioGetPtr();
    if (mario->currPartySlotId[slot] >= 0) {
        partyKill(mario->currPartySlotId[slot]);
        mario->currPartySlotId[slot] = -1;
        ret = TRUE;
    }
    return ret;
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
    if (party->currentMemberId == PARTY_GOOMBELLA && !(party->flags & PARTY_FLAG_IS_GROUNDED)) {
        return 0;
    }
    if (party->currentMemberId == PARTY_YOSHI && yoshiGetStatus()) {
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

/** Sets the partner's current HP in the pouch (nothing for invalid ids). */
// Original name and size (0x4c) from the JP demo map (UNUSED there: nothing calls it); the setter of partyGetHp's
// field with the same id check.
void partySetHp(s32 partnerId, s32 value) {
    if (partnerId < PARTNER_COUNT) {
        pouchGetPtr()->partyData[partnerId].currentHP = value;
    }
}

/** The partner's HP level (rank) from the pouch (0 for invalid ids). */
// Original name and size (0x44) from the JP demo map (UNUSED there: nothing calls it); partyGetHp for another
// PouchPartyData field.
s32 partyGetHpLv(s32 partnerId) {
    s32 ret = 0;
    if (partnerId < PARTNER_COUNT) {
        ret = pouchGetPtr()->partyData[partnerId].HPLevel;
    }
    return ret;
}

/** Sets the partner's HP level (rank) in the pouch (nothing for invalid ids). */
// Original name and size (0x4c) from the JP demo map (UNUSED there: nothing calls it); the setter of partyGetHpLv's
// field with the same id check.
void partySetHpLv(s32 partnerId, s32 value) {
    if (partnerId < PARTNER_COUNT) {
        pouchGetPtr()->partyData[partnerId].HPLevel = value;
    }
}

/** The partner's max HP (with HP Plus P) from the pouch (0 for invalid ids). */
// Original name and size (0x44) from the JP demo map (UNUSED there: nothing calls it); partyGetHp for another
// PouchPartyData field.
s32 partyGetMaxHp(s32 partnerId) {
    s32 ret = 0;
    if (partnerId < PARTNER_COUNT) {
        ret = pouchGetPtr()->partyData[partnerId].maximumHP;
    }
    return ret;
}

/** Sets the partner's max HP (with HP Plus P) in the pouch (nothing for invalid ids). */
// Original name and size (0x4c) from the JP demo map (UNUSED there: nothing calls it); the setter of partyGetMaxHp's
// field with the same id check.
void partySetMaxHp(s32 partnerId, s32 value) {
    if (partnerId < PARTNER_COUNT) {
        pouchGetPtr()->partyData[partnerId].maximumHP = value;
    }
}

/** The partner's max HP without badges from the pouch (0 for invalid ids). */
// Original name and size (0x44) from the JP demo map (UNUSED there: nothing calls it); partyGetHp for another
// PouchPartyData field.
s32 partyGetMaxHpBase(s32 partnerId) {
    s32 ret = 0;
    if (partnerId < PARTNER_COUNT) {
        ret = pouchGetPtr()->partyData[partnerId].baseMaxHP;
    }
    return ret;
}

/** Sets the partner's max HP without badges in the pouch (nothing for invalid ids). */
// Original name and size (0x4c) from the JP demo map (UNUSED there: nothing calls it); the setter of partyGetMaxHpBase's
// field with the same id check.
void partySetMaxHpBase(s32 partnerId, s32 value) {
    if (partnerId < PARTNER_COUNT) {
        pouchGetPtr()->partyData[partnerId].baseMaxHP = value;
    }
}

/** The partner's attack level (rank) from the pouch (0 for invalid ids). */
// Original name and size (0x44) from the JP demo map (UNUSED there: nothing calls it); partyGetHp for another
// PouchPartyData field.
s32 partyGetAttackLv(s32 partnerId) {
    s32 ret = 0;
    if (partnerId < PARTNER_COUNT) {
        ret = pouchGetPtr()->partyData[partnerId].attackLevel;
    }
    return ret;
}

/** Sets the partner's attack level (rank) in the pouch (nothing for invalid ids). */
// Original name and size (0x4c) from the JP demo map (UNUSED there: nothing calls it); the setter of partyGetAttackLv's
// field with the same id check.
void partySetAttackLv(s32 partnerId, s32 value) {
    if (partnerId < PARTNER_COUNT) {
        pouchGetPtr()->partyData[partnerId].attackLevel = value;
    }
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

/** Sets the partner's rank in the pouch (nothing for invalid ids). */
// Original name and size (0x4c) from the JP demo map (UNUSED there: nothing calls it); the setter of partyGetTechLv's
// field with the same id check.
void partySetTechLv(s32 partnerId, s32 value) {
    if (partnerId < PARTNER_COUNT) {
        pouchGetPtr()->partyData[partnerId].techLevel = value;
    }
}
