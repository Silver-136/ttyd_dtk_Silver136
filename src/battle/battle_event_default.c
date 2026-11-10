// Default battle events: standard guard, escape, damage and badge-effect scripts shared by all units.

#include "version.h"
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
#include "system.h"
#include "battle/battle_damage.h"

static USER_FUNC(_anime_load);
static USER_FUNC(_anime_load_wait);
static USER_FUNC(_backfire);
static USER_FUNC(_btl_escape_reset_move_color_lv_player_unit_all);
static USER_FUNC(_check_pose_stay);
static USER_FUNC(_get_escape_rate);
static USER_FUNC(_get_flower_suitoru_point);
static USER_FUNC(_get_heart_suitoru_point);
static USER_FUNC(_set_escape_ac_hlp_msg);
USER_FUNC(btlevtcmd_audience_code_escape);
USER_FUNC(btlevtcmd_audience_code_escape_fail);
USER_FUNC(btlevtcmd_audience_code_escape_success);

EvtWord btldefaultevt_Damage[1133];
EvtWord btldefaultevt_ChangeParty[174];
EvtWord btldefaultevt_BiribiriMove[29];
EvtWord btldefaultevt_guard_move[85];
EvtWord btldefaultevt_guard_return[98];

#if VERSION <= VERSION_DEMO_JP
EvtWord btldefaultevt_Appeal[114];
#else
EvtWord btldefaultevt_Appeal[145];
#endif

static EvtWord _Defence_core[81];
EvtWord btldefaultevt_Escape[657];
EvtWord btldefaultevt_Confuse[19];
EvtWord btldefaultevt_CantMoveZeroGravity[44];

#if VERSION <= VERSION_DEMO_JP
EvtWord btldefaultevt_SuitoruBadgeEffect[100];
#else
EvtWord btldefaultevt_SuitoruBadgeEffect[122];
#endif

EvtWord btldefaultevt_avoid_counter_event[34];
EvtWord btldefaultevt_ProtectPartner[10];
EvtWord btldefaultevt_Dummy[2];
EvtWord btldefaultevt_Wait[5];
EvtWord btldefaultevt_LectureEnd[6];
EvtWord btldefaultevt_Defend[7];
EvtWord btldefaultevt_DefendParty[7];

/**
 * Standard damage reaction that unit damage scripts call with LW(10) = unit and LW(11) = part:
 * plays the reaction for the damage pattern and damage code (hurt pose, Gale Force and Fright blow-away, slaps,
 * counters, guards, lucky and dodged misses, using the unit's data-table events where it has them), then the KO,
 * the backfire (damage code bit 0x4000) or the guard return,
 * and finally walks or floats the unit back to its home position and restarts its wait event.
 */
