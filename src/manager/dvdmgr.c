/* "dvdmgr" - DVD File Manager
 * Easy file handling for loading data from the disc, instead of having to call DVD functions directly and handle errors.
 */
#include "version.h"
#include "manager/dvdmgr.h"

#define DVD_PRIORITY_CHUNK_BYTES 0x40000
#include "memory.h"
#include "system.h"
#include <dolphin/os.h>
#include <string.h>

//.bss
OSThread dvdmgr_thread;
static u8 stack[0x4000];

//.sbss
DVDEntry* dvdq;
static DVDMgrCallback _callback;
BOOL dvdmgr_thread_on;

/** Body of the DVD manager thread: services the read queue once and yields, forever. */
void* proc_main(void* param) {
    while (1) {
        DVDMgrMain();
        OSYieldThread();
    }
}

/**
 * Boot-time setup from mariost.c: allocates the 256-entry request queue
 * and starts the manager thread (priority 16). Hangs if the thread cannot be created.
 */
void DVDMgrInit(void) {
    dvdq = __memAlloc(HEAP_DEFAULT, sizeof(DVDEntry) * DVDEntryCount);
    memset(dvdq, 0, sizeof(DVDEntry) * DVDEntryCount);
    if (!OSCreateThread(&dvdmgr_thread, proc_main, NULL, stack + sizeof(stack), sizeof(stack), 16, OS_THREAD_ATTR_DETACH)) {
        while (1) {}
    }
    dvdmgr_thread_on = TRUE;
    OSResumeThread(&dvdmgr_thread);
}

/** Stops the manager thread; mariost.c calls it before a reset or returning to the menu. */
void DVDMgrDelete(void) {
    if (dvdmgr_thread_on) {
        OSCancelThread(&dvdmgr_thread);
        dvdmgr_thread_on = FALSE;
    }
}

