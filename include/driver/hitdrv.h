#pragma once

#include <dolphin/mtx.h>
#include "driver/mapdrv.h"

typedef struct HitCheckQuery HitCheckQuery;
typedef struct HitEntry HitEntry;
typedef BOOL (*HitFilterCallback)(HitCheckQuery* query, struct HitEntry* entry);

typedef struct HitVector {
    Vec v0;     // 0x0
    Vec v1;     // 0xC
    Vec v2;     // 0x18
    Vec edge0;  // 0x24, v2 - v0
    Vec edge1;  // 0x30, v0 - v1
    Vec edge2;  // 0x3C, v1 - v2
    Vec normal; // 0x48
} HitVector;

struct HitCheckQuery {
    BOOL singleSided;   // 0x0
    s32 user0;          // 0x4
    s32 user1;          // 0x8
    Vec targetPos;      // 0xC
    Vec targetDir;      // 0x18
    Vec hitPos;         // 0x24
    Vec hitNormal;      // 0x30
    f32 targetDistance; // 0x3C
};

typedef struct HitDamageReturn {
    const char* mapobjName; // 0x0
    Vec position;           // 0x4
} HitDamageReturn;

struct HitEntry {
    u16 flags;                      // 0x0
    u8 pad_02[0x4 - 0x2];             // 0x2, never accessed
    s32 attributes;                 // 0x4
    MapFileJoint* joint;            // 0x8
    Mtx worldMtx;                   // 0xC
    Mtx localMtx;                   // 0x3C
    Mtx animMtx;                    // 0x6C, joint animation matrix (output of mapCalcAnimMatrix, concatenated into worldMtx)
    Vec centerLocal;                // 0x9C
    s16 vectorCount;                // 0xA8, number of HitVector triangles (set from hitCalcVtxPosition)
    s16 mapIndex;                   // 0xAA
    HitVector* vectors;             // 0xAC
    HitDamageReturn* damage;        // 0xB0
    u8 pad_B4[0xC0 - 0xB4];          // 0xB4, never accessed
    Vec centerWorld;                // 0xC0
    f32 radius;                     // 0xCC
    union {                         // 0xD0, owner: map object, or the mobj for mobj hit objects
        MapObject* mapObj;
        struct MapObjectEntry* mobj;
    };
    struct HitEntry* parent;        // 0xD4
    struct HitEntry* child;         // 0xD8
    struct HitEntry* sibling;       // 0xDC
    struct HitEntry* siblingActive; // 0xE0
};

void hitInit(void);
void hitMain(void);

void hitReCalcMatrix(HitEntry* hit, Mtx mtx);
HitEntry* hitCheckVecFilter(HitCheckQuery* query, HitFilterCallback callback);
HitEntry* hitCheckFilter(HitFilterCallback callback, f32* hitX, f32* hitY, f32* hitZ, f32* distance, f32* hitNX,
                         f32* hitNY, f32* hitNZ, f32 x, f32 y, f32 z, f32 nx, f32 ny, f32 nz);
BOOL hitCheckVecHitObjXZ(HitCheckQuery* query, HitEntry* entry);
HitEntry* hitCheckAttr(s32 user0, f32* hitX, f32* hitY, f32* hitZ, f32* distance, f32* hitNX, f32* hitNY, f32* hitNZ,
                       f32 x, f32 y, f32 z, f32 nx, f32 ny, f32 nz);
HitEntry* hitCheckSphereFilter(HitFilterCallback callback, f32 x, f32 y, f32 z, f32 distance);
HitEntry* hitNameToPtr(const char* name);
void hitObjGetPos(const char* name, Vec* position);
void hitObjGetNormal(const char* name, Vec* normal);
const char* hitGetName(HitEntry* hit);
s32 hitGetAttr(HitEntry* hit);
void hitGrpDamageReturnSet(const char* name, HitDamageReturn* damage);
Vec* hitGetDamageReturnPos(HitEntry* hit);
void hitBindMapObj(const char* hitName, const char* mapobjName);

void hitDelete(const char* name);
void hitBindUpdate(const char* name);
void hitObjFlagOff(const char* name, u16 mask);
void hitReInit(void);
void hitGrpAttrOff(const char* name, u32 mask);
void hitGrpAttrOn(const char* name, u32 mask);
void hitGrpFlagOff(const char* name, u16 mask);
void hitGrpFlagOn(const char* name, u16 mask);
void hitObjAttrOff(const char* name, u32 mask);
void hitObjAttrOn(const char* name, u32 mask);
void hitObjFlagOn(const char* name, u16 mask);
HitEntry* hitEntry(MapFileJoint* root, Mtx rootMtx, s32 mapIndex);
HitEntry* hitEntryMOBJ(MapFileJoint* joint, Mtx mtx);
