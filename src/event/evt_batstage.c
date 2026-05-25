#include "event/evt_batstage.h"

s32 evt_batstage_set_aud_dark_base(void* event, s32 isFirstCall) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void BattleStageSetAudDarkBase(u8 id, s32 value1, s32 value2);
    s32* args = *(s32**)((s32)event + 0x18);
    u8 id = evtGetValue(event, args[0]);
    s32 value1 = evtGetValue(event, args[1]);
    s32 value2 = evtGetValue(event, args[2]);

    BattleStageSetAudDarkBase(id, value1, value2);
    return 2;
}

s32 evt_batstage_set_aud_dark(void* event, s32 isFirstCall) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void BattleStageSetAudDark(u8 id, s32 value1, s32 value2);
    s32* args = *(s32**)((s32)event + 0x18);
    u8 id = evtGetValue(event, args[0]);
    s32 value1 = evtGetValue(event, args[1]);
    s32 value2 = evtGetValue(event, args[2]);

    BattleStageSetAudDark(id, value1, value2);
    return 2;
}

s32 evt_batstage_set_stg_dark_base(void* event, s32 isFirstCall) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void BattleStageSetStgDarkBase(u8 id, s32 value1, s32 value2);
    s32* args = *(s32**)((s32)event + 0x18);
    u8 id = evtGetValue(event, args[0]);
    s32 value1 = evtGetValue(event, args[1]);
    s32 value2 = evtGetValue(event, args[2]);

    BattleStageSetStgDarkBase(id, value1, value2);
    return 2;
}

s32 evt_batstage_set_stg_dark(void* event, s32 isFirstCall) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void BattleStageSetStgDark(u8 id, s32 value1, s32 value2);
    s32* args = *(s32**)((s32)event + 0x18);
    u8 id = evtGetValue(event, args[0]);
    s32 value1 = evtGetValue(event, args[1]);
    s32 value2 = evtGetValue(event, args[2]);

    BattleStageSetStgDark(id, value1, value2);
    return 2;
}
