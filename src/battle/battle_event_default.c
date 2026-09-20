#include "battle/battle_event_default.h"
#include "battle/battle_audience.h"
#include "effect/eff_fire.h"
extern void* _battleWorkPointer;

extern s32 BattleTransID(void* evt, s32 id);
extern void* BattleGetUnitPtr(void* battleWork, s32 unitId);
extern void* BattleGetMarioPtr(void* battleWork);
extern void* BattleGetPartyPtr(void* battleWork);

extern void BattleAcHelpSetHelp(const char* msg);
extern void BattleAcHelpSetDispType(s32 type);

extern s32 BattlePartyAnimeLoad(void* partyAnimeWork);

extern void evtSetValue(void* evt, s32 var, s32 value);

extern const char str_msg_ac_escape_802ee90c[];

/* Forward declarations for recovered default battle event bytecode. */
extern u32 _Defence_core[];
extern u32 btldefaultevt_Appeal[];
extern u32 btldefaultevt_BiribiriMove[];
extern u32 btldefaultevt_CantMoveZeroGravity[];
extern u32 btldefaultevt_ChangeParty[];
extern u32 btldefaultevt_Confuse[];
extern u32 btldefaultevt_Damage[];
extern u32 btldefaultevt_Defend[];
extern u32 btldefaultevt_DefendParty[];
extern u32 btldefaultevt_Dummy[];
extern u32 btldefaultevt_Escape[];
extern u32 btldefaultevt_LectureEnd[];
extern u32 btldefaultevt_ProtectPartner[];
extern u32 btldefaultevt_SuitoruBadgeEffect[];
extern u32 btldefaultevt_Wait[];
extern u32 btldefaultevt_avoid_counter_event[];
extern u32 btldefaultevt_guard_move[];
extern u32 btldefaultevt_guard_return[];
extern u32 _event_MajinaiEnd[];
extern u32 badgeWeapon_DefenceCommand[];
extern u32 badgeWeapon_DefenceCommandP[];
extern u32 btldefaultevt_avoid_guard[];
extern u32 subset_ultra_hammer_acrobat_damage_event[];
extern u32 subsetevt_binta_damage_left[];
extern u32 subsetevt_binta_damage_right[];
extern u32 subsetevt_blow_dead[];
extern u32 subsetevt_counter_damage[];
extern u32 subsetevt_damage[];
extern u32 subsetevt_dead[];
extern u32 subsetevt_terror[];
extern s32 btlevtcmd_AcSetDifficulty();
extern s32 btlevtcmd_AcSetFlag();
extern s32 btlevtcmd_AcSetParamAll();
extern s32 btlevtcmd_AddRotate();
extern s32 btlevtcmd_AddScale();
extern s32 btlevtcmd_AnimeChangePaperAnime();
extern s32 btlevtcmd_AnimeChangePaperGroup();
extern s32 btlevtcmd_AnimeChangePoseFromTable();
extern s32 btlevtcmd_AnimeChangePoseType();
extern s32 btlevtcmd_AnimeCmpPoseType();
extern s32 btlevtcmd_AnimeSetMotionSpeed();
extern s32 btlevtcmd_AnnounceMessage();
extern s32 btlevtcmd_AudienceAppeal();
extern s32 btlevtcmd_CalculateFaceDirection();
extern s32 btlevtcmd_ChangeFaceDirection();
extern s32 btlevtcmd_ChangeParty();
extern s32 btlevtcmd_CheckActStatus();
extern s32 btlevtcmd_CheckAttribute();
extern s32 btlevtcmd_CheckCommandUnit();
extern s32 btlevtcmd_CheckDamageCode();
extern s32 btlevtcmd_CheckDamagePattern();
extern s32 btlevtcmd_CheckDataOfDataTable();
extern s32 btlevtcmd_CheckDefinePosetype();
extern s32 btlevtcmd_CheckGuardMotionEnable();
extern s32 btlevtcmd_CheckStatus();
extern s32 btlevtcmd_DivePosition();
extern s32 btlevtcmd_FaceDirectionAdd();
extern s32 btlevtcmd_FaceDirectionSub();
extern s32 btlevtcmd_GetBodyId();
extern s32 btlevtcmd_GetConfuseActEvent();
extern s32 btlevtcmd_GetDamageCode();
extern s32 btlevtcmd_GetHeight();
extern s32 btlevtcmd_GetHomePos();
extern s32 btlevtcmd_GetMarioId();
extern s32 btlevtcmd_GetMoveFrame();
extern s32 btlevtcmd_GetPartnerId();
extern s32 btlevtcmd_GetPartsPos();
extern s32 btlevtcmd_GetPartyId();
extern s32 btlevtcmd_GetPos();
extern s32 btlevtcmd_GetProtectId();
extern s32 btlevtcmd_GetResultAC();
extern s32 btlevtcmd_GetStatusMg();
extern s32 btlevtcmd_GetUnitId();
extern s32 btlevtcmd_GuardMoveEnd();
extern s32 btlevtcmd_InterruptStop();
extern s32 btlevtcmd_JumpPosition();
extern s32 btlevtcmd_JumpSetting();
extern s32 btlevtcmd_KillUnit();
extern s32 btlevtcmd_MovePosition();
extern s32 btlevtcmd_OffUnitFlag();
extern s32 btlevtcmd_OnOffStatus();
extern s32 btlevtcmd_OnStatusFlag();
extern s32 btlevtcmd_OnUnitFlag();
extern s32 btlevtcmd_PayWeaponCost();
extern s32 btlevtcmd_RecoverFp();
extern s32 btlevtcmd_RecoverHp();
extern s32 btlevtcmd_ResetFaceDirection();
extern s32 btlevtcmd_ResultAC();
extern s32 btlevtcmd_RunDataEventChild();
extern s32 btlevtcmd_SetDispOffset();
extern s32 btlevtcmd_SetFallAccel();
extern s32 btlevtcmd_SetGuard();
extern s32 btlevtcmd_SetMoveSpeed();
extern s32 btlevtcmd_SetPos();
extern s32 btlevtcmd_SetRotate();
extern s32 btlevtcmd_SetScale();
extern s32 btlevtcmd_SetupAC();
extern s32 btlevtcmd_SortPhaseMoveTable();
extern s32 btlevtcmd_StartAC();
extern s32 btlevtcmd_StartWaitEvent();
extern s32 btlevtcmd_StopAC();
extern s32 btlevtcmd_WaitAttackEnd();
extern s32 btlevtcmd_WaitAttackEndCheck();
extern s32 btlevtcmd_WaitEventEnd();
extern s32 btlevtcmd_WeaponAftereffect();
extern s32 btlevtcmd_check_battleflag();
extern s32 btlevtcmd_ftof();
extern s32 btlevtcmd_ftomsec();
extern s32 btlevtcmd_snd_se();
extern s32 evt_audience_sound_cheer();
extern s32 evt_batstage_light_set_player_off();
extern s32 evt_batstage_light_set_player_on();
extern s32 evt_btl_camera_set_homing_unit();
extern s32 evt_btl_camera_set_mode();
extern s32 evt_btl_camera_set_moveSpeedLv();
extern s32 evt_btl_camera_set_moveto();
extern s32 evt_btl_camera_set_zoom();
extern s32 evt_eff();
extern s32 evt_eff64();
extern s32 evt_snd_sfxoff();
extern s32 evt_snd_sfxon();
extern s32 evt_sub_intpl_get_value();
extern s32 evt_sub_intpl_get_value_para();
extern s32 evt_sub_intpl_init();
extern s32 evt_sub_random();



s32 _backfire(void* evt, s32 firstCall) {
    extern void BtlUnit_GetPos(void* unit, f32* x, f32* y, f32* z);
    extern void effDelete(void* effect);
    extern void BattleDamageDirect(s32 attacker, void* unit, s32 part, s32 damage, s32 zero, s32 flags, s32 arg7, s32 arg8);
    extern s32 evtGetValue(void* evt, s32 value);
    extern f32 sinfd(f32 value);
    extern f32 float_1_804222fc;
    extern f32 float_0p4_80422300;
    extern f32 float_1p6_80422304;
    s32* args;
    void* battleWork;
    s32 unitId;
    void* unit;
    void* effectWork;
    s32 counter;
    s32 value;
    f32 x;
    f32 y;
    f32 z;

    args = *(s32**)((s32)evt + 0x18);
    battleWork = _battleWorkPointer;

    value = evtGetValue(evt, args[0]);
    unitId = BattleTransID(evt, value);
    unit = BattleGetUnitPtr(battleWork, unitId);
    if (unit == 0) {
        return 2;
    }

    if (firstCall != 0) {
        BtlUnit_GetPos(unit, &x, &y, &z);
        *(void**)((s32)evt + 0x78) = effFireEntry(x, y, z, float_1_804222fc, 0, 0);
        *(s32*)((s32)evt + 0x7C) = 0;
    }

    effectWork = *(void**)((s32)*(void**)((s32)evt + 0x78) + 0xC);
    *(s32*)((s32)evt + 0x7C) = *(s32*)((s32)evt + 0x7C) + 1;
    counter = *(s32*)((s32)evt + 0x7C);

    if (*(s32*)((s32)evt + 0x7C) >= 60) {
        effDelete(*(void**)((s32)evt + 0x78));
        BattleDamageDirect(-5, unit, 0, 1, 0, 0x118, 0, 1);
        if (BattleTransID(evt, *(s32*)((s32)evt + 0x160)) != unitId) {
            return 2;
        }
        return 0xFF;
    }

    *(f32*)((s32)effectWork + 0x24) = float_1p6_80422304 * sinfd((f32)(counter * 3)) + float_0p4_80422300;
    return 0;
}

