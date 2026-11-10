#pragma once

#include "version.h"
#include "driver/seqdrv.h"
#include "evt/evt_cmd.h"
#include "evt/evt_word.h"

void seq_gameOverInit(SequenceWork* work);
void seq_gameOverExit(SequenceWork* work);
void seq_gameOverMain(SequenceWork* work);
#if VERSION >= VERSION_JP
USER_FUNC(evt_snd_bgm_unpause);
#endif

extern EvtWord fall_gameover_es[];