EVT_BEGIN(btldefaultevt_Damage)
    // The unit is set upright; a frozen unit that shatters loses its freeze and bursts into ice
    // (the shatter chance is 0, so this never happens).
    CALL(btlevtcmd_SetRotate, LW(10), 0, 0, 0)
    CALL(btlevtcmd_CheckDamageCode, LW(10), DAMAGE_RESULT_FREEZE_CRUSH, LW(1))
    IF_INT_NOT_EQUAL(LW(1), 0)
        CALL(btlevtcmd_OnOffStatus, LW(10), STATUS_FREEZE, 0, 0, 0)
        CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
        CALL(evt_eff64, STRING(""), STRING("freeze_n64"), 0, LW(0), LW(1), LW(2), FLOAT(1.0f), 1, 0, 0, 0, 0, 0, 0)
    END_IF

    // Damage pattern 2 (Yoshi's Gulp, Flurrie's Lip Lock, the Quake Hammer moves):
    // the unit freezes in its hurt pose for good, and the attacker's script takes it from here.
    CALL(btlevtcmd_CheckDamagePattern, LW(10), ATTACKPATTERN_DAMAGE_MOTION, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(LW(14), POSE_TYPE_DAMAGE)
        CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), LW(14))
        BEGIN_SIBLING_EVENT
            CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_DAMAGE_POSE_EVENT)
        END_SIBLING_EVENT
        LABEL(0)
        WAIT_FRAMES(1)
        GOTO(0)
    END_IF

    // Damage pattern 3 (Flurrie's Gale Force on a unit it can blow away):
    // the unit trembles in its hurt pose until the attack ends.
    CALL(btlevtcmd_CheckDamagePattern, LW(10), ATTACKPATTERN_DAMAGE_MOTION_BLOW, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(LW(14), POSE_TYPE_DAMAGE)
        CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), LW(14))
        BEGIN_SIBLING_EVENT
            CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_DAMAGE_POSE_EVENT)
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

    // Damage pattern 20 (Ms. Mowz's Kiss Thief): the unit holds pose 5 until the attack ends.
    CALL(btlevtcmd_CheckDamagePattern, LW(10), ATTACKPATTERN_CHUCHURINA_ITEMSTEAL, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(LW(14), POSE_TYPE_CONFUSE)
        CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), LW(14))
        BEGIN_SIBLING_EVENT
            CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_DAMAGE_POSE_EVENT)
        END_SIBLING_EVENT
        CALL(btlevtcmd_WaitAttackEnd)
        GOTO(10)
    END_IF

    // Yoshi's Gulp hitting the unit's body (damage pattern 7) runs its data-table event 54 if it has one.
    CALL(btlevtcmd_CheckDamagePattern, LW(10), ATTACKPATTERN_SWALLOW_SAMBO_BODY, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(2), UNIT_DATA_SWALLOW_BODY_EVENT)
        IF_INT_NOT_EQUAL(LW(2), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_SWALLOW_BODY_EVENT)
        END_IF
    END_IF

    // The Ultra Hammer's acrobatic hit (damage pattern 18) plays its own spin reaction.
    CALL(btlevtcmd_CheckDamagePattern, LW(10), ATTACKPATTERN_ULTRA_HAMMER_ACROBAT, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(subset_ultra_hammer_acrobat_damage_event)
        GOTO(10)
    END_IF

    // A guarded hit: the unit holds its guard pose for half a second.
    CALL(btlevtcmd_CheckDamageCode, LW(10), DAMAGE_RESULT_GUARD, LW(1))
    IF_INT_NOT_EQUAL(LW(1), 0)
        CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), POSE_TYPE_DAMAGE_GUARDED)
        WAIT_FRAMES(30)
        GOTO(10)
    END_IF

    // Attacks stopped before they land: codes 31-37 (this unit's attack was stopped by its target's spikes or
    // flames) run data-table events 37-43, and a Superguard (42) runs event 47.
    CALL(btlevtcmd_GetDamageCode, LW(10), LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(DAMAGE_CODE_AVOID_TOP_SPIKY)
        CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_AVOID_THORN_HEAD_EVENT)
        GOTO(10)
        CASE_EQUAL(DAMAGE_CODE_AVOID_PREEMPTIVE_SPIKY)
        CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_AVOID_THORN_FRONT_EVENT)
        GOTO(10)
        CASE_EQUAL(DAMAGE_CODE_AVOID_FRONT_SPIKY)
        CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_AVOID_THORN_EVENT)
        GOTO(10)
        CASE_EQUAL(DAMAGE_CODE_AVOID_FIERY)
        CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_AVOID_FIRE_EVENT)
        GOTO(10)
        CASE_EQUAL(DAMAGE_CODE_AVOID_ICY)
        CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_AVOID_ICE_EVENT)
        GOTO(10)
        CASE_EQUAL(DAMAGE_CODE_AVOID_ELECTRIC)
        CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_AVOID_ELECTRIC_EVENT)
        GOTO(10)
        CASE_EQUAL(DAMAGE_CODE_AVOID_POISON)
        CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_AVOID_POISON_EVENT)
        GOTO(10)
        CASE_EQUAL(DAMAGE_CODE_SUPERGUARDED)
        CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_AVOID_COUNTER_EVENT)
        GOTO(10)
        CASE_EQUAL(DAMAGE_CODE_LUCKY)

        // Lucky (code 38): the unit's own lucky event, else the lucky jingle.
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(2), UNIT_DATA_AVOID_LUCKY_EVENT)
        IF_INT_NOT_EQUAL(LW(2), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_AVOID_LUCKY_EVENT)
        ELSE
            CALL(evt_snd_sfxon, STRING("SFX_SYSTEM_LUCKY1"), 0)
        END_IF
        GOTO(10)
        CASE_EQUAL(DAMAGE_CODE_DODGY)

        // Dodgy (code 39): the unit's own dodgy event 44, else a miss sound and the paper-dodge animation.
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(2), UNIT_DATA_AVOID_DODGY_EVENT)
        IF_INT_NOT_EQUAL(LW(2), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_AVOID_DODGY_EVENT)
        ELSE
            CALL(btlevtcmd_CheckCommandUnit, LW(10), LW(0))
            IF_INT_NOT_EQUAL(LW(0), 0)
                CALL(evt_snd_sfxon, STRING("SFX_MARIO_AVOID1"), 0)
            ELSE
                CALL(evt_snd_sfxon, STRING("SFX_BTL_ATTACK_MISS1"), 0)
            END_IF
            CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), POSE_TYPE_AVOID_DODGY)
            CALL(btlevtcmd_AnimeChangePaperGroup, LW(10), LW(11), STRING("p_b_st"), 1)
            CALL(btlevtcmd_CheckStatus, LW(10), STATUS_DEFENSE_DOWN, LW(2))
            IF_INT_NOT_EQUAL(LW(2), 0)
                CALL(btlevtcmd_AnimeChangePaperAnime, LW(10), LW(11), STRING("PM_F_3A"))
                CALL(btlevtcmd_ftomsec, 37, LW(0))
                WAIT_MS(LW(0))
                CALL(btlevtcmd_AnimeChangePaperAnime, LW(10), LW(11), STRING("PM_F_3B"))
                CALL(btlevtcmd_ftomsec, 22, LW(0))
                WAIT_MS(LW(0))
            ELSE
                CALL(btlevtcmd_CheckStatus, LW(10), STATUS_DEFENSE_UP, LW(2))
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
        CASE_EQUAL(DAMAGE_CODE_MISS)

        // Miss (code 40): the unit's data-table miss event 46.
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(2), UNIT_DATA_AVOID_MISS_EVENT)
        IF_INT_NOT_EQUAL(LW(2), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_AVOID_MISS_EVENT)
        ELSE
        END_IF
    END_SWITCH

    // Ms. Mowz's Love Slap (damage patterns 21 and 22) plays the slap from the left or the right.
    CALL(btlevtcmd_CheckDamagePattern, LW(10), ATTACKPATTERN_CHUCHURINA_BINTA_L, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(LW(14), POSE_TYPE_DAMAGE)
        CALL_CHILD(subsetevt_binta_damage_left)
        CALL(btlevtcmd_WaitAttackEnd)
        RETURN
    END_IF
    CALL(btlevtcmd_CheckDamagePattern, LW(10), ATTACKPATTERN_CHUCHURINA_BINTA_R, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(LW(14), POSE_TYPE_DAMAGE)
        CALL_CHILD(subsetevt_binta_damage_right)
        CALL(btlevtcmd_WaitAttackEnd)
        RETURN
    END_IF

    // The main reaction, chosen by the hit type.
    CALL(btlevtcmd_GetDamageCode, LW(10), LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(DAMAGE_CODE_FIRST_ATTACK_POS)

        // The battle opened with Mario's or the partner's First Strike on this unit: data-table event 48.
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(2), UNIT_DATA_FIRST_ATTACK_POS_EVENT)
        IF_INT_NOT_EQUAL(LW(2), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_FIRST_ATTACK_POS_EVENT)
        END_IF
        CALL(btlevtcmd_StartWaitEvent, LW(10))
        RETURN
        CASE_OR_EQUAL(DAMAGE_CODE_NO_DAMAGE)
        CASE_OR_EQUAL(DAMAGE_CODE_ELECTRIC)
        CASE_OR_EQUAL(DAMAGE_CODE_ICE)
        CASE_OR_EQUAL(DAMAGE_CODE_NORMAL)

        // Normal, ice, electric and no-damage hits: the HP gauge shows and the hurt reaction plays
        // (hurt pose 38 for damage pattern 1).
        CALL(btlevtcmd_OnUnitFlag, LW(10), BATTLE_UNIT_FLAG_SHOW_HP)
        SET_VALUE(LW(14), POSE_TYPE_DAMAGE)
        SET_VALUE(LW(15), -1)
        CALL(btlevtcmd_CheckDamagePattern, LW(10), ATTACKPATTERN_DAMAGE_SURPRISE, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            SET_VALUE(LW(14), POSE_TYPE_DAMAGE_SURPRISED)
        END_IF
        BEGIN_SIBLING_EVENT
            CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_DAMAGE_POSE_EVENT)
        END_SIBLING_EVENT
        CALL_CHILD(subsetevt_damage)
        CASE_END
        CASE_EQUAL(DAMAGE_CODE_STATUS_ONLY)

        // A hit that only inflicted a status: the unit shows pose 35 for 45 frames if it has one.
        CALL(btlevtcmd_CheckDefinePosetype, LW(10), LW(11), POSE_TYPE_DAMAGE_STATUS, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            SET_VALUE(LW(14), POSE_TYPE_DAMAGE_STATUS)
            CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), LW(14))
            BEGIN_SIBLING_EVENT
                CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_DAMAGE_POSE_EVENT)
            END_SIBLING_EVENT
            WAIT_FRAMES(45)
        ELSE
            WAIT_FRAMES(45)
        END_IF
        CASE_EQUAL(DAMAGE_CODE_FIRE)

        // Fire and explosion hits: the unit's data-table events 11 and 12,
        // else the normal hurt reaction with pose 29.
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(2), UNIT_DATA_DAMAGE_FIRE_EXPLOSION_EVENT)
        IF_INT_NOT_EQUAL(LW(2), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_DAMAGE_FIRE_EXPLOSION_EVENT)
        ELSE
            CALL(btlevtcmd_OnUnitFlag, LW(10), BATTLE_UNIT_FLAG_SHOW_HP)
            SET_VALUE(LW(14), POSE_TYPE_DAMAGE_FIRE)
            SET_VALUE(LW(15), -1)
            BEGIN_SIBLING_EVENT
                CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_DAMAGE_POSE_EVENT)
            END_SIBLING_EVENT
            CALL_CHILD(subsetevt_damage)
        END_IF
        CASE_EQUAL(DAMAGE_CODE_EXPLOSION)
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(2), UNIT_DATA_DAMAGE_EXPLOSION_EVENT)
        IF_INT_NOT_EQUAL(LW(2), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_DAMAGE_EXPLOSION_EVENT)
        ELSE
            CALL(btlevtcmd_OnUnitFlag, LW(10), BATTLE_UNIT_FLAG_SHOW_HP)
            SET_VALUE(LW(14), POSE_TYPE_DAMAGE_FIRE)
            SET_VALUE(LW(15), -1)
            BEGIN_SIBLING_EVENT
                CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_DAMAGE_POSE_EVENT)
            END_SIBLING_EVENT
            CALL_CHILD(subsetevt_damage)
        END_IF
        CASE_EQUAL(DAMAGE_CODE_COUNTER_TOP_SPIKY)

        // This unit is an attacker hurt by its target's counter (codes 43-55: spikes, fire, ice, electricity,
        // explosions, poison, Payback, Hold Fast): data-table events 24-36, else the standard counter reaction.
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(2), UNIT_DATA_COUNTER_TOP_SPIKY_EVENT)
        IF_INT_NOT_EQUAL(LW(2), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_COUNTER_TOP_SPIKY_EVENT)
        ELSE
            CALL_CHILD(subsetevt_counter_damage)
        END_IF
        CASE_EQUAL(DAMAGE_CODE_COUNTER_PREEMPTIVE_SPIKY)
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(2), UNIT_DATA_COUNTER_PREEMPTIVE_SPIKY_EVENT)
        IF_INT_NOT_EQUAL(LW(2), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_COUNTER_PREEMPTIVE_SPIKY_EVENT)
        ELSE
            CALL_CHILD(subsetevt_counter_damage)
        END_IF
        CASE_EQUAL(DAMAGE_CODE_COUNTER_FRONT_SPIKY)
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(2), UNIT_DATA_COUNTER_FRONT_SPIKY_EVENT)
        IF_INT_NOT_EQUAL(LW(2), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_COUNTER_FRONT_SPIKY_EVENT)
        ELSE
            CALL_CHILD(subsetevt_counter_damage)
        END_IF
        CASE_EQUAL(DAMAGE_CODE_COUNTER_FIERY)
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(2), UNIT_DATA_COUNTER_FIERY_EVENT)
        IF_INT_NOT_EQUAL(LW(2), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_COUNTER_FIERY_EVENT)
        ELSE
            CALL_CHILD(subsetevt_counter_damage)
        END_IF
        CASE_EQUAL(DAMAGE_CODE_COUNTER_ICY)
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(2), UNIT_DATA_COUNTER_ICY_EVENT)
        IF_INT_NOT_EQUAL(LW(2), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_COUNTER_ICY_EVENT)
        ELSE
            CALL_CHILD(subsetevt_counter_damage)
        END_IF
        CASE_EQUAL(DAMAGE_CODE_COUNTER_ELECTRIC)
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(2), UNIT_DATA_COUNTER_ELECTRIC_EVENT)
        IF_INT_NOT_EQUAL(LW(2), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_COUNTER_ELECTRIC_EVENT)
        ELSE
            CALL_CHILD(subsetevt_counter_damage)
        END_IF
        CASE_EQUAL(DAMAGE_CODE_COUNTER_EXPLOSIVE)
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(2), UNIT_DATA_COUNTER_EXPLOSIVE_EVENT)
        IF_INT_NOT_EQUAL(LW(2), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_COUNTER_EXPLOSIVE_EVENT)
        ELSE
            CALL_CHILD(subsetevt_counter_damage)
        END_IF
        CASE_EQUAL(DAMAGE_CODE_COUNTER_VOLATILE_EXPLOSIVE)
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(2), UNIT_DATA_COUNTER_VOLATILE_EXPLOSIVE_EVENT)
        IF_INT_NOT_EQUAL(LW(2), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_COUNTER_VOLATILE_EXPLOSIVE_EVENT)
        ELSE
            CALL_CHILD(subsetevt_counter_damage)
        END_IF
        CASE_EQUAL(DAMAGE_CODE_COUNTER_POISON)
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(2), UNIT_DATA_COUNTER_POISON_EVENT)
        IF_INT_NOT_EQUAL(LW(2), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_COUNTER_POISON_EVENT)
        ELSE
            CALL_CHILD(subsetevt_counter_damage)
        END_IF
        CASE_EQUAL(DAMAGE_CODE_COUNTER_ELECTRIFIED)
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(2), UNIT_DATA_COUNTER_ELECTRIFIED_EVENT)
        IF_INT_NOT_EQUAL(LW(2), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_COUNTER_ELECTRIFIED_EVENT)
        ELSE
            CALL_CHILD(subsetevt_counter_damage)
        END_IF
        CASE_EQUAL(DAMAGE_CODE_COUNTER_PAYBACK)
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(2), UNIT_DATA_COUNTER_PAYBACK_EVENT)
        IF_INT_NOT_EQUAL(LW(2), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_COUNTER_PAYBACK_EVENT)
        ELSE
            CALL_CHILD(subsetevt_counter_damage)
        END_IF
        CASE_EQUAL(DAMAGE_CODE_COUNTER_HOLD_FAST)
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(2), UNIT_DATA_COUNTER_HOLD_FAST_EVENT)
        IF_INT_NOT_EQUAL(LW(2), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_COUNTER_HOLD_FAST_EVENT)
        ELSE
            CALL_CHILD(subsetevt_counter_damage)
        END_IF
        CASE_EQUAL(DAMAGE_CODE_SUPERGUARD_COUNTER)
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(2), UNIT_DATA_COUNTER_SUPERGUARD_EVENT)
        IF_INT_NOT_EQUAL(LW(2), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_COUNTER_SUPERGUARD_EVENT)
        ELSE
            CALL_CHILD(subsetevt_counter_damage)
        END_IF
        CASE_EQUAL(DAMAGE_CODE_PROTECTED)
        CALL_CHILD(btldefaultevt_avoid_guard)
        RETURN
        CASE_EQUAL(DAMAGE_CODE_BODYGUARD_MOVE)
        CALL_CHILD(btldefaultevt_guard_move)
        CASE_EQUAL(DAMAGE_CODE_FRIGHT)
        CALL_CHILD(subsetevt_terror)
        RETURN
        CASE_EQUAL(DAMAGE_CODE_GALE_FORCE)
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(2), UNIT_DATA_BLOW_DEAD_EVENT)
        IF_INT_NOT_EQUAL(LW(2), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_BLOW_DEAD_EVENT)
        ELSE
            CALL_CHILD(subsetevt_blow_dead)
        END_IF
        RETURN
        CASE_DEFAULT
    END_SWITCH
    LABEL(10)

    // After the reaction: a defeated unit plays its KO;
    // a unit at 0 HP that is not out (like a Dull Bones pile) runs its own event 15.
    CALL(btlevtcmd_CheckDamageCode, LW(10), DAMAGE_RESULT_DEATH, LW(1))
    IF_INT_NOT_EQUAL(LW(1), 0)
        WAIT_FRAMES(30)
        CALL_CHILD(subsetevt_dead)
        RETURN
    ELSE
        CALL(btlevtcmd_CheckDamageCode, LW(10), DAMAGE_RESULT_ZERO_HP, LW(1))
        IF_INT_NOT_EQUAL(LW(1), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_FAKE_DEAD_EVENT)
            RETURN
        END_IF
    END_IF

    // A backfiring move finishes with _backfire; a bodyguard that took the hit goes back with guard_return.
    CALL(btlevtcmd_CheckDamageCode, LW(10), DAMAGE_RESULT_BACKFIRE, LW(1))
    IF_INT_NOT_EQUAL(LW(1), 0)
        CALL(_backfire, LW(10))
        RETURN
    END_IF
    CALL(btlevtcmd_CheckDamageCode, LW(10), DAMAGE_RESULT_GUARD_RETURN, LW(1))
    IF_INT_NOT_EQUAL(LW(1), 0)
        CALL_CHILD(btldefaultevt_guard_return)
        RETURN
    END_IF

    // Unless this was the move's last hit, the unit waits for the whole attack to end.
    CALL(btlevtcmd_CheckDamageCode, LW(10), DAMAGE_RESULT_FINAL_HIT, LW(1))
    IF_INT_EQUAL(LW(1), 0)
        CALL(btlevtcmd_WaitAttackEnd)
    END_IF
    LABEL(99)

    // A floating unit flies back to its home position if the hit moved it.
    CALL(btlevtcmd_CheckAttribute, LW(10), kFloating, LW(0))
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
        SET_VALUE(LW(14), POSE_TYPE_RUN)
        CALL(btlevtcmd_SetMoveSpeed, LW(10), FLOAT(4.0f))
        CALL(btlevtcmd_GetHomePos, LW(10), LW(2), LW(3), LW(4))
        CALL(btlevtcmd_GetMoveFrame, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, LW(7))
        IF_INT_GREATER(LW(7), 0)
            CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), LW(14))
            CALL(btlevtcmd_DivePosition, LW(10), LW(2), LW(3), LW(4), 0, 0, EASING_LINEAR, 0, MOVE_SOUND_FROM_TABLE)
        END_IF
        LABEL(101)
    ELSE
        // A ground unit walks back to its home position if the hit moved it.
        CALL(btlevtcmd_GetPos, LW(10), LW(5), LW(6), LW(7))
        CALL(btlevtcmd_GetHomePos, LW(10), LW(2), LW(3), LW(4))
        IF_INT_EQUAL(LW(2), LW(5))
            IF_INT_EQUAL(LW(3), LW(6))
                IF_INT_EQUAL(LW(4), LW(7))
                    GOTO(102)
                END_IF
            END_IF
        END_IF
        SET_VALUE(LW(14), POSE_TYPE_WALK)
        CALL(btlevtcmd_SetMoveSpeed, LW(10), FLOAT(4.0f))
        CALL(btlevtcmd_GetHomePos, LW(10), LW(2), LW(3), LW(4))
        CALL(btlevtcmd_GetMoveFrame, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, LW(7))
        IF_INT_GREATER(LW(7), 0)
            CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), LW(14))
            CALL(btlevtcmd_MovePosition, LW(10), LW(2), LW(3), LW(4), 0, MOVE_SOUND_FROM_TABLE, 0)
        END_IF
        LABEL(102)
    END_IF

    // The unit faces its usual way and its idle animation restarts.
    CALL(btlevtcmd_ResetFaceDirection, LW(10))
    CALL(btlevtcmd_StartWaitEvent, LW(10))
    RETURN
