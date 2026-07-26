#pragma once

#include <dolphin/types.h>

s32 seqCheckSeq(void);
void seqSetSeq(s32 seq, s32 p0, s32 p1);
s32 seqGetNextSeq(void);
s32 seqGetPrevSeq(void);
s32 seqGetSeq(void);
void seqInit_MARIOSTORY(void);
void seqMain(void);
