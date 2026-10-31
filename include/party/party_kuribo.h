// party_kuribo.c: Goombella's (kuribo) field partner functions (init, use, bye) and christineGetStatus.

#pragma once

#include <dolphin/types.h>
#include "party.h"

void kuribo_bye(PartyEntry* party);
void kuribo_use(PartyEntry* party);
void kuribo_use_post(PartyEntry* party);
void kuribo_init_800C23B4(PartyEntry* party);
BOOL christineGetStatus(void);
