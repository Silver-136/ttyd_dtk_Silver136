#include "battle/battle_event_subset.h"
#include "battle/battle_sub.h"

extern void* _battleWorkPointer;
extern s32 evtGetValue(void* evt, s32 var);
extern void evtSetValue(void* evt, s32 var, s32 value);
extern void* BattleGetUnitPtr(void* battleWork, s32 unitId);
extern void BtlUnit_GetPos(void* unit, f32* x, f32* y, f32* z);
extern void* effWhirlwindN64Entry(f32 height, s32 flip, s32 unk, s32 duration);

extern u32 subsetevt_dead[];
extern u32 subsetevt_dead_core[];
extern u32 subsetevt_dead_core_nospin_norotate[];
extern u32 btldefaultevt_avoid_guard[];
extern u32 subsetevt_damage[];
extern u32 subsetevt_counter_damage[];
extern u32 btldefaultevt_ChangePosition[];
extern u32 subsetevt_happy_heart[];
extern u32 subsetevt_happy_flower[];
extern u32 subsetevt_terror[];
extern u32 subsetevt_blow_damage[];
extern u32 subsetevt_blow_dead[];
extern u32 subsetevt_shot_damage[];
extern u32 subsetevt_shot_damage_kaiten[];
extern u32 subsetevt_shot_damage_ultra[];
extern u32 subsetevt_swallow_failed_damage[];
extern u32 subsetevt_swallow_spew_damage[];
extern u32 subsetevt_swallow_shot_damage[];
extern u32 subset_ultra_hammer_acrobat_damage_event[];
extern u32 subsetevt_confuse_flustered[];
extern u32 subsetevt_binta_damage_left[];
extern u32 subsetevt_binta_damage_right[];
extern u32 subsetevt_binta_damage_left_last[];
extern u32 subsetevt_binta_damage_right_last[];
extern u32 subsetevt_status_recovery[];
extern u32 unk_evt_803537c4[];
extern u32 subsetevt_nodamage_motion[];
extern u32 subsetevt_regeneration[];
extern u32 subset_attack_ready_jump_event[];

extern u32 marioWeapon_KaitenHammerInvolved[];
extern u32 marioWeapon_UltraHammerInvolved[];
extern u32 partyWeapon_YoshiNomikomi_Involved[];

extern s32 btlevtcmd_AddRotate();
extern s32 btlevtcmd_AnimeChangePoseFromTable();
extern s32 btlevtcmd_AnimeChangePoseType();
extern s32 btlevtcmd_AnimeCmpPoseType();
extern s32 btlevtcmd_AnimeFlagOnOff();
extern s32 btlevtcmd_AnimeSetMotionSpeed();
extern s32 btlevtcmd_AnimeWaitPlayComplete();
extern s32 btlevtcmd_CheckAttribute();
extern s32 btlevtcmd_CheckDamage();
extern s32 btlevtcmd_CheckDamageCode();
extern s32 btlevtcmd_CheckDamagePattern();
extern s32 btlevtcmd_CheckDataOfDataTable();
extern s32 btlevtcmd_CheckDefinePosetype();
extern s32 btlevtcmd_CheckStatus();
extern s32 btlevtcmd_CheckToken();
extern s32 btlevtcmd_ClearAllStatus();
extern s32 btlevtcmd_ConsumeItemReserve();
extern s32 btlevtcmd_DispItemIcon();
extern s32 btlevtcmd_DivePosition();
extern s32 btlevtcmd_EllipseMovePosition();
extern s32 btlevtcmd_FaceDirectionAdd();
extern s32 btlevtcmd_FaceDirectionSub();
extern s32 btlevtcmd_GetBackItem();
extern s32 btlevtcmd_GetBodyId();
extern s32 btlevtcmd_GetCoin();
extern s32 btlevtcmd_GetDamage();
extern s32 btlevtcmd_GetDamageCode();
extern s32 btlevtcmd_GetExp();
extern s32 btlevtcmd_GetFaceDirection();
extern s32 btlevtcmd_GetFriendBelong();
extern s32 btlevtcmd_GetGuardKouraId();
extern s32 btlevtcmd_GetHitPos();
extern s32 btlevtcmd_GetHomePos();
extern s32 btlevtcmd_GetHp();
extern s32 btlevtcmd_GetPartnerId();
extern s32 btlevtcmd_GetPos();
extern s32 btlevtcmd_GetScale();
extern s32 btlevtcmd_GetSelectEnemy();
extern s32 btlevtcmd_GetSelectNextEnemy();
extern s32 btlevtcmd_GetStageSize();
extern s32 btlevtcmd_GetUnitId();
extern s32 btlevtcmd_GetUnitKind();
extern s32 btlevtcmd_GetUnitWork();
extern s32 btlevtcmd_JumpContinue();
extern s32 btlevtcmd_JumpPosition();
extern s32 btlevtcmd_JumpSetting();
extern s32 btlevtcmd_KillUnit();
extern s32 btlevtcmd_MovePosition();
extern s32 btlevtcmd_OffPartsAttribute();
extern s32 btlevtcmd_OffUnitFlag();
extern s32 btlevtcmd_OnOffStatus();
extern s32 btlevtcmd_OnToken();
extern s32 btlevtcmd_OnUnitFlag();
extern s32 btlevtcmd_PreCheckDamage();
extern s32 btlevtcmd_RecoverHp();
extern s32 btlevtcmd_ResetFaceDirection();
extern s32 btlevtcmd_RunDataEventChild();
extern s32 btlevtcmd_SamplingEnemy();
extern s32 btlevtcmd_SetDispOffset();
extern s32 btlevtcmd_SetFallAccel();
extern s32 btlevtcmd_SetHomePos();
extern s32 btlevtcmd_SetHp();
extern s32 btlevtcmd_SetMoveSpeed();
extern s32 btlevtcmd_SetPos();
extern s32 btlevtcmd_SetRotate();
extern s32 btlevtcmd_SetRotateOffset();
extern s32 btlevtcmd_SetRotateOffsetFromCenterOffset();
extern s32 btlevtcmd_SetScale();
extern s32 btlevtcmd_SetUnitWork();
extern s32 btlevtcmd_SortPhaseMoveTable();
extern s32 btlevtcmd_StartAvoid();
extern s32 btlevtcmd_StartWaitEvent();
extern s32 btlevtcmd_StopWaitEvent();
extern s32 btlevtcmd_StoreCoin();
extern s32 btlevtcmd_StoreExp();
extern s32 btlevtcmd_TransStageFloorPosition();
extern s32 btlevtcmd_WaitAttackEnd();
extern s32 btlevtcmd_WaitEventEnd();
extern s32 btlevtcmd_snd_se();
extern s32 evt_btl_camera_set_homing_unit();
extern s32 evt_btl_camera_set_mode();
extern s32 evt_btl_camera_set_moveSpeedLv();
extern s32 evt_btl_camera_set_zoom();
extern s32 evt_eff();
extern s32 evt_snd_sfxon();
extern s32 evt_snd_sfxon_3d();
extern s32 evt_sub_intpl_get_float();
extern s32 evt_sub_intpl_get_value();
extern s32 evt_sub_intpl_init();
extern s32 evt_sub_intpl_init_float();

s32 _disable_restore_command_cursor(void) {
    void* battleWork;

    battleWork = _battleWorkPointer;
    *(s32*)((s32)battleWork + 0x1C70) = 0;
    *(u8*)((s32)battleWork + 0x1C74) = 0;

    return 2;
}


s32 _check_at_dm_event_wait(void* evt) {
    s32* args;
    void* battleWork;
    s32 type;
    void* unit;
    s32 evtId;

    args = *(s32**)((s32)evt + 0x18);
    battleWork = _battleWorkPointer;
    type = evtGetValue(evt, args[0]);
    unit = BattleGetUnitPtr(battleWork, BattleTransID(evt, type));
    if (unit == 0) {
        return 2;
    }

    evtId = *(s32*)((s32)unit + 0x2A8);
    if (evtId != 0 && evtCheckID(evtId) != 0) {
        return 0;
    }

    evtId = *(s32*)((s32)unit + 0x2B4);
    if (evtId != 0 && evtCheckID(evtId) != 0) {
        return 0;
    }

    return 2;
}


s32 _add_star_point_disp_offset(void* evt) {
    s32* args;
    void* battleWork;
    s32 type;
    void* unit;
    s32 xVar;
    s32 yVar;
    s32 zVar;
    f32 x;
    f32 y;
    f32 z;
    void* part;

    battleWork = _battleWorkPointer;
    args = *(s32**)((s32)evt + 0x18);
    type = evtGetValue(evt, args[0]);
    unit = BattleGetUnitPtr(battleWork, BattleTransID(evt, type));
    xVar = args[1];
    x = (f32)evtGetValue(evt, xVar);
    yVar = args[2];
    y = (f32)evtGetValue(evt, yVar);
    zVar = args[3];
    z = (f32)evtGetValue(evt, zVar);
    part = *(void**)((s32)unit + 0x10);
    x += *(f32*)((s32)part + 0x90);
    y += *(f32*)((s32)part + 0x94);
    z += *(f32*)((s32)part + 0x98);
    evtSetValue(evt, xVar, (s32)x);
    evtSetValue(evt, yVar, (s32)y);
    evtSetValue(evt, zVar, (s32)z);

    return 2;
}

s32 unk_80113f1c(void* evt) {
    s32* args;
    void* battleWork;
    s32 type;
    void* unit;

    args = *(s32**)((s32)evt + 0x18);
    battleWork = _battleWorkPointer;
    type = evtGetValue(evt, args[0]);
    unit = BattleGetUnitPtr(battleWork, BattleTransID(evt, type));
    if (unit != 0) {
        *(u16*)((s32)unit + 0x1FE) |= 1;
    }

    return 2;
}


s32 _check_through_pos(void* evt) {
    s32* args;
    void* battleWork;
    s32 type;
    s32 pos;
    s32 a;
    s32 b;
    s32 dst;
    void* unit;
    s32 min;
    s32 max;

    args = *(s32**)((s32)evt + 0x18);
    battleWork = _battleWorkPointer;
    type = evtGetValue(evt, args[0]);
    pos = evtGetValue(evt, args[1]);
    a = evtGetValue(evt, args[2]);
    b = evtGetValue(evt, args[3]);
    dst = args[4];
    unit = BattleGetUnitPtr(battleWork, BattleTransID(evt, type));

    if (a > b) {
        min = b;
        max = a;
    } else {
        min = a;
        max = b;
    }

    if (pos >= min && pos <= max) {
        evtSetValue(evt, dst, 1);
    } else {
        evtSetValue(evt, dst, 0);
        if (*(s8*)((s32)unit + 0x189) >= 0) {
            if (pos > max) {
                evtSetValue(evt, dst, 1);
            }
        } else if (pos < min) {
            evtSetValue(evt, dst, 1);
        }
    }

    return 2;
}


s32 _binta_effect(void* evt) {
    s32* args;
    void* battleWork;
    s32 type;
    s32 dir;
    void* unit;
    void* eff;
    f32 z;
    f32 y;
    f32 x;

    args = *(s32**)((s32)evt + 0x18);
    battleWork = _battleWorkPointer;
    type = evtGetValue(evt, args[0]);
    dir = evtGetValue(evt, args[1]);
    unit = BattleGetUnitPtr(battleWork, BattleTransID(evt, type));
    BtlUnit_GetPos(unit, &x, &y, &z);
    eff = effWhirlwindN64Entry(*(f32*)((s32)unit + 0x114), (u32)dir >> 31, 0, 0x5A);
    *(f32*)((s32)*(void**)((s32)eff + 0xC) + 8) = x;
    *(f32*)((s32)*(void**)((s32)eff + 0xC) + 0xC) = y;
    *(f32*)((s32)*(void**)((s32)eff + 0xC) + 0x10) = z;

    return 2;
}

const char str_SFX_PARTY_BATTLE_DIE_802ee928[] = "SFX_PARTY_BATTLE_DIE1";
const char str_recovery_802ee940[] = "recovery";
const char str_SFX_BTL_ENEMY_DIE1_1_802ee94c[] = "SFX_BTL_ENEMY_DIE1_1";
const char str_SFX_BTL_ENEMY_DIE1_2_802ee964[] = "SFX_BTL_ENEMY_DIE1_2";
const char str_kemuri_test_802ee97c[] = "kemuri_test";
const char str_star_point_802ee988[] = "star_point";
const char str_SFX_BTL_DAMAGE_PRESS_802ee994[] = "SFX_BTL_DAMAGE_PRESS1";
const char str_SFX_BTL_ATTACK_MISS2_802ee9ac[] = "SFX_BTL_ATTACK_MISS2";
const char str_SFX_VOICE_MARIO_DAMA_802ee9c4[] = "SFX_VOICE_MARIO_DAMAGED1";
const char str_SFX_BTL_ATTACK_MISS3_802ee9e0[] = "SFX_BTL_ATTACK_MISS3";
const char str_SFX_BTL_POS_CHANGE1_802ee9f8[] = "SFX_BTL_POS_CHANGE1";
const char str_SFX_ENM_ESCAPE1_802eea0c[] = "SFX_ENM_ESCAPE1";
const char str_SFX_BTL_CLAUD_BREATH_802eea1c[] = "SFX_BTL_CLAUD_BREATH3";
const f32 float_0_80422308 = 0.0f;

