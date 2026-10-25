#include "battle/battle.h"
#include "battle/battle_sub.h"
#include "battle/battle_unit.h"
#include "driver/effdrv.h"
#include "evt/evt_cmd.h"
#include "manager/evtmgr.h"
#include "manager/evtmgr_cmd.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_mario.h"
#include "evt/evt_eff.h"
#include "evt/evt_sub.h"
#include "evt/evt_snd.h"
#include "unit/party/unit_party_yoshi.h"
#include "eff/n64/eff_whirlwind_n64.h"

USER_FUNC(_binta_effect);
USER_FUNC(btlevtcmd_AddRotate);
USER_FUNC(btlevtcmd_AnimeChangePoseFromTable);
USER_FUNC(btlevtcmd_AnimeChangePoseType);
USER_FUNC(btlevtcmd_AnimeCmpPoseType);
USER_FUNC(btlevtcmd_AnimeSetMotionSpeed);
USER_FUNC(btlevtcmd_CheckDamagePattern);
USER_FUNC(btlevtcmd_CheckDataOfDataTable);
USER_FUNC(btlevtcmd_CheckDefinePosetype);
USER_FUNC(btlevtcmd_CheckStatus);
USER_FUNC(btlevtcmd_EllipseMovePosition);
USER_FUNC(btlevtcmd_FaceDirectionSub);
USER_FUNC(btlevtcmd_GetBodyId);
USER_FUNC(btlevtcmd_GetFaceDirection);
USER_FUNC(btlevtcmd_GetGuardKouraId);
USER_FUNC(btlevtcmd_GetHomePos);
USER_FUNC(btlevtcmd_GetHp);
USER_FUNC(btlevtcmd_GetPartnerId);
USER_FUNC(btlevtcmd_GetPos);
USER_FUNC(btlevtcmd_GetUnitWork);
USER_FUNC(btlevtcmd_JumpPosition);
USER_FUNC(btlevtcmd_MovePosition);
USER_FUNC(btlevtcmd_OffPartsAttribute);
USER_FUNC(btlevtcmd_OnToken);
USER_FUNC(btlevtcmd_RunDataEventChild);
USER_FUNC(btlevtcmd_SetFallAccel);
USER_FUNC(btlevtcmd_SetHomePos);
USER_FUNC(btlevtcmd_SetRotate);
USER_FUNC(btlevtcmd_SetUnitWork);
USER_FUNC(btlevtcmd_SortPhaseMoveTable);
USER_FUNC(btlevtcmd_StartWaitEvent);
USER_FUNC(btlevtcmd_StopWaitEvent);
USER_FUNC(btlevtcmd_TransStageFloorPosition);
USER_FUNC(btlevtcmd_WaitAttackEnd);
USER_FUNC(btlevtcmd_WaitEventEnd);
USER_FUNC(evt_btl_camera_set_homing_unit);
USER_FUNC(evt_btl_camera_set_mode);
USER_FUNC(evt_btl_camera_set_moveSpeedLv);
USER_FUNC(evt_btl_camera_set_zoom);
USER_FUNC(evt_snd_sfxon);
USER_FUNC(evt_sub_intpl_get_value);
USER_FUNC(evt_sub_intpl_init);
USER_FUNC(_check_through_pos);
USER_FUNC(_set_hp_gauge_fade_flag);
USER_FUNC(_add_star_point_disp_offset);
USER_FUNC(_check_at_dm_event_wait);
USER_FUNC(_disable_restore_command_cursor);

#ifdef VERSION_G8ME01
EvtWord subsetevt_dead[612];
#else
EvtWord subsetevt_dead[608];
#endif
EvtWord subsetevt_dead_core[193];
EvtWord subsetevt_dead_core_nospin_norotate[125];
EvtWord btldefaultevt_avoid_guard[92];
EvtWord subsetevt_damage[979];
EvtWord subsetevt_counter_damage[174];
EvtWord btldefaultevt_ChangePosition[186];
EvtWord subsetevt_happy_heart[53];
EvtWord subsetevt_happy_flower[53];
EvtWord subsetevt_terror[51];
EvtWord subsetevt_blow_damage[69];
EvtWord subsetevt_blow_dead[76];
EvtWord subsetevt_shot_damage[31];
EvtWord subsetevt_shot_damage_kaiten[288];
EvtWord subsetevt_shot_damage_ultra[429];
EvtWord subsetevt_swallow_failed_damage[55];
EvtWord subsetevt_swallow_spew_damage[55];
EvtWord subsetevt_swallow_shot_damage[342];
EvtWord subset_ultra_hammer_acrobat_damage_event[63];
EvtWord subsetevt_confuse_flustered[43];
EvtWord subsetevt_binta_damage_left[48];
EvtWord subsetevt_binta_damage_right[48];
EvtWord subsetevt_binta_damage_left_last[69];
EvtWord subsetevt_binta_damage_right_last[69];
EvtWord subsetevt_status_recovery[48];
EvtWord subsetevt_check_target_movable[57];
EvtWord subsetevt_nodamage_motion[6];
EvtWord subsetevt_regeneration[4];
EvtWord subset_attack_ready_jump_event[2];

#ifdef VERSION_G8ME01
EVT_BEGIN(subsetevt_dead)
    CALL(_disable_restore_command_cursor)
    CALL(btlevtcmd_GetUnitKind, LW(10), LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(222)
        GOTO(0)
        CASE_BETWEEN(224, 230)
        GOTO(0)
        CASE_DEFAULT
        GOTO(1)
    END_SWITCH
    LABEL(0)
    CALL(btlevtcmd_CheckAttribute, LW(10), 4, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), 42)
        CALL(btlevtcmd_GetHomePos, LW(10), LW(2), LW(3), LW(4))
        CALL(btlevtcmd_SetMoveSpeed, LW(10), FLOAT(4.0f))
        CALL(btlevtcmd_DivePosition, LW(10), LW(2), LW(3), LW(4), 0, 0, 0, 0, -1)
        ELSE
        CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), 40)
        CALL(btlevtcmd_GetHomePos, LW(10), LW(2), LW(3), LW(4))
        CALL(btlevtcmd_SetMoveSpeed, LW(10), FLOAT(4.0f))
        CALL(btlevtcmd_MovePosition, LW(10), LW(2), LW(3), LW(4), 0, -1, 0)
    END_IF
    CALL(btlevtcmd_ResetFaceDirection, LW(10))
    LABEL(1)
    CALL(btlevtcmd_OnOffStatus, LW(10), 27, 1, 1, 1)
    CALL(btlevtcmd_GetUnitKind, LW(10), LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(222)
        CALL(btlevtcmd_ClearAllStatus, LW(10))
        CALL(btlevtcmd_CheckDamageCode, LW(10), 2048, LW(1))
        IF_INT_NOT_EQUAL(LW(1), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), 51)
            ELSE
            CALL(btlevtcmd_RunDataEventChild, LW(10), 49)
        END_IF
        CASE_BETWEEN(224, 230)
        CALL(btlevtcmd_CheckDamageCode, LW(10), 2048, LW(1))
        IF_INT_NOT_EQUAL(LW(1), 0)
            CALL(btlevtcmd_ClearAllStatus, LW(10))
            CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(0), 51)
            IF_INT_NOT_EQUAL(LW(0), 0)
                CALL(btlevtcmd_RunDataEventChild, LW(10), 51)
                ELSE
                CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), 27)
                BEGIN_SIBLING_EVENT
                    CALL(btlevtcmd_AnimeWaitPlayComplete, LW(10), LW(11))
                    CALL(btlevtcmd_snd_se, LW(10), STRING("SFX_PARTY_BATTLE_DIE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
                END_SIBLING_EVENT
                WAIT_FRAMES(60)
                CALL(btlevtcmd_ClearAllStatus, LW(10))
                CALL(btlevtcmd_ConsumeItemReserve, LW(10), 151)
                BEGIN_SIBLING_EVENT
                    CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
                    ADD(LW(1), 40)
                    CALL(btlevtcmd_DispItemIcon, LW(0), LW(1), LW(2), 151, 90)
                END_SIBLING_EVENT
                WAIT_FRAMES(90)
                CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(0), 67)
                IF_INT_NOT_EQUAL(LW(0), 0)
                    CALL(btlevtcmd_RunDataEventChild, LW(10), 67)
                END_IF
                CALL(btlevtcmd_OnOffStatus, LW(10), 27, 0, 0, 0)
                CALL(btlevtcmd_SetHp, LW(10), 0)
                CALL(btlevtcmd_RecoverHp, LW(10), LW(11), 10)
                CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
                ADD(LW(1), 30)
                CALL(evt_eff, 0, STRING("recovery"), 0, LW(0), LW(1), LW(2), 10, 0, 0, 0, 0, 0, 0, 0)
                WAIT_MS(2000)
                CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), 42)
                CALL(btlevtcmd_JumpSetting, LW(10), 0, FLOAT(0.0f), FLOAT(0.69921875f))
                CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
                CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 20, -1)
                CALL(btlevtcmd_StartWaitEvent, -2)
            END_IF
            ELSE
            CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(0), 49)
            IF_INT_NOT_EQUAL(LW(0), 0)
                CALL(btlevtcmd_RunDataEventChild, LW(10), 49)
                ELSE
            END_IF
            CALL(btlevtcmd_KillUnit, LW(10), 1)
            CALL(btlevtcmd_GetBodyId, -2, LW(11))
            CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), 27)
            BEGIN_SIBLING_EVENT
                CALL(btlevtcmd_AnimeWaitPlayComplete, LW(10), LW(11))
                CALL(btlevtcmd_snd_se, LW(10), STRING("SFX_PARTY_BATTLE_DIE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
            END_SIBLING_EVENT
            CALL(btlevtcmd_GetHomePos, LW(10), LW(0), LW(1), LW(2))
            CALL(btlevtcmd_GetPartnerId, LW(10), LW(4))
            CALL(btlevtcmd_GetHomePos, LW(4), LW(6), LW(7), LW(8))
            IF_INT_LESS(LW(0), 0)
                MULTIPLY(LW(0), -1)
            END_IF
            IF_INT_LESS(LW(6), 0)
                MULTIPLY(LW(6), -1)
            END_IF
            IF_INT_LESS(LW(0), LW(6))
                WAIT_MS(500)
                CALL(_check_at_dm_event_wait, -3)
                CALL(btlevtcmd_GetHp, -3, LW(0))
                IF_INT_GREATER(LW(0), 0)
                    CALL_CHILD(btldefaultevt_ChangePosition)
                END_IF
            END_IF
        END_IF
        CASE_DEFAULT
        CALL(btlevtcmd_CheckDamageCode, LW(10), 2048, LW(1))
        IF_INT_NOT_EQUAL(LW(1), 0)
            CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(0), 51)
            IF_INT_NOT_EQUAL(LW(0), 0)
                CALL(btlevtcmd_ClearAllStatus, LW(10))
                CALL(btlevtcmd_RunDataEventChild, LW(10), 51)
                ELSE
                CALL(btlevtcmd_OnUnitFlag, LW(10), 134217728)
                SET_VALUE(LW(0), 48)
                LABEL(5)
                WAIT_FRAMES(1)
                CALL(btlevtcmd_AddRotate, LW(10), 0, -15, 0)
                SUBTRACT(LW(0), 1)
                IF_INT_GREATER(LW(0), 0)
                    GOTO(5)
                END_IF
                CALL(btlevtcmd_SetRotate, LW(10), 0, 0, 0)
                CALL(btlevtcmd_SetRotateOffset, LW(10), 0, 0, 0)
                SET_VALUE(LW(0), 30)
                LABEL(6)
                WAIT_FRAMES(1)
                CALL(btlevtcmd_AddRotate, LW(10), 3, 0, 0)
                SUBTRACT(LW(0), 1)
                IF_INT_GREATER(LW(0), 0)
                    GOTO(6)
                END_IF
                CALL(btlevtcmd_ClearAllStatus, LW(10))
                CALL(btlevtcmd_ConsumeItemReserve, LW(10), 151)
                BEGIN_SIBLING_EVENT
                    CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
                    ADD(LW(1), 40)
                    CALL(btlevtcmd_DispItemIcon, LW(0), LW(1), LW(2), 151, 120)
                END_SIBLING_EVENT
                WAIT_FRAMES(90)
                SET_VALUE(LW(0), 30)
                LABEL(7)
                WAIT_FRAMES(1)
                CALL(btlevtcmd_AddRotate, LW(10), -3, 0, 0)
                SUBTRACT(LW(0), 1)
                IF_INT_GREATER(LW(0), 0)
                    GOTO(7)
                END_IF
                CALL(btlevtcmd_OnOffStatus, LW(10), 27, 0, 0, 0)
                CALL(btlevtcmd_SetHp, LW(10), 0)
                CALL(btlevtcmd_RecoverHp, LW(10), LW(11), 10)
                CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
                ADD(LW(1), 30)
                CALL(evt_eff, 0, STRING("recovery"), 0, LW(0), LW(1), LW(2), 10, 0, 0, 0, 0, 0, 0, 0)
                WAIT_MS(2000)
                CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), 42)
                CALL(btlevtcmd_JumpSetting, LW(10), 0, FLOAT(0.0f), FLOAT(0.69921875f))
                CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
                CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 20, -1)
                CALL(btlevtcmd_OffUnitFlag, LW(10), 134217728)
                CALL(btlevtcmd_OffUnitFlag, LW(10), 1073741824)
                CALL(btlevtcmd_StartWaitEvent, -2)
            END_IF
            ELSE
            CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(0), 49)
            IF_INT_NOT_EQUAL(LW(0), 0)
                CALL(btlevtcmd_ClearAllStatus, LW(10))
                CALL(btlevtcmd_RunDataEventChild, LW(10), 49)
                ELSE
                CALL_CHILD(subsetevt_dead_core)
            END_IF
        END_IF
    END_SWITCH
    RETURN
