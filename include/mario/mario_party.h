#pragma once

#include <dolphin/types.h>

s32 marioUseParty(void);
s32 unk_8014140c(void);
int marioPartyEntry(int partyMemberId);
int marioPartyHello(int partyMemberId);
u32 marioPartyGoodbye(void);
void marioPartyKill(void);
int marioGetParty(void);
s32 marioGetPartyId(void);
s32 marioGetExtraPartyId(void);
void partyJoin(s32 partyId);
void partyLeft(s32 partyId);
u32 partyChkJoin(s32 partyId);
s32 partyGetHp(s32 partyId);
s32 partyGetTechLv(s32 partyId);
