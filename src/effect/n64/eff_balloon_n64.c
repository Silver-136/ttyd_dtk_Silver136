#include "effect/n64/eff_balloon_n64.h"

typedef struct EffBalloonWork {
    s32 type;
    f32 x;
    f32 y;
    f32 z;
    f32 scale;
    f32 unk14;
    f32 unk18;
    s32 timer;
    s32 frame;
} EffBalloonWork;

void* effEntry(void);
void* __memAlloc(s32 heap, s32 size);

extern char str_BalloonN64_802fab68[];
extern f32 float_0_80424cbc;

u8 effBalloonDisp(void) {
    return 0;
}


u8 effBalloonMain(void) {
    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effBalloonN64Entry(s32 type, f32 x, f32 y, f32 z, f32 unk14, f32 scale) {
    void* entry = effEntry();
    EffBalloonWork* work;
    f32 zero;

    *(char**)((s32)entry + 0x14) = str_BalloonN64_802fab68;
    *(s32*)((s32)entry + 0x8) = 1;
    work = __memAlloc(3, 0xCC);
    *(EffBalloonWork**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effBalloonMain;
    zero = float_0_80424cbc;
    *(u32*)entry |= 2;

    work->type = type;
    work->x = x;
    work->y = y;
    work->z = z;
    work->scale = scale;
    work->timer = 100;
    work->unk14 = unk14;
    work->unk18 = zero;
    work->frame = 0;
    return entry;
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off