EVT_END()

/**
 * Partner switch from the command menu: loads the new partner's animations,
 * shrinks the old partner as it jumps to Mario, swaps in the new one with a cheer from the audience,
 * grows it as it jumps to its home position and removes the old unit.
 * The switch ends Mario's turn unless Quick Change is equipped (and he is not confused).
 */
EVT_BEGIN(btldefaultevt_ChangeParty)
    CALL(_anime_load)
    CALL(btlevtcmd_GetPartyId, UNIT_ID_SELF, LW(3))
    CALL(btlevtcmd_GetBodyId, LW(3), LW(4))
    CALL(btlevtcmd_SetFallAccel, LW(3), FLOAT(0.69921875f))
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetMarioId, UNIT_ID_SELF, LW(8))
    CALL(btlevtcmd_GetPos, LW(8), LW(5), LW(6), LW(7))
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_SetScale, LW(3), FLOAT(1.0f), FLOAT(1.0f), FLOAT(1.0f))
        LOOP(18)
            CALL(btlevtcmd_AddScale, LW(3), FLOAT(-0.049804688f), FLOAT(-0.049804688f), FLOAT(-0.049804688f))
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    CALL(evt_snd_sfxon, STRING("SFX_BTL_PARTY_IN1"), 0)
    CALL(btlevtcmd_JumpPosition, LW(3), LW(5), LW(6), LW(7), 20, MOVE_SOUND_NONE)
    CALL(_anime_load_wait)
    CALL(btlevtcmd_ChangeParty, LW(9))
    CALL(btlevtcmd_OffUnitFlag, LW(9), BATTLE_UNIT_FLAG_FLOAT_ACTIVE)
    CALL(btlevtcmd_GetBodyId, LW(9), LW(0))
    CALL(btlevtcmd_AnimeChangePoseFromTable, LW(9), LW(0))
    CALL(btlevtcmd_SetPos, LW(9), LW(5), LW(6), LW(7))
    CALL(btlevtcmd_SetFallAccel, LW(9), FLOAT(0.69921875f))
    CALL(btlevtcmd_CheckDataOfDataTable, LW(9), LW(0), UNIT_DATA_CHANGE_PARTY_SPAWN_INIT_EVENT)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_RunDataEventChild, LW(9), UNIT_DATA_CHANGE_PARTY_SPAWN_INIT_EVENT)
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
    CALL(btlevtcmd_JumpPosition, LW(9), LW(0), LW(1), LW(2), 20, MOVE_SOUND_NONE)
    CALL(btlevtcmd_OnUnitFlag, LW(9), BATTLE_UNIT_FLAG_FLOAT_ACTIVE)
    CALL(btlevtcmd_KillUnit, LW(3), 0)
    CALL(btlevtcmd_SortPhaseMoveTable)
    CALL(_check_pose_stay, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    END_IF
    RETURN
EVT_END()

#if VERSION <= VERSION_DEMO_JP
/** The short scripts sit where the early demos' .data places them (JP and later keep them in .sdata). */
EVT_BEGIN(btldefaultevt_ProtectPartner)
    CALL(btlevtcmd_GetPartnerId, UNIT_ID_SELF, LW(3))
    CALL(btlevtcmd_SetGuard, UNIT_ID_SELF, LW(3))
    RETURN
EVT_END()
#endif

/**
 * Jitters the unit sideways for electric-shock paralysis ("biribiri"), then restarts its wait event.
 * battle_seq would run it for Electric units, but its chance there is irand(100) < 0, so it never plays.
 */
EVT_BEGIN(btldefaultevt_BiribiriMove)
    CALL(evt_sub_intpl_init, EASING_WAVE_SHRINKING_FASTER, 0, 5, 16)
    LABEL(0)
    CALL(evt_sub_intpl_get_value)
    CALL(btlevtcmd_SetDispOffset, UNIT_ID_SELF, LW(0), 0, 0)
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    CALL(btlevtcmd_StartWaitEvent, LW(10))
    RETURN
EVT_END()

/**
 * Moves a unit (LW(10), part LW(11)) in front of the unit it protects, running or diving if it floats,
 * and ends in the guard pose; run for damage code 56.
 */
EVT_BEGIN(btldefaultevt_guard_move)
    CALL(btlevtcmd_AnimeCmpPoseType, LW(10), LW(11), POSE_TYPE_RUN, 40, LW(2))
    IF_INT_EQUAL(LW(2), 0)
        CALL(btlevtcmd_AnimeSetMotionSpeed, LW(10), LW(11), FLOAT(2.0f))
    END_IF
    CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), POSE_TYPE_RUN)
    CALL(btlevtcmd_CheckAttribute, LW(10), kFloating, LW(6))
    IF_INT_NOT_EQUAL(LW(6), 0)
        CALL(btlevtcmd_GetProtectId, LW(10), LW(6))
        CALL(btlevtcmd_GetHomePos, LW(6), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_DivePosition, LW(10), LW(0), LW(1), LW(2), 20, 0, EASING_LINEAR, 0, MOVE_SOUND_FROM_TABLE)
    ELSE
        CALL(btlevtcmd_GetProtectId, LW(10), LW(6))
        CALL(btlevtcmd_GetHomePos, LW(6), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_MovePosition, LW(10), LW(0), LW(1), LW(2), 20, MOVE_SOUND_FROM_TABLE, 0)
    END_IF
    CALL(btlevtcmd_AnimeSetMotionSpeed, LW(10), LW(11), FLOAT(1.0f))
    CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), POSE_TYPE_BODYGUARD)
    CALL(btlevtcmd_GuardMoveEnd)
    RETURN
