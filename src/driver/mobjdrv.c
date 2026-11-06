// Map object (mobj) driver: blocks, item boxes, switches and the other interactive map objects:
// their update, hit checks and drawing.

#include "version.h"
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

/* static map-file joint data for the mobj cube models (MapFileJoint / MapJointDrawMode, as in a loaded map file) */
typedef struct CubeMesh {
    u8 reserved_00;       // 0x0
    u8 padding[2];        // 0x1
    u8 isPackedDisplay;   // 0x3
    s32 displayListCount; // 0x4
    u32 elementMask;      // 0x8
    void* vcdTable;       // 0xC, cubeDDT
    u32* lists[15];       // 0x10, display lists in cubeDN
} CubeMesh;

/** Draw mode of the built-in collision cube (hit attribute HIT_ATTR_MOBJ set), shared by the cube, head and front hit joints. */
#if VERSION >= VERSION_JP
MapJointDrawMode cubeMSJ = {0, 1, 0, 0, 0, (s32)0x80000000};
#endif

/** Draw mode of the second part of a two-part object's collision (the MOBJ~ part). */
#if VERSION >= VERSION_JP
MapJointDrawMode front2MSJ = {0, 1, 0, 0, 0, 0};
#endif

// cube vertex positions (cubeDVP) and normals (cubeDVN), listed in cubeDDT

// static data from inlined header functions
typedef struct CubeVtxData34 {
    s32 count;
    Vec v[34];
} CubeVtxData34;
typedef struct CubeVtxData18 {
    s32 count;
    Vec v[18];
} CubeVtxData18;

/** Corners of the built-in unit collision cube (1 wide and deep, 1 tall, base at the origin). */
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

/** Face normals of the built-in collision cube: front, top, back, bottom, right, left. */
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

/** Vertex data table of the collision cube (positions and normals); its meshes point here. */
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

/** Mesh of the full collision cube: all six faces. */
CubeMesh cubeDP = {0, {0}, 0, 6, 0, cubeDDT, {cubeDN, &cubeDN[205], &cubeDN[410], &cubeDN[615], &cubeDN[820], &cubeDN[1025]}};

#if VERSION <= VERSION_DEMO_JP
MapJointDrawMode cubeMSJ = {0, 1, 0, 0, 0, (s32)0x80000000};
#endif

/** Joint of the solid cube collision (type 0) that mobjHitEntry copies into a block's hit object. */
MapFileJoint cubeDJ = {
    NULL, NULL, NULL, NULL, NULL, NULL, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f},
    {-0.5f, 0.0f, -0.5f}, {0.5f, 1.0f, 0.5f}, {0}, &cubeMSJ, 1,
    {{NULL, (MapFileMesh*)&cubeDP}},
};

/** Mesh of the head-hit collision: only the cube's bottom face, struck from below. */
CubeMesh headDP = {0, {0}, 0, 1, 0, cubeDDT, {&cubeDN[410]}};

/** Joint of the head-hit collision (type 1), a flat square at the object's base, copied in by mobjHitEntry. */
MapFileJoint headDJ = {
    NULL, NULL, NULL, NULL, NULL, NULL, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f},
    {-0.5f, 0.0f, -0.5f}, {0.5f, 0.0f, 0.5f}, {0}, &cubeMSJ, 1,
    {{NULL, (MapFileMesh*)&headDP}},
};

/** Mesh of the front hit part: only the cube's front face. */
CubeMesh frontDP = {0, {0}, 0, 1, 0, cubeDDT, {&cubeDN[1025]}};

/** Joint of a two-part object's front hit part (type 2), copied in by mobjHitEntry. */
MapFileJoint frontDJ = {
    NULL, NULL, NULL, NULL, NULL, NULL, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f},
    {-0.5f, 0.0f, -0.5f}, {0.5f, 1.0f, 0.5f}, {0}, &cubeMSJ, 1,
    {{NULL, (MapFileMesh*)&frontDP}},
};

#if VERSION <= VERSION_DEMO_JP
MapJointDrawMode front2MSJ = {0, 1, 0, 0, 0, 0};
#endif

/** Mesh of a two-part object's second hit part: the five faces other than the front one. */
CubeMesh front2DP = {0, {0}, 0, 5, 0, cubeDDT, {cubeDN, &cubeDN[205], &cubeDN[410], &cubeDN[615], &cubeDN[820]}};

