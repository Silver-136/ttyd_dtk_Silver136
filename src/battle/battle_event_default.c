#include "eff/eff_fire.h"
#include "battle/battle_event_default.h"
#include "battle/battle.h"
#include "battle/battle_ac_help.h"
#include "battle/battle_sub.h"
#include "battle/battle_unit.h"
#include "driver/effdrv.h"
#include "evt/evt_cmd.h"
#include "manager/evtmgr_cmd.h"
#include "battle/battle_seq_end.h"
#include "battle/battle_mario.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "evt/evt_batstage.h"
#include "evt/evt_sub.h"
#include "battle/battle_audience.h"
#include "battle/battle_message.h"
#include "evt/evt_eff.h"
#include "party/party_motion.h"
#include "evt/evt_audience.h"
#include "battle/battle_status_effect.h"


s32 BattleDamageDirect(s32 attackerId, BattleUnit* target, BattleUnitPart* part, s32 damage, s32 fpDamage,
                       u32 flags, s32 code, u32 counter);

static USER_FUNC(_anime_load);
static USER_FUNC(_anime_load_wait);
USER_FUNC(_backfire);
USER_FUNC(_btl_escape_reset_move_color_lv_player_unit_all);
USER_FUNC(_check_pose_stay);
USER_FUNC(_get_escape_rate);
USER_FUNC(_get_flower_suitoru_point);
USER_FUNC(_get_heart_suitoru_point);
USER_FUNC(_set_escape_ac_hlp_msg);
USER_FUNC(btlevtcmd_AnimeCmpPoseType);
USER_FUNC(btlevtcmd_CheckDefinePosetype);
USER_FUNC(btlevtcmd_CheckGuardMotionEnable);
USER_FUNC(btlevtcmd_GetConfuseActEvent);
USER_FUNC(btlevtcmd_GuardMoveEnd);
USER_FUNC(btlevtcmd_InterruptStop);
USER_FUNC(btlevtcmd_SetGuard);
USER_FUNC(btlevtcmd_SortPhaseMoveTable);
USER_FUNC(btlevtcmd_WaitAttackEndCheck);
USER_FUNC(btlevtcmd_audience_code_escape);
USER_FUNC(btlevtcmd_audience_code_escape_fail);
USER_FUNC(btlevtcmd_audience_code_escape_success);

EvtWord btldefaultevt_Damage[1133];
EvtWord btldefaultevt_ChangeParty[174];
EvtWord btldefaultevt_BiribiriMove[29];
EvtWord btldefaultevt_guard_move[85];
EvtWord btldefaultevt_guard_return[98];
EvtWord btldefaultevt_Appeal[145];
EvtWord _Defence_core[81];
EvtWord btldefaultevt_Escape[657];
EvtWord btldefaultevt_Confuse[19];
EvtWord btldefaultevt_CantMoveZeroGravity[44];
EvtWord btldefaultevt_SuitoruBadgeEffect[122];
EvtWord btldefaultevt_avoid_counter_event[34];
EvtWord btldefaultevt_ProtectPartner[10];
EvtWord btldefaultevt_Dummy[2];
EvtWord btldefaultevt_Wait[5];
EvtWord btldefaultevt_LectureEnd[6];
EvtWord btldefaultevt_Defend[7];
EvtWord btldefaultevt_DefendParty[7];

