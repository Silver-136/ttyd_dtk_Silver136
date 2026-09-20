#include "unit/party/unit_party_nokotarou.h"
#include "effect/eff_nokotarou.h"

extern void* _battleWorkPointer;
extern void evtSetValue(void* evt, s32 arg, s32 value);
extern s32 evtGetValue(void* evt, s32 arg);
extern s32 BattleTransID(void* evt, s32 id);
extern void* BattleGetUnitPtr(void* battleWork, s32 unitId);
extern u32 btldefaultevt_Dummy[];
extern u32 btldefaultevt_avoid_counter_event[];
extern u32 subset_attack_ready_jump_event[];
extern const char str_c_pnk_80424390[];
extern s32 weaponGetACOutputParam();
extern s32 weaponGetPowerFromPartyAttackLv();

/* Forward declarations for recovered Nokotarou event bytecode. */
static u32 attack_audience_event[263];
static u32 battle_entry_event[59];
static u32 init_event[78];
static u32 party_win_reaction[16];
extern u32 change_party_spawn_init_event[];
extern u32 dmg_turn_event[];
extern u32 wakeup_event[];
extern u32 _change_koura_pose[];
extern u32 _change_koura_pose_fast[];
extern u32 _restore_koura_pose[];
extern u32 _koura_rotate_start[];
extern u32 _koura_rotate_stop[];
extern u32 partyNokotarouAttack_FirstAttack[];
extern u32 partyNokotarouAttack_NormalAttack[];
extern u32 partyNokotarouAttack_SyubibinKoura[];
extern u32 partyNokotarouAttack_KouraGuard[];
extern u32 partyNokotarouAttack_TsuranukiKoura[];
extern u32 nokotarou_appeal_event[];
extern u32 unk_evt_80393a14[];
extern s32 _attack_aud();
extern s32 _get_attack_aud_no();
extern s32 btldefaultevt_Confuse();
extern s32 btldefaultevt_SuitoruBadgeEffect();
extern s32 btlevtcmd_ACRGetResult();
extern s32 btlevtcmd_ACRStart();
extern s32 btlevtcmd_ACSuccessEffect();
extern s32 btlevtcmd_AcGetDifficulty();
extern s32 btlevtcmd_AcGetOutputParam();
extern s32 btlevtcmd_AcSetDifficulty();
extern s32 btlevtcmd_AcSetFlag();
extern s32 btlevtcmd_AcSetGaugeParam();
extern s32 btlevtcmd_AcSetParamAll();
extern s32 btlevtcmd_AddPartsRotate();
extern s32 btlevtcmd_AddRotate();
extern s32 btlevtcmd_AnimeChangePose();
extern s32 btlevtcmd_AnimeChangePoseType();
extern s32 btlevtcmd_AnimeSetPoseTable();
extern s32 btlevtcmd_AttackDeclare();
extern s32 btlevtcmd_AttackDeclareAll();
extern s32 btlevtcmd_AudienceAppeal();
extern s32 btlevtcmd_AudienceDeclareACResult();
extern s32 btlevtcmd_AudienceDeclareAcrobatResult();
extern s32 btlevtcmd_CalculateFaceDirection();
extern s32 btlevtcmd_ChangeFaceDirection();
extern s32 btlevtcmd_CheckDamage();
extern s32 btlevtcmd_CheckStatus();
extern s32 btlevtcmd_CommandCheckDamage();
extern s32 btlevtcmd_CommandGetWeaponActionLv();
extern s32 btlevtcmd_CommandGetWeaponAddress();
extern s32 btlevtcmd_CommandPayWeaponCost();
extern s32 btlevtcmd_CommandPreCheckCounter();
extern s32 btlevtcmd_CommandPreCheckDamage();
extern s32 btlevtcmd_DivePosition();
extern s32 btlevtcmd_FaceDirectionAdd();
extern s32 btlevtcmd_FaceDirectionSub();
extern s32 btlevtcmd_FallPosition();
extern s32 btlevtcmd_GetBodyId();
extern s32 btlevtcmd_GetFaceDirection();
extern s32 btlevtcmd_GetFirstAttackTarget();
extern s32 btlevtcmd_GetHitPos();
extern s32 btlevtcmd_GetHomePos();
extern s32 btlevtcmd_GetMoveFrame();
extern s32 btlevtcmd_GetOverTurnCount();
extern s32 btlevtcmd_GetPartsWork();
extern s32 btlevtcmd_GetPos();
extern s32 btlevtcmd_GetResultAC();
extern s32 btlevtcmd_GetResultPrizeLv();
extern s32 btlevtcmd_GetRotate();
extern s32 btlevtcmd_GetSelectEnemy();
extern s32 btlevtcmd_GetSelectNextEnemy();
extern s32 btlevtcmd_GetStageAudienceAreaSize();
extern s32 btlevtcmd_GetStageSize();
extern s32 btlevtcmd_GetUnitId();
extern s32 btlevtcmd_GetUnitWork();
extern s32 btlevtcmd_InviteApInfoReport();
extern s32 btlevtcmd_JumpPosition();
extern s32 btlevtcmd_JumpSetting();
extern s32 btlevtcmd_KillUnit();
extern s32 btlevtcmd_MovePosition();
extern s32 btlevtcmd_OffAttribute();
extern s32 btlevtcmd_OffPartsAttribute();
extern s32 btlevtcmd_OffStatusFlag();
extern s32 btlevtcmd_OnAttribute();
extern s32 btlevtcmd_OnPartsAttribute();
extern s32 btlevtcmd_OnStatusFlag();
extern s32 btlevtcmd_ResetFaceDirection();
extern s32 btlevtcmd_ResultAC();
extern s32 btlevtcmd_ResultACDefence();
extern s32 btlevtcmd_RunDataEventChild();
extern s32 btlevtcmd_SetEventAttack();
extern s32 btlevtcmd_SetEventConfusion();
extern s32 btlevtcmd_SetEventDamage();
extern s32 btlevtcmd_SetEventEntry();
extern s32 btlevtcmd_SetEventWait();
extern s32 btlevtcmd_SetFallAccel();
extern s32 btlevtcmd_SetHomePos();
extern s32 btlevtcmd_SetHp();
extern s32 btlevtcmd_SetJumpSound();
extern s32 btlevtcmd_SetMoveSoundDataWork();
extern s32 btlevtcmd_SetMoveSpeed();
extern s32 btlevtcmd_SetOverTurnCount();
extern s32 btlevtcmd_SetPartsDefenceTable();
extern s32 btlevtcmd_SetPartsRotateOffset();
extern s32 btlevtcmd_SetPartsWork();
extern s32 btlevtcmd_SetPos();
extern s32 btlevtcmd_SetRotate();
extern s32 btlevtcmd_SetRotateOffset();
extern s32 btlevtcmd_SetRunSound();
extern s32 btlevtcmd_SetUnitWork();
extern s32 btlevtcmd_SetWalkSound();
extern s32 btlevtcmd_SetupAC();
extern s32 btlevtcmd_SpawnUnit();
extern s32 btlevtcmd_StartAC();
extern s32 btlevtcmd_StartAvoid();
extern s32 btlevtcmd_StartWaitEvent();
extern s32 btlevtcmd_StopAC();
extern s32 btlevtcmd_TransStageFloorPosition();
extern s32 btlevtcmd_WaitEventEnd();
extern s32 btlevtcmd_WaitGuardMove();
extern s32 btlevtcmd_WeaponAftereffect();
extern s32 btlevtcmd_snd_se();
extern s32 evt_audience_acrobat_notry();
extern s32 evt_audience_ap_recovery();
extern s32 evt_btl_camera_off_posoffset_manual();
extern s32 evt_btl_camera_set_homing_unit();
extern s32 evt_btl_camera_set_homing_unit_audience();
extern s32 evt_btl_camera_set_mode();
extern s32 evt_btl_camera_set_moveSpeedLv();
extern s32 evt_btl_camera_set_posoffset();
extern s32 evt_btl_camera_set_zoom();
extern s32 evt_btl_camera_shake_h();
extern s32 evt_eff();
extern s32 evt_snd_sfx_pit();
extern s32 evt_snd_sfxoff();
extern s32 evt_snd_sfxon();
extern s32 evt_snd_sfxon_3d();
extern s32 evt_sub_intpl_get_value_para();
extern s32 evt_sub_intpl_init();
extern u32 partyWeapon_NokotarouFirstAttack[];
extern u32 pose_table_nokotarou_stay[];
extern u32 pose_table_nokotarou_turn[];
extern const char str_M_B_1_804243a0[];
extern const char str_M_N_7_80424398[];


