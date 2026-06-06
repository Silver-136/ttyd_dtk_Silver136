#include "event/evt_env.h"

extern s32 evtGetValue(void* event, s32 value);
extern void envBlurOn(s32 type, s32 time);
extern void envBlurOff(void);

s32 evt_env_blur_on(void* event, s32 isFirstCall) {
    s32* args;
    s32 type;
    s32 time;
    u32 nowHi;
    u32 nowLo;
    s32 mask;

    args = *(s32**)((s32)event + 0x18);
    type = evtGetValue(event, args[0]);
    time = evtGetValue(event, args[1]);
    nowHi = *(u32*)((s32)event + 0);
    nowLo = *(u32*)((s32)event + 4);
    if (isFirstCall != 0) {
        envBlurOn(type, time);
        mask = -1;
        *(u32*)((s32)event + 0x78) = nowHi & mask;
        *(u32*)((s32)event + 0x7C) = nowLo & mask;
    }
    if (time != 0) {
        u64 now = ((u64)nowHi << 32) | nowLo;
        u64 start = ((u64)*(u32*)((s32)event + 0x78) << 32) | *(u32*)((s32)event + 0x7C);
        u32 ticks = (*(u32*)0x800000F8) >> 2;
        u32 ticksPerMs = (u32)(((u64)0x10624DD3 * ticks) >> 32) >> 6;

        if (((now - start) / ticksPerMs) >= time) {
            envBlurOff();
            return 2;
        }
    }
    return 0;
}
