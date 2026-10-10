#pragma once
#include <dolphin/types.h>

/* Source-local pose-index selector shared by render call sites. */
static inline s32 marioRenderPoseIndex(void) {
    extern void* mp;
    u32 flags = *(u32*)((u8*)mp + 4);
    if ((flags & 0x10000000) == 0) {
        if ((flags & 0x80000000) == 0) {
            return 0;
        } else {
            return 1;
        }
    } else {
        return 2;
    }
}
