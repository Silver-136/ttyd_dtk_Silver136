// evt_seq.c: script commands that change or wait on the game sequence (seqdrv).

#pragma once

#include "driver/seqdrv.h"
#include "evt/evt_cmd.h"
#include <dolphin/types.h>

USER_FUNC(evt_seq_set_seq);
USER_FUNC(evt_seq_wait);
