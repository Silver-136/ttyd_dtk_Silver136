// animdrv.c: the animation driver (AnimData, AnimPoseNode): paper animation files, keyframes and pose drawing.

#pragma once

#include "manager/filemgr.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <dolphin/os.h>

typedef struct AnimData AnimData;
typedef struct AnimPoseNode AnimPoseNode;
typedef struct AnimTexMtx AnimTexMtx;
typedef struct AnimTableEntry AnimTableEntry;

typedef struct AnimDrawCallEntry {
    s32 first; // 0x0
    s32 count; // 0x4
} AnimDrawCallEntry;

typedef struct AnimPoseShapeVertex {
    s32 first; // 0x0
    s32 count; // 0x4
} AnimPoseShapeVertex;

typedef struct AnimPoseShape {
    char name[64];                 // 0x0
    AnimPoseShapeVertex vtxPos;    // 0x40
    AnimPoseShapeVertex vtxNrm;    // 0x48
    AnimPoseShapeVertex vtxClr0;   // 0x50
    AnimPoseShapeVertex vtxTex[8]; // 0x58
    s32 shapeDrawStart;            // 0x98
    s32 shapeDrawCount;            // 0x9C
    s32 dispMode;                  // 0xA0
    s32 cullMode;                  // 0xA4
} AnimPoseShape;

typedef struct AnimPoseShapeDraw {
    s32 texCount;          // 0x0
    u8 pad_04[0x8 - 0x4];  // 0x4, file data, never read
    s32 drawMode;          // 0x8
    u8 pad_0C[0x10 - 0xC]; // 0xC, file data, never read
    s32 texDataIds[8];     // 0x10
    u8 texArrayIds[8];     // 0x30
    s32 firstDrawCall;     // 0x38
    s32 drawCallCount;     // 0x3C
    s32 firstIdPos;        // 0x40
    s32 firstIdNrm;        // 0x44
    s32 firstIdClr0;       // 0x48
    s32 firstIdTex[8];     // 0x4C
} AnimPoseShapeDraw;

// all of these fall under "AnimPoseData"
typedef struct AnimKeyframe {
    f32 time;        // 0x0
    s32 vtxPosStart; // 0x4, index into AnimData.vtxPosDeltas
    s32 vtxPosCount; // 0x8
    s32 vtxNrmStart; // 0xC
    s32 vtxNrmCount; // 0x10
    s32 texStart;    // 0x14
    s32 texCount;    // 0x18
    s32 visStart;    // 0x1C
    s32 visCount;    // 0x20
    s32 nodeStart;   // 0x24
    s32 nodeCount;   // 0x28
} AnimKeyframe;

typedef struct AnimVtxDelta {
    u8 step; // 0x0, index step in the vertex buffer
    s8 x;    // 0x1, in 1/16 units
    s8 y;    // 0x2
    s8 z;    // 0x3
} AnimVtxDelta;

typedef struct AnimVisDelta {
    u8 step; // 0x0, index step in the group visibility buffer
    u8 add;  // 0x1, added to the byte there
} AnimVisDelta;

typedef struct AnimNodeDelta {
    u8 step;         // 0x0, index step in the node buffer
    s8 value;        // 0x1, in 1/16 units
    s8 tangentStart; // 0x2, Hermite tangent as an angle in degrees (tanfTbl)
    s8 tangentEnd;   // 0x3, same; outside -88..88 the node keeps the base value between keyframes
} AnimNodeDelta;

typedef struct AnimTexDelta {
    u8 step;    // 0x0, AnimTexMtx index step
    u8 bindAdd; // 0x1, added to texBindIdx
    u8 pad_2[2];
    f32 transS; // 0x4
    f32 transT; // 0x8
} AnimTexDelta;

typedef struct AnimValueKey {
    f32 time;  // 0x0
    s32 value; // 0x4, copied into AnimationPose.curKeyValue
} AnimValueKey;

