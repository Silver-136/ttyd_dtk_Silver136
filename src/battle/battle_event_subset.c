// Battle event subsets: reusable damage, death, confusion and blow-away scripts shared by units.

#include "version.h"
#include "battle/battle.h"
#include "unit/unit_koura.h"
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
#include "battle/battle_camera.h"
#include "party/party_motion.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_status_effect.h"
#include "battle/battle_damage.h"
#include "system.h"
#include "battle/unit_work.h"

USER_FUNC(_binta_effect);
USER_FUNC(_check_through_pos);
USER_FUNC(_set_hp_gauge_fade_flag);
USER_FUNC(_check_at_dm_event_wait);
USER_FUNC(_disable_restore_command_cursor);

#if VERSION >= VERSION_US
EvtWord subsetevt_dead[612];
#else
#if VERSION <= VERSION_DEMO_JP
EvtWord subsetevt_dead[595];
#else
EvtWord subsetevt_dead[608];
#endif
#endif

EvtWord subsetevt_dead_core[193];
EvtWord subsetevt_dead_core_nospin_norotate[125];
EvtWord btldefaultevt_avoid_guard[92];

#if VERSION <= VERSION_DEMO_JP
extern EvtWord subsetevt_damage[];
extern EvtWord subsetevt_counter_damage[];
extern EvtWord btldefaultevt_ChangePosition[];
extern EvtWord subsetevt_happy_heart[];
extern EvtWord subsetevt_happy_flower[];
#else
EvtWord subsetevt_damage[979];
EvtWord subsetevt_counter_damage[174];
EvtWord btldefaultevt_ChangePosition[186];
EvtWord subsetevt_happy_heart[53];
EvtWord subsetevt_happy_flower[53];
#endif

EvtWord subsetevt_terror[51];
EvtWord subsetevt_blow_damage[69];
EvtWord subsetevt_blow_dead[76];
EvtWord subsetevt_shot_damage[31];
EvtWord subsetevt_shot_damage_kaiten[288];

#if VERSION <= VERSION_DEMO_JP
extern EvtWord subsetevt_shot_damage_ultra[];
#else
EvtWord subsetevt_shot_damage_ultra[429];
#endif

EvtWord subsetevt_swallow_failed_damage[55];
EvtWord subsetevt_swallow_spew_damage[55];

#if VERSION <= VERSION_DEMO_JP
extern EvtWord subsetevt_swallow_shot_damage[];
#else
EvtWord subsetevt_swallow_shot_damage[342];
#endif

EvtWord subset_ultra_hammer_acrobat_damage_event[63];
EvtWord subsetevt_confuse_flustered[43];
EvtWord subsetevt_binta_damage_left[48];
EvtWord subsetevt_binta_damage_right[48];
EvtWord subsetevt_binta_damage_left_last[69];
EvtWord subsetevt_binta_damage_right_last[69];
EvtWord subsetevt_status_recovery[48];

#if VERSION >= VERSION_JP
EvtWord subsetevt_check_target_movable[57];
#endif

EvtWord subsetevt_nodamage_motion[6];
EvtWord subsetevt_regeneration[4];
EvtWord subset_attack_ready_jump_event[2];

/**
 * KO reaction for a unit (LW(10) = unit, LW(11) = part) whose HP reached 0.
 * Mario and the partners first walk home.
 * Mario then runs his data-table event 51 when a Life Shroom revives him (damage code bit 0x800), else event 49.
 * A partner with a Life Shroom keels over, uses it up and gets back up with 10 HP;
 * otherwise it is KO'd and, when it stood in front, swaps places with Mario.
 * Enemies revived by a Life Shroom spin down, show it and recover 10 HP;
 * other enemies run their data-table KO event or subsetevt_dead_core.
 * US on, a revived enemy also loses its holding-stolen-item flag.
 */
EVT_BEGIN(subsetevt_dead)
    // The command cursor will no longer return to this unit.
    // Mario and the partners first float or walk back home; enemies skip ahead.
    CALL(_disable_restore_command_cursor)
    CALL(btlevtcmd_GetUnitKind, LW(10), LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(UNIT_MARIO)
        GOTO(0)
        CASE_BETWEEN(UNIT_GOOMBELLA, UNIT_MS_MOWZ)
        GOTO(0)
        CASE_DEFAULT
        GOTO(1)
    END_SWITCH
    LABEL(0)
    CALL(btlevtcmd_CheckAttribute, LW(10), kFloating, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), POSE_TYPE_RUN)
        CALL(btlevtcmd_GetHomePos, LW(10), LW(2), LW(3), LW(4))
        CALL(btlevtcmd_SetMoveSpeed, LW(10), FLOAT(4.0f))
        CALL(btlevtcmd_DivePosition, LW(10), LW(2), LW(3), LW(4), 0, 0, EASING_LINEAR, 0, MOVE_SOUND_FROM_TABLE)
    ELSE
        CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), POSE_TYPE_WALK)
        CALL(btlevtcmd_GetHomePos, LW(10), LW(2), LW(3), LW(4))
        CALL(btlevtcmd_SetMoveSpeed, LW(10), FLOAT(4.0f))
        CALL(btlevtcmd_MovePosition, LW(10), LW(2), LW(3), LW(4), 0, MOVE_SOUND_FROM_TABLE, 0)
    END_IF
    CALL(btlevtcmd_ResetFaceDirection, LW(10))
    LABEL(1)

    // The unit is marked KO'd.
    CALL(btlevtcmd_OnOffStatus, LW(10), STATUS_INSTANT_KILL, 1, 1, 1)
    CALL(btlevtcmd_GetUnitKind, LW(10), LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(UNIT_MARIO)
        // Mario: his Life Shroom revival (data-table event 51) when one saves him, else his KO event 49.
        CALL(btlevtcmd_ClearAllStatus, LW(10))
        CALL(btlevtcmd_CheckDamageCode, LW(10), DAMAGE_RESULT_REVIVAL, LW(1))
        IF_INT_NOT_EQUAL(LW(1), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_EMERGENCY_REVIVAL_EVENT)
        ELSE
            CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_DEAD_EVENT)
        END_IF
        CASE_BETWEEN(UNIT_GOOMBELLA, UNIT_MS_MOWZ)
        // A partner saved by a Life Shroom runs its own revival event if it has one, else the one below.
        CALL(btlevtcmd_CheckDamageCode, LW(10), DAMAGE_RESULT_REVIVAL, LW(1))
        IF_INT_NOT_EQUAL(LW(1), 0)
            CALL(btlevtcmd_ClearAllStatus, LW(10))
            CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(0), UNIT_DATA_EMERGENCY_REVIVAL_EVENT)
            IF_INT_NOT_EQUAL(LW(0), 0)
                CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_EMERGENCY_REVIVAL_EVENT)
            ELSE
                // The partner keels over, the Life Shroom icon shows above it and the Shroom is used up.
                CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), POSE_TYPE_KO)
                BEGIN_SIBLING_EVENT
                    CALL(btlevtcmd_AnimeWaitPlayComplete, LW(10), LW(11))
                    CALL(btlevtcmd_snd_se, LW(10), STRING("SFX_PARTY_BATTLE_DIE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
                END_SIBLING_EVENT
                WAIT_FRAMES(60)
                CALL(btlevtcmd_ClearAllStatus, LW(10))
                CALL(btlevtcmd_ConsumeItemReserve, LW(10), ITEM_LIFE_SHROOM)
                BEGIN_SIBLING_EVENT
                    CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
                    ADD(LW(1), 40)
                    CALL(btlevtcmd_DispItemIcon, LW(0), LW(1), LW(2), ITEM_LIFE_SHROOM, 90)
                END_SIBLING_EVENT
                WAIT_FRAMES(90)
#if VERSION >= VERSION_JP

                // In JP and later the partner's event 67 also runs (Koops uses it to stand upright).
                CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(0), UNIT_DATA_REVIVE_EVENT)
                IF_INT_NOT_EQUAL(LW(0), 0)
                    CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_REVIVE_EVENT)
                END_IF