#if VERSION <= VERSION_DEMO_US
static s32 compare(const void* a, const void* b) {
#else
static s32 compare(const void** a, const void** b) {
#endif
    u8 value1, value2;
#if VERSION <= VERSION_DEMO_US
value1 = *(u8*)a; // (a const cast here changes the demo code)
    value2 = *(u8*)b;
#else

    value1 = (u8)dvdq[*(u8*)a].priority;
    value2 = (u8)dvdq[*(u8*)b].priority;
#endif
    if (value1 > value2) {
        return 1;
    } else {
        return -(value1 < value2);
    }
}

/**
 * One pass of the DVD thread: sorts the open requests by priority,
 * issues the next chunk of the first pending read (prioritised reads in 256 KB pieces),
 * fires the callback when a read completes, frees closed entries, then runs the hook from DVDMgrSetupCallback.
 * Only one chunk is read per pass.
 */
void DVDMgrMain(void) {
    u8 array[DVDEntryCount];
    u8* table;
    DVDEntry* entry;
    BOOL valid;
    s32 result;
    s32 count;
    s32 size;
    s32 i;

    count = 0;
    memset(array, 0, sizeof(array));

    for (table = array, entry = dvdq, i = 0; i < DVDEntryCount; i++, entry++) {
        if (entry->status & DVDMGR_INUSE) {
            *table = i;
            count++;
            table++;
        }
    }

    qqsort(array, count, sizeof(u8), compare);

    for (table = array, i = 0; i < count; table++, i++) {
        valid = FALSE;
        entry = &dvdq[*table];
        if (!(entry->status & DVDMGR_UNK_ERROR2) && entry->status & DVDMGR_READING) {
            size = entry->bytesLeft;
            if (entry->priority) {
                if (size > DVD_PRIORITY_CHUNK_BYTES) {
                    size = DVD_PRIORITY_CHUNK_BYTES;
                }
            }

            result = DVDRead(&entry->info, entry->address, size, entry->offset + entry->position);
            if (result == DVD_RESULT_CANCELED) {
                entry->status &= ~DVDMGR_READING;
                entry->status |= DVDMGR_FINISHED;
                if (entry->callback != NULL) {
                    entry->callback(result, &entry->info);
                }
            } else if (result < 0) {
                entry->status |= DVDMGR_UNK_ERROR1;
                entry->status |= DVDMGR_UNK_ERROR2;
            } else {
                entry->bytesLeft -= size;
                entry->position += size;
                entry->address = (u8*)entry->address + size;
                if (entry->bytesLeft == 0) {
                    entry->status &= ~DVDMGR_READING;
                    entry->status |= DVDMGR_FINISHED;
                    if (entry->callback != NULL) {
                        entry->callback(result, &entry->info);
                    }
                }
                valid = TRUE;
            }
        }

        if (entry->status & DVDMGR_CLOSED) {
            DVDClose(&entry->info);
#if VERSION <= VERSION_DEMO_US
entry->status &= ~DVDMGR_INUSE;
#endif
            memset(entry, 0, sizeof(DVDEntry));
        }

        if (valid) {
            break;
        }
    }
    if (_callback != NULL) {
        _callback();
    }
}

/**
 * Opens a disc file into a free queue entry with the given read priority;
 * NULL if the file does not exist, the queue is full or DVDOpen fails.
 */
DVDEntry* DVDMgrOpen(const char* path, u8 priority, u16 unknown) {
    DVDEntry* entry;
    int i;

    if (DVDConvertPathToEntrynum(path) == -1) {
        return NULL;
    }

    for (entry = dvdq, i = 0; i < DVDEntryCount; i++, entry++) {
        if (!(entry->status & DVDMGR_INUSE)) {
            break;
        }
    }
    if (i >= DVDEntryCount) {
        return NULL;
    }
    memset(entry, 0, sizeof(DVDEntry));
    strcpy(entry->name, path);
    entry->status = 0;
    entry->priority = priority;
    entry->unknown = unknown;
    entry->address = NULL;
    entry->bytesLeft = 0;
    entry->offset = 0;
    entry->position = 0;
    entry->status |= DVDMGR_INUSE;
    if (!DVDOpen(entry->name, &entry->info)) {
#if VERSION <= VERSION_DEMO_US
entry->status |= DVDMGR_OPEN_FAILED;
        entry->status |= DVDMGR_UNK_ERROR2;
        entry->status &= ~DVDMGR_INUSE;
#endif
        memset(entry, 0, sizeof(DVDEntry));
        return NULL;
    }
    return entry;
}

/**
 * Reads size bytes at offset into address through the DVD thread, yielding until it is done.
 * Returns the file's length, not the byte count read. return is s32 even though it cannot be negative
 */
// Waits until the read has finished; returns the file length (not the number of bytes read).
s32 DVDMgrRead(DVDEntry* entry, void* address, u32 size, s32 offset) {
    entry->address = address;
    entry->bytesLeft = size;
    entry->offset = offset;
    entry->status |= DVDMGR_READING;
    entry->status &= ~DVDMGR_FINISHED;
    entry->callback = NULL;
    entry->position = 0;
    while (TRUE) {
        if (entry->status & DVDMGR_FINISHED) {
            break;
        }
        OSYieldThread();
    }
    return entry->info.length;
}

/**
 * Queues a read of size bytes at offset into address and returns at once;
 * the DVD thread calls callback when it is done.
 */
void DVDMgrReadAsync(DVDEntry* entry, void* address, u32 size, s32 offset, DVDCallback callback) {
    entry->address = address;
    entry->bytesLeft = size;
    entry->offset = offset;
    entry->status |= DVDMGR_READING;
    entry->status &= ~DVDMGR_FINISHED;
    entry->callback = callback;
    entry->position = 0;
}

/** Marks the entry for closing; the DVD thread closes the file and frees the entry on its next pass. */
void DVDMgrClose(DVDEntry* entry) {
    entry->status |= DVDMGR_CLOSED;
}

/** Size in bytes of the opened file. */
u32 DVDMgrGetLength(DVDEntry* entry) {
    return entry->info.length;
}

/**
 * Sets a hook the DVD thread runs after every pass;
 * the logo sequence installs psndPushGroup so sound groups load in the background,
 * and clears it again when done.
 */
void DVDMgrSetupCallback(DVDMgrCallback callback) {
    _callback = callback;
}
