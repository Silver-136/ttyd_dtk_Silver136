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

    void* map;
    void* group;
    void* hit;
    void* part;
    void* poly;
    void* alloc;
    f32 transMtx[3][4];
    f32 scaleMtx[3][4];
    f32 rotXMtx[3][4];
    f32 rotYMtx[3][4];
    f32 rotZMtx[3][4];
    f32 tempVec[3];
    f32 pad;
    s32 groupOffset;
    s32 hitCount;
    s32 partCount;
    s32 triCount;
    s32 partIndex;
    s32 polyIndex;
    s32 polyCount;
    s32 unroll;
    s32 remaining;

    map = mapGetWork();
    groupOffset = param_4 * 0x178;
    group = (void*)((s32)map + groupOffset);
    hitCount = *(s32*)((s32)group + 0x158);
    hit = *(void**)((s32)group + 0x15C);

    if (hitCount > 0) {
        do {
            if (*(void**)((s32)hit + 0x08) == 0) {
                break;
            }
            hit = (void*)((s32)hit + 0xE4);
            hitCount--;
        } while (hitCount != 0);
    }

    *(u16*)hit = 0;
    *(void**)((s32)hit + 0x08) = param_1;
    *(s16*)((s32)hit + 0xAA) = (s16)param_4;

    PSMTXTrans(
        transMtx,
        *(f32*)((s32)param_1 + 0x30),
        *(f32*)((s32)param_1 + 0x34),
        *(f32*)((s32)param_1 + 0x38)
    );
    PSMTXScale(
        scaleMtx,
        *(f32*)((s32)param_1 + 0x18),
        *(f32*)((s32)param_1 + 0x1C),
        *(f32*)((s32)param_1 + 0x20)
    );
    PSMTXRotRad(rotXMtx, 'x', float_deg2rad_8041f850 * *(f32*)((s32)param_1 + 0x24));
    PSMTXRotRad(rotYMtx, 'y', float_deg2rad_8041f850 * *(f32*)((s32)param_1 + 0x28));
    PSMTXRotRad(rotZMtx, 'z', float_deg2rad_8041f850 * *(f32*)((s32)param_1 + 0x2C));

    PSMTXConcat(transMtx, rotZMtx, transMtx);
    PSMTXConcat(transMtx, rotYMtx, transMtx);
    PSMTXConcat(transMtx, rotXMtx, transMtx);
    PSMTXConcat(transMtx, scaleMtx, (void*)((s32)hit + 0x3C));
    PSMTXConcat((void*)param_3, (void*)((s32)hit + 0x3C), (void*)((s32)hit + 0x0C));

    *(s32*)((s32)hit + 0xD4) = param_2;
    *(s32*)((s32)hit + 0xDC) = 0;
    *(s32*)((s32)hit + 0xD8) = 0;
    *(s32*)((s32)hit + 0xAC) = 0;

    if (*(void**)((s32)param_1 + 0x58) != 0) {
        *(u32*)((s32)hit + 0x04) = *(u32*)(*(s32*)((s32)param_1 + 0x58) + 0x08);
    } else {
        *(u32*)((s32)hit + 0x04) = 0;
    }

    if (*(s32*)((s32)param_1 + 0x5C) != 0) {
        PSVECAdd((void*)((s32)param_1 + 0x3C), (void*)((s32)param_1 + 0x48), (void*)((s32)hit + 0x9C));
        PSVECScale((void*)((s32)hit + 0x9C), (void*)((s32)hit + 0x9C), float_0p5_8041f84c);
    } else {
        *(u32*)((s32)hit + 0x9C) = *(u32*)((s32)param_1 + 0x30);
        *(u32*)((s32)hit + 0xA0) = *(u32*)((s32)param_1 + 0x34);
        *(u32*)((s32)hit + 0xA4) = *(u32*)((s32)param_1 + 0x38);
    }

    PSMTXMultVec((void*)((s32)hit + 0x0C), (void*)((s32)hit + 0x9C), (void*)((s32)hit + 0xC0));

    *(f32*)((s32)param_1 + 0x3C) -= float_0p1_8041f83c;
    *(f32*)((s32)param_1 + 0x40) -= float_0p1_8041f83c;
    *(f32*)((s32)param_1 + 0x44) -= float_0p1_8041f83c;
    *(f32*)((s32)param_1 + 0x48) += float_0p1_8041f83c;
    *(f32*)((s32)param_1 + 0x4C) += float_0p1_8041f83c;
    *(f32*)((s32)param_1 + 0x50) += float_0p1_8041f83c;

    PSMTXMultVec((void*)((s32)hit + 0x0C), (void*)((s32)param_1 + 0x48), tempVec);
    *(f32*)((s32)hit + 0xCC) = PSVECDistance(tempVec, (void*)((s32)hit + 0xC0));

    triCount = 0;
    partIndex = 0;
    partCount = *(s32*)((s32)param_1 + 0x5C);
    while (partIndex < partCount) {
        part = *(void**)((s32)param_1 + 0x64 + partIndex * 8);
        if (part != 0) {
            polyCount = *(s32*)((s32)part + 0x04);
            polyIndex = 0;
            if (*(u8*)((s32)part + 0x03) == 0) {
                while (polyIndex < polyCount) {
                    poly = *(void**)((s32)part + 0x10 + polyIndex * 4);
                    triCount += *(s32*)poly - 2;
                    polyIndex++;
                }
            } else {
                while (polyIndex < polyCount) {
                    poly = *(void**)((s32)part + 0x10 + polyIndex * 8);
                    triCount += *(u16*)((s32)poly + 1) - 2;
                    polyIndex++;
                }
            }
        }
        partIndex++;
    }

    if (triCount > 0) {
        alloc = _mapAlloc(mapalloc_base_ptr, triCount * 0x54);
        *(void**)((s32)hit + 0xAC) = alloc;
        *(s16*)((s32)hit + 0xA8) = (s16)hitCalcVtxPosition(hit);
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
    extern void* mapGetWork(void);
    extern s32 mapGetJoints(void* mapObj);
    extern void* _mapAlloc(void* heap, u32 size);
    extern void* mapalloc_base_ptr;
    extern void memset(void* dst, s32 value, u32 size);

    void* map;
    void* group;
    s32 joints;
    u32 size;

    map = mapGetWork();
    joints = mapGetJoints(mapObj);
    group = (void*)((s32)map + idx * 0x178);
    *(s32*)((s32)group + 0x158) = joints;
    size = (u32)((joints + 0x80) * 0xE4 + 0x1F) & ~0x1F;
    *(void**)((s32)group + 0x15C) = _mapAlloc(mapalloc_base_ptr, size);
    memset(*(void**)((s32)group + 0x15C), 0, size);
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
    void* group;
    void* hit;
    void* last = NULL;
    s32 groupIndex;
    s32 hitIndex;
    f32 scale[3][4];

    unk_8041e628 = NULL;
    group = map;
    for (groupIndex = 0; groupIndex < *(s32*)map; groupIndex++, group = (void*)((s32)group + 0x178)) {
        hit = *(void**)((s32)group + 0x15C);
        for (hitIndex = 0; hitIndex < *(s32*)((s32)group + 0x158); hitIndex++, hit = (void*)((s32)hit + 0xE4)) {
            if (*(s32*)(*(s32*)((s32)hit + 8) + 0x5C) != 0 && (*(u16*)hit & 0x81) == 0) {
                if (last == NULL) unk_8041e628 = hit;
                else *(void**)((s32)last + 0xE0) = hit;
                last = hit;
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
            void* tracks = *(void**)(*(s32*)((s32)entry + 0x18) + 0x18);
            if ((flags & 1) != 0 && ((flags & 2) == 0 || (flags & 0x1000) == 0) && tracks != NULL) {
                s32 trackIndex;
                void* trackNode = tracks;
                for (trackIndex = 0; trackIndex < *(s32*)tracks; trackIndex++, trackNode = (void*)((s32)trackNode + 4)) {
                    void* track = *(void**)((s32)trackNode + 4);
                    char* name = *(char**)((s32)track + 4);
                    void* searchGroup = map;
                    s32 searchGroupIndex;
                    hit = NULL;
                    for (searchGroupIndex = 0; name != NULL && searchGroupIndex < *(s32*)map; searchGroupIndex++, searchGroup = (void*)((s32)searchGroup + 0x178)) {
                        void* candidate = *(void**)((s32)searchGroup + 0x15C);
                        for (hitIndex = 0; hitIndex < *(s32*)((s32)searchGroup + 0x158); hitIndex++, candidate = (void*)((s32)candidate + 0xE4)) {
                            if ((*(u16*)candidate & 0x80) == 0 && strcmp(**(char***)((s32)candidate + 8), name) == 0) {
                                hit = candidate;
                                break;
                            }
                        }
                        if (hit != NULL) break;
                    }
                    if (hit != NULL) {
                        mapCalcAnimMatrix((void*)((s32)hit + 0x6C), (void*)((s32)hit + 0x3C), entry, track);
                        *(u16*)hit |= 0x50;
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
    extern void* mapGetWork(void);
    extern void PSMTXMultVec(void*, void*, void*);
    extern void PSVECSubtract(void*, void*, void*);
    extern void PSVECCrossProduct(void*, void*, void*);
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

    if (*(s32*)((s32)hit + 0xAC) == 0) {
        return 0;
    }
    jointOffset = 0;
    for (jointIndex = 0; jointIndex < *(s32*)(*(s32*)((s32)hit + 8) + 0x5C); jointIndex++) {
        joint = *(void**)(*(s32*)((s32)hit + 8) + jointOffset + 0x64);
        if (joint != 0) {
            if (*(u8*)((s32)joint + 3) == 0) {
                s32 index0;
                s32 index1;
                s32 index2;
                s32 temp;
                s32* polyIter;
                shape = joint;
                for (shapeIndex = 0; shapeIndex < *(s32*)((s32)joint + 4); shapeIndex++) {
                    poly = *(s32**)((s32)shape + 0x10);
                    polyIter = poly;
                    triOffset = count * 0x54;
                    for (triIndex = 0; triIndex < *poly - 2; triIndex++) {
                        tri = (void*)(*(s32*)((s32)hit + 0xAC) + triOffset);
                        index1 = *(s16*)((s32)polyIter + 0x1C);
                        index2 = *(s16*)((s32)polyIter + 0x34);
                        count++;
                        triOffset += 0x54;
                        PSMTXMultVec((void*)((s32)hit + 0xC),
                                     (void*)(**(s32**)((s32)joint + 0xC) + 4 +
                                             *(s16*)((s32)polyIter + 4) * 0xC), tri);
                        temp = index1;
                        if ((triIndex & 1) == 0) temp = index2;
                        PSMTXMultVec((void*)((s32)hit + 0xC),
                                     (void*)(**(s32**)((s32)joint + 0xC) + 4 + temp * 0xC),
                                     (void*)((s32)tri + 0xC));
                        temp = index2;
                        if ((triIndex & 1) == 0) temp = index1;
                        PSMTXMultVec((void*)((s32)hit + 0xC),
                                     (void*)(**(s32**)((s32)joint + 0xC) + 4 + temp * 0xC),
                                     (void*)((s32)tri + 0x18));
                        PSVECSubtract((void*)((s32)tri + 0x18), tri, (void*)((s32)tri + 0x24));
                        PSVECSubtract(tri, (void*)((s32)tri + 0xC), (void*)((s32)tri + 0x30));
                        PSVECSubtract((void*)((s32)tri + 0xC), (void*)((s32)tri + 0x18), (void*)((s32)tri + 0x3C));
                        PSVECCrossProduct((void*)((s32)tri + 0x24), (void*)((s32)tri + 0x30), (void*)((s32)tri + 0x48));
                        polyIter += 6;
                    }
                    shape = (void*)((s32)shape + 4);
                }
            } else {
                s32 vertexCount;
                s32 index0;
                s32 index1;
                s32 index2;
                s32 temp;
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
                        tri = (void*)(*(s32*)((s32)hit + 0xAC) + triOffset);
                        temp = index1;
                        if ((triIndex & 1) == 0) temp = index2;
                        index1 = temp;
                        temp = index2;
                        if ((triIndex & 1) == 0) temp = *(u16*)((s32)poly + stride * (triIndex + 1) + 3);
                        index2 = temp;
                        qreg = *(u32*)((s32)*(void**)((s32)map + *(s16*)((s32)hit + 0xAA) * 0x178 + 0xA4) + 0x44);
                        qreg = (qreg << 8) | 7;
                        qreg |= qreg << 16;
                        asm { mtspr GQR5, qreg }
                        qbase = (u8*)vertexBase;
                        qoffset = index0 * 6;
                        asm { psq_lx qvalue, qbase, qoffset, 1, 5 }
                        vertex0[0] = qvalue;
                        qoffset += 2;
                        asm { psq_lx qvalue, qbase, qoffset, 1, 5 }
                        vertex0[1] = qvalue;
                        qoffset += 2;
                        asm { psq_lx qvalue, qbase, qoffset, 1, 5 }
                        vertex0[2] = qvalue;
                        qoffset = index1 * 6;
                        asm { psq_lx qvalue, qbase, qoffset, 1, 5 }
                        vertex1[0] = qvalue;
                        qoffset += 2;
                        asm { psq_lx qvalue, qbase, qoffset, 1, 5 }
                        vertex1[1] = qvalue;
                        qoffset += 2;
                        asm { psq_lx qvalue, qbase, qoffset, 1, 5 }
                        vertex1[2] = qvalue;
                        qoffset = index2 * 6;
                        asm { psq_lx qvalue, qbase, qoffset, 1, 5 }
                        vertex2[0] = qvalue;
                        qoffset += 2;
                        asm { psq_lx qvalue, qbase, qoffset, 1, 5 }
                        vertex2[1] = qvalue;
                        qoffset += 2;
                        asm { psq_lx qvalue, qbase, qoffset, 1, 5 }
                        vertex2[2] = qvalue;
                        PSMTXMultVec((void*)((s32)hit + 0xC), vertex0, tri);
                        PSMTXMultVec((void*)((s32)hit + 0xC), vertex1, (void*)((s32)tri + 0xC));
                        PSMTXMultVec((void*)((s32)hit + 0xC), vertex2, (void*)((s32)tri + 0x18));
                        PSVECSubtract((void*)((s32)tri + 0x18), tri, (void*)((s32)tri + 0x24));
                        PSVECSubtract(tri, (void*)((s32)tri + 0xC), (void*)((s32)tri + 0x30));
                        PSVECSubtract((void*)((s32)tri + 0xC), (void*)((s32)tri + 0x18), (void*)((s32)tri + 0x3C));
                        PSVECCrossProduct((void*)((s32)tri + 0x24), (void*)((s32)tri + 0x30), (void*)((s32)tri + 0x48));
                        if (*(f32*)((s32)tri + 0x48) != 0.0f ||
                            *(f32*)((s32)tri + 0x4C) != 0.0f ||
                            *(f32*)((s32)tri + 0x50) != 0.0f) {
                            triOffset += 0x54;
                            baseOffset += 0x54;
                            count++;
                        }
                    }
                    shape = (void*)((s32)shape + 8);
                }
            }
        }
        jointOffset += 8;
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
    typedef struct VecLocal {
        f32 x;
        f32 y;
        f32 z;
    } VecLocal;
    typedef struct HitCheckQueryLocal {
        s32 singleSided;
        s32 user0;
        s32 user1;
        VecLocal targetPos;
        VecLocal targetDir;
        VecLocal hitPos;
        VecLocal hitNormal;
        f32 targetDistance;
    } HitCheckQueryLocal;

    extern const f32 vec3_802bf7b8[3];
    extern const f32 vec3_802bf7c4[3];

    HitCheckQueryLocal temp;
    HitCheckQueryLocal* query = &temp;
    VecLocal targetPos = *(VecLocal*)vec3_802bf7b8;
    VecLocal targetDir = *(VecLocal*)vec3_802bf7c4;
    void* entry;

    targetPos.x = x;
    targetPos.y = y;
    targetPos.z = z;
    targetDir.x = vx;
    targetDir.y = vy;
    targetDir.z = vz;
    query->targetPos = targetPos;
    query->targetDir = targetDir;
    query->targetDistance = *outDist;
    entry = (void*)hitCheckVecFilter((HitCheckQuery*)query, (HitFilterCallback)filter);
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
    extern void PSVECSubtract(void* a, void* b, void* out);
    extern f32 PSVECDotProduct(void* a, void* b);
    extern void PSVECScale(void* src, void* dst, f32 scale);
    extern const f32 float_0_8041f838;
    extern const f32 float_neg1_8041f848;

    f32 vec0[3];
    f32 vec1[3];
    f32 vec2[3];
    f32 dot;
    f32 side;
    f32 denom;
    f32 t;

    PSVECSubtract((void*)((s32)ray + 0x0C), tri, vec0);
    dot = PSVECDotProduct((void*)((s32)tri + 0x48), vec0);

    if (*(s32*)ray != 0) {
        if (dot < float_0_8041f838) {
            return 0;
        }

        side = vec0[1] * (*(f32*)((s32)tri + 0x24) * *(f32*)((s32)ray + 0x20) - *(f32*)((s32)tri + 0x2C) * *(f32*)((s32)ray + 0x18))
             + vec0[0] * (-*(f32*)((s32)tri + 0x28) * *(f32*)((s32)ray + 0x20))
             + vec0[2] * (*(f32*)((s32)tri + 0x28) * *(f32*)((s32)ray + 0x18));
        if (side < float_0_8041f838) {
            return 0;
        }

        PSVECSubtract((void*)((s32)ray + 0x0C), (void*)((s32)tri + 0x0C), vec1);
        side = vec1[1] * (*(f32*)((s32)tri + 0x30) * *(f32*)((s32)ray + 0x20) - *(f32*)((s32)tri + 0x38) * *(f32*)((s32)ray + 0x18))
             + vec1[0] * (-*(f32*)((s32)tri + 0x34) * *(f32*)((s32)ray + 0x20))
             + vec1[2] * (*(f32*)((s32)tri + 0x34) * *(f32*)((s32)ray + 0x18));
        if (side < float_0_8041f838) {
            return 0;
        }

        PSVECSubtract((void*)((s32)ray + 0x0C), (void*)((s32)tri + 0x18), vec2);
        side = vec2[1] * (*(f32*)((s32)tri + 0x3C) * *(f32*)((s32)ray + 0x20) - *(f32*)((s32)tri + 0x44) * *(f32*)((s32)ray + 0x18))
             + vec2[0] * (-*(f32*)((s32)tri + 0x40) * *(f32*)((s32)ray + 0x20))
             + vec2[2] * (*(f32*)((s32)tri + 0x40) * *(f32*)((s32)ray + 0x18));
        if (side < float_0_8041f838) {
            return 0;
        }
    } else {
        denom = *(f32*)((s32)tri + 0x48) * *(f32*)((s32)ray + 0x18)
              + *(f32*)((s32)tri + 0x50) * *(f32*)((s32)ray + 0x20);
        if (dot * denom >= float_0_8041f838) {
            return 0;
        }

        side = dot * (vec0[1] * (*(f32*)((s32)tri + 0x24) * *(f32*)((s32)ray + 0x20) - *(f32*)((s32)tri + 0x2C) * *(f32*)((s32)ray + 0x18))
                    + vec0[0] * (-*(f32*)((s32)tri + 0x28) * *(f32*)((s32)ray + 0x20))
                    + vec0[2] * (*(f32*)((s32)tri + 0x28) * *(f32*)((s32)ray + 0x18)));
        if (side < float_0_8041f838) {
            return 0;
        }

        PSVECSubtract((void*)((s32)ray + 0x0C), (void*)((s32)tri + 0x0C), vec1);
        side = dot * (vec1[1] * (*(f32*)((s32)tri + 0x30) * *(f32*)((s32)ray + 0x20) - *(f32*)((s32)tri + 0x38) * *(f32*)((s32)ray + 0x18))
                    + vec1[0] * (-*(f32*)((s32)tri + 0x34) * *(f32*)((s32)ray + 0x20))
                    + vec1[2] * (*(f32*)((s32)tri + 0x34) * *(f32*)((s32)ray + 0x18)));
        if (side < float_0_8041f838) {
            return 0;
        }

        PSVECSubtract((void*)((s32)ray + 0x0C), (void*)((s32)tri + 0x18), vec2);
        side = dot * (vec2[1] * (*(f32*)((s32)tri + 0x3C) * *(f32*)((s32)ray + 0x20) - *(f32*)((s32)tri + 0x44) * *(f32*)((s32)ray + 0x18))
                    + vec2[0] * (-*(f32*)((s32)tri + 0x40) * *(f32*)((s32)ray + 0x20))
                    + vec2[2] * (*(f32*)((s32)tri + 0x40) * *(f32*)((s32)ray + 0x18)));
        if (side < float_0_8041f838) {
            return 0;
        }
    }

    denom = *(f32*)((s32)tri + 0x48) * *(f32*)((s32)ray + 0x18)
          + *(f32*)((s32)tri + 0x50) * *(f32*)((s32)ray + 0x20);
    t = -dot / denom;

    if (*(f32*)((s32)ray + 0x3C) < float_0_8041f838 || t < *(f32*)((s32)ray + 0x3C)) {
        *(f32*)((s32)ray + 0x3C) = t;
        *(f32*)((s32)ray + 0x24) = *(f32*)((s32)ray + 0x18) * t + *(f32*)((s32)ray + 0x0C);
        *(f32*)((s32)ray + 0x28) = *(f32*)((s32)ray + 0x10);
        *(f32*)((s32)ray + 0x2C) = *(f32*)((s32)ray + 0x20) * t + *(f32*)((s32)ray + 0x14);

        if (dot < float_0_8041f838) {
            *(s32*)((s32)ray + 0x30) = *(s32*)((s32)tri + 0x48);
            *(s32*)((s32)ray + 0x34) = *(s32*)((s32)tri + 0x4C);
            *(s32*)((s32)ray + 0x38) = *(s32*)((s32)tri + 0x50);
        } else {
            PSVECScale((void*)((s32)tri + 0x48), (void*)((s32)ray + 0x30), float_neg1_8041f848);
        }
        return 1;
    }

    return 0;
}

#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset


u8 hitCheckVecHitObjXZ(s32* ray, void* hitObj) {
    extern f32 PSVECDotProduct(void* a, void* b);
    extern u8 checkTriVec_xz(void* ray, void* tri);
    f32* fRay = (f32*)ray;
    u8* tri;
    s32 i;

    ray[0] = (*(u8*)(*(s32*)(*(s32*)((s32)hitObj + 8) + 0x58) + 1) == 1);
    tri = *(u8**)((s32)hitObj + 0xAC);
    for (i = 0; i < *(s16*)((s32)hitObj + 0xA8); i++, tri += 0x54) {
        if ((ray[0] == 0 || PSVECDotProduct(tri + 0x48, fRay + 6) < 0.0f) &&
            checkTriVec_xz(ray, tri) != 0) {
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
    typedef struct VecLocal {
        f32 x;
        f32 y;
        f32 z;
    } VecLocal;
    typedef struct HitCheckQueryLocal {
        s32 singleSided;
        s32 user0;
        s32 user1;
        VecLocal targetPos;
        VecLocal targetDir;
        VecLocal hitPos;
        VecLocal hitNormal;
        f32 targetDistance;
    } HitCheckQueryLocal;

    extern const f32 vec3_802bf7d0[3];
    extern const f32 vec3_802bf7dc[3];

    HitCheckQueryLocal temp;
    HitCheckQueryLocal* query = &temp;
    VecLocal targetPos = *(VecLocal*)vec3_802bf7d0;
    VecLocal targetDir = *(VecLocal*)vec3_802bf7dc;
    void* entry;

    targetPos.x = x;
    targetPos.y = y;
    targetPos.z = z;
    targetDir.x = vx;
    targetDir.y = vy;
    targetDir.z = vz;
    query->targetPos = targetPos;
    query->targetDir = targetDir;
    query->targetDistance = *outDist;
    query->user0 = flags;
    entry = (void*)hitCheckVecFilter((HitCheckQuery*)query, (HitFilterCallback)chkFilterAttr);
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

void* hitCheckSphereFilter(void* filter, f32 x, f32 y, f32 z, f32 radius) {
    extern void* mapGetWork(void);
    extern f32 PSVECDistance(void* a, void* b);
    extern void PSVECSubtract(void* a, void* b, void* out);
    extern void PSVECNormalize(void* src, void* dst);
    extern f32 PSVECDotProduct(void* a, void* b);
    extern void PSVECCrossProduct(void* a, void* b, void* out);
    extern void PSVECScale(void* src, void* dst, f32 scale);
    extern void PSVECAdd(void* a, void* b, void* out);
    extern const f32 float_neg1_8041f848;
    extern const f32 float_0_8041f838;
    extern const f32 float_0p5_8041f84c;

    typedef s32 (*HitSphereFilterFunc)(void* work, void* hit);

    void* map;
    void* group;
    void* hit;
    void* selected;
    void* tri;
    f32 pos[3];
    f32 scanPos[3];
    f32 work[16];
    f32 vec0[3];
    f32 vec1[3];
    f32 vec2[3];
    f32 cross0[3];
    f32 cross1[3];
    f32 cross2[3];
    f32 scaled[3];
    f32 hitPos[3];
    f32 hitNormal[3];
    f32 bestDist;
    f32 bestRadius;
    f32 dist;
    f32 hitRadius;
    f32 dot;
    f32 denom;
    f32 side;
    f32 t;
    f32 bestT;
    s32 bestGroup;
    s32 bestIndex;
    s32 groupIndex;
    s32 hitIndex;
    s32 triIndex;
    s32 triHit;
    s32 oneSided;

    map = mapGetWork();
    bestDist = float_neg1_8041f848;
    bestIndex = -1;
    bestGroup = 0;

    pos[0] = x;
    pos[1] = y;
    pos[2] = z;
    scanPos[0] = x;
    scanPos[1] = y;
    scanPos[2] = z;

    group = map;
    groupIndex = 0;
    while (groupIndex < *(s32*)map) {
        hit = *(void**)((s32)group + 0x15C);
        hitIndex = 0;
        while (hitIndex < *(s32*)((s32)group + 0x158)) {
            if ((*(u32*)((s32)hit + 0x04) & 0x80000000) != 0 &&
                *(s32*)(*(s32*)((s32)hit + 0x08) + 0x5C) != 0 &&
                (*(u16*)hit & 1) == 0 &&
                (*(u16*)hit & 0x80) == 0) {
                dist = PSVECDistance(scanPos, (void*)((s32)hit + 0xC0));
                hitRadius = *(f32*)((s32)hit + 0xCC);
                if (dist <= radius + hitRadius) {
                    if (filter == 0 || ((HitSphereFilterFunc)filter)(work, hit) != 0) {
                        if (bestDist < float_0_8041f838 || dist < bestDist) {
                            bestDist = dist;
                            bestRadius = hitRadius;
                            bestIndex = hitIndex;
                            bestGroup = groupIndex;
                        }
                    }
                }
            }
            hitIndex++;
            hit = (void*)((s32)hit + 0xE4);
        }
        group = (void*)((s32)group + 0x178);
        groupIndex++;
    }

    if (bestIndex < 0) {
        return 0;
    }

    group = (void*)((s32)map + bestGroup * 0x178);
    selected = (void*)(*(s32*)((s32)group + 0x15C) + bestIndex * 0xE4);

    if (bestDist < float_0p5_8041f84c * bestRadius) {
        return selected;
    }

    PSVECSubtract((void*)((s32)selected + 0xC0), scanPos, (void*)((s32)work + 0x18));
    PSVECNormalize((void*)((s32)work + 0x18), (void*)((s32)work + 0x18));

    work[3] = scanPos[0];
    work[4] = scanPos[1];
    work[5] = scanPos[2];
    work[15] = radius;

    oneSided = (*(u8*)(*(s32*)(*(s32*)((s32)selected + 0x08) + 0x58) + 1) == 1);
    *(s32*)work = oneSided;

    tri = *(void**)((s32)selected + 0xAC);
    bestT = float_neg1_8041f848;
    triHit = -1;
    triIndex = 0;
    while (triIndex < *(s16*)((s32)selected + 0xA8)) {
        if (oneSided != 0) {
            if (PSVECDotProduct((void*)((s32)tri + 0x48), (void*)((s32)work + 0x18)) >= float_0_8041f838) {
                triIndex++;
                tri = (void*)((s32)tri + 0x54);
                continue;
            }

            PSVECSubtract((void*)((s32)work + 0x0C), tri, vec0);
            dot = PSVECDotProduct((void*)((s32)tri + 0x48), vec0);
            if (dot < float_0_8041f838) {
                triIndex++;
                tri = (void*)((s32)tri + 0x54);
                continue;
            }

            PSVECCrossProduct((void*)((s32)work + 0x18), (void*)((s32)tri + 0x24), cross0);
            side = PSVECDotProduct(vec0, cross0);
            if (side < float_0_8041f838) {
                triIndex++;
                tri = (void*)((s32)tri + 0x54);
                continue;
            }

            PSVECSubtract((void*)((s32)work + 0x0C), (void*)((s32)tri + 0x0C), vec1);
            PSVECCrossProduct((void*)((s32)work + 0x18), (void*)((s32)tri + 0x30), cross1);
            side = PSVECDotProduct(vec1, cross1);
            if (side < float_0_8041f838) {
                triIndex++;
                tri = (void*)((s32)tri + 0x54);
                continue;
            }

            PSVECSubtract((void*)((s32)work + 0x0C), (void*)((s32)tri + 0x18), vec2);
            PSVECCrossProduct((void*)((s32)work + 0x18), (void*)((s32)tri + 0x3C), cross2);
            side = PSVECDotProduct(vec2, cross2);
            if (side < float_0_8041f838) {
                triIndex++;
                tri = (void*)((s32)tri + 0x54);
                continue;
            }

            denom = PSVECDotProduct((void*)((s32)tri + 0x48), (void*)((s32)work + 0x18));
        } else {
            PSVECSubtract((void*)((s32)work + 0x0C), tri, vec0);
            dot = PSVECDotProduct((void*)((s32)tri + 0x48), vec0);
            denom = PSVECDotProduct((void*)((s32)tri + 0x48), (void*)((s32)work + 0x18));
            if (denom * dot >= float_0_8041f838) {
                triIndex++;
                tri = (void*)((s32)tri + 0x54);
                continue;
            }

            PSVECCrossProduct((void*)((s32)work + 0x18), (void*)((s32)tri + 0x24), cross0);
            side = dot * PSVECDotProduct(vec0, cross0);
            if (side < float_0_8041f838) {
                triIndex++;
                tri = (void*)((s32)tri + 0x54);
                continue;
            }

            PSVECSubtract((void*)((s32)work + 0x0C), (void*)((s32)tri + 0x0C), vec1);
            PSVECCrossProduct((void*)((s32)work + 0x18), (void*)((s32)tri + 0x30), cross1);
            side = dot * PSVECDotProduct(vec1, cross1);
            if (side < float_0_8041f838) {
                triIndex++;
                tri = (void*)((s32)tri + 0x54);
                continue;
            }

            PSVECSubtract((void*)((s32)work + 0x0C), (void*)((s32)tri + 0x18), vec2);
            PSVECCrossProduct((void*)((s32)work + 0x18), (void*)((s32)tri + 0x3C), cross2);
            side = dot * PSVECDotProduct(vec2, cross2);
            if (side < float_0_8041f838) {
                triIndex++;
                tri = (void*)((s32)tri + 0x54);
                continue;
            }
        }

        t = -dot / denom;
        if (work[15] >= float_0_8041f838 && t >= work[15]) {
            triIndex++;
            tri = (void*)((s32)tri + 0x54);
            continue;
        }

        work[15] = t;
        PSVECScale((void*)((s32)work + 0x18), scaled, t);
        PSVECAdd((void*)((s32)work + 0x0C), scaled, (void*)((s32)work + 0x24));

        if (dot <= float_0_8041f838) {
            *(s32*)((s32)work + 0x30) = *(s32*)((s32)tri + 0x48);
            *(s32*)((s32)work + 0x34) = *(s32*)((s32)tri + 0x4C);
            *(s32*)((s32)work + 0x38) = *(s32*)((s32)tri + 0x50);
        } else {
            PSVECScale((void*)((s32)tri + 0x48), (void*)((s32)work + 0x30), float_neg1_8041f848);
        }

        if (bestT < float_0_8041f838 || work[15] < bestT) {
            bestT = work[15];
            triHit = 1;
        }

        triIndex++;
        tri = (void*)((s32)tri + 0x54);
    }

    if (bestT > radius) {
        return 0;
    }
    if (triHit < 0) {
        return 0;
    }

    return selected;
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
  u32 iVar1;
  u32 iVar2;
  u32 iVar3;
  HitVec VStack_d8;
  HitVec VStack_cc;
  HitVec VStack_c0;
  HitVec VStack_b4;
  HitVec VStack_a8;
  HitVec VStack_9c;
  HitVec VStack_90;
  HitVec VStack_84;
  HitVec VStack_78;
  HitVec VStack_6c;
  HitVec VStack_60;
  HitVec VStack_54;
  HitVec VStack_48;
  HitVec VStack_3c;
  HitVec VStack_30;

  iVar3 = *(int *)(param_1 + 0xd8);
  if (iVar3 != 0) {
    iVar2 = *(int *)(iVar3 + 0xd8);
    if (iVar2 == 0) {
      PSMTXMultVec((void*)(iVar3 + 0xc), (void*)(iVar3 + 0x9c), &VStack_3c);
      PSVECAdd((void*)param_2, &VStack_3c, (void*)param_2);
      *param_3 = *param_3 + 1;
    }
    else {
      iVar1 = *(int *)(iVar2 + 0xd8);
      if (iVar1 == 0) {
        PSMTXMultVec((void*)(iVar2 + 0xc), (void*)(iVar2 + 0x9c), &VStack_54);
        PSVECAdd((void*)param_2, &VStack_54, (void*)param_2);
        *param_3 = *param_3 + 1;
      }
      else {
        if (*(int *)(iVar1 + 0xd8) == 0) {
          PSMTXMultVec((void*)(iVar1 + 0xc), (void*)(iVar1 + 0x9c), &VStack_84);
          PSVECAdd((void*)param_2, &VStack_84, (void*)param_2);
          *param_3 = *param_3 + 1;
        }
        else {
          hitObjGetPosSub(*(int *)(iVar1 + 0xd8), param_2, param_3, 1);
        }
        if (*(int *)(iVar1 + 0xdc) != 0) {
          hitObjGetPosSub(*(int *)(iVar1 + 0xdc), param_2, param_3, 1);
        }
      }
      iVar2 = *(int *)(iVar2 + 0xdc);
      if (iVar2 != 0) {
        if (*(int *)(iVar2 + 0xd8) == 0) {
          PSMTXMultVec((void*)(iVar2 + 0xc), (void*)(iVar2 + 0x9c), &VStack_90);
          PSVECAdd((void*)param_2, &VStack_90, (void*)param_2);
          *param_3 = *param_3 + 1;
        }
        else {
          hitObjGetPosSub(*(int *)(iVar2 + 0xd8), param_2, param_3, 1);
        }
        if (*(int *)(iVar2 + 0xdc) != 0) {
          hitObjGetPosSub(*(int *)(iVar2 + 0xdc), param_2, param_3, 1);
        }
      }
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
        if (*(int *)(iVar2 + 0xd8) == 0) {
          PSMTXMultVec((void*)(iVar2 + 0xc), (void*)(iVar2 + 0x9c), &VStack_9c);
          PSVECAdd((void*)param_2, &VStack_9c, (void*)param_2);
          *param_3 = *param_3 + 1;
        }
        else {
          hitObjGetPosSub(*(int *)(iVar2 + 0xd8), param_2, param_3, 1);
        }
        if (*(int *)(iVar2 + 0xdc) != 0) {
          hitObjGetPosSub(*(int *)(iVar2 + 0xdc), param_2, param_3, 1);
        }
      }
      iVar3 = *(int *)(iVar3 + 0xdc);
      if (iVar3 != 0) {
        if (*(int *)(iVar3 + 0xd8) == 0) {
          PSMTXMultVec((void*)(iVar3 + 0xc), (void*)(iVar3 + 0x9c), &VStack_a8);
          PSVECAdd((void*)param_2, &VStack_a8, (void*)param_2);
          *param_3 = *param_3 + 1;
        }
        else {
          hitObjGetPosSub(*(int *)(iVar3 + 0xd8), param_2, param_3, 1);
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
    if (iVar2 == 0) {
      PSMTXMultVec((void*)(iVar3 + 0xc), (void*)(iVar3 + 0x9c), &VStack_48);
      PSVECAdd((void*)param_2, &VStack_48, (void*)param_2);
      *param_3 = *param_3 + 1;
    }
    else {
      iVar1 = *(int *)(iVar2 + 0xd8);
      if (iVar1 == 0) {
        PSMTXMultVec((void*)(iVar2 + 0xc), (void*)(iVar2 + 0x9c), &VStack_6c);
        PSVECAdd((void*)param_2, &VStack_6c, (void*)param_2);
        *param_3 = *param_3 + 1;
      }
      else {
        if (*(int *)(iVar1 + 0xd8) == 0) {
          PSMTXMultVec((void*)(iVar1 + 0xc), (void*)(iVar1 + 0x9c), &VStack_b4);
          PSVECAdd((void*)param_2, &VStack_b4, (void*)param_2);
          *param_3 = *param_3 + 1;
        }
        else {
          hitObjGetPosSub(*(int *)(iVar1 + 0xd8), param_2, param_3, 1);
        }
        if (*(int *)(iVar1 + 0xdc) != 0) {
          hitObjGetPosSub(*(int *)(iVar1 + 0xdc), param_2, param_3, 1);
        }
      }
      iVar2 = *(int *)(iVar2 + 0xdc);
      if (iVar2 != 0) {
        if (*(int *)(iVar2 + 0xd8) == 0) {
          PSMTXMultVec((void*)(iVar2 + 0xc), (void*)(iVar2 + 0x9c), &VStack_c0);
          PSVECAdd((void*)param_2, &VStack_c0, (void*)param_2);
          *param_3 = *param_3 + 1;
        }
        else {
          hitObjGetPosSub(*(int *)(iVar2 + 0xd8), param_2, param_3, 1);
        }
        if (*(int *)(iVar2 + 0xdc) != 0) {
          hitObjGetPosSub(*(int *)(iVar2 + 0xdc), param_2, param_3, 1);
        }
      }
    }
    iVar3 = *(int *)(iVar3 + 0xdc);
    if (iVar3 != 0) {
      iVar2 = *(int *)(iVar3 + 0xd8);
      if (iVar2 == 0) {
        PSMTXMultVec((void*)(iVar3 + 0xc), (void*)(iVar3 + 0x9c), &VStack_78);
        PSVECAdd((void*)param_2, &VStack_78, (void*)param_2);
        *param_3 = *param_3 + 1;
      }
      else {
        if (*(int *)(iVar2 + 0xd8) == 0) {
          PSMTXMultVec((void*)(iVar2 + 0xc), (void*)(iVar2 + 0x9c), &VStack_cc);
          PSVECAdd((void*)param_2, &VStack_cc, (void*)param_2);
          *param_3 = *param_3 + 1;
        }
        else {
          hitObjGetPosSub(*(int *)(iVar2 + 0xd8), param_2, param_3, 1);
        }
        if (*(int *)(iVar2 + 0xdc) != 0) {
          hitObjGetPosSub(*(int *)(iVar2 + 0xdc), param_2, param_3, 1);
        }
      }
      iVar3 = *(int *)(iVar3 + 0xdc);
      if (iVar3 != 0) {
        if (*(int *)(iVar3 + 0xd8) == 0) {
          PSMTXMultVec((void*)(iVar3 + 0xc), (void*)(iVar3 + 0x9c), &VStack_d8);
          PSVECAdd((void*)param_2, &VStack_d8, (void*)param_2);
          *param_3 = *param_3 + 1;
        }
        else {
          hitObjGetPosSub(*(int *)(iVar3 + 0xd8), param_2, param_3, 1);
        }
        if (*(int *)(iVar3 + 0xdc) != 0) {
          hitObjGetPosSub(*(int *)(iVar3 + 0xdc), param_2, param_3, 1);
        }
      }
    }
  }
}

void hitObjGetPos(char* name, f32* out) {
    extern void* mapGetWork(void);
    extern s32 strcmp(const char* str1, const char* str2);
    extern void hitObjGetPosSub(void* hit, void* pos, s32* count, s32 recursive);
    extern void PSVECScale(void* src, void* dst, f32 scale);
    extern const f32 float_1_8041f844;

    void* map;
    void* group;
    void* hit;
    s32 groupIndex;
    s32 hitIndex;
    s32 count;
    f32 pos[3];

    count = 0;
    pos[0] = 0.0f;
    pos[1] = 0.0f;
    pos[2] = 0.0f;

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
                if ((*(u16*)hit & 0x80) == 0 &&
                    strcmp(**(char***)((s32)hit + 0x8), name) == 0) {
                    goto found;
                }
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
        out[0] = 0.0f;
        out[1] = -1000.0f;
        out[2] = 0.0f;
    } else {
        hitObjGetPosSub(hit, pos, &count, 0);
        PSVECScale(pos, out, float_1_8041f844 / (f32)count);
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
u8 hitDamageReturnSet(int param_1, s32 param_2, int param_3) {
  int iVar1;
  int iVar2;
  int iVar3;

  *(u32 *)(param_1 + 4) = *(u32 *)(param_1 + 4) | 0x40000000;
  *(s32 *)(param_1 + 0xb0) = param_2;
  iVar3 = *(int *)(param_1 + 0xd8);
  if (iVar3 != 0) {
    *(u32 *)(iVar3 + 4) = *(u32 *)(iVar3 + 4) | 0x40000000;
    *(s32 *)(iVar3 + 0xb0) = param_2;
    iVar2 = *(int *)(iVar3 + 0xd8);
    if (iVar2 != 0) {
      *(u32 *)(iVar2 + 4) = *(u32 *)(iVar2 + 4) | 0x40000000;
      *(s32 *)(iVar2 + 0xb0) = param_2;
      iVar1 = *(int *)(iVar2 + 0xd8);
      if (iVar1 != 0) {
        *(u32 *)(iVar1 + 4) = *(u32 *)(iVar1 + 4) | 0x40000000;
        *(s32 *)(iVar1 + 0xb0) = param_2;
        if (*(int *)(iVar1 + 0xd8) != 0) {
          hitDamageReturnSet(*(int *)(iVar1 + 0xd8), param_2, 1);
        }
        if (*(int *)(iVar1 + 0xdc) != 0) {
          hitDamageReturnSet(*(int *)(iVar1 + 0xdc), param_2, 1);
        }
      }
      iVar2 = *(int *)(iVar2 + 0xdc);
      if (iVar2 != 0) {
        *(u32 *)(iVar2 + 4) = *(u32 *)(iVar2 + 4) | 0x40000000;
        *(s32 *)(iVar2 + 0xb0) = param_2;
        if (*(int *)(iVar2 + 0xd8) != 0) {
          hitDamageReturnSet(*(int *)(iVar2 + 0xd8), param_2, 1);
        }
        if (*(int *)(iVar2 + 0xdc) != 0) {
          hitDamageReturnSet(*(int *)(iVar2 + 0xdc), param_2, 1);
        }
      }
    }
    iVar3 = *(int *)(iVar3 + 0xdc);
    if (iVar3 != 0) {
      *(u32 *)(iVar3 + 4) = *(u32 *)(iVar3 + 4) | 0x40000000;
      *(s32 *)(iVar3 + 0xb0) = param_2;
      iVar2 = *(int *)(iVar3 + 0xd8);
      if (iVar2 != 0) {
        *(u32 *)(iVar2 + 4) = *(u32 *)(iVar2 + 4) | 0x40000000;
        *(s32 *)(iVar2 + 0xb0) = param_2;
        if (*(int *)(iVar2 + 0xd8) != 0) {
          hitDamageReturnSet(*(int *)(iVar2 + 0xd8), param_2, 1);
        }
        if (*(int *)(iVar2 + 0xdc) != 0) {
          hitDamageReturnSet(*(int *)(iVar2 + 0xdc), param_2, 1);
        }
      }
      iVar3 = *(int *)(iVar3 + 0xdc);
      if (iVar3 != 0) {
        *(u32 *)(iVar3 + 4) = *(u32 *)(iVar3 + 4) | 0x40000000;
        *(s32 *)(iVar3 + 0xb0) = param_2;
        if (*(int *)(iVar3 + 0xd8) != 0) {
          hitDamageReturnSet(*(int *)(iVar3 + 0xd8), param_2, 1);
        }
        if (*(int *)(iVar3 + 0xdc) != 0) {
          hitDamageReturnSet(*(int *)(iVar3 + 0xdc), param_2, 1);
        }
      }
    }
  }
  if ((param_3 != 0) && (iVar3 = *(int *)(param_1 + 0xdc), iVar3 != 0)) {
    *(u32 *)(iVar3 + 4) = *(u32 *)(iVar3 + 4) | 0x40000000;
    *(s32 *)(iVar3 + 0xb0) = param_2;
    iVar2 = *(int *)(iVar3 + 0xd8);
    if (iVar2 != 0) {
      *(u32 *)(iVar2 + 4) = *(u32 *)(iVar2 + 4) | 0x40000000;
      *(s32 *)(iVar2 + 0xb0) = param_2;
      iVar1 = *(int *)(iVar2 + 0xd8);
      if (iVar1 != 0) {
        *(u32 *)(iVar1 + 4) = *(u32 *)(iVar1 + 4) | 0x40000000;
        *(s32 *)(iVar1 + 0xb0) = param_2;
        if (*(int *)(iVar1 + 0xd8) != 0) {
          hitDamageReturnSet(*(int *)(iVar1 + 0xd8), param_2, 1);
        }
        if (*(int *)(iVar1 + 0xdc) != 0) {
          hitDamageReturnSet(*(int *)(iVar1 + 0xdc), param_2, 1);
        }
      }
      iVar2 = *(int *)(iVar2 + 0xdc);
      if (iVar2 != 0) {
        *(u32 *)(iVar2 + 4) = *(u32 *)(iVar2 + 4) | 0x40000000;
        *(s32 *)(iVar2 + 0xb0) = param_2;
        if (*(int *)(iVar2 + 0xd8) != 0) {
          hitDamageReturnSet(*(int *)(iVar2 + 0xd8), param_2, 1);
        }
        if (*(int *)(iVar2 + 0xdc) != 0) {
          hitDamageReturnSet(*(int *)(iVar2 + 0xdc), param_2, 1);
        }
      }
    }
    iVar3 = *(int *)(iVar3 + 0xdc);
    if (iVar3 != 0) {
      *(u32 *)(iVar3 + 4) = *(u32 *)(iVar3 + 4) | 0x40000000;
      *(s32 *)(iVar3 + 0xb0) = param_2;
      iVar2 = *(int *)(iVar3 + 0xd8);
      if (iVar2 != 0) {
        *(u32 *)(iVar2 + 4) = *(u32 *)(iVar2 + 4) | 0x40000000;
        *(s32 *)(iVar2 + 0xb0) = param_2;
        if (*(int *)(iVar2 + 0xd8) != 0) {
          hitDamageReturnSet(*(int *)(iVar2 + 0xd8), param_2, 1);
        }
        if (*(int *)(iVar2 + 0xdc) != 0) {
          hitDamageReturnSet(*(int *)(iVar2 + 0xdc), param_2, 1);
        }
      }
      iVar3 = *(int *)(iVar3 + 0xdc);
      if (iVar3 != 0) {
        *(u32 *)(iVar3 + 4) = *(u32 *)(iVar3 + 4) | 0x40000000;
        *(s32 *)(iVar3 + 0xb0) = param_2;
        if (*(int *)(iVar3 + 0xd8) != 0) {
          hitDamageReturnSet(*(int *)(iVar3 + 0xd8), param_2, 1);
        }
        if (*(int *)(iVar3 + 0xdc) != 0) {
          hitDamageReturnSet(*(int *)(iVar3 + 0xdc), param_2, 1);
        }
      }
    }
  }
  return;
  return 0;
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
        hitDamageReturnSet((s32)hit, value, 0);
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

const f32 vec3_802bf7b8[3] = { 0.0f, 0.0f, 0.0f };
const f32 vec3_802bf7c4[3] = { 0.0f, 0.0f, 0.0f };
const f32 vec3_802bf7d0[3] = { 0.0f, 0.0f, 0.0f };
const f32 vec3_802bf7dc[3] = { 0.0f, 0.0f, 0.0f };
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
