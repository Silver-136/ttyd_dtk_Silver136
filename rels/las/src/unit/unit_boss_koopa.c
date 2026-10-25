#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_msg.h"
#include "evt/evt_sub.h"

#include "driver/icondrv.h"
#include "driver/camdrv.h"
#include "battle/battle.h"
#include "manager/evtmgr_cmd.h"
#include "driver/dispdrv.h"
#include "system.h"
#include "battle/battle_event_default.h"
#include "battle/battle_message.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_eff.h"
#include "party/party_motion.h"
#include "battle/battle_database_common.h"
#include "battle/battle_sub.h"

typedef struct IconJumpWork { Vec start; Vec pos; Vec end; s32 frames; f32 speed; f32 gravity; f32 vy; u8 pad[0x7C - 0x34]; } IconJumpWork;
static void _disp_icon(CameraId camera, void* param);

USER_FUNC(btlevtcmd_SetEventEntry);
USER_FUNC(btlevtcmd_SetEventPhase);
static USER_FUNC(_icon_jump_icon);
static USER_FUNC(party_cannot_use_command);
static USER_FUNC(mario_cannot_use_command);
extern BattleUnitKind unit_boss_kamec_obaba;

BattleUnitKind unit_boss_koopa;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static PoseTableEntry pose_table[18];
static PoseTableEntry pose_table_forward_leaning[18];
static PoseTableEntry pose_table_dead[3];
static BattleDataEntry data_table[2];
static BattleUnitKindPart parts[1];
static BattleWeapon weapon_koopa_hip_attack;
static BattleWeapon weapon_koopa_bite_attack;
static BattleWeapon weapon_koopa_critical_bite_attack;
static BattleWeapon weapon_koopa_fire_attack;
BattleUnitSetup entry_kamec_obaba;
static EvtWord koopa_zoom[25];
static EvtWord kamec_obaba_zoom[40];
static EvtWord init_event[124];
static EvtWord entry_event[55];
static EvtWord damage_event[22];
static EvtWord dead_event[172];
static EvtWord phase_event[134];
static EvtWord harf_hp_event[94];
static EvtWord attack_event[110];
static EvtWord hip_attack_event[VERSION_PICK(431, 432)];
static EvtWord bite_attack_event[278];
static EvtWord critical_bite_attack_event[261];
static EvtWord fire_attack_event[356];
EvtWord first_mario_damage_check[33];
EvtWord first_mario_damage_event[114];
static EvtWord wait_event[6];
static const char* cmd_disable_string[5];

BattleUnitKind unit_boss_koopa = {
    144,
    "btl_un_koopa",
    70,
    0,
    1,
    1,
    68,
    0,
    0,
    100,
    0,
    50,
    6,
    86,
    80,
    0,
    70,
    0.0f,
    40.0f,
    0.0f,
    0,
    0,
    40.0f,
    40.0f,
    0.0f,
    43.0f,
    0.0f,
    -10.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    21.0f,
    0.0f,
    0.0f,
    36.0f,
    72.0f,
    0.0f,
    0.0f,
    40.0f,
    0.0f,
    86.0f,
    80.0f,
    0,
    0,
    0,
    2,
    100,
    20,
    0.0f,
    0.0f,
    0.0f,
    "SFX_BOSS_KOOPA_DAMAGED1",
    "SFX_BTL_DAMAGE_FIRE1",
    "SFX_BTL_DAMAGE_ICE1",
    "SFX_BTL_DAMAGE_BIRIBIRI1",
    0,
    &regist,
    1,
    parts,
    init_event,
    data_table,
};

static BattleUnitDefense defence = {{2, 2, 2, 2, 2}};

static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

static StatusVulnerability regist = {30, 30, 30, 100, 40, 100, 100, 25, 100, 75, 100, 75, 100, 80, 30, 0, 0, 100, 30, 100, 100, 5};

static PoseTableEntry pose_table[] = {
    {1, "KPA_N_3"},
    {2, "KPA_Z_6"},
    {9, "KPA_Z_6"},
    {5, "KPA_K_2"},
    {4, "KPA_S_2"},
    {3, "KPA_S_2"},
    {28, "KPA_S_1"},
    {29, "KPA_Q_2"},
    {30, "KPA_Q_2"},
    {31, "KPA_D_1"},
    {39, "KPA_D_1"},
    {50, "KPA_S_1"},
    {42, "KPA_R_1"},
    {40, "KPA_W_1"},
    {56, "KPA_S_1"},
    {57, "KPA_S_1"},
    {65, "KPA_T_1"},
    {69, "KPA_S_1"},
};

static PoseTableEntry pose_table_forward_leaning[] = {
    {1, "KPA_N_2"},
    {2, "KPA_Z_5"},
    {9, "KPA_Z_5"},
    {5, "KPA_K_1"},
    {4, "KPA_X_1"},
    {3, "KPA_X_1"},
    {28, "KPA_S_8"},
    {29, "KPA_D_4"},
    {30, "KPA_D_4"},
    {31, "KPA_D_4"},
    {39, "KPA_D_4"},
    {50, "KPA_S_8"},
    {42, "KPA_R_1"},
    {40, "KPA_W_1"},
    {56, "KPA_S_8"},
    {57, "KPA_S_8"},
    {65, "KPA_T_9"},
    {69, "KPA_S_8"},
};

static PoseTableEntry pose_table_dead[] = {
    {28, "KPA_Z_3"},
    {39, "KPA_Z_3"},
    {69, "KPA_Z_3"},
};

static BattleDataEntry data_table[] = {
    {49, dead_event},
    {0, NULL},
};

