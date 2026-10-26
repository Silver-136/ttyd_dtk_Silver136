#pragma once

#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <dolphin/os.h>

typedef struct DisplayList {
    u32 offset;      // 0x00 - Offset to vertex data
    u32 vertexCount; // 0x04 - Number of vertices
} DisplayList;

typedef struct PackedDisplayList {
    u32 offset;      // 0x00 - Offset to packed data
    u32 size;        // 0x04 - display list size in bytes (GXCallDisplayList)
} PackedDisplayList;

// "vcd_table" chunk arrays: an element count, then the elements
typedef struct MapVtxArray {
    u32 count;   // 0x0
    u8 data[1];  // 0x4, GXSetArray base
} MapVtxArray;

typedef struct MapVcdTable {
    MapVtxArray* positions;    // 0x0
    MapVtxArray* normals;      // 0x4
    u8 pad_08[0xC - 0x8];      // 0x8
    MapVtxArray* colors[2];    // 0xC
    s32 texCount;              // 0x14
    MapVtxArray* texCoords[8]; // 0x18
    u8 pad_38[0x44 - 0x38];    // 0x38
    s32 posFrac;               // 0x44, fraction bits of the packed s16 positions (GX vtx fmt / GQR5 in hitdrv)
    s32 texFrac[8];            // 0x48, fraction bits of the packed s16 tex coords
} MapVcdTable;

typedef struct MapVtxRec {
    s16 pos;     // 0x0
    s16 nrm;     // 0x2
    s16 clr;     // 0x4
    s16 clr1;    // 0x6
    s16 tex[8];  // 0x8
} MapVtxRec;

typedef struct MapRawDL {
    s32 count;        // 0x0, vertices in the strip
    MapVtxRec vtx[];  // 0x4
} MapRawDL;

typedef struct MapFileMesh {
    u8 reserved_00;              // 0x0, map file byte, not read
    u8 padding[2];        // 0x1
    u8 isPackedDisplay;   // 0x3, BOOL
    s32 displayListCount; // 0x4
    u32 elementMask;      // 0x8
    MapVcdTable* vcdTable; // 0xC
  union {
    MapRawDL* normalLists[1];          // unpacked: stride 4
    PackedDisplayList packedLists[1];  // packed:   stride 8, count = *(u16*)((u8*)packedLists[j].offset + 1)
} displayLists;  // at 0x10
} MapFileMesh;

typedef struct MapTexInfo {
    char* name;   // 0x0
    u8 format;    // 0x4
    u8 pad5[3];
    u16 width;    // 0x8
    u16 height;   // 0xA
    void* data;   // 0xC
} MapTexInfo;

typedef struct MapTexXform {
    union {
        struct {
            f32 tx;  // 0x0
            f32 ty;  // 0x4
            f32 sx;  // 0x8
            f32 sy;  // 0xC
            f32 rot; // 0x10
        };
        f32 ch[5];   // 0x0, tx..rot as the five material-animation channels
    };
    f32 cx;  // 0x14
    f32 cy;  // 0x18
} MapTexXform;

typedef struct MapMatSampler {
    MapTexInfo* tex; // 0x0
    u8 pad4[4];      // 0x4
    u8 wrapS;        // 0x8
    u8 wrapT;        // 0x9
    u8 blendMode;    // 0xA, as MapMaterial.blendMode
} MapMatSampler;

typedef struct MapMaterial {
    char* name;                  // 0x0
    GXColor color;               // 0x4
    u8 lightMode;                // 0x8
    u8 pad9;                     // 0x9
    u8 blendMode;                // 0xA, 0 opaque, 1 alpha-tested, 2 translucent (_mapEnt picks MapObject.blendMode)
    u8 samplerCount;             // 0xB
    MapMatSampler* samplers[8];  // 0xC
    MapTexXform xforms[8];       // 0x2C
    GXColor blendColor;          // 0x10C, last-stage blend colour (translucency test, mapSetMaterialLastStageBlend)
    u8* tevMode;                 // 0x110
} MapMaterial;

typedef struct MapFileJointPart {
    MapMaterial* material; // 0x0
    MapFileMesh* mesh; // 0x4
} MapFileJointPart;

