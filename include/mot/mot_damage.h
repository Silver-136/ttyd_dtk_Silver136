#pragma once

#include "dolphin/types.h"
#include "dolphin/mtx.h"

void marioSetSplash(s32 kind, Vec* pos);
void set_damage_root_ypos(f32 y);
void marioDmgRetQuakeOn(void);
BOOL marioWaitDamageReturnFall(void);
BOOL marioWaitDamageTogeReturnFall(void);
void mot_damage(void);
void mot_damageToge(void);
