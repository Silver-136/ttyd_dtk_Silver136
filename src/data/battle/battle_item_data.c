// Battle weapon data for usable items (mushrooms, cooked dishes and so on) and their effects.

#include "version.h"
#include "battle/battle.h"
#include "battle/battle_sub.h"
#include "battle/battle_unit.h"
#include "battle/battle_database_common.h"
#include "data/item_data.h"
#include "driver/animdrv.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "driver/icondrv.h"
#include "evt/evt_cmd.h"
#include "manager/evtmgr.h"
#include "manager/evtmgr_cmd.h"
#include "mario/mario_pouch.h"
#include "pmario_sound.h"
#include "system.h"
#include <dolphin/mtx.h>
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_audience.h"
#include "evt/evt_mobj.h"
#include "statuswindow.h"
#include "battle/battle_item_data.h"
#include "evt/evt_eff.h"
#include "evt/evt_env.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_sub.h"
#include "battle/battle_detect.h"
#include "battle/battle_damage.h"
#include "eff/eff_naniga.h"
#include "driver/mapdrv.h"

static void _nancy_disp(CameraId cameraId, void* param);
static void _disp_item_icon(CameraId cameraId, void* param);

static USER_FUNC(_cam_rot);
static USER_FUNC(_get_throw_first_pos);
static USER_FUNC(_nancy_mirage);
static USER_FUNC(_naniga_okorukana_check);
static USER_FUNC(_throw_item_icon);
static USER_FUNC(_hpfp_change_declare_2);
static USER_FUNC(_hpfp_change_declare_1);
static USER_FUNC(_rank_up);
static USER_FUNC(_sampling_random_target);

static EvtWord _return_home_event[44];
static EvtWord ItemEvent_Automatic_Target[38];
static EvtWord ItemEvent_Recovery_Core_Effect[125];
static EvtWord ItemEvent_Recovery_Core[22];
static EvtWord ItemEvent_Recovery[125];
static EvtWord ItemEvent_Support_Sub_UseDeclere[86];
static EvtWord ItemEvent_GetTarget[68];
static EvtWord ItemEvent_Support_Sub_Effect[71];
static EvtWord ItemEvent_Support_NoEffect[20];
static EvtWord ItemEvent_Support_RefreshEffect[101];
BattleWeapon ItemWeaponData_CookingItem;
BattleWeapon ItemWeaponData_Kinoko;
BattleWeapon ItemWeaponData_Kinkyuu_Kinoko;
BattleWeapon ItemWeaponData_Super_Kinoko;
BattleWeapon ItemWeaponData_Ultra_Kinoko;
BattleWeapon ItemWeaponData_Sinabita_Kinoko;
BattleWeapon ItemWeaponData_Honey_Syrup;
BattleWeapon ItemWeaponData_Maple_Syrup;
BattleWeapon ItemWeaponData_Royal_Jelly;
BattleWeapon ItemWeaponData_Tankobu;
static EvtWord ItemEvent_Pow_Block[418];
BattleWeapon ItemWeaponData_Pow_Block;
static EvtWord ItemEvent_Fire_Flower_Core[192];
static EvtWord ItemEvent_Fire_Flower[31];
BattleWeapon ItemWeaponData_Fire_Flower;
static EvtWord ItemEvent_Kaminari_Core[220];
static EvtWord ItemEvent_Kaminari[83];
BattleWeapon ItemWeaponData_Kaminari_Dokkan;
BattleWeapon ItemWeaponData_Kaminari_Gorogoro;
static EvtWord ItemEvent_Kirakira_Otoshi_Core[451];
static EvtWord ItemEvent_Kirakira_Otoshi[29];
BattleWeapon ItemWeaponData_Kirakira_Otoshi;
static EvtWord ItemEvent_Koorino_Ibuki[310];
BattleWeapon ItemWeaponData_Koorino_Ibuki;
static EvtWord ItemEvent_Yurayura_Jishin[342];
BattleWeapon ItemWeaponData_Yurayura_Jishin;
static EvtWord ItemEvent_Teresano_Fuku[236];
BattleWeapon ItemWeaponData_Teresano_Fuku;
static EvtWord ItemEvent_Biribiri_Kinoko_Core[13];
static EvtWord ItemEvent_Biribiri_Kinoko[14];
BattleWeapon ItemWeaponData_Biribiri_Kinoko;
static EvtWord ItemEvent_Teki_Yokeeru_Core[13];
static EvtWord ItemEvent_Teki_Yokeeru[14];
BattleWeapon ItemWeaponData_Teki_Yokeeru;
BattleWeapon ItemWeaponData_Madowaseno_Kona;
static EvtWord ItemEvent_Nemure_Yoikoyo[VERSION_PICK(57, 91)];
BattleWeapon ItemWeaponData_Nemure_Yoikoyo;
static EvtWord ItemEvent_Stop_Watch_Core[143];
static EvtWord ItemEvent_Stop_Watch[86];
BattleWeapon ItemWeaponData_Stop_Watch;
static EvtWord ItemEvent_Guruguru_Memawashi[35];
BattleWeapon ItemWeaponData_Guruguru_Memawashi;
BattleWeapon ItemWeaponData_Dekadeka_Drink;
BattleWeapon ItemWeaponData_Katikati_Koura;
static EvtWord ItemEvent_Suitooru[281];
BattleWeapon ItemWeaponData_Suitooru;
static EvtWord ItemEvent_Teki_Kyouka[236];
BattleWeapon ItemWeaponData_Teki_Kyouka;
BattleWeapon ItemWeaponData_Minimini_Kun;
BattleWeapon ItemWeaponData_Funyafunya_Kun;
BattleWeapon ItemWeaponData_Sukkiri_Drink;
static EvtWord ItemEvent_Jiwajiwa_Kinoko_Core[13];
static EvtWord ItemEvent_Jiwajiwa_Kinoko[14];
BattleWeapon ItemWeaponData_Jiwajiwa_Kinoko;
static EvtWord ItemEvent_Jiwajiwa_Syrup_Core[13];
static EvtWord ItemEvent_Jiwajiwa_Syrup[14];
BattleWeapon ItemWeaponData_Jiwajiwa_Syrup;
static EvtWord ItemEvent_Irekaeeru[157];
BattleWeapon ItemWeaponData_Irekaeeru;

#if VERSION <= VERSION_DEMO_JP
extern EvtWord ItemEvent_Attiike_Shisshi[];
#else
EvtWord ItemEvent_Attiike_Shisshi[167];
#endif

BattleWeapon ItemWeaponData_Attiike_Shisshi;
static EvtWord ItemEvent_Naniga_Okorukana[207];
BattleWeapon ItemWeaponData_Naniga_Okorukana;
static s32 _item_table[14];
BattleWeapon ItemWeaponData_Shikaeshino_Kona;
BattleWeapon ItemWeaponData_Kameno_Noroi;
static EvtWord ItemEvent_RecoveryAndStatus[246];
BattleWeapon ItemWeaponData_SpaceFood;
BattleWeapon ItemWeaponData_IceCandy;
BattleWeapon ItemWeaponData_NancyFrappe;
BattleWeapon ItemWeaponData_SnowRabbit;
BattleWeapon ItemWeaponData_TeaKinoko;
BattleWeapon ItemWeaponData_FirstLovePuddingBiribiri;
BattleWeapon ItemWeaponData_FirstLovePuddingTrans;
BattleWeapon ItemWeaponData_FirstLovePuddingSleep;
BattleWeapon ItemWeaponData_StarryDinner;
BattleWeapon ItemWeaponData_KararinaPasta;
BattleWeapon ItemWeaponData_MeromeroCake;
BattleWeapon ItemWeaponData_PeachTaltBiribiri;
BattleWeapon ItemWeaponData_PeachTaltDodge;
BattleWeapon ItemWeaponData_PeachTaltSleep;
BattleWeapon ItemWeaponData_BiribiriCandy;
BattleWeapon ItemWeaponData_HealthySalad;
BattleWeapon ItemWeaponData_FreshJuice;
BattleWeapon ItemWeaponData_RedKararing;
BattleWeapon ItemWeaponData_FutarideForever;
static EvtWord ItemEvent_ThrowAttack[432];
BattleWeapon ItemWeaponData_NancyDynamite;
BattleWeapon ItemWeaponData_CoconutsBomb;
BattleWeapon ItemWeaponData_KachikachiDish;
BattleWeapon ItemWeaponData_BomberEgg;
static EvtWord ItemEvent_Poison_Kinoko[86];
BattleWeapon ItemWeaponData_PoisonKinoko;

#if VERSION <= VERSION_DEMO_JP
extern EvtWord ItemEvent_LastDinner[];
#else
EvtWord ItemEvent_LastDinner[228];
#endif

BattleWeapon ItemWeaponData_LastDinner;
static EvtWord ItemEvent_Kaminari_Dokkan[7];
static EvtWord ItemEvent_Kaminari_Gorogoro[7];
static EvtWord ItemEvent_Madowaseno_Kona[7];
static EvtWord ItemEvent_Dekadeka_Drink[7];
static EvtWord ItemEvent_Katikati_Koura[7];
static EvtWord ItemEvent_Minimini_Kun[7];
static EvtWord ItemEvent_Funyafunya_Kun[7];
static EvtWord ItemEvent_Sukkiri_Drink[7];
static EvtWord ItemEvent_Shikaeshino_Kona[7];
static EvtWord ItemEvent_Kameno_Noroi[7];

/**
 * Walks the item user back to its home position (if it moved), at walking speed;
 * the common ending of the item scripts.
 */
static EVT_BEGIN(_return_home_event)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    LABEL(95)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(3), LW(4), LW(5))
    IF_INT_NOT_EQUAL(LW(0), LW(3))
        BEGIN_SIBLING_EVENT
            CALL(btlevtcmd_GetBodyId, UNIT_ID_SELF, LW(0))
            CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(0), POSE_TYPE_RUN)
        END_SIBLING_EVENT
        CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(5.0f))
        CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    END_IF
    RETURN
EVT_END()

/**
 * Picks the target in LW(10)/LW(11) for the item in LW(8):
 * the player's selected target when a player unit uses it,
 * otherwise one chosen from the candidates (enemies using items).
 */
static EVT_BEGIN(ItemEvent_Automatic_Target)
    CALL(btlevtcmd_CheckCommandUnit, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(8))
        CALL(btlevtcmd_GetSelectEnemy, LW(10), LW(11))
    ELSE
        CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(8))
        CALL(btlevtcmd_ChoiceSamplingEnemy, LW(8), LW(10), LW(11))
    END_IF
    RETURN
EVT_END()

/**
 * The healing part of a recovery item on target LW(10)/LW(11):
 * reads the item's HP and FP restore from item LW(9),
 * shows the heart and flower effects over the target and restores the HP, then the FP.
 */
static EVT_BEGIN(ItemEvent_Recovery_Core_Effect)
    CALL(evt_btl_camera_set_homing_unit, 0, LW(10), -1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(btlevtcmd_GetItemRecoverParam, LW(9), LW(5), LW(6), LW(7))
    IF_INT_GREATER_OR_EQUAL(LW(5), 1)
        CALL(btlevtcmd_GetPartsPos, LW(10), LW(11), LW(12), LW(13), LW(14))
        CALL(btlevtcmd_GetStatusMg, LW(10), LW(0))
        CALL(btlevtcmd_GetHeight, LW(10), LW(1))
        MULTIPLY_FLOAT(LW(0), LW(1))
        ADD(LW(13), LW(0))
        CALL(evt_eff, 0, STRING("recovery"), 0, LW(12), LW(13), LW(14), LW(5), 0, 0, 0, 0, 0, 0, 0)
        WAIT_FRAMES(120)
        CALL(btlevtcmd_RecoverHp, LW(10), LW(11), LW(5))
        WAIT_FRAMES(60)
    END_IF
    IF_INT_GREATER_OR_EQUAL(LW(6), 1)
        CALL(btlevtcmd_GetPartsPos, LW(10), LW(11), LW(12), LW(13), LW(14))
        CALL(btlevtcmd_GetStatusMg, LW(10), LW(0))
        CALL(btlevtcmd_GetHeight, LW(10), LW(1))
        MULTIPLY_FLOAT(LW(0), LW(1))
        ADD(LW(13), LW(0))
        CALL(evt_eff, 0, STRING("recovery"), 1, LW(12), LW(13), LW(14), LW(6), 0, 0, 0, 0, 0, 0, 0)
        WAIT_FRAMES(120)
        CALL(btlevtcmd_RecoverFp, LW(10), LW(11), LW(6))
        WAIT_FRAMES(60)
    END_IF
    IF_INT_GREATER_OR_EQUAL(LW(7), 1)
    END_IF
    RETURN
EVT_END()

/**
 * Heals the target with ItemEvent_Recovery_Core_Effect, resets the camera,
 * returns the user home and ends both units' turns.
 */
static EVT_BEGIN(ItemEvent_Recovery_Core)
    CALL_CHILD(ItemEvent_Recovery_Core_Effect)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    WAIT_FRAMES(60)
    CALL_CHILD(_return_home_event)
    CALL(btlevtcmd_StartWaitEvent, LW(10))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Recovery items (Mushroom, Super and Ultra Shroom, Life Shroom, Dried Shroom, Honey and Maple Syrup,
 * Jammin' Jelly, the plain cooked dishes and so on):
 * the user steps forward, holds the item up and uses it on the chosen ally,
 * restoring the HP and FP the item table gives.
 */
static EVT_BEGIN(ItemEvent_Recovery)
    CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(8))
    CALL(btlevtcmd_WeaponAftereffect, LW(8))
    CALL(btlevtcmd_CheckCommandUnit, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetSelectEnemy, LW(10), LW(11))
    ELSE
        CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(8))
        CALL(btlevtcmd_ChoiceSamplingEnemy, LW(8), LW(10), LW(11))
    END_IF
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, -1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    WAIT_MS(1000)
    CALL(btlevtcmd_GetFriendBelong, UNIT_ID_SELF, LW(0))
    IF_INT_EQUAL(LW(0), ALLIANCE_PLAYER)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetBodyId, UNIT_ID_SELF, LW(9))
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(9), POSE_TYPE_WALK)
        CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(3.0f))
        CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, -60, LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    END_IF
    CALL(btlevtcmd_GetBodyId, UNIT_ID_SELF, LW(9))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(9), POSE_TYPE_ITEM)
    CALL(btlevtcmd_CommandFlyItem, UNIT_ID_SELF, 60)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(9), POSE_TYPE_ACTION)
    CALL(btlevtcmd_ConsumeItem, UNIT_ID_SELF)
    CALL(btlevtcmd_CommandGetWeaponItemId, LW(9), LW(8))
    IF_INT_EQUAL(LW(9), 0)
        CALL(btlevtcmd_GetConsumeItem, LW(9))
    END_IF
    CALL_CHILD(ItemEvent_Recovery_Core)
    RETURN
EVT_END()

/**
 * The common opening of the support items: the camera closes in, a user on Mario's side steps forward (enemies stay put),
 * holds the item up and it is consumed.
 */
static EVT_BEGIN(ItemEvent_Support_Sub_UseDeclere)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, -1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    WAIT_MS(600)
    CALL(btlevtcmd_GetBodyId, UNIT_ID_SELF, LW(9))
    CALL(btlevtcmd_GetFriendBelong, UNIT_ID_SELF, LW(0))
    IF_INT_EQUAL(LW(0), ALLIANCE_PLAYER)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetBodyId, UNIT_ID_SELF, LW(9))
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(9), POSE_TYPE_WALK)
        CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(3.0f))
        CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, -60, LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    END_IF
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(9), POSE_TYPE_ITEM)
    CALL(btlevtcmd_CommandFlyItem, UNIT_ID_SELF, 40)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(9), POSE_TYPE_ACTION)
    CALL(btlevtcmd_ConsumeItem, UNIT_ID_SELF)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    RETURN
EVT_END()

/**
 * Picks the target in LW(10)/LW(11) for weapon LW(12):
 * the first of all targets for items that hit several, the selected one for player units,
 * otherwise one chosen from the candidates.
 */
static EVT_BEGIN(ItemEvent_GetTarget)
    CALL(btlevtcmd_CheckCommandUnit, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckWeaponPluralTarget, LW(12), LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
            CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(12))
        END_IF
        CALL(btlevtcmd_GetSelectEnemy, LW(10), LW(11))
    ELSE
        CALL(btlevtcmd_CheckWeaponPluralTarget, LW(12), LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
            CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(12))
            CALL(btlevtcmd_GetSelectEnemy, LW(10), LW(11))
        ELSE
            CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
            CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(12))
            CALL(btlevtcmd_ChoiceSamplingEnemy, LW(12), LW(10), LW(11))
        END_IF
    END_IF
    RETURN
EVT_END()

/**
 * Applies weapon LW(12) to each of its targets in turn:
 * missed or dodged targets play their dodge, the rest take the item's damage and status.
 */
static EVT_BEGIN(ItemEvent_Support_Sub_Effect)
    CALL_CHILD(ItemEvent_GetTarget)
    LABEL(10)
    IF_INT_EQUAL(LW(10), -1)
        GOTO(90)
    END_IF
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(10), LW(11), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
    SWITCH(LW(5))
        CASE_EQUAL(PRECHECK_LUCKY)
        CALL(btlevtcmd_StartAvoid, LW(10), DAMAGE_CODE_LUCKY)
        CASE_EQUAL(PRECHECK_DODGY)
        CALL(btlevtcmd_StartAvoid, LW(10), DAMAGE_CODE_DODGY)
        CASE_EQUAL(PRECHECK_MISS)
        CALL(btlevtcmd_StartAvoid, LW(10), DAMAGE_CODE_MISS)
        CASE_EQUAL(PRECHECK_HIT)
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(10), LW(11), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
    END_SWITCH
    CALL(btlevtcmd_CheckWeaponPluralTarget, LW(12), LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetSelectNextEnemy, LW(10), LW(11))
    ELSE
        SET_VALUE(LW(10), -1)
    END_IF
    GOTO(10)
    LABEL(90)
    RETURN
EVT_END()

/**
 * Support items with no effect of their own (Power Punch, Courage Shell, Mini Mr. Mini, Mr. Softener,
 * Ruin Powder, Spite Pouch, Koopa Curse): use, apply weapon LW(12) to its targets, audience boost, return home.
 */
static EVT_BEGIN(ItemEvent_Support_NoEffect)
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL_CHILD(ItemEvent_Support_Sub_UseDeclere)
    WAIT_FRAMES(30)
    CALL_CHILD(ItemEvent_Support_Sub_Effect)
    CALL(evt_audience_ap_recovery)
    WAIT_FRAMES(150)
    CALL_CHILD(_return_home_event)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Status-curing support items (Tasty Tonic): like ItemEvent_Support_NoEffect,
 * with the cleansing sound on each target hit.
 */
