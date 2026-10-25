#pragma once

#include "driver/seqdrv.h"
#include "dolphin/gx/GXStruct.h"

//for seqdrv
void seq_mapChangeInit(SequenceWork* work);
void seq_mapChangeMain(SequenceWork* work);
void seq_mapChangeExit(SequenceWork* work);

extern GXColor mapchange_zero_color;
void _relUnLoad(void);
void _unload(const char* prevMap, const char* nextMap, const char* bero);
