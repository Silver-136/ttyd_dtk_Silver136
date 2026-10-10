#include "driver/hitdrv.h"

extern void* mapGetWork(void);
extern s32 mapGetJoints(void* mapObj);
extern void* hitEntrySub(void* mapObj, s32 arg1, void* arg2, s32 arg3, s32 arg4);

extern void hitReCalcMatrix2(void* hit, void* arg, s32 flag);
extern s32 hitCalcVtxPosition(void* hit);
extern void PSMTXMultVec(void* mtx, void* src, void* dst);
extern f32 PSVECDistance(void* a, void* b);
extern s32 strcmp(const char* str1, const char* str2);

void hitInit(void) {
    ;
}

void hitReInit(void) {
    extern void* mapGetWork(void);
    extern void* mapalloc_base_ptr;
    extern void _mapFree(void* heap, void* ptr);

    void* map;
    void* group;
    void* hit;
    s32 groupIndex;
    s32 hitIndex;

    map = mapGetWork();
    group = map;
    groupIndex = 0;
    while (groupIndex < *(s32*)map) {
        hit = *(void**)((s32)group + 0x15C);
        if (hit != NULL) {
            hitIndex = 0;
            while (hitIndex < *(s32*)((s32)group + 0x158)) {
                if (*(void**)((s32)hit + 0xAC) != NULL) {
                    _mapFree(mapalloc_base_ptr, *(void**)((s32)hit + 0xAC));
                }
                hitIndex++;
                hit = (void*)((s32)hit + 0xE4);
            }
            _mapFree(mapalloc_base_ptr, *(void**)((s32)group + 0x15C));
            *(void**)((s32)group + 0x15C) = NULL;
            *(s32*)((s32)group + 0x158) = 0;
        }
        group = (void*)((s32)group + 0x178);
        groupIndex++;
    }
}

