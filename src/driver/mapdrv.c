// Map driver: loads map.dat (joints, materials, animations), builds the display lists, and animates,
// culls and draws the map objects.

#include "version.h"
#include "driver/mapdrv.h"
#include "driver/animdrv.h"
#include "driver/arcdrv.h"
#include "manager/dvdmgr.h"
#include "manager/arammgr.h"
#include "mario/mariost.h"
#include "memory.h"
#include "system.h"
#include "texPalette.h"
#include "driver/offscreendrv.h"
#include "driver/camdrv.h"
#include "driver/lightdrv.h"
#include "driver/shadowdrv.h"
#include "gxsub.h"
#include <math.h>
#include "driver/envdrv.h"
#include "libc_decls.h"
#include "driver/hitdrv.h"
#include <string.h>
#include "driver/dispdrv.h"
static void makeDisplayList(BOOL group);
static MapObject* _mapEnt(MapFileJoint* joint, MapObject* parent, Mtx mtx, s32 index);


#define TWO_PI 6.28318530717958647692528676655900576839433879875021f

typedef MapEntryAnimData MapAnim;

//.bss
MapError error_data[32];

//.sbss
GXColor paper_ambient;
GXColor paperCraft_ambient;
GXTexObj projTexObj;
void* projTexImgRef;
Mtx projMtx;
BOOL __mapdrv_make_dl;

#if VERSION >= VERSION_JP
BOOL disableMapDraw;
#endif

s32 error_count;
BOOL error_flag;
MapObject* current_mp;
BOOL mapClipOffFlag;
s32 activeGroup;
MapWork* mapWork; //allocates two entries


// Implementation forward declarations; definitions retain target order.
MapFileJoint* mapSearchDmdJoint(MapFileJoint* joint, const char* name);
static void mapSetMaterial(MapObject* obj, void* material);
static void mapSetPolygon(MapObject* obj, MapFileMesh* mesh);
static void mapSetPolygonVtxDesc(MapObject* obj, MapFileMesh* mesh);

/** Single-precision sine for this file, routed through the double sin() of math.h. */
static inline f32 sinf(f32 x) { return sin(x); }

/** Frames per second of the map animation clock. PAL keeps it at 60 even when the display runs at 50 Hz. */
static inline s32 mapAnimFps(void) {
#if VERSION >= VERSION_DEMO_EU
    return 60;
#else
    return gp->framerate;
#endif
}

/**
 * Records a missing map object or hit object name (type 0 map object, 1 hit object) in the 32-entry error ring
 * buffer error_data and sets error_flag; called by hitdrv, evt_door,
 * evt_bero and evt_movefloor when a name lookup fails.
 */
void mapErrorEntry(s32 type, const char* name) {
    MapError* error = error_data;

    error += error_count;
    error->type = type;
    strncpy(error->name, name, sizeof(error->name) - 1);
    error_flag = TRUE;
    error_count++;
    error_count %= 32;
}

/** Root directory of the map data on the disc, always "." (cam_road.c builds its file path from it). */
const char* getMapDataDvdRoot(void) {
    return ".";
}

// Descriptive name (not in the symbol maps).
#if VERSION >= VERSION_JP
/**
 * Stops mapDisp from drawing the map until the next map load or mapDispOn;
 * used by evt_map, win_root (the pause menu) and tik_05. Not in the early demos.
 */
void mapDispOff(void) {
    disableMapDraw = TRUE;
}
#endif

// Descriptive name (not in the symbol maps).
#if VERSION >= VERSION_JP
/** Lets mapDisp draw the map again after mapDispOff. */
void mapDispOn(void) {
    disableMapDraw = FALSE;
}
#endif

/** The MapWork of the active group: index 0 is the field map, 1 the battle stage. */
MapWork* mapGetWork(void) {
    return &mapWork[activeGroup];
}

/** Which MapWork is active (0 field, 1 battle); lightdrv picks its light set from it. */
s32 mapGetActiveGroup(void) {
    return activeGroup;
}

/** Recursive helper of mapGetJoints: counts this joint, its children and its later siblings into *numJoints. */
static void mapGetJointsSub(MapFileJoint* joint, s32* numJoints) {
    (*numJoints)++;
    if (joint->child) {
        mapGetJointsSub(joint->child, numJoints);
    }
    if (joint->next) {
        mapGetJointsSub(joint->next, numJoints);
    }
}

s32 mapGetJoints(MapFileJoint* joint) {
    s32 numJoints;

    numJoints = 0;
    if (joint) {
        numJoints = 1;
        if (joint->child) {
            mapGetJointsSub(joint->child, &numJoints);
        }
    }
    return numJoints;
}

/**
 * World-space bounding box of the first map entry of the active group,
 * from the bbox corners of every joint that has geometry;
 * the box always includes the origin since min and max start at 0.
 */
void mapGetBoundingBox(Vec* min, Vec* max) {
    MapWork* wp = &mapWork[activeGroup];
    MapObject* obj;
    int i;
    Vec vec;

    *min = (Vec){0.0f, 0.0f, 0.0f};
    *max = (Vec){0.0f, 0.0f, 0.0f};
    obj = wp->entries[0].objects;
    for (i = 0; i < wp->entries[0].numJoints; i++, obj++) {
        if (obj->joints && obj->joints->partCount) {
            MTXMultVec(obj->modelWorldMtx, &obj->joints->bboxMin, &vec);
            if (min->x > vec.x) min->x = vec.x;
            if (min->y > vec.y) min->y = vec.y;
            if (min->z > vec.z) min->z = vec.z;
            if (max->x < vec.x) max->x = vec.x;
            if (max->y < vec.y) max->y = vec.y;
            if (max->z < vec.z) max->z = vec.z;
            MTXMultVec(obj->modelWorldMtx, &obj->joints->bboxMax, &vec);
            if (min->x > vec.x) min->x = vec.x;
            if (min->y > vec.y) min->y = vec.y;
            if (min->z > vec.z) min->z = vec.z;
            if (max->x < vec.x) max->x = vec.x;
            if (max->y < vec.y) max->y = vec.y;
            if (max->z < vec.z) max->z = vec.z;
        }
    }
}

/** Allocates and clears the two MapWork slots (field and battle) at boot; called once from mariost.c. */
void mapInit(void) {
    mapWork = __memAlloc(HEAP_DEFAULT, sizeof(MapWork) * 2);
    memset(mapWork, 0, sizeof(MapWork) * 2);
    activeGroup = 0;
    mapClipOffFlag = FALSE;
}

/** Index of the texture called name in the map's texture_table chunk, or -1. */
// Original name from the JP demo's link map (UNUSED there): every call is inlined, so the linker strips it.
s32 mapGetTextureIdx(MapTexTable* table, char* name) {
    s32 i;

    for (i = 0; i < table->count; i++) {
        if (strcmp(table->names[i], name) == 0) {
            return i;
        }
    }
    return -1;
}

/**
 * Points every material sampler of obj and its subtree at its image in the map's texture.tpl (matched by name,
 * falling back to image 0) and converts the TPL format code into the map's own texture format numbering.
 */
static void mapBuildTexture(MapObject* obj, TEXPalette* texture, void* table) {
    int i;
    int j;
    int k;
    int idx;
    MapMaterial* mat;
    MapMatSampler* sampler;
    TEXDescriptor* desc;

    for (i = 0; i < obj->joints->partCount; i++) {
        mat = obj->joints->parts[i].material;
        for (j = 0; j < mat->samplerCount; j++) {
            sampler = mat->samplers[j];
            k = mapGetTextureIdx(table, sampler->tex->name);
            idx = k;
            if (k == -1) {
                idx = 0;
            }
            desc = TEXGet(texture, idx);
            sampler->tex->data = desc->textureHeader->data;
            sampler->tex->width = desc->textureHeader->width;
            sampler->tex->height = desc->textureHeader->height;
            switch (desc->textureHeader->format) {
                case 4: sampler->tex->format = 0; break;
                case 5: sampler->tex->format = 1; break;
                case 6: sampler->tex->format = 2; break;
                case 14: sampler->tex->format = 3; break;
                case 0: sampler->tex->format = 4; break;
                case 1: sampler->tex->format = 5; break;
                case 2: sampler->tex->format = 6; break;
                case 3: sampler->tex->format = 7; break;
                case 8: sampler->tex->format = 8; break;
                case 9: sampler->tex->format = 9; break;
                case 10: sampler->tex->format = 10; break;
            }
        }
    }
    if (obj->child) {
        mapBuildTexture(obj->child, texture, table);
    }
    if (obj->next) {
        mapBuildTexture(obj->next, texture, table);
    }
}

/**
 * Takes the next free MapObject of map entry index and fills it from joint:
 * local and world matrices (rotation in degrees), blend mode from the draw mode and the materials,
 * group and shadow flags, centre, and the bounding-box cull flag
 * when its centre-to-corner distance is under about 141 units. Panics on a joint without a draw mode.
 */
static MapObject* _mapEnt(MapFileJoint* joint, MapObject* parent, Mtx mtx, s32 index) {
    MapWork* wp = &mapWork[activeGroup];
    MapObject* obj;
    s32 i;
    Mtx trans, scale, rotX, rotY, rotZ;
    Vec a, b;

    obj = wp->entries[index].objects;
    for (i = 0; i < wp->entries[index].numJoints; i++, obj++) {
        if (!obj->joints) break;
    }
    obj->flags = 0;
    obj->joints = joint;
    obj->groupIndex = (s16)index;
    PSMTXTrans(trans, joint->translation.x, joint->translation.y, joint->translation.z);
    PSMTXScale(scale, joint->scale.x, joint->scale.y, joint->scale.z);
    PSMTXRotRad(rotX, 'x', 0.017453292f * joint->rotation.x);
    PSMTXRotRad(rotY, 'y', 0.017453292f * joint->rotation.y);
    PSMTXRotRad(rotZ, 'z', 0.017453292f * joint->rotation.z);
    PSMTXConcat(trans, rotZ, trans);
    PSMTXConcat(trans, rotY, trans);
    PSMTXConcat(trans, rotX, trans);
    PSMTXConcat(trans, scale, obj->localMtx);
    PSMTXConcat(mtx, obj->localMtx, obj->modelWorldMtx);
    PSMTXIdentity(obj->transformMtx);
    obj->flags |= MAP_OBJ_FLAG_MTX_DIRTY;
    obj->cameraId = CAMERA_3D;
    obj->offscreenId = -1;
    obj->parent = parent;
    obj->next = NULL;
    obj->child = NULL;
    if (joint->drawMode) {
        if (joint->drawMode->modeFlags & DRAW_MODE_BACKGROUND) {
            obj->flags |= MAP_OBJ_FLAG_BACKGROUND;
        }
        switch (joint->drawMode->blendType) {
            case 0: {
                s32 k, n2 = 0, n1 = 0;
                for (i = 0; i < obj->joints->partCount; i++) {
                    u8 t = obj->joints->parts[i].material->blendMode;

                    if (t == 2) {
                        n2++;
                    } else if (t == 1) {
                        n1++;
                    }
                    for (k = 0; k < 8; k++) {
                        MapMatSampler* tex = obj->joints->parts[i].material->samplers[k];
                        if (!tex) break;
                        t = tex->blendMode;
                        if (t == 2) n2++;
                        else if (t == 1) n1++;
                    }
                }
                if (n2) obj->blendMode = 2;
                else if (n1) obj->blendMode = 1;
                else obj->blendMode = 0;
                break;
            }
            case 1: {
                // FAKE: spare temporary m; the conditional expression of cases 2 and 3 changes the object
                u8 m = 0;
                if (obj->flags & MAP_OBJ_FLAG_BACKGROUND) {
                    m = 3;
                }
                obj->blendMode = m;
                break;
            }
            case 2:
                obj->blendMode = (obj->flags & MAP_OBJ_FLAG_BACKGROUND) ? 4 : 1;
                break;
            case 3:
                obj->blendMode = (obj->flags & MAP_OBJ_FLAG_BACKGROUND) ? 5 : 2;
                break;
        }
        if (joint->drawMode->modeFlags & DRAW_MODE_GROUP) {
            obj->flags |= MAP_OBJ_FLAG_GROUP;
            if (joint->drawMode->modeFlags & DRAW_MODE_DEPTH_OFFSET) {
                obj->flags |= MAP_OBJ_FLAG_DEPTH_OFFSET;
            }
            switch (joint->drawMode->blendType) {
                case 0: obj->blendMode = 2; break;
                case 1: obj->blendMode = 0; break;
                case 2: obj->blendMode = 1; break;
                case 3:
                    default: obj->blendMode = 2; break;
            }
        }
        if (obj->flags & MAP_OBJ_FLAG_GROUP) {
            if (parent && (parent->flags & MAP_OBJ_FLAG_GROUP || parent->flags & MAP_OBJ_FLAG_GROUP_MEMBER)) {
                obj->flags &= ~MAP_OBJ_FLAG_GROUP;
            }
        } else if (parent && (parent->flags & MAP_OBJ_FLAG_GROUP || parent->flags & MAP_OBJ_FLAG_GROUP_MEMBER)) {
            obj->flags |= MAP_OBJ_FLAG_GROUP_MEMBER;
            if (parent->flags & MAP_OBJ_FLAG_DEPTH_OFFSET) {
                obj->flags |= MAP_OBJ_FLAG_DEPTH_OFFSET;
            }
        }
        if (joint->drawMode->modeFlags & DRAW_MODE_CHAR_SHADOW) {
            obj->flags |= MAP_OBJ_FLAG_CHAR_SHADOW;
        }
    } else {
#if VERSION <= VERSION_DEMO_JP
        OSPanic("mapdrv.c", 0x2c1, "\x83\x57\x83\x87\x83\x43\x83\x93\x83\x67\x82\xcc\x83\x66\x81\x5b\x83\x5e\x82\xcc\x8e\x77\x92\xe8\x82\xc9\x82\xa8\x82\xa9\x82\xb5\x82\xc8\x82\xe0\x82\xcc\x82\xaa\x82\xa0\x82\xe9\n");
#else
#if VERSION >= VERSION_DEMO_EU
        OSPanic("mapdrv.c", 0x2e8, "\x83\x57\x83\x87\x83\x43\x83\x93\x83\x67\x82\xcc\x83\x66\x81\x5b\x83\x5e\x82\xcc\x8e\x77\x92\xe8\x82\xc9\x82\xa8\x82\xa9\x82\xb5\x82\xc8\x82\xe0\x82\xcc\x82\xaa\x82\xa0\x82\xe9\n");
#else
        OSPanic("mapdrv.c", 0x2e5, "\x83\x57\x83\x87\x83\x43\x83\x93\x83\x67\x82\xcc\x83\x66\x81\x5b\x83\x5e\x82\xcc\x8e\x77\x92\xe8\x82\xc9\x82\xa8\x82\xa9\x82\xb5\x82\xc8\x82\xe0\x82\xcc\x82\xaa\x82\xa0\x82\xe9\n");
#endif
#endif
    }
    if (joint->partCount) {
        PSVECAdd(&joint->bboxMin, &joint->bboxMax, &obj->center);
        PSVECScale(&obj->center, &obj->center, 0.5f);
    } else {
        obj->center = joint->translation;
    }
    if (joint->partCount) {
        PSMTXMultVec(obj->modelWorldMtx, &obj->center, &a);
        PSMTXMultVec(obj->modelWorldMtx, &joint->bboxMax, &b);
        if (PSVECSquareDistance(&a, &b) < 20000.0f) {
            obj->flags |= MAP_OBJ_FLAG_CLIP;
        }
    }
    return obj;
}

/**
 * Creates the MapObject for joint and, recursively, its children and (unless it is the root) its later siblings,
 * linking them into the child/next tree.
 */
static MapObject* mapEntrySub(MapFileJoint* joint, MapObject* parent, Mtx mtx, BOOL isRoot, s32 index) {
    MapObject* obj = _mapEnt(joint, parent, mtx, index);
    if (joint->child) {
        obj->child = mapEntrySub(joint->child, obj, obj->modelWorldMtx, FALSE, index);
    }
    if (!isRoot) {
        if (joint->next) {
            obj->next = mapEntrySub(joint->next, parent, mtx, FALSE, index);
        }
    }
    return obj;
}

/**
 * Allocates the MapObject array for map entry index, one per joint,
 * and builds the object tree from the root joint with world matrix mtx; returns the root object.
 */
static MapObject* mapEntry(MapFileJoint* joint, Mtx mtx, s32 index) {
    MapWork* wp = mapGetWork();
    u32 size;

    wp->entries[index].numJoints = mapGetJoints(joint);
    size = OSRoundUp32B(wp->entries[index].numJoints * sizeof(MapObject));
    wp->entries[index].objects = _mapAlloc(size);
    memset(wp->entries[index].objects, 0, size);
    return mapEntrySub(joint, NULL, mtx, TRUE, index);
}

static void mapResetPaperCraftAmbColor(void);

/**
 * Records the material setup and cull mode of each eligible part of map entry group into GX display lists,
 * in two passes (size, then copy into one buffer at the heap tail).
 * Only for ver1.02 map files, and skips light-masked parts and Mario's shadow receivers
 * while a character shadow projection is on.
 */
static void makeDisplayList(BOOL group) {
    static const GXCullMode culling[4] = {GX_CULL_FRONT, GX_CULL_BACK, GX_CULL_ALL, GX_CULL_NONE};
    MapEntry* e;
    MapWork* wp = mapGetWork();
    void* buf;
    int i;
    int j;
    int pass;
    u32 total;
    u8* dst;
    MapObject* obj;
    MapJointDrawMode* dm;
    s32 k;
    void* material;

    buf = _mapAlloc(0x10000);
    e = &wp->entries[group];
    __mapdrv_make_dl = TRUE;
    total = 0;
    for (pass = 0; pass < 2; pass++) {
        obj = e->objects;
#if VERSION >= VERSION_JP
        for (k = 0; k < 8; k++) {
            obj->displayList[k] = NULL;
            obj->displayListSize[k] = 0;
        }
#endif
        for (i = 0; i < wp->entries[group].numJoints; i++, obj++) {
            if (obj->joints->partCount < 1 || obj->joints->partCount > 8) {
                continue;
            }
            dm = obj->joints->drawMode;
            if (dm->modeFlags & DRAW_MODE_MAP_LIGHTS) {
                continue;
            }
            if (dm->lightMask) {
                continue;
            }
            if (shadowGetCharShadowProjection() && (obj->flags & MAP_OBJ_FLAG_CHAR_SHADOW)) {
                continue;
            }
            if (!(e->flags & MAP_ENTRY_VER_1_02)) {
                continue;
            }
            for (j = 0; j < obj->joints->partCount; j++) {
                material = obj->joints->parts[j].material;
                memset(buf, 0, 0x10000);
                DCInvalidateRange(buf, 0x10000);
                GXBeginDisplayList(buf, 0x10000);
                GXResetWriteGatherPipe();
                mapSetMaterial(obj, material);
                GXSetCullMode(culling[obj->joints->drawMode->cullMode]);
                obj->displayListSize[j] = GXEndDisplayList();
                if (pass == 0) {
                    total += obj->displayListSize[j];
                } else {
                    obj->displayList[j] = dst;
                    dst += obj->displayListSize[j];
                    memcpy(obj->displayList[j], buf, obj->displayListSize[j]);
                    DCFlushRange(obj->displayList[j], obj->displayListSize[j]);
                }
            }
        }
        if (pass == 0) {
            e->dispListSize = total;
            if (e->dispListSize) {
                e->dispListBuf = _mapAllocTail(e->dispListSize);
                dst = e->dispListBuf;
            } else {
                wp->entries[group].dispListBuf = NULL;
                break;
            }
        }
    }
    _mapFree(buf);
    __mapdrv_make_dl = FALSE;
}

/**
 * The playing map animation called animName in the active group's entries,
 * or NULL (inline twin of mapSearchAnmObj).
 */
static inline MapAnim* mapSearchAnmObj_inl(const char* animName) {
    MapEntry* entry;
    MapWork* wp = &mapWork[activeGroup];
    int j;
    int i;
    MapAnim* anim;

    entry = wp->entries;
    for (i = 0; i < wp->entries[0].count; entry++, i++) {
        anim = (MapAnim*)entry->animData;
        for (j = 0; j < entry->numAnims; j++, anim++) {
            if ((anim->flags & MAP_ANIM_FLAG_ACTIVE) && !strcmp(anim->anim->name, animName)) {
                return anim;
            }
        }
    }
    return NULL;
}

/** The playing map animation called animName in the active group's entries, or NULL; called from aji_06 and aji_11. */
MapEntryAnimData* mapSearchAnmObj(const char* animName) {
    MapWork* wp = &mapWork[activeGroup];
    MapEntry* entry;
    int j;
    int i;
    MapAnim* anim;

    entry = wp->entries;
    for (i = 0; i < wp->entries[0].count; entry++, i++) {
        anim = (MapAnim*)entry->animData;
        for (j = 0; j < entry->numAnims; j++, anim++) {
            if ((anim->flags & MAP_ANIM_FLAG_ACTIVE) && !strcmp(anim->anim->name, animName)) {
                return anim;
            }
        }
    }
    return NULL;
}

void mapCheckAnimation(const char* animName, BOOL* valid, f32* remainingMs) {
    MapAnim* anim = mapSearchAnmObj_inl(animName);

    *valid = FALSE;
    *remainingMs = 0.0f;
    if (anim) {
        if (anim->flags & MAP_ANIM_FLAG_END) {
            *valid = TRUE;
        }
        *remainingMs = 1000.0f * (anim->anim->length - anim->time) / mapAnimFps();
    } else {
        *valid = TRUE;
    }
}

/**
 * Starts the map animation name from map.dat.
 * mode 0 plays once, 1 loops, 2 jumps to the end;
 * level 1-4 times it on the system level clock gp->systemLv0Time..Lv3Time,
 * 0 on the field or battle render clock. Called from evt_map, battle stage objects, gor_03 and hei_07.
 */
void mapPlayAnimationLv(const char* name, s32 mode, s32 level) {
    MapAnim* anim = NULL;
    MapWork* wp;
    MapEntry* entry;
    int j;
    int i = 0;
    BOOL found = FALSE;
    OSTime t;

    wp = &mapWork[activeGroup];
    entry = wp->entries;
    for (i = 0; i < wp->entries[0].count; entry++, i++) {
        anim = (MapAnim*)entry->animData;
        for (j = 0; j < entry->numAnims; j++, anim++) {
            if (!strcmp(anim->anim->name, name)) {
                found = TRUE;
                break;
            }
        }
        if (found) {
            break;
        }
    }
    if (found) {
        anim->flags = 0;
        switch (level) {
            case 0:
                break;
            case 1:
                anim->flags |= MAP_ANIM_FLAG_LV0_TIME;
                break;
            case 2:
                anim->flags |= MAP_ANIM_FLAG_LV1_TIME;
                break;
            case 3:
                anim->flags |= MAP_ANIM_FLAG_LV2_TIME;
                break;
            case 4:
                anim->flags |= MAP_ANIM_FLAG_LV3_TIME;
                break;
        }
        if (anim->flags & MAP_ANIM_FLAG_LV0_TIME) {
            t = gp->systemLv0Time;
        } else if (anim->flags & MAP_ANIM_FLAG_LV1_TIME) {
            t = gp->systemLv1Time;
        } else if (anim->flags & MAP_ANIM_FLAG_LV2_TIME) {
            t = gp->systemLv2Time;
        } else if (anim->flags & MAP_ANIM_FLAG_LV3_TIME) {
            t = gp->systemLv3Time;
        } else if (gp->inBattle) {
            t = gp->renderTime;
        } else {
            t = gp->renderFieldTime;
        }
        anim->startTime = OSTicksToMilliseconds(t);
        anim->time = 0.0f;
        anim->rate = 1.0f;
        anim->flags |= MAP_ANIM_FLAG_ACTIVE;
        if (mode == MAP_ANIM_PLAY_LOOP) {
            anim->flags |= MAP_ANIM_FLAG_LOOP;
        }
        if (mode == MAP_ANIM_PLAY_AT_END) {
            anim->startTime -= (u64)(1000.0f * anim->anim->length / mapAnimFps());
        }
    }
}

