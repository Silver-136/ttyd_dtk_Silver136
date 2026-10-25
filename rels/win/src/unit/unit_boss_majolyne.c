#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_eff.h"
#include "party/party_motion.h"
#include "unit/party/unit_party_vivian.h"
#include "battle/battle_database_common.h"
#include "evt/evt_msg.h"
#include "evt/evt_sub.h"

typedef struct MajoSortEntry { s32 id; s32 value; } MajoSortEntry;

USER_FUNC(btlevtcmd_SetEventPhase);
USER_FUNC(btlevtcmd_SortPhaseMoveTable);
static USER_FUNC(majo3_sort);

BattleUnitKind unit_boss_majolyne;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static PoseTableEntry pose_table[18];
static BattleDataEntry data_table[2];
static BattleUnitKindPart parts[1];
static BattleWeapon weapon_majolyne_kagenuke_attack1;
static BattleWeapon weapon_majolyne_blizzard_attack1;
static BattleWeapon weapon_majolyne_powder_attack1;
static BattleWeapon weapon_majolyne_powder_attack2;
static EvtWord majolyne_down[19];
static EvtWord majolyne_up[27];
static EvtWord majo3_zoom[25];
EvtWord btl_vivian_zoom[7];
EvtWord btl_marilyn_zoom[7];
EvtWord btl_majolyne_zoom[7];
static EvtWord init_event[45];
static EvtWord damage_event[22];
static EvtWord dead_event[122];
EvtWord majo_unison_phase_event[238];
static EvtWord unison_phase_event[83];
static EvtWord phase_event[337];
static EvtWord attack_event[241];
#ifdef VERSION_G8ME01
static EvtWord kagenuke_attack_event[225];
#else
static EvtWord kagenuke_attack_event[222];
#endif
static EvtWord blizzard_attack_event[243];
static EvtWord powder_attack_check[16];
static EvtWord powder_attack_event[179];
static EvtWord wait_event[6];
#ifdef VERSION_G8ME01
EvtWord majo3_shadow_tail_event[79];
#else
EvtWord majo3_shadow_tail_event[78];
#endif

BattleUnitKind unit_boss_majolyne = {
    31,
    "btl_un_majolyne1",
    9,
    0,
    1,
    1,
    26,
    0,
    0,
    100,
    0,
    50,
    9,
    26,
    34,
    0,
    34,
    0.0f,
    17.0f,
    0.0f,
    0,
    0,
    6.0f,
    2.0f,
    0.0f,
    13.0f,
    0.0f,
    -10.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    13.0f,
    0.0f,
    0.0f,
    8.0f,
    22.1f,
    0.0f,
    0.0f,
    17.0f,
    0.0f,
    26.0f,
    34.0f,
    0,
    0,
    0,
    2,
    100,
    20,
    0.0f,
    0.0f,
    0.0f,
    "SFX_BOSS_MAJO_DAMAGED1",
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

static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

static StatusVulnerability regist = {70, 80, 70, 100, 70, 100, 100, 50, 100, 80, 100, 80, 100, 95, 70, 0, 0, 100, 70, 100, 100, 0};

static PoseTableEntry pose_table[] = {
    {1, "MJR_N_1"},
    {2, "MJR_Z_1"},
    {9, "MJR_Z_1"},
    {5, "MJR_X_1"},
    {4, "MJR_X_1"},
    {3, "MJR_X_1"},
    {28, "MJR_S_1"},
    {29, "MJR_Q_1"},
    {30, "MJR_Q_1"},
    {31, "MJR_D_1"},
    {39, "MJR_D_1"},
    {50, "MJR_S_1"},
    {42, "MJR_R_1"},
    {40, "MJR_W_1"},
    {56, "MJR_S_1"},
    {57, "MJR_S_1"},
    {65, "MJR_T_1"},
    {69, "MJR_S_1"},
};

static BattleDataEntry data_table[] = {
    {49, dead_event},
    {0, NULL},
};

static BattleUnitKindPart parts[] = {
    {1, "btl_un_majolyne1", "c_majyorin", 0.0f, 0.0f, 0.0f, 5.0f, 20.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, 0x9, 0, pose_table},
};

static BattleWeapon weapon_majolyne_kagenuke_attack1 = {
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
    {1, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x20000000, // targetPropertyFlags
    2, // element
    0, // damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    7, // special
    2, // counterResistFlags
    0x80002000, // targetWeightFlags
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

static BattleWeapon weapon_majolyne_blizzard_attack1 = {
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
    {2, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x20000000, // targetPropertyFlags
    2, // element
    0, // damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    0x43, // special
    0x7FF, // counterResistFlags
    0x80002000, // targetWeightFlags
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

static BattleWeapon weapon_majolyne_powder_attack1 = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    1, // reserved_19
    1, // mBingoSlotChance
    1, // reserved_1B
    NULL, // hpDamageFn
    {0, 2, 2, 3, 3, 4, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_OPPOSITE_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x10000000, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    0x8F, // special
    0x7FF, // counterResistFlags
    0x80002000, // targetWeightFlags
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
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

static BattleWeapon weapon_majolyne_powder_attack2 = {
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
    NULL, // hpDamageFn
    {0, 2, 2, 3, 3, 4, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x20000000, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    0x87, // special
    0x7FF, // counterResistFlags
    0x80002000, // targetWeightFlags
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
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

static EVT_BEGIN(majolyne_down)
    BEGIN_INLINE_EVENT
        WAIT_MS(250)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_MAJO_SHADOW1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_INLINE_EVENT
    CALL(battle_evt_majo_disp_off, -2, 1, 0, 0)
    RETURN
EVT_END()

static EVT_BEGIN(majolyne_up)
    BEGIN_INLINE_EVENT
        WAIT_MS(250)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_MAJO_SHADOW2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_INLINE_EVENT
    CALL(battle_evt_majo_disp_on, -2, 1, LW(0), LW(1), LW(2), 0)
    CALL(btlevtcmd_SetPos, -2, LW(0), LW(1), LW(2))
    RETURN
EVT_END()

static EVT_BEGIN(majo3_zoom)
    CALL(btlevtcmd_GetPos, LW(0), LW(0), EVT_NULLPTR, EVT_NULLPTR)
    CALL(evt_btl_camera_set_mode, 0, 3)
    CALL(evt_btl_camera_set_moveto, 0, LW(0), 65, 332, LW(0), 30, -340, 30, 11)
    WAIT_FRAMES(37)
    RETURN
EVT_END()

EVT_BEGIN(btl_vivian_zoom)
    SET_VALUE(LW(0), GW(10))
    CALL_CHILD(PTR(majo3_zoom))
    RETURN
EVT_END()

EVT_BEGIN(btl_marilyn_zoom)
    SET_VALUE(LW(0), GW(9))
    CALL_CHILD(PTR(majo3_zoom))
    RETURN
EVT_END()

EVT_BEGIN(btl_majolyne_zoom)
    SET_VALUE(LW(0), GW(8))
    CALL_CHILD(PTR(majo3_zoom))
    RETURN
EVT_END()

static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, -2, PTR(wait_event))
    CALL(btlevtcmd_SetEventPhase, -2, PTR(phase_event))
    CALL(btlevtcmd_SetEventAttack, -2, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, -2, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, -2, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_SetUnitWork, -2, 8, 0)
    CALL(btlevtcmd_SetUnitWork, -2, 9, 0)
    CALL(btlevtcmd_SetUnitWork, -2, 3, 0)
    SPAWN_EVENT(PTR(majo3_shadow_tail_event))
    ALLOCATE_UW(10, GW(15))
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(damage_event)
    CALL(btlevtcmd_CheckDamageCode, -2, 1024, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(GW(11), 1)
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
        CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 40)
        CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
        CALL(btlevtcmd_ResetFaceDirection, -2)
    END_IF
    WAIT_FRAMES(1)
    CALL(btlevtcmd_WaitAttackEnd)
    CALL(btlevtcmd_StatusWindowOnOff, 0)
    CALL_CHILD(PTR(btl_majolyne_zoom))
    WAIT_MS(500)
    CALL(btlevtcmd_SetTalkPose, -2, STRING("MJR_D_1"))
    CALL(btlevtcmd_SetStayPose, -2, STRING("MJR_D_1"))
    IF_INT_NOT_EQUAL(GW(12), 0)
        IF_INT_NOT_EQUAL(GW(13), 0)
            CALL(evt_msg_print, 2, STRING("btl_majo3_11"), 0, -2)
            GOTO(90)
        END_IF
    END_IF
    CALL(evt_msg_print, 2, STRING("btl_majo3_08"), 0, -2)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("MJR_D_3"))
    LABEL(90)
    WAIT_MS(300)
    CALL(btlevtcmd_StatusWindowOnOff, 1)
    CALL(evt_btl_camera_set_mode, 0, 0)
    SET_VALUE(GW(11), 2)
    SET_VALUE(LW(10), -2)
    CALL_CHILD(PTR(subsetevt_dead_core_nospin_norotate))
    RETURN
EVT_END()

EVT_BEGIN(majo_unison_phase_event)
    SET_VALUE(LW(0), -1)
    SET_VALUE(LW(1), -1)
    SET_VALUE(LW(2), -1)
    IF_INT_EQUAL(GW(13), 0)
        CALL(btlevtcmd_GetUnitWork, GW(10), 5, LW(6))
        SWITCH(LW(6))
            CASE_EQUAL(0)
            CALL(btlevtcmd_GetHp, GW(10), LW(0))
            SET_VALUE(LW(3), 1)
            CASE_EQUAL(1)
            CALL(btlevtcmd_GetHp, GW(10), LW(1))
            SET_VALUE(LW(4), 1)
            CASE_EQUAL(2)
            CALL(btlevtcmd_GetHp, GW(10), LW(2))
            SET_VALUE(LW(5), 1)
        END_SWITCH
    END_IF
    IF_INT_EQUAL(GW(12), 0)
        CALL(btlevtcmd_GetUnitWork, GW(9), 5, LW(6))
        SWITCH(LW(6))
            CASE_EQUAL(0)
            CALL(btlevtcmd_GetHp, GW(9), LW(0))
            SET_VALUE(LW(3), 2)
            CASE_EQUAL(1)
            CALL(btlevtcmd_GetHp, GW(9), LW(1))
            SET_VALUE(LW(4), 2)
            CASE_EQUAL(2)
            CALL(btlevtcmd_GetHp, GW(9), LW(2))
            SET_VALUE(LW(5), 2)
        END_SWITCH
    END_IF
    IF_INT_EQUAL(GW(11), 0)
        CALL(btlevtcmd_GetUnitWork, GW(8), 5, LW(6))
        SWITCH(LW(6))
            CASE_EQUAL(0)
            CALL(btlevtcmd_GetHp, GW(8), LW(0))
            SET_VALUE(LW(3), 3)
            CASE_EQUAL(1)
            CALL(btlevtcmd_GetHp, GW(8), LW(1))
            SET_VALUE(LW(4), 3)
            CASE_EQUAL(2)
            CALL(btlevtcmd_GetHp, GW(8), LW(2))
            SET_VALUE(LW(5), 3)
        END_SWITCH
    END_IF
    CALL(majo3_sort)
    SWITCH(LW(0))
        CASE_EQUAL(1)
        IF_INT_EQUAL(GW(13), 0)
            CALL(btlevtcmd_SetUnitWork, GW(10), 5, 0)
        END_IF
        CASE_EQUAL(2)
        IF_INT_EQUAL(GW(12), 0)
            CALL(btlevtcmd_SetUnitWork, GW(9), 5, 0)
        END_IF
        CASE_EQUAL(3)
        IF_INT_EQUAL(GW(11), 0)
            CALL(btlevtcmd_SetUnitWork, GW(8), 5, 0)
        END_IF
    END_SWITCH
    SWITCH(LW(1))
        CASE_EQUAL(1)
        IF_INT_EQUAL(GW(13), 0)
            CALL(btlevtcmd_SetUnitWork, GW(10), 5, 1)
        END_IF
        CASE_EQUAL(2)
        IF_INT_EQUAL(GW(12), 0)
            CALL(btlevtcmd_SetUnitWork, GW(9), 5, 1)
        END_IF
        CASE_EQUAL(3)
        IF_INT_EQUAL(GW(11), 0)
            CALL(btlevtcmd_SetUnitWork, GW(8), 5, 1)
        END_IF
    END_SWITCH
    SWITCH(LW(2))
        CASE_EQUAL(1)
        IF_INT_EQUAL(GW(13), 0)
            CALL(btlevtcmd_SetUnitWork, GW(10), 5, 2)
        END_IF
        CASE_EQUAL(2)
        IF_INT_EQUAL(GW(12), 0)
            CALL(btlevtcmd_SetUnitWork, GW(9), 5, 2)
        END_IF
        CASE_EQUAL(3)
        IF_INT_EQUAL(GW(11), 0)
            CALL(btlevtcmd_SetUnitWork, GW(8), 5, 2)
        END_IF
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(unison_phase_event)
    CALL(btlevtcmd_CheckPhase, LW(0), 67108867)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(PTR(majo_unison_phase_event))
        CALL(btlevtcmd_GetUnitWork, -2, 4, LW(3))
        CALL(btlevtcmd_GetUnitWork, -2, 5, LW(4))
        IF_INT_NOT_EQUAL(LW(3), LW(4))
            CALL(btlevtcmd_SetUnitWork, -2, 4, LW(4))
            CALL(btlevtcmd_StartWaitEvent, -2)
            WAIT_MS(400)
            CALL_CHILD(PTR(majolyne_down))
            WAIT_MS(500)
            SWITCH(LW(4))
                CASE_EQUAL(0)
                SET_VALUE(LW(0), 55)
                SET_VALUE(LW(2), 0)
                CASE_EQUAL(1)
                SET_VALUE(LW(0), 105)
                SET_VALUE(LW(2), 10)
                CASE_EQUAL(2)
                SET_VALUE(LW(0), 155)
                SET_VALUE(LW(2), 20)
            END_SWITCH
            SET_VALUE(LW(1), 0)
            CALL(btlevtcmd_SetHomePos, -2, LW(0), LW(1), LW(2))
            CALL_CHILD(PTR(majolyne_up))
            CALL(btlevtcmd_SortPhaseMoveTable)
            CALL(btlevtcmd_StartWaitEvent, -2)
        END_IF
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(phase_event)
    CALL(btlevtcmd_CheckPhase, LW(0), 67108867)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetUnitWork, -2, 9, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            SUBTRACT(LW(0), 1)
            CALL(btlevtcmd_SetUnitWork, -2, 9, LW(0))
        END_IF
        CALL(btlevtcmd_CheckActStatus, -2, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            GOTO(90)
        END_IF
        CALL(btlevtcmd_GetUnitWork, -2, 6, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_GetMaxHp, -2, LW(0))
            DEBUG_PUT_REG(LW(0))
            MULTIPLY(LW(0), 66)
            DEBUG_PUT_REG(LW(0))
            DIVIDE(LW(0), 100)
            DEBUG_PUT_REG(LW(0))
            CALL(btlevtcmd_GetHp, -2, LW(1))
            DEBUG_PUT_REG(LW(1))
            IF_INT_LESS(LW(1), LW(0))
                CALL(btlevtcmd_SetUnitWork, -2, 6, 1)
                CALL(btlevtcmd_StatusWindowOnOff, 0)
                CALL_CHILD(PTR(btl_majolyne_zoom))
                CALL(evt_msg_print, 2, STRING("btl_majo3_03"), 0, -2)
                WAIT_MS(200)
                CALL(btlevtcmd_GetUnitWork, -2, 7, LW(0))
                IF_INT_EQUAL(LW(0), 0)
                    CALL(btlevtcmd_SetUnitWork, -2, 7, 1)
                    CALL(btlevtcmd_SetUnitWork, -2, 8, 3)
                END_IF
                IF_INT_EQUAL(GW(12), 0)
                    CALL(btlevtcmd_SetUnitWork, GW(9), 7, 1)
                END_IF
                IF_INT_EQUAL(GW(13), 0)
                    CALL(btlevtcmd_SetUnitWork, GW(10), 7, 1)
                END_IF
                CALL(btlevtcmd_StatusWindowOnOff, 1)
                CALL(evt_btl_camera_set_mode, 0, 0)
                CALL(btlevtcmd_StartWaitEvent, -2)
            END_IF
        END_IF
        IF_INT_EQUAL(GW(12), 0)
            CALL(btlevtcmd_GetUnitWork, GW(9), 6, LW(0))
            IF_INT_EQUAL(LW(0), 0)
                CALL(btlevtcmd_GetMaxHp, GW(9), LW(0))
                MULTIPLY(LW(0), 50)
                DIVIDE(LW(0), 100)
                CALL(btlevtcmd_GetHp, GW(9), LW(1))
                IF_INT_LESS(LW(1), LW(0))
                    CALL(btlevtcmd_SetUnitWork, GW(9), 6, 1)
                    CALL(btlevtcmd_StatusWindowOnOff, 0)
                    CALL_CHILD(PTR(btl_majolyne_zoom))
                    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, GW(9), -1, 16, LW(15))
                    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
                    WAIT_MS(200)
                    CALL(evt_msg_print, 2, STRING("btl_majo3_04"), 0, -2)
                    WAIT_MS(200)
                    CALL(btlevtcmd_SetUnitWork, GW(9), 7, 1)
                    CALL(btlevtcmd_CheckActStatus, GW(9), LW(0))
                    IF_INT_NOT_EQUAL(LW(0), 0)
                        CALL_CHILD(PTR(btl_marilyn_zoom))
                        CALL(evt_msg_print, 2, STRING("btl_majo3_05"), 0, GW(9))
                        WAIT_MS(200)
                    END_IF
                    CALL(btlevtcmd_ResetFaceDirection, -2)
                    CALL(btlevtcmd_StatusWindowOnOff, 1)
                    CALL(evt_btl_camera_set_mode, 0, 0)
                    CALL(btlevtcmd_StartWaitEvent, -2)
                END_IF
            END_IF
        END_IF
        IF_INT_EQUAL(GW(13), 0)
            CALL(btlevtcmd_GetUnitWork, GW(10), 6, LW(0))
            IF_INT_EQUAL(LW(0), 0)
                CALL(btlevtcmd_GetMaxHp, GW(10), LW(0))
                MULTIPLY(LW(0), 50)
                DIVIDE(LW(0), 100)
                CALL(btlevtcmd_GetHp, GW(10), LW(1))
                IF_INT_LESS(LW(1), LW(0))
                    CALL(btlevtcmd_SetUnitWork, GW(10), 6, 1)
                    CALL(btlevtcmd_StatusWindowOnOff, 0)
                    CALL_CHILD(PTR(btl_majolyne_zoom))
                    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, GW(10), -1, 16, LW(15))
                    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
                    WAIT_MS(200)
                    CALL(evt_msg_print, 2, STRING("btl_majo3_06"), 0, -2)
                    WAIT_MS(200)
                    CALL(btlevtcmd_SetUnitWork, GW(10), 7, 1)
                    CALL(btlevtcmd_CheckActStatus, GW(10), LW(0))
                    IF_INT_NOT_EQUAL(LW(0), 0)
                        CALL_CHILD(PTR(btl_vivian_zoom))
                        CALL(evt_msg_print, 2, STRING("btl_majo3_07"), 0, GW(10))
                        WAIT_MS(200)
                    END_IF
                    CALL(btlevtcmd_ResetFaceDirection, -2)
                    CALL(btlevtcmd_StatusWindowOnOff, 1)
                    CALL(evt_btl_camera_set_mode, 0, 0)
                    CALL(btlevtcmd_StartWaitEvent, -2)
                END_IF
            END_IF
        END_IF
    END_IF
    LABEL(90)
    RETURN
EVT_END()

static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_EnemyItemUseCheck, -2, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, -2)
        RETURN
    END_IF
    CALL(btlevtcmd_GetUnitWork, -2, 8, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CALL(btlevtcmd_SetUnitWork, -2, 8, 1)
        SET_VALUE(LW(9), PTR(&weapon_majolyne_kagenuke_attack1))
        CALL_CHILD(PTR(kagenuke_attack_event))
        CASE_EQUAL(1)
        CALL(btlevtcmd_SetUnitWork, -2, 8, 2)
        SET_VALUE(LW(9), PTR(&weapon_majolyne_powder_attack2))
        CALL_CHILD(PTR(powder_attack_check))
        CALL_CHILD(PTR(powder_attack_event))
        CASE_EQUAL(2)
        CALL(btlevtcmd_SetUnitWork, -2, 8, 4)
        SET_VALUE(LW(9), PTR(&weapon_majolyne_powder_attack1))
        CALL_CHILD(PTR(powder_attack_check))
        CALL_CHILD(PTR(powder_attack_event))
        CASE_EQUAL(3)
        CALL(btlevtcmd_SetUnitWork, -2, 8, 4)
        CALL(btlevtcmd_SetUnitWork, -2, 9, 2)
        SET_VALUE(LW(9), PTR(&weapon_majolyne_blizzard_attack1))
        CALL_CHILD(PTR(blizzard_attack_event))
        CASE_EQUAL(4)
        SET_VALUE(LW(14), 40)
        SET_VALUE(LW(9), PTR(&weapon_majolyne_powder_attack1))
        CALL_CHILD(PTR(powder_attack_check))
        IF_INT_NOT_EQUAL(LW(3), -1)
            SET_VALUE(LW(10), LW(3))
            SET_VALUE(LW(11), LW(4))
            CALL(btlevtcmd_CheckStatus, LW(3), 10, LW(0))
            IF_INT_EQUAL(LW(0), 0)
                ADD(LW(14), 20)
            END_IF
        END_IF
        SET_VALUE(LW(9), PTR(&weapon_majolyne_powder_attack2))
        CALL_CHILD(PTR(powder_attack_check))
        IF_INT_NOT_EQUAL(LW(3), -1)
            SET_VALUE(LW(12), LW(3))
            SET_VALUE(LW(13), LW(4))
            CALL(btlevtcmd_CheckStatus, LW(3), 11, LW(0))
            IF_INT_EQUAL(LW(0), 0)
                ADD(LW(14), 20)
            END_IF
        END_IF
        CALL(btlevtcmd_GetUnitWork, -2, 7, LW(2))
        IF_INT_NOT_EQUAL(LW(2), 0)
            CALL(btlevtcmd_GetUnitWork, -2, 9, LW(0))
            IF_INT_EQUAL(LW(0), 0)
                ADD(LW(14), 20)
            END_IF
        END_IF
        SUBTRACT(LW(14), 1)
        CALL(evt_sub_random, LW(14), LW(0))
        SET_VALUE(LW(1), 40)
        IF_INT_LESS(LW(0), LW(1))
            SET_VALUE(LW(9), PTR(&weapon_majolyne_kagenuke_attack1))
            CALL_CHILD(PTR(kagenuke_attack_event))
            GOTO(90)
        END_IF
        ADD(LW(1), 20)
        IF_INT_LESS(LW(0), LW(1))
            SET_VALUE(LW(9), PTR(&weapon_majolyne_powder_attack1))
            SET_VALUE(LW(3), LW(10))
            SET_VALUE(LW(4), LW(11))
            CALL_CHILD(PTR(powder_attack_event))
            GOTO(90)
        END_IF
        ADD(LW(1), 20)
        IF_INT_LESS(LW(0), LW(1))
            SET_VALUE(LW(9), PTR(&weapon_majolyne_powder_attack2))
            SET_VALUE(LW(3), LW(12))
            SET_VALUE(LW(4), LW(13))
            CALL_CHILD(PTR(powder_attack_event))
            GOTO(90)
            ELSE
            CALL(btlevtcmd_SetUnitWork, -2, 9, 2)
            SET_VALUE(LW(9), PTR(&weapon_majolyne_blizzard_attack1))
            CALL_CHILD(PTR(blizzard_attack_event))
        END_IF
    END_SWITCH
    LABEL(90)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(kagenuke_attack_event)
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
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
    CALL(btlevtcmd_AttackDeclare, -2, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, -2, LW(9))
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
    CALL(evt_btl_camera_set_mode, 0, 8)
    CALL(evt_btl_camera_set_homing_unit, 0, -2, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("MJR_A1_1"))
    CALL_CHILD(majolyne_down)
    WAIT_MS(300)
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
#ifdef VERSION_G8ME01
    ADD(LW(2), 5)
#endif
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 30)
    SET_VALUE(LW(1), 0)
    CALL_CHILD(majolyne_up)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_MAJO_PUSH1"), EVT_NULLPTR, 0, LW(15))
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("MJR_A1_2"))
    WAIT_FRAMES(25)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("MJR_A1_3"))
    WAIT_MS(350)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("MJR_A1_4"))
    WAIT_MS(200)
    CALL(evt_snd_sfxoff, LW(15))
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
    GOTO(98)
    LABEL(91)
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
    CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(9), 256, LW(5))
    LABEL(98)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("MJR_A1_1"))
    CALL_CHILD(majolyne_down)
    CALL(evt_btl_camera_set_mode, 0, 0)
    WAIT_MS(200)
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL_CHILD(majolyne_up)
    RETURN