EVT_BEGIN(btldefaultevt_Damage)
    CALL(btlevtcmd_SetRotate, LW(10), 0, 0, 0)
    CALL(btlevtcmd_CheckDamageCode, LW(10), 32768, LW(1))
    IF_INT_NOT_EQUAL(LW(1), 0)
        CALL(btlevtcmd_OnOffStatus, LW(10), 9, 0, 0, 0)
        CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
        CALL(evt_eff64, STRING(""), STRING("freeze_n64"), 0, LW(0), LW(1), LW(2), FLOAT(1.0f), 1, 0, 0, 0, 0, 0, 0)
    END_IF
    CALL(btlevtcmd_CheckDamagePattern, LW(10), ATTACKPATTERN_DAMAGE_MOTION, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(LW(14), 39)
        CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), LW(14))
        BEGIN_SIBLING_EVENT
            CALL(btlevtcmd_RunDataEventChild, LW(10), 10)
        END_SIBLING_EVENT
        LABEL(0)
        WAIT_FRAMES(1)
        GOTO(0)
    END_IF
    CALL(btlevtcmd_CheckDamagePattern, LW(10), ATTACKPATTERN_DAMAGE_MOTION_BLOW, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(LW(14), 39)
        CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), LW(14))
        BEGIN_SIBLING_EVENT
            CALL(btlevtcmd_RunDataEventChild, LW(10), 10)
        END_SIBLING_EVENT
        LABEL(1)
        CALL(btlevtcmd_SetDispOffset, LW(10), 2, 0, 0)
        WAIT_FRAMES(2)
        CALL(btlevtcmd_SetDispOffset, LW(10), 0, 0, 0)
        WAIT_FRAMES(2)
        CALL(btlevtcmd_WaitAttackEndCheck, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            GOTO(1)
        END_IF
    END_IF
    CALL(btlevtcmd_CheckDamagePattern, LW(10), ATTACKPATTERN_CHUCHURINA_ITEMSTEAL, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(LW(14), 5)
        CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), LW(14))
        BEGIN_SIBLING_EVENT
            CALL(btlevtcmd_RunDataEventChild, LW(10), 10)
        END_SIBLING_EVENT
        CALL(btlevtcmd_WaitAttackEnd)
        GOTO(10)
    END_IF
    CALL(btlevtcmd_CheckDamagePattern, LW(10), ATTACKPATTERN_SWALLOW_SAMBO_BODY, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(2), UNIT_DATA_SWALLOW_BODY_EVENT)
        IF_INT_NOT_EQUAL(LW(2), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_SWALLOW_BODY_EVENT)
        END_IF
    END_IF
    CALL(btlevtcmd_CheckDamagePattern, LW(10), ATTACKPATTERN_ULTRA_HAMMER_ACROBAT, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(PTR(subset_ultra_hammer_acrobat_damage_event))
        GOTO(10)
    END_IF
    CALL(btlevtcmd_CheckDamageCode, LW(10), 262144, LW(1))
    IF_INT_NOT_EQUAL(LW(1), 0)
        CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), 31)
        WAIT_FRAMES(30)
        GOTO(10)
    END_IF
    CALL(btlevtcmd_GetDamageCode, LW(10), LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(31)
        CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_AVOID_THORN_HEAD_EVENT)
        GOTO(10)
        CASE_EQUAL(32)
        CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_AVOID_THORN_FRONT_EVENT)
        GOTO(10)
        CASE_EQUAL(33)
        CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_AVOID_THORN_EVENT)
        GOTO(10)
        CASE_EQUAL(34)
        CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_AVOID_FIRE_EVENT)
        GOTO(10)
        CASE_EQUAL(35)
        CALL(btlevtcmd_RunDataEventChild, LW(10), 41)
        GOTO(10)
        CASE_EQUAL(36)
        CALL(btlevtcmd_RunDataEventChild, LW(10), 42)
        GOTO(10)
        CASE_EQUAL(37)
        CALL(btlevtcmd_RunDataEventChild, LW(10), 43)
        GOTO(10)
        CASE_EQUAL(42)
        CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_AVOID_COUNTER_EVENT)
        GOTO(10)
        CASE_EQUAL(38)
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(2), 45)
        IF_INT_NOT_EQUAL(LW(2), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), 45)
            ELSE
            CALL(evt_snd_sfxon, STRING("SFX_SYSTEM_LUCKY1"), 0)
        END_IF
        GOTO(10)
        CASE_EQUAL(39)
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(2), 44)
        IF_INT_NOT_EQUAL(LW(2), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), 44)
            ELSE
            CALL(btlevtcmd_CheckCommandUnit, LW(10), LW(0))
            IF_INT_NOT_EQUAL(LW(0), 0)
                CALL(evt_snd_sfxon, STRING("SFX_MARIO_AVOID1"), 0)
                ELSE
                CALL(evt_snd_sfxon, STRING("SFX_BTL_ATTACK_MISS1"), 0)
            END_IF
            CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), 48)
            CALL(btlevtcmd_AnimeChangePaperGroup, LW(10), LW(11), STRING("p_b_st"), 1)
            CALL(btlevtcmd_CheckStatus, LW(10), 15, LW(2))
            IF_INT_NOT_EQUAL(LW(2), 0)
                CALL(btlevtcmd_AnimeChangePaperAnime, LW(10), LW(11), STRING("PM_F_3A"))
                CALL(btlevtcmd_ftomsec, 37, LW(0))
                WAIT_MS(LW(0))
                CALL(btlevtcmd_AnimeChangePaperAnime, LW(10), LW(11), STRING("PM_F_3B"))
                CALL(btlevtcmd_ftomsec, 22, LW(0))
                WAIT_MS(LW(0))
                ELSE
                CALL(btlevtcmd_CheckStatus, LW(10), 14, LW(2))
                IF_INT_NOT_EQUAL(LW(2), 0)
                    CALL(btlevtcmd_AnimeChangePaperAnime, LW(10), LW(11), STRING("PM_Y_2A"))
                    CALL(btlevtcmd_ftomsec, 40, LW(0))
                    WAIT_MS(LW(0))
                    CALL(btlevtcmd_AnimeChangePaperAnime, LW(10), LW(11), STRING("PM_Y_2B"))
                    CALL(btlevtcmd_ftomsec, 65, LW(0))
                    WAIT_MS(LW(0))
                    ELSE
                    CALL(btlevtcmd_AnimeChangePaperAnime, LW(10), LW(11), STRING("PM_Y_2A"))
                    CALL(btlevtcmd_ftomsec, 40, LW(0))
                    WAIT_MS(LW(0))
                    CALL(btlevtcmd_AnimeChangePaperAnime, LW(10), LW(11), STRING("PM_Y_2B"))
                    CALL(btlevtcmd_ftomsec, 65, LW(0))
                    WAIT_MS(LW(0))
                END_IF
            END_IF
        END_IF
        GOTO(10)
        CASE_EQUAL(40)
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(2), UNIT_DATA_AVOID_MISS_EVENT)
        IF_INT_NOT_EQUAL(LW(2), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_AVOID_MISS_EVENT)
            ELSE
        END_IF
    END_SWITCH
    CALL(btlevtcmd_CheckDamagePattern, LW(10), ATTACKPATTERN_CHUCHURINA_BINTA_L, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(LW(14), 39)
        CALL_CHILD(PTR(subsetevt_binta_damage_left))
        CALL(btlevtcmd_WaitAttackEnd)
        RETURN
    END_IF
    CALL(btlevtcmd_CheckDamagePattern, LW(10), ATTACKPATTERN_CHUCHURINA_BINTA_R, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(LW(14), 39)
        CALL_CHILD(PTR(subsetevt_binta_damage_right))
        CALL(btlevtcmd_WaitAttackEnd)
        RETURN
    END_IF
    CALL(btlevtcmd_GetDamageCode, LW(10), LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(57)
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(2), UNIT_DATA_FIRST_ATTACK_POS_EVENT)
        IF_INT_NOT_EQUAL(LW(2), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_FIRST_ATTACK_POS_EVENT)
        END_IF
        CALL(btlevtcmd_StartWaitEvent, LW(10))
        RETURN
        CASE_OR_EQUAL(30)
        CASE_OR_EQUAL(27)
        CASE_OR_EQUAL(26)
        CASE_OR_EQUAL(23)
        CALL(btlevtcmd_OnUnitFlag, LW(10), BATTLE_UNIT_FLAG_SHOW_HP)
        SET_VALUE(LW(14), 39)
        SET_VALUE(LW(15), -1)
        CALL(btlevtcmd_CheckDamagePattern, LW(10), ATTACKPATTERN_DAMAGE_SURPRISE, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            SET_VALUE(LW(14), 38)
        END_IF
        BEGIN_SIBLING_EVENT
            CALL(btlevtcmd_RunDataEventChild, LW(10), 10)
        END_SIBLING_EVENT
        CALL_CHILD(PTR(subsetevt_damage))
        CASE_END
        CASE_EQUAL(22)
        CALL(btlevtcmd_CheckDefinePosetype, LW(10), LW(11), 35, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            SET_VALUE(LW(14), 35)
            CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), LW(14))
            BEGIN_SIBLING_EVENT
                CALL(btlevtcmd_RunDataEventChild, LW(10), 10)
            END_SIBLING_EVENT
            WAIT_FRAMES(45)
            ELSE
            WAIT_FRAMES(45)
        END_IF
        CASE_EQUAL(24)
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(2), UNIT_DATA_DAMAGE_FIRE_EXPLOSION_EVENT)
        IF_INT_NOT_EQUAL(LW(2), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_DAMAGE_FIRE_EXPLOSION_EVENT)
            ELSE
            CALL(btlevtcmd_OnUnitFlag, LW(10), BATTLE_UNIT_FLAG_SHOW_HP)
            SET_VALUE(LW(14), 29)
            SET_VALUE(LW(15), -1)
            BEGIN_SIBLING_EVENT
                CALL(btlevtcmd_RunDataEventChild, LW(10), 10)
            END_SIBLING_EVENT
            CALL_CHILD(PTR(subsetevt_damage))
        END_IF
        CASE_EQUAL(25)
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(2), 12)
        IF_INT_NOT_EQUAL(LW(2), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), 12)
            ELSE
            CALL(btlevtcmd_OnUnitFlag, LW(10), BATTLE_UNIT_FLAG_SHOW_HP)
            SET_VALUE(LW(14), 29)
            SET_VALUE(LW(15), -1)
            BEGIN_SIBLING_EVENT
                CALL(btlevtcmd_RunDataEventChild, LW(10), 10)
            END_SIBLING_EVENT
            CALL_CHILD(PTR(subsetevt_damage))
        END_IF
        CASE_EQUAL(43)
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(2), 24)
        IF_INT_NOT_EQUAL(LW(2), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), 24)
            ELSE
            CALL_CHILD(PTR(subsetevt_counter_damage))
        END_IF
        CASE_EQUAL(44)
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(2), 25)
        IF_INT_NOT_EQUAL(LW(2), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), 25)
            ELSE
            CALL_CHILD(PTR(subsetevt_counter_damage))
        END_IF
        CASE_EQUAL(45)
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(2), 26)
        IF_INT_NOT_EQUAL(LW(2), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), 26)
            ELSE
            CALL_CHILD(PTR(subsetevt_counter_damage))
        END_IF
        CASE_EQUAL(46)
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(2), 27)
        IF_INT_NOT_EQUAL(LW(2), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), 27)
            ELSE
            CALL_CHILD(PTR(subsetevt_counter_damage))
        END_IF
        CASE_EQUAL(47)
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(2), 28)
        IF_INT_NOT_EQUAL(LW(2), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), 28)
            ELSE
            CALL_CHILD(PTR(subsetevt_counter_damage))
        END_IF
        CASE_EQUAL(48)
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(2), 29)
        IF_INT_NOT_EQUAL(LW(2), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), 29)
            ELSE
            CALL_CHILD(PTR(subsetevt_counter_damage))
        END_IF
        CASE_EQUAL(49)
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(2), 30)
        IF_INT_NOT_EQUAL(LW(2), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), 30)
            ELSE
            CALL_CHILD(PTR(subsetevt_counter_damage))
        END_IF
        CASE_EQUAL(50)
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(2), 31)
        IF_INT_NOT_EQUAL(LW(2), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), 31)
            ELSE
            CALL_CHILD(PTR(subsetevt_counter_damage))
        END_IF
        CASE_EQUAL(51)
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(2), 32)
        IF_INT_NOT_EQUAL(LW(2), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), 32)
            ELSE
            CALL_CHILD(PTR(subsetevt_counter_damage))
        END_IF
        CASE_EQUAL(52)
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(2), 33)
        IF_INT_NOT_EQUAL(LW(2), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), 33)
            ELSE
            CALL_CHILD(PTR(subsetevt_counter_damage))
        END_IF
        CASE_EQUAL(53)
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(2), 34)
        IF_INT_NOT_EQUAL(LW(2), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), 34)
            ELSE
            CALL_CHILD(PTR(subsetevt_counter_damage))
        END_IF
        CASE_EQUAL(54)
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(2), 35)
        IF_INT_NOT_EQUAL(LW(2), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), 35)
            ELSE
            CALL_CHILD(PTR(subsetevt_counter_damage))
        END_IF
        CASE_EQUAL(55)
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(2), 36)
        IF_INT_NOT_EQUAL(LW(2), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), 36)
            ELSE
            CALL_CHILD(PTR(subsetevt_counter_damage))
        END_IF
        CASE_EQUAL(41)
        CALL_CHILD(PTR(btldefaultevt_avoid_guard))
        RETURN
        CASE_EQUAL(56)
        CALL_CHILD(PTR(btldefaultevt_guard_move))
        CASE_EQUAL(28)
        CALL_CHILD(PTR(subsetevt_terror))
        RETURN
        CASE_EQUAL(29)
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(2), UNIT_DATA_BLOW_DEAD_EVENT)
        IF_INT_NOT_EQUAL(LW(2), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_BLOW_DEAD_EVENT)
            ELSE
            CALL_CHILD(PTR(subsetevt_blow_dead))
        END_IF
        RETURN
        CASE_DEFAULT
    END_SWITCH
    LABEL(10)
    CALL(btlevtcmd_CheckDamageCode, LW(10), 1024, LW(1))
    IF_INT_NOT_EQUAL(LW(1), 0)
        WAIT_FRAMES(30)
        CALL_CHILD(PTR(subsetevt_dead))
        RETURN
        ELSE
        CALL(btlevtcmd_CheckDamageCode, LW(10), 512, LW(1))
        IF_INT_NOT_EQUAL(LW(1), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_FAKE_DEAD_EVENT)
            RETURN
        END_IF
    END_IF
    CALL(btlevtcmd_CheckDamageCode, LW(10), 16384, LW(1))
    IF_INT_NOT_EQUAL(LW(1), 0)
        CALL(_backfire, LW(10))
        RETURN
    END_IF
    CALL(btlevtcmd_CheckDamageCode, LW(10), 4096, LW(1))
    IF_INT_NOT_EQUAL(LW(1), 0)
        CALL_CHILD(PTR(btldefaultevt_guard_return))
        RETURN
    END_IF
    CALL(btlevtcmd_CheckDamageCode, LW(10), 256, LW(1))
    IF_INT_EQUAL(LW(1), 0)
        CALL(btlevtcmd_WaitAttackEnd)
    END_IF
    LABEL(99)
    CALL(btlevtcmd_CheckAttribute, LW(10), 4, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetPos, LW(10), LW(5), LW(6), LW(7))
        CALL(btlevtcmd_GetHomePos, LW(10), LW(2), LW(3), LW(4))
        IF_INT_EQUAL(LW(2), LW(5))
            IF_INT_EQUAL(LW(3), LW(6))
                IF_INT_EQUAL(LW(4), LW(7))
                    GOTO(101)
                END_IF
            END_IF
        END_IF
        SET_VALUE(LW(14), 42)
        CALL(btlevtcmd_SetMoveSpeed, LW(10), FLOAT(4.0f))
        CALL(btlevtcmd_GetHomePos, LW(10), LW(2), LW(3), LW(4))
        CALL(btlevtcmd_GetMoveFrame, -2, LW(0), LW(1), LW(2), 0, LW(7))
        IF_INT_GREATER(LW(7), 0)
            CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), LW(14))
            CALL(btlevtcmd_DivePosition, LW(10), LW(2), LW(3), LW(4), 0, 0, 0, 0, -1)
        END_IF
        LABEL(101)
        ELSE
        CALL(btlevtcmd_GetPos, LW(10), LW(5), LW(6), LW(7))
        CALL(btlevtcmd_GetHomePos, LW(10), LW(2), LW(3), LW(4))
        IF_INT_EQUAL(LW(2), LW(5))
            IF_INT_EQUAL(LW(3), LW(6))
                IF_INT_EQUAL(LW(4), LW(7))
                    GOTO(102)
                END_IF
            END_IF
        END_IF
        SET_VALUE(LW(14), 40)
        CALL(btlevtcmd_SetMoveSpeed, LW(10), FLOAT(4.0f))
        CALL(btlevtcmd_GetHomePos, LW(10), LW(2), LW(3), LW(4))
        CALL(btlevtcmd_GetMoveFrame, -2, LW(0), LW(1), LW(2), 0, LW(7))
        IF_INT_GREATER(LW(7), 0)
            CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), LW(14))
            CALL(btlevtcmd_MovePosition, LW(10), LW(2), LW(3), LW(4), 0, -1, 0)
        END_IF
        LABEL(102)
    END_IF
    CALL(btlevtcmd_ResetFaceDirection, LW(10))
    CALL(btlevtcmd_StartWaitEvent, LW(10))
    RETURN
