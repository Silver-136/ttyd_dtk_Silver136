#include "unit/party/unit_party_yoshi.h"

extern void* _battleWorkPointer;
extern s32 evtGetValue(void* evt, s32 arg);
extern f32 evtGetFloat(void* evt, s32 arg);
extern void evtSetValue(void* evt, s32 arg, s32 value);
extern s32 BattleTransID(void* evt, s32 id);
extern void* BattleGetUnitPtr(void* battleWork, s32 id);
extern void BtlUnit_GetPos(void* unit, f32* x, f32* y, f32* z);
extern f32 float_0_804240b4;
extern f32 float_1_804240d8;
extern f32 float_5_804240e0;

/* Forward declarations for document-backed structured data. */
extern u32 partyWeapon_YoshiNormalAttack[48];
extern u32 partyWeapon_YoshiNomikomi_Involved[48];
extern u32 btldefaultevt_Dummy[];
extern u32 subset_attack_ready_jump_event[];
extern u32 btldefaultevt_avoid_counter_event[];
extern s32 weaponGetPowerFromPartyAttackLv();
extern s32 weaponGetPowerDefault();

/* Forward declarations for recovered unit_party_yoshi event bytecode. */
static u32 battle_entry_event[59];
static u32 init_event[54];
static u32 damage_event[16];
extern u32 partyYoshiAttack_NormalAttack[1025];
extern u32 partyYoshiAttack_Nomikomi[1000];
extern u32 partyYoshiAttack_EggAttack[1266];
extern u32 _egg_attack_event[195];
extern u32 partyYoshiAttack_CallGuard[553];
extern u32 yoshi_appeal_event[39];
static u32 party_win_reaction[25];
static u32 attack_audience_event[290];
static u32 attack_event[2];
static u32 wait_event[6];
extern s32 _attack_aud();
extern s32 _get_attack_aud_no();
extern s32 btldefaultevt_Confuse();
extern s32 btldefaultevt_Damage();
extern s32 btldefaultevt_SuitoruBadgeEffect();
extern s32 btlevtcmd_ACRGetResult();
extern s32 btlevtcmd_ACRStart();
extern s32 btlevtcmd_ACSuccessEffect();
extern s32 btlevtcmd_AcGetDifficulty();
extern s32 btlevtcmd_AcGetOutputParam();
extern s32 btlevtcmd_AcSetDifficulty();
extern s32 btlevtcmd_AcSetFlag();
extern s32 btlevtcmd_AcSetGaugeParam();
extern s32 btlevtcmd_AcSetOutputParam();
extern s32 btlevtcmd_AcSetParamAll();
extern s32 btlevtcmd_AddPartsRotate();
extern s32 btlevtcmd_AddRotate();
extern s32 btlevtcmd_AnimeChangePose();
extern s32 btlevtcmd_AnimeChangePoseFromTable();
extern s32 btlevtcmd_AnimeChangePoseType();
extern s32 btlevtcmd_AnimeSetPoseSoundTable();
extern s32 btlevtcmd_AnimeSetPoseTable();
extern s32 btlevtcmd_AnimeWaitPlayComplete();
extern s32 btlevtcmd_AttackDeclare();
extern s32 btlevtcmd_AttackDeclareAll();
extern s32 btlevtcmd_AudienceAppeal();
extern s32 btlevtcmd_AudienceDeclareACResult();
extern s32 btlevtcmd_AudienceDeclareAcrobatResult();
extern s32 btlevtcmd_CalculateFaceDirection();
extern s32 btlevtcmd_ChangeFaceDirection();
extern s32 btlevtcmd_CheckDamage();
extern s32 btlevtcmd_ChoiceSamplingEnemy();
extern s32 btlevtcmd_CommandCheckDamage();
extern s32 btlevtcmd_CommandGetWeaponActionLv();
extern s32 btlevtcmd_CommandGetWeaponAddress();
extern s32 btlevtcmd_CommandPayWeaponCost();
extern s32 btlevtcmd_CommandPreCheckDamage();
extern s32 btlevtcmd_DamageDirect();
extern s32 btlevtcmd_DivePartsPosition();
extern s32 btlevtcmd_DivePosition();
extern s32 btlevtcmd_FaceDirectionAdd();
extern s32 btlevtcmd_FaceDirectionSub();
extern s32 btlevtcmd_GetEnemyBelong();
extern s32 btlevtcmd_GetExp();
extern s32 btlevtcmd_GetFaceDirection();
extern s32 btlevtcmd_GetHeight();
extern s32 btlevtcmd_GetHitPos();
extern s32 btlevtcmd_GetHomePos();
extern s32 btlevtcmd_GetMoveFrame();
extern s32 btlevtcmd_GetPartsWork();
extern s32 btlevtcmd_GetPartyTechLv();
extern s32 btlevtcmd_GetPos();
extern s32 btlevtcmd_GetResultAC();
extern s32 btlevtcmd_GetResultPrizeLv();
extern s32 btlevtcmd_GetSelectEnemy();
extern s32 btlevtcmd_GetSelectNextEnemy();
extern s32 btlevtcmd_GetStatusMg();
extern s32 btlevtcmd_GetTakeoffPosition();
extern s32 btlevtcmd_GetUnitWork();
extern s32 btlevtcmd_InviteApInfoReport();
extern s32 btlevtcmd_JumpPartsPosition();
extern s32 btlevtcmd_JumpPosition();
extern s32 btlevtcmd_KillUnit();
extern s32 btlevtcmd_MovePosition();
extern s32 btlevtcmd_OffAttribute();
extern s32 btlevtcmd_OffPartsAttribute();
extern s32 btlevtcmd_OnAttribute();
extern s32 btlevtcmd_OnPartsAttribute();
extern s32 btlevtcmd_PayWeaponCost();
extern s32 btlevtcmd_PreCheckDamage();
extern s32 btlevtcmd_ResetFaceDirection();
extern s32 btlevtcmd_ResultAC();
extern s32 btlevtcmd_RunDataEventChild();
extern s32 btlevtcmd_SamplingEnemy();
extern s32 btlevtcmd_SetDispOffset();
extern s32 btlevtcmd_SetEventAttack();
extern s32 btlevtcmd_SetEventConfusion();
extern s32 btlevtcmd_SetEventDamage();
extern s32 btlevtcmd_SetEventEntry();
extern s32 btlevtcmd_SetEventWait();
extern s32 btlevtcmd_SetFallAccel();
extern s32 btlevtcmd_SetJumpSound();
extern s32 btlevtcmd_SetMoveSoundDataWork();
extern s32 btlevtcmd_SetMoveSpeed();
extern s32 btlevtcmd_SetPartsDispOffset();
extern s32 btlevtcmd_SetPartsFallAccel();
extern s32 btlevtcmd_SetPartsMoveSpeed();
extern s32 btlevtcmd_SetPartsPos();
extern s32 btlevtcmd_SetPartsRotate();
extern s32 btlevtcmd_SetPartsRotateOffset();
extern s32 btlevtcmd_SetPartsScale();
extern s32 btlevtcmd_SetPartsWork();
extern s32 btlevtcmd_SetPos();
extern s32 btlevtcmd_SetRotate();
extern s32 btlevtcmd_SetRotateOffset();
extern s32 btlevtcmd_SetRunSound();
extern s32 btlevtcmd_SetUnitWork();
extern s32 btlevtcmd_SetWalkSound();
extern s32 btlevtcmd_SetupAC();
extern s32 btlevtcmd_StartAC();
extern s32 btlevtcmd_StartAvoid();
extern s32 btlevtcmd_StartWaitEvent();
extern s32 btlevtcmd_StopAC();
extern s32 btlevtcmd_StoreExp();
extern s32 btlevtcmd_TransStageFloorPosition();
extern s32 btlevtcmd_WaitEventEnd();
extern s32 btlevtcmd_WaitGuardMove();
extern s32 btlevtcmd_WeaponAftereffect();
extern s32 btlevtcmd_ac_timing_flag_onoff();
extern s32 btlevtcmd_ftof();
extern s32 btlevtcmd_ftomsec();
extern s32 btlevtcmd_snd_se();
extern s32 evt_audience_acrobat_notry();
extern s32 evt_audience_ap_recovery();
extern s32 evt_btl_camera_noshake();
extern s32 evt_btl_camera_set_homing_unit();
extern s32 evt_btl_camera_set_homing_unit_audience();
extern s32 evt_btl_camera_set_homing_unitparts();
extern s32 evt_btl_camera_set_mode();
extern s32 evt_btl_camera_set_moveSpeedLv();
extern s32 evt_btl_camera_set_zoom();
extern s32 evt_btl_camera_shake_h();
extern s32 evt_eff();
extern s32 evt_env_blur_on();
extern s32 evt_snd_sfxon();
extern s32 evt_snd_sfxon_3d();
extern s32 evt_sub_intpl_get_value();
extern s32 evt_sub_intpl_init();
extern s32 evt_sub_random();
extern u32 partyWeapon_YoshiCallGuard[48];
extern u32 partyWeapon_YoshiEggAttack_Minimini[48];
extern u32 partyWeapon_YoshiNomikomi_Dmg0[48];
extern u32 partyWeapon_YoshiNomikomi_Fire[48];
extern u32 partyWeapon_YoshiNomikomi_Shot[48];
extern u32 partyWeapon_YoshiNomikomi_Spew[48];
extern u32 posesound_normal_attack[75];

/* Recovered target .sdata2 event-string prefix. */
const char str_eff_80424090[] = "eff";
const u8 zero_80424094[1] = {0};
const char str_bomb_80424098[] = "bomb";
const char str_kiss_804240a0[] = "kiss";

