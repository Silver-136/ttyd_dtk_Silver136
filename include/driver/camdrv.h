// camdrv.c: the camera driver (CameraEntry, CameraId): the fixed camera set, projection and view matrices.

#pragma once

#include <dolphin/gx.h>
#include <dolphin/mtx.h>

// CameraEntry.mode: which camera logic cam3dMain runs (camLoadRoad picks FIELD or NO_ROAD).
enum {
    CAM_MODE_OFF = 0,
    CAM_MODE_FIELD = 1,
    CAM_MODE_BATTLE = 2,
    CAM_MODE_NO_ROAD = 3,  // field camera with no road loaded: nothing runs
};

// Fully shown letterbox bars, in pixels (camera letterBoxTop / letterBoxBottom).
#define LETTER_BOX_TOP_HEIGHT 49
#define LETTER_BOX_BOTTOM_HEIGHT 44

// CameraEntry.stateA: the timed move of the field camera (camMain).
typedef enum CameraMoveState {
    CAM_MOVE_NONE = 0,
    CAM_MOVE_BACK = 1,       // progressA runs 1 -> 0: evt_cam3d_evt_off
    CAM_MOVE_TO_UNUSED = 2,  // never set; handled like CAM_MOVE_TO_STORED for progress, like NONE/BACK for the target
    CAM_MOVE_TO_STORED = 3,  // progressA runs 0 -> 1 towards storedPos/storedAt
} CameraMoveState;

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
    u16 mode;                    // 0x2, CAM_MODE_*: which branch of cam3dMain runs (camSetMode, camLoadRoad)
    u16 stateA;                  // 0x4, CameraMoveState: the timed move to storedPos/storedAt
    f32 progressA;               // 0x8, 0..1 progress of the stateA move, from startTimeA and durationA
    Vec cameraPos;               // 0xC, eye; with cameraAt and cameraUp camMain builds view from it
    Vec cameraAt;                // 0x18, look-at target
    Vec cameraUp;                // 0x24
    f32 near;                    // 0x30, near clip plane of the perspective projection (camEvalNearFar may set it)
    f32 far;                     // 0x34, far clip plane
    f32 fovY;                    // 0x38, vertical field of view in degrees (MTXPerspective)
    f32 aspect;                  // 0x3C
    Vec storedPos;               // 0x40, eye at the end of the stateA move (evt_cam3d_evt_* set it)
    Vec storedAt;                // 0x4C, target at the end of the stateA move
    Vec presetPos;               // 0x58, eye when the stateA move started (copied from cameraPos)
    Vec presetAt;                // 0x64, target when the stateA move started
    s64 startTimeA;              // 0x70, gp->renderTime when the stateA move started
    s64 durationA;               // 0x78, length of the stateA move in ticks (OSMillisecondsToTicks)
    u8 curveA;                   // 0x80, intplGetValue curve of the stateA move
    u16 stateB;                  // 0x82, move to anchor: 0 none, 1 back (evt_cam3d_evt_xyz_off), 3 to anchor (set_xyz)
    f32 progressB;               // 0x84, 0..1 progress of the stateB move, from startTimeB and durationB
    Vec blendPos;                // 0x88, road input eye: marioPos (or anchorPrev) blended to anchor by progressB
    Vec marioPos;                // 0x94, followed point: x/z = Mario's camTarget, y eased towards it (cam3dMain)
    Vec blendAt;                 // 0xA0, road input target: marioAt blended towards anchor by progressB
    Vec marioAt;                 // 0xAC, Mario's camCollision point (plus 1 in y), set each frame by cam3dMain
    Vec anchor;                  // 0xB8, fixed point the stateB move blends towards (evt_cam3d_evt_set_xyz)
    Vec anchorPrev;              // 0xC4, blendPos when the stateB move started
    s64 startTimeB;              // 0xD0, gp->renderTime when the stateB move started
    s64 durationB;               // 0xD8, length of the stateB move in ticks
    u8 curveB;                   // 0xE0, intplGetValue curve of the stateB move
    f32 dollyPercent;            // 0xE4, percent of the eye-to-target distance the road eye moves in
    f32 dollyStart;              // 0xE8, dolly percent of the Mario-following end of the stateB move (cam3dMain sets 0)
    f32 dollyEnd;                // 0xEC, dolly percent at anchor (evt_cam3d_evt_set_xyz)
    f32 dollyStartAlt;           // 0xF0, dollyPercent when the stateB move started
    u16 scissor[4];              // 0xF4, left, top, width, height for GXSetScissor (camDraw)
    f32 viewport[6];             // 0xFC, left, top, width, height, nearZ, farZ for GXSetViewport (camDraw)
    f32 viewYaw;                 // 0x114, degrees, angleABf from cameraPos to cameraAt in the XZ plane
    f32 viewDistance;            // 0x118, distance from cameraPos to cameraAt
    Mtx view;                    // 0x11C, view matrix: camMain builds it from the look-at, rollAngle and viewOffset
    f32 rollAngle;               // 0x14C, degrees around the view z axis (camMain)
    Vec viewOffset;              // 0x150, translation applied after the look-at and roll (camMain)
    Mtx44 projection;            // 0x15C
    GXProjectionType projType;   // 0x19C, GX_PERSPECTIVE or GX_ORTHOGRAPHIC (camSetTypePersp/Ortho)
    Mtx44 projAdjust;            // 0x1A0, concatenated into projection once per CAMERA_FLAG_PROJ_ADJUST (evt_cam shake)
    s16 letterBoxAlpha;          // 0x1E0, alpha of the letterbox bars (2D camera; camLetterBox)
    s16 letterBoxTop;            // 0x1E2, height in pixels of the top bar, up to LETTER_BOX_TOP_HEIGHT
    s16 letterBoxBottom;         // 0x1E4, height in pixels of the bottom bar, up to LETTER_BOX_BOTTOM_HEIGHT
    CameraId letterBoxCamId;     // 0x1E8, camera the letterbox bars are queued on (dispEntry)
    CameraCallback callback;     // 0x1EC, update callback camMain runs each frame (NULL: none)
    Vec reserved_1F0;            // 0x1F0, camEntryPersp sets (0, 8, 400), never read
    u32 pad_1FC;                 // 0x1FC, never accessed
    Vec reserved_200;            // 0x200, camEntryPersp zeroes, never read
    Vec reserved_20C;            // 0x20C, camEntryPersp zeroes, never read
    u16 reserved_218;            // 0x218, camEntryPersp zeroes, never read
    u16 reserved_21A;            // 0x21A, camEntryPersp zeroes, never read
    u16 reserved_21C;            // 0x21C, camEntryPersp zeroes, never read
    u16 reserved_21E;            // 0x21E, camEntryPersp zeroes, never read
    u32 pad_220;                 // 0x220, never accessed
    Vec reserved_224;            // 0x224, camEntryPersp zeroes, never read
    Vec reserved_230;            // 0x230, camEntryPersp sets (10, 10, 10), never read
    Vec reserved_23C;            // 0x23C, camEntryPersp zeroes, never read
    Vec reserved_248;            // 0x248, camEntryPersp zeroes, never read
    char name[0xC];              // 0x254, camEntry name ("3d", "2d", "fade", ...)
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
void camSetAt(CameraId cameraId, f32 x, f32 y, f32 z);
void camSetPos(CameraId cameraId, f32 x, f32 y, f32 z);
void camSetUp(CameraId cameraId, f32 x, f32 y, f32 z);
void camSetTypePersp(CameraId cameraId);
void camSetTypeOrtho(CameraId cameraId);

void getScreenPoint(Vec* model, Vec* screen);
