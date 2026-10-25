#pragma once

#include "dolphin/types.h"
#include "party.h"

s32 nokonokoGetStatus(PartyEntry* party);
void nokonoko_bye(PartyEntry* party);
void nokonoko_finish(PartyEntry* party);
void nokonoko_init(PartyEntry* party);
void nokonoko_move(PartyEntry* party);
void nokonoko_use(PartyEntry* party);
void nokonoko_use_post(PartyEntry* party);
void nokotaro_hold_cancel(PartyEntry* party);
BOOL nokonoko_holdItem(void);
void nokonoko_use_cancel(void);
