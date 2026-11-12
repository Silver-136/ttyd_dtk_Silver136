// Battle audience: the crowd in the stands, their types, movement, mood and the items they throw.

#include "version.h"
#include "battle/battle_audience.h"
#include "battle/battle_audience_anim.h"
#include "battle/battle.h"
#include "battle/battle_database_common.h"
#include "battle/battle_unit.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/icondrv.h"
#include "icon_ids.h"
#include "driver/windowdrv.h"
#include "data/item_data.h"
#include "driver/npcdrv.h"
#include "mario/mario_pouch.h"
#include "pmario_sound.h"
#include "system.h"
#include <string.h>
#include <dolphin/gx.h>
#include "battle/battle_status_effect.h"
#include "statuswindow.h"
#include "battle/battle_message.h"
#include "driver/swdrv.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_cmd.h"
#include "battle/battle_break_slot.h"
#include "battle/battle_audience_kinopio.h"
#include "battle/battle_audience_kind_data.h"
#include "libc_decls.h"
/** Announces that the Puni in the audience all ran away (btl_msg_audience_puni_escape), for 120 frames. */
static EVT_BEGIN(msg_puni_all_escape)
    CALL(btlevtcmd_AnnounceMessage, 0, 0, -225, STRING("btl_msg_audience_puni_escape"), 120)
    RETURN
EVT_END()

/** Announces that a crowd of Puni has gathered in the audience (btl_msg_audience_puni_meeting). */
static EVT_BEGIN(msg_puni_all_enter)
    CALL(btlevtcmd_AnnounceMessage, 0, 0, -225, STRING("btl_msg_audience_puni_meeting"), 120)
    RETURN
EVT_END()

/** Announces that the Crazee Dayzee in the audience sang everyone to sleep (btl_msg_audience_pansy_sleep). */
static EVT_BEGIN(msg_pansy_sing)
    CALL(btlevtcmd_AnnounceMessage, 0, 0, -225, STRING("btl_msg_audience_pansy_sleep"), 120)
    RETURN
EVT_END()

/** Announces that a Bulky Bob-omb in the audience lit its fuse (btl_msg_audience_heavybomb_dynamite). */
static EVT_BEGIN(msg_heavy_bomb_fire)
    CALL(btlevtcmd_AnnounceMessage, 0, 0, -225, STRING("btl_msg_audience_heavybomb_dynamite"), 120)
    RETURN
EVT_END()

// A macro, not an inline function: as a function the registers in the callers change.
#define AUD_SOUND_START(sound, dur, fade)                                                           \
    if ((sound)->sfxId != -1) {                                                                     \
        (sound)->flags |= AUD_SOUND_FLAG_PLAYING;                                                   \
        (sound)->duration = (dur);                                                                  \
        (sound)->fadeoutTime = (fade);                                                             \
        (sound)->baseSfxVolume = (sound)->currentBaseSfxVolume = psndSFX_get_vol((sound)->sfxId); \
        (sound)->forceFadeLength = 0;                                                              \
        (sound)->forceFadeTimer = 0;                                                               \
        (sound)->volumeMultiplier = 100;                                                           \
    }

// local prototypes
static void BattleAudienceSettingAudience(void);
static void BattleAudienceGuestTPLRead(s32 index, AudienceMemberType member, const char* tplName);
static void BattleAudienceCtrlProcess(void);
static void BattleAudienceSetThrowItemMax(void);
static void BattleAudienceItemCtrlProcess(void);
static void BattleAudienceApSrcCtrlProcess(void);
static BOOL BattleAudienceApSrcEntry(void);
static void BattleAudienceWinCtrlProcess(void);
static void BattleAudienceAnimProcess(void);
static void BattleAudienceDispAudience(CameraId camId, void* param);
static void BattleAudienceDispItem(CameraId camId, void* param);
static void BattleAudienceDispApSrc(CameraId camId, void* param);
static void BattleAudienceDispWin(CameraId camId, void* param);
static void BattleAudience_Entry_Sub(s32 index, s32 type, u8 status);
s32 BattleAudience_NoUsedHaitiRand(void);
s32 BattleAudience_NoUsedFCHaitiRand(void);
static s32 BattleAudience_GetPPAudienceNum_R(void);
static s32 BattleAudience_GetPPAudienceNum_L(void);
static s32 BattleAudience_GetPPAudienceNum_RL_Sub(s32 offset);
static BOOL BattleAudience_GetPPAudienceNum_Sub(s32 id);

#if VERSION <= VERSION_DEMO_JP
static void BattleAudienceNumToTargetSub(void);
#else
static s32 BattleAudienceNumToTargetSub(void);
#endif

static void BattleAudienceAddTargetNumSub(f32 value);
void BattleAudienceAddPuni(s32 num);
static void BattleAudienceAddPhaseEvtList(s32 type);
static void BattleAudienceCheer(s32 type);
static void BattleAudienceJoy(s32 type);
static void BattleAudienceJoy_Sub(s32 a, s32 b, s32 c, s32 type);
void BattleAudienceSoundNoiseAlways(void);
static s32 BattleAudienceSound0(const char* name);
static s32 BattleAudienceSound1(const char* name, s32 right, s32 left);
static void BattleAudienceSoundGetInfo1(s32 right, s32 left, u8* vol, Vec* pos);
static s32 BattleAudienceSound2(const char* name, s32 index);
static void BattleAudienceSoundGetInfo2(s32 index, Vec* pos);
static void BattleAudienceSoundMain(void);
static void BattleAudienceNoiseMain(void);

/** Weapon for an empty can thrown by a spectator (ITEM_AUDIENCE_CAN): 1 damage plus 1 per extra hit. */
static BattleWeapon weapon_data_akikan = {
    NULL, 0, // name..icon
    {0}, // pad_06
    ITEM_AUDIENCE_CAN, // itemId
    NULL, // description
    100, // baseAccuracy
    0, 0, // baseFpCost..baseSpCost
    2, // superguardsAllowed
    0.0f, 0, 0, 0, 0, // reserved_14..reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {1, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_PIERCE_DEFENSE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
    0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
    0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
    0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
    0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Weapon for a rock thrown by a spectator (ITEM_AUDIENCE_ROCK); copied into presentWeapon like the can's. */
static BattleWeapon weapon_data_brick = {
    NULL, 0, // name..icon
    {0}, // pad_06
    ITEM_AUDIENCE_ROCK, // itemId
    NULL, // description
    100, // baseAccuracy
    0, 0, // baseFpCost..baseSpCost
    2, // superguardsAllowed
    0.0f, 0, 0, 0, 0, // reserved_14..reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {1, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_PIERCE_DEFENSE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
    0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
    0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
    0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
    0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Weapon for a bone thrown by a Dull Bones in the audience (ITEM_AUDIENCE_BONE). */
static BattleWeapon weapon_data_bone = {
    NULL, 0, // name..icon
    {0}, // pad_06
    ITEM_AUDIENCE_BONE, // itemId
    NULL, // description
    100, // baseAccuracy
    0, 0, // baseFpCost..baseSpCost
    2, // superguardsAllowed
    0.0f, 0, 0, 0, 0, // reserved_14..reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {1, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_PIERCE_DEFENSE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
    0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
    0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
    0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
    0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Weapon for a hammer thrown by a Hammer Bro in the audience (ITEM_AUDIENCE_HAMMER). */
static BattleWeapon weapon_data_hammer = {
    NULL, 0, // name..icon
    {0}, // pad_06
    ITEM_AUDIENCE_HAMMER, // itemId
    NULL, // description
    100, // baseAccuracy
    0, 0, // baseFpCost..baseSpCost
    2, // superguardsAllowed
    0.0f, 0, 0, 0, 0, // reserved_14..reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {2, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_PIERCE_DEFENSE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
    0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
    0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
    0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
    0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/**
 * Seating layout per Mario's rank (Rising Star, B-List Star, A-List Star, Superstar): how many seats are open in
 * each of the ten rows (centred in the 20 columns), so the stands fill out as the rank goes up.
 * The free-seat pickers (BattleAudience_NoUsedHaitiRand and kin) choose from these seats.
 */
static s32 haiti_data[][10] = {
    { 18, 16, 16, 0, 0, 0, 0, 0, 0, 0 },
    { 18, 18, 18, 16, 16, 14, 0, 0, 0, 0 },
    { 20, 20, 20, 20, 18, 18, 18, 16, 0, 0 },
    { 20, 20, 20, 20, 20, 20, 20, 20, 20, 20 },
};

#if VERSION >= VERSION_US
// US and later: the first three rows narrowed to 12/14/14 seats; BattleAudience_NoUsedFCHaitiRandCenter seats new
// members there while fewer than 20 are present
s32 haiti_data_front_narrow[][10] = {
    { 12, 14, 14, 0, 0, 0, 0, 0, 0, 0 },
    { 12, 14, 14, 16, 16, 14, 0, 0, 0, 0 },
    { 12, 14, 14, 20, 18, 18, 18, 16, 0, 0 },
    { 12, 14, 14, 20, 20, 20, 20, 20, 20, 20 },
};
#endif

/**
 * Loads the TPL texture file at path (relative to the disc root) into file memory;
 * used for the audience textures.
 */
static FileEntry* tplRead(const char* path) {
    return fileAllocf(FILE_TYPE_TPL, "%s/%s", getMarioStDvdRoot(), path);
}

/**
 * Returns the audience work in BattleWork: the whole crowd, its thrown items, sparkles, sounds and window.
 */
BattleAudience* BattleAudienceBaseGetPtr(void) {
    return &_battleWorkPointer->audience;
}

/** Returns audience member id, one of the 200 seats (column id % AUDIENCE_SEAT_COLUMNS, row id / AUDIENCE_SEAT_COLUMNS). */
BattleAudienceMember* BattleAudienceGetPtr(s32 id) {
    return &BattleAudienceBaseGetPtr()->members[id];
}

/** Returns thrown-item slot id (0-99). */
BattleAudienceItem* BattleAudienceItemGetPtr(s32 id) {
    return &BattleAudienceBaseGetPtr()->items[id];
}

/** Returns the audience-count window shown during command input. */
BattleAudienceWindow* BattleAudienceWinGetPtr(void) {
    return &BattleAudienceBaseGetPtr()->window;
}

/**
 * Returns crowd sound channel id (0-23); each kind of crowd sound
 * (noise, cheers, boos, whistles ...) has its own.
 */
BattleAudienceSound* BattleAudienceSoundGetPtr(s32 id) {
    return &BattleAudienceBaseGetPtr()->sounds[id];
}

/**
 * Sets up the audience when a battle starts (from battle.c):
 * clears the audience work, seats the starting crowd (BattleAudienceSettingAudience),
 * loads the normal audience texture and Luigi's guest texture and rolls the battle's item-throw limit.
 * Then counts the members present on each half for the audience window (not those arriving or leaving),
 * starts the crowd-noise loop fading to 60% over 180 frames and has the crowd move toward its target size.
 */
void BattleAudience_Init(void) {
    BattleAudience* audience;
    BattleAudienceWindow* window;
    int j;
    int i;
    u8 status;

    audience = BattleAudienceBaseGetPtr();
    window = BattleAudienceWinGetPtr();
    pouchGetPtr();
    memset(audience, 0, sizeof(BattleAudience));
    BattleAudienceSettingAudience();
    audience->normalTex = tplRead("battle/audience/audience_normal.tpl");
    BattleAudienceGuestTPLRead(0, AUDIENCE_LUIGI, "audience_luigi.tpl");
    BattleAudienceSetThrowItemMax();

    audience->turnEndPhaseEventChance = 0;

    audience->currentAudienceIntCountLeft = 0;
    audience->currentAudienceIntCountRight = 0;
    audience->currentAudienceIntCount = 0;

    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) {
            int index = (i * 20) + j + 10;
            if ((u8)BattleAudience_GetExist(index) == TRUE) {
                status = BattleAudienceGetPtr(index)->status;
                if (status != AUDIENCE_STATUS_ESCAPE && status != AUDIENCE_STATUS_ENTER) {
                    audience->currentAudienceIntCountRight++;
                }
            }
        }
    }

    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) {
            s32 index = i * 20 + j;
            if ((u8)BattleAudience_GetExist(index) == TRUE) {
                status = BattleAudienceGetPtr(index)->status;
                if (status != AUDIENCE_STATUS_ESCAPE && status != AUDIENCE_STATUS_ENTER) {
                    audience->currentAudienceIntCountLeft++;
                }
            }
        }
    }

    audience->currentAudienceIntCount = audience->currentAudienceIntCountRight + audience->currentAudienceIntCountLeft;
    window->count = (f32)audience->currentAudienceIntCount;
    BattleAudienceSoundNoiseAlways();
    BattleAudienceSoundSetVol(AUD_SOUND_NOISE, 60, 180);
    audience->excited = FALSE;
    BattleAudienceNumToTarget();
}

/**
 * Per-frame audience update (from battle.c): slides the audience window in when command input opens,
 * then updates the members, thrown items, Star Power sparkles, the window and the crowd sounds,
 * and remembers whether command input was open for the next frame.
 */
void BattleAudience_Main(void) {
    BattleWork* wp = _battleWorkPointer;
    BattleAudience* audience = BattleAudienceBaseGetPtr();

    if (!(audience->flags & AUDIENCE_FLAG_PREV_COMMAND_INPUT) && wp->flags & BATTLE_FLAG_COMMAND_INPUT) {
        BattleAudience_WinSetActive(1);
    }
    BattleAudienceCtrlProcess();
    BattleAudienceItemCtrlProcess();
    BattleAudienceApSrcCtrlProcess();
    BattleAudienceWinCtrlProcess();
    BattleAudienceSoundMain();
    if (wp->flags & BATTLE_FLAG_COMMAND_INPUT) {
        audience->flags |= AUDIENCE_FLAG_PREV_COMMAND_INPUT;
    } else {
        audience->flags &= ~AUDIENCE_FLAG_PREV_COMMAND_INPUT;
    }
}

/**
 * Clears the record of the current action
 * (action command, guard and Stylish results, enemy damaged, undamaged or defeated, the Stylish count)
 * before a unit acts; battle_seq.c calls it, as does BattleAudience_PerAct.
 */
void BattleAudience_ActInit(void) {
    BattleAudience* audience = BattleAudienceBaseGetPtr();
    audience->numStylishCommandsThisAttack = 0;
    audience->flags &= ~AUDIENCE_FLAG_AC_GOOD;
    audience->flags &= ~AUDIENCE_FLAG_AC_BAD;
    audience->flags &= ~AUDIENCE_FLAG_GUARD_GOOD;
    audience->flags &= ~AUDIENCE_FLAG_GUARD_BAD;
    audience->flags &= ~AUDIENCE_FLAG_ACROBAT_GOOD;
    audience->flags &= ~AUDIENCE_FLAG_ACROBAT_BAD;
#if VERSION <= VERSION_DEMO_JP
    audience->flags &= ~AUDIENCE_FLAG_STYLISH_MISSED;
#endif
    audience->flags &= ~AUDIENCE_FLAG_ENEMY_DAMAGE;
    audience->flags &= ~AUDIENCE_FLAG_ENEMY_NO_DAMAGE;
    audience->flags &= ~AUDIENCE_FLAG_KILL_ENEMY;
}

/**
 * Settles the crowd's reaction after an action (from battle_seq.c).
 * First the Star Power the action earned becomes a cheer
 * (BattleAudienceCheer: the bigger reaction after a clean Stylish move,
 * none when a Stylish command both landed and failed); if the audience is frozen, that is all.
 * Then it sums a change in crowd size, in percent of the level's audience factor:
 * -5 for hitting an enemy for no damage; for a good action command,
 * the streak (up to 2) times the move's action-command multiplier,
 * -5 for a failed one unless it defeated an enemy; the streak for a good guard;
 * for Stylish moves twice the Stylish multiplier times the Stylish count (the early demos use their Stylish streak),
 * -5 for a failed one. Big gains queue phase reactions (gifts from Toads, Shy Guys and Luigi, a Puni crowd);
 * the thresholds for losses are never reached, since a failure counts only -5.
 * A Stylish move also brings in 10 Punies, and a pending Puni escape is announced.
 * Finally the change goes to BattleAudienceAddTargetNum and the record is cleared.
 */
void BattleAudience_PerAct(void) {
    BattleAudience* audience = BattleAudienceBaseGetPtr();
    f32 total;
    f32 delta;
    s32 good;
    pouchGetPtr();
    total = 0.0f;
    delta = total;
    if (audience->flags & AUDIENCE_FLAG_ACROBAT_GOOD) {
        if (!(audience->flags & AUDIENCE_FLAG_ACROBAT_BAD)) BattleAudienceCheer(3);
    } else {
        BattleAudienceCheer(2);
    }
#if VERSION <= VERSION_DEMO_JP
    if (audience->flags & AUDIENCE_FLAG_EVENT_CTRL) {
        return;
    }
    audience->stylishTotal += audience->numStylishCommandsThisAttack;
    if (audience->stylishTotal > 100) {
        audience->stylishTotal = 100;
    }
#else
    if (audience->flags & AUDIENCE_FLAG_EVENT_CTRL) return;
#endif
    if (!(audience->flags & AUDIENCE_FLAG_ENEMY_DAMAGE) && (audience->flags & AUDIENCE_FLAG_ENEMY_NO_DAMAGE)) {
        total -= 5.0f;
        BattleAudienceAddPhaseEvtList(5);
    }
    if (audience->impendingBonuses && ((audience->flags & AUDIENCE_FLAG_AC_GOOD) || (audience->flags & AUDIENCE_FLAG_AC_BAD))) {
        if (audience->flags & AUDIENCE_FLAG_AC_GOOD) {
            good = 1;
            audience->crowdPleasedStreak++;
            if (audience->crowdPleasedStreak > 2) audience->crowdPleasedStreak = 2;
            delta = audience->crowdPleasedStreak * audience->impendingBonuses->spAcSuccessMultiplier;
            audience->crowdDispleasedStreak = 0;
            total += delta;
        } else if (!(audience->flags & AUDIENCE_FLAG_KILL_ENEMY)) {
            audience->crowdPleasedStreak = 0;
            good = 0;
            audience->crowdDispleasedStreak++;
            if (audience->crowdDispleasedStreak > 5) audience->crowdDispleasedStreak = 5;
            delta = -5.0f;
            total += delta;
        }
    }
    if ((audience->flags & AUDIENCE_FLAG_GUARD_GOOD) || (audience->flags & AUDIENCE_FLAG_GUARD_BAD)) {
        if (audience->flags & AUDIENCE_FLAG_GUARD_GOOD) {
            good = 1;
            audience->crowdPleasedStreak++;
            if (audience->crowdPleasedStreak > 2) audience->crowdPleasedStreak = 2;
            delta = audience->crowdPleasedStreak;
            audience->crowdDispleasedStreak = 0;
            total += delta;
        } else {
            audience->crowdPleasedStreak = 0;
            good = 0;
        }
    }
    if (good == 1) {
        if (delta >= 25.0f) BattleAudienceAddPhaseEvtList(0);
        if (delta >= 40.0f) BattleAudienceAddPhaseEvtList(3);
        if (delta >= 20.0f) BattleAudienceAddPhaseEvtList(8);
        if (delta >= 15.0f) BattleAudienceAddPhaseEvtList(13);
    } else {
        if (delta <= -15.0f) BattleAudienceAddPhaseEvtList(1);
        if (delta <= -20.0f) BattleAudienceAddPhaseEvtList(2);
        if (delta <= -20.0f) BattleAudienceAddPhaseEvtList(5);
        if (delta <= -15.0f) BattleAudienceAddPhaseEvtList(9);
        if (delta <= -20.0f) BattleAudienceAddPhaseEvtList(11);
        if (delta <= -10.0f) BattleAudienceAddPhaseEvtList(13);
    }
    if (audience->impendingBonuses && ((audience->flags & AUDIENCE_FLAG_ACROBAT_GOOD) || (audience->flags & AUDIENCE_FLAG_ACROBAT_BAD))) {
        if (audience->flags & AUDIENCE_FLAG_ACROBAT_GOOD) {
#if VERSION <= VERSION_DEMO_JP
            audience->acStreak++;
            if (audience->acStreak > 2) {
                audience->acStreak = 2;
            }
            delta = audience->impendingBonuses->stylishCommandMultiplier * (audience->acStreak * 2);
#else
            delta = audience->impendingBonuses->stylishCommandMultiplier * (audience->numStylishCommandsThisAttack * 2);
#endif
            total += delta;
#if VERSION <= VERSION_DEMO_JP
            if (delta >= 30.0f) {
                BattleAudienceAddPhaseEvtList(0);
            }
            if (delta >= 40.0f) {
                BattleAudienceAddPhaseEvtList(3);
            }
            if (delta >= 25.0f) {
                BattleAudienceAddPhaseEvtList(8);
            }
            if (delta >= 20.0f) {
                BattleAudienceAddPhaseEvtList(13);
            }
        }
    }
    if (audience->impendingBonuses && (audience->flags & AUDIENCE_FLAG_STYLISH_MISSED)) {
        total -= 5.0f;
        audience->acStreak = 0;
    }
    if (audience->impendingBonuses && ((audience->flags & AUDIENCE_FLAG_ACROBAT_GOOD) || (audience->flags & AUDIENCE_FLAG_ACROBAT_BAD))) {
        if (audience->flags & AUDIENCE_FLAG_ACROBAT_GOOD) {
#else
            if (delta >= 30.0f) BattleAudienceAddPhaseEvtList(0);
            if (delta >= 40.0f) BattleAudienceAddPhaseEvtList(3);
            if (delta >= 25.0f) BattleAudienceAddPhaseEvtList(8);
            if (delta >= 20.0f) BattleAudienceAddPhaseEvtList(13);
#endif
            BattleAudienceAddPuni(10);
        }
#if VERSION >= VERSION_JP
        if (audience->flags & AUDIENCE_FLAG_ACROBAT_BAD) total -= 5.0f;
#endif
    }
    if (audience->flags & AUDIENCE_FLAG_PUNI_ESCAPE) {
        audience->event = evtEntry(&msg_puni_all_escape, 0, EVENT_START_IMMEDIATE);
        audience->flags &= ~AUDIENCE_FLAG_PUNI_ESCAPE;
    }
    BattleAudienceAddTargetNum(audience->audienceDeltaMultiplier * (0.01f * total), 0.0f);
    BattleAudience_ActInit();
}

/**
 * TRUE while the audience's announcement script (the Puni escape from BattleAudience_PerAct) is still
 * running, so battle_seq.c waits after an action; otherwise forgets the script and returns FALSE.
 */
u8 BattleAudience_CheckReaction(void) {
    BattleAudience* audience;
    EventEntry* evt;

    audience = BattleAudienceBaseGetPtr();
    BattleAudienceGetPtr(0);
    evt = audience->event;
    if (evt && evtCheckID(evt->eventId)) {
        return TRUE;
    }
    audience->event = NULL;
#if VERSION <= VERSION_DEMO_JP
    if (audience->flags & AUDIENCE_FLAG_EVENT_CTRL) {
        return FALSE;
    }
    return FALSE;
}

static u8 check_exe_phase_evt_status(BattleAudienceMember* member) {
    u8 status = member->status;

    if (status == AUDIENCE_STATUS_SLEEP) {
        return FALSE;
    }
    if (status == AUDIENCE_STATUS_ESCAPE) {
        return FALSE;
    }
    if (status == AUDIENCE_STATUS_ENTER) {
        return FALSE;
    }
    if (status == 21) {
        return FALSE;
    }
    if (status == 22) {
        return FALSE;
    }
    if (member->flags & AUDIENCE_MEMBER_FLAG_FRONT_CENTER) {
        return TRUE;
    }
    return TRUE;
#else
    return FALSE;
}

static u8 check_exe_phase_evt_status(s32 index, s32 type) {
    BattleAudienceMember* member;
    u8 status;

    member = BattleAudienceGetPtr(index);
    if (!(u8)BattleAudience_GetSysCtrl(index)) {
        return FALSE;
    }
    if (member->type != type) {
        return FALSE;
    }
    status = member->status;
    if (status == AUDIENCE_STATUS_SLEEP) return FALSE;
    if (status == AUDIENCE_STATUS_ESCAPE) return FALSE;
    if (status == AUDIENCE_STATUS_ENTER) return FALSE;
    if (status == 21) return FALSE;
    if (status == 22) return FALSE;
    return (member->flags & AUDIENCE_MEMBER_FLAG_FRONT_CENTER) != 0;
#endif
}

/**
 * Called by battle_seq.c when a phase starts. At the player's phase (unless the audience is frozen) members
 * still holding an item put it away and every item not yet thrown is removed;
 * at the enemy phase the item-throw limit is rolled again
 * and the phase reactions are armed (BattleAudience_CheckReactionPerPhase).
 */
void BattleAudience_PerPhase(u32 value) {
    BattleAudience* audience;
    BattleAudienceMember* member;
    s32 i;

    audience = BattleAudienceBaseGetPtr();
    if (value == BATTLE_PHASE_PLAYER && !(audience->flags & AUDIENCE_FLAG_EVENT_CTRL)) {
        for (i = 0; i < AUDIENCE_MEMBER_MAX; i++) {
            member = BattleAudienceGetPtr(i);
            if ((u8)BattleAudience_GetExist(i) == 1 && member->status == AUDIENCE_STATUS_ITEM_ON) {
                if (member->itemId != -1) {
                    BattleAudienceItemGetPtr(member->itemId)->flags = 0;
                }
                member->itemId = -1;
                BattleAudience_ChangeStatus(i, AUDIENCE_STATUS_WAIT);
            }
        }
        for (i = 0; i < AUDIENCE_ITEM_MAX; i++) {
            if (BattleAudienceItemGetPtr(i)->state <= 1) {
                BattleAudienceItemGetPtr(i)->flags = 0;
            }
        }
    }
    if (value == BATTLE_PHASE_ENEMY) {
        BattleAudienceSetThrowItemMax();
        audience->flags |= AUDIENCE_FLAG_PHASE_REACTION;
        audience->checkPhaseReactionState = 0;
    }
}

