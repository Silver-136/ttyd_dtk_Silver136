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
        s32 addr = offset + 0x10000;
        obj = (void*)((s32)battleWork + addr + 0x715C);
        if (*(s32*)obj == id) {
            return obj;
        }
        offset += 0x7C;
    }
    return 0;
}

void* BattleSearchObjectPtr(const char* name) {
    s32 i;
    s32 offset;
    void* battleWork;

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
    return 0;
}


s32 _fall_frame_wait(void) {
    return 0;
}


u8 BattleStage_DestroyA2(void) {
    return 0;
}


u8 BattleStage_IronFrameFallCheck(void) {
    return 0;
}


u8 BattleStage_NozzleDirChangeCheck(void) {
    return 0;
}


s32 BattleStage_ObjectFallCheck(void) {
    return 0;
}


u8 _rate_mix_3(char* param_1, char* param_2, char* param_3, char* param_4, char* param_5, char* param_6) {
    return 0;
}


s32 _nozzle_work_get_weapon_address(int param_1) {
    return 0;
}


void BattleFogForceStop(void) {
}


u8 BattleStage_WallCloseCheck(void) {
    return 0;
}


double _get_nozzle_dir(int nozzleIdx) {
    return 0.0;
}


u8 _mobj_shake_main(void* stageObject) {
    return 0;
}
