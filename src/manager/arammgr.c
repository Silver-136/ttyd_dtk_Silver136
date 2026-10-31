#include "manager/arammgr.h"
#include "memory.h"
#include <dolphin/os.h>
#include <string.h>

#define ARAM_OWNER 0x19751128
#define ARAM_GARBAGE_BUFFER_SIZE 0x8000
#define ARAM_POOL_BYTES 0x3FC000

//.bss
static ARAMWork work;

//.sdata
static ARAMWork* wp = &work;

/**
 * Boot-time setup from mariost.c: starts AR/ARQ, claims a 0x3FC000-byte ARAM region and a table of 64 entries.
 * mapdrv parks map data, textures and archives there between areas.
 */
void aramMgrInit(void) {
    memset(wp, 0, sizeof(ARAMWork));
    if (!ARCheckInit()) {
        ARInit(wp->arStack, 8);
    }
    ARQInit();
    wp->size = ARAM_POOL_BYTES;
    wp->count = 64;
    wp->baseAddr = ARAlloc(wp->size);
    wp->nextAddr = wp->baseAddr;
    wp->entries = __memAlloc(HEAP_DEFAULT, wp->count * sizeof(ARAMEntry));
    memset(wp->entries, 0, wp->count * sizeof(ARAMEntry));
    wp->head = NULL;
    wp->tail = NULL;
}

/**
 * ARQ completion for the compaction copies: clears the busy flag the copy loop spins on
 * and invalidates the bounce buffer.
 */
void aramCallbackGarbage(u32 request) {
    wp->busy = 0;
    DCInvalidateRange(wp->buffer, wp->bufferSize);
    ICInvalidateRange(wp->buffer, wp->bufferSize);
}

/**
 * Compacts ARAM when the bump allocator runs off the end:
 * waits for transfers in flight, unlinks entries already copied back to main RAM (flag 0x20),
 * then slides the live ones down through a 32 KB main-RAM bounce buffer.
 */
void aramMgrGarbage(void) {
    ARAMEntry* head;
    ARAMEntry* entry;
    ARAMEntry* prev;
    ARAMEntry* first;
    u32 offset;
    u32 addr;
    ARAMEntry* curr;
    int i, n;

    head = wp->head;
    for (entry = head; entry != NULL; entry = entry->next) {
        if (entry->flags & ARAM_FLAG_TO_ARAM) {
            while (!(entry->flags & ARAM_FLAG_IN_ARAM)) {}
        }
        if (entry->flags & ARAM_FLAG_TO_MRAM) {
            while (!(entry->flags & ARAM_FLAG_IN_MRAM)) {}
        }
    }

    entry = head;
    prev = NULL;
    first = NULL;
    while (entry != NULL) {
        if (entry->flags & ARAM_FLAG_FREE) {
            entry->flags = 0;
            if (prev != NULL) {
                prev->next = entry->next;
            }
            entry = entry->next;
        } else {
            if (prev == NULL) {
                first = entry;
            }
            prev = entry;
            entry = entry->next;
        }
    }
    wp->head = first;
    wp->tail = prev;

    addr = wp->baseAddr;
    for (curr = wp->head; curr != NULL; curr = curr->next) {
        if (curr->aramAddr != addr) {
            wp->bufferSize = ARAM_GARBAGE_BUFFER_SIZE;
            wp->buffer = __memAlloc(HEAP_DEFAULT, wp->bufferSize);
            n = (curr->size + (wp->bufferSize - 1)) / wp->bufferSize;
            for (i = 0, offset = 0; i < n; i++, offset += ARAM_GARBAGE_BUFFER_SIZE) {
                wp->busy = 1;
                ARQPostRequest(&wp->request, ARAM_OWNER, ARQ_TYPE_ARAM_TO_MRAM, ARQ_PRIORITY_LOW,
                    curr->aramAddr + offset, (u32)wp->buffer, wp->bufferSize, aramCallbackGarbage);
                while (wp->busy) {}
                wp->busy = 1;
                ARQPostRequest(&wp->request, ARAM_OWNER, ARQ_TYPE_MRAM_TO_ARAM, ARQ_PRIORITY_LOW,
                    (u32)wp->buffer, addr + offset, wp->bufferSize, aramCallbackGarbage);
                while (wp->busy) {}
            }
            __memFree(HEAP_DEFAULT, wp->buffer);
            wp->buffer = NULL;
        }
        curr->aramAddr = addr;
        addr = curr->aramAddr + curr->size;
    }
    wp->nextAddr = addr;
}

