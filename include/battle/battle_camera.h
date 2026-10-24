#pragma once

#include <dolphin/mtx.h>
#include "evt/evt_cmd.h"

typedef struct {
    s32 flags; //0x0
    s32 modeFrame; //0x4, frames in the current camera mode (1 = first frame)
    s32 priLimit; //0x8
    s32 mode; //0xC
    s32 unitId; //0x10
    s32 unitId2; //0x14
    s32 partsId; //0x18
    s32 partsId2; //0x1C
    s32 audienceId; //0x20
    f32 zoom; //0x24
    s16 moveSpeedLv; //0x28
    s16 zoomSpeedLv; //0x2A
    Vec pos; //0x2C
    Vec prevPos; //0x38
    Vec at; //0x44
    Vec prevAt; //0x50
    Vec position; //0x5C
    Vec moveSpeed; //0x68
    Vec moveSpeedAt; //0x74
    s32 moveToType; //0x80
    s32 moveToTimer; //0x84
    s32 moveToFrames; //0x88
    Vec moveToStartPos; //0x8C
    Vec moveToEndPos; //0x98
    Vec moveToStartAt; //0xA4
    Vec moveToEndAt; //0xB0
    Vec homingOffset; //0xBC
    Vec homingOffset2; //0xC8
    s32 shakeWType; //0xD4
    s32 shakeHType; //0xD8
    s32 shakeWFrames; //0xDC
    s32 shakeHFrames; //0xE0
    s32 shakeWTimer; //0xE4
    s32 shakeHTimer; //0xE8
    f32 shakeWAmp[2]; //0xEC
    f32 shakeHAmp[2]; //0xF4
    f32 shakeWOfs; //0xFC
    f32 shakeHOfs; //0x100
} BattleCameraWork;

void battleCameraInit(void);

USER_FUNC(evt_btl_camera_add_zoom);
USER_FUNC(evt_btl_camera_nomove_x_onoff);
USER_FUNC(evt_btl_camera_nomove_y_onoff);
USER_FUNC(evt_btl_camera_nomove_z_onoff);
USER_FUNC(evt_btl_camera_noshake);
USER_FUNC(evt_btl_camera_off_posoffset_manual);
USER_FUNC(evt_btl_camera_set_homing_unit);
USER_FUNC(evt_btl_camera_set_homing_unit_audience);
USER_FUNC(evt_btl_camera_set_homing_unitparts);
USER_FUNC(evt_btl_camera_set_mode);
USER_FUNC(evt_btl_camera_set_moveSpeedLv);
USER_FUNC(evt_btl_camera_set_moveto);
USER_FUNC(evt_btl_camera_set_posoffset);
USER_FUNC(evt_btl_camera_set_prilimit);
USER_FUNC(evt_btl_camera_set_zoom);
USER_FUNC(evt_btl_camera_set_zoomSpeedLv);
USER_FUNC(evt_btl_camera_shake_h);
USER_FUNC(evt_btl_camera_shake_w);
USER_FUNC(evt_btl_camera_wait_move_end);

void battleCameraMain(void);
void btl_camera_set_mode(s32 pri, s32 mode);
void btl_camera_set_moveSpeedLv(s32 pri, s32 lv);