/** Pauses the playing map animation name; mapMain stops advancing it. */
void mapPauseAnimation(const char* name) {
    MapAnim* anim = mapSearchAnmObj_inl(name);
    if (anim) {
        anim->flags |= MAP_ANIM_FLAG_PAUSE;
    }
}

/** Resumes the map animation name paused by mapPauseAnimation, from the frame it stopped on. */
void mapReStartAnimation(const char* name) {
    MapAnim* anim = mapSearchAnmObj_inl(name);
    if (anim) {
        anim->flags |= MAP_ANIM_FLAG_RESTART;
    }
}

/** Pauses every map animation of the active group; the name argument is ignored. */
void mapPauseAnimationAll(const char* name) {
    MapWork* wp = &mapWork[activeGroup];
    MapEntry* entry = wp->entries;
    int j;
    int i;
    MapAnim* anim;

    for (i = 0; i < wp->entries[0].count; entry++, i++) {
        anim = (MapAnim*)entry->animData;
        for (j = 0; j < entry->numAnims; j++, anim++) {
            anim->flags |= MAP_ANIM_FLAG_PAUSE;
        }
    }
}

/** Resumes every paused map animation of the active group; the name argument is ignored. */
void mapReStartAnimationAll(const char* name) {
    MapWork* wp = &mapWork[activeGroup];
    MapEntry* entry = wp->entries;
    int j;
    int i;
    MapAnim* anim;

    for (i = 0; i < wp->entries[0].count; entry++, i++) {
        anim = (MapAnim*)entry->animData;
        for (j = 0; j < entry->numAnims; j++, anim++) {
            anim->flags |= MAP_ANIM_FLAG_RESTART;
        }
    }
}

/** Sets the speed multiplier of the playing map animation name (1.0 is normal speed). */
void mapSetPlayRate(const char* name, f32 rate) {
    MapAnim* anim = mapSearchAnmObj_inl(name);
    if (anim) {
        anim->rate = rate;
    }
}

void* HSD_ArchiveGetPublicAddress(MapHeader* header, const char* name);

/**
 * Turns the file-relative pointers listed in the map.dat relocation table into real addresses by adding the data
 * base.
 */
static inline void mapLoadReloc_(MapEntry* entry) {
    u32 k;
    for (k = 0; k < (u32)entry->header.file.relCount; k++) {
        u32* rel = (u32*)((u8*)entry->header.data + entry->header.rel[k]);
        if (*rel <= 0x80000000) {
            *rel += (u32)entry->header.data;
        }
    }
}

/**
 * Whether the map header's stored file size equals the loaded size.
 * MWCC swaps == operands when the left side is a computed address (wp->entries[group]...);
 * through a pointer parameter the load stays on the left, as in the original (cmplw r0,r17).
 */
static inline BOOL mapHdrSizeOk_(MapHeader* h, u32 size) {
    return h->file.fileSize == size;
}

/**
 * Parses the map.dat header of entry group: locates the data, relocation, chunk,
 * extern and string sections after it and relocates the pointers.
 * Does nothing if the stored file size does not match the loaded size.
 */
static inline void mapLoadHeader_(MapWork* wp, BOOL group) {
    u32 datSize = wp->entries[group].datSize;
    u8* data = (u8*)wp->entries[group].dat;
    u32 sizeCheck;
    MapHeader* header = &wp->entries[group].header;

    // header is the address of a member and never NULL; the test is in the retail code all the same
    if (header != NULL) {
        memset(header, 0, sizeof(MapHeader));
        wp->entries[group].header.loadFlags |= 1;
        memcpy(header, data, sizeof(MapFileHeader));
        if (mapHdrSizeOk_(&wp->entries[group].header, datSize)) {
            sizeCheck = sizeof(MapFileHeader);
            if ((u32)wp->entries[group].header.file.dataSize != 0u) {
                wp->entries[group].header.data = &data[sizeCheck];
                sizeCheck = wp->entries[group].header.file.dataSize;
                sizeCheck += sizeof(MapFileHeader);
            }
            if ((u32)wp->entries[group].header.file.relCount != 0u) {
                wp->entries[group].header.rel = (u32*)&data[sizeCheck];
                sizeCheck += 4 * wp->entries[group].header.file.relCount;
            }
            if ((u32)wp->entries[group].header.file.chunkCount != 0u) {
                wp->entries[group].header.chunks = (MapFileChunk*)&data[sizeCheck];
                sizeCheck += sizeof(MapFileChunk) * wp->entries[group].header.file.chunkCount;
            }
            if ((u32)wp->entries[group].header.file.unused != 0u) {
                wp->entries[group].header.unused = &data[sizeCheck];
                sizeCheck += 8 * wp->entries[group].header.file.unused;
            }
            if (sizeCheck < wp->entries[group].header.file.fileSize) {
                wp->entries[group].header.strings = (const char*)&data[sizeCheck];
            }
            wp->entries[group].header.rawFile = data;
            mapLoadReloc_(&wp->entries[group]);
        }
    }
}

// FAKE: chunk search written out here with k declared last, for the target k/w register order; same search
static inline void mapLoadAnim_(BOOL group) {
    MapWork* w;
    u32* animTbl;
    u32 n;
    u32 k;
    MapEntryAnimData* a;
    int i;
    MapEntry* e2;
    w = &mapWork[activeGroup];
    e2 = &w->entries[group];
    for (k = 0; k < e2->header.file.chunkCount; k++) {
        if (!strcmp(&e2->header.strings[e2->header.chunks[k].string], "animation_table")) {
            animTbl = (u32*)((u8*)e2->header.data + e2->header.chunks[k].offset);
            goto found;
        }
    }
    animTbl = NULL;
found:
    if (animTbl != NULL) {
        w->entries[group].numAnims = animTbl[0];
        if (w->entries[group].numAnims != 0) {
            n = OSRoundUp32B(w->entries[group].numAnims * 32);
            w->entries[group].animData = _mapAlloc(n);
            memset(w->entries[group].animData, 0, n);
            a = w->entries[group].animData;
            for (i = 0; i < w->entries[group].numAnims; i++) {
                a->flags = 0;
                a->anim = (MapAnimHeader*)*++animTbl;
                a++;
            }
        }
    }
}

// The early demos still use the long map file names.
#if VERSION <= VERSION_DEMO_JP
#define MAP_PATH_DAT "./map/%s/map.dat"
#define MAP_PATH_TPL "./map/%s/texture.tpl"
#define MAP_PATH_CAMROAD "./map/%s/camera_road.bin"
#else
#define MAP_PATH_DAT "./m/%s/d"
#define MAP_PATH_TPL "./m/%s/t"
#define MAP_PATH_CAMROAD "./m/%s/c"
#endif

/**
 * Loads map into entry group of wp: map.dat and texture.tpl from the archive or the disc,
 * the joint tree as map and hit objects (scaled x10), the map's lights and fog, the camera road (field only),
 * textures, bounding box corners, animation table and display lists.
 */
