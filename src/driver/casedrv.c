#include "driver/casedrv.h"

extern void* wp;
extern void* __memAlloc();

u8 caseMain(void) {
    return 0;
}


u8 caseInit(void) {
    *(s32*)wp = 0x40;
    *(void**)((s32)wp + 4) = __memAlloc(0, *(s32*)wp * 0xA0);
    memset(*(void**)((s32)wp + 4), 0, *(s32*)wp * 0xA0);
}


void caseReInit(void) {
    void* entry = *(void**)((s32)wp + 4);
    s32 i = 0;

    while (i < *(s32*)wp) {
        *(u16*)entry &= ~1;
        i++;
        entry = (void*)((s32)entry + 0xA0);
    }
}


void caseDelete(s32 id) {
    void* entry = (void*)((s32)*(void**)((s32)wp + 4) + id * 0xA0);

    *(u16*)entry &= ~1;
}


s32 caseEntry(void* param) {
    extern void* wp;
    extern void* memset(void* dst, s32 value, u32 size);
    extern void* memcpy(void* dst, const void* src, u32 size);
    extern char* strcpy(char* dst, const char* src);
    void* work = wp;
    s32 count = *(s32*)work;
    void* entry = *(void**)((s32)work + 4);
    s32 id = 0;

    if (count > 0) {
        do {
            if ((*(u16*)entry & 1) == 0) {
                break;
            }
            id++;
            entry = (void*)((s32)entry + 0xA0);
            count--;
        } while (count != 0);
    }

    memset(entry, 0, 0xA0);
    *(s32*)((s32)entry + 0x48) = id;
    *(u16*)entry = 0;
    *(s32*)((s32)entry + 4) = *(u16*)param & ~0x8000;
    if ((*(u16*)param & 0x8000) != 0) {
        *(u16*)entry |= 4;
    }
    *(u16*)entry |= 1;
    strcpy((char*)((s32)entry + 8), *(char**)((s32)param + 4));
    *(s32*)((s32)entry + 0x4C) = *(s32*)((s32)param + 8);
    *(s32*)((s32)entry + 0x50) = *(s32*)((s32)param + 0xC);
    *(s32*)((s32)entry + 0x98) = *(s32*)((s32)param + 0x54);
    *(s32*)((s32)entry + 0x9C) = *(s32*)((s32)param + 0x58);
    *(s32*)((s32)entry + 0x94) = 0;
    memcpy((void*)((s32)entry + 0x54), (void*)((s32)param + 0x10), 0x40);
    return id;
}

void* caseCheckHitObj2(u32 pHit) {
    extern void* gp;
    extern void* wp;
    extern const char* hitGetName(void* hit);
    extern int strcmp(const char* a, const char* b);
    void* entry;
    s32 i;

    if (*(s32*)((s32)gp + 0x14) != 0) {
        return 0;
    }

    i = 0;
    entry = *(void**)((s32)wp + 4);
    while (i < *(s32*)wp) {
        s32 type;
        if ((*(u16*)entry & 1) != 0) {
            type = *(s32*)((s32)entry + 4);
            if (type != 0x11 && (u32)(type - 0x12) > 1 && type == 0xF) {
                if (strcmp(hitGetName((void*)pHit), (const char*)((s32)entry + 8)) == 0) {
                    return entry;
                }
            }
        }
        i++;
        entry = (void*)((s32)entry + 0xA0);
    }
    return 0;
}

void* caseCheckHitObj(void* pHit) {
    extern void* gp;
    extern void* wp;
    extern const char* hitGetName(void* hit);
    extern int strcmp(const char* a, const char* b);
    void* entry;
    s32 i;

    if (*(s32*)((s32)gp + 0x14) != 0) {
        return 0;
    }

    i = 0;
    entry = *(void**)((s32)wp + 4);
    while (i < *(s32*)wp) {
        s32 type;
        if ((*(u16*)entry & 1) != 0) {
            type = *(s32*)((s32)entry + 4);
            if (type != 0x11 && (u32)(type - 0x12) > 1) {
                if (strcmp(hitGetName(pHit), (const char*)((s32)entry + 8)) == 0) {
                    return entry;
                }
            }
        }
        i++;
        entry = (void*)((s32)entry + 0xA0);
    }
    return 0;
}

void* caseIdToPtr(s32 id) {
    void* entry = (void*)((s32)*(void**)((s32)wp + 4) + id * 0xA0);

    if (*(u16*)entry & 1) {
        return entry;
    }
    return NULL;
}
