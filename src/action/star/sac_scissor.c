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

s32 scissor_control(s32 param_1, s32 reset) {
    extern f32 sqrt(f64 x);
    extern s32 keyGetStickX(s32 pad);
    extern s32 keyGetStickY(s32 pad);
    extern void psndSFXOn(const char* name);
    extern void smartFree(void* ptr);
    extern s32 GXGetTexBufferSize(s32 width, s32 height, s32 format, s32 mipmap, s32 roundUp);
    extern void* smartAlloc(s32 size, s32 group);
    extern void scissor_disp_control(void);
    extern f32 float_0_80427c94;
    extern f32 float_42_80427ccc;
    extern f32 float_50_80427cd0;
    extern f32 float_0p86_80427cd4;
    extern f32 float_neg300_80427cd8;
    extern f32 float_300_80427cdc;
    extern f32 float_neg200_80427ce0;
    extern f32 float_200_80427ce4;
    extern f64 double_5_802fff48;
    extern const char str_SFX_BTL_SAC_KAKONDEP_802fff50[];
    extern void* gp;
    void* work;
    f32* points;
    s32 count;
    s32 i;
    s32 j;
    s32 off;
    f32 oldLen;
    f32 newLen;
    f32 dx;
    f32 dy;
    f32 ix;
    f32 iy;
    f32 adx;
    f32 ady;
    f32* cur;
    f32* last;
    f32 zero;
    s32 (*intersect)(f32*, f32*, f32, f32, f32, f32, f32, f32, f32, f32);
    work = GetScissorPtr();
    points = *(f32**)((s32)work + 0x114);
    if (reset != 0) {
        zero = float_0_80427c94;
        *(s32*)((s32)work + 0x9C) = 0;
        *(f32*)((s32)work + 0xA8) = zero;
        *(f32*)((s32)work + 0xA4) = zero;
        *(f32*)((s32)work + 0xB0) = zero;
        *(f32*)((s32)work + 0xAC) = zero;
        *(s32*)((s32)work + 0xA0) = 0;
        *(f32*)((s32)work + 0xD4) = zero;
        *(f32*)((s32)work + 0xD8) = zero;
        memset(points, 0, 0x1000);
        *(u32*)work &= ~1u;
    }
    *(f32*)((s32)work + 0xA4) = *(f32*)((s32)work + 0xAC);
    *(f32*)((s32)work + 0xA8) = *(f32*)((s32)work + 0xB0);
    *(f32*)((s32)work + 0xAC) = (f32)(s8)keyGetStickX(0);
    *(f32*)((s32)work + 0xB0) = (f32)(s8)keyGetStickY(0);
    oldLen = sqrt((*(f32*)((s32)work + 0xA4) * *(f32*)((s32)work + 0xA4)) +
                  (*(f32*)((s32)work + 0xA8) * *(f32*)((s32)work + 0xA8)));
    (void)oldLen;
    newLen = sqrt((*(f32*)((s32)work + 0xAC) * *(f32*)((s32)work + 0xAC)) +
                  (*(f32*)((s32)work + 0xB0) * *(f32*)((s32)work + 0xB0)));
    if (newLen > float_42_80427ccc) {
        *(f32*)((s32)work + 0xD4) += *(f32*)((s32)work + 0xAC) / float_50_80427cd0;
        *(f32*)((s32)work + 0xD8) += *(f32*)((s32)work + 0xB0) / float_50_80427cd0;
    }
    *(f32*)((s32)work + 0xD4) *= float_0p86_80427cd4;
    *(f32*)((s32)work + 0xD8) *= float_0p86_80427cd4;
    *(f32*)((s32)work + 0xB4) += *(f32*)((s32)work + 0xD4);
    *(f32*)((s32)work + 0xB8) += *(f32*)((s32)work + 0xD8);
    if (*(f32*)((s32)work + 0xB4) < float_neg300_80427cd8) *(f32*)((s32)work + 0xB4) = float_neg300_80427cd8;
    if (*(f32*)((s32)work + 0xB4) > float_300_80427cdc) *(f32*)((s32)work + 0xB4) = float_300_80427cdc;
    if (*(f32*)((s32)work + 0xB8) < float_neg200_80427ce0) *(f32*)((s32)work + 0xB8) = float_neg200_80427ce0;
    if (*(f32*)((s32)work + 0xB8) > float_200_80427ce4) *(f32*)((s32)work + 0xB8) = float_200_80427ce4;
    if (*(s32*)((s32)work + 0x104) <= 0x3B) {
        *(s32*)((s32)work + 0x11C) = 6;
        return 2;
    }
    if (((*(s32*)((s32)gp + 0x1C) % 6) == 0) && (*(s32*)((s32)work + 0x9C) < 0x200)) {
        if ((*(f32*)((s32)work + 0xBC) != *(f32*)((s32)work + 0xB4)) ||
            (*(f32*)((s32)work + 0xC0) != *(f32*)((s32)work + 0xB8))) {
            dx = *(f32*)((s32)work + 0xB4) - *(f32*)((s32)work + 0xBC);
            dy = *(f32*)((s32)work + 0xB8) - *(f32*)((s32)work + 0xC0);
            adx = dx;
            if (adx < float_0_80427c94) adx = -adx;
            ady = dy;
            if (ady < float_0_80427c94) ady = -ady;
            if ((f64)adx >= double_5_802fff48 || (f64)ady >= double_5_802fff48) psndSFXOn(str_SFX_BTL_SAC_KAKONDEP_802fff50);
            count = *(s32*)((s32)work + 0x9C);
            points[count * 2] = *(f32*)((s32)work + 0xB4);
            points[count * 2 + 1] = *(f32*)((s32)work + 0xB8);
            *(f32*)((s32)work + 0xBC) = *(f32*)((s32)work + 0xB4);
            *(f32*)((s32)work + 0xC0) = *(f32*)((s32)work + 0xB8);
            *(s32*)((s32)work + 0x9C) = count + 1;
        }
    }
    count = *(s32*)((s32)work + 0x9C);
    if (count > 3) {
        intersect = (s32 (*)(f32*, f32*, f32, f32, f32, f32, f32, f32, f32, f32))scissor_intersection;
        off = 0;
        for (i = 0; i < count - 3; i++) {
            cur = (f32*)((s32)points + off);
            last = &points[count * 2];
            if (intersect(&ix, &iy, cur[0], cur[1], cur[2], cur[3], last[-2], last[-1], last[0], last[1])) {
                last[-2] = ix; cur[0] = ix; last[-1] = iy; cur[1] = iy;
                for (j = 0; j < i; j++) { points[j * 2] = cur[0]; points[j * 2 + 1] = cur[1]; }
                if (*(void**)((s32)work + 0x170) != 0) { smartFree(*(void**)((s32)work + 0x170)); *(void**)((s32)work + 0x170) = 0; }
                *(void**)((s32)work + 0x170) = smartAlloc(GXGetTexBufferSize(0x260, 0x1E0, 4, 0, 0), 1);
                dispEntry(8, 0, scissor_capture, float_0_80427c94, 0);
                return 2;
            }
            off += 8;
        }
    }
    dispEntry(8, 0, scissor_disp_control, float_0_80427c94, 0);
    return 0;
}

