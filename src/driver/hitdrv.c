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