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
    DAMAGE_CODE_NORMAL = 0x17, // hit by a normal-element weapon
    DAMAGE_CODE_FIRE = 0x18,
    DAMAGE_CODE_EXPLOSION = 0x19,
    DAMAGE_CODE_ICE = 0x1A,
    DAMAGE_CODE_ELECTRIC = 0x1B,
    DAMAGE_CODE_NO_DAMAGE = 0x1E, // the hit did no damage (default result)
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
} BattleDamageCode;

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
