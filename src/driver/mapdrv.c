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


u8 bmapUnLoad(void) {
    return 0;
}


u8 makeDisplayList(int param_1) {
    return 0;
}


u8 mapObjSetFlushColor(char* param_1, u8 param_2, u8 param_3, u8 param_4, u8 param_5) {
    return 0;
}


u8 mapSetPolygonVtxDesc(int param_1, int param_2) {
    return 0;
}


void mapGetBoundingBox(void* pMin, void* pMax) {
}


void mapUnLoad(void) {
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


void mapSetPlayRate(s32 id, f32 rate) {
}


void mapObjClearOffScreen(s32 id) {
}


u8 mapReStartAnimation(char* param_1) {
    return 0;
}


u8 mapPauseAnimation(char* param_1) {
    return 0;
}


void mapGrpClearOffScreen(s32 id) {
}


void* mapGetMapObj(s32 name) {
    return 0;
}


u8 test_kururing_mapdisp(s32 cam) {
    return 0;
}


u8 mapSetMaterialFog(void) {
    return 0;
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

u8 mapSetMaterial(void* param_1, void* param_2) {
    return 0;
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
    return 0;
}


void _mapDispMapObj_NoMaterial(s32 param_1, void* param_2) {
}


u8 mapObjSetColor(char* param_1, void* param_2) {
    return 0;
}


s32 mapTestXLU(u32 materialFlag, void* param_2, void* param_3) {
    return 0;
}


void* mapSearchAnmObj(char* param_1) {
    return 0;
}


void* mapEntry(void* pJoint, void* pMtx, int entryIdx) {
    return 0;
}
