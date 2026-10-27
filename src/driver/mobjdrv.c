#include "driver/mobjdrv.h"
#include "driver/mapdrv.h"
#include "driver/npcdrv.h"
#include "mario/mario_party.h"
#include "party.h"
#include "dolphin/mtx.h"
#include "dolphin/types.h"
#include "driver/animdrv.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/hitdrv.h"
#include "driver/offscreendrv.h"
#include "driver/itemdrv.h"
#include "manager/evtmgr.h"
#include "mario/mario.h"
#include "mario/mariost.h"
#include "memory.h"
#include "system.h"
#include <string.h>
#include "koopa.h"
#include "driver/shadowdrv.h"
#include "manager/evtmgr_cmd.h"
#include <dolphin/pad.h>
#include "koopa_hit.h"
#include "mot/mot_hip.h"

/* static map-file joint data for the mobj cube models (same layout as a loaded map file) */
typedef struct CubeDrawMode {
    MapJointDrawMode mode;
    u32 field_0x10;
} CubeDrawMode;
typedef struct CubeMesh {
    u8 reserved_00;       // 0x0
    u8 padding[2];        // 0x1
    u8 isPackedDisplay;   // 0x3
    s32 displayListCount; // 0x4
    u32 elementMask;      // 0x8
    void* vcdTable;       // 0xC, cubeDDT
    u32* lists[15];       // 0x10, display lists in cubeDN
} CubeMesh;
typedef struct CubeJoint {
    MapFileJoint joint;
    MapFileJointPart part;
} CubeJoint;

CubeDrawMode cubeMSJ = {{0, 1, 0, 0, 0, (s32)0x80000000}, 0};

CubeDrawMode front2MSJ = {{0, 1, 0, 0, 0, 0}, 0};

// cube vertex positions (cubeDVP) and normals (cubeDVN), listed in cubeDDT

typedef struct CubeVtxData34 {
    s32 count;
    Vec v[34];
} CubeVtxData34;
typedef struct CubeVtxData18 {
    s32 count;
    Vec v[18];
} CubeVtxData18;
CubeVtxData34 cubeDVP = {
    8,
    {
        {-0.5f, 0.0f, 0.5f},
        {0.5f, 0.0f, 0.5f},
        {-0.5f, 1.0f, 0.5f},
        {0.5f, 1.0f, 0.5f},
        {-0.5f, 1.0f, -0.5f},
        {0.5f, 1.0f, -0.5f},
        {-0.5f, 0.0f, -0.5f},
        {0.5f, 0.0f, -0.5f},
    },
};

CubeVtxData18 cubeDVN = {
    6,
    {
        {0.0f, 0.0f, 1.0f},
        {0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, -1.0f},
        {0.0f, -1.0f, 0.0f},
        {1.0f, 0.0f, 0.0f},
        {-1.0f, 0.0f, 0.0f},
    },
};

void* cubeDDT[26] = {
    &cubeDVP, &cubeDVN, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL
};

/* GX display list data: six unpacked index lists of 205 words (count, then one index record per vertex) */
u32 cubeDN[1230] = {
    0x00000004, 0x00000005, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00060005,
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00020005, 0xFFFFFFFF, 0xFFFFFFFF,
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00040005, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
    0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000004, 0x00070004, 0xFFFFFFFF,
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00010004, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
    0xFFFFFFFF, 0xFFFFFFFF, 0x00050004, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
    0x00030004, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000004, 0x00070003, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
    0xFFFFFFFF, 0x00060003, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00010003,
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000003, 0xFFFFFFFF, 0xFFFFFFFF,
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000004,
    0x00050002, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00040002, 0xFFFFFFFF,
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00070002, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
    0xFFFFFFFF, 0xFFFFFFFF, 0x00060002, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000004, 0x00030001, 0xFFFFFFFF, 0xFFFFFFFF,
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00020001, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
    0xFFFFFFFF, 0x00050001, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00040001,
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000004, 0x00010000, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
    0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00030000, 0xFFFFFFFF,
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00020000, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
    0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000
};

CubeMesh cubeDP = {0, {0}, 0, 6, 0, cubeDDT, {cubeDN, &cubeDN[205], &cubeDN[410], &cubeDN[615], &cubeDN[820], &cubeDN[1025]}};

CubeJoint cubeDJ = {
    {NULL, NULL, NULL, NULL, NULL, NULL, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f},
     {-0.5f, 0.0f, -0.5f}, {0.5f, 1.0f, 0.5f}, {0}, &cubeMSJ.mode, 1},
    {NULL, (MapFileMesh*)&cubeDP},
};

CubeMesh headDP = {0, {0}, 0, 1, 0, cubeDDT, {&cubeDN[410]}};

CubeJoint headDJ = {
    {NULL, NULL, NULL, NULL, NULL, NULL, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f},
     {-0.5f, 0.0f, -0.5f}, {0.5f, 0.0f, 0.5f}, {0}, &cubeMSJ.mode, 1},
    {NULL, (MapFileMesh*)&headDP},
};

CubeMesh frontDP = {0, {0}, 0, 1, 0, cubeDDT, {&cubeDN[1025]}};

CubeJoint frontDJ = {
    {NULL, NULL, NULL, NULL, NULL, NULL, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f},
     {-0.5f, 0.0f, -0.5f}, {0.5f, 1.0f, 0.5f}, {0}, &cubeMSJ.mode, 1},
    {NULL, (MapFileMesh*)&frontDP},
};

