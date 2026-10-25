#include "version.h"
#include "battle/battle.h"
#include "battle/battle_database_common.h"
#include "battle/battle_stage.h"
#include "battle/battle_sub.h"
#include "driver/effdrv.h"
#include "driver/msgdrv.h"
#include "manager/evtmgr_cmd.h"
#include "mario/mario_party.h"
#include "system.h"
#include "driver/animdrv.h"
#include "pmario_sound.h"
#include <string.h>
#include "battle/battle_attack_audience.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_sub.h"
#include "party/party_motion.h"
#include "driver/camdrv.h"

#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "icon_ids.h"

USER_FUNC(_vivian_make_extra_work_area);
USER_FUNC(battle_evt_majo_disp_on);
USER_FUNC(btlevtcmd_ACRGetResult);
USER_FUNC(btlevtcmd_ACRStart);
USER_FUNC(btlevtcmd_AnimeChangePose);
USER_FUNC(btlevtcmd_AnimeChangePoseFromTable);
USER_FUNC(btlevtcmd_AnimeChangePoseType);
USER_FUNC(btlevtcmd_AnimeWaitPlayComplete);
USER_FUNC(btlevtcmd_AudienceAppeal);
USER_FUNC(btlevtcmd_AudienceDeclareAcrobatResult);
USER_FUNC(btlevtcmd_FaceDirectionAdd);
USER_FUNC(btlevtcmd_FaceDirectionSub);
USER_FUNC(btlevtcmd_GetFaceDirection);
USER_FUNC(btlevtcmd_GetHomePos);
USER_FUNC(btlevtcmd_GetMoveFrame);
USER_FUNC(btlevtcmd_GetPos);
USER_FUNC(btlevtcmd_GetUnitId);
USER_FUNC(btlevtcmd_GetUnitWork);
USER_FUNC(btlevtcmd_InviteApInfoReport);
USER_FUNC(btlevtcmd_MovePosition);
USER_FUNC(btlevtcmd_OffAttribute);
USER_FUNC(btlevtcmd_OffPartsAttribute);
USER_FUNC(btlevtcmd_PhaseEventStartDeclare);
USER_FUNC(btlevtcmd_RunDataEventChild);
USER_FUNC(btlevtcmd_SetDiveSound);
USER_FUNC(btlevtcmd_SetEventAttack);
USER_FUNC(btlevtcmd_SetEventConfusion);
USER_FUNC(btlevtcmd_SetEventDamage);
USER_FUNC(btlevtcmd_SetEventEntry);
USER_FUNC(btlevtcmd_SetEventWait);
USER_FUNC(btlevtcmd_SetJumpSound);
USER_FUNC(btlevtcmd_SetMoveSoundDataWork);
USER_FUNC(btlevtcmd_SetMoveSpeed);
USER_FUNC(btlevtcmd_SetPos);
USER_FUNC(btlevtcmd_SetRotate);
USER_FUNC(btlevtcmd_SetRunSound);
USER_FUNC(btlevtcmd_SetUnitWork);
USER_FUNC(btlevtcmd_SetWalkSound);
USER_FUNC(btlevtcmd_StartWaitEvent);
USER_FUNC(btlevtcmd_TransStageFloorPosition);
USER_FUNC(btlevtcmd_WaitEventEnd);
USER_FUNC(evt_audience_acrobat_notry);
USER_FUNC(evt_audience_ap_recovery);
USER_FUNC(evt_eff);
USER_FUNC(evt_snd_sfxon_3d);
USER_FUNC(_disp_heart_entry_stop_check);
USER_FUNC(_disp_heart_entry_stop);
void _disp_heart(CameraId camera, void* param);
USER_FUNC(_disp_heart_entry);
USER_FUNC(_make_kagenuke_weapon);
USER_FUNC(_get_vivian_attack_position);
USER_FUNC(_get_move_frame);
static void __makeTechMenuFunc(BattleCommand* command, s32* count);
USER_FUNC(battle_evt_majo_disp_off);

PoseTableEntry pose_table_vivian_stay[24];
static BattleDataEntry data_table[20];
BattleUnitKindPart unitpartsdata_Party_Vivian[1];
BattleUnitKind unitdata_Party_Vivian;
typedef struct { BattleUnitDefense d; u8 pad[3]; } VivDef;
typedef struct { BattleUnitDefenseAttr d; u8 pad[3]; } VivDefAttr;
typedef struct { StatusVulnerability d; u8 pad[2]; } VivRegist;
static VivDef defence;
static VivDefAttr defence_attr;
static VivRegist regist;
static EvtWord attack_event[2];
static EvtWord wait_event[6];
static EvtWord battle_entry_event[59];
static EvtWord init_event[68];
static EvtWord damage_event[16];
EvtWord vivian_hide_event[209];
BattleWeapon partyWeapon_VivianNormalAttack;
BattleWeapon partyWeapon_VivianShadowGuard;
BattleWeapon partyWeapon_VivianMagicalPowder;
BattleWeapon partyWeapon_VivianCharmKissAttack;
EvtWord partyVivianAttack_NormalAttack[581];
EvtWord partyVivianAttack_ShadowGuard[430];
EvtWord partyVivianAttack_MagicalPowder[VERSION_PICK(442, 448)];
EvtWord partyVivianAttack_CharmKissAttack[464];
EvtWord vivian_shadow_tail_event[78];
EvtWord vivian_counter_damage_event[177];
EvtWord vivian_appeal_event[80];
static EvtWord party_win_reaction[18];
static EvtWord attack_audience_event[189];




















PoseTableEntry pose_table_vivian_stay[] = {
    {9, "PTR_D_4"},
    {2, "PTR_D_4"},
    {3, "PTR_X_1"},
    {4, "PTR_D_2"},
    {5, "PTR_X_1"},
    {1, "PTR_N_1"},
    {27, "PTR_D_3"},
    {28, "PTR_S_1"},
    {37, "PTR_G_2"},
    {31, "PTR_G_1"},
    {39, "PTR_D_1"},
    {40, "PTR_W_1"},
    {42, "PTR_R_1"},
    {57, "PTR_D_2"},
    {56, "PTR_D_2"},
    {60, "PTR_D_4"},
    {61, "PTR_D_2"},
    {65, "PTR_T_1"},
    {52, "PTR_I_1"},
    {67, "PTR_C_1"},
    {64, "PTR_X_1"},
    {49, "PTR_D_1"},
    {45, "PTR_G_1"},
    {69, "PTR_S_1"},
};

static BattleDataEntry data_table[] = {
    {3, vivian_hide_event},
    {5, vivian_appeal_event},
    {6, attack_audience_event},
    {1, __makeTechMenuFunc},
    {25, vivian_counter_damage_event},
    {26, vivian_counter_damage_event},
    {27, vivian_counter_damage_event},
    {28, vivian_counter_damage_event},
    {29, vivian_counter_damage_event},
    {30, vivian_counter_damage_event},
    {31, vivian_counter_damage_event},
    {32, vivian_counter_damage_event},
    {33, vivian_counter_damage_event},
    {34, vivian_counter_damage_event},
    {35, vivian_counter_damage_event},
    {48, btldefaultevt_Dummy},
    {64, party_win_reaction},
    {7, subset_attack_ready_jump_event},
    {47, btldefaultevt_avoid_counter_event},
    {0, NULL},
};

BattleUnitKindPart unitpartsdata_Party_Vivian[] = {
    {1, "btl_un_vivian", "c_vivian", 0.0f, 0.0f, 0.0f, 16.0f, 24.0f, 5.0f, 0.0f, 24.0f, 0.0f, 20, 24, 255, &defence.d, &defence_attr.d, 0x9, 0, pose_table_vivian_stay},
};

