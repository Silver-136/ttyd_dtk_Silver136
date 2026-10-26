#include "eff/n64/eff_shock_n64.h"
#include "eff/n64/eff_pokopi_thunder_n64.h"
#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "sfx_name_word.h"
#include "pmario_sound.h"
#include "libc_decls.h"
#include "battle/battle_event_cmd.h"
#include "unit/party/unit_party_vivian.h"
#include "evt/evt_batstage.h"
#include "battle/battle_camera.h"
#include "evt/evt_eff.h"
#include "evt/evt_env.h"
#include "evt/evt_ext.h"
#include "evt/evt_msg.h"
#include "evt/evt_npc.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_sub.h"
#include "battle/battle_break_slot.h"
#include "driver/extdrv.h"
#include "eff/n64/eff_stardust2_n64.h"
#include "battle/battle_status_effect.h"

#include "battle/battle.h"
#include "battle/battle_sub.h"
#include "battle/battle_unit.h"
#include "battle/battle_audience.h"
#include "driver/animdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "driver/npcdrv.h"
#include "mario/mariost.h"
#include "memory.h"
#include "string.h"
#include "system.h"
#include "dolphin/mtx.h"
#include "dolphin/os.h"
#include "manager/evtmgr_cmd.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "battle/battle_database_common.h"
#include "battle/battle_damage.h"

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_ATTACK_STEP 0  // next scripted attack: 0, 1, 2 fixed order (Peach: thunder, energy, dark drag; Queen: thunder, charge, black wave), 3 = random draw; 2 = charged, the hands skip their attack; setup value 1 = start as the Shadow Queen
#define UNIT_WORK_MESSAGE_STEP 1 // Shadow Queen attack messages shown: 0 none, 1 stg8_las_142, 2 stg8_las_143, 3 stg8_las_145 (queen_init_event starts at 3)
#define UNIT_WORK_HP_MESSAGE 2   // low-HP messages shown: 1 after queen_2_hp_msg_evt (HP < 1/2), 2 after queen_4_hp_msg_evt (HP < 1/4)

typedef struct HandElem {
    u16 flip;
    u8 field_0x2[2];
    s32 animIdx;
    OSTime time;
    s16 ofs;
    s16 phase;
    f32 speed;
    Vec pos;
    s32 field_0x24;
} HandElem;
typedef struct HandAnim {
    char name[16];
    char anim[8];
    f32 rate;
    s32 start;
    s32 count;
} HandAnim;
typedef struct HandWork {
    HandElem* elems;
    s32 num;
    HandAnim anim[32];
    u8 field_0x488[0x49C - 0x488];
    f32 y;
    u8 field_0x4a0[4];
    s32 animNum;
    s32 timer;
    s32 state;
} HandWork;
typedef struct QueenConnect { s32 poseId[3]; u8 field_0xc[0x40 - 0xC]; s32 idx[2]; } QueenConnect;
ExtTexInfo hand_data[32];
HandWork* hand_wp;
u8* lightGetPaperCraft(void);

USER_FUNC(btlevtcmd_InterruptStop);
USER_FUNC(btlevtcmd_SetEventEntry);
USER_FUNC(btlevtcmd_SetEventPhase);
USER_FUNC(eff_queen_set_alpha_evt);
USER_FUNC(make_pattern);
USER_FUNC(peach_hand1000_dark_drag_1_evt);
USER_FUNC(peach_talk_synchro_shadow_head_evt);
USER_FUNC(queen_drain);
USER_FUNC(queen_get_aud_num);
USER_FUNC(queen_hand_small_1000hand_1_evt);
USER_FUNC(queen_hand_small_1000hand_2_evt);
USER_FUNC(queen_hand_small_1000hand_set_offset_evt);
USER_FUNC(queen_hand_small_dead_evt);
USER_FUNC(queen_hand_small_drag_1_evt);
USER_FUNC(queen_hand_small_drag_2_evt);
USER_FUNC(queen_hand_small_drag_3_evt);
USER_FUNC(queen_hand_small_entry_evt);
USER_FUNC(queen_hp_drain_1_evt);
USER_FUNC(queen_hp_drain_2_evt);
USER_FUNC(queen_hp_drain_3_evt);
USER_FUNC(queen_hp_drain_4_evt);
USER_FUNC(queen_hp_drain_5_evt);
USER_FUNC(queen_kankyaku_delete_evt);
USER_FUNC(queen_thunder);
USER_FUNC(shadow_queen_connect);
USER_FUNC(slot_init);
USER_FUNC(queen_break_slot_flag2_on);
USER_FUNC(queen_hand_small_drag_4_evt);
USER_FUNC(peach_shock_evt);
USER_FUNC(change_pose);
void hand_main(void);
void hand_draw(CameraId cameraId, void* param);
static void hand_init(void);
static void pose_main(void);
static void pose_init(void);

BattleUnitKind unit_boss_black_peach;
BattleUnitDefense peach_defence;
BattleUnitDefenseAttr peach_defence_attr;
StatusVulnerability peach_regist;
PoseTableEntry peach_pose_table[18];
BattleDataEntry peach_data_table[2];
BattleUnitKindPart peach_parts[7];
BattleWeapon weapon_peach_black_thunder_attack;
BattleWeapon weapon_peach_black_energy_attack;
BattleWeapon weapon_peach_drain_attack;
BattleWeapon weapon_peach_dark_drag_attack;
BattleWeapon weapon_peach_dark_drag_last_attack;
BattleWeapon weapon_peach_dark_charge_attack;
BattleWeapon weapon_peach_black_wave_attack;
BattleWeapon weapon_peach_black_sigh_attack;
BattleWeapon weapon_peach_black_temptation_attack;
BattleWeapon weapon_peach_black_poison_attack;
BattleWeapon weapon_peach_black_press_attack;
BattleWeapon weapon_peach_black_1000hand_attack;
BattleUnitKind unit_boss_shadow_queen;
BattleUnitDefense queen_defence;
BattleUnitDefenseAttr queen_defence_attr;
StatusVulnerability queen_regist;
BattleUnitDefense queen_muteki_defence;
BattleUnitDefenseAttr queen_muteki_defence_attr;
StatusVulnerability queen_muteki_regist;
PoseTableEntry queen_pose_table[18];
PoseTableEntry queen_hand_pose_table[4];
BattleDataEntry queen_data_table[2];
BattleUnitKindPart queen_parts[5];
f32 hair_offset[36];
EvtWord shadow_queen_1000[25];
BattleUnitKind unit_boss_shadow_hand_1;
BattleUnitDefense queen_hand_1_defence;
BattleUnitDefenseAttr queen_hand_1_defence_attr;
StatusVulnerability queen_hand_1_regist;
PoseTableEntry queen_hand_1_pose_table[8];
BattleDataEntry queen_hand_1_data_table[2];
BattleUnitKindPart queen_hand_1_parts[1];
BattleUnitKind unit_boss_shadow_hand_2;
BattleUnitDefense queen_hand_2_defence;
BattleUnitDefenseAttr queen_hand_2_defence_attr;
StatusVulnerability queen_hand_2_regist;
PoseTableEntry queen_hand_2_pose_table[8];
BattleDataEntry queen_hand_2_data_table[2];
BattleUnitKindPart queen_hand_2_parts[1];
BattleUnitKind unit_boss_shadow_hand_small;
BattleUnitDefense queen_hand_small_defence;
BattleUnitDefenseAttr queen_hand_small_defence_attr;
StatusVulnerability queen_hand_small_regist;
PoseTableEntry queen_hand_small_pose_table[8];
BattleDataEntry queen_hand_small_data_table[2];
BattleUnitKindPart queen_hand_small_parts[1];
BattleUnitSetup entry_shadow_queen;
BattleUnitSetup entry_shadow_hand_1;
BattleUnitSetup entry_shadow_hand_2;
BattleUnitSetup entry_shadow_hand_small;
EvtWord peach_init_event[170];
EvtWord peach_entry_event[62];
EvtWord peach_damage_event[17];
EvtWord peach_change_evt[556];
EvtWord peach_phase_event[46];
EvtWord peach_dead_event[11];
EvtWord peack_attack_black_thunder[147];
EvtWord peack_attack_black_energy[125];
EvtWord peack_attack_dark_drag[525];
EvtWord peack_attack_drain[526];
EvtWord peach_attack_event[85];
EvtWord peach_wait_event[6];
EvtWord queen_init_event[225];
EvtWord queen_first_attack_message_evt[55];
EvtWord queen_next_attack_message_evt[55];
EvtWord queen_last_attack_message_evt[56];
EvtWord queen_change_hand_small_evt[75];
EvtWord queen_change_hand_big_evt[90];
EvtWord queen_hand_big_life_check_evt[75];
EvtWord queen_hand_small_life_check_evt[40];
EvtWord queen_hp_drain_evt[316];
EvtWord queen_hp_drain_evt_miss[41];
EvtWord queen_first_damage_evt[74];
EvtWord queen_2_hp_msg_evt[39];
EvtWord queen_4_hp_msg_evt[39];
EvtWord queen_dead_hp_msg_evt[88];
EvtWord queen_phase_event[208];
EvtWord queen_attack_black_thunder[197];
EvtWord queen_attack_charge[166];
EvtWord queen_attack_black_wave[445];
EvtWord queen_attack_black_energy[164];
EvtWord queen_attack_bress1[276];
EvtWord queen_attack_bress2[276];
EvtWord queen_attack_bress3[278];
EvtWord queen_attack_event[163];
EvtWord queen_damage_event[63];
EvtWord queen_dead_event[347];
EvtWord queen_wait_event[10];
EvtWord queen_entry_event[62];
EvtWord queen_hand_1_init_event[107];
EvtWord queen_hand_1_press_attack[343];
EvtWord queen_hand_1_drain_attack[408];
EvtWord queen_hand_1_attack_event[41];
EvtWord queen_hand_1_damage_event[24];
EvtWord queen_hand_1_dead_event[41];
EvtWord queen_hand_1_wait_event[6];
EvtWord queen_hand_2_init_event[113];
EvtWord queen_hand_2_attack_event[41];
EvtWord queen_hand_2_damage_event[24];
EvtWord queen_hand_2_dead_event[41];
EvtWord queen_hand_2_wait_event[6];
EvtWord queen_hand_small_init_event[96];
EvtWord queen_hand_small_drag_attack[385];
EvtWord queen_hand_small_1000hand_attack[VERSION_PICK(266, 274)];
EvtWord queen_hand_small_attack_event[35];
EvtWord queen_hand_small_damage_event[48];
EvtWord queen_hand_small_dead_event[32];
EvtWord queen_hand_small_wait_event[6];