s32 _anime_load(void) {
    BattlePartyAnimeLoad(*(void**)((s32)_battleWorkPointer + 0x1C68));

    return 2;
}

s32 _anime_load_wait(void) {
    return BattlePartyAnimeLoad(*(void**)((s32)_battleWorkPointer + 0x1C68)) != 0 ? 2 : 0;
}

s32 _check_pose_stay(void* evt) {
    s32* args = *(s32**)((s32)evt + 0x18);
    void* battleWork = _battleWorkPointer;
    s32 dst = args[0];
    s32 unitId = BattleTransID(evt, -2);
    void* unit = BattleGetUnitPtr(battleWork, unitId);
    s32 value = 0;

    if (*(s32*)((s32)unit + 8) == 0xDE) {
        value = 1;
        if ((*(u32*)((s32)battleWork + 0x163F4) & 1) != 0 &&
            (*(u32*)((s32)unit + 0x27C) & 0x10) == 0) {
            value = 0;
        }
    }
    evtSetValue(evt, dst, value);

    return 2;
}

s32 btlevtcmd_audience_code_escape(void) {
    BattleAudience_Case_Escape();

    return 2;
}

s32 btlevtcmd_audience_code_escape_success(void) {
    BattleAudience_Case_Escape_Good();

    return 2;
}

s32 btlevtcmd_audience_code_escape_fail(void) {
    BattleAudience_Case_Escape_Bad();

    return 2;
}

s32 _btl_escape_reset_move_color_lv_player_unit_all(void) {
    void* battleWork;
    void* unit;

    battleWork = _battleWorkPointer;

    unit = BattleGetMarioPtr(battleWork);
    *(u8*)((s32)unit + 0x311) = 0xFF;

    unit = BattleGetPartyPtr(battleWork);
    if (unit != 0) {
        *(u8*)((s32)unit + 0x311) = 0xFF;
    }

    return 2;
}

s32 _set_escape_ac_hlp_msg(void) {
    BattleAcHelpSetHelp(str_msg_ac_escape_802ee90c);
    BattleAcHelpSetDispType(2);

    return 2;
}

s32 _get_escape_rate(void* evt) {
    s32* args;
    void* battleWork;
    void* data;

    battleWork = _battleWorkPointer;
    args = *(s32**)((s32)evt + 0x18);

    data = *(void**)((s32)battleWork + 0x2738);
    data = *(void**)((s32)data + 0xC);
    data = *(void**)((s32)data + 0x18);
    data = *(void**)((s32)data + 0x4);
    data = *(void**)data;

    evtSetValue(evt, args[0], *(u8*)((s32)data + 0x13));

    return 2;
}

s32 _get_heart_suitoru_point(void* evt) {
    s32* args;
    void* battleWork;
    s32 id;
    s32 dst;
    void* unit;
    s32 value;
    s32 max;

    args = *(s32**)((s32)evt + 0x18);
    battleWork = _battleWorkPointer;
    id = args[0];
    dst = args[1];

    unit = BattleGetUnitPtr(battleWork, BattleTransID(evt, id));

    value = 0;
    if (unit != 0) {
        value = *(u8*)((s32)unit + 0x2F9);
        max = *(s32*)((s32)unit + 0x284);
        if (value > max) {
            value = max;
        }
    }

    evtSetValue(evt, dst, value);

    return 2;
}

s32 _get_flower_suitoru_point(void* evt) {
    s32* args;
    void* battleWork;
    s32 id;
    s32 dst;
    void* unit;
    s32 value;
    s32 max;

    args = *(s32**)((s32)evt + 0x18);
    battleWork = _battleWorkPointer;
    id = args[0];
    dst = args[1];

    unit = BattleGetUnitPtr(battleWork, BattleTransID(evt, id));

    value = 0;
    if (unit != 0) {
        value = *(u8*)((s32)unit + 0x2FA);
        max = *(s32*)((s32)unit + 0x284);
        if (value > max) {
            value = max;
        }
    }

    evtSetValue(evt, dst, value);

    return 2;
}

const char str_freeze_n64_802ee788[] = "freeze_n64";
const char str_SFX_SYSTEM_LUCKY1_802ee794[] = "SFX_SYSTEM_LUCKY1";
const char str_SFX_MARIO_AVOID1_802ee7a8[] = "SFX_MARIO_AVOID1";
const char str_SFX_BTL_ATTACK_MISS1_802ee7bc[] = "SFX_BTL_ATTACK_MISS1";
const char str_p_b_st_802ee7d4[] = "p_b_st";
const char str_PM_F_3A_802ee7dc[] = "PM_F_3A";
const char str_PM_F_3B_802ee7e4[] = "PM_F_3B";
const char str_PM_Y_2A_802ee7ec[] = "PM_Y_2A";
const char str_PM_Y_2B_802ee7f4[] = "PM_Y_2B";
const char str_SFX_BTL_PARTY_IN1_802ee7fc[] = "SFX_BTL_PARTY_IN1";
const char str_SFX_BTL_PARTY_CHANGE_802ee810[] = "SFX_BTL_PARTY_CHANGE1";
const char str_SFX_PARTY_OUT1_802ee828[] = "SFX_PARTY_OUT1";
const char str_SFX_MARIO_APPEAL1_802ee838[] = "SFX_MARIO_APPEAL1";
const char str_SFX_BTL_MARIO_DEFENC_802ee84c[] = "SFX_BTL_MARIO_DEFENCE1";
const char str_SFX_BTL_MARIO_DEFENC_802ee864[] = "SFX_BTL_MARIO_DEFENCE2";
const char str_btl_msg_defend_comma_802ee87c[] = "btl_msg_defend_command_exec";
const char str_SFX_MARIO_BATTLE_STA_802ee898[] = "SFX_MARIO_BATTLE_STAMP1";
const char str_SFX_MARIO_RUN_AWAY_M_802ee8b0[] = "SFX_MARIO_RUN_AWAY_MISS1";
const char str_SFX_MARIO_RUN_AWAY1_802ee8cc[] = "SFX_MARIO_RUN_AWAY1";
const char str_btl_msg_cant_move_ze_802ee8e0[] = "btl_msg_cant_move_zero_gravity";
const char str_recovery_802ee900[] = "recovery";
const char str_msg_ac_escape_802ee90c[] = "msg_ac_escape";
const f64 double_to_int_802ee920 = 4503601774854144.0;

const u8 zero_804222f8 = 0;
const f32 float_1_804222fc = 1.0f;
const f32 float_0p4_80422300 = 0.4f;
const f32 float_1p6_80422304 = 1.6f;

/*
 * Recovered default battle event bytecode.
 * Exact US target words are preserved; relocation words remain symbolic.
 */