BattleUnitKind unitdata_Party_Vivian = {
    228,
    "btl_un_vivian",
    10,
    5,
    5,
    1,
    1,
    0,
    0,
    0,
    0,
    100,
    9,
    20,
    33,
    14,
    36,
    0.0f,
    12.0f,
    0.0f,
    0,
    0,
    0.0f,
    40.0f,
    0.0f,
    0.0f,
    40.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    10.0f,
    0.0f,
    0.0f,
    10.0f,
    21.45f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0,
    0,
    -1,
    0,
    100,
    20,
    0.0f,
    0.0f,
    0.0f,
    "SFX_PARTY_DAMAGE1",
    "SFX_MARIO_DAMAGE_FIRE1",
    "SFX_MARIO_DAMAGE_ICE1",
    "SFX_MARIO_DAMAGE_BIRIBIRI1",
    0x2000004,
    &regist.d,
    1,
    unitpartsdata_Party_Vivian,
    init_event,
    data_table,
};

static VivDef defence = {{{0, 0, 0, 0, 0}}, {0, 0, 0}};

static VivDefAttr defence_attr = {{{0, 0, 0, 0, 0}}, {0, 0, 0}};

static VivRegist regist = {{100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100}, {0, 0}};

static EVT_BEGIN(attack_event)
    RETURN
EVT_END()

static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, -2, 1)
    RETURN
EVT_END()


USER_FUNC(battle_evt_majo_disp_on) {
    EvtWord* args = event->args;
    BattleUnit* unit;
    BattleUnitPart* part;
    s32 id, partNo, flag;
    f32 px, py, pz;
    s32 pose;

    id = BattleTransID(event, evtGetValue(event, args[0]));
    partNo = evtGetValue(event, args[1]);
    px = evtGetFloat(event, args[2]);
    py = evtGetFloat(event, args[3]);
    pz = evtGetFloat(event, args[4]);
    flag = evtGetValue(event, args[5]);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    part = BattleGetUnitPartsPtr(id, partNo);
    pose = part->poseId;
    if (isFirstCall) {
        event->userdata[0] = 0;
        event->userdata[1] = 0;
        {
            Vec v = {0.0f, 0.0f, 0.0f};
            v.x = px;
            v.y = py - 200.0f;
            v.z = pz;
            unit->mPosition = v;
        }
        animEffectAsync("p_bibi", 1);
    }
    switch (event->userdata[0]) {
        case 0:
            part->shadowScale = intplGetValue(0, 0.0f, 2.0f, event->userdata[1], 30);
            if (event->userdata[1] == 0) {
                psndSFXOn_3D("SFX_BTL_VIVIAN_SHADOW1_1", &unit->mPosition);
            }
            event->userdata[1]++;
            if (event->userdata[1] > 15) {
                event->userdata[0]++;
            }
            break;
        case 1:
            if (event->userdata[1] <= 30) {
                part->shadowScale = intplGetValue(0, 0.0f, 2.0f, event->userdata[1], 30);
                event->userdata[1]++;
            }
            if (!animEffectAsync("p_bibi", 1)) {
                return EVT_RETURN_BLOCK;
            }
            {
                Vec v = {0.0f, 0.0f, 0.0f};
                v.x = px;
                v.y = py;
                v.z = pz;
                unit->mPosition = v;
            }
            animPoseSetEffect(pose, "p_bibi", 1);
            animPoseSetEffectAnim(pose, "PM_B_2", 1);
            event->userdata[0]++;
            break;
        case 2:
            if (event->userdata[1] <= 30) {
                part->shadowScale = intplGetValue(0, 0.0f, 2.0f, event->userdata[1], 30);
                event->userdata[1]++;
            }
            if (animPoseGetLoopTimes(animPoseGetAnimPosePtr(pose)->paperId) >= 0.6666667f) {
                animPoseSetPaperAnimGroup(pose, NULL, 1);
                event->userdata[0]++;
                event->userdata[1] = 0;
            }
            break;
        case 3:
            part->shadowScale = intplGetValue(0, 2.0f, 1.0f, event->userdata[1], 15);
            event->userdata[1]++;
            if (event->userdata[1] > 15) {
                part->shadowScale = 1.0f;
                animPoseSetEffect(pose, NULL, 1);
                if (flag != 0) {
                    unit->flags &= ~0x10000;
                }
                return EVT_RETURN_DONE;
            }
            break;
    }
    return EVT_RETURN_BLOCK;
}


static EVT_BEGIN(battle_entry_event)
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 250)
    CALL(btlevtcmd_SetPos, -2, LW(0), LW(1), LW(2))
    WAIT_FRAMES(40)
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 42)
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(4.0f))
    CALL(btlevtcmd_SetMoveSoundDataWork, -2, STRING("SFX_PARTY_BATTLE_SUBHANG1"), STRING("SFX_PARTY_BATTLE_SUBHANG1"), 0, 6, 6, 0, 0)
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -2, 0)
    CALL(btlevtcmd_AnimeChangePoseFromTable, -2, 1)
    RETURN
EVT_END()

static EVT_BEGIN(init_event)
    CALL(_vivian_make_extra_work_area)
    CALL(btlevtcmd_SetEventWait, -2, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, -2, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, -2, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, -2, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_SetUnitWork, -2, 4, 0)
    CALL(btlevtcmd_SetUnitWork, -2, 3, 0)
    SPAWN_EVENT(PTR(vivian_shadow_tail_event))
    CALL(btlevtcmd_SetWalkSound, -2, STRING("SFX_PARTY_BATTLE_MOVE1L"), STRING("SFX_PARTY_BATTLE_MOVE1R"), 0, 20, 20)
    CALL(btlevtcmd_SetRunSound, -2, STRING("SFX_PARTY_BATTLE_MOVE1L"), STRING("SFX_PARTY_BATTLE_MOVE1R"), 0, 11, 11)
    CALL(btlevtcmd_SetDiveSound, -2, STRING("SFX_PARTY_BATTLE_HANG1"), STRING("SFX_PARTY_BATTLE_HANG1"), 0, 11, 11)
    CALL(btlevtcmd_SetJumpSound, -2, STRING("SFX_PARTY_BATTLE_JUMP1"), STRING("SFX_PARTY_BATTLE_LANDING1"))
    CALL(btlevtcmd_SetEventEntry, -2, PTR(battle_entry_event))
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(damage_event)
    CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    RETURN
EVT_END()