CubeMesh front2DP = {0, {0}, 0, 5, 0, cubeDDT, {cubeDN, &cubeDN[205], &cubeDN[410], &cubeDN[615], &cubeDN[820]}};

CubeJoint front2DJ = {
    {NULL, NULL, NULL, NULL, NULL, NULL, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f},
     {-0.5f, 0.0f, -0.5f}, {0.5f, 1.0f, 0.5f}, {0}, &front2MSJ.mode, 1},
    {NULL, (MapFileMesh*)&front2DP},
};

#define MAX_MOBJ_WORK 3

//.sbss
BOOL koopaRunFlag;

//.bss
static MapObjectWork work[MAX_MOBJ_WORK];

static inline MapObjectWork* mobjGetWorkFlag(BOOL inBattle) {
    return inBattle ? &work[1] : &work[0];
}

void mobjKoopaOn(void) {
    if (!koopaRunFlag) {
        MapObjectWork* wp = &work[2];

        koopaRunFlag = TRUE;
        wp->count = 128;
        wp->entries = _mapAlloc(sizeof(MapObjectEntry) * wp->count);
        memset(wp->entries, 0, sizeof(MapObjectEntry) * wp->count);
    }
}

static MapObjectWork* mobjGetWork(void) {
    if (koopaRunFlag) {
        return &work[2];
    } else {
        return mobjGetWorkFlag(gp->inBattle);
    }
}

void mobjDispXLU(CameraId camId, void* param) {
    MapObjectEntry* entry = param;

    if (entry->flags & MOBJ_FLAG_PAPER_TEX) {
        animSetPaperTexMtx(entry->paperTexMtx, NULL, NULL);
        animSetPaperTexObj(&entry->paperTexObj, NULL, NULL, entry->paperTexData, NULL, NULL, 1, 1);
    }
    animPoseDraw(entry->poseId, entry->position.x, entry->position.y, entry->position.z, entry->rotation.y,
                 10.0f * entry->scale.x, 1);
    animPoseDraw(entry->poseId, entry->position.x, entry->position.y, entry->position.z, entry->rotation.y,
                 10.0f * entry->scale.x, 2);
    animPoseDraw(entry->poseId, entry->position.x, entry->position.y, entry->position.z, entry->rotation.y,
                 10.0f * entry->scale.x, 3);
    if (entry->flags & MOBJ_FLAG_PAPER_TEX) {
        animSetPaperTexMtx(NULL, NULL, NULL);
        animSetPaperTexObj(NULL, NULL, NULL, NULL, NULL, NULL, 1, 1);
    }
}

void mobjDisp(CameraId camId, void* param) {
    MapObjectEntry* entry = param;
    DispEntry* disp;

    if (entry->flags & MOBJ_FLAG_PAPER_TEX) {
        animSetPaperTexMtx(entry->paperTexMtx, NULL, NULL);
        animSetPaperTexObj(&entry->paperTexObj, NULL, NULL, entry->paperTexData, NULL, NULL, 1, 1);
    }
    disp = dispGetCurWork();
    switch (disp->renderMode) {
        case 0:
            animPoseDraw(entry->poseId, entry->position.x, entry->position.y, entry->position.z, entry->rotation.y,
                         10.0f * entry->scale.x, 1);
            break;
        case 1:
            animPoseDraw(entry->poseId, entry->position.x, entry->position.y, entry->position.z, entry->rotation.y,
                         10.0f * entry->scale.x, 2);
            break;
        case 2:
        default:
            animPoseDraw(entry->poseId, entry->position.x, entry->position.y, entry->position.z, entry->rotation.y,
                         10.0f * entry->scale.x, 3);
            break;
    }
    if (entry->flags & MOBJ_FLAG_PAPER_TEX) {
        animSetPaperTexMtx(NULL, NULL, NULL);
        animSetPaperTexObj(NULL, NULL, NULL, NULL, NULL, NULL, 1, 1);
    }
}

void mobjDisp_OffscreenXLU(CameraId camId, void* param) {
    MapObjectEntry* entry = param;
    u16 left, top, right, bottom;

    sysWaitDrawSync();
    GXClearBoundingBox();
    if (entry->flags & MOBJ_FLAG_PAPER_TEX) {
        animSetPaperTexMtx(entry->paperTexMtx, NULL, NULL);
        animSetPaperTexObj(&entry->paperTexObj, NULL, NULL, entry->paperTexData, NULL, NULL, 1, 1);
    }
    animPoseDraw(entry->poseId, entry->position.x, entry->position.y, entry->position.z, entry->rotation.y,
                 10.0f * entry->scale.x, 1);
    animPoseDraw(entry->poseId, entry->position.x, entry->position.y, entry->position.z, entry->rotation.y,
                 10.0f * entry->scale.x, 2);
    animPoseDraw(entry->poseId, entry->position.x, entry->position.y, entry->position.z, entry->rotation.y,
                 10.0f * entry->scale.x, 3);
    if (entry->flags & MOBJ_FLAG_PAPER_TEX) {
        animSetPaperTexMtx(NULL, NULL, NULL);
        animSetPaperTexObj(NULL, NULL, NULL, NULL, NULL, NULL, 1, 1);
    }
    sysWaitDrawSync();
    GXReadBoundingBox(&left, &top, &right, &bottom);
    offscreenAddBoundingBox(entry->offscreenId, left, top, right, bottom);
}

