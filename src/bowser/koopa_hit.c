#include "bowser/koopa_hit.h"

extern u8 kpaFlameHitFilter[];

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

void* kpaGetHitobjHead2(void) {
    s32 offset = 0;
    s32 i = 0;
    void* obj = 0;
    for (; i < 10; i++, offset += 4) {
        obj = *(void**)(*(s32*)((s32)marioGetPtr() + 0x298) + offset + 0x20);
        if (obj != 0) {
            break;
        }
    }
    return obj;
}