s32 star_control(void) {
    extern void* BattleGetMarioPtr(void* battleWork);
    extern void BtlUnit_GetPos(void* unit, f32* x, f32* y, f32* z);
    extern void* effStarStoneEntry(f32 x, f32 y, f32 z, f32 scale, s32 type);
    extern f32 intplGetValue(s32 type, s32 current, s32 total, f32 start, f32 end);
    extern void* gp;
    extern void* _battleWorkPointer;
    extern f32 float_0_80427c94;
    extern f32 float_neg1000_80427cfc;
    extern f32 float_1_80427cb4;
    extern f32 float_37_80427d00;
    extern f32 float_50_80427cd0;
    extern f32 float_neg1_80427cb0;
    extern f32 float_2_80427d04;
    extern f32 float_2160_80427d08;
    extern f32 float_300_80427cdc;
    extern f32 float_1p5_80427d0c;
    extern char str_btl_wn_sac_scissor_802ffe78[];

    void* work;
    void* mario;
    void* eff;
    void* effWork;
    f32* hist;
    char* ro;
    s32 state;
    s32 i;
    s32 offset;

    ro = str_btl_wn_sac_scissor_802ffe78;
    work = GetScissorPtr();
    hist = (f32*)((s32)work + 0x9C);
    mario = BattleGetMarioPtr(_battleWorkPointer);

    if ((*(u32*)((s32)gp + 0x1C) & 3) == 0) {
        offset = 0x0C;
        for (i = 0; i < 3; i++) {
            *(f32*)((s32)hist + offset + 0x40) = *(f32*)((s32)hist + offset + 0x3C);
            *(f32*)((s32)hist + offset + 0x50) = *(f32*)((s32)hist + offset + 0x4C);
            offset -= 4;
        }
        hist[0x10] = *(f32*)((s32)work + 0xB4);
        hist[0x14] = *(f32*)((s32)work + 0xB8);
    }

    state = *(s32*)((s32)work + 0x11C);
    switch (state) {
        case 1:
            *(s32*)((s32)work + 0x11C) = 2;
            *(s32*)((s32)work + 0x120) = 0;
            *(void**)((s32)work + 0x16C) = effStarStoneEntry(float_0_80427c94, float_neg1000_80427cfc, float_0_80427c94, float_1_80427cb4, 3);

            BtlUnit_GetPos(mario, (f32*)((s32)work + 0x124), (f32*)((s32)work + 0x128), (f32*)((s32)work + 0x12C));

            *(f32*)((s32)work + 0x128) +=
                (*(f32*)((s32)mario + 0x114) * (f32)*(s16*)((s32)mario + 0xCE)) + float_37_80427d00;

            *(u32*)((s32)work + 0x130) = *(u32*)((s32)work + 0x124);
            *(u32*)((s32)work + 0x134) = *(u32*)((s32)work + 0x128);
            *(u32*)((s32)work + 0x138) = *(u32*)((s32)work + 0x12C);
            *(u32*)((s32)work + 0x13C) = *(u32*)((s32)work + 0x124);
            *(u32*)((s32)work + 0x140) = *(u32*)((s32)work + 0x128);
            *(u32*)((s32)work + 0x144) = *(u32*)((s32)work + 0x12C);
            *(f32*)((s32)work + 0x140) += float_50_80427cd0;
            *(f32*)((s32)work + 0x144) += float_neg1_80427cb0;

            *(u32*)((s32)work + 0x154) = *(u32*)((s32)ro + 0x2C);
            *(u32*)((s32)work + 0x158) = *(u32*)((s32)ro + 0x30);
            *(u32*)((s32)work + 0x15C) = *(u32*)((s32)ro + 0x34);
            *(u32*)((s32)work + 0x160) = *(u32*)((s32)ro + 0x38);
            *(u32*)((s32)work + 0x164) = *(u32*)((s32)ro + 0x3C);
            *(u32*)((s32)work + 0x168) = *(u32*)((s32)ro + 0x40);

            /* fall through into state 2 update like target */
        case 2:
            *(s32*)((s32)work + 0x120) = *(s32*)((s32)work + 0x120) + 1;
            if (*(s32*)((s32)work + 0x120) <= 0x64) {
                *(f32*)((s32)work + 0x124) = intplGetValue(0, *(s32*)((s32)work + 0x120), 0x64, *(f32*)((s32)work + 0x130), *(f32*)((s32)work + 0x13C));
                *(f32*)((s32)work + 0x128) = intplGetValue(0, *(s32*)((s32)work + 0x120), 0x64, *(f32*)((s32)work + 0x134), *(f32*)((s32)work + 0x140));
                *(f32*)((s32)work + 0x12C) = intplGetValue(0, *(s32*)((s32)work + 0x120), 0x64, *(f32*)((s32)work + 0x138), *(f32*)((s32)work + 0x144));
                *(f32*)((s32)work + 0x154) = intplGetValue(0, *(s32*)((s32)work + 0x120), 0x64, float_0_80427c94, float_2_80427d04);
                *(f32*)((s32)work + 0x158) = *(f32*)((s32)work + 0x154);
                *(f32*)((s32)work + 0x15C) = *(f32*)((s32)work + 0x154);
            } else {
                *(u32*)((s32)work + 0x124) = *(u32*)((s32)work + 0x13C);
                *(u32*)((s32)work + 0x128) = *(u32*)((s32)work + 0x140);
                *(u32*)((s32)work + 0x12C) = *(u32*)((s32)work + 0x144);
                *(f32*)((s32)work + 0x154) = float_2_80427d04;
                *(f32*)((s32)work + 0x158) = float_2_80427d04;
                *(f32*)((s32)work + 0x15C) = float_2_80427d04;
            }
            *(f32*)((s32)work + 0x164) = intplGetValue(4, *(s32*)((s32)work + 0x120), 0x78, float_0_80427c94, float_2160_80427d08);
            if (*(s32*)((s32)work + 0x120) >= 0x78) {
                *(s32*)((s32)work + 0x11C) = 3;
                *(s32*)((s32)work + 0x120) = 0;
                *(u32*)((s32)work + 0x130) = *(u32*)((s32)work + 0x124);
                *(u32*)((s32)work + 0x134) = *(u32*)((s32)work + 0x128);
                *(u32*)((s32)work + 0x138) = *(u32*)((s32)work + 0x12C);
                *(f32*)((s32)work + 0x140) = float_300_80427cdc;
            }
            break;

        case 3:
            *(s32*)((s32)work + 0x120) = *(s32*)((s32)work + 0x120) + 1;
            *(f32*)((s32)work + 0x128) = intplGetValue(1, *(s32*)((s32)work + 0x120), 0x3C, *(f32*)((s32)work + 0x134), *(f32*)((s32)work + 0x140));
            if (*(s32*)((s32)work + 0x120) >= 0x3C) {
                *(s32*)((s32)work + 0x11C) = 4;
                *(s32*)((s32)work + 0x120) = 0;
                *(u32*)((s32)work + 0x124) = *(u32*)((s32)ro + 0x44);
                *(u32*)((s32)work + 0x128) = *(u32*)((s32)ro + 0x48);
                *(u32*)((s32)work + 0x12C) = *(u32*)((s32)ro + 0x4C);
                *(u32*)((s32)work + 0x154) = *(u32*)((s32)ro + 0x50);
                *(u32*)((s32)work + 0x158) = *(u32*)((s32)ro + 0x54);
                *(u32*)((s32)work + 0x15C) = *(u32*)((s32)ro + 0x58);
                *(u32*)((s32)work + 0x160) = *(u32*)((s32)ro + 0x5C);
                *(u32*)((s32)work + 0x164) = *(u32*)((s32)ro + 0x60);
                *(u32*)((s32)work + 0x168) = *(u32*)((s32)ro + 0x64);
                *(f32*)((s32)work + 0x134) = *(f32*)((s32)work + 0x128);
                *(f32*)((s32)work + 0x140) = float_0_80427c94;
                hist[6] = *(f32*)((s32)work + 0x124);
                hist[7] = *(f32*)((s32)work + 0x128);
                offset = 0;
                for (i = 0; i < 4; i++) {
                    *(f32*)((s32)hist + offset + 0x40) = float_0_80427c94;
                    *(f32*)((s32)hist + offset + 0x50) = float_neg1000_80427cfc;
                    offset += 4;
                }
            }
            break;

        case 4:
            *(s32*)((s32)work + 0x120) = *(s32*)((s32)work + 0x120) + 1;
            *(f32*)((s32)work + 0x128) = intplGetValue(4, *(s32*)((s32)work + 0x120), 0x3C, *(f32*)((s32)work + 0x134), *(f32*)((s32)work + 0x140));
            hist[6] = *(f32*)((s32)work + 0x124);
            hist[7] = *(f32*)((s32)work + 0x128);
            if (*(s32*)((s32)work + 0x120) >= 0x3C) {
                *(s32*)((s32)work + 0x11C) = 5;
                *(s32*)((s32)work + 0x120) = 0;
            }
            break;

        default:
            break;
    }

    eff = *(void**)((s32)work + 0x16C);
    if (eff != 0) {
        effWork = *(void**)((s32)eff + 0xC);
        if (*(s32*)((s32)work + 0x11C) < 4) {
            *(f32*)((s32)effWork + 0x08) = *(f32*)((s32)work + 0x124);
            *(f32*)((s32)effWork + 0x0C) = *(f32*)((s32)work + 0x128);
            *(f32*)((s32)effWork + 0x10) = *(f32*)((s32)work + 0x12C);
        } else {
            *(f32*)((s32)effWork + 0x08) = float_0_80427c94;
            *(f32*)((s32)effWork + 0x0C) = float_neg1000_80427cfc;
            *(f32*)((s32)effWork + 0x10) = float_0_80427c94;
        }
        *(f32*)((s32)effWork + 0x18) = *(f32*)((s32)work + 0x160);
        *(f32*)((s32)effWork + 0x1C) = *(f32*)((s32)work + 0x164);
        *(f32*)((s32)effWork + 0x20) = *(f32*)((s32)work + 0x168);
        *(f32*)((s32)effWork + 0x14) = float_1p5_80427d0c * *(f32*)((s32)work + 0x154);
    }

    if (*(s32*)((s32)work + 0x11C) >= 4 && *(s32*)((s32)work + 0x11C) <= 6) {
        dispEntry(8, 2, star_disp2D, float_0_80427c94, 0);
    }

    return 0;
}

