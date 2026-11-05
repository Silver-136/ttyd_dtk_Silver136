// cam_road.c: the camera road system (CamRoadCurve records and the road camera entry points that move the field camera).

#pragma once

#include "dolphin/types.h"
#include <dolphin/mtx.h>
#include <dolphin/os.h>

/* camera road curve record (0xB8): the road cameras, collision curves and the curve table all use it */
typedef struct CamRoadCurve {
    char name[0x20];       // 0x0
    u32 fixYOn;              // 0x20
    f32 f_24;              // 0x24
    u32 off;               // 0x28
    u8 pad_2c[0x40 - 0x2c];
    s32 idxA;              // 0x40
    s32 idxB;              // 0x44
    f32 limA;              // 0x48
    f32 limB;              // 0x4c
    u8 pad_50[0x58 - 0x50];
    f32 f_58;              // 0x58
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
    s32 on;                // 0x00
    s32 f_04;              // 0x04
    s32 hitFlags;              // 0x08
    f32 in[6];             // 0x0c
    CamRoadCurve* cam;       // 0x24
    Vec pos;               // 0x28
    Vec dir;               // 0x34
    f32 dist;              // 0x40
    f32 angle;             // 0x44
    f32 rot;               // 0x48
    f32 segment;              // 0x4c
    OSTime time;           // 0x50
    f32 f_58;              // 0x58
    f32 shiftXRate;        // 0x5c
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
    CamRoadMap* map;       // 0x0
    u8 pad0[4];
    CamRoadState cur;      // 0x8
    CamRoadState next;     // 0x68
    s32 hokanOn;              // 0xc8
    s32 hokanType;         // 0xcc
    f32 hokanT;            // 0xd0
    f32 hokanSpeed;        // 0xd4
    CamRoadState prev;     // 0xd8
    CamRoadState start;    // 0x138
    u32 cmpFlags;          // 0x198
    f32 tuning[11];         // 0x19c
    OSTime time;           // 0x1c8
    u32 state;             // 0x1d0
    f32 out[6];            // 0x1d4
    f32 in[6];             // 0x1ec
    s32 keptIdx;
    s32 unusedFlag;
    u8 pad6[0x210 - 0x20c];
} CamRoadWork;

CamRoadWork* camRoadGetWorkPtr(void);
void camRoadMain(CameraParams* out, f32 xA, f32 yA, f32 zA, f32 xB, f32 yB, f32 zB, f32 shiftWeight);

f32 camRoadGetCurrentShiftXRate(void);
void camRoadReset(void);

s32 camRoadSetup(const char* name);
void camRoadCurveOff(const char* name);
void camRoadCurveOn(const char* name);
