// Shared pieces for the star-power special moves (sac_*): the opening camera, cheering and hand-beat helpers.

#include "version.h"
#include "battle/battle.h"
#include "battle/battle_ac_help.h"
#include "battle/battle_audience.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "manager/evtmgr.h"
#include "manager/evtmgr_cmd.h"
#include "system.h"
#include "battle/battle_camera.h"
#include "evt/evt_eff.h"
#include "evt/evt_audience.h"
#include "battle/battle_status_effect.h"
#include "battle/battle_sub.h"

static s32 count;
s32 count2;
s32 handBeatEnable;
s32 cheerEnable;
u8 volPer;

/**
 * Opening camera of every special move: homes the battle camera on Mario and sets zoom 300,
 * or 100 while he is huge (STATUS_HUGE).
 */
EVT_BEGIN(sac_start_camera_evt)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, -1)
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_HUGE, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_btl_camera_set_zoom, 0, 100)
    ELSE
        CALL(evt_btl_camera_set_zoom, 0, 300)
    END_IF
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoomSpeedLv, 0, 2)
    RETURN
EVT_END()

/**
 * Mario steps 15 units forward with a happy shout and the crowd cheers;
 * he strikes pose M_P_2 and the Magical Map (treasure_map effect) and an item-get glow appear above his head,
 * removed again by sac_mario_item_on_end_evt.
 */
EVT_BEGIN(sac_mario_item_on_evt)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 15)
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 5, MOVE_SOUND_FROM_TABLE, 0)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_VOICE_MARIO_HAPPY1"), EVTDAT_POINTER_MAX, 0, EVTDAT_POINTER_MAX)
    CALL(evt_audience_sound_cheer, 180, 60)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_P_2"))
    CALL(btlevtcmd_GetPartsPos, UNIT_ID_SELF, 1, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHeight, UNIT_ID_SELF, LW(5))
    CALL(btlevtcmd_GetStatusMg, UNIT_ID_SELF, LW(6))
    MULTIPLY_FLOAT(LW(5), LW(6))
    ADD(LW(1), LW(5))
    ADD(LW(1), 35)
    ADD(LW(2), -5)
    CALL(evt_eff, STRING("sac_treasure"), STRING("treasure_map"), 0, LW(0), LW(1), LW(2), 0, 0, 0, 0, 0, 0, 0, 0)
    ADD(LW(1), 2)
    ADD(LW(2), -7)
    CALL(evt_eff, STRING("sac_itemget"), STRING("itemget"), 9, LW(0), LW(1), LW(2), 0, 0, 0, 0, 0, 0, 0, 0)
    RETURN
EVT_END()

#if VERSION <= VERSION_DEMO_US
EVT_BEGIN(sac_start_stardust)
    LOOP(6)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        ADD(LW(1), 50)
        CALL(evt_eff, STRING(""), STRING("stardust"), 5, LW(0), LW(1), LW(2), 30, 30, 10, 30, 0, 0, 0, 0)
        WAIT_FRAMES(10)
    END_LOOP
    RETURN
EVT_END()
#endif
EVT_BEGIN(sac_mario_item_on_end_evt)
    CALL(evt_eff_delete, STRING("sac_itemget"))
    CALL(evt_eff_delete, STRING("sac_treasure"))
    RETURN
EVT_END()

/** Six bursts of stardust 50 units above Mario, 10 frames apart, while the Crystal Star appears. */
#if VERSION > VERSION_DEMO_US
EVT_BEGIN(sac_start_stardust)
    LOOP(6)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        ADD(LW(1), 50)
        CALL(evt_eff, STRING(""), STRING("stardust"), 5, LW(0), LW(1), LW(2), 30, 30, 10, 30, 0, 0, 0, 0)
        WAIT_FRAMES(10)
    END_LOOP
    RETURN
EVT_END()
#endif

/** Walks Mario back to his home position after the opening pose. */
EVT_BEGIN(sac_mario_back_home)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 5, MOVE_SOUND_FROM_TABLE, 0)
    RETURN
EVT_END()

/** Per-enemy script of sac_enemy_slide_go: slides the unit 480 units to the right (off stage) at speed 5. */
EVT_BEGIN(enemy_slide_go_evt)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    ADD(LW(0), 480)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(5.0f))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_NONE, 1)
    RETURN
EVT_END()

/** Per-enemy script of sac_enemy_slide_return: slides the unit back to its home position at speed 7. */
EVT_BEGIN(enemy_slide_return_evt)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(7.0f))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_NONE, 1)
    RETURN
EVT_END()

// functions are emitted in reverse definition order (-inline deferred)
USER_FUNC(sac_wao) {
    s32 num = BattleAudience_GetAudienceNum();

    BattleAudienceSoundClap(120, 120);
    if (num >= 1 && num <= 149) {
        BattleAudienceSoundCheerKind(1, 120, 120);
    }
    if (num >= 150) {
        BattleAudienceSoundCheerKind(4, 120, 120);
        BattleAudienceSoundWhistleKind(1);
    }
    return EVT_RETURN_DONE;
}

/**
 * Runs until sac_handbeat_end: the crowd claps along in time,
 * starting every 60 frames and speeding up to every 20 while the clap volume rises to 100%.
 * Stops audience sounds 4 and 5 first; fades out over 180 frames.
 */