s32 scissor_cross(f32* pos1, f32* pos2, f32* poly, s32 count) {
    extern f32 float_neg1000_80427cfc;
    extern f32 float_1_80427cb4;
    extern f32 float_1000_80427d10;
    extern f32 float_0p5_80427ca8;

    f32 outX;
    f32 outY;
    f32 xs[512];
    f32 ys[512];
    s32 num;
    s32 i;
    s32 j;
    s32 k;
    s32 next;
    s32 found;
    f32* cur;
    f32* nxt;
    f32* prev;
    f32 mid;
    f32 tx;
    f32 ty;
    f32 tmp;

    for (i = 0; i < count; i++) {
        next = (i + 1) % count;
        cur = &poly[i * 2];
        nxt = &poly[next * 2];
        if (scissor_intersection2(pos1[0], pos1[1], pos2[0], pos2[1], cur[0], cur[1], nxt[0], nxt[1]) != 0) {
            return 0;
        }
    }

    ys[0] = float_neg1000_80427cfc;
    xs[0] = float_neg1000_80427cfc;
    num = 1;

    for (i = 0; i < count; i++) {
        cur = &poly[i * 2];
        next = (i + 1) % count;
        nxt = &poly[next * 2];
        if (scissor_intersection4(&outX, &outY,
                                  pos1[0] + float_1_80427cb4, pos1[1],
                                  pos2[0] + float_1_80427cb4, pos2[1],
                                  cur[0], cur[1], nxt[0], nxt[1]) != 0) {
            found = 0;
            if (cur[0] == outX && cur[1] == outY) {
                if (i == 0) {
                    prev = &poly[(count - 1) * 2];
                } else {
                    prev = &poly[(i - 1) * 2];
                }
                if (scissor_intersection3(&tx, &ty, pos1[0], pos1[1], pos2[0], pos2[1], prev[0], prev[1], nxt[0], nxt[1]) != 0) {
                    found = 1;
                }
            }
            if (found != 0) {
                for (j = 0; j < count; j++) {
                    if (xs[j] == outX && ys[j] == outY) {
                        break;
                    }
                }
                if (j == count) {
                    xs[num] = outX;
                    ys[num] = outY;
                    num++;
                }
            } else {
                xs[num] = outX;
                ys[num] = outY;
                num++;
            }
        }
    }

    ys[num] = float_1000_80427d10;
    xs[num] = float_1000_80427d10;

    if (pos1[0] != pos2[0]) {
        for (i = 0; i < num; i++) {
            for (j = i + 1; j < num + 1; j++) {
                if (xs[i] > xs[j]) {
                    tmp = xs[i]; xs[i] = xs[j]; xs[j] = tmp;
                    tmp = ys[i]; ys[i] = ys[j]; ys[j] = tmp;
                }
            }
        }
        mid = float_0p5_80427ca8 * (pos1[0] + pos2[0]);
        for (i = 0; i < num; i++) {
            if (xs[i] <= mid && mid < xs[i + 1]) {
                return i & 1;
            }
        }
    } else {
        for (i = 0; i < num; i++) {
            for (j = i + 1; j < num + 1; j++) {
                if (ys[i] > ys[j]) {
                    tmp = ys[i]; ys[i] = ys[j]; ys[j] = tmp;
                    tmp = xs[i]; xs[i] = xs[j]; xs[j] = tmp;
                }
            }
        }
        mid = float_0p5_80427ca8 * (pos1[1] + pos2[1]);
        for (i = 0; i < num; i++) {
            if (ys[i] <= mid && mid < ys[i + 1]) {
                return i & 1;
            }
        }
    }

    return 0;
}