/* Recovered target .rodata string prefix; exact target-relative order. */
const char str_PYS_D_5_802f77d8[] = "PYS_D_5";
const char str_PYS_X_1_802f77e0[] = "PYS_X_1";
const char str_PYS_D_2_802f77e8[] = "PYS_D_2";
const char str_PYS_N_1_802f77f0[] = "PYS_N_1";
const char str_PYS_D_3_802f77f8[] = "PYS_D_3";
const char str_PYS_S_1_802f7800[] = "PYS_S_1";
const char str_PYS_G_2_802f7808[] = "PYS_G_2";
const char str_PYS_G_1_802f7810[] = "PYS_G_1";
const char str_PYS_D_1_802f7818[] = "PYS_D_1";
const char str_PYS_W_1_802f7820[] = "PYS_W_1";
const char str_PYS_R_1_802f7828[] = "PYS_R_1";
const char str_PYS_D_4_802f7830[] = "PYS_D_4";
const char str_PYS_T_1_802f7838[] = "PYS_T_1";
const char str_PYS_I_1_802f7840[] = "PYS_I_1";
const char str_PYS_C_1_802f7848[] = "PYS_C_1";
const char str_PYS_O_1_802f7850[] = "PYS_O_1";
const char str_PYS_F_1_802f7858[] = "PYS_F_1";
const char str_CHU_C_1_802f7860[] = "CHU_C_1";
const char str_CHU_C_2_802f7868[] = "CHU_C_2";
const char str_CHU_C_3_802f7870[] = "CHU_C_3";
const char str_btl_un_yoshi_802f7878[] = "btl_un_yoshi";
const char str_c_babyyoshi_802f7888[] = "c_babyyoshi";
const char str_c_tamago_802f7894[] = "c_tamago";
const char str_SFX_PARTY_DAMAGE1_802f78a0[] = "SFX_PARTY_DAMAGE1";
const char str_SFX_MARIO_DAMAGE_FIR_802f78b4[] = "SFX_MARIO_DAMAGE_FIRE1";
const char str_SFX_MARIO_DAMAGE_ICE_802f78cc[] = "SFX_MARIO_DAMAGE_ICE1";
const char str_SFX_MARIO_DAMAGE_BIR_802f78e4[] = "SFX_MARIO_DAMAGE_BIRIBIRI1";
const char str_SFX_PARTY_BATTLE_SUB_802f7900[] = "SFX_PARTY_BATTLE_SUBMOVE1L";
const char str_SFX_PARTY_BATTLE_SUB_802f791c[] = "SFX_PARTY_BATTLE_SUBMOVE1R";
const char str_SFX_PARTY_BATTLE_MOV_802f7938[] = "SFX_PARTY_BATTLE_MOVE1L";
const char str_SFX_PARTY_BATTLE_MOV_802f7950[] = "SFX_PARTY_BATTLE_MOVE1R";
const char str_SFX_PARTY_BATTLE_JUM_802f7968[] = "SFX_PARTY_BATTLE_JUMP1";
const char str_SFX_PARTY_BATTLE_LAN_802f7980[] = "SFX_PARTY_BATTLE_LANDING1";
const char str_btl_wn_pys_normal_802f799c[] = "btl_wn_pys_normal";
const char str_msg_pys_hip_drop_802f79b0[] = "msg_pys_hip_drop";
const char str_msg_ac_hip_drop_802f79c4[] = "msg_ac_hip_drop";
const char str_btl_wn_pys_lv1_802f79d4[] = "btl_wn_pys_lv1";
const char str_msg_pys_nomikomi_802f79e4[] = "msg_pys_nomikomi";
const char str_msg_ac_nomikomi_802f79f8[] = "msg_ac_nomikomi";
const char str_btl_wn_pys_lv2_802f7a08[] = "btl_wn_pys_lv2";
const char str_msg_pys_wonder_egg_802f7a18[] = "msg_pys_wonder_egg";
const char str_msg_ac_wonder_egg_802f7a2c[] = "msg_ac_wonder_egg";
const char str_btl_wn_pys_lv3_802f7a40[] = "btl_wn_pys_lv3";
const char str_msg_pys_taigun_yoshi_802f7a50[] = "msg_pys_taigun_yoshi";
const char str_msg_ac_taigun_yoshi_802f7a68[] = "msg_ac_taigun_yoshi";
const char str_PYS_R_4_802f7a7c[] = "PYS_R_4";
const char str_PYS_R_5_802f7a84[] = "PYS_R_5";
const char str_PYS_A_3_802f7a8c[] = "PYS_A_3";
const char str_PYS_J_1A_802f7a94[] = "PYS_J_1A";
const char str_PYS_J_1B_802f7aa0[] = "PYS_J_1B";
const char str_SFX_BTL_YOSHI_ATTACK_802f7aac[] = "SFX_BTL_YOSHI_ATTACK1";
const char str_PYS_A_1_802f7ac4[] = "PYS_A_1";
const char str_PYS_A_5_802f7acc[] = "PYS_A_5";
const char str_PYS_Y_1_802f7ad4[] = "PYS_Y_1";
const char str_PYS_A_2_802f7adc[] = "PYS_A_2";
const char str_SFX_BTL_YOSHI_SWALLO_802f7ae4[] = "SFX_BTL_YOSHI_SWALLOW1";
const char str_SFX_BTL_YOSHI_SWALLO_802f7afc[] = "SFX_BTL_YOSHI_SWALLOW2";
const char str_PYS_A_7_802f7b14[] = "PYS_A_7";
const char str_SFX_BTL_YOSHI_SWALLO_802f7b1c[] = "SFX_BTL_YOSHI_SWALLOW5";
const char str_SFX_BTL_YOSHI_SWALLO_802f7b34[] = "SFX_BTL_YOSHI_SWALLOW3";
const char str_PYS_A_4_802f7b4c[] = "PYS_A_4";
const char str_SFX_BTL_YOSHI_NOMIKO_802f7b54[] = "SFX_BTL_YOSHI_NOMIKOMI6";
const char str_PYS_A_6_802f7b6c[] = "PYS_A_6";
const char str_SFX_BTL_YOSHI_SWALLO_802f7b74[] = "SFX_BTL_YOSHI_SWALLOW7";
const char str_SFX_BTL_YOSHI_SWALLO_802f7b8c[] = "SFX_BTL_YOSHI_SWALLOW4";
const char str_breath_fire_802f7ba4[] = "breath_fire";
const char str_PYS_A_8_802f7bb0[] = "PYS_A_8";
const char str_PYS_A_12_802f7bb8[] = "PYS_A_12";
const char str_SFX_BTL_YOSHI_EGG1_802f7bc4[] = "SFX_BTL_YOSHI_EGG1";
const char str_PYS_A3_1_802f7bd8[] = "PYS_A3_1";
const char str_PYS_A3_2_802f7be4[] = "PYS_A3_2";
const char str_PYS_A3_3_802f7bf0[] = "PYS_A3_3";
const char str_PYS_A4_1_802f7bfc[] = "PYS_A4_1";
const char str_PYS_A4_2_802f7c08[] = "PYS_A4_2";
const char str_PYS_A4_3_802f7c14[] = "PYS_A4_3";
const char str_PYS_A2_1_802f7c20[] = "PYS_A2_1";
const char str_PYS_A2_2_802f7c2c[] = "PYS_A2_2";
const char str_PYS_A2_3_802f7c38[] = "PYS_A2_3";
const char str_SFX_BTL_YOSHI_EGG3_802f7c44[] = "SFX_BTL_YOSHI_EGG3";
const char str_SFX_BTL_YOSHI_EGG2_802f7c58[] = "SFX_BTL_YOSHI_EGG2";
const char str_PYS_A_10_802f7c6c[] = "PYS_A_10";
const char str_PYS_A_11_802f7c78[] = "PYS_A_11";
const char str_SFX_ENM_TOGENOKO_CAL_802f7c84[] = "SFX_ENM_TOGENOKO_CALL1";
const char str_SFX_MARIO_APPEAL1_802f7c9c[] = "SFX_MARIO_APPEAL1";

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 yoshi_original_color_anim_set(void* evt, s32 releaseOld) {
    extern s32 pouchGetPartyColor(s32 partyId);
    extern void* BattleGetUnitPartsPtr(s32 unitId, s32 partsId);
    extern void* animPoseGetAnimPosePtr(s32 poseId);
    extern void animPoseRelease(s32 poseId);
    extern s32 animEffectAsync(char* name, s32 flags);
    extern s32 animPoseEntry(char* name, s32 flags);

    char* base = (char*)str_PYS_D_5_802f77d8;
    void* event = evt;
    s32 flag = releaseOld;
    s32* args = *(s32**)((s32)event + 0x18);
    void* battleWork = _battleWorkPointer;
    s32 rawId = evtGetValue(event, args[0]);
    s32 unitId = BattleTransID(event, rawId);
    char* anim;
    void* parts;
    s32 poseId;

    BattleGetUnitPtr(battleWork, unitId);
    switch (pouchGetPartyColor(4)) {
        case 0:
            anim = base + 0x4F8;
            break;
        case 1:
            anim = base + 0x508;
            break;
        case 2:
            anim = base + 0x518;
            break;
        case 3:
            anim = base + 0x528;
            break;
        case 4:
            anim = base + 0x538;
            break;
        case 5:
            anim = base + 0x548;
            break;
        case 6:
        default:
            anim = 0;
            break;
    }
    if (flag != 0) {
        if (anim == 0) {
            return 2;
        }
        parts = BattleGetUnitPartsPtr(unitId, 1);
        animPoseGetAnimPosePtr(*(s32*)((s32)parts + 0x1C0));
        poseId = *(s32*)((s32)parts + 0x1C0);
        if (poseId != -1) {
            animPoseRelease(poseId);
            *(s32*)((s32)parts + 0x1C0) = -1;
        }
    }
    if (animEffectAsync(anim, 1) != 0) {
        parts = BattleGetUnitPartsPtr(unitId, 1);
        *(s32*)((s32)parts + 0x1C0) = animPoseEntry(anim, 1);
        return 2;
    }
    return 0;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void __makeTechMenuFunc(void* commandWork, s32* count) {
    extern void* BattleGetPartyPtr(void* battleWork);
    extern s32 BattleTransPartyId(s32 id);
    extern s32 partyGetTechLv(s32 partyId);
    extern char* msgSearch(char* msg);
    extern u8 lbl_8037C440[];
    s32 techLv;
    void* party;
    u8* data;
    u8* entry;

    party = BattleGetPartyPtr(_battleWorkPointer);
    techLv = partyGetTechLv(BattleTransPartyId(*(s32*)((s32)party + 8)));

    data = lbl_8037C440;
    entry = (u8*)commandWork + *count * 0x1C;
    *(s32*)(entry + 0x90) = -1;
    entry = (u8*)commandWork + *count * 0x1C;
    *(s32*)(entry + 0x94) = 0;
    entry = (u8*)commandWork + *count * 0x1C;
    *(void**)(entry + 0x80) = data + 0x624;
    entry = (u8*)commandWork + *count * 0x1C;
    *(s32*)(entry + 0x84) = 0;
    entry = (u8*)commandWork + *count * 0x1C;
    *(u16*)(entry + 0x8C) = *(u16*)(*(u8**)(entry + 0x80) + 4);
    *(void**)((u8*)commandWork + *count * 0x1C + 0x88) =
        msgSearch(**(char***)((u8*)commandWork + *count * 0x1C + 0x80));
    *count = *count + 1;

    if (techLv >= 0) {
        entry = (u8*)commandWork + *count * 0x1C;
        *(s32*)(entry + 0x90) = -1;
        entry = (u8*)commandWork + *count * 0x1C;
        *(s32*)(entry + 0x94) = 0;
        entry = (u8*)commandWork + *count * 0x1C;
        *(void**)(entry + 0x80) = data + 0x6E4;
        entry = (u8*)commandWork + *count * 0x1C;
        *(s32*)(entry + 0x84) = 0;
        entry = (u8*)commandWork + *count * 0x1C;
        *(u16*)(entry + 0x8C) = *(u16*)(*(u8**)(entry + 0x80) + 4);
        *(void**)((u8*)commandWork + *count * 0x1C + 0x88) =
            msgSearch(**(char***)((u8*)commandWork + *count * 0x1C + 0x80));
        *count = *count + 1;
    }
    if (techLv >= 1) {
        entry = (u8*)commandWork + *count * 0x1C;
        *(s32*)(entry + 0x90) = -1;
        entry = (u8*)commandWork + *count * 0x1C;
        *(s32*)(entry + 0x94) = 0;
        entry = (u8*)commandWork + *count * 0x1C;
        *(void**)(entry + 0x80) = data + 0xAA4;
        entry = (u8*)commandWork + *count * 0x1C;
        *(s32*)(entry + 0x84) = 0;
        entry = (u8*)commandWork + *count * 0x1C;
        *(u16*)(entry + 0x8C) = *(u16*)(*(u8**)(entry + 0x80) + 4);
        *(void**)((u8*)commandWork + *count * 0x1C + 0x88) =
            msgSearch(**(char***)((u8*)commandWork + *count * 0x1C + 0x80));
        *count = *count + 1;
    }
    if (techLv >= 2) {
        entry = (u8*)commandWork + *count * 0x1C;
        *(s32*)(entry + 0x90) = -1;
        entry = (u8*)commandWork + *count * 0x1C;
        *(s32*)(entry + 0x94) = 0;
        entry = (u8*)commandWork + *count * 0x1C;
        *(void**)(entry + 0x80) = data + 0xB64;
        entry = (u8*)commandWork + *count * 0x1C;
        *(s32*)(entry + 0x84) = 0;
        entry = (u8*)commandWork + *count * 0x1C;
        *(u16*)(entry + 0x8C) = *(u16*)(*(u8**)(entry + 0x80) + 4);
        *(void**)((u8*)commandWork + *count * 0x1C + 0x88) =
            msgSearch(**(char***)((u8*)commandWork + *count * 0x1C + 0x80));
        *count = *count + 1;
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

u8 btl_yoshi_yoroyoro_jump_calc_param(void* evt) {
    extern void evtSetFloat(void* evt, s32 arg, f32 value);
    extern void BtlUnit_SetMoveTargetPos(void* unit, f32 x, f32 y, f32 z);
    extern void BtlUnit_SetMoveStartPos(void* unit, f32 x, f32 y, f32 z);
    extern void BtlUnit_SetMoveCurrentPos(void* unit, f32 x, f32 y, f32 z);
    extern f32 angleABf(f32 x0, f32 z0, f32 x1, f32 z1);
    extern f32 distABf(f32 x0, f32 z0, f32 x1, f32 z1);
    s32* args;
    void* battleWork;
    s32 unitId;
    void* unit;
    f32 targetX;
    f32 targetY;
    f32 targetZ;
    f32 moveSpeed;
    s32 speedArg;
    s32 durationOut;
    f32 x;
    f32 y;
    f32 z;
    f32 dx;

    args = *(s32**)((s32)evt + 0x18);
    battleWork = _battleWorkPointer;
    unitId = BattleTransID(evt, evtGetValue(evt, args[0]));
    targetX = (f32)evtGetValue(evt, args[1]);
    targetY = (f32)evtGetValue(evt, args[2]);
    targetZ = (f32)evtGetValue(evt, args[3]);
    speedArg = args[4];
    moveSpeed = evtGetFloat(evt, speedArg);
    unit = BattleGetUnitPtr(battleWork, unitId);
    durationOut = args[5];

    if (moveSpeed > float_0_804240b4) {
        *(f32*)((s32)unit + 0x170) = moveSpeed;
    }
    if (*(f32*)((s32)unit + 0x170) <= float_0_804240b4) {
        *(f32*)((s32)unit + 0x170) = float_1_804240d8;
    }
    BtlUnit_SetMoveTargetPos(unit, targetX, targetY, targetZ);
    BtlUnit_GetPos(unit, &x, &y, &z);
    BtlUnit_SetMoveStartPos(unit, x, y, z);
    BtlUnit_SetMoveCurrentPos(unit, x, y, z);
    *(f32*)((s32)unit + 0x17C) = angleABf(x, z, targetX, targetZ);
    *(f32*)((s32)unit + 0x180) = distABf(x, z, targetX, targetZ);
    dx = targetX - x;
    if (dx > float_0_804240b4) {
        *(s8*)((s32)unit + 0x188) = 1;
    } else if (dx < float_0_804240b4) {
        *(s8*)((s32)unit + 0x188) = -1;
    }
    *(f32*)((s32)unit + 0x180) = (dx < float_0_804240b4) ? -dx : dx;
    *(s32*)((s32)unit + 0x16C) = 0xB4;
    *(f32*)((s32)unit + 0x184) = (f32)*(s32*)((s32)unit + 0x16C);
    *(f32*)((s32)unit + 0x178) = float_0_804240b4;
    *(f32*)((s32)unit + 0x170) = *(f32*)((s32)unit + 0x180) / (f32)*(s32*)((s32)unit + 0x16C);
    evtSetFloat(evt, speedArg, *(f32*)((s32)unit + 0x170));
    evtSetValue(evt, durationOut, *(s32*)((s32)unit + 0x16C));
    return 2;
}
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
u8 btl_yoshi_yoroyoro_jump_move(void* evt, s32 first) {
    extern void* _battleWorkPointer;
    extern s32 evtGetValue(void* evt, s32 arg);
    extern f32 evtGetFloat(void* evt, s32 arg);
    extern s32 BattleTransID(void* evt, s32 id);
    extern void* BattleGetUnitPtr(void* battleWork, s32 id);
    extern f64 sinfd(f64 angle);
    extern void* BtlUnit_GetPartsPtr(void* unit, s32 partId);
    extern void BtlUnit_SetPos(f32 x, f32 y, f32 z, void* unit);
    extern s32 BtlUnit_GetHeight(void* unit);
    extern s32 effSweatN64Entry(f32 x, f32 y, f32 z, f32 scale, f32 xOffset, s32 a, s32 b);
    extern f32 float_0_804240b4;
    extern f32 float_5_804240e0;
    extern f32 float_10_804240e8;
    extern f32 float_180_804240ec;
    extern f32 float_360_804240f0;
    extern f32 float_90_804240f4;
    extern f32 float_75_804240f8;
    extern f32 float_25_804240fc;
    extern f32 float_neg50_80424100;
    extern f32 float_50_80424104;
    extern f32 float_0p5_80424108;

    s32* args;
    void* unit;
    s32 unitId;
    s32 frame;
    s32 phase;
    f32 cur;
    f32 target;
    f32 diff;
    f32 speed;
    f32 jumpFrames;
    f32 height;
    f32 nextSpeedY;
    s32 effect;

    args = *(s32**)((s32)evt + 0x18);
    unitId = BattleTransID(evt, evtGetValue(evt, args[0]));
    evtGetValue(evt, args[1]);
    evtGetValue(evt, args[2]);
    evtGetValue(evt, args[3]);
    evtGetFloat(evt, args[4]);
    unit = BattleGetUnitPtr(_battleWorkPointer, unitId);
    evtGetValue(evt, args[5]);

    if (first != 0) {
        *(u8*)((s32)evt + 0xD) = 1;
        *(s32*)((s32)evt + 0x7C) = 0x14;
        *(s32*)((s32)evt + 0x80) = 0;
    }

    cur = *(f32*)((s32)unit + 0x154);
    target = *(f32*)((s32)unit + 0x160);
    if (cur != target) {
        diff = __fabs(target - cur);
        speed = *(f32*)((s32)unit + 0x170);
        if (diff < speed) {
            *(f32*)((s32)unit + 0x154) = target;
        } else {
            *(f32*)((s32)unit + 0x154) = cur + speed * (f32)*(s8*)((s32)unit + 0x188);
        }
    }

    cur = *(f32*)((s32)unit + 0x15C);
    target = *(f32*)((s32)unit + 0x168);
    if (cur != target) {
        diff = __fabs(target - cur);
        speed = *(f32*)((s32)unit + 0x170);
        if (diff < speed) {
            *(f32*)((s32)unit + 0x15C) = target;
        } else {
            if (target - *(f32*)((s32)unit + 0x150) <= float_0_804240b4) {
                *(f32*)((s32)unit + 0x15C) = cur - speed;
            } else {
                *(f32*)((s32)unit + 0x15C) = cur + speed;
            }
        }
    }

    frame = *(s32*)((s32)unit + 0x16C);
    phase = frame % 90;
    jumpFrames = *(f32*)((s32)unit + 0x184);
    nextSpeedY =
        float_10_804240e8 *
        (f32)sinfd(float_180_804240ec +
                   (float_360_804240f0 * (jumpFrames - (f32)phase)) / float_90_804240f4);

    BtlUnit_GetPartsPtr(unit, 1);
    *(f32*)((s32)unit + 0x158) =
        *(f32*)((s32)unit + 0x178) + *(f32*)((s32)unit + 0x14C) +
        ((*(f32*)((s32)unit + 0x164) - *(f32*)((s32)unit + 0x14C)) *
         (jumpFrames - (f32)frame)) / jumpFrames;
    *(f32*)((s32)unit + 0x178) = nextSpeedY;

    phase = *(s32*)((s32)unit + 0x16C) % 90;
    *(s32*)((s32)unit + 0x1C + 5 * 4) =
        (s32)-(float)(float_25_804240fc *
                      (f32)sinfd((float_360_804240f0 * (jumpFrames - (f32)phase)) /
                                 float_90_804240f4) -
                      float_75_804240f8);
    BtlUnit_SetPos(*(f32*)((s32)unit + 0x154), *(f32*)((s32)unit + 0x158),
                   *(f32*)((s32)unit + 0x15C), unit);

    *(s32*)((s32)evt + 0x7C) = *(s32*)((s32)evt + 0x7C) + 1;
    if (*(s32*)((s32)evt + 0x7C) > 0x14) {
        f32 xOffset;
        *(s32*)((s32)evt + 0x7C) = 0;
        xOffset = float_50_80424104;
        if (*(s8*)((s32)unit + 0x188) >= 0) {
            xOffset = float_neg50_80424100;
        }
        height = (f32)BtlUnit_GetHeight(unit);
        *(s32*)((s32)evt + 0x80) =
            effSweatN64Entry(*(f32*)((s32)unit + 0x154),
                             *(f32*)((s32)unit + 0x114) * float_0p5_80424108 * height +
                                 *(f32*)((s32)unit + 0x158),
                             *(f32*)((s32)unit + 0x15C),
                             float_5_804240e0 * *(f32*)((s32)unit + 0x114), xOffset, 1, 0x10);
    } else {
        effect = *(s32*)((s32)evt + 0x80);
        if (*(s32*)((s32)evt + 0x7C) < 0x10) {
            *(f32*)(*(s32*)((s32)effect + 0xC) + 4) = *(f32*)((s32)unit + 0x154);
            height = (f32)BtlUnit_GetHeight(unit);
            *(f32*)(*(s32*)((s32)effect + 0xC) + 8) =
                *(f32*)((s32)unit + 0x114) * float_0p5_80424108 * height +
                *(f32*)((s32)unit + 0x158);
            *(f32*)(*(s32*)((s32)effect + 0xC) + 0xC) = *(f32*)((s32)unit + 0x15C);
        }
    }

    *(s32*)((s32)unit + 0x16C) = *(s32*)((s32)unit + 0x16C) - 1;
    if (*(s32*)((s32)unit + 0x16C) < 1) {
        BtlUnit_SetPos(*(f32*)((s32)unit + 0x160), *(f32*)((s32)unit + 0x164),
                       *(f32*)((s32)unit + 0x168), unit);
        return 2;
    }
    return 0;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


void _yoshi_slide_move_sound(void* unit, void* move) {
    extern void BtlUnit_snd_se(void* unit, s32 soundId, s32 unk0, s32 volume, s32 unk1);
    s32 volume = (s16)((*(s32*)((s32)unit + 0x228) * *(s32*)((s32)unit + 0x22C)) / 100);

    BtlUnit_snd_se(unit, *(s32*)((s32)move + 0xC), -250000000, volume, 0x51EB851F);
}

s32 _get_nomikomi_hit_position(int param_1) {
    extern f32 evtSetFloat(void* evt, s32 arg, f32 value);

    void* evt = (void*)param_1;
    s32* args = *(s32**)((s32)evt + 0x18);
    void* battleWork = _battleWorkPointer;
    s32 id0;
    s32 id1;
    s32 outX;
    s32 outY;
    s32 outZ;
    void* unit;
    s8 dir;
    f32 z;
    f32 y;
    f32 x;
    f32 scale;

    id0 = evtGetValue(evt, args[0]);
    id1 = evtGetValue(evt, args[1]);
    evtGetValue(evt, args[2]);
    outX = args[3];
    outY = args[4];
    outZ = args[5];
    BattleGetUnitPtr(battleWork, BattleTransID(evt, id0));
    unit = BattleGetUnitPtr(battleWork, BattleTransID(evt, id1));
    dir = *(s8*)((s32)unit + 0x189);
    BtlUnit_GetPos(unit, &x, &y, &z);
    scale = *(f32*)((s32)unit + 0x114);
    x += (f32)dir * (*(f32*)((s32)unit + 0xEC) * scale);
    y += *(f32*)((s32)unit + 0xF0) * scale;
    z += *(f32*)((s32)unit + 0xF4) * scale;
    evtSetFloat(evt, outX, x);
    evtSetFloat(evt, outY, y);
    evtSetFloat(evt, outZ, z);
    return 2;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 _get_swallow_param(void* evt) {

    void* battleWork = _battleWorkPointer;
    s32* args = *(s32**)((s32)evt + 0x18);
    s32 id = evtGetValue(evt, args[0]);
    s32 transId = BattleTransID(evt, id);
    s32 out = args[1];
    void* unit = BattleGetUnitPtr(battleWork, transId);
    evtSetValue(evt, out, *(s8*)((s32)unit + 0x25));
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on



#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 _check_swallow_attribute(void* evt) {

    s32* args = *(s32**)((s32)evt + 0x18);
    void* battleWork = _battleWorkPointer;
    s32 id = evtGetValue(evt, args[0]);
    s32 transId = BattleTransID(evt, id);
    u16 mask = args[1];
    s32 out = args[2];
    void* unit = BattleGetUnitPtr(battleWork, transId);

    if (*(u8*)((s32)unit + 0x26) & mask) {
        evtSetValue(evt, out, 1);
    } else {
        evtSetValue(evt, out, 0);
    }
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


s32 _wait_yoshig_run(void* evt) {
    s32* args = *(s32**)((s32)evt + 0x18);
    s32 id = evtGetValue(evt, args[0]);
    void* unit = BattleGetUnitPtr(_battleWorkPointer, BattleTransID(evt, id));
    void* weapon = *(void**)((s32)unit + 0x314);

    if (weapon == NULL) {
        return 0;
    }
    if (*(f32*)((s32)weapon + 0x3008) <= float_0_804240b4) {
        return 0;
    }
    return 2;
}
u32 _wait_yoshig_complete(void* evt) {
    s32 id;
    void* unit;
    void* weapon;

    id = evtGetValue(evt, **(s32**)((s32)evt + 0x18));
    id = BattleTransID(evt, id);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    weapon = *(void**)((s32)unit + 0x314);
    if (weapon != NULL) {
        return 0;
    }
    return 2;
}


u8 _gundan_yoshi_run_effect(void* evt, s32 first) {
    typedef struct LocalVec {
        f32 x;
        f32 y;
        f32 z;
    } LocalVec;

    extern void* BattleAlloc(s32 size);
    extern void BattleFree(void* ptr);
    extern void* memset(void* ptr, s32 value, u32 size);
    extern s32 psndSFXOn(char* name);
    extern void psndSFXOff(s32 id);
    extern void psndSFX_pos(s32 id, LocalVec* pos);
    extern s32 irand(s32 max);
    extern void effKemuTestEntry(f32 x, f32 y, f32 z, f32 scale, s32 type);
    extern f32 dispCalcZ(void* pos);
    extern void dispEntry(s32 cameraId, s32 renderMode, void* callback, void* param, f32 z);
    extern void _btlYoshiDisp(void* camera, void* work);
    extern const char str_SFX_BTL_YOSHI_TAIGUN_802f7cb8[];
    extern f32 float_3_804240bc;
    extern f32 float_0p1_804240c0;
    extern f32 float_0p001_804240c4;
    extern f32 float_65_804240c8;
    extern f32 float_neg1_804240cc;
    extern f32 float_neg80_804240d0;
    extern f32 float_300_804240d4;
    extern f32 float_0p05_804240dc;
    extern f32 float_0p0039062_804240e4;

    s32* args;
    void* unit;
    s32* work;
    s32* p;
    s32 i;
    s32 active;
    LocalVec pos;

    args = *(s32**)((s32)evt + 0x18);
    unit = BattleGetUnitPtr(_battleWorkPointer,
                            BattleTransID(evt, evtGetValue(evt, args[0])));
    if (first != 0) {
        work = BattleAlloc(0x300C);
        *(void**)((s32)unit + 0x314) = work;
        memset(work, 0, 0x300C);
        work[0] = 0;
        *(f32*)((s32)work + 0x3008) = float_0_804240b4;
        *(s32*)((s32)evt + 0x78) = 0;
        *(s32*)((s32)evt + 0x7C) = psndSFXOn((char*)str_SFX_BTL_YOSHI_TAIGUN_802f7cb8);
    } else {
        work = *(s32**)((s32)unit + 0x314);
    }

    if (work[0] == 1) {
        goto process;
    }
    if (work[0] < 1) {
        if (work[0] == 0) {
            work[0]++;
            goto process;
        }
        return 0;
    }
    if (work[0] == 999) {
        psndSFXOff(*(s32*)((s32)evt + 0x7C));
        BattleFree(*(void**)((s32)unit + 0x314));
        *(void**)((s32)unit + 0x314) = NULL;
        return 2;
    }
    return 0;

process:
    active = 0;
    *(f32*)((s32)work + 0x3008) = float_0_804240b4;
    p = work + 1;
    for (i = 0; i < 0x100; i++, p += 0xC) {
        *(u8*)((s32)p + 0x24) = *(u8*)((s32)p + 0x24) + 1;
        if (*(u8*)((s32)p + 0x24) > 2) {
            *(u8*)((s32)p + 0x24) = 0;
            *(u8*)((s32)p + 0x25) = *(u8*)((s32)p + 0x25) + 1;
            if (*(u8*)((s32)p + 0x25) >= 6) {
                *(u8*)((s32)p + 0x25) = 0;
            }
        }

        if (p[0xC] == 0) {
            p[0xC] = 1;
            p[0xB] = irand(3);
            *(f32*)((s32)p + 0x0) = (f32)(irand(10) - (i / 16) * 30 - 300);
            *(f32*)((s32)p + 0x4) = float_0_804240b4;
            *(f32*)((s32)p + 0x8) = (f32)(((i % 16) * 10) - 0x50);
            *(u8*)((s32)p + 0x25) = (u8)irand(6);
            *(u8*)((s32)p + 0x24) = (u8)irand(5);
            *(f32*)((s32)p + 0x10) = float_0p1_804240c0 * (f32)irand(10) + float_3_804240bc;
            *(f32*)((s32)p + 0x14) = float_0_804240b4;
            *(f32*)((s32)p + 0x18) = float_0p001_804240c4 * (f32)(irand(2000) - 1000);
        }

        if (p[0xC] == 1 || p[0xC] == 2) {
            if (p[0xB] > 0) {
                p[0xB]--;
            } else {
                *(f32*)((s32)p + 0x8) += *(f32*)((s32)p + 0x18);
                if (*(f32*)((s32)p + 0x8) >= float_65_804240c8) {
                    *(f32*)((s32)p + 0x8) = float_65_804240c8;
                    *(f32*)((s32)p + 0x18) *= float_neg1_804240cc;
                }
                if (*(f32*)((s32)p + 0x8) <= float_neg80_804240d0) {
                    *(f32*)((s32)p + 0x8) = float_neg80_804240d0;
                    *(f32*)((s32)p + 0x18) *= float_neg1_804240cc;
                }
                *(f32*)((s32)p + 0x0) += *(f32*)((s32)p + 0x10);
                if (*(f32*)((s32)p + 0x0) >= float_300_804240d4) {
                    *(f32*)((s32)p + 0x0) = float_300_804240d4;
                    *(f32*)((s32)p + 0x10) = float_0_804240b4;
                    p[0xC] = 999;
                }
            }
            active = 1;
        }
        *(f32*)((s32)work + 0x3008) += *(f32*)((s32)p + 0x0);
    }

    if (*(s32*)((s32)evt + 0x78) < 1) {
        p = work + 1 + irand(0x100) * 0xC;
        effKemuTestEntry(*(f32*)((s32)p + 0x0),
                         float_5_804240e0 + *(f32*)((s32)p + 0x4) + (f32)irand(0xF),
                         float_65_804240c8,
                         float_0p05_804240dc * (f32)irand(0x14) + float_1_804240d8,
                         0);
    } else {
        *(s32*)((s32)evt + 0x78) = *(s32*)((s32)evt + 0x78) - 1;
    }

    pos.y = float_0_804240b4;
    *(f32*)((s32)work + 0x3008) *= float_0p0039062_804240e4;
    pos.x = *(f32*)((s32)work + 0x3008);
    pos.z = pos.y;
    psndSFX_pos(*(s32*)((s32)evt + 0x7C), &pos);
    if (active == 0) {
        work[0] = 999;
    }

    p = work + 1;
    for (i = 0; i < 0x100; i++, p += 0xC) {
        dispEntry(4, 1, _btlYoshiDisp, p, dispCalcZ(p));
    }
    return 0;
}


void _btlYoshiDisp(void* camera, void* work) {
    extern f32 float_0p4_804240ac;
    extern f32 float_neg0p4_804240b0;
    extern f32 float_20_804240b8;
    extern s32 dat_804240a8;
    extern u8 _yoshig_tpl_no_tbl[6];
    extern void btlDispTexPlane(s32 tpl, void* color, s32 flags, f32 x, f32 y, f32 z, f32 rx, f32 scale);
    s32 color;
    f32 rx;

    if (*(f32*)((s32)work + 0xC) < float_0_804240b4) {
        rx = float_0p4_804240ac;
    } else {
        rx = float_neg0p4_804240b0;
    }
    color = dat_804240a8;
    btlDispTexPlane(((u8*)&_yoshig_tpl_no_tbl)[*(u8*)((s32)work + 0x25)], &color, 0,
                    *(f32*)work, float_20_804240b8 + *(f32*)((s32)work + 4),
                    *(f32*)((s32)work + 8), rx, float_0p4_804240ac);
}


/* Recovered target .sdata unit metadata dependencies. */
static u8 defence[5] = {0, 0, 0, 0, 0};
static u8 defence_attr[5] = {0, 0, 0, 0, 0};
static u8 regist[22] = {
    100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
    100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100
};
u32 pose_table_egg_g[8] = {
    0x0000001C, (u32)(char*)str_CHU_C_1_802f7860, 0x0000001F, (u32)(char*)str_CHU_C_1_802f7860,
    0x00000027, (u32)(char*)str_CHU_C_1_802f7860, 0x00000045, (u32)(char*)str_CHU_C_1_802f7860
};
u32 pose_table_egg_y[8] = {
    0x0000001C, (u32)(char*)str_CHU_C_2_802f7868, 0x0000001F, (u32)(char*)str_CHU_C_2_802f7868,
    0x00000027, (u32)(char*)str_CHU_C_2_802f7868, 0x00000045, (u32)(char*)str_CHU_C_2_802f7868
};
u32 pose_table_egg_p[8] = {
    0x0000001C, (u32)(char*)str_CHU_C_3_802f7870, 0x0000001F, (u32)(char*)str_CHU_C_3_802f7870,
    0x00000027, (u32)(char*)str_CHU_C_3_802f7870, 0x00000045, (u32)(char*)str_CHU_C_3_802f7870
};
/* Recovered unit_party_yoshi event bytecode (.data/.sdata). */
/* Recovered target .data structured prefix; exact words + symbolic relocations. */
u32 pose_table_yoshi_stay[50] = {
    0x00000009, (u32)(char*)str_PYS_D_5_802f77d8, 0x00000002, (u32)(char*)str_PYS_D_5_802f77d8, 0x00000003, (u32)(char*)str_PYS_X_1_802f77e0,
    0x00000004, (u32)(char*)str_PYS_D_2_802f77e8, 0x00000005, (u32)(char*)str_PYS_X_1_802f77e0, 0x00000001, (u32)(char*)str_PYS_N_1_802f77f0,
    0x0000001B, (u32)(char*)str_PYS_D_3_802f77f8, 0x0000001C, (u32)(char*)str_PYS_S_1_802f7800, 0x00000025, (u32)(char*)str_PYS_G_2_802f7808,
    0x0000001F, (u32)(char*)str_PYS_G_1_802f7810, 0x00000027, (u32)(char*)str_PYS_D_1_802f7818, 0x00000028, (u32)(char*)str_PYS_W_1_802f7820,
    0x0000002A, (u32)(char*)str_PYS_R_1_802f7828, 0x00000039, (u32)(char*)str_PYS_D_2_802f77e8, 0x00000038, (u32)(char*)str_PYS_D_2_802f77e8,
    0x0000003C, (u32)(char*)str_PYS_D_4_802f7830, 0x0000003D, (u32)(char*)str_PYS_D_2_802f77e8, 0x00000041, (u32)(char*)str_PYS_T_1_802f7838,
    0x00000034, (u32)(char*)str_PYS_I_1_802f7840, 0x00000043, (u32)(char*)str_PYS_C_1_802f7848, 0x00000042, (u32)(char*)str_PYS_O_1_802f7850,
    0x00000040, (u32)(char*)str_PYS_F_1_802f7858, 0x00000031, (u32)(char*)str_PYS_F_1_802f7858, 0x0000002D, (u32)(char*)str_PYS_G_1_802f7810,
    0x00000045, (u32)(char*)str_PYS_S_1_802f7800
};

static u32 data_table[16] = {
    0x00000005, (u32)yoshi_appeal_event, 0x00000006, (u32)attack_audience_event, 0x00000001, (u32)__makeTechMenuFunc,
    0x00000030, (u32)btldefaultevt_Dummy, 0x00000040, (u32)party_win_reaction, 0x00000007, (u32)subset_attack_ready_jump_event,
    0x0000002F, (u32)btldefaultevt_avoid_counter_event, 0x00000000, 0x00000000
};

/* Recovered target BattleUnitKindPart/BattleUnitKind records; exact words + symbolic relocations. */
u32 unitpartsdata_Party_Yoshi[133] = {
    0x00000001, (u32)(char*)str_btl_un_yoshi_802f7878, (u32)(char*)str_c_babyyoshi_802f7888, 0x00000000, 0x00000000, 0x00000000,
    0x41800000, 0x41C00000, 0x40A00000, 0x00000000, 0x41C00000, 0x00000000,
    0x00140018, 0x00FF0000, (u32)defence, (u32)defence_attr, 0x00000009, 0x00000000,
    (u32)pose_table_yoshi_stay, 0x00000002, (u32)(char*)str_btl_un_yoshi_802f7878, (u32)(char*)str_c_tamago_802f7894, 0x00000000, 0x00000000,
    0x00000000, 0x41800000, 0x41C00000, 0x40A00000, 0x00000000, 0x41C00000,
    0x00000000, 0x00140018, 0x00FF0000, (u32)defence, (u32)defence_attr, 0x13010000,
    0x00000000, (u32)pose_table_egg_g, 0x00000003, (u32)(char*)str_btl_un_yoshi_802f7878, (u32)(char*)str_c_tamago_802f7894, 0x00000000,
    0x00000000, 0x00000000, 0x41800000, 0x41C00000, 0x40A00000, 0x00000000,
    0x41C00000, 0x00000000, 0x00140018, 0x00FF0000, (u32)defence, (u32)defence_attr,
    0x13010000, 0x00000000, (u32)pose_table_egg_g, 0x00000004, (u32)(char*)str_btl_un_yoshi_802f7878, (u32)(char*)str_c_tamago_802f7894,
    0x00000000, 0x00000000, 0x00000000, 0x41800000, 0x41C00000, 0x40A00000,
    0x00000000, 0x41C00000, 0x00000000, 0x00140018, 0x00FF0000, (u32)defence,
    (u32)defence_attr, 0x13010000, 0x00000000, (u32)pose_table_egg_g, 0x00000005, (u32)(char*)str_btl_un_yoshi_802f7878,
    (u32)(char*)str_c_tamago_802f7894, 0x00000000, 0x00000000, 0x00000000, 0x41800000, 0x41C00000,
    0x40A00000, 0x00000000, 0x41C00000, 0x00000000, 0x00140018, 0x00FF0000,
    (u32)defence, (u32)defence_attr, 0x13010000, 0x00000000, (u32)pose_table_egg_g, 0x00000006,
    (u32)(char*)str_btl_un_yoshi_802f7878, (u32)(char*)str_c_tamago_802f7894, 0x00000000, 0x00000000, 0x00000000, 0x41800000,
    0x41C00000, 0x40A00000, 0x00000000, 0x41C00000, 0x00000000, 0x00140018,
    0x00FF0000, (u32)defence, (u32)defence_attr, 0x13010000, 0x00000000, (u32)pose_table_egg_g,
    0x00000007, (u32)(char*)str_btl_un_yoshi_802f7878, (u32)(char*)str_c_tamago_802f7894, 0x00000000, 0x00000000, 0x00000000,
    0x41800000, 0x41C00000, 0x40A00000, 0x00000000, 0x41C00000, 0x00000000,
    0x00140018, 0x00FF0000, (u32)defence, (u32)defence_attr, 0x13010000, 0x00000000,
    (u32)pose_table_egg_g
};

u32 unitdata_Party_Yoshi[49] = {
    0x000000E2, (u32)(char*)str_btl_un_yoshi_802f7878, 0x000A0005, 0x05010100, 0x00000064, 0x00090014,
    0x00210008, 0x001C0000, 0x00000000, 0x41400000, 0x00000000, 0x00000000,
    0x00000000, 0x42200000, 0x00000000, 0x00000000, 0x42200000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x41200000,
    0x00000000, 0x00000000, 0x41200000, 0x41AB999A, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0000FF00, 0x64140000,
    0x00000000, 0x00000000, 0x00000000, (u32)(char*)str_SFX_PARTY_DAMAGE1_802f78a0, (u32)(char*)str_SFX_MARIO_DAMAGE_FIR_802f78b4, (u32)(char*)str_SFX_MARIO_DAMAGE_ICE_802f78cc,
    (u32)(char*)str_SFX_MARIO_DAMAGE_BIR_802f78e4, 0x02000000, (u32)regist, 0x07000000, (u32)unitpartsdata_Party_Yoshi, (u32)init_event,
    (u32)data_table
};

static u32 battle_entry_event[] = {
    0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE, 0xFE363C80, 0x000000FA, 0x0005005B,
    (u32)btlevtcmd_SetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00010009,
    0x00000028, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001, 0x0000002A,
    0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE, 0xF24A8A80, 0x0009005B, (u32)btlevtcmd_SetMoveSoundDataWork,
    0xFFFFFFFE, (u32)(char*)str_SFX_PARTY_BATTLE_SUB_802f7900, (u32)(char*)str_SFX_PARTY_BATTLE_SUB_802f791c, 0x00000000, 0x00000006, 0x00000006,
    0x00000000, 0x00000000, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000000, 0xFFFFFFFE, 0x00000000, 0x0003005B,
    (u32)btlevtcmd_AnimeChangePoseFromTable, 0xFFFFFFFE, 0x00000001, 0x00000002, 0x00000001,
};

static u32 init_event[] = {
    0x0003005B, (u32)btlevtcmd_SetEventWait, 0xFFFFFFFE, (u32)wait_event, 0x0003005B, (u32)btlevtcmd_SetEventAttack,
    0xFFFFFFFE, (u32)attack_event, 0x0003005B, (u32)btlevtcmd_SetEventDamage, 0xFFFFFFFE, (u32)damage_event,
    0x0003005B, (u32)btlevtcmd_SetEventConfusion, 0xFFFFFFFE, (u32)btldefaultevt_Confuse, 0x0004005B, (u32)btlevtcmd_SetUnitWork,
    0xFFFFFFFE, 0x00000002, 0x00000000, 0x0007005B, (u32)btlevtcmd_SetWalkSound, 0xFFFFFFFE,
    (u32)(char*)str_SFX_PARTY_BATTLE_MOV_802f7938, (u32)(char*)str_SFX_PARTY_BATTLE_MOV_802f7950, 0x00000000, 0x0000000F, 0x0000000D, 0x0007005B,
    (u32)btlevtcmd_SetRunSound, 0xFFFFFFFE, (u32)(char*)str_SFX_PARTY_BATTLE_MOV_802f7938, (u32)(char*)str_SFX_PARTY_BATTLE_MOV_802f7950, 0x00000000, 0x00000006,
    0x00000006, 0x0004005B, (u32)btlevtcmd_SetJumpSound, 0xFFFFFFFE, (u32)(char*)str_SFX_PARTY_BATTLE_JUM_802f7968, (u32)(char*)str_SFX_PARTY_BATTLE_LAN_802f7980,
    0x0002005B, (u32)yoshi_original_color_anim_set, 0xFFFFFFFE, 0x0003005B, (u32)btlevtcmd_SetEventEntry, 0xFFFFFFFE,
    (u32)battle_entry_event, 0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002, 0x00000001,
};

static u32 damage_event[] = {
    0x0005005B, (u32)btlevtcmd_SetRotate, 0xFFFFFFFE, 0x00000000, 0x00000000, 0x00000000,
    0x00020032, 0xFE363C8A, 0xFFFFFFFE, 0x00020032, 0xFE363C8B, 0x00000001,
    0x0001005E, (u32)btldefaultevt_Damage, 0x00000002, 0x00000001,
};

/* Recovered target .data BattleWeapon / pose-sound block; exact words + symbolic relocations. */
u32 partyWeapon_YoshiNormalAttack[48] = {
    (u32)(char*)str_btl_wn_pys_normal_802f799c, 0x01580000, 0x00000000, (u32)(char*)str_msg_pys_hip_drop_802f79b0, 0x64000000, 0x3F800000,
    0x01016432, (u32)weaponGetPowerFromPartyAttackLv, 0x00000001, 0x00000001, 0x00000001, 0x00000001,
    0x00000001, 0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01001260, 0x20001004, 0x00000302, (u32)(char*)str_msg_ac_hip_drop_802f79c4, 0x00035097,
    0x0000000C, 0x80002004, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)partyYoshiAttack_NormalAttack, 0x000A0A64, 0x0A0A0505, 0x05050000
};

u32 partyWeapon_YoshiNomikomi_Shot[48] = {
    (u32)(char*)str_btl_wn_pys_lv1_802f79d4, 0x01590000, 0x00000000, (u32)(char*)str_msg_pys_nomikomi_802f79e4, 0x64040000, 0x3F800000,
    0x01056432, (u32)weaponGetPowerFromPartyAttackLv, 0x00000002, 0x00000004, 0x00000003, 0x00000005,
    0x00000003, 0x00000006, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01101260, 0x21002000, 0x00060302, (u32)(char*)str_msg_ac_nomikomi_802f79f8, 0x00020047,
    0x000007FB, 0x80002004, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)partyYoshiAttack_Nomikomi, 0x00000064, 0x000A0500, 0x00000000
};

u32 partyWeapon_YoshiNomikomi_Spew[48] = {
    (u32)(char*)str_btl_wn_pys_lv1_802f79d4, 0x01590000, 0x00000000, (u32)(char*)str_msg_pys_nomikomi_802f79e4, 0x64040000, 0x3F800000,
    0x01056432, (u32)weaponGetPowerFromPartyAttackLv, 0x00000002, 0x00000004, 0x00000003, 0x00000005,
    0x00000003, 0x00000006, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01101260, 0x21002000, 0x00050302, (u32)(char*)str_msg_ac_nomikomi_802f79f8, 0x00020047,
    0x000007FF, 0x80002004, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)partyYoshiAttack_Nomikomi, 0x00000064, 0x000A0505, 0x00000000
};

u32 partyWeapon_YoshiNomikomi_Dmg0[48] = {
    (u32)(char*)str_btl_wn_pys_lv1_802f79d4, 0x01590000, 0x00000000, (u32)(char*)str_msg_pys_nomikomi_802f79e4, 0x64020000, 0x3F800000,
    0x01056432, (u32)weaponGetPowerDefault, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01101260, 0x21002000, 0x00000302, (u32)(char*)str_msg_ac_nomikomi_802f79f8, 0x00020040,
    0x000007FF, 0x80002004, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)partyYoshiAttack_Nomikomi, 0x00000064, 0x00000000, 0x00000000
};

u32 partyWeapon_YoshiNomikomi_Fire[48] = {
    (u32)(char*)str_btl_wn_pys_lv1_802f79d4, 0x01590000, 0x00000000, (u32)(char*)str_msg_pys_nomikomi_802f79e4, 0x64020000, 0x3F800000,
    0x01056432, (u32)weaponGetPowerDefault, 0x00000001, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x02101260, 0x20000000, 0x01000302, (u32)(char*)str_msg_ac_nomikomi_802f79f8, 0x00020040,
    0x000007FF, 0x80000004, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)partyYoshiAttack_Nomikomi, 0x00000064, 0x00000000, 0x00000000
};

u32 partyWeapon_YoshiNomikomi_Involved[48] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x64000000, 0x00000000,
    0x00000000, (u32)weaponGetPowerFromPartyAttackLv, 0x00000001, 0x00000004, 0x00000002, 0x00000005,
    0x00000003, 0x00000006, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x02100260, 0x20102000, 0x00000302, 0x00000000, 0x00020040,
    0x000007FF, 0x00000004, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000064, 0x00000000, 0x00000000
};

