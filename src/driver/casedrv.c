#include "driver/casedrv.h"

extern void* wp;
extern void* __memAlloc();

u8 caseMain(void) {
    return 0;
}


u8 caseInit(void) {
    *(s32*)wp = 0x40;
    *(void**)((s32)wp + 4) = __memAlloc(0, *(s32*)wp * 0xA0);
    memset(*(void**)((s32)wp + 4), 0, *(s32*)wp * 0xA0);
}


void caseReInit(void) {
    void* entry = *(void**)((s32)wp + 4);
    s32 i = 0;

    while (i < *(s32*)wp) {
        *(u16*)entry &= ~1;
        i++;
        entry = (void*)((s32)entry + 0xA0);
    }
}


void caseDelete(s32 id) {
    void* entry = (void*)((s32)*(void**)((s32)wp + 4) + id * 0xA0);

    *(u16*)entry &= ~1;
}


int caseEntry(void* param_1) {
    return 0;
}


void* caseCheckHitObj2(u32 param_1) {
    return 0;
}


void* caseCheckHitObj(void* pHit) {
    return 0;
}


void* caseIdToPtr(s32 id) {
    void* entry = (void*)((s32)*(void**)((s32)wp + 4) + id * 0xA0);

    if (*(u16*)entry & 1) {
        return entry;
    }
    return NULL;
}