/* CHATGPT STUB FILL: main/action/star/sac_scissor 20260624_184823 */

/* stub-fill: scissor_disp_region | prototype_only | source_prototype */
void scissor_disp_region(void) {
    extern void* GetScissorPtr(void);
    extern void sysWaitDrawSync(void);
    extern void GXClearBoundingBox(void);
    extern void GXSetCullMode(s32 mode);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32 attr, s32 type);
    extern void GXSetVtxAttrFmt(s32 vtxfmt, s32 attr, s32 type, s32 frac, s32 shift);
    extern void GXInitTexObj(void* obj, void* image, s32 width, s32 height, s32 format, s32 wrapS, s32 wrapT, s32 mipmap);
    extern void GXInitTexObjLOD(void* obj, s32 minFilt, s32 magFilt, f32 minLod, f32 maxLod, f32 lodBias, s32 biasClamp, s32 doEdgeLod, s32 maxAniso);
    extern void GXLoadTexObj(void* obj, s32 mapid);
    extern void GXSetNumChans(s32 count);
    extern void GXSetNumTexGens(s32 count);
    extern void C_MTXLightOrtho(f32 t, f32 b, f32 l, f32 r, f32 scaleS, f32 scaleT, f32 transS, f32 transT, void* mtx);
    extern void* camGetPtr(s32 id);
    extern void PSMTXConcat(void* a, void* b, void* out);
    extern void GXLoadTexMtxImm(void* mtx, s32 id, s32 type);
    extern void GXSetTexCoordGen2(s32 coord, s32 func, s32 src, s32 mtx, s32 normalize, s32 postmtx);
    extern void GXSetNumTevStages(s32 count);
    extern void GXSetTevOrder(s32 stage, s32 coord, s32 map, s32 color);
    extern void GXSetTevColorOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 out);
    extern void GXSetTevAlphaOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 out);
    extern void GXSetTevColorIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevAlphaIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevKAlphaSel(s32 stage, s32 sel);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXBegin(s32 prim, s32 vtxfmt, s32 nverts);
    extern void smartFree(void* ptr);
    extern void GXReadBoundingBox(u16* left, u16* top, u16* right, u16* bottom);
    extern s32 offscreenNameToId(char* name);
    extern void offscreenAddBoundingBox(s32 id, u16 left, u16 top, u16 right, u16 bottom);
    extern f32 float_0_80427c94;
    extern f32 float_0p5_80427ca8;
    extern f32 float_neg240_80427c98;
    extern f32 float_240_80427c9c;
    extern f32 float_neg304_80427ca0;
    extern f32 float_304_80427ca4;
    extern char str_ofs_80427c88[];

    void* work;
    void* texObj[8];
    f32 mtx[3][4];
    void* cam;
    f32* tri;
    s32 i;
    u16 left;
    u16 top;
    u16 right;
    u16 bottom;
    volatile f32* fifo;

    work = GetScissorPtr();
    sysWaitDrawSync();
    GXClearBoundingBox();
    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXInitTexObj(texObj, *(void**)((s32)work + 0x170), 0x260, 0x1E0, 4, 0, 0, 0);
    GXInitTexObjLOD(texObj, 1, 1, float_0_80427c94, float_0_80427c94, float_0_80427c94, 0, 0, 0);
    GXLoadTexObj(texObj, 0);
    GXSetNumChans(1);
    GXSetNumTexGens(1);
    C_MTXLightOrtho(float_neg240_80427c98, float_240_80427c9c,
                    float_neg304_80427ca0, float_304_80427ca4,
                    float_0p5_80427ca8, float_0p5_80427ca8,
                    float_0p5_80427ca8, float_0p5_80427ca8, mtx);
    cam = camGetPtr(4);
    PSMTXConcat(mtx, (void*)((s32)cam + 0xA0), mtx);
    GXLoadTexMtxImm(mtx, 0x1E, 0);
    GXSetTexCoordGen2(0, 0, 0, 0x1E, 0, 0x7D);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0xF, 0xF, 0xF, 8);
    GXSetTevAlphaIn(0, 7, 7, 7, 6);
    GXSetTevKAlphaSel(0, 0);
    cam = camGetPtr(4);
    GXLoadPosMtxImm((void*)((s32)cam + 0xA0), 0);
    tri = *(f32**)((s32)work + 0x118);
    GXBegin(0x90, 0, (s16)(*(s32*)((s32)work + 0x14) * 3));
    fifo = (volatile f32*)0xCC008000;
    for (i = 0; i < *(s32*)((s32)work + 0x14); i++) {
        *fifo = tri[0];
        *fifo = tri[3];
        *fifo = float_0_80427c94;
        *fifo = tri[1];
        *fifo = tri[4];
        *fifo = float_0_80427c94;
        *fifo = tri[2];
        *fifo = tri[5];
        *fifo = float_0_80427c94;
        tri += 6;
    }
    sysWaitDrawSync();
    if (*(void**)((s32)work + 0x170) != 0) {
        smartFree(*(void**)((s32)work + 0x170));
        *(void**)((s32)work + 0x170) = 0;
    }
    GXReadBoundingBox(&left, &top, &right, &bottom);
    if (left == 0x3FF && right == 0 && top == 0x3FF && bottom == 0) {
        *(u32*)work &= ~1;
    } else {
        *(u32*)work |= 1;
    }
    offscreenAddBoundingBox(offscreenNameToId(str_ofs_80427c88), left, top, right, bottom);
}

