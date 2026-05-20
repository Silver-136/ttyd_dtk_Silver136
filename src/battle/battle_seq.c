#include "battle/battle_seq.h"
extern void* _battleWorkPointer;

void _mapobj_data_touch_scale(s32 id) {
    s32 i;
    s32 offset;
    void* entry;
    void* data;

    offset = 0;
    for (i = 0; i < 0x20; i++) {
        entry = (void*)((s32)_battleWorkPointer + 0x1715C + offset);

        if (*(s32*)((s32)entry + 0x0) > 0) {
            data = *(void**)((s32)entry + 0x64);
            if (*(u16*)((s32)data + 0x6) == id) {
                *(s32*)((s32)entry + 0x68) |= 2;
            }
        }

        offset += 0x7C;
    }
}