/** Joint of a two-part object's second hit part (type 2), copied in by mobjHitEntry. */
MapFileJoint front2DJ = {
    NULL, NULL, NULL, NULL, NULL, NULL, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f},
    {-0.5f, 0.0f, -0.5f}, {0.5f, 1.0f, 0.5f}, {0}, &front2MSJ, 1,
    {{NULL, (MapFileMesh*)&front2DP}},
};

#define MAX_MOBJ_WORK 3

//.sbss
BOOL koopaRunFlag;

//.bss
static MapObjectWork work[MAX_MOBJ_WORK];

static inline MapObjectWork* mobjGetWorkFlag(BOOL inBattle) {
    return inBattle ? &work[1] : &work[0];
}

/**
 * Switches to a separate 128-entry map object table (in map memory) for Bowser's side-scrolling stages
 * and marks them running; nothing in the main program calls it, so it comes from the map code.
 * mobjReset turns it off again.
 */
void mobjKoopaOn(void) {
    if (!koopaRunFlag) {
        MapObjectWork* wp = &work[2];

        koopaRunFlag = TRUE;
        wp->count = 128;
        wp->entries = _mapAlloc(sizeof(MapObjectEntry) * wp->count);
        memset(wp->entries, 0, sizeof(MapObjectEntry) * wp->count);
    }
}

/** Returns the map object table in use: Bowser's stage table while it runs, else the field or battle one. */
static MapObjectWork* mobjGetWork(void) {
    if (koopaRunFlag) {
        return &work[2];
    } else {
        return mobjGetWorkFlag(gp->inBattle);
    }
}

/**
 * Draw callback for a translucent map object: draws its pose's opaque and translucent parts in one go,
 * with its paper texture if it has one.
 */