EVT_END()

EVT_BEGIN(btldefaultevt_ChangeParty)
    CALL(_anime_load)
    CALL(btlevtcmd_GetPartyId, -2, LW(3))
    CALL(btlevtcmd_GetBodyId, LW(3), LW(4))
    CALL(btlevtcmd_SetFallAccel, LW(3), FLOAT(0.69921875f))
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetMarioId, -2, LW(8))
    CALL(btlevtcmd_GetPos, LW(8), LW(5), LW(6), LW(7))
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_SetScale, LW(3), FLOAT(1.0f), FLOAT(1.0f), FLOAT(1.0f))
        LOOP(18)
            CALL(btlevtcmd_AddScale, LW(3), FLOAT(-0.049804688f), FLOAT(-0.049804688f), FLOAT(-0.049804688f))
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    CALL(evt_snd_sfxon, STRING("SFX_BTL_PARTY_IN1"), 0)
    CALL(btlevtcmd_JumpPosition, LW(3), LW(5), LW(6), LW(7), 20, 0)
    CALL(_anime_load_wait)
    CALL(btlevtcmd_ChangeParty, LW(9))
    CALL(btlevtcmd_OffUnitFlag, LW(9), 2097152)
    CALL(btlevtcmd_GetBodyId, LW(9), LW(0))
    CALL(btlevtcmd_AnimeChangePoseFromTable, LW(9), LW(0))
    CALL(btlevtcmd_SetPos, LW(9), LW(5), LW(6), LW(7))
    CALL(btlevtcmd_SetFallAccel, LW(9), FLOAT(0.69921875f))
    CALL(btlevtcmd_CheckDataOfDataTable, LW(9), LW(0), 4)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_RunDataEventChild, LW(9), 4)
    END_IF
    CALL(evt_snd_sfxon, STRING("SFX_BTL_PARTY_CHANGE1"), 0)
    CALL(evt_audience_sound_cheer, 45, 25)
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_SetScale, LW(9), FLOAT(0.099609375f), FLOAT(0.099609375f), FLOAT(0.099609375f))
        LOOP(18)
            CALL(btlevtcmd_AddScale, LW(9), FLOAT(0.049804688f), FLOAT(0.049804688f), FLOAT(0.049804688f))
            WAIT_FRAMES(1)
        END_LOOP
        CALL(btlevtcmd_SetScale, LW(9), FLOAT(1.0f), FLOAT(1.0f), FLOAT(1.0f))
    END_SIBLING_EVENT
    CALL(btlevtcmd_GetHomePos, LW(9), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon, STRING("SFX_PARTY_OUT1"), 0)
    CALL(btlevtcmd_JumpPosition, LW(9), LW(0), LW(1), LW(2), 20, 0)
    CALL(btlevtcmd_OnUnitFlag, LW(9), 2097152)
    CALL(btlevtcmd_KillUnit, LW(3), 0)
    CALL(btlevtcmd_SortPhaseMoveTable)
    CALL(_check_pose_stay, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_StartWaitEvent, -2)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(btldefaultevt_BiribiriMove)
    CALL(evt_sub_intpl_init, 8, 0, 5, 16)
    LABEL(0)
    CALL(evt_sub_intpl_get_value)
    CALL(btlevtcmd_SetDispOffset, -2, LW(0), 0, 0)
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    CALL(btlevtcmd_StartWaitEvent, LW(10))
    RETURN
