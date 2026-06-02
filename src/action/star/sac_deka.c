#include "action/star/sac_deka.h"
#include "event/evt_cmd.h"

extern void* _battleWorkPointer;
s32 evtSetValue(EventEntry* event, s32 target, s32 value);
void effDelete(void* effect);
void smartFree(void* ptr);
void mapGrpFlagOff(const char* name, s32 flag);
extern const char str_stg_b_yuka_80301050[];

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
    if (*(s32*)((s32)work + 0x3F0) == 0xD) {
        return 2;
    }
    return 0;
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

USER_FUNC(end_deka) {
    void* work = get_ptr();

    effDelete(*(void**)(*(s32*)(*(s32*)((s32)work + 0x44C) + 0xC) + 0x34));
    effDelete(*(void**)((s32)work + 0x44C));
    return 2;
}

USER_FUNC(yuka_end) {
    void* work = get_ptr();

    smartFree(*(void**)work);
    **(s32**)work = 0;
    smartFree(*(void**)((s32)work + 0x2C));
    **(s32**)((s32)work + 0x2C) = 0;
    mapGrpFlagOff(str_stg_b_yuka_80301050, 0x400001);
    return 2;
}
