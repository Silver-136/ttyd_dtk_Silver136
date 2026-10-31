// mot_jabara.c: Mario's Spring Jump (jabara, "bellows") motion, mot_grasp, and the jabara state and overhead checks.

#pragma once

#include "dolphin/types.h"

BOOL jabaraNoHitChk(void);

s32 marioGetJabaraState(void);
BOOL marioChkJabara(void);
BOOL marioPriCheckJabara(void);
void mot_grasp(void);
void mot_jabara(void);
void mot_jabara_post(void);
