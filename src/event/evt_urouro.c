#include "event/evt_urouro.h"

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 urouro_init_func(void* event, s32 isFirstCall) {
    extern s32 evtGetValue(void* event, s32 value);
    extern f32 evtGetFloat(void* event, s32 value);
    extern void* evtNpcNameToPtr(void* event, s32 name);
    extern void* mapalloc_base_ptr;
    extern void* _mapAlloc(void* heap, u32 size);
    extern s32 sysMsec2Frame(s32 msec);

    s32* args;
    void* npc;
    s32 value;
    s32 name;
    f32 x;
    f32 y;
    f32 z;
    f32 scale;

    args = *(s32**)((s32)event + 0x18);
    name = evtGetValue(event, args[0]);
    npc = evtNpcNameToPtr(event, name);
    x = evtGetFloat(event, args[1]);
    y = evtGetFloat(event, args[2]);
    z = evtGetFloat(event, args[3]);
    scale = evtGetFloat(event, args[4]);
    value = evtGetValue(event, args[5]);
    *(void**)((s32)event + 0xC4) = _mapAlloc(mapalloc_base_ptr, 0x2C);
    args = *(s32**)((s32)event + 0xC4);
    args[0] = value;
    args[1] = 0;
    args[2] = 0;
    args[3] = 0;
    args[4] = sysMsec2Frame(0x1770);
    args[5] = sysMsec2Frame(0x2328);
    args[6] = sysMsec2Frame(0x5DC);
    *(f32*)&args[7] = x;
    *(f32*)&args[8] = *(f32*)((s32)npc + 0x90);
    *(f32*)&args[9] = y;
    *(f32*)&args[10] = z;
    *(f32*)((s32)npc + 0x1B0) = scale;
    *(u32*)npc |= 0x400000;
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on
