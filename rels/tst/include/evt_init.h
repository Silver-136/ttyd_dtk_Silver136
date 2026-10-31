// Debug draw call stubs for the tst REL's tessellation test, linked to _unresolved (no target in the retail DOL).
#pragma once

#include "dolphin/types.h"
#include <dolphin/gx.h>
#include "glu/glu.h"

// The tessellation test's debug draw calls have no target in the retail DOL, so the REL links each of them to
// _unresolved (tst.c).
void _unresolved(void);
#define dbgDrawReset ((void (*)(s32))_unresolved)
#define dbgDrawPrintf ((void (*)(s32, s32, GXColor*, f32, const char*, ...))_unresolved)
#define dbgDrawCursor ((void (*)(s32, s32, GXColor, f32))_unresolved)
