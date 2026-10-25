#pragma once

#include "dolphin/types.h"

f32 marioGetJumpHeight(void);
extern f32 marioJumpSwData[];
void marioPreJump(void);
void N_mot_jumpNpc_post(void);
void mot_fall(void);
void mot_fall_post(void);
void mot_jump(void);
void mot_jumpNpc(void);
void mot_jumpSmall(void);
void mot_jumpStand(void);
void mot_jumpSw(void);
void mot_land(void);
void mot_upstairs(void);