/* Reconstructed .data event bytecode; exact target words + symbolic relocations. */
u32 subsetevt_dead[] = {
    0x0001005B, (u32)_disable_restore_command_cursor, 0x0003005B, (u32)btlevtcmd_GetUnitKind, 0xFE363C8A, 0xFE363C80,
    0x00010022, 0xFE363C80, 0x00010024, 0x000000DE, 0x00010004, 0x00000000,
    0x0002002F, 0x000000E0, 0x000000E6, 0x00010004, 0x00000000, 0x0000002A,
    0x00010004, 0x00000001, 0x00000031, 0x00010003, 0x00000000, 0x0004005B,
    (u32)btlevtcmd_CheckAttribute, 0xFE363C8A, 0x00000004, 0xFE363C80, 0x00020019, 0xFE363C80,
    0x00000000, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFE363C8A, 0xFE363C8B, 0x0000002A,
    0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFE363C8A, 0xFE363C82, 0xFE363C83, 0xFE363C84,
    0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFE363C8A, 0xF24A8A80, 0x000A005B, (u32)btlevtcmd_DivePosition,
    0xFE363C8A, 0xFE363C82, 0xFE363C83, 0xFE363C84, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0xFFFFFFFF, 0x00000020, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType,
    0xFE363C8A, 0xFE363C8B, 0x00000028, 0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFE363C8A,
    0xFE363C82, 0xFE363C83, 0xFE363C84, 0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFE363C8A,
    0xF24A8A80, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFE363C8A, 0xFE363C82, 0xFE363C83,
    0xFE363C84, 0x00000000, 0xFFFFFFFF, 0x00000000, 0x00000021, 0x0002005B,
    (u32)btlevtcmd_ResetFaceDirection, 0xFE363C8A, 0x00010003, 0x00000001, 0x0006005B, (u32)btlevtcmd_OnOffStatus,
    0xFE363C8A, 0x0000001B, 0x00000001, 0x00000001, 0x00000001, 0x0003005B,
    (u32)btlevtcmd_GetUnitKind, 0xFE363C8A, 0xFE363C80, 0x00010022, 0xFE363C80, 0x00010024,
    0x000000DE, 0x0002005B, (u32)btlevtcmd_ClearAllStatus, 0xFE363C8A, 0x0004005B, (u32)btlevtcmd_CheckDamageCode,
    0xFE363C8A, 0x00000800, 0xFE363C81, 0x00020019, 0xFE363C81, 0x00000000,
    0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFE363C8A, 0x00000033, 0x00000020, 0x0003005B,
    (u32)btlevtcmd_RunDataEventChild, 0xFE363C8A, 0x00000031, 0x00000021, 0x0002002F, 0x000000E0,
    0x000000E6, 0x0004005B, (u32)btlevtcmd_CheckDamageCode, 0xFE363C8A, 0x00000800, 0xFE363C81,
    0x00020019, 0xFE363C81, 0x00000000, 0x0002005B, (u32)btlevtcmd_ClearAllStatus, 0xFE363C8A,
    0x0004005B, (u32)btlevtcmd_CheckDataOfDataTable, 0xFE363C8A, 0xFE363C80, 0x00000033, 0x00020019,
    0xFE363C80, 0x00000000, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFE363C8A, 0x00000033,
    0x00000020, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFE363C8A, 0xFE363C8B, 0x0000001B,
    0x0000006E, 0x0003005B, (u32)btlevtcmd_AnimeWaitPlayComplete, 0xFE363C8A, 0xFE363C8B, 0x0006005B,
    (u32)btlevtcmd_snd_se, 0xFE363C8A, (u32)(char*)str_SFX_PARTY_BATTLE_DIE_802ee928, 0xF1194D80, 0x00000000, 0xF1194D80,
    0x00000070, 0x00010009, 0x0000003C, 0x0002005B, (u32)btlevtcmd_ClearAllStatus, 0xFE363C8A,
    0x0003005B, (u32)btlevtcmd_ConsumeItemReserve, 0xFE363C8A, 0x00000097, 0x0000006E, 0x0005005B,
    (u32)btlevtcmd_GetPos, 0xFE363C8A, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00020035,
    0xFE363C81, 0x00000028, 0x0006005B, (u32)btlevtcmd_DispItemIcon, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000097, 0x0000005A, 0x00000070, 0x00010009, 0x0000005A,
    0x0004005B, (u32)btlevtcmd_CheckDataOfDataTable, 0xFE363C8A, 0xFE363C80, 0x00000043, 0x00020019,
    0xFE363C80, 0x00000000, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFE363C8A, 0x00000043,
    0x00000021, 0x0006005B, (u32)btlevtcmd_OnOffStatus, 0xFE363C8A, 0x0000001B, 0x00000000,
    0x00000000, 0x00000000, 0x0003005B, (u32)btlevtcmd_SetHp, 0xFE363C8A, 0x00000000,
    0x0004005B, (u32)btlevtcmd_RecoverHp, 0xFE363C8A, 0xFE363C8B, 0x0000000A, 0x0005005B,
    (u32)btlevtcmd_GetPos, 0xFE363C8A, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00020035,
    0xFE363C81, 0x0000001E, 0x000F005B, (u32)evt_eff, 0x00000000, (u32)(char*)str_recovery_802ee940,
    0x00000000, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0000000A, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x0001000A, 0x000007D0, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFE363C8A, 0xFE363C8B,
    0x0000002A, 0x0005005B, (u32)btlevtcmd_JumpSetting, 0xFE363C8A, 0x00000000, 0xF24A7A80,
    0xF24A7D4C, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFE363C8A, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000014, 0xFFFFFFFF, 0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE,
    0x00000021, 0x00000020, 0x0004005B, (u32)btlevtcmd_CheckDataOfDataTable, 0xFE363C8A, 0xFE363C80,
    0x00000031, 0x00020019, 0xFE363C80, 0x00000000, 0x0003005B, (u32)btlevtcmd_RunDataEventChild,
    0xFE363C8A, 0x00000031, 0x00000020, 0x00000021, 0x0003005B, (u32)btlevtcmd_KillUnit,
    0xFE363C8A, 0x00000001, 0x0003005B, (u32)btlevtcmd_GetBodyId, 0xFFFFFFFE, 0xFE363C8B,
    0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFE363C8A, 0xFE363C8B, 0x0000001B, 0x0000006E,
    0x0003005B, (u32)btlevtcmd_AnimeWaitPlayComplete, 0xFE363C8A, 0xFE363C8B, 0x0006005B, (u32)btlevtcmd_snd_se,
    0xFE363C8A, (u32)(char*)str_SFX_PARTY_BATTLE_DIE_802ee928, 0xF1194D80, 0x00000000, 0xF1194D80, 0x00000070,
    0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFE363C8A, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0003005B, (u32)btlevtcmd_GetPartnerId, 0xFE363C8A, 0xFE363C84, 0x0005005B, (u32)btlevtcmd_GetHomePos,
    0xFE363C84, 0xFE363C86, 0xFE363C87, 0xFE363C88, 0x0002001A, 0xFE363C80,
    0x00000000, 0x00020037, 0xFE363C80, 0xFFFFFFFF, 0x00000021, 0x0002001A,
    0xFE363C86, 0x00000000, 0x00020037, 0xFE363C86, 0xFFFFFFFF, 0x00000021,
    0x0002001A, 0xFE363C80, 0xFE363C86, 0x0001000A, 0x000001F4, 0x0002005B,
    (u32)_check_at_dm_event_wait, 0xFFFFFFFD, 0x0003005B, (u32)btlevtcmd_GetHp, 0xFFFFFFFD, 0xFE363C80,
    0x0002001B, 0xFE363C80, 0x00000000, 0x0001005E, (u32)btldefaultevt_ChangePosition, 0x00000021,
    0x00000021, 0x00000021, 0x0000002A, 0x0004005B, (u32)btlevtcmd_CheckDamageCode, 0xFE363C8A,
    0x00000800, 0xFE363C81, 0x00020019, 0xFE363C81, 0x00000000, 0x0004005B,
    (u32)btlevtcmd_CheckDataOfDataTable, 0xFE363C8A, 0xFE363C80, 0x00000033, 0x00020019, 0xFE363C80,
    0x00000000, 0x0002005B, (u32)btlevtcmd_ClearAllStatus, 0xFE363C8A, 0x0003005B, (u32)btlevtcmd_RunDataEventChild,
    0xFE363C8A, 0x00000033, 0x00000020, 0x0003005B, (u32)btlevtcmd_OnUnitFlag, 0xFE363C8A,
    0x08000000, 0x00020032, 0xFE363C80, 0x00000030, 0x00010003, 0x00000005,
    0x00010009, 0x00000001, 0x0005005B, (u32)btlevtcmd_AddRotate, 0xFE363C8A, 0x00000000,
    0xFFFFFFF1, 0x00000000, 0x00020036, 0xFE363C80, 0x00000001, 0x0002001B,
    0xFE363C80, 0x00000000, 0x00010004, 0x00000005, 0x00000021, 0x0005005B,
    (u32)btlevtcmd_SetRotate, 0xFE363C8A, 0x00000000, 0x00000000, 0x00000000, 0x0005005B,
    (u32)btlevtcmd_SetRotateOffset, 0xFE363C8A, 0x00000000, 0x00000000, 0x00000000, 0x00020032,
    0xFE363C80, 0x0000001E, 0x00010003, 0x00000006, 0x00010009, 0x00000001,
    0x0005005B, (u32)btlevtcmd_AddRotate, 0xFE363C8A, 0x00000003, 0x00000000, 0x00000000,
    0x00020036, 0xFE363C80, 0x00000001, 0x0002001B, 0xFE363C80, 0x00000000,
    0x00010004, 0x00000006, 0x00000021, 0x0002005B, (u32)btlevtcmd_ClearAllStatus, 0xFE363C8A,
    0x0003005B, (u32)btlevtcmd_ConsumeItemReserve, 0xFE363C8A, 0x00000097, 0x0000006E, 0x0005005B,
    (u32)btlevtcmd_GetPos, 0xFE363C8A, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00020035,
    0xFE363C81, 0x00000028, 0x0006005B, (u32)btlevtcmd_DispItemIcon, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000097, 0x00000078, 0x00000070, 0x00010009, 0x0000005A,
    0x00020032, 0xFE363C80, 0x0000001E, 0x00010003, 0x00000007, 0x00010009,
    0x00000001, 0x0005005B, (u32)btlevtcmd_AddRotate, 0xFE363C8A, 0xFFFFFFFD, 0x00000000,
    0x00000000, 0x00020036, 0xFE363C80, 0x00000001, 0x0002001B, 0xFE363C80,
    0x00000000, 0x00010004, 0x00000007, 0x00000021, 0x0006005B, (u32)btlevtcmd_OnOffStatus,
    0xFE363C8A, 0x0000001B, 0x00000000, 0x00000000, 0x00000000, 0x0003005B,
    (u32)btlevtcmd_SetHp, 0xFE363C8A, 0x00000000, 0x0004005B, (u32)btlevtcmd_RecoverHp, 0xFE363C8A,
    0xFE363C8B, 0x0000000A, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFE363C8A, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00020035, 0xFE363C81, 0x0000001E, 0x000F005B,
    (u32)evt_eff, 0x00000000, (u32)(char*)str_recovery_802ee940, 0x00000000, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0000000A, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x0001000A, 0x000007D0, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePoseType, 0xFE363C8A, 0xFE363C8B, 0x0000002A, 0x0005005B, (u32)btlevtcmd_JumpSetting,
    0xFE363C8A, 0x00000000, 0xF24A7A80, 0xF24A7D4C, 0x0005005B, (u32)btlevtcmd_GetPos,
    0xFE363C8A, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0007005B, (u32)btlevtcmd_JumpPosition,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000014, 0xFFFFFFFF,
    0x0003005B, (u32)btlevtcmd_OffUnitFlag, 0xFE363C8A, 0x08000000, 0x0003005B, (u32)btlevtcmd_OffUnitFlag,
    0xFE363C8A, 0x40000000, 0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000021,
    0x00000020, 0x0004005B, (u32)btlevtcmd_CheckDataOfDataTable, 0xFE363C8A, 0xFE363C80, 0x00000031,
    0x00020019, 0xFE363C80, 0x00000000, 0x0002005B, (u32)btlevtcmd_ClearAllStatus, 0xFE363C8A,
    0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFE363C8A, 0x00000031, 0x00000020, 0x0001005E,
    (u32)subsetevt_dead_core, 0x00000021, 0x00000021, 0x00000031, 0x00000002, 0x00000001,
};