typedef struct AnimLoopInfo {
    s32 loop;  // 0x0, loop flag
    f32 start; // 0x4, first frame
    f32 end;   // 0x8, last frame
} AnimLoopInfo;

struct AnimData {
    u8 pad_00[0x8 - 0x0];      // 0x0, no named access
    s32 keyframeCount;         // 0x8
    u8 pad_0C[0x20 - 0xC];     // 0xC, no named access
    s32 valueKeyCount;         // 0x20
    AnimLoopInfo* loopInfo;    // 0x24
    AnimKeyframe* keyframes;   // 0x28
    AnimVtxDelta* vtxPosDeltas; // 0x2C
    AnimVtxDelta* vtxNrmDeltas; // 0x30
    AnimTexDelta* texDeltas;   // 0x34
    AnimVisDelta* visDeltas;   // 0x38
    AnimNodeDelta* nodeDeltas; // 0x3C
    AnimValueKey* valueKeys;   // 0x40
    Vec bboxMin;               // 0x44, paper-mode bounds (animPaperPoseDispSub)
    Vec bboxMax;               // 0x50
};

struct AnimTableEntry {
    char name[60];   // 0x0
    AnimData* data; // 0x3C
};

typedef struct AnimPoseTexEntry {
    s32 bindId;    // 0x0
    s32 wrapFlags; // 0x4
} AnimPoseTexEntry;

typedef struct AnimTexBind {
    u8 pad_00[0x4 - 0x0];     // 0x0, never accessed
    u32 texId;                // 0x4, TPL texture index
    s32 texType;              // 0x8
    u8 pad_0C[0x40 - 0xC]; // 0xC, never accessed
} AnimTexBind;

struct AnimTexMtx {
    u8 texBindIdx; // 0x0
    u8 align[3];    // 0x1, padding
    f32 transS;    // 0x4
    f32 transT;    // 0x8
    f32 scaleS;    // 0xC
    f32 scaleT;    // 0x10
    f32 rotate;    // 0x14
};

// end "AnimPoseData" substructs
struct AnimPoseNode {
    Vec translation;         // 0x0
    Vec mScale;               // 0xC
    Vec rot1;                // 0x18
    Vec rot2;                // 0x24
    Vec rotationCenter;      // 0x30
    Vec scaleCenter;         // 0x3C
    Vec rotationTranslation; // 0x48
    Vec scaleTranslation;    // 0x54
};

typedef struct AnimPoseGroup {
    char name[64];      // 0x0
    s32 brotherGroupId; // 0x40
    s32 childGroupId;   // 0x44
    s32 shapeId;        // 0x48
    s32 visibilityId;   // 0x4C
    s32 nodeId;         // 0x50
    BOOL useSSC;        // 0x54, Segment Scale Compensation
} AnimPoseGroup;