EVT_END()
#else
EVT_BEGIN(subsetevt_dead)
    CALL(_disable_restore_command_cursor)
    CALL(btlevtcmd_GetUnitKind, LW(10), LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(222)
        GOTO(0)
        CASE_BETWEEN(224, 230)
        GOTO(0)
        CASE_DEFAULT
        GOTO(1)
    END_SWITCH
    LABEL(0)
    CALL(btlevtcmd_CheckAttribute, LW(10), 4, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), 42)
        CALL(btlevtcmd_GetHomePos, LW(10), LW(2), LW(3), LW(4))
        CALL(btlevtcmd_SetMoveSpeed, LW(10), FLOAT(4.0f))
        CALL(btlevtcmd_DivePosition, LW(10), LW(2), LW(3), LW(4), 0, 0, 0, 0, -1)
    ELSE
        CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), 40)
        CALL(btlevtcmd_GetHomePos, LW(10), LW(2), LW(3), LW(4))
        CALL(btlevtcmd_SetMoveSpeed, LW(10), FLOAT(4.0f))
        CALL(btlevtcmd_MovePosition, LW(10), LW(2), LW(3), LW(4), 0, -1, 0)
    END_IF
    CALL(btlevtcmd_ResetFaceDirection, LW(10))
    LABEL(1)
    CALL(btlevtcmd_OnOffStatus, LW(10), 27, 1, 1, 1)
    CALL(btlevtcmd_GetUnitKind, LW(10), LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(222)
        CALL(btlevtcmd_ClearAllStatus, LW(10))
        CALL(btlevtcmd_CheckDamageCode, LW(10), 2048, LW(1))
        IF_INT_NOT_EQUAL(LW(1), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), 51)
        ELSE
            CALL(btlevtcmd_RunDataEventChild, LW(10), 49)
        END_IF
        CASE_BETWEEN(224, 230)
        CALL(btlevtcmd_CheckDamageCode, LW(10), 2048, LW(1))
        IF_INT_NOT_EQUAL(LW(1), 0)
            CALL(btlevtcmd_ClearAllStatus, LW(10))
            CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(0), 51)
            IF_INT_NOT_EQUAL(LW(0), 0)
                CALL(btlevtcmd_RunDataEventChild, LW(10), 51)
            ELSE
                CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), 27)
                BEGIN_SIBLING_EVENT
                    CALL(btlevtcmd_AnimeWaitPlayComplete, LW(10), LW(11))
                    CALL(btlevtcmd_snd_se, LW(10), STRING("SFX_PARTY_BATTLE_DIE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
                END_SIBLING_EVENT
                WAIT_FRAMES(60)
                CALL(btlevtcmd_ClearAllStatus, LW(10))
                CALL(btlevtcmd_ConsumeItemReserve, LW(10), 151)
                BEGIN_SIBLING_EVENT
                    CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
                    ADD(LW(1), 40)
                    CALL(btlevtcmd_DispItemIcon, LW(0), LW(1), LW(2), 151, 90)
                END_SIBLING_EVENT
                WAIT_FRAMES(90)
                CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(0), 67)
                IF_INT_NOT_EQUAL(LW(0), 0)
                    CALL(btlevtcmd_RunDataEventChild, LW(10), 67)
                END_IF
                CALL(btlevtcmd_OnOffStatus, LW(10), 27, 0, 0, 0)
                CALL(btlevtcmd_SetHp, LW(10), 0)
                CALL(btlevtcmd_RecoverHp, LW(10), LW(11), 10)
                CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
                ADD(LW(1), 30)
                CALL(evt_eff, 0, STRING("recovery"), 0, LW(0), LW(1), LW(2), 10, 0, 0, 0, 0, 0, 0, 0)
                WAIT_MS(2000)
                CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), 42)
                CALL(btlevtcmd_JumpSetting, LW(10), 0, FLOAT(0.0f), FLOAT(0.69921875f))
                CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
                CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 20, -1)
                CALL(btlevtcmd_StartWaitEvent, -2)
            END_IF
        ELSE
            CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(0), 49)
            IF_INT_NOT_EQUAL(LW(0), 0)
                CALL(btlevtcmd_RunDataEventChild, LW(10), 49)
            ELSE
            END_IF
            CALL(btlevtcmd_KillUnit, LW(10), 1)
            CALL(btlevtcmd_GetBodyId, -2, LW(11))
            CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), 27)
            BEGIN_SIBLING_EVENT
                CALL(btlevtcmd_AnimeWaitPlayComplete, LW(10), LW(11))
                CALL(btlevtcmd_snd_se, LW(10), STRING("SFX_PARTY_BATTLE_DIE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
            END_SIBLING_EVENT
            CALL(btlevtcmd_GetHomePos, LW(10), LW(0), LW(1), LW(2))
            CALL(btlevtcmd_GetPartnerId, LW(10), LW(4))
            CALL(btlevtcmd_GetHomePos, LW(4), LW(6), LW(7), LW(8))
            IF_INT_LESS(LW(0), 0)
                MULTIPLY(LW(0), -1)
            END_IF
            IF_INT_LESS(LW(6), 0)
                MULTIPLY(LW(6), -1)
            END_IF
            IF_INT_LESS(LW(0), LW(6))
                WAIT_MS(500)
                CALL(_check_at_dm_event_wait, -3)
                CALL(btlevtcmd_GetHp, -3, LW(0))
                IF_INT_GREATER(LW(0), 0)
                    CALL_CHILD(btldefaultevt_ChangePosition)
                END_IF
            END_IF
        END_IF
        CASE_DEFAULT
        CALL(btlevtcmd_CheckDamageCode, LW(10), 2048, LW(1))
        IF_INT_NOT_EQUAL(LW(1), 0)
            CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(0), 51)
            IF_INT_NOT_EQUAL(LW(0), 0)
                CALL(btlevtcmd_ClearAllStatus, LW(10))
                CALL(btlevtcmd_RunDataEventChild, LW(10), 51)
            ELSE
                CALL(btlevtcmd_OnUnitFlag, LW(10), 134217728)
                SET_VALUE(LW(0), 48)
                LABEL(5)
                WAIT_FRAMES(1)
                CALL(btlevtcmd_AddRotate, LW(10), 0, -15, 0)
                SUBTRACT(LW(0), 1)
                IF_INT_GREATER(LW(0), 0)
                    GOTO(5)
                END_IF
                CALL(btlevtcmd_SetRotate, LW(10), 0, 0, 0)
                CALL(btlevtcmd_SetRotateOffset, LW(10), 0, 0, 0)
                SET_VALUE(LW(0), 30)
                LABEL(6)
                WAIT_FRAMES(1)
                CALL(btlevtcmd_AddRotate, LW(10), 3, 0, 0)
                SUBTRACT(LW(0), 1)
                IF_INT_GREATER(LW(0), 0)
                    GOTO(6)
                END_IF
                CALL(btlevtcmd_ClearAllStatus, LW(10))
                CALL(btlevtcmd_ConsumeItemReserve, LW(10), 151)
                BEGIN_SIBLING_EVENT
                    CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
                    ADD(LW(1), 40)
                    CALL(btlevtcmd_DispItemIcon, LW(0), LW(1), LW(2), 151, 120)
                END_SIBLING_EVENT
                WAIT_FRAMES(90)
                SET_VALUE(LW(0), 30)
                LABEL(7)
                WAIT_FRAMES(1)
                CALL(btlevtcmd_AddRotate, LW(10), -3, 0, 0)
                SUBTRACT(LW(0), 1)
                IF_INT_GREATER(LW(0), 0)
                    GOTO(7)
                END_IF
                CALL(btlevtcmd_OnOffStatus, LW(10), 27, 0, 0, 0)
                CALL(btlevtcmd_SetHp, LW(10), 0)
                CALL(btlevtcmd_RecoverHp, LW(10), LW(11), 10)
                CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
                ADD(LW(1), 30)
                CALL(evt_eff, 0, STRING("recovery"), 0, LW(0), LW(1), LW(2), 10, 0, 0, 0, 0, 0, 0, 0)
                WAIT_MS(2000)
                CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), 42)
                CALL(btlevtcmd_JumpSetting, LW(10), 0, FLOAT(0.0f), FLOAT(0.69921875f))
                CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
                CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 20, -1)
                CALL(btlevtcmd_OffUnitFlag, LW(10), 134217728)
                CALL(btlevtcmd_StartWaitEvent, -2)
            END_IF
        ELSE
            CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(0), 49)
            IF_INT_NOT_EQUAL(LW(0), 0)
                CALL(btlevtcmd_ClearAllStatus, LW(10))
                CALL(btlevtcmd_RunDataEventChild, LW(10), 49)
            ELSE
                CALL_CHILD(subsetevt_dead_core)
            END_IF
        END_IF
    END_SWITCH
    RETURN
EVT_END()
#endif