extern s32 btldefaultevt_Damage();
extern s32 btlevtcmd_AnimeChangePoseFromTable();
extern s32 unit_koura();

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void __makeTechMenuFunc(void* commandWork, s32* count) {
    typedef struct TechMenuEntry {
        char** name;
        s32 value;
        char* message;
        u16 icon;
        u16 pad_e;
        s32 field_10;
        s32 field_14;
        s32 pad_18;
    } TechMenuEntry;
    typedef struct TechMenuWork {
        u8 pad_0[0x80];
        TechMenuEntry entries[1];
    } TechMenuWork;
    extern void* BattleGetPartyPtr(void* battleWork);
    extern s32 BattleTransPartyId(s32 id);
    extern s32 partyGetTechLv(s32 partyId);
    extern char* msgSearch(char* msg);
    extern u8 lbl_8038ED18[];
    s32 techLv;
    void* party;
    u8* data;
    TechMenuWork* work;

    data = lbl_8038ED18;
    work = commandWork;
    party = BattleGetPartyPtr(_battleWorkPointer);
    techLv = partyGetTechLv(BattleTransPartyId(*(s32*)((s32)party + 8)));

    work->entries[*count].field_10 = -1;
    work->entries[*count].field_14 = 0;
    work->entries[*count].name = (char**)(data + 0x844);
    work->entries[*count].value = 0;
    work->entries[*count].icon = 0x158;
    work->entries[*count].message = msgSearch(*work->entries[*count].name);
    *count = *count + 1;

    if (techLv >= 0) {
        work->entries[*count].field_10 = -1;
        work->entries[*count].field_14 = 0;
        work->entries[*count].name = (char**)(data + 0x904);
        work->entries[*count].value = 0;
        work->entries[*count].icon = 0x159;
        work->entries[*count].message = msgSearch(*work->entries[*count].name);
        *count = *count + 1;
    }
    if (techLv >= 1) {
        work->entries[*count].field_10 = -1;
        work->entries[*count].field_14 = 0;
        work->entries[*count].name = (char**)(data + 0x9C4);
        work->entries[*count].value = 0;
        work->entries[*count].icon = *(u16*)(work->entries[*count].name + 1);
        work->entries[*count].message = msgSearch(*work->entries[*count].name);
        *count = *count + 1;
    }
    if (techLv >= 2) {
        work->entries[*count].field_10 = -1;
        work->entries[*count].field_14 = 0;
        work->entries[*count].name = (char**)(data + 0xA84);
        work->entries[*count].value = 0;
        work->entries[*count].icon = *(u16*)(work->entries[*count].name + 1);
        work->entries[*count].message = msgSearch(*work->entries[*count].name);
        *count = *count + 1;
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 _check_mario_move_count(void* evt) {
    extern void* BattleGetMarioPtr(void* battleWork);

    s32* args = *(s32**)((s32)evt + 0x18);
    void* battleWork = _battleWorkPointer;
    s32 out = args[0];
    void* mario = BattleGetMarioPtr(battleWork);
    evtSetValue(evt, out, *(s8*)((s32)mario + 0x22));
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

s32 _color_lv_set(void* evt) {

    s32* args = *(s32**)((s32)evt + 0x18);
    void* battleWork = _battleWorkPointer;
    s32 id = evtGetValue(evt, args[0]);
    s32 mode = evtGetValue(evt, args[1]);
    s32 out = args[2];
    s32 value = evtGetValue(evt, out);
    void* unit = BattleGetUnitPtr(battleWork, BattleTransID(evt, id));

    if (mode != 0) {
        *(u8*)((s32)unit + 0x311) = value;
    } else {
        evtSetValue(evt, out, *(u8*)((s32)unit + 0x311));
    }
    return 2;
}
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 _tsuranuki_effect_control(void* evt, s32 isFirstCall) {
    extern void BtlUnit_GetPos(void* unit, f32* x, f32* y, f32* z);
    extern void* BtlUnit_GetPartsPtr(void* unit, s32 partsId);
    extern void effSoftDelete(void* eff);
    extern f32 float_2_804243a8;
    extern f32 float_0p006_804243ac;
    s32* args;
    void* battleWork;
    void* unit;
    void* parts;
    void* eff;
    s32 target;
    f32 z;
    f32 y;
    f32 x;

    args = *(s32**)((s32)evt + 0x18);
    battleWork = _battleWorkPointer;
    target = evtGetValue(evt, args[0]);
    unit = BattleGetUnitPtr(battleWork, BattleTransID(evt, target));
    BtlUnit_GetPos(unit, &x, &y, &z);
    parts = BtlUnit_GetPartsPtr(unit, 1);
    if (isFirstCall != 0) {
        *(void**)((s32)evt + 0x78) = effNokotarouEntry(
            0, 0x3E8, x, y, z, *(f32*)((s32)unit + 0x114));
        *(s32*)((s32)parts + 0x138) = 0;
        *(s32*)((s32)parts + 0x134) = 0;
        *(s32*)((s32)evt + 0x7C) = 0;
    }
    target = *(s32*)((s32)parts + 0x134) * 100;
    if (*(s32*)((s32)evt + 0x7C) != target) {
        if (*(s32*)((s32)evt + 0x7C) < target) {
            *(s32*)((s32)evt + 0x7C) += 10;
            if (*(s32*)((s32)evt + 0x7C) > target) {
                *(s32*)((s32)evt + 0x7C) = target;
            }
        } else {
            *(s32*)((s32)evt + 0x7C) -= 10;
            if (*(s32*)((s32)evt + 0x7C) < target) {
                *(s32*)((s32)evt + 0x7C) = target;
            }
        }
    }
    eff = *(void**)((s32)evt + 0x78);
    *(f32*)((s32)*(void**)((s32)eff + 0xC) + 0x10) = x;
    *(f32*)((s32)*(void**)((s32)eff + 0xC) + 0x14) = y;
    *(f32*)((s32)*(void**)((s32)eff + 0xC) + 0x18) = z;
    *(f32*)((s32)*(void**)((s32)eff + 0xC) + 0x1C) =
        (float_2_804243a8 +
         float_0p006_804243ac * (f32)*(s32*)((s32)evt + 0x7C)) *
        *(f32*)((s32)unit + 0x114);
    if (*(s32*)((s32)parts + 0x138) != 0) {
        *(s32*)((s32)parts + 0x138) = 0;
        effSoftDelete(eff);
        return 2;
    }
    return 0;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 _check_guard_koura(void* evt) {
    extern void* BtlUnit_GetGuardKouraPtr(void* unit);

    void* battleWork = _battleWorkPointer;
    s32* args = *(s32**)((s32)evt + 0x18);
    s32 id = evtGetValue(evt, args[0]);
    s32 out = args[1];
    void* unit = BattleGetUnitPtr(battleWork, BattleTransID(evt, id));
    void* guard;

    if (!(*(u32*)((s32)unit + 0x104) & 0x20)) {
        evtSetValue(evt, out, -1);
        return 2;
    }
    guard = BtlUnit_GetGuardKouraPtr(unit);
    if (guard == 0) {
        evtSetValue(evt, out, -1);
        return 2;
    }
    evtSetValue(evt, out, *(s32*)guard);
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

/* Recovered Nokotarou .rodata strings. */
const char str_PNK_D_5_802f8ca8[] = "PNK_D_5";
const char str_PNK_X_1_802f8cb0[] = "PNK_X_1";
const char str_PNK_D_2_802f8cb8[] = "PNK_D_2";
const char str_PNK_N_1_802f8cc0[] = "PNK_N_1";
const char str_PNK_D_3_802f8cc8[] = "PNK_D_3";
const char str_PNK_S_1_802f8cd0[] = "PNK_S_1";
const char str_PNK_G_2_802f8cd8[] = "PNK_G_2";
const char str_PNK_G_1_802f8ce0[] = "PNK_G_1";
const char str_PNK_D_1_802f8ce8[] = "PNK_D_1";
const char str_PNK_W_1_802f8cf0[] = "PNK_W_1";
const char str_PNK_R_1_802f8cf8[] = "PNK_R_1";
const char str_PNK_A_1_802f8d00[] = "PNK_A_1";
const char str_PNK_D_4_802f8d08[] = "PNK_D_4";
const char str_PNK_T_1_802f8d10[] = "PNK_T_1";
const char str_PNK_I_1_802f8d18[] = "PNK_I_1";
const char str_PNK_C_1_802f8d20[] = "PNK_C_1";
const char str_PNK_O_1_802f8d28[] = "PNK_O_1";
const char str_PNK_N_2_802f8d30[] = "PNK_N_2";
const char str_PNK_H_1_802f8d38[] = "PNK_H_1";
const char str_btl_un_nokotarou_802f8d40[] = "btl_un_nokotarou";
const char str_SFX_PARTY_DAMAGE1_802f8d54[] = "SFX_PARTY_DAMAGE1";
const char str_SFX_MARIO_DAMAGE_FIR_802f8d68[] = "SFX_MARIO_DAMAGE_FIRE1";
const char str_SFX_MARIO_DAMAGE_ICE_802f8d80[] = "SFX_MARIO_DAMAGE_ICE1";
const char str_SFX_MARIO_DAMAGE_BIR_802f8d98[] = "SFX_MARIO_DAMAGE_BIRIBIRI1";
const char str_SFX_PARTY_BATTLE_SUB_802f8db4[] = "SFX_PARTY_BATTLE_SUBMOVE1L";
const char str_SFX_PARTY_BATTLE_SUB_802f8dd0[] = "SFX_PARTY_BATTLE_SUBMOVE1R";
const char str_SFX_PARTY_BATTLE_MOV_802f8dec[] = "SFX_PARTY_BATTLE_MOVE1L";
const char str_SFX_PARTY_BATTLE_MOV_802f8e04[] = "SFX_PARTY_BATTLE_MOVE1R";
const char str_SFX_PARTY_BATTLE_JUM_802f8e1c[] = "SFX_PARTY_BATTLE_JUMP1";
const char str_SFX_PARTY_BATTLE_LAN_802f8e34[] = "SFX_PARTY_BATTLE_LANDING1";
const char str_SFX_CONDITION_INSIDE_802f8e50[] = "SFX_CONDITION_INSIDE";
const char str_btl_wn_pnk_normal_802f8e68[] = "btl_wn_pnk_normal";
const char str_msg_pnk_normal_attac_802f8e7c[] = "msg_pnk_normal_attack";
const char str_msg_ac_hammer_802f8e94[] = "msg_ac_hammer";
const char str_btl_wn_pnk_lv1_802f8ea4[] = "btl_wn_pnk_lv1";
const char str_msg_pnk_syubibin_kou_802f8eb4[] = "msg_pnk_syubibin_koura";
const char str_msg_ac_koura_shubibi_802f8ecc[] = "msg_ac_koura_shubibin";
const char str_btl_wn_pnk_lv2_802f8ee4[] = "btl_wn_pnk_lv2";
const char str_msg_pnk_koura_no_mam_802f8ef4[] = "msg_pnk_koura_no_mamori";
const char str_msg_ac_koura_no_mamo_802f8f0c[] = "msg_ac_koura_no_mamori";
const char str_btl_wn_pnk_lv3_tsura_802f8f24[] = "btl_wn_pnk_lv3_tsuranuki";
const char str_msg_pnk_koura_tsuran_802f8f40[] = "msg_pnk_koura_tsuranuki";
const char str_msg_ac_tsuranuki_kou_802f8f58[] = "msg_ac_tsuranuki_koura";
const char str_PNK_K_2_802f8f70[] = "PNK_K_2";
const char str_PNK_K_1_802f8f78[] = "PNK_K_1";
const char str_kemuri_test_802f8f80[] = "kemuri_test";
const char str_SFX_BTL_NOKO_ATTACK2_802f8f8c[] = "SFX_BTL_NOKO_ATTACK2";
const char str_SFX_BTL_NOKO_ATTACK1_802f8fa4[] = "SFX_BTL_NOKO_ATTACK1";
const char str_PNK_E_2_802f8fbc[] = "PNK_E_2";
const char str_PNK_E_3_802f8fc4[] = "PNK_E_3";
const char str_SFX_BTL_NOKO_SHUBIBI_802f8fcc[] = "SFX_BTL_NOKO_SHUBIBIN1";
const char str_SFX_BTL_NOKO_SHUBIBI_802f8fe4[] = "SFX_BTL_NOKO_SHUBIBIN2";
const char str_PNK_E_1_802f8ffc[] = "PNK_E_1";
const char str_PNK_A_2_802f9004[] = "PNK_A_2";
const char str_SFX_BTL_NOKO_GUARD2_802f900c[] = "SFX_BTL_NOKO_GUARD2";
const char str_PKO_F_1_802f9020[] = "PKO_F_1";
const char str_SFX_BTL_NOKO_GUARD1_802f9028[] = "SFX_BTL_NOKO_GUARD1";
const char str_PKO_H_1_802f903c[] = "PKO_H_1";
const char str_PKO_H_2_802f9044[] = "PKO_H_2";
const char str_SFX_BTL_NOKO_IPPATSU_802f904c[] = "SFX_BTL_NOKO_IPPATSU1";
const char str_SFX_BTL_ACROBAT_SHAK_802f9064[] = "SFX_BTL_ACROBAT_SHAKE1";
const char str_SFX_BTL_NOKO_IPPATSU_802f907c[] = "SFX_BTL_NOKO_IPPATSU2";
const char str_SFX_MARIO_APPEAL1_802f9094[] = "SFX_MARIO_APPEAL1";
const char str_PNK_Y_1_802f90a8[] = "PNK_Y_1";

/* Recovered Nokotarou .sdata support tables and small event scripts. */
static u8 defence[5] = { 0x01, 0x01, 0x01, 0x01, 0x01 };

static u8 defence_attr[5] = { 0x00, 0x00, 0x00, 0x00, 0x00 };

u8 defence_turn[5] = { 0x00, 0x00, 0x00, 0x00, 0x00 };

static u8 regist[22] = { 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64 };

static u32 damage_event[10] = {
    0x00020032, 0xFE363C8A, 0xFFFFFFFE, 0x00020032, 0xFE363C8B, 0x00000001,
    0x0001005E, (u32)btldefaultevt_Damage, 0x00000002, 0x00000001,
};

static u32 attack_event[2] = {
    0x00000002, 0x00000001,
};

static u32 wait_event[6] = {
    0x0003005B, (u32)btlevtcmd_AnimeChangePoseFromTable, 0xFFFFFFFE, 0x00000001, 0x00000002, 0x00000001,
};

u32 entry_koura[12] = {
    (u32)unit_koura, 0x00000000, 0x04000001, 0x447A0000, 0x00000000, 0x447A0000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
};

/* Recovered Nokotarou pose/unit structured data. */
u32 pose_table_nokotarou_stay[52] = {
    0x00000009, (u32)(char*)str_PNK_D_5_802f8ca8, 0x00000002, (u32)(char*)str_PNK_D_5_802f8ca8, 0x00000003, (u32)(char*)str_PNK_X_1_802f8cb0,
    0x00000004, (u32)(char*)str_PNK_D_2_802f8cb8, 0x00000005, (u32)(char*)str_PNK_X_1_802f8cb0, 0x00000001, (u32)(char*)str_PNK_N_1_802f8cc0,
    0x0000001B, (u32)(char*)str_PNK_D_3_802f8cc8, 0x0000001C, (u32)(char*)str_PNK_S_1_802f8cd0, 0x00000025, (u32)(char*)str_PNK_G_2_802f8cd8,
    0x0000001F, (u32)(char*)str_PNK_G_1_802f8ce0, 0x00000027, (u32)(char*)str_PNK_D_1_802f8ce8, 0x00000028, (u32)(char*)str_PNK_W_1_802f8cf0,
    0x0000002A, (u32)(char*)str_PNK_R_1_802f8cf8, 0x00000032, (u32)(char*)str_PNK_A_1_802f8d00, 0x00000039, (u32)(char*)str_PNK_D_2_802f8cb8,
    0x00000038, (u32)(char*)str_PNK_D_2_802f8cb8, 0x0000003C, (u32)(char*)str_PNK_D_4_802f8d08, 0x0000003D, (u32)(char*)str_PNK_X_1_802f8cb0,
    0x00000041, (u32)(char*)str_PNK_T_1_802f8d10, 0x00000034, (u32)(char*)str_PNK_I_1_802f8d18, 0x00000043, (u32)(char*)str_PNK_C_1_802f8d20,
    0x00000042, (u32)(char*)str_PNK_O_1_802f8d28, 0x00000040, (u32)(char*)str_PNK_X_1_802f8cb0, 0x00000031, (u32)(char*)str_PNK_A_1_802f8d00,
    0x0000002D, (u32)(char*)str_PNK_G_1_802f8ce0, 0x00000045, (u32)(char*)str_PNK_S_1_802f8cd0,
};

u32 pose_table_nokotarou_turn[24] = {
    0x00000009, (u32)(char*)str_PNK_D_5_802f8ca8, 0x00000002, (u32)(char*)str_PNK_D_5_802f8ca8, 0x00000001, (u32)(char*)str_PNK_N_2_802f8d30,
    0x0000001B, (u32)(char*)str_PNK_D_3_802f8cc8, 0x0000001C, (u32)(char*)str_PNK_H_1_802f8d38, 0x0000001F, (u32)(char*)str_PNK_D_1_802f8ce8,
    0x00000027, (u32)(char*)str_PNK_D_1_802f8ce8, 0x00000039, (u32)(char*)str_PNK_H_1_802f8d38, 0x00000038, (u32)(char*)str_PNK_H_1_802f8d38,
    0x00000040, (u32)(char*)str_PNK_H_1_802f8d38, 0x0000002D, (u32)(char*)str_PNK_H_1_802f8d38, 0x00000045, (u32)(char*)str_PNK_H_1_802f8d38,
};

static u32 data_table[24] = {
    0x00000005, (u32)nokotarou_appeal_event, 0x00000006, (u32)attack_audience_event, 0x00000004, (u32)change_party_spawn_init_event,
    0x0000000D, (u32)dmg_turn_event, 0x00000001, (u32)__makeTechMenuFunc, 0x00000002, (u32)wakeup_event,
    0x00000030, (u32)btldefaultevt_Dummy, 0x00000040, (u32)party_win_reaction, 0x00000007, (u32)subset_attack_ready_jump_event,
    0x00000043, (u32)unk_evt_80393a14, 0x0000002F, (u32)btldefaultevt_avoid_counter_event, 0x00000000, 0x00000000,
};

u32 unitpartsdata_Party_Nokotarou[19] = {
    0x00000001, (u32)(char*)str_btl_un_nokotarou_802f8d40, (u32)(char*)str_c_pnk_80424390, 0x00000000, 0x00000000, 0x00000000,
    0x41400000, 0x41F00000, 0x40A00000, 0x00000000, 0x41F00000, 0x00000000,
    0x0014001E, 0x00FF0000, (u32)defence, (u32)defence_attr, 0x00001009, 0x00000000,
    (u32)pose_table_nokotarou_stay,
};

u32 unitdata_Party_Nokotarou[49] = {
    0x000000E1, (u32)(char*)str_btl_un_nokotarou_802f8d40, 0x000A0005, 0x05010100, 0x00000064, 0x00090014,
    0x0021000E, 0x00240000, 0x00000000, 0x41400000, 0x00000000, 0x00000000,
    0x00000000, 0x42200000, 0x00000000, 0x00000000, 0x42200000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x41200000,
    0x00000000, 0x00000000, 0x41200000, 0x41AB999A, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0000FF00, 0x64140000,
    0x00000000, 0x00000000, 0x00000000, (u32)(char*)str_SFX_PARTY_DAMAGE1_802f8d54, (u32)(char*)str_SFX_MARIO_DAMAGE_FIR_802f8d68, (u32)(char*)str_SFX_MARIO_DAMAGE_ICE_802f8d80,
    (u32)(char*)str_SFX_MARIO_DAMAGE_BIR_802f8d98, 0x02000000, (u32)regist, 0x01000000, (u32)unitpartsdata_Party_Nokotarou, (u32)init_event,
    (u32)data_table,
};

/*
 * Recovered Nokotarou battle event bytecode.
 * Exact US target words are preserved; relocation words remain symbolic.
 * Structured/unit/BattleWeapon data remains a separate later lane.
 */
static u32 battle_entry_event[59] = {
    0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE, 0xFE363C80, 0x000000FA, 0x0005005B,
    (u32)btlevtcmd_SetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00010009,
    0x00000028, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001, 0x0000002A,
    0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE, 0xF24A8A80, 0x0009005B, (u32)btlevtcmd_SetMoveSoundDataWork,
    0xFFFFFFFE, (u32)(char*)str_SFX_PARTY_BATTLE_SUB_802f8db4, (u32)(char*)str_SFX_PARTY_BATTLE_SUB_802f8dd0, 0x00000000, 0x00000009, 0x00000009,
    0x00000000, 0x00000000, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000000, 0xFFFFFFFE, 0x00000000, 0x0003005B,
    (u32)btlevtcmd_AnimeChangePoseFromTable, 0xFFFFFFFE, 0x00000001, 0x00000002, 0x00000001,
};

static u32 init_event[78] = {
    0x0003005B, (u32)btlevtcmd_SetEventWait, 0xFFFFFFFE, (u32)wait_event, 0x0003005B, (u32)btlevtcmd_SetEventAttack,
    0xFFFFFFFE, (u32)attack_event, 0x0003005B, (u32)btlevtcmd_SetEventDamage, 0xFFFFFFFE, (u32)damage_event,
    0x0003005B, (u32)btlevtcmd_SetEventConfusion, 0xFFFFFFFE, (u32)btldefaultevt_Confuse, 0x0004005B, (u32)btlevtcmd_SetUnitWork,
    0xFFFFFFFE, 0x00000000, 0x00000000, 0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE,
    0x00000001, 0x00000000, 0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000004,
    0x00000D52, 0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000005, 0x00000000,
    0x0005005B, (u32)btlevtcmd_SetPartsWork, 0xFFFFFFFE, 0x00000001, 0x00000000, 0x00000000,
    0x0005005B, (u32)btlevtcmd_SetPartsWork, 0xFFFFFFFE, 0x00000001, 0x00000002, 0x00000000,
    0x0007005B, (u32)btlevtcmd_SetWalkSound, 0xFFFFFFFE, (u32)(char*)str_SFX_PARTY_BATTLE_MOV_802f8dec, (u32)(char*)str_SFX_PARTY_BATTLE_MOV_802f8e04, 0x00000006,
    0x0000000C, 0x00000010, 0x0007005B, (u32)btlevtcmd_SetRunSound, 0xFFFFFFFE, (u32)(char*)str_SFX_PARTY_BATTLE_MOV_802f8dec,
    (u32)(char*)str_SFX_PARTY_BATTLE_MOV_802f8e04, 0x00000000, 0x00000009, 0x00000003, 0x0004005B, (u32)btlevtcmd_SetJumpSound,
    0xFFFFFFFE, (u32)(char*)str_SFX_PARTY_BATTLE_JUM_802f8e1c, (u32)(char*)str_SFX_PARTY_BATTLE_LAN_802f8e34, 0x0003005B, (u32)btlevtcmd_SetEventEntry, 0xFFFFFFFE,
    (u32)battle_entry_event, 0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002, 0x00000001,
};

u32 change_party_spawn_init_event[] = {
    0x0003005B, (u32)btlevtcmd_GetOverTurnCount, 0xFFFFFFFC, 0xFE363C80, 0x0002001D, 0xFE363C80,
    0x00000001, 0x0004005B, (u32)btlevtcmd_SetPartsDefenceTable, 0xFFFFFFFC, 0x00000001, (u32)defence_turn,
    0x0004005B, (u32)btlevtcmd_AnimeSetPoseTable, 0xFFFFFFFC, 0x00000001, (u32)pose_table_nokotarou_turn, 0x0003005B,
    (u32)btlevtcmd_OnStatusFlag, 0xFFFFFFFC, 0x00000001, 0x00000021, 0x0003005B, (u32)btlevtcmd_AnimeChangePoseFromTable,
    0xFFFFFFFC, 0x00000001, 0x00000002, 0x00000001,
};

u32 dmg_turn_event[] = {
    0x00020032, 0xFE363C8A, 0xFFFFFFFE, 0x00020032, 0xFE363C8B, 0x00000001,
    0x0004005B, (u32)btlevtcmd_SetPartsDefenceTable, 0xFE363C8A, 0xFE363C8B, (u32)defence_turn, 0x0004005B,
    (u32)btlevtcmd_AnimeSetPoseTable, 0xFE363C8A, 0xFE363C8B, (u32)pose_table_nokotarou_turn, 0x0003005B, (u32)btlevtcmd_OnStatusFlag,
    0xFE363C8A, 0x00000001, 0x0003005B, (u32)btlevtcmd_SetOverTurnCount, 0xFE363C8A, 0x00000002,
    0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFE363C8A, 0xFE363C8B, (u32)(char*)str_PNK_D_1_802f8ce8, 0x0003005B,
    (u32)btlevtcmd_OffStatusFlag, 0xFE363C8A, 0x01000000, 0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_CONDITION_INSIDE_802f8e50,
    0x00000000, 0x0003005B, (u32)btlevtcmd_SetFallAccel, 0xFFFFFFFE, 0xF24A7E80, 0x0005005B,
    (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0007005B,
    (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0000000C,
    0xFFFFFFFF, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000006, 0xFFFFFFFF, 0x00000002, 0x00000001,
};

u32 wakeup_event[] = {
    0x00020032, 0xFE363C8A, 0xFFFFFFFE, 0x00020032, 0xFE363C8B, 0x00000001,
    0x0003005B, (u32)btlevtcmd_GetOverTurnCount, 0xFE363C8A, 0xFE363C80, 0x00020036, 0xFE363C80,
    0x00000001, 0x0003005B, (u32)btlevtcmd_SetOverTurnCount, 0xFE363C8A, 0xFE363C80, 0x00020019,
    0xFE363C80, 0x00000000, 0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002,
    0x00000021, 0x0004005B, (u32)btlevtcmd_SetPartsDefenceTable, 0xFE363C8A, 0xFE363C8B, (u32)defence,
    0x0003005B, (u32)btlevtcmd_OffStatusFlag, 0xFE363C8A, 0x00000001, 0x0004005B, (u32)btlevtcmd_AnimeSetPoseTable,
    0xFE363C8A, 0xFE363C8B, (u32)pose_table_nokotarou_stay, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFE363C8A,
    0xFE363C8B, (u32)(char*)str_PNK_S_1_802f8cd0, 0x0003005B, (u32)btlevtcmd_SetFallAccel, 0xFFFFFFFE, 0xF24A7E80,
    0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0000000C, 0xFFFFFFFF, 0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002,
    0x00000001,
};

/* Recovered Nokotarou BattleWeapon records. */
u32 partyWeapon_NokotarouFirstAttack[48] = {
    0x00000000, 0x01580000, 0x00000000, 0x00000000, 0x64000000, 0x3F800000,
    0x01010101, (u32)weaponGetPowerFromPartyAttackLv, 0x00000002, 0x00000002, 0x00000003, 0x00000003,
    0x00000005, 0x00000005, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01101260, 0x21004004, 0x00080302, 0x00000000, 0x0002008F,
    0x000007FF, 0x80002004, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)partyNokotarouAttack_FirstAttack, 0x00000064, 0x00000000, 0x00000000,
};

u32 partyWeapon_NokotarouKouraAttack[48] = {
    (u32)(char*)str_btl_wn_pnk_normal_802f8e68, 0x01580000, 0x00000000, (u32)(char*)str_msg_pnk_normal_attac_802f8e7c, 0x64000000, 0x3F800000,
    0x01016432, (u32)weaponGetPowerFromPartyAttackLv, 0x00000001, 0x00000002, 0x00000002, 0x00000003,
    0x00000003, 0x00000005, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01001260, 0x21004004, 0x00080302, (u32)(char*)str_msg_ac_hammer_802f8e94, 0x00030087,
    0x000005FF, 0x80002004, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)partyNokotarouAttack_NormalAttack, 0x00000064, 0x00040200, 0x00000000,
};

u32 partyWeapon_NokotarouSyubibinKoura[48] = {
    (u32)(char*)str_btl_wn_pnk_lv1_802f8ea4, 0x01590000, 0x00000000, (u32)(char*)str_msg_pnk_syubibin_kou_802f8eb4, 0x64030000, 0x3F800000,
    0x01056432, (u32)weaponGetPowerFromPartyAttackLv, 0x00000001, 0x00000002, 0x00000002, 0x00000003,
    0x00000003, 0x00000005, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x02101260, 0x20004004, 0x00080302, (u32)(char*)str_msg_ac_koura_shubibi_802f8ecc, 0x00020087,
    0x000005FF, 0x80002004, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)partyNokotarouAttack_SyubibinKoura, 0x00000064, 0x000A0500, 0x00000000,
};

u32 partyWeapon_NokotarouKouraGuard[48] = {
    (u32)(char*)str_btl_wn_pnk_lv2_802f8ee4, 0x015A0000, 0x00000000, (u32)(char*)str_msg_pnk_koura_no_mam_802f8ef4, 0x64040000, 0x3F800000,
    0x01056432, 0x00000000, 0x00000000, 0x00000002, 0x00000002, 0x00000003,
    0x00000003, 0x00000004, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01110020, 0x10000000, 0x00000302, (u32)(char*)str_msg_ac_koura_no_mamo_802f8f0c, 0x00020000,
    0x000007FF, 0x80002004, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)partyNokotarouAttack_KouraGuard, 0x00000064, 0x00000000, 0x00000000,
};

u32 partyWeapon_NokotarouTsuranukiKoura[48] = {
    (u32)(char*)str_btl_wn_pnk_lv3_tsura_802f8f24, 0x015B0000, 0x00000000, (u32)(char*)str_msg_pnk_koura_tsuran_802f8f40, 0x64060000, 0x3F800000,
    0x01056432, (u32)weaponGetACOutputParam, 0x00000002, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x02101260, 0x20004004, 0x00000302, (u32)(char*)str_msg_ac_tsuranuki_kou_802f8f58, 0x000200C7,
    0x000005FF, 0x80002004, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)partyNokotarouAttack_TsuranukiKoura, 0x00000064, 0x000A0500, 0x00000000,
};

u32 _change_koura_pose[] = {
    0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PNK_K_2_802f8f70, 0x00010005,
    0x00000012, 0x00010009, 0x00000001, 0x0005005B, (u32)btlevtcmd_AddRotate, 0xFFFFFFFE,
    0x00000000, 0x0000000F, 0x00000000, 0x00000006, 0x0004005B, (u32)btlevtcmd_AnimeChangePose,
    0xFFFFFFFE, 0x00000001, (u32)(char*)str_PNK_K_1_802f8f78, 0x00010005, 0x00000006, 0x00010009,
    0x00000001, 0x0005005B, (u32)btlevtcmd_AddRotate, 0xFFFFFFFE, 0x00000000, 0x0000000F,
    0x00000000, 0x00000006, 0x00000002, 0x00000001,
};

u32 _change_koura_pose_fast[] = {
    0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x000F005B, (u32)evt_eff, 0x00000000, (u32)(char*)str_kemuri_test_802f8f80, 0x00000005, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0xF24A7E80, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0004005B, (u32)btlevtcmd_AnimeChangePose,
    0xFFFFFFFE, 0x00000001, (u32)(char*)str_PNK_K_2_802f8f70, 0x00010005, 0x0000000A, 0x00010009,
    0x00000001, 0x0005005B, (u32)btlevtcmd_AddRotate, 0xFFFFFFFE, 0x00000000, 0x0000001B,
    0x00000000, 0x00000006, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001,
    (u32)(char*)str_PNK_K_1_802f8f78, 0x00010005, 0x00000003, 0x00010009, 0x00000001, 0x0005005B,
    (u32)btlevtcmd_AddRotate, 0xFFFFFFFE, 0x00000000, 0x0000001E, 0x00000000, 0x00000006,
    0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x000F005B, (u32)evt_eff, 0x00000000, (u32)(char*)str_kemuri_test_802f8f80, 0x00000005, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0xF24A7E80, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000002, 0x00000001,
};

u32 _restore_koura_pose[] = {
    0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000000, 0x00000000, 0x0005005B,
    (u32)btlevtcmd_SetRotate, 0xFFFFFFFE, 0x00000000, 0x00000000, 0x00000000, 0x00010009,
    0x00000001, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PNK_K_1_802f8f78,
    0x00010005, 0x00000006, 0x00010009, 0x00000001, 0x0005005B, (u32)btlevtcmd_AddRotate,
    0xFFFFFFFE, 0x00000000, 0x0000002D, 0x00000000, 0x00000006, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PNK_K_2_802f8f70, 0x00010005, 0x00000006,
    0x00010009, 0x00000001, 0x0005005B, (u32)btlevtcmd_AddRotate, 0xFFFFFFFE, 0x00000000,
    0x0000000F, 0x00000000, 0x00000006, 0x00000002, 0x00000001,
};

u32 _koura_rotate_start[] = {
    0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000000, 0x00000001, 0x0004005B,
    (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000002, 0x00000000, 0x0000006B, 0x00010003,
    0x00000000, 0x0004005B, (u32)btlevtcmd_GetUnitWork, 0xFFFFFFFE, 0x00000000, 0xFE363C81,
    0x00020018, 0xFE363C81, 0x00000000, 0x00010004, 0x0000000A, 0x00000021,
    0x0004005B, (u32)btlevtcmd_GetUnitWork, 0xFFFFFFFE, 0x00000002, 0xFE363C8B, 0x00020019,
    0xFE363C8B, 0x00000000, 0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000002,
    0x00000000, 0x0004005B, (u32)btlevtcmd_GetUnitWork, 0xFFFFFFFE, 0x00000001, 0xFE363C8A,
    0x00020037, 0xFE363C8A, 0xFFFFFFFF, 0x00020037, 0xFE363C80, 0xFFFFFFFF,
    0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000001, 0xFE363C8A, 0x00000021,
    0x0004005B, (u32)btlevtcmd_GetUnitWork, 0xFFFFFFFE, 0x00000001, 0xFE363C8A, 0x0002001D,
    0xFE363C8A, 0x00000000, 0x00020035, 0xFE363C80, 0x00000001, 0x0002001B,
    0xFE363C80, 0xFE363C8A, 0x00020032, 0xFE363C80, 0xFE363C8A, 0x00000021,
    0x0005005B, (u32)btlevtcmd_AddRotate, 0xFFFFFFFE, 0x00000000, 0xFE363C80, 0x00000000,
    0x0005005B, (u32)btlevtcmd_GetRotate, 0xFFFFFFFE, 0xF1194D80, 0xFE363C81, 0xF1194D80,
    0x0002001A, 0xFE363C81, 0xFE363C80, 0x0005005B, (u32)btlevtcmd_GetPartsWork, 0xFFFFFFFE,
    0x00000001, 0x00000000, 0xFE363C81, 0x00020019, 0xFE363C81, 0x00000000,
    0x0000006E, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x000F005B, (u32)evt_eff, 0x00000000, (u32)(char*)str_kemuri_test_802f8f80, 0x00000005,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0xF24A7E80, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000070,
    0x00000021, 0x00000021, 0x00000020, 0x00020036, 0xFE363C80, 0x00000001,
    0x0002001A, 0xFE363C80, 0xFE363C8A, 0x00020032, 0xFE363C80, 0xFE363C8A,
    0x00000021, 0x0005005B, (u32)btlevtcmd_AddRotate, 0xFFFFFFFE, 0x00000000, 0xFE363C80,
    0x00000000, 0x0005005B, (u32)btlevtcmd_GetPartsWork, 0xFFFFFFFE, 0x00000001, 0x00000000,
    0xFE363C81, 0x00020019, 0xFE363C81, 0x00000000, 0x0000006E, 0x00020037,
    0xFE363C80, 0xFFFFFFFF, 0x0005005B, (u32)btlevtcmd_GetRotate, 0xFFFFFFFE, 0xF1194D80,
    0xFE363C81, 0xF1194D80, 0x00020035, 0xFE363C81, 0x00000168, 0x0002001A,
    0xFE363C81, 0xFE363C80, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x000F005B, (u32)evt_eff, 0x00000000, (u32)(char*)str_kemuri_test_802f8f80,
    0x00000005, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0xF24A7E80, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000021, 0x00000070, 0x00000021, 0x00000021, 0x00010009, 0x00000001,
    0x00010004, 0x00000000, 0x00010003, 0x0000000A, 0x0000006D, 0x00000002,
    0x00000001,
};

u32 _koura_rotate_stop[] = {
    0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000000, 0x00000000, 0x0005005B,
    (u32)btlevtcmd_SetRotate, 0xFFFFFFFE, 0x00000000, 0x00000000, 0x00000000, 0x00000002,
    0x00000001,
};

u32 partyNokotarouAttack_FirstAttack[] = {
    0x0005005B, (u32)btlevtcmd_JumpSetting, 0xFFFFFFFE, 0x00000014, 0xF24A7A80, 0xF24A7D4C,
    0x0003005B, (u32)btlevtcmd_GetFirstAttackTarget, 0xFE363C83, 0xFE363C84, 0x00020018, 0xFE363C83,
    0xFFFFFFFF, 0x00010004, 0x00000063, 0x00000021, 0x0002005B, (u32)btlevtcmd_WeaponAftereffect,
    (u32)partyWeapon_NokotarouFirstAttack, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000007, 0x0004005B,
    (u32)evt_btl_camera_set_homing_unit, 0x00000000, 0xFE363C83, 0xFFFFFFFF, 0x0003005B, (u32)evt_btl_camera_set_zoom,
    0x00000000, 0x0000012C, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000002,
    0x00020032, 0xFE363C80, 0x0000002D, 0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE,
    0x00000001, 0x0000002D, 0x0001005E, (u32)_koura_rotate_start, 0x0005005B, (u32)btlevtcmd_GetHomePos,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub,
    0xFFFFFFFE, 0xFE363C80, 0x000000FA, 0x0005005B, (u32)btlevtcmd_SetPos, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00010009, 0x00000032, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001, 0x00000032, 0x0005005B, (u32)btlevtcmd_GetPos,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0006005B, (u32)btlevtcmd_GetHitPos,
    0xFE363C83, 0xFE363C84, 0xFE363C80, 0xFE363C85, 0xFE363C82, 0x0003005B,
    (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE, 0xF24A9280, 0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_BTL_NOKO_ATTACK2_802f8f8c,
    0x00000000, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000000, 0x00000000, 0x00000000, 0x0001005E, (u32)_koura_rotate_stop,
    0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000, 0x0003005B, (u32)btlevtcmd_ResultACDefence,
    0xFE363C83, (u32)partyWeapon_NokotarouFirstAttack, 0x0007005B, (u32)btlevtcmd_CheckDamage, 0xFFFFFFFE, 0xFE363C83,
    0xFE363C84, (u32)partyWeapon_NokotarouFirstAttack, 0x00020100, 0xFE363C85, 0x00020032, 0xFE363C80,
    0x0000002D, 0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000001, 0x0000002D,
    0x0001005E, (u32)_koura_rotate_start, 0x0003005B, (u32)btlevtcmd_SetFallAccel, 0xFFFFFFFE, 0xF24A7D4C,
    0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00020032, 0xFE363C81, 0x00000000, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE,
    0xFE363C80, 0x0000001E, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000012, 0xFFFFFFFF, 0x0003005B, (u32)btlevtcmd_SetFallAccel,
    0xFFFFFFFE, 0xF24A7CE6, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE, 0xFE363C80,
    0x00000014, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0000000C, 0xFFFFFFFF, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE,
    0xFE363C80, 0x00000008, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000008, 0xFFFFFFFF, 0x0001005E, (u32)_koura_rotate_stop,
    0x0001005E, (u32)_restore_koura_pose, 0x0001005B, (u32)evt_audience_ap_recovery, 0x0001005B, (u32)btlevtcmd_InviteApInfoReport,
    0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv,
    0x00000000, 0x00000001, 0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE, 0xF24A9280,
    0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001, 0x00000028, 0x0005005B,
    (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0008005B,
    (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000,
    0xFFFFFFFF, 0x00000000, 0x00010003, 0x00000063, 0x0002005B, (u32)btlevtcmd_ResetFaceDirection,
    0xFFFFFFFE, 0x0001005E, (u32)btldefaultevt_SuitoruBadgeEffect, 0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE,
    0x00000002, 0x00000001,
};

u32 partyNokotarouAttack_NormalAttack[] = {
    0x0003005B, (u32)btlevtcmd_GetSelectEnemy, 0xFE363C83, 0xFE363C84, 0x00020018, 0xFE363C83,
    0xFFFFFFFF, 0x00010004, 0x00000063, 0x00000021, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress,
    0xFFFFFFFE, 0xFE363C8C, 0x0002005B, (u32)btlevtcmd_WeaponAftereffect, 0xFE363C8C, 0x0004005B,
    (u32)btlevtcmd_AttackDeclare, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0x0001005B, (u32)btlevtcmd_WaitGuardMove,
    0x0002005B, (u32)btlevtcmd_CommandPayWeaponCost, 0xFFFFFFFE, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFFFFFFFE,
    0x00000007, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000007, 0x0004005B,
    (u32)evt_btl_camera_set_homing_unit, 0x00000000, 0xFFFFFFFE, 0x00000001, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv,
    0x00000000, 0x00000002, 0x0003005B, (u32)evt_btl_camera_set_zoom, 0x00000000, 0x000000C8,
    0x0005005B, (u32)evt_btl_camera_set_posoffset, 0x00000000, 0x00000014, 0x00000000, 0x00000000,
    0x00010009, 0x00000014, 0x0007005B, (u32)btlevtcmd_CalculateFaceDirection, 0xFFFFFFFE, 0xFFFFFFFF,
    0xFE363C83, 0xFE363C84, 0x00000010, 0xFE363C8F, 0x0003005B, (u32)btlevtcmd_ChangeFaceDirection,
    0xFFFFFFFE, 0xFE363C8F, 0x0001005E, (u32)_change_koura_pose, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType,
    0xFFFFFFFE, 0x00000001, 0x00000032, 0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_BTL_NOKO_ATTACK1_802f8fa4,
    0xFE363C8D, 0x00020032, 0xFE363C80, 0x0000000F, 0x0004005B, (u32)btlevtcmd_SetUnitWork,
    0xFFFFFFFE, 0x00000001, 0x0000002D, 0x0001005E, (u32)_koura_rotate_start, 0x0002005B,
    (u32)btlevtcmd_CommandGetWeaponActionLv, 0xFE363C80, 0x0003005B, (u32)btlevtcmd_AcSetDifficulty, 0xFFFFFFFE, 0xFE363C80,
    0x0002005B, (u32)btlevtcmd_AcGetDifficulty, 0xFE363C80, 0x00020036, 0xFE363C80, 0x00000003,
    0x00010022, 0xFE363C80, 0x00010028, 0xFFFFFFFD, 0x00020032, 0xFE363C80,
    0x00000168, 0x00020032, 0xFE363C81, 0x00000064, 0x00010024, 0xFFFFFFFE,
    0x00020032, 0xFE363C80, 0x00000118, 0x00020032, 0xFE363C81, 0x00000096,
    0x00010024, 0xFFFFFFFF, 0x00020032, 0xFE363C80, 0x000000F0, 0x00020032,
    0xFE363C81, 0x000000C8, 0x00010024, 0x00000000, 0x00020032, 0xFE363C80,
    0x000000C8, 0x00020032, 0xFE363C81, 0x000000FA, 0x00010024, 0x00000001,
    0x00020032, 0xFE363C80, 0x000000C8, 0x00020032, 0xFE363C81, 0x00000168,
    0x00010024, 0x00000002, 0x00020032, 0xFE363C80, 0x000000C8, 0x00020032,
    0xFE363C81, 0x00000258, 0x0000002A, 0x00020032, 0xFE363C80, 0x000000C8,
    0x00020032, 0xFE363C81, 0x000003C0, 0x00000031, 0x0009005B, (u32)btlevtcmd_AcSetParamAll,
    0x00000002, 0xFE363C80, 0x00000000, 0x00000001, 0xFE363C81, 0xF1194D80,
    0xF1194D80, 0xF1194D80, 0x0002005B, (u32)btlevtcmd_AcSetFlag, 0x00000003, 0x0005005B,
    (u32)btlevtcmd_SetupAC, 0xFFFFFFFE, 0x00000013, 0x00000001, 0x0000003C, 0x00010009,
    0x00000016, 0x0001006F, 0xFE363C8F, 0x00010005, 0x00002710, 0x0003005B,
    (u32)btlevtcmd_AcGetOutputParam, 0x00000000, 0xFE363C80, 0x00020019, 0xFE363C80, 0x00000000,
    0x00000007, 0x00000021, 0x00010009, 0x00000001, 0x00000006, 0x00020032,
    0xFE363C86, 0x00000000, 0x00010005, 0x00002710, 0x00020035, 0xFE363C86,
    0x00000001, 0x0002001B, 0xFE363C86, 0x00000064, 0x00020032, 0xFE363C86,
    0x00000064, 0x00000021, 0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000005,
    0xFE363C86, 0x0004005B, (u32)btlevtcmd_GetUnitWork, 0xFFFFFFFE, 0x00000004, 0xFE363C80,
    0x0004005B, (u32)btlevtcmd_GetUnitWork, 0xFFFFFFFE, 0x00000005, 0xFE363C81, 0x00020037,
    0xFE363C80, 0xFE363C81, 0x00020038, 0xFE363C80, 0x00000064, 0x0003005B,
    (u32)evt_snd_sfx_pit, 0xFE363C8D, 0xFE363C80, 0x00010009, 0x00000001, 0x00000006,
    0x00000070, 0x0006005B, (u32)btlevtcmd_CommandPreCheckDamage, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84,
    0x00000100, 0xFE363C86, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000004,
    0x0003005B, (u32)evt_btl_camera_set_zoom, 0x00000000, 0x00000190, 0x0002005B, (u32)btlevtcmd_StartAC,
    0x00000001, 0x0001005B, (u32)btlevtcmd_ResultAC, 0x0001005F, 0xFE363C8F, 0x0004005B,
    (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000005, 0x00000000, 0x0002005B, (u32)evt_snd_sfxoff,
    0xFE363C8D, 0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_BTL_NOKO_ATTACK2_802f8f8c, 0x00000000, 0x0003005B,
    (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000,
    0x00000002, 0x0005005B, (u32)evt_btl_camera_set_posoffset, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001, 0x00000032,
    0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE, 0xF24AA280, 0x0005005B, (u32)btlevtcmd_GetPos,
    0xFE363C83, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0006005B, (u32)btlevtcmd_GetHitPos,
    0xFE363C83, 0xFE363C84, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00020035,
    0xFE363C82, 0x00000005, 0x0004005B, (u32)btlevtcmd_TransStageFloorPosition, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0007005B, (u32)btlevtcmd_GetMoveFrame, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000000, 0xFE363C8D, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0xFE363C8D, 0x00000000, 0x00000000,
    0x00020019, 0xFE363C86, 0x00000001, 0x00020018, 0xFE363C86, 0x00000003,
    0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000026, 0x00000021, 0x00020018,
    0xFE363C86, 0x00000006, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000027,
    0x00000021, 0x00020018, 0xFE363C86, 0x00000002, 0x0003005B, (u32)btlevtcmd_StartAvoid,
    0xFE363C83, 0x00000028, 0x00000021, 0x0001005B, (u32)btlevtcmd_StopAC, 0x0004005B,
    (u32)btlevtcmd_FaceDirectionAdd, 0xFFFFFFFE, 0xFE363C80, 0x00000028, 0x0008005B, (u32)btlevtcmd_MovePosition,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000, 0xFFFFFFFF,
    0x00000000, 0x0001005E, (u32)_koura_rotate_stop, 0x0001005E, (u32)_restore_koura_pose, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001, 0x0000002B, 0x00010009, 0x0000001E,
    0x0001005B, (u32)evt_audience_ap_recovery, 0x0001005B, (u32)btlevtcmd_InviteApInfoReport, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType,
    0xFFFFFFFE, 0x00000001, 0x0000002A, 0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE,
    0xF24A9280, 0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000000, 0xFFFFFFFF, 0x00000000, 0x00010004, 0x00000063,
    0x00000021, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000, 0x0006005B,
    (u32)btlevtcmd_CommandPreCheckCounter, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0x00000100, 0xFE363C80,
    0x00020019, 0xFE363C80, 0x00000000, 0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE,
    0x00000001, 0x00000000, 0x0005005B, (u32)btlevtcmd_SetRotate, 0xFFFFFFFE, 0x00000000,
    0x00000000, 0x00000000, 0x0001005E, (u32)_koura_rotate_stop, 0x00000021, 0x0002005B,
    (u32)btlevtcmd_GetResultAC, 0xFE363C80, 0x0002001E, 0xFE363C80, 0x00000002, 0x0006005B,
    (u32)btlevtcmd_CommandCheckDamage, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0x00020100, 0xFE363C85,
    0x0004005B, (u32)btlevtcmd_GetResultPrizeLv, 0xFE363C83, 0x00000000, 0xFE363C86, 0x0006005B,
    (u32)btlevtcmd_GetHitPos, 0xFE363C83, 0xFE363C84, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0005005B, (u32)btlevtcmd_ACSuccessEffect, 0xFE363C86, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress, 0xFFFFFFFE, 0xFE363C80, 0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult,
    0xFE363C80, 0xFE363C86, 0x00000020, 0x0006005B, (u32)btlevtcmd_CommandCheckDamage, 0xFFFFFFFE,
    0xFE363C83, 0xFE363C84, 0x00000100, 0xFE363C85, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress,
    0xFFFFFFFE, 0xFE363C80, 0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult, 0xFE363C80, 0xFFFFFFFF,
    0x00000021, 0x00010003, 0x0000005A, 0x0001005B, (u32)btlevtcmd_StopAC, 0x0003005B,
    (u32)btlevtcmd_SetFallAccel, 0xFFFFFFFE, 0xF24A7D4C, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B, (u32)btlevtcmd_TransStageFloorPosition, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE, 0xFE363C80,
    0x0000001E, 0x0000006E, 0x0006005B, (u32)btlevtcmd_ACRStart, 0xFFFFFFFE, 0x00000000,
    0x00000011, 0x00000011, 0x00000000, 0x00000070, 0x0007005B, (u32)btlevtcmd_JumpPosition,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000012, 0xFFFFFFFF,
    0x0003005B, (u32)btlevtcmd_ACRGetResult, 0xFE363C86, 0xFE363C87, 0x00020018, 0xFE363C86,
    0x00000002, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress, 0xFFFFFFFE, 0xFE363C80, 0x0006005B,
    (u32)btlevtcmd_AudienceDeclareAcrobatResult, 0xFE363C80, 0x00000001, 0x00000000, 0x00000000, 0x00000000,
    0x0005005B, (u32)btlevtcmd_SetRotateOffset, 0xFFFFFFFE, 0x00000000, 0x0000000A, 0x00000000,
    0x00010005, 0x00000009, 0x0005005B, (u32)btlevtcmd_AddRotate, 0xFFFFFFFE, 0x00000000,
    0x00000000, 0x00000014, 0x00010009, 0x00000001, 0x00000006, 0x0003005B,
    (u32)btlevtcmd_SetFallAccel, 0xFFFFFFFE, 0xF24A7D4C, 0x0005005B, (u32)btlevtcmd_SetRotate, 0xFFFFFFFE,
    0x00000000, 0x000000B4, 0x00000000, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE,
    0x00000001, (u32)(char*)str_PNK_E_2_802f8fbc, 0x0000006E, 0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE,
    0x00000001, 0x0000002D, 0x00010003, 0x0000005B, 0x0004005B, (u32)btlevtcmd_GetUnitWork,
    0xFFFFFFFE, 0x00000001, 0xFE363C80, 0x0002001C, 0xFE363C80, 0x00000000,
    0x00010004, 0x0000005C, 0x00000021, 0x00020036, 0xFE363C80, 0x00000001,
    0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000001, 0xFE363C80, 0x00010009,
    0x00000002, 0x00010004, 0x0000005B, 0x00010003, 0x0000005C, 0x00000070,
    0x0006005B, (u32)btlevtcmd_ACRStart, 0xFFFFFFFE, 0x0000003C, 0x0000005A, 0x0000005A,
    0x00000000, 0x0003005B, (u32)btlevtcmd_ACRGetResult, 0xFE363C86, 0xFE363C87, 0x00020018,
    0xFE363C86, 0x00000001, 0x00010004, 0x0000005E, 0x00000021, 0x00020019,
    0xFE363C86, 0x00000002, 0x00010004, 0x0000005E, 0x00000021, 0x0001005E,
    (u32)_koura_rotate_stop, 0x00010005, 0x00000006, 0x00010009, 0x00000001, 0x0005005B,
    (u32)btlevtcmd_AddRotate, 0xFFFFFFFE, 0x00000000, 0x0000002D, 0x00000000, 0x00000006,
    0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PNK_E_3_802f8fc4, 0x00010005,
    0x00000006, 0x00010009, 0x00000001, 0x0005005B, (u32)btlevtcmd_AddRotate, 0xFFFFFFFE,
    0x00000000, 0x0000000F, 0x00000000, 0x00000006, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress,
    0xFFFFFFFE, 0xFE363C80, 0x0006005B, (u32)btlevtcmd_AudienceDeclareAcrobatResult, 0xFE363C80, 0x00000001,
    0x00000000, 0x00000000, 0x00000000, 0x00010009, 0x0000003C, 0x00010004,
    0x0000005F, 0x00010003, 0x0000005E, 0x0001005E, (u32)_koura_rotate_stop, 0x0001005B,
    (u32)evt_audience_acrobat_notry, 0x0001005E, (u32)_restore_koura_pose, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE,
    0x00000001, 0x0000002B, 0x00010009, 0x0000001E, 0x00010004, 0x0000005F,
    0x00000021, 0x0003005B, (u32)btlevtcmd_SetFallAccel, 0xFFFFFFFE, 0xF24A7CE6, 0x0004005B,
    (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE, 0xFE363C80, 0x00000014, 0x0007005B, (u32)btlevtcmd_JumpPosition,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0000000C, 0xFFFFFFFF,
    0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE, 0xFE363C80, 0x00000008, 0x0007005B,
    (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000008,
    0xFFFFFFFF, 0x0001005E, (u32)_restore_koura_pose, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE,
    0x00000001, 0x0000002B, 0x00010009, 0x0000001E, 0x00010003, 0x0000005F,
    0x0001005B, (u32)evt_audience_ap_recovery, 0x0001005B, (u32)btlevtcmd_InviteApInfoReport, 0x0003005B, (u32)evt_btl_camera_set_mode,
    0x00000000, 0x00000000, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001,
    0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001, 0x0000002A, 0x0003005B,
    (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE, 0xF24A9280, 0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000, 0xFFFFFFFF, 0x00000000,
    0x00010003, 0x00000063, 0x0002005B, (u32)btlevtcmd_ResetFaceDirection, 0xFFFFFFFE, 0x0001005E,
    (u32)btldefaultevt_SuitoruBadgeEffect, 0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002, 0x00000001,
};

u32 partyNokotarouAttack_SyubibinKoura[] = {
    0x0003005B, (u32)btlevtcmd_GetSelectEnemy, 0xFE363C83, 0xFE363C84, 0x00020018, 0xFE363C83,
    0xFFFFFFFF, 0x00010004, 0x00000063, 0x00000021, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress,
    0xFFFFFFFE, 0xFE363C8C, 0x0002005B, (u32)btlevtcmd_WeaponAftereffect, 0xFE363C8C, 0x0002005B,
    (u32)btlevtcmd_AttackDeclareAll, 0xFFFFFFFE, 0x0001005B, (u32)btlevtcmd_WaitGuardMove, 0x0002005B, (u32)btlevtcmd_CommandPayWeaponCost,
    0xFFFFFFFE, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFFFFFFFE, 0x00000007, 0x0003005B,
    (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000007, 0x0004005B, (u32)evt_btl_camera_set_homing_unit, 0x00000000,
    0xFFFFFFFE, 0x00000001, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000002,
    0x0003005B, (u32)evt_btl_camera_set_zoom, 0x00000000, 0x000000C8, 0x0005005B, (u32)evt_btl_camera_set_posoffset,
    0x00000000, 0x00000014, 0x00000000, 0x00000000, 0x00010009, 0x00000014,
    0x0001005E, (u32)_change_koura_pose, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001,
    0x00000032, 0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_BTL_NOKO_SHUBIBI_802f8fcc, 0xFE363C8D, 0x0004005B,
    (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000005, 0x00000000, 0x00020032, 0xFE363C80,
    0x0000000F, 0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000001, 0x0000002D,
    0x0001005E, (u32)_koura_rotate_start, 0x0002005B, (u32)btlevtcmd_CommandGetWeaponActionLv, 0xFE363C80, 0x0003005B,
    (u32)btlevtcmd_AcSetDifficulty, 0xFFFFFFFE, 0xFE363C80, 0x0002005B, (u32)btlevtcmd_AcGetDifficulty, 0xFE363C80,
    0x00020036, 0xFE363C80, 0x00000003, 0x00010022, 0xFE363C80, 0x00010028,
    0xFFFFFFFD, 0x00020032, 0xFE363C80, 0x000001E0, 0x00020032, 0xFE363C81,
    0x000000F0, 0x00010024, 0xFFFFFFFE, 0x00020032, 0xFE363C80, 0x0000012C,
    0x00020032, 0xFE363C81, 0x000000B4, 0x00010024, 0xFFFFFFFF, 0x00020032,
    0xFE363C80, 0x000000F0, 0x00020032, 0xFE363C81, 0x00000078, 0x00010024,
    0x00000000, 0x00020032, 0xFE363C80, 0x000000B4, 0x00020032, 0xFE363C81,
    0x00000069, 0x00010024, 0x00000001, 0x00020032, 0xFE363C80, 0x00000096,
    0x00020032, 0xFE363C81, 0x0000005A, 0x00010024, 0x00000002, 0x00020032,
    0xFE363C80, 0x00000078, 0x00020032, 0xFE363C81, 0x0000003C, 0x0000002A,
    0x00020032, 0xFE363C80, 0x00000064, 0x00020032, 0xFE363C81, 0x0000001E,
    0x00000031, 0x0009005B, (u32)btlevtcmd_AcSetParamAll, 0x00000002, 0xFE363C80, 0x00000001,
    0x0000005F, 0xFE363C81, 0xF1194D80, 0xF1194D80, 0xF1194D80, 0x0002005B,
    (u32)btlevtcmd_AcSetFlag, 0x00000003, 0x0005005B, (u32)btlevtcmd_SetupAC, 0xFFFFFFFE, 0x00000013,
    0x00000001, 0x0000003C, 0x00010009, 0x00000016, 0x0001006F, 0xFE363C8F,
    0x00010005, 0x00002710, 0x0003005B, (u32)btlevtcmd_AcGetOutputParam, 0x00000000, 0xFE363C80,
    0x00020019, 0xFE363C80, 0x00000000, 0x00000007, 0x00000021, 0x00010009,
    0x00000001, 0x00000006, 0x00020032, 0xFE363C86, 0x00000000, 0x00010005,
    0x00002710, 0x00020035, 0xFE363C86, 0x00000001, 0x0002001B, 0xFE363C86,
    0x000000C8, 0x00020032, 0xFE363C86, 0x000000C8, 0x00000021, 0x00020032,
    0xFE363C87, 0xFE363C86, 0x00020038, 0xFE363C87, 0x00000002, 0x0004005B,
    (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000005, 0xFE363C87, 0x0004005B, (u32)btlevtcmd_GetUnitWork,
    0xFFFFFFFE, 0x00000004, 0xFE363C80, 0x0004005B, (u32)btlevtcmd_GetUnitWork, 0xFFFFFFFE,
    0x00000005, 0xFE363C81, 0x00020037, 0xFE363C80, 0xFE363C81, 0x00020038,
    0xFE363C80, 0x00000064, 0x0003005B, (u32)evt_snd_sfx_pit, 0xFE363C8D, 0xFE363C80,
    0x00010009, 0x00000001, 0x00000006, 0x00000070, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv,
    0x00000000, 0x00000004, 0x0003005B, (u32)evt_btl_camera_set_zoom, 0x00000000, 0x00000190,
    0x0002005B, (u32)btlevtcmd_StartAC, 0x00000001, 0x0001005B, (u32)btlevtcmd_ResultAC, 0x0001005B,
    (u32)btlevtcmd_StopAC, 0x0001005F, 0xFE363C8F, 0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE,
    0x00000005, 0x00000000, 0x0002005B, (u32)evt_snd_sfxoff, 0xFE363C8D, 0x0003005B,
    (u32)evt_snd_sfxon, (u32)(char*)str_SFX_BTL_NOKO_SHUBIBI_802f8fe4, 0x00000000, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000,
    0x00000000, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000002, 0x0005005B,
    (u32)evt_btl_camera_set_posoffset, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001, 0x00000032, 0x00020032, 0xFE363C8A,
    0x00000000, 0x00010003, 0x0000000A, 0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE,
    0xF24AA280, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0006005B, (u32)btlevtcmd_GetHitPos, 0xFE363C83, 0xFE363C84, 0xFE363C80,
    0xFE363C85, 0xFE363C85, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000000, 0x00000000, 0x00000000, 0x0006005B,
    (u32)btlevtcmd_CommandPreCheckDamage, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0x00000100, 0xFE363C86,
    0x00020019, 0xFE363C86, 0x00000001, 0x00020018, 0xFE363C86, 0x00000003,
    0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000026, 0x00000021, 0x00020018,
    0xFE363C86, 0x00000006, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000027,
    0x00000021, 0x00020018, 0xFE363C86, 0x00000002, 0x0003005B, (u32)btlevtcmd_StartAvoid,
    0xFE363C83, 0x00000028, 0x00000021, 0x00010004, 0x00000032, 0x00000021,
    0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv,
    0x00000000, 0x00000000, 0x0006005B, (u32)btlevtcmd_CommandPreCheckCounter, 0xFFFFFFFE, 0xFE363C83,
    0xFE363C84, 0x00000100, 0xFE363C80, 0x00020019, 0xFE363C80, 0x00000000,
    0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000001, 0x00000000, 0x0005005B,
    (u32)btlevtcmd_SetRotate, 0xFFFFFFFE, 0x00000000, 0x00000000, 0x00000000, 0x0001005E,
    (u32)_koura_rotate_stop, 0x00000021, 0x0002005B, (u32)btlevtcmd_GetResultAC, 0xFE363C80, 0x0002001E,
    0xFE363C80, 0x00000002, 0x0006005B, (u32)btlevtcmd_CommandCheckDamage, 0xFFFFFFFE, 0xFE363C83,
    0xFE363C84, 0x00020100, 0xFE363C85, 0x0004005B, (u32)btlevtcmd_GetResultPrizeLv, 0xFE363C83,
    0x00000000, 0xFE363C86, 0x0006005B, (u32)btlevtcmd_GetHitPos, 0xFE363C83, 0xFE363C84,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)btlevtcmd_ACSuccessEffect, 0xFE363C86,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress, 0xFFFFFFFE,
    0xFE363C80, 0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult, 0xFE363C80, 0xFE363C86, 0x00000020,
    0x0006005B, (u32)btlevtcmd_CommandCheckDamage, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0x00000100,
    0xFE363C85, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress, 0xFFFFFFFE, 0xFE363C80, 0x0003005B,
    (u32)btlevtcmd_AudienceDeclareACResult, 0xFE363C80, 0xFFFFFFFF, 0x00000021, 0x00010003, 0x00000032,
    0x0003005B, (u32)btlevtcmd_GetSelectNextEnemy, 0xFE363C83, 0xFE363C84, 0x00020019, 0xFE363C83,
    0xFFFFFFFF, 0x00020035, 0xFE363C8A, 0x00000001, 0x00010004, 0x0000000A,
    0x00000021, 0x00010003, 0x00000050, 0x0004005B, (u32)btlevtcmd_TransStageFloorPosition, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0x00000190,
    0xFE363C81, 0xFE363C82, 0x00000000, 0x00000000, 0x00000000, 0x00010003,
    0x0000005A, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000, 0x0003005B,
    (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00020037, 0xFE363C80, 0xFFFFFFFF,
    0x0005005B, (u32)btlevtcmd_SetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00000000, 0x00000000, 0x00000000, 0x0006005B, (u32)btlevtcmd_ACRStart, 0xFFFFFFFE,
    0x00000000, 0x00000009, 0x00000009, 0x00000000, 0x0003005B, (u32)btlevtcmd_ACRGetResult,
    0xFE363C86, 0xFE363C87, 0x00020018, 0xFE363C86, 0x00000002, 0x0003005B,
    (u32)btlevtcmd_CommandGetWeaponAddress, 0xFFFFFFFE, 0xFE363C80, 0x0006005B, (u32)btlevtcmd_AudienceDeclareAcrobatResult, 0xFE363C80,
    0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x0005005B, (u32)btlevtcmd_SetRotate,
    0xFFFFFFFE, 0x00000000, 0x00000000, 0x00000000, 0x0004005B, (u32)btlevtcmd_SetUnitWork,
    0xFFFFFFFE, 0x00000001, 0x0000000C, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0003005B, (u32)btlevtcmd_SetFallAccel, 0xFFFFFFFE,
    0xF24A7BB3, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0000001E, 0xFFFFFFFF, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE,
    0x00000001, (u32)(char*)str_PNK_E_1_802f8ffc, 0x00010009, 0x0000000A, 0x0006005B, (u32)btlevtcmd_ACRStart,
    0xFFFFFFFE, 0x00000031, 0x0000004F, 0x0000004F, 0x00000000, 0x0003005B,
    (u32)btlevtcmd_ACRGetResult, 0xFE363C86, 0xFE363C87, 0x00020019, 0xFE363C86, 0x00000002,
    0x0001005E, (u32)_koura_rotate_stop, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001,
    (u32)(char*)str_PNK_E_2_802f8fbc, 0x00010005, 0x00000006, 0x00010009, 0x00000001, 0x0005005B,
    (u32)btlevtcmd_AddRotate, 0xFFFFFFFE, 0x00000000, 0x0000002D, 0x00000000, 0x00000006,
    0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PNK_S_1_802f8cd0, 0x00010005,
    0x00000006, 0x00010009, 0x00000001, 0x0005005B, (u32)btlevtcmd_AddRotate, 0xFFFFFFFE,
    0x00000000, 0x0000000F, 0x00000000, 0x00000006, 0x0001005B, (u32)evt_audience_acrobat_notry,
    0x00010004, 0x00000063, 0x00000021, 0x0001005E, (u32)_koura_rotate_stop, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PNK_E_2_802f8fbc, 0x00010005, 0x00000006,
    0x00010009, 0x00000001, 0x0005005B, (u32)btlevtcmd_AddRotate, 0xFFFFFFFE, 0x00000000,
    0x0000002D, 0x00000000, 0x00000006, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE,
    0x00000001, (u32)(char*)str_PNK_E_3_802f8fc4, 0x00010005, 0x00000006, 0x00010009, 0x00000001,
    0x0005005B, (u32)btlevtcmd_AddRotate, 0xFFFFFFFE, 0x00000000, 0x0000000F, 0x00000000,
    0x00000006, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress, 0xFFFFFFFE, 0xFE363C80, 0x0006005B,
    (u32)btlevtcmd_AudienceDeclareAcrobatResult, 0xFE363C80, 0x00000001, 0x00000000, 0x00000000, 0x00000000,
    0x00010009, 0x0000003C, 0x00010004, 0x00000063, 0x00000021, 0x0001005E,
    (u32)_restore_koura_pose, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001, 0x0000002B,
    0x00010009, 0x0000001E, 0x00010003, 0x00000063, 0x0001005B, (u32)evt_audience_ap_recovery,
    0x0001005B, (u32)btlevtcmd_InviteApInfoReport, 0x0002005B, (u32)btlevtcmd_ResetFaceDirection, 0xFFFFFFFE, 0x0001005E,
    (u32)btldefaultevt_SuitoruBadgeEffect, 0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002, 0x00000001,
};

u32 partyNokotarouAttack_KouraGuard[] = {
    0x0003005B, (u32)btlevtcmd_GetSelectEnemy, 0xFE363C83, 0xFE363C84, 0x00020018, 0xFE363C83,
    0xFFFFFFFF, 0x00010004, 0x00000063, 0x00000021, 0x0003005B, (u32)btlevtcmd_RunDataEventChild,
    0xFFFFFFFE, 0x00000007, 0x0002005B, (u32)btlevtcmd_CommandPayWeaponCost, 0xFFFFFFFE, 0x0003005B,
    (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000007, 0x0004005B, (u32)evt_btl_camera_set_homing_unit, 0x00000000,
    0xFFFFFFFE, 0xFFFFFFFE, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000002,
    0x0003005B, (u32)evt_btl_camera_set_zoom, 0x00000000, 0x000000C8, 0x0005005B, (u32)evt_btl_camera_set_posoffset,
    0x00000000, 0x00000014, 0x00000000, 0x00000000, 0x0007005B, (u32)btlevtcmd_CalculateFaceDirection,
    0xFFFFFFFE, 0xFFFFFFFF, 0xFE363C83, 0xFE363C84, 0x00000010, 0xFE363C8F,
    0x0003005B, (u32)btlevtcmd_ChangeFaceDirection, 0xFFFFFFFE, 0xFE363C8F, 0x0003005B, (u32)_check_guard_koura,
    0xFFFFFFFD, 0xFE363C8A, 0x00020019, 0xFE363C8A, 0xFFFFFFFF, 0x0003005B,
    (u32)btlevtcmd_RunDataEventChild, 0xFE363C8A, 0x0000003E, 0x00000021, 0x0004005B, (u32)_color_lv_set,
    0xFFFFFFFD, 0x00000000, 0xFE363C8E, 0x0004005B, (u32)_color_lv_set, 0xFFFFFFFD,
    0x00000001, 0x000000FF, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001,
    (u32)(char*)str_PNK_A_2_802f9004, 0x00010009, 0x0000001E, 0x0004005B, (u32)btlevtcmd_SpawnUnit, 0xFE363C8A,
    (u32)entry_koura, 0x00000000, 0x0003005B, (u32)btlevtcmd_OnAttribute, 0xFE363C8A, 0x01000000,
    0x0005005B, (u32)btlevtcmd_GetPos, 0xFE363C83, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00020035, 0xFE363C82, 0x00000005, 0x00020035, 0xFE363C81, 0x000000F0,
    0x0005005B, (u32)btlevtcmd_SetPos, 0xFE363C8A, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0003005B, (u32)btlevtcmd_OffAttribute, 0xFE363C8A, 0x01000000, 0x0002005B, (u32)btlevtcmd_CommandGetWeaponActionLv,
    0xFE363C80, 0x0003005B, (u32)btlevtcmd_AcSetDifficulty, 0xFFFFFFFE, 0xFE363C80, 0x0003005B,
    (u32)btlevtcmd_GetBodyId, 0xFE363C8A, 0xFE363C8B, 0x0002005B, (u32)btlevtcmd_AcGetDifficulty, 0xFE363C80,
    0x00020036, 0xFE363C80, 0x00000003, 0x00010022, 0xFE363C80, 0x00010028,
    0xFFFFFFFD, 0x00020032, 0xFE363C80, 0x00000168, 0x00020032, 0xFE363C81,
    0x000001E0, 0x00010024, 0xFFFFFFFE, 0x00020032, 0xFE363C80, 0x000000B4,
    0x00020032, 0xFE363C81, 0x00000168, 0x00010024, 0xFFFFFFFF, 0x00020032,
    0xFE363C80, 0x00000078, 0x00020032, 0xFE363C81, 0x0000012C, 0x00010024,
    0x00000000, 0x00020032, 0xFE363C80, 0x0000005A, 0x00020032, 0xFE363C81,
    0x000000F0, 0x00010024, 0x00000001, 0x00020032, 0xFE363C80, 0x00000046,
    0x00020032, 0xFE363C81, 0x000000C8, 0x00010024, 0x00000002, 0x00020032,
    0xFE363C80, 0x00000028, 0x00020032, 0xFE363C81, 0x00000078, 0x0000002A,
    0x00020032, 0xFE363C80, 0x00000014, 0x00020032, 0xFE363C81, 0x00000050,
    0x00000031, 0x0009005B, (u32)btlevtcmd_AcSetParamAll, 0x00000001, 0xFE363C8A, 0xFE363C8B,
    0x0000001E, 0xFE363C80, 0xFE363C81, 0xF1194D80, 0xF1194D80, 0x0002005B,
    (u32)btlevtcmd_AcSetFlag, 0x00000000, 0x0005005B, (u32)btlevtcmd_SetupAC, 0xFFFFFFFE, 0x0000000C,
    0x00000001, 0x00000000, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFE363C8A, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00020032, 0xFE363C81, 0x0000007D, 0x000A005B,
    (u32)btlevtcmd_DivePosition, 0xFE363C8A, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0000003C,
    0x00000000, 0x0000000D, 0x00000000, 0xFFFFFFFF, 0x0002005B, (u32)btlevtcmd_StartAC,
    0x00000001, 0x0001005B, (u32)btlevtcmd_ResultAC, 0x0002005B, (u32)btlevtcmd_GetResultAC, 0xFE363C86,
    0x00020018, 0x00000000, 0x00000000, 0x0000006E, 0x00010009, 0x0000000A,
    0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFD, 0x00000001, (u32)(char*)str_M_N_7_80424398, 0x00010009,
    0x00000023, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFD, 0x00000001, (u32)(char*)str_M_B_1_804243a0,
    0x00010009, 0x0000000F, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFD, 0x00000001,
    0x0000002B, 0x00000070, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001,
    0x0003005B, (u32)evt_btl_camera_set_zoom, 0x00000000, 0x00000190, 0x0005005B, (u32)evt_btl_camera_set_posoffset,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0003005B, (u32)btlevtcmd_SetFallAccel,
    0xFE363C8A, 0xF24A7C19, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFE363C83, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00020035, 0xFE363C82, 0x00000005, 0x0005005B,
    (u32)btlevtcmd_GetPos, 0xFE363C8A, 0xFE363C80, 0xF1194D80, 0xF1194D80, 0x0004005B,
    (u32)btlevtcmd_TransStageFloorPosition, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0006005B, (u32)btlevtcmd_snd_se,
    0xFE363C8A, (u32)(char*)str_SFX_BTL_NOKO_GUARD2_802f900c, 0xF1194D80, 0x00000000, 0xFE363C85, 0x0000006E,
    0x00010009, 0x00000002, 0x0006005B, (u32)btlevtcmd_ACRStart, 0xFFFFFFFE, 0x00000030,
    0x00000043, 0x00000043, 0x00000000, 0x0003005B, (u32)btlevtcmd_ACRGetResult, 0xFE363C86,
    0xFE363C87, 0x00020018, 0xFE363C86, 0x00000002, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress,
    0xFFFFFFFE, 0xFE363C80, 0x0006005B, (u32)btlevtcmd_AudienceDeclareAcrobatResult, 0xFE363C80, 0x00000001,
    0x00000000, 0x00000000, 0x00000000, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE,
    0x00000001, (u32)(char*)str_PNK_O_1_802f8d28, 0x0001000A, 0x000003E8, 0x00000020, 0x0001005B,
    (u32)evt_audience_acrobat_notry, 0x00000021, 0x00000070, 0x0006005B, (u32)btlevtcmd_FallPosition, 0xFE363C8A,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0000003C, 0x0002005B, (u32)evt_snd_sfxoff,
    0xFE363C85, 0x0005005B, (u32)btlevtcmd_SetHomePos, 0xFE363C8A, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFE363C8A, 0x00000001, (u32)(char*)str_PKO_F_1_802f9020,
    0x0006005B, (u32)btlevtcmd_snd_se, 0xFE363C8A, (u32)(char*)str_SFX_BTL_NOKO_GUARD1_802f9028, 0xF1194D80, 0x00000000,
    0xF1194D80, 0x0006005B, (u32)evt_btl_camera_shake_h, 0x00000000, 0x00000002, 0x00000000,
    0x0000000A, 0x0000000D, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress, 0xFFFFFFFE, 0xFE363C8C,
    0x0002005B, (u32)btlevtcmd_WeaponAftereffect, 0xFE363C8C, 0x0000006E, 0x00020032, 0xFE363C89,
    0xFE363C8A, 0x0005005B, (u32)btlevtcmd_GetRotate, 0xFE363C89, 0xF1194D80, 0xF1194D80,
    0xFE363C88, 0x0002001A, 0xFE363C88, 0x000000B4, 0x0005005B, (u32)evt_sub_intpl_init,
    0x0000000B, 0xFE363C88, 0x00000000, 0x00000032, 0x00000020, 0x0005005B,
    (u32)evt_sub_intpl_init, 0x0000000B, 0xFE363C88, 0x00000168, 0x00000032, 0x00000021,
    0x00010005, 0x00000032, 0x0003005B, (u32)evt_sub_intpl_get_value_para, 0xFE363C81, 0xFE363C82,
    0x0005005B, (u32)btlevtcmd_SetRotate, 0xFE363C89, 0xF1194D80, 0xF1194D80, 0xFE363C81,
    0x00010009, 0x00000001, 0x00000006, 0x00000070, 0x0002001E, 0xFE363C86,
    0x00000002, 0x0003005B, (u32)btlevtcmd_AcGetOutputParam, 0x00000001, 0xFE363C80, 0x00010022,
    0xFE363C80, 0x00010029, 0x00000008, 0x00020032, 0xFE363C80, 0x00000002,
    0x00010029, 0x00000006, 0x00020032, 0xFE363C80, 0x00000001, 0x00010029,
    0x00000004, 0x00020032, 0xFE363C80, 0x00000000, 0x0000002A, 0x00020032,
    0xFE363C80, 0xFFFFFFFF, 0x00000031, 0x0002001D, 0xFE363C80, 0x00000000,
    0x0004005B, (u32)btlevtcmd_GetResultPrizeLv, 0xFE363C83, 0xFE363C80, 0xFE363C87, 0x0005005B,
    (u32)btlevtcmd_GetPos, 0xFE363C8A, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B,
    (u32)btlevtcmd_ACSuccessEffect, 0xFE363C87, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0003005B,
    (u32)btlevtcmd_AudienceDeclareACResult, 0xFE363C8C, 0xFE363C87, 0x00000020, 0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult,
    0xFE363C8C, 0xFFFFFFFF, 0x00000021, 0x00000020, 0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult,
    0xFE363C8C, 0xFFFFFFFF, 0x00000021, 0x00010009, 0x00000032, 0x0003005B,
    (u32)btlevtcmd_OnAttribute, 0xFE363C83, 0x00000020, 0x0003005B, (u32)btlevtcmd_GetUnitId, 0xFE363C83,
    0xFE363C80, 0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFE363C8A, 0x00000000, 0xFE363C80,
    0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001, 0x0000002B, 0x0004005B,
    (u32)btlevtcmd_SetUnitWork, 0xFE363C8A, 0x00000002, 0x00000003, 0x0001006F, 0xFE363C8F,
    0x00020032, 0xFE363C85, 0x00000005, 0x00010005, 0xFE363C85, 0x0004005B,
    (u32)btlevtcmd_GetUnitWork, 0xFE363C8A, 0x00000002, 0xFE363C8A, 0x00020019, 0xFE363C80,
    0x00000003, 0x00000007, 0x00000021, 0x00010009, 0x00000001, 0x00020032,
    0xFE363C85, 0x00000005, 0x00000006, 0x00000070, 0x0002005B, (u32)btlevtcmd_WaitEventEnd,
    0xFE363C8F, 0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFE363C8A, 0x00000002, 0x00000000,
    0x0002001E, 0xFE363C86, 0x00000002, 0x0003005B, (u32)btlevtcmd_AcGetOutputParam, 0x00000001,
    0xFE363C80, 0x00000020, 0x0003005B, (u32)btlevtcmd_AcGetOutputParam, 0x00000002, 0xFE363C80,
    0x00000021, 0x0003005B, (u32)btlevtcmd_SetHp, 0xFE363C8A, 0xFE363C80, 0x0003005B,
    (u32)btlevtcmd_RunDataEventChild, 0xFE363C8A, 0x0000003D, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFE363C8A,
    0x00000001, 0x0000002B, 0x0004005B, (u32)btlevtcmd_OnPartsAttribute, 0xFE363C83, 0x00000001,
    0x01000000, 0x0003005B, (u32)btlevtcmd_OnAttribute, 0xFE363C83, 0x00000060, 0x0002005B,
    (u32)btlevtcmd_StartWaitEvent, 0xFE363C83, 0x0002005B, (u32)_check_mario_move_count, 0xFE363C80, 0x0002001D,
    0xFE363C80, 0x00000001, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000,
    0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001, 0x0002005B, (u32)evt_btl_camera_off_posoffset_manual,
    0x00000000, 0x0001000A, 0x000001F4, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFE363C8A,
    0x0000003B, 0x00010009, 0x0000003C, 0x00000021, 0x0002005B, (u32)btlevtcmd_ResetFaceDirection,
    0xFFFFFFFE, 0x0001005B, (u32)evt_audience_ap_recovery, 0x0001005B, (u32)btlevtcmd_InviteApInfoReport, 0x0002005B,
    (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000,
    0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001, 0x0002005B, (u32)evt_btl_camera_off_posoffset_manual,
    0x00000000, 0x00000002, 0x00000021, 0x0001006F, 0xFE363C8F, 0x00010009,
    0x0000000A, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFD, 0x00000001, (u32)(char*)str_M_N_7_80424398,
    0x00010009, 0x00000023, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFD, 0x00000001,
    (u32)(char*)str_M_B_1_804243a0, 0x00010009, 0x00000096, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFD,
    0x00000001, 0x0000002B, 0x00000070, 0x0006005B, (u32)btlevtcmd_snd_se, 0xFE363C8A,
    (u32)(char*)str_SFX_BTL_NOKO_GUARD2_802f900c, 0xF1194D80, 0x00000000, 0xFE363C85, 0x0003005B, (u32)btlevtcmd_SetFallAccel,
    0xFE363C8A, 0xF24A7C19, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFE363C83, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00020035, 0xFE363C82, 0x00000005, 0x0006005B,
    (u32)btlevtcmd_FallPosition, 0xFE363C8A, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0000003C,
    0x0005005B, (u32)btlevtcmd_SetHomePos, 0xFE363C8A, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0002005B, (u32)evt_snd_sfxoff, 0xFE363C85, 0x0006005B, (u32)evt_btl_camera_shake_h, 0x00000000,
    0x00000002, 0x00000000, 0x0000000A, 0x0000000D, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress,
    0xFFFFFFFE, 0xFE363C8C, 0x0002005B, (u32)btlevtcmd_WeaponAftereffect, 0xFE363C8C, 0x0005005B,
    (u32)btlevtcmd_SetRotate, 0xFE363C8A, 0x00000000, 0x00000000, 0x00000000, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePose, 0xFE363C8A, 0x00000001, (u32)(char*)str_PKO_F_1_802f9020, 0x00010009, 0x00000032,
    0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFE363C8A, 0x00000001, (u32)(char*)str_PKO_H_1_802f903c, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePose, 0xFE363C8A, 0x00000002, (u32)(char*)str_PKO_H_2_802f9044, 0x0004005B, (u32)btlevtcmd_OffPartsAttribute,
    0xFE363C8A, 0x00000002, 0x01000000, 0x0006005B, (u32)btlevtcmd_SetPartsRotateOffset, 0xFE363C8A,
    0x00000001, 0x00000014, 0x00000000, 0x00000000, 0x0006005B, (u32)btlevtcmd_SetPartsRotateOffset,
    0xFE363C8A, 0x00000002, 0xFFFFFFEC, 0x00000000, 0x00000000, 0x00010005,
    0x00000014, 0x0006005B, (u32)btlevtcmd_AddPartsRotate, 0xFE363C8A, 0x00000001, 0x00000000,
    0x00000000, 0x00000002, 0x0006005B, (u32)btlevtcmd_AddPartsRotate, 0xFE363C8A, 0x00000002,
    0x00000000, 0x00000000, 0xFFFFFFFE, 0x00010009, 0x00000001, 0x00000006,
    0x00010005, 0x0000000A, 0x0004005B, (u32)btlevtcmd_OnPartsAttribute, 0xFE363C8A, 0x00000001,
    0x01000000, 0x0004005B, (u32)btlevtcmd_OnPartsAttribute, 0xFE363C8A, 0x00000002, 0x01000000,
    0x00010009, 0x00000002, 0x0004005B, (u32)btlevtcmd_OffPartsAttribute, 0xFE363C8A, 0x00000001,
    0x01000000, 0x0004005B, (u32)btlevtcmd_OffPartsAttribute, 0xFE363C8A, 0x00000002, 0x01000000,
    0x00010009, 0x00000002, 0x00000006, 0x0003005B, (u32)btlevtcmd_KillUnit, 0xFE363C8A,
    0x00000000, 0x0002005B, (u32)btlevtcmd_WaitEventEnd, 0xFE363C8F, 0x00010003, 0x00000063,
    0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv,
    0x00000000, 0x00000001, 0x0002005B, (u32)evt_btl_camera_off_posoffset_manual, 0x00000000, 0x0004005B,
    (u32)_color_lv_set, 0xFFFFFFFD, 0x00000001, 0xFE363C8E, 0x0001005B, (u32)evt_audience_ap_recovery,
    0x0001005B, (u32)btlevtcmd_InviteApInfoReport, 0x0002005B, (u32)btlevtcmd_ResetFaceDirection, 0xFFFFFFFE, 0x0002005B,
    (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002, 0x00000001,
};

u32 partyNokotarouAttack_TsuranukiKoura[] = {
    0x0003005B, (u32)btlevtcmd_GetSelectEnemy, 0xFE363C83, 0xFE363C84, 0x00020018, 0xFE363C83,
    0xFFFFFFFF, 0x00010004, 0x00000063, 0x00000021, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress,
    0xFFFFFFFE, 0xFE363C8C, 0x0002005B, (u32)btlevtcmd_WeaponAftereffect, 0xFE363C8C, 0x0002005B,
    (u32)btlevtcmd_AttackDeclareAll, 0xFFFFFFFE, 0x0001005B, (u32)btlevtcmd_WaitGuardMove, 0x0002005B, (u32)btlevtcmd_CommandPayWeaponCost,
    0xFFFFFFFE, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFFFFFFFE, 0x00000007, 0x0003005B,
    (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000007, 0x0004005B, (u32)evt_btl_camera_set_homing_unit, 0x00000000,
    0xFFFFFFFE, 0x00000001, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000002,
    0x0003005B, (u32)evt_btl_camera_set_zoom, 0x00000000, 0x000000C8, 0x0005005B, (u32)evt_btl_camera_set_posoffset,
    0x00000000, 0x00000014, 0x00000000, 0x00000000, 0x00010009, 0x00000014,
    0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000004, 0x0003005B, (u32)evt_btl_camera_set_zoom,
    0x00000000, 0x00000190, 0x0002005B, (u32)btlevtcmd_CommandGetWeaponActionLv, 0xFE363C80, 0x0003005B,
    (u32)btlevtcmd_AcSetDifficulty, 0xFFFFFFFE, 0xFE363C80, 0x0002005B, (u32)btlevtcmd_AcGetDifficulty, 0xFE363C80,
    0x00020036, 0xFE363C80, 0x00000003, 0x00010022, 0xFE363C80, 0x00010028,
    0xFFFFFFFD, 0x00020032, 0xFE363C80, 0x00000019, 0x00020032, 0xFE363C81,
    0x0000000F, 0x00010024, 0xFFFFFFFE, 0x00020032, 0xFE363C80, 0x00000018,
    0x00020032, 0xFE363C81, 0x00000015, 0x00010024, 0xFFFFFFFF, 0x00020032,
    0xFE363C80, 0x00000016, 0x00020032, 0xFE363C81, 0x00000018, 0x00010024,
    0x00000000, 0x00020032, 0xFE363C80, 0x00000014, 0x00020032, 0xFE363C81,
    0x00000019, 0x00010024, 0x00000001, 0x00020032, 0xFE363C80, 0x00000011,
    0x00020032, 0xFE363C81, 0x0000001B, 0x00010024, 0x00000002, 0x00020032,
    0xFE363C80, 0x0000000F, 0x00020032, 0xFE363C81, 0x0000001E, 0x0000002A,
    0x00020032, 0xFE363C80, 0x0000000C, 0x00020032, 0xFE363C81, 0x0000001E,
    0x00000031, 0x0009005B, (u32)btlevtcmd_AcSetParamAll, 0x00000007, 0x000000C8, 0x000000B2,
    0xFE363C80, 0xFE363C81, 0x00000014, 0x0000003D, 0xF1194D80, 0x0005005B,
    (u32)btlevtcmd_AcSetGaugeParam, 0x0000003C, 0x00000050, 0x00000064, 0x00000064, 0x0002005B,
    (u32)btlevtcmd_AcSetFlag, 0x00000000, 0x0005005B, (u32)btlevtcmd_SetupAC, 0xFFFFFFFE, 0x00000006,
    0x00000001, 0x00000000, 0x00010009, 0x00000016, 0x0004005B, (u32)btlevtcmd_SetUnitWork,
    0xFFFFFFFE, 0x00000003, 0xFFFFFFFF, 0x0001006F, 0xFE363C8F, 0x00010003,
    0x00000005, 0x0003005B, (u32)btlevtcmd_AcGetOutputParam, 0x00000000, 0xFE363C80, 0x00020018,
    0xFE363C80, 0x00000000, 0x00010009, 0x00000001, 0x00010004, 0x00000005,
    0x00000021, 0x0001005E, (u32)_change_koura_pose_fast, 0x0005005B, (u32)btlevtcmd_SetPartsWork, 0xFFFFFFFE,
    0x00000001, 0x00000000, 0x00000001, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE,
    0x00000001, 0x00000032, 0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_BTL_NOKO_IPPATSU_802f904c, 0xFE363C8D,
    0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000003, 0xFE363C8D, 0x0004005B,
    (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000005, 0x00000000, 0x00020032, 0xFE363C80,
    0x0000000F, 0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000001, 0x0000000F,
    0x0001005E, (u32)_koura_rotate_start, 0x00000070, 0x0000006E, 0x0002005B, (u32)btlevtcmd_WaitEventEnd,
    0xFE363C8F, 0x0002005B, (u32)_tsuranuki_effect_control, 0xFFFFFFFE, 0x00000070, 0x0002005B,
    (u32)btlevtcmd_StartAC, 0x00000001, 0x0001006F, 0xFE363C8E, 0x00020032, 0xFE363C85,
    0x00000005, 0x00010005, 0xFE363C85, 0x0003005B, (u32)btlevtcmd_AcGetOutputParam, 0x00000002,
    0xFE363C80, 0x00010022, 0xFE363C80, 0x00010028, 0x00000001, 0x0005005B,
    (u32)btlevtcmd_SetPartsWork, 0xFFFFFFFE, 0x00000001, 0x00000001, 0x00000000, 0x00010024,
    0x00000002, 0x0005005B, (u32)btlevtcmd_SetPartsWork, 0xFFFFFFFE, 0x00000001, 0x00000001,
    0x00000001, 0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000001, 0x00000018,
    0x00010024, 0x00000003, 0x0005005B, (u32)btlevtcmd_SetPartsWork, 0xFFFFFFFE, 0x00000001,
    0x00000001, 0x00000002, 0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000001,
    0x00000024, 0x00010024, 0x00000004, 0x0005005B, (u32)btlevtcmd_SetPartsWork, 0xFFFFFFFE,
    0x00000001, 0x00000001, 0x00000003, 0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE,
    0x00000001, 0x0000002D, 0x00000031, 0x0004005B, (u32)btlevtcmd_GetUnitWork, 0xFFFFFFFE,
    0x00000003, 0xFE363C8D, 0x00020019, 0xFE363C8D, 0xFFFFFFFF, 0x0003005B,
    (u32)btlevtcmd_AcGetOutputParam, 0x00000001, 0xFE363C81, 0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE,
    0x00000005, 0xFE363C81, 0x0004005B, (u32)btlevtcmd_GetUnitWork, 0xFFFFFFFE, 0x00000004,
    0xFE363C80, 0x0004005B, (u32)btlevtcmd_GetUnitWork, 0xFFFFFFFE, 0x00000005, 0xFE363C81,
    0x00020037, 0xFE363C80, 0xFE363C81, 0x00020038, 0xFE363C80, 0x00000064,
    0x0003005B, (u32)evt_snd_sfx_pit, 0xFE363C8D, 0xFE363C80, 0x00000021, 0x00010009,
    0x00000001, 0x00020032, 0xFE363C85, 0x00000005, 0x00000006, 0x00000070,
    0x0001005B, (u32)btlevtcmd_ResultAC, 0x0001005F, 0xFE363C8E, 0x0003005B, (u32)btlevtcmd_AcGetOutputParam,
    0x00000000, 0xFE363C80, 0x00020018, 0xFE363C80, 0x00000000, 0x0001005F,
    0xFE363C8F, 0x0005005B, (u32)btlevtcmd_SetPartsWork, 0xFFFFFFFE, 0x00000001, 0x00000000,
    0x00000000, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PNK_K_2_802f8f70,
    0x00010005, 0x0000000A, 0x0005005B, (u32)btlevtcmd_AddRotate, 0xFFFFFFFE, 0x00000000,
    0x0000000C, 0x00000000, 0x00010009, 0x00000001, 0x00000006, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PNK_K_1_802f8f78, 0x00010005, 0x00000010,
    0x0005005B, (u32)btlevtcmd_AddRotate, 0xFFFFFFFE, 0x00000000, 0x00000014, 0x00000000,
    0x00010009, 0x00000001, 0x00000006, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE,
    0x00000001, (u32)(char*)str_PNK_H_1_802f8d38, 0x00010005, 0x0000000E, 0x0005005B, (u32)btlevtcmd_AddRotate,
    0xFFFFFFFE, 0x00000000, 0x00000014, 0x00000000, 0x00010009, 0x00000001,
    0x00000006, 0x0005005B, (u32)btlevtcmd_SetRotate, 0xFFFFFFFE, 0x00000000, 0x00000000,
    0x00000000, 0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_BTL_ACROBAT_SHAK_802f9064, 0x00000000, 0x00010009,
    0x00000028, 0x0005005B, (u32)btlevtcmd_SetPartsWork, 0xFFFFFFFE, 0x00000001, 0x00000002,
    0x00000001, 0x0001005E, (u32)_restore_koura_pose, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE,
    0x00000001, 0x0000002B, 0x00010009, 0x0000001E, 0x00010004, 0x00000063,
    0x00000021, 0x0002005B, (u32)btlevtcmd_WaitEventEnd, 0xFE363C8F, 0x0005005B, (u32)btlevtcmd_SetPartsWork,
    0xFFFFFFFE, 0x00000001, 0x00000000, 0x00000000, 0x0004005B, (u32)btlevtcmd_GetUnitWork,
    0xFFFFFFFE, 0x00000003, 0xFE363C8D, 0x0002005B, (u32)evt_snd_sfxoff, 0xFE363C8D,
    0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_BTL_NOKO_IPPATSU_802f907c, 0x00000000, 0x0001005B, (u32)btlevtcmd_StopAC,
    0x0001005F, 0xFE363C8F, 0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000005,
    0x00000000, 0x0002005B, (u32)evt_snd_sfxoff, 0xFE363C8D, 0x0003005B, (u32)evt_snd_sfxon,
    (u32)(char*)str_SFX_BTL_NOKO_SHUBIBI_802f8fe4, 0x00000000, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000,
    0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000002, 0x0005005B, (u32)evt_btl_camera_set_posoffset,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType,
    0xFFFFFFFE, 0x00000001, 0x00000032, 0x00020032, 0xFE363C8A, 0x00000000,
    0x00010003, 0x0000000A, 0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE, 0xF24AA280,
    0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0006005B, (u32)btlevtcmd_GetHitPos, 0xFE363C83, 0xFE363C84, 0xFE363C80, 0xF1194D80,
    0xF1194D80, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000000, 0x00000000, 0x00000000, 0x0006005B, (u32)btlevtcmd_CommandPreCheckDamage,
    0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0x00000100, 0xFE363C86, 0x00020019,
    0xFE363C86, 0x00000001, 0x00020018, 0xFE363C86, 0x00000003, 0x0003005B,
    (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000026, 0x00000021, 0x00020018, 0xFE363C86,
    0x00000006, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000027, 0x00000021,
    0x00020018, 0xFE363C86, 0x00000002, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83,
    0x00000028, 0x00000021, 0x00010004, 0x00000032, 0x00000021, 0x0003005B,
    (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000,
    0x00000000, 0x0006005B, (u32)btlevtcmd_CommandPreCheckCounter, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84,
    0x00000100, 0xFE363C80, 0x00020019, 0xFE363C80, 0x00000000, 0x0005005B,
    (u32)btlevtcmd_SetPartsWork, 0xFFFFFFFE, 0x00000001, 0x00000002, 0x00000001, 0x0004005B,
    (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000001, 0x00000000, 0x0005005B, (u32)btlevtcmd_SetRotate,
    0xFFFFFFFE, 0x00000000, 0x00000000, 0x00000000, 0x0001005E, (u32)_koura_rotate_stop,
    0x00000021, 0x0002005B, (u32)btlevtcmd_GetResultAC, 0xFE363C80, 0x0002001E, 0xFE363C80,
    0x00000002, 0x0006005B, (u32)btlevtcmd_CommandCheckDamage, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84,
    0x00020100, 0xFE363C85, 0x0003005B, (u32)btlevtcmd_AcGetOutputParam, 0x00000002, 0xFE363C89,
    0x0002001D, 0xFE363C89, 0x00000004, 0x00020036, 0xFE363C89, 0x00000004,
    0x0004005B, (u32)btlevtcmd_GetResultPrizeLv, 0xFE363C83, 0xFE363C89, 0xFE363C86, 0x0006005B,
    (u32)btlevtcmd_GetHitPos, 0xFE363C83, 0xFE363C84, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0005005B, (u32)btlevtcmd_ACSuccessEffect, 0xFE363C86, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress, 0xFFFFFFFE, 0xFE363C80, 0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult,
    0xFE363C80, 0xFE363C86, 0x00000020, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress, 0xFFFFFFFE,
    0xFE363C80, 0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult, 0xFE363C80, 0xFFFFFFFF, 0x00000021,
    0x00000020, 0x0006005B, (u32)btlevtcmd_CommandCheckDamage, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84,
    0x00000100, 0xFE363C85, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress, 0xFFFFFFFE, 0xFE363C80,
    0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult, 0xFE363C80, 0xFFFFFFFF, 0x00000021, 0x00010003,
    0x00000032, 0x0003005B, (u32)btlevtcmd_GetSelectNextEnemy, 0xFE363C83, 0xFE363C84, 0x00020019,
    0xFE363C83, 0xFFFFFFFF, 0x00020035, 0xFE363C8A, 0x00000001, 0x00010004,
    0x0000000A, 0x00000021, 0x00010003, 0x00000050, 0x0004005B, (u32)btlevtcmd_TransStageFloorPosition,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE,
    0x00000190, 0xFE363C81, 0xFE363C82, 0x00000000, 0x00000000, 0x00000000,
    0x0005005B, (u32)btlevtcmd_SetPartsWork, 0xFFFFFFFE, 0x00000001, 0x00000002, 0x00000001,
    0x0001000A, 0x00000064, 0x00010003, 0x0000005A, 0x0003005B, (u32)evt_btl_camera_set_mode,
    0x00000000, 0x00000000, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001,
    0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00020037, 0xFE363C80, 0xFFFFFFFF, 0x0005005B, (u32)btlevtcmd_SetPos, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000, 0x00000000, 0x00000000,
    0x0006005B, (u32)btlevtcmd_ACRStart, 0xFFFFFFFE, 0x00000000, 0x00000009, 0x00000009,
    0x00000000, 0x0003005B, (u32)btlevtcmd_ACRGetResult, 0xFE363C86, 0xFE363C87, 0x00020018,
    0xFE363C86, 0x00000002, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress, 0xFFFFFFFE, 0xFE363C80,
    0x0006005B, (u32)btlevtcmd_AudienceDeclareAcrobatResult, 0xFE363C80, 0x00000001, 0x00000000, 0x00000000,
    0x00000000, 0x0005005B, (u32)btlevtcmd_SetRotate, 0xFFFFFFFE, 0x00000000, 0x00000000,
    0x00000000, 0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000001, 0x0000000C,
    0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0003005B, (u32)btlevtcmd_SetFallAccel, 0xFFFFFFFE, 0xF24A7BB3, 0x0007005B, (u32)btlevtcmd_JumpPosition,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0000001E, 0xFFFFFFFF,
    0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PNK_E_1_802f8ffc, 0x00010009,
    0x0000000A, 0x0006005B, (u32)btlevtcmd_ACRStart, 0xFFFFFFFE, 0x00000031, 0x0000004F,
    0x0000004F, 0x00000000, 0x0003005B, (u32)btlevtcmd_ACRGetResult, 0xFE363C86, 0xFE363C87,
    0x00020019, 0xFE363C86, 0x00000002, 0x0001005B, (u32)evt_audience_acrobat_notry, 0x0001005E,
    (u32)_koura_rotate_stop, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PNK_E_2_802f8fbc,
    0x00010005, 0x00000006, 0x00010009, 0x00000001, 0x0005005B, (u32)btlevtcmd_AddRotate,
    0xFFFFFFFE, 0x00000000, 0x0000002D, 0x00000000, 0x00000006, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PNK_S_1_802f8cd0, 0x00010005, 0x00000006,
    0x00010009, 0x00000001, 0x0005005B, (u32)btlevtcmd_AddRotate, 0xFFFFFFFE, 0x00000000,
    0x0000000F, 0x00000000, 0x00000006, 0x00010004, 0x00000063, 0x00000021,
    0x0001005E, (u32)_koura_rotate_stop, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001,
    (u32)(char*)str_PNK_E_2_802f8fbc, 0x00010005, 0x00000006, 0x00010009, 0x00000001, 0x0005005B,
    (u32)btlevtcmd_AddRotate, 0xFFFFFFFE, 0x00000000, 0x0000002D, 0x00000000, 0x00000006,
    0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PNK_E_3_802f8fc4, 0x00010005,
    0x00000006, 0x00010009, 0x00000001, 0x0005005B, (u32)btlevtcmd_AddRotate, 0xFFFFFFFE,
    0x00000000, 0x0000000F, 0x00000000, 0x00000006, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress,
    0xFFFFFFFE, 0xFE363C80, 0x0006005B, (u32)btlevtcmd_AudienceDeclareAcrobatResult, 0xFE363C80, 0x00000001,
    0x00000000, 0x00000000, 0x00000000, 0x00010009, 0x0000003C, 0x00010004,
    0x00000063, 0x00000020, 0x0001005B, (u32)evt_audience_acrobat_notry, 0x00000021, 0x0001005E,
    (u32)_restore_koura_pose, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001, 0x0000002B,
    0x00010009, 0x0000001E, 0x00010003, 0x00000063, 0x0001005B, (u32)evt_audience_ap_recovery,
    0x0001005B, (u32)btlevtcmd_InviteApInfoReport, 0x0002005B, (u32)btlevtcmd_ResetFaceDirection, 0xFFFFFFFE, 0x0001005E,
    (u32)btldefaultevt_SuitoruBadgeEffect, 0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002, 0x00000001,
};

u32 nokotarou_appeal_event[] = {
    0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFFFFFFFE, 0x00000007, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType,
    0xFFFFFFFE, 0x00000001, 0x00000042, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_MARIO_APPEAL1_802f9094,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000, 0x00010009, 0x0000003A,
    0x0002005B, (u32)btlevtcmd_AudienceAppeal, 0xFFFFFFFE, 0x00010009, 0x0000001E, 0x0002005B,
    (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002, 0x00000001,
};

static u32 party_win_reaction[16] = {
    0x0004005B, (u32)btlevtcmd_CheckStatus, 0xFFFFFFFE, 0x00000011, 0xFE363C80, 0x00020018,
    0xFE363C80, 0x00000000, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001,
    (u32)(char*)str_PNK_Y_1_802f90a8, 0x00000021, 0x00000002, 0x00000001,
};

static u32 attack_audience_event[263] = {
    0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0004005B, (u32)btlevtcmd_GetStageSize, 0xFE363C83, 0xF1194D80, 0xF1194D80, 0x0003005B,
    (u32)btlevtcmd_GetFaceDirection, 0xFFFFFFFE, 0xFE363C84, 0x00020032, 0xFE363C88, 0xFE363C83,
    0x00020037, 0xFE363C88, 0xFE363C84, 0x00020037, 0xFE363C88, 0xFFFFFFFF,
    0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE, 0xF24A9280, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType,
    0xFFFFFFFE, 0x00000001, 0x0000002A, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE,
    0xFE363C88, 0xFE363C81, 0xFE363C82, 0x00000000, 0xFFFFFFFF, 0x00000000,
    0x00010009, 0x00000014, 0x0005005B, (u32)_get_attack_aud_no, 0xFE363C83, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0004005B, (u32)btlevtcmd_GetStageAudienceAreaSize, 0xFE363C84, 0xF1194D80,
    0xF1194D80, 0x0003005B, (u32)btlevtcmd_GetFaceDirection, 0xFFFFFFFE, 0xFE363C85, 0x00020032,
    0xFE363C86, 0xFE363C84, 0x00020037, 0xFE363C86, 0xFE363C85, 0x00020037,
    0xFE363C86, 0xFFFFFFFF, 0x0005005B, (u32)btlevtcmd_SetPos, 0xFFFFFFFE, 0xFE363C86,
    0xFE363C81, 0xFE363C82, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x0000000C,
    0x0005005B, (u32)evt_btl_camera_set_homing_unit_audience, 0x00000000, 0xFFFFFFFE, 0x00000001, 0xFE363C83,
    0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000002, 0x0003005B, (u32)evt_btl_camera_set_zoom,
    0x00000000, 0x00000064, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001,
    0x00000032, 0x0001006F, 0xFE363C8F, 0x00020032, 0xFE363C80, 0x0000000F,
    0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000001, 0x0000002D, 0x0001005E,
    (u32)_koura_rotate_start, 0x00000070, 0x0002005B, (u32)btlevtcmd_WaitEventEnd, 0xFE363C8F, 0x0001000A,
    0x000003E8, 0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_BTL_NOKO_ATTACK2_802f8f8c, 0x00000000, 0x0003005B,
    (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE, 0xF24A9A80, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE,
    0xFE363C80, 0x0000000F, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000000, 0x00000000, 0x00000000, 0x0003005B,
    (u32)_attack_aud, 0xFE363C83, 0x00000000, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000,
    0x00000003, 0x0001006F, 0xFE363C8F, 0x0001000A, 0x000005DC, 0x0003005B,
    (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000, 0x0001000A, 0x000003E8, 0x00000070,
    0x0003005B, (u32)btlevtcmd_SetFallAccel, 0xFFFFFFFE, 0xF24A7D4C, 0x0005005B, (u32)btlevtcmd_GetPos,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B, (u32)btlevtcmd_TransStageFloorPosition,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE,
    0xFE363C80, 0x0000001E, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000012, 0xFFFFFFFF, 0x0003005B, (u32)btlevtcmd_SetFallAccel,
    0xFFFFFFFE, 0xF24A7CE6, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE, 0xFE363C80,
    0x00000014, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0000000C, 0xFFFFFFFF, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE,
    0xFE363C80, 0x00000008, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000008, 0xFFFFFFFF, 0x0001005E, (u32)_restore_koura_pose,
    0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001, 0x0000002A, 0x0003005B,
    (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE, 0xF24A8A80, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE,
    0xFE363C86, 0xFE363C81, 0xFE363C82, 0x00000000, 0xFFFFFFFF, 0x00000000,
    0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0005005B, (u32)btlevtcmd_SetPos, 0xFFFFFFFE, 0xFE363C88, 0xFE363C81, 0xFE363C82,
    0x0002005B, (u32)btlevtcmd_WaitEventEnd, 0xFE363C8F, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000, 0xFFFFFFFF, 0x00000000,
    0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002, 0x00000001,
};

u32 unk_evt_80393a14[] = {
    0x0004005B, (u32)btlevtcmd_SetPartsDefenceTable, 0xFE363C8A, 0xFE363C8B, (u32)defence, 0x0003005B,
    (u32)btlevtcmd_OffStatusFlag, 0xFE363C8A, 0x00000001, 0x0004005B, (u32)btlevtcmd_AnimeSetPoseTable, 0xFE363C8A,
    0xFE363C8B, (u32)pose_table_nokotarou_stay, 0x00000002, 0x00000001, 0x00000000,
};
