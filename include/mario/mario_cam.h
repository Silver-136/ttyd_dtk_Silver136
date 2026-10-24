#pragma once

#include <dolphin/types.h>
#include "driver/camdrv.h"

s32 marioGetCamId(void);
void marioCamZoomOffReq2(s32);
f32 marioGetCamFollowRate(void);
void marioResetCamFollowRate(void);
void marioResetCamShift(void);
void marioResetCamShiftRate(void);
void marioInitCamId(void);
void marioSetCamFollowRate(f32 followRate);
void marioPreCamera(void);
void marioCamMain(void);
void marioUpdateCam(void);
void marioUpdateCamPos(void);
void marioNoUpdateCamPos(void);
void camFollowYOn(void);
void camFollowYOff(void);
void marioCamZoomOffReq(void);
BOOL marioCamZoomOff(void);
BOOL marioCamZoomUp(void);
BOOL marioCamZoomUpLevelMain(void);

void marioSetCamId(CameraId camId);