static EVT_BEGIN(ItemEvent_Support_RefreshEffect)
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL_CHILD(ItemEvent_Support_Sub_UseDeclere)
    WAIT_FRAMES(30)
    CALL_CHILD(ItemEvent_GetTarget)
    LABEL(10)
    IF_INT_EQUAL(LW(10), -1)
        GOTO(90)
    END_IF
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(10), LW(11), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
    SWITCH(LW(5))
        CASE_EQUAL(PRECHECK_LUCKY)
        CALL(btlevtcmd_StartAvoid, LW(10), DAMAGE_CODE_LUCKY)
        CASE_EQUAL(PRECHECK_DODGY)
        CALL(btlevtcmd_StartAvoid, LW(10), DAMAGE_CODE_DODGY)
        CASE_EQUAL(PRECHECK_MISS)
        CALL(btlevtcmd_StartAvoid, LW(10), DAMAGE_CODE_MISS)
        CASE_EQUAL(PRECHECK_HIT)
        CALL(btlevtcmd_GetHitPos, LW(10), LW(11), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_REST_CONDITION1"), LW(0), LW(1), LW(2), 0)
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(10), LW(11), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
    END_SWITCH
    CALL(btlevtcmd_CheckWeaponPluralTarget, LW(12), LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetSelectNextEnemy, LW(10), LW(11))
    ELSE
        SET_VALUE(LW(10), -1)
    END_IF
    GOTO(10)
    LABEL(90)
    CALL(evt_audience_ap_recovery)
    WAIT_FRAMES(150)
    CALL_CHILD(_return_home_event)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Stand-in battle data for cooking items that have none of their own (battle_seq_command puts it in the menu):
 * heals one ally via ItemEvent_Recovery.
 */
BattleWeapon ItemWeaponData_CookingItem = {
    NULL, 0, // name..icon
    {0}, // pad_06
    ITEM_NULL, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, 0, 0.0f, 0, 0, 0, 0, // baseFpCost..reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_OPPOSITE_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_SAME_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    ItemEvent_Recovery, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Mushroom: restores 5 HP to one ally (ItemEvent_Recovery). */
BattleWeapon ItemWeaponData_Kinoko = {
    NULL, 0, // name..icon
    {0}, // pad_06
    ITEM_MUSHROOM, // itemId
    NULL, // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_OPPOSITE_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_SAME_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_PIERCE_DEFENSE | SPECIAL_IGNORE_VULNERABILITY | SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_IN_PERIL | TARGET_WEIGHT_LOW_HP_RATIO | TARGET_WEIGHT_OWN_SIDE, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    ItemEvent_Recovery, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Life Shroom: restores 10 HP; also revives Mario or his partner when he falls (ItemEvent_Recovery). */
BattleWeapon ItemWeaponData_Kinkyuu_Kinoko = {
    NULL, 0, // name..icon
    {0}, // pad_06
    ITEM_LIFE_SHROOM, // itemId
    NULL, // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_OPPOSITE_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_SAME_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_PIERCE_DEFENSE | SPECIAL_IGNORE_VULNERABILITY | SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_IN_PERIL | TARGET_WEIGHT_LOW_HP_RATIO | TARGET_WEIGHT_OWN_SIDE, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    ItemEvent_Recovery, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Super Shroom: restores 10 HP to one ally (ItemEvent_Recovery). */
BattleWeapon ItemWeaponData_Super_Kinoko = {
    NULL, 0, // name..icon
    {0}, // pad_06
    ITEM_SUPER_SHROOM, // itemId
    NULL, // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_OPPOSITE_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_SAME_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_PIERCE_DEFENSE | SPECIAL_IGNORE_VULNERABILITY | SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_IN_PERIL | TARGET_WEIGHT_LOW_HP_RATIO | TARGET_WEIGHT_OWN_SIDE, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    ItemEvent_Recovery, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Ultra Shroom: restores 50 HP to one ally (ItemEvent_Recovery). */
BattleWeapon ItemWeaponData_Ultra_Kinoko = {
    NULL, 0, // name..icon
    {0}, // pad_06
    ITEM_ULTRA_SHROOM, // itemId
    NULL, // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_OPPOSITE_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_SAME_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_PIERCE_DEFENSE | SPECIAL_IGNORE_VULNERABILITY | SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_IN_PERIL | TARGET_WEIGHT_LOW_HP_RATIO | TARGET_WEIGHT_OWN_SIDE, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    ItemEvent_Recovery, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Dried Shroom: restores 1 HP to one ally (ItemEvent_Recovery). */
BattleWeapon ItemWeaponData_Sinabita_Kinoko = {
    NULL, 0, // name..icon
    {0}, // pad_06
    ITEM_DRIED_SHROOM, // itemId
    NULL, // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_OPPOSITE_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_SAME_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_PIERCE_DEFENSE | SPECIAL_IGNORE_VULNERABILITY | SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_IN_PERIL | TARGET_WEIGHT_LOW_HP_RATIO | TARGET_WEIGHT_OWN_SIDE, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    ItemEvent_Recovery, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Honey Syrup: restores 5 FP to one ally (ItemEvent_Recovery). */
BattleWeapon ItemWeaponData_Honey_Syrup = {
    NULL, 0, // name..icon
    {0}, // pad_06
    ITEM_HONEY_SYRUP, // itemId
    NULL, // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_OPPOSITE_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_SAME_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_PIERCE_DEFENSE | SPECIAL_IGNORE_VULNERABILITY | SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM | TARGET_WEIGHT_PARTNER, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    ItemEvent_Recovery, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Maple Syrup: restores 10 FP to one ally (ItemEvent_Recovery). */
BattleWeapon ItemWeaponData_Maple_Syrup = {
    NULL, 0, // name..icon
    {0}, // pad_06
    ITEM_MAPLE_SYRUP, // itemId
    NULL, // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_OPPOSITE_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_SAME_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_PIERCE_DEFENSE | SPECIAL_IGNORE_VULNERABILITY | SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM | TARGET_WEIGHT_PARTNER, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    ItemEvent_Recovery, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Jammin' Jelly: restores 50 FP to one ally (ItemEvent_Recovery). */
BattleWeapon ItemWeaponData_Royal_Jelly = {
    NULL, 0, // name..icon
    {0}, // pad_06
    ITEM_JAMMIN_JELLY, // itemId
    NULL, // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_OPPOSITE_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_SAME_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_PIERCE_DEFENSE | SPECIAL_IGNORE_VULNERABILITY | SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM | TARGET_WEIGHT_PARTNER, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    ItemEvent_Recovery, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Whacka Bump: restores 25 HP and 25 FP to one ally (ItemEvent_Recovery). */
BattleWeapon ItemWeaponData_Tankobu = {
    NULL, 0, // name..icon
    {0}, // pad_06
    ITEM_WHACKA_BUMP, // itemId
    NULL, // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_OPPOSITE_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_SAME_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_PIERCE_DEFENSE | SPECIAL_IGNORE_VULNERABILITY | SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM | TARGET_WEIGHT_PARTNER, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    ItemEvent_Recovery, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** POW Block: a POW Block appears above the user, who hits it, and every enemy takes 2 damage as the stage shakes. */
static EVT_BEGIN(ItemEvent_Pow_Block)
    // The camera closes in on the user; on the player's side they first walk forward.
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, -1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    WAIT_MS(1000)
    CALL(btlevtcmd_GetFriendBelong, UNIT_ID_SELF, LW(0))
    IF_INT_EQUAL(LW(0), ALLIANCE_PLAYER)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetBodyId, UNIT_ID_SELF, LW(9))
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(9), POSE_TYPE_WALK)
        CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(3.0f))
        CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, -60, LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    END_IF

    // The user holds up the POW Block, which is used up.
    CALL(btlevtcmd_GetBodyId, UNIT_ID_SELF, LW(9))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(9), POSE_TYPE_ITEM)
    CALL(btlevtcmd_CommandFlyItem, UNIT_ID_SELF, 40)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(9), POSE_TYPE_ACTION)
    CALL(btlevtcmd_ConsumeItem, UNIT_ID_SELF)

    // The POW Block appears above the user's head in a puff of smoke.
    CALL(btlevtcmd_CheckCommandUnit, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetHeight, UNIT_ID_SELF, LW(5))
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 30)
        ADD(LW(1), 16)
        ADD(LW(1), LW(5))
    ELSE
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetHeight, UNIT_ID_SELF, LW(5))
        ADD(LW(1), 16)
        ADD(LW(1), LW(5))
    END_IF
    BEGIN_SIBLING_EVENT
        WAIT_MS(100)
        CALL(evt_mobj_entry, STRING("_BtCIPw"), STRING("MOBJ_PowerBlock"))
        CALL(evt_mobj_set_position, STRING("_BtCIPw"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_ITEM_POW_APPEAR1"), LW(0), LW(1), LW(2), 0)
    END_SIBLING_EVENT
    ADD(LW(1), 10)
    ADD(LW(2), 25)
    CALL(evt_eff, STRING(""), STRING("kemuri_test"), 2, LW(0), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(2050)
        CALL(evt_eff, STRING(""), STRING("kemuri_test"), 2, LW(0), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
    END_INLINE_EVENT

    // The camera pulls back and the user walks under the block.
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(9), POSE_TYPE_ACTION)
    WAIT_FRAMES(5)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(9), POSE_TYPE_RUN)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(3.0f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, EVT_NULLPTR, LW(1), EVT_NULLPTR)
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(9), POSE_TYPE_NEUTRAL)
    WAIT_FRAMES(30)

    // The user jumps, hits the block from below and walks home after landing.
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 20, FLOAT(0.0f), FLOAT(0.8f))
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(9), POSE_TYPE_JUMP)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 14, MOVE_SOUND_FROM_TABLE)
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(9), POSE_TYPE_NEUTRAL)
        WAIT_FRAMES(60)
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(9), POSE_TYPE_RUN)
        CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(3.0f))
        CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(9), POSE_TYPE_NEUTRAL)
    END_SIBLING_EVENT

    // The block bounces and the stage shakes; sleeping audience members wake up.
    BEGIN_INLINE_EVENT
        WAIT_FRAMES(5)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_MOBJ_POW_BLOCK1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_mobj_set_anim, STRING("_BtCIPw"), STRING("A_1"))
        WAIT_FRAMES(3)
        CALL(evt_btl_camera_shake_h, 0, 8, 0, 10, 13)
        WAIT_FRAMES(50)
        CALL(evt_mobj_delete, STRING("_BtCIPw"))
    END_INLINE_EVENT
    CALL(evt_audience_reflesh_natural_all, 100)
    WAIT_FRAMES(7)

    // Every enemy takes the damage unless it dodges (a miss, a lucky dodge or Dodgy).
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&ItemWeaponData_Pow_Block))
    CALL(btlevtcmd_GetSelectEnemy, LW(10), LW(11))
    LABEL(10)
    IF_INT_EQUAL(LW(10), -1)
        GOTO(90)
    END_IF
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(10), LW(11), PTR(&ItemWeaponData_Pow_Block), DAMAGE_RESULT_FINAL_HIT, LW(5))
    SWITCH(LW(5))
        CASE_EQUAL(PRECHECK_LUCKY)
        CALL(btlevtcmd_StartAvoid, LW(10), DAMAGE_CODE_LUCKY)
        CASE_EQUAL(PRECHECK_DODGY)
        CALL(btlevtcmd_StartAvoid, LW(10), DAMAGE_CODE_DODGY)
        CASE_EQUAL(PRECHECK_MISS)
        CALL(btlevtcmd_StartAvoid, LW(10), DAMAGE_CODE_MISS)
        CASE_EQUAL(PRECHECK_HIT)
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(10), LW(11), PTR(&ItemWeaponData_Pow_Block), DAMAGE_RESULT_FINAL_HIT, LW(5))
    END_SWITCH
    CALL(btlevtcmd_GetSelectNextEnemy, LW(10), LW(11))
    GOTO(10)
    LABEL(90)

    // The user returns home.
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_WeaponAftereffect, PTR(&ItemWeaponData_Pow_Block))
    WAIT_FRAMES(150)
    CALL_CHILD(_return_home_event)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** POW Block: damages every enemy on the ground (ItemEvent_Pow_Block). */
BattleWeapon ItemWeaponData_Pow_Block = {
    NULL, 0, // name..icon
    {0}, // pad_06
    ITEM_POW_BLOCK, // itemId
    NULL, // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {2, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_PIERCE_DEFENSE | SPECIAL_FLIP_SHELLED | SPECIAL_FLIP_BOMB_FLIPPABLE | SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    ItemEvent_Pow_Block, // attackEvtScript
    {0, 100, 100, 100, 75, 10, 5, 5, 5, 5, 0, 0}, // stageEffectRates
};

/** Fire Flower's attack: the fire effect rains on the enemies with its sound, then each enemy takes 3 fire damage. */
static EVT_BEGIN(ItemEvent_Fire_Flower_Core)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    WAIT_MS(100)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 40)
    SET_VALUE(LW(3), LW(0))
    SET_VALUE(LW(4), LW(1))
    SET_VALUE(LW(5), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(3), 240)
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    IF_INT_LESS(LW(0), LW(3))
        CALL(evt_eff64, STRING(""), STRING("fireflower_n64"), 0, LW(0), LW(1), LW(2), 180, 0, 0, 0, 0, 0, 0, 0)
        BEGIN_SIBLING_EVENT
            WAIT_FRAMES(45)
            LOOP(36)
                CALL(evt_snd_sfxon_3d, STRING("SFX_ITEM_FIRE_FLOWER1"), LW(0), LW(1), LW(2), 0)
                WAIT_FRAMES(5)
            END_LOOP
        END_SIBLING_EVENT
    ELSE
        CALL(evt_eff64, STRING(""), STRING("fireflower_n64"), 1, LW(0), LW(1), LW(2), 180, 0, 0, 0, 0, 0, 0, 0)
        BEGIN_SIBLING_EVENT
            WAIT_FRAMES(45)
            LOOP(36)
                CALL(evt_snd_sfxon_3d, STRING("SFX_ITEM_FIRE_FLOWER1"), LW(0), LW(1), LW(2), 0)
                WAIT_FRAMES(5)
            END_LOOP
        END_SIBLING_EVENT
    END_IF
    WAIT_MS(2800)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&ItemWeaponData_Fire_Flower))
    CALL(btlevtcmd_GetSelectEnemy, LW(10), LW(11))
    LABEL(10)
    IF_INT_EQUAL(LW(10), -1)
        GOTO(90)
    END_IF
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(10), LW(11), PTR(&ItemWeaponData_Fire_Flower), DAMAGE_RESULT_FINAL_HIT, LW(5))
    SWITCH(LW(5))
        CASE_EQUAL(PRECHECK_LUCKY)
        CALL(btlevtcmd_StartAvoid, LW(10), DAMAGE_CODE_LUCKY)
        CASE_EQUAL(PRECHECK_DODGY)
        CALL(btlevtcmd_StartAvoid, LW(10), DAMAGE_CODE_DODGY)
        CASE_EQUAL(PRECHECK_MISS)
        CALL(btlevtcmd_StartAvoid, LW(10), DAMAGE_CODE_MISS)
        CASE_EQUAL(PRECHECK_HIT)
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(10), LW(11), PTR(&ItemWeaponData_Fire_Flower), DAMAGE_RESULT_FINAL_HIT, LW(5))
    END_SWITCH
    CALL(btlevtcmd_GetSelectNextEnemy, LW(10), LW(11))
    WAIT_MS(80)
    GOTO(10)
    LABEL(90)
    CALL(evt_audience_ap_recovery)
    WAIT_FRAMES(150)
    CALL_CHILD(_return_home_event)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Fire Flower: the use animation, then ItemEvent_Fire_Flower_Core. */
static EVT_BEGIN(ItemEvent_Fire_Flower)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, -1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    SET_VALUE(LW(12), PTR(&ItemWeaponData_Fire_Flower))
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL_CHILD(ItemEvent_Support_Sub_UseDeclere)
    WAIT_FRAMES(30)
    CALL_CHILD(ItemEvent_Fire_Flower_Core)
    RETURN
EVT_END()

/** Fire Flower: fireballs on every enemy, fire damage only (no burn chance) (ItemEvent_Fire_Flower). */
BattleWeapon ItemWeaponData_Fire_Flower = {
    NULL, 0, // name..icon
    {0}, // pad_06
    ITEM_FIRE_FLOWER, // itemId
    NULL, // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {3, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_FIRE, // element
    ATTACKPATTERN_NULL, // damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_PIERCE_DEFENSE | SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    ItemEvent_Fire_Flower, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/**
 * Thunder attack for weapon LW(12): lightning strikes each target (one or all) and deals the weapon's damage,
 * then the user returns home.
 */
static EVT_BEGIN(ItemEvent_Kaminari_Core)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_CheckCommandUnit, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckWeaponPluralTarget, LW(12), LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
            CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(12))
        END_IF
        CALL(btlevtcmd_GetSelectEnemy, LW(10), LW(11))
    ELSE
        CALL(btlevtcmd_CheckWeaponPluralTarget, LW(12), LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
            CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(12))
            CALL(btlevtcmd_GetSelectEnemy, LW(10), LW(11))
        ELSE
            CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
            CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(12))
            CALL(btlevtcmd_ChoiceSamplingEnemy, LW(12), LW(10), LW(11))
        END_IF
    END_IF
    LABEL(10)
    IF_INT_EQUAL(LW(10), -1)
        GOTO(90)
    END_IF
    CALL(evt_sub_random, 2, LW(4))
    ADD(LW(4), 3)
    CALL(btlevtcmd_GetHitPos, LW(10), LW(11), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetWidth, LW(10), LW(5))
    CALL(btlevtcmd_GetHeight, LW(10), LW(6))
    CALL(evt_eff64, STRING(""), STRING("thunder_n64"), LW(4), LW(0), LW(1), LW(2), LW(5), LW(6), 0, 0, 0, 0, 0, 0)
    CALL(evt_snd_sfxon_3d, STRING("SFX_ITEM_ICE_THUNDER_FALL1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_btl_camera_shake_h, 0, 3, 0, 25, 13)
    CALL(evt_sub_rumble_onoff, 0, 0)
    WAIT_MS(83)
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(10), LW(11), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
    SWITCH(LW(5))
        CASE_EQUAL(PRECHECK_LUCKY)
        CALL(btlevtcmd_StartAvoid, LW(10), DAMAGE_CODE_LUCKY)
        CASE_EQUAL(PRECHECK_DODGY)
        CALL(btlevtcmd_StartAvoid, LW(10), DAMAGE_CODE_DODGY)
        CASE_EQUAL(PRECHECK_MISS)
        CALL(btlevtcmd_StartAvoid, LW(10), DAMAGE_CODE_MISS)
        CASE_EQUAL(PRECHECK_HIT)
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(10), LW(11), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
    END_SWITCH
    WAIT_MS(167)
    CALL(evt_sub_rumble_onoff, 1, 0)
    WAIT_MS(167)
    CALL(btlevtcmd_CheckWeaponPluralTarget, LW(12), LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetSelectNextEnemy, LW(10), LW(11))
    ELSE
        SET_VALUE(LW(10), -1)
    END_IF
    GOTO(10)
    LABEL(90)
    CALL(evt_audience_ap_recovery)
    WAIT_FRAMES(30)
    CALL_CHILD(_return_home_event)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** The thunder items' use animation (weapon LW(12)), then ItemEvent_Kaminari_Core. */
static EVT_BEGIN(ItemEvent_Kaminari)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, -1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    WAIT_MS(1000)
    CALL(btlevtcmd_GetFriendBelong, UNIT_ID_SELF, LW(0))
    IF_INT_EQUAL(LW(0), ALLIANCE_PLAYER)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetBodyId, UNIT_ID_SELF, LW(9))
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(9), POSE_TYPE_WALK)
        CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(3.0f))
        CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, -60, LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_GetBodyId, UNIT_ID_SELF, LW(9))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(9), POSE_TYPE_ITEM)
    CALL(btlevtcmd_CommandFlyItem, UNIT_ID_SELF, 40)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(9), POSE_TYPE_ACTION)
    CALL(btlevtcmd_ConsumeItem, UNIT_ID_SELF)
    CALL_CHILD(ItemEvent_Kaminari_Core)
    RETURN
EVT_END()

/** Thunder Rage: 5 lightning damage to every enemy. */
#if VERSION <= VERSION_DEMO_JP
static EVT_BEGIN(ItemEvent_Kaminari_Dokkan)
    SET_VALUE(LW(12), PTR(&ItemWeaponData_Kaminari_Dokkan))
    CALL_CHILD(ItemEvent_Kaminari)
    RETURN
EVT_END()
#endif

/** Thunder Rage: lightning on every enemy (ItemEvent_Kaminari_Dokkan). */
BattleWeapon ItemWeaponData_Kaminari_Dokkan = {
    NULL, 0, // name..icon
    {0}, // pad_06
    ITEM_THUNDER_RAGE, // itemId
    NULL, // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {5, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_ELECTRIC, // element
    ATTACKPATTERN_NULL, // damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_PIERCE_DEFENSE | SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    ItemEvent_Kaminari_Dokkan, // attackEvtScript
    {0, 0, 0, 100, 0, 5, 5, 5, 5, 0, 0, 0}, // stageEffectRates
};

