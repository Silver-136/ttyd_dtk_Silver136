#include "action/star/sac_deka.h"
#include "event/evt_cmd.h"

extern void* _battleWorkPointer;
s32 evtSetValue(EventEntry* event, s32 target, s32 value);

void disp_3D(void) {
}

void disp_3D_alpha(void) {
}

void* get_ptr(void) {
    return (void*)((s32)_battleWorkPointer + 0x1F4C);
}

USER_FUNC(get_result) {
    void* work = get_ptr();
    evtSetValue(event, event->args[0], *(s32*)((s32)work + 0x44));
    return 2;
}

USER_FUNC(star_stone_attack) {
    void* work = get_ptr();
    if (isFirstCall != 0) {
        *(s32*)((s32)work + 0x3F0) = 0xA;
    }
    return *(s32*)((s32)work + 0x3F0) == 0xD ? 2 : 0;
}

USER_FUNC(star_stone_appear) {
    void* wp;

    wp = get_ptr();
    *(s32*)((s32)wp + 0x3F0) = 1;
    return 2;
}

USER_FUNC(wait_game_end) {
    s32 value = *(s32*)((s32)get_ptr() + 0x30);
    return value == 5 ? 2 : 0;
}

USER_FUNC(start_game) {
    void* wp;

    wp = get_ptr();
    *(s32*)((s32)wp + 0x30) = 1;

    return 2;
}

USER_FUNC(wait_star_stone_take_on) {
    s32 value = *(s32*)((s32)get_ptr() + 0x3F0);
    return value == 4 ? 2 : 0;
}

s32 weaponGetPower_Deka(void) {
    return *(s32*)((s32)get_ptr() + 0x44) + 1;
}
