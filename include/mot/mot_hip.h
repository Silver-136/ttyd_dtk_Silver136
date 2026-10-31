// mot_hip.c: Mario's spin jump / hip drop motion (mot_hip, mot_hip_post) and its attack and bump checks.

#pragma once

#include <dolphin/types.h>

BOOL marioChkHipAttack(void);
BOOL marioChkHipBump(void);
void mot_hip(void);
void mot_hip_post(void);