EVT_BEGIN(vivian_hide_event)
    CALL(btlevtcmd_GetUnitWork, -2, 4, LW(0))
    IF_INT_GREATER_OR_EQUAL(LW(0), 1)
        SUBTRACT(LW(0), 1)
    END_IF
    CALL(btlevtcmd_SetUnitWork, -2, 4, LW(0))
    IF_INT_LESS_OR_EQUAL(LW(0), 0)
        SET_VALUE(LW(3), -3)
        CALL(btlevtcmd_PhaseEventStartDeclare, LW(3))
        CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionSub, LW(3), LW(0), 15)
        CALL(btlevtcmd_SetPos, -2, LW(0), LW(1), LW(2))
        BEGIN_INLINE_EVENT
            CALL(btlevtcmd_OffAttribute, LW(3), 16)
            CALL(btlevtcmd_OffPartsAttribute, LW(3), 1, 50331648)
            CALL(battle_evt_majo_disp_on, LW(3), 1, LW(0), LW(1), LW(2), 1)
            CALL(btlevtcmd_SetPos, LW(3), LW(0), LW(1), LW(2))
            CALL(btlevtcmd_GetHomePos, LW(3), LW(0), LW(1), LW(2))
            CALL(btlevtcmd_SetMoveSpeed, LW(3), FLOAT(2.0f))
            CALL(btlevtcmd_MovePosition, LW(3), LW(0), LW(1), LW(2), 0, -1, 0)
        END_INLINE_EVENT
        CALL(btlevtcmd_OffPartsAttribute, -2, 1, 65536)
        CALL(btlevtcmd_OffPartsAttribute, -2, 1, 16777216)
        CALL(battle_evt_majo_disp_on, -2, 1, LW(0), LW(1), LW(2), 1)
        CALL(btlevtcmd_SetPos, -2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetMoveFrame, -2, LW(0), LW(1), LW(2), FLOAT(2.0f), LW(15))
        BEGIN_SIBLING_EVENT_GET_ID(LW(14))
            CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), LW(15), -1, 0)
        END_SIBLING_EVENT
        CALL(btlevtcmd_ACRStart, -2, 0, LW(15), LW(15), 15)
        CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
        SWITCH(LW(6))
            CASE_GREATER_OR_EQUAL(2)
            DELETE_EVENT(LW(14))
            CALL(btlevtcmd_AudienceDeclareAcrobatResult, PTR(&partyWeapon_VivianShadowGuard), 1, 0, 0, 0)
            CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PTR_Y_1"))
            CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(2.0f))
            CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
            CALL(btlevtcmd_AnimeWaitPlayComplete, -2, 1)
            CASE_DEFAULT
            CALL(evt_audience_acrobat_notry)
            CALL(btlevtcmd_WaitEventEnd, LW(14))
        END_SWITCH
        CALL(evt_audience_ap_recovery)
        CALL(btlevtcmd_InviteApInfoReport)
        CALL(btlevtcmd_StartWaitEvent, LW(3))
        CALL(btlevtcmd_StartWaitEvent, -2)
    END_IF
    RETURN
EVT_END()

BattleWeapon partyWeapon_VivianNormalAttack = {
    "btl_wn_ptr_normal", // name
#ifdef VERSION_G8ME01
    344, // icon
#else
    336, // icon
#endif
    {0}, // pad_06
    0, // itemId
    "msg_ptr_kagenuke", // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    1, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    weaponGetPowerFromPartyAttackLv, // hpDamageFn
    {3, 3, 4, 4, 5, 5, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x20000000, // targetPropertyFlags
    ELEMENT_NORMAL, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_kagenuke", // acHelpMsg
    0x30007, // special
    COUNTER_RESIST_TOP_SPIKY, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, // poisonStrength..dodgyTime
    100, // burnChance
    2, // burnTime
    0, 0, 0, 0, 0, 0, 0, 0, // freezeChance..atkChangeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // defChangeChance..fastChance
    0, 0, 0, 0, 0, 0, 0, 0, // fastTime..invisibleChance
    0, 0, 0, 0, 0, // invisibleTime..fpRegenStrength
    {0}, // pad_AE
    partyVivianAttack_NormalAttack, // mAttackEvtScript
#ifdef VERSION_G8ME01
    {0, 0, 0, 100, 0, 4, 2, 0, 0, 0, 0, 0}, // stageEffectRates
#else
    {0, 0, 0, 100, 0, 2, 2, 0, 0, 0, 0, 0}, // stageEffectRates
#endif
};

BattleWeapon partyWeapon_VivianShadowGuard = {
    "btl_wn_ptr_lv1", // name
#ifdef VERSION_G8ME01
    345, // icon
#else
    337, // icon
#endif
    {0}, // pad_06
    0, // itemId
    "msg_ptr_kagegakure", // description
    100, // baseAccuracy
    1, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
#ifdef VERSION_G8ME01
    1, // mStylishCommandMultiplier
#else
    2, // mStylishCommandMultiplier
#endif
    5, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    NULL, // hpDamageFn
    {0, 2, 2, 3, 3, 4, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OPPOSITE_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x10000002, // targetPropertyFlags
    ELEMENT_NORMAL, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_kagegakure", // acHelpMsg
    0x20008, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    partyVivianAttack_ShadowGuard, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

BattleWeapon partyWeapon_VivianMagicalPowder = {
    "btl_wn_ptr_lv2", // name
#ifdef VERSION_G8ME01
    346, // icon
#else
    338, // icon
#endif
    {0}, // pad_06
    0, // itemId
    "msg_ptr_mahou_no_kona", // description
    100, // baseAccuracy
    6, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    5, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    weaponGetACOutputParam, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x20000000, // targetPropertyFlags
    ELEMENT_FIRE, // element
    0, // damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_mahou_no_kona", // acHelpMsg
    0x20047, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0x80002000, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, // poisonStrength..dodgyTime
    100, // burnChance
    3, // burnTime
    0, 0, 0, 0, 0, 0, 0, 0, // freezeChance..atkChangeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // defChangeChance..fastChance
    0, 0, 0, 0, 0, 0, 0, 0, // fastTime..invisibleChance
    0, 0, 0, 0, 0, // invisibleTime..fpRegenStrength
    {0}, // pad_AE
    partyVivianAttack_MagicalPowder, // mAttackEvtScript
#ifdef VERSION_G8ME01
    {0, 0, 0, 100, 0, 10, 5, 0, 0, 0, 0, 0}, // stageEffectRates
#else
    {0, 0, 0, 100, 0, 5, 5, 0, 0, 0, 0, 0}, // stageEffectRates
#endif
};

BattleWeapon partyWeapon_VivianCharmKissAttack = {
    "btl_wn_ptr_lv3", // name
#ifdef VERSION_G8ME01
    347, // icon
#else
    339, // icon
#endif
    {0}, // pad_06
    0, // itemId
    "msg_ptr_meromero_kiss", // description
    100, // baseAccuracy
    4, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
#ifdef VERSION_G8ME01
    1, // mStylishCommandMultiplier
#else
    2, // mStylishCommandMultiplier
#endif
    5, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x20000000, // targetPropertyFlags
    ELEMENT_NORMAL, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_meromero_kiss", // acHelpMsg
    0x20000, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0x80002004, // targetWeightFlags
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
    partyVivianAttack_CharmKissAttack, // mAttackEvtScript
#ifdef VERSION_G8ME01
    {0, 0, 0, 100, 0, 10, 5, 0, 0, 0, 0, 0}, // stageEffectRates
#else
    {0, 0, 0, 100, 0, 5, 5, 0, 0, 0, 0, 0}, // stageEffectRates
#endif
};

EVT_BEGIN(partyVivianAttack_NormalAttack)
    CALL(btlevtcmd_JumpSetting, -2, 20, FLOAT(0.0f), FLOAT(0.69921875f))
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(12))
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_GetPartyTechLv, -2, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(2)
        CALL(_make_kagenuke_weapon, LW(12), 2)
        CASE_EQUAL(1)
        CALL(_make_kagenuke_weapon, LW(12), 2)
        CASE_DEFAULT
        CALL(_make_kagenuke_weapon, LW(12), 2)
    END_SWITCH
    CALL(btlevtcmd_AttackDeclare, -2, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_CommandPayWeaponCost, -2)
    CALL(btlevtcmd_RunDataEventChild, -2, 7)
    CALL(evt_btl_camera_set_mode, 0, 11)
    CALL(evt_btl_camera_set_homing_unitparts, 0, -2, 1, LW(3), LW(4))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(btlevtcmd_CommandGetWeaponActionLv, LW(0))
    CALL(btlevtcmd_AcSetDifficulty, -2, LW(0))
    CALL(btlevtcmd_AcSetParamAll, 1, 1, 40, 1, 2, 0, 3, 0)
    CALL(btlevtcmd_AcSetFlag, 23)
    CALL(btlevtcmd_SetupAC, -2, 18, 1, 0)
    CALL(evt_snd_sfxon, STRING("SFX_BTL_VIVIAN_ATTACK1"), 0)
    CALL(battle_evt_majo_disp_off, -2, 1, 0, 0)
    CALL(btlevtcmd_StartAC, 1)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(_get_vivian_attack_position, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetPos, -2, EVT_NULLPTR, LW(1), EVT_NULLPTR)
    CALL(btlevtcmd_DivePosition, -2, LW(0), LW(1), LW(2), 50, 0, 5, 0, -1)
    CALL(btlevtcmd_SetUnitWork, -2, 3, 1)
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BTL_VIVIAN_ATTACK2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(battle_evt_majo_disp_on, -2, 1, LW(0), LW(1), LW(2), 0)
    CALL(btlevtcmd_SetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(6), LW(7), LW(8))
    CALL(_get_vivian_attack_position, LW(3), LW(0), LW(1), LW(2))
    CALL(_get_vivian_attack_position, LW(3), LW(6), LW(7), LW(8))
    CALL(btlevtcmd_TransStageFloorPosition, LW(6), LW(7), LW(8))
    CALL(btlevtcmd_GetFaceDirection, LW(3), LW(15))
    MULTIPLY(LW(15), -1)
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
    CALL(btlevtcmd_GetPos, -2, LW(6), LW(7), LW(8))
    CALL(_get_move_frame, LW(0), LW(1), LW(2), LW(6), LW(7), LW(8), FLOAT(4.0f), LW(6))
    IF_INT_GREATER_OR_EQUAL(LW(6), 1)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_BTL_VIVIAN_ATTACK3"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_DivePosition, -2, LW(0), LW(1), LW(2), 12, 0, 5, 0, -1)
    END_IF
    LABEL(9)
    CALL(btlevtcmd_CommandPreCheckDamage, -2, LW(3), LW(4), 256, LW(5))
    IF_INT_EQUAL(LW(5), 5)
        CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(12), 256, LW(5))
    END_IF
    CALL(btlevtcmd_ResultAC)
    SET_VALUE(LW(10), 0)
    LABEL(10)
    IF_INT_GREATER_OR_EQUAL(LW(10), 1)
        CALL(btlevtcmd_PreCheckDamage, -2, LW(3), LW(4), LW(12), 1048832, LW(5))
    ELSE
        CALL(btlevtcmd_PreCheckDamage, -2, LW(3), LW(4), LW(12), 256, LW(5))
    END_IF
    SWITCH(LW(5))
        CASE_OR_EQUAL(2)
        CASE_OR_EQUAL(3)
        CASE_OR_EQUAL(6)
        CASE_OR_EQUAL(4)
        CASE_END
        CASE_EQUAL(1)
        CASE_DEFAULT
    END_SWITCH
    IF_INT_NOT_EQUAL(LW(5), 1)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PTR_A1_2"))
        WAIT_MS(166)
        IF_INT_EQUAL(LW(5), 3)
            CALL(btlevtcmd_StartAvoid, LW(3), 38)
        END_IF
        IF_INT_EQUAL(LW(5), 6)
            CALL(btlevtcmd_StartAvoid, LW(3), 39)
        END_IF
        IF_INT_EQUAL(LW(5), 2)
            CALL(btlevtcmd_StartAvoid, LW(3), 40)
        END_IF
        WAIT_MS(500)
        GOTO(90)
    END_IF
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PTR_A1_2"))
    CALL(btlevtcmd_ftomsec, 4, LW(0))
    WAIT_MS(LW(0))
    CALL(evt_snd_sfxon, STRING("SFX_BTL_VIVIAN_ATTACK4"), 0)
    CALL(btlevtcmd_GetResultAC, LW(0))
    IF_BITS_SET(LW(0), 2)
        CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(12), 131328, LW(5))
    ELSE
        CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(12), 536871168, LW(5))
    END_IF
    IF_BITS_SET(LW(0), 2)
        CALL(btlevtcmd_GetResultPrizeLv, LW(3), 0, LW(6))
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_ACSuccessEffect, LW(6), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_AudienceDeclareACResult, LW(12), LW(6))
    ELSE
        CALL(btlevtcmd_AudienceDeclareACResult, LW(12), -1)
    END_IF
    WAIT_MS(500)
    LABEL(90)
    CALL(btlevtcmd_StopAC)
    LABEL(95)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PTR_A1_1"))
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_DivePosition, -2, LW(0), LW(1), LW(2), 12, 0, 5, 0, -1)
    CALL(btlevtcmd_SetUnitWork, -2, 3, 0)
    CALL(battle_evt_majo_disp_off, -2, 1, 0, 0)
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetPos, -2, EVT_NULLPTR, LW(1), EVT_NULLPTR)
    CALL(btlevtcmd_DivePosition, -2, LW(0), LW(1), LW(2), 40, 0, 5, 0, -1)
    BEGIN_SIBLING_EVENT