/* .data */
u32 btldefaultevt_Damage[] = {
    0x0005005B, (u32)btlevtcmd_SetRotate, 0xFE363C8A, 0x00000000, 0x00000000, 0x00000000,
    0x0004005B, (u32)btlevtcmd_CheckDamageCode, 0xFE363C8A, 0x00008000, 0xFE363C81, 0x00020019,
    0xFE363C81, 0x00000000, 0x0006005B, (u32)btlevtcmd_OnOffStatus, 0xFE363C8A, 0x00000009,
    0x00000000, 0x00000000, 0x00000000, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFE363C8A,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000F005B, (u32)evt_eff64, (u32)(u8*)&zero_804222f8,
    (u32)(char*)str_freeze_n64_802ee788, 0x00000000, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0xF24A7E80,
    0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000021, 0x0004005B, (u32)btlevtcmd_CheckDamagePattern, 0xFE363C8A, 0x00000002,
    0xFE363C80, 0x00020019, 0xFE363C80, 0x00000000, 0x00020032, 0xFE363C8E,
    0x00000027, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFE363C8A, 0xFE363C8B, 0xFE363C8E,
    0x0000006E, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFE363C8A, 0x0000000A, 0x00000070,
    0x00010003, 0x00000000, 0x00010009, 0x00000001, 0x00010004, 0x00000000,
    0x00000021, 0x0004005B, (u32)btlevtcmd_CheckDamagePattern, 0xFE363C8A, 0x00000003, 0xFE363C80,
    0x00020019, 0xFE363C80, 0x00000000, 0x00020032, 0xFE363C8E, 0x00000027,
    0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFE363C8A, 0xFE363C8B, 0xFE363C8E, 0x0000006E,
    0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFE363C8A, 0x0000000A, 0x00000070, 0x00010003,
    0x00000001, 0x0005005B, (u32)btlevtcmd_SetDispOffset, 0xFE363C8A, 0x00000002, 0x00000000,
    0x00000000, 0x00010009, 0x00000002, 0x0005005B, (u32)btlevtcmd_SetDispOffset, 0xFE363C8A,
    0x00000000, 0x00000000, 0x00000000, 0x00010009, 0x00000002, 0x0002005B,
    (u32)btlevtcmd_WaitAttackEndCheck, 0xFE363C80, 0x00020018, 0xFE363C80, 0x00000000, 0x00010004,
    0x00000001, 0x00000021, 0x00000021, 0x0004005B, (u32)btlevtcmd_CheckDamagePattern, 0xFE363C8A,
    0x00000014, 0xFE363C80, 0x00020019, 0xFE363C80, 0x00000000, 0x00020032,
    0xFE363C8E, 0x00000005, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFE363C8A, 0xFE363C8B,
    0xFE363C8E, 0x0000006E, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFE363C8A, 0x0000000A,
    0x00000070, 0x0001005B, (u32)btlevtcmd_WaitAttackEnd, 0x00010004, 0x0000000A, 0x00000021,
    0x0004005B, (u32)btlevtcmd_CheckDamagePattern, 0xFE363C8A, 0x00000007, 0xFE363C80, 0x00020019,
    0xFE363C80, 0x00000000, 0x0004005B, (u32)btlevtcmd_CheckDataOfDataTable, 0xFE363C8A, 0xFE363C82,
    0x00000036, 0x00020019, 0xFE363C82, 0x00000000, 0x0003005B, (u32)btlevtcmd_RunDataEventChild,
    0xFE363C8A, 0x00000036, 0x00000021, 0x00000021, 0x0004005B, (u32)btlevtcmd_CheckDamagePattern,
    0xFE363C8A, 0x00000012, 0xFE363C80, 0x00020019, 0xFE363C80, 0x00000000,
    0x0001005E, (u32)subset_ultra_hammer_acrobat_damage_event, 0x00010004, 0x0000000A, 0x00000021, 0x0004005B,
    (u32)btlevtcmd_CheckDamageCode, 0xFE363C8A, 0x00040000, 0xFE363C81, 0x00020019, 0xFE363C81,
    0x00000000, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFE363C8A, 0xFE363C8B, 0x0000001F,
    0x00010009, 0x0000001E, 0x00010004, 0x0000000A, 0x00000021, 0x0003005B,
    (u32)btlevtcmd_GetDamageCode, 0xFE363C8A, 0xFE363C80, 0x00010022, 0xFE363C80, 0x00010024,
    0x0000001F, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFE363C8A, 0x00000025, 0x00010004,
    0x0000000A, 0x00010024, 0x00000020, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFE363C8A,
    0x00000026, 0x00010004, 0x0000000A, 0x00010024, 0x00000021, 0x0003005B,
    (u32)btlevtcmd_RunDataEventChild, 0xFE363C8A, 0x00000027, 0x00010004, 0x0000000A, 0x00010024,
    0x00000022, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFE363C8A, 0x00000028, 0x00010004,
    0x0000000A, 0x00010024, 0x00000023, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFE363C8A,
    0x00000029, 0x00010004, 0x0000000A, 0x00010024, 0x00000024, 0x0003005B,
    (u32)btlevtcmd_RunDataEventChild, 0xFE363C8A, 0x0000002A, 0x00010004, 0x0000000A, 0x00010024,
    0x00000025, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFE363C8A, 0x0000002B, 0x00010004,
    0x0000000A, 0x00010024, 0x0000002A, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFE363C8A,
    0x0000002F, 0x00010004, 0x0000000A, 0x00010024, 0x00000026, 0x0004005B,
    (u32)btlevtcmd_CheckDataOfDataTable, 0xFE363C8A, 0xFE363C82, 0x0000002D, 0x00020019, 0xFE363C82,
    0x00000000, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFE363C8A, 0x0000002D, 0x00000020,
    0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_SYSTEM_LUCKY1_802ee794, 0x00000000, 0x00000021, 0x00010004,
    0x0000000A, 0x00010024, 0x00000027, 0x0004005B, (u32)btlevtcmd_CheckDataOfDataTable, 0xFE363C8A,
    0xFE363C82, 0x0000002C, 0x00020019, 0xFE363C82, 0x00000000, 0x0003005B,
    (u32)btlevtcmd_RunDataEventChild, 0xFE363C8A, 0x0000002C, 0x00000020, 0x0003005B, (u32)btlevtcmd_CheckCommandUnit,
    0xFE363C8A, 0xFE363C80, 0x00020019, 0xFE363C80, 0x00000000, 0x0003005B,
    (u32)evt_snd_sfxon, (u32)(char*)str_SFX_MARIO_AVOID1_802ee7a8, 0x00000000, 0x00000020, 0x0003005B, (u32)evt_snd_sfxon,
    (u32)(char*)str_SFX_BTL_ATTACK_MISS1_802ee7bc, 0x00000000, 0x00000021, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFE363C8A,
    0xFE363C8B, 0x00000030, 0x0005005B, (u32)btlevtcmd_AnimeChangePaperGroup, 0xFE363C8A, 0xFE363C8B,
    (u32)(char*)str_p_b_st_802ee7d4, 0x00000001, 0x0004005B, (u32)btlevtcmd_CheckStatus, 0xFE363C8A, 0x0000000F,
    0xFE363C82, 0x00020019, 0xFE363C82, 0x00000000, 0x0004005B, (u32)btlevtcmd_AnimeChangePaperAnime,
    0xFE363C8A, 0xFE363C8B, (u32)(char*)str_PM_F_3A_802ee7dc, 0x0003005B, (u32)btlevtcmd_ftomsec, 0x00000025,
    0xFE363C80, 0x0001000A, 0xFE363C80, 0x0004005B, (u32)btlevtcmd_AnimeChangePaperAnime, 0xFE363C8A,
    0xFE363C8B, (u32)(char*)str_PM_F_3B_802ee7e4, 0x0003005B, (u32)btlevtcmd_ftomsec, 0x00000016, 0xFE363C80,
    0x0001000A, 0xFE363C80, 0x00000020, 0x0004005B, (u32)btlevtcmd_CheckStatus, 0xFE363C8A,
    0x0000000E, 0xFE363C82, 0x00020019, 0xFE363C82, 0x00000000, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePaperAnime, 0xFE363C8A, 0xFE363C8B, (u32)(char*)str_PM_Y_2A_802ee7ec, 0x0003005B, (u32)btlevtcmd_ftomsec,
    0x00000028, 0xFE363C80, 0x0001000A, 0xFE363C80, 0x0004005B, (u32)btlevtcmd_AnimeChangePaperAnime,
    0xFE363C8A, 0xFE363C8B, (u32)(char*)str_PM_Y_2B_802ee7f4, 0x0003005B, (u32)btlevtcmd_ftomsec, 0x00000041,
    0xFE363C80, 0x0001000A, 0xFE363C80, 0x00000020, 0x0004005B, (u32)btlevtcmd_AnimeChangePaperAnime,
    0xFE363C8A, 0xFE363C8B, (u32)(char*)str_PM_Y_2A_802ee7ec, 0x0003005B, (u32)btlevtcmd_ftomsec, 0x00000028,
    0xFE363C80, 0x0001000A, 0xFE363C80, 0x0004005B, (u32)btlevtcmd_AnimeChangePaperAnime, 0xFE363C8A,
    0xFE363C8B, (u32)(char*)str_PM_Y_2B_802ee7f4, 0x0003005B, (u32)btlevtcmd_ftomsec, 0x00000041, 0xFE363C80,
    0x0001000A, 0xFE363C80, 0x00000021, 0x00000021, 0x00000021, 0x00010004,
    0x0000000A, 0x00010024, 0x00000028, 0x0004005B, (u32)btlevtcmd_CheckDataOfDataTable, 0xFE363C8A,
    0xFE363C82, 0x0000002E, 0x00020019, 0xFE363C82, 0x00000000, 0x0003005B,
    (u32)btlevtcmd_RunDataEventChild, 0xFE363C8A, 0x0000002E, 0x00000020, 0x00000021, 0x00000031,
    0x0004005B, (u32)btlevtcmd_CheckDamagePattern, 0xFE363C8A, 0x00000015, 0xFE363C80, 0x00020019,
    0xFE363C80, 0x00000000, 0x00020032, 0xFE363C8E, 0x00000027, 0x0001005E,
    (u32)subsetevt_binta_damage_left, 0x0001005B, (u32)btlevtcmd_WaitAttackEnd, 0x00000002, 0x00000021, 0x0004005B,
    (u32)btlevtcmd_CheckDamagePattern, 0xFE363C8A, 0x00000016, 0xFE363C80, 0x00020019, 0xFE363C80,
    0x00000000, 0x00020032, 0xFE363C8E, 0x00000027, 0x0001005E, (u32)subsetevt_binta_damage_right,
    0x0001005B, (u32)btlevtcmd_WaitAttackEnd, 0x00000002, 0x00000021, 0x0003005B, (u32)btlevtcmd_GetDamageCode,
    0xFE363C8A, 0xFE363C80, 0x00010022, 0xFE363C80, 0x00010024, 0x00000039,
    0x0004005B, (u32)btlevtcmd_CheckDataOfDataTable, 0xFE363C8A, 0xFE363C82, 0x00000030, 0x00020019,
    0xFE363C82, 0x00000000, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFE363C8A, 0x00000030,
    0x00000021, 0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFE363C8A, 0x00000002, 0x0001002B,
    0x0000001E, 0x0001002B, 0x0000001B, 0x0001002B, 0x0000001A, 0x0001002B,
    0x00000017, 0x0003005B, (u32)btlevtcmd_OnUnitFlag, 0xFE363C8A, 0x00000001, 0x00020032,
    0xFE363C8E, 0x00000027, 0x00020032, 0xFE363C8F, 0xFFFFFFFF, 0x0004005B,
    (u32)btlevtcmd_CheckDamagePattern, 0xFE363C8A, 0x00000001, 0xFE363C80, 0x00020019, 0xFE363C80,
    0x00000000, 0x00020032, 0xFE363C8E, 0x00000026, 0x00000021, 0x0000006E,
    0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFE363C8A, 0x0000000A, 0x00000070, 0x0001005E,
    (u32)subsetevt_damage, 0x0000002E, 0x00010024, 0x00000016, 0x0005005B, (u32)btlevtcmd_CheckDefinePosetype,
    0xFE363C8A, 0xFE363C8B, 0x00000023, 0xFE363C80, 0x00020019, 0xFE363C80,
    0x00000000, 0x00020032, 0xFE363C8E, 0x00000023, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType,
    0xFE363C8A, 0xFE363C8B, 0xFE363C8E, 0x0000006E, 0x0003005B, (u32)btlevtcmd_RunDataEventChild,
    0xFE363C8A, 0x0000000A, 0x00000070, 0x00010009, 0x0000002D, 0x00000020,
    0x00010009, 0x0000002D, 0x00000021, 0x00010024, 0x00000018, 0x0004005B,
    (u32)btlevtcmd_CheckDataOfDataTable, 0xFE363C8A, 0xFE363C82, 0x0000000B, 0x00020019, 0xFE363C82,
    0x00000000, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFE363C8A, 0x0000000B, 0x00000020,
    0x0003005B, (u32)btlevtcmd_OnUnitFlag, 0xFE363C8A, 0x00000001, 0x00020032, 0xFE363C8E,
    0x0000001D, 0x00020032, 0xFE363C8F, 0xFFFFFFFF, 0x0000006E, 0x0003005B,
    (u32)btlevtcmd_RunDataEventChild, 0xFE363C8A, 0x0000000A, 0x00000070, 0x0001005E, (u32)subsetevt_damage,
    0x00000021, 0x00010024, 0x00000019, 0x0004005B, (u32)btlevtcmd_CheckDataOfDataTable, 0xFE363C8A,
    0xFE363C82, 0x0000000C, 0x00020019, 0xFE363C82, 0x00000000, 0x0003005B,
    (u32)btlevtcmd_RunDataEventChild, 0xFE363C8A, 0x0000000C, 0x00000020, 0x0003005B, (u32)btlevtcmd_OnUnitFlag,
    0xFE363C8A, 0x00000001, 0x00020032, 0xFE363C8E, 0x0000001D, 0x00020032,
    0xFE363C8F, 0xFFFFFFFF, 0x0000006E, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFE363C8A,
    0x0000000A, 0x00000070, 0x0001005E, (u32)subsetevt_damage, 0x00000021, 0x00010024,
    0x0000002B, 0x0004005B, (u32)btlevtcmd_CheckDataOfDataTable, 0xFE363C8A, 0xFE363C82, 0x00000018,
    0x00020019, 0xFE363C82, 0x00000000, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFE363C8A,
    0x00000018, 0x00000020, 0x0001005E, (u32)subsetevt_counter_damage, 0x00000021, 0x00010024,
    0x0000002C, 0x0004005B, (u32)btlevtcmd_CheckDataOfDataTable, 0xFE363C8A, 0xFE363C82, 0x00000019,
    0x00020019, 0xFE363C82, 0x00000000, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFE363C8A,
    0x00000019, 0x00000020, 0x0001005E, (u32)subsetevt_counter_damage, 0x00000021, 0x00010024,
    0x0000002D, 0x0004005B, (u32)btlevtcmd_CheckDataOfDataTable, 0xFE363C8A, 0xFE363C82, 0x0000001A,
    0x00020019, 0xFE363C82, 0x00000000, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFE363C8A,
    0x0000001A, 0x00000020, 0x0001005E, (u32)subsetevt_counter_damage, 0x00000021, 0x00010024,
    0x0000002E, 0x0004005B, (u32)btlevtcmd_CheckDataOfDataTable, 0xFE363C8A, 0xFE363C82, 0x0000001B,
    0x00020019, 0xFE363C82, 0x00000000, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFE363C8A,
    0x0000001B, 0x00000020, 0x0001005E, (u32)subsetevt_counter_damage, 0x00000021, 0x00010024,
    0x0000002F, 0x0004005B, (u32)btlevtcmd_CheckDataOfDataTable, 0xFE363C8A, 0xFE363C82, 0x0000001C,
    0x00020019, 0xFE363C82, 0x00000000, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFE363C8A,
    0x0000001C, 0x00000020, 0x0001005E, (u32)subsetevt_counter_damage, 0x00000021, 0x00010024,
    0x00000030, 0x0004005B, (u32)btlevtcmd_CheckDataOfDataTable, 0xFE363C8A, 0xFE363C82, 0x0000001D,
    0x00020019, 0xFE363C82, 0x00000000, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFE363C8A,
    0x0000001D, 0x00000020, 0x0001005E, (u32)subsetevt_counter_damage, 0x00000021, 0x00010024,
    0x00000031, 0x0004005B, (u32)btlevtcmd_CheckDataOfDataTable, 0xFE363C8A, 0xFE363C82, 0x0000001E,
    0x00020019, 0xFE363C82, 0x00000000, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFE363C8A,
    0x0000001E, 0x00000020, 0x0001005E, (u32)subsetevt_counter_damage, 0x00000021, 0x00010024,
    0x00000032, 0x0004005B, (u32)btlevtcmd_CheckDataOfDataTable, 0xFE363C8A, 0xFE363C82, 0x0000001F,
    0x00020019, 0xFE363C82, 0x00000000, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFE363C8A,
    0x0000001F, 0x00000020, 0x0001005E, (u32)subsetevt_counter_damage, 0x00000021, 0x00010024,
    0x00000033, 0x0004005B, (u32)btlevtcmd_CheckDataOfDataTable, 0xFE363C8A, 0xFE363C82, 0x00000020,
    0x00020019, 0xFE363C82, 0x00000000, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFE363C8A,
    0x00000020, 0x00000020, 0x0001005E, (u32)subsetevt_counter_damage, 0x00000021, 0x00010024,
    0x00000034, 0x0004005B, (u32)btlevtcmd_CheckDataOfDataTable, 0xFE363C8A, 0xFE363C82, 0x00000021,
    0x00020019, 0xFE363C82, 0x00000000, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFE363C8A,
    0x00000021, 0x00000020, 0x0001005E, (u32)subsetevt_counter_damage, 0x00000021, 0x00010024,
    0x00000035, 0x0004005B, (u32)btlevtcmd_CheckDataOfDataTable, 0xFE363C8A, 0xFE363C82, 0x00000022,
    0x00020019, 0xFE363C82, 0x00000000, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFE363C8A,
    0x00000022, 0x00000020, 0x0001005E, (u32)subsetevt_counter_damage, 0x00000021, 0x00010024,
    0x00000036, 0x0004005B, (u32)btlevtcmd_CheckDataOfDataTable, 0xFE363C8A, 0xFE363C82, 0x00000023,
    0x00020019, 0xFE363C82, 0x00000000, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFE363C8A,
    0x00000023, 0x00000020, 0x0001005E, (u32)subsetevt_counter_damage, 0x00000021, 0x00010024,
    0x00000037, 0x0004005B, (u32)btlevtcmd_CheckDataOfDataTable, 0xFE363C8A, 0xFE363C82, 0x00000024,
    0x00020019, 0xFE363C82, 0x00000000, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFE363C8A,
    0x00000024, 0x00000020, 0x0001005E, (u32)subsetevt_counter_damage, 0x00000021, 0x00010024,
    0x00000029, 0x0001005E, (u32)btldefaultevt_avoid_guard, 0x00000002, 0x00010024, 0x00000038,
    0x0001005E, (u32)btldefaultevt_guard_move, 0x00010024, 0x0000001C, 0x0001005E, (u32)subsetevt_terror,
    0x00000002, 0x00010024, 0x0000001D, 0x0004005B, (u32)btlevtcmd_CheckDataOfDataTable, 0xFE363C8A,
    0xFE363C82, 0x0000003A, 0x00020019, 0xFE363C82, 0x00000000, 0x0003005B,
    (u32)btlevtcmd_RunDataEventChild, 0xFE363C8A, 0x0000003A, 0x00000020, 0x0001005E, (u32)subsetevt_blow_dead,
    0x00000021, 0x00000002, 0x0000002A, 0x00000031, 0x00010003, 0x0000000A,
    0x0004005B, (u32)btlevtcmd_CheckDamageCode, 0xFE363C8A, 0x00000400, 0xFE363C81, 0x00020019,
    0xFE363C81, 0x00000000, 0x00010009, 0x0000001E, 0x0001005E, (u32)subsetevt_dead,
    0x00000002, 0x00000020, 0x0004005B, (u32)btlevtcmd_CheckDamageCode, 0xFE363C8A, 0x00000200,
    0xFE363C81, 0x00020019, 0xFE363C81, 0x00000000, 0x0003005B, (u32)btlevtcmd_RunDataEventChild,
    0xFE363C8A, 0x0000000F, 0x00000002, 0x00000021, 0x00000021, 0x0004005B,
    (u32)btlevtcmd_CheckDamageCode, 0xFE363C8A, 0x00004000, 0xFE363C81, 0x00020019, 0xFE363C81,
    0x00000000, 0x0002005B, (u32)_backfire, 0xFE363C8A, 0x00000002, 0x00000021,
    0x0004005B, (u32)btlevtcmd_CheckDamageCode, 0xFE363C8A, 0x00001000, 0xFE363C81, 0x00020019,
    0xFE363C81, 0x00000000, 0x0001005E, (u32)btldefaultevt_guard_return, 0x00000002, 0x00000021,
    0x0004005B, (u32)btlevtcmd_CheckDamageCode, 0xFE363C8A, 0x00000100, 0xFE363C81, 0x00020018,
    0xFE363C81, 0x00000000, 0x0001005B, (u32)btlevtcmd_WaitAttackEnd, 0x00000021, 0x00010003,
    0x00000063, 0x0004005B, (u32)btlevtcmd_CheckAttribute, 0xFE363C8A, 0x00000004, 0xFE363C80,
    0x00020019, 0xFE363C80, 0x00000000, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFE363C8A,
    0xFE363C85, 0xFE363C86, 0xFE363C87, 0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFE363C8A,
    0xFE363C82, 0xFE363C83, 0xFE363C84, 0x00020018, 0xFE363C82, 0xFE363C85,
    0x00020018, 0xFE363C83, 0xFE363C86, 0x00020018, 0xFE363C84, 0xFE363C87,
    0x00010004, 0x00000065, 0x00000021, 0x00000021, 0x00000021, 0x00020032,
    0xFE363C8E, 0x0000002A, 0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFE363C8A, 0xF24A8A80,
    0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFE363C8A, 0xFE363C82, 0xFE363C83, 0xFE363C84,
    0x0007005B, (u32)btlevtcmd_GetMoveFrame, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00000000, 0xFE363C87, 0x0002001B, 0xFE363C87, 0x00000000, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePoseType, 0xFE363C8A, 0xFE363C8B, 0xFE363C8E, 0x000A005B, (u32)btlevtcmd_DivePosition,
    0xFE363C8A, 0xFE363C82, 0xFE363C83, 0xFE363C84, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0xFFFFFFFF, 0x00000021, 0x00010003, 0x00000065,
    0x00000020, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFE363C8A, 0xFE363C85, 0xFE363C86,
    0xFE363C87, 0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFE363C8A, 0xFE363C82, 0xFE363C83,
    0xFE363C84, 0x00020018, 0xFE363C82, 0xFE363C85, 0x00020018, 0xFE363C83,
    0xFE363C86, 0x00020018, 0xFE363C84, 0xFE363C87, 0x00010004, 0x00000066,
    0x00000021, 0x00000021, 0x00000021, 0x00020032, 0xFE363C8E, 0x00000028,
    0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFE363C8A, 0xF24A8A80, 0x0005005B, (u32)btlevtcmd_GetHomePos,
    0xFE363C8A, 0xFE363C82, 0xFE363C83, 0xFE363C84, 0x0007005B, (u32)btlevtcmd_GetMoveFrame,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000, 0xFE363C87,
    0x0002001B, 0xFE363C87, 0x00000000, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFE363C8A,
    0xFE363C8B, 0xFE363C8E, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFE363C8A, 0xFE363C82,
    0xFE363C83, 0xFE363C84, 0x00000000, 0xFFFFFFFF, 0x00000000, 0x00000021,
    0x00010003, 0x00000066, 0x00000021, 0x0002005B, (u32)btlevtcmd_ResetFaceDirection, 0xFE363C8A,
    0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFE363C8A, 0x00000002, 0x00000001,
};