void mobjInit(void) {
    MapObjectWork* wp = &work[0];

    wp->count = 16;
    wp->entries = __memAlloc(HEAP_DEFAULT, sizeof(MapObjectEntry) * wp->count);
    memset(wp->entries, 0, sizeof(MapObjectEntry) * wp->count);

    wp = &work[1];
    wp->count = 8;
    wp->entries = __memAlloc(HEAP_DEFAULT, sizeof(MapObjectEntry) * wp->count);
    memset(wp->entries, 0, sizeof(MapObjectEntry) * wp->count);

    koopaRunFlag = FALSE;
}

void mobjReset(BOOL inBattle) {
    MapObjectWork* wp = mobjGetWorkFlag(inBattle);
    memset(wp->entries, 0, sizeof(MapObjectEntry) * wp->count);
    koopaRunFlag = FALSE;
}

void mobjHitEntry(MapObjectEntry* entry, s32 type) {
    AnimationPoseData* pose;
    Mtx mtx;
    Vec min;
    Vec max;
    Vec position;
    s32 i;

    pose = animPoseGetAnimBaseDataPtr(entry->poseId);
    min = (Vec){pose->bboxMin.x, pose->bboxMin.y, pose->bboxMin.z};
    max = (Vec){pose->bboxMax.x, pose->bboxMax.y, pose->bboxMax.z};
    for (i = 0; i < 2; i++) {
        if (entry->hitObj[i].hitObject != NULL) {
            hitDelete(entry->hitObj[i].name);
        }
    }
    PSVECAdd(&max, &min, &entry->translation);
    PSVECScale(&entry->translation, &entry->translation, 0.5f);
    PSVECScale(&entry->translation, &entry->translation, 10.0f);
    entry->translation.y = 0.0f;
    // FAKE: (0, x) comma expressions around the x and z __fabs calls; the plain __fabs changes the object
    entry->scale2.x = 10.0f * (entry->scale.x * (f32)(0, __fabs(max.x - min.x)));
    entry->scale2.y = 10.0f * (entry->scale.y * (f32)__fabs(max.y));
    entry->scale2.z = 10.0f * (entry->scale.z * (f32)(0, __fabs(max.z - min.z)));
    if (strncmp(pose->fileName, "HOBJ_", strlen("HOBJ_")) == 0) {
        entry->translation.z = 0.0f;
        entry->scale2.z = 10.0f * (2.5f * entry->scale.z);
    }
    switch (type) {
        case 0:
            PSVECAdd(&entry->position, &entry->translation, &position);
            {
                Mtx zrot, yrot, xrot, scale, trans;
                MTXTrans(trans, position.x, position.y, position.z);
                MTXScale(scale, entry->scale2.x, entry->scale2.y, entry->scale2.z);
                MTXRotRad(xrot, 'x', MTXDegToRad(entry->rotation.x));
                MTXRotRad(yrot, 'y', MTXDegToRad(entry->rotation.y));
                MTXRotRad(zrot, 'z', MTXDegToRad(entry->rotation.z));
                MTXConcat(trans, zrot, trans);
                MTXConcat(trans, yrot, trans);
                MTXConcat(trans, xrot, trans);
                MTXConcat(trans, scale, mtx);
            }
            memcpy(entry->hitObj[0].jointData + 8, &cubeDJ, sizeof(CubeJoint));
            strcpy(entry->hitObj[0].name, "MOBJ_");
            strcat(entry->hitObj[0].name, entry->name);
            *(char**)(entry->hitObj[0].jointData + 8) = entry->hitObj[0].name;
            entry->hitObj[0].hitObject = hitEntryMOBJ((MapFileJoint*)(entry->hitObj[0].jointData + 8), mtx);
            entry->hitObj[0].hitObject->mobj = entry;
            break;
        case 1:
            PSVECAdd(&entry->position, &entry->translation, &position);
            {
                Mtx zrot, yrot, xrot, scale, trans;
                MTXTrans(trans, position.x, position.y, position.z);
                MTXScale(scale, entry->scale2.x, entry->scale2.y, entry->scale2.z);
                MTXRotRad(xrot, 'x', MTXDegToRad(entry->rotation.x));
                MTXRotRad(yrot, 'y', MTXDegToRad(entry->rotation.y));
                MTXRotRad(zrot, 'z', MTXDegToRad(entry->rotation.z));
                MTXConcat(trans, zrot, trans);
                MTXConcat(trans, yrot, trans);
                MTXConcat(trans, xrot, trans);
                MTXConcat(trans, scale, mtx);
            }
            memcpy(entry->hitObj[0].jointData + 8, &headDJ, sizeof(CubeJoint));
            strcpy(entry->hitObj[0].name, "MOBJ_");
            strcat(entry->hitObj[0].name, entry->name);
            *(char**)(entry->hitObj[0].jointData + 8) = entry->hitObj[0].name;
            entry->hitObj[0].hitObject = hitEntryMOBJ((MapFileJoint*)(entry->hitObj[0].jointData + 8), mtx);
            entry->hitObj[0].hitObject->mobj = entry;
            break;
        case 2:
            PSVECAdd(&entry->position, &entry->translation, &position);
            {
                Mtx zrot, yrot, xrot, scale, trans;
                MTXTrans(trans, position.x, position.y, position.z);
                MTXScale(scale, entry->scale2.x, entry->scale2.y, entry->scale2.z);
                MTXRotRad(xrot, 'x', MTXDegToRad(entry->rotation.x));
                MTXRotRad(yrot, 'y', MTXDegToRad(entry->rotation.y));
                MTXRotRad(zrot, 'z', MTXDegToRad(entry->rotation.z));
                MTXConcat(trans, zrot, trans);
                MTXConcat(trans, yrot, trans);
                MTXConcat(trans, xrot, trans);
                MTXConcat(trans, scale, mtx);
            }
            memcpy(entry->hitObj[0].jointData + 8, &frontDJ, sizeof(CubeJoint));
            strcpy(entry->hitObj[0].name, "MOBJ_");
            strcat(entry->hitObj[0].name, entry->name);
            *(char**)(entry->hitObj[0].jointData + 8) = entry->hitObj[0].name;
            entry->hitObj[0].hitObject = hitEntryMOBJ((MapFileJoint*)(entry->hitObj[0].jointData + 8), mtx);
            entry->hitObj[0].hitObject->mobj = entry;
            memcpy(entry->hitObj[1].jointData + 8, &front2DJ, sizeof(CubeJoint));
            strcpy(entry->hitObj[1].name, "MOBJ~");
            strcat(entry->hitObj[1].name, entry->name);
            *(char**)(entry->hitObj[1].jointData + 8) = entry->hitObj[1].name;
            entry->hitObj[1].hitObject = hitEntryMOBJ((MapFileJoint*)(entry->hitObj[1].jointData + 8), mtx);
            entry->hitObj[1].hitObject->mobj = entry;
            break;
    }
}

