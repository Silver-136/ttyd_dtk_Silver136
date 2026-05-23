#include "battle/battle_seq.h"
extern void* _battleWorkPointer;

void _mapobj_data_touch_scale(s32 id) {
    s32 offset = 0;
    void* battleWork = _battleWorkPointer;
    s32 i;

    for (i = 0; i < 0x20; i++, offset += 0x7C) {
        void* entry = (void*)((s32)battleWork + offset + 0x1715C);

        if (*(s32*)entry > 0) {
            void* data = *(void**)((s32)entry + 0x64);
            if (*(u16*)((s32)data + 6) == id) {
                *(u32*)((s32)entry + 0x68) |= 2;
            }
        }
    }
}
