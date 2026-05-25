#include "driver/arcdrv.h"

//.bss
ArchiveEntry work[4];

void arcInit(void) {
    int i;

    for (i = 0; i < 4; i++) {
        memset(&work[i], 0, sizeof(ArchiveEntry));
    }
}

BOOL arcDataCheck(void* address) {
    int i;
    ArchiveEntry* entry;

    for (i = 0, entry = work; i < 4; i++, entry++) {
        if (entry->data <= address && (void*)((u32)entry->data + entry->size) >= address) {
            return TRUE;
        }
    }
    return FALSE;
}

void arcEntry(s32 index, void* data, u32 size) {
    ArchiveEntry* entry = &work[index];

    entry->data = data;
    entry->size = size;
    ARCInitHandle(entry->data, &work[index].handle);
    entry->flags |= 1;
}

void arcDelete(s32 index) {
    memset(&work[index], 0, sizeof(ArchiveEntry));
}

void* arcOpen(const char* filename, void** address, u32* length) {
    ARCFileInfo temp;
    ARCFileInfo* info = &temp;
    int i;

    for (i = 0; i < 4; i++) {
        if ((work[i].flags & 1) && ARCOpen(&work[i].handle, filename, info)) {
            void* startAddr = ARCGetStartAddrInMem(info);
            if (address) {
                *address = startAddr;
            }
            if (length) {
                *length = ARCGetLength(info);
            }
            ARCClose(info);
            return startAddr;
        }
    }
    if (address) {
        *address = NULL;
    }
    if (length) {
        *length = 0;
    }
    return NULL;
}