#ifdef VERSION_G8ME01
        CALL(btlevtcmd_ACRStart, -2, 14, 29, 29, 0)
#else
        CALL(btlevtcmd_ACRStart, -2, 20, 29, 29, 0)
#endif
    END_SIBLING_EVENT
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(battle_evt_majo_disp_on, -2, 1, LW(0), LW(1), LW(2), 0)
    CALL(btlevtcmd_SetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
    IF_INT_EQUAL(LW(6), 2)
        CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(0))
        CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(0), 1, 0, 0, 0)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PTR_Y_1"))
        WAIT_FRAMES(52)
    END_IF
    LABEL(99)
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL_CHILD(btldefaultevt_SuitoruBadgeEffect)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

EVT_BEGIN(partyVivianAttack_ShadowGuard)
    SET_VALUE(LW(3), -3)
    CALL(btlevtcmd_GetBodyId, LW(3), LW(4))
    CALL(btlevtcmd_CommandPayWeaponCost, -2)
    CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(12))
    CALL(btlevtcmd_RunDataEventChild, -2, 7)
    CALL(evt_btl_camera_set_mode, 0, 7)
    CALL(evt_btl_camera_set_homing_unit, 0, -2, 1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(evt_btl_camera_set_posoffset, 0, 20, 0, 0)
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(3), -1, 16, LW(0))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(0))
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PTR_A2_1"))
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, LW(3), LW(0), 15)
    ADD(LW(2), 5)
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(2.0f))
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(3), -1, 16, LW(0))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(0))
    CALL(btlevtcmd_CommandGetWeaponActionLv, LW(0))
    CALL(btlevtcmd_AcSetDifficulty, -2, LW(0))
    CALL(btlevtcmd_CommandGetWeaponActionLv, LW(0))
    CALL(btlevtcmd_AcSetDifficulty, -2, LW(0))
    CALL(btlevtcmd_AcGetDifficulty, LW(0))
    SUBTRACT(LW(0), 3)
    SWITCH(LW(0))
        CASE_LESS_OR_EQUAL(-3)
        SET_VALUE(LW(0), 420)
        CASE_EQUAL(-2)
        SET_VALUE(LW(0), 330)
        CASE_EQUAL(-1)
        SET_VALUE(LW(0), 270)
        CASE_EQUAL(0)
        SET_VALUE(LW(0), 240)
        CASE_EQUAL(1)
        SET_VALUE(LW(0), 200)
        CASE_EQUAL(2)
        SET_VALUE(LW(0), 160)
        CASE_DEFAULT
        SET_VALUE(LW(0), 120)
    END_SWITCH
    CALL(btlevtcmd_AcSetParamAll, LW(0), 1, 5, -2, 5, EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR)
    CALL(btlevtcmd_AcSetFlag, 0)
    CALL(btlevtcmd_SetupAC, -2, 10, 1, 0)
    WAIT_FRAMES(22)
    CALL(btlevtcmd_StartAC, 1)
    CALL(btlevtcmd_ResultAC)
    CALL(btlevtcmd_GetResultAC, LW(0))
    IF_BITS_CLEAR(LW(0), 2)
        CALL(btlevtcmd_AudienceDeclareACResult, LW(12), -1)
        CALL(btlevtcmd_StopAC)
        CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(2.0f))
        CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
        GOTO(90)
    END_IF
    CALL(btlevtcmd_GetResultPrizeLv, -1, 0, LW(6))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_ACSuccessEffect, LW(6), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_AudienceDeclareACResult, LW(12), LW(6))
    CALL(btlevtcmd_StopAC)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 350)
    CALL(evt_btl_camera_set_posoffset, 0, 0, 0, 0)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PTR_A2_2"))
    CALL(btlevtcmd_AnimeWaitPlayComplete, -2, 1)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PTR_A2_3"))
    CALL(btlevtcmd_AnimeWaitPlayComplete, -2, 1)
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_AnimeChangePose, LW(3), 1, STRING("M_B_3"))
        CALL(battle_evt_majo_disp_off, LW(3), 1, 1, 0)
        CALL(btlevtcmd_OnAttribute, LW(3), 16)
        CALL(btlevtcmd_OnPartsAttribute, LW(3), 1, 50331648)
        WAIT_FRAMES(1)
        CALL(btlevtcmd_GetHomePos, LW(3), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_SetPos, LW(3), LW(0), LW(1), LW(2))
    END_SIBLING_EVENT
    CALL(battle_evt_majo_disp_off, -2, 1, 1, 1)
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetPos, LW(3), LW(5), LW(1), LW(2))
    ADD(LW(0), LW(5))
    DIVIDE(LW(0), 2)
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    ADD(LW(1), 1)
    CALL(evt_eff, 0, STRING("ripple"), 1, LW(0), LW(1), LW(2), 0, 0, 0, 0, 0, 0, 0, 0)
    WAIT_MS(333)
    CALL(btlevtcmd_OnPartsAttribute, -2, 1, 16842752)
    WAIT_FRAMES(1)
    CALL(btlevtcmd_SetUnitWork, -2, 4, 1)
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPos, -2, LW(0), LW(1), LW(2))
    WAIT_FRAMES(1)
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, -1, -1, 4, LW(0))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(0))
    WAIT_MS(500)
    LABEL(90)
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    LABEL(95)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_posoffset, 0, 0, 0, 0)
    LABEL(99)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

