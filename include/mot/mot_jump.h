// mot_jump.c: Mario's jump, fall and landing motions (mot_jump, mot_jumpStand, mot_fall, mot_land, mot_upstairs).

#pragma once

#include "dolphin/types.h"

f32 marioGetJumpHeight(void);
extern f32 marioJumpSwData[];
void marioPreJump(void);
void mot_jumpNpc_post(void);
void mot_fall(void);
void mot_fall_post(void);
void mot_jump(void);
void mot_jumpNpc(void);
void mot_jumpSmall(void);
void mot_jumpStand(void);
void mot_jumpSw(void);
void mot_land(void);
void mot_upstairs(void);
