#include "action/star/sac_scissor.h"
#include "event/evt_cmd.h"

extern void* _battleWorkPointer;
void* BattleAlloc(u32 size);
void BattleFree(void* ptr);
void* memset(void* dst, int value, u32 size);
s32 evtSetValue(EventEntry* event, s32 target, s32 value);
void dispEntry(s32 cameraId, s32 layer, void* callback, f32 z, void* param);
extern f32 float_0_80427c94;
extern void scissor_disp_region(void);
extern void star_disp2D_sub(s32 id, s32 alpha);

void* GetScissorPtr(void) {
    return (void*)((s32)_battleWorkPointer + 0x1F4C);
}

USER_FUNC(star_stone_appear) {
    void* wp;

    wp = GetScissorPtr();
    *(s32*)((s32)wp + 0x11C) = 1;
    return 2;
}

USER_FUNC(scissor_end) {
    extern void effDelete(void* eff);
    extern void smartFree(void* ptr);
    void* work;
    void* texture;

    work = GetScissorPtr();
    effDelete(*(void**)((s32)*(void**)((s32)*(void**)((s32)work + 0x16C) + 0xC) + 0x34));
    effDelete(*(void**)((s32)work + 0x16C));
    texture = *(void**)((s32)work + 0x170);
    if (texture != 0) {
        smartFree(texture);
        *(void**)((s32)work + 0x170) = 0;
    }
    BattleFree(*(void**)((s32)work + 0x114));
    BattleFree(*(void**)((s32)work + 0x118));
    return 2;
}

USER_FUNC(wait_star_standby) {
    s32 state = *(s32*)((s32)GetScissorPtr() + 0x11C);
    return (state == 5) ? 2 : 0;
}

USER_FUNC(scissor_offscreen) {
    GetScissorPtr();
    dispEntry(1, 1, scissor_disp_region, float_0_80427c94, 0);
    return 2;
}

USER_FUNC(scissor_init) {
    void* work = GetScissorPtr();
    f32 zero;

    memset(work, 0, 0x174);
    *(s32*)((s32)work + 0x104) = 0x3BF;
    *(void**)((s32)work + 0x114) = BattleAlloc(0x1000);
    *(void**)((s32)work + 0x118) = BattleAlloc(0x3000);
    zero = float_0_80427c94;
    *(f32*)((s32)work + 0xB4) = zero;
    *(f32*)((s32)work + 0xB8) = zero;
    *(s32*)((s32)work + 0xC) = 0;
    *(u8*)((s32)work + 0x98) = 0;
    return 2;
}

USER_FUNC(scissor_timer_get) {
    void* work;
    s32* args;
    s32 timer;

    work = GetScissorPtr();
    args = event->args;

    timer = *(s32*)((s32)work + 0x104);
    if (timer <= 0x3B) {
        *(s32*)((s32)work + 0x11C) = 6;
    }

    evtSetValue(event, args[0], *(s32*)((s32)work + 0x104));

    return 2;
}

USER_FUNC(star_disp2D) {
    s32 state = *(s32*)((s32)GetScissorPtr() + 0x11C);
    if (state >= 2 && state <= 6) {
        star_disp2D_sub(0, 0xFF);
        star_disp2D_sub(1, 0xC0);
        star_disp2D_sub(2, 0x80);
        star_disp2D_sub(3, 0x40);
        star_disp2D_sub(4, 0x20);
    }
}

USER_FUNC(scissor_ofscreen_ok) {
    s32* args = event->args;
    void* work = GetScissorPtr();

    if ((*(u32*)work & 1) != 0) {
        evtSetValue(event, args[0], 1);
    } else {
        evtSetValue(event, args[0], 0);
    }
    return 2;
}

USER_FUNC(scissor_capture) {
    extern void GXSetTexCopySrc(s32 left, s32 top, s32 width, s32 height);
    extern void GXSetTexCopyDst(s32 width, s32 height, s32 format, s32 mipmap);
    extern void GXCopyTex(void* dest, s32 clear);
    extern void GXPixModeSync(void);
    void* work;

    work = GetScissorPtr();
    GXSetTexCopySrc(0, 0, 0x260, 0x1E0);
    GXSetTexCopyDst(0x260, 0x1E0, 4, 0);
    GXCopyTex(**(void***)((s32)work + 0x170), 0);
    GXPixModeSync();
}