// this is the structure of the actual stored files
typedef struct AnimationPoseData {
    u32 size;                     // 0x0
    char fileName[64];             // 0x4
    char textureName[64];          // 0x44
    char buildTime[64];           // 0x84
    u32 flags;                     // 0xC4
    s32 radius;                    // 0xC8
    s32 height;                    // 0xCC
    Vec bboxMin;                   // 0xD0
    Vec bboxMax;                   // 0xDC
    s32 shapeCount;                // 0xE8
    u8 pad_EC[0xF0 - 0xEC];    // 0xEC, no access found
    u32 bufferPosNum;             // 0xF0
    u32 bufferPosIdxNum;          // 0xF4
    u32 bufferNrmNum;             // 0xF8
    u32 bufferNrmIdxNum;          // 0xFC
    u8 pad_100[0x12C - 0x100]; // 0x100, no access found
    u32 bufferTexAnimNum;         // 0x12C
    s32 texDataCount;              // 0x130
    s32 texBindCount;              // 0x134
    u8 pad_138[0x13C - 0x138]; // 0x138, no access found
    u32 bufferVisibilityNum;      // 0x13C
    u32 bufferNodeNum;            // 0x140
    s32 groupCount;                // 0x144
    s32 animCount;                // 0x148
    AnimPoseShape* shapes;         // 0x14C
    AnimDrawCallEntry* drawCalls;  // 0x150, finished name
    Vec* bufferVtxPos;             // 0x154
    u32* posIndex;                 // 0x158, 32-bit index entries; GX draws use the low 16 bits
    Vec* nrmVtxBuffer;             // 0x15C
    u32* nrmIndex;                 // 0x160, same entries as posIndex
    GXColor* clr0VtxBuffer;        // 0x164, finished name
    u32* clr0Index;                // 0x168, finished name, same entries as posIndex
    u32* texIndex[8];              // 0x16C, finished name, same entries as posIndex
    f32* texVtxBuffer;             // 0x18C, finished name, pointer to list of [S, T]
    AnimTexMtx* texAnimEntries;    // 0x190, copied into the pose buffer each keyframe reset
    AnimPoseTexEntry* texData;     // 0x194
    AnimTexBind* texBinds;         // 0x198
    AnimPoseShapeDraw* draws;      // 0x19C
    s8* groupVisibility;           // 0x1A0, copied into the pose buffer each keyframe reset
    f32* nodeData;        // 0x1A4, copied into the pose buffer each keyframe reset
    AnimPoseGroup* groups;         // 0x1A8
    AnimTableEntry* anims;       // 0x1AC
} AnimationPoseData;

// AnimationPose.flags, named from their setters and drawing consumers. The kururing capture in evt_mobj
// sets/clears ANIM_POSE_FLAG_CHANGE_TEXTURE (0x10), which materialProc reads.
typedef enum AnimationPoseFlags {
    ANIM_POSE_FLAG_ACTIVE = (1 << 0),               // 0x1, slot in use (animPoseEntry and the paper/effect entries)
    ANIM_POSE_FLAG_PAPER_GROUP = (1 << 1),          // 0x2, animPoseSetPaperAnimGroup
    ANIM_POSE_FLAG_PAPER_ANIM_CHANGED = (1 << 2),   // 0x4, animPoseSetAnim on a paper pose; consumed by the paper draw
    ANIM_POSE_FLAG_PERA = (1 << 3),                 // 0x8, animPosePaperPeraOn (paper flip)
    ANIM_POSE_FLAG_CHANGE_TEXTURE = (1 << 4),       // 0x10, materials drawn with texture slot 0 instead (animSetMaterial_ChangeTexture; set by the kururing capture)
    ANIM_POSE_FLAG_VIVIAN = (1 << 5),               // 0x20, animPoseVivianMain
    ANIM_POSE_FLAG_WORLD_POSITION_EVAL = (1 << 6),  // 0x40, animPoseWorldPositionEvalOn
    ANIM_POSE_FLAG_WORLD_MATRIX_EVAL = (1 << 7),    // 0x80, animPoseWorldMatrixEvalOn
    ANIM_POSE_FLAG_INSTANT_TURN = (1 << 8),         // 0x100, paper rotation snaps to the target (NPC_FLAG_INSTANT_TURN)
} AnimationPoseFlags;

// AnimationPose.typeFlags (0 for a plain animPoseEntry)
typedef enum AnimPoseTypeFlags {
    ANIM_POSE_TYPE_SUB = (1 << 0),    // 0x1, entered as a paper or effect pose (reference counted)
    ANIM_POSE_TYPE_PAPER = (1 << 1),  // 0x2, paper pose (animPaperPoseEntry): own vertex scratch, own local time
} AnimPoseTypeFlags;

// AnimationPose.materialLightFlag / mapSetMaterialLight flags: the lights used for the material
typedef enum MaterialLightFlags {
    MATERIAL_LIGHT_PAPER = (1 << 0),        // 0x1, the paper light, with the paper ambient
    MATERIAL_LIGHT_CHARA = (1 << 1),        // 0x2, the nearest character lights (LIGHT_CHARA)
    MATERIAL_LIGHT_MAT_COLOR = (1 << 2),    // 0x4, material colour from the register, not the vertices
    MATERIAL_LIGHT_PAPER_CRAFT = (1 << 3),  // 0x8, the papercraft light, with its ambient
    MATERIAL_LIGHT_PAPER_PLAIN = (1 << 4),  // 0x10, the paper light without the paper ambient
} MaterialLightFlags;