EVT_BEGIN(partyVivianAttack_MagicalPowder)
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(12))
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_AttackDeclareAll, -2)
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_CommandPayWeaponCost, -2)
    CALL(btlevtcmd_RunDataEventChild, -2, 7)
    WAIT_FRAMES(20)
    CALL(btlevtcmd_CommandGetWeaponActionLv, LW(0))
    CALL(btlevtcmd_AcSetDifficulty, -2, LW(0))
    CALL(btlevtcmd_AcGetDifficulty, LW(0))
    SUBTRACT(LW(0), 3)
    SWITCH(LW(0))
        CASE_LESS_OR_EQUAL(-3)
        SET_VALUE(LW(0), 360)
        CASE_EQUAL(-2)
        SET_VALUE(LW(0), 270)
        CASE_EQUAL(-1)
        SET_VALUE(LW(0), 210)
        CASE_EQUAL(0)
        SET_VALUE(LW(0), 180)
        CASE_EQUAL(1)
        SET_VALUE(LW(0), 160)
        CASE_EQUAL(2)
        SET_VALUE(LW(0), 140)
        CASE_DEFAULT
        SET_VALUE(LW(0), 120)
    END_SWITCH
    CALL(btlevtcmd_AcSetParamAll, LW(0), 1, 5, -3, 1, EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR)
    CALL(btlevtcmd_AcSetFlag, 7)
    CALL(btlevtcmd_SetupAC, -2, 10, 1, 0)
    WAIT_FRAMES(22)
    CALL(evt_btl_camera_set_mode, 0, 7)
    CALL(evt_btl_camera_set_homing_unit, 0, -2, 1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 4)
    CALL(evt_btl_camera_set_zoom, 0, 400)
    CALL(evt_btl_camera_set_posoffset, 0, 20, 0, 0)
    CALL(btlevtcmd_StartAC, 1)
    CALL(btlevtcmd_ResultAC)
    CALL(btlevtcmd_StopAC)
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetWidth, -2, LW(5))
        CALL(btlevtcmd_GetHeight, -2, LW(6))
        CALL(evt_eff, 0, STRING("stardust"), 4, LW(0), LW(1), LW(2), LW(5), LW(6), 30, 60, 0, 0, 0, 0)
    END_SIBLING_EVENT
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BTL_VIVIAN_MAGIC1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PTR_A3_1"))
    CALL(btlevtcmd_ftof, 20, LW(0))
    WAIT_FRAMES(20)
    CALL(btlevtcmd_ftof, 26, LW(0))
    CALL(btlevtcmd_ftof, 46, LW(1))
    CALL(btlevtcmd_ACRStart, -2, LW(0), LW(1), LW(1), 0)
    CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
    SWITCH(LW(6))
        CASE_GREATER_OR_EQUAL(2)
        CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(12), 1, 0, 0, 0)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PTR_Y_1"))
        CASE_DEFAULT
        CALL(evt_audience_acrobat_notry)
    END_SWITCH
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
#ifdef VERSION_G8ME01
    CALL(evt_btl_camera_set_posoffset, 0, 0, 0, 0)
#endif
    WAIT_FRAMES(40)
    SET_VALUE(LW(10), 0)
    LABEL(10)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(evt_eff, 0, STRING("kemuri_test"), 6, LW(0), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
    CALL(btlevtcmd_CommandPreCheckDamage, -2, LW(3), LW(4), 256, LW(6))
    IF_INT_NOT_EQUAL(LW(6), 1)
        IF_INT_EQUAL(LW(6), 3)
            CALL(btlevtcmd_StartAvoid, LW(3), 38)
        END_IF
        IF_INT_EQUAL(LW(6), 6)
            CALL(btlevtcmd_StartAvoid, LW(3), 39)
        END_IF
        IF_INT_EQUAL(LW(6), 2)
            CALL(btlevtcmd_StartAvoid, LW(3), 40)
        END_IF
        GOTO(50)
    END_IF
    CALL(btlevtcmd_AcGetOutputParam, 0, LW(0))
    IF_INT_LESS_OR_EQUAL(LW(0), 3)
        CALL(btlevtcmd_CommandCheckDamage, -2, LW(3), LW(4), 131328, LW(5))
    ELSE
        CALL(btlevtcmd_CommandCheckDamage, -2, LW(3), LW(4), 256, LW(5))
    END_IF
    IF_INT_EQUAL(LW(5), 18)
        CALL(btlevtcmd_AcGetOutputParam, 0, LW(0))
        SWITCH(LW(0))
            CASE_LESS_OR_EQUAL(0)
            SET_VALUE(LW(0), -1)
            CASE_OR_EQUAL(1)
            CASE_OR_EQUAL(2)
            SET_VALUE(LW(0), 0)
            CASE_END
            CASE_OR_EQUAL(3)
            CASE_OR_EQUAL(4)
            SET_VALUE(LW(0), 1)
            CASE_END
            CASE_DEFAULT
            SET_VALUE(LW(0), 2)
        END_SWITCH
        IF_INT_GREATER_OR_EQUAL(LW(0), 0)
            CALL(btlevtcmd_GetResultPrizeLv, LW(3), LW(0), LW(6))
            CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
            CALL(btlevtcmd_ACSuccessEffect, LW(6), LW(0), LW(1), LW(2))
            CALL(btlevtcmd_AudienceDeclareACResult, LW(12), LW(6))
        ELSE
            CALL(btlevtcmd_AudienceDeclareACResult, LW(12), -1)
        END_IF
    END_IF
    LABEL(50)
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    IF_INT_NOT_EQUAL(LW(3), -1)
        ADD(LW(10), 1)
        GOTO(10)
    END_IF
    LABEL(80)
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    LABEL(90)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_posoffset, 0, 0, 0, 0)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PTR_A1_1"))
    WAIT_MS(800)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL_CHILD(btldefaultevt_SuitoruBadgeEffect)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

