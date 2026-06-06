#include "driver/imgdrv.h"

extern void* wp;

void* __memAlloc(s32 heap, u32 size);
void* memset(void* dst, int value, u32 size);

void imgSetShadow(void* img, s32 shadow) {
    *(s32*)((s32)img + 0x100) = shadow;
}

void imgClearVirtualPoint(void* img) {
    *(u32*)((s32)img + 0xCC) |= 8;
}

void imgSetVirtualPoint(void* img, void* point) {
    u32 flags;
    u32 y;
    u32 x;
    u32 z;

    flags = *(u32*)((s32)img + 0xCC);
    x = *(u32*)((s32)point + 0);
    flags &= ~8;
    y = *(u32*)((s32)point + 4);
    *(u32*)((s32)img + 0xCC) = flags;
    z = *(u32*)((s32)point + 8);
    *(u32*)((s32)img + 0xF0) = x;
    *(u32*)((s32)img + 0xF4) = y;
    *(u32*)((s32)img + 0xF8) = z;
}

void imgInit(void) {
    *(s32*)wp = 10;
    *(void**)((s32)wp + 4) = __memAlloc(0, *(s32*)wp * 0x158);
    memset(*(void**)((s32)wp + 4), 0, *(s32*)wp * 0x158);
}


u8 imgDisp(void) {
    return 0;
}


u8 imgShadowDisp(void) {
    return 0;
}


u8 imgCapture_Prim(int param_1, int param_2) {
    return 0;
}


void zFill(void) {
}


u8 imgDisp_ProjPlane(s32 param_1, int param_2) {
    return 0;
}


u8 imgMain(void) {
    return 0;
}


void imgEntry(s32 name, s32 flag) {
}


void imgAutoRelease(s32 value) {
    extern void* wp;
    extern void* memset(void* dst, s32 value, u32 size);
    extern void smartFree(void* ptr);
    s32 count;
    void* entry;
    s32 i;

    i = 0;
    count = *(s32*)wp;
    entry = *(void**)((s32)wp + 4);
    while (i < count) {
        if ((*(u32*)((s32)entry + 0xCC) & 1) != 0 && *(s32*)((s32)entry + 0xD0) == value) {
            void* part = entry;
            s32 j = 0;
            while (j < 3) {
                u32 flags = *(u32*)part;
                if ((flags & 4) != 0) {
                    *(u32*)part = flags & ~0x10;
                    if ((*(u32*)part & 0x100) != 0) {
                        *(u32*)part = 0;
                    } else if (*(s32*)((s32)part + 0x40) == 0) {
                        *(u32*)part = 0;
                    } else {
                        *(u32*)part = 0;
                        smartFree(*(void**)((s32)part + 0x3C));
                    }
                }
                j++;
                part = (void*)((s32)part + 0x44);
            }
            *(u32*)((s32)entry + 0xCC) = 0;
            memset(entry, 0, 0x158);
        }
        i++;
        entry = (void*)((s32)entry + 0x158);
    }
}

void imgRelease(void* image) {
    extern void* memset(void* dst, s32 value, u32 size);
    extern void smartFree(void* ptr);
    void* part = image;
    s32 i = 0;

    while (i < 3) {
        u32 flags = *(u32*)part;
        if ((flags & 4) != 0) {
            *(u32*)part = flags & ~0x10;
            if ((*(u32*)part & 0x100) != 0) {
                *(u32*)part = 0;
            } else if (*(s32*)((s32)part + 0x40) == 0) {
                *(u32*)part = 0;
            } else {
                *(u32*)part = 0;
                smartFree(*(void**)((s32)part + 0x3C));
            }
        }
        i++;
        part = (void*)((s32)part + 0x44);
    }
    *(u32*)((s32)image + 0xCC) = 0;
    memset(image, 0, 0x158);
}

s32 imgCapture(s32 layer) {
    extern void* wp;
    extern void* gp;
    extern void imgCapture_Prim(void* img, s32 index);
    s32 flag = *(s32*)((s32)gp + 0x14);
    s32 localFlag = ((u32)(-flag) | (u32)flag) >> 31;
    s32 count = *(s32*)wp;
    void* entry = *(void**)((s32)wp + 4);
    s32 i = 0;

    while (i < count) {
        if ((*(u32*)((s32)entry + 0xCC) & 1) != 0 &&
            *(s32*)((s32)entry + 0x104) != -1) {
            s32 entryFlag = *(s32*)((s32)entry + 0xD0);
            if ((entryFlag == 2 || entryFlag == localFlag) &&
                *(s32*)((s32)entry + 0x154) == layer) {
                s32 j = 0;
                while (j < 3) {
                    imgCapture_Prim(entry, j);
                    j++;
                }
            }
        }
        i++;
        entry = (void*)((s32)entry + 0x158);
    }
    return 0;
}

void* imgNameToPtr(s32 name, s32 flag) {
    extern void* wp;
    extern s32 strcmp(const char* a, const char* b);
    s32 count;
    void* entry;
    s32 i;

    i = 0;
    count = *(s32*)wp;
    entry = *(void**)((s32)wp + 4);
    while (i < count) {
        if ((*(u32*)((s32)entry + 0xCC) & 1) != 0) {
            s32 entryFlag = *(s32*)((s32)entry + 0xD0);
            if (entryFlag == 2 || entryFlag == flag) {
                if (strcmp((char*)((s32)entry + 0xD4), (char*)name) == 0) {
                    break;
                }
            }
        }
        i++;
        entry = (void*)((s32)entry + 0x158);
    }
    return entry;
}

u8 imgFreeCapture(int param_1, int param_2) {
    return 0;
}