/**
 * Plays the crowd's phase reactions at the start of the enemy phase;
 * battle_seq.c calls it every frame until it returns FALSE.
 * First Bulky Bob-ombs whose fuse has burnt down explode (announced once the blasts end),
 * then a pending Puni escape is announced and the queued reactions are dropped.
 * Without a blast, of the reactions queued since the last phase
 * (BattleAudienceAddPhaseEvtList) those with a member able to take part are kept,
 * and one is picked at random: 0 a Toad throws an item, 1 the Koopas hide in their shells,
 * 2 the Dull Bones throw bones, 3 a crowd of Punies arrives (announced), 4 a Boo turns a unit invisible,
 * 5 the X-Nauts throw rocks, 6 an X-Naut throws an item, 7 a Hammer Bro throws a hammer,
 * 8 a Shy Guy throws an item, 9 a Shy Guy runs onto the stage,
 * 10 a Crazee Dayzee sings the rest of the crowd to sleep (announced), 11 a Bulky Bob-omb lights its fuse,
 * 12 a Piranha Plant bites a neighbour, 13 Luigi throws an item.
 * 9, 11 and 12 are left out while audience hazards are off.
 * Last, sleeping members count down to waking, Koopas in their shells come out when their timer ends
 * and the post-BINGO! boost loses a turn. Skipped, apart from that last step, while the audience is frozen.
 */