EVT_END()

/**
 * After a protecting unit has taken the hit, moves it back to its home position,
 * ends the guard and restarts its wait event.
 */
EVT_BEGIN(btldefaultevt_guard_return)
    CALL(btlevtcmd_WaitAttackEnd)
    CALL(btlevtcmd_CheckGuardMotionEnable, LW(10), LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_AnimeCmpPoseType, LW(10), LW(11), POSE_TYPE_RUN, 40, LW(2))
        IF_INT_EQUAL(LW(2), 0)
            CALL(btlevtcmd_AnimeSetMotionSpeed, LW(10), LW(11), FLOAT(2.0f))
        END_IF
        CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), POSE_TYPE_RUN)
    END_IF
    CALL(btlevtcmd_CheckAttribute, LW(10), kFloating, LW(6))
    IF_INT_NOT_EQUAL(LW(6), 0)
        CALL(btlevtcmd_GetHomePos, LW(10), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_DivePosition, LW(10), LW(0), LW(1), LW(2), 20, 0, EASING_LINEAR, 0, MOVE_SOUND_FROM_TABLE)
    ELSE
        CALL(btlevtcmd_GetHomePos, LW(10), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_MovePosition, LW(10), LW(0), LW(1), LW(2), 20, MOVE_SOUND_FROM_TABLE, 0)
    END_IF
    CALL(btlevtcmd_CheckGuardMotionEnable, LW(10), LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_AnimeSetMotionSpeed, LW(10), LW(11), FLOAT(1.0f))
        CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), POSE_TYPE_DEFEND)
    END_IF
    CALL(btlevtcmd_GuardMoveEnd)
    CALL(btlevtcmd_StartWaitEvent, LW(10))
    RETURN
