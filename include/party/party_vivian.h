#pragma once

#include <dolphin/types.h>

u8 N_marioForceVivianAnime(void);
u8 mot_vivian(void);
u8 mot_vivian_post(void);
void vivian_init(void* party);
s32 vivianGetStatus(void);
void vivianUnhold(void* party);
void L_partyVivianTailStop(void* party);
void L_partyVivianTailStart(void* party);
s32 N_vivianDisplayTailCheck(void* pParty);
u8 vivianShadow(void* pParty);
u8 vivian_move(void* pParty);
void vivian_use(void* pParty);
