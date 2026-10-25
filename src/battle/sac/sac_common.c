#pragma optimize_for_size on
#include "battle/battle.h"
#include "battle/battle_ac_help.h"
#include "battle/battle_audience.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "manager/evtmgr.h"
#include "manager/evtmgr_cmd.h"
#include "system.h"

USER_FUNC(evt_btl_camera_set_mode);
USER_FUNC(evt_btl_camera_set_homing_unit);
USER_FUNC(evt_btl_camera_set_zoom);
USER_FUNC(evt_btl_camera_set_moveSpeedLv);
USER_FUNC(evt_btl_camera_set_zoomSpeedLv);
USER_FUNC(btlevtcmd_CheckStatus);
USER_FUNC(btlevtcmd_AnimeChangePoseType);
USER_FUNC(btlevtcmd_AnimeChangePose);
USER_FUNC(btlevtcmd_FaceDirectionAdd);
USER_FUNC(btlevtcmd_MovePosition);
USER_FUNC(btlevtcmd_snd_se);
USER_FUNC(btlevtcmd_GetHomePos);
USER_FUNC(btlevtcmd_SetMoveSpeed);
USER_FUNC(btlevtcmd_GetStatusMg);
USER_FUNC(evt_audience_sound_cheer);
USER_FUNC(evt_eff);
USER_FUNC(evt_eff_delete);

s32 BattleAudienceSoundCheck(s32 id);
void BattleAudienceSoundSetVol(s32 id, u8 vol, s32 c);
void BattleAudienceSoundHandBeat(void);
void BattleAudienceSoundCheerKind(s32 kind, s32 a, s32 b);
void BattleAudienceSoundWhistleKind(s32 kind);

static s32 count;
s32 count2;
s32 handBeatEnable;
s32 cheerEnable;
u8 volPer;

EVT_BEGIN(sac_start_camera_evt)
    CALL(evt_btl_camera_set_mode, 0, 7)
    CALL(evt_btl_camera_set_homing_unit, 0, -2, -1)
    CALL(btlevtcmd_CheckStatus, -2, 10, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_btl_camera_set_zoom, 0, 100)
    ELSE
        CALL(evt_btl_camera_set_zoom, 0, 300)
    END_IF
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoomSpeedLv, 0, 2)
    RETURN
EVT_END()

EVT_BEGIN(sac_mario_item_on_evt)
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 40)
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, -2, LW(0), 15)
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 5, -1, 0)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_VOICE_MARIO_HAPPY1"), EVTDAT_POINTER_MAX, 0, EVTDAT_POINTER_MAX)
    CALL(evt_audience_sound_cheer, 180, 60)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_P_2"))
    CALL(btlevtcmd_GetPartsPos, -2, 1, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHeight, -2, LW(5))
    CALL(btlevtcmd_GetStatusMg, -2, LW(6))
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

EVT_BEGIN(sac_mario_item_on_end_evt)
    CALL(evt_eff_delete, STRING("sac_itemget"))
    CALL(evt_eff_delete, STRING("sac_treasure"))
    RETURN
EVT_END()

EVT_BEGIN(sac_start_stardust)
    LOOP(6)
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        ADD(LW(1), 50)
        CALL(evt_eff, STRING(""), STRING("stardust"), 5, LW(0), LW(1), LW(2), 30, 30, 10, 30, 0, 0, 0, 0)
        WAIT_FRAMES(10)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(sac_mario_back_home)
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 40)
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 5, -1, 0)
    RETURN
EVT_END()

EVT_BEGIN(enemy_slide_go_evt)
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    ADD(LW(0), 480)
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(5.0f))
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, 0, 1)
    RETURN
EVT_END()

EVT_BEGIN(enemy_slide_return_evt)
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(7.0f))
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, 0, 1)
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

USER_FUNC(sac_handbeat) {
    BattleAudienceSound* sound;

    if (isFirstCall) {
        count = 0;
        count2 = 60;
        handBeatEnable = 1;
        volPer = 0;
        if (BattleAudienceSoundCheck(4) == 1) {
            BattleAudienceSoundStop(4);
        }
        if (BattleAudienceSoundCheck(5) == 1) {
            BattleAudienceSoundStop(5);
        }
        BattleAudienceSoundCheer(-1, 0);
        BattleAudienceSoundSetVol(4, 0, 1);
    }
    BattleAudienceSoundSetVol(4, volPer, 1);
    if (!handBeatEnable) {
        sound = BattleAudienceSoundGetPtr(4);
        sound->mFadeoutTime = *(s32*)(sound->header + 0xC) = 180;
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

USER_FUNC(sac_handbeat_end) {
    handBeatEnable = 0;
    return EVT_RETURN_DONE;
}

USER_FUNC(sac_cheer) {
    BattleAudienceSound* sound;
    s32 flag = evtGetValue(event, event->args[0]);

    if (isFirstCall) {
        count = 0;
        count2 = 60;
        cheerEnable = 1;
        volPer = 0;
        if (BattleAudienceSoundCheck(4) == 1) {
            BattleAudienceSoundStop(4);
        }
        if (BattleAudienceSoundCheck(5) == 1) {
            BattleAudienceSoundStop(5);
        }
        BattleAudienceSoundCheer(-1, 0);
        BattleAudienceSoundSetVol(4, 0, 1);
    }
    if (!flag) {
        BattleAudienceSoundSetVol(4, volPer, 1);
    }
    if (!cheerEnable) {
        sound = BattleAudienceSoundGetPtr(4);
        sound->mFadeoutTime = *(s32*)(sound->header + 0xC) = 180;
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

USER_FUNC(sac_cheer_end) {
    cheerEnable = 0;
    return EVT_RETURN_DONE;
}

USER_FUNC(sac_enemy_slide_go) {
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    EventEntry* evt;
    s32 i;

    for (i = 0; i < 64; i++) {
        unit = BattleGetUnitPtr(wp, i);
        if (unit && unit->mAlliance == ALLIANCE_ENEMY) {
            evt = evtEntry(enemy_slide_go_evt, 0, 0x20);
            evt->unitId = i;
        }
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(sac_enemy_slide_return) {
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    EventEntry* evt;
    s32 i;

    for (i = 0; i < 64; i++) {
        unit = BattleGetUnitPtr(wp, i);
        if (unit && unit->mAlliance == ALLIANCE_ENEMY) {
            evt = evtEntry(enemy_slide_return_evt, 0, 0x20);
            evt->unitId = i;
        }
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(sac_ac_help_on) {
    BattleAcHelpSetDispType(2);
    return EVT_RETURN_DONE;
}

USER_FUNC(sac_ac_help_off) {
    BattleAcHelpSetDispType(0);
    return EVT_RETURN_DONE;
}
