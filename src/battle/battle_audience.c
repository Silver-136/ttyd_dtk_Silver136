#include "battle/battle_audience.h"
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
#include "eff/eff_magic1_n64.h"
EVT_BEGIN(msg_puni_all_escape)
    CALL(btlevtcmd_AnnounceMessage, 0, 0, -225, STRING("btl_msg_audience_puni_escape"), 120)
    RETURN
EVT_END()
EVT_BEGIN(msg_puni_all_enter)
    CALL(btlevtcmd_AnnounceMessage, 0, 0, -225, STRING("btl_msg_audience_puni_meeting"), 120)
    RETURN
EVT_END()
EVT_BEGIN(msg_pansy_sing)
    CALL(btlevtcmd_AnnounceMessage, 0, 0, -225, STRING("btl_msg_audience_pansy_sleep"), 120)
    RETURN
EVT_END()
EVT_BEGIN(msg_heavy_bomb_fire)
    CALL(btlevtcmd_AnnounceMessage, 0, 0, -225, STRING("btl_msg_audience_heavybomb_dynamite"), 120)
    RETURN
EVT_END()
typedef struct { f32 v[65]; } AudDeltaArr;
typedef struct { s32 v[11]; } AudWArr;
const AudDeltaArr R_audience_delta_multiplier = {{8.0f,11.0f,14.0f,17.0f,20.0f,23.0f,26.0f,29.0f,32.0f,35.0f,38.0f,41.0f,44.0f,47.0f,50.0f,53.0f,56.0f,59.0f,62.0f,65.0f,68.0f,71.0f,74.0f,77.0f,80.0f,83.0f,86.0f,89.0f,92.0f,95.0f,98.0f,101.0f,104.0f,107.0f,110.0f,113.0f,116.0f,119.0f,122.0f,125.0f,128.0f,131.0f,134.0f,137.0f,140.0f,143.0f,146.0f,149.0f,152.0f,155.0f,158.0f,161.0f,164.0f,167.0f,170.0f,173.0f,176.0f,179.0f,182.0f,185.0f,188.0f,191.0f,194.0f,197.0f,200.0f}};
const AudWArr R_num_audience_type_weights = {{20,40,20,5,3,2,1,1,1,1,1}};

// BattleAudienceSound with the leading fields named (the header keeps the raw header[] for sac_common)
typedef struct AudSound {
    s32 flags;                     // 0x0
    s32 sfxId;                     // 0x4
    u8 kind;                       // 0x8
    u8 pad_09[0xC - 0x9];       // 0x9, never accessed
    s32 duration;                  // 0xC
    s32 mFadeoutTime;              // 0x10
    s32 mForceFadeTimer;           // 0x14
    s32 mForceFadeLength;          // 0x18
    u8 mBaseSfxVolume;             // 0x1C
    u8 mCurrentBaseSfxVolume;      // 0x1D
    u8 mVolumeMultiplier;          // 0x1E
    u8 mFadeStartVolumeMultiplier; // 0x1F
    u8 mFadeEndVolumeMultiplier;   // 0x20
    u8 pad_21[3];              // 0x21, never accessed
} AudSound;

#define AudSoundGetPtr(id) ((AudSound*)BattleAudienceSoundGetPtr(id))

#define AUD_SOUND_START(sound, dur, fade)                                                           \
    if ((sound)->sfxId != -1) {                                                                     \
        (sound)->flags |= 1;                                                                        \
        (sound)->duration = (dur);                                                                  \
        (sound)->mFadeoutTime = (fade);                                                             \
        (sound)->mBaseSfxVolume = (sound)->mCurrentBaseSfxVolume = psndSFX_get_vol((sound)->sfxId); \
        (sound)->mForceFadeLength = 0;                                                              \
        (sound)->mForceFadeTimer = 0;                                                               \
        (sound)->mVolumeMultiplier = 100;                                                           \
    }

void psndSFX_vol(s32 id, s32 vol);
void psndSFX_pit(s32 id, s32 pitch);

// local prototypes
FileEntry* tplRead(const char* path);
u8 check_exe_phase_evt_status(s32 index, s32 type);
void BattleAudienceSettingAudience(void);
void BattleAudienceGuestTPLRead(s32 index, AudienceMemberType member, const char* tplName);
void BattleAudienceCtrlProcess(void);
void BattleAudienceSetThrowItemMax(void);
BattleUnit* BattleAudienceDetectTargetPlayer(void);
void BattleAudienceItemCtrlProcess(void);
void BattleAudienceApSrcCtrlProcess(void);
BOOL BattleAudienceApSrcEntry(void);
void BattleAudienceWinCtrlProcess(void);
void BattleAudienceGXInit(void);
void BattleAudienceAnimProcess(void);
void BattleAudienceDispAudience(CameraId camId, void* param);
void BattleAudienceDispItem(CameraId camId, void* param);
void BattleAudienceDispApSrc(CameraId camId, void* param);
void BattleAudienceDispWin(CameraId camId, void* param);
void BattleAudience_Entry_Sub(s32 index, s32 type, u8 status);
BOOL BattleAudience_GetEscapeChangeOK(s32 index);
s32 BattleAudience_NoUsedHaitiRand(void);
s32 BattleAudience_NoUsedFCHaitiRand(void);
s32 BattleAudience_GetPPAudienceNum(void);
s32 BattleAudience_GetPPAudienceNumKind(s32 type);
s32 BattleAudience_GetPPAudienceNum_R(void);
s32 BattleAudience_GetPPAudienceNum_L(void);
s32 BattleAudience_GetPPAudienceNum_RL_Sub(s32 offset);
BOOL BattleAudience_GetPPAudienceNum_Sub(s32 id);
double sqrt(double);
s32 BattleAudienceNumToTargetSub(void);
void BattleAudienceAddTargetNumSub(f32 value);
void BattleAudienceAddPuni(s32 num);
void BattleAudienceAddPhaseEvtList(s32 type);
void BattleAudienceCheer(s32 type);
void BattleAudienceJoy(s32 type);
void BattleAudienceJoy_Sub(s32 a, s32 b, s32 c, s32 type);
s32 BattleAudienceDetectPakkunEatTargetSub2(s32 index, s32 dir);
s32 BattleAudienceDetectPakkunEatTargetSub(s32 index, s32 dir1, s32 dir2);
void BattleAudienceSoundNoiseAlways(void);
s32 BattleAudienceSound0(const char* name);
s32 BattleAudienceSound1(const char* name, s32 right, s32 left);
void BattleAudienceSoundGetInfo1(s32 right, s32 left, u8* vol, Vec* pos);
s32 BattleAudienceSound2(const char* name, s32 index);
void BattleAudienceSoundGetInfo2(s32 index, Vec* pos);
void BattleAudienceSoundMain(void);
void BattleAudienceNoiseMain(void);

#define AUD_U8CALL(fn, i) (((u8 (*)(s32))(fn))(i))

extern ItemData itemDataTable[];

#pragma force_active on

BattleWeapon weapon_data_akikan = {
    NULL, 0, // name..icon
    {0}, // pad_06
    236, // itemId
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
    0x20000000, // targetPropertyFlags
    ELEMENT_NORMAL, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    0x48, // special
    COUNTER_RESIST_ALL, // counterResistFlags
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

BattleWeapon weapon_data_brick = {
    NULL, 0, // name..icon
    {0}, // pad_06
    237, // itemId
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
    0x20000000, // targetPropertyFlags
    ELEMENT_NORMAL, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    0x48, // special
    COUNTER_RESIST_ALL, // counterResistFlags
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

BattleWeapon weapon_data_bone = {
    NULL, 0, // name..icon
    {0}, // pad_06
    238, // itemId
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
    0x20000000, // targetPropertyFlags
    ELEMENT_NORMAL, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    0x48, // special
    COUNTER_RESIST_ALL, // counterResistFlags
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

BattleWeapon weapon_data_hammer = {
    NULL, 0, // name..icon
    {0}, // pad_06
    239, // itemId
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
    0x20000000, // targetPropertyFlags
    ELEMENT_NORMAL, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    0x48, // special
    COUNTER_RESIST_ALL, // counterResistFlags
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
#pragma force_active reset
s32 haiti_data[][10] = {
    { 18, 16, 16, 0, 0, 0, 0, 0, 0, 0 },
    { 18, 18, 18, 16, 16, 14, 0, 0, 0, 0 },
    { 20, 20, 20, 20, 18, 18, 18, 16, 0, 0 },
    { 20, 20, 20, 20, 20, 20, 20, 20, 20, 20 },
};
#ifdef VERSION_G8ME01
// US: front rows thinned (12/14/14) for the first-pick seat search
s32 haiti_data_front_narrow[][10] = {
    { 12, 14, 14, 0, 0, 0, 0, 0, 0, 0 },
    { 12, 14, 14, 16, 16, 14, 0, 0, 0, 0 },
    { 12, 14, 14, 20, 18, 18, 18, 16, 0, 0 },
    { 12, 14, 14, 20, 20, 20, 20, 20, 20, 20 },
};
#endif

FileEntry* tplRead(const char* path) {
    return fileAllocf(4, "%s/%s", getMarioStDvdRoot(), path);
}

BattleAudience* BattleAudienceBaseGetPtr(void) {
    return &_battleWorkPointer->audience;
}

BattleAudienceMember* BattleAudienceGetPtr(s32 id) {
    return &BattleAudienceBaseGetPtr()->members[id];
}

BattleAudienceItem* BattleAudienceItemGetPtr(s32 id) {
    return &BattleAudienceBaseGetPtr()->items[id];
}

BattleAudienceWindow* BattleAudienceWinGetPtr(void) {
    return &BattleAudienceBaseGetPtr()->window;
}

BattleAudienceSound* BattleAudienceSoundGetPtr(s32 id) {
    return &BattleAudienceBaseGetPtr()->sounds[id];
}

void BattleAudience_Init(void) {
    BattleAudience* audience;
    BattleAudienceWindow* window;
    int j; int i;
    u8 status;

    audience = BattleAudienceBaseGetPtr();
    window = BattleAudienceWinGetPtr();
    if (1) {
    pouchGetPtr(); //useless call
    memset(audience, 0, sizeof(BattleAudience));
    }
    BattleAudienceSettingAudience();
    audience->normalTex = tplRead("battle/audience/audience_normal.tpl");
    BattleAudienceGuestTPLRead(0, AUDIENCE_LUIGI, "audience_luigi.tpl");
    BattleAudienceSetThrowItemMax();

    audience->mTurnEndPhaseEventChance = 0;

    audience->mCurrentAudienceIntCountLeft = 0;
    audience->mCurrentAudienceIntCountRight = 0;
    audience->mCurrentAudienceIntCount = 0;

    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) {
            int index = (i * 20) + j + 10;
            if (AUD_U8CALL(BattleAudience_GetExist, index) == (1 & 0xFFFF)) {
                status = BattleAudienceGetPtr(index)->status;
                if (status != 12 && status != 11) { //TODO: add status enum
                    audience->mCurrentAudienceIntCountRight++;
                }
            }
        }
    }

    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) {
            s32 index = i * 20 + j;
            if (AUD_U8CALL(BattleAudience_GetExist, index) == TRUE) {
                status = BattleAudienceGetPtr(index)->status;
                if (status != 12 && status != 11) {
                    audience->mCurrentAudienceIntCountLeft++;
                }
            }
        }
    }

    audience->mCurrentAudienceIntCount = audience->mCurrentAudienceIntCountRight + audience->mCurrentAudienceIntCountLeft;
    window->count = (f32)audience->mCurrentAudienceIntCount;
    BattleAudienceSoundNoiseAlways();
    BattleAudienceSoundSetVol(2, 60, 180);
    audience->excited = FALSE;
    BattleAudienceNumToTarget();
}

void BattleAudience_Main(void) {
    BattleWork* wp = _battleWorkPointer;
    BattleAudience* audience = BattleAudienceBaseGetPtr();

    if (!(audience->flags & 0x10000) && wp->flags & 0x80) {
        BattleAudience_WinSetActive(1);
    }
    BattleAudienceCtrlProcess();
    BattleAudienceItemCtrlProcess();
    BattleAudienceApSrcCtrlProcess();
    BattleAudienceWinCtrlProcess();
    BattleAudienceSoundMain();
    if (wp->flags & 0x80) {
        audience->flags |= 0x10000;
    }
    else {
        audience->flags &= ~0x10000;
    }
}

void BattleAudience_ActInit(void) {
    BattleAudience* audience = BattleAudienceBaseGetPtr();
    audience->mNumStylishCommandsThisAttack = 0;
    audience->flags &= ~0x10;
    audience->flags &= ~0x20;
    audience->flags &= ~0x40;
    audience->flags &= ~0x80;
    audience->flags &= ~0x100;
    audience->flags &= ~0x200;
    audience->flags &= ~0x400;
    audience->flags &= ~0x800;
    audience->flags &= ~0x1000;
}

