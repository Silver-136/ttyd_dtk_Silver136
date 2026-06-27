#include "battle/battle_unit.h"
#include "battle/battle_status_effect.h"

s8 _st_pri_data[0x19];
extern void* _battleWorkPointer;

void* memset(void* dest, int value, unsigned long size);
void effDelete(void* effect);
void effSoftDelete(void* effect);
void iconDelete(void* icon);
void* BattleGetUnitPtr(void* battleWork, s32 unitId);
BattleWorkUnitPart* BtlUnit_GetPartsPtr(BattleWorkUnit* unit, s32 index);
s32 BtlUnit_GetBodyPartsId(BattleWorkUnit* unit);
void BtlUnit_GetPartsWorldPos(BattleWorkUnitPart* part, f32* x, f32* y, f32* z);
s32 BtlUnit_GetWidth(BattleWorkUnit* unit);
s32 BtlUnit_GetHeight(BattleWorkUnit* unit);
void* effIceEntry(f32 x, f32 y, f32 z, f32 scale, s32 type, s32 unk);
void* effFireEntry(f32 x, f32 y, f32 z, f32 scale, s32 type, s32 unk);
void* effSparkEntry(f32 x, f32 y, f32 z, f32 scale, s32 type, s32 unk);
void* effSleepEntry(f32 x, f32 y, f32 z, f32 scale, s32 type, s32 unk, f32 time);
int sprintf(char* str, const char* format, ...);
void iconEntry(char* name, u16 flags);
void* iconNameToPtr(char* name);
void iconSetPos(void* icon, f32 x, f32 y, f32 z);
s32 FontGetMessageWidthLine(char* msg, u16* lines);
void windowDispGX_Waku_col(s32 texture, void* color, f32 x, f32 y, f32 width, f32 height, f32 curve);
void FontDrawStart(void);
void FontDrawMessage(s32 x, s32 y, char* msg);

extern f32 float_0_80424710;
extern f32 float_20_80424714;
extern f32 float_neg117_80424718;
extern f32 float_10_8042471c;
extern f32 float_neg120_80424720;
extern f32 float_1_80424728;
extern f32 float_5_8042472c;
extern f32 float_neg1000_80424738;
extern f32 float_36_8042473c;
extern f32 float_neg1_80424740;
extern f32 float_60_80424744;
extern f32 float_0p5_80424748;
extern f32 float_0p95_8042474c;
extern u32 dat_8042470c;

const char str_biK_PCT02x_802f9a60[] = "biK_%02x";

void BattleStatusEffectInit(BattleWorkUnit* unit) {
    ;
}

s32 BattleStatusChangeAnnouceMain_Unit(BattleWorkUnit* unit) {
    return 0;
}

void BSE_TurnFirstProcessEffectEntry(BattleWorkUnit* unit, s32 value) {
    *(s16*)((s32)unit + 0x338) = 1;
    *(s16*)((s32)unit + 0x33A) = value;
}

s32 _get_pri(s32 status) {
    s32 target;
    s32 pri;
    s32 value;
    s8* data;

    target = (s8)status;
    pri = 0x18;
    data = _st_pri_data;

    while (1) {
        value = *data;

        if (value == -1) {
            return -1;
        }

        if (value == target) {
            return pri;
        }

        pri--;
        data++;
    }
}

void BSE_BiribiriDelete(BattleWorkUnit* unit) {
    void* effect;

    effect = *(void**)((s32)unit + 0x330);
    if (effect != 0) {
        effDelete(effect);
        *(void**)((s32)unit + 0x330) = 0;
    }
}

void BSE_FireDelete(BattleWorkUnit* unit) {
    void* effect;

    effect = *(void**)((s32)unit + 0x328);
    if (effect != 0) {
        effDelete(effect);
        *(void**)((s32)unit + 0x328) = 0;
    }
}

void BSE_FreezeDelete(BattleWorkUnit* unit) {
    void* effect;

    effect = *(void**)((s32)unit + 0x32C);
    if (effect != 0) {
        effSoftDelete(effect);
        *(void**)((s32)unit + 0x32C) = 0;
    }
}

void BSE_SleepDelete(BattleWorkUnit* unit) {
    void* effect;

    effect = *(void**)((s32)unit + 0x324);
    if (effect != 0) {
        effDelete(effect);
        *(void**)((s32)unit + 0x324) = 0;
    }
}