EVT_END()

EVT_BEGIN(btldefaultevt_guard_move)
    CALL(btlevtcmd_AnimeCmpPoseType, LW(10), LW(11), 42, 40, LW(2))
    IF_INT_EQUAL(LW(2), 0)
        CALL(btlevtcmd_AnimeSetMotionSpeed, LW(10), LW(11), FLOAT(2.0f))
    END_IF
    CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), 42)
    CALL(btlevtcmd_CheckAttribute, LW(10), 4, LW(6))
    IF_INT_NOT_EQUAL(LW(6), 0)
        CALL(btlevtcmd_GetProtectId, LW(10), LW(6))
        CALL(btlevtcmd_GetHomePos, LW(6), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_DivePosition, LW(10), LW(0), LW(1), LW(2), 20, 0, 0, 0, -1)
        ELSE
        CALL(btlevtcmd_GetProtectId, LW(10), LW(6))
        CALL(btlevtcmd_GetHomePos, LW(6), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_MovePosition, LW(10), LW(0), LW(1), LW(2), 20, -1, 0)
    END_IF
    CALL(btlevtcmd_AnimeSetMotionSpeed, LW(10), LW(11), FLOAT(1.0f))
    CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), 46)
    CALL(btlevtcmd_GuardMoveEnd)
    RETURN