#endif

                // The partner gets back up with 10 HP, the recovery sparkle shows and it hops back to idle.
                CALL(btlevtcmd_OnOffStatus, LW(10), STATUS_INSTANT_KILL, 0, 0, 0)
                CALL(btlevtcmd_SetHp, LW(10), 0)
                CALL(btlevtcmd_RecoverHp, LW(10), LW(11), 10)
                CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
                ADD(LW(1), 30)
                CALL(evt_eff, 0, STRING("recovery"), 0, LW(0), LW(1), LW(2), 10, 0, 0, 0, 0, 0, 0, 0)
                WAIT_MS(2000)
                CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), POSE_TYPE_RUN)
                CALL(btlevtcmd_JumpSetting, LW(10), 0, FLOAT(0.0f), FLOAT(0.69921875f))
                CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
                CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 20, MOVE_SOUND_FROM_TABLE)
                CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
            END_IF
            ELSE
            // No Life Shroom: the partner runs its KO event and falls over.
            CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(0), UNIT_DATA_DEAD_EVENT)
            IF_INT_NOT_EQUAL(LW(0), 0)
                CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_DEAD_EVENT)
            ELSE
            END_IF
            CALL(btlevtcmd_KillUnit, LW(10), 1)
            CALL(btlevtcmd_GetBodyId, UNIT_ID_SELF, LW(11))
            CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), POSE_TYPE_KO)
            BEGIN_SIBLING_EVENT
                CALL(btlevtcmd_AnimeWaitPlayComplete, LW(10), LW(11))
                CALL(btlevtcmd_snd_se, LW(10), STRING("SFX_PARTY_BATTLE_DIE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
            END_SIBLING_EVENT

            // A KO'd partner standing in front of Mario swaps places with him, as long as Mario still has HP.
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
                CALL(_check_at_dm_event_wait, UNIT_ID_MARIO)
                CALL(btlevtcmd_GetHp, UNIT_ID_MARIO, LW(0))
                IF_INT_GREATER(LW(0), 0)
                    CALL_CHILD(btldefaultevt_ChangePosition)
                END_IF
            END_IF
        END_IF
        CASE_DEFAULT
        // Enemies: a Life Shroom revival runs the unit's own event 51 when it has one.
        CALL(btlevtcmd_CheckDamageCode, LW(10), DAMAGE_RESULT_REVIVAL, LW(1))
        IF_INT_NOT_EQUAL(LW(1), 0)
            CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(0), UNIT_DATA_EMERGENCY_REVIVAL_EVENT)
            IF_INT_NOT_EQUAL(LW(0), 0)
                CALL(btlevtcmd_ClearAllStatus, LW(10))
                CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_EMERGENCY_REVIVAL_EVENT)
            ELSE
                // The standard enemy revival: the enemy spins and tips over, and the Life Shroom icon shows.
                CALL(btlevtcmd_OnUnitFlag, LW(10), BATTLE_UNIT_FLAG_HIDE_HELD_ITEM)
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
                CALL(btlevtcmd_ConsumeItemReserve, LW(10), ITEM_LIFE_SHROOM)
                BEGIN_SIBLING_EVENT
                    CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
                    ADD(LW(1), 40)
                    CALL(btlevtcmd_DispItemIcon, LW(0), LW(1), LW(2), ITEM_LIFE_SHROOM, 120)
                END_SIBLING_EVENT
                WAIT_FRAMES(90)

                // The enemy rights itself, recovers 10 HP with the recovery sparkle, hops up and idles again.
                SET_VALUE(LW(0), 30)
                LABEL(7)
                WAIT_FRAMES(1)
                CALL(btlevtcmd_AddRotate, LW(10), -3, 0, 0)
                SUBTRACT(LW(0), 1)
                IF_INT_GREATER(LW(0), 0)
                    GOTO(7)
                END_IF
                CALL(btlevtcmd_OnOffStatus, LW(10), STATUS_INSTANT_KILL, 0, 0, 0)
                CALL(btlevtcmd_SetHp, LW(10), 0)
                CALL(btlevtcmd_RecoverHp, LW(10), LW(11), 10)
                CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
                ADD(LW(1), 30)
                CALL(evt_eff, 0, STRING("recovery"), 0, LW(0), LW(1), LW(2), 10, 0, 0, 0, 0, 0, 0, 0)
                WAIT_MS(2000)
                CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), POSE_TYPE_RUN)
                CALL(btlevtcmd_JumpSetting, LW(10), 0, FLOAT(0.0f), FLOAT(0.69921875f))
                CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
                CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 20, MOVE_SOUND_FROM_TABLE)
                CALL(btlevtcmd_OffUnitFlag, LW(10), BATTLE_UNIT_FLAG_HIDE_HELD_ITEM)
#if VERSION >= VERSION_US
                CALL(btlevtcmd_OffUnitFlag, LW(10), BATTLE_UNIT_FLAG_STOLEN_ITEM)
#endif
                CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
            END_IF
        ELSE
            // No Life Shroom: the enemy's own KO event 49, else the standard spin-and-smoke defeat.
            CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(0), UNIT_DATA_DEAD_EVENT)
            IF_INT_NOT_EQUAL(LW(0), 0)
                CALL(btlevtcmd_ClearAllStatus, LW(10))
                CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_DEAD_EVENT)
            ELSE
                CALL_CHILD(subsetevt_dead_core)
            END_IF
        END_IF
    END_SWITCH
    RETURN
EVT_END()

/**
 * Standard enemy defeat: spins the unit, puffs it away in smoke,
 * banks its coins and Star Points (with the star point effect at the unit's offset), tips it over,
 * returns any item it stole and removes the unit.
 */
EVT_BEGIN(subsetevt_dead_core)
    CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(0), UNIT_DATA_PRE_DEAD_EVENT)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_PRE_DEAD_EVENT)
    END_IF
    SET_VALUE(LW(0), 48)
    CALL(btlevtcmd_OnUnitFlag, LW(10), BATTLE_UNIT_FLAG_HIDE_HELD_ITEM)
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
    CALL(btlevtcmd_CheckDamageCode, LW(10), DAMAGE_RESULT_DEATH, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_KillUnit, LW(10), 0)
    ELSE
        CALL(btlevtcmd_KillUnit, LW(10), 2)
    END_IF
    RETURN
EVT_END()

/**
 * Enemy defeat without the spin and tip-over: smoke, coins and Star Points, stolen item returned,
 * then the unit is removed.
 */
EVT_BEGIN(subsetevt_dead_core_nospin_norotate)
    CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(0), UNIT_DATA_PRE_DEAD_EVENT)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_PRE_DEAD_EVENT)
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
    CALL(btlevtcmd_CheckDamageCode, LW(10), DAMAGE_RESULT_DEATH, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_KillUnit, LW(10), 1)
    ELSE
        CALL(btlevtcmd_KillUnit, LW(10), 2)
    END_IF
    RETURN
EVT_END()

/**
 * Reaction of a unit whose hit a bodyguard took (damage code 41):
 * it backs away, waits in the bodyguard pose until the attack ends and runs home.
 */
