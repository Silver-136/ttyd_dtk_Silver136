#include "mario/mario_party.h"


s32 marioUseParty(void) {
    extern void* marioGetPtr(void);
    extern s32 N_marioChkUseParty(void);
    extern s32 marioKeyOffChk(void);
    extern void* partyGetPtr(s32 partyId);
    extern s32 N_fbatPreventMarioEventChk(void);
    extern s32 yoshiGetStatus(void);
    void* mario;
    void* mario2;
    void* party;
    s32 partyId;
    u16 motion;

    mario = marioGetPtr();
    if (N_marioChkUseParty() != 0) {
        *(u8*)((s32)mario + 0x3B) = 0;
        *(u32*)((s32)mario + 0xC) &= ~2;
        return 0;
    }

    if ((*(u32*)((s32)mario + 0xC) & 2) != 0 && marioKeyOffChk() != 0) {
        *(u8*)((s32)mario + 0x3B) = 0;
        *(u32*)((s32)mario + 0xC) &= ~2;
        return 0;
    }

    mario2 = marioGetPtr();
    partyId = *(s8*)((s32)mario2 + 0x245);
    if (partyId >= 0) {
        party = partyGetPtr(partyId);
        if (party != 0 && (*(u32*)party & 8) == 0) {
            goto got_party;
        }
    }
    partyId = *(s8*)((s32)mario2 + 0x246);
    if (partyId >= 0) {
        party = partyGetPtr(partyId);
        if (party != 0 && (*(u32*)party & 8) == 0) {
            goto got_party;
        }
    }
    partyId = -1;
got_party:
    party = partyGetPtr(partyId);
    if (party != 0 && *(u8*)((s32)party + 0x34) == 3) {
        return 0;
    }

    if (*(s8*)((s32)mario + 0x245) < 0) {
        *(u32*)((s32)mario + 0xC) &= ~2;
        return 0;
    }
    if ((*(u32*)mario & 0x80000000) != 0) {
        return 0;
    }
    if (N_fbatPreventMarioEventChk() != 0) {
        *(u8*)((s32)mario + 0x3B) = 0;
        *(u32*)((s32)mario + 0xC) &= ~2;
        return 0;
    }
    if ((*(u32*)mario & 0x30000) != 0 || *(s16*)((s32)mario + 0x50) != 0) {
        *(u8*)((s32)mario + 0x3B) = 0;
        *(u32*)((s32)mario + 0xC) &= ~2;
        return 0;
    }

    motion = *(u16*)((s32)mario + 0x2E);
    if (motion != 0 && motion != 1 && motion != 2) {
        *(u32*)((s32)mario + 0xC) &= ~2;
        return 0;
    }
    if ((*(u32*)((s32)mario + 4) & 0x01000000) != 0) {
        return 0;
    }
    if (party != 0 && *(u8*)((s32)party + 0x37) == 1) {
        return 0;
    }

    mario2 = marioGetPtr();
    partyId = *(s8*)((s32)mario2 + 0x245);
    if (partyId >= 0) {
        party = partyGetPtr(partyId);
        if (party != 0 && (*(u32*)party & 8) == 0) {
            goto got_party2;
        }
    }
    partyId = *(s8*)((s32)mario2 + 0x246);
    if (partyId >= 0) {
        party = partyGetPtr(partyId);
        if (party != 0 && (*(u32*)party & 8) == 0) {
            goto got_party2;
        }
    }
    partyId = -1;
got_party2:
    party = partyGetPtr(partyId);
    if ((*(u32*)party & 0x100) != 0) {
        return 0;
    }

    mario2 = marioGetPtr();
    partyId = *(s8*)((s32)mario2 + 0x245);
    if (partyId >= 0) {
        party = partyGetPtr(partyId);
        if (party != 0 && (*(u32*)party & 8) == 0) {
            goto got_party3;
        }
    }
    partyId = *(s8*)((s32)mario2 + 0x246);
    if (partyId >= 0) {
        party = partyGetPtr(partyId);
        if (party != 0 && (*(u32*)party & 8) == 0) {
            goto got_party3;
        }
    }
    partyId = -1;
got_party3:
    party = partyGetPtr(partyId);
    if (party == 0) {
        return 0;
    }
    if (*(u8*)((s32)party + 0x31) == 1 && (*(u32*)party & 0x10) == 0) {
        return 0;
    }
    if (*(u8*)((s32)party + 0x31) == 6 && yoshiGetStatus() != 0) {
        return 0;
    }
    if ((*(u32*)((s32)mario + 0xC) & 2) == 0 && (*(u16*)((s32)mario + 0x24C) & 0x400) != 0) {
        *(u32*)((s32)mario + 0xC) |= 2;
        *(u8*)((s32)mario + 0x3B) = 5;
    }

    return 0;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 unk_8014140c(void) {
    extern void* marioGetPtr(void);
    extern s32 marioKeyOffChk(void);
    extern s32 partyDoWork(void);
    void* mario = marioGetPtr();
    s32 timer;

    if ((*(u32*)((s32)mario + 0xC) & 2) != 0) {
        if (marioKeyOffChk() != 0) {
            *(u8*)((s32)mario + 0x3B) = 0;
            *(u32*)((s32)mario + 0xC) &= ~2;
            return 0;
        }
        timer = *(u8*)((s32)mario + 0x3B) - 1;
        *(u8*)((s32)mario + 0x3B) = timer;
        if ((s8)timer <= 0) {
            *(u8*)((s32)mario + 0x3B) = 0;
            *(u32*)((s32)mario + 0xC) &= ~2;
            return partyDoWork();
        }
    }
    return 0;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
int marioPartyEntry(int partyMemberId) {
    extern void* marioGetPtr(void);
    extern void* pouchGetPtr(void);
    extern s32 partyEntry(s32 partyId);
    extern s32 partyEntry2(s32 partyId);
    extern s32 partyKill(s32 partyId);
    extern void* partyGetPtr(s32 partyId);
    extern void partyHello(void* party);
    void* mario;
    s32 result;
    s32 joined;

    mario = marioGetPtr();
    if (partyMemberId < 8) {
        joined = 0;
        if (partyMemberId < 8) {
            joined = ((u16*)pouchGetPtr())[partyMemberId * 7] & 1;
        }
        if (joined == 0) {
            return -1;
        }
        if (*(s8*)((s32)mario + 0x245) >= 0) {
            void* mario2 = marioGetPtr();
            u32 partyId = *(u8*)((s32)mario2 + 0x245);

            if ((s8)partyId >= 0) {
                partyKill((s8)partyId);
                *(s8*)((s32)mario2 + 0x245) = -1;
            }
        }
        result = partyEntry(partyMemberId);
        if (result >= 0) {
            *(u8*)((s32)mario + 0x245) = result;
            partyHello(partyGetPtr(result));
        }
    } else {
        if (*(s8*)((s32)mario + 0x246) >= 0) {
            void* mario2 = marioGetPtr();
            u32 partyId = *(u8*)((s32)mario2 + 0x246);

            if ((s8)partyId >= 0) {
                partyKill((s8)partyId);
                *(s8*)((s32)mario2 + 0x246) = -1;
            }
        }
        result = partyEntry2(partyMemberId);
        if (result >= 0) {
            *(u8*)((s32)mario + 0x246) = result;
            partyHello(partyGetPtr(result));
        }
    }
    return result;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
int marioPartyHello(int partyMemberId) {
    extern void* marioGetPtr(void);
    extern void* pouchGetPtr(void);
    extern s32 partyEntryHello(s32 partyId);
    extern s32 partyEntry2Hello(s32 partyId);
    void* mario;
    s32 result = -1;
    s32 joined;

    mario = marioGetPtr();
    if (partyMemberId < 8) {
        joined = 0;
        if (partyMemberId < 8) {
            joined = ((u16*)pouchGetPtr())[partyMemberId * 7] & 1;
        }
        if (joined == 0) {
            return -1;
        }
        if (*(s8*)((s32)mario + 0x245) < 0) {
            result = partyEntryHello(partyMemberId);
            if (result >= 0) {
                *(u8*)((s32)mario + 0x245) = result;
            }
        }
    } else {
        result = partyEntry2Hello(partyMemberId);
        if (result >= 0) {
            *(u8*)((s32)mario + 0x246) = result;
        }
    }
    return result;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


u32 marioPartyGoodbye(void) {
    extern void* marioGetPtr(void);
    extern void partyGoodbye(s32 partyId);
    s32 result = 0;
    u32 partyId = *(u8*)((s32)marioGetPtr() + 0x245);

    if ((s8)partyId >= 0) {
        partyGoodbye((s8)partyId);
        result = 1;
    }
    return result;
}

void marioPartyKill(void) {
    extern void* marioGetPtr(void);
    extern void partyKill(s32 partyId);
    void* mario;
    u32 partyId;

    mario = marioGetPtr();
    partyId = *(u8*)((s32)mario + 0x245);
    if ((s8)partyId >= 0) {
        partyKill((s8)partyId);
        *(s8*)((s32)mario + 0x245) = -1;
    }

    mario = marioGetPtr();
    partyId = *(u8*)((s32)mario + 0x246);
    if ((s8)partyId >= 0) {
        partyKill((s8)partyId);
        *(s8*)((s32)mario + 0x246) = -1;
    }
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
int marioGetParty(void) {
    extern void* marioGetPtr(void);
    extern void* partyGetPtr(s32 partyId);
    void* mario = marioGetPtr();
    s32 partyId = *(s8*)((s32)mario + 0x245);
    void* party;

    if (partyId < 0) {
        goto second;
    }
    party = partyGetPtr(partyId);
    if (party == 0) {
        goto second;
    }
    if ((*(u32*)party & 8) != 0) {
    } else {
        goto done;
    }
second:
    partyId = *(s8*)((s32)mario + 0x246);
    if (partyId < 0) {
        goto fail;
    }
    party = partyGetPtr(partyId);
    if (party == 0) {
        goto fail;
    }
    if ((*(u32*)party & 8) != 0) {
fail:
        partyId = -1;
    } else {
        goto done;
    }

done:
    if (partyId >= 0) {
        return *(s8*)((s32)partyGetPtr(partyId) + 0x31);
    }
    return 0;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 marioGetPartyId(void) {
    extern void* marioGetPtr(void);
    extern void* partyGetPtr(s32 partyId);
    void* mario = marioGetPtr();
    s32 partyId = *(s8*)((s32)mario + 0x245);
    void* party;

    if (partyId >= 0) {
        party = partyGetPtr(partyId);
        if (party != 0 && (*(u32*)party & 8) == 0) {
            return partyId;
        }
    }

    partyId = *(s8*)((s32)mario + 0x246);
    if (partyId >= 0) {
        party = partyGetPtr(partyId);
        if (party != 0 && (*(u32*)party & 8) == 0) {
            return partyId;
        }
    }
    return -1;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 marioGetExtraPartyId(void) {
    extern void* marioGetPtr(void);
    extern void* partyGetPtr(s32 partyId);
    void* mario = marioGetPtr();
    s32 partyId = *(s8*)((s32)mario + 0x245);
    void* party;

    if (partyId >= 0) {
        party = partyGetPtr(partyId);
        if (party != 0 && (*(u32*)party & 8) != 0) {
            return partyId;
        }
    }

    partyId = *(s8*)((s32)mario + 0x246);
    if (partyId >= 0) {
        party = partyGetPtr(partyId);
        if (party != 0 && (*(u32*)party & 8) != 0) {
            return partyId;
        }
    }
    return -1;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


void partyJoin(s32 partyId) {
    extern void* pouchGetPtr(void);

    if (partyId < 8) {
        ((u16*)pouchGetPtr())[partyId * 7] |= 1;
    }
}

void partyLeft(s32 partyId) {
    extern void* pouchGetPtr(void);

    if (partyId < 8) {
        ((u16*)pouchGetPtr())[partyId * 7] &= ~1;
    }
}

u32 partyChkJoin(s32 partyId) {
    extern void* pouchGetPtr(void);

    u32 joined = 0;
    if (partyId < 8) {
        joined = ((u16*)pouchGetPtr())[partyId * 7] & 1;
    }
    return joined;
}

s32 partyGetHp(s32 partyId) {
    extern void* pouchGetPtr(void);

    s32 value = 0;
    if (partyId < 8) {
        value = ((s16*)pouchGetPtr())[partyId * 7 + 3];
    }
    return value;
}

s32 partyGetTechLv(s32 partyId) {
    extern void* pouchGetPtr(void);

    s32 value = 0;
    if (partyId < 8) {
        value = ((s16*)pouchGetPtr())[partyId * 7 + 6];
    }
    return value;
}