#if VERSION <= VERSION_DEMO_JP
static void _mapLoad(MapWork* wp, BOOL group, const char* map) {
    MapFileInfo* info;
    u32 datSize, k;
    int i;
    u32 r4, r7, r0;
    u32 sizeCheck;
#else
static void _mapLoad(MapWork* wp, BOOL group, const char* map) {
    MapFileInfo* info;
    int i;
#endif
    char str1[128];
    char str2[128];
    char str3[128];
    DVDEntry* dvd;
    MapWork* wo; // FAKE: alias of wp; the chunk-table stores go through it to keep the original address reloads
    MapHeader* header;

    wo = wp;
    error_flag = FALSE;
    error_count = 0;
    strncpy(wp->entries[group].name, map, sizeof(wp->entries[group].name));

    sprintf(str1, MAP_PATH_DAT, map);
    arcOpen(str1, &wp->entries[group].dat, &wp->entries[group].datSize);
    if (!wp->entries[group].dat) {
        sprintf(str2, "%s/%s", ".", str1);
        dvd = DVDMgrOpen(str2, 2, 0);
        wp->entries[group].datSize = DVDMgrGetLength(dvd);
        wp->entries[group].dat = _mapAllocTail(OSRoundUp32B(wp->entries[group].datSize));
        DVDMgrRead(dvd, wp->entries[group].dat, OSRoundUp32B(wp->entries[group].datSize), 0);
        DVDMgrClose(dvd);
    }

    sprintf(str1, MAP_PATH_TPL, map);
    wp->entries[group].tpl = NULL;
    arcOpen(str1, &wp->entries[group].tpl, &wp->entries[group].tplSize);
    if (wp->entries[group].tpl) {
        UnpackTexPalette(wp->entries[group].tpl);
    } else {
        sprintf(str3, "%s/%s", ".", str1);
        dvd = DVDMgrOpen(str3, 2, 0);
        if (dvd) {
            wp->entries[group].tplSize = DVDMgrGetLength(dvd);
            wp->entries[group].tpl = _mapAllocTail(OSRoundUp32B(wp->entries[group].tplSize));
            DVDMgrRead(dvd, wp->entries[group].tpl, OSRoundUp32B(wp->entries[group].tplSize), 0);
            DVDMgrClose(dvd);
            UnpackTexPalette(wp->entries[group].tpl);
        }
    }

    GXInvalidateVtxCache();
    mapLoadHeader_(wp, group);
    header = &wp->entries[group].header;
    info = (MapFileInfo*)HSD_ArchiveGetPublicAddress(header, "information");
    wo->entries[group].textureTable = HSD_ArchiveGetPublicAddress(header, "texture_table");
    wo->entries[group].lightTable = HSD_ArchiveGetPublicAddress(header, "light_table");
    wo->entries[group].fogTable = HSD_ArchiveGetPublicAddress(header, "fog_table");
    wo->entries[group].materialNameTable = HSD_ArchiveGetPublicAddress(header, "material_name_table");
    wo->entries[group].vcdTable = HSD_ArchiveGetPublicAddress(header, "vcd_table");
    {
        MapFileJoint* mapJoint = mapSearchDmdJoint(info->joint, info->mapName);
        MapFileJoint* hitJoint = mapSearchDmdJoint(info->joint, info->hitName);
        Mtx scale;
        if (strcmp(info->version, "ver1.02") == 0) {
            wp->entries[group].flags |= MAP_ENTRY_VER_1_02;
        } else {
            wp->entries[group].flags &= ~MAP_ENTRY_VER_1_02;
        }
        PSMTXScale(scale, 10.0f, 10.0f, 10.0f);
        wp->entries[group].rootMapObj = mapEntry(mapJoint, scale, group);
        wp->entries[group].rootHitObj = hitEntry(hitJoint, scale, group);
        for (i = 0; i < wp->entries[group].lightTable->count; i++) {
            MapFileLight* light = wp->entries[group].lightTable->lights[i];
            Vec pos;
            GXColor lcolor;
            s32 type = LIGHT_TYPE_POINT;
            s32 lflags = 0;
            PSVECScale(&light->position, &pos, 10.0f);
            if (!strcmp(light->type, "ambientLight")) {
                type = LIGHT_TYPE_AMBIENT;
            } else if (!strcmp(light->type, "pointLight")) {
                type = LIGHT_TYPE_POINT;
            } else if (!strcmp(light->type, "directionalLight")) {
                type = LIGHT_TYPE_DIRECTIONAL;
            } else if (!strcmp(light->type, "spotLight")) {
                type = LIGHT_TYPE_SPOT;
            }
            if (light->magic == MAP_LIGHT_LFLAGS_MAGIC) {
                lflags = light->lflags;
            }
            lcolor = light->color;
            lightEntry(light->name, type, &pos, &light->rotation, &lcolor, light->spotAngle, light->attenAngle, light->attenType, lflags);
        }
        mapResetPaperAmbColor();
        mapResetPaperCraftAmbColor();
        if ((&wo->entries[group])->fogTable != NULL) {
            wo->entries[group].fogEnable = (&wo->entries[group])->fogTable->enable;
            wp->entries[group].fogType = (&wo->entries[group])->fogTable->type;
            wp->entries[group].fogStart = (&wo->entries[group])->fogTable->start;
            wp->entries[group].fogEnd = (&wo->entries[group])->fogTable->end;
            wp->entries[group].fogColor = (&wo->entries[group])->fogTable->color;
            if (wo->entries[group].fogEnable != 0) {
                wp->entries[group].flags |= MAP_ENTRY_FOG;
            }
        } else {
            wo->entries[group].fogEnable = 0;
            wp->entries[group].fogType = 0;
            wp->entries[group].fogStart = 0.0f;
            wp->entries[group].fogEnd = 0.0f;
            wp->entries[group].fogColor = (GXColor){0, 0, 0, 0};
            wp->entries[group].flags &= ~MAP_ENTRY_FOG;
        }
        if (activeGroup != 1) {
            sprintf(str1, MAP_PATH_CAMROAD, map);
            camLoadRoad(CAMERA_3D, str1);
        }
        camSetTypePersp(CAMERA_3D);
#if VERSION <= VERSION_DEMO_JP
        mapBuildTexture(wp->entries[group].rootMapObj, wp->entries[group].tpl, wp->entries[group].textureTable);
#else
        mapBuildTexture(wp->entries[group].rootMapObj, wp->entries[group].tpl, (&wo->entries[group])->textureTable);
#endif
        {
            Vec mn;
            Vec mx;
            mapGetBoundingBox(&mn, &mx);

// Macro, not inline: as a static inline (mapSetBBoxCorner(dst, x, y, z)) the object changes.
#define SET_BBOX_CORNER(idx, X, Y, Z)     \
    {                                     \
        Vec v = {0.0f, 0.0f, 0.0f};       \
        v.x = X;                          \
        v.y = Y;                          \
        v.z = Z;                          \
        wp->entries[group].bbox[idx] = v; \
    }
            SET_BBOX_CORNER(0, mx.x, mx.y, mx.z)
            SET_BBOX_CORNER(1, mn.x, mx.y, mx.z)
            SET_BBOX_CORNER(2, mx.x, mn.y, mx.z)
            SET_BBOX_CORNER(3, mx.x, mx.y, mn.z)
            SET_BBOX_CORNER(4, mn.x, mn.y, mx.z)
            SET_BBOX_CORNER(5, mx.x, mn.y, mn.z)
            SET_BBOX_CORNER(6, mn.x, mx.y, mn.z)
            SET_BBOX_CORNER(7, mn.x, mn.y, mn.z)
#undef SET_BBOX_CORNER
        }
        mapLoadAnim_(group);
    }
    makeDisplayList(group);
    {
        MapWork* w3 = &mapWork[activeGroup];
        MapObject* o = w3->entries[group].objects;
        for (i = 0; i < w3->entries[group].numJoints; i++, o++) {
            MapJointDrawMode* dm;
            if (o->joints->partCount < 1 || o->joints->partCount > 8) {
                continue;
            }
            dm = o->joints->drawMode;
            if (dm->modeFlags & DRAW_MODE_MAP_LIGHTS) {
                continue;
            }
            if (dm->lightMask) {
                continue;
            }
            if (shadowGetCharShadowProjection() && (o->flags & MAP_OBJ_FLAG_CHAR_SHADOW)) {
                continue;
            }
            if (!(w3->entries[group].flags & MAP_ENTRY_VER_1_02)) {
                continue;
            }
            o->flags |= MAP_OBJ_FLAG_DISPLAY_LIST;
        }
    }
#if VERSION >= VERSION_JP
    disableMapDraw = FALSE;
#endif
}

/**
 * Frees the field map and everything built from it:
 * map objects, animations, display lists, hit objects, lights, environment, the camera road and the map files.
 * Called by seq_mapchange when leaving a map.
 */
void mapUnLoad(void) {
    MapEntry* entry2;
    MapEntry* entry;
    int i;
    MapWork* wp;
#if VERSION >= VERSION_JP
    sysWaitDrawSync();
#endif
    entry = mapWork->entries;
    wp = mapWork;
    activeGroup = i = 0;
    for (; i < wp->entries[0].count; entry++, i++) {
        if (entry->objects) {
            _mapFree(entry->objects);
            entry->numJoints = 0;
            entry->objects = NULL;
        }
        if (entry->animData) {
            _mapFree(entry->animData);
            entry->numAnims = 0;
            entry->animData = NULL;
        }
        if (entry->dispListBuf) {
            _mapFree(entry->dispListBuf);
            entry->dispListBuf = NULL;
            entry->dispListSize = 0;
        }
    }
#if VERSION >= VERSION_JP
    disableMapDraw = FALSE;
#endif
    hitReInit();
    lightReInit();
    envReInit();
    camUnLoadRoad(CAMERA_3D);
    camSetTypePersp(CAMERA_3D);
    entry2 = wp->entries;
    for (i = 0; i < wp->entries[0].count; entry2++, i++) {
        if (entry2->dat) {
            if (!arcDataCheck(entry2->dat)) {
                _mapFree(entry2->dat);
            }
            entry2->dat = NULL;
        }
        if (entry2->tpl) {
            if (!arcDataCheck(entry2->tpl)) {
                _mapFree(entry2->tpl);
            }
            entry2->tpl = NULL;
        }
        if (entry2->arcData) {
            _mapFree(entry2->arcData);
            entry2->arcData = NULL;
        }
        strncpy(entry2->name, "", sizeof(entry2->name));
    }
    arcDelete(1);
    memset(wp, 0, sizeof(MapWork));
}

#if VERSION >= VERSION_JP
BOOL mapPreLoad(const char* map) {
    return TRUE;
}
#endif

/**
 * Loads the field map map into MapWork slot 0 as its only entry; called by seq_mapchange.
 * The early demos first unpack the map's compressed archive.
 */
#if VERSION >= VERSION_JP
void mapLoad(const char* map) {
    MapWork* wp;

    // FAKE: wp2 is a second copy of mapWork used only by the memset; memset(wp, ...) without it changes the object
    MapWork* wp2;

    sysWaitDrawSync();
    wp2 = wp = mapWork;
    activeGroup = 0;
    memset(wp2, 0, sizeof(MapWork));
    _mapLoad(wp, 0, map);
    wp->entries[0].count = 1;
}
#endif

// FAKE: opt_common_subs off for bmapUnLoad (the block also spans the early demos' mapPreLoad and mapLoad before
// it). With CSE on, `wp = mapWork` (last loop) becomes a CSE temp created before the lifetime-split temps, so wp's
// register lands above i's. With it off, the second web of the reused `wp` is a
// split temp; split temps are created in order of the variables' first reference (i before wp before entry), which gives
// the target's i r31 / wp r30 / entry r29. Code is otherwise identical.
#pragma opt_common_subs off
#if VERSION <= VERSION_DEMO_JP
/**
 * The early demos read each map's LZ77-compressed archive "<root>/b/<map>" itself:
 * mapPreLoad starts the read, mapLoad unpacks it into the arc slot before loading the map.
 */
BOOL mapPreLoad(const char* map) {
    FileEntry* file = (FileEntry*)fileAsyncf(0, NULL, "%s/b/%s", getMarioStDvdRoot(), map);

    if (file == NULL) {
        return FALSE;
    }
    if ((s32)file == -1) {
        return TRUE;
    }
    return TRUE;
}
#endif

/**
 * Loads the field map map into MapWork slot 0 as its only entry; called by seq_mapchange.
 * The early demos first unpack the map's compressed archive.
 */
#if VERSION <= VERSION_DEMO_JP
void mapLoad(const char* map) {
    char path[256];
    MapWork* wp;
    FileEntry* file;
    u32 size;
    void* arc;
    u32 header;

    activeGroup = 0;
    memset(wp = mapWork, 0, sizeof(MapWork));
    sprintf(path, "%s/b/%s", getMarioStDvdRoot(), map);
    file = fileAlloc(path, 0);
    if (file != NULL) {
        header = *(u32*)*file->data;
        size = ((header & 0xFF) << 16) | (header & 0xFF00) | ((header >> 16) & 0xFF);
        arc = _mapAllocTail(size);
        LZ77Decode(*file->data, arc);
        arcEntry(1, arc, size);
        DCFlushRange(arc, size);
        GXInvalidateVtxCache();
        GXInvalidateTexAll();
        wp->entries[0].arcData = arc;
        wp->entries[0].arcSize = size;
        fileFree(file);
    }
    _mapLoad(wp, 0, map);
    wp->entries[0].count = 1;
}
#endif

/**
 * Frees the battle stage maps (MapWork slot 1) at the end of a battle,
 * then copies the field map's files back from ARAM and rebuilds its display lists so the field can resume.
 */
#if VERSION <= VERSION_DEMO_JP
void bmapUnLoad(void) {
    MapWork* wp2;
    MapEntry* entry3;
    int j;
    MapWork* wp;
    MapEntry* entry;
    MapEntry* entry2;
    int i;
    s32 gz;
#else
void bmapUnLoad(void) {
    MapEntry* entry2;
    MapEntry* entry;
    int i;
    MapWork* wp;

    sysWaitDrawSync();
#endif
    i = 0;
    wp = &mapWork[1];
    activeGroup = 1;
    for (entry = wp->entries; i < wp->entries[0].count; entry++, i++) {
        if (entry->objects) {
            _mapFree(entry->objects);
            entry->numJoints = 0;
            entry->objects = NULL;
        }
        if (entry->animData) {
            _mapFree(entry->animData);
            entry->numAnims = 0;
            entry->animData = NULL;
        }
        if (entry->dispListBuf) {
            _mapFree(entry->dispListBuf);
            entry->dispListBuf = NULL;
            entry->dispListSize = 0;
        }
    }
#if VERSION >= VERSION_JP
    disableMapDraw = FALSE;
#endif
    hitReInit();
    lightReInit();
    envReInit();
    entry2 = wp->entries;
    for (i = 0; i < wp->entries[0].count; entry2++, i++) {
        if (entry2->dat) {
            if (!arcDataCheck(entry2->dat)) {
                _mapFree(entry2->dat);
            }
            entry2->dat = NULL;
        }
        if (entry2->tpl) {
            if (!arcDataCheck(entry2->tpl)) {
                _mapFree(entry2->tpl);
            }
            entry2->tpl = NULL;
        }
        if (entry2->arcData) {
            _mapFree(entry2->arcData);
            entry2->arcData = NULL;
        }
        strncpy(entry2->name, "", sizeof(entry2->name));
    }
    arcDelete(1);
    arcDelete(2);
    memset(wp, 0, sizeof(MapWork));

    // the tpl ARAM copy is kept in the word after the entry (MAPENT_TPLARAM)
#if VERSION <= VERSION_DEMO_JP
    j = 0;
    gz = 0;
    if (gz != 0) {
        j = 0;
    }
    wp2 = mapWork;
    activeGroup = j;
    for (entry3 = wp2->entries; j < wp2->entries[0].count; entry3++, j++) {

        // the tpl ARAM copy sits in the word after the entry (MAPENT_TPLARAM; a MapWork { count; entries@4 } layout was tried and changes codegen: the original addresses entries from wp itself)
        if (entry3->datAram) {
            entry3->dat = _mapAllocTail(entry3->datSize);
            entry3->tpl = _mapAllocTail(entry3->tplSize);
            aramMgrToMram(entry3->datAram, entry3->dat);
            aramMgrToMram(MAPENT_TPLARAM(entry3), entry3->tpl);
            entry3->datAram = NULL;
            MAPENT_TPLARAM(entry3) = NULL;
        } else {
            entry3->arcData = _mapAllocTail(entry3->arcSize);
            aramMgrToMram(entry3->arcAram, entry3->arcData);
            entry3->arcAram = NULL;
        }
        makeDisplayList(j);
    }
    if (wp2->entries->arcData) {
        arcEntry(1, wp2->entries->arcData, wp2->entries->arcSize);
    }
}
#else
    wp = mapWork;
    activeGroup = 0;
    for (i = 0, entry = wp->entries; i < wp->entries[0].count; entry++, i++) {
        if (entry->datAram) {
            entry->dat = _mapAllocTail(entry->datSize);
            entry->tpl = _mapAllocTail(entry->tplSize);
            aramMgrToMram(entry->datAram, entry->dat);
            aramMgrToMram(MAPENT_TPLARAM(entry), entry->tpl);
            entry->datAram = NULL;
            MAPENT_TPLARAM(entry) = NULL;
        } else {
            entry->arcData = _mapAllocTail(entry->arcSize);
            aramMgrToMram(entry->arcAram, entry->arcData);
            entry->arcAram = NULL;
        }
        makeDisplayList(i);
    }
    if (wp->entries->arcData) {
        arcEntry(1, wp->entries->arcData, wp->entries->arcSize);
    }
}
#endif
#pragma opt_common_subs reset

#if VERSION <= VERSION_DEMO_JP
/**
 * Early demos only: starts the async read of the battle map archives arc_d/<map>.arc;
 * TRUE only when map and map2 are both given and available (a NULL map2 always gives FALSE).
 */
BOOL bmapPreLoad(const char* map, const char* map2) {
    FileEntry* file = NULL;
    FileEntry* file2 = NULL;

    if (map != NULL) {
        file = (FileEntry*)fileAsyncf(0, NULL, "%s/arc_d/%s.arc", getMarioStDvdRoot(), map);
    }
    if (map2 != NULL) {
        file2 = (FileEntry*)fileAsyncf(0, NULL, "%s/arc_d/%s.arc", getMarioStDvdRoot(), map2);
    }
    if (file == NULL) {
        return FALSE;
    }
    return file2 != NULL;
}
#endif
/**
 * Called by seq_battle at battle start: parks the field map's files in ARAM,
 * then loads the battle stage map into MapWork slot 1 as entry 0 and, if given,
 * the stage-specific map map2 as entry 1. The early demos read and unpack both archives themselves.
 */
void bmapLoad(const char* map, const char* map2) {
#if VERSION <= VERSION_DEMO_JP
    char path[256];
#endif
    MapWork* wp;
    MapEntry* entry;
    MapEntry* e;
    int i;
#if VERSION <= VERSION_DEMO_JP
    FileEntry* file;
    u32 size;
    void* arc;
    u32 header;
    u32 size2;
    FileEntry* file2;
    void* arc2;
    u32 header2;
#else
    sysWaitDrawSync();
#endif
    activeGroup = 0;
    wp = mapGetWork();
    for (i = 0; i < wp->entries[0].count; i++) {
        e = &mapWork[activeGroup].entries[i];
        entry = &wp->entries[i];
        if (e->dispListBuf) {
            _mapFree(e->dispListBuf);
        }
        e->dispListSize = 0;
        e->dispListBuf = NULL;

        // the tpl ARAM copy is kept in the word after the entry (MAPENT_TPLARAM)
        entry->datAram = NULL;
        MAPENT_TPLARAM(entry) = NULL;
        entry->arcAram = NULL;
        if (entry->dat) {
            if (!arcDataCheck(entry->dat)) {
                entry->datAram = aramMgrToAram(entry->dat, entry->datSize);
                MAPENT_TPLARAM(entry) = aramMgrToAram(entry->tpl, entry->tplSize);
                _mapFree(entry->tpl);
                _mapFree(entry->dat);
                entry->tpl = NULL;
                entry->dat = NULL;
            } else {
                entry->arcAram = aramMgrToAram(entry->arcData, entry->arcSize);
                _mapFree(entry->arcData);
                entry->arcData = NULL;
            }
        }
    }

    // FAKE: comma expression `(0, X)`: X becomes a separate compiler temp; the plain X changes the object
    wp = (0, &mapWork[1]);
    activeGroup = 1;
    memset(wp, 0, sizeof(MapWork));
#if VERSION <= VERSION_DEMO_JP
    if (map != NULL) {
        sprintf(path, "%s/b/%s", getMarioStDvdRoot(), map);
        file = fileAlloc(path, 0);
        if (file != NULL) {
            header = *(u32*)*file->data;
            size = ((header & 0xFF) << 16) | (header & 0xFF00) | ((header >> 16) & 0xFF);
            arc = _mapAllocTail(size);
            LZ77Decode(*file->data, arc);
            arcEntry(1, arc, size);
            DCFlushRange(arc, size);
            GXInvalidateVtxCache();
            GXInvalidateTexAll();
            wp->entries[0].arcData = arc;
            wp->entries[0].arcSize = size;
            fileFree(file);
        }
    }
    if (map2 != NULL) {
        sprintf(path, "%s/b/%s", getMarioStDvdRoot(), map2);
        file2 = fileAlloc(path, 0);
        if (file2 != NULL) {
            header2 = *(u32*)*file2->data;
            size2 = ((header2 & 0xFF) << 16) | (header2 & 0xFF00) | ((header2 >> 16) & 0xFF);
            arc2 = _mapAllocTail(size2);
            LZ77Decode(*file2->data, arc2);
            arcEntry(2, arc2, size2);
            DCFlushRange(arc2, size2);
            GXInvalidateVtxCache();
            GXInvalidateTexAll();
            wp->entries[1].arcData = arc2;
            wp->entries[1].arcSize = size2;
            fileFree(file2);
        }
    }
#endif
    _mapLoad(wp, 0, map);
    wp->entries[0].count = 1;
    if (map2) {
        _mapLoad(wp, 1, map2);
        wp->entries[0].count = 2;
    }
}

typedef struct MapAnimKeyCh {
    f32 value;  // 0x0
    f32 tanIn;  // 0x4
    f32 tanOut; // 0x8
    f32 unkC;   // 0xC
    s32 step;   // 0x10
} MapAnimKeyCh;

typedef struct MapAnimKey {
    f32 time;             // 0x0
    MapAnimKeyCh ch[21];  // 0x4
} MapAnimKey;

typedef struct MapAnimJoint {
    const char* name;           // 0x0, read through MapAnimNm (mapMain)
    f32 base[9];                // 0x4
    u8 pad_28[0x58 - 0x28];     // 0x28
    s32 keyCount;               // 0x58
    MapAnimKey keys[];          // 0x5C
} MapAnimJoint;

/**
 * Cubic Hermite interpolation at time cur between key (t0, p0, tangent m0) and key (t1, p1, tangent m1),
 * as used by the map animation keys.
 */
static inline f32 mapHermite(f32 cur, f32 t0, f32 t1, f32 p0, f32 m0, f32 p1, f32 m1) {
    f32 s = cur - t0;
    f32 dt = t1 - t0;
    f32 t = s / (!dt ? 1e-8f : dt);
    f32 t2 = t * t;
    f32 t3 = t2 * t;

    return p0 * (2.0f * t3 - 3.0f * t2 + 1.0f)
        + m0 * (dt * (t3 - 2.0f * t2 + t))
        + p1 * (-2.0f * t3 + 3.0f * t2)
        + m1 * (dt * (t3 - t2));
}

/** mapHermite between two animation key channels: a's value and out tangent to b's value and in tangent. */
static inline f32 mapHermiteCh(f32 cur, f32 t0, f32 t1, MapAnimKeyCh* a, MapAnimKeyCh* b) {
    f32 s = cur - t0;
    f32 dt = t1 - t0;
    f32 m1 = b->tanIn;
    f32 p1 = b->value;
    f32 m0 = a->tanOut;
    f32 p0 = a->value;
    f32 t = s / (!dt ? 1e-8f : dt);
    f32 t2 = t * t;
    f32 t3 = t2 * t;

    return p0 * (2.0f * t3 - 3.0f * t2 + 1.0f)
        + m0 * (dt * (t3 - 2.0f * t2 + t))
        + p1 * (-2.0f * t3 + 3.0f * t2)
        + m1 * (dt * (t3 - t2));
}

/**
 * Builds out = in times the animated offset of a joint at anim's current time:
 * translation, rotation (degrees) and scale channels are interpolated between keys
 * and taken relative to the joint's base pose. Also used by hitdrv for animated collision.
 */
void mapCalcAnimMatrix(Mtx out, Mtx in, MapAnim* anim, MapAnimJoint* joint) {
    f32 v[21];
    Mtx mT;
    Mtx mS;
    Mtx mX;
    Mtx mY;
    Mtx mZ;
    Mtx m;
    f32 dx, dy, dz, rx, ry, sx, sy, sz, rz;
    f32 cur;
    int c;
    int i;
    MapAnimKey* k0;
    MapAnimKey* k1;

    for (i = 1; i < joint->keyCount; i++) {
        k1 = &joint->keys[i];
        k0 = &joint->keys[i - 1];
        if (anim->time <= k1->time) {
            cur = anim->time;
        } else if (i == joint->keyCount - 1) {
            cur = k1->time;
        } else {
            continue;
        }
        for (c = 0; c < 9; c++) {
            if (k0->ch[c].step) {
                v[c] = k0->ch[c].value;
            } else {
                v[c] = mapHermiteCh(cur, k0->time, k1->time, &k0->ch[c], &k1->ch[c]);
            }
        }
        dx = v[0] - joint->base[0];
        dy = v[1] - joint->base[1];
        dz = v[2] - joint->base[2];
        rx = v[3] - joint->base[3];
        ry = v[4] - joint->base[4];
        rz = v[5] - joint->base[5];
        sx = v[6] / joint->base[6];
        sy = v[7] / joint->base[7];
        sz = v[8] / joint->base[8];
        if (dx || dy || dz) {
            PSMTXTrans(mT, dx, dy, dz);
        }
        if (rx) {
            PSMTXRotRad(mX, 'x', 0.017453292f * rx);
        }
        if (ry) {
            PSMTXRotRad(mY, 'y', 0.017453292f * ry);
        }
        if (rz) {
            PSMTXRotRad(mZ, 'z', 0.017453292f * rz);
        }
        PSMTXScale(mS, sx, sy, sz);
        PSMTXIdentity(m);
        if (dx || dy || dz) {
            PSMTXConcat(m, mT, m);
        }
        if (rz) {
            PSMTXConcat(m, mZ, m);
        }
        if (ry) {
            PSMTXConcat(m, mY, m);
        }
        if (rx) {
            PSMTXConcat(m, mX, m);
        }
        if (sx || sy || sz) {
            PSMTXConcat(m, mS, m);
        }
        PSMTXConcat(in, m, out);
        return;
    }
}

/**
 * Recomputes the world matrix of obj (dirty flag 0x200 or force) from the parent matrix mtx and its local
 * or animated matrix plus the script transform,
 * and walks the children (forced when the parent changed) and siblings.
 */
static void mapReCalcMatrix(MapObject* obj, Mtx mtx, BOOL force) {
    BOOL dirty = FALSE;

    if (obj->flags & MAP_OBJ_FLAG_MTX_DIRTY) {
        dirty = TRUE;
    }
    if (force) {
        dirty = TRUE;
    }
    if (dirty) {
        if (obj->flags & MAP_OBJ_FLAG_ANIM_MTX) {
            PSMTXConcat(mtx, obj->animMtx, obj->modelWorldMtx);
        } else {
            PSMTXConcat(mtx, obj->localMtx, obj->modelWorldMtx);
        }
        if (obj->flags & MAP_OBJ_FLAG_TRANSFORM) {
            PSMTXConcat(obj->modelWorldMtx, obj->transformMtx, obj->modelWorldMtx);
        }
        obj->flags &= ~MAP_OBJ_FLAG_MTX_DIRTY;
        if (obj->child) {
            mapReCalcMatrix(obj->child, obj->modelWorldMtx, TRUE);
        }
        if (obj->next) {
            mapReCalcMatrix(obj->next, mtx, force);
        }
    } else {
        if (obj->child) {
            mapReCalcMatrix(obj->child, obj->modelWorldMtx, force);
        }
        if (obj->next) {
            mapReCalcMatrix(obj->next, mtx, force);
        }
    }
}

typedef struct MapAnimKey4 {
    f32 time;
    MapAnimKeyCh ch[4];
} MapAnimKey4;

typedef struct MapAnimKey5 {
    f32 time;
    MapAnimKeyCh ch[5];
} MapAnimKey5;

typedef struct MapAnimKey9 {
    f32 time;
    MapAnimKeyCh ch[9];
} MapAnimKey9;

typedef struct MapAnimMatData {
    const char* name;
    s32 idx;
    f32 u;
    f32 v;
    s32 keyCount;
    MapAnimKey5 keys[1];
} MapAnimMatData;

typedef struct MapAnimAlphaData {
    const char* name;
    s32 keyCount;
    MapAnimKey4 keys[1];
} MapAnimAlphaData;

typedef struct MapAnimLightData {
    const char* name;
    s32 keyCount;
    MapAnimKey9 keys[1];
} MapAnimLightData;

typedef struct MapAnimLightData2 {
    const char* name;
    s32 keyCount;
    MapAnimKey5 keys[1];
} MapAnimLightData2;

/**
 * Applies a material animation (texture scroll, scale, rotation) at the animation's current time to texture
 * transform d->idx of the material, with the transform centre from the data.
 */
static inline void mapMainMatKeys_(MapAnim* a_, MapAnimMatData* d, MapMaterial* obj) {
    int n, c;
    f32 cur;
    for (n = 1; n < d->keyCount; n++) {
        MapAnimKey5* k0 = &d->keys[n - 1];
        MapAnimKey5* k1 = &d->keys[n];
        if (a_->time <= k1->time) {
            cur = a_->time;
        } else if (n == d->keyCount - 1) {
            cur = k1->time;
        } else {
            continue;
        }

        for (c = 0; c < 5; c++) {
            if (k0->ch[c].step) {
                obj->xforms[d->idx].ch[c] = k0->ch[c].value;
            } else {
                obj->xforms[d->idx].ch[c] = mapHermiteCh(cur, k0->time, k1->time, &k0->ch[c], &k1->ch[c]);
            }
        }
        obj->xforms[d->idx].cx = d->u;
        obj->xforms[d->idx].cy = d->v;
        break;
    }
}

/**
 * Applies an alpha animation at the animation's current time to the material,
 * stored as 255 minus 255 times the animated value in blendColor.r.
 */
static inline void mapMainAlphaKeys_(MapAnim* a_, MapAnimAlphaData* d, MapMaterial* obj) {
    int n;
    f32 cur;
    for (n = 1; n < d->keyCount; n++) {
        MapAnimKey4* k0 = &d->keys[n - 1];
        MapAnimKey4* k1 = &d->keys[n];

        // FAKE: cur is loaded before the test, which reads d->keys[n] instead of k1; the test of mapMainMatKeys_ changes the object
        cur = a_->time;
        if (cur <= d->keys[n].time) {
            cur = a_->time;
        } else if (n == d->keyCount - 1) {
            cur = k1->time;
        } else {
            continue;
        }

        if (k0->ch[0].step) {
            obj->blendColor.r = (u8)(255.0f - 255.0f * k0->ch[0].value);
        } else {
            obj->blendColor.r = (u8)(255.0f - 255.0f * mapHermiteCh(cur, k0->time, k1->time, &k0->ch[0], &k1->ch[0]));
        }

        break;
    }
}

/**
 * Applies a light animation at the animation's current time to the light's position (x10 to world scale)
 * and rotation.
 */
static inline void mapMainLight_ll(MapAnim* anim, LightEntry* lp, MapAnimLightData* d, f32* v) {
    int n, c;
    f32 cur;

    for (n = 1; n < d->keyCount; n++) {
        MapAnimKey9* k0 = &d->keys[n - 1];
        MapAnimKey9* k1 = &d->keys[n];
        if (anim->time <= k1->time) {
            cur = anim->time;
        } else if (n == d->keyCount - 1) {
            cur = k1->time;
        } else {
            continue;
        }
        for (c = 0; c < 9; c++) {
            if (k0->ch[c].step) {
                v[c] = k0->ch[c].value;
            } else {
                v[c] = mapHermiteCh(cur, k0->time, k1->time, &k0->ch[c], &k1->ch[c]);
            }
        }
        lp->position.x = 10.0f * v[0];
        lp->position.y = 10.0f * v[1];
        lp->position.z = 10.0f * v[2];
        lp->rotation.x = v[3];
        lp->rotation.y = v[4];
        lp->rotation.z = v[5];
        break;
    }
}

/**
 * Applies a light colour animation at the animation's current time to the light's colour,
 * spot angle and attenuation angle.
 */
static inline void mapMainLight_lc(MapAnim* anim, LightEntry* lp, MapAnimLightData2* d, f32* cv) {
    int n, c;
    f32 cur;

    for (n = 1; n < d->keyCount; n++) {
        MapAnimKey5* k0 = &d->keys[n - 1];
        MapAnimKey5* k1 = &d->keys[n];
        if (anim->time <= k1->time) {
            cur = anim->time;
        } else if (n == d->keyCount - 1) {
            cur = k1->time;
        } else {
            continue;
        }
        for (c = 0; c < 5; c++) {
            if (k0->ch[c].step) {
                cv[c] = k0->ch[c].value;
            } else {
                cv[c] = mapHermiteCh(cur, k0->time, k1->time, &k0->ch[c], &k1->ch[c]);
            }
        }
        lp->color.r = (u8)(255.0f * cv[0]);
        lp->color.g = (u8)(255.0f * cv[1]);
        lp->color.b = (u8)(255.0f * cv[2]);
        lp->color.a = 0xff;
        lp->spotAngle = cv[4];
        lp->attenAngle = cv[3];
        break;
    }
}

static inline MapMaterial* mapMainFindTbl_(const char* name) {
#if VERSION <= VERSION_DEMO_JP
    int j;
    int i;

    MapEntry* entry;
    MapWork* wp;
    MapMaterialName* mat;
    wp = mapGetWork();
#else
    MapEntry* entry;
    MapWork* wp;
    int j;
    MapMaterialName* mat;
    int i;

    wp = &mapWork[activeGroup];
#endif
    for (i = 0; i < wp->entries[0].count; i++) {
        entry = &wp->entries[i];
        if (!entry->materialNameTable) {
            return NULL;
        }
        for (j = 0; j < entry->materialNameTable->count; j++) {
            mat = &entry->materialNameTable->entries[j];
            if (!strcmp(mat->name, name)) {
                return mat->data;
            }
        }
    }
    return NULL;
}

// A macro, not an inline function: as a function mapMain changes (together with mapMainAlpha).
#define mapMainMat(a_, i_, b_, x_)           \
{                                            \
    MapMaterial* obj;                                 \
    strcpy(b_, (const char*)i_->name);       \
    if (x_) {                                \
        strcat(b_, x_);                      \
    }                                        \
    obj = mapMainFindTbl_(b_);               \
    if (obj) {                               \
        mapMainMatKeys_(a_, i_, obj); \
    }                                        \
}

#define mapMainAlpha(a_, i_, b_, x_)           \
{                                              \
    MapMaterial* obj;                                   \
    strcpy(b_, (const char*)i_->name);         \
    if (x_) {                                  \
        strcat(b_, x_);                        \
    }                                          \
    obj = mapMainFindTbl_(b_);                 \
    if (obj) {                                 \
        mapMainAlphaKeys_(a_, i_, obj); \
    }                                          \
}

/**
 * Per-frame map update from mariost: advances each playing map animation by its clock
 * and rate (looping or stopping at the end, handling pause and resume) and applies its joint, material, alpha,
 * light and light colour tracks, then recalculates the changed object matrices.
 */
void mapMain(void) {
    MapObject* o;
    f32 cur;
    MapWork* wp = &mapWork[activeGroup];
    MapEntry* entry;
    char mbuf[0x100];
    s32 i, j;
    s32 k;
    MapAnim* anim;
    BOOL changed;
    OSTime ms, t;
    char abuf[0x100];
    Mtx scale;
    f32 v[9];
    f32 cv[5];
#if VERSION <= VERSION_DEMO_JP
    for (i = 0; i < wp->entries[0].count; i++) {
        entry = &wp->entries[i];
#else
    entry = wp->entries;
    for (i = 0; i < wp->entries[0].count; entry++, i++) {
#endif
        changed = FALSE;
        anim = (MapAnim*)entry->animData;
        for (j = 0; j < entry->numAnims; j++, anim++) {
            MapAnimJointList* jl = anim->anim->jointAnims;
            MapAnimMatDataList* ml = anim->anim->materialAnims;
            MapAnimAlphaDataList* al = anim->anim->alphaAnims;
            MapAnimLightDataList* ll = anim->anim->lightAnims;
            MapAnimLightData2List* lc = anim->anim->lightColorAnims;

            if (!(anim->flags & MAP_ANIM_FLAG_ACTIVE) || (anim->flags & MAP_ANIM_FLAG_END)) {
                continue;
            }
            if (anim->flags & MAP_ANIM_FLAG_LV0_TIME) {
                t = gp->systemLv0Time;
            } else if (anim->flags & MAP_ANIM_FLAG_LV1_TIME) {
                t = gp->systemLv1Time;
            } else if (anim->flags & MAP_ANIM_FLAG_LV2_TIME) {
                t = gp->systemLv2Time;
            } else if (anim->flags & MAP_ANIM_FLAG_LV3_TIME) {
                t = gp->systemLv3Time;
            } else if (gp->inBattle) {
                t = gp->renderTime;
            } else {
                t = gp->renderFieldTime;
            }
            if (!(anim->flags & MAP_ANIM_FLAG_PAUSE)) {
                ms = OSTicksToMilliseconds(t);
#if VERSION >= VERSION_DEMO_EU
                cur = 60.0f * (f32)(u64)(ms - anim->startTime) / 1000.0f;
#else
                cur = gp->framerate * (f32)(u64)(ms - anim->startTime) / 1000.0f;
#endif
                if (cur < anim->time) {
                    anim->time = anim->time - (anim->time - cur);
                } else {
                    anim->time = anim->time + anim->rate * (cur - anim->time);
                }
                anim->startTime = OSTicksToMilliseconds(t);
                anim->startTime -= (u64)(1000.0f * anim->time / mapAnimFps());
            } else if (anim->flags & MAP_ANIM_FLAG_RESTART) {
                anim->startTime = OSTicksToMilliseconds(t);
                anim->startTime -= (u64)(1000.0f * anim->time / mapAnimFps());
                anim->flags &= ~MAP_ANIM_FLAG_RESTART;
                anim->flags &= ~MAP_ANIM_FLAG_PAUSE;
            }
            if (anim->time >= anim->anim->length) {
                if (anim->flags & MAP_ANIM_FLAG_LOOP) {
                    f32 t2 = anim->time;
                    while (t2 > anim->anim->length) {
                        t2 -= anim->anim->length;
                    }
                    anim->time = t2;
                    anim->startTime = OSTicksToMilliseconds(t);
                    anim->startTime -= (u64)(1000.0f * anim->time / mapAnimFps());
                } else {
                    anim->time = anim->anim->length;
                    anim->flags |= MAP_ANIM_FLAG_END;
                }
            }
            if (jl) {
                for (k = 0; k < jl->count; k++) {
                    o = mapGetMapObj((const char*)jl->items[k]->name);
                    if (o) {
                        mapCalcAnimMatrix(o->animMtx, o->localMtx, anim, jl->items[k]);
                        changed = TRUE;
                        o->flags |= MAP_OBJ_FLAG_ANIM_MTX;
                        o->flags |= MAP_OBJ_FLAG_MTX_DIRTY;
                    }
                }
            }
            if (ml) {
                for (k = 0; k < ml->count; k++) {
                    mapMainMat(anim, ml->items[k], mbuf, NULL);
                    mapMainMat(anim, ml->items[k], mbuf, "_v");
                    mapMainMat(anim, ml->items[k], mbuf, "_v_x");
                }
            }
            if (al) {
                for (k = 0; k < al->count; k++) {
                    mapMainAlpha(anim, al->items[k], abuf, NULL);
                    mapMainAlpha(anim, al->items[k], abuf, "_v");
                    mapMainAlpha(anim, al->items[k], abuf, "_v_x");
                }
            }
            if (ll) {
                for (k = 0; k < ll->count; k++) {
                    LightEntry* lp = lightNameToPtr((const char*)ll->items[k]->name);
                    if (lp) {
                        mapMainLight_ll(anim, lp, ll->items[k], v);
                    }
                }
            }
            if (lc) {
                for (k = 0; k < lc->count; k++) {
                    LightEntry* lp = lightNameToPtr((const char*)lc->items[k]->name);
                    if (lp) {
                        mapMainLight_lc(anim, lp, lc->items[k], cv);
                    }
                }
            }
        }
        if (changed) {
            PSMTXScale(scale, 10.0f, 10.0f, 10.0f);
            mapReCalcMatrix(entry->rootMapObj, scale, FALSE);
        }
    }
}

static void mapSetTextureMatrix(MapMaterial* mat);

void mapSetMaterialFogMapObj(MapObject* obj);

/**
 * TRUE when the object must go in the translucent pass:
 * its own colour (flag 0x40) or its material is not fully opaque,
 * or the map-wide blend colour (entry flag 2, unless object flag 0x800)
 * or second blend colour (entry flag 4, unless object flag 0x1000) is.
 */
static inline BOOL mapObjIsTranslucent(u32 flags, GXColor objColor, GXColor matColor) {
    if ((flags & MAP_OBJ_FLAG_COLOR) && objColor.a != 0xFF) {
        return TRUE;
    }
    if (matColor.a != 0xFF) {
        return TRUE;
    }
    if (!(flags & MAP_OBJ_FLAG_NO_MAP_BLEND) && (mapGetWork()->entries[0].flags & MAP_ENTRY_BLEND) && mapGetWork()->entries[0].blend.a != 0xFF) {
        return TRUE;
    }
    if (!(flags & MAP_OBJ_FLAG_NO_MAP_BLEND2) && (mapGetWork()->entries[0].flags & MAP_ENTRY_BLEND2) && mapGetWork()->entries[0].blend2.a != 0xFF) {
        return TRUE;
    }
    return FALSE;
}

/** Draws every packed display list of the mesh straight from the map file. */
static inline void callPackedLists(MapFileMesh* mesh) {
    int j;
    for (j = 0; j < mesh->displayListCount; j++) {
        GXCallDisplayList((void*)mesh->displayLists.packedLists[j].offset, mesh->displayLists.packedLists[j].size);
    }
}

/**
 * Draws one map object on camera cam: for objects drawn inside a group, sets the GX blend,
 * alpha test and z mode for its blend mode and (flag 0x8000) biases the depth projection by its index among its
 * siblings; then loads its matrices, cull mode and fog and draws each part from the prebuilt display lists
 * or by setting up the material and mesh. Skips hidden or fully transparent group members.
 */
static void _mapDispMapObj(CameraId cam, MapObject* obj) {
    CameraEntry* camera;
    MapFileJoint* joint;
    f32 scale;
    s32 n;
    s32 mode;
    GXCullMode cull;
    f32 d;
    Mtx mtx;
    Mtx nrm;
    f32 proj[7];
    MapFileMesh* mesh;
    int i;
    MapMaterial* material;

    camera = camGetPtr(cam);
    current_mp = obj;
    if (!(obj->flags & MAP_OBJ_FLAG_GROUP_MEMBER)) {
        goto setup;
    }
    if (obj->flags & MAP_OBJ_FLAG_DEPTH_OFFSET) {
        n = 0;
        for (joint = obj->joints; joint->prev; joint = joint->prev) {
        }

        for (; joint != obj->joints; joint = joint->next) {
            n++;
        }

        scale = 0.01f * n;
        GXGetProjectionv(proj);

        // d is set to 1.0f and used as a factor (the multiply by d is in the retail code)
        d = 1.0f;
        d = (d * (camera->far + camera->near)) * (d + scale);
        proj[5] *= 1.0f + ((scale * (((-2.0f) * camera->far) * camera->near)) / d);
        GXSetProjectionv(proj);
    }
    mode = obj->blendMode;
    if (obj->joints->partCount > 0) {
        if (mapObjIsTranslucent(obj->flags, obj->color, obj->joints->parts[0].material->blendColor)) {
            mode = DISP_MODE_XLU;
        }
    }
    switch (mode) {
        case 0:
            GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_OR);
            GXSetZCompLoc(GX_TRUE);
            GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
            GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
            break;

        case 3:
            GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_CLEAR);
            GXSetZCompLoc(GX_TRUE);
            GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
            GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
            break;

        case 1:
            GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_CLEAR);
            GXSetZCompLoc(GX_FALSE);
            GXSetAlphaCompare(GX_GEQUAL, 0x80, GX_AOP_OR, GX_NEVER, 0);
            GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
            break;

        case 4:
            GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_CLEAR);
            GXSetZCompLoc(GX_FALSE);
            GXSetAlphaCompare(GX_GEQUAL, 0x80, GX_AOP_OR, GX_NEVER, 0);
            GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
            break;

        case 2:
        case 5:
        default:
            GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
            GXSetZCompLoc(GX_TRUE);
            GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
            GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
            break;
    }

    setup:
    if (cam == CAMERA_OFFSCREEN2) {
        GXSetZCompLoc(GX_FALSE);
    }

    PSMTXConcat(camera->view, obj->modelWorldMtx, mtx);
    GXLoadPosMtxImm(mtx, GX_PNMTX0);
    if (!(obj->flags & MAP_OBJ_FLAG_DISPLAY_LIST)) {
        PSMTXInvXpose(mtx, nrm);
        GXLoadNrmMtxImm(nrm, GX_PNMTX0);
    }
    GXSetCurrentMtx(GX_PNMTX0);
    GXGetCullMode(&cull);
    if (cull != culling[obj->joints->drawMode->cullMode]) {
        GXSetCullMode(culling[obj->joints->drawMode->cullMode]);
    }
    mapSetMaterialFogMapObj(obj);

    // a group member is not drawn when it is disabled, hidden (and not offscreen) or fully transparent
    if (obj->flags & MAP_OBJ_FLAG_GROUP_MEMBER) {
        if (obj->flags & MAP_OBJ_FLAG_DISABLED) {
            goto end;
        }
        if ((obj->flags & MAP_OBJ_FLAG_HIDDEN) && (!(obj->flags & MAP_OBJ_FLAG_OFFSCREEN))) {
            goto end;
        }
        if ((obj->flags & MAP_OBJ_FLAG_COLOR) && (obj->color.a == 0)) {
            goto end;
        }
    }
    if (obj->flags & MAP_OBJ_FLAG_DISPLAY_LIST) {
        for (i = 0; i < obj->joints->partCount; i++) {
            mesh = obj->joints->parts[i].mesh;
            material = obj->joints->parts[i].material;
            mapSetTextureMatrix(material);
            mapSetMaterialLastStageBlend(obj->flags, obj->color, material->blendColor);
            mapSetPolygonVtxDesc(obj, mesh);
            GXCallDisplayList(obj->displayList[i], obj->displayListSize[i]);
            callPackedLists(mesh);
        }
    } else {
        for (i = 0; i < obj->joints->partCount; i++) {
            material = obj->joints->parts[i].material;
            mesh = obj->joints->parts[i].mesh;
            mapSetTextureMatrix(material);
            mapSetMaterialLastStageBlend(obj->flags, obj->color, material->blendColor);
            mapSetMaterial(obj, material);
            mapSetPolygon(obj, mesh);
        }
    }
    end:
    if (obj->flags & MAP_OBJ_FLAG_DEPTH_OFFSET) {
        GXSetProjection(camera->projection, camera->projType);
    }

    if (obj->flags & MAP_OBJ_TEV_CALLBACK_MASK) {
        GXSetTevDirect(GX_TEVSTAGE0);
        GXSetTevDirect(GX_TEVSTAGE1);
        GXSetTevDirect(GX_TEVSTAGE2);
        GXSetTevDirect(GX_TEVSTAGE3);
        GXSetTevDirect(GX_TEVSTAGE4);
        GXSetNumIndStages(0);
    }
}