void BSE_KagegakureDelete(BattleWorkUnit* unit) {
    void* effect;

    effect = *(void**)((s32)unit + 0x334);
    if (effect != 0) {
        iconDelete((void*)((s32)effect + 0x18));
        *(void**)((s32)unit + 0x334) = 0;
    }
}

void BattleStatusChangeInfoWorkInit(BattleWorkUnit* unit) {
    s32 offset;
    s32 count;
    u8* base;
    s8 minusOne;
    u8 zeroA;
    u8 zeroB;
    u8 zeroC;
    s32 zeroWord;
    u8* entry;

    offset = 0;
    count = 6;
    base = (u8*)((s32)unit + 0xAE8);
    minusOne = -1;
    zeroA = 0;
    zeroB = 0;
    zeroC = 0;
    zeroWord = 0;

    while (count != 0) {
        entry = base + offset;
        offset += 0xC;

        *(s8*)(entry + 0x0) = minusOne;
        *(u8*)(entry + 0x1) = zeroA;
        *(u8*)(entry + 0x2) = zeroB;
        *(u8*)(entry + 0x3) = zeroC;
        *(s32*)(entry + 0x4) = zeroWord;

        count--;
    }
}

void BattleStatusEffectDelete(BattleWorkUnit* unit) {
    BSE_SleepDelete(unit);
    BSE_BiribiriDelete(unit);
    BSE_FireDelete(unit);
    BSE_FreezeDelete(unit);
    BSE_KagegakureDelete(unit);
}

void BattleStatusChangeMsgWorkInit(void) {
    u8* work;
    s32 i;

    work = (u8*)((s32)_battleWorkPointer + 0x18DA0);
    memset(work, 0, 0x258);

    for (i = 0; i < 0x1C; i++) {
        *(s8*)(work + i * 0xC) = -1;
    }
}

void BattleStatusChangeMsgSetAnnouce(void* info, s32 index, s32 value) {
    u8* entry;
    s32 kind;

    entry = (u8*)((s32)_battleWorkPointer + 0x18DA0 + index * 0xC);

    *(s32*)(entry + 0x4) = *(s32*)((s32)info + 0x0);
    *(u8*)(entry + 0x0) = index;
    *(u8*)(entry + 0x2) = 0;
    *(s32*)(entry + 0x8) = value;

    kind = *(s32*)((s32)info + 0x8);

    if (kind == 0xDE) {
        *(u8*)(entry + 0x1) |= 1;
        return;
    }

    if (kind >= 0xE0 && kind < 0xE7) {
        if (*(s8*)((s32)info + 0xC) == 0) {
            *(u8*)(entry + 0x1) |= 2;
            return;
        }
    }

    *(u8*)(entry + 0x1) |= 4;
}


int BattleStatusChangeMsgMain(void* battleWork) {
    return 0;
}


s32 BSE_TurnFirstProcessEffectMain(void* unit) {
    return 0;
}


u8 BattleStatusChangeInfoSetAnnouce(void* unit, int param_2, u8 param_3, u32 param_4) {
    return 0;
}


void BSE_Sleep(BattleWorkUnit* unit) {
    void* effect;
    f32 x;
    f32 y;
    f32 z;
    f32 addX;
    f32 addY;
    void* work;
    BattleWorkUnitPart* part;
    s32 active;

    active = 0;
    if (BtlUnit_CheckStatus(unit, STATUS_SLEEP) != 0) {
        active = 1;
    }
    if (active != 0) {
        if (*(void**)((s32)unit + 0x324) == 0) {
            *(void**)((s32)unit + 0x324) = effSleepEntry(float_neg1000_80424738, float_neg1000_80424738, float_neg1000_80424738, float_1_80424728, 0, 0, float_60_80424744);
        }
        effect = *(void**)((s32)unit + 0x324);
        work = *(void**)((s32)effect + 0xC);
        part = BtlUnit_GetPartsPtr(unit, BtlUnit_GetBodyPartsId(unit));
        BtlUnit_GetPartsWorldPos(part, &x, &y, &z);
        addX = float_0p5_80424748 * (unit->sizeMultiplier * (f32)BtlUnit_GetWidth(unit));
        addY = float_0p95_8042474c * (unit->sizeMultiplier * (f32)BtlUnit_GetHeight(unit));
        if ((unit->attributes & 2) != 0) {
            addY = float_0_80424710;
        }
        x += addX;
        y += addY;
        *(f32*)((s32)work + 4) = x;
        *(f32*)((s32)work + 8) = y;
        *(f32*)((s32)work + 0xC) = z;
    } else {
        BSE_SleepDelete(unit);
    }
}