#ifdef VERSION_G8ME01
EVT_BEGIN(subsetevt_dead_core)
    CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(0), 50)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_RunDataEventChild, LW(10), 50)
    END_IF
    SET_VALUE(LW(0), 48)
    CALL(btlevtcmd_OnUnitFlag, LW(10), 134217728)
    CALL(btlevtcmd_snd_se, LW(10), STRING("SFX_BTL_ENEMY_DIE1_1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    LABEL(10)
    WAIT_FRAMES(1)
    CALL(btlevtcmd_AddRotate, LW(10), 0, -15, 0)
    SUBTRACT(LW(0), 1)
    IF_INT_GREATER(LW(0), 0)
        GOTO(10)
    END_IF
    CALL(btlevtcmd_SetRotate, LW(10), 0, 0, 0)
    CALL(btlevtcmd_snd_se, LW(10), STRING("SFX_BTL_ENEMY_DIE1_2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
    ADD(LW(2), 10)
    CALL(evt_eff, 0, STRING("kemuri_test"), 4, LW(0), LW(1), LW(2), FLOAT(0.7998047f), 0, 0, 0, 0, 0, 0, 0)
    CALL(btlevtcmd_GetCoin, LW(10), LW(0))
    CALL(btlevtcmd_StoreCoin, LW(0))
    CALL(btlevtcmd_GetExp, LW(10), LW(0))
    CALL(btlevtcmd_StoreExp, LW(0))
    CALL(btlevtcmd_GetPos, LW(10), LW(3), LW(1), LW(2))
    ADD(LW(2), 10)
    ADD(LW(1), 30)
    CALL(_add_star_point_disp_offset, LW(10), LW(3), LW(1), LW(2))
    CALL(evt_eff, STRING(""), STRING("star_point"), 0, LW(3), LW(1), LW(2), LW(0), 0, 0, 0, 0, 0, 0, 0)
    IF_INT_GREATER_OR_EQUAL(LW(0), 1)
        LOOP(LW(0))
        END_LOOP
    END_IF
    CALL(btlevtcmd_SetRotateOffset, LW(10), 0, 0, 0)
    SET_VALUE(LW(0), 30)
    LABEL(20)
    WAIT_FRAMES(1)
    CALL(btlevtcmd_AddRotate, LW(10), 3, 0, 0)
    SUBTRACT(LW(0), 1)
    IF_INT_GREATER(LW(0), 0)
        GOTO(20)
    END_IF
    CALL(btlevtcmd_GetBackItem, LW(10))
    CALL(btlevtcmd_ClearAllStatus, LW(10))
    CALL(btlevtcmd_CheckDamageCode, LW(10), 1024, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_KillUnit, LW(10), 0)
        ELSE
        CALL(btlevtcmd_KillUnit, LW(10), 2)
    END_IF
    RETURN
EVT_END()
#else
EVT_BEGIN(subsetevt_dead_core)
    CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(0), 50)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_RunDataEventChild, LW(10), 50)
    END_IF
    SET_VALUE(LW(0), 48)
    CALL(btlevtcmd_OnUnitFlag, LW(10), 134217728)
    CALL(btlevtcmd_snd_se, LW(10), STRING("SFX_BTL_ENEMY_DIE1_1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    LABEL(10)
    WAIT_FRAMES(1)
    CALL(btlevtcmd_AddRotate, LW(10), 0, -15, 0)
    SUBTRACT(LW(0), 1)
    IF_INT_GREATER(LW(0), 0)
        GOTO(10)
    END_IF
    CALL(btlevtcmd_SetRotate, LW(10), 0, 0, 0)
    CALL(btlevtcmd_snd_se, LW(10), STRING("SFX_BTL_ENEMY_DIE1_2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
    ADD(LW(2), 10)
    CALL(evt_eff, 0, STRING("kemuri_test"), 4, LW(0), LW(1), LW(2), FLOAT(0.7998047f), 0, 0, 0, 0, 0, 0, 0)
    CALL(btlevtcmd_GetCoin, LW(10), LW(0))
    CALL(btlevtcmd_StoreCoin, LW(0))
    CALL(btlevtcmd_GetExp, LW(10), LW(0))
    CALL(btlevtcmd_StoreExp, LW(0))
    CALL(btlevtcmd_GetPos, LW(10), LW(3), LW(1), LW(2))
    ADD(LW(2), 10)
    ADD(LW(1), 30)
    CALL(_add_star_point_disp_offset, LW(10), LW(3), LW(1), LW(2))
    CALL(evt_eff, STRING(""), STRING("star_point"), 0, LW(3), LW(1), LW(2), LW(0), 0, 0, 0, 0, 0, 0, 0)
    IF_INT_GREATER_OR_EQUAL(LW(0), 1)
        LOOP(LW(0))
        END_LOOP
    END_IF
    CALL(btlevtcmd_SetRotateOffset, LW(10), 0, 0, 0)
    SET_VALUE(LW(0), 30)
    LABEL(20)
    WAIT_FRAMES(1)
    CALL(btlevtcmd_AddRotate, LW(10), 3, 0, 0)
    SUBTRACT(LW(0), 1)
    IF_INT_GREATER(LW(0), 0)
        GOTO(20)
    END_IF
    CALL(btlevtcmd_GetBackItem, LW(10))
    CALL(btlevtcmd_ClearAllStatus, LW(10))
    CALL(btlevtcmd_CheckDamageCode, LW(10), 1024, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_KillUnit, LW(10), 0)
    ELSE
        CALL(btlevtcmd_KillUnit, LW(10), 2)
    END_IF
    RETURN
EVT_END()
#endif

#ifdef VERSION_G8ME01
EVT_BEGIN(subsetevt_dead_core_nospin_norotate)
    CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(0), 50)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_RunDataEventChild, LW(10), 50)
    END_IF
    CALL(btlevtcmd_snd_se, LW(10), STRING("SFX_BTL_ENEMY_DIE1_2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
    ADD(LW(2), 10)
    CALL(evt_eff, 0, STRING("kemuri_test"), 4, LW(0), LW(1), LW(2), FLOAT(0.7998047f), 0, 0, 0, 0, 0, 0, 0)
    CALL(btlevtcmd_GetCoin, LW(10), LW(0))
    CALL(btlevtcmd_StoreCoin, LW(0))
    CALL(btlevtcmd_GetExp, LW(10), LW(0))
    CALL(btlevtcmd_StoreExp, LW(0))
    CALL(btlevtcmd_GetPos, LW(10), LW(3), LW(1), LW(2))
    ADD(LW(2), 10)
    ADD(LW(1), 30)
    CALL(_add_star_point_disp_offset, LW(10), LW(3), LW(1), LW(2))
    CALL(evt_eff, STRING(""), STRING("star_point"), 0, LW(3), LW(1), LW(2), LW(0), 0, 0, 0, 0, 0, 0, 0)
    IF_INT_GREATER_OR_EQUAL(LW(0), 1)
        LOOP(LW(0))
        END_LOOP
    END_IF
    WAIT_FRAMES(30)
    CALL(btlevtcmd_GetBackItem, LW(10))
    CALL(btlevtcmd_CheckDamageCode, LW(10), 1024, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_KillUnit, LW(10), 1)
        ELSE
        CALL(btlevtcmd_KillUnit, LW(10), 2)
    END_IF
    RETURN
EVT_END()
#else
EVT_BEGIN(subsetevt_dead_core_nospin_norotate)
    CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(0), 50)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_RunDataEventChild, LW(10), 50)
    END_IF
    CALL(btlevtcmd_snd_se, LW(10), STRING("SFX_BTL_ENEMY_DIE1_2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
    ADD(LW(2), 10)
    CALL(evt_eff, 0, STRING("kemuri_test"), 4, LW(0), LW(1), LW(2), FLOAT(0.7998047f), 0, 0, 0, 0, 0, 0, 0)
    CALL(btlevtcmd_GetCoin, LW(10), LW(0))
    CALL(btlevtcmd_StoreCoin, LW(0))
    CALL(btlevtcmd_GetExp, LW(10), LW(0))
    CALL(btlevtcmd_StoreExp, LW(0))
    CALL(btlevtcmd_GetPos, LW(10), LW(3), LW(1), LW(2))
    ADD(LW(2), 10)
    ADD(LW(1), 30)
    CALL(_add_star_point_disp_offset, LW(10), LW(3), LW(1), LW(2))
    CALL(evt_eff, STRING(""), STRING("star_point"), 0, LW(3), LW(1), LW(2), LW(0), 0, 0, 0, 0, 0, 0, 0)
    IF_INT_GREATER_OR_EQUAL(LW(0), 1)
        LOOP(LW(0))
        END_LOOP
    END_IF
    WAIT_FRAMES(30)
    CALL(btlevtcmd_GetBackItem, LW(10))
    CALL(btlevtcmd_CheckDamageCode, LW(10), 1024, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_KillUnit, LW(10), 1)
    ELSE
        CALL(btlevtcmd_KillUnit, LW(10), 2)
    END_IF
    RETURN
EVT_END()
#endif

EVT_BEGIN(btldefaultevt_avoid_guard)
    CALL(btlevtcmd_AnimeCmpPoseType, LW(10), LW(11), 42, 40, LW(2))
    IF_INT_EQUAL(LW(2), 0)
        CALL(btlevtcmd_AnimeSetMotionSpeed, LW(10), LW(11), FLOAT(2.0f))
    END_IF
    CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), 42)
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 20)
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 20, -1, 0)
    CALL(btlevtcmd_AnimeSetMotionSpeed, LW(10), LW(11), FLOAT(1.0f))
    CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), 46)
    CALL(btlevtcmd_WaitAttackEnd)
    IF_INT_EQUAL(LW(14), LW(15))
        CALL(btlevtcmd_AnimeSetMotionSpeed, LW(10), LW(11), FLOAT(2.0f))
    END_IF
    CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), 42)
    CALL(btlevtcmd_GetHomePos, -2, LW(2), LW(3), LW(4))
    CALL(btlevtcmd_MovePosition, -2, LW(2), LW(3), LW(4), 20, -1, 0)
    CALL(btlevtcmd_AnimeSetMotionSpeed, LW(10), LW(11), FLOAT(1.0f))
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

