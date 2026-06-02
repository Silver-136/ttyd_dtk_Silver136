#include "action/star/sac_muki.h"
#include "event/evt_cmd.h"

extern void* _battleWorkPointer;
void effDelete(void* effect);

void* get_ptr(void) {
    return (void*)((s32)_battleWorkPointer + 0x1F4C);
}

USER_FUNC(star_stone_appear) {
    void* wp;

    wp = get_ptr();
    *(s32*)((s32)wp + 0x3B4) = 1;
    return 2;
}

USER_FUNC(start_game) {
    void* wp;

    wp = get_ptr();
    *(s32*)((s32)wp + 0x4) = 1;

    return 2;
}

USER_FUNC(end_muki) {
    s32 offset;
    void* work;
    s32 i;

    work = get_ptr();
    effDelete(*(void**)(*(s32*)(*(s32*)((s32)work + 0x404) + 0xC) + 0x34));
    effDelete(*(void**)((s32)work + 0x404));
    i = 0;
    offset = 0;
    for (; i < 9; i++, offset += 0x38) {
        effDelete(*(void**)((s32)work + offset + 0x1EC));
    }
    return 2;
}