EVT_END()

EVT_BEGIN(btldefaultevt_guard_return)
    CALL(btlevtcmd_WaitAttackEnd)
    CALL(btlevtcmd_CheckGuardMotionEnable, LW(10), LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_AnimeCmpPoseType, LW(10), LW(11), 42, 40, LW(2))
        IF_INT_EQUAL(LW(2), 0)
            CALL(btlevtcmd_AnimeSetMotionSpeed, LW(10), LW(11), FLOAT(2.0f))
        END_IF
        CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), 42)
    END_IF
    CALL(btlevtcmd_CheckAttribute, LW(10), 4, LW(6))
    IF_INT_NOT_EQUAL(LW(6), 0)
        CALL(btlevtcmd_GetHomePos, LW(10), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_DivePosition, LW(10), LW(0), LW(1), LW(2), 20, 0, 0, 0, -1)
        ELSE
        CALL(btlevtcmd_GetHomePos, LW(10), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_MovePosition, LW(10), LW(0), LW(1), LW(2), 20, -1, 0)
    END_IF
    CALL(btlevtcmd_CheckGuardMotionEnable, LW(10), LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_AnimeSetMotionSpeed, LW(10), LW(11), FLOAT(1.0f))
        CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), 45)
    END_IF
    CALL(btlevtcmd_GuardMoveEnd)
    CALL(btlevtcmd_StartWaitEvent, LW(10))
    RETURN
EVT_END()

EVT_BEGIN(btldefaultevt_Appeal)
    CALL(btlevtcmd_GetBodyId, -2, LW(0))
    CALL(btlevtcmd_AnimeChangePoseType, -2, LW(0), 43)
    CALL(btlevtcmd_CheckStatus, -2, STATUS_HUGE, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        SET_VALUE(LW(3), LW(1))
        ADD(LW(1), 53)
        ADD(LW(3), 35)
        SET_VALUE(LW(4), LW(2))
        ADD(LW(2), 250)
    ELSE
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        SET_VALUE(LW(3), LW(1))
        ADD(LW(1), 150)
        ADD(LW(3), 80)
        SET_VALUE(LW(4), LW(2))
        ADD(LW(2), 400)
    END_IF
    CALL(evt_btl_camera_set_mode, 0, 3)
    CALL(evt_btl_camera_set_moveto, 0, LW(0), LW(1), LW(2), LW(0), LW(3), LW(4), 30, 11)
    CALL(evt_batstage_light_set_player_on, 0)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_CheckDataOfDataTable, -2, LW(0), UNIT_DATA_APPEAL_EVENT)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_RunDataEventChild, -2, UNIT_DATA_APPEAL_EVENT)
        CALL(evt_btl_camera_set_mode, 0, 0)
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
        CALL(evt_batstage_light_set_player_off)
        WAIT_FRAMES(60)
        RETURN
    END_IF
    CALL(btlevtcmd_GetBodyId, -2, LW(0))
    CALL(btlevtcmd_AnimeChangePoseType, -2, LW(0), 66)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_MARIO_APPEAL1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    WAIT_FRAMES(60)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(btlevtcmd_AudienceAppeal, -2)
    CALL(evt_batstage_light_set_player_off)
    WAIT_FRAMES(60)
    CALL(btlevtcmd_StartWaitEvent, LW(10))
    RETURN
EVT_END()

EVT_BEGIN(_Defence_core)
    CALL(btlevtcmd_GetUnitId, -2, LW(3))
    CALL(btlevtcmd_GetBodyId, -2, LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_PayWeaponCost, -2, LW(12))
    CALL(btlevtcmd_RunDataEventChild, -2, 7)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BTL_MARIO_DEFENCE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    LOOP(20)
        CALL(btlevtcmd_AddRotate, -2, 0, 36, 0)
        WAIT_FRAMES(1)
    END_LOOP
    CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_OnStatusFlag, -2, 16777216)
    CALL(btlevtcmd_AnimeChangePoseType, -2, LW(4), 45)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BTL_MARIO_DEFENCE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_ftof, 60, LW(0))
    CALL(btlevtcmd_AnnounceMessage, 0, 0, 0, STRING("btl_msg_defend_command_exec"), LW(0))
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

