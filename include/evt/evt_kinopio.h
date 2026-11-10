// evt_kinopio.c: inn scripts and helpers (evt_kinopio_setup, bed and payment commands); kinopio is Toad.

#pragma once

#include "version.h"
#include "evt/evt_cmd.h"
#include "dolphin/types.h"

USER_FUNC(evt_kinopio_setup);
extern EvtWord kinopio_house_talk_evt[];

extern EvtWord evt_50[];

#if VERSION >= VERSION_JP
USER_FUNC(cameraman_on);
#endif

USER_FUNC(offset);
