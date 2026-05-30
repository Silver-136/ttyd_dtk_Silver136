#include "driver/hitdrv.h"

extern void* mapGetWork(void);
extern s32 mapGetJoints(void* mapObj);
extern void hitEntrySub(void* mapObj, s32 arg1, void* arg2, s32 arg3, s32 arg4);

extern void hitReCalcMatrix2(void* hit, void* arg, s32 flag);
extern void PSMTXMultVec(void* mtx, void* src, void* dst);
extern f32 PSVECDistance(void* a, void* b);

s32 chkFilterAttr(void* a, void* b) {
    return ((*(u32*)((s32)b + 0x4) & *(u32*)((s32)a + 0x4)) == 0);
}

u32 hitGetAttr(void* hit) {
    if (hit != NULL) {
        return *(u32*)((s32)hit + 0x4);
    }

    return 0;
}

void* hitGetDamageReturnPos(void* hit) {
    return (void*)(*(s32*)((s32)hit + 0xB0) + 0x4);
}

void hitInit(void) {
}

void hitEntryMOBJ(void* mapObj, void* arg) {
    void* mapWork;

    mapWork = mapGetWork();

    *(s32*)((s32)mapWork + 0x158) += mapGetJoints(mapObj);

    hitEntrySub(mapObj, 0, arg, 1, 0);
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


u8 hitCheckVecFilter(void) {
    return 0;
}


void* hitCheckSphereFilter(void* filter, f32 x, f32 y, f32 z, f32 radius) {
    return 0;
}


u8 hitObjGetPosSub(int param_1, s32 param_2, int* param_3, int param_4) {
    return 0;
}


u8 hitCalcVtxPosition(void) {
    return 0;
}


void hitReCalcMatrix2(void* hit, void* arg, s32 flag) {
}


u8 checkTriVec_xz(void) {
    return 0;
}


u8 hitDamageReturnSet(int param_1, s32 param_2, int param_3) {
    return 0;
}


u8 hitAtrOff(int param_1, u32 param_2, int param_3) {
    return 0;
}


u8 hitAtrOn(int param_1, u32 param_2, int param_3) {
    return 0;
}


void hitEntrySub(void* mapObj, s32 arg1, void* arg2, s32 arg3, s32 arg4) {
}


u8 hitMain(void) {
    return 0;
}


void hitObjGetPos(s32 name, void* pos) {
}


u8 hitDelete(char* param_1) {
    return 0;
}


int hitCheckAttr(double param_1, double param_2, double param_3, double param_4, double param_5, double param_6, s32 param_7, void* param_8, void* param_9, void* param_10, void* param_11, void* param_12, void* param_13, void* param_14) {
    return 0;
}


s32 hitCheckFilter(f32 x, f32 y, f32 z, f32 vx, f32 vy, f32 vz, s32 flags, f32* outZ, f32* outY, f32* outX, f32* outDist, void* outA, void* outB, void* outC) {
    return 0;
}


void hitBindUpdate(s32 name) {
}


u8 hitBindMapObj(char* param_1, char* param_2) {
    return 0;
}


u8 hitCheckVecHitObjXZ(void) {
    return 0;
}


u8 hitObjGetNormal(char* param_1, s32 param_2) {
    return 0;
}


u8 hitGrpAttrOff(char* param_1, u32 param_2) {
    return 0;
}


u8 hitGrpAttrOn(char* param_1, u32 param_2) {
    return 0;
}


u8 hitGrpFlagOff(char* param_1, s32 param_2) {
    return 0;
}


u8 hitGrpFlagOn(char* param_1, s32 param_2) {
    return 0;
}


u8 hitObjAttrOff(char* param_1, u32 param_2) {
    return 0;
}


u8 hitObjAttrOn(char* param_1, u32 param_2) {
    return 0;
}


u8 hitObjFlagOff(char* param_1, u16 param_2) {
    return 0;
}


u8 hitObjFlagOn(char* param_1, u16 param_2) {
    return 0;
}


u8 hitGrpDamageReturnSet(char* param_1, s32 param_2) {
    return 0;
}


u8 hitReInit(void) {
    return 0;
}


s32 hitGetName(void* pHit) {
    return 0;
}


void* hitEntry(void* param_1, void* param_2, int idx) {
    return 0;
}


void* _hitEnt(void* param_1, s32 param_2, s32 param_3, int param_4) {
    return 0;
}


u8 hitFlgOff(void* param_1, s32 param_2, int param_3) {
    return 0;
}


u8 hitFlgOn(void* param_1, s32 param_2, int param_3) {
    return 0;
}


void* hitNameToPtr(char* hitName) {
    return 0;
}
