#pragma once

#include "dolphin/types.h"
#include "evt/evt_word.h"

#ifndef VERSION_G8ME01
extern EvtWord dig_00_init_evt[];
#endif
void dig_thpInit(void);
void dig_thpStop(void);
