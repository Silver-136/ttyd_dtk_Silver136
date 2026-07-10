#include "manager/dvdmgr.h"

typedef struct DVDEntry {
    u8 pad0[0x74];
    s32 length;
    s32 pad78;
    void* dst;
    u32 size;
    u32 offset;
    s32 done;
    void* callback;
    u16 flags;
} DVDEntry;

u8 dvdmgr_thread[0x318];
s32 dvdmgr_thread_on;
void* _callback;
void* dvdq;

void DVDMgrMain(void);
void OSYieldThread(void);
void OSCancelThread(void* thread);
void OSResumeThread(void* thread);

s32 DVDMgrGetLength(DVDEntry* entry) {
    return entry->length;
}

void DVDMgrSetupCallback(void* callback) {
    _callback = callback;
}

void DVDMgrClose(DVDEntry* entry) {
    entry->flags |= 8;
}

void DVDMgrReadAsync(DVDEntry* entry, void* dst, u32 size, u32 offset, void* callback) {
    entry->dst = dst;
    entry->size = size;
    entry->offset = offset;
    entry->flags |= 1;
    entry->flags &= ~2;
    entry->callback = callback;
    entry->done = 0;
}

void DVDMgrDelete(void) {
    if (dvdmgr_thread_on != 0) {
        OSCancelThread(dvdmgr_thread);
        dvdmgr_thread_on = 0;
    }
}

void proc_main(void) {
    while (1) {
        DVDMgrMain();
        OSYieldThread();
    }
}


void DVDMgrMain(void) {
    extern void* memset(void* dst, s32 value, u32 size);
    extern void qqsort(void* base, s32 count, s32 size, void* compare);
    extern s32 DVDReadPrio(void* info, void* dst, s32 size, s32 offset, s32 prio);
    extern void DVDClose(void* info);
    extern int compare(void* param_1, void* param_2);
    extern void* dvdq;
    extern void* _callback;
    u8 order[0x100];
    u8* out;
    s32 count;
    s32 i;
    s32 index;
    void* entry;
    s32 didWork;
    s32 readSize;
    s32 result;
    void (*cb)(s32 result, void* info);
    void (*globalCallback)(void);

    memset(order, 0, 0x100);
    count = 0;
    out = order;
    entry = dvdq;
    index = 0;
    for (i = 0; i < 0x40; i++) {
        if ((*(u16*)((s32)entry + 0x90) & 0x8000) != 0) {
            *out++ = (u8)index;
            count++;
        }
        index++;
        if ((*(u16*)((s32)entry + 0x128) & 0x8000) != 0) {
            *out++ = (u8)index;
            count++;
        }
        index++;
        if ((*(u16*)((s32)entry + 0x1C0) & 0x8000) != 0) {
            *out++ = (u8)index;
            count++;
        }
        index++;
        if ((*(u16*)((s32)entry + 0x258) & 0x8000) != 0) {
            *out++ = (u8)index;
            count++;
        }
        index++;
        entry = (void*)((s32)entry + 0x260);
    }

    qqsort(order, count, 1, compare);

    out = order;
    for (i = 0; i < count; i++, out++) {
        entry = (void*)((s32)dvdq + *out * 0x98);
        didWork = 0;
        if ((*(u16*)((s32)entry + 0x90) & 0x80) == 0 && (*(u16*)((s32)entry + 0x90) & 1) != 0) {
            readSize = *(s32*)((s32)entry + 0x80);
            if (*(u16*)((s32)entry + 0x92) != 0 && readSize > 0x40000) {
                readSize = 0x40000;
            }

            result = DVDReadPrio((void*)((s32)entry + 0x40),
                                 *(void**)((s32)entry + 0x7C),
                                 readSize,
                                 *(s32*)((s32)entry + 0x84) + *(s32*)((s32)entry + 0x88),
                                 2);
            if (result == -3) {
                *(u16*)((s32)entry + 0x90) &= ~1;
                *(u16*)((s32)entry + 0x90) |= 2;
                cb = *(void (**)(s32 result, void* info))((s32)entry + 0x8C);
                if (cb != 0) {
                    cb(result, (void*)((s32)entry + 0x40));
                }
            } else if (result < 0) {
                *(u16*)((s32)entry + 0x90) |= 0x20;
                *(u16*)((s32)entry + 0x90) |= 0x80;
            } else {
                *(s32*)((s32)entry + 0x80) -= readSize;
                *(s32*)((s32)entry + 0x88) += readSize;
                *(void**)((s32)entry + 0x7C) = (void*)((s32)*(void**)((s32)entry + 0x7C) + readSize);
                if (*(s32*)((s32)entry + 0x80) == 0) {
                    *(u16*)((s32)entry + 0x90) &= ~1;
                    *(u16*)((s32)entry + 0x90) |= 2;
                    cb = *(void (**)(s32 result, void* info))((s32)entry + 0x8C);
                    if (cb != 0) {
                        cb(result, (void*)((s32)entry + 0x40));
                    }
                }
                didWork = 1;
            }
        }

        if ((*(u16*)((s32)entry + 0x90) & 8) != 0) {
            DVDClose((void*)((s32)entry + 0x40));
            memset(entry, 0, 0x98);
        }

        if (didWork != 0) {
            break;
        }
    }

    globalCallback = (void (*)(void))_callback;
    if (globalCallback != 0) {
        globalCallback();
    }
}

