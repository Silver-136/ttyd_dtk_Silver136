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
USER_FUNC(evt_snd_bgm_freq);
USER_FUNC(evt_snd_bgmon_f_d);
USER_FUNC(evt_snd_env_lpf);
USER_FUNC(evt_snd_envoff_f);
USER_FUNC(evt_snd_envon_f);
USER_FUNC(evt_snd_sfx_all_stop);
USER_FUNC(evt_snd_sfx_dist);
USER_FUNC(evt_snd_sfxon_);
USER_FUNC(evt_snd_sfxon_3d_ex);
USER_FUNC(evt_snd_sfxon__);
