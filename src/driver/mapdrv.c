#include "driver/mapdrv.h"


void mapErrorEntry(s32 type, char* message) {
    extern char error_data[];
    extern s32 error_count;
    extern s32 error_flag;
    extern char* strncpy(char* dst, const char* src, u32 n);
    char* entry;

    entry = error_data;
    entry += error_count * 0x24;
    *(s32*)(entry + 0x20) = type;
    strncpy(entry, message, 0x1F);
    error_flag = 1;
    error_count++;
    error_count %= 32;
}

void* getMapDataDvdRoot(void) {
    extern char str__8041f928[1];
    return str__8041f928;
}

void N_mapDispOff(void) {
    extern s32 unk_8041e678;
    unk_8041e678 = 1;
}

void N_mapDispOn(void) {
    extern s32 unk_8041e678;
    unk_8041e678 = 0;
}

void* mapGetWork(void) {
    extern s32 activeGroup;
    extern s32 mapWork;
    return (void*)(mapWork + activeGroup * 0x2F4);
}
s32 mapGetActiveGroup(void) {
    extern s32 activeGroup;
    return activeGroup;
}

void mapGetJointsSub(void* joint, s32* count) {
    void* child;
    void* sibling;
    void* level2;
    void* level3;

    (*count)++;
    child = *(void**)((u8*)joint + 0xC);
    if (child != 0) {
        (*count)++;
        level2 = *(void**)((u8*)child + 0xC);
        if (level2 != 0) {
            (*count)++;
            level3 = *(void**)((u8*)level2 + 0xC);
            if (level3 != 0) {
                (*count)++;
                if (*(void**)((u8*)level3 + 0xC) != 0) {
                    mapGetJointsSub(*(void**)((u8*)level3 + 0xC), count);
                }
                if (*(void**)((u8*)level3 + 0x10) != 0) {
                    mapGetJointsSub(*(void**)((u8*)level3 + 0x10), count);
                }
            }
            level3 = *(void**)((u8*)level2 + 0x10);
            if (level3 != 0) {
                (*count)++;
                if (*(void**)((u8*)level3 + 0xC) != 0) {
                    mapGetJointsSub(*(void**)((u8*)level3 + 0xC), count);
                }
                if (*(void**)((u8*)level3 + 0x10) != 0) {
                    mapGetJointsSub(*(void**)((u8*)level3 + 0x10), count);
                }
            }
        }
        level2 = *(void**)((u8*)child + 0x10);
        if (level2 != 0) {
            (*count)++;
            level3 = *(void**)((u8*)level2 + 0xC);
            if (level3 != 0) {
                (*count)++;
                if (*(void**)((u8*)level3 + 0xC) != 0) {
                    mapGetJointsSub(*(void**)((u8*)level3 + 0xC), count);
                }
                if (*(void**)((u8*)level3 + 0x10) != 0) {
                    mapGetJointsSub(*(void**)((u8*)level3 + 0x10), count);
                }
            }
            level3 = *(void**)((u8*)level2 + 0x10);
            if (level3 != 0) {
                (*count)++;
                if (*(void**)((u8*)level3 + 0xC) != 0) {
                    mapGetJointsSub(*(void**)((u8*)level3 + 0xC), count);
                }
                if (*(void**)((u8*)level3 + 0x10) != 0) {
                    mapGetJointsSub(*(void**)((u8*)level3 + 0x10), count);
                }
            }
        }
    }
    sibling = *(void**)((u8*)joint + 0x10);
    if (sibling != 0) {
        (*count)++;
        level2 = *(void**)((u8*)sibling + 0xC);
        if (level2 != 0) {
            (*count)++;
            level3 = *(void**)((u8*)level2 + 0xC);
            if (level3 != 0) {
                (*count)++;
                if (*(void**)((u8*)level3 + 0xC) != 0) mapGetJointsSub(*(void**)((u8*)level3 + 0xC), count);
                if (*(void**)((u8*)level3 + 0x10) != 0) mapGetJointsSub(*(void**)((u8*)level3 + 0x10), count);
            }
            level3 = *(void**)((u8*)level2 + 0x10);
            if (level3 != 0) {
                (*count)++;
                if (*(void**)((u8*)level3 + 0xC) != 0) mapGetJointsSub(*(void**)((u8*)level3 + 0xC), count);
                if (*(void**)((u8*)level3 + 0x10) != 0) mapGetJointsSub(*(void**)((u8*)level3 + 0x10), count);
            }
        }
        level2 = *(void**)((u8*)sibling + 0x10);
        if (level2 != 0) {
            (*count)++;
            level3 = *(void**)((u8*)level2 + 0xC);
            if (level3 != 0) {
                (*count)++;
                if (*(void**)((u8*)level3 + 0xC) != 0) mapGetJointsSub(*(void**)((u8*)level3 + 0xC), count);
                if (*(void**)((u8*)level3 + 0x10) != 0) mapGetJointsSub(*(void**)((u8*)level3 + 0x10), count);
            }
            level3 = *(void**)((u8*)level2 + 0x10);
            if (level3 != 0) {
                (*count)++;
                if (*(void**)((u8*)level3 + 0xC) != 0) mapGetJointsSub(*(void**)((u8*)level3 + 0xC), count);
                if (*(void**)((u8*)level3 + 0x10) != 0) mapGetJointsSub(*(void**)((u8*)level3 + 0x10), count);
            }
        }
    }
}

s32 mapGetJoints(void* data) {
    extern void mapGetJointsSub(void*, s32*);
    s32 count = 0;
    if (data != 0) {
        count = 1;
        data = *(void**)((s32)data + 0xC);
        if (data != 0) {
            mapGetJointsSub(data, &count);
        }
    }
    return count;
}

void mapGetBoundingBox(void* minRaw, void* maxRaw) {
    extern s32 activeGroup;
    extern s32 mapWork;
    extern void PSMTXMultVec(void*, void*, void*);
    f32* min;
    f32* max;
    f32 point[3];
    u8* entry;
    u8* object;
    u8* joint;
    s32 i;

    min = minRaw;
    max = maxRaw;
    min[0] = 1000000.0f;
    min[1] = 1000000.0f;
    min[2] = 1000000.0f;
    max[0] = -1000000.0f;
    max[1] = -1000000.0f;
    max[2] = -1000000.0f;
    entry = (u8*)(mapWork + activeGroup * 0x2F4);
    object = *(u8**)(entry + 0x154);
    for (i = 0; i < *(s32*)(entry + 0x150); i++, object += 0x134) {
        joint = *(u8**)(object + 8);
        if (joint != 0 && *(s32*)(joint + 0x5C) != 0) {
            PSMTXMultVec(object + 0x1C, joint + 0x3C, point);
            if (point[0] < min[0]) min[0] = point[0];
            if (point[1] < min[1]) min[1] = point[1];
            if (point[2] < min[2]) min[2] = point[2];
            if (point[0] > max[0]) max[0] = point[0];
            if (point[1] > max[1]) max[1] = point[1];
            if (point[2] > max[2]) max[2] = point[2];
            PSMTXMultVec(object + 0x1C, joint + 0x48, point);
            if (point[0] < min[0]) min[0] = point[0];
            if (point[1] < min[1]) min[1] = point[1];
            if (point[2] < min[2]) min[2] = point[2];
            if (point[0] > max[0]) max[0] = point[0];
            if (point[1] > max[1]) max[1] = point[1];
            if (point[2] > max[2]) max[2] = point[2];
        }
    }
}

void mapInit(void) {
    extern void* __memAlloc(s32, u32);
    extern void* memset(void*, s32, u32);
    extern void* mapWork;
    extern s32 activeGroup;
    extern s32 mapClipOffFlag;
    mapWork = __memAlloc(0, 0x5E8);
    memset(mapWork, 0, 0x5E8);
    activeGroup = 0;
    mapClipOffFlag = 0;
}

u8 mapBuildTexture(void* objectRaw, int palette, int* names) {
    extern s32 strcmp(const char*, const char*);
    extern void* TEXGet(s32, s32);
    u8* object;
    u8* joint;
    u8* material;
    u8* texture;
    u8* descriptor;
    u8* header;
    char* name;
    s32 partIndex;
    s32 textureIndex;
    s32 nameIndex;
    s32 format;

    object = objectRaw;
    joint = *(u8**)(object + 8);
    for (partIndex = 0; partIndex < *(s32*)(joint + 0x5C); partIndex++) {
        material = *(u8**)(joint + 0x64 + partIndex * 8);
        for (textureIndex = 0; textureIndex < material[0xB]; textureIndex++) {
            texture = *(u8**)(material + 0xC + textureIndex * 4);
            name = *(char**)*(u32*)texture;
            for (nameIndex = 0; nameIndex < names[0]; nameIndex++) {
                if (strcmp((char*)names[nameIndex + 1], name) == 0) {
                    break;
                }
            }
            if (nameIndex == names[0]) {
                nameIndex = 0;
            }
            descriptor = TEXGet(palette, nameIndex);
            header = *(u8**)descriptor;
            *(void**)(*(u8**)texture + 0xC) = *(void**)(header + 8);
            *(u16*)(*(u8**)texture + 8) = *(u16*)(header + 2);
            *(u16*)(*(u8**)texture + 10) = *(u16*)header;
            format = *(s32*)(header + 4);
            switch (format) {
                case 0:  (*(u8**)texture)[4] = 4; break;
                case 1:  (*(u8**)texture)[4] = 5; break;
                case 2:  (*(u8**)texture)[4] = 6; break;
                case 3:  (*(u8**)texture)[4] = 7; break;
                case 4:  (*(u8**)texture)[4] = 0; break;
                case 5:  (*(u8**)texture)[4] = 1; break;
                case 6:  (*(u8**)texture)[4] = 2; break;
                case 8:  (*(u8**)texture)[4] = 8; break;
                case 9:  (*(u8**)texture)[4] = 9; break;
                case 10: (*(u8**)texture)[4] = 10; break;
                case 14: (*(u8**)texture)[4] = 3; break;
            }
        }
    }
    if (*(void**)(object + 0xE4) != 0) {
        mapBuildTexture(*(void**)(object + 0xE4), palette, names);
    }
    if (*(void**)(object + 0xE8) != 0) {
        mapBuildTexture(*(void**)(object + 0xE8), palette, names);
    }
    return 0;
}

void* _mapEnt(void* jointRaw, void* parent, void* parentMtx, int entryIdx) {
    extern s32 activeGroup;
    extern s32 mapWork;
    extern f32 float_deg2rad_8041f948;
    extern void PSMTXTrans(void*, f32, f32, f32);
    extern void PSMTXScale(void*, f32, f32, f32);
    extern void PSMTXRotRad(void*, s32, f32);
    extern void PSMTXConcat(void*, void*, void*);
    extern void PSMTXIdentity(void*);
    extern void PSVECAdd(void*, void*, void*);
    extern void PSVECScale(void*, void*, f32);
    u8* joint;
    u8* entry;
    u8* object;
    u8* drawMode;
    s32 remaining;
    f32 trans[3][4];
    f32 scale[3][4];
    f32 rotX[3][4];
    f32 rotY[3][4];
    f32 rotZ[3][4];

    joint = jointRaw;
    entry = (u8*)(mapWork + activeGroup * 0x2F4 + entryIdx * 0x178);
    remaining = *(s32*)(entry + 0x150);
    object = *(u8**)(entry + 0x154);
    while (remaining > 0 && *(void**)(object + 8) != 0) {
        object += 0x134;
        remaining--;
    }
    *(u32*)object = 0;
    *(void**)(object + 8) = joint;
    *(s16*)(object + 0xDE) = entryIdx;
    PSMTXTrans(trans, *(f32*)(joint + 0x30), *(f32*)(joint + 0x34), *(f32*)(joint + 0x38));
    PSMTXScale(scale, *(f32*)(joint + 0x18), *(f32*)(joint + 0x1C), *(f32*)(joint + 0x20));
    PSMTXRotRad(rotX, 'x', float_deg2rad_8041f948 * *(f32*)(joint + 0x24));
    PSMTXRotRad(rotY, 'y', float_deg2rad_8041f948 * *(f32*)(joint + 0x28));
    PSMTXRotRad(rotZ, 'z', float_deg2rad_8041f948 * *(f32*)(joint + 0x2C));
    PSMTXConcat(trans, rotZ, trans);
    PSMTXConcat(trans, rotY, trans);
    PSMTXConcat(trans, rotX, trans);
    PSMTXConcat(trans, scale, object + 0x4C);
    PSMTXConcat(parentMtx, object + 0x4C, object + 0x1C);
    PSMTXIdentity(object + 0xAC);
    *(u32*)object |= 0x200;
    *(u8*)(object + 5) = 4;
    *(s16*)(object + 0xDC) = -1;
    *(void**)(object + 0xE0) = parent;
    *(void**)(object + 0xE4) = 0;
    *(void**)(object + 0xE8) = 0;
    drawMode = *(u8**)(joint + 0x58);
    if (drawMode != 0) {
        if ((drawMode[2] & 1) != 0) {
            *(u32*)object |= 8;
        }
        if (drawMode[0] == 0) {
            *(u8*)(object + 4) = (*(u32*)object & 8) ? 3 : 0;
        } else if (drawMode[0] == 1) {
            *(u8*)(object + 4) = (*(u32*)object & 8) ? 3 : 0;
        } else if (drawMode[0] == 2) {
            *(u8*)(object + 4) = (*(u32*)object & 8) ? 4 : 1;
        } else if (drawMode[0] == 3) {
            *(u8*)(object + 4) = (*(u32*)object & 8) ? 5 : 2;
        }
        if ((drawMode[2] & 2) != 0) {
            *(u32*)object |= 0x10;
            if ((drawMode[2] & 0x10) != 0) {
                *(u32*)object |= 0x8000;
            }
        }
        if ((drawMode[2] & 4) != 0) {
            *(u32*)object |= 4;
        }
    }
    if (*(s32*)(joint + 0x5C) == 0) {
        *(f32*)(object + 0x10) = *(f32*)(joint + 0x30);
        *(f32*)(object + 0x14) = *(f32*)(joint + 0x34);
        *(f32*)(object + 0x18) = *(f32*)(joint + 0x38);
    } else {
        PSVECAdd(joint + 0x3C, joint + 0x48, object + 0x10);
        PSVECScale(object + 0x10, object + 0x10, 0.5f);
    }
    return object;
}

void* mapEntrySub(void* joint, void* parent, void* mtx, int isRoot, int entryIdx) {
    extern void* _mapEnt(void*, void*, void*, int);
    void* result;
    void* child;
    void* sibling;
    void* childObj;
    void* grand;
    void* grandObj;
    void* next;
    void* nextObj;

    result = _mapEnt(joint, parent, mtx, entryIdx);
    child = *(void**)((u8*)joint + 0xC);
    if (child != 0) {
        childObj = _mapEnt(child, result, (u8*)result + 0x1C, entryIdx);
        grand = *(void**)((u8*)child + 0xC);
        if (grand != 0) {
            grandObj = _mapEnt(grand, childObj, (u8*)childObj + 0x1C, entryIdx);
            next = *(void**)((u8*)grand + 0xC);
            if (next != 0) {
                nextObj = mapEntrySub(next, grandObj, (u8*)grandObj + 0x1C, 0, entryIdx);
                *(void**)((u8*)grandObj + 0xE4) = nextObj;
            }
            next = *(void**)((u8*)grand + 0x10);
            if (next != 0) {
                nextObj = mapEntrySub(next, childObj, (u8*)childObj + 0x1C, 0, entryIdx);
                *(void**)((u8*)grandObj + 0xE8) = nextObj;
            }
            *(void**)((u8*)childObj + 0xE4) = grandObj;
        }
        sibling = *(void**)((u8*)child + 0x10);
        if (sibling != 0) {
            grandObj = _mapEnt(sibling, result, (u8*)result + 0x1C, entryIdx);
            next = *(void**)((u8*)sibling + 0xC);
            if (next != 0) {
                nextObj = mapEntrySub(next, grandObj, (u8*)grandObj + 0x1C, 0, entryIdx);
                *(void**)((u8*)grandObj + 0xE4) = nextObj;
            }
            next = *(void**)((u8*)sibling + 0x10);
            if (next != 0) {
                nextObj = mapEntrySub(next, result, (u8*)result + 0x1C, 0, entryIdx);
                *(void**)((u8*)grandObj + 0xE8) = nextObj;
            }
            *(void**)((u8*)childObj + 0xE8) = grandObj;
        }
        *(void**)((u8*)result + 0xE4) = childObj;
    }
    if (isRoot == 0) {
        sibling = *(void**)((u8*)joint + 0x10);
        if (sibling != 0) {
            childObj = _mapEnt(sibling, parent, mtx, entryIdx);
            grand = *(void**)((u8*)sibling + 0xC);
            if (grand != 0) {
                grandObj = _mapEnt(grand, childObj, (u8*)childObj + 0x1C, entryIdx);
                next = *(void**)((u8*)grand + 0xC);
                if (next != 0) {
                    nextObj = mapEntrySub(next, grandObj, (u8*)grandObj + 0x1C, 0, entryIdx);
                    *(void**)((u8*)grandObj + 0xE4) = nextObj;
                }
                next = *(void**)((u8*)grand + 0x10);
                if (next != 0) {
                    nextObj = mapEntrySub(next, childObj, (u8*)childObj + 0x1C, 0, entryIdx);
                    *(void**)((u8*)grandObj + 0xE8) = nextObj;
                }
                *(void**)((u8*)childObj + 0xE4) = grandObj;
            }
            grand = *(void**)((u8*)sibling + 0x10);
            if (grand != 0) {
                grandObj = _mapEnt(grand, parent, mtx, entryIdx);
                next = *(void**)((u8*)grand + 0xC);
                if (next != 0) {
                    nextObj = mapEntrySub(next, grandObj, (u8*)grandObj + 0x1C, 0, entryIdx);
                    *(void**)((u8*)grandObj + 0xE4) = nextObj;
                }
                next = *(void**)((u8*)grand + 0x10);
                if (next != 0) {
                    nextObj = mapEntrySub(next, parent, mtx, 0, entryIdx);
                    *(void**)((u8*)grandObj + 0xE8) = nextObj;
                }
                *(void**)((u8*)childObj + 0xE8) = grandObj;
            }
            *(void**)((u8*)result + 0xE8) = childObj;
        }
    }
    return result;
}

void* mapEntry(void* pJoint, void* pMtx, int entryIdx) {
    extern s32 activeGroup;
    extern s32 mapWork;
    extern void* mapalloc_base_ptr;
    extern s32 mapGetJoints(void*);
    extern void* _mapAlloc(void*, u32);
    extern void* memset(void*, s32, u32);
    extern void* mapEntrySub(void*, void*, void*, int, int);
    u8* entry;
    u32 size;
    s32 count;

    entry = (u8*)(mapWork + activeGroup * 0x2F4 + entryIdx * 0x178);
    count = mapGetJoints(pJoint);
    *(s32*)(entry + 0x150) = count;
    size = (count * 0x134 + 0x1F) & ~0x1F;
    *(void**)(entry + 0x154) = _mapAlloc(mapalloc_base_ptr, size);
    memset(*(void**)(entry + 0x154), 0, size);
    return mapEntrySub(pJoint, 0, pMtx, 1, entryIdx);
}

u8 makeDisplayList(int entryIdx) {
    extern s32 activeGroup;
    extern s32 mapWork;
    extern s32 unk_8041e680;
    extern void* mapalloc_base_ptr;
    extern void* _mapAlloc(void*, u32);
    extern void* _mapAllocTail(void*, u32);
    extern void _mapFree(void*, void*);
    extern void* memset(void*, s32, u32);
    extern void* memcpy(void*, const void*, u32);
    extern void DCInvalidateRange(void*, u32);
    extern void DCFlushRange(void*, u32);
    extern void GXBeginDisplayList(void*, u32);
    extern void GXResetWriteGatherPipe(void);
    extern u32 GXEndDisplayList(void);
    extern void mapSetMaterial(void*, void*);
    extern void GXSetCullMode(s32);
    extern s32 shadowGetCharShadowProjection(void);
    extern u32 culling_678[];
    u8* entry;
    u8* object;
    u8* joint;
    u8* writePtr;
    void* scratch;
    void* material;
    s32 pass;
    s32 objectIndex;
    s32 partIndex;
    u32 total;
    u32 size;

    entry = (u8*)(mapWork + activeGroup * 0x2F4 + entryIdx * 0x178);
    scratch = _mapAlloc(mapalloc_base_ptr, 0x10000);
    unk_8041e680 = 1;
    total = 0;
    writePtr = 0;
    for (pass = 0; pass < 2; pass++) {
        object = *(u8**)(entry + 0x154);
        for (objectIndex = 0; objectIndex < *(s32*)(entry + 0x150); objectIndex++, object += 0x134) {
            *(void**)(object + 0xF4) = 0;
            *(u32*)(object + 0x114) = 0;
            joint = *(u8**)(object + 8);
            if (*(s32*)(joint + 0x5C) > 0 && *(s32*)(joint + 0x5C) < 9 &&
                ((*(u8*)(*(u8**)(joint + 0x58) + 2) & 8) == 0) &&
                *(u32*)(*(u8**)(joint + 0x58) + 4) == 0) {
                if ((shadowGetCharShadowProjection() == 0 || (*(u32*)object & 4) == 0) &&
                    (*(u16*)(entry + 4) & 0x8000) != 0) {
                    for (partIndex = 0; partIndex < *(s32*)(joint + 0x5C); partIndex++) {
                        material = *(void**)(joint + 0x64 + partIndex * 8);
                        memset(scratch, 0, 0x10000);
                        DCInvalidateRange(scratch, 0x10000);
                        GXBeginDisplayList(scratch, 0x10000);
                        GXResetWriteGatherPipe();
                        mapSetMaterial(object, material);
                        GXSetCullMode(culling_678[*(u8*)(*(u8**)(joint + 0x58) + 1)]);
                        size = GXEndDisplayList();
                        *(u32*)(object + 0x114 + partIndex * 4) = size;
                        if (pass == 0) {
                            total += size;
                        } else {
                            *(void**)(object + 0xF4 + partIndex * 4) = writePtr;
                            memcpy(writePtr, scratch, size);
                            DCFlushRange(writePtr, size);
                            writePtr += size;
                        }
                    }
                }
            }
        }
        if (pass == 0) {
            *(u32*)(entry + 0x168) = total;
            if (total == 0) {
                *(void**)(entry + 0x16C) = 0;
                break;
            }
            *(void**)(entry + 0x16C) = _mapAllocTail(mapalloc_base_ptr, total);
            writePtr = *(u8**)(entry + 0x16C);
        }
    }
    _mapFree(mapalloc_base_ptr, scratch);
    unk_8041e680 = 0;
    return 0;
}

void* mapSearchAnmObj(char* name) {
    extern s32 activeGroup;
    extern s32 mapWork;
    extern s32 strcmp(const char*, const char*);

    void* group;
    void* work;
    s32 j;
    s32 i;
    void* anim;

    work = (void*)(mapWork + activeGroup * 0x2F4);
    group = work;
    i = 0;
    while (i < *(s32*)work) {
        anim = *(void**)((s32)group + 0x164);
        j = 0;
        while (j < *(s32*)((s32)group + 0x160)) {
            if ((*(u16*)anim & 1) != 0) {
                if (strcmp(*(const char**)*(s32*)((s32)anim + 0x18), name) == 0) {
                    return anim;
                }
            }
            j++;
            anim = (void*)((s32)anim + 0x20);
        }
        group = (void*)((s32)group + 0x178);
        i++;
    }

    return 0;
}

void mapCheckAnimation(s32 nameValue, s32* outDone, f32* outFrame) {
    extern s32 activeGroup;
    extern s32 mapWork;
    extern s32 strcmp(const char*, const char*);
    extern f32 float_0_8041f930;
    u8* group;
    u8* entry;
    u8* anim;
    char* name;
    s32 entryIndex;
    s32 animIndex;

    name = (char*)nameValue;
    group = (u8*)(mapWork + activeGroup * 0x2F4);
    entry = group;
    anim = 0;
    for (entryIndex = 0; entryIndex < *(s32*)group; entryIndex++) {
        anim = *(u8**)(entry + 0x164);
        for (animIndex = 0; animIndex < *(s32*)(entry + 0x160); animIndex++) {
            if (((*(u16*)anim & 1) != 0) &&
                strcmp(*(char**)*(u32*)(anim + 0x18), name) == 0) {
                goto found;
            }
            anim += 0x20;
        }
        entry += 0x178;
    }
    anim = 0;

found:
    *outDone = 0;
    *outFrame = float_0_8041f930;
    if (anim == 0) {
        *outDone = 1;
    } else {
        if ((*(u16*)anim & 2) != 0) {
            *outDone = 1;
        }
        *outFrame = *(f32*)(anim + 0x10);
    }
}

u8 mapPlayAnimationLv(char* name, int mode, int level) {
    extern s32 activeGroup;
    extern s32 mapWork;
    extern s32 strcmp(const char*, const char*);
    u8* group;
    u8* entry;
    u8* anim;
    s32 entryIndex;
    s32 animIndex;
    s32 found;

    group = (u8*)(mapWork + activeGroup * 0x2F4);
    entry = group;
    anim = 0;
    found = 0;
    for (entryIndex = 0; entryIndex < *(s32*)group; entryIndex++) {
        anim = *(u8**)(entry + 0x164);
        for (animIndex = 0; animIndex < *(s32*)(entry + 0x160); animIndex++) {
            if (strcmp(*(char**)*(u32*)(anim + 0x18), name) == 0) {
                found = 1;
                break;
            }
            anim += 0x20;
        }
        if (found) {
            break;
        }
        entry += 0x178;
    }

    if (found) {
        *(u16*)anim = 0;
        if (level == 1) {
            *(u16*)anim |= 0x100;
        } else if (level == 2) {
            *(u16*)anim |= 0x200;
        } else if (level == 3) {
            *(u16*)anim |= 0x400;
        } else if (level == 4) {
            *(u16*)anim |= 0x800;
        }
        *(f32*)(anim + 0x10) = 0.0f;
        *(f32*)(anim + 0x14) = 0.0f;
        *(u16*)anim |= 1;
        if (mode == 1) {
            *(u16*)anim |= 4;
        }
    }
    return 0;
}


void mapPauseAnimation(char* name) {
    extern s32 activeGroup;
    extern s32 mapWork;
    extern s32 strcmp(const char*, const char*);
    void* work;
    void* group;
    void* anim;
    s32 i;
    s32 j;

    work = (void*)(mapWork + activeGroup * 0x2F4);
    group = work;
    i = 0;
    while (i < *(s32*)work) {
        anim = *(void**)((s32)group + 0x164);
        j = 0;
        while (j < *(s32*)((s32)group + 0x160)) {
            if ((*(u16*)anim & 1) != 0) {
                if (strcmp(*(const char**)*(s32*)((s32)anim + 0x18), name) == 0) {
                    goto found;
                }
            }
            j++;
            anim = (void*)((s32)anim + 0x20);
        }
        group = (void*)((s32)group + 0x178);
        i++;
    }
    anim = 0;

found:
    if (anim != 0) {
        *(u16*)anim |= 0x10;
    }
}

void mapReStartAnimation(char* name) {
    extern s32 activeGroup;
    extern s32 mapWork;
    extern s32 strcmp(const char*, const char*);
    void* work;
    void* group;
    void* anim;
    s32 i;
    s32 j;

    work = (void*)(mapWork + activeGroup * 0x2F4);
    group = work;
    i = 0;
    while (i < *(s32*)work) {
        anim = *(void**)((s32)group + 0x164);
        j = 0;
        while (j < *(s32*)((s32)group + 0x160)) {
            if ((*(u16*)anim & 1) != 0) {
                if (strcmp(*(const char**)*(s32*)((s32)anim + 0x18), name) == 0) {
                    goto found;
                }
            }
            j++;
            anim = (void*)((s32)anim + 0x20);
        }
        group = (void*)((s32)group + 0x178);
        i++;
    }
    anim = 0;

found:
    if (anim != 0) {
        *(u16*)anim |= 0x20;
    }
}


void mapPauseAnimationAll(void) {
    extern s32 activeGroup;
    extern s32 mapWork;
    void* work;
    void* group;
    s32 j;
    s32 i;
    void* anim;

    work = (void*)(mapWork + activeGroup * 0x2F4);
    group = work;
    i = 0;
    while (i < *(s32*)work) {
        anim = *(void**)((s32)group + 0x164);
        j = 0;
        while (j < *(s32*)((s32)group + 0x160)) {
            *(u16*)anim |= 0x10;
            j++;
            anim = (void*)((s32)anim + 0x20);
        }
        group = (void*)((s32)group + 0x178);
        i++;
    }
}

void mapReStartAnimationAll(void) {
    extern s32 activeGroup;
    extern s32 mapWork;
    void* work;
    void* group;
    s32 j;
    s32 i;
    void* anim;

    work = (void*)(mapWork + activeGroup * 0x2F4);
    group = work;
    i = 0;
    while (i < *(s32*)work) {
        anim = *(void**)((s32)group + 0x164);
        j = 0;
        while (j < *(s32*)((s32)group + 0x160)) {
            *(u16*)anim |= 0x20;
            j++;
            anim = (void*)((s32)anim + 0x20);
        }
        group = (void*)((s32)group + 0x178);
        i++;
    }
}

void mapSetPlayRate(char* name, f32 rate) {
    extern s32 activeGroup;
    extern s32 mapWork;
    extern s32 strcmp(const char*, const char*);
    void* work;
    void* group;
    void* anim;
    s32 i;
    s32 j;

    work = (void*)(mapWork + activeGroup * 0x2F4);
    group = work;
    i = 0;
    while (i < *(s32*)work) {
        anim = *(void**)((s32)group + 0x164);
        j = 0;
        while (j < *(s32*)((s32)group + 0x160)) {
            if ((*(u16*)anim & 1) != 0) {
                if (strcmp(*(const char**)*(s32*)((s32)anim + 0x18), name) == 0) {
                    goto found;
                }
            }
            j++;
            anim = (void*)((s32)anim + 0x20);
        }
        group = (void*)((s32)group + 0x178);
        i++;
    }
    anim = 0;

found:
    if (anim != 0) {
        *(f32*)((s32)anim + 0x14) = rate;
    }
}