u32 partyWeapon_YoshiEggAttack_Minimini[48] = {
    (u32)(char*)str_btl_wn_pys_lv2_802f7a08, 0x015A0000, 0x00000000, (u32)(char*)str_msg_pys_wonder_egg_802f7a18, 0x64030000, 0x3F800000,
    0x01056432, (u32)weaponGetPowerDefault, 0x00000001, 0x00000001, 0x00000001, 0x00000001,
    0x00000001, 0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x02101260, 0x20000000, 0x00000302, (u32)(char*)str_msg_ac_wonder_egg_802f7a2c, 0x00020003,
    0x000007FF, 0x80002000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000064, 0x03FE0000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)partyYoshiAttack_EggAttack, 0x00000064, 0x00060300, 0x00000000
};

u32 partyWeapon_YoshiCallGuard[48] = {
    (u32)(char*)str_btl_wn_pys_lv3_802f7a40, 0x015B0000, 0x00000000, (u32)(char*)str_msg_pys_taigun_yoshi_802f7a50, 0x64060000, 0x3F800000,
    0x01056432, (u32)weaponGetPowerFromPartyAttackLv, 0x00000001, 0x00000001, 0x00000001, 0x00000001,
    0x00000001, 0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x02101260, 0x20000000, 0x00000302, (u32)(char*)str_msg_ac_taigun_yoshi_802f7a68, 0x00020017,
    0x000007FF, 0x80002004, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)partyYoshiAttack_CallGuard, 0x00141464, 0x14280505, 0x14050000
};

