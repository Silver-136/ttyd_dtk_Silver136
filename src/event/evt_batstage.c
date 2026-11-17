// Script commands for the battle stage: stage and audience darkness, stage lights.

#include "battle/battle.h"
#include "manager/evtmgr_cmd.h"
#include <dolphin/mtx.h>
#include "battle/battle_stage.h"

// Old-style (non-prototype) declarations of battle_stage.c's darkness setters: the calls pass the s32 mode
// unconverted (battle_stage.c defines the parameter as u8 and converts it there).
void BattleStageReturnAudDarkBase();
void BattleStageSetAudDarkBase();
void BattleStageSetAudDark();
void BattleStageReturnStgDarkBase();
void BattleStageSetStgDarkBase();
void BattleStageSetStgDark();

/**
 * Fades the battle stage's darkness: args are the darkness level (0-255), the fade time in frames and a mode.
 * Used by the battle-end and Showstopper (sac_suki) scripts.
 */
USER_FUNC(evt_batstage_set_stg_dark) {
    EvtWord* args = event->args;
    u8 dark = evtGetValue(event, args[0]);
    s32 time = evtGetValue(event, args[1]);
    s32 mode = evtGetValue(event, args[2]);
    BattleStageSetStgDark(dark, time, mode);
    return EVT_RETURN_DONE;
}

/** Sets the stage's base darkness (the level evt_batstage_return_stg_dark_base fades back to): level, frames, mode. */
USER_FUNC(evt_batstage_set_stg_dark_base) {
    EvtWord* args = event->args;
    u8 dark = evtGetValue(event, args[0]);
    s32 time = evtGetValue(event, args[1]);
    s32 mode = evtGetValue(event, args[2]);
    BattleStageSetStgDarkBase(dark, time, mode);
    return EVT_RETURN_DONE;
}

/** Fades the stage darkness back to its base level; args are the fade time in frames and a mode. */
USER_FUNC(evt_batstage_return_stg_dark_base) {
    EvtWord* args = event->args;
    s32 time = evtGetValue(event, args[0]);
    s32 mode = evtGetValue(event, args[1]);
    BattleStageReturnStgDarkBase(time, mode);
    return EVT_RETURN_DONE;
}

/** Fades the audience's darkness: args are the darkness level (0-255), the fade time in frames and a mode. */
USER_FUNC(evt_batstage_set_aud_dark) {
    EvtWord* args = event->args;
    u8 dark = evtGetValue(event, args[0]);
    s32 time = evtGetValue(event, args[1]);
    s32 mode = evtGetValue(event, args[2]);
    BattleStageSetAudDark(dark, time, mode);
    return EVT_RETURN_DONE;
}

/** Sets the audience's base darkness level (restored by evt_batstage_return_aud_dark_base): level, frames, mode. */
USER_FUNC(evt_batstage_set_aud_dark_base) {
    EvtWord* args = event->args;
    u8 dark = evtGetValue(event, args[0]);
    s32 time = evtGetValue(event, args[1]);
    s32 mode = evtGetValue(event, args[2]);
    BattleStageSetAudDarkBase(dark, time, mode);
    return EVT_RETURN_DONE;
}

/** Fades the audience darkness back to its base level; args are the fade time in frames and a mode. */
USER_FUNC(evt_batstage_return_aud_dark_base) {
    EvtWord* args = event->args;
    s32 time = evtGetValue(event, args[0]);
    s32 mode = evtGetValue(event, args[1]);
    BattleStageReturnAudDarkBase(time, mode);
    return EVT_RETURN_DONE;
}

