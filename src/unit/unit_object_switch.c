#include "unit/unit_object_switch.h"

extern void* _battleWorkPointer;

extern s32 evtGetValue(void* event, s32 value);
extern s32 BattleTransID(void* event, s32 id);
extern void* BattleGetUnitPtr(void* battleWork, s32 id);
extern void* BattleGetObjectPtr(s32 id);

static s32 _data[] = {
    -5, 0, 15, 0, -15, 0, 14, 0, -13, 0, 12, 0, -11, 0, 10, 0, -9, 0,
    8, 0, -7, 0, 6, 0, -6, 0, 5, 0, -4, 0, 2, 0, -2, 0, 0
};

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 _rumble_switch(void* event, s32 reset) {
    void* unit;
    void* object;
    s32* args;
    s32 counter;
    s32 value;

    args = *(s32**)((s32)event + 0x18);
    value = evtGetValue(event, args[0]);
    unit = BattleGetUnitPtr(_battleWorkPointer, BattleTransID(event, value));
    if (unit == NULL) {
        return 2;
    }

    object = BattleGetObjectPtr(*(s32*)((s32)unit + 0xB30));
    if (reset != 0) {
        *(s32*)((s32)event + 0x78) = 0;
    }

    counter = *(s32*)((s32)event + 0x78);
    *(f32*)((s32)object + 8) += _data[counter];
    *(s32*)((s32)event + 0x78) += 1;
    if (*(u32*)((s32)event + 0x78) >= 0x22) {
        return 2;
    }
    return 0;
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset
