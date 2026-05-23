#include "motion/mot_jabara.h"

void mot_grasp(void) {
    extern void* marioGetPtr(void);

    void* mario = marioGetPtr();
    u32 flags = *(u32*)((s32)mario + 0xC);
    if (flags & 1) {
        *(u32*)((s32)mario + 0xC) = flags & ~1;
        *(u32*)mario &= ~0xF0000;
    }
}

s32 marioPriCheckJabara(void) {
    extern void* marioGetPtr(void);
    extern s32 pouchCheckItem(s32 itemId);

    void* mario = marioGetPtr();
    u32 flags = *(u32*)mario;
    s32 item;

    if ((flags & 0x80000000) || (flags & 0x02100000)) {
        *(u8*)((s32)mario + 0x40) = 0;
        return 0;
    }
    item = pouchCheckItem(8);
    return ((u32)(-item) | (u32)item) >> 31;
}
