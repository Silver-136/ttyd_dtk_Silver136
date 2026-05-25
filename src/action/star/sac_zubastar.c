#include "action/star/sac_zubastar.h"

extern void* _battleWorkPointer;

void* memset(void* dst, int value, u32 size);

void* GetZubaStarPtr(void) {
    return (void*)((s32)_battleWorkPointer + 0x1F4C);
}

void zubaStarDispStar(void) {
}

s32 zubastar_init(void) {
    memset(GetZubaStarPtr(), 0, 0x5E0);
    return 2;
}

s32 wait_star_stone_up(void) {
    s32 value = *(s32*)((s32)GetZubaStarPtr() + 0x58C);
    return (value == 4) ? 2 : 0;
}

s32 star_stone_appear(void) {
    s32* ptr = GetZubaStarPtr();

    ptr[0x163] = 1;
    return 2;
}

s32 weaponGetPower_ZubaStar(void* unit, s32* weapon) {
    return weapon[8 + *(s32*)((s32)GetZubaStarPtr() + 0x10)];
}
