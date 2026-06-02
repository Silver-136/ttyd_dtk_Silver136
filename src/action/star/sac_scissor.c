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