/** Switches stage light args[0] off over args[1] frames with curve mode args[2] (BattleStageLightRelease). */
// evt_batstage_light_release, _set_lightcolor, _set_width, _set_position, _set_target and _set_upvec: original names
// and sizes (0x7c, 0xa8, 0x98, 0xcc, 0xcc, 0xcc) from the JP demo map (UNUSED there: no script uses them). Each passes
// its arguments to the BattleStageLight function its name gives, in the form of evt_batstage_set_stg_dark; the sizes
// fit the light being given by its number. The map's _entry, _release_all and _set_target_by_ commands are not
// restored (their bodies or the functions they call are not known).
USER_FUNC(evt_batstage_light_release) {
    EvtWord* args = event->args;
    s32 idx = evtGetValue(event, args[0]);
    s32 time = evtGetValue(event, args[1]);
    s32 mode = evtGetValue(event, args[2]);
    BattleStageLightRelease(idx, time, mode);
    return EVT_RETURN_DONE;
}

/** Fades stage light args[0] to the colour args[1..3] over args[4] frames with curve mode args[5]. */
// Original name and size from the JP demo map (see evt_batstage_light_release).
USER_FUNC(evt_batstage_light_set_lightcolor) {
    EvtWord* args = event->args;
    s32 idx = evtGetValue(event, args[0]);
    u8 r = evtGetValue(event, args[1]);
    u8 g = evtGetValue(event, args[2]);
    u8 b = evtGetValue(event, args[3]);
    s32 time = evtGetValue(event, args[4]);
    s32 mode = evtGetValue(event, args[5]);
    BattleStageLightSetLightColor(idx, r, g, b, time, mode);
    return EVT_RETURN_DONE;
}

/** Changes the width of stage light args[0] to args[1] over args[2] frames with curve mode args[3]. */
// Original name and size from the JP demo map (see evt_batstage_light_release).
USER_FUNC(evt_batstage_light_set_width) {
    EvtWord* args = event->args;
    s32 idx = evtGetValue(event, args[0]);
    s32 width = evtGetValue(event, args[1]);
    s32 time = evtGetValue(event, args[2]);
    s32 mode = evtGetValue(event, args[3]);
    BattleStageLightSetWidth(idx, width, time, mode);
    return EVT_RETURN_DONE;
}

/** Moves stage light args[0] to the floats args[1..3] over args[4] frames with curve mode args[5]. */
// Original name and size from the JP demo map (see evt_batstage_light_release).
USER_FUNC(evt_batstage_light_set_position) {
    EvtWord* args = event->args;
    s32 idx = evtGetValue(event, args[0]);
    f32 x = evtGetFloat(event, args[1]);
    f32 y = evtGetFloat(event, args[2]);
    f32 z = evtGetFloat(event, args[3]);
    s32 time = evtGetValue(event, args[4]);
    BattleStageLightSetPosition(idx, x, y, z, time, evtGetValue(event, args[5]));
    return EVT_RETURN_DONE;
}

/** Moves the target of stage light args[0] to the floats args[1..3] over args[4] frames with curve mode args[5]. */
// Original name and size from the JP demo map (see evt_batstage_light_release).
USER_FUNC(evt_batstage_light_set_target) {
    EvtWord* args = event->args;
    s32 idx = evtGetValue(event, args[0]);
    f32 x = evtGetFloat(event, args[1]);
    f32 y = evtGetFloat(event, args[2]);
    f32 z = evtGetFloat(event, args[3]);
    s32 time = evtGetValue(event, args[4]);
    BattleStageLightSetTarget(idx, x, y, z, time, evtGetValue(event, args[5]));
    return EVT_RETURN_DONE;
}

/** Turns the up vector of stage light args[0] to the floats args[1..3] over args[4] frames with curve mode args[5]. */
// Original name and size from the JP demo map (see evt_batstage_light_release).
USER_FUNC(evt_batstage_light_set_upvec) {
    EvtWord* args = event->args;
    s32 idx = evtGetValue(event, args[0]);
    f32 x = evtGetFloat(event, args[1]);
    f32 y = evtGetFloat(event, args[2]);
    f32 z = evtGetFloat(event, args[3]);
    s32 time = evtGetValue(event, args[4]);
    BattleStageLightSetUpVec(idx, x, y, z, time, evtGetValue(event, args[5]));
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

    // Never read; unused in the original too: its stack slot and initializer template are in the retail code
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