u32 btldefaultevt_ChangeParty[] = {
    0x0001005B, (u32)_anime_load, 0x0003005B, (u32)btlevtcmd_GetPartyId, 0xFFFFFFFE, 0xFE363C83,
    0x0003005B, (u32)btlevtcmd_GetBodyId, 0xFE363C83, 0xFE363C84, 0x0003005B, (u32)btlevtcmd_SetFallAccel,
    0xFE363C83, 0xF24A7D4C, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFE363C83, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0003005B, (u32)btlevtcmd_GetMarioId, 0xFFFFFFFE, 0xFE363C88,
    0x0005005B, (u32)btlevtcmd_GetPos, 0xFE363C88, 0xFE363C85, 0xFE363C86, 0xFE363C87,
    0x0000006E, 0x0005005B, (u32)btlevtcmd_SetScale, 0xFE363C83, 0xF24A7E80, 0xF24A7E80,
    0xF24A7E80, 0x00010005, 0x00000012, 0x0005005B, (u32)btlevtcmd_AddScale, 0xFE363C83,
    0xF24A7A4D, 0xF24A7A4D, 0xF24A7A4D, 0x00010009, 0x00000001, 0x00000006,
    0x00000070, 0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_BTL_PARTY_IN1_802ee7fc, 0x00000000, 0x0007005B,
    (u32)btlevtcmd_JumpPosition, 0xFE363C83, 0xFE363C85, 0xFE363C86, 0xFE363C87, 0x00000014,
    0x00000000, 0x0001005B, (u32)_anime_load_wait, 0x0002005B, (u32)btlevtcmd_ChangeParty, 0xFE363C89,
    0x0003005B, (u32)btlevtcmd_OffUnitFlag, 0xFE363C89, 0x00200000, 0x0003005B, (u32)btlevtcmd_GetBodyId,
    0xFE363C89, 0xFE363C80, 0x0003005B, (u32)btlevtcmd_AnimeChangePoseFromTable, 0xFE363C89, 0xFE363C80,
    0x0005005B, (u32)btlevtcmd_SetPos, 0xFE363C89, 0xFE363C85, 0xFE363C86, 0xFE363C87,
    0x0003005B, (u32)btlevtcmd_SetFallAccel, 0xFE363C89, 0xF24A7D4C, 0x0004005B, (u32)btlevtcmd_CheckDataOfDataTable,
    0xFE363C89, 0xFE363C80, 0x00000004, 0x00020019, 0xFE363C80, 0x00000000,
    0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFE363C89, 0x00000004, 0x00000021, 0x0003005B,
    (u32)evt_snd_sfxon, (u32)(char*)str_SFX_BTL_PARTY_CHANGE_802ee810, 0x00000000, 0x0003005B, (u32)evt_audience_sound_cheer, 0x0000002D,
    0x00000019, 0x0000006E, 0x0005005B, (u32)btlevtcmd_SetScale, 0xFE363C89, 0xF24A7AE6,
    0xF24A7AE6, 0xF24A7AE6, 0x00010005, 0x00000012, 0x0005005B, (u32)btlevtcmd_AddScale,
    0xFE363C89, 0xF24A7AB3, 0xF24A7AB3, 0xF24A7AB3, 0x00010009, 0x00000001,
    0x00000006, 0x0005005B, (u32)btlevtcmd_SetScale, 0xFE363C89, 0xF24A7E80, 0xF24A7E80,
    0xF24A7E80, 0x00000070, 0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFE363C89, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_PARTY_OUT1_802ee828, 0x00000000,
    0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFE363C89, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00000014, 0x00000000, 0x0003005B, (u32)btlevtcmd_OnUnitFlag, 0xFE363C89, 0x00200000,
    0x0003005B, (u32)btlevtcmd_KillUnit, 0xFE363C83, 0x00000000, 0x0001005B, (u32)btlevtcmd_SortPhaseMoveTable,
    0x0002005B, (u32)_check_pose_stay, 0xFE363C80, 0x00020019, 0xFE363C80, 0x00000000,
    0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000021, 0x00000002, 0x00000001,
};