typedef struct MapJointDrawMode {
    u8 blendType;            // 0x0, 0..3, selects MapObject.blendMode
    u8 cullMode;             // 0x1
    u8 modeFlags;            // 0x2, 1 = MapObject flag 8, 2 = group (flag 0x10), 8 = use lightMask, 0x10 = flag 0x8000
    u8 pad_03;               // 0x3, never accessed
    u32 lightMask;           // 0x4, bit i = map light i applies (with modeFlags 8)
    s32 hitAttributes;       // 0x8, copied to HitEntry.attributes
    u8 pad_0C[0x10 - 0xC];   // 0xC, never accessed
} MapJointDrawMode;

typedef struct MapFileJoint {
    const char* name;            // 0x0
    const char* type;            // 0x4
    struct MapFileJoint* parent; // 0x8
    struct MapFileJoint* child;  // 0xC
    struct MapFileJoint* next;   // 0x10
    struct MapFileJoint* prev;   // 0x14
    Vec scale;                   // 0x18
    Vec rotation;                // 0x24
    Vec translation;             // 0x30
    Vec bboxMin;                 // 0x3C
    Vec bboxMax;                 // 0x48
    u8 pad_54[0x58 - 0x54];  // 0x54, never accessed
    MapJointDrawMode* drawMode;  // 0x58
    s32 partCount;               // 0x5C
    MapFileJointPart parts[];    // 0x60+
} MapFileJoint;

typedef struct MapFileHeader {
    s32 fileSize;     // 0x0
    s32 dataSize;     // 0x4
    s32 relCount;     // 0x8
    s32 chunkCount;   // 0xC
    s32 unused;       // 0x10
    u8 reserved[0xC]; // 0x14
} MapFileHeader;

// entry in the chunk table
typedef struct MapFileChunk {
    u32 offset; // 0x0, relative offset into data
    u32 string; // 0x4, relative offset into strings
} MapFileChunk;

typedef struct MapFileInfo {
    const char* version; // 0x0
    MapFileJoint* joint; // 0x4
    const char* mapName; // 0x8
    const char* hitName; // 0xC
} MapFileInfo;

typedef struct MapHeader {
    MapFileHeader file;         // 0x0
    void* data;                 // 0x20
    u32* rel;                   // 0x24, relocation table: offsets into data of the pointers to rebase
    MapFileChunk* chunks;       // 0x28
    void* unused;               // 0x2C
    const char* strings;        // 0x30
    u8 pad_34[0x3C - 0x34]; // 0x34, never accessed
    s32 loadFlags;             // 0x3C, bit 1 set by mapLoadHeader_ before the header copy
    void* rawFile;          // 0x40, the map.dat buffer the header was read from (SPM MapFileContext.rawFile)
} MapHeader;

// MapObject.flags, named from mapdrv (draw passes, _mapEnt, the mapObj* setters) and envdrv. Bits 24-27 are not
// flags but the index of the TEV callback (MAP_OBJ_TEV_CALLBACK). 0x10000/0x20000/0x40000/0x800000/0x80000000 (only
// set on animation poses, which share the material flag layout) and 0x40000000 are not named.
typedef enum MapObjFlags {
    MAP_OBJ_FLAG_HIDDEN = (1 << 0),          // 0x1, not drawn by its own camera (offscreen pass still draws it)
    MAP_OBJ_FLAG_CLIP = (1 << 1),            // 0x2, view frustum cull (small bounding boxes, _mapEnt)
    MAP_OBJ_FLAG_CHAR_SHADOW = (1 << 2),     // 0x4, receives character shadows (draw mode flag)
    MAP_OBJ_FLAG_BACKGROUND = (1 << 3),      // 0x8, drawn first; draw modes 3/4 without Z test/write
    MAP_OBJ_FLAG_GROUP = (1 << 4),           // 0x10, group node (draw mode flag)
    MAP_OBJ_FLAG_GROUP_MEMBER = (1 << 5),    // 0x20, belongs to a group
    MAP_OBJ_FLAG_COLOR = (1 << 6),           // 0x40, mapObjSetColor
    MAP_OBJ_FLAG_OFFSCREEN = (1 << 7),       // 0x80, mapObjSetOffScreen
    MAP_OBJ_FLAG_DISPLAY_LIST = (1 << 8),    // 0x100, material baked into a display list
    MAP_OBJ_FLAG_MTX_DIRTY = (1 << 9),       // 0x200, matrix recalculated on the next update
    MAP_OBJ_FLAG_PROJ_TEX = (1 << 10),       // 0x400, projected texture
    MAP_OBJ_FLAG_NO_MAP_BLEND = (1 << 11),   // 0x800, ignores mapSetBlend
    MAP_OBJ_FLAG_NO_MAP_BLEND2 = (1 << 12),  // 0x1000, ignores mapSetBlend2
    MAP_OBJ_FLAG_ANIM_MTX = (1 << 13),       // 0x2000, joint animation matrix
    MAP_OBJ_FLAG_DISABLED = (1 << 14),       // 0x4000, skipped by every draw pass
    MAP_OBJ_FLAG_DEPTH_OFFSET = (1 << 15),   // 0x8000, projection depth bias by its place among its siblings
    MAP_OBJ_FLAG_NO_FOG = (1 << 19),         // 0x80000
    MAP_OBJ_FLAG_FLUSH = (1 << 20),          // 0x100000, mapObjFlushOn
    MAP_OBJ_FLAG_TRANSFORM = (1 << 21),      // 0x200000, mapObjRotate/Translate/Scale applied
    MAP_OBJ_FLAG_KURURING = (1 << 22),       // 0x400000, drawn by test_kururing_mapdisp
    MAP_OBJ_FLAG_NO_COLOR_STAGE = (1 << 28), // 0x10000000, no final colour-multiply TEV stage
    MAP_OBJ_FLAG_SCREEN_RECT = (1 << 29),    // 0x20000000, mapDispMapObj_bbox screen rectangle
} MapObjFlags;
#define MAP_OBJ_TEV_CALLBACK_MASK 0x0F000000
#define MAP_OBJ_TEV_CALLBACK(n) ((n) << 24)

