#pragma once

#include <dolphin/types.h>
#include "driver/effdrv.h"
#include "driver/camdrv.h"

void effKemuTestDrawCam(EffectEntry* effect, CameraId camera);
void effKemuTestSetRxRz(EffectEntry* effect, f32 rx, f32 rz);