BattleUnitKind unit_boss_black_peach = {
    UNIT_SHADOW_QUEEN_PEACH, // mUnitType
    "btl_un_black_peach", // name
    150, // maxHp
    0, // maxFp
    1, // mDangerHP
    1, // mPerilHP
    0, // mLevel
    0, // mBonusExp
    0, // bonusCoin
    100, // bonusCoinRate
    0, // baseCoin
    50, // runRate
    6, // pbSoftCap
    40, // width
    54, // height
    {5, 54}, // hitOffset
    {0.0f, 27.0f, 0.0f}, // centerOffset
    {0, -3}, // hpGaugeOffset
    {0.0f, 40.0f, 0.0f}, // talkTogeBaseOffset
    {20.0f, 0.0f, -10.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
    {15.0f, 0.0f, 0.0f}, // bintaHitOffset
    {15.0f, 45.0f, 0.0f}, // kissHitOffset
    {0.0f, 27.0f, 0.0f}, // cutBaseOffset
    40.0f, // cutWidth
    54.0f, // cutHeight
    80, // turnOrder
    0, // turnOrderVariance
    0, // swallowChance
    2, // swallowAttributes
    100, // hammerKnockbackChance
    0, // itemStealParam
    {0.0f, 0.0f, 0.0f}, // starPointDispOffset
    "SFX_BOSS_BPCH_DAMAGED1", // damageSfxName
    "SFX_BTL_DAMAGE_FIRE1", // fireDamageSfxName
    "SFX_BTL_DAMAGE_ICE1", // iceDamageSfxName
    "SFX_BTL_DAMAGE_BIRIBIRI1", // electricDamageSfxName
    0, // attributeFlags
    &peach_regist, // statusVulnerability
    7, // numParts
    peach_parts, // mParts
    peach_init_event, // initEventCode
    peach_data_table, // dataTable
};

BattleUnitDefense peach_defence = {{0, 0, 0, 0, 0}};

BattleUnitDefenseAttr peach_defence_attr = {{0, 0, 0, 0, 0}};

StatusVulnerability peach_regist = {
    30, // sleep
    30, // stop
    30, // dizzy
    100, // poison
    30, // confuse
    100, // electric
    100, // burn
    0, // freeze
    100, // huge
    60, // tiny
    100, // attackUp
    60, // attackDown
    100, // defenseUp
    75, // defenseDown
    30, // allergic
    0, // fright
    0, // galeForce
    100, // fast
    30, // slow
    100, // dodgy
    100, // invisible
    0, // ohko
};

PoseTableEntry peach_pose_table[] = {
    {POSE_TYPE_SLEEP, "KRT_N_1"},
    {POSE_TYPE_STOP, "KRT_Z_1"},
    {POSE_TYPE_FREEZE, "KRT_Z_1"},
    {POSE_TYPE_CONFUSE, "KRT_K_1"},
    {POSE_TYPE_POISON, "KRT_S_1"},
    {POSE_TYPE_DIZZY, "KRT_K_1"},
    {POSE_TYPE_NORMAL, "KRT_S_1"},
    {POSE_TYPE_DAMAGE_FIRE, "KRT_D_1"},
    {POSE_TYPE_DAMAGE_FIRE_2_UNUSED, "KRT_Q_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "KRT_Q_1"},
    {POSE_TYPE_DAMAGE, "KRT_D_1"},
    {POSE_TYPE_ATTACK, "KRT_A_1"},
    {POSE_TYPE_RUN, "KRT_W_1"},
    {POSE_TYPE_WALK, "KRT_W_1"},
    {POSE_TYPE_DANGER, "KRT_X_1"},
    {POSE_TYPE_PERIL, "KRT_X_1"},
    {POSE_TYPE_TALK, "KRT_T_1"},
    {POSE_TYPE_ACTION, "KRT_S_1"},
};

BattleDataEntry peach_data_table[] = {
    {UNIT_DATA_DEAD_EVENT, peach_dead_event},
    {0, NULL},
};

BattleUnitKindPart peach_parts[] = {
    {1, "btl_un_shadow_queen", "c_b_peach_b", 0.0f, VERSION_PICK(0.0f, 2.0f), 0.0f, -2.0f, 44.0f, 0.0f, -2.0f, 44.0f, 0.0f, 41, 61, 255, &peach_defence, &peach_defence_attr, kMainPart | kGuardTargetPart, 0, peach_pose_table},
    {2, "btl_un_shadow_queen", "c_q_kage_b", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 41, 61, 255, &peach_defence, &peach_defence_attr, 0x3018000, 0, peach_pose_table},
    {3, "btl_un_shadow_queen", "c_q_kage_f", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 41, 61, 255, &peach_defence, &peach_defence_attr, 0x13018000, 0, peach_pose_table},
    {4, "btl_un_shadow_queen", "c_q_kage_h", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 41, 61, 255, &peach_defence, &peach_defence_attr, 0x13018000, 0, peach_pose_table},
    {5, "btl_un_shadow_queen", "c_q_kage_f", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 41, 61, 255, &peach_defence, &peach_defence_attr, 0x13018000, 0, peach_pose_table},
    {6, "btl_un_shadow_hand_big_1", "c_b_hand", 15.0f, 0.0f, -50.0f, 0.0f, 38.0f, 5.0f, 0.0f, 38.0f, 5.0f, 41, 61, 255, &peach_defence, &peach_defence_attr, 0x13018000, 0, peach_pose_table},
    {7, "btl_un_shadow_hand_big_2", "c_b_hand", 160.0f, 0.0f, 50.0f, 0.0f, 38.0f, 5.0f, 0.0f, 38.0f, 5.0f, 41, 61, 255, &peach_defence, &peach_defence_attr, 0x13018000, 0, peach_pose_table},
};

BattleWeapon weapon_peach_black_thunder_attack = {
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
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_ELECTRIC, // element
    ATTACKPATTERN_NULL, // damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_PIERCE_DEFENSE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0x80002004, // targetWeightFlags
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

BattleWeapon weapon_peach_black_energy_attack = {
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
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_SELF | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_SAME_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH | SPECIAL_FLIP_SHELLED | SPECIAL_GROUND_WINGED, // special
    0, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, // burnTime..sizeChangeStrength
    100, // atkChangeChance
    3, // atkChangeTime
    3, // atkChangeStrength
    100, // defChangeChance
    3, // defChangeTime
    3, // defChangeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // allergicChance..slowTime
    0, 0, 0, 0, 0, 0, 0, 0, // frightChance..hpRegenStrength
    0, 0, // fpRegenTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

BattleWeapon weapon_peach_drain_attack = {
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
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_PIERCE_DEFENSE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0x80002004, // targetWeightFlags
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

BattleWeapon weapon_peach_dark_drag_attack = {
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
    weaponGetPowerDefault, // hpDamageFn
    {3, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_PIERCE_DEFENSE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0x80002004, // targetWeightFlags
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

BattleWeapon weapon_peach_dark_drag_last_attack = {
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
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, // element
    ATTACKPATTERN_NO_DAMAGE_FINISH, // damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_CANNOT_MISS | SPECIAL_PIERCE_DEFENSE | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0x80002004, // targetWeightFlags
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

BattleWeapon weapon_peach_dark_charge_attack = {
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
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_SELF | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_IGNORE_VULNERABILITY, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
    0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
    0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
    7, // chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

BattleWeapon weapon_peach_black_wave_attack = {
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
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_PIERCE_DEFENSE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0x2004, // targetWeightFlags
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

BattleWeapon weapon_peach_black_sigh_attack = {
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
    {4, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_PIERCE_DEFENSE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0x2004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, // atkChangeStrength..defChangeStrength
#ifdef VERSION_G8ME01
    90, // allergicChance
#else
    60, // allergicChance
#endif
    3, // allergicTime
    0, 0, 0, 0, 0, 0, 0, 0, // ohkoChance..galeForceChance
    0, 0, 0, 0, 0, 0, 0, 0, // paybackTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

BattleWeapon weapon_peach_black_temptation_attack = {
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
    {4, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_PIERCE_DEFENSE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0x2004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, // poisonStrength
#ifdef VERSION_G8ME01
    75, // confuseChance
#else
    50, // confuseChance
#endif
    3, // confuseTime
    0, 0, 0, 0, 0, 0, 0, 0, // electricChance..freezeTime
    0, 0, 0, 0, 0, 0, 0, 0, // sizeChangeChance..defChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // defChangeStrength..slowChance
    0, 0, 0, 0, 0, 0, 0, 0, // slowTime..hpRegenTime
    0, 0, 0, // hpRegenStrength..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

BattleWeapon weapon_peach_black_poison_attack = {
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
    {4, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_PIERCE_DEFENSE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0x2004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, // sleepChance..dizzyTime
#ifdef VERSION_G8ME01
    75, // poisonChance
#else
    50, // poisonChance
#endif
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

BattleWeapon weapon_peach_black_press_attack = {
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
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0x80002004, // targetWeightFlags
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

BattleWeapon weapon_peach_black_1000hand_attack = {
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
    {3, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
#ifdef VERSION_G8ME01
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_PIERCE_DEFENSE, // special
#else
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE, // special
#endif
    COUNTER_RESIST_ALL, // counterResistFlags
    0x80002004, // targetWeightFlags
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

BattleUnitKind unit_boss_shadow_queen = {
    UNIT_SHADOW_QUEEN_DEMON,
    "btl_un_shadow_queen",
    150,
    0,
    1,
    1,
    0,
    0,
    0,
    100,
    0,
    50,
    VERSION_PICK(6, 5),
    40,
    54,
    0,
    54,
    0.0f,
    90.0f,
    0.0f,
    0,
    0,
    0.0f,
    40.0f,
    0.0f,
    20.0f,
    0.0f,
    -10.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    25.0f,
    0.0f,
    0.0f,
    50.0f,
    40.0f,
    0.0f,
    0.0f,
    27.0f,
    0.0f,
    40.0f,
    54.0f,
    80,
    0,
    -1,
    0,
    0,
    0,
    0.0f,
    0.0f,
    0.0f,
    "SFX_BOSS_QWEN_DAMAGED1",
    "SFX_BTL_DAMAGE_FIRE1",
    "SFX_BTL_DAMAGE_ICE1",
    "SFX_BTL_DAMAGE_BIRIBIRI1",
    0,
    &queen_regist,
    5,
    queen_parts,
    queen_init_event,
    queen_data_table,
};

BattleUnitDefense queen_defence = {{1, 1, 1, 1, 1}};

BattleUnitDefenseAttr queen_defence_attr = {{0, 0, 0, 0, 0}};

StatusVulnerability queen_regist = {
    30, // sleep
    30, // stop
    30, // dizzy
    100, // poison
    30, // confuse
    100, // electric
    100, // burn
    0, // freeze
    100, // huge
    0, // tiny
    100, // attackUp
    0, // attackDown
    100, // defenseUp
    60, // defenseDown
    30, // allergic
    0, // fright
    0, // galeForce
    100, // fast
    30, // slow
    0, // dodgy
    0, // invisible
    0, // ohko
};

BattleUnitDefense queen_muteki_defence = {{99, 99, 99, 99, 99}};

BattleUnitDefenseAttr queen_muteki_defence_attr = {{2, 2, 2, 2, 2}};

StatusVulnerability queen_muteki_regist = {0};

PoseTableEntry queen_pose_table[] = {
    {POSE_TYPE_SLEEP, "KRT_S_1"},
    {POSE_TYPE_STOP, "KRT_S_1"},
    {POSE_TYPE_FREEZE, "KRT_S_1"},
    {POSE_TYPE_CONFUSE, "KRT_S_1"},
    {POSE_TYPE_POISON, "KRT_S_1"},
    {POSE_TYPE_DIZZY, "KRT_S_1"},
    {POSE_TYPE_NORMAL, "KRT_S_1"},
    {POSE_TYPE_DAMAGE_FIRE, "KRT_S_1"},
    {POSE_TYPE_DAMAGE_FIRE_2_UNUSED, "KRT_S_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "KRT_S_1"},
    {POSE_TYPE_DAMAGE, "KRT_S_1"},
    {POSE_TYPE_ATTACK, "KRT_S_1"},
    {POSE_TYPE_RUN, "KRT_S_1"},
    {POSE_TYPE_WALK, "KRT_S_1"},
    {POSE_TYPE_DANGER, "KRT_S_1"},
    {POSE_TYPE_PERIL, "KRT_S_1"},
    {POSE_TYPE_TALK, "KRT_T_1"},
    {POSE_TYPE_ACTION, "KRT_S_1"},
};

PoseTableEntry queen_hand_pose_table[] = {
    {POSE_TYPE_NORMAL, "KRT_S_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "KRT_S_1"},
    {POSE_TYPE_DAMAGE, "KRT_S_1"},
    {POSE_TYPE_ACTION, "KRT_S_1"},
};

BattleDataEntry queen_data_table[] = {
    {UNIT_DATA_DEAD_EVENT, queen_dead_event},
    {0, NULL},
};

BattleUnitKindPart queen_parts[] = {
    {1, "btl_un_shadow_queen", "c_b_peach_b", 0.0f, VERSION_PICK(0.0f, 2.0f), 0.0f, 0.0f, 47.0f, 0.0f, 0.0f, 47.0f, 0.0f, 41, 61, 255, &queen_defence, &queen_defence_attr, kMainPart | kGuardTargetPart, 0, peach_pose_table},
    {2, "btl_un_shadow_queen", "c_q_kage_b", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 41, 61, 255, &queen_defence, &queen_defence_attr, 0x2018000, 0, queen_pose_table},
    {3, "btl_un_shadow_queen", "c_q_kage_f", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 41, 61, 255, &queen_defence, &queen_defence_attr, 0x12018000, 0, queen_pose_table},
    {4, "btl_un_shadow_queen", "c_q_kage_h", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 41, 61, 255, &queen_defence, &queen_defence_attr, 0x12018000, 0, queen_pose_table},
    {5, "btl_un_shadow_queen", "c_q_kage_f", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 41, 61, 255, &queen_defence, &queen_defence_attr, 0x13018000, 0, queen_pose_table},
};

f32 hair_offset[] = {
    -13.0f, 63.0f, 5.0f, 57.0f, -24.0f, -40.0f, -10.0f, 61.0f,
    -7.0f, 24.0f, 17.0f, -16.0f, -10.0f, 60.0f, -9.0f, -27.0f,
    29.0f, 7.0f, -4.0f, 63.0f, -5.0f, 21.0f, -3.0f, 14.0f,
    -6.0f, 63.0f, 1.0f, 22.0f, -18.0f, -18.0f, -7.0f, 61.0f,
    0.0f, 13.0f, 0.0f, -21.0f,
};

// inline helper for pose_init: defined here so its Vec template lands at this .rodata position
inline void hand_light_init(void) {
    Mtx mtx;
    u8* light = lightGetPaperCraft();

    *(Vec*)(light + 0x24) = (Vec){-200.0f, 100.0f, 500.0f};
    PSMTXScale(mtx, 3.0f, 3.0f, 3.0f);
    PSMTXMultVec(mtx, (Vec*)(light + 0x24), (Vec*)(light + 0x24));
}

EVT_BEGIN(shadow_queen_1000)
    CALL(evt_npc_entry, STRING("dummy"), STRING("c_s_hand"))
    CALL(evt_npc_set_anim, STRING("dummy"), STRING("KRT_S_1"))
    CALL(evt_npc_set_position, STRING("dummy"), 0, -1000, 0)
    CALL(make_pattern)
    CALL(evt_ext_entry, 100, PTR(hand_data), PTR(hand_init), PTR(hand_main), 0)
    RETURN
EVT_END()

BattleUnitKind unit_boss_shadow_hand_1 = {
    UNIT_SHADOW_QUEEN_LEFT_RIGHT_HAND,
    "btl_un_shadow_queen",
    5,
    0,
    1,
    1,
    0,
    0,
    0,
    100,
    0,
    50,
    7,
    90,
    110,
    0,
    54,
    0.0f,
    27.0f,
    0.0f,
    0,
    0,
    40.0f,
    40.0f,
    0.0f,
    45.0f,
    0.0f,
    -10.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    25.0f,
    0.0f,
    0.0f,
    45.0f,
    71.5f,
    0.0f,
    -30.0f,
    55.0f,
    0.0f,
    75.0f,
    110.0f,
    100,
    0,
    -1,
    0,
    0,
    0,
    0.0f,
    0.0f,
    0.0f,
    "SFX_BOSS_QWEN_DAMAGED1",
    "SFX_BTL_DAMAGE_FIRE1",
    "SFX_BTL_DAMAGE_ICE1",
    "SFX_BTL_DAMAGE_BIRIBIRI1",
    kUnitNoHpGauge,
    &queen_hand_1_regist,
    1,
    queen_hand_1_parts,
    queen_hand_1_init_event,
    queen_hand_1_data_table,
};

BattleUnitDefense queen_hand_1_defence = {{0, 0, 0, 0, 0}};

BattleUnitDefenseAttr queen_hand_1_defence_attr = {{0, 0, 0, 0, 0}};

StatusVulnerability queen_hand_1_regist = {
    30, // sleep
    30, // stop
    30, // dizzy
    0, // poison
    30, // confuse
    0, // electric
    100, // burn
    0, // freeze
    0, // huge
    0, // tiny
    0, // attackUp
    0, // attackDown
    0, // defenseUp
    80, // defenseDown
    30, // allergic
    0, // fright
    0, // galeForce
    0, // fast
    0, // slow
    0, // dodgy
    0, // invisible
    0, // ohko
};

PoseTableEntry queen_hand_1_pose_table[] = {
    {POSE_TYPE_NORMAL, "KRT_S_1"},
    {POSE_TYPE_DAMAGE_FIRE, "KRT_Q_1"},
    {POSE_TYPE_DAMAGE_FIRE_2_UNUSED, "KRT_Q_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "KRT_D_1"},
    {POSE_TYPE_DAMAGE, "KRT_D_1"},
    {POSE_TYPE_DANGER, "KRT_S_1"},
    {POSE_TYPE_PERIL, "KRT_S_1"},
    {POSE_TYPE_ACTION, "KRT_S_1"},
};

BattleDataEntry queen_hand_1_data_table[] = {
    {UNIT_DATA_DEAD_EVENT, queen_hand_1_dead_event},
    {0, NULL},
};

BattleUnitKindPart queen_hand_1_parts[] = {
    {1, "btl_un_shadow_hand_big_1", "c_b_hand", 0.0f, 0.0f, 0.0f, 16.0f, 47.0f, 30.0f, 16.0f, 47.0f, 30.0f, 41, 61, 255, &queen_hand_1_defence, &queen_hand_1_defence_attr, 0x3018009, 0, queen_hand_1_pose_table},
};

BattleUnitKind unit_boss_shadow_hand_2 = {
    UNIT_SHADOW_QUEEN_LEFT_RIGHT_HAND,
    "btl_un_shadow_queen",
    5,
    0,
    1,
    1,
    0,
    0,
    0,
    100,
    0,
    50,
    7,
    90,
    110,
    0,
    54,
    0.0f,
    27.0f,
    0.0f,
    0,
    0,
    40.0f,
    40.0f,
    0.0f,
    45.0f,
    0.0f,
    -10.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    25.0f,
    0.0f,
    0.0f,
    45.0f,
    71.5f,
    0.0f,
    -30.0f,
    55.0f,
    0.0f,
    75.0f,
    110.0f,
    100,
    0,
    -1,
    0,
    0,
    0,
    0.0f,
    0.0f,
    0.0f,
    "SFX_BOSS_QWEN_DAMAGED1",
    "SFX_BTL_DAMAGE_FIRE1",
    "SFX_BTL_DAMAGE_ICE1",
    "SFX_BTL_DAMAGE_BIRIBIRI1",
    kUnitNoHpGauge,
    &queen_hand_2_regist,
    1,
    queen_hand_2_parts,
    queen_hand_2_init_event,
    queen_hand_2_data_table,
};

BattleUnitDefense queen_hand_2_defence = {{0, 0, 0, 0, 0}};

BattleUnitDefenseAttr queen_hand_2_defence_attr = {{0, 0, 0, 0, 0}};

StatusVulnerability queen_hand_2_regist = {
    30, // sleep
    30, // stop
    30, // dizzy
    0, // poison
    30, // confuse
    0, // electric
    100, // burn
    0, // freeze
    0, // huge
    0, // tiny
    0, // attackUp
    0, // attackDown
    0, // defenseUp
    80, // defenseDown
    30, // allergic
    0, // fright
    0, // galeForce
    0, // fast
    0, // slow
    0, // dodgy
    0, // invisible
    0, // ohko
};

PoseTableEntry queen_hand_2_pose_table[] = {
    {POSE_TYPE_NORMAL, "KRT_S_1"},
    {POSE_TYPE_DAMAGE_FIRE, "KRT_Q_1"},
    {POSE_TYPE_DAMAGE_FIRE_2_UNUSED, "KRT_Q_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "KRT_D_1"},
    {POSE_TYPE_DAMAGE, "KRT_D_1"},
    {POSE_TYPE_DANGER, "KRT_S_1"},
    {POSE_TYPE_PERIL, "KRT_S_1"},
    {POSE_TYPE_ACTION, "KRT_S_1"},
};

BattleDataEntry queen_hand_2_data_table[] = {
    {UNIT_DATA_DEAD_EVENT, queen_hand_2_dead_event},
    {0, NULL},
};

BattleUnitKindPart queen_hand_2_parts[] = {
    {1, "btl_un_shadow_hand_big_2", "c_b_hand", 0.0f, 0.0f, 0.0f, 28.0f, 56.0f, 20.0f, 28.0f, 56.0f, 20.0f, 41, 61, 255, &queen_hand_2_defence, &queen_hand_2_defence_attr, 0x3018009, 0, queen_hand_2_pose_table},
};

BattleUnitKind unit_boss_shadow_hand_small = {
    UNIT_SHADOW_QUEEN_DEAD_HANDS,
    "btl_un_shadow_queen",
    8,
    0,
    1,
    1,
    0,
    0,
    0,
    100,
    0,
    50,
    7,
    220,
    45,
    0,
    54,
    0.0f,
    27.0f,
    0.0f,
    0,
    0,
    40.0f,
    40.0f,
    0.0f,
    110.0f,
    0.0f,
    -10.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    110.0f,
    0.0f,
    0.0f,
    110.0f,
    29.25f,
    0.0f,
    0.0f,
    22.0f,
    0.0f,
    180.0f,
    45.0f,
    90,
    0,
    -1,
    0,
    0,
    0,
    0.0f,
    0.0f,
    0.0f,
    "SFX_BOSS_QWEN_DAMAGED1",
    "SFX_BTL_DAMAGE_FIRE1",
    "SFX_BTL_DAMAGE_ICE1",
    "SFX_BTL_DAMAGE_BIRIBIRI1",
    0,
    &queen_hand_small_regist,
    1,
    queen_hand_small_parts,
    queen_hand_small_init_event,
    queen_hand_small_data_table,
};

BattleUnitDefense queen_hand_small_defence = {{0, 0, 0, 0, 0}};

BattleUnitDefenseAttr queen_hand_small_defence_attr = {{0, 0, 0, 0, 0}};

StatusVulnerability queen_hand_small_regist = {
    30, // sleep
    30, // stop
    30, // dizzy
    0, // poison
    30, // confuse
    0, // electric
    100, // burn
    0, // freeze
    0, // huge
    0, // tiny
    0, // attackUp
    0, // attackDown
    0, // defenseUp
    80, // defenseDown
    30, // allergic
    0, // fright
    0, // galeForce
    0, // fast
    0, // slow
    0, // dodgy
    0, // invisible
    0, // ohko
};

PoseTableEntry queen_hand_small_pose_table[] = {
    {POSE_TYPE_NORMAL, "KRT_S_1"},
    {POSE_TYPE_DAMAGE_FIRE, "KRT_S_1"},
    {POSE_TYPE_DAMAGE_FIRE_2_UNUSED, "KRT_S_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "KRT_S_1"},
    {POSE_TYPE_DAMAGE, "KRT_S_1"},
    {POSE_TYPE_DANGER, "KRT_S_1"},
    {POSE_TYPE_PERIL, "KRT_S_1"},
    {POSE_TYPE_ACTION, "KRT_S_1"},
};

BattleDataEntry queen_hand_small_data_table[] = {
    {UNIT_DATA_DEAD_EVENT, queen_hand_small_dead_event},
    {0, NULL},
};

BattleUnitKindPart queen_hand_small_parts[] = {
    {1, "btl_un_shadow_hand_small", "c_b_hand", 0.0f, 0.0f, 0.0f, 70.0f, 18.0f, 0.0f, 70.0f, 18.0f, 0.0f, 188, 40, 255, &queen_hand_small_defence, &queen_hand_small_defence_attr, 0x3008009, 0, queen_hand_small_pose_table},
};

BattleUnitSetup entry_shadow_queen = {
    &unit_boss_shadow_queen, // mUnitKindParams
    ALLIANCE_ENEMY, // alliance
    {0}, // pad_05
    0x4000004, // attackPhase
    {75.0f, 0.0f, 0.0f}, // position
    0, // hitOffsetX
    {0}, // unitWork
    NULL, // itemDropTable
};

BattleUnitSetup entry_shadow_hand_1 = {
    &unit_boss_shadow_hand_1, // mUnitKindParams
    ALLIANCE_ENEMY, // alliance
    {0}, // pad_05
    0x4000004, // attackPhase
    {15.0f, 0.0f, -50.0f}, // position
    0, // hitOffsetX
    {0}, // unitWork
    NULL, // itemDropTable
};

BattleUnitSetup entry_shadow_hand_2 = {
    &unit_boss_shadow_hand_2, // mUnitKindParams
    ALLIANCE_ENEMY, // alliance
    {0}, // pad_05
    0x4000004, // attackPhase
    {160.0f, 0.0f, 50.0f}, // position
    0, // hitOffsetX
    {0}, // unitWork
    NULL, // itemDropTable
};

BattleUnitSetup entry_shadow_hand_small = {
    &unit_boss_shadow_hand_small, // mUnitKindParams
    ALLIANCE_ENEMY, // alliance
    {0}, // pad_05
    0x4000004, // attackPhase
    {95.0f, 0.0f, 0.0f}, // position
    0, // hitOffsetX
    {0}, // unitWork
    NULL, // itemDropTable
};

EVT_BEGIN(peach_init_event)
    SET_VALUE(GW(8), -1)
    SET_VALUE(GW(9), -1)
    SET_VALUE(GW(10), -1)
    SET_VALUE(GW(14), 0)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_ATTACK_STEP, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_ATTACK_STEP, 0)
        ELSE
        CALL(btlevtcmd_SpawnUnit, LW(0), PTR(&entry_shadow_queen), 0)
        CALL(btlevtcmd_SetPos, UNIT_ID_SELF, 0, -1000, 0)
        CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 1, kNotTargetable | kHidden | kNoShadow)
        CALL(btlevtcmd_OnAttribute, UNIT_ID_SELF, kUnitNoHpGauge | kUnitNoncombatant)
        CALL(btlevtcmd_SetMaxMoveCount, UNIT_ID_SELF, 0)
        CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 2, kHidden)
        RETURN
    END_IF
    CALL(btlevtcmd_GetUnitId, UNIT_ID_SELF, LW(0))
    SET_VALUE(GW(13), LW(0))
    BEGIN_INLINE_EVENT
        CALL(shadow_queen_connect)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(peach_talk_synchro_shadow_head_evt)
    END_INLINE_EVENT
    CALL(btlevtcmd_SetPartsBaseScale, UNIT_ID_SELF, 7, FLOAT(-1.0f), FLOAT(1.0f), FLOAT(1.0f))
    CALL(btlevtcmd_SetPartsBaseRotate, UNIT_ID_SELF, 6, 0, 315, 0)
    CALL(btlevtcmd_SetPartsBaseRotate, UNIT_ID_SELF, 7, 0, 315, 0)
    CALL(btlevtcmd_SetPartsPos, UNIT_ID_SELF, 6, 15, 0, -50)
    CALL(btlevtcmd_SetPartsPos, UNIT_ID_SELF, 7, 160, 0, 50)
    SPAWN_EVENT(PTR(shadow_queen_1000))
    BEGIN_INLINE_EVENT
        SET_FLOAT_VALUE(LW(0), FLOAT(360.0f))
        LOOP(0)
            CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, 2, FLOAT(0.0f), LW(0), FLOAT(3.6992188f))
            SUBTRACT_FLOAT(LW(0), FLOAT(0.71972656f))
            IF_FLOAT_LESS(LW(0), FLOAT(0.0f))
                ADD_FLOAT(LW(0), FLOAT(360.0f))
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 4, STRING("KRT_S_1"))
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(peach_wait_event))
    CALL(btlevtcmd_SetEventPhase, UNIT_ID_SELF, PTR(peach_phase_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(peach_attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(peach_damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_SetEventEntry, UNIT_ID_SELF, PTR(peach_entry_event))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

EVT_BEGIN(peach_entry_event)
    WAIT_MS(2000)
    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_OFF)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
    CALL(evt_btl_camera_set_moveto, 0, 83, 69, 332, 83, 46, 0, 30, 11)
    WAIT_MS(800)
    CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("stg8_las_140_02"), 0, -2)
    WAIT_MS(300)
    CALL(evt_btl_camera_set_moveto, 0, -122, 69, 332, -122, 46, 0, 15, 11)
    WAIT_MS(550)
    CALL(evt_msg_print_battle_party, STRING("stg8_las_141"))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_PARTNER)
    WAIT_MS(300)
    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_ON)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(queen_break_slot_flag2_on)
    RETURN
EVT_END()

EVT_BEGIN(peach_damage_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

EVT_BEGIN(peach_change_evt)
    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_OFF)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
    CALL(evt_btl_camera_set_moveto, 0, 83, 69, 332, 83, 46, 0, 30, 11)
    WAIT_MS(800)
    CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("stg8_las_141_01"), 0, -2)
    WAIT_MS(200)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_L_2"))
    WAIT_MS(200)
    CALL(btlevtcmd_SetTalkPose, UNIT_ID_SELF, STRING("KRT_T_3"))
    CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("stg8_las_141_02"), 0, -2)
    WAIT_MS(200)
    BEGIN_INLINE_EVENT
        CALL(evt_env_blur_on, 0, 500)
    END_INLINE_EVENT
    CALL(evt_btl_camera_set_moveto, 0, 83, 125, 565, 83, 86, 0, 30, 11)
    WAIT_MS(400)
    CALL(btlevtcmd_ClearAllStatus, UNIT_ID_SELF)
    WAIT_MS(400)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(evt_eff, STRING("m_q"), STRING("mist2"), 0, LW(0), 15, LW(2), 100, 0, -1, 0, 0, 0, 0, 0)
    ADD(LW(2), 30)
    CALL(evt_eff, STRING("m_q2"), STRING("queen"), 0, LW(0), 0, LW(2), FLOAT(1.0f), 330, 0, 0, 0, 0, 0, 0)
    CALL(eff_queen_set_alpha_evt, 90)
    CALL(evt_snd_sfxon_3d, STRING("SFX_BOSS_BPCH_QWEN_APPEAR2"), LW(0), LW(1), LW(2), 0)
    BEGIN_INLINE_EVENT
        CALL(btlevtcmd_OnUnitFlag, UNIT_ID_SELF, BATTLE_UNIT_FLAG_SOLID_MODEL)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_SetPartsPos, UNIT_ID_SELF, 2, LW(0), 0, LW(2))
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 3, STRING("KRT_O_1"))
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 2, STRING("KRT_S_1"))
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 4, STRING("KRT_S_1"))
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 5, STRING("KRT_O_2"))
        CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 2, 0)
        CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 3, 0)
        CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 4, 0)
        CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 5, 0)
        CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 2, kHidden)
        CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 3, kHidden)
        CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 4, kHidden)
        CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 5, kHidden)
        SET_VALUE(LW(0), 0)
        LOOP(63)
            CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 2, LW(0))
            CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 3, LW(0))
            CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 4, LW(0))
            CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 5, LW(0))
            ADD(LW(0), 4)
            WAIT_MS(50)
        END_LOOP
        CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 2, 255)
        CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 3, 255)
        CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 4, 255)
        CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 5, 255)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 3, STRING("KRT_S_1"))
        CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 5, kHidden)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(2000)
        CALL(btlevtcmd_SpawnUnit, LW(0), PTR(&entry_shadow_hand_1), 0)
        SET_VALUE(GW(8), LW(0))
        CALL(btlevtcmd_SpawnUnit, LW(0), PTR(&entry_shadow_hand_2), 0)
        SET_VALUE(GW(9), LW(0))
    END_INLINE_EVENT
    CALL(evt_snd_bgmoff_f, PSND_FADEOUT | PSND_SLOT(1), 2000)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    SET_FLOAT_VALUE(LW(1), LW(1))
    SET_FLOAT_VALUE(LW(3), FLOAT(0.0f))
    LOOP(100)
        ADD_FLOAT(LW(1), FLOAT(0.4091797f))
        ADD_FLOAT(LW(3), FLOAT(0.4091797f))
        CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        SET_FLOAT_VALUE(LW(4), LW(1))
        SUBTRACT_FLOAT(LW(4), LW(3))
        CALL(btlevtcmd_SetPartsPos, UNIT_ID_SELF, 2, LW(0), LW(4), LW(2))
        WAIT_MS(20)
    END_LOOP
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_L_1"))
    WAIT_MS(800)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_S_1"))
    WAIT_MS(500)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 3, STRING("KRT_T_1"))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
    CALL(evt_btl_camera_set_moveto, 0, 83, 141, 412, 83, 111, 0, 30, 11)
    CALL(evt_snd_bgmon, PSND_FADEOUT | PSND_SLOT(1), STRING("BGM_BOSS_STG8_LAST_BOSS1"))
    WAIT_MS(800)
    CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("stg8_las_141_03"), 0, -2)
    WAIT_MS(300)
    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_ON)
    CALL(btlevtcmd_GetPartsPos, UNIT_ID_SELF, 2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPartsHomePos, UNIT_ID_SELF, 2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPartsDefenceTable, UNIT_ID_SELF, 1, PTR(&queen_muteki_defence))
    CALL(btlevtcmd_SetPartsDefenceTable, GW(8), 1, PTR(&queen_muteki_defence))
    CALL(btlevtcmd_SetPartsDefenceTable, GW(9), 1, PTR(&queen_muteki_defence))
    CALL(btlevtcmd_SetPartsDefenceAttrTable, UNIT_ID_SELF, 1, PTR(&queen_muteki_defence_attr))
    CALL(btlevtcmd_SetPartsDefenceAttrTable, GW(8), 1, PTR(&queen_muteki_defence_attr))
    CALL(btlevtcmd_SetPartsDefenceAttrTable, GW(9), 1, PTR(&queen_muteki_defence_attr))
    CALL(btlevtcmd_SetRegistStatus, UNIT_ID_SELF, PTR(&queen_muteki_regist))
    CALL(btlevtcmd_SetRegistStatus, GW(8), PTR(&queen_muteki_regist))
    CALL(btlevtcmd_SetRegistStatus, GW(9), PTR(&queen_muteki_regist))
    CALL(btlevtcmd_OffPartsAttribute, GW(8), 1, kNotTargetable)
    CALL(btlevtcmd_OffPartsAttribute, GW(9), 1, kNotTargetable)
    CALL(btlevtcmd_OffAttribute, GW(8), kUnitNoHpGauge)
    CALL(btlevtcmd_OffAttribute, GW(9), kUnitNoHpGauge)
    CALL(btlevtcmd_SetHitOffset, UNIT_ID_SELF, 1, 34, -15, 30)
    CALL(btlevtcmd_SetHitCursorOffset, UNIT_ID_SELF, 1, 34, -15, 30)
    CALL(btlevtcmd_SetBintaHitOffset, UNIT_ID_SELF, 25, 0, 0)
    CALL(btlevtcmd_SetKissHitOffset, UNIT_ID_SELF, 50, 40, 0)
    CALL(btlevtcmd_SetEventPhase, UNIT_ID_SELF, PTR(queen_phase_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(queen_attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(queen_damage_event))
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(queen_wait_event))
    SET_VALUE(GW(11), 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_ATTACK_STEP, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_MESSAGE_STEP, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_HP_MESSAGE, 0)
    SET_VALUE(GW(10), -1)
    SET_VALUE(GW(12), 0)
    CALL(btlevtcmd_GetHp, UNIT_ID_SELF, LW(0))
    IF_INT_EQUAL(LW(0), 0)
#ifdef VERSION_G8ME01
        CALL(btlevtcmd_SetHp, UNIT_ID_SELF, 90)
#else
        CALL(btlevtcmd_SetHp, UNIT_ID_SELF, 112)
#endif
    END_IF
    CALL(btlevtcmd_GetHp, UNIT_ID_SELF, LW(0))
    SET_VALUE(GSW_LAS_SHADOW_QUEEN_HP_UNUSED, LW(0))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    RETURN
EVT_END()

EVT_BEGIN(peach_phase_event)
    CALL(btlevtcmd_CheckActStatus, UNIT_ID_SELF, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_ATTACK_STEP, LW(0))
    IF_INT_EQUAL(LW(0), 4)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_CheckPhase, LW(0), 67108867)
    IF_INT_NOT_EQUAL(LW(0), 1)
        RETURN
    END_IF
    CALL(btlevtcmd_GetHp, UNIT_ID_SELF, LW(0))