/* stub-fill: scissor_disp_control | prototype_only | source_prototype */
void scissor_disp_control(s32 cameraId) {
    extern void* GetScissorPtr(void);
    extern f32 intplGetValue(f32 start, f32 end, s32 type, s32 current, s32 total);
    extern void GXSetCullMode(s32 mode);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32 attr, s32 type);
    extern void GXSetVtxAttrFmt(s32 vtxfmt, s32 attr, s32 type, s32 frac, s32 shift);
    extern void GXSetNumChans(s32 count);
    extern void GXSetChanCtrl(s32 chan, s32 enable, s32 ambSrc, s32 matSrc, s32 lightMask, s32 diffFn, s32 attnFn);
    extern void GXSetNumTexGens(s32 count);
    extern void GXSetNumTevStages(s32 count);
    extern void GXSetTevOrder(s32 stage, s32 coord, s32 map, s32 color);
    extern void GXSetTevColorOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 out);
    extern void GXSetTevAlphaOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 out);
    extern void GXSetTevOp(s32 stage, s32 mode);
    extern void* camGetPtr(s32 id);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXSetCurrentMtx(s32 id);
    extern void GXSetLineWidth(s32 width, s32 fmt);
    extern u32 HSV2RGB(void* hsv);
    extern void GXSetChanMatColor(s32 chan, void* color);
    extern void GXBegin(s32 prim, s32 vtxfmt, s32 nverts);
    extern void* gpGlobals;
    extern f32 float_0_80427c94;
    extern f32 float_255_80427cac;

    void* work;
    f32* verts;
    void* cam;
    u32 hsv;
    u32 color;
    s32 timer;
    s32 i;
    s32 offset;
    s32 hueAdd;
    volatile f32* fifo;

    work = GetScissorPtr();
    verts = *(f32**)((s32)work + 0x114);
    *(s32*)((s32)work + 0x10) = *(s32*)((s32)work + 0x10) + 1;
    if (*(s32*)((s32)work + 0x10) > 0x3B) {
        *(s32*)((s32)work + 0x10) = *(s32*)((s32)work + 0x10) - 0x3C;
    }
    timer = *(s32*)((s32)work + 0x10);
    if (timer < 0x1E) {
        intplGetValue(float_0_80427c94, float_255_80427cac, 0, timer, 0x1D);
    } else {
        intplGetValue(float_255_80427cac, float_0_80427c94, 0, timer - 0x1E, 0x3B);
    }
    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0xFF, 0xFF, 4);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevOp(0, 4);
    cam = camGetPtr(cameraId);
    GXLoadPosMtxImm((void*)((s32)cam + 0xA0), 0);
    GXSetCurrentMtx(0);
    GXSetLineWidth(0x10, 0);
    fifo = (volatile f32*)0xCC008000;
    offset = 0;
    hueAdd = 0;
    for (i = 0; i < *(s32*)((s32)work + 0x9C) - 1U; i++) {
        hsv = (((*(u32*)((s32)gpGlobals + 0x1C) + hueAdd) % 0x7F) << 1) << 24;
        color = HSV2RGB(&hsv);
        GXSetChanMatColor(4, &color);
        GXBegin(0xA8, 0, 2);
        *fifo = *(f32*)((s32)verts + offset);
        *fifo = *(f32*)((s32)verts + offset + 4);
        *fifo = float_0_80427c94;
        *fifo = *(f32*)((s32)verts + offset + 8);
        *fifo = *(f32*)((s32)verts + offset + 0xC);
        *fifo = float_0_80427c94;
        offset += 8;
        hueAdd += 2;
    }
}

