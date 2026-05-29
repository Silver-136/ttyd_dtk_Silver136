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
    *(s32*)((s32)GetScissorPtr() + 0x11C) = 1;
    return 2;
}

USER_FUNC(wait_star_standby) {
    s32 state = *(s32*)((s32)GetScissorPtr() + 0x11C);
    return (state > 5) ? 2 : 0;
}

USER_FUNC(scissor_offscreen) {
    GetScissorPtr();
    dispEntry(1, 1, scissor_disp_region, float_0_80427c94, 0);
    return 2;
}

USER_FUNC(scissor_init) {
    void* work = GetScissorPtr();
    memset(work, 0, 0x174);
    *(s32*)((s32)work + 0x104) = 0x3BF;
    *(void**)((s32)work + 0x114) = BattleAlloc(0x1000);
    *(void**)((s32)work + 0x118) = BattleAlloc(0x3000);
    *(f32*)((s32)work + 0xB4) = float_0_80427c94;
    *(f32*)((s32)work + 0xB8) = float_0_80427c94;
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
    return 2;
}