#ifdef VERSION_G8ME01
    IF_INT_LESS(LW(0), 90)
#else
    IF_INT_LESS(LW(0), 112)
#endif
        CALL_CHILD(PTR(peach_change_evt))
        CALL(btlevtcmd_reset_turn)
    END_IF
    LABEL(99)
    RETURN
EVT_END()

EVT_BEGIN(peach_dead_event)
    CALL(btlevtcmd_WaitAttackEnd)
    CALL_CHILD(PTR(peach_change_evt))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    CALL(btlevtcmd_reset_turn)
    RETURN
EVT_END()

EVT_BEGIN(peack_attack_black_thunder)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_peach_black_thunder_attack))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_peach_black_thunder_attack), LW(3), LW(4))
    CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(PTR(subsetevt_confuse_flustered))
        RETURN
    END_IF
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_peach_black_thunder_attack))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_A_1"))
    WAIT_MS(500)
    WAIT_MS(700)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(queen_thunder, LW(0), LW(1), LW(2))
    WAIT_MS(80)
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_peach_black_thunder_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    SWITCH(LW(5))
        CASE_OR_EQUAL(4)
        GOTO(90)
        CASE_END
        CASE_EQUAL(3)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
        GOTO(90)
        CASE_END
        CASE_EQUAL(6)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
        GOTO(90)
        CASE_EQUAL(2)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
        GOTO(90)
        CASE_EQUAL(1)
        GOTO(91)
        CASE_END
        CASE_DEFAULT
        GOTO(98)
        CASE_END
    END_SWITCH
    LABEL(90)
    GOTO(95)
    LABEL(91)
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_peach_black_thunder_attack))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_peach_black_thunder_attack))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_peach_black_thunder_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    LABEL(95)
    LABEL(98)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    LABEL(99)
    RETURN
EVT_END()

EVT_BEGIN(peack_attack_black_energy)
    CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(PTR(subsetevt_confuse_flustered))
        RETURN
    END_IF
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, -1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 400)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_S_1"))
    WAIT_MS(200)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_BOSS_BPCH_HAND_UP1"), LW(0), LW(1), LW(2), 0)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_A_2"))
    WAIT_MS(750)
    BEGIN_INLINE_EVENT
        CALL(evt_env_blur_on, 0, 1000)
    END_INLINE_EVENT
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHeight, UNIT_ID_SELF, LW(5))
    ADD(LW(1), LW(5))
    ADD(LW(1), -15)
    CALL(evt_eff, STRING(""), STRING("charge"), 2, LW(0), LW(1), LW(2), FLOAT(1.0f), 60, 0, 0, 0, 0, 0, 0)
    CALL(evt_snd_sfxon_3d, STRING("SFX_BOSS_BPCH_DARK_POWER1"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(1000)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_peach_black_energy_attack))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, UNIT_ID_SELF, 1, PTR(&weapon_peach_black_energy_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    RETURN
EVT_END()

EVT_BEGIN(peack_attack_dark_drag)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_peach_dark_drag_attack))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_peach_dark_drag_attack), LW(3), LW(4))
    CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(PTR(subsetevt_confuse_flustered))
        RETURN
    END_IF
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_peach_dark_drag_attack))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_S_1"))
    WAIT_MS(200)
    BEGIN_INLINE_EVENT
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_BOSS_BPCH_QWEN_APPEAR1"), LW(0), LW(1), LW(2), LW(10))
        CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 2, 0)
        CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 3, 0)
        CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 4, 0)
        CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 6, 0)
        CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 7, 0)
        CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 2, kHidden)
        CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 3, kHidden)
        CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 4, kHidden)
        CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 6, kHidden)
        CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 7, kHidden)
        SET_VALUE(LW(0), 0)
        LOOP(6)
            CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 2, LW(0))
            CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 3, LW(0))
            CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 4, LW(0))
            CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 6, LW(0))
            CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 7, LW(0))
            ADD(LW(0), 16)
            WAIT_MS(5)
        END_LOOP
    END_INLINE_EVENT
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_BOSS_BPCH_HAND_UP1"), LW(0), LW(1), LW(2), 0)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_A_2"))
    WAIT_MS(500)
    CALL(evt_btl_camera_set_homing_unit, 0, LW(3), LW(4))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    WAIT_MS(500)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, LW(3), LW(4))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_BOSS_BPCH_HAND_APPEAR1"), LW(0), LW(1), LW(2), 0)
    BEGIN_INLINE_EVENT
        CALL(peach_hand1000_dark_drag_1_evt)
    END_INLINE_EVENT
    CALL(evt_eff, STRING("m_h_s1"), STRING("mist2"), 0, -100, 15, 0, 100, 0, -1, 0, 0, 0, 0, 0)
    CALL(evt_eff, STRING("m_h_s2"), STRING("mist2"), 0, -100, 15, 0, 100, 0, -1, 0, 0, 0, 0, 0)
    CALL(evt_eff, STRING("m_h_s3"), STRING("mist2"), 0, -100, 15, 0, 100, 0, -1, 0, 0, 0, 0, 0)
    WAIT_MS(1250)
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_peach_dark_drag_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    SWITCH(LW(5))
        CASE_OR_EQUAL(4)
        GOTO(90)
        CASE_END
        CASE_EQUAL(3)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
        GOTO(90)
        CASE_END
        CASE_EQUAL(6)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
        GOTO(90)
        CASE_EQUAL(2)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
        GOTO(90)
        CASE_EQUAL(1)
        GOTO(91)
        CASE_END
        CASE_DEFAULT
        GOTO(98)
        CASE_END
    END_SWITCH
    LABEL(90)
    WAIT_MS(2250)
    CALL(evt_eff_softdelete, STRING("m_h_s1"))
    CALL(evt_eff_softdelete, STRING("m_h_s2"))
    CALL(evt_eff_softdelete, STRING("m_h_s3"))
    GOTO(95)
    LABEL(91)
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_peach_dark_drag_attack))
    CALL(battle_evt_majo_disp_off, LW(3), LW(4), 0, 0)
    CALL(btlevtcmd_SetScale, LW(3), 0, 0, 0)
    CALL(btlevtcmd_GetHomePos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_peach_dark_drag_attack))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_peach_dark_drag_attack), 0, LW(5))
    WAIT_MS(750)
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_peach_dark_drag_attack))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_peach_dark_drag_attack), 0, LW(5))
    WAIT_MS(750)
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_peach_dark_drag_attack))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_peach_dark_drag_attack), 0, LW(5))
    WAIT_MS(750)
    CALL(evt_eff_softdelete, STRING("m_h_s1"))
    CALL(evt_eff_softdelete, STRING("m_h_s2"))
    CALL(evt_eff_softdelete, STRING("m_h_s3"))
    CALL(btlevtcmd_GetHomePos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetScale, LW(3), 1, 1, 1)
    CALL(battle_evt_majo_disp_on, LW(3), LW(4), LW(0), LW(1), LW(2), 0)
    CALL(btlevtcmd_SetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_peach_dark_drag_attack))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_peach_dark_drag_last_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    WAIT_MS(750)
    LABEL(95)
    LABEL(98)
    CALL(evt_snd_sfxoff, LW(10))
    BEGIN_INLINE_EVENT
        SET_VALUE(LW(0), 100)
        LOOP(6)
            CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 2, LW(0))
            CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 3, LW(0))
            CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 4, LW(0))
            CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 6, LW(0))
            CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 7, LW(0))
            ADD(LW(0), -16)
            WAIT_MS(5)
        END_LOOP
        CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 2, kHidden)
        CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 3, kHidden)
        CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 4, kHidden)
        CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 6, kHidden)
        CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 7, kHidden)
    END_INLINE_EVENT
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    RETURN
EVT_END()

EVT_BEGIN(peack_attack_drain)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_peach_drain_attack))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_peach_drain_attack), LW(3), LW(4))
    CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(PTR(subsetevt_confuse_flustered))
        RETURN
    END_IF
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_peach_drain_attack))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_S_1"))
    WAIT_MS(200)
    BEGIN_INLINE_EVENT
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_BOSS_BPCH_QWEN_APPEAR1"), LW(0), LW(1), LW(2), LW(10))
        CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 2, 0)
        CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 3, 0)
        CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 4, 0)
        CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 7, 0)
        CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 2, kHidden)
        CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 3, kHidden)
        CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 4, kHidden)
        CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 6, kHidden)
        CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 7, kHidden)
        SET_VALUE(LW(0), 0)
        LOOP(6)
            CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 2, LW(0))
            CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 3, LW(0))
            CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 4, LW(0))
            CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 7, LW(0))
            ADD(LW(0), 16)
            WAIT_MS(5)
        END_LOOP
    END_INLINE_EVENT
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_BOSS_BPCH_HAND_UP1"), LW(0), LW(1), LW(2), 0)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_A_2"))
    WAIT_MS(500)
    CALL(evt_btl_camera_set_homing_unit, 0, LW(3), LW(4))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    WAIT_MS(500)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, LW(3), LW(4))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 6, STRING("KRT_S_1"))
    CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 6, 0)
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 6, kHidden)
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(evt_eff, STRING("m_h_1"), STRING("mist2"), 0, LW(0), 15, LW(2), 50, 0, -1, 0, 0, 0, 0, 0)
    ADD(LW(0), 20)
    SET_VALUE(LW(1), 0)
    ADD(LW(2), -30)
    CALL(btlevtcmd_SetPartsPos, UNIT_ID_SELF, 6, LW(0), LW(1), LW(2))
    SET_VALUE(LW(0), 0)
    LOOP(15)
        CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 6, LW(0))
        ADD(LW(0), 16)
        WAIT_MS(5)
    END_LOOP
    CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 6, 255)
    WAIT_MS(500)
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_BOSS_BPCH_DRAIN1"), LW(0), LW(1), LW(2), 0)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 6, STRING("KRT_A2_1"))
    BEGIN_INLINE_EVENT
        WAIT_MS(300)
        CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
        ADD(LW(1), 40)
        CALL(queen_drain, LW(0), LW(1), LW(2))
        WAIT_MS(600)
        CALL(queen_drain, LW(0), LW(1), LW(2))
        WAIT_MS(600)
        CALL(queen_drain, LW(0), LW(1), LW(2))
        WAIT_MS(600)
    END_INLINE_EVENT
    WAIT_MS(2000)
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_peach_drain_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    SWITCH(LW(5))
        CASE_OR_EQUAL(4)
        GOTO(90)
        CASE_END
        CASE_EQUAL(3)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
        GOTO(90)
        CASE_END
        CASE_EQUAL(6)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
        GOTO(90)
        CASE_EQUAL(2)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
        GOTO(90)
        CASE_EQUAL(1)
        GOTO(91)
        CASE_END
        CASE_DEFAULT
        GOTO(98)
        CASE_END
    END_SWITCH
    LABEL(90)
    GOTO(95)
    LABEL(91)
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_peach_drain_attack))
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_peach_drain_attack))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_peach_drain_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    CALL(btlevtcmd_GetHpDamage, LW(3), LW(0))
    CALL(btlevtcmd_RecoverHp, UNIT_ID_SELF, 1, LW(0))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(5), LW(6), LW(7))
    CALL(btlevtcmd_GetHeight, UNIT_ID_SELF, LW(8))
    ADD(LW(6), LW(8))
    ADD(LW(7), 30)
    CALL(evt_eff, STRING(""), STRING("recovery"), 0, LW(5), LW(6), LW(7), LW(0), 0, 0, 0, 0, 0, 0, 0)
    LABEL(95)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_eff_softdelete, STRING("m_h_1"))
    CALL(evt_snd_sfxoff, LW(10))
    BEGIN_INLINE_EVENT
        SET_VALUE(LW(0), 100)
        LOOP(6)
            CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 2, LW(0))
            CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 3, LW(0))
            CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 4, LW(0))
            CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 7, LW(0))
            ADD(LW(0), -16)
            WAIT_MS(5)
        END_LOOP
        CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 2, kHidden)
        CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 3, kHidden)
        CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 4, kHidden)
        CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 7, kHidden)
    END_INLINE_EVENT
    SET_VALUE(LW(0), 255)
    LOOP(15)
        CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 6, LW(0))
        ADD(LW(0), -16)
        WAIT_MS(5)
    END_LOOP
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 6, kHidden)
    CALL(btlevtcmd_SetPartsPos, UNIT_ID_SELF, 6, 15, 0, -50)
    LABEL(99)
    RETURN
EVT_END()

EVT_BEGIN(peach_attack_event)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_ATTACK_STEP, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_ATTACK_STEP, 1)
        CALL_CHILD(PTR(peack_attack_black_thunder))
        CASE_EQUAL(1)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_ATTACK_STEP, 2)
        CALL_CHILD(PTR(peack_attack_black_energy))
        CASE_EQUAL(2)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_ATTACK_STEP, 3)
        CALL_CHILD(PTR(peack_attack_dark_drag))
        CASE_EQUAL(3)
        CALL(btlevtcmd_DrawLots, LW(0), 4, 100, 30, 30, 30)
        CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_DEFENSE_UP, LW(1))
        IF_INT_EQUAL(LW(1), 1)
            IF_INT_EQUAL(LW(0), 1)
                SET_VALUE(LW(0), 0)
            END_IF
        END_IF
        SWITCH(LW(0))
            CASE_EQUAL(0)
            CALL_CHILD(PTR(peack_attack_black_thunder))
            CASE_EQUAL(1)
            CALL_CHILD(PTR(peack_attack_black_energy))
            CASE_EQUAL(2)
            CALL_CHILD(PTR(peack_attack_drain))
            CASE_EQUAL(3)
            CALL_CHILD(PTR(peack_attack_dark_drag))
        END_SWITCH
    END_SWITCH
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

EVT_BEGIN(peach_wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

EVT_BEGIN(queen_init_event)
    CALL(btlevtcmd_GetMaxHp, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SetHp, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(queen_wait_event))
    CALL(btlevtcmd_SetEventPhase, UNIT_ID_SELF, PTR(queen_phase_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(queen_attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(queen_damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_SetEventEntry, UNIT_ID_SELF, PTR(queen_entry_event))
    CALL(btlevtcmd_OnUnitFlag, UNIT_ID_SELF, BATTLE_UNIT_FLAG_SOLID_MODEL)
    SET_VALUE(GW(11), 1)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_ATTACK_STEP, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_MESSAGE_STEP, 3)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_HP_MESSAGE, 0)
    SET_VALUE(GW(10), -1)
    SET_VALUE(GW(12), 300)
    SET_VALUE(GW(18), 0)
    CALL(queen_kankyaku_delete_evt)
    CALL(btlevtcmd_SpawnUnit, LW(0), PTR(&entry_shadow_hand_1), 0)
    SET_VALUE(GW(8), LW(0))
    CALL(btlevtcmd_SpawnUnit, LW(0), PTR(&entry_shadow_hand_2), 0)
    SET_VALUE(GW(9), LW(0))
    CALL(btlevtcmd_GetUnitId, UNIT_ID_SELF, LW(0))
    SET_VALUE(GW(13), LW(0))
    BEGIN_INLINE_EVENT
        SET_FLOAT_VALUE(LW(0), FLOAT(360.0f))
        LOOP(0)
            CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, 2, FLOAT(0.0f), LW(0), FLOAT(3.6992188f))
            SUBTRACT_FLOAT(LW(0), FLOAT(0.71972656f))
            IF_FLOAT_LESS(LW(0), FLOAT(0.0f))
                ADD_FLOAT(LW(0), FLOAT(360.0f))
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    ADD(LW(1), 40)
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    SET_VALUE(LW(1), 0)
    CALL(btlevtcmd_SetPartsPos, UNIT_ID_SELF, 2, LW(0), LW(1), LW(2))
    BEGIN_INLINE_EVENT
        CALL(shadow_queen_connect)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(peach_talk_synchro_shadow_head_evt)
    END_INLINE_EVENT
    SPAWN_EVENT(PTR(shadow_queen_1000))
    CALL(btlevtcmd_OffPartsAttribute, GW(8), 1, kNotTargetable)
    CALL(btlevtcmd_OffPartsAttribute, GW(9), 1, kNotTargetable)
    CALL(btlevtcmd_OffAttribute, GW(8), kUnitNoHpGauge)
    CALL(btlevtcmd_OffAttribute, GW(9), kUnitNoHpGauge)
    CALL(btlevtcmd_SetHitOffset, UNIT_ID_SELF, 1, 34, -15, 30)
    CALL(btlevtcmd_SetHitCursorOffset, UNIT_ID_SELF, 1, 34, -15, 30)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 4, STRING("KRT_S_1"))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 3, STRING("KRT_S_1"))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(evt_eff, STRING("m_q"), STRING("mist2"), 0, LW(0), 15, LW(2), 100, 0, -1, 0, 0, 0, 0, 0)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

