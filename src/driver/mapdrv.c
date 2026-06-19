#include "driver/mapdrv.h"

void* getMapDataDvdRoot(void) {
    extern char str__8041f928[1];
    return str__8041f928;
}

s32 mapGetActiveGroup(void) {
    extern s32 activeGroup;
    return activeGroup;
}

s32 mapPreLoad(void) {
    return 1;
}

void N_mapDispOff(void) {
    extern s32 unk_8041e678;
    unk_8041e678 = 1;
}

void N_mapDispOn(void) {
    extern s32 unk_8041e678;
    unk_8041e678 = 0;
}

void mapResetPaperAmbColor(void) {
    extern s32 dat_8041f90c;
    extern s32 paper_ambient;
    paper_ambient = dat_8041f90c;
}

void mapSetPaperAmbColor(void* color) {
    extern s32 paper_ambient;
    paper_ambient = *(s32*)color;
}

void* mapGetWork(void) {
    extern s32 activeGroup;
    extern s32 mapWork;
    return (void*)(mapWork + activeGroup * 0x2F4);
}

void mapDispMapGrp(void) {
    extern void _mapDispMapGrp(void);
    _mapDispMapGrp();
}

void mapDispMapObj(void) {
    extern void _mapDispMapObj(void);
    _mapDispMapObj();
}

void mapSetTevCallback(s32 index, void* callback) {
    extern s32 activeGroup;
    extern s32 mapWork;
    *(void**)(mapWork + activeGroup * 0x2F4 + 0x110 + index * 4) = callback;
}

void mapGrpFlushOff(s32 group) {
    extern void mapGrpFlagOff(s32, u32);
    mapGrpFlagOff(group, 0x100000);
}

void mapGrpFlushOn(s32 group) {
    extern void mapGrpFlagOn(s32, u32);
    mapGrpFlagOn(group, 0x100000);
}