static BattleUnitKindPart parts[] = {
    {1, "btl_un_koopa", "c_koopa", 0.0f, 0.0f, 0.0f, 34.0f, 62.0f, 0.0f, 22.0f, 78.0f, 0.0f, 70, 80, 255, &defence, &defence_attr, 0x9, 0, pose_table},
};

static BattleWeapon weapon_koopa_hip_attack = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, // baseFpCost..baseSpCost
    1, // superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    1, // reserved_19
    1, // mBingoSlotChance
    1, // reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {7, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x20001004, // targetPropertyFlags
    0, // element
    10, // damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    0x1087, // special
    0xC, // counterResistFlags
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

static BattleWeapon weapon_koopa_bite_attack = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, // baseFpCost..baseSpCost
    1, // superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    1, // reserved_19
    1, // mBingoSlotChance
    1, // reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {7, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x21002000, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    0x87, // special
    2, // counterResistFlags
    4, // targetWeightFlags
    0, 0, 0, 0, 0, 0, // sleepChance..dizzyTime
    30, // poisonChance
    10, // poisonTime
    1, // poisonStrength
    0, 0, 0, 0, 0, 0, 0, 0, // confuseChance..burnTime
    0, 0, 0, 0, 0, 0, 0, 0, // freezeChance..atkChangeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // defChangeChance..fastChance
    0, 0, 0, 0, 0, 0, 0, 0, // fastTime..invisibleChance
    0, 0, 0, 0, 0, // invisibleTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

static BattleWeapon weapon_koopa_critical_bite_attack = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, // baseFpCost..baseSpCost
    1, // superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    1, // reserved_19
    1, // mBingoSlotChance
    1, // reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {2, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x21002000, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    0x87, // special
    2, // counterResistFlags
    4, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

static BattleWeapon weapon_koopa_fire_attack = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, // baseFpCost..baseSpCost
#ifdef VERSION_G8ME01
    2, // superguardsAllowed
#else
    0, // superguardsAllowed
#endif
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    1, // reserved_19
    1, // mBingoSlotChance
    1, // reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {7, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x20000000, // targetPropertyFlags
    1, // element
    0, // damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    0x47, // special
    0x7FF, // counterResistFlags
    0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
    0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
    0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
    0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
    0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

BattleUnitSetup entry_kamec_obaba = {
    &unit_boss_kamec_obaba, // mUnitKindParams
    1, // alliance
    {0}, // pad_05
    0x4000004, // attackPhase
    {1000.0f, 0.0f, 1000.0f}, // position
    0, // hitOffsetX
    {0}, // unitWork
    NULL, // itemDropTable
};

static EVT_BEGIN(koopa_zoom)
    CALL(evt_btl_camera_set_mode, 0, 3)
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(evt_btl_camera_set_moveto, 0, LW(0), 95, 350, LW(0), 45, -300, 30, 11)
    WAIT_FRAMES(37)
    RETURN
EVT_END()

static EVT_BEGIN(kamec_obaba_zoom)
    CALL(btlevtcmd_GetUnitWork, GW(9), 0, LW(1))
    CALL(evt_btl_camera_set_mode, 0, 3)
    IF_INT_EQUAL(LW(1), 1)
        CALL(evt_btl_camera_set_moveto, 0, 135, 106, 296, 135, 55, -402, 30, 11)
        ELSE
        CALL(evt_btl_camera_set_moveto, 0, 140, 34, 296, 140, 50, -402, 30, 11)
    END_IF
    WAIT_FRAMES(37)
    RETURN
EVT_END()

static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, -2, PTR(wait_event))
    CALL(btlevtcmd_SetEventPhase, -2, PTR(phase_event))
    CALL(btlevtcmd_SetEventAttack, -2, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, -2, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, -2, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_SetEventEntry, -2, PTR(entry_event))
    CALL(btlevtcmd_SpawnUnit, GW(9), PTR(&entry_kamec_obaba), 0)
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 60)
    ADD(LW(1), 40)
    ADD(LW(2), 10)
    CALL(btlevtcmd_SetPos, GW(9), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetHomePos, GW(9), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetUnitId, -2, LW(0))
    SET_VALUE(GW(8), LW(0))
    CALL(btlevtcmd_SetUnitWork, -2, 0, 0)
    CALL(btlevtcmd_SetUnitWork, -2, 1, 0)
    CALL(btlevtcmd_GetMaxHp, -2, LW(0))
    DIVIDE(LW(0), 2)
    CALL(btlevtcmd_SetUnitWork, -2, 2, LW(0))
    CALL(btlevtcmd_SetUnitWork, -2, 3, 0)
    CALL(btlevtcmd_SetUnitWork, -2, 4, 0)
    SET_VALUE(GW(10), 0)
    SET_VALUE(GW(11), 0)
    CALL(btlevtcmd_SetWalkSound, -2, STRING("SFX_BOSS_KOOPA_MOVE1L"), STRING("SFX_BOSS_KOOPA_MOVE1R"), 0, 15, 15)
    CALL(btlevtcmd_SetRunSound, -2, STRING("SFX_BOSS_KOOPA_MOVE1L"), STRING("SFX_BOSS_KOOPA_MOVE1R"), 0, 8, 8)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(entry_event)
    CALL(btlevtcmd_StatusWindowOnOff, 0)
    WAIT_FRAMES(60)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("KPA_S_1"))
    WAIT_FRAMES(60)
    CALL_CHILD(koopa_zoom)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_VOICE_KOOPA_LAUGH2_1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("KPA_Y_2"))
    WAIT_MS(1000)
    CALL(evt_msg_print, 2, STRING("stg8_las_96"), 0, -2)
    WAIT_MS(300)
    CALL_CHILD(kamec_obaba_zoom)
    CALL(evt_msg_print, 2, STRING("stg8_las_97"), 0, GW(9))
    WAIT_MS(300)
    CALL(btlevtcmd_StatusWindowOnOff, 1)
    CALL(evt_btl_camera_set_mode, 0, 0)
    RETURN
