#include "driver/effdrv.h"

typedef struct EffEntry {
    u32 flags;
    s32 unk4;
    void* unk8;
    void* data;
    void* unk10;
    void* unk14;
    char name[0x10];
} EffEntry;

typedef struct EffWork {
    s32 count;
    EffEntry* entries;
    void* unk8;
    s32 unkC;
} EffWork;

typedef struct EffGp {
    u8 pad0[0x14];
    s32 unk14;
} EffGp;

extern EffWork* wp;
extern EffGp* gp;

void __memFree(s32 heap, void* ptr);
void UnpackTexPalette(void* tpl);
void DVDMgrClose(void* entry);
s32 strcmp(const char* s1, const char* s2);
char* strcpy(char* dst, const char* src);

void effDeleteMayaAnim(void* ptr) {
    __memFree(3, ptr);
}

void effDelete(EffEntry* entry) {
    if (entry != 0 && entry->flags != 0) {
        __memFree(3, entry->data);
        entry->flags = 0;
    }
}

void effSoftDelete(EffEntry* entry) {
    u32 flags;

    if (entry != 0) {
        flags = entry->flags;
        if (flags != 0) {
            if (flags & 2) {
                entry->flags = flags | 4;
            } else {
                if (entry != 0 && flags != 0) {
                    __memFree(3, entry->data);
                    entry->flags = 0;
                }
            }
        }
    }
}

EffEntry* effEntry(void) {
    EffEntry* entry = wp->entries;
    s32 count = wp->count;
    s32 i;

    for (i = 0; i < count; i++) {
        if (entry->flags == 0) {
            break;
        }
        entry++;
    }

    entry->flags = 1;
    entry->unk4 = (u32)(-gp->unk14 | gp->unk14) >> 31;
    entry->unk14 = 0;
    entry->unk8 = 0;
    entry->data = 0;
    entry->unk10 = 0;
    entry->name[0] = 0;
    return entry;
}

void effSetName(EffEntry* entry, char* name) {
    s32 i = 0;
    s32 count = wp->count;
    EffEntry* scan = wp->entries;

    while (i < count) {
        if (scan->flags != 0 && strcmp(scan->name, name) == 0) {
            break;
        }
        i++;
        scan++;
    }

    strcpy(entry->name, name);
}

void _callback_tpl(void* unk, void* dvdEntry) {
    void* closeEntry = *(void**)((s32)dvdEntry + 0x2C);

    UnpackTexPalette(wp->unk8);
    DVDMgrClose(closeEntry);
    wp->unkC = 1;
}
