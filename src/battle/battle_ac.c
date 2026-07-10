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
extern const char str_SFX_AC_POWER_GAUGE1_802ee034[20];
extern f32 float_0_80422188;
extern f32 float_900_804221a8;

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
    *(s32*)((s32)work + 0x1F18) = psndSFXOn(str_SFX_AC_POWER_GAUGE1_802ee034);
    *(f32*)((s32)work + 0x1F1C) = float_0_80422188;
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

        dispCallback = *(void**)((s32)work + 0x1CA8);
        if (dispCallback != 0) {
            dispEntry(8, 1, dispCallback, float_900_804221a8, work);
        }
    }
}

u8 BattleAcDrawGauge(s64 ratioFilled, s32 x, s32 y, s32 innerBarWidth, s32 param_5, s32 bar1EndPercent, s32 bar2EndPercent, s32 bar3EndPercent, s32 flags_) {
    return 0;
}


void BattleActionCommandCheckDefence(void* unit, s32 value) {
    extern u8 N_normal_guard_frames[];
    extern u8 N_superguard_frames[];
    u8 normalFrames[7];
    u8 superFrames[7];
    void* work;
    void* padWork;
    s32 guarded;
    s32 difficulty;
    s32 superCount;
    s32 normalCount;
    s32 count;
    s32 i;

    normalFrames[0] = N_normal_guard_frames[0];
    normalFrames[1] = N_normal_guard_frames[1];
    normalFrames[2] = N_normal_guard_frames[2];
    normalFrames[3] = N_normal_guard_frames[3];
    normalFrames[4] = N_normal_guard_frames[4];
    normalFrames[5] = N_normal_guard_frames[5];
    normalFrames[6] = N_normal_guard_frames[6];
    superFrames[0] = N_superguard_frames[0];
    superFrames[1] = N_superguard_frames[1];
    superFrames[2] = N_superguard_frames[2];
    superFrames[3] = N_superguard_frames[3];
    superFrames[4] = N_superguard_frames[4];
    superFrames[5] = N_superguard_frames[5];
    superFrames[6] = N_superguard_frames[6];

    work = _battleWorkPointer;
    padWork = (void*)((s32)work + 0x1D1C);
    guarded = 0;
    *(s32*)((s32)work + 0x1CB0) = 0;

    difficulty = *(u8*)((s32)unit + 0x306) + 3 - *(u8*)((s32)unit + 0x305);
    if (difficulty < 0) {
        difficulty = 0;
    }
    if (difficulty > 6) {
        difficulty = 6;
    }

    superCount = superFrames[difficulty];
    normalCount = normalFrames[difficulty];

    if (*(u8*)(value + 0x13) != 0 && *(s16*)((s32)unit + 0x10C) > 0) {
        if (*(s16*)((s32)work + 0x1D18) > 0) {
            *(s16*)((s32)work + 0x1D18) = *(s16*)((s32)work + 0x1D18) - 1;
            guarded = 1;
            *(s32*)((s32)work + 0x1CB0) = 5;
        } else if (*(u8*)((s32)unit + 0x307) != 0) {
            guarded = 1;
            *(s32*)((s32)work + 0x1CB0) = 5;
        } else {
            for (i = 0; i < superCount; i++) {
                if (BattleACPadCheckRecordTrigger(i, 0x200) != 0) {
                    break;
                }
            }
            if (i < superCount) {
                count = 0;
                for (i = 0; i < 15; i++) {
                    if (BattleACPadCheckRecordTrigger(i, 0x100) != 0) {
                        count++;
                    }
                    if (BattleACPadCheckRecordTrigger(i, 0x200) != 0) {
                        count++;
                    }
                }
                if (count < 2) {
                    guarded = 1;
                    *(s32*)((s32)work + 0x1CB0) = 5;
                }
            }
        }
    }

    if (guarded == 0) {
        if (*(s16*)((s32)work + 0x1D18) > 0) {
            *(s16*)((s32)work + 0x1D18) = *(s16*)((s32)work + 0x1D18) - 1;
            *(s32*)((s32)work + 0x1CB0) = 4;
        } else {
            count = 0;
            for (i = 0; i < 15; i++) {
                if (BattleACPadCheckRecordTrigger(i, 0x100) != 0) {
                    count++;
                }
                if (BattleACPadCheckRecordTrigger(i, 0x200) != 0) {
                    count++;
                }
            }
            if (count >= 2) {
                *(s32*)((s32)work + 0x1CB0) = 1;
            } else {
                for (i = 0; i < normalCount; i++) {
                    if (BattleACPadCheckRecordTrigger(i, 0x100) != 0) {
                        break;
                    }
                }
                if (i < normalCount) {
                    *(s32*)((s32)work + 0x1CB0) = 4;
                } else if (count > 0) {
                    *(s32*)((s32)work + 0x1CB0) = 1;
                } else {
                    *(s32*)((s32)work + 0x1CB0) = 2;
                }
            }
        }
    }

    BtlPad_WorkInit(padWork);
}