/** Thunder Bolt: 5 lightning damage to one enemy. */
#if VERSION <= VERSION_DEMO_JP
static EVT_BEGIN(ItemEvent_Kaminari_Gorogoro)
    SET_VALUE(LW(12), PTR(&ItemWeaponData_Kaminari_Gorogoro))
    CALL_CHILD(ItemEvent_Kaminari)
    RETURN
EVT_END()
#endif

/** Thunder Bolt: lightning on one enemy (ItemEvent_Kaminari_Gorogoro). */
BattleWeapon ItemWeaponData_Kaminari_Gorogoro = {
    NULL, 0, // name..icon
    {0}, // pad_06
    ITEM_THUNDER_BOLT, // itemId
    NULL, // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {5, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_ELECTRIC, // element
    ATTACKPATTERN_NULL, // damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_PIERCE_DEFENSE | SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    ItemEvent_Kaminari_Gorogoro, // attackEvtScript
    {0, 0, 0, 100, 0, 5, 5, 5, 5, 0, 0, 0}, // stageEffectRates
};

/**
 * Shooting Star's attack: stars fall over the enemies one after another (to random targets),
 * then every enemy takes 6 damage.
 */
static EVT_BEGIN(ItemEvent_Kirakira_Otoshi_Core)
    // The camera pulls back and the targets are chosen (a random one if the player did not pick it).
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_CheckCommandUnit, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(LW(15), 0)
        CALL(btlevtcmd_CheckWeaponPluralTarget, LW(12), LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
            CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(12))
            SET_VALUE(LW(15), 1)
        END_IF
        CALL(btlevtcmd_GetSelectEnemy, LW(10), LW(11))
    ELSE
        CALL(btlevtcmd_CheckWeaponPluralTarget, LW(12), LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            SET_VALUE(LW(15), 1)
            CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
            CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(12))
            CALL(btlevtcmd_GetSelectEnemy, LW(10), LW(11))
        ELSE
            SET_VALUE(LW(15), 0)
            CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
            CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(12))
            CALL(btlevtcmd_ChoiceSamplingEnemy, LW(12), LW(10), LW(11))
        END_IF
    END_IF

    // Single target: one star streaks from above the user down onto the target.
    CALL(evt_snd_sfxon, STRING("SFX_ITEM_KIRAKIRA_FALL1"), 0)
    IF_INT_EQUAL(LW(15), 0)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetStageSize, EVT_NULLPTR, LW(1), EVT_NULLPTR)
        CALL(btlevtcmd_GetHitPos, LW(10), LW(11), LW(3), LW(4), LW(5))
        CALL(evt_eff64, STRING(""), STRING("sstar_n64"), 2, LW(0), LW(1), LW(2), LW(3), LW(4), LW(5), FLOAT(10.0f), 0, 0, 0, 0)
        CALL(evt_snd_sfxon_3d, STRING("SFX_ITEM_KIRAKIRA_FALL1"), LW(0), LW(1), LW(2), 0)
        WAIT_FRAMES(50)
    ELSE
        // All enemies: the camera shakes five times while sparks hit random enemies.
        BEGIN_SIBLING_EVENT
            WAIT_MS(500)
            LOOP(5)
                CALL(evt_btl_camera_shake_h, 0, 5, 0, 20, 12)
                WAIT_MS(400)
                WAIT_MS(200)
            END_LOOP
        END_SIBLING_EVENT
        BEGIN_SIBLING_EVENT_GET_ID(LW(15))
            LOOP(5)
                CALL(evt_sub_random, 6, LW(0))
                ADD(LW(0), 27)
                WAIT_FRAMES(LW(0))
                CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
                CALL(_sampling_random_target, LW(12), LW(10), LW(11), LW(0))
                IF_INT_NOT_EQUAL(LW(10), -1)
                    CALL(btlevtcmd_GetHitPos, LW(10), LW(11), LW(0), LW(1), LW(2))
                    CALL(evt_eff, 0, STRING("hit"), 0, LW(0), LW(1), LW(2), FLOAT(1.0f), 3, 0, 0, 0, 0, 0, 0)
                END_IF
            END_LOOP
        END_SIBLING_EVENT
        BEGIN_SIBLING_EVENT
            LOOP(7)
                CALL(evt_snd_sfxon_3d, STRING("SFX_ITEM_KIRAKIRA_FALL1"), LW(0), LW(1), LW(2), 0)
                WAIT_MS(500)
            END_LOOP
        END_SIBLING_EVENT

        // Sixty stars rain down at random spots over the enemies' side of the stage.
        LOOP(60)
            CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_GetStageSize, EVT_NULLPTR, LW(1), EVT_NULLPTR)
            CALL(evt_sub_random, 200, LW(0))
            CALL(evt_sub_random, 320, LW(3))
            CALL(btlevtcmd_GetFriendBelong, LW(10), LW(6))
            IF_INT_EQUAL(LW(6), 0)
                MULTIPLY(LW(3), -1)
                SET_VALUE(LW(4), 0)
            ELSE
                MULTIPLY(LW(0), -1)
                SET_VALUE(LW(4), 0)
            END_IF
            CALL(btlevtcmd_check_battleflag, LW(8), BATTLE_FLAG_PARTY_IN_AUDIENCE)
            IF_INT_EQUAL(LW(8), 0)
                SET_VALUE(LW(2), 0)
                SET_VALUE(LW(5), 0)
            ELSE
                SET_VALUE(LW(2), 200)
                SET_VALUE(LW(5), 200)
            END_IF
            CALL(evt_sub_random, 120, LW(8))
            SUBTRACT(LW(8), 60)
            ADD(LW(5), LW(8))
            CALL(evt_sub_random, 120, LW(8))
            SUBTRACT(LW(8), 60)
            ADD(LW(2), LW(8))
            CALL(btlevtcmd_TransStageFloorPosition, LW(3), LW(4), LW(5))
            CALL(evt_eff64, STRING(""), STRING("sstar_n64"), 2, LW(0), LW(1), LW(2), LW(3), LW(4), LW(5), FLOAT(8.0f), 0, 0, 0, 0)
            WAIT_FRAMES(3)
        END_LOOP

        // After the shower the enemies are picked again for the damage.
        WAIT_FRAMES(50)
        CALL(btlevtcmd_WaitEventEnd, LW(15))
        CALL(btlevtcmd_CheckCommandUnit, UNIT_ID_SELF, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
            CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(12))
            CALL(btlevtcmd_GetSelectEnemy, LW(10), LW(11))
        ELSE
            CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
            CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(12))
            CALL(btlevtcmd_GetSelectEnemy, LW(10), LW(11))
        END_IF
    END_IF

    // Each target takes the damage unless it dodges; the single-target version stops after one.
    LABEL(10)
    IF_INT_EQUAL(LW(10), -1)
        GOTO(90)
    END_IF
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(10), LW(11), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
    SWITCH(LW(5))
        CASE_EQUAL(PRECHECK_LUCKY)
        CALL(btlevtcmd_StartAvoid, LW(10), DAMAGE_CODE_LUCKY)
        CASE_EQUAL(PRECHECK_DODGY)
        CALL(btlevtcmd_StartAvoid, LW(10), DAMAGE_CODE_DODGY)
        CASE_EQUAL(PRECHECK_MISS)
        CALL(btlevtcmd_StartAvoid, LW(10), DAMAGE_CODE_MISS)
        CASE_EQUAL(PRECHECK_HIT)
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(10), LW(11), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
    END_SWITCH
    CALL(btlevtcmd_CheckWeaponPluralTarget, LW(12), LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetSelectNextEnemy, LW(10), LW(11))
    ELSE
        SET_VALUE(LW(10), -1)
    END_IF
    WAIT_MS(80)
    GOTO(10)
    LABEL(90)

    // The user returns home.
    CALL(evt_audience_ap_recovery)
    WAIT_FRAMES(150)
    CALL_CHILD(_return_home_event)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Shooting Star: the use animation, then ItemEvent_Kirakira_Otoshi_Core. */
static EVT_BEGIN(ItemEvent_Kirakira_Otoshi)
    SET_VALUE(LW(12), PTR(&ItemWeaponData_Kirakira_Otoshi))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, -1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL_CHILD(ItemEvent_Support_Sub_UseDeclere)
    CALL_CHILD(ItemEvent_Kirakira_Otoshi_Core)
    RETURN
EVT_END()

/** Shooting Star: falling stars on every enemy (ItemEvent_Kirakira_Otoshi). */
BattleWeapon ItemWeaponData_Kirakira_Otoshi = {
    NULL, 0, // name..icon
    {0}, // pad_06
    ITEM_SHOOTING_STAR, // itemId
    NULL, // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {6, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_PIERCE_DEFENSE | SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    ItemEvent_Kirakira_Otoshi, // attackEvtScript
    {0, 10, 10, 100, 10, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Ice Storm: an icy wind blows over the enemies for 3 ice damage each, with a chance to freeze them for two turns. */
static EVT_BEGIN(ItemEvent_Koorino_Ibuki)
    // The camera closes in on the user; on the player's side they first walk forward.
    SET_VALUE(LW(12), PTR(&ItemWeaponData_Koorino_Ibuki))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, -1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    WAIT_MS(1000)
    CALL(btlevtcmd_GetFriendBelong, UNIT_ID_SELF, LW(0))
    IF_INT_EQUAL(LW(0), ALLIANCE_PLAYER)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetBodyId, UNIT_ID_SELF, LW(9))
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(9), POSE_TYPE_WALK)
        CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(3.0f))
        CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, -60, LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    END_IF

    // The user holds up the Ice Storm, which is used up, and the camera pulls back.
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_GetBodyId, UNIT_ID_SELF, LW(9))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(9), POSE_TYPE_ITEM)
    CALL(btlevtcmd_CommandFlyItem, UNIT_ID_SELF, 40)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(9), POSE_TYPE_ACTION)
    CALL(btlevtcmd_ConsumeItem, UNIT_ID_SELF)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    WAIT_FRAMES(30)

    // The targets are chosen (a random one for a single target when the user is not Mario or a partner).
    CALL(btlevtcmd_CheckCommandUnit, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckWeaponPluralTarget, LW(12), LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
            CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(12))
        END_IF
        CALL(btlevtcmd_GetSelectEnemy, LW(10), LW(11))
    ELSE
        CALL(btlevtcmd_CheckWeaponPluralTarget, LW(12), LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
            CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(12))
            CALL(btlevtcmd_GetSelectEnemy, LW(10), LW(11))
        ELSE
            CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
            CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(12))
            CALL(btlevtcmd_ChoiceSamplingEnemy, LW(12), LW(10), LW(11))
        END_IF
    END_IF

    // The icy wind blows from the user towards the enemies and clears any fog from the stage.
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_ITEM_ICE_WIND1"), LW(0), LW(1), LW(2), LW(15))
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(5))
    ADD(LW(1), 10)
    ADD(LW(2), 20)
    IF_INT_GREATER_OR_EQUAL(LW(5), 0)
        CALL(evt_eff, 0, STRING("ibuki"), 0, LW(0), LW(1), LW(2), 180, 0, 0, 0, 0, 0, 0, 0)
    ELSE
        CALL(evt_eff, 0, STRING("ibuki"), 1, LW(0), LW(1), LW(2), 180, 0, 0, 0, 0, 0, 0, 0)
    END_IF
    WAIT_MS(500)
    CALL(btlevtcmd_StageDispellFog)
    WAIT_MS(2000)
    CALL(evt_snd_sfxoff, LW(15))

    // Each target takes the damage unless it dodges (a miss, a lucky dodge or Dodgy).
    LABEL(10)
    IF_INT_EQUAL(LW(10), -1)
        GOTO(90)
    END_IF
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(10), LW(11), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
    SWITCH(LW(5))
        CASE_EQUAL(PRECHECK_LUCKY)
        CALL(btlevtcmd_StartAvoid, LW(10), DAMAGE_CODE_LUCKY)
        CASE_EQUAL(PRECHECK_DODGY)
        CALL(btlevtcmd_StartAvoid, LW(10), DAMAGE_CODE_DODGY)
        CASE_EQUAL(PRECHECK_MISS)
        CALL(btlevtcmd_StartAvoid, LW(10), DAMAGE_CODE_MISS)
        CASE_EQUAL(PRECHECK_HIT)
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(10), LW(11), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
    END_SWITCH
    CALL(btlevtcmd_CheckWeaponPluralTarget, LW(12), LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetSelectNextEnemy, LW(10), LW(11))
    ELSE
        SET_VALUE(LW(10), -1)
    END_IF
    GOTO(10)
    LABEL(90)

    // The user returns home.
    CALL(evt_audience_ap_recovery)
    WAIT_FRAMES(150)
    CALL_CHILD(_return_home_event)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Ice Storm: a cold wind on every enemy, with a chance to freeze (ItemEvent_Koorino_Ibuki). */
BattleWeapon ItemWeaponData_Koorino_Ibuki = {
    NULL, 0, // name..icon
    {0}, // pad_06
    ITEM_ICE_STORM, // itemId
    NULL, // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {3, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_ICE, // element
    ATTACKPATTERN_NULL, // damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_PIERCE_DEFENSE | SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, // burnTime
#if VERSION <= VERSION_DEMO_JP
    100,
#else
    95, // freezeChance
#endif
    2, // freezeTime
    0, 0, 0, 0, 0, 0, 0, 0, // sizeChangeChance..defChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // defChangeStrength..slowChance
    0, 0, 0, 0, 0, 0, 0, 0, // slowTime..hpRegenTime
    0, 0, 0, // hpRegenStrength..fpRegenStrength
    {0}, // pad_AE
    ItemEvent_Koorino_Ibuki, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Earthquake: the stage shakes and dust falls, and every enemy that is not floating takes 5 damage. */
static EVT_BEGIN(ItemEvent_Yurayura_Jishin)
    // The camera closes in on the user; on the player's side they first walk forward.
    SET_VALUE(LW(12), PTR(&ItemWeaponData_Yurayura_Jishin))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, -1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    WAIT_MS(1000)
    CALL(btlevtcmd_GetFriendBelong, UNIT_ID_SELF, LW(0))
    IF_INT_EQUAL(LW(0), ALLIANCE_PLAYER)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetBodyId, UNIT_ID_SELF, LW(9))
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(9), POSE_TYPE_WALK)
        CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(3.0f))
        CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, -60, LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    END_IF

    // The user holds up the Earthquake, which is used up, and the camera pulls back.
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_GetBodyId, UNIT_ID_SELF, LW(9))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(9), POSE_TYPE_ITEM)
    CALL(btlevtcmd_CommandFlyItem, UNIT_ID_SELF, 40)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(9), POSE_TYPE_ACTION)
    CALL(btlevtcmd_ConsumeItem, UNIT_ID_SELF)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    WAIT_FRAMES(30)

    // Dust falls from above while the stage shakes harder and harder.
    BEGIN_SIBLING_EVENT
        CALL(evt_eff64, 0, STRING("hokori_n64"), 0, 0, 300, 0, 150, 0, 0, 0, 0, 0, 0, 0)
        WAIT_MS(600)
        CALL(evt_eff64, 0, STRING("hokori_n64"), 0, 0, 300, 0, 60, 0, 0, 0, 0, 0, 0, 0)
        WAIT_MS(600)
        CALL(evt_eff64, 0, STRING("hokori_n64"), 0, 0, 300, 0, 60, 0, 0, 0, 0, 0, 0, 0)
        WAIT_MS(600)
        CALL(evt_eff64, 0, STRING("hokori_n64"), 0, 0, 300, 0, 60, 0, 0, 0, 0, 0, 0, 0)
    END_SIBLING_EVENT
    CALL(evt_snd_sfxon, STRING("SFX_ITEM_QUAKE1"), LW(10))
    CALL(evt_btl_camera_shake_h, 0, 0, 5, 90, 0)
    WAIT_MS(1500)
    CALL(evt_btl_camera_shake_h, 0, 5, 5, 40, 0)
    WAIT_MS(667)
    CALL(evt_btl_camera_shake_h, 0, 5, 0, 30, 13)
    WAIT_MS(333)
    CALL(evt_snd_sfxoff, LW(10))

    // The targets are chosen (a random one for a single target when the user is not Mario or a partner).
    CALL(btlevtcmd_CheckCommandUnit, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckWeaponPluralTarget, LW(12), LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
            CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(12))
        END_IF
        CALL(btlevtcmd_GetSelectEnemy, LW(10), LW(11))
    ELSE
        CALL(btlevtcmd_CheckWeaponPluralTarget, LW(12), LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
            CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(12))
            CALL(btlevtcmd_GetSelectEnemy, LW(10), LW(11))
        ELSE
            CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
            CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(12))
            CALL(btlevtcmd_ChoiceSamplingEnemy, LW(12), LW(10), LW(11))
        END_IF
    END_IF

    // Each target takes the damage unless it dodges (a miss, a lucky dodge or Dodgy).
    LABEL(10)
    IF_INT_EQUAL(LW(10), -1)
        GOTO(90)
    END_IF
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(10), LW(11), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
    SWITCH(LW(5))
        CASE_EQUAL(PRECHECK_LUCKY)
        CALL(btlevtcmd_StartAvoid, LW(10), DAMAGE_CODE_LUCKY)
        CASE_EQUAL(PRECHECK_DODGY)
        CALL(btlevtcmd_StartAvoid, LW(10), DAMAGE_CODE_DODGY)
        CASE_EQUAL(PRECHECK_MISS)
        CALL(btlevtcmd_StartAvoid, LW(10), DAMAGE_CODE_MISS)
        CASE_EQUAL(PRECHECK_HIT)
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(10), LW(11), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
    END_SWITCH
    CALL(btlevtcmd_CheckWeaponPluralTarget, LW(12), LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetSelectNextEnemy, LW(10), LW(11))
    ELSE
        SET_VALUE(LW(10), -1)
    END_IF
    GOTO(10)
    LABEL(90)

    // The user returns home.
    CALL(evt_audience_ap_recovery)
    WAIT_FRAMES(150)
    CALL_CHILD(_return_home_event)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Earth Quake: damages every enemy on the ground (ItemEvent_Yurayura_Jishin). */
BattleWeapon ItemWeaponData_Yurayura_Jishin = {
    NULL, 0, // name..icon
    {0}, // pad_06
    ITEM_EARTH_QUAKE, // itemId
    NULL, // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {5, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_NO_FLOATING | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_PIERCE_DEFENSE | SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    ItemEvent_Yurayura_Jishin, // attackEvtScript
    {0, 100, 100, 100, 75, 10, 5, 5, 5, 5, 0, 0}, // stageEffectRates
};

/** Boo's Sheet: makes the chosen ally invisible for two turns. */
static EVT_BEGIN(ItemEvent_Teresano_Fuku)
    SET_VALUE(LW(12), PTR(&ItemWeaponData_Teresano_Fuku))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, -1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    WAIT_MS(1000)
    CALL(btlevtcmd_GetFriendBelong, UNIT_ID_SELF, LW(0))
    IF_INT_EQUAL(LW(0), ALLIANCE_PLAYER)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetBodyId, UNIT_ID_SELF, LW(9))
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(9), POSE_TYPE_WALK)
        CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(3.0f))
        CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, -60, LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_GetBodyId, UNIT_ID_SELF, LW(9))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(9), POSE_TYPE_ITEM)
    CALL(btlevtcmd_CommandFlyItem, UNIT_ID_SELF, 40)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(9), POSE_TYPE_ACTION)
    CALL(btlevtcmd_ConsumeItem, UNIT_ID_SELF)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_CheckCommandUnit, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckWeaponPluralTarget, LW(12), LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
            CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(12))
        END_IF
        CALL(btlevtcmd_GetSelectEnemy, LW(10), LW(11))
    ELSE
        CALL(btlevtcmd_CheckWeaponPluralTarget, LW(12), LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
            CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(12))
            CALL(btlevtcmd_GetSelectEnemy, LW(10), LW(11))
        ELSE
            CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
            CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(12))
            CALL(btlevtcmd_ChoiceSamplingEnemy, LW(12), LW(10), LW(11))
        END_IF
    END_IF
    LABEL(10)
    IF_INT_EQUAL(LW(10), -1)
        GOTO(90)
    END_IF
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(10), LW(11), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
    SWITCH(LW(5))
        CASE_EQUAL(PRECHECK_LUCKY)
        CALL(btlevtcmd_StartAvoid, LW(10), DAMAGE_CODE_LUCKY)
        CASE_EQUAL(PRECHECK_DODGY)
        CALL(btlevtcmd_StartAvoid, LW(10), DAMAGE_CODE_DODGY)
        CASE_EQUAL(PRECHECK_MISS)
        CALL(btlevtcmd_StartAvoid, LW(10), DAMAGE_CODE_MISS)
        CASE_EQUAL(PRECHECK_HIT)
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(10), LW(11), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
    END_SWITCH
    CALL(btlevtcmd_CheckWeaponPluralTarget, LW(12), LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetSelectNextEnemy, LW(10), LW(11))
    ELSE
        SET_VALUE(LW(10), -1)
    END_IF
    GOTO(10)
    LABEL(90)
    CALL(evt_audience_ap_recovery)
    WAIT_FRAMES(150)
    CALL_CHILD(_return_home_event)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Boo's Sheet: makes one ally invisible (ItemEvent_Teresano_Fuku). */