EVT_END()

static EVT_BEGIN(damage_event)
    CALL(btlevtcmd_CheckDamageCode, -2, 1024, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(GW(10), 1)
    END_IF
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    RETURN
EVT_END()

static EVT_BEGIN(dead_event)
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetPos, -2, LW(3), LW(4), LW(5))
    SET_VALUE(LW(6), 1)
    IF_INT_EQUAL(LW(0), LW(3))
        IF_INT_EQUAL(LW(1), LW(4))
            IF_INT_EQUAL(LW(2), LW(5))
                SET_VALUE(LW(6), 0)
            END_IF
        END_IF
    END_IF
    IF_INT_EQUAL(LW(6), 1)
        CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(6.0f))
        CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 42)
        CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
        CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 43)
        CALL(btlevtcmd_ResetFaceDirection, -2)
    END_IF
    WAIT_FRAMES(1)
    CALL(btlevtcmd_WaitAttackEnd)
    CALL(btlevtcmd_WaitAttackEnd)
    CALL(btlevtcmd_StatusWindowOnOff, 0)
    CALL_CHILD(koopa_zoom)
    SWITCH(GW(11))
        CASE_EQUAL(0)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_VOICE_KOOPA_SATIATED2_2"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("KPA_D_2"))
        WAIT_MS(200)
        CALL(btlevtcmd_SetTalkPose, -2, STRING("KPA_T_8"))
        CALL(btlevtcmd_SetStayPose, -2, STRING("KPA_Z_3"))
        CALL(evt_msg_print, 2, STRING("stg8_las_100_02"), 0, -2)
        WAIT_MS(300)
        CALL(btlevtcmd_AnimeSetPoseTable, -2, 1, PTR(pose_table_dead))
        SET_VALUE(GW(10), 2)
        SET_VALUE(LW(10), -2)
        CALL_CHILD(subsetevt_dead_core_nospin_norotate)
        CASE_DEFAULT
        CALL(btlevtcmd_SetTalkPose, -2, STRING("KPA_D_1"))
        CALL(btlevtcmd_SetStayPose, -2, STRING("KPA_D_1"))
        CALL(evt_msg_print, 2, STRING("stg8_las_100_04"), 0, -2)
        WAIT_MS(300)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_VOICE_KOOPA_SATIATED2_2"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("KPA_D_2"))
        WAIT_MS(700)
        CALL(btlevtcmd_AnimeSetPoseTable, -2, 1, PTR(pose_table_dead))
        SET_VALUE(GW(10), 2)
        SET_VALUE(LW(10), -2)
        CALL_CHILD(subsetevt_dead_core_nospin_norotate)
    END_SWITCH
    CALL(btlevtcmd_StatusWindowOnOff, 1)
    CALL(evt_btl_camera_set_mode, 0, 0)
    RETURN
EVT_END()

static EVT_BEGIN(phase_event)
    CALL(btlevtcmd_CheckStatus, -2, 27, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_get_turn, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        GOTO(90)
    END_IF
    CALL(btlevtcmd_CheckPhase, LW(0), 67108865)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckActStatus, -2, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_PhaseEventStartDeclare, -2)
            CALL(btlevtcmd_GetUnitWork, -2, 0, LW(0))
            IF_INT_EQUAL(LW(0), 0)
                CALL(evt_sub_random, 99, LW(1))
                IF_INT_LESS(LW(1), 0)
                    CALL(btlevtcmd_SetUnitWork, -2, 0, 1)
                    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("KPA_U_2"))
                    WAIT_MS(200)
                    CALL(btlevtcmd_OnPartsCounterAttribute, -2, 1, 1)
                    CALL(btlevtcmd_AnimeSetPoseTable, -2, 1, PTR(pose_table_forward_leaning))
                END_IF
                ELSE
                CALL(evt_sub_random, 99, LW(1))
                IF_INT_LESS(LW(1), 100)
                    CALL(btlevtcmd_SetUnitWork, -2, 0, 0)
                    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("KPA_U_3"))
                    WAIT_MS(200)
                    CALL(btlevtcmd_OffPartsCounterAttribute, -2, 1, 1)
                    CALL(btlevtcmd_AnimeSetPoseTable, -2, 1, PTR(pose_table))
                END_IF
            END_IF
            CALL(btlevtcmd_StartWaitEvent, -2)
        END_IF
    END_IF
    LABEL(90)
    CALL(btlevtcmd_CheckPhase, LW(0), 67108867)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_PhaseEventStartDeclare, -2)
        CALL_CHILD(PTR(harf_hp_event))
        CALL(btlevtcmd_StartWaitEvent, -2)
    END_IF
    LABEL(99)
    RETURN
EVT_END()