void* DVDMgrOpen(const char* path, s32 mode, s32 unk) {
    extern s32 DVDConvertPathToEntrynum(const char* path);
    extern s32 DVDOpen(const char* path, void* info);
    extern void* memset(void* dst, s32 value, u32 size);
    extern char* strcpy(char* dst, const char* src);
    extern void* dvdq;
    void* entry;
    s32 i;

    if (DVDConvertPathToEntrynum(path) == -1) {
        return 0;
    }

    entry = dvdq;
    for (i = 0; i < 0x100; i++, entry = (void*)((s32)entry + 0x98)) {
        if ((*(u16*)((s32)entry + 0x90) & 0x8000) == 0) {
            break;
        }
    }

    if (i >= 0x100) {
        return 0;
    }

    memset(entry, 0, 0x98);
    strcpy((char*)entry, path);
    *(u16*)((s32)entry + 0x90) = 0;
    *(u16*)((s32)entry + 0x92) = (u8)mode;
    *(u16*)((s32)entry + 0x94) = (u16)unk;
    *(s32*)((s32)entry + 0x7C) = 0;
    *(s32*)((s32)entry + 0x80) = 0;
    *(s32*)((s32)entry + 0x84) = 0;
    *(s32*)((s32)entry + 0x88) = 0;
    *(u16*)((s32)entry + 0x90) |= 0x8000;

    if (DVDOpen(path, (void*)((s32)entry + 0x40)) == 0) {
        memset(entry, 0, 0x98);
        return 0;
    }

    return entry;
}

u8 DVDMgrInit(void) {
    extern void* __memAlloc(s32 heap, u32 size);
    extern u8 stack[];
    extern s32 OSCreateThread(void* thread, void* func, void* param, void* stack, u32 stackSize, s32 priority, s16 attr);

    dvdq = __memAlloc(0, 0x9800);
    memset(dvdq, 0, 0x9800);
    if (OSCreateThread(dvdmgr_thread, proc_main, 0, stack + 0x4000, 0x4000, 0x10, 1) == 0) {
        while (1) {
        }
    }
    dvdmgr_thread_on = 1;
    OSResumeThread(dvdmgr_thread);
}


s32 DVDMgrRead(void* handle, void* dst, u32 size, s32 offset) {
    extern void OSYieldThread(void);
    s32 zero;

    zero = 0;
    *(void**)((s32)handle + 0x7C) = dst;
    *(u32*)((s32)handle + 0x80) = size;
    *(s32*)((s32)handle + 0x84) = offset;
    *(u16*)((s32)handle + 0x90) |= 1;
    *(u16*)((s32)handle + 0x90) &= ~2;
    *(s32*)((s32)handle + 0x8C) = zero;
    *(s32*)((s32)handle + 0x88) = zero;
    while (1) {
        if ((*(u16*)((s32)handle + 0x90) & 2) != 0) {
            break;
        }
        OSYieldThread();
    }
    return *(s32*)((s32)handle + 0x74);
}

s32 compare(u8* lhs, u8* rhs) {
    extern void* dvdq;
    u32 left;
    u32 right;

    left = *(u16*)((s32)dvdq + 0x92 + *lhs * 0x98) & 0xFF;
    right = *(u16*)((s32)dvdq + 0x92 + *rhs * 0x98) & 0xFF;
    if (left > right) {
        return 1;
    }
    if (left < right) {
        return -1;
    }
    return 0;
}