EVT_BEGIN(queen_first_attack_message_evt)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_MESSAGE_STEP, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        RETURN
    END_IF
    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_OFF)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
    CALL(evt_btl_camera_set_moveto, 0, 63, 141, 412, 63, 111, 0, 30, 11)
    WAIT_MS(800)
    CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("stg8_las_142"), 0, -2)
    WAIT_MS(300)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_MESSAGE_STEP, 1)
    SET_VALUE(GW(12), 100)
    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_ON)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    RETURN
EVT_END()

EVT_BEGIN(queen_next_attack_message_evt)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_MESSAGE_STEP, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 1)
        RETURN
    END_IF
    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_OFF)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
    CALL(evt_btl_camera_set_moveto, 0, 63, 141, 412, 63, 111, 0, 30, 11)
    WAIT_MS(800)
    CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("stg8_las_143"), 0, -2)
    WAIT_MS(300)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_MESSAGE_STEP, 2)
    SET_VALUE(GW(12), 200)
    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_ON)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    RETURN
EVT_END()

EVT_BEGIN(queen_last_attack_message_evt)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_MESSAGE_STEP, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 2)
        RETURN
    END_IF
    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_OFF)
    CALL(evt_btl_camera_set_moveto, 0, -113, 69, 332, -113, 46, 0, 15, 11)
    WAIT_MS(550)
    CALL(evt_msg_print_battle_party, STRING("stg8_las_145"))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_PARTNER)
    WAIT_MS(300)
    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_ON)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_MESSAGE_STEP, 3)
    SET_VALUE(GW(12), 300)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    WAIT_MS(1000)
    CALL(btlevtcmd_InterruptStop, 1)
    RETURN
EVT_END()

EVT_BEGIN(queen_change_hand_small_evt)
    CALL(btlevtcmd_GetUnitWork, GW(13), UNIT_WORK_ATTACK_STEP, LW(0))
    IF_INT_EQUAL(LW(0), 2)
        RETURN
    END_IF
    SET_VALUE(GF(1), 1)
    BEGIN_INLINE_EVENT
        IF_INT_NOT_EQUAL(GW(8), -1)
            SET_VALUE(GF(1), 0)
            CALL_CHILD(PTR(queen_hand_1_dead_event))
        END_IF
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        IF_INT_NOT_EQUAL(GW(9), -1)
            SET_VALUE(GF(1), 0)
            CALL_CHILD(PTR(queen_hand_2_dead_event))
        END_IF
    END_INLINE_EVENT
    WAIT_MS(300)
    SET_VALUE(GF(0), 0)
    CALL(btlevtcmd_SpawnUnit, LW(0), PTR(&entry_shadow_hand_small), 0)
    SET_VALUE(GW(10), LW(0))
    LOOP(0)
        IF_INT_EQUAL(GF(0), 1)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    LOOP(0)
        IF_INT_EQUAL(GF(1), 1)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_MS(1000)
    SET_VALUE(GW(11), 2)
    RETURN
EVT_END()

EVT_BEGIN(queen_change_hand_big_evt)
    CALL(btlevtcmd_GetUnitWork, GW(13), UNIT_WORK_ATTACK_STEP, LW(0))
    IF_INT_EQUAL(LW(0), 2)
        RETURN
    END_IF
    SET_VALUE(GF(1), 1)
    BEGIN_INLINE_EVENT
        IF_INT_NOT_EQUAL(GW(10), -1)
            SET_VALUE(GF(1), 0)
            CALL_CHILD(PTR(queen_hand_small_dead_event))
        END_IF
    END_INLINE_EVENT
    WAIT_MS(300)
    SET_VALUE(GF(0), 0)
    CALL(btlevtcmd_SpawnUnit, LW(0), PTR(&entry_shadow_hand_1), 0)
    SET_VALUE(GW(8), LW(0))
    CALL(btlevtcmd_SpawnUnit, LW(0), PTR(&entry_shadow_hand_2), 0)
    SET_VALUE(GW(9), LW(0))
    CALL(btlevtcmd_OffPartsAttribute, GW(8), 1, kNotTargetable)
    CALL(btlevtcmd_OffPartsAttribute, GW(9), 1, kNotTargetable)
    CALL(btlevtcmd_OffAttribute, GW(8), kUnitNoHpGauge)
    CALL(btlevtcmd_OffAttribute, GW(9), kUnitNoHpGauge)
    LOOP(0)
        IF_INT_EQUAL(GF(0), 1)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    LOOP(0)
        IF_INT_EQUAL(GF(1), 1)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_MS(1000)
    SET_VALUE(GW(11), 1)
    RETURN
EVT_END()

EVT_BEGIN(queen_hand_big_life_check_evt)
    CALL(btlevtcmd_GetUnitWork, GW(13), UNIT_WORK_ATTACK_STEP, LW(0))
    IF_INT_EQUAL(LW(0), 2)
        RETURN
    END_IF
    SET_VALUE(GF(0), 1)
    IF_INT_EQUAL(GW(8), -1)
        SET_VALUE(GF(0), 0)
        CALL(btlevtcmd_SpawnUnit, LW(0), PTR(&entry_shadow_hand_1), 0)
        SET_VALUE(GW(8), LW(0))
        CALL(btlevtcmd_OffPartsAttribute, GW(8), 1, kNotTargetable)
        CALL(btlevtcmd_OffAttribute, GW(8), kUnitNoHpGauge)
    END_IF
    IF_INT_EQUAL(GW(9), -1)
        SET_VALUE(GF(0), 0)
        CALL(btlevtcmd_SpawnUnit, LW(0), PTR(&entry_shadow_hand_2), 0)
        SET_VALUE(GW(9), LW(0))
        CALL(btlevtcmd_OffPartsAttribute, GW(9), 1, kNotTargetable)
        CALL(btlevtcmd_OffAttribute, GW(9), kUnitNoHpGauge)
    END_IF
    LOOP(0)
        IF_INT_EQUAL(GF(0), 1)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_MS(1000)
    RETURN
EVT_END()

EVT_BEGIN(queen_hand_small_life_check_evt)
    CALL(btlevtcmd_GetUnitWork, GW(13), UNIT_WORK_ATTACK_STEP, LW(0))
    IF_INT_EQUAL(LW(0), 2)
        RETURN
    END_IF
    IF_INT_NOT_EQUAL(GW(10), -1)
        RETURN
    END_IF
    SET_VALUE(GF(0), 0)
    CALL(btlevtcmd_SpawnUnit, LW(0), PTR(&entry_shadow_hand_small), 0)
    SET_VALUE(GW(10), LW(0))
    LOOP(0)
        IF_INT_EQUAL(GF(0), 1)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_MS(1000)
    RETURN
EVT_END()

EVT_BEGIN(queen_hp_drain_evt)
    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_OFF)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
    CALL(evt_btl_camera_set_moveto, 0, 63, 141, 412, 63, 111, 0, 30, 11)
    WAIT_MS(800)
    CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("stg8_las_144"), 0, -2)
    WAIT_MS(300)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    BEGIN_INLINE_EVENT
        IF_INT_NOT_EQUAL(GW(8), -1)
            CALL_CHILD(PTR(queen_hand_1_dead_event))
        END_IF
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        IF_INT_NOT_EQUAL(GW(9), -1)
            CALL_CHILD(PTR(queen_hand_2_dead_event))
        END_IF
    END_INLINE_EVENT
    IF_INT_EQUAL(GW(10), -1)
        CALL(btlevtcmd_SpawnUnit, LW(0), PTR(&entry_shadow_hand_small), 0)
        SET_VALUE(GW(10), LW(0))
        WAIT_MS(1000)
    END_IF
    CALL(evt_eff_softdelete, STRING("m_h_1"))
    CALL(evt_eff_softdelete, STRING("m_h_2"))
    CALL(evt_eff_softdelete, STRING("m_h_3"))
    WAIT_MS(1000)
    CALL(queen_get_aud_num, LW(10))
    CALL(evt_snd_sfxon, STRING("SFX_BOSS_QWEN_HAND_GOOUT1"), 0)
    CALL(queen_hp_drain_1_evt)
    CALL(evt_eff, STRING("m_h_s1_"), STRING("mist2"), 0, 0, -40, 0, 400, 0, -1, 0, 0, 0, 0, 0)
    CALL(evt_eff, STRING("m_h_s2_"), STRING("mist2"), 0, 0, -25, 0, 400, 0, -1, 0, 0, 0, 0, 0)
    CALL(evt_eff, STRING("m_h_s3_"), STRING("mist2"), 0, 0, -35, 0, 400, 0, -1, 0, 0, 0, 0, 0)
    CALL(evt_eff, STRING("m_h_s4_"), STRING("mist2"), 0, 0, -15, 0, 400, 0, -1, 0, 0, 0, 0, 0)
    CALL(evt_btl_camera_shake_h, 0, 1, 1, 240, 0)
    CALL(evt_snd_sfxon, STRING("SFX_BOSS_QWENH_HAND_APPEAR2"), 0)
    CALL(evt_snd_sfxon, STRING("SFX_AUDIENCE_PANIC_SCREAM1"), LW(11))
    CALL(queen_hp_drain_2_evt)
    WAIT_MS(1000)
    BEGIN_INLINE_EVENT
        WAIT_MS(300)
        CALL(queen_hp_drain_3_evt)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        SET_VALUE(LW(0), 127)
        LOOP(63)
            SUBTRACT(LW(0), 2)
            CALL(evt_snd_sfx_vol, LW(11), LW(0))
            WAIT_MS(31)
        END_LOOP
        CALL(evt_snd_sfxoff, LW(11))
    END_INLINE_EVENT
    CALL(queen_hp_drain_4_evt)
    IF_INT_GREATER(LW(10), 7)
        SET_VALUE(LW(10), 7)
    END_IF
    IF_INT_GREATER(LW(10), 99)
        SET_VALUE(LW(10), 99)
    END_IF
    CALL(btlevtcmd_GetMaxHp, UNIT_ID_SELF, LW(10))
    CALL(btlevtcmd_RecoverHp, GW(13), 1, LW(10))
    CALL(btlevtcmd_GetPos, GW(13), LW(5), LW(6), LW(7))
    CALL(btlevtcmd_GetHeight, GW(13), LW(8))
    ADD(LW(6), LW(8))
    ADD(LW(7), 30)
    CALL(evt_eff, STRING(""), STRING("recovery"), 4, LW(5), LW(6), LW(7), 0, 0, 0, 0, 0, 0, 0, 0)
    WAIT_MS(1500)
    CALL(evt_batstage_return_aud_dark_base, 0, 0)
    CALL(evt_snd_sfxon, STRING("SFX_BOSS_QWEN_HAND_APPEAR1_2"), 0)
    CALL(queen_hp_drain_5_evt)
    CALL(evt_eff_softdelete, STRING("m_h_s1_"))
    CALL(evt_eff_softdelete, STRING("m_h_s2_"))
    CALL(evt_eff_softdelete, STRING("m_h_s3_"))
    CALL(evt_eff_softdelete, STRING("m_h_s4_"))
    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_OFF)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
    CALL(evt_btl_camera_set_moveto, 0, 63, 141, 412, 63, 111, 0, 30, 11)
    WAIT_MS(800)
    CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("stg8_las_142_01"), 0, -2)
    WAIT_MS(300)
    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_ON)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    WAIT_MS(1000)
    SET_VALUE(GW(11), 2)
    CALL(btlevtcmd_reset_turn)
    RETURN
EVT_END()

EVT_BEGIN(queen_hp_drain_evt_miss)
    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_OFF)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
    CALL(evt_btl_camera_set_moveto, 0, 63, 141, 412, 63, 111, 0, 30, 11)
    WAIT_MS(800)
    CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("stg8_las_144"), 0, -2)
    WAIT_MS(300)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    WAIT_MS(1000)
    SET_VALUE(GW(11), 2)
    CALL(btlevtcmd_reset_turn)
    RETURN
EVT_END()

EVT_BEGIN(queen_first_damage_evt)
    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_OFF)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    WAIT_MS(800)
    CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("stg8_las_163"), 0, -2)
    WAIT_MS(300)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
    CALL(evt_btl_camera_set_moveto, 0, -113, 69, 332, -113, 46, 0, 30, 11)
    WAIT_MS(800)
    CALL(evt_msg_print_battle_party, STRING("stg8_las_164"))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_PARTNER)
    WAIT_MS(300)
    CALL(evt_btl_camera_set_moveto, 0, 63, 141, 412, 63, 111, 0, 1, 11)
    WAIT_MS(350)
    CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("stg8_las_165"), 0, -2)
    WAIT_MS(300)
    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_ON)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    WAIT_MS(1000)
    RETURN
EVT_END()

EVT_BEGIN(queen_2_hp_msg_evt)
    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_OFF)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
    CALL(evt_btl_camera_set_moveto, 0, 63, 141, 412, 63, 111, 0, 30, 11)
    WAIT_MS(800)
    CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("stg8_las_166"), 0, -2)
    WAIT_MS(300)
    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_ON)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    WAIT_MS(1000)
    RETURN
EVT_END()

EVT_BEGIN(queen_4_hp_msg_evt)
    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_OFF)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
    CALL(evt_btl_camera_set_moveto, 0, 63, 141, 412, 63, 111, 0, 30, 11)
    WAIT_MS(800)
    CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("stg8_las_167"), 0, -2)
    WAIT_MS(300)
    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_ON)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    WAIT_MS(1000)
    RETURN
EVT_END()

EVT_BEGIN(queen_dead_hp_msg_evt)
    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_OFF)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
    CALL(evt_btl_camera_set_moveto, 0, 63, 141, 412, 63, 111, 0, 30, 11)
    WAIT_MS(800)
    CALL(btlevtcmd_SetTalkPose, UNIT_ID_SELF, STRING("KRT_D_1"))
    CALL(btlevtcmd_SetStayPose, UNIT_ID_SELF, STRING("KRT_D_1"))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 2, STRING("KRT_S_1"))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 3, STRING("KRT_D_1"))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 4, STRING("KRT_S_1"))
    IF_INT_NOT_EQUAL(GW(8), -1)
        CALL(btlevtcmd_AnimeChangePose, GW(8), 1, STRING("KRT_D_1"))
    END_IF
    IF_INT_NOT_EQUAL(GW(9), -1)
        CALL(btlevtcmd_AnimeChangePose, GW(9), 1, STRING("KRT_D_1"))
    END_IF
    CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("stg8_las_168"), 0, -2)
    WAIT_MS(300)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_S_1"))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    WAIT_MS(1000)
    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_ON)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

EVT_BEGIN(queen_phase_event)
    CALL(btlevtcmd_CheckActStatus, UNIT_ID_SELF, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        GOTO(99)
    END_IF
    IF_INT_NOT_EQUAL(GW(11), 0)
        IF_INT_EQUAL(GW(18), 1)
            SET_VALUE(GW(18), 2)
            CALL_CHILD(PTR(queen_first_damage_evt))
        END_IF
        CALL(btlevtcmd_GetMaxHp, GW(13), LW(0))
        CALL(btlevtcmd_GetHp, GW(13), LW(1))
        DIVIDE(LW(0), 4)
        IF_INT_LESS(LW(1), LW(0))
            CALL(btlevtcmd_GetUnitWork, GW(13), UNIT_WORK_HP_MESSAGE, LW(0))
            IF_INT_LESS_OR_EQUAL(LW(0), 1)
                CALL(btlevtcmd_SetUnitWork, GW(13), UNIT_WORK_HP_MESSAGE, 2)
                CALL_CHILD(PTR(queen_4_hp_msg_evt))
            END_IF
        END_IF
        CALL(btlevtcmd_GetMaxHp, GW(13), LW(0))
        CALL(btlevtcmd_GetHp, GW(13), LW(1))
        DIVIDE(LW(0), 2)
        IF_INT_LESS(LW(1), LW(0))
            CALL(btlevtcmd_GetUnitWork, GW(13), UNIT_WORK_HP_MESSAGE, LW(0))
            IF_INT_LESS_OR_EQUAL(LW(0), 0)
                CALL(btlevtcmd_SetUnitWork, GW(13), UNIT_WORK_HP_MESSAGE, 1)
                CALL_CHILD(PTR(queen_2_hp_msg_evt))
            END_IF
        END_IF
    END_IF
    SWITCH(GW(11))
        CASE_EQUAL(0)
        IF_INT_GREATER(GW(12), 200)
            CALL(queen_get_aud_num, LW(0))
            IF_INT_GREATER_OR_EQUAL(LW(0), 1)
                SET_VALUE(GW(11), 3)
                SET_VALUE(GW(14), 1)
                CALL_CHILD(PTR(queen_hp_drain_evt))
                ELSE
                CALL_CHILD(PTR(queen_hp_drain_evt_miss))
            END_IF
            CALL_CHILD(PTR(queen_last_attack_message_evt))
            BREAK_SWITCH
        END_IF
        IF_INT_GREATER(GW(12), 100)
            CALL_CHILD(PTR(queen_next_attack_message_evt))
            BREAK_SWITCH
        END_IF
        IF_INT_GREATER(GW(12), 0)
            CALL_CHILD(PTR(queen_first_attack_message_evt))
            BREAK_SWITCH
        END_IF
        CASE_EQUAL(1)
        CALL(btlevtcmd_CheckPhase, LW(0), 67108865)
        IF_INT_EQUAL(LW(0), 1)
            CALL_CHILD(PTR(queen_hand_big_life_check_evt))
        END_IF
        CALL(btlevtcmd_CheckPhase, LW(0), 67108867)
        IF_INT_EQUAL(LW(0), 1)
            CALL(evt_sub_random, 100, LW(0))
            IF_INT_LESS_OR_EQUAL(LW(0), 50)
                CALL_CHILD(PTR(queen_change_hand_small_evt))
                BREAK_SWITCH
            END_IF
        END_IF
        CASE_EQUAL(2)
        CALL(btlevtcmd_CheckPhase, LW(0), 67108865)
        IF_INT_EQUAL(LW(0), 1)
            CALL_CHILD(PTR(queen_hand_small_life_check_evt))
        END_IF
        CALL(btlevtcmd_CheckPhase, LW(0), 67108867)
        IF_INT_EQUAL(LW(0), 1)
            CALL(evt_sub_random, 100, LW(0))
            IF_INT_LESS_OR_EQUAL(LW(0), 50)
                CALL_CHILD(PTR(queen_change_hand_big_evt))
                BREAK_SWITCH
            END_IF
        END_IF
        CASE_EQUAL(3)
        CALL(btlevtcmd_CheckPhase, LW(0), 67108868)
        IF_INT_EQUAL(LW(0), 1)
            SET_VALUE(GW(11), 2)
        END_IF
    END_SWITCH
    LABEL(99)
    RETURN
EVT_END()

EVT_BEGIN(queen_attack_black_thunder)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_peach_black_thunder_attack))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_peach_black_thunder_attack), LW(3), LW(4))
    CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(PTR(subsetevt_confuse_flustered))
        RETURN
    END_IF
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_peach_black_thunder_attack))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_A_1"))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 3, STRING("KRT_A_1"))
    WAIT_MS(500)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    ADD(LW(0), -33)
    ADD(LW(1), 82)
    CALL(evt_eff, STRING(""), STRING("toge_flush"), 4, LW(0), LW(1), LW(2), 60, 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_snd_sfxon_3d, STRING("SFX_BOSS_QWEN_EYE_SHINE1"), LW(0), LW(1), LW(2), 0)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 3, STRING("KRT_A_2"))
    WAIT_MS(700)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(queen_thunder, LW(0), LW(1), LW(2))
    WAIT_MS(80)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 3, STRING("KRT_S_1"))
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_peach_black_thunder_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    SWITCH(LW(5))
        CASE_OR_EQUAL(4)
        GOTO(90)
        CASE_END
        CASE_EQUAL(3)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
        GOTO(90)
        CASE_END
        CASE_EQUAL(6)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
        GOTO(90)
        CASE_EQUAL(2)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
        GOTO(90)
        CASE_EQUAL(1)
        GOTO(91)
        CASE_END
        CASE_DEFAULT
        GOTO(98)
        CASE_END
    END_SWITCH
    LABEL(90)
    GOTO(95)
    LABEL(91)
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_peach_black_thunder_attack))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_peach_black_thunder_attack))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_peach_black_thunder_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    LABEL(95)
    LABEL(98)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    LABEL(99)
    RETURN