s32 BattleACGetButtonIcon(int button, int pressed) {
    s32 icon;

    icon = 0;
    switch (button) {
        case 0x100:
            if (pressed != 0) {
                icon = 0x6D;
            } else {
                icon = 0x6C;
            }
            break;
        case 0x200:
            if (pressed != 0) {
                icon = 0x6F;
            } else {
                icon = 0x6E;
            }
            break;
        case 0x400:
            if (pressed != 0) {
                icon = 0x71;
            } else {
                icon = 0x70;
            }
            break;
        case 0x800:
            if (pressed != 0) {
                icon = 0x73;
            } else {
                icon = 0x72;
            }
            break;
        case 0x20:
            if (pressed != 0) {
                icon = 0x89;
            } else {
                icon = 0x88;
            }
            break;
        case 0x40:
            if (pressed != 0) {
                icon = 0x87;
            } else {
                icon = 0x86;
            }
            break;
        case 0x10:
            if (pressed != 0) {
                icon = 0x8B;
            } else {
                icon = 0x8A;
            }
            break;
        case 0x10000:
            if (pressed != 0) {
                icon = 0x84;
            } else {
                icon = 0x81;
            }
            break;
        case 0x20000:
            if (pressed != 0) {
                icon = 0x7E;
            } else {
                icon = 0x81;
            }
            break;
        case 0x40000:
            if (pressed != 0) {
                icon = 0x80;
            } else {
                icon = 0x81;
            }
            break;
        case 0x80000:
            if (pressed != 0) {
                icon = 0x82;
            } else {
                icon = 0x81;
            }
            break;
        case 0x90000:
            if (pressed != 0) {
                icon = 0x85;
            } else {
                icon = 0x81;
            }
            break;
        case 0xA0000:
            if (pressed != 0) {
                icon = 0x7F;
            } else {
                icon = 0x81;
            }
            break;
        case 0x50000:
            if (pressed != 0) {
                icon = 0x83;
            } else {
                icon = 0x81;
            }
            break;
        case 0x60000:
            if (pressed != 0) {
                icon = 0x7D;
            } else {
                icon = 0x81;
            }
            break;
    }
    return icon;
}

void BattleActionCommandSetup(void* battleWork, s32 param, void* unit, s32 rawArg, s32 value) {
    typedef struct ActionCommandEntry {
        s32 id;
        void* mainCallback;
        void* resultCallback;
        void* dispCallback;
        void* stopCallback;
    } ActionCommandEntry;
    extern ActionCommandEntry ActionCommandList[];
    ActionCommandEntry* entry;
    s32 zero;
    s32 id;

    *(void**)((s32)battleWork + 0x1C90) = unit;
    entry = ActionCommandList;
    zero = 0;
    *(s32*)((s32)battleWork + 0x1C94) = rawArg;
    *(s32*)((s32)battleWork + 0x1C9C) = zero;
    *(s32*)((s32)battleWork + 0x1CB4) = zero;
    *(s32*)((s32)battleWork + 0x1CB8) = 1;
    *(s32*)((s32)battleWork + 0x1CA0) = zero;
    *(s32*)((s32)battleWork + 0x1CA8) = zero;
    *(s32*)((s32)battleWork + 0x1CA4) = zero;
    *(s32*)((s32)battleWork + 0x1CAC) = zero;
    *(s32*)((s32)battleWork + 0x1C98) = value;
    *(s32*)((s32)battleWork + 0x1CC0) = zero;

    while ((id = entry->id) != 0) {
        if (id == param) {
            *(void**)((s32)battleWork + 0x1CA0) = entry->mainCallback;
            unit = *(void**)((s32)battleWork + 0x1C90);
            if ((*(u32*)((s32)unit + 0x27C) & 0x10) == 0) {
                *(void**)((s32)battleWork + 0x1CA8) = entry->dispCallback;
            }
            *(void**)((s32)battleWork + 0x1CA4) = entry->resultCallback;
            *(void**)((s32)battleWork + 0x1CAC) = entry->stopCallback;
            return;
        }
        entry++;
    }
}

void BattleActionCommandDeclareACResult(void* battleWork, void* unit, s32 result) {
    extern void BattleAudience_Case_ActionCommandBad(void*);
    extern void BattleAudience_Case_ActionCommandGood(void*);
    extern f64 double_to_int_802ee048;
    extern f32 float_0p25_804221a4;
    extern f32 float_0p5_804221a0;
    void* base;

    base = (void*)((s32)battleWork + 0x20000);
    if (result == -1) {
        goto bad;
    }
    goto good;

bad:
    BattleAudience_Case_ActionCommandBad(unit);
    return;

good:
    *(void**)((s32)base - 0x7004) = unit;
    *(f32*)((s32)base - 0x7000) = (f32)result * float_0p25_804221a4 + float_0p5_804221a0;
    *(u8*)((s32)base - 0x6FFB) = *(u8*)((s32)unit + 0x19);
    *(u8*)((s32)base - 0x6FFA) = *(u8*)((s32)unit + 0x1A);
    BattleAudience_Case_ActionCommandGood(unit);
}

void BattleAcGaugeSeUpdate(f32 value) {
    extern f32 float_0_80422188;
    extern f32 float_100_8042218c;
    extern f32 float_0p01_80422190;
    extern f32 float_4092_80422194;
    extern void psndSFX_pit(u32 index, u32 pitch);
    void* work = _battleWorkPointer;
    u32 sfx;

    if (value < float_0_80422188) {
        value = float_0_80422188;
    }
    if (value > float_100_8042218c) {
        value = float_100_8042218c;
    }
    *(f32*)((s32)work + 0x1F1C) = value;
    sfx = *(u32*)((s32)work + 0x1F18);
    if (sfx != 0xFFFFFFFF) {
        psndSFX_pit(sfx, (s32)(float_4092_80422194 * *(f32*)((s32)work + 0x1F1C) * float_0p01_80422190));
    }
}