EVT_BEGIN(btldefaultevt_Escape)
    CALL(evt_btl_camera_set_mode, 0, 20)
    CALL(_btl_escape_reset_move_color_lv_player_unit_all)
    CALL(btlevtcmd_AcSetDifficulty, -2, 3)
    CALL(evt_sub_random, 99, LW(0))
    CALL(evt_sub_random, 6, LW(1))
    ADD(LW(1), 30)
    CALL(_get_escape_rate, LW(2))
    CALL(btlevtcmd_AcSetParamAll, 1, 180, LW(0), LW(1), 0, 4, 6, LW(2))
    CALL(btlevtcmd_AcSetFlag, 3)
    CALL(btlevtcmd_SetupAC, -2, 5, 1, 0)
    CALL(_set_escape_ac_hlp_msg)
    BEGIN_INLINE_EVENT
        CALL(btlevtcmd_GetPartnerId, -2, LW(10))
        IF_INT_NOT_EQUAL(LW(10), -1)
            CALL(btlevtcmd_CheckActStatus, LW(10), LW(0))
            IF_INT_NOT_EQUAL(LW(0), 0)
                CALL(btlevtcmd_GetBodyId, LW(10), LW(11))
                CALL(btlevtcmd_CalculateFaceDirection, LW(10), LW(11), -1, -1, 260, LW(0))
                CALL(btlevtcmd_ChangeFaceDirection, LW(10), LW(0))
                CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), 42)
                CALL(btlevtcmd_JumpSetting, LW(10), 20, FLOAT(0.0f), FLOAT(0.69921875f))
                CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
                CALL(btlevtcmd_JumpPosition, LW(10), LW(0), LW(1), LW(2), 16, -1)
                CALL(btlevtcmd_JumpPosition, LW(10), LW(0), LW(1), LW(2), 16, -1)
            END_IF
        END_IF
    END_INLINE_EVENT
    CALL(btlevtcmd_GetBodyId, -2, LW(11))
    CALL(btlevtcmd_CalculateFaceDirection, -2, LW(11), -1, -1, 260, LW(0))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(0))
    CALL(btlevtcmd_AnimeChangePoseType, -2, LW(11), 42)
    CALL(btlevtcmd_JumpSetting, -2, 20, FLOAT(0.0f), FLOAT(0.69921875f))
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 16, -1)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 16, -1)
    CALL(btlevtcmd_audience_code_escape)
    CALL(evt_snd_sfxon, STRING("SFX_MARIO_BATTLE_STAMP1"), LW(15))
    BEGIN_INLINE_EVENT
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 15)
        CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 180, -1, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(btlevtcmd_GetPartnerId, -2, LW(10))
        IF_INT_NOT_EQUAL(LW(10), -1)
            CALL(btlevtcmd_CheckActStatus, LW(10), LW(0))
            IF_INT_NOT_EQUAL(LW(0), 0)
                CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
                CALL(btlevtcmd_FaceDirectionSub, LW(10), LW(0), 15)
                CALL(btlevtcmd_MovePosition, LW(10), LW(0), LW(1), LW(2), 180, -1, 0)
            END_IF
        END_IF
    END_INLINE_EVENT
    CALL(btlevtcmd_StartAC, 1)
    CALL(btlevtcmd_ResultAC)
    CALL(btlevtcmd_StopAC)
    CALL(btlevtcmd_JumpSetting, -2, 20, FLOAT(0.0f), FLOAT(0.69921875f))
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 16, -1)
    CALL(btlevtcmd_GetResultAC, LW(6))
    IF_BITS_CLEAR(LW(6), 2)
        CALL(evt_snd_sfxoff, LW(15))
        CALL(evt_snd_sfxon, STRING("SFX_MARIO_RUN_AWAY_MISS1"), 0)
        BEGIN_SIBLING_EVENT_GET_ID(LW(15))
            CALL(btlevtcmd_GetPartnerId, -2, LW(10))
            IF_INT_NOT_EQUAL(LW(10), -1)
                CALL(btlevtcmd_CheckActStatus, LW(10), LW(0))
                IF_INT_NOT_EQUAL(LW(0), 0)
                    CALL(btlevtcmd_OffUnitFlag, LW(10), 2097152)
                    CALL(btlevtcmd_GetBodyId, LW(10), LW(11))
                    CALL(btlevtcmd_SetMoveSpeed, LW(10), FLOAT(10.0f))
                    CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
                    CALL(btlevtcmd_FaceDirectionAdd, LW(10), LW(0), 30)
                    CALL(btlevtcmd_MovePosition, LW(10), LW(0), LW(1), LW(2), 0, -1, 0)
                    CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), 60)
                    CALL(btlevtcmd_FaceDirectionAdd, LW(10), LW(0), 5)
                    CALL(btlevtcmd_MovePosition, LW(10), LW(0), LW(1), LW(2), 5, -1, 0)
                    CALL(btlevtcmd_FaceDirectionAdd, LW(10), LW(0), 10)
                    CALL(btlevtcmd_MovePosition, LW(10), LW(0), LW(1), LW(2), 60, -1, 0)
                    WAIT_FRAMES(25)
                    CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), 61)
                    WAIT_FRAMES(40)
                    CALL(btlevtcmd_OnUnitFlag, LW(10), 2097152)
                END_IF
            END_IF
        END_SIBLING_EVENT
        CALL(btlevtcmd_OffUnitFlag, LW(10), 2097152)
        CALL(btlevtcmd_GetBodyId, -2, LW(11))
        CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(10.0f))
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionAdd, -2, LW(0), 30)
        CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
        CALL(btlevtcmd_AnimeChangePoseType, -2, LW(11), 60)
        CALL(btlevtcmd_audience_code_escape_fail)
        CALL(btlevtcmd_FaceDirectionAdd, -2, LW(0), 5)
        CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 5, -1, 0)
        CALL(btlevtcmd_FaceDirectionAdd, -2, LW(0), 10)
        CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 60, -1, 0)
        WAIT_FRAMES(25)
        CALL(btlevtcmd_AnimeChangePoseType, -2, LW(11), 61)
        CALL(evt_btl_camera_set_mode, 0, 0)
        WAIT_FRAMES(40)
        CALL(btlevtcmd_OnUnitFlag, LW(10), 2097152)
        BEGIN_SIBLING_EVENT_GET_ID(LW(15))
            CALL(btlevtcmd_GetPartnerId, -2, LW(10))
            IF_INT_NOT_EQUAL(LW(10), -1)
                CALL(btlevtcmd_CheckActStatus, LW(10), LW(0))
                IF_INT_NOT_EQUAL(LW(0), 0)
                    CALL(btlevtcmd_GetBodyId, LW(10), LW(11))
                    CALL(btlevtcmd_CalculateFaceDirection, LW(10), LW(11), 0, 0, 4, LW(0))
                    CALL(btlevtcmd_ChangeFaceDirection, LW(10), LW(0))
                    CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), 42)
                    CALL(btlevtcmd_SetMoveSpeed, LW(10), FLOAT(3.0f))
                    CALL(btlevtcmd_GetHomePos, LW(10), LW(0), LW(1), LW(2))
                    CALL(btlevtcmd_MovePosition, LW(10), LW(0), LW(1), LW(2), 0, -1, 0)
                    CALL(btlevtcmd_StartWaitEvent, LW(10))
                END_IF
            END_IF
        END_SIBLING_EVENT
        CALL(btlevtcmd_CalculateFaceDirection, -2, LW(11), 0, 0, 4, LW(0))
        CALL(btlevtcmd_ChangeFaceDirection, -2, LW(0))
        CALL(btlevtcmd_AnimeChangePoseType, -2, LW(11), 42)
        CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(3.0f))
        CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
        CALL(btlevtcmd_StartWaitEvent, -2)
        CALL(btlevtcmd_WaitEventEnd, LW(15))
        RETURN
    END_IF
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_GetPartnerId, -2, LW(10))
        IF_INT_NOT_EQUAL(LW(10), -1)
            CALL(btlevtcmd_CheckActStatus, LW(10), LW(0))
            IF_INT_NOT_EQUAL(LW(0), 0)
                CALL(btlevtcmd_SetMoveSpeed, LW(10), FLOAT(10.0f))
                CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
                CALL(btlevtcmd_FaceDirectionAdd, LW(10), LW(0), 200)
                CALL(btlevtcmd_MovePosition, LW(10), LW(0), LW(1), LW(2), 0, -1, 0)
            END_IF
        END_IF
    END_SIBLING_EVENT
    CALL(evt_snd_sfxoff, LW(15))
    CALL(btlevtcmd_audience_code_escape_success)
    CALL(evt_snd_sfxon, STRING("SFX_MARIO_RUN_AWAY1"), 0)
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(10.0f))
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, -2, LW(0), 200)
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, 0, 0)
    SET_VALUE(LW(15), 0)
    CALL(btlevtcmd_check_battleflag, LW(0), 4194304)
    IF_INT_NOT_EQUAL(LW(0), 0)
        BEGIN_SIBLING_EVENT_GET_ID(LW(15))
            CALL_CHILD(PTR(_event_MajinaiEnd))
        END_SIBLING_EVENT
    END_IF
    IF_INT_NOT_EQUAL(LW(15), 0)
        CALL(btlevtcmd_WaitEventEnd, LW(15))
    END_IF
    CALL(btlevtcmd_InterruptStop, 0)
    RETURN