typedef void (*AnimPoseDispCallback)(void* userdata, s32 group, Mtx* modeling);

typedef struct AnimationPose {
    u32 flags;                          // 0x0, AnimationPoseFlags
    u32 typeFlags;                      // 0x4, AnimPoseTypeFlags
    s32 references;                     // 0x8
    s32 type;                           // 0xC
    s32 fileId;                         // 0x10
    s32 animId;                         // 0x14, current animation from AnimPoseData
    OSTime localTime;                   // 0x18
    f32 rawFrame;                       // 0x20, elapsed frames before loop wrap (SPM frmCounter)
    f32 localFrame;                     // 0x24
    s32 keyframeIdx;                    // 0x28, current AnimKeyframe index (also the renderProc frame)
    s32 nextKeyframeIdx;                // 0x2C, next AnimKeyframe index
    f32 keyframeBlend;                  // 0x30, t between keyframeIdx and nextKeyframeIdx
    s32 valueKeyIdx;                    // 0x34, current AnimValueKey index
    s32 nextValueKeyIdx;                // 0x38, next AnimValueKey index
    s32 lastFrameZero;                  // 0x3C
    f32 lastFrameTime;                  // 0x40
    f32 localTimeRate;                  // 0x44
    Vec* bufferVtxPos;                // 0x48
    Vec* vtxArrayPos;                 // 0x4C
    Vec* bufferVtxNrm;                // 0x50
    Vec* vtxArrayNrm;                 // 0x54
    s8* bufferGroupVisibility;        // 0x58
    s8* groupVisibility;              // 0x5C
    f32* bufferNode;         // 0x60
    f32* nodes;              // 0x64
    AnimTexMtx* bufferTexAnimEntries; // 0x68
    AnimTexMtx* texAnimEntries;       // 0x6C
    f32 peraRotY;                       // 0x70, base Y rotation for the paper turn (flags & 8)
    f32 rotationY;                      // 0x74
    f32 scaleZFlip;                     // 0x78, -1.0f mirrors Z (saved in NpcFieldEntry.scaleZFlip)
    f32 peraTargetRotY;                 // 0x7C, rotationY turns toward this; animPoseGetPeraEnd checks it
    s32 curKeyValue;                    // 0x80, AnimValueKey.value of valueKeyIdx; not read in C
    f32 loopTime;                       // 0x84
    OSTime localTimeInit;              // 0x88
    s32 paperId;                        // 0x90
    s32 paperAnimIdx;                  // 0x94, paper anim index from animPoseGetAnimIdx, -1 = none (not a string; passed to animPoseSetAnim as its name arg)
    struct smartEntry* paperCapture0;   // 0x98, smartEntry of the paper capture texture
    struct smartEntry* paperCapture1;   // 0x9C, second paper capture (never allocated in C)
    GXTexObj paperTexObj[2];            // 0xA0, texture objects for paperCapture0 / paperCapture1
    void (*gxCallback)(s32 wXluStage);  // 0xE0
    u32 disableDraw;                   // 0xE4
    s32 materialFlag;                   // 0xE8
    s32 materialLightFlag;              // 0xEC, MaterialLightFlags
    GXColor materialColor;              // 0xF0
    GXColor materialColor2;             // 0xF4
    f32 paperScaleXZ;                   // 0xF8, paper-mode draw scale
    f32 paperScaleY;                    // 0xFC
    f32 paperOfsY;                      // 0x100, paper-mode Y offset (doubled in battle)
    f32 paperTexOfsX;                   // 0x104, paper texture matrix translate x
    f32 paperTexOfsY;                   // 0x108, paper texture matrix translate y
    f32 paperTexScaleX;                 // 0x10C, paper texture matrix scale x
    f32 paperTexScaleY;                 // 0x110, paper texture matrix scale y
    s16 paperRectLeft;                  // 0x114, paper capture rect, screen x
    s16 paperRectRight;                 // 0x116, paper capture rect, screen x
    s16 paperRectTop;                   // 0x118, paper capture rect, screen y
    s16 paperRectBottom;                // 0x11A, paper capture rect, screen y
    s32 vivianType;                     // 0x11C
    s32* vivianGroupNo;                 // 0x120
    Mtx matrix;                         // 0x124
    AnimPoseDispCallback dispCallback;                 // 0x154
    void* dispUserDataCallback;         // 0x158
    smartEntry* worldPositions;               // 0x15C, filled while animPoseWorldPositionEvalOn is set
    s32 numWorldPositions;              // 0x160
    smartEntry* worldMatrices;                // 0x164, filled while animPoseWorldMatrixEvalOn is set
    s32 numWorldMatrices;               // 0x168
    u8 pad_16C[0x170 - 0x16C];          // 0x16C, never accessed
} AnimationPose;

