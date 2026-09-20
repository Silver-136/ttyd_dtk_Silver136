#include "unit/party/unit_party_christine.h"

extern void* _battleWorkPointer;
extern s32 evtGetValue(void* evt, s32 arg);
extern void evtSetValue(void* evt, s32 arg, s32 value);
extern s32 BattleTransID(void* evt, s32 id);
extern void* BattleGetUnitPtr(void* battleWork, s32 id);

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 krb_get_dir(void* evt) {
    extern f32 evtGetFloat(void* evt, s32 arg);
    extern f32 angleABf(f32 x1, f32 z1, f32 x2, f32 z2);
    extern const f32 float_0_804243cc;

    s32* args = *(s32**)((s32)evt + 0x18);
    f32 x1 = evtGetFloat(evt, args[0]);
    f32 z1 = evtGetFloat(evt, args[1]);
    f32 x2 = evtGetFloat(evt, args[2]);
    f32 z2 = evtGetFloat(evt, args[3]);
    s32 def = evtGetValue(evt, args[4]);
    f32 dx = x2 - x1;
    f32 dz = z2 - z1;
    f32 zero;

    zero = float_0_804243cc;
    if (zero == dx && zero == dz) {
        evtSetValue(evt, args[4], def);
        return 2;
    }
    zero = float_0_804243cc;
    evtSetValue(evt, args[4], (s32)angleABf(dx, dz, zero, zero));
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


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
    extern u8 lbl_80393A58[];
    s32 techLv;
    void* party;
    u8* data;
    TechMenuWork* work;

    data = lbl_80393A58 + 0x450;
    work = commandWork;
    party = BattleGetPartyPtr(_battleWorkPointer);
    techLv = partyGetTechLv(BattleTransPartyId(*(s32*)((s32)party + 8)));

    work->entries[*count].field_10 = -1;
    work->entries[*count].field_14 = 0;
    work->entries[*count].name = (char**)(data + 0x0);
    work->entries[*count].value = 0;
    work->entries[*count].icon = *(u16*)(work->entries[*count].name + 1);
    work->entries[*count].message = msgSearch(*work->entries[*count].name);
    *count = *count + 1;

    if (techLv >= 0) {
        work->entries[*count].field_10 = -1;
        work->entries[*count].field_14 = 0;
        work->entries[*count].name = (char**)(data + 0xC0);
        work->entries[*count].value = 0;
        work->entries[*count].icon = *(u16*)(work->entries[*count].name + 1);
        work->entries[*count].message = msgSearch(*work->entries[*count].name);
        *count = *count + 1;
    }
    if (techLv >= 1) {
        work->entries[*count].field_10 = -1;
        work->entries[*count].field_14 = 0;
        work->entries[*count].name = (char**)(data + 0x180);
        work->entries[*count].value = 0;
        work->entries[*count].icon = *(u16*)(work->entries[*count].name + 1);
        work->entries[*count].message = msgSearch(*work->entries[*count].name);
        *count = *count + 1;
    }
    if (techLv >= 2) {
        work->entries[*count].field_10 = -1;
        work->entries[*count].field_14 = 0;
        work->entries[*count].name = (char**)(data + 0x240);
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

s32 btlevtcmd_get_monosiri_msg_no(void* evt) {
    extern void* battleGetUnitMonosiriPtr(void* unit);

    s32* args = *(s32**)((s32)evt + 0x18);
    void* mono;
    s32 id = evtGetValue(evt, args[0]);
    evtGetValue(evt, args[1]);
    mono = battleGetUnitMonosiriPtr(*(void**)((s32)BattleGetUnitPtr(_battleWorkPointer, id) + 8));
    evtSetValue(evt, args[2], *(s32*)((s32)mono + 4));
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

s32 _monosiri_flag_on(void* evt) {
    extern void battleSetUnitMonosiriFlag(void* unit);

    s32 id = evtGetValue(evt, **(s32**)((s32)evt + 0x18));
    void* unit = BattleGetUnitPtr(_battleWorkPointer, id);
    battleSetUnitMonosiriFlag(*(void**)((s32)unit + 8));
    return 2;
}
s32 _dictionary(void* evt) {
    extern void* monosiriInit(void* unit, f32 x, f32 y);
    extern void monosiriMain(void* work);
    extern void monosiriClose(void* work);
    extern s32 monosiriCloseWait(void* work);
    extern void monosiriExit(void* work);
    extern const f32 float_neg50_804243c4;
    extern const f32 float_50_804243c8;

    void* battleWork = _battleWorkPointer;
    s32* args = *(s32**)((s32)evt + 0x18);
    void* unit;
    s32 state;
    void* work;
    s32 currentId = BattleTransID(evt, -2);
    s32 targetId = BattleTransID(evt, evtGetValue(evt, args[0]));
    evtGetValue(evt, args[1]);
    unit = BattleGetUnitPtr(battleWork, currentId);
    state = *(s32*)((s32)unit + 0x218);
    work = *(void**)((s32)unit + 0x21C);

    switch (state) {
        case 0:
            work = monosiriInit(*(void**)((s32)BattleGetUnitPtr(battleWork, targetId) + 8),
                                float_neg50_804243c4, float_50_804243c8);
            state = 2;
            break;
        case 1:
            monosiriMain(work);
            state = 3;
            break;
        case 2:
            monosiriMain(work);
            break;
        case 3:
            monosiriClose(work);
            monosiriMain(work);
            state = 5;
            break;
        case 4:
            if (monosiriCloseWait(work)) {
                monosiriExit(work);
                state = 6;
            } else {
                monosiriMain(work);
            }
            break;
    }
    *(s32*)((s32)unit + 0x218) = state;
    *(void**)((s32)unit + 0x21C) = work;
    return 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 _set_hustle(void* evt) {
    extern s32 BtlUnit_CheckStatus(void* unit, s32 status);
    extern void BtlUnit_OnStatusFlag(void* unit, u32 flag);

    s32* args = *(s32**)((s32)evt + 0x18);
    void* battleWork = _battleWorkPointer;
    s32 id = evtGetValue(evt, args[0]);
    void* unit = BattleGetUnitPtr(battleWork, BattleTransID(evt, id));

    if (unit != 0) {
        if (*(s8*)((s32)unit + 0x22) <= 0) {
            *(u8*)((s32)unit + 0x22) = *(u8*)((s32)unit + 0x22) + 1;
            *(u8*)((s32)unit + 0x20) = 0;
            *(u8*)((s32)unit + 0x311) = 0xFF;
            if (*(s8*)((s32)unit + 0x23) <= 0) {
                *(u8*)((s32)unit + 0x23) = 1;
                if (BtlUnit_CheckStatus(unit, 0x14)) {
                    *(u32*)((s32)unit + 0x27C) |= 0x2000;
                }
            }
        } else {
            BtlUnit_OnStatusFlag(unit, 0x02000000);
        }
    }
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

/* Recovered Christine battle-event dependencies. */
extern const char str_btl_un_christine_802f9140[];
extern const char str_c_pkr_804243b0[];
extern const char str_SFX_PARTY_DAMAGE1_802f9154[];
extern const char str_SFX_MARIO_DAMAGE_FIR_802f9168[];
extern const char str_SFX_MARIO_DAMAGE_ICE_802f9180[];
extern const char str_SFX_MARIO_DAMAGE_BIR_802f9198[];
static u32 init_event[51];
extern u32 btldefaultevt_Dummy[];
extern u32 btldefaultevt_avoid_counter_event[];
extern u32 subset_attack_ready_jump_event[];
extern u32 christine_appeal_event[];
extern u32 partyChristineAttack_NormalAttack[];
extern u32 partyChristineAttack_Monosiri[];
extern u32 partyChristineAttack_RenzokuAttack[];
extern u32 partyChristineAttack_Kiss[];
static u32 attack_audience_event[392];
static u32 party_win_reaction[14];
extern const char str_PKR_A_1_802f9138[];
extern const char str_PKR_C_1_802f9128[];
extern const char str_PKR_D_1_802f90f8[];
extern const char str_PKR_D_2_802f90c8[];
extern const char str_PKR_D_3_802f90d8[];
extern const char str_PKR_D_4_802f9110[];
extern const char str_PKR_D_5_802f90b8[];
extern const char str_PKR_G_1_802f90f0[];
extern const char str_PKR_G_2_802f90e8[];
extern const char str_PKR_I_1_802f9120[];
extern const char str_PKR_N_1_802f90d0[];
extern const char str_PKR_R_1_802f9108[];
extern const char str_PKR_W_1_802f9100[];
extern const char str_PKR_X_1_802f90c0[];
extern const char str_btl_wn_pkr_lv1_802f9288[];
extern const char str_btl_wn_pkr_lv2_802f92c0[];
extern const char str_btl_wn_pkr_lv3_802f92e8[];
extern const char str_btl_wn_pkr_normal_802f9250[];
extern const char str_msg_ac_kiss_802f930c[];
extern const char str_msg_ac_monoshiri_802f92ac[];
extern const char str_msg_ac_zutsuki_802f9278[];
extern const char str_msg_pkr_monosiri_802f9298[];
extern const char str_msg_pkr_nage_kiss_802f92f8[];
extern const char str_msg_pkr_normal_jump_802f9264[];
extern const char str_msg_pkr_renzoku_zuts_802f92d0[];
extern s32 weaponGetPowerFromPartyAttackLv();
extern s32 _attack_aud();
extern s32 _get_attack_aud_no();
extern s32 btldefaultevt_Confuse();
extern s32 btldefaultevt_Damage();
extern s32 btldefaultevt_SuitoruBadgeEffect();
extern s32 btlevtcmd_ACRGetResult();
extern s32 btlevtcmd_ACRStart();
extern s32 btlevtcmd_ACSuccessEffect();
extern s32 btlevtcmd_AcGetDifficulty();
extern s32 btlevtcmd_AcSetDifficulty();
extern s32 btlevtcmd_AcSetFlag();
extern s32 btlevtcmd_AcSetParamAll();
extern s32 btlevtcmd_AddRotate();
extern s32 btlevtcmd_AnimeChangePose();
extern s32 btlevtcmd_AnimeChangePoseFromTable();
extern s32 btlevtcmd_AnimeChangePoseType();
extern s32 btlevtcmd_AnnounceMessage();
extern s32 btlevtcmd_AttackDeclare();
extern s32 btlevtcmd_AudienceAppeal();
extern s32 btlevtcmd_AudienceDeclareACResult();
extern s32 btlevtcmd_AudienceDeclareAcrobatResult();
extern s32 btlevtcmd_CalculateFaceDirection();
extern s32 btlevtcmd_ChangeFaceDirection();
extern s32 btlevtcmd_CheckDamage();
extern s32 btlevtcmd_CommandCheckDamage();
extern s32 btlevtcmd_CommandGetWeaponActionLv();
extern s32 btlevtcmd_CommandGetWeaponAddress();
extern s32 btlevtcmd_CommandPayWeaponCost();
extern s32 btlevtcmd_CommandPreCheckDamage();
extern s32 btlevtcmd_FaceDirectionAdd();
extern s32 btlevtcmd_FaceDirectionSub();
extern s32 btlevtcmd_GetBodyId();
extern s32 btlevtcmd_GetFaceDirection();
extern s32 btlevtcmd_GetHeight();
extern s32 btlevtcmd_GetHitPos();
extern s32 btlevtcmd_GetHomePos();
extern s32 btlevtcmd_GetMoveFrame();
extern s32 btlevtcmd_GetPos();
extern s32 btlevtcmd_GetPosFloat();
extern s32 btlevtcmd_GetResultAC();
extern s32 btlevtcmd_GetResultCountAC();
extern s32 btlevtcmd_GetResultPrizeLv();
extern s32 btlevtcmd_GetRotate();
extern s32 btlevtcmd_GetSelectEnemy();
extern s32 btlevtcmd_GetStageAudienceAreaSize();
extern s32 btlevtcmd_GetStageSize();
extern s32 btlevtcmd_GetStatusMg();
extern s32 btlevtcmd_GetTakeoffPosition();
extern s32 btlevtcmd_GetUnitWork();
extern s32 btlevtcmd_InviteApInfoReport();
extern s32 btlevtcmd_JumpContinue();
extern s32 btlevtcmd_JumpPosition();
extern s32 btlevtcmd_JumpSetting();
extern s32 btlevtcmd_MarioJumpParam();
extern s32 btlevtcmd_MarioJumpPosition();
extern s32 btlevtcmd_MoveDirectionAdd();
extern s32 btlevtcmd_MovePosition();
extern s32 btlevtcmd_ResetFaceDirection();
extern s32 btlevtcmd_ResultAC();
extern s32 btlevtcmd_RunDataEventChild();
extern s32 btlevtcmd_SetEventAttack();
extern s32 btlevtcmd_SetEventConfusion();
extern s32 btlevtcmd_SetEventDamage();
extern s32 btlevtcmd_SetEventEntry();
extern s32 btlevtcmd_SetEventWait();
extern s32 btlevtcmd_SetFallAccel();
extern s32 btlevtcmd_SetJumpSound();
extern s32 btlevtcmd_SetMoveSoundDataWork();
extern s32 btlevtcmd_SetMoveSpeed();
extern s32 btlevtcmd_SetPos();
extern s32 btlevtcmd_SetRotate();
extern s32 btlevtcmd_SetRotateOffset();
extern s32 btlevtcmd_SetRunSound();
extern s32 btlevtcmd_SetScale();
extern s32 btlevtcmd_SetStayPose();
extern s32 btlevtcmd_SetTalkPose();
extern s32 btlevtcmd_SetUnitWork();
extern s32 btlevtcmd_SetWalkSound();
extern s32 btlevtcmd_SetupAC();
extern s32 btlevtcmd_StartAC();
extern s32 btlevtcmd_StartAvoid();
extern s32 btlevtcmd_StartWaitEvent();
extern s32 btlevtcmd_StopAC();
extern s32 btlevtcmd_TransStageFloorPosition();
extern s32 btlevtcmd_WaitEventEnd();
extern s32 btlevtcmd_WaitGuardMove();
extern s32 btlevtcmd_WeaponAftereffect();
extern s32 btlevtcmd_ac_timing_flag_onoff();
extern s32 btlevtcmd_ac_timing_get_success_frame();
extern s32 btlevtcmd_ftof();
extern s32 btlevtcmd_ftomsec();
extern s32 btlevtcmd_snd_se();
extern s32 evt_audience_acrobat_notry();
extern s32 evt_audience_ap_recovery();
extern s32 evt_btl_camera_off_posoffset_manual();
extern s32 evt_btl_camera_set_homing_unit();
extern s32 evt_btl_camera_set_homing_unit_audience();
extern s32 evt_btl_camera_set_homing_unitparts();
extern s32 evt_btl_camera_set_mode();
extern s32 evt_btl_camera_set_moveSpeedLv();
extern s32 evt_btl_camera_set_posoffset();
extern s32 evt_btl_camera_set_zoom();
extern s32 evt_eff();
extern s32 evt_eff64();
extern s32 evt_eff_fukidashi();
extern s32 evt_msg_print();
extern s32 evt_snd_sfxon_3d();
extern s32 mario_get_renzoku_count_max();
extern s32 unk_evt_803537c4();
extern u32 partyWeapon_ChristineKiss[];
extern const char str_PKR_2A_1_802f9348[];
extern const char str_PKR_2A_2_802f9354[];
extern const char str_PKR_2A_3_802f9378[];
extern const char str_PKR_2A_4_802f93a0[];
extern const char str_PKR_2A_5_802f93cc[];
extern const char str_PKR_A_2B_802f9318[];
extern const char str_PKR_J_1A_802f9324[];
extern const char str_PKR_J_1B_802f9438[];
extern const char str_PKR_O_1_802f9130[];
extern const char str_PKR_S_1_802f90e0[];
extern const char str_PKR_T_1_802f9118[];
extern const char str_PKR_Y_1_802f93c4[];
extern const char str_SFX_BTL_KURI_ATTACK1_802f9330[];
extern const char str_SFX_BTL_KURI_ATTACK2_802f93d8[];
extern const char str_SFX_BTL_KURI_CHEER_K_802f941c[];
extern const char str_SFX_BTL_KURI_FAIL_LE_802f9384[];
extern const char str_SFX_BTL_KURI_KISS1_802f93fc[];
extern const char str_SFX_BTL_KURI_LEARNED_802f9360[];
extern const char str_SFX_BTL_KURI_LEARNED_802f93ac[];
extern const char str_SFX_MARIO_APPEAL1_802f945c[];
extern const char str_SFX_PARTY_BATTLE_JUM_802f921c[];
extern const char str_SFX_PARTY_BATTLE_LAN_802f9234[];
extern const char str_SFX_PARTY_BATTLE_MOV_802f91ec[];
extern const char str_SFX_PARTY_BATTLE_MOV_802f9204[];
extern const char str_SFX_PARTY_BATTLE_SUB_802f91b4[];
extern const char str_SFX_PARTY_BATTLE_SUB_802f91d0[];
extern const char str_kemuri2_n64_802f93f0[];
extern const char str_kiss_804243bc[];
extern const char str_msg_st_chg_mario_qui_802f9444[];
extern const char str_recovery_802f9410[];
extern const u8 zero_804243b8[];
static u32 damage_event[16];

/* Recovered target Christine .sdata unit metadata prefix. */
static u8 defence[5] = {0, 0, 0, 0, 0};
static u8 defence_attr[5] = {0, 0, 0, 0, 0};
static u8 regist[22] = {
    100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
    100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100
};

/* Recovered target Christine event/event-like arrays. */
static u32 attack_event[2] = {
    0x00000002, 0x00000001
};

static u32 wait_event[6] = {
    0x0003005B, (u32)btlevtcmd_AnimeChangePoseFromTable, 0xFFFFFFFE, 0x00000001, 0x00000002, 0x00000001
};

/* Recovered target Christine pose/data tables. */
u32 pose_table_christine_stay[50] = {
    0x00000009, (u32)(char*)str_PKR_D_5_802f90b8, 0x00000002, (u32)(char*)str_PKR_D_5_802f90b8, 0x00000003, (u32)(char*)str_PKR_X_1_802f90c0,
    0x00000004, (u32)(char*)str_PKR_D_2_802f90c8, 0x00000005, (u32)(char*)str_PKR_X_1_802f90c0, 0x00000001, (u32)(char*)str_PKR_N_1_802f90d0,
    0x0000001B, (u32)(char*)str_PKR_D_3_802f90d8, 0x0000001C, (u32)(char*)str_PKR_S_1_802f90e0, 0x00000025, (u32)(char*)str_PKR_G_2_802f90e8,
    0x0000001F, (u32)(char*)str_PKR_G_1_802f90f0, 0x00000027, (u32)(char*)str_PKR_D_1_802f90f8, 0x00000028, (u32)(char*)str_PKR_W_1_802f9100,
    0x0000002A, (u32)(char*)str_PKR_R_1_802f9108, 0x00000039, (u32)(char*)str_PKR_D_2_802f90c8, 0x00000038, (u32)(char*)str_PKR_D_2_802f90c8,
    0x0000003C, (u32)(char*)str_PKR_D_4_802f9110, 0x0000003D, (u32)(char*)str_PKR_D_2_802f90c8, 0x00000041, (u32)(char*)str_PKR_T_1_802f9118,
    0x00000034, (u32)(char*)str_PKR_I_1_802f9120, 0x00000043, (u32)(char*)str_PKR_C_1_802f9128, 0x00000042, (u32)(char*)str_PKR_O_1_802f9130,
    0x00000040, (u32)(char*)str_PKR_X_1_802f90c0, 0x00000031, (u32)(char*)str_PKR_A_1_802f9138, 0x0000002D, (u32)(char*)str_PKR_G_1_802f90f0,
    0x00000045, (u32)(char*)str_PKR_S_1_802f90e0
};

static u32 data_table[16] = {
    0x00000005, (u32)christine_appeal_event, 0x00000006, (u32)attack_audience_event, 0x00000001, (u32)__makeTechMenuFunc,
    0x00000030, (u32)btldefaultevt_Dummy, 0x00000040, (u32)party_win_reaction, 0x00000007, (u32)subset_attack_ready_jump_event,
    0x0000002F, (u32)btldefaultevt_avoid_counter_event, 0x00000000, 0x00000000
};

/* Recovered target Christine BattleUnitKindPart/BattleUnitKind records. */
u32 unitpartsdata_Party_Christine[19] = {
    0x00000001, (u32)(char*)str_btl_un_christine_802f9140, (u32)(char*)str_c_pkr_804243b0, 0x00000000, 0x00000000, 0x00000000,
    0x41800000, 0x41C00000, 0x40A00000, 0x00000000, 0x41C00000, 0x00000000,
    0x00140018, 0x00FF0000, (u32)defence, (u32)defence_attr, 0x00000009, 0x00000000,
    (u32)pose_table_christine_stay
};

u32 unitdata_Party_Christine[49] = {
    0x000000E0, (u32)(char*)str_btl_un_christine_802f9140, 0x000A0005, 0x05010100, 0x00000064, 0x00090014,
    0x0014000A, 0x00180000, 0x00000000, 0x41400000, 0x00000000, 0x00000000,
    0x00000000, 0x41F00000, 0x00000000, 0x00000000, 0x41F00000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x41200000,
    0x00000000, 0x00000000, 0x41200000, 0x41500000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0000FF00, 0x64140000,
    0x00000000, 0x00000000, 0x00000000, (u32)(char*)str_SFX_PARTY_DAMAGE1_802f9154, (u32)(char*)str_SFX_MARIO_DAMAGE_FIR_802f9168, (u32)(char*)str_SFX_MARIO_DAMAGE_ICE_802f9180,
    (u32)(char*)str_SFX_MARIO_DAMAGE_BIR_802f9198, 0x02000000, (u32)regist, 0x01000000, (u32)unitpartsdata_Party_Christine, (u32)init_event,
    (u32)data_table
};

static u32 battle_entry_event[59] = {
    0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE, 0xFE363C80, 0x000000FA, 0x0005005B,
    (u32)btlevtcmd_SetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00010009,
    0x00000028, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001, 0x0000002A,
    0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE, 0xF24A8A80, 0x0009005B, (u32)btlevtcmd_SetMoveSoundDataWork,
    0xFFFFFFFE, (u32)(char*)str_SFX_PARTY_BATTLE_SUB_802f91b4, (u32)(char*)str_SFX_PARTY_BATTLE_SUB_802f91d0, 0x00000000, 0x00000009, 0x00000003,
    0x00000000, 0x00000000, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000000, 0xFFFFFFFE, 0x00000000, 0x0003005B,
    (u32)btlevtcmd_AnimeChangePoseFromTable, 0xFFFFFFFE, 0x00000001, 0x00000002, 0x00000001
};

static u32 init_event[51] = {
    0x0003005B, (u32)btlevtcmd_SetEventWait, 0xFFFFFFFE, (u32)wait_event, 0x0003005B, (u32)btlevtcmd_SetEventAttack,
    0xFFFFFFFE, (u32)attack_event, 0x0003005B, (u32)btlevtcmd_SetEventDamage, 0xFFFFFFFE, (u32)damage_event,
    0x0003005B, (u32)btlevtcmd_SetEventConfusion, 0xFFFFFFFE, (u32)btldefaultevt_Confuse, 0x0007005B, (u32)btlevtcmd_SetWalkSound,
    0xFFFFFFFE, (u32)(char*)str_SFX_PARTY_BATTLE_MOV_802f91ec, (u32)(char*)str_SFX_PARTY_BATTLE_MOV_802f9204, 0x00000006, 0x0000000C, 0x00000010,
    0x0007005B, (u32)btlevtcmd_SetRunSound, 0xFFFFFFFE, (u32)(char*)str_SFX_PARTY_BATTLE_MOV_802f91ec, (u32)(char*)str_SFX_PARTY_BATTLE_MOV_802f9204, 0x00000000,
    0x00000009, 0x00000003, 0x0004005B, (u32)btlevtcmd_SetJumpSound, 0xFFFFFFFE, (u32)(char*)str_SFX_PARTY_BATTLE_JUM_802f921c,
    (u32)(char*)str_SFX_PARTY_BATTLE_LAN_802f9234, 0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000000, 0x00000000,
    0x0003005B, (u32)btlevtcmd_SetEventEntry, 0xFFFFFFFE, (u32)battle_entry_event, 0x0002005B, (u32)btlevtcmd_StartWaitEvent,
    0xFFFFFFFE, 0x00000002, 0x00000001
};

static u32 damage_event[16] = {
    0x0005005B, (u32)btlevtcmd_SetRotate, 0xFFFFFFFE, 0x00000000, 0x00000000, 0x00000000,
    0x00020032, 0xFE363C8A, 0xFFFFFFFE, 0x00020032, 0xFE363C8B, 0x00000001,
    0x0001005E, (u32)btldefaultevt_Damage, 0x00000002, 0x00000001
};

/* Recovered target Christine BattleWeapon records. */
u32 partyWeapon_ChristineNormalAttack[48] = {
    (u32)(char*)str_btl_wn_pkr_normal_802f9250, 0x01580000, 0x00000000, (u32)(char*)str_msg_pkr_normal_jump_802f9264, 0x64000000, 0x3F800000,
    0x01016432, (u32)weaponGetPowerFromPartyAttackLv, 0x00000001, 0x00000001, 0x00000002, 0x00000002,
    0x00000003, 0x00000003, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01001260, 0x20001004, 0x00000302, (u32)(char*)str_msg_ac_zutsuki_802f9278, 0x00035007,
    0x0000000C, 0x80002004, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)partyChristineAttack_NormalAttack, 0x00000064, 0x00040200, 0x00000000
};

u32 partyWeapon_ChristineMonosiri[48] = {
    (u32)(char*)str_btl_wn_pkr_lv1_802f9288, 0x01590000, 0x00000000, (u32)(char*)str_msg_pkr_monosiri_802f9298, 0x64000000, 0x3F800000,
    0x01056432, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01101260, 0x20000001, 0x00000302, (u32)(char*)str_msg_ac_monoshiri_802f92ac, 0x00020008,
    0x000007FF, 0x80002000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)partyChristineAttack_Monosiri, 0x00000064, 0x00000000, 0x00000000
};

u32 partyWeapon_ChristineRenzokuAttack[48] = {
    (u32)(char*)str_btl_wn_pkr_lv2_802f92c0, 0x015A0000, 0x00000000, (u32)(char*)str_msg_pkr_renzoku_zuts_802f92d0, 0x64030000, 0x3F800000,
    0x01056432, (u32)weaponGetPowerFromPartyAttackLv, 0x00000001, 0x00000001, 0x00000002, 0x00000002,
    0x00000003, 0x00000003, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01001260, 0x20001004, 0x00000302, (u32)(char*)str_msg_ac_zutsuki_802f9278, 0x00025017,
    0x0000000C, 0x80002000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)partyChristineAttack_RenzokuAttack, 0x00000064, 0x000A0500, 0x00050000
};

u32 partyWeapon_ChristineKiss[48] = {
    (u32)(char*)str_btl_wn_pkr_lv3_802f92e8, 0x015B0000, 0x00000000, (u32)(char*)str_msg_pkr_nage_kiss_802f92f8, 0x64040000, 0x3F800000,
    0x01056432, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01101160, 0x10000000, 0x00000302, (u32)(char*)str_msg_ac_kiss_802f930c, 0x00020108,
    0x000007FF, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x64020000, 0x00000000,
    0x00000000, 0x00000000, (u32)partyChristineAttack_Kiss, 0x00000064, 0x000A0500, 0x00000000
};

u32 partyChristineAttack_NormalAttack[824] = {
    0x0005005B, (u32)btlevtcmd_JumpSetting, 0xFFFFFFFE, 0x00000014, 0xF24A7A80, 0xF24A7D4C,
    0x0003005B, (u32)btlevtcmd_GetSelectEnemy, 0xFE363C83, 0xFE363C84, 0x00020018, 0xFE363C83,
    0xFFFFFFFF, 0x00010004, 0x00000063, 0x00000021, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress,
    0xFFFFFFFE, 0xFE363C8C, 0x0002005B, (u32)btlevtcmd_WeaponAftereffect, 0xFE363C8C, 0x0004005B,
    (u32)btlevtcmd_AttackDeclare, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0x0001005B, (u32)btlevtcmd_WaitGuardMove,
    0x0002005B, (u32)btlevtcmd_CommandPayWeaponCost, 0xFFFFFFFE, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFFFFFFFE,
    0x00000007, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x0000000B, 0x0006005B,
    (u32)evt_btl_camera_set_homing_unitparts, 0x00000000, 0xFFFFFFFE, 0x00000001, 0xFE363C83, 0xFE363C84,
    0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001, 0x0003005B, (u32)evt_btl_camera_set_zoom,
    0x00000000, 0x000000FA, 0x0002005B, (u32)btlevtcmd_CommandGetWeaponActionLv, 0xFE363C80, 0x0003005B,
    (u32)btlevtcmd_AcSetDifficulty, 0xFFFFFFFE, 0xFE363C80, 0x0005005B, (u32)btlevtcmd_SetupAC, 0xFFFFFFFE,
    0x00000001, 0x00000001, 0x00000000, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE,
    0x00000001, 0x0000002A, 0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE, 0xF24A8680,
    0x0007005B, (u32)btlevtcmd_GetTakeoffPosition, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0007005B, (u32)btlevtcmd_CalculateFaceDirection, 0xFFFFFFFE, 0xFFFFFFFF,
    0xFE363C80, 0xFE363C82, 0x00000001, 0xFE363C8F, 0x0003005B, (u32)btlevtcmd_ChangeFaceDirection,
    0xFFFFFFFE, 0xFE363C8F, 0x0007005B, (u32)btlevtcmd_GetMoveFrame, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000000, 0xFE363C8E, 0x0008005B, (u32)btlevtcmd_MovePosition,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0xFE363C8E, 0xFFFFFFFF,
    0x00000000, 0x0007005B, (u32)btlevtcmd_CalculateFaceDirection, 0xFFFFFFFE, 0xFFFFFFFF, 0xFE363C83,
    0xFE363C84, 0x00000010, 0xFE363C8F, 0x0003005B, (u32)btlevtcmd_ChangeFaceDirection, 0xFFFFFFFE,
    0xFE363C8F, 0x0002005B, (u32)btlevtcmd_StartAC, 0x00000001, 0x00020032, 0xFE363C8A,
    0x00000000, 0x00010003, 0x0000000A, 0x0002001D, 0xFE363C8A, 0x00000001,
    0x0006005B, (u32)btlevtcmd_CommandPreCheckDamage, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0x00100100,
    0xFE363C85, 0x00000020, 0x0006005B, (u32)btlevtcmd_CommandPreCheckDamage, 0xFFFFFFFE, 0xFE363C83,
    0xFE363C84, 0x00000100, 0xFE363C85, 0x00000021, 0x00010022, 0xFE363C85,
    0x0001002B, 0x00000002, 0x0001002B, 0x00000003, 0x0001002B, 0x00000006,
    0x0001002B, 0x00000004, 0x0000002E, 0x00010024, 0x00000001, 0x0000002A,
    0x0001000A, 0x000003E8, 0x00000031, 0x0002001C, 0xFE363C8A, 0x00000000,
    0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PKR_A_2B_802f9318, 0x0000006E,
    0x00010009, 0x00000002, 0x0006005B, (u32)btlevtcmd_ACRStart, 0xFFFFFFFE, 0x00000000,
    0x00000007, 0x00000007, 0x00000000, 0x00000070, 0x00010009, 0x0000000A,
    0x00000021, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PKR_J_1A_802f9324,
    0x0000006B, 0x00010009, 0x0000000F, 0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE,
    (u32)(char*)str_SFX_BTL_KURI_ATTACK1_802f9330, 0xF1194D80, 0x00000000, 0xF1194D80, 0x0000006D, 0x0005005B,
    (u32)btlevtcmd_JumpSetting, 0xFFFFFFFE, 0x00000014, 0xF24A7A80, 0xF24A7D4C, 0x0002001D,
    0xFE363C8A, 0x00000001, 0x0006005B, (u32)btlevtcmd_GetHitPos, 0xFE363C83, 0xFE363C84,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00020035, 0xFE363C82, 0x00000005,
    0x0005005B, (u32)btlevtcmd_SetRotateOffset, 0xFFFFFFFE, 0x00000000, 0x0000000E, 0x00000000,
    0x0005005B, (u32)btlevtcmd_SetRotate, 0xFFFFFFFE, 0x00000000, 0x00000000, 0x00000000,
    0x0000006E, 0x00010009, 0x0000000F, 0x00010005, 0x00000006, 0x0003005B,
    (u32)btlevtcmd_GetFaceDirection, 0xFFFFFFFE, 0xFE363C80, 0x00020037, 0xFE363C80, 0xFFFFFFE2,
    0x0005005B, (u32)btlevtcmd_AddRotate, 0xFFFFFFFE, 0x00000000, 0x00000000, 0xFE363C80,
    0x00010009, 0x00000001, 0x00000006, 0x00000070, 0x0006005B, (u32)btlevtcmd_snd_se,
    0xFFFFFFFE, (u32)(char*)str_SFX_PARTY_BATTLE_JUM_802f921c, 0xF1194D80, 0x00000000, 0xF1194D80, 0x0008005B,
    (u32)btlevtcmd_MarioJumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000024,
    0x00000003, 0x00000000, 0x00000020, 0x0005005B, (u32)btlevtcmd_SetRotateOffset, 0xFFFFFFFE,
    0x00000000, 0x0000000E, 0x00000000, 0x0003005B, (u32)btlevtcmd_ACRGetResult, 0xFE363C86,
    0xFE363C87, 0x00020018, 0xFE363C86, 0x00000002, 0x0000006E, 0x0003005B,
    (u32)btlevtcmd_CommandGetWeaponAddress, 0xFFFFFFFE, 0xFE363C80, 0x0006005B, (u32)btlevtcmd_AudienceDeclareAcrobatResult, 0xFE363C80,
    0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x00000070, 0x0000006E,
    0x0007005B, (u32)btlevtcmd_CalculateFaceDirection, 0xFFFFFFFE, 0xFFFFFFFF, 0xFE363C83, 0xFE363C84,
    0x00000010, 0xFE363C8F, 0x00020018, 0xFE363C8F, 0x00000001, 0x00020032,
    0xFE363C89, 0xFFFFFFF1, 0x00000020, 0x00020032, 0xFE363C89, 0x0000000F,
    0x00000021, 0x0005005B, (u32)btlevtcmd_GetPosFloat, 0xFFFFFFFE, 0xFE363C85, 0xFE363C86,
    0xFE363C87, 0x00020032, 0xFE363C88, 0x00000000, 0x00010005, 0x00000024,
    0x0005005B, (u32)btlevtcmd_AddRotate, 0xFFFFFFFE, 0x00000000, 0x00000000, 0xFE363C89,
    0x00010009, 0x00000001, 0x00000006, 0x00000070, 0x00000020, 0x0000006E,
    0x0005005B, (u32)btlevtcmd_GetPosFloat, 0xFFFFFFFE, 0xFE363C85, 0xFE363C86, 0xFE363C87,
    0x00020032, 0xFE363C88, 0x00000000, 0x00010005, 0x00000024, 0x0005005B,
    (u32)btlevtcmd_GetPosFloat, 0xFFFFFFFE, 0xFE363C8C, 0xFE363C8D, 0xFE363C8E, 0x0006005B,
    (u32)krb_get_dir, 0xFE363C85, 0xFE363C86, 0xFE363C8C, 0xFE363C8D, 0xFE363C88,
    0x0005005B, (u32)btlevtcmd_GetRotate, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0005005B, (u32)btlevtcmd_SetRotate, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C88,
    0x00020034, 0xFE363C85, 0xFE363C8C, 0x00020034, 0xFE363C86, 0xFE363C8D,
    0x00010009, 0x00000001, 0x00000006, 0x00000070, 0x00000021, 0x0006005B,
    (u32)btlevtcmd_GetHitPos, 0xFE363C83, 0xFE363C84, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00020035, 0xFE363C82, 0x00000005, 0x00020032, 0xFE363C88, 0x00000024,
    0x0006005B, (u32)btlevtcmd_MarioJumpParam, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0xFE363C88, 0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_PARTY_BATTLE_JUM_802f921c, 0xF1194D80,
    0x00000000, 0xF1194D80, 0x0008005B, (u32)btlevtcmd_MarioJumpPosition, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0xFE363C88, 0x00000000, 0x00000000, 0x00000021,
    0x00020019, 0xFE363C85, 0x00000001, 0x00020018, 0xFE363C85, 0x00000003,
    0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000026, 0x00000021, 0x00020018,
    0xFE363C85, 0x00000006, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000027,
    0x00000021, 0x00020018, 0xFE363C85, 0x00000002, 0x0003005B, (u32)btlevtcmd_StartAvoid,
    0xFE363C83, 0x00000028, 0x00000021, 0x0001005B, (u32)btlevtcmd_StopAC, 0x0002005B,
    (u32)btlevtcmd_JumpContinue, 0xFFFFFFFE, 0x0005005B, (u32)btlevtcmd_SetRotateOffset, 0xFFFFFFFE, 0x00000000,
    0x00000000, 0x00000000, 0x0005005B, (u32)btlevtcmd_SetRotate, 0xFFFFFFFE, 0x00000000,
    0x00000000, 0x00000000, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001,
    (u32)(char*)str_PKR_S_1_802f90e0, 0x0003005B, (u32)btlevtcmd_SetFallAccel, 0xFFFFFFFE, 0xF24A7D4C, 0x0005005B,
    (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B,
    (u32)btlevtcmd_MoveDirectionAdd, 0xFFFFFFFE, 0xFE363C80, 0x0000001E, 0x0007005B, (u32)btlevtcmd_JumpPosition,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000014, 0xFFFFFFFF,
    0x0004005B, (u32)btlevtcmd_MoveDirectionAdd, 0xFFFFFFFE, 0xFE363C80, 0x00000014, 0x0007005B,
    (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0000000A,
    0xFFFFFFFF, 0x0001000A, 0x000001F4, 0x00010004, 0x0000005F, 0x00000021,
    0x0003005B, (u32)btlevtcmd_ac_timing_flag_onoff, 0x00000001, 0x00000001, 0x0002005B, (u32)btlevtcmd_ac_timing_get_success_frame,
    0xFE363C80, 0x00020038, 0xFE363C80, 0x00000002, 0x00010009, 0xFE363C80,
    0x0003005B, (u32)btlevtcmd_ac_timing_flag_onoff, 0x00000000, 0x00000001, 0x0001005B, (u32)btlevtcmd_ResultAC,
    0x0005005B, (u32)btlevtcmd_SetScale, 0xFFFFFFFE, 0xF24A7E80, 0xF24A7E80, 0xF24A7E80,
    0x0002005B, (u32)btlevtcmd_GetResultAC, 0xFE363C86, 0x0002001F, 0xFE363C86, 0x00000002,
    0x0006005B, (u32)btlevtcmd_CommandCheckDamage, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0x00000100,
    0xFE363C85, 0x00020018, 0xFE363C8A, 0x00000000, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress,
    0xFFFFFFFE, 0xFE363C80, 0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult, 0xFE363C80, 0xFFFFFFFF,
    0x00000021, 0x00010004, 0x0000005A, 0x00000021, 0x0002005B, (u32)btlevtcmd_GetResultCountAC,
    0xFE363C86, 0x0002001C, 0xFE363C86, 0x00000001, 0x0000006B, 0x0003005B,
    (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000000, 0x0003005B, (u32)evt_btl_camera_set_zoom, 0x00000000,
    0x000000C8, 0x00010009, 0x0000000A, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000,
    0x00000002, 0x0003005B, (u32)evt_btl_camera_set_zoom, 0x00000000, 0x000000FA, 0x0000006D,
    0x0006005B, (u32)btlevtcmd_CommandCheckDamage, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0x00020000,
    0xFE363C85, 0x0004005B, (u32)btlevtcmd_GetResultPrizeLv, 0xFE363C83, 0x00000000, 0xFE363C86,
    0x0006005B, (u32)btlevtcmd_GetHitPos, 0xFE363C83, 0xFE363C84, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0005005B, (u32)btlevtcmd_ACSuccessEffect, 0xFE363C86, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress, 0xFFFFFFFE, 0xFE363C80, 0x0003005B,
    (u32)btlevtcmd_AudienceDeclareACResult, 0xFE363C80, 0xFE363C86, 0x00020035, 0xFE363C8A, 0x00000001,
    0x00010004, 0x0000000A, 0x00000020, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000,
    0x00000000, 0x0006005B, (u32)btlevtcmd_CommandCheckDamage, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84,
    0x00020100, 0xFE363C85, 0x0002005B, (u32)btlevtcmd_GetResultAC, 0xFE363C80, 0x00000021,
    0x00010003, 0x0000005A, 0x0001005B, (u32)btlevtcmd_StopAC, 0x0003005B, (u32)evt_btl_camera_set_mode,
    0x00000000, 0x00000000, 0x0001005B, (u32)evt_audience_ap_recovery, 0x0001005B, (u32)btlevtcmd_InviteApInfoReport,
    0x0000006E, 0x0005005B, (u32)btlevtcmd_GetRotate, 0xFFFFFFFE, 0xF1194D80, 0xF1194D80,
    0xFE363C86, 0x0002001A, 0xFE363C86, 0x00000000, 0x00020037, 0xFE363C86,
    0xFFFFFFFF, 0x00000021, 0x0003005B, (u32)btlevtcmd_GetFaceDirection, 0xFFFFFFFE, 0xFE363C80,
    0x00020037, 0xFE363C86, 0xFE363C80, 0x00020038, 0xFE363C86, 0x0000000F,
    0x00020032, 0xFE363C87, 0x0000000F, 0x00010003, 0x00000014, 0x0005005B,
    (u32)btlevtcmd_AddRotate, 0xFFFFFFFE, 0x00000000, 0x00000000, 0xFE363C86, 0x00020036,
    0xFE363C87, 0x00000001, 0x00010009, 0x00000001, 0x0002001D, 0xFE363C87,
    0x00000001, 0x00010004, 0x00000014, 0x00000021, 0x0005005B, (u32)btlevtcmd_SetRotate,
    0xFFFFFFFE, 0x00000000, 0x00000000, 0x00000000, 0x00000070, 0x0003005B,
    (u32)btlevtcmd_SetFallAccel, 0xFFFFFFFE, 0xF24A7D4C, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE,
    0xF1194D80, 0xFE363C81, 0xF1194D80, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE,
    0xFE363C80, 0x0000003C, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0000001E, 0xFFFFFFFF, 0x0003005B, (u32)btlevtcmd_SetFallAccel,
    0xFFFFFFFE, 0xF24A7CE6, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE, 0xFE363C80,
    0x00000014, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0000000C, 0xFFFFFFFF, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE,
    0xFE363C80, 0x00000008, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000008, 0xFFFFFFFF, 0x00010003, 0x0000005F,
    0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv,
    0x00000000, 0x00000001, 0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE, 0xF24A9280,
    0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001, 0x00000028, 0x0005005B,
    (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0008005B,
    (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000,
    0xFFFFFFFF, 0x00000000, 0x00010003, 0x00000063, 0x0002005B, (u32)btlevtcmd_ResetFaceDirection,
    0xFFFFFFFE, 0x0001005E, (u32)btldefaultevt_SuitoruBadgeEffect, 0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE,
    0x00000002, 0x00000001
};

u32 christine_dictionary_event[26] = {
    0x00010003, 0x00000000, 0x0003005B, (u32)_dictionary, 0xFE363C83, 0xFE363C84,
    0x0004005B, (u32)btlevtcmd_GetUnitWork, 0xFFFFFFFE, 0x00000000, 0xFE363C80, 0x00020019,
    0xFE363C80, 0x00000006, 0x00010009, 0x00000001, 0x00010004, 0x00000000,
    0x00000021, 0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000000, 0x00000000,
    0x00000002, 0x00000001
};

u32 partyChristineAttack_Monosiri[397] = {
    0x0005005B, (u32)btlevtcmd_JumpSetting, 0xFFFFFFFE, 0x00000014, 0xF24A7A80, 0xF24A7D4C,
    0x0003005B, (u32)btlevtcmd_GetSelectEnemy, 0xFE363C83, 0xFE363C84, 0x00020018, 0xFE363C83,
    0xFFFFFFFF, 0x00010004, 0x00000063, 0x00000021, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress,
    0xFFFFFFFE, 0xFE363C8C, 0x0002005B, (u32)btlevtcmd_WeaponAftereffect, 0xFE363C8C, 0x0002005B,
    (u32)btlevtcmd_CommandPayWeaponCost, 0xFFFFFFFE, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFFFFFFFE, 0x00000007,
    0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000007, 0x0004005B, (u32)evt_btl_camera_set_homing_unit,
    0x00000000, 0xFFFFFFFE, 0xFFFFFFFE, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000,
    0x00000001, 0x0003005B, (u32)evt_btl_camera_set_zoom, 0x00000000, 0x0000015E, 0x0005005B,
    (u32)evt_btl_camera_set_posoffset, 0x00000000, 0x00000028, 0x00000000, 0x00000000, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PKR_2A_1_802f9348, 0x0001000A, 0x000001F4,
    0x00010005, 0x00000002, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001,
    (u32)(char*)str_PKR_2A_2_802f9354, 0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_BTL_KURI_LEARNED_802f9360, 0xF1194D80,
    0x00000000, 0xF1194D80, 0x0001000A, 0x00000190, 0x0004005B, (u32)btlevtcmd_AnimeChangePose,
    0xFFFFFFFE, 0x00000001, (u32)(char*)str_PKR_2A_3_802f9378, 0x0001000A, 0x0000015E, 0x00000006,
    0x0002005B, (u32)btlevtcmd_CommandGetWeaponActionLv, 0xFE363C80, 0x0003005B, (u32)btlevtcmd_AcSetDifficulty, 0xFFFFFFFE,
    0xFE363C80, 0x0009005B, (u32)btlevtcmd_AcSetParamAll, 0xFE363C83, 0xFE363C84, 0xF1194D80,
    0xF1194D80, 0xF1194D80, 0xF1194D80, 0xF1194D80, 0xF1194D80, 0x0002005B,
    (u32)btlevtcmd_AcSetFlag, 0x00000000, 0x0005005B, (u32)btlevtcmd_SetupAC, 0xFFFFFFFE, 0x00000015,
    0x00000001, 0x00000000, 0x0000006E, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000,
    0x00000000, 0x0005005B, (u32)evt_btl_camera_set_posoffset, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000070, 0x00010009, 0x00000016, 0x0002005B, (u32)btlevtcmd_StartAC,
    0x00000001, 0x0001005B, (u32)btlevtcmd_ResultAC, 0x0002005B, (u32)btlevtcmd_GetResultAC, 0xFE363C86,
    0x0002001E, 0xFE363C86, 0x00000002, 0x0004005B, (u32)btlevtcmd_GetResultPrizeLv, 0xFFFFFFFB,
    0x00000000, 0xFE363C86, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress, 0xFFFFFFFE, 0xFE363C80,
    0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult, 0xFE363C80, 0xFE363C86, 0x0006005B, (u32)btlevtcmd_GetHitPos,
    0xFE363C83, 0xFE363C84, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B,
    (u32)btlevtcmd_ACSuccessEffect, 0xFE363C86, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000020,
    0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress, 0xFFFFFFFE, 0xFE363C80, 0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult,
    0xFE363C80, 0xFFFFFFFF, 0x00000021, 0x0002005B, (u32)btlevtcmd_GetResultAC, 0xFE363C86,
    0x0002001F, 0xFE363C86, 0x00000002, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000C005B, (u32)evt_eff_fukidashi, 0x00000002,
    (u32)(u8*)zero_804243b8, 0x00000000, 0x00000002, 0x00000000, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0000001E, 0x00000000, 0x00000038, 0x0006005B, (u32)btlevtcmd_snd_se,
    0xFFFFFFFE, (u32)(char*)str_SFX_BTL_KURI_FAIL_LE_802f9384, 0xF1194D80, 0x00000000, 0xF1194D80, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PKR_2A_4_802f93a0, 0x00010009, 0x00000038,
    0x00010004, 0x0000005A, 0x00000021, 0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE,
    (u32)(char*)str_SFX_BTL_KURI_LEARNED_802f93ac, 0xF1194D80, 0x00000000, 0xF1194D80, 0x0005005B, (u32)btlevtcmd_GetPos,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B, (u32)btlevtcmd_AnimeChangePose,
    0xFFFFFFFE, 0x00000001, (u32)(char*)str_PKR_2A_4_802f93a0, 0x00010009, 0x00000003, 0x0001006F,
    0xFE363C8F, 0x00010009, 0x00000035, 0x00000070, 0x0003005B, (u32)btlevtcmd_ftof,
    0x00000000, 0xFE363C80, 0x0003005B, (u32)btlevtcmd_ftof, 0x00000035, 0xFE363C81,
    0x0006005B, (u32)btlevtcmd_ACRStart, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C81,
    0x00000000, 0x0003005B, (u32)btlevtcmd_ACRGetResult, 0xFE363C86, 0xFE363C87, 0x0002001D,
    0xFE363C86, 0x00000002, 0x0001005F, 0xFE363C8F, 0x0006005B, (u32)btlevtcmd_AudienceDeclareAcrobatResult,
    0xFE363C8C, 0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PKR_Y_1_802f93c4, 0x0003005B, (u32)btlevtcmd_ftomsec,
    0x0000001C, 0xFE363C80, 0x0001000A, 0xFE363C80, 0x00000020, 0x0001005B,
    (u32)evt_audience_acrobat_notry, 0x0002005B, (u32)btlevtcmd_WaitEventEnd, 0xFE363C8F, 0x00000021, 0x0003005B,
    (u32)btlevtcmd_SetTalkPose, 0xFFFFFFFE, (u32)(char*)str_PKR_2A_5_802f93cc, 0x0003005B, (u32)btlevtcmd_SetStayPose, 0xFFFFFFFE,
    (u32)(char*)str_PKR_2A_5_802f93cc, 0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000000, 0x00000000,
    0x0001005C, (u32)christine_dictionary_event, 0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000000,
    0x00000001, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000, 0x0003005B,
    (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001, 0x0002005B, (u32)evt_btl_camera_off_posoffset_manual, 0x00000000,
    0x00010003, 0x0000000A, 0x0004005B, (u32)btlevtcmd_GetUnitWork, 0xFFFFFFFE, 0x00000000,
    0xFE363C80, 0x00020019, 0xFE363C80, 0x00000003, 0x00010009, 0x00000001,
    0x00010004, 0x0000000A, 0x00000021, 0x0004005B, (u32)btlevtcmd_get_monosiri_msg_no, 0xFE363C83,
    0xFE363C84, 0xFE363C80, 0x0005005B, (u32)evt_msg_print, 0x00000002, 0xFE363C80,
    0x00000000, 0xFFFFFFFE, 0x0002005B, (u32)_monosiri_flag_on, 0xFE363C83, 0x0003005B,
    (u32)btlevtcmd_SetTalkPose, 0xFFFFFFFE, (u32)(char*)str_PKR_T_1_802f9118, 0x0003005B, (u32)btlevtcmd_SetStayPose, 0xFFFFFFFE,
    (u32)(char*)str_PKR_S_1_802f90e0, 0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000000, 0x00000004,
    0x00010003, 0x00000014, 0x0004005B, (u32)btlevtcmd_GetUnitWork, 0xFFFFFFFE, 0x00000000,
    0xFE363C80, 0x00020019, 0xFE363C80, 0x00000000, 0x00010009, 0x00000001,
    0x00010004, 0x00000014, 0x00000021, 0x00010003, 0x0000005A, 0x0003005B,
    (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000,
    0x00000001, 0x0002005B, (u32)evt_btl_camera_off_posoffset_manual, 0x00000000, 0x0001005B, (u32)evt_audience_ap_recovery,
    0x0001005B, (u32)btlevtcmd_InviteApInfoReport, 0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002,
    0x00000001
};

u32 partyChristineAttack_RenzokuAttack[1078] = {
    0x0005005B, (u32)btlevtcmd_JumpSetting, 0xFFFFFFFE, 0x00000014, 0xF24A7A80, 0xF24A7D4C,
    0x0003005B, (u32)btlevtcmd_GetSelectEnemy, 0xFE363C83, 0xFE363C84, 0x00020018, 0xFE363C83,
    0xFFFFFFFF, 0x00010004, 0x00000063, 0x00000021, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress,
    0xFFFFFFFE, 0xFE363C8C, 0x0002005B, (u32)btlevtcmd_WeaponAftereffect, 0xFE363C8C, 0x0004005B,
    (u32)btlevtcmd_AttackDeclare, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0x0001005B, (u32)btlevtcmd_WaitGuardMove,
    0x0002005B, (u32)btlevtcmd_CommandPayWeaponCost, 0xFFFFFFFE, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFFFFFFFE,
    0x00000007, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x0000000B, 0x0006005B,
    (u32)evt_btl_camera_set_homing_unitparts, 0x00000000, 0xFFFFFFFE, 0x00000001, 0xFE363C83, 0xFE363C84,
    0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001, 0x0003005B, (u32)evt_btl_camera_set_zoom,
    0x00000000, 0x000000FA, 0x0002005B, (u32)evt_btl_camera_off_posoffset_manual, 0x00000000, 0x0002005B,
    (u32)btlevtcmd_CommandGetWeaponActionLv, 0xFE363C80, 0x0003005B, (u32)btlevtcmd_AcSetDifficulty, 0xFFFFFFFE, 0xFE363C80,
    0x0005005B, (u32)btlevtcmd_SetupAC, 0xFFFFFFFE, 0x00000001, 0x00000001, 0x00000000,
    0x0003005B, (u32)btlevtcmd_ac_timing_flag_onoff, 0x00000001, 0x00000002, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType,
    0xFFFFFFFE, 0x00000001, 0x0000002A, 0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE,
    0xF24A8680, 0x0007005B, (u32)btlevtcmd_GetTakeoffPosition, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0007005B, (u32)btlevtcmd_CalculateFaceDirection, 0xFFFFFFFE,
    0xFFFFFFFF, 0xFE363C80, 0xFE363C82, 0x00000001, 0xFE363C8F, 0x0003005B,
    (u32)btlevtcmd_ChangeFaceDirection, 0xFFFFFFFE, 0xFE363C8F, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000, 0xFFFFFFFF, 0x00000000,
    0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PKR_A_2B_802f9318, 0x0006005B,
    (u32)btlevtcmd_ACRStart, 0xFFFFFFFE, 0x00000000, 0x0000000A, 0x0000000A, 0x00000000,
    0x00010009, 0x0000000A, 0x0007005B, (u32)btlevtcmd_CalculateFaceDirection, 0xFFFFFFFE, 0xFFFFFFFF,
    0xFE363C83, 0xFE363C84, 0x00000010, 0xFE363C8F, 0x0003005B, (u32)btlevtcmd_ChangeFaceDirection,
    0xFFFFFFFE, 0xFE363C8F, 0x0002005B, (u32)btlevtcmd_StartAC, 0x00000001, 0x00020032,
    0xFE363C8A, 0x00000000, 0x00010003, 0x0000000A, 0x0002001D, 0xFE363C8A,
    0x00000001, 0x0006005B, (u32)btlevtcmd_CommandPreCheckDamage, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84,
    0x00100100, 0xFE363C85, 0x00000020, 0x0006005B, (u32)btlevtcmd_CommandPreCheckDamage, 0xFFFFFFFE,
    0xFE363C83, 0xFE363C84, 0x00000100, 0xFE363C85, 0x00000021, 0x00010022,
    0xFE363C85, 0x0001002B, 0x00000002, 0x0001002B, 0x00000003, 0x0001002B,
    0x00000006, 0x0001002B, 0x00000004, 0x0000002E, 0x00010024, 0x00000001,
    0x0000002A, 0x0001000A, 0x000003E8, 0x00000031, 0x0004005B, (u32)btlevtcmd_AnimeChangePose,
    0xFFFFFFFE, 0x00000001, (u32)(char*)str_PKR_J_1A_802f9324, 0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE,
    0x00000003, 0x00000000, 0x0002001D, 0xFE363C8A, 0x00000001, 0x0005005B,
    (u32)btlevtcmd_SetRotateOffset, 0xFFFFFFFE, 0x00000000, 0x0000000E, 0x00000000, 0x0005005B,
    (u32)btlevtcmd_SetRotate, 0xFFFFFFFE, 0x00000000, 0x00000000, 0x00000000, 0x0000006E,
    0x00010009, 0x0000000F, 0x00010005, 0x00000006, 0x0003005B, (u32)btlevtcmd_GetFaceDirection,
    0xFFFFFFFE, 0xFE363C80, 0x00020037, 0xFE363C80, 0xFFFFFFE2, 0x0005005B,
    (u32)btlevtcmd_AddRotate, 0xFFFFFFFE, 0x00000000, 0x00000000, 0xFE363C80, 0x00010009,
    0x00000001, 0x00000006, 0x00000070, 0x0000006B, 0x00010009, 0x0000000F,
    0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_BTL_KURI_ATTACK2_802f93d8, 0xF1194D80, 0x00000000,
    0xF1194D80, 0x0000006D, 0x0006005B, (u32)btlevtcmd_GetHitPos, 0xFE363C83, 0xFE363C84,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00020035, 0xFE363C82, 0x00000005,
    0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_PARTY_BATTLE_JUM_802f921c, 0xF1194D80, 0x00000000,
    0xF1194D80, 0x0008005B, (u32)btlevtcmd_MarioJumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000024, 0x00000003, 0x00000000, 0x00000020, 0x0003005B,
    (u32)btlevtcmd_ACRGetResult, 0xFE363C86, 0xFE363C87, 0x0006005B, (u32)btlevtcmd_GetHitPos, 0xFE363C83,
    0xFE363C84, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00020035, 0xFE363C82,
    0x00000005, 0x00020032, 0xFE363C88, 0x00000028, 0x0006005B, (u32)btlevtcmd_MarioJumpParam,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0xFE363C88, 0x0005005B,
    (u32)btlevtcmd_SetRotateOffset, 0xFFFFFFFE, 0x00000000, 0x0000000E, 0x00000000, 0x0000006E,
    0x0002001D, 0xFE363C86, 0x00000002, 0x0006005B, (u32)btlevtcmd_AudienceDeclareAcrobatResult, 0xFE363C8C,
    0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x00010009, 0x00000014,
    0x00000021, 0x0005005B, (u32)btlevtcmd_GetPosFloat, 0xFFFFFFFE, 0xFE363C85, 0xFE363C86,
    0xFE363C87, 0x00020032, 0xFE363C8F, 0x00000000, 0x00010005, 0xFE363C88,
    0x0005005B, (u32)btlevtcmd_GetPosFloat, 0xFFFFFFFE, 0xFE363C8C, 0xFE363C8D, 0xFE363C8E,
    0x0006005B, (u32)krb_get_dir, 0xFE363C85, 0xFE363C86, 0xFE363C8C, 0xFE363C8D,
    0xFE363C8F, 0x0005005B, (u32)btlevtcmd_GetRotate, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0004005B, (u32)btlevtcmd_GetUnitWork, 0xFFFFFFFE, 0x00000003, 0xFE363C89,
    0x00020035, 0xFE363C8F, 0xFE363C89, 0x0005005B, (u32)btlevtcmd_SetRotate, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C8F, 0x00020034, 0xFE363C85, 0xFE363C8C,
    0x00020034, 0xFE363C86, 0xFE363C8D, 0x00010009, 0x00000001, 0x00000006,
    0x00000070, 0x0000006B, 0x0002001D, 0xFE363C86, 0x00000002, 0x00010009,
    0x00000014, 0x00000021, 0x00010009, 0x0000000F, 0x0006005B, (u32)btlevtcmd_snd_se,
    0xFFFFFFFE, (u32)(char*)str_SFX_BTL_KURI_ATTACK2_802f93d8, 0xF1194D80, 0x00000000, 0xF1194D80, 0x0000006D,
    0x0002001D, 0xFE363C86, 0x00000002, 0x0000006E, 0x0003005B, (u32)btlevtcmd_GetFaceDirection,
    0xFFFFFFFE, 0xFE363C85, 0x00020037, 0xFE363C85, 0xFFFFFFEE, 0x00010005,
    0x00000014, 0x0005005B, (u32)btlevtcmd_AddRotate, 0xFFFFFFFE, 0x00000000, 0x00000000,
    0xFE363C85, 0x00010009, 0x00000001, 0x00000006, 0x00000070, 0x0005005B,
    (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0007005B,
    (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000014,
    0x00000000, 0x0000006E, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x000F005B, (u32)evt_eff64, (u32)(u8*)zero_804243b8, (u32)(char*)str_kemuri2_n64_802f93f0,
    0x00000002, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000070, 0x00020032, 0xFE363C89, 0xFE363C88, 0x00020036, 0xFE363C89,
    0x00000001, 0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000002, 0xFE363C89,
    0x0000006E, 0x00010005, 0xFE363C89, 0x00010009, 0x00000001, 0x0004005B,
    (u32)btlevtcmd_GetUnitWork, 0xFFFFFFFE, 0x00000002, 0xFE363C80, 0x00020036, 0xFE363C80,
    0x00000001, 0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000002, 0xFE363C80,
    0x00000006, 0x00000070, 0x0001006F, 0xFE363C8F, 0x00010005, 0x00000001,
    0x0004005B, (u32)btlevtcmd_GetUnitWork, 0xFFFFFFFE, 0x00000002, 0xFE363C80, 0x0002001B,
    0xFE363C80, 0x0000000F, 0x00020036, 0xFE363C80, 0x0000000F, 0x0006005B,
    (u32)btlevtcmd_ACRStart, 0xFFFFFFFE, 0x00000000, 0xFE363C80, 0xFE363C80, 0x00000000,
    0x0003005B, (u32)btlevtcmd_ACRGetResult, 0xFE363C86, 0xFE363C87, 0x00010022, 0xFE363C86,
    0x00010029, 0x00000002, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress, 0xFFFFFFFE, 0xFE363C80,
    0x0006005B, (u32)btlevtcmd_AudienceDeclareAcrobatResult, 0xFE363C80, 0x00000001, 0x00000000, 0x00000000,
    0x00000000, 0x00010005, 0x0000000F, 0x0005005B, (u32)btlevtcmd_AddRotate, 0xFFFFFFFE,
    0x00000000, 0x00000018, 0x00000000, 0x0004005B, (u32)btlevtcmd_GetUnitWork, 0xFFFFFFFE,
    0x00000003, 0xFE363C80, 0x0007005B, (u32)btlevtcmd_CalculateFaceDirection, 0xFFFFFFFE, 0xFFFFFFFF,
    0xFE363C83, 0xFE363C84, 0x00000010, 0xFE363C81, 0x0002001D, 0xFE363C81,
    0x00000000, 0x00020035, 0xFE363C80, 0xFFFFFFE8, 0x00000020, 0x00020035,
    0xFE363C80, 0x00000018, 0x00000021, 0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE,
    0x00000003, 0xFE363C80, 0x00010009, 0x00000001, 0x00000006, 0x0000002A,
    0x00000031, 0x00000021, 0x00000006, 0x00000070, 0x0006005B, (u32)btlevtcmd_GetHitPos,
    0xFE363C83, 0xFE363C84, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00020035,
    0xFE363C82, 0x00000005, 0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_PARTY_BATTLE_JUM_802f921c,
    0xF1194D80, 0x00000000, 0xF1194D80, 0x0008005B, (u32)btlevtcmd_MarioJumpPosition, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0xFE363C88, 0x00000000, 0x00000000,
    0x0001005F, 0x0000000F, 0x00000020, 0x0000006E, 0x0005005B, (u32)btlevtcmd_GetPos,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000F005B, (u32)evt_eff64,
    (u32)(u8*)zero_804243b8, (u32)(char*)str_kemuri2_n64_802f93f0, 0x00000002, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000070, 0x0006005B, (u32)btlevtcmd_GetHitPos, 0xFE363C83,
    0xFE363C84, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00020035, 0xFE363C82,
    0x00000005, 0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_PARTY_BATTLE_JUM_802f921c, 0xF1194D80,
    0x00000000, 0xF1194D80, 0x0008005B, (u32)btlevtcmd_MarioJumpPosition, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0xFE363C88, 0x00000000, 0x00000000, 0x00000021,
    0x00000021, 0x00020019, 0xFE363C85, 0x00000001, 0x00020018, 0xFE363C85,
    0x00000003, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000026, 0x00000021,
    0x00020018, 0xFE363C85, 0x00000006, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83,
    0x00000027, 0x00000021, 0x00020018, 0xFE363C85, 0x00000002, 0x0003005B,
    (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000028, 0x00000021, 0x0001005B, (u32)btlevtcmd_StopAC,
    0x0002005B, (u32)btlevtcmd_JumpContinue, 0xFFFFFFFE, 0x0005005B, (u32)btlevtcmd_SetRotateOffset, 0xFFFFFFFE,
    0x00000000, 0x00000000, 0x00000000, 0x0005005B, (u32)btlevtcmd_SetRotate, 0xFFFFFFFE,
    0x00000000, 0x00000000, 0x00000000, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE,
    0x00000001, (u32)(char*)str_PKR_S_1_802f90e0, 0x0003005B, (u32)btlevtcmd_SetFallAccel, 0xFFFFFFFE, 0xF24A7D4C,
    0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0004005B, (u32)btlevtcmd_MoveDirectionAdd, 0xFFFFFFFE, 0xFE363C80, 0x0000001E, 0x0007005B,
    (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000014,
    0xFFFFFFFF, 0x0004005B, (u32)btlevtcmd_MoveDirectionAdd, 0xFFFFFFFE, 0xFE363C80, 0x00000014,
    0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0000000A, 0xFFFFFFFF, 0x0001000A, 0x000001F4, 0x00010004, 0x0000005F,
    0x00000021, 0x0003005B, (u32)btlevtcmd_ac_timing_flag_onoff, 0x00000001, 0x00000001, 0x0002005B,
    (u32)btlevtcmd_ac_timing_get_success_frame, 0xFE363C80, 0x00020038, 0xFE363C80, 0x00000002, 0x00010009,
    0xFE363C80, 0x0003005B, (u32)btlevtcmd_ac_timing_flag_onoff, 0x00000000, 0x00000001, 0x0001005B,
    (u32)btlevtcmd_ResultAC, 0x0005005B, (u32)btlevtcmd_SetScale, 0xFFFFFFFE, 0xF24A7E80, 0xF24A7E80,
    0xF24A7E80, 0x0002005B, (u32)btlevtcmd_GetResultAC, 0xFE363C86, 0x0002001F, 0xFE363C86,
    0x00000002, 0x0006005B, (u32)btlevtcmd_CommandCheckDamage, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84,
    0x00000100, 0xFE363C85, 0x0002001C, 0xFE363C8A, 0x00000000, 0x0003005B,
    (u32)btlevtcmd_CommandGetWeaponAddress, 0xFFFFFFFE, 0xFE363C80, 0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult, 0xFE363C80,
    0xFFFFFFFF, 0x00000020, 0x00020032, 0xFE363C86, 0xFE363C8A, 0x00020036,
    0xFE363C86, 0x00000001, 0x0004005B, (u32)btlevtcmd_GetResultPrizeLv, 0xFE363C83, 0xFE363C86,
    0xFE363C86, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress, 0xFFFFFFFE, 0xFE363C80, 0x0003005B,
    (u32)btlevtcmd_AudienceDeclareACResult, 0xFE363C80, 0xFE363C86, 0x00000021, 0x00010004, 0x0000005A,
    0x00000021, 0x0002005B, (u32)btlevtcmd_GetResultCountAC, 0xFE363C86, 0x0004005B, (u32)mario_get_renzoku_count_max,
    0xFE363C83, 0xFE363C86, 0xFE363C87, 0x0002001A, 0xFE363C86, 0xFE363C87,
    0x0000006B, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000000, 0x0003005B,
    (u32)evt_btl_camera_set_zoom, 0x00000000, 0x000000C8, 0x00010009, 0x0000000A, 0x0003005B,
    (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000002, 0x0003005B, (u32)evt_btl_camera_set_zoom, 0x00000000,
    0x000000FA, 0x0000006D, 0x0006005B, (u32)btlevtcmd_CommandCheckDamage, 0xFFFFFFFE, 0xFE363C83,
    0xFE363C84, 0x00020000, 0xFE363C85, 0x0004005B, (u32)btlevtcmd_GetResultPrizeLv, 0xFE363C83,
    0xFE363C8A, 0xFE363C86, 0x0006005B, (u32)btlevtcmd_GetHitPos, 0xFE363C83, 0xFE363C84,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)btlevtcmd_ACSuccessEffect, 0xFE363C86,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00020035, 0xFE363C8A, 0x00000001,
    0x0002005B, (u32)btlevtcmd_StartAC, 0x00000000, 0x00010004, 0x0000000A, 0x00000020,
    0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000, 0x0006005B, (u32)btlevtcmd_CommandCheckDamage,
    0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0x00020100, 0xFE363C85, 0x0004005B,
    (u32)btlevtcmd_GetResultPrizeLv, 0xFE363C83, 0xFE363C8A, 0xFE363C86, 0x0006005B, (u32)btlevtcmd_GetHitPos,
    0xFE363C83, 0xFE363C84, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B,
    (u32)btlevtcmd_ACSuccessEffect, 0xFE363C86, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0003005B,
    (u32)btlevtcmd_CommandGetWeaponAddress, 0xFFFFFFFE, 0xFE363C80, 0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult, 0xFE363C80,
    0xFE363C86, 0x00000021, 0x00010003, 0x0000005A, 0x0001005B, (u32)btlevtcmd_StopAC,
    0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000, 0x0000006E, 0x0005005B,
    (u32)btlevtcmd_GetRotate, 0xFFFFFFFE, 0xF1194D80, 0xF1194D80, 0xFE363C86, 0x0002001A,
    0xFE363C86, 0x00000000, 0x00020037, 0xFE363C86, 0xFFFFFFFF, 0x00000021,
    0x0003005B, (u32)btlevtcmd_GetFaceDirection, 0xFFFFFFFE, 0xFE363C80, 0x00020037, 0xFE363C86,
    0xFE363C80, 0x00020038, 0xFE363C86, 0x0000000F, 0x00020032, 0xFE363C87,
    0x0000000F, 0x00010003, 0x00000014, 0x0005005B, (u32)btlevtcmd_AddRotate, 0xFFFFFFFE,
    0x00000000, 0x00000000, 0xFE363C86, 0x00020036, 0xFE363C87, 0x00000001,
    0x00010009, 0x00000001, 0x0002001D, 0xFE363C87, 0x00000001, 0x00010004,
    0x00000014, 0x00000021, 0x0005005B, (u32)btlevtcmd_SetRotate, 0xFFFFFFFE, 0x00000000,
    0x00000000, 0x00000000, 0x00000070, 0x0003005B, (u32)btlevtcmd_SetFallAccel, 0xFFFFFFFE,
    0xF24A7D4C, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xF1194D80, 0xFE363C81,
    0xF1194D80, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE, 0xFE363C80, 0x0000003C,
    0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0000001E, 0xFFFFFFFF, 0x0003005B, (u32)btlevtcmd_SetFallAccel, 0xFFFFFFFE, 0xF24A7CE6,
    0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE, 0xFE363C80, 0x00000014, 0x0007005B,
    (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0000000C,
    0xFFFFFFFF, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE, 0xFE363C80, 0x00000008,
    0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00000008, 0xFFFFFFFF, 0x00010003, 0x0000005F, 0x0001005B, (u32)evt_audience_ap_recovery,
    0x0001005B, (u32)btlevtcmd_InviteApInfoReport, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000,
    0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001, 0x0003005B, (u32)btlevtcmd_SetMoveSpeed,
    0xFFFFFFFE, 0xF24A9280, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001,
    0x00000028, 0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000000, 0xFFFFFFFF, 0x00000000, 0x00010003, 0x00000063,
    0x0002005B, (u32)btlevtcmd_ResetFaceDirection, 0xFFFFFFFE, 0x0001005E, (u32)btldefaultevt_SuitoruBadgeEffect, 0x0002005B,
    (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002, 0x00000001
};

u32 partyChristineAttack_Kiss[546] = {
    0x0005005B, (u32)btlevtcmd_JumpSetting, 0xFFFFFFFE, 0x00000014, 0xF24A7A80, 0xF24A7D4C,
    0x0003005B, (u32)btlevtcmd_GetSelectEnemy, 0xFE363C83, 0xFE363C84, 0x00020018, 0xFE363C83,
    0xFFFFFFFF, 0x00010004, 0x00000063, 0x00000021, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress,
    0xFFFFFFFE, 0xFE363C8C, 0x0002005B, (u32)btlevtcmd_WeaponAftereffect, 0xFE363C8C, 0x0002005B,
    (u32)btlevtcmd_CommandPayWeaponCost, 0xFFFFFFFE, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFFFFFFFE, 0x00000007,
    0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000007, 0x0004005B, (u32)evt_btl_camera_set_homing_unit,
    0x00000000, 0xFFFFFFFE, 0xFFFFFFFE, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000,
    0x00000001, 0x0003005B, (u32)evt_btl_camera_set_zoom, 0x00000000, 0x000000FA, 0x0007005B,
    (u32)btlevtcmd_CalculateFaceDirection, 0xFFFFFFFE, 0xFFFFFFFF, 0xFE363C83, 0xFE363C84, 0x00000010,
    0xFE363C8F, 0x0003005B, (u32)btlevtcmd_ChangeFaceDirection, 0xFFFFFFFE, 0xFE363C8F, 0x0001005E,
    (u32)unk_evt_803537c4, 0x00020019, 0xFE363C80, 0x00000000, 0x0007005B, (u32)btlevtcmd_CalculateFaceDirection,
    0xFE363C83, 0xFE363C84, 0xFFFFFFFE, 0xFFFFFFFF, 0x00000010, 0xFE363C8F,
    0x0003005B, (u32)btlevtcmd_ChangeFaceDirection, 0xFE363C83, 0xFE363C8F, 0x00000021, 0x0002005B,
    (u32)btlevtcmd_CommandGetWeaponActionLv, 0xFE363C80, 0x0003005B, (u32)btlevtcmd_AcSetDifficulty, 0xFFFFFFFE, 0xFE363C80,
    0x0002005B, (u32)btlevtcmd_AcGetDifficulty, 0xFE363C80, 0x00020036, 0xFE363C80, 0x00000003,
    0x00010022, 0xFE363C80, 0x00010028, 0xFFFFFFFD, 0x00020032, 0xFE363C80,
    0x0000001B, 0x00020032, 0xFE363C81, 0x00000014, 0x00010024, 0xFFFFFFFE,
    0x00020032, 0xFE363C80, 0x00000019, 0x00020032, 0xFE363C81, 0x0000001C,
    0x00010024, 0xFFFFFFFF, 0x00020032, 0xFE363C80, 0x00000017, 0x00020032,
    0xFE363C81, 0x0000001E, 0x00010024, 0x00000000, 0x00020032, 0xFE363C80,
    0x00000017, 0x00020032, 0xFE363C81, 0x0000001E, 0x00010024, 0x00000001,
    0x00020032, 0xFE363C80, 0x00000017, 0x00020032, 0xFE363C81, 0x0000001E,
    0x00010024, 0x00000002, 0x00020032, 0xFE363C80, 0x00000017, 0x00020032,
    0xFE363C81, 0x0000001E, 0x0000002A, 0x00020032, 0xFE363C80, 0x00000017,
    0x00020032, 0xFE363C81, 0x00000020, 0x00000031, 0x0009005B, (u32)btlevtcmd_AcSetParamAll,
    0x00000010, 0x000000DC, 0x00000046, 0x00000000, 0x00000000, 0xFE363C80,
    0xFE363C81, 0x00000000, 0x0002005B, (u32)btlevtcmd_AcSetFlag, 0x00000000, 0x0005005B,
    (u32)btlevtcmd_SetupAC, 0xFFFFFFFE, 0x00000005, 0x00000003, 0x0000003C, 0x00010009,
    0x00000016, 0x0002005B, (u32)btlevtcmd_StartAC, 0x00000001, 0x0003005B, (u32)btlevtcmd_GetStatusMg,
    0xFFFFFFFE, 0xFE363C86, 0x0002003C, 0xFE363C86, 0x0000000C, 0x0005005B,
    (u32)btlevtcmd_GetPos, 0xFE363C83, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B,
    (u32)btlevtcmd_FaceDirectionAdd, 0xFE363C83, 0xFE363C80, 0xFE363C86, 0x0003005B, (u32)btlevtcmd_GetStatusMg,
    0xFE363C83, 0xFE363C86, 0x0002003C, 0xFE363C86, 0x0000000C, 0x0004005B,
    (u32)btlevtcmd_FaceDirectionAdd, 0xFE363C83, 0xFE363C80, 0xFE363C86, 0x0008005B, (u32)btlevtcmd_MovePosition,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000000C8, 0xFFFFFFFF,
    0x00000000, 0x0001005B, (u32)btlevtcmd_ResultAC, 0x0002005B, (u32)btlevtcmd_GetResultAC, 0xFE363C80,
    0x0002001E, 0xFE363C80, 0x00000002, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0003005B, (u32)btlevtcmd_GetHeight, 0xFFFFFFFE,
    0xFE363C86, 0x00020035, 0xFE363C81, 0xFE363C86, 0x0004005B, (u32)btlevtcmd_GetResultPrizeLv,
    0xFFFFFFFB, 0x00000000, 0xFE363C86, 0x0005005B, (u32)btlevtcmd_ACSuccessEffect, 0xFE363C86,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000021, 0x0002005B, (u32)btlevtcmd_GetResultAC,
    0xFE363C86, 0x0001005B, (u32)btlevtcmd_StopAC, 0x0007005B, (u32)btlevtcmd_CalculateFaceDirection, 0xFFFFFFFE,
    0xFFFFFFFF, 0xFE363C83, 0xFE363C84, 0x00000010, 0xFE363C8F, 0x0003005B,
    (u32)btlevtcmd_ChangeFaceDirection, 0xFFFFFFFE, 0xFE363C8F, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000,
    0x00000002, 0x0003005B, (u32)evt_btl_camera_set_zoom, 0x00000000, 0x0000015E, 0x0006005B,
    (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_BTL_KURI_KISS1_802f93fc, 0xF1194D80, 0x00000000, 0xF1194D80,
    0x0003005B, (u32)btlevtcmd_GetBodyId, 0xFFFFFFFE, 0xFE363C89, 0x0004005B, (u32)btlevtcmd_AnimeChangePose,
    0xFFFFFFFE, 0xFE363C89, (u32)(char*)str_PKR_O_1_802f9130, 0x0001000A, 0x000000C8, 0x0005005B,
    (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B,
    (u32)btlevtcmd_FaceDirectionAdd, 0xFFFFFFFE, 0xFE363C80, 0x0000000F, 0x00020035, 0xFE363C81,
    0x0000001E, 0x0003005B, (u32)btlevtcmd_GetFaceDirection, 0xFFFFFFFE, 0xFE363C8E, 0x00020037,
    0xFE363C8E, 0x0000002D, 0x000F005B, (u32)evt_eff, (u32)(u8*)zero_804243b8, (u32)(char*)str_kiss_804243bc,
    0x00000001, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0xFE363C8E, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x0001006C, 0xFE363C8F, 0x0002001E, 0xFE363C86, 0x00000002, 0x0001005E,
    (u32)unk_evt_803537c4, 0x00020019, 0xFE363C80, 0x00000000, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType,
    0xFE363C83, 0xFE363C84, 0x0000003A, 0x00000021, 0x0005005B, (u32)btlevtcmd_GetPos,
    0xFE363C83, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00020035, 0xFE363C82,
    0x0000000F, 0x0003005B, (u32)btlevtcmd_GetHeight, 0xFE363C83, 0xFE363C8F, 0x0003005B,
    (u32)btlevtcmd_GetStatusMg, 0xFE363C83, 0xFE363C8E, 0x0002003C, 0xFE363C8F, 0xFE363C8E,
    0x00020038, 0xFE363C8F, 0x00000002, 0x00020035, 0xFE363C81, 0xFE363C8F,
    0x000F005B, (u32)evt_eff, 0x00000000, (u32)(char*)str_recovery_802f9410, 0x00000006, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0006005B, (u32)evt_snd_sfxon_3d,
    (u32)(char*)str_SFX_BTL_KURI_CHEER_K_802f941c, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000, 0x00010009,
    0x00000028, 0x0004005B, (u32)btlevtcmd_GetResultPrizeLv, 0xFFFFFFFB, 0x00000000, 0xFE363C86,
    0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult, (u32)partyWeapon_ChristineKiss, 0xFE363C86, 0x0002005B, (u32)_set_hustle,
    0xFE363C83, 0x00000020, 0x00010009, 0x00000028, 0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult,
    (u32)partyWeapon_ChristineKiss, 0xFFFFFFFF, 0x0007005B, (u32)btlevtcmd_CheckDamage, 0xFFFFFFFE, 0xFE363C83,
    0xFE363C84, (u32)partyWeapon_ChristineKiss, 0x20000100, 0xFE363C85, 0x00000021, 0x0000006D,
    0x0003005B, (u32)btlevtcmd_ftof, 0x00000018, 0xFE363C80, 0x0003005B, (u32)btlevtcmd_ftof,
    0x00000030, 0xFE363C81, 0x0006005B, (u32)btlevtcmd_ACRStart, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C81, 0x00000000, 0x0003005B, (u32)btlevtcmd_ACRGetResult, 0xFE363C87,
    0xFE363C88, 0x0002001D, 0xFE363C87, 0x00000002, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress,
    0xFFFFFFFE, 0xFE363C80, 0x0006005B, (u32)btlevtcmd_AudienceDeclareAcrobatResult, 0xFE363C80, 0x00000001,
    0x00000000, 0x00000000, 0x00000000, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE,
    0x00000001, (u32)(char*)str_PKR_J_1B_802f9438, 0x0001000A, 0x000000A6, 0x0004005B, (u32)btlevtcmd_AnimeChangePose,
    0xFFFFFFFE, 0x00000001, (u32)(char*)str_PKR_Y_1_802f93c4, 0x0001000A, 0x000000A7, 0x00000020,
    0x0001005B, (u32)evt_audience_acrobat_notry, 0x00010009, 0x00000014, 0x00000021, 0x0002001E,
    0xFE363C86, 0x00000002, 0x0006005B, (u32)btlevtcmd_AnnounceMessage, 0x00000000, 0x00000000,
    0x00000000, (u32)(char*)str_msg_st_chg_mario_qui_802f9444, 0x0000003C, 0x00000020, 0x0001000A, 0x000001F4,
    0x00000021, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000, 0x0003005B,
    (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001, 0x0002005B, (u32)btlevtcmd_WaitEventEnd, 0xFE363C8F,
    0x0002005B, (u32)btlevtcmd_ResetFaceDirection, 0xFFFFFFFE, 0x0001005B, (u32)evt_audience_ap_recovery, 0x0001005B,
    (u32)btlevtcmd_InviteApInfoReport, 0x0001005E, (u32)unk_evt_803537c4, 0x00020019, 0xFE363C80, 0x00000000,
    0x0002005B, (u32)btlevtcmd_ResetFaceDirection, 0xFE363C83, 0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFE363C83,
    0x00000021, 0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE, 0xF24A9280, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001, 0x00000028, 0x0005005B, (u32)btlevtcmd_GetHomePos,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0008005B, (u32)btlevtcmd_MovePosition,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000, 0xFFFFFFFF,
    0x00000000, 0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002, 0x00000001
};

u32 christine_appeal_event[71] = {
    0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFFFFFFFE, 0x00000007, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType,
    0xFFFFFFFE, 0x00000001, 0x00000042, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE,
    (u32)(char*)str_SFX_MARIO_APPEAL1_802f945c, 0xF1194D80, 0x00000000, 0xF1194D80, 0x0005005B, (u32)btlevtcmd_GetPos,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B, (u32)btlevtcmd_FaceDirectionAdd,
    0xFFFFFFFE, 0xFE363C80, 0x0000000F, 0x00020035, 0xFE363C81, 0x0000001E,
    0x0003005B, (u32)btlevtcmd_GetFaceDirection, 0xFFFFFFFE, 0xFE363C8E, 0x00020037, 0xFE363C8E,
    0x0000002D, 0x000F005B, (u32)evt_eff, (u32)(u8*)zero_804243b8, (u32)(char*)str_kiss_804243bc, 0x00000001,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0xFE363C8E, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0001000A,
    0x0000029B, 0x0002005B, (u32)btlevtcmd_AudienceAppeal, 0xFFFFFFFE, 0x00010009, 0x0000001E,
    0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002, 0x00000001
};

static u32 party_win_reaction[14] = {
    0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PKR_Y_1_802f93c4, 0x00010009,
    0x00000040, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001, 0x0000002B,
    0x00000002, 0x00000001
};

static u32 attack_audience_event[392] = {
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
    0xFE363C81, 0xFE363C82, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x0000000D,
    0x0005005B, (u32)evt_btl_camera_set_homing_unit_audience, 0x00000000, 0xFFFFFFFE, 0x00000001, 0xFE363C83,
    0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000002, 0x0003005B, (u32)evt_btl_camera_set_zoom,
    0x00000000, 0x00000064, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE, 0xFE363C80,
    0x00000032, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000000, 0xFFFFFFFF, 0x00000000, 0x0003005B, (u32)evt_btl_camera_set_zoom,
    0x00000000, 0x000000C8, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001,
    (u32)(char*)str_PKR_A_2B_802f9318, 0x00010009, 0x00000014, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE,
    0x00000001, (u32)(char*)str_PKR_J_1A_802f9324, 0x0005005B, (u32)btlevtcmd_SetRotateOffset, 0xFFFFFFFE, 0x00000000,
    0x0000000E, 0x00000000, 0x0000006E, 0x0005005B, (u32)btlevtcmd_GetPosFloat, 0xFFFFFFFE,
    0xFE363C85, 0xFE363C86, 0xFE363C87, 0x00020032, 0xFE363C88, 0x00000000,
    0x00010005, 0x00000024, 0x0005005B, (u32)btlevtcmd_GetPosFloat, 0xFFFFFFFE, 0xFE363C8C,
    0xFE363C8D, 0xFE363C8E, 0x0006005B, (u32)krb_get_dir, 0xFE363C85, 0xFE363C86,
    0xFE363C8C, 0xFE363C8D, 0xFE363C88, 0x0005005B, (u32)btlevtcmd_SetRotate, 0xFFFFFFFE,
    0x00000000, 0x00000000, 0xFE363C88, 0x00020034, 0xFE363C85, 0xFE363C8C,
    0x00020034, 0xFE363C86, 0xFE363C8D, 0x00010009, 0x00000001, 0x00000006,
    0x00000070, 0x0000006B, 0x00010009, 0x0000000F, 0x0006005B, (u32)btlevtcmd_snd_se,
    0xFFFFFFFE, (u32)(char*)str_SFX_BTL_KURI_ATTACK2_802f93d8, 0xF1194D80, 0x00000000, 0xF1194D80, 0x0000006D,
    0x0005005B, (u32)_get_attack_aud_no, 0xFE363C83, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00020035, 0xFE363C81, 0x0000001C, 0x00020035, 0xFE363C82, 0x00000005,
    0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_PARTY_BATTLE_JUM_802f921c, 0xF1194D80, 0x00000000,
    0xF1194D80, 0x0008005B, (u32)btlevtcmd_MarioJumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000024, 0x00000000, 0x00000000, 0x00010009, 0x00000002,
    0x0003005B, (u32)_attack_aud, 0xFE363C83, 0x00000000, 0x0003005B, (u32)evt_btl_camera_set_mode,
    0x00000000, 0x00000003, 0x0005005B, (u32)btlevtcmd_SetScale, 0xFFFFFFFE, 0xF24A7E80,
    0xF24A7E80, 0xF24A7E80, 0x0001006F, 0xFE363C8F, 0x0001000A, 0x000005DC,
    0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000, 0x0001000A, 0x000003E8,
    0x00000070, 0x0000006E, 0x0005005B, (u32)btlevtcmd_GetRotate, 0xFFFFFFFE, 0xF1194D80,
    0xF1194D80, 0xFE363C86, 0x0002001A, 0xFE363C86, 0x00000000, 0x00020037,
    0xFE363C86, 0xFFFFFFFF, 0x00000021, 0x00020038, 0xFE363C86, 0x0000000F,
    0x00020032, 0xFE363C87, 0x0000000F, 0x00010003, 0x00000014, 0x0005005B,
    (u32)btlevtcmd_AddRotate, 0xFFFFFFFE, 0x00000000, 0x00000000, 0xFE363C86, 0x00020036,
    0xFE363C87, 0x00000001, 0x00010009, 0x00000001, 0x0002001D, 0xFE363C87,
    0x00000001, 0x00010004, 0x00000014, 0x00000021, 0x0005005B, (u32)btlevtcmd_SetRotate,
    0xFFFFFFFE, 0x00000000, 0x00000000, 0x00000000, 0x00000070, 0x0003005B,
    (u32)btlevtcmd_SetFallAccel, 0xFFFFFFFE, 0xF24A7D4C, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE,
    0xF1194D80, 0xFE363C81, 0xF1194D80, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE,
    0xFE363C80, 0x0000003C, 0x0004005B, (u32)btlevtcmd_TransStageFloorPosition, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0000001E, 0xFFFFFFFF, 0x0003005B, (u32)btlevtcmd_SetFallAccel, 0xFFFFFFFE,
    0xF24A7CE6, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE, 0xFE363C80, 0x00000014,
    0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0000000C, 0xFFFFFFFF, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE, 0xFE363C80,
    0x00000008, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000008, 0xFFFFFFFF, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE,
    0x00000001, 0x0000002A, 0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE, 0xF24A8A80,
    0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C86, 0xFE363C81, 0xFE363C82,
    0x00000000, 0xFFFFFFFF, 0x00000000, 0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)btlevtcmd_SetPos, 0xFFFFFFFE,
    0xFE363C88, 0xFE363C81, 0xFE363C82, 0x0002005B, (u32)btlevtcmd_WaitEventEnd, 0xFE363C8F,
    0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00000000, 0xFFFFFFFF, 0x00000000, 0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE,
    0x00000002, 0x00000001
};

/* Recovered target Christine .rodata string catalog, in target order. */
const char str_PKR_D_5_802f90b8[] = "PKR_D_5";
const char str_PKR_X_1_802f90c0[] = "PKR_X_1";
const char str_PKR_D_2_802f90c8[] = "PKR_D_2";
const char str_PKR_N_1_802f90d0[] = "PKR_N_1";
const char str_PKR_D_3_802f90d8[] = "PKR_D_3";
const char str_PKR_S_1_802f90e0[] = "PKR_S_1";
const char str_PKR_G_2_802f90e8[] = "PKR_G_2";
const char str_PKR_G_1_802f90f0[] = "PKR_G_1";
const char str_PKR_D_1_802f90f8[] = "PKR_D_1";
const char str_PKR_W_1_802f9100[] = "PKR_W_1";
const char str_PKR_R_1_802f9108[] = "PKR_R_1";
const char str_PKR_D_4_802f9110[] = "PKR_D_4";
const char str_PKR_T_1_802f9118[] = "PKR_T_1";
const char str_PKR_I_1_802f9120[] = "PKR_I_1";
const char str_PKR_C_1_802f9128[] = "PKR_C_1";
const char str_PKR_O_1_802f9130[] = "PKR_O_1";
const char str_PKR_A_1_802f9138[] = "PKR_A_1";
const char str_btl_un_christine_802f9140[] = "btl_un_christine";
const char str_SFX_PARTY_DAMAGE1_802f9154[] = "SFX_PARTY_DAMAGE1";
const char str_SFX_MARIO_DAMAGE_FIR_802f9168[] = "SFX_MARIO_DAMAGE_FIRE1";
const char str_SFX_MARIO_DAMAGE_ICE_802f9180[] = "SFX_MARIO_DAMAGE_ICE1";
const char str_SFX_MARIO_DAMAGE_BIR_802f9198[] = "SFX_MARIO_DAMAGE_BIRIBIRI1";
const char str_SFX_PARTY_BATTLE_SUB_802f91b4[] = "SFX_PARTY_BATTLE_SUBMOVE1L";
const char str_SFX_PARTY_BATTLE_SUB_802f91d0[] = "SFX_PARTY_BATTLE_SUBMOVE1R";
const char str_SFX_PARTY_BATTLE_MOV_802f91ec[] = "SFX_PARTY_BATTLE_MOVE1L";
const char str_SFX_PARTY_BATTLE_MOV_802f9204[] = "SFX_PARTY_BATTLE_MOVE1R";
const char str_SFX_PARTY_BATTLE_JUM_802f921c[] = "SFX_PARTY_BATTLE_JUMP1";
const char str_SFX_PARTY_BATTLE_LAN_802f9234[] = "SFX_PARTY_BATTLE_LANDING1";
const char str_btl_wn_pkr_normal_802f9250[] = "btl_wn_pkr_normal";
const char str_msg_pkr_normal_jump_802f9264[] = "msg_pkr_normal_jump";
const char str_msg_ac_zutsuki_802f9278[] = "msg_ac_zutsuki";
const char str_btl_wn_pkr_lv1_802f9288[] = "btl_wn_pkr_lv1";
const char str_msg_pkr_monosiri_802f9298[] = "msg_pkr_monosiri";
const char str_msg_ac_monoshiri_802f92ac[] = "msg_ac_monoshiri";
const char str_btl_wn_pkr_lv2_802f92c0[] = "btl_wn_pkr_lv2";
const char str_msg_pkr_renzoku_zuts_802f92d0[] = "msg_pkr_renzoku_zutsuki";
const char str_btl_wn_pkr_lv3_802f92e8[] = "btl_wn_pkr_lv3";
const char str_msg_pkr_nage_kiss_802f92f8[] = "msg_pkr_nage_kiss";
const char str_msg_ac_kiss_802f930c[] = "msg_ac_kiss";
const char str_PKR_A_2B_802f9318[] = "PKR_A_2B";
const char str_PKR_J_1A_802f9324[] = "PKR_J_1A";
const char str_SFX_BTL_KURI_ATTACK1_802f9330[] = "SFX_BTL_KURI_ATTACK1";
const char str_PKR_2A_1_802f9348[] = "PKR_2A_1";
const char str_PKR_2A_2_802f9354[] = "PKR_2A_2";
const char str_SFX_BTL_KURI_LEARNED_802f9360[] = "SFX_BTL_KURI_LEARNED1";
const char str_PKR_2A_3_802f9378[] = "PKR_2A_3";
const char str_SFX_BTL_KURI_FAIL_LE_802f9384[] = "SFX_BTL_KURI_FAIL_LEARNED1";
const char str_PKR_2A_4_802f93a0[] = "PKR_2A_4";
const char str_SFX_BTL_KURI_LEARNED_802f93ac[] = "SFX_BTL_KURI_LEARNED2";
const char str_PKR_Y_1_802f93c4[] = "PKR_Y_1";
const char str_PKR_2A_5_802f93cc[] = "PKR_2A_5";
const char str_SFX_BTL_KURI_ATTACK2_802f93d8[] = "SFX_BTL_KURI_ATTACK2";
const char str_kemuri2_n64_802f93f0[] = "kemuri2_n64";
const char str_SFX_BTL_KURI_KISS1_802f93fc[] = "SFX_BTL_KURI_KISS1";
const char str_recovery_802f9410[] = "recovery";
const char str_SFX_BTL_KURI_CHEER_K_802f941c[] = "SFX_BTL_KURI_CHEER_KISS1";
const char str_PKR_J_1B_802f9438[] = "PKR_J_1B";
const char str_msg_st_chg_mario_qui_802f9444[] = "msg_st_chg_mario_quick";
const char str_SFX_MARIO_APPEAL1_802f945c[] = "SFX_MARIO_APPEAL1";

/* Recovered target Christine .sdata2 family, defined after all consumers. */
const char str_c_pkr_804243b0[] = "c_pkr";
const u8 zero_804243b8[1] = {0};
const char str_kiss_804243bc[] = "kiss";
const f32 float_neg50_804243c4 = -50.0f;
const f32 float_50_804243c8 = 50.0f;
const f32 float_0_804243cc = 0.0f;