static EVT_BEGIN(harf_hp_event)
    CALL(btlevtcmd_GetUnitWork, -2, 3, LW(1))
    IF_INT_EQUAL(LW(1), 0)
        CALL(btlevtcmd_GetHp, -2, LW(0))
        CALL(btlevtcmd_GetUnitWork, -2, 2, LW(1))
        IF_INT_LESS(LW(0), LW(1))
            CALL(btlevtcmd_SetUnitWork, -2, 3, 1)
            CALL(btlevtcmd_GetUnitWork, -2, 0, LW(0))
            IF_INT_EQUAL(LW(0), 1)
                CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("KPA_U_3"))
                WAIT_MS(200)
            END_IF
            CALL(btlevtcmd_StatusWindowOnOff, 0)
            CALL_CHILD(PTR(koopa_zoom))
            CALL(btlevtcmd_SetTalkPose, -2, STRING("KPA_T_3"))
            CALL(btlevtcmd_SetStayPose, -2, STRING("KPA_S_4"))
            CALL(evt_msg_print, 2, STRING("stg8_las_100"), 0, -2)
            WAIT_MS(300)
            CALL(btlevtcmd_StatusWindowOnOff, 1)
            CALL(evt_btl_camera_set_mode, 0, 0)
            CALL(btlevtcmd_GetUnitWork, -2, 0, LW(0))
            IF_INT_EQUAL(LW(0), 1)
                CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("KPA_U_2"))
                WAIT_MS(200)
            END_IF
            CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 43)
        END_IF
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_EnemyItemUseCheck, -2, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, -2)
        RETURN
    END_IF
    CALL(btlevtcmd_GetUnitWork, -2, 4, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        SET_VALUE(LW(9), PTR(&weapon_koopa_fire_attack))
        CALL_CHILD(PTR(fire_attack_event))
        CALL(btlevtcmd_SetUnitWork, -2, 4, 1)
        CASE_EQUAL(1)
        SET_VALUE(LW(9), PTR(&weapon_koopa_bite_attack))
        CALL_CHILD(PTR(bite_attack_event))
        CALL(btlevtcmd_SetUnitWork, -2, 4, 2)
        CASE_EQUAL(2)
        SET_VALUE(LW(9), PTR(&weapon_koopa_hip_attack))
        CALL_CHILD(PTR(hip_attack_event))
        CALL(btlevtcmd_SetUnitWork, -2, 4, 3)
        CASE_DEFAULT
        CALL(btlevtcmd_DrawLots, LW(0), 4, 20, 20, 0, 40)
        SWITCH(LW(0))
            CASE_EQUAL(0)
            SET_VALUE(LW(9), PTR(&weapon_koopa_hip_attack))
            CALL_CHILD(PTR(hip_attack_event))
            CASE_EQUAL(1)
            SET_VALUE(LW(9), PTR(&weapon_koopa_bite_attack))
            CALL_CHILD(PTR(bite_attack_event))
            CASE_EQUAL(2)
            SET_VALUE(LW(9), PTR(&weapon_koopa_critical_bite_attack))
            CALL_CHILD(PTR(critical_bite_attack_event))
            CASE_EQUAL(3)
            SET_VALUE(LW(9), PTR(&weapon_koopa_fire_attack))
            CALL_CHILD(PTR(fire_attack_event))
        END_SWITCH
    END_SWITCH
    LABEL(90)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL_CHILD(PTR(first_mario_damage_event))
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

#ifdef VERSION_G8ME01
static EVT_BEGIN(hip_attack_event)
    CALL(btlevtcmd_GetEnemyBelong, -2, LW(0))
    CALL(btlevtcmd_SamplingEnemy, -2, LW(0), LW(9))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(9), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, -2, 16, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
        END_IF
        RETURN
    END_IF
    CALL(btlevtcmd_GetUnitKind, LW(3), LW(0))
    CALL(btlevtcmd_SetUnitWork, -2, 5, LW(0))
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
    CALL(btlevtcmd_AttackDeclare, -2, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, -2, LW(9))
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 40)
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, -2, LW(0), 40)
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(2.0f))
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_KOOPA_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("KPA_A_2A"))
    WAIT_MS(300)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("KPA_A_2B"))
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(25)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("KPA_A_2C"))
        WAIT_FRAMES(15)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("KPA_A_2D"))
    END_SIBLING_EVENT
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.25f))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    SUBTRACT(LW(2), 20)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 50, -1)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_KOOPA_HIP_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_PreCheckDamage, -2, LW(3), LW(4), LW(9), 256, LW(5))
    SWITCH(LW(5))
        CASE_OR_EQUAL(4)
        GOTO(90)
        CASE_END
        CASE_EQUAL(3)
        CALL(btlevtcmd_StartAvoid, LW(3), 38)
        GOTO(90)
        CASE_END
        CASE_EQUAL(6)
        CALL(btlevtcmd_StartAvoid, LW(3), 39)
        GOTO(90)
        CASE_EQUAL(2)
        CALL(btlevtcmd_StartAvoid, LW(3), 40)
        GOTO(90)
        CASE_EQUAL(1)
        GOTO(91)
        CASE_END
        CASE_DEFAULT
        GOTO(98)
        CASE_END
    END_SWITCH
    LABEL(90)
    CALL(btlevtcmd_JumpContinue, -2)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("KPA_A_2E"))
    WAIT_MS(500)
    GOTO(98)
    LABEL(91)
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
    CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(9), 256, LW(5))
    CALL(evt_btl_camera_shake_h, 0, 3, 0, 50, 13)
    CALL(btlevtcmd_JumpContinue, -2)
    WAIT_MS(500)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("KPA_A_2E"))
    WAIT_MS(300)
    CALL(btlevtcmd_GetResultACDefence, LW(7))
    IF_INT_LESS_OR_EQUAL(LW(7), 3)
        SET_VALUE(LW(15), 0)
        CALL(btlevtcmd_GetUnitWork, -2, 5, LW(0))
        SWITCH(LW(0))
            CASE_EQUAL(222)
            CALL(mario_cannot_use_command, LW(15))
            CASE_BETWEEN(224, 230)
            CALL(party_cannot_use_command, LW(15), LW(3))
            CASE_DEFAULT
            SET_VALUE(LW(0), -1)
        END_SWITCH
        IF_INT_NOT_EQUAL(LW(0), -1)
            IF_INT_NOT_EQUAL(LW(15), 0)
                BEGIN_INLINE_EVENT
                    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
                    CALL(btlevtcmd_GetPos, -2, LW(5), LW(6), LW(7))
                    CALL(btlevtcmd_FaceDirectionSub, LW(3), LW(5), 60)
                    CALL(btlevtcmd_TransStageFloorPosition, LW(5), LW(6), LW(7))
                    CALL(_icon_jump_icon, LW(15), LW(0), LW(1), LW(2), LW(5), LW(6), LW(7), FLOAT(0.5f), 30)
                    SET_VALUE(LW(0), LW(5))
                    SET_VALUE(LW(1), LW(6))
                    SET_VALUE(LW(2), LW(7))
                    CALL(btlevtcmd_FaceDirectionSub, LW(3), LW(5), 40)
                    CALL(_icon_jump_icon, LW(15), LW(0), LW(1), LW(2), LW(5), LW(6), LW(7), FLOAT(0.5f), 20)
                    SET_VALUE(LW(0), LW(5))
                    SET_VALUE(LW(1), LW(6))
                    SET_VALUE(LW(2), LW(7))
                    CALL(btlevtcmd_FaceDirectionSub, LW(3), LW(5), 25)
                    CALL(_icon_jump_icon, LW(15), LW(0), LW(1), LW(2), LW(5), LW(6), LW(7), FLOAT(0.5f), 12)
                END_INLINE_EVENT
            END_IF
            BEGIN_SIBLING_EVENT
                CALL(btlevtcmd_AnnounceMessage, 0, 0, 0, LW(0), 120)
            END_SIBLING_EVENT
        END_IF
    END_IF
    LABEL(98)
    CALL(evt_btl_camera_set_mode, 0, 0)
    WAIT_MS(250)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("KPA_R_1"))
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(4.0f))
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("KPA_S_1"))
    CALL_CHILD(first_mario_damage_check)
    RETURN
