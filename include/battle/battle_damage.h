#pragma once

#include <dolphin/types.h>

typedef struct BattleVulnerableStatus {
    u8 sleepRate; //0x0
    u8 stopRate; //0x1
    u8 dizzyRate; //0x2
    u8 poisonRate; //0x3
    u8 confuseRate; //0x4
    u8 electricRate; //0x5
    u8 burnRate; //0x6
    u8 freezeRate; //0x7
    u8 hugeRate; //0x8
    u8 tinyRate; //0x9
    u8 attackUpRate; //0xA
    u8 attackDownRate; //0xB
    u8 defenseUpRate; //0xC
    u8 defenseDownRate; //0xD
    u8 allergicRate; //0xE
    u8 frightRate; //0xF
    u8 galeForceRate; //0x10
    u8 fastRate; //0x11
    u8 slowRate; //0x12
    u8 dodgyRate; //0x13
    u8 invisibleRate; //0x14
    u8 instaKillRate; //0x15
} BattleVulnerableStatus;

// Low byte of a hit result (the upper bits are flags; 0x16, 0x1C and 0x1D are status-only outcomes not named yet)
typedef enum BattleDamageCode {
    DAMAGE_CODE_STATUS_ONLY = 0x16, // status effect without normal impact
    DAMAGE_CODE_NORMAL = 0x17, // hit by a normal-element weapon
    DAMAGE_CODE_FIRE = 0x18,
    DAMAGE_CODE_EXPLOSION = 0x19,
    DAMAGE_CODE_ICE = 0x1A,
    DAMAGE_CODE_ELECTRIC = 0x1B,
    DAMAGE_CODE_FRIGHT = 0x1C, // Fright outcome
    DAMAGE_CODE_GALE_FORCE = 0x1D, // Gale Force (blown away) outcome
    DAMAGE_CODE_NO_DAMAGE = 0x1E, // the hit did no damage (default result)
    DAMAGE_CODE_AVOID_TOP_SPIKY = 0x1F, // contact hazards that stopped the attack (BattleCheckCounter)
    DAMAGE_CODE_AVOID_PREEMPTIVE_SPIKY = 0x20,
    DAMAGE_CODE_AVOID_FRONT_SPIKY = 0x21,
    DAMAGE_CODE_AVOID_FIERY = 0x22,
    DAMAGE_CODE_AVOID_ICY = 0x23,
    DAMAGE_CODE_AVOID_ELECTRIC = 0x24,
    DAMAGE_CODE_AVOID_POISON = 0x25, // default route exists; no producer in BattleCheckCounter
    DAMAGE_CODE_LUCKY = 0x26, // evaded by Lucky (badges)
    DAMAGE_CODE_DODGY = 0x27, // evaded by Dodgy
    DAMAGE_CODE_MISS = 0x28,
    DAMAGE_CODE_PROTECTED = 0x29, // target whose attack a bodyguard took
    DAMAGE_CODE_SUPERGUARDED = 0x2A,
    DAMAGE_CODE_COUNTER_TOP_SPIKY = 0x2B, // BattleCheckCounter kinds; the message id is 0x100 + the code
    DAMAGE_CODE_COUNTER_PREEMPTIVE_SPIKY = 0x2C,
    DAMAGE_CODE_COUNTER_FRONT_SPIKY = 0x2D,
    DAMAGE_CODE_COUNTER_FIERY = 0x2E,
    DAMAGE_CODE_COUNTER_ICY = 0x2F,
    DAMAGE_CODE_COUNTER_ELECTRIC = 0x30, // electric part
    DAMAGE_CODE_COUNTER_EXPLOSIVE = 0x31,
    DAMAGE_CODE_COUNTER_VOLATILE_EXPLOSIVE = 0x32,
    DAMAGE_CODE_COUNTER_POISON = 0x33,
    DAMAGE_CODE_COUNTER_ELECTRIFIED = 0x34, // target under STATUS_ELECTRIC
    DAMAGE_CODE_COUNTER_PAYBACK = 0x35, // Payback status or Return Postage
    DAMAGE_CODE_COUNTER_HOLD_FAST = 0x36,
    DAMAGE_CODE_SUPERGUARD_COUNTER = 0x37, // superguard damage back to the attacker
    DAMAGE_CODE_BODYGUARD_MOVE = 0x38, // the guarding unit moves in
    DAMAGE_CODE_FIRST_ATTACK_POS = 0x39, // first-strike positioning
} BattleDamageCode;