#ifdef VERSION_G8ME01
EVT_BEGIN(subsetevt_damage)
    CALL(btlevtcmd_CheckDamageCode, LW(10), 2097152, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_RunDataEventChild, LW(10), 13)
        RETURN
    END_IF
    CALL(btlevtcmd_CheckDamageCode, LW(10), 4194304, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_RunDataEventChild, LW(10), 14)
        RETURN
    END_IF
    CALL(btlevtcmd_CheckDamagePattern, LW(10), 9, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(0), 17)
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), 17)
            RETURN
        END_IF
    END_IF
    CALL(btlevtcmd_CheckDamagePattern, LW(10), 8, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(0), 16)
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), 16)
            RETURN
        END_IF
    END_IF
    CALL(btlevtcmd_CheckDamagePattern, LW(10), 14, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(0), 18)
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), 18)
            RETURN
        END_IF
    END_IF
    CALL(btlevtcmd_CheckDamagePattern, LW(10), 16, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(0), 18)
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), 18)
            RETURN
        END_IF
    END_IF
    CALL(btlevtcmd_CheckDamagePattern, LW(10), 15, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(0), 19)
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), 19)
            RETURN
        END_IF
    END_IF
    CALL(btlevtcmd_CheckDamagePattern, LW(10), 17, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(0), 19)
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), 19)
            RETURN
        END_IF
    END_IF
    CALL(btlevtcmd_CheckDamagePattern, LW(10), 21, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(0), 20)
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), 20)
            RETURN
        END_IF
    END_IF
    CALL(btlevtcmd_CheckDamagePattern, LW(10), 22, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(0), 21)
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), 21)
            RETURN
        END_IF
    END_IF
    CALL(btlevtcmd_CheckDamagePattern, LW(10), 23, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(0), 22)
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), 22)
            RETURN
        END_IF
    END_IF
    CALL(btlevtcmd_CheckDamagePattern, LW(10), 24, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(0), 23)
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), 23)
            RETURN
        END_IF
    END_IF
    CALL(btlevtcmd_CheckDamagePattern, LW(10), 25, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(subsetevt_blow_damage)
        CALL(btlevtcmd_SetRotate, LW(10), 0, 0, 0)
        RETURN
    END_IF
    CALL(btlevtcmd_CheckDamagePattern, LW(10), 4, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(0), 55)
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), 55)
            RETURN
        END_IF
    END_IF
    CALL(btlevtcmd_CheckDamagePattern, LW(10), 5, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(0), 56)
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), 56)
            RETURN
        END_IF
    END_IF
    CALL(btlevtcmd_CheckDamagePattern, LW(10), 6, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(0), 57)
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), 57)
            RETURN
        END_IF
    END_IF
    CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(0), 9)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_RunDataEventChild, LW(10), 9)
        RETURN
    END_IF
    CALL(btlevtcmd_CheckDamagePattern, LW(10), 4, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(subsetevt_swallow_failed_damage)
        RETURN
    END_IF
    CALL(btlevtcmd_CheckDamagePattern, LW(10), 5, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(subsetevt_swallow_spew_damage)
        RETURN
    END_IF
    CALL(btlevtcmd_CheckDamagePattern, LW(10), 6, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(subsetevt_swallow_shot_damage)
        RETURN
    END_IF
    CALL(btlevtcmd_GetDamage, LW(10), LW(0))
    IF_INT_GREATER_OR_EQUAL(LW(0), 1)
        CALL(btlevtcmd_CheckDamageCode, LW(10), 256, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_CheckDamagePattern, LW(10), 10, LW(0))
            IF_INT_NOT_EQUAL(LW(0), 0)
                GOTO(10)
            END_IF
            CALL(btlevtcmd_CheckDamagePattern, LW(10), 12, LW(0))
            IF_INT_NOT_EQUAL(LW(0), 0)
                GOTO(10)
            END_IF
            CALL(btlevtcmd_CheckDamagePattern, LW(10), 11, LW(0))
            IF_INT_NOT_EQUAL(LW(0), 0)
                GOTO(10)
            END_IF
            GOTO(11)
            LABEL(10)
            CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), LW(14))
            BEGIN_SIBLING_EVENT_GET_ID(LW(0))
                CALL(evt_sub_intpl_init_float, 5, FLOAT(1.0f), FLOAT(0.14941406f), 10)
                LOOP(11)
                    CALL(evt_sub_intpl_get_float, LW(5), EVT_NULLPTR)
                    CALL(btlevtcmd_GetScale, LW(10), LW(0), LW(1), LW(2))
                    CALL(btlevtcmd_SetScale, LW(10), LW(0), LW(5), LW(2))
                    WAIT_FRAMES(1)
                END_LOOP
            END_SIBLING_EVENT
            CALL(btlevtcmd_WaitEventEnd, LW(0))
            CALL(btlevtcmd_CheckDamagePattern, LW(10), 11, LW(1))
            IF_INT_NOT_EQUAL(LW(1), 0)
                WAIT_MS(3000)
                ELSE
                CALL(btlevtcmd_CheckDamagePattern, LW(10), 12, LW(1))
                IF_INT_NOT_EQUAL(LW(1), 0)
                    WAIT_MS(2500)
                    ELSE
                    WAIT_MS(1500)
                END_IF
            END_IF
            BEGIN_SIBLING_EVENT
                CALL(btlevtcmd_GetFriendBelong, LW(10), LW(0))
                IF_INT_EQUAL(LW(0), 0)
                    CALL(btlevtcmd_snd_se, LW(10), STRING("SFX_BTL_DAMAGE_PRESS1"), EVT_NULLPTR, 0, EVT_NULLPTR)
                END_IF
            END_SIBLING_EVENT
            BEGIN_SIBLING_EVENT
                CALL(evt_sub_intpl_init_float, 5, FLOAT(0.14941406f), FLOAT(1.25f), 10)
                LOOP(10)
                    CALL(evt_sub_intpl_get_float, LW(5), EVT_NULLPTR)
                    CALL(btlevtcmd_GetScale, LW(10), LW(0), LW(1), LW(2))
                    CALL(btlevtcmd_SetScale, LW(10), LW(0), LW(5), LW(2))
                    WAIT_FRAMES(1)
                END_LOOP
                CALL(evt_sub_intpl_init_float, 7, FLOAT(1.25f), FLOAT(1.0f), 30)
                LOOP(30)
                    CALL(evt_sub_intpl_get_float, LW(5), EVT_NULLPTR)
                    CALL(btlevtcmd_GetScale, LW(10), LW(0), LW(1), LW(2))
                    CALL(btlevtcmd_SetScale, LW(10), LW(0), LW(5), LW(2))
                    WAIT_FRAMES(1)
                END_LOOP
            END_SIBLING_EVENT
            WAIT_FRAMES(60)
            RETURN
            LABEL(11)
            CALL(btlevtcmd_CheckDamagePattern, LW(10), 14, LW(0))
            IF_INT_NOT_EQUAL(LW(0), 0)
                CALL_CHILD(subsetevt_shot_damage)
                RETURN
            END_IF
        END_IF
        CALL(btlevtcmd_CheckDamagePattern, LW(10), 16, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_shot_damage)
            RETURN
        END_IF
        CALL(btlevtcmd_CheckDamagePattern, LW(10), 21, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_binta_damage_left)
            RETURN
        END_IF
        CALL(btlevtcmd_CheckDamagePattern, LW(10), 22, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_binta_damage_right)
            RETURN
        END_IF
        CALL(btlevtcmd_CheckDamagePattern, LW(10), 23, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_binta_damage_left_last)
            RETURN
        END_IF
        CALL(btlevtcmd_CheckDamagePattern, LW(10), 24, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_binta_damage_right_last)
            RETURN
        END_IF
        CALL(btlevtcmd_CheckDamagePattern, LW(10), 13, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_AnimeFlagOnOff, LW(10), LW(11), 32, 1)
            CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), LW(14))
            WAIT_MS(2000)
            CALL(btlevtcmd_AnimeFlagOnOff, LW(10), LW(11), 32, 0)
            ELSE
            CALL(btlevtcmd_CheckDamagePattern, LW(10), 26, LW(0))
            IF_INT_NOT_EQUAL(LW(0), 0)
                CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), LW(14))
                WAIT_MS(2000)
                IF_INT_NOT_EQUAL(LW(15), -1)
                    CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), LW(15))
                    WAIT_FRAMES(30)
                END_IF
                ELSE
                CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), LW(14))
                WAIT_MS(1000)
                IF_INT_NOT_EQUAL(LW(15), -1)
                    CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), LW(15))
                    WAIT_FRAMES(30)
                END_IF
            END_IF
        END_IF
        RETURN
        ELSE
        CALL(btlevtcmd_CheckDamageCode, LW(10), 256, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(_set_hp_gauge_fade_flag, LW(10))
        END_IF
        CALL(btlevtcmd_GetDamageCode, LW(10), LW(0))
        SWITCH(LW(0))
            CASE_EQUAL(23)
            CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), LW(14))
            WAIT_MS(1000)
            IF_INT_NOT_EQUAL(LW(15), -1)
                CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), LW(15))
                WAIT_FRAMES(30)
            END_IF
            RETURN
            CASE_DEFAULT
            CALL(evt_snd_sfxon, STRING("SFX_BTL_ATTACK_MISS2"), 0)
            CALL(btlevtcmd_SetDispOffset, LW(10), -2, 0, 0)
            WAIT_FRAMES(1)
            CALL(btlevtcmd_SetDispOffset, LW(10), -1, 0, 0)
            WAIT_FRAMES(1)
            CALL(btlevtcmd_SetDispOffset, LW(10), 0, 0, 0)
            WAIT_FRAMES(1)
            CALL(btlevtcmd_SetDispOffset, LW(10), -2, 0, 0)
            WAIT_FRAMES(1)
            CALL(btlevtcmd_SetDispOffset, LW(10), -4, 0, 0)
            WAIT_FRAMES(1)
            CALL(btlevtcmd_SetDispOffset, LW(10), -2, 0, 0)
            WAIT_FRAMES(1)
            CALL(btlevtcmd_SetDispOffset, LW(10), 0, 0, 0)
            WAIT_FRAMES(1)
            CALL(btlevtcmd_SetDispOffset, LW(10), -1, 0, 0)
            WAIT_FRAMES(1)
            CALL(btlevtcmd_SetDispOffset, LW(10), -2, 0, 0)
            WAIT_FRAMES(1)
            CALL(btlevtcmd_SetDispOffset, LW(10), -1, 0, 0)
            WAIT_FRAMES(1)
            CALL(btlevtcmd_SetDispOffset, LW(10), 0, 0, 0)
            WAIT_FRAMES(1)
            CALL(btlevtcmd_SetDispOffset, LW(10), -1, 0, 0)
            WAIT_FRAMES(1)
            CALL(btlevtcmd_SetDispOffset, LW(10), -3, 0, 0)
            WAIT_FRAMES(1)
            CALL(btlevtcmd_SetDispOffset, LW(10), -1, 0, 0)
            WAIT_FRAMES(1)
            CALL(btlevtcmd_SetDispOffset, LW(10), 0, 0, 0)
            WAIT_FRAMES(1)
            CALL(btlevtcmd_SetDispOffset, LW(10), -1, 0, 0)
            WAIT_FRAMES(1)
            CALL(btlevtcmd_SetDispOffset, LW(10), -2, 0, 0)
            WAIT_FRAMES(1)
            CALL(btlevtcmd_SetDispOffset, LW(10), -1, 0, 0)
            WAIT_FRAMES(1)
            CALL(btlevtcmd_SetDispOffset, LW(10), 0, 0, 0)
            WAIT_FRAMES(1)
            CALL(btlevtcmd_SetDispOffset, LW(10), 0, 0, 0)
            WAIT_FRAMES(1)
            CALL(btlevtcmd_SetDispOffset, LW(10), -1, 0, 0)
            WAIT_FRAMES(1)
            CALL(btlevtcmd_SetDispOffset, LW(10), -1, 0, 0)
            WAIT_FRAMES(1)
            CALL(btlevtcmd_SetDispOffset, LW(10), 0, 0, 0)
            WAIT_FRAMES(1)
            CALL(btlevtcmd_SetDispOffset, LW(10), 0, 0, 0)
            WAIT_FRAMES(1)
        END_SWITCH
    END_IF
    RETURN