USER_FUNC(sac_handbeat) {
    BattleAudienceSound* sound;

    if (isFirstCall) {
        count = 0;
        count2 = 60;
        handBeatEnable = 1;
        volPer = 0;
        if (BattleAudienceSoundCheck(AUD_SOUND_CHEER_A) == 1) {
            BattleAudienceSoundStop(AUD_SOUND_CHEER_A);
        }
        if (BattleAudienceSoundCheck(AUD_SOUND_CHEER_B) == 1) {
            BattleAudienceSoundStop(AUD_SOUND_CHEER_B);
        }
        BattleAudienceSoundCheer(-1, 0);
        BattleAudienceSoundSetVol(AUD_SOUND_CHEER_A, 0, 1);
    }
    BattleAudienceSoundSetVol(AUD_SOUND_CHEER_A, volPer, 1);
    if (!handBeatEnable) {
        sound = BattleAudienceSoundGetPtr(AUD_SOUND_CHEER_A);
        sound->fadeoutTime = sound->duration = 180;
        return EVT_RETURN_DONE;
    }
    if (count < 0) {
        BattleAudienceSoundHandBeat();
        count = count2;
        count += irand(2);
        count2 -= 4;
        if (count2 < 20) {
            count2 = 20;
        }
        volPer += 5;
        if (volPer > 100) {
            volPer = 100;
        }
    }
    count--;
    return EVT_RETURN_BLOCK;
}

/** Tells a running sac_handbeat to fade the clapping out and finish. */
USER_FUNC(sac_handbeat_end) {
    handBeatEnable = 0;
    return EVT_RETURN_DONE;
}

/**
 * Runs until sac_cheer_end: a crowd cheer whose volume rises 5% per beat.
 * args[0] nonzero leaves the volume alone.
 * Same timer as sac_handbeat but without the claps; fades out over 180 frames when ended.
 */
USER_FUNC(sac_cheer) {
    BattleAudienceSound* sound;
    s32 flag = evtGetValue(event, event->args[0]);

    if (isFirstCall) {
        count = 0;
        count2 = 60;
        cheerEnable = 1;
        volPer = 0;
        if (BattleAudienceSoundCheck(AUD_SOUND_CHEER_A) == 1) {
            BattleAudienceSoundStop(AUD_SOUND_CHEER_A);
        }
        if (BattleAudienceSoundCheck(AUD_SOUND_CHEER_B) == 1) {
            BattleAudienceSoundStop(AUD_SOUND_CHEER_B);
        }
        BattleAudienceSoundCheer(-1, 0);
        BattleAudienceSoundSetVol(AUD_SOUND_CHEER_A, 0, 1);
    }
    if (!flag) {
        BattleAudienceSoundSetVol(AUD_SOUND_CHEER_A, volPer, 1);
    }
    if (!cheerEnable) {
        sound = BattleAudienceSoundGetPtr(AUD_SOUND_CHEER_A);
        sound->fadeoutTime = sound->duration = 180;
        return EVT_RETURN_DONE;
    }
    if (count < 0) {
        count = count2;
        count += irand(2);
        count2 -= 4;
        if (count2 < 20) {
            count2 = 20;
        }
        volPer += 5;
        if (volPer > 100) {
            volPer = 100;
        }
    }
    count--;
    return EVT_RETURN_BLOCK;
}

/** Tells a running sac_cheer to fade the cheer out and finish. */
USER_FUNC(sac_cheer_end) {
    cheerEnable = 0;
    return EVT_RETURN_DONE;
}

/** Starts enemy_slide_go_evt on every enemy-side unit, sliding the whole enemy party off stage. */
USER_FUNC(sac_enemy_slide_go) {
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    EventEntry* evt;
    s32 i;

    for (i = 0; i < BATTLE_UNIT_MAX; i++) {
        unit = BattleGetUnitPtr(wp, i);
        if (unit && unit->alliance == ALLIANCE_ENEMY) {
            evt = evtEntry(enemy_slide_go_evt, 0, EVENT_START_IMMEDIATE);
            evt->unitId = i;
        }
    }
    return EVT_RETURN_DONE;
}

/**
 * Starts enemy_slide_return_evt on every enemy-side unit to slide them home.
 * The PAL release also records each script's id in the unit (slideReturnEventId)
 * so battle_seq can wait for the slide to finish.
 */
USER_FUNC(sac_enemy_slide_return) {
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    EventEntry* evt;
    s32 i;

    for (i = 0; i < BATTLE_UNIT_MAX; i++) {
        unit = BattleGetUnitPtr(wp, i);
        if (unit && unit->alliance == ALLIANCE_ENEMY) {
            evt = evtEntry(enemy_slide_return_evt, 0, EVENT_START_IMMEDIATE);
            evt->unitId = i;
#if VERSION == VERSION_EU
            unit->slideReturnEventId = evt->eventId;
#endif
        }
    }
    return EVT_RETURN_DONE;
}

/** Switches the action-command help display to the special-move style (type 2), which hides the audience window. */
USER_FUNC(sac_ac_help_on) {
    BattleAcHelpSetDispType(2);
    return EVT_RETURN_DONE;
}

/** Restores the normal action-command help display (type 0) and shows the audience window again. */
USER_FUNC(sac_ac_help_off) {
    BattleAcHelpSetDispType(0);
    return EVT_RETURN_DONE;
}
