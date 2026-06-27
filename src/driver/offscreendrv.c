#include "driver/offscreendrv.h"

extern void* gp;
extern s32 work;
extern void* __memAlloc();

void offscreenAddBoundingBox(s32, u16, u16, u16, u16) {
    ;
}


u8 offscreenDisp(void) {
    return 0;
}


int offscreenEntry(char* param_1) {
    return 0;
}


u8 offscreenMain(void) {
    return 0;
}


void offscreenDelete(char* name) {
    extern void* gp;
    extern s32 work;
    extern s32 strcmp(const char* a, const char* b);
    void* set = &work;
    s32 i;
    s32 count;
    void* entry;

    if (*(s32*)((s32)gp + 0x14) != 0) {
        set = (void*)((s32)set + 8);
    }
    count = *(s32*)set;
    i = 0;
    entry = *(void**)((s32)set + 4);
    while (i < count) {
        if ((*(u32*)entry & 1) != 0) {
            if (strcmp((char*)((s32)entry + 4), name) == 0) {
                break;
            }
        }
        i++;
        entry = (void*)((s32)entry + 0x50);
    }
    *(u32*)entry = 0;
}

void* offscreenNameToPtr(char* name) {
    extern void* gp;
    extern s32 work;
    extern s32 strcmp(const char* a, const char* b);
    void* set = &work;
    s32 i;
    s32 count;
    void* entry;

    if (*(s32*)((s32)gp + 0x14) != 0) {
        set = (void*)((s32)set + 8);
    }
    count = *(s32*)set;
    i = 0;
    entry = *(void**)((s32)set + 4);
    while (i < count) {
        if ((*(u32*)entry & 1) != 0) {
            if (strcmp((char*)((s32)entry + 4), name) == 0) {
                break;
            }
        }
        i++;
        entry = (void*)((s32)entry + 0x50);
    }
    return entry;
}

s32 offscreenNameToId(char* name) {
    extern void* gp;
    extern s32 work;
    extern s32 strcmp(const char* a, const char* b);
    void* set = &work;
    s32 i;
    s32 count;
    void* entry;

    if (*(s32*)((s32)gp + 0x14) != 0) {
        set = (void*)((s32)set + 8);
    }
    count = *(s32*)set;
    i = 0;
    entry = *(void**)((s32)set + 4);
    while (i < count) {
        if ((*(u32*)entry & 1) != 0) {
            if (strcmp((char*)((s32)entry + 4), name) == 0) {
                break;
            }
        }
        i++;
        entry = (void*)((s32)entry + 0x50);
    }
    return i;
}

void offscreenGetTexObj(s32 id, void** texObj, u32* size) {
    extern void* gp;
    extern s32 work;
    void* set = &work;
    u32 flags;
    void* entry;

    if (*(s32*)((s32)gp + 0x14) != 0) {
        set = (void*)((s32)set + 8);
    }
    entry = (void*)((s32)*(void**)((s32)set + 4) + id * 0x50);
    flags = *(u32*)entry;
    if ((flags & 1) == 0) {
        *texObj = 0;
        *size = 0;
        return;
    }
    if ((flags & 2) == 0) {
        *texObj = 0;
        *size = 0;
        return;
    }
    if ((flags & 4) == 0) {
        *texObj = 0;
        *size = 0;
        return;
    }
    *texObj = (void*)((s32)entry + 0x1C);
    *size = *(u32*)((s32)entry + 0x44);
}

void offscreenInit(void) {
    void* base = &work;

    *(s32*)base = 10;
    *(void**)((s32)base + 4) = __memAlloc(0, *(s32*)base * 0x50);
    memset(*(void**)((s32)base + 4), 0, *(s32*)base * 0x50);
    *(s32*)((s32)base + 8) = 10;
    *(void**)((s32)base + 0xC) = __memAlloc(0, *(s32*)((s32)base + 8) * 0x50);
    memset(*(void**)((s32)base + 0xC), 0, *(s32*)((s32)base + 8) * 0x50);
}


s32 offscreenGetBoundingBox(int param_1, void* param_2, void* param_3, void* param_4, void* param_5) {
    void* wp = &work;
    void* entry;

    if (*(s32*)((s32)gp + 0x14) != 0) {
        wp = (void*)((s32)wp + 8);
    }
    entry = (void*)((s32)*(void**)((s32)wp + 4) + param_1 * 0x50);
    if ((*(s32*)entry & 1) == 0) {
        return 0;
    }
    if ((*(s32*)entry & 2) == 0) {
        return 0;
    }
    *(u16*)param_2 = *(u16*)((s32)entry + 0x14);
    *(u16*)param_3 = *(u16*)((s32)entry + 0x16);
    *(u16*)param_4 = *(u16*)((s32)entry + 0x18);
    *(u16*)param_5 = *(u16*)((s32)entry + 0x1A);
    return 1;
}


void offscreenReset(s32 value) {
    void* wp = &work;

    if (value != 0) {
        wp = (void*)((s32)wp + 8);
    }
    memset(*(void**)((s32)wp + 4), 0, *(s32*)wp * 0x50);
}


s32 offscreenGetBoundingBox_BeforeFrame(int param_1, void* param_2, void* param_3, void* param_4, void* param_5) {
    void* wp = &work;
    void* entry;
    u16 x0;

    if (*(s32*)((s32)gp + 0x14) != 0) {
        wp = (void*)((s32)wp + 8);
    }
    entry = (void*)((s32)*(void**)((s32)wp + 4) + param_1 * 0x50);
    if ((*(s32*)entry & 1) == 0) {
        return 0;
    }
    x0 = *(u16*)((s32)entry + 0x48);
    if (x0 > *(u16*)((s32)entry + 0x4C)) {
        return 0;
    }
    *(u16*)param_2 = x0;
    *(u16*)param_3 = *(u16*)((s32)entry + 0x4A);
    *(u16*)param_4 = *(u16*)((s32)entry + 0x4C);
    *(u16*)param_5 = *(u16*)((s32)entry + 0x4E);
    return 1;
}