EVT_END()
#else
EVT_BEGIN(subsetevt_damage)
    CALL(btlevtcmd_CheckDamageCode, LW(10), 2097152, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_RunDataEventChild, LW(10), 13)
        RETURN
    END_IF
    CALL(btlevtcmd_CheckDamageCode, LW(10), 4194304, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_RunDataEventChild, LW(10), 14)
        RETURN
    END_IF
    CALL(btlevtcmd_CheckDamagePattern, LW(10), 9, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(0), 17)
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), 17)
            RETURN
        END_IF
    END_IF
    CALL(btlevtcmd_CheckDamagePattern, LW(10), 8, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(0), 16)
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), 16)
            RETURN
        END_IF
    END_IF
    CALL(btlevtcmd_CheckDamagePattern, LW(10), 14, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(0), 18)
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), 18)
            RETURN
        END_IF
    END_IF
    CALL(btlevtcmd_CheckDamagePattern, LW(10), 16, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(0), 18)
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), 18)
            RETURN
        END_IF
    END_IF
    CALL(btlevtcmd_CheckDamagePattern, LW(10), 15, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(0), 19)
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), 19)
            RETURN
        END_IF
    END_IF
    CALL(btlevtcmd_CheckDamagePattern, LW(10), 17, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(0), 19)
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), 19)
            RETURN
        END_IF
    END_IF
    CALL(btlevtcmd_CheckDamagePattern, LW(10), 21, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(0), 20)
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), 20)
            RETURN
        END_IF
    END_IF
    CALL(btlevtcmd_CheckDamagePattern, LW(10), 22, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(0), 21)
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), 21)
            RETURN
        END_IF
    END_IF
    CALL(btlevtcmd_CheckDamagePattern, LW(10), 23, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(0), 22)
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), 22)
            RETURN
        END_IF
    END_IF
    CALL(btlevtcmd_CheckDamagePattern, LW(10), 24, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(0), 23)
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), 23)
            RETURN
        END_IF
    END_IF
    CALL(btlevtcmd_CheckDamagePattern, LW(10), 25, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(subsetevt_blow_damage)
        CALL(btlevtcmd_SetRotate, LW(10), 0, 0, 0)
        RETURN
    END_IF
    CALL(btlevtcmd_CheckDamagePattern, LW(10), 4, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(0), 55)
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), 55)
            RETURN
        END_IF
    END_IF
    CALL(btlevtcmd_CheckDamagePattern, LW(10), 5, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(0), 56)
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), 56)
            RETURN
        END_IF
    END_IF
    CALL(btlevtcmd_CheckDamagePattern, LW(10), 6, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(0), 57)
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), 57)
            RETURN
        END_IF
    END_IF
    CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(0), 9)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_RunDataEventChild, LW(10), 9)
        RETURN
    END_IF
    CALL(btlevtcmd_CheckDamagePattern, LW(10), 4, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(subsetevt_swallow_failed_damage)
        RETURN
    END_IF
    CALL(btlevtcmd_CheckDamagePattern, LW(10), 5, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(subsetevt_swallow_spew_damage)
        RETURN
    END_IF
    CALL(btlevtcmd_CheckDamagePattern, LW(10), 6, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(subsetevt_swallow_shot_damage)
        RETURN
    END_IF
    CALL(btlevtcmd_GetDamage, LW(10), LW(0))
    IF_INT_GREATER_OR_EQUAL(LW(0), 1)
        CALL(btlevtcmd_CheckDamageCode, LW(10), 256, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_CheckDamagePattern, LW(10), 10, LW(0))
            IF_INT_NOT_EQUAL(LW(0), 0)
                GOTO(10)
            END_IF
            CALL(btlevtcmd_CheckDamagePattern, LW(10), 12, LW(0))
            IF_INT_NOT_EQUAL(LW(0), 0)
                GOTO(10)
            END_IF
            CALL(btlevtcmd_CheckDamagePattern, LW(10), 11, LW(0))
            IF_INT_NOT_EQUAL(LW(0), 0)
                GOTO(10)
            END_IF
            GOTO(11)
            LABEL(10)
            CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), LW(14))
            BEGIN_SIBLING_EVENT_GET_ID(LW(0))
                CALL(evt_sub_intpl_init_float, 5, FLOAT(1.0f), FLOAT(0.14941406f), 10)
                LOOP(11)
                    CALL(evt_sub_intpl_get_float, LW(5), EVT_NULLPTR)
                    CALL(btlevtcmd_GetScale, LW(10), LW(0), LW(1), LW(2))
                    CALL(btlevtcmd_SetScale, LW(10), LW(0), LW(5), LW(2))
                    WAIT_FRAMES(1)
                END_LOOP
            END_SIBLING_EVENT
            CALL(btlevtcmd_WaitEventEnd, LW(0))
            CALL(btlevtcmd_CheckDamagePattern, LW(10), 11, LW(1))
            IF_INT_NOT_EQUAL(LW(1), 0)
                WAIT_MS(3000)
            ELSE
                CALL(btlevtcmd_CheckDamagePattern, LW(10), 12, LW(1))
                IF_INT_NOT_EQUAL(LW(1), 0)
                    WAIT_MS(2500)
                ELSE
                    WAIT_MS(1500)
                END_IF
            END_IF
            BEGIN_SIBLING_EVENT
                CALL(btlevtcmd_GetFriendBelong, LW(10), LW(0))
                IF_INT_EQUAL(LW(0), 0)
                    CALL(btlevtcmd_snd_se, LW(10), STRING("SFX_BTL_DAMAGE_PRESS1"), EVT_NULLPTR, 0, EVT_NULLPTR)
                END_IF
            END_SIBLING_EVENT
            BEGIN_SIBLING_EVENT
                CALL(evt_sub_intpl_init_float, 5, FLOAT(0.14941406f), FLOAT(1.25f), 10)
                LOOP(10)
                    CALL(evt_sub_intpl_get_float, LW(5), EVT_NULLPTR)
                    CALL(btlevtcmd_GetScale, LW(10), LW(0), LW(1), LW(2))
                    CALL(btlevtcmd_SetScale, LW(10), LW(0), LW(5), LW(2))
                    WAIT_FRAMES(1)
                END_LOOP
                CALL(evt_sub_intpl_init_float, 7, FLOAT(1.25f), FLOAT(1.0f), 30)
                LOOP(30)
                    CALL(evt_sub_intpl_get_float, LW(5), EVT_NULLPTR)
                    CALL(btlevtcmd_GetScale, LW(10), LW(0), LW(1), LW(2))
                    CALL(btlevtcmd_SetScale, LW(10), LW(0), LW(5), LW(2))
                    WAIT_FRAMES(1)
                END_LOOP
            END_SIBLING_EVENT
            WAIT_FRAMES(60)
            RETURN
            LABEL(11)
            CALL(btlevtcmd_CheckDamagePattern, LW(10), 14, LW(0))
            IF_INT_NOT_EQUAL(LW(0), 0)
                CALL_CHILD(subsetevt_shot_damage)
                RETURN
            END_IF
        END_IF
        CALL(btlevtcmd_CheckDamagePattern, LW(10), 16, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_shot_damage)
            RETURN
        END_IF
        CALL(btlevtcmd_CheckDamagePattern, LW(10), 21, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_binta_damage_left)
            RETURN
        END_IF
        CALL(btlevtcmd_CheckDamagePattern, LW(10), 22, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_binta_damage_right)
            RETURN
        END_IF
        CALL(btlevtcmd_CheckDamagePattern, LW(10), 23, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_binta_damage_left_last)
            RETURN
        END_IF
        CALL(btlevtcmd_CheckDamagePattern, LW(10), 24, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_binta_damage_right_last)
            RETURN
        END_IF
        CALL(btlevtcmd_CheckDamagePattern, LW(10), 13, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_AnimeFlagOnOff, LW(10), LW(11), 32, 1)
            CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), LW(14))
            WAIT_MS(2000)
            CALL(btlevtcmd_AnimeFlagOnOff, LW(10), LW(11), 32, 0)
        ELSE
            CALL(btlevtcmd_CheckDamagePattern, LW(10), 26, LW(0))
            IF_INT_NOT_EQUAL(LW(0), 0)
                CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), LW(14))
                WAIT_MS(2000)
                IF_INT_NOT_EQUAL(LW(15), -1)
                    CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), LW(15))
                    WAIT_FRAMES(30)
                END_IF
            ELSE
                CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), LW(14))
                WAIT_MS(1000)
                IF_INT_NOT_EQUAL(LW(15), -1)
                    CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), LW(15))
                    WAIT_FRAMES(30)
                END_IF
            END_IF
        END_IF
        RETURN
    ELSE
        CALL(btlevtcmd_CheckDamageCode, LW(10), 256, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(_set_hp_gauge_fade_flag, LW(10))
        END_IF
        CALL(btlevtcmd_GetDamageCode, LW(10), LW(0))
        SWITCH(LW(0))
            CASE_EQUAL(23)
            CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), LW(14))
            WAIT_MS(1000)
            IF_INT_NOT_EQUAL(LW(15), -1)
                CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), LW(15))
                WAIT_FRAMES(30)
            END_IF
            RETURN
            CASE_DEFAULT
            CALL(evt_snd_sfxon, STRING("SFX_BTL_ATTACK_MISS2"), 0)
            CALL(btlevtcmd_SetDispOffset, LW(10), -2, 0, 0)
            WAIT_FRAMES(1)
            CALL(btlevtcmd_SetDispOffset, LW(10), -1, 0, 0)
            WAIT_FRAMES(1)
            CALL(btlevtcmd_SetDispOffset, LW(10), 0, 0, 0)
            WAIT_FRAMES(1)
            CALL(btlevtcmd_SetDispOffset, LW(10), -2, 0, 0)
            WAIT_FRAMES(1)
            CALL(btlevtcmd_SetDispOffset, LW(10), -4, 0, 0)
            WAIT_FRAMES(1)
            CALL(btlevtcmd_SetDispOffset, LW(10), -2, 0, 0)
            WAIT_FRAMES(1)
            CALL(btlevtcmd_SetDispOffset, LW(10), 0, 0, 0)
            WAIT_FRAMES(1)
            CALL(btlevtcmd_SetDispOffset, LW(10), -1, 0, 0)
            WAIT_FRAMES(1)
            CALL(btlevtcmd_SetDispOffset, LW(10), -2, 0, 0)
            WAIT_FRAMES(1)
            CALL(btlevtcmd_SetDispOffset, LW(10), -1, 0, 0)
            WAIT_FRAMES(1)
            CALL(btlevtcmd_SetDispOffset, LW(10), 0, 0, 0)
            WAIT_FRAMES(1)
            CALL(btlevtcmd_SetDispOffset, LW(10), -1, 0, 0)
            WAIT_FRAMES(1)
            CALL(btlevtcmd_SetDispOffset, LW(10), -3, 0, 0)
            WAIT_FRAMES(1)
            CALL(btlevtcmd_SetDispOffset, LW(10), -1, 0, 0)
            WAIT_FRAMES(1)
            CALL(btlevtcmd_SetDispOffset, LW(10), 0, 0, 0)
            WAIT_FRAMES(1)
            CALL(btlevtcmd_SetDispOffset, LW(10), -1, 0, 0)
            WAIT_FRAMES(1)
            CALL(btlevtcmd_SetDispOffset, LW(10), -2, 0, 0)
            WAIT_FRAMES(1)
            CALL(btlevtcmd_SetDispOffset, LW(10), -1, 0, 0)
            WAIT_FRAMES(1)
            CALL(btlevtcmd_SetDispOffset, LW(10), 0, 0, 0)
            WAIT_FRAMES(1)
            CALL(btlevtcmd_SetDispOffset, LW(10), 0, 0, 0)
            WAIT_FRAMES(1)
            CALL(btlevtcmd_SetDispOffset, LW(10), -1, 0, 0)
            WAIT_FRAMES(1)
            CALL(btlevtcmd_SetDispOffset, LW(10), -1, 0, 0)
            WAIT_FRAMES(1)
            CALL(btlevtcmd_SetDispOffset, LW(10), 0, 0, 0)
            WAIT_FRAMES(1)
            CALL(btlevtcmd_SetDispOffset, LW(10), 0, 0, 0)
            WAIT_FRAMES(1)
        END_SWITCH
    END_IF
    RETURN
EVT_END()
#endif

#ifdef VERSION_G8ME01
EVT_BEGIN(subsetevt_counter_damage)
    CALL(btlevtcmd_OnUnitFlag, LW(10), 1)
    BEGIN_SIBLING_EVENT
        SWITCH(LW(0))
            CASE_OR_EQUAL(43)
            CASE_OR_EQUAL(44)
            CASE_OR_EQUAL(45)
            CALL(btlevtcmd_GetUnitKind, LW(10), LW(2))
            IF_INT_EQUAL(LW(2), 222)
                CALL(btlevtcmd_GetPos, LW(10), LW(2), LW(3), LW(4))
                CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_DAMAGED1"), LW(2), LW(3), LW(4), 0)
            END_IF
            CASE_END
        END_SWITCH
    END_SIBLING_EVENT
    SWITCH(LW(0))
        CASE_EQUAL(43)
        CALL(evt_snd_sfxon, STRING("SFX_BTL_ATTACK_MISS3"), 0)
        SET_VALUE(LW(14), 36)
        CASE_EQUAL(46)
        CALL(btlevtcmd_GetUnitKind, LW(10), LW(2))
        IF_INT_EQUAL(LW(2), 222)
            CALL(btlevtcmd_snd_se, LW(10), STRING("SFX_VOICE_MARIO_DAMAGED1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        END_IF
        SET_VALUE(LW(14), 29)
        CASE_OR_EQUAL(49)
        CASE_OR_EQUAL(50)
        CASE_OR_EQUAL(54)
        SET_VALUE(LW(14), 29)
        CASE_END
        CASE_DEFAULT
        SET_VALUE(LW(14), 39)
    END_SWITCH
    CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), LW(14))
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_RunDataEventChild, LW(10), 10)
    END_SIBLING_EVENT
    CALL(btlevtcmd_SetFallAccel, LW(10), FLOAT(1.0f))
    CALL(btlevtcmd_GetPos, LW(10), LW(2), LW(3), LW(4))
    CALL(btlevtcmd_FaceDirectionSub, LW(10), LW(2), 25)
    ADD(LW(3), 25)
    CALL(btlevtcmd_DivePosition, LW(10), LW(2), LW(3), LW(4), 10, 0, 0, 0, -1)
    WAIT_MS(500)
    CALL(btlevtcmd_SetFallAccel, LW(10), FLOAT(1.0f))
    CALL(btlevtcmd_GetPos, LW(10), LW(2), LW(3), LW(4))
    CALL(btlevtcmd_TransStageFloorPosition, LW(2), LW(3), LW(4))
    CALL(btlevtcmd_FaceDirectionSub, LW(10), LW(2), 30)
    CALL(btlevtcmd_JumpPosition, LW(10), LW(2), LW(3), LW(4), 20, -1)
    CALL(btlevtcmd_FaceDirectionSub, LW(10), LW(2), 15)
    CALL(btlevtcmd_JumpPosition, LW(10), LW(2), LW(3), LW(4), 10, -1)
    WAIT_FRAMES(30)
    CALL(evt_btl_camera_set_mode, 0, 0)
    RETURN
EVT_END()
#else
EVT_BEGIN(subsetevt_counter_damage)
    CALL(btlevtcmd_OnUnitFlag, LW(10), 1)
    BEGIN_SIBLING_EVENT
        SWITCH(LW(0))
            CASE_OR_EQUAL(43)
            CASE_OR_EQUAL(44)
            CASE_OR_EQUAL(45)
            CALL(btlevtcmd_GetUnitKind, LW(10), LW(2))
            IF_INT_EQUAL(LW(2), 222)
                CALL(btlevtcmd_GetPos, LW(10), LW(2), LW(3), LW(4))
                CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_DAMAGED1"), LW(2), LW(3), LW(4), 0)
            END_IF
            CASE_END
        END_SWITCH
    END_SIBLING_EVENT
    SWITCH(LW(0))
        CASE_EQUAL(43)
        CALL(evt_snd_sfxon, STRING("SFX_BTL_ATTACK_MISS3"), 0)
        SET_VALUE(LW(14), 36)
        CASE_EQUAL(46)
        CALL(btlevtcmd_GetUnitKind, LW(10), LW(2))
        IF_INT_EQUAL(LW(2), 222)
            CALL(btlevtcmd_snd_se, LW(10), STRING("SFX_VOICE_MARIO_DAMAGED1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        END_IF
        SET_VALUE(LW(14), 29)
        CASE_OR_EQUAL(49)
        CASE_OR_EQUAL(50)
        CASE_OR_EQUAL(54)
        SET_VALUE(LW(14), 29)
        CASE_END
        CASE_DEFAULT
        SET_VALUE(LW(14), 39)
    END_SWITCH
    CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), LW(14))
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_RunDataEventChild, LW(10), 10)
    END_SIBLING_EVENT
    CALL(btlevtcmd_SetFallAccel, LW(10), FLOAT(1.0f))
    CALL(btlevtcmd_GetPos, LW(10), LW(2), LW(3), LW(4))
    CALL(btlevtcmd_FaceDirectionSub, LW(10), LW(2), 25)
    ADD(LW(3), 25)
    CALL(btlevtcmd_DivePosition, LW(10), LW(2), LW(3), LW(4), 10, 0, 0, 0, -1)
    WAIT_MS(500)
    CALL(btlevtcmd_SetFallAccel, LW(10), FLOAT(1.0f))
    CALL(btlevtcmd_GetPos, LW(10), LW(2), LW(3), LW(4))
    CALL(btlevtcmd_TransStageFloorPosition, LW(2), LW(3), LW(4))
    CALL(btlevtcmd_FaceDirectionSub, LW(10), LW(2), 30)
    CALL(btlevtcmd_JumpPosition, LW(10), LW(2), LW(3), LW(4), 20, -1)
    CALL(btlevtcmd_FaceDirectionSub, LW(10), LW(2), 15)
    CALL(btlevtcmd_JumpPosition, LW(10), LW(2), LW(3), LW(4), 10, -1)
    WAIT_FRAMES(30)
    CALL(evt_btl_camera_set_mode, 0, 0)
    RETURN
