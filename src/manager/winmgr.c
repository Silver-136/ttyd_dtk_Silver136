#include "manager/winmgr.h"

void* wp;
void* __memAlloc(s32 heap, u32 size);
void* memset(void* dst, int val, unsigned long size);
void __memFree(s32 heap, void* ptr);
extern u8 itemDataTable[];
char* msgSearch(char* msg);
void winMgrHelpDraw(void* win);

s32 unk_8023cf04(void* win) {
    return *(s32*)((s32)win + 0xC);
}

void winMgrHelpInit(void* win) {
    *(s32*)((s32)win + 0x38) = 0;
    *(s32*)((s32)win + 0x3C) = 0;
    *(s32*)((s32)win + 0x34) = 0;
}

void* winMgrGetPtr(s32 id) {
    return (void*)((s32)*(void**)((s32)wp + 4) + id * 0x44);
}

s32 winMgrAction(s32 id) {
    return (*(u32*)((s32)*(void**)((s32)wp + 4) + id * 0x44) >> 2) & 1;
}

void winMgrDelete(s32 id) {
    u32* flags = (u32*)((s32)*(void**)((s32)wp + 4) + id * 0x44);
    *flags &= ~1;
}

void winMgrSetPriority(s32 id, s32 priority) {
    void* win = (void*)((s32)*(void**)((s32)wp + 4) + id * 0x44);
    if ((*(u32*)win & 1) != 0) {
        *(s32*)((s32)win + 0x30) = priority;
    }
}

void winMgrSetParam(s32 id, s32 param) {
    void* win = (void*)((s32)*(void**)((s32)wp + 4) + id * 0x44);
    if ((*(u32*)win & 1) != 0) {
        *(s32*)((s32)win + 0x2C) = param;
    }
}

void winMgrSetSize(s32 id, s32 x, s32 y, s32 w, s32 h) {
    void* win = (void*)((s32)*(void**)((s32)wp + 4) + id * 0x44);
    if ((*(u32*)win & 1) != 0) {
        *(s32*)((s32)win + 0x18) = x;
        *(s32*)((s32)win + 0x1C) = y;
        *(s32*)((s32)win + 0x20) = w;
        *(s32*)((s32)win + 0x24) = h;
    }
}

void winMgrClose(s32 id) {
    void* win = (void*)((s32)*(void**)((s32)wp + 4) + id * 0x44);
    if ((*(u32*)win & 1) != 0) {
        *(s32*)((s32)win + 4) = 2;
        *(s32*)((s32)win + 8) = 0;
    }
}

void winMgrCloseAutoDelete(s32 id) {
    void* win = (void*)((s32)*(void**)((s32)wp + 4) + id * 0x44);
    if ((*(u32*)win & 1) != 0) {
        *(s32*)((s32)win + 4) = 2;
        *(s32*)((s32)win + 8) = 0;
        *(u32*)win |= 8;
    }
}

void winMgrOpen(s32 id) {
    void* win = (void*)((s32)*(void**)((s32)wp + 4) + id * 0x44);
    if ((*(u32*)win & 1) != 0) {
        *(s32*)((s32)win + 4) = 1;
        *(s32*)((s32)win + 8) = 0;
        *(s32*)((s32)win + 0x38) = 0;
        *(s32*)((s32)win + 0x3C) = 0;
        *(s32*)((s32)win + 0x34) = 0;
    }
}

void winMgrInit(void) {
    *(s32*)wp = 0x20;
    *(void**)((s32)wp + 4) = __memAlloc(0, *(s32*)wp * 0x44);
    memset(*(void**)((s32)wp + 4), 0, *(s32*)wp * 0x44);
}

void winMgrReInit(void) {
    memset(*(void**)((s32)wp + 4), 0, *(s32*)wp * 0x44);
}

void winMgrSelectDelete(void* select) {
    __memFree(0, *(void**)((s32)select + 0x30));
    __memFree(0, select);
}

s32 unk_8023d5e4(void* a, void* b) {
    u16 ai = *(u16*)((s32)a + 2);
    u16 bi = *(u16*)((s32)b + 2);
    u8* values = itemDataTable + 0x18;
    u16 av = *(u16*)(values + ai * 0x28);
    u16 bv = *(u16*)(values + bi * 0x28);
    if (av > bv) {
        return 1;
    }
    return av < bv ? -1 : 0;
}

void select_disp3(void* win) {
    void* data = *(void**)((s32)win + 0x2C);
    s32 id = *(s32*)((s32)data + 0x2C);
    void* mgr = *(void**)((s32)wp + 4);
    if ((*(u32*)((s32)mgr + id * 0x44) & 4) == 0) {
        s32 index = *(s32*)((s32)data + 0xC);
        void* list = *(void**)((s32)data + 0x30);
        u16 item = *(u16*)((s32)list + index * 4 + 2);
        *(char**)((s32)win + 0x34) = msgSearch(*(char**)(itemDataTable + item * 0x28 + 8));
        winMgrHelpDraw(win);
    }
}

s32 winMgrSelect(void* select) {
    u16 flags = *(u16*)select;

    if (flags & 0x1000) {
        if (flags & 0x2000) {
            return -1;
        }
        if (*(s32*)((s32)select + 0x38) != 0 && *(s32*)((s32)select + 0xC) == 0 && (flags & 0x100)) {
            return -1;
        }
        {
            s32 offset = *(s32*)((s32)select + 0xC) << 2;
            void* list = *(void**)((s32)select + 0x30);

            list = (void*)((s32)list + offset);
            return *(u16*)((s32)list + 2);
        }
    }
    return 0;
}
