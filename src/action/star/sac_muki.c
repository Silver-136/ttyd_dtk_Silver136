#include "action/star/sac_muki.h"
#include "event/evt_cmd.h"

extern void* _battleWorkPointer;

void* get_ptr(void) {
    return (void*)((s32)_battleWorkPointer + 0x1F4C);
}

s32 star_stone_appear(void) {
    *(s32*)((s32)get_ptr() + 0x3B4) = 1;
    return 2;
}

s32 start_game(void) {
    *(s32*)((s32)get_ptr() + 4) = 1;
    return 2;
}