static void mobjDispXLU(CameraId camId, void* param) {
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

/**
 * Draw callback for a map object, called once per render pass (opaque, alpha-tested, translucent);
 * draws its pose with its paper texture if it has one.
 */
static void mobjDisp(CameraId camId, void* param) {
    MapObjectEntry* entry = param;
    DispEntry* disp;

    if (entry->flags & MOBJ_FLAG_PAPER_TEX) {
        animSetPaperTexMtx(entry->paperTexMtx, NULL, NULL);
        animSetPaperTexObj(&entry->paperTexObj, NULL, NULL, entry->paperTexData, NULL, NULL, 1, 1);
    }
    disp = dispGetCurWork();
    switch (disp->renderMode) {
        case DISP_MODE_OPAQUE:
            animPoseDraw(entry->poseId, entry->position.x, entry->position.y, entry->position.z, entry->rotation.y,
                         10.0f * entry->scale.x, 1);
            break;
        case DISP_MODE_ALPHA_TEST:
            animPoseDraw(entry->poseId, entry->position.x, entry->position.y, entry->position.z, entry->rotation.y,
                         10.0f * entry->scale.x, 2);
            break;
        case DISP_MODE_XLU:
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

/**
 * Draw callback on the offscreen camera for a map object with an offscreen region:
 * draws its pose and grows the region by the screen box it covered.
 */
static void mobjDisp_OffscreenXLU(CameraId camId, void* param) {
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

/** Allocates the field (16) and battle (8) map object tables; called once at boot. */
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

/** Clears the field or battle map object table and leaves Bowser's stage mode; run when a map or battle starts. */
void mobjReset(BOOL inBattle) {
    MapObjectWork* wp = mobjGetWorkFlag(inBattle);
    memset(wp->entries, 0, sizeof(MapObjectEntry) * wp->count);
    koopaRunFlag = FALSE;
}

/**
 * Creates the collision for a map object from its pose's bounding box, replacing any it had:
 * type 0 a solid cube (MOBJ_<name>), 1 only the cube's bottom face, for head hits,
 * 2 the front face plus a second part with the other five faces (MOBJ~<name>) for objects with two hit parts.
 */
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
            memcpy(entry->hitObj[0].jointData + 8, &cubeDJ, sizeof(MapFileJoint));
            strcpy(entry->hitObj[0].name, "MOBJ_");
            strcat(entry->hitObj[0].name, entry->name);
            *(char**)(entry->hitObj[0].jointData + 8) = entry->hitObj[0].name;
            entry->hitObj[0].hitObject = hitEntryMOBJ((MapFileJoint*)(entry->hitObj[0].jointData + 8), mtx);
#if VERSION >= VERSION_JP
            entry->hitObj[0].hitObject->mobj = entry;
#endif
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
            memcpy(entry->hitObj[0].jointData + 8, &headDJ, sizeof(MapFileJoint));
            strcpy(entry->hitObj[0].name, "MOBJ_");
            strcat(entry->hitObj[0].name, entry->name);
            *(char**)(entry->hitObj[0].jointData + 8) = entry->hitObj[0].name;
            entry->hitObj[0].hitObject = hitEntryMOBJ((MapFileJoint*)(entry->hitObj[0].jointData + 8), mtx);
#if VERSION >= VERSION_JP
            entry->hitObj[0].hitObject->mobj = entry;
#endif
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
            memcpy(entry->hitObj[0].jointData + 8, &frontDJ, sizeof(MapFileJoint));
            strcpy(entry->hitObj[0].name, "MOBJ_");
            strcat(entry->hitObj[0].name, entry->name);
            *(char**)(entry->hitObj[0].jointData + 8) = entry->hitObj[0].name;
            entry->hitObj[0].hitObject = hitEntryMOBJ((MapFileJoint*)(entry->hitObj[0].jointData + 8), mtx);
#if VERSION >= VERSION_JP
            entry->hitObj[0].hitObject->mobj = entry;
#endif
            memcpy(entry->hitObj[1].jointData + 8, &front2DJ, sizeof(MapFileJoint));
            strcpy(entry->hitObj[1].name, "MOBJ~");
            strcat(entry->hitObj[1].name, entry->name);
            *(char**)(entry->hitObj[1].jointData + 8) = entry->hitObj[1].name;
            entry->hitObj[1].hitObject = hitEntryMOBJ((MapFileJoint*)(entry->hitObj[1].jointData + 8), mtx);
#if VERSION >= VERSION_JP
            entry->hitObj[1].hitObject->mobj = entry;
#endif
            break;
    }
}

/**
 * Creates a named map object (blocks, switches, boxes, save blocks, ...) drawing animation pose animPoseName,
 * using the first free slot; returns its index. Called by the evt_mobj entry commands.
 */
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

/** Removes the named map object and its collision. */
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

/** Moves the named map object to (x, y, z). */
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


/**
 * Per-frame update of all map objects: works out which one Mario hammered, hit from below, stood on,
 * spun on or touched with an action, which one a partner, a Bobbery bomb or Bowser hit,
 * and reports it to the object; runs each object's main function, drops falling ones,
 * keeps their shadows and colours, and queues their drawing (offscreen if they have a region).
 */
void mobjMain(void) {
    MapObjectWork* wp;
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

    wp = mobjGetWork();
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
#if VERSION <= VERSION_DEMO_JP
        if (!(gp->inBattle == 0 && (gp->inBattle != 0 || ((u32)fbat > FBAT_MODE_FIELD && fbat != FBAT_MODE_EVENT_MOBJ_ACTIVE)))) {
#else
        // every version reads inBattle again; `gp->inBattle != 0 ||` is always false here
#if VERSION >= VERSION_US
        if (gp->inBattle == 0 && (gp->inBattle != 0 || ((u32)fbat > FBAT_MODE_FIELD && fbat != FBAT_MODE_EVENT_MOBJ_ACTIVE)) && !(entry->flags & MOBJ_FLAG_PAPER_TEX)) {
#else
        if (gp->inBattle == 0 && (gp->inBattle != 0 || ((u32)fbat > FBAT_MODE_FIELD && fbat != FBAT_MODE_EVENT_MOBJ_ACTIVE))) {
#endif
            goto afterHit;
        }
#endif
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
#if VERSION <= VERSION_DEMO_JP
                dist = 1.0f + (f32)__fabs(entry->velocity.y);
                if (hitCheckFilter(NULL, &hx, &hy, &hz, &dist, &nx, &ny, &nz, entry->position.x,
                                   10.0f + entry->position.y, entry->position.z, 0.0f, -10.0f, 0.0f) != NULL) {
#else
                dist = 10.0f + (1.0f + (f32)__fabs(entry->velocity.y));
                if (hitCheckFilter(NULL, &hx, &hy, &hz, &dist, &nx, &ny, &nz, entry->position.x,
                                   10.0f + entry->position.y, entry->position.z, 0.0f, -1.0f, 0.0f) != NULL) {
#endif
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
                // The first hit kind that applies wins. The hammer and partner checks
                // fall through to the next kind when their inner test fails.
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
#if VERSION <= VERSION_DEMO_JP
                if ((entry->flags & MOBJ_FLAG_HIT_A_BUTTON) && mario->hitobjFront == hitPtr &&
#else
                if (marioFrontActionValidChk() && (entry->flags & MOBJ_FLAG_HIT_A_BUTTON) && mario->hitobjFront == hitPtr &&
#endif
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
                        // Bobbery's distance fallback reports MOBJ_FLAG_HIT_PARTY, unlike the direct hit above (MOBJ_FLAG_HIT_BOBBERY); the events tell the two apart.
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
                    for (k = 0; k < KPA_HIT_SLOTS; k++) {
                        if (kpaGetHitobjHead(k) == hitPtr) {
                            entry->hitFlag = MOBJ_FLAG_HIT_KOOPA_HEAD;
                            hit = 1;
                            break;
                        }
                    }
                }
                if (entry->flags & MOBJ_FLAG_HIT_KOOPA_HIP) {
                    for (k = 0; k < KPA_HIT_SLOTS && hit == 0; k++) {
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
#if VERSION <= VERSION_DEMO_JP
        if ((u32)marioStGetSystemLevel() != 0) {
            entry->flags &= ~MOBJ_FLAG_HIT;
#else
        if (marioStGetSystemLevel() != 0) {
            entry->flags &= ~MOBJ_FLAG_HIT;
        }
        {
            MarioMotion motion = marioGetPtr()->currMotionId;
            if (motion == MARIO_MOTION_DAMAGE || motion == MARIO_MOTION_DAMAGE_TOGE) {
                entry->flags &= ~MOBJ_FLAG_HIT;
            }
#endif
        }
        if (entry->mainFunc != NULL) {
            if (entry->mainFunc(entry) == -1) {
                continue;
            }
        }
#if VERSION <= VERSION_DEMO_JP
    }
#else
        afterHit:
#endif
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
                    shadowSetType(shadowId, SHADOW_TYPE_CUBE);
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
                dispEntry(entry->camId, dispCalcZ(&entry->position), DISP_MODE_XLU, mobjDispXLU, entry);
            } else {
                dispEntry(entry->camId, dispCalcZ(&entry->position), DISP_MODE_OPAQUE, mobjDisp, entry);
                dispEntry(entry->camId, dispCalcZ(&entry->position), DISP_MODE_ALPHA_TEST, mobjDisp, entry);
                dispEntry(entry->camId, dispCalcZ(&entry->position), DISP_MODE_XLU, mobjDisp, entry);
            }
        } else {
            if ((animPoseGetMaterialFlag(entry->poseId) & MAP_OBJ_FLAG_COLOR) && evt.a != 0xff) {
                dispEntry(CAMERA_OFFSCREEN2, dispCalcZ(&entry->position), DISP_MODE_XLU, mobjDisp_OffscreenXLU, entry);
            } else {
                dispEntry(CAMERA_OFFSCREEN2, dispCalcZ(&entry->position), DISP_MODE_OPAQUE, mobjDisp_OffscreenXLU, entry);
                dispEntry(CAMERA_OFFSCREEN2, dispCalcZ(&entry->position), DISP_MODE_ALPHA_TEST, mobjDisp_OffscreenXLU, entry);
                dispEntry(CAMERA_OFFSCREEN2, dispCalcZ(&entry->position), DISP_MODE_XLU, mobjDisp_OffscreenXLU, entry);
            }
        }
    }
}


/** Returns the map object with this name; one past the table when there is none (the name is expected to exist). */
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

/** Returns the map object with this name, or NULL if there is none. */
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

/**
 * the early demos search the mobj list for the entry whose first hit object is hitObj; JP and later return the
 * hit object's own mobj pointer when it has HIT_ATTR_MOBJ, else NULL
 */
MapObjectEntry* mobjHitObjPtrToPtr(HitEntry* hitObj) {
#if VERSION <= VERSION_DEMO_JP
    int i;
    s32 entryCount;
    MapObjectEntry* entry;
    MapObjectWork* wp = mobjGetWork();

    entryCount = wp->count;
    entry = wp->entries;
    for (i = 0; i < entryCount; i++, entry++) {
        if ((entry->flags & MOBJ_FLAG_ACTIVE) && hitObj == entry->hitObj[0].hitObject) {
            break;
        }
#else
    if (hitObj->attributes & HIT_ATTR_MOBJ) {
        return hitObj->mobj;
#endif
    }

#if VERSION <= VERSION_DEMO_JP
    return entry;
#else
    return NULL;
#endif
}

/**
 * Starts script eventCode for a map object (its this-object is set),
 * unless the object's previous script is still running; returns the event or NULL.
 */
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

/** Builds a scale, x/y/z rotation (degrees) and translation matrix. */
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

/**
 * Recomputes a map object's collision matrices from its position, scale and rotation,
 * sizing the hit cube from its pose's bounding box (HOBJ_ poses get a thin fixed depth).
 */
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

/**
 * Recomputes a map object's collision matrices from its position, rotation and the already-computed hit size;
 * evt_mobj calls it as objects move.
 */
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

/**
 * Returns whether a map object shows the "!" hint balloon over Mario when he is near (MOBJ_FLAG_SHOW_HINT).
 * JP and later accept NULL.
 */
BOOL mobjGetHint(MapObjectEntry* entry) {
#if VERSION <= VERSION_DEMO_JP
    return (entry->flags & MOBJ_FLAG_SHOW_HINT) != 0;
}
#else
    if (entry == NULL) {
        return FALSE;
    }
    return (entry->flags >> 5) & 1; // MOBJ_FLAG_SHOW_HINT, normalized to BOOL
}
#endif

/** Returns TRUE while any map object is busy (MOBJ_FLAG_BUSY); the pause menu does not open then. */
BOOL mobjCheckExec(void) {
    MapObjectWork* wp = mobjGetWork();
    int i;
    MapObjectEntry* entry;

    for (i = 0, entry = wp->entries; i < wp->count; entry++, i++) {
        if (entry->flags & MOBJ_FLAG_BUSY) {
            return TRUE;
        }
    }
    return FALSE;
}

#if VERSION >= VERSION_JP

/**
 * For a MOBJ_KururinFloor object returns 2 while its item is still there to find,
 * 1 once its item is out or its flag is set, and 0 for any other object. The early demos keep it as a real function.
 */
static int mobjCheckKururingFloorItem(MapObjectEntry* entry) { // always inlined in JP and later, a real function in the early demos
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
#endif

/**
 * For treasure chests (normal, big, gray, black) returns 2 once opened (state 99) and 1 while closed;
 * 0 for any other object.
 */
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


#if VERSION <= VERSION_DEMO_JP
// FAKE: stand-in; nothing calls it (the linker strips it); it only places its literals in this object's constant pool. Removing it changes the object.
void mobjStrippedFallPool(MapObjectEntry* entry) {
    entry->velocity.y = 0.0f;
    entry->position.y = 1.0f;
    entry->position.x = 10.0f;
    entry->position.z = -10.0f;
    entry->velocity.x = -0.5f;
    if (__fabs(entry->velocity.y) <= 2.0) {
        entry->velocity.z = 0.0f;
    }
}

/** early demos only: the nearest active mobj within minMagnitude of (x, y, z) */
MapObjectEntry* mobjNearDistCheck(f32 x, f32 y, f32 z, f32 minMagnitude) {
    int entryCount;
    Vec pos;
    Vec dist;
    Vec _pos;
    MapObjectEntry* entry;
    MapObjectWork* wp = mobjGetWork();
    MapObjectEntry* ret;
    int i;
    float mag;

    entryCount = wp->count;
    entry = wp->entries;
    ret = NULL;

    _pos = (Vec){0.0f, 0.0f, 0.0f};
    _pos.x = x;
    _pos.y = y;
    _pos.z = z;
    pos = _pos;

    for (i = 0; i < entryCount; i++, entry++) {
        if ((entry->flags & MOBJ_FLAG_ACTIVE) == 0)
            continue;

        VECSubtract(&entry->position, &pos, &dist);
        mag = VECMag(&dist);
        if (mag < minMagnitude) {
            minMagnitude = mag;
            ret = entry;
        }
    }

    return ret;
}
#endif
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
#if VERSION >= VERSION_JP
    // FAKE: unusedVec is never read and the position goes through the zeroed copy _pos; setting pos.x/y/z directly changes the object
    unusedVec = (Vec){ 0.0f, 0.0f, 0.0f };
#endif
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

#if VERSION <= VERSION_DEMO_JP

/**
 * For a MOBJ_KururinFloor object returns 2 while its item is still there to find,
 * 1 once its item is out or its flag is set, and 0 for any other object. The early demos keep it as a real function.
 */
// A real function in the early demos, defined after its callers.
int mobjCheckKururingFloorItem(MapObjectEntry* entry) {
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
#endif

/** Name of the flipping floor panels that hide Star Pieces, checked to treat their hidden item specially. */
const char kururinName[] = "MOBJ_KururinFloor";

// FAKE: stand-in, never called (in neither symbol map; the linker strips it); it only places its literals in the .sdata2 pool
#if VERSION >= VERSION_JP
void mobjStrippedFallPool(MapObjectEntry* entry) {
    entry->velocity.y = 0.0f;
    entry->position.y = 10.0f;
    entry->position.x = 1.0f;
    entry->position.z = -1.0f;
    entry->velocity.x = -0.5f;
}
#endif