EVT_BEGIN(btldefaultevt_avoid_guard)
    CALL(btlevtcmd_AnimeCmpPoseType, LW(10), LW(11), POSE_TYPE_RUN, 40, LW(2))
    IF_INT_EQUAL(LW(2), 0)
        CALL(btlevtcmd_AnimeSetMotionSpeed, LW(10), LW(11), FLOAT(2.0f))
    END_IF
    CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), POSE_TYPE_RUN)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 20)
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 20, MOVE_SOUND_FROM_TABLE, 0)
    CALL(btlevtcmd_AnimeSetMotionSpeed, LW(10), LW(11), FLOAT(1.0f))
    CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), POSE_TYPE_BODYGUARD)
    CALL(btlevtcmd_WaitAttackEnd)
    IF_INT_EQUAL(LW(14), LW(15))
        CALL(btlevtcmd_AnimeSetMotionSpeed, LW(10), LW(11), FLOAT(2.0f))
    END_IF
    CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), POSE_TYPE_RUN)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(2), LW(3), LW(4))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(2), LW(3), LW(4), 20, MOVE_SOUND_FROM_TABLE, 0)
    CALL(btlevtcmd_AnimeSetMotionSpeed, LW(10), LW(11), FLOAT(1.0f))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * The hurt reaction for a hit, called from btldefaultevt_Damage with the hurt pose in LW(14) and the pose
 * after it in LW(15): flipped or grounded units, hammer knockback (Spin Hammer, Ultra Hammer), slaps,
 * Gale Force and Yoshi's swallow patterns run the unit's data-table event or the matching subset;
 * a squashing hit flattens the unit for 1.5 to 3 seconds; other hits hold the hurt pose.
 * A hit that does no damage shakes the unit with a miss sound (JP and later also fade the HP gauge on the last
 * hit).
 */
EVT_BEGIN(subsetevt_damage)
    // A flipped shell or a winged unit knocked to the ground runs the unit's own event 13 or 14 and stops here.
    CALL(btlevtcmd_CheckDamageCode, LW(10), DAMAGE_RESULT_FLIPPED, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_DAMAGE_TURN_EVENT)
        RETURN
    END_IF
    CALL(btlevtcmd_CheckDamageCode, LW(10), DAMAGE_RESULT_GROUNDED, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_DAMAGE_FALL_EVENT)
        RETURN
    END_IF

    // Hits the unit's data table can take over: shell attacks (pattern 8, event 16) and pattern 9 (event 17);
    // Spin and Ultra Hammer knockback (14, 16) uses event 18, a hit by a knocked-back unit (15, 17) event 19.
    CALL(btlevtcmd_CheckDamagePattern, LW(10), ATTACKPATTERN_ROTATE_SHELL_ATTACK, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(0), UNIT_DATA_DAMAGE_ROTATE_SHELL_ATTACK_EVENT)
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_DAMAGE_ROTATE_SHELL_ATTACK_EVENT)
            RETURN
        END_IF
    END_IF
    CALL(btlevtcmd_CheckDamagePattern, LW(10), ATTACKPATTERN_SHELL_ATTACK, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(0), UNIT_DATA_DAMAGE_SHELL_ATTACK_EVENT)
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_DAMAGE_SHELL_ATTACK_EVENT)
            RETURN
        END_IF
    END_IF
    CALL(btlevtcmd_CheckDamagePattern, LW(10), ATTACKPATTERN_KAITEN_HAMMER, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(0), UNIT_DATA_DAMAGE_KAITEN_HAMMER_EVENT)
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_DAMAGE_KAITEN_HAMMER_EVENT)
            RETURN
        END_IF
    END_IF
    CALL(btlevtcmd_CheckDamagePattern, LW(10), ATTACKPATTERN_ULTRA_HAMMER, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(0), UNIT_DATA_DAMAGE_KAITEN_HAMMER_EVENT)
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_DAMAGE_KAITEN_HAMMER_EVENT)
            RETURN
        END_IF
    END_IF
    CALL(btlevtcmd_CheckDamagePattern, LW(10), ATTACKPATTERN_KAITEN_HAMMER_INVOLVED, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(0), UNIT_DATA_DAMAGE_KAITEN_HAMMER_INVOLVED_EVENT)
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_DAMAGE_KAITEN_HAMMER_INVOLVED_EVENT)
            RETURN
        END_IF
    END_IF
    CALL(btlevtcmd_CheckDamagePattern, LW(10), ATTACKPATTERN_ULTRA_HAMMER_INVOLVED, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(0), UNIT_DATA_DAMAGE_KAITEN_HAMMER_INVOLVED_EVENT)
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_DAMAGE_KAITEN_HAMMER_INVOLVED_EVENT)
            RETURN
        END_IF
    END_IF

    // Ms. Mowz's Love Slap (patterns 21-24) uses the unit's events 20-23 when it has them.
    CALL(btlevtcmd_CheckDamagePattern, LW(10), ATTACKPATTERN_CHUCHURINA_BINTA_L, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(0), UNIT_DATA_DAMAGE_BINTA_L_EVENT)
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_DAMAGE_BINTA_L_EVENT)
            RETURN
        END_IF
    END_IF
    CALL(btlevtcmd_CheckDamagePattern, LW(10), ATTACKPATTERN_CHUCHURINA_BINTA_R, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(0), UNIT_DATA_DAMAGE_BINTA_R_EVENT)
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_DAMAGE_BINTA_R_EVENT)
            RETURN
        END_IF
    END_IF
    CALL(btlevtcmd_CheckDamagePattern, LW(10), ATTACKPATTERN_CHUCHURINA_BINTA_L_LAST, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(0), UNIT_DATA_DAMAGE_BINTA_L_LAST_EVENT)
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_DAMAGE_BINTA_L_LAST_EVENT)
            RETURN
        END_IF
    END_IF
    CALL(btlevtcmd_CheckDamagePattern, LW(10), ATTACKPATTERN_CHUCHURINA_BINTA_R_LAST, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(0), UNIT_DATA_DAMAGE_BINTA_R_LAST_EVENT)
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_DAMAGE_BINTA_R_LAST_EVENT)
            RETURN
        END_IF
    END_IF

    // Flurrie's Gale Force breath (pattern 25) blows the unit back.
    CALL(btlevtcmd_CheckDamagePattern, LW(10), ATTACKPATTERN_BLOW, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(subsetevt_blow_damage)
        CALL(btlevtcmd_SetRotate, LW(10), 0, 0, 0)
        RETURN
    END_IF

    // Yoshi's Gulp (pattern 4 failed swallow, 5 spat out, 6 spat at another enemy)
    // uses the unit's events 55-57 when it has them.
    CALL(btlevtcmd_CheckDamagePattern, LW(10), ATTACKPATTERN_SWALLOW_FAILED, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(0), UNIT_DATA_DAMAGE_SWALLOW_FAILED_EVENT)
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_DAMAGE_SWALLOW_FAILED_EVENT)
            RETURN
        END_IF
    END_IF
    CALL(btlevtcmd_CheckDamagePattern, LW(10), ATTACKPATTERN_SWALLOW_SPEW, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(0), UNIT_DATA_DAMAGE_SWALLOW_SPEW_EVENT)
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_DAMAGE_SWALLOW_SPEW_EVENT)
            RETURN
        END_IF
    END_IF
    CALL(btlevtcmd_CheckDamagePattern, LW(10), ATTACKPATTERN_SWALLOW_SHOT, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(0), UNIT_DATA_DAMAGE_SWALLOW_SHOT_EVENT)
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_DAMAGE_SWALLOW_SHOT_EVENT)
            RETURN
        END_IF
    END_IF

    // A unit with its own hurt event (data-table event 9) plays it instead of everything below.
    CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(0), UNIT_DATA_DAMAGE_SUB_EVENT)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_DAMAGE_SUB_EVENT)
        RETURN
    END_IF

    // Otherwise the standard Gulp reactions play.
    CALL(btlevtcmd_CheckDamagePattern, LW(10), ATTACKPATTERN_SWALLOW_FAILED, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(subsetevt_swallow_failed_damage)
        RETURN
    END_IF
    CALL(btlevtcmd_CheckDamagePattern, LW(10), ATTACKPATTERN_SWALLOW_SPEW, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(subsetevt_swallow_spew_damage)
        RETURN
    END_IF
    CALL(btlevtcmd_CheckDamagePattern, LW(10), ATTACKPATTERN_SWALLOW_SHOT, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(subsetevt_swallow_shot_damage)
        RETURN
    END_IF

    // The hit did damage. A squashing last hit flattens the unit
    // (pattern 10: Flurrie's Body Slam and similar; 11: PRESS_GONBABA; 12: PRESS_BATTENDER).
    CALL(btlevtcmd_GetDamage, LW(10), LW(0))
    IF_INT_GREATER_OR_EQUAL(LW(0), 1)
        CALL(btlevtcmd_CheckDamageCode, LW(10), DAMAGE_RESULT_FINAL_HIT, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_CheckDamagePattern, LW(10), ATTACKPATTERN_PRESS, LW(0))
            IF_INT_NOT_EQUAL(LW(0), 0)
                GOTO(10)
            END_IF
            CALL(btlevtcmd_CheckDamagePattern, LW(10), ATTACKPATTERN_PRESS_BATTENDER, LW(0))
            IF_INT_NOT_EQUAL(LW(0), 0)
                GOTO(10)
            END_IF
            CALL(btlevtcmd_CheckDamagePattern, LW(10), ATTACKPATTERN_PRESS_GONBABA, LW(0))
            IF_INT_NOT_EQUAL(LW(0), 0)
                GOTO(10)
            END_IF
            GOTO(11)
            LABEL(10)

            // The unit squashes flat, stays flat for 1.5 seconds (3 for pattern 11, 2.5 for 12),
            // then springs back up with a squish sound.
            CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), LW(14))
            BEGIN_SIBLING_EVENT_GET_ID(LW(0))
                CALL(evt_sub_intpl_init_float, EASING_CUBIC_OUT, FLOAT(1.0f), FLOAT(0.14941406f), 10)
                LOOP(11)
                    CALL(evt_sub_intpl_get_float, LW(5), EVT_NULLPTR)
                    CALL(btlevtcmd_GetScale, LW(10), LW(0), LW(1), LW(2))
                    CALL(btlevtcmd_SetScale, LW(10), LW(0), LW(5), LW(2))
                    WAIT_FRAMES(1)
                END_LOOP
            END_SIBLING_EVENT
            CALL(btlevtcmd_WaitEventEnd, LW(0))
            CALL(btlevtcmd_CheckDamagePattern, LW(10), ATTACKPATTERN_PRESS_GONBABA, LW(1))
            IF_INT_NOT_EQUAL(LW(1), 0)
                WAIT_MS(3000)
            ELSE
                CALL(btlevtcmd_CheckDamagePattern, LW(10), ATTACKPATTERN_PRESS_BATTENDER, LW(1))
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
                CALL(evt_sub_intpl_init_float, EASING_CUBIC_OUT, FLOAT(0.14941406f), FLOAT(1.25f), 10)
                LOOP(10)
                    CALL(evt_sub_intpl_get_float, LW(5), EVT_NULLPTR)
                    CALL(btlevtcmd_GetScale, LW(10), LW(0), LW(1), LW(2))
                    CALL(btlevtcmd_SetScale, LW(10), LW(0), LW(5), LW(2))
                    WAIT_FRAMES(1)
                END_LOOP
                CALL(evt_sub_intpl_init_float, EASING_WAVE_SHRINKING, FLOAT(1.25f), FLOAT(1.0f), 30)
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

            // A Spin Hammer knockback on the last hit sends the unit flying into the others.
            CALL(btlevtcmd_CheckDamagePattern, LW(10), ATTACKPATTERN_KAITEN_HAMMER, LW(0))
            IF_INT_NOT_EQUAL(LW(0), 0)
                CALL_CHILD(subsetevt_shot_damage)
                RETURN
            END_IF
        END_IF

        // Ultra Hammer knockback and Ms. Mowz's Love Slap hits play their own reactions.
        CALL(btlevtcmd_CheckDamagePattern, LW(10), ATTACKPATTERN_ULTRA_HAMMER, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_shot_damage)
            RETURN
        END_IF
        CALL(btlevtcmd_CheckDamagePattern, LW(10), ATTACKPATTERN_CHUCHURINA_BINTA_L, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_binta_damage_left)
            RETURN
        END_IF
        CALL(btlevtcmd_CheckDamagePattern, LW(10), ATTACKPATTERN_CHUCHURINA_BINTA_R, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_binta_damage_right)
            RETURN
        END_IF
        CALL(btlevtcmd_CheckDamagePattern, LW(10), ATTACKPATTERN_CHUCHURINA_BINTA_L_LAST, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_binta_damage_left_last)
            RETURN
        END_IF
        CALL(btlevtcmd_CheckDamagePattern, LW(10), ATTACKPATTERN_CHUCHURINA_BINTA_R_LAST, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_binta_damage_right_last)
            RETURN
        END_IF

        // Blooper's ink (pattern 13) holds the hurt pose for 2 seconds with animation flag 32 set;
        // pattern 26 holds it 2 seconds and any other hit 1 second, then the follow-up pose in LW(15) shows.
        CALL(btlevtcmd_CheckDamagePattern, LW(10), ATTACKPATTERN_IKASUMI, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_AnimeFlagOnOff, LW(10), LW(11), PART_ANIM_INKED, 1)
            CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), LW(14))
            WAIT_MS(2000)
            CALL(btlevtcmd_AnimeFlagOnOff, LW(10), LW(11), PART_ANIM_INKED, 0)
        ELSE
            CALL(btlevtcmd_CheckDamagePattern, LW(10), ATTACKPATTERN_BG_FALL_DOWN, LW(0))
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
#if VERSION >= VERSION_JP

        // In JP and later a no-damage last hit fades the HP gauge out.
        CALL(btlevtcmd_CheckDamageCode, LW(10), DAMAGE_RESULT_FINAL_HIT, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(_set_hp_gauge_fade_flag, LW(10))
        END_IF
#endif

        // No damage: a normal hit still shows the hurt pose; anything else plays the miss sound.
        CALL(btlevtcmd_GetDamageCode, LW(10), LW(0))
        SWITCH(LW(0))
            CASE_EQUAL(DAMAGE_CODE_NORMAL)
            CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), LW(14))
            WAIT_MS(1000)
            IF_INT_NOT_EQUAL(LW(15), -1)
                CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), LW(15))
                WAIT_FRAMES(30)
            END_IF
            RETURN
            CASE_DEFAULT
            // The miss sound plays and the unit shudders for 24 frames.
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