s32 mobjEntry(const char* name, const char* animPoseName) {
    MapObjectWork* wp = mobjGetWork();
    s32 entryCount;
    s32 idx;
    MapObjectEntry* entry;

    entryCount = wp->count;
    for (idx = 0, entry = wp->entries; idx < entryCount; idx++, entry++) {
        if ((entry->flags & MOBJ_FLAG_ACTIVE) && (strcmp(entry->name, name) == 0)) {
            break;
        }
    }
    // the search for an active entry with the same name has no effect: its result is not used

    for (idx = 0, entry = wp->entries; idx < entryCount; idx++, entry++) {
        if ((entry->flags & MOBJ_FLAG_ACTIVE) == 0) {
            break;
        }
    }

    memset(entry, 0, sizeof(*entry));
    entry->flags |= MOBJ_FLAG_ACTIVE;
    strcpy(entry->name, name);

    entry->position = (Vec){ 0.0f, 0.0f, 0.0f };
    entry->scale = (Vec){ 1.0f, 1.0f, 1.0f };
    entry->rotation = (Vec){ 0.0f, 0.0f, 0.0f };
    entry->velocity = (Vec){ 0.0f, 0.0f, 0.0f };
    entry->gravity = 1.0f;
    entry->bounce = 1.0f;
    entry->camId = CAMERA_3D;
    entry->poseId = animPoseEntry(animPoseName, gp->inBattle != 0);
    animPosePeraOff(entry->poseId);
    strcpy(entry->animName, animPoseName);
    animPoseSetAnim(entry->poseId, "S_1", 1);
    animPoseSetMaterialLightFlagOn(entry->poseId, MATERIAL_LIGHT_CHARA);
    mobjHitEntry(entry, 0);
    entry->offscreenId = -1;
    return idx;
}

void mobjDelete(const char* name) {
    MapObjectWork* wp = mobjGetWork();
    s32 entryCount;
    int i;
    MapObjectEntry* entry;

    entryCount = wp->count;
    entry = wp->entries;
    for (i = 0; i < entryCount; i++, entry++) {
        if ((entry->flags & MOBJ_FLAG_ACTIVE) && strcmp(entry->name, name) == 0) {
            for (i = 0; i < 2; i++) {
                if ((entry->hitObj[i].name) != NULL) {
                    if (!gp->inBattle) {
                        marioResetHitObj(entry->hitObj[i].name);
                    }
                    hitDelete(entry->hitObj[i].name);
                }
            }
            entry->flags &= ~MOBJ_FLAG_ACTIVE;
            break;
        }
    }
}

void mobjSetPosition(const char* name, f32 x, f32 y, f32 z) {
    MapObjectWork* wp = mobjGetWork();
    MapObjectEntry* entry;
    s32 entryCount;
    int i;

    entryCount = wp->count;
    entry = wp->entries;
    for (i = 0; i < entryCount; i++, entry++) {
        if ((entry->flags & MOBJ_FLAG_ACTIVE) && strcmp(entry->name, name) == 0) {
            break;
        }
    }

    entry->position.x = x;
    entry->position.y = y + 0.1f;
    entry->position.z = z;
}


typedef struct MobjWorkHit {
    s32 count;
    MapObjectEntry* entries;
    HitEntry* hit[5];
} MobjWorkHit;

