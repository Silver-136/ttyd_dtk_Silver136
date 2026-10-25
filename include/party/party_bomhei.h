#pragma once

#include "party.h"
#include "dolphin/mtx.h"

void bomhei_bye(PartyEntry* party);
void bomhei_exit(PartyEntry* party);
void bomhei_use(PartyEntry* party);
void bomhei_use_cancel();
void bomhei_use_post(PartyEntry* party);
void partyGetReturnPos(Vec* out);
void bomhei_init_800BFBB0(PartyEntry* party);
void bomhei_use_cancel2(void);
s32 mario_bomhei_keychk(void);
