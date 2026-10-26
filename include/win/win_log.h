#pragma once

#include "dolphin/types.h"

struct MonosiriWork;

struct MonosiriWork* monosiriInit(s32 id, f32 x, f32 y);
void monosiriMain(struct MonosiriWork* m);
void monosiriClose(struct MonosiriWork* m);
BOOL monosiriCloseWait(struct MonosiriWork* m);
void monosiriExit(struct MonosiriWork* m);