EVT_END()

#if VERSION <= VERSION_DEMO_JP
/** Action for a unit that does nothing this turn: restarts its wait event. */
EVT_BEGIN(btldefaultevt_Wait)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()
/** Action that ends a tutorial battle turn: runs the lecture unit's data-table event 70 (69 in the early demos). */
EVT_BEGIN(btldefaultevt_LectureEnd)
    CALL(btlevtcmd_RunDataEventChild, UNIT_ID_LECTURE, 69)
    RETURN
EVT_END()
#endif

/**
 * Appeal command: the camera moves in on the unit (higher and farther when it is Huge, JP and later), then its
 * own appeal event if it has one, else the appeal pose and sound and the audience's appeal reaction.
 */
EVT_BEGIN(btldefaultevt_Appeal)
    CALL(btlevtcmd_GetBodyId, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(0), POSE_TYPE_NEUTRAL)
#if VERSION >= VERSION_JP
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_HUGE, LW(0))
    IF_INT_EQUAL(LW(0), 0)
#endif
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        SET_VALUE(LW(3), LW(1))
        ADD(LW(1), 53)
        ADD(LW(3), 35)
        SET_VALUE(LW(4), LW(2))
        ADD(LW(2), 250)
#if VERSION >= VERSION_JP
    ELSE
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        SET_VALUE(LW(3), LW(1))
        ADD(LW(1), 150)
        ADD(LW(3), 80)
        SET_VALUE(LW(4), LW(2))
        ADD(LW(2), 400)
    END_IF
#endif
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
    CALL(evt_btl_camera_set_moveto, 0, LW(0), LW(1), LW(2), LW(0), LW(3), LW(4), 30, 11)
    CALL(evt_batstage_light_set_player_on, 0)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_CheckDataOfDataTable, UNIT_ID_SELF, LW(0), UNIT_DATA_APPEAL_EVENT)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_RunDataEventChild, UNIT_ID_SELF, UNIT_DATA_APPEAL_EVENT)
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
        CALL(evt_batstage_light_set_player_off)
        WAIT_FRAMES(60)
        RETURN
    END_IF
    CALL(btlevtcmd_GetBodyId, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(0), POSE_TYPE_APPEAL)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_MARIO_APPEAL1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    WAIT_FRAMES(60)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(btlevtcmd_AudienceAppeal, UNIT_ID_SELF)
    CALL(evt_batstage_light_set_player_off)
    WAIT_FRAMES(60)
    CALL(btlevtcmd_StartWaitEvent, LW(10))
    RETURN
EVT_END()

/**
 * Shared Defend action (LW(12) = the defend weapon):
 * pays the cost, spins the unit twice, marks it as defending, shows the "defend" message and returns to waiting.
 */
static EVT_BEGIN(_Defence_core)
    CALL(btlevtcmd_GetUnitId, UNIT_ID_SELF, LW(3))
    CALL(btlevtcmd_GetBodyId, UNIT_ID_SELF, LW(4))
    IF_INT_EQUAL(LW(3), -1)
        // BUG: this script has no LABEL(99): evtSearchLabel resumes at this GOTO's argument word, which runs
        // as SET_TYPE with the next command word as its value, and the script goes on after the GOTO.
        GOTO(99)
    END_IF
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(12))
    CALL(btlevtcmd_RunDataEventChild, UNIT_ID_SELF, UNIT_DATA_ATTACK_READY_EVENT)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_MARIO_DEFENCE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    LOOP(20)
        CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 36, 0)
        WAIT_FRAMES(1)
    END_LOOP
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_OnStatusFlag, UNIT_ID_SELF, UNIT_STATUS_FLAG_DEFEND)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(4), POSE_TYPE_DEFEND)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_MARIO_DEFENCE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_ftof, 60, LW(0))
    CALL(btlevtcmd_AnnounceMessage, 0, 0, 0, STRING("btl_msg_defend_command_exec"), LW(0))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