EVT_END()
#else
static EVT_BEGIN(hip_attack_event)
    CALL(btlevtcmd_GetEnemyBelong, -2, LW(0))
    CALL(btlevtcmd_SamplingEnemy, -2, LW(0), LW(9))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(9), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, -2, 16, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
        END_IF
        RETURN
    END_IF
    CALL(btlevtcmd_GetUnitKind, LW(3), LW(0))
    CALL(btlevtcmd_SetUnitWork, -2, 5, LW(0))
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
    CALL(btlevtcmd_AttackDeclare, -2, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, -2, LW(9))
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 40)
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, -2, LW(0), 40)
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(2.0f))
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_KOOPA_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("KPA_A_2A"))
    WAIT_MS(300)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("KPA_A_2B"))
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(25)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("KPA_A_2C"))
        WAIT_FRAMES(15)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("KPA_A_2D"))
    END_SIBLING_EVENT
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.25f))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    SUBTRACT(LW(2), 20)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 50, -1)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_KOOPA_HIP_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_PreCheckDamage, -2, LW(3), LW(4), LW(9), 256, LW(5))
    SWITCH(LW(5))
        CASE_OR_EQUAL(4)
        GOTO(90)
        CASE_END
        CASE_EQUAL(3)
        CALL(btlevtcmd_StartAvoid, LW(3), 38)
        GOTO(90)
        CASE_END
        CASE_EQUAL(6)
        CALL(btlevtcmd_StartAvoid, LW(3), 39)
        GOTO(90)
        CASE_EQUAL(2)
        CALL(btlevtcmd_StartAvoid, LW(3), 40)
        GOTO(90)
        CASE_EQUAL(1)
        GOTO(91)
        CASE_END
        CASE_DEFAULT
        GOTO(98)
        CASE_END
    END_SWITCH
    LABEL(90)
    CALL(btlevtcmd_JumpContinue, -2)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("KPA_A_2E"))
    WAIT_MS(500)
    GOTO(98)
    LABEL(91)
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
    CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(9), 256, LW(5))
    CALL(evt_btl_camera_shake_h, 0, 3, 0, 50, 13)
    CALL(btlevtcmd_JumpContinue, -2)
    WAIT_MS(500)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("KPA_A_2E"))
    WAIT_MS(300)
    CALL(btlevtcmd_GetResultACDefence, LW(7))
    IF_INT_LESS_OR_EQUAL(LW(7), 3)
        SET_VALUE(LW(15), 0)
        CALL(btlevtcmd_GetUnitWork, -2, 5, LW(0))
        SWITCH(LW(0))
            CASE_EQUAL(222)
            CALL(mario_cannot_use_command, LW(15))
            CASE_BETWEEN(224, 230)
            CALL(party_cannot_use_command, LW(15))
            CASE_DEFAULT
            SET_VALUE(LW(0), -1)
        END_SWITCH
        IF_INT_NOT_EQUAL(LW(0), -1)
            IF_INT_NOT_EQUAL(LW(15), 0)
                BEGIN_INLINE_EVENT
                    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
                    CALL(btlevtcmd_GetPos, -2, LW(5), LW(6), LW(7))
                    CALL(btlevtcmd_FaceDirectionSub, LW(3), LW(5), 60)
                    CALL(btlevtcmd_TransStageFloorPosition, LW(5), LW(6), LW(7))
                    CALL(_icon_jump_icon, LW(15), LW(0), LW(1), LW(2), LW(5), LW(6), LW(7), FLOAT(0.5f), 30)
                    SET_VALUE(LW(0), LW(5))
                    SET_VALUE(LW(1), LW(6))
                    SET_VALUE(LW(2), LW(7))
                    CALL(btlevtcmd_FaceDirectionSub, LW(3), LW(5), 40)
                    CALL(_icon_jump_icon, LW(15), LW(0), LW(1), LW(2), LW(5), LW(6), LW(7), FLOAT(0.5f), 20)
                    SET_VALUE(LW(0), LW(5))
                    SET_VALUE(LW(1), LW(6))
                    SET_VALUE(LW(2), LW(7))
                    CALL(btlevtcmd_FaceDirectionSub, LW(3), LW(5), 25)
                    CALL(_icon_jump_icon, LW(15), LW(0), LW(1), LW(2), LW(5), LW(6), LW(7), FLOAT(0.5f), 12)
                END_INLINE_EVENT
            END_IF
            BEGIN_SIBLING_EVENT
                CALL(btlevtcmd_AnnounceMessage, 0, 0, 0, LW(0), 120)
            END_SIBLING_EVENT
        END_IF
    END_IF
    LABEL(98)
    CALL(evt_btl_camera_set_mode, 0, 0)
    WAIT_MS(250)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("KPA_R_1"))
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(4.0f))
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("KPA_S_1"))
    CALL_CHILD(first_mario_damage_check)
    RETURN
