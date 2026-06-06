#include "sequence/seq_e3.h"

extern void* wp;
extern void* gp;
extern void str_b_mario_80302608(void);
extern char str_fOff_peron_803027c0[];
extern u32 dat_804286e0;
extern f32 float_270_80428784;

void* getMarioStDvdRoot(void);
void* fileAllocf(s32 size, const char* fmt, ...);
void sysWaitDrawSync(void);
void GXClearBoundingBox(void);
void GXReadBoundingBox(u16* left, u16* top, u16* right, u16* bottom);
void e3Disp(s32 camera, void* param);
s32 offscreenNameToId(char* name);
void offscreenAddBoundingBox(s32 id, u16 left, u16 top, u16 right, u16 bottom);
void OSCreateAlarm(void* alarm);
void OSSetAlarm(void* alarm, u64 tick, void* handler);
u64 OSGetTime(void);
void fadeEntry(s32 type, s32 duration, void* color);
void fadeReset(s32 type);
void psndStopAllFadeOut(void);
void psndBGMOn(s32 flags, void* name);
void animGroupBaseAsync(void* name, s32 mode, s32 flags);

void alarm_handler(void) {
    *(u32*)gp |= 0x2000;
}

void seq_e3Exit(void) {
    extern void fileFree(void* ptr);

    fileFree(*(void**)wp);
    psndStopAllFadeOut();
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void e3DispOffscreen(s32 camera, void* param) {
    u16 left;
    u16 top;
    u16 right;
    u16 bottom;

    *(s32*)((s32)wp + 4) = *(s32*)((s32)wp + 8);
    sysWaitDrawSync();
    GXClearBoundingBox();
    e3Disp(camera, param);
    sysWaitDrawSync();
    GXReadBoundingBox(&left, &top, &right, &bottom);
    offscreenAddBoundingBox(offscreenNameToId(str_fOff_peron_803027c0), left, top, right, bottom);
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void seq_e3Init(void* seq) {
    register char* rodata;
    register void* seqLocal;
    u32 color;
    u64 time;
    f32 angle;

    rodata = (char*)str_b_mario_80302608;
    seqLocal = seq;
    *(void**)wp = fileAllocf(4, rodata + 0x204, getMarioStDvdRoot());
    angle = float_270_80428784;
    *(s32*)((s32)wp + 4) = 0;
    *(s32*)((s32)wp + 8) = 0;
    *(s32*)((s32)wp + 0xC) = 0;
    *(f32*)((s32)wp + 0x64) = angle;
    OSCreateAlarm((void*)((s32)wp + 0x20));
    OSSetAlarm((void*)((s32)wp + 0x20), ((u64)(*(u32*)0x800000F8 >> 2) * 0x12C), alarm_handler);
    time = OSGetTime();
    *(u64*)((s32)wp + 0x48) = time;
    *(u32*)gp &= ~0x2;
    *(u32*)gp |= 0x20;
    *(u32*)gp |= 0x1000;
    *(u32*)gp &= ~0x2000;
    color = dat_804286e0;
    fadeEntry(9, 0xC8, &color);
    fadeReset(1);
    psndStopAllFadeOut();
    psndBGMOn(0x200, rodata + 0x214);
    *(s32*)((s32)seqLocal + 4) = 0;
    animGroupBaseAsync(rodata + 0x224, 0, 0);
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on