/**
 * Hurt reaction for an attacker damaged by the target's counter (LW(0) = damage code: spikes, fire, ice,
 * electric, explosions, poison, Payback and so on):
 * the attacker hops back with the matching pose, Mario cries out on the spike counters (and, in JP and later, on
 * the fire counter).
 */
EVT_BEGIN(subsetevt_counter_damage)
    CALL(btlevtcmd_OnUnitFlag, LW(10), BATTLE_UNIT_FLAG_SHOW_HP)
    BEGIN_SIBLING_EVENT
        SWITCH(LW(0))
            CASE_OR_EQUAL(DAMAGE_CODE_COUNTER_TOP_SPIKY)
            CASE_OR_EQUAL(DAMAGE_CODE_COUNTER_PREEMPTIVE_SPIKY)
            CASE_OR_EQUAL(DAMAGE_CODE_COUNTER_FRONT_SPIKY)
            CALL(btlevtcmd_GetUnitKind, LW(10), LW(2))
            IF_INT_EQUAL(LW(2), UNIT_MARIO)
                CALL(btlevtcmd_GetPos, LW(10), LW(2), LW(3), LW(4))
                CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_DAMAGED1"), LW(2), LW(3), LW(4), 0)
            END_IF
            CASE_END
        END_SWITCH
    END_SIBLING_EVENT
    SWITCH(LW(0))
        CASE_EQUAL(DAMAGE_CODE_COUNTER_TOP_SPIKY)
        CALL(evt_snd_sfxon, STRING("SFX_BTL_ATTACK_MISS3"), 0)
        SET_VALUE(LW(14), POSE_TYPE_DAMAGE_TOP_SPIKY)
