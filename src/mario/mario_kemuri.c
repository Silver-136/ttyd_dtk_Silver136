#include "mario/mario_kemuri.h"

void* marioGetPtr(void);
u32 hitGetAttr(void* hitObj);
void* effKemuriEntry(s32 kind, f32 x, f32 y, f32 z, f32 zero);
extern f32 float_0_80422a38;
extern f32 float_0p6_80422a3c;


u8 kemuri_mugi(void) {
    return 0;
}


u8 kemuri(void) {
    return 0;
}


u8 party_kemuri(void* party) {
    return 0;
}


u8 mario_kemuri_landon(void) {
    return 0;
}


u8 mario_kemuri(void) {
    return 0;
}


u8 mario_simple_kemuri(void) {
    return 0;
}


void unk_8013587c(void) {
    void* mario;
    void* hitObj;
    void* effect;
    void* effectWork;
    u32 flags;
    u32 attr;

    mario = marioGetPtr();
    hitObj = *(void**)((s32)mario + 0x1E8);
    if (hitObj != 0) {
        flags = *(u32*)((s32)mario + 0xC);
        if ((flags & 0x20000) != 0) {
            *(u32*)((s32)mario + 0xC) = flags & ~0x20000;
        } else {
            attr = hitGetAttr(hitObj);
            if ((attr & 0x1000) != 0) {
                kemuri_mugi();
            } else if ((attr & 0x100) != 0) {
                mario_kemuri_landon();
            } else {
                effect = effKemuriEntry(0,
                                        *(f32*)((s32)mario + 0x8C),
                                        *(f32*)((s32)mario + 0x90),
                                        *(f32*)((s32)mario + 0x94),
                                        float_0_80422a38);
                if ((*(u32*)mario & 0x02000000) != 0) {
                    effectWork = *(void**)((s32)effect + 0xC);
                    *(f32*)((s32)effectWork + 0x74) = float_0p6_80422a3c;
                }
            }
        }
    }
}