void arqCallbackToAram(u32 request) {
    ARAMEntry* entry;

    for (entry = wp->head; entry != NULL; entry = entry->next) {
        if (request == (u32)&entry->request) {
            break;
        }
    }
    entry->flags |= ARAM_FLAG_IN_ARAM;
}

/**
 * Starts copying size bytes (rounded up to 32) from main RAM to the next free ARAM space
 * and returns the entry tracking it; compacts ARAM first if the region is full.
 * Does not check for a free entry slot.
 */
ARAMEntry* aramMgrToAramAsync(void* src, u32 size) {
    ARAMEntry* entry = NULL;
    ARAMEntry* curr;
    u32 addr;
    int i;

    for (i = 0, curr = wp->entries; i < wp->count; i++, curr++) {
        if (!(curr->flags & ARAM_FLAG_IN_USE)) {
            entry = curr;
            break;
        }
    }
    // entry is still NULL here when all entries are in use; the pool is assumed to have room.
    memset(entry, 0, sizeof(ARAMEntry));
    size = OSRoundUp32B(size);
    addr = wp->nextAddr;
    wp->nextAddr += size;
    if (wp->nextAddr >= wp->baseAddr + wp->size) {
        aramMgrGarbage();
        addr = wp->nextAddr;
        wp->nextAddr += size;
    }
    DCStoreRange(src, size);
    ARQPostRequest(&entry->request, ARAM_OWNER, ARQ_TYPE_MRAM_TO_ARAM, ARQ_PRIORITY_LOW,
        (u32)src, addr, size, arqCallbackToAram);
    entry->flags |= ARAM_FLAG_IN_USE;
    entry->flags |= ARAM_FLAG_TO_ARAM;
    entry->mramAddr = src;
    entry->size = size;
    entry->aramAddr = addr;
    entry->next = NULL;
    if (wp->head == NULL) {
        wp->head = entry;
        wp->tail = entry;
    } else {
        wp->tail->next = entry;
        wp->tail = entry;
    }
    return entry;
}

/** Copies a block to ARAM and spins until the transfer is done; mapdrv uses it to stash the previous map's data. */
ARAMEntry* aramMgrToAram(void* src, u32 size) {
    ARAMEntry* entry = aramMgrToAramAsync(src, size);
    while (!(entry->flags & ARAM_FLAG_IN_ARAM)) {}
    return entry;
}

void arqCallbackToMram(u32 request) {
    ARAMEntry* entry;

    for (entry = wp->head; entry != NULL; entry = entry->next) {
        if (request == (u32)&entry->request) {
            break;
        }
    }
    entry->flags |= ARAM_FLAG_IN_MRAM;
    entry->flags |= ARAM_FLAG_FREE;
    DCInvalidateRange(entry->dest, entry->size);
    ICInvalidateRange(entry->dest, entry->size);
}

/**
 * Copies an entry back from ARAM to dest, waiting for its upload to finish first and then for the copy itself.
 * Each entry is read back once; mapdrv restores a stashed map with it.
 */
void aramMgrToMram(ARAMEntry* entry, void* dest) {
    while (!(entry->flags & ARAM_FLAG_IN_ARAM)) {}
    ARQPostRequest(&entry->request, ARAM_OWNER, ARQ_TYPE_ARAM_TO_MRAM, ARQ_PRIORITY_LOW,
        entry->aramAddr, (u32)dest, entry->size, arqCallbackToMram);
    entry->flags |= ARAM_FLAG_TO_MRAM;
    entry->dest = dest;
    while (!(entry->flags & ARAM_FLAG_IN_MRAM)) {}
}