u8 BattleAudience_CheckReactionPerPhase(void) {
    BattleAudience* audience;
    BattleAudienceMember* member;
    s32 i, counter, k, qn;
    s32 queue[14];
    s32 list[200];
    u32 n;

    audience = BattleAudienceBaseGetPtr();
    pouchGetPtr();
    if (!(audience->flags & AUDIENCE_FLAG_PHASE_REACTION)) {
        return FALSE;
    }
    switch (audience->checkPhaseReactionState) {
        case 0:
            audience->checkPhaseReactionState = 5;
            if (audience->flags & AUDIENCE_FLAG_EVENT_CTRL) {
                audience->checkPhaseReactionState = 15;
                break;
            }

            /* fallthrough */
        case 5:
            for (counter = 0, i = 0; i < AUDIENCE_MEMBER_MAX; i++) {
                member = BattleAudienceGetPtr(i);

                if ((u8)BattleAudience_GetSysCtrl(i) == 1 && member->type == AUDIENCE_BULKY_BOBOMB &&
                        member->status == AUDIENCE_STATUS_IGNITE) {
                    member->statusTimer--;
                    if (member->statusTimer <= 0) {
                        BattleAudience_ChangeStatus(i, AUDIENCE_STATUS_FIRE);
                        counter++;
                    }
                }
            }
            if (counter >= 1) {
                audience->checkPhaseReactionState = 6;
            } else {
                audience->checkPhaseReactionState = 7;
            }

            /* fallthrough */
        case 6:
        case 7:
            for (i = 0; i < AUDIENCE_MEMBER_MAX; i++) {
                member = BattleAudienceGetPtr(i);
                if ((u8)BattleAudience_GetSysCtrl(i) == 1 && member->type == AUDIENCE_BULKY_BOBOMB &&
                        member->status == AUDIENCE_STATUS_FIRE) {
                    break;
                }
            }
            if (i == 200) { // no Bulky Bob-omb is still firing
                if (audience->checkPhaseReactionState == 6) {
                    audience->checkPhaseReactionState = 8;
                } else {
                    audience->checkPhaseReactionState = 13;
                }
            }
            break;
        case 8:
            audience->event = evtEntry(&msg_heavy_bomb_fire, 0, EVENT_START_IMMEDIATE);
            audience->checkPhaseReactionState = 9;
            break;
        case 9:
            if (!evtCheckID(audience->event->eventId)) {
                audience->event = NULL;
                audience->checkPhaseReactionState = 10;
            }
            break;
        case 10:
            if (audience->flags & AUDIENCE_FLAG_PUNI_ESCAPE) {
                audience->event = evtEntry(&msg_puni_all_escape, 0, EVENT_START_IMMEDIATE);
                audience->flags &= ~AUDIENCE_FLAG_PUNI_ESCAPE;
                audience->checkPhaseReactionState = 11;
            } else {
                audience->checkPhaseReactionState = 15;
            }
            break;
        case 11:
            if (!evtCheckID(audience->event->eventId)) {
                audience->event = NULL;
                audience->checkPhaseReactionState = 15;
            }
            break;
        case 13:
            memcpy(queue, audience->phaseList, audience->phaseListCount * sizeof(s32));
            qn = audience->phaseListCount;
            audience->phaseListCount = counter = 0;
            for (; counter < qn; counter++) {
                switch (queue[counter]) {
                    case 0:
                        for (i = 0; i <= 59; i++) {
#if VERSION <= VERSION_DEMO_JP
                            member = BattleAudienceGetPtr(i);
                            if ((u8)BattleAudience_GetSysCtrl(i) && member->type == AUDIENCE_TOAD && check_exe_phase_evt_status(member)) {
#else
                            if (check_exe_phase_evt_status(i, AUDIENCE_TOAD)) {
#endif
                                BattleAudienceAddPhaseEvtList(0);
                                break;
                            }
                        }
                        break;
                    case 1:
                        for (i = 0; i <= 59; i++) {
#if VERSION <= VERSION_DEMO_JP
                            member = BattleAudienceGetPtr(i);
                            if ((u8)BattleAudience_GetSysCtrl(i) && member->type == AUDIENCE_KOOPA && check_exe_phase_evt_status(member) && member->status != AUDIENCE_STATUS_SHELL) {
#else
                            if (check_exe_phase_evt_status(i, AUDIENCE_KOOPA) && BattleAudienceGetPtr(i)->status != AUDIENCE_STATUS_SHELL) {
#endif
                                BattleAudienceAddPhaseEvtList(1);
                                break;
                            }
                        }
                        break;
                    case 2:
                        for (i = 0; i <= 59; i++) {
#if VERSION <= VERSION_DEMO_JP
                            member = BattleAudienceGetPtr(i);
                            if ((u8)BattleAudience_GetSysCtrl(i) && member->type == AUDIENCE_DULL_BONES && check_exe_phase_evt_status(member)) {
#else
                            if (check_exe_phase_evt_status(i, AUDIENCE_DULL_BONES)) {
#endif
                                BattleAudienceAddPhaseEvtList(2);
                                break;
                            }
                        }
                        break;
                    case 3:
                        if (audience->typeWeights[AUDIENCE_PUNI]) {
                            for (i = 0; i < audience->maxAudience; ) {
                                if (!(u8)BattleAudience_GetExist(i)) {
                                    BattleAudienceAddPhaseEvtList(3);
                                    break;
                                }
                                if (BattleAudienceGetPtr(i)->flags & AUDIENCE_MEMBER_FLAG_WIDE) {
                                    i += 2;
                                } else {
                                    i++;
                                }
                            }
                        }
                        break;
                    case 4:
                        for (i = 0; i <= 59; i++) {
#if VERSION <= VERSION_DEMO_JP
                            member = BattleAudienceGetPtr(i);
                            if ((u8)BattleAudience_GetSysCtrl(i) && member->type == AUDIENCE_BOO && check_exe_phase_evt_status(member)) {
#else
                            if (check_exe_phase_evt_status(i, AUDIENCE_BOO)) {
#endif
                                BattleAudienceAddPhaseEvtList(4);
                                break;
                            }
                        }
                        break;
                    case 5:
                        for (i = 0; i <= 59; i++) {
#if VERSION <= VERSION_DEMO_JP
                            member = BattleAudienceGetPtr(i);
                            if ((u8)BattleAudience_GetSysCtrl(i) && member->type == AUDIENCE_XNAUT && check_exe_phase_evt_status(member)) {
#else
                            if (check_exe_phase_evt_status(i, AUDIENCE_XNAUT)) {
#endif
                                BattleAudienceAddPhaseEvtList(5);
                                break;
                            }
                        }
                        break;
                    case 6:
                        for (i = 0; i <= 59; i++) {
#if VERSION <= VERSION_DEMO_JP
                            member = BattleAudienceGetPtr(i);
                            if ((u8)BattleAudience_GetSysCtrl(i) && member->type == AUDIENCE_XNAUT && check_exe_phase_evt_status(member)) {
#else
                            if (check_exe_phase_evt_status(i, AUDIENCE_XNAUT)) {
#endif
                                BattleAudienceAddPhaseEvtList(6);
                                break;
                            }
                        }
                        break;
                    case 7:
                        for (i = 0; i <= 59; i++) {
#if VERSION <= VERSION_DEMO_JP
                            member = BattleAudienceGetPtr(i);
                            if ((u8)BattleAudience_GetSysCtrl(i) && member->type == AUDIENCE_HAMMER_BRO && check_exe_phase_evt_status(member)) {
#else
                            if (check_exe_phase_evt_status(i, AUDIENCE_HAMMER_BRO)) {
#endif
                                BattleAudienceAddPhaseEvtList(7);
                                break;
                            }
                        }
                        break;
                    case 8:
                        for (i = 0; i <= 59; i++) {
#if VERSION <= VERSION_DEMO_JP
                            member = BattleAudienceGetPtr(i);
                            if ((u8)BattleAudience_GetSysCtrl(i) && member->type == AUDIENCE_SHY_GUY && check_exe_phase_evt_status(member)) {
#else
                            if (check_exe_phase_evt_status(i, AUDIENCE_SHY_GUY)) {
#endif
                                BattleAudienceAddPhaseEvtList(8);
                                break;
                            }
                        }
                        break;
                    case 9:
                        if (!(audience->flags & AUDIENCE_FLAG_NO_AUDIENCE_EVENTS)) {
                            for (i = 0; i <= 59; i++) {
#if VERSION <= VERSION_DEMO_JP
                                member = BattleAudienceGetPtr(i);
                                if ((u8)BattleAudience_GetSysCtrl(i) && member->type == AUDIENCE_SHY_GUY && check_exe_phase_evt_status(member)) {
#else
                                if (check_exe_phase_evt_status(i, AUDIENCE_SHY_GUY)) {
#endif
                                    BattleAudienceAddPhaseEvtList(9);
                                    break;
                                }
                            }
                        }
                        break;
                    case 10:
                        for (i = 0; i <= 59; i++) {
#if VERSION <= VERSION_DEMO_JP
                            member = BattleAudienceGetPtr(i);
                            if ((u8)BattleAudience_GetSysCtrl(i) && member->type == AUDIENCE_CRAZY_DAYZEE && check_exe_phase_evt_status(member)) {
#else
                            if (check_exe_phase_evt_status(i, AUDIENCE_CRAZY_DAYZEE)) {
#endif
                                BattleAudienceAddPhaseEvtList(10);
                                break;
                            }
                        }
                        break;
                    case 11:
                        if (!(audience->flags & AUDIENCE_FLAG_NO_AUDIENCE_EVENTS)) {
                            for (i = 0; i <= 59; i++) {
#if VERSION <= VERSION_DEMO_JP
                                member = BattleAudienceGetPtr(i);
                                if ((u8)BattleAudience_GetSysCtrl(i) && member->type == AUDIENCE_BULKY_BOBOMB && check_exe_phase_evt_status(member) && member->status != AUDIENCE_STATUS_IGNITE) {
#else
                                if (check_exe_phase_evt_status(i, AUDIENCE_BULKY_BOBOMB) && BattleAudienceGetPtr(i)->status != AUDIENCE_STATUS_IGNITE) {
#endif
                                    BattleAudienceAddPhaseEvtList(11);
                                    break;
                                }
                            }
                        }
                        break;
                    case 12:
                        if (!(audience->flags & AUDIENCE_FLAG_NO_AUDIENCE_EVENTS)) {
                            for (i = 0; i <= 59; i++) {
#if VERSION <= VERSION_DEMO_JP
                                member = BattleAudienceGetPtr(i);
                                if ((u8)BattleAudience_GetSysCtrl(i) && member->type == AUDIENCE_PIRANHA_PLANT && check_exe_phase_evt_status(member) && BattleAudienceDetectPakkunEatTarget(i) != -1) {
#else
                                if (check_exe_phase_evt_status(i, AUDIENCE_PIRANHA_PLANT) && BattleAudienceDetectPakkunEatTarget(i) != -1) {
#endif
                                    BattleAudienceAddPhaseEvtList(12);
                                    break;
                                }
                            }
                        }
                        break;
                    case 13:
                        for (i = 0; i <= 59; i++) {
#if VERSION <= VERSION_DEMO_JP
                            member = BattleAudienceGetPtr(i);
                            if ((u8)BattleAudience_GetSysCtrl(i) && member->type == AUDIENCE_LUIGI && check_exe_phase_evt_status(member)) {
#else
                            if (check_exe_phase_evt_status(i, AUDIENCE_LUIGI)) {
#endif
                                BattleAudienceAddPhaseEvtList(13);
                                break;
                            }
                        }
                        break;
                }
            }
            if (audience->phaseListCount >= 1) {
                audience->phaseList[0] = audience->phaseList[irand(audience->phaseListCount)];
                audience->phaseListCount = 1;
            } else {
                audience->phaseListCount = 0;
            }
            audience->checkPhaseReactionState = 12;
            audience->phaseSub = 0;

            /* fallthrough */
        case 12:
            if (audience->phaseListCount == 1) {
                switch (audience->phaseList[0]) {
                    case 0:
                        n = 0;
                        for (i = 0; i <= 59; i++) {
#if VERSION <= VERSION_DEMO_JP
                            member = BattleAudienceGetPtr(i);
                            if ((u8)BattleAudience_GetSysCtrl(i) && member->type == AUDIENCE_TOAD && check_exe_phase_evt_status(member)) {
#else
                            if (check_exe_phase_evt_status(i, AUDIENCE_TOAD)) {
#endif
                                list[n] = i;
                                n++;
                            }
                        }
                        BattleAudienceItemOn(list[irand(n)], ITEM_NULL, 1);
                        audience->phaseListCount = 0;
                        break;
                    case 1:
                        for (i = 0; i < AUDIENCE_MEMBER_MAX; i++) {
#if VERSION <= VERSION_DEMO_JP
                            member = BattleAudienceGetPtr(i);
                            if ((u8)BattleAudience_GetSysCtrl(i) && member->type == AUDIENCE_KOOPA && check_exe_phase_evt_status(member) && member->status != AUDIENCE_STATUS_SHELL) {
#else
                            if (check_exe_phase_evt_status(i, AUDIENCE_KOOPA) && BattleAudienceGetPtr(i)->status != AUDIENCE_STATUS_SHELL) {
#endif
                                BattleAudience_ChangeStatus(i, AUDIENCE_STATUS_SHELL);
#if VERSION <= VERSION_DEMO_JP
                                member->statusTimer = 5;
#else
                                BattleAudienceGetPtr(i)->statusTimer = 5;
#endif
                            }
                        }
                        audience->phaseListCount = 0;
                        break;
                    case 2:
                        for (i = 0; i <= 59; i++) {
#if VERSION <= VERSION_DEMO_JP
                            member = BattleAudienceGetPtr(i);
                            if ((u8)BattleAudience_GetSysCtrl(i) && member->type == AUDIENCE_DULL_BONES && check_exe_phase_evt_status(member)) {
#else
                            if (check_exe_phase_evt_status(i, AUDIENCE_DULL_BONES)) {
#endif
                                BattleAudienceItemOn(i, ITEM_AUDIENCE_BONE, 100);
                            }
                        }
                        audience->phaseListCount = 0;
                        break;
                    case 3:
                        switch (audience->phaseSub) {
                            case 0:
                                BattleAudienceAddPuni(200);
                                audience->phaseSub = 5;
                                break;
                            case 5:
                                for (i = 0; i < AUDIENCE_MEMBER_MAX; i++) {
                                    if ((u8)BattleAudience_GetSysCtrl(i) && BattleAudienceGetPtr(i)->status == AUDIENCE_STATUS_ENTER) break;
                                }
                                if (i == 200) {
                                    audience->phaseSub = 10;
                                }
                                break;
                            case 10:
                                audience->phaseSub = 15;
                                audience->event = evtEntry(&msg_puni_all_enter, 0, EVENT_START_IMMEDIATE);
                                break;
                            case 15:
                                if (!evtCheckID(audience->event->eventId)) {
                                    audience->event = NULL;
                                    audience->phaseListCount = 0;
                                }
                                break;
                        }
                        break;
                    case 4:
                        switch (audience->phaseSub) {
                            case 0:
                                for (counter = 0, i = 0; i <= 59; i++) {
#if VERSION <= VERSION_DEMO_JP
                                    member = BattleAudienceGetPtr(i);
                                    if ((u8)BattleAudience_GetSysCtrl(i) && member->type == AUDIENCE_BOO && check_exe_phase_evt_status(member)) {
#else
                                    if (check_exe_phase_evt_status(i, AUDIENCE_BOO)) {
#endif
                                        list[counter] = i;
                                        counter++;
                                    }
                                }
                                BattleAudience_ChangeStatus(list[irand(counter)], AUDIENCE_STATUS_TRANS_EVT);
                                audience->phaseSub = 5;
                            case 5:
                                for (i = 0; i <= 59; i++) {
                                    if ((u8)BattleAudience_GetSysCtrl(i) && BattleAudienceGetPtr(i)->status == AUDIENCE_STATUS_TRANS_EVT) break;
                                }
                                if (i == 60) {
                                    audience->phaseListCount = 0;
                                }
                                break;
                        }
                        break;
                    case 5:
                        for (i = 0; i <= 59; i++) {
#if VERSION <= VERSION_DEMO_JP
                            member = BattleAudienceGetPtr(i);
                            if ((u8)BattleAudience_GetSysCtrl(i) && member->type == AUDIENCE_XNAUT && check_exe_phase_evt_status(member)) {
#else
                            if (check_exe_phase_evt_status(i, AUDIENCE_XNAUT)) {
#endif
                                BattleAudienceItemOn(i, ITEM_AUDIENCE_ROCK, 100);
                            }
                        }
                        audience->phaseListCount = 0;
                        break;
                    case 6:
                        for (counter = 0, i = 0; i <= 59; i++) {
#if VERSION <= VERSION_DEMO_JP
                            member = BattleAudienceGetPtr(i);
                            if ((u8)BattleAudience_GetSysCtrl(i) && member->type == AUDIENCE_XNAUT && check_exe_phase_evt_status(member)) {
#else
                            if (check_exe_phase_evt_status(i, AUDIENCE_XNAUT)) {
#endif
                                list[counter] = i;
                                counter++;
                            }
                        }
                        BattleAudienceItemOn(list[irand(counter)], ITEM_NULL, 1);
                        audience->phaseListCount = 0;
                        break;
                    case 7:
                        for (counter = 0, i = 0; i <= 59; i++) {
#if VERSION <= VERSION_DEMO_JP
                            member = BattleAudienceGetPtr(i);
                            if ((u8)BattleAudience_GetSysCtrl(i) && member->type == AUDIENCE_HAMMER_BRO && check_exe_phase_evt_status(member)) {
#else
                            if (check_exe_phase_evt_status(i, AUDIENCE_HAMMER_BRO)) {
#endif
                                list[counter] = i;
                                counter++;
                            }
                        }
                        BattleAudienceItemOn(list[irand(counter)], ITEM_AUDIENCE_HAMMER, 1);
                        audience->phaseListCount = 0;
                        break;
                    case 8:
                        for (counter = 0, i = 0; i <= 59; i++) {
#if VERSION <= VERSION_DEMO_JP
                            member = BattleAudienceGetPtr(i);
                            if ((u8)BattleAudience_GetSysCtrl(i) && member->type == AUDIENCE_SHY_GUY && check_exe_phase_evt_status(member)) {
#else
                            if (check_exe_phase_evt_status(i, AUDIENCE_SHY_GUY)) {
#endif
                                list[counter] = i;
                                counter++;
                            }
                        }
                        BattleAudienceItemOn(list[irand(counter)], ITEM_NULL, 1);
                        audience->phaseListCount = 0;
                        break;
                    case 9:
                        switch (audience->phaseSub) {
                            case 0:
                                for (counter = 0, i = 0; i <= 59; i++) {
#if VERSION <= VERSION_DEMO_JP
                                    member = BattleAudienceGetPtr(i);
                                    if ((u8)BattleAudience_GetSysCtrl(i) && member->type == AUDIENCE_SHY_GUY && check_exe_phase_evt_status(member)) {
#else
                                    if (check_exe_phase_evt_status(i, AUDIENCE_SHY_GUY)) {
#endif
                                        list[counter] = i;
                                        counter++;
                                    }
                                }
                                BattleAudience_ChangeStatus(list[irand(counter)], AUDIENCE_STATUS_INTRUDE);
                                audience->phaseSub = 5;
                            case 5:
                                for (i = 0; i <= 59; i++) {
                                    if ((u8)BattleAudience_GetSysCtrl(i) && BattleAudienceGetPtr(i)->status == AUDIENCE_STATUS_INTRUDE) break;
                                }
                                if (i == 60) {
                                    audience->phaseListCount = 0;
                                }
                                break;
                        }
                        break;
                    case 10:
                        switch (audience->phaseSub) {
                            case 0:
                                for (counter = 0, i = 0; i <= 59; i++) {
#if VERSION <= VERSION_DEMO_JP
                                    member = BattleAudienceGetPtr(i);
                                    if ((u8)BattleAudience_GetSysCtrl(i) && member->type == AUDIENCE_CRAZY_DAYZEE && check_exe_phase_evt_status(member)) {
#else
                                    if (check_exe_phase_evt_status(i, AUDIENCE_CRAZY_DAYZEE)) {
#endif
                                        list[counter] = i;
                                        counter++;
                                    }
                                }
                                BattleAudience_ChangeStatus(list[irand(counter)], AUDIENCE_STATUS_SING);
                                audience->phaseSub = 5;
                            case 5:
                                for (i = 0; i <= 59; i++) {
                                    if ((u8)BattleAudience_GetSysCtrl(i) && BattleAudienceGetPtr(i)->status == AUDIENCE_STATUS_SING) break;
                                }
                                if (i == 60) {
                                    audience->phaseSub = 10;
                                }
                                break;
                            case 10:
                                audience->phaseSub = 15;
                                audience->event = evtEntry(&msg_pansy_sing, 0, EVENT_START_IMMEDIATE);
                                break;
                            case 15:
                                if (!evtCheckID(audience->event->eventId)) {
                                    audience->event = NULL;
                                    audience->phaseListCount = 0;
                                }
                                break;
                        }
                        break;
                    case 11:
                        for (counter = 0, i = 0; i <= 59; i++) {
#if VERSION <= VERSION_DEMO_JP
                            member = BattleAudienceGetPtr(i);
                            if ((u8)BattleAudience_GetSysCtrl(i) && member->type == AUDIENCE_BULKY_BOBOMB && check_exe_phase_evt_status(member) && member->status != AUDIENCE_STATUS_IGNITE) {
#else
                            if (check_exe_phase_evt_status(i, AUDIENCE_BULKY_BOBOMB) && BattleAudienceGetPtr(i)->status != AUDIENCE_STATUS_IGNITE) {
#endif
                                list[counter] = i;
                                counter++;
                            }
                        }
                        k = list[irand(counter)];
                        member = BattleAudienceGetPtr(k);
                        BattleAudience_ChangeStatus(k, AUDIENCE_STATUS_IGNITE);
                        member->statusTimer = 5;
                        audience->phaseListCount = 0;
                        break;
                    case 12:
                        switch (audience->phaseSub) {
                            case 0:
                                for (counter = 0, i = 0; i <= 59; i++) {
#if VERSION <= VERSION_DEMO_JP
                                    member = BattleAudienceGetPtr(i);
                                    if ((u8)BattleAudience_GetSysCtrl(i) && member->type == AUDIENCE_PIRANHA_PLANT && check_exe_phase_evt_status(member) && BattleAudienceDetectPakkunEatTarget(i) != -1) {
#else
                                    if (check_exe_phase_evt_status(i, AUDIENCE_PIRANHA_PLANT) && BattleAudienceDetectPakkunEatTarget(i) != -1) {
#endif
                                        list[counter] = i;
                                        counter++;
                                    }
                                }
                                BattleAudience_ChangeStatus(list[irand(counter)], AUDIENCE_STATUS_EAT);
                                audience->phaseSub = 5;
                            case 5:
                                for (i = 0; i <= 59; i++) {
                                    if ((u8)BattleAudience_GetSysCtrl(i) && BattleAudienceGetPtr(i)->status == AUDIENCE_STATUS_EAT) break;
                                }
                                if (i == 60) {
                                    audience->phaseListCount = 0;
                                }
                                break;
                        }
                        break;
                    case 13:
                        for (i = 0; i <= 59; i++) {
#if VERSION <= VERSION_DEMO_JP
                            member = BattleAudienceGetPtr(i);
                            if ((u8)BattleAudience_GetSysCtrl(i) && member->type == AUDIENCE_LUIGI && check_exe_phase_evt_status(member)) {
#else
                            if (check_exe_phase_evt_status(i, AUDIENCE_LUIGI)) {
#endif
                                BattleAudienceItemOn(i, ITEM_NULL, 1);
                                break;
                            }
                        }
                        audience->phaseListCount = 0;
                        break;
                }
            } else {
                audience->checkPhaseReactionState = 15;
            }
            break;
        case 15:
            for (i = 0; i < AUDIENCE_MEMBER_MAX; i++) {
                member = BattleAudienceGetPtr(i);

                // FAKE: comma expression; `--member->statusDelay <= 0` changes the object
                if ((u8)BattleAudience_GetSysCtrl(i) == 1 && member->status == AUDIENCE_STATUS_SLEEP &&
                        (member->statusDelay--, member->statusDelay <= 0)) {
                    BattleAudience_ChangeStatus(i, AUDIENCE_STATUS_WAIT_JUMP);
                    member->statusDelay = 0;
                }
            }
            for (i = 0; i < AUDIENCE_MEMBER_MAX; i++) {
                member = BattleAudienceGetPtr(i);
                if ((u8)BattleAudience_GetSysCtrl(i) == 1 && member->type == AUDIENCE_KOOPA && member->status == AUDIENCE_STATUS_SHELL) {
                    member->statusTimer--;
                    if (member->statusTimer <= 0) {
                        member->step = 4;
                    }
                }
            }
            BattleBreakSlot_DecBreakTurn();
        case 20:
            audience->flags &= ~AUDIENCE_FLAG_PHASE_REACTION;
            audience->phaseListCount = 0;
            return FALSE;
    }
    return TRUE;
}

/**
 * Queues the audience's draws for the frame (from battle_disp.c):
 * steps the members' animation scripts and, unless the audience is hidden, draws the members (two passes),
 * thrown items and Star Power sparkles; the audience-count window is drawn once Mario has Star Power.
 */
void BattleAudience_Disp(void) {
    BattleAudienceBaseGetPtr();
    BattleAudienceAnimProcess();
    if (!(BattleAudienceBaseGetPtr()->flags & AUDIENCE_FLAG_NO_DISP)) {
        dispEntry(CAMERA_3D, 0.0f, DISP_MODE_ALPHA_TEST, BattleAudienceDispAudience, NULL);
        dispEntry(CAMERA_3D, 0.0f, DISP_MODE_XLU, BattleAudienceDispAudience, NULL);
        dispEntry(CAMERA_3D, 0.0f, DISP_MODE_ALPHA_TEST, BattleAudienceDispItem, NULL);
        dispEntry(CAMERA_2D, 499.0f, DISP_MODE_OPAQUE, BattleAudienceDispApSrc, NULL);
    }
    if (pouchGetPtr()->starPowersObtained) {
        dispEntry(CAMERA_2D, 499.0f, DISP_MODE_ALPHA_TEST, BattleAudienceDispWin, NULL);
    }
}

/**
 * Battle end (from battle.c): frees the audience textures and, once Mario has Star Power,
 * saves the target audience size (clamped to 0 and the rank's maximum) to the pouch for the next battle.
 * US and later also free the copy of the Star Power bonuses.
 */
void BattleAudience_End(void) {
    BattleAudience* audience;
    FileEntry* obj;
    int i;

    audience = BattleAudienceBaseGetPtr();
    for (i = 0; i < 2; i++) {
        obj = audience->guestTex[i];
        if (obj) {
            fileFree(obj);
        }
    }

    obj = audience->normalTex;
    if (obj) {
        fileFree(obj);
    }

    if (pouchGetPtr()->starPowersObtained) {
        if (audience->targetAudienceCount < 0.0f) {
            audience->targetAudienceCount = 0.0f;
        }
        if (audience->targetAudienceCount > (f32)audience->maxAudience) {
            audience->targetAudienceCount = (f32)audience->maxAudience;
        }
        pouchSetAudienceNum(audience->targetAudienceCount);
    }
#if VERSION >= VERSION_US
    // US and later free the star-power bonus info allocated in BattleAudience_ApRecoveryBuild
    if (audience->impendingBonuses) {
        BattleFree(audience->impendingBonuses);
        audience->impendingBonuses = NULL;
    }
#endif
}

/**
 * Fills the crowd when a battle starts.
 * The size limit comes from Mario's rank (50/100/150/200, none before the first crystal star),
 * the expected size from pouchGetAudienceNum adjusted by his level and some randomness,
 * and the mix of member kinds from the battle's NpcBattleRandom weights;
 * one in twenty battles swaps a member for the rare kind 12 (Luigi).
 * Battle mode 2 seats only Punies on the left and X-Nauts on the right. Debug flags can force the count or kind.
 */
static void BattleAudienceSettingAudience(void) {
    s32 hasKind = 0;
    s32 target;
    s32 sum;
    s32 lim;
    s32 id;
    s32 n;
    s32 i;
    s32 j;
    s32 k;
    s32 ar;
    s32 kind;
    s32 used;
    s32 list[200];
    s32 perm[12];
    s32 weights[12];
    s32 counts[13];
    BattleDebugCtrl* debug = &_battleWorkPointer->debug;
    BattleAudience* audience = BattleAudienceBaseGetPtr();
    NpcBattleRandom* rec = &_battleWorkPointer->fbatInfo->setup->random;
    PouchData* pouch = pouchGetPtr();
    s32 cnt;
    s32 q;
    f32 delta[65] = {8.0f,11.0f,14.0f,17.0f,20.0f,23.0f,26.0f,29.0f,32.0f,35.0f,38.0f,41.0f,44.0f,47.0f,50.0f,53.0f,56.0f,59.0f,62.0f,65.0f,68.0f,71.0f,74.0f,77.0f,80.0f,83.0f,86.0f,89.0f,92.0f,95.0f,98.0f,101.0f,104.0f,107.0f,110.0f,113.0f,116.0f,119.0f,122.0f,125.0f,128.0f,131.0f,134.0f,137.0f,140.0f,143.0f,146.0f,149.0f,152.0f,155.0f,158.0f,161.0f,164.0f,167.0f,170.0f,173.0f,176.0f,179.0f,182.0f,185.0f,188.0f,191.0f,194.0f,197.0f,200.0f};

    switch (pouch->rank) {
        case 0: audience->maxAudience = 50; break;
        case 1: audience->maxAudience = 100; break;
        case 2: audience->maxAudience = 150; break;
        case 3: audience->maxAudience = 200; break;
    }
    if (pouchGetPtr()->starPowersObtained == 0) {
        audience->maxAudience = 0;
    }
    i = pouch->level - 1;
    lim = 64;
    if (i < 65) lim = i;
    audience->audienceDeltaMultiplier = delta[lim];

    if (debug->flags & BATTLE_DEBUG_AUDIENCE_NUM) {
        audience->targetAudienceCount = (f32)debug->audienceNum;
        if (audience->targetAudienceCount > (f32)audience->maxAudience) {
            audience->targetAudienceCount = (f32)audience->maxAudience;
        }
    } else {
        f32 base = pouchGetAudienceNum();
        audience->targetAudienceCount = (pouchGetAudienceNum() - audience->audienceDeltaMultiplier) * (f32)(-13 + irand(10) + irand(10)) * 0.01f + base;
    }

    switch (rec->mode) {
        case 0:
        case 1:
            audience->addedAudienceCount = 0.0f;
            break;
        case 4:
            audience->addedAudienceCount = 0.5f * audience->targetAudienceCount;
            break;
        case 2:
        case 3:
        case 5:
        case 6:
            audience->addedAudienceCount = (f32)audience->maxAudience - audience->targetAudienceCount;
            break;
    }

    switch (rec->mode) {
        case 2:
            for (q = 0; q < 12; q++) {
                if (q == AUDIENCE_XNAUT || q == AUDIENCE_PUNI) {
                    weights[q] = 1;
                } else {
                    weights[q] = 0;
                }
            }
            hasKind = 0;
                break;
        case 5:
            for (i = 0; i < 12; i++) {
                if (i == AUDIENCE_XNAUT) {
                    weights[i] = 1;
                } else {
                    weights[i] = 0;
                }
            }
            hasKind = 0;
            break;
        case 6:
            for (i = 0; i < 12; i++) {
                if (audience_kind[i].flags & AUDIENCE_KIND_WIDE) {
                    weights[i] = 2;
                } else {
                    weights[i] = 1;
                }
            }
            hasKind = 1;
            break;
        default: {
            s32 w[11] = {20,40,20,5,3,2,1,1,1,1,1};
                sum = 0;
                target = irand(100);
                kind = 12;
                for (i = 1; i < 12; i++) {
                    sum += w[i - 1];
                    if (target < sum) {
                        kind = i;
                        break;
                    }
                }
                for (i = 0; i < 12; i++) perm[i] = i;
                for (j = 0; j < 24; j++) {
                    s32 a = irand(12);
                    s32 b = irand(12);
                    s32 t = perm[a];
                    perm[a] = perm[b];
                    perm[b] = t;
                }
                for (i = 0, k = 0; i <= 11; i++) {
                    id = perm[i];
                    if (k >= kind) {
                        weights[id] = 0;
                    } else if (rec->values[id] != 0) {
                        weights[id] = rec->values[id] * (irand(10) + 1);
                        if (audience_kind[id].flags & AUDIENCE_KIND_WIDE) {
                            weights[id] *= 2;
                        }
                        k++;
                    } else {
                        weights[id] = 0;
                    }
                }
                if (debug->audienceKind != 0) {
                    switch (debug->audienceKind) {
                    case 1: id = AUDIENCE_TOAD; break;
                    case 2: id = AUDIENCE_GOOMBA; break;
                    case 3: id = AUDIENCE_KOOPA; break;
                    case 4: id = AUDIENCE_DULL_BONES; break;
                    case 5: id = AUDIENCE_PUNI; break;
                    case 6: id = AUDIENCE_BOO; break;
                    case 7: id = AUDIENCE_XNAUT; break;
                    case 8: id = AUDIENCE_HAMMER_BRO; break;
                    case 9: id = AUDIENCE_SHY_GUY; break;
                    case 10: id = AUDIENCE_CRAZY_DAYZEE; break;
                    case 11: id = AUDIENCE_BULKY_BOBOMB; break;
                    case 12: id = AUDIENCE_PIRANHA_PLANT; break;
                    }
                    for (i = 0; i < 12; i++) {
                        if (i == id) {
                            weights[i] = 1;
                        } else {
                            weights[i] = 0;
                        }
                    }
                }
                hasKind = 1;
                break;
        }
    }
    memset(counts, 0, sizeof(counts));
    ar = (s32)(audience->targetAudienceCount + audience->addedAudienceCount);
    if (ar < 0) ar = 0;
    if (ar > audience->maxAudience) ar = audience->maxAudience;
    used = 0;
    sum = 0;
    for (i = 0; i < 12; i++) sum += weights[i];
    for (i = 0; i < 12; i++) {
        counts[i] = ar * weights[i] / sum;
        if (audience_kind[i].flags & AUDIENCE_KIND_WIDE) {
            counts[i] /= 2;
            used += counts[i] * 2;
        } else {
            used += counts[i];
        }
    }
    for (i = 0; i < 12; i++) perm[i] = i;
    for (i = 0; i <= 10; i++) {
        for (id = i + 1; id <= 11; id++) {
            if ((weights[i] == weights[id] && irand(2) == 0) || weights[i] < weights[id]) {
                s32 t = perm[i];
                perm[i] = perm[id];
                perm[id] = t;
            }
        }
    }
    i = 0;
    k = 0;
    while (used < ar) {
        s32 id = perm[i];
        if (weights[id] > 0) {
            if (audience_kind[id].flags & AUDIENCE_KIND_WIDE) {
                if (used <= ar - 2) {
                    used += 2;
                    counts[id]++;
                    k = 0;
                } else {
                    k++;
                    if (k >= 12) break;
                }
            } else {
                used++;
                counts[id]++;
            }
        }
        i++;
        i %= 12;
    }
    if (hasKind == 1 && used != 0 && (irand(20) == 0 || (debug->flags & BATTLE_DEBUG_FORCE_LUIGI))) {
        for (i = 0; i < 12; i++) perm[i] = i;
        for (i = 0; i < 24; i++) {
            s32 a = irand(12);
            s32 b = irand(12);
            s32 t = perm[a];
            perm[a] = perm[b];
            perm[b] = t;
        }
        for (i = 0; i < 12; i++) {
            s32 id = perm[i];
            if (counts[id] > 0) {
                counts[id]--;
                counts[AUDIENCE_LUIGI]++;
                break;
            }
        }
    }
    switch (rec->mode) {
        case 2:
            while ((n = BattleAudience_NoUsedHaitiRand()) != -1) {
                if (n % AUDIENCE_SEAT_COLUMNS <= 9) {
                    BattleAudience_Entry(n, AUDIENCE_PUNI, AUDIENCE_STATUS_WAIT);
                } else {
                    BattleAudience_Entry(n, AUDIENCE_XNAUT, AUDIENCE_STATUS_WAIT);
                }
            }
            break;
        default:
            for (kind = 0, cnt = 0; kind <= 12; kind++) {
                for (j = 0; j < counts[kind]; j++) {
                    list[cnt++] = kind;
                }
            }
            for (i = 0; i < 150; i++) {
                s32 a = irand(cnt);
                s32 b = irand(cnt);
                s32 t = list[a];
                list[a] = list[b];
                list[b] = t;
            }
            for (i = 0; i < cnt; i++) {
                s32 idx = BattleAudience_NoUsedFCHaitiRand();
                if (idx == -1) break;
                BattleAudience_Entry(idx, (u8)list[i], 0);
            }
            break;
    }
    memcpy(audience->typeWeights, weights, sizeof(audience->typeWeights));
}

/**
 * Loads guest texture slot id (0 or 1) from battle/audience/<path> for a special member kind;
 * BattleAudience_Init loads Luigi's into slot 0.
 */
static void BattleAudienceGuestTPLRead(s32 id, AudienceMemberType member, const char* path) {
    char full_path[256];

    BattleAudience* audience = BattleAudienceBaseGetPtr();
    if (id < 2) {
        sprintf(full_path, "battle/audience/%s", path);
        audience->guestTex[id] = tplRead(full_path);
        audience->guestType[id] = member;
    }
}

/**
 * Per-frame crowd update: counts the members present on each half, moves members a script is hopping,
 * runs every member's state machine, and during command input may make a random member throw an item (each
 * further throw in a battle is rarer: 1/1800, 1/6000, 1/30000 per frame).
 */
static void BattleAudienceCtrlProcess(void) {
    s32 chance;
    BattleWork* wp = _battleWorkPointer;
    BattleDebugCtrl* debug = &wp->debug;
    BattleAudience* audience = BattleAudienceBaseGetPtr();
    BattleAudienceMember* member;
    s32 i, j, index;
    u8 status;
    s32 order[200];

    audience->currentAudienceIntCountLeft = 0;
    audience->currentAudienceIntCountRight = 0;
    audience->currentAudienceIntCount = 0;

    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) {
            index = (i * 20) + j + 10;
            if ((u8)BattleAudience_GetExist(index) == TRUE) {
                status = BattleAudienceGetPtr(index)->status;
                if (status != AUDIENCE_STATUS_ESCAPE && status != AUDIENCE_STATUS_ENTER) {
                    audience->currentAudienceIntCountRight++;
                }
            }
        }
    }

    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) {
            index = (i * 20) + j;
            if ((u8)BattleAudience_GetExist(index) == TRUE) {
                status = BattleAudienceGetPtr(index)->status;
                if (status != AUDIENCE_STATUS_ESCAPE && status != AUDIENCE_STATUS_ENTER) {
                    audience->currentAudienceIntCountLeft++;
                }
            }
        }
    }

    audience->currentAudienceIntCount = audience->currentAudienceIntCountRight + audience->currentAudienceIntCountLeft;

    i = 0;
    member = BattleAudienceGetPtr(0);
    for (; i < AUDIENCE_MEMBER_MAX; i++, member++) {
        if (member->flags & AUDIENCE_MEMBER_FLAG_NO_SYS_CTRL) {
            member->hopY += member->hopSpeed;
            member->hopSpeed += member->hopAccel;
            if (member->hopY <= 0.0f) {
                member->hopY = 0.0f;
                member->hopSpeed = 0.0f;
                member->hopAccel = 0.0f;
            }
        } else if ((u8)BattleAudience_GetExist(i)) {
            switch (member->type) {
                case 0: BattleAudienceCtrlProcessKinopio(i); break;
                case 1: BattleAudienceCtrlProcessKinopio(i); break;
                case 2: BattleAudienceCtrlProcessKinopio(i); break;
                case 3: BattleAudienceCtrlProcessKinopio(i); break;
                case 4: BattleAudienceCtrlProcessKinopio(i); break;
                case 5: BattleAudienceCtrlProcessKinopio(i); break;
                case 6: BattleAudienceCtrlProcessKinopio(i); break;
                case 7: BattleAudienceCtrlProcessKinopio(i); break;
                case 8: BattleAudienceCtrlProcessKinopio(i); break;
                case 9: BattleAudienceCtrlProcessKinopio(i); break;
                case 10: BattleAudienceCtrlProcessKinopio(i); break;
                case 11: BattleAudienceCtrlProcessKinopio(i); break;
                case 12: BattleAudienceCtrlProcessKinopio(i); break;
            }
        }
    }

    if (debug->flags & BATTLE_DEBUG_ITEM_THROW) {
        audience->itemThrowMax = 10000;
    }
    if (wp->flags & BATTLE_FLAG_COMMAND_INPUT) {
        if (audience->itemThrowCount < audience->itemThrowMax) {
            switch (audience->itemThrowCount) {
                case 0: chance = 1800; break;
                case 1: chance = 6000; break;
                case 2: chance = 30000; break;
            }
            if (debug->flags & BATTLE_DEBUG_ITEM_THROW) {
                chance = 60;
            }
            if (irand(chance) == 0) {
                for (i = 0; i < AUDIENCE_MEMBER_MAX; i++) {
                    order[i] = i;
                }
                for (i = 0; i < 1000; i++) {
                    s32 a = irand(200);
                    s32 b = irand(200);
                    s32 t = order[a];
                    order[a] = order[b];
                    order[b] = t;
                }
                for (i = 0; i < AUDIENCE_MEMBER_MAX; i++) {
                    if (BattleAudienceItemOn(order[i], ITEM_NULL, 1) == TRUE) {
                        audience->itemThrowCount++;
                        return;
                    }
                }
            }
        }
    }
}

/**
 * Makes member index hold up an item to throw (status 7),
 * in the first free slot among the first numItems item slots (1 allows one throw at a time, 100 any number);
 * itemId 0 lets the member's kind table pick the item.
 * Refused while the audience is frozen, for kinds that never throw,
 * for a missing or busy member (holding, throwing, asleep, arriving, leaving, in its shell, fuse lit, hit)
 * and outside the centre seats. Outside the early demos, also refused in the side columns (outside 4-15) and,
 * for a consumable item, before the first-item tutorial has been seen. TRUE when the member took the item.
 * Called by the phase reactions, the random throws in BattleAudienceCtrlProcess and Frankly's lecture.
 */
BOOL BattleAudienceItemOn(s32 index, u16 itemId, s32 numItems) {
    BattleAudience* audience = BattleAudienceBaseGetPtr();
    BattleAudienceMember* member = BattleAudienceGetPtr(index);
    BattleAudienceItem* item;
    s32 st;
    u8 ust;

#if VERSION <= VERSION_DEMO_JP
    if (audience->flags & AUDIENCE_FLAG_EVENT_CTRL) {
        return FALSE;
    }
    if (audience_kind[member->type].itemTable->type == 0) {
        return FALSE;
    }
    if (!(u8)BattleAudience_GetExist(index))
    return FALSE;
#else
    if (audience->flags & AUDIENCE_FLAG_EVENT_CTRL) return FALSE;
    if (audience_kind[member->type].itemTable->type == 0) return FALSE;
    if (itemId >= ITEM_MIN && itemId < ITEM_MAX && swGet(GSWF_ID(GSWF_ITEM_TUTORIAL_SEEN)) == 0) return FALSE;
    st = index % AUDIENCE_SEAT_COLUMNS;
    if (st < 4 || st > 15) return FALSE;
    if (!(u8)BattleAudience_GetExist(index)) return FALSE;
#endif
    ust = member->status;
    if (ust == AUDIENCE_STATUS_ITEM_ON) return FALSE;
    if (ust == AUDIENCE_STATUS_ITEM_THROW) return FALSE;
    if (ust == AUDIENCE_STATUS_SLEEP) return FALSE;
    if (ust == AUDIENCE_STATUS_ESCAPE) return FALSE;
    if (ust == AUDIENCE_STATUS_ENTER) return FALSE;
    if (ust == AUDIENCE_STATUS_IGNITE) return FALSE;
    if (ust == AUDIENCE_STATUS_SHELL) return FALSE;
    if (ust == 21) return FALSE;
    if (ust == AUDIENCE_STATUS_DAMAGE) return FALSE;
    if (member->flags & AUDIENCE_MEMBER_FLAG_FRONT_CENTER) {
        s32 i = 0;
        item = BattleAudienceItemGetPtr(0);
        for (; i < numItems; i++, item++) {
            if (!(item->flags & AUDIENCE_ITEM_FLAG_ACTIVE)) {
                item->flags |= AUDIENCE_ITEM_FLAG_ACTIVE;
                item->state = 0;
                item->audienceId = index;
                item->itemType = itemId;
                BattleAudience_ChangeStatus(index, AUDIENCE_STATUS_ITEM_ON);
                member->itemId = i;
                return TRUE;
            }
        }
    }
    return FALSE;
}

/**
 * Rolls how many random item throws the crowd may make (none 80%, one 10%, two 5%, three 5%)
 * and resets the throw and hit counters; at battle start and each enemy phase.
 */
static void BattleAudienceSetThrowItemMax(void) {
    BattleAudience* audience = BattleAudienceBaseGetPtr();
    s32 r = irand(100);

    if (r < 80) {
        audience->itemThrowMax = 0;
    } else if (r < 90) {
        audience->itemThrowMax = 1;
    } else if (r < 95) {
        audience->itemThrowMax = 2;
    } else if (r < 100) {
        audience->itemThrowMax = 3;
    }
    audience->itemThrowCount = 0;
    audience->presentHitCount = 0;
}

/**
 * Picks a random targetable unit on either side with HP left,
 * skipping the unused kinds UNIT_UNUSED_TREE and UNIT_UNUSED_SWITCH; NULL if there is none.
 * The Boo's phase event uses it to choose whom to turn invisible.
 */
BattleUnit* BattleAudienceDetectTargetAll(void) {
    s32 i;
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    s32 count;
    BattleUnitPart* part;
    s32 ids[64];

    BattleAudienceBaseGetPtr();
    count = 0;
    for (i = 0; i < BATTLE_UNIT_MAX; i++) {
        unit = wp->units[i];
        if (unit != NULL) {
            part = BattleGetUnitPartsPtr(i, BtlUnit_GetBodyPartsId(unit));
            if (!(part->attributes & kNotTargetable) && unit->currentHp > 0 &&
                (unit->currentKind < UNIT_UNUSED_TREE || unit->currentKind >= UNIT_UNUSED_TESTNPC)) {
                ids[count++] = i;
            }
        }
    }
    if (count == 0) {
        return NULL;
    }
    i = irand(count);
    return BattleGetUnitPtr(wp, ids[i]);
}

/**
 * Picks a random targetable unit on Mario's side (alliance 0) with HP left, or NULL;
 * the target of a thrown audience item (BattleAudienceItemCtrlProcess).
 */
BattleUnit* BattleAudienceDetectTargetPlayer(void) {
    s32 i;
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    s32 count;
    BattleUnitPart* part;
    s32 ids[64];

    BattleAudienceBaseGetPtr();
    count = 0;
    for (i = 0; i < BATTLE_UNIT_MAX; i++) {
        unit = wp->units[i];
        if (unit != NULL) {
            part = BattleGetUnitPartsPtr(i, BtlUnit_GetBodyPartsId(unit));
            if (unit->alliance == ALLIANCE_PLAYER && unit->currentHp > 0 && !(part->attributes & kNotTargetable)) {
                ids[count++] = i;
            }
        }
    }
    if (count == 0) {
        return NULL;
    }
    i = irand(count);
    return BattleGetUnitPtr(wp, ids[i]);
}

/**
 * Per-frame update of thrown audience items: picks the item from the thrower's kind table,
 * flies it in an arc to Mario or his partner, then either hands it over as a gift (if the pouch has room) or,
 * for junk (can, rock, bone, hammer), loads the matching weapon so it does damage,
 * and records the item and the receiving unit (BattleAudience_SetPresent*).
 * Items whose thrower falls asleep, explodes, leaves or is gone are removed.
 */
static void BattleAudienceItemCtrlProcess(void) {
    Vec hv;
    s32 i;
    BattleWork* wp = _battleWorkPointer;
    BattleAudience* base = BattleAudienceBaseGetPtr();
    BattleAudienceItem* it;
    BattleAudienceMember* member;
    BattleUnit* unit;
    BattleAudienceKind* kd;
    s32 j;
    f32 t;
    BattleUnitPart* pp;
    Vec v;

    i = 0;
    it = BattleAudienceItemGetPtr(0);
    for (; i < 100; i++, it++) {
        if (!(it->flags & AUDIENCE_ITEM_FLAG_ACTIVE)) {
            continue;
        }
        member = BattleAudienceGetPtr(it->audienceId);
        kd = &audience_kind[member->type];
        if (member->status == AUDIENCE_STATUS_SLEEP || member->status == AUDIENCE_STATUS_FIRE || member->status == 21 || member->status == AUDIENCE_STATUS_ESCAPE ||
            !(u8)BattleAudience_GetExist(it->audienceId)) {
            goto kill;
        }
        switch (it->state) {
            case 0:
                it->flags |= AUDIENCE_ITEM_FLAG_VISIBLE;
                if (it->itemType == ITEM_NULL) {
                    BattleAudienceItemKind* tbl = kd->itemTable;
                    s32 cnt = 0;
                    s32 sum;
                    s32 acc;
                    s32 r;
                    j = 0;
                    sum = 0;
                    for (;;) {
                        if (tbl[j].type == ITEM_NULL) {
                            break;
                        }
                        sum += tbl[j].chance;
                        cnt++;
                        j++;
                    }
                    if (cnt == 0) {
                        goto done;
                    }
                    r = irand(sum);
                    j = 0;
                    acc = 0;
                    for (;; j++) {
                        if (r >= acc && r < acc + tbl[j].chance) {
                            break;
                        }
                        acc += tbl[j].chance;
                    }
                    it->itemType = tbl[j].type;
                }
                it->timer = i + 180;
                it->scale = 1.0f;
                it->rotZ = 0.0f;
                v = (Vec){0.0f, 0.0f, 0.0f};
                v.x = member->position.x + kd->itemOffsetX;
                v.y = (member->position.y + kd->itemOffsetY) - 10.0f;
                v.z = 1.0f + member->position.z;
                it->position = v;
                it->state++;

                /* fallthrough */
            case 1:
                if (it->timer <= 0) {
                    BattleUnit* t;
                    f32 dx, n;
                    for (j = 0; j < AUDIENCE_ITEM_MAX; j++) {
                        if (j != i && (BattleAudienceItemGetPtr(j)->flags & AUDIENCE_ITEM_FLAG_ACTIVE) &&
                            BattleAudienceItemGetPtr(j)->state == 1) {
                            it->mode = 2;
                            break;
                        }
                    }
                    if (j == 100) {
                        it->mode = 1;
                    }
                    t = BattleAudienceDetectTargetPlayer();
                    if (t == NULL) {
                        goto done;
                    }
                    it->target = t;
                    pp = BtlUnit_GetPartsPtr(t, BtlUnit_GetBodyPartsId(t));
                    BtlUnit_GetHitPos(t, pp, &hv.x, &hv.y, &hv.z);
                    dx = hv.x - it->position.x;
                    n = 30.0f + (f32)__fabs(dx) / 15.0f;
                    it->velocity.x = dx / n;
                    it->accel.x = 0.0f;
                    it->velocity.y = 7.0f;
                    it->accel.y = 2.0f * ((hv.y - it->position.y) - it->velocity.y * n) / (n * n);
                    it->velocity.z = (hv.z - it->position.z) / n;
                    it->accel.z = 0.0f;
                    it->timer = (s32)n;
                    it->state++;
                } else {
                    s32 k = 180 - it->timer;
                    f32 ox;
                    if (k >= 5) {
                        k = 5;
                    }
                    {
                        Vec v = {0.0f, 0.0f, 0.0f};
                        v.x = member->drawPosX + kd->itemOffsetX;
                        ox = intplGetValue(EASING_LINEAR, -10.0f, 0.0f, k, 5);

                        t = member->drawPosY + kd->itemOffsetY;
                        v.y = t + ox;
                        v.z = 1.0f + member->drawPosZ;
                        it->position = v;
                    }
                }
                break;
            case 2:
                if (!(wp->flags & BATTLE_FLAG_COMMAND_INPUT)) {
                    it->mode = 0;
                }
                if (it->timer == 20) {
                    BattleAudience_ChangeStatus(it->audienceId, AUDIENCE_STATUS_WAIT);
                }
                if (it->timer == 0) {
                    it->state++;
                }
                if (it->timer > 0) {
                    it->position.x += it->velocity.x;
                    it->position.y += it->velocity.y;
                    it->position.z += it->velocity.z;
                    it->velocity.x += it->accel.x;
                    it->velocity.y += it->accel.y;
                    it->velocity.z += it->accel.z;
                    if (it->itemType == ITEM_AUDIENCE_HAMMER) {
                        it->rotZ += 18.0f;
                    }
                    it->timer--;
                }
                break;
                case 3: {
                    s8 a, b;
                    if (!(wp->flags & BATTLE_FLAG_COMMAND_INPUT)) {
                        it->mode = 0;
                    }
                    unit = it->target;
                    BtlUnit_GetStatus(unit, STATUS_INVISIBLE, &a, &b);
                    if (a >= 1 || BtlUnit_CheckShadowGuard(unit)) {
                        it->mode = 0;
                    }
                    if (it->flags & AUDIENCE_ITEM_FLAG_HARMLESS_UNUSED) {
                        it->mode = 0;
                    }
                    if (it->itemType >= ITEM_THUNDER_BOLT && it->itemType < ITEM_MAX && pouchGetEmptyHaveItemCnt() <= 0) {
                        it->mode = 0;
                    }
                    if (it->mode != 0) {
                        base->presentHitCount++;
                        if (base->presentHitCount > 3) {
                            base->presentHitCount = 3;
                        }
                        if (it->mode == 2) {
                            it->state = 4;
                            goto s4;
                        }
                        if (it->itemType >= ITEM_MIN && it->itemType < ITEM_MAX) {
                            BattleAudience_SetPresentItemType(PRESENT_ITEM_GET);
                        } else if (it->itemType >= ITEM_AUDIENCE_CAN && it->itemType < BADGE_MIN) {
                            BattleAudience_SetPresentItemType(PRESENT_ITEM_DAMAGE);
                        }
                        BattleAudience_SetPresentTargetUnitId(BtlUnit_GetUnitId(unit));
                        switch (it->itemType) {
                        case ITEM_AUDIENCE_CAN:
                            memcpy(&base->presentWeapon, &weapon_data_akikan, sizeof(BattleWeapon));
                            base->presentWeapon.hpDamageWork[0] = base->presentHitCount + base->presentWeapon.hpDamageWork[0] - 1;
                            break;
                        case ITEM_AUDIENCE_ROCK:
                            memcpy(&base->presentWeapon, &weapon_data_brick, sizeof(BattleWeapon));
                            base->presentWeapon.hpDamageWork[0] = base->presentHitCount + base->presentWeapon.hpDamageWork[0] - 1;
                            break;
                        case ITEM_AUDIENCE_BONE:
                            memcpy(&base->presentWeapon, &weapon_data_bone, sizeof(BattleWeapon));
                            base->presentWeapon.hpDamageWork[0] = base->presentHitCount + base->presentWeapon.hpDamageWork[0] - 1;
                            break;
                        case ITEM_AUDIENCE_HAMMER:
                            memcpy(&base->presentWeapon, &weapon_data_hammer, sizeof(BattleWeapon));
                            base->presentWeapon.hpDamageWork[0] = base->presentHitCount + base->presentWeapon.hpDamageWork[0] - 1;
                            break;
                        }
                        BattleAudience_SetPresentItemNo(it->itemType);
                        switch (BattleAudience_GetPresentItemType()) {
                        case PRESENT_ITEM_GET:
                            it->flags &= ~AUDIENCE_ITEM_FLAG_VISIBLE;
                            it->timer = 5;
                            it->state = 7;
                            break;
                        case PRESENT_ITEM_DAMAGE:
                            BattleAudienceSoundStop(AUD_SOUND_ITEM_THROW_L);
                            BattleAudienceSoundStop(AUD_SOUND_ITEM_THROW_R);
                            if (it->velocity.x < 0.0f) {
                                it->velocity.x *= -0.2f;
                            }
                            it->velocity.y = 1.5f;
                            it->velocity.z = 0.0f;
                            it->state = 4;
                            break;
                        }
                        base->presentHitCount = 0;
                    } else {
                        it->state = 4;
                    }
                    if (it->state != 4) {
                        break;
                    }
                }

                /* fallthrough */
            case 4:
                s4: {
                    it->position.x += it->velocity.x;
                    it->position.y += it->velocity.y;
                    it->position.z += it->velocity.z;
                    it->velocity.x += it->accel.x;
                    it->velocity.y += it->accel.y;
                    it->velocity.z += it->accel.z;
                    if (it->position.y <= 0.0f) {
                        BattleAudienceSoundStop(AUD_SOUND_ITEM_THROW_L);
                        BattleAudienceSoundStop(AUD_SOUND_ITEM_THROW_R);
                        it->state = 5;
                        it->position.y = 0.1f;
                        it->velocity.y = 1.0f;
                        it->velocity.z = 0.0f;
                    }
                    break;
                }
            case 5:
                it->position.x += it->velocity.x;
                it->position.y += it->velocity.y;
                it->position.z += it->velocity.z;
                it->velocity.x += it->accel.x;
                it->velocity.y += it->accel.y;
                it->velocity.z += it->accel.z;
                if (it->position.y <= 0.0f) {
                    it->timer = 60;
                    it->state = 6;
                    it->position.y = 0.0f;
                }
                break;
            case 6:
                if (it->timer <= 0) {
                    goto done;
                }
                if (it->timer < 30) {
                    if ((it->timer & 3) <= 1) {
                        it->flags |= AUDIENCE_ITEM_FLAG_VISIBLE;
                    } else {
                        it->flags &= ~AUDIENCE_ITEM_FLAG_VISIBLE;
                    }
                }
                it->timer--;
                break;
            case 7:
                it->timer--;
                if (it->timer > 0) {
                    break;
                }
                done:
                    BattleAudienceSoundStop(AUD_SOUND_ITEM_THROW_L);
                    BattleAudienceSoundStop(AUD_SOUND_ITEM_THROW_R);
                    if (it->audienceId != -1) {
                        BattleAudienceGetPtr(it->audienceId);
                        BattleAudience_ChangeStatus(it->audienceId, AUDIENCE_STATUS_WAIT);
                    }
                kill:
                    it->flags = 0;
                    it->timer = 0;
                    it->state = 0;
                    if (it->audienceId != -1) {
                        // FAKE: raw store to offset 0x1C (BattleAudienceMember.itemId); the field store changes the object
                        u8* ap = (u8*)BattleAudienceGetPtr(it->audienceId);
                        *(s16*)(ap + 0x1c) = -1;
                    }
                    break;
        }
    }
}

/**
 * Per-frame update of the Star Power sparkles the crowd sends up after an appeal or good move:
 * spawns one per pending point, flies each to the Star Power gauge (statusGetApPos),
 * keeps the sparkle sound volume tied to how many are in flight,
 * and turns the gauge recovery glow off once all have arrived.
 */
static void BattleAudienceApSrcCtrlProcess(void) {
    BattleAudience* base = BattleAudienceBaseGetPtr();
    s32 cnt;
    s32 i;
    s32 k;
    f32 dy;
    f32 dx;

    if (!(base->flags & AUDIENCE_FLAG_SP_GAINED)) {
        return;
    }
    cnt = 0;
    for (i = 0; i < AUDIENCE_ITEM_MAX; i++) {
        if (base->starpower[i].flags & AUDIENCE_STAR_FLAG_ACTIVE) {
            cnt++;
        }
    }
    if (psndSFXChk(base->apStarSfxId) != -1) {
        psndSFX_vol(base->apStarSfxId, 63.0f * cnt / 100.0f + 64.0f);
    }
    if (base->starPowerGain > 0) {
        if (BattleAudienceApSrcEntry() == TRUE) {
            base->starPowerGain--;
        }
        if (cnt == 0 && psndSFXChk(base->apStarSfxId) == -1) {
            base->apStarSfxId = psndSFXOn("SFX_BTL_SYS_AP_STAR1");
        }
    } else {
        for (k = 0, i = 0; i < AUDIENCE_ITEM_MAX; i++) {
            if (base->starpower[i].flags & AUDIENCE_STAR_FLAG_ACTIVE) {
                break;
            }
            k++;
        }
        if (k == 100) {
            base->flags &= ~AUDIENCE_FLAG_SP_GAINED;
            psndSFXOff(base->apStarSfxId);
            base->apStarSfxId = 0;
            base->starPowerGain = 0;
            BattleStatusWindowAPRecoveryOff();
            return;
        }
    }
    for (i = 0; i < AUDIENCE_ITEM_MAX; i++) {
        BattleAudienceStar* s = &base->starpower[i];
        pouchGetPtr();
        if (!(s->flags & AUDIENCE_STAR_FLAG_ACTIVE)) {
            continue;
        }
        switch (s->state) {
            case 0:
                s->state = 5;
                s->targetPos = s->position;
                s->startPos = s->targetPos;
                s->targetPos.x += 50.0f * rand() / 32767.0f - 25.0f;
                s->targetPos.y += 25.0f;
                s->timer = 0;
                s->duration = 30;

                /* fallthrough */
            case 5:
                s->timer++;
                s->position.x = intplGetValue(EASING_CUBIC_OUT, s->startPos.x, s->targetPos.x, s->timer, s->duration);
                s->position.y = intplGetValue(EASING_CUBIC_OUT, s->startPos.y, s->targetPos.y, s->timer, s->duration);
                if (s->timer >= s->duration - 20) {
                    s->scale.y = intplGetValue(EASING_QUADRATIC_OUT, 0.9f, 0.8f, s->timer - (s->duration - 20), 10);
                }
                if (s->timer >= s->duration) {
                    s->state = 10;
                    s->timer = 0;
                    s->duration = 15 + irand(10);
                    s->scale = (Vec){0.9f, 0.9f, 0.9f};
                }
                break;
            case 10:
                s->timer++;
                if (s->timer >= s->duration) {
                    s->state = 15;
                    s->timer = 0;
                    s->duration = 20 + irand(20);
                    s->startPos = s->position;
                    s->targetPos = statusGetApPos();
                    dx = s->startPos.x - s->targetPos.x;
                    dy = s->startPos.y - s->targetPos.y;
                    s->radius = s->startRadius = sqrt(dx * dx + dy * dy);
                    s->angle = asin(dy / s->radius);
                    s->angleSpeed = 0.05f + 0.2f * rand() / 32767.0f;
                }
                break;
            case 15:
                s->timer++;
                s->radius = intplGetValue(EASING_QUADRATIC_OUT, s->startRadius, 0.0f, s->timer, s->duration);
                s->angle += s->angleSpeed;
                if (s->angle < 0.0f) {
                    s->angle += 6.283184f;
                }
                s->position.x = s->radius * (f32)cos(s->angle) + s->targetPos.x;
                s->position.y = s->radius * (f32)sin(s->angle) + s->targetPos.y;
                if (s->timer >= s->duration) {
                    s->state = 20;
                    s->timer = 0;
                    s->duration = 15;
                    s->position = statusGetApPos();
                }
                break;
            case 20:
                s->timer++;
                s->alpha = intplGetValue(EASING_QUADRATIC_OUT, 128.0f, 0.0f, s->timer, s->duration);
                if (s->timer >= s->duration) {
                    s->flags = 0;
                    pouchAddAP(1);
                }
                break;
        }
    }
}

/**
 * Spawns one Star Power sparkle above a random member present, converted to screen space,
 * heading for the Star Power gauge; each sparkle adds 1 AP when it arrives.
 * FALSE when all 100 sparkle slots are busy. TRUE otherwise, also when nobody is in the stands,
 * so that point of gain is lost. Called each frame by BattleAudienceApSrcCtrlProcess while gain is pending.
 */
static BOOL BattleAudienceApSrcEntry(void) {
    BattleAudienceStar* star;
    CameraEntry* cam;
    s32 list[200];
    s32 count;
    s32 i;
    BattleAudience* base = BattleAudienceBaseGetPtr();
    BattleAudienceMember* m;
    star = base->starpower;
    for (i = 0; i < 100; i++, star++) {
        if (!(star->flags & AUDIENCE_STAR_FLAG_ACTIVE)) {
            break;
        }
    }
    if (i == 100) {
        return FALSE;
    }
    count = 0;
    i = 0;
    BattleAudienceGetPtr(0);
    for (; i < AUDIENCE_MEMBER_MAX; i++) {
        if ((u8)BattleAudience_GetExist(i) == 1) {
            list[count++] = i;
        }
    }
    if (count == 0) {
        return TRUE;
    }
    star->flags = AUDIENCE_STAR_FLAG_ACTIVE;
    star->state = 0;
    star->timer = 0;
    star->scale = (Vec){0.75f, 0.75f, 0.75f};
    star->alpha = 0x80;
    m = BattleAudienceGetPtr(list[irand(count)]);
    star->position.x = m->position.x;
    star->position.y = 20.0f + m->position.y;
    star->position.z = 1.0f + m->position.z;
    cam = camGetPtr(CAMERA_3D);
    PSMTXMultVec(cam->view, &star->position, &star->position);
    PSMTX44MultVec(cam->projection, &star->position, &star->position);
    star->position.x *= 304.0f;
    star->position.y *= 240.0f;
    star->position.z = 0.0f;
    star->reserved_14 = 40.0f * rand() / 32767.0f - 20.0f;
    star->reserved_18 = 10.0f * rand() / 32767.0f;
    star->reserved_1C = 0.0f;
    star->startPos = star->position;
    star->targetPos = (Vec){-96.0f, 106.0f, 0.0f};
    return TRUE;
}

/**
 * Per-frame update of the audience-count window: the number shown moves toward the real count by a tenth of
 * the gap each frame (at least 0.1), and the window slides in from the left over 15 frames
 * while active and back out when not.
 */
static void BattleAudienceWinCtrlProcess(void) {
    BattleAudience* base = BattleAudienceBaseGetPtr();
    BattleAudienceWindow* win = BattleAudienceWinGetPtr();
    f32 diff = (f32)base->currentAudienceIntCount - win->count;
    f32 step;

    if (diff != 0.0f) {
        if (diff > 0.0f) {
            step = (0.1f * diff < 0.1f) ? 0.1f : 0.1f * diff;
            win->count += step;
            if ((f32)base->currentAudienceIntCount - win->count < 0.0f) {
                win->count = (f32)base->currentAudienceIntCount;
            }
        } else {
            step = (0.1f * diff > -0.1f) ? 0.1f : -0.1f * diff;
            win->count -= step;
            if ((f32)base->currentAudienceIntCount - win->count > 0.0f) {
                win->count = (f32)base->currentAudienceIntCount;
            }
        }
    }

    if (win->active) {
        win->slideTimer++;
        if (win->slideTimer > 15) {
            win->slideTimer = 15;
        }
    } else {
        win->slideTimer--;
        if (win->slideTimer < 0) {
            win->slideTimer = 0;
        }
    }
    win->x = intplGetValue(EASING_QUADRATIC_OUT, -500.0f, -280.0f, win->slideTimer, 15);
    win->y = -160.0f;
    win->z = 0.0f;
}

/**
 * Sets up GX for the audience's textured quads: no culling, position,
 * colour and texture coordinates sent directly, one TEV stage that multiplies the texture by the vertex
 * colour and alpha. Used before drawing thrown items and Star Power sparkles.
 */
static void BattleAudienceGXInit(void) {
    BattleAudienceBaseGetPtr();
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_VTX, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetNumTevStages(1);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_RASC, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_RASA, GX_CA_TEXA, GX_CA_ZERO);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
}

/**
 * Steps each visible member's animation script once per frame (from BattleAudience_Disp).
 * Script ops: -2 show texture n for m frames, -3 restart, -4 label, -5 jump to a label,
 * -6 and -7 loop start and end (nested up to 3 deep), -8 wait, -1 end.
 */
static void BattleAudienceAnimProcess(void) {
    BattleAudienceMember* m;
    s32 i;
    s32* pc;
    s32 n;
    s32 target;

    BattleAudienceBaseGetPtr();
    i = 0;
    m = BattleAudienceGetPtr(0);
    for (; i < AUDIENCE_MEMBER_MAX; i++, m++) {
        if (!(m->flags & AUDIENCE_MEMBER_FLAG_USED)) continue;
        if (!(m->flags & AUDIENCE_MEMBER_FLAG_DISP)) continue;
        pc = m->animPc;
        if (m->animWait > 0) {
            m->animWait--;
            continue;
        }
        switch (pc[0]) {
            case AUD_OP_FRAME:
                m->texId = pc[1];
                m->animWait = pc[2];
                m->animPc += 3;
                break;
            case AUD_OP_RESTART:
                m->animPc = m->animStart;
                break;
            case AUD_OP_LABEL:
                m->animPc += 2;
                break;
            case AUD_OP_GOTO:
                target = pc[1];
                m->animPc = m->animStart;
                while (!(m->animPc[0] == AUD_OP_LABEL && m->animPc[1] == target)) {
                    m->animPc++;
                }
                break;
            case AUD_OP_REPEAT:
                m->animLoop[m->animDepth] = pc[1];
                m->animDepth++;
                m->animPc += 2;
                break;
            case AUD_OP_END_REPEAT:
                m->animLoop[m->animDepth - 1]--;
                if (m->animLoop[m->animDepth - 1] <= 0) {
                    m->animDepth--;
                    m->animPc += 1;
                } else {
                    n = 1;
                    do {
                        m->animPc--;
                        if (m->animPc[0] == AUD_OP_REPEAT) n--;
                        if (m->animPc[0] == AUD_OP_END_REPEAT) n++;
                    } while (n > 0);
                    m->animPc += 2;
                }
                break;
            case AUD_OP_WAIT:
                m->animWait = pc[1];
                m->animPc += 2;
                break;
            case AUD_OP_END:
                break;
        }
    }
}

// A macro, not an inline function: as a function GXPosition3f32 changes BattleAudienceDispAudience (AUD_POS / AUD_POSS differ only by temporaries).
#define AUD_POS(x, y, z)   \
    do {                   \
        f32 _x = (x);      \
        f32 _y = (y);      \
        f32 _z = (z);      \
        GXWGFifo.f32 = _x; \
        GXWGFifo.f32 = _y; \
        GXWGFifo.f32 = _z; \
    } while (0)
#define AUD_POSS(x, y, z)   \
    do {                    \
        GXWGFifo.f32 = (x); \
        GXWGFifo.f32 = (y); \
        GXWGFifo.f32 = (z); \
    } while (0)
// A macro, not an inline function: as a function GXTexCoord2f32 changes BattleAudienceDispAudience.
#define AUD_TEX(s, t)      \
    do {                   \
        f32 _s = (s);      \
        f32 _t = (t);      \
        GXWGFifo.f32 = _s; \
        GXWGFifo.f32 = _t; \
    } while (0)
/**
 * Draws the audience: each present member as a camera-facing textured quad in its seat,
 * opaque members (colorA 0xFF) and translucent ones in separate passes (alpha-test and translucent render modes),
 * with flipping, hop offsets and fade colours.
 */
static void BattleAudienceDispAudience(CameraId camId, void* param) {
    f32 sx, cx, sy, cy, sz, cz;
    f32 nhw;
    f32 ox, oy, oz;
    f32 a0, a1, a2, t0, t1;
    BattleAudience* audience;
    CameraEntry* cam;
    s32 i;
    BattleAudienceMember* m;
    s32 flip;
    s32 k;
    s32 off;
    GXTexObj tex;
    u8 cr, cg, cb, ca;
    Mtx rot;
    Mtx scl;
    Mtx fin;
    BOOL front;
    s32* cur;
    f32 hw;

    audience = BattleAudienceBaseGetPtr();
    cam = camGetPtr(CAMERA_3D);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
#if VERSION <= VERSION_DEMO_JP
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
#endif
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
#if VERSION <= VERSION_DEMO_JP
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
#endif
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSetNumChans(1);
#if VERSION <= VERSION_DEMO_JP
    GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_VTX, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
#else
    GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_VTX, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
#endif
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetNumTevStages(1);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ONE, GX_CC_TEXC, GX_CC_RASC);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_RASA, GX_CA_TEXA, GX_CA_ZERO);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    i = 0;
    m = BattleAudienceGetPtr(0);
    hw = 20.0f;
    nhw = -hw;
    for (; i < AUDIENCE_MEMBER_MAX; i++, m++) {
        if (dispGetCurWork()->renderMode != DISP_MODE_XLU) {
            if (m->colorA != 0xFF) continue;
        } else {
            if (m->colorA == 0xFF) continue;
        }
        if (!(m->flags & AUDIENCE_MEMBER_FLAG_USED) || !(m->flags & AUDIENCE_MEMBER_FLAG_DISP)) continue;
        if (m->type <= 11) {
            TEXGetGXTexObjFromPalette((TEXPalette*)*audience->normalTex->data, &tex, m->texId);
            GXLoadTexObj(&tex, GX_TEXMAP0);
            m->scale.x = m->baseScale.x * ((audience_kind[m->type].texScaleX * (f32)GXGetTexObjWidth(&tex)) / 64.0f);
            m->scale.y = m->baseScale.y * ((audience_kind[m->type].texScaleY * (f32)GXGetTexObjHeight(&tex)) / 64.0f);
        } else {
            for (k = 0; k < 2; k++) {
                if (m->type == audience->guestType[k]) {
                    TEXGetGXTexObjFromPalette((TEXPalette*)*audience->guestTex[k]->data, &tex, m->texId);
                    GXLoadTexObj(&tex, GX_TEXMAP0);
                    m->scale.x = m->baseScale.x * ((audience_kind[m->type].texScaleX * (f32)GXGetTexObjWidth(&tex)) / 64.0f);
                    m->scale.y = m->baseScale.y * ((audience_kind[m->type].texScaleY * (f32)GXGetTexObjHeight(&tex)) / 64.0f);
                    break;
                }
            }
        }
        ox = m->rotationOffset.x * m->scale.x;
        oy = m->rotationOffset.y * m->scale.y;
        oz = m->rotationOffset.z * m->scale.z;
        PSMTXScale(scl, m->scale.x, m->scale.y, m->scale.z);

        {
            f32 degToRad = 0.017453289f;
            a0 = m->rotation.x * degToRad;
            a1 = m->rotation.y * degToRad;
            a2 = m->rotation.z * degToRad;
        }
        sx = sin(a0);
        cx = cos(a0);
        sy = sin(a1);
        cy = cos(a1);
        sz = sin(a2);
        cz = cos(a2);
        t0 = sx * sy;
        t1 = sx * cy;
        rot[0][0] = cy * cz - t0 * sz;
        rot[0][1] = -cy * sz - t0 * cz;
        rot[0][2] = -cx * sy;
        rot[0][3] = 0.0f;
        rot[1][3] = 0.0f;
        rot[2][3] = 0.0f;
        rot[1][0] = cx * sz;
        rot[1][1] = cx * cz;
        rot[1][2] = -sx;
        rot[2][0] = sy * cz + t1 * sz;
        rot[2][1] = -sy * sz + t1 * cz;
        rot[2][2] = cx * cy;
        m->facingDir = (Vec){0.0f, 0.0f, -1.0f};
        PSMTXMultVec(rot, &m->facingDir, &m->facingDir);
        scl[0][3] += ox;
        scl[1][3] += oy;
        scl[2][3] += oz;
        PSMTXConcat(rot, scl, scl);
#if VERSION <= VERSION_DEMO_JP
        if (m->type != 2) {
            scl[0][3] = scl[0][3] + (m->hopX + (-ox + m->position.x));
            scl[1][3] = scl[1][3] + (m->hopY + (-oy + m->position.y));
            scl[2][3] = scl[2][3] + (m->hopZ + (-oz + m->position.z));
        } else {
            scl[0][3] = scl[0][3] + (m->hopX + (-ox + m->position.x));
            scl[1][3] = scl[1][3] + (m->bobOffset + (m->hopY + (-oy + m->position.y)));
            scl[2][3] = scl[2][3] + (m->hopZ + (-oz + m->position.z));
#else
        scl[0][3] = scl[0][3] + (m->hopX + (-ox + m->position.x));
        scl[2][3] = scl[2][3] + (m->hopZ + (-oz + m->position.z));
        if (m->type != AUDIENCE_BOO) {
            scl[1][3] = scl[1][3] + (m->hopY + (-oy + m->position.y));
        } else {
            scl[1][3] = scl[1][3] + (m->bobOffset + (m->hopY + (-oy + m->position.y)));
#endif
        }
        PSMTXConcat(cam->view, scl, fin);
        GXLoadPosMtxImm(fin, GX_PNMTX0);
        {
        f32 uvs[32] = {
            0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
            0.0f, 1.075f, 0.0f, 0.075f, 1.0f, 0.075f, 1.0f, 1.075f,
            0.0f, 0.075f, 0.0f, 1.075f, 1.0f, 1.075f, 0.0f, 1.075f
        };
        off = 0;
        front = BattleAudience_GetFront(i);
        cr = m->colorR;
        cg = m->colorG;
        cb = m->colorB;
        ca = m->colorA;
        if (m->animStart == m->animTable[AUD_ANIM_DAMAGE_FRONT]) off = 8;
        t0 = m->angle;
        if (t0 >= 360.0f) {
            m->angle = t0 - 360.0f * (f32)(s32)(t0 / 360.0f);
        }
        t0 = m->angle;
        if (t0 < 0.0f) {
            m->angle = t0 + 360.0f * (f32)(1 - (s32)(t0 / 360.0f));
        }
        t0 = m->angle;
        if (t0 > 0.0f && t0 <= 180.0f) flip = 1;
        if (t0 == 0.0f || (t0 > 180.0f && t0 < 360.0f)) flip = 0;
        // FAKE: (u8) cast on the BOOL front for the byte compare; plain front changes the object
        if ((u8)front == 1 && (cur = m->animStart) != m->animTable[AUD_ANIM_DAMAGE_FRONT] && cur != m->animTable[8] && cur != m->animTable[7]) {
            flip ^= 1;
        }
#if VERSION <= VERSION_DEMO_JP
        if (flip == 1) {
            off += 4;
        }
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        AUD_POS(nhw + m->dispOffset.x, 40.0f + m->dispOffset.y, m->dispOffset.z);
        GXWGFifo.u8 = cr;
        GXWGFifo.u8 = cg;
        GXWGFifo.u8 = cb;
        GXWGFifo.u8 = ca;
        AUD_TEX(uvs[off * 2], uvs[off * 2 + 1]);
        AUD_POS(hw + m->dispOffset.x, 40.0f + m->dispOffset.y, m->dispOffset.z);
        GXWGFifo.u8 = cr;
        GXWGFifo.u8 = cg;
        GXWGFifo.u8 = cb;
        GXWGFifo.u8 = ca;
        AUD_TEX(uvs[(off + 1) * 2], uvs[(off + 1) * 2 + 1]);
        AUD_POSS(hw, 0.0f, 0.0f);
        GXWGFifo.u8 = cr;
        GXWGFifo.u8 = cg;
        GXWGFifo.u8 = cb;
        GXWGFifo.u8 = ca;
        AUD_TEX(uvs[(off + 2) * 2], uvs[(off + 2) * 2 + 1]);
        AUD_POSS(nhw, 0.0f, 0.0f);
        GXWGFifo.u8 = cr;
        GXWGFifo.u8 = cg;
        GXWGFifo.u8 = cb;
        GXWGFifo.u8 = ca;
#else
        if (flip == 1) off += 4;
        GXSetChanMatColor(GX_COLOR0A0, (GXColor){cr, cg, cb, ca});
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        AUD_POS(nhw + m->dispOffset.x, 40.0f + m->dispOffset.y, m->dispOffset.z);
        AUD_TEX(uvs[off * 2], uvs[off * 2 + 1]);
        AUD_POS(hw + m->dispOffset.x, 40.0f + m->dispOffset.y, m->dispOffset.z);
        AUD_TEX(uvs[(off + 1) * 2], uvs[(off + 1) * 2 + 1]);
        AUD_POSS(hw, 0.0f, 0.0f);
        AUD_TEX(uvs[(off + 2) * 2], uvs[(off + 2) * 2 + 1]);
        AUD_POSS(nhw, 0.0f, 0.0f);
#endif
        AUD_TEX(uvs[(off + 3) * 2], uvs[(off + 3) * 2 + 1]);
        }
    }
}

/**
 * Draws each visible thrown item as its item icon, scaled
 * and spun about a point 20 units above it (display callback queued by BattleAudience_Disp).
 */
static void BattleAudienceDispItem(CameraId camId, void* param) {
    BattleAudienceItem* item;
    s32 i;
    u16 iconId;
    Mtx mtx;
    Mtx rot;

    BattleAudienceGXInit();
    i = 0;
    item = BattleAudienceItemGetPtr(0);
    for (; i < 100; i++, item++) {
        if ((item->flags & AUDIENCE_ITEM_FLAG_ACTIVE) && (item->flags & AUDIENCE_ITEM_FLAG_VISIBLE)) {
            iconId = itemDataTable[item->itemType].icon_id;
            PSMTXTrans(mtx, 0.0f, -20.0f, 0.0f);
            PSMTXRotRad(rot, 'z', 0.017453292f * item->rotZ);
            PSMTXConcat(rot, mtx, mtx);
            PSMTXTransApply(mtx, mtx, 0.0f, 20.0f, 0.0f);
            PSMTXScaleApply(mtx, mtx, item->scale, item->scale, 1.0f);
            PSMTXTransApply(mtx, mtx, item->position.x, item->position.y, item->position.z);
            ((void (*)(Mtx, s16, u16))iconDispGx2)(mtx, 0, iconId);
        }
    }
}

static void BattleAudienceDispApSrc(CameraId camId, void* param) {
    s32 i;
    BattleAudienceStar* star;
    Mtx rot;
    Mtx scale;
    Mtx trans;
    Mtx mtx;

    // FAKE: star holds the BattleAudience pointer until the loop; a separate BattleAudience* variable changes the object
    star = (BattleAudienceStar*)BattleAudienceBaseGetPtr();
    camGetPtr(CAMERA_2D);
    BattleAudienceGXInit();
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetZCompLoc(GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
    star = ((BattleAudience*)star)->starpower;
    for (i = 0; i < 100; i++, star++) {
        if (star->flags & AUDIENCE_STAR_FLAG_ACTIVE) {
            PSMTXScale(scale, star->scale.x, star->scale.y, star->scale.z);
            PSMTXRotRad(rot, 'z', 0.0f);
            PSMTXTrans(trans, star->position.x, star->position.y, 0.0f);
            PSMTXConcat(scale, rot, mtx);
            PSMTXConcat(trans, mtx, mtx);
            iconDispGxCol(mtx, ICON_FLAG_2D, ICON_STATUS_SP, (GXColor){0xFF, 0xFF, 0xFF, star->alpha});
        }
    }
}

/**
 * Draws the audience-count window: a frame at its sliding position,
 * the audience icon and the count shown (2D display callback queued by BattleAudience_Disp).
 * The early demos draw the icon slightly larger.
 */
static void BattleAudienceDispWin(CameraId camId, void* param) {
    BattleAudienceWindow* win;
    Mtx mtx;

    BattleAudienceBaseGetPtr();
    win = BattleAudienceWinGetPtr();
    {
        GXColor col = {0xFF, 0xFF, 0xFF, 0xCF};
        windowDispGX_Waku_col(0, col, win->x, win->y, 90.0f, 40.0f, 10.0f);
    }
    {
        Vec pos = {0.0f, 0.0f, 0.0f};
        pos.x = 17.0f + win->x;
        pos.y = win->y - 40.0f;
#if VERSION <= VERSION_DEMO_JP
        iconDispGx(&pos, ICON_FLAG_2D, ICON_AUDIENCE, 0.85f);
#else
        iconDispGx(&pos, ICON_FLAG_2D, ICON_AUDIENCE, 0.8f);
#endif
    }
    PSMTXTrans(mtx, 75.0f + win->x, win->y - 40.0f, 0.0f);
    {
        GXColor numCol = {0xFF, 0xFF, 0xFF, 0xFF};
        iconNumberDispGx(mtx, win->count, 0, numCol);
    }
}

BOOL BattleAudience_Entry(s32 index, AudienceMemberType type, u8 status) {
    BattleAudienceMember* m;
    f32 x;
    s32 t;

    BattleAudienceBaseGetPtr();
    BattleStageGetPtr();
    m = BattleAudienceGetPtr(index);
    if (!(audience_kind[type & 0xFF].flags & AUDIENCE_KIND_WIDE)) {
        if (m->flags & AUDIENCE_MEMBER_FLAG_USED) {
            return FALSE;
        }
        if (m->flags & AUDIENCE_MEMBER_FLAG_WIDE_SUB) {
            return FALSE;
        }
        m->flags = AUDIENCE_MEMBER_FLAG_USED | AUDIENCE_MEMBER_FLAG_DISP;
        t = (index % AUDIENCE_SEAT_COLUMNS) * 25 + rand() * 10 / 0x7fff;
        x = t - 0xeb;
        m->position.x = x;
        m->homePosition.x = x;
        BattleAudience_Entry_Sub(index, type, status);
    } else {
        if (index % 2 == 1) {
            return FALSE;
        }
        if (m->flags & AUDIENCE_MEMBER_FLAG_USED) {
            return FALSE;
        }
        if (m->flags & AUDIENCE_MEMBER_FLAG_WIDE_SUB) {
            return FALSE;
        }
        m->flags = AUDIENCE_MEMBER_FLAG_USED | AUDIENCE_MEMBER_FLAG_DISP | AUDIENCE_MEMBER_FLAG_WIDE;
        x = 12.5f + (f32)((index % AUDIENCE_SEAT_COLUMNS) * 25 - 230) + (f32)(rand() * 10 / 0x7fff) - 5.0f;
        m->position.x = x;
        m->homePosition.x = x;
        BattleAudience_Entry_Sub(index, type, status);
        m[1].flags = AUDIENCE_MEMBER_FLAG_USED | AUDIENCE_MEMBER_FLAG_WIDE_SUB;
    }
    return TRUE;
}

static void BattleAudience_Entry_Sub(s32 index, s32 type, u8 status) {
    BattleAudienceMember* m;
    BattleStage* stage;
    s32 row;
    f32 y, z;

    BattleAudienceBaseGetPtr();
    stage = BattleStageGetPtr();
    m = BattleAudienceGetPtr(index);
    m->step = 0;
    m->counter = 0;
    m->status = status;
    m->type = type;
    m->itemId = -1;
    m->animTable = audience_kind[type].animTable;
    m->timer = 0;
    BattleAudience_SetAnim(index, AUD_ANIM_WAIT_BACK, TRUE);
    m->animDepth = 0;
    m->colorB = 0;
    m->colorG = 0;
    m->colorR = 0;
    m->colorA = 255;
    row = index / AUDIENCE_SEAT_COLUMNS;
    y = stage->audienceSizeY;
    m->position.y = y;
    m->homePosition.y = y;
    z = 0.1f * (f32)(index % AUDIENCE_SEAT_COLUMNS) + (f32)(row * 30 + 0x73);
    m->position.z = z;
    m->homePosition.z = z;
    if (m->type == AUDIENCE_PIRANHA_PLANT) {
        m->homePosition.z += 0.2f;
        m->position.z += 0.2f;
    }
    if (m->status == AUDIENCE_STATUS_ENTER) {
        m->position.y = -2000.0f;
    }
    m->accel = (Vec){0.0f, 0.0f, 0.0f};
    m->velocity = m->accel;
    m->facingDir = (Vec){0.0f, 0.0f, -1.0f};
    m->rotation = (Vec){0.0f, 0.0f, 0.0f};
    m->rotationOffset = (Vec){0.0f, 0.0f, 0.0f};
    m->scale = (Vec){1.0f, 1.0f, 1.0f};
    m->baseScale = (Vec){1.0f, 1.0f, 1.0f};
    m->dispOffset = (Vec){0.0f, 0.0f, 0.0f};
    m->angle = 270.0f;
#if VERSION <= VERSION_DEMO_JP
    if ((index >= 2 && index <= 17) || (index >= 22 && index <= 37) || (index >= 42 && index <= 57)) {
#else
    if ((index >= 4 && index <= 15) || (index >= 24 && index <= 35) || (index >= 44 && index <= 55)) {
#endif
        m->flags |= AUDIENCE_MEMBER_FLAG_FRONT_CENTER;
    }
}

/**
 * Removes member index from the stands at once (no walk-out),
 * also freeing the right seat of a two-seat member. Called by the audience status code, audience scripts,
 * Frankly's lecture and the Cortez and Shadow Queen battles.
 */
void BattleAudience_Delete(s32 index) {
    BattleAudienceMember* member;

    BattleAudienceBaseGetPtr();
    member = BattleAudienceGetPtr(index);
    if (member->flags & AUDIENCE_MEMBER_FLAG_WIDE) {
        member[1].flags = 0;
    }
    member->flags = 0;
}

/**
 * Starts animation animId from member id's kind table, resetting its script and loop counters;
 * does nothing for a free seat, the right half of a two-seat member,
 * or an animation already running unless force is set.
 */
void BattleAudience_SetAnim(s32 id, s32 animId, BOOL force) {
    BattleAudienceMember* member;
    s32* anim;
    s32 i;

    BattleAudienceBaseGetPtr();
    member = BattleAudienceGetPtr(id);
    if (!(member->flags & AUDIENCE_MEMBER_FLAG_USED)) {
        return;
    }
    if (member->flags & AUDIENCE_MEMBER_FLAG_WIDE_SUB) {
        return;
    }
    anim = member->animTable[animId];
    if (member->animStart != NULL && member->animStart == anim && !force) {
        return;
    }
    member->animStart = anim;
    member->animPc = anim;
    member->animWait = 0;
    for (i = 0; i < 3; i++) {
        member->animLoop[i] = 0;
    }
    member->animDepth = 0;
}

/** TRUE once member index's animation script has reached its end op (-1) and its last wait has run out. */
BOOL BattleAudience_GetAnimEnd(s32 index) {
    BattleAudienceMember* member = BattleAudienceGetPtr(index);

    if (*member->animPc != -1) {
        return FALSE;
    }
    return member->animWait <= 0;
}

/** Reads member index's current position. */
void BattleAudience_GetPosition(s32 index, f32* x, f32* y, f32* z) {
    BattleAudienceMember* member;

    member = BattleAudienceGetPtr(index);
    *x = member->position.x;
    *y = member->position.y;
    *z = member->position.z;
}

/** Reads member index's seat position; the falling stage objects play their landing sound there. */
void BattleAudience_GetHomePosition(s32 index, f32* x, f32* y, f32* z) {
    BattleAudienceMember* member;

    member = BattleAudienceGetPtr(index);
    *x = member->homePosition.x;
    *y = member->homePosition.y;
    *z = member->homePosition.z;
}

/** Reads member index's rotation in degrees (used by Cortez's battle script). */
void BattleAudience_GetRotate(s32 index, f32* x, f32* y, f32* z) {
    BattleAudienceMember* member;

    member = BattleAudienceGetPtr(index);
    *x = member->rotation.x;
    *y = member->rotation.y;
    *z = member->rotation.z;
}

/** Moves member index to (x, y, z); used by audience scripts, Clock Out and the Shadow Queen battle. */
void BattleAudience_SetPosition(s32 index, f32 x, f32 y, f32 z) {
    BattleAudienceMember* member;

    member = BattleAudienceGetPtr(index);
    member->position.x = x;
    member->position.y = y;
    member->position.z = z;
}

/** Sets member index's rotation in degrees; used by audience scripts and Cortez's battle script. */
void BattleAudience_SetRotate(s32 index, f32 x, f32 y, f32 z) {
    BattleAudienceMember* member;

    member = BattleAudienceGetPtr(index);
    member->rotation.x = x;
    member->rotation.y = y;
    member->rotation.z = z;
}

/**
 * Sets the point member index rotates about, relative to its position and scaled with it (audience scripts).
 */
void BattleAudience_SetRotateOffset(s32 index, f32 x, f32 y, f32 z) {
    BattleAudienceMember* member;

    member = BattleAudienceGetPtr(index);
    member->rotationOffset.x = x;
    member->rotationOffset.y = y;
    member->rotationOffset.z = z;
}

/**
 * Finds the first member holding an item that has not landed yet and reports its index,
 * position (z + 3) and item; memberId gets -1 when nobody holds one.
 * Also remembers that member for BattleAudience_GetItemOn2.
 * The command menu uses it for the X-button attack on the item holder
 * (battle_menu_disp.c, battle_seq_command.c).
 */
void BattleAudience_GetItemOn(s32* memberId, f32* x, f32* y, f32* z, ItemType* itemType) {
    BattleAudience* audience;
    BattleAudienceMember* member;
    BattleAudienceItem* item;
    int i;

    audience = BattleAudienceBaseGetPtr();
    for (i = 0, member = BattleAudienceGetPtr(0); i < AUDIENCE_MEMBER_MAX; i++, member++) {
        if ((member->flags & AUDIENCE_MEMBER_FLAG_USED) && !(member->flags & AUDIENCE_MEMBER_FLAG_WIDE_SUB) && !(member->flags & AUDIENCE_MEMBER_FLAG_NOT_EXIST) && member->itemId != -1) {
            item = BattleAudienceItemGetPtr(member->itemId);
            if (item->state <= 4 || item->state == 7) {
                if (memberId) {
                    audience->itemOnMemberId = i;
                    *memberId = i;
                }
                audience->itemOnMemberPos.x = member->position.x;
                audience->itemOnMemberPos.y = member->position.y;
                audience->itemOnMemberPos.z = member->position.z + 3.0f;

                if (x) {
                    *x = audience->itemOnMemberPos.x;
                }
                if (y) {
                    *y = audience->itemOnMemberPos.y;
                }
                if (z) {
                    *z = audience->itemOnMemberPos.z;
                }

                if (itemType) {
                    if (member->itemId < 0) {
                        *itemType = ITEM_NULL;
                    } else {
                        *itemType = audience->items[member->itemId].itemType;
                    }
                }
                return;
            }
        }
    }
    *memberId = -1;
}

/**
 * Reports the item holder remembered by the last BattleAudience_GetItemOn:
 * index, position and item (any pointer may be NULL);
 * used by the audience-attack scripts in battle_attack_audience.c.
 */
void BattleAudience_GetItemOn2(s32* memberId, f32* x, f32* y, f32* z, ItemType* itemType) {
    BattleAudience* audience;
    BattleAudienceMember* member;

    audience = BattleAudienceBaseGetPtr();
    member = BattleAudienceGetPtr(audience->itemOnMemberId);
    if (memberId) {
        *memberId = audience->itemOnMemberId;
    }

    if (x) {
        *x = audience->itemOnMemberPos.x;
    }
    if (y) {
        *y = audience->itemOnMemberPos.y;
    }
    if (z) {
        *z = audience->itemOnMemberPos.z;
    }

    if (itemType) {
        if (member->itemId < 0) {
            *itemType = ITEM_NULL;
        } else {
            *itemType = audience->items[member->itemId].itemType;
        }
    }
}

void BattleAudience_SetTarget(s32 index) {
    BattleAudienceMember* member;
    s32 i;
#if VERSION <= VERSION_DEMO_JP
    member = BattleAudienceGetPtr(index);
    if (member->status == 7 && member->itemId != -1) {
        member->flags |= AUDIENCE_MEMBER_FLAG_TARGETED;
        for (i = 0; i < AUDIENCE_MEMBER_MAX; i++) {
            if (i != index && (u8)BattleAudience_GetExist(i)) {
                member = BattleAudienceGetPtr(i);
                if (member->status == AUDIENCE_STATUS_ITEM_ON) {
                    BattleAudienceItemGetPtr(member->itemId)->flags = 0;
                    member->itemId = -1;
                    BattleAudience_ChangeStatus(i, AUDIENCE_STATUS_WAIT);
                }
            }
        }
    }
}
#else
    BattleAudienceGetPtr(index)->flags |= AUDIENCE_MEMBER_FLAG_TARGETED;
    for (i = 0; i < AUDIENCE_MEMBER_MAX; i++) {
        if (i != index && (u8)BattleAudience_GetExist(i)) {
            member = BattleAudienceGetPtr(i);
            if (member->status == AUDIENCE_STATUS_ITEM_ON) {
                BattleAudienceItemGetPtr(member->itemId)->flags = 0;
                member->itemId = -1;
                BattleAudience_ChangeStatus(i, AUDIENCE_STATUS_WAIT);
            }
        }
    }
}
#endif

void BattleAudience_Attack(s32 index) {
    BattleAudienceGetPtr(index)->flags |= AUDIENCE_MEMBER_FLAG_ATTACKED;
}

/**
 * TRUE when member index faces the camera: its facing direction points against
 * the 3D camera's view direction.
 */
BOOL BattleAudience_GetFront(s32 index) {
    BattleAudienceMember* member;
    CameraEntry* camera;
    Vec dir;

    BattleAudienceBaseGetPtr();
    member = BattleAudienceGetPtr(index);
    camera = camGetPtr(CAMERA_3D);
    dir.x = camera->cameraAt.x - camera->cameraPos.x;
    dir.y = camera->cameraAt.y - camera->cameraPos.y;
    dir.z = camera->cameraAt.z - camera->cameraPos.z;
    return !(dir.x * member->facingDir.x + dir.y * member->facingDir.y + dir.z * member->facingDir.z >= 0.0f);
}

/**
 * Switches member index to a new status (the list is at BattleAudienceCtrlProcessKinopio),
 * resetting its colour, rotation, offsets, scale, velocity, timers and status work.
 * A call for the right half of a two-seat member goes to its left half; free seats are ignored.
 */
void BattleAudience_ChangeStatus(s32 index, s32 status) {
    BattleAudienceMember* member;

    BattleAudienceBaseGetPtr();
    member = BattleAudienceGetPtr(index);
    if (member->flags & AUDIENCE_MEMBER_FLAG_USED) {
        if (member->flags & AUDIENCE_MEMBER_FLAG_WIDE_SUB) {
            if (index >= 1) {
                BattleAudience_ChangeStatus(index - 1, status);
            }
        } else {
            member->colorB = 0;
            member->colorG = 0;
            member->colorR = 0;
            member->colorA = 0xFF;
            member->rotation = (Vec){0.0f, 0.0f, 0.0f};
            member->dispOffset = (Vec){0.0f, 0.0f, 0.0f};
            member->baseScale = (Vec){1.0f, 1.0f, 1.0f};
            member->velocity = (Vec){0.0f, 0.0f, 0.0f};
            member->timer = 0;
            member->step = 0;
            member->counter = 0;
            member->workF2 = 0.0f;
            member->workF1 = 0.0f;
            member->counterF = 0.0f;
            member->status = status;
        }
    }
}

BOOL BattleAudience_GetExist(s32 index) {
    BattleAudienceMember* member;

    BattleAudienceBaseGetPtr();
    member = BattleAudienceGetPtr(index);
    if (!(member->flags & AUDIENCE_MEMBER_FLAG_USED)) {
        return FALSE;
    }
    if (member->flags & AUDIENCE_MEMBER_FLAG_WIDE_SUB) {
        return FALSE;
    }
    return !(member->flags & AUDIENCE_MEMBER_FLAG_NOT_EXIST);
}

BOOL BattleAudience_GetSysCtrl(s32 index) {
    BattleAudienceMember* member;

    BattleAudienceBaseGetPtr();
    member = BattleAudienceGetPtr(index);
    if ((u8)BattleAudience_GetExist(index) == FALSE) {
        return FALSE;
    }
    return !(member->flags & AUDIENCE_MEMBER_FLAG_NO_SYS_CTRL);
}

/**
 * TRUE when member index is under audience control, holds no item and is idle or fidgeting (status 0-3),
 * so it is free to react; cheers, boos and whistles pick from these members.
 */
u8 BattleAudience_GetWaiting(s32 index) {
    BattleAudienceMember* member;

    BattleAudienceBaseGetPtr();
    member = BattleAudienceGetPtr(index);
    if (!(u8)BattleAudience_GetSysCtrl(index)) {
        return FALSE;
    }
    if (member->itemId != -1) {
        return FALSE;
    }
    return (u8)(member->status <= AUDIENCE_STATUS_WAIT_JUMP);
}

/**
 * TRUE when member index may be sent walking out (status 12):
 * audience hazards are on, it is under audience control, not marked never to leave, and not asleep,
 * in its shell, fuse lit, in a Boo or Shy Guy event, in an early-demo-only status or already leaving.
 * Used when the crowd shrinks, the Punies flee and falling objects or blasts scatter the crowd.
 */
BOOL BattleAudience_GetEscapeChangeOK(s32 index) {
    BattleAudience* audience;
    BattleAudienceMember* member;

    audience = BattleAudienceBaseGetPtr();
    member = BattleAudienceGetPtr(index);
    if (audience->flags & AUDIENCE_FLAG_NO_AUDIENCE_EVENTS) {
        return FALSE;
    }
    if (!(u8)BattleAudience_GetSysCtrl(index)) {
        return FALSE;
    }
    if (member->flags & AUDIENCE_MEMBER_FLAG_NO_ESCAPE) {
        return FALSE;
    }
    if (member->status == AUDIENCE_STATUS_SLEEP) {
        return FALSE;
    }
    if (member->status == AUDIENCE_STATUS_SHELL) {
        return FALSE;
    }
    if (member->status == AUDIENCE_STATUS_IGNITE) {
        return FALSE;
    }
    if (member->status == AUDIENCE_STATUS_TRANS_EVT) {
        return FALSE;
    }
    if (member->status == AUDIENCE_STATUS_INTRUDE) {
        return FALSE;
    }
    if (member->status == 21) {
        return FALSE;
    }
    if (member->status == 22) {
        return FALSE;
    }
    return member->status != AUDIENCE_STATUS_ESCAPE;
}

/**
 * Returns the seat column columns to the side and row rows off from seat index
 * (row is subtracted from the seat's row), or -1 when that falls outside the 20 columns or the 200 seats.
 * Used for neighbourhoods: Bulky Bob-omb blasts, Piranha Plant bites, falling objects and an idle-hop check.
 */
s32 BattleAudience_GetAudienceNoFromOffset(s32 index, s32 column, s32 row) {
    s32 audienceNo;

    BattleAudienceBaseGetPtr();
    BattleAudienceGetPtr(index);
    if (index % AUDIENCE_SEAT_COLUMNS + column < 0 || index % AUDIENCE_SEAT_COLUMNS + column >= AUDIENCE_SEAT_COLUMNS) {
        return -1;
    }

    audienceNo = column + (index - (row * AUDIENCE_SEAT_COLUMNS));
    if (audienceNo < 0 || audienceNo >= AUDIENCE_MEMBER_MAX) {
        return -1;
    }
    return audienceNo;
}

/**
 * Picks a random seat in the first three rows for a stage object to fall on (battle_stage_object.c):
 * row 0 columns 4-15, row 1 5-14, row 2 6-13 (the early demos: 1-18, 2-17, 3-16). Haiti is Japanese for seating.
 */
s32 BattleAudience_HaitiRandForFallObject(void) {
    s32 row = irand(3);
    s32 col = 10;
#if VERSION <= VERSION_DEMO_JP
    irand(18);
    switch (row) {
        case 0:
            col = irand(18) + 1;
            break;
        case 1:
            col = irand(16) + 2;
            break;
        case 2:
            col = irand(14) + 3;
#else
    irand(12);
    switch (row) {
        case 0:
            col = irand(12) + 4;
            break;
        case 1:
            col = irand(10) + 5;
            break;
        case 2:
            col = irand(8) + 6;
#endif
        break;
    }
    return col + row * 20;
}

/**
 * Picks a random free seat among those open at Mario's rank (haiti_data), or -1 when there is none;
 * battle mode 2 fills the stands with it.
 */
s32 BattleAudience_NoUsedHaitiRand(void) {
    s32* haiti;
    s32 id;
    s32 j;
    s32 k;
    s32 count;
    s32 i;
    s32 list[200];
    BattleAudienceBaseGetPtr();
    haiti = haiti_data[pouchGetPtr()->rank];
    count = 0;
    for (i = 0; i < 10; i++) {
        for (j = 0, k = (20 - haiti[i]) / 2; j < haiti[i]; j++, k++) {
            id = k + i * 20;
            if (!(BattleAudienceGetPtr(id)->flags & AUDIENCE_MEMBER_FLAG_USED) && !(BattleAudienceGetPtr(id)->flags & AUDIENCE_MEMBER_FLAG_WIDE_SUB)) {
                list[count++] = id;
            }
        }
    }
    if (count >= 1) {
        return list[irand(count)];
    }
    return -1;
}

#if VERSION >= VERSION_US
/**
 * Picks a random free seat in the rank's full seating layout (haiti_data),
 * weighting front rows and centre columns heavily; returns the member index or -1 when the stands are full.
 */
s32 BattleAudience_NoUsedFCHaitiRandFull(void) {
#else
/**
 * Picks the seat for a new member (crowd setup, arrivals, Punies): a random free seat in the rank's seating
 * layout (haiti_data), weighting front rows and centre columns heavily; -1 when the stands are full.
 * JP and the early demos have no narrow front block (BattleAudience_NoUsedFCHaitiRandCenter is US and later).
 */
s32 BattleAudience_NoUsedFCHaitiRand(void) {
#endif
    // audience is not used (the call is in the retail code)
    BattleAudience* audience = BattleAudienceBaseGetPtr();
    s32 rank = pouchGetPtr()->rank;
    s32* haiti;
    s32 count = 0;
    s32 total = 0;
    s32 rowW[10] = { 10000, 1000, 100, 10, 5, 2, 1, 1, 1, 1 };
    s32 colW[20] = { 1, 1, 2, 2, 4, 4, 8, 8, 16, 16, 16, 16, 8, 8, 4, 4, 2, 2, 1, 1 };
    s32 list[200];
    s32 cumul[200];
    s32 i, j, k, id, w, r;

    haiti = haiti_data[rank];
    for (i = 0; i < 10; i++) {
        for (j = 0, k = (20 - haiti[i]) / 2; j < haiti[i]; j++, k++) {
            id = k + i * 20;
            if (!(BattleAudienceGetPtr(id)->flags & AUDIENCE_MEMBER_FLAG_USED) && !(BattleAudienceGetPtr(id)->flags & AUDIENCE_MEMBER_FLAG_WIDE_SUB)) {
                w = rowW[i] * colW[k] + 1;
                list[count] = id;
                if (count) {
                    cumul[count] = w + cumul[count - 1];
                } else {
                    cumul[count] = w;
                }
                total += w;
                count++;
            }
        }
    }
    r = (f32)total * (f32)rand() / 32767.0f;
    for (i = 0; i < count; i++) {
        if (r < cumul[i]) {
            return list[i];
        }
    }
    return -1;
#if VERSION >= VERSION_US
}

/**
 * Same as BattleAudience_NoUsedFCHaitiRandFull but only over the narrower front block (haiti_data_front_narrow),
 * so a small crowd sits together near the stage.
 */
s32 BattleAudience_NoUsedFCHaitiRandCenter(void) {
    // audience is not used (the call is in the retail code)
    BattleAudience* audience = BattleAudienceBaseGetPtr();
    s32 rank = pouchGetPtr()->rank;
    s32* haiti;
    s32 count = 0;
    s32 total = 0;
    s32 rowW[10] = { 10000, 1000, 100, 10, 5, 2, 1, 1, 1, 1 };
    s32 colW[20] = { 1, 1, 2, 2, 4, 4, 8, 8, 16, 16, 16, 16, 8, 8, 4, 4, 2, 2, 1, 1 };
    s32 list[200];
    s32 cumul[200];
    s32 i, j, k, id, w, r;

    haiti = haiti_data_front_narrow[rank];
    for (i = 0; i < 10; i++) {
        for (j = 0, k = (20 - haiti[i]) / 2; j < haiti[i]; j++, k++) {
            id = k + i * 20;
            if (!(BattleAudienceGetPtr(id)->flags & AUDIENCE_MEMBER_FLAG_USED) && !(BattleAudienceGetPtr(id)->flags & AUDIENCE_MEMBER_FLAG_WIDE_SUB)) {
                w = rowW[i] * colW[k] + 1;
                list[count] = id;
                if (count) {
                    cumul[count] = w + cumul[count - 1];
                } else {
                    cumul[count] = w;
                }
                total += w;
                count++;
            }
        }
    }
    r = (f32)total * (f32)rand() / 32767.0f;
    for (i = 0; i < count; i++) {
        if (r < cumul[i]) {
            return list[i];
        }
    }
    return -1;
}

/**
 * Picks the seat for a new member (crowd setup, arrivals, Punies):
 * while fewer than 20 are present it tries the narrow front block first
 * (BattleAudience_NoUsedFCHaitiRandCenter) so a small crowd sits together,
 * otherwise the full layout (BattleAudience_NoUsedFCHaitiRandFull); -1 when no seat is free.
 */
s32 BattleAudience_NoUsedFCHaitiRand(void) {
    s32 id;

    if (BattleAudience_GetAudienceNum() >= 20) {
        id = BattleAudience_NoUsedFCHaitiRandFull();
    } else {
        id = BattleAudience_NoUsedFCHaitiRandCenter();
        if (id == -1) {
            id = BattleAudience_NoUsedFCHaitiRandFull();
        }
    }
    return id;
#endif
}

/**
 * Returns the number of members in the stands as counted this frame (not those arriving or leaving);
 * the seat picker, the Star Power special moves and the Magnus von Grapple 2.0
 * and Shadow Queen battles use it.
 */
s32 BattleAudience_GetAudienceNum(void) {
    return BattleAudienceBaseGetPtr()->currentAudienceIntCount;
}

/**
 * Counts the members present and taking part (not asleep, in a shell or leaving) on both halves;
 * it sets the crowd-sound tiers and the Star Power from an Appeal.
 */
s32 BattleAudience_GetPPAudienceNum(void) {
    return BattleAudience_GetPPAudienceNum_R() + BattleAudience_GetPPAudienceNum_L();
}

/**
 * Counts the members of kind type present and taking part;
 * BattleAudience_ApRecoveryBuild weighs the Star Power by kind with it.
 */
s32 BattleAudience_GetPPAudienceNumKind(s32 type) {
    s32 i, count;

    BattleAudienceBaseGetPtr();
    count = 0;
    for (i = 0; i < AUDIENCE_MEMBER_MAX; i++) {
        if (BattleAudience_GetPPAudienceNum_Sub(i) && type == BattleAudienceGetPtr(i)->type) {
            count++;
        }
    }
    return count;
}

/**
 * Counts the members taking part in the right half of the stands (columns 10-19); crowd sounds pan with it.
 */
static s32 BattleAudience_GetPPAudienceNum_R(void) {
    return BattleAudience_GetPPAudienceNum_RL_Sub(10);
}

/** Counts the members taking part in the left half of the stands (columns 0-9); crowd sounds pan with it. */
static s32 BattleAudience_GetPPAudienceNum_L(void) {
    return BattleAudience_GetPPAudienceNum_RL_Sub(0);
}

/**
 * Counts the members in one half of the stands (offset 0 = left ten columns, 10 = right) that are present
 * and taking part: not asleep, in a shell or leaving (BattleAudience_GetPPAudienceNum_Sub).
 */
static s32 BattleAudience_GetPPAudienceNum_RL_Sub(s32 offset) {
    s32 i, j, index, count;

    BattleAudienceBaseGetPtr();
    count = 0;
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) {
            index = offset + ((i * 20) + j);
            if (BattleAudience_GetPPAudienceNum_Sub(index) == TRUE) {
                count++;
            }
        }
    }
    return count;
}

/** TRUE when member id exists and takes part: not asleep (13), hiding in its shell (17) or leaving (12). */
static BOOL BattleAudience_GetPPAudienceNum_Sub(s32 id) {
    BattleAudienceMember* member = BattleAudienceGetPtr(id);

    if (!(u8)BattleAudience_GetExist(id)) {
        return FALSE;
    }

    // a two-sided range test with equal bounds: the retail code has cmplwi 13 with both blt and bgt (== gives one bne)
    if (member->status >= AUDIENCE_STATUS_SLEEP && member->status <= AUDIENCE_STATUS_SLEEP) {
        return FALSE;
    }
    if (member->status == AUDIENCE_STATUS_SHELL) {
        return FALSE;
    }
    return member->status != AUDIENCE_STATUS_ESCAPE;
}

/**
 * Turns the Star Power bonuses of the move just made into Star Power gain (btlevtcmd_InviteApInfoReport):
 * keeps a copy of them for BattleAudience_PerAct, then adds the square root of the crowd's weight
 * (each kind's spWeight times its members taking part) times the sum of the action-command
 * and Stylish multipliers, times how many of those two are nonzero,
 * times 3 when Mario is in Peril (2 in Danger) and times 2 when the partner is in Peril (1.5 in Danger);
 * during the post-BINGO! boost also times its multiplier.
 * May also add a point to the bingo slots (bingoSlotChance percent).
 */
void BattleAudience_ApRecoveryBuild(StarPowerInfo* info) {
    BattleWork* bw = _battleWorkPointer;
    BattleAudience* audience = BattleAudienceBaseGetPtr();
    BattleUnit* mario;
    BattleUnit* party;
    f32 sum, root, a, b, cnt, mm, pm, scale = 1.0f;
    s32 i;

    if (audience->impendingBonuses == NULL) {
        audience->impendingBonuses = BattleAlloc(sizeof(StarPowerInfo));
    }
    memcpy(audience->impendingBonuses, info, sizeof(StarPowerInfo));
    mario = BattleGetMarioPtr(bw);
    party = BattleGetPartyPtr(bw);
    sum = 0.0f;
    for (i = 0; i < 13; i++) {
        sum += audience_kind[i].spWeight * (f32)BattleAudience_GetPPAudienceNumKind(i);
    }
    root = sqrt(sum);
    a = info->spAcSuccessMultiplier;
    b = (f32)info->stylishCommandMultiplier;
    cnt = 0.0f;
    if (a > 0.0f) {
        cnt += 1.0f;
    }
    if (b > 0.0f) {
        cnt += 1.0f;
    }
    if (mario->statusFlags & UNIT_STATUS_FLAG_PERIL) {
        mm = 3.0f;
    } else if (mario->statusFlags & UNIT_STATUS_FLAG_DANGER) {
        mm = 2.0f;
    } else {
        mm = 1.0f;
    }
    if (party != NULL) {
        if (party->statusFlags & UNIT_STATUS_FLAG_PERIL) {
            pm = 2.0f;
        } else if (party->statusFlags & UNIT_STATUS_FLAG_DANGER) {
            pm = 1.5f;
        } else {
            pm = 1.0f;
        }
    } else {
        pm = 1.0f;
    }
    if (BattleBreakSlot_GetBreakTurn() >= 1) {
        audience->starPowerGain = audience->starPowerGain + (s32)((pm * (mm * (cnt * (scale * root * (a + b))))) * BattleBreakSlotGetPtr()->spMultiplier);
    } else {
        audience->starPowerGain = audience->starPowerGain + (s32)((mm * (cnt * (scale * root * (a + b)))) * pm);
    }
    if (irand(100) < (u8)info->bingoSlotChance) {
        BattleBreakSlot_PointInc();
    }
}

/**
 * Shows (nonzero) or hides the audience-count window, which slides in or out over 15 frames;
 * used by the action-command help and battle end.
 */
void BattleAudience_WinSetActive(u8 active) {
    BattleAudienceWinGetPtr()->active = active;
}

/**
 * Walks members in or out to bring the crowd to its target size plus the temporary extra,
 * clamped to 0 and the rank's maximum, counting the members present
 * and not leaving (BattleAudienceAddAudienceNum). Does nothing while the audience is frozen.
 * Called at battle start, at each turn end and by the bingo slots.
 */
void BattleAudienceNumToTarget(void) {
    s32 num;
    BattleAudience* audience = BattleAudienceBaseGetPtr();

    if (audience->flags & AUDIENCE_FLAG_EVENT_CTRL) {
        return;
    }
#if VERSION <= VERSION_DEMO_JP
    BattleAudienceNumToTargetSub();
#endif
    num = audience->targetAudienceCount + audience->addedAudienceCount;
    if (num < 0) {
        num = 0;
    }
    if (num > audience->maxAudience) {
        num = audience->maxAudience;
    }
#if VERSION <= VERSION_DEMO_JP
    BattleAudienceAddAudienceNum(num - audience->presentCount);
    BattleAudienceNumToTargetSub();
#else
    BattleAudienceAddAudienceNum(num - BattleAudienceNumToTargetSub());
#endif
}

/**
 * Counts the members present in the stands that are not on their way out;
 * the early demos store it in presentCount instead of returning it.
 */
#if VERSION <= VERSION_DEMO_JP
static void BattleAudienceNumToTargetSub(void) {
    BattleAudience* audience = BattleAudienceBaseGetPtr();
    s32 i;

    audience->presentCount = 0;
    for (i = 0; i < AUDIENCE_MEMBER_MAX; i++) {
        if ((u8)BattleAudience_GetExist(i) == TRUE && BattleAudienceGetPtr(i)->status != AUDIENCE_STATUS_ESCAPE) {
            audience->presentCount++;
        }
    }
#else
static s32 BattleAudienceNumToTargetSub(void) {
    s32 i, count;

    BattleAudienceBaseGetPtr();
    count = 0;
    for (i = 0; i < AUDIENCE_MEMBER_MAX; i++) {
        if ((u8)BattleAudience_GetExist(i) == TRUE && BattleAudienceGetPtr(i)->status != AUDIENCE_STATUS_ESCAPE) {
            count++;
        }
    }
    return count;
#endif
}

/**
 * Changes the crowd's target size by value and its temporary extra by value2.
 * A change against the sign of the extra is taken from the extra first
 * (a loss eats into extra fans, a gain makes up a shortfall);
 * the rest goes through BattleAudienceAddTargetNumSub.
 * The target is clamped to 0 and the rank's maximum, the extra to -1000..1000.
 * Members only come and go at the next BattleAudienceNumToTarget.
 * The crowd reactions (BattleAudience_Case_*) use it, as do the bingo slots,
 * audience scripts and some bosses.
 */
void BattleAudienceAddTargetNum(f32 value, f32 value2) {
    BattleAudience* audience = BattleAudienceBaseGetPtr();

    audience->addedAudienceCount += value2;
    if (audience->addedAudienceCount > 0.0f && value < 0.0f) {
        audience->addedAudienceCount += value;
        if (audience->addedAudienceCount < 0.0f) {
            value = audience->addedAudienceCount;
            audience->addedAudienceCount = 0.0f;
        } else {
            value = 0.0f;
        }
    } else if (audience->addedAudienceCount < 0.0f && value > 0.0f) {
        audience->addedAudienceCount += value;
        if (audience->addedAudienceCount > 0.0f) {
            value = audience->addedAudienceCount;
            audience->addedAudienceCount = 0.0f;
        } else {
            value = 0.0f;
        }
    }
    BattleAudienceAddTargetNumSub(value);
    if (audience->targetAudienceCount < 0.0f) {
        audience->targetAudienceCount = 0.0f;
    }
    if (audience->targetAudienceCount > audience->maxAudience) {
        audience->targetAudienceCount = audience->maxAudience;
    }
    if (audience->addedAudienceCount < -1000.0f) {
        audience->addedAudienceCount = -1000.0f;
    }
    if (audience->addedAudienceCount > 1000.0f) {
        audience->addedAudienceCount = 1000.0f;
    }
}

/**
 * Adds value to the target audience size one fan at a time with diminishing returns:
 * while the target is at least 25% above the level's audience factor (audienceDeltaMultiplier),
 * each fan gained counts 1 / (n + 1) for n whole 25% steps above it, and while it is 25% or more below,
 * each fan lost counts the same way, so the crowd drifts back toward the level's usual size.
 */
static void BattleAudienceAddTargetNumSub(f32 value) {
    f32 subtract;
    s32 skew;

    BattleAudience* audience = BattleAudienceBaseGetPtr();
    while (1) {
        if (value == 0.0f) {
            return;
        }
        if (value > 0.0f) {
            subtract = value < 1.0f ? value : 1.0f;
            value -= subtract;
            if (value < 0.0f) {
                value = 0.0f;
            }
        }
        if (value < 0.0f) {
            subtract = value > -1.0f ? value : -1.0f;
            value -= subtract;
            if (value > 0.0f) {
                value = 0.0f;
            }
        }
        if (subtract > 0.0f) {
            skew = (s32)(((audience->targetAudienceCount / audience->audienceDeltaMultiplier) - 1.0f) / 0.25f);
            if (skew >= 1) {
                audience->targetAudienceCount += subtract / (skew + 1);
                continue;
            }
        }
        if (subtract < 0.0f) {
            skew = (s32)(((audience->targetAudienceCount / audience->audienceDeltaMultiplier) - 1.0f) / 0.25f);
            if (skew <= -1) {
                audience->targetAudienceCount -= subtract / (skew - 1);
                continue;
            }
        }
        audience->targetAudienceCount += subtract;
    }
}

/**
 * Brings num members into the stands or, for a negative num, sends that many away.
 * Arrivals get kinds rolled from the battle's kind weights
 * and walk in to seats from BattleAudience_NoUsedFCHaitiRand; leavers are random members allowed to leave.
 * Each gets its place in line as counter, the start delay of its walk.
 * Returns how many came (positive) or left (negative);
 * does nothing before Mario has Star Power or while Mario's party stands in the audience area
 * (BATTLE_FLAG_PARTY_IN_AUDIENCE, Hooktail).
 * Used by BattleAudienceNumToTarget, the bingo slots and Frankly's lecture.
 */
s32 BattleAudienceAddAudienceNum(s32 num) {
    s32 total;
    s32 list[200];
    BattleWork* wp;
    BattleAudience* audience;
    s32 i, j, r, sum, id, count;
    u8 type;
    s32 added;

    wp = _battleWorkPointer;
    audience = BattleAudienceBaseGetPtr();
    total = 0;
    added = 0;
    if (pouchGetPtr()->starPowersObtained == 0) {
        return 0;
    }
    if (num == 0) {
        return 0;
    }
    if (wp->flags & BATTLE_FLAG_PARTY_IN_AUDIENCE) {
        return 0;
    }
    if (num > 0) {
        for (i = 0; i < 12; i++) {
            total += audience->typeWeights[i];
        }
        for (i = 0; i < num; i++) {
            sum = 0;
            r = irand(total);
            for (j = 0; j < 12; j++) {
                sum += audience->typeWeights[j];
                if (r < sum) {
                    type = j;
                    break;
                }
            }
            id = BattleAudience_NoUsedFCHaitiRand();
            if (id == -1) {
                break;
            }
            if (BattleAudience_Entry(id, type, AUDIENCE_STATUS_ENTER) == TRUE) {
                added++;
                BattleAudienceGetPtr(id)->counter = added;
            }
        }
    } else {
        for (i = 0; i > num; i--) {
            count = 0;
            for (j = 0; j < AUDIENCE_MEMBER_MAX; j++) {
                if ((u8)BattleAudience_GetEscapeChangeOK(j)) {
                    list[count++] = j;
                }
            }
            if (count >= 1) {
                id = list[irand(count)];
                BattleAudience_ChangeStatus(id, AUDIENCE_STATUS_ESCAPE);
                added--;
                BattleAudienceGetPtr(id)->counter = -added;
            }
        }
    }
    return added;
}

/**
 * Sends every Puni that may leave walking out, each taken off the temporary extra
 * (the target size in the early demos), and flags the escape for its announcement (msg_puni_all_escape).
 * Called after falling objects hit the stands and after a Bulky Bob-omb blast.
 */
void BattleAudiencePuniAllEscape(void) {
    BattleAudienceMember* member;
    BattleAudience* audience;
    s32 i;
    s32 count;

    audience = BattleAudienceBaseGetPtr();
    count = 0;
    for (i = 0; i < AUDIENCE_MEMBER_MAX; i++) {
        member = BattleAudienceGetPtr(i);
        if ((u8)BattleAudience_GetEscapeChangeOK(i) && member->type == AUDIENCE_PUNI) {
            BattleAudience_ChangeStatus(i, AUDIENCE_STATUS_ESCAPE);
#if VERSION <= VERSION_DEMO_JP
            audience->targetAudienceCount -= 1.0f;
#else
            audience->addedAudienceCount -= 1.0f;
#endif
            count++;
        }
    }
    if (count >= 1) {
        audience->flags |= AUDIENCE_FLAG_PUNI_ESCAPE;
    }
}

/**
 * Brings up to num Punies to free seats, each adding one to the temporary extra
 * (the target size in the early demos); only once Mario has Star Power,
 * when the battle's crowd includes Punies and not while Mario's party stands in the audience area.
 * A Stylish move brings 10 (BattleAudience_PerAct), the Puni phase reaction 200.
 */
void BattleAudienceAddPuni(s32 num) {
    BattleAudience* audience;
    BattleWork* wp = _battleWorkPointer;
    s32 i;

    audience = BattleAudienceBaseGetPtr();
    if (pouchGetPtr()->starPowersObtained != 0 && audience->typeWeights[AUDIENCE_PUNI] > 0 && num > 0 && !(wp->flags & BATTLE_FLAG_PARTY_IN_AUDIENCE)) {
        for (i = 0; i < num; i++) {
            s32 id = BattleAudience_NoUsedFCHaitiRand();
            if (id == -1) {
                return;
            }
            if (BattleAudience_Entry(id, AUDIENCE_PUNI, AUDIENCE_STATUS_ENTER) == TRUE) {
#if VERSION <= VERSION_DEMO_JP
                audience->targetAudienceCount += 1.0f;
#else
                audience->addedAudienceCount += 1.0f;
#endif
            }
        }
    }
}

/** Queues phase reaction type (see BattleAudience_CheckReactionPerPhase) for the next enemy phase, once. */
static void BattleAudienceAddPhaseEvtList(s32 type) {
    s32 i;
    BattleAudience* audience = BattleAudienceBaseGetPtr();

    for (i = 0; i < audience->phaseListCount; i++) {
        if (audience->phaseList[i] == type) {
            return;
        }
    }
    audience->phaseList[audience->phaseListCount] = type;
    audience->phaseListCount++;
}

/**
 * Hands out the Star Power the action earned, capped at 800:
 * the crowd reacts (BattleAudienceJoy with type) and the status window shows the AP refill,
 * which the sparkles then deliver. Before Mario can hold Star Power the gain is dropped,
 * though the crowd still reacts.
 */
static void BattleAudienceCheer(s32 type) {
    BattleAudience* audience = BattleAudienceBaseGetPtr();

    if (audience->starPowerGain > 800) {
        audience->starPowerGain = 800;
    }
    if (audience->starPowerGain > 0) {
        if (pouchGetPtr()->maximumSP == 0) {
            audience->starPowerGain = 0;
        } else {
            audience->flags |= AUDIENCE_FLAG_SP_GAINED;
        }
        BattleAudienceJoy(type);
        if (audience->starPowerGain != 0) {
            BattleStatusWindowAPRecoveryOn();
        }
    }
}

/**
 * Makes the idle members react: type 0 all raise their arms, 1 all hop, 2 mostly raise their arms (70 to 30),
 * 3 a mix that leans further toward big spinning hops each time it is used in the battle
 * (joyLevel 0-4; it never rises in the early demos). Called by the action-command, Stylish and Appeal reactions,
 * the Star Power cheer and the audience scripts.
 */
static void BattleAudienceJoy(s32 type) {
    BattleAudience* audience = BattleAudienceBaseGetPtr();

    switch (type) {
        case 0:
            BattleAudienceJoy_Sub(100, 0, 0, type);
            break;
        case 1:
            BattleAudienceJoy_Sub(0, 100, 0, type);
            break;
        case 2:
            BattleAudienceJoy_Sub(70, 30, 0, type);
            break;
        case 3:
            switch (audience->joyLevel) {
            case 0:
                BattleAudienceJoy_Sub(50, 40, 10, type);
                break;
            case 1:
                BattleAudienceJoy_Sub(40, 40, 20, type);
                break;
            case 2:
                BattleAudienceJoy_Sub(30, 40, 30, type);
                break;
            case 3:
                BattleAudienceJoy_Sub(20, 40, 40, type);
                break;
            case 4:
                BattleAudienceJoy_Sub(10, 40, 50, type);
                break;
            default:
                BattleAudienceJoy_Sub(50, 40, 10, type);
                break;
            }
#if VERSION >= VERSION_JP
            audience->joyLevel++;
            if (audience->joyLevel > 4) {
                audience->joyLevel = 4;
            }
#endif
            break;
    }
}

/** Crowd reaction for Frankly's Star Power lecture: a 120-frame cheer and BattleAudienceJoy type 2. */
void BattleAudienceJoySACLecture(void) {
    BattleAudienceSoundCheer(120, 60);
    BattleAudienceJoy(2);
}

/** Whole-crowd reaction of BattleAudienceJoy type 3, for the audience script command in evt_audience.c. */
void BattleAudienceJoyEnding(void) {
    BattleAudienceJoy(3);
}

#pragma push
// kept: loop-invariant code motion off for BattleAudienceJoy_Sub (until the pop); without it the function changes,
// and -opt noloop for the whole file changes other functions
#pragma opt_loop_invariants off
/**
 * Makes every waiting member react (BattleAudienceJoy):
 * status 4 (counterF 3 for type 3, else 2) with weight a, status 5 with weight b, status 6 with weight c.
 * Piranha Plants always take status 4.
 */
static void BattleAudienceJoy_Sub(s32 a, s32 b, s32 c, s32 type) {
    BattleAudienceMember* member;
    s32 i;
    s32 total;
    s32 r, ab;

    BattleAudienceBaseGetPtr();
    total = c + (a + b);
    for (i = 0, member = BattleAudienceGetPtr(0); i < AUDIENCE_MEMBER_MAX; i++, member++) {
        if (!BattleAudience_GetWaiting(i)) {
            continue;
        }
        if (member->type == AUDIENCE_PIRANHA_PLANT) {
            BattleAudience_ChangeStatus(i, AUDIENCE_STATUS_JOY_RAISE_ARM);
            if (type == 3) {
                member->counterF = 3.0f;
            } else {
                member->counterF = 2.0f;
            }
        } else {
            r = irand(total);
            if (r < a) {
                BattleAudience_ChangeStatus(i, AUDIENCE_STATUS_JOY_RAISE_ARM);
                if (type == 3) {
                    member->counterF = 3.0f;
                } else {
                    member->counterF = 2.0f;
                }
            } else if (r < (ab = a + (s32)b)) { // FAKE: no-op (s32) cast; `a + b` changes the object
                BattleAudience_ChangeStatus(i, AUDIENCE_STATUS_JOY_JUMP);
            } else if (r < ab + c) {
                BattleAudience_ChangeStatus(i, AUDIENCE_STATUS_JOY_JUMP2);
            }
        }
    }
}

#pragma pop

/**
 * Returns the seat dir columns beside Piranha Plant index
 * when a member under audience control sits there, else -1.
 */
static s32 BattleAudienceDetectPakkunEatTargetSub2(s32 index, s32 dir) {
    s32 id;

    BattleAudienceBaseGetPtr();
    id = BattleAudience_GetAudienceNoFromOffset(index, dir, 0);
    if (id == -1) {
        return -1;
    }
    BattleAudienceGetPtr(id);
    if ((u8)BattleAudience_GetSysCtrl(id) == TRUE) {
        return id;
    }
    return -1;
}

/**
 * Returns the neighbour on the dir1 side, else the one on the dir2 side
 * (BattleAudienceDetectPakkunEatTargetSub2), or -1.
 */
static s32 BattleAudienceDetectPakkunEatTargetSub(s32 index, s32 dir1, s32 dir2) {
    s32 a = BattleAudienceDetectPakkunEatTargetSub2(index, dir1);
    s32 b = BattleAudienceDetectPakkunEatTargetSub2(index, dir2);

    if (a != -1) {
        return a;
    }
    if (b != -1) {
        return b;
    }
    return -1;
}

/**
 * Picks the neighbour a Piranha Plant (pakkun) in seat index bites in its phase event:
 * the member right beside it, the side tried first chosen at random;
 * -1 when neither seat holds one, and then the event is not offered.
 */
s32 BattleAudienceDetectPakkunEatTarget(s32 index) {
    if (irand(2) == 0) {
        return BattleAudienceDetectPakkunEatTargetSub(index, -1, 1);
    }
    return BattleAudienceDetectPakkunEatTargetSub(index, 1, -1);
}

/**
 * Starts the crowd's booing (SFX_AUDIENCE_BOO_ALLY1-4) at the tier for the members taking part
 * (under 50, 50-99, 100-149, 150 and more). Used by the failed-move and run-away reactions,
 * the bingo slots and Earth Tremor.
 */
void BattleAudienceSoundBooing(void) {
    s32 kind;
    s32 num;

    BattleAudienceBaseGetPtr();
    kind = 0;
    num = BattleAudience_GetPPAudienceNum_R() + BattleAudience_GetPPAudienceNum_L();
    if (num >= 0 && num <= 49) {
        kind = 1;
    }
    if (num >= 50 && num <= 99) {
        kind = 2;
    }
    if (num >= 100 && num <= 149) {
        kind = 3;
    }
    if (num >= 150) {
        kind = 4;
    }
    BattleAudienceSoundBooingKind(kind);
}

/**
 * Plays boo sound kind on channel 0, replacing any boo still playing:
 * 1-4 SFX_AUDIENCE_BOO_ALLY1-4, 5-8 SFX_AUDIENCE_BOO_ENEMY1-4, panned and scaled to the crowd;
 * it plays to its end. The Star Power special moves call it directly.
 */
void BattleAudienceSoundBooingKind(s32 kind) {
    char name[64];
    BattleAudienceSound* sound;
    s32 right, left;

    BattleAudienceBaseGetPtr();
    sound = BattleAudienceSoundGetPtr(AUD_SOUND_BOOING);
    right = BattleAudience_GetPPAudienceNum_R();
    left = BattleAudience_GetPPAudienceNum_L();
    BattleAudienceSoundStop(AUD_SOUND_BOOING);
    if (kind == 1) {
        strcpy(name, "SFX_AUDIENCE_BOO_ALLY1");
        sound->kind = 1;
    }
    if (kind == 2) {
        strcpy(name, "SFX_AUDIENCE_BOO_ALLY2");
        sound->kind = 2;
    }
    if (kind == 3) {
        strcpy(name, "SFX_AUDIENCE_BOO_ALLY3");
        sound->kind = 3;
    }
    if (kind == 4) {
        strcpy(name, "SFX_AUDIENCE_BOO_ALLY4");
        sound->kind = 4;
    }
    if (kind == 5) {
        strcpy(name, "SFX_AUDIENCE_BOO_ENEMY1");
        sound->kind = 5;
    }
    if (kind == 6) {
        strcpy(name, "SFX_AUDIENCE_BOO_ENEMY2");
        sound->kind = 6;
    }
    if (kind == 7) {
        strcpy(name, "SFX_AUDIENCE_BOO_ENEMY3");
        sound->kind = 7;
    }
    if (kind == 8) {
        strcpy(name, "SFX_AUDIENCE_BOO_ENEMY4");
        sound->kind = 8;
    }
    sound->sfxId = BattleAudienceSound1(name, right, left);
    AUD_SOUND_START(sound, -1, 0);
}

/**
 * Plays the crowd clapping on channel 1 for duration frames,
 * fading out over the last fade (-1 plays to the end):
 * SFX_AUDIENCE_CLAPPING1-4, a louder tier at 50, 100 and 150 members taking part.
 * Used by the Stylish and Appeal reactions, battle end and the Star Power special moves.
 */
void BattleAudienceSoundClap(s32 duration, s32 fade) {
    char name[64];
    BattleAudienceSound* sound;
    s32 right, left, sum;

    BattleAudienceBaseGetPtr();
    sound = BattleAudienceSoundGetPtr(AUD_SOUND_CLAP);
    right = BattleAudience_GetPPAudienceNum_R();
    left = BattleAudience_GetPPAudienceNum_L();
    sum = right + left;
    BattleAudienceSoundStop(AUD_SOUND_CLAP);
    if (sum >= 1 && sum <= 49) {
        strcpy(name, "SFX_AUDIENCE_CLAPPING1");
        sound->kind = 1;
    }
    if (sum >= 50 && sum <= 99) {
        strcpy(name, "SFX_AUDIENCE_CLAPPING2");
        sound->kind = 2;
    }
    if (sum >= 100 && sum <= 149) {
        strcpy(name, "SFX_AUDIENCE_CLAPPING3");
        sound->kind = 3;
    }
    if (sum >= 150) {
        strcpy(name, "SFX_AUDIENCE_CLAPPING4");
        sound->kind = 4;
    }

    // BUG: with no member taking part (sum 0) name is never written, so an uninitialised buffer is played.
    sound->sfxId = BattleAudienceSound1(name, right, left);
    AUD_SOUND_START(sound, duration + fade, fade);
}

/**
 * Starts the crowd's background noise loop on channel 2 (SFX_AUDIENCE_NOISY1-4 by crowd size)
 * unless it is already playing, panned to where the crowd sits; it never times out.
 * Marks the loop as started, which lets BattleAudienceNoiseMain give it volume.
 */
void BattleAudienceSoundNoiseAlways(void) {
    char name[64];
    BattleAudience* audience;
    BattleAudienceSound* sound;
    s32 right, left, sum;

    audience = BattleAudienceBaseGetPtr();
    sound = BattleAudienceSoundGetPtr(AUD_SOUND_NOISE);
    right = BattleAudience_GetPPAudienceNum_R();
    left = BattleAudience_GetPPAudienceNum_L();
    sum = right + left;
    if (!(sound->flags & AUD_SOUND_FLAG_PLAYING)) {
        if (sum == 0) {
            sound->kind = 0;
        }
        if (sum >= 1 && sum <= 49) {
            strcpy(name, "SFX_AUDIENCE_NOISY1");
            sound->kind = 1;
        }
        if (sum >= 50 && sum <= 99) {
            strcpy(name, "SFX_AUDIENCE_NOISY2");
            sound->kind = 2;
        }
        if (sum >= 100 && sum <= 149) {
            strcpy(name, "SFX_AUDIENCE_NOISY3");
            sound->kind = 3;
        }
        if (sum >= 150) {
            strcpy(name, "SFX_AUDIENCE_NOISY4");
            sound->kind = 4;
        }

        // BUG: with no member taking part (sum 0) name is never written, so an uninitialised buffer is played.
        sound->sfxId = BattleAudienceSound1(name, right, left);
        if (sound->sfxId != -1) {
            sound->flags |= AUD_SOUND_FLAG_PLAYING;
            sound->duration = -1;
            sound->fadeoutTime = 0;
            sound->baseSfxVolume = sound->currentBaseSfxVolume = psndSFX_get_vol(sound->sfxId);
            sound->forceFadeLength = 0;
            sound->forceFadeTimer = 0;
            sound->volumeMultiplier = 100;
            audience->flags |= AUDIENCE_FLAG_NOISE_ON;
        }
    }
}

/**
 * Plays the crowd's cheer (BattleAudienceSoundCheerKind) at the tier for the members taking part
 * (1-49, 50-99, 100-149, 150 and more) for duration frames, fading over the last fade.
 */
void BattleAudienceSoundCheer(s32 duration, s32 fade) {
    s32 kind;
    s32 num;

    BattleAudienceBaseGetPtr();
    kind = 0;
    num = BattleAudience_GetPPAudienceNum_R() + BattleAudience_GetPPAudienceNum_L();
    if (num >= 1 && num <= 49) {
        kind = 1;
    }
    if (num >= 50 && num <= 99) {
        kind = 2;
    }
    if (num >= 100 && num <= 149) {
        kind = 3;
    }
    if (num >= 150) {
        kind = 4;
    }
    BattleAudienceSoundCheerKind(kind, duration, fade);
}

/**
 * Plays cheer kind 1-4 (SFX_AUDIENCE_CHEER_ALLY1-4) for duration frames, fading over the last fade,
 * on one of two cheer channels (4 and 5) so two cheers can overlap:
 * a free one is used, the second cheer pitch-shifted, and when both are busy the one nearer its end is cut.
 * Called by BattleAudienceSoundCheer and sac_common.c.
 */
void BattleAudienceSoundCheerKind(s32 kind, s32 duration, s32 fade) {
    char name[64];
    BattleAudienceSound* sound;
    BattleAudienceSound* sound5;
    s32 right, left;
    s32 lowPitch;

    BattleAudienceBaseGetPtr();
    lowPitch = 0;
    right = BattleAudience_GetPPAudienceNum_R();
    left = BattleAudience_GetPPAudienceNum_L();
    sound = BattleAudienceSoundGetPtr(AUD_SOUND_CHEER_A);
    sound5 = BattleAudienceSoundGetPtr(AUD_SOUND_CHEER_B);
    if ((sound->flags & AUD_SOUND_FLAG_PLAYING) && (sound5->flags & AUD_SOUND_FLAG_PLAYING)) {
        if (sound->duration <= sound5->duration) {
            BattleAudienceSoundStop(AUD_SOUND_CHEER_A);
            sound = BattleAudienceSoundGetPtr(AUD_SOUND_CHEER_A);
        } else {
            BattleAudienceSoundStop(AUD_SOUND_CHEER_B);
            sound = BattleAudienceSoundGetPtr(AUD_SOUND_CHEER_B);
        }
    } else if (sound->flags & AUD_SOUND_FLAG_PLAYING) {
        sound = BattleAudienceSoundGetPtr(AUD_SOUND_CHEER_B);
        lowPitch = 1;
    } else if (sound5->flags & AUD_SOUND_FLAG_PLAYING) {
        sound = BattleAudienceSoundGetPtr(AUD_SOUND_CHEER_A);
        lowPitch = 1;
    } else {
        sound = BattleAudienceSoundGetPtr(AUD_SOUND_CHEER_A);
    }
    if (kind == 1) {
        strcpy(name, "SFX_AUDIENCE_CHEER_ALLY1");
        sound->kind = 1;
    }
    if (kind == 2) {
        strcpy(name, "SFX_AUDIENCE_CHEER_ALLY2");
        sound->kind = 2;
    }
    if (kind == 3) {
        strcpy(name, "SFX_AUDIENCE_CHEER_ALLY3");
        sound->kind = 3;
    }
    if (kind == 4) {
        strcpy(name, "SFX_AUDIENCE_CHEER_ALLY4");
        sound->kind = 4;
    }

    // BUG: kind 0 (BattleAudienceSoundCheer with no member taking part) never writes name, so an uninitialised
    // buffer is played.
    if (lowPitch == 1) {
        sound->sfxId = BattleAudienceSound1(name, right, left);
        psndSFX_pit(sound->sfxId, 0x7F);
    } else {
        sound->sfxId = BattleAudienceSound1(name, right, left);
    }
    AUD_SOUND_START(sound, duration + fade, fade);
}

/**
 * A whistle from a random idle member (BattleAudienceSoundWhistleKind with a random kind 1-3); at battle end.
 */
void BattleAudienceSoundWhistle(void) {
    BattleAudienceSoundWhistleKind(irand(3) + 1);
}

/**
 * Plays whistle kind 1-3 (SFX_AUDIENCE_WHISTLE1-3) at the seat of a random idle member,
 * on channel 6 for the left half of the stands or 7 for the right; nothing when nobody is idle.
 * Used by the Star Power special moves.
 */
void BattleAudienceSoundWhistleKind(s32 kind) {
    Vec pos;
    s32 id, idx;
    s32 list[200];
    s32 i;
    s32 count;
    BattleAudienceSound* sound;

    count = 0;
    for (i = 0; i < AUDIENCE_MEMBER_MAX; i++) {
        if (BattleAudience_GetWaiting(i) == TRUE) {
            list[count] = i;
            count++;
        }
    }
    if (count != 0) {
        idx = list[irand(count)];
        BattleAudienceSoundGetInfo2(idx, &pos);
        if (pos.x <= 0.0f) {
            id = AUD_SOUND_WHISTLE_L;
        } else {
            id = AUD_SOUND_WHISTLE_R;
        }
        sound = BattleAudienceSoundGetPtr(id);
        BattleAudienceSoundStop(id);
        switch (kind) {
            case 1:
                sound->sfxId = BattleAudienceSound2("SFX_AUDIENCE_WHISTLE1", idx);
                break;
            case 2:
                sound->sfxId = BattleAudienceSound2("SFX_AUDIENCE_WHISTLE2", idx);
                break;
            case 3:
                sound->sfxId = BattleAudienceSound2("SFX_AUDIENCE_WHISTLE3", idx);
                break;
        }
        AUD_SOUND_START(sound, -1, 0);
    }
}

/**
 * Plays crowd call kind 1 or 2 (SFX_AUDIENCE_CALL_ALLY1 or 2) on channel 8, panned and scaled to the crowd;
 * used by the Star Power special moves.
 */
void BattleAudienceSoundCallKind(s32 kind) {
    BattleAudienceSound* sound;
    s32 right, left;

    BattleAudienceBaseGetPtr();
    sound = BattleAudienceSoundGetPtr(AUD_SOUND_CALL);
    right = BattleAudience_GetPPAudienceNum_R();
    left = BattleAudience_GetPPAudienceNum_L();
    BattleAudienceSoundStop(AUD_SOUND_CALL);
    switch (kind) {
        case 1:
            sound->sfxId = BattleAudienceSound1("SFX_AUDIENCE_CALL_ALLY1", right, left);
            break;
        case 2:
            sound->sfxId = BattleAudienceSound1("SFX_AUDIENCE_CALL_ALLY2", right, left);
            break;
    }
    AUD_SOUND_START(sound, -1, 0);
}

/**
 * Starts the crowd's rhythmic clapping loop (SFX_AUDIENCE_HANDBEAT1) on channel 9 for the Star Power special
 * moves, louder the more members take part: 75 plus a random share, silent for an empty crowd.
 */
void BattleAudienceSoundHandBeat(void) {
    s32 right;
    BattleAudienceSound* sound;
    s32 left;
    s32 sum;
    u8 vol;

    BattleAudienceBaseGetPtr();
    sound = BattleAudienceSoundGetPtr(AUD_SOUND_HAND_BEAT);
    right = BattleAudience_GetPPAudienceNum_R();
    left = BattleAudience_GetPPAudienceNum_L();
    sum = right + left;
    BattleAudienceSoundStop(AUD_SOUND_HAND_BEAT);
    sound->sfxId = BattleAudienceSound1("SFX_AUDIENCE_HANDBEAT1", right, left);
    if (sound->sfxId != -1) {
        sound->flags |= AUD_SOUND_FLAG_PLAYING;
        sound->duration = -1;
        sound->fadeoutTime = 0;
        sound->forceFadeLength = 0;
        sound->forceFadeTimer = 0;
        if (sum == 0) {
            vol = 0;
        } else {
            vol = 75.0f + sum * (26.0f + irand(26)) / 200.0f;
        }

        psndSFX_vol(sound->sfxId, vol);
        sound->currentBaseSfxVolume = vol;
        sound->baseSfxVolume = vol;
        sound->volumeMultiplier = 100;
    }
}

/**
 * Plays a member's throw sound at its seat (kind 0 SFX_AUDIENCE_THROW_OBJECT1, else OBJECT2),
 * on channel 10 for the left half or 11 for the right; from the throwing status in battle_audience_kinopio.c.
 */
void BattleAudienceSoundItemThrow(s32 kind, s32 index) {
    Vec pos;
    s32 id;
    BattleAudienceSound* sound;

    BattleAudienceSoundGetInfo2(index, &pos);
    if (pos.x <= 0.0f) {
        id = AUD_SOUND_ITEM_THROW_L;
    } else {
        id = AUD_SOUND_ITEM_THROW_R;
    }
    sound = BattleAudienceSoundGetPtr(id);
    BattleAudienceSoundStop(id);
    switch (kind) {
        case 0:
            sound->sfxId = BattleAudienceSound2("SFX_AUDIENCE_THROW_OBJECT1", index);
            break;
        default:
            sound->sfxId = BattleAudienceSound2("SFX_AUDIENCE_THROW_OBJECT2", index);
            break;
    }
    AUD_SOUND_START(sound, -1, 0);
}

/**
 * Plays the crowd's running sound (kind 0 SFX_AUDIENCE_RUN1, 1 RUN2) on channel 13, not positioned;
 * the bingo slots play it when their result brings fans in or sends them away.
 */
void BattleAudienceSoundRun(s32 kind) {
    BattleAudienceSound* sound = BattleAudienceSoundGetPtr(AUD_SOUND_RUN);

    BattleAudienceSoundStop(AUD_SOUND_RUN);
    switch (kind) {
        case 0:
            sound->sfxId = BattleAudienceSound0("SFX_AUDIENCE_RUN1");
            break;
        case 1:
            sound->sfxId = BattleAudienceSound0("SFX_AUDIENCE_RUN2");
            break;
    }
    AUD_SOUND_START(sound, -1, 0);
}

/**
 * Plays a random sleep sound (SFX_AUDIENCE_SLEEP1-3) at member index as it dozes off
 * after a Crazee Dayzee's song, on channel 14 for the left half or 15 for the right.
 */
void BattleAudienceSoundSleep(s32 index) {
    Vec pos;
    s32 id;
    BattleAudienceSound* sound;

    BattleAudienceSoundGetInfo2(index, &pos);
    if (pos.x <= 0.0f) {
        id = AUD_SOUND_SLEEP_L;
    } else {
        id = AUD_SOUND_SLEEP_R;
    }
    sound = BattleAudienceSoundGetPtr(id);
    BattleAudienceSoundStop(id);
    switch (irand(3)) {
        case 0:
            sound->sfxId = BattleAudienceSound2("SFX_AUDIENCE_SLEEP1", index);
            break;
        case 1:
            sound->sfxId = BattleAudienceSound2("SFX_AUDIENCE_SLEEP2", index);
            break;
        case 2:
            sound->sfxId = BattleAudienceSound2("SFX_AUDIENCE_SLEEP3", index);
            break;
    }
    AUD_SOUND_START(sound, -1, 0);
}

/**
 * Plays the snoring sound (SFX_AUDIENCE_ZZZ1) at sleeping member index on channel 16.
 * The new sound's handle is not kept, so the channel still holds the previous one
 * and cannot stop or fade this sound.
 */
void BattleAudienceSoundZZZ(s32 index) {
    BattleAudienceSound* sound = BattleAudienceSoundGetPtr(AUD_SOUND_ZZZ);

    BattleAudienceSoundStop(AUD_SOUND_ZZZ);

    // BUG: the returned id is not stored in sound->sfxId, so AUD_SOUND_START tests and fetches the volume of the
    // slot's previous id.
    BattleAudienceSound2("SFX_AUDIENCE_ZZZ1", index);
    AUD_SOUND_START(sound, -1, 0);
}

/** Plays the Crazee Dayzee's song (SFX_AUDIENCE_PANSY_SINGING1) at member index on channel 17. */
void BattleAudienceSoundSing(s32 index) {
    BattleAudienceSound* sound = BattleAudienceSoundGetPtr(AUD_SOUND_SING);

    BattleAudienceSoundStop(AUD_SOUND_SING);
    sound->sfxId = BattleAudienceSound2("SFX_AUDIENCE_PANSY_SINGING1", index);
    AUD_SOUND_START(sound, -1, 0);
}

/**
 * Plays the Koopa's shell sound (SFX_AUDIENCE_SHELL_TRANSFORM1) at member index as it goes into
 * or comes out of its shell, on channel 18 for the left half or 19 for the right.
 */
void BattleAudienceSoundShell(s32 index) {
    Vec pos;
    s32 id;
    BattleAudienceSound* sound;

    BattleAudienceSoundGetInfo2(index, &pos);
    if (pos.x <= 0.0f) {
        id = AUD_SOUND_SHELL_L;
    } else {
        id = AUD_SOUND_SHELL_R;
    }
    sound = BattleAudienceSoundGetPtr(id);
    BattleAudienceSoundStop(id);
    sound->sfxId = BattleAudienceSound2("SFX_AUDIENCE_SHELL_TRANSFORM1", index);
    AUD_SOUND_START(sound, -1, 0);
}

/**
 * Plays a Bulky Bob-omb's burning fuse (SFX_AUDIENCE_BOMB_READY1) at member index on channel 20 for duration
 * frames, fading over the last fade (180 and 120 when it lights).
 */
void BattleAudienceSoundBombIgnite(s32 index, s32 duration, s32 fade) {
    BattleAudienceSound* sound = BattleAudienceSoundGetPtr(AUD_SOUND_BOMB_IGNITE);

    BattleAudienceSoundStop(AUD_SOUND_BOMB_IGNITE);
    sound->sfxId = BattleAudienceSound2("SFX_AUDIENCE_BOMB_READY1", index);
    AUD_SOUND_START(sound, duration + fade, fade);
}

/**
 * Plays a Bulky Bob-omb's blast (SFX_AUDIENCE_BOMB_BURST1) at member index,
 * on channel 21 for the left half or 22 for the right.
 */
void BattleAudienceSoundBombFire(s32 index) {
    Vec pos;
    s32 id;
    BattleAudienceSound* sound;

    BattleAudienceSoundGetInfo2(index, &pos);
    if (pos.x <= 0.0f) {
        id = AUD_SOUND_BOMB_FIRE_L;
    } else {
        id = AUD_SOUND_BOMB_FIRE_R;
    }
    sound = BattleAudienceSoundGetPtr(id);
    BattleAudienceSoundStop(id);
    sound->sfxId = BattleAudienceSound2("SFX_AUDIENCE_BOMB_BURST1", index);
    AUD_SOUND_START(sound, -1, 0);
}

/** Plays a Piranha Plant's bite (SFX_AUDIENCE_PAKU_EAT1) at member index on channel 23. */
void BattleAudienceSoundPakkunEat(s32 index) {
    BattleAudienceSound* sound = BattleAudienceSoundGetPtr(AUD_SOUND_PAKKUN_EAT);

    BattleAudienceSoundStop(AUD_SOUND_PAKKUN_EAT);
    sound->sfxId = BattleAudienceSound2("SFX_AUDIENCE_PAKU_EAT1", index);
    AUD_SOUND_START(sound, -1, 0);
}

/**
 * Fades crowd sound channel index from its current volume to vol percent over length frames
 * and cancels its timed fade-out; BattleAudienceSoundMain applies it.
 */
void BattleAudienceSoundSetVol(s32 index, u8 vol, s32 length) {
    BattleAudienceSound* sound;

    sound = BattleAudienceSoundGetPtr(index);
    sound->fadeStartVolumeMultiplier = sound->volumeMultiplier;
    sound->fadeEndVolumeMultiplier = vol;
    sound->forceFadeTimer = 0;
    sound->forceFadeLength = length;
    sound->fadeoutTime = 0;
}

/**
 * Fades all 24 crowd sound channels to vol percent over length frames; battle end silences the crowd with it.
 */
void BattleAudienceSoundSetVolAll(u8 vol, s32 length) {
    s32 i;

    for (i = 0; i < 24; i++) {
        BattleAudienceSoundSetVol(i, vol, length);
    }
}

/**
 * Nonzero while crowd sound channel index is playing; the Star Power special moves wait on it (sac_common.c).
 */
s32 BattleAudienceSoundCheck(s32 index) {
    return BattleAudienceSoundGetPtr(index)->flags & AUD_SOUND_FLAG_PLAYING;
}

/** Stops the sound on crowd channel index if it is playing. */
void BattleAudienceSoundStop(s32 index) {
    BattleAudienceSound* sound = BattleAudienceSoundGetPtr(index);

    if (sound->flags & AUD_SOUND_FLAG_PLAYING) {
        psndSFXOff(sound->sfxId);
        sound->flags &= ~AUD_SOUND_FLAG_PLAYING;
    }
}

/** Starts sound name unpositioned; returns its handle. */
static s32 BattleAudienceSound0(const char* name) {
    return psndSFXOn(name);
}

/**
 * Starts sound name for the whole crowd, placed and scaled by the members taking part on the right
 * and left (BattleAudienceSoundGetInfo1); returns its handle.
 */
static s32 BattleAudienceSound1(const char* name, s32 right, s32 left) {
    u8 vol;
    Vec pos;
    s32 id;

    BattleAudienceSoundGetInfo1(right, left, &vol, &pos);
    id = psndSFXOn_3D(name, &pos);
    psndSFX_vol(id, vol);
    return id;
}

/**
 * Works out a crowd-wide sound's volume and position from the members taking part on each half:
 * volume 100-126 with the crowd size (0 for nobody), x 250 when nobody is on the left,
 * else 250 times (right - left) / total. The x passes through a u8,
 * so a crowd heavier on the left wraps to a positive value.
 */
static void BattleAudienceSoundGetInfo1(s32 right, s32 left, u8* vol, Vec* pos) {
    s32 sum = right + left;

    if (sum == 0) {
        *vol = 0;
    } else {
        *vol = 100.0f + 26.0f * sum / 200.0f;
    }
    if (left == 0 || sum == 0) {
        pos->x = 250.0f;
    } else {
        // BUG: the (u8) cast drops the sign: with more members on the left (left > right) the value wraps to a
        // positive x instead of a negative one.
        pos->x = (u8)(250.0f * (right - left) / sum);
    }
    pos->y = 0.0f;
    pos->z = 0.0f;
}

/** Starts sound name at member index's position; returns its handle. */
static s32 BattleAudienceSound2(const char* name, s32 index) {
    Vec pos;

    BattleAudienceSoundGetInfo2(index, &pos);
    return psndSFXOn_3D(name, &pos);
}

/**
 * Gives member index's current position, where its sounds play; its x also picks the left or right channel.
 */
static void BattleAudienceSoundGetInfo2(s32 index, Vec* pos) {
    BattleAudienceMember* member = BattleAudienceGetPtr(index);

    pos->x = member->position.x;
    pos->y = member->position.y;
    pos->z = member->position.z;
}

/**
 * Per-frame update of the 24 crowd sound channels (from BattleAudience_Main):
 * runs the noise loop (BattleAudienceNoiseMain), drops channels whose sound has ended,
 * stops a sound whose duration has run out, applies a volume fade from BattleAudienceSoundSetVol
 * and fades a timed sound out over its last fadeoutTime frames.
 * A duration of -1 plays until the sound ends or is stopped.
 */
static void BattleAudienceSoundMain(void) {
    BattleAudienceSound* sound;
    s32 i;

    BattleAudienceBaseGetPtr();
    BattleAudienceNoiseMain();
    for (i = 0, sound = BattleAudienceSoundGetPtr(AUD_SOUND_BOOING); i < 24; i++, sound++) {
        if (psndSFXChk(sound->sfxId) == -1) {
            sound->flags &= ~AUD_SOUND_FLAG_PLAYING;
            continue;
        }
        if (sound->duration == 0) {
            psndSFXOff(sound->sfxId);
            sound->flags &= ~AUD_SOUND_FLAG_PLAYING;
            continue;
        }
        if (sound->forceFadeLength > 0) {
            sound->forceFadeTimer++;
            sound->volumeMultiplier = intplGetValue(EASING_LINEAR, sound->fadeStartVolumeMultiplier, sound->fadeEndVolumeMultiplier, sound->forceFadeTimer, sound->forceFadeLength);
            psndSFX_vol(sound->sfxId, (u8)(sound->currentBaseSfxVolume * sound->volumeMultiplier / 100));
            if (sound->forceFadeTimer >= sound->forceFadeLength) {
                sound->forceFadeLength = 0;
            }
        }
        if (sound->duration > 0) {
            if (sound->duration == sound->fadeoutTime) {
                sound->fadeStartVolumeMultiplier = sound->volumeMultiplier;
                sound->fadeEndVolumeMultiplier = 0;
            }
            if (sound->duration <= sound->fadeoutTime) {
                sound->volumeMultiplier = intplGetValue(EASING_LINEAR, sound->fadeStartVolumeMultiplier, sound->fadeEndVolumeMultiplier, sound->fadeoutTime - sound->duration, sound->fadeoutTime);
                psndSFX_vol(sound->sfxId, (u8)(sound->currentBaseSfxVolume * sound->volumeMultiplier / 100));
            }
            sound->duration--;
        }
    }
}

/**
 * Keeps the crowd-noise loop (channel 2) in step with the crowd:
 * fades it to 60% when command input opens and to silence when it closes (80 frames),
 * switches it off when the stands empty and asks for it again
 * when the size tier (1-49, 50-99, 100-149, 150-200) changes,
 * and sets its base volume to 63-126 with the members taking part (0 until the loop has started).
 */
static void BattleAudienceNoiseMain(void) {
    BattleWork* wp = _battleWorkPointer;
    BattleAudience* audience = BattleAudienceBaseGetPtr();
    BattleAudienceSound* sound = BattleAudienceSoundGetPtr(AUD_SOUND_NOISE);
    s32 num = BattleAudience_GetPPAudienceNum();

    if (!(audience->flags & AUDIENCE_FLAG_PREV_COMMAND_INPUT) && (wp->flags & BATTLE_FLAG_COMMAND_INPUT)) {
        BattleAudienceSoundSetVol(AUD_SOUND_NOISE, 60, 80);
    }
    if ((audience->flags & AUDIENCE_FLAG_PREV_COMMAND_INPUT) && !(wp->flags & BATTLE_FLAG_COMMAND_INPUT)) {
        BattleAudienceSoundSetVol(AUD_SOUND_NOISE, 0, 80);
    }
    if (num == 0 && sound->kind != 0) {
        psndSFXOff(sound->sfxId);
        sound->kind = 0;
    }
    if (num >= 1 && num <= 49 && sound->kind != 1) {
        BattleAudienceSoundNoiseAlways();
        sound->kind = 1;
    }
    if (num >= 50 && num <= 99 && sound->kind != 2) {
        BattleAudienceSoundNoiseAlways();
        sound->kind = 2;
    }
    if (num >= 100 && num <= 149 && sound->kind != 3) {
        BattleAudienceSoundNoiseAlways();
        sound->kind = 3;
    }
    if (num >= 150 && num <= 200 && sound->kind != 4) {
        BattleAudienceSoundNoiseAlways();
        sound->kind = 4;
    }
    if (!(audience->flags & AUDIENCE_FLAG_NOISE_ON)) {
        sound->currentBaseSfxVolume = 0;
    } else if (num == 0) {
        sound->currentBaseSfxVolume = 0;
    } else {
        sound->currentBaseSfxVolume = 63.0f + 63.0f * num / 200.0f;
    }
}

/**
 * Notes that the action defeated an enemy (level is unused);
 * a failed action command then costs no fans (BattleAudience_PerAct). From the damage code.
 */
void BattleAudience_Case_KillEnemy(s32 level) {
    BattleAudienceBaseGetPtr()->flags |= AUDIENCE_FLAG_KILL_ENEMY;
}

/**
 * Crowd reaction to a successful action command (battle_ac.c):
 * if any member is idle, a 180-frame cheer fading over 120 and everyone raises their arms;
 * records the success for BattleAudience_PerAct. The early demos also count it as one Stylish command
 * and clears the missed-Stylish flag.
 */
void BattleAudience_Case_ActionCommandGood(BattleWeapon* weapon) {
    BattleAudience* audience = BattleAudienceBaseGetPtr();
    s32 i;
    s32 count = 0;
#if VERSION <= VERSION_DEMO_JP
    audience->numStylishCommandsThisAttack = 1;
#endif
    for (i = 0; i < AUDIENCE_MEMBER_MAX; i++) {
        if (BattleAudience_GetWaiting(i) == TRUE) {
            count++;
        }
    }
    if (count != 0) {
        BattleAudienceSoundCheer(180, 120);
        BattleAudienceJoy(0);
    }
    audience->flags |= AUDIENCE_FLAG_AC_GOOD;
#if VERSION <= VERSION_DEMO_JP
    audience->flags &= ~AUDIENCE_FLAG_STYLISH_MISSED;
#endif
}

/**
 * Crowd reaction to a failed action command (battle_ac.c):
 * if any member is idle, booing and the arms-up reaction,
 * and after a failed hammer attack a Hammer Bro in the crowd may throw a hammer next phase;
 * records the failure for BattleAudience_PerAct.
 */
void BattleAudience_Case_ActionCommandBad(BattleWeapon* weapon) {
    BattleAudience* audience = BattleAudienceBaseGetPtr();
    s32 count = 0;
    s32 i;

    for (i = 0; i < AUDIENCE_MEMBER_MAX; i++) {
        if (BattleAudience_GetWaiting(i) == TRUE) {
            count++;
        }
    }
    if (count != 0) {
        BattleAudienceSoundBooing();
        BattleAudienceJoy(0);
        if (weapon->hpDamageFn == weaponGetPowerFromMarioHammerLv) {
            BattleAudienceAddPhaseEvtList(7);
        }
    }
    audience->flags |= AUDIENCE_FLAG_AC_BAD;
}

/** Notes a successful guard or Superguard for BattleAudience_PerAct (from the damage code). */
void BattleAudience_Case_GuardGood(void) {
    BattleAudienceBaseGetPtr()->flags |= AUDIENCE_FLAG_GUARD_GOOD;
}

/** Notes a failed guard for BattleAudience_PerAct, which ends the pleased streak (from the damage code). */
void BattleAudience_Case_GuardBad(void) {
    BattleAudienceBaseGetPtr()->flags |= AUDIENCE_FLAG_GUARD_BAD;
}

/**
 * Crowd reaction to a Stylish command (battle event commands):
 * if anyone is idle, a cheer and clapping (180 frames, fading over 120) and everyone hops;
 * counts the Stylish command (up to 5 per action) for BattleAudience_PerAct.
 * The early demos only flag the success and clear the missed-Stylish flag.
 */
void BattleAudience_Case_AcrobatGood(void) {
    BattleAudience* audience = BattleAudienceBaseGetPtr();
    s32 count = 0;
    s32 i;

    for (i = 0; i < AUDIENCE_MEMBER_MAX; i++) {
        if (BattleAudience_GetWaiting(i) == TRUE) {
            count++;
        }
    }
    if (count >= 1) {
        BattleAudienceSoundCheer(180, 120);
        BattleAudienceSoundClap(180, 120);
        BattleAudienceJoy(1);
    }
#if VERSION >= VERSION_JP
    audience->numStylishCommandsThisAttack++;
    if (audience->numStylishCommandsThisAttack > 5) {
        audience->numStylishCommandsThisAttack = 5;
    }
#endif
    audience->flags |= AUDIENCE_FLAG_ACROBAT_GOOD;
#if VERSION <= VERSION_DEMO_JP
    audience->flags &= ~AUDIENCE_FLAG_STYLISH_MISSED;
#endif
}

/**
 * Crowd reaction to a failed Stylish command (battle event commands):
 * if anyone is idle, booing and the arms-up reaction;
 * flags it for BattleAudience_PerAct (the early demos also note the miss).
 */
void BattleAudience_Case_AcrobatBad(void) {
    BattleAudience* audience = BattleAudienceBaseGetPtr();
    s32 count = 0;
    s32 i;

    for (i = 0; i < AUDIENCE_MEMBER_MAX; i++) {
        if (BattleAudience_GetWaiting(i) == TRUE) {
            count++;
        }
    }
    if (count >= 1) {
        BattleAudienceSoundBooing();
        BattleAudienceJoy(0);
    }
    audience->flags |= AUDIENCE_FLAG_ACROBAT_BAD;
#if VERSION <= VERSION_DEMO_JP
    audience->flags |= AUDIENCE_FLAG_STYLISH_MISSED;
#endif
}

/**
 * Called by a script command when a Stylish chance passed untried;
 * only the early demos act on it, clearing the missed-Stylish flag.
 */
void BattleAudience_Case_AcrobatNoTry(void) {
#if VERSION <= VERSION_DEMO_JP
    BattleAudienceBaseGetPtr()->flags &= ~AUDIENCE_FLAG_STYLISH_MISSED;
#else
    BattleAudienceBaseGetPtr();
#endif
}

/** Hook for Mario taking damage (from the damage code); the crowd does nothing with it. */
void BattleAudience_Case_MarioBigDamage(s32 damage) {
    BattleAudienceBaseGetPtr();
}

/**
 * Mario has just fallen into Danger (BtlUnit_CheckPinchStatus):
 * the target size grows by 5% of the level's audience factor.
 */
void BattleAudience_Case_MarioPinch(void) {
    BattleAudienceAddTargetNum(0.049999997f * BattleAudienceBaseGetPtr()->audienceDeltaMultiplier, 0.0f);
}

/**
 * Mario has just fallen into Peril, despite the name (BtlUnit_CheckPinchStatus):
 * the target size grows by 15% of the level's audience factor.
 */
void BattleAudience_Case_MarioDanger(void) {
    BattleAudienceAddTargetNum(0.14999999f * BattleAudienceBaseGetPtr()->audienceDeltaMultiplier, 0.0f);
}

/** A partner was knocked out (damage code): the target size drops by 25% of the level's audience factor. */
void BattleAudience_Case_PartyDown(void) {
    BattleAudienceAddTargetNum(-0.25f * BattleAudienceBaseGetPtr()->audienceDeltaMultiplier, 0.0f);
}

/** Empty hook for a new Power Bounce record (battle_mario.c). */
void BattleAudience_Case_JumpNewRecord(void) {
}

/** Empty hook for an enemy being defeated (from the damage code). */
void BattleAudience_Case_FinalAttack(void) {
}

/**
 * End-of-turn audience update (battle_seq.c), skipped while frozen.
 * Once the battle has run for twice the enemies' total level divided by Mario's level in turns (at least 5),
 * the crowd shrinks by 5% of the level's audience factor each turn,
 * and on that turn an X-Naut throws an item (the early demos queue that every late turn).
 * The chance of the bigger phase reactions grows by 3% per turn;
 * when it comes up, the Boo, Shy Guy run, Crazee Dayzee, Bulky Bob-omb,
 * Piranha Plant and Luigi reactions are queued, the Koopas too late in battle, and the chance resets.
 * Finally the crowd moves toward its target size.
 */
void BattleAudience_Case_TurnEnd(void) {
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* mario = BattleGetMarioPtr(wp);
    BattleAudience* audience = BattleAudienceBaseGetPtr();
    s32 i, sum, turns;
    BattleGroupSetup* group;
    BattleUnitSetup* setup;

    pouchGetPtr();
    sum = 0;
    group = wp->fbatInfo->setup->groupSetup;
    setup = group->enemyData;
    if (!(audience->flags & AUDIENCE_FLAG_EVENT_CTRL)) {
        audience->turnEndPhaseEventChance += 3;
        if (audience->turnEndPhaseEventChance > 100) {
            audience->turnEndPhaseEventChance = 100;
        }
        for (i = 0; i < group->numEnemies; i++, setup++) {
            sum += setup->unitKindParams->level;
        }
        turns = sum * 2 / mario->level;
        if (turns < 5) {
            turns = 5;
        }
        if (wp->turnCount >= turns) {
            BattleAudienceAddTargetNum(-0.049999997f * BattleAudienceBaseGetPtr()->audienceDeltaMultiplier, 0.0f);
#if VERSION >= VERSION_JP
        }
        if (wp->turnCount == turns) {
#endif
            BattleAudienceAddPhaseEvtList(6);
        }
        if (irand(100) < audience->turnEndPhaseEventChance) {
            if (wp->turnCount >= turns) {
                BattleAudienceAddPhaseEvtList(1);
            }
            BattleAudienceAddPhaseEvtList(4);
            BattleAudienceAddPhaseEvtList(9);
            BattleAudienceAddPhaseEvtList(10);
            BattleAudienceAddPhaseEvtList(11);
            BattleAudienceAddPhaseEvtList(12);
            BattleAudienceAddPhaseEvtList(13);
            audience->turnEndPhaseEventChance = 0;
        }
        BattleAudienceNumToTarget();
    }
}

/**
 * Crowd reaction to an Appeal (battle event commands):
 * with anyone taking part, Star Power gain of a quarter of the crowd plus 25, plus 25 per Super Appeal badge,
 * a 180-frame cheer and clapping fading over 60 and the arms-up reaction;
 * the target size grows by 5% of the level's audience factor either way.
 * The early demos also count the Appeal as a Stylish command.
 */
void BattleAudience_Case_Appeal(BattleUnit* unit) {
    BattleAudience* audience = BattleAudienceBaseGetPtr();
#if VERSION <= VERSION_DEMO_JP
    if (BattleAudience_GetPPAudienceNum() > 0) {
#else
    if (BattleAudience_GetPPAudienceNum() > 0.0f) {
        audience->starPowerGain += (s32)(BattleAudience_GetPPAudienceNum() / 4.0f) + 25;
#endif
        if (unit->badgesEquipped.SuperAppeal) {
#if VERSION <= VERSION_DEMO_JP
            audience->starPowerGain += BattleAudience_GetPPAudienceNum() / 4 + 25 + unit->badgesEquipped.SuperAppeal * 25;
        } else {
            audience->starPowerGain += BattleAudience_GetPPAudienceNum() / 4 + 25;
#else
            audience->starPowerGain += unit->badgesEquipped.SuperAppeal * 25;
#endif
        }
#if VERSION <= VERSION_DEMO_JP
        audience->numStylishCommandsThisAttack = 1;
#endif
        BattleAudienceSoundCheer(180, 60);
        BattleAudienceSoundClap(180, 60);
        BattleAudienceJoy(0);
    }
    BattleAudienceAddTargetNum(0.049999997f * BattleAudienceBaseGetPtr()->audienceDeltaMultiplier, 0.0f);
}

/** Empty hook for the start of a run-away attempt (battle_event_default.c). */
void BattleAudience_Case_Escape(void) {
}

/**
 * Mario's side ran away: the target size drops by 10% of the level's audience factor,
 * the crowd noise swells to full and the crowd boos.
 */
void BattleAudience_Case_Escape_Good(void) {
    BattleAudienceAddTargetNum(-0.099999994f * BattleAudienceBaseGetPtr()->audienceDeltaMultiplier, 0.0f);
    BattleAudienceSoundSetVol(AUD_SOUND_NOISE, 100, 30);
    BattleAudienceSoundBooing();
}

/**
 * A run-away attempt failed: the target size drops by 20% of the level's audience factor and the crowd boos.
 */
void BattleAudience_Case_Escape_Bad(void) {
    BattleAudienceAddTargetNum(-0.19999999f * BattleAudienceBaseGetPtr()->audienceDeltaMultiplier, 0.0f);
    BattleAudienceSoundBooing();
}

/** Notes that the action damaged an enemy or gave it a status (damage is unused); from the damage code. */
void BattleAudience_Case_EnemyDamage(s32 damage) {
    BattleAudienceBaseGetPtr()->flags |= AUDIENCE_FLAG_ENEMY_DAMAGE;
}

/**
 * Notes that an attack hit an enemy for no damage and no status;
 * unless another hit did damage, BattleAudience_PerAct takes 5% off and the X-Nauts may throw rocks.
 */
void BattleAudience_Case_EnemyNoDamage(void) {
    BattleAudienceBaseGetPtr()->flags |= AUDIENCE_FLAG_ENEMY_NO_DAMAGE;
}

/**
 * An attack by Mario's side was countered: the target size drops by 10% of the level's audience factor
 * and the X-Nauts may throw rocks next phase (from the damage code).
 */
void BattleAudience_Case_Countered(void) {
    BattleAudienceAddTargetNum(-0.099999994f * BattleAudienceBaseGetPtr()->audienceDeltaMultiplier, 0.0f);
    BattleAudienceAddPhaseEvtList(5);
}

/**
 * A background set fell onto the stage (haikei = background; battle_stage_object.c):
 * the target size grows by 5% of the level's audience factor.
 */
void BattleAudience_Case_HaikeiSet(void) {
    BattleAudienceAddTargetNum(0.049999997f * BattleAudienceBaseGetPtr()->audienceDeltaMultiplier, 0.0f);
}

/**
 * Won a battle whose level is more than one above Mario's:
 * the target size grows by 15% of the level's audience factor (battle_seq_end.c).
 */
void BattleAudience_Case_GreatVictory(void) {
    BattleAudienceAddTargetNum(0.14999999f * BattleAudienceBaseGetPtr()->audienceDeltaMultiplier, 0.0f);
}

/**
 * Won quickly: the target size grows by 5% of the level's audience factor per turn under par,
 * rate being the turns saved (battle_seq_end.c).
 */
void BattleAudience_Case_FastVictory(f32 rate) {
    BattleAudienceAddTargetNum(0.01f * (5.0f * rate) * BattleAudienceBaseGetPtr()->audienceDeltaMultiplier, 0.0f);
}

/** A stage object fell onto the stage: the target size grows by 5% of the level's audience factor. */
void BattleAudience_Case_FallObject_Stage(void) {
    BattleAudienceAddTargetNum(0.049999997f * BattleAudienceBaseGetPtr()->audienceDeltaMultiplier, 0.0f);
}

/**
 * A stage object of the given type fell into the stands on seat index (battle_stage_object.c).
 * The member hit is knocked back unless it is a Shy Guy out on the stage,
 * and nearby members that may leave flee, each taking one off the target size:
 * the 8 seats around it for most objects, a 24-seat diamond for type 7,
 * the whole crowd for type 6 (which also takes one more off); other types hit the one seat only.
 * The stage code never drops types 6 and 7 into the stands. Then the Punies run away.
 */
void BattleAudience_Case_FallObject_Aud(s32 index, u8 type) {
    BattleAudience* audience = BattleAudienceBaseGetPtr();
    s32 near[8][2] = {
        {-1, 1}, {0, 1}, {1, 1}, {-1, 0}, {1, 0}, {-1, -1}, {0, -1}, {1, -1},
    };
    s32 far[24][2] = {
        {0, 3}, {-1, 2}, {0, 2}, {1, 2}, {-2, 1}, {-1, 1}, {0, 1}, {1, 1},
        {2, 1}, {-3, 0}, {-2, 0}, {-1, 0}, {1, 0}, {2, 0}, {3, 0}, {-2, -1},
        {-1, -1}, {0, -1}, {1, -1}, {2, -1}, {-1, -2}, {0, -2}, {1, -2}, {0, -3},
    };
    s32 i, no;

    switch (type) {
        case STAGE_FALL_BASIN:
        case STAGE_FALL_BUCKET:
        case STAGE_FALL_LARGE_BUG:
        case STAGE_FALL_FORK:
        case STAGE_FALL_WATER:
        case STAGE_FALL_STAGE_LIGHT:
            if ((u8)BattleAudience_GetSysCtrl(index) == TRUE && BattleAudienceGetPtr(index)->status != AUDIENCE_STATUS_INTRUDE) {
                BattleAudience_ChangeStatus(index, AUDIENCE_STATUS_DAMAGE);
            }
            for (i = 0; i < 8; i++) {
                no = BattleAudience_GetAudienceNoFromOffset(index, near[i][0], near[i][1]);
                if (no != -1 && (u8)BattleAudience_GetEscapeChangeOK(no)) {
                    BattleAudience_ChangeStatus(no, AUDIENCE_STATUS_ESCAPE);
                    audience->targetAudienceCount -= 1.0f;
                }
            }
            break;
        case STAGE_FALL_METEOR:
            if ((u8)BattleAudience_GetSysCtrl(index) == TRUE && BattleAudienceGetPtr(index)->status != AUDIENCE_STATUS_INTRUDE) {
                BattleAudience_ChangeStatus(index, AUDIENCE_STATUS_DAMAGE);
            }
            for (i = 0; i < 24; i++) {
                no = BattleAudience_GetAudienceNoFromOffset(index, far[i][0], far[i][1]);
                if (no != -1 && (u8)BattleAudience_GetEscapeChangeOK(no)) {
                    BattleAudience_ChangeStatus(no, AUDIENCE_STATUS_ESCAPE);
                    audience->targetAudienceCount -= 1.0f;
                }
            }
            break;
        case STAGE_FALL_BOWSER_STATUE:
            if ((u8)BattleAudience_GetSysCtrl(index) == TRUE && BattleAudienceGetPtr(index)->status != AUDIENCE_STATUS_INTRUDE) {
                BattleAudience_ChangeStatus(index, AUDIENCE_STATUS_DAMAGE);
            }
            for (i = 0; i < AUDIENCE_MEMBER_MAX; i++) {
                if ((u8)BattleAudience_GetEscapeChangeOK(i)) {
                    BattleAudience_ChangeStatus(i, AUDIENCE_STATUS_ESCAPE);
                    audience->targetAudienceCount -= 1.0f;
                }
            }

            /* fallthrough */
        default:
            if ((u8)BattleAudience_GetSysCtrl(index) == TRUE && BattleAudienceGetPtr(index)->status != AUDIENCE_STATUS_INTRUDE) {
                BattleAudience_ChangeStatus(index, AUDIENCE_STATUS_DAMAGE);
            }
            audience->targetAudienceCount -= 1.0f;
            break;
    }
    BattleAudiencePuniAllEscape();
}

/** Mario's side struck first: the target size grows by 2% of the level's audience factor (battle_seq.c). */
void BattleAudience_Case_FirstAttack_Good(void) {
    BattleAudienceAddTargetNum(0.02f * BattleAudienceBaseGetPtr()->audienceDeltaMultiplier, 0.0f);
}

/** The enemy struck first: the target size drops by 5% of the level's audience factor (battle_seq.c). */
void BattleAudience_Case_FirstAttack_Bad(void) {
    BattleAudienceAddTargetNum(-0.049999997f * BattleAudienceBaseGetPtr()->audienceDeltaMultiplier, 0.0f);
}

/**
 * Stores the item a landed audience throw delivers (0 once used up); set by BattleAudienceItemCtrlProcess.
 */
void BattleAudience_SetPresentItemNo(s32 no) {
    BattleAudienceBaseGetPtr()->presentItemNo = no;
}

/**
 * Returns the item a landed audience throw delivers;
 * read by the audience item scripts and the command sequence.
 */
s32 BattleAudience_GetPresentItemNo(void) {
    return BattleAudienceBaseGetPtr()->presentItemNo;
}

/** Stores the unit a landed audience throw goes to (BattleAudienceItemCtrlProcess). */
void BattleAudience_SetPresentTargetUnitId(s32 unitId) {
    BattleAudienceBaseGetPtr()->presentTargetUnitId = unitId;
}

/**
 * Returns the unit a landed audience throw goes to; read by the audience item
 * scripts and the command sequence.
 */
s32 BattleAudience_GetPresentTargetUnitId(void) {
    return BattleAudienceBaseGetPtr()->presentTargetUnitId;
}

/**
 * Stores whether a landed audience throw is a gift (0, a consumable item)
 * or junk that does damage (1: can, rock, bone, hammer).
 */
void BattleAudience_SetPresentItemType(s32 type) {
    BattleAudienceBaseGetPtr()->presentItemType = type;
}

/**
 * Returns 0 for a gift or 1 for a damaging throw; btlataudGetMoveEventPtr picks the target's script by it.
 */
s32 BattleAudience_GetPresentItemType(void) {
    return BattleAudienceBaseGetPtr()->presentItemType;
}
