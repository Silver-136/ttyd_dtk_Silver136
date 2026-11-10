#pragma once

#include "driver/seqdrv.h"
#include "evt/evt_word.h"

void seq_loadExit(SequenceWork* seq);
void seq_loadInit(SequenceWork* seq);
void seq_loadMain(SequenceWork* seq);

#include "evt/evt_cmd.h"

USER_FUNC(evt_seq_load_keep_msg_window);
