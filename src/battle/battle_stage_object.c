#include "battle/battle_stage_object.h"

extern void* _battleWorkPointer;
void BattleAudience_Case_HaikeiSet(void);
s32 strcmp(const char*, const char*);

void BattleObjectConfig(void) {
}

void _rate_mix(s8* maxValue, s8* total, s8* current, s8* add) {
    s8 value = *current;

    if (value <= 0) {
        return;
    }
    if (value > *maxValue) {
        *maxValue = value;
    }
    *total += *add;
}

s32 _bgset_audience_bgset_access(void) {
    BattleAudience_Case_HaikeiSet();
    return 2;
}

void* BattleGetObjectPtr(s32 id) {
    s32 offset;
    void* battleWork;
    s32 i;
    void* obj;

    offset = 0;
    battleWork = _battleWorkPointer;
    for (i = 0; i < 0x20; i++) {
        obj = (void*)((s32)battleWork + offset + 0x1715C);
        if (*(s32*)obj == id) {
            return obj;
        }
        offset += 0x7C;
    }
    return 0;
}

void* BattleSearchObjectPtr(const char* name) {
    s32 offset;
    void* battleWork;
    s32 i;

    offset = 0;
    battleWork = _battleWorkPointer;
    for (i = 0; i < 0x20; i++) {
        s32 addr = offset + 0x10000;
        void* obj = (void*)((s32)battleWork + addr + 0x715C);
        if (*(s32*)obj != 0 && strcmp(**(const char***)((s32)obj + 0x64), name) == 0) {
            return obj;
        }
        offset += 0x7C;
    }
    return 0;
}


u8 _object_fall_attack(void) {
    return 0;
}


u8 BattleStageObjectInit(void) {
    return 0;
}


u8 _nozzle_work_effect(void) {
    return 0;
}


void BattleStageObjectMain(void) {
}


u32 _set_mobj_rotate_x(void* param_1, int param_2) {
    return 0;
}


u8 _nozzle_turn(void) {
    return 0;
}


void _combine_nozzle_weapon(int copyBaseOnly, void* tempWeapon, void* baseWeapon) {
}


u8 _bgset_kemuri_effect(void) {
    return 0;
}


s32 _mobj_set_alpha(void* param_1) {
    return 0;
}


s32 _mobj_destroy(int param_1) {
    return 0;
}


int _get_nozzle_attack_index(u8 nozzleIdx) {
    return 0;
}


s32 _fall_frame_wait_unit_ready(s32 param_1, int param_2) {
    return 0;
}


u8 _Nozzle_Change_Check(u32 nozzleIdx, void* turnWeights) {
    return 0;
}


s32 _nozzle_work_attack_set(void) {
    return 0;
}


s32 _fall_frame(void* param_1, int param_2) {
    return 0;
}


s32 _set_mobj_shake(int param_1) {
    return 0;
}


s32 _set_mobj_shake_init(int param_1) {
    return 0;
}


s32 _object_fall_wait(void) {
    return 0;
}


u8 BattleStage_NozzleWorkCheck(int param_1) {
    return 0;
}


s32 _get_bgset_attack_data(int param_1) {
    return 0;
}


u8 BattleStage_DestroyB(void) {
    return 0;
}


u8 BattleStage_DestroyA1(void) {
    return 0;
}


u8 _nozzle_type_init(void) {
    return 0;
}


s32 _fall_frame_delete(int param_1, int param_2) {
    return 0;
}


s32 _nozzle_work_wait(void) {
    return 0;
}


s32 BattleFogEndCheck(void) {
    extern void* _battleWorkPointer;
    extern void effSoftDelete(void* effect);
    void* base = (void*)((s32)_battleWorkPointer + 0x20000);
    s32 timer;
    void* effect;

    timer = *(s32*)((s32)base - 0x7D48);
    if (timer == 0) {
        return 1;
    }

    *(s32*)((s32)base - 0x7D48) = timer - 1;
    if (*(s32*)((s32)base - 0x7D48) > 0) {
        return 0;
    }

    effect = *(void**)((s32)base - 0x7D44);
    if (effect != NULL) {
        effSoftDelete(effect);
        *(void**)((s32)base - 0x7D44) = NULL;
    }
    return 1;
}

