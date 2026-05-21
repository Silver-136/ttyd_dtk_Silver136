#include "battle/battle_stage.h"

extern void* wp;
extern void* _battleWorkPointer;

void BattleFree(void* ptr);

void BattleStageOffLightEnemyInCommand(void) {
}

void BattleStageOffLightFriendInCommand(void) {
}

void BattleStageOffLightInCommand(void) {
}

void BattleStageOnLightEnemyInCommand(void) {
}

void BattleStageOnLightFriendInCommand(void) {
}

void BattleStageOnLightInCommand(void) {
}

void* BatSpotGetPtr(void) {
    return wp;
}

void* BattleStageGetPtr(void) {
    return (void*)((s32)_battleWorkPointer + 0x163FC);
}

void BattleStageSetDark(void* dark, s32 value, s32 frames, s32 type) {
    *(u8*)((s32)dark + 0x6) = *(u8*)((s32)dark + 0x4);
    *(u8*)((s32)dark + 0x8) = value;
    *(s32*)((s32)dark + 0xC) = frames;
    *(s32*)((s32)dark + 0x14) = frames;
    *(u8*)((s32)dark + 0x1C) = type;

    if (frames == 0) {
        *(u8*)((s32)dark + 0x4) = value;
    }
}

void BattleStageSetDarkBase(void* dark, s32 value, s32 frames, s32 type) {
    *(u8*)((s32)dark + 0x7) = *(u8*)((s32)dark + 0x5);
    *(u8*)((s32)dark + 0x9) = value;
    *(s32*)((s32)dark + 0x10) = frames;
    *(s32*)((s32)dark + 0x18) = frames;
    *(u8*)((s32)dark + 0x1D) = type;

    if (frames == 0) {
        *(u8*)((s32)dark + 0x5) = value;
    }
}

void BattleStageSetAudDark(s32 value, s32 frames, s32 type) {
    BattleStageSetDark((void*)((s32)_battleWorkPointer + 0x16F00), value, frames, type);
}

void BattleStageSetAudDarkBase(s32 value, s32 frames, s32 type) {
    BattleStageSetDarkBase((void*)((s32)_battleWorkPointer + 0x16F00), value, frames, type);
}

void BattleStageSetStgDark(s32 value, s32 frames, s32 type) {
    BattleStageSetDark((void*)((s32)_battleWorkPointer + 0x16EE0), value, frames, type);
}

void BattleStageSetStgDarkBase(s32 value, s32 frames, s32 type) {
    BattleStageSetDarkBase((void*)((s32)_battleWorkPointer + 0x16EE0), value, frames, type);
}

void BattleStageReturnAudDarkBase(s32 frames, s32 type) {
    void* dark;

    dark = (void*)((s32)_battleWorkPointer + 0x16F00);
    BattleStageSetDark(dark, *(u8*)((s32)dark + 0x5), frames, type);
}

void BattleStageReturnStgDarkBase(s32 frames, s32 type) {
    void* dark;

    dark = (void*)((s32)_battleWorkPointer + 0x16EE0);
    BattleStageSetDark(dark, *(u8*)((s32)dark + 0x5), frames, type);
}

void unk_80137fe4(void) {
    BattleFree(*(void**)((s32)wp + 0x4));
}

void* BattleGetUnitPtr(void* battleWork, s32 unitId);
void dispEntry(s32 cameraId, s32 priority, void* callback, f32 z, void* param);

void screen_capture(void);
void BattleStageDispDark(void);
void BattleStageDispLight(void);

void DarkByPoint(s32 alpha, f32 x, f32 y, f32 z) {
    volatile f32* fifoF;
    volatile u8* fifoB;

    fifoF = (volatile f32*)0xCC008000;
    fifoB = (volatile u8*)0xCC008000;

    *fifoF = x;
    *fifoF = y;
    *fifoF = z;
    *fifoB = 0;
    *fifoB = 0;
    *fifoB = 0;
    *fifoB = alpha;
}

s32 screen_evt_func(void) {
    dispEntry(8, 0, screen_capture, 1000.0f, 0);
    return 0;
}

void BattleStageDisp(void) {
    void* battleWork;

    battleWork = _battleWorkPointer;

    BattleGetUnitPtr(battleWork, *(s32*)((s32)battleWork + 0x420));

    dispEntry(4, 2, BattleStageDispDark, 5000.0f, 0);
    dispEntry(4, 2, BattleStageDispLight, 6000.0f, 0);
}

s32 strcmp(const char* s1, const char* s2);

void BattleStageLightSetWidth(s32 index, s32 width, s32 frames, s32 type) {
    void* workHi;
    void* stage;
    void* light;
    s32 offset;

    offset = index * 0xE8;
    workHi = (void*)((s32)_battleWorkPointer + 0x10000);
    stage = (void*)((s32)workHi + 0x63FC);
    light = (void*)((s32)stage + offset + 4);

    *(f32*)((s32)light + 0x40) = *(f32*)((s32)light + 0x3C);
    *(f32*)((s32)light + 0x44) = width;
    *(s32*)((s32)light + 0x48) = frames;
    *(s32*)((s32)light + 0x4C) = frames;
    *(u8*)((s32)light + 0x50) = type;

    if (frames == 0) {
        *(f32*)((s32)light + 0x3C) = width;
    }
}

s32 BattleStageGetLightNumberByName(const char* name) {
    s32 offset;
    s32 i;
    void* stage;
    void* light;

    i = 0;
    offset = 0;
    stage = (void*)((s32)_battleWorkPointer + 0x163FC);

    while (i < 0xC) {
        light = (void*)((s32)stage + offset + 4);

        if ((*(u32*)light & 1) != 0) {
            if (strcmp((const char*)((s32)light + 4), name) == 0) {
                return i;
            }
        }

        i++;
        offset += 0xE8;
    }

    return -1;
}