USER_FUNC(scissor_tess) {
    extern void* memcpy(void* dest, const void* src, u32 size);
    extern void tess(void* entries, s32 count);
    extern void DCFlushRange(void* addr, u32 nBytes);
    void* work;
    void* copy;
    s32 count;

    work = GetScissorPtr();
    count = *(s32*)((s32)work + 0x9C);
    copy = BattleAlloc(count * 8);
    memcpy(copy, *(void**)((s32)work + 0x114), *(s32*)((s32)work + 0x9C) * 8);
    *(s32*)((s32)work + 0x14) = 0;
    tess(copy, *(s32*)((s32)work + 0x9C));
    DCFlushRange(*(void**)((s32)work + 0x118), 0x3000);
    return 2;
}

s32 scissor_intersection(f32 x1, f32 y1, f32 x2, f32 y2, f32 x3, f32 y3, f32 x4, f32 y4);

s32 scissor_intersection2(f32 x1, f32 y1, f32 x2, f32 y2, f32 x3, f32 y3, f32 x4, f32 y4) {
    if (x1 == x3 && y1 == y3) {
        return 0;
    }
    if (x1 == x4 && y1 == y4) {
        return 0;
    }
    if (x2 == x3 && y2 == y3) {
        return 0;
    }
    if (x2 == x4 && y2 == y4) {
        return 0;
    }
    return scissor_intersection(x1, y1, x2, y2, x3, y3, x4, y4);
}

s32 weaponGetPower_Scissor(void* weapon, void* attacker, void* target) {
    extern void* GetScissorPtr(void);
    extern s32 BtlUnit_GetUnitId(void* unit);

    void* work;
    s32 i;
    s32 offset;

    work = GetScissorPtr();
    if (target == 0) {
        return 0;
    }
    if (*(u8*)((s32)target + 0xC) != 1) {
        return 0;
    }
    offset = 0;
    for (i = 0; i < 0x10; i++) {
        if (*(s32*)((s32)work + offset + 0x18) == BtlUnit_GetUnitId(target)) {
            return *(s32*)((s32)work + offset + 0x58);
        }
        offset += 4;
    }
    return 0;
}

s32 scissor_intersection4(f32* outX, f32* outY, f32 x1, f32 y1, f32 x2, f32 y2, f32 x3, f32 y3, f32 x4, f32 y4) {
    extern s32 scissor_intersection3(f32* outX, f32* outY, f32 x1, f32 y1, f32 x2, f32 y2, f32 x3, f32 y3, f32 x4, f32 y4);

    if (x1 == x3 && y1 == y3) {
        *outX = x1;
        *outY = y1;
        return 1;
    }
    if (x1 == x4 && y1 == y4) {
        *outX = x1;
        *outY = y1;
        return 1;
    }
    if (x2 == x3 && y2 == y3) {
        *outX = x2;
        *outY = y2;
        return 1;
    }
    if (x2 == x4 && y2 == y4) {
        *outX = x2;
        *outY = y2;
        return 1;
    }
    return scissor_intersection3(outX, outY, x1, y1, x2, y2, x3, y3, x4, y4);
}

s32 scissor_intersection3(f32* outX, f32* outY, f32 x1, f32 y1, f32 x2, f32 y2, f32 x3, f32 y3, f32 x4, f32 y4) {
    extern f32 float_0_80427c94;
    extern f32 float_1000_80427d10;
    extern f32 float_neg1000_80427cfc;
    extern s32 scissor_intersection(f32 x1, f32 y1, f32 x2, f32 y2, f32 x3, f32 y3, f32 x4, f32 y4);

    f32 dx;
    f32 zero;
    f32 slope;
    f32 amount;

    dx = x2 - x1;
    zero = float_0_80427c94;
    if (zero != dx) {
        slope = (y2 - y1) / dx;
        if (x1 >= x2) {
            amount = float_1000_80427d10;
            slope = amount * slope;
            x1 += amount;
            x2 -= amount;
            y1 += slope;
            y2 -= slope;
        } else {
            amount = float_1000_80427d10;
            slope = amount * slope;
            x1 -= amount;
            x2 += amount;
            y1 -= slope;
            y2 += slope;
        }
    } else if (y1 >= y2) {
        y1 = float_1000_80427d10;
        y2 = float_neg1000_80427cfc;
    } else {
        y1 = float_neg1000_80427cfc;
        y2 = float_1000_80427d10;
    }
    return scissor_intersection(x1, y1, x2, y2, x3, y3, x4, y4);
}

