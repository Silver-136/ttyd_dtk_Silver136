#include "driver/offscreendrv.h"

extern void* gp;
extern s32 work;
extern void* __memAlloc();

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void offscreenAddBoundingBox(s32 id, u16 x0, u16 y0, u16 x1, u16 y1) {
    extern void* gp;
    extern s32 work;
    extern void* DEMOGetRenderModeObj(void);
    void* set;
    void* entry;
    u16 width;
    u16 height;

    if (x1 <= x0) {
        return;
    }

    set = &work;
    if (*(s32*)((s32)gp + 0x14) != 0) {
        set = (void*)((s32)set + 8);
    }
    entry = (void*)((s32)*(void**)((s32)set + 4) + id * 0x50);
    if ((*(u32*)entry & 1) == 0) {
        return;
    }

    if (x0 >= 2) {
        x0 -= 2;
    }
    if (y0 >= 2) {
        y0 -= 2;
    }
    width = *(u16*)((s32)DEMOGetRenderModeObj() + 4) - 2;
    if (x1 <= width) {
        x1 += 2;
    }
    height = *(u16*)((s32)DEMOGetRenderModeObj() + 6) - 2;
    if (y1 <= height) {
        y1 += 2;
    }

    x0 -= x0 & 1;
    y0 -= y0 & 1;
    x1 += x1 & 1;
    y1 += y1 & 1;

    if (x0 < *(u16*)((s32)entry + 0x14)) {
        *(u16*)((s32)entry + 0x14) = x0;
        *(u32*)entry |= 2;
    }
    if (y0 < *(u16*)((s32)entry + 0x16)) {
        *(u16*)((s32)entry + 0x16) = y0;
        *(u32*)entry |= 2;
    }
    if (x1 > *(u16*)((s32)entry + 0x18)) {
        *(u16*)((s32)entry + 0x18) = x1;
        *(u32*)entry |= 2;
    }
    if (y1 > *(u16*)((s32)entry + 0x1A)) {
        *(u16*)((s32)entry + 0x1A) = y1;
        *(u32*)entry |= 2;
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void offscreenDisp(void) {
    extern void* gp;
    extern s32 work;
    extern void GXSetTexCopySrc(u16 x, u16 y, u16 wd, u16 ht);
    extern void GXSetTexCopyDst(u16 wd, u16 ht, s32 fmt, s32 mipmap);
    extern u32 GXGetTexBufferSize(u16 wd, u16 ht, s32 fmt, s32 mipmap, s32 max_lod);
    extern void* smartAlloc(u32 size, s32 heap);
    extern void GXSetZMode(s32 enable, s32 func, s32 update_enable);
    extern void GXCopyTex(void* dest, s32 clear);
    extern void GXPixModeSync(void);
    extern void GXInitTexObj(void* obj, void* image_ptr, u16 wd, u16 ht, s32 fmt, s32 wrap_s, s32 wrap_t, s32 mipmap);
    void* set = &work;
    s32 count;
    s32 i;
    void* entry;

    if (*(s32*)((s32)gp + 0x14) != 0) {
        set = (void*)((s32)set + 8);
    }

    count = *(s32*)set;
    i = 0;
    entry = *(void**)((s32)set + 4);
    while (i < count) {
        u32 flags = *(u32*)entry;
        if ((flags & 1) != 0 && (flags & 2) != 0) {
            u16 x0 = *(u16*)((s32)entry + 0x14);
            u16 x1 = *(u16*)((s32)entry + 0x18);
            u16 y0 = *(u16*)((s32)entry + 0x16);
            u16 y1 = *(u16*)((s32)entry + 0x1A);
            u16 w = x1 - x0;
            u16 h = y1 - y0;
            GXSetTexCopySrc(x0, y0, w, h);
            GXSetTexCopyDst(w, h, *(s32*)((s32)entry + 0x3C), 0);
            *(void**)((s32)entry + 0x44) = smartAlloc(GXGetTexBufferSize(w, h, *(s32*)((s32)entry + 0x3C), 0, 0), 3);
            GXSetZMode(1, 3, 1);
            GXCopyTex(*(void**)*(void**)((s32)entry + 0x44), 1);
            GXPixModeSync();
            GXInitTexObj((void*)((s32)entry + 0x1C), *(void**)*(void**)((s32)entry + 0x44), w, h, *(s32*)((s32)entry + 0x40), 0, 0, 0);
            *(u32*)entry |= 4;
        }
        i++;
        entry = (void*)((s32)entry + 0x50);
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 offscreenEntry(char* name) {
    extern void* gp;
    extern s32 work;
    extern s32 strcmp(const char* a, const char* b);
    extern char* strcpy(char* dst, const char* src);
    void* set = &work;
    s32 count;
    s32 i;
    void* entry;
    s32 ret;

    if (*(s32*)((s32)gp + 0x14) != 0) {
        set = (void*)((s32)set + 8);
    }

    count = *(s32*)set;
    i = 0;
    entry = *(void**)((s32)set + 4);
    while (i < count) {
        if ((*(u32*)entry & 1) != 0 && strcmp((char*)((s32)entry + 4), name) == 0) {
            break;
        }
        i++;
        entry = (void*)((s32)entry + 0x50);
    }

    entry = *(void**)((s32)set + 4);
    ret = 0;
    if (count > 0) {
        do {
            if ((*(u32*)entry & 1) == 0) {
                break;
            }
            ret++;
            entry = (void*)((s32)entry + 0x50);
            count--;
        } while (count != 0);
    }

    *(u32*)entry = 1;
    strcpy((char*)((s32)entry + 4), name);
    *(u16*)((s32)entry + 0x14) = 0x3FC;
    *(u16*)((s32)entry + 0x16) = 0x3FC;
    *(u16*)((s32)entry + 0x18) = 2;
    *(u16*)((s32)entry + 0x1A) = 2;
    *(u16*)((s32)entry + 0x48) = 0x3FC;
    *(u16*)((s32)entry + 0x4A) = 2;
    *(u16*)((s32)entry + 0x4C) = 2;
    *(u16*)((s32)entry + 0x4E) = 2;
    *(s32*)((s32)entry + 0x3C) = 5;
    *(s32*)((s32)entry + 0x40) = 5;
    return ret;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

void offscreenMain(void) {
    extern void* gp;
    extern s32 work;
    extern void dispEntry(s32 layer, s32 order, void* callback, f32 z, void* user);
    extern void offscreenDisp(void);
    extern const f32 float_0_804203a8;
    void* set = &work;
    s32 count;
    void* entry;

    if (*(s32*)((s32)gp + 0x14) != 0) {
        set = (void*)((s32)set + 8);
    }

    count = *(s32*)set;
    entry = *(void**)((s32)set + 4);
    while (count > 0) {
        u32 flags = *(u32*)entry;
        if ((flags & 1) != 0) {
            *(u32*)entry = flags & ~2;
            *(u32*)entry = *(u32*)entry & ~4;
            *(u16*)((s32)entry + 0x48) = *(u16*)((s32)entry + 0x14);
            *(u16*)((s32)entry + 0x4A) = *(u16*)((s32)entry + 0x16);
            *(u16*)((s32)entry + 0x4C) = *(u16*)((s32)entry + 0x18);
            *(u16*)((s32)entry + 0x4E) = *(u16*)((s32)entry + 0x1A);
            *(u16*)((s32)entry + 0x14) = 0x3FC;
            *(u16*)((s32)entry + 0x16) = 0x3FC;
            *(u16*)((s32)entry + 0x18) = 2;
            *(u16*)((s32)entry + 0x1A) = 2;
        }
        entry = (void*)((s32)entry + 0x50);
        count--;
    }
    dispEntry(1, 7, offscreenDisp, float_0_804203a8, 0);
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
