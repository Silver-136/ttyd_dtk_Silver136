#include "manager/arammgr.h"

extern void* wp;
extern void DCInvalidateRange();
extern void ICInvalidateRange();
void* memset(void* ptr, int value, u32 size);
void DCStoreRange(void* ptr, u32 size);
void ARQPostRequest(void* request, u32 owner, s32 type, s32 priority, u32 source, u32 dest, u32 length, void* callback);
s32 ARCheckInit(void);
void ARInit(s32* stackIndexAddr, s32 numEntries);
void ARQInit(void);
u32 ARAlloc(u32 size);
void* __memAlloc(s32 heap, u32 size);
u8 aramMgrGarbage(void);
void arqCallbackToAram(void* arq);
void arqCallbackToMram(void* arq);

u8 aramMgrGarbage(void) {
    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* aramMgrToAramAsync(u32 mramAddr, int size) {
    void* entry = 0;
    void* work = wp;
    void* scan;
    s32 i;
    u32 aramAddr;

    scan = *(void**)((s32)work + 0x2C);
    for (i = 0; i < *(s32*)((s32)work + 0x30); i++) {
        if ((*(u16*)scan & 1) == 0) {
            entry = scan;
            break;
        }
        scan = (void*)((s32)scan + 0x38);
    }

    memset(entry, 0, 0x38);
    size = (size + 0x1F) & ~0x1F;
    work = wp;
    aramAddr = *(u32*)((s32)work + 0x20);
    *(u32*)((s32)work + 0x20) = aramAddr + size;
    work = wp;
    if (*(u32*)((s32)work + 0x20) >= *(u32*)((s32)work + 0x24) + *(u32*)((s32)work + 0x28)) {
        aramMgrGarbage();
        work = wp;
        aramAddr = *(u32*)((s32)work + 0x20);
        *(u32*)((s32)work + 0x20) = aramAddr + size;
    }
    DCStoreRange((void*)mramAddr, size);
    ARQPostRequest((void*)((s32)entry + 4), 0x19751128, 0, 0, mramAddr, aramAddr, size, arqCallbackToAram);
    *(u16*)entry |= 1;
    *(u16*)entry |= 2;
    *(u32*)((s32)entry + 0x2C) = mramAddr;
    *(s32*)((s32)entry + 0x24) = size;
    *(u32*)((s32)entry + 0x28) = aramAddr;
    *(s32*)((s32)entry + 0x34) = 0;
    work = wp;
    if (*(void**)((s32)work + 0x34) == 0) {
        *(void**)((s32)work + 0x34) = entry;
        work = wp;
        *(void**)((s32)work + 0x38) = entry;
    } else {
        *(void**)((s32)*(void**)((s32)work + 0x38) + 0x34) = entry;
        work = wp;
        *(void**)((s32)work + 0x38) = entry;
    }
    return entry;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

void aramMgrInit(void) {
    memset(wp, 0, 0x68);
    if (ARCheckInit() == 0) {
        ARInit(wp, 8);
    }
    ARQInit();
    *(u32*)((s32)wp + 0x28) = 0x3FC000;
    *(s32*)((s32)wp + 0x30) = 0x40;
    *(u32*)((s32)wp + 0x24) = ARAlloc(*(u32*)((s32)wp + 0x28));
    *(u32*)((s32)wp + 0x20) = *(u32*)((s32)wp + 0x24);
    *(void**)((s32)wp + 0x2C) = __memAlloc(0, *(s32*)((s32)wp + 0x30) * 0x38);
    memset(*(void**)((s32)wp + 0x2C), 0, *(s32*)((s32)wp + 0x30) * 0x38);
    *(s32*)((s32)wp + 0x34) = 0;
    *(s32*)((s32)wp + 0x38) = 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void aramMgrToMram(void* entry, s32 dest) {
    while ((*(volatile u16*)entry & 4) == 0) {
    }
    ARQPostRequest(
        (void*)((s32)entry + 4),
        0x19751128,
        1,
        0,
        *(u32*)((s32)entry + 0x28),
        dest,
        *(u32*)((s32)entry + 0x24),
        arqCallbackToMram
    );
    *(u16*)entry |= 8;
    *(s32*)((s32)entry + 0x30) = dest;
    while ((*(volatile u16*)entry & 0x10) == 0) {
    }
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


void arqCallbackToMram(void* arq) {
    void* entry = *(void**)((s32)wp + 0x34);

    goto check;
loop:
    if (arq == (void*)((s32)entry + 4)) {
        goto found;
    }
    entry = *(void**)((s32)entry + 0x34);
check:
    if (entry != NULL) {
        goto loop;
    }
found:
    *(u16*)entry |= 0x10;
    *(u16*)entry |= 0x20;
    DCInvalidateRange(*(void**)((s32)entry + 0x30), *(s32*)((s32)entry + 0x24));
    ICInvalidateRange(*(void**)((s32)entry + 0x30), *(s32*)((s32)entry + 0x24));
}


void aramCallbackGarbage(void) {
    *(s32*)((s32)wp + 0x5C) = 0;
    DCInvalidateRange(*(void**)((s32)wp + 0x60), *(s32*)((s32)wp + 0x64));
    ICInvalidateRange(*(void**)((s32)wp + 0x60), *(s32*)((s32)wp + 0x64));
}


void arqCallbackToAram(void* arq) {
    void* entry = *(void**)((s32)wp + 0x34);

    goto check;
loop:
    if (arq == (void*)((s32)entry + 4)) {
        goto found;
    }
    entry = *(void**)((s32)entry + 0x34);
check:
    if (entry != NULL) {
        goto loop;
    }
found:
    *(u16*)entry |= 4;
}


void* aramMgrToAram(u32 param_1, int param_2) {
    void* entry = aramMgrToAramAsync(param_1, param_2);

    while (1) {
        if (*(volatile u16*)entry & 4) {
            break;
        }
    }
    return entry;
}