u32 posesound_normal_attack[75] = {
    (u32)(char*)str_PYS_W_1_802f7820, 0x3C23D70A, (u32)_yoshi_slide_move_sound, (u32)(char*)str_SFX_PARTY_BATTLE_MOV_802f7938, 0x00000001, (u32)(char*)str_PYS_W_1_802f7820,
    0x3E800000, (u32)_yoshi_slide_move_sound, (u32)(char*)str_SFX_PARTY_BATTLE_MOV_802f7950, 0x00000001, (u32)(char*)str_PYS_R_1_802f7828, 0x3C23D70A,
    (u32)_yoshi_slide_move_sound, (u32)(char*)str_SFX_PARTY_BATTLE_MOV_802f7938, 0x00000001, (u32)(char*)str_PYS_R_1_802f7828, 0x3DCCCCCD, (u32)_yoshi_slide_move_sound,
    (u32)(char*)str_SFX_PARTY_BATTLE_MOV_802f7950, 0x00000001, (u32)(char*)str_PYS_R_4_802f7a7c, 0x3C23D70A, (u32)_yoshi_slide_move_sound, (u32)(char*)str_SFX_PARTY_BATTLE_MOV_802f7938,
    0x05540001, (u32)(char*)str_PYS_R_4_802f7a7c, 0x3D4CCCCD, (u32)_yoshi_slide_move_sound, (u32)(char*)str_SFX_PARTY_BATTLE_MOV_802f7950, 0x05540001,
    (u32)(char*)str_PYS_R_4_802f7a7c, 0x3DCCCCCD, (u32)_yoshi_slide_move_sound, (u32)(char*)str_SFX_PARTY_BATTLE_MOV_802f7938, 0x05540001, (u32)(char*)str_PYS_R_4_802f7a7c,
    0x3E19999A, (u32)_yoshi_slide_move_sound, (u32)(char*)str_SFX_PARTY_BATTLE_MOV_802f7950, 0x05540001, (u32)(char*)str_PYS_R_5_802f7a84, 0x3C23D70A,
    (u32)_yoshi_slide_move_sound, (u32)(char*)str_SFX_PARTY_BATTLE_MOV_802f7938, 0x09530001, (u32)(char*)str_PYS_R_5_802f7a84, 0x3D088889, (u32)_yoshi_slide_move_sound,
    (u32)(char*)str_SFX_PARTY_BATTLE_MOV_802f7950, 0x09530001, (u32)(char*)str_PYS_R_5_802f7a84, 0x3DAAAAAB, (u32)_yoshi_slide_move_sound, (u32)(char*)str_SFX_PARTY_BATTLE_MOV_802f7938,
    0x09530001, (u32)(char*)str_PYS_R_5_802f7a84, 0x3DEEEEEF, (u32)_yoshi_slide_move_sound, (u32)(char*)str_SFX_PARTY_BATTLE_MOV_802f7950, 0x09530001,
    (u32)(char*)str_PYS_A_3_802f7a8c, 0x3C23D70A, (u32)_yoshi_slide_move_sound, (u32)(char*)str_SFX_PARTY_BATTLE_MOV_802f7938, 0x09530001, (u32)(char*)str_PYS_A_3_802f7a8c,
    0x3DAAAAAB, (u32)_yoshi_slide_move_sound, (u32)(char*)str_SFX_PARTY_BATTLE_MOV_802f7950, 0x09530001, 0x00000000, 0x00000000,
    (u32)_yoshi_slide_move_sound, 0x00000000, 0x00000001
};