typedef struct MapObject {
    u32 flags;                             // 0x0, MapObjFlags
    u8 blendMode;                          // 0x4
    u8 cameraId;                           // 0x5, dispEntry camera (4 by default; set by evt_lect_* / tik_00)
    u8 pad_06[0x8 - 0x6];                  // 0x6, never accessed
    MapFileJoint* joints;                  // 0x8
    GXColor color;                         // 0xC
    Vec center;                            // 0x10
    Mtx modelWorldMtx;                     // 0x1C
    Mtx localMtx;                          // 0x4C
    Mtx animMtx;                           // 0x7C, mapCalcAnimMatrix output, concatenated into modelWorldMtx
    Mtx transformMtx;                      // 0xAC, mapObjRotate/mapObjTrans/mapObjScale matrix (identity at load)
    s16 offscreenId;                       // 0xDC
    s16 groupIndex;                        // 0xDE
    struct MapObject* parent;              // 0xE0
    struct MapObject* child;               // 0xE4
    struct MapObject* next;                // 0xE8
    u16 screenRect[4];                     // 0xEC, projected bounds (0x3FF when off screen)
    void* displayList[8];                  // 0xF4, prebuilt display lists (makeDisplayList)
    u32 displayListSize[8];                // 0x114, their sizes; flag 0x100 (use display list) only sticks when [0] is nonzero
} MapObject;

typedef struct MapError {
    char name[32]; // 0x0, map object name that was not found
    s32 type;      // 0x20, mapErrorEntry type (0 for missing objects)
} MapError;

// "animation_table" entry in map.dat (SPM MapFileAnimData)
typedef struct MapAnimHeader {
    const char* name;                    // 0x0
    u8 pad_04[0x8 - 0x4];                // 0x4
    f32 length;                          // 0x8
    struct MapAnimList* jointAnims;      // 0xC, also read by hitMain as the joint list (count + joints)
    struct MapAnimList* materialAnims;   // 0x10
    struct MapAnimList* alphaAnims;      // 0x14
    struct MapAnimList* lightAnims;      // 0x18
    struct MapAnimList* lightColorAnims; // 0x1C
} MapAnimHeader;

// MapEntryAnimData.flags (map animations)
typedef enum MapAnimFlags {
    MAP_ANIM_FLAG_ACTIVE = (1 << 0),       // 0x1
    MAP_ANIM_FLAG_END = (1 << 1),          // 0x2
    MAP_ANIM_FLAG_LOOP = (1 << 2),         // 0x4
    MAP_ANIM_FLAG_PAUSE = (1 << 4),        // 0x10
    MAP_ANIM_FLAG_RESTART = (1 << 5),      // 0x20
    MAP_ANIM_FLAG_LV0_TIME = (1 << 8),     // 0x100, which clock it runs on
    MAP_ANIM_FLAG_LV1_TIME = (1 << 9),     // 0x200
    MAP_ANIM_FLAG_LV2_TIME = (1 << 10),    // 0x400
    MAP_ANIM_FLAG_LV3_TIME = (1 << 11),    // 0x800
    MAP_ANIM_FLAG_HIT_UPDATED = (1 << 12), // 0x1000, hitMain
} MapAnimFlags;