void BattleStatusChangeMsgAdjust(void* battleWork) {
    u8* work;
    u8* entry;
    BattleWorkUnit* unit;
    u8* info;
    s32 i;
    s32 unitId;
    s32 j;
    s32 found;
    s32 idx;
    s32 mask;

    work = (u8*)((s32)battleWork + 0x18DA0);
    for (i = 0; (s8)i < 0x1C; i++) {
        idx = (s8)i;
        entry = work + idx * 0xC;
        if (*(s8*)entry != -1) {
            entry[1] = 0;
            for (unitId = 0; unitId < 0x40; unitId++) {
                unit = BattleGetUnitPtr(battleWork, unitId);
                if (unit != 0 && BtlUnit_CheckStatus(unit, STATUS_INSTAKILL) == 0) {
                    info = (u8*)((s32)unit + 0xAE8);
                    found = 0;
                    for (j = 0; j < 6; j++) {
                        if ((s8)info[j * 0xC] == idx) {
                            found = 1;
                            break;
                        }
                    }
                    if (found != 0) {
                        BattleStatusChangeMsgSetAnnouce(unit, idx, *(s32*)(info + j * 0xC + 8));
                    }
                }
            }
            if (entry[1] == 0) {
                *(s32*)(entry + 4) = -1;
                *(s8*)entry = -1;
            } else {
                mask = 1 << idx;
                if (((*(u32*)(work + 0x254) & mask) | (*(u32*)(work + 0x250) & (mask >> 31))) != 0) {
                    *(s32*)(entry + 4) = -1;
                    *(s8*)entry = -1;
                }
            }
        }
    }
}


void BSE_Freeze(BattleWorkUnit* unit) {
    void* effect;
    f32 x;
    f32 y;
    f32 z;
    f32 scale;
    f32 widthScale;
    f32 heightScale;
    void* work;
    BattleWorkUnitPart* part;
    s32 active;

    active = 0;
    if (BtlUnit_CheckStatus(unit, STATUS_FROZEN) != 0) {
        active = 1;
    }
    if (active != 0) {
        if (*(void**)((s32)unit + 0x32C) == 0) {
            *(void**)((s32)unit + 0x32C) = effIceEntry(float_neg1000_80424738, float_neg1000_80424738, float_neg1000_80424738, float_1_80424728, 0, -1);
        }
        effect = *(void**)((s32)unit + 0x32C);
        work = *(void**)((s32)effect + 0xC);
        part = BtlUnit_GetPartsPtr(unit, BtlUnit_GetBodyPartsId(unit));
        widthScale = (f32)unit->width / float_20_80424714;
        heightScale = (f32)unit->height / float_36_8042473c;
        if (widthScale <= heightScale) {
            scale = widthScale;
        } else {
            scale = heightScale;
        }
        scale *= unit->sizeMultiplier;
        *(f32*)((s32)work + 0x10) = scale;
        BtlUnit_GetPartsWorldPos(part, &x, &y, &z);
        *(f32*)((s32)work + 4) = x;
        *(f32*)((s32)work + 8) = y + float_20_80424714 * scale;
        *(f32*)((s32)work + 0xC) = z + float_10_8042471c;
    } else {
        BSE_FreezeDelete(unit);
    }
}


void BSE_Biribiri(BattleWorkUnit* unit) {
    void* effect;
    f32 x;
    f32 y;
    f32 z;
    f32 addY;
    void* work;
    BattleWorkUnitPart* part;
    s32 active;
    s32 halfWidth;

    active = 0;
    if (BtlUnit_CheckStatus(unit, STATUS_ELECTRIC) != 0) {
        active = 1;
    }
    if (active != 0) {
        if (*(void**)((s32)unit + 0x330) == 0) {
            halfWidth = BtlUnit_GetWidth(unit) / 2;
            *(void**)((s32)unit + 0x330) = effSparkEntry(float_neg1000_80424738, float_neg1000_80424738, float_neg1000_80424738, (f32)halfWidth, 0, 0xA);
        }
        effect = *(void**)((s32)unit + 0x330);
        work = *(void**)((s32)effect + 0xC);
        part = BtlUnit_GetPartsPtr(unit, BtlUnit_GetBodyPartsId(unit));
        BtlUnit_GetPartsWorldPos(part, &x, &y, &z);
        addY = unit->sizeMultiplier * (f32)(BtlUnit_GetHeight(unit) / 2);
        if ((unit->attributes & 2) != 0) {
            addY = float_0_80424710;
        }
        y += addY;
        *(f32*)((s32)work + 4) = x;
        *(f32*)((s32)work + 8) = y;
        *(f32*)((s32)work + 0xC) = z + float_10_8042471c;
    } else {
        BSE_BiribiriDelete(unit);
    }
}