u32 subsetevt_dead_core[] = {
    0x0004005B, (u32)btlevtcmd_CheckDataOfDataTable, 0xFE363C8A, 0xFE363C80, 0x00000032, 0x00020019,
    0xFE363C80, 0x00000000, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFE363C8A, 0x00000032,
    0x00000021, 0x00020032, 0xFE363C80, 0x00000030, 0x0003005B, (u32)btlevtcmd_OnUnitFlag,
    0xFE363C8A, 0x08000000, 0x0006005B, (u32)btlevtcmd_snd_se, 0xFE363C8A, (u32)(char*)str_SFX_BTL_ENEMY_DIE1_1_802ee94c,
    0xF1194D80, 0x00000000, 0xF1194D80, 0x00010003, 0x0000000A, 0x00010009,
    0x00000001, 0x0005005B, (u32)btlevtcmd_AddRotate, 0xFE363C8A, 0x00000000, 0xFFFFFFF1,
    0x00000000, 0x00020036, 0xFE363C80, 0x00000001, 0x0002001B, 0xFE363C80,
    0x00000000, 0x00010004, 0x0000000A, 0x00000021, 0x0005005B, (u32)btlevtcmd_SetRotate,
    0xFE363C8A, 0x00000000, 0x00000000, 0x00000000, 0x0006005B, (u32)btlevtcmd_snd_se,
    0xFE363C8A, (u32)(char*)str_SFX_BTL_ENEMY_DIE1_2_802ee964, 0xF1194D80, 0x00000000, 0xF1194D80, 0x0005005B,
    (u32)btlevtcmd_GetPos, 0xFE363C8A, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00020035,
    0xFE363C82, 0x0000000A, 0x000F005B, (u32)evt_eff, 0x00000000, (u32)(char*)str_kemuri_test_802ee97c,
    0x00000004, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0xF24A7DB3, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x0003005B, (u32)btlevtcmd_GetCoin, 0xFE363C8A, 0xFE363C80, 0x0002005B, (u32)btlevtcmd_StoreCoin,
    0xFE363C80, 0x0003005B, (u32)btlevtcmd_GetExp, 0xFE363C8A, 0xFE363C80, 0x0002005B,
    (u32)btlevtcmd_StoreExp, 0xFE363C80, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFE363C8A, 0xFE363C83,
    0xFE363C81, 0xFE363C82, 0x00020035, 0xFE363C82, 0x0000000A, 0x00020035,
    0xFE363C81, 0x0000001E, 0x0005005B, (u32)_add_star_point_disp_offset, 0xFE363C8A, 0xFE363C83,
    0xFE363C81, 0xFE363C82, 0x000F005B, (u32)evt_eff, (u32)(f32*)&float_0_80422308, (u32)(char*)str_star_point_802ee988,
    0x00000000, 0xFE363C83, 0xFE363C81, 0xFE363C82, 0xFE363C80, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x0002001D, 0xFE363C80, 0x00000001, 0x00010005, 0xFE363C80, 0x00000006,
    0x00000021, 0x0005005B, (u32)btlevtcmd_SetRotateOffset, 0xFE363C8A, 0x00000000, 0x00000000,
    0x00000000, 0x00020032, 0xFE363C80, 0x0000001E, 0x00010003, 0x00000014,
    0x00010009, 0x00000001, 0x0005005B, (u32)btlevtcmd_AddRotate, 0xFE363C8A, 0x00000003,
    0x00000000, 0x00000000, 0x00020036, 0xFE363C80, 0x00000001, 0x0002001B,
    0xFE363C80, 0x00000000, 0x00010004, 0x00000014, 0x00000021, 0x0002005B,
    (u32)btlevtcmd_GetBackItem, 0xFE363C8A, 0x0002005B, (u32)btlevtcmd_ClearAllStatus, 0xFE363C8A, 0x0004005B,
    (u32)btlevtcmd_CheckDamageCode, 0xFE363C8A, 0x00000400, 0xFE363C80, 0x00020019, 0xFE363C80,
    0x00000000, 0x0003005B, (u32)btlevtcmd_KillUnit, 0xFE363C8A, 0x00000000, 0x00000020,
    0x0003005B, (u32)btlevtcmd_KillUnit, 0xFE363C8A, 0x00000002, 0x00000021, 0x00000002,
    0x00000001,
};

u32 subsetevt_dead_core_nospin_norotate[] = {
    0x0004005B, (u32)btlevtcmd_CheckDataOfDataTable, 0xFE363C8A, 0xFE363C80, 0x00000032, 0x00020019,
    0xFE363C80, 0x00000000, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFE363C8A, 0x00000032,
    0x00000021, 0x0006005B, (u32)btlevtcmd_snd_se, 0xFE363C8A, (u32)(char*)str_SFX_BTL_ENEMY_DIE1_2_802ee964, 0xF1194D80,
    0x00000000, 0xF1194D80, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFE363C8A, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00020035, 0xFE363C82, 0x0000000A, 0x000F005B,
    (u32)evt_eff, 0x00000000, (u32)(char*)str_kemuri_test_802ee97c, 0x00000004, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0xF24A7DB3, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x0003005B, (u32)btlevtcmd_GetCoin, 0xFE363C8A,
    0xFE363C80, 0x0002005B, (u32)btlevtcmd_StoreCoin, 0xFE363C80, 0x0003005B, (u32)btlevtcmd_GetExp,
    0xFE363C8A, 0xFE363C80, 0x0002005B, (u32)btlevtcmd_StoreExp, 0xFE363C80, 0x0005005B,
    (u32)btlevtcmd_GetPos, 0xFE363C8A, 0xFE363C83, 0xFE363C81, 0xFE363C82, 0x00020035,
    0xFE363C82, 0x0000000A, 0x00020035, 0xFE363C81, 0x0000001E, 0x0005005B,
    (u32)_add_star_point_disp_offset, 0xFE363C8A, 0xFE363C83, 0xFE363C81, 0xFE363C82, 0x000F005B,
    (u32)evt_eff, (u32)(f32*)&float_0_80422308, (u32)(char*)str_star_point_802ee988, 0x00000000, 0xFE363C83, 0xFE363C81,
    0xFE363C82, 0xFE363C80, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x0002001D, 0xFE363C80, 0x00000001,
    0x00010005, 0xFE363C80, 0x00000006, 0x00000021, 0x00010009, 0x0000001E,
    0x0002005B, (u32)btlevtcmd_GetBackItem, 0xFE363C8A, 0x0004005B, (u32)btlevtcmd_CheckDamageCode, 0xFE363C8A,
    0x00000400, 0xFE363C80, 0x00020019, 0xFE363C80, 0x00000000, 0x0003005B,
    (u32)btlevtcmd_KillUnit, 0xFE363C8A, 0x00000001, 0x00000020, 0x0003005B, (u32)btlevtcmd_KillUnit,
    0xFE363C8A, 0x00000002, 0x00000021, 0x00000002, 0x00000001,
};

u32 btldefaultevt_avoid_guard[] = {
    0x0006005B, (u32)btlevtcmd_AnimeCmpPoseType, 0xFE363C8A, 0xFE363C8B, 0x0000002A, 0x00000028,
    0xFE363C82, 0x00020018, 0xFE363C82, 0x00000000, 0x0004005B, (u32)btlevtcmd_AnimeSetMotionSpeed,
    0xFE363C8A, 0xFE363C8B, 0xF24A8280, 0x00000021, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType,
    0xFE363C8A, 0xFE363C8B, 0x0000002A, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE,
    0xFE363C80, 0x00000014, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000014, 0xFFFFFFFF, 0x00000000, 0x0004005B,
    (u32)btlevtcmd_AnimeSetMotionSpeed, 0xFE363C8A, 0xFE363C8B, 0xF24A7E80, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType,
    0xFE363C8A, 0xFE363C8B, 0x0000002E, 0x0001005B, (u32)btlevtcmd_WaitAttackEnd, 0x00020018,
    0xFE363C8E, 0xFE363C8F, 0x0004005B, (u32)btlevtcmd_AnimeSetMotionSpeed, 0xFE363C8A, 0xFE363C8B,
    0xF24A8280, 0x00000021, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFE363C8A, 0xFE363C8B,
    0x0000002A, 0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xFE363C82, 0xFE363C83,
    0xFE363C84, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C82, 0xFE363C83,
    0xFE363C84, 0x00000014, 0xFFFFFFFF, 0x00000000, 0x0004005B, (u32)btlevtcmd_AnimeSetMotionSpeed,
    0xFE363C8A, 0xFE363C8B, 0xF24A7E80, 0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE,
    0x00000002, 0x00000001,
};

