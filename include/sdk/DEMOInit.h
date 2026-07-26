#pragma once

#include <dolphin/types.h>

void DEMOBeforeRender(void);
void DEMODoneRender(void);
void DEMOInit(void* param_1);
void* DEMOGetRenderModeObj(void);
void DEMOEnableGPHangWorkaround(u32 threshold);