EVT_END()

EVT_BEGIN(btldefaultevt_Confuse)
    CALL(btlevtcmd_GetBodyId, -2, LW(0))
    CALL(btlevtcmd_AnimeChangePoseType, -2, LW(0), 5)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_GetConfuseActEvent, -2, LW(0))
    CALL_CHILD(LW(0))
    RETURN
EVT_END()

EVT_BEGIN(btldefaultevt_CantMoveZeroGravity)
    CALL(btlevtcmd_GetBodyId, -2, LW(0))
    CALL(btlevtcmd_AnimeChangePoseType, -2, LW(0), 64)
    CALL(evt_btl_camera_set_mode, 0, 7)
    CALL(evt_btl_camera_set_homing_unit, 0, -2, -1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(btlevtcmd_AnnounceMessage, 0, 0, 0, STRING("btl_msg_cant_move_zero_gravity"), 120)
    CALL(evt_btl_camera_set_mode, 0, 0)
    WAIT_FRAMES(20)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

EVT_BEGIN(btldefaultevt_SuitoruBadgeEffect)
    CALL(_get_heart_suitoru_point, -2, LW(5))
    IF_INT_GREATER_OR_EQUAL(LW(5), 1)
        BEGIN_SIBLING_EVENT
            CALL(btlevtcmd_GetBodyId, -2, LW(4))
            CALL(btlevtcmd_GetStatusMg, -2, LW(6))
            CALL(btlevtcmd_GetHeight, -2, LW(7))
            MULTIPLY_FLOAT(LW(7), LW(6))
            CALL(btlevtcmd_GetPartsPos, -2, LW(4), LW(0), LW(1), LW(2))
            ADD(LW(1), LW(7))
            CALL(evt_eff, 0, STRING("recovery"), 0, LW(0), LW(1), LW(2), LW(5), 0, 0, 0, 0, 0, 0, 0)
            WAIT_FRAMES(120)
            CALL(btlevtcmd_RecoverHp, -2, LW(4), LW(5))
        END_SIBLING_EVENT
        WAIT_FRAMES(125)
    END_IF
    CALL(_get_flower_suitoru_point, -2, LW(5))
    IF_INT_GREATER_OR_EQUAL(LW(5), 1)
        BEGIN_SIBLING_EVENT
            CALL(btlevtcmd_GetStatusMg, -2, LW(6))
            CALL(btlevtcmd_GetHeight, -2, LW(7))
            MULTIPLY_FLOAT(LW(7), LW(6))
            CALL(btlevtcmd_GetBodyId, -2, LW(4))
            CALL(btlevtcmd_GetPartsPos, -2, LW(4), LW(0), LW(1), LW(2))
            ADD(LW(1), LW(7))
            CALL(evt_eff, 0, STRING("recovery"), 1, LW(0), LW(1), LW(2), LW(5), 0, 0, 0, 0, 0, 0, 0)
            WAIT_FRAMES(120)
            CALL(btlevtcmd_RecoverFp, -2, LW(4), LW(5))
        END_SIBLING_EVENT
        WAIT_FRAMES(125)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(btldefaultevt_avoid_counter_event)
    CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), 49)
    BEGIN_SIBLING_EVENT_GET_ID(LW(0))
        CALL(evt_sub_intpl_init, 5, 0, 1800, 40)
        LOOP(40)
            CALL(evt_sub_intpl_get_value_para, LW(1), LW(2))
            CALL(btlevtcmd_SetRotate, LW(10), EVT_NULLPTR, LW(1), EVT_NULLPTR)
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_WaitEventEnd, LW(0))
    RETURN
