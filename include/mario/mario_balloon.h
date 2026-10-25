#pragma once

#include "dolphin/types.h"
#include "driver/hitdrv.h"

BOOL chkBalloon(HitEntry* hit);
void marioBalloonMain(void);
void marioBalloonOff(void);
void marioBalloonOn(s32 type);
void marioBaloonInit(void);
