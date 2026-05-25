#include "event/evt_img.h"

s32 evt_img_entry(void* event, s32 isFirstCall) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void imgEntry(s32 name, s32 flag);
    extern void* gp;
    s32* args = *(s32**)((s32)event + 0x18);
    s32 name = evtGetValue(event, args[0]);
    s32 flag = *(s32*)((s32)gp + 0x14);

    imgEntry(name, ((u32)(-flag) | (u32)flag) >> 31);
    return 2;
}

s32 evt_img_set_shadow(void* event, s32 isFirstCall) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void* imgNameToPtr(s32 name, s32 flag);
    extern void imgSetShadow(void* img, s32 shadow);
    extern void* gp;
    s32* args = *(s32**)((s32)event + 0x18);
    s32 name = evtGetValue(event, args[0]);
    s32 shadow = evtGetValue(event, args[1]);
    s32 flag = *(s32*)((s32)gp + 0x14);

    imgSetShadow(imgNameToPtr(name, ((u32)(-flag) | (u32)flag) >> 31), shadow);
    return 2;
}

s32 N_evt_img_set_z(void* event, s32 isFirstCall) {
    extern s32 evtGetValue(void* event, s32 value);
    extern f32 evtGetFloat(void* event, s32 value);
    extern void* imgNameToPtr(s32 name, s32 flag);
    extern void* gp;
    s32* args = *(s32**)((s32)event + 0x18);
    s32 name = evtGetValue(event, args[0]);
    f32 z = evtGetFloat(event, args[1]);
    s32 flag = *(s32*)((s32)gp + 0x14);
    void* img = imgNameToPtr(name, ((u32)(-flag) | (u32)flag) >> 31);

    *(f32*)((s32)img + 0xFC) = z;
    return 2;
}