// Upper bits of BattleUnit.damageCode next to the low-byte BattleDamageCode (BattleCheckDamage's result). The
// damage check functions take the same bits as their flags argument and start the result from them.
typedef enum BattleDamageResultFlags {
    DAMAGE_RESULT_FINAL_HIT = 0x100, // last hit of the attack (status, gauge and death are resolved)
    DAMAGE_RESULT_ZERO_HP = 0x200, // HP is zero after the final hit
    DAMAGE_RESULT_DEATH = 0x400, // the unit dies (undead only from fire/explosive hits)
    DAMAGE_RESULT_REVIVAL = 0x800, // a Life Shroom can revive the unit
    DAMAGE_RESULT_GUARD_RETURN = 0x1000, // the target is a bodyguard: it goes back to its place afterwards
                                         // (btldefaultevt_guard_return)
    DAMAGE_RESULT_ABSORBED = 0x2000, // DEFENSE_ATTR_ABSORB: the hit heals; no damage display, Pity Flower or FP damage
    DAMAGE_RESULT_BACKFIRE = 0x4000, // the default damage event runs _backfire
    DAMAGE_RESULT_FREEZE_CRUSH = 0x8000, // SPECIAL_FREEZE_CRUSH broke the target's freeze
    DAMAGE_RESULT_AC_SUCCESS = 0x20000, // the attack's action command succeeded
    DAMAGE_RESULT_GUARD = 0x40000,
    DAMAGE_RESULT_SUPERGUARD = 0x80000,
    DAMAGE_RESULT_NO_EVADE = 0x100000, // BattlePreCheckDamage reports a hit without the evasion checks
    DAMAGE_RESULT_FLIPPED = 0x200000, // a shelled or bomb-flippable part was flipped (dmg_turn_event)
    DAMAGE_RESULT_GROUNDED = 0x400000, // a winged part was grounded on the final hit (dmg_fall_event)
    DAMAGE_RESULT_ATTACK_FX_R = 0x1000000, // an Attack FX badge sound played: R (Hooktail's damage event reacts to it)
    DAMAGE_RESULT_ATTACK_FX_Y = 0x2000000,
    DAMAGE_RESULT_ATTACK_FX_B = 0x4000000,
    DAMAGE_RESULT_ATTACK_FX_G = 0x8000000,
    DAMAGE_RESULT_ATTACK_FX_P = 0x10000000,
    DAMAGE_RESULT_NO_STATUS = 0x20000000, // BattleSetStatusDamageFromWeapon inflicts nothing
    DAMAGE_RESULT_STATUS_EVERY_HIT = 0x40000000, // status is tried on this hit, not only on the final one
    DAMAGE_RESULT_GULP = (s32)0x80000000, // Yoshi's Gulp: passes DEFENSE_ATTR_IMMUNE_EXCEPT_GULP
} BattleDamageResultFlags;

// Counter result filled by BattleCheckCounter (BattleInitCounterPreCheckWork clears it and sets attackHits)
typedef struct CounterWork {
    s32 attackHits;     // 0x0, cleared when a contact hazard (spiky, fiery, icy, electric part) stops the attack
    s32 payback;        // 0x4, Payback status or poison counter: half the damage goes back
    s32 burnStatus;     // 0x8, the counter also burns the attacker
    s32 poisonStatus;   // 0xC, ... poisons the attacker
    s32 electricStatus; // 0x10, ... electrifies the attacker
    s32 freezeStatus;   // 0x14, ... freezes the attacker
    s32 holdFast;       // 0x18, Hold Fast status: half the damage goes back, with an explosion
    s32 returnPostage;  // 0x1C, Return Postage badge: half the damage goes back
    s32 type;           // 0x20
    u32 msg;            // 0x24
    s32 count;          // 0x28, damage dealt back to the attacker
    u32 hitEvent;       // 0x2C, hit event run on the target (BattleRunHitEvent)
} CounterWork;

s32 BattleCheckDamage(struct BattleUnit* unit1, struct BattleUnit* unit2, struct BattleWorkUnitPart* part, struct BattleWeapon* weapon, s32 flags);
void BattleAttackDeclare(s32* unitIdx, s32* partIdx);
void BattleInitCounterPreCheckWork(CounterWork* work);
s32 BattleCheckCounter(CounterWork* work, struct BattleUnit* attacker, struct BattleUnit* target,
                       struct BattleWorkUnitPart* part, struct BattleWeapon* weapon, u32* flags);
