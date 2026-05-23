#include "mario/mario_hit.h"

s32 chkFilterAttr2(void* a, void* b) {
    u32 attr = *(u32*)((s32)b + 0x4);
    return ((attr & *(u32*)((s32)a + 0x4)) == 0 || (attr & *(u32*)((s32)a + 0x8)) != 0);
}

void clear_hitobj_ride(void) {
    extern void* marioGetPtr(void);
    extern void kpaClearHitobjRide(void);

    void* mario = marioGetPtr();
    *(s32*)((s32)mario + 0x1E8) = 0;
    if ((s32)*(u8*)((s32)mario + 0x3C) == 2) {
        kpaClearHitobjRide();
    }
}
