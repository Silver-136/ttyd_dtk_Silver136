#include "event/evt_uranaisi.h"
#include "event/evt_cmd.h"

typedef struct UranaisiData {
    void* table;
    s32 next;
    s32 unk8;
    s32 unkC;
    s32 index;
    s32 type;
    u8 starpieceFlags[0xD];
    u8 supercoinFlags[0x6];
    u8 pad2B[0x38 - 0x2B];
} UranaisiData;

UranaisiData _udt;
s32 uranaisi_ryokin_tbl[3];
extern void* mapalloc_base_ptr;
extern const char str_PCTs_PCT04d_802fdda0[];

s32 evtGetValue(EventEntry* event, s32 value);
s32 evtSetValue(EventEntry* event, s32 target, s32 value);
void* memset(void* dst, int value, u32 size);
void* _mapAlloc(void* heap, u32 size);
void _mapFree(void* heap, void* ptr);
s32 johoya_get(void* flags, s32 id);
void johoya_set(void* flags, s32 id, s32 value);
void* pouchGetPtr(void);
s32 sprintf(char* dst, const char* fmt, ...);
int uranaisi_data_make_next(s32 param_1, u32 param_2);
int uranaisi_data_make_starpiece(s32 param_1, u32 param_2);
int uranaisi_data_make_supercoin(s32 param_1, u32 param_2);

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(uranaisi_data_alloc) {
    UranaisiData* data = &_udt;
    UranaisiData* initData;
    s32* args = event->args;
    s32 count;
    s32 base;

    initData = &_udt;
    initData->type = evtGetValue(event, args[0]);
    initData->unk8 = evtGetValue(event, args[1]);
    switch (initData->type) {
        case 0:
            count = uranaisi_data_make_next(data->unk8, 0);
            break;
        case 1:
            count = uranaisi_data_make_starpiece(data->unk8, 0);
            break;
        case 2:
            count = uranaisi_data_make_supercoin(data->unk8, 0);
            break;
        default:
            count = 0;
            break;
    }

    data->next = count;
    data->table = _mapAlloc(mapalloc_base_ptr, count * 6 + 0x40);
    memset(data->table, 0, data->next * 6 + 0x40);
    data->unkC = (s32)data->table + data->next * 6;
    base = data->unk8;
    switch (data->type) {
        case 0:
            uranaisi_data_make_next(base, 1);
            break;
        case 1:
            uranaisi_data_make_starpiece(base, 1);
            break;
        case 2:
            uranaisi_data_make_supercoin(base, 1);
            break;
    }
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


USER_FUNC(uranaisi_data_free) {
    UranaisiData* data = &_udt;

    if (data->table == 0) {
        return 2;
    }
    _mapFree(mapalloc_base_ptr, data->table);
    data->table = 0;
    data->next = 0;
    data->unk8 = 0;
    data->unkC = 0;
    data->index = 0;
    return 2;
}

USER_FUNC(uranaisi_table_clear) {
    memset(&_udt.starpieceFlags, 0, 0xD);
    memset(&_udt.supercoinFlags, 0, 6);
    return 2;
}

USER_FUNC(uranaisi_next_check) {
    evtSetValue(event, event->args[0], _udt.next);
    return 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(uranaisi_next_makelabel) {
    s32* args = event->args;
    s32 index = evtGetValue(event, args[0]);
    UranaisiData* data = &_udt;
    s32 offset = index * 6;
    void* entry = (void*)((s32)data->table + offset);

    sprintf((char*)data->unkC, str_PCTs_PCT04d_802fdda0, data->unk8,
            *(s16*)((s32)entry + 4));
    evtSetValue(event, args[1], data->unkC);
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

USER_FUNC(uranaisi_starpiece_check) {
    UranaisiData* data = &_udt;
    s32 visible = 0;
    s32 i = 0;
    s32 offset = 0;
    s32* args = event->args;

    while (i < data->next) {
        if ((s16)johoya_get(&data->starpieceFlags, *(s16*)((s32)data->table + offset + 4)) == 0) {
            visible++;
        }
        offset += 6;
        i++;
    }
    evtSetValue(event, args[0], visible);
    return 2;
}

USER_FUNC(uranaisi_starpiece_makelabel) {
    s32* args = event->args;
    s32 target = evtGetValue(event, args[0]);
    UranaisiData* data = &_udt;
    s32 visible = 0;
    s32 i = 0;
    s32 offset = 0;

    while (i < data->next) {
        if ((s16)johoya_get(&data->starpieceFlags, *(s16*)((s32)data->table + offset + 4)) == 0) {
            if (target == visible) {
                sprintf((char*)data->unkC, str_PCTs_PCT04d_802fdda0, data->unk8,
                        *(s16*)((s32)data->table + i * 6 + 4));
                evtSetValue(event, args[1], data->unkC);
                data->index = i;
                return 2;
            }
            visible++;
        }
        offset += 6;
        i++;
    }
    *(u8*)data->unkC = 0;
    evtSetValue(event, args[1], data->unkC);
    return 2;
}

USER_FUNC(uranaisi_starpiece_setreadflag) {
    s32 id = *(s16*)((s32)_udt.table + _udt.index * 6 + 4);
    johoya_set(&_udt.starpieceFlags, id, 1);
    return 2;
}

USER_FUNC(uranaisi_supercoin_check) {
    UranaisiData* data = &_udt;
    s32 visible = 0;
    s32 i = 0;
    s32 offset = 0;
    s32* args = event->args;

    while (i < data->next) {
        if ((s16)johoya_get(&data->supercoinFlags, *(s16*)((s32)data->table + offset + 4)) == 0) {
            visible++;
        }
        offset += 6;
        i++;
    }
    evtSetValue(event, args[0], visible);
    return 2;
}

USER_FUNC(uranaisi_supercoin_makelabel) {
    s32* args = event->args;
    s32 target = evtGetValue(event, args[0]);
    UranaisiData* data = &_udt;
    s32 visible = 0;
    s32 i = 0;
    s32 offset = 0;

    while (i < data->next) {
        if ((s16)johoya_get(&data->supercoinFlags, *(s16*)((s32)data->table + offset + 4)) == 0) {
            if (target == visible) {
                sprintf((char*)data->unkC, str_PCTs_PCT04d_802fdda0, data->unk8,
                        *(s16*)((s32)data->table + i * 6 + 4));
                evtSetValue(event, args[1], data->unkC);
                data->index = i;
                return 2;
            }
            visible++;
        }
        offset += 6;
        i++;
    }
    *(u8*)data->unkC = 0;
    evtSetValue(event, args[1], data->unkC);
    return 2;
}

USER_FUNC(uranaisi_supercoin_setreadflag) {
    s32 id = *(s16*)((s32)_udt.table + _udt.index * 6 + 4);
    johoya_set(&_udt.supercoinFlags, id, 1);
    return 2;
}

int uranaisi_data_make_next(s32 param_1, u32 param_2) {
    extern int strcmp(const char* s1, const char* s2);
    extern char* msgSearch(char* key);
    extern s32 _ismbblead(s32 c);
    extern int search_evt_no(void* pEvt);
    char label[64];
    char key[72];
    char* scan;
    s32 maxLabel = evtGetValue(NULL, GSW(0));
    s32 count;
    s32 index;
    s32 field;
    s32 pos;
    s32 done;
    s32 labelNo;
    s32* nextBest = (s32*)((s32)&_udt + 0x34);

    if ((param_2 & 1) == 0) {
        *nextBest = -1;
    } else if (*nextBest == -1) {
        return 0;
    }

    count = 0;
    index = 0;
    while (1) {
        sprintf(key, str_PCTs_PCT04d_802fdda0, param_1, index);
        scan = msgSearch(key);
        if (scan[0] != '<' || scan[1] != '!') {
            return count;
        }

        scan += 2;
        field = 0;
        pos = 0;
        done = 0;
        while (!done) {
            if (_ismbblead(*scan) != 0) {
                if (field == 0) {
                    label[pos] = scan[0];
                    label[pos + 1] = scan[1];
                    pos += 2;
                } else {
                    pos += 2;
                }
                scan += 2;
            } else {
                if (*scan == '>') {
                    done = 1;
                } else if (*scan != ',') {
                    if (field == 0) {
                        label[pos] = *scan;
                        pos++;
                    } else {
                        pos++;
                    }
                    scan++;
                    continue;
                }
                scan++;
                if (field == 0) {
                    label[pos] = 0;
                }
                pos = 0;
                field++;
            }
        }

        if (strcmp(label, "enddata") == 0) {
            return count;
        }

        if (strcmp(label, "") != 0) {
            labelNo = search_evt_no(label);
            if (labelNo != -1) {
                if ((param_2 & 1) == 0) {
                    if (labelNo <= maxLabel && *nextBest <= labelNo) {
                        if (*nextBest < labelNo) {
                            count = 0;
                            *nextBest = labelNo;
                        }
                        count++;
                    }
                } else if (labelNo == *nextBest) {
                    *(s16*)((s32)_udt.table + count * 6 + 2) = labelNo;
                    *(s16*)((s32)_udt.table + count * 6 + 4) = index;
                    count++;
                }
            }
        }
        index++;
    }
}

int uranaisi_data_make_starpiece(s32 param_1, u32 param_2) {
    extern int strcmp(const char* s1, const char* s2);
    extern char* msgSearch(char* key);
    extern s32 _ismbblead(s32 c);
    extern int search_evt_no(void* pEvt);
    extern void* uranai_table_starpiece[];
    char label[64];
    char key[76];
    char* scan;
    char* body;
    s16 bodyLen;
    s32 maxLabel = evtGetValue(NULL, GSW(0));
    s32 count = 0;
    s32 index = 0;
    s32 offset = 0;
    s32 field;
    s32 pos;
    s32 done;
    s32 labelNo;
    s32 found;
    s32 tableIndex;
    char saved;
    void** table;

    while (1) {
        sprintf(key, str_PCTs_PCT04d_802fdda0, param_1, index);
        scan = msgSearch(key);
        if (scan[0] != '<' || scan[1] != '!') {
            return count;
        }

        scan += 2;
        field = 0;
        pos = 0;
        done = 0;
        while (!done) {
            if (_ismbblead(*scan) != 0) {
                if (field == 0) {
                    label[pos] = scan[0];
                    label[pos + 1] = scan[1];
                    pos += 2;
                } else {
                    pos += 2;
                }
                scan += 2;
            } else {
                if (*scan == '>') {
                    done = 1;
                } else if (*scan != ',') {
                    if (field == 0) {
                        label[pos] = *scan;
                        pos++;
                    } else {
                        pos++;
                    }
                    scan++;
                    continue;
                }
                scan++;
                if (field == 0) {
                    label[pos] = 0;
                    body = scan;
                } else {
                    bodyLen = pos;
                }
                pos = 0;
                field++;
            }
        }

        if (strcmp(label, "enddata") == 0) {
            return count;
        }

        if (strcmp(label, "") != 0) {
            labelNo = search_evt_no(label);
            if (labelNo != -1 && labelNo <= maxLabel) {
                saved = body[bodyLen];
                body[bodyLen] = 0;
                found = -1;
                table = uranai_table_starpiece;
                tableIndex = 0;
                while (table[0] != 0) {
                    if (strcmp(body, table[0]) == 0) {
                        found = ((s32*)uranai_table_starpiece)[tableIndex * 2 + 1];
                        break;
                    }
                    table += 2;
                    tableIndex++;
                }
                body[bodyLen] = saved;

                if (found != -1 && evtGetValue(NULL, found) == 0) {
                    if (param_2 & 1) {
                        *(s16*)((s32)_udt.table + offset + 2) = labelNo;
                        *(s16*)((s32)_udt.table + offset + 4) = index;
                    }
                    offset += 6;
                    count++;
                }
            }
        }
        index++;
    }
}

int uranaisi_data_make_supercoin(s32 param_1, u32 param_2) {
    extern int strcmp(const char* s1, const char* s2);
    extern char* msgSearch(char* key);
    extern s32 _ismbblead(s32 c);
    extern int search_evt_no(void* pEvt);
    extern void* uranai_table_supercoin[];
    char label[64];
    char key[76];
    char* scan;
    char* body;
    s16 bodyLen;
    s32 maxLabel = evtGetValue(NULL, GSW(0));
    s32 count = 0;
    s32 index = 0;
    s32 offset = 0;
    s32 field;
    s32 pos;
    s32 done;
    s32 labelNo;
    s32 found;
    s32 tableIndex;
    char saved;
    void** table;

    while (1) {
        sprintf(key, str_PCTs_PCT04d_802fdda0, param_1, index);
        scan = msgSearch(key);
        if (scan[0] != '<' || scan[1] != '!') {
            return count;
        }

        scan += 2;
        field = 0;
        pos = 0;
        done = 0;
        while (!done) {
            if (_ismbblead(*scan) != 0) {
                if (field == 0) {
                    label[pos] = scan[0];
                    label[pos + 1] = scan[1];
                    pos += 2;
                } else {
                    pos += 2;
                }
                scan += 2;
            } else {
                if (*scan == '>') {
                    done = 1;
                } else if (*scan != ',') {
                    if (field == 0) {
                        label[pos] = *scan;
                        pos++;
                    } else {
                        pos++;
                    }
                    scan++;
                    continue;
                }
                scan++;
                if (field == 0) {
                    label[pos] = 0;
                    body = scan;
                } else {
                    bodyLen = pos;
                }
                pos = 0;
                field++;
            }
        }

        if (strcmp(label, "enddata") == 0) {
            return count;
        }

        if (strcmp(label, "") != 0) {
            labelNo = search_evt_no(label);
            if (labelNo != -1 && labelNo <= maxLabel) {
                saved = body[bodyLen];
                body[bodyLen] = 0;
                found = -1;
                table = uranai_table_supercoin;
                tableIndex = 0;
                while (table[0] != 0) {
                    if (strcmp(body, table[0]) == 0) {
                        found = ((s32*)uranai_table_supercoin)[tableIndex * 2 + 1];
                        break;
                    }
                    table += 2;
                    tableIndex++;
                }
                body[bodyLen] = saved;

                if (found != -1 && evtGetValue(NULL, found) == 0) {
                    if (param_2 & 1) {
                        *(s16*)((s32)_udt.table + offset + 2) = labelNo;
                        *(s16*)((s32)_udt.table + offset + 4) = index;
                    }
                    offset += 6;
                    count++;
                }
            }
        }
        index++;
    }
}

s32 uranaisi_ryokin_rtn(s32 type) {
    if (type != 0) {
        return uranaisi_ryokin_tbl[type];
    }
    return *(s16*)((s32)pouchGetPtr() + 0x8A) * 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(uranaisi_ryokin) {
    s32* args = event->args;
    s32 type = evtGetValue(event, args[0]);
    s32 value;

    if (type != 0) {
        value = uranaisi_ryokin_tbl[type];
    } else {
        value = *(s16*)((s32)pouchGetPtr() + 0x8A) * 2;
    }
    evtSetValue(event, args[1], value);
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on