#if VERSION <= VERSION_DEMO_JP
        CASE_OR_EQUAL(DAMAGE_CODE_COUNTER_FIERY)
#else
        CASE_EQUAL(DAMAGE_CODE_COUNTER_FIERY)
        CALL(btlevtcmd_GetUnitKind, LW(10), LW(2))
        IF_INT_EQUAL(LW(2), UNIT_MARIO)
            CALL(btlevtcmd_snd_se, LW(10), STRING("SFX_VOICE_MARIO_DAMAGED1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        END_IF
        SET_VALUE(LW(14), POSE_TYPE_DAMAGE_FIRE)
#endif
        CASE_OR_EQUAL(DAMAGE_CODE_COUNTER_EXPLOSIVE)
        CASE_OR_EQUAL(DAMAGE_CODE_COUNTER_VOLATILE_EXPLOSIVE)
        CASE_OR_EQUAL(DAMAGE_CODE_COUNTER_HOLD_FAST)
        SET_VALUE(LW(14), POSE_TYPE_DAMAGE_FIRE)
        CASE_END
        CASE_DEFAULT
        SET_VALUE(LW(14), POSE_TYPE_DAMAGE)
    END_SWITCH
    CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), LW(14))
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_DAMAGE_POSE_EVENT)
    END_SIBLING_EVENT
    CALL(btlevtcmd_SetFallAccel, LW(10), FLOAT(1.0f))
    CALL(btlevtcmd_GetPos, LW(10), LW(2), LW(3), LW(4))
    CALL(btlevtcmd_FaceDirectionSub, LW(10), LW(2), 25)
    ADD(LW(3), 25)
    CALL(btlevtcmd_DivePosition, LW(10), LW(2), LW(3), LW(4), 10, 0, EASING_LINEAR, 0, MOVE_SOUND_FROM_TABLE)
    WAIT_MS(500)
    CALL(btlevtcmd_SetFallAccel, LW(10), FLOAT(1.0f))
    CALL(btlevtcmd_GetPos, LW(10), LW(2), LW(3), LW(4))
    CALL(btlevtcmd_TransStageFloorPosition, LW(2), LW(3), LW(4))
    CALL(btlevtcmd_FaceDirectionSub, LW(10), LW(2), 30)
    CALL(btlevtcmd_JumpPosition, LW(10), LW(2), LW(3), LW(4), 20, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_FaceDirectionSub, LW(10), LW(2), 15)
    CALL(btlevtcmd_JumpPosition, LW(10), LW(2), LW(3), LW(4), 10, MOVE_SOUND_FROM_TABLE)
    WAIT_FRAMES(30)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    RETURN
EVT_END()

/** Plays the miss sound for an attack that did nothing (the early demos' copy). */
#if VERSION <= VERSION_DEMO_JP
EVT_BEGIN(subsetevt_nodamage_motion)
    CALL(evt_snd_sfxon, STRING("SFX_BTL_ATTACK_MISS2"), 0)
    RETURN
EVT_END()
#endif

/**
 * Swaps Mario and the partner (the front-and-back switch):
 * both arc to each other's home positions and the Koops shell shield follows Mario.
 * In JP and later the shell reopens at its new place only when it still has HP.
 */
EVT_BEGIN(btldefaultevt_ChangePosition)
    CALL(btlevtcmd_GetGuardKouraId, UNIT_ID_MARIO, LW(10))
    IF_INT_NOT_EQUAL(LW(10), -1)
        CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_KOURA_CLOSE_EVENT)
        CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_MARIO, 1, kHidden)
        CALL(btlevtcmd_SetUnitWork, LW(10), SHELL_SHIELD_WORK_FOLLOW, KOURA_FOLLOW_TRAIL)
    END_IF
    CALL(evt_snd_sfxon, STRING("SFX_BTL_POS_CHANGE1"), 0)
    CALL(btlevtcmd_GetPartnerId, UNIT_ID_SELF, LW(3))
    CALL(btlevtcmd_GetBodyId, UNIT_ID_SELF, LW(5))
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_INSTANT_KILL, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(5), POSE_TYPE_RUN)
    END_IF
    CALL(btlevtcmd_GetBodyId, LW(3), LW(5))
    CALL(btlevtcmd_CheckStatus, LW(3), STATUS_INSTANT_KILL, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_AnimeChangePoseType, LW(3), LW(5), POSE_TYPE_RUN)
    END_IF
    BEGIN_INLINE_EVENT
        CALL(btlevtcmd_GetHomePos, LW(3), LW(5), LW(6), LW(7))
        CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(5), LW(8), LW(7))
        CALL(btlevtcmd_EllipseMovePosition, LW(3), LW(5), LW(6), LW(7), 20, 15, EASING_LINEAR)
        CALL(btlevtcmd_SetHomePos, LW(3), LW(5), LW(6), LW(7))
    END_INLINE_EVENT
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHomePos, LW(3), LW(0), LW(4), LW(2))
    CALL(btlevtcmd_EllipseMovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 20, 15, EASING_LINEAR)
    CALL(btlevtcmd_SetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    WAIT_FRAMES(2)
    CALL(btlevtcmd_GetGuardKouraId, UNIT_ID_MARIO, LW(10))
    IF_INT_NOT_EQUAL(LW(10), -1)
        CALL(btlevtcmd_SetUnitWork, LW(10), SHELL_SHIELD_WORK_FOLLOW, KOURA_FOLLOW_SLIDE)
        LABEL(10)
        CALL(btlevtcmd_GetUnitWork, LW(10), SHELL_SHIELD_WORK_FOLLOW, LW(0))
        IF_INT_EQUAL(LW(0), KOURA_FOLLOW_SLIDE)
            WAIT_FRAMES(1)
            GOTO(10)
        END_IF
#if VERSION >= VERSION_JP
        CALL(btlevtcmd_GetHp, LW(10), LW(0))
        IF_INT_GREATER(LW(0), 0)
#endif
            CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_KOURA_OPEN_EVENT)
            CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
            CALL(btlevtcmd_SetHomePos, LW(10), LW(0), LW(1), LW(2))
#if VERSION >= VERSION_JP
        END_IF
#endif
    END_IF
    CALL(btlevtcmd_SortPhaseMoveTable)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_MARIO)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_PARTNER)
    RETURN
EVT_END()

/**
 * Happy Heart badge regeneration: the unit strikes its happy pose (JP and later: only when LW(3) is not frozen,
 * asleep or stopped) and waits a moment.
 */
EVT_BEGIN(subsetevt_happy_heart)
    CALL(btlevtcmd_GetBodyId, UNIT_ID_SELF, LW(5))
    CALL(btlevtcmd_CheckDefinePosetype, UNIT_ID_SELF, LW(5), POSE_TYPE_HAPPY_HEART, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
#if VERSION <= VERSION_DEMO_JP
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(5), 60)
#else
        CALL(btlevtcmd_CheckStatus, LW(3), STATUS_FREEZE, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_CheckStatus, LW(3), STATUS_SLEEP, LW(0))
            IF_INT_EQUAL(LW(0), 0)
                CALL(btlevtcmd_CheckStatus, LW(3), STATUS_STOP, LW(0))
                IF_INT_EQUAL(LW(0), 0)
                    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(5), POSE_TYPE_HAPPY_HEART)
#endif
                END_IF
#if VERSION <= VERSION_DEMO_JP
            WAIT_FRAMES(120)
#else
            END_IF
        END_IF
    END_IF
    WAIT_FRAMES(60)
#endif
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Happy Flower badge regeneration: like subsetevt_happy_heart for FP. */
EVT_BEGIN(subsetevt_happy_flower)
    CALL(btlevtcmd_GetBodyId, UNIT_ID_SELF, LW(5))
    CALL(btlevtcmd_CheckDefinePosetype, UNIT_ID_SELF, LW(5), POSE_TYPE_HAPPY_FLOWER, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
#if VERSION <= VERSION_DEMO_JP
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(5), 61)
#else
        CALL(btlevtcmd_CheckStatus, LW(3), STATUS_FREEZE, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_CheckStatus, LW(3), STATUS_SLEEP, LW(0))
            IF_INT_EQUAL(LW(0), 0)
                CALL(btlevtcmd_CheckStatus, LW(3), STATUS_STOP, LW(0))
                IF_INT_EQUAL(LW(0), 0)
                    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(5), POSE_TYPE_HAPPY_FLOWER)
