#include "action/star/sac_zubastar.h"

extern void* _battleWorkPointer;

void* memset(void* dst, int value, u32 size);
void GXSetTexCopySrc(s32 left, s32 top, s32 wd, s32 ht);
void GXSetTexCopyDst(s32 wd, s32 ht, s32 fmt, s32 mipmap);
void GXCopyTex(void* dest, s32 clear);
void GXPixModeSync(void);

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

void zubastar_capture(void) {
    void* work = GetZubaStarPtr();

    GXSetTexCopySrc(0xD0, 0, 0x190, 0x1E0);
    GXSetTexCopyDst(0x190, 0x1E0, 4, 0);
    GXCopyTex(*(void**)*(s32*)((s32)work + 0x588), 0);
    GXPixModeSync();
}
