#pragma once

#include <dolphin/gx.h>
#include <dolphin/mtx.h>

typedef struct CameraEntry CameraEntry;
typedef void (*CameraCallback)(CameraEntry* entry);

typedef enum CameraId {
    CAMERA_OFFSCREEN = 0,
    CAMERA_OFFSCREEN2 = 1,
    CAMERA_SHADOW = 2,
    CAMERA_BACKGROUND = 3,
    CAMERA_3D = 4,
    CAMERA_3D_EFFECTA = 5,
    CAMERA_3D_IMAGE = 6,
    CAMERA_3D_EFFECTB = 7,
    CAMERA_2D = 8,
    CAMERA_FADE = 9,
    CAMERA_FADE2 = 10,
    CAMERA_DEBUG = 11,
    CAMERA_DEBUG_3D = 12,
    CAMERA_MAX,
} CameraId;

// CameraEntry.flags (camdrv, evt_cam)
typedef enum CameraFlags {
    CAMERA_FLAG_DISP_OFF = (1 << 0),          // 0x1, camDispOff: not drawn
    CAMERA_FLAG_CTRL_OFF = (1 << 1),          // 0x2, camCtrlOff: cam3dMain does nothing
    CAMERA_FLAG_ROAD = (1 << 2),              // 0x4, camLoadRoad loaded a camera road
    CAMERA_FLAG_PROJ_ADJUST = (1 << 3),       // 0x8, projAdjust applied to the projection once (shake)
    CAMERA_FLAG_NO_Y_FOLLOW = (1 << 4),       // 0x10, cam3dMain does not follow Mario's height
    CAMERA_FLAG_ROAD_SHIFT_OFF = (1 << 5),    // 0x20, evt_cam3d_road_shift_onoff(0)
    CAMERA_FLAG_AUTO_NEAR_FAR = (1 << 6),     // 0x40, camEvalNearFar before each perspective draw
    CAMERA_FLAG_LETTER_BOX = (1 << 8),        // 0x100, letter box bars shown
    CAMERA_FLAG_LETTER_BOX_OFF = (1 << 9),    // 0x200, evt_cam_letter_box_disable: no automatic bars
    CAMERA_FLAG_LETTER_BOX_INSTANT = (1 << 10), // 0x400, the next bar change is immediate
} CameraFlags;

struct CameraEntry {
    u16 flags;                   // 0x0, CameraFlags
    u16 mode;                    // 0x2
    u16 stateA;                  // 0x4
    u8 align_6[0x8 - 0x6];       // 0x6
    f32 progressA;               // 0x8
    Vec cameraPos;               // 0xC
    Vec cameraAt;                // 0x18
    Vec cameraUp;                // 0x24
    f32 near;                    // 0x30
    f32 far;                     // 0x34
    f32 fovY;                    // 0x38
    f32 aspect;                  // 0x3C
    Vec storedPos;               // 0x40
    Vec storedAt;                // 0x4C
    Vec presetPos;               // 0x58
    Vec presetAt;                // 0x64
    s64 startTimeA;              // 0x70
    s64 durationA;               // 0x78
    u8 curveA;                   // 0x80
    u8 align_81[0x82 - 0x81];    // 0x81
    u16 stateB;                  // 0x82
    f32 progressB;               // 0x84
    Vec blendPos;                // 0x88
    Vec marioPos;                // 0x94
    Vec blendAt;                 // 0xA0
    Vec marioAt;                 // 0xAC
    Vec anchor;                  // 0xB8
    Vec anchorPrev;              // 0xC4
    s64 startTimeB;              // 0xD0
    s64 durationB;               // 0xD8
    u8 curveB;                   // 0xE0
    u8 align_E1[0xE4 - 0xE1];    // 0xE1
    f32 dollyPercent;            // 0xE4
    f32 dollyStart;              // 0xE8
    f32 dollyEnd;                // 0xEC
    f32 dollyStartAlt;           // 0xF0
    u16 scissor[4];              // 0xF4
    f32 viewport[6];             // 0xFC
    f32 viewYaw;                 // 0x114
    f32 viewDistance;            // 0x118
    Mtx view;                    // 0x11C
    f32 rollAngle;               // 0x14C
    Vec viewOffset;              // 0x150
    Mtx44 projection;            // 0x15C
    GXProjectionType projType;   // 0x19C
    Mtx44 projAdjust;            // 0x1A0
    s16 letterBoxAlpha;          // 0x1E0
    s16 letterBoxTop;            // 0x1E2
    s16 letterBoxBottom;         // 0x1E4
    u8 align_1E6[0x1E8 - 0x1E6]; // 0x1E6
    CameraId letterBoxCamId;     // 0x1E8
    CameraCallback callback;     // 0x1EC
    Vec reserved_1F0;               // 0x1F0, camInit sets (0, 8, 400), never read
    u32 pad_1FC;               // 0x1FC, never accessed
    Vec reserved_200;               // 0x200, camInit zeroes, never read
    Vec reserved_20C;               // 0x20C, camInit zeroes, never read
    u16 reserved_218;               // 0x218, camInit zeroes, never read
    u16 reserved_21A;               // 0x21A, camInit zeroes, never read
    u16 reserved_21C;               // 0x21C, camInit zeroes, never read
    u16 reserved_21E;               // 0x21E, camInit zeroes, never read
    u32 pad_220;               // 0x220, never accessed
    Vec reserved_224;               // 0x224, camInit zeroes, never read
    Vec reserved_230;               // 0x230, camInit sets (10, 10, 10), never read
    Vec reserved_23C;               // 0x23C, camInit zeroes, never read
    Vec reserved_248;               // 0x248, camInit zeroes, never read
    char name[0xC];              // 0x254
};

void camInit(void);
void camMain(void);
void camDraw(void);

void camLoadRoad(CameraId cameraId, const char* filename);
void camUnLoadRoad(CameraId cameraId);

CameraEntry* camGetPtr(CameraId cameraId);
CameraEntry* camGetCurPtr(void);
CameraId camGetCurNo(void);
void camSetCurNo(CameraId cameraId);

void camSetMode(CameraId cameraId, u16 mode);
void camDispOn(CameraId cameraId);
void camDispOff(CameraId cameraId);
void camCtrlOn(CameraId cameraId);
void camCtrlOff(CameraId cameraId);
void camSetTypePersp(CameraId cameraId);
void camSetTypeOrtho(CameraId cameraId);

void getScreenPoint(Vec* model, Vec* screen);
void camLetterBox(void);
