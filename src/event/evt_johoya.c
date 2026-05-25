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
    JohoyaEntry* entry = (JohoyaEntry*)((s32)_jdt.entries + offset);

    johoya_set(_jdt.flags, entry->unkC, 1);
    *(u16*)((s32)_jdt.entries + offset) |= 1;
    return 2;
}