EVT_END()

EVT_BEGIN(queen_attack_charge)
    CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(PTR(subsetevt_confuse_flustered))
        RETURN
    END_IF
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_peach_dark_charge_attack))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, -1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_A_2"))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_BOSS_BPCH_HAND_UP1"), LW(0), LW(1), LW(2), 0)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 3, STRING("KRT_A_1"))
    IF_INT_NOT_EQUAL(GW(8), -1)
        CALL(btlevtcmd_AnimeChangePose, GW(8), 1, STRING("KRT_A_3"))
    END_IF
    IF_INT_NOT_EQUAL(GW(9), -1)
        CALL(btlevtcmd_AnimeChangePose, GW(9), 1, STRING("KRT_A_3"))
    END_IF
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        CALL(evt_env_blur_on, 0, 1000)
    END_INLINE_EVENT
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHeight, UNIT_ID_SELF, LW(5))
    ADD(LW(1), LW(5))
    ADD(LW(2), 30)
    CALL(evt_eff, STRING(""), STRING("charge"), 2, LW(0), LW(1), LW(2), FLOAT(3.0f), 60, 0, 0, 0, 0, 0, 0)
    CALL(evt_snd_sfxon_3d, STRING("SFX_BOSS_QWEN_DARK_POWER1"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(1000)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 3, STRING("KRT_S_1"))
    IF_INT_NOT_EQUAL(GW(8), -1)
        CALL(btlevtcmd_AnimeChangePose, GW(8), 1, STRING("KRT_S_1"))
    END_IF
    IF_INT_NOT_EQUAL(GW(9), -1)
        CALL(btlevtcmd_AnimeChangePose, GW(9), 1, STRING("KRT_S_1"))
    END_IF
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, UNIT_ID_SELF, 1, PTR(&weapon_peach_dark_charge_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    LABEL(99)
    RETURN
EVT_END()

EVT_BEGIN(queen_attack_black_wave)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_peach_black_wave_attack))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_peach_black_wave_attack), LW(3), LW(4))
    CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(PTR(subsetevt_confuse_flustered))
        RETURN
    END_IF
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_peach_black_wave_attack))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, -1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    IF_INT_NOT_EQUAL(GW(10), -1)
        CALL_CHILD(PTR(queen_hand_small_dead_event))
        CALL(btlevtcmd_SpawnUnit, LW(0), PTR(&entry_shadow_hand_1), 0)
        SET_VALUE(GW(8), LW(0))
        CALL(btlevtcmd_SpawnUnit, LW(0), PTR(&entry_shadow_hand_2), 0)
        SET_VALUE(GW(9), LW(0))
        WAIT_MS(1500)
    END_IF
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_L_1"))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 3, STRING("KRT_T_3"))
    IF_INT_NOT_EQUAL(GW(8), -1)
        CALL(btlevtcmd_AnimeChangePose, GW(8), 1, STRING("KRT_A_3"))
    END_IF
    IF_INT_NOT_EQUAL(GW(9), -1)
        CALL(btlevtcmd_AnimeChangePose, GW(9), 1, STRING("KRT_A_3"))
    END_IF
    WAIT_MS(1000)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_S_1"))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 3, STRING("KRT_S_3"))
    WAIT_MS(200)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_A_1"))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 3, STRING("KRT_A_1"))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    ADD(LW(0), -33)
    ADD(LW(1), 82)
    CALL(evt_eff, STRING(""), STRING("toge_flush"), 4, LW(0), LW(1), LW(2), 60, 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_snd_sfxon_3d, STRING("SFX_BOSS_QWEN_EYE_SHINE1"), LW(0), LW(1), LW(2), 0)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(evt_eff, STRING("m_q2"), STRING("queen2"), 0, LW(0), LW(1), LW(2), 0, 0, 0, 0, 0, 0, 0, 0)
    BEGIN_INLINE_EVENT
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_BOSS_QWEN_DARK_WAVE1"), LW(0), LW(1), LW(2), 0)
        WAIT_MS(1500)
        LOOP(3)
            CALL(evt_snd_sfxon_3d, STRING("SFX_BOSS_QWEN_DARK_WAVE2"), LW(0), LW(1), LW(2), 0)
            WAIT_MS(1000)
        END_LOOP
        WAIT_MS(500)
        CALL(evt_snd_sfxon_3d, STRING("SFX_BOSS_QWEN_DARK_WAVE3"), LW(0), LW(1), LW(2), 0)
        WAIT_MS(500)
        CALL(evt_snd_sfxon_3d, STRING("SFX_BOSS_QWEN_DARK_WAVE3"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    WAIT_MS(1500)
    BEGIN_INLINE_EVENT
        CALL(evt_env_blur_on, 0, 1000)
    END_INLINE_EVENT
    CALL(evt_btl_camera_shake_h, 0, 1, 1, 120, 0)
    WAIT_MS(3500)
    CALL(evt_btl_camera_shake_h, 0, 1, 1, 120, 0)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 3, STRING("KRT_A_2"))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_BOSS_BPCH_HAND_UP1"), LW(0), LW(1), LW(2), 0)
    IF_INT_NOT_EQUAL(GW(8), -1)
        CALL(btlevtcmd_AnimeChangePose, GW(8), 1, STRING("KRT_A_4"))
    END_IF
    IF_INT_NOT_EQUAL(GW(9), -1)
        CALL(btlevtcmd_AnimeChangePose, GW(9), 1, STRING("KRT_A_4"))
    END_IF
    WAIT_MS(1500)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 3, STRING("KRT_S_1"))
    IF_INT_NOT_EQUAL(GW(8), -1)
        CALL(btlevtcmd_AnimeChangePose, GW(8), 1, STRING("KRT_S_1"))
    END_IF
    IF_INT_NOT_EQUAL(GW(9), -1)
        CALL(btlevtcmd_AnimeChangePose, GW(9), 1, STRING("KRT_S_1"))
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_peach_black_wave_attack))
    SET_VALUE(LF(0), 0)
    LABEL(0)
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_peach_black_wave_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    SWITCH(LW(5))
        CASE_OR_EQUAL(4)
        GOTO(90)
        CASE_END
        CASE_EQUAL(3)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
        GOTO(90)
        CASE_END
        CASE_EQUAL(6)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
        GOTO(90)
        CASE_EQUAL(2)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
        GOTO(90)
        CASE_EQUAL(1)
        GOTO(91)
        CASE_END
        CASE_DEFAULT
        GOTO(95)
        CASE_END
    END_SWITCH
    LABEL(90)
    GOTO(95)
    LABEL(91)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    IF_INT_EQUAL(LF(0), 0)
        CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_peach_black_wave_attack))
        SET_VALUE(LF(0), 1)
    END_IF
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_peach_black_wave_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    LABEL(95)
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(98)
    END_IF
    GOTO(0)
    LABEL(98)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    LABEL(99)
    IF_INT_NOT_EQUAL(GW(11), 0)
        SET_VALUE(GW(11), 1)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(queen_attack_black_energy)
    CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(PTR(subsetevt_confuse_flustered))
        RETURN
    END_IF
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, -1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 400)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_A_2"))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_BOSS_BPCH_HAND_UP1"), LW(0), LW(1), LW(2), 0)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 3, STRING("KRT_A_1"))
    IF_INT_NOT_EQUAL(GW(8), -1)
        CALL(btlevtcmd_AnimeChangePose, GW(8), 1, STRING("KRT_A_3"))
    END_IF
    IF_INT_NOT_EQUAL(GW(9), -1)
        CALL(btlevtcmd_AnimeChangePose, GW(9), 1, STRING("KRT_A_3"))
    END_IF
    WAIT_MS(750)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 3, STRING("KRT_S_1"))
    IF_INT_NOT_EQUAL(GW(8), -1)
        CALL(btlevtcmd_AnimeChangePose, GW(8), 1, STRING("KRT_S_1"))
    END_IF
    IF_INT_NOT_EQUAL(GW(9), -1)
        CALL(btlevtcmd_AnimeChangePose, GW(9), 1, STRING("KRT_S_1"))
    END_IF
    BEGIN_INLINE_EVENT
        CALL(evt_env_blur_on, 0, 1000)
    END_INLINE_EVENT
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHeight, UNIT_ID_SELF, LW(5))
    ADD(LW(1), LW(5))
    ADD(LW(1), -15)
    CALL(evt_eff, STRING(""), STRING("charge"), 2, LW(0), LW(1), LW(2), FLOAT(1.0f), 60, 0, 0, 0, 0, 0, 0)
    CALL(evt_snd_sfxon_3d, STRING("SFX_BOSS_BPCH_DARK_POWER1"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(1000)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_peach_black_energy_attack))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, UNIT_ID_SELF, 1, PTR(&weapon_peach_black_energy_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    RETURN
EVT_END()

EVT_BEGIN(queen_attack_bress1)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_peach_black_sigh_attack))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_peach_black_sigh_attack), LW(3), LW(4))
    CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(PTR(subsetevt_confuse_flustered))
        RETURN
    END_IF
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_peach_black_sigh_attack))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_A_1"))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 3, STRING("KRT_A_1"))
    WAIT_MS(500)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    ADD(LW(0), -33)
    ADD(LW(1), 82)
    CALL(evt_eff, STRING(""), STRING("toge_flush"), 4, LW(0), LW(1), LW(2), 60, 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_snd_sfxon_3d, STRING("SFX_BOSS_QWEN_EYE_SHINE1"), LW(0), LW(1), LW(2), 0)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 3, STRING("KRT_A2_1"))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_BOSS_BPCH_HAND_UP1"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(250)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 3, STRING("KRT_A2_2"))
    CALL(evt_eff, STRING(""), STRING("toiki"), 0, 73, 112, 0, 180, 0, 0, 0, 0, 0, 0, 0)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_BOSS_QWEN_BREATH1"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(2500)
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_BOSS_QWEN_BREATH2"), LW(0), LW(1), LW(2), 0)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 3, STRING("KRT_A2_3"))
    WAIT_MS(80)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 3, STRING("KRT_S_1"))
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_peach_black_sigh_attack))
    SET_VALUE(LF(0), 0)
    LABEL(0)
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_peach_black_sigh_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    SWITCH(LW(5))
        CASE_OR_EQUAL(4)
        GOTO(90)
        CASE_END
        CASE_EQUAL(3)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
        GOTO(90)
        CASE_END
        CASE_EQUAL(6)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
        GOTO(90)
        CASE_EQUAL(2)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
        GOTO(90)
        CASE_EQUAL(1)
        GOTO(91)
        CASE_END
        CASE_DEFAULT
        GOTO(95)
        CASE_END
    END_SWITCH
    LABEL(90)
    GOTO(95)
    LABEL(91)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    IF_INT_EQUAL(LF(0), 0)
        CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_peach_black_sigh_attack))
        SET_VALUE(LF(0), 1)
    END_IF
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_peach_black_sigh_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    LABEL(95)
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(98)
    END_IF
    GOTO(0)
    LABEL(98)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    LABEL(99)
    RETURN
EVT_END()

EVT_BEGIN(queen_attack_bress2)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_peach_black_temptation_attack))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_peach_black_temptation_attack), LW(3), LW(4))
    CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(PTR(subsetevt_confuse_flustered))
        RETURN
    END_IF
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_peach_black_temptation_attack))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_A_1"))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 3, STRING("KRT_A_1"))
    WAIT_MS(500)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    ADD(LW(0), -33)
    ADD(LW(1), 82)
    CALL(evt_eff, STRING(""), STRING("toge_flush"), 4, LW(0), LW(1), LW(2), 60, 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_snd_sfxon_3d, STRING("SFX_BOSS_QWEN_EYE_SHINE1"), LW(0), LW(1), LW(2), 0)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 3, STRING("KRT_A2_1"))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_BOSS_BPCH_HAND_UP1"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(250)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 3, STRING("KRT_A2_2"))
    CALL(evt_eff, STRING(""), STRING("toiki"), 1, 73, 112, 0, 180, 0, 0, 0, 0, 0, 0, 0)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_BOSS_QWEN_BREATH1"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(2500)
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_BOSS_QWEN_BREATH2"), LW(0), LW(1), LW(2), 0)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 3, STRING("KRT_A2_3"))
    WAIT_MS(80)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 3, STRING("KRT_S_1"))
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_peach_black_temptation_attack))
    SET_VALUE(LF(0), 0)
    LABEL(0)
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_peach_black_temptation_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    SWITCH(LW(5))
        CASE_OR_EQUAL(4)
        GOTO(90)
        CASE_END
        CASE_EQUAL(3)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
        GOTO(90)
        CASE_END
        CASE_EQUAL(6)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
        GOTO(90)
        CASE_EQUAL(2)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
        GOTO(90)
        CASE_EQUAL(1)
        GOTO(91)
        CASE_END
        CASE_DEFAULT
        GOTO(95)
        CASE_END
    END_SWITCH
    LABEL(90)
    GOTO(95)
    LABEL(91)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    IF_INT_EQUAL(LF(0), 0)
        CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_peach_black_temptation_attack))
        SET_VALUE(LF(0), 1)
    END_IF
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_peach_black_temptation_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    LABEL(95)
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(98)
    END_IF
    GOTO(0)
    LABEL(98)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    LABEL(99)
    RETURN
EVT_END()

EVT_BEGIN(queen_attack_bress3)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_peach_black_poison_attack))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_peach_black_poison_attack), LW(3), LW(4))
    CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(PTR(subsetevt_confuse_flustered))
        RETURN
    END_IF
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_peach_black_poison_attack))
    WAIT_MS(200)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_A_1"))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 3, STRING("KRT_A_1"))
    WAIT_MS(500)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    ADD(LW(0), -33)
    ADD(LW(1), 82)
    CALL(evt_eff, STRING(""), STRING("toge_flush"), 4, LW(0), LW(1), LW(2), 60, 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_snd_sfxon_3d, STRING("SFX_BOSS_QWEN_EYE_SHINE1"), LW(0), LW(1), LW(2), 0)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 3, STRING("KRT_A2_1"))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_BOSS_BPCH_HAND_UP1"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(250)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 3, STRING("KRT_A2_2"))
    CALL(evt_eff, STRING(""), STRING("toiki"), 2, 73, 112, 0, 180, 0, 0, 0, 0, 0, 0, 0)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_BOSS_QWEN_BREATH1"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(2500)
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_BOSS_QWEN_BREATH2"), LW(0), LW(1), LW(2), 0)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 3, STRING("KRT_A2_3"))
    WAIT_MS(80)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 3, STRING("KRT_S_1"))
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_peach_black_poison_attack))
    SET_VALUE(LF(0), 0)
    LABEL(0)
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_peach_black_poison_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    SWITCH(LW(5))
        CASE_OR_EQUAL(4)
        GOTO(90)
        CASE_END
        CASE_EQUAL(3)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
        GOTO(90)
        CASE_END
        CASE_EQUAL(6)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
        GOTO(90)
        CASE_EQUAL(2)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
        GOTO(90)
        CASE_EQUAL(1)
        GOTO(91)
        CASE_END
        CASE_DEFAULT
        GOTO(95)
        CASE_END
    END_SWITCH
    LABEL(90)
    GOTO(95)
    LABEL(91)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    IF_INT_EQUAL(LF(0), 0)
        CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_peach_black_poison_attack))
        SET_VALUE(LF(0), 1)
    END_IF
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_peach_black_poison_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    LABEL(95)
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(98)
    END_IF
    GOTO(0)
    LABEL(98)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    LABEL(99)
    RETURN
EVT_END()

EVT_BEGIN(queen_attack_event)
    IF_INT_EQUAL(GW(11), 0)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_ATTACK_STEP, LW(0))
        SWITCH(LW(0))
            CASE_EQUAL(0)
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_ATTACK_STEP, 3)
            CALL_CHILD(PTR(queen_attack_black_thunder))
            CASE_EQUAL(1)
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_ATTACK_STEP, 3)
            CALL_CHILD(PTR(queen_attack_black_energy))
            CASE_EQUAL(3)
            CALL(btlevtcmd_DrawLots, LW(0), 4, 100, 20, 30, 30)
            SWITCH(LW(0))
                CASE_EQUAL(0)
                CALL_CHILD(PTR(queen_attack_black_thunder))
                CASE_EQUAL(1)
                CALL_CHILD(PTR(queen_attack_bress1))
                CASE_EQUAL(2)
                CALL_CHILD(PTR(queen_attack_bress2))
                CASE_EQUAL(3)
                CALL_CHILD(PTR(queen_attack_bress3))
            END_SWITCH
        END_SWITCH
        GOTO(99)
    END_IF
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_ATTACK_STEP, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_ATTACK_STEP, 1)
        CALL_CHILD(PTR(queen_attack_black_thunder))
        CASE_EQUAL(1)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_ATTACK_STEP, 2)
        CALL_CHILD(PTR(queen_attack_charge))
        CASE_EQUAL(2)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_ATTACK_STEP, 3)
        CALL_CHILD(PTR(queen_attack_black_wave))
        CASE_EQUAL(3)
#ifdef VERSION_G8ME01
        CALL(btlevtcmd_DrawLots, LW(0), 6, 100, 50, 60, 20, 30, 30)
#else
        CALL(btlevtcmd_DrawLots, LW(0), 6, 100, 50, 30, 20, 30, 30)
#endif
        CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_DEFENSE_UP, LW(1))
        IF_INT_EQUAL(LW(1), 1)
            IF_INT_EQUAL(LW(0), 1)
                SET_VALUE(LW(0), 0)
            END_IF
        END_IF
        SWITCH(LW(0))
            CASE_EQUAL(0)
            CALL_CHILD(PTR(queen_attack_black_thunder))
            CASE_EQUAL(1)
            CALL_CHILD(PTR(queen_attack_black_energy))
            CASE_EQUAL(2)
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_ATTACK_STEP, 2)
            CALL_CHILD(PTR(queen_attack_charge))
            CASE_EQUAL(3)
            CALL_CHILD(PTR(queen_attack_bress1))
            CASE_EQUAL(4)
            CALL_CHILD(PTR(queen_attack_bress2))
            CASE_EQUAL(5)
            CALL_CHILD(PTR(queen_attack_bress3))
        END_SWITCH
    END_SWITCH
    LABEL(99)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

EVT_BEGIN(queen_damage_event)
    CALL(btlevtcmd_GetDamageCode, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 22)
        ADD(GW(12), 1)
        IF_INT_EQUAL(GW(18), 0)
            SET_VALUE(GW(18), 1)
        END_IF
        BEGIN_INLINE_EVENT
            LOOP(5)
                CALL(btlevtcmd_AddPartsPos, UNIT_ID_SELF, 2, 2, 0, 0)
                WAIT_MS(70)
                CALL(btlevtcmd_AddPartsPos, UNIT_ID_SELF, 2, -2, 0, 0)
                WAIT_MS(70)
            END_LOOP
        END_INLINE_EVENT
    END_IF
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_S_1"))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