u32 subsetevt_damage[] = {
    0x0004005B, (u32)btlevtcmd_CheckDamageCode, 0xFE363C8A, 0x00200000, 0xFE363C80, 0x00020019,
    0xFE363C80, 0x00000000, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFE363C8A, 0x0000000D,
    0x00000002, 0x00000021, 0x0004005B, (u32)btlevtcmd_CheckDamageCode, 0xFE363C8A, 0x00400000,
    0xFE363C80, 0x00020019, 0xFE363C80, 0x00000000, 0x0003005B, (u32)btlevtcmd_RunDataEventChild,
    0xFE363C8A, 0x0000000E, 0x00000002, 0x00000021, 0x0004005B, (u32)btlevtcmd_CheckDamagePattern,
    0xFE363C8A, 0x00000009, 0xFE363C80, 0x00020019, 0xFE363C80, 0x00000000,
    0x0004005B, (u32)btlevtcmd_CheckDataOfDataTable, 0xFE363C8A, 0xFE363C80, 0x00000011, 0x00020019,
    0xFE363C80, 0x00000000, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFE363C8A, 0x00000011,
    0x00000002, 0x00000021, 0x00000021, 0x0004005B, (u32)btlevtcmd_CheckDamagePattern, 0xFE363C8A,
    0x00000008, 0xFE363C80, 0x00020019, 0xFE363C80, 0x00000000, 0x0004005B,
    (u32)btlevtcmd_CheckDataOfDataTable, 0xFE363C8A, 0xFE363C80, 0x00000010, 0x00020019, 0xFE363C80,
    0x00000000, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFE363C8A, 0x00000010, 0x00000002,
    0x00000021, 0x00000021, 0x0004005B, (u32)btlevtcmd_CheckDamagePattern, 0xFE363C8A, 0x0000000E,
    0xFE363C80, 0x00020019, 0xFE363C80, 0x00000000, 0x0004005B, (u32)btlevtcmd_CheckDataOfDataTable,
    0xFE363C8A, 0xFE363C80, 0x00000012, 0x00020019, 0xFE363C80, 0x00000000,
    0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFE363C8A, 0x00000012, 0x00000002, 0x00000021,
    0x00000021, 0x0004005B, (u32)btlevtcmd_CheckDamagePattern, 0xFE363C8A, 0x00000010, 0xFE363C80,
    0x00020019, 0xFE363C80, 0x00000000, 0x0004005B, (u32)btlevtcmd_CheckDataOfDataTable, 0xFE363C8A,
    0xFE363C80, 0x00000012, 0x00020019, 0xFE363C80, 0x00000000, 0x0003005B,
    (u32)btlevtcmd_RunDataEventChild, 0xFE363C8A, 0x00000012, 0x00000002, 0x00000021, 0x00000021,
    0x0004005B, (u32)btlevtcmd_CheckDamagePattern, 0xFE363C8A, 0x0000000F, 0xFE363C80, 0x00020019,
    0xFE363C80, 0x00000000, 0x0004005B, (u32)btlevtcmd_CheckDataOfDataTable, 0xFE363C8A, 0xFE363C80,
    0x00000013, 0x00020019, 0xFE363C80, 0x00000000, 0x0003005B, (u32)btlevtcmd_RunDataEventChild,
    0xFE363C8A, 0x00000013, 0x00000002, 0x00000021, 0x00000021, 0x0004005B,
    (u32)btlevtcmd_CheckDamagePattern, 0xFE363C8A, 0x00000011, 0xFE363C80, 0x00020019, 0xFE363C80,
    0x00000000, 0x0004005B, (u32)btlevtcmd_CheckDataOfDataTable, 0xFE363C8A, 0xFE363C80, 0x00000013,
    0x00020019, 0xFE363C80, 0x00000000, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFE363C8A,
    0x00000013, 0x00000002, 0x00000021, 0x00000021, 0x0004005B, (u32)btlevtcmd_CheckDamagePattern,
    0xFE363C8A, 0x00000015, 0xFE363C80, 0x00020019, 0xFE363C80, 0x00000000,
    0x0004005B, (u32)btlevtcmd_CheckDataOfDataTable, 0xFE363C8A, 0xFE363C80, 0x00000014, 0x00020019,
    0xFE363C80, 0x00000000, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFE363C8A, 0x00000014,
    0x00000002, 0x00000021, 0x00000021, 0x0004005B, (u32)btlevtcmd_CheckDamagePattern, 0xFE363C8A,
    0x00000016, 0xFE363C80, 0x00020019, 0xFE363C80, 0x00000000, 0x0004005B,
    (u32)btlevtcmd_CheckDataOfDataTable, 0xFE363C8A, 0xFE363C80, 0x00000015, 0x00020019, 0xFE363C80,
    0x00000000, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFE363C8A, 0x00000015, 0x00000002,
    0x00000021, 0x00000021, 0x0004005B, (u32)btlevtcmd_CheckDamagePattern, 0xFE363C8A, 0x00000017,
    0xFE363C80, 0x00020019, 0xFE363C80, 0x00000000, 0x0004005B, (u32)btlevtcmd_CheckDataOfDataTable,
    0xFE363C8A, 0xFE363C80, 0x00000016, 0x00020019, 0xFE363C80, 0x00000000,
    0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFE363C8A, 0x00000016, 0x00000002, 0x00000021,
    0x00000021, 0x0004005B, (u32)btlevtcmd_CheckDamagePattern, 0xFE363C8A, 0x00000018, 0xFE363C80,
    0x00020019, 0xFE363C80, 0x00000000, 0x0004005B, (u32)btlevtcmd_CheckDataOfDataTable, 0xFE363C8A,
    0xFE363C80, 0x00000017, 0x00020019, 0xFE363C80, 0x00000000, 0x0003005B,
    (u32)btlevtcmd_RunDataEventChild, 0xFE363C8A, 0x00000017, 0x00000002, 0x00000021, 0x00000021,
    0x0004005B, (u32)btlevtcmd_CheckDamagePattern, 0xFE363C8A, 0x00000019, 0xFE363C80, 0x00020019,
    0xFE363C80, 0x00000000, 0x0001005E, (u32)subsetevt_blow_damage, 0x0005005B, (u32)btlevtcmd_SetRotate,
    0xFE363C8A, 0x00000000, 0x00000000, 0x00000000, 0x00000002, 0x00000021,
    0x0004005B, (u32)btlevtcmd_CheckDamagePattern, 0xFE363C8A, 0x00000004, 0xFE363C80, 0x00020019,
    0xFE363C80, 0x00000000, 0x0004005B, (u32)btlevtcmd_CheckDataOfDataTable, 0xFE363C8A, 0xFE363C80,
    0x00000037, 0x00020019, 0xFE363C80, 0x00000000, 0x0003005B, (u32)btlevtcmd_RunDataEventChild,
    0xFE363C8A, 0x00000037, 0x00000002, 0x00000021, 0x00000021, 0x0004005B,
    (u32)btlevtcmd_CheckDamagePattern, 0xFE363C8A, 0x00000005, 0xFE363C80, 0x00020019, 0xFE363C80,
    0x00000000, 0x0004005B, (u32)btlevtcmd_CheckDataOfDataTable, 0xFE363C8A, 0xFE363C80, 0x00000038,
    0x00020019, 0xFE363C80, 0x00000000, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFE363C8A,
    0x00000038, 0x00000002, 0x00000021, 0x00000021, 0x0004005B, (u32)btlevtcmd_CheckDamagePattern,
    0xFE363C8A, 0x00000006, 0xFE363C80, 0x00020019, 0xFE363C80, 0x00000000,
    0x0004005B, (u32)btlevtcmd_CheckDataOfDataTable, 0xFE363C8A, 0xFE363C80, 0x00000039, 0x00020019,
    0xFE363C80, 0x00000000, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFE363C8A, 0x00000039,
    0x00000002, 0x00000021, 0x00000021, 0x0004005B, (u32)btlevtcmd_CheckDataOfDataTable, 0xFE363C8A,
    0xFE363C80, 0x00000009, 0x00020019, 0xFE363C80, 0x00000000, 0x0003005B,
    (u32)btlevtcmd_RunDataEventChild, 0xFE363C8A, 0x00000009, 0x00000002, 0x00000021, 0x0004005B,
    (u32)btlevtcmd_CheckDamagePattern, 0xFE363C8A, 0x00000004, 0xFE363C80, 0x00020019, 0xFE363C80,
    0x00000000, 0x0001005E, (u32)subsetevt_swallow_failed_damage, 0x00000002, 0x00000021, 0x0004005B,
    (u32)btlevtcmd_CheckDamagePattern, 0xFE363C8A, 0x00000005, 0xFE363C80, 0x00020019, 0xFE363C80,
    0x00000000, 0x0001005E, (u32)subsetevt_swallow_spew_damage, 0x00000002, 0x00000021, 0x0004005B,
    (u32)btlevtcmd_CheckDamagePattern, 0xFE363C8A, 0x00000006, 0xFE363C80, 0x00020019, 0xFE363C80,
    0x00000000, 0x0001005E, (u32)subsetevt_swallow_shot_damage, 0x00000002, 0x00000021, 0x0003005B,
    (u32)btlevtcmd_GetDamage, 0xFE363C8A, 0xFE363C80, 0x0002001D, 0xFE363C80, 0x00000001,
    0x0004005B, (u32)btlevtcmd_CheckDamageCode, 0xFE363C8A, 0x00000100, 0xFE363C80, 0x00020019,
    0xFE363C80, 0x00000000, 0x0004005B, (u32)btlevtcmd_CheckDamagePattern, 0xFE363C8A, 0x0000000A,
    0xFE363C80, 0x00020019, 0xFE363C80, 0x00000000, 0x00010004, 0x0000000A,
    0x00000021, 0x0004005B, (u32)btlevtcmd_CheckDamagePattern, 0xFE363C8A, 0x0000000C, 0xFE363C80,
    0x00020019, 0xFE363C80, 0x00000000, 0x00010004, 0x0000000A, 0x00000021,
    0x0004005B, (u32)btlevtcmd_CheckDamagePattern, 0xFE363C8A, 0x0000000B, 0xFE363C80, 0x00020019,
    0xFE363C80, 0x00000000, 0x00010004, 0x0000000A, 0x00000021, 0x00010004,
    0x0000000B, 0x00010003, 0x0000000A, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFE363C8A,
    0xFE363C8B, 0xFE363C8E, 0x0001006F, 0xFE363C80, 0x0005005B, (u32)evt_sub_intpl_init_float,
    0x00000005, 0xF24A7E80, 0xF24A7B19, 0x0000000A, 0x00010005, 0x0000000B,
    0x0003005B, (u32)evt_sub_intpl_get_float, 0xFE363C85, 0xF1194D80, 0x0005005B, (u32)btlevtcmd_GetScale,
    0xFE363C8A, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)btlevtcmd_SetScale,
    0xFE363C8A, 0xFE363C80, 0xFE363C85, 0xFE363C82, 0x00010009, 0x00000001,
    0x00000006, 0x00000070, 0x0002005B, (u32)btlevtcmd_WaitEventEnd, 0xFE363C80, 0x0004005B,
    (u32)btlevtcmd_CheckDamagePattern, 0xFE363C8A, 0x0000000B, 0xFE363C81, 0x00020019, 0xFE363C81,
    0x00000000, 0x0001000A, 0x00000BB8, 0x00000020, 0x0004005B, (u32)btlevtcmd_CheckDamagePattern,
    0xFE363C8A, 0x0000000C, 0xFE363C81, 0x00020019, 0xFE363C81, 0x00000000,
    0x0001000A, 0x000009C4, 0x00000020, 0x0001000A, 0x000005DC, 0x00000021,
    0x00000021, 0x0000006E, 0x0003005B, (u32)btlevtcmd_GetFriendBelong, 0xFE363C8A, 0xFE363C80,
    0x00020018, 0xFE363C80, 0x00000000, 0x0006005B, (u32)btlevtcmd_snd_se, 0xFE363C8A,
    (u32)(char*)str_SFX_BTL_DAMAGE_PRESS_802ee994, 0xF1194D80, 0x00000000, 0xF1194D80, 0x00000021, 0x00000070,
    0x0000006E, 0x0005005B, (u32)evt_sub_intpl_init_float, 0x00000005, 0xF24A7B19, 0xF24A7F80,
    0x0000000A, 0x00010005, 0x0000000A, 0x0003005B, (u32)evt_sub_intpl_get_float, 0xFE363C85,
    0xF1194D80, 0x0005005B, (u32)btlevtcmd_GetScale, 0xFE363C8A, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0005005B, (u32)btlevtcmd_SetScale, 0xFE363C8A, 0xFE363C80, 0xFE363C85,
    0xFE363C82, 0x00010009, 0x00000001, 0x00000006, 0x0005005B, (u32)evt_sub_intpl_init_float,
    0x00000007, 0xF24A7F80, 0xF24A7E80, 0x0000001E, 0x00010005, 0x0000001E,
    0x0003005B, (u32)evt_sub_intpl_get_float, 0xFE363C85, 0xF1194D80, 0x0005005B, (u32)btlevtcmd_GetScale,
    0xFE363C8A, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)btlevtcmd_SetScale,
    0xFE363C8A, 0xFE363C80, 0xFE363C85, 0xFE363C82, 0x00010009, 0x00000001,
    0x00000006, 0x00000070, 0x00010009, 0x0000003C, 0x00000002, 0x00010003,
    0x0000000B, 0x0004005B, (u32)btlevtcmd_CheckDamagePattern, 0xFE363C8A, 0x0000000E, 0xFE363C80,
    0x00020019, 0xFE363C80, 0x00000000, 0x0001005E, (u32)subsetevt_shot_damage, 0x00000002,
    0x00000021, 0x00000021, 0x0004005B, (u32)btlevtcmd_CheckDamagePattern, 0xFE363C8A, 0x00000010,
    0xFE363C80, 0x00020019, 0xFE363C80, 0x00000000, 0x0001005E, (u32)subsetevt_shot_damage,
    0x00000002, 0x00000021, 0x0004005B, (u32)btlevtcmd_CheckDamagePattern, 0xFE363C8A, 0x00000015,
    0xFE363C80, 0x00020019, 0xFE363C80, 0x00000000, 0x0001005E, (u32)subsetevt_binta_damage_left,
    0x00000002, 0x00000021, 0x0004005B, (u32)btlevtcmd_CheckDamagePattern, 0xFE363C8A, 0x00000016,
    0xFE363C80, 0x00020019, 0xFE363C80, 0x00000000, 0x0001005E, (u32)subsetevt_binta_damage_right,
    0x00000002, 0x00000021, 0x0004005B, (u32)btlevtcmd_CheckDamagePattern, 0xFE363C8A, 0x00000017,
    0xFE363C80, 0x00020019, 0xFE363C80, 0x00000000, 0x0001005E, (u32)subsetevt_binta_damage_left_last,
    0x00000002, 0x00000021, 0x0004005B, (u32)btlevtcmd_CheckDamagePattern, 0xFE363C8A, 0x00000018,
    0xFE363C80, 0x00020019, 0xFE363C80, 0x00000000, 0x0001005E, (u32)subsetevt_binta_damage_right_last,
    0x00000002, 0x00000021, 0x0004005B, (u32)btlevtcmd_CheckDamagePattern, 0xFE363C8A, 0x0000000D,
    0xFE363C80, 0x00020019, 0xFE363C80, 0x00000000, 0x0005005B, (u32)btlevtcmd_AnimeFlagOnOff,
    0xFE363C8A, 0xFE363C8B, 0x00000020, 0x00000001, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType,
    0xFE363C8A, 0xFE363C8B, 0xFE363C8E, 0x0001000A, 0x000007D0, 0x0005005B,
    (u32)btlevtcmd_AnimeFlagOnOff, 0xFE363C8A, 0xFE363C8B, 0x00000020, 0x00000000, 0x00000020,
    0x0004005B, (u32)btlevtcmd_CheckDamagePattern, 0xFE363C8A, 0x0000001A, 0xFE363C80, 0x00020019,
    0xFE363C80, 0x00000000, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFE363C8A, 0xFE363C8B,
    0xFE363C8E, 0x0001000A, 0x000007D0, 0x00020019, 0xFE363C8F, 0xFFFFFFFF,
    0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFE363C8A, 0xFE363C8B, 0xFE363C8F, 0x00010009,
    0x0000001E, 0x00000021, 0x00000020, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFE363C8A,
    0xFE363C8B, 0xFE363C8E, 0x0001000A, 0x000003E8, 0x00020019, 0xFE363C8F,
    0xFFFFFFFF, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFE363C8A, 0xFE363C8B, 0xFE363C8F,
    0x00010009, 0x0000001E, 0x00000021, 0x00000021, 0x00000021, 0x00000002,
    0x00000020, 0x0004005B, (u32)btlevtcmd_CheckDamageCode, 0xFE363C8A, 0x00000100, 0xFE363C80,
    0x00020019, 0xFE363C80, 0x00000000, 0x0002005B, (u32)unk_80113f1c, 0xFE363C8A,
    0x00000021, 0x0003005B, (u32)btlevtcmd_GetDamageCode, 0xFE363C8A, 0xFE363C80, 0x00010022,
    0xFE363C80, 0x00010024, 0x00000017, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFE363C8A,
    0xFE363C8B, 0xFE363C8E, 0x0001000A, 0x000003E8, 0x00020019, 0xFE363C8F,
    0xFFFFFFFF, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFE363C8A, 0xFE363C8B, 0xFE363C8F,
    0x00010009, 0x0000001E, 0x00000021, 0x00000002, 0x0000002A, 0x0003005B,
    (u32)evt_snd_sfxon, (u32)(char*)str_SFX_BTL_ATTACK_MISS2_802ee9ac, 0x00000000, 0x0005005B, (u32)btlevtcmd_SetDispOffset, 0xFE363C8A,
    0xFFFFFFFE, 0x00000000, 0x00000000, 0x00010009, 0x00000001, 0x0005005B,
    (u32)btlevtcmd_SetDispOffset, 0xFE363C8A, 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00010009,
    0x00000001, 0x0005005B, (u32)btlevtcmd_SetDispOffset, 0xFE363C8A, 0x00000000, 0x00000000,
    0x00000000, 0x00010009, 0x00000001, 0x0005005B, (u32)btlevtcmd_SetDispOffset, 0xFE363C8A,
    0xFFFFFFFE, 0x00000000, 0x00000000, 0x00010009, 0x00000001, 0x0005005B,
    (u32)btlevtcmd_SetDispOffset, 0xFE363C8A, 0xFFFFFFFC, 0x00000000, 0x00000000, 0x00010009,
    0x00000001, 0x0005005B, (u32)btlevtcmd_SetDispOffset, 0xFE363C8A, 0xFFFFFFFE, 0x00000000,
    0x00000000, 0x00010009, 0x00000001, 0x0005005B, (u32)btlevtcmd_SetDispOffset, 0xFE363C8A,
    0x00000000, 0x00000000, 0x00000000, 0x00010009, 0x00000001, 0x0005005B,
    (u32)btlevtcmd_SetDispOffset, 0xFE363C8A, 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00010009,
    0x00000001, 0x0005005B, (u32)btlevtcmd_SetDispOffset, 0xFE363C8A, 0xFFFFFFFE, 0x00000000,
    0x00000000, 0x00010009, 0x00000001, 0x0005005B, (u32)btlevtcmd_SetDispOffset, 0xFE363C8A,
    0xFFFFFFFF, 0x00000000, 0x00000000, 0x00010009, 0x00000001, 0x0005005B,
    (u32)btlevtcmd_SetDispOffset, 0xFE363C8A, 0x00000000, 0x00000000, 0x00000000, 0x00010009,
    0x00000001, 0x0005005B, (u32)btlevtcmd_SetDispOffset, 0xFE363C8A, 0xFFFFFFFF, 0x00000000,
    0x00000000, 0x00010009, 0x00000001, 0x0005005B, (u32)btlevtcmd_SetDispOffset, 0xFE363C8A,
    0xFFFFFFFD, 0x00000000, 0x00000000, 0x00010009, 0x00000001, 0x0005005B,
    (u32)btlevtcmd_SetDispOffset, 0xFE363C8A, 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00010009,
    0x00000001, 0x0005005B, (u32)btlevtcmd_SetDispOffset, 0xFE363C8A, 0x00000000, 0x00000000,
    0x00000000, 0x00010009, 0x00000001, 0x0005005B, (u32)btlevtcmd_SetDispOffset, 0xFE363C8A,
    0xFFFFFFFF, 0x00000000, 0x00000000, 0x00010009, 0x00000001, 0x0005005B,
    (u32)btlevtcmd_SetDispOffset, 0xFE363C8A, 0xFFFFFFFE, 0x00000000, 0x00000000, 0x00010009,
    0x00000001, 0x0005005B, (u32)btlevtcmd_SetDispOffset, 0xFE363C8A, 0xFFFFFFFF, 0x00000000,
    0x00000000, 0x00010009, 0x00000001, 0x0005005B, (u32)btlevtcmd_SetDispOffset, 0xFE363C8A,
    0x00000000, 0x00000000, 0x00000000, 0x00010009, 0x00000001, 0x0005005B,
    (u32)btlevtcmd_SetDispOffset, 0xFE363C8A, 0x00000000, 0x00000000, 0x00000000, 0x00010009,
    0x00000001, 0x0005005B, (u32)btlevtcmd_SetDispOffset, 0xFE363C8A, 0xFFFFFFFF, 0x00000000,
    0x00000000, 0x00010009, 0x00000001, 0x0005005B, (u32)btlevtcmd_SetDispOffset, 0xFE363C8A,
    0xFFFFFFFF, 0x00000000, 0x00000000, 0x00010009, 0x00000001, 0x0005005B,
    (u32)btlevtcmd_SetDispOffset, 0xFE363C8A, 0x00000000, 0x00000000, 0x00000000, 0x00010009,
    0x00000001, 0x0005005B, (u32)btlevtcmd_SetDispOffset, 0xFE363C8A, 0x00000000, 0x00000000,
    0x00000000, 0x00010009, 0x00000001, 0x00000031, 0x00000021, 0x00000002,
    0x00000001,
};