typedef struct AnimationPoseFile {
    u32 active;        // 0x0, in use or not, unsigned
    s32 references;    // 0x4, reference count
    FileEntry* handle; // 0x8
    s32 textureId;     // 0xC
} AnimationPoseFile;

typedef struct AnimationTextureFile {
    BOOL active;       // 0x0
    s32 references;    // 0x4
    FileEntry* handle; // 0x8
} AnimationTextureFile;

typedef struct AnimationWork {
    AnimationPoseFile* files;         // 0x0
    s32 totalFiles;                   // 0x4
    AnimationTextureFile* textures;   // 0x8
    s32 totalTextures;                // 0xC
    AnimationPose* poses;             // 0x10
    s32 totalPoses;                   // 0x14
    AnimationPose* curPose;         // 0x18
    AnimData* curAnimData;          // 0x1C
    AnimationPoseData* curPoseData; // 0x20
    void* texDatas[2];              // 0x24
    void* curTexData;               // 0x2C
    GXTexObj* texObjs[2];           // 0x30
    GXTexObj* curTexObj;            // 0x38
    Mtx paperMtx[3];                  // 0x3C
    Mtx* paperMtxPtr[3];              // 0xCC
    s32 paperTexMode0;                   // 0xD8, animSetPaperTexObj texMode (both fields get it); == 0 tested with texChange1 in the paper draw path
    s32 paperTexMode1;                   // 0xDC, animSetPaperTexObj texMode; == 0 tested with check2 in the paper draw path
    s32 currDispMode;                 // 0xE0
    u8 pad_E4[0xE8 - 0xE4];       // 0xE4, never accessed
    s32 mbUseFloatScratch;            // 0xE8
    Vec* floatScratch;              // 0xEC
    s32 floatScratchRP;              // 0xF0
    s32 floatScratchWP;              // 0xF4
    Vec* vtxArrayPos;                 // 0xF8
    Vec* vtxArrayNrm;                 // 0xFC
    FileEntry* mp_ag2tg;              // 0x100
    void* testHeap;                   // 0x104
    u8* testAlloc;                    // 0x108
    BOOL inBattle;                    // 0x10C
} AnimationWork;

AnimationWork* animGetPtr(void);
OSTime animTimeGetTime(BOOL globalTime);
void animInit(void);
void animMain(void);
void animPoseBattleInit(void);

s32 animPoseEntry(const char* animName, s32 type);
s32 animPaperPoseEntry(const char* animName, s32 type);

BOOL animEffectAsync(const char* animName, u32 group);

void animPosePeraOn(s32 poseId);
void animPosePeraOff(s32 poseId);

void animPosePaperPeraOn(s32 poseId);
void animPosePaperPeraOff(s32 poseId);

void animPoseSetLocalTimeRate(s32 poseId, f32 localTimeRate);
void animPoseSetLocalTime(s32 poseId, f32 localTimeFrames);
void animPoseSetStartTime(s32 poseId, OSTime startTime);

