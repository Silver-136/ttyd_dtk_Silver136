#include "battle/battle.h"
#include "manager/evtmgr_cmd.h"
#include <dolphin/mtx.h>
#include "battle/battle_stage.h"

void BattleStageReturnAudDarkBase(s32 a, s32 b);
void BattleStageSetAudDarkBase(u8 a, s32 b, s32 c);
void BattleStageSetAudDark(u8 a, s32 b, s32 c);
void BattleStageReturnStgDarkBase(s32 a, s32 b);
void BattleStageSetStgDarkBase(u8 a, s32 b, s32 c);
void BattleStageSetStgDark(u8 a, s32 b, s32 c);

USER_FUNC(evt_batstage_set_stg_dark) {
    EvtWord* args = event->args;
    u8 a = evtGetValue(event, args[0]);
    s32 time = evtGetValue(event, args[1]);
    s32 mode = evtGetValue(event, args[2]);
    BattleStageSetStgDark(a, time, mode);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_batstage_set_stg_dark_base) {
    EvtWord* args = event->args;
    u8 a = evtGetValue(event, args[0]);
    s32 time = evtGetValue(event, args[1]);
    s32 mode = evtGetValue(event, args[2]);
    BattleStageSetStgDarkBase(a, time, mode);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_batstage_return_stg_dark_base) {
    EvtWord* args = event->args;
    s32 time = evtGetValue(event, args[0]);
    s32 mode = evtGetValue(event, args[1]);
    BattleStageReturnStgDarkBase(time, mode);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_batstage_set_aud_dark) {
    EvtWord* args = event->args;
    u8 a = evtGetValue(event, args[0]);
    s32 time = evtGetValue(event, args[1]);
    s32 mode = evtGetValue(event, args[2]);
    BattleStageSetAudDark(a, time, mode);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_batstage_set_aud_dark_base) {
    EvtWord* args = event->args;
    u8 a = evtGetValue(event, args[0]);
    s32 time = evtGetValue(event, args[1]);
    s32 mode = evtGetValue(event, args[2]);
    BattleStageSetAudDarkBase(a, time, mode);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_batstage_return_aud_dark_base) {
    EvtWord* args = event->args;
    s32 time = evtGetValue(event, args[0]);
    s32 mode = evtGetValue(event, args[1]);
    BattleStageReturnAudDarkBase(time, mode);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_batstage_light_set_player_on) {
    EvtWord* args = event->args;
    BattleWork* bw = _battleWorkPointer;
    s32 sel = evtGetValue(event, *args++);
    s32 i;
    s32 idx;
    char names[3][32] = {"sys_activeUnit_1", "sys_activeUnit_2", "sys_activeUnit_3"};
    Vec pos[3] = {{100.0f, 250.0f, 70.0f}, {-300.0f, 250.0f, 70.0f}, {-120.0f, 250.0f, -70.0f}};
    Vec tg[3] = {{100.0f, 0.0f, -70.0f}, {-300.0f, 0.0f, -70.0f}, {-120.0f, 0.0f, -70.0f}};
    Vec tu[3] = {{15.0f, 0.0f, 15.0f}, {-15.0f, 0.0f, 15.0f}, {0.0f, 0.0f, -15.0f}};
    Vec upv[3] = {{0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, -1.0f}};
    u8 col[3][3] = {{120, 120, 120}, {120, 120, 120}, {120, 120, 120}};
    u8 en[3] = {0, 0, 0};
    u8 en2[4] = {0, 0, 0, 0};

    switch (sel) {
    case 1:
        en[2] = 1;
        col[2][0] = 255;
        col[2][1] = 255;
        col[2][2] = 255;
        break;
    default:
        en[0] = 1;
        en[1] = 1;
        en[2] = 1;
        break;
    }
    for (i = 0; i < 3; i++) {
        if (en[i]) {
            idx = BattleStageGetLightNumberByName(names[i]);
            if (idx == -1) {
                idx = BattleStageSysLightEntry(names[i]);
            }
            BattleStageLightSetLightColor(idx, col[i][0], col[i][1], col[i][2], 30, 0);
            BattleStageLightSetWidth(idx, 120, 1, 0);
            BattleStageLightSetPosition(idx, pos[i].x, pos[i].y, pos[i].z, 0, 0);
            BattleStageLightSetUpVec(idx, upv[i].x, upv[i].y, upv[i].z, 0, 0);
            BattleStageLightSetTarget(idx, tg[i].x, tg[i].y, tg[i].z, 0, 0);
            BattleStageLightSetTargetByUnit(idx, bw->activeUnitId, tu[i].x, tu[i].y, tu[i].z, 30, 5);
        }
    }
    BattleStageSetStgDark(140, 30, 0);
    BattleStageSetAudDark(140, 0, 0);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_batstage_light_set_player_off) {
    s32 idx;
    BattleStageReturnAudDarkBase(0, 0);
    BattleStageReturnStgDarkBase(30, 0);
    idx = BattleStageGetLightNumberByName("sys_activeUnit_1");
    if (idx != -1) {
        BattleStageLightRelease(idx, 30, 0);
    }
    idx = BattleStageGetLightNumberByName("sys_activeUnit_2");
    if (idx != -1) {
        BattleStageLightRelease(idx, 30, 0);
    }
    idx = BattleStageGetLightNumberByName("sys_activeUnit_3");
    if (idx != -1) {
        BattleStageLightRelease(idx, 30, 0);
    }
    return EVT_RETURN_DONE;
}