u32 subsetevt_counter_damage[] = {
    0x0003005B, (u32)btlevtcmd_OnUnitFlag, 0xFE363C8A, 0x00000001, 0x0000006E, 0x00010022,
    0xFE363C80, 0x0001002B, 0x0000002B, 0x0001002B, 0x0000002C, 0x0001002B,
    0x0000002D, 0x0003005B, (u32)btlevtcmd_GetUnitKind, 0xFE363C8A, 0xFE363C82, 0x00020018,
    0xFE363C82, 0x000000DE, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFE363C8A, 0xFE363C82,
    0xFE363C83, 0xFE363C84, 0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_VOICE_MARIO_DAMA_802ee9c4, 0xFE363C82,
    0xFE363C83, 0xFE363C84, 0x00000000, 0x00000021, 0x0000002E, 0x00000031,
    0x00000070, 0x00010022, 0xFE363C80, 0x00010024, 0x0000002B, 0x0003005B,
    (u32)evt_snd_sfxon, (u32)(char*)str_SFX_BTL_ATTACK_MISS3_802ee9e0, 0x00000000, 0x00020032, 0xFE363C8E, 0x00000024,
    0x00010024, 0x0000002E, 0x0003005B, (u32)btlevtcmd_GetUnitKind, 0xFE363C8A, 0xFE363C82,
    0x00020018, 0xFE363C82, 0x000000DE, 0x0006005B, (u32)btlevtcmd_snd_se, 0xFE363C8A,
    (u32)(char*)str_SFX_VOICE_MARIO_DAMA_802ee9c4, 0xF1194D80, 0x00000000, 0xF1194D80, 0x00000021, 0x00020032,
    0xFE363C8E, 0x0000001D, 0x0001002B, 0x00000031, 0x0001002B, 0x00000032,
    0x0001002B, 0x00000036, 0x00020032, 0xFE363C8E, 0x0000001D, 0x0000002E,
    0x0000002A, 0x00020032, 0xFE363C8E, 0x00000027, 0x00000031, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePoseType, 0xFE363C8A, 0xFE363C8B, 0xFE363C8E, 0x0000006E, 0x0003005B,
    (u32)btlevtcmd_RunDataEventChild, 0xFE363C8A, 0x0000000A, 0x00000070, 0x0003005B, (u32)btlevtcmd_SetFallAccel,
    0xFE363C8A, 0xF24A7E80, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFE363C8A, 0xFE363C82,
    0xFE363C83, 0xFE363C84, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFE363C8A, 0xFE363C82,
    0x00000019, 0x00020035, 0xFE363C83, 0x00000019, 0x000A005B, (u32)btlevtcmd_DivePosition,
    0xFE363C8A, 0xFE363C82, 0xFE363C83, 0xFE363C84, 0x0000000A, 0x00000000,
    0x00000000, 0x00000000, 0xFFFFFFFF, 0x0001000A, 0x000001F4, 0x0003005B,
    (u32)btlevtcmd_SetFallAccel, 0xFE363C8A, 0xF24A7E80, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFE363C8A,
    0xFE363C82, 0xFE363C83, 0xFE363C84, 0x0004005B, (u32)btlevtcmd_TransStageFloorPosition, 0xFE363C82,
    0xFE363C83, 0xFE363C84, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFE363C8A, 0xFE363C82,
    0x0000001E, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFE363C8A, 0xFE363C82, 0xFE363C83,
    0xFE363C84, 0x00000014, 0xFFFFFFFF, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFE363C8A,
    0xFE363C82, 0x0000000F, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFE363C8A, 0xFE363C82,
    0xFE363C83, 0xFE363C84, 0x0000000A, 0xFFFFFFFF, 0x00010009, 0x0000001E,
    0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000, 0x00000002, 0x00000001,
};

