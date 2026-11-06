// mario_party.c: Mario's side of the partners (partyJoin/partyLeft, marioPartyEntry/Hello/Goodbye, marioUseParty).

#pragma once

#include <dolphin/types.h>
#include "mario/mario_pouch.h"

void partyJoin(s32 partnerId);


s32 marioGetPartyId(void);
s32 marioGetExtraPartyId(void);
s32 partyGetTechLv(s32 partnerId);
s32 partyGetHp(s32 partnerId);
BOOL partyChkJoin(s32 partnerId);
void partyLeft(s32 partnerId);
s32 marioGetParty(void);
void marioPartyKill(void);
BOOL marioPartyGoodbye(void);
s32 marioPartyHello(s32 id);
s32 marioPartyEntry(s32 id);
s32 marioUsePartyMain(void);
s32 marioUseParty(void);