u32 btldefaultevt_BiribiriMove[] = {
    0x0005005B, (u32)evt_sub_intpl_init, 0x00000008, 0x00000000, 0x00000005, 0x00000010,
    0x00010003, 0x00000000, 0x0001005B, (u32)evt_sub_intpl_get_value, 0x0005005B, (u32)btlevtcmd_SetDispOffset,
    0xFFFFFFFE, 0xFE363C80, 0x00000000, 0x00000000, 0x00010009, 0x00000001,
    0x00020018, 0xFE363C81, 0x00000001, 0x00010004, 0x00000000, 0x00000021,
    0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFE363C8A, 0x00000002, 0x00000001,
};

u32 btldefaultevt_guard_move[] = {
    0x0006005B, (u32)btlevtcmd_AnimeCmpPoseType, 0xFE363C8A, 0xFE363C8B, 0x0000002A, 0x00000028,
    0xFE363C82, 0x00020018, 0xFE363C82, 0x00000000, 0x0004005B, (u32)btlevtcmd_AnimeSetMotionSpeed,
    0xFE363C8A, 0xFE363C8B, 0xF24A8280, 0x00000021, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType,
    0xFE363C8A, 0xFE363C8B, 0x0000002A, 0x0004005B, (u32)btlevtcmd_CheckAttribute, 0xFE363C8A,
    0x00000004, 0xFE363C86, 0x00020019, 0xFE363C86, 0x00000000, 0x0003005B,
    (u32)btlevtcmd_GetProtectId, 0xFE363C8A, 0xFE363C86, 0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFE363C86,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000A005B, (u32)btlevtcmd_DivePosition, 0xFE363C8A,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000014, 0x00000000, 0x00000000,
    0x00000000, 0xFFFFFFFF, 0x00000020, 0x0003005B, (u32)btlevtcmd_GetProtectId, 0xFE363C8A,
    0xFE363C86, 0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFE363C86, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFE363C8A, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000014, 0xFFFFFFFF, 0x00000000, 0x00000021, 0x0004005B,
    (u32)btlevtcmd_AnimeSetMotionSpeed, 0xFE363C8A, 0xFE363C8B, 0xF24A7E80, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType,
    0xFE363C8A, 0xFE363C8B, 0x0000002E, 0x0001005B, (u32)btlevtcmd_GuardMoveEnd, 0x00000002,
    0x00000001,
};

