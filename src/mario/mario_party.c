// the best Mario Party, partners with actual abilities in Paper Mario,
// something Nintendo's forgotten about
#include "mario/mario_party.h"
#include "mario/mario.h"
#include "party.h"
#include <dolphin/pad.h>
#include "driver/npcdrv.h"

s32 partyEntry(PartyId id);
s32 partyEntry2(PartyId id);
s32 partyEntryHello(PartyId id);
s32 partyEntry2Hello(PartyId id);
void partyGoodbye(s32 slot);
void partyKill(s32 slot);
s32 partyDoWork(void);
BOOL yoshiGetStatus(void);

static inline void marioPartyKillSlot0(void) {
    MarioWork* wp = marioGetPtr();
    if (wp->currPartySlotId[0] >= 0) {
        partyKill(wp->currPartySlotId[0]);
        wp->currPartySlotId[0] = -1;
    }
}

static inline void marioPartyKillSlot1(void) {
    MarioWork* wp = marioGetPtr();
    if (wp->currPartySlotId[1] >= 0) {
        partyKill(wp->currPartySlotId[1]);
        wp->currPartySlotId[1] = -1;
    }
}

s32 marioUseParty(void) {
    MarioWork* wp = marioGetPtr();
    PartyEntry* party;

    if (N_marioChkUseParty()) {
        wp->partyUseDelay = 0;
        wp->trigFlags &= ~MARIO_TRIG_FLAG_USE_PARTNER;
        return 0;
    }
    if ((wp->trigFlags & MARIO_TRIG_FLAG_USE_PARTNER) && marioKeyOffChk()) {
        wp->partyUseDelay = 0;
        wp->trigFlags &= ~MARIO_TRIG_FLAG_USE_PARTNER;
        return 0;
    }
    party = partyGetPtr(marioGetPartyId());
    if (party != NULL && party->runMode == 3) {
        return 0;
    }
    if (wp->currPartySlotId[0] < 0) {
        wp->trigFlags &= ~MARIO_TRIG_FLAG_USE_PARTNER;
        return 0;
    }
    if (wp->flags & MARIO_FLAG_IS_8_BIT) {
        return 0;
    }
    if (N_fbatPreventMarioEventChk()) {
        wp->partyUseDelay = 0;
        wp->trigFlags &= ~MARIO_TRIG_FLAG_USE_PARTNER;
        return 0;
    }
    if ((wp->flags & (MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING)) || wp->airTimer != 0) {
        wp->partyUseDelay = 0;
        wp->trigFlags &= ~MARIO_TRIG_FLAG_USE_PARTNER;
        return 0;
    }
    if (wp->currMotionId != MARIO_MOTION_STAY && wp->currMotionId != MARIO_MOTION_WALK && wp->currMotionId != MARIO_MOTION_DASH) {
        wp->trigFlags &= ~MARIO_TRIG_FLAG_USE_PARTNER;
        return 0;
    }
    if ((wp->dispFlags & 0x1000000) || party->slitOn == 1) {
        return 0;
    }
    if (partyGetPtr(marioGetPartyId())->flags & 0x100) {
        return 0;
    }
    party = partyGetPtr(marioGetPartyId());
    if (party == NULL) {
        return 0;
    }
    if (party->currentMemberId == 1 && !(party->flags & 2)) {
        return 0;
    }
    if (party->currentMemberId == 4 && yoshiGetStatus()) {
        return 0;
    }
    if (!(wp->trigFlags & MARIO_TRIG_FLAG_USE_PARTNER) && (wp->buttonTrg & PAD_BUTTON_X)) {
        wp->trigFlags |= MARIO_TRIG_FLAG_USE_PARTNER;
        wp->partyUseDelay = 5;
    }
    return 0;
}

s32 marioUsePartyMain(void) {
    MarioWork* wp = marioGetPtr();

    if (wp->trigFlags & MARIO_TRIG_FLAG_USE_PARTNER) {
        if (marioKeyOffChk()) {
            wp->partyUseDelay = 0;
            wp->trigFlags &= ~MARIO_TRIG_FLAG_USE_PARTNER;
            return 0;
        }
        if (--wp->partyUseDelay <= 0) {
            wp->partyUseDelay = 0;
            wp->trigFlags &= ~MARIO_TRIG_FLAG_USE_PARTNER;
            return partyDoWork();
        }
    }
    return 0;
}

