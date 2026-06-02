#include "bowser/koopa_hit.h"

s32 kpaFlameHitFilter(void* unused, void* obj);

void* marioGetPtr(void);
void* hitCheckSphereFilter(void* filter, f32 x, f32 y, f32 z, f32 radius);

s32 kpaSearchFrontUnder(void) {
    return 0;
}

void* kpaGetHitFlame(void) {
    void* mario = marioGetPtr();
    if (*(s8*)((s32)mario + 0x3C) != 2) {
        return 0;
    }
    return *(void**)(*(s32*)((s32)mario + 0x298) + 0x18);
}

void* kpaGetHitobj(void) {
    void* mario = marioGetPtr();
    if (*(s8*)((s32)mario + 0x3C) != 2) {
        return 0;
    }
    return *(void**)(*(s32*)((s32)mario + 0x298) + 0x1C);
}

void* kpaGetHitobjHead(s32 index) {
    s32 offset = index * 4;
    void* base = *(void**)((s32)marioGetPtr() + 0x298);
    return *(void**)((s32)base + offset + 0x20);
}

void* kpaGetHitobjRide(s32 index) {
    s32 offset = index * 4;
    void* base = *(void**)((s32)marioGetPtr() + 0x298);
    return *(void**)((s32)base + offset + 0x4C);
}

void* kpaFlameHitCheck(f32 x, f32 y, f32 z, f32 radius) {
    marioGetPtr();
    return hitCheckSphereFilter(kpaFlameHitFilter, x, y, z, radius);
}

typedef struct KpaMobjName {
    u8 pad[0x15];
    char name[1];
} KpaMobjName;

s32 kpaFlameHitFilter(void* unused, void* obj) {
    extern KpaMobjName* mobjHitObjPtrToPtr(void* hitObj);
    extern s32 strcmp(const char* str1, const char* str2);
    extern const char str_MOBJ_HiddenHatenaBlo_802fec9c[];
    extern const char str_MOBJ_Block_802fecb4[];

    KpaMobjName* mobj = mobjHitObjPtrToPtr(obj);
    s32 cmp;

    if (strcmp(mobj->name, str_MOBJ_HiddenHatenaBlo_802fec9c) == 0) {
        return 0;
    }
    cmp = strcmp(mobj->name, str_MOBJ_Block_802fecb4);
    return ((u32)(-cmp) | (u32)cmp) >> 31;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* kpaGetHitobjHead2(void) {
    s32 offset = 0;
    s32 i = 0;
    void* obj = 0;
    for (; i < 10; i++, offset += 4) {
        void* base = *(void**)((s32)marioGetPtr() + 0x298);
        s32 index = offset + 0x20;
        obj = *(void**)((s32)base + index);
        if (obj != 0) {
            break;
        }
    }
    return obj;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

void kpaClearHitobjRide(void) {
    void* mario = marioGetPtr();

    *(void**)(*(s32*)((s32)mario + 0x298) + 0x4C) = 0;
    *(void**)(*(s32*)((s32)mario + 0x298) + 0x50) = 0;
    *(void**)(*(s32*)((s32)mario + 0x298) + 0x54) = 0;
    *(void**)(*(s32*)((s32)mario + 0x298) + 0x58) = 0;
    *(void**)(*(s32*)((s32)mario + 0x298) + 0x5C) = 0;
    *(void**)(*(s32*)((s32)mario + 0x298) + 0x60) = 0;
    *(void**)(*(s32*)((s32)mario + 0x298) + 0x64) = 0;
    *(void**)(*(s32*)((s32)mario + 0x298) + 0x68) = 0;
    *(void**)(*(s32*)((s32)mario + 0x298) + 0x6C) = 0;
    *(void**)(*(s32*)((s32)mario + 0x298) + 0x70) = 0;
}
