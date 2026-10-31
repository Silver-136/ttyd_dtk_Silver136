#pragma once

#include "version.h"
#include "dolphin/types.h"
#include "evt/evt_word.h"

#if VERSION < VERSION_US
extern EvtWord dig_00_init_evt[];
#endif
void dig_thpInit(void);
void dig_thpStop(void);