/** Draws a group: the object, its children, and for group members (flag 0x20) the later siblings. */
static void _mapDispMapGrp(CameraId cam, MapObject* obj) {
    _mapDispMapObj(cam, obj);
    if (obj->child) {
        _mapDispMapGrp(cam, obj->child);
    }
    if ((obj->flags & MAP_OBJ_FLAG_GROUP_MEMBER) && obj->next) {
        _mapDispMapGrp(cam, obj->next);
    }
}

/** dispdrv callback that draws a single map object (queued by mapDisp). */
static void mapDispMapObj(CameraId cam, void* payload) {
    MapObject* obj = payload;
    _mapDispMapObj(cam, obj);
}

/** dispdrv callback that draws a group map object and its members (queued by mapDisp). */
static void mapDispMapGrp(CameraId cam, void* payload) {
    MapObject* obj = payload;
    _mapDispMapGrp(cam, obj);
}

/**
 * dispdrv callback for an object drawn into its offscreen buffer:
 * draws it and adds the screen rectangle it covered to that offscreen entry.
 */
static void mapDispMapObj_off(CameraId cam, void* payload) {
    MapObject* obj = payload;
    u16 left, top, right, bottom;

    sysWaitDrawSync();
    GXClearBoundingBox();
    _mapDispMapObj(cam, obj);
    sysWaitDrawSync();
    GXReadBoundingBox(&left, &top, &right, &bottom);
    offscreenAddBoundingBox(obj->offscreenId, left, top, right, bottom);
}

/** As mapDispMapObj_off for a whole group. */
static void mapDispMapGrp_off(CameraId cam, void* payload) {
    MapObject* obj = payload;
    u16 left, top, right, bottom;

    sysWaitDrawSync();
    GXClearBoundingBox();
    _mapDispMapGrp(cam, obj);
    sysWaitDrawSync();
    GXReadBoundingBox(&left, &top, &right, &bottom);
    offscreenAddBoundingBox(obj->offscreenId, left, top, right, bottom);
}

/**
 * Draws every map object with flag 0x400000 straight away, without bounding-box culling.
 * Named after the flipping floor panels (evt_mobj_kururing_floor flags the map group under a panel),
 * whose kururing_capture renders that floor from above into the panel's texture;
 * the sac_deka action command uses it the same way to photograph the stage floor.
 */
void test_kururing_mapdisp(CameraId cam) {
    MapWork* wp = &mapWork[activeGroup];
    MapEntry* entry = wp->entries;
    u32 flags;
    int j;
    int i;
    MapObject* obj;

    for (i = 0; i < wp->entries[0].count; entry++, i++) {
        obj = entry->objects;
        for (j = 0; j < entry->numJoints; j++, obj++) {
            flags = obj->flags;
            if (flags & MAP_OBJ_FLAG_KURURING) {
                obj->flags = flags & ~MAP_OBJ_FLAG_CLIP;
                _mapDispMapObj(cam, obj);
                obj->flags = flags;
            }
        }
    }
}

/**
 * Draws the object's meshes in flat black with no material, lighting or texture,
 * for measuring its screen rectangle.
 */