s32 _fall_frame_wait(void) {
    return 0;
}


u8 BattleStage_DestroyA2(void) {
    return 0;
}


void BattleStage_IronFrameFallCheck(void) {
    extern s32 _fall_frame_event[];
    extern void* evtEntry(void* script, s32 priority, s32 flags);
    void* base = (void*)((s32)_battleWorkPointer + 0x20000);
    u8 flags = *(u8*)((s32)base - 0x7F08);
    void* event;

    if ((flags & 0x40) == 0 && (flags & 0x20) == 0) {
        flags |= 0x20;
        *(u8*)((s32)base - 0x7F08) = flags;
        event = evtEntry(_fall_frame_event, 0xA, 0);
        *(s32*)((s32)base - 0x7D3C) = *(s32*)((s32)event + 0x15C);
    }
}

void BattleStage_NozzleDirChangeCheck(void) {
    extern void _Nozzle_Change_Check(s32 nozzleIdx, void* work);
    void* base = (void*)((s32)_battleWorkPointer + 0x20000);
    s32 i;

    if (*(void**)((s32)base - 0x7EF0) != NULL) {
        for (i = 0; (u8)i < 3; i++) {
            _Nozzle_Change_Check(i, (void*)((s32)*(void**)((s32)base - 0x7EF0) + ((u8)i << 2)));
        }
    }
}

s32 BattleStage_ObjectFallCheck(void) {
    extern s32 _object_fall_event[];
    extern void* evtEntry(void* script, s32 priority, s32 flags);
    void* base = (void*)((s32)_battleWorkPointer + 0x20000);
    void* event;

    if ((*(u32*)((s32)_battleWorkPointer + 0xEF4) & 0x1000) != 0) {
        return 0;
    }

    event = evtEntry(_object_fall_event, 0xA, 0);
    *(s32*)((s32)base - 0x7D38) = *(s32*)((s32)event + 0x15C);
    return *(s32*)((s32)event + 0x15C);
}

u8 _rate_mix_3(char* param_1, char* param_2, char* param_3, char* param_4, char* param_5, char* param_6) {
    return 0;
}


s32 _nozzle_work_get_weapon_address(void* event) {
    extern s32 evtGetValue(void* event, s32 value);
    extern s32 evtSetValue(void* event, s32 target, s32 value);
    void* evt = event;
    s32* args = *(s32**)((s32)event + 0x18);
    void* base = (void*)((s32)_battleWorkPointer + 0x20000);
    s32 index;

    base = (void*)((s32)base - 0x7F24);
    index = evtGetValue(event, args[0]);
    index <<= 2;
    index = (s32)base + index;
    evtSetValue(evt, args[1], *(s32*)(index + 0x3C));
    return 2;
}


void BattleFogForceStop(void) {
}


void BattleStage_WallCloseCheck(void) {
    extern void* _battleWorkPointer;
    extern void* _close_wall_event;
    extern void* evtEntry(void* script, s32 priority, s32 flags);
    void* base = (void*)((s32)_battleWorkPointer + 0x20000);
    u8 flags = *(u8*)((s32)base - 0x7F08);
    void* event;

    if ((flags & 0x10) == 0) {
        flags |= 0x10;
        *(u8*)((s32)base - 0x7F08) = flags;
        event = evtEntry(&_close_wall_event, 0xA, 0);
        *(s32*)((s32)base - 0x7D40) = *(s32*)((s32)event + 0x15C);
    }
}

f32 _get_nozzle_dir(int nozzleIdx) {
    extern void* _battleWorkPointer;
    extern f32 float_neg45_80423018;
    extern f32 float_neg135_8042301c;
    extern f32 float_neg90_80423020;
    s32 base = (s32)_battleWorkPointer + 0x20000;
    s32 value;

    nozzleIdx = base + nozzleIdx;
    value = *(u8*)(nozzleIdx - 0x7D60);

    switch (value) {
        case 1:
            return float_neg45_80423018;
        case 3:
            return float_neg135_8042301c;
        default:
            return float_neg90_80423020;
    }
}

u8 _mobj_shake_main(void* stageObject) {
    return 0;
}