#if VERSION <= VERSION_DEMO_JP
/** Mario's Defend command. */
EVT_BEGIN(btldefaultevt_Defend)
    SET_VALUE(LW(12), PTR(&badgeWeapon_DefenceCommand))
    CALL_CHILD(_Defence_core)
    RETURN
EVT_END()
/** The partner's Defend command. */
EVT_BEGIN(btldefaultevt_DefendParty)
    SET_VALUE(LW(12), PTR(&badgeWeapon_DefenceCommandP))
    CALL_CHILD(_Defence_core)
    RETURN
EVT_END()
#endif

/**
 * The Run Away command: Mario and the partner turn and run in place
 * while the escape action command plays (the gauge uses the enemy group's run rate).
 * On a miss both stumble and run home; on a success they run off screen,
 * the audience reacts and the battle ends (after _event_MajinaiEnd when Merlee's spells ran out).
 */
EVT_BEGIN(btldefaultevt_Escape)
    // The camera frames the party and the Run Away Action Command is set up,
    // its gauge set by the enemy group's escape rate.
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_ESCAPE)
    CALL(_btl_escape_reset_move_color_lv_player_unit_all)
    CALL(btlevtcmd_AcSetDifficulty, UNIT_ID_SELF, 3)
    CALL(evt_sub_random, 99, LW(0))
    CALL(evt_sub_random, 6, LW(1))
    ADD(LW(1), 30)
    CALL(_get_escape_rate, LW(2))
    CALL(btlevtcmd_AcSetParamAll, 1, 180, LW(0), LW(1), 0, 4, 6, LW(2))
    CALL(btlevtcmd_AcSetFlag, AC_REPEATEDLY_FLAG_ESCAPE_UNUSED | AC_REPEATEDLY_FLAG_PLAIN_GAUGE)
    CALL(btlevtcmd_SetupAC, UNIT_ID_SELF, 5, 1, 0)
    CALL(_set_escape_ac_hlp_msg)

    // The partner, then Mario, turns away from the enemies and hops twice.
    BEGIN_INLINE_EVENT
        CALL(btlevtcmd_GetPartnerId, UNIT_ID_SELF, LW(10))
        IF_INT_NOT_EQUAL(LW(10), -1)
            CALL(btlevtcmd_CheckActStatus, LW(10), LW(0))
            IF_INT_NOT_EQUAL(LW(0), 0)
                CALL(btlevtcmd_GetBodyId, LW(10), LW(11))
                CALL(btlevtcmd_CalculateFaceDirection, LW(10), LW(11), -1, -1, (FACE_DIR_ATTACK_DIR | FACE_DIR_REVERSE), LW(0))
                CALL(btlevtcmd_ChangeFaceDirection, LW(10), LW(0))
                CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), POSE_TYPE_RUN)
                CALL(btlevtcmd_JumpSetting, LW(10), 20, FLOAT(0.0f), FLOAT(0.69921875f))
                CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
                CALL(btlevtcmd_JumpPosition, LW(10), LW(0), LW(1), LW(2), 16, MOVE_SOUND_FROM_TABLE)
                CALL(btlevtcmd_JumpPosition, LW(10), LW(0), LW(1), LW(2), 16, MOVE_SOUND_FROM_TABLE)
            END_IF
        END_IF
    END_INLINE_EVENT
    CALL(btlevtcmd_GetBodyId, UNIT_ID_SELF, LW(11))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, LW(11), -1, -1, (FACE_DIR_ATTACK_DIR | FACE_DIR_REVERSE), LW(0))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(11), POSE_TYPE_RUN)
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 20, FLOAT(0.0f), FLOAT(0.69921875f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 16, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 16, MOVE_SOUND_FROM_TABLE)

    // The audience reacts and both run in place, slowly edging back, while the player fills the gauge.
    CALL(btlevtcmd_audience_code_escape)
    CALL(evt_snd_sfxon, STRING("SFX_MARIO_BATTLE_STAMP1"), LW(15))
    BEGIN_INLINE_EVENT
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 15)
        CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 180, MOVE_SOUND_FROM_TABLE, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(btlevtcmd_GetPartnerId, UNIT_ID_SELF, LW(10))
        IF_INT_NOT_EQUAL(LW(10), -1)
            CALL(btlevtcmd_CheckActStatus, LW(10), LW(0))
            IF_INT_NOT_EQUAL(LW(0), 0)
                CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
                CALL(btlevtcmd_FaceDirectionSub, LW(10), LW(0), 15)
                CALL(btlevtcmd_MovePosition, LW(10), LW(0), LW(1), LW(2), 180, MOVE_SOUND_FROM_TABLE, 0)
            END_IF
        END_IF
    END_INLINE_EVENT

    // The Action Command runs; Mario hops when it ends.
    CALL(btlevtcmd_StartAC, 1)
    CALL(btlevtcmd_ResultAC)
    CALL(btlevtcmd_StopAC)
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 20, FLOAT(0.0f), FLOAT(0.69921875f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 16, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_GetResultAC, LW(6))

    // The escape failed: the partner and Mario dash a few steps, trip and fall, and the audience reacts.
    IF_BITS_CLEAR(LW(6), AC_RESULT_SUCCESS)
        CALL(evt_snd_sfxoff, LW(15))
        CALL(evt_snd_sfxon, STRING("SFX_MARIO_RUN_AWAY_MISS1"), 0)
        BEGIN_SIBLING_EVENT_GET_ID(LW(15))
            CALL(btlevtcmd_GetPartnerId, UNIT_ID_SELF, LW(10))
            IF_INT_NOT_EQUAL(LW(10), -1)
                CALL(btlevtcmd_CheckActStatus, LW(10), LW(0))
                IF_INT_NOT_EQUAL(LW(0), 0)
                    CALL(btlevtcmd_OffUnitFlag, LW(10), BATTLE_UNIT_FLAG_FLOAT_ACTIVE)
                    CALL(btlevtcmd_GetBodyId, LW(10), LW(11))
                    CALL(btlevtcmd_SetMoveSpeed, LW(10), FLOAT(10.0f))
                    CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
                    CALL(btlevtcmd_FaceDirectionAdd, LW(10), LW(0), 30)
                    CALL(btlevtcmd_MovePosition, LW(10), LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
                    CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), POSE_TYPE_ESCAPE_STUMBLE)
                    CALL(btlevtcmd_FaceDirectionAdd, LW(10), LW(0), 5)
                    CALL(btlevtcmd_MovePosition, LW(10), LW(0), LW(1), LW(2), 5, MOVE_SOUND_FROM_TABLE, 0)
                    CALL(btlevtcmd_FaceDirectionAdd, LW(10), LW(0), 10)
                    CALL(btlevtcmd_MovePosition, LW(10), LW(0), LW(1), LW(2), 60, MOVE_SOUND_FROM_TABLE, 0)
                    WAIT_FRAMES(25)
                    CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), POSE_TYPE_ESCAPE_RECOVER)
                    WAIT_FRAMES(40)
                    CALL(btlevtcmd_OnUnitFlag, LW(10), BATTLE_UNIT_FLAG_FLOAT_ACTIVE)
                END_IF
            END_IF
        END_SIBLING_EVENT
        CALL(btlevtcmd_OffUnitFlag, LW(10), BATTLE_UNIT_FLAG_FLOAT_ACTIVE)
        CALL(btlevtcmd_GetBodyId, UNIT_ID_SELF, LW(11))
        CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(10.0f))
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 30)
        CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(11), POSE_TYPE_ESCAPE_STUMBLE)
        CALL(btlevtcmd_audience_code_escape_fail)
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 5)
        CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 5, MOVE_SOUND_FROM_TABLE, 0)
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 10)
        CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 60, MOVE_SOUND_FROM_TABLE, 0)
        WAIT_FRAMES(25)
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(11), POSE_TYPE_ESCAPE_RECOVER)
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
        WAIT_FRAMES(40)
        CALL(btlevtcmd_OnUnitFlag, LW(10), BATTLE_UNIT_FLAG_FLOAT_ACTIVE)

        // Both get up and run back to their home positions; the turn goes on.
        BEGIN_SIBLING_EVENT_GET_ID(LW(15))
            CALL(btlevtcmd_GetPartnerId, UNIT_ID_SELF, LW(10))
            IF_INT_NOT_EQUAL(LW(10), -1)
                CALL(btlevtcmd_CheckActStatus, LW(10), LW(0))
                IF_INT_NOT_EQUAL(LW(0), 0)
                    CALL(btlevtcmd_GetBodyId, LW(10), LW(11))
                    CALL(btlevtcmd_CalculateFaceDirection, LW(10), LW(11), 0, 0, FACE_DIR_ATTACK_DIR, LW(0))
                    CALL(btlevtcmd_ChangeFaceDirection, LW(10), LW(0))
                    CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), POSE_TYPE_RUN)
                    CALL(btlevtcmd_SetMoveSpeed, LW(10), FLOAT(3.0f))
                    CALL(btlevtcmd_GetHomePos, LW(10), LW(0), LW(1), LW(2))
                    CALL(btlevtcmd_MovePosition, LW(10), LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
                    CALL(btlevtcmd_StartWaitEvent, LW(10))
                END_IF
            END_IF
        END_SIBLING_EVENT
        CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, LW(11), 0, 0, FACE_DIR_ATTACK_DIR, LW(0))
        CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(11), POSE_TYPE_RUN)
        CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(3.0f))
        CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        CALL(btlevtcmd_WaitEventEnd, LW(15))
        RETURN
    END_IF

    // The escape worked: the partner and Mario dash off the side of the stage while the audience reacts.
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_GetPartnerId, UNIT_ID_SELF, LW(10))
        IF_INT_NOT_EQUAL(LW(10), -1)
            CALL(btlevtcmd_CheckActStatus, LW(10), LW(0))
            IF_INT_NOT_EQUAL(LW(0), 0)
                CALL(btlevtcmd_SetMoveSpeed, LW(10), FLOAT(10.0f))
                CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
                CALL(btlevtcmd_FaceDirectionAdd, LW(10), LW(0), 200)
                CALL(btlevtcmd_MovePosition, LW(10), LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
            END_IF
        END_IF
    END_SIBLING_EVENT
    CALL(evt_snd_sfxoff, LW(15))
    CALL(btlevtcmd_audience_code_escape_success)
    CALL(evt_snd_sfxon, STRING("SFX_MARIO_RUN_AWAY1"), 0)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(10.0f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 200)
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_NONE, 0)

    // When Merlee's spells ran out during this battle, her farewell plays first; then the battle ends.
    SET_VALUE(LW(15), 0)
    CALL(btlevtcmd_check_battleflag, LW(0), BATTLE_FLAG_MAJINAI_END)
    IF_INT_NOT_EQUAL(LW(0), 0)
        BEGIN_SIBLING_EVENT_GET_ID(LW(15))
            CALL_CHILD(_event_MajinaiEnd)
        END_SIBLING_EVENT
    END_IF
    IF_INT_NOT_EQUAL(LW(15), 0)
        CALL(btlevtcmd_WaitEventEnd, LW(15))
    END_IF
    CALL(btlevtcmd_InterruptStop, 0)
    RETURN