EVT_END()
#endif

EVT_BEGIN(btldefaultevt_ChangePosition)
    CALL(btlevtcmd_GetGuardKouraId, -3, LW(10))
    IF_INT_NOT_EQUAL(LW(10), -1)
        CALL(btlevtcmd_RunDataEventChild, LW(10), 60)
        CALL(btlevtcmd_OffPartsAttribute, -3, 1, 16777216)
        CALL(btlevtcmd_SetUnitWork, LW(10), 2, 2)
    END_IF
    CALL(evt_snd_sfxon, STRING("SFX_BTL_POS_CHANGE1"), 0)
    CALL(btlevtcmd_GetPartnerId, -2, LW(3))
    CALL(btlevtcmd_GetBodyId, -2, LW(5))
    CALL(btlevtcmd_CheckStatus, -2, 27, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_AnimeChangePoseType, -2, LW(5), 42)
    END_IF
    CALL(btlevtcmd_GetBodyId, LW(3), LW(5))
    CALL(btlevtcmd_CheckStatus, LW(3), 27, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_AnimeChangePoseType, LW(3), LW(5), 42)
    END_IF
    BEGIN_INLINE_EVENT
        CALL(btlevtcmd_GetHomePos, LW(3), LW(5), LW(6), LW(7))
        CALL(btlevtcmd_GetHomePos, -2, LW(5), LW(8), LW(7))
        CALL(btlevtcmd_EllipseMovePosition, LW(3), LW(5), LW(6), LW(7), 20, 15, 0)
        CALL(btlevtcmd_SetHomePos, LW(3), LW(5), LW(6), LW(7))
    END_INLINE_EVENT
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHomePos, LW(3), LW(0), LW(4), LW(2))
    CALL(btlevtcmd_EllipseMovePosition, -2, LW(0), LW(1), LW(2), 20, 15, 0)
    CALL(btlevtcmd_SetHomePos, -2, LW(0), LW(1), LW(2))
    WAIT_FRAMES(2)
    CALL(btlevtcmd_GetGuardKouraId, -3, LW(10))
    IF_INT_NOT_EQUAL(LW(10), -1)
        CALL(btlevtcmd_SetUnitWork, LW(10), 2, 3)
        LABEL(10)
        CALL(btlevtcmd_GetUnitWork, LW(10), 2, LW(0))
        IF_INT_EQUAL(LW(0), 3)
            WAIT_FRAMES(1)
            GOTO(10)
        END_IF
        CALL(btlevtcmd_GetHp, LW(10), LW(0))
        IF_INT_GREATER(LW(0), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), 59)
            CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
            CALL(btlevtcmd_SetHomePos, LW(10), LW(0), LW(1), LW(2))
        END_IF
    END_IF
    CALL(btlevtcmd_SortPhaseMoveTable)
    CALL(btlevtcmd_StartWaitEvent, -3)
    CALL(btlevtcmd_StartWaitEvent, -4)
    RETURN
EVT_END()

EVT_BEGIN(subsetevt_happy_heart)
    CALL(btlevtcmd_GetBodyId, -2, LW(5))
    CALL(btlevtcmd_CheckDefinePosetype, -2, LW(5), 62, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckStatus, LW(3), 9, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_CheckStatus, LW(3), 1, LW(0))
            IF_INT_EQUAL(LW(0), 0)
                CALL(btlevtcmd_CheckStatus, LW(3), 2, LW(0))
                IF_INT_EQUAL(LW(0), 0)
                    CALL(btlevtcmd_AnimeChangePoseType, -2, LW(5), 62)
                END_IF
            END_IF
        END_IF
    END_IF
    WAIT_FRAMES(60)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

EVT_BEGIN(subsetevt_happy_flower)
    CALL(btlevtcmd_GetBodyId, -2, LW(5))
    CALL(btlevtcmd_CheckDefinePosetype, -2, LW(5), 63, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckStatus, LW(3), 9, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_CheckStatus, LW(3), 1, LW(0))
            IF_INT_EQUAL(LW(0), 0)
                CALL(btlevtcmd_CheckStatus, LW(3), 2, LW(0))
                IF_INT_EQUAL(LW(0), 0)
                    CALL(btlevtcmd_AnimeChangePoseType, -2, LW(5), 63)
                END_IF
            END_IF
        END_IF
    END_IF
    WAIT_FRAMES(60)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

#ifdef VERSION_G8ME01
EVT_BEGIN(subsetevt_terror)
    CALL(btlevtcmd_JumpSetting, -2, 20, FLOAT(0.0f), FLOAT(0.69921875f))
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 16, -1)
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(10.0f))
    CALL(btlevtcmd_GetStageSize, LW(0), EVT_NULLPTR, EVT_NULLPTR)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_ESCAPE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL(btlevtcmd_KillUnit, -2, 0)
    RETURN
EVT_END()
#else
EVT_BEGIN(subsetevt_terror)
    CALL(btlevtcmd_JumpSetting, -2, 20, FLOAT(0.0f), FLOAT(0.69921875f))
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 16, -1)
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(10.0f))
    CALL(btlevtcmd_GetStageSize, LW(0), EVT_NULLPTR, EVT_NULLPTR)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_ESCAPE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL(btlevtcmd_KillUnit, -2, 0)
    RETURN
EVT_END()
#endif

#ifdef VERSION_G8ME01
EVT_BEGIN(subsetevt_blow_damage)
    CALL(evt_snd_sfxon, STRING("SFX_BTL_CLAUD_BREATH3"), 0)
    CALL(btlevtcmd_GetBodyId, -2, LW(0))
    CALL(btlevtcmd_AnimeChangePoseType, -2, LW(0), 39)
    BEGIN_SIBLING_EVENT
        LABEL(0)
        CALL(btlevtcmd_AddRotate, -2, 0, 20, 0)
        WAIT_FRAMES(1)
        GOTO(0)
    END_SIBLING_EVENT
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(2.0f))
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetStageSize, LW(0), EVT_NULLPTR, EVT_NULLPTR)
    ADD(LW(1), 40)
    CALL(btlevtcmd_DivePosition, -2, LW(0), LW(1), LW(2), 0, -10, 2, 0, -1)
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPos, LW(10), LW(0), LW(1), LW(2))
    RETURN
EVT_END()
#else
EVT_BEGIN(subsetevt_blow_damage)
    CALL(evt_snd_sfxon, STRING("SFX_BTL_CLAUD_BREATH3"), 0)
    CALL(btlevtcmd_GetBodyId, -2, LW(0))
    CALL(btlevtcmd_AnimeChangePoseType, -2, LW(0), 39)
    BEGIN_SIBLING_EVENT
        LABEL(0)
        CALL(btlevtcmd_AddRotate, -2, 0, 20, 0)
        WAIT_FRAMES(1)
        GOTO(0)
    END_SIBLING_EVENT
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(2.0f))
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetStageSize, LW(0), EVT_NULLPTR, EVT_NULLPTR)
    ADD(LW(1), 40)
    CALL(btlevtcmd_DivePosition, -2, LW(0), LW(1), LW(2), 0, -10, 2, 0, -1)
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPos, LW(10), LW(0), LW(1), LW(2))
    RETURN
EVT_END()
#endif

#ifdef VERSION_G8ME01
EVT_BEGIN(subsetevt_blow_dead)
    CALL(evt_snd_sfxon, STRING("SFX_BTL_CLAUD_BREATH3"), 0)
    CALL(btlevtcmd_GetBodyId, -2, LW(0))
    CALL(btlevtcmd_AnimeChangePoseType, -2, LW(0), 39)
    BEGIN_SIBLING_EVENT
        LABEL(0)
        CALL(btlevtcmd_AddRotate, -2, 0, 20, 0)
        WAIT_FRAMES(1)
        GOTO(0)
    END_SIBLING_EVENT
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(2.0f))
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetStageSize, LW(0), EVT_NULLPTR, EVT_NULLPTR)
    ADD(LW(1), 40)
    CALL(btlevtcmd_DivePosition, -2, LW(0), LW(1), LW(2), 0, -10, 2, 0, -1)
    CALL(btlevtcmd_GetCoin, LW(10), LW(0))
    CALL(btlevtcmd_StoreCoin, LW(0))
    CALL(btlevtcmd_GetExp, LW(10), LW(0))
    CALL(btlevtcmd_StoreExp, LW(0))
    CALL(btlevtcmd_KillUnit, -2, 0)
    RETURN
EVT_END()
#else
EVT_BEGIN(subsetevt_blow_dead)
    CALL(evt_snd_sfxon, STRING("SFX_BTL_CLAUD_BREATH3"), 0)
    CALL(btlevtcmd_GetBodyId, -2, LW(0))
    CALL(btlevtcmd_AnimeChangePoseType, -2, LW(0), 39)
    BEGIN_SIBLING_EVENT
        LABEL(0)
        CALL(btlevtcmd_AddRotate, -2, 0, 20, 0)
        WAIT_FRAMES(1)
        GOTO(0)
    END_SIBLING_EVENT
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(2.0f))
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetStageSize, LW(0), EVT_NULLPTR, EVT_NULLPTR)
    ADD(LW(1), 40)
    CALL(btlevtcmd_DivePosition, -2, LW(0), LW(1), LW(2), 0, -10, 2, 0, -1)
    CALL(btlevtcmd_GetCoin, LW(10), LW(0))
    CALL(btlevtcmd_StoreCoin, LW(0))
    CALL(btlevtcmd_GetExp, LW(10), LW(0))
    CALL(btlevtcmd_StoreExp, LW(0))
    CALL(btlevtcmd_KillUnit, -2, 0)
    RETURN
EVT_END()
#endif

