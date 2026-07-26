#pragma once

#include <dolphin/types.h>

void marioBaloonInit(void);
void marioBalloonOn(s32 type);
void marioBalloonOff(void);
u8 marioBalloonMain(void);
s32 chkBalloon(void* hitObj);
