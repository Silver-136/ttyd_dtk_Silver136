#include "event/evt_uranaisi.h"
#include "event/evt_cmd.h"

typedef struct UranaisiData {
    void* table;
    s32 next;
    s32 unk8;
    s32 unkC;
    s32 index;
    u8 pad14[4];
    u8 starpieceFlags[0xD];
    u8 supercoinFlags[0x6];
    u8 pad2B[0x38 - 0x2B];
} UranaisiData;

UranaisiData _udt;
s32 uranaisi_ryokin_tbl[3];
extern void* mapalloc_base_ptr;

s32 evtGetValue(EventEntry* event, s32 value);
s32 evtSetValue(EventEntry* event, s32 target, s32 value);
void* memset(void* dst, int value, u32 size);
void _mapFree(void* heap, void* ptr);
void johoya_set(void* flags, s32 id, s32 value);
void* pouchGetPtr(void);

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
    if (_udt.table == 0) {
        return 2;
    }
    _mapFree(mapalloc_base_ptr, _udt.table);
    _udt.table = 0;
    _udt.next = 0;
    _udt.unk8 = 0;
    _udt.unkC = 0;
    _udt.index = 0;
    return 2;
}

USER_FUNC(uranaisi_ryokin) {
    s32* args = event->args;
    s32 type = evtGetValue(event, args[0]);
    evtSetValue(event, args[1], uranaisi_ryokin_rtn(type));
    return 2;
}


int uranaisi_data_make_supercoin(s32 param_1, u32 param_2) {
    return 0;
}


int uranaisi_data_make_starpiece(s32 param_1, u32 param_2) {
    return 0;
}


int uranaisi_data_make_next(s32 param_1, u32 param_2) {
    return 0;
}


s32 uranaisi_data_alloc(int param_1) {
    return 0;
}


s32 uranaisi_supercoin_makelabel(int param_1, s32 param_2, s32 param_3, u32 param_4, u32 param_5) {
    return 0;
}


s32 uranaisi_starpiece_makelabel(int param_1, s32 param_2, s32 param_3, u32 param_4, u32 param_5) {
    return 0;
}


s32 uranaisi_supercoin_check(int param_1, s32 param_2, s32 param_3, u32 param_4, u32 param_5) {
    return 0;
}


s32 uranaisi_starpiece_check(int param_1, s32 param_2, s32 param_3, u32 param_4, u32 param_5) {
    return 0;
}


s32 uranaisi_next_makelabel(int param_1) {
    return 0;
}