static void _mapDispMapObj_NoMaterial(CameraId cam, MapObject* obj) {
    Mtx mtx;
    Mtx nrm;
    int i;

    PSMTXConcat(camGetPtr(cam)->view, obj->modelWorldMtx, mtx);
    GXLoadPosMtxImm(mtx, GX_PNMTX0);
    PSMTXInvXpose(mtx, nrm);
    GXLoadNrmMtxImm(nrm, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    GXSetCullMode(GX_CULL_ALL);
    GXSetNumChans(1);
    GXSetChanMatColor(GX_COLOR0A0, (GXColor){0, 0, 0, 0});
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    for (i = 0; i < obj->joints->partCount; i++) {
        mapSetPolygon(obj, obj->joints->parts[i].mesh);
    }
}

/** _mapDispMapObj_NoMaterial over a group: the object, its children and, for group members, the later siblings. */
static void _mapDispMapGrp_NoMaterial(CameraId cam, MapObject* obj) {
    _mapDispMapObj_NoMaterial(cam, obj);
    if (obj->child) {
        _mapDispMapGrp_NoMaterial(cam, obj->child);
    }
    if ((obj->flags & MAP_OBJ_FLAG_GROUP_MEMBER) && obj->next) {
        _mapDispMapGrp_NoMaterial(cam, obj->next);
    }
}

/**
 * dispdrv callback for objects with flag 0x20000000:
 * draws the object untextured and stores the screen rectangle the GPU reports in obj->screenRect.
 */
static void mapDispMapObj_bbox(CameraId cam, void* payload) {
    MapObject* obj = payload;
    sysWaitDrawSync();
    GXClearBoundingBox();
    _mapDispMapObj_NoMaterial(cam, obj);
    sysWaitDrawSync();
    GXReadBoundingBox(&obj->screenRect[0], &obj->screenRect[1], &obj->screenRect[2], &obj->screenRect[3]);
}

/** As mapDispMapObj_bbox for a whole group. */
static void mapDispMapGrp_bbox(CameraId cam, void* payload) {
    MapObject* obj = payload;
    sysWaitDrawSync();
    GXClearBoundingBox();
    _mapDispMapGrp_NoMaterial(cam, obj);
    sysWaitDrawSync();
    GXReadBoundingBox(&obj->screenRect[0], &obj->screenRect[1], &obj->screenRect[2], &obj->screenRect[3]);
}

/**
 * Lookup tables of the map drawing: a map part's cull mode, texture matrix and light slots by index, its
 * texture wrap modes, and the GX texture and palette formats for the map file's codes (these two unused).
 */
const GXCullMode culling[4] = {GX_CULL_FRONT, GX_CULL_BACK, GX_CULL_ALL, GX_CULL_NONE};
const GXTexMtx texmtx_tbl[10] = {GX_TEXMTX0, GX_TEXMTX1, GX_TEXMTX2, GX_TEXMTX3, GX_TEXMTX4, GX_TEXMTX5, GX_TEXMTX6, GX_TEXMTX7, GX_TEXMTX8, GX_TEXMTX9};
const s32 texfmt_tbl[11] = {4, 5, 6, 14, 0, 1, 2, 3, 8, 9, 10};
const s32 texwrap_tbl[3] = {GX_CLAMP, GX_REPEAT, GX_MIRROR};
const s32 palfmt_tbl[2] = {1, 2};
const GXLightID lightid_tbl[8] = {GX_LIGHT0, GX_LIGHT1, GX_LIGHT2, GX_LIGHT3, GX_LIGHT4, GX_LIGHT5, GX_LIGHT6, GX_LIGHT7};
// Macro, not inline: an inline test at the eight sites changes the object.
#define MAP_CLIP_IN(v) ((v).z > -1.0f && (v).z < 0.0f && (v).x >= -1.0f && (v).x <= 1.0f && (v).y >= -1.0f && (v).y <= 1.0f)

/**
 * Per-frame map draw from mariost: recalculates the object matrices, skips hidden, empty,
 * group-member and fully transparent objects and those whose bounding box is out of view,
 * and queues each remaining object with dispdrv on its camera, sorted by projected depth,
 * plus its offscreen and screen-rectangle passes. Does nothing after mapDispOff.
 */
void mapDisp(void) {
    MapWork* wp = &mapWork[activeGroup];
    CameraEntry* camera = camGetPtr(CAMERA_3D);
    MapEntry* entry;
    MapObject* obj;
    int j, i;
    Mtx scale;
    Mtx mtx;
    Vec v;
    Vec center;
    f32 z;
    s32 mode;
    MapFileJoint* joint;
    BOOL clipped;
#if VERSION >= VERSION_JP
    if (disableMapDraw) {
        return;
    }
#endif
    entry = wp->entries;
    for (i = 0; i < wp->entries[0].count; entry++, i++) {
        if (entry->rootMapObj != NULL) {
            PSMTXScale(scale, 10.0f, 10.0f, 10.0f);
            mapReCalcMatrix(entry->rootMapObj, scale, FALSE);
        }
        obj = entry->objects;
        for (j = 0; j < entry->numJoints; j++, obj++) {
            if (obj->flags & MAP_OBJ_FLAG_DISABLED) continue;
            if ((obj->flags & MAP_OBJ_FLAG_HIDDEN) && !(obj->flags & MAP_OBJ_FLAG_OFFSCREEN)) continue;
            if ((obj->flags & MAP_OBJ_FLAG_COLOR) && obj->color.a == 0) continue;
            if (obj->flags & MAP_OBJ_FLAG_SCREEN_RECT) {
                obj->screenRect[2] = 0;
                obj->screenRect[3] = 0;
                obj->screenRect[0] = 0;
                obj->screenRect[1] = 0;
            }
            if (obj->flags & MAP_OBJ_FLAG_GROUP_MEMBER) continue;
            if (!(obj->flags & MAP_OBJ_FLAG_GROUP) && obj->joints->partCount < 1) continue;
            if (!(obj->flags & MAP_OBJ_FLAG_NO_MAP_BLEND) && (entry->flags & MAP_ENTRY_BLEND) && entry->blend.a == 0) continue;
            if (!(obj->flags & MAP_OBJ_FLAG_NO_MAP_BLEND2) && (entry->flags & MAP_ENTRY_BLEND2) && entry->blend2.a == 0) continue;
            PSMTXConcat(camera->view, obj->modelWorldMtx, mtx);
            if ((obj->flags & MAP_OBJ_FLAG_CLIP) && !mapClipOffFlag) {
                joint = obj->joints;
                clipped = FALSE;

                // the eight bounding-box corners, unrolled: the first one inside the view skips to `vis`
                v.x = joint->bboxMax.x;
                v.y = joint->bboxMax.y;
                v.z = joint->bboxMax.z;
                PSMTXMultVec(mtx, &v, &v);
                PSMTX44MultVec(camera->projection, &v, &v);
                if (MAP_CLIP_IN(v)) goto vis;
                v.x = joint->bboxMin.x;
                v.y = joint->bboxMin.y;
                v.z = joint->bboxMin.z;
                PSMTXMultVec(mtx, &v, &v);
                PSMTX44MultVec(camera->projection, &v, &v);
                if (MAP_CLIP_IN(v)) goto vis;
                v.x = joint->bboxMin.x;
                v.y = joint->bboxMax.y;
                v.z = joint->bboxMax.z;
                PSMTXMultVec(mtx, &v, &v);
                PSMTX44MultVec(camera->projection, &v, &v);
                if (MAP_CLIP_IN(v)) goto vis;
                v.x = joint->bboxMax.x;
                v.y = joint->bboxMin.y;
                v.z = joint->bboxMax.z;
                PSMTXMultVec(mtx, &v, &v);
                PSMTX44MultVec(camera->projection, &v, &v);
                if (MAP_CLIP_IN(v)) goto vis;
                v.x = joint->bboxMax.x;
                v.y = joint->bboxMax.y;
                v.z = joint->bboxMin.z;

                // BUG: this corner is transformed by mtx twice
                PSMTXMultVec(mtx, &v, &v);
                PSMTXMultVec(mtx, &v, &v);
                PSMTX44MultVec(camera->projection, &v, &v);
                if (MAP_CLIP_IN(v)) goto vis;
                v.x = joint->bboxMin.x;
                v.y = joint->bboxMin.y;
                v.z = joint->bboxMax.z;
                PSMTXMultVec(mtx, &v, &v);
                PSMTX44MultVec(camera->projection, &v, &v);
                if (MAP_CLIP_IN(v)) goto vis;
                v.x = joint->bboxMax.x;
                v.y = joint->bboxMin.y;
                v.z = joint->bboxMin.z;
                PSMTXMultVec(mtx, &v, &v);
                PSMTX44MultVec(camera->projection, &v, &v);
                if (MAP_CLIP_IN(v)) goto vis;
                v.x = joint->bboxMin.x;
                v.y = joint->bboxMax.y;
                v.z = joint->bboxMin.z;
                PSMTXMultVec(mtx, &v, &v);
                PSMTX44MultVec(camera->projection, &v, &v);
                if (MAP_CLIP_IN(v)) goto vis;
                clipped = TRUE;
            vis:
                if (clipped) continue;
            }
            PSMTXMultVec(mtx, &obj->center, &center);
            PSMTX44MultVec(camera->projection, &center, &center);
            z = 5000.0f * center.z + 5000.0f;
            if (z < 0.0f) {
                z = 0.0f;
            } else if (z > 10000.0f) {
                z = 10000.0f;
            }
            z *= -1.0f;
            mode = obj->blendMode;
            if (obj->joints->partCount > 0) {
                if (mapObjIsTranslucent(obj->flags, obj->color,
                        obj->joints->parts[0].material->blendColor)) {
                    mode = DISP_MODE_XLU;
                }
            }
            if (obj->flags & MAP_OBJ_FLAG_SCREEN_RECT) {
                dispEntry(CAMERA_OFFSCREEN2, 5000.0f, DISP_MODE_OPAQUE_NO_Z, (!(obj->flags & MAP_OBJ_FLAG_GROUP) ? mapDispMapObj_bbox : mapDispMapGrp_bbox), obj);
            }
            if (obj->flags & MAP_OBJ_FLAG_OFFSCREEN) {
                dispEntry(CAMERA_OFFSCREEN2, z, mode, (!(obj->flags & MAP_OBJ_FLAG_GROUP) ? mapDispMapObj_off : mapDispMapGrp_off), obj);
            }
            if (!(obj->flags & MAP_OBJ_FLAG_HIDDEN)) {
                dispEntry(obj->cameraId, z, mode, (!(obj->flags & MAP_OBJ_FLAG_GROUP) ? mapDispMapObj : mapDispMapGrp), obj);
            }
        }
    }
}

/**
 * Fills lights (up to 8) with the lights for obj:
 * the map lights picked by its draw mode's light mask,
 * then (draw mode flag 8) the nearest other lights to its centre; returns how many.
 */
static s32 mapSetLight(MapObject* obj, LightEntry** lights) {
    Vec pos;
    s32 count;
    s32 i;
    LightEntry* light;
    MapJointDrawMode* dm;

    count = 0;
    PSMTXMultVec(obj->modelWorldMtx, &obj->center, &pos);
    dm = obj->joints->drawMode;
    if (dm->modeFlags & DRAW_MODE_MAP_LIGHTS) {
        if (!dm->lightMask) {
            count = lightGetNearObj(&pos, lights, 8, 0);
        } else {
            for (i = 0; i < 32; i++) {
                if (obj->joints->drawMode->lightMask & (1 << i)) {
                    light = lightNameToPtr(mapWork[activeGroup].entries[obj->groupIndex].lightTable->lights[i]->name);
                    if (!(light->flags & LIGHT_AMBIENT)) {
                        lights[count] = light;
                        count++;
                        if (count >= 8) {
                            break;
                        }
                    }
                }
            }
            if (count < 8) {
                count += lightGetNearObj(&pos, &lights[count], 8 - count, 0);
            }
        }
    } else if (dm->lightMask) {
        for (i = 0; i < 32; i++) {
            if (obj->joints->drawMode->lightMask & (1 << i)) {
                light = lightNameToPtr(mapWork[activeGroup].entries[obj->groupIndex].lightTable->lights[i]->name);
                if (!(light->flags & LIGHT_AMBIENT)) {
                    lights[count] = light;
                    count++;
                    if (count >= 8) {
                        break;
                    }
                }
            }
        }
    } else {
        count = 0;
    }
    return count;
}

static u32 texture_0[64] ATTRIBUTE_ALIGN(32) = {
    0xEB88E228, 0x55FFAA00, 0x00000000, 0x00000000,
    0xF528EC08, 0x55FFAA00, 0x00000000, 0x00000000,
    0xFEE8F5A8, 0x55FFAA00, 0x00000000, 0x00000000,
    0xFF47D6E8, 0x00AAFF55, 0x00000000, 0x00000000,
    0xBEA8860A, 0x00AAFF55, 0x00000000, 0x00000000,
    0x7DEA0D8B, 0x00AAFF55, 0x00000000, 0x00000000,
    0x056B054A, 0x00AAAA00, 0x00000000, 0x00000000,
    0x056B054E, 0x0000AAFF, 0x00000000, 0x00000000,
    0x05BA0570, 0x55FFAAAA, 0x00000000, 0x00000000,
    0x05DE05BA, 0x55FFAAAA, 0x00000000, 0x00000000,
    0x05BD051B, 0x0000AA55, 0x00000000, 0x00000000,
    0x1376049A, 0x55FFAA00, 0x00000000, 0x00000000,
    0x33154253, 0x00AA5555, 0x00000000, 0x00000000,
    0x6A333A53, 0xFFFFAA00, 0x00000000, 0x00000000,
    0xB1F28212, 0x55FFAA00, 0x00000000, 0x00000000,
    0xE9F2C212, 0x55FFAA00, 0x00000000, 0x00000000,
};

#pragma push
// FAKE: lifetimes optimisation off for mapSetMaterialTev only; with it on the object changes
#pragma opt_lifetimes off
/**
 * Sets up the GX TEV stages for a map or model material:
 * the texture combine for tevMode drawMode, then the colour stage (kcolor 0 from mapSetMaterialLastStageBlend)
 * and optional extra stages selected by flags: Mario's projected shadow, the projected texture,
 * depth and projected shadows, the flash colour, a pulsing glow, a shimmer texture and a per-map TEV callback.
 * Also used by animdrv for models.
 */
void mapSetMaterialTev(s32 texCount, s32 drawMode, s32 flags, Mtx mtx) {
    static f32 scrl_uv[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    GXTexMapID map;
    GXTexCoordID coord;
    s32 tevStage;
    s32 indStages;
    s32 i;
    Mtx queen_mtx;

    map = (GXTexMapID)texCount;
    coord = (GXTexCoordID)texCount;
    indStages = 0;

    if ((s32)texCount > 0) {
        switch (drawMode) {
            case 0:
                GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
                GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
                GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
                tevStage = GX_TEVSTAGE1;
                break;

            case 1:
                GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
                GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
                GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
                GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
                GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
                GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_CPREV, GX_CC_TEXC, GX_CC_TEXA, GX_CC_ZERO);
                GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
                GXSetTevSwapMode(GX_TEVSTAGE1, GX_TEV_SWAP0, GX_TEV_SWAP0);
                GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
                GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_ZERO, GX_CC_CPREV, GX_CC_RASC, GX_CC_ZERO);
                GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_APREV, GX_CA_RASA, GX_CA_ZERO);
                GXSetTevSwapMode(GX_TEVSTAGE2, GX_TEV_SWAP0, GX_TEV_SWAP0);
                tevStage = GX_TEVSTAGE3;
                break;

            case 2:
                GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
                GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
                GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
                GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
                GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
                GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_CPREV, GX_CC_TEXA, GX_CC_ZERO);
                GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_APREV, GX_CA_TEXA, GX_CA_ZERO);
                GXSetTevSwapMode(GX_TEVSTAGE1, GX_TEV_SWAP0, GX_TEV_SWAP0);
                GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
                GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_ZERO, GX_CC_CPREV, GX_CC_RASC, GX_CC_ZERO);
                GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_APREV, GX_CA_RASA, GX_CA_ZERO);
                GXSetTevSwapMode(GX_TEVSTAGE2, GX_TEV_SWAP0, GX_TEV_SWAP0);
                tevStage = GX_TEVSTAGE3;
                break;

            case 12:
                GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
                GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
                GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
                GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
                GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
                GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_CPREV);
                GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
                GXSetTevSwapMode(GX_TEVSTAGE1, GX_TEV_SWAP0, GX_TEV_SWAP0);
                GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
                GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_ZERO, GX_CC_CPREV, GX_CC_RASC, GX_CC_ZERO);
                GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_APREV, GX_CA_RASA, GX_CA_ZERO);
                GXSetTevSwapMode(GX_TEVSTAGE2, GX_TEV_SWAP0, GX_TEV_SWAP0);
                tevStage = GX_TEVSTAGE3;
                break;

            case 3:
                GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
                GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
                GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
                GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
                GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
                GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_CPREV, GX_CC_ZERO, GX_CC_TEXA, GX_CC_ZERO);
                GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_APREV, GX_CA_ZERO, GX_CA_TEXA, GX_CA_ZERO);
                GXSetTevSwapMode(GX_TEVSTAGE1, GX_TEV_SWAP0, GX_TEV_SWAP0);
                GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
                GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_ZERO, GX_CC_CPREV, GX_CC_RASC, GX_CC_ZERO);
                GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_APREV, GX_CA_RASA, GX_CA_ZERO);
                GXSetTevSwapMode(GX_TEVSTAGE2, GX_TEV_SWAP0, GX_TEV_SWAP0);
                tevStage = GX_TEVSTAGE3;
                break;

            case 4:
                GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
                GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
                GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
                GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
                GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
                GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_CPREV, GX_CC_TEXC, GX_CC_ZERO);
                GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_APREV, GX_CA_TEXA, GX_CA_ZERO);
                GXSetTevSwapMode(GX_TEVSTAGE1, GX_TEV_SWAP0, GX_TEV_SWAP0);
                GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
                GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_TEXC, GX_CC_CPREV, GX_CC_APREV, GX_CC_ZERO);
                GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
                GXSetTevSwapMode(GX_TEVSTAGE2, GX_TEV_SWAP0, GX_TEV_SWAP0);
                tevStage = GX_TEVSTAGE3;
                break;

            case 5:
                GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
                GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVREG0);
                GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVREG0);
                GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
                GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
                GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
                GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
                GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVREG1);
                GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVREG1);
                GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
                GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
                GXSetTevSwapMode(GX_TEVSTAGE1, GX_TEV_SWAP0, GX_TEV_SWAP0);
                GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD2, GX_TEXMAP2, GX_COLOR_NULL);
                GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_C0, GX_CC_C1, GX_CC_TEXA, GX_CC_ZERO);
                GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_A0, GX_CA_A1, GX_CA_TEXA, GX_CA_ZERO);
                GXSetTevSwapMode(GX_TEVSTAGE2, GX_TEV_SWAP0, GX_TEV_SWAP0);
                GXSetTevOrder(GX_TEVSTAGE3, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
                GXSetTevColorOp(GX_TEVSTAGE3, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevAlphaOp(GX_TEVSTAGE3, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevColorIn(GX_TEVSTAGE3, GX_CC_ZERO, GX_CC_CPREV, GX_CC_RASC, GX_CC_ZERO);
                GXSetTevAlphaIn(GX_TEVSTAGE3, GX_CA_ZERO, GX_CA_APREV, GX_CA_RASA, GX_CA_ZERO);
                GXSetTevSwapMode(GX_TEVSTAGE3, GX_TEV_SWAP0, GX_TEV_SWAP0);
                tevStage = GX_TEVSTAGE4;
                break;

            case 6:
                GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
                GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
                GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
                GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
                GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
                GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_CPREV, GX_CC_TEXC, GX_CC_APREV, GX_CC_ZERO);
                GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_TEXA, GX_CA_APREV, GX_CA_ZERO);
                GXSetTevSwapMode(GX_TEVSTAGE1, GX_TEV_SWAP0, GX_TEV_SWAP0);
                GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
                GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_ZERO, GX_CC_CPREV, GX_CC_RASC, GX_CC_ZERO);
                GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_APREV, GX_CA_RASA, GX_CA_ZERO);
                GXSetTevSwapMode(GX_TEVSTAGE2, GX_TEV_SWAP0, GX_TEV_SWAP0);
                tevStage = GX_TEVSTAGE3;
                break;

            case 7:
                GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
                GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
                GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
                GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
                GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
                GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_CPREV, GX_CC_TEXC, GX_CC_TEXA, GX_CC_ZERO);
                GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_APREV, GX_CA_ZERO, GX_CA_TEXA, GX_CA_TEXA);
                GXSetTevSwapMode(GX_TEVSTAGE1, GX_TEV_SWAP0, GX_TEV_SWAP0);
                GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
                GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_ZERO, GX_CC_CPREV, GX_CC_RASC, GX_CC_ZERO);
                GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_APREV, GX_CA_RASA, GX_CA_ZERO);
                GXSetTevSwapMode(GX_TEVSTAGE2, GX_TEV_SWAP0, GX_TEV_SWAP0);
                tevStage = GX_TEVSTAGE3;
                break;

            case 8:
                GXSetTevColor(GX_TEVREG0, (GXColor){0, (u8)((127.0f * sinf((TWO_PI * (f32)(2 * gp->retraceCount)) / 360.0f)) + 127.0f), 0, 0});
                GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
                GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_RASC, GX_CC_TEXC, GX_CC_C0);
                GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_RASA, GX_CA_TEXA, GX_CA_A0);
                GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
                tevStage = GX_TEVSTAGE1;
                break;

            case 9:
                for (i = 0; i < 4; i++) {
                    scrl_uv[i] += queen_uv[i];
                    if (scrl_uv[i] > 100.0f) {
                        scrl_uv[i] -= 100.0f;
                    }
                    if (scrl_uv[i] < -100.0f) {
                        scrl_uv[i] += 100.0f;
                    }
                }

                MTXTrans(queen_mtx, scrl_uv[0], scrl_uv[1], 0.0f);
                GXLoadTexMtxImm(queen_mtx, GX_TEXMTX0, GX_MTX2x4);
                GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
                MTXTrans(queen_mtx, scrl_uv[2], scrl_uv[3], 0.0f);
                GXLoadTexMtxImm(queen_mtx, GX_TEXMTX1, GX_MTX2x4);
                GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX1, GX_TEXMTX1, GX_FALSE, GX_PTIDENTITY);
                GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
                GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
                GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
                GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
                GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
                GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_CPREV, GX_CC_TEXC, GX_CC_TEXA, GX_CC_ZERO);
                GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_APREV, GX_CA_ZERO, GX_CA_TEXA, GX_CA_TEXA);
                GXSetTevSwapMode(GX_TEVSTAGE1, GX_TEV_SWAP0, GX_TEV_SWAP0);
                GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
                GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_ZERO, GX_CC_CPREV, GX_CC_RASC, GX_CC_ZERO);
                GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_APREV, GX_CA_RASA, GX_CA_ZERO);
                GXSetTevSwapMode(GX_TEVSTAGE2, GX_TEV_SWAP0, GX_TEV_SWAP0);
                tevStage = GX_TEVSTAGE3;
                break;

            default:
                GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
                GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_RASC, GX_CC_TEXC, GX_CC_ZERO);
                GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_RASA, GX_CA_TEXA, GX_CA_ZERO);
                GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
                tevStage = GX_TEVSTAGE1;
                break;
        }
    } else {
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_RASC);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_RASA);
        GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
        tevStage = GX_TEVSTAGE1;
    }

    if (!(flags & MAP_OBJ_FLAG_NO_COLOR_STAGE)) {
        GXSetTevKColorSel(tevStage, GX_TEV_KCSEL_K0);
        GXSetTevKAlphaSel(tevStage, GX_TEV_KASEL_K0_A);
        GXSetTevOrder(tevStage, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
        GXSetTevColorOp(tevStage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(tevStage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(tevStage, GX_CC_ZERO, GX_CC_CPREV, GX_CC_KONST, GX_CC_ZERO);
        GXSetTevAlphaIn(tevStage, GX_CA_ZERO, GX_CA_APREV, GX_CA_KONST, GX_CA_ZERO);
        GXSetTevSwapMode(tevStage, GX_TEV_SWAP0, GX_TEV_SWAP0);
        tevStage++;
    }

    if ((flags & MAP_OBJ_FLAG_CHAR_SHADOW) && shadowGetCharShadowProjection() && shadowGetCharShadowTexObj()) {
        GXColor shColor = shadowGetCharShadowColor();
        Mtx shMtx;
        PSMTXConcat(shadowGetCharShadowProjTexMtx(), mtx, shMtx);
        GXSetTexCoordGen2((GXTexCoordID)coord, GX_TG_MTX3x4, GX_TG_POS, texmtx_tbl[map], GX_FALSE, GX_PTIDENTITY);
        GXLoadTexMtxImm(shMtx, texmtx_tbl[map], GX_MTX3x4);
        GXLoadTexObj(shadowGetCharShadowTexObj(), (GXTexMapID)map);
        GXSetTevOrder(tevStage, (GXTexCoordID)coord, (GXTexMapID)map, GX_COLOR_NULL);
        GXSetTevColorOp(tevStage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(tevStage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(tevStage, GX_CC_CPREV, GX_CC_KONST, GX_CC_TEXC, GX_CC_ZERO);
        GXSetTevAlphaIn(tevStage, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
        GXSetTevSwapMode(tevStage, GX_TEV_SWAP0, GX_TEV_SWAP0);
        GXSetTevKColor(GX_KCOLOR1, shColor);
        GXSetTevKColorSel(tevStage, GX_TEV_KCSEL_K1);
        tevStage++;
        map = (GXTexMapID)(map + 1);
        coord = (GXTexCoordID)(coord + 1);
    }

    if (flags & MAP_OBJ_FLAG_PROJ_TEX) {
        Mtx pjMtx;
        GXSetTevColorOp((GXTevStageID)(tevStage - 1), GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVREG0);
        GXSetTevAlphaOp((GXTevStageID)tevStage - 1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVREG0);
        PSMTXConcat(projMtx, mtx, pjMtx);
        GXSetTexCoordGen2((GXTexCoordID)coord, GX_TG_MTX3x4, GX_TG_POS, texmtx_tbl[map], GX_FALSE, GX_PTIDENTITY);
        GXLoadTexMtxImm(pjMtx, texmtx_tbl[map], GX_MTX3x4);
        GXLoadTexObj(smartTexObj(&projTexObj, projTexImgRef), (GXTexMapID)map);
        GXSetTevOrder(tevStage, (GXTexCoordID)coord, (GXTexMapID)map, GX_COLOR_NULL);
        GXSetTevColorOp(tevStage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(tevStage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_2, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(tevStage, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C1);
        GXSetTevAlphaIn(tevStage, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
        GXSetTevSwapMode(tevStage, GX_TEV_SWAP3, GX_TEV_SWAP3);
        GXSetTevSwapModeTable(GX_TEV_SWAP3, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_RED);
        GXSetTevColor(GX_TEVREG1, (GXColor){0, 0, 0, 0xFF});
        map = (GXTexMapID)(map + 1);
        coord = (GXTexCoordID)(coord + 1);
        GXSetTevOrder((GXTevStageID)(tevStage + 1), GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
        GXSetTevColorOp((GXTevStageID)(tevStage + 1), GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp((GXTevStageID)(tevStage + 1), GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn((GXTevStageID)(tevStage + 1), GX_CC_C0, GX_CC_CPREV, GX_CC_APREV, GX_CC_ZERO);
        GXSetTevAlphaIn((GXTevStageID)(tevStage + 1), GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A0);
        GXSetTevSwapMode((GXTevStageID)(tevStage + 1), GX_TEV_SWAP0, GX_TEV_SWAP0);
        tevStage += 2;
    }

    if ((flags & MAP_OBJ_FLAG_DEPTH_SHADOW) && shadowGetDepthShadowTexObj()) {
        GXTexObj* depthTex = shadowGetDepthShadowTexObj();
        MtxPtr depthMtx = shadowGetDepthShadowDepthMtx();
        MtxPtr projTexMtx = shadowGetDepthShadowProjTexMtx();
        GXColor depthColor = shadowGetDepthShadowColor();
        Mtx dsMtx;

        switch ((s32)shadowGetDepthShadowConfig()->format) {
            case 0:
                GXSetTevColorOp((GXTevStageID)(tevStage - 1), GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVREG0);
                GXSetTevAlphaOp((GXTevStageID)tevStage - 1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVREG0);
                GXSetTevOrder(tevStage, (GXTexCoordID)coord, (GXTexMapID)map, GX_COLOR_NULL);
                GXSetTevColorIn(tevStage, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
                GXSetTevColorOp(tevStage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevAlphaIn(tevStage, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
                GXSetTevAlphaOp(tevStage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevSwapMode(tevStage, GX_TEV_SWAP0, GX_TEV_SWAP0);
                GXSetTexCoordGen2((GXTexCoordID)coord, GX_TG_MTX3x4, GX_TG_POS, texmtx_tbl[map], GX_FALSE, GX_PTIDENTITY);
                GXLoadTexObj(shadowGetRamp8TexObj(), (GXTexMapID)map);
                PSMTXConcat(depthMtx, mtx, dsMtx);
                GXLoadTexMtxImm(dsMtx, texmtx_tbl[map], GX_MTX3x4);
                map = (GXTexMapID)(map + 1);
                GXSetTevOrder((GXTevStageID)(tevStage + 1), (GXTexCoordID)(coord + 1), (GXTexMapID)map, GX_COLOR_NULL);
                GXSetTevColorIn((GXTevStageID)(tevStage + 1), GX_CC_CPREV, GX_CC_TEXC, GX_CC_ONE, GX_CC_ZERO);
                GXSetTevColorOp((GXTevStageID)(tevStage + 1), GX_TEV_COMP_R8_GT, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
                GXSetTevAlphaIn((GXTevStageID)(tevStage + 1), GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
                GXSetTevAlphaOp((GXTevStageID)(tevStage + 1), GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevSwapMode((GXTevStageID)(tevStage + 1), GX_TEV_SWAP0, GX_TEV_SWAP0);
                GXSetTexCoordGen2((GXTexCoordID)(coord + 1), GX_TG_MTX3x4, GX_TG_POS, texmtx_tbl[map], GX_FALSE, GX_PTIDENTITY);
                GXLoadTexObj(depthTex, (GXTexMapID)map);
                PSMTXConcat(projTexMtx, mtx, dsMtx);
                GXLoadTexMtxImm(dsMtx, texmtx_tbl[map], GX_MTX3x4);
                map = (GXTexMapID)(map + 1);
                coord = (GXTexCoordID)(coord + 2);
                GXSetTevOrder((GXTevStageID)(tevStage + 2), GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
                GXSetTevColorIn((GXTevStageID)(tevStage + 2), GX_CC_ZERO, GX_CC_KONST, GX_CC_CPREV, GX_CC_C0);
                GXSetTevColorOp((GXTevStageID)(tevStage + 2), GX_TEV_SUB, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevKColor(GX_KCOLOR1, depthColor);
                GXSetTevKColorSel((GXTevStageID)(tevStage + 2), GX_TEV_KCSEL_K1);
                GXSetTevAlphaIn((GXTevStageID)(tevStage + 2), GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A0);
                GXSetTevAlphaOp((GXTevStageID)(tevStage + 2), GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevSwapMode((GXTevStageID)(tevStage + 2), GX_TEV_SWAP0, GX_TEV_SWAP0);
                tevStage += 3;
                break;

            case 1:
                GXSetTevColor(GX_TEVREG1, (GXColor){0xFF, 0x00, 0x00, 0x00});
                GXSetTevOrder(tevStage, (GXTexCoordID)coord, (GXTexMapID)map, GX_COLOR_NULL);
                GXSetTevColorIn(tevStage, GX_CC_ZERO, GX_CC_TEXC, GX_CC_C1, GX_CC_ZERO);
                GXSetTevColorOp(tevStage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevAlphaIn(tevStage, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
                GXSetTevAlphaOp(tevStage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevSwapMode(tevStage, GX_TEV_SWAP0, GX_TEV_SWAP0);
                GXSetTexCoordGen2((GXTexCoordID)coord, GX_TG_MTX3x4, GX_TG_POS, texmtx_tbl[map], GX_FALSE, GX_PTIDENTITY);
                GXLoadTexObj(shadowGetRamp8TexObj(), (GXTexMapID)map);
                PSMTXConcat(depthMtx, mtx, dsMtx);
                GXLoadTexMtxImm(dsMtx, texmtx_tbl[map], GX_MTX3x4);
                tevStage++;
                map = (GXTexMapID)(map + 1);
                coord = (GXTexCoordID)(coord + 1);
                break;

            case 2:
                GXSetTevColor(GX_TEVREG1, (GXColor){0xFF, 0x00, 0x00, 0x00});
                GXSetTevOrder(tevStage, (GXTexCoordID)coord, (GXTexMapID)map, GX_COLOR_NULL);
                GXSetTevColorIn(tevStage, GX_CC_ZERO, GX_CC_TEXC, GX_CC_C1, GX_CC_ZERO);
                GXSetTevColorOp(tevStage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevAlphaIn(tevStage, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
                GXSetTevAlphaOp(tevStage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevSwapMode(tevStage, GX_TEV_SWAP0, GX_TEV_SWAP0);
                GXSetTexCoordGen2((GXTexCoordID)coord, GX_TG_MTX3x4, GX_TG_POS, texmtx_tbl[map], GX_FALSE, GX_PTIDENTITY);
                GXLoadTexObj(depthTex, (GXTexMapID)map);
                PSMTXConcat(projTexMtx, mtx, dsMtx);
                GXLoadTexMtxImm(dsMtx, texmtx_tbl[map], GX_MTX3x4);
                tevStage++;
                map = (GXTexMapID)(map + 1);
                coord = (GXTexCoordID)(coord + 1);
                break;

            case 3:
                GXSetTevColorOp((GXTevStageID)(tevStage - 1), GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVREG0);
                GXSetTevAlphaOp((GXTevStageID)tevStage - 1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVREG0);
                GXSetTevSwapModeTable(GX_TEV_SWAP1, GX_CH_RED, GX_CH_ALPHA, GX_CH_BLUE, GX_CH_ALPHA);
                GXSetTevSwapModeTable(GX_TEV_SWAP2, GX_CH_ALPHA, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
                GXSetTevOrder(tevStage, (GXTexCoordID)coord, (GXTexMapID)map, GX_COLOR_NULL);
                GXSetTevColorIn(tevStage, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
                GXSetTevColorOp(tevStage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevAlphaIn(tevStage, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
                GXSetTevAlphaOp(tevStage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevSwapMode(tevStage, GX_TEV_SWAP0, GX_TEV_SWAP1);
                GXSetTexCoordGen2((GXTexCoordID)coord, GX_TG_MTX3x4, GX_TG_POS, texmtx_tbl[map], GX_FALSE, GX_PTIDENTITY);
                GXLoadTexObj(shadowGetRamp16TexObj(), (GXTexMapID)map);
                PSMTXConcat(depthMtx, mtx, dsMtx);
                GXLoadTexMtxImm(dsMtx, texmtx_tbl[map], GX_MTX3x4);
                map = (GXTexMapID)(map + 1);
                GXSetTevOrder((GXTevStageID)(tevStage + 1), (GXTexCoordID)(coord + 1), (GXTexMapID)map, GX_COLOR_NULL);
                GXSetTevColorIn((GXTevStageID)(tevStage + 1), GX_CC_CPREV, GX_CC_TEXC, GX_CC_ONE, GX_CC_ZERO);
                GXSetTevColorOp((GXTevStageID)(tevStage + 1), GX_TEV_COMP_GR16_GT, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
                GXSetTevAlphaIn((GXTevStageID)(tevStage + 1), GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
                GXSetTevAlphaOp((GXTevStageID)(tevStage + 1), GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevSwapMode((GXTevStageID)(tevStage + 1), GX_TEV_SWAP0, GX_TEV_SWAP2);
                GXSetTexCoordGen2((GXTexCoordID)(coord + 1), GX_TG_MTX3x4, GX_TG_POS, texmtx_tbl[map], GX_FALSE, GX_PTIDENTITY);
                GXLoadTexObj(depthTex, (GXTexMapID)map);
                PSMTXConcat(projTexMtx, mtx, dsMtx);
                GXLoadTexMtxImm(dsMtx, texmtx_tbl[map], GX_MTX3x4);
                map = (GXTexMapID)(map + 1);
                coord = (GXTexCoordID)(coord + 2);
                GXSetTevOrder((GXTevStageID)(tevStage + 2), GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
                GXSetTevColorIn((GXTevStageID)(tevStage + 2), GX_CC_ZERO, GX_CC_KONST, GX_CC_CPREV, GX_CC_C0);
                GXSetTevColorOp((GXTevStageID)(tevStage + 2), GX_TEV_SUB, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevKColor(GX_KCOLOR1, depthColor);
                GXSetTevKColorSel((GXTevStageID)(tevStage + 2), GX_TEV_KCSEL_K1);
                GXSetTevAlphaIn((GXTevStageID)(tevStage + 2), GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A0);
                GXSetTevAlphaOp((GXTevStageID)(tevStage + 2), GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevSwapMode((GXTevStageID)(tevStage + 2), GX_TEV_SWAP0, GX_TEV_SWAP0);
                tevStage += 3;
                break;

            case 4:
                GXSetTevColor(GX_TEVREG1, (GXColor){0xFF, 0xFF, 0x00, 0x00});
                GXSetTevSwapModeTable(GX_TEV_SWAP1, GX_CH_ALPHA, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
                GXSetTevOrder(tevStage, (GXTexCoordID)coord, (GXTexMapID)map, GX_COLOR_NULL);
                GXSetTevColorIn(tevStage, GX_CC_ZERO, GX_CC_TEXC, GX_CC_C1, GX_CC_ZERO);
                GXSetTevColorOp(tevStage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevAlphaIn(tevStage, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
                GXSetTevAlphaOp(tevStage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevSwapMode(tevStage, GX_TEV_SWAP0, GX_TEV_SWAP1);
                GXSetTexCoordGen2((GXTexCoordID)coord, GX_TG_MTX3x4, GX_TG_POS, texmtx_tbl[map], GX_FALSE, GX_PTIDENTITY);
                GXLoadTexObj(shadowGetRamp16TexObj(), (GXTexMapID)map);
                PSMTXConcat(depthMtx, mtx, dsMtx);
                GXLoadTexMtxImm(dsMtx, texmtx_tbl[map], GX_MTX3x4);
                tevStage++;
                map = (GXTexMapID)(map + 1);
                coord = (GXTexCoordID)(coord + 1);
                break;

            case 5:
                GXSetTevColor(GX_TEVREG1, (GXColor){0xFF, 0xFF, 0x00, 0x00});
                GXSetTevSwapModeTable(GX_TEV_SWAP1, GX_CH_RED, GX_CH_ALPHA, GX_CH_BLUE, GX_CH_ALPHA);
                GXSetTevOrder(tevStage, (GXTexCoordID)coord, (GXTexMapID)map, GX_COLOR_NULL);
                GXSetTevColorIn(tevStage, GX_CC_ZERO, GX_CC_TEXC, GX_CC_C1, GX_CC_ZERO);
                GXSetTevColorOp(tevStage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevAlphaIn(tevStage, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
                GXSetTevAlphaOp(tevStage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevSwapMode(tevStage, GX_TEV_SWAP0, GX_TEV_SWAP1);
                GXSetTexCoordGen2((GXTexCoordID)coord, GX_TG_MTX3x4, GX_TG_POS, texmtx_tbl[map], GX_FALSE, GX_PTIDENTITY);
                GXLoadTexObj(depthTex, (GXTexMapID)map);
                PSMTXConcat(projTexMtx, mtx, dsMtx);
                GXLoadTexMtxImm(dsMtx, texmtx_tbl[map], GX_MTX3x4);
                tevStage++;
                map = (GXTexMapID)(map + 1);
                coord = (GXTexCoordID)(coord + 1);
                break;
        }
    }

    if ((flags & MAP_OBJ_FLAG_PROJ_SHADOW) && shadowGetProjShadowTexObj()) {
        GXTexObj* projTex = shadowGetProjShadowTexObj();
        MtxPtr projTexMtx = shadowGetProjShadowProjTexMtx();
        GXColor projColor = shadowGetProjShadowColor();
        Mtx psMtx;

        shadowGetProjShadowConfig();
        GXSetTevOrder(tevStage, (GXTexCoordID)coord, (GXTexMapID)map, GX_COLOR_NULL);
        GXSetTevColorIn(tevStage, GX_CC_CPREV, GX_CC_KONST, GX_CC_TEXA, GX_CC_ZERO);
        GXSetTevColorOp(tevStage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaIn(tevStage, GX_CA_ZERO, GX_CA_KONST, GX_CA_TEXA, GX_CA_APREV);
        GXSetTevAlphaOp(tevStage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevSwapMode(tevStage, GX_TEV_SWAP0, GX_TEV_SWAP0);
        GXSetTexCoordGen2((GXTexCoordID)coord, GX_TG_MTX3x4, GX_TG_POS, texmtx_tbl[map], GX_FALSE, GX_PTIDENTITY);
        GXLoadTexObj(projTex, (GXTexMapID)map);
        PSMTXConcat(projTexMtx, mtx, psMtx);
        GXLoadTexMtxImm(psMtx, texmtx_tbl[map], GX_MTX3x4);
        GXSetTevKColor(GX_KCOLOR1, projColor);
        GXSetTevKColorSel(tevStage, GX_TEV_KCSEL_K1);
        GXSetTevKAlphaSel(tevStage, GX_TEV_KASEL_K1_A);
        tevStage++;
        map = (GXTexMapID)(map + 1);
        coord = (GXTexCoordID)(coord + 1);
    }

    if ((flags & MAP_OBJ_FLAG_DEPTH_SHADOW_2) && shadowGetDepthShadowTexObj()) {
        GXTexObj* depthTex = shadowGetDepthShadowTexObj();
        MtxPtr depthMtx = shadowGetDepthShadowDepthMtx();
        MtxPtr projTexMtx = shadowGetDepthShadowProjTexMtx();
        GXColor depthColor = shadowGetDepthShadowColor();
        Mtx dsMtx;

        switch ((s32)shadowGetDepthShadowConfig()->format) {
            case 0:
                GXSetTevColorOp((GXTevStageID)(tevStage - 1), GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVREG0);
                GXSetTevAlphaOp((GXTevStageID)tevStage - 1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVREG0);
                GXSetTevOrder(tevStage, (GXTexCoordID)coord, (GXTexMapID)map, GX_COLOR_NULL);
                GXSetTevColorIn(tevStage, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
                GXSetTevColorOp(tevStage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevAlphaIn(tevStage, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
                GXSetTevAlphaOp(tevStage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevSwapMode(tevStage, GX_TEV_SWAP0, GX_TEV_SWAP0);
                GXSetTexCoordGen2((GXTexCoordID)coord, GX_TG_MTX3x4, GX_TG_POS, texmtx_tbl[map], GX_FALSE, GX_PTIDENTITY);
                GXLoadTexObj(shadowGetRamp8TexObj(), (GXTexMapID)map);
                PSMTXConcat(depthMtx, mtx, dsMtx);
                GXLoadTexMtxImm(dsMtx, texmtx_tbl[map], GX_MTX3x4);
                map = (GXTexMapID)(map + 1);
                GXSetTevOrder((GXTevStageID)(tevStage + 1), (GXTexCoordID)(coord + 1), (GXTexMapID)map, GX_COLOR_NULL);
                GXSetTevColorIn((GXTevStageID)(tevStage + 1), GX_CC_CPREV, GX_CC_TEXC, GX_CC_ONE, GX_CC_ZERO);
                GXSetTevColorOp((GXTevStageID)(tevStage + 1), GX_TEV_COMP_R8_GT, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
                GXSetTevAlphaIn((GXTevStageID)(tevStage + 1), GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
                GXSetTevAlphaOp((GXTevStageID)(tevStage + 1), GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevSwapMode((GXTevStageID)(tevStage + 1), GX_TEV_SWAP0, GX_TEV_SWAP0);
                GXSetTexCoordGen2((GXTexCoordID)(coord + 1), GX_TG_MTX3x4, GX_TG_POS, texmtx_tbl[map], GX_FALSE, GX_PTIDENTITY);
                GXLoadTexObj(depthTex, (GXTexMapID)map);
                PSMTXConcat(projTexMtx, mtx, dsMtx);
                GXLoadTexMtxImm(dsMtx, texmtx_tbl[map], GX_MTX3x4);
                map = (GXTexMapID)(map + 1);
                coord = (GXTexCoordID)(coord + 2);
                GXSetTevOrder((GXTevStageID)(tevStage + 2), GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
                GXSetTevColorIn((GXTevStageID)(tevStage + 2), GX_CC_C0, GX_CC_KONST, GX_CC_CPREV, GX_CC_ZERO);
                GXSetTevColorOp((GXTevStageID)(tevStage + 2), GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevAlphaIn((GXTevStageID)(tevStage + 2), GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
                GXSetTevAlphaOp((GXTevStageID)(tevStage + 2), GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevSwapMode((GXTevStageID)(tevStage + 2), GX_TEV_SWAP0, GX_TEV_SWAP0);
                GXSetTevKColor(GX_KCOLOR1, depthColor);
                GXSetTevKColorSel((GXTevStageID)(tevStage + 2), GX_TEV_KCSEL_K1);
                GXSetTevKAlphaSel((GXTevStageID)(tevStage + 2), GX_TEV_KASEL_K1_A);
                tevStage += 3;
                break;

            case 3:
                GXSetTevColorOp((GXTevStageID)(tevStage - 1), GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVREG0);
                GXSetTevAlphaOp((GXTevStageID)tevStage - 1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVREG0);
                GXSetTevSwapModeTable(GX_TEV_SWAP1, GX_CH_RED, GX_CH_ALPHA, GX_CH_BLUE, GX_CH_ALPHA);
                GXSetTevSwapModeTable(GX_TEV_SWAP2, GX_CH_ALPHA, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
                GXSetTevOrder(tevStage, (GXTexCoordID)coord, (GXTexMapID)map, GX_COLOR_NULL);
                GXSetTevColorIn(tevStage, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
                GXSetTevColorOp(tevStage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevAlphaIn(tevStage, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
                GXSetTevAlphaOp(tevStage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevSwapMode(tevStage, GX_TEV_SWAP0, GX_TEV_SWAP1);
                GXSetTexCoordGen2((GXTexCoordID)coord, GX_TG_MTX3x4, GX_TG_POS, texmtx_tbl[map], GX_FALSE, GX_PTIDENTITY);
                GXLoadTexObj(shadowGetRamp16TexObj(), (GXTexMapID)map);
                PSMTXConcat(depthMtx, mtx, dsMtx);
                GXLoadTexMtxImm(dsMtx, texmtx_tbl[map], GX_MTX3x4);
                map = (GXTexMapID)(map + 1);
                GXSetTevOrder((GXTevStageID)(tevStage + 1), (GXTexCoordID)(coord + 1), (GXTexMapID)map, GX_COLOR_NULL);
                GXSetTevColorIn((GXTevStageID)(tevStage + 1), GX_CC_CPREV, GX_CC_TEXC, GX_CC_ONE, GX_CC_ZERO);
                GXSetTevColorOp((GXTevStageID)(tevStage + 1), GX_TEV_COMP_GR16_GT, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
                GXSetTevAlphaIn((GXTevStageID)(tevStage + 1), GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
                GXSetTevAlphaOp((GXTevStageID)(tevStage + 1), GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevSwapMode((GXTevStageID)(tevStage + 1), GX_TEV_SWAP0, GX_TEV_SWAP2);
                GXSetTexCoordGen2((GXTexCoordID)(coord + 1), GX_TG_MTX3x4, GX_TG_POS, texmtx_tbl[map], GX_FALSE, GX_PTIDENTITY);
                GXLoadTexObj(depthTex, (GXTexMapID)map);
                PSMTXConcat(projTexMtx, mtx, dsMtx);
                GXLoadTexMtxImm(dsMtx, texmtx_tbl[map], GX_MTX3x4);
                map = (GXTexMapID)(map + 1);
                coord = (GXTexCoordID)(coord + 2);
                GXSetTevOrder((GXTevStageID)(tevStage + 2), GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
                GXSetTevColorIn((GXTevStageID)(tevStage + 2), GX_CC_C0, GX_CC_KONST, GX_CC_CPREV, GX_CC_ZERO);
                GXSetTevColorOp((GXTevStageID)(tevStage + 2), GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevAlphaIn((GXTevStageID)(tevStage + 2), GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
                GXSetTevAlphaOp((GXTevStageID)(tevStage + 2), GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevSwapMode((GXTevStageID)(tevStage + 2), GX_TEV_SWAP0, GX_TEV_SWAP0);
                GXSetTevKColor(GX_KCOLOR1, depthColor);
                GXSetTevKColorSel((GXTevStageID)(tevStage + 2), GX_TEV_KCSEL_K1);
                GXSetTevKAlphaSel((GXTevStageID)(tevStage + 2), GX_TEV_KASEL_K1_A);
                tevStage += 3;
                break;
        }
    }

    if (flags & (MAP_OBJ_FLAG_PULSE | MAP_OBJ_FLAG_FLUSH)) {
        s32 r, g, b, a;
        f32 pulse;

        if (flags & MAP_OBJ_FLAG_FLUSH) {
            r = current_mp->color.r;
            g = current_mp->color.g;
            b = current_mp->color.b;
            a = current_mp->color.a;
        } else {
            r = 0xF0;
            g = 0xF0;
            b = 0x64;
            pulse = cos((1.5707964f * gp->retraceCount) / 20.0f);
            a = (u8)(64.0f * (1.0f - pulse) / 2.0f + 64.0f);
        }
        GXSetTevOrder(tevStage, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
        GXSetTevColorOp(tevStage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(tevStage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(tevStage, GX_CC_CPREV, GX_CC_ZERO, GX_CC_ZERO, GX_CC_KONST);
        GXSetTevAlphaIn(tevStage, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
        GXSetTevSwapMode(tevStage, GX_TEV_SWAP0, GX_TEV_SWAP0);
        GXSetTevKColor(GX_KCOLOR1, (GXColor){r * a / 255, g * a / 255, b * a / 255, 0xFF});
        GXSetTevKColorSel(tevStage, GX_TEV_KCSEL_K1);
        tevStage++;
    }

    if (flags & MAP_OBJ_FLAG_WAVE_TEX) {
        Vec camPos = {0.0f, 0.0f, 1.0f};
        Vec camUp = {0.0f, 1.0f, 0.0f};
        Vec target = {0.0f, 0.0f, 0.0f};
        GXTexObj waveTex;
        Mtx texMtx;
        Mtx lookMtx;
        s32 level;
        f32 pulse;

        pulse = cos((1.5707964f * gp->retraceCount) / 30.0f);
        level = (127.0f * (1.0f - pulse)) / 2.0f + 128.0f;
        GXInitTexObj(&waveTex, texture_0, 4, 64, GX_TF_CMPR, GX_CLAMP, GX_REPEAT, GX_FALSE);
        GXLoadTexObj(&waveTex, (GXTexMapID)map);
        C_MTXLightOrtho(texMtx, -2.0f, 2.0f, -2.0f, 2.0f, 0.5f, 0.5f, 0.5f, gp->retraceCount / 50.0f);
        C_MTXLookAt(lookMtx, &camPos, &camUp, &target);
        PSMTXConcat(texMtx, lookMtx, texMtx);
        GXLoadTexMtxImm(texMtx, texmtx_tbl[map], GX_MTX3x4);
        GXSetTexCoordGen2((GXTexCoordID)coord, GX_TG_MTX3x4, GX_TG_POS, texmtx_tbl[map], GX_FALSE, GX_PTIDENTITY);
        GXSetTevOrder(tevStage, (GXTexCoordID)coord, (GXTexMapID)map, GX_COLOR_NULL);
        GXSetTevColorOp(tevStage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(tevStage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(tevStage, GX_CC_TEXC, GX_CC_CPREV, GX_CC_KONST, GX_CC_ZERO);
        GXSetTevAlphaIn(tevStage, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
        GXSetTevSwapMode(tevStage, GX_TEV_SWAP0, GX_TEV_SWAP0);
        GXSetTevKColor(GX_KCOLOR1, (GXColor){level, level, level, 0xFF});
        GXSetTevKColorSel(tevStage, GX_TEV_KCSEL_K1);
        tevStage++;
        map = (GXTexMapID)(map + 1);
        coord = (GXTexCoordID)(coord + 1);
    }

    if (flags & MAP_OBJ_TEV_CALLBACK_MASK) {
        MapTevCallbackInfo info;
        MapTevCallback callback;

        MapTevCallbackInfo* ip = &info;
        MapWork* mw;

        info.mapObj = current_mp;
        info.mtx = mtx;
        info.drawMode = drawMode;
        info.tevStage = tevStage;
        info.texMap = map;
        info.texCoord = coord;
        info.indStage = 0;

        mw = &mapWork[activeGroup];
        callback = mw->entries[0].tevCallbacks[(u32)(flags & MAP_OBJ_TEV_CALLBACK_MASK) >> 24];
        if (callback) {
            callback(ip);
        }
        tevStage = info.tevStage;
        coord = info.texCoord;
        indStages = info.indStage;
    }

    GXSetNumTexGens((u8)coord);
    GXSetNumTevStages((u8)tevStage);
    GXSetNumIndStages((u8)indStages);
}
#pragma pop

/**
 * Sets TEV kcolor 0, the colour the last stage multiplies by:
 * white scaled by the object colour (flag 0x40),
 * the material alpha (color2.r) and the map-wide blend colours and colour unless the object opts out.
 * Shared with animdrv.
 */
void mapSetMaterialLastStageBlend(s32 flags, GXColor color1, GXColor color2) {
    MapWork* wp = mapGetWork();
    GXColor color = {0xFF, 0xFF, 0xFF, 0xFF};

    if (flags & MAP_OBJ_FLAG_COLOR) {
        color.r = color.r * color1.r / 255;
        color.g = color.g * color1.g / 255;
        color.b = color.b * color1.b / 255;
        color.a = color.a * color1.a / 255;
    }
    if (color2.r != 0xFF) {
        color.a = color.a * color2.r / 255;
    }
    if ((wp->entries[0].flags & MAP_ENTRY_BLEND) && !(flags & MAP_OBJ_FLAG_NO_MAP_BLEND)) {
        color.r = color.r * wp->entries[0].blend.r / 255;
        color.g = color.g * wp->entries[0].blend.g / 255;
        color.b = color.b * wp->entries[0].blend.b / 255;
        color.a = color.a * wp->entries[0].blend.a / 255;
    }
    if ((wp->entries[0].flags & MAP_ENTRY_BLEND2) && !(flags & MAP_OBJ_FLAG_NO_MAP_BLEND2)) {
        color.r = color.r * wp->entries[0].blend2.r / 255;
        color.g = color.g * wp->entries[0].blend2.g / 255;
        color.b = color.b * wp->entries[0].blend2.b / 255;
        color.a = color.a * wp->entries[0].blend2.a / 255;
    }
    if ((wp->entries[0].flags & MAP_ENTRY_COLOR) && !(flags & MAP_OBJ_FLAG_NO_MAP_BLEND)) {
        color.r = color.r * wp->entries[0].color.r / 255;
        color.g = color.g * wp->entries[0].color.g / 255;
        color.b = color.b * wp->entries[0].color.b / 255;
        color.a = color.a * wp->entries[0].color.a / 255;
    }
    GXSetTevKColor(GX_KCOLOR0, color);
}

// .sdata: defined after mapSetMaterialTev so it follows that function's static scrl_uv in .sdata
s32 fog_type[5] = {2, 4, 5, 6, 7};

/**
 * Sets the GX fog to the map's fog (fog_table or mapSetFog) when map fog is on, else turns fog off;
 * animdrv uses it for models.
 */
void mapSetMaterialFog(void) {
    CameraEntry* camera = camGetCurPtr();
    MapWork* wp = mapGetWork();

    if (wp->entries[0].flags & MAP_ENTRY_FOG) {
        GXSetFog((GXFogType)fog_type[wp->entries[0].fogType], wp->entries[0].fogStart, wp->entries[0].fogEnd,
                 camera->near, camera->far, wp->entries[0].fogColor);
    } else {
        GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
    }
}

/** As mapSetMaterialFog for a map object, which can opt out of the fog with flag 0x80000. */
// Original name from the JP demo's link map (UNUSED there): every call is inlined, so the linker strips it.
void mapSetMaterialFogMapObj(MapObject* obj) {
    CameraEntry* camera = camGetCurPtr();
    MapWork* wp = mapGetWork();

    if ((wp->entries[0].flags & MAP_ENTRY_FOG) && !(obj->flags & MAP_OBJ_FLAG_NO_FOG)) {
        GXSetFog((GXFogType)fog_type[wp->entries[0].fogType], wp->entries[0].fogStart, wp->entries[0].fogEnd,
                 camera->near, camera->far, wp->entries[0].fogColor);
    } else {
        GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
    }
}

/**
 * Loads the texture coordinate generation for each sampler of mat,
 * with a texture matrix for the animated scroll,
 * scale and rotation (about the transform centre) when it is not the identity.
 */
static void mapSetTextureMatrix(MapMaterial* mat) {
    s32 j;
    s32 k;
    s32 i;
    f32 rot, sx, sy, tx, ty;
    f32 cx, cy;
    Mtx mtx, tmpMtx, transMtx, rotMtx, scaleMtx;

    j = 0;
    k = 0;
    for (i = mat->samplerCount - 1; i >= 0; j++, k++, i--) {
        sx = mat->xforms[i].sx;
        rot = -mat->xforms[i].rot;
        sy = mat->xforms[i].sy;
        ty = -mat->xforms[i].ty;
        tx = mat->xforms[i].tx;
        if (1.0f != mat->xforms[i].sx || 1.0f != mat->xforms[i].sy || 0.0f != tx || 0.0f != ty || 0.0f != rot) {
            if (rot) {
                cx = 0.5f * mat->xforms[i].cx * mat->xforms[i].sx;
                cy = (1.0f - 0.5f * mat->xforms[i].cy) * mat->xforms[i].sy;
                MTXTrans(mtx, cx, cy, 0.0f);
                MTXRotRad(rotMtx, 'z', MTXDegToRad(rot));
                MTXTrans(tmpMtx, -cx, -cy, 0.0f);
                MTXConcat(mtx, rotMtx, rotMtx);
                MTXConcat(rotMtx, tmpMtx, rotMtx);
            }
            if (sx || sy) {
                MTXScale(scaleMtx, sx, sy, 1.0f);
            }
            if (tx || ty) {
                MTXTrans(transMtx, tx, ty, 0.0f);
            }
            if (rot) {
                if (sx || sy) {
                    MTXConcat(rotMtx, scaleMtx, mtx);
                    if (tx || ty) {
                        MTXConcat(mtx, transMtx, mtx);
                    }
                } else if (tx || ty) {
                    MTXConcat(rotMtx, transMtx, mtx);
                } else {
                    MTXCopy(rotMtx, mtx);
                }
            } else if (sx || sy) {
                if (tx || ty) {
                    MTXConcat(scaleMtx, transMtx, mtx);
                } else {
                    MTXCopy(scaleMtx, mtx);
                }
            } else if (tx || ty) {
                MTXCopy(transMtx, mtx);
            }
            GXLoadTexMtxImm(mtx, texmtx_tbl[i], GX_MTX2x4);
            GXSetTexCoordGen2((GXTexCoordID)j, GX_TG_MTX2x4, (GXTexGenSrc)(mat->samplerCount + 3 - k), texmtx_tbl[i], GX_FALSE, GX_PTIDENTITY);
        } else {
            GXSetTexCoordGen2((GXTexCoordID)j, GX_TG_MTX2x4, (GXTexGenSrc)(mat->samplerCount + 3 - k), GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
        }
    }
}

/**
 * Sets up GX for drawing one part of a map object with material:
 * up to 8 lights (point, spot, directional) with the ambient light, the material colour and lighting mode,
 * the textures from the map's texture.tpl, then the TEV stages.
 */
static void mapSetMaterial(MapObject* obj, void* material) {
    MapMaterial* mat = material;
    GXColor ambient;
    GXColor ambColor = {0x00, 0x00, 0x00, 0xFF};
    s32 count;
    s32 texIdx;
    u32 lightMask;
    s32 attnFn;
    s32 i;
    LightEntry* light;
    MapWork* wp;
    MapMatSampler* sampler;
    s32 k;
    TEXDescriptor* desc;
    GXBool mip;
    GXLightObj lightObj;
    Mtx rotX, rotY, rotZ;
    LightEntry* lights[8];
    GXTexObj texObj;
    Vec pos;

    texIdx = 0;
    attnFn = GX_AF_NONE;
    count = mapSetLight(obj, lights);
    lightMask = 0;
    for (i = 0; i < count; i++) {
        Vec dir = {0.0f, 0.0f, -1.0f};

        light = lights[i];
        if (light->flags & LIGHT_POINT) {
            MTXMultVec(camGetCurPtr()->view, &light->position, &pos);
            GXInitLightPos(&lightObj, pos.x, pos.y, pos.z);
            GXInitLightColor(&lightObj, lights[i]->color);
            GXInitLightAttn(&lightObj, lights[i]->attenAngle, 0.0f, 0.0f, lights[i]->attenDistCoef.x, lights[i]->attenDistCoef.y, lights[i]->attenDistCoef.z);
            GXLoadLightObjImm(&lightObj, lightid_tbl[i]);
            attnFn = GX_AF_SPOT;
        } else if (light->flags & LIGHT_DIRECTIONAL) {
            MTXRotRad(rotX, 'x', MTXDegToRad(light->rotation.x));
            MTXRotRad(rotY, 'y', MTXDegToRad(lights[i]->rotation.y));
            MTXRotRad(rotZ, 'z', MTXDegToRad(lights[i]->rotation.z));
            MTXConcat(rotZ, rotY, rotY);
            MTXConcat(rotY, rotX, rotX);
            MTXMultVec(rotX, &dir, &dir);
            VECScale(&dir, &dir, -1048576.0f);
            VECAdd(&lights[i]->position, &dir, &pos);
            MTXMultVec(camGetCurPtr()->view, &pos, &pos);
            GXInitLightPos(&lightObj, pos.x, pos.y, pos.z);
            GXInitLightColor(&lightObj, lights[i]->color);
            GXInitLightAttn(&lightObj, lights[i]->attenAngle, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
            GXLoadLightObjImm(&lightObj, lightid_tbl[i]);
        } else if (light->flags & LIGHT_SPOT) {
            MTXMultVec(camGetCurPtr()->view, &light->position, &pos);
            MTXRotRad(rotX, 'x', MTXDegToRad(lights[i]->rotation.x));
            MTXRotRad(rotY, 'y', MTXDegToRad(lights[i]->rotation.y));
            MTXRotRad(rotZ, 'z', MTXDegToRad(lights[i]->rotation.z));
            MTXConcat(rotZ, rotY, rotY);
            MTXConcat(rotY, rotX, rotX);
            MTXMultVec(rotX, &dir, &dir);
            MTXMultVecSR(camGetCurPtr()->view, &dir, &dir);
            GXInitLightPos(&lightObj, pos.x, pos.y, pos.z);
            GXInitLightDir(&lightObj, dir.x, dir.y, dir.z);
            GXInitLightSpot(&lightObj, lights[i]->spotAngle / 2, GX_SP_SHARP);
            GXInitLightDistAttn(&lightObj, 1000.0f, 0.5f, GX_DA_OFF);
            GXInitLightColor(&lightObj, lights[i]->color);
            GXLoadLightObjImm(&lightObj, lightid_tbl[i]);
            attnFn = GX_AF_SPOT;
        }
        lightMask |= lightid_tbl[i];
    }

    light = lightGetAmbient();
    if (light) {
        ambient = light->color;
    } else {
        ambient = ambColor;
    }
    GXSetNumChans(1);
    GXSetChanAmbColor(GX_COLOR0A0, ambient);
    GXSetChanMatColor(GX_COLOR0A0, mat->color);
    switch (mat->lightMode) {
        case 0:
            GXSetChanCtrl(GX_COLOR0, count > 0, GX_SRC_REG, GX_SRC_REG, lightMask, GX_DF_CLAMP, (GXAttnFn)attnFn);
            GXSetChanCtrl(GX_ALPHA0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
            break;
        case 1:
            GXSetChanCtrl(GX_COLOR0, count > 0, GX_SRC_REG, GX_SRC_VTX, lightMask, GX_DF_CLAMP, (GXAttnFn)attnFn);
            GXSetChanCtrl(GX_ALPHA0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
            break;
    }

    for (i = mat->samplerCount - 1; i >= 0; i--) {
        sampler = mat->samplers[i];
        wp = mapGetWork();
        k = mapGetTextureIdx(wp->entries[obj->groupIndex].textureTable, sampler->tex->name);
        desc = TEXGet(wp->entries[obj->groupIndex].tpl, k);
        if (desc->textureHeader->minLOD == desc->textureHeader->maxLOD) {
            mip = GX_FALSE;
        } else {
            mip = GX_TRUE;
        }
        GXInitTexObj(&texObj, desc->textureHeader->data, desc->textureHeader->width, desc->textureHeader->height, (GXTexFmt)desc->textureHeader->format,
                     (GXTexWrapMode)texwrap_tbl[sampler->wrapS], (GXTexWrapMode)texwrap_tbl[sampler->wrapT],
                     mip);
        GXInitTexObjLOD(&texObj, GX_LINEAR, GX_LINEAR, desc->textureHeader->minLOD, desc->textureHeader->maxLOD, desc->textureHeader->LODBias, GX_FALSE, desc->textureHeader->edgeLODEnable, GX_ANISO_1);
        GXLoadTexObj(&texObj, (GXTexMapID)texIdx);
        texIdx++;
    }
    mapSetMaterialTev(mat->samplerCount, mat->tevMode ? *mat->tevMode : 0, obj->flags, obj->modelWorldMtx);
}

/**
 * Sets the ambient colour used with the paper light for models (mapSetMaterialLight flag 1);
 * mot_slit sets it for Mario's Paper Mode.
 */
void mapSetPaperAmbColor(GXColor* color) {
    paper_ambient = *color;
}

/** Resets the paper ambient colour to grey 0x70; done on every map load and by mot_slit. */
void mapResetPaperAmbColor(void) {
    paper_ambient = (GXColor){0x70, 0x70, 0x70, 0xFF};
}

/** Resets the paper-craft light's ambient colour to grey 0x70; done on every map load. */
static void mapResetPaperCraftAmbColor(void) {
    paperCraft_ambient = (GXColor){0x70, 0x70, 0x70, 0xFF};
}

/**
 * Sets up the GX lighting channel for an animdrv model at position:
 * flags 1 and 0x10 add the paper light, 8 the paper-craft light, 2 the nearest character lights;
 * flag 4 lights a white material colour instead of the vertex colours. Fog is turned off.
 */
void mapSetMaterialLight(s32 flags, Vec position) {
    GXColor matColor = {0xFF, 0xFF, 0xFF, 0xFF};
    GXColor ambColor = {0x00, 0x00, 0x00, 0xFF};
    s32 count;
    u32 lightMask;
    u32 paper;
    s32 attnFn;
    s32 i;
    u32 paperCraft;
    LightEntry* light;
    GXLightObj lightObj;
    Mtx rotX, rotY, rotZ;
    LightEntry* lights[8];
    GXColor ambient;
    Vec pos;
    f32 ang;

    count = 0;
    attnFn = GX_AF_NONE;
    paper = flags & MATERIAL_LIGHT_PAPER;
    if (paper) {
        lights[0] = lightGetPaper();
        count = 1;
    }
    paperCraft = flags & MATERIAL_LIGHT_PAPER_CRAFT;
    if (paperCraft) {
        lights[count] = lightGetPaperCraft();
        count++;
    }
    if (flags & MATERIAL_LIGHT_PAPER_PLAIN) {
        lights[count] = lightGetPaper();
        count++;
    }
    if ((flags & MATERIAL_LIGHT_CHARA) && lightCheckCharaLight() && count < 8) {
        count += lightGetNearObj(&position, &lights[count], 8 - count, LIGHT_CHARA);
    }
    if (flags & (MATERIAL_LIGHT_PAPER | MATERIAL_LIGHT_PAPER_CRAFT | MATERIAL_LIGHT_PAPER_PLAIN)) {
        if (count == 1) {
            if (paper) {
                ambColor = paper_ambient;
            } else if (paperCraft) {
                ambColor = paperCraft_ambient;
            } else {
                ambColor = (GXColor){0x00, 0x00, 0x00, 0xFF};
            }
        } else {
            count = lightGetNearObj(&position, lights, 8, LIGHT_CHARA);
        }
    }

    lightMask = 0;
    for (i = 0; i < count; i++) {
        Vec dir = {0.0f, 0.0f, -1.0f};

        light = lights[i];
        if (light->flags & LIGHT_POINT) {
            MTXMultVec(camGetCurPtr()->view, &light->position, &pos);
            GXInitLightPos(&lightObj, pos.x, pos.y, pos.z);
            GXInitLightColor(&lightObj, lights[i]->color);
            if (lights[i]->flags & LIGHT_NO_DIST_ATTEN) {
                GXInitLightAttn(&lightObj, lights[i]->attenAngle, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
            } else {
                GXInitLightAttn(&lightObj, lights[i]->attenAngle, 0.0f, 0.0f, lights[i]->attenDistCoef.x, lights[i]->attenDistCoef.y, lights[i]->attenDistCoef.z);
                attnFn = GX_AF_SPOT;
            }
            GXLoadLightObjImm(&lightObj, lightid_tbl[i]);
        } else if (light->flags & LIGHT_DIRECTIONAL) {
            MTXRotRad(rotX, 'x', MTXDegToRad(light->rotation.x));
            MTXRotRad(rotY, 'y', MTXDegToRad(lights[i]->rotation.y));
            MTXRotRad(rotZ, 'z', MTXDegToRad(lights[i]->rotation.z));
            MTXConcat(rotZ, rotY, rotY);
            MTXConcat(rotY, rotX, rotX);
            MTXMultVec(rotX, &dir, &dir);
            VECScale(&dir, &dir, -1048576.0f);
            VECAdd(&lights[i]->position, &dir, &pos);
            MTXMultVec(camGetCurPtr()->view, &pos, &pos);
            GXInitLightPos(&lightObj, pos.x, pos.y, pos.z);
            GXInitLightColor(&lightObj, lights[i]->color);
            GXLoadLightObjImm(&lightObj, lightid_tbl[i]);
        } else if (light->flags & LIGHT_SPOT) {
            MTXMultVec(camGetCurPtr()->view, &light->position, &pos);
            MTXRotRad(rotX, 'x', MTXDegToRad(lights[i]->rotation.x));
            MTXRotRad(rotY, 'y', MTXDegToRad(lights[i]->rotation.y));
            MTXRotRad(rotZ, 'z', MTXDegToRad(lights[i]->rotation.z));
            MTXConcat(rotZ, rotY, rotY);
            MTXConcat(rotY, rotX, rotX);
            MTXMultVec(rotX, &dir, &dir);
            MTXMultVecSR(camGetCurPtr()->view, &dir, &dir);
            GXInitLightPos(&lightObj, pos.x, pos.y, pos.z);
            GXInitLightDir(&lightObj, dir.x, dir.y, dir.z);

            ang = lights[i]->spotAngle;
            ang *= 0.5f;
            GXInitLightSpot(&lightObj, ang, GX_SP_SHARP);
            GXInitLightDistAttn(&lightObj, 1000.0f, 0.5f, GX_DA_OFF);
            GXInitLightColor(&lightObj, lights[i]->color);
            GXLoadLightObjImm(&lightObj, lightid_tbl[i]);
            attnFn = GX_AF_SPOT;
        }
        lightMask |= lightid_tbl[i];
    }

    light = lightGetCharaAmbient();
    if (light) {
        ambient = light->color;
    } else {
        ambient = ambColor;
    }
    GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, matColor);
    GXSetNumChans(1);
    GXSetChanAmbColor(GX_COLOR0A0, ambient);
    if (flags & MATERIAL_LIGHT_MAT_COLOR) {
        GXSetChanMatColor(GX_COLOR0A0, matColor);
        GXSetChanCtrl(GX_COLOR0, count > 0, GX_SRC_REG, GX_SRC_REG, lightMask, GX_DF_CLAMP, (GXAttnFn)attnFn);
        GXSetChanCtrl(GX_ALPHA0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
    } else {
        GXSetChanCtrl(GX_COLOR0, count > 0, GX_SRC_REG, GX_SRC_VTX, lightMask, GX_DF_CLAMP, (GXAttnFn)attnFn);
        GXSetChanCtrl(GX_ALPHA0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
    }
}

/** Vertex formats of map geometry (float positions, normals and texture coordinates, RGBA8 colours). */
const GXVtxAttrFmtList attr_fmt[12] = {
    {GX_VA_POS, GX_POS_XYZ, GX_F32, 0},
    {GX_VA_NRM, GX_NRM_XYZ, GX_F32, 0},
    {GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0},
    {GX_VA_TEX0, GX_TEX_ST, GX_F32, 0},
    {GX_VA_TEX1, GX_TEX_ST, GX_F32, 0},
    {GX_VA_TEX2, GX_TEX_ST, GX_F32, 0},
    {GX_VA_TEX3, GX_TEX_ST, GX_F32, 0},
    {GX_VA_TEX4, GX_TEX_ST, GX_F32, 0},
    {GX_VA_TEX5, GX_TEX_ST, GX_F32, 0},
    {GX_VA_TEX6, GX_TEX_ST, GX_F32, 0},
    {GX_VA_TEX7, GX_TEX_ST, GX_F32, 0},
    {GX_VA_NULL, GX_POS_XY, GX_U8, 0},
};

/**
 * Sends the mesh's geometry to GX: older map files as indexed float triangle strips written vertex by vertex,
 * ver1.02 files as packed s16 vertex arrays and the file's display lists.
 */
static void mapSetPolygon(MapObject* obj, MapFileMesh* mesh) {
    MapWork* wp;
    void* pos;
    void* nrm;
    s32 texCount;
    MapVcdTable* vcd;
    int i;
    int j;
    int k;
    MapRawDL* dl;
    MapVtxRec* v;

    wp = mapGetWork();
    vcd = mesh->vcdTable;
    pos = vcd->positions->data;
    nrm = vcd->normals->data;
    texCount = vcd->texCount;

    GXSetCullMode(culling[obj->joints->drawMode->cullMode]);
    if (!(wp->entries[obj->groupIndex].flags & MAP_ENTRY_VER_1_02)) {
        GXClearVtxDesc();
        GXSetVtxAttrFmtv(GX_VTXFMT0, attr_fmt);
        GXSetVtxDesc(GX_VA_POS, GX_INDEX16);
        GXSetArray(GX_VA_POS, pos, 12);
        GXSetVtxDesc(GX_VA_NRM, GX_INDEX16);
        GXSetArray(GX_VA_NRM, nrm, 12);
        GXSetVtxDesc(GX_VA_CLR0, GX_INDEX16);
        GXSetArray(GX_VA_CLR0, mesh->vcdTable->colors[0]->data, 4);
        for (i = 0; i < texCount; i++) {
            GXSetVtxDesc(GX_VA_TEX0 + i, GX_INDEX16);
            GXSetArray(GX_VA_TEX0 + i, mesh->vcdTable->texCoords[i]->data, 8);
        }
        for (i = 0; i < mesh->displayListCount; i++) {
            dl = mesh->displayLists.normalLists[i];
            GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, (u16)dl->count);
            for (j = 0; j < dl->count; j++) {
                v = &dl->vtx[j];
                GXPosition1x16(v->pos);
                GXNormal1x16(v->nrm);
                GXColor1x16(v->clr);
                for (k = 0; k < texCount; k++) {
                    GXTexCoord1x16(v->tex[k] != -1 ? (u16)v->tex[k] : 0);
                }
            }
        }
    } else {
        u32 mask = mesh->elementMask;
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_INDEX16);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, (u8)wp->entries[obj->groupIndex].vcdTable->posFrac);
        GXSetArray(GX_VA_POS, pos, 6);
        GXSetVtxDesc(GX_VA_NRM, GX_INDEX16);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_NRM, GX_NRM_XYZ, GX_S8, 6);
        GXSetArray(GX_VA_NRM, nrm, 3);
        for (i = 0; i < 2; i++) {
            if (!(mask & (1 << (i + 2)))) {
                break;
            }
            GXSetVtxDesc(GX_VA_CLR0, GX_INDEX16);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
            GXSetArray(GX_VA_CLR0, mesh->vcdTable->colors[i]->data, 4);
        }
        for (i = 0; i < 8; i++) {
            if (!(mask & (1 << (i + 4)))) {
                break;
            }
            GXSetVtxDesc(GX_VA_TEX0 + i, GX_INDEX16);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0 + i, GX_TEX_ST, GX_S16, (u8)wp->entries[obj->groupIndex].vcdTable->texFrac[i]);
            GXSetArray(GX_VA_TEX0 + i, mesh->vcdTable->texCoords[i]->data, 4);
        }
        for (i = 0; i < mesh->displayListCount; i++) {
            GXCallDisplayList((void*)mesh->displayLists.packedLists[i].offset, mesh->displayLists.packedLists[i].size);
        }
    }
}

/** Sets the GX vertex descriptors, formats and arrays for a ver1.02 packed mesh, ahead of calling its display lists. */
static void mapSetPolygonVtxDesc(MapObject* obj, MapFileMesh* mesh) {
    MapWork* wp;
    u32 mask;
    MapVcdTable* vcd;
    void* pos;
    void* nrm;
    int i;

    wp = mapGetWork();

    vcd = mesh->vcdTable;
    mask = mesh->elementMask;
    pos = vcd->positions->data;
    nrm = vcd->normals->data;
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_INDEX16);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, (u8)wp->entries[obj->groupIndex].vcdTable->posFrac);
    GXSetArray(GX_VA_POS, pos, 6);
    GXSetVtxDesc(GX_VA_NRM, GX_INDEX16);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_NRM, GX_NRM_XYZ, GX_S8, 6);
    GXSetArray(GX_VA_NRM, nrm, 3);
    for (i = 0; i < 2; i++) {
        if (!(mask & (1 << (i + 2)))) {
            break;
        }
        GXSetVtxDesc(GX_VA_CLR0, GX_INDEX16);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
        GXSetArray(GX_VA_CLR0, mesh->vcdTable->colors[i]->data, 4);
    }
    for (i = 0; i < 8; i++) {
        if (!(mask & (1 << (i + 4)))) {
            break;
        }
        GXSetVtxDesc(GX_VA_TEX0 + i, GX_INDEX16);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0 + i, GX_TEX_ST, GX_S16, (u8)wp->entries[obj->groupIndex].vcdTable->texFrac[i]);
        GXSetArray(GX_VA_TEX0 + i, mesh->vcdTable->texCoords[i]->data, 4);
    }
}

/** Depth-first search of the joint tree (children, then later siblings) for the joint called name. */
static MapFileJoint* mapSearchDmdJointSub(MapFileJoint* joint, const char* name) {
    MapFileJoint* found;

    if (!strcmp(joint->name, name)) {
        return joint;
    }
    if (joint->child) {
        found = mapSearchDmdJointSub(joint->child, name);
        if (found) {
            return found;
        }
    }
    if (joint->next) {
        found = mapSearchDmdJointSub(joint->next, name);
        if (found) {
            return found;
        }
    }
    return NULL;
}

/**
 * Depth-first search by full path: matches name against the joint's "|"-separated path from the root (prefix
 * plus its name).
 */
static MapFileJoint* mapSearchDmdJointSub2(MapFileJoint* joint, const char* name, const char* prefix) {
    MapFileJoint* found;
    char path[256];

    strcpy(path, prefix);
    strcat(path, joint->name);
    if (!strcmp(path, name)) {
        return joint;
    }
    if (joint->child) {
        strcat(path, "|");
        found = mapSearchDmdJointSub2(joint->child, name, path);
        if (found) {
            return found;
        }
    }
    if (joint->next) {
        found = mapSearchDmdJointSub2(joint->next, name, prefix);
        if (found) {
            return found;
        }
    }
    return NULL;
}

/**
 * The joint called name in the tree under joint;
 * a name starting with "|" is a full path ("|a|b"), anything else a plain joint name.
 * Used to find the map and hit roots named in the information chunk.
 */
MapFileJoint* mapSearchDmdJoint(MapFileJoint* joint, const char* name) {
    // FAKE: no-op (s32) cast; name[0] != '|' changes the object
    if ((s32)name[0] != '|') {
        return mapSearchDmdJointSub(joint, name);
    }
    return mapSearchDmdJointSub2(joint, name, "|");
}

/**
 * The map object of the active group whose joint is called name, or NULL;
 * the lookup behind the map object script helpers.
 */
MapObject* mapGetMapObj(const char* name) {
    MapWork* wp;
    MapEntry* entry;
    int j;
    int i;
    MapObject* object;

    wp = mapGetWork();
    if (!name) {
        return NULL;
    }
    for (i = 0; i < wp->entries[0].count; i++) {
        entry = &wp->entries[i];
        object = entry->objects;
        for (j = 0; j < entry->numJoints; j++, object++) {
            if (!strcmp(object->joints->name, name)) {
                return object;
            }
        }
    }
    return NULL;
}

void mapObjFlagOn(const char* name, u32 flags) {
    MapObject* obj = mapGetMapObj(name);

    if (!obj) {
        error_data[error_count].type = 0;
        strncpy(error_data[error_count].name, name, sizeof(error_data[0].name) - 1);
        error_flag = TRUE;
        error_count++;
        error_count %= 32;
        return;
    }
#if VERSION >= VERSION_JP
    if (!(flags & MAP_OBJ_FLAG_DISPLAY_LIST) || obj->displayListSize[0]) {
        obj->flags |= flags;
    }
}
#else
    obj->flags |= flags;
}
#endif

void mapObjFlagOff(const char* name, u32 flags) {
    MapObject* obj = mapGetMapObj(name);

    if (!obj) {
        error_data[error_count].type = 0;
        strncpy(error_data[error_count].name, name, sizeof(error_data[0].name) - 1);
        error_flag = TRUE;
        error_count++;
        error_count %= 32;
        return;
    }
    obj->flags &= ~flags;
}

/**
 * Sets flags on obj and its whole subtree, and its later siblings when doNext;
 * JP and later skip the display-list flag on objects without a display list.
 */
#if VERSION <= VERSION_DEMO_JP
static void mapFlgOn(MapObject* obj, u32 flags, BOOL doNext) {
    obj->flags |= flags;
#else
static void mapFlgOn(MapObject* obj, u32 flags, BOOL doNext) {
    if (!(flags & MAP_OBJ_FLAG_DISPLAY_LIST) || obj->displayListSize[0]) {
        obj->flags |= flags;
    }
#endif
    if (obj->child) {
        mapFlgOn(obj->child, flags, TRUE);
    }
    if (doNext && obj->next) {
        mapFlgOn(obj->next, flags, TRUE);
    }
}

/** Clears flags on obj and its whole subtree, and its later siblings when doNext. */
static void mapFlgOff(MapObject* obj, u32 flags, BOOL doNext) {
    obj->flags &= ~flags;
    if (obj->child) {
        mapFlgOff(obj->child, flags, TRUE);
    }
    if (doNext && obj->next) {
        mapFlgOff(obj->next, flags, TRUE);
    }
}

/** Sets flags on the map object name and everything under it, logging a map error if it does not exist. */
void mapGrpFlagOn(const char* name, u32 flags) {
    MapObject* obj = mapGetMapObj(name);

    if (!obj) {
        error_data[error_count].type = 0;
        strncpy(error_data[error_count].name, name, sizeof(error_data[0].name) - 1);
        error_flag = TRUE;
        error_count++;
        error_count %= 32;
        return;
    }
    mapFlgOn(obj, flags, FALSE);
}

/** Clears flags on the map object name and everything under it, logging a map error if it does not exist. */
void mapGrpFlagOff(const char* name, u32 flags) {
    MapObject* obj = mapGetMapObj(name);

    if (!obj) {
        error_data[error_count].type = 0;
        strncpy(error_data[error_count].name, name, sizeof(error_data[0].name) - 1);
        error_flag = TRUE;
        error_count++;
        error_count %= 32;
        return;
    }
    mapFlgOff(obj, flags, FALSE);
}

/**
 * Assigns offscreen buffer id to obj and its subtree (and later siblings when doNext),
 * setting the offscreen flag 0x80, or clearing it for id -1.
 */
static void _setOffScrnId(MapObject* obj, s32 id, BOOL doNext) {
    // Requires a non-NULL object; group offscreen operations pass their lookup result without a check.
    obj->offscreenId = id;
    if (id != -1) {
        obj->flags |= MAP_OBJ_FLAG_OFFSCREEN;
    } else {
        obj->flags &= ~MAP_OBJ_FLAG_OFFSCREEN;
    }
    if (obj->child) {
        _setOffScrnId(obj->child, id, TRUE);
    }
    if (doNext && obj->next) {
        _setOffScrnId(obj->next, id, TRUE);
    }
}

/** Makes the map object name also draw into the offscreen buffer called offscreen (evt_map). */
void mapObjSetOffScreen(const char* name, const char* offscreen) {
    MapObject* obj;
    MapObject* object;
    int i;
    int j;
    MapWork* wp;
    MapEntry* entry;

    // the search exits through `done` (an inline helper returning the object, or a flag, changes the code)
    wp = &mapWork[activeGroup];
    if (!name) {
        obj = NULL;
        goto done;
    }
    entry = &wp->entries[0];
    for (i = 0; i < wp->entries[0].count; entry++, i++) {
        object = entry->objects;
        for (j = 0; j < entry->numJoints; j++, object++) {
            if (!strcmp(object->joints->name, name)) {
                obj = object;
                goto done;
            }
        }
    }
    obj = NULL;
done:
    // BUG: obj is NULL when no map object has this name, and it is written through anyway
    obj->offscreenId = offscreenNameToId(offscreen);
    obj->flags |= MAP_OBJ_FLAG_OFFSCREEN;
}

/**
 * Makes the map object name and everything under it also draw into the offscreen buffer called offscreen
 * (evt_map, evt_cloud).
 */
void mapGrpSetOffScreen(const char* name, const char* offscreen) {
    MapObject* obj = mapGetMapObj(name);
    s32 id = offscreenNameToId(offscreen);

    // Requires name to resolve to a map object.
    _setOffScrnId(obj, id, FALSE);
}

/** Stops the map object name drawing into an offscreen buffer; crashes on an unknown name. */
void mapObjClearOffScreen(const char* name) {
    MapObject* obj = mapGetMapObj(name);

    // Requires name to resolve to a map object.
    obj->offscreenId = -1;
    obj->flags &= ~MAP_OBJ_FLAG_OFFSCREEN;
}

/** Stops the map object name and everything under it drawing into an offscreen buffer. */
void mapGrpClearOffScreen(const char* name) {
    MapObject* obj = mapGetMapObj(name);

    // Requires name to resolve to a map object.
    _setOffScrnId(obj, -1, FALSE);
}

/**
 * Adds a rotation (degrees, applied z, y, x) to the script transform of the map object name,
 * which then follows its world matrix; the transform restarts from identity
 * unless a change is still pending this frame. Used by evt_map and battle stage objects.
 */
void mapObjRotate(const char* name, f32 x, f32 y, f32 z) {
    MapObject* obj = mapGetMapObj(name);
    Mtx rotX, rotY, rotZ;

    if (obj) {
        if (!(obj->flags & MAP_OBJ_FLAG_MTX_DIRTY)) {
            MTXIdentity(obj->transformMtx);
        }
        MTXRotRad(rotX, 'x', MTXDegToRad(x));
        MTXRotRad(rotY, 'y', MTXDegToRad(y));
        MTXRotRad(rotZ, 'z', MTXDegToRad(z));
        MTXConcat(obj->transformMtx, rotZ, obj->transformMtx);
        MTXConcat(obj->transformMtx, rotY, obj->transformMtx);
        MTXConcat(obj->transformMtx, rotX, obj->transformMtx);
        obj->flags |= MAP_OBJ_FLAG_TRANSFORM;
        obj->flags |= MAP_OBJ_FLAG_MTX_DIRTY;
    }
}

/**
 * Adds a translation in world units to the script transform of the map object name (divided by the map's x10
 * scale); the transform restarts from identity unless a change is still pending this frame.
 */
void mapObjTranslate(const char* name, f32 x, f32 y, f32 z) {
    MapObject* obj = mapGetMapObj(name);
    Mtx trans;

    if (obj) {
        if (!(obj->flags & MAP_OBJ_FLAG_MTX_DIRTY)) {
            MTXIdentity(obj->transformMtx);
        }
        MTXTrans(trans, x / 10.0f, y / 10.0f, z / 10.0f);
        MTXConcat(obj->transformMtx, trans, obj->transformMtx);
        obj->flags |= MAP_OBJ_FLAG_TRANSFORM;
        obj->flags |= MAP_OBJ_FLAG_MTX_DIRTY;
    }
}

/**
 * Adds a scale to the script transform of the map object name;
 * the transform restarts from identity unless a change is still pending this frame.
 */
void mapObjScale(const char* name, f32 x, f32 y, f32 z) {
    MapObject* obj = mapGetMapObj(name);
    Mtx scale;

    if (obj) {
        if (!(obj->flags & MAP_OBJ_FLAG_MTX_DIRTY)) {
            MTXIdentity(obj->transformMtx);
        }
        MTXScale(scale, x, y, z);
        MTXConcat(obj->transformMtx, scale, obj->transformMtx);
        obj->flags |= MAP_OBJ_FLAG_TRANSFORM;
        obj->flags |= MAP_OBJ_FLAG_MTX_DIRTY;
    }
}

/**
 * Adds the world-space centre of each leaf object under obj (and its later siblings when doNext) to *sum,
 * counting them in *count.
 */
static void mapObjGetPosSub(MapObject* obj, Vec* sum, s32* count, BOOL doNext) {
    Vec pos;

    if (obj->child) {
        mapObjGetPosSub(obj->child, sum, count, TRUE);
    } else {
        MTXMultVec(obj->modelWorldMtx, &obj->center, &pos);
        VECAdd(sum, &pos, sum);
        (*count)++;
    }
    if (doNext && obj->next) {
        mapObjGetPosSub(obj->next, sum, count, TRUE);
    }
}

/**
 * World position of the map object name: the average centre of the leaf objects under it,
 * or the origin if it does not exist. Used by evt_map, evt_shop, evt_movefloor and others.
 */
void mapObjGetPos(const char* name, Vec* pos) {
    s32 count = 0;
    Vec sum = {0.0f, 0.0f, 0.0f};
    MapObject* obj = mapGetMapObj(name);

    if (!obj) {
        *pos = (Vec){0.0f, 0.0f, 0.0f};
        return;
    }
    mapObjGetPosSub(obj, &sum, &count, FALSE);
    VECScale(&sum, pos, 1.0f / count);
}

/** Sets the active map's fog type (index into fog_type), start, end and colour; mapFogOn turns it on. */
void mapSetFog(s32 type, f32 start, f32 end, GXColor color) {
    MapWork* wp = &mapWork[activeGroup];

    wp->entries[0].fogType = type;
    wp->entries[0].fogStart = start;
    wp->entries[0].fogEnd = end;
    wp->entries[0].fogColor = color;
}

/** Reads back the active map's fog type, start, end and colour. */
void mapGetFog(s32* type, f32* start, f32* end, GXColor* color) {
    MapWork* wp = &mapWork[activeGroup];

    *type = wp->entries[0].fogType;
    *start = wp->entries[0].fogStart;
    *end = wp->entries[0].fogEnd;
    *color = wp->entries[0].fogColor;
}

/** Turns the active map's fog on. */
void mapFogOn(void) {
    mapGetWork()->entries[0].flags |= MAP_ENTRY_FOG;
}

/** Turns the active map's fog off. */
void mapFogOff(void) {
    mapGetWork()->entries[0].flags &= ~MAP_ENTRY_FOG;
}

/**
 * Sets and enables the map-wide blend colour, which tints
 * and fades every map object that does not opt out (flag 0x800); evt_map.
 */
void mapSetBlend(GXColor* color) {
    MapWork* wp = &mapWork[activeGroup];

    wp->entries[0].blend = *color;
    wp->entries[0].flags |= MAP_ENTRY_BLEND;
}

/**
 * Sets and enables the second map-wide blend colour, applied like mapSetBlend to objects without flag 0x1000;
 * evt_map.
 */
void mapSetBlend2(GXColor* color) {
    MapWork* wp = &mapWork[activeGroup];

    wp->entries[0].blend2 = *color;
    wp->entries[0].flags |= MAP_ENTRY_BLEND2;
}

/** The map-wide blend colour, or white when it is off. */
void mapGetBlend(GXColor* color) {
    MapWork* wp = &mapWork[activeGroup];

    if (wp->entries[0].flags & MAP_ENTRY_BLEND) {
        *color = wp->entries[0].blend;
    } else {
        *color = (GXColor){0xFF, 0xFF, 0xFF, 0xFF};
    }
}

/** The second map-wide blend colour, or white when it is off. */
void mapGetBlend2(GXColor* color) {
    MapWork* wp = &mapWork[activeGroup];

    if (wp->entries[0].flags & MAP_ENTRY_BLEND2) {
        *color = wp->entries[0].blend2;
    } else {
        *color = (GXColor){0xFF, 0xFF, 0xFF, 0xFF};
    }
}

/** Turns the map-wide blend colour off. */
void mapBlendOff(void) {
    mapGetWork()->entries[0].flags &= ~MAP_ENTRY_BLEND;
}

/** Turns the second map-wide blend colour off. */
void mapBlendOff2(void) {
    mapGetWork()->entries[0].flags &= ~MAP_ENTRY_BLEND2;
}

/**
 * Tints the map object name by color, alpha included (flag 0x40);
 * objects with alpha below 255 draw translucent and at 0 not at all.
 */
void mapObjSetColor(const char* name, GXColor* color) {
    MapObject* obj = mapGetMapObj(name);

    if (obj) {
        obj->color = *color;
        obj->flags |= MAP_OBJ_FLAG_COLOR;
    }
}

/** Sets the tint colour and flag 0x40 on obj, its subtree and, when doNext, its later siblings. */
static void setColor(MapObject* obj, GXColor color, BOOL doNext) {
    if (obj) {
        obj->color = color;
        obj->flags |= MAP_OBJ_FLAG_COLOR;
        if (obj->child) {
            setColor(obj->child, color, TRUE);
        }
        if (doNext && obj->next) {
            setColor(obj->next, color, TRUE);
        }
    }
}

/** Tints the map object name and everything under it by color (see mapObjSetColor). */
void mapGrpSetColor(const char* name, GXColor color) {
    setColor(mapGetMapObj(name), color, FALSE);
}

/** Stores the texture for the projected texture stage of objects with flag 0x400; called by hei_00 and tou_01. */
void mapSetProjTexObj(GXTexObj* obj, void* imgRef) {
    projTexObj = *obj;
    projTexImgRef = imgRef;
}

/** Stores the projection matrix for the projected texture stage of objects with flag 0x400; called by hei_00 and tou_01. */
void mapSetProjMtx(Mtx mtx) {
    MTXCopy(mtx, projMtx);
}

/**
 * Prepares a cubic spline through the n points pts for spline_getvalue:
 * t gets each point's parameter (cumulative distance normalised to 0..1) and out the second-derivative table.
 * Used by evt_sub, himo and the aji unit_barriern units.
 */
void spline_maketable(s32 n, Vec* pts, f32* t, Vec* out) {
    f32* h;
    Vec* w;
    s32 i;
    f32 dx, dy, dz;
    f32 ax, ay, az;

    h = _mapAlloc(n * sizeof(f32));
    w = _mapAlloc(n * sizeof(Vec));
    t[0] = 0.0f;
    for (i = 1; i < n; i++) {
        dx = pts[i].x - pts[i - 1].x;
        dy = pts[i].y - pts[i - 1].y;
        dz = pts[i].z - pts[i - 1].z;
        t[i] = t[i - 1] + sqrtf(dx * dx + dy * dy + dz * dz);
    }
    for (i = 1; i < n; i++) {
        t[i] /= t[n - 1];
    }
    out[0].x = 0.0f;
    out[0].y = 0.0f;
    out[0].z = 0.0f;
    out[n - 1].x = 0.0f;
    out[n - 1].y = 0.0f;
    out[n - 1].z = 0.0f;
    for (i = 0; i < n - 1; i++) {
        h[i] = t[i + 1] - t[i];
        if (h[i] == 0.0f) {
            h[i] = 0.0001f;
        }
        w[i + 1].x = (pts[i + 1].x - pts[i].x) / h[i];
        w[i + 1].y = (pts[i + 1].y - pts[i].y) / h[i];
        w[i + 1].z = (pts[i + 1].z - pts[i].z) / h[i];
    }
    out[1].x = w[2].x - w[1].x;
    out[1].y = w[2].y - w[1].y;
    out[1].z = w[2].z - w[1].z;
    w[1].x = 2.0f * (t[2] - t[0]);
    w[1].y = 2.0f * (t[2] - t[0]);
    w[1].z = 2.0f * (t[2] - t[0]);
    for (i = 1; i < n - 2; i++) {
        ax = h[i] / w[i].x;
        ay = h[i] / w[i].y;
        az = h[i] / w[i].z;
        out[i + 1].x = (w[i + 2].x - w[i + 1].x) - ax * out[i].x;
        out[i + 1].y = (w[i + 2].y - w[i + 1].y) - ay * out[i].y;
        out[i + 1].z = (w[i + 2].z - w[i + 1].z) - az * out[i].z;
        w[i + 1].x = 2.0f * (t[i + 2] - t[i]) - ax * h[i];
        w[i + 1].y = 2.0f * (t[i + 2] - t[i]) - ay * h[i];
        w[i + 1].z = 2.0f * (t[i + 2] - t[i]) - az * h[i];
    }
    out[n - 2].x -= h[n - 2] * out[n - 1].x;
    out[n - 2].y -= h[n - 2] * out[n - 1].y;
    out[n - 2].z -= h[n - 2] * out[n - 1].z;
    for (i = n - 2; i > 0; i--) {
        out[i].x = (out[i].x - h[i] * out[i + 1].x) / w[i].x;
        out[i].y = (out[i].y - h[i] * out[i + 1].y) / w[i].y;
        out[i].z = (out[i].z - h[i] * out[i + 1].z) / w[i].z;
    }
    _mapFree(w);
    _mapFree(h);
}

/**
 * Point at parameter t (0..1) on the spline through the n points pts,
 * from the ts and tbl tables of spline_maketable; straight line for two points.
 */
void spline_getvalue(Vec* out, f32 t, s32 n, Vec* pts, f32* ts, Vec* tbl) {
    s32 lo;
    s32 hi;
    s32 mid;
    f32 y0;
    f32 u;
    f32 h;

    if (n == 2) {
        out->x = t * (pts[1].x - pts[0].x) + pts[0].x;
        y0 = pts[0].y;
        out->y = t * (pts[1].y - y0) + y0;
        out->z = t * (pts[1].z - pts[0].z) + pts[0].z;
        return;
    }
    hi = n - 1;
    lo = 0;
    while (lo < hi) {
        mid = (lo + hi) / 2;
        if (ts[mid] < t) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    if (lo > 0) {
        lo--;
    }
    u = t - ts[lo];
    h = ts[lo + 1] - ts[lo];
    out->x = pts[lo].x + u * ((pts[lo + 1].x - pts[lo].x) / h - h * (2.0f * tbl[lo].x + tbl[lo + 1].x) + u * (3.0f * tbl[lo].x + u * (tbl[lo + 1].x - tbl[lo].x) / h));
    out->y = pts[lo].y + u * ((pts[lo + 1].y - pts[lo].y) / h - h * (2.0f * tbl[lo].y + tbl[lo + 1].y) + u * (3.0f * tbl[lo].y + u * (tbl[lo + 1].y - tbl[lo].y) / h));
    out->z = pts[lo].z + u * ((pts[lo + 1].z - pts[lo].z) / h - h * (2.0f * tbl[lo].z + tbl[lo + 1].z) + u * (3.0f * tbl[lo].z + u * (tbl[lo + 1].z - tbl[lo].z) / h));
}

/**
 * Pointer to the map.dat chunk with the given name (information, texture_table, light_table, ...) via the file's
 * chunk table, or NULL.
 */
// Original name from the JP demo's link map (UNUSED there): every call is inlined, so the linker strips it.
void* HSD_ArchiveGetPublicAddress(MapHeader* header, const char* name) {
    u32 k;

    for (k = 0; k < header->file.chunkCount; k++) {
        if (!strcmp(&header->strings[header->chunks[k].string], name)) {
            return (u8*)header->data + header->chunks[k].offset;
        }
    }
    return NULL;
}

/**
 * Same translucency test as mapObjIsTranslucent, for animdrv models:
 * TRUE when the model colour (flag 0x40), material colour or a map-wide blend colour is not fully opaque.
 */
BOOL mapTestXLU(u32 flags, GXColor* color, GXColor* matColor) {
    if ((flags & MAP_OBJ_FLAG_COLOR) && color->a != 0xFF) {
        return TRUE;
    }
    if (matColor->a != 0xFF) {
        return TRUE;
    }
    if (!(flags & MAP_OBJ_FLAG_NO_MAP_BLEND) && (mapGetWork()->entries[0].flags & MAP_ENTRY_BLEND) && mapGetWork()->entries[0].blend.a != 0xFF) {
        return TRUE;
    }
    if (!(flags & MAP_OBJ_FLAG_NO_MAP_BLEND2) && (mapGetWork()->entries[0].flags & MAP_ENTRY_BLEND2) && mapGetWork()->entries[0].blend2.a != 0xFF) {
        return TRUE;
    }
    return FALSE;
}

/**
 * Installs callback in slot index of the active map's TEV callbacks;
 * objects or models whose flags carry that index in bits 24-27 get extra TEV stages from it (eff_mahojin uses
 * 10, unit_mario 11).
 */
void mapSetTevCallback(s32 index, MapTevCallback callback) {
    mapGetWork()->entries[0].tevCallbacks[index] = callback;
}

/** Turns on the flash colour of the map object name: its tint colour is added on top of its texture (flag 0x100000). */
void mapObjFlushOn(const char* name) {
    mapObjFlagOn(name, MAP_OBJ_FLAG_FLUSH);
}

/** Turns off the flash colour of the map object name. */
void mapObjFlushOff(const char* name) {
    mapObjFlagOff(name, MAP_OBJ_FLAG_FLUSH);
}

/** Turns on the flash colour of the map object name and everything under it. */
void mapGrpFlushOn(const char* name) {
    mapGrpFlagOn(name, MAP_OBJ_FLAG_FLUSH);
}

/** Turns off the flash colour of the map object name and everything under it. */
void mapGrpFlushOff(const char* name) {
    mapGrpFlagOff(name, MAP_OBJ_FLAG_FLUSH);
}

/**
 * Sets the flash colour of the map object name (stored as its tint colour,
 * with the tint flag cleared so it does not also tint).
 */
void mapObjSetFlushColor(const char* name, u8 r, u8 g, u8 b, u8 a) {
    GXColor color = {0};

    color.r = r;
    color.g = g;
    color.b = b;
    color.a = a;

    mapObjSetColor(name, &color);
    mapObjFlagOff(name, MAP_OBJ_FLAG_COLOR);
}

/** Sets the flash colour of the map object name and everything under it, without enabling the tint. */
void mapGrpSetFlushColor(const char* name, u8 r, u8 g, u8 b, u8 a) {
    mapGrpSetColor(name, (GXColor){r, g, b, a});
    mapGrpFlagOff(name, MAP_OBJ_FLAG_COLOR);
}

/** Reads the flash (tint) colour of the map object name, logging a map error if it does not exist. */
void mapObjGetFlushColor(const char* name, u8* r, u8* g, u8* b, u8* a) {
    MapObject* obj = mapGetMapObj(name);

    if (!obj) {
        error_data[error_count].type = 0;
        strncpy(error_data[error_count].name, name, sizeof(error_data[0].name) - 1);
        error_flag = TRUE;
        error_count++;
        error_count %= 32;
        return;
    }
    *r = obj->color.r;
    *g = obj->color.g;
    *b = obj->color.b;
    *a = obj->color.a;
}

// FAKE: stand-in for functions the linker dead-stripped; it only keeps the constants they left behind in
// .rodata/.sdata2, in their original order. Nothing calls it. The JP demo map lists stripped functions in this
// file (UNUSED): mapGetConvertDate (0x26c, with a function-static date$2457, 0x20 .bss), mapGetAnmDt (0xa8) and
// mapGetCurve (0x120). By their meaning the date and "not converted" texts fit mapGetConvertDate, "animation_table"
// (@2530) mapGetAnmDt and "curve_table" (@2724) mapGetCurve, but the map does not show which function owned which
// constant: ".", "information" and "ver1.02" are also read by _mapLoad, and the owner of the floats is unknown.
// The bodies are not known.
static void mapStrippedPool(const char** str, f32* val) {
    str[0] = ".";
    val[0] = 3.0f;
    val[1] = 0.0f;
    val[2] = 0.5f;
    val[3] = 2.0f;
#if VERSION <= VERSION_DEMO_JP
    str[1] = "%s/map/%s/map.dat";
#else
    str[1] = "%s/m/%s/d";
#endif
    str[2] = "information";
    str[3] = "ver1.01";
    str[4] = "ver1.02";
    str[5] = "--/--/-- --:--:--";
    str[6] = "コンバートされていません";
    str[7] = "animation_table";
    str[8] = "curve_table";
}

