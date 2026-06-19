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

USER_FUNC(uranaisi_next_check) {
    evtSetValue(event, event->args[0], _udt.next);
    return 2;
}

s32 uranaisi_ryokin_rtn(s32 type) {
    if (type != 0) {
        return uranaisi_ryokin_tbl[type];
    }
    return *(s16*)((s32)pouchGetPtr() + 0x8A) * 2;
}

USER_FUNC(uranaisi_starpiece_setreadflag) {
    s32 id = *(s16*)((s32)_udt.table + _udt.index * 6 + 4);
    johoya_set(&_udt.starpieceFlags, id, 1);
    return 2;
}

USER_FUNC(uranaisi_supercoin_setreadflag) {
    s32 id = *(s16*)((s32)_udt.table + _udt.index * 6 + 4);
    johoya_set(&_udt.supercoinFlags, id, 1);
    return 2;
}

USER_FUNC(uranaisi_table_clear) {
    memset(&_udt.starpieceFlags, 0, 0xD);
    memset(&_udt.supercoinFlags, 0, 6);
    return 2;
}

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


int uranaisi_data_make_supercoin(s32 param_1, u32 param_2) {
    return 0;
}


int uranaisi_data_make_starpiece(s32 param_1, u32 param_2) {
    return 0;
}


int uranaisi_data_make_next(s32 param_1, u32 param_2) {
    return 0;
}


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