EVT_END()
#endif

static EVT_BEGIN(bite_attack_event)
    CALL(btlevtcmd_GetEnemyBelong, -2, LW(0))
    CALL(btlevtcmd_SamplingEnemy, -2, LW(0), LW(9))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(9), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, -2, 16, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
        END_IF
        RETURN
    END_IF
    CALL(btlevtcmd_GetUnitKind, LW(3), LW(0))
    CALL(btlevtcmd_SetUnitWork, -2, 5, LW(0))
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
    CALL(btlevtcmd_AttackDeclare, -2, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, -2, LW(9))
    CALL(evt_btl_camera_set_mode, 0, 8)
    CALL(evt_btl_camera_set_homing_unit, 0, -2, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("KPA_R_1"))
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 65)
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(0), LW(2), 1, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(4.0f))
    CALL(btlevtcmd_MovePosition, -2, LW(0), 0, LW(2), 0, -1, 1)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("KPA_A_3A"))
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_KOOPA_BITE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
    WAIT_MS(170)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("KPA_A_3B"))
    WAIT_MS(150)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_KOOPA_BITE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_PreCheckDamage, -2, LW(3), LW(4), LW(9), 256, LW(5))
    SWITCH(LW(5))
        CASE_OR_EQUAL(4)
        GOTO(90)
        CASE_END
        CASE_EQUAL(3)
        CALL(btlevtcmd_StartAvoid, LW(3), 38)
        GOTO(90)
        CASE_END
        CASE_EQUAL(6)
        CALL(btlevtcmd_StartAvoid, LW(3), 39)
        GOTO(90)
        CASE_EQUAL(2)
        CALL(btlevtcmd_StartAvoid, LW(3), 40)
        GOTO(90)
        CASE_EQUAL(1)
        GOTO(91)
        CASE_END
        CASE_DEFAULT
        GOTO(98)
        CASE_END
    END_SWITCH
    LABEL(90)
    WAIT_MS(100)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("KPA_A_3C"))
    WAIT_MS(150)
    GOTO(98)
    LABEL(91)
    WAIT_MS(100)
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
    CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(9), 256, LW(5))
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("KPA_A_3C"))
    WAIT_MS(150)
    LABEL(98)
    CALL(evt_btl_camera_set_mode, 0, 0)
    WAIT_MS(250)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("KPA_R_1"))
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(4.0f))
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("KPA_S_1"))
    CALL_CHILD(first_mario_damage_check)
    RETURN
EVT_END()

static EVT_BEGIN(critical_bite_attack_event)
    CALL(btlevtcmd_GetEnemyBelong, -2, LW(0))
    CALL(btlevtcmd_SamplingEnemy, -2, LW(0), LW(9))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(9), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, -2, 16, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
        END_IF
        RETURN
    END_IF
    CALL(btlevtcmd_GetUnitKind, LW(3), LW(0))
    CALL(btlevtcmd_SetUnitWork, -2, 5, LW(0))
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
    CALL(btlevtcmd_AttackDeclare, -2, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, -2, LW(9))
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("KPA_R_1"))
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 65)
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(0), LW(2), 1, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(4.0f))
    CALL(btlevtcmd_MovePosition, -2, LW(0), 0, LW(2), 0, -1, 1)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("KPA_A_3A"))
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_KOOPA_BITE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
    WAIT_MS(170)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("KPA_A_3B"))
    WAIT_MS(150)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_KOOPA_BITE3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_PreCheckDamage, -2, LW(3), LW(4), LW(9), 256, LW(5))
    SWITCH(LW(5))
        CASE_OR_EQUAL(4)
        GOTO(90)
        CASE_END
        CASE_EQUAL(3)
        CALL(btlevtcmd_StartAvoid, LW(3), 38)
        GOTO(90)
        CASE_END
        CASE_EQUAL(6)
        CALL(btlevtcmd_StartAvoid, LW(3), 39)
        GOTO(90)
        CASE_EQUAL(2)
        CALL(btlevtcmd_StartAvoid, LW(3), 40)
        GOTO(90)
        CASE_EQUAL(1)
        GOTO(91)
        CASE_END
        CASE_DEFAULT
        GOTO(98)
        CASE_END
    END_SWITCH
    LABEL(90)
    WAIT_MS(100)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("KPA_A_3C"))
    WAIT_MS(150)
    GOTO(98)
    LABEL(91)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("KPA_A_4"))
    WAIT_MS(1500)
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
    CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(9), 256, LW(5))
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("KPA_A_3C"))
    WAIT_MS(150)
    LABEL(98)
    CALL(evt_btl_camera_set_mode, 0, 0)
    WAIT_MS(250)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("KPA_R_1"))
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(4.0f))
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL_CHILD(first_mario_damage_check)
    RETURN