EVT_END()

static EVT_BEGIN(blizzard_attack_event)
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
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
    CALL(btlevtcmd_AttackDeclareAll, -2)
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, -2, LW(9))
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_MAJO_HAND_OPEN1"), EVT_NULLPTR, 0, LW(15))
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("MJR_A1_2"))
    WAIT_FRAMES(25)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("MJR_A1_3"))
    WAIT_MS(700)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("MJR_A1_4"))
    WAIT_MS(200)
    CALL(evt_snd_sfxoff, LW(15))
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    SET_VALUE(LW(10), 0)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_MAJO_ATTACK_ICE1"), EVT_NULLPTR, 0, LW(15))
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetFaceDirection, -2, LW(5))
    IF_INT_GREATER_OR_EQUAL(LW(5), 0)
        CALL(evt_eff, 0, STRING("ibuki"), 0, LW(0), LW(1), LW(2), 180, 0, 0, 0, 0, 0, 0, 0)
        ELSE
        CALL(evt_eff, 0, STRING("ibuki"), 1, LW(0), LW(1), LW(2), 180, 0, 0, 0, 0, 0, 0, 0)
    END_IF
    WAIT_FRAMES(150)
    CALL(evt_snd_sfxoff, LW(15))
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
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    IF_INT_NOT_EQUAL(LW(3), -1)
        ADD(LW(10), 1)
        GOTO(10)
    END_IF
    LABEL(98)
    CALL(evt_btl_camera_set_mode, 0, 0)
    WAIT_MS(1000)
    RETURN