/* stub-fill: scissor_damage_sub | missing_definition | ghidra_signature */
void scissor_damage_sub(void* unit, s32* out) {
    extern void BtlUnit_GetPos(void* unit, f32* x, f32* y, f32* z);
    extern void btlGetScreenPoint(f32* in, f32* out);
    extern s32 scissor_damage_sub2(f32* pos);
    extern f32 float_neg1_80427cb0;
    extern f32 float_1_80427cb4;
    extern f32 float_0p5_80427ca8;
    extern f32 float_9_80427cb8;
    extern f32 float_100_80427cbc;
    extern f32 float_0p3_80427cc0;
    extern f32 float_0p6_80427cc4;
    extern f32 float_0p9_80427cc8;

    f32 pos[3];
    f32 test[3];
    f32 screen[3];
    f32 side;
    f32 width;
    f32 height;
    f32 halfWidth;
    f32 halfHeight;
    f32 ratio;
    s32 hits;
    s32 i;
    s32 j;

    hits = 0;
    if ((*(u32*)((s32)unit + 0x104) & 2) != 0) {
        side = float_neg1_80427cb0;
    } else {
        side = float_1_80427cb4;
    }

    width = *(f32*)((s32)unit + 0xE4) * *(f32*)((s32)unit + 0x114);
    height = *(f32*)((s32)unit + 0xE8) * *(f32*)((s32)unit + 0x114);
    BtlUnit_GetPos(unit, &pos[0], &pos[1], &pos[2]);
    pos[0] += *(f32*)((s32)unit + 0xD8) * *(f32*)((s32)unit + 0x114);
    pos[1] += side * (*(f32*)((s32)unit + 0xDC) * *(f32*)((s32)unit + 0x114));

    halfHeight = float_0p5_80427ca8 * -height;
    halfWidth = float_0p5_80427ca8 * width;

    for (i = 0; i < 10; i++) {
        f32 xStep = ((f32)i * width) / float_9_80427cb8;
        for (j = 0; j < 10; j++) {
            test[0] = (pos[0] - halfWidth) + xStep;
            test[1] = pos[1] + side * (halfHeight + (((f32)j * height) / float_9_80427cb8));
            test[2] = pos[2];
            btlGetScreenPoint(test, test);
            screen[0] = test[0];
            screen[1] = test[1];
            screen[2] = test[2];
            if (scissor_damage_sub2(screen) != 0) {
                hits++;
            }
        }
    }

    ratio = (f32)hits / float_100_80427cbc;
    *out = 0;
    if (ratio >= float_0p3_80427cc0) {
        *out = 1;
    }
    if (ratio >= float_0p6_80427cc4) {
        *out = 2;
    }
    if (ratio >= float_0p9_80427cc8) {
        *out = 3;
    }
}

