#include "event/evt_johoya.h"
#include "event/evt_cmd.h"

typedef struct JohoyaEntry {
    u16 flags;
    u8 pad2[0xA];
    s16 unkC;
    u8 padE[2];
} JohoyaEntry;

typedef struct JohoyaData {
    JohoyaEntry* entries;
    s32 count;
    s32 unk8;
    JohoyaEntry* end;
    s32 index;
    u8 flags[0x27];
} JohoyaData;

extern char* evtNoLabel[];
extern void* mapalloc_base_ptr;
JohoyaData _jdt;

void* memset(void* dst, int value, u32 size);
s32 johoya_set(void* flags, s32 value, s32 set);
void* _mapAlloc(void* heap, u32 size);

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


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(johoya_keti_oldspeak_check) {
    JohoyaData* data = &_jdt;
    s32 count = 0;
    s32 offset = 0;
    u8* flags = data->flags;
    s32 i = 0;
    s32* args = event->args;
    s32 total = data->count;
    JohoyaEntry* entries = data->entries;

    while (i < total) {
        JohoyaEntry* entry = (JohoyaEntry*)((s32)entries + offset);
        s32 index = entry->unkC;
        s32 mask;
        s32 shift;

        switch (index & 7) {
            case 0:
                mask = 1;
                shift = 0;
                break;
            case 1:
                mask = 2;
                shift = 1;
                break;
            case 2:
                mask = 4;
                shift = 2;
                break;
            case 3:
                mask = 8;
                shift = 3;
                break;
            case 4:
                mask = 0x10;
                shift = 4;
                break;
            case 5:
                mask = 0x20;
                shift = 5;
                break;
            case 6:
                mask = 0x40;
                shift = 6;
                break;
            case 7:
                mask = 0x80;
                shift = 7;
                break;
        }

        if ((s16)((flags[index / 8] & (u8)mask) >> shift) != 0 && (entry->flags & 1) == 0) {
            count++;
        }
        offset += 0x10;
        i++;
    }

    evtSetValue(event, args[0], count);
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(johoya_keti_newjoho_check) {
    JohoyaData* data = &_jdt;
    s32 count = 0;
    s32 offset = 0;
    u8* flags = data->flags;
    s32 i = 0;
    s32* args = event->args;
    s32 total = data->count;
    JohoyaEntry* entries = data->entries;

    while (i < total) {
        s32 entryOffset = offset + 0xC;
        s32 index = *(s16*)((s32)entries + entryOffset);
        s32 mask;
        s32 shift;

        switch (index & 7) {
            case 0:
                mask = 1;
                shift = 0;
                break;
            case 1:
                mask = 2;
                shift = 1;
                break;
            case 2:
                mask = 4;
                shift = 2;
                break;
            case 3:
                mask = 8;
                shift = 3;
                break;
            case 4:
                mask = 0x10;
                shift = 4;
                break;
            case 5:
                mask = 0x20;
                shift = 5;
                break;
            case 6:
                mask = 0x40;
                shift = 6;
                break;
            case 7:
                mask = 0x80;
                shift = 7;
                break;
        }

        if ((s16)((flags[index / 8] & (u8)mask) >> shift) == 0) {
            count++;
        }
        offset += 0x10;
        i++;
    }

    evtSetValue(event, args[0], count);
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

s32 johoya_set(void* flags, s32 value, s32 set) {
    s32 index = (s16)value;
    s32 mask;
    s32 shift;
    s32 byteIndex;
    u8 cleared;
    u8 shifted;

    switch (index & 7) {
        case 0:
            mask = 0xFE;
            shift = 0;
            break;
        case 1:
            mask = 0xFD;
            shift = 1;
            break;
        case 2:
            mask = 0xFB;
            shift = 2;
            break;
        case 3:
            mask = 0xF7;
            shift = 3;
            break;
        case 4:
            mask = 0xEF;
            shift = 4;
            break;
        case 5:
            mask = 0xDF;
            shift = 5;
            break;
        case 6:
            mask = 0xBF;
            shift = 6;
            break;
        case 7:
            mask = 0x7F;
            shift = 7;
            break;
    }

    byteIndex = index / 8;
    if ((s16)set < 0) {
        set = 0;
    } else if ((s16)set > 1) {
        set = 1;
    }

    cleared = (u8)(((u8*)flags)[byteIndex] & mask);
    shifted = (u8)(((s16)set) << shift);
    ((u8*)flags)[byteIndex] = cleared + shifted;
    return set;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(johoya_data_alloc) {
    JohoyaData* data = &_jdt;
    s32* args = event->args;
    s32 value = evtGetValue(event, args[0]);
    s32 count;
    s32 size;

    data->unk8 = value;
    count = johoya_data_make(value, 0);
    data->count = count;
    size = (count << 4) + 0x40;
    data->entries = _mapAlloc(mapalloc_base_ptr, size);
    memset(data->entries, 0, (data->count << 4) + 0x40);
    data->end = (JohoyaEntry*)((s32)data->entries + (data->count << 4));
    johoya_data_make(data->unk8, 1);
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

s32 johoya_get(void* flags, s16 value) {
    s32 index = value;
    s32 mask;
    s32 shift;

    switch (index & 7) {
        case 0:
            mask = 1;
            shift = 0;
            break;
        case 1:
            mask = 2;
            shift = 1;
            break;
        case 2:
            mask = 4;
            shift = 2;
            break;
        case 3:
            mask = 8;
            shift = 3;
            break;
        case 4:
            mask = 0x10;
            shift = 4;
            break;
        case 5:
            mask = 0x20;
            shift = 5;
            break;
        case 6:
            mask = 0x40;
            shift = 6;
            break;
        case 7:
            mask = 0x80;
            shift = 7;
            break;
    }

    return (s16)((((u8*)flags)[index / 8] & (u8)mask) >> shift);
}

int search_evt_no(void* pEvt) {
    return 0;
}


s32 johoya_data_free(void) {
    return 0;
}