void mobjMain(void) {
    MobjWorkHit* wp;
    MapObjectEntry* entry;
    MarioWork* mario;
    PartyEntry* party;
    s16 fbat;
    s32 count;
    s32 i;
    s32 hit;
    HitEntry* hitJump;
    s32 partyFlag;
    HitEntry* hitPtr;
    s32 k;
    GXColor blend1;
    GXColor blend2;
    GXColor evt;
    Vec camPos;
    f32 nz, ny, nx, dist, hz, hy, hx;

    // FAKE: the work pointer goes through the block-local w; `wp = (MobjWorkHit*)mobjGetWork();` changes the object
    {
        MapObjectWork* w = mobjGetWork();
        wp = (MobjWorkHit*)w;
    }
    count = wp->count;
    entry = wp->entries;
    mario = marioGetPtr();
    party = partyGetPtr(marioGetPartyId());
    fbat = fbatGetPointer()->mode;
    hitJump = NULL;
    partyFlag = 0;
    if (party != NULL && (party->flags & PARTY_FLAG_IS_BEING_USED)) {
        partyFlag = 1;
    }

    if (gp->inBattle == 0) {
        if (mario->hitobjHammer != NULL) {
            if (wp->hit[0] == NULL || strcmp(hitGetName(wp->hit[0]), hitGetName(mario->hitobjHammer)) != 0) {
                wp->hit[0] = mario->hitobjHammer;
            }
        } else {
            wp->hit[0] = NULL;
        }
        if (mario->hitobjHead != NULL) {
            if (wp->hit[1] == NULL || strcmp(hitGetName(wp->hit[1]), hitGetName(mario->hitobjHead)) != 0) {
                hitJump = wp->hit[1] = mario->hitobjHead;
            }
        } else {
            wp->hit[1] = NULL;
        }
        if (mario->hitobjStandOn != NULL) {
            if (wp->hit[2] == NULL || strcmp(hitGetName(wp->hit[2]), hitGetName(mario->hitobjStandOn)) != 0) {
                wp->hit[2] = mario->hitobjStandOn;
            }
        } else {
            wp->hit[2] = NULL;
        }
        if (mario->hitobjFront != NULL) {
            if (wp->hit[3] == NULL || strcmp(hitGetName(wp->hit[3]), hitGetName(mario->hitobjFront)) != 0) {
                wp->hit[3] = mario->hitobjFront;
            }
        } else {
            wp->hit[3] = NULL;
        }
        if (party != NULL && party->hitobjAction != NULL) {
            if (wp->hit[4] == NULL || strcmp(hitGetName(wp->hit[4]), hitGetName(party->hitobjAction)) != 0) {
                wp->hit[4] = party->hitobjAction;
            }
        } else {
            wp->hit[4] = NULL;
        }
    }

    for (i = 0; i < count; i++, entry++) {
        if (!(entry->flags & MOBJ_FLAG_ACTIVE)) {
            continue;
        }
        // FAKE: `gp->inBattle != 0 ||` is always false here; without it the object changes
#ifdef VERSION_G8ME01
        if (gp->inBattle == 0 && (gp->inBattle != 0 || ((u32)fbat > 1 && fbat != 8)) && !(entry->flags & MOBJ_FLAG_PAPER_TEX)) {
#else
        if (gp->inBattle == 0 && (gp->inBattle != 0 || ((u32)fbat > 1 && fbat != 8))) {
#endif
            goto afterHit;
        }
        if (entry->flags & MOBJ_FLAG_UPDATE_MTX) {
            if (mobjCheckItemboxOpen(entry) == 2) {
                mobjCalcMtx2(entry);
            } else {
                mobjCalcMtx(entry);
            }
            entry->flags &= ~MOBJ_FLAG_UPDATE_MTX;
        }
        if (entry->flags & MOBJ_FLAG_GRAVITY) {
            if (entry->velocity.y >= 0.0f) {
                entry->position.y += entry->velocity.y;
                entry->velocity.y -= entry->gravity;
            } else {
                dist = 10.0f + (1.0f + (f32)__fabs(entry->velocity.y));
                if (hitCheckFilter(NULL, &hx, &hy, &hz, &dist, &nx, &ny, &nz, entry->position.x,
                                   10.0f + entry->position.y, entry->position.z, 0.0f, -1.0f, 0.0f) != NULL) {
                    entry->position.y = hy;
                    entry->velocity.y = (-0.5f * entry->bounce) * entry->velocity.y;
                    if (__fabs(entry->velocity.y) <= 2.0) {
                        entry->velocity.y = 0.0f;
                    }
                } else {
                    entry->position.y += entry->velocity.y;
                    entry->velocity.y -= entry->gravity;
                }
            }
            mobjCalcMtx(entry);
        }
        if (!(entry->flags & MOBJ_FLAG_HIT)) {
            hit = 0;
            hitPtr = entry->hitObj[0].hitObject;
            if (mario->characterId != CHARACTER_BOWSER) {
                if ((entry->flags & MOBJ_FLAG_HIT_HAMMER) && mario->hitobjHammer == hitPtr) {
                    if (marioGetPtr()->position.y <= 5.0f + entry->position.y) {
                        entry->hitFlag = MOBJ_FLAG_HIT_HAMMER;
                        hit = 1;
                        goto hitDone;
                    }
                }
                if ((entry->flags & MOBJ_FLAG_HIT_HEAD) && hitJump == hitPtr) {
                    entry->hitFlag = MOBJ_FLAG_HIT_HEAD;
                    hit = 1;
                    goto hitDone;
                }
                if ((entry->flags & MOBJ_FLAG_HIT_STAND) && mario->hitobjStandOn == hitPtr) {
                    entry->hitFlag = MOBJ_FLAG_HIT_STAND;
                    hit = 1;
                    goto hitDone;
                }
                if ((entry->flags & MOBJ_FLAG_HIT_SPIN_JUMP) && mario->hitobjStandOn == hitPtr && marioChkHipBump()) {
                    entry->hitFlag = MOBJ_FLAG_HIT_SPIN_JUMP;
                    hit = 1;
                    goto hitDone;
                }
                if (marioFrontActionValidChk() && (entry->flags & MOBJ_FLAG_HIT_A_BUTTON) && mario->hitobjFront == hitPtr &&
                    (mario->buttonTrg & PAD_BUTTON_A) && !partyFlag) {
                    entry->hitFlag = MOBJ_FLAG_HIT_A_BUTTON;
                    hit = 1;
                    goto hitDone;
                }
                if ((entry->flags & MOBJ_FLAG_HIT_PARTY) && party != NULL) {
                    if (party->hitobjAction == hitPtr) {
                        entry->hitFlag = MOBJ_FLAG_HIT_PARTY;
                        hit = 1;
                        goto hitDone;
                    } else if (party->hitobjAction != NULL && party->currentMemberId == PARTY_MEMBER_BOBBERY) {
                        if (PSVECDistance(&party->position, &entry->position) <
                            entry->hitObj[0].hitObject->radius + party->width) {
                            entry->hitFlag = MOBJ_FLAG_HIT_PARTY;
                            hit = 1;
                            goto hitDone;
                        }
                    }
                }
                if ((entry->flags & MOBJ_FLAG_HIT_BOBBERY) && party != NULL && party->currentMemberId == PARTY_MEMBER_BOBBERY) {
                    if (party->hitobjAction == hitPtr) {
                        entry->hitFlag = MOBJ_FLAG_HIT_BOBBERY;
                        hit = 1;
                        goto hitDone;
                    } else if (party->hitobjAction != NULL) {
                        if (PSVECDistance(&party->position, &entry->position) <
                            entry->hitObj[0].hitObject->radius + party->width) {
                            entry->hitFlag = MOBJ_FLAG_HIT_PARTY;
                            hit = 1;
                            goto hitDone;
                        }
                    }
                }
            } else {
                if (entry->flags & MOBJ_FLAG_HIT_KOOPA_HEAD) {
                    for (k = 0; k < 10; k++) {
                        if (kpaGetHitobjHead(k) == hitPtr) {
                            entry->hitFlag = MOBJ_FLAG_HIT_KOOPA_HEAD;
                            hit = 1;
                            break;
                        }
                    }
                }
                if (entry->flags & MOBJ_FLAG_HIT_KOOPA_HIP) {
                    for (k = 0; k < 10 && hit == 0; k++) {
                        if (kpaGetHitobjRide(k) == hitPtr && kpaHipAttackCheck()) {
                            entry->hitFlag = MOBJ_FLAG_HIT_KOOPA_HIP;
                            hit = 1;
                            break;
                        }
                    }
                }
                if ((entry->flags & MOBJ_FLAG_HIT_KOOPA_BODY_LV3) && kpaGetHitobj() == hitPtr && kpaGetLevel() == 3) {
                    entry->hitFlag = MOBJ_FLAG_HIT_KOOPA_BODY_LV3;
                    hit = 1;
                    goto hitDone;
                }
                if ((entry->flags & MOBJ_FLAG_HIT_KOOPA_FLAME) && kpaGetHitFlame() == hitPtr) {
                    entry->hitFlag = MOBJ_FLAG_HIT_KOOPA_FLAME;
                    hit = 1;
                    goto hitDone;
                }
                if ((entry->flags & MOBJ_FLAG_HIT_KOOPA_FRONT) && mario->hitobjFront == hitPtr) {
                    entry->hitFlag = MOBJ_FLAG_HIT_KOOPA_FRONT;
                    hit = 1;
                    goto hitDone;
                }
                if ((entry->flags & MOBJ_FLAG_HIT_STAND) && mario->hitobjStandOn == hitPtr) {
                    entry->hitFlag = MOBJ_FLAG_HIT_STAND;
                    hit = 1;
                }
            }
        hitDone:
            if (hit != 0) {
                entry->flags |= MOBJ_FLAG_HIT;
            }
        }
        if (entry->flags & MOBJ_FLAG_HIT_EVERY_FRAME) {
            entry->flags &= ~MOBJ_FLAG_HIT;
        }
        if (marioStGetSystemLevel() != 0) {
            entry->flags &= ~MOBJ_FLAG_HIT;
        }
        {
            MarioMotion motion = marioGetPtr()->currMotionId;
            if (motion == MARIO_MOTION_DAMAGE || motion == MARIO_MOTION_DAMAGE_TOGE) {
                entry->flags &= ~MOBJ_FLAG_HIT;
            }
        }
        if (entry->mainFunc != NULL) {
            if (entry->mainFunc(entry) == -1) {
                continue;
            }
        }
    afterHit:
        if (!(entry->flags & MOBJ_FLAG_ACTIVE)) {
            continue;
        }
        if (!koopaRunFlag && !(entry->flags & (MOBJ_FLAG_NO_SHADOW | MOBJ_FLAG_HIDDEN))) {
            MapWork* mw = mapGetWork();
            s32 shadowId;
            mapGetBlend(&blend1);
            mapGetBlend2(&blend2);
            if ((mw->entries[0].flags & MAP_ENTRY_BLEND) && !(animPoseGetMaterialFlag(entry->poseId) & MAP_OBJ_FLAG_NO_MAP_BLEND) && blend1.a != 0xff) {
            } else if ((mw->entries[0].flags & MAP_ENTRY_BLEND2) && !(animPoseGetMaterialFlag(entry->poseId) & MAP_OBJ_FLAG_NO_MAP_BLEND2) &&
                       blend2.a != 0xff) {
            } else {
                shadowId = shadowEntry(entry->position.x, entry->position.y, entry->position.z,
                                       10.0f * entry->scale.x * entry->scale2.x / 25.0f);
                if (entry->flags & MOBJ_FLAG_HIT_HEAD) {
                    shadowSetType(shadowId, 1);
                    shadowSetSize(shadowId, 10.0f * entry->scale.x * entry->scale2.x / 20.0f);
                }
            }
        }
        animPoseMain(entry->poseId);
        evt = animPoseGetMaterialEvtColor(entry->poseId);
        if (entry->flags & MOBJ_FLAG_NO_DISP) {
            continue;
        }
        if (!(entry->flags & MOBJ_FLAG_NO_CULL)) {
            PSMTXMultVec(camGetPtr(CAMERA_3D)->view, &entry->position, &camPos);
            PSMTX44MultVec(camGetPtr(CAMERA_3D)->projection, &camPos, &camPos);
            if (__fabs(camPos.x) > 1.5 || __fabs(camPos.y) > 1.5 || __fabs(camPos.z) > 1.0) {
                continue;
            }
        }
        if ((entry->flags & MOBJ_FLAG_BLINK) && ((gp->retraceCount >> 3) & 1)) {
            continue;
        }
        if (entry->offscreenId == -1) {
            if ((animPoseGetMaterialFlag(entry->poseId) & MAP_OBJ_FLAG_COLOR) && evt.a != 0xff) {
                dispEntry(entry->camId, dispCalcZ(&entry->position), 2, mobjDispXLU, entry);
            } else {
                dispEntry(entry->camId, dispCalcZ(&entry->position), 0, mobjDisp, entry);
                dispEntry(entry->camId, dispCalcZ(&entry->position), 1, mobjDisp, entry);
                dispEntry(entry->camId, dispCalcZ(&entry->position), 2, mobjDisp, entry);
            }
        } else {
            if ((animPoseGetMaterialFlag(entry->poseId) & MAP_OBJ_FLAG_COLOR) && evt.a != 0xff) {
                dispEntry(CAMERA_OFFSCREEN2, dispCalcZ(&entry->position), 2, mobjDisp_OffscreenXLU, entry);
            } else {
                dispEntry(CAMERA_OFFSCREEN2, dispCalcZ(&entry->position), 0, mobjDisp_OffscreenXLU, entry);
                dispEntry(CAMERA_OFFSCREEN2, dispCalcZ(&entry->position), 1, mobjDisp_OffscreenXLU, entry);
                dispEntry(CAMERA_OFFSCREEN2, dispCalcZ(&entry->position), 2, mobjDisp_OffscreenXLU, entry);
            }
        }
    }
}


MapObjectEntry* mobjNameToPtr(const char* name) {
    int i;
    s32 entryCount;
    MapObjectEntry* entry;
    MapObjectWork* wp = mobjGetWork();

    entryCount = wp->count;
    entry = wp->entries;
    for (i = 0; i < entryCount; i++, entry++) {
        if ((entry->flags & MOBJ_FLAG_ACTIVE) && strcmp(entry->name, name) == 0) {
            break;
        }
    }

    return entry;
}

MapObjectEntry* mobjNameToPtrNoAssert(const char* name) {
    int i;
    s32 entryCount;
    MapObjectEntry* entry;
    MapObjectWork* wp = mobjGetWork();

    entryCount = wp->count;
    entry = wp->entries;
    for (i = 0; i < entryCount; i++, entry++) {
        if ((entry->flags & MOBJ_FLAG_ACTIVE) && strcmp(entry->name, name) == 0) {
            break;
        }
    }

    if (i >= entryCount) {
        return NULL;
    }

    return entry;
}

MapObjectEntry* mobjHitObjPtrToPtr(HitEntry* hitObj) {
    if (hitObj->attributes & HIT_ATTR_MOBJ) {
        return hitObj->mobj;
    }

    return NULL;
}

EventEntry* mobjRunEvent(MapObjectEntry* entry, EvtWord* eventCode) {
    EventEntry* event;

    if (eventCode == NULL) {
        return NULL;
    }
    if (evtCheckID(entry->eventId)) {
        return NULL;
    }

    entry->eventId = 0;
    event = evtEntryType(eventCode, 0x1E, 0, 0x1A);
    event->thisMapObj = entry;
    entry->eventId = event->eventId;

    return event;
}

static void calcMtx(Mtx mtx, Vec* pos, Vec* sc, Vec* rot) {
    Mtx trans;
    Mtx scale;
    Mtx xrot;
    Mtx yrot;
    Mtx zrot;
    MTXTrans(trans, pos->x, pos->y, pos->z);
    MTXScale(scale, sc->x, sc->y, sc->z);
    MTXRotRad(xrot, 'x', MTXDegToRad(rot->x));
    MTXRotRad(yrot, 'y', MTXDegToRad(rot->y));
    MTXRotRad(zrot, 'z', MTXDegToRad(rot->z));
    MTXConcat(trans, zrot, trans);
    MTXConcat(trans, yrot, trans);
    MTXConcat(trans, xrot, trans);
    MTXConcat(trans, scale, mtx);
}

void mobjCalcMtx(MapObjectEntry* entry) {
    AnimationPoseData* pose;
    Mtx mtx;
    Vec position;
    Vec min;
    Vec max;
    s32 i;

    PSVECAdd(&entry->position, &entry->translation, &position);
    pose = animPoseGetAnimBaseDataPtr(entry->poseId);
    min = (Vec){pose->bboxMin.x, pose->bboxMin.y, pose->bboxMin.z};
    max = (Vec){pose->bboxMax.x, pose->bboxMax.y, pose->bboxMax.z};
    entry->scale2.x = 10.0f * (entry->scale.x * (f32)__fabs(max.x - min.x));
    entry->scale2.y = 10.0f * (entry->scale.y * (f32)__fabs(max.y));
    entry->scale2.z = 10.0f * (entry->scale.z * (f32)__fabs(max.z - min.z));
    if (strncmp(pose->fileName, "HOBJ_", strlen("HOBJ_")) == 0) {
        entry->translation.z = 0.0f;
        entry->scale2.z = 10.0f * (2.5f * entry->scale.z);
    }
    calcMtx(mtx, &position, &entry->scale2, &entry->rotation);
    for (i = 0; i < 2; i++) {
        if (entry->hitObj[i].hitObject != NULL) {
            hitReCalcMatrix(entry->hitObj[i].hitObject, mtx);
        }
    }
}

void mobjCalcMtx2(MapObjectEntry* entry) {
    Mtx mtx;
    Vec position;
    s32 i;

    PSVECAdd(&entry->position, &entry->translation, &position);

    calcMtx(mtx, &position, &entry->scale2, &entry->rotation);

    for (i = 0; i < 2; i++) {
        if (entry->hitObj[i].hitObject != NULL) {
            hitReCalcMatrix(entry->hitObj[i].hitObject, mtx);
        }
    }
}

BOOL mobjGetHint(MapObjectEntry* entry) {
    if (entry == NULL) {
        return FALSE;
    }
    return (entry->flags >> 5) & 1;
}

BOOL mobjCheckExec(void) {
    MapObjectWork* wp = mobjGetWork();
    int i;
    MapObjectEntry* entry;

    for (i = 0, entry = wp->entries; i < wp->count; entry++, i++) {
        if (entry->flags & 0x200) {
            return TRUE;
        }
    }
    return FALSE;
}

static int mobjCheckKururingFloorItem(MapObjectEntry* entry) { // was always inlined in retail but not in demo
    if (!strcmp(entry->animName, kururinName)) {
        ItemEntry* item = itemNameToPtr(entry->name);
        if (item != NULL && (item->flags & ITEM_FLAG_ACTIVE) && evtGetValue(0, entry->flagVar) == 0) {
            return 1;
        } else if (evtGetValue(0, entry->flagVar) != 0) {
            return 1;
        } else {
            return 2;
        }
    } else {
        return 0;
    }
}

s32 mobjCheckItemboxOpen(MapObjectEntry* entry) {
    if (strcmp(entry->animName, "MOBJ_TreasureBox") == 0 || strcmp(entry->animName, "MOBJ_BigTreasureBox") == 0 ||
        strcmp(entry->animName, "MOBJ_GrayTreasureBox") == 0 || strcmp(entry->animName, "MOBJ_BlackTreasureBox") == 0) {
        if (entry->state == 99) {
            return 2;
        } else {
            return 1;
        }
    } else {
        return 0;
    }
}


MapObjectEntry* mobjNearDistCheck2(f32 x, f32 y, f32 z, f32 minMagnitude, char** names) {
    int entryCount;
    Vec pos;
    Vec dist;
    Vec _pos;
    Vec unusedVec;
    MapObjectEntry* entry;
    MapObjectWork* wp = mobjGetWork();
    MapObjectEntry* ret;
    int i;
    float mag;
    char** animName;

    entryCount = wp->count;
    entry = wp->entries;
    ret = NULL;

    // FAKE: unusedVec is never read and the position goes through the zeroed copy _pos; setting pos.x/y/z directly changes the object
    unusedVec = (Vec){ 0.0f, 0.0f, 0.0f };
    _pos = (Vec){ 0.0f, 0.0f, 0.0f };
    _pos.x = x;
    _pos.y = y;
    _pos.z = z;
    pos = _pos;

    for (i = 0; i < entryCount; i++, entry++) {
        if ((entry->flags & MOBJ_FLAG_ACTIVE) == 0) {
            continue;
        }

        animName = names;
        while (*animName != NULL) {
            if (strcmp(entry->animName, *animName) == 0) {
                if (strcmp(entry->animName, "MOBJ_TreasureBox") == 0 ||
                    strcmp(entry->animName, "MOBJ_BigTreasureBox") == 0 ||
                    strcmp(entry->animName, "MOBJ_GrayTreasureBox") == 0 ||
                    strcmp(entry->animName, "MOBJ_BlackTreasureBox") == 0) {
                    if (mobjCheckItemboxOpen(entry) != 1) {
                        animName++;
                        continue;
                    }
                } else if (strcmp(entry->animName, kururinName) == 0) {
                    if (mobjCheckKururingFloorItem(entry) != 2) {
                        animName++;
                        continue;
                    }
                }

                VECSubtract(&entry->position, &pos, &dist);
                mag = VECMag(&dist);
                if (mag < minMagnitude) {
                    minMagnitude = mag;
                    ret = entry;
                }
                break;
            }
            animName++;
        }
    }

    return ret;
}

const char kururinName[] = "MOBJ_KururinFloor";

// FAKE: stand-in, never called (in neither symbol map; the linker strips it); it only places its literals in the .sdata2 pool
void mobjStrippedFallPool(MapObjectEntry* entry) {
    entry->velocity.y = 0.0f;
    entry->position.y = 10.0f;
    entry->position.x = 1.0f;
    entry->position.z = -1.0f;
    entry->velocity.x = -0.5f;
}