#endif
                END_IF
#if VERSION <= VERSION_DEMO_JP
            WAIT_FRAMES(120)
#else
            END_IF
        END_IF
    END_IF
    WAIT_FRAMES(60)
#endif
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Pause after the HP or FP Regen status heals a unit at turn start (the early demos' copy, 120 frames). */
#if VERSION <= VERSION_DEMO_JP
EVT_BEGIN(subsetevt_regeneration)
    WAIT_FRAMES(120)
    RETURN
EVT_END()
#endif

/** Fright: the unit hops, runs off the edge of the stage and is removed from the battle (no coins or Star Points). */
EVT_BEGIN(subsetevt_terror)
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 20, FLOAT(0.0f), FLOAT(0.69921875f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 16, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(10.0f))
    CALL(btlevtcmd_GetStageSize, LW(0), EVT_NULLPTR, EVT_NULLPTR)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_ESCAPE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    CALL(btlevtcmd_KillUnit, UNIT_ID_SELF, 0)
    RETURN
EVT_END()

/**
 * Gale Force blow-away that fails: the unit spins up and flies to the stage edge,
 * then lands back on the floor there.
 */
EVT_BEGIN(subsetevt_blow_damage)
    CALL(evt_snd_sfxon, STRING("SFX_BTL_CLAUD_BREATH3"), 0)
    CALL(btlevtcmd_GetBodyId, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(0), POSE_TYPE_DAMAGE)
    BEGIN_SIBLING_EVENT
        LABEL(0)
        CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 20, 0)
        WAIT_FRAMES(1)
        GOTO(0)
    END_SIBLING_EVENT
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(2.0f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetStageSize, LW(0), EVT_NULLPTR, EVT_NULLPTR)
    ADD(LW(1), 40)
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -10, EASING_CUBIC_IN, 0, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPos, LW(10), LW(0), LW(1), LW(2))
    RETURN
EVT_END()

/**
 * Gale Force blow-away that succeeds: the unit spins and flies off the stage;
 * its coins and Star Points are still banked before it is removed.
 */
EVT_BEGIN(subsetevt_blow_dead)
    CALL(evt_snd_sfxon, STRING("SFX_BTL_CLAUD_BREATH3"), 0)
    CALL(btlevtcmd_GetBodyId, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(0), POSE_TYPE_DAMAGE)
    BEGIN_SIBLING_EVENT
        LABEL(0)
        CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 20, 0)
        WAIT_FRAMES(1)
        GOTO(0)
    END_SIBLING_EVENT
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(2.0f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetStageSize, LW(0), EVT_NULLPTR, EVT_NULLPTR)
    ADD(LW(1), 40)
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -10, EASING_CUBIC_IN, 0, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_GetCoin, LW(10), LW(0))
    CALL(btlevtcmd_StoreCoin, LW(0))
    CALL(btlevtcmd_GetExp, LW(10), LW(0))
    CALL(btlevtcmd_StoreExp, LW(0))
    CALL(btlevtcmd_KillUnit, UNIT_ID_SELF, 0)
    RETURN
EVT_END()

/**
 * A hammer knockback hit sends the unit flying into the units behind it:
 * Spin Hammer (damage pattern 14) or Ultra Hammer, with the matching "involved" weapon in LW(12).
 */
