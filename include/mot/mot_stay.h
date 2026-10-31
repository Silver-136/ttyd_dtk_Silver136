// mot_stay.c: Mario's standing and idle motion (mot_stay, mot_stay_post, marioChgMotAuto), sleep and glare checks.

#pragma once

#include "dolphin/types.h"

BOOL marioChkDeepSleep(void);
void marioChgMotAuto(void);
void marioVoiceGlareOff(void);
void marioVoiceGlareOn(void);
void mot_stay(void);
void mot_stay_post(void);