typedef struct MapEntryAnimData {
    u16 flags;                // 0x0, MapAnimFlags
    u8 pad_02[0x8 - 0x2];     // 0x2
    OSTime startTime;         // 0x8
    f32 time;                 // 0x10
    f32 rate;                 // 0x14
    MapAnimHeader* anim;      // 0x18
    u8 pad_1C[0x20 - 0x1C];   // 0x1C
} MapEntryAnimData;

// "light_table" chunk
typedef struct MapFileLight {
    const char* name;   // 0x0
    const char* type;   // 0x4
    Vec position;       // 0x8
    Vec rotation;       // 0x14
    u8 pad20[0x2C - 0x20];
    GXColor color;      // 0x2C
    f32 spotAngle;      // 0x30
    f32 attenAngle;     // 0x34
    s32 attenType;      // 0x38
    s32 lflags;         // 0x3C
    s32 magic;          // 0x40
} MapFileLight;



typedef struct MapFileLightTable {
    s32 count;              // 0x0
    MapFileLight* lights[]; // 0x4
} MapFileLightTable;

// "fog_table" chunk
typedef struct MapFileFog {
    s32 enable;     // 0x0
    s32 type;       // 0x4
    f32 start;      // 0x8
    f32 end;        // 0xC
    GXColor color;  // 0x10
} MapFileFog;

// "texture_table" chunk
typedef struct MapTexTable {
    s32 count;      // 0x0
    char* names[];  // 0x4
} MapTexTable;



// "material_name_table" chunk
typedef struct MapMaterialName {
    const char* name; // 0x0
    void* data;       // 0x4
} MapMaterialName;

typedef struct MapMaterialNameTable {
    s32 count;                  // 0x0
    MapMaterialName entries[];  // 0x4
} MapMaterialNameTable;

typedef struct MapEntry {
    union {
        s32 count;                 // 0x0, entries[0]: number of active entries
        struct ARAMEntry* prevTplAram; // 0x0, entries[1] / MapWork.lastTplAram: the previous entry's tpl ARAM copy (MAPENT_TPLARAM)
    };
    u16 flags;                     // 0x4
    char name[16];                 // 0x6
    GXColor blend;                 // 0x16
    GXColor blend2;                // 0x1A
    GXColor color;                 // 0x1E
    u8 pad_22[0x24 - 0x22];        // 0x22, never accessed
    s32 fogEnable;                 // 0x24, fog_table word 0
    s32 fogType;                   // 0x28
    f32 fogStart;                  // 0x2C
    f32 fogEnd;                    // 0x30
    GXColor fogColor;              // 0x34
    MapHeader header;              // 0x38
    void* dat;                     // 0x7C, ./map/%s/map.dat
    u32 datSize;                   // 0x80
    void* tpl;                     // 0x84, ./map/%s/texture.tpl
    u32 tplSize;                   // 0x88
    void* arcData;                 // 0x8C, buffer kept in ARAM (arcAram) when dat is an archive
    u32 arcSize;                   // 0x90
    MapTexTable* textureTable;     // 0x94, "texture_table" chunk
    MapFileLightTable* lightTable; // 0x98, "light_table" chunk
    MapFileFog* fogTable;          // 0x9C, "fog_table" chunk
    MapMaterialNameTable* materialNameTable; // 0xA0, "material_name_table" chunk
    MapVcdTable* vcdTable;         // 0xA4, "vcd_table" chunk
    MapObject* rootMapObj;         // 0xA8
    struct HitEntry* rootHitObj;     // 0xAC
    Vec bbox[8];                   // 0xB0
    void* tevCallbacks[16];        // 0x110
    s32 numJoints;                 // 0x150
    MapObject* objects;            // 0x154
    s32 hitNumJoints;              // 0x158
    struct HitEntry* hitEntries;     // 0x15C
    s32 numAnims;                  // 0x160
    MapEntryAnimData* animData;    // 0x164
    u32 dispListSize;              // 0x168, total display-list bytes (makeDisplayList)
    void* dispListBuf;             // 0x16C, _mapAllocTail(dispListSize); freed in bmapLoad
    struct ARAMEntry* datAram;     // 0x170, ARAM copy of dat (bmapLoad / bmapUnLoad)
    struct ARAMEntry* arcAram;     // 0x174, ARAM copy of arcData
    // the tpl ARAM copy has no slot of its own: it lives in the first word after the entry (MAPENT_TPLARAM)
} MapEntry;

