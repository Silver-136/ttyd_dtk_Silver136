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
    ;
}


void* DVDMgrOpen(const char* path, s32 mode, s32 unk) {
    return 0;
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


void DVDMgrRead(void* handle, void* dst, u32 size, s32 offset) {
    ;
}


int compare(void* param_1, void* param_2) {
    return 0;
}