EVT_END()

static EVT_BEGIN(fire_attack_event)
    CALL(btlevtcmd_GetEnemyBelong, -2, LW(0))
    CALL(btlevtcmd_SamplingEnemy, -2, LW(0), LW(9))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(9), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, -2, 16, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
        END_IF
        RETURN
    END_IF
    CALL(btlevtcmd_GetUnitKind, LW(3), LW(0))
    CALL(btlevtcmd_SetUnitWork, -2, 5, LW(0))
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
    CALL(btlevtcmd_AttackDeclareAll, -2)
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, -2, LW(9))
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("KPA_R_1"))
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 120)
    CALL(evt_btl_camera_set_mode, 0, 8)
    CALL(evt_btl_camera_set_homing_unit, 0, -2, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(0), LW(2), 1, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(4.0f))
    CALL(btlevtcmd_MovePosition, -2, LW(0), 0, LW(2), 0, -1, 1)
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
    WAIT_MS(170)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("KPA_A_1A"))
    WAIT_MS(300)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_VOICE_KOOPA_FIRE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_KOOPA_FIRE_BREATH1"), EVT_NULLPTR, 0, LW(15))
    BEGIN_INLINE_EVENT
        WAIT_MS(1500)
        CALL(evt_snd_sfxoff, LW(15))
    END_INLINE_EVENT
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("KPA_A_1B"))
    WAIT_MS(300)
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetStatusMg, -2, LW(5))
    SET_FLOAT_VALUE(LW(6), FLOAT(60.0f))
    MULTIPLY_FLOAT(LW(6), LW(5))
    CALL(btlevtcmd_FaceDirectionAdd, -2, LW(0), LW(6))
    SET_FLOAT_VALUE(LW(6), FLOAT(22.5f))
    MULTIPLY_FLOAT(LW(6), LW(5))
    ADD_FLOAT(LW(1), LW(6))
    SUBTRACT(LW(2), 5)
    MULTIPLY_FLOAT(LW(5), FLOAT(3.5f))
    CALL(evt_eff, STRING(""), STRING("gonbaba_breath"), 7, LW(0), LW(1), LW(2), LW(5), 90, 0, 0, 0, 0, 0, 0)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("KPA_A_1C"))
    WAIT_MS(300)
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    SET_VALUE(LW(10), 0)
    LABEL(10)
    CALL(btlevtcmd_PreCheckDamage, -2, LW(3), LW(4), LW(9), 256, LW(5))
    SWITCH(LW(5))
        CASE_OR_EQUAL(4)
        GOTO(90)
        CASE_END
        CASE_EQUAL(3)
        CALL(btlevtcmd_StartAvoid, LW(3), 38)
        GOTO(90)
        CASE_END
        CASE_EQUAL(6)
        CALL(btlevtcmd_StartAvoid, LW(3), 39)
        GOTO(90)
        CASE_EQUAL(2)
        CALL(btlevtcmd_StartAvoid, LW(3), 40)
        GOTO(90)
        CASE_EQUAL(1)
        GOTO(91)
        CASE_END
        CASE_DEFAULT
        GOTO(98)
        CASE_END
    END_SWITCH
    LABEL(90)
    GOTO(97)
    LABEL(91)
    IF_INT_EQUAL(LW(10), 0)
        CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
    END_IF
    CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(9), 256, LW(5))
    LABEL(97)
    CALL_CHILD(first_mario_damage_check)
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    IF_INT_NOT_EQUAL(LW(3), -1)
        ADD(LW(10), 1)
        GOTO(10)
    END_IF
    LABEL(98)
    WAIT_MS(1200)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("KPA_A_1D"))
    CALL(evt_btl_camera_set_mode, 0, 0)
    WAIT_MS(250)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("KPA_R_1"))
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(4.0f))
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    RETURN
EVT_END()

EVT_BEGIN(first_mario_damage_check)
    CALL(btlevtcmd_GetUnitWork, -2, 1, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetUnitWork, -2, 5, LW(0))
        IF_INT_EQUAL(LW(0), 222)
            CALL(btlevtcmd_GetTotalDamage, LW(3), LW(1))
            IF_INT_NOT_EQUAL(LW(1), 0)
                CALL(btlevtcmd_SetUnitWork, -2, 1, 1)
            END_IF
        END_IF
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(first_mario_damage_event)
    CALL(btlevtcmd_GetUnitWork, -2, 1, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(btlevtcmd_SetUnitWork, -2, 1, 2)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("KPA_S_1"))
        CALL(btlevtcmd_StatusWindowOnOff, 0)
        CALL_CHILD(koopa_zoom)
        CALL(btlevtcmd_GetUnitWork, -2, 0, LW(0))
        IF_INT_EQUAL(LW(0), 1)
            CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("KPA_U_3"))
            WAIT_MS(200)
        END_IF
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_VOICE_KOOPA_LAUGH2_1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("KPA_Y_2"))
        WAIT_MS(1000)
        CALL(btlevtcmd_SetTalkPose, -2, STRING("KPA_T_2"))
        CALL(btlevtcmd_SetStayPose, -2, STRING("KPA_S_3"))
        CALL(evt_msg_print, 2, STRING("stg8_las_98"), 0, -2)
        WAIT_MS(300)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("KPA_S_1"))
        IF_INT_EQUAL(GW(11), 0)
            CALL_CHILD(kamec_obaba_zoom)
            CALL(evt_msg_print, 2, STRING("stg8_las_99"), 0, GW(9))
            WAIT_MS(300)
        END_IF
        CALL(btlevtcmd_GetUnitWork, -2, 0, LW(0))
        IF_INT_EQUAL(LW(0), 1)
            CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("KPA_U_2"))
            WAIT_MS(200)
        END_IF
        CALL(btlevtcmd_StatusWindowOnOff, 1)
        CALL(evt_btl_camera_set_mode, 0, 0)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, -2, 1)
    RETURN