EVT_BEGIN(partyVivianAttack_CharmKissAttack)
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(12))
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_AttackDeclareAll, -2)
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_CommandPayWeaponCost, -2)
    CALL(btlevtcmd_RunDataEventChild, -2, 7)
    WAIT_FRAMES(20)
    SET_VALUE(LW(10), 0)
    LABEL(10)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PTR_A4_1"))
    CALL(evt_snd_sfxon, STRING("SFX_PARTY_MOJIMOJI1"), 0)
    CALL(btlevtcmd_CommandGetWeaponActionLv, LW(0))
    CALL(btlevtcmd_AcSetDifficulty, -2, LW(0))
    CALL(btlevtcmd_AcGetDifficulty, LW(0))
    SUBTRACT(LW(0), 3)
    SWITCH(LW(0))
        CASE_LESS_OR_EQUAL(-3)
        SET_VALUE(LW(5), 60)
        CASE_EQUAL(-2)
        SET_VALUE(LW(5), 45)
        CASE_EQUAL(-1)
        SET_VALUE(LW(5), 30)
        CASE_EQUAL(0)
        SET_VALUE(LW(5), 25)
        CASE_EQUAL(1)
        SET_VALUE(LW(5), 20)
        CASE_EQUAL(2)
        SET_VALUE(LW(5), 12)
        CASE_DEFAULT
        SET_VALUE(LW(5), 6)
    END_SWITCH
    CALL(evt_sub_random, LW(1), 89)
    ADD(LW(1), 30)
    SET_VALUE(LW(2), LW(1))
    SUBTRACT(LW(2), LW(5))
    CALL(btlevtcmd_AcSetParamAll, 5, LW(1), LW(2), LW(1), EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR)
    IF_INT_GREATER(LW(10), 0)
        CALL(btlevtcmd_AcSetFlag, 9)
    ELSE
        CALL(btlevtcmd_AcSetFlag, 8)
    END_IF
    CALL(btlevtcmd_SetupAC, -2, 11, 1, 0)
    WAIT_FRAMES(22)
    IF_INT_GREATER(LW(10), 0)
        CALL(btlevtcmd_StartAC, 0)
    ELSE
        CALL(btlevtcmd_StartAC, 1)
    END_IF
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        SET_VALUE(LW(0), 5)
        LOOP(LW(0))
            CALL(btlevtcmd_AcGetOutputParam, 2, LW(1))
            IF_INT_NOT_EQUAL(LW(1), 0)
                CALL(_disp_heart_entry, 10, 20, 25, LW(3), LW(4))
                WAIT_FRAMES(1)
                BREAK_LOOP
            END_IF
            CALL(_disp_heart_entry_stop_check, LW(1))
            IF_INT_NOT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            SET_VALUE(LW(0), 5)
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_ResultAC)
    CALL(btlevtcmd_GetResultAC, LW(7))
    IS_EVENT_RUNNING(LW(15), LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(_disp_heart_entry_stop, LW(15))
    END_IF
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PTR_A4_2"))
    WAIT_MS(200)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BTL_VIVIAN_MEROMERO1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, -2, LW(0), 15)
    ADD(LW(1), 30)
    CALL(btlevtcmd_GetFaceDirection, -2, LW(6))
    MULTIPLY(LW(6), 45)
    CALL(evt_eff, 0, STRING("kiss"), 4, LW(0), LW(1), LW(2), LW(6), 0, 0, 0, 0, 0, 0, 0)
    CALL(btlevtcmd_CommandPreCheckDamage, -2, LW(3), LW(4), 256, LW(6))
    IF_INT_NOT_EQUAL(LW(6), 1)
        IF_INT_EQUAL(LW(6), 3)
            CALL(btlevtcmd_StartAvoid, LW(3), 38)
        END_IF
        IF_INT_EQUAL(LW(6), 6)
            CALL(btlevtcmd_StartAvoid, LW(3), 39)
        END_IF
        IF_INT_EQUAL(LW(6), 2)
            CALL(btlevtcmd_StartAvoid, LW(3), 40)
        END_IF
        GOTO(50)
    END_IF
    IF_BITS_SET(LW(7), 2)
        CALL(btlevtcmd_CommandCheckDamage, -2, LW(3), LW(4), 131328, LW(5))
    ELSE
        CALL(btlevtcmd_CommandCheckDamage, -2, LW(3), LW(4), 536871168, LW(5))
    END_IF
    IF_INT_EQUAL(LW(5), 18)
        IF_BITS_SET(LW(7), 2)
            CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
            ADD(LW(2), 10)
            CALL(evt_eff, 0, STRING("kiss"), 5, LW(0), LW(1), LW(2), 0, 0, 0, 0, 0, 0, 0, 0)
            CALL(btlevtcmd_GetResultPrizeLv, LW(3), 0, LW(6))
            CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
            CALL(btlevtcmd_ACSuccessEffect, LW(6), LW(0), LW(1), LW(2))
            CALL(btlevtcmd_AudienceDeclareACResult, LW(12), LW(6))
        ELSE
            CALL(btlevtcmd_AudienceDeclareACResult, LW(12), -1)
        END_IF
    END_IF
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_ACRStart, -2, 0, 30, 30, 0)
    END_SIBLING_EVENT
    CALL(btlevtcmd_AnimeWaitPlayComplete, -2, 1)
    CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
    IF_INT_EQUAL(LW(6), 2)
        CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(0))
        CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(0), 1, 0, 0, 0)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PTR_Y_1"))
        WAIT_FRAMES(32)
    END_IF
    WAIT_FRAMES(5)
    LABEL(50)
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    IF_INT_NOT_EQUAL(LW(3), -1)
        ADD(LW(10), 1)
        GOTO(10)
    END_IF
    LABEL(80)
    CALL(btlevtcmd_StopAC)
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    LABEL(90)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PTR_A1_1"))
    WAIT_FRAMES(48)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL_CHILD(btldefaultevt_SuitoruBadgeEffect)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

EVT_BEGIN(vivian_shadow_tail_event)
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetUnitWork, -2, 0, LW(0))
    CALL(btlevtcmd_SetUnitWork, -2, 1, LW(1))
    CALL(btlevtcmd_SetUnitWork, -2, 2, LW(2))
    LABEL(0)
    CALL(btlevtcmd_GetUnitId, -2, LW(5))
    IF_INT_EQUAL(LW(5), -1)
        RETURN
    END_IF
    CALL(btlevtcmd_GetUnitWork, -2, 3, LW(5))
    IF_INT_EQUAL(LW(5), 0)
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_SetUnitWork, -2, 0, LW(0))
        CALL(btlevtcmd_SetUnitWork, -2, 1, LW(1))
        CALL(btlevtcmd_SetUnitWork, -2, 2, LW(2))
    END_IF
    WAIT_FRAMES(1)
    GOTO(0)
    RETURN
