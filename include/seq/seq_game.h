#pragma once

#include <dolphin/os.h>
#include "driver/seqdrv.h"

void seq_gameInit(SequenceWork* work);
void seq_gameMain(SequenceWork* work);

void seq_gameExit(SequenceWork* wp);
OSTime e3GetNoneKeyTime(void);