s32 scissor_timer_main(void) {
    extern void* GetScissorPtr(void);
    extern f32 float_320_80427ce8;
    extern f32 float_185_80427cec;
    extern f32 float_120_80427cf0;
    extern f32 intplGetValue(s32 type, s32 current, s32 total, f32 start, f32 end);

    void* work;
    s32 timer;
    s32 counter;

    work = GetScissorPtr();
    switch (*(u32*)((s32)work + 0xFC)) {
        case 0:
            *(f32*)((s32)work + 0x108) = float_320_80427ce8;
            *(f32*)((s32)work + 0x10C) = float_185_80427cec;
            *(s32*)((s32)work + 0xFC) = 5;
            *(s32*)((s32)work + 0x100) = 0;
            /* fallthrough */
        case 5:
            *(f32*)((s32)work + 0x108) = intplGetValue(
                4,
                *(s32*)((s32)work + 0x100),
                0xF,
                float_320_80427ce8,
                float_120_80427cf0);
            counter = *(s32*)((s32)work + 0x100) + 1;
            *(s32*)((s32)work + 0x100) = counter;
            if (*(s32*)((s32)work + 0x100) > 0xF) {
                *(s32*)((s32)work + 0xFC) = 0xA;
            }
            break;
        case 10:
            timer = *(s32*)((s32)work + 0x104) - 1;
            *(s32*)((s32)work + 0x104) = timer;
            if (*(s32*)((s32)work + 0x104) <= 0x3B) {
                *(s32*)((s32)work + 0xFC) = 0xF;
                *(s32*)((s32)work + 0x100) = 0;
            }
            break;
        case 15:
            *(f32*)((s32)work + 0x108) = intplGetValue(
                4,
                *(s32*)((s32)work + 0x100),
                0xF,
                float_120_80427cf0,
                float_320_80427ce8);
            counter = *(s32*)((s32)work + 0x100) + 1;
            *(s32*)((s32)work + 0x100) = counter;
            if (*(s32*)((s32)work + 0x100) > 0xF) {
                *(s32*)((s32)work + 0xFC) = 0x14;
            }
            timer = *(s32*)((s32)work + 0x104) - 1;
            *(s32*)((s32)work + 0x104) = timer;
            if (*(s32*)((s32)work + 0x104) < 0) {
                *(s32*)((s32)work + 0x104) = 0;
            }
            break;
        case 20:
            timer = *(s32*)((s32)work + 0x104) - 1;
            *(s32*)((s32)work + 0x104) = timer;
            if (*(s32*)((s32)work + 0x104) < 0) {
                *(s32*)((s32)work + 0x104) = 0;
                return 2;
            }
            break;
    }
    return 0;
}

s32 scissor_damage_sub2(f32* pos) {
    extern void* GetScissorPtr(void);
    extern f32 float_0_80427c94;

    void* work;
    f32 x;
    f32 y;
    f32 zero;
    f32 cross[3];
    f32* tri;
    s32 count;
    s32 i;
    s32 j;
    s32 next;
    s32 offset;

    work = GetScissorPtr();
    count = *(s32*)((s32)work + 0x14);
    x = pos[0];
    y = pos[1];
    zero = float_0_80427c94;
    offset = 0;
    for (i = 0; i < count; i++) {
        tri = (f32*)((s32)*(void**)((s32)work + 0x118) + offset);
        for (j = 0; j < 3; j++) {
            next = (j + 1) % 3;
            cross[j] = ((x - tri[j]) * (tri[next + 3] - tri[j + 3])) - ((y - tri[j + 3]) * (tri[next] - tri[j]));
        }
        if (cross[0] > zero && cross[1] > zero && cross[2] > zero) {
            return 1;
        }
        if (cross[0] < zero && cross[1] < zero && cross[2] < zero) {
            return 1;
        }
        offset += 0x18;
    }
    return 0;
}