/* stub-fill: scissor_damage | missing_definition | ghidra_signature */
s32 scissor_damage(EventEntry* event) {
    extern void* _battleWorkPointer;
    extern void* GetScissorPtr(void);
    extern void* BattleGetMarioPtr(void* battleWork);
    extern void* BattleGetUnitPtr(void* battleWork, s32 id);
    extern s8 BtlUnit_GetBelong(void* unit);
    extern s32 BtlUnit_GetBodyPartsId(void* unit);
    extern void* BtlUnit_GetPartsPtr(void* unit, s32 partsId);
    extern s32 BtlUnit_CheckStatus(void* unit, s32 status);
    extern void scissor_damage_sub(void* unit, s32* out);
    extern void BattleCheckDamage(void* attacker, void* target, void* parts, void* weapon, s32 flags);
    extern s32 BattleAudience_GetAudienceNum(void);
    extern void BattleAudienceSoundCallKind(s32 kind);
    extern void BattleAudienceSoundHandBeat(void);
    extern void BattleAudienceSoundSetVol(s32 kind, u8 volume, s32 frames);
    extern s32 evtSetValue(EventEntry* event, s32 target, s32 value);
    extern void weapon_scissor;

    void* battleWork;
    void* mario;
    void* work;
    void* unit;
    void* parts;
    s32* args;
    s32 total;
    s32 allHit;
    s32 didDamage;
    s32 audience;
    s32 damage;
    s32 i;
    s32 offset;

    battleWork = _battleWorkPointer;
    args = event->args;
    mario = BattleGetMarioPtr(battleWork);
    work = GetScissorPtr();
    total = 0;
    offset = 0;
    allHit = 1;
    audience = BattleAudience_GetAudienceNum();
    didDamage = 0;

    for (i = 0; i < 0x40; i++) {
        unit = BattleGetUnitPtr(battleWork, i);
        if (unit != 0 && BtlUnit_GetBelong(unit) == 1) {
            parts = BtlUnit_GetPartsPtr(unit, BtlUnit_GetBodyPartsId(unit));
            if (BtlUnit_CheckStatus(unit, 0x1B) != 1 &&
                ((*(u32*)((s32)parts + 0x1AC) & 0x00004000) == 0) &&
                ((*(u32*)((s32)parts + 0x1AC) & 0x20000000) == 0) &&
                ((*(u32*)((s32)parts + 0x1AC) & 0x00010000) == 0) &&
                (s8)**(u8**)((s32)parts + 0x1B8) != 4 &&
                (s8)**(u8**)((s32)parts + 0x1B8) != 2) {
                scissor_damage_sub(unit, &damage);
                *(s32*)((s32)work + offset + 0x18) = i;
                *(s32*)((s32)work + offset + 0x58) = damage;
                total++;
                offset += 4;
                if (damage < 1) {
                    allHit = 0;
                }
            }
        }
    }

    if (total == 0) {
        allHit = 0;
    }

    offset = 0;
    for (i = 0; i < total; i++) {
        s32 id = *(s32*)((s32)work + offset + 0x18);
        if (id != -1 && *(s32*)((s32)work + offset + 0x58) >= 1) {
            unit = BattleGetUnitPtr(battleWork, id);
            parts = BtlUnit_GetPartsPtr(unit, BtlUnit_GetBodyPartsId(unit));
            BattleCheckDamage(mario, unit, parts, &weapon_scissor, 0);
            didDamage = 1;
        }
        offset += 4;
    }

    if (allHit == 1) {
        s32 value = *(s32*)((s32)work + 0xC) + 1;
        *(s32*)((s32)work + 0xC) = value;
        value++;
        if (value >= 7) {
            value = 6;
        }
        evtSetValue(event, args[0], value);
    } else {
        evtSetValue(event, args[0], -1);
    }

    if (didDamage == 1) {
        if (audience >= 1 && audience <= 0x31) {
            BattleAudienceSoundCallKind(1);
        }
        if (audience >= 0x32 && audience <= 0x63) {
            BattleAudienceSoundCallKind(1);
            BattleAudienceSoundHandBeat();
        }
        if (audience >= 0x64 && audience <= 0x95) {
            BattleAudienceSoundCallKind(2);
        }
        if (audience >= 0x96) {
            BattleAudienceSoundCallKind(2);
            BattleAudienceSoundHandBeat();
        }
        *(u8*)((s32)work + 0x98) += 0x14;
        if (*(u8*)((s32)work + 0x98) >= 0x64) {
            *(u8*)((s32)work + 0x98) = 0x64;
        }
        BattleAudienceSoundSetVol(4, *(u8*)((s32)work + 0x98), 0x1E);
    }

    return 2;
}

/* stub-fill: star_disp2D_sub | prototype_only | source_prototype */
void star_disp2D_sub(s32 id, s32 alpha) {
    extern void* GetScissorPtr(void);
    extern f32 intplGetValue(s32 type, s32 current, s32 total, f32 start, f32 end);
    extern void PSMTXTrans(f32 mtx[3][4], f32 x, f32 y, f32 z);
    extern void PSMTXScale(f32 mtx[3][4], f32 x, f32 y, f32 z);
    extern void PSMTXRotRad(f32 mtx[3][4], s32 axis, f32 rad);
    extern void PSMTXConcat(f32 a[3][4], f32 b[3][4], f32 out[3][4]);
    extern void iconDispGxCol(f32 mtx[3][4], s32 size, s32 iconId, void* color);
    extern u32 dat_80427c78;
    extern f32 float_0_80427c94;
    extern f32 float_20_80427cf4;
    extern f32 float_deg2rad_80427cf8;

    void* work;
    f32* hist;
    f32 trans[3][4];
    f32 scale[3][4];
    f32 rotX[3][4];
    f32 rotY[3][4];
    f32 rotZ[3][4];
    f32 mtx[3][4];
    u32 color;
    f32 x;
    f32 y;

    work = GetScissorPtr();
    hist = (f32*)((s32)work + 0x9C);
    if (*(s32*)((s32)work + 0x11C) >= 6) {
        *(s32*)((s32)work + 0x120) = *(s32*)((s32)work + 0x120) + 1;
        if (*(s32*)((s32)work + 0x120) > 0x1E) {
            *(s32*)((s32)work + 0x120) = 0x1E;
        }
        alpha = (s32)intplGetValue(0, *(s32*)((s32)work + 0x120), 0x1E,
                                   float_0_80427c94, (f32)(u8)alpha);
    }

    color = dat_80427c78;
    ((u8*)&color)[3] = alpha;

    if (id == 0) {
        x = hist[6];
        y = hist[7];
    } else {
        x = *(f32*)((s32)hist + (id * 4) + 0x3C);
        y = *(f32*)((s32)hist + (id * 4) + 0x4C);
    }

    PSMTXTrans(trans, x, y - float_20_80427cf4, float_0_80427c94);
    PSMTXScale(scale, *(f32*)((s32)work + 0x154), *(f32*)((s32)work + 0x158), *(f32*)((s32)work + 0x15C));
    PSMTXRotRad(rotX, 0x78, float_deg2rad_80427cf8 * *(f32*)((s32)work + 0x160));
    PSMTXRotRad(rotY, 0x79, float_deg2rad_80427cf8 * *(f32*)((s32)work + 0x164));
    PSMTXRotRad(rotZ, 0x7A, float_deg2rad_80427cf8 * *(f32*)((s32)work + 0x168));
    PSMTXConcat(rotZ, rotY, mtx);
    PSMTXConcat(rotX, mtx, mtx);
    PSMTXConcat(scale, mtx, mtx);
    PSMTXConcat(trans, mtx, mtx);
    iconDispGxCol(mtx, 0x10, 0x1A4, &color);
}

