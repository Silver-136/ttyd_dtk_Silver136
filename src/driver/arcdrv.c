#include "driver/arcdrv.h"
#include <string.h>

//.bss
static ArchiveEntry work[4];

void arcInit(void) {
    int i;

    for (i = 0; i < 4; i++) {
        memset(&work[i], 0, sizeof(ArchiveEntry));
    }
}

BOOL arcDataCheck(void* addr) {
    int i;

    for (i = 0; i < 4; i++) {
        if ((u8*)work[i].data <= (u8*)addr && (u8*)work[i].data + work[i].size >= (u8*)addr) {
            return TRUE;
        }
    }
    return FALSE;
}

void arcEntry(s32 id, void* data, u32 size) {
    ArchiveEntry* entry = &work[id];

    entry->data = data;
    entry->size = size;
    ARCInitHandle(entry->data, &entry->handle);
    entry->flags |= 1;
}

void arcDelete(s32 id) {
    memset(&work[id], 0, sizeof(ArchiveEntry));
}

void* arcOpen(const char* filename, void** addr, u32* length) {
    ARCFileInfo info;
    // FAKE: info used through the pointer infop; passing &info directly changes the object
    ARCFileInfo* infop = &info;
    int i;
    void* startAddr;

    for (i = 0; i < 4; i++) {
        if ((work[i].flags & 1) && ARCOpen(&work[i].handle, filename, infop)) {
            startAddr = ARCGetStartAddrInMem(infop);
            if (addr) {
                *addr = startAddr;
            }
            if (length) {
                *length = ARCGetLength(infop);
            }
            ARCClose(infop);
            return startAddr;
        }
    }
    if (addr) {
        *addr = NULL;
    }
    if (length) {
        *length = 0;
    }
    return NULL;
}