void BattleAudience_PerAct(void) {
    BattleAudience* audience = BattleAudienceBaseGetPtr();
    f32 total;
    f32 delta;
    s32 good;
    pouchGetPtr();
    total = 0.0f;
    delta = total;
    if (audience->flags & 0x100) {
        if (!(audience->flags & 0x200)) BattleAudienceCheer(3);
    } else {
        BattleAudienceCheer(2);
    }
    if (audience->flags & 0x40000) return;
    if (!(audience->flags & 0x400) && (audience->flags & 0x800)) {
        total -= 5.0f;
        BattleAudienceAddPhaseEvtList(5);
    }
    if (audience->impendingBonuses && ((audience->flags & 0x10) || (audience->flags & 0x20))) {
        if (audience->flags & 0x10) {
            good = 1;
            audience->crowdPleasedStreak++;
            if (audience->crowdPleasedStreak > 2) audience->crowdPleasedStreak = 2;
            delta = audience->crowdPleasedStreak * audience->impendingBonuses->mSpAcSuccessMultiplier;
            audience->crowdDispleasedStreak = 0;
            total += delta;
        } else if (!(audience->flags & 0x1000)) {
            audience->crowdPleasedStreak = 0;
            good = 0;
            audience->crowdDispleasedStreak++;
            if (audience->crowdDispleasedStreak > 5) audience->crowdDispleasedStreak = 5;
            delta = -5.0f;
            total += delta;
        }
    }
    if ((audience->flags & 0x40) || (audience->flags & 0x80)) {
        if (audience->flags & 0x40) {
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
    if (audience->impendingBonuses && ((audience->flags & 0x100) || (audience->flags & 0x200))) {
        if (audience->flags & 0x100) {
            delta = audience->impendingBonuses->mStylishCommandMultiplier * (audience->mNumStylishCommandsThisAttack * 2);
            total += delta;
            if (delta >= 30.0f) BattleAudienceAddPhaseEvtList(0);
            if (delta >= 40.0f) BattleAudienceAddPhaseEvtList(3);
            if (delta >= 25.0f) BattleAudienceAddPhaseEvtList(8);
            if (delta >= 20.0f) BattleAudienceAddPhaseEvtList(13);
            BattleAudienceAddPuni(10);
        }
        if (audience->flags & 0x200) total -= 5.0f;
    }
    if (audience->flags & 0x8000) {
        audience->event = evtEntry(&msg_puni_all_escape, 0, EVENT_START_IMMEDIATE);
        audience->flags &= ~0x8000;
    }
    BattleAudienceAddTargetNum(audience->mAudienceDeltaMultiplier * (0.01f * total), 0.0f);
    BattleAudience_ActInit();
}

BOOL BattleAudience_CheckReaction(void) {
    BattleAudience* audience;
    EventEntry* evt;

    audience = BattleAudienceBaseGetPtr();
    BattleAudienceGetPtr(0); //unused
    evt = audience->event;
    if (evt && evtCheckID(evt->eventId)) {
        return TRUE;
    }
    audience->event = NULL;
    return FALSE;
}

u8 check_exe_phase_evt_status(s32 index, s32 type) {
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
    if (status == 13) return FALSE;
    if (status == 12) return FALSE;
    if (status == 11) return FALSE;
    if (status == 21) return FALSE;
    if (status == 22) return FALSE;
    return (member->flags >> 3) & 1;
}

void BattleAudience_PerPhase(u32 value) {
    BattleAudience* audience;
    BattleAudienceMember* member;
    s32 i;

    audience = BattleAudienceBaseGetPtr();
    if (value == 0x4000002 && !(audience->flags & 0x40000)) {
        for (i = 0; i < 200; i++) {
            member = BattleAudienceGetPtr(i);
            if ((u8)BattleAudience_GetExist(i) == 1 && member->status == 7) {
                if (member->itemId != -1) {
                    BattleAudienceItemGetPtr(member->itemId)->flags = 0;
                }
                member->itemId = -1;
                BattleAudience_ChangeStatus(i, 0);
            }
        }
        for (i = 0; i < 100; i++) {
            if (BattleAudienceItemGetPtr(i)->state <= 1) {
                BattleAudienceItemGetPtr(i)->flags = 0;
            }
        }
    }
    if (value == 0x4000004) {
        BattleAudienceSetThrowItemMax();
        audience->flags |= 8;
        audience->mCheckPhaseReactionState = 0;
    }
}

#define SUB pe->phaseSub
#define CLR pe->phaseListCount = 0;
#define QSEL(t) for (counter = 0, i = 0; i <= 59; i++) { if (check_exe_phase_evt_status(i, t)) { list[counter] = i; counter++; } }
BOOL BattleAudience_CheckReactionPerPhase(void) {
    BattleAudience* audience;
    BattleAudience* pe;
    BattleAudienceMember* member;
    s32 i, counter, k, qn;
    s32 queue[14];
    s32 list[200];

    audience = BattleAudienceBaseGetPtr();
    pe = audience;
    pouchGetPtr();
    if (!(audience->flags & 8)) {
        return FALSE;
    }
    switch (audience->mCheckPhaseReactionState) {
        case 0:
            audience->mCheckPhaseReactionState = 5;
            if (audience->flags & 0x40000) {
                audience->mCheckPhaseReactionState = 15;
                break;
            }
            //no break
        case 5:
            for (counter = 0, i = 0; i < 200; i++) {
                member = BattleAudienceGetPtr(i);
                if ((u8)BattleAudience_GetSysCtrl(i) == 1 && member->type == AUDIENCE_BULKY_BOBOMB &&
                        member->status == 18 && (member->statusTimer--, member->statusTimer <= 0)) {
                    BattleAudience_ChangeStatus(i, 19);
                    counter++;
                }
            }
            if (counter >= 1) {
                audience->mCheckPhaseReactionState = 6;
            }
            else {
                audience->mCheckPhaseReactionState = 7;
            }
            //no break
        case 6:
        case 7:
            for (i = 0; i < 200; i++) {
                member = BattleAudienceGetPtr(i);
                if ((u8)BattleAudience_GetSysCtrl(i) == 1 && member->type == AUDIENCE_BULKY_BOBOMB &&
                        member->status == 19) {
                    break;
                }
            }
            if (i == 200) { //didn't meet that condition
                if (audience->mCheckPhaseReactionState == 6) {
                    audience->mCheckPhaseReactionState = 8;
                }
                else {
                    audience->mCheckPhaseReactionState = 13;
                }
            }
            break;
        case 8:
            audience->event = evtEntry(&msg_heavy_bomb_fire, 0, EVENT_START_IMMEDIATE);
            audience->mCheckPhaseReactionState = 9;
            break;
        case 9:
            if (!evtCheckID(audience->event->eventId)) {
                audience->event = NULL;
                audience->mCheckPhaseReactionState = 10;
            }
            break;
        case 10:
            if (audience->flags & 0x8000) {
                audience->event = evtEntry(&msg_puni_all_escape, 0, EVENT_START_IMMEDIATE);
                audience->flags &= ~0x8000;
                audience->mCheckPhaseReactionState = 11;
            }
            else {
                audience->mCheckPhaseReactionState = 15;
            }
            break;
        case 11:
            if (!evtCheckID(audience->event->eventId)) {
                audience->event = NULL;
                audience->mCheckPhaseReactionState = 15;
            }
            break;
        case 13:
            memcpy(queue, pe->phaseList, pe->phaseListCount * 4);
            qn = pe->phaseListCount;
            pe->phaseListCount = counter = 0;
            for (; counter < qn; counter++) {
                switch (queue[counter]) {
                    case 0:
                        for (i = 0; i <= 59; i++) {
                            if (check_exe_phase_evt_status(i, 0)) { BattleAudienceAddPhaseEvtList(0); break; }
                        }
                        break;
                    case 1:
                        for (i = 0; i <= 59; i++) {
                            if (check_exe_phase_evt_status(i, 8) && BattleAudienceGetPtr(i)->status != 17) { BattleAudienceAddPhaseEvtList(1); break; }
                        }
                        break;
                    case 2:
                        for (i = 0; i <= 59; i++) {
                            if (check_exe_phase_evt_status(i, 4)) { BattleAudienceAddPhaseEvtList(2); break; }
                        }
                        break;
                    case 3:
                        if (pe->typeWeights[7]) {
                            for (i = 0; i < audience->mMaxAudience; ) {
                                if (!(u8)BattleAudience_GetExist(i)) { BattleAudienceAddPhaseEvtList(3); break; }
                                if (BattleAudienceGetPtr(i)->flags & 0x80) { i += 2; } else { i++; }
                            }
                        }
                        break;
                    case 4:
                        for (i = 0; i <= 59; i++) {
                            if (check_exe_phase_evt_status(i, 2)) { BattleAudienceAddPhaseEvtList(4); break; }
                        }
                        break;
                    case 5:
                        for (i = 0; i <= 59; i++) {
                            if (check_exe_phase_evt_status(i, 1)) { BattleAudienceAddPhaseEvtList(5); break; }
                        }
                        break;
                    case 6:
                        for (i = 0; i <= 59; i++) {
                            if (check_exe_phase_evt_status(i, 1)) { BattleAudienceAddPhaseEvtList(6); break; }
                        }
                        break;
                    case 7:
                        for (i = 0; i <= 59; i++) {
                            if (check_exe_phase_evt_status(i, 3)) { BattleAudienceAddPhaseEvtList(7); break; }
                        }
                        break;
                    case 8:
                        for (i = 0; i <= 59; i++) {
                            if (check_exe_phase_evt_status(i, 5)) { BattleAudienceAddPhaseEvtList(8); break; }
                        }
                        break;
                    case 9:
                        if (!(audience->flags & 0x4000)) {
                            for (i = 0; i <= 59; i++) {
                                if (check_exe_phase_evt_status(i, 5)) { BattleAudienceAddPhaseEvtList(9); break; }
                            }
                        }
                        break;
                    case 10:
                        for (i = 0; i <= 59; i++) {
                            if (check_exe_phase_evt_status(i, 6)) { BattleAudienceAddPhaseEvtList(10); break; }
                        }
                        break;
                    case 11:
                        if (!(audience->flags & 0x4000)) {
                            for (i = 0; i <= 59; i++) {
                                if (check_exe_phase_evt_status(i, 9) && BattleAudienceGetPtr(i)->status != 18) { BattleAudienceAddPhaseEvtList(11); break; }
                            }
                        }
                        break;
                    case 12:
                        if (!(audience->flags & 0x4000)) {
                            for (i = 0; i <= 59; i++) {
                                if (check_exe_phase_evt_status(i, 11) && BattleAudienceDetectPakkunEatTarget(i) != -1) { BattleAudienceAddPhaseEvtList(12); break; }
                            }
                        }
                        break;
                    case 13:
                        for (i = 0; i <= 59; i++) {
                            if (check_exe_phase_evt_status(i, 12)) { BattleAudienceAddPhaseEvtList(13); break; }
                        }
                        break;
                }
            }
            if (pe->phaseListCount >= 1) {
                pe->phaseList[0] = pe->phaseList[irand(pe->phaseListCount)];
                pe->phaseListCount = 1;
            }
            else {
                pe->phaseListCount = 0;
            }
            audience->mCheckPhaseReactionState = 12;
            SUB = 0;
        //no break
        case 12:
            if (pe->phaseListCount == 1) {
                switch (pe->phaseList[0]) {
                    case 0: {
                        u32 n = 0;
                        for (i = 0; i <= 59; i++) { if (check_exe_phase_evt_status(i, 0)) { list[n] = i; n++; } }
                        BattleAudienceItemOn(list[irand(n)], 0, 1);
                        CLR
                        break;
                    }
                    case 1:
                        for (i = 0; i < 200; i++) {
                            if (check_exe_phase_evt_status(i, 8) && BattleAudienceGetPtr(i)->status != 17) {
                                BattleAudience_ChangeStatus(i, 17);
                                BattleAudienceGetPtr(i)->statusTimer = 5;
                            }
                        }
                        CLR
                        break;
                    case 2:
                        for (i = 0; i <= 59; i++) {
                            if (check_exe_phase_evt_status(i, 4)) { BattleAudienceItemOn(i, 0xee, 100); }
                        }
                        CLR
                        break;
                    case 3:
                        switch (SUB) {
                            case 0:
                                BattleAudienceAddPuni(200);
                                SUB = 5;
                                break;
                            case 5:
                                for (i = 0; i < 200; i++) {
                                    if ((u8)BattleAudience_GetSysCtrl(i) && BattleAudienceGetPtr(i)->status == 11) break;
                                }
                                if (i == 200) { SUB = 10; }
                                break;
                            case 10:
                                SUB = 15;
                                audience->event = evtEntry(&msg_puni_all_enter, 0, EVENT_START_IMMEDIATE);
                                break;
                            case 15:
                                if (!evtCheckID(audience->event->eventId)) {
                                    audience->event = NULL;
                                    CLR
                                }
                                break;
                        }
                        break;
                    case 4:
                        switch (SUB) {
                            case 0:
                                QSEL(2)
                                BattleAudience_ChangeStatus(list[irand(counter)], 14);
                                SUB = 5;
                            case 5:
                                for (i = 0; i <= 59; i++) {
                                    if ((u8)BattleAudience_GetSysCtrl(i) && BattleAudienceGetPtr(i)->status == 14) break;
                                }
                                if (i == 60) { CLR }
                                break;
                        }
                        break;
                    case 5:
                        for (i = 0; i <= 59; i++) {
                            if (check_exe_phase_evt_status(i, 1)) { BattleAudienceItemOn(i, 0xed, 100); }
                        }
                        CLR
                        break;
                    case 6:
                        QSEL(1)
                        BattleAudienceItemOn(list[irand(counter)], 0, 1);
                        CLR
                        break;
                    case 7:
                        QSEL(3)
                        BattleAudienceItemOn(list[irand(counter)], 0xef, 1);
                        CLR
                        break;
                    case 8:
                        QSEL(5)
                        BattleAudienceItemOn(list[irand(counter)], 0, 1);
                        CLR
                        break;
                    case 9:
                        switch (SUB) {
                            case 0:
                                QSEL(5)
                                BattleAudience_ChangeStatus(list[irand(counter)], 15);
                                SUB = 5;
                            case 5:
                                for (i = 0; i <= 59; i++) {
                                    if ((u8)BattleAudience_GetSysCtrl(i) && BattleAudienceGetPtr(i)->status == 15) break;
                                }
                                if (i == 60) { CLR }
                                break;
                        }
                        break;
                    case 10:
                        switch (SUB) {
                            case 0:
                                QSEL(6)
                                BattleAudience_ChangeStatus(list[irand(counter)], 16);
                                SUB = 5;
                            case 5:
                                for (i = 0; i <= 59; i++) {
                                    if ((u8)BattleAudience_GetSysCtrl(i) && BattleAudienceGetPtr(i)->status == 16) break;
                                }
                                if (i == 60) { SUB = 10; }
                                break;
                            case 10:
                                SUB = 15;
                                audience->event = evtEntry(&msg_pansy_sing, 0, EVENT_START_IMMEDIATE);
                                break;
                            case 15:
                                if (!evtCheckID(audience->event->eventId)) {
                                    audience->event = NULL;
                                    CLR
                                }
                                break;
                        }
                        break;
                    case 11:
                        for (counter = 0, i = 0; i <= 59; i++) {
                            if (check_exe_phase_evt_status(i, 9) && BattleAudienceGetPtr(i)->status != 18) {
                                list[counter] = i;
                                counter++;
                            }
                        }
                        k = list[irand(counter)];
                        member = BattleAudienceGetPtr(k);
                        BattleAudience_ChangeStatus(k, 18);
                        member->statusTimer = 5;
                        CLR
                        break;
                    case 12:
                        switch (SUB) {
                            case 0:
                                for (counter = 0, i = 0; i <= 59; i++) {
                                    if (check_exe_phase_evt_status(i, 11) && BattleAudienceDetectPakkunEatTarget(i) != -1) {
                                        list[counter] = i;
                                        counter++;
                                    }
                                }
                                BattleAudience_ChangeStatus(list[irand(counter)], 20);
                                SUB = 5;
                            case 5:
                                for (i = 0; i <= 59; i++) {
                                    if ((u8)BattleAudience_GetSysCtrl(i) && BattleAudienceGetPtr(i)->status == 20) break;
                                }
                                if (i == 60) { CLR }
                                break;
                        }
                        break;
                    case 13:
                        for (i = 0; i <= 59; i++) {
                            if (check_exe_phase_evt_status(i, 12)) {
                                BattleAudienceItemOn(i, 0, 1);
                                break;
                            }
                        }
                        CLR
                        break;
                }
            }
            else {
                audience->mCheckPhaseReactionState = 15;
            }
            break;
        case 15:
            for (i = 0; i < 200; i++) {
                member = BattleAudienceGetPtr(i);
                if ((u8)BattleAudience_GetSysCtrl(i) == 1 && member->status == 13 &&
                        (member->statusDelay--, member->statusDelay <= 0)) {
                    BattleAudience_ChangeStatus(i, 3);
                    member->statusDelay = 0;
                }
            }
            for (i = 0; i < 200; i++) {
                member = BattleAudienceGetPtr(i);
                if ((u8)BattleAudience_GetSysCtrl(i) == 1 && member->type == 8 && member->status == 17 &&
                        (member->statusTimer--, member->statusTimer <= 0)) {
                    member->step = 4;
                }
            }
            BattleBreakSlot_DecBreakTurn();
        case 20:
            audience->flags &= ~8;
            pe->phaseListCount = 0;
            return FALSE;
    }
    return TRUE;
}

void BattleAudience_Disp(void) {
    BattleAudienceBaseGetPtr();
    BattleAudienceAnimProcess();
    if (!(BattleAudienceBaseGetPtr()->flags & 0x20000)) {
        dispEntry(CAMERA_3D, 0.0f, 1, BattleAudienceDispAudience, NULL);
        dispEntry(CAMERA_3D, 0.0f, 2, BattleAudienceDispAudience, NULL);
        dispEntry(CAMERA_3D, 0.0f, 1, BattleAudienceDispItem, NULL);
        dispEntry(CAMERA_2D, 499.0f, 0, BattleAudienceDispApSrc, NULL);
    }
    if (pouchGetPtr()->mStarPowersObtained) {
        dispEntry(CAMERA_2D, 499.0f, 1, BattleAudienceDispWin, NULL);
    }
}

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

    if (pouchGetPtr()->mStarPowersObtained) {
        if (audience->mTargetAudienceCount < 0.0f) {
            audience->mTargetAudienceCount = 0.0f;
        }
        if (1) {
        if (audience->mTargetAudienceCount > (f32)audience->mMaxAudience) {
            audience->mTargetAudienceCount = (f32)audience->mMaxAudience;
        }
        pouchSetAudienceNum(audience->mTargetAudienceCount);
        }
    }
#ifdef VERSION_G8ME01
    // US frees the star-power bonus info allocated in BattleAudience_PerAct
    if (audience->impendingBonuses) {
        BattleFree(audience->impendingBonuses);
        audience->impendingBonuses = NULL;
    }
#endif
}


void BattleAudienceSettingAudience(void) {
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
    AudDeltaArr delta;
    s32 perm[12];
    s32 weights[12];
    s32 counts[13];
    AudWArr w;
    BattleDebugCtrl* bflags = &_battleWorkPointer->debug;
    BattleAudience* audience = BattleAudienceBaseGetPtr();
    NpcBattleRandom* rec = &_battleWorkPointer->fbatInfo->setup->random;
    PouchData* pouch = pouchGetPtr();

    delta = R_audience_delta_multiplier;
    switch (pouch->rank) {
    case 0: audience->mMaxAudience = 50; break;
    case 1: audience->mMaxAudience = 100; break;
    case 2: audience->mMaxAudience = 150; break;
    case 3: audience->mMaxAudience = 200; break;
    }
    if (pouchGetPtr()->mStarPowersObtained == 0) {
        audience->mMaxAudience = 0;
    }
    i = pouch->level - 1;
    lim = 64;
    if (i < 65) lim = i;
    audience->mAudienceDeltaMultiplier = delta.v[lim];

    if (bflags->flags & 0x10000) {
        audience->mTargetAudienceCount = (f32)bflags->audienceNum;
        if (audience->mTargetAudienceCount > (f32)audience->mMaxAudience) {
            audience->mTargetAudienceCount = (f32)audience->mMaxAudience;
        }
    } else {
        f32 base = pouchGetAudienceNum();
        audience->mTargetAudienceCount = (pouchGetAudienceNum() - audience->mAudienceDeltaMultiplier) * (f32)(-13 + irand(10) + irand(10)) * 0.01f + base;
    }

    switch (rec->mode) {
    case 0: case 1:
        audience->mAddedAudienceCount = 0.0f;
        break;
    case 4:
        audience->mAddedAudienceCount = 0.5f * audience->mTargetAudienceCount;
        break;
    case 2: case 3: case 5: case 6:
        audience->mAddedAudienceCount = (f32)audience->mMaxAudience - audience->mTargetAudienceCount;
        break;
    }

    switch (rec->mode) {
    case 2: {
        s32 q;
        for (q = 0; q < 12; q++) {
            if (q == 1 || q == 7) weights[q] = 1; else weights[q] = 0;
        }
        hasKind = 0;
        }
        break;
    case 5:
        for (i = 0; i < 12; i++) {
            if (i == 1) weights[i] = 1; else weights[i] = 0;
        }
        hasKind = 0;
        break;
    case 6:
        for (i = 0; i < 12; i++) {
            if (audience_kind[i].flags & 1) weights[i] = 2; else weights[i] = 1;
        }
        hasKind = 1;
        break;
    default:
        w = R_num_audience_type_weights;
        sum = 0;
        target = irand(100);
        kind = 12;
        for (i = 1; i < 12; i++) {
            sum += w.v[i - 1];
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
                if (audience_kind[id].flags & 1) {
                    weights[id] *= 2;
                }
                k++;
            } else {
                weights[id] = 0;
            }
        }
        if (bflags->audienceKind != 0) {
            switch (bflags->audienceKind) {
            case 1: id = 0; break;
            case 2: id = 10; break;
            case 3: id = 8; break;
            case 4: id = 4; break;
            case 5: id = 7; break;
            case 6: id = 2; break;
            case 7: id = 1; break;
            case 8: id = 3; break;
            case 9: id = 5; break;
            case 10: id = 6; break;
            case 11: id = 9; break;
            case 12: id = 11; break;
            }
            for (i = 0; i < 12; i++) {
                if (i == id) weights[i] = 1; else weights[i] = 0;
            }
        }
        hasKind = 1;
        break;
    }
    memset(counts, 0, 0x34);
    ar = (s32)(audience->mTargetAudienceCount + audience->mAddedAudienceCount);
    if (ar < 0) ar = 0;
    if (ar > audience->mMaxAudience) ar = audience->mMaxAudience;
    used = 0;
    sum = 0;
    for (i = 0; i < 12; i++) sum += weights[i];
    for (i = 0; i < 12; i++) {
        counts[i] = ar * weights[i] / sum;
        if (audience_kind[i].flags & 1) {
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
            if (audience_kind[id].flags & 1) {
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
        i++; i %= 12;
    }
    if (hasKind == 1 && used != 0 && (irand(20) == 0 || (bflags->flags & 0x40000))) {
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
                counts[12]++;
                break;
            }
        }
    }
    switch (rec->mode) {
    case 2:
        while ((n = BattleAudience_NoUsedHaitiRand()) != -1) {
            if (n % 20 <= 9) {
                BattleAudience_Entry(n, AUDIENCE_PUNI, 0);
            } else {
                BattleAudience_Entry(n, AUDIENCE_XNAUT, 0);
            }
        }
        break;
    default: {
        s32 cnt;
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
    }
    memcpy(audience->typeWeights, weights, sizeof(audience->typeWeights));
}

void BattleAudienceGuestTPLRead(s32 id, AudienceMemberType member, const char* path) {
    char full_path[256];

    BattleAudience* audience = BattleAudienceBaseGetPtr();
    if (id < 2) {
        sprintf(full_path, "battle/audience/%s", path);
        audience->guestTex[id] = tplRead(full_path);
        audience->guestType[id] = member;
    }
}


void BattleAudienceCtrlProcess(void) {
    s32 chance;
    BattleWork* wp = _battleWorkPointer;
    BattleDebugCtrl* bflags = &wp->debug;
    BattleAudience* audience = BattleAudienceBaseGetPtr();
    BattleAudienceMember* member;
    s32 i, j, index;
    u8 status;
    s32 order[200];

    audience->mCurrentAudienceIntCountLeft = 0;
    audience->mCurrentAudienceIntCountRight = 0;
    audience->mCurrentAudienceIntCount = 0;

    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) {
            index = (i * 20) + j + 10;
            if (AUD_U8CALL(BattleAudience_GetExist, index) == TRUE) {
                status = BattleAudienceGetPtr(index)->status;
                if (status != 12 && status != 11) {
                    audience->mCurrentAudienceIntCountRight++;
                }
            }
        }
    }

    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) {
            index = (i * 20) + j;
            if (AUD_U8CALL(BattleAudience_GetExist, index) == TRUE) {
                status = BattleAudienceGetPtr(index)->status;
                if (status != 12 && status != 11) {
                    audience->mCurrentAudienceIntCountLeft++;
                }
            }
        }
    }

    audience->mCurrentAudienceIntCount = audience->mCurrentAudienceIntCountRight + audience->mCurrentAudienceIntCountLeft;

    i = 0;
    member = BattleAudienceGetPtr(0);
    for (; i < 200; i++, member++) {
        if (member->flags & 0x10) {
            f32* f = &member->hopY;
            f[0] += f[3];
            f[3] += f[5];
            if (f[0] <= 0.0f) {
                f[0] = 0.0f;
                f[3] = 0.0f;
                f[5] = 0.0f;
            }
        } else if (AUD_U8CALL(BattleAudience_GetExist, i)) {
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

    if ((u32)bflags->flags & 0x20000) {
        audience->itemThrowMax = 10000;
    }
    if (wp->flags & 0x80) {
        if (audience->itemThrowCount < audience->itemThrowMax) {
            switch (audience->itemThrowCount) {
            case 0: chance = 1800; break;
            case 1: chance = 6000; break;
            case 2: chance = 30000; break;
            }
            if ((u32)bflags->flags & 0x20000) {
                chance = 60;
            }
            if (irand(chance) == 0) {
                for (i = 0; i < 200; i++) {
                    order[i] = i;
                }
                for (i = 0; i < 1000; i++) {
                    s32 a = irand(200);
                    s32 b = irand(200);
                    s32 t = order[a];
                    order[a] = order[b];
                    order[b] = t;
                }
                for (i = 0; i < 200; i++) {
                    if (BattleAudienceItemOn(order[i], 0, 1) == TRUE) {
                        audience->itemThrowCount++;
                        return;
                    }
                }
            }
        }
    }
}

BOOL BattleAudienceItemOn(s32 index, u16 itemId, s32 numItems) {
    BattleAudience* audience = BattleAudienceBaseGetPtr();
    BattleAudienceMember* member = BattleAudienceGetPtr(index);
    BattleAudienceItem* item;
    s32 st;
    u8 ust;

    if (audience->flags & 0x40000) return FALSE;
    if (audience_kind[member->type].itemTable->type == 0) return FALSE;
    if (itemId >= 0x79 && itemId < 0xEC && swGet(0xEB) == 0) return FALSE;
    st = index % 20;
    if (st < 4 || st > 15) return FALSE;
    if (!AUD_U8CALL(BattleAudience_GetExist, index)) return FALSE;
    ust = member->status;
    if (ust == 7) return FALSE;
    if (ust == 8) return FALSE;
    if (ust == 13) return FALSE;
    if (ust == 12) return FALSE;
    if (ust == 11) return FALSE;
    if (ust == 18) return FALSE;
    if (ust == 17) return FALSE;
    if (ust == 21) return FALSE;
    if (ust == 10) return FALSE;
    if (member->flags & 8) {
        s32 i = 0;
        item = BattleAudienceItemGetPtr(0);
        for (; i < numItems; i++, item++) {
            if (!(item->flags & 1)) {
                item->flags |= 1;
                item->state = 0;
                item->audienceId = index;
                item->mItemType = itemId;
                BattleAudience_ChangeStatus(index, 7);
                member->itemId = i;
                return TRUE;
            }
        }
    }
    return FALSE;
}

void BattleAudienceSetThrowItemMax(void) {
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

BattleUnit* BattleAudienceDetectTargetAll(void) {
    s32 i;
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    s32 count;
    BattleUnitPart* part;
    s32 ids[64];

    BattleAudienceBaseGetPtr();
    count = 0;
    for (i = 0; i < 64; i++) {
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

BattleUnit* BattleAudienceDetectTargetPlayer(void) {
    s32 i;
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    s32 count;
    BattleUnitPart* part;
    s32 ids[64];

    BattleAudienceBaseGetPtr();
    count = 0;
    for (i = 0; i < 64; i++) {
        unit = wp->units[i];
        if (unit != NULL) {
            part = BattleGetUnitPartsPtr(i, BtlUnit_GetBodyPartsId(unit));
            if (unit->mAlliance == ALLIANCE_PLAYER && unit->currentHp > 0 && !(part->attributes & kNotTargetable)) {
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

extern double __fabs(double);
void BattleAudienceItemCtrlProcess(void) {
    Vec hv;
    s32 i;
    BattleWork* wp = _battleWorkPointer;
    BattleAudience* base = BattleAudienceBaseGetPtr();
    BattleAudienceItem* it;
    u8* aud;
    BattleAudienceKind* kd;
    s32 j;

    i = 0;
    it = BattleAudienceItemGetPtr(0);
    for (; i < 100; i++, it++) {
        if (!(it->flags & 1)) {
            continue;
        }
        aud = (u8*)BattleAudienceGetPtr(it->audienceId);
        kd = &audience_kind[((BattleAudienceMember*)aud)->type];
        if (((BattleAudienceMember*)aud)->status == 13 || ((BattleAudienceMember*)aud)->status == 19 || ((BattleAudienceMember*)aud)->status == 21 || ((BattleAudienceMember*)aud)->status == 12 ||
            !(u8)BattleAudience_GetExist(it->audienceId)) {
            goto kill;
        }
        switch (it->state) {
        case 0: {
            it->flags |= 2;
            if (it->mItemType == ITEM_NULL) {
                s32* tbl = (s32*)kd->itemTable;
                s32 cnt = 0;
                s32 sum;
                s32 acc;
                s32 r;
                j = 0;
                sum = 0;
                for (;;) {
                    if (tbl[j * 2] == 0) {
                        break;
                    }
                    sum += tbl[j * 2 + 1];
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
                    if (r >= acc && r < acc + tbl[j * 2 + 1]) {
                        break;
                    }
                    acc += tbl[j * 2 + 1];
                }
                it->mItemType = tbl[j * 2];
            }
            it->timer = i + 180;
            it->scale = 1.0f;
            it->rotZ = 0.0f;
            {
                Vec v;
                v = (Vec){0.0f, 0.0f, 0.0f};
                v.x = ((BattleAudienceMember*)aud)->position.x + kd->itemOffsetX;
                v.y = (((BattleAudienceMember*)aud)->position.y + kd->itemOffsetY) - 10.0f;
                v.z = 1.0f + ((BattleAudienceMember*)aud)->position.z;
                it->position = v;
            }
            it->state++;
        }
        /* fallthrough */
        case 1: {
            if (it->timer <= 0) {
                BattleUnit* t;
                f32 dx, n;
                for (j = 0; j < 100; j++) {
                    if (j != i && (BattleAudienceItemGetPtr(j)->flags & 1) &&
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
                { BattleUnitPart* pp = BtlUnit_GetPartsPtr(t, BtlUnit_GetBodyPartsId(t));
                BtlUnit_GetHitPos(t, pp, &hv.x, &hv.y, &hv.z); }
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
                v.x = ((BattleAudienceMember*)aud)->drawPosX + kd->itemOffsetX;
                ox = intplGetValue(0, -10.0f, 0.0f, k, 5);
                { f32 t = ((BattleAudienceMember*)aud)->drawPosY + kd->itemOffsetY; v.y = t + ox; }
                v.z = 1.0f + ((BattleAudienceMember*)aud)->drawPosZ;
                it->position = v;
                }
            }
            break;
        }
        case 2: {
            if (!(wp->flags & 0x80)) {
                it->mode = 0;
            }
            if (it->timer == 20) {
                BattleAudience_ChangeStatus(it->audienceId, 0);
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
                if (it->mItemType == ITEM_AUDIENCE_HAMMER) {
                    it->rotZ += 18.0f;
                }
                it->timer--;
            }
            break;
        }
        case 3: {
                        s8 a, b;
            if (!(wp->flags & 0x80)) {
                it->mode = 0;
            }
            aud = (u8*)it->target;
            BtlUnit_GetStatus((BattleUnit*)aud, STATUS_INVISIBLE, &a, &b);
            if (a >= 1 || BtlUnit_CheckShadowGuard((BattleUnit*)aud)) {
                it->mode = 0;
            }
            if (it->flags & 4) {
                it->mode = 0;
            }
            if (it->mItemType >= ITEM_THUNDER_BOLT && it->mItemType < ITEM_MAX && pouchGetEmptyHaveItemCnt() <= 0) {
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
                if (it->mItemType >= ITEM_MIN && it->mItemType < ITEM_MAX) {
                    BattleAudience_SetPresentItemType(0);
                } else if (it->mItemType >= ITEM_AUDIENCE_CAN && it->mItemType < BADGE_MIN) {
                    BattleAudience_SetPresentItemType(1);
                }
                BattleAudience_SetPresentTargetUnitId(BtlUnit_GetUnitId((BattleUnit*)aud));
                switch (it->mItemType) {
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
                BattleAudience_SetPresentItemNo(it->mItemType);
                switch (BattleAudience_GetPresentItemType()) {
                case 0:
                    it->flags &= ~2;
                    it->timer = 5;
                    it->state = 7;
                    break;
                case 1:
                    BattleAudienceSoundStop(10);
                    BattleAudienceSoundStop(11);
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
                BattleAudienceSoundStop(10);
                BattleAudienceSoundStop(11);
                it->state = 5;
                it->position.y = 0.1f;
                it->velocity.y = 1.0f;
                it->velocity.z = 0.0f;
            }
            break;
        }
        case 5: {
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
        }
        case 6: {
            if (it->timer <= 0) {
                goto done;
            }
            if (it->timer < 30) {
                if ((it->timer & 3) <= 1) {
                    it->flags |= 2;
                } else {
                    it->flags &= ~2;
                }
            }
            it->timer--;
            break;
        }
        case 7: {
            it->timer--;
            if (it->timer > 0) {
                break;
            }
        }
        done:
            BattleAudienceSoundStop(10);
            BattleAudienceSoundStop(11);
            if (it->audienceId != -1) {
                BattleAudienceGetPtr(it->audienceId);
                BattleAudience_ChangeStatus(it->audienceId, 0);
            }
        kill:
            it->flags = 0;
            it->timer = 0;
            it->state = 0;
            if (it->audienceId != -1) {
                u8* ap = (u8*)BattleAudienceGetPtr(it->audienceId);
                *(s16*)(ap + 0x1c) = -1;
            }
            break;
        }
    }
}


void BattleAudienceApSrcCtrlProcess(void) {
    BattleAudience* base = BattleAudienceBaseGetPtr();
    BattleAudienceStar* star;
    s32 cnt;
    s32 i;
    s32 k;
    f32 dy;
    f32 dx;
    Vec tmp;
    Vec tmp2;

    if (!(*(u32*)base & 2)) {
        return;
    }
    cnt = 0;
    star = base->starpower;
    for (i = 0; i < 100; i++) {
        if ((u32)base->starpower[i].flags & 1) {
            cnt++;
        }
    }
    if (psndSFXChk(base->apStarSfxId) != -1) {
        psndSFX_vol(base->apStarSfxId, 63.0f * cnt / 100.0f + 64.0f);
    }
    if (base->starPowerGain > 0) {
        if (BattleAudienceApSrcEntry() == TRUE) {
            (base->starPowerGain)--;
        }
        if (cnt == 0 && psndSFXChk(base->apStarSfxId) == -1) {
            base->apStarSfxId = psndSFXOn("SFX_BTL_SYS_AP_STAR1");
        }
    } else {
        for (k = 0, i = 0; i < 100; i++) {
            if ((u32)base->starpower[i].flags & 1) {
                break;
            }
            k++;
        }
        if (k == 100) {
            *(u32*)base &= ~2;
            psndSFXOff(base->apStarSfxId);
            base->apStarSfxId = 0;
            base->starPowerGain = 0;
            BattleStatusWindowAPRecoveryOff();
            return;
        }
    }
    for (i = 0; i < 100; i++) {
        BattleAudienceStar* s = &base->starpower[i];
        pouchGetPtr();
        if (!(s->flags & 1)) {
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
            (s->timer)++;
            dx = s->startPos.x; dy = s->targetPos.x;
            s->position.x = intplGetValue(5, dx, dy, s->timer, s->duration);
            dx = s->startPos.y; dy = s->targetPos.y;
            s->position.y = intplGetValue(5, dx, dy, s->timer, s->duration);
            if (s->timer >= s->duration - 20) {
                s->scale.y = intplGetValue(4, 0.9f, 0.8f, s->timer - (s->duration - 20), 10);
            }
            if (s->timer >= s->duration) {
                s->state = 10;
                s->timer = 0;
                s->duration = 15 + irand(10);
                s->scale = (Vec){0.9f, 0.9f, 0.9f};
            }
            break;
        case 10:
            (s->timer)++;
            if (s->timer >= s->duration) {
                s->state = 15;
                s->timer = 0;
                s->duration = 20 + irand(20);
                s->startPos = s->position;
                tmp = statusGetApPos();
                s->targetPos = tmp;
                dx = s->startPos.x - s->targetPos.x;
                dy = s->startPos.y - s->targetPos.y;
                s->radius = s->startRadius =
                    sqrt(dx * dx + dy * dy);
                s->angle = asin(dy / s->radius);
                s->angleSpeed = 0.05f + 0.2f * rand() / 32767.0f;
            }
            break;
        case 15:
            (s->timer)++;
            s->radius = intplGetValue(4, s->startRadius, 0.0f, s->timer, s->duration);
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
                tmp2 = statusGetApPos();
                s->position = tmp2;
            }
            break;
        case 20:
            (s->timer)++;
            s->alpha = intplGetValue(4, 128.0f, 0.0f, s->timer, s->duration);
            if (s->timer >= s->duration) {
                s->flags = 0;
                pouchAddAP(1);
            }
            break;
        }
    }
}

BOOL BattleAudienceApSrcEntry(void) {
    BattleAudienceStar* star;
    CameraEntry* cam;
    s32 list[200];
    s32 count;
    s32 i;
    BattleAudience* base = BattleAudienceBaseGetPtr();
    BattleAudienceMember* m;
    star = base->starpower;
    for (i = 0; i < 100; i++, star++) {
        if (!(star->flags & 1)) {
            break;
        }
    }
    if (i == 100) {
        return FALSE;
    }
    count = 0;
    i = 0;
    BattleAudienceGetPtr(0);
    for (; i < 200; i++) {
        if ((u8)BattleAudience_GetExist(i) == 1) {
            list[count++] = i;
        }
    }
    if (count == 0) {
        return TRUE;
    }
    star->flags = 1;
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


void BattleAudienceWinCtrlProcess(void) {
    BattleAudience* base = BattleAudienceBaseGetPtr();
    BattleAudienceWindow* win = BattleAudienceWinGetPtr();
    f32 diff = (f32)base->mCurrentAudienceIntCount - win->count;
    f32 step;

    if (diff != 0.0f) {
        if (diff > 0.0f) {
            step = (0.1f * diff < 0.1f) ? 0.1f : 0.1f * diff;
            win->count += step;
            if ((f32)base->mCurrentAudienceIntCount - win->count < 0.0f) {
                win->count = (f32)base->mCurrentAudienceIntCount;
            }
        } else {
            step = (0.1f * diff > -0.1f) ? 0.1f : -0.1f * diff;
            win->count -= step;
            if ((f32)base->mCurrentAudienceIntCount - win->count > 0.0f) {
                win->count = (f32)base->mCurrentAudienceIntCount;
            }
        }
    }

    if (win->mActive) {
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
    win->x = intplGetValue(4, -500.0f, -280.0f, win->slideTimer, 15);
    win->y = -160.0f;
    win->z = 0.0f;
}

void BattleAudienceGXInit(void) {
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

void BattleAudienceAnimProcess(void) {
    BattleAudienceMember* m;
    s32 i;
    s32* pc;
    s32 n;
    s32 target;

    BattleAudienceBaseGetPtr();
    i = 0;
    m = BattleAudienceGetPtr(0);
    for (; i < 200; i++, m++) {
        if (!(m->flags & 1)) continue;
        if (!(m->flags & 4)) continue;
        pc = m->animPc;
        if (m->animWait > 0) {
            m->animWait--;
            continue;
        }
        switch (pc[0]) {
        case -2:
            m->texId = pc[1];
            m->animWait = pc[2];
            m->animPc += 3;
            break;
        case -3:
            m->animPc = m->animStart;
            break;
        case -4:
            m->animPc += 2;
            break;
        case -5:
            target = pc[1];
            m->animPc = m->animStart;
            while (!(m->animPc[0] == -4 && m->animPc[1] == target)) {
                m->animPc++;
            }
            break;
        case -6:
            m->animLoop[m->animDepth] = pc[1];
            m->animDepth++;
            m->animPc += 2;
            break;
        case -7:
            m->animLoop[m->animDepth - 1]--;
            if (m->animLoop[m->animDepth - 1] <= 0) {
                m->animDepth--;
                m->animPc += 1;
            } else {
                n = 1;
                do {
                    m->animPc--;
                    if (m->animPc[0] == -6) n--;
                    if (m->animPc[0] == -7) n++;
                } while (n > 0);
                m->animPc += 2;
            }
            break;
        case -8:
            m->animWait = pc[1];
            m->animPc += 2;
            break;
        case -1:
            break;
        }
    }
}

#define AUD_POS(x, y, z) do { f32 _x = (x); f32 _y = (y); f32 _z = (z); GXWGFifo.f32 = _x; GXWGFifo.f32 = _y; GXWGFifo.f32 = _z; } while (0)
#define AUD_POSS(x, y, z) do { GXWGFifo.f32 = (x); GXWGFifo.f32 = (y); GXWGFifo.f32 = (z); } while (0)
#define AUD_TEX(s, t) do { f32 _s = (s); f32 _t = (t); GXWGFifo.f32 = _s; GXWGFifo.f32 = _t; } while (0)
void BattleAudienceDispAudience(CameraId camId, void* param) {
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
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_VTX, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
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
    for (; i < 200; i++, m++) {
        if (dispGetCurWork()->renderMode != 2) {
            if (m->colorA != 0xFF) continue;
        } else {
            if (m->colorA == 0xFF) continue;
        }
        if (!(m->flags & 1) || !(m->flags & 4)) continue;
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
        ox = m->mRotationOffset.x * m->scale.x;
        oy = m->mRotationOffset.y * m->scale.y;
        oz = m->mRotationOffset.z * m->scale.z;
        PSMTXScale(scl, m->scale.x, m->scale.y, m->scale.z);
        { f32 k = 0.017453289f;
        a0 = m->mRotation.x * k;
        a1 = m->mRotation.y * k;
        a2 = m->mRotation.z * k; }
        sx = sin(a0); cx = cos(a0);
        sy = sin(a1); cy = cos(a1);
        sz = sin(a2); cz = cos(a2);
        t0 = sx * sy;
        t1 = sx * cy;
        rot[0][0] = cy * cz - t0 * sz;
        rot[0][1] = -cy * sz - t0 * cz;
        rot[0][2] = -cx * sy;
        rot[0][3] = 0.0f; rot[1][3] = 0.0f; rot[2][3] = 0.0f;
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
        scl[0][3] = scl[0][3] + (m->hopX + (-ox + m->position.x));
        scl[2][3] = scl[2][3] + (m->hopZ + (-oz + m->position.z));
        if (m->type != 2) {
            scl[1][3] = scl[1][3] + (m->hopY + (-oy + m->position.y));
        } else {
            scl[1][3] = scl[1][3] + (m->bobOffset + (m->hopY + (-oy + m->position.y)));
        }
        PSMTXConcat(cam->view, scl, fin);
        GXLoadPosMtxImm(fin, 0);
        {
        f32 uvs[32] = {
            0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
            0.0f, 1.075f, 0.0f, 0.075f, 1.0f, 0.075f, 1.0f, 1.075f,
            0.0f, 0.075f, 0.0f, 1.075f, 1.0f, 1.075f, 0.0f, 1.075f
    };
        off = 0;
        front = BattleAudience_GetFront(i);
        cr = m->colorR; cg = m->colorG; cb = m->colorB; ca = m->colorA;
                if (m->animStart == m->animTable[9]) off = 8;
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
        if ((u8)front == 1 && (cur = m->animStart) != m->animTable[9] && cur != m->animTable[8] && cur != m->animTable[7]) {
            flip ^= 1;
        }
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
        AUD_TEX(uvs[(off + 3) * 2], uvs[(off + 3) * 2 + 1]);
        }
    }
}

void BattleAudienceDispItem(CameraId camId, void* param) {
    BattleAudienceItem* item;
    s32 i;
    u16 iconId;
    Mtx mtx;
    Mtx rot;

    BattleAudienceGXInit();
    i = 0;
    item = BattleAudienceItemGetPtr(0);
    for (; i < 100; i++, item++) {
        if ((item->flags & 1) && (item->flags & 2)) {
            iconId = itemDataTable[item->mItemType].icon_id;
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

void BattleAudienceDispApSrc(CameraId camId, void* param) {
    s32 i;
    BattleAudienceStar* star;
    Mtx rot;
    Mtx scale;
    Mtx trans;
    Mtx mtx;

    star = (BattleAudienceStar*)BattleAudienceBaseGetPtr();
    camGetPtr(CAMERA_2D);
    BattleAudienceGXInit();
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetZCompLoc(GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
    star = ((BattleAudience*)star)->starpower;
    for (i = 0; i < 100; i++, star++) {
        if (star->flags & 1) {
            PSMTXScale(scale, star->scale.x, star->scale.y, star->scale.z);
            PSMTXRotRad(rot, 'z', 0.0f);
            PSMTXTrans(trans, star->position.x, star->position.y, 0.0f);
            PSMTXConcat(scale, rot, mtx);
            PSMTXConcat(trans, mtx, mtx);
            iconDispGxCol(mtx, 0x10, ICON_1B2, (GXColor){0xFF, 0xFF, 0xFF, star->alpha});

        }
    }
}
void BattleAudienceDispWin(CameraId camId, void* param) {
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
        iconDispGx(&pos, 0x10, ICON_209, 0.8f);
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
    if (!(audience_kind[type & 0xFF].flags & 1)) {
        if (m->flags & 1) {
            return FALSE;
        }
        if (m->flags & 0x100) {
            return FALSE;
        }
        m->flags = 5;
        t = (index % 20) * 25 + rand() * 10 / 0x7fff;
        x = t - 0xeb;
        m->position.x = x;
        m->mHomePosition.x = x;
        BattleAudience_Entry_Sub(index, type, status);
    } else {
        if (index % 2 == 1) {
            return FALSE;
        }
        if (m->flags & 1) {
            return FALSE;
        }
        if (m->flags & 0x100) {
            return FALSE;
        }
        m->flags = 0x85;
        x = 12.5f + (f32)((index % 20) * 25 - 230) + (f32)(rand() * 10 / 0x7fff) - 5.0f;
        m->position.x = x;
        m->mHomePosition.x = x;
        BattleAudience_Entry_Sub(index, type, status);
        m[1].flags = 0x101;
    }
    return TRUE;
}

void BattleAudience_Entry_Sub(s32 index, s32 type, u8 status) {
    BattleAudienceMember* m;
    u8* stage;
    s32 row;
    f32 y, z;

    BattleAudienceBaseGetPtr();
    stage = (u8*)BattleStageGetPtr();
    m = (BattleAudienceMember*)BattleAudienceGetPtr(index);
    m->step = 0;
    m->counter = 0;
    m->status = status;
    m->type = type;
    m->itemId = -1;
    m->animTable = audience_kind[type].animTable;
    m->timer = 0;
    BattleAudience_SetAnim(index, 1, TRUE);
    m->animDepth = 0;
    m->colorB = 0;
    m->colorG = 0;
    m->colorR = 0;
    m->colorA = 255;
    row = index / 20;
    y = *(f32*)(stage + 0xB34);
    m->position.y = y;
    m->mHomePosition.y = y;
    z = 0.1f * (f32)(index % 20) + (f32)(row * 30 + 0x73);
    m->position.z = z;
    m->mHomePosition.z = z;
    if (m->type == 11) {
        m->mHomePosition.z += 0.2f;
        m->position.z += 0.2f;
    }
    if (m->status == 11) {
        m->position.y = -2000.0f;
    }
    m->accel = (Vec){0.0f, 0.0f, 0.0f};
    m->velocity = m->accel;
    m->facingDir = (Vec){0.0f, 0.0f, -1.0f};
    m->mRotation = (Vec){0.0f, 0.0f, 0.0f};
    m->mRotationOffset = (Vec){0.0f, 0.0f, 0.0f};
    m->scale = (Vec){1.0f, 1.0f, 1.0f};
    m->baseScale = (Vec){1.0f, 1.0f, 1.0f};
    m->dispOffset = (Vec){0.0f, 0.0f, 0.0f};
    m->angle = 270.0f;
    if ((index >= 4 && index <= 15) || (index >= 24 && index <= 35) || (index >= 44 && index <= 55)) {
        m->flags |= 8;
    }
}

void BattleAudience_Delete(s32 index) {
    BattleAudienceMember* member;

    BattleAudienceBaseGetPtr();
    member = BattleAudienceGetPtr(index);
    if (member->flags & 0x80) {
        member[1].flags = 0;
    }
    member->flags = 0;
}

void BattleAudience_SetAnim(s32 id, s32 animId, BOOL force) {
    BattleAudienceMember* member;
    s32* anim;
    s32 i;

    BattleAudienceBaseGetPtr();
    member = BattleAudienceGetPtr(id);
    if (!(member->flags & 1)) {
        return;
    }
    if (member->flags & 0x100) {
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

BOOL BattleAudience_GetAnimEnd(s32 index) {
    BattleAudienceMember* member = BattleAudienceGetPtr(index);

    if (*member->animPc != -1) {
        return FALSE;
    }
    return member->animWait <= 0;
}

void BattleAudience_GetPosition(s32 index, f32* x, f32* y, f32* z) {
    BattleAudienceMember* member;

    member = BattleAudienceGetPtr(index);
    *x = member->position.x;
    *y = member->position.y;
    *z = member->position.z;
}

void BattleAudience_GetHomePosition(s32 index, f32* x, f32* y, f32* z) {
    BattleAudienceMember* member;

    member = BattleAudienceGetPtr(index);
    *x = member->mHomePosition.x;
    *y = member->mHomePosition.y;
    *z = member->mHomePosition.z;
}

void BattleAudience_GetRotate(s32 index, f32* x, f32* y, f32* z) {
    BattleAudienceMember* member;

    member = BattleAudienceGetPtr(index);
    *x = member->mRotation.x;
    *y = member->mRotation.y;
    *z = member->mRotation.z;
}

void BattleAudience_SetPosition(s32 index, f32 x, f32 y, f32 z) {
    BattleAudienceMember* member;

    member = BattleAudienceGetPtr(index);
    member->position.x = x;
    member->position.y = y;
    member->position.z = z;
}

void BattleAudience_SetRotate(s32 index, f32 x, f32 y, f32 z) {
    BattleAudienceMember* member;

    member = BattleAudienceGetPtr(index);
    member->mRotation.x = x;
    member->mRotation.y = y;
    member->mRotation.z = z;
}

void BattleAudience_SetRotateOffset(s32 index, f32 x, f32 y, f32 z) {
    BattleAudienceMember* member;

    member = BattleAudienceGetPtr(index);
    member->mRotationOffset.x = x;
    member->mRotationOffset.y = y;
    member->mRotationOffset.z = z;
}

void BattleAudience_GetItemOn(s32* memberId, f32* x, f32* y, f32* z, ItemType* itemType) {
    BattleAudience* audience;
    BattleAudienceMember* member;
    BattleAudienceItem* item;
    int i;

    audience = BattleAudienceBaseGetPtr();
    for (i = 0, member = BattleAudienceGetPtr(0); i < 200; i++, member++) {
        if ((member->flags & 1) && !(member->flags & 0x100) && !(member->flags & 2) && member->itemId != -1) {
            item = BattleAudienceItemGetPtr(member->itemId);
            if (item->state <= 4 || item->state == 7) {
                if (memberId) {
                    audience->mItemOnMemberId = i;
                    *memberId = i;
                }
                audience->mItemOnMemberPos.x = member->position.x;
                audience->mItemOnMemberPos.y = member->position.y;
                audience->mItemOnMemberPos.z = member->position.z + 3.0f;

                if (x) {
                    *x = audience->mItemOnMemberPos.x;
                }
                if (y) {
                    *y = audience->mItemOnMemberPos.y;
                }
                if (z) {
                    *z = audience->mItemOnMemberPos.z;
                }

                if (itemType) {
                    if (member->itemId < 0) {
                        *itemType = ITEM_NULL;
                    }
                    else {
                        *itemType = audience->items[member->itemId].mItemType;
                    }
                }
                return;
            }
        }
    }
    *memberId = -1;
}

void BattleAudience_GetItemOn2(s32* memberId, f32* x, f32* y, f32* z, ItemType* itemType) {
    BattleAudience* audience;
    BattleAudienceMember* member;

    audience = BattleAudienceBaseGetPtr();
    member = BattleAudienceGetPtr(audience->mItemOnMemberId);
    if (memberId) {
        *memberId = audience->mItemOnMemberId;
    }

    if (x) {
        *x = audience->mItemOnMemberPos.x;
    }
    if (y) {
        *y = audience->mItemOnMemberPos.y;
    }
    if (z) {
        *z = audience->mItemOnMemberPos.z;
    }

    if (itemType) {
        if (member->itemId < 0) {
            *itemType = ITEM_NULL;
        }
        else {
            *itemType = audience->items[member->itemId].mItemType;
        }
    }
}

void BattleAudience_SetTarget(s32 index) {
    BattleAudienceMember* member;
    s32 i;

    BattleAudienceGetPtr(index)->flags |= 0x20;
    for (i = 0; i < 200; i++) {
        if (i != index && (u8)BattleAudience_GetExist(i)) {
            member = BattleAudienceGetPtr(i);
            if (member->status == 7) {
                BattleAudienceItemGetPtr(member->itemId)->flags = 0;
                member->itemId = -1;
                BattleAudience_ChangeStatus(i, 0);
            }
        }
    }
}

void BattleAudience_Attack(s32 index) {
    BattleAudienceGetPtr(index)->flags |= 0x40;
}

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

void BattleAudience_ChangeStatus(s32 index, s32 status) {
    BattleAudienceMember* member;

    BattleAudienceBaseGetPtr();
    member = BattleAudienceGetPtr(index);
    if (member->flags & 1) {
        if (member->flags & 0x100) {
            if (index >= 1) {
                BattleAudience_ChangeStatus(index - 1, status);
            }
        }
        else {
            member->colorB = 0;
            member->colorG = 0;
            member->colorR = 0;
            member->colorA = 0xFF;
            member->mRotation = (Vec){0.0f, 0.0f, 0.0f};
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
    if (!(member->flags & 1)) {
        return FALSE;
    }
    if (member->flags & 0x100) {
        return FALSE;
    }
    return !(member->flags & 2);
}

BOOL BattleAudience_GetSysCtrl(s32 index) {
    BattleAudienceMember* member;

    BattleAudienceBaseGetPtr();
    member = BattleAudienceGetPtr(index);
    if ((u8)BattleAudience_GetExist(index) == FALSE) {
        return FALSE;
    }
    return !(member->flags & 0x10);
}

BOOL BattleAudience_GetWaiting(s32 index) {
    BattleAudienceMember* member;

    BattleAudienceBaseGetPtr();
    member = BattleAudienceGetPtr(index);
    if (!(u8)BattleAudience_GetSysCtrl(index)) {
        return FALSE;
    }
    if (member->itemId != -1) {
        return FALSE;
    }
    return (u8)(member->status <= 3);
}

BOOL BattleAudience_GetEscapeChangeOK(s32 index) {
    BattleAudience* audience;
    BattleAudienceMember* member;

    audience = BattleAudienceBaseGetPtr();
    member = BattleAudienceGetPtr(index);
    if (audience->flags & 0x4000) {
        return FALSE;
    }
    if (!(u8)BattleAudience_GetSysCtrl(index)) {
        return FALSE;
    }
    if (member->flags & 0x200) {
        return FALSE;
    }
    if (member->status == 13) {
        return FALSE;
    }
    if (member->status == 17) {
        return FALSE;
    }
    if (member->status == 18) {
        return FALSE;
    }
    if (member->status == 14) {
        return FALSE;
    }
    if (member->status == 15) {
        return FALSE;
    }
    if (member->status == 21) {
        return FALSE;
    }
    if (member->status == 22) {
        return FALSE;
    }
    return member->status != 12;
}

s32 BattleAudience_GetAudienceNoFromOffset(s32 index, s32 column, s32 row) {
    s32 audienceNo;

    BattleAudience* audience = BattleAudienceBaseGetPtr();
    BattleAudienceMember* member = BattleAudienceGetPtr(index);
    if (index % 20 + column < 0 || index % 20 + column >= 20) {
        return -1;
    }

    audienceNo = column + (index - (row * 20));
    if (audienceNo < 0 || audienceNo >= 200) {
        return -1;
    }
    return audienceNo;
}

s32 BattleAudience_HaitiRandForFallObject(void) {
    s32 row = irand(3);
    s32 col = 10;

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
        break;
    }
    return col + row * 20;
}

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
            if (!(BattleAudienceGetPtr(id)->flags & 1) && !(BattleAudienceGetPtr(id)->flags & 0x100)) {
                list[count++] = id;
            }
        }
    }
    if (count >= 1) {
        return list[irand(count)];
    }
    return -1;
}

#ifdef VERSION_G8ME01
s32 BattleAudience_NoUsedFCHaitiRandFull(void);
s32 BattleAudience_NoUsedFCHaitiRandCenter(void);

s32 BattleAudience_NoUsedFCHaitiRandFull(void) {
#else
s32 BattleAudience_NoUsedFCHaitiRand(void) {
#endif
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
            if (!(BattleAudienceGetPtr(id)->flags & 1) && !(BattleAudienceGetPtr(id)->flags & 0x100)) {
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
#ifdef VERSION_G8ME01
}

s32 BattleAudience_NoUsedFCHaitiRandCenter(void) {
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
            if (!(BattleAudienceGetPtr(id)->flags & 1) && !(BattleAudienceGetPtr(id)->flags & 0x100)) {
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


s32 BattleAudience_GetAudienceNum(void) {
    return BattleAudienceBaseGetPtr()->mCurrentAudienceIntCount;
}

s32 BattleAudience_GetPPAudienceNum(void) {
    return BattleAudience_GetPPAudienceNum_R() + BattleAudience_GetPPAudienceNum_L();
}

s32 BattleAudience_GetPPAudienceNumKind(s32 type) {
    s32 i, count;

    BattleAudienceBaseGetPtr();
    count = 0;
    for (i = 0; i < 200; i++) {
        if (BattleAudience_GetPPAudienceNum_Sub(i) && type == BattleAudienceGetPtr(i)->type) {
            count++;
        }
    }
    return count;
}

s32 BattleAudience_GetPPAudienceNum_R(void) {
    return BattleAudience_GetPPAudienceNum_RL_Sub(10);
}

s32 BattleAudience_GetPPAudienceNum_L(void) {
    return BattleAudience_GetPPAudienceNum_RL_Sub(0);
}

#pragma push
#pragma opt_loop_invariants off
s32 BattleAudience_GetPPAudienceNum_RL_Sub(s32 offset) {
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
#pragma pop

BOOL BattleAudience_GetPPAudienceNum_Sub(s32 id) {
    BattleAudienceMember* member = BattleAudienceGetPtr(id);

    if (!AUD_U8CALL(BattleAudience_GetExist, id)) {
        return FALSE;
    }
    if (member->status >= 13 && member->status <= 13) {
        return FALSE;
    }
    if (member->status == 17) {
        return FALSE;
    }
    return member->status != 12;
}

void BattleAudience_ApRecoveryBuild(StarPowerInfo* info) {
    BattleWork* bw = _battleWorkPointer;
    BattleAudience* audience = BattleAudienceBaseGetPtr();
    BattleUnit* mario;
    BattleUnit* party;
    f32 sum, root, a, b, cnt, mm, pm, scale = 1.0f;
    s32 j, i;

    if (audience->impendingBonuses == NULL) {
        audience->impendingBonuses = BattleAlloc(0xC);
    }
    memcpy(audience->impendingBonuses, info, 0xC);
    mario = BattleGetMarioPtr(bw);
    party = BattleGetPartyPtr(bw);
    sum = 0.0f;
    for (i = 0; i < 13; i++) {
        sum += audience_kind[i].spWeight * (f32)BattleAudience_GetPPAudienceNumKind(i);
    }
    root = sqrt(sum);
    a = info->mSpAcSuccessMultiplier;
    b = (f32)info->mStylishCommandMultiplier;
    cnt = 0.0f;
    if (a > 0.0f) {
        cnt += 1.0f;
    }
    if (b > 0.0f) {
        cnt += 1.0f;
    }
    if ((u32)mario->mStatusFlags & 0x20000000) {
        mm = 3.0f;
    } else if ((u32)mario->mStatusFlags & 0x10000000) {
        mm = 2.0f;
    } else {
        mm = 1.0f;
    }
    if (party != NULL) {
        if ((u32)party->mStatusFlags & 0x20000000) {
            pm = 2.0f;
        } else if ((u32)party->mStatusFlags & 0x10000000) {
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
    if (irand(100) < (u8)info->mBingoSlotChance) {
        BattleBreakSlot_PointInc();
    }
}

void BattleAudience_WinSetActive(u8 active) {
    BattleAudienceWinGetPtr()->mActive = active;
}

void BattleAudienceNumToTarget(void) {
    s32 num;
    BattleAudience* audience = BattleAudienceBaseGetPtr();

    if (audience->flags & 0x40000) {
        return;
    }
    num = audience->mTargetAudienceCount + audience->mAddedAudienceCount;
    if (num < 0) {
        num = 0;
    }
    if (num > audience->mMaxAudience) {
        num = audience->mMaxAudience;
    }
    BattleAudienceAddAudienceNum(num - BattleAudienceNumToTargetSub());
}

s32 BattleAudienceNumToTargetSub(void) {
    s32 i, count;

    BattleAudienceBaseGetPtr();
    count = 0;
    for (i = 0; i < 200; i++) {
        if (AUD_U8CALL(BattleAudience_GetExist, i) == TRUE && BattleAudienceGetPtr(i)->status != 12) {
            count++;
        }
    }
    return count;
}

void BattleAudienceAddTargetNum(f32 value, f32 value2) {
    BattleAudience* audience = BattleAudienceBaseGetPtr();

    audience->mAddedAudienceCount += value2;
    if (audience->mAddedAudienceCount > 0.0f && value < 0.0f) {
        audience->mAddedAudienceCount += value;
        if (audience->mAddedAudienceCount < 0.0f) {
            value = audience->mAddedAudienceCount;
            audience->mAddedAudienceCount = 0.0f;
        } else {
            value = 0.0f;
        }
    } else if (audience->mAddedAudienceCount < 0.0f && value > 0.0f) {
        audience->mAddedAudienceCount += value;
        if (audience->mAddedAudienceCount > 0.0f) {
            value = audience->mAddedAudienceCount;
            audience->mAddedAudienceCount = 0.0f;
        } else {
            value = 0.0f;
        }
    }
    BattleAudienceAddTargetNumSub(value);
    if (audience->mTargetAudienceCount < 0.0f) {
        audience->mTargetAudienceCount = 0.0f;
    }
    if (audience->mTargetAudienceCount > audience->mMaxAudience) {
        audience->mTargetAudienceCount = audience->mMaxAudience;
    }
    if (audience->mAddedAudienceCount < -1000.0f) {
        audience->mAddedAudienceCount = -1000.0f;
    }
    if (audience->mAddedAudienceCount > 1000.0f) {
        audience->mAddedAudienceCount = 1000.0f;
    }
}

void BattleAudienceAddTargetNumSub(f32 value) {
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
            skew = (s32)(((audience->mTargetAudienceCount / audience->mAudienceDeltaMultiplier) - 1.0f) / 0.25f);
            if (skew >= 1) {
                audience->mTargetAudienceCount += subtract / (skew + 1);
                continue;
            }
        }
        if (subtract < 0.0f) {
            skew = (s32)(((audience->mTargetAudienceCount / audience->mAudienceDeltaMultiplier) - 1.0f) / 0.25f);
            if (skew <= -1) {
                audience->mTargetAudienceCount -= subtract / (skew - 1);
                continue;
            }
        }
        audience->mTargetAudienceCount += subtract;
    }
}

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
    if (pouchGetPtr()->mStarPowersObtained == 0) {
        return 0;
    }
    if (num == 0) {
        return 0;
    }
    if (wp->flags & 0x100) {
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
            if (BattleAudience_Entry(id, type, 11) == TRUE) {
                added++;
                BattleAudienceGetPtr(id)->counter = added;
            }
        }
    } else {
        for (i = 0; i > num; i--) {
            count = 0;
            for (j = 0; j < 200; j++) {
                if (AUD_U8CALL(BattleAudience_GetEscapeChangeOK, j)) {
                    list[count++] = j;
                }
            }
            if (count >= 1) {
                id = list[irand(count)];
                BattleAudience_ChangeStatus(id, 12);
                added--;
                BattleAudienceGetPtr(id)->counter = -added;
            }
        }
    }
    return added;
}

void BattleAudiencePuniAllEscape(void) {
    BattleAudienceMember* member;
    BattleAudience* audience;
    s32 i;
    s32 count;

    audience = BattleAudienceBaseGetPtr();
    count = 0;
    for (i = 0; i < 200; i++) {
        member = BattleAudienceGetPtr(i);
        if (AUD_U8CALL(BattleAudience_GetEscapeChangeOK, i) && member->type == 7) {
            BattleAudience_ChangeStatus(i, 12);
            audience->mAddedAudienceCount -= 1.0f;
            count++;
        }
    }
    if (count >= 1) {
        audience->flags |= 0x8000;
    }
}

void BattleAudienceAddPuni(s32 num) {
    BattleAudience* audience;
    BattleWork* wp = _battleWorkPointer;
    s32 i;

    audience = BattleAudienceBaseGetPtr();
    if (pouchGetPtr()->mStarPowersObtained != 0 && audience->typeWeights[7] > 0 && num > 0 && !(wp->flags & 0x100)) {
        for (i = 0; i < num; i++) {
            s32 id = BattleAudience_NoUsedFCHaitiRand();
            if (id == -1) {
                return;
            }
            if (BattleAudience_Entry(id, AUDIENCE_PUNI, 11) == TRUE) {
                audience->mAddedAudienceCount += 1.0f;
            }
        }
    }
}

void BattleAudienceAddPhaseEvtList(s32 type) {
    s32 i;
    BattleAudience* p = BattleAudienceBaseGetPtr();

    for (i = 0; i < p->phaseListCount; i++) {
        if (p->phaseList[i] == type) {
            return;
        }
    }
    p->phaseList[p->phaseListCount] = type;
    p->phaseListCount++;
}

void BattleAudienceCheer(s32 type) {
    BattleAudience* audience = BattleAudienceBaseGetPtr();

    if (audience->starPowerGain > 800) {
        audience->starPowerGain = 800;
    }
    if (audience->starPowerGain > 0) {
        if (pouchGetPtr()->maximumSP == 0) {
            audience->starPowerGain = 0;
        } else {
            audience->flags |= 2;
        }
        BattleAudienceJoy(type);
        if (audience->starPowerGain != 0) {
            BattleStatusWindowAPRecoveryOn();
        }
    }
}

void BattleAudienceJoy(s32 type) {
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
        audience->joyLevel++;
        if (audience->joyLevel > 4) {
            audience->joyLevel = 4;
        }
        break;
    }
}

void BattleAudienceJoySACLecture(void) {
    BattleAudienceSoundCheer(120, 60);
    BattleAudienceJoy(2);
}

void BattleAudienceJoyEnding(void) {
    BattleAudienceJoy(3);
}

#pragma push
#pragma opt_loop_invariants off
void BattleAudienceJoy_Sub(s32 a, s32 b, s32 c, s32 type) {
    BattleAudienceMember* member;
    s32 i;
    s32 total;
    s32 r, ab;

    BattleAudienceBaseGetPtr();
    total = c + (a + b);
    for (i = 0, member = BattleAudienceGetPtr(0); i < 200; i++, member++) {
        if (!AUD_U8CALL(BattleAudience_GetWaiting, i)) {
            continue;
        }
        if (member->type == 11) {
            BattleAudience_ChangeStatus(i, 4);
            if (type == 3) {
                member->counterF = 3.0f;
            } else {
                member->counterF = 2.0f;
            }
        } else {
            r = irand(total);
            if (r < a) {
                BattleAudience_ChangeStatus(i, 4);
                if (type == 3) {
                    member->counterF = 3.0f;
                } else {
                    member->counterF = 2.0f;
                }
            } else if (r < (ab = a + (s32)b)) {
                BattleAudience_ChangeStatus(i, 5);
            } else if (r < ab + c) {
                BattleAudience_ChangeStatus(i, 6);
            }
        }
    }
}

#pragma pop

s32 BattleAudienceDetectPakkunEatTargetSub2(s32 index, s32 dir) {
    s32 id;

    BattleAudienceBaseGetPtr();
    id = BattleAudience_GetAudienceNoFromOffset(index, dir, 0);
    if (id == -1) {
        return -1;
    }
    BattleAudienceGetPtr(id);
    if (AUD_U8CALL(BattleAudience_GetSysCtrl, id) == TRUE) {
        return id;
    }
    return -1;
}

s32 BattleAudienceDetectPakkunEatTargetSub(s32 index, s32 dir1, s32 dir2) {
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

s32 BattleAudienceDetectPakkunEatTarget(s32 index) {
    if (irand(2) == 0) {
        return BattleAudienceDetectPakkunEatTargetSub(index, -1, 1);
    }
    return BattleAudienceDetectPakkunEatTargetSub(index, 1, -1);
}

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

void BattleAudienceSoundBooingKind(s32 kind) {
    char name[64];
    AudSound* sound;
    s32 right, left;

    BattleAudienceBaseGetPtr();
    sound = AudSoundGetPtr(0);
    right = BattleAudience_GetPPAudienceNum_R();
    left = BattleAudience_GetPPAudienceNum_L();
    BattleAudienceSoundStop(0);
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

void BattleAudienceSoundClap(s32 duration, s32 fade) {
    char name[64];
    AudSound* sound;
    s32 right, left, sum;

    BattleAudienceBaseGetPtr();
    sound = AudSoundGetPtr(1);
    right = BattleAudience_GetPPAudienceNum_R();
    left = BattleAudience_GetPPAudienceNum_L();
    sum = right + left;
    BattleAudienceSoundStop(1);
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
    sound->sfxId = BattleAudienceSound1(name, right, left);
    AUD_SOUND_START(sound, duration + fade, fade);
}

void BattleAudienceSoundNoiseAlways(void) {
    char name[64];
    BattleAudience* audience;
    AudSound* sound;
    s32 right, left, sum;

    audience = BattleAudienceBaseGetPtr();
    sound = AudSoundGetPtr(2);
    right = BattleAudience_GetPPAudienceNum_R();
    left = BattleAudience_GetPPAudienceNum_L();
    sum = right + left;
    if (!(sound->flags & 1)) {
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
        sound->sfxId = BattleAudienceSound1(name, right, left);
        if (sound->sfxId != -1) {
            sound->flags |= 1;
            sound->duration = -1;
            sound->mFadeoutTime = 0;
            sound->mBaseSfxVolume = sound->mCurrentBaseSfxVolume = psndSFX_get_vol(sound->sfxId);
            sound->mForceFadeLength = 0;
            sound->mForceFadeTimer = 0;
            sound->mVolumeMultiplier = 100;
            audience->flags |= 0x2000;
        }
    }
}

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

void BattleAudienceSoundCheerKind(s32 kind, s32 duration, s32 fade) {
    char name[64];
    AudSound* sound;
    AudSound* sound5;
    s32 right, left;
    s32 lowPitch;

    BattleAudienceBaseGetPtr();
    lowPitch = 0;
    right = BattleAudience_GetPPAudienceNum_R();
    left = BattleAudience_GetPPAudienceNum_L();
    sound = AudSoundGetPtr(4);
    sound5 = AudSoundGetPtr(5);
    if ((sound->flags & 1) && (sound5->flags & 1)) {
        if (sound->duration <= sound5->duration) {
            BattleAudienceSoundStop(4);
            sound = AudSoundGetPtr(4);
        }
        else {
            BattleAudienceSoundStop(5);
            sound = AudSoundGetPtr(5);
        }
    }
    else if (sound->flags & 1) {
        sound = AudSoundGetPtr(5);
        lowPitch = 1;
    }
    else if (sound5->flags & 1) {
        sound = AudSoundGetPtr(4);
        lowPitch = 1;
    }
    else {
        sound = AudSoundGetPtr(4);
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
    if (lowPitch == 1) {
        sound->sfxId = BattleAudienceSound1(name, right, left);
        psndSFX_pit(sound->sfxId, 0x7F);
    }
    else {
        sound->sfxId = BattleAudienceSound1(name, right, left);
    }
    AUD_SOUND_START(sound, duration + fade, fade);
}

void BattleAudienceSoundWhistle(void) {
    BattleAudienceSoundWhistleKind(irand(3) + 1);
}

void BattleAudienceSoundWhistleKind(s32 kind) {
    Vec pos;
    s32 id, idx;
    s32 list[200];
    s32 i;
    s32 count;
    AudSound* sound;

    count = 0;
    for (i = 0; i < 200; i++) {
        if ((u8)BattleAudience_GetWaiting(i) == TRUE) {
            list[count] = i;
            count++;
        }
    }
    if (count != 0) {
        idx = list[irand(count)];
        BattleAudienceSoundGetInfo2(idx, &pos);
        if (pos.x <= 0.0f) {
            id = 6;
        }
        else {
            id = 7;
        }
        sound = AudSoundGetPtr(id);
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

void BattleAudienceSoundCallKind(s32 kind) {
    AudSound* sound;
    s32 right, left;

    BattleAudienceBaseGetPtr();
    sound = AudSoundGetPtr(8);
    right = BattleAudience_GetPPAudienceNum_R();
    left = BattleAudience_GetPPAudienceNum_L();
    BattleAudienceSoundStop(8);
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

void BattleAudienceSoundHandBeat(void) {
    s32 right;
    AudSound* sound;
    s32 left;
    s32 sum;

    BattleAudienceBaseGetPtr();
    sound = AudSoundGetPtr(9);
    right = BattleAudience_GetPPAudienceNum_R();
    left = BattleAudience_GetPPAudienceNum_L();
    sum = right + left;
    BattleAudienceSoundStop(9);
    sound->sfxId = BattleAudienceSound1("SFX_AUDIENCE_HANDBEAT1", right, left);
    if (sound->sfxId != -1) {
        sound->flags |= 1;
        sound->duration = -1;
        right = 0;
        sound->mFadeoutTime = right;
        sound->mForceFadeLength = right;
        sound->mForceFadeTimer = right;
        if (sum == 0) {
            right = 0;
        }
        else {
            right = 75.0f + sum * (26.0f + irand(26)) / 200.0f;
        }
        psndSFX_vol(sound->sfxId, right);
        sound->mCurrentBaseSfxVolume = right;
        sound->mBaseSfxVolume = right;
        sound->mVolumeMultiplier = 100;
    }
}

void BattleAudienceSoundItemThrow(s32 kind, s32 index) {
    Vec pos;
    s32 id;
    AudSound* sound;

    BattleAudienceSoundGetInfo2(index, &pos);
    if (pos.x <= 0.0f) {
        id = 10;
    }
    else {
        id = 11;
    }
    sound = AudSoundGetPtr(id);
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

void BattleAudienceSoundRun(s32 kind) {
    AudSound* sound = AudSoundGetPtr(13);

    BattleAudienceSoundStop(13);
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

void BattleAudienceSoundSleep(s32 index) {
    Vec pos;
    s32 id;
    AudSound* sound;

    BattleAudienceSoundGetInfo2(index, &pos);
    if (pos.x <= 0.0f) {
        id = 14;
    }
    else {
        id = 15;
    }
    sound = AudSoundGetPtr(id);
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

void BattleAudienceSoundZZZ(s32 index) {
    AudSound* sound = AudSoundGetPtr(16);

    BattleAudienceSoundStop(16);
    BattleAudienceSound2("SFX_AUDIENCE_ZZZ1", index);
    AUD_SOUND_START(sound, -1, 0);
}

void BattleAudienceSoundSing(s32 index) {
    AudSound* sound = AudSoundGetPtr(17);

    BattleAudienceSoundStop(17);
    sound->sfxId = BattleAudienceSound2("SFX_AUDIENCE_PANSY_SINGING1", index);
    AUD_SOUND_START(sound, -1, 0);
}

void BattleAudienceSoundShell(s32 index) {
    Vec pos;
    s32 id;
    AudSound* sound;

    BattleAudienceSoundGetInfo2(index, &pos);
    if (pos.x <= 0.0f) {
        id = 18;
    }
    else {
        id = 19;
    }
    sound = AudSoundGetPtr(id);
    BattleAudienceSoundStop(id);
    sound->sfxId = BattleAudienceSound2("SFX_AUDIENCE_SHELL_TRANSFORM1", index);
    AUD_SOUND_START(sound, -1, 0);
}

void BattleAudienceSoundBombIgnite(s32 index, s32 duration, s32 fade) {
    AudSound* sound = AudSoundGetPtr(20);

    BattleAudienceSoundStop(20);
    sound->sfxId = BattleAudienceSound2("SFX_AUDIENCE_BOMB_READY1", index);
    AUD_SOUND_START(sound, duration + fade, fade);
}

void BattleAudienceSoundBombFire(s32 index) {
    Vec pos;
    s32 id;
    AudSound* sound;

    BattleAudienceSoundGetInfo2(index, &pos);
    if (pos.x <= 0.0f) {
        id = 21;
    }
    else {
        id = 22;
    }
    sound = AudSoundGetPtr(id);
    BattleAudienceSoundStop(id);
    sound->sfxId = BattleAudienceSound2("SFX_AUDIENCE_BOMB_BURST1", index);
    AUD_SOUND_START(sound, -1, 0);
}

void BattleAudienceSoundPakkunEat(s32 index) {
    AudSound* sound = AudSoundGetPtr(23);

    BattleAudienceSoundStop(23);
    sound->sfxId = BattleAudienceSound2("SFX_AUDIENCE_PAKU_EAT1", index);
    AUD_SOUND_START(sound, -1, 0);
}

void BattleAudienceSoundSetVol(s32 index, u8 vol, s32 length) {
    BattleAudienceSound* sound;

    sound = BattleAudienceSoundGetPtr(index);
    sound->mFadeStartVolumeMultiplier = sound->mVolumeMultiplier;
    sound->mFadeEndVolumeMultiplier = vol;
    sound->mForceFadeTimer = 0;
    sound->mForceFadeLength = length;
    sound->mFadeoutTime = 0;
}

void BattleAudienceSoundSetVolAll(u8 vol, s32 length) {
    s32 i;

    for (i = 0; i < 24; i++) {
        BattleAudienceSoundSetVol(i, vol, length);
    }
}

s32 BattleAudienceSoundCheck(s32 index) {
    return AudSoundGetPtr(index)->flags & 1;
}

void BattleAudienceSoundStop(s32 index) {
    AudSound* sound = AudSoundGetPtr(index);

    if (sound->flags & 1) {
        psndSFXOff(sound->sfxId);
        sound->flags &= ~1;
    }
}

s32 BattleAudienceSound0(const char* name) {
    return psndSFXOn(name);
}

s32 BattleAudienceSound1(const char* name, s32 right, s32 left) {
    u8 vol;
    Vec pos;
    s32 id;

    BattleAudienceSoundGetInfo1(right, left, &vol, &pos);
    id = psndSFXOn_3D(name, &pos);
    psndSFX_vol(id, vol);
    return id;
}

void BattleAudienceSoundGetInfo1(s32 right, s32 left, u8* vol, Vec* pos) {
    s32 sum = right + left;

    if (sum == 0) {
        *vol = 0;
    }
    else {
        *vol = 100.0f + 26.0f * sum / 200.0f;
    }
    if (left == 0 || sum == 0) {
        pos->x = 250.0f;
    }
    else {
        pos->x = (u8)(250.0f * (right - left) / sum);
    }
    pos->y = 0.0f;
    pos->z = 0.0f;
}

s32 BattleAudienceSound2(const char* name, s32 index) {
    Vec pos;

    BattleAudienceSoundGetInfo2(index, &pos);
    return psndSFXOn_3D(name, &pos);
}

void BattleAudienceSoundGetInfo2(s32 index, Vec* pos) {
    BattleAudienceMember* member = BattleAudienceGetPtr(index);

    pos->x = member->position.x;
    pos->y = member->position.y;
    pos->z = member->position.z;
}

void BattleAudienceSoundMain(void) {
    AudSound* sound;
    s32 i;

    BattleAudienceBaseGetPtr();
    BattleAudienceNoiseMain();
    for (i = 0, sound = AudSoundGetPtr(0); i < 24; i++, sound++) {
        if (psndSFXChk(sound->sfxId) == -1) {
            sound->flags &= ~1;
            continue;
        }
        if (sound->duration == 0) {
            psndSFXOff(sound->sfxId);
            sound->flags &= ~1;
            continue;
        }
        if (sound->mForceFadeLength > 0) {
            sound->mForceFadeTimer++;
            sound->mVolumeMultiplier = intplGetValue(0, sound->mFadeStartVolumeMultiplier, sound->mFadeEndVolumeMultiplier, sound->mForceFadeTimer, sound->mForceFadeLength);
            psndSFX_vol(sound->sfxId, (u8)(sound->mCurrentBaseSfxVolume * sound->mVolumeMultiplier / 100));
            if (sound->mForceFadeTimer >= sound->mForceFadeLength) {
                sound->mForceFadeLength = 0;
            }
        }
        if (sound->duration > 0) {
            if (sound->duration == sound->mFadeoutTime) {
                sound->mFadeStartVolumeMultiplier = sound->mVolumeMultiplier;
                sound->mFadeEndVolumeMultiplier = 0;
            }
            if (sound->duration <= sound->mFadeoutTime) {
                sound->mVolumeMultiplier = intplGetValue(0, sound->mFadeStartVolumeMultiplier, sound->mFadeEndVolumeMultiplier, sound->mFadeoutTime - sound->duration, sound->mFadeoutTime);
                psndSFX_vol(sound->sfxId, (u8)(sound->mCurrentBaseSfxVolume * sound->mVolumeMultiplier / 100));
            }
            sound->duration--;
        }
    }
}

void BattleAudienceNoiseMain(void) {
    BattleWork* wp = _battleWorkPointer;
    BattleAudience* audience = BattleAudienceBaseGetPtr();
    AudSound* sound = AudSoundGetPtr(2);
    s32 num = BattleAudience_GetPPAudienceNum();

    if (!(audience->flags & 0x10000) && (wp->flags & 0x80)) {
        BattleAudienceSoundSetVol(2, 60, 80);
    }
    if ((audience->flags & 0x10000) && !(wp->flags & 0x80)) {
        BattleAudienceSoundSetVol(2, 0, 80);
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
    if (!(audience->flags & 0x2000)) {
        sound->mCurrentBaseSfxVolume = 0;
    }
    else if (num == 0) {
        sound->mCurrentBaseSfxVolume = 0;
    }
    else {
        sound->mCurrentBaseSfxVolume = 63.0f + 63.0f * num / 200.0f;
    }
}

void BattleAudience_Case_KillEnemy(s32 level) {
    BattleAudienceBaseGetPtr()->flags |= 0x1000;
}

void BattleAudience_Case_ActionCommandGood(BattleWeapon* weapon) {
    BattleAudience* audience = BattleAudienceBaseGetPtr();
    s32 i;
    s32 count = 0;

    for (i = 0; i < 200; i++) {
        if ((u8)BattleAudience_GetWaiting(i) == TRUE) {
            count++;
        }
    }
    if (count != 0) {
        BattleAudienceSoundCheer(180, 120);
        BattleAudienceJoy(0);
    }
    audience->flags |= 0x10;
}

void BattleAudience_Case_ActionCommandBad(BattleWeapon* weapon) {
    BattleAudience* audience = BattleAudienceBaseGetPtr();
    s32 count = 0;
    s32 i;

    for (i = 0; i < 200; i++) {
        if ((u8)BattleAudience_GetWaiting(i) == TRUE) {
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
    audience->flags |= 0x20;
}

void BattleAudience_Case_GuardGood(void) {
    BattleAudienceBaseGetPtr()->flags |= 0x40;
}

void BattleAudience_Case_GuardBad(void) {
    BattleAudienceBaseGetPtr()->flags |= 0x80;
}

void BattleAudience_Case_AcrobatGood(void) {
    BattleAudience* audience = BattleAudienceBaseGetPtr();
    s32 count = 0;
    s32 i;

    for (i = 0; i < 200; i++) {
        if ((u8)BattleAudience_GetWaiting(i) == TRUE) {
            count++;
        }
    }
    if (count >= 1) {
        BattleAudienceSoundCheer(180, 120);
        BattleAudienceSoundClap(180, 120);
        BattleAudienceJoy(1);
    }
    audience->mNumStylishCommandsThisAttack++;
    if (audience->mNumStylishCommandsThisAttack > 5) {
        audience->mNumStylishCommandsThisAttack = 5;
    }
    audience->flags |= 0x100;
}

void BattleAudience_Case_AcrobatBad(void) {
    BattleAudience* audience = BattleAudienceBaseGetPtr();
    s32 count = 0;
    s32 i;

    for (i = 0; i < 200; i++) {
        if ((u8)BattleAudience_GetWaiting(i) == TRUE) {
            count++;
        }
    }
    if (count >= 1) {
        BattleAudienceSoundBooing();
        BattleAudienceJoy(0);
    }
    audience->flags |= 0x200;
}

void BattleAudience_Case_AcrobatNoTry(void) {
    BattleAudienceBaseGetPtr();
}

void BattleAudience_Case_MarioBigDamage(s32 damage) {
    BattleAudienceBaseGetPtr();
}

void BattleAudience_Case_MarioPinch(void) {
    BattleAudienceAddTargetNum(0.049999997f * BattleAudienceBaseGetPtr()->mAudienceDeltaMultiplier, 0.0f);
}

void BattleAudience_Case_MarioDanger(void) {
    BattleAudienceAddTargetNum(0.14999999f * BattleAudienceBaseGetPtr()->mAudienceDeltaMultiplier, 0.0f);
}

void BattleAudience_Case_PartyDown(void) {
    BattleAudienceAddTargetNum(-0.25f * BattleAudienceBaseGetPtr()->mAudienceDeltaMultiplier, 0.0f);
}

void BattleAudience_Case_JumpNewRecord(void) {
}

void BattleAudience_Case_FinalAttack(void) {
}

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
    if (!(audience->flags & 0x40000)) {
        audience->mTurnEndPhaseEventChance += 3;
        if (audience->mTurnEndPhaseEventChance > 100) {
            audience->mTurnEndPhaseEventChance = 100;
        }
        for (i = 0; i < group->numEnemies; i++, setup++) {
            sum += setup->mUnitKindParams->mLevel;
        }
        turns = sum * 2 / mario->mLevel;
        if (turns < 5) {
            turns = 5;
        }
        if (wp->turnCount >= turns) {
            BattleAudienceAddTargetNum(-0.049999997f * BattleAudienceBaseGetPtr()->mAudienceDeltaMultiplier, 0.0f);
        }
        if (wp->turnCount == turns) {
            BattleAudienceAddPhaseEvtList(6);
        }
        if (irand(100) < audience->mTurnEndPhaseEventChance) {
            if (wp->turnCount >= turns) {
                BattleAudienceAddPhaseEvtList(1);
            }
            BattleAudienceAddPhaseEvtList(4);
            BattleAudienceAddPhaseEvtList(9);
            BattleAudienceAddPhaseEvtList(10);
            BattleAudienceAddPhaseEvtList(11);
            BattleAudienceAddPhaseEvtList(12);
            BattleAudienceAddPhaseEvtList(13);
            audience->mTurnEndPhaseEventChance = 0;
        }
        BattleAudienceNumToTarget();
    }
}

void BattleAudience_Case_Appeal(BattleUnit* unit) {
    BattleAudience* audience = BattleAudienceBaseGetPtr();

    if (BattleAudience_GetPPAudienceNum() > 0.0f) {
        audience->starPowerGain += (s32)(BattleAudience_GetPPAudienceNum() / 4.0f) + 25;
        if (unit->badgesEquipped.SuperAppeal) {
            audience->starPowerGain += unit->badgesEquipped.SuperAppeal * 25;
        }
        BattleAudienceSoundCheer(180, 60);
        BattleAudienceSoundClap(180, 60);
        BattleAudienceJoy(0);
    }
    BattleAudienceAddTargetNum(0.049999997f * BattleAudienceBaseGetPtr()->mAudienceDeltaMultiplier, 0.0f);
}

void BattleAudience_Case_Escape(void) {
}

void BattleAudience_Case_Escape_Good(void) {
    BattleAudienceAddTargetNum(-0.099999994f * BattleAudienceBaseGetPtr()->mAudienceDeltaMultiplier, 0.0f);
    BattleAudienceSoundSetVol(2, 100, 30);
    BattleAudienceSoundBooing();
}

void BattleAudience_Case_Escape_Bad(void) {
    BattleAudienceAddTargetNum(-0.19999999f * BattleAudienceBaseGetPtr()->mAudienceDeltaMultiplier, 0.0f);
    BattleAudienceSoundBooing();
}

void BattleAudience_Case_EnemyDamage(s32 damage) {
    BattleAudienceBaseGetPtr()->flags |= 0x400;
}

void BattleAudience_Case_EnemyNoDamage(void) {
    BattleAudienceBaseGetPtr()->flags |= 0x800;
}

void BattleAudience_Case_Countered(void) {
    BattleAudienceAddTargetNum(-0.099999994f * BattleAudienceBaseGetPtr()->mAudienceDeltaMultiplier, 0.0f);
    BattleAudienceAddPhaseEvtList(5);
}

void BattleAudience_Case_HaikeiSet(void) {
    BattleAudienceAddTargetNum(0.049999997f * BattleAudienceBaseGetPtr()->mAudienceDeltaMultiplier, 0.0f);
}

void BattleAudience_Case_GreatVictory(void) {
    BattleAudienceAddTargetNum(0.14999999f * BattleAudienceBaseGetPtr()->mAudienceDeltaMultiplier, 0.0f);
}

void BattleAudience_Case_FastVictory(f32 rate) {
    BattleAudienceAddTargetNum(0.01f * (5.0f * rate) * BattleAudienceBaseGetPtr()->mAudienceDeltaMultiplier, 0.0f);
}

void BattleAudience_Case_FallObject_Stage(void) {
    BattleAudienceAddTargetNum(0.049999997f * BattleAudienceBaseGetPtr()->mAudienceDeltaMultiplier, 0.0f);
}

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
        case 0:
        case 1:
        case 3:
        case 4:
        case 5:
        case 8:
            if ((u8)BattleAudience_GetSysCtrl(index) == TRUE && BattleAudienceGetPtr(index)->status != 15) {
                BattleAudience_ChangeStatus(index, 10);
            }
            for (i = 0; i < 8; i++) {
                no = BattleAudience_GetAudienceNoFromOffset(index, near[i][0], near[i][1]);
                if (no != -1 && (u8)BattleAudience_GetEscapeChangeOK(no)) {
                    BattleAudience_ChangeStatus(no, 12);
                    audience->mTargetAudienceCount -= 1.0f;
                }
            }
            break;
        case 7:
            if ((u8)BattleAudience_GetSysCtrl(index) == TRUE && BattleAudienceGetPtr(index)->status != 15) {
                BattleAudience_ChangeStatus(index, 10);
            }
            for (i = 0; i < 24; i++) {
                no = BattleAudience_GetAudienceNoFromOffset(index, far[i][0], far[i][1]);
                if (no != -1 && (u8)BattleAudience_GetEscapeChangeOK(no)) {
                    BattleAudience_ChangeStatus(no, 12);
                    audience->mTargetAudienceCount -= 1.0f;
                }
            }
            break;
        case 6:
            if ((u8)BattleAudience_GetSysCtrl(index) == TRUE && BattleAudienceGetPtr(index)->status != 15) {
                BattleAudience_ChangeStatus(index, 10);
            }
            for (i = 0; i < 200; i++) {
                if ((u8)BattleAudience_GetEscapeChangeOK(i)) {
                    BattleAudience_ChangeStatus(i, 12);
                    audience->mTargetAudienceCount -= 1.0f;
                }
            }
        default:
            if ((u8)BattleAudience_GetSysCtrl(index) == TRUE && BattleAudienceGetPtr(index)->status != 15) {
                BattleAudience_ChangeStatus(index, 10);
            }
            audience->mTargetAudienceCount -= 1.0f;
            break;
    }
    BattleAudiencePuniAllEscape();
}

void BattleAudience_Case_FirstAttack_Good(void) {
    BattleAudienceAddTargetNum(0.02f * BattleAudienceBaseGetPtr()->mAudienceDeltaMultiplier, 0.0f);
}

void BattleAudience_Case_FirstAttack_Bad(void) {
    BattleAudienceAddTargetNum(-0.049999997f * BattleAudienceBaseGetPtr()->mAudienceDeltaMultiplier, 0.0f);
}

void BattleAudience_SetPresentItemNo(s32 no) {
    BattleAudienceBaseGetPtr()->presentItemNo = no;
}

s32 BattleAudience_GetPresentItemNo(void) {
    return BattleAudienceBaseGetPtr()->presentItemNo;
}

void BattleAudience_SetPresentTargetUnitId(s32 unitId) {
    BattleAudienceBaseGetPtr()->presentTargetUnitId = unitId;
}

s32 BattleAudience_GetPresentTargetUnitId(void) {
    return BattleAudienceBaseGetPtr()->presentTargetUnitId;
}

void BattleAudience_SetPresentItemType(s32 type) {
    BattleAudienceBaseGetPtr()->presentItemType = type;
}

s32 BattleAudience_GetPresentItemType(void) {
    return BattleAudienceBaseGetPtr()->presentItemType;
}