EVT_BEGIN(subsetevt_shot_damage)
    CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), LW(14))
    CALL(btlevtcmd_OnToken, LW(10), 32)
    CALL(btlevtcmd_CheckDamagePattern, LW(10), 14, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(LW(12), PTR(&marioWeapon_KaitenHammerInvolved))
        CALL_CHILD(PTR(subsetevt_shot_damage_kaiten))
        ELSE
        SET_VALUE(LW(12), PTR(&marioWeapon_UltraHammerInvolved))
        CALL_CHILD(PTR(subsetevt_shot_damage_ultra))
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(subsetevt_shot_damage_kaiten)
    BEGIN_SIBLING_EVENT_GET_ID(LW(13))
        LABEL(0)
        CALL(btlevtcmd_AddRotate, LW(10), 0, 30, 0)
        WAIT_FRAMES(1)
        GOTO(0)
    END_SIBLING_EVENT
    CALL(btlevtcmd_GetFriendBelong, LW(10), LW(0))
    CALL(btlevtcmd_SamplingEnemy, LW(10), LW(0), LW(12))
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    LABEL(1)
    IF_INT_EQUAL(LW(3), -1)
        GOTO(50)
    END_IF
    CALL(btlevtcmd_GetUnitId, LW(10), LW(0))
    IF_INT_NOT_EQUAL(LW(0), LW(3))
        CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
        GOTO(1)
    END_IF
    LABEL(10)
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(50)
    END_IF
    CALL(btlevtcmd_JumpSetting, LW(10), 0, FLOAT(5.0f), FLOAT(0.009765625f))
    CALL(btlevtcmd_GetPos, LW(10), LW(5), LW(6), LW(7))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    IF_INT_GREATER_OR_EQUAL(LW(5), LW(0))
        SET_VALUE(LW(8), LW(5))
        SUBTRACT(LW(8), LW(0))
    ELSE
        SET_VALUE(LW(8), LW(0))
        SUBTRACT(LW(8), LW(5))
    END_IF
    DIVIDE(LW(8), 2)
    ADD(LW(8), LW(6))
    IF_INT_LESS(LW(8), LW(1))
        SET_VALUE(LW(1), LW(8))
    END_IF
    CALL(btlevtcmd_JumpPosition, LW(10), LW(0), LW(1), LW(2), 0, -1)
    CALL(btlevtcmd_PreCheckDamage, LW(10), LW(3), LW(4), LW(12), 256, LW(5))
    SWITCH(LW(5))
        CASE_EQUAL(2)
        CALL(btlevtcmd_StartAvoid, LW(3), 40)
        CASE_EQUAL(3)
        CALL(btlevtcmd_StartAvoid, LW(3), 38)
        CASE_EQUAL(6)
        CALL(btlevtcmd_StartAvoid, LW(3), 39)
        CASE_EQUAL(4)
        CASE_EQUAL(1)
        CALL(btlevtcmd_CheckDamage, LW(10), LW(3), LW(4), LW(12), 256, LW(5))
        CASE_DEFAULT
    END_SWITCH
    GOTO(10)
    LABEL(50)
    CALL(btlevtcmd_GetStageSize, LW(5), EVT_NULLPTR, EVT_NULLPTR)
    SET_VALUE(LW(0), 0)
    CALL(btlevtcmd_FaceDirectionSub, LW(10), LW(0), LW(5))
    CALL(btlevtcmd_GetPos, LW(10), EVT_NULLPTR, LW(1), EVT_NULLPTR)
    CALL(btlevtcmd_JumpSetting, LW(10), 0, FLOAT(5.0f), FLOAT(0.009765625f))
    CALL(btlevtcmd_JumpPosition, LW(10), LW(0), 40, LW(2), 0, -1)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_CheckDamageCode, LW(10), 512, LW(6))
    IF_INT_NOT_EQUAL(LW(6), 0)
        MULTIPLY(LW(0), -1)
        CALL(btlevtcmd_SetPos, LW(10), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetHomePos, LW(10), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionAdd, LW(10), LW(0), 40)
        CALL(btlevtcmd_JumpSetting, LW(10), 0, FLOAT(7.0f), FLOAT(0.099609375f))
        CALL(btlevtcmd_JumpPosition, LW(10), LW(0), LW(1), LW(2), 0, -1)
        CALL(btlevtcmd_GetHomePos, LW(10), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_MovePosition, LW(10), LW(0), LW(1), LW(2), 16, -1, 0)
        WAIT_FRAMES(30)
    ELSE
        CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_SetPos, LW(10), LW(0), LW(1), LW(2))
    END_IF
    DELETE_EVENT(LW(13))
    CALL(btlevtcmd_SetRotate, LW(10), 0, 0, 0)
    RETURN
EVT_END()

EVT_BEGIN(subsetevt_shot_damage_ultra)
    BEGIN_SIBLING_EVENT_GET_ID(LW(13))
        CALL(btlevtcmd_SetRotateOffsetFromCenterOffset, LW(10))
        CALL(btlevtcmd_GetFaceDirection, LW(10), LW(0))
        MULTIPLY(LW(0), 20)
        SET_VALUE(LW(5), 5)
        LOOP(LW(5))
            CALL(btlevtcmd_AddRotate, LW(10), 0, LW(0), 0)
            SET_VALUE(LW(5), 5)
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_GetFriendBelong, LW(10), LW(0))
    CALL(btlevtcmd_SamplingEnemy, LW(10), LW(0), LW(12))
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    LABEL(1)
    IF_INT_EQUAL(LW(3), -1)
        GOTO(50)
    END_IF
    CALL(btlevtcmd_GetUnitId, LW(10), LW(0))
    IF_INT_NOT_EQUAL(LW(0), LW(3))
        CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
        GOTO(1)
    END_IF
    SET_VALUE(LW(8), -1)
    SET_VALUE(LW(9), -1)
    LABEL(2)
    CALL(btlevtcmd_GetSelectNextEnemy, LW(6), LW(7))
    IF_INT_NOT_EQUAL(LW(6), -1)
        SET_VALUE(LW(8), LW(6))
        SET_VALUE(LW(9), LW(7))
        GOTO(2)
    END_IF
    IF_INT_EQUAL(LW(8), -1)
        GOTO(50)
    END_IF
    BEGIN_SIBLING_EVENT_GET_ID(LW(5))
        CALL(btlevtcmd_JumpSetting, LW(10), 0, FLOAT(5.0f), FLOAT(0.39941406f))
        CALL(btlevtcmd_GetHitPos, LW(8), LW(9), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionAdd, LW(8), LW(0), 20)
        CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_JumpPosition, LW(10), LW(0), LW(1), LW(2), 0, -1)
    END_SIBLING_EVENT
    CALL(btlevtcmd_GetFriendBelong, LW(10), LW(0))
    CALL(btlevtcmd_SamplingEnemy, LW(10), LW(0), LW(12))
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    LABEL(30)
    IF_INT_EQUAL(LW(3), -1)
        DELETE_EVENT(LW(5))
        GOTO(50)
    END_IF
    CALL(btlevtcmd_GetUnitId, LW(10), LW(0))
    IF_INT_NOT_EQUAL(LW(0), LW(3))
        CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
        GOTO(30)
    END_IF
    CALL(btlevtcmd_GetPos, LW(10), LW(1), EVT_NULLPTR, EVT_NULLPTR)
    CALL(btlevtcmd_FaceDirectionAdd, LW(10), LW(1), 20)
    LABEL(31)
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        DELETE_EVENT(LW(5))
        GOTO(50)
    END_IF
    LABEL(32)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(2), EVT_NULLPTR, EVT_NULLPTR)
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(2), 20)
    CALL(btlevtcmd_GetPos, LW(10), LW(0), EVT_NULLPTR, EVT_NULLPTR)
    CALL(_check_through_pos, LW(10), LW(2), LW(0), LW(1), LW(2))
    IF_INT_EQUAL(LW(2), 0)
        IS_EVENT_RUNNING(LW(5), LW(6))
        IF_INT_EQUAL(LW(6), 1)
            SET_VALUE(LW(1), LW(0))
            WAIT_FRAMES(1)
            GOTO(32)
        END_IF
    END_IF
    CALL(btlevtcmd_PreCheckDamage, LW(10), LW(3), LW(4), LW(12), 256, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(2)
        CALL(btlevtcmd_StartAvoid, LW(3), 40)
        CASE_EQUAL(3)
        CALL(btlevtcmd_StartAvoid, LW(3), 38)
        CASE_EQUAL(6)
        CALL(btlevtcmd_StartAvoid, LW(3), 39)
        CASE_EQUAL(4)
        CASE_EQUAL(1)
        CALL(btlevtcmd_CheckDamage, LW(10), LW(3), LW(4), LW(12), 256, LW(0))
        CASE_DEFAULT
    END_SWITCH
    GOTO(31)
    DELETE_EVENT(LW(5))
    LABEL(50)
    CALL(btlevtcmd_GetStageSize, LW(5), EVT_NULLPTR, EVT_NULLPTR)
    SET_VALUE(LW(0), 0)
    CALL(btlevtcmd_FaceDirectionSub, LW(10), LW(0), LW(5))
    CALL(btlevtcmd_GetPos, LW(10), EVT_NULLPTR, LW(1), EVT_NULLPTR)
    CALL(btlevtcmd_JumpSetting, LW(10), 0, FLOAT(5.0f), FLOAT(0.099609375f))
    CALL(btlevtcmd_JumpPosition, LW(10), LW(0), 80, LW(2), 0, -1)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_CheckDamagePattern, LW(10), 16, LW(5))
    IF_INT_NOT_EQUAL(LW(5), 0)
        CALL(btlevtcmd_OnToken, LW(10), 64)
        LABEL(55)
        CALL(btlevtcmd_CheckToken, LW(10), 64, LW(5))
        IF_INT_NOT_EQUAL(LW(5), 0)
            WAIT_FRAMES(1)
            GOTO(55)
        END_IF
    END_IF
    MULTIPLY(LW(0), -1)
    CALL(btlevtcmd_SetPos, LW(10), LW(0), 80, LW(2))
    CALL(btlevtcmd_GetHomePos, LW(10), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpSetting, LW(10), 0, FLOAT(7.0f), FLOAT(0.099609375f))
    CALL(btlevtcmd_JumpPosition, LW(10), LW(0), LW(1), LW(2), 0, -1)
    CALL(btlevtcmd_CheckDamagePattern, LW(10), 16, LW(5))
    IF_INT_NOT_EQUAL(LW(5), 0)
        CALL(btlevtcmd_OnToken, LW(10), 128)
        CALL(btlevtcmd_WaitAttackEnd)
    END_IF
    END_IF
    CALL(btlevtcmd_JumpSetting, LW(10), 0, FLOAT(0.0f), FLOAT(0.14941406f))
    CALL(btlevtcmd_JumpPosition, LW(10), LW(0), LW(1), LW(2), 20, -1)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_CheckDamageCode, LW(10), 256, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_WaitAttackEnd)
    END_IF
    DELETE_EVENT(LW(13))
    CALL(btlevtcmd_SetRotate, LW(10), 0, 0, 0)
    RETURN
EVT_END()

EVT_BEGIN(subsetevt_swallow_failed_damage)
    CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), 39)
    CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetFallAccel, LW(10), FLOAT(0.5f))
    CALL(btlevtcmd_FaceDirectionSub, LW(10), LW(0), 30)
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, LW(10), LW(0), LW(1), LW(2), 20, -1)
    CALL(btlevtcmd_FaceDirectionSub, LW(10), LW(0), 18)
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, LW(10), LW(0), LW(1), LW(2), 12, -1)
    WAIT_FRAMES(20)
    RETURN
EVT_END()

EVT_BEGIN(subsetevt_swallow_spew_damage)
    CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), 39)
    CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetFallAccel, LW(10), FLOAT(0.5f))
    CALL(btlevtcmd_FaceDirectionSub, LW(10), LW(0), 30)
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, LW(10), LW(0), LW(1), LW(2), 20, -1)
    CALL(btlevtcmd_FaceDirectionSub, LW(10), LW(0), 18)
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, LW(10), LW(0), LW(1), LW(2), 12, -1)
    WAIT_FRAMES(20)
    RETURN
EVT_END()

