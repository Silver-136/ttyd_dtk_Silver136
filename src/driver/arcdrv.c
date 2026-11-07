// Archive driver: remembers where up to four loaded archives sit in memory, so other code can ask
// whether an address belongs to one.

#include "driver/arcdrv.h"
#include <string.h>

//.bss
static ArchiveEntry work[4];

/** Clears all four archive slots at startup. */
void arcInit(void) {
    int i;

    for (i = 0; i < 4; i++) {
        memset(&work[i], 0, sizeof(ArchiveEntry));
    }
}

/**
 * Tells whether addr falls inside any registered archive's data;
 * the map driver uses it to avoid freeing map data or textures that live inside the map archive.
 */
BOOL arcDataCheck(void* addr) {
    int i;

    for (i = 0; i < 4; i++) {
        if ((u8*)work[i].data <= (u8*)addr && (u8*)work[i].data + work[i].size >= (u8*)addr) {
            return TRUE;
        }
    }
    return FALSE;
}

/**
 * Registers a loaded archive in slot id (data and size) and opens an ARC handle on it
 * so arcOpen can find files in it. The map driver uses slots 1 and 2; seq_logo registers the startup archive.
 */
void arcEntry(s32 id, void* data, u32 size) {
    ArchiveEntry* entry = &work[id];

    entry->data = data;
    entry->size = size;
    ARCInitHandle(entry->data, &entry->handle);
    entry->flags |= ARC_FLAG_USED;
}

/** Forgets the archive in slot id (the memory itself is freed by the owner). */
void arcDelete(s32 id) {
    memset(&work[id], 0, sizeof(ArchiveEntry));
}

/**
 * Looks a file up by path in every registered archive.
 * On a hit it returns the file's address inside the archive and stores it and the length in addr
 * and length (either may be NULL); on a miss it returns NULL and zeroes both,
 * and callers fall back to reading the file from the disc.
 */
void* arcOpen(const char* filename, void** addr, u32* length) {
    ARCFileInfo info;

    ARCFileInfo* infop = &info;
    int i;
    void* startAddr;

    for (i = 0; i < 4; i++) {
        if ((work[i].flags & ARC_FLAG_USED) && ARCOpen(&work[i].handle, filename, infop)) {
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
