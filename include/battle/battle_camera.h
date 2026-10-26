#pragma once

#include <dolphin/mtx.h>
#include "evt/evt_cmd.h"

// BattleCameraWork.flags
typedef enum BattleCameraFlags {
    BTL_CAM_FLAG_SHAKE_W = (1 << 0),          // 0x1, btl_camera_shake_w running
    BTL_CAM_FLAG_SHAKE_H = (1 << 1),          // 0x2, btl_camera_shake_h running
    BTL_CAM_FLAG_NOMOVE_X = (1 << 2),         // 0x4, btl_camera_nomove_x_onoff
    BTL_CAM_FLAG_NOMOVE_Y = (1 << 3),         // 0x8
    BTL_CAM_FLAG_NOMOVE_Z = (1 << 4),         // 0x10
    BTL_CAM_FLAG_POS_OFFSET = (1 << 5),       // 0x20, btl_camera_set_posoffset
    BTL_CAM_FLAG_NO_RUMBLE_UNUSED = (1 << 6), // 0x40, shakes without rumble; nothing sets it
} BattleCameraFlags;

typedef struct {
    s32 flags; //0x0, BattleCameraFlags
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

// BattleWork.cameraWork.mode (btl_camera_set_mode / evt_btl_camera_set_mode); names from the debug name
// battleCameraMain copies for each mode and from what the mode does.
typedef enum BattleCameraMode {
    BTL_CAM_MODE_DEFAULT = 0, // debug name: default
    BTL_CAM_MODE_DEFAULT_ZOOM = 1,
    BTL_CAM_MODE_CURSOR_UP = 2, // no callers; falls through into FREE
    BTL_CAM_MODE_FREE = 3, // position/target from set_moveto
    BTL_CAM_MODE_BACKSTAGE = 4, // behind the stage; no callers
    BTL_CAM_MODE_BACKSTAGE_2 = 5, // no callers
    BTL_CAM_MODE_AUDIENCE = 6,
    BTL_CAM_MODE_UNIT = 7, // follows one unit (homing unit)
    BTL_CAM_MODE_UNIT_TO_UNIT = 8, // between the two homing units
    BTL_CAM_MODE_UNIT_MIDPOINT = 9, // midpoint of the homing units (shares UNIT's debug name)
    BTL_CAM_MODE_PART = 10, // follows one part
    BTL_CAM_MODE_PART_TO_PART = 11, // between two parts
    BTL_CAM_MODE_AUDIENCE_MEMBER = 12, // follows an audience member
    BTL_CAM_MODE_UNIT_TO_AUDIENCE = 13, // between a unit and an audience member
    BTL_CAM_MODE_ENTRY_NORMAL = 14, // battle start, normal (switches to FREE)
    BTL_CAM_MODE_ENTRY_FIRST_STRIKE = 15, // battle start after Mario's first strike
    BTL_CAM_MODE_ENTRY_ENEMY_FIRST_STRIKE = 16, // battle start after the enemy's first strike
    BTL_CAM_MODE_VICTORY = 17,
    BTL_CAM_MODE_VICTORY_AUDIENCE = 18, // shares VICTORY's debug name
    BTL_CAM_MODE_DEFEAT = 19,
    BTL_CAM_MODE_ESCAPE = 20, // running away
} BattleCameraMode;

// btl_camera_set_mode priority: a request applies only at or above the limit set by evt_btl_camera_set_prilimit
// (event and boss scripts raise it to 1 to keep normal attack code from moving the camera).
enum {
    BTL_CAM_PRI_NORMAL = 0,
    BTL_CAM_PRI_EVENT = 1,
};

void battleCameraMain(void);
void btl_camera_set_mode(s32 pri, s32 mode);
void btl_camera_set_moveSpeedLv(s32 pri, s32 lv);
f32 battleCameraGetPosMoveSpeed(void);
void btl_camera_noshake(s32 pri);
void btl_camera_set_posoffset(s32 pri, f32 x, f32 y, f32 z);
void btl_camera_shake_h(s32 pri, f32 a, f32 b, s32 frames, s32 type);
