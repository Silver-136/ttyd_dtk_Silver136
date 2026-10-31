// evt_snd.c: sound script commands for BGM (evt_snd_bgmon), SFX (evt_snd_sfxon_3d) and environment sounds
// (evt_snd_envon).

#pragma once

#include <dolphin/types.h>
#include "evt/evt_cmd.h"
#include "driver/seqdrv.h"

USER_FUNC(evt_snd_bgmon);



USER_FUNC(evt_snd_sfxon_3d);

void seq_gameOverExit(SequenceWork* work);
void seq_gameOverInit(SequenceWork* work);
USER_FUNC(evt_snd_bgm_unpause);