BattleWeapon ItemWeaponData_Teresano_Fuku = {
    NULL, 0, // name..icon
    {0}, // pad_06
    ITEM_BOOS_SHEET, // itemId
    NULL, // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_OPPOSITE_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_SAME_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_IGNORE_VULNERABILITY | SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    100, // invisibleChance
    2, // invisibleTime
    0, 0, 0, 0, // hpRegenTime..fpRegenStrength
    {0}, // pad_AE
    ItemEvent_Teresano_Fuku, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Volt Shroom's effect on its target (electrified for five turns), then the user returns home. */
static EVT_BEGIN(ItemEvent_Biribiri_Kinoko_Core)
    CALL_CHILD(ItemEvent_Support_Sub_Effect)
    CALL(evt_audience_ap_recovery)
    WAIT_FRAMES(150)
    CALL_CHILD(_return_home_event)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Volt Shroom: the use animation, then ItemEvent_Biribiri_Kinoko_Core. */
static EVT_BEGIN(ItemEvent_Biribiri_Kinoko)
    SET_VALUE(LW(12), PTR(&ItemWeaponData_Biribiri_Kinoko))
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL_CHILD(ItemEvent_Support_Sub_UseDeclere)
    WAIT_FRAMES(30)
    CALL_CHILD(ItemEvent_Biribiri_Kinoko_Core)
    RETURN
EVT_END()

/** Volt Shroom: electrifies one ally so direct attackers get hurt (ItemEvent_Biribiri_Kinoko). */
BattleWeapon ItemWeaponData_Biribiri_Kinoko = {
    NULL, 0, // name..icon
    {0}, // pad_06
    ITEM_VOLT_SHROOM, // itemId
    NULL, // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_OPPOSITE_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_SAME_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_IGNORE_VULNERABILITY | SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, // poisonStrength..confuseTime
    100, // electricChance
    5, // electricTime
    0, 0, 0, 0, 0, 0, 0, 0, // dodgyChance..sizeChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // sizeChangeStrength..allergicChance
    0, 0, 0, 0, 0, 0, 0, 0, // allergicTime..frightChance
    0, 0, 0, 0, 0, 0, 0, 0, // galeForceChance..fpRegenTime
    0, // fpRegenStrength
    {0}, // pad_AE
    ItemEvent_Biribiri_Kinoko, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Repel Cape's effect on its target (dodgy for four turns), then the user returns home. */
static EVT_BEGIN(ItemEvent_Teki_Yokeeru_Core)
    CALL_CHILD(ItemEvent_Support_Sub_Effect)
    CALL(evt_audience_ap_recovery)
    WAIT_FRAMES(150)
    CALL_CHILD(_return_home_event)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Repel Cape: the use animation, then ItemEvent_Teki_Yokeeru_Core. */
static EVT_BEGIN(ItemEvent_Teki_Yokeeru)
    SET_VALUE(LW(12), PTR(&ItemWeaponData_Teki_Yokeeru))
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL_CHILD(ItemEvent_Support_Sub_UseDeclere)
    WAIT_FRAMES(30)
    CALL_CHILD(ItemEvent_Teki_Yokeeru_Core)
    RETURN
EVT_END()

/** Repel Cape: makes one ally dodgy, harder to hit (ItemEvent_Teki_Yokeeru). */
BattleWeapon ItemWeaponData_Teki_Yokeeru = {
    NULL, 0, // name..icon
    {0}, // pad_06
    ITEM_REPEL_CAPE, // itemId
    NULL, // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_OPPOSITE_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_SAME_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_IGNORE_VULNERABILITY | SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, // poisonStrength..electricTime
    100, // dodgyChance
    4, // dodgyTime
    0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
    0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
    0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
    {0}, // pad_AE
    ItemEvent_Teki_Yokeeru, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Ruin Powder: confuses every enemy for three turns. */
#if VERSION <= VERSION_DEMO_JP
static EVT_BEGIN(ItemEvent_Madowaseno_Kona)
    SET_VALUE(LW(12), PTR(&ItemWeaponData_Madowaseno_Kona))
    CALL_CHILD(ItemEvent_Support_NoEffect)
    RETURN
EVT_END()
#endif

/** Ruin Powder: confuses every enemy (ItemEvent_Madowaseno_Kona). */
BattleWeapon ItemWeaponData_Madowaseno_Kona = {
    NULL, 0, // name..icon
    {0}, // pad_06
    ITEM_RUIN_POWDER, // itemId
    NULL, // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, // poisonStrength
    100, // confuseChance
    3, // confuseTime
    0, 0, 0, 0, 0, 0, 0, 0, // electricChance..freezeTime
    0, 0, 0, 0, 0, 0, 0, 0, // sizeChangeChance..defChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // defChangeStrength..slowChance
    0, 0, 0, 0, 0, 0, 0, 0, // slowTime..hpRegenTime
    0, 0, 0, // hpRegenStrength..fpRegenStrength
    {0}, // pad_AE
    ItemEvent_Madowaseno_Kona, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Sleepy Sheep: a flock of sheep runs across the enemies, putting them to sleep for five turns. */
static EVT_BEGIN(ItemEvent_Nemure_Yoikoyo)
    SET_VALUE(LW(12), PTR(&ItemWeaponData_Nemure_Yoikoyo))
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL_CHILD(ItemEvent_Support_Sub_UseDeclere)
    WAIT_FRAMES(30)
    CALL(evt_btl_camera_shake_h, 0, 1, 0, 10000, 0)
#if VERSION >= VERSION_US
    CALL(btlevtcmd_check_battleflag, LW(0), BATTLE_FLAG_PARTY_IN_AUDIENCE)
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(evt_eff, 0, STRING("sheep"), 0, LW(0), LW(1), 0, 0, 0, 0, 0, 0, 0, 0, 0)
    ELSE
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        ADD(LW(2), 15)
        CALL(evt_eff, 0, STRING("sheep"), 0, LW(0), LW(1), LW(2), 0, 0, 0, 0, 0, 0, 0, 0)
    END_IF
#else
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(evt_eff, 0, STRING("sheep"), 0, LW(0), LW(1), LW(2), 0, 0, 0, 0, 0, 0, 0, 0)
#endif
    WAIT_FRAMES(300)
    CALL(evt_btl_camera_noshake, 0)
    CALL_CHILD(ItemEvent_Support_Sub_Effect)
    CALL(evt_audience_ap_recovery)
    WAIT_FRAMES(150)
    CALL_CHILD(_return_home_event)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Sleepy Sheep: puts every enemy to sleep (ItemEvent_Nemure_Yoikoyo). */
BattleWeapon ItemWeaponData_Nemure_Yoikoyo = {
    NULL, 0, // name..icon
    {0}, // pad_06
    ITEM_SLEEPY_SHEEP, // itemId
    NULL, // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
#if VERSION <= VERSION_DEMO_JP
    100,
#else
    95, // sleepChance
#endif
    5, // sleepTime
    0, 0, 0, 0, 0, 0, 0, 0, // stopChance..confuseChance
    0, 0, 0, 0, 0, 0, 0, 0, // confuseTime..freezeChance
    0, 0, 0, 0, 0, 0, 0, 0, // freezeTime..defChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // defChangeTime..fastTime
    0, 0, 0, 0, 0, 0, 0, 0, // slowChance..invisibleTime
    0, 0, 0, 0, // hpRegenTime..fpRegenStrength
    {0}, // pad_AE
    ItemEvent_Nemure_Yoikoyo, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Stopwatch's effect: the clock effect over the stage, then every enemy is stopped for three turns. */
static EVT_BEGIN(ItemEvent_Stop_Watch_Core)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    WAIT_FRAMES(30)
    CALL(evt_snd_sfxon, STRING("SFX_ITEM_STOP_WATCH1"), 0)
    CALL(btlevtcmd_check_battleflag, LW(0), BATTLE_FLAG_PARTY_IN_AUDIENCE)
    IF_INT_EQUAL(LW(0), 0)
#if VERSION <= VERSION_DEMO_JP
        CALL(evt_eff64, 0, STRING("timestamp_n64"), 0, 0, 0, 0, FLOAT(1.0f), 200, 0, 0, 0, 0, 0, 0)
#else
        CALL(evt_eff64, 0, STRING("timestamp_n64"), 0, 0, 0, 103, FLOAT(1.0f), 200, 0, 0, 0, 0, 0, 0)
#endif
        ELSE
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        ADD(LW(2), 30)
        CALL(evt_eff64, 0, STRING("timestamp_n64"), 0, 0, 0, LW(2), FLOAT(1.0f), 200, 0, 0, 0, 0, 0, 0)
    END_IF
    WAIT_FRAMES(200)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(12))
    CALL(btlevtcmd_GetSelectEnemy, LW(10), LW(11))
    LABEL(10)
    IF_INT_EQUAL(LW(10), -1)
        GOTO(90)
    END_IF
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(10), LW(11), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
    SWITCH(LW(5))
        CASE_EQUAL(PRECHECK_LUCKY)
        CALL(btlevtcmd_StartAvoid, LW(10), DAMAGE_CODE_LUCKY)
        CASE_EQUAL(PRECHECK_DODGY)
        CALL(btlevtcmd_StartAvoid, LW(10), DAMAGE_CODE_DODGY)
        CASE_EQUAL(PRECHECK_MISS)
        CALL(btlevtcmd_StartAvoid, LW(10), DAMAGE_CODE_MISS)
        CASE_EQUAL(PRECHECK_HIT)
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(10), LW(11), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
    END_SWITCH
    CALL(btlevtcmd_GetSelectNextEnemy, LW(10), LW(11))
    GOTO(10)
    LABEL(90)
    CALL(evt_audience_ap_recovery)
    CALL_CHILD(_return_home_event)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Stopwatch: the use animation, then ItemEvent_Stop_Watch_Core. */
static EVT_BEGIN(ItemEvent_Stop_Watch)
    SET_VALUE(LW(12), PTR(&ItemWeaponData_Stop_Watch))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, -1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    WAIT_MS(1000)
    CALL(btlevtcmd_GetFriendBelong, UNIT_ID_SELF, LW(0))
    IF_INT_EQUAL(LW(0), ALLIANCE_PLAYER)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetBodyId, UNIT_ID_SELF, LW(9))
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(9), POSE_TYPE_WALK)
        CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(3.0f))
        CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, -60, LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_GetBodyId, UNIT_ID_SELF, LW(9))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(9), POSE_TYPE_ITEM)
    CALL(btlevtcmd_CommandFlyItem, UNIT_ID_SELF, 40)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(9), POSE_TYPE_ACTION)
    CALL(btlevtcmd_ConsumeItem, UNIT_ID_SELF)
    CALL_CHILD(ItemEvent_Stop_Watch_Core)
    RETURN
EVT_END()

/** Stopwatch: stops every enemy (ItemEvent_Stop_Watch). */
BattleWeapon ItemWeaponData_Stop_Watch = {
    NULL, 0, // name..icon
    {0}, // pad_06
    ITEM_STOPWATCH, // itemId
    NULL, // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, // sleepChance..sleepTime
#if VERSION <= VERSION_DEMO_JP
    100,
#else
    95, // stopChance
#endif
    3, // stopTime
    0, 0, 0, 0, 0, 0, 0, 0, // dizzyChance..electricChance
    0, 0, 0, 0, 0, 0, 0, 0, // electricTime..sizeChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // sizeChangeTime..defChangeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // allergicChance..slowTime
    0, 0, 0, 0, 0, 0, 0, 0, // frightChance..hpRegenStrength
    0, 0, // fpRegenTime..fpRegenStrength
    {0}, // pad_AE
    ItemEvent_Stop_Watch, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Dizzy Dial: the camera rolls around twice (_cam_rot) and every enemy becomes dizzy for three turns. */
static EVT_BEGIN(ItemEvent_Guruguru_Memawashi)
    SET_VALUE(LW(12), PTR(&ItemWeaponData_Guruguru_Memawashi))
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL_CHILD(ItemEvent_Support_Sub_UseDeclere)
    WAIT_FRAMES(30)
    CALL(evt_snd_sfxon, STRING("SFX_ITEM_GURUGURU1"), 0)
    BEGIN_SIBLING_EVENT
        CALL(evt_env_blur_on, 0, 2000)
    END_SIBLING_EVENT
    CALL(_cam_rot)
    CALL_CHILD(ItemEvent_Support_Sub_Effect)
    CALL(evt_audience_ap_recovery)
    WAIT_FRAMES(150)
    CALL_CHILD(_return_home_event)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Dizzy Dial: makes every enemy dizzy (ItemEvent_Guruguru_Memawashi). */
BattleWeapon ItemWeaponData_Guruguru_Memawashi = {
    NULL, 0, // name..icon
    {0}, // pad_06
    ITEM_DIZZY_DIAL, // itemId
    NULL, // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, // sleepChance..stopTime
    100, // dizzyChance
    3, // dizzyTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonChance..dodgyChance
    0, 0, 0, 0, 0, 0, 0, 0, // dodgyTime..sizeChangeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeChance..allergicTime
    0, 0, 0, 0, 0, 0, 0, 0, // ohkoChance..galeForceChance
    0, 0, 0, 0, 0, 0, 0, 0, // paybackTime..fpRegenStrength
    {0}, // pad_AE
    ItemEvent_Guruguru_Memawashi, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Power Punch: makes the chosen ally huge (attack up) for three turns. */
#if VERSION <= VERSION_DEMO_JP
static EVT_BEGIN(ItemEvent_Dekadeka_Drink)
    SET_VALUE(LW(12), PTR(&ItemWeaponData_Dekadeka_Drink))
    CALL_CHILD(ItemEvent_Support_NoEffect)
    RETURN
EVT_END()
#endif

/** Power Punch: makes one ally huge, raising its attack (ItemEvent_Dekadeka_Drink). */
BattleWeapon ItemWeaponData_Dekadeka_Drink = {
    NULL, 0, // name..icon
    {0}, // pad_06
    ITEM_POWER_PUNCH, // itemId
    NULL, // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_OPPOSITE_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_SAME_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_IGNORE_VULNERABILITY | SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, // burnTime..freezeTime
    100, // sizeChangeChance
    3, // sizeChangeTime
    2, // sizeChangeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeChance..allergicTime
    0, 0, 0, 0, 0, 0, 0, 0, // ohkoChance..galeForceChance
    0, 0, 0, 0, 0, 0, 0, 0, // paybackTime..fpRegenStrength
    {0}, // pad_AE
    ItemEvent_Dekadeka_Drink, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Courage Shell: raises the chosen ally's defence by 3 for three turns. */
#if VERSION <= VERSION_DEMO_JP
static EVT_BEGIN(ItemEvent_Katikati_Koura)
    SET_VALUE(LW(12), PTR(&ItemWeaponData_Katikati_Koura))
    CALL_CHILD(ItemEvent_Support_NoEffect)
    RETURN
EVT_END()
#endif

/** Courage Shell: raises one ally's defence (ItemEvent_Katikati_Koura). */
BattleWeapon ItemWeaponData_Katikati_Koura = {
    NULL, 0, // name..icon
    {0}, // pad_06
    ITEM_COURAGE_SHELL, // itemId
    NULL, // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_OPPOSITE_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_SAME_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_IGNORE_VULNERABILITY | SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, // atkChangeStrength
    100, // defChangeChance
    3, // defChangeTime
    3, // defChangeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // allergicChance..slowTime
    0, 0, 0, 0, 0, 0, 0, 0, // frightChance..hpRegenStrength
    0, 0, // fpRegenTime..fpRegenStrength
    {0}, // pad_AE
    ItemEvent_Katikati_Koura, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** HP Drain: deals 5 damage to one enemy and restores as much HP to the user as it dealt. */
static EVT_BEGIN(ItemEvent_Suitooru)
    SET_VALUE(LW(12), PTR(&ItemWeaponData_Suitooru))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, -1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    WAIT_MS(1000)
    CALL(btlevtcmd_GetFriendBelong, UNIT_ID_SELF, LW(0))
    IF_INT_EQUAL(LW(0), ALLIANCE_PLAYER)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetBodyId, UNIT_ID_SELF, LW(9))
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(9), POSE_TYPE_WALK)
        CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(3.0f))
        CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, -60, LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_GetBodyId, UNIT_ID_SELF, LW(9))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(9), POSE_TYPE_ITEM)
    CALL(btlevtcmd_CommandFlyItem, UNIT_ID_SELF, 40)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(9), POSE_TYPE_ACTION)
    CALL(btlevtcmd_ConsumeItem, UNIT_ID_SELF)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_CheckCommandUnit, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckWeaponPluralTarget, LW(12), LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
            CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(12))
        END_IF
        CALL(btlevtcmd_GetSelectEnemy, LW(10), LW(11))
    ELSE
        CALL(btlevtcmd_CheckWeaponPluralTarget, LW(12), LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
            CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(12))
            CALL(btlevtcmd_GetSelectEnemy, LW(10), LW(11))
        ELSE
            CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
            CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(12))
            CALL(btlevtcmd_ChoiceSamplingEnemy, LW(12), LW(10), LW(11))
        END_IF
    END_IF
    LABEL(10)
    IF_INT_EQUAL(LW(10), -1)
        GOTO(90)
    END_IF
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(10), LW(11), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
    SWITCH(LW(5))
        CASE_EQUAL(PRECHECK_LUCKY)
        CALL(btlevtcmd_StartAvoid, LW(10), DAMAGE_CODE_LUCKY)
        CASE_EQUAL(PRECHECK_DODGY)
        CALL(btlevtcmd_StartAvoid, LW(10), DAMAGE_CODE_DODGY)
        CASE_EQUAL(PRECHECK_MISS)
        CALL(btlevtcmd_StartAvoid, LW(10), DAMAGE_CODE_MISS)
        CASE_EQUAL(PRECHECK_HIT)
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(10), LW(11), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
    END_SWITCH
    CALL(btlevtcmd_GetDamage, LW(10), LW(0))
    IF_INT_GREATER_OR_EQUAL(LW(0), 1)
        CALL(btlevtcmd_RecoverHp, UNIT_ID_SELF, LW(9), LW(0))
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(5), LW(6), LW(7))
        CALL(btlevtcmd_GetHeight, UNIT_ID_SELF, LW(8))
        ADD(LW(6), LW(8))
        ADD(LW(7), 3)
        CALL(evt_eff, 0, STRING("recovery"), 0, LW(5), LW(6), LW(7), LW(0), 0, 0, 0, 0, 0, 0, 0)
    END_IF
    CALL(btlevtcmd_CheckWeaponPluralTarget, LW(12), LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetSelectNextEnemy, LW(10), LW(11))
    ELSE
        SET_VALUE(LW(10), -1)
    END_IF
    GOTO(10)
    LABEL(90)
    CALL(evt_audience_ap_recovery)
    WAIT_FRAMES(60)
    CALL_CHILD(_return_home_event)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** HP Drain: damages one enemy and heals the user by the damage dealt (ItemEvent_Suitooru). */
