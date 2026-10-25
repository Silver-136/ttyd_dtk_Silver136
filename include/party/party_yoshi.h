#pragma once

#include "dolphin/types.h"
#include "party.h"

s32 yoshiGetStatus(void);

s32 evt_get_yoshi_frm(void);
void yoshi_bye(PartyEntry* party);
void yoshi_exit(PartyEntry* party);
void yoshi_init(PartyEntry* party);
void yoshi_use(PartyEntry* party);

void evt_set_yoshi_spd(f32 spd);
void marioRideYoshi(void);
void marioYoshiForceCancel(void);
void yoshi_jumpStand(void);