EVT_END()

EVT_BEGIN(btldefaultevt_ProtectPartner)
    CALL(btlevtcmd_GetPartnerId, -2, LW(3))
    CALL(btlevtcmd_SetGuard, -2, LW(3))
    RETURN
EVT_END()

EVT_BEGIN(btldefaultevt_Dummy)
    RETURN
EVT_END()

EVT_BEGIN(btldefaultevt_Wait)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

EVT_BEGIN(btldefaultevt_LectureEnd)
    CALL(btlevtcmd_RunDataEventChild, -6, 70)
    RETURN
EVT_END()

EVT_BEGIN(btldefaultevt_Defend)
    SET_VALUE(LW(12), PTR(&badgeWeapon_DefenceCommand))
    CALL_CHILD(PTR(_Defence_core))
    RETURN
EVT_END()

EVT_BEGIN(btldefaultevt_DefendParty)
    SET_VALUE(LW(12), PTR(&badgeWeapon_DefenceCommandP))
    CALL_CHILD(PTR(_Defence_core))
    RETURN
EVT_END()

USER_FUNC(_backfire) {
    s32 id;
    BattleUnit* unit;
    BattleWork* wp = _battleWorkPointer;
    f32 x, y, z;
    s32 self;
    EffectEntry* eff;
    EffFireWork* fire;

    id = evtGetValue(event, event->args[0]);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(wp, id);
    if (!unit) {
        return EVT_RETURN_DONE;
    }
    if (isFirstCall) {
        BtlUnit_GetPos(unit, &x, &y, &z);
        event->userdatap[0] = effFireEntry(0, 0, x, y, z, 1.0f);
        event->userdata[1] = 0;
    }
    eff = event->userdatap[0];
    fire = (EffFireWork*)eff->userdata;
    event->userdata[1]++;
    if (event->userdata[1] >= 60) {
        effDelete(event->userdatap[0]);
        BattleDamageDirect(-5, unit, NULL, 1, 0, 0x118, 0, 1);
        self = BattleTransID(event, event->unitId);
        if (self != id) {
            return EVT_RETURN_DONE;
        }
        return EVT_RETURN_FINISH;
    }
    fire->scale = 1.6f * sinfd(event->userdata[1] * 3) + 0.4f;
    return EVT_RETURN_BLOCK;
}

static USER_FUNC(_anime_load) {
    BattlePartyAnimeLoad(_battleWorkPointer->commandMenu.partyKind);
    return EVT_RETURN_DONE;
}

static USER_FUNC(_anime_load_wait) {
    return BattlePartyAnimeLoad(_battleWorkPointer->commandMenu.partyKind) ? EVT_RETURN_DONE : EVT_RETURN_BLOCK;
}

USER_FUNC(_check_pose_stay) {
    BattleWork* wp = _battleWorkPointer;
    s32 out = event->args[0];
    BattleUnit* unit;
    s32 stay;
    s32 id;

    id = BattleTransID(event, -2);
    unit = BattleGetUnitPtr(wp, id);
    stay = FALSE;
    if (unit->currentKind == UNIT_MARIO) {
        stay = TRUE;
        if (wp->badgeEquippedFlags & 1) {
            if (!(unit->mTokenFlags & kConfusedAction)) {
                stay = FALSE;
            }
        }
    }
    evtSetValue(event, out, stay);
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_audience_code_escape) {
    BattleAudience_Case_Escape();
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_audience_code_escape_success) {
    BattleAudience_Case_Escape_Good();
    return EVT_RETURN_DONE;
}

USER_FUNC(btlevtcmd_audience_code_escape_fail) {
    BattleAudience_Case_Escape_Bad();
    return EVT_RETURN_DONE;
}

USER_FUNC(_btl_escape_reset_move_color_lv_player_unit_all) {
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;

    unit = BattleGetMarioPtr(wp);
    unit->mMoveColorLv = 0xFF;
    unit = BattleGetPartyPtr(wp);
    if (unit) {
        unit->mMoveColorLv = 0xFF;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(_set_escape_ac_hlp_msg) {
    BattleAcHelpSetHelp("msg_ac_escape");
    BattleAcHelpSetDispType(2);
    return EVT_RETURN_DONE;
}

USER_FUNC(_get_escape_rate) {
    BattleWork* wp = _battleWorkPointer;
    evtSetValue(event, event->args[0],
                wp->fbatInfo->setup->groupSetup->enemyData->mUnitKindParams->runRate);
    return EVT_RETURN_DONE;
}

USER_FUNC(_get_heart_suitoru_point) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    s32 id = *args++;
    s32 out = *args++;

    BattleUnit* unit;
    s32 point;

    unit = BattleGetUnitPtr(wp, BattleTransID(event, id));
    point = 0;
    if (unit) {
        point = unit->badgesEquipped.HpDrain;
        if (point > unit->totalDamageDealt) {
            point = unit->totalDamageDealt;
        }
    }
    evtSetValue(event, out, point);
    return EVT_RETURN_DONE;
}

USER_FUNC(_get_flower_suitoru_point) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    s32 id = *args++;
    s32 out = *args++;

    BattleUnit* unit;
    s32 point;

    unit = BattleGetUnitPtr(wp, BattleTransID(event, id));
    point = 0;
    if (unit) {
        point = unit->badgesEquipped.FpDrain;
        if (point > unit->totalDamageDealt) {
            point = unit->totalDamageDealt;
        }
    }
    evtSetValue(event, out, point);
    return EVT_RETURN_DONE;
}