/* stub-fill: tess | prototype_only | source_prototype */
void tess(void* entries, s32 count) {
    extern void* GetScissorPtr(void);
    extern void* BattleAlloc(u32 size);
    extern void BattleFree(void* ptr);
    extern s32 scissor_cross(f32* pos1, f32* pos2, f32* poly, s32 count);

    void* work;
    f32* src;
    f32* dst;
    s32 i;
    s32 j;

    work = GetScissorPtr();
    src = (f32*)entries;
    if (count == 3) {
        dst = (f32*)((s32)*(void**)((s32)work + 0x118) + (*(s32*)((s32)work + 0x14) * 0x18));
        for (i = 0; i < 3; i++) {
            dst[i] = src[i * 2];
            dst[i + 3] = src[(i * 2) + 1];
        }
        *(s32*)((s32)work + 0x14) = *(s32*)((s32)work + 0x14) + 1;
        BattleFree(entries);
        return;
    }

    for (i = 0; i < count; i++) {
        f32* pos1 = &src[i * 2];
        for (j = 2; j < count - 1; j++) {
            s32 end = (i + j) % count;
            f32* pos2 = &src[end * 2];
            if (scissor_cross(pos1, pos2, src, count) == 1) {
                s32 count1 = j + 1;
                s32 count2 = (count - j) + 1;
                f32* poly1 = BattleAlloc(count1 * 8);
                f32* poly2 = BattleAlloc(count2 * 8);
                s32 k;

                for (k = 0; k < count1; k++) {
                    s32 idx = (i + k) % count;
                    poly1[k * 2] = src[idx * 2];
                    poly1[k * 2 + 1] = src[idx * 2 + 1];
                }

                poly2[0] = pos1[0];
                poly2[1] = pos1[1];
                for (k = 0; k < count2 - 1; k++) {
                    s32 idx = (end + k) % count;
                    poly2[(k + 1) * 2] = src[idx * 2];
                    poly2[(k + 1) * 2 + 1] = src[idx * 2 + 1];
                }

                BattleFree(entries);
                tess(poly1, count1);
                tess(poly2, count2);
                return;
            }
        }
    }

    dst = (f32*)((s32)*(void**)((s32)work + 0x118) + (*(s32*)((s32)work + 0x14) * 0x18));
    for (i = 0; i < 3; i++) {
        dst[i] = src[i * 2];
        dst[i + 3] = src[(i * 2) + 1];
    }
    *(s32*)((s32)work + 0x14) = *(s32*)((s32)work + 0x14) + 1;
    BattleFree(entries);
}

/* stub-fill: scissor_intersection | prototype_only | source_prototype */
s32 scissor_intersection(f32 x1, f32 y1, f32 x2, f32 y2,
                         f32 x3, f32 y3, f32 x4, f32 y4) {
    f32 outX, outY;
    f32 minX1 = x1 < x2 ? x1 : x2;
    f32 maxX1 = x1 < x2 ? x2 : x1;
    f32 minY1 = y1 < y2 ? y1 : y2;
    f32 maxY1 = y1 < y2 ? y2 : y1;
    f32 minX2 = x3 < x4 ? x3 : x4;
    f32 maxX2 = x3 < x4 ? x4 : x3;
    f32 minY2 = y3 < y4 ? y3 : y4;
    f32 maxY2 = y3 < y4 ? y4 : y3;
    f32 dx1 = x2 - x1;
    f32 dy1 = y2 - y1;
    f32 dx2 = x4 - x3;
    f32 dy2 = y4 - y3;
    s32 kind1 = dx1 == 0.0f;
    s32 kind2 = dx2 == 0.0f;

    if (dy1 == 0.0f) kind1 = 2;
    if (dy2 == 0.0f) kind2 = 2;

    if (kind1 == 0 && kind2 == 0) {
        f32 denominator = dx1 * dy2 - dy1 * dx2;
        f32 t;
        if (denominator == 0.0f) denominator = 0.000001f;
        t = ((x3 - x1) * dy2 - (y3 - y1) * dx2) / denominator;
        outX = x1 + dx1 * t;
        outY = y1 + dy1 * t;
        return outX >= minX1 && outX <= maxX1 && outY >= minY1 && outY <= maxY1 &&
               outX >= minX2 && outX <= maxX2 && outY >= minY2 && outY <= maxY2;
    }
    if (kind1 == 1 && kind2 == 0) {
        outX = x1;
        outY = y3 + dy2 / dx2 * (outX - x3);
        return outX >= minX2 && outX <= maxX2 && outY >= minY1 && outY <= maxY1;
    }
    if (kind1 == 0 && kind2 == 1) {
        outX = x3;
        outY = y1 + dy1 / dx1 * (outX - x1);
        return outX >= minX1 && outX <= maxX1 && outY >= minY2 && outY <= maxY2;
    }
    if (kind1 == 2 && kind2 == 0) {
        outY = y1;
        outX = x3 + dx2 / dy2 * (outY - y3);
        return outY >= minY2 && outY <= maxY2 && outX >= minX1 && outX <= maxX1;
    }
    if (kind1 == 0 && kind2 == 2) {
        outY = y3;
        outX = x1 + dx1 / dy1 * (outY - y1);
        return outY >= minY1 && outY <= maxY1 && outX >= minX2 && outX <= maxX2;
    }
    if (kind1 == 1 && kind2 == 1)
        return x1 == x3 && ((y1 >= minY2 && y1 <= maxY2) || (y2 >= minY2 && y2 <= maxY2));
    if (kind1 == 2 && kind2 == 2)
        return y1 == y3 && ((x1 >= minX2 && x1 <= maxX2) || (x2 >= minX2 && x2 <= maxX2));
    if (kind1 == 1 && kind2 == 2)
        return x1 >= minX2 && x1 <= maxX2 && y3 >= minY1 && y3 <= maxY1;
    if (kind1 == 2 && kind2 == 1)
        return x3 >= minX1 && x3 <= maxX1 && y1 >= minY2 && y1 <= maxY2;
    return 0;
}

