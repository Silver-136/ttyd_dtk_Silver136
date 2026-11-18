#pragma once

#include <dolphin/os.h>
#include "driver/seqdrv.h"

OSTime e3GetNoneKeyTime(void);
void seq_gameInit(SequenceWork* work);
void seq_gameMain(SequenceWork* work);
void seq_gameExit(SequenceWork* wp);
