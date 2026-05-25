#include "driver/msgdrv.h"

void* msgWork;
extern void* gp;

void* windowGetPointer(s32 id);
void smartFree(void* ptr);
void* animGetPtr(void* anim);
void GXSetZMode(s32 enable, s32 func, s32 update);
void GXSetBlendMode(s32 type, s32 srcFactor, s32 dstFactor, s32 op);
void GXSetZCompLoc(s32 beforeTex);
void GXSetAlphaCompare(s32 comp0, s32 ref0, s32 op, s32 comp1, s32 ref1);

void* L_msgGetWork(void) {
    return &msgWork;
}

s32 _ismbblead(s32 c) {
    u32 value;

    if (*(void**)((s32)gp + 0x16C) != 0) {
        return 0;
    }

    value = (u8)c;
    if ((value >= 0x81 && value <= 0x9F) || (value >= 0xE0 && value <= 0xFC)) {
        return 1;
    }
    return 0;
}

void msgWindow_Continue(s32 id) {
    void* win = windowGetPointer(id);
    void* obj;
    void* work;
    s32 x;
    s32 y;

    *(u16*)win = 1;
    *(u16*)((s32)win + 2) |= 2;
    work = gp;
    obj = *(void**)((s32)win + 0x28);
    x = *(s32*)((s32)work + 0x38);
    y = *(s32*)((s32)work + 0x3C);
    obj = *(void**)obj;
    *(s32*)((s32)obj + 0x34) = y;
    *(s32*)((s32)obj + 0x30) = x;
}

void msgWindow_Delete(void* win) {
    void* ptr = *(void**)((s32)win + 0x28);

    if (ptr != 0) {
        s32 count = *(s32*)(*(s32*)ptr + 0x38);

        while (count > 0) {
            count--;
        }
        smartFree(ptr);
        *(void**)((s32)win + 0x28) = 0;
    }
}

void msgDispKeyWait_render(void* anim) {
    void* ptr = animGetPtr(anim);

    GXSetZMode(1, 7, 0);
    GXSetBlendMode(0, 4, 5, 0);
    GXSetZCompLoc(0);
    GXSetAlphaCompare(6, 0x80, 1, 0, 0);
    *(void**)((s32)ptr + 0xE0) = anim;
}