EVT_BEGIN(subsetevt_shot_damage)
    CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), LW(14))
    CALL(btlevtcmd_OnToken, LW(10), kKnockedAway)
    CALL(btlevtcmd_CheckDamagePattern, LW(10), ATTACKPATTERN_KAITEN_HAMMER, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(LW(12), PTR(&marioWeapon_KaitenHammerInvolved))
        CALL_CHILD(PTR(subsetevt_shot_damage_kaiten))
    ELSE
        SET_VALUE(LW(12), PTR(&marioWeapon_UltraHammerInvolved))
        CALL_CHILD(PTR(subsetevt_shot_damage_ultra))
    END_IF
    RETURN
EVT_END()

/**
 * Spin Hammer knockback: the unit spins and flies along the line of enemies behind it, hitting each one,
 * then off the stage edge; a unit the hit brought to 0 HP drops back in from the other side and returns home,
 * any other is set down on the floor where it landed.
 */
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
    CALL(btlevtcmd_JumpPosition, LW(10), LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_PreCheckDamage, LW(10), LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
    SWITCH(LW(5))
        CASE_EQUAL(PRECHECK_MISS)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
        CASE_EQUAL(PRECHECK_LUCKY)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
        CASE_EQUAL(PRECHECK_DODGY)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
        CASE_EQUAL(PRECHECK_INTANGIBLE)
        CASE_EQUAL(PRECHECK_HIT)
        CALL(btlevtcmd_CheckDamage, LW(10), LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
        CASE_DEFAULT
    END_SWITCH
    GOTO(10)
    LABEL(50)
    CALL(btlevtcmd_GetStageSize, LW(5), EVT_NULLPTR, EVT_NULLPTR)
    SET_VALUE(LW(0), 0)
    CALL(btlevtcmd_FaceDirectionSub, LW(10), LW(0), LW(5))
    CALL(btlevtcmd_GetPos, LW(10), EVT_NULLPTR, LW(1), EVT_NULLPTR)
    CALL(btlevtcmd_JumpSetting, LW(10), 0, FLOAT(5.0f), FLOAT(0.009765625f))
    CALL(btlevtcmd_JumpPosition, LW(10), LW(0), 40, LW(2), 0, MOVE_SOUND_FROM_TABLE)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_CheckDamageCode, LW(10), DAMAGE_RESULT_ZERO_HP, LW(6))
    IF_INT_NOT_EQUAL(LW(6), 0)
        MULTIPLY(LW(0), -1)
        CALL(btlevtcmd_SetPos, LW(10), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetHomePos, LW(10), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionAdd, LW(10), LW(0), 40)
        CALL(btlevtcmd_JumpSetting, LW(10), 0, FLOAT(7.0f), FLOAT(0.099609375f))
        CALL(btlevtcmd_JumpPosition, LW(10), LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE)
        CALL(btlevtcmd_GetHomePos, LW(10), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_MovePosition, LW(10), LW(0), LW(1), LW(2), 16, MOVE_SOUND_FROM_TABLE, 0)
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

/**
 * Ultra Hammer knockback: the unit tumbles through the enemies behind it to the last one,
 * damaging each as it passes (_check_through_pos), flies off the stage and bounces back to its home position.
 */
EVT_BEGIN(subsetevt_shot_damage_ultra)
    // The unit starts spinning the way it faces and keeps spinning while it flies.
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

    // Find this unit in the line of enemies and the last enemy behind it;
    // with none behind it, the unit just flies off.
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

    // The unit flies in an arc to just past the last enemy.
    BEGIN_SIBLING_EVENT_GET_ID(LW(5))
        CALL(btlevtcmd_JumpSetting, LW(10), 0, FLOAT(5.0f), FLOAT(0.39941406f))
        CALL(btlevtcmd_GetHitPos, LW(8), LW(9), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionAdd, LW(8), LW(0), 20)
        CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_JumpPosition, LW(10), LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE)
    END_SIBLING_EVENT

    // While it flies, every enemy it passes is hit by the knocked-back unit,
    // unless it dodges or the attack misses.
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
#if VERSION >= VERSION_JP
        IS_EVENT_RUNNING(LW(5), LW(6))
        IF_INT_EQUAL(LW(6), 1)
#endif
            SET_VALUE(LW(1), LW(0))
            WAIT_FRAMES(1)
            GOTO(32)
#if VERSION >= VERSION_JP
        END_IF
#endif
    END_IF
    CALL(btlevtcmd_PreCheckDamage, LW(10), LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(PRECHECK_MISS)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
        CASE_EQUAL(PRECHECK_LUCKY)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
        CASE_EQUAL(PRECHECK_DODGY)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
        CASE_EQUAL(PRECHECK_INTANGIBLE)
        CASE_EQUAL(PRECHECK_HIT)
        CALL(btlevtcmd_CheckDamage, LW(10), LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(0))
        CASE_DEFAULT
    END_SWITCH
    GOTO(31)
    DELETE_EVENT(LW(5))
    LABEL(50)

    // The unit flies off the stage edge.
    CALL(btlevtcmd_GetStageSize, LW(5), EVT_NULLPTR, EVT_NULLPTR)
    SET_VALUE(LW(0), 0)
    CALL(btlevtcmd_FaceDirectionSub, LW(10), LW(0), LW(5))
    CALL(btlevtcmd_GetPos, LW(10), EVT_NULLPTR, LW(1), EVT_NULLPTR)
    CALL(btlevtcmd_JumpSetting, LW(10), 0, FLOAT(5.0f), FLOAT(0.099609375f))
    CALL(btlevtcmd_JumpPosition, LW(10), LW(0), 80, LW(2), 0, MOVE_SOUND_FROM_TABLE)
    WAIT_FRAMES(30)

    // For an Ultra Hammer hit it waits off stage until Mario's attack script calls it back.
    CALL(btlevtcmd_CheckDamagePattern, LW(10), ATTACKPATTERN_ULTRA_HAMMER, LW(5))
    IF_INT_NOT_EQUAL(LW(5), 0)
        CALL(btlevtcmd_OnToken, LW(10), kKnockedAwayWaiting)
        LABEL(55)
        CALL(btlevtcmd_CheckToken, LW(10), kKnockedAwayWaiting, LW(5))
        IF_INT_NOT_EQUAL(LW(5), 0)
            WAIT_FRAMES(1)
            GOTO(55)
        END_IF
    END_IF

    // It drops back in from the other side and bounces home;
    // for an Ultra Hammer hit it signals Mario's script and waits for the attack to end.
    MULTIPLY(LW(0), -1)
    CALL(btlevtcmd_SetPos, LW(10), LW(0), 80, LW(2))
    CALL(btlevtcmd_GetHomePos, LW(10), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpSetting, LW(10), 0, FLOAT(7.0f), FLOAT(0.099609375f))
    CALL(btlevtcmd_JumpPosition, LW(10), LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_CheckDamagePattern, LW(10), ATTACKPATTERN_ULTRA_HAMMER, LW(5))
    IF_INT_NOT_EQUAL(LW(5), 0)
        CALL(btlevtcmd_OnToken, LW(10), kKnockedAwayReturned)
        CALL(btlevtcmd_WaitAttackEnd)
    END_IF

    // No IF_* is open here: this END_IF has no matching IF.
    END_IF

    // A last hop at home, then the spin stops.
    CALL(btlevtcmd_JumpSetting, LW(10), 0, FLOAT(0.0f), FLOAT(0.14941406f))
    CALL(btlevtcmd_JumpPosition, LW(10), LW(0), LW(1), LW(2), 20, MOVE_SOUND_FROM_TABLE)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_CheckDamageCode, LW(10), DAMAGE_RESULT_FINAL_HIT, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_WaitAttackEnd)
    END_IF
    DELETE_EVENT(LW(13))
    CALL(btlevtcmd_SetRotate, LW(10), 0, 0, 0)
    RETURN
EVT_END()

/** Yoshi's Gulp on a unit that could not be swallowed: it is knocked back with a hop. */
EVT_BEGIN(subsetevt_swallow_failed_damage)
    CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), POSE_TYPE_DAMAGE)
    CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetFallAccel, LW(10), FLOAT(0.5f))
    CALL(btlevtcmd_FaceDirectionSub, LW(10), LW(0), 30)
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, LW(10), LW(0), LW(1), LW(2), 20, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_FaceDirectionSub, LW(10), LW(0), 18)
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, LW(10), LW(0), LW(1), LW(2), 12, MOVE_SOUND_FROM_TABLE)
    WAIT_FRAMES(20)
    RETURN
EVT_END()

/** Yoshi spits a swallowed unit out without hitting anyone: it lands with a hop. */
EVT_BEGIN(subsetevt_swallow_spew_damage)
    CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), POSE_TYPE_DAMAGE)
    CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetFallAccel, LW(10), FLOAT(0.5f))
    CALL(btlevtcmd_FaceDirectionSub, LW(10), LW(0), 30)
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, LW(10), LW(0), LW(1), LW(2), 20, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_FaceDirectionSub, LW(10), LW(0), 18)
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, LW(10), LW(0), LW(1), LW(2), 12, MOVE_SOUND_FROM_TABLE)
    WAIT_FRAMES(20)
    RETURN
EVT_END()

/**
 * Yoshi's Gulp spits the unit at the enemy behind it:
 * it flies at that enemy and damages it, then bounces away, or flies off past x = 400 when nothing is hit.
 */
EVT_BEGIN(subsetevt_swallow_shot_damage)
    // The spat-out unit becomes Yoshi's projectile and looks for the enemy behind it in line.
    CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), LW(14))
    SET_VALUE(LW(12), PTR(&partyWeapon_YoshiNomikomi_Involved))
    CALL(btlevtcmd_GetFriendBelong, LW(10), LW(0))
    CALL(btlevtcmd_SamplingEnemy, LW(10), LW(0), LW(12))
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    LABEL(0)
    IF_INT_EQUAL(LW(3), -1)
        // No enemies in line: the unit lands with two hops.
        CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), POSE_TYPE_DAMAGE)
        CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_SetFallAccel, LW(10), FLOAT(0.5f))
        CALL(btlevtcmd_FaceDirectionSub, LW(10), LW(0), 30)
        CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_JumpPosition, LW(10), LW(0), LW(1), LW(2), 20, MOVE_SOUND_FROM_TABLE)
        CALL(btlevtcmd_FaceDirectionSub, LW(10), LW(0), 18)
        CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_JumpPosition, LW(10), LW(0), LW(1), LW(2), 12, MOVE_SOUND_FROM_TABLE)
        WAIT_FRAMES(20)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_GetUnitId, LW(10), LW(0))
    IF_INT_NOT_EQUAL(LW(0), LW(3))
        CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
        GOTO(0)
    END_IF

    // The next enemy in line is the target; with none, the unit lands with two hops.
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
#if VERSION >= VERSION_JP
    DEBUG_PUT_REG(LW(3))
#endif
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), POSE_TYPE_DAMAGE)
        CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_SetFallAccel, LW(10), FLOAT(0.5f))
        CALL(btlevtcmd_FaceDirectionSub, LW(10), LW(0), 30)
        CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_JumpPosition, LW(10), LW(0), LW(1), LW(2), 20, MOVE_SOUND_FROM_TABLE)
        CALL(btlevtcmd_FaceDirectionSub, LW(10), LW(0), 18)
        CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_JumpPosition, LW(10), LW(0), LW(1), LW(2), 12, MOVE_SOUND_FROM_TABLE)
        WAIT_FRAMES(20)
        GOTO(99)
    END_IF

    // The unit flies at the target, which takes the hit unless it dodges or the attack misses.
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpSetting, LW(10), 0, FLOAT(5.0f), FLOAT(0.099609375f))
    CALL(btlevtcmd_JumpPosition, LW(10), LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_PARTNER, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
    SWITCH(LW(5))
        CASE_EQUAL(PRECHECK_MISS)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
        CASE_EQUAL(PRECHECK_LUCKY)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
        CASE_EQUAL(PRECHECK_DODGY)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
        CASE_EQUAL(PRECHECK_INTANGIBLE)
        CASE_EQUAL(PRECHECK_HIT)
        CALL(btlevtcmd_CheckDamage, UNIT_ID_PARTNER, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT | DAMAGE_RESULT_GULP, LW(5))
        GOTO(50)
        CASE_DEFAULT
    END_SWITCH
    LABEL(20)

    // Without a hit the unit keeps flying until it leaves the stage (past x = 400).
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

    // After a hit the unit bounces back off the target with two hops.
    CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetFallAccel, LW(10), FLOAT(0.5f))
    CALL(btlevtcmd_FaceDirectionAdd, LW(10), LW(0), 30)
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, LW(10), LW(0), LW(1), LW(2), 20, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_FaceDirectionAdd, LW(10), LW(0), 18)
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, LW(10), LW(0), LW(1), LW(2), 12, MOVE_SOUND_FROM_TABLE)
    WAIT_FRAMES(20)
    LABEL(99)
    RETURN
