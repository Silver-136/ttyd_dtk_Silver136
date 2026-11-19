// cam_road.c: the camera road system (CamRoadCurve records and the road camera entry points that move the field camera).

#pragma once

#include "dolphin/types.h"
#include <dolphin/mtx.h>
#include <dolphin/os.h>

/* camera road curve record (0xB8): the road cameras, collision curves and the curve table all use it */
typedef struct CamRoadCurve {
    char name[0x20];       // 0x0
    u32 fixYOn;              // 0x20
    f32 fixY;              // 0x24, camera y used instead of the target's when fixYOn is set (format 101)
    u32 off;               // 0x28
    u8 pad_2c[0x40 - 0x2c];
    s32 idxA;              // 0x40
    s32 idxB;              // 0x44
    f32 limA;              // 0x48
    f32 limB;              // 0x4c
    u8 pad_50[0x58 - 0x50];
    f32 dist;              // 0x58, camera distance on this curve (copied into CamRoadState.dist)
    u8 pad_5c[0x64 - 0x5c];
    f32 angle;              // 0x64
    u8 pad_68[0x70 - 0x68];
    f32 rot;              // 0x70
    u8 pad_74[0x7c - 0x74];
    f32 shiftXRate;              // 0x7c
    u8 pad_80[0x84 - 0x80];
    s32 flag;              // 0x84
    u8 pad_88[0x8c - 0x88];
    f32 height;            // 0x8c
    u8 pad_90[0xa0 - 0x90];
    s32 vtxOfs;            // 0xa0, into CamRoadMap.curveVtx
    s32 vtxNum;            // 0xa4
    s32 grpOfs;            // 0xa8, into CamRoadMap.groups
    s32 grpNum;            // 0xac
    s32 collGrpOfs;              // 0xb0, into CamRoadMap.collGroups
    s32 collGrpNum;              // 0xb4
} CamRoadCurve;
typedef struct CamRoadGroup {
    u8 pad0[0x58];
    s32 vtxOfs;            // 0x58, into CamRoadMap.groupVtx
    u8 pad1[4];
    s32 stripOfs;          // 0x60, into CamRoadMap.strips (or curveVtx for collision)
    s32 stripNum;          // 0x64
} CamRoadGroup;
typedef struct CamRoadStrip {
    s32 idxOfs;            // 0x0, into CamRoadMap.stripIdx
    s32 num;               // 0x4
} CamRoadStrip;
typedef struct CamRoadMap {
    u8 pad0[0x44];
    char version[0x84];    // 0x44
    s32 curveNum;          // 0xc8
    u8 pad1[0xe8 - 0xcc];
    Vec* projection;             // 0xe8
    CamRoadCurve* curves;  // 0xec
    CamRoadGroup* groups;    // 0xf0
    CamRoadStrip* strips;    // 0xf4
    Vec* curveVtx;             // 0xf8
    Vec* groupVtx;             // 0xfc
    s32* stripIdx;            // 0x100
    CamRoadGroup* collGroups;   // 0x104
    void* extra;           // 0x108
} CamRoadMap;
typedef struct CamRoadState {
    s32 on;                // 0x00, set by calcCamRoad; while clear the eye is at (0, 100, 100) facing the origin
    s32 onCollCurve;       // 0x04, a collision curve was hit (calcCamRoad)
    s32 hitFlags;          // 0x08, CamRoadHitFlags (cam_road.c)
    f32 in[6];             // 0x0c, calcCamRoad's target and probe arguments (world units / 10); never read
    CamRoadCurve* cam;     // 0x24, curve the state was computed on (the hit curve, else the previous state's)
    Vec pos;               // 0x28, the eye sits dist away from it; the look-at is pos turned about the eye by rot
    Vec dir;               // 0x34, XZ direction from pos to the eye (the curve normal, negated)
    f32 dist;              // 0x40, eye distance from pos
    f32 angle;             // 0x44, eye elevation in degrees
    f32 rot;               // 0x48, degrees the look-at is turned about the side axis
    f32 segment;           // 0x4c, curve segment of the hit (else the previous one's); compareCamRoad compares it
    OSTime time;           // 0x50, renderFieldTime when computed
    f32 shiftWeight;       // 0x58, calcCamRoad's shiftWeight argument; the next frame scales its side shift by it
    f32 shiftXRate;        // 0x5c, the curve's shiftXRate (camRoadGetCurrentShiftXRate)
} CamRoadState;

/* camera parameters a camera road produces (camdrv blends them into the live camera) */
typedef struct CameraParams {
    f32 fovY;              // 0x0
    f32 aspect;            // 0x4
    f32 far;               // 0x8
    Vec pos;               // 0xc
    Vec at;                // 0x18
} CameraParams;
typedef struct CamRoadWork {
    CamRoadMap* map;       // 0x0, the loaded camera road file (camRoadSetup); kept by camRoadReset
    CamRoadState cur;      // 0x8, state the camera uses (calcCurrentCamState): the blend output while hokanOn
    CamRoadState next;     // 0x68, state calcCamRoad computed this frame
    s32 hokanOn;           // 0xc8, a blend runs (compareCamRoad found a jump between states)
    s32 hokanType;         // 0xcc, calcHokanCamRoad type: 1 with a hit, a fixYOn (1.01) or a shiftXRate curve
    f32 hokanT;            // 0xd0, blend progress, 0 to 1 (hokanSpeed at 60 fps)
    f32 hokanSpeed;        // 0xd4, blend speed from compareCamRoad
    CamRoadState prev;     // 0xd8, blend target; calcCamRoad's previous state (gor_02 reads its curve name)
    CamRoadState start;    // 0x138, state at the blend start
    u32 cmpFlags;          // 0x198, compareCamRoad flags of the last blend start; never read
    f32 tuning[11];        // 0x19c, calcCamRoad step limits ([0]-[5]),
                           // compareCamRoad blend speed weights ([6]-[10]; dou_05 sets [10])
    OSTime time;           // 0x1c8, renderFieldTime of the last blend step
    u32 state;             // 0x1d0, CAM_ROAD_FORMAT_* of the file; kept by camRoadReset
    f32 out[6];            // 0x1d4, calcCamRoad's arguments when no curve applies; never read
    f32 in[6];             // 0x1ec, camRoadMain's arguments; never read
    s32 keptIdx;           // 0x204, -1 at setup; camRoadReset keeps it; nothing else reads it
    s32 unusedFlag;        // 0x208, cleared at setup and reset; never read
} CamRoadWork;

CamRoadWork* camRoadGetWorkPtr(void);
void camRoadMain(CameraParams* out, f32 xA, f32 yA, f32 zA, f32 xB, f32 yB, f32 zB, f32 shiftWeight);

f32 camRoadGetCurrentShiftXRate(void);
void camRoadReset(void);

s32 camRoadSetup(const char* name);
void camRoadCurveOff(const char* name);
void camRoadCurveOn(const char* name);