BattleWeapon ItemWeaponData_Suitooru = {
    NULL, 0, // name..icon
    {0}, // pad_06
    ITEM_HP_DRAIN_ITEM, // itemId
    NULL, // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {5, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_PIERCE_DEFENSE | SPECIAL_IGNORE_VULNERABILITY | SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    ItemEvent_Suitooru, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Trade Off: raises the enemies' level by 5 (_rank_up), which raises the Star Points they are worth. */
static EVT_BEGIN(ItemEvent_Teki_Kyouka)
    SET_VALUE(LW(12), PTR(&ItemWeaponData_Teki_Kyouka))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, -1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    WAIT_MS(1000)
    CALL(btlevtcmd_GetFriendBelong, UNIT_ID_SELF, LW(0))
    IF_INT_EQUAL(LW(0), ALLIANCE_PLAYER)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetBodyId, UNIT_ID_SELF, LW(9))
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(9), POSE_TYPE_WALK)
        CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(3.0f))
        CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, -60, LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_GetBodyId, UNIT_ID_SELF, LW(9))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(9), POSE_TYPE_ITEM)
    CALL(btlevtcmd_CommandFlyItem, UNIT_ID_SELF, 40)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(9), POSE_TYPE_ACTION)
    CALL(btlevtcmd_ConsumeItem, UNIT_ID_SELF)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_CheckCommandUnit, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckWeaponPluralTarget, LW(12), LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
            CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(12))
        END_IF
        CALL(btlevtcmd_GetSelectEnemy, LW(10), LW(11))
    ELSE
        CALL(btlevtcmd_CheckWeaponPluralTarget, LW(12), LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
            CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(12))
            CALL(btlevtcmd_GetSelectEnemy, LW(10), LW(11))
        ELSE
            CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
            CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(12))
            CALL(btlevtcmd_ChoiceSamplingEnemy, LW(12), LW(10), LW(11))
        END_IF
    END_IF
    LABEL(10)
    IF_INT_EQUAL(LW(10), -1)
        GOTO(90)
    END_IF
    CALL(btlevtcmd_snd_se, LW(10), STRING("SFX_ITEM_RANK_UP_ENEMY1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetHitPos, LW(10), LW(11), LW(0), LW(1), LW(2))
    ADD(LW(1), 20)
    ADD(LW(2), 5)
    CALL(evt_eff, STRING(""), STRING("rankup"), 0, LW(0), LW(1), LW(2), 0, 0, 0, 0, 0, 0, 0, 0)
    CALL(_rank_up, LW(10))
    CALL(btlevtcmd_CheckWeaponPluralTarget, LW(12), LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetSelectNextEnemy, LW(10), LW(11))
    ELSE
        SET_VALUE(LW(10), -1)
    END_IF
    GOTO(10)
    LABEL(90)
    CALL(evt_audience_ap_recovery)
    WAIT_FRAMES(150)
    CALL_CHILD(_return_home_event)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Trade Off: raises the enemies' level so they are worth more Star Points (ItemEvent_Teki_Kyouka). */
BattleWeapon ItemWeaponData_Teki_Kyouka = {
    NULL, 0, // name..icon
    {0}, // pad_06
    ITEM_TRADE_OFF, // itemId
    NULL, // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_IGNORE_VULNERABILITY | SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    ItemEvent_Teki_Kyouka, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Mini Mr. Mini: shrinks every enemy (attack down) for three turns. */
#if VERSION <= VERSION_DEMO_JP
static EVT_BEGIN(ItemEvent_Minimini_Kun)
    SET_VALUE(LW(12), PTR(&ItemWeaponData_Minimini_Kun))
    CALL_CHILD(ItemEvent_Support_NoEffect)
    RETURN
EVT_END()
#endif

/** Mini Mr. Mini: shrinks every enemy, lowering its attack (ItemEvent_Minimini_Kun). */
BattleWeapon ItemWeaponData_Minimini_Kun = {
    NULL, 0, // name..icon
    {0}, // pad_06
    ITEM_MINI_MR_MINI, // itemId
    NULL, // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, // burnTime..freezeTime
    100, // sizeChangeChance
    3, // sizeChangeTime
    -2, // sizeChangeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeChance..allergicTime
    0, 0, 0, 0, 0, 0, 0, 0, // ohkoChance..galeForceChance
    0, 0, 0, 0, 0, 0, 0, 0, // paybackTime..fpRegenStrength
    {0}, // pad_AE
    ItemEvent_Minimini_Kun, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Mr. Softener: lowers every enemy's defence by 3 for three turns. */
#if VERSION <= VERSION_DEMO_JP
static EVT_BEGIN(ItemEvent_Funyafunya_Kun)
    SET_VALUE(LW(12), PTR(&ItemWeaponData_Funyafunya_Kun))
    CALL_CHILD(ItemEvent_Support_NoEffect)
    RETURN
EVT_END()
#endif

/** Mr. Softener: lowers every enemy's defence (ItemEvent_Funyafunya_Kun). */
BattleWeapon ItemWeaponData_Funyafunya_Kun = {
    NULL, 0, // name..icon
    {0}, // pad_06
    ITEM_MR_SOFTENER, // itemId
    NULL, // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, // atkChangeStrength
    100, // defChangeChance
    3, // defChangeTime
    -3, // defChangeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // allergicChance..slowTime
    0, 0, 0, 0, 0, 0, 0, 0, // frightChance..hpRegenStrength
    0, 0, // fpRegenTime..fpRegenStrength
    {0}, // pad_AE
    ItemEvent_Funyafunya_Kun, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Tasty Tonic: cures the chosen ally's bad status effects. */
#if VERSION <= VERSION_DEMO_JP
static EVT_BEGIN(ItemEvent_Sukkiri_Drink)
    SET_VALUE(LW(12), PTR(&ItemWeaponData_Sukkiri_Drink))
    CALL_CHILD(ItemEvent_Support_RefreshEffect)
    RETURN
EVT_END()
#endif

/** Tasty Tonic: cures one ally's poison and other ailments (ItemEvent_Sukkiri_Drink). */
BattleWeapon ItemWeaponData_Sukkiri_Drink = {
    NULL, 0, // name..icon
    {0}, // pad_06
    ITEM_TASTY_TONIC, // itemId
    NULL, // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_OPPOSITE_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_SAME_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_IGNORE_VULNERABILITY | SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
    100, // sleepChance
    0, // sleepTime
    100, // stopChance
    0, // stopTime
    100, // dizzyChance
    0, // dizzyTime
    100, // poisonChance
    0, 0, // poisonTime..poisonStrength
    100, // confuseChance
    0, 0, 0, 0, 0, // confuseTime..dodgyTime
    100, // burnChance
    0, // burnTime
    100, // freezeChance
    0, // freezeTime
    100, // sizeChangeChance
    0, 0, 0, 0, 0, // sizeChangeTime..atkChangeStrength
    100, // defChangeChance
    0, 0, // defChangeTime..defChangeStrength
    100, // allergicChance
    0, 0, 0, 0, 0, // allergicTime..fastTime
    100, // slowChance
    0, 0, 0, 0, 0, 0, 0, 0, // slowTime..hpRegenTime
    0, 0, 0, // hpRegenStrength..fpRegenStrength
    {0}, // pad_AE
    ItemEvent_Sukkiri_Drink, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Slow Shroom's effect on its target (HP regained over several turns), then the user returns home. */
static EVT_BEGIN(ItemEvent_Jiwajiwa_Kinoko_Core)
    CALL_CHILD(ItemEvent_Support_Sub_Effect)
    CALL(evt_audience_ap_recovery)
    WAIT_FRAMES(150)
    CALL_CHILD(_return_home_event)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Slow Shroom: the use animation, then ItemEvent_Jiwajiwa_Kinoko_Core. */
static EVT_BEGIN(ItemEvent_Jiwajiwa_Kinoko)
    SET_VALUE(LW(12), PTR(&ItemWeaponData_Jiwajiwa_Kinoko))
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL_CHILD(ItemEvent_Support_Sub_UseDeclere)
    WAIT_FRAMES(30)
    CALL_CHILD(ItemEvent_Jiwajiwa_Kinoko_Core)
    RETURN
EVT_END()

/** Slow Shroom: one ally regains HP each turn for a while (ItemEvent_Jiwajiwa_Kinoko). */
BattleWeapon ItemWeaponData_Jiwajiwa_Kinoko = {
    NULL, 0, // name..icon
    {0}, // pad_06
    ITEM_SLOW_SHROOM, // itemId
    NULL, // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_OPPOSITE_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_SAME_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_IGNORE_VULNERABILITY | SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_IN_PERIL | TARGET_WEIGHT_LOW_HP_RATIO | TARGET_WEIGHT_OWN_SIDE, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, // invisibleChance..invisibleTime
    10, // hpRegenTime
    2, // hpRegenStrength
    0, 0, // fpRegenTime..fpRegenStrength
    {0}, // pad_AE
    ItemEvent_Jiwajiwa_Kinoko, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Gradual Syrup's effect on its target (FP regained over several turns), then the user returns home. */
static EVT_BEGIN(ItemEvent_Jiwajiwa_Syrup_Core)
    CALL_CHILD(ItemEvent_Support_Sub_Effect)
    CALL(evt_audience_ap_recovery)
    WAIT_FRAMES(150)
    CALL_CHILD(_return_home_event)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Gradual Syrup: the use animation, then ItemEvent_Jiwajiwa_Syrup_Core. */
static EVT_BEGIN(ItemEvent_Jiwajiwa_Syrup)
    SET_VALUE(LW(12), PTR(&ItemWeaponData_Jiwajiwa_Syrup))
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL_CHILD(ItemEvent_Support_Sub_UseDeclere)
    WAIT_FRAMES(30)
    CALL_CHILD(ItemEvent_Jiwajiwa_Syrup_Core)
    RETURN
EVT_END()

/** Gradual Syrup: one ally regains FP each turn for a while (ItemEvent_Jiwajiwa_Syrup). */
BattleWeapon ItemWeaponData_Jiwajiwa_Syrup = {
    NULL, 0, // name..icon
    {0}, // pad_06
    ITEM_GRADUAL_SYRUP, // itemId
    NULL, // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_OPPOSITE_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_SAME_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_IGNORE_VULNERABILITY | SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, // invisibleChance..hpRegenStrength
    10, // fpRegenTime
    2, // fpRegenStrength
    {0}, // pad_AE
    ItemEvent_Jiwajiwa_Syrup, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Point Swap: swaps the target's HP and FP, blinking the HP and FP in the status window. */
static EVT_BEGIN(ItemEvent_Irekaeeru)
    SET_VALUE(LW(12), PTR(&ItemWeaponData_Irekaeeru))
    CALL_CHILD(ItemEvent_GetTarget)
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL_CHILD(ItemEvent_Support_Sub_UseDeclere)
    WAIT_FRAMES(30)
    CALL(_hpfp_change_declare_1, LW(10))
    CALL(btlevtcmd_GetHitPos, LW(10), LW(11), LW(0), LW(1), LW(2))
    ADD(LW(1), 20)
    ADD(LW(2), 5)
    CALL(evt_eff, STRING(""), STRING("irekae"), 0, LW(0), LW(1), LW(2), 0, 0, 0, 0, 0, 0, 0, 0)
    CALL(btlevtcmd_snd_se, LW(10), STRING("SFX_ITEM_HP_FP_CHANGE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    WAIT_FRAMES(120)
    CALL(btlevtcmd_CheckAttribute, LW(10), kUnitNoHpFpSwap_UNUSED, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetMaxHp, LW(10), LW(5))
        CALL(btlevtcmd_GetHp, LW(10), LW(6))
        CALL(btlevtcmd_GetMaxFp, LW(10), LW(7))
        CALL(btlevtcmd_GetFp, LW(10), LW(8))
        IF_INT_GREATER(LW(8), LW(5))
            SET_VALUE(LW(8), LW(5))
        END_IF
        CALL(btlevtcmd_SetHp, LW(10), LW(8))
        IF_INT_GREATER(LW(6), LW(7))
            SET_VALUE(LW(6), LW(7))
        END_IF
        CALL(btlevtcmd_SetFp, LW(10), LW(6))
        CALL(_hpfp_change_declare_2, LW(10))
        IF_INT_EQUAL(LW(8), 0)
            CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(10), LW(11), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
            SWITCH(LW(5))
                CASE_EQUAL(PRECHECK_LUCKY)
                CALL(btlevtcmd_StartAvoid, LW(10), DAMAGE_CODE_LUCKY)
                CASE_EQUAL(PRECHECK_DODGY)
                CALL(btlevtcmd_StartAvoid, LW(10), DAMAGE_CODE_DODGY)
                CASE_EQUAL(PRECHECK_MISS)
                CALL(btlevtcmd_StartAvoid, LW(10), DAMAGE_CODE_MISS)
                CASE_EQUAL(PRECHECK_HIT)
                CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(10), LW(11), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
            END_SWITCH
        END_IF
    END_IF
    CALL(evt_audience_ap_recovery)
    WAIT_FRAMES(30)
    CALL_CHILD(_return_home_event)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Point Swap: swaps the target's HP and FP (ItemEvent_Irekaeeru). */
BattleWeapon ItemWeaponData_Irekaeeru = {
    NULL, 0, // name..icon
    {0}, // pad_06
    ITEM_POINT_SWAP, // itemId
    NULL, // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_OPPOSITE_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_SAME_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, // element
    ATTACKPATTERN_NO_DAMAGE_FINISH, // damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_IGNORE_VULNERABILITY | SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    ItemEvent_Irekaeeru, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Fright Mask: a scary hologram appears twice and frightens the enemies, which can make them flee. */
EVT_BEGIN(ItemEvent_Attiike_Shisshi)
    SET_VALUE(LW(12), PTR(&ItemWeaponData_Attiike_Shisshi))
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL_CHILD(ItemEvent_Support_Sub_UseDeclere)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ITEM_HORROR1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(60)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ITEM_HORROR2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_SIBLING_EVENT
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
#if VERSION <= VERSION_DEMO_JP
    CALL(evt_eff64, STRING(""), STRING("holography_n64"), 0, LW(0), LW(1), LW(2), FLOAT(1.0f), 120, 0, 0, 0, 0, 0, 0)
    WAIT_FRAMES(120)
#else
    CALL(evt_eff64, 0, STRING("holography_n64"), 0, LW(0), LW(1), LW(2), FLOAT(1.0f), 120, 0, 0, 0, 0, 0, 0)
    BEGIN_SIBLING_EVENT
        WAIT_MS(400)
        CALL(evt_env_blur_on, 0, 1600)
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        WAIT_MS(400)
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
        CALL(evt_btl_camera_set_moveto, 0, 0, 90, 450, 0, 60, 0, 30, 11)
        WAIT_MS(250)
        CALL(evt_btl_camera_set_moveto, 0, 0, 110, 750, 0, 60, 0, 30, 11)
        WAIT_MS(250)
        CALL(evt_btl_camera_set_moveto, 0, 0, 90, 450, 0, 60, 0, 30, 11)
        WAIT_MS(250)
        CALL(evt_btl_camera_set_moveto, 0, 0, 110, 750, 0, 60, 0, 30, 11)
        WAIT_MS(250)
        CALL(evt_btl_camera_set_moveto, 0, 0, 90, 450, 0, 60, 0, 30, 11)
        WAIT_MS(250)
        CALL(evt_btl_camera_set_moveto, 0, 0, 110, 750, 0, 60, 0, 30, 11)
        WAIT_MS(250)
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    END_SIBLING_EVENT
    WAIT_MS(2000)
#endif
    CALL_CHILD(ItemEvent_Support_Sub_Effect)
    CALL(evt_audience_ap_recovery)
#if VERSION <= VERSION_DEMO_JP
    WAIT_FRAMES(150)
#else
    WAIT_MS(2500)
#endif
    CALL_CHILD(_return_home_event)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Fright Mask: scares the enemies, which can make some flee (ItemEvent_Attiike_Shisshi). */
BattleWeapon ItemWeaponData_Attiike_Shisshi = {
    NULL, 0, // name..icon
    {0}, // pad_06
    ITEM_FRIGHT_MASK, // itemId
    NULL, // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, // fastChance..slowTime
    100, // frightChance
    0, 0, 0, 0, 0, 0, 0, 0, // galeForceChance..fpRegenTime
    0, // fpRegenStrength
    {0}, // pad_AE
    ItemEvent_Attiike_Shisshi, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/**
 * Mystery: a roulette picks a random item (_naniga_okorukana_check) and its effect is played:
 * one of the recovery, status or attack item scripts on a fitting target.
 */
static EVT_BEGIN(ItemEvent_Naniga_Okorukana)
    // Mystery is announced, then the roulette spins and lands on an item.
    SET_VALUE(LW(12), PTR(&ItemWeaponData_Naniga_Okorukana))
    CALL_CHILD(ItemEvent_Support_Sub_UseDeclere)
    WAIT_FRAMES(30)
    CALL(_naniga_okorukana_check, UNIT_ID_SELF, LW(13))
    SWITCH(LW(13))
        CASE_EQUAL(ITEM_MUSHROOM)

        // Mushroom, Ultra Shroom, Dried Shroom, Honey Syrup or Jammin' Jelly: it heals an automatic target.
        SET_VALUE(LW(12), PTR(&ItemWeaponData_Kinoko))
        SET_VALUE(LW(8), LW(12))
        CALL_CHILD(ItemEvent_Automatic_Target)
        SET_VALUE(LW(9), ITEM_MUSHROOM)
        CALL_CHILD(ItemEvent_Recovery_Core)
        CASE_EQUAL(ITEM_ULTRA_SHROOM)
        SET_VALUE(LW(12), PTR(&ItemWeaponData_Ultra_Kinoko))
        SET_VALUE(LW(8), LW(12))
        CALL_CHILD(ItemEvent_Automatic_Target)
        SET_VALUE(LW(9), ITEM_ULTRA_SHROOM)
        CALL_CHILD(ItemEvent_Recovery_Core)
        CASE_EQUAL(ITEM_DRIED_SHROOM)
        SET_VALUE(LW(12), PTR(&ItemWeaponData_Sinabita_Kinoko))
        SET_VALUE(LW(8), LW(12))
        CALL_CHILD(ItemEvent_Automatic_Target)
        SET_VALUE(LW(9), ITEM_DRIED_SHROOM)
        CALL_CHILD(ItemEvent_Recovery_Core)
        CASE_EQUAL(ITEM_HONEY_SYRUP)
        SET_VALUE(LW(12), PTR(&ItemWeaponData_Honey_Syrup))
        SET_VALUE(LW(8), LW(12))
        CALL_CHILD(ItemEvent_Automatic_Target)
        SET_VALUE(LW(9), ITEM_HONEY_SYRUP)
        CALL_CHILD(ItemEvent_Recovery_Core)
        CASE_EQUAL(ITEM_JAMMIN_JELLY)
        SET_VALUE(LW(12), PTR(&ItemWeaponData_Royal_Jelly))
        SET_VALUE(LW(8), LW(12))
        CALL_CHILD(ItemEvent_Automatic_Target)
        SET_VALUE(LW(9), ITEM_JAMMIN_JELLY)
        CALL_CHILD(ItemEvent_Recovery_Core)
        CASE_EQUAL(ITEM_VOLT_SHROOM)

        // Volt Shroom, Repel Cape, Slow Shroom or Gradual Syrup: its effect on an automatic target.
        SET_VALUE(LW(12), PTR(&ItemWeaponData_Biribiri_Kinoko))
        SET_VALUE(LW(8), LW(12))
        CALL_CHILD(ItemEvent_Automatic_Target)
        CALL_CHILD(ItemEvent_Biribiri_Kinoko_Core)
        CASE_EQUAL(ITEM_REPEL_CAPE)
        SET_VALUE(LW(12), PTR(&ItemWeaponData_Teki_Yokeeru))
        SET_VALUE(LW(8), LW(12))
        CALL_CHILD(ItemEvent_Automatic_Target)
        CALL_CHILD(ItemEvent_Teki_Yokeeru_Core)
        CASE_EQUAL(ITEM_SLOW_SHROOM)
        SET_VALUE(LW(12), PTR(&ItemWeaponData_Jiwajiwa_Kinoko))
        SET_VALUE(LW(8), LW(12))
        CALL_CHILD(ItemEvent_Automatic_Target)
        CALL_CHILD(ItemEvent_Jiwajiwa_Kinoko_Core)
        CASE_EQUAL(ITEM_GRADUAL_SYRUP)
        SET_VALUE(LW(12), PTR(&ItemWeaponData_Jiwajiwa_Syrup))
        SET_VALUE(LW(8), LW(12))
        CALL_CHILD(ItemEvent_Automatic_Target)
        CALL_CHILD(ItemEvent_Jiwajiwa_Syrup_Core)
        CASE_EQUAL(ITEM_THUNDER_BOLT)

        // Thunder Bolt, Thunder Rage, Fire Flower, Shooting Star or Stopwatch: its attack on the enemies.
        SET_VALUE(LW(12), PTR(&ItemWeaponData_Kaminari_Gorogoro))
        SET_VALUE(LW(8), LW(12))
        CALL_CHILD(ItemEvent_Automatic_Target)
        CALL_CHILD(ItemEvent_Kaminari_Core)
        CASE_EQUAL(ITEM_THUNDER_RAGE)
        SET_VALUE(LW(12), PTR(&ItemWeaponData_Kaminari_Dokkan))
        SET_VALUE(LW(8), LW(12))
        CALL_CHILD(ItemEvent_Automatic_Target)
        CALL_CHILD(ItemEvent_Kaminari_Core)
        CASE_EQUAL(ITEM_FIRE_FLOWER)
        SET_VALUE(LW(12), PTR(&ItemWeaponData_Fire_Flower))
        SET_VALUE(LW(8), LW(12))
        CALL_CHILD(ItemEvent_Automatic_Target)
        CALL_CHILD(ItemEvent_Fire_Flower_Core)
        CASE_EQUAL(ITEM_SHOOTING_STAR)
        SET_VALUE(LW(12), PTR(&ItemWeaponData_Kirakira_Otoshi))
        SET_VALUE(LW(8), LW(12))
        CALL_CHILD(ItemEvent_Automatic_Target)
        CALL_CHILD(ItemEvent_Kirakira_Otoshi_Core)
        CASE_EQUAL(ITEM_STOPWATCH)
        SET_VALUE(LW(12), PTR(&ItemWeaponData_Stop_Watch))
        SET_VALUE(LW(8), LW(12))
        CALL_CHILD(ItemEvent_Automatic_Target)
        CALL_CHILD(ItemEvent_Stop_Watch_Core)
        CASE_DEFAULT
        // Any other result: nothing happens.
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_SWITCH
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Mystery: turns into a random item effect when used (ItemEvent_Naniga_Okorukana). */
BattleWeapon ItemWeaponData_Naniga_Okorukana = {
    NULL, 0, // name..icon
    {0}, // pad_06
    ITEM_MYSTERY, // itemId
    NULL, // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_SAME_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_IGNORE_VULNERABILITY | SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    ItemEvent_Naniga_Okorukana, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

static s32 _item_table[] = {
    ITEM_MUSHROOM,
    ITEM_HONEY_SYRUP,
    ITEM_JAMMIN_JELLY,
    ITEM_VOLT_SHROOM,
    ITEM_REPEL_CAPE,
    ITEM_SLOW_SHROOM,
    ITEM_GRADUAL_SYRUP,
    ITEM_ULTRA_SHROOM,
    ITEM_DRIED_SHROOM,
    ITEM_THUNDER_RAGE,
    ITEM_FIRE_FLOWER,
    ITEM_SHOOTING_STAR,
    ITEM_STOPWATCH,
    ITEM_NULL, // end of list (_naniga_okorukana_check counts up to it)
};

/** Spite Pouch: gives the chosen ally payback for three turns. */
#if VERSION <= VERSION_DEMO_JP
static EVT_BEGIN(ItemEvent_Shikaeshino_Kona)
    SET_VALUE(LW(12), PTR(&ItemWeaponData_Shikaeshino_Kona))
    CALL_CHILD(ItemEvent_Support_NoEffect)
    RETURN
EVT_END()
#endif

/** Spite Pouch: gives one ally payback against direct attacks (ItemEvent_Shikaeshino_Kona). */
BattleWeapon ItemWeaponData_Shikaeshino_Kona = {
    NULL, 0, // name..icon
    {0}, // pad_06
    ITEM_SPITE_POUCH, // itemId
    NULL, // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_OPPOSITE_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_SAME_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_IGNORE_VULNERABILITY | SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, // fastChance..galeForceChance
    3, // paybackTime
    0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
    {0}, // pad_AE
    ItemEvent_Shikaeshino_Kona, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Koopa Curse: slows the chosen enemy for three turns. */
#if VERSION <= VERSION_DEMO_JP
static EVT_BEGIN(ItemEvent_Kameno_Noroi)
    SET_VALUE(LW(12), PTR(&ItemWeaponData_Kameno_Noroi))
    CALL_CHILD(ItemEvent_Support_NoEffect)
    RETURN
EVT_END()
#endif

/** Koopa Curse: slows one enemy (ItemEvent_Kameno_Noroi). */
BattleWeapon ItemWeaponData_Kameno_Noroi = {
    NULL, 0, // name..icon
    {0}, // pad_06
    ITEM_KOOPA_CURSE, // itemId
    NULL, // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, // fastChance..fastTime
    100, // slowChance
    3, // slowTime
    0, 0, 0, 0, 0, 0, 0, 0, // frightChance..hpRegenStrength
    0, 0, // fpRegenTime..fpRegenStrength
    {0}, // pad_AE
    ItemEvent_Kameno_Noroi, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/**
 * Cooked dishes that heal and also apply a status (Space Food, Icicle Pop, Snow Bunny, Love Pudding,
 * Peach Tart and others): heal as ItemEvent_Recovery, then apply the dish's weapon;
 * Love Pudding and Peach Tart pick one of three effects at random (40/40/20%).
 */
static EVT_BEGIN(ItemEvent_RecoveryAndStatus)
    CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(8))
    CALL(btlevtcmd_WeaponAftereffect, LW(8))
    CALL(btlevtcmd_CheckCommandUnit, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetSelectEnemy, LW(10), LW(11))
    ELSE
        CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(8))
        CALL(btlevtcmd_ChoiceSamplingEnemy, LW(8), LW(10), LW(11))
    END_IF
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, -1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    WAIT_MS(1000)
    CALL(btlevtcmd_GetFriendBelong, UNIT_ID_SELF, LW(0))
    IF_INT_EQUAL(LW(0), ALLIANCE_PLAYER)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetBodyId, UNIT_ID_SELF, LW(9))
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(9), POSE_TYPE_WALK)
        CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(3.0f))
        CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, -60, LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    END_IF
    CALL(btlevtcmd_GetBodyId, UNIT_ID_SELF, LW(9))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(9), POSE_TYPE_ITEM)
    CALL(btlevtcmd_CommandFlyItem, UNIT_ID_SELF, 60)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(9), POSE_TYPE_ACTION)
    CALL(btlevtcmd_ConsumeItem, UNIT_ID_SELF)
    CALL(btlevtcmd_CommandGetWeaponItemId, LW(9), LW(8))
    IF_INT_EQUAL(LW(9), 0)
        CALL(btlevtcmd_GetConsumeItem, LW(9))
    END_IF
    CALL_CHILD(ItemEvent_Recovery_Core_Effect)
    SWITCH(LW(9))
        CASE_EQUAL(ITEM_LOVE_PUDDING)
        CALL(evt_sub_random, 100, LW(0))
        IF_INT_LESS(LW(0), 40)
            SET_VALUE(LW(8), PTR(&ItemWeaponData_FirstLovePuddingBiribiri))
        ELSE
            IF_INT_LESS(LW(0), 80)
                SET_VALUE(LW(8), PTR(&ItemWeaponData_FirstLovePuddingTrans))
            ELSE
                SET_VALUE(LW(8), PTR(&ItemWeaponData_FirstLovePuddingSleep))
            END_IF
        END_IF
        CASE_EQUAL(ITEM_PEACH_TART)
        CALL(evt_sub_random, 100, LW(0))
        IF_INT_LESS(LW(0), 40)
            SET_VALUE(LW(8), PTR(&ItemWeaponData_PeachTaltBiribiri))
        ELSE
            IF_INT_LESS(LW(0), 80)
                SET_VALUE(LW(8), PTR(&ItemWeaponData_PeachTaltDodge))
            ELSE
                SET_VALUE(LW(8), PTR(&ItemWeaponData_PeachTaltSleep))
            END_IF
        END_IF
        CASE_OR_EQUAL(ITEM_HEALTHY_SALAD)
        CASE_OR_EQUAL(ITEM_FRESH_JUICE)
        CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_REST_CONDITION1"), LW(0), LW(1), LW(2), 0)
        CASE_END
    END_SWITCH
    SET_VALUE(LW(12), LW(8))
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(10), LW(11), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
    SWITCH(LW(5))
        CASE_EQUAL(PRECHECK_LUCKY)
        CALL(btlevtcmd_StartAvoid, LW(10), DAMAGE_CODE_LUCKY)
        CASE_EQUAL(PRECHECK_DODGY)
        CALL(btlevtcmd_StartAvoid, LW(10), DAMAGE_CODE_DODGY)
        CASE_EQUAL(PRECHECK_MISS)
        CALL(btlevtcmd_StartAvoid, LW(10), DAMAGE_CODE_MISS)
        CASE_EQUAL(PRECHECK_HIT)
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(10), LW(11), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
    END_SWITCH
    CALL_CHILD(_return_home_event)
    CALL(btlevtcmd_StartWaitEvent, LW(10))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Space Food (Zess T. cooking): restores 5 HP (ItemEvent_RecoveryAndStatus). */
BattleWeapon ItemWeaponData_SpaceFood = {
    NULL, 0, // name..icon
    {0}, // pad_06
    ITEM_SPACE_FOOD, // itemId
    NULL, // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_OPPOSITE_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_SAME_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_IGNORE_VULNERABILITY | SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, // atkChangeStrength..defChangeStrength
    50, // allergicChance
    3, // allergicTime
    0, 0, 0, 0, 0, 0, 0, 0, // ohkoChance..galeForceChance
    0, 0, 0, 0, 0, 0, 0, 0, // paybackTime..fpRegenStrength
    {0}, // pad_AE
    ItemEvent_RecoveryAndStatus, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Icicle Pop (Zess T. cooking): restores 15 FP (item table) (ItemEvent_RecoveryAndStatus). */
BattleWeapon ItemWeaponData_IceCandy = {
    NULL, 0, // name..icon
    {0}, // pad_06
    ITEM_ICICLE_POP, // itemId
    NULL, // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_OPPOSITE_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_SAME_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_IGNORE_VULNERABILITY | SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, // burnTime
    20, // freezeChance
    2, // freezeTime
    0, 0, 0, 0, 0, 0, 0, 0, // sizeChangeChance..defChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // defChangeStrength..slowChance
    0, 0, 0, 0, 0, 0, 0, 0, // slowTime..hpRegenTime
    0, 0, 0, // hpRegenStrength..fpRegenStrength
    {0}, // pad_AE
    ItemEvent_RecoveryAndStatus, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Zess Frappe (Zess T. cooking): restores 20 FP (item table) (ItemEvent_RecoveryAndStatus). */
BattleWeapon ItemWeaponData_NancyFrappe = {
    NULL, 0, // name..icon
    {0}, // pad_06
    ITEM_ZESS_FRAPPE, // itemId
    NULL, // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_OPPOSITE_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_SAME_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_IGNORE_VULNERABILITY | SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, // burnTime
    20, // freezeChance
    2, // freezeTime
    0, 0, 0, 0, 0, 0, 0, 0, // sizeChangeChance..defChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // defChangeStrength..slowChance
    0, 0, 0, 0, 0, 0, 0, 0, // slowTime..hpRegenTime
    0, 0, 0, // hpRegenStrength..fpRegenStrength
    {0}, // pad_AE
    ItemEvent_RecoveryAndStatus, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Snow Bunny (Zess T. cooking): restores 15 HP but may freeze the eater (50%) (ItemEvent_RecoveryAndStatus). */
BattleWeapon ItemWeaponData_SnowRabbit = {
    NULL, 0, // name..icon
    {0}, // pad_06
    ITEM_SNOW_BUNNY, // itemId
    NULL, // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_OPPOSITE_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_SAME_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_IGNORE_VULNERABILITY | SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, // burnTime
    50, // freezeChance
    3, // freezeTime
    0, 0, 0, 0, 0, 0, 0, 0, // sizeChangeChance..defChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // defChangeStrength..slowChance
    0, 0, 0, 0, 0, 0, 0, 0, // slowTime..hpRegenTime
    0, 0, 0, // hpRegenStrength..fpRegenStrength
    {0}, // pad_AE
    ItemEvent_RecoveryAndStatus, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Shroom Broth (Zess T. cooking): restores HP gradually (ItemEvent_RecoveryAndStatus). */
BattleWeapon ItemWeaponData_TeaKinoko = {
    NULL, 0, // name..icon
    {0}, // pad_06
    ITEM_SHROOM_BROTH, // itemId
    NULL, // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_OPPOSITE_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_SAME_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_IGNORE_VULNERABILITY | SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, // atkChangeStrength
    100, // defChangeChance
    3, // defChangeTime
    -1, // defChangeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // allergicChance..slowTime
    0, 0, 0, 0, 0, 0, // frightChance..invisibleTime
    5, // hpRegenTime
    2, // hpRegenStrength
    0, 0, // fpRegenTime..fpRegenStrength
    {0}, // pad_AE
    ItemEvent_RecoveryAndStatus, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Love Pudding (Zess T. cooking), electrified outcome: ItemEvent_RecoveryAndStatus picks it 40% of the time. */
BattleWeapon ItemWeaponData_FirstLovePuddingBiribiri = {
    NULL, 0, // name..icon
    {0}, // pad_06
    ITEM_LOVE_PUDDING, // itemId
    NULL, // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_OPPOSITE_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_SAME_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_IGNORE_VULNERABILITY | SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, // poisonStrength..confuseTime
    100, // electricChance
    2, // electricTime
    0, 0, 0, 0, 0, 0, 0, 0, // dodgyChance..sizeChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // sizeChangeStrength..allergicChance
    0, 0, 0, 0, 0, 0, 0, 0, // allergicTime..frightChance
    0, 0, 0, 0, 0, 0, 0, 0, // galeForceChance..fpRegenTime
    0, // fpRegenStrength
    {0}, // pad_AE
    ItemEvent_RecoveryAndStatus, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Love Pudding, invisible outcome: ItemEvent_RecoveryAndStatus picks it 40% of the time. */
BattleWeapon ItemWeaponData_FirstLovePuddingTrans = {
    NULL, 0, // name..icon
    {0}, // pad_06
    ITEM_LOVE_PUDDING, // itemId
    NULL, // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_OPPOSITE_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_SAME_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_IGNORE_VULNERABILITY | SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    100, // invisibleChance
    2, // invisibleTime
    0, 0, 0, 0, // hpRegenTime..fpRegenStrength
    {0}, // pad_AE
    ItemEvent_RecoveryAndStatus, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Love Pudding, sleepy outcome: ItemEvent_RecoveryAndStatus picks it 20% of the time. */
BattleWeapon ItemWeaponData_FirstLovePuddingSleep = {
    NULL, 0, // name..icon
    {0}, // pad_06
    ITEM_LOVE_PUDDING, // itemId
    NULL, // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_OPPOSITE_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_SAME_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_IGNORE_VULNERABILITY | SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
    100, // sleepChance
    2, // sleepTime
    0, 0, 0, 0, 0, 0, 0, 0, // stopChance..confuseChance
    0, 0, 0, 0, 0, 0, 0, 0, // confuseTime..freezeChance
    0, 0, 0, 0, 0, 0, 0, 0, // freezeTime..defChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // defChangeTime..fastTime
    0, 0, 0, 0, 0, 0, 0, 0, // slowChance..invisibleTime
    0, 0, 0, 0, // hpRegenTime..fpRegenStrength
    {0}, // pad_AE
    ItemEvent_RecoveryAndStatus, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Meteor Meal (Zess T. cooking): restores 7 HP, then more over time (ItemEvent_RecoveryAndStatus). */
BattleWeapon ItemWeaponData_StarryDinner = {
    NULL, 0, // name..icon
    {0}, // pad_06
    ITEM_METEOR_MEAL, // itemId
    NULL, // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_OPPOSITE_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_SAME_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_IGNORE_VULNERABILITY | SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, // invisibleChance..invisibleTime
    3, // hpRegenTime
    2, // hpRegenStrength
    0, 0, // fpRegenTime..fpRegenStrength
    {0}, // pad_AE
    ItemEvent_RecoveryAndStatus, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Spicy Pasta (Zess T. cooking): restores 10 HP and 10 FP (ItemEvent_RecoveryAndStatus). */
BattleWeapon ItemWeaponData_KararinaPasta = {
    NULL, 0, // name..icon
    {0}, // pad_06
    ITEM_SPICY_PASTA, // itemId
    NULL, // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_OPPOSITE_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_SAME_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_IGNORE_VULNERABILITY | SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..ohkoChance
    1, // chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    ItemEvent_RecoveryAndStatus, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Heartful Cake (Zess T. cooking): restores 20 FP but softens the eater (ItemEvent_RecoveryAndStatus). */
BattleWeapon ItemWeaponData_MeromeroCake = {
    NULL, 0, // name..icon
    {0}, // pad_06
    ITEM_HEARTFUL_CAKE, // itemId
    NULL, // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_OPPOSITE_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_SAME_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_IGNORE_VULNERABILITY | SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, // atkChangeStrength
    100, // defChangeChance
    3, // defChangeTime
    -1, // defChangeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // allergicChance..slowTime
    0, 0, 0, 0, 0, 0, 0, 0, // frightChance..hpRegenStrength
    0, 0, // fpRegenTime..fpRegenStrength
    {0}, // pad_AE
    ItemEvent_RecoveryAndStatus, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Peach Tart (Zess T. cooking), electrified outcome: ItemEvent_RecoveryAndStatus picks it 40% of the time. */
BattleWeapon ItemWeaponData_PeachTaltBiribiri = {
    NULL, 0, // name..icon
    {0}, // pad_06
    ITEM_PEACH_TART, // itemId
    NULL, // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_OPPOSITE_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_SAME_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_IGNORE_VULNERABILITY | SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, // poisonStrength..confuseTime
    100, // electricChance
    2, // electricTime
    0, 0, 0, 0, 0, 0, 0, 0, // dodgyChance..sizeChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // sizeChangeStrength..allergicChance
    0, 0, 0, 0, 0, 0, 0, 0, // allergicTime..frightChance
    0, 0, 0, 0, 0, 0, 0, 0, // galeForceChance..fpRegenTime
    0, // fpRegenStrength
    {0}, // pad_AE
    ItemEvent_RecoveryAndStatus, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Peach Tart, dodgy outcome: ItemEvent_RecoveryAndStatus picks it 40% of the time. */
BattleWeapon ItemWeaponData_PeachTaltDodge = {
    NULL, 0, // name..icon
    {0}, // pad_06
    ITEM_PEACH_TART, // itemId
    NULL, // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_OPPOSITE_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_SAME_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_IGNORE_VULNERABILITY | SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, // poisonStrength..electricTime
    100, // dodgyChance
    2, // dodgyTime
    0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
    0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
    0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
    {0}, // pad_AE
    ItemEvent_RecoveryAndStatus, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Peach Tart, sleepy outcome: ItemEvent_RecoveryAndStatus picks it 20% of the time. */
BattleWeapon ItemWeaponData_PeachTaltSleep = {
    NULL, 0, // name..icon
    {0}, // pad_06
    ITEM_PEACH_TART, // itemId
    NULL, // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_OPPOSITE_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_SAME_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_IGNORE_VULNERABILITY | SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
    100, // sleepChance
    2, // sleepTime
    0, 0, 0, 0, 0, 0, 0, 0, // stopChance..confuseChance
    0, 0, 0, 0, 0, 0, 0, 0, // confuseTime..freezeChance
    0, 0, 0, 0, 0, 0, 0, 0, // freezeTime..defChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // defChangeTime..fastTime
    0, 0, 0, 0, 0, 0, 0, 0, // slowChance..invisibleTime
    0, 0, 0, 0, // hpRegenTime..fpRegenStrength
    {0}, // pad_AE
    ItemEvent_RecoveryAndStatus, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Electro Pop (Zess T. cooking): restores 15 FP (ItemEvent_RecoveryAndStatus). */
BattleWeapon ItemWeaponData_BiribiriCandy = {
    NULL, 0, // name..icon
    {0}, // pad_06
    ITEM_ELECTRO_POP, // itemId
    NULL, // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_OPPOSITE_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_SAME_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_IGNORE_VULNERABILITY | SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, // poisonStrength..confuseTime
    100, // electricChance
    4, // electricTime
    0, 0, 0, 0, 0, 0, 0, 0, // dodgyChance..sizeChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // sizeChangeStrength..allergicChance
    0, 0, 0, 0, 0, 0, 0, 0, // allergicTime..frightChance
    0, 0, 0, 0, 0, 0, 0, 0, // galeForceChance..fpRegenTime
    0, // fpRegenStrength
    {0}, // pad_AE
    ItemEvent_RecoveryAndStatus, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Healthy Salad (Zess T. cooking): restores 15 FP and cures poison (ItemEvent_RecoveryAndStatus). */
BattleWeapon ItemWeaponData_HealthySalad = {
    NULL, 0, // name..icon
    {0}, // pad_06
    ITEM_HEALTHY_SALAD, // itemId
    NULL, // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_OPPOSITE_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_SAME_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_IGNORE_VULNERABILITY | SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
    100, // sleepChance
    0, // sleepTime
    100, // stopChance
    0, // stopTime
    100, // dizzyChance
    0, // dizzyTime
    100, // poisonChance
    0, 0, // poisonTime..poisonStrength
    100, // confuseChance
    0, 0, 0, 0, 0, // confuseTime..dodgyTime
    100, // burnChance
    0, // burnTime
    100, // freezeChance
    0, // freezeTime
    100, // sizeChangeChance
    0, 0, 0, 0, 0, // sizeChangeTime..atkChangeStrength
    100, // defChangeChance
    0, 0, // defChangeTime..defChangeStrength
    100, // allergicChance
    0, 0, 0, 0, 0, // allergicTime..fastTime
    100, // slowChance
    0, 0, 0, 0, 0, 0, 0, 0, // slowTime..hpRegenTime
    0, 0, 0, // hpRegenStrength..fpRegenStrength
    {0}, // pad_AE
    ItemEvent_RecoveryAndStatus, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Fresh Juice (Zess T. cooking): restores 5 FP and cures poison (ItemEvent_RecoveryAndStatus). */
BattleWeapon ItemWeaponData_FreshJuice = {
    NULL, 0, // name..icon
    {0}, // pad_06
    ITEM_FRESH_JUICE, // itemId
    NULL, // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_OPPOSITE_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_SAME_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_IGNORE_VULNERABILITY | SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
    100, // sleepChance
    0, // sleepTime
    100, // stopChance
    0, // stopTime
    100, // dizzyChance
    0, // dizzyTime
    100, // poisonChance
    0, 0, // poisonTime..poisonStrength
    100, // confuseChance
    0, 0, 0, 0, 0, // confuseTime..dodgyTime
    100, // burnChance
    0, // burnTime
    100, // freezeChance
    0, // freezeTime
    100, // sizeChangeChance
    0, 0, 0, 0, 0, // sizeChangeTime..atkChangeStrength
    100, // defChangeChance
    0, 0, // defChangeTime..defChangeStrength
    100, // allergicChance
    0, 0, 0, 0, 0, // allergicTime..fastTime
    100, // slowChance
    0, 0, 0, 0, 0, 0, 0, 0, // slowTime..hpRegenTime
    0, 0, 0, // hpRegenStrength..fpRegenStrength
    {0}, // pad_AE
    ItemEvent_RecoveryAndStatus, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Hot Sauce (Zess T. cooking): battle data run by ItemEvent_RecoveryAndStatus. */
BattleWeapon ItemWeaponData_RedKararing = {
    NULL, 0, // name..icon
    {0}, // pad_06
    ITEM_HOT_SAUCE, // itemId
    NULL, // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_OPPOSITE_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_SAME_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_IGNORE_VULNERABILITY | SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..ohkoChance
    1, // chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    ItemEvent_RecoveryAndStatus, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Couple's Cake (Zess T. cooking): restores HP slowly (ItemEvent_RecoveryAndStatus). */
BattleWeapon ItemWeaponData_FutarideForever = {
    NULL, 0, // name..icon
    {0}, // pad_06
    ITEM_COUPLES_CAKE, // itemId
    NULL, // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OPPOSITE_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_SAME_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_IGNORE_VULNERABILITY | SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, // invisibleChance..invisibleTime
    10, // hpRegenTime
    2, // hpRegenStrength
    0, 0, // fpRegenTime..fpRegenStrength
    {0}, // pad_AE
    ItemEvent_RecoveryAndStatus, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/**
 * Fire Pop (Zess T. cooking), early demos only: restores 20 FP and deals 1 fire damage to the eater
 * (ItemEvent_RecoveryAndStatus).
 */
#if VERSION <= VERSION_DEMO_JP
BattleWeapon ItemWeaponData_FireCandy = {
    NULL, 0, // name..icon
    {0}, // pad_06
    ITEM_FIRE_POP, // itemId
    NULL, // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {1, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_NO_TREE_OR_SWITCH | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_OPPOSITE_SIDE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_SINGLE, // targetClassFlags
    TARGET_PROP_SAME_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_FIRE, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_PIERCE_DEFENSE | SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    ItemEvent_RecoveryAndStatus, // attackEvtScript
    {0, 0, 0, 100, 0, 5, 5, 5, 5, 0, 0, 0}, // stageEffectRates
};
#endif

/**
 * Thrown cooked dishes (Zess Dynamite, Coconut Bomb, Egg Bomb and others):
 * the dish's icon is thrown in an arc at the target, which takes the dish's damage;
 * Zess Dynamite also shows fire pillars and the cook's ghostly image, the bombs explode.
 */
static EVT_BEGIN(ItemEvent_ThrowAttack)
    // The camera closes in on the user; on the player's side they first walk forward.
    CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(12))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, -1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    WAIT_MS(1000)
    CALL(btlevtcmd_GetFriendBelong, UNIT_ID_SELF, LW(0))
    IF_INT_EQUAL(LW(0), ALLIANCE_PLAYER)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetBodyId, UNIT_ID_SELF, LW(9))
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(9), POSE_TYPE_WALK)
        CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(3.0f))
        CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, -60, LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    END_IF

    // The user holds up the dish, which is used up, and the camera pulls back.
    CALL(btlevtcmd_GetBodyId, UNIT_ID_SELF, LW(9))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(9), POSE_TYPE_ITEM)
    CALL(btlevtcmd_CommandFlyItem, UNIT_ID_SELF, 40)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(9), POSE_TYPE_ACTION)
    CALL(btlevtcmd_CommandGetWeaponItemId, LW(9), LW(12))
    IF_INT_EQUAL(LW(9), 0)
        CALL(btlevtcmd_GetConsumeItem, LW(9))
    END_IF
    CALL(btlevtcmd_ConsumeItem, UNIT_ID_SELF)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_WeaponAftereffect, LW(12))

    // The targets are chosen (a random one for a single target when the user is not Mario or a partner).
    CALL(btlevtcmd_CheckCommandUnit, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckWeaponPluralTarget, LW(12), LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
            CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(12))
        END_IF
        CALL(btlevtcmd_GetSelectEnemy, LW(10), LW(11))
    ELSE
        CALL(btlevtcmd_CheckWeaponPluralTarget, LW(12), LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
            CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(12))
            CALL(btlevtcmd_GetSelectEnemy, LW(10), LW(11))
        ELSE
            CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
            CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(12))
            CALL(btlevtcmd_ChoiceSamplingEnemy, LW(12), LW(10), LW(11))
        END_IF
    END_IF

    // If the user is Mario, he takes a throwing pose.
    CALL(btlevtcmd_GetUnitKind, UNIT_ID_SELF, LW(0))
    IF_INT_EQUAL(LW(0), UNIT_MARIO)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_C_1"))
        BEGIN_SIBLING_EVENT
            WAIT_MS(500)
            CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_NEUTRAL)
        END_SIBLING_EVENT
    END_IF

    // The dish flies in an arc at the target, or at the middle of the enemies' side for an all-enemy dish.
    CALL(btlevtcmd_CheckWeaponPluralTarget, LW(12), LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetHitPos, LW(10), LW(11), LW(5), LW(6), LW(7))
    ELSE
        CALL(btlevtcmd_GetFriendBelong, LW(10), LW(0))
        IF_INT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(5), LW(6), LW(7))
            SET_VALUE(LW(5), -120)
        ELSE
            CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(5), LW(6), LW(7))
            SET_VALUE(LW(5), 100)
        END_IF
        CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    END_IF
    CALL(_get_throw_first_pos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(_throw_item_icon, LW(9), LW(0), LW(1), LW(2), LW(5), LW(6), LW(7))

    // Zess Dynamite raises fire pillars and the cook's ghostly image; Coconut Bomb and Egg Bomb explode.
    SWITCH(LW(9))
        CASE_EQUAL(ITEM_ZESS_DYNAMITE)
        CALL(evt_snd_sfxon_3d, STRING("SFX_BTL_THUNDERS_ATTACK4"), LW(5), LW(6), LW(7), 0)
        SET_VALUE(LW(0), LW(5))
        SET_VALUE(LW(1), LW(6))
        SET_VALUE(LW(2), LW(7))
        ADD(LW(0), 100)
        CALL(evt_eff, 0, STRING("hibashira"), 5, LW(5), LW(6), LW(6), LW(0), LW(1), LW(2), 6, FLOAT(1.4f), 60, 0, 0)
        SUBTRACT(LW(0), 200)
        CALL(evt_eff, 0, STRING("hibashira"), 5, LW(5), LW(6), LW(6), LW(0), LW(1), LW(2), 6, FLOAT(1.4f), 60, 0, 0)
        BEGIN_INLINE_EVENT
            ADD(LW(7), 20)
            CALL(_nancy_mirage, LW(5), LW(6), LW(7))
        END_INLINE_EVENT
        WAIT_MS(500)
        CASE_OR_EQUAL(ITEM_COCONUT_BOMB)
        CASE_OR_EQUAL(ITEM_EGG_BOMB)
        CALL(evt_eff, 0, STRING("bomb"), 0, LW(5), LW(6), LW(7), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
        CALL(evt_snd_sfxon_3d, STRING("SFX_BTL_THUNDERS_ATTACK3"), LW(5), LW(6), LW(7), 0)
        CASE_END
    END_SWITCH

    // Each target takes the damage unless it dodges (a miss, a lucky dodge or Dodgy).
    LABEL(10)
    IF_INT_EQUAL(LW(10), -1)
        GOTO(90)
    END_IF
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(10), LW(11), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
    SWITCH(LW(5))
        CASE_EQUAL(PRECHECK_LUCKY)
        CALL(btlevtcmd_StartAvoid, LW(10), DAMAGE_CODE_LUCKY)
        CASE_EQUAL(PRECHECK_DODGY)
        CALL(btlevtcmd_StartAvoid, LW(10), DAMAGE_CODE_DODGY)
        CASE_EQUAL(PRECHECK_MISS)
        CALL(btlevtcmd_StartAvoid, LW(10), DAMAGE_CODE_MISS)
        CASE_EQUAL(PRECHECK_HIT)
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(10), LW(11), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
    END_SWITCH
    CALL(btlevtcmd_CheckWeaponPluralTarget, LW(12), LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetSelectNextEnemy, LW(10), LW(11))
    ELSE
        SET_VALUE(LW(10), -1)
    END_IF
    GOTO(10)
    LABEL(90)

    // The user returns home.
    CALL(evt_audience_ap_recovery)
    WAIT_FRAMES(30)
    CALL_CHILD(_return_home_event)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Zess Dynamite (Zess T. cooking): thrown at every enemy (ItemEvent_ThrowAttack). */
BattleWeapon ItemWeaponData_NancyDynamite = {
    NULL, 0, // name..icon
    {0}, // pad_06
    ITEM_ZESS_DYNAMITE, // itemId
    NULL, // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {7, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_EXPLOSION, // element
    ATTACKPATTERN_NULL, // damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_PIERCE_DEFENSE | SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    ItemEvent_ThrowAttack, // attackEvtScript
    {0, 0, 0, 100, 0, 5, 5, 5, 5, 0, 0, 0}, // stageEffectRates
};

/** Coconut Bomb (Zess T. cooking): thrown at one enemy (ItemEvent_ThrowAttack). */
BattleWeapon ItemWeaponData_CoconutsBomb = {
    NULL, 0, // name..icon
    {0}, // pad_06
    ITEM_COCONUT_BOMB, // itemId
    NULL, // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {7, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_EXPLOSION, // element
    ATTACKPATTERN_NULL, // damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_PIERCE_DEFENSE | SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    ItemEvent_ThrowAttack, // attackEvtScript
    {0, 0, 0, 100, 0, 5, 5, 5, 5, 0, 0, 0}, // stageEffectRates
};

/** Courage Meal (Zess T. cooking): thrown at one enemy (ItemEvent_ThrowAttack). */
BattleWeapon ItemWeaponData_KachikachiDish = {
    NULL, 0, // name..icon
    {0}, // pad_06
    ITEM_COURAGE_MEAL, // itemId
    NULL, // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {4, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_PIERCE_DEFENSE | SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    ItemEvent_ThrowAttack, // attackEvtScript
    {0, 0, 0, 100, 0, 5, 5, 5, 5, 0, 0, 0}, // stageEffectRates
};

/** Egg Bomb (Zess T. cooking): thrown at one enemy (ItemEvent_ThrowAttack). */
BattleWeapon ItemWeaponData_BomberEgg = {
    NULL, 0, // name..icon
    {0}, // pad_06
    ITEM_EGG_BOMB, // itemId
    NULL, // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {6, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_EXPLOSION, // element
    ATTACKPATTERN_NULL, // damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_PIERCE_DEFENSE | SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    ItemEvent_ThrowAttack, // attackEvtScript
    {0, 0, 0, 100, 0, 5, 5, 5, 5, 0, 0, 0}, // stageEffectRates
};

/** Poison Shroom: 80% of the time the target is poisoned; otherwise the target's HP is fully restored. */
static EVT_BEGIN(ItemEvent_Poison_Kinoko)
    SET_VALUE(LW(12), PTR(&ItemWeaponData_PoisonKinoko))
    CALL_CHILD(ItemEvent_Support_Sub_UseDeclere)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(evt_sub_random, 100, LW(0))
    IF_INT_LESS(LW(0), 80)
        CALL_CHILD(ItemEvent_Support_Sub_Effect)
    ELSE
        CALL_CHILD(ItemEvent_GetTarget)
        CALL(evt_btl_camera_set_homing_unit, 0, LW(10), -1)
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
        CALL(evt_btl_camera_set_zoom, 0, 200)
        CALL(btlevtcmd_GetPartsPos, LW(10), LW(11), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetStatusMg, LW(10), LW(5))
        MULTIPLY_FLOAT(LW(5), 50)
        ADD(LW(1), LW(5))
        CALL(evt_eff, 0, STRING("recovery"), 4, LW(0), LW(1), LW(2), 0, 0, 0, 0, 0, 0, 0, 0)
        WAIT_FRAMES(60)
        CALL(btlevtcmd_RecoverHp, LW(10), LW(11), 999)
    END_IF

    // the original script has one END_IF too many; the interpreter ignores it
    END_IF // unmatched: the IF above is already closed
    CALL(evt_audience_ap_recovery)
    CALL_CHILD(_return_home_event)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Poison Shroom (Zess T. cooking): poisons the eater (ItemEvent_Poison_Kinoko). */
BattleWeapon ItemWeaponData_PoisonKinoko = {
    NULL, 0, // name..icon
    {0}, // pad_06
    ITEM_POISON_SHROOM, // itemId
    NULL, // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    weaponGetPowerHPHalf2, // hpDamageFn
    {7, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_OPPOSITE_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_SAME_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_PIERCE_DEFENSE | SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, // sleepChance..dizzyTime
    100, // poisonChance
    3, // poisonTime
    1, // poisonStrength
    0, 0, 0, 0, 0, 0, 0, 0, // confuseChance..burnTime
    0, 0, 0, 0, 0, 0, 0, 0, // freezeChance..atkChangeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // defChangeChance..fastChance
    0, 0, 0, 0, 0, 0, 0, 0, // fastTime..invisibleChance
    0, 0, 0, 0, 0, // invisibleTime..fpRegenStrength
    {0}, // pad_AE
    ItemEvent_Poison_Kinoko, // attackEvtScript
    {0, 0, 0, 100, 0, 5, 5, 5, 5, 0, 0, 0}, // stageEffectRates
};

// _nancy_mirage keeps x/y/z as three s32 words from userdata3 on (over wInterpRelatedTime)

typedef struct ThrowWork {
    Vec start;    // 0x0
    Vec pos;      // 0xC
    Vec target;   // 0x18
    s32 frames;   // 0x24
    f32 speed;    // 0x28
    f32 gravity;  // 0x2C
    f32 vy;       // 0x30
    u8 pad[0x7C - 0x34]; // 0x34, never accessed (the work block is 0x7C bytes)
} ThrowWork;

/**
 * Picks a random target of weapon args[0] among the units of alliance args[3] (the user's opponents)
 * and returns it in args[1] (unit) and args[2] (part), or -1 and -1 if there is none.
 * Used by Shooting Star's falling stars.
 */
static USER_FUNC(_sampling_random_target) {
    BattleWorkWeaponTargets targets;
    BattleWork* wp;
    BattleWeapon* weapon;
    EvtWord* args;
    s8 belong;
    s32 outUnit;
    s32 outPart;
    BattleWorkWeaponTargets* t;
    s32 idx;
    s32 r;
    wp = _battleWorkPointer;
    args = event->args;
    weapon = (BattleWeapon*)evtGetValue(event, args[0]);
    outUnit = args[1];
    outPart = args[2];
    belong = evtGetValue(event, args[3]);

    t = &targets;
    BattleSamplingEnemy(t, weapon, BattleTransID(event, UNIT_ID_SELF), belong, weapon->targetClassFlags,
                        weapon->targetPropertyFlags, -wp->alliances[belong].attackDirection);
    if (t->numTargets <= 0) {
        evtSetValue(event, outUnit, -1);
        evtSetValue(event, outPart, -1);
    } else {
        r = irand(t->numTargets);
        idx = t->targetIndices[r];
        evtSetValue(event, outUnit, t->targets[idx].unitIdx);
        evtSetValue(event, outPart, t->targets[idx].partIdx);
    }
    return EVT_RETURN_DONE;
}

/** Spins the 3D camera's roll from 720 degrees back to 0 over two seconds, for Dizzy Dial. */
static USER_FUNC(_cam_rot) {
    f32 rot;

    if (isFirstCall) {
        event->userdata[0] = sysMsec2Frame(2000);
        event->userdata[1] = event->userdata[0];
    }
    rot = intplGetValue(EASING_QUADRATIC_OUT, 720.0f, 0.0f, event->userdata[0], event->userdata[1]);
    camGetPtr(CAMERA_3D)->rollAngle = rot;
    event->userdata[0]--;
    if (event->userdata[0] <= 0) {
        camGetPtr(CAMERA_3D)->rollAngle = 0.0f;
        return EVT_RETURN_YIELD;
    }
    return EVT_RETURN_BLOCK;
}

/** Raises unit args[0]'s level by 5 (at most 99), for Trade Off. */
static USER_FUNC(_rank_up) {
    s32 id = evtGetValue(event, event->args[0]);
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, BattleTransID(event, id));

    if (unit) {
        unit->level += 5;
        if (unit->level > 99) {
            unit->level = 99;
        }
    }
    return EVT_RETURN_DONE;
}

/**
 * Opens the battle status window when unit args[0] is Mario or a partner,
 * before Point Swap changes their HP and FP.
 */
static USER_FUNC(_hpfp_change_declare_1) {
    BattleWork* wp = _battleWorkPointer;
    s32 id = evtGetValue(event, event->args[0]);
    BattleUnit* unit = BattleGetUnitPtr(wp, BattleTransID(event, id));

    if (unit) {
        if (unit->currentKind == UNIT_MARIO) {
            BattleStatusWindowSystemOn();
        } else if (unit->currentKind >= TYPE_PARTNER_MIN && unit->currentKind < TYPE_PARTNER_MAX) {
            BattleStatusWindowSystemOn();
        }
    }
    return EVT_RETURN_DONE;
}

/**
 * Blinks the HP and FP readouts for 120 frames when unit args[0] is Mario or a partner, after Point Swap.
 * JP and later clear any earlier blink first.
 */
#if VERSION <= VERSION_DEMO_JP
static USER_FUNC(_hpfp_change_declare_2) {
#else
USER_FUNC(_hpfp_change_declare_2) {
#endif

    BattleWork* wp = _battleWorkPointer;
    s32 id = evtGetValue(event, event->args[0]);
    BattleUnit* unit = BattleGetUnitPtr(wp, BattleTransID(event, id));

    if (unit) {
        if (unit->currentKind == UNIT_MARIO) {
#if VERSION >= VERSION_JP
            statusClearBlink();
#endif
            statusMarioHPBlink(120);
            statusFPBlink(120);
        } else if (unit->currentKind >= TYPE_PARTNER_MIN && unit->currentKind < TYPE_PARTNER_MAX) {
#if VERSION >= VERSION_JP
            statusClearBlink();
#endif
            statusPartyHPBlink(120);
            statusFPBlink(120);
        }
    }
    return EVT_RETURN_DONE;
}

#if VERSION <= VERSION_DEMO_JP
static USER_FUNC(_naniga_okorukana_check) {
#else
USER_FUNC(_naniga_okorukana_check) {
#endif
    Vec pos;
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, args[0]);
    s32 out = args[1];
    BattleUnit* unit;
    s32 item;
    s32 i;
    u8* attackItemCount;
    u8* supportItemCount;

    unit = BattleGetUnitPtr(wp, BattleTransID(event, id));
    BtlUnit_GetPos(unit, &pos.x, &pos.y, &pos.z);
    if (!(unit->attributeFlags & kVeiled)) {
        if (unit->attributeFlags & kCeiling) {
            pos.y -= 5.0f;
        } else {
            pos.y += BtlUnit_GetHeight(unit) + 20;
        }
    }
    if (isFirstCall) {
        for (i = 0; _item_table[i] != 0; i++) {
        }
        item = _item_table[irand(i)];
        event->userdata[0] = (EvtWord)effNanigaEntry(0, pos.x, pos.y, pos.z, _item_table, item);
        effSetName((EffectEntry*)event->userdata[0], "effNaniga");
        evtSetValue(event, out, item);
#if VERSION >= VERSION_JP
        if (unit->currentKind == UNIT_MARIO) {
            attackItemCount = &wp->actRecord.marioAttackItems;
            supportItemCount = &wp->actRecord.marioSupportItems;
        } else if (unit->currentKind >= TYPE_PARTNER_MIN && unit->currentKind < TYPE_PARTNER_MAX) {
            attackItemCount = &wp->actRecord.partnerAttackItems;
            supportItemCount = &wp->actRecord.partnerSupportItems;
        } else {
            goto end;
        }
        switch (item) {
            case ITEM_THUNDER_BOLT:
            case ITEM_THUNDER_RAGE:
            case ITEM_SHOOTING_STAR:
            case ITEM_FIRE_FLOWER:
            case ITEM_STOPWATCH:
                (*attackItemCount)++;
                break;
            default:
                (*supportItemCount)++;
                break;
        }
    }
    end:
#else
    }
#endif
    if (effNameToPtr("effNaniga")) {
        return EVT_RETURN_BLOCK;
    }
    return EVT_RETURN_DONE;
}

/** Returns in args[1..3] where a thrown item starts: just above unit args[0]'s head (just below for ceiling units). */
static USER_FUNC(_get_throw_first_pos) {
    BattleUnit* unit;
    Vec pos;
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, args[0]);
    s32 ox = args[1];
    s32 oy = args[2];
    s32 oz = args[3];

    unit = BattleGetUnitPtr(wp, BattleTransID(event, id));
    BtlUnit_GetPos(unit, &pos.x, &pos.y, &pos.z);
    pos.y += 10.0f;
    if (!(unit->attributeFlags & kVeiled)) {
        if (unit->attributeFlags & kCeiling) {
            pos.y -= 5.0f;
        } else {
            pos.y += unit->sizeMultiplier * BtlUnit_GetHeight(unit);
        }
    }
    evtSetFloat(event, ox, pos.x);
    evtSetFloat(event, oy, pos.y);
    evtSetFloat(event, oz, pos.z);
    return EVT_RETURN_DONE;
}

/**
 * Throws the icon of item args[0] from args[1..3] to args[4..6] in an arc (a few frames longer the farther it
 * goes), with the throwing sound; finishes when it lands.
 */
static USER_FUNC(_throw_item_icon) {
    EvtWord* args = event->args;
    ThrowWork* wk;
    s32 item = evtGetValue(event, args[0]);
    f32 sx = evtGetFloat(event, args[1]);
    f32 sy = evtGetFloat(event, args[2]);
    f32 sz = evtGetFloat(event, args[3]);
    f32 tx = evtGetFloat(event, args[4]);
    f32 ty = evtGetFloat(event, args[5]);
    f32 tz = evtGetFloat(event, args[6]);

    if (isFirstCall) {
        Vec v;
        event->userdatap[0] = BattleAlloc(sizeof(ThrowWork));
        wk = (ThrowWork*)event->userdatap[0];
        wk->target.x = tx;
        wk->target.y = ty;
        wk->target.z = tz;
        v.x = sx;
        v.y = sy;
        v.z = sz;
        wk->pos = v;
        wk->start = v;
        wk->frames = (s32)(distABf(wk->start.x, wk->start.z, wk->target.x, wk->target.z) / 15.0f) + 30;
        wk->gravity = 0.5f;
        wk->vy = (wk->target.y - wk->start.y) / wk->frames + wk->gravity * wk->frames / 2.0f;
        wk->speed = distABf(wk->start.x, wk->start.z, wk->target.x, wk->target.z) / wk->frames;
        event->userdata[1] = item;
        event->userdata[2] = 0;
        event->userdata3 = psndSFXOn_3D("SFX_ITEM_COOKING_THROW1", &v);
    }
    wk = (ThrowWork*)event->userdatap[0];
    if (event->userdata[2]) {
        psndSFXOff(event->userdata3);
        BattleFree(wk);
        return EVT_RETURN_DONE;
    }
    wk->pos.y += wk->vy;
    wk->vy -= wk->gravity;
    if (wk->vy < 0.0f && wk->pos.y < wk->target.y) {
        wk->pos.y = wk->target.y;
    }
    btlMovePos(wk->speed, angleABf(wk->start.x, wk->start.z, wk->target.x, wk->target.z), &wk->pos.x, &wk->pos.z);
    wk->frames--;
    if (wk->frames <= 0) {
        event->userdata[2] = 1;
        wk->pos = wk->target;
    }
    dispEntry(CAMERA_3D, 900.0f, DISP_MODE_ALPHA_TEST, _disp_item_icon, event);
    return EVT_RETURN_BLOCK;
}

/** Draws the thrown item's icon at its current position. */
static void _disp_item_icon(CameraId cameraId, void* param) {
    EventEntry* event = param;
    iconDispGx(&((ThrowWork*)event->userdatap[0])->pos, 0, itemDataTable[event->userdata[1]].icon_id, 1.0f);
}

/**
 * Shows the cook's ghostly image (c_cook) at args[0..2] for 60 frames, fading and growing in the last 30,
 * for Zess Dynamite.
 */
static USER_FUNC(_nancy_mirage) {
    EvtWord* args = event->args;
    f32 x = evtGetFloat(event, args[0]);
    f32 y = evtGetFloat(event, args[1]);
    f32 z = evtGetFloat(event, args[2]);

    if (isFirstCall) {
        event->userdata[0] = 60;
        event->userdata[1] = animPoseEntry("c_cook", 1);
        event->userdata[2] = 0;

        // x/y/z go into three s32 words from userdata3 on (over wInterpRelatedTime), converted to integers
        ((s32*)&event->userdata3)[0] = x;
        ((s32*)&event->userdata3)[1] = y;
        ((s32*)&event->userdata3)[2] = z;
        animPoseSetAnim(event->userdata[1], "A_2", 1);
        animPoseSetEffectAnim(event->userdata[1], "PM_Y_1A", 1);
    }
    event->userdata[0]--;
    if (event->userdata[0] < 0) {
        animPoseSetEffect(event->userdata[1], NULL, 0);
        animPoseRelease(event->userdata[1]);
        return EVT_RETURN_DONE;
    }
    animPoseMain(event->userdata[1]);
    dispEntry(CAMERA_3D, z, DISP_MODE_XLU, _nancy_disp, event);
    return EVT_RETURN_BLOCK;
}

/** Draws the cook's image for _nancy_mirage, scaled up and translucent as it fades. */
static void _nancy_disp(CameraId cameraId, void* param) {
    EventEntry* event = param;
    Mtx mtx;
    Mtx scale;
    Mtx trans;
    f32 s;
    u8 alpha;
    s32 n;

    if (event->userdata[0] < 30) {
        n = 30 - event->userdata[0];
        s = 4.0f + 0.05f * n;
        alpha = 128 - n * 4;
    } else {
        s = 4.0f;
        alpha = 128;
    }
    PSMTXTrans(trans, ((s32*)&event->userdata3)[0], ((s32*)&event->userdata3)[1], ((s32*)&event->userdata3)[2]);
    PSMTXScale(scale, s, s, 1.0f);
    PSMTXIdentity(mtx);
    PSMTXConcat(scale, mtx, mtx);
    PSMTXConcat(trans, mtx, mtx);
    animPoseSetMaterialEvtColor(event->userdata[1], (GXColor){0xFF, 0xFF, 0xFF, alpha});
    animPoseSetMaterialFlagOn(event->userdata[1], MAP_OBJ_FLAG_COLOR);
    animPoseDrawMtx(event->userdata[1], mtx, 1, 0.0f, 2.0f);
    animPoseDrawMtx(event->userdata[1], mtx, 2, 0.0f, 2.0f);
    animPoseDrawMtx(event->userdata[1], mtx, 3, 0.0f, 2.0f);
}

/** Fills the party's Star Power when unit args[0] is on the player's side, for Trial Stew. */
static USER_FUNC(_full_ap_recover) {
    BattleWork* wp = _battleWorkPointer;
    s32 id = evtGetValue(event, event->args[0]);
    BattleUnit* unit = BattleGetUnitPtr(wp, BattleTransID(event, id));

    if (BtlUnit_GetBelong(unit) == ALLIANCE_PLAYER) {
        pouchSetAP(pouchGetMaxAP());
    }
    return EVT_RETURN_DONE;
}

/**
 * Trial Stew: a dark cloud over the target, then its HP drops to 1,
 * its FP to 0 and the party's Star Power fills;
 * the early demos play the cooking sound once, JP and later after each hit.
 */
EVT_BEGIN(ItemEvent_LastDinner)
    SET_VALUE(LW(12), PTR(&ItemWeaponData_PoisonKinoko))
    CALL_CHILD(ItemEvent_Support_Sub_UseDeclere)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL_CHILD(ItemEvent_GetTarget)
    CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHeight, LW(10), LW(5))
    CALL(btlevtcmd_GetStatusMg, LW(10), LW(6))
    MULTIPLY_FLOAT(LW(5), LW(6))
    DIVIDE(LW(5), 2)
    ADD(LW(1), LW(5))
    ADD(LW(2), 10)
    CALL(evt_eff, 0, STRING("charge"), 1, LW(0), LW(1), LW(2), FLOAT(1.0f), 120, 0, 0, 0, 0, 0, 0)
    CALL(evt_snd_sfxon_3d, STRING("SFX_ITEM_COOK_LAST1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_eff, 0, STRING("mist"), 6, LW(0), LW(1), LW(2), 0, 180, 0, 0, 0, 0, 0, 0)
    CALL(evt_snd_sfxon_3d, STRING("SFX_ITEM_COOK_LAST2"), LW(0), LW(1), LW(2), LW(7))
    WAIT_MS(2000)
    CALL(evt_snd_sfxoff, LW(7))
    CALL(btlevtcmd_SetHp, LW(10), 1)
    CALL(btlevtcmd_SetFp, LW(10), 0)
    CALL(_full_ap_recover, LW(10))
    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_SYSTEM_ON)
#if VERSION <= VERSION_DEMO_JP
    CALL(evt_snd_sfxon_3d, STRING("SFX_ITEM_COOK_LAST3"), LW(0), LW(1), LW(2), 0)
    CALL(evt_eff, 0, STRING("indirect"), 0, LW(0), LW(1), LW(2), 60, 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_eff, 0, STRING("hit"), 0, LW(0), LW(1), LW(2), FLOAT(1.0f), 3, 0, 0, 0, 0, 0, 0)
    WAIT_MS(250)
    CALL(evt_eff, 0, STRING("hit"), 1, LW(0), LW(1), LW(2), FLOAT(1.0f), 3, 0, 0, 0, 0, 0, 0)
    WAIT_MS(250)
    CALL(evt_eff, 0, STRING("hit"), 2, LW(0), LW(1), LW(2), FLOAT(1.0f), 3, 0, 0, 0, 0, 0, 0)
    WAIT_MS(250)
    CALL(evt_eff, 0, STRING("hit"), 3, LW(0), LW(1), LW(2), FLOAT(1.0f), 3, 0, 0, 0, 0, 0, 0)
#else
    CALL(evt_eff, 0, STRING("indirect"), 0, LW(0), LW(1), LW(2), 60, 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_eff, 0, STRING("hit"), 0, LW(0), LW(1), LW(2), FLOAT(1.0f), 3, 0, 0, 0, 0, 0, 0)
    CALL(evt_snd_sfxon_3d, STRING("SFX_ITEM_COOK_LAST3"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(250)
    CALL(evt_eff, 0, STRING("hit"), 1, LW(0), LW(1), LW(2), FLOAT(1.0f), 3, 0, 0, 0, 0, 0, 0)
    CALL(evt_snd_sfxon_3d, STRING("SFX_ITEM_COOK_LAST3"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(250)
    CALL(evt_eff, 0, STRING("hit"), 2, LW(0), LW(1), LW(2), FLOAT(1.0f), 3, 0, 0, 0, 0, 0, 0)
    CALL(evt_snd_sfxon_3d, STRING("SFX_ITEM_COOK_LAST3"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(250)
    CALL(evt_eff, 0, STRING("hit"), 3, LW(0), LW(1), LW(2), FLOAT(1.0f), 3, 0, 0, 0, 0, 0, 0)
    CALL(evt_snd_sfxon_3d, STRING("SFX_ITEM_COOK_LAST3"), LW(0), LW(1), LW(2), 0)
#endif
    WAIT_MS(1750)
    CALL(evt_audience_ap_recovery)
    CALL_CHILD(_return_home_event)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Trial Stew (Zess T. cooking): a drastic effect on the eater (ItemEvent_LastDinner). */
BattleWeapon ItemWeaponData_LastDinner = {
    NULL, 0, // name..icon
    {0}, // pad_06
    ITEM_TRIAL_STEW, // itemId
    NULL, // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_OPPOSITE_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_SAME_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_IGNORE_VULNERABILITY | SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    ItemEvent_LastDinner, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Thunder Rage: 5 lightning damage to every enemy. */
#if VERSION >= VERSION_JP
static EVT_BEGIN(ItemEvent_Kaminari_Dokkan)
    SET_VALUE(LW(12), PTR(&ItemWeaponData_Kaminari_Dokkan))
    CALL_CHILD(ItemEvent_Kaminari)
    RETURN
EVT_END()
#endif

/** Thunder Bolt: 5 lightning damage to one enemy. */
#if VERSION >= VERSION_JP
static EVT_BEGIN(ItemEvent_Kaminari_Gorogoro)
    SET_VALUE(LW(12), PTR(&ItemWeaponData_Kaminari_Gorogoro))
    CALL_CHILD(ItemEvent_Kaminari)
    RETURN
EVT_END()
#endif

/** Ruin Powder: confuses every enemy for three turns. */
#if VERSION >= VERSION_JP
static EVT_BEGIN(ItemEvent_Madowaseno_Kona)
    SET_VALUE(LW(12), PTR(&ItemWeaponData_Madowaseno_Kona))
    CALL_CHILD(ItemEvent_Support_NoEffect)
    RETURN
EVT_END()
#endif

/** Power Punch: makes the chosen ally huge (attack up) for three turns. */
#if VERSION >= VERSION_JP
static EVT_BEGIN(ItemEvent_Dekadeka_Drink)
    SET_VALUE(LW(12), PTR(&ItemWeaponData_Dekadeka_Drink))
    CALL_CHILD(ItemEvent_Support_NoEffect)
    RETURN
EVT_END()
#endif

/** Courage Shell: raises the chosen ally's defence by 3 for three turns. */
#if VERSION >= VERSION_JP
static EVT_BEGIN(ItemEvent_Katikati_Koura)
    SET_VALUE(LW(12), PTR(&ItemWeaponData_Katikati_Koura))
    CALL_CHILD(ItemEvent_Support_NoEffect)
    RETURN
EVT_END()
#endif

/** Mini Mr. Mini: shrinks every enemy (attack down) for three turns. */
#if VERSION >= VERSION_JP
static EVT_BEGIN(ItemEvent_Minimini_Kun)
    SET_VALUE(LW(12), PTR(&ItemWeaponData_Minimini_Kun))
    CALL_CHILD(ItemEvent_Support_NoEffect)
    RETURN
EVT_END()
#endif

/** Mr. Softener: lowers every enemy's defence by 3 for three turns. */
#if VERSION >= VERSION_JP
static EVT_BEGIN(ItemEvent_Funyafunya_Kun)
    SET_VALUE(LW(12), PTR(&ItemWeaponData_Funyafunya_Kun))
    CALL_CHILD(ItemEvent_Support_NoEffect)
    RETURN
EVT_END()
#endif

/** Tasty Tonic: cures the chosen ally's bad status effects. */
#if VERSION >= VERSION_JP
static EVT_BEGIN(ItemEvent_Sukkiri_Drink)
    SET_VALUE(LW(12), PTR(&ItemWeaponData_Sukkiri_Drink))
    CALL_CHILD(ItemEvent_Support_RefreshEffect)
    RETURN
EVT_END()
#endif

/** Spite Pouch: gives the chosen ally payback for three turns. */
#if VERSION >= VERSION_JP
static EVT_BEGIN(ItemEvent_Shikaeshino_Kona)
    SET_VALUE(LW(12), PTR(&ItemWeaponData_Shikaeshino_Kona))
    CALL_CHILD(ItemEvent_Support_NoEffect)
    RETURN
EVT_END()
#endif

/** Koopa Curse: slows the chosen enemy for three turns. */
#if VERSION >= VERSION_JP
static EVT_BEGIN(ItemEvent_Kameno_Noroi)
    SET_VALUE(LW(12), PTR(&ItemWeaponData_Kameno_Noroi))
    CALL_CHILD(ItemEvent_Support_NoEffect)
    RETURN
EVT_END()
#endif
