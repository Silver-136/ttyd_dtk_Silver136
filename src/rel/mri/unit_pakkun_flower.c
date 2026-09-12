#include "rel/mri/unit_pakkun_flower.h"

extern s32 evtGetValue(void* event, s32 arg);
extern s32 BattleTransID(void* event, s32 id);
extern void* BattleGetUnitPtr(void* battleWork, s32 unitId);
extern void* BtlUnit_GetPartsPtr(void* unit, s32 partId);
extern void* _battleWorkPointer;
extern f32 intplGetValue(f32 start, f32 end, s32 type, s32 current, s32 total);
extern s32 evtSetValue(void* event, s32 target, s32 value);

static s32 shadow_scale_return(void* event, s32 reset) {
    s32* args;
    u8* unit;
    u8* part;
    s32 unitId;

    args = *(s32**)((u8*)event + 0x18);
    unitId = BattleTransID(event, evtGetValue(event, args[0]));
    unit = (u8*)BattleGetUnitPtr(_battleWorkPointer, unitId);
    part = (u8*)BtlUnit_GetPartsPtr(unit, 1);

    if (reset != 0) {
        *(s32*)(unit + 0x21C) = 0;
    }

    *(f32*)(part + 0x78) = intplGetValue(
        0.0f,
        (f32)*(s32*)(unit + 0x218),
        0,
        *(s32*)(unit + 0x21C),
        15
    );

    if (*(s32*)(unit + 0x21C) >= 15) {
        return 2;
    }

    *(s32*)(unit + 0x21C) += 1;
    return 0;
}

static s32 shadow_scale_small(void* event, s32 reset) {
    s32* args;
    u8* unit;
    u8* part;
    s32 unitId;

    args = *(s32**)((u8*)event + 0x18);
    unitId = BattleTransID(event, evtGetValue(event, args[0]));
    unit = (u8*)BattleGetUnitPtr(_battleWorkPointer, unitId);
    part = (u8*)BtlUnit_GetPartsPtr(unit, 1);

    if (reset != 0) {
        *(s32*)(unit + 0x21C) = 0;
    }

    *(f32*)(part + 0x78) = intplGetValue(
        (f32)*(s32*)(unit + 0x218),
        0.0f,
        0,
        *(s32*)(unit + 0x21C),
        15
    );

    if (*(s32*)(unit + 0x21C) >= 15) {
        return 2;
    }

    *(s32*)(unit + 0x21C) += 1;
    return 0;
}

static s32 shadow_scale_memory(void* event) {
    s32* args;
    u8* unit;
    u8* part;
    s32 unitId;

    args = *(s32**)((u8*)event + 0x18);
    unitId = BattleTransID(event, evtGetValue(event, args[0]));
    unit = (u8*)BattleGetUnitPtr(_battleWorkPointer, unitId);
    part = (u8*)BtlUnit_GetPartsPtr(unit, 1);

    *(s32*)(unit + 0x218) = (s32)*(f32*)(part + 0x78);
    return 2;
}

static s32 pakkun_ceil_exist(void* event) {
    s32* args;

    args = *(s32**)((u8*)event + 0x18);
    if ((*(u8*)((u8*)_battleWorkPointer + 0x180F8) & 0x20) != 0) {
        evtSetValue(event, args[0], 0);
    } else {
        evtSetValue(event, args[0], 1);
    }
    return 2;
}