typedef struct MapWork {
    MapEntry entries[2]; // 0x0
    struct ARAMEntry* lastTplAram; // 0x2F0, entries[1]'s tpl ARAM copy (MAPENT_TPLARAM(&entries[1]))
} MapWork;

// tpl ARAM copy of a map entry: stored in the word after it (the next entry's count slot, or lastTplAram)
#define MAPENT_TPLARAM(entry) ((entry) + 1)->prevTplAram

void mapErrorEntry(s32 type, const char* name);
const char* getMapDataDvdRoot(void);
void N_mapDispOff(void);
void N_mapDispOn(void);
MapWork* mapGetWork(void);
s32 mapGetActiveGroup(void);
s32 mapGetJoints(MapFileJoint* joint);
void mapInit(void);
MapEntryAnimData* mapSearchAnmObj(const char* animName);
void mapCheckAnimation(const char* animName, BOOL* valid, f32* remainingMs);
void mapUnLoad(void);
BOOL mapPreLoad(const char* map);
void mapLoad(const char* map);

void mapMain(void);
void mapDisp(void);

void mapSetMaterialLight(s32 flags, Vec position);
void mapSetMaterialLastStageBlend(s32 flags, GXColor color1, GXColor color2);
void mapSetMaterialFog(void);
typedef struct MapTevCallbackInfo {
    MapObject* mapObj; // 0x0, map object being drawn
    MtxPtr mtx;        // 0x4
    s32 drawMode;      // 0x8
    s32 tevStage;      // 0xC, next free TEV stage
    s32 texMap;        // 0x10, next free texture map
    s32 texCoord;      // 0x14, next free texture coordinate
    s32 indStage;      // 0x18, next free indirect TEV stage
} MapTevCallbackInfo;

typedef void (*MapTevCallback)(MapTevCallbackInfo* info);
void mapSetTevCallback(s32 index, MapTevCallback callback);
void mapSetMaterialTev(s32 texCount, s32 drawMode, s32 flags, Mtx mtx);

MapObject* mapGetMapObj(const char* name);

void mapGetBlend(GXColor* color);
void mapGetBlend2(GXColor* color);
BOOL mapTestXLU(u32 flags, GXColor* color, GXColor* matColor);

extern BOOL __mapdrv_make_dl; // mapdrv.c
void mapGrpFlagOn(const char* name, u32 flags);

extern const GXCullMode culling[4];
extern s32 fog_type[5];

void mapGrpFlagOff(const char* name, u32 flags);
void bmapUnLoad(void);
void mapBlendOff(void);
void mapBlendOff2(void);
void mapFogOff(void);
void mapFogOn(void);
void mapGrpClearOffScreen(const char* name);
void mapGrpFlushOff(const char* name);
void mapGrpFlushOn(const char* name);
void mapGrpSetFlushColor(const char* name, u8 r, u8 g, u8 b, u8 a);
void mapObjClearOffScreen(const char* name);
void mapObjFlagOff(const char* name, u32 flags);
void mapObjFlagOn(const char* name, u32 flags);
void mapObjFlushOff(const char* name);
void mapObjFlushOn(const char* name);
void mapObjGetFlushColor(const char* name, u8* r, u8* g, u8* b, u8* a);
void mapObjRotate(const char* name, f32 x, f32 y, f32 z);
void mapObjScale(const char* name, f32 x, f32 y, f32 z);
void mapObjSetFlushColor(const char* name, u8 r, u8 g, u8 b, u8 a);
void mapObjTranslate(const char* name, f32 x, f32 y, f32 z);
void mapPauseAnimation(const char* name);
void mapPauseAnimationAll(const char* name);
void mapPlayAnimationLv(const char* name, s32 mode, s32 level);
void mapReStartAnimation(const char* name);
void mapReStartAnimationAll(const char* name);
void mapSetPaperAmbColor(GXColor* color);
void mapResetPaperAmbColor(void);
void mapSetPlayRate(const char* name, f32 rate);
void bmapLoad(const char *map, const char *map2);
void mapObjGetPos(const char* name, Vec* pos);
void mapGrpSetColor(const char* name, GXColor color);
void mapSetFog(s32 type, f32 start, f32 end, GXColor color);
void mapGetFog(s32* type, f32* start, f32* end, GXColor* color);
void mapSetProjMtx(Mtx mtx);
void mapSetProjTexObj(GXTexObj* obj, void* imgRef);
