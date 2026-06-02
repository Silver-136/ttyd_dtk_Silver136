#include "event/evt_johoya.h"

typedef struct JohoyaEntry {
    u16 flags;
    u8 pad2[0xA];
    s16 unkC;
    u8 padE[2];
} JohoyaEntry;

typedef struct JohoyaData {
    JohoyaEntry* entries;
    u8 pad4[0xC];
    s32 index;
    u8 flags[0x27];
} JohoyaData;

extern char* evtNoLabel[];
JohoyaData _jdt;

void* memset(void* dst, int value, u32 size);
void johoya_set(void* flags, s32 value, s32 set);

char* search_evt_moji(s32 index) {
    return evtNoLabel[index];
}

void johoya_init(void) {
    u8* flags = _jdt.flags;

    memset(flags, 0, 0xD);
    memset(flags + 0xD, 0, 0xD);
    memset(flags + 0x1A, 0, 0xD);
}

s32 johoya_luigi_newjoho_setreadflag(void) {
    johoya_set(_jdt.flags + 0x1A, _jdt.entries[_jdt.index].unkC, 1);
    return 2;
}

s32 johoya_perfect_newjoho_setreadflag(void) {
    johoya_set(_jdt.flags + 0xD, _jdt.entries[_jdt.index].unkC, 1);
    return 2;
}

s32 johoya_keti_oldspeak_setreadflag(void) {
    _jdt.entries[_jdt.index].flags |= 1;
    return 2;
}

s32 johoya_keti_newjoho_setreadflag(void) {
    s32 offset = _jdt.index << 4;
    void* entry = _jdt.entries;

    entry = (void*)((s32)entry + offset);
    johoya_set(_jdt.flags, *(s16*)((s32)entry + 0xC), 1);
    *(u16*)((s32)_jdt.entries + offset) |= 1;
    return 2;
}


int johoya_data_make(s32 param_1, u32 param_2) {
    return 0;
}


s32 johoya_keti_newjoho_makelabel(int param_1) {
    return 0;
}


s32 johoya_keti_oldspeak_makelabel(int param_1) {
    return 0;
}


u8 johoya_keti_oldspeak_check(void) {
    return 0;
}


u8 johoya_keti_newjoho_check(void) {
    return 0;
}


void johoya_set(void* flags, s32 value, s32 set) {
}


s32 johoya_data_alloc(void* pEvt) {
    return 0;
}


int johoya_get(int param_1, short param_2) {
    return 0;
}


int search_evt_no(void* pEvt) {
    return 0;
}


s32 johoya_data_free(void) {
    return 0;
}
