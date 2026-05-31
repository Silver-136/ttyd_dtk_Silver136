#include "manager/arammgr.h"

extern void* wp;
extern void DCInvalidateRange();
extern void ICInvalidateRange();

u8 aramMgrGarbage(void) {
    return 0;
}


void* aramMgrToAramAsync(u32 param_1, int param_2) {
    return 0;
}


u8 aramMgrInit(void) {
    return 0;
}


u8 aramMgrToMram(void* param_1, s32 param_2) {
    return 0;
}


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
        if (*(u16*)entry & 4) {
            break;
        }
    }
    return entry;
}