EVT_BEGIN(subsetevt_swallow_shot_damage)
    CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), LW(14))
    SET_VALUE(LW(12), PTR(&partyWeapon_YoshiNomikomi_Involved))
    CALL(btlevtcmd_GetFriendBelong, LW(10), LW(0))
    CALL(btlevtcmd_SamplingEnemy, LW(10), LW(0), LW(12))
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    LABEL(0)
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), 39)
        CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_SetFallAccel, LW(10), FLOAT(0.5f))
        CALL(btlevtcmd_FaceDirectionSub, LW(10), LW(0), 30)
        CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_JumpPosition, LW(10), LW(0), LW(1), LW(2), 20, -1)
        CALL(btlevtcmd_FaceDirectionSub, LW(10), LW(0), 18)
        CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_JumpPosition, LW(10), LW(0), LW(1), LW(2), 12, -1)
        WAIT_FRAMES(20)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_GetUnitId, LW(10), LW(0))
    IF_INT_NOT_EQUAL(LW(0), LW(3))
        CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
        GOTO(0)
    END_IF
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    DEBUG_PUT_REG(LW(3))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), 39)
        CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_SetFallAccel, LW(10), FLOAT(0.5f))
        CALL(btlevtcmd_FaceDirectionSub, LW(10), LW(0), 30)
        CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_JumpPosition, LW(10), LW(0), LW(1), LW(2), 20, -1)
        CALL(btlevtcmd_FaceDirectionSub, LW(10), LW(0), 18)
        CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_JumpPosition, LW(10), LW(0), LW(1), LW(2), 12, -1)
        WAIT_FRAMES(20)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpSetting, LW(10), 0, FLOAT(5.0f), FLOAT(0.099609375f))
    CALL(btlevtcmd_JumpPosition, LW(10), LW(0), LW(1), LW(2), 0, -1)
    CALL(btlevtcmd_PreCheckDamage, -4, LW(3), LW(4), LW(12), 256, LW(5))
    SWITCH(LW(5))
        CASE_EQUAL(2)
        CALL(btlevtcmd_StartAvoid, LW(3), 40)
        CASE_EQUAL(3)
        CALL(btlevtcmd_StartAvoid, LW(3), 38)
        CASE_EQUAL(6)
        CALL(btlevtcmd_StartAvoid, LW(3), 39)
        CASE_EQUAL(4)
        CASE_EQUAL(1)
        CALL(btlevtcmd_CheckDamage, -4, LW(3), LW(4), LW(12), -2147483392, LW(5))
        GOTO(50)
        CASE_DEFAULT
    END_SWITCH
    LABEL(20)
    BEGIN_SIBLING_EVENT_GET_ID(LW(5))
        CALL(btlevtcmd_JumpContinue, LW(10))
    END_SIBLING_EVENT
    LABEL(21)
    IS_EVENT_RUNNING(LW(5), LW(0))
    IF_INT_EQUAL(LW(0), 0)
        GOTO(25)
    END_IF
    CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
    SET_VALUE(LW(6), LW(0))
    IF_INT_LESS(LW(6), 0)
        MULTIPLY(LW(6), -1)
    END_IF
    IF_INT_GREATER_OR_EQUAL(LW(6), 400)
        DELETE_EVENT(LW(5))
        CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_SetPos, LW(10), LW(0), LW(1), LW(2))
        GOTO(99)
    END_IF
    WAIT_FRAMES(1)
    GOTO(21)
    LABEL(25)
    GOTO(99)
    LABEL(50)
    CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetFallAccel, LW(10), FLOAT(0.5f))
    CALL(btlevtcmd_FaceDirectionAdd, LW(10), LW(0), 30)
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, LW(10), LW(0), LW(1), LW(2), 20, -1)
    CALL(btlevtcmd_FaceDirectionAdd, LW(10), LW(0), 18)
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, LW(10), LW(0), LW(1), LW(2), 12, -1)
    WAIT_FRAMES(20)
    LABEL(99)
    RETURN
EVT_END()

EVT_BEGIN(subset_ultra_hammer_acrobat_damage_event)
    CALL(btlevtcmd_SetRotate, LW(10), 0, 0, 0)
    CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), 39)
    CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, LW(10), LW(0), 40)
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetFallAccel, LW(10), FLOAT(0.5f))
    CALL(btlevtcmd_JumpPosition, LW(10), LW(0), LW(1), LW(2), 24, -1)
    CALL(btlevtcmd_FaceDirectionSub, LW(10), LW(0), 18)
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, LW(10), LW(0), LW(1), LW(2), 12, -1)
    CALL(btlevtcmd_OnToken, LW(10), 128)
    RETURN
EVT_END()

EVT_BEGIN(subsetevt_confuse_flustered)
    CALL(evt_btl_camera_set_mode, 0, 7)
    CALL(evt_btl_camera_set_homing_unit, 0, -2, -1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(btlevtcmd_GetBodyId, -2, LW(0))
    CALL(btlevtcmd_AnimeChangePoseType, -2, LW(0), 5)
    WAIT_MS(1500)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    WAIT_MS(1000)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

EVT_BEGIN(subsetevt_binta_damage_left)
    CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), LW(14))
    CALL(btlevtcmd_SetRotate, LW(10), 0, 0, 0)
    CALL(btlevtcmd_GetFaceDirection, LW(10), LW(0))
    IF_INT_GREATER_OR_EQUAL(LW(0), 0)
        SET_VALUE(LW(0), -18)
        ELSE
        SET_VALUE(LW(0), 18)
    END_IF
    LOOP(20)
        CALL(btlevtcmd_AddRotate, LW(10), 0, LW(0), 0)
        WAIT_FRAMES(1)
    END_LOOP
    CALL(btlevtcmd_SetRotate, LW(10), 0, 0, 0)
    WAIT_FRAMES(10)
    RETURN
    RETURN
EVT_END()

EVT_BEGIN(subsetevt_binta_damage_right)
    CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), LW(14))
    CALL(btlevtcmd_SetRotate, LW(10), 0, 0, 0)
    CALL(btlevtcmd_GetFaceDirection, LW(10), LW(0))
    IF_INT_GREATER_OR_EQUAL(LW(0), 0)
        SET_VALUE(LW(0), 18)
        ELSE
        SET_VALUE(LW(0), -18)
    END_IF
    LOOP(20)
        CALL(btlevtcmd_AddRotate, LW(10), 0, LW(0), 0)
        WAIT_FRAMES(1)
    END_LOOP
    CALL(btlevtcmd_SetRotate, LW(10), 0, 0, 0)
    WAIT_FRAMES(10)
    RETURN
    RETURN
EVT_END()

EVT_BEGIN(subsetevt_binta_damage_left_last)
    CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), LW(14))
    CALL(btlevtcmd_SetRotate, LW(10), 0, 0, 0)
    BEGIN_SIBLING_EVENT_GET_ID(LW(0))
        CALL(btlevtcmd_GetFaceDirection, LW(10), LW(0))
        IF_INT_GREATER_OR_EQUAL(LW(0), 0)
            SET_VALUE(LW(8), -2160)
            ELSE
            SET_VALUE(LW(8), 2160)
        END_IF
        CALL(_binta_effect, LW(10), LW(8))
        SET_VALUE(LW(7), LW(10))
        CALL(evt_sub_intpl_init, 11, 0, LW(8), 90)
        LOOP(90)
            CALL(evt_sub_intpl_get_value)
            CALL(btlevtcmd_SetRotate, LW(7), 0, LW(0), 0)
            WAIT_FRAMES(1)
        END_LOOP
        CALL(btlevtcmd_SetRotate, LW(7), 0, 0, 0)
    END_SIBLING_EVENT
    CALL(btlevtcmd_WaitEventEnd, LW(0))
    WAIT_FRAMES(30)
    RETURN
    RETURN
EVT_END()

EVT_BEGIN(subsetevt_binta_damage_right_last)
    CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), LW(14))
    CALL(btlevtcmd_SetRotate, LW(10), 0, 0, 0)
    BEGIN_SIBLING_EVENT_GET_ID(LW(0))
        CALL(btlevtcmd_GetFaceDirection, LW(10), LW(0))
        IF_INT_GREATER_OR_EQUAL(LW(0), 0)
            SET_VALUE(LW(8), 2160)
            ELSE
            SET_VALUE(LW(8), -2160)
        END_IF
        CALL(_binta_effect, LW(10), LW(8))
        SET_VALUE(LW(7), LW(10))
        CALL(evt_sub_intpl_init, 11, 0, LW(8), 90)
        LOOP(90)
            CALL(evt_sub_intpl_get_value)
            CALL(btlevtcmd_SetRotate, LW(7), 0, LW(0), 0)
            WAIT_FRAMES(1)
        END_LOOP
        CALL(btlevtcmd_SetRotate, LW(7), 0, 0, 0)
    END_SIBLING_EVENT
    CALL(btlevtcmd_WaitEventEnd, LW(0))
    WAIT_FRAMES(30)
    RETURN
    RETURN
EVT_END()

EVT_BEGIN(subsetevt_status_recovery)
    CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(0), 8)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_RunDataEventChild, LW(10), 8)
        RETURN
    END_IF
    CALL(btlevtcmd_StopWaitEvent, -2)
    CALL(btlevtcmd_GetBodyId, -2, LW(0))
    CALL(btlevtcmd_AnimeChangePoseFromTable, -2, LW(0))
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.39941406f))
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 30, -1)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

EVT_BEGIN(subsetevt_check_target_movable)
    CALL(btlevtcmd_CheckStatus, LW(3), 27, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(LW(0), 0)
        RETURN
    END_IF
    CALL(btlevtcmd_CheckStatus, LW(3), 9, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(LW(0), 0)
        RETURN
    END_IF
    CALL(btlevtcmd_CheckStatus, LW(3), 2, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(LW(0), 0)
        RETURN
    END_IF
    CALL(btlevtcmd_CheckStatus, LW(3), 2, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(LW(0), 0)
        RETURN
    END_IF
    SET_VALUE(LW(0), 1)
    RETURN
EVT_END()

EVT_BEGIN(subsetevt_nodamage_motion)
    CALL(evt_snd_sfxon, STRING("SFX_BTL_ATTACK_MISS2"), 0)
    RETURN
EVT_END()

EVT_BEGIN(subsetevt_regeneration)
    WAIT_FRAMES(60)
    RETURN
EVT_END()

EVT_BEGIN(subset_attack_ready_jump_event)
    RETURN
EVT_END()

USER_FUNC(_disable_restore_command_cursor) {
    BattleWork* wp = _battleWorkPointer;
    wp->commandMenu.recCommand = 0;
    wp->commandMenu.attackAudience = 0;
    return EVT_RETURN_DONE;
}

USER_FUNC(_check_at_dm_event_wait) {
    BattleWork* wp = _battleWorkPointer;
    s32 id;
    BattleUnit* unit;

    id = evtGetValue(event, event->args[0]);
    unit = BattleGetUnitPtr(wp, BattleTransID(event, id));
    if (!unit) {
        return EVT_RETURN_DONE;
    }
    if (unit->attackEventId && evtCheckID(unit->attackEventId)) {
        return EVT_RETURN_BLOCK;
    }
    if (unit->damageEventId && evtCheckID(unit->damageEventId)) {
        return EVT_RETURN_BLOCK;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(_add_star_point_disp_offset) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    s32 id;
    BattleUnit* unit;
    s32 xi, yi, zi;
    f32 x, y, z;

    id = evtGetValue(event, args[0]);
    unit = BattleGetUnitPtr(wp, BattleTransID(event, id));
    xi = args[1];
    x = evtGetValue(event, xi);
    yi = args[2];
    y = evtGetValue(event, yi);
    zi = args[3];
    z = evtGetValue(event, zi);
    x += unit->mKindParams->starPointDispOffset.x;
    y += unit->mKindParams->starPointDispOffset.y;
    z += unit->mKindParams->starPointDispOffset.z;
    evtSetValue(event, xi, x);
    evtSetValue(event, yi, y);
    evtSetValue(event, zi, z);
    return EVT_RETURN_DONE;
}

USER_FUNC(_set_hp_gauge_fade_flag) {
    BattleWork* wp = _battleWorkPointer;
    s32 id;
    BattleUnit* unit;

    id = evtGetValue(event, event->args[0]);
    unit = BattleGetUnitPtr(wp, BattleTransID(event, id));
    if (unit) {
        unit->hpGauge.flags |= 1;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(_check_through_pos) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, *args++);
    s32 pos = evtGetValue(event, *args++);
    s32 a = evtGetValue(event, *args++);
    s32 b = evtGetValue(event, *args++);
    s32 out = *args++;
    BattleUnit* unit;
    s32 min, max;

    unit = BattleGetUnitPtr(wp, BattleTransID(event, id));
    if (a > b) {
        max = a;
        min = b;
    }
    else {
        max = b;
        min = a;
    }
    if (pos >= min && pos <= max) {
        evtSetValue(event, out, TRUE);
    }
    else {
        evtSetValue(event, out, FALSE);
        if (unit->faceDirection >= 0) {
            if (pos > max) {
                evtSetValue(event, out, TRUE);
            }
        }
        else if (pos < min) {
            evtSetValue(event, out, TRUE);
        }
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(_binta_effect) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, *args++);
    s32 dir = evtGetValue(event, *args++);
    BattleUnit* unit;
    EffectEntry* eff;
    Vec pos;

    unit = BattleGetUnitPtr(wp, BattleTransID(event, id));
    BtlUnit_GetPos(unit, &pos.x, &pos.y, &pos.z);
    eff = effWhirlwindN64Entry(dir < 0, NULL, unit->mSizeMultiplier, 90);
    ((EffWhirlwindN64Work*)eff->userdata)->position.x = pos.x;
    ((EffWhirlwindN64Work*)eff->userdata)->position.y = pos.y;
    ((EffWhirlwindN64Work*)eff->userdata)->position.z = pos.z;
    return EVT_RETURN_DONE;
}
