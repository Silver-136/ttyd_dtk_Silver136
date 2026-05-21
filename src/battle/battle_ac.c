#include "battle/battle_ac.h"

typedef struct BattleACPadRecordWork {
    u8 pad[0x1D34];
    s32 mButtonsPressedHistory[4];
} BattleACPadRecordWork;

void BtlPad_WorkInit(void* work);
void BtlPad_WorkUpdate(void* work, s32 flags);
s32 psndSFXOn(const char* name);
void psndSFXOff(s32 id);
void dispEntry(s32 cameraId, s32 priority, void* callback, f32 z, void* param);

s32 BattleActionCommandGetDifficulty(void* work) {
    return *(u8*)((s32)work + 0x1CBD);
}

s32 BattleActionCommandGetDefenceResult(void) {
    return *(s32*)((s32)_battleWorkPointer + 0x1CB0);
}

void BattleActionCommandStart(void* work) {
    *(s32*)((s32)work + 0x1C9C) = 100;
}

s32 BattleACPadCheckRecordTrigger(s32 id, u32 mask) {
    BattleACPadRecordWork* work;

    work = (BattleACPadRecordWork*)_battleWorkPointer;
    return work->mButtonsPressedHistory[id] & mask;
}

void BattleActionCommandResetDefenceResult(void) {
    void* work;

    work = _battleWorkPointer;
    *(s32*)((s32)work + 0x1CB0) = 0;
    BtlPad_WorkInit((void*)((s32)work + 0x1D1C));
}

void BattleActionCommandSetDifficulty(void* work, void* unit, s32 difficulty) {
    s32 value;

    *(u8*)((s32)work + 0x1CBC) = difficulty;

    value = difficulty - *(u8*)((s32)unit + 0x305);
    value += *(u8*)((s32)unit + 0x306);

    if (value < 0) {
        value = 0;
    }

    if (value > 6) {
        value = 6;
    }

    *(u8*)((s32)work + 0x1CBD) = value;
}

void BattleActionCommandStop(void* work) {
    void (*callback)(void*);

    callback = *(void (**)(void*))((s32)work + 0x1CAC);
    if (callback != 0) {
        callback(work);
    }
}

s32 BattleActionCommandResult(void* work) {
    s32 (*callback)(void*);

    callback = *(s32 (**)(void*))((s32)work + 0x1CA4);
    if (callback != 0) {
        return callback(work);
    }

    return *(u32*)((s32)work + 0x1CB8) & 2;
}

void BattleAcGaugeSeInit(void) {
    void* work;

    work = _battleWorkPointer;
    *(s32*)((s32)work + 0x1F18) = psndSFXOn("SFX_AC_POWER_GAUGE1");
    *(f32*)((s32)work + 0x1F1C) = 0.0f;
}

void BattleAcGaugeSeDelete(void) {
    void* work;
    s32 id;

    work = _battleWorkPointer;
    id = *(s32*)((s32)work + 0x1F18);

    if ((u32)(id + 0x10000) != 0xFFFF) {
        psndSFXOff(id);
        *(s32*)((s32)work + 0x1F18) = -1;
    }
}

s32 BattleActionCommandGetPrizeLv(void* work, void* unit, s32 base) {
    s32 difficulty;
    s32 result;

    if (unit != 0) {
        if ((*(u32*)((s32)unit + 0x278) & 0xFF) == 0x1E) {
            return -1;
        }
    }

    difficulty = *(u8*)((s32)work + 0x1CBD);
    result = base + difficulty;
    result -= 1;

    if (result < 0) {
        result = 0;
    }

    if (result < 7) {
        return result;
    }

    return 6;
}

void BattleActionCommandManagerInit(struct BattleWork* work) {
    BtlPad_WorkInit((void*)((s32)work + 0x1D1C));

    *(s32*)((s32)work + 0x1CA0) = 0;
    *(s32*)((s32)work + 0x1CA4) = 0;
    *(s32*)((s32)work + 0x1CA8) = 0;
    *(s32*)((s32)work + 0x1CAC) = 0;
    *(s32*)((s32)work + 0x1F18) = -1;
}

void BattleActionCommandManager(struct BattleWork* work) {
    s32 (*mainCallback)(void*);
    void* dispCallback;

    BtlPad_WorkUpdate((void*)((s32)work + 0x1D1C), 0);

    mainCallback = *(s32 (**)(void*))((s32)work + 0x1CA0);
    if (mainCallback != 0) {
        if (mainCallback(work) == 0) {
            *(s32*)((s32)work + 0x1CA0) = 0;
        }
    }

    dispCallback = *(void**)((s32)work + 0x1CA8);
    if (dispCallback != 0) {
        dispEntry(8, 1, dispCallback, 900.0f, work);
    }
}