void animPoseSetAnim(s32 poseId, const char* animName, BOOL reset);

s32 animPaperPoseGetId(const char* animName, s32 group);
void animPoseSetPaperAnimGroup(s32 poseId, const char* animName, BOOL paperGroup);
void animPoseSetPaperAnim(s32 poseId, const char* animName, BOOL reset);
void animPoseSetEffect(s32 poseId, const char* animName, BOOL paperGroup);
void animPoseSetEffectAnim(s32 poseId, const char* animName, BOOL reset);

AnimTableEntry* animPoseGetCurrentAnim(s32 poseId);
AnimationPoseData* animPoseGetAnimBaseDataPtr(s32 poseId);
AnimData* animPoseGetAnimDataPtr(s32 poseId);
AnimationPose* animPoseGetAnimPosePtr(s32 poseId);

void animPaperPoseRelease(s32 poseId);
void animPoseRelease(s32 poseId);

GXColor animPoseGetMaterialEvtColor(s32 poseId);
u32 animPoseGetMaterialLightFlag(s32 poseId);
u32 animPoseGetMaterialFlag(s32 poseId);
void animPoseSetMaterialEvtColor(s32 poseId, GXColor color);
void animPoseSetMaterialLightFlagOff(s32 poseId, u32 mask);
void animPoseSetMaterialLightFlagOn(s32 poseId, u32 mask);
void animPoseSetMaterialFlagOff(s32 poseId, u32 mask);
void animPoseSetMaterialFlagOn(s32 poseId, u32 mask);
BOOL animPoseGetPeraEnd(s32 poseId);

f32 animPoseGetRadius(s32 poseId);
f32 animPoseGetHeight(s32 poseId);

s32 animPoseGetVivianType(s32 poseId);

f32 animPoseGetLoopTimes(s32 id);

void animPoseWorldPositionEvalOn(s32 poseId);
void animPoseWorldMatrixEvalOn(s32 poseId);
void animPoseMain(s32 poseId);

BOOL animGroupBaseAsync(const char* name, s32 group, void* callback);

void animPoseSetGXFunc(s32 poseId, void (*callback)(s32 wXluStage), BOOL disableDraw);

s32 animPoseGetShapeIdx(s32 poseId, const char* name);
s32 animPoseGetGroupIdx(s32 poseId, const char* name);
const char* animPoseGetGroupName(s32 poseId, s32 group);

void animSetPaperTexMtx(Mtx mtx1, Mtx mtx2, Mtx mtx3);
void animSetPaperTexObj(GXTexObj* obj1, GXTexObj* obj2, GXTexObj* obj3, void* data1, void* data2, void* data3, s32 texMode, s32 unused);
void animPoseDraw(s32 poseId, f32 x, f32 y, f32 z, f32 rotY, f32 scale, s32 xluMode);

void animPoseDrawMtx(s32 poseId, Mtx matrix, s32 xluMode, f32 rotY, f32 scale);

extern u32 anim_ext_tex_size;

// Smorg (moamoa) body: pairs of joint groups, each segment linked to the one before it
typedef struct MoaChain {
    const char* parent; // joint group the segment hangs from
    const char* child;
} MoaChain;
extern const s32 g_moamoa_chain_num;
extern MoaChain g_moamoa_chain_tbl[47];

void animPoseAutoRelease(s32 type);
void animPoseWorldPositionEvalOff(s32 poseId);
void animPoseDrawShape(s32 poseId, s32 shapeId);
BOOL animPoseTestXLU(s32 poseId);
void animPoseVivianMain(s32 poseId, Vec* pos);

void animPoseSetDispCallBack(s32 poseId, AnimPoseDispCallback callback, void* userData);

// Names from LLCOOLT's tree.
#define ANIM_MATERIAL_FLAG_NO_BLEND 0x800
#define ANIM_MATERIAL_FLAG_NO_BLEND2 0x1000
#define ANIM_MATERIAL_FLAG_PROJ_TEXTURE 0x400
