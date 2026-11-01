#pragma once

#include "version.h"
#include <dolphin/types.h>

#if VERSION <= VERSION_DEMO_US
extern f32 queen_uv[];
#include <dolphin/gx.h>
BOOL gxsubMiscSetGX(s32 mode);
void gxsubFillGX(s32 x, s32 y, s32 w, s32 h, GXColor* color);
void gxsubFillSetTexObj(GXTexObj* obj);
#else
extern f32 queen_uv[];
#endif