u32 btldefaultevt_guard_return[] = {
    0x0001005B, (u32)btlevtcmd_WaitAttackEnd, 0x0003005B, (u32)btlevtcmd_CheckGuardMotionEnable, 0xFE363C8A, 0xFE363C80,
    0x00020019, 0xFE363C80, 0x00000000, 0x0006005B, (u32)btlevtcmd_AnimeCmpPoseType, 0xFE363C8A,
    0xFE363C8B, 0x0000002A, 0x00000028, 0xFE363C82, 0x00020018, 0xFE363C82,
    0x00000000, 0x0004005B, (u32)btlevtcmd_AnimeSetMotionSpeed, 0xFE363C8A, 0xFE363C8B, 0xF24A8280,
    0x00000021, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFE363C8A, 0xFE363C8B, 0x0000002A,
    0x00000021, 0x0004005B, (u32)btlevtcmd_CheckAttribute, 0xFE363C8A, 0x00000004, 0xFE363C86,
    0x00020019, 0xFE363C86, 0x00000000, 0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFE363C8A,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000A005B, (u32)btlevtcmd_DivePosition, 0xFE363C8A,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000014, 0x00000000, 0x00000000,
    0x00000000, 0xFFFFFFFF, 0x00000020, 0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFE363C8A,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFE363C8A,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000014, 0xFFFFFFFF, 0x00000000,
    0x00000021, 0x0003005B, (u32)btlevtcmd_CheckGuardMotionEnable, 0xFE363C8A, 0xFE363C80, 0x00020019,
    0xFE363C80, 0x00000000, 0x0004005B, (u32)btlevtcmd_AnimeSetMotionSpeed, 0xFE363C8A, 0xFE363C8B,
    0xF24A7E80, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFE363C8A, 0xFE363C8B, 0x0000002D,
    0x00000021, 0x0001005B, (u32)btlevtcmd_GuardMoveEnd, 0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFE363C8A,
    0x00000002, 0x00000001,
};