EVT_END()

EVT_BEGIN(vivian_counter_damage_event)
    CALL(btlevtcmd_SetScale, -2, FLOAT(1.0f), FLOAT(1.0f), FLOAT(1.0f))
    CALL(btlevtcmd_OnUnitFlag, -2, 1)
    IF_INT_EQUAL(LW(0), 43)
        SET_VALUE(LW(14), 36)
    ELSE
        SET_VALUE(LW(14), 39)
    END_IF
    CALL(btlevtcmd_AnimeChangePoseType, -2, LW(11), LW(14))
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(1.0f))
    CALL(btlevtcmd_GetPos, -2, LW(2), LW(3), LW(4))
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(2), 25)
    ADD(LW(3), 25)
    CALL(btlevtcmd_DivePosition, -2, LW(2), LW(3), LW(4), 5, 0, 0, 0, -1)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_GetUnitWork, -2, 3, LW(5))
    IF_INT_NOT_EQUAL(LW(5), 0)
        CALL(btlevtcmd_GetUnitWork, -2, 0, LW(0))
        CALL(btlevtcmd_GetUnitWork, -2, 1, LW(1))
        CALL(btlevtcmd_GetUnitWork, -2, 2, LW(2))
        CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_DivePosition, -2, LW(0), LW(1), LW(2), 12, 0, 5, 0, -1)
        CALL(btlevtcmd_SetUnitWork, -2, 3, 0)
        CALL(battle_evt_majo_disp_off, -2, 1, 0, 0)
        CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetPos, -2, EVT_NULLPTR, LW(1), EVT_NULLPTR)
        CALL(btlevtcmd_DivePosition, -2, LW(0), LW(1), LW(2), 80, 0, 5, 0, -1)
        CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
        CALL(battle_evt_majo_disp_on, -2, 1, LW(0), LW(1), LW(2), 0)
        CALL(btlevtcmd_SetPos, -2, LW(0), LW(1), LW(2))
    ELSE
        CALL(btlevtcmd_GetHomePos, -2, LW(2), LW(3), LW(4))
        CALL(btlevtcmd_DivePosition, -2, LW(2), LW(3), LW(4), 5, 0, 0, 0, -1)
    END_IF
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

EVT_BEGIN(vivian_appeal_event)
    CALL(btlevtcmd_RunDataEventChild, -2, 7)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PTR_O_1"))
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_MARIO_APPEAL1"), LW(0), LW(1), LW(2), 0)
    WAIT_FRAMES(10)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PTR_O_2"))
    WAIT_MS(333)
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, -2, LW(0), 15)
    ADD(LW(1), 30)
    CALL(btlevtcmd_GetFaceDirection, -2, LW(14))
    MULTIPLY(LW(14), 45)
    CALL(evt_eff, STRING(""), STRING("kiss"), 0, LW(0), LW(1), LW(2), LW(14), 0, 0, 0, 0, 0, 0, 0)
    WAIT_MS(333)
    CALL(btlevtcmd_AudienceAppeal, -2)
    WAIT_MS(500)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(party_win_reaction)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PTR_Y_1"))
    CALL(btlevtcmd_AnimeWaitPlayComplete, -2, 1)
    WAIT_MS(500)
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 43)
    RETURN
EVT_END()

static EVT_BEGIN(attack_audience_event)
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetStageSize, LW(3), EVT_NULLPTR, EVT_NULLPTR)
    CALL(btlevtcmd_GetFaceDirection, -2, LW(4))
    SET_VALUE(LW(8), LW(3))
    MULTIPLY(LW(8), LW(4))
    MULTIPLY(LW(8), -1)
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(6.0f))
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 42)
    CALL(btlevtcmd_MovePosition, -2, LW(8), LW(1), LW(2), 0, -1, 0)
    WAIT_FRAMES(20)
    CALL(_get_attack_aud_no, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetStageAudienceAreaSize, LW(4), EVT_NULLPTR, EVT_NULLPTR)
    CALL(btlevtcmd_GetFaceDirection, -2, LW(5))
    SET_VALUE(LW(6), LW(4))
    MULTIPLY(LW(6), LW(5))
    MULTIPLY(LW(6), -1)
    CALL(btlevtcmd_SetPos, -2, LW(6), LW(1), LW(2))
    CALL(evt_btl_camera_set_mode, 0, 13)
    CALL(evt_btl_camera_set_homing_unit_audience, 0, -2, 1, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 100)
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 20)
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PTR_A1_2"))
    CALL(btlevtcmd_ftomsec, 4, LW(0))
    WAIT_MS(LW(0))
    CALL(evt_snd_sfxon, STRING("SFX_BTL_VIVIAN_ATTACK4"), 0)
    CALL(_attack_aud, LW(3), 0)
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        WAIT_MS(1500)
        CALL(evt_btl_camera_set_mode, 0, 0)
        WAIT_MS(1000)
    END_SIBLING_EVENT
    WAIT_MS(500)
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 42)
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(4.0f))
    CALL(btlevtcmd_MovePosition, -2, LW(6), LW(1), LW(2), 0, -1, 0)
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPos, -2, LW(8), LW(1), LW(2))
    CALL(btlevtcmd_WaitEventEnd, LW(15))
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()


#define intplGetValue(mode, a, b, cur, steps) intplGetValue(mode, a, b, cur, steps)

USER_FUNC(battle_evt_majo_disp_off) {
    EvtWord* args = event->args;
    BattleUnit* unit;
    BattleUnitPart* part;
    s32 id, partNo, flag, flag2;
    s32 pose;

    id = BattleTransID(event, evtGetValue(event, args[0]));
    partNo = evtGetValue(event, args[1]);
    flag = evtGetValue(event, args[2]);
    flag2 = evtGetValue(event, args[3]);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    part = BattleGetUnitPartsPtr(id, partNo);
    pose = part->poseId;
    if (isFirstCall) {
        event->userdata[0] = 0;
        event->userdata[1] = 0;
        animEffectAsync("p_bibi", 1);
    }
    switch (event->userdata[0]) {
        case 0:
            part->shadowScale = intplGetValue(0, 1.0f, 2.0f, event->userdata[1], 30);
            if (flag != 0) {
                unit->flags |= 0x10000;
            }
            event->userdata[1]++;
            if (event->userdata[1] > 15) {
                event->userdata[0]++;
            }
            break;
        case 1:
            if (event->userdata[1] <= 30) {
                part->shadowScale = intplGetValue(0, 1.0f, 2.0f, event->userdata[1], 30);
                event->userdata[1]++;
            }
            if (!animEffectAsync("p_bibi", 1)) {
                return EVT_RETURN_BLOCK;
            }
            animPoseSetEffect(pose, "p_bibi", 1);
            animPoseSetEffectAnim(pose, "PM_B_1", 1);
            animPoseSetLocalTime(animPoseGetAnimPosePtr(pose)->paperId, 8.0f);
            event->userdata[0]++;
            break;
        case 2:
            if (event->userdata[1] <= 30) {
                part->shadowScale = intplGetValue(0, 1.0f, 2.0f, event->userdata[1], 30);
                event->userdata[1]++;
            }
            if (animPoseGetLoopTimes(animPoseGetAnimPosePtr(pose)->paperId) >= 1.0f) {
                event->userdata[0]++;
                event->userdata[1] = 0;
                psndSFXOn_3D("SFX_BTL_VIVIAN_SHADOW1_2", &unit->mPosition);
                if (flag2 != 0) {
                    part->attributes |= 0x1000000;
                }
            }
            break;
        case 3:
            part->shadowScale = intplGetValue(0, 2.0f, 0.0f, event->userdata[1], 20);
            event->userdata[1]++;
            if (event->userdata[1] > 20) {
                part->shadowScale = 0.0f;
                unit->mPosition.y -= 500.0f;
                animPoseSetEffect(pose, NULL, 1);
                animPoseSetPaperAnimGroup(pose, NULL, 1);
                return EVT_RETURN_DONE;
            }
            break;
    }
    return EVT_RETURN_BLOCK;
}

