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

s32 zubastar_end(void) {
    extern void* GetZubaStarPtr(void);
    extern void effDelete(void* effect);
    extern void BattleFree(void* ptr);
    extern void smartFree(void* ptr);

    void* work;
    void* obj;
    s32 i;
    s32 offset;

    work = GetZubaStarPtr();
    effDelete(*(void**)((s32)*(void**)((s32)*(void**)((s32)work + 0x5DC) + 0xC) + 0x34));
    effDelete(*(void**)((s32)work + 0x5DC));
    offset = 0;
    for (i = 0; i < 0xC8; i++) {
        obj = *(void**)((s32)work + offset + 0xA8);
        if (obj != 0) {
            if (*(void**)((s32)obj + 8) != 0) {
                BattleFree(*(void**)((s32)obj + 8));
                *(void**)((s32)obj + 8) = 0;
            }
            BattleFree(obj);
            *(void**)((s32)work + offset + 0xA8) = 0;
        }
        offset += 4;
    }
    smartFree(*(void**)((s32)work + 0x588));
    return 2;
}

void zubastar_create_takaku(s32 count, void* points) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;
    extern void* GetZubaStarPtr(void);
    extern void* BattleAlloc(u32 size);
    extern const Vec vec3_802fffb8;

    void* work;
    void* obj;
    void* copy;
    s32 i;
    s32 offset;

    work = GetZubaStarPtr();
    *(void**)((s32)work + (*(s32*)((s32)work + 0x14) << 2) + 0xA8) = BattleAlloc(0x50);
    obj = *(void**)((s32)work + (*(s32*)((s32)work + 0x14) << 2) + 0xA8);
    copy = BattleAlloc(count * 0xC);
    *(void**)((s32)obj + 8) = copy;
    *(s32*)((s32)obj + 0) = 0;
    *(s32*)((s32)obj + 4) = count;
    *(Vec*)((s32)obj + 0xC) = vec3_802fffb8;
    *(s32*)((s32)obj + 0x48) = 0;
    *(s32*)((s32)obj + 0x4C) = 1;
    offset = 0;
    for (i = 0; i < count; i++) {
        *(Vec*)((s32)*(void**)((s32)obj + 8) + offset) = *(Vec*)((s32)points + offset);
        offset += 0xC;
    }
    *(s32*)((s32)work + 0x14) = *(s32*)((s32)work + 0x14) + 1;
}

s32 zubastar_get_kouten(f32* outX, f32* outY, f32 x1, f32 y1, f32 x2, f32 y2, f32 x3, f32 y3, f32 x4, f32 y4) {
    extern f32 float_0_80427d40;
    extern f32 float_1Eneg05_80427dc0;
    extern f32 float_1_80427d30;

    f32 dx1;
    f32 dx2;
    f32 slope1;
    f32 slope2;
    f32 denom;
    f32 zero;
    f32 t;

    dx1 = x2 - x1;
    zero = float_0_80427d40;
    if (zero == dx1 && zero == (x4 - x3)) {
        return 0;
    }
    if (zero != dx1) {
        dx1 = dx1;
    } else {
        dx1 = float_1Eneg05_80427dc0;
    }
    dx2 = x4 - x3;
    if (zero != dx2) {
        dx2 = dx2;
    } else {
        dx2 = float_1Eneg05_80427dc0;
    }
    slope1 = (y2 - y1) / dx1;
    slope2 = (y4 - y3) / dx2;
    denom = slope1 - slope2;
    if (zero == denom) {
        return 0;
    }
    *outX = ((y3 - y1) + (slope2 * x3 - slope1 * x1)) / denom;
    if (zero != (x2 - x1)) {
        *outY = slope1 * (*outX - x1) + y1;
    } else {
        *outY = slope2 * (*outX - x3) + y3;
    }
    t = (*outX - x3) / dx2;
    if (t >= zero && t <= float_1_80427d30) {
        return 1;
    }
    return 0;
}