EVT_END()

/** Confused unit's action: the confused pose for 30 frames, then the unit's confusion action script. */
EVT_BEGIN(btldefaultevt_Confuse)
    CALL(btlevtcmd_GetBodyId, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(0), POSE_TYPE_CONFUSE)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_GetConfuseActEvent, UNIT_ID_SELF, LW(0))
    CALL_CHILD(LW(0))
    RETURN
EVT_END()

/**
 * Run instead of a unit's action in zero gravity when it cannot act there:
 * zooms on it and shows the "can't move in zero gravity" message.
 */
EVT_BEGIN(btldefaultevt_CantMoveZeroGravity)
    CALL(btlevtcmd_GetBodyId, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(0), POSE_TYPE_ZERO_GRAVITY)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, -1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(btlevtcmd_AnnounceMessage, 0, 0, 0, STRING("btl_msg_cant_move_zero_gravity"), 120)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    WAIT_FRAMES(20)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * HP Drain and FP Drain badge effect after an attack:
 * heals the attacker by its equipped HP Drain and then FP Drain power, capped at the damage it dealt this turn,
 * with the recovery effect over its head.
 */
EVT_BEGIN(btldefaultevt_SuitoruBadgeEffect)
    CALL(_get_heart_suitoru_point, UNIT_ID_SELF, LW(5))
    IF_INT_GREATER_OR_EQUAL(LW(5), 1)
        BEGIN_SIBLING_EVENT
            CALL(btlevtcmd_GetBodyId, UNIT_ID_SELF, LW(4))
#if VERSION >= VERSION_JP
            CALL(btlevtcmd_GetStatusMg, UNIT_ID_SELF, LW(6))
            CALL(btlevtcmd_GetHeight, UNIT_ID_SELF, LW(7))
            MULTIPLY_FLOAT(LW(7), LW(6))
#endif
            CALL(btlevtcmd_GetPartsPos, UNIT_ID_SELF, LW(4), LW(0), LW(1), LW(2))
#if VERSION <= VERSION_DEMO_JP
            ADD(LW(1), 50)
            CALL(evt_eff, STRING(""), STRING("recovery"), 0, LW(0), LW(1), LW(2), LW(5), 0, 0, 0, 0, 0, 0, 0)
#else
            ADD(LW(1), LW(7))
            CALL(evt_eff, 0, STRING("recovery"), 0, LW(0), LW(1), LW(2), LW(5), 0, 0, 0, 0, 0, 0, 0)
#endif
            WAIT_FRAMES(120)
            CALL(btlevtcmd_RecoverHp, UNIT_ID_SELF, LW(4), LW(5))
        END_SIBLING_EVENT
        WAIT_FRAMES(125)
    END_IF
    CALL(_get_flower_suitoru_point, UNIT_ID_SELF, LW(5))
    IF_INT_GREATER_OR_EQUAL(LW(5), 1)
        BEGIN_SIBLING_EVENT
#if VERSION >= VERSION_JP
            CALL(btlevtcmd_GetStatusMg, UNIT_ID_SELF, LW(6))
            CALL(btlevtcmd_GetHeight, UNIT_ID_SELF, LW(7))
            MULTIPLY_FLOAT(LW(7), LW(6))