u32 btldefaultevt_ChangePosition[] = {
    0x0003005B, (u32)btlevtcmd_GetGuardKouraId, 0xFFFFFFFD, 0xFE363C8A, 0x00020019, 0xFE363C8A,
    0xFFFFFFFF, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFE363C8A, 0x0000003C, 0x0004005B,
    (u32)btlevtcmd_OffPartsAttribute, 0xFFFFFFFD, 0x00000001, 0x01000000, 0x0004005B, (u32)btlevtcmd_SetUnitWork,
    0xFE363C8A, 0x00000002, 0x00000002, 0x00000021, 0x0003005B, (u32)evt_snd_sfxon,
    (u32)(char*)str_SFX_BTL_POS_CHANGE1_802ee9f8, 0x00000000, 0x0003005B, (u32)btlevtcmd_GetPartnerId, 0xFFFFFFFE, 0xFE363C83,
    0x0003005B, (u32)btlevtcmd_GetBodyId, 0xFFFFFFFE, 0xFE363C85, 0x0004005B, (u32)btlevtcmd_CheckStatus,
    0xFFFFFFFE, 0x0000001B, 0xFE363C80, 0x00020018, 0xFE363C80, 0x00000000,
    0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0xFE363C85, 0x0000002A, 0x00000021,
    0x0003005B, (u32)btlevtcmd_GetBodyId, 0xFE363C83, 0xFE363C85, 0x0004005B, (u32)btlevtcmd_CheckStatus,
    0xFE363C83, 0x0000001B, 0xFE363C80, 0x00020018, 0xFE363C80, 0x00000000,
    0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFE363C83, 0xFE363C85, 0x0000002A, 0x00000021,
    0x0000006B, 0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFE363C83, 0xFE363C85, 0xFE363C86,
    0xFE363C87, 0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xFE363C85, 0xFE363C88,
    0xFE363C87, 0x0008005B, (u32)btlevtcmd_EllipseMovePosition, 0xFE363C83, 0xFE363C85, 0xFE363C86,
    0xFE363C87, 0x00000014, 0x0000000F, 0x00000000, 0x0005005B, (u32)btlevtcmd_SetHomePos,
    0xFE363C83, 0xFE363C85, 0xFE363C86, 0xFE363C87, 0x0000006D, 0x0005005B,
    (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B,
    (u32)btlevtcmd_GetHomePos, 0xFE363C83, 0xFE363C80, 0xFE363C84, 0xFE363C82, 0x0008005B,
    (u32)btlevtcmd_EllipseMovePosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000014,
    0x0000000F, 0x00000000, 0x0005005B, (u32)btlevtcmd_SetHomePos, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00010009, 0x00000002, 0x0003005B, (u32)btlevtcmd_GetGuardKouraId,
    0xFFFFFFFD, 0xFE363C8A, 0x00020019, 0xFE363C8A, 0xFFFFFFFF, 0x0004005B,
    (u32)btlevtcmd_SetUnitWork, 0xFE363C8A, 0x00000002, 0x00000003, 0x00010003, 0x0000000A,
    0x0004005B, (u32)btlevtcmd_GetUnitWork, 0xFE363C8A, 0x00000002, 0xFE363C80, 0x00020018,
    0xFE363C80, 0x00000003, 0x00010009, 0x00000001, 0x00010004, 0x0000000A,
    0x00000021, 0x0003005B, (u32)btlevtcmd_GetHp, 0xFE363C8A, 0xFE363C80, 0x0002001B,
    0xFE363C80, 0x00000000, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFE363C8A, 0x0000003B,
    0x0005005B, (u32)btlevtcmd_GetPos, 0xFE363C8A, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0005005B, (u32)btlevtcmd_SetHomePos, 0xFE363C8A, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00000021, 0x00000021, 0x0001005B, (u32)btlevtcmd_SortPhaseMoveTable, 0x0002005B, (u32)btlevtcmd_StartWaitEvent,
    0xFFFFFFFD, 0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFC, 0x00000002, 0x00000001,
};

u32 subsetevt_happy_heart[] = {
    0x0003005B, (u32)btlevtcmd_GetBodyId, 0xFFFFFFFE, 0xFE363C85, 0x0005005B, (u32)btlevtcmd_CheckDefinePosetype,
    0xFFFFFFFE, 0xFE363C85, 0x0000003E, 0xFE363C80, 0x00020019, 0xFE363C80,
    0x00000000, 0x0004005B, (u32)btlevtcmd_CheckStatus, 0xFE363C83, 0x00000009, 0xFE363C80,
    0x00020018, 0xFE363C80, 0x00000000, 0x0004005B, (u32)btlevtcmd_CheckStatus, 0xFE363C83,
    0x00000001, 0xFE363C80, 0x00020018, 0xFE363C80, 0x00000000, 0x0004005B,
    (u32)btlevtcmd_CheckStatus, 0xFE363C83, 0x00000002, 0xFE363C80, 0x00020018, 0xFE363C80,
    0x00000000, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0xFE363C85, 0x0000003E,
    0x00000021, 0x00000021, 0x00000021, 0x00000021, 0x00010009, 0x0000003C,
    0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002, 0x00000001,
};

u32 subsetevt_happy_flower[] = {
    0x0003005B, (u32)btlevtcmd_GetBodyId, 0xFFFFFFFE, 0xFE363C85, 0x0005005B, (u32)btlevtcmd_CheckDefinePosetype,
    0xFFFFFFFE, 0xFE363C85, 0x0000003F, 0xFE363C80, 0x00020019, 0xFE363C80,
    0x00000000, 0x0004005B, (u32)btlevtcmd_CheckStatus, 0xFE363C83, 0x00000009, 0xFE363C80,
    0x00020018, 0xFE363C80, 0x00000000, 0x0004005B, (u32)btlevtcmd_CheckStatus, 0xFE363C83,
    0x00000001, 0xFE363C80, 0x00020018, 0xFE363C80, 0x00000000, 0x0004005B,
    (u32)btlevtcmd_CheckStatus, 0xFE363C83, 0x00000002, 0xFE363C80, 0x00020018, 0xFE363C80,
    0x00000000, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0xFE363C85, 0x0000003F,
    0x00000021, 0x00000021, 0x00000021, 0x00000021, 0x00010009, 0x0000003C,
    0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002, 0x00000001,
};

u32 subsetevt_terror[] = {
    0x0005005B, (u32)btlevtcmd_JumpSetting, 0xFFFFFFFE, 0x00000014, 0xF24A7A80, 0xF24A7D4C,
    0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00000010, 0xFFFFFFFF, 0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE, 0xF24AA280,
    0x0004005B, (u32)btlevtcmd_GetStageSize, 0xFE363C80, 0xF1194D80, 0xF1194D80, 0x0006005B,
    (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_ENM_ESCAPE1_802eea0c, 0xF1194D80, 0x00000000, 0xF1194D80,
    0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00000000, 0xFFFFFFFF, 0x00000000, 0x0003005B, (u32)btlevtcmd_KillUnit, 0xFFFFFFFE,
    0x00000000, 0x00000002, 0x00000001,
};

u32 subsetevt_blow_damage[] = {
    0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_BTL_CLAUD_BREATH_802eea1c, 0x00000000, 0x0003005B, (u32)btlevtcmd_GetBodyId,
    0xFFFFFFFE, 0xFE363C80, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0xFE363C80,
    0x00000027, 0x0000006E, 0x00010003, 0x00000000, 0x0005005B, (u32)btlevtcmd_AddRotate,
    0xFFFFFFFE, 0x00000000, 0x00000014, 0x00000000, 0x00010009, 0x00000001,
    0x00010004, 0x00000000, 0x00000070, 0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE,
    0xF24A8280, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0004005B, (u32)btlevtcmd_GetStageSize, 0xFE363C80, 0xF1194D80, 0xF1194D80,
    0x00020035, 0xFE363C81, 0x00000028, 0x000A005B, (u32)btlevtcmd_DivePosition, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000, 0xFFFFFFF6, 0x00000002,
    0x00000000, 0xFFFFFFFF, 0x0004005B, (u32)btlevtcmd_TransStageFloorPosition, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0005005B, (u32)btlevtcmd_SetPos, 0xFE363C8A, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000002, 0x00000001,
};

u32 subsetevt_blow_dead[] = {
    0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_BTL_CLAUD_BREATH_802eea1c, 0x00000000, 0x0003005B, (u32)btlevtcmd_GetBodyId,
    0xFFFFFFFE, 0xFE363C80, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0xFE363C80,
    0x00000027, 0x0000006E, 0x00010003, 0x00000000, 0x0005005B, (u32)btlevtcmd_AddRotate,
    0xFFFFFFFE, 0x00000000, 0x00000014, 0x00000000, 0x00010009, 0x00000001,
    0x00010004, 0x00000000, 0x00000070, 0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE,
    0xF24A8280, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0004005B, (u32)btlevtcmd_GetStageSize, 0xFE363C80, 0xF1194D80, 0xF1194D80,
    0x00020035, 0xFE363C81, 0x00000028, 0x000A005B, (u32)btlevtcmd_DivePosition, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000, 0xFFFFFFF6, 0x00000002,
    0x00000000, 0xFFFFFFFF, 0x0003005B, (u32)btlevtcmd_GetCoin, 0xFE363C8A, 0xFE363C80,
    0x0002005B, (u32)btlevtcmd_StoreCoin, 0xFE363C80, 0x0003005B, (u32)btlevtcmd_GetExp, 0xFE363C8A,
    0xFE363C80, 0x0002005B, (u32)btlevtcmd_StoreExp, 0xFE363C80, 0x0003005B, (u32)btlevtcmd_KillUnit,
    0xFFFFFFFE, 0x00000000, 0x00000002, 0x00000001,
};

u32 subsetevt_shot_damage[] = {
    0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFE363C8A, 0xFE363C8B, 0xFE363C8E, 0x0003005B,
    (u32)btlevtcmd_OnToken, 0xFE363C8A, 0x00000020, 0x0004005B, (u32)btlevtcmd_CheckDamagePattern, 0xFE363C8A,
    0x0000000E, 0xFE363C80, 0x00020019, 0xFE363C80, 0x00000000, 0x00020032,
    0xFE363C8C, (u32)marioWeapon_KaitenHammerInvolved, 0x0001005E, (u32)subsetevt_shot_damage_kaiten, 0x00000020, 0x00020032,
    0xFE363C8C, (u32)marioWeapon_UltraHammerInvolved, 0x0001005E, (u32)subsetevt_shot_damage_ultra, 0x00000021, 0x00000002,
    0x00000001,
};

u32 subsetevt_shot_damage_kaiten[] = {
    0x0001006F, 0xFE363C8D, 0x00010003, 0x00000000, 0x0005005B, (u32)btlevtcmd_AddRotate,
    0xFE363C8A, 0x00000000, 0x0000001E, 0x00000000, 0x00010009, 0x00000001,
    0x00010004, 0x00000000, 0x00000070, 0x0003005B, (u32)btlevtcmd_GetFriendBelong, 0xFE363C8A,
    0xFE363C80, 0x0004005B, (u32)btlevtcmd_SamplingEnemy, 0xFE363C8A, 0xFE363C80, 0xFE363C8C,
    0x0003005B, (u32)btlevtcmd_GetSelectEnemy, 0xFE363C83, 0xFE363C84, 0x00010003, 0x00000001,
    0x00020018, 0xFE363C83, 0xFFFFFFFF, 0x00010004, 0x00000032, 0x00000021,
    0x0003005B, (u32)btlevtcmd_GetUnitId, 0xFE363C8A, 0xFE363C80, 0x00020019, 0xFE363C80,
    0xFE363C83, 0x0003005B, (u32)btlevtcmd_GetSelectNextEnemy, 0xFE363C83, 0xFE363C84, 0x00010004,
    0x00000001, 0x00000021, 0x00010003, 0x0000000A, 0x0003005B, (u32)btlevtcmd_GetSelectNextEnemy,
    0xFE363C83, 0xFE363C84, 0x00020018, 0xFE363C83, 0xFFFFFFFF, 0x00010004,
    0x00000032, 0x00000021, 0x0005005B, (u32)btlevtcmd_JumpSetting, 0xFE363C8A, 0x00000000,
    0xF24A8E80, 0xF24A7A8A, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFE363C8A, 0xFE363C85,
    0xFE363C86, 0xFE363C87, 0x0006005B, (u32)btlevtcmd_GetHitPos, 0xFE363C83, 0xFE363C84,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0002001D, 0xFE363C85, 0xFE363C80,
    0x00020032, 0xFE363C88, 0xFE363C85, 0x00020036, 0xFE363C88, 0xFE363C80,
    0x00000020, 0x00020032, 0xFE363C88, 0xFE363C80, 0x00020036, 0xFE363C88,
    0xFE363C85, 0x00000021, 0x00020038, 0xFE363C88, 0x00000002, 0x00020035,
    0xFE363C88, 0xFE363C86, 0x0002001A, 0xFE363C88, 0xFE363C81, 0x00020032,
    0xFE363C81, 0xFE363C88, 0x00000021, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFE363C8A,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000, 0xFFFFFFFF, 0x0007005B,
    (u32)btlevtcmd_PreCheckDamage, 0xFE363C8A, 0xFE363C83, 0xFE363C84, 0xFE363C8C, 0x00000100,
    0xFE363C85, 0x00010022, 0xFE363C85, 0x00010024, 0x00000002, 0x0003005B,
    (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000028, 0x00010024, 0x00000003, 0x0003005B,
    (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000026, 0x00010024, 0x00000006, 0x0003005B,
    (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000027, 0x00010024, 0x00000004, 0x00010024,
    0x00000001, 0x0007005B, (u32)btlevtcmd_CheckDamage, 0xFE363C8A, 0xFE363C83, 0xFE363C84,
    0xFE363C8C, 0x00000100, 0xFE363C85, 0x0000002A, 0x00000031, 0x00010004,
    0x0000000A, 0x00010003, 0x00000032, 0x0004005B, (u32)btlevtcmd_GetStageSize, 0xFE363C85,
    0xF1194D80, 0xF1194D80, 0x00020032, 0xFE363C80, 0x00000000, 0x0004005B,
    (u32)btlevtcmd_FaceDirectionSub, 0xFE363C8A, 0xFE363C80, 0xFE363C85, 0x0005005B, (u32)btlevtcmd_GetPos,
    0xFE363C8A, 0xF1194D80, 0xFE363C81, 0xF1194D80, 0x0005005B, (u32)btlevtcmd_JumpSetting,
    0xFE363C8A, 0x00000000, 0xF24A8E80, 0xF24A7A8A, 0x0007005B, (u32)btlevtcmd_JumpPosition,
    0xFE363C8A, 0xFE363C80, 0x00000028, 0xFE363C82, 0x00000000, 0xFFFFFFFF,
    0x00010009, 0x0000001E, 0x0004005B, (u32)btlevtcmd_CheckDamageCode, 0xFE363C8A, 0x00000200,
    0xFE363C86, 0x00020019, 0xFE363C86, 0x00000000, 0x00020037, 0xFE363C80,
    0xFFFFFFFF, 0x0005005B, (u32)btlevtcmd_SetPos, 0xFE363C8A, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFE363C8A, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0004005B, (u32)btlevtcmd_FaceDirectionAdd, 0xFE363C8A, 0xFE363C80, 0x00000028,
    0x0005005B, (u32)btlevtcmd_JumpSetting, 0xFE363C8A, 0x00000000, 0xF24A9680, 0xF24A7AE6,
    0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFE363C8A, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00000000, 0xFFFFFFFF, 0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFE363C8A, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFE363C8A, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000010, 0xFFFFFFFF, 0x00000000, 0x00010009,
    0x0000001E, 0x00000020, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFE363C8A, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0004005B, (u32)btlevtcmd_TransStageFloorPosition, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0005005B, (u32)btlevtcmd_SetPos, 0xFE363C8A, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000021, 0x0001005F, 0xFE363C8D, 0x0005005B, (u32)btlevtcmd_SetRotate,
    0xFE363C8A, 0x00000000, 0x00000000, 0x00000000, 0x00000002, 0x00000001,
};

u32 subsetevt_shot_damage_ultra[] = {
    0x0001006F, 0xFE363C8D, 0x0002005B, (u32)btlevtcmd_SetRotateOffsetFromCenterOffset, 0xFE363C8A, 0x0003005B,
    (u32)btlevtcmd_GetFaceDirection, 0xFE363C8A, 0xFE363C80, 0x00020037, 0xFE363C80, 0x00000014,
    0x00020032, 0xFE363C85, 0x00000005, 0x00010005, 0xFE363C85, 0x0005005B,
    (u32)btlevtcmd_AddRotate, 0xFE363C8A, 0x00000000, 0xFE363C80, 0x00000000, 0x00020032,
    0xFE363C85, 0x00000005, 0x00010009, 0x00000001, 0x00000006, 0x00000070,
    0x0003005B, (u32)btlevtcmd_GetFriendBelong, 0xFE363C8A, 0xFE363C80, 0x0004005B, (u32)btlevtcmd_SamplingEnemy,
    0xFE363C8A, 0xFE363C80, 0xFE363C8C, 0x0003005B, (u32)btlevtcmd_GetSelectEnemy, 0xFE363C83,
    0xFE363C84, 0x00010003, 0x00000001, 0x00020018, 0xFE363C83, 0xFFFFFFFF,
    0x00010004, 0x00000032, 0x00000021, 0x0003005B, (u32)btlevtcmd_GetUnitId, 0xFE363C8A,
    0xFE363C80, 0x00020019, 0xFE363C80, 0xFE363C83, 0x0003005B, (u32)btlevtcmd_GetSelectNextEnemy,
    0xFE363C83, 0xFE363C84, 0x00010004, 0x00000001, 0x00000021, 0x00020032,
    0xFE363C88, 0xFFFFFFFF, 0x00020032, 0xFE363C89, 0xFFFFFFFF, 0x00010003,
    0x00000002, 0x0003005B, (u32)btlevtcmd_GetSelectNextEnemy, 0xFE363C86, 0xFE363C87, 0x00020019,
    0xFE363C86, 0xFFFFFFFF, 0x00020032, 0xFE363C88, 0xFE363C86, 0x00020032,
    0xFE363C89, 0xFE363C87, 0x00010004, 0x00000002, 0x00000021, 0x00020018,
    0xFE363C88, 0xFFFFFFFF, 0x00010004, 0x00000032, 0x00000021, 0x0001006F,
    0xFE363C85, 0x0005005B, (u32)btlevtcmd_JumpSetting, 0xFE363C8A, 0x00000000, 0xF24A8E80,
    0xF24A7C19, 0x0006005B, (u32)btlevtcmd_GetHitPos, 0xFE363C88, 0xFE363C89, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0004005B, (u32)btlevtcmd_FaceDirectionAdd, 0xFE363C88, 0xFE363C80,
    0x00000014, 0x0004005B, (u32)btlevtcmd_TransStageFloorPosition, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFE363C8A, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00000000, 0xFFFFFFFF, 0x00000070, 0x0003005B, (u32)btlevtcmd_GetFriendBelong, 0xFE363C8A,
    0xFE363C80, 0x0004005B, (u32)btlevtcmd_SamplingEnemy, 0xFE363C8A, 0xFE363C80, 0xFE363C8C,
    0x0003005B, (u32)btlevtcmd_GetSelectEnemy, 0xFE363C83, 0xFE363C84, 0x00010003, 0x0000001E,
    0x00020018, 0xFE363C83, 0xFFFFFFFF, 0x0001005F, 0xFE363C85, 0x00010004,
    0x00000032, 0x00000021, 0x0003005B, (u32)btlevtcmd_GetUnitId, 0xFE363C8A, 0xFE363C80,
    0x00020019, 0xFE363C80, 0xFE363C83, 0x0003005B, (u32)btlevtcmd_GetSelectNextEnemy, 0xFE363C83,
    0xFE363C84, 0x00010004, 0x0000001E, 0x00000021, 0x0005005B, (u32)btlevtcmd_GetPos,
    0xFE363C8A, 0xFE363C81, 0xF1194D80, 0xF1194D80, 0x0004005B, (u32)btlevtcmd_FaceDirectionAdd,
    0xFE363C8A, 0xFE363C81, 0x00000014, 0x00010003, 0x0000001F, 0x0003005B,
    (u32)btlevtcmd_GetSelectNextEnemy, 0xFE363C83, 0xFE363C84, 0x00020018, 0xFE363C83, 0xFFFFFFFF,
    0x0001005F, 0xFE363C85, 0x00010004, 0x00000032, 0x00000021, 0x00010003,
    0x00000020, 0x0006005B, (u32)btlevtcmd_GetHitPos, 0xFE363C83, 0xFE363C84, 0xFE363C82,
    0xF1194D80, 0xF1194D80, 0x0004005B, (u32)btlevtcmd_FaceDirectionAdd, 0xFE363C83, 0xFE363C82,
    0x00000014, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFE363C8A, 0xFE363C80, 0xF1194D80,
    0xF1194D80, 0x0006005B, (u32)_check_through_pos, 0xFE363C8A, 0xFE363C82, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00020018, 0xFE363C82, 0x00000000, 0x0002006A,
    0xFE363C85, 0xFE363C86, 0x00020018, 0xFE363C86, 0x00000001, 0x00020032,
    0xFE363C81, 0xFE363C80, 0x00010009, 0x00000001, 0x00010004, 0x00000020,
    0x00000021, 0x00000021, 0x0007005B, (u32)btlevtcmd_PreCheckDamage, 0xFE363C8A, 0xFE363C83,
    0xFE363C84, 0xFE363C8C, 0x00000100, 0xFE363C80, 0x00010022, 0xFE363C80,
    0x00010024, 0x00000002, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000028,
    0x00010024, 0x00000003, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000026,
    0x00010024, 0x00000006, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000027,
    0x00010024, 0x00000004, 0x00010024, 0x00000001, 0x0007005B, (u32)btlevtcmd_CheckDamage,
    0xFE363C8A, 0xFE363C83, 0xFE363C84, 0xFE363C8C, 0x00000100, 0xFE363C80,
    0x0000002A, 0x00000031, 0x00010004, 0x0000001F, 0x0001005F, 0xFE363C85,
    0x00010003, 0x00000032, 0x0004005B, (u32)btlevtcmd_GetStageSize, 0xFE363C85, 0xF1194D80,
    0xF1194D80, 0x00020032, 0xFE363C80, 0x00000000, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub,
    0xFE363C8A, 0xFE363C80, 0xFE363C85, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFE363C8A,
    0xF1194D80, 0xFE363C81, 0xF1194D80, 0x0005005B, (u32)btlevtcmd_JumpSetting, 0xFE363C8A,
    0x00000000, 0xF24A8E80, 0xF24A7AE6, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFE363C8A,
    0xFE363C80, 0x00000050, 0xFE363C82, 0x00000000, 0xFFFFFFFF, 0x00010009,
    0x0000001E, 0x0004005B, (u32)btlevtcmd_CheckDamagePattern, 0xFE363C8A, 0x00000010, 0xFE363C85,
    0x00020019, 0xFE363C85, 0x00000000, 0x0003005B, (u32)btlevtcmd_OnToken, 0xFE363C8A,
    0x00000040, 0x00010003, 0x00000037, 0x0004005B, (u32)btlevtcmd_CheckToken, 0xFE363C8A,
    0x00000040, 0xFE363C85, 0x00020019, 0xFE363C85, 0x00000000, 0x00010009,
    0x00000001, 0x00010004, 0x00000037, 0x00000021, 0x00000021, 0x00020037,
    0xFE363C80, 0xFFFFFFFF, 0x0005005B, (u32)btlevtcmd_SetPos, 0xFE363C8A, 0xFE363C80,
    0x00000050, 0xFE363C82, 0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFE363C8A, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0005005B, (u32)btlevtcmd_JumpSetting, 0xFE363C8A, 0x00000000,
    0xF24A9680, 0xF24A7AE6, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFE363C8A, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000000, 0xFFFFFFFF, 0x0004005B, (u32)btlevtcmd_CheckDamagePattern,
    0xFE363C8A, 0x00000010, 0xFE363C85, 0x00020019, 0xFE363C85, 0x00000000,
    0x0003005B, (u32)btlevtcmd_OnToken, 0xFE363C8A, 0x00000080, 0x0001005B, (u32)btlevtcmd_WaitAttackEnd,
    0x00000021, 0x00000021, 0x0005005B, (u32)btlevtcmd_JumpSetting, 0xFE363C8A, 0x00000000,
    0xF24A7A80, 0xF24A7B19, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFE363C8A, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000014, 0xFFFFFFFF, 0x00010009, 0x0000001E,
    0x0004005B, (u32)btlevtcmd_CheckDamageCode, 0xFE363C8A, 0x00000100, 0xFE363C80, 0x00020018,
    0xFE363C80, 0x00000000, 0x0001005B, (u32)btlevtcmd_WaitAttackEnd, 0x00000021, 0x0001005F,
    0xFE363C8D, 0x0005005B, (u32)btlevtcmd_SetRotate, 0xFE363C8A, 0x00000000, 0x00000000,
    0x00000000, 0x00000002, 0x00000001,
};

u32 subsetevt_swallow_failed_damage[] = {
    0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFE363C8A, 0xFE363C8B, 0x00000027, 0x0005005B,
    (u32)btlevtcmd_GetPos, 0xFE363C8A, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0003005B,
    (u32)btlevtcmd_SetFallAccel, 0xFE363C8A, 0xF24A7C80, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFE363C8A,
    0xFE363C80, 0x0000001E, 0x0004005B, (u32)btlevtcmd_TransStageFloorPosition, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFE363C8A, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000014, 0xFFFFFFFF, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFE363C8A,
    0xFE363C80, 0x00000012, 0x0004005B, (u32)btlevtcmd_TransStageFloorPosition, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFE363C8A, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0000000C, 0xFFFFFFFF, 0x00010009, 0x00000014, 0x00000002,
    0x00000001,
};

u32 subsetevt_swallow_spew_damage[] = {
    0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFE363C8A, 0xFE363C8B, 0x00000027, 0x0005005B,
    (u32)btlevtcmd_GetPos, 0xFE363C8A, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0003005B,
    (u32)btlevtcmd_SetFallAccel, 0xFE363C8A, 0xF24A7C80, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFE363C8A,
    0xFE363C80, 0x0000001E, 0x0004005B, (u32)btlevtcmd_TransStageFloorPosition, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFE363C8A, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000014, 0xFFFFFFFF, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFE363C8A,
    0xFE363C80, 0x00000012, 0x0004005B, (u32)btlevtcmd_TransStageFloorPosition, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFE363C8A, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0000000C, 0xFFFFFFFF, 0x00010009, 0x00000014, 0x00000002,
    0x00000001,
};

u32 subsetevt_swallow_shot_damage[] = {
    0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFE363C8A, 0xFE363C8B, 0xFE363C8E, 0x00020032,
    0xFE363C8C, (u32)partyWeapon_YoshiNomikomi_Involved, 0x0003005B, (u32)btlevtcmd_GetFriendBelong, 0xFE363C8A, 0xFE363C80,
    0x0004005B, (u32)btlevtcmd_SamplingEnemy, 0xFE363C8A, 0xFE363C80, 0xFE363C8C, 0x0003005B,
    (u32)btlevtcmd_GetSelectEnemy, 0xFE363C83, 0xFE363C84, 0x00010003, 0x00000000, 0x00020018,
    0xFE363C83, 0xFFFFFFFF, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFE363C8A, 0xFE363C8B,
    0x00000027, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFE363C8A, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0003005B, (u32)btlevtcmd_SetFallAccel, 0xFE363C8A, 0xF24A7C80, 0x0004005B,
    (u32)btlevtcmd_FaceDirectionSub, 0xFE363C8A, 0xFE363C80, 0x0000001E, 0x0004005B, (u32)btlevtcmd_TransStageFloorPosition,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFE363C8A,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000014, 0xFFFFFFFF, 0x0004005B,
    (u32)btlevtcmd_FaceDirectionSub, 0xFE363C8A, 0xFE363C80, 0x00000012, 0x0004005B, (u32)btlevtcmd_TransStageFloorPosition,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFE363C8A,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0000000C, 0xFFFFFFFF, 0x00010009,
    0x00000014, 0x00010004, 0x00000063, 0x00000021, 0x0003005B, (u32)btlevtcmd_GetUnitId,
    0xFE363C8A, 0xFE363C80, 0x00020019, 0xFE363C80, 0xFE363C83, 0x0003005B,
    (u32)btlevtcmd_GetSelectNextEnemy, 0xFE363C83, 0xFE363C84, 0x00010004, 0x00000000, 0x00000021,
    0x0003005B, (u32)btlevtcmd_GetSelectNextEnemy, 0xFE363C83, 0xFE363C84, 0x00010073, 0xFE363C83,
    0x00020018, 0xFE363C83, 0xFFFFFFFF, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFE363C8A,
    0xFE363C8B, 0x00000027, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFE363C8A, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0003005B, (u32)btlevtcmd_SetFallAccel, 0xFE363C8A, 0xF24A7C80,
    0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFE363C8A, 0xFE363C80, 0x0000001E, 0x0004005B,
    (u32)btlevtcmd_TransStageFloorPosition, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0007005B, (u32)btlevtcmd_JumpPosition,
    0xFE363C8A, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000014, 0xFFFFFFFF,
    0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFE363C8A, 0xFE363C80, 0x00000012, 0x0004005B,
    (u32)btlevtcmd_TransStageFloorPosition, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0007005B, (u32)btlevtcmd_JumpPosition,
    0xFE363C8A, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0000000C, 0xFFFFFFFF,
    0x00010009, 0x00000014, 0x00010004, 0x00000063, 0x00000021, 0x0006005B,
    (u32)btlevtcmd_GetHitPos, 0xFE363C83, 0xFE363C84, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0005005B, (u32)btlevtcmd_JumpSetting, 0xFE363C8A, 0x00000000, 0xF24A8E80, 0xF24A7AE6,
    0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFE363C8A, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00000000, 0xFFFFFFFF, 0x0007005B, (u32)btlevtcmd_PreCheckDamage, 0xFFFFFFFC, 0xFE363C83,
    0xFE363C84, 0xFE363C8C, 0x00000100, 0xFE363C85, 0x00010022, 0xFE363C85,
    0x00010024, 0x00000002, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000028,
    0x00010024, 0x00000003, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000026,
    0x00010024, 0x00000006, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000027,
    0x00010024, 0x00000004, 0x00010024, 0x00000001, 0x0007005B, (u32)btlevtcmd_CheckDamage,
    0xFFFFFFFC, 0xFE363C83, 0xFE363C84, 0xFE363C8C, 0x80000100, 0xFE363C85,
    0x00010004, 0x00000032, 0x0000002A, 0x00000031, 0x00010003, 0x00000014,
    0x0001006F, 0xFE363C85, 0x0002005B, (u32)btlevtcmd_JumpContinue, 0xFE363C8A, 0x00000070,
    0x00010003, 0x00000015, 0x0002006A, 0xFE363C85, 0xFE363C80, 0x00020018,
    0xFE363C80, 0x00000000, 0x00010004, 0x00000019, 0x00000021, 0x0005005B,
    (u32)btlevtcmd_GetPos, 0xFE363C8A, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00020032,
    0xFE363C86, 0xFE363C80, 0x0002001A, 0xFE363C86, 0x00000000, 0x00020037,
    0xFE363C86, 0xFFFFFFFF, 0x00000021, 0x0002001D, 0xFE363C86, 0x00000190,
    0x0001005F, 0xFE363C85, 0x0004005B, (u32)btlevtcmd_TransStageFloorPosition, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0005005B, (u32)btlevtcmd_SetPos, 0xFE363C8A, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00010004, 0x00000063, 0x00000021, 0x00010009, 0x00000001,
    0x00010004, 0x00000015, 0x00010003, 0x00000019, 0x00010004, 0x00000063,
    0x00010003, 0x00000032, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFE363C8A, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0003005B, (u32)btlevtcmd_SetFallAccel, 0xFE363C8A, 0xF24A7C80,
    0x0004005B, (u32)btlevtcmd_FaceDirectionAdd, 0xFE363C8A, 0xFE363C80, 0x0000001E, 0x0004005B,
    (u32)btlevtcmd_TransStageFloorPosition, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0007005B, (u32)btlevtcmd_JumpPosition,
    0xFE363C8A, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000014, 0xFFFFFFFF,
    0x0004005B, (u32)btlevtcmd_FaceDirectionAdd, 0xFE363C8A, 0xFE363C80, 0x00000012, 0x0004005B,
    (u32)btlevtcmd_TransStageFloorPosition, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0007005B, (u32)btlevtcmd_JumpPosition,
    0xFE363C8A, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0000000C, 0xFFFFFFFF,
    0x00010009, 0x00000014, 0x00010003, 0x00000063, 0x00000002, 0x00000001,
};

u32 subset_ultra_hammer_acrobat_damage_event[] = {
    0x0005005B, (u32)btlevtcmd_SetRotate, 0xFE363C8A, 0x00000000, 0x00000000, 0x00000000,
    0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFE363C8A, 0xFE363C8B, 0x00000027, 0x0005005B,
    (u32)btlevtcmd_GetPos, 0xFE363C8A, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B,
    (u32)btlevtcmd_FaceDirectionSub, 0xFE363C8A, 0xFE363C80, 0x00000028, 0x0004005B, (u32)btlevtcmd_TransStageFloorPosition,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0003005B, (u32)btlevtcmd_SetFallAccel, 0xFE363C8A,
    0xF24A7C80, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFE363C8A, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000018, 0xFFFFFFFF, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFE363C8A,
    0xFE363C80, 0x00000012, 0x0004005B, (u32)btlevtcmd_TransStageFloorPosition, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFE363C8A, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0000000C, 0xFFFFFFFF, 0x0003005B, (u32)btlevtcmd_OnToken, 0xFE363C8A,
    0x00000080, 0x00000002, 0x00000001,
};

u32 subsetevt_confuse_flustered[] = {
    0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000007, 0x0004005B, (u32)evt_btl_camera_set_homing_unit,
    0x00000000, 0xFFFFFFFE, 0xFFFFFFFF, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000,
    0x00000002, 0x0003005B, (u32)evt_btl_camera_set_zoom, 0x00000000, 0x000000C8, 0x0003005B,
    (u32)btlevtcmd_GetBodyId, 0xFFFFFFFE, 0xFE363C80, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE,
    0xFE363C80, 0x00000005, 0x0001000A, 0x000005DC, 0x0003005B, (u32)evt_btl_camera_set_mode,
    0x00000000, 0x00000000, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000002,
    0x0001000A, 0x000003E8, 0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002,
    0x00000001,
};

u32 subsetevt_binta_damage_left[] = {
    0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFE363C8A, 0xFE363C8B, 0xFE363C8E, 0x0005005B,
    (u32)btlevtcmd_SetRotate, 0xFE363C8A, 0x00000000, 0x00000000, 0x00000000, 0x0003005B,
    (u32)btlevtcmd_GetFaceDirection, 0xFE363C8A, 0xFE363C80, 0x0002001D, 0xFE363C80, 0x00000000,
    0x00020032, 0xFE363C80, 0xFFFFFFEE, 0x00000020, 0x00020032, 0xFE363C80,
    0x00000012, 0x00000021, 0x00010005, 0x00000014, 0x0005005B, (u32)btlevtcmd_AddRotate,
    0xFE363C8A, 0x00000000, 0xFE363C80, 0x00000000, 0x00010009, 0x00000001,
    0x00000006, 0x0005005B, (u32)btlevtcmd_SetRotate, 0xFE363C8A, 0x00000000, 0x00000000,
    0x00000000, 0x00010009, 0x0000000A, 0x00000002, 0x00000002, 0x00000001,
};

u32 subsetevt_binta_damage_right[] = {
    0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFE363C8A, 0xFE363C8B, 0xFE363C8E, 0x0005005B,
    (u32)btlevtcmd_SetRotate, 0xFE363C8A, 0x00000000, 0x00000000, 0x00000000, 0x0003005B,
    (u32)btlevtcmd_GetFaceDirection, 0xFE363C8A, 0xFE363C80, 0x0002001D, 0xFE363C80, 0x00000000,
    0x00020032, 0xFE363C80, 0x00000012, 0x00000020, 0x00020032, 0xFE363C80,
    0xFFFFFFEE, 0x00000021, 0x00010005, 0x00000014, 0x0005005B, (u32)btlevtcmd_AddRotate,
    0xFE363C8A, 0x00000000, 0xFE363C80, 0x00000000, 0x00010009, 0x00000001,
    0x00000006, 0x0005005B, (u32)btlevtcmd_SetRotate, 0xFE363C8A, 0x00000000, 0x00000000,
    0x00000000, 0x00010009, 0x0000000A, 0x00000002, 0x00000002, 0x00000001,
};

u32 subsetevt_binta_damage_left_last[] = {
    0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFE363C8A, 0xFE363C8B, 0xFE363C8E, 0x0005005B,
    (u32)btlevtcmd_SetRotate, 0xFE363C8A, 0x00000000, 0x00000000, 0x00000000, 0x0001006F,
    0xFE363C80, 0x0003005B, (u32)btlevtcmd_GetFaceDirection, 0xFE363C8A, 0xFE363C80, 0x0002001D,
    0xFE363C80, 0x00000000, 0x00020032, 0xFE363C88, 0xFFFFF790, 0x00000020,
    0x00020032, 0xFE363C88, 0x00000870, 0x00000021, 0x0003005B, (u32)_binta_effect,
    0xFE363C8A, 0xFE363C88, 0x00020032, 0xFE363C87, 0xFE363C8A, 0x0005005B,
    (u32)evt_sub_intpl_init, 0x0000000B, 0x00000000, 0xFE363C88, 0x0000005A, 0x00010005,
    0x0000005A, 0x0001005B, (u32)evt_sub_intpl_get_value, 0x0005005B, (u32)btlevtcmd_SetRotate, 0xFE363C87,
    0x00000000, 0xFE363C80, 0x00000000, 0x00010009, 0x00000001, 0x00000006,
    0x0005005B, (u32)btlevtcmd_SetRotate, 0xFE363C87, 0x00000000, 0x00000000, 0x00000000,
    0x00000070, 0x0002005B, (u32)btlevtcmd_WaitEventEnd, 0xFE363C80, 0x00010009, 0x0000001E,
    0x00000002, 0x00000002, 0x00000001,
};

u32 subsetevt_binta_damage_right_last[] = {
    0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFE363C8A, 0xFE363C8B, 0xFE363C8E, 0x0005005B,
    (u32)btlevtcmd_SetRotate, 0xFE363C8A, 0x00000000, 0x00000000, 0x00000000, 0x0001006F,
    0xFE363C80, 0x0003005B, (u32)btlevtcmd_GetFaceDirection, 0xFE363C8A, 0xFE363C80, 0x0002001D,
    0xFE363C80, 0x00000000, 0x00020032, 0xFE363C88, 0x00000870, 0x00000020,
    0x00020032, 0xFE363C88, 0xFFFFF790, 0x00000021, 0x0003005B, (u32)_binta_effect,
    0xFE363C8A, 0xFE363C88, 0x00020032, 0xFE363C87, 0xFE363C8A, 0x0005005B,
    (u32)evt_sub_intpl_init, 0x0000000B, 0x00000000, 0xFE363C88, 0x0000005A, 0x00010005,
    0x0000005A, 0x0001005B, (u32)evt_sub_intpl_get_value, 0x0005005B, (u32)btlevtcmd_SetRotate, 0xFE363C87,
    0x00000000, 0xFE363C80, 0x00000000, 0x00010009, 0x00000001, 0x00000006,
    0x0005005B, (u32)btlevtcmd_SetRotate, 0xFE363C87, 0x00000000, 0x00000000, 0x00000000,
    0x00000070, 0x0002005B, (u32)btlevtcmd_WaitEventEnd, 0xFE363C80, 0x00010009, 0x0000001E,
    0x00000002, 0x00000002, 0x00000001,
};

u32 subsetevt_status_recovery[] = {
    0x0004005B, (u32)btlevtcmd_CheckDataOfDataTable, 0xFE363C8A, 0xFE363C80, 0x00000008, 0x00020019,
    0xFE363C80, 0x00000000, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFE363C8A, 0x00000008,
    0x00000002, 0x00000021, 0x0002005B, (u32)btlevtcmd_StopWaitEvent, 0xFFFFFFFE, 0x0003005B,
    (u32)btlevtcmd_GetBodyId, 0xFFFFFFFE, 0xFE363C80, 0x0003005B, (u32)btlevtcmd_AnimeChangePoseFromTable, 0xFFFFFFFE,
    0xFE363C80, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0003005B, (u32)btlevtcmd_SetFallAccel, 0xFFFFFFFE, 0xF24A7C19, 0x0007005B,
    (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0000001E,
    0xFFFFFFFF, 0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002, 0x00000001,
};

u32 unk_evt_803537c4[] = {
    0x0004005B, (u32)btlevtcmd_CheckStatus, 0xFE363C83, 0x0000001B, 0xFE363C80, 0x00020019,
    0xFE363C80, 0x00000000, 0x00020032, 0xFE363C80, 0x00000000, 0x00000002,
    0x00000021, 0x0004005B, (u32)btlevtcmd_CheckStatus, 0xFE363C83, 0x00000009, 0xFE363C80,
    0x00020019, 0xFE363C80, 0x00000000, 0x00020032, 0xFE363C80, 0x00000000,
    0x00000002, 0x00000021, 0x0004005B, (u32)btlevtcmd_CheckStatus, 0xFE363C83, 0x00000002,
    0xFE363C80, 0x00020019, 0xFE363C80, 0x00000000, 0x00020032, 0xFE363C80,
    0x00000000, 0x00000002, 0x00000021, 0x0004005B, (u32)btlevtcmd_CheckStatus, 0xFE363C83,
    0x00000002, 0xFE363C80, 0x00020019, 0xFE363C80, 0x00000000, 0x00020032,
    0xFE363C80, 0x00000000, 0x00000002, 0x00000021, 0x00020032, 0xFE363C80,
    0x00000001, 0x00000002, 0x00000001,
};

/* Reconstructed .sdata event bytecode; exact target words + symbolic relocations. */
u32 subsetevt_nodamage_motion[] = {
    0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_BTL_ATTACK_MISS2_802ee9ac, 0x00000000, 0x00000002, 0x00000001,
};

u32 subsetevt_regeneration[] = {
    0x00010009, 0x0000003C, 0x00000002, 0x00000001,
};

u32 subset_attack_ready_jump_event[] = {
    0x00000002, 0x00000001,
};