EVT_BEGIN(queen_dead_event)
    CALL(btlevtcmd_WaitAttackEnd)
    CALL_CHILD(PTR(queen_dead_hp_msg_evt))
    CALL(evt_eff_softdelete, STRING("m_q"))
    SET_VALUE(GF(0), 0)
    BEGIN_INLINE_EVENT
        CALL(btlevtcmd_SetTalkPose, UNIT_ID_SELF, STRING("KRT_S_1"))
        CALL(btlevtcmd_SetStayPose, UNIT_ID_SELF, STRING("KRT_S_1"))
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_S_1"))
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 3, STRING("KRT_O_1"))
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 2, STRING("KRT_S_1"))
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 4, STRING("KRT_S_1"))
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 5, STRING("KRT_O_2"))
        CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 2, 254)
        CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 3, 254)
        CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 4, 254)
        CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 5, 254)
        CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 2, kHidden)
        CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 3, kHidden)
        CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 4, kHidden)
        CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 5, kHidden)
        SET_VALUE(LW(0), 254)
        LOOP(63)
            CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 2, LW(0))
            CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 3, LW(0))
            CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 4, LW(0))
            CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 5, LW(0))
            SUBTRACT(LW(0), 4)
            WAIT_MS(50)
        END_LOOP
        CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 2, 0)
        CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 3, 0)
        CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 4, 0)
        CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 5, 0)
        SET_VALUE(GF(0), 1)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_BOSS_BPCH_DIE1"), LW(0), LW(1), LW(2), LW(10))
        CALL(evt_eff, STRING(""), STRING("mist"), 6, 80, 50, 20, 0, 1500, 0, 0, 0, 0, 0, 0)
        WAIT_MS(200)
        CALL(evt_eff, STRING(""), STRING("mist"), 6, 100, 30, 10, 0, 1500, 0, 0, 0, 0, 0, 0)
        WAIT_MS(100)
        CALL(evt_eff, STRING(""), STRING("mist"), 6, 60, 40, 0, 0, 1500, 0, 0, 0, 0, 0, 0)
        WAIT_MS(50)
        CALL(evt_eff, STRING(""), STRING("mist"), 6, 70, 10, -20, 0, 1500, 0, 0, 0, 0, 0, 0)
        WAIT_MS(4500)
        SET_VALUE(LW(0), 127)
        LOOP(16)
            SUBTRACT(LW(0), 7)
            CALL(evt_snd_sfx_vol, LW(10), LW(0))
            WAIT_MS(125)
        END_LOOP
        CALL(evt_snd_sfxoff, LW(10))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        IF_INT_NOT_EQUAL(GW(8), -1)
            CALL_CHILD(PTR(queen_hand_1_dead_event))
        END_IF
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        IF_INT_NOT_EQUAL(GW(9), -1)
            CALL_CHILD(PTR(queen_hand_2_dead_event))
        END_IF
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        IF_INT_NOT_EQUAL(GW(10), -1)
            CALL_CHILD(PTR(queen_hand_small_dead_event))
        END_IF
    END_INLINE_EVENT
    LABEL(0)
    WAIT_MS(100)
    IF_INT_NOT_EQUAL(GW(8), -1)
        GOTO(0)
    END_IF
    IF_INT_NOT_EQUAL(GW(9), -1)
        GOTO(0)
    END_IF
    IF_INT_NOT_EQUAL(GW(10), -1)
        GOTO(0)
    END_IF
    IF_INT_NOT_EQUAL(GF(0), 1)
        GOTO(0)
    END_IF
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    SET_FLOAT_VALUE(LW(1), LW(1))
    SET_FLOAT_VALUE(LW(3), FLOAT(0.0f))
    LOOP(50)
        SUBTRACT_FLOAT(LW(1), FLOAT(0.81933594f))
        SUBTRACT_FLOAT(LW(3), FLOAT(0.81933594f))
        CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        SET_FLOAT_VALUE(LW(4), LW(1))
        SUBTRACT_FLOAT(LW(4), LW(3))
        CALL(btlevtcmd_SetPartsPos, UNIT_ID_SELF, 2, LW(0), LW(4), LW(2))
        WAIT_MS(10)
    END_LOOP
    CALL(btlevtcmd_KillUnit, UNIT_ID_SELF, 1)
    CALL(evt_btl_camera_set_prilimit, BTL_CAM_PRI_NORMAL)
    RETURN
EVT_END()

EVT_BEGIN(queen_wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 3)
    RETURN
EVT_END()

EVT_BEGIN(queen_entry_event)
    WAIT_MS(2000)
    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_OFF)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
    CALL(evt_btl_camera_set_moveto, 0, -113, 69, 332, -113, 46, 0, 15, 11)
    WAIT_MS(550)
    CALL(evt_msg_print_battle_party, STRING("stg8_las_161"))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_PARTNER)
    WAIT_MS(300)
    CALL(evt_btl_camera_set_moveto, 0, 63, 141, 412, 63, 111, 0, 1, 11)
    WAIT_MS(350)
    CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("stg8_las_162"), 0, -2)
    WAIT_MS(300)
    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_ON)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(slot_init)
    RETURN
EVT_END()

EVT_BEGIN(queen_hand_1_init_event)
    CALL(btlevtcmd_SetBaseRotate, UNIT_ID_SELF, 0, 315, 0)
    BEGIN_INLINE_EVENT
        CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 1, kHidden)
        SET_VALUE(LW(0), 0)
        LOOP(15)
            CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 1, LW(0))
            ADD(LW(0), 16)
            WAIT_MS(5)
        END_LOOP
        CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 1, 255)
    END_INLINE_EVENT
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(queen_hand_1_wait_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(queen_hand_1_attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(queen_hand_1_damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(evt_eff, STRING("m_h_1"), STRING("mist2"), 0, LW(0), 15, LW(2), 50, 0, -1, 0, 0, 0, 0, 0)
    IF_INT_EQUAL(GW(11), 0)
        CALL(btlevtcmd_SetPartsDefenceTable, UNIT_ID_SELF, 1, PTR(&queen_muteki_defence))
        CALL(btlevtcmd_SetPartsDefenceAttrTable, UNIT_ID_SELF, 1, PTR(&queen_muteki_defence_attr))
        CALL(btlevtcmd_SetRegistStatus, UNIT_ID_SELF, PTR(&queen_muteki_regist))
    END_IF
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 1, kNotTargetable)
    CALL(btlevtcmd_OffAttribute, UNIT_ID_SELF, kUnitNoHpGauge)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    SET_VALUE(GF(0), 1)
    RETURN
EVT_END()

EVT_BEGIN(queen_hand_1_press_attack)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_peach_black_press_attack))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_peach_black_press_attack), LW(3), LW(4))
    CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(PTR(subsetevt_confuse_flustered))
        RETURN
    END_IF
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_peach_black_press_attack))
    CALL(evt_eff_softdelete, LW(10))
    SET_VALUE(LW(0), 255)
    LOOP(15)
        CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 1, LW(0))
        ADD(LW(0), -16)
        WAIT_MS(5)
    END_LOOP
    CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 1, 0)
    WAIT_MS(500)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    ADD(LW(0), 20)
    SET_VALUE(LW(1), 0)
    ADD(LW(2), -30)
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, UNIT_ID_SELF)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    SET_VALUE(LW(0), 0)
    LOOP(15)
        CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 1, LW(0))
        ADD(LW(0), 16)
        WAIT_MS(5)
    END_LOOP
    CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 1, 255)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(evt_eff, LW(10), STRING("mist2"), 0, LW(0), 15, LW(2), 50, 0, -1, 0, 0, 0, 0, 0)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_A1_1"))
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_BOSS_QWEN_HAND_UP1"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(1100)
    CALL(evt_btl_camera_shake_h, 0, 1, 1, 15, 0)
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_peach_black_press_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    SWITCH(LW(5))
        CASE_OR_EQUAL(4)
        GOTO(90)
        CASE_END
        CASE_EQUAL(3)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
        GOTO(90)
        CASE_END
        CASE_EQUAL(6)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
        GOTO(90)
        CASE_EQUAL(2)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
        GOTO(90)
        CASE_EQUAL(1)
        GOTO(91)
        CASE_END
        CASE_DEFAULT
        GOTO(98)
        CASE_END
    END_SWITCH
    LABEL(90)
    GOTO(95)
    LABEL(91)
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_peach_black_press_attack))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_peach_black_press_attack))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_peach_black_press_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    LABEL(95)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    LABEL(98)
    LABEL(99)
    CALL(evt_eff_softdelete, LW(10))
    SET_VALUE(LW(0), 255)
    LOOP(15)
        CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 1, LW(0))
        ADD(LW(0), -16)
        WAIT_MS(5)
    END_LOOP
    CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 1, 0)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_S_1"))
    SET_VALUE(LW(0), 0)
    LOOP(15)
        CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 1, LW(0))
        ADD(LW(0), 16)
        WAIT_MS(5)
    END_LOOP
    CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 1, 255)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(evt_eff, LW(10), STRING("mist2"), 0, LW(0), 15, LW(2), 50, 0, -1, 0, 0, 0, 0, 0)
    RETURN
EVT_END()

EVT_BEGIN(queen_hand_1_drain_attack)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_peach_drain_attack))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_peach_drain_attack), LW(3), LW(4))
    CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(PTR(subsetevt_confuse_flustered))
        RETURN
    END_IF
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_peach_drain_attack))
    CALL(evt_eff_softdelete, LW(10))
    SET_VALUE(LW(0), 255)
    LOOP(15)
        CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 1, LW(0))
        ADD(LW(0), -16)
        WAIT_MS(5)
    END_LOOP
    CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 1, 0)
    WAIT_MS(500)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    SET_VALUE(LW(1), 0)
    ADD(LW(2), -30)
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, UNIT_ID_SELF)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    SET_VALUE(LW(0), 0)
    LOOP(15)
        CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 1, LW(0))
        ADD(LW(0), 16)
        WAIT_MS(5)
    END_LOOP
    CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 1, 255)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(evt_eff, LW(10), STRING("mist2"), 0, LW(0), 15, LW(2), 50, 0, -1, 0, 0, 0, 0, 0)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_A2_1"))
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_BOSS_QWEN_DRAIN1"), LW(0), LW(1), LW(2), 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(300)
        CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
        ADD(LW(1), 40)
        CALL(queen_drain, LW(0), LW(1), LW(2))
        WAIT_MS(600)
        CALL(queen_drain, LW(0), LW(1), LW(2))
        WAIT_MS(600)
        CALL(queen_drain, LW(0), LW(1), LW(2))
        WAIT_MS(600)
    END_INLINE_EVENT
    WAIT_MS(2000)
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_peach_drain_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    SWITCH(LW(5))
        CASE_OR_EQUAL(4)
        GOTO(90)
        CASE_END
        CASE_EQUAL(3)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
        GOTO(90)
        CASE_END
        CASE_EQUAL(6)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
        GOTO(90)
        CASE_EQUAL(2)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
        GOTO(90)
        CASE_EQUAL(1)
        GOTO(91)
        CASE_END
        CASE_DEFAULT
        GOTO(98)
        CASE_END
    END_SWITCH
    LABEL(90)
    GOTO(95)
    LABEL(91)
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_peach_drain_attack))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_peach_drain_attack))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_peach_drain_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    CALL(btlevtcmd_GetHpDamage, LW(3), LW(0))
    CALL(btlevtcmd_RecoverHp, GW(13), 1, LW(0))
    CALL(btlevtcmd_GetPos, GW(13), LW(5), LW(6), LW(7))
    CALL(btlevtcmd_GetHeight, GW(13), LW(8))
    ADD(LW(6), LW(8))
    ADD(LW(7), 30)
    CALL(evt_eff, STRING(""), STRING("recovery"), 0, LW(5), LW(6), LW(7), LW(0), 0, 0, 0, 0, 0, 0, 0)
    LABEL(95)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    LABEL(98)
    LABEL(99)
    CALL(evt_eff_softdelete, LW(10))
    SET_VALUE(LW(0), 255)
    LOOP(15)
        CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 1, LW(0))
        ADD(LW(0), -16)
        WAIT_MS(5)
    END_LOOP
    CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 1, 0)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_S_1"))
    SET_VALUE(LW(0), 0)
    LOOP(15)
        CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 1, LW(0))
        ADD(LW(0), 16)
        WAIT_MS(5)
    END_LOOP
    CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 1, 255)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(evt_eff, LW(10), STRING("mist2"), 0, LW(0), 15, LW(2), 50, 0, -1, 0, 0, 0, 0, 0)
    RETURN
EVT_END()

EVT_BEGIN(queen_hand_1_attack_event)
    CALL(btlevtcmd_GetUnitWork, GW(13), UNIT_WORK_ATTACK_STEP, LW(0))
    IF_INT_EQUAL(LW(0), 2)
        GOTO(99)
    END_IF
#ifdef VERSION_G8ME01
    CALL(btlevtcmd_DrawLots, LW(0), 2, 50, 50)
#else
    CALL(btlevtcmd_DrawLots, LW(0), 2, 100, 20)
#endif
    SWITCH(LW(0))
        CASE_EQUAL(0)
        SET_VALUE(LW(10), STRING("m_h_1"))
        CALL_CHILD(PTR(queen_hand_1_press_attack))
        CASE_EQUAL(1)
        SET_VALUE(LW(10), STRING("m_h_1"))
        CALL_CHILD(PTR(queen_hand_1_drain_attack))
    END_SWITCH
    LABEL(99)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

EVT_BEGIN(queen_hand_1_damage_event)
    IF_INT_EQUAL(GW(18), 0)
        SET_VALUE(GW(18), 1)
    END_IF
    ADD(GW(12), 1)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    RETURN
EVT_END()

EVT_BEGIN(queen_hand_1_dead_event)
    SET_VALUE(LW(0), 255)
    LOOP(15)
        CALL(btlevtcmd_SetAlpha, GW(8), 1, LW(0))
        ADD(LW(0), -16)
        WAIT_MS(5)
    END_LOOP
    CALL(btlevtcmd_SetAlpha, GW(8), 1, 0)
    CALL(evt_eff_softdelete, STRING("m_h_1"))
    WAIT_FRAMES(40)
    CALL(btlevtcmd_ClearAllStatus, GW(8))
    CALL(btlevtcmd_KillUnit, GW(8), 0)
    SET_VALUE(GW(8), -1)
    SET_VALUE(GF(1), 1)
    RETURN
EVT_END()

EVT_BEGIN(queen_hand_1_wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

EVT_BEGIN(queen_hand_2_init_event)
    CALL(btlevtcmd_SetBaseScale, UNIT_ID_SELF, FLOAT(-1.0996094f), FLOAT(1.0996094f), FLOAT(1.0996094f))
    CALL(btlevtcmd_SetBaseRotate, UNIT_ID_SELF, 0, 315, 0)
    BEGIN_INLINE_EVENT
        CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 1, kHidden)
        SET_VALUE(LW(0), 0)
        LOOP(15)
            CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 1, LW(0))
            ADD(LW(0), 16)
            WAIT_MS(5)
        END_LOOP
        CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 1, 255)
    END_INLINE_EVENT
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(queen_hand_2_wait_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(queen_hand_2_attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(queen_hand_2_damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(evt_eff, STRING("m_h_2"), STRING("mist2"), 0, LW(0), 15, LW(2), 50, 0, -1, 0, 0, 0, 0, 0)
    IF_INT_EQUAL(GW(11), 0)
        CALL(btlevtcmd_SetPartsDefenceTable, UNIT_ID_SELF, 1, PTR(&queen_muteki_defence))
        CALL(btlevtcmd_SetPartsDefenceAttrTable, UNIT_ID_SELF, 1, PTR(&queen_muteki_defence_attr))
        CALL(btlevtcmd_SetRegistStatus, UNIT_ID_SELF, PTR(&queen_muteki_regist))
    END_IF
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 1, kNotTargetable)
    CALL(btlevtcmd_OffAttribute, UNIT_ID_SELF, kUnitNoHpGauge)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    SET_VALUE(GF(0), 1)
    RETURN
EVT_END()

EVT_BEGIN(queen_hand_2_attack_event)
    CALL(btlevtcmd_GetUnitWork, GW(13), UNIT_WORK_ATTACK_STEP, LW(0))
    IF_INT_EQUAL(LW(0), 2)
        GOTO(99)
    END_IF
#ifdef VERSION_G8ME01
    CALL(btlevtcmd_DrawLots, LW(0), 2, 50, 50)
#else
    CALL(btlevtcmd_DrawLots, LW(0), 2, 100, 20)
#endif
    SWITCH(LW(0))
        CASE_EQUAL(0)
        SET_VALUE(LW(10), STRING("m_h_2"))
        CALL_CHILD(PTR(queen_hand_1_press_attack))
        CASE_EQUAL(1)
        SET_VALUE(LW(10), STRING("m_h_2"))
        CALL_CHILD(PTR(queen_hand_1_drain_attack))
    END_SWITCH
    LABEL(99)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

EVT_BEGIN(queen_hand_2_damage_event)
    IF_INT_EQUAL(GW(18), 0)
        SET_VALUE(GW(18), 1)
    END_IF
    ADD(GW(12), 1)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    RETURN
EVT_END()

EVT_BEGIN(queen_hand_2_dead_event)
    SET_VALUE(LW(0), 255)
    LOOP(15)
        CALL(btlevtcmd_SetAlpha, GW(9), 1, LW(0))
        ADD(LW(0), -16)
        WAIT_MS(5)
    END_LOOP
    CALL(btlevtcmd_SetAlpha, GW(9), 1, 0)
    CALL(evt_eff_softdelete, STRING("m_h_2"))
    WAIT_FRAMES(40)
    CALL(btlevtcmd_ClearAllStatus, GW(9))
    CALL(btlevtcmd_KillUnit, GW(9), 0)
    SET_VALUE(GW(9), -1)
    SET_VALUE(GF(1), 1)
    RETURN
EVT_END()

EVT_BEGIN(queen_hand_2_wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

EVT_BEGIN(queen_hand_small_init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(queen_hand_small_wait_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(queen_hand_small_attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(queen_hand_small_damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(btldefaultevt_Confuse))
    CALL(evt_eff, STRING("m_h_s1"), STRING("mist2"), 0, 100, 15, 0, 150, 0, -1, 0, 0, 0, 0, 0)
    CALL(evt_eff, STRING("m_h_s2"), STRING("mist2"), 0, 100, 15, 0, 150, 0, -1, 0, 0, 0, 0, 0)
    CALL(evt_eff, STRING("m_h_s3"), STRING("mist2"), 0, 100, 15, 0, 150, 0, -1, 0, 0, 0, 0, 0)
    CALL(evt_snd_sfxon, STRING("SFX_BOSS_QWEN_HAND_APPEAR1_2"), 0)
    CALL(queen_hand_small_entry_evt)
    IF_INT_EQUAL(GW(11), 0)
        CALL(btlevtcmd_SetPartsDefenceTable, UNIT_ID_SELF, 1, PTR(&queen_muteki_defence))
        CALL(btlevtcmd_SetPartsDefenceAttrTable, UNIT_ID_SELF, 1, PTR(&queen_muteki_defence_attr))
        CALL(btlevtcmd_SetRegistStatus, UNIT_ID_SELF, PTR(&queen_muteki_regist))
    END_IF
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    SET_VALUE(GF(0), 1)
    RETURN
EVT_END()

EVT_BEGIN(queen_hand_small_drag_attack)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_peach_dark_drag_attack))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_peach_dark_drag_attack), LW(3), LW(4))
    CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(PTR(subsetevt_confuse_flustered))
        RETURN
    END_IF
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_peach_dark_drag_attack))
    CALL(evt_eff_softdelete, STRING("m_h_s1"))
    CALL(evt_eff_softdelete, STRING("m_h_s2"))
    CALL(evt_eff_softdelete, STRING("m_h_s3"))
    CALL(evt_snd_sfxon, STRING("SFX_BOSS_QWEN_HAND_GOOUT1"), 0)
    CALL(queen_hand_small_drag_2_evt)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, LW(3), LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_BOSS_QWENH_HAND_APPEAR1"), LW(0), LW(1), LW(2), 0)
    BEGIN_INLINE_EVENT
        CALL(queen_hand_small_drag_1_evt)
    END_INLINE_EVENT
    CALL(evt_eff, STRING("m_h_s1_"), STRING("mist2"), 0, -100, 15, 0, 100, 0, -1, 0, 0, 0, 0, 0)
    CALL(evt_eff, STRING("m_h_s2_"), STRING("mist2"), 0, -100, 15, 0, 100, 0, -1, 0, 0, 0, 0, 0)
    CALL(evt_eff, STRING("m_h_s3_"), STRING("mist2"), 0, -100, 15, 0, 100, 0, -1, 0, 0, 0, 0, 0)
    WAIT_MS(1250)
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_peach_dark_drag_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    SWITCH(LW(5))
        CASE_OR_EQUAL(4)
        GOTO(90)
        CASE_END
        CASE_EQUAL(3)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
        GOTO(90)
        CASE_END
        CASE_EQUAL(6)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
        GOTO(90)
        CASE_EQUAL(2)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
        GOTO(90)
        CASE_EQUAL(1)
        GOTO(91)
        CASE_END
        CASE_DEFAULT
        GOTO(98)
        CASE_END
    END_SWITCH
    LABEL(90)
    WAIT_MS(2250)
    CALL(evt_eff_softdelete, STRING("m_h_s1_"))
    CALL(evt_eff_softdelete, STRING("m_h_s2_"))
    CALL(evt_eff_softdelete, STRING("m_h_s3_"))
    GOTO(95)
    LABEL(91)
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_peach_dark_drag_attack))
    CALL(battle_evt_majo_disp_off, LW(3), LW(4), 0, 0)
    CALL(btlevtcmd_SetScale, LW(3), 0, 0, 0)
    CALL(btlevtcmd_GetHomePos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_peach_dark_drag_attack))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_peach_dark_drag_attack), 0, LW(5))
    WAIT_MS(750)
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_peach_dark_drag_attack))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_peach_dark_drag_attack), 0, LW(5))
    WAIT_MS(750)
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_peach_dark_drag_attack))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_peach_dark_drag_attack), 0, LW(5))
    WAIT_MS(750)
    CALL(evt_eff_softdelete, STRING("m_h_s1_"))
    CALL(evt_eff_softdelete, STRING("m_h_s2_"))
    CALL(evt_eff_softdelete, STRING("m_h_s3_"))
    CALL(btlevtcmd_GetHomePos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetScale, LW(3), 1, 1, 1)
    CALL(battle_evt_majo_disp_on, LW(3), LW(4), LW(0), LW(1), LW(2), 0)
    CALL(btlevtcmd_SetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_peach_dark_drag_attack))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_peach_dark_drag_last_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    WAIT_MS(750)
    LABEL(95)
    LABEL(98)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_eff, STRING("m_h_s1"), STRING("mist2"), 0, 100, 15, 0, 150, 0, -1, 0, 0, 0, 0, 0)
    CALL(evt_eff, STRING("m_h_s2"), STRING("mist2"), 0, 100, 15, 0, 150, 0, -1, 0, 0, 0, 0, 0)
    CALL(evt_eff, STRING("m_h_s3"), STRING("mist2"), 0, 100, 15, 0, 150, 0, -1, 0, 0, 0, 0, 0)
    CALL(evt_snd_sfxon, STRING("SFX_BOSS_QWEN_HAND_APPEAR1_2"), 0)
    CALL(queen_hand_small_drag_3_evt)
    RETURN
