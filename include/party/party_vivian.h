// party_vivian.c: Vivian's field partner functions (init, move, shadow, unhold) and her Mario motion mot_vivian.

#pragma once

#include <dolphin/types.h>

struct PartyEntry;

s32 vivianGetStatus(void);
void marioForceVivianAnime(void);
void mot_vivian(void);
void mot_vivian_post(void);
void vivianShadow(struct PartyEntry* party);
void vivianUnhold(struct PartyEntry* party);
void vivian_move(struct PartyEntry* party);
void vivian_use(struct PartyEntry* party);

void partyVivianTailStart(struct PartyEntry* party);
void partyVivianTailStop(struct PartyEntry* party);