s32 marioPartyEntry(PartyId id) {
    MarioWork* wp = marioGetPtr();
    s32 ret;

    if (id < 8) {
        if (!partyChkJoin(id)) {
            return -1;
        }
        if (wp->currPartySlotId[0] >= 0) {
            marioPartyKillSlot0();
        }
        ret = partyEntry(id);
        if (ret >= 0) {
            wp->currPartySlotId[0] = ret;
            partyHello(partyGetPtr(ret));
        }
    } else {
        if (wp->currPartySlotId[1] >= 0) {
            marioPartyKillSlot1();
        }
        ret = partyEntry2(id);
        if (ret >= 0) {
            wp->currPartySlotId[1] = ret;
            partyHello(partyGetPtr(ret));
        }
    }
    return ret;
}

s32 marioPartyHello(PartyId id) {
    s32 ret = -1;
    MarioWork* wp = marioGetPtr();

    if (id < 8) {
        if (!partyChkJoin(id)) {
            return -1;
        }
        if (wp->currPartySlotId[0] < 0) {
            ret = partyEntryHello(id);
            if (ret >= 0) {
                wp->currPartySlotId[0] = ret;
            }
        }
    } else {
        ret = partyEntry2Hello(id);
        if (ret >= 0) {
            wp->currPartySlotId[1] = ret;
        }
    }
    return ret;
}

BOOL marioPartyGoodbye(void) {
    BOOL ret = FALSE;
    s8 slot = marioGetPtr()->currPartySlotId[0];
    if (slot >= 0) {
        partyGoodbye(slot);
        ret = TRUE;
    }
    return ret;
}

void marioPartyKill(void) {
    marioPartyKillSlot0();
    marioPartyKillSlot1();
}

s32 marioGetParty(void) {
    s32 id;

    id = marioGetPartyId();
    if (id >= 0) {
        return partyGetPtr(id)->currentMemberId;
    } else {
        return 0;
    }
}

s32 marioGetPartyId(void) {
    MarioWork* wp = marioGetPtr();
    PartyEntry* party;
    s32 partyId;

    partyId = wp->currPartySlotId[0];
    if (partyId >= 0) {
        party = partyGetPtr(partyId);
        if ((party != NULL) && !(party->flags & 8)) {
            return partyId;
        }
    }
    partyId = wp->currPartySlotId[1];
    if (partyId >= 0) {
        party = partyGetPtr(partyId);
        if ((party != NULL) && !(party->flags & 8)) {
            return partyId;
        }
    }
    return -1;
}

s32 marioGetExtraPartyId(void) {
    MarioWork* wp = marioGetPtr();
    PartyEntry* party;
    s32 partyId;

    partyId = wp->currPartySlotId[0];
    if (partyId >= 0) {
        party = partyGetPtr(partyId);
        if ((party != NULL) && (party->flags & 8)) {
            return partyId;
        }
    }

    partyId = wp->currPartySlotId[1];
    if (partyId >= 0) {
        party = partyGetPtr(partyId);
        if ((party != NULL) && (party->flags & 8)) {
            return partyId;
        }
    }
    return -1;
}

void partyJoin(PartyId partnerId) {
    if (partnerId < 8) {
        pouchGetPtr()->partyData[partnerId].flags |= 1; // obtained
    }
}

void partyLeft(PartyId partnerId) {
    if (partnerId < 8) {
        pouchGetPtr()->partyData[partnerId].flags &= ~1;
    }
}

BOOL partyChkJoin(PartyId partnerId) {
    BOOL ret = FALSE;
    if (partnerId < 8) {
        u32 joined = pouchGetPtr()->partyData[partnerId].flags & 1;
        ret = joined;
    }
    return ret;
}

s32 partyGetHp(PartyId partnerId) {
    s32 ret = 0;
    if (partnerId < 8) {
        ret = pouchGetPtr()->partyData[partnerId].currentHP;
    }
    return ret;
}

s32 partyGetTechLv(PartyId partnerId) {
    s32 ret = 0;
    if (partnerId < 8) {
        ret = pouchGetPtr()->partyData[partnerId].techLevel;
    }
    return ret;
}
