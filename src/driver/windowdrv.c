#include "driver/windowdrv.h"

void* winTpl;
void* wakuTexObj;
void* WinObjects;
s32 unk_8041ea1c;

void* __memAlloc(s32 heap, u32 size);

void* getWakuTexObj(s32 id) {
    return (void*)((s32)wakuTexObj + id * 0x20);
}

void* windowGetPointer(s32 id) {
    return (void*)((s32)WinObjects + id * 0x48);
}

s32 windowCheckID(s32 id) {
    return *(u16*)((s32)WinObjects + id * 0x48 + 2) & 2;
}

s32 windowDelete(void* win) {
    void (*cb)(void*) = *(void (**) (void*))((s32)win + 0x40);
    if (cb != 0) {
        cb(win);
    }
    *(u16*)((s32)win + 2) = 0;
    *(s32*)((s32)win + 0x28) = 0;
    return 1;
}

s32 windowDeleteID(s32 id) {
    void* win = (void*)((s32)WinObjects + id * 0x48);
    void (*cb)(void*);
    if ((*(u16*)((s32)win + 2) & 1) == 0) {
        return 0;
    }
    cb = *(void (**) (void*))((s32)win + 0x40);
    if (cb != 0) {
        cb(win);
    }
    *(u16*)((s32)win + 2) = 0;
    *(s32*)((s32)win + 0x28) = 0;
    return 1;
}

void windowReInit(void) {
    void* win = WinObjects;
    *(u16*)((s32)win + 0x2) = 0;
    *(s32*)((s32)win + 0x28) = 0;
    *(u16*)((s32)win + 0x4A) = 0;
    *(s32*)((s32)win + 0x70) = 0;
    *(u16*)((s32)win + 0x92) = 0;
    *(s32*)((s32)win + 0xB8) = 0;
    *(u16*)((s32)win + 0xDA) = 0;
    *(s32*)((s32)win + 0x100) = 0;
    *(u16*)((s32)win + 0x122) = 0;
    *(s32*)((s32)win + 0x148) = 0;
    *(u16*)((s32)win + 0x16A) = 0;
    *(s32*)((s32)win + 0x190) = 0;
    *(u16*)((s32)win + 0x1B2) = 0;
    *(s32*)((s32)win + 0x1D8) = 0;
}

void windowInit(void) {
    void* win = __memAlloc(0, 0x1F8);
    WinObjects = win;
    *(u16*)((s32)win + 0x2) = 0;
    *(s32*)((s32)win + 0x28) = 0;
    *(u16*)((s32)win + 0x4A) = 0;
    *(s32*)((s32)win + 0x70) = 0;
    *(u16*)((s32)win + 0x92) = 0;
    *(s32*)((s32)win + 0xB8) = 0;
    *(u16*)((s32)win + 0xDA) = 0;
    *(s32*)((s32)win + 0x100) = 0;
    *(u16*)((s32)win + 0x122) = 0;
    *(s32*)((s32)win + 0x148) = 0;
    *(u16*)((s32)win + 0x16A) = 0;
    *(s32*)((s32)win + 0x190) = 0;
    *(u16*)((s32)win + 0x1B2) = 0;
    *(s32*)((s32)win + 0x1D8) = 0;
    winTpl = 0;
    unk_8041ea1c = 0;
}
