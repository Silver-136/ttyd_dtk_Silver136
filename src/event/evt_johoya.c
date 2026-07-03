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
    extern int sprintf(char* str, const char* format, ...);
    extern int strcmp(const char* s1, const char* s2);
    extern char* msgSearch(char* key);
    extern s32 _ismbblead(s32 c);
    char label[64];
    char key[76];
    char* scan;
    char* body;
    s16 bodyLen;
    s32 maxLabel = evtGetValue(NULL, GSW(0));
    s32 count = 0;
    s32 index = 0;
    s32 offset = 0;
    s32 copyField;
    s32 pos;
    s32 done;
    s32 labelIndex;
    char** table;

    while (1) {
        sprintf(key, "%s%04d", param_1, index);
        scan = msgSearch(key);
        if (scan[0] != '<' || scan[1] != '!') {
            return count;
        }

        scan += 2;
        copyField = 0;
        pos = 0;
        done = 0;
        while (!done) {
            if (_ismbblead(*scan) != 0) {
                if (copyField == 0) {
                    label[pos++] = scan[0];
                    label[pos++] = scan[1];
                } else {
                    pos += 2;
                }
                scan += 2;
            } else {
                if (*scan == '>') {
                    done = 1;
                } else if (*scan != ',') {
                    if (copyField == 0) {
                        label[pos++] = *scan++;
                    } else {
                        pos++;
                        scan++;
                    }
                    continue;
                }

                scan++;
                if (copyField == 0) {
                    label[pos] = 0;
                    body = scan;
                } else {
                    bodyLen = (s16)pos;
                }
                pos = 0;
                copyField++;
            }
        }

        if (strcmp(label, "enddata") == 0) {
            return count;
        }

        if (strcmp(label, "") != 0) {
            labelIndex = 0;
            table = evtNoLabel;
            while (labelIndex < 0x197) {
                if (strcmp(label, *table) == 0) {
                    break;
                }
                labelIndex++;
                table++;
            }
            if (labelIndex >= 0x197) {
                labelIndex = -1;
            }

            if (labelIndex != -1 && labelIndex <= maxLabel) {
                if ((param_2 & 1) != 0) {
                    *(s16*)((s32)_jdt.entries + offset + 2) = (s16)labelIndex;
                    *(char**)((s32)_jdt.entries + offset + 4) = body;
                    *(s16*)((s32)_jdt.entries + offset + 8) = bodyLen;
                    *(s16*)((s32)_jdt.entries + offset + 0xA) = 5;
                    *(s16*)((s32)_jdt.entries + offset + 0xC) = (s16)index;
                }
                offset += 0x10;
                count++;
            }
        }

        index++;
    }
}

s32 johoya_keti_newjoho_makelabel(int param_1) {
    extern int sprintf(char* str, const char* format, ...);
    extern char* msgSearch(char* key);
    extern u32 strlen(const char* str);
    EventEntry* event = (EventEntry*)param_1;
    s32* args = event->args;
    s32 target = evtGetValue(event, args[0]);
    s32 seen = 0;
    s32 i;
    s32 offset = 0;
    char titleKey[64];
    JohoyaEntry* entries = _jdt.entries;
    u8* flags = _jdt.flags;

    for (i = 0; i < _jdt.count; i++, offset += 0x10) {
        JohoyaEntry* entry = (JohoyaEntry*)((s32)entries + offset);
        s32 index = entry->unkC;
        s32 mask;
        s32 shift;

        switch (index & 7) {
            case 0: mask = 1; shift = 0; break;
            case 1: mask = 2; shift = 1; break;
            case 2: mask = 4; shift = 2; break;
            case 3: mask = 8; shift = 3; break;
            case 4: mask = 0x10; shift = 4; break;
            case 5: mask = 0x20; shift = 5; break;
            case 6: mask = 0x40; shift = 6; break;
            case 7: mask = 0x80; shift = 7; break;
        }

        if ((s16)((flags[index / 8] & (u8)mask) >> shift) == 0) {
            if (target == seen) {
                char* title;
                sprintf((char*)_jdt.end, "%s%04d", _jdt.unk8, entry->unkC);
                sprintf(titleKey, "jketi_title_%04d", entry->unkC);
                title = msgSearch(titleKey);
                evtSetValue(event, args[1], (s32)_jdt.end);
                evtSetValue(event, args[2], (s32)title);
                evtSetValue(event, args[3], strlen(title));
                evtSetValue(event, args[4], *(s16*)((s32)entry + 0xA));
                _jdt.index = i;
                return 2;
            }
            seen++;
        }
    }

    *(char*)_jdt.end = 0;
    evtSetValue(event, args[1], (s32)_jdt.end);
    evtSetValue(event, args[2], (s32)_jdt.end);
    evtSetValue(event, args[3], 0);
    evtSetValue(event, args[4], 0);
    return 2;
}

s32 johoya_keti_oldspeak_makelabel(int param_1) {
    extern int sprintf(char* str, const char* format, ...);
    EventEntry* event = (EventEntry*)param_1;
    s32* args = event->args;
    s32 target = evtGetValue(event, args[0]);
    s32 seen = 0;
    s32 i;
    s32 offset = 0;
    JohoyaEntry* entries = _jdt.entries;
    u8* flags = _jdt.flags;

    for (i = 0; i < _jdt.count; i++, offset += 0x10) {
        JohoyaEntry* entry = (JohoyaEntry*)((s32)entries + offset);
        s32 index = entry->unkC;
        s32 mask;
        s32 shift;

        switch (index & 7) {
            case 0: mask = 1; shift = 0; break;
            case 1: mask = 2; shift = 1; break;
            case 2: mask = 4; shift = 2; break;
            case 3: mask = 8; shift = 3; break;
            case 4: mask = 0x10; shift = 4; break;
            case 5: mask = 0x20; shift = 5; break;
            case 6: mask = 0x40; shift = 6; break;
            case 7: mask = 0x80; shift = 7; break;
        }

        if ((s16)((flags[index / 8] & (u8)mask) >> shift) != 0 && (entry->flags & 1) == 0) {
            if (target == seen) {
                sprintf((char*)_jdt.end, "%s%04d", _jdt.unk8, entry->unkC);
                evtSetValue(event, args[1], (s32)_jdt.end);
                evtSetValue(event, args[2], *(s32*)((s32)entry + 4));
                evtSetValue(event, args[3], *(s16*)((s32)entry + 8));
                evtSetValue(event, args[4], 0);
                _jdt.index = i;
                return 2;
            }
            seen++;
        }
    }

    *(char*)_jdt.end = 0;
    evtSetValue(event, args[1], (s32)_jdt.end);
    evtSetValue(event, args[2], (s32)_jdt.end);
    evtSetValue(event, args[3], 0);
    evtSetValue(event, args[4], 0);
    return 2;
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
