#pragma once

#include "version.h"
#include <dolphin/types.h>

extern f32 queen_uv[];
#if VERSION <= VERSION_DEMO_US
#include <dolphin/gx.h>
BOOL gxsubMiscSetGX(s32 mode);
void gxsubFillGX(s32 x, s32 y, s32 w, s32 h, GXColor* color);
void gxsubFillSetTexObj(GXTexObj* obj);
#endif