void _mapLoad(void* pWork, int idx, char* pMapName) {
    extern s32 error_flag;
    extern s32 error_count;
    extern void* mapalloc_base_ptr;
    extern void arcOpen(char*, void*, void*);
    extern void* DVDMgrOpen(char*, s32, s32);
    extern s32 DVDMgrGetLength(void*);
    extern void DVDMgrRead(void*, void*, u32, s32);
    extern void DVDMgrClose(void*);
    extern void* _mapAllocTail(void*, u32);
    extern void UnpackTexPalette(void*);
    extern void GXInvalidateVtxCache(void);
    extern s32 sprintf(char*, const char*, ...);
    extern char* strncpy(char*, const char*, u32);
    extern void* memcpy(void*, const void*, u32);
    extern void* memset(void*, s32, u32);
    extern s32 strcmp(const char*, const char*);
    extern void* mapSearchDmdJoint(void*, char*);
    extern void PSMTXScale(void*, f32, f32, f32);
    extern void* mapEntry(void*, void*, int);
    extern void* hitEntry(void*, void*, int);
    extern s32 activeGroup;
    extern void camLoadRoad(s32, char*);
    extern void camSetTypePersp(s32);
    extern u8 mapBuildTexture(void*, int, int*);
    extern void mapGetBoundingBox(void*, void*);
    extern void makeDisplayList(s32);
    extern u32 unk_80429520;
    u8* entry;
    char filePath[128];
    char arcName[132];
    void* file;
    u32 size;
    u8* data;
    u32 offset;
    u32 i;
    u8* chunkTable;
    char* stringTable;
    u8* mainData;
    u32* info;
    void* mapJoint;
    void* hitJoint;
    f32 matrix[3][4];
    f32 min[3];
    f32 max[3];

    error_flag = 0;
    error_count = 0;
    entry = (u8*)pWork + idx * 0x178;
    strncpy((char*)entry + 6, pMapName, 0x10);
    sprintf(arcName, "m/%s/d", pMapName);
    arcOpen(arcName, entry + 0x7C, entry + 0x80);
    if (*(void**)(entry + 0x7C) == 0) {
        sprintf(filePath, "%s/%s", ".", arcName);
        file = DVDMgrOpen(filePath, 2, 0);
        size = DVDMgrGetLength(file);
        *(u32*)(entry + 0x80) = size;
        data = _mapAllocTail(mapalloc_base_ptr, (size + 0x1F) & ~0x1F);
        *(void**)(entry + 0x7C) = data;
        DVDMgrRead(file, data, (size + 0x1F) & ~0x1F, 0);
        DVDMgrClose(file);
    }
    sprintf(arcName, "m/%s/t", pMapName);
    *(void**)(entry + 0x84) = 0;
    arcOpen(arcName, entry + 0x84, entry + 0x88);
    if (*(void**)(entry + 0x84) == 0) {
        sprintf(filePath, "%s/%s", ".", arcName);
        file = DVDMgrOpen(filePath, 2, 0);
        if (file != 0) {
            size = DVDMgrGetLength(file);
            *(u32*)(entry + 0x88) = size;
            data = _mapAllocTail(mapalloc_base_ptr, (size + 0x1F) & ~0x1F);
            *(void**)(entry + 0x84) = data;
            DVDMgrRead(file, data, (size + 0x1F) & ~0x1F, 0);
            DVDMgrClose(file);
            UnpackTexPalette(data);
        }
    } else {
        UnpackTexPalette(*(void**)(entry + 0x84));
    }
    GXInvalidateVtxCache();

    data = *(u8**)(entry + 0x7C);
    memset(entry + 0x38, 0, 0x44);
    *(u32*)(entry + 0x74) |= 1;
    memcpy(entry + 0x38, data, 0x20);
    if (*(u32*)(entry + 0x3C) == *(u32*)(entry + 0x80)) {
        offset = 0x20;
        if (*(u32*)(entry + 0x40) != 0) {
            *(void**)(entry + 0x58) = data + offset;
            offset += *(u32*)(entry + 0x40);
        }
        if (*(u32*)(entry + 0x44) != 0) {
            *(void**)(entry + 0x5C) = data + offset;
            offset += *(u32*)(entry + 0x44) * 4;
        }
        if (*(u32*)(entry + 0x48) != 0) {
            *(void**)(entry + 0x60) = data + offset;
            offset += *(u32*)(entry + 0x48) * 8;
        }
        if (*(u32*)(entry + 0x4C) != 0) {
            *(void**)(entry + 0x68) = data + offset;
            offset += *(u32*)(entry + 0x4C) * 8;
        }
        if (offset < *(u32*)(entry + 0x3C)) {
            *(char**)(entry + 0x64) = (char*)data + offset;
        }
        *(void**)(entry + 0x78) = data;
        for (i = 0; i < *(u32*)(entry + 0x44); i++) {
            u32* rel = (u32*)(*(u8**)(entry + 0x58) + ((u32*)*(void**)(entry + 0x5C))[i]);
            if (*rel <= 0x80000000) {
                *rel += (u32)*(void**)(entry + 0x58);
            }
        }
    }

    mainData = *(u8**)(entry + 0x58);
    chunkTable = *(u8**)(entry + 0x60);
    stringTable = *(char**)(entry + 0x64);
    info = 0;
    *(void**)(entry + 0x94) = 0;
    *(void**)(entry + 0x98) = 0;
    *(void**)(entry + 0x9C) = 0;
    *(void**)(entry + 0xA0) = 0;
    *(void**)(entry + 0xA4) = 0;
    for (i = 0; i < *(u32*)(entry + 0x48); i++) {
        u32 chunkOffset = *(u32*)(chunkTable + i * 8);
        char* chunkName = stringTable + *(u32*)(chunkTable + i * 8 + 4);
        if (strcmp(chunkName, "information") == 0) {
            info = (u32*)(mainData + chunkOffset);
        } else if (strcmp(chunkName, "texture_table") == 0) {
            *(void**)(entry + 0x94) = mainData + chunkOffset;
        } else if (strcmp(chunkName, "light_table") == 0) {
            *(void**)(entry + 0x98) = mainData + chunkOffset;
        } else if (strcmp(chunkName, "fog_table") == 0) {
            *(void**)(entry + 0x9C) = mainData + chunkOffset;
        } else if (strcmp(chunkName, "material_name_table") == 0) {
            *(void**)(entry + 0xA0) = mainData + chunkOffset;
        } else if (strcmp(chunkName, "vcd_table") == 0) {
            *(void**)(entry + 0xA4) = mainData + chunkOffset;
        }
    }
    if (info != 0) {
        mapJoint = mapSearchDmdJoint((void*)info[1], (char*)info[2]);
        hitJoint = mapSearchDmdJoint((void*)info[1], (char*)info[3]);
        if (strcmp((char*)info[0], "ver1.02") == 0) {
            *(u16*)(entry + 4) |= 0x8000;
        } else {
            *(u16*)(entry + 4) &= 0x7FFF;
        }
        PSMTXScale(matrix, 10.0f, 10.0f, 10.0f);
        *(void**)(entry + 0xA8) = mapEntry(mapJoint, matrix, idx);
        *(void**)(entry + 0xAC) = hitEntry(hitJoint, matrix, idx);
    }

    if (*(void**)(entry + 0x9C) == 0) {
        *(u32*)(entry + 0x24) = 0;
        *(u32*)(entry + 0x28) = 0;
        *(f32*)(entry + 0x2C) = 0.0f;
        *(f32*)(entry + 0x30) = 0.0f;
        *(u32*)(entry + 0x34) = unk_80429520;
        *(u16*)(entry + 4) &= 0xFFFE;
    } else {
        u8* fog = *(u8**)(entry + 0x9C);
        *(u32*)(entry + 0x24) = *(u32*)fog;
        *(u32*)(entry + 0x28) = *(u32*)(fog + 4);
        *(f32*)(entry + 0x2C) = *(f32*)(fog + 8);
        *(f32*)(entry + 0x30) = *(f32*)(fog + 0xC);
        *(u32*)(entry + 0x34) = *(u32*)(fog + 0x10);
        if (*(u32*)(entry + 0x24) != 0) {
            *(u16*)(entry + 4) |= 1;
        }
    }
    if (activeGroup != 1) {
        sprintf(arcName, "m/%s/c", pMapName);
        camLoadRoad(4, arcName);
    }
    camSetTypePersp(4);
    mapBuildTexture(*(void**)(entry + 0xA8), (s32)*(void**)(entry + 0x84), *(int**)(entry + 0x94));
    mapGetBoundingBox(min, max);
    *(f32*)(entry + 0xB0) = max[0];
    *(f32*)(entry + 0xB4) = max[1];
    *(f32*)(entry + 0xB8) = max[2];
    *(f32*)(entry + 0xBC) = min[0];
    *(f32*)(entry + 0xC0) = max[1];
    *(f32*)(entry + 0xC4) = max[2];
    *(f32*)(entry + 0xC8) = max[0];
    *(f32*)(entry + 0xCC) = min[1];
    *(f32*)(entry + 0xD0) = max[2];
    *(f32*)(entry + 0xD4) = max[0];
    *(f32*)(entry + 0xD8) = max[1];
    *(f32*)(entry + 0xDC) = min[2];
    *(f32*)(entry + 0xE0) = min[0];
    *(f32*)(entry + 0xE4) = min[1];
    *(f32*)(entry + 0xE8) = max[2];
    *(f32*)(entry + 0xEC) = max[0];
    *(f32*)(entry + 0xF0) = min[1];
    *(f32*)(entry + 0xF4) = min[2];
    *(f32*)(entry + 0xF8) = min[0];
    *(f32*)(entry + 0xFC) = max[1];
    *(f32*)(entry + 0x100) = min[2];
    *(f32*)(entry + 0x104) = min[0];
    *(f32*)(entry + 0x108) = min[1];
    *(f32*)(entry + 0x10C) = min[2];
    makeDisplayList(idx);
}

void mapUnLoad(void) {
    extern void sysWaitDrawSync(void);
    extern void _mapFree(void* heap, void* ptr);
    extern void hitReInit(void);
    extern void lightReInit(void);
    extern void envReInit(void);
    extern void camUnLoadRoad(s32 cameraId);
    extern void camSetTypePersp(s32 cameraId);
    extern s32 arcDataCheck(u32 ptr);
    extern void strncpy(char* dst, const char* src, u32 n);
    extern void arcDelete(s32 type);
    extern void* mapWork;
    extern s32 activeGroup;
    extern s32 unk_8041e678;
    extern void* mapalloc_base_ptr;
    extern char float_0_8041f9ac[];
    void* work;
    void* entry;
    s32 i;

    sysWaitDrawSync();
    work = mapWork;
    activeGroup = 0;

    entry = work;
    for (i = 0; i < *(s32*)work; i++, entry = (void*)((s32)entry + 0x178)) {
        if (*(void**)((s32)entry + 0x154) != NULL) {
            _mapFree(mapalloc_base_ptr, *(void**)((s32)entry + 0x154));
            *(s32*)((s32)entry + 0x150) = 0;
            *(void**)((s32)entry + 0x154) = NULL;
        }
        if (*(void**)((s32)entry + 0x164) != NULL) {
            _mapFree(mapalloc_base_ptr, *(void**)((s32)entry + 0x164));
            *(s32*)((s32)entry + 0x160) = 0;
            *(void**)((s32)entry + 0x164) = NULL;
        }
        if (*(void**)((s32)entry + 0x16C) != NULL) {
            _mapFree(mapalloc_base_ptr, *(void**)((s32)entry + 0x16C));
            *(void**)((s32)entry + 0x16C) = NULL;
            *(s32*)((s32)entry + 0x168) = 0;
        }
    }

    unk_8041e678 = 0;
    hitReInit();
    lightReInit();
    envReInit();
    camUnLoadRoad(4);
    camSetTypePersp(4);

    entry = work;
    for (i = 0; i < *(s32*)work; i++, entry = (void*)((s32)entry + 0x178)) {
        if (*(void**)((s32)entry + 0x7C) != NULL) {
            if (arcDataCheck((u32)*(void**)((s32)entry + 0x7C)) == 0) {
                _mapFree(mapalloc_base_ptr, *(void**)((s32)entry + 0x7C));
            }
            *(void**)((s32)entry + 0x7C) = NULL;
        }
        if (*(void**)((s32)entry + 0x84) != NULL) {
            if (arcDataCheck((u32)*(void**)((s32)entry + 0x84)) == 0) {
                _mapFree(mapalloc_base_ptr, *(void**)((s32)entry + 0x84));
            }
            *(void**)((s32)entry + 0x84) = NULL;
        }
        if (*(void**)((s32)entry + 0x8C) != NULL) {
            _mapFree(mapalloc_base_ptr, *(void**)((s32)entry + 0x8C));
            *(void**)((s32)entry + 0x8C) = NULL;
        }
        strncpy((char*)((s32)entry + 0x6), float_0_8041f9ac, 0x10);
    }

    arcDelete(1);
    memset(work, 0, 0x2F4);
}

s32 mapPreLoad(void) {
    return 1;
}

void mapLoad(char* name) {
    extern void sysWaitDrawSync(void);
    extern void* memset(void*, s32, u32);
    extern void _mapLoad(void*, int, char*);
    extern s32 activeGroup;
    extern s32 mapWork;
    void* work;

    sysWaitDrawSync();
    work = (void*)mapWork;
    activeGroup = 0;
    memset(work, 0, 0x2F4);
    _mapLoad(work, 0, name);
    *(s32*)work = 1;
}

void bmapUnLoad(void) {
    extern void sysWaitDrawSync(void);
    extern void _mapFree(void* heap, void* ptr);
    extern void* _mapAllocTail(void* heap, u32 size);
    extern void hitReInit(void);
    extern void lightReInit(void);
    extern void envReInit(void);
    extern s32 arcDataCheck(u32 ptr);
    extern void strncpy(char* dst, const char* src, u32 n);
    extern void arcDelete(s32 type);
    extern void arcEntry(s32 type, void* data, u32 size);
    extern void aramMgrToMram(void* aram, void* mram);
    extern void makeDisplayList(s32 index);
    extern void* mapWork;
    extern s32 activeGroup;
    extern s32 unk_8041e678;
    extern void* mapalloc_base_ptr;
    extern char float_0_8041f9ac[];
    void* group;
    void* work;
    void* entry;
    s32 i;

    sysWaitDrawSync();
    work = mapWork;
    activeGroup = 1;
    group = (void*)((s32)work + 0x2F4);

    entry = group;
    for (i = 0; i < *(s32*)group; i++, entry = (void*)((s32)entry + 0x178)) {
        if (*(void**)((s32)entry + 0x154) != NULL) {
            _mapFree(mapalloc_base_ptr, *(void**)((s32)entry + 0x154));
            *(s32*)((s32)entry + 0x150) = 0;
            *(void**)((s32)entry + 0x154) = NULL;
        }
        if (*(void**)((s32)entry + 0x164) != NULL) {
            _mapFree(mapalloc_base_ptr, *(void**)((s32)entry + 0x164));
            *(s32*)((s32)entry + 0x160) = 0;
            *(void**)((s32)entry + 0x164) = NULL;
        }
        if (*(void**)((s32)entry + 0x16C) != NULL) {
            _mapFree(mapalloc_base_ptr, *(void**)((s32)entry + 0x16C));
            *(void**)((s32)entry + 0x16C) = NULL;
            *(s32*)((s32)entry + 0x168) = 0;
        }
    }

    unk_8041e678 = 0;
    hitReInit();
    lightReInit();
    envReInit();

    entry = group;
    for (i = 0; i < *(s32*)group; i++, entry = (void*)((s32)entry + 0x178)) {
        if (*(void**)((s32)entry + 0x7C) != NULL) {
            if (arcDataCheck((u32)*(void**)((s32)entry + 0x7C)) == 0) {
                _mapFree(mapalloc_base_ptr, *(void**)((s32)entry + 0x7C));
            }
            *(void**)((s32)entry + 0x7C) = NULL;
        }
        if (*(void**)((s32)entry + 0x84) != NULL) {
            if (arcDataCheck((u32)*(void**)((s32)entry + 0x84)) == 0) {
                _mapFree(mapalloc_base_ptr, *(void**)((s32)entry + 0x84));
            }
            *(void**)((s32)entry + 0x84) = NULL;
        }
        if (*(void**)((s32)entry + 0x8C) != NULL) {
            _mapFree(mapalloc_base_ptr, *(void**)((s32)entry + 0x8C));
            *(void**)((s32)entry + 0x8C) = NULL;
        }
        strncpy((char*)((s32)entry + 0x6), float_0_8041f9ac, 0x10);
    }

    arcDelete(1);
    arcDelete(2);
    memset(group, 0, 0x2F4);

    activeGroup = 0;
    entry = work;
    for (i = 0; i < *(s32*)work; i++, entry = (void*)((s32)entry + 0x178)) {
        if (*(void**)((s32)entry + 0x170) != NULL) {
            *(void**)((s32)entry + 0x7C) = _mapAllocTail(mapalloc_base_ptr, *(u32*)((s32)entry + 0x80));
            *(void**)((s32)entry + 0x84) = _mapAllocTail(mapalloc_base_ptr, *(u32*)((s32)entry + 0x88));
            aramMgrToMram(*(void**)((s32)entry + 0x170), *(void**)((s32)entry + 0x7C));
            aramMgrToMram(*(void**)((s32)entry + 0x178), *(void**)((s32)entry + 0x84));
            *(void**)((s32)entry + 0x170) = NULL;
            *(void**)((s32)entry + 0x178) = NULL;
        } else {
            *(void**)((s32)entry + 0x8C) = _mapAllocTail(mapalloc_base_ptr, *(u32*)((s32)entry + 0x90));
            aramMgrToMram(*(void**)((s32)entry + 0x174), *(void**)((s32)entry + 0x8C));
            *(void**)((s32)entry + 0x174) = NULL;
        }
        makeDisplayList(i);
    }

    if (*(void**)((s32)work + 0x8C) != NULL) {
        arcEntry(1, *(void**)((s32)work + 0x8C), *(u32*)((s32)work + 0x90));
    }
}

void bmapLoad(char* rankStageName, char* battleStageName) {
    extern s32 activeGroup;
    extern s32 mapWork;
    extern s32 mapalloc_base_ptr;
    extern void sysWaitDrawSync(void);
    extern void _mapFree(s32 base, s32 ptr);
    extern s32 arcDataCheck(u32 data);
    extern void* aramMgrToAram(u32 data, s32 size);
    extern void* memset(void* dst, s32 value, u32 size);
    extern void _mapLoad(void* work, s32 index, char* name);
    u8* work;
    u8* entry;
    u8* group;
    s32 count;
    s32 i;

    sysWaitDrawSync();
    work = (u8*)mapWork;
    activeGroup = 0;
    count = *(s32*)work;
    entry = work;

    for (i = 0; i < count; i++, entry += 0x178) {
        if (*(s32*)(entry + 0x16C) != 0) {
            _mapFree(mapalloc_base_ptr, *(s32*)(entry + 0x16C));
        }
        *(s32*)(entry + 0x168) = 0;
        *(s32*)(entry + 0x16C) = 0;
        *(s32*)(entry + 0x170) = 0;
        *(s32*)(entry + 0x178) = 0;
        *(s32*)(entry + 0x174) = 0;

        if (*(s32*)(entry + 0x7C) != 0) {
            if (arcDataCheck(*(u32*)(entry + 0x7C)) == 0) {
                *(void**)(entry + 0x170) = aramMgrToAram(
                    *(u32*)(entry + 0x7C),
                    *(s32*)(entry + 0x80)
                );
                *(void**)(entry + 0x178) = aramMgrToAram(
                    *(u32*)(entry + 0x84),
                    *(s32*)(entry + 0x88)
                );
                _mapFree(mapalloc_base_ptr, *(s32*)(entry + 0x84));
                _mapFree(mapalloc_base_ptr, *(s32*)(entry + 0x7C));
                *(s32*)(entry + 0x84) = 0;
                *(s32*)(entry + 0x7C) = 0;
            } else {
                *(void**)(entry + 0x174) = aramMgrToAram(
                    *(u32*)(entry + 0x8C),
                    *(s32*)(entry + 0x90)
                );
                _mapFree(mapalloc_base_ptr, *(s32*)(entry + 0x8C));
                *(s32*)(entry + 0x8C) = 0;
            }
        }
    }

    activeGroup = 1;
    group = work + 0x2F4;
    memset(group, 0, 0x2F4);
    _mapLoad(group, 0, rankStageName);
    *(s32*)group = 1;
    if (battleStageName != 0) {
        _mapLoad(group, 1, battleStageName);
        *(s32*)group = 2;
    }
}

void mapCalcAnimMatrix(void* pDstMtx, void* pParentMtx, void* pEntry, void* pTrack) {
    typedef f32 Mtx[3][4];
    extern void PSMTXTrans(void*, f32, f32, f32);
    extern void PSMTXRotRad(void*, s32, f32);
    extern void PSMTXScale(void*, f32, f32, f32);
    extern void PSMTXIdentity(void*);
    extern void PSMTXConcat(void*, void*, void*);
    extern f32 float_0_8041f930;
    extern f32 float_2_8041f938;
    extern f32 float_3_8041f92c;
    extern f32 float_neg2_8041f980;
    extern f32 float_deg2rad_8041f948;
    extern f32 dat_8041f998;
    u8* track;
    f32* previous;
    f32* next;
    f32 values[9];
    f32 frame;
    f32 firstFrame;
    f32 span;
    f32 denominator;
    f32 t;
    f32 t2;
    f32 t3;
    f32 tx;
    f32 ty;
    f32 tz;
    f32 rx;
    f32 ry;
    f32 rz;
    f32 sx;
    f32 sy;
    f32 sz;
    s32 frameCount;
    s32 frameIndex;
    s32 component;
    Mtx result;
    Mtx translation;
    Mtx scale;
    Mtx rotX;
    Mtx rotY;
    Mtx rotZ;

    track = pTrack;
    frameCount = *(s32*)(track + 0x58);
    frameIndex = 1;
    if (frameCount <= 1) {
        return;
    }

    while (frameIndex < frameCount) {
        previous = (f32*)(track + (frameIndex - 1) * 0x1A8 + 0x5C);
        next = (f32*)((u8*)previous + 0x1A8);
        frame = *(f32*)((u8*)pEntry + 0x10);
        if (frame <= next[0] || frameIndex == frameCount - 1) {
            if (frameIndex == frameCount - 1 && next[0] < frame) {
                frame = next[0];
            }

            for (component = 0; component < 9; component++) {
                if (previous[5] == float_0_8041f930) {
                    firstFrame = previous[0];
                    span = next[0] - firstFrame;
                    denominator = span;
                    if (denominator == float_0_8041f930) {
                        denominator = dat_8041f998;
                    }
                    t = (frame - firstFrame) / denominator;
                    t2 = t * t;
                    t3 = t2 * t;
                    values[component] = next[2] * span * (t3 - t2)
                        + next[1] * (float_neg2_8041f980 * t3 + float_3_8041f92c * t2)
                        + previous[1] * (1.0f + (float_2_8041f938 * t3 - float_3_8041f92c * t2))
                        + previous[3] * span * (t - (float_2_8041f938 * t2 - t3));
                } else {
                    values[component] = previous[1];
                }
                previous += 5;
                next += 5;
            }

            tx = values[0] - *(f32*)(track + 4);
            ty = values[1] - *(f32*)(track + 8);
            tz = values[2] - *(f32*)(track + 0xC);
            rx = values[3] - *(f32*)(track + 0x10);
            ry = values[4] - *(f32*)(track + 0x14);
            rz = values[5] - *(f32*)(track + 0x18);
            sx = values[6] / *(f32*)(track + 0x1C);
            sy = values[7] / *(f32*)(track + 0x20);
            sz = values[8] / *(f32*)(track + 0x24);

            if (tx != 0.0f || ty != 0.0f || tz != 0.0f) {
                PSMTXTrans(translation, tx, ty, tz);
            }
            if (rx != 0.0f) {
                PSMTXRotRad(rotX, 'x', float_deg2rad_8041f948 * rx);
            }
            if (ry != 0.0f) {
                PSMTXRotRad(rotY, 'y', float_deg2rad_8041f948 * ry);
            }
            if (rz != 0.0f) {
                PSMTXRotRad(rotZ, 'z', float_deg2rad_8041f948 * rz);
            }
            PSMTXScale(scale, sx, sy, sz);
            PSMTXIdentity(result);
            if (tx != 0.0f || ty != 0.0f || tz != 0.0f) {
                PSMTXConcat(result, translation, result);
            }
            if (rz != 0.0f) {
                PSMTXConcat(result, rotZ, result);
            }
            if (ry != 0.0f) {
                PSMTXConcat(result, rotY, result);
            }
            if (rx != 0.0f) {
                PSMTXConcat(result, rotX, result);
            }
            if (sx != 0.0f || sy != 0.0f || sz != 0.0f) {
                PSMTXConcat(result, scale, result);
            }
            PSMTXConcat(pParentMtx, result, pDstMtx);
            return;
        }
        frameIndex++;
    }
}

void mapReCalcMatrix(void* obj, void* parentMtx, int recurse) {
    extern void PSMTXConcat(void* a, void* b, void* out);

    void* child;
    u32 flags;
    s32 doSelf;

#define FLAGS(n_) (*(u32*)(n_))
#define CHILD(n_) (*(void**)((s32)(n_) + 0xE4))
#define SIB(n_) (*(void**)((s32)(n_) + 0xE8))
#define WORLD(n_) ((void*)((s32)(n_) + 0x1C))
#define LOCAL(n_) ((void*)((s32)(n_) + 0x4C))
#define ALT(n_) ((void*)((s32)(n_) + 0x7C))
#define RUNTIME(n_) ((void*)((s32)(n_) + 0xAC))
#define APPLY_NODE(n_, p_)                                      \
    do {                                                        \
        if ((FLAGS(n_) & 0x2000) == 0) {                        \
            PSMTXConcat((p_), LOCAL(n_), WORLD(n_));            \
        } else {                                                \
            PSMTXConcat((p_), ALT(n_), WORLD(n_));              \
        }                                                       \
        if ((FLAGS(n_) & 0x200000) != 0) {                      \
            PSMTXConcat(WORLD(n_), RUNTIME(n_), WORLD(n_));     \
        }                                                       \
        FLAGS(n_) &= ~0x200;                                    \
    } while (0)

    flags = FLAGS(obj);
    doSelf = 0;
    if ((flags & 0x200) != 0) {
        doSelf = 1;
    }
    if (recurse != 0) {
        doSelf = 1;
    }

    if (doSelf != 0) {
        APPLY_NODE(obj, parentMtx);

        child = CHILD(obj);
        if (child != 0) {
            APPLY_NODE(child, WORLD(obj));
            if (CHILD(child) != 0) {
                mapReCalcMatrix(CHILD(child), WORLD(child), 1);
            }
            if (SIB(child) != 0) {
                mapReCalcMatrix(SIB(child), WORLD(obj), 1);
            }
        }

        child = SIB(obj);
        if (child != 0) {
            if ((recurse == 0) && ((FLAGS(child) & 0x200) == 0)) {
                if (CHILD(child) != 0) {
                    mapReCalcMatrix(CHILD(child), WORLD(child), recurse);
                }
                if (SIB(child) != 0) {
                    mapReCalcMatrix(SIB(child), parentMtx, recurse);
                }
            } else {
                APPLY_NODE(child, parentMtx);
                if (CHILD(child) != 0) {
                    mapReCalcMatrix(CHILD(child), WORLD(child), 1);
                }
                if (SIB(child) != 0) {
                    mapReCalcMatrix(SIB(child), parentMtx, recurse);
                }
            }
        }
    } else {
        child = CHILD(obj);
        if (child != 0) {
            if ((FLAGS(child) & 0x200) == 0) {
                if (CHILD(child) != 0) {
                    mapReCalcMatrix(CHILD(child), WORLD(child), recurse);
                }
                if (SIB(child) != 0) {
                    mapReCalcMatrix(SIB(child), WORLD(obj), recurse);
                }
            } else {
                APPLY_NODE(child, WORLD(obj));
                if (CHILD(child) != 0) {
                    mapReCalcMatrix(CHILD(child), WORLD(child), 1);
                }
                if (SIB(child) != 0) {
                    mapReCalcMatrix(SIB(child), WORLD(obj), recurse);
                }
            }
        }

        child = SIB(obj);
        if (child != 0) {
            if ((FLAGS(child) & 0x200) == 0) {
                if (CHILD(child) != 0) {
                    mapReCalcMatrix(CHILD(child), WORLD(child), recurse);
                }
                if (SIB(child) != 0) {
                    mapReCalcMatrix(SIB(child), parentMtx, recurse);
                }
            } else {
                APPLY_NODE(child, parentMtx);
                if (CHILD(child) != 0) {
                    mapReCalcMatrix(CHILD(child), WORLD(child), 1);
                }
                if (SIB(child) != 0) {
                    mapReCalcMatrix(SIB(child), parentMtx, recurse);
                }
            }
        }
    }

#undef FLAGS
#undef CHILD
#undef SIB
#undef WORLD
#undef LOCAL
#undef ALT
#undef RUNTIME
#undef APPLY_NODE
}