EVT_END()

/**
 * Reaction for damage pattern 18, which no weapon in the tree uses (btldefaultevt_Damage checks for it):
 * the unit takes pose type 39 and is knocked back 40 then 18 units with two hops,
 * then sets its kKnockedAwayReturned token.
 * Guess: meant for an Ultra Hammer acrobatic hit, from the name.
 */
EVT_BEGIN(subset_ultra_hammer_acrobat_damage_event)
    CALL(btlevtcmd_SetRotate, LW(10), 0, 0, 0)
    CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), POSE_TYPE_DAMAGE)
    CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, LW(10), LW(0), 40)
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetFallAccel, LW(10), FLOAT(0.5f))
    CALL(btlevtcmd_JumpPosition, LW(10), LW(0), LW(1), LW(2), 24, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_FaceDirectionSub, LW(10), LW(0), 18)
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, LW(10), LW(0), LW(1), LW(2), 12, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_OnToken, LW(10), kKnockedAwayReturned)
    RETURN
EVT_END()

EVT_BEGIN(subsetevt_confuse_flustered)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, -1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(btlevtcmd_GetBodyId, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(0), POSE_TYPE_CONFUSE)
    WAIT_MS(1500)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    WAIT_MS(1000)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Slap from the left (damage pattern 21): the unit twists one turn round over 20 frames. */
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

/** Slap from the right (damage pattern 22): the mirror of subsetevt_binta_damage_left. */
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

/**
 * Final slap from the left (pattern 23): the unit spins six times round over 90 frames inside a whirlwind
 * effect.
 * Pattern 23 is the last left slap of Ms. Mowz's Love Slap (partyWeapon_ChuchurinaNormalAttackLeftLast)
 * and of its copy enemyWeapon_ChuchurinaNormalAttackLlast in las.
 */
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
        CALL(evt_sub_intpl_init, EASING_SINE_IN_OUT, 0, LW(8), 90)
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

/** Final slap from the right (pattern 24): the mirror of subsetevt_binta_damage_left_last. */
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
        CALL(evt_sub_intpl_init, EASING_SINE_IN_OUT, 0, LW(8), 90)
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

/**
 * A unit recovers from a status that stopped it (sleep, stop, freeze and similar):
 * its data-table event 8 if it has one, else it returns to its normal pose with a small hop.
 */
EVT_BEGIN(subsetevt_status_recovery)
    CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(0), UNIT_DATA_STATUS_RECOVERY_EVENT)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_STATUS_RECOVERY_EVENT)
        RETURN
    END_IF
    CALL(btlevtcmd_StopWaitEvent, UNIT_ID_SELF)
    CALL(btlevtcmd_GetBodyId, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.39941406f))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 30, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

#if VERSION >= VERSION_JP
EVT_BEGIN(subsetevt_check_target_movable)
    CALL(btlevtcmd_CheckStatus, LW(3), STATUS_INSTANT_KILL, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(LW(0), 0)
        RETURN
    END_IF
    CALL(btlevtcmd_CheckStatus, LW(3), STATUS_FREEZE, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(LW(0), 0)
        RETURN
    END_IF
    CALL(btlevtcmd_CheckStatus, LW(3), STATUS_STOP, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(LW(0), 0)
        RETURN
    END_IF

    // The same STATUS_STOP check a second time.
    CALL(btlevtcmd_CheckStatus, LW(3), STATUS_STOP, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(LW(0), 0)
        RETURN
    END_IF
    SET_VALUE(LW(0), 1)
    RETURN
EVT_END()
#endif

/** Plays the miss sound for an attack that did nothing. */
#if VERSION >= VERSION_JP
EVT_BEGIN(subsetevt_nodamage_motion)
    CALL(evt_snd_sfxon, STRING("SFX_BTL_ATTACK_MISS2"), 0)
    RETURN
EVT_END()
#endif

/** Pause after the HP or FP Regen status heals a unit at turn start. */
#if VERSION >= VERSION_JP
EVT_BEGIN(subsetevt_regeneration)
    WAIT_FRAMES(60)
    RETURN
EVT_END()
#endif

/** Empty data-table event 7 (attack ready) for partners that have nothing to do there. */
EVT_BEGIN(subset_attack_ready_jump_event)
    RETURN
EVT_END()

USER_FUNC(_disable_restore_command_cursor) {
    BattleWork* wp = _battleWorkPointer;
    wp->commandMenu.recCommand = 0;
    wp->commandMenu.attackAudience = 0;
    return EVT_RETURN_DONE;
}

/** Script: blocks until the unit args[0] has no attack or damage event running. */
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

/**
 * Script: adds the unit's starPointDispOffset (from its kind data) to the position in args[1..3],
 * where the Star Point effect appears when it is defeated.
 */
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
    x += unit->kindParams->starPointDispOffset.x;
    y += unit->kindParams->starPointDispOffset.y;
    z += unit->kindParams->starPointDispOffset.z;
    evtSetValue(event, xi, x);
    evtSetValue(event, yi, y);
    evtSetValue(event, zi, z);
    return EVT_RETURN_DONE;
}

// Descriptive name (not in the symbol maps); placeholder unk_8010ee30, JP 0x8010EE30. See docs/NAME_PROVENANCE.md.
#if VERSION >= VERSION_JP
USER_FUNC(_set_hp_gauge_fade_flag) {
    BattleWork* wp = _battleWorkPointer;
    s32 id;
    BattleUnit* unit;

    id = evtGetValue(event, event->args[0]);
    unit = BattleGetUnitPtr(wp, BattleTransID(event, id));
    if (unit) {
        unit->hpGauge.flags |= HP_GAUGE_FLAG_TIMED;
    }
    return EVT_RETURN_DONE;
}
#endif

/**
 * Script: sets args[4] to 1 when x = args[1] lies between args[2] and args[3],
 * or beyond them in the direction the unit args[0] faces;
 * the Ultra Hammer knockback uses it to tell when the flying unit has passed a target.
 */
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
    } else {
        max = b;
        min = a;
    }
    if (pos >= min && pos <= max) {
        evtSetValue(event, out, TRUE);
    } else {
        evtSetValue(event, out, FALSE);
        if (unit->faceDirection >= 0) {
            if (pos > max) {
                evtSetValue(event, out, TRUE);
            }
        } else if (pos < min) {
            evtSetValue(event, out, TRUE);
        }
    }
    return EVT_RETURN_DONE;
}

/**
 * Script: starts a 90-frame whirlwind effect at the unit args[0], turning the way of args[1]'s sign;
 * used by the final slaps.
 */
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
    eff = effWhirlwindN64Entry(dir < 0, NULL, unit->sizeMultiplier, 90);
    ((EffWhirlwindN64Work*)eff->userdata)->position.x = pos.x;
    ((EffWhirlwindN64Work*)eff->userdata)->position.y = pos.y;
    ((EffWhirlwindN64Work*)eff->userdata)->position.z = pos.z;
    return EVT_RETURN_DONE;
}