void mapSetProjMtx(void* mtx) {
    extern void PSMTXCopy(void*, void*);
    extern s32 projMtx[12];
    PSMTXCopy(mtx, projMtx);
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

void mapGetFog(void* type, f32* start, f32* end, void* color) {
    extern s32 activeGroup;
    extern s32 mapWork;
    s32 work = mapWork + activeGroup * 0x2F4;
    *(s32*)type = *(s32*)(work + 0x28);
    *start = *(f32*)(work + 0x2C);
    *end = *(f32*)(work + 0x30);
    *(s32*)color = *(s32*)(work + 0x34);
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

void* mapSearchDmdJoint(void* data, char* name) {
    extern void* mapSearchDmdJointSub(void*, char*);
    extern void* mapSearchDmdJointSub2(void*, char*, char*);
    extern char str__8041f94c[1];
    if ((s32)*(u8*)name != '|') {
        return mapSearchDmdJointSub(data, name);
    }
    return mapSearchDmdJointSub2(data, name, str__8041f94c);
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

void mapSetProjTexObj(void* texObj, void* imgRef) {
    typedef struct TexObjCopy {
        s32 word[8];
    } TexObjCopy;
    extern TexObjCopy projTexObj;
    extern void* projTexImgRef;
    projTexObj = *(TexObjCopy*)texObj;
    projTexImgRef = imgRef;
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


void mapSetMaterialTev(u32 texCount, int drawMode, u32 materialFlag, s32 pMtx) {
}


u8 mapMain(void) {
    return 0;
}


void _mapDispMapObj(void) {
}


u8 mapDisp(void) {
    return 0;
}


u8 spline_maketable(int param_1, float* param_2, float* param_3, float* param_4) {
    return 0;
}


void* mapSearchDmdJointSub(void*, char*) {
    return 0;
}


u8 mapSetMaterialLight(u32 materialLightFlag, void* pPos) {
    return 0;
}


u8 mapObjGetPosSub(int param_1, s32 param_2, int* param_3, int param_4) {
    return 0;
}


void _mapDispMapGrp_NoMaterial(s32, void*) {
}


u8 mapSetPolygon(int param_1, int param_2) {
    return 0;
}


void* mapSearchDmdJointSub2(void*, char*, char*) {
    return 0;
}


void _mapDispMapGrp(void) {
}


u8 mapSetTextureMatrix(void) {
    return 0;
}


u8 mapFlgOff(u32* param_1, u32 param_2, int param_3) {
    return 0;
}


void mapGetJointsSub(void*, s32*) {
}


u8 mapPlayAnimationLv(char* param_1, int param_2, int param_3) {
    return 0;
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

u8 makeDisplayList(int param_1) {
    return 0;
}


void mapObjSetFlushColor(char* name, u8 r, u8 g, u8 b, u8 a) {
    extern void* mapGetMapObj(char* name);
    extern void mapErrorEntry(s32 type, char* message);
    void* obj;
    u32 color;

    color = 0;
    *((u8*)&color + 0) = r;
    *((u8*)&color + 1) = g;
    *((u8*)&color + 2) = b;
    *((u8*)&color + 3) = a;

    obj = mapGetMapObj(name);
    if (obj != NULL) {
        *(u32*)((s32)obj + 0xC) = color;
        *(u32*)obj |= 0x40;
    }

    obj = mapGetMapObj(name);
    if (obj == NULL) {
        mapErrorEntry(0, name);
    } else {
        *(u32*)obj &= ~0x40;
    }
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

void mapGetBoundingBox(void* pMin, void* pMax) {
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

u8 mapObjRotate(s64 x, s64 y, s64 z, s32 pName) {
    return 0;
}


u8 bmapLoad(char* rankStageName, char* battleStageName) {
    return 0;
}


u8 spline_getvalue(double param_1, float* param_2, int param_3, float* param_4, int param_5, int param_6) {
    return 0;
}


u8 mapDispMapObj_bbox(int param_1, int param_2) {
    return 0;
}


u8 mapObjGetPos(char* param_1, void* param_2) {
    return 0;
}


u8 mapObjTranslate(void) {
    return 0;
}


void mapObjGetFlushColor(char* pName, void* param_2, void* param_3, void* param_4, void* param_5) {
}


void mapCheckAnimation(s32 id, s32* outDone, f32* outFrame) {
}


u8 mapObjFlagOn(char* param_1, u32 param_2) {
    return 0;
}


u8 mapObjScale(s64 param_1, s64 param_2, s64 param_3, char* param_4) {
    return 0;
}


void mapGrpFlagOff(s32, u32) {
}


void mapGrpFlagOn(s32, u32) {
}


u8 mapObjFlagOff(char* param_1, u32 param_2) {
    return 0;
}


u8 mapObjFlushOff(char* param_1) {
    return 0;
}


u8 mapObjFlushOn(char* param_1) {
    return 0;
}


u8 mapGrpSetOffScreen(char* param_1, char* param_2) {
    return 0;
}


u8 mapObjSetOffScreen(char* param_1, char* param_2) {
    return 0;
}


void mapGrpSetColor(s32, void*) {
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


void mapObjClearOffScreen(s32 id) {
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


void mapGrpClearOffScreen(s32 id) {
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


u8 test_kururing_mapdisp(s32 cam) {
    return 0;
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


void mapBlendOff2(void) {
    extern s32 activeGroup;
    extern s32 mapWork;
    ((u16*)mapWork)[activeGroup * 0x17A + 2] &= ~4;
}

void mapBlendOff(void) {
    extern s32 activeGroup;
    extern s32 mapWork;
    ((u16*)mapWork)[activeGroup * 0x17A + 2] &= ~2;
}

void mapFogOff(void) {
    extern s32 activeGroup;
    extern s32 mapWork;
    ((u16*)mapWork)[activeGroup * 0x17A + 2] &= ~1;
}

void mapFogOn(void) {
    extern s32 activeGroup;
    extern s32 mapWork;
    ((u16*)mapWork)[activeGroup * 0x17A + 2] |= 1;
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


void _mapLoad(void* pWork, int idx, char* pMapName) {
}


void* _mapEnt(void* pJoint, void* pParent, void* pMtx, int entryIdx) {
    return 0;
}


u8 _setOffScrnId(void* param_1, int param_2, int param_3) {
    return 0;
}


u8 setColor(void* param_1, u32* param_2, int param_3) {
    return 0;
}


u8 mapFlgOn(void* param_1, u32 param_2, int param_3) {
    return 0;
}


u8 mapReCalcMatrix(void* this, s32 param_2, int param_3) {
    return 0;
}


u8 mapCalcAnimMatrix(void* pDstMtx, void* pParentMtx, void* pEntry, void* pTrack) {
    return 0;
}


u8 mapSetMaterialLastStageBlend(u32 flags, void* pEvtColor, void* param_3) {
    return 0;
}


void* mapEntrySub(void* pJoint, void* pParent, void* pMtx, int isRoot, int entryIdx) {
    return 0;
}


u8 mapBuildTexture(void* param_1, int param_2, int* param_3) {
    return 0;
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

void _mapDispMapObj_NoMaterial(s32 param_1, void* param_2) {
}


u8 mapObjSetColor(char* param_1, void* param_2) {
    return 0;
}


s32 mapTestXLU(u32 materialFlag, void* param_2, void* param_3) {
    return 0;
}


void* mapSearchAnmObj(char* name) {
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


void* mapEntry(void* pJoint, void* pMtx, int entryIdx) {
    return 0;
}