void mapMain(void) {
    extern s32 activeGroup;
    extern s32 mapWork;
    extern s32 gp;
    extern s32 strcmp(const char* a, const char* b);
    extern char* strcpy(char* dst, const char* src);
    extern char* strcat(char* dst, const char* src);
    extern u8 mapCalcAnimMatrix(void* pDstMtx, void* pParentMtx, void* pEntry, void* pTrack);
    extern u16* lightNameToPtr(char* name);
    extern void PSMTXScale(f32 mtx[3][4], f32 x, f32 y, f32 z);
    extern u8 mapReCalcMatrix(void* obj, s32 mtx, int recurse);
    extern char str_v_8041f99c[];
    extern char str_v_x_8041f9a0[];

    f32 zero;
    f32 one;
    f32 two;
    f32 three;
    f32 negTwo;
    f32 ten;
    f32 scale255;
    f32 eps;
    f32 scaleMtx[3][4];
    f32 lightColor[5];
    f32 lightParam[9];
    char nameBuf[256];
    char nameBuf2[256];
    s32 group;
    s32 entryBase;
    s32 entry;
    s32 entryIndex;
    s32 animIndex;
    s32 animCount;
    s32 animEntry;
    s32 flags;
    s32 animData;
    s32 meshTracks;
    s32 materialTracks;
    s32 alphaTracks;
    s32 lightTracks;
    s32 lightColorTracks;
    s32 trackList;
    s32 track;
    s32 trackIndex;
    s32 obj;
    s32 objIndex;
    s32 searchEntry;
    s32 searchEntryIndex;
    s32 matTable;
    s32 matIndex;
    s32 matEntry;
    s32 material;
    s32 dobj;
    s32 keyIndex;
    s32 lastKey;
    s32 keyBase;
    s32 nextKeyBase;
    s32 component;
    s32 outOffset;
    s32 nowHi;
    s32 nowLo;
    s32 timeOff;
    s32 g;
    s32 denom;
    u64 rawTime;
    u64 now;
    u64 old;
    f32 frame;
    f32 duration;
    f32 delta;
    f32 t0;
    f32 t1;
    f32 span;
    f32 den;
    f32 u;
    f32 u2;
    f32 u3;
    f32 value;
    f32 curValue;
    f32 nextValue;
    f32 tan0;
    f32 tan1;
    u16* light;
    s32 changed;

#define W(p,o) (*(s32*)((s32)(p) + (o)))
#define UW(p,o) (*(u32*)((s32)(p) + (o)))
#define H(p,o) (*(u16*)((s32)(p) + (o)))
#define B(p,o) (*(u8*)((s32)(p) + (o)))
#define F(p,o) (*(f32*)((s32)(p) + (o)))
#define P(p,o) (*(void**)((s32)(p) + (o)))

    zero = 0.0f;
    one = 1.0f;
    two = 2.0f;
    three = 3.0f;
    negTwo = -2.0f;
    ten = 10.0f;
    scale255 = 255.0f;
    eps = 0.0001f;

    entryIndex = 0;
    group = mapWork + activeGroup * 0x2F4;
    entryBase = group;

    while (entryIndex < W(entryBase, 0x0)) {
        entry = group;
        changed = 0;
        animEntry = W(entry, 0x164);
        animCount = W(entry, 0x160);

        for (animIndex = 0; animIndex < animCount; animIndex++, animEntry += 0x20) {
            flags = H(animEntry, 0x0);
            animData = W(animEntry, 0x18);
            meshTracks = W(animData, 0x0C);
            materialTracks = W(animData, 0x10);
            alphaTracks = W(animData, 0x14);
            lightTracks = W(animData, 0x18);
            lightColorTracks = W(animData, 0x1C);

            if ((flags & 1) && !(flags & 2)) {
                g = gp;
                if (flags & 0x100) {
                    timeOff = 0x48;
                } else if (flags & 0x200) {
                    timeOff = 0x50;
                } else if (flags & 0x400) {
                    timeOff = 0x58;
                } else if (flags & 0x800) {
                    timeOff = 0x60;
                } else if (W(g, 0x14) != 0) {
                    timeOff = 0x38;
                } else {
                    timeOff = 0x40;
                }
                nowHi = W(g, timeOff);
                nowLo = W(g, timeOff + 4);
                rawTime = ((u64)(u32)nowHi << 32) | (u32)nowLo;
                denom = (*(u32*)0x800000F8) / 4000;
                now = rawTime / (u32)denom;

                if (!(flags & 0x10)) {
                    old = *(u64*)((s32)animEntry + 0x8);
                    delta = (f32)(now - old);
                    frame = F(animEntry, 0x10);
                    delta = ((f32)W(g, 0x4) * delta) / 1000.0f;
                    if (delta < frame) {
                        F(animEntry, 0x10) = frame - (frame - delta);
                    } else {
                        F(animEntry, 0x10) = F(animEntry, 0x14) * (delta - frame) + frame;
                    }
                    *(u64*)((s32)animEntry + 0x8) = now - (u64)(((1000.0f * F(animEntry, 0x10)) / (f32)W(g, 0x4)));
                } else if (flags & 0x20) {
                    *(u64*)((s32)animEntry + 0x8) = now - (u64)(((1000.0f * F(animEntry, 0x10)) / (f32)W(g, 0x4)));
                    H(animEntry, 0x0) &= (u16)~0x20;
                    H(animEntry, 0x0) &= (u16)~0x10;
                }

                frame = F(animEntry, 0x10);
                duration = F(animData, 0x8);
                if (duration <= frame) {
                    if (!(H(animEntry, 0x0) & 4)) {
                        F(animEntry, 0x10) = duration;
                        H(animEntry, 0x0) |= 2;
                    } else {
                        while (duration < frame) {
                            frame -= duration;
                        }
                        F(animEntry, 0x10) = frame;
                        *(u64*)((s32)animEntry + 0x8) = now - (u64)(((1000.0f * F(animEntry, 0x10)) / (f32)W(g, 0x4)));
                    }
                }

                if (meshTracks != 0) {
                    for (trackIndex = 0; trackIndex < W(meshTracks, 0); trackIndex++) {
                        track = W(meshTracks, 4 + trackIndex * 4);
                        obj = 0;
                        if (W(track, 0) != 0) {
                            searchEntry = group;
                            for (searchEntryIndex = 0; searchEntryIndex < W(group, 0) && obj == 0; searchEntryIndex++, searchEntry += 0x178) {
                                dobj = W(searchEntry, 0x154);
                                for (objIndex = 0; objIndex < W(searchEntry, 0x150); objIndex++, dobj += 0x134) {
                                    if (strcmp(*(char**)W(dobj, 0x8), (char*)W(track, 0)) == 0) {
                                        obj = dobj;
                                        break;
                                    }
                                }
                            }
                        }
                        if (obj != 0) {
                            mapCalcAnimMatrix((void*)(obj + 0x7C), (void*)(obj + 0x1C), (void*)animEntry, (void*)track);
                            changed = 1;
                            W(obj, 0x0) |= 0x2000;
                            W(obj, 0x0) |= 0x200;
                        }
                    }
                }

                if (materialTracks != 0) {
                    trackList = materialTracks;
                    for (trackIndex = 0; trackIndex < W(materialTracks, 0); trackIndex++, trackList += 4) {
                        track = W(trackList, 4);
                        strcpy(nameBuf, (char*)W(track, 0));
                        matIndex = 0;
                        searchEntry = group;
                        for (searchEntryIndex = 0; searchEntryIndex < W(group, 0) && matIndex == 0; searchEntryIndex++, searchEntry += 0x178) {
                            matTable = W(searchEntry, 0xA0);
                            if (matTable == 0) {
                                break;
                            }
                            for (objIndex = 0; objIndex < W(matTable, 0); objIndex++) {
                                matEntry = matTable + 4 + objIndex * 8;
                                if (strcmp((char*)W(matEntry, 0), nameBuf) == 0) {
                                    matIndex = W(matEntry, 4);
                                    break;
                                }
                            }
                        }
                        if (matIndex != 0) {
                            lastKey = W(track, 0x10) - 1;
                            keyBase = track;
                            for (keyIndex = 1; keyIndex <= lastKey; keyIndex++, keyBase += 0x68) {
                                nextKeyBase = keyBase + 0x68;
                                frame = F(animEntry, 0x10);
                                if (F(nextKeyBase, 0x14) < frame && keyIndex != lastKey) {
                                    continue;
                                }
                                for (component = 0; component < 5; component++) {
                                    outOffset = W(track, 4) * 0x1C + 0x2C + component * 4;
                                    if (F(keyBase, 0x28 + component * 0x14) == zero) {
                                        t0 = F(keyBase, 0x14);
                                        t1 = F(nextKeyBase, 0x14);
                                        span = t1 - t0;
                                        den = span;
                                        if (span == zero) den = eps;
                                        u = (frame - t0) / den;
                                        u2 = u * u;
                                        u3 = u2 * u;
                                        curValue = F(keyBase, 0x18 + component * 0x14);
                                        nextValue = F(nextKeyBase, 0x18 + component * 0x14);
                                        tan0 = F(keyBase, 0x20 + component * 0x14);
                                        tan1 = F(nextKeyBase, 0x1C + component * 0x14);
                                        value = tan1 * span * (u3 - u2) + nextValue * (negTwo * u3 + three * u2) + curValue * (one + (two * u3 - three * u2)) + tan0 * span * (u - (two * u2 - u3));
                                    } else {
                                        value = F(keyBase, 0x18 + component * 0x14);
                                    }
                                    F(matIndex, outOffset) = value;
                                }
                                W(matIndex, W(track, 4) * 0x1C + 0x40) = W(track, 8);
                                W(matIndex, W(track, 4) * 0x1C + 0x44) = W(track, 0xC);
                                break;
                            }
                        }

                        strcpy(nameBuf, (char*)W(track, 0));
                        strcat(nameBuf, str_v_8041f99c);
                        matIndex = 0;
                        searchEntry = group;
                        for (searchEntryIndex = 0; searchEntryIndex < W(group, 0) && matIndex == 0; searchEntryIndex++, searchEntry += 0x178) {
                            matTable = W(searchEntry, 0xA0);
                            if (matTable == 0) break;
                            for (objIndex = 0; objIndex < W(matTable, 0); objIndex++) {
                                matEntry = matTable + 4 + objIndex * 8;
                                if (strcmp((char*)W(matEntry, 0), nameBuf) == 0) {
                                    matIndex = W(matEntry, 4);
                                    break;
                                }
                            }
                        }
                        if (matIndex != 0) {
                            lastKey = W(track, 0x10) - 1;
                            keyBase = track;
                            for (keyIndex = 1; keyIndex <= lastKey; keyIndex++, keyBase += 0x68) {
                                nextKeyBase = keyBase + 0x68;
                                frame = F(animEntry, 0x10);
                                if (F(nextKeyBase, 0x14) < frame && keyIndex != lastKey) continue;
                                for (component = 0; component < 5; component++) {
                                    outOffset = W(track, 4) * 0x1C + 0x2C + component * 4;
                                    if (F(keyBase, 0x28 + component * 0x14) == zero) {
                                        t0 = F(keyBase, 0x14); t1 = F(nextKeyBase, 0x14); span = t1 - t0; den = span; if (span == zero) den = eps;
                                        u = (frame - t0) / den; u2 = u * u; u3 = u2 * u;
                                        curValue = F(keyBase, 0x18 + component * 0x14); nextValue = F(nextKeyBase, 0x18 + component * 0x14);
                                        tan0 = F(keyBase, 0x20 + component * 0x14); tan1 = F(nextKeyBase, 0x1C + component * 0x14);
                                        value = tan1 * span * (u3 - u2) + nextValue * (negTwo * u3 + three * u2) + curValue * (one + (two * u3 - three * u2)) + tan0 * span * (u - (two * u2 - u3));
                                    } else {
                                        value = F(keyBase, 0x18 + component * 0x14);
                                    }
                                    F(matIndex, outOffset) = value;
                                }
                                W(matIndex, W(track, 4) * 0x1C + 0x40) = W(track, 8);
                                W(matIndex, W(track, 4) * 0x1C + 0x44) = W(track, 0xC);
                                break;
                            }
                        }

                        strcpy(nameBuf, (char*)W(track, 0));
                        strcat(nameBuf, str_v_x_8041f9a0);
                        matIndex = 0;
                        searchEntry = group;
                        for (searchEntryIndex = 0; searchEntryIndex < W(group, 0) && matIndex == 0; searchEntryIndex++, searchEntry += 0x178) {
                            matTable = W(searchEntry, 0xA0);
                            if (matTable == 0) break;
                            for (objIndex = 0; objIndex < W(matTable, 0); objIndex++) {
                                matEntry = matTable + 4 + objIndex * 8;
                                if (strcmp((char*)W(matEntry, 0), nameBuf) == 0) {
                                    matIndex = W(matEntry, 4);
                                    break;
                                }
                            }
                        }
                        if (matIndex != 0) {
                            lastKey = W(track, 0x10) - 1;
                            keyBase = track;
                            for (keyIndex = 1; keyIndex <= lastKey; keyIndex++, keyBase += 0x68) {
                                nextKeyBase = keyBase + 0x68;
                                frame = F(animEntry, 0x10);
                                if (F(nextKeyBase, 0x14) < frame && keyIndex != lastKey) continue;
                                for (component = 0; component < 5; component++) {
                                    outOffset = W(track, 4) * 0x1C + 0x2C + component * 4;
                                    if (F(keyBase, 0x28 + component * 0x14) == zero) {
                                        t0 = F(keyBase, 0x14); t1 = F(nextKeyBase, 0x14); span = t1 - t0; den = span; if (span == zero) den = eps;
                                        u = (frame - t0) / den; u2 = u * u; u3 = u2 * u;
                                        curValue = F(keyBase, 0x18 + component * 0x14); nextValue = F(nextKeyBase, 0x18 + component * 0x14);
                                        tan0 = F(keyBase, 0x20 + component * 0x14); tan1 = F(nextKeyBase, 0x1C + component * 0x14);
                                        value = tan1 * span * (u3 - u2) + nextValue * (negTwo * u3 + three * u2) + curValue * (one + (two * u3 - three * u2)) + tan0 * span * (u - (two * u2 - u3));
                                    } else {
                                        value = F(keyBase, 0x18 + component * 0x14);
                                    }
                                    F(matIndex, outOffset) = value;
                                }
                                W(matIndex, W(track, 4) * 0x1C + 0x40) = W(track, 8);
                                W(matIndex, W(track, 4) * 0x1C + 0x44) = W(track, 0xC);
                                break;
                            }
                        }
                    }
                }

                if (alphaTracks != 0) {
                    trackList = alphaTracks;
                    for (trackIndex = 0; trackIndex < W(alphaTracks, 0); trackIndex++, trackList += 4) {
                        track = W(trackList, 4);
                        strcpy(nameBuf2, (char*)W(track, 0));
                        matIndex = 0;
                        searchEntry = group;
                        for (searchEntryIndex = 0; searchEntryIndex < W(group, 0) && matIndex == 0; searchEntryIndex++, searchEntry += 0x178) {
                            matTable = W(searchEntry, 0xA0);
                            if (matTable == 0) break;
                            for (objIndex = 0; objIndex < W(matTable, 0); objIndex++) {
                                matEntry = matTable + 4 + objIndex * 8;
                                if (strcmp((char*)W(matEntry, 0), nameBuf2) == 0) { matIndex = W(matEntry, 4); break; }
                            }
                        }
                        if (matIndex != 0) {
                            lastKey = W(track, 4) - 1;
                            keyBase = track;
                            for (keyIndex = 1; keyIndex <= lastKey; keyIndex++, keyBase += 0x54) {
                                nextKeyBase = keyBase + 0x54;
                                frame = F(animEntry, 0x10);
                                if (F(nextKeyBase, 8) < frame && keyIndex != lastKey) continue;
                                if (F(keyBase, 0x1C) == zero) {
                                    t0 = F(keyBase, 8); t1 = F(nextKeyBase, 8); span = t1 - t0; den = span; if (span == zero) den = eps;
                                    u = (frame - t0) / den; u2 = u * u; u3 = u2 * u;
                                    curValue = F(keyBase, 0x0C); nextValue = F(nextKeyBase, 0x0C);
                                    tan0 = F(keyBase, 0x14); tan1 = F(nextKeyBase, 0x10);
                                    value = tan1 * span * (u3 - u2) + nextValue * (negTwo * u3 + three * u2) + curValue * (one + (two * u3 - three * u2)) + tan0 * span * (u - (two * u2 - u3));
                                } else {
                                    value = F(keyBase, 0x0C);
                                }
                                B(matIndex, 0x10C) = (u8)(s32)(-(scale255 * value - scale255));
                                break;
                            }
                        }

                        strcpy(nameBuf2, (char*)W(track, 0));
                        strcat(nameBuf2, str_v_8041f99c);
                        matIndex = 0;
                        searchEntry = group;
                        for (searchEntryIndex = 0; searchEntryIndex < W(group, 0) && matIndex == 0; searchEntryIndex++, searchEntry += 0x178) {
                            matTable = W(searchEntry, 0xA0);
                            if (matTable == 0) break;
                            for (objIndex = 0; objIndex < W(matTable, 0); objIndex++) {
                                matEntry = matTable + 4 + objIndex * 8;
                                if (strcmp((char*)W(matEntry, 0), nameBuf2) == 0) { matIndex = W(matEntry, 4); break; }
                            }
                        }
                        if (matIndex != 0) {
                            lastKey = W(track, 4) - 1;
                            keyBase = track;
                            for (keyIndex = 1; keyIndex <= lastKey; keyIndex++, keyBase += 0x54) {
                                nextKeyBase = keyBase + 0x54; frame = F(animEntry, 0x10);
                                if (F(nextKeyBase, 8) < frame && keyIndex != lastKey) continue;
                                if (F(keyBase, 0x1C) == zero) {
                                    t0 = F(keyBase, 8); t1 = F(nextKeyBase, 8); span = t1 - t0; den = span; if (span == zero) den = eps;
                                    u = (frame - t0) / den; u2 = u * u; u3 = u2 * u;
                                    curValue = F(keyBase, 0x0C); nextValue = F(nextKeyBase, 0x0C); tan0 = F(keyBase, 0x14); tan1 = F(nextKeyBase, 0x10);
                                    value = tan1 * span * (u3 - u2) + nextValue * (negTwo * u3 + three * u2) + curValue * (one + (two * u3 - three * u2)) + tan0 * span * (u - (two * u2 - u3));
                                } else { value = F(keyBase, 0x0C); }
                                B(matIndex, 0x10C) = (u8)(s32)(-(scale255 * value - scale255));
                                break;
                            }
                        }

                        strcpy(nameBuf2, (char*)W(track, 0));
                        strcat(nameBuf2, str_v_x_8041f9a0);
                        matIndex = 0;
                        searchEntry = group;
                        for (searchEntryIndex = 0; searchEntryIndex < W(group, 0) && matIndex == 0; searchEntryIndex++, searchEntry += 0x178) {
                            matTable = W(searchEntry, 0xA0);
                            if (matTable == 0) break;
                            for (objIndex = 0; objIndex < W(matTable, 0); objIndex++) {
                                matEntry = matTable + 4 + objIndex * 8;
                                if (strcmp((char*)W(matEntry, 0), nameBuf2) == 0) { matIndex = W(matEntry, 4); break; }
                            }
                        }
                        if (matIndex != 0) {
                            lastKey = W(track, 4) - 1;
                            keyBase = track;
                            for (keyIndex = 1; keyIndex <= lastKey; keyIndex++, keyBase += 0x54) {
                                nextKeyBase = keyBase + 0x54; frame = F(animEntry, 0x10);
                                if (F(nextKeyBase, 8) < frame && keyIndex != lastKey) continue;
                                if (F(keyBase, 0x1C) == zero) {
                                    t0 = F(keyBase, 8); t1 = F(nextKeyBase, 8); span = t1 - t0; den = span; if (span == zero) den = eps;
                                    u = (frame - t0) / den; u2 = u * u; u3 = u2 * u;
                                    curValue = F(keyBase, 0x0C); nextValue = F(nextKeyBase, 0x0C); tan0 = F(keyBase, 0x14); tan1 = F(nextKeyBase, 0x10);
                                    value = tan1 * span * (u3 - u2) + nextValue * (negTwo * u3 + three * u2) + curValue * (one + (two * u3 - three * u2)) + tan0 * span * (u - (two * u2 - u3));
                                } else { value = F(keyBase, 0x0C); }
                                B(matIndex, 0x10C) = (u8)(s32)(-(scale255 * value - scale255));
                                break;
                            }
                        }
                    }
                }

                if (lightTracks != 0) {
                    trackList = lightTracks;
                    for (trackIndex = 0; trackIndex < W(lightTracks, 0); trackIndex++, trackList += 4) {
                        track = W(trackList, 4);
                        light = lightNameToPtr((char*)W(track, 0));
                        if (light != 0) {
                            lastKey = W(track, 4) - 1;
                            keyBase = track;
                            for (keyIndex = 1; keyIndex <= lastKey; keyIndex++, keyBase += 0xB8) {
                                nextKeyBase = keyBase + 0xB8;
                                frame = F(animEntry, 0x10);
                                if (F(nextKeyBase, 8) < frame && keyIndex != lastKey) continue;
                                for (component = 0; component < 9; component++) {
                                    if (F(keyBase, 0x1C + component * 0x14) == zero) {
                                        t0 = F(keyBase, 8); t1 = F(nextKeyBase, 8); span = t1 - t0; den = span; if (span == zero) den = eps;
                                        u = (frame - t0) / den; u2 = u * u; u3 = u2 * u;
                                        curValue = F(keyBase, 0x0C + component * 0x14); nextValue = F(nextKeyBase, 0x0C + component * 0x14);
                                        tan0 = F(keyBase, 0x14 + component * 0x14); tan1 = F(nextKeyBase, 0x10 + component * 0x14);
                                        lightParam[component] = tan1 * span * (u3 - u2) + nextValue * (negTwo * u3 + three * u2) + curValue * (one + (two * u3 - three * u2)) + tan0 * span * (u - (two * u2 - u3));
                                    } else {
                                        lightParam[component] = F(keyBase, 0x0C + component * 0x14);
                                    }
                                }
                                *(f32*)((s32)light + 0x24) = ten * lightParam[0];
                                *(f32*)((s32)light + 0x28) = ten * lightParam[1];
                                *(f32*)((s32)light + 0x2C) = ten * lightParam[2];
                                *(f32*)((s32)light + 0x30) = lightParam[3];
                                *(f32*)((s32)light + 0x34) = lightParam[4];
                                *(f32*)((s32)light + 0x38) = lightParam[5];
                                break;
                            }
                        }
                    }
                }

                if (lightColorTracks != 0) {
                    trackList = lightColorTracks;
                    for (trackIndex = 0; trackIndex < W(lightColorTracks, 0); trackIndex++, trackList += 4) {
                        track = W(trackList, 4);
                        light = lightNameToPtr((char*)W(track, 0));
                        if (light != 0) {
                            lastKey = W(track, 4) - 1;
                            keyBase = track;
                            for (keyIndex = 1; keyIndex <= lastKey; keyIndex++, keyBase += 0x68) {
                                nextKeyBase = keyBase + 0x68;
                                frame = F(animEntry, 0x10);
                                if (F(nextKeyBase, 8) < frame && keyIndex != lastKey) continue;
                                for (component = 0; component < 5; component++) {
                                    if (F(keyBase, 0x1C + component * 0x14) == zero) {
                                        t0 = F(keyBase, 8); t1 = F(nextKeyBase, 8); span = t1 - t0; den = span; if (span == zero) den = eps;
                                        u = (frame - t0) / den; u2 = u * u; u3 = u2 * u;
                                        curValue = F(keyBase, 0x0C + component * 0x14); nextValue = F(nextKeyBase, 0x0C + component * 0x14);
                                        tan0 = F(keyBase, 0x14 + component * 0x14); tan1 = F(nextKeyBase, 0x10 + component * 0x14);
                                        lightColor[component] = tan1 * span * (u3 - u2) + nextValue * (negTwo * u3 + three * u2) + curValue * (one + (two * u3 - three * u2)) + tan0 * span * (u - (two * u2 - u3));
                                    } else {
                                        lightColor[component] = F(keyBase, 0x0C + component * 0x14);
                                    }
                                }
                                B(light, 0x48) = (u8)(s32)(scale255 * lightColor[0]);
                                B(light, 0x49) = (u8)(s32)(scale255 * lightColor[1]);
                                B(light, 0x4A) = (u8)(s32)(scale255 * lightColor[2]);
                                B(light, 0x4B) = 0xFF;
                                *(f32*)((s32)light + 0x50) = lightColor[3];
                                break;
                            }
                        }
                    }
                }
            }
        }

        if (changed != 0) {
            PSMTXScale(scaleMtx, zero, zero, zero);
            mapReCalcMatrix((void*)W(group, 0xA8), (s32)scaleMtx, 0);
        }
        group += 0x178;
        entryIndex++;
    }

#undef W
#undef UW
#undef H
#undef B
#undef F
#undef P
}

void _mapDispMapObj(s32 cameraId, void* mapObj) {
    typedef f32 Mtx[3][4];
    typedef struct Color {
        u8 r;
        u8 g;
        u8 b;
        u8 a;
    } Color;
    extern void* current_mp;
    extern void* camGetPtr(s32 id);
    extern void* camGetCurPtr(void);
    extern void PSMTXConcat(void*, void*, void*);
    extern void PSMTXInvXpose(void*, void*);
    extern void GXLoadPosMtxImm(void*, s32);
    extern void GXLoadNrmMtxImm(void*, s32);
    extern void GXSetCurrentMtx(s32);
    extern void GXSetBlendMode(s32, s32, s32, s32);
    extern void GXSetZCompLoc(s32);
    extern void GXSetAlphaCompare(s32, s32, s32, s32, s32);
    extern void GXSetZMode(s32, s32, s32);
    extern void GXGetCullMode(s32*);
    extern void GXSetCullMode(s32);
    extern s32 activeGroup;
    extern s32 mapWork;
    extern u32 fog_type[];
    extern u32 dat_8041f904;
    extern f32 float_0_8041f930;
    extern void GXSetFog(s32, f32, f32, f32, f32, void*);
    extern void mapSetTextureMatrix(void*);
    extern u32 dat_8041f8fc;
    extern void GXSetTevKColor(s32, void*);
    extern void mapSetMaterial(void*, void*);
    extern void mapSetPolygon(s32, s32);
    extern void mapSetPolygonVtxDesc(s32, s32);
    extern void GXCallDisplayList(void*, u32);
    extern void GXSetTevDirect(s32);
    extern void GXSetNumIndStages(s32);
    extern u32 culling[];
    u8* obj;
    u8* camera;
    u8* currentCamera;
    u8* joint;
    u8* drawMode;
    void* material;
    u8* mesh;
    u32 flags;
    u8 blendMode;
    s32 desiredCull;
    s32 currentCull;
    s32 count;
    s32 i;
    s32 j;
    s32 group;
    u16 groupFlags;
    u32 fogColor;
    Color tevColor;
    Color outputColor;
    Color* objectColor;
    Color* materialColor;
    Color* blendColor;
    Mtx normal;
    Mtx model;

    obj = mapObj;
    camera = camGetPtr(cameraId);
    current_mp = mapObj;
    flags = *(u32*)obj;
    joint = *(u8**)(obj + 8);
    group = mapWork + activeGroup * 0x2F4;
    groupFlags = *(u16*)(group + 4);

    if ((flags & 0x20) != 0) {
        blendMode = obj[4];
        if (*(s32*)(joint + 0x5C) > 0) {
            material = *(void**)(joint + 0x60);
            if (((flags & 0x40) != 0 && obj[0x0F] != 0xFF)
                || *(s8*)((u8*)material + 0x10C) != -1
                || (((flags & 0x800) == 0) && ((groupFlags & 2) != 0)
                    && *(u8*)(group + 0x19) != 0xFF)
                || (((flags & 0x1000) == 0) && ((groupFlags & 4) != 0)
                    && *(u8*)(group + 0x1D) != 0xFF)) {
                blendMode = 2;
            }
        }

        switch (blendMode) {
            case 0:
                GXSetBlendMode(0, 1, 0, 7);
                GXSetZCompLoc(1);
                GXSetAlphaCompare(7, 0, 0, 7, 0);
                GXSetZMode(1, 3, 1);
                break;
            case 1:
                GXSetBlendMode(0, 1, 0, 0);
                GXSetZCompLoc(0);
                GXSetAlphaCompare(6, 0x80, 1, 0, 0);
                GXSetZMode(1, 3, 1);
                break;
            case 3:
                GXSetBlendMode(0, 1, 0, 0);
                GXSetZCompLoc(1);
                GXSetAlphaCompare(7, 0, 0, 7, 0);
                GXSetZMode(0, 3, 0);
                break;
            case 4:
                GXSetBlendMode(0, 1, 0, 0);
                GXSetZCompLoc(0);
                GXSetAlphaCompare(6, 0x80, 1, 0, 0);
                GXSetZMode(0, 3, 0);
                break;
            default:
                GXSetBlendMode(1, 4, 5, 0);
                GXSetZCompLoc(1);
                GXSetAlphaCompare(7, 0, 0, 7, 0);
                GXSetZMode(1, 3, 0);
                break;
        }
    }

    if (cameraId == 1) {
        GXSetZCompLoc(0);
    }

    PSMTXConcat(camera + 0x11C, obj + 0x1C, model);
    GXLoadPosMtxImm(model, 0);
    if ((flags & 0x100) == 0) {
        PSMTXInvXpose(model, normal);
        GXLoadNrmMtxImm(normal, 0);
    }
    GXSetCurrentMtx(0);

    drawMode = *(u8**)(joint + 0x58);
    desiredCull = culling[drawMode[1]];
    GXGetCullMode(&currentCull);
    if (currentCull != desiredCull) {
        GXSetCullMode(desiredCull);
    }
    currentCamera = camGetCurPtr();
    if ((groupFlags & 1) == 0 || (flags & 0x80000) != 0) {
        fogColor = dat_8041f904;
        GXSetFog(
            0,
            float_0_8041f930,
            float_0_8041f930,
            float_0_8041f930,
            float_0_8041f930,
            &fogColor
        );
    } else {
        GXSetFog(
            fog_type[*(s32*)(group + 0x28)],
            *(f32*)(group + 0x2C),
            *(f32*)(group + 0x30),
            *(f32*)(currentCamera + 0x30),
            *(f32*)(currentCamera + 0x34),
            (void*)(group + 0x34)
        );
    }

    if ((flags & 0x20) == 0
        || ((flags & 0x4000) == 0
            && ((flags & 1) == 0 || (flags & 0x80) != 0)
            && ((flags & 0x40) == 0 || obj[0x0F] != 0))) {
        count = *(s32*)(joint + 0x5C);
        for (i = 0; i < count; i++) {
            material = *(void**)(joint + 0x60 + i * 8);
            mesh = *(u8**)(joint + 0x64 + i * 8);
            mapSetTextureMatrix(material);
            *(u32*)&tevColor = dat_8041f8fc;
            objectColor = (Color*)(obj + 0x0C);
            materialColor = (Color*)((u8*)material + 0x10C);
            if ((flags & 0x40) != 0) {
                tevColor.r = (tevColor.r * objectColor->r) / 0xFF;
                tevColor.g = (tevColor.g * objectColor->g) / 0xFF;
                tevColor.b = (tevColor.b * objectColor->b) / 0xFF;
                tevColor.a = (tevColor.a * objectColor->a) / 0xFF;
            }
            if (materialColor->r != 0xFF) {
                tevColor.a = (tevColor.a * materialColor->r) / 0xFF;
            }
            if (((groupFlags & 2) != 0) && ((flags & 0x800) == 0)) {
                blendColor = (Color*)(group + 0x16);
                tevColor.r = (tevColor.r * blendColor->r) / 0xFF;
                tevColor.g = (tevColor.g * blendColor->g) / 0xFF;
                tevColor.b = (tevColor.b * blendColor->b) / 0xFF;
                tevColor.a = (tevColor.a * blendColor->a) / 0xFF;
            }
            if (((groupFlags & 4) != 0) && ((flags & 0x1000) == 0)) {
                blendColor = (Color*)(group + 0x1A);
                tevColor.r = (tevColor.r * blendColor->r) / 0xFF;
                tevColor.g = (tevColor.g * blendColor->g) / 0xFF;
                tevColor.b = (tevColor.b * blendColor->b) / 0xFF;
                tevColor.a = (tevColor.a * blendColor->a) / 0xFF;
            }
            if (((groupFlags & 8) != 0) && ((flags & 0x800) == 0)) {
                blendColor = (Color*)(group + 0x1E);
                tevColor.r = (tevColor.r * blendColor->r) / 0xFF;
                tevColor.g = (tevColor.g * blendColor->g) / 0xFF;
                tevColor.b = (tevColor.b * blendColor->b) / 0xFF;
                tevColor.a = (tevColor.a * blendColor->a) / 0xFF;
            }
            outputColor = tevColor;
            GXSetTevKColor(0, &outputColor);

            if ((flags & 0x100) == 0) {
                mapSetMaterial(obj, material);
                mapSetPolygon((s32)obj, (s32)mesh);
            } else {
                mapSetPolygonVtxDesc((s32)obj, (s32)mesh);
                GXCallDisplayList(*(void**)(obj + 0xF4), *(u32*)(obj + 0x114));
                for (j = 0; j < *(s32*)(mesh + 4); j++) {
                    GXCallDisplayList(
                        *(void**)(mesh + 0x10 + j * 8),
                        *(u32*)(mesh + 0x14 + j * 8)
                    );
                }
            }
        }
    }

    if ((flags & 0x0F000000) != 0) {
        GXSetTevDirect(0);
        GXSetTevDirect(1);
        GXSetTevDirect(2);
        GXSetTevDirect(3);
        GXSetTevDirect(4);
        GXSetNumIndStages(0);
    }
}

