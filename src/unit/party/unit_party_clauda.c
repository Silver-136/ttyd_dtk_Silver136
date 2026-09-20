#include "unit/party/unit_party_clauda.h"

extern void* _battleWorkPointer;
extern s32 BattleTransID(void* evt, s32 id);
extern void* BattleGetUnitPtr(void* battleWork, s32 id);
extern s32 evtGetValue(void* evt, s32 arg);
extern void evtSetValue(void* evt, s32 arg, s32 value);
extern void* memcpy(void* dest, const void* src, u32 size);
extern void BtlUnit_GetPos(void* unit, f32* x, f32* y, f32* z);
extern void* effKumokumoN64Entry(s32 type, s32 time, f32 x, f32 y, f32 z, f32 angle, f32 speed, f32 scale);
extern const f32 float_0_80424074;
extern const f32 float_1_8042407c;
extern const f32 float_180_80424080;

s32 _clauda_make_extra_work_area(void* evt) {
    extern void* BattleAlloc(u32 size);
    void* battleWork;
    s32 id;
    void* unit;

    battleWork = _battleWorkPointer;
    id = BattleTransID(evt, -2);
    unit = BattleGetUnitPtr(battleWork, id);
    *(void**)((s32)unit + 0x314) = BattleAlloc(0xC0);
    return 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void __makeTechMenuFunc(void* commandWork, s32* count) {
    extern void* BattleGetPartyPtr(void* battleWork);
    extern s32 BattleTransPartyId(s32 id);
    extern s32 partyGetTechLv(s32 partyId);
    extern char* msgSearch(char* msg);
    extern u8 lbl_80378D60[];
    s32 techLv;
    void* party;
    u8* data;
    u8* entry;

    party = BattleGetPartyPtr(_battleWorkPointer);
    techLv = partyGetTechLv(BattleTransPartyId(*(s32*)((s32)party + 8)));
    data = lbl_80378D60;

    entry = (u8*)commandWork + *count * 0x1C;
    *(s32*)(entry + 0x90) = -1;
    entry = (u8*)commandWork + *count * 0x1C;
    *(s32*)(entry + 0x94) = 0;
    entry = (u8*)commandWork + *count * 0x1C;
    *(void**)(entry + 0x80) = data + 0x40;
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
        *(void**)(entry + 0x80) = data + 0x100;
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
        *(void**)(entry + 0x80) = data + 0x1C0;
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
        *(void**)(entry + 0x80) = data + 0x280;
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


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 _check_blow_rate(void* evt) {

    s32* args = *(s32**)((s32)evt + 0x18);
    void* battleWork = _battleWorkPointer;
    s32 id = evtGetValue(evt, args[0]);
    s32 out = args[1];
    void* unit = BattleGetUnitPtr(battleWork, BattleTransID(evt, id));
    evtSetValue(evt, out, *(u8*)((s32)*(void**)((s32)unit + 0x144) + 0x10));
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 _clauda_breath_effect_ready(void* evt) {
    extern const f32 float_25_80424088;
    extern const f32 float_2_8042408c;

    void* battleWork = _battleWorkPointer;
    s32 id = BattleTransID(evt, -2);
    void* unit = BattleGetUnitPtr(battleWork, id);
    f32 z;
    f32 y;
    f32 x;
    void* eff;
    void* work;
    s8 dir;
    f32 scale;

    BtlUnit_GetPos(unit, &x, &y, &z);
    dir = *(s8*)((s32)unit + 0x189);
    scale = *(f32*)((s32)unit + 0x114);
    x += (f32)(dir * 20) * scale;
    y += float_25_80424088 * scale;
    if (dir >= 0) {
        eff = effKumokumoN64Entry(0, 60, x, y, z, float_0_80424074, float_2_8042408c, float_1_8042407c);
    } else {
        eff = effKumokumoN64Entry(0, 60, x, y, z, float_180_80424080, float_2_8042408c, float_1_8042407c);
    }
    work = *(void**)((s32)eff + 0xC);
    *(s32*)((s32)work + 0x18) = 0x4C;
    *(s32*)((s32)*(void**)((s32)eff + 0xC) + 0x1C) = 0xA7;
    *(s32*)((s32)*(void**)((s32)eff + 0xC) + 0x20) = 0xCA;
    *(s32*)((s32)*(void**)((s32)eff + 0xC) + 0x28) = 0x7D;
    *(s32*)((s32)*(void**)((s32)eff + 0xC) + 0x2C) = 0x1B;
    *(s32*)((s32)*(void**)((s32)eff + 0xC) + 0x30) = 0xF5;
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 _clauda_breath_effect_fire(void* evt) {
    extern const f32 float_15_80424070;
    extern const f32 float_neg2_80424078;
    extern const f32 float_neg10_80424084;

    void* battleWork = _battleWorkPointer;
    s32* args = *(s32**)((s32)evt + 0x18);
    s32 id = BattleTransID(evt, -2);
    void* unit = BattleGetUnitPtr(battleWork, id);
    s32 out = args[0];
    f32 z;
    f32 y;
    f32 x;
    void* eff;
    void* work;
    s8 dir;
    f32 scale;

    BtlUnit_GetPos(unit, &x, &y, &z);
    dir = *(s8*)((s32)unit + 0x189);
    scale = *(f32*)((s32)unit + 0x114);
    x += (f32)(dir * 25) * scale;
    y += float_15_80424070 * scale;
    if (dir >= 0) {
        eff = effKumokumoN64Entry(0, 0, x, y, z, float_0_80424074, float_neg2_80424078, float_1_8042407c);
    } else {
        eff = effKumokumoN64Entry(0, 0, x, y, z, float_180_80424080, float_neg2_80424078, float_1_8042407c);
    }
    work = *(void**)((s32)eff + 0xC);
    *(s32*)((s32)work + 0x18) = 0x4C;
    *(s32*)((s32)*(void**)((s32)eff + 0xC) + 0x1C) = 0xA7;
    *(s32*)((s32)*(void**)((s32)eff + 0xC) + 0x20) = 0xCA;
    *(s32*)((s32)*(void**)((s32)eff + 0xC) + 0x28) = 0x7D;
    *(s32*)((s32)*(void**)((s32)eff + 0xC) + 0x2C) = 0x1B;
    *(s32*)((s32)*(void**)((s32)eff + 0xC) + 0x30) = 0xF5;
    *(f32*)((s32)*(void**)((s32)eff + 0xC) + 0x58) = float_neg10_80424084;
    evtSetValue(evt, out, (s32)eff);
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


s32 _make_breath_weapon(void* pEvt) {

    void* battleWork = _battleWorkPointer;
    s32* args = *(s32**)((s32)pEvt + 0x18);
    s32 id = BattleTransID(pEvt, -2);
    void* unit = BattleGetUnitPtr(battleWork, id);
    s32 out = args[0];
    void* weapon = *(void**)((s32)unit + 0x314);
    s32 src = evtGetValue(pEvt, out);
    s32 flag = evtGetValue(pEvt, args[1]);
    memcpy(weapon, (void*)src, 0xC0);
    *(u8*)((s32)weapon + 0xA5) = flag;
    evtSetValue(pEvt, out, (s32)weapon);
    return 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 _make_kiss_weapon(void* evt) {

    void* battleWork = _battleWorkPointer;
    s32* args = *(s32**)((s32)evt + 0x18);
    s32 id = BattleTransID(evt, -2);
    void* unit = BattleGetUnitPtr(battleWork, id);
    s32 out = args[0];
    void* weapon = *(void**)((s32)unit + 0x314);
    s32 src = evtGetValue(evt, out);
    evtGetValue(evt, args[1]);
    memcpy(weapon, (void*)src, 0xC0);
    evtSetValue(evt, out, (s32)weapon);
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

s32 _get_clauda_kiss_hit_position(void* evt) {
    extern f32 BattleGetFloorHeight(void* battleWork, f32 x, f32 y, f32 z);
    extern void evtSetFloat(void* evt, s32 arg, f32 value);
    extern const f32 float_30_8042406c;
    s32* args;
    void* battleWork;
    s32 ownerId;
    s32 targetId;
    s32 outX;
    s32 outY;
    s32 outZ;
    void* owner;
    void* target;
    f32 x;
    f32 y;
    f32 z;
    f32 floor;
    s8 dir;
    f32 scale;

    args = *(s32**)((s32)evt + 0x18);
    battleWork = _battleWorkPointer;
    ownerId = evtGetValue(evt, args[0]);
    targetId = evtGetValue(evt, args[1]);
    evtGetValue(evt, args[2]);
    outX = args[3];
    outY = args[4];
    outZ = args[5];
    owner = BattleGetUnitPtr(battleWork, BattleTransID(evt, ownerId));
    target = BattleGetUnitPtr(battleWork, BattleTransID(evt, targetId));
    dir = *(s8*)((s32)target + 0x189);
    BtlUnit_GetPos(target, &x, &y, &z);
    scale = *(f32*)((s32)target + 0x114);
    x += (f32)dir * (*(f32*)((s32)target + 0xF8) * scale);
    y += *(f32*)((s32)target + 0xFC) * scale;
    z += *(f32*)((s32)target + 0x100) * scale;
    y -= float_30_8042406c * *(f32*)((s32)owner + 0x114);
    floor = BattleGetFloorHeight(battleWork, x, y, z);
    if (floor > y) {
        y = floor;
    }
    evtSetFloat(evt, outX, x);
    evtSetFloat(evt, outY, y);
    evtSetFloat(evt, outZ, z);
    return 2;
}
s32 _make_kumoguard_weapon(void* evt) {

    void* battleWork = _battleWorkPointer;
    s32* args = *(s32**)((s32)evt + 0x18);
    s32 id = BattleTransID(evt, -2);
    void* unit = BattleGetUnitPtr(battleWork, id);
    s32 out = args[0];
    void* weapon = *(void**)((s32)unit + 0x314);
    s32 src = evtGetValue(evt, out);
    s32 flag = evtGetValue(evt, args[1]);
    memcpy(weapon, (void*)src, 0xC0);
    *(u8*)((s32)weapon + 0x8E) = flag;
    evtSetValue(evt, out, (s32)weapon);
    return 2;
}


/* Recovered Clauda battle-event dependencies. */
extern const char str_btl_un_clauda_802f7598[];
extern const char str_c_windy_802f75a8[];
extern const char str_SFX_PARTY_DAMAGE1_802f75b0[];
extern const char str_SFX_MARIO_DAMAGE_FIR_802f75c4[];
extern const char str_SFX_MARIO_DAMAGE_ICE_802f75dc[];
extern const char str_SFX_MARIO_DAMAGE_BIR_802f75f4[];
extern u32 btldefaultevt_Dummy[];
extern u32 btldefaultevt_avoid_counter_event[];
extern const char str_PWD_C_1_802f757c[];
extern const char str_PWD_D_1_802f7554[];
extern const char str_PWD_D_2_802f7524[];
extern const char str_PWD_D_5_802f7514[];
extern const char str_PWD_G_1_802f754c[];
extern const char str_PWD_G_2_802f7544[];
extern const char str_PWD_I_1_802f7574[];
extern const char str_PWD_N_1_802f752c[];
extern const char str_PWD_O_1_802f7584[];
extern const char str_PWD_R_1_802f7564[];
extern const char str_PWD_S_1_802f753c[];
extern const char str_PWD_T_1_802f756c[];
extern const char str_PWD_W_1_802f755c[];
extern const char str_PWD_X_1_802f751c[];
extern const char str_btl_wn_pwd_lv1_802f7474[];
extern const char str_btl_wn_pwd_lv2_802f74a4[];
extern const char str_btl_wn_pwd_lv3_802f74dc[];
extern const char str_btl_wn_pwd_normal_802f7438[];
extern const char str_msg_ac_body_press_802f7460[];
extern const char str_msg_ac_breath_802f7494[];
extern const char str_msg_ac_kumogakure_802f7500[];
extern const char str_msg_ac_sexy_kiss_802f74c8[];
extern const char str_msg_pwd_body_press_802f744c[];
extern const char str_msg_pwd_breath_802f7484[];
extern const char str_msg_pwd_kumogakure_802f74ec[];
extern const char str_msg_pwd_sexy_kiss_802f74b4[];
extern u32 subset_attack_ready_jump_event[];
extern s32 weaponGetACOutputParam();
extern s32 weaponGetPowerFromPartyAttackLv();
static u32 battle_entry_event[59];
static u32 init_event[60];
static u32 damage_event[16];
extern u32 partyClaudaAttack_NormalAttack[];
extern u32 partyClaudaAttack_BreathAttack[];
extern u32 partyClaudaAttack_PredationAttack[];
extern u32 partyClaudaAttack_KumoGuard[];
extern u32 clauda_appeal_event[];
static u32 attack_audience_event[15];
extern u32 attack_bad_audience_event[];
extern u32 attack_good_audience_event[];
static u32 attack_event[2];
static u32 wait_event[6];
static u32 party_win_reaction[11];
extern s32 _attack_aud();
extern s32 _check_aud_item_type();
extern s32 _get_attack_aud_no();
extern u32 btldefaultevt_Confuse[];
extern u32 btldefaultevt_Damage[];
extern u32 btldefaultevt_SuitoruBadgeEffect[];
extern s32 btlevtcmd_ACRGetResult();
extern s32 btlevtcmd_ACRStart();
extern s32 btlevtcmd_ACSuccessEffect();
extern s32 btlevtcmd_AcGetDifficulty();
extern s32 btlevtcmd_AcGetOutputParam();
extern s32 btlevtcmd_AcSetDifficulty();
extern s32 btlevtcmd_AcSetFlag();
extern s32 btlevtcmd_AcSetGaugeParam();
extern s32 btlevtcmd_AcSetParamAll();
extern s32 btlevtcmd_AnimeChangePose();
extern s32 btlevtcmd_AnimeChangePoseFromTable();
extern s32 btlevtcmd_AnimeChangePoseType();
extern s32 btlevtcmd_AnimeWaitPlayComplete();
extern s32 btlevtcmd_AttackDeclare();
extern s32 btlevtcmd_AttackDeclareAll();
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
extern s32 btlevtcmd_CommandPreCheckCounter();
extern s32 btlevtcmd_CommandPreCheckDamage();
extern s32 btlevtcmd_DamageDirect();
extern s32 btlevtcmd_DivePosition();
extern s32 btlevtcmd_FaceDirectionAdd();
extern s32 btlevtcmd_FaceDirectionSub();
extern s32 btlevtcmd_GetDamageCode();
extern s32 btlevtcmd_GetEnemyBelong();
extern s32 btlevtcmd_GetFaceDirection();
extern s32 btlevtcmd_GetHeight();
extern s32 btlevtcmd_GetHitPos();
extern s32 btlevtcmd_GetHomePos();
extern s32 btlevtcmd_GetHpDamage();
extern s32 btlevtcmd_GetMoveFrame();
extern s32 btlevtcmd_GetPartyTechLv();
extern s32 btlevtcmd_GetPos();
extern s32 btlevtcmd_GetRGB();
extern s32 btlevtcmd_GetResultAC();
extern s32 btlevtcmd_GetResultPrizeLv();
extern s32 btlevtcmd_GetScale();
extern s32 btlevtcmd_GetSelectEnemy();
extern s32 btlevtcmd_GetSelectNextEnemy();
extern s32 btlevtcmd_GetStatusMg();
extern s32 btlevtcmd_InviteApInfoReport();
extern s32 btlevtcmd_JumpContinue();
extern s32 btlevtcmd_JumpPosition();
extern s32 btlevtcmd_JumpSetting();
extern s32 btlevtcmd_MarioJumpParam();
extern s32 btlevtcmd_MarioJumpPosition();
extern s32 btlevtcmd_MovePosition();
extern s32 btlevtcmd_OffUnitFlag();
extern s32 btlevtcmd_OnUnitFlag();
extern s32 btlevtcmd_PayWeaponCost();
extern s32 btlevtcmd_PreCheckDamage();
extern s32 btlevtcmd_RecoverHp();
extern s32 btlevtcmd_ResetFaceDirection();
extern s32 btlevtcmd_ResultAC();
extern s32 btlevtcmd_RunDataEventChild();
extern s32 btlevtcmd_SamplingEnemy();
extern s32 btlevtcmd_SetDispOffset();
extern s32 btlevtcmd_SetDiveSound();
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
extern s32 btlevtcmd_SetRGB();
extern s32 btlevtcmd_SetRotate();
extern s32 btlevtcmd_SetRunSound();
extern s32 btlevtcmd_SetScale();
extern s32 btlevtcmd_SetWalkSound();
extern s32 btlevtcmd_SetupAC();
extern s32 btlevtcmd_StageDispellFog();
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
extern s32 evt_btl_camera_nomove_x_onoff();
extern s32 evt_btl_camera_nomove_y_onoff();
extern s32 evt_btl_camera_nomove_z_onoff();
extern s32 evt_btl_camera_set_homing_unit();
extern s32 evt_btl_camera_set_homing_unit_audience();
extern s32 evt_btl_camera_set_homing_unitparts();
extern s32 evt_btl_camera_set_mode();
extern s32 evt_btl_camera_set_moveSpeedLv();
extern s32 evt_btl_camera_set_moveto();
extern s32 evt_btl_camera_set_zoom();
extern s32 evt_btl_camera_shake_h();
extern s32 evt_eff();
extern s32 evt_eff_delete_ptr();
extern s32 evt_snd_sfxoff();
extern s32 evt_snd_sfxon_3d();
extern s32 evt_sub_intpl_get_float();
extern s32 evt_sub_intpl_init_float();
extern s32 evt_sub_random();
extern u32 partyWeapon_ClaudaBreathAttack[];
extern const char str_PWD_A1_1_802f7678[];
extern const char str_PWD_A1_2_802f769c[];
extern const char str_PWD_A2_1_802f76dc[];
extern const char str_PWD_A2_2_802f7700[];
extern const char str_PWD_A2_3_802f770c[];
extern const char str_PWD_A3_1_802f7730[];
extern const char str_PWD_A3_2_802f773c[];
extern const char str_PWD_A3_3_802f777c[];
extern const char str_PWD_A4_2_802f7790[];
extern const char str_PWD_A4_3_802f758c[];
extern const char str_PWD_A_7_802f7788[];
extern const char str_PWD_D_3_802f7534[];
extern const char str_PWD_Y_1_802f76d4[];
extern const char str_SFX_BTL_CLAUD_ATT2_802f76a8[];
extern const char str_SFX_BTL_CLAUD_ATTACK_802f7684[];
extern const char str_SFX_BTL_CLAUD_ATTACK_802f76bc[];
extern const char str_SFX_BTL_CLAUD_BREATH_802f76e8[];
extern const char str_SFX_BTL_CLAUD_BREATH_802f7718[];
extern const char str_SFX_BTL_CLAUD_HIDE2_802f779c[];
extern const char str_SFX_BTL_CLAUD_KISS1_802f7748[];
extern const char str_SFX_BTL_CLAUD_KISS2_802f7768[];
extern const char str_SFX_MARIO_APPEAL1_802f77bc[];
extern const char str_SFX_PARTY_BATTLE_HAN_802f762c[];
extern const char str_SFX_PARTY_BATTLE_JUM_802f7644[];
extern const char str_SFX_PARTY_BATTLE_LAN_802f765c[];
extern const char str_SFX_PARTY_BATTLE_SUB_802f7610[];
extern const char str_kemuri_test_802f77b0[];
extern const char str_kiss_80424064[];
extern const char str_recovery_802f775c[];
extern const u8 zero_80424060[];

/* Recovered target Clauda BattleWeapon/pose/data-table core. */
u32 partyWeapon_ClaudaNormalAttack[48] = {
    (u32)(char*)str_btl_wn_pwd_normal_802f7438, 0x01580000, 0x00000000, (u32)(char*)str_msg_pwd_body_press_802f744c, 0x64000000, 0x3F800000,
    0x01016432, (u32)weaponGetPowerFromPartyAttackLv, 0x00000001, 0x00000002, 0x00000002, 0x00000004,
    0x00000003, 0x00000006, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01001260, 0x20000000, 0x000A0302, (u32)(char*)str_msg_ac_body_press_802f7460, 0x00035087,
    0x0000000C, 0x80002004, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)partyClaudaAttack_NormalAttack, 0x00141464, 0x140A0205, 0x0A000000
};

u32 partyWeapon_ClaudaBreathAttack[48] = {
    (u32)(char*)str_btl_wn_pwd_lv1_802f7474, 0x01590000, 0x00000000, (u32)(char*)str_msg_pwd_breath_802f7484, 0x64040000, 0x3F800000,
    0x01056432, 0x00000000, 0x00000001, 0x00000002, 0x00000002, 0x00000003,
    0x00000003, 0x00000004, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x02101260, 0x20000000, 0x00190302, (u32)(char*)str_msg_ac_breath_802f7494, 0x00020287,
    0x000007FF, 0x80002004, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00640000,
    0x00000000, 0x00000000, (u32)partyClaudaAttack_BreathAttack, 0x00000064, 0x000A0500, 0x00000000
};

u32 partyWeapon_ClaudaPredationAttack[48] = {
    (u32)(char*)str_btl_wn_pwd_lv2_802f74a4, 0x015A0000, 0x00000000, (u32)(char*)str_msg_pwd_sexy_kiss_802f74b4, 0x64030000, 0x3F800000,
    0x01056432, (u32)weaponGetACOutputParam, 0x00000000, 0x00000001, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000003,
    0x00000003, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01101260, 0x20000000, 0x00000302, (u32)(char*)str_msg_ac_sexy_kiss_802f74c8, 0x000200C7,
    0x00000002, 0x80002004, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)partyClaudaAttack_PredationAttack, 0x00000064, 0x00000500, 0x00000000
};

u32 partyWeapon_ClaudaKumogakureAttack[48] = {
    (u32)(char*)str_btl_wn_pwd_lv3_802f74dc, 0x015B0000, 0x00000000, (u32)(char*)str_msg_pwd_kumogakure_802f74ec, 0x64040000, 0x3F800000,
    0x01056432, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01101160, 0x10000000, 0x00000302, (u32)(char*)str_msg_ac_kumogakure_802f7500, 0x00020008,
    0x000007FF, 0x80002004, 0x00000000, 0x00000000, 0x00000000, 0x00640200,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)partyClaudaAttack_KumoGuard, 0x00000064, 0x00000500, 0x00000000
};

u32 pose_table_clauda_stay[50] = {
    0x00000009, (u32)(char*)str_PWD_D_5_802f7514, 0x00000002, (u32)(char*)str_PWD_D_5_802f7514, 0x00000003, (u32)(char*)str_PWD_X_1_802f751c,
    0x00000004, (u32)(char*)str_PWD_D_2_802f7524, 0x00000005, (u32)(char*)str_PWD_X_1_802f751c, 0x00000001, (u32)(char*)str_PWD_N_1_802f752c,
    0x0000001B, (u32)(char*)str_PWD_D_3_802f7534, 0x0000001C, (u32)(char*)str_PWD_S_1_802f753c, 0x00000025, (u32)(char*)str_PWD_G_2_802f7544,
    0x0000001F, (u32)(char*)str_PWD_G_1_802f754c, 0x00000027, (u32)(char*)str_PWD_D_1_802f7554, 0x00000028, (u32)(char*)str_PWD_W_1_802f755c,
    0x0000002A, (u32)(char*)str_PWD_R_1_802f7564, 0x00000039, (u32)(char*)str_PWD_D_2_802f7524, 0x00000038, (u32)(char*)str_PWD_D_2_802f7524,
    0x0000003C, (u32)(char*)str_PWD_D_3_802f7534, 0x0000003D, (u32)(char*)str_PWD_D_2_802f7524, 0x00000041, (u32)(char*)str_PWD_T_1_802f756c,
    0x00000034, (u32)(char*)str_PWD_I_1_802f7574, 0x00000043, (u32)(char*)str_PWD_C_1_802f757c, 0x00000042, (u32)(char*)str_PWD_O_1_802f7584,
    0x00000040, (u32)(char*)str_PWD_X_1_802f751c, 0x00000031, (u32)(char*)str_PWD_A4_3_802f758c, 0x0000002D, (u32)(char*)str_PWD_G_1_802f754c,
    0x00000045, (u32)(char*)str_PWD_S_1_802f753c
};

static u32 data_table[16] = {
    0x00000005, (u32)clauda_appeal_event, 0x00000006, (u32)attack_audience_event, 0x00000001, (u32)__makeTechMenuFunc,
    0x00000030, (u32)btldefaultevt_Dummy, 0x00000040, (u32)party_win_reaction, 0x00000007, (u32)subset_attack_ready_jump_event,
    0x0000002F, (u32)btldefaultevt_avoid_counter_event, 0x00000000, 0x00000000
};

/* Recovered target Clauda .sdata unit metadata prefix. */
static u8 defence[5] = {0, 0, 0, 0, 0};
static u8 defence_attr[5] = {0, 0, 0, 0, 0};
static u8 regist[22] = {
    100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
    100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100
};

/* Recovered target Clauda event/event-like arrays. */
static u32 attack_event[2] = {
    0x00000002, 0x00000001
};

static u32 wait_event[6] = {
    0x0003005B, (u32)btlevtcmd_AnimeChangePoseFromTable, 0xFFFFFFFE, 0x00000001, 0x00000002, 0x00000001
};

static u32 party_win_reaction[11] = {
    0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PWD_Y_1_802f76d4, 0x0003005B,
    (u32)btlevtcmd_AnimeWaitPlayComplete, 0xFFFFFFFE, 0x00000001, 0x00000002, 0x00000001
};

/* Recovered target Clauda BattleUnitKindPart/BattleUnitKind records. */
u32 unitpartsdata_Party_Clauda[19] = {
    0x00000001, (u32)(char*)str_btl_un_clauda_802f7598, (u32)(char*)str_c_windy_802f75a8, 0x00000000, 0x00000000, 0x00000000,
    0x41800000, 0x42300000, 0x40A00000, 0x00000000, 0x42300000, 0x00000000,
    0x001A0032, 0x00FF0000, (u32)defence, (u32)defence_attr, 0x00000009, 0x00000000,
    (u32)pose_table_clauda_stay
};

u32 unitdata_Party_Clauda[49] = {
    0x000000E3, (u32)(char*)str_btl_un_clauda_802f7598, 0x000A0005, 0x05010100, 0x00000064, 0x00090014,
    0x00320010, 0x002C0000, 0x00000000, 0x41400000, 0x00000000, 0x00000000,
    0x00000000, 0x42200000, 0x00000000, 0x00000000, 0x42200000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x41200000,
    0x00000000, 0x00000000, 0x41200000, 0x42020000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0000FF00, 0x64140000,
    0x00000000, 0x00000000, 0x00000000, (u32)(char*)str_SFX_PARTY_DAMAGE1_802f75b0, (u32)(char*)str_SFX_MARIO_DAMAGE_FIR_802f75c4, (u32)(char*)str_SFX_MARIO_DAMAGE_ICE_802f75dc,
    (u32)(char*)str_SFX_MARIO_DAMAGE_BIR_802f75f4, 0x02000004, (u32)regist, 0x01000000, (u32)unitpartsdata_Party_Clauda, (u32)init_event,
    (u32)data_table
};

static u32 battle_entry_event[59] = {
    0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE, 0xFE363C80, 0x000000FA, 0x0005005B,
    (u32)btlevtcmd_SetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00010009,
    0x00000028, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001, 0x0000002A,
    0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE, 0xF24A8A80, 0x0009005B, (u32)btlevtcmd_SetMoveSoundDataWork,
    0xFFFFFFFE, (u32)(char*)str_SFX_PARTY_BATTLE_SUB_802f7610, (u32)(char*)str_SFX_PARTY_BATTLE_SUB_802f7610, 0x00000000, 0x00000009, 0x00000003,
    0x00000000, 0x00000000, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000000, 0xFFFFFFFE, 0x00000000, 0x0003005B,
    (u32)btlevtcmd_AnimeChangePoseFromTable, 0xFFFFFFFE, 0x00000001, 0x00000002, 0x00000001
};

static u32 init_event[60] = {
    0x0001005B, (u32)_clauda_make_extra_work_area, 0x0003005B, (u32)btlevtcmd_SetEventWait, 0xFFFFFFFE, (u32)wait_event,
    0x0003005B, (u32)btlevtcmd_SetEventAttack, 0xFFFFFFFE, (u32)attack_event, 0x0003005B, (u32)btlevtcmd_SetEventDamage,
    0xFFFFFFFE, (u32)damage_event, 0x0003005B, (u32)btlevtcmd_SetEventConfusion, 0xFFFFFFFE, (u32)btldefaultevt_Confuse,
    0x0007005B, (u32)btlevtcmd_SetWalkSound, 0xFFFFFFFE, (u32)(char*)str_SFX_PARTY_BATTLE_HAN_802f762c, (u32)(char*)str_SFX_PARTY_BATTLE_HAN_802f762c, 0x00000006,
    0x0000000C, 0x00000010, 0x0007005B, (u32)btlevtcmd_SetRunSound, 0xFFFFFFFE, (u32)(char*)str_SFX_PARTY_BATTLE_HAN_802f762c,
    (u32)(char*)str_SFX_PARTY_BATTLE_HAN_802f762c, 0x00000000, 0x00000009, 0x00000008, 0x0007005B, (u32)btlevtcmd_SetDiveSound,
    0xFFFFFFFE, (u32)(char*)str_SFX_PARTY_BATTLE_HAN_802f762c, (u32)(char*)str_SFX_PARTY_BATTLE_HAN_802f762c, 0x00000000, 0x00000009, 0x00000008,
    0x0004005B, (u32)btlevtcmd_SetJumpSound, 0xFFFFFFFE, (u32)(char*)str_SFX_PARTY_BATTLE_JUM_802f7644, (u32)(char*)str_SFX_PARTY_BATTLE_LAN_802f765c, 0x0003005B,
    (u32)btlevtcmd_SetEventEntry, 0xFFFFFFFE, (u32)battle_entry_event, 0x0003005B, (u32)btlevtcmd_OnUnitFlag, 0xFFFFFFFE,
    0x00300000, 0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002, 0x00000001
};

static u32 damage_event[16] = {
    0x0005005B, (u32)btlevtcmd_SetRotate, 0xFFFFFFFE, 0x00000000, 0x00000000, 0x00000000,
    0x00020032, 0xFE363C8A, 0xFFFFFFFE, 0x00020032, 0xFE363C8B, 0x00000001,
    0x0001005E, (u32)btldefaultevt_Damage, 0x00000002, 0x00000001
};

u32 partyClaudaAttack_NormalAttack[700] = {
    0x0005005B, (u32)btlevtcmd_JumpSetting, 0xFFFFFFFE, 0x00000014, 0xF24A7A80, 0xF24A7D4C,
    0x0003005B, (u32)btlevtcmd_GetSelectEnemy, 0xFE363C83, 0xFE363C84, 0x00020018, 0xFE363C83,
    0xFFFFFFFF, 0x00010004, 0x00000063, 0x00000021, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress,
    0xFFFFFFFE, 0xFE363C8C, 0x0002005B, (u32)btlevtcmd_WeaponAftereffect, 0xFE363C8C, 0x0004005B,
    (u32)btlevtcmd_AttackDeclare, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0x0001005B, (u32)btlevtcmd_WaitGuardMove,
    0x0002005B, (u32)btlevtcmd_CommandPayWeaponCost, 0xFFFFFFFE, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFFFFFFFE,
    0x00000007, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x0000000B, 0x0006005B,
    (u32)evt_btl_camera_set_homing_unitparts, 0x00000000, 0xFFFFFFFE, 0x00000001, 0xFE363C83, 0xFE363C84,
    0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001, 0x0003005B, (u32)evt_btl_camera_set_zoom,
    0x00000000, 0x000000C8, 0x0007005B, (u32)btlevtcmd_CalculateFaceDirection, 0xFFFFFFFE, 0xFFFFFFFF,
    0xFE363C83, 0xFE363C84, 0x00000010, 0xFE363C8F, 0x0003005B, (u32)btlevtcmd_ChangeFaceDirection,
    0xFFFFFFFE, 0xFE363C8F, 0x0002005B, (u32)btlevtcmd_CommandGetWeaponActionLv, 0xFE363C80, 0x0003005B,
    (u32)btlevtcmd_AcSetDifficulty, 0xFFFFFFFE, 0xFE363C80, 0x0002005B, (u32)btlevtcmd_AcGetDifficulty, 0xFE363C80,
    0x0009005B, (u32)btlevtcmd_AcSetParamAll, 0xFE363C83, 0xFE363C84, 0x000000B4, 0xFE363C80,
    0xF1194D80, 0xF1194D80, 0xF1194D80, 0xF1194D80, 0x0002005B, (u32)btlevtcmd_AcSetFlag,
    0x00000000, 0x0005005B, (u32)btlevtcmd_SetupAC, 0xFFFFFFFE, 0x00000008, 0x00000001,
    0x00000000, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001, 0x0000002A,
    0x00010009, 0x00000014, 0x0002005B, (u32)btlevtcmd_StartAC, 0x00000001, 0x0001005B,
    (u32)btlevtcmd_ResultAC, 0x0001005B, (u32)btlevtcmd_StopAC, 0x0003005B, (u32)btlevtcmd_OffUnitFlag, 0xFFFFFFFE,
    0x00200000, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PWD_A1_1_802f7678,
    0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_BTL_CLAUD_ATTACK_802f7684, 0xF1194D80, 0x00000000,
    0xF1194D80, 0x0003005B, (u32)btlevtcmd_SetFallAccel, 0xFFFFFFFE, 0xF24A7B4C, 0x0006005B,
    (u32)btlevtcmd_GetHitPos, 0xFE363C83, 0xFE363C84, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00020035, 0xFE363C82, 0x00000005, 0x0002005B, (u32)btlevtcmd_GetResultAC, 0xFE363C85,
    0x0002001F, 0xFE363C85, 0x00000002, 0x0003005B, (u32)evt_sub_random, 0x00000028,
    0xFE363C85, 0x00020036, 0xFE363C85, 0x00000014, 0x00020035, 0xFE363C80,
    0xFE363C85, 0x0003005B, (u32)evt_sub_random, 0x00000014, 0xFE363C85, 0x00020036,
    0xFE363C85, 0x0000000A, 0x00020035, 0xFE363C81, 0xFE363C85, 0x00000021,
    0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0000003C, 0xFFFFFFFF, 0x0003005B, (u32)btlevtcmd_OnUnitFlag, 0xFFFFFFFE, 0x00200000,
    0x0006005B, (u32)btlevtcmd_CommandPreCheckDamage, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0x00000100,
    0xFE363C85, 0x00020019, 0xFE363C85, 0x00000001, 0x00010004, 0x0000000A,
    0x00000021, 0x0002005B, (u32)btlevtcmd_GetResultAC, 0xFE363C80, 0x0002001E, 0xFE363C80,
    0x00000002, 0x00010004, 0x00000014, 0x00000021, 0x00020032, 0xFE363C85,
    0x00000001, 0x00010003, 0x0000000A, 0x00020019, 0xFE363C85, 0x00000001,
    0x00020018, 0xFE363C85, 0x00000003, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83,
    0x00000026, 0x00000021, 0x00020018, 0xFE363C85, 0x00000006, 0x0003005B,
    (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000027, 0x00000021, 0x00020018, 0xFE363C85,
    0x00000002, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000028, 0x00000021,
    0x00000021, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PWD_A1_2_802f769c,
    0x0002005B, (u32)btlevtcmd_JumpContinue, 0xFFFFFFFE, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE,
    0x00000001, (u32)(char*)str_PWD_D_3_802f7534, 0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult, 0xFE363C8C, 0xFFFFFFFF,
    0x0001006F, 0xFE363C8F, 0x0005005B, (u32)evt_sub_intpl_init_float, 0x0000000B, 0xF24A7E80,
    0xF24A7C19, 0x0000000A, 0x00010005, 0x0000000A, 0x0003005B, (u32)evt_sub_intpl_get_float,
    0xFE363C85, 0xF1194D80, 0x0005005B, (u32)btlevtcmd_GetScale, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0005005B, (u32)btlevtcmd_SetScale, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C85, 0xFE363C82, 0x00010009, 0x00000001, 0x00000006, 0x0005005B,
    (u32)evt_sub_intpl_init_float, 0x0000000B, 0xF24A7C19, 0xF24A7E80, 0x00000006, 0x00010005,
    0x00000006, 0x0003005B, (u32)evt_sub_intpl_get_float, 0xFE363C85, 0xF1194D80, 0x0005005B,
    (u32)btlevtcmd_GetScale, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B,
    (u32)btlevtcmd_SetScale, 0xFFFFFFFE, 0xFE363C80, 0xFE363C85, 0xFE363C82, 0x00010009,
    0x00000001, 0x00000006, 0x00000070, 0x0000006E, 0x0005005B, (u32)evt_sub_intpl_init_float,
    0x0000000B, 0xF24A7E80, 0xF24A8080, 0x0000000A, 0x00010005, 0x0000000A,
    0x0003005B, (u32)evt_sub_intpl_get_float, 0xFE363C85, 0xF1194D80, 0x0005005B, (u32)btlevtcmd_GetScale,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)btlevtcmd_SetScale,
    0xFFFFFFFE, 0xFE363C85, 0xFE363C81, 0xFE363C82, 0x00010009, 0x00000001,
    0x00000006, 0x0005005B, (u32)evt_sub_intpl_init_float, 0x0000000B, 0xF24A8080, 0xF24A7E80,
    0x00000006, 0x00010005, 0x00000006, 0x0003005B, (u32)evt_sub_intpl_get_float, 0xFE363C85,
    0xF1194D80, 0x0005005B, (u32)btlevtcmd_GetScale, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0005005B, (u32)btlevtcmd_SetScale, 0xFFFFFFFE, 0xFE363C85, 0xFE363C81,
    0xFE363C82, 0x00010009, 0x00000001, 0x00000006, 0x00000070, 0x0002005B,
    (u32)btlevtcmd_WaitEventEnd, 0xFE363C8F, 0x00010009, 0x0000001E, 0x0001005B, (u32)evt_audience_ap_recovery,
    0x0001005B, (u32)btlevtcmd_InviteApInfoReport, 0x00010004, 0x0000005F, 0x00010003, 0x00000014,
    0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_BTL_CLAUD_ATT2_802f76a8, 0xF1194D80, 0x00000000,
    0xF1194D80, 0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_BTL_CLAUD_ATTACK_802f76bc, 0xF1194D80,
    0x00000000, 0xF1194D80, 0x0002005B, (u32)btlevtcmd_GetResultAC, 0xFE363C80, 0x0002001E,
    0xFE363C80, 0x00000002, 0x0006005B, (u32)btlevtcmd_CommandCheckDamage, 0xFFFFFFFE, 0xFE363C83,
    0xFE363C84, 0x00020100, 0xFE363C85, 0x0004005B, (u32)btlevtcmd_GetResultPrizeLv, 0xFE363C83,
    0x00000000, 0xFE363C86, 0x0006005B, (u32)btlevtcmd_GetHitPos, 0xFE363C83, 0xFE363C84,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)btlevtcmd_ACSuccessEffect, 0xFE363C86,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult, 0xFE363C8C,
    0xFE363C86, 0x00000020, 0x0006005B, (u32)btlevtcmd_CommandCheckDamage, 0xFFFFFFFE, 0xFE363C83,
    0xFE363C84, 0x00000100, 0xFE363C85, 0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult, 0xFE363C8C,
    0xFFFFFFFF, 0x00000021, 0x0002005B, (u32)btlevtcmd_JumpContinue, 0xFFFFFFFE, 0x0001006F,
    0xFE363C8E, 0x0006005B, (u32)btlevtcmd_ACRStart, 0xFFFFFFFE, 0x00000000, 0x00000010,
    0x00000010, 0x00000014, 0x00000070, 0x0006005B, (u32)evt_btl_camera_shake_h, 0x00000000,
    0x00000008, 0x00000000, 0x0000001E, 0x0000000D, 0x0001006F, 0xFE363C8F,
    0x0005005B, (u32)evt_sub_intpl_init_float, 0x0000000B, 0xF24A7E80, 0xF24A7C19, 0x0000000A,
    0x00010005, 0x0000000A, 0x0003005B, (u32)evt_sub_intpl_get_float, 0xFE363C85, 0xF1194D80,
    0x0005005B, (u32)btlevtcmd_GetScale, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0005005B, (u32)btlevtcmd_SetScale, 0xFFFFFFFE, 0xFE363C80, 0xFE363C85, 0xFE363C82,
    0x00010009, 0x00000001, 0x00000006, 0x0005005B, (u32)evt_sub_intpl_init_float, 0x0000000B,
    0xF24A7C19, 0xF24A7E80, 0x00000006, 0x00010005, 0x00000006, 0x0003005B,
    (u32)evt_sub_intpl_get_float, 0xFE363C85, 0xF1194D80, 0x0005005B, (u32)btlevtcmd_GetScale, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)btlevtcmd_SetScale, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C85, 0xFE363C82, 0x00010009, 0x00000001, 0x00000006,
    0x00000070, 0x0000006E, 0x0005005B, (u32)evt_sub_intpl_init_float, 0x0000000B, 0xF24A7E80,
    0xF24A8080, 0x0000000A, 0x00010005, 0x0000000A, 0x0003005B, (u32)evt_sub_intpl_get_float,
    0xFE363C85, 0xF1194D80, 0x0005005B, (u32)btlevtcmd_GetScale, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0005005B, (u32)btlevtcmd_SetScale, 0xFFFFFFFE, 0xFE363C85,
    0xFE363C81, 0xFE363C82, 0x00010009, 0x00000001, 0x00000006, 0x0005005B,
    (u32)evt_sub_intpl_init_float, 0x0000000B, 0xF24A8080, 0xF24A7E80, 0x00000006, 0x00010005,
    0x00000006, 0x0003005B, (u32)evt_sub_intpl_get_float, 0xFE363C85, 0xF1194D80, 0x0005005B,
    (u32)btlevtcmd_GetScale, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B,
    (u32)btlevtcmd_SetScale, 0xFFFFFFFE, 0xFE363C85, 0xFE363C81, 0xFE363C82, 0x00010009,
    0x00000001, 0x00000006, 0x00000070, 0x0002005B, (u32)btlevtcmd_WaitEventEnd, 0xFE363C8E,
    0x0003005B, (u32)btlevtcmd_ACRGetResult, 0xFE363C86, 0xFE363C87, 0x00020019, 0xFE363C86,
    0x00000002, 0x0002005B, (u32)btlevtcmd_WaitEventEnd, 0xFE363C8F, 0x0001005B, (u32)evt_audience_acrobat_notry,
    0x00010004, 0x0000005F, 0x00000021, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B, (u32)btlevtcmd_FaceDirectionAdd, 0xFFFFFFFE,
    0xFE363C80, 0x00000014, 0x00020035, 0xFE363C81, 0x00000014, 0x0005005B,
    (u32)btlevtcmd_SetRotate, 0xFFFFFFFE, 0x00000000, 0x00000000, 0x00000000, 0x0003005B,
    (u32)btlevtcmd_SetFallAccel, 0xFFFFFFFE, 0xF24A7BB3, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000012, 0xFFFFFFFF, 0x0006005B,
    (u32)btlevtcmd_AudienceDeclareAcrobatResult, 0xFE363C8C, 0x00000001, 0xF2498A80, 0x00000000, 0x00000000,
    0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PWD_Y_1_802f76d4, 0x0001000A,
    0x000001F4, 0x0002005B, (u32)btlevtcmd_WaitEventEnd, 0xFE363C8F, 0x00010003, 0x0000005F,
    0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv,
    0x00000000, 0x00000001, 0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE, 0xF24A9280,
    0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001, 0x00000028, 0x0005005B,
    (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0007005B,
    (u32)btlevtcmd_GetMoveFrame, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0xF24A9280,
    0xFE363C8F, 0x000A005B, (u32)btlevtcmd_DivePosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0xFE363C8F, 0x00000000, 0x00000004, 0x00000000, 0xFFFFFFFF,
    0x00010003, 0x00000063, 0x0001005B, (u32)evt_audience_ap_recovery, 0x0001005B, (u32)btlevtcmd_InviteApInfoReport,
    0x0002005B, (u32)btlevtcmd_ResetFaceDirection, 0xFFFFFFFE, 0x0001005E, (u32)btldefaultevt_SuitoruBadgeEffect, 0x0002005B,
    (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002, 0x00000001
};

u32 partyClaudaAttack_BreathAttack[520] = {
    0x0003005B, (u32)btlevtcmd_GetSelectEnemy, 0xFE363C83, 0xFE363C84, 0x00020018, 0xFE363C83,
    0xFFFFFFFF, 0x00010004, 0x00000063, 0x00000021, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress,
    0xFFFFFFFE, 0xFE363C8C, 0x0002005B, (u32)btlevtcmd_WeaponAftereffect, 0xFE363C8C, 0x0002005B,
    (u32)btlevtcmd_AttackDeclareAll, 0xFFFFFFFE, 0x0001005B, (u32)btlevtcmd_WaitGuardMove, 0x0003005B, (u32)btlevtcmd_PayWeaponCost,
    0xFFFFFFFE, 0xFE363C8C, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFFFFFFFE, 0x00000007,
    0x00010009, 0x00000014, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000007,
    0x0004005B, (u32)evt_btl_camera_set_homing_unit, 0x00000000, 0xFFFFFFFE, 0xFFFFFFFE, 0x0003005B,
    (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001, 0x0003005B, (u32)evt_btl_camera_set_zoom, 0x00000000,
    0x0000012C, 0x0002005B, (u32)btlevtcmd_CommandGetWeaponActionLv, 0xFE363C80, 0x0003005B, (u32)btlevtcmd_AcSetDifficulty,
    0xFFFFFFFE, 0xFE363C80, 0x0002005B, (u32)btlevtcmd_AcGetDifficulty, 0xFE363C81, 0x00020036,
    0xFE363C81, 0x00000003, 0x00010022, 0xFE363C81, 0x00010028, 0xFFFFFFFD,
    0x00020032, 0xFE363C80, 0x0000012C, 0x00010024, 0xFFFFFFFE, 0x00020032,
    0xFE363C80, 0x0000012C, 0x00010024, 0xFFFFFFFF, 0x00020032, 0xFE363C80,
    0x0000012C, 0x00010024, 0x00000000, 0x00020032, 0xFE363C80, 0x0000012C,
    0x00010024, 0x00000001, 0x00020032, 0xFE363C80, 0x0000012C, 0x00010024,
    0x00000002, 0x00020032, 0xFE363C80, 0x0000012C, 0x0000002A, 0x00020032,
    0xFE363C80, 0x0000012C, 0x00000031, 0x0002005B, (u32)btlevtcmd_AcGetDifficulty, 0xFE363C81,
    0x0009005B, (u32)btlevtcmd_AcSetParamAll, 0x00000001, 0xFE363C80, 0x00000004, 0x00000028,
    0x0000003C, 0x00000050, 0x00000064, 0xFE363C81, 0x0005005B, (u32)btlevtcmd_SetupAC,
    0xFFFFFFFE, 0x0000000E, 0x00000001, 0x00000000, 0x0001005B, (u32)_clauda_breath_effect_ready,
    0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PWD_A2_1_802f76dc, 0x0006005B,
    (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_BTL_CLAUD_BREATH_802f76e8, 0xF1194D80, 0x00000000, 0xF1194D80,
    0x0003005B, (u32)btlevtcmd_AnimeWaitPlayComplete, 0xFFFFFFFE, 0x00000001, 0x0004005B, (u32)btlevtcmd_AnimeChangePose,
    0xFFFFFFFE, 0x00000001, (u32)(char*)str_PWD_A2_2_802f7700, 0x0003005B, (u32)evt_btl_camera_nomove_x_onoff, 0x00000000,
    0x00000001, 0x0003005B, (u32)evt_btl_camera_nomove_y_onoff, 0x00000000, 0x00000001, 0x0003005B,
    (u32)evt_btl_camera_nomove_z_onoff, 0x00000000, 0x00000001, 0x0003005B, (u32)btlevtcmd_AnimeWaitPlayComplete, 0xFFFFFFFE,
    0x00000001, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000, 0x0003005B,
    (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001, 0x0003005B, (u32)evt_btl_camera_set_zoom, 0x00000000,
    0x00000000, 0x0003005B, (u32)evt_btl_camera_nomove_x_onoff, 0x00000000, 0x00000000, 0x0003005B,
    (u32)evt_btl_camera_nomove_y_onoff, 0x00000000, 0x00000000, 0x0003005B, (u32)evt_btl_camera_nomove_z_onoff, 0x00000000,
    0x00000000, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PWD_A2_3_802f770c,
    0x0000006B, 0x0001000A, 0x000001F4, 0x0001005B, (u32)btlevtcmd_StageDispellFog, 0x0000006D,
    0x0002005B, (u32)_clauda_breath_effect_fire, 0xFE363C8D, 0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE,
    (u32)(char*)str_SFX_BTL_CLAUD_BREATH_802f7718, 0xF1194D80, 0x00000000, 0xFE363C8E, 0x0001006F, 0xFE363C8F,
    0x00020032, 0xFE363C8E, 0xFFFFFFFF, 0x00010003, 0x00000005, 0x0003005B,
    (u32)btlevtcmd_AcGetOutputParam, 0x00000001, 0xFE363C80, 0x00010009, 0x00000001, 0x00010004,
    0x00000005, 0x00000070, 0x0003005B, (u32)btlevtcmd_GetEnemyBelong, 0xFFFFFFFE, 0xFE363C80,
    0x0004005B, (u32)btlevtcmd_SamplingEnemy, 0xFFFFFFFE, 0xFE363C80, 0xFE363C8C, 0x0003005B,
    (u32)btlevtcmd_GetSelectEnemy, 0xFE363C83, 0xFE363C84, 0x00010003, 0x00000008, 0x00020019,
    0xFE363C83, 0xFFFFFFFF, 0x0003005B, (u32)_check_blow_rate, 0xFE363C83, 0xFE363C80,
    0x0002001D, 0xFE363C80, 0x00000001, 0x0007005B, (u32)btlevtcmd_DamageDirect, 0xFE363C83,
    0xFE363C84, 0x00000000, 0x00000000, 0x00000003, 0x00000001, 0x00000021,
    0x0003005B, (u32)btlevtcmd_GetSelectNextEnemy, 0xFE363C83, 0xFE363C84, 0x00010004, 0x00000008,
    0x00000021, 0x0002005B, (u32)btlevtcmd_StartAC, 0x00000001, 0x0001005B, (u32)btlevtcmd_ResultAC,
    0x0001005F, 0xFE363C8F, 0x0005005B, (u32)btlevtcmd_SetRotate, 0xFFFFFFFE, 0x00000000,
    0x00000000, 0x00000000, 0x0001005B, (u32)btlevtcmd_StopAC, 0x00020032, 0xFE363C8B,
    0x00000063, 0x00020032, 0xFE363C8C, (u32)partyWeapon_ClaudaBreathAttack, 0x0003005B, (u32)btlevtcmd_AcGetOutputParam,
    0x00000002, 0xFE363C80, 0x0003005B, (u32)_make_breath_weapon, 0xFE363C8C, 0xFE363C80,
    0x00010009, 0x0000003C, 0x0003005B, (u32)btlevtcmd_GetEnemyBelong, 0xFFFFFFFE, 0xFE363C80,
    0x0004005B, (u32)btlevtcmd_SamplingEnemy, 0xFFFFFFFE, 0xFE363C80, 0xFE363C8C, 0x0003005B,
    (u32)btlevtcmd_GetSelectEnemy, 0xFE363C83, 0xFE363C84, 0x00020032, 0xFE363C8A, 0x00000000,
    0x00010003, 0x0000000A, 0x0005005B, (u32)btlevtcmd_SetDispOffset, 0xFE363C83, 0x00000000,
    0x00000000, 0x00000000, 0x0007005B, (u32)btlevtcmd_PreCheckDamage, 0xFFFFFFFE, 0xFE363C83,
    0xFE363C84, 0xFE363C8C, 0x00000100, 0xFE363C86, 0x00020019, 0xFE363C86,
    0x00000001, 0x00020018, 0xFE363C86, 0x00000003, 0x0003005B, (u32)btlevtcmd_StartAvoid,
    0xFE363C83, 0x00000026, 0x00010004, 0x00000033, 0x00000021, 0x00020018,
    0xFE363C86, 0x00000006, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000027,
    0x00010004, 0x00000033, 0x00000021, 0x00020018, 0xFE363C86, 0x00000002,
    0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000028, 0x00010004, 0x00000033,
    0x00000021, 0x00010004, 0x00000032, 0x00000021, 0x0002005B, (u32)btlevtcmd_GetResultAC,
    0xFE363C80, 0x0002001E, 0xFE363C80, 0x00000002, 0x0007005B, (u32)btlevtcmd_CheckDamage,
    0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0xFE363C8C, 0x00020100, 0xFE363C85,
    0x0003005B, (u32)btlevtcmd_GetDamageCode, 0xFE363C83, 0xFE363C80, 0x00020018, 0xFE363C80,
    0x0000001D, 0x0004005B, (u32)btlevtcmd_GetResultPrizeLv, 0xFE363C83, 0x00000000, 0xFE363C86,
    0x0006005B, (u32)btlevtcmd_GetHitPos, 0xFE363C83, 0xFE363C84, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0005005B, (u32)btlevtcmd_ACSuccessEffect, 0xFE363C86, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult, 0xFE363C8C, 0xFE363C86, 0x00000021,
    0x00000020, 0x0007005B, (u32)btlevtcmd_CheckDamage, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84,
    0xFE363C8C, 0x00000100, 0xFE363C85, 0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult, 0xFE363C8C,
    0xFFFFFFFF, 0x00000021, 0x00010003, 0x00000032, 0x0001000A, 0x0000012C,
    0x00010003, 0x00000033, 0x00020036, 0xFE363C8B, 0x00000001, 0x0002001D,
    0xFE363C8B, 0x00000001, 0x00010003, 0x00000034, 0x0003005B, (u32)btlevtcmd_GetSelectNextEnemy,
    0xFE363C83, 0xFE363C84, 0x00020019, 0xFE363C83, 0xFFFFFFFF, 0x00020035,
    0xFE363C8A, 0x00000001, 0x00010004, 0x0000000A, 0x00000021, 0x00000021,
    0x00010003, 0x00000050, 0x0001000A, 0x000005DC, 0x00010003, 0x0000005A,
    0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv,
    0x00000000, 0x00000001, 0x0002005B, (u32)evt_snd_sfxoff, 0xFE363C8E, 0x0002005B,
    (u32)evt_eff_delete_ptr, 0xFE363C8D, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001,
    0x0000002B, 0x0006005B, (u32)btlevtcmd_ACRStart, 0xFFFFFFFE, 0x00000000, 0x00000019,
    0x00000019, 0x00000014, 0x0003005B, (u32)btlevtcmd_ACRGetResult, 0xFE363C86, 0xFE363C87,
    0x00020019, 0xFE363C86, 0x00000002, 0x0001005B, (u32)evt_audience_acrobat_notry, 0x0001000A,
    0x000001F4, 0x00010004, 0x00000063, 0x00000021, 0x0006005B, (u32)btlevtcmd_AudienceDeclareAcrobatResult,
    0xFE363C8C, 0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PWD_Y_1_802f76d4, 0x0001000A, 0x000003E8,
    0x00010003, 0x00000063, 0x0001005B, (u32)evt_audience_ap_recovery, 0x0001005B, (u32)btlevtcmd_InviteApInfoReport,
    0x0002005B, (u32)btlevtcmd_ResetFaceDirection, 0xFFFFFFFE, 0x0001005E, (u32)btldefaultevt_SuitoruBadgeEffect, 0x0002005B,
    (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002, 0x00000001
};

u32 partyClaudaAttack_PredationAttack[875] = {
    0x0005005B, (u32)btlevtcmd_JumpSetting, 0xFFFFFFFE, 0x00000014, 0xF24A7A80, 0xF24A7D4C,
    0x0003005B, (u32)btlevtcmd_GetSelectEnemy, 0xFE363C83, 0xFE363C84, 0x00020018, 0xFE363C83,
    0xFFFFFFFF, 0x00010004, 0x00000063, 0x00000021, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress,
    0xFFFFFFFE, 0xFE363C8C, 0x0002005B, (u32)btlevtcmd_WeaponAftereffect, 0xFE363C8C, 0x0004005B,
    (u32)btlevtcmd_AttackDeclare, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0x0001005B, (u32)btlevtcmd_WaitGuardMove,
    0x0002005B, (u32)btlevtcmd_CommandPayWeaponCost, 0xFFFFFFFE, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFFFFFFFE,
    0x00000007, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x0000000B, 0x0006005B,
    (u32)evt_btl_camera_set_homing_unitparts, 0x00000000, 0xFFFFFFFE, 0x00000001, 0xFE363C83, 0xFE363C84,
    0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001, 0x0003005B, (u32)evt_btl_camera_set_zoom,
    0x00000000, 0x0000012C, 0x0002005B, (u32)btlevtcmd_CommandGetWeaponActionLv, 0xFE363C80, 0x0003005B,
    (u32)btlevtcmd_AcSetDifficulty, 0xFFFFFFFE, 0xFE363C80, 0x0003005B, (u32)btlevtcmd_GetPartyTechLv, 0xFFFFFFFE,
    0xFE363C80, 0x00010022, 0xFE363C80, 0x00010024, 0x00000002, 0x0002005B,
    (u32)btlevtcmd_AcGetDifficulty, 0xFE363C80, 0x00020036, 0xFE363C80, 0x00000003, 0x00010022,
    0xFE363C80, 0x00010028, 0xFFFFFFFD, 0x00020032, 0xFE363C80, 0x00000078,
    0x00020032, 0xFE363C81, 0x00000000, 0x00010024, 0xFFFFFFFE, 0x00020032,
    0xFE363C80, 0x00000064, 0x00020032, 0xFE363C81, 0x00000000, 0x00010024,
    0xFFFFFFFF, 0x00020032, 0xFE363C80, 0x00000050, 0x00020032, 0xFE363C81,
    0x00000001, 0x00010024, 0x00000000, 0x00020032, 0xFE363C80, 0x00000046,
    0x00020032, 0xFE363C81, 0x00000002, 0x00010024, 0x00000001, 0x00020032,
    0xFE363C80, 0x00000042, 0x00020032, 0xFE363C81, 0x00000002, 0x00010024,
    0x00000002, 0x00020032, 0xFE363C80, 0x0000003E, 0x00020032, 0xFE363C81,
    0x00000002, 0x0000002A, 0x00020032, 0xFE363C80, 0x0000003A, 0x00020032,
    0xFE363C81, 0x00000002, 0x00000031, 0x0009005B, (u32)btlevtcmd_AcSetParamAll, 0x00000000,
    0xFE363C80, 0xFE363C81, 0x00000005, 0x00000003, 0xF1194D80, 0xF1194D80,
    0xF1194D80, 0x0005005B, (u32)btlevtcmd_AcSetGaugeParam, 0x0000003C, 0x00000050, 0x00000064,
    0x00000064, 0x0000002A, 0x0002005B, (u32)btlevtcmd_AcGetDifficulty, 0xFE363C80, 0x00020036,
    0xFE363C80, 0x00000003, 0x00010022, 0xFE363C80, 0x00010028, 0xFFFFFFFD,
    0x00020032, 0xFE363C80, 0x00000078, 0x00020032, 0xFE363C81, 0x00000000,
    0x00010024, 0xFFFFFFFE, 0x00020032, 0xFE363C80, 0x00000064, 0x00020032,
    0xFE363C81, 0x00000000, 0x00010024, 0xFFFFFFFF, 0x00020032, 0xFE363C80,
    0x00000050, 0x00020032, 0xFE363C81, 0x00000001, 0x00010024, 0x00000000,
    0x00020032, 0xFE363C80, 0x00000046, 0x00020032, 0xFE363C81, 0x00000002,
    0x00010024, 0x00000001, 0x00020032, 0xFE363C80, 0x00000042, 0x00020032,
    0xFE363C81, 0x00000002, 0x00010024, 0x00000002, 0x00020032, 0xFE363C80,
    0x0000003E, 0x00020032, 0xFE363C81, 0x00000002, 0x0000002A, 0x00020032,
    0xFE363C80, 0x0000003A, 0x00020032, 0xFE363C81, 0x00000002, 0x00000031,
    0x0009005B, (u32)btlevtcmd_AcSetParamAll, 0x00000000, 0xFE363C80, 0xFE363C81, 0x00000003,
    0x00000002, 0xF1194D80, 0xF1194D80, 0xF1194D80, 0x0005005B, (u32)btlevtcmd_AcSetGaugeParam,
    0x00000022, 0x00000044, 0x00000064, 0x00000064, 0x00000031, 0x0002005B,
    (u32)btlevtcmd_AcSetFlag, 0x00000000, 0x0005005B, (u32)btlevtcmd_SetupAC, 0xFFFFFFFE, 0x00000011,
    0x00000001, 0x0000003C, 0x00010009, 0x00000016, 0x0006005B, (u32)btlevtcmd_CommandPreCheckDamage,
    0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0x00000100, 0xFE363C85, 0x00020018,
    0xFE363C85, 0x00000005, 0x0003005B, (u32)btlevtcmd_GetStatusMg, 0xFE363C83, 0xFE363C86,
    0x0002003C, 0xFE363C86, 0x00000028, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFE363C83,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B, (u32)btlevtcmd_TransStageFloorPosition, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0004005B, (u32)btlevtcmd_FaceDirectionAdd, 0xFE363C83, 0xFE363C80,
    0xFE363C86, 0x0007005B, (u32)btlevtcmd_CalculateFaceDirection, 0xFFFFFFFE, 0xFFFFFFFF, 0xFE363C80,
    0xFE363C82, 0x00000001, 0xFE363C8F, 0x0003005B, (u32)btlevtcmd_ChangeFaceDirection, 0xFFFFFFFE,
    0xFE363C8F, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001, 0x00000028,
    0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE, 0xF24A8680, 0x000A005B, (u32)btlevtcmd_DivePosition,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000028, 0x00000000,
    0x00000005, 0x00000000, 0xFFFFFFFF, 0x0006005B, (u32)btlevtcmd_CommandCheckDamage, 0xFFFFFFFE,
    0xFE363C83, 0xFE363C84, 0x00000100, 0xFE363C85, 0x00000021, 0x0007005B,
    (u32)_get_clauda_kiss_hit_position, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0003005B, (u32)btlevtcmd_GetStatusMg, 0xFFFFFFFE, 0xFE363C86, 0x0002003C,
    0xFE363C86, 0x00000014, 0x0004005B, (u32)btlevtcmd_FaceDirectionAdd, 0xFE363C83, 0xFE363C80,
    0xFE363C86, 0x0007005B, (u32)btlevtcmd_CalculateFaceDirection, 0xFFFFFFFE, 0xFFFFFFFF, 0xFE363C80,
    0xFE363C82, 0x00000001, 0xFE363C8F, 0x0003005B, (u32)btlevtcmd_ChangeFaceDirection, 0xFFFFFFFE,
    0xFE363C8F, 0x00020035, 0xFE363C82, 0x00000005, 0x000A005B, (u32)btlevtcmd_DivePosition,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000028, 0x00000000,
    0x00000005, 0x00000000, 0xFFFFFFFF, 0x0007005B, (u32)btlevtcmd_CalculateFaceDirection, 0xFFFFFFFE,
    0xFFFFFFFF, 0xFE363C83, 0xFE363C84, 0x00000010, 0xFE363C8F, 0x0003005B,
    (u32)btlevtcmd_ChangeFaceDirection, 0xFFFFFFFE, 0xFE363C8F, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE,
    0x00000001, (u32)(char*)str_PWD_A3_1_802f7730, 0x0003005B, (u32)btlevtcmd_AnimeWaitPlayComplete, 0xFFFFFFFE, 0x00000001,
    0x0006005B, (u32)btlevtcmd_CommandPreCheckDamage, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0x00000100,
    0xFE363C85, 0x00010022, 0xFE363C85, 0x0001002B, 0x00000002, 0x0001002B,
    0x00000003, 0x0001002B, 0x00000006, 0x0001002B, 0x00000004, 0x0000002E,
    0x00010024, 0x00000001, 0x0000002A, 0x00000031, 0x00020019, 0xFE363C85,
    0x00000001, 0x00020018, 0xFE363C85, 0x00000003, 0x0003005B, (u32)btlevtcmd_StartAvoid,
    0xFE363C83, 0x00000026, 0x00000021, 0x00020018, 0xFE363C85, 0x00000006,
    0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000027, 0x00000021, 0x00020018,
    0xFE363C85, 0x00000002, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000028,
    0x00000021, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PWD_A3_2_802f773c,
    0x0003005B, (u32)btlevtcmd_AnimeWaitPlayComplete, 0xFFFFFFFE, 0x00000001, 0x00010004, 0x0000005A,
    0x00000021, 0x0006005B, (u32)btlevtcmd_CommandPreCheckCounter, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84,
    0x00000100, 0xFE363C80, 0x00010022, 0xFE363C80, 0x0001002B, 0x00000007,
    0x0001002B, 0x00000008, 0x0001002B, 0x00000009, 0x0001002B, 0x0000000A,
    0x0001002B, 0x0000000B, 0x0001002B, 0x0000000C, 0x0001002B, 0x0000000D,
    0x0001002B, 0x0000000E, 0x0001002B, 0x0000000F, 0x0004005B, (u32)btlevtcmd_AnimeChangePose,
    0xFFFFFFFE, 0x00000001, (u32)(char*)str_PWD_A3_2_802f773c, 0x0006005B, (u32)btlevtcmd_CommandCheckDamage, 0xFFFFFFFE,
    0xFE363C83, 0xFE363C84, 0x00000100, 0xFE363C85, 0x0000002E, 0x00000031,
    0x0003005B, (u32)btlevtcmd_OffUnitFlag, 0xFFFFFFFE, 0x00200000, 0x0006005B, (u32)btlevtcmd_snd_se,
    0xFFFFFFFE, (u32)(char*)str_SFX_BTL_CLAUD_KISS1_802f7748, 0xF1194D80, 0x00000000, 0xFE363C8E, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PWD_A3_2_802f773c, 0x0006005B, (u32)btlevtcmd_GetHitPos,
    0xFE363C83, 0xFE363C84, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000F005B,
    (u32)evt_eff, (u32)(u8*)zero_80424060, (u32)(char*)str_kiss_80424064, 0x00000002, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0xFE363C86, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x0007005B, (u32)btlevtcmd_DamageDirect, 0xFE363C83,
    0xFE363C84, 0x00000000, 0x00000000, 0x00000002, 0x00000001, 0x0002005B,
    (u32)btlevtcmd_StartAC, 0x00000001, 0x0001006F, 0xFE363C8F, 0x0006005B, (u32)btlevtcmd_SetRGB,
    0xFE363C83, 0xFE363C84, 0x000000FF, 0x000000FF, 0x000000FF, 0x00010005,
    0x0000002D, 0x0006005B, (u32)btlevtcmd_GetRGB, 0xFE363C83, 0xFE363C84, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00020036, 0xFE363C81, 0x00000002, 0x0002001A,
    0xFE363C81, 0x000000BE, 0x00020032, 0xFE363C81, 0x000000BE, 0x00000021,
    0x00020036, 0xFE363C82, 0x00000004, 0x0006005B, (u32)btlevtcmd_SetRGB, 0xFE363C83,
    0xFE363C84, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00010009, 0x00000001,
    0x00000006, 0x00010005, 0x0000002D, 0x0006005B, (u32)btlevtcmd_GetRGB, 0xFE363C83,
    0xFE363C84, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00020036, 0xFE363C80,
    0x00000002, 0x00020036, 0xFE363C81, 0x00000002, 0x00020035, 0xFE363C82,
    0x00000003, 0x0002001A, 0xFE363C81, 0x00000000, 0x00020032, 0xFE363C81,
    0x00000000, 0x00000021, 0x0006005B, (u32)btlevtcmd_SetRGB, 0xFE363C83, 0xFE363C84,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00010009, 0x00000001, 0x00000006,
    0x00000070, 0x0001005B, (u32)btlevtcmd_ResultAC, 0x0002005B, (u32)evt_snd_sfxoff, 0xFE363C8E,
    0x0001005F, 0xFE363C8F, 0x0002005B, (u32)btlevtcmd_GetResultAC, 0xFE363C89, 0x0002001E,
    0xFE363C89, 0x00000002, 0x0003005B, (u32)btlevtcmd_GetPartyTechLv, 0xFFFFFFFE, 0xFE363C80,
    0x00010022, 0xFE363C80, 0x00010024, 0x00000002, 0x00020032, 0xFE363C87,
    0x00000003, 0x0000002A, 0x00020032, 0xFE363C87, 0x00000002, 0x00000031,
    0x0003005B, (u32)btlevtcmd_AcGetOutputParam, 0x00000000, 0xFE363C88, 0x0002001D, 0xFE363C88,
    0xFE363C87, 0x00020036, 0xFE363C88, 0xFE363C87, 0x0004005B, (u32)btlevtcmd_GetResultPrizeLv,
    0xFE363C83, 0xFE363C88, 0xFE363C86, 0x0006005B, (u32)btlevtcmd_GetHitPos, 0xFE363C83,
    0xFE363C84, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)btlevtcmd_ACSuccessEffect,
    0xFE363C86, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult,
    0xFE363C8C, 0xFE363C86, 0x00000020, 0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult, 0xFE363C8C,
    0xFFFFFFFF, 0x00000021, 0x00000020, 0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult, 0xFE363C8C,
    0xFFFFFFFF, 0x00000021, 0x0003005B, (u32)btlevtcmd_OnUnitFlag, 0xFFFFFFFE, 0x00200000,
    0x0006005B, (u32)btlevtcmd_SetRGB, 0xFE363C83, 0xFE363C84, 0x000000FF, 0x000000FF,
    0x000000FF, 0x0003005B, (u32)btlevtcmd_AcGetOutputParam, 0x00000000, 0xFE363C80, 0x0003005B,
    (u32)_make_kiss_weapon, 0xFE363C8C, 0xFE363C80, 0x0002001E, 0xFE363C89, 0x00000002,
    0x0007005B, (u32)btlevtcmd_CheckDamage, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0xFE363C8C,
    0x00020100, 0xFE363C85, 0x00000020, 0x0007005B, (u32)btlevtcmd_CheckDamage, 0xFFFFFFFE,
    0xFE363C83, 0xFE363C84, 0xFE363C8C, 0x00000100, 0xFE363C85, 0x00000021,
    0x0000006E, 0x0003005B, (u32)btlevtcmd_GetHpDamage, 0xFE363C83, 0xFE363C80, 0x00010009,
    0x00000032, 0x0002001D, 0xFE363C80, 0x00000001, 0x0005005B, (u32)btlevtcmd_GetPos,
    0xFFFFFFFE, 0xFE363C85, 0xFE363C86, 0xFE363C87, 0x0003005B, (u32)btlevtcmd_GetStatusMg,
    0xFFFFFFFE, 0xFE363C88, 0x0002003C, 0xFE363C88, 0x00000028, 0x00020035,
    0xFE363C86, 0xFE363C88, 0x0004005B, (u32)btlevtcmd_RecoverHp, 0xFFFFFFFE, 0x00000001,
    0xFE363C80, 0x000F005B, (u32)evt_eff, (u32)(u8*)zero_80424060, (u32)(char*)str_recovery_802f775c, 0x00000007,
    0xFE363C85, 0xFE363C86, 0xFE363C87, 0xFE363C80, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000021,
    0x00000070, 0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_BTL_CLAUD_KISS2_802f7768, 0xF1194D80,
    0x00000000, 0xF1194D80, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001,
    (u32)(char*)str_PWD_A3_3_802f777c, 0x00010009, 0x00000005, 0x0006005B, (u32)btlevtcmd_ACRStart, 0xFFFFFFFE,
    0x00000000, 0x0000001D, 0x0000001D, 0x00000000, 0x0003005B, (u32)btlevtcmd_ACRGetResult,
    0xFE363C86, 0xFE363C87, 0x00020018, 0xFE363C86, 0x00000002, 0x0006005B,
    (u32)btlevtcmd_AudienceDeclareAcrobatResult, 0xFE363C8C, 0x00000001, 0x00000000, 0x00000000, 0x00000000,
    0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PWD_A_7_802f7788, 0x00010009,
    0x00000038, 0x00000020, 0x0001005B, (u32)evt_audience_acrobat_notry, 0x00010009, 0x0000003C,
    0x00000021, 0x00010003, 0x0000005A, 0x0001005B, (u32)btlevtcmd_StopAC, 0x0001005B,
    (u32)evt_audience_ap_recovery, 0x0001005B, (u32)btlevtcmd_InviteApInfoReport, 0x00010003, 0x0000005F, 0x0003005B,
    (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000,
    0x00000001, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001, 0x0000002A,
    0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x000A005B, (u32)btlevtcmd_DivePosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00000028, 0x00000000, 0x00000005, 0x00000000, 0xFFFFFFFF, 0x00010003,
    0x00000063, 0x0002005B, (u32)btlevtcmd_ResetFaceDirection, 0xFFFFFFFE, 0x0001005E, (u32)btldefaultevt_SuitoruBadgeEffect,
    0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002, 0x00000001
};

u32 partyClaudaAttack_KumoGuard[290] = {
    0x0003005B, (u32)btlevtcmd_GetSelectEnemy, 0xFE363C83, 0xFE363C84, 0x00020018, 0xFE363C83,
    0xFFFFFFFF, 0x00010004, 0x00000063, 0x00000021, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress,
    0xFFFFFFFE, 0xFE363C8C, 0x0002005B, (u32)btlevtcmd_WeaponAftereffect, 0xFE363C8C, 0x0002005B,
    (u32)btlevtcmd_AttackDeclareAll, 0xFFFFFFFE, 0x0001005B, (u32)btlevtcmd_WaitGuardMove, 0x0002005B, (u32)btlevtcmd_CommandPayWeaponCost,
    0xFFFFFFFE, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFFFFFFFE, 0x00000007, 0x0007005B,
    (u32)btlevtcmd_CalculateFaceDirection, 0xFFFFFFFE, 0xFFFFFFFF, 0xFE363C83, 0xFE363C84, 0x00000010,
    0xFE363C8F, 0x0003005B, (u32)btlevtcmd_ChangeFaceDirection, 0xFFFFFFFE, 0xFE363C8F, 0x0005005B,
    (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xF1194D80, 0xF1194D80, 0x0005005B,
    (u32)btlevtcmd_GetPos, 0xFE363C83, 0xFE363C81, 0xF1194D80, 0xF1194D80, 0x00020035,
    0xFE363C80, 0xFE363C81, 0x00020038, 0xFE363C80, 0x00000002, 0x0003005B,
    (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000003, 0x000A005B, (u32)evt_btl_camera_set_moveto, 0x00000001,
    0xFE363C80, 0x00000048, 0x00000190, 0xFE363C80, 0x0000002A, 0x00000000,
    0x0000001E, 0x00000005, 0x0002005B, (u32)btlevtcmd_CommandGetWeaponActionLv, 0xFE363C80, 0x0003005B,
    (u32)btlevtcmd_AcSetDifficulty, 0xFFFFFFFE, 0xFE363C80, 0x0009005B, (u32)btlevtcmd_AcSetParamAll, 0x00000003,
    0x00000000, 0x00000028, 0x00000000, 0x00000001, 0x00000001, 0x00000002,
    0x00000008, 0x0002005B, (u32)btlevtcmd_AcSetFlag, 0x00000005, 0x0005005B, (u32)btlevtcmd_SetupAC,
    0xFFFFFFFE, 0x00000012, 0x00000001, 0x00000000, 0x00010009, 0x00000001,
    0x00010009, 0x0000001E, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001,
    (u32)(char*)str_PWD_A4_2_802f7790, 0x0002005B, (u32)btlevtcmd_StartAC, 0x00000001, 0x0001005B, (u32)btlevtcmd_ResultAC,
    0x00010009, 0x0000003C, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001,
    (u32)(char*)str_PWD_A4_3_802f758c, 0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_BTL_CLAUD_HIDE2_802f779c, 0xF1194D80,
    0x00000000, 0xF1194D80, 0x0001000A, 0x00000096, 0x0002005B, (u32)btlevtcmd_GetResultAC,
    0xFE363C86, 0x0001005B, (u32)btlevtcmd_StopAC, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFE363C83,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0003005B, (u32)btlevtcmd_GetHeight, 0xFE363C83,
    0xFE363C87, 0x00020038, 0xFE363C87, 0x00000002, 0x00020035, 0xFE363C81,
    0xFE363C87, 0x00020035, 0xFE363C82, 0x00000014, 0x000F005B, (u32)evt_eff,
    (u32)(u8*)zero_80424060, (u32)(char*)str_kemuri_test_802f77b0, 0x00000008, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0xF24A7E80, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x0003005B, (u32)btlevtcmd_AcGetOutputParam, 0x00000000, 0xFE363C80,
    0x00020035, 0xFE363C80, 0x00000001, 0x0003005B, (u32)_make_kumoguard_weapon, 0xFE363C8C,
    0xFE363C80, 0x0003005B, (u32)btlevtcmd_AcGetOutputParam, 0x00000000, 0xFE363C80, 0x0002001D,
    0xFE363C80, 0x00000001, 0x0007005B, (u32)btlevtcmd_CheckDamage, 0xFFFFFFFE, 0xFE363C83,
    0xFE363C84, 0xFE363C8C, 0x00020100, 0xFE363C85, 0x00020036, 0xFE363C80,
    0x00000001, 0x0004005B, (u32)btlevtcmd_GetResultPrizeLv, 0xFFFFFFFE, 0xFE363C80, 0xFE363C86,
    0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0005005B, (u32)btlevtcmd_ACSuccessEffect, 0xFE363C86, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult, 0xFE363C8C, 0xFE363C86, 0x00000020, 0x0007005B,
    (u32)btlevtcmd_CheckDamage, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0xFE363C8C, 0x00000100,
    0xFE363C85, 0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult, 0xFE363C8C, 0xFFFFFFFF, 0x00000021,
    0x0006005B, (u32)btlevtcmd_ACRStart, 0xFFFFFFFE, 0x00000000, 0x0000001D, 0x0000001D,
    0x00000014, 0x0003005B, (u32)btlevtcmd_ACRGetResult, 0xFE363C86, 0xFE363C87, 0x00020018,
    0xFE363C86, 0x00000002, 0x0006005B, (u32)btlevtcmd_AudienceDeclareAcrobatResult, 0xFE363C8C, 0x00000001,
    0x00000000, 0x00000000, 0x00000000, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE,
    0x00000001, (u32)(char*)str_PWD_A_7_802f7788, 0x0001000A, 0x000003E8, 0x00000020, 0x0001005B,
    (u32)evt_audience_acrobat_notry, 0x0001000A, 0x000003E8, 0x00000021, 0x00010003, 0x00000063,
    0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv,
    0x00000000, 0x00000001, 0x0001005B, (u32)evt_audience_ap_recovery, 0x0001005B, (u32)btlevtcmd_InviteApInfoReport,
    0x0002005B, (u32)btlevtcmd_ResetFaceDirection, 0xFFFFFFFE, 0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE,
    0x00000002, 0x00000001
};

u32 clauda_appeal_event[71] = {
    0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFFFFFFFE, 0x00000007, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType,
    0xFFFFFFFE, 0x00000001, 0x00000042, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_MARIO_APPEAL1_802f77bc,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000, 0x0005005B, (u32)btlevtcmd_GetPos,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B, (u32)btlevtcmd_FaceDirectionAdd,
    0xFFFFFFFE, 0xFE363C80, 0x0000000F, 0x00020035, 0xFE363C81, 0x0000001E,
    0x0003005B, (u32)btlevtcmd_GetFaceDirection, 0xFFFFFFFE, 0xFE363C8E, 0x00020037, 0xFE363C8E,
    0x0000002D, 0x000F005B, (u32)evt_eff, (u32)(u8*)zero_80424060, (u32)(char*)str_kiss_80424064, 0x00000002,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0xFE363C8E, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00010009,
    0x0000001E, 0x0002005B, (u32)btlevtcmd_AudienceAppeal, 0xFFFFFFFE, 0x00010009, 0x0000001E,
    0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002, 0x00000001
};

static u32 attack_audience_event[15] = {
    0x0002005B, (u32)_check_aud_item_type, 0xFE363C80, 0x00010022, 0xFE363C80, 0x00010024,
    0x00000001, 0x0001005E, (u32)attack_bad_audience_event, 0x0000002A, 0x0001005E, (u32)attack_good_audience_event,
    0x00000031, 0x00000002, 0x00000001
};

u32 attack_bad_audience_event[348] = {
    0x0003005B, (u32)btlevtcmd_OffUnitFlag, 0xFFFFFFFE, 0x00200000, 0x0004005B, (u32)btlevtcmd_AnimeChangePose,
    0xFFFFFFFE, 0x00000001, (u32)(char*)str_PWD_A1_1_802f7678, 0x0005005B, (u32)_get_attack_aud_no, 0xFE363C83,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0000006E, 0x00010009, 0x0000000F,
    0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x0000000D, 0x0005005B, (u32)evt_btl_camera_set_homing_unit_audience,
    0x00000000, 0xFFFFFFFE, 0x00000001, 0xFE363C83, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv,
    0x00000000, 0x00000002, 0x0003005B, (u32)evt_btl_camera_set_zoom, 0x00000000, 0x000000C8,
    0x00000070, 0x00020032, 0xFE363C85, 0xFE363C81, 0x00020035, 0xFE363C81,
    0x0000001C, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE, 0xFE363C80, 0x00000005,
    0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE, 0xF24A9280, 0x0003005B, (u32)btlevtcmd_SetFallAccel,
    0xFFFFFFFE, 0xF24A7CE6, 0x00020032, 0xFE363C86, 0x00000024, 0x0006005B,
    (u32)btlevtcmd_MarioJumpParam, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0xFE363C86,
    0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PWD_A1_1_802f7678, 0x0006005B,
    (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_BTL_CLAUD_ATTACK_802f7684, 0xF1194D80, 0x00000000, 0xF1194D80,
    0x0000006E, 0x00020038, 0xFE363C86, 0x00000002, 0x00010009, 0xFE363C86,
    0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PWD_A1_2_802f769c, 0x0006005B,
    (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_BTL_CLAUD_ATTACK_802f7684, 0xF1194D80, 0x00000000, 0xF1194D80,
    0x00000070, 0x0008005B, (u32)btlevtcmd_MarioJumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0xFE363C86, 0x00000000, 0x00000000, 0x0003005B, (u32)_attack_aud,
    0xFE363C83, 0x00000000, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000003,
    0x0002005B, (u32)btlevtcmd_JumpContinue, 0xFFFFFFFE, 0x0006005B, (u32)evt_btl_camera_shake_h, 0x00000000,
    0x00000008, 0x00000000, 0x0000001E, 0x0000000D, 0x0001006F, 0xFE363C8F,
    0x0001000A, 0x000005DC, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000,
    0x0001000A, 0x000003E8, 0x00000070, 0x0001006F, 0xFE363C8E, 0x0005005B,
    (u32)evt_sub_intpl_init_float, 0x0000000B, 0xF24A7E80, 0xF24A7C19, 0x0000000A, 0x00010005,
    0x0000000A, 0x0003005B, (u32)evt_sub_intpl_get_float, 0xFE363C85, 0xF1194D80, 0x0005005B,
    (u32)btlevtcmd_GetScale, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B,
    (u32)btlevtcmd_SetScale, 0xFFFFFFFE, 0xFE363C80, 0xFE363C85, 0xFE363C82, 0x00010009,
    0x00000001, 0x00000006, 0x0005005B, (u32)evt_sub_intpl_init_float, 0x0000000B, 0xF24A7C19,
    0xF24A7E80, 0x00000006, 0x00010005, 0x00000006, 0x0003005B, (u32)evt_sub_intpl_get_float,
    0xFE363C85, 0xF1194D80, 0x0005005B, (u32)btlevtcmd_GetScale, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0005005B, (u32)btlevtcmd_SetScale, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C85, 0xFE363C82, 0x00010009, 0x00000001, 0x00000006, 0x00000070,
    0x0000006E, 0x0005005B, (u32)evt_sub_intpl_init_float, 0x0000000B, 0xF24A7E80, 0xF24A8080,
    0x0000000A, 0x00010005, 0x0000000A, 0x0003005B, (u32)evt_sub_intpl_get_float, 0xFE363C85,
    0xF1194D80, 0x0005005B, (u32)btlevtcmd_GetScale, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0005005B, (u32)btlevtcmd_SetScale, 0xFFFFFFFE, 0xFE363C85, 0xFE363C81,
    0xFE363C82, 0x00010009, 0x00000001, 0x00000006, 0x0005005B, (u32)evt_sub_intpl_init_float,
    0x0000000B, 0xF24A8080, 0xF24A7E80, 0x00000006, 0x00010005, 0x00000006,
    0x0003005B, (u32)evt_sub_intpl_get_float, 0xFE363C85, 0xF1194D80, 0x0005005B, (u32)btlevtcmd_GetScale,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)btlevtcmd_SetScale,
    0xFFFFFFFE, 0xFE363C85, 0xFE363C81, 0xFE363C82, 0x00010009, 0x00000001,
    0x00000006, 0x00000070, 0x0002005B, (u32)btlevtcmd_WaitEventEnd, 0xFE363C8E, 0x0005005B,
    (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B,
    (u32)btlevtcmd_FaceDirectionAdd, 0xFFFFFFFE, 0xFE363C80, 0x00000014, 0x00020035, 0xFE363C81,
    0x00000014, 0x0005005B, (u32)btlevtcmd_SetRotate, 0xFFFFFFFE, 0x00000000, 0x00000000,
    0x00000000, 0x0003005B, (u32)btlevtcmd_SetFallAccel, 0xFFFFFFFE, 0xF24A7BB3, 0x0007005B,
    (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000012,
    0xFFFFFFFF, 0x0003005B, (u32)btlevtcmd_OnUnitFlag, 0xFFFFFFFE, 0x00200000, 0x0001000A,
    0x00000190, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001, 0x00000028,
    0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x000A005B, (u32)btlevtcmd_DivePosition, 0xFFFFFFFE, 0xFE363C80, 0x00000000, 0xFE363C82,
    0x00000010, 0x00000000, 0x00000004, 0x00000000, 0x00000000, 0x0003005B,
    (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE, 0xF24A9280, 0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0007005B, (u32)btlevtcmd_GetMoveFrame, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0xF24A9280, 0xFE363C8F, 0x00020035,
    0xFE363C8F, 0x0000001E, 0x000A005B, (u32)btlevtcmd_DivePosition, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0xFE363C8F, 0x00000014, 0x00000004, 0x00000000,
    0xFFFFFFFF, 0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002, 0x00000001
};

u32 attack_good_audience_event[216] = {
    0x0003005B, (u32)btlevtcmd_OffUnitFlag, 0xFFFFFFFE, 0x00200000, 0x0004005B, (u32)btlevtcmd_AnimeChangePose,
    0xFFFFFFFE, 0x00000001, (u32)(char*)str_PWD_A1_1_802f7678, 0x0005005B, (u32)_get_attack_aud_no, 0xFE363C83,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0000006E, 0x0003005B, (u32)evt_btl_camera_set_mode,
    0x00000000, 0x0000000D, 0x0005005B, (u32)evt_btl_camera_set_homing_unit_audience, 0x00000000, 0xFFFFFFFE,
    0x00000001, 0xFE363C83, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000002,
    0x0003005B, (u32)evt_btl_camera_set_zoom, 0x00000000, 0x000000C8, 0x00000070, 0x00020032,
    0xFE363C85, 0xFE363C81, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE, 0xFE363C80,
    0x0000001E, 0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE, 0xF24A9280, 0x0003005B,
    (u32)btlevtcmd_SetFallAccel, 0xFFFFFFFE, 0xF24A7CE6, 0x000A005B, (u32)btlevtcmd_DivePosition, 0xFFFFFFFE,
    0xFE363C80, 0x00000000, 0xFE363C82, 0x00000028, 0x0000000F, 0x00000004,
    0x00000000, 0x00000000, 0x000A005B, (u32)btlevtcmd_DivePosition, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000014, 0x00000000, 0x00000004, 0x00000000,
    0x00000000, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PWD_A3_1_802f7730,
    0x0003005B, (u32)btlevtcmd_AnimeWaitPlayComplete, 0xFFFFFFFE, 0x00000001, 0x0006005B, (u32)btlevtcmd_snd_se,
    0xFFFFFFFE, (u32)(char*)str_SFX_BTL_CLAUD_KISS1_802f7748, 0xF1194D80, 0x00000000, 0xFE363C8E, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PWD_A3_2_802f773c, 0x0005005B, (u32)_get_attack_aud_no,
    0xFE363C83, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00020035, 0xFE363C81,
    0x00000019, 0x000F005B, (u32)evt_eff, (u32)(u8*)zero_80424060, (u32)(char*)str_kiss_80424064, 0x00000002,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0xFE363C86, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0003005B,
    (u32)_attack_aud, 0xFE363C83, 0x00000001, 0x0001006F, 0xFE363C8F, 0x0001000A,
    0x000005DC, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000, 0x0001000A,
    0x000003E8, 0x00000070, 0x0001000A, 0x000001F4, 0x0002005B, (u32)evt_snd_sfxoff,
    0xFE363C8E, 0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_BTL_CLAUD_KISS2_802f7768, 0xF1194D80,
    0x00000000, 0xF1194D80, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001,
    (u32)(char*)str_PWD_A3_3_802f777c, 0x0003005B, (u32)btlevtcmd_AnimeWaitPlayComplete, 0xFFFFFFFE, 0x00000001, 0x0001000A,
    0x0000012C, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001, 0x00000028,
    0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x000A005B, (u32)btlevtcmd_DivePosition, 0xFFFFFFFE, 0xFE363C80, 0x00000000, 0xFE363C82,
    0x00000010, 0x00000000, 0x00000004, 0x00000000, 0x00000000, 0x0003005B,
    (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE, 0xF24A9280, 0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0007005B, (u32)btlevtcmd_GetMoveFrame, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0xF24A9280, 0xFE363C8F, 0x00020035,
    0xFE363C8F, 0x0000001E, 0x000A005B, (u32)btlevtcmd_DivePosition, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0xFE363C8F, 0x00000014, 0x00000004, 0x00000000,
    0xFFFFFFFF, 0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002, 0x00000001
};

/* Recovered target Clauda .rodata ASCII string catalog, in target order. */
const char str_btl_wn_pwd_normal_802f7438[] = "btl_wn_pwd_normal";
const char str_msg_pwd_body_press_802f744c[] = "msg_pwd_body_press";
const char str_msg_ac_body_press_802f7460[] = "msg_ac_body_press";
const char str_btl_wn_pwd_lv1_802f7474[] = "btl_wn_pwd_lv1";
const char str_msg_pwd_breath_802f7484[] = "msg_pwd_breath";
const char str_msg_ac_breath_802f7494[] = "msg_ac_breath";
const char str_btl_wn_pwd_lv2_802f74a4[] = "btl_wn_pwd_lv2";
const char str_msg_pwd_sexy_kiss_802f74b4[] = "msg_pwd_sexy_kiss";
const char str_msg_ac_sexy_kiss_802f74c8[] = "msg_ac_sexy_kiss";
const char str_btl_wn_pwd_lv3_802f74dc[] = "btl_wn_pwd_lv3";
const char str_msg_pwd_kumogakure_802f74ec[] = "msg_pwd_kumogakure";
const char str_msg_ac_kumogakure_802f7500[] = "msg_ac_kumogakure";
const char str_PWD_D_5_802f7514[] = "PWD_D_5";
const char str_PWD_X_1_802f751c[] = "PWD_X_1";
const char str_PWD_D_2_802f7524[] = "PWD_D_2";
const char str_PWD_N_1_802f752c[] = "PWD_N_1";
const char str_PWD_D_3_802f7534[] = "PWD_D_3";
const char str_PWD_S_1_802f753c[] = "PWD_S_1";
const char str_PWD_G_2_802f7544[] = "PWD_G_2";
const char str_PWD_G_1_802f754c[] = "PWD_G_1";
const char str_PWD_D_1_802f7554[] = "PWD_D_1";
const char str_PWD_W_1_802f755c[] = "PWD_W_1";
const char str_PWD_R_1_802f7564[] = "PWD_R_1";
const char str_PWD_T_1_802f756c[] = "PWD_T_1";
const char str_PWD_I_1_802f7574[] = "PWD_I_1";
const char str_PWD_C_1_802f757c[] = "PWD_C_1";
const char str_PWD_O_1_802f7584[] = "PWD_O_1";
const char str_PWD_A4_3_802f758c[] = "PWD_A4_3";
const char str_btl_un_clauda_802f7598[] = "btl_un_clauda";
const char str_c_windy_802f75a8[] = "c_windy";
const char str_SFX_PARTY_DAMAGE1_802f75b0[] = "SFX_PARTY_DAMAGE1";
const char str_SFX_MARIO_DAMAGE_FIR_802f75c4[] = "SFX_MARIO_DAMAGE_FIRE1";
const char str_SFX_MARIO_DAMAGE_ICE_802f75dc[] = "SFX_MARIO_DAMAGE_ICE1";
const char str_SFX_MARIO_DAMAGE_BIR_802f75f4[] = "SFX_MARIO_DAMAGE_BIRIBIRI1";
const char str_SFX_PARTY_BATTLE_SUB_802f7610[] = "SFX_PARTY_BATTLE_SUBHANG1";
const char str_SFX_PARTY_BATTLE_HAN_802f762c[] = "SFX_PARTY_BATTLE_HANG1";
const char str_SFX_PARTY_BATTLE_JUM_802f7644[] = "SFX_PARTY_BATTLE_JUMP1";
const char str_SFX_PARTY_BATTLE_LAN_802f765c[] = "SFX_PARTY_BATTLE_LANDING1";
const char str_PWD_A1_1_802f7678[] = "PWD_A1_1";
const char str_SFX_BTL_CLAUD_ATTACK_802f7684[] = "SFX_BTL_CLAUD_ATTACK1";
const char str_PWD_A1_2_802f769c[] = "PWD_A1_2";
const char str_SFX_BTL_CLAUD_ATT2_802f76a8[] = "SFX_BTL_CLAUD_ATT2";
const char str_SFX_BTL_CLAUD_ATTACK_802f76bc[] = "SFX_BTL_CLAUD_ATTACK2";
const char str_PWD_Y_1_802f76d4[] = "PWD_Y_1";
const char str_PWD_A2_1_802f76dc[] = "PWD_A2_1";
const char str_SFX_BTL_CLAUD_BREATH_802f76e8[] = "SFX_BTL_CLAUD_BREATH1";
const char str_PWD_A2_2_802f7700[] = "PWD_A2_2";
const char str_PWD_A2_3_802f770c[] = "PWD_A2_3";
const char str_SFX_BTL_CLAUD_BREATH_802f7718[] = "SFX_BTL_CLAUD_BREATH2";
const char str_PWD_A3_1_802f7730[] = "PWD_A3_1";
const char str_PWD_A3_2_802f773c[] = "PWD_A3_2";
const char str_SFX_BTL_CLAUD_KISS1_802f7748[] = "SFX_BTL_CLAUD_KISS1";
const char str_recovery_802f775c[] = "recovery";
const char str_SFX_BTL_CLAUD_KISS2_802f7768[] = "SFX_BTL_CLAUD_KISS2";
const char str_PWD_A3_3_802f777c[] = "PWD_A3_3";
const char str_PWD_A_7_802f7788[] = "PWD_A_7";
const char str_PWD_A4_2_802f7790[] = "PWD_A4_2";
const char str_SFX_BTL_CLAUD_HIDE2_802f779c[] = "SFX_BTL_CLAUD_HIDE2";
const char str_kemuri_test_802f77b0[] = "kemuri_test";
const char str_SFX_MARIO_APPEAL1_802f77bc[] = "SFX_MARIO_APPEAL1";

/* Recovered target Clauda .sdata2 family, defined after all consumers. */
const u8 zero_80424060[1] = {0};
const char str_kiss_80424064[] = "kiss";
const f32 float_30_8042406c = 30.0f;
const f32 float_15_80424070 = 15.0f;
const f32 float_0_80424074 = 0.0f;
const f32 float_neg2_80424078 = -2.0f;
const f32 float_1_8042407c = 1.0f;
const f32 float_180_80424080 = 180.0f;
const f32 float_neg10_80424084 = -10.0f;
const f32 float_25_80424088 = 25.0f;
const f32 float_2_8042408c = 2.0f;
