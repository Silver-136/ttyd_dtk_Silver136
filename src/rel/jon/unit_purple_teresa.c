#include "rel/jon/unit_purple_teresa.h"
#include "battle/battle.h"
#include "manager/evtmgr_cmd.h"

static s32 teresa_check_teresa(void* event) {
    s32 result;
    s32* args = *(s32**)((u8*)event + 0x18);
    s32 unitId = evtGetValue(event, args[0]);
    BattleWorkUnit* unit;
    s32 kind;

    result = 0;
    unit = BattleGetUnitPtr(_battleWorkPointer, unitId);

    if (unit == 0) {
        evtSetValue(event, args[1], 0);
        return 2;
    }

    kind = *(s32*)((s32)unit + 0x8);
    if (kind == 0x4B) {
        result = 1;
    }
    if (kind == 0x6A) {
        result = 1;
    }

    evtSetValue(event, args[1], result);
    return 2;
}