u32 partyYoshiAttack_NormalAttack[] = {
    0x0003005B, (u32)btlevtcmd_GetSelectEnemy, 0xFE363C83, 0xFE363C84, 0x00020018, 0xFE363C83,
    0xFFFFFFFF, 0x00010004, 0x00000063, 0x00000021, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress,
    0xFFFFFFFE, 0xFE363C8C, 0x0002005B, (u32)btlevtcmd_WeaponAftereffect, 0xFE363C8C, 0x0004005B,
    (u32)btlevtcmd_AttackDeclare, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0x0001005B, (u32)btlevtcmd_WaitGuardMove,
    0x0002005B, (u32)btlevtcmd_CommandPayWeaponCost, 0xFFFFFFFE, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFFFFFFFE,
    0x00000007, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000007, 0x0004005B,
    (u32)evt_btl_camera_set_homing_unit, 0x00000000, 0xFFFFFFFE, 0xFFFFFFFF, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv,
    0x00000000, 0x00000002, 0x0003005B, (u32)evt_btl_camera_set_zoom, 0x00000000, 0x000000FA,
    0x00010009, 0x00000014, 0x0007005B, (u32)btlevtcmd_CalculateFaceDirection, 0xFFFFFFFE, 0xFFFFFFFF,
    0xFE363C83, 0xFE363C84, 0x00000010, 0xFE363C8F, 0x0003005B, (u32)btlevtcmd_ChangeFaceDirection,
    0xFFFFFFFE, 0xFE363C8F, 0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000002,
    0x00000001, 0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000003, 0xFFFFFFFF,
    0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000004, 0x00000000, 0x0004005B,
    (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000005, 0x00000064, 0x0004005B, (u32)btlevtcmd_AnimeSetPoseSoundTable,
    0xFFFFFFFE, 0x00000001, (u32)posesound_normal_attack, 0x0003005B, (u32)btlevtcmd_AcSetOutputParam, 0x00000001,
    0x00000000, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PYS_W_1_802f7820,
    0x0000006E, 0x00020032, 0xFE363C80, 0x00000000, 0x00010005, 0x00000019,
    0x00020035, 0xFE363C80, 0x00000004, 0x00020032, 0xFE363C81, 0x00000953,
    0x00020037, 0xFE363C81, 0xFE363C80, 0x00020038, 0xFE363C81, 0x00000064,
    0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000004, 0xFE363C81, 0x00010009,
    0x00000001, 0x00000006, 0x00000070, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE,
    0x00000001, (u32)(char*)str_PYS_R_5_802f7a84, 0x00010009, 0x00000019, 0x0003005B, (u32)evt_btl_camera_set_mode,
    0x00000000, 0x0000000B, 0x0006005B, (u32)evt_btl_camera_set_homing_unitparts, 0x00000000, 0xFFFFFFFE,
    0x00000001, 0xFE363C83, 0xFE363C84, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000,
    0x00000001, 0x0003005B, (u32)evt_btl_camera_set_zoom, 0x00000000, 0x0000015E, 0x0003005B,
    (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE, 0xF24A8A80, 0x0007005B, (u32)btlevtcmd_GetTakeoffPosition, 0xFFFFFFFE,
    0xFE363C83, 0xFE363C84, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0007005B,
    (u32)btlevtcmd_CalculateFaceDirection, 0xFFFFFFFE, 0xFFFFFFFF, 0xFE363C80, 0xFE363C82, 0x00000001,
    0xFE363C8F, 0x0003005B, (u32)btlevtcmd_ChangeFaceDirection, 0xFFFFFFFE, 0xFE363C8F, 0x0007005B,
    (u32)btlevtcmd_CalculateFaceDirection, 0xFFFFFFFE, 0xFFFFFFFF, 0xFE363C83, 0xFE363C84, 0x00000010,
    0xFE363C8F, 0x0003005B, (u32)btlevtcmd_ChangeFaceDirection, 0xFFFFFFFE, 0xFE363C8F, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PYS_J_1A_802f7a94, 0x0003005B, (u32)btlevtcmd_SetFallAccel,
    0xFFFFFFFE, 0xF24A7BB3, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0004005B, (u32)btlevtcmd_FaceDirectionAdd, 0xFFFFFFFE, 0xFE363C80,
    0x00000019, 0x00020035, 0xFE363C81, 0x00000023, 0x0006005B, (u32)btlevtcmd_snd_se,
    0xFFFFFFFE, (u32)(char*)str_SFX_PARTY_BATTLE_JUM_802f7968, 0xF1194D80, 0x00000000, 0xF1194D80, 0x0001006F,
    0xFE363C8F, 0x0002005B, (u32)btlevtcmd_CommandGetWeaponActionLv, 0xFE363C8F, 0x0003005B, (u32)btlevtcmd_AcSetDifficulty,
    0xFFFFFFFE, 0xFE363C8F, 0x0003005B, (u32)btlevtcmd_GetPartyTechLv, 0xFFFFFFFE, 0xFE363C80,
    0x00010022, 0xFE363C80, 0x00010024, 0x00000002, 0x0009005B, (u32)btlevtcmd_AcSetParamAll,
    0x00000005, 0x000000B4, 0x000000B2, 0x00000017, 0x00000019, 0x00000014,
    0x0000003D, 0x00000001, 0x0005005B, (u32)btlevtcmd_AcSetGaugeParam, 0x0000003C, 0x00000050,
    0x00000064, 0x00000064, 0x00010024, 0x00000001, 0x0009005B, (u32)btlevtcmd_AcSetParamAll,
    0x00000005, 0x000000B4, 0x000000B2, 0x00000017, 0x00000019, 0x00000019,
    0x0000004C, 0x00000001, 0x0005005B, (u32)btlevtcmd_AcSetGaugeParam, 0x0000004B, 0x00000064,
    0x00000064, 0x00000064, 0x0000002A, 0x0009005B, (u32)btlevtcmd_AcSetParamAll, 0x00000005,
    0x000000B4, 0x000000B2, 0x00000017, 0x00000019, 0x00000022, 0x00000064,
    0x00000001, 0x0005005B, (u32)btlevtcmd_AcSetGaugeParam, 0x00000064, 0x00000064, 0x00000064,
    0x00000064, 0x00000031, 0x0002005B, (u32)btlevtcmd_AcSetFlag, 0x00000048, 0x0005005B,
    (u32)btlevtcmd_SetupAC, 0xFFFFFFFE, 0x00000006, 0x00000001, 0x00000000, 0x00000070,
    0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00000019, 0xFFFFFFFF, 0x0002005B, (u32)btlevtcmd_WaitEventEnd, 0xFE363C8F, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PYS_A_3_802f7a8c, 0x0006005B, (u32)btlevtcmd_GetHitPos,
    0xFE363C83, 0xFE363C84, 0xFE363C86, 0xFE363C87, 0xFE363C88, 0x00020035,
    0xFE363C87, 0x00000028, 0x00020032, 0xFE363C8A, 0xF24A7E80, 0x0001006F,
    0xFE363C8F, 0x0002005B, (u32)btlevtcmd_StartAC, 0x00000001, 0x0001005B, (u32)btlevtcmd_ResultAC,
    0x00000070, 0x0007005B, (u32)btl_yoshi_yoroyoro_jump_calc_param, 0xFFFFFFFE, 0xFE363C86, 0xFE363C87,
    0xFE363C88, 0xFE363C8A, 0xFE363C89, 0x0007005B, (u32)btl_yoshi_yoroyoro_jump_move, 0xFFFFFFFE,
    0xFE363C86, 0xFE363C87, 0xFE363C88, 0xFE363C8A, 0xFE363C89, 0x0002005B,
    (u32)btlevtcmd_WaitEventEnd, 0xFE363C8F, 0x00020032, 0xFE363C8D, 0x00000001, 0x0003005B,
    (u32)btlevtcmd_AcGetOutputParam, 0x00000002, 0xFE363C8E, 0x00010003, 0x00000032, 0x0002001D,
    0xFE363C8D, 0x00000002, 0x0006005B, (u32)btlevtcmd_CommandPreCheckDamage, 0xFFFFFFFE, 0xFE363C83,
    0xFE363C84, 0x00100100, 0xFE363C85, 0x00000020, 0x0006005B, (u32)btlevtcmd_CommandPreCheckDamage,
    0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0x00000100, 0xFE363C85, 0x00000021,
    0x00010022, 0xFE363C85, 0x0001002B, 0x00000002, 0x0001002B, 0x00000003,
    0x0001002B, 0x00000006, 0x0001002B, 0x00000004, 0x0000002E, 0x00010024,
    0x00000001, 0x0000002A, 0x00000031, 0x0004005B, (u32)btlevtcmd_AnimeSetPoseSoundTable, 0xFFFFFFFE,
    0x00000001, 0x00000000, 0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000002,
    0x00000000, 0x0002001A, 0xFE363C8D, 0x00000002, 0x0006005B, (u32)btlevtcmd_ACRStart,
    0xFFFFFFFE, 0x00000000, 0x00000014, 0x00000014, 0x00000000, 0x0003005B,
    (u32)btlevtcmd_ACRGetResult, 0xFE363C86, 0xFE363C87, 0x00000020, 0x00020032, 0xFE363C86,
    0x00000001, 0x00000021, 0x00020018, 0xFE363C8D, 0x00000001, 0x0003005B,
    (u32)btlevtcmd_GetFaceDirection, 0xFFFFFFFE, 0xFE363C8F, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE,
    0x00000001, (u32)(char*)str_PYS_J_1B_802f7aa0, 0x00020018, 0xFE363C86, 0x00000002, 0x00020032,
    0xFE363C80, 0x00000003, 0x00020032, 0xFE363C81, 0x00000006, 0x00020037,
    0xFE363C8F, 0xFFFFFFC4, 0x00000020, 0x00020032, 0xFE363C80, 0x00000001,
    0x00020032, 0xFE363C81, 0x00000009, 0x00020037, 0xFE363C8F, 0xFFFFFFD8,
    0x00000021, 0x0005005B, (u32)btlevtcmd_SetRotateOffset, 0xFFFFFFFE, 0x00000000, 0x0000000C,
    0x00000000, 0x0005005B, (u32)btlevtcmd_SetRotate, 0xFFFFFFFE, 0x00000000, 0x00000000,
    0x00000000, 0x00010009, 0x0000000F, 0x00020018, 0xFE363C86, 0x00000002,
    0x0006005B, (u32)btlevtcmd_AudienceDeclareAcrobatResult, 0xFE363C8C, 0x00000001, 0xFFFFFFC4, 0xFFFFFFE7,
    0x00000000, 0x00000021, 0x00010005, 0xFE363C80, 0x00020032, 0xFE363C82,
    0xFE363C81, 0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_BTL_YOSHI_ATTACK_802f7aac, 0x00000000, 0x00010005,
    0xFE363C82, 0x0005005B, (u32)btlevtcmd_AddRotate, 0xFFFFFFFE, 0x00000000, 0x00000000,
    0xFE363C8F, 0x00010009, 0x00000001, 0x00000006, 0x00000006, 0x0005005B,
    (u32)btlevtcmd_SetRotate, 0xFFFFFFFE, 0x00000000, 0x00000000, 0x00000000, 0x0005005B,
    (u32)btlevtcmd_SetRotateOffset, 0xFFFFFFFE, 0x00000000, 0x00000000, 0x00000000, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PYS_A_1_802f7ac4, 0x00010009, 0x0000000F,
    0x00000020, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PYS_A_1_802f7ac4,
    0x00000021, 0x0006005B, (u32)btlevtcmd_GetHitPos, 0xFE363C83, 0xFE363C84, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x000A005B, (u32)btlevtcmd_DivePosition, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000004, 0x00000000, 0x00000000, 0x00000000,
    0xFFFFFFFF, 0x00020019, 0xFE363C85, 0x00000001, 0x0001005B, (u32)btlevtcmd_StopAC,
    0x00020018, 0xFE363C85, 0x00000003, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83,
    0x00000026, 0x00000021, 0x00020018, 0xFE363C85, 0x00000006, 0x0003005B,
    (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000027, 0x00000021, 0x00020018, 0xFE363C85,
    0x00000002, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000028, 0x00000021,
    0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00020032, 0xFE363C8F, 0xFE363C81, 0x0004005B, (u32)btlevtcmd_TransStageFloorPosition, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00020036, 0xFE363C8F, 0xFE363C81, 0x00020038,
    0xFE363C8F, 0x0000000A, 0x000A005B, (u32)btlevtcmd_DivePosition, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0xFE363C8F, 0x00000000, 0x00000000, 0x00000000,
    0xFFFFFFFF, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PYS_D_1_802f7818,
    0x0005005B, (u32)btlevtcmd_SetDispOffset, 0xFFFFFFFE, 0x00000000, 0xFFFFFFF9, 0x00000000,
    0x0006005B, (u32)evt_btl_camera_shake_h, 0x00000000, 0x00000004, 0x00000000, 0x0000000A,
    0x0000000D, 0x00010009, 0x00000028, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE,
    0x00000001, (u32)(char*)str_PYS_A_5_802f7acc, 0x0005005B, (u32)btlevtcmd_SetDispOffset, 0xFFFFFFFE, 0x00000000,
    0x00000000, 0x00000000, 0x00010009, 0x00000036, 0x0001005B, (u32)evt_audience_ap_recovery,
    0x0001005B, (u32)btlevtcmd_InviteApInfoReport, 0x00010004, 0x0000005F, 0x00000021, 0x0003005B,
    (u32)btlevtcmd_ac_timing_flag_onoff, 0x00000001, 0x00000001, 0x00010009, 0x00000002, 0x0003005B,
    (u32)btlevtcmd_ac_timing_flag_onoff, 0x00000000, 0x00000001, 0x0002005B, (u32)btlevtcmd_GetResultAC, 0xFE363C80,
    0x0002001E, 0xFE363C80, 0x00000002, 0x0002001D, 0xFE363C8D, 0xFE363C8E,
    0x0006005B, (u32)btlevtcmd_CommandCheckDamage, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0x00020100,
    0xFE363C85, 0x00000020, 0x0006005B, (u32)btlevtcmd_CommandCheckDamage, 0xFFFFFFFE, 0xFE363C83,
    0xFE363C84, 0x00020000, 0xFE363C85, 0x00000021, 0x0003005B, (u32)btlevtcmd_GetPartyTechLv,
    0xFFFFFFFE, 0xFE363C80, 0x00010022, 0xFE363C80, 0x00010024, 0x00000002,
    0x0003005B, (u32)btlevtcmd_AcGetOutputParam, 0x00000002, 0xFE363C80, 0x00010022, 0xFE363C80,
    0x00010029, 0x00000006, 0x00020032, 0xFE363C80, 0x00000002, 0x00010029,
    0x00000004, 0x00020032, 0xFE363C80, 0x00000001, 0x00010029, 0x00000002,
    0x00020032, 0xFE363C80, 0x00000000, 0x0000002A, 0x00020032, 0xFE363C80,
    0xFFFFFFFF, 0x00000031, 0x00010024, 0x00000001, 0x0003005B, (u32)btlevtcmd_AcGetOutputParam,
    0x00000002, 0xFE363C80, 0x00010022, 0xFE363C80, 0x00010029, 0x00000004,
    0x00020032, 0xFE363C80, 0x00000001, 0x00010029, 0x00000002, 0x00020032,
    0xFE363C80, 0x00000000, 0x0000002A, 0x00020032, 0xFE363C80, 0xFFFFFFFF,
    0x00000031, 0x0000002A, 0x0003005B, (u32)btlevtcmd_AcGetOutputParam, 0x00000002, 0xFE363C80,
    0x00010022, 0xFE363C80, 0x00010029, 0x00000004, 0x00020032, 0xFE363C80,
    0x00000001, 0x00010029, 0x00000002, 0x00020032, 0xFE363C80, 0x00000000,
    0x0000002A, 0x00020032, 0xFE363C80, 0xFFFFFFFF, 0x00000031, 0x00000031,
    0x0002001D, 0xFE363C80, 0x00000000, 0x00020032, 0xFE363C81, 0xFE363C80,
    0x00020032, 0xFE363C82, 0xFE363C8D, 0x00020036, 0xFE363C82, 0x00000001,
    0x0002001B, 0xFE363C81, 0xFE363C82, 0x00020032, 0xFE363C81, 0xFE363C82,
    0x00000021, 0x0004005B, (u32)btlevtcmd_GetResultPrizeLv, 0xFE363C83, 0xFE363C81, 0xFE363C86,
    0x0006005B, (u32)btlevtcmd_GetHitPos, 0xFE363C83, 0xFE363C84, 0xFE363C89, 0xFE363C81,
    0xFE363C82, 0x0005005B, (u32)btlevtcmd_ACSuccessEffect, 0xFE363C86, 0xFE363C89, 0xFE363C81,
    0xFE363C82, 0x00020032, 0xFE363C81, 0xFE363C8D, 0x00020035, 0xFE363C81,
    0x00000001, 0x0002001B, 0xFE363C81, 0xFE363C8E, 0x0004005B, (u32)btlevtcmd_GetResultPrizeLv,
    0xFE363C83, 0xFE363C80, 0xFE363C86, 0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult, 0xFE363C8C,
    0xFE363C86, 0x00000021, 0x00000020, 0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult, 0xFE363C8C,
    0xFFFFFFFF, 0x00000021, 0x00000020, 0x0002001D, 0xFE363C8D, 0xFE363C8E,
    0x0006005B, (u32)btlevtcmd_CommandCheckDamage, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0x00000100,
    0xFE363C85, 0x00000020, 0x0006005B, (u32)btlevtcmd_CommandCheckDamage, 0xFFFFFFFE, 0xFE363C83,
    0xFE363C84, 0x00000000, 0xFE363C85, 0x00000021, 0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult,
    0xFE363C8C, 0xFFFFFFFF, 0x00000021, 0x00010003, 0x0000005A, 0x00020035,
    0xFE363C8D, 0x00000001, 0x0002001C, 0xFE363C8D, 0xFE363C8E, 0x0005005B,
    (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0003005B,
    (u32)btlevtcmd_SetFallAccel, 0xFFFFFFFE, 0xF24A7B4C, 0x00020035, 0xFE363C81, 0x00000028,
    0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00000014, 0xFFFFFFFF, 0x00010004, 0x00000032, 0x00000021, 0x0001005B,
    (u32)btlevtcmd_StopAC, 0x0003005B, (u32)btlevtcmd_SetFallAccel, 0xFFFFFFFE, 0xF24A7D4C, 0x0005005B,
    (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B,
    (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xF1194D80, 0xFE363C81, 0xF1194D80, 0x0004005B,
    (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE, 0xFE363C80, 0x0000003C, 0x0007005B, (u32)btlevtcmd_JumpPosition,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0000001E, 0xFFFFFFFF,
    0x0000006E, 0x0006005B, (u32)btlevtcmd_ACRStart, 0xFFFFFFFE, 0x00000000, 0x0000000F,
    0x0000000F, 0x00000014, 0x00000070, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE,
    0xFE363C80, 0x00000019, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000010, 0xFFFFFFFF, 0x0003005B, (u32)btlevtcmd_ACRGetResult,
    0xFE363C86, 0xFE363C87, 0x00010022, 0xFE363C86, 0x00010029, 0x00000002,
    0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress, 0xFFFFFFFE, 0xFE363C80, 0x0006005B, (u32)btlevtcmd_AudienceDeclareAcrobatResult,
    0xFE363C80, 0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PYS_Y_1_802f7ad4, 0x0003005B, (u32)btlevtcmd_AnimeWaitPlayComplete,
    0xFFFFFFFE, 0x00000001, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001,
    (u32)(char*)str_PYS_Y_1_802f7ad4, 0x0003005B, (u32)btlevtcmd_AnimeWaitPlayComplete, 0xFFFFFFFE, 0x00000001, 0x0000002A,
    0x0001005B, (u32)evt_audience_acrobat_notry, 0x00000031, 0x0001005B, (u32)evt_audience_ap_recovery, 0x0001005B,
    (u32)btlevtcmd_InviteApInfoReport, 0x00010003, 0x0000005F, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000,
    0x00000000, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001, 0x0003005B,
    (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE, 0xF24A9280, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE,
    0x00000001, 0x00000028, 0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000000, 0xFFFFFFFF, 0x00000000, 0x00010003,
    0x00000063, 0x0002005B, (u32)btlevtcmd_ResetFaceDirection, 0xFFFFFFFE, 0x0001005E, (u32)btldefaultevt_SuitoruBadgeEffect,
    0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002, 0x00000001,
};

u32 partyYoshiAttack_Nomikomi[] = {
    0x0003005B, (u32)btlevtcmd_GetSelectEnemy, 0xFE363C83, 0xFE363C84, 0x00020018, 0xFE363C83,
    0xFFFFFFFF, 0x00010004, 0x00000063, 0x00000021, 0x0004005B, (u32)btlevtcmd_SetUnitWork,
    0xFFFFFFFE, 0x00000001, 0x00000000, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress, 0xFFFFFFFE,
    0xFE363C8C, 0x0002005B, (u32)btlevtcmd_WeaponAftereffect, 0xFE363C8C, 0x0004005B, (u32)btlevtcmd_AttackDeclare,
    0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0x0001005B, (u32)btlevtcmd_WaitGuardMove, 0x0002005B,
    (u32)btlevtcmd_CommandPayWeaponCost, 0xFFFFFFFE, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFFFFFFFE, 0x00000007,
    0x0002005B, (u32)btlevtcmd_CommandGetWeaponActionLv, 0xFE363C8F, 0x0003005B, (u32)btlevtcmd_AcSetDifficulty, 0xFFFFFFFE,
    0xFE363C8F, 0x0002005B, (u32)btlevtcmd_AcGetDifficulty, 0xFE363C80, 0x00020036, 0xFE363C80,
    0x00000003, 0x00010022, 0xFE363C80, 0x00010028, 0xFFFFFFFD, 0x00020032,
    0xFE363C80, 0x000000A0, 0x00020032, 0xFE363C81, 0x00000028, 0x00010024,
    0xFFFFFFFE, 0x00020032, 0xFE363C80, 0x0000008C, 0x00020032, 0xFE363C81,
    0x0000001E, 0x00010024, 0xFFFFFFFF, 0x00020032, 0xFE363C80, 0x00000078,
    0x00020032, 0xFE363C81, 0x00000019, 0x00010024, 0x00000000, 0x00020032,
    0xFE363C80, 0x00000064, 0x00020032, 0xFE363C81, 0x00000014, 0x00010024,
    0x00000001, 0x00020032, 0xFE363C80, 0x00000050, 0x00020032, 0xFE363C81,
    0x0000000A, 0x00010024, 0x00000002, 0x00020032, 0xFE363C80, 0x00000032,
    0x00020032, 0xFE363C81, 0x00000005, 0x0000002A, 0x00020032, 0xFE363C80,
    0x0000001E, 0x00020032, 0xFE363C81, 0x00000002, 0x00000031, 0x0009005B,
    (u32)btlevtcmd_AcSetParamAll, 0x00000003, 0xFE363C80, 0xFE363C81, 0x00000000, 0x00000064,
    0xF1194D80, 0xF1194D80, 0xF1194D80, 0x0002005B, (u32)btlevtcmd_AcSetFlag, 0x0000000F,
    0x0005005B, (u32)btlevtcmd_SetupAC, 0xFFFFFFFE, 0x0000000F, 0x00000001, 0x00000000,
    0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x0000000A, 0x0006005B, (u32)evt_btl_camera_set_homing_unitparts,
    0x00000000, 0xFFFFFFFE, 0x00000001, 0xFFFFFFFF, 0x00000000, 0x0003005B,
    (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000002, 0x0003005B, (u32)evt_btl_camera_set_zoom, 0x00000000,
    0x000000FA, 0x00010009, 0x00000016, 0x0002005B, (u32)btlevtcmd_StartAC, 0x00000001,
    0x00010003, 0x00000005, 0x0003005B, (u32)btlevtcmd_AcGetOutputParam, 0x00000000, 0xFE363C80,
    0x00020018, 0xFE363C80, 0x00000000, 0x00010009, 0x00000001, 0x00010004,
    0x00000005, 0x00000021, 0x0006005B, (u32)btlevtcmd_CommandPreCheckDamage, 0xFFFFFFFE, 0xFE363C83,
    0xFE363C84, 0x00000100, 0xFE363C85, 0x00020018, 0xFE363C85, 0x00000005,
    0x0003005B, (u32)btlevtcmd_GetStatusMg, 0xFFFFFFFE, 0xFE363C86, 0x0002003C, 0xFE363C86,
    0x00000028, 0x0007005B, (u32)_get_nomikomi_hit_position, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B, (u32)btlevtcmd_FaceDirectionAdd, 0xFE363C83,
    0xFE363C80, 0xFE363C86, 0x0004005B, (u32)btlevtcmd_TransStageFloorPosition, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0007005B, (u32)btlevtcmd_CalculateFaceDirection, 0xFFFFFFFE, 0xFFFFFFFF, 0xFE363C80,
    0xFE363C82, 0x00000001, 0xFE363C8F, 0x0003005B, (u32)btlevtcmd_ChangeFaceDirection, 0xFFFFFFFE,
    0xFE363C8F, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001, 0x00000028,
    0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE, 0xF24A8680, 0x0008005B, (u32)btlevtcmd_MovePosition,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000050, 0xFFFFFFFF,
    0x00000000, 0x0006005B, (u32)btlevtcmd_CommandCheckDamage, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84,
    0x00000100, 0xFE363C85, 0x00000021, 0x0003005B, (u32)btlevtcmd_GetStatusMg, 0xFFFFFFFE,
    0xFE363C86, 0x0002003C, 0xFE363C86, 0x00000028, 0x0007005B, (u32)_get_nomikomi_hit_position,
    0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0004005B, (u32)btlevtcmd_FaceDirectionAdd, 0xFE363C83, 0xFE363C80, 0xFE363C86, 0x0004005B,
    (u32)btlevtcmd_TransStageFloorPosition, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0007005B, (u32)btlevtcmd_CalculateFaceDirection,
    0xFFFFFFFE, 0xFFFFFFFF, 0xFE363C80, 0xFE363C82, 0x00000001, 0xFE363C8F,
    0x0003005B, (u32)btlevtcmd_ChangeFaceDirection, 0xFFFFFFFE, 0xFE363C8F, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType,
    0xFFFFFFFE, 0x00000001, 0x00000028, 0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE,
    0xF24A8680, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000050, 0xFFFFFFFF, 0x00000000, 0x0007005B, (u32)btlevtcmd_CalculateFaceDirection,
    0xFFFFFFFE, 0xFFFFFFFF, 0xFE363C83, 0xFE363C84, 0x00000010, 0xFE363C8F,
    0x0003005B, (u32)btlevtcmd_ChangeFaceDirection, 0xFFFFFFFE, 0xFE363C8F, 0x0001005B, (u32)btlevtcmd_ResultAC,
    0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PYS_A_2_802f7adc, 0x0003005B,
    (u32)evt_snd_sfxon, (u32)(char*)str_SFX_BTL_YOSHI_SWALLO_802f7ae4, 0x00000000, 0x00020019, 0xFE363C85, 0x00000001,
    0x00020018, 0xFE363C85, 0x00000003, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83,
    0x00000026, 0x00000021, 0x00020018, 0xFE363C85, 0x00000006, 0x0003005B,
    (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000027, 0x00000021, 0x00020018, 0xFE363C85,
    0x00000002, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000028, 0x00000021,
    0x00010009, 0x00000032, 0x00010004, 0x0000005A, 0x00000021, 0x00010009,
    0x00000007, 0x0003005B, (u32)_get_swallow_param, 0xFE363C83, 0xFE363C87, 0x00020018,
    0xFE363C87, 0xFFFFFFFF, 0x0007005B, (u32)btlevtcmd_CheckDamage, 0xFFFFFFFE, 0xFE363C83,
    0xFE363C84, (u32)partyWeapon_YoshiNomikomi_Dmg0, 0x80000100, 0xFE363C85, 0x00010009, 0x0000002B,
    0x00010004, 0x0000005A, 0x00000021, 0x0002005B, (u32)btlevtcmd_GetResultAC, 0xFE363C80,
    0x0002001F, 0xFE363C80, 0x00000002, 0x0007005B, (u32)btlevtcmd_CheckDamage, 0xFFFFFFFE,
    0xFE363C83, 0xFE363C84, (u32)partyWeapon_YoshiNomikomi_Dmg0, 0x80000100, 0xFE363C85, 0x00010009,
    0x0000002B, 0x00010004, 0x0000005A, 0x00000021, 0x0003005B, (u32)evt_snd_sfxon,
    (u32)(char*)str_SFX_BTL_YOSHI_SWALLO_802f7afc, 0x00000000, 0x0004005B, (u32)_check_swallow_attribute, 0xFE363C83, 0x00000008,
    0xFE363C80, 0x00020019, 0xFE363C80, 0x00000000, 0x0007005B, (u32)btlevtcmd_DamageDirect,
    0xFE363C83, 0xFE363C84, 0x00000000, 0x00000000, 0x00000007, 0x00000001,
    0x00010009, 0x0000000D, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001,
    (u32)(char*)str_PYS_A_7_802f7b14, 0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_BTL_YOSHI_SWALLO_802f7b1c, 0x00000000, 0x00010009,
    0x00000022, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PYS_A_5_802f7acc,
    0x00010009, 0x00000036, 0x00010004, 0x0000005A, 0x00000021, 0x0003005B,
    (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000002, 0x0003005B, (u32)evt_btl_camera_set_zoom, 0x00000000,
    0x0000015E, 0x0007005B, (u32)btlevtcmd_DamageDirect, 0xFE363C83, 0xFE363C84, 0x00000000,
    0x00000000, 0x00000002, 0x00000001, 0x00010009, 0x00000003, 0x0005005B,
    (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B,
    (u32)btlevtcmd_FaceDirectionAdd, 0xFFFFFFFE, 0xFE363C80, 0x0000000C, 0x000A005B, (u32)btlevtcmd_DivePosition,
    0xFE363C83, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000005, 0x00000000,
    0x00000000, 0x00000000, 0xFFFFFFFF, 0x0003005B, (u32)btlevtcmd_OnAttribute, 0xFE363C83,
    0x01000000, 0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_BTL_YOSHI_SWALLO_802f7b34, 0x00000000, 0x00010009,
    0x00000006, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PYS_A_3_802f7a8c,
    0x00010009, 0x00000007, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000,
    0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000002, 0x0003005B, (u32)evt_sub_random,
    0x00000063, 0xFE363C80, 0x0002001D, 0xFE363C80, 0xFE363C87, 0x0000006B,
    0x0003005B, (u32)evt_env_blur_on, 0x00000000, 0x00000258, 0x0000006D, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PYS_A_4_802f7b4c, 0x0003005B, (u32)btlevtcmd_ftomsec,
    0x00000016, 0xFE363C80, 0x0001000A, 0xFE363C80, 0x0001006C, 0xFE363C8F,
    0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_BTL_YOSHI_NOMIKO_802f7b54, 0x00000000, 0x0003005B, (u32)btlevtcmd_OffAttribute,
    0xFE363C83, 0x01000000, 0x0004005B, (u32)_check_swallow_attribute, 0xFE363C83, 0x00000002,
    0xFE363C80, 0x00020018, 0xFE363C80, 0x00000000, 0x0007005B, (u32)btlevtcmd_CheckDamage,
    0xFFFFFFFE, 0xFE363C83, 0xFE363C84, (u32)partyWeapon_YoshiNomikomi_Spew, 0x80000100, 0xFE363C85,
    0x00000020, 0x0007005B, (u32)btlevtcmd_CheckDamage, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84,
    (u32)partyWeapon_YoshiNomikomi_Shot, 0x80000100, 0xFE363C85, 0x00000021, 0x0002005B, (u32)btlevtcmd_GetResultAC,
    0xFE363C80, 0x0002001E, 0xFE363C80, 0x00000002, 0x0004005B, (u32)btlevtcmd_GetResultPrizeLv,
    0xFE363C83, 0x00000000, 0xFE363C86, 0x0006005B, (u32)btlevtcmd_GetHitPos, 0xFE363C83,
    0xFE363C84, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)btlevtcmd_ACSuccessEffect,
    0xFE363C86, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult,
    0xFE363C8C, 0xFE363C86, 0x00000020, 0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult, 0xFE363C8C,
    0xFFFFFFFF, 0x00000021, 0x0003005B, (u32)btlevtcmd_AnimeWaitPlayComplete, 0xFFFFFFFE, 0x00000001,
    0x0006005B, (u32)btlevtcmd_ACRStart, 0xFFFFFFFE, 0x00000000, 0x0000000F, 0x0000000F,
    0x00000014, 0x0003005B, (u32)btlevtcmd_ACRGetResult, 0xFE363C86, 0xFE363C87, 0x00010022,
    0xFE363C86, 0x00010029, 0x00000002, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress, 0xFFFFFFFE,
    0xFE363C80, 0x0006005B, (u32)btlevtcmd_AudienceDeclareAcrobatResult, 0xFE363C80, 0x00000001, 0x00000000,
    0x00000000, 0x00000000, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001,
    (u32)(char*)str_PYS_Y_1_802f7ad4, 0x0003005B, (u32)btlevtcmd_AnimeWaitPlayComplete, 0xFFFFFFFE, 0x00000001, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PYS_Y_1_802f7ad4, 0x0003005B, (u32)btlevtcmd_AnimeWaitPlayComplete,
    0xFFFFFFFE, 0x00000001, 0x0000002A, 0x0001005B, (u32)evt_audience_acrobat_notry, 0x00000031,
    0x0000006D, 0x0002005B, (u32)btlevtcmd_WaitEventEnd, 0xFE363C8F, 0x00010004, 0x0000005A,
    0x00000021, 0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000001, 0x00000001,
    0x0000006E, 0x0004005B, (u32)btlevtcmd_GetResultPrizeLv, 0xFE363C83, 0x00000000, 0xFE363C86,
    0x0006005B, (u32)btlevtcmd_GetHitPos, 0xFE363C83, 0xFE363C84, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0005005B, (u32)btlevtcmd_ACSuccessEffect, 0xFE363C86, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000070, 0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_BTL_YOSHI_SWALLO_802f7b1c, 0x00000000,
    0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PYS_A_7_802f7b14, 0x00010009,
    0x0000001A, 0x0004005B, (u32)_check_swallow_attribute, 0xFE363C83, 0x00000001, 0xFE363C81,
    0x0004005B, (u32)_check_swallow_attribute, 0xFE363C83, 0x00000004, 0xFE363C82, 0x0003005B,
    (u32)btlevtcmd_GetExp, 0xFE363C83, 0xFE363C80, 0x0002005B, (u32)btlevtcmd_StoreExp, 0xFE363C80,
    0x0003005B, (u32)btlevtcmd_KillUnit, 0xFE363C83, 0x00000000, 0x00010009, 0x00000008,
    0x00020019, 0xFE363C81, 0x00000000, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE,
    0x00000001, (u32)(char*)str_PYS_A_6_802f7b6c, 0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_BTL_YOSHI_SWALLO_802f7b74, 0x00000000,
    0x00010009, 0x00000032, 0x00010004, 0x0000005A, 0x00000020, 0x00020018,
    0xFE363C82, 0x00000000, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001,
    (u32)(char*)str_PYS_A_5_802f7acc, 0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_BTL_YOSHI_SWALLO_802f7b8c, 0x00000000, 0x00010009,
    0x00000036, 0x00010004, 0x0000005A, 0x00000021, 0x00000021, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PYS_A_4_802f7b4c, 0x00010009, 0x00000016,
    0x0000006E, 0x00010009, 0x00000012, 0x00000070, 0x0005005B, (u32)btlevtcmd_GetPos,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B, (u32)btlevtcmd_FaceDirectionAdd,
    0xFFFFFFFE, 0xFE363C80, 0x0000000F, 0x00020035, 0xFE363C81, 0x0000000F,
    0x00020032, 0xFE363C83, 0xFE363C80, 0x00020032, 0xFE363C84, 0xFE363C81,
    0x00020032, 0xFE363C85, 0xFE363C82, 0x0004005B, (u32)btlevtcmd_FaceDirectionAdd, 0xFFFFFFFE,
    0xFE363C83, 0x000000F0, 0x000F005B, (u32)evt_eff, (u32)(char*)str_eff_80424090, (u32)(char*)str_breath_fire_802f7ba4,
    0x00000001, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0xFE363C83, 0xFE363C84,
    0xFE363C85, 0x00000001, 0x00000002, 0x0000003C, 0xF24A7C80, 0xF24AF280,
    0x00010009, 0x0000003C, 0x0003005B, (u32)btlevtcmd_GetEnemyBelong, 0xFFFFFFFE, 0xFE363C80,
    0x0004005B, (u32)btlevtcmd_SamplingEnemy, 0xFFFFFFFE, 0xFE363C80, (u32)partyWeapon_YoshiNomikomi_Fire, 0x0003005B,
    (u32)btlevtcmd_GetSelectEnemy, 0xFE363C8A, 0xFE363C8B, 0x00010003, 0x0000000A, 0x00020018,
    0xFE363C8A, 0xFFFFFFFF, 0x00010004, 0x0000005A, 0x00000021, 0x0007005B,
    (u32)btlevtcmd_CheckDamage, 0xFFFFFFFE, 0xFE363C8A, 0xFE363C8B, (u32)partyWeapon_YoshiNomikomi_Fire, 0x80000100,
    0xFE363C85, 0x0003005B, (u32)btlevtcmd_GetSelectNextEnemy, 0xFE363C8A, 0xFE363C8B, 0x00010004,
    0x0000000A, 0x00010003, 0x0000005A, 0x0001005B, (u32)btlevtcmd_StopAC, 0x0001005B,
    (u32)evt_audience_ap_recovery, 0x0001005B, (u32)btlevtcmd_InviteApInfoReport, 0x00010003, 0x0000005F, 0x0003005B,
    (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000,
    0x00000001, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001, 0x0000002A,
    0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE, 0xF24A8A80, 0x0005005B, (u32)btlevtcmd_GetHomePos,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B, (u32)btlevtcmd_GetUnitWork,
    0xFFFFFFFE, 0x00000001, 0xFE363C85, 0x00020018, 0xFE363C85, 0x00000000,
    0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00000000, 0xFFFFFFFF, 0x00000000, 0x00010004, 0x00000063, 0x00000021,
    0x0007005B, (u32)btlevtcmd_GetMoveFrame, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0xF24A8A80, 0xFE363C85, 0x0002001C, 0xFE363C85, 0x00000014, 0x0008005B,
    (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000,
    0xFFFFFFFF, 0x00000000, 0x00010004, 0x00000063, 0x00000021, 0x0001006F,
    0xFE363C8F, 0x00020036, 0xFE363C85, 0x00000001, 0x00020032, 0xFE363C86,
    0xFE363C85, 0x00020036, 0xFE363C86, 0x0000000A, 0x0006005B, (u32)btlevtcmd_ACRStart,
    0xFFFFFFFE, 0xFE363C86, 0xFE363C85, 0xFE363C85, 0x00000000, 0x00000070,
    0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0xFE363C85, 0xFFFFFFFF, 0x00000000, 0x0002005B, (u32)btlevtcmd_WaitEventEnd, 0xFE363C8F,
    0x0003005B, (u32)btlevtcmd_ACRGetResult, 0xFE363C86, 0xFE363C87, 0x00020018, 0xFE363C86,
    0x00000002, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PYS_O_1_802f7850,
    0x0006005B, (u32)btlevtcmd_AudienceDeclareAcrobatResult, 0xFE363C8C, 0x00000001, 0x00000000, 0x00000000,
    0x00000000, 0x00010009, 0x00000014, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE,
    0x00000001, (u32)(char*)str_PYS_J_1B_802f7aa0, 0x0005005B, (u32)evt_sub_intpl_init, 0x00000004, 0x00000000,
    0x00000168, 0x0000001E, 0x00010005, 0x0000001E, 0x0001005B, (u32)evt_sub_intpl_get_value,
    0x0005005B, (u32)btlevtcmd_SetRotate, 0xFFFFFFFE, 0x00000000, 0xFE363C80, 0x00000000,
    0x00010009, 0x00000001, 0x00000006, 0x00000021, 0x00010003, 0x00000063,
    0x0002005B, (u32)btlevtcmd_ResetFaceDirection, 0xFFFFFFFE, 0x0001005E, (u32)btldefaultevt_SuitoruBadgeEffect, 0x0002005B,
    (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002, 0x00000001,
};

u32 partyYoshiAttack_EggAttack[] = {
    0x0003005B, (u32)btlevtcmd_GetSelectEnemy, 0xFE363C83, 0xFE363C84, 0x00020018, 0xFE363C83,
    0xFFFFFFFF, 0x00010004, 0x00000063, 0x00000021, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress,
    0xFFFFFFFE, 0xFE363C80, 0x0002005B, (u32)btlevtcmd_WeaponAftereffect, 0xFE363C80, 0x0004005B,
    (u32)btlevtcmd_AttackDeclare, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0x0001005B, (u32)btlevtcmd_WaitGuardMove,
    0x0002005B, (u32)btlevtcmd_CommandPayWeaponCost, 0xFFFFFFFE, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFFFFFFFE,
    0x00000007, 0x0003005B, (u32)btlevtcmd_GetEnemyBelong, 0xFFFFFFFE, 0xFE363C80, 0x0004005B,
    (u32)btlevtcmd_SamplingEnemy, 0xFFFFFFFE, 0xFE363C80, (u32)partyWeapon_YoshiEggAttack_Minimini, 0x0004005B, (u32)btlevtcmd_ChoiceSamplingEnemy,
    (u32)partyWeapon_YoshiEggAttack_Minimini, 0xFE363C83, 0xFE363C84, 0x0005005B, (u32)btlevtcmd_SetPartsWork, 0xFFFFFFFE,
    0x00000002, 0x00000003, 0xFE363C83, 0x0005005B, (u32)btlevtcmd_SetPartsWork, 0xFFFFFFFE,
    0x00000002, 0x00000004, 0xFE363C84, 0x0005005B, (u32)btlevtcmd_SetPartsWork, 0xFFFFFFFE,
    0x00000002, 0x00000005, 0x00000000, 0x0005005B, (u32)btlevtcmd_SetPartsWork, 0xFFFFFFFE,
    0x00000002, 0x00000006, 0x00000000, 0x0005005B, (u32)btlevtcmd_SetPartsWork, 0xFFFFFFFE,
    0x00000002, 0x00000007, 0x00000000, 0x0003005B, (u32)btlevtcmd_GetEnemyBelong, 0xFFFFFFFE,
    0xFE363C80, 0x0004005B, (u32)btlevtcmd_SamplingEnemy, 0xFFFFFFFE, 0xFE363C80, (u32)partyWeapon_YoshiEggAttack_Minimini,
    0x0004005B, (u32)btlevtcmd_ChoiceSamplingEnemy, (u32)partyWeapon_YoshiEggAttack_Minimini, 0xFE363C8A, 0xFE363C8B, 0x0005005B,
    (u32)btlevtcmd_SetPartsWork, 0xFFFFFFFE, 0x00000007, 0x00000003, 0xFE363C8A, 0x0005005B,
    (u32)btlevtcmd_SetPartsWork, 0xFFFFFFFE, 0x00000007, 0x00000004, 0xFE363C8B, 0x0005005B,
    (u32)btlevtcmd_SetPartsWork, 0xFFFFFFFE, 0x00000007, 0x00000005, 0x00000000, 0x0005005B,
    (u32)btlevtcmd_SetPartsWork, 0xFFFFFFFE, 0x00000007, 0x00000006, 0x00000000, 0x0005005B,
    (u32)btlevtcmd_SetPartsWork, 0xFFFFFFFE, 0x00000007, 0x00000007, 0x00000000, 0x0004005B,
    (u32)btlevtcmd_SamplingEnemy, 0xFFFFFFFE, 0xFE363C80, (u32)partyWeapon_YoshiEggAttack_Minimini, 0x0004005B, (u32)btlevtcmd_ChoiceSamplingEnemy,
    (u32)partyWeapon_YoshiEggAttack_Minimini, 0xFE363C8A, 0xFE363C8B, 0x0005005B, (u32)btlevtcmd_SetPartsWork, 0xFFFFFFFE,
    0x00000006, 0x00000003, 0xFE363C8A, 0x0005005B, (u32)btlevtcmd_SetPartsWork, 0xFFFFFFFE,
    0x00000006, 0x00000004, 0xFE363C8B, 0x0005005B, (u32)btlevtcmd_SetPartsWork, 0xFFFFFFFE,
    0x00000006, 0x00000005, 0x00000000, 0x0005005B, (u32)btlevtcmd_SetPartsWork, 0xFFFFFFFE,
    0x00000006, 0x00000006, 0x00000000, 0x0005005B, (u32)btlevtcmd_SetPartsWork, 0xFFFFFFFE,
    0x00000006, 0x00000007, 0x00000000, 0x0004005B, (u32)btlevtcmd_SamplingEnemy, 0xFFFFFFFE,
    0xFE363C80, (u32)partyWeapon_YoshiEggAttack_Minimini, 0x0004005B, (u32)btlevtcmd_ChoiceSamplingEnemy, (u32)partyWeapon_YoshiEggAttack_Minimini, 0xFE363C8A,
    0xFE363C8B, 0x0005005B, (u32)btlevtcmd_SetPartsWork, 0xFFFFFFFE, 0x00000005, 0x00000003,
    0xFE363C8A, 0x0005005B, (u32)btlevtcmd_SetPartsWork, 0xFFFFFFFE, 0x00000005, 0x00000004,
    0xFE363C8B, 0x0005005B, (u32)btlevtcmd_SetPartsWork, 0xFFFFFFFE, 0x00000005, 0x00000005,
    0x00000000, 0x0005005B, (u32)btlevtcmd_SetPartsWork, 0xFFFFFFFE, 0x00000005, 0x00000006,
    0x00000000, 0x0005005B, (u32)btlevtcmd_SetPartsWork, 0xFFFFFFFE, 0x00000005, 0x00000007,
    0x00000000, 0x0004005B, (u32)btlevtcmd_SamplingEnemy, 0xFFFFFFFE, 0xFE363C80, (u32)partyWeapon_YoshiEggAttack_Minimini,
    0x0004005B, (u32)btlevtcmd_ChoiceSamplingEnemy, (u32)partyWeapon_YoshiEggAttack_Minimini, 0xFE363C8A, 0xFE363C8B, 0x0005005B,
    (u32)btlevtcmd_SetPartsWork, 0xFFFFFFFE, 0x00000004, 0x00000003, 0xFE363C8A, 0x0005005B,
    (u32)btlevtcmd_SetPartsWork, 0xFFFFFFFE, 0x00000004, 0x00000004, 0xFE363C8B, 0x0005005B,
    (u32)btlevtcmd_SetPartsWork, 0xFFFFFFFE, 0x00000004, 0x00000005, 0x00000000, 0x0005005B,
    (u32)btlevtcmd_SetPartsWork, 0xFFFFFFFE, 0x00000004, 0x00000006, 0x00000000, 0x0005005B,
    (u32)btlevtcmd_SetPartsWork, 0xFFFFFFFE, 0x00000004, 0x00000007, 0x00000000, 0x0004005B,
    (u32)btlevtcmd_SamplingEnemy, 0xFFFFFFFE, 0xFE363C80, (u32)partyWeapon_YoshiEggAttack_Minimini, 0x0004005B, (u32)btlevtcmd_ChoiceSamplingEnemy,
    (u32)partyWeapon_YoshiEggAttack_Minimini, 0xFE363C8A, 0xFE363C8B, 0x0005005B, (u32)btlevtcmd_SetPartsWork, 0xFFFFFFFE,
    0x00000003, 0x00000003, 0xFE363C8A, 0x0005005B, (u32)btlevtcmd_SetPartsWork, 0xFFFFFFFE,
    0x00000003, 0x00000004, 0xFE363C8B, 0x0005005B, (u32)btlevtcmd_SetPartsWork, 0xFFFFFFFE,
    0x00000003, 0x00000005, 0x00000000, 0x0005005B, (u32)btlevtcmd_SetPartsWork, 0xFFFFFFFE,
    0x00000003, 0x00000006, 0x00000000, 0x0005005B, (u32)btlevtcmd_SetPartsWork, 0xFFFFFFFE,
    0x00000003, 0x00000007, 0x00000000, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000,
    0x0000000A, 0x0006005B, (u32)evt_btl_camera_set_homing_unitparts, 0x00000000, 0xFFFFFFFE, 0x00000001,
    0xFFFFFFFF, 0x00000000, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000002,
    0x0003005B, (u32)evt_btl_camera_set_zoom, 0x00000000, 0x0000012C, 0x0002005B, (u32)btlevtcmd_CommandGetWeaponActionLv,
    0xFE363C80, 0x0003005B, (u32)btlevtcmd_AcSetDifficulty, 0xFFFFFFFE, 0xFE363C80, 0x0003005B,
    (u32)btlevtcmd_GetPartyTechLv, 0xFFFFFFFE, 0xFE363C80, 0x00010022, 0xFE363C80, 0x00010024,
    0x00000002, 0x00020032, 0xFE363C81, 0x00000004, 0x0000002A, 0x00020032,
    0xFE363C81, 0x00000003, 0x00000031, 0x0002005B, (u32)btlevtcmd_AcGetDifficulty, 0xFE363C80,
    0x00020036, 0xFE363C80, 0x00000003, 0x00010022, 0xFE363C80, 0x00010028,
    0xFFFFFFFD, 0x00020032, 0xFE363C80, 0x000001E0, 0x00010024, 0xFFFFFFFE,
    0x00020032, 0xFE363C80, 0x00000168, 0x00010024, 0xFFFFFFFF, 0x00020032,
    0xFE363C80, 0x000000F0, 0x00010024, 0x00000000, 0x00020032, 0xFE363C80,
    0x000000B4, 0x00010024, 0x00000001, 0x00020032, 0xFE363C80, 0x00000096,
    0x00010024, 0x00000002, 0x00020032, 0xFE363C80, 0x00000064, 0x0000002A,
    0x00020032, 0xFE363C80, 0x0000003C, 0x00000031, 0x0009005B, (u32)btlevtcmd_AcSetParamAll,
    0x00000000, 0xFE363C80, 0xFE363C81, 0x00000000, 0xF1194D80, 0xF1194D80,
    0xF1194D80, 0xF1194D80, 0x0002005B, (u32)btlevtcmd_AcSetFlag, 0x00000000, 0x0003005B,
    (u32)btlevtcmd_AcSetOutputParam, 0x00000001, 0x00000000, 0x0005005B, (u32)btlevtcmd_SetupAC, 0xFFFFFFFE,
    0x00000014, 0x00000001, 0x0000003C, 0x00010009, 0x00000016, 0x0007005B,
    (u32)btlevtcmd_CalculateFaceDirection, 0xFFFFFFFE, 0xFFFFFFFF, 0xFE363C83, 0xFE363C84, 0x00000110,
    0xFE363C8F, 0x0003005B, (u32)btlevtcmd_ChangeFaceDirection, 0xFFFFFFFE, 0xFE363C8F, 0x0002005B,
    (u32)btlevtcmd_StartAC, 0x00000001, 0x00020032, 0xFE363C8C, 0x00000002, 0x00010003,
    0x00000005, 0x0003005B, (u32)btlevtcmd_AcGetOutputParam, 0x00000000, 0xFE363C8D, 0x00020035,
    0xFE363C8D, 0x00000001, 0x0002001B, 0xFE363C8C, 0xFE363C8D, 0x0003005B,
    (u32)btlevtcmd_AcGetOutputParam, 0x00000001, 0xFE363C80, 0x00020019, 0xFE363C80, 0x00000000,
    0x0001005B, (u32)btlevtcmd_StopAC, 0x0001000A, 0x000001F4, 0x0003005B, (u32)btlevtcmd_AcGetOutputParam,
    0x00000000, 0xFE363C8D, 0x0002001D, 0xFE363C8D, 0x00000001, 0x0003005B,
    (u32)btlevtcmd_AcGetOutputParam, 0x00000000, 0xFE363C8D, 0x00020035, 0xFE363C8D, 0x00000001,
    0x00010004, 0x00000019, 0x00000020, 0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult, (u32)partyWeapon_YoshiEggAttack_Minimini,
    0xFFFFFFFF, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PYS_A_5_802f7acc,
    0x00010009, 0x00000036, 0x0001005B, (u32)evt_audience_ap_recovery, 0x0001005B, (u32)btlevtcmd_InviteApInfoReport,
    0x00010004, 0x0000005F, 0x00000021, 0x00000021, 0x00010009, 0x00000001,
    0x00010004, 0x00000005, 0x00000021, 0x00020032, 0xFE363C80, 0xFE363C8C,
    0x00020036, 0xFE363C80, 0x00000001, 0x00010022, 0xFE363C80, 0x00010028,
    0x00000002, 0x00020032, 0xFE363C80, 0x00000000, 0x00010028, 0x00000003,
    0x00020032, 0xFE363C80, 0x00000001, 0x0000002A, 0x00020032, 0xFE363C80,
    0x00000002, 0x00000031, 0x0004005B, (u32)btlevtcmd_GetResultPrizeLv, 0xFFFFFFFB, 0xFE363C80,
    0xFE363C86, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00020035, 0xFE363C81, 0x00000028, 0x0005005B, (u32)btlevtcmd_ACSuccessEffect,
    0xFE363C86, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult,
    (u32)partyWeapon_YoshiEggAttack_Minimini, 0xFE363C86, 0x00010003, 0x0000000A, 0x0003005B, (u32)evt_sub_random,
    0x00000002, 0xFE363C80, 0x0005005B, (u32)btlevtcmd_SetPartsWork, 0xFFFFFFFE, 0xFE363C8C,
    0x00000000, 0xFE363C80, 0x00010022, 0xFE363C80, 0x00010024, 0x00000001,
    0x0004005B, (u32)btlevtcmd_AnimeSetPoseTable, 0xFFFFFFFE, 0xFE363C8C, (u32)pose_table_egg_y, 0x00010024,
    0x00000002, 0x0004005B, (u32)btlevtcmd_AnimeSetPoseTable, 0xFFFFFFFE, 0xFE363C8C, (u32)pose_table_egg_p,
    0x0000002A, 0x0004005B, (u32)btlevtcmd_AnimeSetPoseTable, 0xFFFFFFFE, 0xFE363C8C, (u32)pose_table_egg_g,
    0x00000031, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0xFE363C8C, 0x00000045,
    0x0005005B, (u32)btlevtcmd_SetPartsWork, 0xFFFFFFFE, 0xFE363C8C, 0x00000001, 0x00000004,
    0x0005005B, (u32)btlevtcmd_SetPartsWork, 0xFFFFFFFE, 0xFE363C8C, 0x00000002, (u32)partyWeapon_YoshiEggAttack_Minimini,
    0x00010003, 0x00000014, 0x0003005B, (u32)btlevtcmd_GetStatusMg, 0xFFFFFFFE, 0xFE363C86,
    0x0006005B, (u32)btlevtcmd_SetPartsScale, 0xFFFFFFFE, 0xFE363C8C, 0xF24A7E80, 0xF24A7E80,
    0xF24A7E80, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PYS_A_8_802f7bb0,
    0x00010009, 0x0000000A, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001,
    (u32)(char*)str_PYS_A_12_802f7bb8, 0x0003005B, (u32)btlevtcmd_GetStatusMg, 0xFFFFFFFE, 0xFE363C85, 0x0002003C,
    0xFE363C85, 0x0000000A, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C85, 0x00020035, 0xFE363C81, 0xFE363C85, 0x0006005B, (u32)btlevtcmd_SetPartsPos,
    0xFFFFFFFE, 0xFE363C8C, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0006005B,
    (u32)btlevtcmd_SetPartsDispOffset, 0xFFFFFFFE, 0xFE363C8C, 0x00000000, 0x00000000, 0x00000000,
    0x0006005B, (u32)btlevtcmd_SetPartsRotate, 0xFFFFFFFE, 0xFE363C8C, 0x00000000, 0x00000000,
    0x00000000, 0x0006005B, (u32)btlevtcmd_SetPartsRotateOffset, 0xFFFFFFFE, 0xFE363C8C, 0x00000000,
    0x00000000, 0x00000000, 0x0004005B, (u32)btlevtcmd_OffPartsAttribute, 0xFFFFFFFE, 0xFE363C8C,
    0x03000000, 0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_BTL_YOSHI_EGG1_802f7bc4, 0x00000000, 0x00020032,
    0xFE363C8F, 0x00000000, 0x0000006E, 0x0006005B, (u32)btlevtcmd_SetPartsRotateOffset, 0xFFFFFFFE,
    0xFE363C8C, 0x00000000, 0x00000006, 0x00000000, 0x00010005, 0x0000001E,
    0x0006005B, (u32)btlevtcmd_AddPartsRotate, 0xFFFFFFFE, 0xFE363C8C, 0x00000000, 0x00000000,
    0x00000030, 0x00010009, 0x00000001, 0x00000006, 0x0006005B, (u32)btlevtcmd_SetPartsRotate,
    0xFFFFFFFE, 0xFE363C8C, 0x00000000, 0x00000000, 0x00000000, 0x00000070,
    0x0000006E, 0x00020032, 0xFE363C85, 0xFE363C8C, 0x00020036, 0xFE363C85,
    0x00000002, 0x0002003C, 0xFE363C85, 0x0000000A, 0x0002003A, 0xFE363C85,
    0x0000000F, 0x0003005B, (u32)btlevtcmd_GetStatusMg, 0xFFFFFFFE, 0xFE363C80, 0x0002003C,
    0xFE363C85, 0xFE363C80, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C85, 0x0004005B, (u32)btlevtcmd_TransStageFloorPosition, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0004005B, (u32)btlevtcmd_SetPartsFallAccel, 0xFFFFFFFE, 0xFE363C8C, 0xF24A7B4C, 0x0008005B,
    (u32)btlevtcmd_JumpPartsPosition, 0xFFFFFFFE, 0xFE363C8C, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0000001E, 0xFFFFFFFF, 0x00000070, 0x00010009, 0x0000000A, 0x00020035,
    0xFE363C8C, 0x00000001, 0x00010004, 0x00000005, 0x00010003, 0x00000019,
    0x0005005B, (u32)btlevtcmd_SetPartsWork, 0xFFFFFFFE, 0x00000002, 0x00000006, 0xFFFFFFFF,
    0x0005005B, (u32)btlevtcmd_SetPartsWork, 0xFFFFFFFE, 0x00000003, 0x00000006, 0xFFFFFFFF,
    0x0005005B, (u32)btlevtcmd_SetPartsWork, 0xFFFFFFFE, 0x00000004, 0x00000006, 0xFFFFFFFF,
    0x0005005B, (u32)btlevtcmd_SetPartsWork, 0xFFFFFFFE, 0x00000005, 0x00000006, 0xFFFFFFFF,
    0x0005005B, (u32)btlevtcmd_SetPartsWork, 0xFFFFFFFE, 0x00000006, 0x00000006, 0xFFFFFFFF,
    0x0005005B, (u32)btlevtcmd_SetPartsWork, 0xFFFFFFFE, 0x00000007, 0x00000006, 0xFFFFFFFF,
    0x00020032, 0xFE363C8C, 0xFE363C8D, 0x00010003, 0x0000001A, 0x0005005B,
    (u32)btlevtcmd_GetPartsWork, 0xFFFFFFFE, 0xFE363C8C, 0x00000006, 0xFE363C80, 0x00020018,
    0xFE363C80, 0xFFFFFFFF, 0x0005005B, (u32)btlevtcmd_SetPartsWork, 0xFFFFFFFE, 0xFE363C8C,
    0x00000006, 0x00000001, 0x00020032, 0xFE363C8E, 0xFE363C8C, 0x00010003,
    0x0000001B, 0x00020036, 0xFE363C8E, 0x00000001, 0x0002001D, 0xFE363C8E,
    0x00000002, 0x0005005B, (u32)btlevtcmd_GetPartsWork, 0xFFFFFFFE, 0xFE363C8C, 0x00000003,
    0xFE363C80, 0x0005005B, (u32)btlevtcmd_GetPartsWork, 0xFFFFFFFE, 0xFE363C8E, 0x00000003,
    0xFE363C81, 0x00020018, 0xFE363C80, 0xFE363C81, 0x0005005B, (u32)btlevtcmd_SetPartsWork,
    0xFFFFFFFE, 0xFE363C8E, 0x00000006, 0x00000000, 0x00000021, 0x00010004,
    0x0000001B, 0x00000021, 0x00000021, 0x00020036, 0xFE363C8C, 0x00000001,
    0x0002001D, 0xFE363C8C, 0x00000002, 0x00010004, 0x0000001A, 0x00000021,
    0x00020032, 0xFE363C8C, 0x00000002, 0x00010003, 0x0000001C, 0x0005005B,
    (u32)btlevtcmd_GetPartsWork, 0xFFFFFFFE, 0xFE363C8C, 0x00000006, 0xFE363C80, 0x00010022,
    0xFE363C80, 0x00010024, 0x00000000, 0x0005005B, (u32)btlevtcmd_GetPartsWork, 0xFFFFFFFE,
    0xFE363C8C, 0x00000003, 0xFE363C80, 0x0005005B, (u32)btlevtcmd_GetPartsWork, 0xFFFFFFFE,
    0xFE363C8C, 0x00000004, 0xFE363C81, 0x0006005B, (u32)btlevtcmd_CommandPreCheckDamage, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0x00000100, 0xFE363C86, 0x0005005B, (u32)btlevtcmd_SetPartsWork,
    0xFFFFFFFE, 0xFE363C8C, 0x00000005, 0xFE363C86, 0x00020018, 0xFE363C86,
    0x00000001, 0x00020032, 0xFE363C8E, 0xFE363C8C, 0x00010003, 0x0000001D,
    0x00020035, 0xFE363C8E, 0x00000001, 0x0002001C, 0xFE363C8E, 0xFE363C8D,
    0x0005005B, (u32)btlevtcmd_GetPartsWork, 0xFFFFFFFE, 0xFE363C8C, 0x00000003, 0xFE363C80,
    0x0005005B, (u32)btlevtcmd_GetPartsWork, 0xFFFFFFFE, 0xFE363C8E, 0x00000003, 0xFE363C81,
    0x00020018, 0xFE363C80, 0xFE363C81, 0x0005005B, (u32)btlevtcmd_GetPartsWork, 0xFFFFFFFE,
    0xFE363C8E, 0x00000006, 0xFE363C80, 0x00020019, 0xFE363C80, 0x00000000,
    0x0005005B, (u32)btlevtcmd_SetPartsWork, 0xFFFFFFFE, 0xFE363C8E, 0x00000006, 0x00000002,
    0x00000021, 0x00000021, 0x00010004, 0x0000001D, 0x00000021, 0x00000021,
    0x00010024, 0x00000001, 0x0005005B, (u32)btlevtcmd_GetPartsWork, 0xFFFFFFFE, 0xFE363C8C,
    0x00000003, 0xFE363C80, 0x0005005B, (u32)btlevtcmd_GetPartsWork, 0xFFFFFFFE, 0xFE363C8C,
    0x00000004, 0xFE363C81, 0x0006005B, (u32)btlevtcmd_CommandPreCheckDamage, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0x00000100, 0xFE363C86, 0x0005005B, (u32)btlevtcmd_SetPartsWork, 0xFFFFFFFE,
    0xFE363C8C, 0x00000005, 0xFE363C86, 0x0000002A, 0x0005005B, (u32)btlevtcmd_GetPartsWork,
    0xFFFFFFFE, 0xFE363C8C, 0x00000003, 0xFE363C80, 0x0005005B, (u32)btlevtcmd_GetPartsWork,
    0xFFFFFFFE, 0xFE363C8C, 0x00000004, 0xFE363C81, 0x0006005B, (u32)btlevtcmd_CommandPreCheckDamage,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0x00100100, 0xFE363C86, 0x0005005B,
    (u32)btlevtcmd_SetPartsWork, 0xFFFFFFFE, 0xFE363C8C, 0x00000005, 0xFE363C86, 0x00000031,
    0x0002001A, 0xFE363C8C, 0xFE363C8D, 0x00020035, 0xFE363C8C, 0x00000001,
    0x00010004, 0x0000001C, 0x00000021, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000,
    0x00000000, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000002, 0x00020032,
    0xFE363C8C, 0x00000002, 0x00010003, 0x0000001E, 0x0007005B, (u32)btlevtcmd_CalculateFaceDirection,
    0xFFFFFFFE, 0xFFFFFFFF, 0xFE363C83, 0xFE363C84, 0x00000010, 0xFE363C8F,
    0x0003005B, (u32)btlevtcmd_ChangeFaceDirection, 0xFFFFFFFE, 0xFE363C8F, 0x0004005B, (u32)btlevtcmd_OnPartsAttribute,
    0xFFFFFFFE, 0xFE363C8C, 0x03000000, 0x0005005B, (u32)btlevtcmd_GetPartsWork, 0xFFFFFFFE,
    0xFE363C8C, 0x00000000, 0xFE363C80, 0x00010022, 0xFE363C80, 0x00010024,
    0x00000001, 0x00020032, 0xFE363C86, (u32)(char*)str_PYS_A3_1_802f7bd8, 0x00020032, 0xFE363C87,
    (u32)(char*)str_PYS_A3_2_802f7be4, 0x00020032, 0xFE363C88, (u32)(char*)str_PYS_A3_3_802f7bf0, 0x00010024, 0x00000002,
    0x00020032, 0xFE363C86, (u32)(char*)str_PYS_A4_1_802f7bfc, 0x00020032, 0xFE363C87, (u32)(char*)str_PYS_A4_2_802f7c08,
    0x00020032, 0xFE363C88, (u32)(char*)str_PYS_A4_3_802f7c14, 0x0000002A, 0x00020032, 0xFE363C86,
    (u32)(char*)str_PYS_A2_1_802f7c20, 0x00020032, 0xFE363C87, (u32)(char*)str_PYS_A2_2_802f7c2c, 0x00020032, 0xFE363C88,
    (u32)(char*)str_PYS_A2_3_802f7c38, 0x00000031, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001,
    0xFE363C86, 0x0003005B, (u32)btlevtcmd_AnimeWaitPlayComplete, 0xFFFFFFFE, 0x00000001, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, 0xFE363C88, 0x00010009, 0x00000007,
    0x0001006F, 0xFE363C8F, 0x0001005E, (u32)_egg_attack_event, 0x00000070, 0x0005005B,
    (u32)btlevtcmd_SetPartsWork, 0xFFFFFFFE, 0xFE363C8C, 0x00000007, 0xFE363C8F, 0x0003005B,
    (u32)btlevtcmd_AnimeWaitPlayComplete, 0xFFFFFFFE, 0x00000001, 0x00010009, 0x0000000A, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001, 0x00000045, 0x00010003, 0x0000005A,
    0x00020019, 0xFE363C8C, 0xFE363C8D, 0x00020035, 0xFE363C8C, 0x00000001,
    0x0005005B, (u32)btlevtcmd_GetPartsWork, 0xFFFFFFFE, 0xFE363C8C, 0x00000003, 0xFE363C83,
    0x0005005B, (u32)btlevtcmd_GetPartsWork, 0xFFFFFFFE, 0xFE363C8C, 0x00000004, 0xFE363C84,
    0x00010004, 0x0000001E, 0x00000021, 0x0001005B, (u32)btlevtcmd_StopAC, 0x0006005B,
    (u32)btlevtcmd_ACRStart, 0xFFFFFFFE, 0x00000000, 0x0000000F, 0x0000000F, 0x00000014,
    0x0003005B, (u32)btlevtcmd_ACRGetResult, 0xFE363C86, 0xFE363C87, 0x00010022, 0xFE363C86,
    0x00010029, 0x00000002, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress, 0xFFFFFFFE, 0xFE363C80,
    0x0006005B, (u32)btlevtcmd_AudienceDeclareAcrobatResult, 0xFE363C80, 0x00000001, 0x00000000, 0x00000000,
    0x00000000, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PYS_Y_1_802f7ad4,
    0x0003005B, (u32)btlevtcmd_AnimeWaitPlayComplete, 0xFFFFFFFE, 0x00000001, 0x0004005B, (u32)btlevtcmd_AnimeChangePose,
    0xFFFFFFFE, 0x00000001, (u32)(char*)str_PYS_Y_1_802f7ad4, 0x0003005B, (u32)btlevtcmd_AnimeWaitPlayComplete, 0xFFFFFFFE,
    0x00000001, 0x0000002A, 0x0001005B, (u32)evt_audience_acrobat_notry, 0x00000031, 0x0001005B,
    (u32)evt_audience_ap_recovery, 0x0001005B, (u32)btlevtcmd_InviteApInfoReport, 0x00010003, 0x0000005F, 0x00020032,
    0xFE363C8C, 0x00000002, 0x00010003, 0x00000060, 0x0005005B, (u32)btlevtcmd_GetPartsWork,
    0xFFFFFFFE, 0xFE363C8C, 0x00000007, 0xFE363C8F, 0x00020019, 0xFE363C8F,
    0x00000000, 0x0002005B, (u32)btlevtcmd_WaitEventEnd, 0xFE363C8F, 0x0005005B, (u32)btlevtcmd_SetPartsWork,
    0xFFFFFFFE, 0xFE363C8C, 0x00000007, 0x00000000, 0x00000021, 0x00020035,
    0xFE363C8C, 0x00000001, 0x0002001C, 0xFE363C8C, 0xFE363C8D, 0x00010004,
    0x00000060, 0x00000021, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000,
    0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType,
    0xFFFFFFFE, 0x00000001, 0x0000002A, 0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE,
    0xF24A9280, 0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000000, 0xFFFFFFFF, 0x00000000, 0x00010003, 0x00000063,
    0x0002005B, (u32)btlevtcmd_ResetFaceDirection, 0xFFFFFFFE, 0x0001005E, (u32)btldefaultevt_SuitoruBadgeEffect, 0x0001000A,
    0x000007D0, 0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002, 0x00000001,
};

u32 _egg_attack_event[] = {
    0x0003005B, (u32)btlevtcmd_GetStatusMg, 0xFFFFFFFE, 0xFE363C86, 0x0002003C, 0xFE363C86,
    0x0000000A, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00020035, 0xFE363C81, 0xFE363C86, 0x0006005B, (u32)btlevtcmd_SetPartsPos,
    0xFFFFFFFE, 0xFE363C8C, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B,
    (u32)btlevtcmd_OffPartsAttribute, 0xFFFFFFFE, 0xFE363C8C, 0x03000000, 0x0004005B, (u32)btlevtcmd_SetPartsMoveSpeed,
    0xFFFFFFFE, 0xFE363C8C, 0xF24A8E80, 0x0004005B, (u32)btlevtcmd_SetPartsFallAccel, 0xFFFFFFFE,
    0xFE363C8C, 0xF24A7AE6, 0x0006005B, (u32)btlevtcmd_GetHitPos, 0xFE363C83, 0xFE363C84,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0000006E, 0x0007005B, (u32)btlevtcmd_GetMoveFrame,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0xF24A8E80, 0xFE363C85,
    0x0003005B, (u32)btlevtcmd_GetStatusMg, 0xFFFFFFFE, 0xFE363C86, 0x0002003C, 0xFE363C86,
    0x00000006, 0x0006005B, (u32)btlevtcmd_SetPartsRotateOffset, 0xFFFFFFFE, 0xFE363C8C, 0x00000000,
    0xFE363C86, 0x00000000, 0x00010005, 0xFE363C85, 0x0006005B, (u32)btlevtcmd_AddPartsRotate,
    0xFFFFFFFE, 0xFE363C8C, 0x00000000, 0x00000000, 0x00000030, 0x00010009,
    0x00000001, 0x00000006, 0x00000070, 0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_BTL_YOSHI_EGG3_802f7c44,
    0x00000000, 0x000B005B, (u32)btlevtcmd_DivePartsPosition, 0xFFFFFFFE, 0xFE363C8C, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000000, 0x00000019, 0x00000000, 0x00000000,
    0xFFFFFFFF, 0x0004005B, (u32)btlevtcmd_OnPartsAttribute, 0xFFFFFFFE, 0xFE363C8C, 0x03000000,
    0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_BTL_YOSHI_EGG2_802f7c58, 0x00000000, 0x000F005B, (u32)evt_eff,
    (u32)(u8*)zero_80424094, (u32)(char*)str_bomb_80424098, 0x00000000, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0xF24A7CE6, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x0005005B, (u32)btlevtcmd_GetPartsWork, 0xFFFFFFFE, 0xFE363C8C,
    0x00000005, 0xFE363C86, 0x00020019, 0xFE363C86, 0x00000001, 0x00020018,
    0xFE363C86, 0x00000003, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000026,
    0x00000021, 0x00020018, 0xFE363C86, 0x00000006, 0x0003005B, (u32)btlevtcmd_StartAvoid,
    0xFE363C83, 0x00000027, 0x00000021, 0x00020018, 0xFE363C86, 0x00000002,
    0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000028, 0x00000021, 0x00010004,
    0x00000059, 0x00000021, 0x0005005B, (u32)btlevtcmd_GetPartsWork, 0xFFFFFFFE, 0xFE363C8C,
    0x00000002, 0xFE363C80, 0x0005005B, (u32)btlevtcmd_GetPartsWork, 0xFFFFFFFE, 0xFE363C8C,
    0x00000006, 0xFE363C81, 0x00020018, 0xFE363C81, 0x00000000, 0x0007005B,
    (u32)btlevtcmd_CheckDamage, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0xFE363C80, 0x40020000,
    0xFE363C85, 0x00000020, 0x0007005B, (u32)btlevtcmd_CheckDamage, 0xFFFFFFFE, 0xFE363C83,
    0xFE363C84, 0xFE363C80, 0x40020100, 0xFE363C85, 0x00000021, 0x00010003,
    0x00000059, 0x00000002, 0x00000001,
};

u32 partyYoshiAttack_CallGuard[] = {
    0x00020032, 0xFE363C8C, (u32)partyWeapon_YoshiCallGuard, 0x0003005B, (u32)btlevtcmd_GetEnemyBelong, 0xFFFFFFFE,
    0xFE363C80, 0x0004005B, (u32)btlevtcmd_SamplingEnemy, 0xFFFFFFFE, 0xFE363C80, 0xFE363C8C,
    0x0003005B, (u32)btlevtcmd_GetSelectEnemy, 0xFE363C83, 0xFE363C84, 0x00020018, 0xFE363C83,
    0xFFFFFFFF, 0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002, 0x00000021,
    0x0002005B, (u32)btlevtcmd_WeaponAftereffect, 0xFE363C8C, 0x0002005B, (u32)btlevtcmd_AttackDeclareAll, 0xFFFFFFFE,
    0x0001005B, (u32)btlevtcmd_WaitGuardMove, 0x0003005B, (u32)btlevtcmd_PayWeaponCost, 0xFFFFFFFE, 0xFE363C8C,
    0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFFFFFFFE, 0x00000007, 0x0002005B, (u32)btlevtcmd_CommandGetWeaponActionLv,
    0xFE363C80, 0x0003005B, (u32)btlevtcmd_AcSetDifficulty, 0xFFFFFFFE, 0xFE363C80, 0x0002005B,
    (u32)btlevtcmd_AcGetDifficulty, 0xFE363C80, 0x00020036, 0xFE363C80, 0x00000003, 0x00010022,
    0xFE363C80, 0x00010028, 0xFFFFFFFD, 0x00020032, 0xFE363C80, 0x00000118,
    0x00020032, 0xFE363C81, 0x00000012, 0x00020032, 0xFE363C82, 0x00000019,
    0x00010024, 0xFFFFFFFE, 0x00020032, 0xFE363C80, 0x000000FA, 0x00020032,
    0xFE363C81, 0x00000011, 0x00020032, 0xFE363C82, 0x0000001B, 0x00010024,
    0xFFFFFFFF, 0x00020032, 0xFE363C80, 0x000000F0, 0x00020032, 0xFE363C81,
    0x00000010, 0x00020032, 0xFE363C82, 0x0000001D, 0x00010024, 0x00000000,
    0x00020032, 0xFE363C80, 0x000000F0, 0x00020032, 0xFE363C81, 0x0000000F,
    0x00020032, 0xFE363C82, 0x0000001E, 0x00010024, 0x00000001, 0x00020032,
    0xFE363C80, 0x000000C8, 0x00020032, 0xFE363C81, 0x0000000F, 0x00020032,
    0xFE363C82, 0x0000001E, 0x00010024, 0x00000002, 0x00020032, 0xFE363C80,
    0x000000B4, 0x00020032, 0xFE363C81, 0x0000000F, 0x00020032, 0xFE363C82,
    0x0000001E, 0x0000002A, 0x00020032, 0xFE363C80, 0x00000096, 0x00020032,
    0xFE363C81, 0x0000000E, 0x00020032, 0xFE363C82, 0x0000001E, 0x00000031,
    0x0009005B, (u32)btlevtcmd_AcSetParamAll, 0x0000000F, 0xFE363C80, 0x000000B2, 0xFE363C81,
    0xFE363C82, 0x00000014, 0x0000003D, 0xF1194D80, 0x0005005B, (u32)btlevtcmd_AcSetGaugeParam,
    0x0000003C, 0x00000050, 0x00000064, 0x00000064, 0x0002005B, (u32)btlevtcmd_AcSetFlag,
    0x00000040, 0x0005005B, (u32)btlevtcmd_SetupAC, 0xFFFFFFFE, 0x00000006, 0x00000001,
    0x00000000, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000007, 0x0004005B,
    (u32)evt_btl_camera_set_homing_unit, 0x00000000, 0xFFFFFFFE, 0x00000001, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv,
    0x00000000, 0x00000004, 0x0003005B, (u32)evt_btl_camera_set_zoom, 0x00000000, 0x00000190,
    0x00010009, 0x00000014, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001,
    (u32)(char*)str_PYS_A_10_802f7c6c, 0x0003005B, (u32)btlevtcmd_ftof, 0x00000018, 0xFE363C80, 0x00010009,
    0xFE363C80, 0x0002005B, (u32)btlevtcmd_StartAC, 0x00000001, 0x0001005B, (u32)btlevtcmd_ResultAC,
    0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PYS_A_11_802f7c78, 0x0003005B,
    (u32)btlevtcmd_GetHeight, 0xFFFFFFFE, 0xFE363C86, 0x0003005B, (u32)btlevtcmd_GetStatusMg, 0xFFFFFFFE,
    0xFE363C80, 0x0002003C, 0xFE363C86, 0xFE363C80, 0x0005005B, (u32)btlevtcmd_GetPos,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00020035, 0xFE363C81,
    0xFE363C86, 0x0003005B, (u32)btlevtcmd_GetFaceDirection, 0xFFFFFFFE, 0xFE363C85, 0x00020037,
    0xFE363C85, 0x0000001E, 0x000F005B, (u32)evt_eff, 0x00000000, (u32)(char*)str_kiss_804240a0,
    0x00000003, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0xFE363C85, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_ENM_TOGENOKO_CAL_802f7c84, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00000000, 0x0003005B, (u32)btlevtcmd_ftof, 0x00000021, 0xFE363C80, 0x0001006F,
    0xFE363C8F, 0x00010009, 0xFE363C80, 0x00000070, 0x0006005B, (u32)btlevtcmd_ACRStart,
    0xFFFFFFFE, 0x00000000, 0xFE363C80, 0xFE363C80, 0x00000014, 0x0003005B,
    (u32)btlevtcmd_ACRGetResult, 0xFE363C86, 0xFE363C87, 0x00010022, 0xFE363C86, 0x00010029,
    0x00000002, 0x0006005B, (u32)btlevtcmd_AudienceDeclareAcrobatResult, 0xFE363C8C, 0x00000001, 0x00000000,
    0x00000000, 0x00000000, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001,
    (u32)(char*)str_PYS_Y_1_802f7ad4, 0x0003005B, (u32)btlevtcmd_AnimeWaitPlayComplete, 0xFFFFFFFE, 0x00000001, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PYS_Y_1_802f7ad4, 0x0003005B, (u32)btlevtcmd_AnimeWaitPlayComplete,
    0xFFFFFFFE, 0x00000001, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001,
    0x0000002B, 0x0000002A, 0x0001005B, (u32)evt_audience_acrobat_notry, 0x0002005B, (u32)btlevtcmd_WaitEventEnd,
    0xFE363C8F, 0x0000006E, 0x0001000A, 0x000000A6, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType,
    0xFFFFFFFE, 0x00000001, 0x0000002B, 0x00000070, 0x00000031, 0x0001006C,
    0xFE363C89, 0x0001000A, 0x000003E8, 0x0002005B, (u32)_gundan_yoshi_run_effect, 0xFFFFFFFE,
    0x0000006D, 0x0001000A, 0x000000A6, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000,
    0x00000000, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001, 0x0006005B,
    (u32)evt_btl_camera_shake_h, 0x00000000, 0x00000001, 0x00000000, 0x00001770, 0x00000000,
    0x0001000A, 0x000003E8, 0x0002005B, (u32)_wait_yoshig_run, 0xFFFFFFFE, 0x00020032,
    0xFE363C8F, 0x00000000, 0x00010003, 0x00000046, 0x00020018, 0xFE363C83,
    0xFFFFFFFF, 0x00020019, 0xFE363C8F, 0x00000000, 0x0002005B, (u32)btlevtcmd_WaitEventEnd,
    0xFE363C8F, 0x00000021, 0x0002005B, (u32)btlevtcmd_WaitEventEnd, 0xFE363C89, 0x00010004,
    0x0000005A, 0x00000021, 0x0007005B, (u32)btlevtcmd_PreCheckDamage, 0xFFFFFFFE, 0xFE363C83,
    0xFE363C84, 0xFE363C8C, 0x00000100, 0xFE363C86, 0x00020019, 0xFE363C86,
    0x00000001, 0x00020018, 0xFE363C86, 0x00000003, 0x0003005B, (u32)btlevtcmd_StartAvoid,
    0xFE363C83, 0x00000026, 0x00000021, 0x00020018, 0xFE363C86, 0x00000006,
    0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000027, 0x00000021, 0x00020018,
    0xFE363C86, 0x00000002, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000028,
    0x00000021, 0x00010004, 0x0000004B, 0x00000021, 0x0003005B, (u32)btlevtcmd_AcGetOutputParam,
    0x00000002, 0xFE363C8E, 0x0001006F, 0xFE363C8F, 0x00010005, 0xFE363C8E,
    0x0002001D, 0xFE363C8E, 0x00000002, 0x0003005B, (u32)btlevtcmd_AcGetOutputParam, 0x00000002,
    0xFE363C80, 0x0002001D, 0xFE363C80, 0x00000002, 0x0007005B, (u32)btlevtcmd_CheckDamage,
    0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0xFE363C8C, 0x00020000, 0xFE363C85,
    0x00000020, 0x0007005B, (u32)btlevtcmd_CheckDamage, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84,
    0xFE363C8C, 0x00000000, 0xFE363C85, 0x00000021, 0x00000020, 0x0003005B,
    (u32)btlevtcmd_AcGetOutputParam, 0x00000002, 0xFE363C80, 0x00010022, 0xFE363C80, 0x00010029,
    0x00000006, 0x00020032, 0xFE363C80, 0x00000002, 0x00010029, 0x00000004,
    0x00020032, 0xFE363C80, 0x00000001, 0x00010029, 0x00000002, 0x00020032,
    0xFE363C80, 0x00000000, 0x0000002A, 0x00020032, 0xFE363C80, 0xFFFFFFFF,
    0x00000031, 0x0002001D, 0xFE363C80, 0x00000000, 0x0007005B, (u32)btlevtcmd_CheckDamage,
    0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0xFE363C8C, 0x00020100, 0xFE363C85,
    0x0004005B, (u32)btlevtcmd_GetResultPrizeLv, 0xFE363C83, 0xFE363C80, 0xFE363C86, 0x0006005B,
    (u32)btlevtcmd_GetHitPos, 0xFE363C83, 0xFE363C84, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0005005B, (u32)btlevtcmd_ACSuccessEffect, 0xFE363C86, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult, 0xFE363C8C, 0xFE363C86, 0x00000020, 0x0007005B,
    (u32)btlevtcmd_CheckDamage, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0xFE363C8C, 0x00000100,
    0xFE363C85, 0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult, 0xFE363C8C, 0xFFFFFFFF, 0x00000021,
    0x00000021, 0x00010009, 0x0000000A, 0x00000006, 0x00000070, 0x00010003,
    0x0000004B, 0x0003005B, (u32)btlevtcmd_GetSelectNextEnemy, 0xFE363C83, 0xFE363C84, 0x00010004,
    0x00000046, 0x00010003, 0x0000005A, 0x0002005B, (u32)_wait_yoshig_complete, 0xFFFFFFFE,
    0x0002005B, (u32)evt_btl_camera_noshake, 0x00000001, 0x0001000A, 0x000001F4, 0x0001005B,
    (u32)evt_audience_ap_recovery, 0x0001005B, (u32)btlevtcmd_InviteApInfoReport, 0x0002005B, (u32)btlevtcmd_ResetFaceDirection, 0xFFFFFFFE,
    0x0001005E, (u32)btldefaultevt_SuitoruBadgeEffect, 0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002,
    0x00000001,
};

u32 yoshi_appeal_event[] = {
    0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFFFFFFFE, 0x00000007, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType,
    0xFFFFFFFE, 0x00000001, 0x00000042, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_MARIO_APPEAL1_802f7c9c,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000, 0x00010009, 0x00000028,
    0x0002005B, (u32)btlevtcmd_AudienceAppeal, 0xFFFFFFFE, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE,
    0x00000001, 0x0000002B, 0x00010009, 0x0000001E, 0x0002005B, (u32)btlevtcmd_StartWaitEvent,
    0xFFFFFFFE, 0x00000002, 0x00000001,
};

static u32 party_win_reaction[] = {
    0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PYS_Y_1_802f7ad4, 0x0003005B,
    (u32)btlevtcmd_AnimeWaitPlayComplete, 0xFFFFFFFE, 0x00000001, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE,
    0x00000001, (u32)(char*)str_PYS_Y_1_802f7ad4, 0x0003005B, (u32)btlevtcmd_AnimeWaitPlayComplete, 0xFFFFFFFE, 0x00000001,
    0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001, 0x0000002B, 0x00000002,
    0x00000001,
};

static u32 attack_audience_event[] = {
    0x0005005B, (u32)_get_attack_aud_no, 0xFE363C83, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0007005B, (u32)btlevtcmd_CalculateFaceDirection, 0xFFFFFFFE, 0xFFFFFFFF, 0xFE363C80, 0xFE363C82,
    0x00000001, 0xFE363C8F, 0x0003005B, (u32)btlevtcmd_ChangeFaceDirection, 0xFFFFFFFE, 0xFE363C8F,
    0x00020032, 0xFE363C8C, 0x00000002, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00020035, 0xFE363C81, 0x0000000A,
    0x0006005B, (u32)btlevtcmd_SetPartsPos, 0xFFFFFFFE, 0xFE363C8C, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0000006E, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x0000000D,
    0x0005005B, (u32)evt_btl_camera_set_homing_unit_audience, 0x00000000, 0xFFFFFFFE, 0xFE363C8C, 0xFE363C83,
    0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000002, 0x0003005B, (u32)evt_btl_camera_set_zoom,
    0x00000000, 0x000000C8, 0x00000070, 0x0003005B, (u32)evt_sub_random, 0x00000002,
    0xFE363C80, 0x0005005B, (u32)btlevtcmd_SetPartsWork, 0xFFFFFFFE, 0xFE363C8C, 0x00000000,
    0xFE363C80, 0x00010022, 0xFE363C80, 0x00010024, 0x00000001, 0x0004005B,
    (u32)btlevtcmd_AnimeSetPoseTable, 0xFFFFFFFE, 0xFE363C8C, (u32)pose_table_egg_y, 0x00010024, 0x00000002,
    0x0004005B, (u32)btlevtcmd_AnimeSetPoseTable, 0xFFFFFFFE, 0xFE363C8C, (u32)pose_table_egg_p, 0x0000002A,
    0x0004005B, (u32)btlevtcmd_AnimeSetPoseTable, 0xFFFFFFFE, 0xFE363C8C, (u32)pose_table_egg_g, 0x00000031,
    0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0xFE363C8C, 0x00000045, 0x0004005B,
    (u32)btlevtcmd_SetPartsMoveSpeed, 0xFFFFFFFE, 0xFE363C8C, 0xF24A9880, 0x0004005B, (u32)btlevtcmd_SetPartsFallAccel,
    0xFFFFFFFE, 0xFE363C8C, 0xF24A7AE6, 0x0004005B, (u32)btlevtcmd_OnPartsAttribute, 0xFFFFFFFE,
    0xFE363C8C, 0x03000000, 0x0005005B, (u32)btlevtcmd_GetPartsWork, 0xFFFFFFFE, 0xFE363C8C,
    0x00000000, 0xFE363C80, 0x00010022, 0xFE363C80, 0x00010024, 0x00000001,
    0x00020032, 0xFE363C86, (u32)(char*)str_PYS_A3_1_802f7bd8, 0x00020032, 0xFE363C87, (u32)(char*)str_PYS_A3_2_802f7be4,
    0x00020032, 0xFE363C88, (u32)(char*)str_PYS_A3_3_802f7bf0, 0x00010024, 0x00000002, 0x00020032,
    0xFE363C86, (u32)(char*)str_PYS_A4_1_802f7bfc, 0x00020032, 0xFE363C87, (u32)(char*)str_PYS_A4_2_802f7c08, 0x00020032,
    0xFE363C88, (u32)(char*)str_PYS_A4_3_802f7c14, 0x0000002A, 0x00020032, 0xFE363C86, (u32)(char*)str_PYS_A2_1_802f7c20,
    0x00020032, 0xFE363C87, (u32)(char*)str_PYS_A2_2_802f7c2c, 0x00020032, 0xFE363C88, (u32)(char*)str_PYS_A2_3_802f7c38,
    0x00000031, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, 0xFE363C86,
    0x0003005B, (u32)btlevtcmd_AnimeWaitPlayComplete, 0xFFFFFFFE, 0x00000001, 0x0004005B, (u32)btlevtcmd_AnimeChangePose,
    0xFFFFFFFE, 0x00000001, 0xFE363C88, 0x0000006E, 0x0001000A, 0x000003E8,
    0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001, 0x0000002B, 0x00000070,
    0x00010009, 0x00000007, 0x0004005B, (u32)btlevtcmd_OffPartsAttribute, 0xFFFFFFFE, 0xFE363C8C,
    0x03000000, 0x0005005B, (u32)_get_attack_aud_no, 0xFE363C83, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00020035, 0xFE363C81, 0x0000001C, 0x0000006E, 0x0007005B,
    (u32)btlevtcmd_GetMoveFrame, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0xF24A8E80,
    0xFE363C85, 0x0006005B, (u32)btlevtcmd_SetPartsRotateOffset, 0xFFFFFFFE, 0xFE363C8C, 0x00000000,
    0x00000006, 0x00000000, 0x00010005, 0xFE363C85, 0x0006005B, (u32)btlevtcmd_AddPartsRotate,
    0xFFFFFFFE, 0xFE363C8C, 0x00000000, 0x00000000, 0x00000030, 0x00010009,
    0x00000001, 0x00000006, 0x00000070, 0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_BTL_YOSHI_EGG3_802f7c44,
    0x00000000, 0x000B005B, (u32)btlevtcmd_DivePartsPosition, 0xFFFFFFFE, 0xFE363C8C, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000000, 0x00000019, 0x00000000, 0x00000000,
    0xFFFFFFFF, 0x0004005B, (u32)btlevtcmd_OnPartsAttribute, 0xFFFFFFFE, 0xFE363C8C, 0x03000000,
    0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_BTL_YOSHI_EGG2_802f7c58, 0x00000000, 0x000F005B, (u32)evt_eff,
    (u32)(u8*)zero_80424094, (u32)(char*)str_bomb_80424098, 0x00000000, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0xF24A7CE6, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x0003005B, (u32)_attack_aud, 0xFE363C83, 0x00000000,
    0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000003, 0x0002005B, (u32)btlevtcmd_ResetFaceDirection,
    0xFFFFFFFE, 0x0001006F, 0xFE363C8F, 0x0001000A, 0x000005DC, 0x0003005B,
    (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000, 0x0001000A, 0x000001F4, 0x00000070,
    0x0002005B, (u32)btlevtcmd_WaitEventEnd, 0xFE363C8F, 0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE,
    0x00000002, 0x00000001,
};

static u32 attack_event[] = {
    0x00000002, 0x00000001,
};

static u32 wait_event[] = {
    0x0003005B, (u32)btlevtcmd_AnimeChangePoseFromTable, 0xFFFFFFFE, 0x00000001, 0x00000002, 0x00000001,
};

u8 _yoshig_tpl_no_tbl[6] = {
    0x5C, 0x5F, 0x5D, 0x5C, 0x60, 0x5E
};

/* Recovered target .rodata suffix after the compiler conversion double. */
const char str_SFX_BTL_YOSHI_TAIGUN_802f7cb8[] = "SFX_BTL_YOSHI_TAIGUN1";
const char str_c_babyyoshi2_802f7cd0[] = "c_babyyoshi2";
const char str_c_babyyoshi3_802f7ce0[] = "c_babyyoshi3";
const char str_c_babyyoshi4_802f7cf0[] = "c_babyyoshi4";
const char str_c_babyyoshi5_802f7d00[] = "c_babyyoshi5";
const char str_c_babyyoshi6_802f7d10[] = "c_babyyoshi6";
const char str_c_babyyoshi7_802f7d20[] = "c_babyyoshi7";