void _st_chg_msg_disp(void) {
    void* battleWork;
    char* msg;
    u16 lines[2];
    u32 color;
    s32 width;
    s32 halfWidth;
    u16 lineCount;
    s32 metrics;
    f32 x;

    battleWork = _battleWorkPointer;
    msg = (char*)((s32)battleWork + 0x18EF0);
    metrics = FontGetMessageWidthLine(msg, lines);
    width = (u16)metrics;
    halfWidth = (metrics >> 16) & 0x7FFF;
    lineCount = lines[0] + 1;
    x = float_0_80424710 - (f32)halfWidth;
    lines[0] = lineCount;
    color = dat_8042470c;
    windowDispGX_Waku_col(0, &color, x - float_20_80424714, float_neg117_80424718 + (f32)(width + 0x28), (f32)((lineCount - 1) * 0x1D), (f32)(lineCount * 0x1D + 1), float_10_8042471c);
    FontDrawStart();
    FontDrawMessage((s32)x, (s32)float_neg120_80424720 + (lineCount - 1) * 0x1D, msg);
}


void BSE_Fire(BattleWorkUnit* unit) {
    void* effect;
    f32 x;
    f32 y;
    f32 z;
    f32 dir;
    f32 scale;
    f32 offset;
    void* work;
    BattleWorkUnitPart* part;
    s32 active;

    active = 0;
    if (BtlUnit_CheckStatus(unit, STATUS_BURN) != 0) {
        active = 1;
    }
    if (active != 0) {
        if (*(void**)((s32)unit + 0x328) == 0) {
            *(void**)((s32)unit + 0x328) = effFireEntry(float_neg1000_80424738, float_neg1000_80424738, float_neg1000_80424738, float_1_80424728, 0, 0);
        }
        effect = *(void**)((s32)unit + 0x328);
        work = *(void**)((s32)effect + 0xC);
        part = BtlUnit_GetPartsPtr(unit, BtlUnit_GetBodyPartsId(unit));
        BtlUnit_GetPartsWorldPos(part, &x, &y, &z);
        scale = unit->sizeMultiplier;
        offset = *(f32*)((s32)unit->data + 0x48);
        if (unit->faceDirection >= 0) {
            dir = float_1_80424728;
        } else {
            dir = float_neg1_80424740;
        }
        x = scale * (offset * dir) + x;
        y = *(f32*)((s32)unit->data + 0x4C) * unit->sizeMultiplier + y;
        z = *(f32*)((s32)unit->data + 0x50) * unit->sizeMultiplier + z;
        *(f32*)((s32)work + 4) = x;
        *(f32*)((s32)work + 8) = y;
        *(f32*)((s32)work + 0xC) = z + float_5_8042472c;
    } else {
        BSE_FireDelete(unit);
    }
}


void BSE_Kagegakure(BattleWorkUnit* unit) {
    s32 enabled;
    char name[16];
    f32 x;
    f32 y;
    f32 z;
    BattleWorkUnitPart* part;

    enabled = 0;
    if ((*(u32*)((s32)_battleWorkPointer + 0xEF8) & 1) != 0) {
        if ((unit->attributes & ATTRIBUTE_VEILED) != 0) {
            enabled = 1;
        }
    }
    if (enabled != 0) {
        if (*(void**)((s32)unit + 0x334) == 0) {
            sprintf(name, str_biK_PCT02x_802f9a60, unit->unitId);
            iconEntry(name, 0xA1);
            *(void**)((s32)unit + 0x334) = iconNameToPtr(name);
        }
        part = BtlUnit_GetPartsPtr(unit, BtlUnit_GetBodyPartsId(unit));
        BtlUnit_GetPartsWorldPos(part, &x, &y, &z);
        iconSetPos((void*)((s32)*(void**)((s32)unit + 0x334) + 0x18), x, y, float_5_8042472c + z);
    } else {
        BSE_KagegakureDelete(unit);
    }
}


s32 BattleStatusChangeAnnouceMain(void* battleWork) {
    return 0;
}


u8 BattleStatusEffectMain(void* unit) {
    return 0;
}
