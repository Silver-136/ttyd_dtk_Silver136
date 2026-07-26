#pragma once

#include "event/evt_cmd.h"

#include <dolphin/mtx.h>


typedef struct BattleWorkCamera {
    s32 flags;              //0x0
    s32 counter;            //0x4, counter when firstAttack is 0
    s32 priorityLimit;      //0x8
    s32 mode;               //0xC
    u8 unk10[0x24 - 0x10];  //0x10
    f32 zoom;               //0x24
    s16 moveSpeedLevel;     //0x28
    s16 zoomSpeedLevel;     //0x2A
    u8 unk2C[0x5C - 0x2C];  //0x2C
    Vec offset;             //0x5C
    Vec moveSpeed;          //0x68
    u8 unk74[0x104 - 0x74]; //0x74
} BattleWorkCamera;
STATIC_ASSERT_SIZEOF(BattleWorkCamera, 0x104);

void battleCameraInit(void);
void battleCameraMain(void);
f32 battleCameraGetPosMoveSpeed(void);
void btl_camera_set_mode(s32 priority, s32 mode);
void btl_camera_set_posoffset(s32 priority, f32 x, f32 y, f32 z);
void btl_camera_set_moveSpeedLv(s32 priority, s32 level);
void btl_camera_set_moveto(s32 priority, f32 x, f32 y, f32 z, f32 tx, f32 ty, f32 tz, s32 frames, s32 type);
void btl_camera_shake_h(s32 priority, f32 a, f32 b, s32 frames, s32 type);

USER_FUNC(evt_btl_camera_set_prilimit);
