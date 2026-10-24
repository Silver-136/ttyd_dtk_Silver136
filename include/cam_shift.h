#pragma once

#include "driver/camdrv.h"
#include "mario/mario.h"
#include "dolphin/types.h"

void camShiftInit(void);
void camShiftMain(CameraEntry* camera, MarioWork* mario, f32* out);
void camShiftReset(void);

void camShiftPostMain(void);