void _mapDispMapGrp(s32 cam, void* obj) {
    extern void _mapDispMapObj(s32 cam, void* obj);

    void* p1;
    void* p2;
    void* p3;

    _mapDispMapObj(cam, obj);

    p1 = *(void**)((s32)obj + 0xE4);
    if (p1 != 0) {
        _mapDispMapObj(cam, p1);

        p2 = *(void**)((s32)p1 + 0xE4);
        if (p2 != 0) {
            _mapDispMapObj(cam, p2);

            p3 = *(void**)((s32)p2 + 0xE4);
            if (p3 != 0) {
                _mapDispMapObj(cam, p3);

                if (*(void**)((s32)p3 + 0xE4) != 0) {
                    _mapDispMapGrp(cam, *(void**)((s32)p3 + 0xE4));
                }

                if ((*(u32*)p3 & 0x20) != 0) {
                    if (*(void**)((s32)p3 + 0xE8) != 0) {
                        _mapDispMapGrp(cam, *(void**)((s32)p3 + 0xE8));
                    }
                }
            }

            if ((*(u32*)p2 & 0x20) != 0) {
                p3 = *(void**)((s32)p2 + 0xE8);
                if (p3 != 0) {
                    _mapDispMapObj(cam, p3);

                    if (*(void**)((s32)p3 + 0xE4) != 0) {
                        _mapDispMapGrp(cam, *(void**)((s32)p3 + 0xE4));
                    }

                    if ((*(u32*)p3 & 0x20) != 0) {
                        if (*(void**)((s32)p3 + 0xE8) != 0) {
                            _mapDispMapGrp(cam, *(void**)((s32)p3 + 0xE8));
                        }
                    }
                }
            }
        }

        if ((*(u32*)p1 & 0x20) != 0) {
            p1 = *(void**)((s32)p1 + 0xE8);
            if (p1 != 0) {
                _mapDispMapObj(cam, p1);

                p2 = *(void**)((s32)p1 + 0xE4);
                if (p2 != 0) {
                    _mapDispMapObj(cam, p2);

                    if (*(void**)((s32)p2 + 0xE4) != 0) {
                        _mapDispMapGrp(cam, *(void**)((s32)p2 + 0xE4));
                    }

                    if ((*(u32*)p2 & 0x20) != 0) {
                        if (*(void**)((s32)p2 + 0xE8) != 0) {
                            _mapDispMapGrp(cam, *(void**)((s32)p2 + 0xE8));
                        }
                    }
                }

                if ((*(u32*)p1 & 0x20) != 0) {
                    p1 = *(void**)((s32)p1 + 0xE8);
                    if (p1 != 0) {
                        _mapDispMapObj(cam, p1);

                        if (*(void**)((s32)p1 + 0xE4) != 0) {
                            _mapDispMapGrp(cam, *(void**)((s32)p1 + 0xE4));
                        }

                        if ((*(u32*)p1 & 0x20) != 0) {
                            if (*(void**)((s32)p1 + 0xE8) != 0) {
                                _mapDispMapGrp(cam, *(void**)((s32)p1 + 0xE8));
                            }
                        }
                    }
                }
            }
        }
    }

    if ((*(u32*)obj & 0x20) != 0) {
        p1 = *(void**)((s32)obj + 0xE8);
        if (p1 != 0) {
            _mapDispMapObj(cam, p1);

            p2 = *(void**)((s32)p1 + 0xE4);
            if (p2 != 0) {
                _mapDispMapObj(cam, p2);

                p3 = *(void**)((s32)p2 + 0xE4);
                if (p3 != 0) {
                    _mapDispMapObj(cam, p3);

                    if (*(void**)((s32)p3 + 0xE4) != 0) {
                        _mapDispMapGrp(cam, *(void**)((s32)p3 + 0xE4));
                    }

                    if ((*(u32*)p3 & 0x20) != 0) {
                        if (*(void**)((s32)p3 + 0xE8) != 0) {
                            _mapDispMapGrp(cam, *(void**)((s32)p3 + 0xE8));
                        }
                    }
                }

                if ((*(u32*)p2 & 0x20) != 0) {
                    p2 = *(void**)((s32)p2 + 0xE8);
                    if (p2 != 0) {
                        _mapDispMapObj(cam, p2);

                        if (*(void**)((s32)p2 + 0xE4) != 0) {
                            _mapDispMapGrp(cam, *(void**)((s32)p2 + 0xE4));
                        }

                        if ((*(u32*)p2 & 0x20) != 0) {
                            if (*(void**)((s32)p2 + 0xE8) != 0) {
                                _mapDispMapGrp(cam, *(void**)((s32)p2 + 0xE8));
                            }
                        }
                    }
                }
            }

            if ((*(u32*)p1 & 0x20) != 0) {
                p1 = *(void**)((s32)p1 + 0xE8);
                if (p1 != 0) {
                    _mapDispMapObj(cam, p1);

                    p2 = *(void**)((s32)p1 + 0xE4);
                    if (p2 != 0) {
                        _mapDispMapObj(cam, p2);

                        if (*(void**)((s32)p2 + 0xE4) != 0) {
                            _mapDispMapGrp(cam, *(void**)((s32)p2 + 0xE4));
                        }

                        if ((*(u32*)p2 & 0x20) != 0) {
                            if (*(void**)((s32)p2 + 0xE8) != 0) {
                                _mapDispMapGrp(cam, *(void**)((s32)p2 + 0xE8));
                            }
                        }
                    }

                    if ((*(u32*)p1 & 0x20) != 0) {
                        p1 = *(void**)((s32)p1 + 0xE8);
                        if (p1 != 0) {
                            _mapDispMapObj(cam, p1);

                            if (*(void**)((s32)p1 + 0xE4) != 0) {
                                _mapDispMapGrp(cam, *(void**)((s32)p1 + 0xE4));
                            }

                            if ((*(u32*)p1 & 0x20) != 0) {
                                if (*(void**)((s32)p1 + 0xE8) != 0) {
                                    _mapDispMapGrp(cam, *(void**)((s32)p1 + 0xE8));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void mapDispMapObj(void) {
    extern void _mapDispMapObj(void);
    _mapDispMapObj();
}

void mapDispMapGrp(void) {
    extern void _mapDispMapGrp(void);
    _mapDispMapGrp();
}

void mapDispMapObj_off(s32 group, void* obj) {
    extern void sysWaitDrawSync(void);
    extern void GXClearBoundingBox(void);
    extern void _mapDispMapObj(s32, void*);
    extern void GXReadBoundingBox(void*, void*, void*, void*);
    extern void offscreenAddBoundingBox(s32, u16, u16, u16, u16);
    u16 left;
    u16 top;
    u16 right;
    u16 bottom;
    sysWaitDrawSync();
    GXClearBoundingBox();
    _mapDispMapObj(group, obj);
    sysWaitDrawSync();
    GXReadBoundingBox(&left, &top, &right, &bottom);
    offscreenAddBoundingBox(*(s16*)((s32)obj + 0xDC), left, top, right, bottom);
}

void mapDispMapGrp_off(s32 group, void* obj) {
    extern void sysWaitDrawSync(void);
    extern void GXClearBoundingBox(void);
    extern void _mapDispMapGrp(s32, void*);
    extern void GXReadBoundingBox(void*, void*, void*, void*);
    extern void offscreenAddBoundingBox(s32, u16, u16, u16, u16);
    u16 left;
    u16 top;
    u16 right;
    u16 bottom;
    sysWaitDrawSync();
    GXClearBoundingBox();
    _mapDispMapGrp(group, obj);
    sysWaitDrawSync();
    GXReadBoundingBox(&left, &top, &right, &bottom);
    offscreenAddBoundingBox(*(s16*)((s32)obj + 0xDC), left, top, right, bottom);
}


u8 test_kururing_mapdisp(s32 cam) {
    extern s32 activeGroup;
    extern s32 mapWork;
    extern void _mapDispMapObj(s32 cam, void* obj);

    void* work;
    void* group;
    void* obj;
    s32 groupCount;
    s32 objCount;
    s32 i;
    s32 j;
    u32 flags;

    work = (void*)(mapWork + activeGroup * 0x2F4);
    group = work;
    groupCount = *(s32*)work;

    for (i = 0; i < groupCount; i++) {
        obj = *(void**)((s32)group + 0x154);
        objCount = *(s32*)((s32)group + 0x150);

        for (j = 0; j < objCount; j++) {
            flags = *(u32*)obj;
            if ((flags & 0x400000) != 0) {
                *(u32*)obj = flags & 0xFFFFFFFD;
                _mapDispMapObj(cam, obj);
                *(u32*)obj = flags;
            }

            obj = (void*)((s32)obj + 0x134);
        }

        group = (void*)((s32)group + 0x178);
    }
}

void _mapDispMapObj_NoMaterial(s32 cameraId, void* mapObj) {
    typedef f32 Mtx[3][4];
    extern void* camGetPtr(s32 cameraId);
    extern void PSMTXConcat(void* a, void* b, void* out);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void PSMTXInvXpose(void* src, void* dst);
    extern void GXLoadNrmMtxImm(void* mtx, s32 id);
    extern void GXSetCurrentMtx(s32 id);
    extern void GXSetCullMode(s32 mode);
    extern void GXSetNumChans(s32 count);
    extern void GXSetChanMatColor(s32 chan, void* color);
    extern void GXSetChanCtrl(s32, s32, s32, s32, s32, s32, s32);
    extern void GXSetNumTexGens(s32 count);
    extern void GXSetNumTevStages(s32 count);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevOp(s32, s32);
    extern void mapSetPolygon(s32 obj, s32 mesh);
    extern u32 unk_80429524;
    u8* obj;
    void* camera;
    s32 joint;
    s32 i;
    s32 offset;
    u32 color;
    Mtx normal;
    Mtx model;

    obj = mapObj;
    camera = camGetPtr(cameraId);
    PSMTXConcat((u8*)camera + 0x11C, obj + 0x1C, model);
    GXLoadPosMtxImm(model, 0);
    PSMTXInvXpose(model, normal);
    GXLoadNrmMtxImm(normal, 0);
    GXSetCurrentMtx(0);
    GXSetCullMode(3);
    GXSetNumChans(1);
    color = unk_80429524;
    GXSetChanMatColor(4, &color);
    GXSetChanCtrl(0, 0, 0, 0, 0, 0, 2);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0xFF, 0xFF, 0);
    GXSetTevOp(0, 4);

    joint = *(s32*)(obj + 8);
    offset = 0;
    for (i = 0; i < *(s32*)(joint + 0x5C); i++) {
        mapSetPolygon((s32)obj, *(s32*)(joint + offset + 0x64));
        offset += 8;
    }
}

void _mapDispMapGrp_NoMaterial(s32 camId, void* grp) {
    extern void* camGetPtr(s32 id);
    extern void PSMTXConcat(void* a, void* b, void* out);
    extern void PSMTXInvXpose(void* src, void* dst);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXLoadNrmMtxImm(void* mtx, s32 id);
    extern void GXSetCurrentMtx(s32 id);
    extern void GXSetCullMode(s32 mode);
    extern void GXSetNumChans(s32 count);
    extern void GXSetChanMatColor(s32 chan, void* color);
    extern void GXSetChanCtrl(s32 chan, s32 enable, s32 ambSrc, s32 matSrc, s32 lightMask, s32 diffFn, s32 attnFn);
    extern void GXSetNumTexGens(s32 count);
    extern void GXSetNumTevStages(s32 count);
    extern void GXSetTevOrder(s32 stage, s32 texcoord, s32 texmap, s32 color);
    extern void GXSetTevOp(s32 stage, s32 mode);
    extern u8 mapSetPolygon(int grp, int polygon);
    extern void _mapDispMapObj_NoMaterial(s32 camId, void* obj);
    extern u32 unk_80429524;

    void* cam;
    void* child;
    void* next;
    void* obj;
    s32 i;
    s32 offset;
    u32 color0;
    u32 color1;
    u32 color2;
    f32 nrm0[3][4];
    f32 mtx0[3][4];
    f32 nrm1[3][4];
    f32 mtx1[3][4];
    f32 nrm2[3][4];
    f32 mtx2[3][4];

#define SETUP_NOMAT(node_, mtx_, nrm_, color_)                                      \
    do {                                                                            \
        cam = camGetPtr(camId);                                                     \
        PSMTXConcat((void*)((s32)cam + 0x11C), (void*)((s32)(node_) + 0x1C), (mtx_)); \
        GXLoadPosMtxImm((mtx_), 0);                                                 \
        PSMTXInvXpose((mtx_), (nrm_));                                              \
        GXLoadNrmMtxImm((nrm_), 0);                                                 \
        GXSetCurrentMtx(0);                                                         \
        GXSetCullMode(3);                                                           \
        GXSetNumChans(1);                                                           \
        (color_) = unk_80429524;                                                    \
        GXSetChanMatColor(4, &(color_));                                            \
        GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2);                                         \
        GXSetNumTexGens(0);                                                         \
        GXSetNumTevStages(1);                                                       \
        GXSetTevOrder(0, 0xFF, 0xFF, 4);                                            \
        GXSetTevOp(0, 4);                                                           \
        offset = 0;                                                                 \
        i = 0;                                                                      \
        while (i < *(s32*)(*(s32*)((s32)(node_) + 0x08) + 0x5C)) {                  \
            mapSetPolygon((int)(node_), *(int*)(*(s32*)((s32)(node_) + 0x08) + offset + 0x64)); \
            offset += 8;                                                            \
            i++;                                                                    \
        }                                                                           \
    } while (0)

#define DRAW_OBJ_AND_LINKS(obj_)                                                    \
    do {                                                                            \
        _mapDispMapObj_NoMaterial(camId, (obj_));                                   \
        next = *(void**)((s32)(obj_) + 0xE4);                                       \
        if (next != 0) {                                                            \
            _mapDispMapGrp_NoMaterial(camId, next);                                 \
        }                                                                           \
        if ((*(u32*)(obj_) & 0x20) != 0) {                                          \
            next = *(void**)((s32)(obj_) + 0xE8);                                   \
            if (next != 0) {                                                        \
                _mapDispMapGrp_NoMaterial(camId, next);                             \
            }                                                                       \
        }                                                                           \
    } while (0)

    SETUP_NOMAT(grp, mtx0, nrm0, color0);

    child = *(void**)((s32)grp + 0xE4);
    if (child != 0) {
        SETUP_NOMAT(child, mtx1, nrm1, color1);

        obj = *(void**)((s32)child + 0xE4);
        if (obj != 0) {
            DRAW_OBJ_AND_LINKS(obj);
        }

        if ((*(u32*)child & 0x20) != 0) {
            obj = *(void**)((s32)child + 0xE8);
            if (obj != 0) {
                DRAW_OBJ_AND_LINKS(obj);
            }
        }
    }

    if ((*(u32*)grp & 0x20) != 0) {
        child = *(void**)((s32)grp + 0xE8);
        if (child != 0) {
            SETUP_NOMAT(child, mtx2, nrm2, color2);

            obj = *(void**)((s32)child + 0xE4);
            if (obj != 0) {
                DRAW_OBJ_AND_LINKS(obj);
            }

            if ((*(u32*)child & 0x20) != 0) {
                obj = *(void**)((s32)child + 0xE8);
                if (obj != 0) {
                    DRAW_OBJ_AND_LINKS(obj);
                }
            }
        }
    }

#undef DRAW_OBJ_AND_LINKS
#undef SETUP_NOMAT
}

void mapDispMapObj_bbox(int cameraId, int obj) {
    typedef f32 Mtx[3][4];
    extern void sysWaitDrawSync(void);
    extern void GXClearBoundingBox(void);
    extern void* camGetPtr(s32 cameraId);
    extern void PSMTXConcat(void* a, void* b, void* out);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void PSMTXInvXpose(void* src, void* dst);
    extern void GXLoadNrmMtxImm(void* mtx, s32 id);
    extern void GXSetCurrentMtx(s32 id);
    extern void GXSetCullMode(s32 mode);
    extern void GXSetNumChans(s32 count);
    extern void GXSetChanMatColor(s32 chan, void* color);
    extern void GXSetChanCtrl(s32, s32, s32, s32, s32, s32, s32);
    extern void GXSetNumTexGens(s32 count);
    extern void GXSetNumTevStages(s32 count);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevOp(s32, s32);
    extern void mapSetPolygon(s32 obj, s32 mesh);
    extern void GXReadBoundingBox(u16* left, u16* top, u16* right, u16* bottom);
    extern u32 unk_80429524;
    void* camera;
    s32 joint;
    s32 i;
    s32 offset;
    u32 color;
    Mtx model;
    Mtx normal;

    sysWaitDrawSync();
    GXClearBoundingBox();
    camera = camGetPtr(cameraId);
    PSMTXConcat((u8*)camera + 0x11C, (void*)(obj + 0x1C), model);
    GXLoadPosMtxImm(model, 0);
    PSMTXInvXpose(model, normal);
    GXLoadNrmMtxImm(normal, 0);
    GXSetCurrentMtx(0);
    GXSetCullMode(3);
    GXSetNumChans(1);
    color = unk_80429524;
    GXSetChanMatColor(4, &color);
    GXSetChanCtrl(0, 0, 0, 0, 0, 0, 2);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0xFF, 0xFF, 0);
    GXSetTevOp(0, 4);

    joint = *(s32*)(obj + 8);
    offset = 0;
    for (i = 0; i < *(s32*)(joint + 0x5C); i++) {
        mapSetPolygon(obj, *(s32*)(joint + offset + 0x64));
        offset += 8;
    }

    sysWaitDrawSync();
    GXReadBoundingBox(
        (u16*)(obj + 0xEC),
        (u16*)(obj + 0xEE),
        (u16*)(obj + 0xF0),
        (u16*)(obj + 0xF2)
    );
}

void mapDispMapGrp_bbox(s32 group, void* obj) {
    extern void sysWaitDrawSync(void);
    extern void GXClearBoundingBox(void);
    extern void _mapDispMapGrp_NoMaterial(s32, void*);
    extern void GXReadBoundingBox(void*, void*, void*, void*);
    sysWaitDrawSync();
    GXClearBoundingBox();
    _mapDispMapGrp_NoMaterial(group, obj);
    sysWaitDrawSync();
    GXReadBoundingBox((void*)((s32)obj + 0xEC), (void*)((s32)obj + 0xEE),
                      (void*)((s32)obj + 0xF0), (void*)((s32)obj + 0xF2));
}

void mapDisp(void) {
    extern s32 activeGroup;
    extern s32 mapWork;
    extern s32 unk_8041e678;
    extern s32 mapClipOffFlag;
    extern void* camGetPtr(s32 cameraId);
    extern void PSMTXScale(void* mtx, f32 x, f32 y, f32 z);
    extern u8 mapReCalcMatrix(void* obj, s32 mtx, int recurse);
    extern void PSMTXConcat(void* a, void* b, void* out);
    extern void PSMTXMultVec(void* mtx, void* src, void* dst);
    extern void PSMTX44MultVec(void* mtx, void* src, void* dst);
    extern void dispEntry(s32 cameraId, s32 renderMode, void* callback, void* param, f32 order);
    extern u8 mapDispMapObj_bbox(int cameraId, int obj);

    f32 scaleMtx[3][4];
    f32 modelView[3][4];
    f32 pos[3];
    f32 order;
    void* cam;
    s32 group;
    s32 entry;
    s32 obj;
    s32 joint;
    s32 count;
    s32 i;
    s32 j;
    s32 flags;
    s32 renderMode;
    s32 outside;
    s32 transparent;
    s32 material;

    group = mapWork + activeGroup * 0x2F4;
    cam = camGetPtr(4);

    if (unk_8041e678 != 0) {
        return;
    }

    entry = group;
    for (i = 0; i < *(s32*)group; i++, entry += 0x178) {
        if (*(s32*)(entry + 0xA8) != 0) {
            PSMTXScale(scaleMtx, 0.0f, 0.0f, 0.0f);
            mapReCalcMatrix(*(void**)(entry + 0xA8), (s32)scaleMtx, 0);
        }

        obj = *(s32*)(entry + 0x154);
        count = *(s32*)(entry + 0x150);
        for (j = 0; j < count; j++, obj += 0x134) {
            flags = *(s32*)obj;

            if (flags & 0x4000) {
                continue;
            }
            if ((flags & 1) && !(flags & 0x80)) {
                continue;
            }
            if ((flags & 0x40) && (*(u8*)(obj + 0xF) == 0)) {
                continue;
            }

            if (flags & 0x20000000) {
                *(u16*)(obj + 0xF0) = 0;
                *(u16*)(obj + 0xF2) = 0;
                *(u16*)(obj + 0xEC) = 0;
                *(u16*)(obj + 0xEE) = 0;
            }

            flags = *(s32*)obj;
            if (flags & 0x20) {
                continue;
            }

            joint = *(s32*)(obj + 8);
            if (!(flags & 0x10) && (*(s32*)(joint + 0x5C) < 1)) {
                continue;
            }
            if (!(flags & 0x800) && (*(u16*)(entry + 4) & 2) && (*(u8*)(entry + 0x19) == 0)) {
                continue;
            }
            if (!(flags & 0x1000) && (*(u16*)(entry + 4) & 4) && (*(u8*)(entry + 0x1D) == 0)) {
                continue;
            }

            PSMTXConcat((void*)((s32)cam + 0x11C), (void*)(obj + 0x1C), modelView);

            if ((flags & 2) && (mapClipOffFlag == 0)) {
                outside = 1;

                pos[0] = *(f32*)(joint + 0x48);
                pos[1] = *(f32*)(joint + 0x4C);
                pos[2] = *(f32*)(joint + 0x50);
                PSMTXMultVec(modelView, pos, pos);
                PSMTX44MultVec((void*)((s32)cam + 0x15C), pos, pos);
                if ((pos[2] > -1.0f) && (pos[2] < 0.0f) &&
                    (pos[0] >= -1.0f) && (pos[0] <= 1.0f) &&
                    (pos[1] >= -1.0f) && (pos[1] <= 1.0f)) {
                    outside = 0;
                } else {
                    pos[0] = *(f32*)(joint + 0x3C);
                    pos[1] = *(f32*)(joint + 0x40);
                    pos[2] = *(f32*)(joint + 0x44);
                    PSMTXMultVec(modelView, pos, pos);
                    PSMTX44MultVec((void*)((s32)cam + 0x15C), pos, pos);
                    if ((pos[2] > -1.0f) && (pos[2] < 0.0f) &&
                        (pos[0] >= -1.0f) && (pos[0] <= 1.0f) &&
                        (pos[1] >= -1.0f) && (pos[1] <= 1.0f)) {
                        outside = 0;
                    } else {
                        pos[0] = *(f32*)(joint + 0x3C);
                        pos[1] = *(f32*)(joint + 0x4C);
                        pos[2] = *(f32*)(joint + 0x50);
                        PSMTXMultVec(modelView, pos, pos);
                        PSMTX44MultVec((void*)((s32)cam + 0x15C), pos, pos);
                        if ((pos[2] > -1.0f) && (pos[2] < 0.0f) &&
                            (pos[0] >= -1.0f) && (pos[0] <= 1.0f) &&
                            (pos[1] >= -1.0f) && (pos[1] <= 1.0f)) {
                            outside = 0;
                        } else {
                            pos[0] = *(f32*)(joint + 0x48);
                            pos[1] = *(f32*)(joint + 0x40);
                            pos[2] = *(f32*)(joint + 0x50);
                            PSMTXMultVec(modelView, pos, pos);
                            PSMTX44MultVec((void*)((s32)cam + 0x15C), pos, pos);
                            if ((pos[2] > -1.0f) && (pos[2] < 0.0f) &&
                                (pos[0] >= -1.0f) && (pos[0] <= 1.0f) &&
                                (pos[1] >= -1.0f) && (pos[1] <= 1.0f)) {
                                outside = 0;
                            } else {
                                pos[0] = *(f32*)(joint + 0x48);
                                pos[1] = *(f32*)(joint + 0x4C);
                                pos[2] = *(f32*)(joint + 0x44);
                                PSMTXMultVec(modelView, pos, pos);
                                PSMTX44MultVec((void*)((s32)cam + 0x15C), pos, pos);
                                if ((pos[2] > -1.0f) && (pos[2] < 0.0f) &&
                                    (pos[0] >= -1.0f) && (pos[0] <= 1.0f) &&
                                    (pos[1] >= -1.0f) && (pos[1] <= 1.0f)) {
                                    outside = 0;
                                } else {
                                    pos[0] = *(f32*)(joint + 0x3C);
                                    pos[1] = *(f32*)(joint + 0x40);
                                    pos[2] = *(f32*)(joint + 0x50);
                                    PSMTXMultVec(modelView, pos, pos);
                                    PSMTX44MultVec((void*)((s32)cam + 0x15C), pos, pos);
                                    if ((pos[2] > -1.0f) && (pos[2] < 0.0f) &&
                                        (pos[0] >= -1.0f) && (pos[0] <= 1.0f) &&
                                        (pos[1] >= -1.0f) && (pos[1] <= 1.0f)) {
                                        outside = 0;
                                    } else {
                                        pos[0] = *(f32*)(joint + 0x48);
                                        pos[1] = *(f32*)(joint + 0x40);
                                        pos[2] = *(f32*)(joint + 0x44);
                                        PSMTXMultVec(modelView, pos, pos);
                                        PSMTX44MultVec((void*)((s32)cam + 0x15C), pos, pos);
                                        if ((pos[2] > -1.0f) && (pos[2] < 0.0f) &&
                                            (pos[0] >= -1.0f) && (pos[0] <= 1.0f) &&
                                            (pos[1] >= -1.0f) && (pos[1] <= 1.0f)) {
                                            outside = 0;
                                        } else {
                                            pos[0] = *(f32*)(joint + 0x3C);
                                            pos[1] = *(f32*)(joint + 0x4C);
                                            pos[2] = *(f32*)(joint + 0x44);
                                            PSMTXMultVec(modelView, pos, pos);
                                            PSMTX44MultVec((void*)((s32)cam + 0x15C), pos, pos);
                                            if ((pos[2] > -1.0f) && (pos[2] < 0.0f) &&
                                                (pos[0] >= -1.0f) && (pos[0] <= 1.0f) &&
                                                (pos[1] >= -1.0f) && (pos[1] <= 1.0f)) {
                                                outside = 0;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                if (outside != 0) {
                    continue;
                }
            }

            PSMTXMultVec(modelView, (void*)(obj + 0x10), pos);
            PSMTX44MultVec((void*)((s32)cam + 0x15C), pos, pos);

            order = 5000.0f * pos[2] + 5000.0f;
            if (order < 0.0f) {
                order = 0.0f;
            } else if (order > 10000.0f) {
                order = 10000.0f;
            }
            order = -order;

            renderMode = *(u8*)(obj + 4);
            if (*(s32*)(joint + 0x5C) > 0) {
                transparent = 0;
                if ((flags & 0x40) && (*(u8*)(obj + 0xF) != 0xFF)) {
                    transparent = 1;
                } else {
                    material = *(s32*)(*(s32*)(joint + 0x60));
                    if (*(u8*)(material + 0x10F) != 0xFF) {
                        transparent = 1;
                    } else if (!(flags & 0x800) && (*(u16*)(entry + 4) & 2) && (*(u8*)(entry + 0x19) != 0xFF)) {
                        transparent = 1;
                    } else if (!(flags & 0x1000) && (*(u16*)(entry + 4) & 4) && (*(u8*)(entry + 0x1D) != 0xFF)) {
                        transparent = 1;
                    }
                }
                if (transparent != 0) {
                    renderMode = 2;
                }
            }

            if (flags & 0x20000000) {
                dispEntry(1, 3, (flags & 0x10) ? (void*)mapDispMapGrp_bbox : (void*)mapDispMapObj_bbox, (void*)obj, 5000.0f);
            }
            if (flags & 0x80) {
                dispEntry(1, renderMode, (flags & 0x10) ? (void*)mapDispMapGrp_off : (void*)mapDispMapObj_off, (void*)obj, order);
            }
            if (!(flags & 1)) {
                dispEntry(*(u8*)(obj + 5), renderMode, (flags & 0x10) ? (void*)mapDispMapGrp : (void*)mapDispMapObj, (void*)obj, order);
            }
        }
    }
}

int mapSetLight(void* param_1, void** param_2) {
    extern void PSMTXMultVec(void* mtx, void* src, void* dst);
    extern int lightGetNearObj(void* pos, void** dst, int count, int flags);
    extern u16* lightNameToPtr(char* name);
    extern s32 activeGroup;
    extern s32 mapWork;
    f32 pos[3];
    void* drawMode;
    u16* light;
    register int count;
    register int i;
    register void** out;
    register int offset;

    count = 0;
    PSMTXMultVec((void*)((s32)param_1 + 0x1C), (void*)((s32)param_1 + 0x10), pos);
    drawMode = *(void**)(*(s32*)((s32)param_1 + 8) + 0x58);
    if ((*(u8*)((s32)drawMode + 2) & 8) != 0) {
        if (*(u32*)((s32)drawMode + 4) == 0) {
            count = lightGetNearObj(pos, param_2, 8, 0);
        } else {
            out = param_2;
            i = 0;
            offset = 0;
            do {
                if ((*(u32*)(*(s32*)(*(s32*)((s32)param_1 + 8) + 0x58) + 4) & (1 << i)) != 0) {
                    void* table = *(void**)(mapWork + activeGroup * 0x2F4 + *(s16*)((s32)param_1 + 0xDE) * 0x178 + 0x98);
                    light = lightNameToPtr(*(char**)*(s32*)((s32)table + offset + 4));
                    if ((*light & 0x8000) == 0) {
                        count++;
                        *out = light;
                        out++;
                        if (count >= 8) {
                            break;
                        }
                    }
                }
                i++;
                offset += 4;
            } while (i < 0x20);
            if (count < 8) {
                count += lightGetNearObj(pos, param_2 + count, 8 - count, 0);
            }
        }
    } else {
        if (*(u32*)((s32)drawMode + 4) != 0) {
            i = 0;
            out = param_2;
            offset = 0;
            do {
                if ((*(u32*)(*(s32*)(*(s32*)((s32)param_1 + 8) + 0x58) + 4) & (1 << i)) != 0) {
                    void* table = *(void**)(mapWork + activeGroup * 0x2F4 + *(s16*)((s32)param_1 + 0xDE) * 0x178 + 0x98);
                    light = lightNameToPtr(*(char**)*(s32*)((s32)table + offset + 4));
                    if ((*light & 0x8000) == 0) {
                        count++;
                        *out = light;
                        out++;
                        if (count >= 8) {
                            break;
                        }
                    }
                }
                i++;
                offset += 4;
            } while (i < 0x20);
        }
    }
    return count;
}


void mapSetMaterialTev(u32 texCount, int drawMode, u32 materialFlag, s32 pMtx) {
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevOp(s32, s32);
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
    extern void GXSetTevSwapMode(s32, s32, s32);
    extern void GXSetTevKColorSel(s32, s32);
    extern void GXSetTevKAlphaSel(s32, s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern void GXSetNumChans(s32);
    extern void GXSetCullMode(s32);
    extern void GXSetBlendMode(s32, s32, s32, s32);
    extern void GXSetZCompLoc(s32);
    extern void GXSetAlphaCompare(s32, s32, s32, s32, s32);
    extern void GXSetZMode(s32, s32, s32);
    u32 stages;
    s32 i;

    if ((s32)texCount < 1) {
        GXSetTevOrder(0, 0xFF, 0xFF, 0);
        GXSetTevColorOp(0, 0, 0, 0, 1, 0);
        GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
        GXSetTevColorIn(0, 0xF, 0xF, 0xF, 10);
        GXSetTevAlphaIn(0, 7, 7, 7, 5);
        GXSetTevSwapMode(0, 0, 0);
        stages = 1;
    } else {
        switch (drawMode) {
            case 0:
                GXSetTevOrder(0, 0, 0, 0);
                GXSetTevOp(0, 0);
                GXSetTevSwapMode(0, 0, 0);
                stages = 1;
                break;
            case 1:
                GXSetTevOrder(0, 0, 0, 0xFF);
                GXSetTevColorOp(0, 0, 0, 0, 1, 0);
                GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
                GXSetTevColorIn(0, 0xF, 0xF, 0xF, 8);
                GXSetTevAlphaIn(0, 7, 7, 7, 4);
                GXSetTevSwapMode(0, 0, 0);
                GXSetTevOrder(1, 1, 1, 0xFF);
                GXSetTevColorOp(1, 0, 0, 0, 1, 0);
                GXSetTevAlphaOp(1, 0, 0, 0, 1, 0);
                GXSetTevColorIn(1, 0, 8, 4, 0xF);
                GXSetTevAlphaIn(1, 7, 7, 7, 0);
                GXSetTevSwapMode(1, 0, 0);
                GXSetTevOrder(2, 0xFF, 0xFF, 0);
                GXSetTevColorOp(2, 0, 0, 0, 1, 0);
                GXSetTevAlphaOp(2, 0, 0, 0, 1, 0);
                GXSetTevColorIn(2, 0xF, 0, 10, 0xF);
                GXSetTevAlphaIn(2, 7, 0, 5, 7);
                GXSetTevSwapMode(2, 0, 0);
                stages = 3;
                break;
            case 2:
                GXSetTevOrder(0, 0, 0, 0xFF);
                GXSetTevColorOp(0, 0, 0, 0, 1, 0);
                GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
                GXSetTevColorIn(0, 0xF, 0xF, 0xF, 8);
                GXSetTevAlphaIn(0, 7, 7, 7, 4);
                GXSetTevSwapMode(0, 0, 0);
                GXSetTevOrder(1, 1, 1, 0xFF);
                GXSetTevColorOp(1, 0, 0, 0, 1, 0);
                GXSetTevAlphaOp(1, 0, 0, 0, 1, 0);
                GXSetTevColorIn(1, 0xF, 0, 4, 0xF);
                GXSetTevAlphaIn(1, 7, 0, 4, 7);
                GXSetTevSwapMode(1, 0, 0);
                GXSetTevOrder(2, 0xFF, 0xFF, 0);
                GXSetTevColorOp(2, 0, 0, 0, 1, 0);
                GXSetTevAlphaOp(2, 0, 0, 0, 1, 0);
                GXSetTevColorIn(2, 0xF, 0, 10, 0xF);
                GXSetTevAlphaIn(2, 7, 0, 5, 7);
                GXSetTevSwapMode(2, 0, 0);
                stages = 3;
                break;
            case 3:
                GXSetTevOrder(0, 0, 0, 0xFF);
                GXSetTevColorOp(0, 0, 0, 0, 1, 0);
                GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
                GXSetTevColorIn(0, 0xF, 0xF, 0xF, 8);
                GXSetTevAlphaIn(0, 7, 7, 7, 4);
                GXSetTevSwapMode(0, 0, 0);
                GXSetTevOrder(1, 1, 1, 0xFF);
                GXSetTevColorOp(1, 0, 0, 0, 1, 0);
                GXSetTevAlphaOp(1, 0, 0, 0, 1, 0);
                GXSetTevColorIn(1, 0, 0xF, 4, 0xF);
                GXSetTevAlphaIn(1, 0, 7, 4, 7);
                GXSetTevSwapMode(1, 0, 0);
                GXSetTevOrder(2, 0xFF, 0xFF, 0);
                GXSetTevColorOp(2, 0, 0, 0, 1, 0);
                GXSetTevAlphaOp(2, 0, 0, 0, 1, 0);
                GXSetTevColorIn(2, 0xF, 0, 10, 0xF);
                GXSetTevAlphaIn(2, 7, 0, 5, 7);
                GXSetTevSwapMode(2, 0, 0);
                stages = 3;
                break;
            case 4:
                GXSetTevOrder(0, 0, 0, 0xFF);
                GXSetTevColorOp(0, 0, 0, 0, 1, 0);
                GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
                GXSetTevColorIn(0, 0xF, 0xF, 0xF, 8);
                GXSetTevAlphaIn(0, 7, 7, 7, 4);
                GXSetTevSwapMode(0, 0, 0);
                GXSetTevOrder(1, 1, 1, 0xFF);
                GXSetTevColorOp(1, 0, 0, 0, 1, 0);
                GXSetTevAlphaOp(1, 0, 0, 0, 1, 0);
                GXSetTevColorIn(1, 0xF, 0, 8, 0xF);
                GXSetTevAlphaIn(1, 7, 0, 4, 7);
                GXSetTevSwapMode(1, 0, 0);
                GXSetTevOrder(2, 0, 0, 0xFF);
                GXSetTevColorOp(2, 0, 0, 0, 1, 0);
                GXSetTevAlphaOp(2, 0, 0, 0, 1, 0);
                GXSetTevColorIn(2, 8, 0, 0, 0xF);
                GXSetTevAlphaIn(2, 7, 7, 7, 4);
                GXSetTevSwapMode(2, 0, 0);
                stages = 3;
                break;
            case 5:
                GXSetTevOrder(0, 0, 0, 0xFF);
                GXSetTevColorOp(0, 0, 0, 0, 1, 1);
                GXSetTevAlphaOp(0, 0, 0, 0, 1, 1);
                GXSetTevColorIn(0, 0xF, 0xF, 0xF, 8);
                GXSetTevAlphaIn(0, 7, 7, 7, 4);
                GXSetTevSwapMode(0, 0, 0);
                GXSetTevOrder(1, 1, 1, 0xFF);
                GXSetTevColorOp(1, 0, 0, 0, 1, 2);
                GXSetTevAlphaOp(1, 0, 0, 0, 1, 2);
                GXSetTevColorIn(1, 0xF, 0xF, 0xF, 8);
                GXSetTevAlphaIn(1, 7, 7, 7, 4);
                GXSetTevSwapMode(1, 0, 0);
                GXSetTevOrder(2, 2, 2, 0xFF);
                GXSetTevColorOp(2, 0, 0, 0, 1, 0);
                GXSetTevAlphaOp(2, 0, 0, 0, 1, 0);
                GXSetTevColorIn(2, 2, 4, 4, 0xF);
                GXSetTevAlphaIn(2, 1, 2, 4, 7);
                GXSetTevSwapMode(2, 0, 0);
                GXSetTevOrder(3, 0xFF, 0xFF, 0);
                GXSetTevColorOp(3, 0, 0, 0, 1, 0);
                GXSetTevAlphaOp(3, 0, 0, 0, 1, 0);
                GXSetTevColorIn(3, 0xF, 0, 10, 0xF);
                GXSetTevAlphaIn(3, 7, 0, 5, 7);
                GXSetTevSwapMode(3, 0, 0);
                stages = 4;
                break;
            case 6:
                GXSetTevOrder(0, 0, 0, 0xFF);
                GXSetTevColorOp(0, 0, 0, 0, 1, 0);
                GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
                GXSetTevColorIn(0, 0xF, 0xF, 0xF, 8);
                GXSetTevAlphaIn(0, 7, 7, 7, 4);
                GXSetTevSwapMode(0, 0, 0);
                GXSetTevOrder(1, 1, 1, 0xFF);
                GXSetTevColorOp(1, 0, 0, 0, 1, 0);
                GXSetTevAlphaOp(1, 0, 0, 0, 1, 0);
                GXSetTevColorIn(1, 0, 8, 0, 0xF);
                GXSetTevAlphaIn(1, 7, 4, 0, 7);
                GXSetTevSwapMode(1, 0, 0);
                GXSetTevOrder(2, 0xFF, 0xFF, 0);
                GXSetTevColorOp(2, 0, 0, 0, 1, 0);
                GXSetTevAlphaOp(2, 0, 0, 0, 1, 0);
                GXSetTevColorIn(2, 0xF, 0, 10, 0xF);
                GXSetTevAlphaIn(2, 7, 0, 5, 7);
                GXSetTevSwapMode(2, 0, 0);
                stages = 3;
                break;
            case 7:
                GXSetTevOrder(0, 0, 0, 0xFF);
                GXSetTevColorOp(0, 0, 0, 0, 1, 0);
                GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
                GXSetTevColorIn(0, 0xF, 0xF, 0xF, 8);
                GXSetTevAlphaIn(0, 7, 7, 7, 4);
                GXSetTevSwapMode(0, 0, 0);
                GXSetTevOrder(1, 1, 1, 0xFF);
                GXSetTevColorOp(1, 0, 0, 0, 1, 0);
                GXSetTevAlphaOp(1, 0, 0, 0, 1, 0);
                GXSetTevColorIn(1, 0, 8, 4, 0xF);
                GXSetTevAlphaIn(1, 0, 7, 4, 4);
                GXSetTevSwapMode(1, 0, 0);
                GXSetTevOrder(2, 0xFF, 0xFF, 0);
                GXSetTevColorOp(2, 0, 0, 0, 1, 0);
                GXSetTevAlphaOp(2, 0, 0, 0, 1, 0);
                GXSetTevColorIn(2, 0xF, 0, 10, 0xF);
                GXSetTevAlphaIn(2, 7, 0, 5, 7);
                GXSetTevSwapMode(2, 0, 0);
                stages = 3;
                break;
            case 0xC:
                GXSetTevOrder(0, 0, 0, 0xFF);
                GXSetTevColorOp(0, 0, 0, 0, 1, 0);
                GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
                GXSetTevColorIn(0, 0xF, 0xF, 0xF, 8);
                GXSetTevAlphaIn(0, 7, 7, 7, 7);
                GXSetTevSwapMode(0, 0, 0);
                GXSetTevOrder(1, 1, 1, 0xFF);
                GXSetTevColorOp(1, 0, 0, 0, 1, 0);
                GXSetTevAlphaOp(1, 0, 0, 0, 1, 0);
                GXSetTevColorIn(1, 0xF, 0xF, 0xF, 0);
                GXSetTevAlphaIn(1, 7, 7, 7, 4);
                GXSetTevSwapMode(1, 0, 0);
                GXSetTevOrder(2, 0xFF, 0xFF, 0);
                GXSetTevColorOp(2, 0, 0, 0, 1, 0);
                GXSetTevAlphaOp(2, 0, 0, 0, 1, 0);
                GXSetTevColorIn(2, 0xF, 0, 10, 0xF);
                GXSetTevAlphaIn(2, 7, 0, 5, 7);
                GXSetTevSwapMode(2, 0, 0);
                stages = 3;
                break;
            default:
                GXSetTevOrder(0, 0, 0, 0);
                GXSetTevColorOp(0, 0, 0, 0, 1, 0);
                GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
                GXSetTevColorIn(0, 0xF, 10, 8, 0xF);
                GXSetTevAlphaIn(0, 7, 5, 4, 7);
                GXSetTevSwapMode(0, 0, 0);
                stages = 1;
                break;
        }
    }

    if ((materialFlag & 0x10000000) == 0) {
        GXSetTevKColorSel(stages, 0xC);
        GXSetTevKAlphaSel(stages, 0x1C);
        GXSetTevOrder(stages, 0xFF, 0xFF, 0xFF);
        GXSetTevColorOp(stages, 0, 0, 0, 1, 0);
        GXSetTevAlphaOp(stages, 0, 0, 0, 1, 0);
        GXSetTevColorIn(stages, 0xF, 0, 0xE, 0xF);
        GXSetTevAlphaIn(stages, 7, 0, 6, 7);
        GXSetTevSwapMode(stages, 0, 0);
        stages++;
    }

    GXSetNumTevStages(stages);
    GXSetNumTexGens(texCount);
    for (i = 0; i < (s32)texCount && i < 8; i++) {
        GXSetTexCoordGen2(i, 1, 4, 0x1E + i * 3, 0, 0x7D);
    }
    GXSetNumChans(1);
    GXSetCullMode((materialFlag & 0x400) ? 2 : ((materialFlag & 0x800) ? 1 : 0));
    if ((materialFlag & 0x20) != 0) {
        GXSetBlendMode(1, 4, 5, 0);
        GXSetZCompLoc(1);
        GXSetAlphaCompare(7, 0, 0, 7, 0);
        GXSetZMode(1, 3, 0);
    } else {
        GXSetBlendMode(0, 1, 0, 0);
        GXSetZCompLoc(0);
        GXSetAlphaCompare(6, 0x80, 1, 0, 0);
        GXSetZMode(1, 3, 1);
    }
    (void)pMtx;
}

void mapSetMaterialLastStageBlend(u32 flags, void* pEvtColor, void* param_3) {
    typedef struct Color {
        u8 r;
        u8 g;
        u8 b;
        u8 a;
    } Color;
    extern u32 dat_8041f8fc;
    extern s32 activeGroup;
    extern s32 mapWork;
    extern void GXSetTevKColor(s32 id, void* color);
    Color color;
    Color outColor;
    Color* evtColor;
    Color* alphaColor;
    Color* blend;
    u8* group;
    u16 dataFlags;

    group = (u8*)(mapWork + activeGroup * 0x2F4);
    *(u32*)&color = dat_8041f8fc;
    evtColor = (Color*)pEvtColor;
    alphaColor = (Color*)param_3;

    if ((flags & 0x40) != 0) {
        color.r = (color.r * evtColor->r) / 0xFF;
        color.g = (color.g * evtColor->g) / 0xFF;
        color.b = (color.b * evtColor->b) / 0xFF;
        color.a = (color.a * evtColor->a) / 0xFF;
    }
    if (alphaColor->r != 0xFF) {
        color.a = (color.a * alphaColor->r) / 0xFF;
    }

    dataFlags = *(u16*)(group + 4);
    if (((dataFlags & 2) != 0) && ((flags & 0x800) == 0)) {
        blend = (Color*)(group + 0x16);
        color.r = (color.r * blend->r) / 0xFF;
        color.g = (color.g * blend->g) / 0xFF;
        color.b = (color.b * blend->b) / 0xFF;
        color.a = (color.a * blend->a) / 0xFF;
    }
    if (((dataFlags & 4) != 0) && ((flags & 0x1000) == 0)) {
        blend = (Color*)(group + 0x1A);
        color.r = (color.r * blend->r) / 0xFF;
        color.g = (color.g * blend->g) / 0xFF;
        color.b = (color.b * blend->b) / 0xFF;
        color.a = (color.a * blend->a) / 0xFF;
    }
    if (((dataFlags & 8) != 0) && ((flags & 0x800) == 0)) {
        blend = (Color*)(group + 0x1E);
        color.r = (color.r * blend->r) / 0xFF;
        color.g = (color.g * blend->g) / 0xFF;
        color.b = (color.b * blend->b) / 0xFF;
        color.a = (color.a * blend->a) / 0xFF;
    }

    outColor = color;
    GXSetTevKColor(0, &outColor);
}

void mapSetMaterialFog(void) {
    extern s32 activeGroup;
    extern s32 mapWork;
    extern s32 fog_type;
    extern u32 dat_8041f900;
    extern f32 float_0_8041f930;
    extern void* camGetCurPtr(void);
    extern void GXSetFog(s32 type, f32 start, f32 end, f32 nearz, f32 farz, void* color);
    void* cam;
    void* work;
    u32 color;
    u32 zeroColor;

    cam = camGetCurPtr();
    work = (void*)(mapWork + activeGroup * 0x2F4);
    if ((*(u16*)((s32)work + 4) & 1) != 0) {
        color = *(u32*)((s32)work + 0x34);
        GXSetFog(
            *(s32*)((s32)&fog_type + (*(s32*)((s32)work + 0x28) * 4)),
            *(f32*)((s32)work + 0x2C),
            *(f32*)((s32)work + 0x30),
            *(f32*)((s32)cam + 0x30),
            *(f32*)((s32)cam + 0x34),
            &color
        );
    } else {
        zeroColor = dat_8041f900;
        GXSetFog(
            0,
            float_0_8041f930,
            float_0_8041f930,
            float_0_8041f930,
            float_0_8041f930,
            &zeroColor
        );
    }
}

void mapSetTextureMatrix(void* param_1) {
    extern s32 texmtx_tbl[];
    extern f32 float_1_8041f940;
    extern f32 float_0_8041f930;
    extern f32 float_0p5_8041f934;
    extern f32 float_deg2rad_8041f948;

    extern void PSMTXTrans(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXRotRad(void* mtx, s32 axis, f32 rad);
    extern void PSMTXConcat(void* a, void* b, void* out);
    extern void PSMTXScale(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXCopy(void* src, void* dst);
    extern void GXLoadTexMtxImm(void* mtx, s32 id, s32 type);
    extern void GXSetTexCoordGen2(s32 coord, s32 func, s32 source, u32 mtx, u32 normalize, s32 postmtx);

    f32 outMtx[3][4];
    f32 negCenterMtx[3][4];
    f32 transMtx[3][4];
    f32 rotMtx[3][4];
    f32 scaleMtx[3][4];

    f32 one;
    f32 scaleX;
    f32 scaleY;
    f32 transX;
    f32 negTransY;
    f32 negRot;
    f32 centerX;
    f32 centerY;
    f32 zero;
    void* work;
    s32 coord;
    s32 index;
    s32 countIndex;
    s32* texMtx;
    s32 texCount;

    one = float_1_8041f940;
    work = param_1;
    texCount = *(u8*)((s32)work + 0x0B);
    coord = 0;
    index = texCount - 1;
    countIndex = 0;
    work = (void*)((s32)work + index * 0x1C);
    texMtx = &texmtx_tbl[index];

    while (index >= 0) {
        scaleX = *(f32*)((s32)work + 0x34);
        negRot = -*(f32*)((s32)work + 0x3C);
        scaleY = *(f32*)((s32)work + 0x38);
        transX = *(f32*)((s32)work + 0x2C);
        negTransY = -*(f32*)((s32)work + 0x30);

        if (one == scaleX &&
            one == scaleY &&
            float_0_8041f930 == transX &&
            float_0_8041f930 == negTransY &&
            float_0_8041f930 == negRot) {
            GXSetTexCoordGen2(coord, 1, (*(u8*)((s32)param_1 + 0x0B) + 3) - countIndex, 0x3C, 0, 0x7D);
        } else {
            zero = float_0_8041f930;
            if (negRot != zero) {
                centerX = (float_0p5_8041f934 * *(f32*)((s32)work + 0x40)) * scaleX;
                centerY = (float_1_8041f940 - (float_0p5_8041f934 * *(f32*)((s32)work + 0x44))) * scaleY;

                PSMTXTrans(outMtx, centerX, centerY, zero);
                PSMTXRotRad(rotMtx, 'z', float_deg2rad_8041f948 * negRot);
                PSMTXTrans(negCenterMtx, -centerX, -centerY, float_0_8041f930);
                PSMTXConcat(outMtx, rotMtx, rotMtx);
                PSMTXConcat(rotMtx, negCenterMtx, rotMtx);
            }

            if (scaleX != float_0_8041f930 || scaleY != float_0_8041f930) {
                PSMTXScale(scaleMtx, scaleX, scaleY, float_1_8041f940);
            }

            if (transX != float_0_8041f930 || negTransY != float_0_8041f930) {
                PSMTXTrans(transMtx, transX, negTransY, float_0_8041f930);
            }

            zero = float_0_8041f930;
            if (negRot != zero) {
                if (scaleX != zero || scaleY != zero) {
                    PSMTXConcat(rotMtx, scaleMtx, outMtx);
                    if (transX != float_0_8041f930 || negTransY != float_0_8041f930) {
                        PSMTXConcat(outMtx, transMtx, outMtx);
                    }
                } else if (transX != zero || negTransY != zero) {
                    PSMTXConcat(rotMtx, transMtx, outMtx);
                } else {
                    PSMTXCopy(rotMtx, outMtx);
                }
            } else if (scaleX != zero || scaleY != zero) {
                if (transX != float_0_8041f930 || negTransY != float_0_8041f930) {
                    PSMTXConcat(scaleMtx, transMtx, outMtx);
                } else {
                    PSMTXCopy(scaleMtx, outMtx);
                }
            } else if (transX != zero || negTransY != zero) {
                PSMTXCopy(transMtx, outMtx);
            }

            GXLoadTexMtxImm(outMtx, *texMtx, 1);
            GXSetTexCoordGen2(coord, 1, (*(u8*)((s32)param_1 + 0x0B) + 3) - countIndex, *texMtx, 0, 0x7D);
        }

        coord++;
        countIndex++;
        work = (void*)((s32)work - 0x1C);
        texMtx--;
        index--;
    }
}

void mapSetMaterial(void* param_1, void* param_2) {
    extern void mapSetMaterialTev(u32 texCount, int drawMode, u32 materialFlag, s32 pMtx);
    extern void* lightGetAmbient(void);
    extern u32 dat_8041f908;
    extern void* camGetCurPtr(void);
    extern void PSMTXMultVec(void* mtx, void* src, void* dst);
    extern void GXInitLightPos(void* lightObj, f32 x, f32 y, f32 z);
    extern void GXInitLightColor(void* lightObj, void* color);
    extern void GXInitLightAttn(void* lightObj, f32 a0, f32 a1, f32 a2, f32 k0, f32 k1, f32 k2);
    extern void GXLoadLightObjImm(void* lightObj, u32 lightId);
    extern s32 activeGroup;
    extern s32 mapWork;
    extern u32 texwrap_tbl[];
    extern s32 mapSetLight(void* mapObj, void* lightList);
    extern u32 lightid_tbl[];
    extern s32 strcmp(char* s1, char* s2);
    extern void* TEXGet(s32 table, s32 id);
    extern f32 float_deg2rad_8041f948;
    extern f32 float_neg1p0486E06_8041f950;
    extern f32 float_0_8041f930;
    extern f32 float_1_8041f940;
    extern u8 vec3_802bf958[];
    extern void PSMTXRotRad(void* mtx, s32 axis, f32 rad);
    extern void PSMTXConcat(void* a, void* b, void* ab);
    extern void PSVECScale(void* src, void* dst, f32 scale);
    extern void PSVECAdd(void* a, void* b, void* out);
    extern void GXSetNumChans(s32 count);
    extern void GXSetChanAmbColor(s32 chan, void* color);
    extern void GXSetChanMatColor(s32 chan, void* color);
    extern f32 float_0p5_8041f934;
    extern f32 float_1000_8041f954;
    extern void PSMTXMultVecSR(void* mtx, void* src, void* dst);
    extern void GXInitLightDir(void* lightObj, f32 x, f32 y, f32 z);
    extern void GXInitLightSpot(void* lightObj, f32 cutoff, s32 spotFn);
    extern void GXInitLightDistAttn(void* lightObj, f32 refDist, f32 refBrightness, s32 distFn);
    extern void GXSetChanCtrl(
        s32 chan,
        s32 enable,
        s32 ambSrc,
        s32 matSrc,
        u32 lightMask,
        s32 diffFn,
        s32 attnFn
    );

    extern void GXInitTexObj(
        void* obj,
        void* data,
        u16 width,
        u16 height,
        s32 format,
        u32 wrapS,
        u32 wrapT,
        s32 mipmap
    );
    extern void GXInitTexObjLOD(
        void* obj,
        s32 minFilt,
        s32 magFilt,
        f32 minLOD,
        f32 maxLOD,
        f32 lodBias,
        s32 biasClamp,
        s32 edgeLOD,
        s32 maxAniso
    );
    extern void GXLoadTexObj(void* obj, s32 mapId);

    void* ambientPtr;
    void* drawModePtr;
    u32 ambientColor;
    u32 materialColor;
    u32 texCount;
    int drawMode;
    u32 materialFlag;
    s32 pMtx;

        /* light-loop locals */
    s32 lightCount;
    u32 lightMask;
    s32 lightAttn;
    s32 lightEnable;
    u8 chanMode;
    void* lightList[16];
    s32 lightIndex;
    void* light;
    u16 lightFlags;
    u8 lightObj[0x40];
    f32 lightPos[3];
    u32 lightColor;
    void* cam;
    u8 mtxX[0x30];
    u8 mtxY[0x30];
    u8 mtxZ[0x30];
    f32 baseDir[3];

    /* texture-loop locals */
    u8 texObj[0x20];
    s32 texIndex;
    s32 loadedTexCount;
    s32 groupBase;
    s32 entryIndex;
    s32 entryBase;
    s32 texTableAddr;
    s32 texSearchIndex;
    s32 texSearchCount;
    s32 texId;
    s32 texTableEntry;
    void* texSlot;
    char* texName;
    void* tplDesc;
    void* texHeader;
    s32 mipmap;

        for (lightIndex = 0; lightIndex < lightCount; lightIndex++) {
        light = lightList[lightIndex];
        lightFlags = *(u16*)light;

        if ((lightFlags & 0x1000) != 0) {
            cam = camGetCurPtr();

            PSMTXMultVec(
                (void*)((s32)cam + 0x11C),
                (void*)((s32)light + 0x24),
                lightPos
            );

            GXInitLightPos(lightObj, lightPos[0], lightPos[1], lightPos[2]);

            lightColor = *(u32*)((s32)light + 0x48);
            GXInitLightColor(lightObj, &lightColor);

            GXInitLightAttn(
                lightObj,
                *(f32*)((s32)light + 0x50),
                0.0f,
                0.0f,
                *(f32*)((s32)light + 0x54),
                *(f32*)((s32)light + 0x58),
                *(f32*)((s32)light + 0x5C)
            );

            GXLoadLightObjImm(lightObj, lightid_tbl[lightIndex]);
            lightAttn = 1;
        } else if ((lightFlags & 0x2000) != 0) {
            baseDir[0] = *(f32*)((s32)vec3_802bf958 + 0x144);
            baseDir[1] = *(f32*)((s32)vec3_802bf958 + 0x148);
            baseDir[2] = *(f32*)((s32)vec3_802bf958 + 0x14C);

            PSMTXRotRad(
                mtxX,
                0x78,
                float_deg2rad_8041f948 * *(f32*)((s32)light + 0x30)
            );
            PSMTXRotRad(
                mtxY,
                0x79,
                float_deg2rad_8041f948 * *(f32*)((s32)light + 0x34)
            );
            PSMTXRotRad(
                mtxZ,
                0x7A,
                float_deg2rad_8041f948 * *(f32*)((s32)light + 0x38)
            );

            PSMTXConcat(mtxZ, mtxY, mtxY);
            PSMTXConcat(mtxY, mtxX, mtxX);
            PSMTXMultVec(mtxX, baseDir, baseDir);

            PSVECScale(baseDir, baseDir, float_neg1p0486E06_8041f950);
            PSVECAdd((void*)((s32)light + 0x24), baseDir, lightPos);

            cam = camGetCurPtr();
            PSMTXMultVec(
                (void*)((s32)cam + 0x11C),
                lightPos,
                lightPos
            );

            GXInitLightPos(lightObj, lightPos[0], lightPos[1], lightPos[2]);

            lightColor = *(u32*)((s32)light + 0x48);
            GXInitLightColor(lightObj, &lightColor);

            GXInitLightAttn(
                lightObj,
                *(f32*)((s32)light + 0x50),
                0.0f,
                0.0f,
                0.0f,
                0.0f,
                1.0f
            );

            GXLoadLightObjImm(lightObj, lightid_tbl[lightIndex]);
        } else if ((lightFlags & 0x4000) != 0) {
    cam = camGetCurPtr();

    PSMTXMultVec(
        (void*)((s32)cam + 0x11C),
        (void*)((s32)light + 0x24),
        lightPos
    );

    baseDir[0] = *(f32*)((s32)vec3_802bf958 + 0x144);
    baseDir[1] = *(f32*)((s32)vec3_802bf958 + 0x148);
    baseDir[2] = *(f32*)((s32)vec3_802bf958 + 0x14C);

    PSMTXRotRad(
        mtxX,
        0x78,
        float_deg2rad_8041f948 * *(f32*)((s32)light + 0x30)
    );
    PSMTXRotRad(
        mtxY,
        0x79,
        float_deg2rad_8041f948 * *(f32*)((s32)light + 0x34)
    );
    PSMTXRotRad(
        mtxZ,
        0x7A,
        float_deg2rad_8041f948 * *(f32*)((s32)light + 0x38)
    );

    PSMTXConcat(mtxZ, mtxY, mtxY);
    PSMTXConcat(mtxY, mtxX, mtxX);
    PSMTXMultVec(mtxX, baseDir, baseDir);

    cam = camGetCurPtr();
    PSMTXMultVecSR(
        (void*)((s32)cam + 0x11C),
        baseDir,
        baseDir
    );

    GXInitLightPos(lightObj, lightPos[0], lightPos[1], lightPos[2]);
    GXInitLightDir(lightObj, baseDir[0], baseDir[1], baseDir[2]);

    GXInitLightSpot(
        lightObj,
        *(f32*)((s32)light + 0x4C) * float_0p5_8041f934,
        4
    );

    GXInitLightDistAttn(
        lightObj,
        float_1000_8041f954,
        float_0p5_8041f934,
        0
    );

    lightColor = *(u32*)((s32)light + 0x48);
    GXInitLightColor(lightObj, &lightColor);

    GXLoadLightObjImm(lightObj, lightid_tbl[lightIndex]);
    lightAttn = 1;
}

        lightMask |= lightid_tbl[lightIndex];
    }

    ambientPtr = lightGetAmbient();
    if (ambientPtr != 0) {
        ambientColor = *(u32*)((s32)ambientPtr + 0x48);
    } else {
        ambientColor = dat_8041f908;
    }

    GXSetNumChans(1);

    GXSetChanAmbColor(4, &ambientColor);

    materialColor = *(u32*)((s32)param_2 + 4);
    GXSetChanMatColor(4, &materialColor);

    lightEnable = lightCount != 0;
    chanMode = *(u8*)((s32)param_2 + 8);

    switch (chanMode) {
        case 0:
            GXSetChanCtrl(0, lightEnable, 0, 0, lightMask, 2, lightAttn);
            GXSetChanCtrl(2, 0, 0, 0, 0, 0, 2);
            break;

        case 1:
            GXSetChanCtrl(0, lightEnable, 0, 1, lightMask, 2, lightAttn);
            GXSetChanCtrl(2, 0, 0, 1, 0, 0, 2);
            break;

        default:
            break;
    }

    loadedTexCount = 0;
    texIndex = *(u8*)((s32)param_2 + 0xB) - 1;

    while (texIndex >= 0) {
        texSlot = *(void**)((s32)param_2 + 0xC + texIndex * 4);
        texName = *(char**)(*(s32*)texSlot);

        entryIndex = *(s16*)((s32)param_1 + 0xDE);
        groupBase = mapWork + activeGroup * 0x2F4;
        entryBase = groupBase + entryIndex * 0x178;

        texTableAddr = *(s32*)(entryBase + 0x94);
        texSearchCount = *(s32*)texTableAddr;
        texSearchIndex = 0;

        while (texSearchIndex < texSearchCount) {
            texTableEntry = *(s32*)(texTableAddr + 4 + texSearchIndex * 4);
            if (strcmp((char*)texTableEntry, texName) == 0) {
                break;
            }
            texSearchIndex++;
        }

        if (texSearchIndex >= texSearchCount) {
            texId = -1;
        } else {
            texId = texSearchIndex;
        }

        tplDesc = TEXGet(*(s32*)(entryBase + 0x84), texId);
        texHeader = *(void**)tplDesc;

        mipmap = (*(u8*)((s32)texHeader + 0x21) != *(u8*)((s32)texHeader + 0x22));

        GXInitTexObj(
            texObj,
            *(void**)((s32)texHeader + 0x8),
            *(u16*)((s32)texHeader + 0x2),
            *(u16*)((s32)texHeader + 0x0),
            *(s32*)((s32)texHeader + 0x4),
            texwrap_tbl[*(u8*)((s32)texSlot + 0x8)],
            texwrap_tbl[*(u8*)((s32)texSlot + 0x9)],
            mipmap
        );

        GXInitTexObjLOD(
            texObj,
            1,
            1,
            (f32)*(u8*)((s32)texHeader + 0x21),
            (f32)*(u8*)((s32)texHeader + 0x22),
            *(f32*)((s32)texHeader + 0x1C),
            0,
            *(u8*)((s32)texHeader + 0x20),
            0
        );

        GXLoadTexObj(texObj, loadedTexCount);

        loadedTexCount++;
        texIndex--;
    }

    texCount = *(u8*)((s32)param_2 + 0xB);

    drawModePtr = *(void**)((s32)param_2 + 0x110);
    if (drawModePtr != 0) {
        drawMode = *(u8*)drawModePtr;
    } else {
        drawMode = 0;
    }

    materialFlag = *(u32*)((s32)param_1 + 0x0);
    pMtx = (s32)param_1 + 0x1C;

    mapSetMaterialTev(texCount, drawMode, materialFlag, pMtx);
}

void mapSetPaperAmbColor(void* color) {
    extern s32 paper_ambient;
    paper_ambient = *(s32*)color;
}

void mapResetPaperAmbColor(void) {
    extern s32 dat_8041f90c;
    extern s32 paper_ambient;
    paper_ambient = dat_8041f90c;
}

u8 mapSetMaterialLight(u32 materialLightFlag, void* pPos) {
    extern void* lightGetPaper(void);
    extern void* lightGetPaperCraft(void);
    extern s32 lightCheckCharaLight(void);
    extern s32 lightGetNearObj(void* pPos, void** pDst, s32 count, s32 flags);
    extern void* lightGetCharaAmbient(void);
    extern void* camGetCurPtr(void);

    extern void PSMTXMultVec(void* mtx, void* src, void* dst);
    extern void PSMTXMultVecSR(void* mtx, void* src, void* dst);
    extern void PSMTXRotRad(void* mtx, s32 axis, f32 rad);
    extern void PSMTXConcat(void* a, void* b, void* out);
    extern void PSVECScale(void* src, void* dst, f32 scale);
    extern void PSVECAdd(void* a, void* b, void* out);

    extern void GXInitLightPos(void* light, f32 x, f32 y, f32 z);
    extern void GXInitLightDir(void* light, f32 x, f32 y, f32 z);
    extern void GXInitLightSpot(void* light, f32 angle, s32 fn);
    extern void GXInitLightDistAttn(void* light, f32 refDistance, f32 refBrightness, s32 fn);
    extern void GXInitLightColor(void* light, void* color);
    extern void GXInitLightAttn(void* light, f32 a0, f32 a1, f32 a2, f32 k0, f32 k1, f32 k2);
    extern void GXLoadLightObjImm(void* light, u32 id);
    extern void GXSetFog(s32 type, f32 start, f32 end, f32 nearZ, f32 farZ, void* color);
    extern void GXSetNumChans(u32 nChans);
    extern void GXSetChanAmbColor(s32 chan, void* color);
    extern void GXSetChanMatColor(s32 chan, void* color);
    extern void GXSetChanCtrl(s32 chan, s32 enable, s32 ambSrc, s32 matSrc, u32 lightMask, u32 diffFn, s32 attnFn);

    extern u32 dat_8041f914;
    extern u32 dat_8041f918;
    extern u32 dat_8041f91c;
    extern u32 paper_ambient;
    extern u32 paperCraft_ambient;
    extern u32 lightid_tbl[];
    extern u32 vec3_802bfaa8[];
    extern f32 float_0_8041f930;
    extern f32 float_1_8041f940;
    extern f32 float_deg2rad_8041f948;
    extern f32 float_neg1p0486E06_8041f950;
    extern f32 float_0p5_8041f934;
    extern f32 float_1000_8041f954;

    u32 matColor;
    u32 ambColor;
    u32 charAmbColor;
    u32 fogColor;
    u32 tmpColor;
    f32 dir[3];
    f32 pos[3];
    void* lights[8];
    f32 mtxZ[3][4];
    f32 mtxY[3][4];
    f32 mtxX[3][4];
    u32 lightObj[16];

    u32 paperFlag;
    u32 craftFlag;
    u32 lightMask;
    u32* lightId;
    void** lightEntryPtr;
    void* light;
    void* cam;
    u16 flags;
    s32 lightNum;
    s32 i;
    s32 attnFn;
    s32 enable;

#define F32_AT(p_, o_) (*(f32*)((s32)(p_) + (o_)))
#define U16_AT(p_, o_) (*(u16*)((s32)(p_) + (o_)))
#define U32_AT(p_, o_) (*(u32*)((s32)(p_) + (o_)))
#define BUILD_DIR(light_)                                                        \
    do {                                                                        \
        PSMTXRotRad(mtxX, 'x', float_deg2rad_8041f948 * F32_AT((light_), 0x30)); \
        PSMTXRotRad(mtxY, 'y', float_deg2rad_8041f948 * F32_AT((light_), 0x34)); \
        PSMTXRotRad(mtxZ, 'z', float_deg2rad_8041f948 * F32_AT((light_), 0x38)); \
        PSMTXConcat(mtxZ, mtxY, mtxY);                                           \
        PSMTXConcat(mtxY, mtxX, mtxX);                                           \
        PSMTXMultVec(mtxX, dir, dir);                                            \
    } while (0)

    paperFlag = materialLightFlag & 1;
    attnFn = 2;
    matColor = dat_8041f914;
    ambColor = dat_8041f918;

    if (paperFlag != 0) {
        lights[0] = lightGetPaper();
    }
    lightNum = (paperFlag != 0);

    craftFlag = materialLightFlag & 8;
    if (craftFlag != 0) {
        lights[lightNum] = lightGetPaperCraft();
        lightNum++;
    }

    if ((materialLightFlag & 0x10) != 0) {
        lights[lightNum] = lightGetPaper();
        lightNum++;
    }

    if ((materialLightFlag & 2) != 0) {
        if (lightCheckCharaLight() != 0) {
            if (lightNum < 8) {
                lightNum += lightGetNearObj(pPos, &lights[lightNum], 8 - lightNum, 0x80);
            }
        }
    }

    if ((materialLightFlag & 0x19) != 0) {
        if (lightNum == 1) {
            if (paperFlag != 0) {
                ambColor = paper_ambient;
            } else if (craftFlag != 0) {
                ambColor = paperCraft_ambient;
            } else {
                ambColor = dat_8041f91c;
            }
        } else {
            lightNum = lightGetNearObj(pPos, lights, 8, 0x80);
        }
    }

    lightId = lightid_tbl;
    lightEntryPtr = lights;
    lightMask = 0;
    i = 0;

    while (i < lightNum) {
        ((u32*)dir)[0] = vec3_802bfaa8[0];
        light = *lightEntryPtr;
        ((u32*)dir)[1] = vec3_802bfaa8[1];
        ((u32*)dir)[2] = vec3_802bfaa8[2];

        flags = U16_AT(light, 0);
        if ((flags & 0x1000) != 0) {
            cam = camGetCurPtr();
            PSMTXMultVec((void*)((s32)cam + 0x11C), (void*)((s32)light + 0x24), pos);

            GXInitLightPos(lightObj, pos[0], pos[1], pos[2]);
            tmpColor = U32_AT(light, 0x48);
            GXInitLightColor(lightObj, &tmpColor);

            if ((U16_AT(light, 0) & 0x40) != 0) {
                GXInitLightAttn(lightObj, F32_AT(light, 0x50), float_0_8041f930, float_0_8041f930,
                                float_1_8041f940, float_0_8041f930, float_0_8041f930);
            } else {
                GXInitLightAttn(lightObj, F32_AT(light, 0x50), float_0_8041f930, float_0_8041f930,
                                F32_AT(light, 0x54), F32_AT(light, 0x58), F32_AT(light, 0x5C));
                attnFn = 1;
            }

            GXLoadLightObjImm(lightObj, *lightId);
        } else if ((flags & 0x2000) != 0) {
            BUILD_DIR(light);
            PSVECScale(dir, dir, float_neg1p0486E06_8041f950);
            PSVECAdd((void*)((s32)light + 0x24), dir, pos);

            cam = camGetCurPtr();
            PSMTXMultVec((void*)((s32)cam + 0x11C), pos, pos);

            GXInitLightPos(lightObj, pos[0], pos[1], pos[2]);
            tmpColor = U32_AT(light, 0x48);
            GXInitLightColor(lightObj, &tmpColor);
            GXLoadLightObjImm(lightObj, *lightId);
        } else {
            if ((flags & 0x4000) != 0) {
                cam = camGetCurPtr();
                PSMTXMultVec((void*)((s32)cam + 0x11C), (void*)((s32)light + 0x24), pos);

                BUILD_DIR(light);

                cam = camGetCurPtr();
                PSMTXMultVecSR((void*)((s32)cam + 0x11C), dir, dir);

                GXInitLightPos(lightObj, pos[0], pos[1], pos[2]);
                GXInitLightDir(lightObj, dir[0], dir[1], dir[2]);
                GXInitLightSpot(lightObj, F32_AT(light, 0x4C) * float_0p5_8041f934, 4);
                GXInitLightDistAttn(lightObj, float_1000_8041f954, float_0p5_8041f934, 0);

                tmpColor = U32_AT(light, 0x48);
                GXInitLightColor(lightObj, &tmpColor);
                GXLoadLightObjImm(lightObj, *lightId);
                attnFn = 1;
            }
        }

        lightMask |= *lightId;
        lightEntryPtr++;
        lightId++;
        i++;
    }

    light = lightGetCharaAmbient();
    if (light == 0) {
        charAmbColor = ambColor;
    } else {
        charAmbColor = U32_AT(light, 0x48);
    }

    fogColor = matColor;
    GXSetFog(0, float_0_8041f930, float_0_8041f930, float_0_8041f930, float_0_8041f930, &fogColor);
    GXSetNumChans(1);

    tmpColor = charAmbColor;
    GXSetChanAmbColor(4, &tmpColor);

    enable = ((u32)(-lightNum) & ~((u32)lightNum)) >> 31;
    if ((materialLightFlag & 4) != 0) {
        tmpColor = matColor;
        GXSetChanMatColor(4, &tmpColor);
        GXSetChanCtrl(0, enable, 0, 0, lightMask, 2, attnFn);
        GXSetChanCtrl(2, 0, 0, 0, 0, 2, 2);
    } else {
        GXSetChanCtrl(0, enable, 0, 1, lightMask, 2, attnFn);
        GXSetChanCtrl(2, 0, 0, 1, 0, 2, 2);
    }

#undef F32_AT
#undef U16_AT
#undef U32_AT
#undef BUILD_DIR
}

void mapSetPolygon(int param_1, int param_2) {
    extern s32 activeGroup;
    extern s32 mapWork;
    extern s32 culling[];
    extern s32 attr_fmt[];
    extern void GXSetCullMode(s32 mode);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxAttrFmtv(s32 vtxfmt, void* fmtv);
    extern void GXSetVtxDesc(s32 attr, s32 type);
    extern void GXSetArray(s32 attr, void* base, s32 stride);
    extern void GXBegin(s32 prim, s32 vtxfmt, s32 nverts);
    extern void GXSetVtxAttrFmt(s32 vtxfmt, s32 attr, s32 cnt, s32 type, s32 frac);
    extern void GXCallDisplayList(void* list, u32 nbytes);

    s32 obj;
    s32 poly;
    s32 groupBase;
    s32 texCount;
    s32 posArray;
    s32 nrmArray;
    s32 flags;
    s32 i;
    s32 offset;
    s32 listCursor;
    s32 prim;
    s32 vtx;
    s32 j;
    s32 k;
    s32 q;
    s32 idx;
    s32 mask;
    s32 texPtr;
    volatile u16* fifo;

#define S16_AT(p_, o_) (*(s16*)((s32)(p_) + (o_)))
#define S32_AT(p_, o_) (*(s32*)((s32)(p_) + (o_)))
#define U32_AT(p_, o_) (*(u32*)((s32)(p_) + (o_)))
#define ARRAYS() S32_AT(poly, 0x0C)
#define ENTRY() (groupBase + S16_AT(obj, 0xDE) * 0x178)
#define ARRAY_PTR(off_) ((void*)(S32_AT(ARRAYS(), (off_)) + 4))
#define SAFE_IDX(v_) ((u16)(v_) & (s32)(((v_) + 1) | (-(v_) - 1)) >> 31)

    obj = param_1;
    poly = param_2;
    groupBase = mapWork + activeGroup * 0x2F4;
    posArray = S32_AT(ARRAYS(), 0x00) + 4;
    nrmArray = S32_AT(ARRAYS(), 0x04) + 4;
    texCount = S32_AT(ARRAYS(), 0x14);

    GXSetCullMode(culling[*(u8*)(S32_AT(S32_AT(obj, 0x08), 0x58) + 1)]);

    if ((*(u16*)(ENTRY() + 4) & 0x8000) == 0) {
        GXClearVtxDesc();
        GXSetVtxAttrFmtv(0, attr_fmt);

        GXSetVtxDesc(9, 3);
        GXSetArray(9, (void*)posArray, 0xC);

        GXSetVtxDesc(0xA, 3);
        GXSetArray(0xA, (void*)nrmArray, 0xC);

        GXSetVtxDesc(0xB, 3);
        GXSetArray(0xB, ARRAY_PTR(0x0C), 4);

        offset = 0;
        for (i = 0; i < texCount; i++, offset += 4) {
            GXSetVtxDesc(i + 0xD, 3);
            GXSetArray(i + 0xD, ARRAY_PTR(offset + 0x18), 8);
        }

        fifo = (volatile u16*)0xCC008000;
        listCursor = poly;
        for (i = 0; i < S32_AT(poly, 4); i++) {
            prim = S32_AT(listCursor, 0x10);
            GXBegin(0x98, 0, (u16)S32_AT(prim, 0));

            vtx = prim;
            j = 0;
            while (j < S32_AT(prim, 0)) {
                *fifo = S16_AT(vtx, 4);
                *fifo = S16_AT(vtx, 6);
                *fifo = S16_AT(vtx, 8);

                if (texCount > 0) {
                    k = texCount;
                    texPtr = vtx + 4;

                    q = (u32)k >> 2;
                    if (q != 0) {
                        do {
                            idx = S16_AT(texPtr, 8);
                            *fifo = SAFE_IDX(idx);

                            idx = S16_AT(texPtr, 0xA);
                            *fifo = SAFE_IDX(idx);

                            idx = S16_AT(texPtr, 0xC);
                            *fifo = SAFE_IDX(idx);

                            idx = S16_AT(texPtr, 0xE);
                            *fifo = SAFE_IDX(idx);

                            texPtr += 8;
                            q--;
                        } while (q != 0);
                    }

                    k &= 3;
                    if (k != 0) {
                        do {
                            idx = S16_AT(texPtr, 8);
                            texPtr += 2;
                            *fifo = SAFE_IDX(idx);
                            k--;
                        } while (k != 0);
                    }
                }

                vtx += 0x18;
                j++;
            }

            listCursor += 4;
        }
    } else {
        flags = S32_AT(poly, 8);

        GXClearVtxDesc();

        GXSetVtxDesc(9, 3);
        GXSetVtxAttrFmt(0, 9, 1, 3, U32_AT(S32_AT(ENTRY(), 0xA4), 0x44) & 0xFF);
        GXSetArray(9, (void*)posArray, 6);

        GXSetVtxDesc(0xA, 3);
        GXSetVtxAttrFmt(0, 0xA, 0, 1, 6);
        GXSetArray(0xA, (void*)nrmArray, 3);

        offset = 0;
        i = 0;
        while (i < 2) {
            mask = 1 << (i + 2);
            if ((flags & mask) == 0) {
                break;
            }

            GXSetVtxDesc(0xB, 3);
            GXSetVtxAttrFmt(0, 0xB, 1, 5, 0);
            GXSetArray(0xB, ARRAY_PTR(offset + 0x0C), 4);

            i++;
            offset += 4;
        }

        offset = 0;
        i = 0;
        while (i < 8) {
            mask = 1 << (i + 4);
            if ((flags & mask) == 0) {
                break;
            }

            GXSetVtxDesc(i + 0xD, 3);
            GXSetVtxAttrFmt(0, i + 0xD, 1, 3, U32_AT(S32_AT(ENTRY(), 0xA4), offset + 0x48) & 0xFF);
            GXSetArray(i + 0xD, ARRAY_PTR(offset + 0x18), 4);

            i++;
            offset += 4;
        }

        listCursor = poly;
        for (i = 0; i < S32_AT(poly, 4); i++) {
            GXCallDisplayList((void*)S32_AT(listCursor, 0x10), U32_AT(listCursor, 0x14));
            listCursor += 8;
        }
    }

#undef S16_AT
#undef S32_AT
#undef U32_AT
#undef ARRAYS
#undef ENTRY
#undef ARRAY_PTR
#undef SAFE_IDX
}

void mapSetPolygonVtxDesc(int obj, int poly) {
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32 attr, s32 type);
    extern void GXSetVtxAttrFmt(s32 vtxfmt, s32 attr, s32 cnt, s32 type, s32 frac);
    extern void GXSetArray(s32 attr, void* base, s32 stride);
    extern s32 activeGroup;
    extern void* mapWork;
    void* group;
    void* vcd;
    void* arrays;
    u32 flags;
    s32 i;
    s32 offset;

    arrays = *(void**)((s32)poly + 0xC);
    flags = *(u32*)((s32)poly + 0x8);
    group = (void*)((s32)mapWork + activeGroup * 0x2F4);

    GXClearVtxDesc();
    GXSetVtxDesc(9, 3);
    vcd = *(void**)((s32)group + (*(s16*)((s32)obj + 0xDE) * 0x178) + 0xA4);
    GXSetVtxAttrFmt(0, 9, 1, 3, *(u32*)((s32)vcd + 0x44) & 0xFF);
    GXSetArray(9, (void*)(*(s32*)arrays + 4), 6);

    GXSetVtxDesc(0xA, 3);
    GXSetVtxAttrFmt(0, 0xA, 0, 1, 6);
    GXSetArray(0xA, (void*)(*(s32*)((s32)arrays + 4) + 4), 3);

    offset = 0;
    for (i = 0; i < 2; i++, offset += 4) {
        if ((flags & (1 << (i + 2))) == 0) {
            break;
        }
        GXSetVtxDesc(0xB, 3);
        GXSetVtxAttrFmt(0, 0xB, 1, 5, 0);
        GXSetArray(0xB, (void*)(*(s32*)((s32)arrays + offset + 0xC) + 4), 4);
    }

    offset = 0;
    for (i = 0; i < 8; i++, offset += 4) {
        if ((flags & (1 << (i + 4))) == 0) {
            break;
        }
        GXSetVtxDesc(i + 0xD, 3);
        vcd = *(void**)((s32)group + (*(s16*)((s32)obj + 0xDE) * 0x178) + 0xA4);
        GXSetVtxAttrFmt(0, i + 0xD, 1, 3, *(u32*)((s32)vcd + offset + 0x48) & 0xFF);
        GXSetArray(i + 0xD, (void*)(*(s32*)((s32)arrays + offset + 0x18) + 4), 4);
    }
}

void* mapSearchDmdJointSub(void* param_1, char* param_2) {
    extern s32 strcmp(const char* a, const char* b);

    void* pMVar5;
    char* target;
    void* root;
    void* pMVar4;
    void* pMVar3;
    void* pMVar2;

#define JOINT_NAME(j_) (*(char**)((s32)(j_) + 0x00))
#define JOINT_CHILD(j_) (*(void**)((s32)(j_) + 0x0C))
#define JOINT_SIB(j_) (*(void**)((s32)(j_) + 0x10))

    root = param_1;
    target = param_2;

    if (strcmp(JOINT_NAME(root), target) == 0) {
        return root;
    }

    pMVar5 = JOINT_CHILD(root);
    if (pMVar5 != 0) {
        if (strcmp(JOINT_NAME(pMVar5), target) != 0) {
            pMVar4 = JOINT_CHILD(pMVar5);
            if (pMVar4 != 0) {
                if (strcmp(JOINT_NAME(pMVar4), target) != 0) {
                    pMVar3 = JOINT_CHILD(pMVar4);
                    if (pMVar3 != 0) {
                        if (strcmp(JOINT_NAME(pMVar3), target) != 0) {
                            if (JOINT_CHILD(pMVar3) != 0) {
                                pMVar2 = mapSearchDmdJointSub(JOINT_CHILD(pMVar3), target);
                                if (pMVar2 != 0) {
                                    pMVar3 = pMVar2;
                                    goto first_grand_done;
                                }
                            }
                            if (JOINT_SIB(pMVar3) != 0) {
                                pMVar2 = mapSearchDmdJointSub(JOINT_SIB(pMVar3), target);
                                if (pMVar2 != 0) {
                                    pMVar3 = pMVar2;
                                    goto first_grand_done;
                                }
                            }
                            pMVar3 = 0;
                        }

first_grand_done:
                        if (pMVar3 != 0) {
                            pMVar4 = pMVar3;
                            goto first_child_done;
                        }
                    }

                    pMVar4 = JOINT_SIB(pMVar4);
                    if (pMVar4 != 0) {
                        if (strcmp(JOINT_NAME(pMVar4), target) != 0) {
                            if (JOINT_CHILD(pMVar4) != 0) {
                                pMVar2 = mapSearchDmdJointSub(JOINT_CHILD(pMVar4), target);
                                if (pMVar2 != 0) {
                                    pMVar4 = pMVar2;
                                    goto first_child_sib_done;
                                }
                            }
                            if (JOINT_SIB(pMVar4) != 0) {
                                pMVar2 = mapSearchDmdJointSub(JOINT_SIB(pMVar4), target);
                                if (pMVar2 != 0) {
                                    pMVar4 = pMVar2;
                                    goto first_child_sib_done;
                                }
                            }
                            pMVar4 = 0;
                        }

first_child_sib_done:
                        if (pMVar4 != 0) {
                            goto first_child_done;
                        }
                    }
                    pMVar4 = 0;
                }

first_child_done:
                if (pMVar4 != 0) {
                    pMVar5 = pMVar4;
                    goto first_top_done;
                }
            }

            pMVar5 = JOINT_SIB(pMVar5);
            if (pMVar5 != 0) {
                if (strcmp(JOINT_NAME(pMVar5), target) != 0) {
                    pMVar4 = JOINT_CHILD(pMVar5);
                    if (pMVar4 != 0) {
                        if (strcmp(JOINT_NAME(pMVar4), target) != 0) {
                            if (JOINT_CHILD(pMVar4) != 0) {
                                pMVar2 = mapSearchDmdJointSub(JOINT_CHILD(pMVar4), target);
                                if (pMVar2 != 0) {
                                    pMVar4 = pMVar2;
                                    goto first_top_sib_child_done;
                                }
                            }
                            if (JOINT_SIB(pMVar4) != 0) {
                                pMVar2 = mapSearchDmdJointSub(JOINT_SIB(pMVar4), target);
                                if (pMVar2 != 0) {
                                    pMVar4 = pMVar2;
                                    goto first_top_sib_child_done;
                                }
                            }
                            pMVar4 = 0;
                        }

first_top_sib_child_done:
                        if (pMVar4 != 0) {
                            pMVar5 = pMVar4;
                            goto first_top_sib_done;
                        }
                    }

                    pMVar5 = JOINT_SIB(pMVar5);
                    if (pMVar5 != 0) {
                        if (strcmp(JOINT_NAME(pMVar5), target) != 0) {
                            if (JOINT_CHILD(pMVar5) != 0) {
                                pMVar2 = mapSearchDmdJointSub(JOINT_CHILD(pMVar5), target);
                                if (pMVar2 != 0) {
                                    pMVar5 = pMVar2;
                                    goto first_top_sib_sib_done;
                                }
                            }
                            if (JOINT_SIB(pMVar5) != 0) {
                                pMVar2 = mapSearchDmdJointSub(JOINT_SIB(pMVar5), target);
                                if (pMVar2 != 0) {
                                    pMVar5 = pMVar2;
                                    goto first_top_sib_sib_done;
                                }
                            }
                            pMVar5 = 0;
                        }

first_top_sib_sib_done:
                        if (pMVar5 != 0) {
                            goto first_top_sib_done;
                        }
                    }
                    pMVar5 = 0;
                }

first_top_sib_done:
                if (pMVar5 != 0) {
                    goto first_top_done;
                }
            }
            pMVar5 = 0;
        }

first_top_done:
        if (pMVar5 != 0) {
            return pMVar5;
        }
    }

    pMVar5 = JOINT_SIB(root);
    if (pMVar5 == 0) {
        return 0;
    }

    if (strcmp(JOINT_NAME(pMVar5), target) != 0) {
        pMVar4 = JOINT_CHILD(pMVar5);
        if (pMVar4 != 0) {
            if (strcmp(JOINT_NAME(pMVar4), target) != 0) {
                pMVar3 = JOINT_CHILD(pMVar4);
                if (pMVar3 != 0) {
                    if (strcmp(JOINT_NAME(pMVar3), target) != 0) {
                        if (JOINT_CHILD(pMVar3) != 0) {
                            pMVar2 = mapSearchDmdJointSub(JOINT_CHILD(pMVar3), target);
                            if (pMVar2 != 0) {
                                pMVar3 = pMVar2;
                                goto sib_grand_done;
                            }
                        }
                        if (JOINT_SIB(pMVar3) != 0) {
                            pMVar2 = mapSearchDmdJointSub(JOINT_SIB(pMVar3), target);
                            if (pMVar2 != 0) {
                                pMVar3 = pMVar2;
                                goto sib_grand_done;
                            }
                        }
                        pMVar3 = 0;
                    }

sib_grand_done:
                    if (pMVar3 != 0) {
                        pMVar4 = pMVar3;
                        goto sib_child_done;
                    }
                }

                pMVar4 = JOINT_SIB(pMVar4);
                if (pMVar4 != 0) {
                    if (strcmp(JOINT_NAME(pMVar4), target) != 0) {
                        if (JOINT_CHILD(pMVar4) != 0) {
                            pMVar2 = mapSearchDmdJointSub(JOINT_CHILD(pMVar4), target);
                            if (pMVar2 != 0) {
                                pMVar4 = pMVar2;
                                goto sib_child_sib_done;
                            }
                        }
                        if (JOINT_SIB(pMVar4) != 0) {
                            pMVar2 = mapSearchDmdJointSub(JOINT_SIB(pMVar4), target);
                            if (pMVar2 != 0) {
                                pMVar4 = pMVar2;
                                goto sib_child_sib_done;
                            }
                        }
                        pMVar4 = 0;
                    }

sib_child_sib_done:
                    if (pMVar4 != 0) {
                        goto sib_child_done;
                    }
                }
                pMVar4 = 0;
            }

sib_child_done:
            if (pMVar4 != 0) {
                pMVar5 = pMVar4;
                goto sib_done;
            }
        }

        pMVar5 = JOINT_SIB(pMVar5);
        if (pMVar5 != 0) {
            if (strcmp(JOINT_NAME(pMVar5), target) != 0) {
                pMVar4 = JOINT_CHILD(pMVar5);
                if (pMVar4 != 0) {
                    if (strcmp(JOINT_NAME(pMVar4), target) != 0) {
                        if (JOINT_CHILD(pMVar4) != 0) {
                            pMVar2 = mapSearchDmdJointSub(JOINT_CHILD(pMVar4), target);
                            if (pMVar2 != 0) {
                                pMVar4 = pMVar2;
                                goto sib_sib_child_done;
                            }
                        }
                        if (JOINT_SIB(pMVar4) != 0) {
                            pMVar2 = mapSearchDmdJointSub(JOINT_SIB(pMVar4), target);
                            if (pMVar2 != 0) {
                                pMVar4 = pMVar2;
                                goto sib_sib_child_done;
                            }
                        }
                        pMVar4 = 0;
                    }

sib_sib_child_done:
                    if (pMVar4 != 0) {
                        pMVar5 = pMVar4;
                        goto sib_sib_done;
                    }
                }

                pMVar5 = JOINT_SIB(pMVar5);
                if (pMVar5 != 0) {
                    if (strcmp(JOINT_NAME(pMVar5), target) != 0) {
                        if (JOINT_CHILD(pMVar5) != 0) {
                            pMVar2 = mapSearchDmdJointSub(JOINT_CHILD(pMVar5), target);
                            if (pMVar2 != 0) {
                                pMVar5 = pMVar2;
                                goto sib_sib_sib_done;
                            }
                        }
                        if (JOINT_SIB(pMVar5) != 0) {
                            pMVar2 = mapSearchDmdJointSub(JOINT_SIB(pMVar5), target);
                            if (pMVar2 != 0) {
                                pMVar5 = pMVar2;
                                goto sib_sib_sib_done;
                            }
                        }
                        pMVar5 = 0;
                    }

sib_sib_sib_done:
                    if (pMVar5 != 0) {
                        goto sib_sib_done;
                    }
                }
                pMVar5 = 0;
            }

sib_sib_done:
            if (pMVar5 != 0) {
                goto sib_done;
            }
        }
        pMVar5 = 0;
    }

sib_done:
    if (pMVar5 == 0) {
        return 0;
    }
    return pMVar5;

#undef JOINT_NAME
#undef JOINT_CHILD
#undef JOINT_SIB
}

void* mapSearchDmdJointSub2(void* param_1, char* param_2, char* param_3) {
    extern char* strcpy(char* dst, const char* src);
    extern char* strcat(char* dst, const char* src);
    extern s32 strcmp(const char* a, const char* b);
    extern char str__8041f94c[1];

    void* node;
    char* prefix;
    char* target;
    void* cur;
    void* sub;
    char childBuf[256];
    char topSibBuf[256];
    char grandChildBuf[256];
    char childSibBuf[256];
    char topSibChildBuf[256];
    char topSibSibBuf[256];
    char rootBuf[260];

#define JOINT_NAME(j_) (*(char**)((s32)(j_) + 0x00))
#define JOINT_CHILD(j_) (*(void**)((s32)(j_) + 0x0C))
#define JOINT_SIB(j_) (*(void**)((s32)(j_) + 0x10))

    node = param_1;
    prefix = param_3;
    target = param_2;

    strcpy(rootBuf, prefix);
    strcat(rootBuf, JOINT_NAME(node));
    if (strcmp(rootBuf, target) == 0) {
        return node;
    }

    if (JOINT_CHILD(node) != NULL) {
        strcat(rootBuf, str__8041f94c);

        cur = JOINT_CHILD(node);
        strcpy(childBuf, rootBuf);
        strcat(childBuf, JOINT_NAME(cur));

        if (strcmp(childBuf, target) != 0) {
            if (JOINT_CHILD(cur) != NULL) {
                strcat(childBuf, str__8041f94c);

                sub = JOINT_CHILD(cur);
                strcpy(grandChildBuf, childBuf);
                strcat(grandChildBuf, JOINT_NAME(sub));

                if (strcmp(grandChildBuf, target) != 0) {
                    if (JOINT_CHILD(sub) != NULL) {
                        strcat(grandChildBuf, str__8041f94c);
                        sub = mapSearchDmdJointSub2(JOINT_CHILD(sub), target, grandChildBuf);
                        if (sub != NULL) {
                            goto first_grand_done;
                        }
                    }

                    if (JOINT_SIB(sub) != NULL) {
                        sub = mapSearchDmdJointSub2(JOINT_SIB(sub), target, childBuf);
                        if (sub != NULL) {
                            goto first_grand_done;
                        }
                    }

                    sub = NULL;
                }

first_grand_done:
                if (sub != NULL) {
                    cur = sub;
                    goto first_child_done;
                }
            }

            cur = JOINT_SIB(cur);
            if (cur != NULL) {
                strcpy(childSibBuf, rootBuf);
                strcat(childSibBuf, JOINT_NAME(cur));

                if (strcmp(childSibBuf, target) != 0) {
                    if (JOINT_CHILD(cur) != NULL) {
                        strcat(childSibBuf, str__8041f94c);
                        sub = mapSearchDmdJointSub2(JOINT_CHILD(cur), target, childSibBuf);
                        if (sub != NULL) {
                            cur = sub;
                            goto first_child_done;
                        }
                    }

                    if (JOINT_SIB(cur) != NULL) {
                        sub = mapSearchDmdJointSub2(JOINT_SIB(cur), target, rootBuf);
                        if (sub != NULL) {
                            cur = sub;
                            goto first_child_done;
                        }
                    }

                    cur = NULL;
                }
            } else {
                cur = NULL;
            }
        }

first_child_done:
        if (cur != NULL) {
            return cur;
        }
    }

    cur = JOINT_SIB(node);
    if (cur == NULL) {
        return NULL;
    }

    strcpy(topSibBuf, prefix);
    strcat(topSibBuf, JOINT_NAME(cur));

    if (strcmp(topSibBuf, target) != 0) {
        if (JOINT_CHILD(cur) != NULL) {
            strcat(topSibBuf, str__8041f94c);

            sub = JOINT_CHILD(cur);
            strcpy(topSibChildBuf, topSibBuf);
            strcat(topSibChildBuf, JOINT_NAME(sub));

            if (strcmp(topSibChildBuf, target) != 0) {
                if (JOINT_CHILD(sub) != NULL) {
                    strcat(topSibChildBuf, str__8041f94c);
                    sub = mapSearchDmdJointSub2(JOINT_CHILD(sub), target, topSibChildBuf);
                    if (sub != NULL) {
                        goto top_sib_child_done;
                    }
                }

                if (JOINT_SIB(sub) != NULL) {
                    sub = mapSearchDmdJointSub2(JOINT_SIB(sub), target, topSibBuf);
                    if (sub != NULL) {
                        goto top_sib_child_done;
                    }
                }

                sub = NULL;
            }

top_sib_child_done:
            if (sub != NULL) {
                cur = sub;
                goto top_sib_done;
            }
        }

        cur = JOINT_SIB(cur);
        if (cur != NULL) {
            strcpy(topSibSibBuf, prefix);
            strcat(topSibSibBuf, JOINT_NAME(cur));

            if (strcmp(topSibSibBuf, target) != 0) {
                if (JOINT_CHILD(cur) != NULL) {
                    strcat(topSibSibBuf, str__8041f94c);
                    sub = mapSearchDmdJointSub2(JOINT_CHILD(cur), target, topSibSibBuf);
                    if (sub != NULL) {
                        cur = sub;
                        goto top_sib_done;
                    }
                }

                if (JOINT_SIB(cur) != NULL) {
                    sub = mapSearchDmdJointSub2(JOINT_SIB(cur), target, prefix);
                    if (sub != NULL) {
                        cur = sub;
                        goto top_sib_done;
                    }
                }

                cur = NULL;
            }
        } else {
            cur = NULL;
        }
    }

top_sib_done:
    if (cur == NULL) {
        return NULL;
    }
    return cur;

#undef JOINT_NAME
#undef JOINT_CHILD
#undef JOINT_SIB
}

void* mapSearchDmdJoint(void* data, char* name) {
    extern void* mapSearchDmdJointSub(void*, char*);
    extern void* mapSearchDmdJointSub2(void*, char*, char*);
    extern char str__8041f94c[1];
    if ((s32)*(u8*)name != '|') {
        return mapSearchDmdJointSub(data, name);
    }
    return mapSearchDmdJointSub2(data, name, str__8041f94c);
}

void* mapGetMapObj(s32 name) {
    extern s32 activeGroup;
    extern s32 mapWork;
    extern s32 strcmp(const char*, const char*);
    void* work;
    void* group;
    void* obj;
    s32 i;
    s32 j;

    work = (void*)(mapWork + activeGroup * 0x2F4);
    if (name == 0) {
        return 0;
    }

    group = work;
    i = 0;
    while (i < *(s32*)work) {
        obj = *(void**)((s32)group + 0x154);
        j = 0;
        while (j < *(s32*)((s32)group + 0x150)) {
            if (strcmp(*(const char**)*(s32*)((s32)obj + 0x8), (const char*)name) == 0) {
                return obj;
            }
            j++;
            obj = (void*)((s32)obj + 0x134);
        }
        group = (void*)((s32)group + 0x178);
        i++;
    }

    return 0;
}

u8 mapObjFlagOn(char* param_1, u32 param_2) {
    extern s32 activeGroup;
    extern s32 mapWork;
    extern s32 strcmp(const char*, const char*);
    extern char* strncpy(char* dst, const char* src, u32 n);
    extern char error_data[];
    extern s32 error_count;
    extern s32 error_flag;

    void* groupBase;
    void* group;
    void* obj;
    s32 groupIndex;
    s32 objIndex;
    s32 errorOffset;
    char* entry;

    groupBase = (void*)(mapWork + activeGroup * 0x2F4);

    if (param_1 == NULL) {
        obj = NULL;
        goto done;
    }

    group = groupBase;
    groupIndex = 0;
    while (groupIndex < *(s32*)groupBase) {
        obj = *(void**)((s32)group + 0x154);
        objIndex = 0;
        while (objIndex < *(s32*)((s32)group + 0x150)) {
            if (strcmp(*(const char**)*(s32*)((s32)obj + 0x08), param_1) != 0) {
                objIndex++;
                obj = (void*)((s32)obj + 0x134);
            } else {
                goto done;
            }
        }
        group = (void*)((s32)group + 0x178);
        groupIndex++;
    }

    obj = NULL;

done:
    if (obj == NULL) {
        errorOffset = error_count * 0x24;
        entry = error_data + errorOffset;
        *(s32*)(entry + 0x20) = 0;
        strncpy(entry, param_1, 0x1F);
        error_flag = 1;
        error_count++;
        error_count %= 32;
    } else {
        if ((param_2 & 0x100) == 0 || *(void**)((s32)obj + 0x114) != NULL) {
            *(u32*)obj = *(u32*)obj | param_2;
        }
    }
}

u8 mapObjFlagOff(char* param_1, u32 param_2) {
    extern s32 activeGroup;
    extern s32 mapWork;
    extern s32 strcmp(const char*, const char*);
    extern char* strncpy(char* dst, const char* src, u32 n);
    extern char error_data[];
    extern s32 error_count;
    extern s32 error_flag;

    void* groupBase;
    void* group;
    void* obj;
    s32 groupIndex;
    s32 objIndex;
    s32 errorOffset;
    char* entry;

    groupBase = (void*)(mapWork + activeGroup * 0x2F4);

    if (param_1 == NULL) {
        obj = NULL;
        goto done;
    }

    group = groupBase;
    groupIndex = 0;
    while (groupIndex < *(s32*)groupBase) {
        obj = *(void**)((s32)group + 0x154);
        objIndex = 0;
        while (objIndex < *(s32*)((s32)group + 0x150)) {
            if (strcmp(*(const char**)*(s32*)((s32)obj + 0x08), param_1) != 0) {
                objIndex++;
                obj = (void*)((s32)obj + 0x134);
            } else {
                goto done;
            }
        }
        group = (void*)((s32)group + 0x178);
        groupIndex++;
    }

    obj = NULL;

done:
    if (obj == NULL) {
        errorOffset = error_count * 0x24;
        entry = error_data + errorOffset;
        *(s32*)(entry + 0x20) = 0;
        strncpy(entry, param_1, 0x1F);
        error_flag = 1;
        error_count++;
        error_count %= 32;
    } else {
        *(u32*)obj = *(u32*)obj & ~param_2;
    }
}

void mapFlgOn(void* param_1, u32 param_2, int param_3) {
    u32 bit;
    void* p1;
    void* p2;
    void* p3;

    bit = param_2 & 0x100;

#define FLG_ON(node_)                                                   \
    do {                                                                \
        if (bit == 0 || *(void**)((s32)(node_) + 0x114) != NULL) {      \
            *(u32*)(node_) = *(u32*)(node_) | param_2;                  \
        }                                                               \
    } while (0)

    FLG_ON(param_1);

    p1 = *(void**)((s32)param_1 + 0xE4);
    if (p1 != 0) {
        FLG_ON(p1);

        p2 = *(void**)((s32)p1 + 0xE4);
        if (p2 != 0) {
            FLG_ON(p2);

            p3 = *(void**)((s32)p2 + 0xE4);
            if (p3 != 0) {
                FLG_ON(p3);

                if (*(void**)((s32)p3 + 0xE4) != 0) {
                    mapFlgOn(*(void**)((s32)p3 + 0xE4), param_2, 1);
                }
                if (*(void**)((s32)p3 + 0xE8) != 0) {
                    mapFlgOn(*(void**)((s32)p3 + 0xE8), param_2, 1);
                }
            }

            p3 = *(void**)((s32)p2 + 0xE8);
            if (p3 != 0) {
                FLG_ON(p3);

                if (*(void**)((s32)p3 + 0xE4) != 0) {
                    mapFlgOn(*(void**)((s32)p3 + 0xE4), param_2, 1);
                }
                if (*(void**)((s32)p3 + 0xE8) != 0) {
                    mapFlgOn(*(void**)((s32)p3 + 0xE8), param_2, 1);
                }
            }
        }

        p2 = *(void**)((s32)p1 + 0xE8);
        if (p2 != 0) {
            FLG_ON(p2);

            p3 = *(void**)((s32)p2 + 0xE4);
            if (p3 != 0) {
                FLG_ON(p3);

                if (*(void**)((s32)p3 + 0xE4) != 0) {
                    mapFlgOn(*(void**)((s32)p3 + 0xE4), param_2, 1);
                }
                if (*(void**)((s32)p3 + 0xE8) != 0) {
                    mapFlgOn(*(void**)((s32)p3 + 0xE8), param_2, 1);
                }
            }

            p2 = *(void**)((s32)p2 + 0xE8);
            if (p2 != 0) {
                FLG_ON(p2);

                if (*(void**)((s32)p2 + 0xE4) != 0) {
                    mapFlgOn(*(void**)((s32)p2 + 0xE4), param_2, 1);
                }
                if (*(void**)((s32)p2 + 0xE8) != 0) {
                    mapFlgOn(*(void**)((s32)p2 + 0xE8), param_2, 1);
                }
            }
        }
    }

    if (param_3 != 0) {
        p1 = *(void**)((s32)param_1 + 0xE8);
        if (p1 != 0) {
            FLG_ON(p1);

            p2 = *(void**)((s32)p1 + 0xE4);
            if (p2 != 0) {
                FLG_ON(p2);

                p3 = *(void**)((s32)p2 + 0xE4);
                if (p3 != 0) {
                    FLG_ON(p3);

                    if (*(void**)((s32)p3 + 0xE4) != 0) {
                        mapFlgOn(*(void**)((s32)p3 + 0xE4), param_2, 1);
                    }
                    if (*(void**)((s32)p3 + 0xE8) != 0) {
                        mapFlgOn(*(void**)((s32)p3 + 0xE8), param_2, 1);
                    }
                }

                p3 = *(void**)((s32)p2 + 0xE8);
                if (p3 != 0) {
                    FLG_ON(p3);

                    if (*(void**)((s32)p3 + 0xE4) != 0) {
                        mapFlgOn(*(void**)((s32)p3 + 0xE4), param_2, 1);
                    }
                    if (*(void**)((s32)p3 + 0xE8) != 0) {
                        mapFlgOn(*(void**)((s32)p3 + 0xE8), param_2, 1);
                    }
                }
            }

            p2 = *(void**)((s32)p1 + 0xE8);
            if (p2 != 0) {
                FLG_ON(p2);

                p3 = *(void**)((s32)p2 + 0xE4);
                if (p3 != 0) {
                    FLG_ON(p3);

                    if (*(void**)((s32)p3 + 0xE4) != 0) {
                        mapFlgOn(*(void**)((s32)p3 + 0xE4), param_2, 1);
                    }
                    if (*(void**)((s32)p3 + 0xE8) != 0) {
                        mapFlgOn(*(void**)((s32)p3 + 0xE8), param_2, 1);
                    }
                }

                p2 = *(void**)((s32)p2 + 0xE8);
                if (p2 != 0) {
                    FLG_ON(p2);

                    if (*(void**)((s32)p2 + 0xE4) != 0) {
                        mapFlgOn(*(void**)((s32)p2 + 0xE4), param_2, 1);
                    }
                    if (*(void**)((s32)p2 + 0xE8) != 0) {
                        mapFlgOn(*(void**)((s32)p2 + 0xE8), param_2, 1);
                    }
                }
            }
        }
    }

#undef FLG_ON
}

void mapFlgOff(u32* param_1, u32 param_2, int param_3) {
    u32* root;
    u32 flags;
    u32 mask;
    int recurse;
    u32* p1;
    u32* p2;
    u32* p3;

    root = param_1;
    flags = param_2;
    recurse = param_3;
    mask = ~flags;

#define FLG_OFF(node_)              \
    do {                            \
        *(node_) = *(node_) & mask; \
    } while (0)

    FLG_OFF(root);

    p1 = (u32*)root[0x39];
    if (p1 != 0) {
        FLG_OFF(p1);

        p2 = (u32*)p1[0x39];
        if (p2 != 0) {
            FLG_OFF(p2);

            p3 = (u32*)p2[0x39];
            if (p3 != 0) {
                FLG_OFF(p3);

                if ((u32*)p3[0x39] != 0) {
                    mapFlgOff((u32*)p3[0x39], flags, 1);
                }
                if ((u32*)p3[0x3A] != 0) {
                    mapFlgOff((u32*)p3[0x3A], flags, 1);
                }
            }

            p3 = (u32*)p2[0x3A];
            if (p3 != 0) {
                FLG_OFF(p3);

                if ((u32*)p3[0x39] != 0) {
                    mapFlgOff((u32*)p3[0x39], flags, 1);
                }
                if ((u32*)p3[0x3A] != 0) {
                    mapFlgOff((u32*)p3[0x3A], flags, 1);
                }
            }
        }

        p2 = (u32*)p1[0x3A];
        if (p2 != 0) {
            FLG_OFF(p2);

            p3 = (u32*)p2[0x39];
            if (p3 != 0) {
                FLG_OFF(p3);

                if ((u32*)p3[0x39] != 0) {
                    mapFlgOff((u32*)p3[0x39], flags, 1);
                }
                if ((u32*)p3[0x3A] != 0) {
                    mapFlgOff((u32*)p3[0x3A], flags, 1);
                }
            }

            p2 = (u32*)p2[0x3A];
            if (p2 != 0) {
                FLG_OFF(p2);

                if ((u32*)p2[0x39] != 0) {
                    mapFlgOff((u32*)p2[0x39], flags, 1);
                }
                if ((u32*)p2[0x3A] != 0) {
                    mapFlgOff((u32*)p2[0x3A], flags, 1);
                }
            }
        }
    }

    if (recurse != 0) {
        p1 = (u32*)root[0x3A];
        if (p1 != 0) {
            FLG_OFF(p1);

            p2 = (u32*)p1[0x39];
            if (p2 != 0) {
                FLG_OFF(p2);

                p3 = (u32*)p2[0x39];
                if (p3 != 0) {
                    FLG_OFF(p3);

                    if ((u32*)p3[0x39] != 0) {
                        mapFlgOff((u32*)p3[0x39], flags, 1);
                    }
                    if ((u32*)p3[0x3A] != 0) {
                        mapFlgOff((u32*)p3[0x3A], flags, 1);
                    }
                }

                p3 = (u32*)p2[0x3A];
                if (p3 != 0) {
                    FLG_OFF(p3);

                    if ((u32*)p3[0x39] != 0) {
                        mapFlgOff((u32*)p3[0x39], flags, 1);
                    }
                    if ((u32*)p3[0x3A] != 0) {
                        mapFlgOff((u32*)p3[0x3A], flags, 1);
                    }
                }
            }

            p2 = (u32*)p1[0x3A];
            if (p2 != 0) {
                FLG_OFF(p2);

                p3 = (u32*)p2[0x39];
                if (p3 != 0) {
                    FLG_OFF(p3);

                    if ((u32*)p3[0x39] != 0) {
                        mapFlgOff((u32*)p3[0x39], flags, 1);
                    }
                    if ((u32*)p3[0x3A] != 0) {
                        mapFlgOff((u32*)p3[0x3A], flags, 1);
                    }
                }

                p2 = (u32*)p2[0x3A];
                if (p2 != 0) {
                    FLG_OFF(p2);

                    if ((u32*)p2[0x39] != 0) {
                        mapFlgOff((u32*)p2[0x39], flags, 1);
                    }
                    if ((u32*)p2[0x3A] != 0) {
                        mapFlgOff((u32*)p2[0x3A], flags, 1);
                    }
                }
            }
        }
    }

#undef FLG_OFF
}

void mapGrpFlagOn(s32 param_1, u32 param_2) {
    extern s32 activeGroup;
    extern s32 mapWork;
    extern s32 strcmp(const char*, const char*);
    extern char* strncpy(char* dst, const char* src, u32 n);
    extern u8 mapFlgOn(u32* param_1, u32 param_2, int param_3);
    extern char error_data[];
    extern s32 error_count;
    extern s32 error_flag;

    void* groupBase;
    void* group;
    void* obj;
    s32 groupIndex;
    s32 objIndex;
    s32 errorOffset;
    char* entry;

    groupBase = (void*)(mapWork + activeGroup * 0x2F4);

    if (param_1 == 0) {
        obj = NULL;
        goto done;
    }

    group = groupBase;
    groupIndex = 0;
    while (groupIndex < *(s32*)groupBase) {
        obj = *(void**)((s32)group + 0x154);
        objIndex = 0;
        while (objIndex < *(s32*)((s32)group + 0x150)) {
            if (strcmp(*(const char**)*(s32*)((s32)obj + 0x08), (const char*)param_1) != 0) {
                objIndex++;
                obj = (void*)((s32)obj + 0x134);
            } else {
                goto done;
            }
        }
        group = (void*)((s32)group + 0x178);
        groupIndex++;
    }

    obj = NULL;

done:
    if (obj == NULL) {
        errorOffset = error_count * 0x24;
        entry = error_data + errorOffset;
        *(s32*)(entry + 0x20) = 0;
        strncpy(entry, (const char*)param_1, 0x1F);
        error_flag = 1;
        error_count++;
        error_count %= 32;
    } else {
        mapFlgOn((u32*)obj, param_2, 0);
    }
}

void mapGrpFlagOff(s32 param_1, u32 param_2) {
    extern s32 activeGroup;
    extern s32 mapWork;
    extern s32 strcmp(const char*, const char*);
    extern char* strncpy(char* dst, const char* src, u32 n);
    extern u8 mapFlgOff(u32* param_1, u32 param_2, int param_3);
    extern char error_data[];
    extern s32 error_count;
    extern s32 error_flag;

    void* groupBase;
    void* group;
    void* obj;
    s32 groupIndex;
    s32 objIndex;
    s32 errorOffset;
    char* entry;

    groupBase = (void*)(mapWork + activeGroup * 0x2F4);

    if (param_1 == 0) {
        obj = NULL;
        goto done;
    }

    group = groupBase;
    groupIndex = 0;
    while (groupIndex < *(s32*)groupBase) {
        obj = *(void**)((s32)group + 0x154);
        objIndex = 0;
        while (objIndex < *(s32*)((s32)group + 0x150)) {
            if (strcmp(*(const char**)*(s32*)((s32)obj + 0x08), (const char*)param_1) != 0) {
                objIndex++;
                obj = (void*)((s32)obj + 0x134);
            } else {
                goto done;
            }
        }
        group = (void*)((s32)group + 0x178);
        groupIndex++;
    }

    obj = NULL;

done:
    if (obj == NULL) {
        errorOffset = error_count * 0x24;
        entry = error_data + errorOffset;
        *(s32*)(entry + 0x20) = 0;
        strncpy(entry, (const char*)param_1, 0x1F);
        error_flag = 1;
        error_count++;
        error_count %= 32;
    } else {
        mapFlgOff((u32*)obj, param_2, 0);
    }
}

void _setOffScrnId(void* param_1, int param_2, int param_3) {
    int offId;
    int recurse;
    void* p1;
    void* p2;
    void* p3;

    offId = param_2;
    recurse = param_3;

#define SET_OFFSCRN(node_)                                      \
    do {                                                        \
        *(u16*)((s32)(node_) + 0xDC) = (u16)offId;              \
        if (offId != -1) {                                      \
            *(u32*)(node_) = *(u32*)(node_) | 0x80;             \
        } else {                                                \
            *(u32*)(node_) = *(u32*)(node_) & ~0x80;            \
        }                                                       \
    } while (0)

    SET_OFFSCRN(param_1);

    p1 = *(void**)((s32)param_1 + 0xE4);
    if (p1 != 0) {
        SET_OFFSCRN(p1);

        p2 = *(void**)((s32)p1 + 0xE4);
        if (p2 != 0) {
            SET_OFFSCRN(p2);

            p3 = *(void**)((s32)p2 + 0xE4);
            if (p3 != 0) {
                SET_OFFSCRN(p3);

                if (*(void**)((s32)p3 + 0xE4) != 0) {
                    _setOffScrnId(*(void**)((s32)p3 + 0xE4), offId, 1);
                }
                if (*(void**)((s32)p3 + 0xE8) != 0) {
                    _setOffScrnId(*(void**)((s32)p3 + 0xE8), offId, 1);
                }
            }

            p3 = *(void**)((s32)p2 + 0xE8);
            if (p3 != 0) {
                SET_OFFSCRN(p3);

                if (*(void**)((s32)p3 + 0xE4) != 0) {
                    _setOffScrnId(*(void**)((s32)p3 + 0xE4), offId, 1);
                }
                if (*(void**)((s32)p3 + 0xE8) != 0) {
                    _setOffScrnId(*(void**)((s32)p3 + 0xE8), offId, 1);
                }
            }
        }

        p2 = *(void**)((s32)p1 + 0xE8);
        if (p2 != 0) {
            SET_OFFSCRN(p2);

            p3 = *(void**)((s32)p2 + 0xE4);
            if (p3 != 0) {
                SET_OFFSCRN(p3);

                if (*(void**)((s32)p3 + 0xE4) != 0) {
                    _setOffScrnId(*(void**)((s32)p3 + 0xE4), offId, 1);
                }
                if (*(void**)((s32)p3 + 0xE8) != 0) {
                    _setOffScrnId(*(void**)((s32)p3 + 0xE8), offId, 1);
                }
            }

            p2 = *(void**)((s32)p2 + 0xE8);
            if (p2 != 0) {
                SET_OFFSCRN(p2);

                if (*(void**)((s32)p2 + 0xE4) != 0) {
                    _setOffScrnId(*(void**)((s32)p2 + 0xE4), offId, 1);
                }
                if (*(void**)((s32)p2 + 0xE8) != 0) {
                    _setOffScrnId(*(void**)((s32)p2 + 0xE8), offId, 1);
                }
            }
        }
    }

    if (recurse != 0) {
        p1 = *(void**)((s32)param_1 + 0xE8);
        if (p1 != 0) {
            SET_OFFSCRN(p1);

            p2 = *(void**)((s32)p1 + 0xE4);
            if (p2 != 0) {
                SET_OFFSCRN(p2);

                p3 = *(void**)((s32)p2 + 0xE4);
                if (p3 != 0) {
                    SET_OFFSCRN(p3);

                    if (*(void**)((s32)p3 + 0xE4) != 0) {
                        _setOffScrnId(*(void**)((s32)p3 + 0xE4), offId, 1);
                    }
                    if (*(void**)((s32)p3 + 0xE8) != 0) {
                        _setOffScrnId(*(void**)((s32)p3 + 0xE8), offId, 1);
                    }
                }

                p3 = *(void**)((s32)p2 + 0xE8);
                if (p3 != 0) {
                    SET_OFFSCRN(p3);

                    if (*(void**)((s32)p3 + 0xE4) != 0) {
                        _setOffScrnId(*(void**)((s32)p3 + 0xE4), offId, 1);
                    }
                    if (*(void**)((s32)p3 + 0xE8) != 0) {
                        _setOffScrnId(*(void**)((s32)p3 + 0xE8), offId, 1);
                    }
                }
            }

            p2 = *(void**)((s32)p1 + 0xE8);
            if (p2 != 0) {
                SET_OFFSCRN(p2);

                p3 = *(void**)((s32)p2 + 0xE4);
                if (p3 != 0) {
                    SET_OFFSCRN(p3);

                    if (*(void**)((s32)p3 + 0xE4) != 0) {
                        _setOffScrnId(*(void**)((s32)p3 + 0xE4), offId, 1);
                    }
                    if (*(void**)((s32)p3 + 0xE8) != 0) {
                        _setOffScrnId(*(void**)((s32)p3 + 0xE8), offId, 1);
                    }
                }

                p2 = *(void**)((s32)p2 + 0xE8);
                if (p2 != 0) {
                    SET_OFFSCRN(p2);

                    if (*(void**)((s32)p2 + 0xE4) != 0) {
                        _setOffScrnId(*(void**)((s32)p2 + 0xE4), offId, 1);
                    }
                    if (*(void**)((s32)p2 + 0xE8) != 0) {
                        _setOffScrnId(*(void**)((s32)p2 + 0xE8), offId, 1);
                    }
                }
            }
        }
    }

#undef SET_OFFSCRN
}

u8 mapObjSetOffScreen(char* param_1, char* param_2) {
    extern s32 activeGroup;
    extern s32 mapWork;
    extern s32 strcmp(const char*, const char*);
    extern s32 offscreenNameToId(char* name);

    void* groupBase;
    void* group;
    void* obj;
    s32 groupIndex;
    s32 objIndex;
    s32 offId;

    groupBase = (void*)(mapWork + activeGroup * 0x2F4);

    if (param_1 == NULL) {
        obj = NULL;
        goto done;
    }

    group = groupBase;
    groupIndex = 0;
    while (groupIndex < *(s32*)groupBase) {
        obj = *(void**)((s32)group + 0x154);
        objIndex = 0;
        while (objIndex < *(s32*)((s32)group + 0x150)) {
            if (strcmp(*(const char**)*(s32*)((s32)obj + 0x08), param_1) != 0) {
                objIndex++;
                obj = (void*)((s32)obj + 0x134);
            } else {
                goto done;
            }
        }
        group = (void*)((s32)group + 0x178);
        groupIndex++;
    }

    obj = NULL;

done:
    offId = offscreenNameToId(param_2);
    *(u16*)((s32)obj + 0xDC) = (u16)offId;
    *(u32*)obj = *(u32*)obj | 0x80;
}

u8 mapGrpSetOffScreen(char* param_1, char* param_2) {
    extern s32 activeGroup;
    extern s32 mapWork;
    extern s32 strcmp(const char*, const char*);
    extern s32 offscreenNameToId(char* name);
    extern u8 _setOffScrnId(void* param_1, int param_2, int param_3);

    void* groupBase;
    void* group;
    void* obj;
    s32 groupIndex;
    s32 objIndex;
    s32 offId;

    groupBase = (void*)(mapWork + activeGroup * 0x2F4);

    if (param_1 == NULL) {
        obj = NULL;
        goto done;
    }

    group = groupBase;
    groupIndex = 0;
    while (groupIndex < *(s32*)groupBase) {
        obj = *(void**)((s32)group + 0x154);
        objIndex = 0;
        while (objIndex < *(s32*)((s32)group + 0x150)) {
            if (strcmp(*(const char**)*(s32*)((s32)obj + 0x08), param_1) != 0) {
                objIndex++;
                obj = (void*)((s32)obj + 0x134);
            } else {
                goto done;
            }
        }
        group = (void*)((s32)group + 0x178);
        groupIndex++;
    }

    obj = NULL;

done:
    offId = offscreenNameToId(param_2);
    _setOffScrnId(obj, offId, 0);
}


void mapObjClearOffScreen(s32 id) {
    extern s32 activeGroup;
    extern s32 mapWork;
    extern s32 strcmp(const char*, const char*);

    void* groupBase;
    void* group;
    void* obj;
    s32 groupIndex;
    s32 objIndex;

    groupBase = (void*)(mapWork + activeGroup * 0x2F4);

    if (id == 0) {
        obj = NULL;
        goto done;
    }

    group = groupBase;
    groupIndex = 0;
    while (groupIndex < *(s32*)groupBase) {
        obj = *(void**)((s32)group + 0x154);
        objIndex = 0;
        while (objIndex < *(s32*)((s32)group + 0x150)) {
            if (strcmp(*(const char**)*(s32*)((s32)obj + 0x08), (const char*)id) != 0) {
                objIndex++;
                obj = (void*)((s32)obj + 0x134);
            } else {
                goto done;
            }
        }
        group = (void*)((s32)group + 0x178);
        groupIndex++;
    }

    obj = NULL;

done:
    *(s16*)((s32)obj + 0xDC) = -1;
    *(u32*)obj = *(u32*)obj & ~0x80;
}


void mapGrpClearOffScreen(s32 id) {
    extern s32 activeGroup;
    extern s32 mapWork;
    extern s32 strcmp(const char*, const char*);
    extern u8 _setOffScrnId(void* param_1, int param_2, int param_3);

    void* groupBase;
    void* group;
    void* obj;
    s32 groupIndex;
    s32 objIndex;

    groupBase = (void*)(mapWork + activeGroup * 0x2F4);

    if (id == 0) {
        obj = NULL;
        goto done;
    }

    group = groupBase;
    groupIndex = 0;
    while (groupIndex < *(s32*)groupBase) {
        obj = *(void**)((s32)group + 0x154);
        objIndex = 0;
        while (objIndex < *(s32*)((s32)group + 0x150)) {
            if (strcmp(*(const char**)*(s32*)((s32)obj + 0x08), (const char*)id) != 0) {
                objIndex++;
                obj = (void*)((s32)obj + 0x134);
            } else {
                goto done;
            }
        }
        group = (void*)((s32)group + 0x178);
        groupIndex++;
    }

    obj = NULL;

done:
    _setOffScrnId(obj, -1, 0);
}

u8 mapObjRotate(char* name, f32 x, f32 y, f32 z) {
    extern s32 activeGroup;
    extern s32 mapWork;
    extern s32 strcmp(const char*, const char*);
    extern void PSMTXIdentity(void*);
    extern void PSMTXRotRad(void*, s8, f32);
    extern void PSMTXConcat(void*, void*, void*);
    extern f32 float_deg2rad_8041f948;
    void* work;
    void* group;
    void* obj;
    s32 groupIndex;
    s32 objIndex;
    f32 mtxX[3][4];
    f32 mtxY[3][4];
    f32 mtxZ[3][4];

    work = (void*)(mapWork + activeGroup * 0x2F4);
    if (name == 0) {
        obj = 0;
    } else {
        group = work;
        groupIndex = 0;
        while (groupIndex < *(s32*)work) {
            obj = *(void**)((s32)group + 0x154);
            objIndex = 0;
            while (objIndex < *(s32*)((s32)group + 0x150)) {
                if (strcmp(*(const char**)*(s32*)((s32)obj + 8), name) == 0) {
                    goto found;
                }
                objIndex++;
                obj = (void*)((s32)obj + 0x134);
            }
            group = (void*)((s32)group + 0x178);
            groupIndex++;
        }
        obj = 0;
    }

found:
    if (obj != 0) {
        if ((*(u32*)obj & 0x200) == 0) {
            PSMTXIdentity((void*)((s32)obj + 0xAC));
        }
        PSMTXRotRad(mtxX, 'x', float_deg2rad_8041f948 * x);
        PSMTXRotRad(mtxY, 'y', float_deg2rad_8041f948 * y);
        PSMTXRotRad(mtxZ, 'z', float_deg2rad_8041f948 * z);
        PSMTXConcat((void*)((s32)obj + 0xAC), mtxZ, (void*)((s32)obj + 0xAC));
        PSMTXConcat((void*)((s32)obj + 0xAC), mtxY, (void*)((s32)obj + 0xAC));
        PSMTXConcat((void*)((s32)obj + 0xAC), mtxX, (void*)((s32)obj + 0xAC));
        *(u32*)obj |= 0x2000200;
    }
}

u8 mapObjTranslate(char* name, f32 x, f32 y, f32 z) {
    extern s32 activeGroup;
    extern s32 mapWork;
    extern s32 strcmp(const char*, const char*);
    extern void PSMTXIdentity(void*);
    extern void PSMTXTrans(void*, f32, f32, f32);
    extern void PSMTXConcat(void*, void*, void*);
    extern f32 float_10_8041f944;
    void* work;
    void* group;
    void* obj;
    s32 groupIndex;
    s32 objIndex;
    f32 mtx[3][4];

    work = (void*)(mapWork + activeGroup * 0x2F4);
    if (name == 0) {
        obj = 0;
    } else {
        group = work;
        groupIndex = 0;
        while (groupIndex < *(s32*)work) {
            obj = *(void**)((s32)group + 0x154);
            objIndex = 0;
            while (objIndex < *(s32*)((s32)group + 0x150)) {
                if (strcmp(*(const char**)*(s32*)((s32)obj + 8), name) == 0) {
                    goto found;
                }
                objIndex++;
                obj = (void*)((s32)obj + 0x134);
            }
            group = (void*)((s32)group + 0x178);
            groupIndex++;
        }
        obj = 0;
    }

found:
    if (obj != 0) {
        if ((*(u32*)obj & 0x200) == 0) {
            PSMTXIdentity((void*)((s32)obj + 0xAC));
        }
        PSMTXTrans(mtx, x / float_10_8041f944, y / float_10_8041f944, z / float_10_8041f944);
        PSMTXConcat((void*)((s32)obj + 0xAC), mtx, (void*)((s32)obj + 0xAC));
        *(u32*)obj |= 0x2000200;
    }
}

u8 mapObjScale(char* name, f32 x, f32 y, f32 z) {
    extern s32 activeGroup;
    extern s32 mapWork;
    extern s32 strcmp(const char*, const char*);
    extern void PSMTXIdentity(void*);
    extern void PSMTXScale(void*, f32, f32, f32);
    extern void PSMTXConcat(void*, void*, void*);
    void* work;
    void* group;
    void* obj;
    s32 groupIndex;
    s32 objIndex;
    f32 mtx[3][4];

    work = (void*)(mapWork + activeGroup * 0x2F4);
    if (name == 0) {
        obj = 0;
    } else {
        group = work;
        groupIndex = 0;
        while (groupIndex < *(s32*)work) {
            obj = *(void**)((s32)group + 0x154);
            objIndex = 0;
            while (objIndex < *(s32*)((s32)group + 0x150)) {
                if (strcmp(*(const char**)*(s32*)((s32)obj + 8), name) == 0) {
                    goto found;
                }
                objIndex++;
                obj = (void*)((s32)obj + 0x134);
            }
            group = (void*)((s32)group + 0x178);
            groupIndex++;
        }
        obj = 0;
    }

found:
    if (obj != 0) {
        if ((*(u32*)obj & 0x200) == 0) {
            PSMTXIdentity((void*)((s32)obj + 0xAC));
        }
        PSMTXScale(mtx, x, y, z);
        PSMTXConcat((void*)((s32)obj + 0xAC), mtx, (void*)((s32)obj + 0xAC));
        *(u32*)obj |= 0x2000200;
    }
}

void mapObjGetPosSub(int param_1, s32 param_2, int* param_3, int param_4) {
    extern void PSMTXMultVec(void* mtx, void* src, void* dst);
    extern void PSVECAdd(void* a, void* b, void* out);

    u32 iVar1;
    u32 iVar2;
    u32 iVar3;
    f32 VStack_d8[3];
    f32 VStack_cc[3];
    f32 VStack_c0[3];
    f32 VStack_b4[3];
    f32 VStack_a8[3];
    f32 VStack_9c[3];
    f32 VStack_90[3];
    f32 VStack_84[3];
    f32 VStack_78[3];
    f32 VStack_6c[3];
    f32 VStack_60[3];
    f32 VStack_54[3];
    f32 VStack_48[3];
    f32 VStack_3c[3];
    f32 VStack_30[3];

#define DO_LEAF(node_, vec_)                                      \
    do {                                                          \
        PSMTXMultVec((void*)((node_) + 0x1C),                     \
                     (void*)((node_) + 0x10),                     \
                     (vec_));                                     \
        PSVECAdd((void*)param_2, (vec_), (void*)param_2);         \
        *param_3 = *param_3 + 1;                                  \
    } while (0)

    iVar3 = *(u32 *)(param_1 + 0xE4);
    if (iVar3 != 0) {
        iVar2 = *(u32 *)(iVar3 + 0xE4);
        if (iVar2 != 0) {
            iVar1 = *(u32 *)(iVar2 + 0xE4);
            if (iVar1 != 0) {
                if (*(u32 *)(iVar1 + 0xE4) != 0) {
                    mapObjGetPosSub(*(u32 *)(iVar1 + 0xE4), param_2, param_3, 1);
                } else {
                    DO_LEAF(iVar1, VStack_84);
                }
                if (*(u32 *)(iVar1 + 0xE8) != 0) {
                    mapObjGetPosSub(*(u32 *)(iVar1 + 0xE8), param_2, param_3, 1);
                }
            } else {
                DO_LEAF(iVar2, VStack_54);
            }

            iVar2 = *(u32 *)(iVar2 + 0xE8);
            if (iVar2 != 0) {
                if (*(u32 *)(iVar2 + 0xE4) != 0) {
                    mapObjGetPosSub(*(u32 *)(iVar2 + 0xE4), param_2, param_3, 1);
                } else {
                    DO_LEAF(iVar2, VStack_90);
                }
                if (*(u32 *)(iVar2 + 0xE8) != 0) {
                    mapObjGetPosSub(*(u32 *)(iVar2 + 0xE8), param_2, param_3, 1);
                }
            }
        } else {
            DO_LEAF(iVar3, VStack_3c);
        }

        iVar3 = *(u32 *)(iVar3 + 0xE8);
        if (iVar3 != 0) {
            iVar2 = *(u32 *)(iVar3 + 0xE4);
            if (iVar2 != 0) {
                if (*(u32 *)(iVar2 + 0xE4) != 0) {
                    mapObjGetPosSub(*(u32 *)(iVar2 + 0xE4), param_2, param_3, 1);
                } else {
                    DO_LEAF(iVar2, VStack_9c);
                }
                if (*(u32 *)(iVar2 + 0xE8) != 0) {
                    mapObjGetPosSub(*(u32 *)(iVar2 + 0xE8), param_2, param_3, 1);
                }
            } else {
                DO_LEAF(iVar3, VStack_60);
            }

            iVar3 = *(u32 *)(iVar3 + 0xE8);
            if (iVar3 != 0) {
                if (*(u32 *)(iVar3 + 0xE4) != 0) {
                    mapObjGetPosSub(*(u32 *)(iVar3 + 0xE4), param_2, param_3, 1);
                } else {
                    DO_LEAF(iVar3, VStack_a8);
                }
                if (*(u32 *)(iVar3 + 0xE8) != 0) {
                    mapObjGetPosSub(*(u32 *)(iVar3 + 0xE8), param_2, param_3, 1);
                }
            }
        }
    } else {
        DO_LEAF(param_1, VStack_30);
    }

    if ((param_4 != 0) && ((iVar3 = *(u32 *)(param_1 + 0xE8)) != 0)) {
        iVar2 = *(u32 *)(iVar3 + 0xE4);
        if (iVar2 != 0) {
            iVar1 = *(u32 *)(iVar2 + 0xE4);
            if (iVar1 != 0) {
                if (*(u32 *)(iVar1 + 0xE4) != 0) {
                    mapObjGetPosSub(*(u32 *)(iVar1 + 0xE4), param_2, param_3, 1);
                } else {
                    DO_LEAF(iVar1, VStack_b4);
                }
                if (*(u32 *)(iVar1 + 0xE8) != 0) {
                    mapObjGetPosSub(*(u32 *)(iVar1 + 0xE8), param_2, param_3, 1);
                }
            } else {
                DO_LEAF(iVar2, VStack_6c);
            }

            iVar2 = *(u32 *)(iVar2 + 0xE8);
            if (iVar2 != 0) {
                if (*(u32 *)(iVar2 + 0xE4) != 0) {
                    mapObjGetPosSub(*(u32 *)(iVar2 + 0xE4), param_2, param_3, 1);
                } else {
                    DO_LEAF(iVar2, VStack_c0);
                }
                if (*(u32 *)(iVar2 + 0xE8) != 0) {
                    mapObjGetPosSub(*(u32 *)(iVar2 + 0xE8), param_2, param_3, 1);
                }
            }
        } else {
            DO_LEAF(iVar3, VStack_48);
        }

        iVar3 = *(u32 *)(iVar3 + 0xE8);
        if (iVar3 != 0) {
            iVar2 = *(u32 *)(iVar3 + 0xE4);
            if (iVar2 != 0) {
                if (*(u32 *)(iVar2 + 0xE4) != 0) {
                    mapObjGetPosSub(*(u32 *)(iVar2 + 0xE4), param_2, param_3, 1);
                } else {
                    DO_LEAF(iVar2, VStack_cc);
                }
                if (*(u32 *)(iVar2 + 0xE8) != 0) {
                    mapObjGetPosSub(*(u32 *)(iVar2 + 0xE8), param_2, param_3, 1);
                }
            } else {
                DO_LEAF(iVar3, VStack_78);
            }

            iVar3 = *(u32 *)(iVar3 + 0xE8);
            if (iVar3 != 0) {
                if (*(u32 *)(iVar3 + 0xE4) != 0) {
                    mapObjGetPosSub(*(u32 *)(iVar3 + 0xE4), param_2, param_3, 1);
                } else {
                    DO_LEAF(iVar3, VStack_d8);
                }
                if (*(u32 *)(iVar3 + 0xE8) != 0) {
                    mapObjGetPosSub(*(u32 *)(iVar3 + 0xE8), param_2, param_3, 1);
                }
            }
        }
    }

#undef DO_LEAF
}

u8 mapObjGetPos(char* param_1, void* param_2) {
    extern s32 activeGroup;
    extern s32 mapWork;
    extern s32 strcmp(const char*, const char*);
    extern void mapObjGetPosSub(void*, void*, s32*, s32);
    extern void PSVECScale(void*, void*, f32);
    extern f32 float_1_8041f940;
    extern f32 vec3_802bfb74[3];
    extern f32 vec3_802bfb80[3];
    void* work;
    void* group;
    void* obj;
    s32 groupIndex;
    s32 objIndex;
    s32 count;
    f32 pos[3];

    count = 0;
    pos[0] = vec3_802bfb74[0];
    pos[1] = vec3_802bfb74[1];
    pos[2] = vec3_802bfb74[2];
    work = (void*)(mapWork + activeGroup * 0x2F4);

    if (param_1 == 0) {
        obj = 0;
    } else {
        group = work;
        groupIndex = 0;
        while (groupIndex < *(s32*)work) {
            obj = *(void**)((s32)group + 0x154);
            objIndex = 0;
            while (objIndex < *(s32*)((s32)group + 0x150)) {
                if (strcmp(*(const char**)*(s32*)((s32)obj + 8), param_1) == 0) {
                    goto found;
                }
                objIndex++;
                obj = (void*)((s32)obj + 0x134);
            }
            group = (void*)((s32)group + 0x178);
            groupIndex++;
        }
        obj = 0;
    }

found:
    if (obj == 0) {
        ((f32*)param_2)[0] = vec3_802bfb80[0];
        ((f32*)param_2)[1] = vec3_802bfb80[1];
        ((f32*)param_2)[2] = vec3_802bfb80[2];
    } else {
        mapObjGetPosSub(obj, pos, &count, 0);
        PSVECScale(pos, param_2, float_1_8041f940 / (f32)count);
    }
}


void mapSetFog(s32 type, f32 start, f32 end, void* color) {
    extern s32 activeGroup;
    extern s32 mapWork;
    s32 rgba;
    s32 work;

    rgba = *(s32*)color;
    work = mapWork + activeGroup * 0x2F4;
    *(s32*)(work + 0x28) = type;
    *(f32*)(work + 0x2C) = start;
    *(f32*)(work + 0x30) = end;
    *(s32*)(work + 0x34) = rgba;
}

void mapGetFog(void* type, f32* start, f32* end, void* color) {
    extern s32 activeGroup;
    extern s32 mapWork;
    s32 work = mapWork + activeGroup * 0x2F4;
    *(s32*)type = *(s32*)(work + 0x28);
    *start = *(f32*)(work + 0x2C);
    *end = *(f32*)(work + 0x30);
    *(s32*)color = *(s32*)(work + 0x34);
}

void mapFogOn(void) {
    extern s32 activeGroup;
    extern s32 mapWork;
    ((u16*)mapWork)[activeGroup * 0x17A + 2] |= 1;
}

void mapFogOff(void) {
    extern s32 activeGroup;
    extern s32 mapWork;
    ((u16*)mapWork)[activeGroup * 0x17A + 2] &= ~1;
}

void mapSetBlend(void* color) {
    extern s32 activeGroup;
    extern s32 mapWork;
    s32 work = mapWork + activeGroup * 0x2F4;
    *(s32*)(work + 0x16) = *(s32*)color;
    *(u16*)(work + 4) |= 2;
}

void mapSetBlend2(void* color) {
    extern s32 activeGroup;
    extern s32 mapWork;
    s32 work = mapWork + activeGroup * 0x2F4;
    *(s32*)(work + 0x1A) = *(s32*)color;
    *(u16*)(work + 4) |= 4;
}

void mapGetBlend(void* color) {
    extern s32 activeGroup;
    extern s32 mapWork;
    extern s32 dat_8041f920;
    s32 work = mapWork + activeGroup * 0x2F4;
    if (*(u16*)(work + 4) & 2) {
        *(s32*)color = *(s32*)(work + 0x16);
    } else {
        *(s32*)color = dat_8041f920;
    }
}

void mapGetBlend2(void* color) {
    extern s32 activeGroup;
    extern s32 mapWork;
    extern s32 dat_8041f924;
    s32 work = mapWork + activeGroup * 0x2F4;
    if (*(u16*)(work + 4) & 4) {
        *(s32*)color = *(s32*)(work + 0x1A);
    } else {
        *(s32*)color = dat_8041f924;
    }
}

void mapBlendOff(void) {
    extern s32 activeGroup;
    extern s32 mapWork;
    ((u16*)mapWork)[activeGroup * 0x17A + 2] &= ~2;
}


void mapBlendOff2(void) {
    extern s32 activeGroup;
    extern s32 mapWork;
    ((u16*)mapWork)[activeGroup * 0x17A + 2] &= ~4;
}

u8 mapObjSetColor(char* param_1, void* param_2) {
    extern s32 activeGroup;
    extern s32 mapWork;
    extern s32 strcmp(const char*, const char*);

    void* groupBase;
    void* group;
    void* obj;
    s32 groupIndex;
    s32 objIndex;

    groupBase = (void*)(mapWork + activeGroup * 0x2F4);

    if (param_1 == NULL) {
        obj = NULL;
        goto done;
    }

    group = groupBase;
    groupIndex = 0;
    while (groupIndex < *(s32*)groupBase) {
        obj = *(void**)((s32)group + 0x154);
        objIndex = 0;
        while (objIndex < *(s32*)((s32)group + 0x150)) {
            if (strcmp(*(const char**)*(s32*)((s32)obj + 0x08), param_1) == 0) {
                goto done;
            }
            objIndex++;
            obj = (void*)((s32)obj + 0x134);
        }
        group = (void*)((s32)group + 0x178);
        groupIndex++;
    }

    obj = NULL;

done:
    if (obj != NULL) {
        *(u32*)((s32)obj + 0x0C) = *(u32*)param_2;
        *(u32*)obj = *(u32*)obj | 0x40;
    }
}

u8 setColor(void* param_1, u32* param_2, int param_3) {
    void* p2;
    void* p3;
    void* p4;
    u32 color;
    u32 local_68;
    u32 local_64;
    u32 local_60;
    u32 local_5c;
    u32 local_58;
    u32 local_54;
    u32 local_50;
    u32 local_4c;
    u32 local_48;
    u32 local_44;
    u32 local_40;
    u32 local_3c;
    u32 local_38;
    u32 local_34;
    u32 local_30;
    u32 local_2c;
    u32 local_28;
    u32 local_24;

    if (param_1 != 0) {
        color = *param_2;
        *(u32*)((s32)param_1 + 0x0C) = color;
        *(u32*)param_1 = *(u32*)param_1 | 0x40;

        p4 = *(void**)((s32)param_1 + 0xE4);
        if (p4 != 0) {
            *(u32*)((s32)p4 + 0x0C) = color;
            *(u32*)p4 = *(u32*)p4 | 0x40;

            p3 = *(void**)((s32)p4 + 0xE4);
            local_24 = color;
            if (p3 != 0) {
                *(u32*)((s32)p3 + 0x0C) = color;
                *(u32*)p3 = *(u32*)p3 | 0x40;

                p2 = *(void**)((s32)p3 + 0xE4);
                if (p2 != 0) {
                    *(u32*)((s32)p2 + 0x0C) = color;
                    *(u32*)p2 = *(u32*)p2 | 0x40;

                    if (*(void**)((s32)p2 + 0xE4) != 0) {
                        local_30 = color;
                        setColor(*(void**)((s32)p2 + 0xE4), &local_30, 1);
                    }
                    if (*(void**)((s32)p2 + 0xE8) != 0) {
                        local_2c = local_24;
                        setColor(*(void**)((s32)p2 + 0xE8), &local_2c, 1);
                    }
                }

                p3 = *(void**)((s32)p3 + 0xE8);
                if (p3 != 0) {
                    *(u32*)((s32)p3 + 0x0C) = local_24;
                    *(u32*)p3 = *(u32*)p3 | 0x40;

                    if (*(void**)((s32)p3 + 0xE4) != 0) {
                        local_38 = local_24;
                        setColor(*(void**)((s32)p3 + 0xE4), &local_38, 1);
                    }
                    if (*(void**)((s32)p3 + 0xE8) != 0) {
                        local_34 = local_24;
                        setColor(*(void**)((s32)p3 + 0xE8), &local_34, 1);
                    }
                }
            }

            p4 = *(void**)((s32)p4 + 0xE8);
            if (p4 != 0) {
                *(u32*)((s32)p4 + 0x0C) = local_24;
                *(u32*)p4 = *(u32*)p4 | 0x40;

                p3 = *(void**)((s32)p4 + 0xE4);
                if (p3 != 0) {
                    *(u32*)((s32)p3 + 0x0C) = local_24;
                    *(u32*)p3 = *(u32*)p3 | 0x40;

                    if (*(void**)((s32)p3 + 0xE4) != 0) {
                        local_40 = local_24;
                        setColor(*(void**)((s32)p3 + 0xE4), &local_40, 1);
                    }
                    if (*(void**)((s32)p3 + 0xE8) != 0) {
                        local_3c = local_24;
                        setColor(*(void**)((s32)p3 + 0xE8), &local_3c, 1);
                    }
                }

                p4 = *(void**)((s32)p4 + 0xE8);
                if (p4 != 0) {
                    *(u32*)((s32)p4 + 0x0C) = local_24;
                    *(u32*)p4 = *(u32*)p4 | 0x40;

                    if (*(void**)((s32)p4 + 0xE4) != 0) {
                        local_48 = local_24;
                        setColor(*(void**)((s32)p4 + 0xE4), &local_48, 1);
                    }
                    if (*(void**)((s32)p4 + 0xE8) != 0) {
                        local_44 = local_24;
                        setColor(*(void**)((s32)p4 + 0xE8), &local_44, 1);
                    }
                }
            }
        }

        if (param_3 != 0) {
            p4 = *(void**)((s32)param_1 + 0xE8);
            if (p4 != 0) {
                local_28 = *param_2;
                *(u32*)((s32)p4 + 0x0C) = local_28;
                *(u32*)p4 = *(u32*)p4 | 0x40;

                p3 = *(void**)((s32)p4 + 0xE4);
                if (p3 != 0) {
                    *(u32*)((s32)p3 + 0x0C) = local_28;
                    *(u32*)p3 = *(u32*)p3 | 0x40;

                    p2 = *(void**)((s32)p3 + 0xE4);
                    if (p2 != 0) {
                        *(u32*)((s32)p2 + 0x0C) = local_28;
                        *(u32*)p2 = *(u32*)p2 | 0x40;

                        if (*(void**)((s32)p2 + 0xE4) != 0) {
                            local_50 = local_28;
                            setColor(*(void**)((s32)p2 + 0xE4), &local_50, 1);
                        }
                        if (*(void**)((s32)p2 + 0xE8) != 0) {
                            local_4c = local_28;
                            setColor(*(void**)((s32)p2 + 0xE8), &local_4c, 1);
                        }
                    }

                    p3 = *(void**)((s32)p3 + 0xE8);
                    if (p3 != 0) {
                        *(u32*)((s32)p3 + 0x0C) = local_28;
                        *(u32*)p3 = *(u32*)p3 | 0x40;

                        if (*(void**)((s32)p3 + 0xE4) != 0) {
                            local_58 = local_28;
                            setColor(*(void**)((s32)p3 + 0xE4), &local_58, 1);
                        }
                        if (*(void**)((s32)p3 + 0xE8) != 0) {
                            local_54 = local_28;
                            setColor(*(void**)((s32)p3 + 0xE8), &local_54, 1);
                        }
                    }
                }

                p4 = *(void**)((s32)p4 + 0xE8);
                if (p4 != 0) {
                    *(u32*)((s32)p4 + 0x0C) = local_28;
                    *(u32*)p4 = *(u32*)p4 | 0x40;

                    p3 = *(void**)((s32)p4 + 0xE4);
                    if (p3 != 0) {
                        *(u32*)((s32)p3 + 0x0C) = local_28;
                        *(u32*)p3 = *(u32*)p3 | 0x40;

                        if (*(void**)((s32)p3 + 0xE4) != 0) {
                            local_60 = local_28;
                            setColor(*(void**)((s32)p3 + 0xE4), &local_60, 1);
                        }
                        if (*(void**)((s32)p3 + 0xE8) != 0) {
                            local_5c = local_28;
                            setColor(*(void**)((s32)p3 + 0xE8), &local_5c, 1);
                        }
                    }

                    p4 = *(void**)((s32)p4 + 0xE8);
                    if (p4 != 0) {
                        *(u32*)((s32)p4 + 0x0C) = local_28;
                        *(u32*)p4 = *(u32*)p4 | 0x40;

                        if (*(void**)((s32)p4 + 0xE4) != 0) {
                            local_68 = local_28;
                            setColor(*(void**)((s32)p4 + 0xE4), &local_68, 1);
                        }
                        if (*(void**)((s32)p4 + 0xE8) != 0) {
                            local_64 = local_28;
                            setColor(*(void**)((s32)p4 + 0xE8), &local_64, 1);
                        }
                    }
                }
            }
        }
    }

    return 0;
}

void mapGrpSetColor(s32 param_1, void* param_2) {
    extern s32 activeGroup;
    extern s32 mapWork;
    extern s32 strcmp(const char*, const char*);
    extern u8 setColor(void* param_1, u32* param_2, int param_3);

    void* obj;
    void* group;
    void* groupBase;
    s32 objIndex;
    s32 groupIndex;
    u32 color;

    color = *(u32*)param_2;
    groupBase = (void*)(mapWork + activeGroup * 0x2F4);

    if (param_1 == 0) {
        obj = NULL;
        goto done;
    }

    group = groupBase;
    groupIndex = 0;
    while (groupIndex < *(s32*)groupBase) {
        obj = *(void**)((s32)group + 0x154);
        objIndex = 0;
        while (objIndex < *(s32*)((s32)group + 0x150)) {
            if (strcmp(*(const char**)*(s32*)((s32)obj + 0x08), (const char*)param_1) == 0) {
                goto done;
            }
            objIndex++;
            obj = (void*)((s32)obj + 0x134);
        }
        group = (void*)((s32)group + 0x178);
        groupIndex++;
    }

    obj = NULL;

done:
    setColor(obj, &color, 0);
}

void mapSetProjTexObj(void* texObj, void* imgRef) {
    typedef struct TexObjCopy {
        s32 word[8];
    } TexObjCopy;
    extern TexObjCopy projTexObj;
    extern void* projTexImgRef;
    projTexObj = *(TexObjCopy*)texObj;
    projTexImgRef = imgRef;
}

void mapSetProjMtx(void* mtx) {
    extern void PSMTXCopy(void*, void*);
    extern s32 projMtx[12];
    PSMTXCopy(mtx, projMtx);
}

void spline_maketable(s32 count, f32* points, f32* table, f32* out) {
    extern void* mapalloc_base_ptr;
    extern void* _mapAlloc(void* heap, u32 size);
    extern void _mapFree(void* heap, void* ptr);
    extern f32 sqrtf(f32);
    extern f32 __float_nan;

    f32* h;
    f32* work;
    f32 total;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 len;
    f32 inv;
    s32 i;
    s32 last;

    h = (f32*)_mapAlloc(mapalloc_base_ptr, count * 4);
    work = (f32*)_mapAlloc(mapalloc_base_ptr, count * 12);

    table[0] = 0.0f;
    for (i = 1; i < count; i++) {
        dx = points[i * 3 + 0] - points[(i - 1) * 3 + 0];
        dy = points[i * 3 + 1] - points[(i - 1) * 3 + 1];
        dz = points[i * 3 + 2] - points[(i - 1) * 3 + 2];
        len = dx * dx + dy * dy + dz * dz;
        if (len > 0.0f) {
            len = sqrtf(len);
        } else if (len < 0.0f) {
            len = __float_nan;
        }
        table[i] = table[i - 1] + len;
    }

    total = table[count - 1];
    for (i = 1; i < count; i++) {
        table[i] = table[i] / total;
    }

    last = count - 1;
    out[0] = 0.0f;
    out[1] = 0.0f;
    out[2] = 0.0f;
    out[last * 3 + 0] = 0.0f;
    out[last * 3 + 1] = 0.0f;
    out[last * 3 + 2] = 0.0f;

    for (i = 0; i < last; i++) {
        h[i] = table[i + 1] - table[i];
        if (h[i] == 0.0f) {
            h[i] = 0.0001f;
        }
        work[(i + 1) * 3 + 0] = (points[(i + 1) * 3 + 0] - points[i * 3 + 0]) / h[i];
        work[(i + 1) * 3 + 1] = (points[(i + 1) * 3 + 1] - points[i * 3 + 1]) / h[i];
        work[(i + 1) * 3 + 2] = (points[(i + 1) * 3 + 2] - points[i * 3 + 2]) / h[i];
    }

    out[3] = work[6] - work[3];
    out[4] = work[7] - work[4];
    out[5] = work[8] - work[5];

    work[3] = 2.0f * (table[2] - table[0]);
    work[4] = 2.0f * (table[2] - table[0]);
    work[5] = 2.0f * (table[2] - table[0]);

    for (i = 1; i < count - 2; i++) {
        f32 step;
        f32 ratio0;
        f32 ratio1;
        f32 ratio2;
        f32 span;

        step = h[i];
        ratio0 = step / work[i * 3 + 0];
        ratio1 = step / work[i * 3 + 1];
        ratio2 = step / work[i * 3 + 2];
        span = 2.0f * (table[i + 2] - table[i]);

        out[(i + 1) * 3 + 0] = (work[(i + 2) * 3 + 0] - work[(i + 1) * 3 + 0]) - ratio0 * out[i * 3 + 0];
        out[(i + 1) * 3 + 1] = (work[(i + 2) * 3 + 1] - work[(i + 1) * 3 + 1]) - ratio1 * out[i * 3 + 1];
        out[(i + 1) * 3 + 2] = (work[(i + 2) * 3 + 2] - work[(i + 1) * 3 + 2]) - ratio2 * out[i * 3 + 2];

        work[(i + 1) * 3 + 0] = span - ratio0 * h[i];
        work[(i + 1) * 3 + 1] = span - ratio1 * h[i];
        work[(i + 1) * 3 + 2] = span - ratio2 * h[i];
    }

    i = count - 2;
    out[i * 3 + 0] = (out[i * 3 + 0] - h[i] * out[(i + 1) * 3 + 0]) / work[i * 3 + 0];
    out[i * 3 + 1] = (out[i * 3 + 1] - h[i] * out[(i + 1) * 3 + 1]) / work[i * 3 + 1];
    out[i * 3 + 2] = (out[i * 3 + 2] - h[i] * out[(i + 1) * 3 + 2]) / work[i * 3 + 2];

    for (i = count - 3; i >= 0; i--) {
        out[i * 3 + 0] = (out[i * 3 + 0] - h[i] * out[(i + 1) * 3 + 0]) / work[i * 3 + 0];
        out[i * 3 + 1] = (out[i * 3 + 1] - h[i] * out[(i + 1) * 3 + 1]) / work[i * 3 + 1];
        out[i * 3 + 2] = (out[i * 3 + 2] - h[i] * out[(i + 1) * 3 + 2]) / work[i * 3 + 2];
    }

    _mapFree(mapalloc_base_ptr, work);
    _mapFree(mapalloc_base_ptr, h);
}

void spline_getvalue(double time, float* out, int count, float* values, int timesRaw, int slopesRaw) {
    extern f32 float_2_8041f938;
    extern f32 float_3_8041f92c;
    f32* times;
    f32* slopes;
    s32 low;
    s32 high;
    s32 middle;
    s32 index;
    f32 localTime;
    f32 span;
    f32 slope0;
    f32 slope1;
    f32 value0;
    f32 value1;
    s32 axis;

    times = (f32*)timesRaw;
    slopes = (f32*)slopesRaw;

    if (count == 2) {
        out[0] = (f32)(time * (double)(values[3] - values[0]) + values[0]);
        out[1] = (f32)(time * (double)(values[4] - values[1]) + values[1]);
        out[2] = (f32)(time * (double)(values[5] - values[2]) + values[2]);
        return;
    }

    low = 0;
    high = count - 1;
    while (low < high) {
        middle = (low + high) / 2;
        if ((double)times[middle] < time) {
            low = middle + 1;
        } else {
            high = middle;
        }
    }
    index = low;
    if (index > 0) {
        index--;
    }

    localTime = (f32)(time - (double)times[index]);
    span = times[index + 1] - times[index];
    for (axis = 0; axis < 3; axis++) {
        slope0 = slopes[index * 3 + axis];
        slope1 = slopes[index * 3 + axis + 3];
        value0 = values[index * 3 + axis];
        value1 = values[index * 3 + axis + 3];
        out[axis] = localTime * (
            localTime * (
                float_3_8041f92c * slope0
                + (localTime * (slope1 - slope0)) / span
            )
            - (
                span * (float_2_8041f938 * slope0 + slope1)
                - (value1 - value0) / span
            )
        ) + value0;
    }
}

s32 mapTestXLU(u32 materialFlag, void* param_2, void* param_3) {
    extern s32 activeGroup;
    extern s32 mapWork;
    void* work;

    if ((materialFlag & 0x40) && (*(u8*)((s32)param_2 + 3) != 0xFF)) {
        return 1;
    }

    if (*(u8*)((s32)param_3 + 3) != 0xFF) {
        return 1;
    }

    if ((materialFlag & 0x800) == 0) {
        work = (void*)(mapWork + activeGroup * 0x2F4);
        if ((*(u16*)((s32)work + 4) & 2) && (*(u8*)((s32)work + 0x19) != 0xFF)) {
            return 1;
        }
    }

    if ((materialFlag & 0x1000) == 0) {
        work = (void*)(mapWork + activeGroup * 0x2F4);
        if ((*(u16*)((s32)work + 4) & 4) && (*(u8*)((s32)work + 0x1D) != 0xFF)) {
            return 1;
        }
    }

    return 0;
}

void mapSetTevCallback(s32 index, void* callback) {
    extern s32 activeGroup;
    extern s32 mapWork;
    *(void**)(mapWork + activeGroup * 0x2F4 + 0x110 + index * 4) = callback;
}

u8 mapObjFlushOn(char* param_1) {
    extern s32 activeGroup;
    extern s32 mapWork;
    extern s32 strcmp(const char*, const char*);
    extern char* strncpy(char* dst, const char* src, u32 n);
    extern char error_data[];
    extern s32 error_count;
    extern s32 error_flag;

    void* groupBase;
    void* group;
    void* obj;
    s32 groupIndex;
    s32 objIndex;
    s32 errorOffset;
    char* entry;

    groupBase = (void*)(mapWork + activeGroup * 0x2F4);

    if (param_1 == NULL) {
        obj = NULL;
        goto done;
    }

    group = groupBase;
    groupIndex = 0;
    while (groupIndex < *(s32*)groupBase) {
        obj = *(void**)((s32)group + 0x154);
        objIndex = 0;
        while (objIndex < *(s32*)((s32)group + 0x150)) {
            if (strcmp(*(const char**)*(s32*)((s32)obj + 0x08), param_1) != 0) {
                objIndex++;
                obj = (void*)((s32)obj + 0x134);
            } else {
                goto done;
            }
        }
        group = (void*)((s32)group + 0x178);
        groupIndex++;
    }

    obj = NULL;

done:
    if (obj == NULL) {
        errorOffset = error_count * 0x24;
        entry = error_data + errorOffset;
        *(s32*)(entry + 0x20) = 0;
        strncpy(entry, param_1, 0x1F);
        error_flag = 1;
        error_count++;
        error_count %= 32;
    } else {
        *(u32*)obj = *(u32*)obj | 0x100000;
    }
}

u8 mapObjFlushOff(char* param_1) {
    extern s32 activeGroup;
    extern s32 mapWork;
    extern s32 strcmp(const char*, const char*);
    extern char* strncpy(char* dst, const char* src, u32 n);
    extern char error_data[];
    extern s32 error_count;
    extern s32 error_flag;

    void* groupBase;
    void* group;
    void* obj;
    s32 groupIndex;
    s32 objIndex;
    s32 errorOffset;
    char* entry;

    groupBase = (void*)(mapWork + activeGroup * 0x2F4);

    if (param_1 == NULL) {
        obj = NULL;
        goto done;
    }

    group = groupBase;
    groupIndex = 0;
    while (groupIndex < *(s32*)groupBase) {
        obj = *(void**)((s32)group + 0x154);
        objIndex = 0;
        while (objIndex < *(s32*)((s32)group + 0x150)) {
            if (strcmp(*(const char**)*(s32*)((s32)obj + 0x08), param_1) != 0) {
                objIndex++;
                obj = (void*)((s32)obj + 0x134);
            } else {
                goto done;
            }
        }
        group = (void*)((s32)group + 0x178);
        groupIndex++;
    }

    obj = NULL;

done:
    if (obj == NULL) {
        errorOffset = error_count * 0x24;
        entry = error_data + errorOffset;
        *(s32*)(entry + 0x20) = 0;
        strncpy(entry, param_1, 0x1F);
        error_flag = 1;
        error_count++;
        error_count %= 32;
    } else {
        *(u32*)obj = *(u32*)obj & ~0x100000;
    }
}

void mapGrpFlushOn(s32 group) {
    extern void mapGrpFlagOn(s32, u32);
    mapGrpFlagOn(group, 0x100000);
}

void mapGrpFlushOff(s32 group) {
    extern void mapGrpFlagOff(s32, u32);
    mapGrpFlagOff(group, 0x100000);
}

void mapObjSetFlushColor(char* name, u8 r, u8 g, u8 b, u8 a) {
    extern s32 activeGroup;
    extern s32 mapWork;
    extern s32 strcmp(const char*, const char*);
    extern char* strncpy(char* dst, const char* src, u32 n);
    extern char error_data[];
    extern s32 error_count;
    extern s32 error_flag;
    extern u32 unk_8042952c;

    void* groupBase;
    void* group;
    void* obj;
    s32 groupIndex;
    s32 objIndex;
    u32 color;
    s32 errorOffset;
    char* entry;

    color = unk_8042952c;
    *((u8*)&color + 0) = r;
    *((u8*)&color + 1) = g;
    *((u8*)&color + 2) = b;
    *((u8*)&color + 3) = a;

    groupBase = (void*)(mapWork + activeGroup * 0x2F4);

    if (name == NULL) {
        obj = NULL;
        goto first_done;
    }

    group = groupBase;
    groupIndex = 0;
    while (groupIndex < *(s32*)groupBase) {
        obj = *(void**)((s32)group + 0x154);
        objIndex = 0;
        while (objIndex < *(s32*)((s32)group + 0x150)) {
            if (strcmp(*(const char**)*(s32*)((s32)obj + 0x08), name) != 0) {
                objIndex++;
                obj = (void*)((s32)obj + 0x134);
            } else {
                goto first_done;
            }
        }
        group = (void*)((s32)group + 0x178);
        groupIndex++;
    }

    obj = NULL;

first_done:
    if (obj != NULL) {
        *(u32*)((s32)obj + 0x0C) = color;
        *(u32*)obj = *(u32*)obj | 0x40;
    }

    groupBase = (void*)(mapWork + activeGroup * 0x2F4);

    if (name == NULL) {
        obj = NULL;
        goto second_done;
    }

    group = groupBase;
    groupIndex = 0;
    while (groupIndex < *(s32*)groupBase) {
        obj = *(void**)((s32)group + 0x154);
        objIndex = 0;
        while (objIndex < *(s32*)((s32)group + 0x150)) {
            if (strcmp(*(const char**)*(s32*)((s32)obj + 0x08), name) != 0) {
                objIndex++;
                obj = (void*)((s32)obj + 0x134);
            } else {
                goto second_done;
            }
        }
        group = (void*)((s32)group + 0x178);
        groupIndex++;
    }

    obj = NULL;

second_done:
    if (obj == NULL) {
        errorOffset = error_count * 0x24;
        entry = error_data + errorOffset;
        *(s32*)(entry + 0x20) = 0;
        strncpy(entry, name, 0x1F);
        error_flag = 1;
        error_count++;
        error_count %= 32;
    } else {
        *(u32*)obj = *(u32*)obj & ~0x40;
    }
}

void mapGrpSetFlushColor(s32 group, u8 r, u8 g, u8 b, u8 a) {
    extern s32 unk_80429530;
    extern void mapGrpSetColor(s32, void*);
    extern void mapGrpFlagOff(s32, u32);
    s32 copy;
    s32 color;
    color = unk_80429530;
    ((u8*)&color)[0] = r;
    ((u8*)&color)[1] = g;
    ((u8*)&color)[2] = b;
    ((u8*)&color)[3] = a;
    copy = color;
    mapGrpSetColor(group, &copy);
    mapGrpFlagOff(group, 0x40);
}

void mapObjGetFlushColor(char* pName, void* param_2, void* param_3, void* param_4, void* param_5) {
    extern s32 activeGroup;
    extern s32 mapWork;
    extern s32 strcmp(const char*, const char*);
    extern char* strncpy(char* dst, const char* src, u32 n);
    extern char error_data[];
    extern s32 error_count;
    extern s32 error_flag;

    void* groupBase;
    void* group;
    void* obj;
    s32 groupIndex;
    s32 objIndex;
    s32 errorOffset;
    char* entry;

    groupBase = (void*)(mapWork + activeGroup * 0x2F4);

    if (pName == NULL) {
        obj = NULL;
        goto done;
    }

    group = groupBase;
    groupIndex = 0;
    while (groupIndex < *(s32*)groupBase) {
        obj = *(void**)((s32)group + 0x154);
        objIndex = 0;
        while (objIndex < *(s32*)((s32)group + 0x150)) {
            if (strcmp(*(const char**)*(s32*)((s32)obj + 0x08), pName) != 0) {
                objIndex++;
                obj = (void*)((s32)obj + 0x134);
            } else {
                goto done;
            }
        }
        group = (void*)((s32)group + 0x178);
        groupIndex++;
    }

    obj = NULL;

done:
    if (obj == NULL) {
        errorOffset = error_count * 0x24;
        entry = error_data + errorOffset;
        *(s32*)(entry + 0x20) = 0;
        strncpy(entry, pName, 0x1F);
        error_flag = 1;
        error_count++;
        error_count %= 32;
    } else {
        *(u8*)param_2 = *(u8*)((s32)obj + 0x0C);
        *(u8*)param_3 = *(u8*)((s32)obj + 0x0D);
        *(u8*)param_4 = *(u8*)((s32)obj + 0x0E);
        *(u8*)param_5 = *(u8*)((s32)obj + 0x0F);
    }
}