#endif
            CALL(btlevtcmd_GetBodyId, UNIT_ID_SELF, LW(4))
            CALL(btlevtcmd_GetPartsPos, UNIT_ID_SELF, LW(4), LW(0), LW(1), LW(2))
#if VERSION <= VERSION_DEMO_JP
            ADD(LW(1), 50)
            CALL(evt_eff, STRING(""), STRING("recovery"), 1, LW(0), LW(1), LW(2), LW(5), 0, 0, 0, 0, 0, 0, 0)
#else
            ADD(LW(1), LW(7))
            CALL(evt_eff, 0, STRING("recovery"), 1, LW(0), LW(1), LW(2), LW(5), 0, 0, 0, 0, 0, 0, 0)
#endif
            WAIT_FRAMES(120)
            CALL(btlevtcmd_RecoverFp, UNIT_ID_SELF, LW(4), LW(5))
        END_SIBLING_EVENT
        WAIT_FRAMES(125)
    END_IF
    RETURN
EVT_END()

/** Data-table event for avoiding a counter: the unit spins round five times (1800 degrees) over 40 frames. */
EVT_BEGIN(btldefaultevt_avoid_counter_event)
    CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), POSE_TYPE_AVOID_COUNTER)
    BEGIN_SIBLING_EVENT_GET_ID(LW(0))
        CALL(evt_sub_intpl_init, EASING_CUBIC_OUT, 0, 1800, 40)
        LOOP(40)
            CALL(evt_sub_intpl_get_value_para, LW(1), LW(2))
            CALL(btlevtcmd_SetRotate, LW(10), EVT_NULLPTR, LW(1), EVT_NULLPTR)
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_WaitEventEnd, LW(0))
    RETURN
EVT_END()

#if VERSION >= VERSION_JP
EVT_BEGIN(btldefaultevt_ProtectPartner)
    CALL(btlevtcmd_GetPartnerId, UNIT_ID_SELF, LW(3))
    CALL(btlevtcmd_SetGuard, UNIT_ID_SELF, LW(3))
    RETURN
EVT_END()
#endif

EVT_BEGIN(btldefaultevt_Dummy)
    RETURN
EVT_END()

#if VERSION >= VERSION_JP
EVT_BEGIN(btldefaultevt_Wait)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

EVT_BEGIN(btldefaultevt_LectureEnd)
    CALL(btlevtcmd_RunDataEventChild, UNIT_ID_LECTURE, UNIT_DATA_LECTURE_END_EVENT)
    RETURN
EVT_END()

EVT_BEGIN(btldefaultevt_Defend)
    SET_VALUE(LW(12), PTR(&badgeWeapon_DefenceCommand))
    CALL_CHILD(_Defence_core)
    RETURN
EVT_END()

EVT_BEGIN(btldefaultevt_DefendParty)
    SET_VALUE(LW(12), PTR(&badgeWeapon_DefenceCommandP))
    CALL_CHILD(_Defence_core)
    RETURN
EVT_END()
#endif

static USER_FUNC(_backfire) {
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
        BattleDamageDirect(UNIT_ID_SYSTEM, unit, NULL, 1, 0, DAMAGE_RESULT_FINAL_HIT | DAMAGE_CODE_FIRE, 0, 1);
        self = BattleTransID(event, event->unitId);
        if (self != id) {
            return EVT_RETURN_DONE;
        }
        return EVT_RETURN_FINISH;
    }
    fire->scale = 1.6f * sinfd(event->userdata[1] * 3) + 0.4f;
    return EVT_RETURN_BLOCK;
}

/** Script: starts loading the animations of the partner chosen in the command menu. */
static USER_FUNC(_anime_load) {
    BattlePartyAnimeLoad(_battleWorkPointer->commandMenu.partyKind);
    return EVT_RETURN_DONE;
}

/** Script: blocks until the chosen partner's animations are loaded. */
static USER_FUNC(_anime_load_wait) {
    return BattlePartyAnimeLoad(_battleWorkPointer->commandMenu.partyKind) ? EVT_RETURN_DONE : EVT_RETURN_BLOCK;
}

/**
 * Script: sets args[0] to 1 when the active unit is Mario
 * and the partner switch costs his turn (no Quick Change, or he is confused).
 */
static USER_FUNC(_check_pose_stay) {
    BattleWork* wp = _battleWorkPointer;
    s32 out = event->args[0];
    BattleUnit* unit;
    s32 stay;
    s32 id;

    id = BattleTransID(event, UNIT_ID_SELF);
    unit = BattleGetUnitPtr(wp, id);
    stay = FALSE;
    if (unit->currentKind == UNIT_MARIO) {
        stay = TRUE;
        if (wp->badgeEquippedFlags & BATTLE_BADGE_QUICK_CHANGE) {
            if (!(unit->tokenFlags & kConfusedAction)) {
                stay = FALSE;
            }
        }
    }
    evtSetValue(event, out, stay);
    return EVT_RETURN_DONE;
}

/** Script: tells the audience that Mario is running away. */
USER_FUNC(btlevtcmd_audience_code_escape) {
    BattleAudience_Case_Escape();
    return EVT_RETURN_DONE;
}

/** Script: audience reaction to a successful escape. */
USER_FUNC(btlevtcmd_audience_code_escape_success) {
    BattleAudience_Case_Escape_Good();
    return EVT_RETURN_DONE;
}

/** Script: audience reaction to a failed escape. */
USER_FUNC(btlevtcmd_audience_code_escape_fail) {
    BattleAudience_Case_Escape_Bad();
    return EVT_RETURN_DONE;
}

/** Script: resets the moveColorLv shading of Mario and the partner to full brightness before the escape. */
static USER_FUNC(_btl_escape_reset_move_color_lv_player_unit_all) {
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;

    unit = BattleGetMarioPtr(wp);
    unit->moveColorLv = MOVE_COLOR_LV_FULL;
    unit = BattleGetPartyPtr(wp);
    if (unit) {
        unit->moveColorLv = MOVE_COLOR_LV_FULL;
    }
    return EVT_RETURN_DONE;
}

/** Script: shows the escape action command's help text. */
static USER_FUNC(_set_escape_ac_hlp_msg) {
    BattleAcHelpSetHelp("msg_ac_escape");
    BattleAcHelpSetDispType(AC_HELP_DISP_ON);
    return EVT_RETURN_DONE;
}

/** Script: returns in args[0] the enemy group's run rate, which sets the escape gauge. */
static USER_FUNC(_get_escape_rate) {
    BattleWork* wp = _battleWorkPointer;
    evtSetValue(event, event->args[0],
                wp->fbatInfo->setup->groupSetup->enemyData->unitKindParams->runRate);
    return EVT_RETURN_DONE;
}

/**
 * Script: returns in args[1] the HP the unit args[0] regains from HP Drain:
 * its equipped power, capped at its damage dealt.
 */
static USER_FUNC(_get_heart_suitoru_point) {
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

/**
 * Script: returns in args[1] the FP the unit args[0] regains from FP Drain:
 * its equipped power, capped at its damage dealt.
 */
static USER_FUNC(_get_flower_suitoru_point) {
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