EVT_END()

EVT_BEGIN(queen_hand_small_1000hand_attack)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_peach_black_1000hand_attack))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_peach_black_1000hand_attack), LW(3), LW(4))
    CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
#ifdef VERSION_G8ME01
        CALL_CHILD(subsetevt_confuse_flustered)
#else
        CALL_CHILD(PTR(subsetevt_confuse_flustered))
#endif
        RETURN
    END_IF
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_peach_black_1000hand_attack))
    CALL(evt_btl_camera_shake_h, 0, 1, 1, 180, 0)
    CALL(evt_eff_softdelete, STRING("m_h_s1"))
    CALL(evt_eff_softdelete, STRING("m_h_s2"))
    CALL(evt_eff_softdelete, STRING("m_h_s3"))
    CALL(evt_snd_sfxon, STRING("SFX_BOSS_QWEN_HANDS_MOVE1"), 0)
#ifdef VERSION_G8ME01
    BEGIN_INLINE_EVENT_GET_ID(LW(15))
#else
    BEGIN_INLINE_EVENT
#endif
        CALL(queen_hand_small_1000hand_1_evt)
    END_INLINE_EVENT
    WAIT_MS(400)
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_peach_dark_drag_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    SWITCH(LW(5))
        CASE_OR_EQUAL(4)
        GOTO(90)
        CASE_END
        CASE_EQUAL(3)
        GOTO(90)
        CASE_END
        CASE_EQUAL(6)
        GOTO(90)
        CASE_EQUAL(2)
        GOTO(90)
        CASE_EQUAL(1)
        GOTO(91)
        CASE_END
        CASE_DEFAULT
        GOTO(98)
        CASE_END
    END_SWITCH
    LABEL(90)
    LOOP(4)
        SWITCH(LW(5))
            CASE_OR_EQUAL(4)
            CASE_END
            CASE_EQUAL(3)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
            CASE_END
            CASE_EQUAL(6)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
            CASE_EQUAL(2)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
        END_SWITCH
        WAIT_MS(500)
    END_LOOP
    WAIT_MS(1000)
    GOTO(95)
    LABEL(91)
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_peach_black_1000hand_attack))
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_peach_black_1000hand_attack))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_peach_black_1000hand_attack), 0, LW(5))
    WAIT_MS(500)
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_peach_black_1000hand_attack))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_peach_black_1000hand_attack), 0, LW(5))
    WAIT_MS(500)
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_peach_black_1000hand_attack))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_peach_black_1000hand_attack), 0, LW(5))
    WAIT_MS(500)
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_peach_black_1000hand_attack))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_peach_black_1000hand_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    WAIT_MS(500)
    WAIT_MS(1000)
    GOTO(98)
    LABEL(95)
    LABEL(98)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_eff, STRING("m_h_s1"), STRING("mist2"), 0, 100, 15, 0, 150, 0, -1, 0, 0, 0, 0, 0)
    CALL(evt_eff, STRING("m_h_s2"), STRING("mist2"), 0, 100, 15, 0, 150, 0, -1, 0, 0, 0, 0, 0)
    CALL(evt_eff, STRING("m_h_s3"), STRING("mist2"), 0, 100, 15, 0, 150, 0, -1, 0, 0, 0, 0, 0)
#ifdef VERSION_G8ME01
    IF_INT_NOT_EQUAL(LW(15), 0)
        CALL(btlevtcmd_WaitEventEnd, LW(15))
    END_IF
#endif
    CALL(evt_snd_sfxon, STRING("SFX_BOSS_QWEN_HAND_APPEAR1_2"), 0)
    CALL(queen_hand_small_1000hand_2_evt)
    RETURN
EVT_END()

EVT_BEGIN(queen_hand_small_attack_event)
    CALL(btlevtcmd_GetUnitWork, GW(13), UNIT_WORK_ATTACK_STEP, LW(0))
    IF_INT_EQUAL(LW(0), 2)
        GOTO(99)
    END_IF
#ifdef VERSION_G8ME01
    CALL(btlevtcmd_DrawLots, LW(0), 2, 50, 50)
#else
    CALL(btlevtcmd_DrawLots, LW(0), 2, 60, 40)
#endif
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CALL_CHILD(PTR(queen_hand_small_1000hand_attack))
        CASE_EQUAL(1)
        CALL_CHILD(PTR(queen_hand_small_drag_attack))
    END_SWITCH
    LABEL(99)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

EVT_BEGIN(queen_hand_small_damage_event)
    IF_INT_EQUAL(GW(18), 0)
        SET_VALUE(GW(18), 1)
    END_IF
    ADD(GW(12), 1)
    CALL(btlevtcmd_GetDamageCode, UNIT_ID_SELF, LW(0))
    BEGIN_INLINE_EVENT
        LOOP(5)
            CALL(queen_hand_small_1000hand_set_offset_evt, 2, 0, 0)
            WAIT_MS(70)
            CALL(queen_hand_small_1000hand_set_offset_evt, -2, 0, 0)
            WAIT_MS(70)
        END_LOOP
    END_INLINE_EVENT
END_IF
SET_VALUE(LW(10), -2)
SET_VALUE(LW(11), 1)
CALL_CHILD(PTR(btldefaultevt_Damage))
CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
RETURN
EVT_END()

EVT_BEGIN(queen_hand_small_dead_event)
    CALL(evt_eff_softdelete, STRING("m_h_s1"))
    CALL(evt_eff_softdelete, STRING("m_h_s2"))
    CALL(evt_eff_softdelete, STRING("m_h_s3"))
    CALL(evt_snd_sfxon, STRING("SFX_BOSS_QWEN_HAND_GOOUT1"), 0)
    CALL(queen_hand_small_dead_evt)
    WAIT_FRAMES(40)
    CALL(btlevtcmd_ClearAllStatus, GW(10))
    CALL(btlevtcmd_KillUnit, GW(10), 0)
    SET_VALUE(GW(10), -1)
    SET_VALUE(GF(1), 1)
    RETURN
EVT_END()

EVT_BEGIN(queen_hand_small_wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

USER_FUNC(queen_hand_small_1000hand_set_offset_evt) {
    EvtWord* args = event->args;
    s32 x = evtGetValue(event, args[0]);
    s32 y = evtGetValue(event, args[1]);
    s32 z = evtGetValue(event, args[2]);
    s32 i;
    HandElem* elem;

    extGetPosePtr();
    elem = hand_wp->elems;
    for (i = 0; i < hand_wp->num; i++, elem++) {
        elem->pos.x += x;
        elem->pos.y += y;
        elem->pos.z += z;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(queen_hand_small_1000hand_2_evt) {
    s32 i;
    ExtdrvPose* pose;
    HandElem* elem;

    if (isFirstCall) {
        hand_wp->state = 0;
    }
    switch (hand_wp->state) {
        case 0:
            hand_wp->y = -80.0f;
            pose = extGetPosePtr();
            elem = hand_wp->elems;
            for (i = 0; i < hand_wp->num; i++, pose++, elem++) {
                pose->flags |= EXT_POSE_FLAG_VISIBLE;
                pose->flags &= ~EXT_POSE_FLAG_BILLBOARD;
                PSMTXIdentity(*pose->mtx);
                elem->ofs = irand(20);
                elem->speed = 1.0f + irand(200) / 100.0f;
                elem->pos.x = (i % 10) * 20 + irand(15);
                elem->pos.y = hand_wp->y - elem->ofs;
                elem->pos.z = (i / 10) * -15 + 70 + irand(15);
                pose->dir = irand(20) - 10;
                (*pose->mtx)[0][0] = 50.0f / (50.0f + irand(50));
                (*pose->mtx)[1][1] = 50.0f / (50.0f + irand(50));
                if (elem->flip) {
                    (*pose->mtx)[0][0] *= -1.0f;
                }
            }
            hand_wp->state++;
        case 1:
            if (hand_wp->y >= 0.0f) {
                hand_wp->state++;
                break;
            }
            hand_wp->y += 1.0f;
            extGetPosePtr();
            elem = hand_wp->elems;
            for (i = 0; i < hand_wp->num; i++, elem++) {
                elem->pos.y = hand_wp->y * elem->speed - elem->ofs;
            }
            break;
        case 2:
        case 99:
            return 1;
    }
    return 0;
}

USER_FUNC(queen_hand_small_1000hand_1_evt) {
    s32 i;
    HandElem* elem;
    f32 c;

    if (isFirstCall) {
        hand_wp->state = 0;
    }
    switch (hand_wp->state) {
        case 0:
            hand_wp->timer = 0;
            hand_wp->y = 0.0f;
            hand_wp->timer = 0;
            extGetPosePtr();
            elem = hand_wp->elems;
            for (i = 0; i < hand_wp->num; i++, elem++) {
                elem->phase = 0;
            }
            hand_wp->state++;
        case 1:
            if (hand_wp->timer > 200) {
                hand_wp->state++;
                break;
            }
            hand_wp->timer++;
            extGetPosePtr();
            elem = hand_wp->elems;
            for (i = 0; i < hand_wp->num; i++, elem++) {
                if (i % 10 <= hand_wp->timer / 3) {
                    elem->pos.x -= 3.0f;
                    c = cos(elem->phase / 6.0f);
                    elem->pos.y -= 0.3f * c;
                    elem->phase++;
                    if (elem->phase >= 2160) {
                        elem->phase -= 2160;
                    }
                }
            }
            break;
        case 2:
        case 99:
            return 1;
    }
    return 0;
}

USER_FUNC(queen_hand_small_drag_1_evt) {
    s32 i;
    ExtdrvPose* pose;
    HandElem* elem;

    if (isFirstCall) {
        hand_wp->state = 0;
    }
    switch (hand_wp->state) {
        case 0:
            hand_wp->y = -80.0f;
            pose = extGetPosePtr();
            elem = hand_wp->elems;
            for (i = 0; i < hand_wp->num; i++, pose++, elem++) {
                pose->flags |= EXT_POSE_FLAG_VISIBLE;
                pose->flags &= ~EXT_POSE_FLAG_BILLBOARD;
                PSMTXIdentity(*pose->mtx);
                elem->ofs = irand(20);
                elem->speed = 1.0f + irand(200) / 100.0f;
                elem->pos.x = (i % 10) * 20 - 200 + irand(15);
                elem->pos.y = hand_wp->y - elem->ofs;
                elem->pos.z = (i / 10) * -15 + 70 + irand(15);
                pose->dir = irand(20) - 10;
                (*pose->mtx)[0][0] = 50.0f / (50.0f + irand(50));
                (*pose->mtx)[1][1] = 50.0f / (50.0f + irand(50));
                if (elem->flip) {
                    (*pose->mtx)[0][0] *= -1.0f;
                }
            }
            hand_wp->state++;
        case 1:
            if (hand_wp->y >= 0.0f) {
                hand_wp->state++;
                break;
            }
            hand_wp->y += 1.0f;
            extGetPosePtr();
            elem = hand_wp->elems;
            for (i = 0; i < hand_wp->num; i++, elem++) {
                elem->pos.y = hand_wp->y * elem->speed - elem->ofs;
            }
            break;
        case 2:
            hand_wp->timer = 30;
            hand_wp->state++;
            break;
        case 3:
            if (hand_wp->timer != 0) {
                hand_wp->timer--;
                if (hand_wp->timer == 0) {
                    Vec pos;
                    pos.x = -100.0f;
                    pos.y = 0.0f;
                    pos.z = 0.0f;
                    psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_BOSS_QWEN_HAND_IN1), &pos);
                }
                break;
            }
            if (hand_wp->y <= -80.0f) {
                hand_wp->state++;
                break;
            }
            hand_wp->y -= 1.0f;
            extGetPosePtr();
            elem = hand_wp->elems;
            for (i = 0; i < hand_wp->num; i++, elem++) {
                elem->pos.y = hand_wp->y * elem->speed - elem->ofs;
            }
            break;
        case 4:
        case 99:
            return 1;
    }
    return 0;
}

USER_FUNC(queen_hand_small_drag_2_evt) {
    s32 i;
    ExtdrvPose* pose;
    HandElem* elem;

    if (isFirstCall) {
        hand_wp->state = 0;
    }
    switch (hand_wp->state) {
        case 0:
            hand_wp->y = 0.0f;
            hand_wp->state++;
        case 1:
            if (hand_wp->y <= -80.0f) {
                hand_wp->state++;
                break;
            }
            hand_wp->y -= 1.0f;
            extGetPosePtr();
            elem = hand_wp->elems;
            for (i = 0; i < hand_wp->num; i++, elem++) {
                elem->pos.y = hand_wp->y * elem->speed - elem->ofs;
            }
            break;
        case 2:
        case 99:
            pose = extGetPosePtr();
            for (i = 0; i < hand_wp->num; i++, pose++) {
                pose->flags &= ~EXT_POSE_FLAG_VISIBLE;
            }
            return 1;
    }
    return 0;
}

USER_FUNC(queen_hand_small_drag_3_evt) {
    s32 i;
    ExtdrvPose* pose;
    HandElem* elem;

    if (isFirstCall) {
        hand_wp->state = 0;
    }
    switch (hand_wp->state) {
        case 0:
            hand_wp->y = -80.0f;
            pose = extGetPosePtr();
            elem = hand_wp->elems;
            for (i = 0; i < hand_wp->num; i++, pose++, elem++) {
                pose->flags |= EXT_POSE_FLAG_VISIBLE;
                pose->flags &= ~EXT_POSE_FLAG_BILLBOARD;
                PSMTXIdentity(*pose->mtx);
                elem->ofs = irand(20);
                elem->speed = 1.0f + irand(200) / 100.0f;
                elem->pos.x = (i % 10) * 20 + irand(15);
                elem->pos.y = hand_wp->y - elem->ofs;
                elem->pos.z = (i / 10) * -15 + 70 + irand(15);
                pose->dir = irand(20) - 10;
                (*pose->mtx)[0][0] = 50.0f / (50.0f + irand(50));
                (*pose->mtx)[1][1] = 50.0f / (50.0f + irand(50));
                if (elem->flip) {
                    (*pose->mtx)[0][0] *= -1.0f;
                }
            }
            hand_wp->state++;
        case 1:
            if (hand_wp->y >= 0.0f) {
                hand_wp->state++;
                break;
            }
            hand_wp->y += 1.0f;
            extGetPosePtr();
            elem = hand_wp->elems;
            for (i = 0; i < hand_wp->num; i++, elem++) {
                elem->pos.y = hand_wp->y * elem->speed - elem->ofs;
            }
            break;
        case 2:
        case 99:
            return 1;
    }
    return 0;
}

USER_FUNC(queen_hand_small_drag_4_evt) {
    s32 i;
    ExtdrvPose* pose;
    HandElem* elem;

    if (isFirstCall) {
        hand_wp->state = 0;
    }
    switch (hand_wp->state) {
        case 0:
            hand_wp->y = 0.0f;
            hand_wp->state++;
        case 1:
            if (hand_wp->y <= -80.0f) {
                hand_wp->state++;
                break;
            }
            hand_wp->y -= 1.0f;
            extGetPosePtr();
            elem = hand_wp->elems;
            for (i = 0; i < hand_wp->num; i++, elem++) {
                elem->pos.y = hand_wp->y * elem->speed - elem->ofs;
            }
            break;
        case 2:
        case 99:
            pose = extGetPosePtr();
            for (i = 0; i < hand_wp->num; i++, pose++) {
                pose->flags &= ~EXT_POSE_FLAG_VISIBLE;
            }
            return 1;
    }
    return 0;
}

USER_FUNC(queen_hand_small_dead_evt) {
    s32 i;
    ExtdrvPose* pose;
    HandElem* elem;

    if (isFirstCall) {
        hand_wp->state = 0;
    }
    switch (hand_wp->state) {
        case 0:
            hand_wp->y = 0.0f;
            hand_wp->state++;
        case 1:
            if (hand_wp->y <= -80.0f) {
                hand_wp->state++;
                break;
            }
            hand_wp->y -= 1.0f;
            extGetPosePtr();
            elem = hand_wp->elems;
            for (i = 0; i < hand_wp->num; i++, elem++) {
                elem->pos.y = hand_wp->y * elem->speed - elem->ofs;
            }
            break;
        case 2:
        case 99:
            pose = extGetPosePtr();
            for (i = 0; i < hand_wp->num; i++, pose++) {
                pose->flags &= ~EXT_POSE_FLAG_VISIBLE;
            }
            return 1;
    }
    return 0;
}

USER_FUNC(queen_hand_small_entry_evt) {
    s32 i;
    ExtdrvPose* pose;
    HandElem* elem;

    if (isFirstCall) {
        hand_wp->state = 0;
    }
    switch (hand_wp->state) {
        case 0:
            hand_wp->y = -80.0f;
            pose = extGetPosePtr();
            elem = hand_wp->elems;
            for (i = 0; i < hand_wp->num; i++, pose++, elem++) {
                pose->flags |= EXT_POSE_FLAG_VISIBLE;
                pose->flags &= ~EXT_POSE_FLAG_BILLBOARD;
                PSMTXIdentity(*pose->mtx);
                elem->ofs = irand(20);
                elem->speed = 1.0f + irand(200) / 100.0f;
                elem->pos.x = (i % 10) * 20 + irand(15);
                elem->pos.y = hand_wp->y - elem->ofs;
                elem->pos.z = (i / 10) * -15 + 70 + irand(15);
                pose->dir = irand(20) - 10;
                (*pose->mtx)[0][0] = 50.0f / (50.0f + irand(50));
                (*pose->mtx)[1][1] = 50.0f / (50.0f + irand(50));
                if (elem->flip) {
                    (*pose->mtx)[0][0] *= -1.0f;
                }
            }
            hand_wp->state++;
        case 1:
            if (hand_wp->y >= 0.0f) {
                hand_wp->state++;
                break;
            }
            hand_wp->y += 1.0f;
            extGetPosePtr();
            elem = hand_wp->elems;
            for (i = 0; i < hand_wp->num; i++, elem++) {
                elem->pos.y = hand_wp->y * elem->speed - elem->ofs;
            }
            break;
        case 2:
        case 99:
            return 1;
    }
    return 0;
}

USER_FUNC(slot_init) {
    BattleBreakSlotGetPtr()->flags |= 4;
    return EVT_RETURN_DONE;
}

USER_FUNC(peach_shock_evt) {
    EvtWord* args = event->args;
    s32 x = evtGetValue(event, args[0]);
    s32 y = evtGetValue(event, args[1]);
    s32 z = evtGetValue(event, args[2]);
    EffectEntry* eff;

    eff = effShockN64Entry(3, x, y, z, 200.0f, 150.0f);
    ((EffShockN64Work*)eff->userdata)->col0[0] = 10;
    ((EffShockN64Work*)eff->userdata)->col0[1] = 10;
    ((EffShockN64Work*)eff->userdata)->col0[2] = 10;
    ((EffShockN64Work*)eff->userdata)->col1[0] = irand(50) + 155;
    ((EffShockN64Work*)eff->userdata)->col1[1] = irand(50) + 155;
    ((EffShockN64Work*)eff->userdata)->col1[2] = irand(50) + 155;
    return EVT_RETURN_DONE;
}

USER_FUNC(queen_hp_drain_5_evt) {
    s32 i;
    ExtdrvPose* pose;
    HandElem* elem;

    if (isFirstCall) {
        hand_wp->state = 0;
    }
    switch (hand_wp->state) {
        case 0:
            hand_wp->y = -80.0f;
            pose = extGetPosePtr();
            elem = hand_wp->elems;
            for (i = 0; i < hand_wp->num; i++, pose++, elem++) {
                pose->flags |= EXT_POSE_FLAG_VISIBLE;
                pose->flags &= ~EXT_POSE_FLAG_BILLBOARD;
                PSMTXIdentity(*pose->mtx);
                elem->ofs = irand(20);
                elem->speed = 1.0f + irand(200) / 100.0f;
                elem->pos.x = (i % 10) * 20 + irand(15);
                elem->pos.y = hand_wp->y - elem->ofs;
                elem->pos.z = (i / 10) * -15 + 70 + irand(15);
                pose->dir = irand(20) - 10;
                (*pose->mtx)[0][0] = 50.0f / (50.0f + irand(50));
                (*pose->mtx)[1][1] = 50.0f / (50.0f + irand(50));
                if (elem->flip) {
                    (*pose->mtx)[0][0] *= -1.0f;
                }
            }
            hand_wp->state++;
        case 1:
            if (hand_wp->y >= 0.0f) {
                hand_wp->state++;
                break;
            }
            hand_wp->y += 1.0f;
            extGetPosePtr();
            elem = hand_wp->elems;
            for (i = 0; i < hand_wp->num; i++, elem++) {
                elem->pos.y = hand_wp->y * elem->speed - elem->ofs;
            }
            break;
        case 2:
        case 99:
            return 1;
    }
    return 0;
}

USER_FUNC(queen_hp_drain_4_evt) {
    s32 i;
    f32 x, y, z;

    if (isFirstCall) {
        event->userdata[0] = 0;
    }
    switch (event->userdata[0]) {
        case 0:
            event->userdata[1] = 0;
            event->userdata[0]++;
            for (i = 0; i < 200; i++) {
                if ((u8)BattleAudience_GetSysCtrl(i)) {
                    BattleAudience_SetAnim(i, 9, TRUE);
                }
            }
        case 1:
            for (i = 0; i < 200; i++) {
                if ((u8)BattleAudience_GetSysCtrl(i)) {
                    BattleAudience_GetPosition(i, &x, &y, &z);
                    BattleAudience_SetPosition(i, x, y - 1.0f, z);
                }
            }
            event->userdata[1]--;
            if (event->userdata[1] < -80) {
                event->userdata[0]++;
            }
            break;
        case 2:
            for (i = 0; i < 200; i++) {
                if ((u8)BattleAudience_GetSysCtrl(i)) {
                    BattleAudience_Delete(i);
                    BattleAudienceAddTargetNum(0.0f, -1.0f);
                }
            }
            event->userdata[0] = 99;
        case 99:
            return 1;
    }
    return 0;
}

USER_FUNC(queen_hp_drain_3_evt) {
    s32 i;
    ExtdrvPose* pose;
    HandElem* elem;

    if (isFirstCall) {
        hand_wp->state = 0;
    }
    switch (hand_wp->state) {
        case 0:
            hand_wp->y = -30.0f;
            hand_wp->state++;
        case 1:
            if (hand_wp->y <= -110.0f) {
                hand_wp->state++;
                break;
            }
            hand_wp->y -= 1.0f;
            extGetPosePtr();
            elem = hand_wp->elems;
            for (i = 0; i < hand_wp->num; i++, elem++) {
                elem->pos.y = hand_wp->y * elem->speed - elem->ofs;
            }
            break;
        case 2:
        case 99:
            pose = extGetPosePtr();
            for (i = 0; i < hand_wp->num; i++, pose++) {
                pose->flags &= ~EXT_POSE_FLAG_VISIBLE;
            }
            return 1;
    }
    return 0;
}

USER_FUNC(queen_hp_drain_2_evt) {
    s32 i;
    ExtdrvPose* pose;
    HandElem* elem;

    if (isFirstCall) {
        hand_wp->state = 0;
    }
    switch (hand_wp->state) {
        case 0:
            event->userdata[0]++;
            hand_wp->y = -110.0f;
            pose = extGetPosePtr();
            elem = hand_wp->elems;
            for (i = 0; i < hand_wp->num; i++, pose++, elem++) {
                pose->flags |= EXT_POSE_FLAG_VISIBLE;
                pose->flags &= ~EXT_POSE_FLAG_BILLBOARD;
                PSMTXIdentity(*pose->mtx);
                elem->ofs = irand(20);
                elem->speed = 1.0f + irand(200) / 100.0f;
                elem->pos.x = (i % 10) * 40 - 200 + irand(15);
                elem->pos.y = hand_wp->y - elem->ofs;
                elem->pos.z = (i / 10) * -15 + 270 + irand(15);
                pose->dir = irand(20) - 10;
                (*pose->mtx)[0][0] = 50.0f / (50.0f + irand(50));
                (*pose->mtx)[1][1] = 50.0f / (50.0f + irand(50));
                if (elem->flip) {
                    (*pose->mtx)[0][0] *= -1.0f;
                }
            }
            hand_wp->state++;
        case 1:
            if (hand_wp->y >= -30.0f) {
                hand_wp->state++;
                break;
            }
            hand_wp->y += 1.0f;
            extGetPosePtr();
            elem = hand_wp->elems;
            for (i = 0; i < hand_wp->num; i++, elem++) {
                elem->pos.y = hand_wp->y * elem->speed - elem->ofs;
            }
            break;
        case 2:
        case 99:
            return 1;
    }
    return 0;
}

USER_FUNC(queen_hp_drain_1_evt) {
    s32 i;
    ExtdrvPose* pose;
    HandElem* elem;

    if (isFirstCall) {
        hand_wp->state = 0;
    }
    switch (hand_wp->state) {
        case 0:
            hand_wp->y = 0.0f;
            hand_wp->state++;
        case 1:
            if (hand_wp->y <= -80.0f) {
                hand_wp->state++;
                break;
            }
            hand_wp->y -= 1.0f;
            extGetPosePtr();
            elem = hand_wp->elems;
            for (i = 0; i < hand_wp->num; i++, elem++) {
                elem->pos.y = hand_wp->y * elem->speed - elem->ofs;
            }
            break;
        case 2:
        case 99:
            pose = extGetPosePtr();
            for (i = 0; i < hand_wp->num; i++, pose++) {
                pose->flags &= ~EXT_POSE_FLAG_VISIBLE;
            }
            return 1;
    }
    return 0;
}

USER_FUNC(queen_get_aud_num) {
    EvtWord* args = event->args;

    BattleAudience_GetAudienceNum();
    evtSetValue(event, args[0], BattleAudience_GetAudienceNum());
    return EVT_RETURN_DONE;
}

USER_FUNC(queen_kankyaku_delete_evt) {
    s32 i;

    for (i = 0; i < 200; i++) {
        if ((u8)BattleAudience_GetSysCtrl(i)) {
            BattleAudience_Delete(i);
            BattleAudienceAddTargetNum(0.0f, -1.0f);
        }
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(peach_hand1000_dark_drag_1_evt) {
    s32 i;
    ExtdrvPose* pose;
    HandElem* elem;

    if (isFirstCall) {
        hand_wp->state = 0;
    }
    switch (hand_wp->state) {
        case 0:
            hand_wp->y = -80.0f;
            pose = extGetPosePtr();
            elem = hand_wp->elems;
            for (i = 0; i < hand_wp->num; i++, pose++, elem++) {
                pose->flags |= EXT_POSE_FLAG_VISIBLE;
                pose->flags &= ~EXT_POSE_FLAG_BILLBOARD;
                PSMTXIdentity(*pose->mtx);
                elem->ofs = irand(20);
                elem->speed = 1.0f + irand(200) / 100.0f;
                elem->pos.x = (i % 10) * 20 - 200 + irand(15);
                elem->pos.y = hand_wp->y - elem->ofs;
                elem->pos.z = (i / 10) * -15 + 70 + irand(15);
                pose->dir = irand(20) - 10;
                (*pose->mtx)[0][0] = 50.0f / (50.0f + irand(50));
                (*pose->mtx)[1][1] = 50.0f / (50.0f + irand(50));
                if (elem->flip) {
                    (*pose->mtx)[0][0] *= -1.0f;
                }
            }
            hand_wp->state++;
        case 1:
            if (hand_wp->y >= 0.0f) {
                hand_wp->state++;
                break;
            }
            hand_wp->y += 1.0f;
            extGetPosePtr();
            elem = hand_wp->elems;
            for (i = 0; i < hand_wp->num; i++, elem++) {
                elem->pos.y = hand_wp->y * elem->speed - elem->ofs;
            }
            break;
        case 2:
            hand_wp->timer = 30;
            hand_wp->state++;
            break;
        case 3:
            if (hand_wp->timer != 0) {
                hand_wp->timer--;
                break;
            }
            if (hand_wp->y <= -80.0f) {
                hand_wp->state++;
                break;
            }
            hand_wp->y -= 1.0f;
            extGetPosePtr();
            elem = hand_wp->elems;
            for (i = 0; i < hand_wp->num; i++, elem++) {
                elem->pos.y = hand_wp->y * elem->speed - elem->ofs;
            }
            break;
        case 4:
        case 99:
            return 1;
    }
    return 0;
}

USER_FUNC(eff_queen_set_alpha_evt) {
    s32 alpha = evtGetValue(event, *event->args);

    ((s32*)effNameToPtr("m_q2")->userdata)[10] = alpha;
    return EVT_RETURN_DONE;
}

USER_FUNC(queen_break_slot_flag2_on) {
    BattleBreakSlotGetPtr()->flags |= 2;
    return EVT_RETURN_DONE;
}

USER_FUNC(queen_drain) {
    EvtWord* args = event->args;
    f32 x = evtGetFloat(event, args[0]);
    f32 y = evtGetFloat(event, args[1]);
    f32 z = evtGetFloat(event, args[2]);
    Vec pos;

    pos.x = x;
    pos.y = y;
    pos.z = z;
    psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_BOSS_BPCH_DRAIN2), &pos);
    effStardust2N64Entry(5, 10, 30, x, y, z, 20.0f, 40.0f);
    return EVT_RETURN_DONE;
}

USER_FUNC(queen_thunder) {
    EvtWord* args = event->args;
    f32 x = evtGetFloat(event, args[0]);
    f32 y = evtGetFloat(event, args[1]);
    f32 z = evtGetFloat(event, args[2]);
    EffectEntry* eff;
    Vec pos;

    pos.x = x;
    pos.y = y;
    pos.z = z;
    psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_BOSS_BPCH_THUNDER1), &pos);
    eff = effPokopiThunderN64Entry(0, 40, 20.0f + x + irand(20), 150.0f, 20.0f + z, x, 0.0f, z, 1.0f);
    ((EffPokopiThunderN64Work*)eff->userdata)->r = 255;
    ((EffPokopiThunderN64Work*)eff->userdata)->g = 160;
    ((EffPokopiThunderN64Work*)eff->userdata)->b = 192;
    ((EffPokopiThunderN64Work*)eff->userdata)->r2 = 20;
    ((EffPokopiThunderN64Work*)eff->userdata)->g2 = 20;
    ((EffPokopiThunderN64Work*)eff->userdata)->b2 = 20;
    eff = effShockN64Entry(3, x, 0.0f, z, 80.0f, 80.0f);
    ((EffShockN64Work*)eff->userdata)->col0[0] = 20;
    ((EffShockN64Work*)eff->userdata)->col0[1] = 20;
    ((EffShockN64Work*)eff->userdata)->col0[2] = 20;
    ((EffShockN64Work*)eff->userdata)->col1[0] = 255;
    ((EffShockN64Work*)eff->userdata)->col1[1] = 160;
    ((EffShockN64Work*)eff->userdata)->col1[2] = 192;
    btl_camera_shake_w(0, 1.0f, 1.0f, 8, 0);
    return EVT_RETURN_DONE;
}