EVT_END()

static EVT_BEGIN(powder_attack_check)
    CALL(btlevtcmd_GetEnemyBelong, -2, LW(0))
    CALL(btlevtcmd_SamplingEnemy, -2, LW(0), LW(9))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(9), LW(3), LW(4))
    RETURN
EVT_END()

static EVT_BEGIN(powder_attack_event)
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, -2, 16, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
    CALL(btlevtcmd_AttackDeclare, -2, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, -2, LW(9))
    IF_INT_EQUAL(LW(9), PTR(&weapon_majolyne_powder_attack1))
        CALL(evt_btl_camera_set_mode, 0, 9)
        CALL(evt_btl_camera_set_homing_unit, 0, -2, LW(3))
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
        CALL(evt_btl_camera_set_zoom, 0, 200)
    END_IF
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_MAJO_HAND_OPEN1"), EVT_NULLPTR, 0, LW(15))
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("MJR_A2_1"))
    WAIT_MS(700)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("MJR_A2_2"))
    WAIT_MS(100)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("MJR_A2_3"))
    CALL(evt_snd_sfxoff, LW(15))
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
    GOTO(99)
    LABEL(91)
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
    CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(9), 256, LW(5))
    WAIT_FRAMES(30)
    GOTO(99)
    LABEL(99)
    CALL(evt_btl_camera_set_mode, 0, 0)
    WAIT_MS(250)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    RETURN
EVT_END()

static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, -2, 1)
    RETURN
EVT_END()

EVT_BEGIN(majo3_shadow_tail_event)
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

static USER_FUNC(majo3_sort) {
    MajoSortEntry tbl[3];
    MajoSortEntry tmp1;
    MajoSortEntry tmp3;
    MajoSortEntry tmp2;
    s32 i;

    for (i = 0; i < 3; i++) {
        tbl[i].id = event->lwData[3 + i];
        tbl[i].value = event->lwData[i];
    }
    if (tbl[0].value < tbl[1].value) {
        tmp1 = tbl[0];
        tbl[0] = tbl[1];
        tbl[1] = tmp1;
    }
    if (tbl[1].value < tbl[2].value) {
        tmp2 = tbl[1];
        tbl[1] = tbl[2];
        tbl[2] = tmp2;
    }
    if (tbl[0].value < tbl[1].value) {
        tmp3 = tbl[0];
        tbl[0] = tbl[1];
        tbl[1] = tmp3;
    }
    for (i = 0; i < 3; i++) {
        event->lwData[i] = tbl[i].id;
    }
    return EVT_RETURN_DONE;
}
