// evt_kinopio.c: inn scripts and helpers (evt_kinopio_setup, bed and payment commands); kinopio is Toad.

#pragma once

#include "evt/evt_cmd.h"
#include "dolphin/types.h"

USER_FUNC(evt_kinopio_setup);
extern EvtWord kinopio_house_talk_evt[];

extern EvtWord evt_50[];
#include "version.h"
#if VERSION > VERSION_DEMO_US
USER_FUNC(cameraman_on);
#endif
USER_FUNC(breakfast);
USER_FUNC(coin_chk);
USER_FUNC(get_bed);
USER_FUNC(get_beddeg);
USER_FUNC(get_bedside);
USER_FUNC(get_cam);
USER_FUNC(get_from_bed_evt);
USER_FUNC(get_npcname);
USER_FUNC(get_npctalk);
USER_FUNC(get_ryoukin);
USER_FUNC(get_to_bed_evt);
USER_FUNC(msg_no);
USER_FUNC(offset);
USER_FUNC(power_chk);
USER_FUNC(snd_off);
USER_FUNC(snd_on);