USER_FUNC(change_pose) {
    s32 anim = evtGetValue(event, *event->args);
    s32 i;
    HandElem* elem;

    elem = hand_wp->elems;
    for (i = 0; i < hand_wp->num; i++, elem++) {
        elem->animIdx = anim;
        elem->time = gp->renderTime;
    }
    return EVT_RETURN_DONE;
}

// same body as pose_main; hand_main inlines it (register/stack order only match this way)
inline void hand_pose_inl(void) {
    ExtdrvPose* pose;
    HandElem* elem;
    s32 i;
    HandAnim* a;
    f32 t;
    s32 id;
    f32 x, y, z;
    f32 hx, hy, hz;

    pose = extGetPosePtr();
    elem = hand_wp->elems;
    for (i = 0; i < hand_wp->num; i++, pose++, elem++) {
        a = &hand_wp->anim[elem->animIdx];
        t = (f32)gp->framerate * OSTicksToMilliseconds((u64)(gp->renderTime - elem->time)) / 1000.0f / a->rate;
        if (t >= 1.0f) {
            elem->time = gp->renderTime;
            t -= 1.0f;
        }
        pose->texId = a->start + (s32)((a->count - 1) * t);
        id = evtGetValue(NULL, GW(10));
        if (id == -1) {
            (*pose->mtx)[0][3] = elem->pos.x;
            (*pose->mtx)[1][3] = elem->pos.y;
            (*pose->mtx)[2][3] = elem->pos.z;
        } else {
            BattleGetUnitPtr(_battleWorkPointer, id);
            BtlUnit_GetPos(BattleGetUnitPtr(_battleWorkPointer, id), &x, &y, &z);
            BtlUnit_GetHomePos(BattleGetUnitPtr(_battleWorkPointer, id), &hx, &hy, &hz);
            (*pose->mtx)[0][3] = elem->pos.x + x - hx;
            (*pose->mtx)[1][3] = elem->pos.y + y - hy;
            (*pose->mtx)[2][3] = elem->pos.z + z - hz;
        }
    }
}
void hand_main(void) {
    hand_pose_inl();
    dispEntry(CAMERA_3D, 0.0f, 1, hand_draw, NULL);
}

void hand_draw(CameraId cameraId, void* param) {
    extLoadRenderMode();
    extLoadVertex();
    extLoadTexture();
    extLoadTev();
    extDraw();
    extLoadTextureExit();
}

static void hand_init(void) {
    s32 i;
    ExtdrvPose* pose;
    HandElem* elem;

    hand_wp = BTL_ALLOC(sizeof(HandWork));
    memset(hand_wp, 0, sizeof(HandWork));
    hand_wp->num = extGetPoseNum();
    hand_wp->elems = BTL_ALLOC(hand_wp->num * sizeof(HandElem));
    memset(hand_wp->elems, 0, hand_wp->num * sizeof(HandElem));
    pose_init();
    pose = extGetPosePtr();
    elem = hand_wp->elems;
    for (i = 0; i < hand_wp->num; i++, pose++, elem++) {
        pose->flags |= (EXT_POSE_FLAG_BILLBOARD | EXT_POSE_FLAG_NO_SHADOW);
        PSMTXIdentity(*pose->mtx);
        elem->animIdx = 0;
        elem->time = gp->renderTime - (u64)irand(100) * 10000000;
        elem->flip = irand(1);
    }
}

static void pose_main(void) {
    ExtdrvPose* pose;
    HandElem* elem;
    s32 i;
    HandAnim* a;
    f32 t;
    s32 id;
    f32 x, y, z;
    f32 hx, hy, hz;

    pose = extGetPosePtr();
    elem = hand_wp->elems;
    for (i = 0; i < hand_wp->num; i++, pose++, elem++) {
        a = &hand_wp->anim[elem->animIdx];
        t = (f32)gp->framerate * OSTicksToMilliseconds((u64)(gp->renderTime - elem->time)) / 1000.0f / a->rate;
        if (t >= 1.0f) {
            elem->time = gp->renderTime;
            t -= 1.0f;
        }
        pose->texId = a->start + (s32)((a->count - 1) * t);
        id = evtGetValue(NULL, GW(10));
        if (id == -1) {
            (*pose->mtx)[0][3] = elem->pos.x;
            (*pose->mtx)[1][3] = elem->pos.y;
            (*pose->mtx)[2][3] = elem->pos.z;
        } else {
            BattleGetUnitPtr(_battleWorkPointer, id);
            BtlUnit_GetPos(BattleGetUnitPtr(_battleWorkPointer, id), &x, &y, &z);
            BtlUnit_GetHomePos(BattleGetUnitPtr(_battleWorkPointer, id), &hx, &hy, &hz);
            (*pose->mtx)[0][3] = elem->pos.x + x - hx;
            (*pose->mtx)[1][3] = elem->pos.y + y - hy;
            (*pose->mtx)[2][3] = elem->pos.z + z - hz;
        }
    }
}

static void pose_init(void) {
    char name[32];
    char anim[32];
    ExtTexInfo* d = hand_data;
    s32 i = 0;
    s32 start;

    // hand_light_init is an inline helper defined by hand before EVT_BEGIN(shadow_queen_1000) in the
    // rels/ source: its (Vec){-200, 100, 500} template must sit there in .rodata
    hand_light_init();
    strcpy(name, "");
    strcpy(anim, "");
    hand_wp->animNum = -1;
    do {
        if (strcmp(d->group, name) != 0) {
            strcpy(name, d->group);
            strcpy(anim, "");
        }
        if (strcmp(d->anim, anim) != 0) {
            strcpy(anim, d->anim);
            start = i;
            hand_wp->animNum++;
        }
        strcpy(hand_wp->anim[hand_wp->animNum].name, name);
        strcpy(hand_wp->anim[hand_wp->animNum].anim, anim);
        hand_wp->anim[hand_wp->animNum].start = start;
        hand_wp->anim[hand_wp->animNum].count = i - start + 1;
        hand_wp->anim[hand_wp->animNum].rate = d->frame;
        i++;
        d++;
    } while (d->group != NULL);
    hand_wp->animNum++;
}

USER_FUNC(make_pattern) {
    ExtTexInfo* d = hand_data;
    NpcEntry* npc = npcNameToPtr("dummy");
    AnimData* data;
    s32 i;

    animPoseSetAnim(npc->poseId, "KRT_S_1", TRUE);
    animPoseGetAnimPosePtr(npc->poseId);
    data = animPoseGetAnimDataPtr(npc->poseId);
    for (i = 0; i < (s32)(data->loopInfo)[2]; i += 3, d++) {
        d->group = "c_s_hand";
        d->anim = "KRT_S_1";
        d->frame = 3.0f * i;
    }
    d->group = NULL;
    return EVT_RETURN_DONE;
}

USER_FUNC(peach_talk_synchro_shadow_head_evt) {
    BattleUnitPart* part;

    part = BattleGetUnitPartsPtr(BattleTransID(event, -2), 1);
    if (strcmp(part->poseName, "KRT_T_1") == 0 || strcmp(part->poseName, "KRT_T_2") == 0 ||
        strcmp(part->poseName, "KRT_T_3") == 0) {
        part = BattleGetUnitPartsPtr(BattleTransID(event, -2), 3);
        if (strcmp(part->poseName, "KRT_T_1") != 0) {
            BtlUnit_SetAnim(part, "KRT_T_1");
            return 0;
        }
    }
    part = BattleGetUnitPartsPtr(BattleTransID(event, -2), 1);
    if (strcmp(part->poseName, "KRT_T_1") != 0 && strcmp(part->poseName, "KRT_T_2") != 0 &&
        strcmp(part->poseName, "KRT_T_3") != 0) {
        part = BattleGetUnitPartsPtr(BattleTransID(event, -2), 3);
        if (strcmp(part->poseName, "KRT_T_1") == 0) {
            BtlUnit_SetAnim(part, "KRT_S_1");
            return 0;
        }
    }
    if (strcmp(BattleGetUnitPartsPtr(BattleTransID(event, -2), 1)->poseName, "KRT_D_1") == 0) {
        part = BattleGetUnitPartsPtr(BattleTransID(event, -2), 3);
        if (strcmp(part->poseName, "KRT_D_1") != 0) {
            BtlUnit_SetAnim(part, "KRT_D_1");
            return 0;
        }
    }
    if (strcmp(BattleGetUnitPartsPtr(BattleTransID(event, -2), 1)->poseName, "KRT_D_1") != 0) {
        part = BattleGetUnitPartsPtr(BattleTransID(event, -2), 3);
        if (strcmp(part->poseName, "KRT_D_1") == 0) {
            BtlUnit_SetAnim(part, "KRT_S_1");
            return 0;
        }
    }
    return 0;
}

USER_FUNC(shadow_queen_connect) {
    QueenConnect* wk;
    Vec* wpos;
    BattleUnitPart* part;

    if (isFirstCall) {
        event->userdatap[0] = wk = BTL_ALLOC(sizeof(QueenConnect));
        wk->poseId[0] = BattleGetUnitPartsPtr(BattleTransID(event, -2), 2)->poseId;
        animPoseWorldPositionEvalOn(wk->poseId[0]);
        wk->idx[0] = animPoseGetGroupIdx(wk->poseId[0], "locator12");
        wk->poseId[1] = BattleGetUnitPartsPtr(BattleTransID(event, -2), 3)->poseId;
        animPoseWorldPositionEvalOn(wk->poseId[1]);
        wk->idx[1] = animPoseGetGroupIdx(wk->poseId[1], "locator12");
        animPosePeraOff(wk->poseId[0]);
        animPosePeraOff(wk->poseId[1]);
        wk->poseId[2] = BattleGetUnitPartsPtr(BattleTransID(event, -2), 4)->poseId;
        animPosePeraOff(wk->poseId[2]);
    }
    wk = event->userdatap[0];
    wpos = *animPoseGetAnimPosePtr(wk->poseId[0])->worldPositions;
    part = BattleGetUnitPartsPtr(BattleTransID(event, -2), 3);
    BtlUnit_SetPartsPos(part, wpos[wk->idx[0]].x, wpos[wk->idx[0]].y, wpos[wk->idx[0]].z);
    part = BattleGetUnitPartsPtr(BattleTransID(event, -2), 5);
    part->dispZOffset = 7.0f;
    BtlUnit_SetPartsPos(part, wpos[wk->idx[0]].x, wpos[wk->idx[0]].y, wpos[wk->idx[0]].z);
    wpos = *animPoseGetAnimPosePtr(wk->poseId[1])->worldPositions;
    part = BattleGetUnitPartsPtr(BattleTransID(event, -2), 4);
    part->dispZOffset = 1.0f;
    BtlUnit_SetPartsPos(part, wpos[wk->idx[1]].x, wpos[wk->idx[1]].y, wpos[wk->idx[1]].z);
    return 0;
}