u32 btldefaultevt_Appeal[] = {
    0x0003005B, (u32)btlevtcmd_GetBodyId, 0xFFFFFFFE, 0xFE363C80, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType,
    0xFFFFFFFE, 0xFE363C80, 0x0000002B, 0x0004005B, (u32)btlevtcmd_CheckStatus, 0xFFFFFFFE,
    0x0000000A, 0xFE363C80, 0x00020018, 0xFE363C80, 0x00000000, 0x0005005B,
    (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00020032,
    0xFE363C83, 0xFE363C81, 0x00020035, 0xFE363C81, 0x00000035, 0x00020035,
    0xFE363C83, 0x00000023, 0x00020032, 0xFE363C84, 0xFE363C82, 0x00020035,
    0xFE363C82, 0x000000FA, 0x00000020, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00020032, 0xFE363C83, 0xFE363C81,
    0x00020035, 0xFE363C81, 0x00000096, 0x00020035, 0xFE363C83, 0x00000050,
    0x00020032, 0xFE363C84, 0xFE363C82, 0x00020035, 0xFE363C82, 0x00000190,
    0x00000021, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000003, 0x000A005B,
    (u32)evt_btl_camera_set_moveto, 0x00000000, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0xFE363C80,
    0xFE363C83, 0xFE363C84, 0x0000001E, 0x0000000B, 0x0002005B, (u32)evt_batstage_light_set_player_on,
    0x00000000, 0x00010009, 0x0000001E, 0x0004005B, (u32)btlevtcmd_CheckDataOfDataTable, 0xFFFFFFFE,
    0xFE363C80, 0x00000005, 0x00020019, 0xFE363C80, 0x00000000, 0x0003005B,
    (u32)btlevtcmd_RunDataEventChild, 0xFFFFFFFE, 0x00000005, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000,
    0x00000000, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000002, 0x0001005B,
    (u32)evt_batstage_light_set_player_off, 0x00010009, 0x0000003C, 0x00000002, 0x00000021, 0x0003005B,
    (u32)btlevtcmd_GetBodyId, 0xFFFFFFFE, 0xFE363C80, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE,
    0xFE363C80, 0x00000042, 0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_MARIO_APPEAL1_802ee838,
    0xF1194D80, 0x00000000, 0xF1194D80, 0x00010009, 0x0000003C, 0x0003005B,
    (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000,
    0x00000002, 0x0002005B, (u32)btlevtcmd_AudienceAppeal, 0xFFFFFFFE, 0x0001005B, (u32)evt_batstage_light_set_player_off,
    0x00010009, 0x0000003C, 0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFE363C8A, 0x00000002,
    0x00000001,
};

u32 _Defence_core[] = {
    0x0003005B, (u32)btlevtcmd_GetUnitId, 0xFFFFFFFE, 0xFE363C83, 0x0003005B, (u32)btlevtcmd_GetBodyId,
    0xFFFFFFFE, 0xFE363C84, 0x00020018, 0xFE363C83, 0xFFFFFFFF, 0x00010004,
    0x00000063, 0x00000021, 0x0003005B, (u32)btlevtcmd_PayWeaponCost, 0xFFFFFFFE, 0xFE363C8C,
    0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFFFFFFFE, 0x00000007, 0x0006005B, (u32)btlevtcmd_snd_se,
    0xFFFFFFFE, (u32)(char*)str_SFX_BTL_MARIO_DEFENC_802ee84c, 0xF1194D80, 0x00000000, 0xF1194D80, 0x00010005,
    0x00000014, 0x0005005B, (u32)btlevtcmd_AddRotate, 0xFFFFFFFE, 0x00000000, 0x00000024,
    0x00000000, 0x00010009, 0x00000001, 0x00000006, 0x0005005B, (u32)btlevtcmd_SetRotate,
    0xFFFFFFFE, 0x00000000, 0x00000000, 0x00000000, 0x0002005B, (u32)btlevtcmd_WeaponAftereffect,
    0xFE363C8C, 0x0003005B, (u32)btlevtcmd_OnStatusFlag, 0xFFFFFFFE, 0x01000000, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0xFE363C84, 0x0000002D, 0x0006005B, (u32)btlevtcmd_snd_se,
    0xFFFFFFFE, (u32)(char*)str_SFX_BTL_MARIO_DEFENC_802ee864, 0xF1194D80, 0x00000000, 0xF1194D80, 0x0003005B,
    (u32)btlevtcmd_ftof, 0x0000003C, 0xFE363C80, 0x0006005B, (u32)btlevtcmd_AnnounceMessage, 0x00000000,
    0x00000000, 0x00000000, (u32)(char*)str_btl_msg_defend_comma_802ee87c, 0xFE363C80, 0x0002005B, (u32)btlevtcmd_StartWaitEvent,
    0xFFFFFFFE, 0x00000002, 0x00000001,
};

u32 btldefaultevt_Escape[] = {
    0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000014, 0x0001005B, (u32)_btl_escape_reset_move_color_lv_player_unit_all,
    0x0003005B, (u32)btlevtcmd_AcSetDifficulty, 0xFFFFFFFE, 0x00000003, 0x0003005B, (u32)evt_sub_random,
    0x00000063, 0xFE363C80, 0x0003005B, (u32)evt_sub_random, 0x00000006, 0xFE363C81,
    0x00020035, 0xFE363C81, 0x0000001E, 0x0002005B, (u32)_get_escape_rate, 0xFE363C82,
    0x0009005B, (u32)btlevtcmd_AcSetParamAll, 0x00000001, 0x000000B4, 0xFE363C80, 0xFE363C81,
    0x00000000, 0x00000004, 0x00000006, 0xFE363C82, 0x0002005B, (u32)btlevtcmd_AcSetFlag,
    0x00000003, 0x0005005B, (u32)btlevtcmd_SetupAC, 0xFFFFFFFE, 0x00000005, 0x00000001,
    0x00000000, 0x0001005B, (u32)_set_escape_ac_hlp_msg, 0x0000006B, 0x0003005B, (u32)btlevtcmd_GetPartnerId,
    0xFFFFFFFE, 0xFE363C8A, 0x00020019, 0xFE363C8A, 0xFFFFFFFF, 0x0003005B,
    (u32)btlevtcmd_CheckActStatus, 0xFE363C8A, 0xFE363C80, 0x00020019, 0xFE363C80, 0x00000000,
    0x0003005B, (u32)btlevtcmd_GetBodyId, 0xFE363C8A, 0xFE363C8B, 0x0007005B, (u32)btlevtcmd_CalculateFaceDirection,
    0xFE363C8A, 0xFE363C8B, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000104, 0xFE363C80,
    0x0003005B, (u32)btlevtcmd_ChangeFaceDirection, 0xFE363C8A, 0xFE363C80, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType,
    0xFE363C8A, 0xFE363C8B, 0x0000002A, 0x0005005B, (u32)btlevtcmd_JumpSetting, 0xFE363C8A,
    0x00000014, 0xF24A7A80, 0xF24A7D4C, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFE363C8A,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFE363C8A,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000010, 0xFFFFFFFF, 0x0007005B,
    (u32)btlevtcmd_JumpPosition, 0xFE363C8A, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000010,
    0xFFFFFFFF, 0x00000021, 0x00000021, 0x0000006D, 0x0003005B, (u32)btlevtcmd_GetBodyId,
    0xFFFFFFFE, 0xFE363C8B, 0x0007005B, (u32)btlevtcmd_CalculateFaceDirection, 0xFFFFFFFE, 0xFE363C8B,
    0xFFFFFFFF, 0xFFFFFFFF, 0x00000104, 0xFE363C80, 0x0003005B, (u32)btlevtcmd_ChangeFaceDirection,
    0xFFFFFFFE, 0xFE363C80, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0xFE363C8B,
    0x0000002A, 0x0005005B, (u32)btlevtcmd_JumpSetting, 0xFFFFFFFE, 0x00000014, 0xF24A7A80,
    0xF24A7D4C, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000010, 0xFFFFFFFF, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000010, 0xFFFFFFFF, 0x0001005B,
    (u32)btlevtcmd_audience_code_escape, 0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_MARIO_BATTLE_STA_802ee898, 0xFE363C8F, 0x0000006B,
    0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE, 0xFE363C80, 0x0000000F, 0x0008005B,
    (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000000B4,
    0xFFFFFFFF, 0x00000000, 0x0000006D, 0x0000006B, 0x0003005B, (u32)btlevtcmd_GetPartnerId,
    0xFFFFFFFE, 0xFE363C8A, 0x00020019, 0xFE363C8A, 0xFFFFFFFF, 0x0003005B,
    (u32)btlevtcmd_CheckActStatus, 0xFE363C8A, 0xFE363C80, 0x00020019, 0xFE363C80, 0x00000000,
    0x0005005B, (u32)btlevtcmd_GetPos, 0xFE363C8A, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFE363C8A, 0xFE363C80, 0x0000000F, 0x0008005B,
    (u32)btlevtcmd_MovePosition, 0xFE363C8A, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000000B4,
    0xFFFFFFFF, 0x00000000, 0x00000021, 0x00000021, 0x0000006D, 0x0002005B,
    (u32)btlevtcmd_StartAC, 0x00000001, 0x0001005B, (u32)btlevtcmd_ResultAC, 0x0001005B, (u32)btlevtcmd_StopAC,
    0x0005005B, (u32)btlevtcmd_JumpSetting, 0xFFFFFFFE, 0x00000014, 0xF24A7A80, 0xF24A7D4C,
    0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00000010, 0xFFFFFFFF, 0x0002005B, (u32)btlevtcmd_GetResultAC, 0xFE363C86, 0x0002001F,
    0xFE363C86, 0x00000002, 0x0002005B, (u32)evt_snd_sfxoff, 0xFE363C8F, 0x0003005B,
    (u32)evt_snd_sfxon, (u32)(char*)str_SFX_MARIO_RUN_AWAY_M_802ee8b0, 0x00000000, 0x0001006F, 0xFE363C8F, 0x0003005B,
    (u32)btlevtcmd_GetPartnerId, 0xFFFFFFFE, 0xFE363C8A, 0x00020019, 0xFE363C8A, 0xFFFFFFFF,
    0x0003005B, (u32)btlevtcmd_CheckActStatus, 0xFE363C8A, 0xFE363C80, 0x00020019, 0xFE363C80,
    0x00000000, 0x0003005B, (u32)btlevtcmd_OffUnitFlag, 0xFE363C8A, 0x00200000, 0x0003005B,
    (u32)btlevtcmd_GetBodyId, 0xFE363C8A, 0xFE363C8B, 0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFE363C8A,
    0xF24AA280, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFE363C8A, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0004005B, (u32)btlevtcmd_FaceDirectionAdd, 0xFE363C8A, 0xFE363C80, 0x0000001E,
    0x0008005B, (u32)btlevtcmd_MovePosition, 0xFE363C8A, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00000000, 0xFFFFFFFF, 0x00000000, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFE363C8A,
    0xFE363C8B, 0x0000003C, 0x0004005B, (u32)btlevtcmd_FaceDirectionAdd, 0xFE363C8A, 0xFE363C80,
    0x00000005, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFE363C8A, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000005, 0xFFFFFFFF, 0x00000000, 0x0004005B, (u32)btlevtcmd_FaceDirectionAdd,
    0xFE363C8A, 0xFE363C80, 0x0000000A, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFE363C8A,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0000003C, 0xFFFFFFFF, 0x00000000,
    0x00010009, 0x00000019, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFE363C8A, 0xFE363C8B,
    0x0000003D, 0x00010009, 0x00000028, 0x0003005B, (u32)btlevtcmd_OnUnitFlag, 0xFE363C8A,
    0x00200000, 0x00000021, 0x00000021, 0x00000070, 0x0003005B, (u32)btlevtcmd_OffUnitFlag,
    0xFE363C8A, 0x00200000, 0x0003005B, (u32)btlevtcmd_GetBodyId, 0xFFFFFFFE, 0xFE363C8B,
    0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE, 0xF24AA280, 0x0005005B, (u32)btlevtcmd_GetPos,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B, (u32)btlevtcmd_FaceDirectionAdd,
    0xFFFFFFFE, 0xFE363C80, 0x0000001E, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000, 0xFFFFFFFF, 0x00000000,
    0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0xFE363C8B, 0x0000003C, 0x0001005B,
    (u32)btlevtcmd_audience_code_escape_fail, 0x0004005B, (u32)btlevtcmd_FaceDirectionAdd, 0xFFFFFFFE, 0xFE363C80, 0x00000005,
    0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00000005, 0xFFFFFFFF, 0x00000000, 0x0004005B, (u32)btlevtcmd_FaceDirectionAdd, 0xFFFFFFFE,
    0xFE363C80, 0x0000000A, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0000003C, 0xFFFFFFFF, 0x00000000, 0x00010009,
    0x00000019, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0xFE363C8B, 0x0000003D,
    0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000, 0x00010009, 0x00000028,
    0x0003005B, (u32)btlevtcmd_OnUnitFlag, 0xFE363C8A, 0x00200000, 0x0001006F, 0xFE363C8F,
    0x0003005B, (u32)btlevtcmd_GetPartnerId, 0xFFFFFFFE, 0xFE363C8A, 0x00020019, 0xFE363C8A,
    0xFFFFFFFF, 0x0003005B, (u32)btlevtcmd_CheckActStatus, 0xFE363C8A, 0xFE363C80, 0x00020019,
    0xFE363C80, 0x00000000, 0x0003005B, (u32)btlevtcmd_GetBodyId, 0xFE363C8A, 0xFE363C8B,
    0x0007005B, (u32)btlevtcmd_CalculateFaceDirection, 0xFE363C8A, 0xFE363C8B, 0x00000000, 0x00000000,
    0x00000004, 0xFE363C80, 0x0003005B, (u32)btlevtcmd_ChangeFaceDirection, 0xFE363C8A, 0xFE363C80,
    0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFE363C8A, 0xFE363C8B, 0x0000002A, 0x0003005B,
    (u32)btlevtcmd_SetMoveSpeed, 0xFE363C8A, 0xF24A8680, 0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFE363C8A,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFE363C8A,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000, 0xFFFFFFFF, 0x00000000,
    0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFE363C8A, 0x00000021, 0x00000021, 0x00000070,
    0x0007005B, (u32)btlevtcmd_CalculateFaceDirection, 0xFFFFFFFE, 0xFE363C8B, 0x00000000, 0x00000000,
    0x00000004, 0xFE363C80, 0x0003005B, (u32)btlevtcmd_ChangeFaceDirection, 0xFFFFFFFE, 0xFE363C80,
    0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0xFE363C8B, 0x0000002A, 0x0003005B,
    (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE, 0xF24A8680, 0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000, 0xFFFFFFFF, 0x00000000,
    0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x0002005B, (u32)btlevtcmd_WaitEventEnd, 0xFE363C8F,
    0x00000002, 0x00000021, 0x0000006E, 0x0003005B, (u32)btlevtcmd_GetPartnerId, 0xFFFFFFFE,
    0xFE363C8A, 0x00020019, 0xFE363C8A, 0xFFFFFFFF, 0x0003005B, (u32)btlevtcmd_CheckActStatus,
    0xFE363C8A, 0xFE363C80, 0x00020019, 0xFE363C80, 0x00000000, 0x0003005B,
    (u32)btlevtcmd_SetMoveSpeed, 0xFE363C8A, 0xF24AA280, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFE363C8A,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B, (u32)btlevtcmd_FaceDirectionAdd, 0xFE363C8A,
    0xFE363C80, 0x000000C8, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFE363C8A, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000000, 0xFFFFFFFF, 0x00000000, 0x00000021,
    0x00000021, 0x00000070, 0x0002005B, (u32)evt_snd_sfxoff, 0xFE363C8F, 0x0001005B,
    (u32)btlevtcmd_audience_code_escape_success, 0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_MARIO_RUN_AWAY1_802ee8cc, 0x00000000, 0x0003005B,
    (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE, 0xF24AA280, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B, (u32)btlevtcmd_FaceDirectionAdd, 0xFFFFFFFE,
    0xFE363C80, 0x000000C8, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000000, 0x00000000, 0x00000000, 0x00020032,
    0xFE363C8F, 0x00000000, 0x0003005B, (u32)btlevtcmd_check_battleflag, 0xFE363C80, 0x00400000,
    0x00020019, 0xFE363C80, 0x00000000, 0x0001006F, 0xFE363C8F, 0x0001005E,
    (u32)_event_MajinaiEnd, 0x00000070, 0x00000021, 0x00020019, 0xFE363C8F, 0x00000000,
    0x0002005B, (u32)btlevtcmd_WaitEventEnd, 0xFE363C8F, 0x00000021, 0x0002005B, (u32)btlevtcmd_InterruptStop,
    0x00000000, 0x00000002, 0x00000001,
};

u32 btldefaultevt_Confuse[] = {
    0x0003005B, (u32)btlevtcmd_GetBodyId, 0xFFFFFFFE, 0xFE363C80, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType,
    0xFFFFFFFE, 0xFE363C80, 0x00000005, 0x00010009, 0x0000001E, 0x0003005B,
    (u32)btlevtcmd_GetConfuseActEvent, 0xFFFFFFFE, 0xFE363C80, 0x0001005E, 0xFE363C80, 0x00000002,
    0x00000001,
};

u32 btldefaultevt_CantMoveZeroGravity[] = {
    0x0003005B, (u32)btlevtcmd_GetBodyId, 0xFFFFFFFE, 0xFE363C80, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType,
    0xFFFFFFFE, 0xFE363C80, 0x00000040, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000,
    0x00000007, 0x0004005B, (u32)evt_btl_camera_set_homing_unit, 0x00000000, 0xFFFFFFFE, 0xFFFFFFFF,
    0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001, 0x0003005B, (u32)evt_btl_camera_set_zoom,
    0x00000000, 0x000000C8, 0x0006005B, (u32)btlevtcmd_AnnounceMessage, 0x00000000, 0x00000000,
    0x00000000, (u32)(char*)str_btl_msg_cant_move_ze_802ee8e0, 0x00000078, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000,
    0x00000000, 0x00010009, 0x00000014, 0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE,
    0x00000002, 0x00000001,
};

u32 btldefaultevt_SuitoruBadgeEffect[] = {
    0x0003005B, (u32)_get_heart_suitoru_point, 0xFFFFFFFE, 0xFE363C85, 0x0002001D, 0xFE363C85,
    0x00000001, 0x0000006E, 0x0003005B, (u32)btlevtcmd_GetBodyId, 0xFFFFFFFE, 0xFE363C84,
    0x0003005B, (u32)btlevtcmd_GetStatusMg, 0xFFFFFFFE, 0xFE363C86, 0x0003005B, (u32)btlevtcmd_GetHeight,
    0xFFFFFFFE, 0xFE363C87, 0x0002003C, 0xFE363C87, 0xFE363C86, 0x0006005B,
    (u32)btlevtcmd_GetPartsPos, 0xFFFFFFFE, 0xFE363C84, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00020035, 0xFE363C81, 0xFE363C87, 0x000F005B, (u32)evt_eff, 0x00000000,
    (u32)(char*)str_recovery_802ee900, 0x00000000, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0xFE363C85,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00010009, 0x00000078, 0x0004005B, (u32)btlevtcmd_RecoverHp, 0xFFFFFFFE,
    0xFE363C84, 0xFE363C85, 0x00000070, 0x00010009, 0x0000007D, 0x00000021,
    0x0003005B, (u32)_get_flower_suitoru_point, 0xFFFFFFFE, 0xFE363C85, 0x0002001D, 0xFE363C85,
    0x00000001, 0x0000006E, 0x0003005B, (u32)btlevtcmd_GetStatusMg, 0xFFFFFFFE, 0xFE363C86,
    0x0003005B, (u32)btlevtcmd_GetHeight, 0xFFFFFFFE, 0xFE363C87, 0x0002003C, 0xFE363C87,
    0xFE363C86, 0x0003005B, (u32)btlevtcmd_GetBodyId, 0xFFFFFFFE, 0xFE363C84, 0x0006005B,
    (u32)btlevtcmd_GetPartsPos, 0xFFFFFFFE, 0xFE363C84, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00020035, 0xFE363C81, 0xFE363C87, 0x000F005B, (u32)evt_eff, 0x00000000,
    (u32)(char*)str_recovery_802ee900, 0x00000001, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0xFE363C85,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00010009, 0x00000078, 0x0004005B, (u32)btlevtcmd_RecoverFp, 0xFFFFFFFE,
    0xFE363C84, 0xFE363C85, 0x00000070, 0x00010009, 0x0000007D, 0x00000021,
    0x00000002, 0x00000001,
};

u32 btldefaultevt_avoid_counter_event[] = {
    0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFE363C8A, 0xFE363C8B, 0x00000031, 0x0001006F,
    0xFE363C80, 0x0005005B, (u32)evt_sub_intpl_init, 0x00000005, 0x00000000, 0x00000708,
    0x00000028, 0x00010005, 0x00000028, 0x0003005B, (u32)evt_sub_intpl_get_value_para, 0xFE363C81,
    0xFE363C82, 0x0005005B, (u32)btlevtcmd_SetRotate, 0xFE363C8A, 0xF1194D80, 0xFE363C81,
    0xF1194D80, 0x00010009, 0x00000001, 0x00000006, 0x00000070, 0x0002005B,
    (u32)btlevtcmd_WaitEventEnd, 0xFE363C80, 0x00000002, 0x00000001,
};

/* .sdata */
u32 btldefaultevt_ProtectPartner[] = {
    0x0003005B, (u32)btlevtcmd_GetPartnerId, 0xFFFFFFFE, 0xFE363C83, 0x0003005B, (u32)btlevtcmd_SetGuard,
    0xFFFFFFFE, 0xFE363C83, 0x00000002, 0x00000001,
};

u32 btldefaultevt_Dummy[] = {
    0x00000002, 0x00000001,
};

u32 btldefaultevt_Wait[] = {
    0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002, 0x00000001,
};

u32 btldefaultevt_LectureEnd[] = {
    0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFFFFFFFA, 0x00000046, 0x00000002, 0x00000001,
};

u32 btldefaultevt_Defend[] = {
    0x00020032, 0xFE363C8C, (u32)badgeWeapon_DefenceCommand, 0x0001005E, (u32)_Defence_core, 0x00000002,
    0x00000001,
};

u32 btldefaultevt_DefendParty[] = {
    0x00020032, 0xFE363C8C, (u32)badgeWeapon_DefenceCommandP, 0x0001005E, (u32)_Defence_core, 0x00000002,
    0x00000001,
};
