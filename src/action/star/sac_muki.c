#include "action/star/sac_muki.h"
#include "event/evt_cmd.h"

extern void* _battleWorkPointer;

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

