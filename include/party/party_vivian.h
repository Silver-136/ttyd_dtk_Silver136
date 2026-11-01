// party_vivian.c: Vivian's field partner functions (init, move, shadow, unhold) and her Mario motion mot_vivian.

#pragma once

#include <dolphin/types.h>

struct PartyEntry;

// vivianGetStatus: where Vivian is in her Veil.
enum {
    VIVIAN_STATUS_INACTIVE = 0,  // not in use, or not Vivian
    VIVIAN_STATUS_SINKING = 1,   // sinking into the floor with Mario
    VIVIAN_STATUS_HIDDEN = 2,
    VIVIAN_STATUS_RISING = 3,
    VIVIAN_STATUS_OTHER = 4,
};

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
