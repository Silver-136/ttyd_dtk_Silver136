#include "manager/arammgr.h"
#include "memory.h"
#include <dolphin/os.h>
#include <string.h>

#define ARAM_OWNER 0x19751128

//.bss
static ARAMWork work;

//.sdata
static ARAMWork* wp = &work;

//local prototypes
void aramCallbackGarbage(u32 request);
void arqCallbackToAram(u32 request);
void arqCallbackToMram(u32 request);

void aramMgrInit(void) {
    memset(wp, 0, sizeof(ARAMWork));
    if (!ARCheckInit()) {
        ARInit(wp->arStack, 8);
    }
    ARQInit();
    wp->size = 0x3FC000;
    wp->count = 64;
    wp->baseAddr = ARAlloc(wp->size);
    wp->nextAddr = wp->baseAddr;
    wp->entries = __memAlloc(HEAP_DEFAULT, wp->count * sizeof(ARAMEntry));
    memset(wp->entries, 0, wp->count * sizeof(ARAMEntry));
    wp->head = NULL;
    wp->tail = NULL;
}

void aramCallbackGarbage(u32 request) {
    wp->busy = 0;
    DCInvalidateRange(wp->buffer, wp->bufferSize);
    ICInvalidateRange(wp->buffer, wp->bufferSize);
}

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
        if (entry->flags & 2) {
            while (!(entry->flags & 4)) {}
        }
        if (entry->flags & 8) {
            while (!(entry->flags & 0x10)) {}
        }
    }

    entry = head;
    prev = NULL;
    first = NULL;
    while (entry != NULL) {
        if (entry->flags & 0x20) {
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
            wp->bufferSize = 0x8000;
            wp->buffer = __memAlloc(HEAP_DEFAULT, wp->bufferSize);
            n = (curr->size + (wp->bufferSize - 1)) / wp->bufferSize;
            for (i = 0, offset = 0; i < n; i++, offset += 0x8000) {
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
        if (request == (u32)&entry->request) break;
    }
    entry->flags |= 4;
}

ARAMEntry* aramMgrToAramAsync(void* src, u32 size) {
    ARAMEntry* entry = NULL;
    ARAMEntry* curr;
    u32 addr;
    int i;

    for (i = 0, curr = wp->entries; i < wp->count; i++, curr++) {
        if (!(curr->flags & 1)) {
            entry = curr;
            break;
        }
    }
    memset(entry, 0, sizeof(ARAMEntry));
    size = (size + 31) & ~31;
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
    entry->flags |= 1;
    entry->flags |= 2;
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

ARAMEntry* aramMgrToAram(void* src, u32 size) {
    ARAMEntry* entry = aramMgrToAramAsync(src, size);
    while (!(entry->flags & 4)) {}
    return entry;
}

void arqCallbackToMram(u32 request) {
    ARAMEntry* entry;

    for (entry = wp->head; entry != NULL; entry = entry->next) {
        if (request == (u32)&entry->request) break;
    }
    entry->flags |= 0x10;
    entry->flags |= 0x20;
    DCInvalidateRange(entry->dest, entry->size);
    ICInvalidateRange(entry->dest, entry->size);
}

void aramMgrToMram(ARAMEntry* entry, void* dest) {
    while (!(entry->flags & 4)) {}
    ARQPostRequest(&entry->request, ARAM_OWNER, ARQ_TYPE_ARAM_TO_MRAM, ARQ_PRIORITY_LOW,
        entry->aramAddr, (u32)dest, entry->size, arqCallbackToMram);
    entry->flags |= 8;
    entry->dest = dest;
    while (!(entry->flags & 0x10)) {}
}