EVT_END()

static const char* cmd_disable_string[] = {
    "btl_jump_cmd_disable",
    "btl_hammer_cmd_disable",
    "btl_item_cmd_disable",
    "btl_party_tech_cmd_disable",
    "btl_party_item_cmd_disable",
};

static void _disp_icon(CameraId camera, void* param) {
    EventEntry* event = param;

    iconDispGx(&((IconJumpWork*)event->userdatap[0])->pos, 0, event->userdata[1], 1.0f);
}

static USER_FUNC(_icon_jump_icon) {
    EvtWord* args = event->args;
    s32 icon = evtGetValue(event, args[0]);
    f32 x0 = evtGetFloat(event, args[1]);
    f32 y0 = evtGetFloat(event, args[2]);
    f32 z0 = evtGetFloat(event, args[3]);
    f32 x1 = evtGetFloat(event, args[4]);
    f32 y1 = evtGetFloat(event, args[5]);
    f32 z1 = evtGetFloat(event, args[6]);
    f32 gravity = evtGetFloat(event, args[7]);
    s32 frames = evtGetValue(event, args[8]);
    IconJumpWork* wk;

    if (isFirstCall) {
        Vec start;

        event->userdatap[0] = BattleAlloc(sizeof(IconJumpWork));
        wk = event->userdatap[0];
        wk->end.x = x1;
        wk->end.y = y1;
        wk->end.z = z1;
        start.x = x0;
        start.y = y0;
        start.z = z0;
        wk->pos = start;
        wk->start = start;
        wk->gravity = gravity;
        wk->frames = frames;
        wk->vy = (wk->end.y - wk->start.y) / wk->frames + wk->gravity * wk->frames / 2;
        wk->speed = distABf(wk->start.x, wk->start.z, wk->end.x, wk->end.z) / wk->frames;
        event->userdata[1] = icon;
        event->userdata[2] = 0;
    }
    wk = event->userdatap[0];
    if (event->userdata[2]) {
        BattleFree(wk);
        return EVT_RETURN_DONE;
    }
    wk->pos.y += wk->vy;
    wk->vy -= wk->gravity;
    if (wk->vy < 0.0f && wk->pos.y < wk->end.y) {
        wk->pos.y = wk->end.y;
    }
    btlMovePos(wk->speed, angleABf(wk->start.x, wk->start.z, wk->end.x, wk->end.z), &wk->pos.x, &wk->pos.z);
    wk->frames--;
    if (wk->frames <= 0) {
        event->userdata[2] = 1;
        wk->pos = wk->end;
    }
    dispEntry(CAMERA_3D, 900.0f, 1, _disp_icon, event);
    return EVT_RETURN_BLOCK;
}

static USER_FUNC(party_cannot_use_command) {
    BattleWork* wp = _battleWorkPointer;
    s32 r = irand(1);
    s32 var = event->args[0];
#ifdef VERSION_G8ME01
    s32 id = evtGetValue(event, event->args[1]);
    s32 weapon = 0;
    BattleUnit* unit;

    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(wp, id);
#else
    s32 weapon = 0;
#endif
#ifdef VERSION_G8ME01

    switch (r) {
        case 0:
            if (unit != NULL) {
                unit->usCommandDisabled[0] = 3;
            }
            weapon = 344;
            break;
        case 1:
            if (unit != NULL) {
                unit->usCommandDisabled[1] = 3;
            }
            weapon = 340;
            break;
    }
#else
    switch (r) {
        case 0:
            wp->commandDisabled[3] = 3;
            weapon = 336;
            break;
        case 1:
            wp->commandDisabled[4] = 3;
            weapon = 332;
            break;
    }
#endif
    event->lwData[0] = (EvtWord)cmd_disable_string[r + 3];
    evtSetValue(event, var, weapon);
    return EVT_RETURN_DONE;
}

static USER_FUNC(mario_cannot_use_command) {
    BattleWork* wp = _battleWorkPointer;
    s32 r = irand(2);
    s32 var = event->args[0];
    s32 weapon = 0;

    switch (r) {
        case 0:
            wp->commandDisabled[0] = 3;
#ifdef VERSION_G8ME01
            weapon = 334;
#else
            weapon = 326;
#endif
            break;
        case 1:
            wp->commandDisabled[1] = 3;
#ifdef VERSION_G8ME01
            weapon = 337;
#else
            weapon = 329;
#endif
            break;
        case 2:
            wp->commandDisabled[2] = 3;
#ifdef VERSION_G8ME01
            weapon = 340;
#else
            weapon = 332;
#endif
            break;
    }
    event->lwData[0] = (EvtWord)cmd_disable_string[r];
    evtSetValue(event, var, weapon);
    return EVT_RETURN_DONE;
}