USER_FUNC(_vivian_make_extra_work_area) {
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    s32 id;

    id = BattleTransID(event, -2);
    unit = BattleGetUnitPtr(wp, id);
    unit->countersetWeapon = BattleAlloc(sizeof(BattleWeapon));
    return EVT_RETURN_DONE;
}

static void __makeTechMenuFunc(BattleCommand* command, s32* count) {
    s32 techLv;

    techLv = partyGetTechLv(BattleTransPartyId(BattleGetPartyPtr(_battleWorkPointer)->currentKind));

#define ADD_W(w)                                                                                   \
    command->weapon[*count].index = -1;                                                     \
    command->weapon[*count].itemId = 0;                                                       \
    command->weapon[*count].weapon = (BattleWeapon*)(w);                              \
    command->weapon[*count].enabled = 0;                                                   \
    command->weapon[*count].icon = command->weapon[*count].weapon->icon;       \
    command->weapon[*count].name = msgSearch(command->weapon[*count].weapon->name); \
    (*count)++;
    ADD_W(&partyWeapon_VivianNormalAttack)
    if (techLv >= 0) {
        ADD_W(&partyWeapon_VivianShadowGuard)
    }
    if (techLv >= 1) {
        ADD_W(&partyWeapon_VivianMagicalPowder)
    }
    if (techLv >= 2) {
        ADD_W(&partyWeapon_VivianCharmKissAttack)
    }
}

double sqrt(double);

USER_FUNC(_get_move_frame) {
    EvtWord* args = event->args;
    Vec a, b, d;
    f32 speed;
    s32 frames, out;

    a.x = evtGetFloat(event, args[0]);
    a.y = evtGetFloat(event, args[1]);
    a.z = evtGetFloat(event, args[2]);
    b.x = evtGetFloat(event, args[3]);
    b.y = evtGetFloat(event, args[4]);
    b.z = evtGetFloat(event, args[5]);
    speed = evtGetFloat(event, args[6]);
    out = args[7];
    PSVECSubtract(&a, &b, &d);
    frames = (s32)((f32)sqrt(d.x * d.x + d.y * d.y + d.z * d.z) / speed);
    if (frames > 15) {
        frames = 15;
    }
    evtSetValue(event, out, frames);
    return EVT_RETURN_DONE;
}

f32 BattleGetFloorHeight(BattleWork* wp, f32 x, f32 y, f32 z);

USER_FUNC(_get_vivian_attack_position) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    BattleUnit* self;
    BattleUnit* target;
    s32 targetId, x, y, z;
    s32 i1, i2, i3;
    s32 ix, iy;
    f32 floor;
    f32 sx, sy, sz;

    targetId = evtGetValue(event, args[0]);
    i1 = args[1];
    x = evtGetValue(event, i1);
    i2 = args[2];
    y = evtGetValue(event, i2);
    i3 = args[3];
    z = evtGetValue(event, i3);
    self = BattleGetUnitPtr(wp, BattleTransID(event, -2));
    target = BattleGetUnitPtr(wp, BattleTransID(event, targetId));
    BtlUnit_GetScale(target, &sx, &sy, &sz);
    ix = (s32)((f32)target->faceDirection * (target->mKindParams->vivianOffsetX * sx));
    iy = (s32)(target->mKindParams->vivianOffsetZ * sz);
    {
        x += -30.0f * self->mSizeMultiplier * (f32)self->faceDirection + ix;
        z = z + iy;
        y -= 20.0f * self->mSizeMultiplier;
    }
    floor = BattleGetFloorHeight(wp, x, y, z);
    if (floor > y) {
        y = (s32)floor;
    }
    evtSetValue(event, i1, x);
    evtSetValue(event, i2, y);
    evtSetValue(event, i3, z);
    return EVT_RETURN_DONE;
}

USER_FUNC(_make_kagenuke_weapon) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    BattleUnit* unit;
    s32 index;
    BattleWeapon* weapon;
    void* src;
    s32 id, value;

    id = BattleTransID(event, -2);
    unit = BattleGetUnitPtr(wp, id);
    index = args[0];
    weapon = unit->countersetWeapon;
    src = (void*)evtGetValue(event, index);
    value = evtGetValue(event, args[1]);
    memcpy(weapon, src, sizeof(BattleWeapon));
    weapon->burnTime = value;
    evtSetValue(event, index, (EvtWord)weapon);
    return EVT_RETURN_DONE;
}


USER_FUNC(_disp_heart_entry) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    BattleUnit* unit;
    s32 id;

    id = BattleTransID(event, -2);
    unit = BattleGetUnitPtr(wp, id);
    if (isFirstCall) {
        event->heart.unit = unit;
        event->heart.cur = 0;
        event->heart.start = evtGetValue(event, args[0]);
        event->heart.end = evtGetValue(event, args[1]);
        event->heart.total = evtGetValue(event, args[2]);
        event->heart.stop = 0;
        event->heart.unitId = evtGetValue(event, args[3]);
        event->heart.partId = evtGetValue(event, args[4]);
    }
    if (event->heart.stop != 0) {
        return EVT_RETURN_DONE;
    }
    event->heart.cur++;
    dispEntry(CAMERA_3D, 0.0f, 1, (DispCallback)_disp_heart, event);
    return event->heart.cur < event->heart.total ? EVT_RETURN_BLOCK : EVT_RETURN_DONE;
}

void _disp_heart(CameraId camera, void* param) {
    EventEntry* w;
    f32 scale;
    Vec p, b, a, c;
    BattleUnit* unit;

    if (param == NULL) {
        return;
    }
    w = param;
    if (w->heart.unit != NULL) {
        {
            if (w->heart.cur <= w->heart.start) {
                scale = intplGetValue(5, 0.0f, 1.2f, w->heart.cur, w->heart.start);
            } else if (w->heart.cur <= w->heart.end) {
                scale = intplGetValue(0xb, 1.2f, 1.0f, w->heart.cur - w->heart.start, w->heart.end - w->heart.start);
            } else {
                scale = 1.0f;
            }
            unit = BattleGetUnitPtr(_battleWorkPointer, BattleTransID(w, w->heart.unitId));
            BtlUnit_GetHitPos(unit, BtlUnit_GetPartsPtr(unit, w->heart.partId), &p.x, &p.y, &p.z);
            p.z += 10.0f;
            a = p;
            b = p;
            c = p;
            b.y = 16.0f * scale + a.y;
            b.z = 5.0f + a.z;
            iconDispGx(&a, 0, ICON_1F2, scale);
            iconDispGx(&b, 0, ICON_189, scale);
        }
    }
}

USER_FUNC(_disp_heart_entry_stop) {
    EventEntry* e = evtGetPtrID(evtGetValue(event, *event->args));
    e->heart.stop = 1;
    return EVT_RETURN_DONE;
}

USER_FUNC(_disp_heart_entry_stop_check) {
    evtSetValue(event, *event->args, event->heart.stop);
    return EVT_RETURN_DONE;
}