void* _hitEnt(void* param_1, s32 param_2, s32 param_3, int param_4) {
    typedef f32 MtxLocal[3][4];
    typedef struct VecLocal { f32 x, y, z; } VecLocal;
    typedef struct DrawModeLocal { u8 pad0[8]; u32 attributes; } DrawModeLocal;
    typedef struct MeshLocal {
        u8 pad0[3];
        u8 isPackedDisplay;
        s32 displayListCount;
        u8 pad8[8];
        void* displayLists[1];
    } MeshLocal;
    typedef struct PartLocal { MeshLocal* mesh; u32 pad4; } PartLocal;
    typedef struct JointLocal {
        u8 pad0[0x18];
        VecLocal scale;
        VecLocal rotation;
        VecLocal translation;
        VecLocal bboxMin;
        VecLocal bboxMax;
        DrawModeLocal* drawMode;
        s32 partCount;
        u8 pad60[4];
        PartLocal parts[1];
    } JointLocal;
    typedef struct HitVectorLocal { u8 data[0x54]; } HitVectorLocal;
    typedef struct HitEntryLocal {
        u16 flags;
        u8 pad2[2];
        u32 attributes;
        JointLocal* joint;
        MtxLocal worldMtx;
        MtxLocal localMtx;
        VecLocal centerLocal;
        s16 unkA8;
        s16 mapIndex;
        HitVectorLocal* vectors;
        void* damage;
        u8 padB4[0xC];
        VecLocal centerWorld;
        f32 radius;
        void* mapObj;
        struct HitEntryLocal* parent;
        struct HitEntryLocal* child;
        struct HitEntryLocal* sibling;
        struct HitEntryLocal* siblingActive;
    } HitEntryLocal;
    typedef struct MapEntryLocal {
        u8 pad0[0x158];
        s32 hitNumJoints;
        HitEntryLocal* hitEntries;
        u8 pad160[0x18];
    } MapEntryLocal;
    typedef struct MapWorkLocal { MapEntryLocal entries[1]; } MapWorkLocal;
    extern void* mapGetWork(void);
    extern void PSMTXTrans(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXScale(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXRotRad(void* mtx, s32 axis, f32 rad);
    extern void PSMTXConcat(void* a, void* b, void* out);
    extern void PSVECAdd(void* a, void* b, void* out);
    extern void PSVECScale(void* src, void* dst, f32 scale);
    extern void PSMTXMultVec(void* mtx, void* src, void* dst);
    extern f32 PSVECDistance(void* a, void* b);
    extern void* _mapAlloc(void* heap, s32 size);
    extern void* mapalloc_base_ptr;
    extern const f32 float_deg2rad_8041f850;
    extern const f32 float_0p5_8041f84c;
    extern const f32 float_0p1_8041f83c;
    MapWorkLocal* wp = mapGetWork();
    JointLocal* joint = (JointLocal*)param_1;
    HitEntryLocal* hit = wp->entries[param_4].hitEntries;
    MtxLocal trans, scale, rotX, rotY, rotZ;
    VecLocal vtmp;
    s32 i;

    for (i = 0; i < wp->entries[param_4].hitNumJoints; hit++, i++) {
        if (!hit->joint) break;
    }
    hit->flags = 0;
    hit->joint = joint;
    hit->mapIndex = param_4;
    PSMTXTrans(trans, joint->translation.x, joint->translation.y, joint->translation.z);
    PSMTXScale(scale, joint->scale.x, joint->scale.y, joint->scale.z);
    PSMTXRotRad(rotX, 'x', float_deg2rad_8041f850 * joint->rotation.x);
    PSMTXRotRad(rotY, 'y', float_deg2rad_8041f850 * joint->rotation.y);
    PSMTXRotRad(rotZ, 'z', float_deg2rad_8041f850 * joint->rotation.z);
    PSMTXConcat(trans, rotZ, trans);
    PSMTXConcat(trans, rotY, trans);
    PSMTXConcat(trans, rotX, trans);
    PSMTXConcat(trans, scale, hit->localMtx);
    PSMTXConcat((void*)param_3, hit->localMtx, hit->worldMtx);
    hit->parent = (HitEntryLocal*)param_2;
    hit->sibling = 0;
    hit->child = 0;
    hit->vectors = 0;
    hit->attributes = joint->drawMode != 0 ? joint->drawMode->attributes : 0;
    if (joint->partCount) {
        PSVECAdd(&joint->bboxMin, &joint->bboxMax, &hit->centerLocal);
        PSVECScale(&hit->centerLocal, &hit->centerLocal, float_0p5_8041f84c);
    } else {
        hit->centerLocal = joint->translation;
    }
    PSMTXMultVec(hit->worldMtx, &hit->centerLocal, &hit->centerWorld);
    joint->bboxMin.x -= float_0p1_8041f83c;
    joint->bboxMin.y -= float_0p1_8041f83c;
    joint->bboxMin.z -= float_0p1_8041f83c;
    joint->bboxMax.x += float_0p1_8041f83c;
    joint->bboxMax.y += float_0p1_8041f83c;
    joint->bboxMax.z += float_0p1_8041f83c;
    PSMTXMultVec(hit->worldMtx, &hit->joint->bboxMax, &vtmp);
    hit->radius = PSVECDistance(&vtmp, &hit->centerWorld);
    {
        s32 numVectors, j, k;
        s32 partIdx;
        for (numVectors = 0, partIdx = 0; partIdx < hit->joint->partCount; partIdx++) {
            MeshLocal* mesh = hit->joint->parts[partIdx].mesh;
            if (mesh == 0) continue;
            if (!mesh->isPackedDisplay) {
                for (j = 0; j < mesh->displayListCount; j++) {
                    numVectors += *(s32*)mesh->displayLists[j] - 2;
                }
            } else {
                for (k = 0; k < mesh->displayListCount; k++) {
                    numVectors += *(u16*)((u8*)mesh->displayLists[k * 2] + 1) - 2;
                }
            }
        }
        if (numVectors > 0) {
            hit->vectors = _mapAlloc(mapalloc_base_ptr, sizeof(HitVectorLocal) * numVectors);
            hit->unkA8 = (s16)hitCalcVtxPosition(hit);
        }
    }
    return hit;
}

void* hitEntrySub(void* joint, s32 parent, void* parentMtx, s32 rootOnly, s32 groupIndex) {
    extern void* _hitEnt(void* joint, s32 parent, s32 parentMtx, s32 groupIndex);
    void* root;
    void* child;
    void* grand;
    void* hitChild;
    void* hitGrand;

    root = _hitEnt(joint, parent, (s32)parentMtx, groupIndex);
    child = *(void**)((s32)joint + 0x0C);
    if (child != NULL) {
        hitChild = _hitEnt(child, (s32)root, (s32)root + 0x0C, groupIndex);
        grand = *(void**)((s32)child + 0x0C);
        if (grand != NULL) {
            hitGrand = _hitEnt(grand, (s32)hitChild, (s32)hitChild + 0x0C, groupIndex);
            if (*(void**)((s32)grand + 0x0C) != NULL) {
                *(void**)((s32)hitGrand + 0xD8) = hitEntrySub(*(void**)((s32)grand + 0x0C), (s32)hitGrand, (void*)((s32)hitGrand + 0x0C), 0, groupIndex);
            }
            if (*(void**)((s32)grand + 0x10) != NULL) {
                *(void**)((s32)hitGrand + 0xDC) = hitEntrySub(*(void**)((s32)grand + 0x10), (s32)hitChild, (void*)((s32)hitChild + 0x0C), 0, groupIndex);
            }
            *(void**)((s32)hitChild + 0xD8) = hitGrand;
        }
        child = *(void**)((s32)child + 0x10);
        if (child != NULL) {
            hitGrand = _hitEnt(child, (s32)root, (s32)root + 0x0C, groupIndex);
            if (*(void**)((s32)child + 0x0C) != NULL) {
                *(void**)((s32)hitGrand + 0xD8) = hitEntrySub(*(void**)((s32)child + 0x0C), (s32)hitGrand, (void*)((s32)hitGrand + 0x0C), 0, groupIndex);
            }
            if (*(void**)((s32)child + 0x10) != NULL) {
                *(void**)((s32)hitGrand + 0xDC) = hitEntrySub(*(void**)((s32)child + 0x10), (s32)root, (void*)((s32)root + 0x0C), 0, groupIndex);
            }
            *(void**)((s32)hitChild + 0xDC) = hitGrand;
        }
        *(void**)((s32)root + 0xD8) = hitChild;
    }
    if (rootOnly == 0) {
        child = *(void**)((s32)joint + 0x10);
        if (child != NULL) {
            hitChild = _hitEnt(child, parent, (s32)parentMtx, groupIndex);
            grand = *(void**)((s32)child + 0x0C);
            if (grand != NULL) {
                hitGrand = _hitEnt(grand, (s32)hitChild, (s32)hitChild + 0x0C, groupIndex);
                if (*(void**)((s32)grand + 0x0C) != NULL) {
                    *(void**)((s32)hitGrand + 0xD8) = hitEntrySub(*(void**)((s32)grand + 0x0C), (s32)hitGrand, (void*)((s32)hitGrand + 0x0C), 0, groupIndex);
                }
                if (*(void**)((s32)grand + 0x10) != NULL) {
                    *(void**)((s32)hitGrand + 0xDC) = hitEntrySub(*(void**)((s32)grand + 0x10), (s32)hitChild, (void*)((s32)hitChild + 0x0C), 0, groupIndex);
                }
                *(void**)((s32)hitChild + 0xD8) = hitGrand;
            }
            child = *(void**)((s32)child + 0x10);
            if (child != NULL) {
                hitGrand = _hitEnt(child, parent, (s32)parentMtx, groupIndex);
                if (*(void**)((s32)child + 0x0C) != NULL) {
                    *(void**)((s32)hitGrand + 0xD8) = hitEntrySub(*(void**)((s32)child + 0x0C), (s32)hitGrand, (void*)((s32)hitGrand + 0x0C), 0, groupIndex);
                }
                if (*(void**)((s32)child + 0x10) != NULL) {
                    *(void**)((s32)hitGrand + 0xDC) = hitEntrySub(*(void**)((s32)child + 0x10), parent, parentMtx, 0, groupIndex);
                }
                *(void**)((s32)hitChild + 0xDC) = hitGrand;
            }
            *(void**)((s32)root + 0xDC) = hitChild;
        }
    }
    return root;
}

void* hitEntry(void* mapObj, void* arg, s32 idx) {
    typedef struct HitEntryLocal {
        u8 data[0xE4];
    } HitEntryLocal;
    typedef struct MapEntryLocal {
        u8 unk0[0x158];
        s32 hitNumJoints;
        HitEntryLocal* hitEntries;
        u8 unk160[0x18];
    } MapEntryLocal;
    typedef struct MapWorkLocal {
        MapEntryLocal entries[1];
    } MapWorkLocal;
    extern void* mapGetWork(void);
    extern s32 mapGetJoints(void* mapObj);
    extern void* _mapAlloc(void* heap, u32 size);
    extern void* mapalloc_base_ptr;
    extern void memset(void* dst, s32 value, u32 size);

    MapWorkLocal* wp = mapGetWork();
    u32 size;

    wp->entries[idx].hitNumJoints = mapGetJoints(mapObj);
    size = (u32)((wp->entries[idx].hitNumJoints + 0x80) * sizeof(HitEntryLocal) + 0x1F) & ~0x1F;
    wp->entries[idx].hitEntries = _mapAlloc(mapalloc_base_ptr, size);
    memset(wp->entries[idx].hitEntries, 0, size);
    return hitEntrySub(mapObj, 0, arg, 1, idx);
}

void hitEntryMOBJ(void* mapObj, void* arg) {
    void* mapWork;

    mapWork = mapGetWork();

    *(s32*)((s32)mapWork + 0x158) += mapGetJoints(mapObj);

    hitEntrySub(mapObj, 0, arg, 1, 0);
}

void hitDelete(char* name) {
    extern void* mapGetWork(void);
    extern s32 strcmp(const char* str1, const char* str2);
    extern void* unk_8041e628;

    void* map;
    void* group;
    void* hit;
    void* last;
    s32 groupIndex;
    s32 hitIndex;

    map = mapGetWork();
    group = map;
    groupIndex = 0;
    while (groupIndex < *(s32*)map) {
        hit = *(void**)((s32)group + 0x15C);
        hitIndex = 0;
        while (hitIndex < *(s32*)((s32)group + 0x158)) {
            if ((*(u16*)hit & 0x80) == 0 &&
                strcmp(name, **(char***)((s32)hit + 0x8)) == 0) {
                *(u16*)hit |= 0x80;

                map = mapGetWork();
                unk_8041e628 = NULL;
                last = NULL;
                group = map;
                groupIndex = 0;
                while (groupIndex < *(s32*)map) {
                    hit = *(void**)((s32)group + 0x15C);
                    hitIndex = 0;
                    while (hitIndex < *(s32*)((s32)group + 0x158)) {
                        if (*(s32*)(*(s32*)((s32)hit + 0x8) + 0x5C) != 0 &&
                            (*(u16*)hit & 0x81) == 0) {
                            if (last == NULL) {
                                unk_8041e628 = hit;
                                last = hit;
                            } else {
                                *(void**)((s32)last + 0xE0) = hit;
                                last = hit;
                            }
                        }
                        hitIndex++;
                        hit = (void*)((s32)hit + 0xE4);
                    }
                    group = (void*)((s32)group + 0x178);
                    groupIndex++;
                }
                if (last != NULL) {
                    *(void**)((s32)last + 0xE0) = NULL;
                }
                return;
            }
            hitIndex++;
            hit = (void*)((s32)hit + 0xE4);
        }
        group = (void*)((s32)group + 0x178);
        groupIndex++;
    }
}

void hitMain(void) {
    extern void* unk_8041e628;
    extern void mapCalcAnimMatrix(void* dst, void* parent, void* entry, void* track);
    extern void PSMTXScale(void* mtx, f32 x, f32 y, f32 z);
    extern const f32 float_10_8041f840;
    void* map = mapGetWork();
    void* scan = mapGetWork();
    void* group;
    void* hit;
    void* last = NULL;
    s32 groupIndex;
    s32 hitIndex;
    f32 scale[3][4];

    unk_8041e628 = NULL;
    group = scan;
    for (groupIndex = 0; groupIndex < *(s32*)scan; groupIndex++, group = (void*)((s32)group + 0x178)) {
        hit = *(void**)((s32)group + 0x15C);
        for (hitIndex = 0; hitIndex < *(s32*)((s32)group + 0x158); hitIndex++, hit = (void*)((s32)hit + 0xE4)) {
            if (*(s32*)(*(s32*)((s32)hit + 8) + 0x5C) != 0 && (*(u16*)hit & 0x81) == 0) {
                if (last == NULL) {
                    unk_8041e628 = hit;
                    last = hit;
                } else {
                    *(void**)((s32)last + 0xE0) = hit;
                    last = hit;
                }
            }
        }
    }
    if (last != NULL) *(void**)((s32)last + 0xE0) = NULL;

    group = map;
    for (groupIndex = 0; groupIndex < *(s32*)map; groupIndex++, group = (void*)((s32)group + 0x178)) {
        void* entry = *(void**)((s32)group + 0x164);
        s32 animIndex;
        s32 changed = 0;
        for (animIndex = 0; animIndex < *(s32*)((s32)group + 0x160); animIndex++, entry = (void*)((s32)entry + 0x20)) {
            u16 flags = *(u16*)entry;
            void* tracks = *(void**)(*(s32*)((s32)entry + 0x18) + 0x0C);
            if ((flags & 1) != 0 && ((flags & 2) == 0 || (flags & 0x1000) == 0) && tracks != NULL) {
                s32 trackIndex;
                void* trackNode = tracks;
                for (trackIndex = 0; trackIndex < *(s32*)tracks; trackIndex++, trackNode = (void*)((s32)trackNode + 4)) {
                    void* track = *(void**)((s32)trackNode + 4);
                    char* name = *(char**)track;
                    void* searchMap = mapGetWork();
                    void* searchGroup;
                    s32 searchGroupIndex;
                    if (name == NULL) {
                        hit = NULL;
                    } else {
                        searchGroup = searchMap;
                        for (searchGroupIndex = 0; searchGroupIndex < *(s32*)searchMap;
                             searchGroupIndex++, searchGroup = (void*)((s32)searchGroup + 0x178)) {
                            hit = *(void**)((s32)searchGroup + 0x15C);
                            for (hitIndex = 0; hitIndex < *(s32*)((s32)searchGroup + 0x158);
                                 hitIndex++, hit = (void*)((s32)hit + 0xE4)) {
                                if ((*(u16*)hit & 0x80) == 0 &&
                                    strcmp(**(char***)((s32)hit + 8), name) == 0) {
                                    goto search_done;
                                }
                            }
                        }
                        hit = NULL;
                    }
search_done:
                    if (hit != NULL) {
                        mapCalcAnimMatrix((void*)((s32)hit + 0x6C), (void*)((s32)hit + 0x3C), entry, track);
                        *(u16*)hit |= 0x10;
                        *(u16*)hit |= 0x40;
                        *(u16*)entry |= 0x1000;
                        changed = 1;
                    }
                }
            }
        }
        if (changed) {
            PSMTXScale(scale, float_10_8041f840, float_10_8041f840, float_10_8041f840);
            hitReCalcMatrix2(*(void**)((s32)group + 0xAC), scale, 0);
        }
    }
}

void hitObjFlagOn(char* name, u16 flag) {
    extern void* mapGetWork(void);
    extern s32 strcmp(const char* str1, const char* str2);
    extern void mapErrorEntry(s32 type, char* message);

    s32 groupIndex;
    void* hit;
    s32 hitIndex;
    void* map;
    void* group;

    map = mapGetWork();
    if (name == NULL) {
        hit = NULL;
    } else {
        group = map;
        groupIndex = 0;
        while (groupIndex < *(s32*)map) {
            hit = *(void**)((s32)group + 0x15C);
            hitIndex = 0;
            while (hitIndex < *(s32*)((s32)group + 0x158)) {
                if ((*(u16*)hit & 0x80) != 0) {
                    goto next_hit;
                }
                if (strcmp(**(char***)((s32)hit + 0x8), name) != 0) {
                    goto next_hit;
                }
                goto found;

next_hit:
                hitIndex++;
                hit = (void*)((s32)hit + 0xE4);
            }
            group = (void*)((s32)group + 0x178);
            groupIndex++;
        }
        hit = NULL;
    }

found:
    if (hit == NULL) {
        mapErrorEntry(1, name);
    } else {
        *(u16*)hit |= flag;
    }
}

void hitObjFlagOff(char* name, u16 flag) {
    extern void* mapGetWork(void);
    extern s32 strcmp(const char* str1, const char* str2);
    extern void mapErrorEntry(s32 type, char* message);

    s32 groupIndex;
    void* hit;
    s32 hitIndex;
    void* map;
    void* group;

    map = mapGetWork();
    if (name == NULL) {
        hit = NULL;
    } else {
        group = map;
        groupIndex = 0;
        while (groupIndex < *(s32*)map) {
            hit = *(void**)((s32)group + 0x15C);
            hitIndex = 0;
            while (hitIndex < *(s32*)((s32)group + 0x158)) {
                if ((*(u16*)hit & 0x80) != 0) {
                    goto next_hit;
                }
                if (strcmp(**(char***)((s32)hit + 0x8), name) != 0) {
                    goto next_hit;
                }
                goto found;

next_hit:
                hitIndex++;
                hit = (void*)((s32)hit + 0xE4);
            }
            group = (void*)((s32)group + 0x178);
            groupIndex++;
        }
        hit = NULL;
    }

found:
    if (hit == NULL) {
        mapErrorEntry(1, name);
    } else {
        *(u16*)hit &= ~flag;
    }
}

void hitFlgOn(void* param_1, s32 param_2, int param_3) {
    void* iVar1;
    void* iVar2;
    void* iVar3;
    u32 value = param_2;

    *(u16*)param_1 = *(u16*)param_1 | value;

    iVar3 = *(void**)((s32)param_1 + 0xD8);
    if (iVar3 != 0) {
        *(u16*)iVar3 = *(u16*)iVar3 | value;

        iVar2 = *(void**)((s32)iVar3 + 0xD8);
        if (iVar2 != 0) {
            *(u16*)iVar2 = *(u16*)iVar2 | value;

            iVar1 = *(void**)((s32)iVar2 + 0xD8);
            if (iVar1 != 0) {
                *(u16*)iVar1 = *(u16*)iVar1 | value;
                if (*(void**)((s32)iVar1 + 0xD8) != 0) {
                    hitFlgOn(*(void**)((s32)iVar1 + 0xD8), param_2, 1);
                }
                if (*(void**)((s32)iVar1 + 0xDC) != 0) {
                    hitFlgOn(*(void**)((s32)iVar1 + 0xDC), param_2, 1);
                }
            }

            iVar2 = *(void**)((s32)iVar2 + 0xDC);
            if (iVar2 != 0) {
                *(u16*)iVar2 = *(u16*)iVar2 | value;
                if (*(void**)((s32)iVar2 + 0xD8) != 0) {
                    hitFlgOn(*(void**)((s32)iVar2 + 0xD8), param_2, 1);
                }
                if (*(void**)((s32)iVar2 + 0xDC) != 0) {
                    hitFlgOn(*(void**)((s32)iVar2 + 0xDC), param_2, 1);
                }
            }
        }

        iVar3 = *(void**)((s32)iVar3 + 0xDC);
        if (iVar3 != 0) {
            *(u16*)iVar3 = *(u16*)iVar3 | value;

            iVar2 = *(void**)((s32)iVar3 + 0xD8);
            if (iVar2 != 0) {
                *(u16*)iVar2 = *(u16*)iVar2 | value;
                if (*(void**)((s32)iVar2 + 0xD8) != 0) {
                    hitFlgOn(*(void**)((s32)iVar2 + 0xD8), param_2, 1);
                }
                if (*(void**)((s32)iVar2 + 0xDC) != 0) {
                    hitFlgOn(*(void**)((s32)iVar2 + 0xDC), param_2, 1);
                }
            }

            iVar3 = *(void**)((s32)iVar3 + 0xDC);
            if (iVar3 != 0) {
                *(u16*)iVar3 = *(u16*)iVar3 | value;
                if (*(void**)((s32)iVar3 + 0xD8) != 0) {
                    hitFlgOn(*(void**)((s32)iVar3 + 0xD8), param_2, 1);
                }
                if (*(void**)((s32)iVar3 + 0xDC) != 0) {
                    hitFlgOn(*(void**)((s32)iVar3 + 0xDC), param_2, 1);
                }
            }
        }
    }

    if (param_3 != 0) {
        iVar3 = *(void**)((s32)param_1 + 0xDC);
        if (iVar3 != 0) {
            *(u16*)iVar3 = *(u16*)iVar3 | value;

            iVar2 = *(void**)((s32)iVar3 + 0xD8);
            if (iVar2 != 0) {
                *(u16*)iVar2 = *(u16*)iVar2 | value;

                iVar1 = *(void**)((s32)iVar2 + 0xD8);
                if (iVar1 != 0) {
                    *(u16*)iVar1 = *(u16*)iVar1 | value;
                    if (*(void**)((s32)iVar1 + 0xD8) != 0) {
                        hitFlgOn(*(void**)((s32)iVar1 + 0xD8), param_2, 1);
                    }
                    if (*(void**)((s32)iVar1 + 0xDC) != 0) {
                        hitFlgOn(*(void**)((s32)iVar1 + 0xDC), param_2, 1);
                    }
                }

                iVar2 = *(void**)((s32)iVar2 + 0xDC);
                if (iVar2 != 0) {
                    *(u16*)iVar2 = *(u16*)iVar2 | value;
                    if (*(void**)((s32)iVar2 + 0xD8) != 0) {
                        hitFlgOn(*(void**)((s32)iVar2 + 0xD8), param_2, 1);
                    }
                    if (*(void**)((s32)iVar2 + 0xDC) != 0) {
                        hitFlgOn(*(void**)((s32)iVar2 + 0xDC), param_2, 1);
                    }
                }
            }

            iVar3 = *(void**)((s32)iVar3 + 0xDC);
            if (iVar3 != 0) {
                *(u16*)iVar3 = *(u16*)iVar3 | value;

                iVar2 = *(void**)((s32)iVar3 + 0xD8);
                if (iVar2 != 0) {
                    *(u16*)iVar2 = *(u16*)iVar2 | value;
                    if (*(void**)((s32)iVar2 + 0xD8) != 0) {
                        hitFlgOn(*(void**)((s32)iVar2 + 0xD8), param_2, 1);
                    }
                    if (*(void**)((s32)iVar2 + 0xDC) != 0) {
                        hitFlgOn(*(void**)((s32)iVar2 + 0xDC), param_2, 1);
                    }
                }

                iVar3 = *(void**)((s32)iVar3 + 0xDC);
                if (iVar3 != 0) {
                    *(u16*)iVar3 = *(u16*)iVar3 | value;
                    if (*(void**)((s32)iVar3 + 0xD8) != 0) {
                        hitFlgOn(*(void**)((s32)iVar3 + 0xD8), param_2, 1);
                    }
                    if (*(void**)((s32)iVar3 + 0xDC) != 0) {
                        hitFlgOn(*(void**)((s32)iVar3 + 0xDC), param_2, 1);
                    }
                }
            }
        }
    }

    return;
}

void hitFlgOff(void* param_1, s32 param_2, int param_3) {
    void* iVar1;
    void* iVar2;
    void* iVar3;
    u32 value = ~((u32)(u16)param_2);

    *(u16*)param_1 = *(u16*)param_1 & value;

    iVar3 = *(void**)((s32)param_1 + 0xD8);
    if (iVar3 != 0) {
        *(u16*)iVar3 = *(u16*)iVar3 & value;

        iVar2 = *(void**)((s32)iVar3 + 0xD8);
        if (iVar2 != 0) {
            *(u16*)iVar2 = *(u16*)iVar2 & value;

            iVar1 = *(void**)((s32)iVar2 + 0xD8);
            if (iVar1 != 0) {
                *(u16*)iVar1 = *(u16*)iVar1 & value;
                if (*(void**)((s32)iVar1 + 0xD8) != 0) {
                    hitFlgOff(*(void**)((s32)iVar1 + 0xD8), param_2, 1);
                }
                if (*(void**)((s32)iVar1 + 0xDC) != 0) {
                    hitFlgOff(*(void**)((s32)iVar1 + 0xDC), param_2, 1);
                }
            }

            iVar2 = *(void**)((s32)iVar2 + 0xDC);
            if (iVar2 != 0) {
                *(u16*)iVar2 = *(u16*)iVar2 & value;
                if (*(void**)((s32)iVar2 + 0xD8) != 0) {
                    hitFlgOff(*(void**)((s32)iVar2 + 0xD8), param_2, 1);
                }
                if (*(void**)((s32)iVar2 + 0xDC) != 0) {
                    hitFlgOff(*(void**)((s32)iVar2 + 0xDC), param_2, 1);
                }
            }
        }

        iVar3 = *(void**)((s32)iVar3 + 0xDC);
        if (iVar3 != 0) {
            *(u16*)iVar3 = *(u16*)iVar3 & value;

            iVar2 = *(void**)((s32)iVar3 + 0xD8);
            if (iVar2 != 0) {
                *(u16*)iVar2 = *(u16*)iVar2 & value;
                if (*(void**)((s32)iVar2 + 0xD8) != 0) {
                    hitFlgOff(*(void**)((s32)iVar2 + 0xD8), param_2, 1);
                }
                if (*(void**)((s32)iVar2 + 0xDC) != 0) {
                    hitFlgOff(*(void**)((s32)iVar2 + 0xDC), param_2, 1);
                }
            }

            iVar3 = *(void**)((s32)iVar3 + 0xDC);
            if (iVar3 != 0) {
                *(u16*)iVar3 = *(u16*)iVar3 & value;
                if (*(void**)((s32)iVar3 + 0xD8) != 0) {
                    hitFlgOff(*(void**)((s32)iVar3 + 0xD8), param_2, 1);
                }
                if (*(void**)((s32)iVar3 + 0xDC) != 0) {
                    hitFlgOff(*(void**)((s32)iVar3 + 0xDC), param_2, 1);
                }
            }
        }
    }

    if (param_3 != 0) {
        iVar3 = *(void**)((s32)param_1 + 0xDC);
        if (iVar3 != 0) {
            *(u16*)iVar3 = *(u16*)iVar3 & value;

            iVar2 = *(void**)((s32)iVar3 + 0xD8);
            if (iVar2 != 0) {
                *(u16*)iVar2 = *(u16*)iVar2 & value;

                iVar1 = *(void**)((s32)iVar2 + 0xD8);
                if (iVar1 != 0) {
                    *(u16*)iVar1 = *(u16*)iVar1 & value;
                    if (*(void**)((s32)iVar1 + 0xD8) != 0) {
                        hitFlgOff(*(void**)((s32)iVar1 + 0xD8), param_2, 1);
                    }
                    if (*(void**)((s32)iVar1 + 0xDC) != 0) {
                        hitFlgOff(*(void**)((s32)iVar1 + 0xDC), param_2, 1);
                    }
                }

                iVar2 = *(void**)((s32)iVar2 + 0xDC);
                if (iVar2 != 0) {
                    *(u16*)iVar2 = *(u16*)iVar2 & value;
                    if (*(void**)((s32)iVar2 + 0xD8) != 0) {
                        hitFlgOff(*(void**)((s32)iVar2 + 0xD8), param_2, 1);
                    }
                    if (*(void**)((s32)iVar2 + 0xDC) != 0) {
                        hitFlgOff(*(void**)((s32)iVar2 + 0xDC), param_2, 1);
                    }
                }
            }

            iVar3 = *(void**)((s32)iVar3 + 0xDC);
            if (iVar3 != 0) {
                *(u16*)iVar3 = *(u16*)iVar3 & value;

                iVar2 = *(void**)((s32)iVar3 + 0xD8);
                if (iVar2 != 0) {
                    *(u16*)iVar2 = *(u16*)iVar2 & value;
                    if (*(void**)((s32)iVar2 + 0xD8) != 0) {
                        hitFlgOff(*(void**)((s32)iVar2 + 0xD8), param_2, 1);
                    }
                    if (*(void**)((s32)iVar2 + 0xDC) != 0) {
                        hitFlgOff(*(void**)((s32)iVar2 + 0xDC), param_2, 1);
                    }
                }

                iVar3 = *(void**)((s32)iVar3 + 0xDC);
                if (iVar3 != 0) {
                    *(u16*)iVar3 = *(u16*)iVar3 & value;
                    if (*(void**)((s32)iVar3 + 0xD8) != 0) {
                        hitFlgOff(*(void**)((s32)iVar3 + 0xD8), param_2, 1);
                    }
                    if (*(void**)((s32)iVar3 + 0xDC) != 0) {
                        hitFlgOff(*(void**)((s32)iVar3 + 0xDC), param_2, 1);
                    }
                }
            }
        }
    }

    return;
}

void hitGrpFlagOn(char* name, u16 value) {
    extern void* mapGetWork(void);
    extern s32 strcmp(const char* str1, const char* str2);
    extern void mapErrorEntry(s32 type, char* message);
    extern void hitFlgOn(void* hit, u16 value, s32 recursive);

    s32 groupIndex;
    void* hit;
    s32 hitIndex;
    void* map;
    void* group;

    map = mapGetWork();
    if (name == NULL) {
        hit = NULL;
    } else {
        group = map;
        groupIndex = 0;
        while (groupIndex < *(s32*)map) {
            hit = *(void**)((s32)group + 0x15C);
            hitIndex = 0;
            while (hitIndex < *(s32*)((s32)group + 0x158)) {
                if ((*(u16*)hit & 0x80) != 0) {
                    goto next_hit;
                }
                if (strcmp(**(char***)((s32)hit + 0x8), name) != 0) {
                    goto next_hit;
                }
                goto found;

next_hit:
                hitIndex++;
                hit = (void*)((s32)hit + 0xE4);
            }
            group = (void*)((s32)group + 0x178);
            groupIndex++;
        }
        hit = NULL;
    }

found:
    if (hit == NULL) {
        mapErrorEntry(1, name);
    } else {
        hitFlgOn(hit, value, 0);
    }
}

void hitGrpFlagOff(char* name, u16 value) {
    extern void* mapGetWork(void);
    extern s32 strcmp(const char* str1, const char* str2);
    extern void mapErrorEntry(s32 type, char* message);
    extern void hitFlgOff(void* hit, u16 value, s32 recursive);

    s32 groupIndex;
    void* hit;
    s32 hitIndex;
    void* map;
    void* group;

    map = mapGetWork();
    if (name == NULL) {
        hit = NULL;
    } else {
        group = map;
        groupIndex = 0;
        while (groupIndex < *(s32*)map) {
            hit = *(void**)((s32)group + 0x15C);
            hitIndex = 0;
            while (hitIndex < *(s32*)((s32)group + 0x158)) {
                if ((*(u16*)hit & 0x80) != 0) {
                    goto next_hit;
                }
                if (strcmp(**(char***)((s32)hit + 0x8), name) != 0) {
                    goto next_hit;
                }
                goto found;

next_hit:
                hitIndex++;
                hit = (void*)((s32)hit + 0xE4);
            }
            group = (void*)((s32)group + 0x178);
            groupIndex++;
        }
        hit = NULL;
    }

found:
    if (hit == NULL) {
        mapErrorEntry(1, name);
    } else {
        hitFlgOff(hit, value, 0);
    }
}

void hitObjAttrOn(char* name, u32 attr) {
    extern void* mapGetWork(void);
    extern s32 strcmp(const char* str1, const char* str2);
    extern void mapErrorEntry(s32 type, char* message);

    s32 groupIndex;
    void* hit;
    s32 hitIndex;
    void* map;
    void* group;

    map = mapGetWork();
    if (name == NULL) {
        hit = NULL;
    } else {
        group = map;
        groupIndex = 0;
        while (groupIndex < *(s32*)map) {
            hit = *(void**)((s32)group + 0x15C);
            hitIndex = 0;
            while (hitIndex < *(s32*)((s32)group + 0x158)) {
                if ((*(u16*)hit & 0x80) != 0) {
                    goto next_hit;
                }
                if (strcmp(**(char***)((s32)hit + 0x8), name) != 0) {
                    goto next_hit;
                }
                goto found;

next_hit:
                hitIndex++;
                hit = (void*)((s32)hit + 0xE4);
            }
            group = (void*)((s32)group + 0x178);
            groupIndex++;
        }
        hit = NULL;
    }

found:
    if (hit == NULL) {
        mapErrorEntry(1, name);
    } else {
        *(u32*)((s32)hit + 0x4) |= attr;
    }
}

void hitObjAttrOff(char* name, u32 attr) {
    extern void* mapGetWork(void);
    extern s32 strcmp(const char* str1, const char* str2);
    extern void mapErrorEntry(s32 type, char* message);

    s32 groupIndex;
    void* hit;
    s32 hitIndex;
    void* map;
    void* group;

    map = mapGetWork();
    if (name == NULL) {
        hit = NULL;
    } else {
        group = map;
        groupIndex = 0;
        while (groupIndex < *(s32*)map) {
            hit = *(void**)((s32)group + 0x15C);
            hitIndex = 0;
            while (hitIndex < *(s32*)((s32)group + 0x158)) {
                if ((*(u16*)hit & 0x80) != 0) {
                    goto next_hit;
                }
                if (strcmp(**(char***)((s32)hit + 0x8), name) != 0) {
                    goto next_hit;
                }
                goto found;

next_hit:
                hitIndex++;
                hit = (void*)((s32)hit + 0xE4);
            }
            group = (void*)((s32)group + 0x178);
            groupIndex++;
        }
        hit = NULL;
    }

found:
    if (hit == NULL) {
        mapErrorEntry(1, name);
    } else {
        *(u32*)((s32)hit + 0x4) &= ~attr;
    }
}

void hitAtrOn(int param_1, u32 param_2, int param_3) {
    void* iVar1;
    void* iVar2;
    void* iVar3;
    u32 value = param_2;

    *(u32*)(param_1 + 0x04) = *(u32*)(param_1 + 0x04) | value;

    iVar3 = *(void**)(param_1 + 0xD8);
    if (iVar3 != 0) {
        *(u32*)((s32)iVar3 + 0x04) = *(u32*)((s32)iVar3 + 0x04) | value;

        iVar2 = *(void**)((s32)iVar3 + 0xD8);
        if (iVar2 != 0) {
            *(u32*)((s32)iVar2 + 0x04) = *(u32*)((s32)iVar2 + 0x04) | value;

            iVar1 = *(void**)((s32)iVar2 + 0xD8);
            if (iVar1 != 0) {
                *(u32*)((s32)iVar1 + 0x04) = *(u32*)((s32)iVar1 + 0x04) | value;
                if (*(void**)((s32)iVar1 + 0xD8) != 0) {
                    hitAtrOn((s32)*(void**)((s32)iVar1 + 0xD8), param_2, 1);
                }
                if (*(void**)((s32)iVar1 + 0xDC) != 0) {
                    hitAtrOn((s32)*(void**)((s32)iVar1 + 0xDC), param_2, 1);
                }
            }

            iVar2 = *(void**)((s32)iVar2 + 0xDC);
            if (iVar2 != 0) {
                *(u32*)((s32)iVar2 + 0x04) = *(u32*)((s32)iVar2 + 0x04) | value;
                if (*(void**)((s32)iVar2 + 0xD8) != 0) {
                    hitAtrOn((s32)*(void**)((s32)iVar2 + 0xD8), param_2, 1);
                }
                if (*(void**)((s32)iVar2 + 0xDC) != 0) {
                    hitAtrOn((s32)*(void**)((s32)iVar2 + 0xDC), param_2, 1);
                }
            }
        }

        iVar3 = *(void**)((s32)iVar3 + 0xDC);
        if (iVar3 != 0) {
            *(u32*)((s32)iVar3 + 0x04) = *(u32*)((s32)iVar3 + 0x04) | value;

            iVar2 = *(void**)((s32)iVar3 + 0xD8);
            if (iVar2 != 0) {
                *(u32*)((s32)iVar2 + 0x04) = *(u32*)((s32)iVar2 + 0x04) | value;
                if (*(void**)((s32)iVar2 + 0xD8) != 0) {
                    hitAtrOn((s32)*(void**)((s32)iVar2 + 0xD8), param_2, 1);
                }
                if (*(void**)((s32)iVar2 + 0xDC) != 0) {
                    hitAtrOn((s32)*(void**)((s32)iVar2 + 0xDC), param_2, 1);
                }
            }

            iVar3 = *(void**)((s32)iVar3 + 0xDC);
            if (iVar3 != 0) {
                *(u32*)((s32)iVar3 + 0x04) = *(u32*)((s32)iVar3 + 0x04) | value;
                if (*(void**)((s32)iVar3 + 0xD8) != 0) {
                    hitAtrOn((s32)*(void**)((s32)iVar3 + 0xD8), param_2, 1);
                }
                if (*(void**)((s32)iVar3 + 0xDC) != 0) {
                    hitAtrOn((s32)*(void**)((s32)iVar3 + 0xDC), param_2, 1);
                }
            }
        }
    }

    if (param_3 != 0) {
        iVar3 = *(void**)(param_1 + 0xDC);
        if (iVar3 != 0) {
            *(u32*)((s32)iVar3 + 0x04) = *(u32*)((s32)iVar3 + 0x04) | value;

            iVar2 = *(void**)((s32)iVar3 + 0xD8);
            if (iVar2 != 0) {
                *(u32*)((s32)iVar2 + 0x04) = *(u32*)((s32)iVar2 + 0x04) | value;

                iVar1 = *(void**)((s32)iVar2 + 0xD8);
                if (iVar1 != 0) {
                    *(u32*)((s32)iVar1 + 0x04) = *(u32*)((s32)iVar1 + 0x04) | value;
                    if (*(void**)((s32)iVar1 + 0xD8) != 0) {
                        hitAtrOn((s32)*(void**)((s32)iVar1 + 0xD8), param_2, 1);
                    }
                    if (*(void**)((s32)iVar1 + 0xDC) != 0) {
                        hitAtrOn((s32)*(void**)((s32)iVar1 + 0xDC), param_2, 1);
                    }
                }

                iVar2 = *(void**)((s32)iVar2 + 0xDC);
                if (iVar2 != 0) {
                    *(u32*)((s32)iVar2 + 0x04) = *(u32*)((s32)iVar2 + 0x04) | value;
                    if (*(void**)((s32)iVar2 + 0xD8) != 0) {
                        hitAtrOn((s32)*(void**)((s32)iVar2 + 0xD8), param_2, 1);
                    }
                    if (*(void**)((s32)iVar2 + 0xDC) != 0) {
                        hitAtrOn((s32)*(void**)((s32)iVar2 + 0xDC), param_2, 1);
                    }
                }
            }

            iVar3 = *(void**)((s32)iVar3 + 0xDC);
            if (iVar3 != 0) {
                *(u32*)((s32)iVar3 + 0x04) = *(u32*)((s32)iVar3 + 0x04) | value;

                iVar2 = *(void**)((s32)iVar3 + 0xD8);
                if (iVar2 != 0) {
                    *(u32*)((s32)iVar2 + 0x04) = *(u32*)((s32)iVar2 + 0x04) | value;
                    if (*(void**)((s32)iVar2 + 0xD8) != 0) {
                        hitAtrOn((s32)*(void**)((s32)iVar2 + 0xD8), param_2, 1);
                    }
                    if (*(void**)((s32)iVar2 + 0xDC) != 0) {
                        hitAtrOn((s32)*(void**)((s32)iVar2 + 0xDC), param_2, 1);
                    }
                }

                iVar3 = *(void**)((s32)iVar3 + 0xDC);
                if (iVar3 != 0) {
                    *(u32*)((s32)iVar3 + 0x04) = *(u32*)((s32)iVar3 + 0x04) | value;
                    if (*(void**)((s32)iVar3 + 0xD8) != 0) {
                        hitAtrOn((s32)*(void**)((s32)iVar3 + 0xD8), param_2, 1);
                    }
                    if (*(void**)((s32)iVar3 + 0xDC) != 0) {
                        hitAtrOn((s32)*(void**)((s32)iVar3 + 0xDC), param_2, 1);
                    }
                }
            }
        }
    }

    return;
}

void hitAtrOff(int param_1, u32 param_2, int param_3) {
    void* iVar1;
    void* iVar2;
    void* iVar3;
    u32 value = ~param_2;

    *(u32*)(param_1 + 0x04) = *(u32*)(param_1 + 0x04) & value;

    iVar3 = *(void**)(param_1 + 0xD8);
    if (iVar3 != 0) {
        *(u32*)((s32)iVar3 + 0x04) = *(u32*)((s32)iVar3 + 0x04) & value;

        iVar2 = *(void**)((s32)iVar3 + 0xD8);
        if (iVar2 != 0) {
            *(u32*)((s32)iVar2 + 0x04) = *(u32*)((s32)iVar2 + 0x04) & value;

            iVar1 = *(void**)((s32)iVar2 + 0xD8);
            if (iVar1 != 0) {
                *(u32*)((s32)iVar1 + 0x04) = *(u32*)((s32)iVar1 + 0x04) & value;
                if (*(void**)((s32)iVar1 + 0xD8) != 0) {
                    hitAtrOff((s32)*(void**)((s32)iVar1 + 0xD8), param_2, 1);
                }
                if (*(void**)((s32)iVar1 + 0xDC) != 0) {
                    hitAtrOff((s32)*(void**)((s32)iVar1 + 0xDC), param_2, 1);
                }
            }

            iVar2 = *(void**)((s32)iVar2 + 0xDC);
            if (iVar2 != 0) {
                *(u32*)((s32)iVar2 + 0x04) = *(u32*)((s32)iVar2 + 0x04) & value;
                if (*(void**)((s32)iVar2 + 0xD8) != 0) {
                    hitAtrOff((s32)*(void**)((s32)iVar2 + 0xD8), param_2, 1);
                }
                if (*(void**)((s32)iVar2 + 0xDC) != 0) {
                    hitAtrOff((s32)*(void**)((s32)iVar2 + 0xDC), param_2, 1);
                }
            }
        }

        iVar3 = *(void**)((s32)iVar3 + 0xDC);
        if (iVar3 != 0) {
            *(u32*)((s32)iVar3 + 0x04) = *(u32*)((s32)iVar3 + 0x04) & value;

            iVar2 = *(void**)((s32)iVar3 + 0xD8);
            if (iVar2 != 0) {
                *(u32*)((s32)iVar2 + 0x04) = *(u32*)((s32)iVar2 + 0x04) & value;
                if (*(void**)((s32)iVar2 + 0xD8) != 0) {
                    hitAtrOff((s32)*(void**)((s32)iVar2 + 0xD8), param_2, 1);
                }
                if (*(void**)((s32)iVar2 + 0xDC) != 0) {
                    hitAtrOff((s32)*(void**)((s32)iVar2 + 0xDC), param_2, 1);
                }
            }

            iVar3 = *(void**)((s32)iVar3 + 0xDC);
            if (iVar3 != 0) {
                *(u32*)((s32)iVar3 + 0x04) = *(u32*)((s32)iVar3 + 0x04) & value;
                if (*(void**)((s32)iVar3 + 0xD8) != 0) {
                    hitAtrOff((s32)*(void**)((s32)iVar3 + 0xD8), param_2, 1);
                }
                if (*(void**)((s32)iVar3 + 0xDC) != 0) {
                    hitAtrOff((s32)*(void**)((s32)iVar3 + 0xDC), param_2, 1);
                }
            }
        }
    }

    if (param_3 != 0) {
        iVar3 = *(void**)(param_1 + 0xDC);
        if (iVar3 != 0) {
            *(u32*)((s32)iVar3 + 0x04) = *(u32*)((s32)iVar3 + 0x04) & value;

            iVar2 = *(void**)((s32)iVar3 + 0xD8);
            if (iVar2 != 0) {
                *(u32*)((s32)iVar2 + 0x04) = *(u32*)((s32)iVar2 + 0x04) & value;

                iVar1 = *(void**)((s32)iVar2 + 0xD8);
                if (iVar1 != 0) {
                    *(u32*)((s32)iVar1 + 0x04) = *(u32*)((s32)iVar1 + 0x04) & value;
                    if (*(void**)((s32)iVar1 + 0xD8) != 0) {
                        hitAtrOff((s32)*(void**)((s32)iVar1 + 0xD8), param_2, 1);
                    }
                    if (*(void**)((s32)iVar1 + 0xDC) != 0) {
                        hitAtrOff((s32)*(void**)((s32)iVar1 + 0xDC), param_2, 1);
                    }
                }

                iVar2 = *(void**)((s32)iVar2 + 0xDC);
                if (iVar2 != 0) {
                    *(u32*)((s32)iVar2 + 0x04) = *(u32*)((s32)iVar2 + 0x04) & value;
                    if (*(void**)((s32)iVar2 + 0xD8) != 0) {
                        hitAtrOff((s32)*(void**)((s32)iVar2 + 0xD8), param_2, 1);
                    }
                    if (*(void**)((s32)iVar2 + 0xDC) != 0) {
                        hitAtrOff((s32)*(void**)((s32)iVar2 + 0xDC), param_2, 1);
                    }
                }
            }

            iVar3 = *(void**)((s32)iVar3 + 0xDC);
            if (iVar3 != 0) {
                *(u32*)((s32)iVar3 + 0x04) = *(u32*)((s32)iVar3 + 0x04) & value;

                iVar2 = *(void**)((s32)iVar3 + 0xD8);
                if (iVar2 != 0) {
                    *(u32*)((s32)iVar2 + 0x04) = *(u32*)((s32)iVar2 + 0x04) & value;
                    if (*(void**)((s32)iVar2 + 0xD8) != 0) {
                        hitAtrOff((s32)*(void**)((s32)iVar2 + 0xD8), param_2, 1);
                    }
                    if (*(void**)((s32)iVar2 + 0xDC) != 0) {
                        hitAtrOff((s32)*(void**)((s32)iVar2 + 0xDC), param_2, 1);
                    }
                }

                iVar3 = *(void**)((s32)iVar3 + 0xDC);
                if (iVar3 != 0) {
                    *(u32*)((s32)iVar3 + 0x04) = *(u32*)((s32)iVar3 + 0x04) & value;
                    if (*(void**)((s32)iVar3 + 0xD8) != 0) {
                        hitAtrOff((s32)*(void**)((s32)iVar3 + 0xD8), param_2, 1);
                    }
                    if (*(void**)((s32)iVar3 + 0xDC) != 0) {
                        hitAtrOff((s32)*(void**)((s32)iVar3 + 0xDC), param_2, 1);
                    }
                }
            }
        }
    }

    return;
}

void hitGrpAttrOn(char* name, u32 value) {
    extern void* mapGetWork(void);
    extern s32 strcmp(const char* str1, const char* str2);
    extern void mapErrorEntry(s32 type, char* message);
    extern void hitAtrOn(void* hit, u32 value, s32 recursive);

    s32 groupIndex;
    void* hit;
    s32 hitIndex;
    void* map;
    void* group;

    map = mapGetWork();
    if (name == NULL) {
        hit = NULL;
    } else {
        group = map;
        groupIndex = 0;
        while (groupIndex < *(s32*)map) {
            hit = *(void**)((s32)group + 0x15C);
            hitIndex = 0;
            while (hitIndex < *(s32*)((s32)group + 0x158)) {
                if ((*(u16*)hit & 0x80) != 0) {
                    goto next_hit;
                }
                if (strcmp(**(char***)((s32)hit + 0x8), name) != 0) {
                    goto next_hit;
                }
                goto found;

next_hit:
                hitIndex++;
                hit = (void*)((s32)hit + 0xE4);
            }
            group = (void*)((s32)group + 0x178);
            groupIndex++;
        }
        hit = NULL;
    }

found:
    if (hit == NULL) {
        mapErrorEntry(1, name);
    } else {
        hitAtrOn(hit, value, 0);
    }
}

void hitGrpAttrOff(char* name, u32 value) {
    extern void* mapGetWork(void);
    extern s32 strcmp(const char* str1, const char* str2);
    extern void mapErrorEntry(s32 type, char* message);
    extern void hitAtrOff(void* hit, u32 value, s32 recursive);

    s32 groupIndex;
    void* hit;
    s32 hitIndex;
    void* map;
    void* group;

    map = mapGetWork();
    if (name == NULL) {
        hit = NULL;
    } else {
        group = map;
        groupIndex = 0;
        while (groupIndex < *(s32*)map) {
            hit = *(void**)((s32)group + 0x15C);
            hitIndex = 0;
            while (hitIndex < *(s32*)((s32)group + 0x158)) {
                if ((*(u16*)hit & 0x80) != 0) {
                    goto next_hit;
                }
                if (strcmp(**(char***)((s32)hit + 0x8), name) != 0) {
                    goto next_hit;
                }
                goto found;

next_hit:
                hitIndex++;
                hit = (void*)((s32)hit + 0xE4);
            }
            group = (void*)((s32)group + 0x178);
            groupIndex++;
        }
        hit = NULL;
    }

found:
    if (hit == NULL) {
        mapErrorEntry(1, name);
    } else {
        hitAtrOff(hit, value, 0);
    }
}

void hitReCalcMatrix(void* hit, void* arg) {
    f32 pos[3];

    *(u16*)hit |= 0x40;

    hitReCalcMatrix2(hit, arg, 0);

    PSMTXMultVec(
        (void*)((s32)hit + 0xC),
        (void*)(*(s32*)((s32)hit + 0x8) + 0x48),
        pos
    );

    *(f32*)((s32)hit + 0xCC) = PSVECDistance(
        pos,
        (void*)((s32)hit + 0xC0)
    );
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

void hitReCalcMatrix2(void* hit, void* arg, s32 flag) {
    extern void PSMTXConcat(void* a, void* b, void* out);
    extern void PSMTXMultVec(void* mtx, void* src, void* dst);

    void* child;
    void* sub;
    u16 flags;
    s32 doRecalc;

#define DO_RECALC_NODE(node_, parent_)                                                     \
    do {                                                                                    \
        if ((*(u16*)(node_) & 0x10) != 0) {                                                 \
            PSMTXConcat((parent_), (void*)((s32)(node_) + 0x6C), (void*)((s32)(node_) + 0x0C)); \
        } else {                                                                            \
            PSMTXConcat((parent_), (void*)((s32)(node_) + 0x3C), (void*)((s32)(node_) + 0x0C)); \
        }                                                                                   \
        if ((*(u16*)(node_) & 0x20) != 0) {                                                 \
            PSMTXConcat((void*)((s32)(node_) + 0x0C),                                      \
                        (void*)(*(s32*)((s32)(node_) + 0xD0) + 0xAC),                       \
                        (void*)((s32)(node_) + 0x0C));                                     \
        }                                                                                   \
        PSMTXMultVec((void*)((s32)(node_) + 0x0C),                                         \
                     (void*)((s32)(node_) + 0x9C),                                         \
                     (void*)((s32)(node_) + 0xC0));                                        \
        hitCalcVtxPosition((node_));                                                        \
        *(u16*)(node_) = *(u16*)(node_) & ~0x40;                                            \
    } while (0)

    flags = *(u16*)hit;
    doRecalc = 0;
    if ((flags & 0x40) != 0) {
        doRecalc = 1;
    }
    if (flag != 0) {
        doRecalc = 1;
    }

    if (doRecalc != 0) {
        DO_RECALC_NODE(hit, arg);

        child = *(void**)((s32)hit + 0xD8);
        if (child != 0) {
            DO_RECALC_NODE(child, (void*)((s32)hit + 0x0C));

            sub = *(void**)((s32)child + 0xD8);
            if (sub != 0) {
                hitReCalcMatrix2(sub, (void*)((s32)child + 0x0C), 1);
            }
            sub = *(void**)((s32)child + 0xDC);
            if (sub != 0) {
                hitReCalcMatrix2(sub, (void*)((s32)hit + 0x0C), 1);
            }
        }

        child = *(void**)((s32)hit + 0xDC);
        if (child != 0) {
            flags = *(u16*)child;
            doRecalc = 0;
            if ((flags & 0x40) != 0) {
                doRecalc = 1;
            }
            if (flag != 0) {
                doRecalc = 1;
            }

            if (doRecalc != 0) {
                DO_RECALC_NODE(child, arg);

                sub = *(void**)((s32)child + 0xD8);
                if (sub != 0) {
                    hitReCalcMatrix2(sub, (void*)((s32)child + 0x0C), 1);
                }
                sub = *(void**)((s32)child + 0xDC);
                if (sub != 0) {
                    hitReCalcMatrix2(sub, arg, flag);
                }
            } else {
                sub = *(void**)((s32)child + 0xD8);
                if (sub != 0) {
                    hitReCalcMatrix2(sub, (void*)((s32)child + 0x0C), flag);
                }
                sub = *(void**)((s32)child + 0xDC);
                if (sub != 0) {
                    hitReCalcMatrix2(sub, arg, flag);
                }
            }
        }
    } else {
        child = *(void**)((s32)hit + 0xD8);
        if (child != 0) {
            flags = *(u16*)child;
            doRecalc = 0;
            if ((flags & 0x40) != 0) {
                doRecalc = 1;
            }
            if (flag != 0) {
                doRecalc = 1;
            }

            if (doRecalc != 0) {
                DO_RECALC_NODE(child, (void*)((s32)hit + 0x0C));

                sub = *(void**)((s32)child + 0xD8);
                if (sub != 0) {
                    hitReCalcMatrix2(sub, (void*)((s32)child + 0x0C), 1);
                }
                sub = *(void**)((s32)child + 0xDC);
                if (sub != 0) {
                    hitReCalcMatrix2(sub, (void*)((s32)hit + 0x0C), flag);
                }
            } else {
                sub = *(void**)((s32)child + 0xD8);
                if (sub != 0) {
                    hitReCalcMatrix2(sub, (void*)((s32)child + 0x0C), flag);
                }
                sub = *(void**)((s32)child + 0xDC);
                if (sub != 0) {
                    hitReCalcMatrix2(sub, (void*)((s32)hit + 0x0C), flag);
                }
            }
        }

        child = *(void**)((s32)hit + 0xDC);
        if (child != 0) {
            flags = *(u16*)child;
            doRecalc = 0;
            if ((flags & 0x40) != 0) {
                doRecalc = 1;
            }
            if (flag != 0) {
                doRecalc = 1;
            }

            if (doRecalc != 0) {
                DO_RECALC_NODE(child, arg);

                sub = *(void**)((s32)child + 0xD8);
                if (sub != 0) {
                    hitReCalcMatrix2(sub, (void*)((s32)child + 0x0C), 1);
                }
                sub = *(void**)((s32)child + 0xDC);
                if (sub != 0) {
                    hitReCalcMatrix2(sub, arg, flag);
                }
            } else {
                sub = *(void**)((s32)child + 0xD8);
                if (sub != 0) {
                    hitReCalcMatrix2(sub, (void*)((s32)child + 0x0C), flag);
                }
                sub = *(void**)((s32)child + 0xDC);
                if (sub != 0) {
                    hitReCalcMatrix2(sub, arg, flag);
                }
            }
        }
    }

#undef DO_RECALC_NODE
}

#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset


#pragma no_register_save_helpers on
#pragma use_lmw_stmw on
s32 hitCalcVtxPosition(void* hit) {
    extern const f32 float_0_8041f838;
    extern void* mapGetWork(void);
    extern void PSMTXMultVec(void*, void*, void*);
    extern void PSVECSubtract(void*, void*, void*);
    extern void PSVECCrossProduct(void*, void*, void*);
    typedef struct VecCalcUnpacked {
        f32 x;
        f32 y;
        f32 z;
    } VecCalcUnpacked;
    typedef struct HitVectorCalcUnpacked {
        VecCalcUnpacked v0;
        VecCalcUnpacked v1;
        VecCalcUnpacked v2;
        VecCalcUnpacked edge0;
        VecCalcUnpacked edge1;
        VecCalcUnpacked edge2;
        VecCalcUnpacked normal;
    } HitVectorCalcUnpacked;
    typedef struct HitEntryCalc {
        u16 flags;
        u8 unk2[2];
        s32 attributes;
        void* joint;
        f32 worldMtx[3][4];
        u8 unk3C[0xA8 - 0x3C];
        s16 unkA8;
        s16 mapIndex;
        void* vectors;
    } HitEntryCalc;
    HitEntryCalc* entry = (HitEntryCalc*)hit;
    void* joint;
    void* shape;
    s32* poly;
    void* tri;
    s32 count = 0;
    s32 jointOffset;
    s32 jointIndex;
    s32 shapeIndex;
    s32 triIndex;
    s32 triOffset;

    if (entry->vectors == NULL) {
        return 0;
    }
    for (jointIndex = 0, jointOffset = 0;
         jointIndex < *(s32*)((s32)entry->joint + 0x5C);
         jointIndex++, jointOffset += 8) {
        joint = *(void**)((s32)entry->joint + jointOffset + 0x64);
        if (joint != 0) {
            if (*(u8*)((s32)joint + 3) == 0) {
                s32 index0;
                s16 index1;
                s16 index2;
                s16 temp;
                s32* polyIter;
                HitVectorCalcUnpacked* out;
                s32 vertexBase;
                BOOL even;
                shape = joint;
                for (shapeIndex = 0; shapeIndex < *(s32*)((s32)joint + 4); shapeIndex++) {
                    poly = *(s32**)((s32)shape + 0x10);
                    polyIter = poly;
                    triOffset = count * 0x54;
                    for (triIndex = 0; triIndex < *poly - 2; triIndex++) {
                        tri = (void*)((s32)entry->vectors + triOffset);
                        out = (HitVectorCalcUnpacked*)tri;
                        index1 = *(s16*)((s32)poly + (triIndex + 1) * 0x18 + 4);
                        index2 = *(s16*)((s32)poly + (triIndex + 2) * 0x18 + 4);
                        count++;
                        triOffset += 0x54;
                        vertexBase = **(s32**)((s32)joint + 0xC) + 4;
                        PSMTXMultVec(entry->worldMtx,
                                     (void*)(vertexBase +
                                             *(s16*)((s32)polyIter + 4) * 0xC), &out->v0);
                        even = (triIndex & 1) == 0;
                        temp = index1;
                        if (even) temp = index2;
                        PSMTXMultVec(entry->worldMtx,
                                     (void*)(vertexBase + temp * 0xC),
                                     &out->v1);
                        temp = index2;
                        if (even) temp = index1;
                        PSMTXMultVec(entry->worldMtx,
                                     (void*)(vertexBase + temp * 0xC),
                                     &out->v2);
                        PSVECSubtract(&out->v2, &out->v0, &out->edge0);
                        PSVECSubtract(&out->v0, &out->v1, &out->edge1);
                        PSVECSubtract(&out->v1, &out->v2, &out->edge2);
                        PSVECCrossProduct(&out->edge0, &out->edge1, &out->normal);
                        polyIter += 6;
                    }
                    shape = (void*)((s32)shape + 4);
                }
            } else {
                s32 vertexCount;
                u32 index0;
                u32 index1;
                u32 index2;
                u16 temp;
                BOOL even;
                HitVectorCalcUnpacked* out;
                void* map = mapGetWork();
                u32 flags = *(u32*)((s32)joint + 8);
                s32 bits = 0;
                s32 stride;
                s32 i;
                s32 baseOffset = count * 0x54;
                f32 vertex0[3];
                f32 vertex1[3];
                f32 vertex2[3];
                (void)map;
                for (i = 0; i < 12; i++) {
                    bits += (flags >> i) & 1;
                }
                stride = bits * 2;
                shape = joint;
                for (shapeIndex = 0; shapeIndex < *(s32*)((s32)joint + 4); shapeIndex++) {
                    poly = *(s32**)((s32)shape + 0x10);
                    vertexCount = *(u16*)((s32)poly + 1);
                    triOffset = baseOffset;
                    for (triIndex = 0; triIndex < vertexCount - 2; triIndex++) {
                        s32 vertexBase = **(s32**)((s32)joint + 0xC) + 4;
                        register u8* qbase;
                        register u32 qoffset;
                        register u32 qreg;
                        register f32 qvalue;
                        index0 = *(u16*)((s32)poly + stride * triIndex + 3);
                        index1 = *(u16*)((s32)poly + stride * (triIndex + 1) + 3);
                        index2 = *(u16*)((s32)poly + stride * (triIndex + 2) + 3);
                        tri = (void*)((s32)entry->vectors + triOffset);
                        out = (HitVectorCalcUnpacked*)tri;
                        even = (triIndex & 1) == 0;
                        temp = index1;
                        if (even) temp = index2;
                        index1 = temp;
                        temp = index2;
                        if (even) temp = *(u16*)((s32)poly + stride * (triIndex + 1) + 3);
                        index2 = temp;
                        index0 *= 3;
                        index1 *= 3;
                        index2 *= 3;
                        qreg = *(u32*)((s32)*(void**)((s32)map + entry->mapIndex * 0x178 + 0xA4) + 0x44);
                        qreg = (qreg << 8) | 7;
                        qreg |= qreg << 16;
                        asm { mtspr GQR5, qreg }
                        qbase = (u8*)vertexBase;
                        qoffset = index0 * 2;
                        asm { psq_lx qvalue, qbase, qoffset, 1, 5 }
                        vertex0[0] = qvalue;
                        qoffset = (index0 + 1) * 2;
                        asm { psq_lx qvalue, qbase, qoffset, 1, 5 }
                        vertex0[1] = qvalue;
                        qoffset = (index0 + 2) * 2;
                        asm { psq_lx qvalue, qbase, qoffset, 1, 5 }
                        vertex0[2] = qvalue;
                        qoffset = index1 * 2;
                        asm { psq_lx qvalue, qbase, qoffset, 1, 5 }
                        vertex1[0] = qvalue;
                        qoffset = (index1 + 1) * 2;
                        asm { psq_lx qvalue, qbase, qoffset, 1, 5 }
                        vertex1[1] = qvalue;
                        qoffset = (index1 + 2) * 2;
                        asm { psq_lx qvalue, qbase, qoffset, 1, 5 }
                        vertex1[2] = qvalue;
                        qoffset = index2 * 2;
                        asm { psq_lx qvalue, qbase, qoffset, 1, 5 }
                        vertex2[0] = qvalue;
                        qoffset = (index2 + 1) * 2;
                        asm { psq_lx qvalue, qbase, qoffset, 1, 5 }
                        vertex2[1] = qvalue;
                        qoffset = (index2 + 2) * 2;
                        asm { psq_lx qvalue, qbase, qoffset, 1, 5 }
                        vertex2[2] = qvalue;
                        PSMTXMultVec(entry->worldMtx, vertex0, &out->v0);
                        PSMTXMultVec(entry->worldMtx, vertex1, &out->v1);
                        PSMTXMultVec(entry->worldMtx, vertex2, &out->v2);
                        PSVECSubtract(&out->v2, &out->v0, &out->edge0);
                        PSVECSubtract(&out->v0, &out->v1, &out->edge1);
                        PSVECSubtract(&out->v1, &out->v2, &out->edge2);
                        PSVECCrossProduct(&out->edge0, &out->edge1, &out->normal);
                        if (out->normal.x != float_0_8041f838 ||
                            out->normal.y != float_0_8041f838 ||
                            out->normal.z != float_0_8041f838) {
                            triOffset += 0x54;
                            baseOffset += 0x54;
                            count++;
                        }
                    }
                    shape = (void*)((s32)shape + 8);
                }
            }
        }
    }
    return count;
}


typedef struct HitVecLocal {
    f32 x;
    f32 y;
    f32 z;
} Vec;

typedef struct HitVector {
    Vec v0;
    Vec v1;
    Vec v2;
    Vec edge0;
    Vec edge1;
    Vec edge2;
    Vec normal;
} HitVector;

typedef struct HitDrawModeLocal {
    u8 unk0;
    u8 cullMode;
} HitDrawModeLocal;

typedef struct HitJointLocal {
    u8 unk0[0x58];
    HitDrawModeLocal* drawMode;
} HitJointLocal;

struct HitCheckQuery {
    BOOL singleSided;
    s32 user0;
    s32 user1;
    Vec targetPos;
    Vec targetDir;
    Vec hitPos;
    Vec hitNormal;
    f32 targetDistance;
};

struct HitEntry {
    u16 flags;
    u8 unk2[2];
    s32 attributes;
    HitJointLocal* joint;
    u8 unkC[0x9C - 0xC];
    Vec centerLocal;
    s16 unkA8;
    s16 mapIndex;
    HitVector* vectors;
    void* damage;
    u8 unkB4[0xC0 - 0xB4];
    Vec centerWorld;
    f32 radius;
    void* mapObj;
    struct HitEntry* parent;
    struct HitEntry* child;
    struct HitEntry* sibling;
    struct HitEntry* siblingActive;
};

extern void* unk_8041e628;
extern const f32 float_0_8041f838;
extern const f32 float_1_8041f844;
extern const f32 float_neg1_8041f848;
extern const f32 float_0p5_8041f84c;
extern f32 PSVECSquareMag(void* v);
extern void PSVECScale(void* src, void* dst, f32 scale);
extern void PSVECAdd(void* a, void* b, void* out);
extern void PSVECSubtract(void* a, void* b, void* out);
extern f32 PSVECDistance(void* a, void* b);
extern f32 PSVECDotProduct(void* a, void* b);
extern void PSVECCrossProduct(void* a, void* b, void* out);
extern void PSVECNormalize(void* src, void* dst);
extern u8 checkTriVec_xz(void* ray, void* tri);
inline BOOL tempfunc(HitCheckQuery* query, HitVector* vector) {
    Vec sp80;
    Vec sp8C;
    Vec sp98;
    f32 dotProduct;
    f32 temp_f2;

    PSVECSubtract(&query->targetPos, &vector->v0, &sp80);
    dotProduct = PSVECDotProduct(&vector->normal, &sp80);
    if (query->singleSided) {
        if (dotProduct < float_0_8041f838) {
            return 0;
        }
        if (((sp80.z * vector->edge0.x) - (sp80.x * vector->edge0.z)) < float_0_8041f838) {
            return 0;
        }
        PSVECSubtract(&query->targetPos, &vector->v1, &sp8C);
        if (((sp8C.z * vector->edge1.x) - (sp8C.x * vector->edge1.z)) < float_0_8041f838) {
            return 0;
        }
        PSVECSubtract(&query->targetPos, &vector->v2, &sp98);
        if (((sp98.z * vector->edge2.x) - (sp98.x * vector->edge2.z)) < float_0_8041f838) {
            return 0;
        }
    } else {
        if ((vector->normal.y * dotProduct) <= float_0_8041f838) {
            return 0;
        }
        if ((dotProduct * ((sp80.z * vector->edge0.x) - (sp80.x * vector->edge0.z))) < float_0_8041f838) {
            return 0;
        }
        PSVECSubtract(&query->targetPos, &vector->v1, &sp8C);
        if ((dotProduct * ((sp8C.z * vector->edge1.x) - (sp8C.x * vector->edge1.z))) < float_0_8041f838) {
            return 0;
        }
        PSVECSubtract(&query->targetPos, &vector->v2, &sp98);
        if ((dotProduct * ((sp98.z * vector->edge2.x) - (sp98.x * vector->edge2.z))) < float_0_8041f838) {
            return 0;
        }
    }
    temp_f2 = -vector->normal.y;
    if ((query->targetDistance >= float_0_8041f838) && ((-dotProduct / temp_f2) >= query->targetDistance)) {
        return 0;
    }
    query->targetDistance = -dotProduct / temp_f2;
    query->hitPos.x = query->targetPos.x;
    query->hitPos.y = query->targetPos.y - query->targetDistance;
    query->hitPos.z = query->targetPos.z;
    if (dotProduct >= float_0_8041f838) {
        query->hitNormal = vector->normal;
    } else {
        PSVECScale(&vector->normal, &query->hitNormal, float_neg1_8041f848);
    }
    return 1;
}

inline BOOL tempfunc2(HitCheckQuery* query, HitVector* vector) {
    Vec sp5C;
    Vec sp68;
    Vec sp74;
    f32 dotProduct;
    f32 temp_f2;

    PSVECSubtract(&query->targetPos, &vector->v0, &sp5C);
    dotProduct = PSVECDotProduct(&vector->normal, &sp5C);
    if (query->singleSided) {
        if (dotProduct < float_0_8041f838) {
            return 0;
        }
        if (((sp5C.z * vector->edge0.x) - (sp5C.x * vector->edge0.z)) > float_0_8041f838) {
            return 0;
        }
        PSVECSubtract(&query->targetPos, &vector->v1, &sp68);
        if (((sp68.z * vector->edge1.x) - (sp68.x * vector->edge1.z)) > float_0_8041f838) {
            return 0;
        }
        PSVECSubtract(&query->targetPos, &vector->v2, &sp74);
        if (((sp74.z * vector->edge2.x) - (sp74.x * vector->edge2.z)) > float_0_8041f838) {
            return 0;
        }
    } else {
        if ((vector->normal.y * dotProduct) >= float_0_8041f838) {
            return 0;
        }
        if ((dotProduct * ((sp5C.z * vector->edge0.x) - (sp5C.x * vector->edge0.z))) > float_0_8041f838) {
            return 0;
        }
        PSVECSubtract(&query->targetPos, &vector->v1, &sp68);
        if ((dotProduct * ((sp68.z * vector->edge1.x) - (sp68.x * vector->edge1.z))) > float_0_8041f838) {
            return 0;
        }
        PSVECSubtract(&query->targetPos, &vector->v2, &sp74);
        if ((dotProduct * ((sp74.z * vector->edge2.x) - (sp74.x * vector->edge2.z))) > float_0_8041f838) {
            return 0;
        }
    }
    temp_f2 = vector->normal.y;
    if ((query->targetDistance >= float_0_8041f838) && ((-dotProduct / temp_f2) >= query->targetDistance)) {
        return 0;
    }
    query->targetDistance = -dotProduct / temp_f2;
    query->hitPos.x = query->targetPos.x;
    query->hitPos.y = query->targetPos.y + query->targetDistance;
    query->hitPos.z = query->targetPos.z;
    if (dotProduct >= float_0_8041f838) {
        query->hitNormal = vector->normal;
    } else {
        PSVECScale(&vector->normal, &query->hitNormal, float_neg1_8041f848);
    }
    return 1;
}

inline BOOL tempfunc3(HitCheckQuery* query, HitVector* vector) {
    Vec sp8;
    Vec sp14;
    Vec sp20;
    Vec sp2C;
    Vec sp38;
    Vec sp44;
    Vec sp50;
    f32 dotProduct;
    f32 var_f27;

    PSVECSubtract(&query->targetPos, &vector->v0, &sp8);
    dotProduct = PSVECDotProduct(&vector->normal, &sp8);
    if (query->singleSided) {
        if (dotProduct < float_0_8041f838) {
            return 0;
        }
        PSVECCrossProduct(&query->targetDir, &vector->edge0, &sp2C);
        if (PSVECDotProduct(&sp8, &sp2C) < float_0_8041f838) {
            return 0;
        }
        PSVECSubtract(&query->targetPos, &vector->v1, &sp14);
        PSVECCrossProduct(&query->targetDir, &vector->edge1, &sp38);
        if (PSVECDotProduct(&sp14, &sp38) < float_0_8041f838) {
            return 0;
        }
        PSVECSubtract(&query->targetPos, &vector->v2, &sp20);
        PSVECCrossProduct(&query->targetDir, &vector->edge2, &sp44);
        if (PSVECDotProduct(&sp20, &sp44) < float_0_8041f838) {
            return 0;
        }
        var_f27 = PSVECDotProduct(&vector->normal, &query->targetDir);
    } else {
        var_f27 = PSVECDotProduct(&vector->normal, &query->targetDir);
        if ((var_f27 * dotProduct) >= float_0_8041f838) {
            return 0;
        }
        PSVECCrossProduct(&query->targetDir, &vector->edge0, &sp2C);
        if ((dotProduct * PSVECDotProduct(&sp8, &sp2C)) < float_0_8041f838) {
            return 0;
        }
        PSVECSubtract(&query->targetPos, &vector->v1, &sp14);
        PSVECCrossProduct(&query->targetDir, &vector->edge1, &sp38);
        if ((dotProduct * PSVECDotProduct(&sp14, &sp38)) < float_0_8041f838) {
            return 0;
        }
        PSVECSubtract(&query->targetPos, &vector->v2, &sp20);
        PSVECCrossProduct(&query->targetDir, &vector->edge2, &sp44);
        if ((dotProduct * PSVECDotProduct(&sp20, &sp44)) < float_0_8041f838) {
            return 0;
        }
    }

    if ((query->targetDistance >= float_0_8041f838) && ((-dotProduct / var_f27) >= query->targetDistance)) {
        return 0;
    }
    query->targetDistance = -dotProduct / var_f27;
    PSVECScale(&query->targetDir, &sp50, query->targetDistance);
    PSVECAdd(&query->targetPos, &sp50, &query->hitPos);
    if (dotProduct >= float_0_8041f838) {
        query->hitNormal = vector->normal;
    } else {
        PSVECScale(&vector->normal, &query->hitNormal, float_neg1_8041f848);
    }
    return 1;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw on
HitEntry* hitCheckVecFilter(HitCheckQuery* query, HitFilterCallback callback) {
    f32 temp;
    HitVector* vector;
    Vec position;
    Vec spBC;
    Vec normal;
    Vec spA4;
    HitEntry* var_r29;
    HitEntry* var_r28;
    int i;
    f32 distance;
    f32 var_f29;
    HitVector* var_r31;
    f32 temp_f28;

    mapGetWork();
    var_r28 = NULL;
    if (!PSVECSquareMag(&query->targetDir)) {
        return NULL;
    }
    var_f29 = float_neg1_8041f848;
    temp = float_0p5_8041f84c;
    temp_f28 = query->targetDistance * temp;
    PSVECScale(&query->targetDir, &spA4, temp_f28);
    PSVECAdd(&query->targetPos, &spA4, &spA4);
    var_r29 = (HitEntry*)unk_8041e628;
    if (query->targetDir.y == float_0_8041f838) {
        while (var_r29 != NULL) {
            if (callback == NULL || callback(query, var_r29)) {
                if (!(PSVECDistance(&spA4, &var_r29->centerWorld) > temp_f28 + var_r29->radius)) {
                    if (var_r29->joint->drawMode->cullMode == 1) {
                        query->singleSided = 1;
                    } else {
                        query->singleSided = 0;
                    }
                    var_r31 = var_r29->vectors;
                    for (i = 0; i < var_r29->unkA8; i++, var_r31++) {
                        if ((query->singleSided == 0) ||
                            !(PSVECDotProduct(&var_r31->normal, &query->targetDir) >= float_0_8041f838)) {
                            if (checkTriVec_xz(query, var_r31) && (var_f29 < float_0_8041f838 || var_f29 > query->targetDistance)) {
                                var_f29 = query->targetDistance;
                                var_r28 = var_r29;
                                distance = var_f29;
                                position = query->hitPos;
                                spBC = query->hitNormal;
                            }
                        }
                    }
                }
            }
            var_r29 = var_r29->siblingActive;
        }
    } else if (query->targetDir.x == float_0_8041f838 && query->targetDir.z == float_0_8041f838) {
        if (query->targetDir.y == float_neg1_8041f848) {
            while (var_r29 != NULL) {
                if (callback == NULL || callback(query, var_r29)) {
                    if (!(PSVECDistance(&spA4, &var_r29->centerWorld) > temp_f28 + var_r29->radius)) {
                        if (var_r29->joint->drawMode->cullMode == 1) {
                            query->singleSided = 1;
                        } else {
                            query->singleSided = 0;
                        }
                        vector = var_r29->vectors;
                        for (i = 0; i < var_r29->unkA8; i++, vector++) {
                            if (query->singleSided == 0 || !(vector->normal.y <= float_0_8041f838)) {
                                if (tempfunc(query, vector) && (var_f29 < float_0_8041f838 || var_f29 > query->targetDistance)) {
                                    var_f29 = query->targetDistance;
                                    var_r28 = var_r29;
                                    distance = var_f29;
                                    position = query->hitPos;
                                    spBC = query->hitNormal;
                                }
                            }
                        }
                    }
                }
                var_r29 = var_r29->siblingActive;
            }
        } else if (query->targetDir.y == float_1_8041f844) {
            while (var_r29 != NULL) {
                if (callback == NULL || callback(query, var_r29)) {
                    if (!(PSVECDistance(&spA4, &var_r29->centerWorld) > temp_f28 + var_r29->radius)) {
                        if (var_r29->joint->drawMode->cullMode == 1) {
                            query->singleSided = 1;
                        } else {
                            query->singleSided = 0;
                        }
                        vector = var_r29->vectors;
                        for (i = 0; i < var_r29->unkA8; i++, vector++) {
                            if (query->singleSided == 0 || !(vector->normal.y >= float_0_8041f838)) {
                                if (tempfunc2(query, vector) && (var_f29 < float_0_8041f838 || var_f29 > query->targetDistance)) {
                                    var_f29 = query->targetDistance;
                                    var_r28 = var_r29;
                                    distance = var_f29;
                                    position = query->hitPos;
                                    spBC = query->hitNormal;
                                }
                            }
                        }
                    }
                }
                var_r29 = var_r29->siblingActive;
            }
        } else {
            goto label_235;
        }
    } else {
    label_235:
        while (var_r29 != NULL) {
            if (callback == NULL || callback(query, var_r29)) {
                if (!(PSVECDistance(&spA4, &var_r29->centerWorld) > (temp_f28 + var_r29->radius))) {
                    if (var_r29->joint->drawMode->cullMode == 1) {
                        query->singleSided = 1;
                    } else {
                        query->singleSided = 0;
                    }
                    vector = var_r29->vectors;
                    for (i = 0; i < var_r29->unkA8; i++, vector++) {
                        if (query->singleSided == 0 || !(PSVECDotProduct(&vector->normal, &query->targetDir) >= float_0_8041f838)) {
                            if (tempfunc3(query, vector) && (var_f29 < float_0_8041f838 || var_f29 > query->targetDistance)) {
                                var_f29 = query->targetDistance;
                                var_r28 = var_r29;
                                distance = var_f29;
                                position = query->hitPos;
                                spBC = query->hitNormal;
                            }
                        }
                    }
                }
            }
            var_r29 = var_r29->siblingActive;
        }
    }
    if (var_r28 == NULL) {
        return NULL;
    }
    PSVECNormalize(&spBC, &normal);
    query->targetDistance = distance;
    query->hitPos = position;
    query->hitNormal = normal;
    return var_r28;
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset


#pragma no_register_save_helpers on
#pragma use_lmw_stmw on
void* hitCheckFilter(void* filter, f32* outX, f32* outY, f32* outZ,
                     f32* outDist, f32* outNX, f32* outNY, f32* outNZ,
                     f32 x, f32 y, f32 z, f32 vx, f32 vy, f32 vz) {
    HitCheckQuery temp;
    HitCheckQuery* query = &temp;
    HitEntry* entry;

    query->targetPos = (Vec){ x, y, z };
    query->targetDir = (Vec){ vx, vy, vz };
    query->targetDistance = *outDist;
    entry = hitCheckVecFilter(query, (HitFilterCallback)filter);
    if (entry == NULL) {
        return NULL;
    }

    *outDist = query->targetDistance;
    *outX = query->hitPos.x;
    *outY = query->hitPos.y;
    *outZ = query->hitPos.z;
    *outNX = query->hitNormal.x;
    *outNY = query->hitNormal.y;
    *outNZ = query->hitNormal.z;
    return entry;
}

#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

u8 checkTriVec_xz(void* ray, void* tri) {
    HitCheckQuery* query = (HitCheckQuery*)ray;
    HitVector* vector = (HitVector*)tri;
    Vec v1, v2, v3;
    f32 cosAngle;
    f32 distToTrianglePlane;

    PSVECSubtract(&query->targetPos, &vector->v0, &v1);
    distToTrianglePlane = PSVECDotProduct(&vector->normal, &v1);
    if (query->singleSided != 0) {
        if (distToTrianglePlane < float_0_8041f838) {
            return 0;
        }
        if ((v1.x * (-vector->edge0.y * query->targetDir.z)) +
                (v1.y * ((vector->edge0.x * query->targetDir.z) -
                         (vector->edge0.z * query->targetDir.x))) +
                (v1.z * (vector->edge0.y * query->targetDir.x)) <
            float_0_8041f838) {
            return 0;
        }
        PSVECSubtract(&query->targetPos, &vector->v1, &v2);
        if ((v2.x * (-vector->edge1.y * query->targetDir.z)) +
                (v2.y * ((vector->edge1.x * query->targetDir.z) -
                         (vector->edge1.z * query->targetDir.x))) +
                (v2.z * (vector->edge1.y * query->targetDir.x)) <
            float_0_8041f838) {
            return 0;
        }
        PSVECSubtract(&query->targetPos, &vector->v2, &v3);
        if ((v3.x * (-vector->edge2.y * query->targetDir.z)) +
                (v3.y * ((vector->edge2.x * query->targetDir.z) -
                         (vector->edge2.z * query->targetDir.x))) +
                (v3.z * (vector->edge2.y * query->targetDir.x)) <
            float_0_8041f838) {
            return 0;
        }
    } else {
        if (((vector->normal.x * query->targetDir.x) +
             (vector->normal.z * query->targetDir.z)) *
                distToTrianglePlane >=
            float_0_8041f838) {
            return 0;
        }
        if (((v1.x * (-vector->edge0.y * query->targetDir.z)) +
             (v1.y * ((vector->edge0.x * query->targetDir.z) -
                      (vector->edge0.z * query->targetDir.x))) +
             (v1.z * (vector->edge0.y * query->targetDir.x))) *
                distToTrianglePlane <
            float_0_8041f838) {
            return 0;
        }
        PSVECSubtract(&query->targetPos, &vector->v1, &v2);
        if (((v2.x * (-vector->edge1.y * query->targetDir.z)) +
             (v2.y * ((vector->edge1.x * query->targetDir.z) -
                      (vector->edge1.z * query->targetDir.x))) +
             (v2.z * (vector->edge1.y * query->targetDir.x))) *
                distToTrianglePlane <
            float_0_8041f838) {
            return 0;
        }
        PSVECSubtract(&query->targetPos, &vector->v2, &v3);
        if (((v3.x * (-vector->edge2.y * query->targetDir.z)) +
             (v3.y * ((vector->edge2.x * query->targetDir.z) -
                      (vector->edge2.z * query->targetDir.x))) +
             (v3.z * (vector->edge2.y * query->targetDir.x))) *
                distToTrianglePlane <
            float_0_8041f838) {
            return 0;
        }
    }
    cosAngle = (vector->normal.x * query->targetDir.x) +
               (vector->normal.z * query->targetDir.z);
    if (query->targetDistance >= float_0_8041f838 &&
        -distToTrianglePlane / cosAngle >= query->targetDistance) {
        return 0;
    }
    query->targetDistance = -distToTrianglePlane / cosAngle;
    query->hitPos.x = query->targetPos.x +
                      (query->targetDir.x * query->targetDistance);
    query->hitPos.y = query->targetPos.y;
    query->hitPos.z = query->targetPos.z +
                      (query->targetDir.z * query->targetDistance);
    if (distToTrianglePlane >= float_0_8041f838) {
        query->hitNormal = vector->normal;
    } else {
        PSVECScale(&vector->normal, &query->hitNormal, float_neg1_8041f848);
    }
    return 1;
}

#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset


u8 hitCheckVecHitObjXZ(s32* ray, void* hitObj) {
    HitCheckQuery* query = (HitCheckQuery*)ray;
    HitEntry* entry = (HitEntry*)hitObj;
    HitVector* vector;
    int i;

    if (entry->joint->drawMode->cullMode == 1) {
        query->singleSided = 1;
    } else {
        query->singleSided = 0;
    }
    vector = entry->vectors;
    for (i = 0; i < entry->unkA8; i++, vector++) {
        if ((query->singleSided == 0 ||
             !(PSVECDotProduct(&vector->normal, &query->targetDir) >= float_0_8041f838)) &&
            checkTriVec_xz(query, vector)) {
            return 1;
        }
    }
    return 0;
}

s32 chkFilterAttr(void* a, void* b) {
    return ((*(u32*)((s32)b + 0x4) & *(u32*)((s32)a + 0x4)) == 0);
}

void* hitCheckAttr(s32 flags, f32* outX, f32* outY, f32* outZ, f32* outDist,
                   f32* outNX, f32* outNY, f32* outNZ,
                   f32 x, f32 y, f32 z, f32 vx, f32 vy, f32 vz) {
    HitCheckQuery temp;
    HitCheckQuery* query = &temp;
    HitEntry* entry;

    query->targetPos = (Vec){ x, y, z };
    query->targetDir = (Vec){ vx, vy, vz };
    query->targetDistance = *outDist;
    query->user0 = flags;
    entry = hitCheckVecFilter(query, (HitFilterCallback)chkFilterAttr);
    if (entry == NULL) {
        return NULL;
    }

    *outDist = query->targetDistance;
    *outX = query->hitPos.x;
    *outY = query->hitPos.y;
    *outZ = query->hitPos.z;
    *outNX = query->hitNormal.x;
    *outNY = query->hitNormal.y;
    *outNZ = query->hitNormal.z;
    return entry;
}

void* hitCheckSphereFilter(void* filter, f32 x, f32 y, f32 z, f32 distance) {
    typedef s32 (*HitSphereFilterFunc)(HitCheckQuery* query, HitEntry* entry);
    typedef struct MapEntryLocal {
        s32 count;
        u8 unk4[0x154];
        s32 hitNumJoints;
        HitEntry* hitEntries;
        u8 unk160[0x18];
    } MapEntryLocal;
    typedef struct MapWorkLocal {
        MapEntryLocal entries[1];
    } MapWorkLocal;
    extern const f32 vec3_802bf7e8[3];

    MapWorkLocal* wp = mapGetWork();
    MapEntryLocal* var_r31 = wp->entries;
    MapEntryLocal* map;
    Vec sp68;
    int j;
    HitEntry* temp_r27;
    HitEntry* var_r23;
    HitVector* vector;
    f32 var_f28;
    f32 temp_f1;
    f32 var_f30;
    f32 var_f31;
    s32 entryId;
    s32 objectId;
    s32 var_r29;
    HitCheckQuery temp;
    HitCheckQuery* query;
    int i;

    var_f28 = float_neg1_8041f848;
    objectId = -1;
    query = &temp;
    sp68 = *(Vec*)vec3_802bf7e8;
    sp68.x = x;
    sp68.y = y;
    sp68.z = z;
    for (i = 0; i < var_r31->count; var_r31++, i++) {
        var_r23 = var_r31->hitEntries;
        for (j = 0; j < var_r31->hitNumJoints; j++, var_r23++) {
            if ((var_r23->attributes & 0x80000000) &&
                (*(s32*)((u8*)var_r23->joint + 0x5C) != 0)) {
                var_r29 = var_r23->flags;
                if (!(var_r29 & 1) && !(var_r29 & 0x80)) {
                    temp_f1 = PSVECDistance(&sp68, &var_r23->centerWorld);
                    if (!(temp_f1 > (distance + var_r23->radius))) {
                        if (filter == NULL || ((HitSphereFilterFunc)filter)(query, var_r23)) {
                            if (var_f28 < float_0_8041f838 || var_f28 > temp_f1) {
                                var_f28 = temp_f1;
                                var_f31 = var_r23->radius;
                                objectId = j;
                                entryId = i;
                            }
                        }
                    }
                }
            }
        }
    }
    if (objectId < 0) {
        return NULL;
    }
    if (var_f28 < (float_0p5_8041f84c * var_f31)) {
        return &wp->entries[entryId].hitEntries[objectId];
    }
    map = &wp->entries[entryId];
    temp_r27 = &map->hitEntries[objectId];
    PSVECSubtract(&temp_r27->centerWorld, &sp68, &query->targetDir);
    PSVECNormalize(&query->targetDir, &query->targetDir);
    var_f30 = float_neg1_8041f848;
    var_r29 = -1;
    query->targetPos = sp68;
    query->targetDistance = distance;
    if (temp_r27->joint->drawMode->cullMode == 1) {
        query->singleSided = 1;
    } else {
        query->singleSided = 0;
    }
    vector = temp_r27->vectors;
    for (i = 0; i < temp_r27->unkA8; i++, vector++) {
        if ((query->singleSided == 0) ||
            !(PSVECDotProduct(&vector->normal, &query->targetDir) >= float_0_8041f838)) {
            if (tempfunc3(query, vector) &&
                ((var_f30 < float_0_8041f838) || (var_f30 > query->targetDistance))) {
                var_f30 = query->targetDistance;
                var_r29 = 1;
            }
        }
    }
    if (var_f30 > distance) {
        return NULL;
    }
    if (var_r29 < 0) {
        return NULL;
    }
    return &map->hitEntries[objectId];
}

void* hitNameToPtr(char* hitName) {
    void* map;
    void* group;
    s32 hitIndex;
    s32 groupIndex;
    void* hit;

    map = mapGetWork();
    if (hitName == NULL) {
        return NULL;
    }

    group = map;
    groupIndex = 0;
    while (groupIndex < *(s32*)map) {
        hit = *(void**)((s32)group + 0x15C);
        hitIndex = 0;
        while (hitIndex < *(s32*)((s32)group + 0x158)) {
            if ((*(u16*)hit & 0x80) != 0) {
                goto next_hit;
            }
            if (strcmp(**(char***)((s32)hit + 0x8), hitName) != 0) {
                goto next_hit;
            }
            return hit;

next_hit:
            hitIndex++;
            hit = (void*)((s32)hit + 0xE4);
        }
        group = (void*)((s32)group + 0x178);
        groupIndex++;
    }

    return NULL;
}

void hitObjGetPosSub(u32 param_1, u32 param_2, s32* param_3, BOOL param_4) {
  typedef struct { f32 x; f32 y; f32 z; } HitVec;
  extern void PSMTXMultVec(void* mtx, void* src, void* dst);
  extern void PSVECAdd(void* a, void* b, void* out);
  u32 iVar3;
  u32 iVar2;
  u32 iVar1;
  HitVec VStack_30;
  HitVec VStack_3c;
  HitVec VStack_48;
  HitVec VStack_54;
  HitVec VStack_60;
  HitVec VStack_6c;
  HitVec VStack_78;
  HitVec VStack_84;
  HitVec VStack_90;
  HitVec VStack_9c;
  HitVec VStack_a8;
  HitVec VStack_b4;
  HitVec VStack_c0;
  HitVec VStack_cc;
  HitVec VStack_d8;

  iVar3 = *(int *)(param_1 + 0xd8);
  if (iVar3 != 0) {
    iVar2 = *(int *)(iVar3 + 0xd8);
    if (iVar2 != 0) {
      iVar1 = *(int *)(iVar2 + 0xd8);
      if (iVar1 != 0) {
        if (*(int *)(iVar1 + 0xd8) != 0) {
          hitObjGetPosSub(*(int *)(iVar1 + 0xd8), param_2, param_3, 1);
        }
        else {
          PSMTXMultVec((void*)(iVar1 + 0xc), (void*)(iVar1 + 0x9c), &VStack_84);
          PSVECAdd((void*)param_2, &VStack_84, (void*)param_2);
          *param_3 = *param_3 + 1;
        }
        if (*(int *)(iVar1 + 0xdc) != 0) {
          hitObjGetPosSub(*(int *)(iVar1 + 0xdc), param_2, param_3, 1);
        }
      }
      else {
        PSMTXMultVec((void*)(iVar2 + 0xc), (void*)(iVar2 + 0x9c), &VStack_54);
        PSVECAdd((void*)param_2, &VStack_54, (void*)param_2);
        *param_3 = *param_3 + 1;
      }
      iVar2 = *(int *)(iVar2 + 0xdc);
      if (iVar2 != 0) {
        if (*(int *)(iVar2 + 0xd8) != 0) {
          hitObjGetPosSub(*(int *)(iVar2 + 0xd8), param_2, param_3, 1);
        }
        else {
          PSMTXMultVec((void*)(iVar2 + 0xc), (void*)(iVar2 + 0x9c), &VStack_90);
          PSVECAdd((void*)param_2, &VStack_90, (void*)param_2);
          *param_3 = *param_3 + 1;
        }
        if (*(int *)(iVar2 + 0xdc) != 0) {
          hitObjGetPosSub(*(int *)(iVar2 + 0xdc), param_2, param_3, 1);
        }
      }
    }
    else {
      PSMTXMultVec((void*)(iVar3 + 0xc), (void*)(iVar3 + 0x9c), &VStack_3c);
      PSVECAdd((void*)param_2, &VStack_3c, (void*)param_2);
      *param_3 = *param_3 + 1;
    }
    iVar3 = *(int *)(iVar3 + 0xdc);
    if (iVar3 != 0) {
      iVar2 = *(int *)(iVar3 + 0xd8);
      if (iVar2 == 0) {
        PSMTXMultVec((void*)(iVar3 + 0xc), (void*)(iVar3 + 0x9c), &VStack_60);
        PSVECAdd((void*)param_2, &VStack_60, (void*)param_2);
        *param_3 = *param_3 + 1;
      }
      else {
        if (*(int *)(iVar2 + 0xd8) != 0) {
          hitObjGetPosSub(*(int *)(iVar2 + 0xd8), param_2, param_3, 1);
        }
        else {
          PSMTXMultVec((void*)(iVar2 + 0xc), (void*)(iVar2 + 0x9c), &VStack_9c);
          PSVECAdd((void*)param_2, &VStack_9c, (void*)param_2);
          *param_3 = *param_3 + 1;
        }
        if (*(int *)(iVar2 + 0xdc) != 0) {
          hitObjGetPosSub(*(int *)(iVar2 + 0xdc), param_2, param_3, 1);
        }
      }
      iVar3 = *(int *)(iVar3 + 0xdc);
      if (iVar3 != 0) {
        if (*(int *)(iVar3 + 0xd8) != 0) {
          hitObjGetPosSub(*(int *)(iVar3 + 0xd8), param_2, param_3, 1);
        }
        else {
          PSMTXMultVec((void*)(iVar3 + 0xc), (void*)(iVar3 + 0x9c), &VStack_a8);
          PSVECAdd((void*)param_2, &VStack_a8, (void*)param_2);
          *param_3 = *param_3 + 1;
        }
        if (*(int *)(iVar3 + 0xdc) != 0) {
          hitObjGetPosSub(*(int *)(iVar3 + 0xdc), param_2, param_3, 1);
        }
      }
    }
  }
  else {
    PSMTXMultVec((void*)(param_1 + 0xc), (void*)(param_1 + 0x9c), &VStack_30);
    PSVECAdd((void*)param_2, &VStack_30, (void*)param_2);
    *param_3 = *param_3 + 1;
  }
  if ((param_4 != 0) && (iVar3 = *(int *)(param_1 + 0xdc), iVar3 != 0)) {
    iVar2 = *(int *)(iVar3 + 0xd8);
    if (iVar2 != 0) {
      iVar1 = *(int *)(iVar2 + 0xd8);
      if (iVar1 != 0) {
        if (*(int *)(iVar1 + 0xd8) != 0) {
          hitObjGetPosSub(*(int *)(iVar1 + 0xd8), param_2, param_3, 1);
        }
        else {
          PSMTXMultVec((void*)(iVar1 + 0xc), (void*)(iVar1 + 0x9c), &VStack_b4);
          PSVECAdd((void*)param_2, &VStack_b4, (void*)param_2);
          *param_3 = *param_3 + 1;
        }
        if (*(int *)(iVar1 + 0xdc) != 0) {
          hitObjGetPosSub(*(int *)(iVar1 + 0xdc), param_2, param_3, 1);
        }
      }
      else {
        PSMTXMultVec((void*)(iVar2 + 0xc), (void*)(iVar2 + 0x9c), &VStack_6c);
        PSVECAdd((void*)param_2, &VStack_6c, (void*)param_2);
        *param_3 = *param_3 + 1;
      }
      iVar2 = *(int *)(iVar2 + 0xdc);
      if (iVar2 != 0) {
        if (*(int *)(iVar2 + 0xd8) != 0) {
          hitObjGetPosSub(*(int *)(iVar2 + 0xd8), param_2, param_3, 1);
        }
        else {
          PSMTXMultVec((void*)(iVar2 + 0xc), (void*)(iVar2 + 0x9c), &VStack_c0);
          PSVECAdd((void*)param_2, &VStack_c0, (void*)param_2);
          *param_3 = *param_3 + 1;
        }
        if (*(int *)(iVar2 + 0xdc) != 0) {
          hitObjGetPosSub(*(int *)(iVar2 + 0xdc), param_2, param_3, 1);
        }
      }
    }
    else {
      PSMTXMultVec((void*)(iVar3 + 0xc), (void*)(iVar3 + 0x9c), &VStack_48);
      PSVECAdd((void*)param_2, &VStack_48, (void*)param_2);
      *param_3 = *param_3 + 1;
    }
    iVar3 = *(int *)(iVar3 + 0xdc);
    if (iVar3 != 0) {
      iVar2 = *(int *)(iVar3 + 0xd8);
      if (iVar2 != 0) {
        if (*(int *)(iVar2 + 0xd8) != 0) {
          hitObjGetPosSub(*(int *)(iVar2 + 0xd8), param_2, param_3, 1);
        }
        else {
          PSMTXMultVec((void*)(iVar2 + 0xc), (void*)(iVar2 + 0x9c), &VStack_cc);
          PSVECAdd((void*)param_2, &VStack_cc, (void*)param_2);
          *param_3 = *param_3 + 1;
        }
        if (*(int *)(iVar2 + 0xdc) != 0) {
          hitObjGetPosSub(*(int *)(iVar2 + 0xdc), param_2, param_3, 1);
        }
      }
      else {
        PSMTXMultVec((void*)(iVar3 + 0xc), (void*)(iVar3 + 0x9c), &VStack_78);
        PSVECAdd((void*)param_2, &VStack_78, (void*)param_2);
        *param_3 = *param_3 + 1;
      }
      iVar3 = *(int *)(iVar3 + 0xdc);
      if (iVar3 != 0) {
        if (*(int *)(iVar3 + 0xd8) != 0) {
          hitObjGetPosSub(*(int *)(iVar3 + 0xd8), param_2, param_3, 1);
        }
        else {
          PSMTXMultVec((void*)(iVar3 + 0xc), (void*)(iVar3 + 0x9c), &VStack_d8);
          PSVECAdd((void*)param_2, &VStack_d8, (void*)param_2);
          *param_3 = *param_3 + 1;
        }
        if (*(int *)(iVar3 + 0xdc) != 0) {
          hitObjGetPosSub(*(int *)(iVar3 + 0xdc), param_2, param_3, 1);
        }
      }
    }
  }
}

void hitObjGetPos(char* name, f32* out) {
    typedef struct HitMapEntryInline {
        s32 count;
        u8 unk4[0x154];
        s32 hitNumJoints;
        HitEntry* hitEntries;
        u8 unk160[0x18];
    } HitMapEntryInline;
    extern const f32 vec3_802bf7f4[3];
    extern const f32 vec3_802bf800[3];
    Vec pos;
    s32 count;
    HitEntry* hit;
    int i;
    int j;
    HitMapEntryInline* wp;
    HitMapEntryInline* map;
    HitMapEntryInline* new_var;
    HitMapEntryInline* new_var2;

    count = 0;
    pos = *(Vec*)vec3_802bf7f4;
    wp = mapGetWork();
    if (name == NULL) {
        hit = NULL;
    } else {
        map = wp;
        new_var = map;
        new_var2 = new_var;
        for (i = 0; i < new_var2->count; map++, i++) {
            hit = map->hitEntries;
            for (j = 0; j < map->hitNumJoints; j++, hit++) {
                if (!(hit->flags & 0x80) && !strcmp(*(char**)hit->joint, name)) {
                    goto found;
                }
            }
        }
        hit = NULL;
    }

found:
    if (hit == NULL) {
        *(Vec*)out = *(Vec*)vec3_802bf800;
    } else {
        hitObjGetPosSub((u32)hit, (u32)&pos, &count, 0);
        PSVECScale(&pos, out, float_1_8041f844 / (f32)count);
    }
}

void hitObjGetNormal(char* name, void* out) {
    extern void* mapGetWork(void);
    extern s32 strcmp(const char* str1, const char* str2);
    extern void PSVECNormalize(void* src, void* dst);

    s32 groupIndex;
    void* hit;
    s32 hitIndex;
    void* map;
    void* group;

    map = mapGetWork();
    if (name == NULL) {
        hit = NULL;
    } else {
        group = map;
        groupIndex = 0;
        while (groupIndex < *(s32*)map) {
            hit = *(void**)((s32)group + 0x15C);
            hitIndex = 0;
            while (hitIndex < *(s32*)((s32)group + 0x158)) {
                if ((*(u16*)hit & 0x80) != 0) {
                    goto next_hit;
                }
                if (strcmp(**(char***)((s32)hit + 0x8), name) != 0) {
                    goto next_hit;
                }
                goto found;

next_hit:
                hitIndex++;
                hit = (void*)((s32)hit + 0xE4);
            }
            group = (void*)((s32)group + 0x178);
            groupIndex++;
        }
        hit = NULL;
    }

found:
    if (hit != NULL) {
        if (*(s32*)(*(s32*)((s32)hit + 0x8) + 0x5C) > 0) {
            PSVECNormalize((void*)(*(s32*)((s32)hit + 0xAC) + 0x48), out);
        }
    }
}

s32 hitGetName(void* pHit) {
    void* map;
    s32 groupCount;
    s32 groupIndex;

    if (pHit == NULL) {
        return 0;
    }

    map = mapGetWork();
    groupCount = *(s32*)map;
    groupIndex = 0;
    if (groupCount > 0) {
        do {
            void* hitBase = *(void**)((s32)map + 0x15C);
            u32 size = (*(s32*)((s32)map + 0x158) + 0x80) * 0xE4;
            size = (size + 0x1F) & ~0x1F;
            if ((u32)pHit >= (u32)hitBase && (u32)pHit < (u32)hitBase + size) {
                break;
            }
            map = (void*)((s32)map + 0x178);
            groupIndex++;
        } while (groupIndex < groupCount);
    }

    if (groupIndex >= groupCount) {
        return 0;
    }
    return **(s32**)((s32)pHit + 0x8);
}

u32 hitGetAttr(void* hit) {
    if (hit != NULL) {
        return *(u32*)((s32)hit + 0x4);
    }

    return 0;
}
void hitDamageReturnSet(HitEntry* param_1, void* param_2, BOOL param_3) {
  HitEntry* iVar3;
  HitEntry* iVar2;
  HitEntry* iVar1;

  param_1->attributes = param_1->attributes | 0x40000000;
  param_1->damage = param_2;
  iVar3 = param_1->child;
  if (iVar3 != 0) {
    iVar3->attributes = iVar3->attributes | 0x40000000;
    iVar3->damage = param_2;
    iVar2 = iVar3->child;
    if (iVar2 != 0) {
      iVar2->attributes = iVar2->attributes | 0x40000000;
      iVar2->damage = param_2;
      iVar1 = iVar2->child;
      if (iVar1 != 0) {
        iVar1->attributes = iVar1->attributes | 0x40000000;
        iVar1->damage = param_2;
        if (iVar1->child != 0) {
          hitDamageReturnSet(iVar1->child, param_2, 1);
        }
        if (iVar1->sibling != 0) {
          hitDamageReturnSet(iVar1->sibling, param_2, 1);
        }
      }
      iVar2 = iVar2->sibling;
      if (iVar2 != 0) {
        iVar2->attributes = iVar2->attributes | 0x40000000;
        iVar2->damage = param_2;
        if (iVar2->child != 0) {
          hitDamageReturnSet(iVar2->child, param_2, 1);
        }
        if (iVar2->sibling != 0) {
          hitDamageReturnSet(iVar2->sibling, param_2, 1);
        }
      }
    }
    iVar3 = iVar3->sibling;
    if (iVar3 != 0) {
      iVar3->attributes = iVar3->attributes | 0x40000000;
      iVar3->damage = param_2;
      iVar2 = iVar3->child;
      if (iVar2 != 0) {
        iVar2->attributes = iVar2->attributes | 0x40000000;
        iVar2->damage = param_2;
        if (iVar2->child != 0) {
          hitDamageReturnSet(iVar2->child, param_2, 1);
        }
        if (iVar2->sibling != 0) {
          hitDamageReturnSet(iVar2->sibling, param_2, 1);
        }
      }
      iVar3 = iVar3->sibling;
      if (iVar3 != 0) {
        iVar3->attributes = iVar3->attributes | 0x40000000;
        iVar3->damage = param_2;
        if (iVar3->child != 0) {
          hitDamageReturnSet(iVar3->child, param_2, 1);
        }
        if (iVar3->sibling != 0) {
          hitDamageReturnSet(iVar3->sibling, param_2, 1);
        }
      }
    }
  }
  if ((param_3 != 0) && (iVar3 = param_1->sibling, iVar3 != 0)) {
    iVar3->attributes = iVar3->attributes | 0x40000000;
    iVar3->damage = param_2;
    iVar2 = iVar3->child;
    if (iVar2 != 0) {
      iVar2->attributes = iVar2->attributes | 0x40000000;
      iVar2->damage = param_2;
      iVar1 = iVar2->child;
      if (iVar1 != 0) {
        iVar1->attributes = iVar1->attributes | 0x40000000;
        iVar1->damage = param_2;
        if (iVar1->child != 0) {
          hitDamageReturnSet(iVar1->child, param_2, 1);
        }
        if (iVar1->sibling != 0) {
          hitDamageReturnSet(iVar1->sibling, param_2, 1);
        }
      }
      iVar2 = iVar2->sibling;
      if (iVar2 != 0) {
        iVar2->attributes = iVar2->attributes | 0x40000000;
        iVar2->damage = param_2;
        if (iVar2->child != 0) {
          hitDamageReturnSet(iVar2->child, param_2, 1);
        }
        if (iVar2->sibling != 0) {
          hitDamageReturnSet(iVar2->sibling, param_2, 1);
        }
      }
    }
    iVar3 = iVar3->sibling;
    if (iVar3 != 0) {
      iVar3->attributes = iVar3->attributes | 0x40000000;
      iVar3->damage = param_2;
      iVar2 = iVar3->child;
      if (iVar2 != 0) {
        iVar2->attributes = iVar2->attributes | 0x40000000;
        iVar2->damage = param_2;
        if (iVar2->child != 0) {
          hitDamageReturnSet(iVar2->child, param_2, 1);
        }
        if (iVar2->sibling != 0) {
          hitDamageReturnSet(iVar2->sibling, param_2, 1);
        }
      }
      iVar3 = iVar3->sibling;
      if (iVar3 != 0) {
        iVar3->attributes = iVar3->attributes | 0x40000000;
        iVar3->damage = param_2;
        if (iVar3->child != 0) {
          hitDamageReturnSet(iVar3->child, param_2, 1);
        }
        if (iVar3->sibling != 0) {
          hitDamageReturnSet(iVar3->sibling, param_2, 1);
        }
      }
    }
  }

}

void hitGrpDamageReturnSet(char* name, s32 value) {
    s32 groupIndex;
    void* hit;
    s32 hitIndex;
    void* map;
    void* group;

    map = mapGetWork();
    if (name == NULL) {
        hit = NULL;
    } else {
        group = map;
        groupIndex = 0;
        while (groupIndex < *(s32*)map) {
            hit = *(void**)((s32)group + 0x15C);
            hitIndex = 0;
            while (hitIndex < *(s32*)((s32)group + 0x158)) {
                if ((*(u16*)hit & 0x80) != 0) {
                    goto next_hit;
                }
                if (strcmp(**(char***)((s32)hit + 0x8), name) != 0) {
                    goto next_hit;
                }
                goto found;

next_hit:
                hitIndex++;
                hit = (void*)((s32)hit + 0xE4);
            }
            group = (void*)((s32)group + 0x178);
            groupIndex++;
        }
        hit = NULL;
    }

found:
    if (hit != NULL) {
        hitDamageReturnSet((HitEntry*)hit, (void*)value, 0);
    }
}

void* hitGetDamageReturnPos(void* hit) {
    return (void*)(*(s32*)((s32)hit + 0xB0) + 0x4);
}

void hitBindMapObj(char* hitName, char* mapObjName) {
    extern void* mapGetWork(void);
    extern void* mapGetMapObj(char* name);
    extern void mapErrorEntry(s32 type, char* message);
    extern s32 strcmp(const char* str1, const char* str2);

    s32 groupIndex;
    void* hit;
    s32 hitIndex;
    void* map;
    void* group;
    void* mapObj;

    map = mapGetWork();
    if (hitName == NULL) {
        hit = NULL;
    } else {
        group = map;
        groupIndex = 0;
        while (groupIndex < *(s32*)map) {
            hit = *(void**)((s32)group + 0x15C);
            hitIndex = 0;
            while (hitIndex < *(s32*)((s32)group + 0x158)) {
                if ((*(u16*)hit & 0x80) != 0) {
                    goto next_hit;
                }
                if (strcmp(**(char***)((s32)hit + 0x8), hitName) != 0) {
                    goto next_hit;
                }
                goto found;

next_hit:
                hitIndex++;
                hit = (void*)((s32)hit + 0xE4);
            }
            group = (void*)((s32)group + 0x178);
            groupIndex++;
        }
        hit = NULL;
    }

found:
    mapObj = mapGetMapObj(mapObjName);
    if (mapObj == NULL) {
        mapErrorEntry(0, mapObjName);
    } else if (hit == NULL) {
        mapErrorEntry(1, hitName);
    } else {
        *(void**)((s32)hit + 0xD0) = mapObj;
        *(u16*)hit |= 0x20;
    }
}
void hitBindUpdate(char* name) {
    extern void* mapGetWork(void);
    extern s32 strcmp(const char* str1, const char* str2);
    extern void mapErrorEntry(s32 type, char* message);
    extern void PSMTXScale(void* mtx, f32 x, f32 y, f32 z);
    extern void hitReCalcMatrix(void* hit, void* arg);
    extern const f32 float_10_8041f840;

    s32 groupIndex;
    void* hit;
    s32 hitIndex;
    void* map;
    void* group;
    f32 scale[3][4];

    map = mapGetWork();
    if (name == NULL) {
        hit = NULL;
    } else {
        group = map;
        groupIndex = 0;
        while (groupIndex < *(s32*)map) {
            hit = *(void**)((s32)group + 0x15C);
            hitIndex = 0;
            while (hitIndex < *(s32*)((s32)group + 0x158)) {
                if ((*(u16*)hit & 0x80) != 0) {
                    goto next_hit;
                }
                if (strcmp(**(char***)((s32)hit + 0x8), name) != 0) {
                    goto next_hit;
                }
                goto found;

next_hit:
                hitIndex++;
                hit = (void*)((s32)hit + 0xE4);
            }
            group = (void*)((s32)group + 0x178);
            groupIndex++;
        }
        hit = NULL;
    }

found:
    if (hit == NULL) {
        mapErrorEntry(1, name);
        return;
    }

    if ((*(u16*)hit & 0x20) != 0) {
        void* mapObj = *(void**)((s32)hit + 0xD4);
        if (mapObj != NULL) {
            hitReCalcMatrix(hit, (void*)((s32)mapObj + 0xC));
        } else {
            PSMTXScale(scale, float_10_8041f840, float_10_8041f840, float_10_8041f840);
            hitReCalcMatrix(hit, scale);
        }
    }
}

const f32 vec3_802bf7e8[3] = { 0.0f, 0.0f, 0.0f };
const f32 vec3_802bf7f4[3] = { 0.0f, 0.0f, 0.0f };
const f32 vec3_802bf800[3] = { 0.0f, 0.0f, 0.0f };

void* unk_8041e628;

const f32 float_0_8041f838 = 0.0f;
const f32 float_0p1_8041f83c = 0.1f;
const f32 float_10_8041f840 = 10.0f;
const f32 float_1_8041f844 = 1.0f;
const f32 float_neg1_8041f848 = -1.0f;
const f32 float_0p5_8041f84c = 0.5f;
const f32 float_deg2rad_8041f850 = 0.017453292f;
const u32 gap_09_8041F854_sdata2 = 0;
