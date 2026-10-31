// party_chuchu.c: Ms. Mowz's (chuchu) field partner functions (init, move, use, exit) and the treasure search.

#pragma once

#include "party.h"
#include "dolphin/types.h"

void chuchu_exit(PartyEntry* party);
void chuchu_init(PartyEntry* party);
void chuchu_move(PartyEntry* party);
BOOL chuchu_searchObject(void);
void chuchu_use(PartyEntry* party);
void chuchu_reinit(PartyEntry* party);
