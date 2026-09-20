#include "unit/party/unit_party_sanders.h"

extern void* _battleWorkPointer;
extern s32 BattleTransID(void* evt, s32 id);
extern void* BattleGetUnitPtr(void* battleWork, s32 id);
extern s32 evtGetValue(void* evt, s32 arg);
extern f32 evtSetFloat(void* evt, s32 arg, f32 value);
extern void evtSetValue(void* evt, s32 arg, s32 value);
extern void BtlUnit_GetPos(void* unit, f32* x, f32* y, f32* z);

/* Forward declarations for recovered unit_party_sanders event bytecode. */
static u32 battle_entry_event[59];
static u32 init_event[63];
static u32 damage_event[16];
static u32 wait_event[26];
extern u32 partySandersAttack_FirstAttack[321];
extern u32 partySandersAttack_NormalAttack[970];
extern u32 partySandersAttack_SuperBombAttack[1118];
extern u32 partySandersAttack_CounterSet[242];
extern u32 _shot_bomb_event[155];
extern u32 sanders_appeal_event[121];
static u32 attack_audience_event[284];
extern u32 unk_evt_8038b678[14];
static u32 attack_event[2];
static u32 party_win_reaction[9];
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
extern s32 btlevtcmd_AcSetOutputParam();
extern s32 btlevtcmd_GetUnitId();
extern s32 btlevtcmd_OffAttribute();
extern s32 btlevtcmd_OnAttribute();
extern s32 btlevtcmd_SpawnUnit();
extern s32 btlevtcmd_AcSetGaugeParam();
extern s32 btlevtcmd_AcSetParamAll();
extern s32 btlevtcmd_AddRotate();
extern s32 btlevtcmd_AnimeChangePose();
extern s32 btlevtcmd_AnimeChangePoseFromTable();
extern s32 btlevtcmd_AnimeChangePoseType();
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
extern s32 btlevtcmd_CheckStatus();
extern s32 btlevtcmd_CommandCheckDamage();
extern s32 btlevtcmd_CommandGetWeaponActionLv();
extern s32 btlevtcmd_CommandGetWeaponAddress();
extern s32 btlevtcmd_CommandPayWeaponCost();
extern s32 btlevtcmd_CommandPreCheckDamage();
extern s32 btlevtcmd_FaceDirectionAdd();
extern s32 btlevtcmd_FaceDirectionSub();
extern s32 btlevtcmd_GetBodyId();
extern s32 btlevtcmd_GetFaceDirection();
extern s32 btlevtcmd_GetFirstAttackTarget();
extern s32 btlevtcmd_GetHeight();
extern s32 btlevtcmd_GetHitPos();
extern s32 btlevtcmd_GetHomePos();
extern s32 btlevtcmd_GetPos();
extern s32 btlevtcmd_GetResultAC();
extern s32 btlevtcmd_GetResultPrizeLv();
extern s32 btlevtcmd_GetScale();
extern s32 btlevtcmd_GetSelectEnemy();
extern s32 btlevtcmd_GetSelectNextEnemy();
extern s32 btlevtcmd_GetStageAudienceAreaSize();
extern s32 btlevtcmd_GetStageSize();
extern s32 btlevtcmd_GetStatusMg();
extern s32 btlevtcmd_InviteApInfoReport();
extern s32 btlevtcmd_JumpPosition();
extern s32 btlevtcmd_JumpSetting();
extern s32 btlevtcmd_KillUnit();
extern s32 btlevtcmd_MovePosition();
extern s32 btlevtcmd_OnOffStatus();
extern s32 btlevtcmd_PayWeaponCost();
extern s32 btlevtcmd_ResetFaceDirection();
extern s32 btlevtcmd_ResultAC();
extern s32 btlevtcmd_RunDataEventChild();
extern s32 btlevtcmd_SetEventAttack();
extern s32 btlevtcmd_SetEventConfusion();
extern s32 btlevtcmd_SetEventDamage();
extern s32 btlevtcmd_SetEventEntry();
extern s32 btlevtcmd_SetEventWait();
extern s32 btlevtcmd_SetFallAccel();
extern s32 btlevtcmd_SetHomePos();
extern s32 btlevtcmd_SetJumpSound();
extern s32 btlevtcmd_SetMoveSoundDataWork();
extern s32 btlevtcmd_SetMoveSpeed();
extern s32 btlevtcmd_SetPos();
extern s32 btlevtcmd_SetRotate();
extern s32 btlevtcmd_SetRotateOffset();
extern s32 btlevtcmd_SetRunSound();
extern s32 btlevtcmd_SetScale();
extern s32 btlevtcmd_SetUnitWork();
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
extern s32 evt_btl_camera_set_homing_unit_audience();
extern s32 evt_btl_camera_set_homing_unitparts();
extern s32 evt_btl_camera_set_mode();
extern s32 evt_btl_camera_set_moveSpeedLv();
extern s32 evt_btl_camera_set_moveto();
extern s32 evt_btl_camera_set_zoom();
extern s32 evt_btl_camera_shake_h();
extern s32 evt_eff();
extern s32 evt_eff64();
extern s32 evt_snd_sfx_pit();
extern s32 evt_snd_sfxoff();
extern s32 evt_snd_sfxon();
extern s32 evt_snd_sfxon_3d();
extern s32 evt_sub_intpl_get_float();
extern s32 evt_sub_intpl_init_float();

extern u8 unit_bomzou[];

extern s32 weaponGetACOutputParam();
extern s32 weaponGetPowerFromPartyAttackLv();
extern u32 btldefaultevt_Dummy[];
extern u32 btldefaultevt_avoid_counter_event[];
extern u32 subset_attack_ready_jump_event[];
extern u32 partySandersAttack_TimeBombSet[];
extern u32 entry_bomzou[12];

/* Recovered target .sdata2 event/pose-string prefix. */
const char str_D_4_80424148[] = "D_4";
const char str_X_1_8042414c[] = "X_1";
const char str_D_2_80424150[] = "D_2";
const char str_N_1_80424154[] = "N_1";
const char str_D_3_80424158[] = "D_3";
const char str_S_1_8042415c[] = "S_1";
const char str_G_2_80424160[] = "G_2";
const char str_G_1_80424164[] = "G_1";
const char str_D_1_80424168[] = "D_1";
const char str_W_1_8042416c[] = "W_1";
const char str_R_1_80424170[] = "R_1";
const char str_T_1_80424174[] = "T_1";
const char str_I_1_80424178[] = "I_1";
const char str_C_1_8042417c[] = "C_1";
const char str_O_1_80424180[] = "O_1";
const char str_R_3_80424184[] = "R_3";
const char str_J_1A_80424188[] = "J_1A";
const char str_N_2_80424190[] = "N_2";
const char str_A_3_80424194[] = "A_3";
const char str_R_2_80424198[] = "R_2";
const u8 zero_8042419c[1] = {0};
const char str_J_1B_804241a0[] = "J_1B";
const char str_F_1_804241a8[] = "F_1";
const char str_Y_1_804241ac[] = "Y_1";
const char str_S_2_804241b0[] = "S_2";
const char str_A_4A_804241b4[] = "A_4A";
const char str_A_4B_804241bc[] = "A_4B";
const char str_B_2_804241c4[] = "B_2";

/* Exact Shift-JIS target string used by Time Bomb Set. */
/* Recovered target .rodata ASCII string prefix. */
const char str_btl_un_sanders_802f81e0[] = "btl_un_sanders";
const char str_c_bomt_n_802f81f0[] = "c_bomt_n";
const char str_SFX_PARTY_DAMAGE1_802f81fc[] = "SFX_PARTY_DAMAGE1";
const char str_SFX_MARIO_DAMAGE_FIR_802f8210[] = "SFX_MARIO_DAMAGE_FIRE1";
const char str_SFX_MARIO_DAMAGE_ICE_802f8228[] = "SFX_MARIO_DAMAGE_ICE1";
const char str_SFX_MARIO_DAMAGE_BIR_802f8240[] = "SFX_MARIO_DAMAGE_BIRIBIRI1";
const char str_SFX_PARTY_BATTLE_SUB_802f825c[] = "SFX_PARTY_BATTLE_SUBMOVE1L";
const char str_SFX_PARTY_BATTLE_SUB_802f8278[] = "SFX_PARTY_BATTLE_SUBMOVE1R";
const char str_SFX_PARTY_BATTLE_MOV_802f8294[] = "SFX_PARTY_BATTLE_MOVE1L";
const char str_SFX_PARTY_BATTLE_MOV_802f82ac[] = "SFX_PARTY_BATTLE_MOVE1R";
const char str_SFX_PARTY_BATTLE_JUM_802f82c4[] = "SFX_PARTY_BATTLE_JUMP1";
const char str_SFX_PARTY_BATTLE_LAN_802f82dc[] = "SFX_PARTY_BATTLE_LANDING1";
const char str_btl_wn_pbm_normal_802f82f8[] = "btl_wn_pbm_normal";
const char str_msg_pbm_bakuhatsu_802f830c[] = "msg_pbm_bakuhatsu";
const char str_msg_ac_bakuhatsu_802f8320[] = "msg_ac_bakuhatsu";
const char str_btl_wn_pbm_lv1_802f8334[] = "btl_wn_pbm_lv1";
const char str_msg_pbm_jigen_bakuda_802f8344[] = "msg_pbm_jigen_bakudan";
const char str_msg_ac_jigen_bakudan_802f835c[] = "msg_ac_jigen_bakudan";
const char str_btl_wn_pbm_lv2_802f8374[] = "btl_wn_pbm_lv2";
const char str_msg_pbm_counter_802f8384[] = "msg_pbm_counter";
const char str_msg_ac_bom_counter_802f8394[] = "msg_ac_bom_counter";
const char str_btl_wn_pbm_lv3_802f83a8[] = "btl_wn_pbm_lv3";
const char str_msg_pbm_sungoi_bakuh_802f83b8[] = "msg_pbm_sungoi_bakuhatsu";
const char str_msg_ac_super_bakuhat_802f83d4[] = "msg_ac_super_bakuhatsu";
const char str_SFX_BTL_THUNDERS_ATT_802f83ec[] = "SFX_BTL_THUNDERS_ATTACK1";
const char str_SFX_BTL_THUNDERS_ATT_802f8408[] = "SFX_BTL_THUNDERS_ATTACK2";
const char str_SFX_BTL_THUNDERS_ATT_802f8424[] = "SFX_BTL_THUNDERS_ATTACK3";
const char str_bomb_n64_802f8440[] = "bomb_n64";
const char str_SFX_BTL_THUNDERS_DOW_802f844c[] = "SFX_BTL_THUNDERS_DOWN1";
const char str_sandars_802f8464[] = "sandars";
const char str_SFX_BTL_THUNDERS_ATT_802f846c[] = "SFX_BTL_THUNDERS_ATTACK4";

const char str_event_end_802f8488[] = "\x83\x43\x83\x78\x83\x93\x83\x67\x8F\x49\x97\xB9";
/* Recovered target .rodata ASCII string suffix. */
const char str_SFX_BTL_THUNDERS_BOM_802f8498[] = "SFX_BTL_THUNDERS_BOMB1";
const char str_SFX_MARIO_APPEAL1_802f84b0[] = "SFX_MARIO_APPEAL1";

s32 _sanders_make_extra_work_area(void* evt) {
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


void __makeTechMenuFunc(void* commandWork, s32* count) {
    extern void* BattleGetPartyPtr(void* battleWork);
    extern s32 BattleTransPartyId(s32 id);
    extern s32 partyGetTechLv(s32 partyId);
    extern char* msgSearch(char* msg);
    extern u8 lbl_803875D0[];
    s32 techLv;
    void* party;
    u8* data;
    u8* entry;

    party = BattleGetPartyPtr(_battleWorkPointer);
    techLv = partyGetTechLv(BattleTransPartyId(*(s32*)((s32)party + 8)));

    data = lbl_803875D0;
    entry = (u8*)commandWork + *count * 0x1C;
    *(s32*)(entry + 0x90) = -1;
    entry = (u8*)commandWork + *count * 0x1C;
    *(s32*)(entry + 0x94) = 0;
    entry = (u8*)commandWork + *count * 0x1C;
    *(void**)(entry + 0x80) = data + 0x650;
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
        *(void**)(entry + 0x80) = data + 0x710;
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
        *(void**)(entry + 0x80) = data + 0x7D0;
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
        *(void**)(entry + 0x80) = data + 0x890;
        entry = (u8*)commandWork + *count * 0x1C;
        *(s32*)(entry + 0x84) = 0;
        entry = (u8*)commandWork + *count * 0x1C;
        *(u16*)(entry + 0x8C) = *(u16*)(*(u8**)(entry + 0x80) + 4);
        *(void**)((u8*)commandWork + *count * 0x1C + 0x88) =
            msgSearch(**(char***)((u8*)commandWork + *count * 0x1C + 0x80));
        *count = *count + 1;
    }
}

s32 _get_bomb_hit_position(int param_1) {

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

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on
s32 _make_counterset_weapon(void* evt) {
    extern void* memcpy(void* dest, const void* src, u32 size);

    void* battleWork = _battleWorkPointer;
    s32* args = *(s32**)((s32)evt + 0x18);
    s32 id = BattleTransID(evt, -2);
    void* unit = BattleGetUnitPtr(battleWork, id);
    s32 out = args[0];
    void* weapon = *(void**)((s32)unit + 0x314);
    s32 src = evtGetValue(evt, out);
    s32 flag = evtGetValue(evt, args[1]);
    memcpy(weapon, (void*)src, 0xC0);
    *(u8*)((s32)weapon + 0xA7) = flag;
    evtSetValue(evt, out, (s32)weapon);
    return 2;
}

u8 _shot_move(void* evt, s32 isFirstCall) {
    extern void BtlUnit_SetPos(void* unit, f32 x, f32 y, f32 z);
    extern f32 angleABf(f32 x0, f32 z0, f32 x1, f32 z1);
    extern f64 cosfd(f64 deg);
    extern f64 sinfd(f64 deg);
    extern void btlMovePos(f32* x, f32* z, f32 speed, f32 angle);
    extern f32 BattleGetFloorHeight(void* battleWork, f32 x, f32 y, f32 z);
    extern const f32 float_0p1_804241cc;
    extern const f32 float_0_804241d0;
    extern const f32 float_1_804241d4;
    extern const f32 float_0p2_804241d8;
    extern const f32 float_0p8_804241dc;
    extern const f32 float_0p9_804241e0;
    extern const f32 float_0p75_804241e4;
    s32* args;
    void* battleWork;
    s32 unitId;
    s32 angle;
    f32 speed;
    void* unit;
    s32 outX;
    s32 outY;
    s32 outZ;
    s32 outSpeed;
    f32 x;
    f32 y;
    f32 z;
    f32 floor;

    args = *(s32**)((s32)evt + 0x18);
    battleWork = _battleWorkPointer;
    unitId = BattleTransID(evt, evtGetValue(evt, args[0]));
    angle = evtGetValue(evt, args[1]);
    speed = (f32)evtGetValue(evt, args[2]) * float_0p1_804241cc;
    outX = args[3];
    outY = args[4];
    outZ = args[5];
    outSpeed = args[6];
    unit = BattleGetUnitPtr(battleWork, unitId);

    if (isFirstCall != 0) {
        BtlUnit_GetPos(unit, &x, &y, &z);
        *(f32*)((s32)unit + 0x154) = x;
        *(f32*)((s32)unit + 0x158) = y;
        *(f32*)((s32)unit + 0x15C) = z;
        *(f32*)((s32)unit + 0x17C) = angleABf(float_0_804241d0, float_0_804241d0, float_1_804241d4, float_0_804241d0);
        *(f32*)((s32)unit + 0x174) = float_0p2_804241d8;
        *(f32*)((s32)unit + 0x178) = float_0p8_804241dc * (float_0p9_804241e0 * (speed * (f32)cosfd((f64)angle)));
        *(f32*)((s32)unit + 0x170) = float_0p8_804241dc * (float_0p75_804241e4 * (speed * (f32)sinfd((f64)angle)));
    }

    *(f32*)((s32)unit + 0x158) += *(f32*)((s32)unit + 0x178);
    *(f32*)((s32)unit + 0x178) -= *(f32*)((s32)unit + 0x174);
    btlMovePos((f32*)((s32)unit + 0x154), (f32*)((s32)unit + 0x15C), *(f32*)((s32)unit + 0x170), *(f32*)((s32)unit + 0x17C));
    BtlUnit_SetPos(unit, *(f32*)((s32)unit + 0x154), *(f32*)((s32)unit + 0x158), *(f32*)((s32)unit + 0x15C));
    floor = BattleGetFloorHeight(battleWork, *(f32*)((s32)unit + 0x154), *(f32*)((s32)unit + 0x158), *(f32*)((s32)unit + 0x15C));
    if (*(f32*)((s32)unit + 0x158) > floor) {
        return 0;
    }
    BtlUnit_SetPos(unit, *(f32*)((s32)unit + 0x154), floor, *(f32*)((s32)unit + 0x15C));
    evtSetValue(evt, outX, (s32)*(f32*)((s32)unit + 0x154));
    evtSetValue(evt, outY, (s32)floor);
    evtSetValue(evt, outZ, (s32)*(f32*)((s32)unit + 0x15C));
    evtSetFloat(evt, outSpeed, *(f32*)((s32)unit + 0x170));
    return 1;
}




#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 _judge_on_stage(int param_1) {
    extern void* BattleStageGetPtr(void);
    extern f32 __fabsf(f32 value);
    extern const f32 float_40_804241c8;

    void* evt = (void*)param_1;
    void* battleWork = _battleWorkPointer;
    s32* args = *(s32**)((s32)evt + 0x18);
    void* stage = BattleStageGetPtr();
    s32 id = evtGetValue(evt, args[0]);
    s32 out = args[1];
    void* unit = BattleGetUnitPtr(battleWork, BattleTransID(evt, id));
    f32 x;
    f32 y;
    f32 z;

    BtlUnit_GetPos(unit, &x, &y, &z);
    if (__fabsf(x) < float_40_804241c8 + *(f32*)((s32)stage + 0xB24)) {
        evtSetValue(evt, out, 1);
    } else {
        evtSetValue(evt, out, 0);
    }
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

/* Target .sdata2 scalars defined after both consumers so each function sees declarations only. */
const f32 float_40_804241c8 = 40.0f;
const f32 float_0p1_804241cc = 0.1f;
const f32 float_0_804241d0 = 0.0f;
const f32 float_1_804241d4 = 1.0f;
const f32 float_0p2_804241d8 = 0.2f;
const f32 float_0p8_804241dc = 0.8f;
const f32 float_0p9_804241e0 = 0.9f;
const f32 float_0p75_804241e4 = 0.75f;

/* Recovered target .sdata unit metadata prefix. */
static u8 defence[5] = {0, 0, 0, 0, 0};
static u8 defence_attr[5] = {0, 0, 0, 0, 0};
static u8 regist[22] = {
    100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
    100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100
};

/* Recovered unit_party_sanders event bytecode (.data/.sdata). */
/* Recovered target pose/data tables; exact words + symbolic relocations. */
u32 pose_table_sanders_stay[50] = {
    0x00000009, (u32)(char*)str_D_4_80424148, 0x00000002, (u32)(char*)str_D_4_80424148, 0x00000003, (u32)(char*)str_X_1_8042414c,
    0x00000004, (u32)(char*)str_D_2_80424150, 0x00000005, (u32)(char*)str_X_1_8042414c, 0x00000001, (u32)(char*)str_N_1_80424154,
    0x0000001B, (u32)(char*)str_D_3_80424158, 0x0000001C, (u32)(char*)str_S_1_8042415c, 0x00000025, (u32)(char*)str_G_2_80424160,
    0x0000001F, (u32)(char*)str_G_1_80424164, 0x00000027, (u32)(char*)str_D_1_80424168, 0x00000028, (u32)(char*)str_W_1_8042416c,
    0x0000002A, (u32)(char*)str_R_1_80424170, 0x00000039, (u32)(char*)str_D_2_80424150, 0x00000038, (u32)(char*)str_D_2_80424150,
    0x0000003C, (u32)(char*)str_D_4_80424148, 0x0000003D, (u32)(char*)str_D_2_80424150, 0x00000041, (u32)(char*)str_T_1_80424174,
    0x00000034, (u32)(char*)str_I_1_80424178, 0x00000043, (u32)(char*)str_C_1_8042417c, 0x00000042, (u32)(char*)str_O_1_80424180,
    0x00000040, (u32)(char*)str_R_3_80424184, 0x00000031, (u32)(char*)str_J_1A_80424188, 0x0000002D, (u32)(char*)str_G_1_80424164,
    0x00000045, (u32)(char*)str_S_1_8042415c
};

u32 pose_table_counter_sanders[40] = {
    0x00000001, (u32)(char*)str_N_2_80424190, 0x0000001B, (u32)(char*)str_D_3_80424158, 0x0000001C, (u32)(char*)str_A_3_80424194,
    0x00000025, (u32)(char*)str_G_2_80424160, 0x0000001F, (u32)(char*)str_G_1_80424164, 0x00000027, (u32)(char*)str_D_1_80424168,
    0x00000028, (u32)(char*)str_W_1_8042416c, 0x0000002A, (u32)(char*)str_R_1_80424170, 0x00000039, (u32)(char*)str_D_2_80424150,
    0x00000038, (u32)(char*)str_D_2_80424150, 0x0000003C, (u32)(char*)str_D_4_80424148, 0x0000003D, (u32)(char*)str_D_2_80424150,
    0x00000041, (u32)(char*)str_T_1_80424174, 0x00000034, (u32)(char*)str_I_1_80424178, 0x00000043, (u32)(char*)str_C_1_8042417c,
    0x00000042, (u32)(char*)str_O_1_80424180, 0x00000040, (u32)(char*)str_R_3_80424184, 0x00000031, (u32)(char*)str_J_1A_80424188,
    0x0000002D, (u32)(char*)str_G_1_80424164, 0x00000045, (u32)(char*)str_A_3_80424194
};

static u32 data_table[18] = {
    0x00000005, (u32)sanders_appeal_event, 0x00000006, (u32)attack_audience_event, 0x00000001, (u32)__makeTechMenuFunc,
    0x00000030, (u32)btldefaultevt_Dummy, 0x00000040, (u32)party_win_reaction, 0x00000007, (u32)subset_attack_ready_jump_event,
    0x00000042, (u32)unk_evt_8038b678, 0x0000002F, (u32)btldefaultevt_avoid_counter_event, 0x00000000, 0x00000000
};

/* Recovered target BattleUnitKindPart/BattleUnitKind records; exact words + symbolic relocations. */
u32 unitpartsdata_Party_Sanders[19] = {
    0x00000001, (u32)(char*)str_btl_un_sanders_802f81e0, (u32)(char*)str_c_bomt_n_802f81f0, 0x00000000, 0x00000000, 0x00000000,
    0x41800000, 0x41C00000, 0x40A00000, 0x00000000, 0x41C00000, 0x00000000,
    0x00140018, 0x00FF0000, (u32)defence, (u32)defence_attr, 0x00000009, 0x00000000,
    (u32)pose_table_sanders_stay
};

u32 unitdata_Party_Sanders[49] = {
    0x000000E5, (u32)(char*)str_btl_un_sanders_802f81e0, 0x000A0005, 0x05010100, 0x00000064, 0x00090014,
    0x0021000B, 0x001C0000, 0x00000000, 0x41800000, 0x00000000, 0x00000000,
    0x00000000, 0x42200000, 0x00000000, 0x00000000, 0x42200000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x41200000,
    0x00000000, 0x00000000, 0x41200000, 0x41AB999A, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0000FF00, 0x64140000,
    0x00000000, 0x00000000, 0x00000000, (u32)(char*)str_SFX_PARTY_DAMAGE1_802f81fc, (u32)(char*)str_SFX_MARIO_DAMAGE_FIR_802f8210, (u32)(char*)str_SFX_MARIO_DAMAGE_ICE_802f8228,
    (u32)(char*)str_SFX_MARIO_DAMAGE_BIR_802f8240, 0x02000000, (u32)regist, 0x01000000, (u32)unitpartsdata_Party_Sanders, (u32)init_event,
    (u32)data_table
};

static u32 battle_entry_event[] = {
    0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE, 0xFE363C80, 0x000000FA, 0x0005005B,
    (u32)btlevtcmd_SetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00010009,
    0x00000028, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001, 0x0000002A,
    0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE, 0xF24A8A80, 0x0009005B, (u32)btlevtcmd_SetMoveSoundDataWork,
    0xFFFFFFFE, (u32)(char*)str_SFX_PARTY_BATTLE_SUB_802f825c, (u32)(char*)str_SFX_PARTY_BATTLE_SUB_802f8278, 0x00000000, 0x00000009, 0x00000009,
    0x00000000, 0x00000000, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000000, 0xFFFFFFFE, 0x00000000, 0x0003005B,
    (u32)btlevtcmd_AnimeChangePoseFromTable, 0xFFFFFFFE, 0x00000001, 0x00000002, 0x00000001
};

static u32 init_event[] = {
    0x0003005B, (u32)btlevtcmd_SetEventWait, 0xFFFFFFFE, (u32)wait_event, 0x0003005B, (u32)btlevtcmd_SetEventAttack,
    0xFFFFFFFE, (u32)attack_event, 0x0003005B, (u32)btlevtcmd_SetEventDamage, 0xFFFFFFFE, (u32)damage_event,
    0x0003005B, (u32)btlevtcmd_SetEventConfusion, 0xFFFFFFFE, (u32)btldefaultevt_Confuse, 0x0007005B, (u32)btlevtcmd_SetWalkSound,
    0xFFFFFFFE, (u32)(char*)str_SFX_PARTY_BATTLE_MOV_802f8294, (u32)(char*)str_SFX_PARTY_BATTLE_MOV_802f82ac, 0x00000000, 0x00000012, 0x00000012,
    0x0007005B, (u32)btlevtcmd_SetRunSound, 0xFFFFFFFE, (u32)(char*)str_SFX_PARTY_BATTLE_MOV_802f8294, (u32)(char*)str_SFX_PARTY_BATTLE_MOV_802f82ac, 0x00000000,
    0x00000009, 0x00000009, 0x0004005B, (u32)btlevtcmd_SetJumpSound, 0xFFFFFFFE, (u32)(char*)str_SFX_PARTY_BATTLE_JUM_802f82c4,
    (u32)(char*)str_SFX_PARTY_BATTLE_LAN_802f82dc, 0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000000, 0xFFFFFFFF,
    0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000001, 0xFFFFFFFF, 0x0004005B,
    (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000002, 0xFFFFFFFF, 0x0001005B, (u32)_sanders_make_extra_work_area,
    0x0003005B, (u32)btlevtcmd_SetEventEntry, 0xFFFFFFFE, (u32)battle_entry_event, 0x0002005B, (u32)btlevtcmd_StartWaitEvent,
    0xFFFFFFFE, 0x00000002, 0x00000001
};

static u32 damage_event[] = {
    0x0005005B, (u32)btlevtcmd_SetRotate, 0xFFFFFFFE, 0x00000000, 0x00000000, 0x00000000,
    0x00020032, 0xFE363C8A, 0xFFFFFFFE, 0x00020032, 0xFE363C8B, 0x00000001,
    0x0001005E, (u32)btldefaultevt_Damage, 0x00000002, 0x00000001
};

static u32 wait_event[] = {
    0x0004005B, (u32)btlevtcmd_CheckStatus, 0xFFFFFFFE, 0x00000016, 0xFE363C80, 0x00020019,
    0xFE363C80, 0x00000000, 0x0004005B, (u32)btlevtcmd_AnimeSetPoseTable, 0xFFFFFFFE, 0x00000001,
    (u32)pose_table_counter_sanders, 0x00000020, 0x0004005B, (u32)btlevtcmd_AnimeSetPoseTable, 0xFFFFFFFE, 0x00000001,
    (u32)pose_table_sanders_stay, 0x00000021, 0x0003005B, (u32)btlevtcmd_AnimeChangePoseFromTable, 0xFFFFFFFE, 0x00000001,
    0x00000002, 0x00000001
};

/* Recovered target BattleWeapon records. */
u32 partyWeapon_SandersFirstAttack[48] = {
    0x00000000, 0x01580000, 0x00000000, 0x00000000, 0x64000000, 0x3F800000,
    0x01010132, (u32)weaponGetPowerFromPartyAttackLv, 0x00000004, 0x00000004, 0x00000005, 0x00000005,
    0x00000006, 0x00000006, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01101260, 0x21002000, 0x03000302, 0x00000000, 0x00026087,
    0x000007FB, 0x80002004, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)partySandersAttack_FirstAttack, 0x00000064, 0x00000000, 0x00000000
};

u32 partyWeapon_SandersNormalAttack[48] = {
    (u32)(char*)str_btl_wn_pbm_normal_802f82f8, 0x01580000, 0x00000000, (u32)(char*)str_msg_pbm_bakuhatsu_802f830c, 0x64000000, 0x3F800000,
    0x01016432, (u32)weaponGetPowerFromPartyAttackLv, 0x00000001, 0x00000004, 0x00000002, 0x00000005,
    0x00000003, 0x00000006, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01101260, 0x21002000, 0x03000302, (u32)(char*)str_msg_ac_bakuhatsu_802f8320, 0x00036087,
    0x000007FB, 0x80002004, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)partySandersAttack_NormalAttack, 0x000A0A64, 0x0A0A0A05, 0x0A000000
};

u32 partyWeapon_SandersTimeBombSet[48] = {
    (u32)(char*)str_btl_wn_pbm_lv1_802f8334, 0x01590000, 0x00000000, (u32)(char*)str_msg_pbm_jigen_bakuda_802f8344, 0x64030000, 0x3F800000,
    0x01056432, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x02101260, 0x20000000, 0x00000302, (u32)(char*)str_msg_ac_jigen_bakudan_802f835c, 0x00020000,
    0x000007FF, 0x80002004, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)partySandersAttack_TimeBombSet, 0x00000000, 0x00000000, 0x00000000
};

u32 partyWeapon_SandersCounterSet[48] = {
    (u32)(char*)str_btl_wn_pbm_lv2_802f8374, 0x015A0000, 0x00000000, (u32)(char*)str_msg_pbm_counter_802f8384, 0x64040000, 0x3F800000,
    0x01056432, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01004020, 0x10000000, 0x00000302, (u32)(char*)str_msg_ac_bom_counter_802f8394, 0x00020108,
    0x000007FF, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000001,
    0x00000000, 0x00000000, (u32)partySandersAttack_CounterSet, 0x00050564, 0x140A0505, 0x05050000
};

u32 partyWeapon_SandersSuperBombAttack[48] = {
    (u32)(char*)str_btl_wn_pbm_lv3_802f83a8, 0x015B0000, 0x00000000, (u32)(char*)str_msg_pbm_sungoi_bakuh_802f83b8, 0x64090000, 0x3F800000,
    0x01056432, (u32)weaponGetACOutputParam, 0x00000002, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x02101260, 0x20000000, 0x03000302, (u32)(char*)str_msg_ac_super_bakuhat_802f83d4, 0x00026087,
    0x000007FF, 0x80002004, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)partySandersAttack_SuperBombAttack, 0x00323264, 0x1419190A, 0x0A050000
};

u32 partySandersAttack_FirstAttack[] = {
    0x0005005B, (u32)btlevtcmd_JumpSetting, 0xFFFFFFFE, 0x00000014, 0xF24A7A80, 0xF24A7D4C,
    0x0003005B, (u32)btlevtcmd_GetFirstAttackTarget, 0xFE363C83, 0xFE363C84, 0x00020018, 0xFE363C83,
    0xFFFFFFFF, 0x00010004, 0x00000063, 0x00000021, 0x00020032, 0xFE363C8C,
    (u32)partyWeapon_SandersFirstAttack, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x0000000B, 0x0006005B,
    (u32)evt_btl_camera_set_homing_unitparts, 0x00000000, 0xFFFFFFFE, 0x00000001, 0xFE363C83, 0xFE363C84,
    0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000002, 0x0003005B, (u32)evt_btl_camera_set_zoom,
    0x00000000, 0x000000C8, 0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE, 0xFE363C80,
    0x000000FA, 0x0005005B, (u32)btlevtcmd_SetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00010009, 0x00000032, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE,
    0x00000001, (u32)(char*)str_R_2_80424198, 0x0003005B, (u32)btlevtcmd_GetStatusMg, 0xFFFFFFFE, 0xFE363C86,
    0x0002003C, 0xFE363C86, 0x00000014, 0x0007005B, (u32)_get_bomb_hit_position, 0xFFFFFFFE,
    0xFE363C83, 0xFE363C84, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B,
    (u32)btlevtcmd_FaceDirectionAdd, 0xFE363C83, 0xFE363C80, 0xFE363C86, 0x0004005B, (u32)btlevtcmd_TransStageFloorPosition,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0009005B, (u32)btlevtcmd_SetMoveSoundDataWork, 0xFFFFFFFE,
    (u32)(char*)str_SFX_BTL_THUNDERS_ATT_802f83ec, 0x00000000, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000,
    0x00000000, 0x00020035, 0xFE363C82, 0x00000005, 0x0003005B, (u32)btlevtcmd_SetMoveSpeed,
    0xFFFFFFFE, 0xF24A8E80, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C80,
    0x00000000, 0xFE363C82, 0x00000000, 0xFFFFFFFE, 0x00000001, 0x0003005B,
    (u32)evt_snd_sfxon, (u32)(char*)str_SFX_BTL_THUNDERS_ATT_802f8408, 0xFE363C8E, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE,
    0x00000001, (u32)(char*)str_A_3_80424194, 0x00010009, 0x0000003C, 0x0002005B, (u32)evt_snd_sfxoff,
    0xFE363C8E, 0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_BTL_THUNDERS_ATT_802f8424, 0x00000000, 0x0005005B,
    (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0003005B,
    (u32)btlevtcmd_GetHeight, 0xFFFFFFFE, 0xFE363C86, 0x00020038, 0xFE363C86, 0x00000002,
    0x00020035, 0xFE363C81, 0xFE363C86, 0x000F005B, (u32)evt_eff64, (u32)(u8*)zero_8042419c,
    (u32)(char*)str_bomb_n64_802f8440, 0x00000001, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x0006005B, (u32)evt_btl_camera_shake_h, 0x00000000, 0x00000005, 0x00000000,
    0x0000000A, 0x0000000D, 0x0001005B, (u32)btlevtcmd_StageDispellFog, 0x0002005B, (u32)btlevtcmd_WeaponAftereffect,
    0xFE363C8C, 0x0007005B, (u32)btlevtcmd_CheckDamage, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84,
    0xFE363C8C, 0x00020100, 0xFE363C85, 0x00010003, 0x0000005A, 0x0001005B,
    (u32)evt_audience_ap_recovery, 0x0001005B, (u32)btlevtcmd_InviteApInfoReport, 0x00010003, 0x0000005C, 0x0003005B,
    (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000,
    0x00000001, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_J_1A_80424188,
    0x0000006E, 0x00010009, 0x0000001E, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE,
    0x00000001, (u32)(char*)str_J_1B_804241a0, 0x00000070, 0x0003005B, (u32)btlevtcmd_SetFallAccel, 0xFFFFFFFE,
    0xF24A7C80, 0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0004005B, (u32)btlevtcmd_FaceDirectionAdd, 0xFFFFFFFE, 0xFE363C80, 0x00000028,
    0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0000005A, 0xFFFFFFFF, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001,
    (u32)(char*)str_J_1A_80424188, 0x0000006E, 0x00010009, 0x00000008, 0x0004005B, (u32)btlevtcmd_AnimeChangePose,
    0xFFFFFFFE, 0x00000001, (u32)(char*)str_J_1B_804241a0, 0x00000070, 0x0003005B, (u32)btlevtcmd_SetFallAccel,
    0xFFFFFFFE, 0xF24A7D4C, 0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0004005B, (u32)btlevtcmd_FaceDirectionAdd, 0xFFFFFFFE, 0xFE363C80,
    0x00000014, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000010, 0xFFFFFFFF, 0x00010009, 0x00000014, 0x00010003,
    0x0000005F, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000, 0x0003005B,
    (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001, 0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE,
    0xF24A9280, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001, 0x00000028,
    0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00000000, 0xFFFFFFFF, 0x00000000, 0x00010003, 0x00000063, 0x0002005B,
    (u32)btlevtcmd_ResetFaceDirection, 0xFFFFFFFE, 0x0001005E, (u32)btldefaultevt_SuitoruBadgeEffect, 0x0002005B, (u32)btlevtcmd_StartWaitEvent,
    0xFFFFFFFE, 0x00000002, 0x00000001
};

u32 partySandersAttack_NormalAttack[] = {
    0x0005005B, (u32)btlevtcmd_JumpSetting, 0xFFFFFFFE, 0x00000014, 0xF24A7A80, 0xF24A7D4C,
    0x0003005B, (u32)btlevtcmd_GetSelectEnemy, 0xFE363C83, 0xFE363C84, 0x00020018, 0xFE363C83,
    0xFFFFFFFF, 0x00010004, 0x00000063, 0x00000021, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress,
    0xFFFFFFFE, 0xFE363C8C, 0x0002005B, (u32)btlevtcmd_WeaponAftereffect, 0xFE363C8C, 0x0004005B,
    (u32)btlevtcmd_AttackDeclare, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0x0001005B, (u32)btlevtcmd_WaitGuardMove,
    0x0002005B, (u32)btlevtcmd_CommandPayWeaponCost, 0xFFFFFFFE, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFFFFFFFE,
    0x00000007, 0x0007005B, (u32)btlevtcmd_CalculateFaceDirection, 0xFFFFFFFE, 0xFFFFFFFF, 0xFE363C83,
    0xFE363C84, 0x00000010, 0xFE363C8F, 0x0003005B, (u32)btlevtcmd_ChangeFaceDirection, 0xFFFFFFFE,
    0xFE363C8F, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x0000000B, 0x0006005B,
    (u32)evt_btl_camera_set_homing_unitparts, 0x00000000, 0xFFFFFFFE, 0x00000001, 0xFE363C83, 0xFE363C84,
    0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000002, 0x0003005B, (u32)evt_btl_camera_set_zoom,
    0x00000000, 0x000000C8, 0x0002005B, (u32)btlevtcmd_CommandGetWeaponActionLv, 0xFE363C80, 0x0003005B,
    (u32)btlevtcmd_AcSetDifficulty, 0xFFFFFFFE, 0xFE363C80, 0x0002005B, (u32)btlevtcmd_AcGetDifficulty, 0xFE363C80,
    0x00020036, 0xFE363C80, 0x00000003, 0x00010022, 0xFE363C80, 0x00010028,
    0xFFFFFFFD, 0x00020032, 0xFE363C80, 0x00000078, 0x00020032, 0xFE363C81,
    0x0000003C, 0x00010024, 0xFFFFFFFE, 0x00020032, 0xFE363C80, 0x00000078,
    0x00020032, 0xFE363C81, 0x0000002D, 0x00010024, 0xFFFFFFFF, 0x00020032,
    0xFE363C80, 0x00000078, 0x00020032, 0xFE363C81, 0x0000001E, 0x00010024,
    0x00000000, 0x00020032, 0xFE363C80, 0x00000078, 0x00020032, 0xFE363C81,
    0x00000019, 0x00010024, 0x00000001, 0x00020032, 0xFE363C80, 0x00000078,
    0x00020032, 0xFE363C81, 0x00000014, 0x00010024, 0x00000002, 0x00020032,
    0xFE363C80, 0x00000078, 0x00020032, 0xFE363C81, 0x0000000C, 0x0000002A,
    0x00020032, 0xFE363C80, 0x00000078, 0x00020032, 0xFE363C81, 0x00000006,
    0x00000031, 0x0009005B, (u32)btlevtcmd_AcSetParamAll, 0x00000002, 0xFE363C80, 0xFE363C81,
    0x00000000, 0x00000064, 0xF1194D80, 0xF1194D80, 0xF1194D80, 0x0002005B,
    (u32)btlevtcmd_AcSetFlag, 0x0000000F, 0x0005005B, (u32)btlevtcmd_SetupAC, 0xFFFFFFFE, 0x0000000F,
    0x00000001, 0x00000000, 0x00010009, 0x00000016, 0x0001006F, 0xFE363C8F,
    0x00020032, 0xFE363C80, 0x00000005, 0x00010005, 0xFE363C80, 0x0003005B,
    (u32)btlevtcmd_AcGetOutputParam, 0x00000000, 0xFE363C81, 0x00020018, 0xFE363C81, 0x00000000,
    0x00020032, 0xFE363C80, 0x00000005, 0x00000020, 0x00000007, 0x00000021,
    0x00010009, 0x00000001, 0x00000006, 0x00000070, 0x0002005B, (u32)btlevtcmd_StartAC,
    0x00000001, 0x0002005B, (u32)btlevtcmd_WaitEventEnd, 0xFE363C8F, 0x0003005B, (u32)btlevtcmd_AcGetOutputParam,
    0x00000000, 0xFE363C81, 0x00020018, 0xFE363C81, 0x00000002, 0x0005005B,
    (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B,
    (u32)btlevtcmd_FaceDirectionAdd, 0xFFFFFFFE, 0xFE363C80, 0x00000014, 0x0009005B, (u32)btlevtcmd_SetMoveSoundDataWork,
    0xFFFFFFFE, (u32)(char*)str_SFX_BTL_THUNDERS_ATT_802f83ec, 0x00000000, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF,
    0x00000000, 0x00000000, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000014, 0xFFFFFFFE, 0x00000001, 0x0005005B,
    (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_F_1_804241a8, 0x0006005B, (u32)btlevtcmd_snd_se,
    0xFFFFFFFE, (u32)(char*)str_SFX_BTL_THUNDERS_DOW_802f844c, 0xF1194D80, 0x00000000, 0xF1194D80, 0x0004005B,
    (u32)btlevtcmd_FaceDirectionAdd, 0xFFFFFFFE, 0xFE363C80, 0x00000014, 0x0003005B, (u32)btlevtcmd_SetFallAccel,
    0xFFFFFFFE, 0xF24A7C80, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0000000C, 0xFFFFFFFF, 0x0001005B, (u32)btlevtcmd_StopAC,
    0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress, 0xFFFFFFFE, 0xFE363C80, 0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult,
    0xFE363C80, 0x00000000, 0x00010009, 0x0000003C, 0x0001005B, (u32)evt_audience_ap_recovery,
    0x0001005B, (u32)btlevtcmd_InviteApInfoReport, 0x00010004, 0x0000005F, 0x00000021, 0x0006005B,
    (u32)btlevtcmd_CommandPreCheckDamage, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0x00000100, 0xFE363C85,
    0x00020018, 0xFE363C85, 0x00000005, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE,
    0x00000001, (u32)(char*)str_R_2_80424198, 0x0003005B, (u32)btlevtcmd_GetStatusMg, 0xFFFFFFFE, 0xFE363C85,
    0x0002003C, 0xFE363C85, 0x00000023, 0x0007005B, (u32)_get_bomb_hit_position, 0xFFFFFFFE,
    0xFE363C83, 0xFE363C84, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B,
    (u32)btlevtcmd_FaceDirectionAdd, 0xFE363C83, 0xFE363C80, 0xFE363C85, 0x0004005B, (u32)btlevtcmd_TransStageFloorPosition,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0009005B, (u32)btlevtcmd_SetMoveSoundDataWork, 0xFFFFFFFE,
    (u32)(char*)str_SFX_BTL_THUNDERS_ATT_802f83ec, 0x00000000, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000,
    0x00000000, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C80, 0x00000000,
    0xFE363C82, 0x0000003C, 0xFFFFFFFE, 0x00000001, 0x0006005B, (u32)btlevtcmd_CommandCheckDamage,
    0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0x00000100, 0xFE363C85, 0x00000021,
    0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_R_2_80424198, 0x0003005B,
    (u32)btlevtcmd_GetStatusMg, 0xFFFFFFFE, 0xFE363C86, 0x0002003C, 0xFE363C86, 0x00000014,
    0x0007005B, (u32)_get_bomb_hit_position, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0004005B, (u32)btlevtcmd_FaceDirectionAdd, 0xFE363C83, 0xFE363C80,
    0xFE363C86, 0x0004005B, (u32)btlevtcmd_TransStageFloorPosition, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0009005B, (u32)btlevtcmd_SetMoveSoundDataWork, 0xFFFFFFFE, (u32)(char*)str_SFX_BTL_THUNDERS_ATT_802f83ec, 0x00000000, 0x00000000,
    0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00020035, 0xFE363C82,
    0x00000005, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C80, 0x00000000,
    0xFE363C82, 0x0000005A, 0xFFFFFFFE, 0x00000001, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv,
    0x00000000, 0x00000002, 0x0003005B, (u32)evt_btl_camera_set_zoom, 0x00000000, 0x0000012C,
    0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_BTL_THUNDERS_ATT_802f8408, 0xFE363C8E, 0x0004005B, (u32)btlevtcmd_AnimeChangePose,
    0xFFFFFFFE, 0x00000001, (u32)(char*)str_A_3_80424194, 0x00010009, 0x0000003C, 0x00010022,
    0xFE363C85, 0x0001002B, 0x00000002, 0x0001002B, 0x00000003, 0x0001002B,
    0x00000006, 0x0001002B, 0x00000004, 0x0000002E, 0x00010024, 0x00000001,
    0x0000002A, 0x0001000A, 0x000003E8, 0x00000031, 0x0001005B, (u32)btlevtcmd_ResultAC,
    0x0002005B, (u32)evt_snd_sfxoff, 0xFE363C8E, 0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_BTL_THUNDERS_ATT_802f8424,
    0x00000000, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0003005B, (u32)btlevtcmd_GetHeight, 0xFFFFFFFE, 0xFE363C86, 0x00020038,
    0xFE363C86, 0x00000002, 0x00020035, 0xFE363C81, 0xFE363C86, 0x0002005B,
    (u32)btlevtcmd_GetResultAC, 0xFE363C86, 0x0002001E, 0xFE363C86, 0x00000002, 0x000F005B,
    (u32)evt_eff64, (u32)(u8*)zero_8042419c, (u32)(char*)str_bomb_n64_802f8440, 0x00000001, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000020, 0x000F005B, (u32)evt_eff64,
    (u32)(u8*)zero_8042419c, (u32)(char*)str_bomb_n64_802f8440, 0x00000000, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000021, 0x0006005B, (u32)evt_btl_camera_shake_h, 0x00000000,
    0x00000005, 0x00000000, 0x0000000A, 0x0000000D, 0x0001005B, (u32)btlevtcmd_StageDispellFog,
    0x00020019, 0xFE363C85, 0x00000001, 0x00020018, 0xFE363C85, 0x00000003,
    0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000026, 0x00000021, 0x00020018,
    0xFE363C85, 0x00000006, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000027,
    0x00000021, 0x00020018, 0xFE363C85, 0x00000002, 0x0003005B, (u32)btlevtcmd_StartAvoid,
    0xFE363C83, 0x00000028, 0x00000021, 0x00010004, 0x0000005A, 0x00000021,
    0x0002005B, (u32)btlevtcmd_GetResultAC, 0xFE363C86, 0x0002001E, 0xFE363C86, 0x00000002,
    0x0006005B, (u32)btlevtcmd_CommandCheckDamage, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0x00020100,
    0xFE363C85, 0x0004005B, (u32)btlevtcmd_GetResultPrizeLv, 0xFE363C83, 0x00000000, 0xFE363C86,
    0x0006005B, (u32)btlevtcmd_GetHitPos, 0xFE363C83, 0xFE363C84, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0005005B, (u32)btlevtcmd_ACSuccessEffect, 0xFE363C86, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult, 0xFE363C8C, 0xFE363C86, 0x00000020,
    0x0006005B, (u32)btlevtcmd_CommandCheckDamage, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0x00000100,
    0xFE363C85, 0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult, 0xFE363C8C, 0xFFFFFFFF, 0x00000021,
    0x00010003, 0x0000005A, 0x0001005B, (u32)btlevtcmd_StopAC, 0x00010003, 0x0000005C,
    0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv,
    0x00000000, 0x00000001, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001,
    (u32)(char*)str_J_1A_80424188, 0x0000006E, 0x00010009, 0x0000001E, 0x0004005B, (u32)btlevtcmd_AnimeChangePose,
    0xFFFFFFFE, 0x00000001, (u32)(char*)str_J_1B_804241a0, 0x00000070, 0x0003005B, (u32)btlevtcmd_SetFallAccel,
    0xFFFFFFFE, 0xF24A7C19, 0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0001006F, 0xFE363C8F, 0x0007005B, (u32)btlevtcmd_JumpPosition,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0000005A, 0xFFFFFFFF,
    0x00000070, 0x00010009, 0x0000003C, 0x0001005F, 0xFE363C8F, 0x0005005B,
    (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C85, 0xFE363C86, 0xFE363C87, 0x0004005B,
    (u32)btlevtcmd_GetStageSize, 0xF1194D80, 0xFE363C88, 0xF1194D80, 0x00020035, 0xFE363C86,
    0xFE363C88, 0x00020038, 0xFE363C86, 0x00000002, 0x0005005B, (u32)btlevtcmd_SetPos,
    0xFFFFFFFE, 0xFE363C85, 0xFE363C86, 0xFE363C87, 0x0003005B, (u32)btlevtcmd_SetFallAccel,
    0xFFFFFFFE, 0xF24A7B80, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0000001E, 0xFFFFFFFF, 0x0003005B, (u32)btlevtcmd_SetFallAccel,
    0xFFFFFFFE, 0xF24A7D4C, 0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0001006F, 0xFE363C8F, 0x0007005B, (u32)btlevtcmd_JumpPosition,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000010, 0xFFFFFFFF,
    0x00000070, 0x0006005B, (u32)btlevtcmd_ACRStart, 0xFFFFFFFE, 0x00000000, 0x0000000F,
    0x0000000F, 0x0000000F, 0x0003005B, (u32)btlevtcmd_ACRGetResult, 0xFE363C86, 0xFE363C87,
    0x0002005B, (u32)btlevtcmd_WaitEventEnd, 0xFE363C8F, 0x0002001D, 0xFE363C86, 0x00000002,
    0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_Y_1_804241ac, 0x0003005B,
    (u32)btlevtcmd_CommandGetWeaponAddress, 0xFFFFFFFE, 0xFE363C80, 0x0006005B, (u32)btlevtcmd_AudienceDeclareAcrobatResult, 0xFE363C80,
    0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x0001006F, 0xFE363C8F,
    0x0005005B, (u32)evt_sub_intpl_init_float, 0x0000000B, 0xF24A7E80, 0xF24A7CE6, 0x00000008,
    0x00010005, 0x00000008, 0x0003005B, (u32)evt_sub_intpl_get_float, 0xFE363C85, 0xF1194D80,
    0x0005005B, (u32)btlevtcmd_GetScale, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0005005B, (u32)btlevtcmd_SetScale, 0xFFFFFFFE, 0xFE363C80, 0xFE363C85, 0xFE363C82,
    0x00010009, 0x00000001, 0x00000006, 0x0005005B, (u32)evt_sub_intpl_init_float, 0x0000000B,
    0xF24A7CE6, 0xF24A7F4C, 0x00000008, 0x00010005, 0x00000008, 0x0003005B,
    (u32)evt_sub_intpl_get_float, 0xFE363C85, 0xF1194D80, 0x0005005B, (u32)btlevtcmd_GetScale, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)btlevtcmd_SetScale, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C85, 0xFE363C82, 0x00010009, 0x00000001, 0x00000006,
    0x0005005B, (u32)evt_sub_intpl_init_float, 0x0000000B, 0xF24A7F4C, 0xF24A7E80, 0x00000006,
    0x00010005, 0x00000006, 0x0003005B, (u32)evt_sub_intpl_get_float, 0xFE363C85, 0xF1194D80,
    0x0005005B, (u32)btlevtcmd_GetScale, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0005005B, (u32)btlevtcmd_SetScale, 0xFFFFFFFE, 0xFE363C80, 0xFE363C85, 0xFE363C82,
    0x00010009, 0x00000001, 0x00000006, 0x00000070, 0x0000006E, 0x0005005B,
    (u32)evt_sub_intpl_init_float, 0x0000000B, 0xF24A7E80, 0xF24A8080, 0x00000008, 0x00010005,
    0x00000008, 0x0003005B, (u32)evt_sub_intpl_get_float, 0xFE363C85, 0xF1194D80, 0x0005005B,
    (u32)btlevtcmd_GetScale, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B,
    (u32)btlevtcmd_SetScale, 0xFFFFFFFE, 0xFE363C85, 0xFE363C81, 0xFE363C82, 0x00010009,
    0x00000001, 0x00000006, 0x0005005B, (u32)evt_sub_intpl_init_float, 0x0000000B, 0xF24A8080,
    0xF24A7CE6, 0x00000008, 0x00010005, 0x00000008, 0x0003005B, (u32)evt_sub_intpl_get_float,
    0xFE363C85, 0xF1194D80, 0x0005005B, (u32)btlevtcmd_GetScale, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0005005B, (u32)btlevtcmd_SetScale, 0xFFFFFFFE, 0xFE363C85,
    0xFE363C81, 0xFE363C82, 0x00010009, 0x00000001, 0x00000006, 0x0005005B,
    (u32)evt_sub_intpl_init_float, 0x0000000B, 0xF24A7CE6, 0xF24A7E80, 0x00000006, 0x00010005,
    0x00000006, 0x0003005B, (u32)evt_sub_intpl_get_float, 0xFE363C85, 0xF1194D80, 0x0005005B,
    (u32)btlevtcmd_GetScale, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B,
    (u32)btlevtcmd_SetScale, 0xFFFFFFFE, 0xFE363C85, 0xFE363C81, 0xFE363C82, 0x00010009,
    0x00000001, 0x00000006, 0x00000070, 0x0002005B, (u32)btlevtcmd_WaitEventEnd, 0xFE363C8F,
    0x00010009, 0x00000028, 0x00000020, 0x0001005B, (u32)evt_audience_acrobat_notry, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_F_1_804241a8, 0x0006005B, (u32)btlevtcmd_snd_se,
    0xFFFFFFFE, (u32)(char*)str_SFX_BTL_THUNDERS_DOW_802f844c, 0xF1194D80, 0x00000000, 0xF1194D80, 0x00010009,
    0x00000028, 0x00000021, 0x0001005B, (u32)evt_audience_ap_recovery, 0x0001005B, (u32)btlevtcmd_InviteApInfoReport,
    0x00010003, 0x0000005F, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000,
    0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001, 0x0003005B, (u32)btlevtcmd_SetMoveSpeed,
    0xFFFFFFFE, 0xF24A9280, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001,
    0x00000028, 0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000000, 0xFFFFFFFF, 0x00000000, 0x00010003, 0x00000063,
    0x0002005B, (u32)btlevtcmd_ResetFaceDirection, 0xFFFFFFFE, 0x0001005E, (u32)btldefaultevt_SuitoruBadgeEffect, 0x0002005B,
    (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002, 0x00000001
};

u32 partySandersAttack_SuperBombAttack[] = {
    0x0003005B, (u32)btlevtcmd_GetSelectEnemy, 0xFE363C83, 0xFE363C84, 0x00020018, 0xFE363C83,
    0xFFFFFFFF, 0x00010004, 0x00000063, 0x00000021, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress,
    0xFFFFFFFE, 0xFE363C8C, 0x0002005B, (u32)btlevtcmd_WeaponAftereffect, 0xFE363C8C, 0x0002005B,
    (u32)btlevtcmd_AttackDeclareAll, 0xFFFFFFFE, 0x0001005B, (u32)btlevtcmd_WaitGuardMove, 0x0002005B, (u32)btlevtcmd_CommandPayWeaponCost,
    0xFFFFFFFE, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFFFFFFFE, 0x00000007, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_Y_1_804241ac, 0x0003005B, (u32)evt_btl_camera_set_mode,
    0x00000000, 0x0000000B, 0x0006005B, (u32)evt_btl_camera_set_homing_unitparts, 0x00000000, 0xFFFFFFFE,
    0x00000001, 0xFE363C83, 0xFE363C84, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000,
    0x00000002, 0x0003005B, (u32)evt_btl_camera_set_zoom, 0x00000000, 0x000000C8, 0x0002005B,
    (u32)btlevtcmd_CommandGetWeaponActionLv, 0xFE363C80, 0x0003005B, (u32)btlevtcmd_AcSetDifficulty, 0xFFFFFFFE, 0xFE363C80,
    0x0002005B, (u32)btlevtcmd_AcGetDifficulty, 0xFE363C80, 0x00020036, 0xFE363C80, 0x00000003,
    0x00010022, 0xFE363C80, 0x00010028, 0xFFFFFFFD, 0x00020032, 0xFE363C80,
    0x0000012C, 0x00020032, 0xFE363C81, 0x00000014, 0x00020032, 0xFE363C82,
    0x00000014, 0x00010024, 0xFFFFFFFE, 0x00020032, 0xFE363C80, 0x00000172,
    0x00020032, 0xFE363C81, 0x00000012, 0x00020032, 0xFE363C82, 0x00000016,
    0x00010024, 0xFFFFFFFF, 0x00020032, 0xFE363C80, 0x000000FF, 0x00020032,
    0xFE363C81, 0x00000011, 0x00020032, 0xFE363C82, 0x00000017, 0x00010024,
    0x00000000, 0x00020032, 0xFE363C80, 0x000000E1, 0x00020032, 0xFE363C81,
    0x00000010, 0x00020032, 0xFE363C82, 0x00000018, 0x00010024, 0x00000001,
    0x00020032, 0xFE363C80, 0x000000D2, 0x00020032, 0xFE363C81, 0x0000000F,
    0x00020032, 0xFE363C82, 0x0000001A, 0x00010024, 0x00000002, 0x00020032,
    0xFE363C80, 0x000000B4, 0x00020032, 0xFE363C81, 0x0000000E, 0x00020032,
    0xFE363C82, 0x0000001C, 0x0000002A, 0x00020032, 0xFE363C80, 0x00000096,
    0x00020032, 0xFE363C81, 0x0000000D, 0x00020032, 0xFE363C82, 0x0000001C,
    0x00000031, 0x0009005B, (u32)btlevtcmd_AcSetParamAll, 0x00000001, 0xFE363C80, 0x000000B2,
    0xFE363C81, 0xFE363C82, 0x0000000F, 0x0000002E, 0xF1194D80, 0x0005005B,
    (u32)btlevtcmd_AcSetGaugeParam, 0x0000002D, 0x0000004B, 0x00000064, 0x00000064, 0x0002005B,
    (u32)btlevtcmd_AcSetFlag, 0x00000040, 0x0005005B, (u32)btlevtcmd_SetupAC, 0xFFFFFFFE, 0x00000006,
    0x00000001, 0x00000000, 0x00010009, 0x00000016, 0x0002005B, (u32)btlevtcmd_StartAC,
    0x00000001, 0x0006005B, (u32)btlevtcmd_CommandPreCheckDamage, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84,
    0x00000100, 0xFE363C85, 0x00020018, 0xFE363C85, 0x00000005, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_R_2_80424198, 0x0003005B, (u32)btlevtcmd_GetStatusMg,
    0xFFFFFFFE, 0xFE363C86, 0x0002003C, 0xFE363C86, 0x00000023, 0x0007005B,
    (u32)_get_bomb_hit_position, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0004005B, (u32)btlevtcmd_FaceDirectionAdd, 0xFE363C83, 0xFE363C80, 0xFE363C86,
    0x0004005B, (u32)btlevtcmd_TransStageFloorPosition, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0009005B,
    (u32)btlevtcmd_SetMoveSoundDataWork, 0xFFFFFFFE, (u32)(char*)str_SFX_BTL_THUNDERS_ATT_802f83ec, 0x00000000, 0x00000000, 0xFFFFFFFF,
    0xFFFFFFFF, 0x00000000, 0x00000000, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE,
    0xFE363C80, 0x00000000, 0xFE363C82, 0x0000003C, 0xFFFFFFFE, 0x00000001,
    0x0005005B, (u32)btlevtcmd_SetScale, 0xFFFFFFFE, 0xF24A7E80, 0xF24A7E80, 0xF24A7E80,
    0x0006005B, (u32)btlevtcmd_CommandCheckDamage, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0x00000100,
    0xFE363C85, 0x00000002, 0x00000021, 0x0000006E, 0x00020036, 0xFE363C80,
    0x00000055, 0x00010009, 0xFE363C80, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000F005B, (u32)evt_eff, (u32)(u8*)zero_8042419c,
    (u32)(char*)str_sandars_802f8464, 0x00000005, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0000003C,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00010009, 0x0000000F, 0x000F005B, (u32)evt_eff, (u32)(u8*)zero_8042419c,
    (u32)(char*)str_sandars_802f8464, 0x00000005, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000096,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00010009, 0x0000000F, 0x000F005B, (u32)evt_eff, (u32)(u8*)zero_8042419c,
    (u32)(char*)str_sandars_802f8464, 0x00000005, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0000001E,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00010009, 0x0000000F, 0x000F005B, (u32)evt_eff, (u32)(u8*)zero_8042419c,
    (u32)(char*)str_sandars_802f8464, 0x00000005, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000078,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00010009, 0x00000028, 0x00000070, 0x0000006E, 0x00020036,
    0xFE363C80, 0x00000055, 0x00010009, 0xFE363C80, 0x0003005B, (u32)evt_snd_sfxon,
    (u32)(char*)str_SFX_BTL_THUNDERS_ATT_802f8408, 0xFE363C8E, 0x00020032, 0xFE363C80, 0x00000000, 0x00010005,
    0x00000050, 0x00020035, 0xFE363C80, 0x00000001, 0x0002001B, 0xFE363C80,
    0x00000050, 0x00020032, 0xFE363C80, 0x00000050, 0x00000021, 0x00020032,
    0xFE363C81, 0x00001000, 0x00020037, 0xFE363C81, 0xFE363C80, 0x00020038,
    0xFE363C81, 0x00000050, 0x0003005B, (u32)evt_snd_sfx_pit, 0xFE363C8E, 0xFE363C81,
    0x00010009, 0x00000001, 0x00000006, 0x00010009, 0x00000003, 0x0002005B,
    (u32)evt_snd_sfxoff, 0xFE363C8E, 0x00000070, 0x0000006E, 0x00010005, 0xFE363C80,
    0x0003005B, (u32)btlevtcmd_AcGetOutputParam, 0x00000001, 0xFE363C80, 0x00020035, 0xFE363C80,
    0x00000064, 0x0002003C, 0xFE363C80, 0xF24A7A8A, 0x0005005B, (u32)btlevtcmd_SetScale,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C80, 0xF24A7E80, 0x00010009, 0x00000001,
    0x00000006, 0x00000070, 0x00020032, 0xFE363C8D, 0xFE363C80, 0x00020036,
    0xFE363C8D, 0x00000055, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001,
    (u32)(char*)str_R_2_80424198, 0x0003005B, (u32)btlevtcmd_GetStatusMg, 0xFFFFFFFE, 0xFE363C86, 0x0002003C,
    0xFE363C86, 0x00000014, 0x0007005B, (u32)_get_bomb_hit_position, 0xFFFFFFFE, 0xFE363C83,
    0xFE363C84, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B, (u32)btlevtcmd_FaceDirectionAdd,
    0xFE363C83, 0xFE363C80, 0xFE363C86, 0x0004005B, (u32)btlevtcmd_TransStageFloorPosition, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0009005B, (u32)btlevtcmd_SetMoveSoundDataWork, 0xFFFFFFFE, (u32)(char*)str_SFX_BTL_THUNDERS_ATT_802f83ec,
    0x00000000, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x00000000,
    0x00020035, 0xFE363C82, 0x00000005, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0xFE363C8D, 0xFFFFFFFE, 0x00000001,
    0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000002, 0x0003005B, (u32)evt_btl_camera_set_zoom,
    0x00000000, 0x0000012C, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001,
    (u32)(char*)str_A_3_80424194, 0x0001005B, (u32)btlevtcmd_ResultAC, 0x0001005B, (u32)btlevtcmd_StopAC, 0x00010022,
    0xFE363C85, 0x0001002B, 0x00000002, 0x0001002B, 0x00000003, 0x0001002B,
    0x00000006, 0x0001002B, 0x00000004, 0x0000002E, 0x00010024, 0x00000001,
    0x0000002A, 0x0001000A, 0x000003E8, 0x00000031, 0x0001005B, (u32)btlevtcmd_ResultAC,
    0x0005005B, (u32)btlevtcmd_SetScale, 0xFFFFFFFE, 0xF24A7E80, 0xF24A7E80, 0xF24A7E80,
    0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_BTL_THUNDERS_ATT_802f846c, 0x00000000, 0x0005005B, (u32)btlevtcmd_GetPos,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0002005B, (u32)btlevtcmd_GetResultAC,
    0xFE363C86, 0x0002001E, 0xFE363C86, 0x00000002, 0x000F005B, (u32)evt_eff,
    (u32)(u8*)zero_8042419c, (u32)(char*)str_sandars_802f8464, 0x00000003, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000020, 0x000F005B, (u32)evt_eff, (u32)(u8*)zero_8042419c,
    (u32)(char*)str_sandars_802f8464, 0x00000004, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000021, 0x0001005B, (u32)btlevtcmd_StageDispellFog, 0x0006005B, (u32)evt_btl_camera_shake_h,
    0x00000000, 0x0000000A, 0x00000000, 0x00000014, 0x0000000D, 0x0003005B,
    (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000,
    0x00000002, 0x00020032, 0xFE363C8A, 0x00000000, 0x00010003, 0x0000000A,
    0x0006005B, (u32)btlevtcmd_CommandPreCheckDamage, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0x00000100,
    0xFE363C86, 0x00020019, 0xFE363C86, 0x00000001, 0x00020018, 0xFE363C86,
    0x00000003, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000026, 0x00000021,
    0x00020018, 0xFE363C86, 0x00000006, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83,
    0x00000027, 0x00000021, 0x00020018, 0xFE363C86, 0x00000002, 0x0003005B,
    (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000028, 0x00000021, 0x00010004, 0x00000032,
    0x00000021, 0x0002005B, (u32)btlevtcmd_GetResultAC, 0xFE363C80, 0x0002001E, 0xFE363C80,
    0x00000002, 0x0003005B, (u32)btlevtcmd_AcGetOutputParam, 0x00000002, 0xFE363C80, 0x00010022,
    0xFE363C80, 0x00010028, 0x00000003, 0x00020032, 0xFE363C80, 0xFFFFFFFF,
    0x00010028, 0x00000005, 0x00020032, 0xFE363C80, 0x00000000, 0x00010028,
    0x00000007, 0x00020032, 0xFE363C80, 0x00000001, 0x0000002A, 0x00020032,
    0xFE363C80, 0x00000002, 0x00000031, 0x0002001D, 0xFE363C80, 0x00000000,
    0x0006005B, (u32)btlevtcmd_CommandCheckDamage, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0x00020100,
    0xFE363C85, 0x0004005B, (u32)btlevtcmd_GetResultPrizeLv, 0xFE363C83, 0xFE363C80, 0xFE363C86,
    0x0006005B, (u32)btlevtcmd_GetHitPos, 0xFE363C83, 0xFE363C84, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0005005B, (u32)btlevtcmd_ACSuccessEffect, 0xFE363C86, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress, 0xFFFFFFFE, 0xFE363C80, 0x0003005B,
    (u32)btlevtcmd_AudienceDeclareACResult, 0xFE363C80, 0xFE363C86, 0x00000020, 0x0006005B, (u32)btlevtcmd_CommandCheckDamage,
    0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0x00000100, 0xFE363C85, 0x0003005B,
    (u32)btlevtcmd_CommandGetWeaponAddress, 0xFFFFFFFE, 0xFE363C80, 0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult, 0xFE363C80,
    0xFFFFFFFF, 0x00000021, 0x00000020, 0x0006005B, (u32)btlevtcmd_CommandCheckDamage, 0xFFFFFFFE,
    0xFE363C83, 0xFE363C84, 0x00000100, 0xFE363C85, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress,
    0xFFFFFFFE, 0xFE363C80, 0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult, 0xFE363C80, 0xFFFFFFFF,
    0x00000021, 0x00010003, 0x00000032, 0x0003005B, (u32)btlevtcmd_GetSelectNextEnemy, 0xFE363C83,
    0xFE363C84, 0x00020019, 0xFE363C83, 0xFFFFFFFF, 0x00020035, 0xFE363C8A,
    0x00000001, 0x00010004, 0x0000000A, 0x00000021, 0x00010003, 0x0000005A,
    0x0001005B, (u32)btlevtcmd_StopAC, 0x00010003, 0x0000005C, 0x0003005B, (u32)evt_btl_camera_set_mode,
    0x00000000, 0x00000000, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001,
    0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_J_1A_80424188, 0x0000006E,
    0x00010009, 0x0000001E, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001,
    (u32)(char*)str_J_1B_804241a0, 0x0005005B, (u32)btlevtcmd_SetScale, 0xFFFFFFFE, 0xF24A7E80, 0xF24A7E80,
    0xF24A7E80, 0x00000070, 0x0003005B, (u32)btlevtcmd_SetFallAccel, 0xFFFFFFFE, 0xF24A7C19,
    0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0001006F, 0xFE363C8F, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0000005A, 0xFFFFFFFF, 0x00000070, 0x00010009,
    0x0000003C, 0x0001005F, 0xFE363C8F, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE,
    0xFE363C85, 0xFE363C86, 0xFE363C87, 0x0004005B, (u32)btlevtcmd_GetStageSize, 0xF1194D80,
    0xFE363C88, 0xF1194D80, 0x00020035, 0xFE363C86, 0xFE363C88, 0x00020038,
    0xFE363C86, 0x00000002, 0x0005005B, (u32)btlevtcmd_SetPos, 0xFFFFFFFE, 0xFE363C85,
    0xFE363C86, 0xFE363C87, 0x0003005B, (u32)btlevtcmd_SetFallAccel, 0xFFFFFFFE, 0xF24A7B80,
    0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0000001E, 0xFFFFFFFF, 0x0003005B, (u32)btlevtcmd_SetFallAccel, 0xFFFFFFFE, 0xF24A7D4C,
    0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0001006F, 0xFE363C8F, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000010, 0xFFFFFFFF, 0x00000070, 0x0006005B,
    (u32)btlevtcmd_ACRStart, 0xFFFFFFFE, 0x00000000, 0x0000000F, 0x0000000F, 0x0000000F,
    0x0003005B, (u32)btlevtcmd_ACRGetResult, 0xFE363C86, 0xFE363C87, 0x0002005B, (u32)btlevtcmd_WaitEventEnd,
    0xFE363C8F, 0x0002001D, 0xFE363C86, 0x00000002, 0x0004005B, (u32)btlevtcmd_AnimeChangePose,
    0xFFFFFFFE, 0x00000001, (u32)(char*)str_Y_1_804241ac, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress, 0xFFFFFFFE,
    0xFE363C80, 0x0006005B, (u32)btlevtcmd_AudienceDeclareAcrobatResult, 0xFE363C80, 0x00000001, 0x00000000,
    0x00000000, 0x00000000, 0x0001006F, 0xFE363C8F, 0x0005005B, (u32)evt_sub_intpl_init_float,
    0x0000000B, 0xF24A7E80, 0xF24A7CE6, 0x00000008, 0x00010005, 0x00000008,
    0x0003005B, (u32)evt_sub_intpl_get_float, 0xFE363C85, 0xF1194D80, 0x0005005B, (u32)btlevtcmd_GetScale,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)btlevtcmd_SetScale,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C85, 0xFE363C82, 0x00010009, 0x00000001,
    0x00000006, 0x0005005B, (u32)evt_sub_intpl_init_float, 0x0000000B, 0xF24A7CE6, 0xF24A7F4C,
    0x00000008, 0x00010005, 0x00000008, 0x0003005B, (u32)evt_sub_intpl_get_float, 0xFE363C85,
    0xF1194D80, 0x0005005B, (u32)btlevtcmd_GetScale, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0005005B, (u32)btlevtcmd_SetScale, 0xFFFFFFFE, 0xFE363C80, 0xFE363C85,
    0xFE363C82, 0x00010009, 0x00000001, 0x00000006, 0x0005005B, (u32)evt_sub_intpl_init_float,
    0x0000000B, 0xF24A7F4C, 0xF24A7E80, 0x00000006, 0x00010005, 0x00000006,
    0x0003005B, (u32)evt_sub_intpl_get_float, 0xFE363C85, 0xF1194D80, 0x0005005B, (u32)btlevtcmd_GetScale,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)btlevtcmd_SetScale,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C85, 0xFE363C82, 0x00010009, 0x00000001,
    0x00000006, 0x00000070, 0x0000006E, 0x0005005B, (u32)evt_sub_intpl_init_float, 0x0000000B,
    0xF24A7E80, 0xF24A8080, 0x00000008, 0x00010005, 0x00000008, 0x0003005B,
    (u32)evt_sub_intpl_get_float, 0xFE363C85, 0xF1194D80, 0x0005005B, (u32)btlevtcmd_GetScale, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)btlevtcmd_SetScale, 0xFFFFFFFE,
    0xFE363C85, 0xFE363C81, 0xFE363C82, 0x00010009, 0x00000001, 0x00000006,
    0x0005005B, (u32)evt_sub_intpl_init_float, 0x0000000B, 0xF24A8080, 0xF24A7CE6, 0x00000008,
    0x00010005, 0x00000008, 0x0003005B, (u32)evt_sub_intpl_get_float, 0xFE363C85, 0xF1194D80,
    0x0005005B, (u32)btlevtcmd_GetScale, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0005005B, (u32)btlevtcmd_SetScale, 0xFFFFFFFE, 0xFE363C85, 0xFE363C81, 0xFE363C82,
    0x00010009, 0x00000001, 0x00000006, 0x0005005B, (u32)evt_sub_intpl_init_float, 0x0000000B,
    0xF24A7CE6, 0xF24A7E80, 0x00000006, 0x00010005, 0x00000006, 0x0003005B,
    (u32)evt_sub_intpl_get_float, 0xFE363C85, 0xF1194D80, 0x0005005B, (u32)btlevtcmd_GetScale, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)btlevtcmd_SetScale, 0xFFFFFFFE,
    0xFE363C85, 0xFE363C81, 0xFE363C82, 0x00010009, 0x00000001, 0x00000006,
    0x00000070, 0x0002005B, (u32)btlevtcmd_WaitEventEnd, 0xFE363C8F, 0x00010009, 0x00000028,
    0x00000020, 0x0001005B, (u32)evt_audience_acrobat_notry, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE,
    0x00000001, (u32)(char*)str_F_1_804241a8, 0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_BTL_THUNDERS_DOW_802f844c,
    0xF1194D80, 0x00000000, 0xF1194D80, 0x00010009, 0x00000028, 0x00000021,
    0x00010003, 0x0000005F, 0x0001005B, (u32)evt_audience_ap_recovery, 0x0001005B, (u32)btlevtcmd_InviteApInfoReport,
    0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv,
    0x00000000, 0x00000001, 0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE, 0xF24A9280,
    0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001, 0x00000028, 0x0005005B,
    (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0008005B,
    (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000,
    0xFFFFFFFF, 0x00000000, 0x00010003, 0x00000063, 0x0002005B, (u32)btlevtcmd_ResetFaceDirection,
    0xFFFFFFFE, 0x0001005E, (u32)btldefaultevt_SuitoruBadgeEffect, 0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE,
    0x00000002, 0x00000001
};

u32 partySandersAttack_CounterSet[] = {
    0x0005005B, (u32)btlevtcmd_JumpSetting, 0xFFFFFFFE, 0x00000014, 0xF24A7A80, 0xF24A7D4C,
    0x0003005B, (u32)btlevtcmd_GetSelectEnemy, 0xFE363C83, 0xFE363C84, 0x00020018, 0xFE363C83,
    0xFFFFFFFF, 0x00010004, 0x00000063, 0x00000021, 0x00020032, 0xFE363C8C,
    (u32)partyWeapon_SandersCounterSet, 0x0003005B, (u32)btlevtcmd_PayWeaponCost, 0xFFFFFFFE, 0xFE363C8C, 0x0003005B,
    (u32)btlevtcmd_RunDataEventChild, 0xFFFFFFFE, 0x00000007, 0x00010009, 0x00000014, 0x0005005B,
    (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xF1194D80, 0xF1194D80, 0x0005005B,
    (u32)btlevtcmd_GetPos, 0xFFFFFFFD, 0xFE363C81, 0xF1194D80, 0xF1194D80, 0x00020035,
    0xFE363C80, 0xFE363C81, 0x00020038, 0xFE363C80, 0x00000002, 0x0003005B,
    (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000003, 0x000A005B, (u32)evt_btl_camera_set_moveto, 0x00000001,
    0xFE363C80, 0x00000049, 0x00000190, 0xFE363C80, 0x0000002B, 0x00000000,
    0x0000001E, 0x00000005, 0x0003005B, (u32)btlevtcmd_GetBodyId, 0xFFFFFFFE, 0xFE363C89,
    0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0xFE363C89, (u32)(char*)str_O_1_80424180, 0x0002005B,
    (u32)btlevtcmd_CommandGetWeaponActionLv, 0xFE363C80, 0x0003005B, (u32)btlevtcmd_AcSetDifficulty, 0xFFFFFFFE, 0xFE363C80,
    0x0009005B, (u32)btlevtcmd_AcSetParamAll, 0x00000003, 0x00000000, 0x00000028, 0x00000001,
    0x00000000, 0x00000001, 0x00000002, 0x00000000, 0x0002005B, (u32)btlevtcmd_AcSetFlag,
    0x00000008, 0x0005005B, (u32)btlevtcmd_SetupAC, 0xFFFFFFFE, 0x00000012, 0x00000001,
    0x00000000, 0x00010009, 0x00000034, 0x0003005B, (u32)btlevtcmd_AnimeWaitPlayComplete, 0xFFFFFFFE,
    0x00000001, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_S_2_804241b0,
    0x0002005B, (u32)btlevtcmd_StartAC, 0x00000001, 0x0001005B, (u32)btlevtcmd_ResultAC, 0x0002005B,
    (u32)btlevtcmd_GetResultAC, 0xFE363C80, 0x0001005B, (u32)btlevtcmd_StopAC, 0x0002001F, 0xFE363C80,
    0x00000002, 0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult, 0xFE363C8C, 0xFFFFFFFF, 0x00000020,
    0x0003005B, (u32)btlevtcmd_AcGetOutputParam, 0x00000000, 0xFE363C80, 0x00020036, 0xFE363C80,
    0x00000001, 0x0004005B, (u32)btlevtcmd_GetResultPrizeLv, 0xFFFFFFFF, 0xFE363C80, 0xFE363C86,
    0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0005005B, (u32)btlevtcmd_ACSuccessEffect, 0xFE363C86, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult, 0xFE363C8C, 0xFE363C86, 0x00000021, 0x0006005B,
    (u32)btlevtcmd_ACRStart, 0xFFFFFFFE, 0x00000000, 0x0000000C, 0x0000000C, 0x00000000,
    0x0003005B, (u32)btlevtcmd_ACRGetResult, 0xFE363C86, 0xFE363C87, 0x00010022, 0xFE363C86,
    0x00010029, 0x00000002, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress, 0xFFFFFFFE, 0xFE363C80,
    0x0006005B, (u32)btlevtcmd_AudienceDeclareAcrobatResult, 0xFE363C80, 0x00000001, 0x00000000, 0x00000000,
    0x00000000, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_Y_1_804241ac,
    0x00010009, 0x0000001E, 0x0000002A, 0x0001005B, (u32)evt_audience_acrobat_notry, 0x00000031,
    0x0003005B, (u32)btlevtcmd_AcGetOutputParam, 0x00000000, 0xFE363C80, 0x00020035, 0xFE363C80,
    0x00000001, 0x0003005B, (u32)_make_counterset_weapon, 0xFE363C8C, 0xFE363C80, 0x00010009,
    0x00000014, 0x0000006B, 0x00010009, 0x0000001E, 0x0003005B, (u32)evt_btl_camera_set_mode,
    0x00000000, 0x00000000, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001,
    0x0000006D, 0x0001005B, (u32)evt_audience_ap_recovery, 0x0001005B, (u32)btlevtcmd_InviteApInfoReport, 0x0007005B,
    (u32)btlevtcmd_CheckDamage, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0xFE363C8C, 0x00000100,
    0xFE363C85, 0x00010009, 0x00000050, 0x00010003, 0x00000063, 0x0001005B,
    (u32)evt_audience_ap_recovery, 0x0001005B, (u32)btlevtcmd_InviteApInfoReport, 0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE,
    0x00000002, 0x00000001
};

u32 partySandersAttack_TimeBombSet[] = {
    0x0002005B, (u32)btlevtcmd_CommandPayWeaponCost, 0xFFFFFFFE, 0x0004005B, (u32)btlevtcmd_SpawnUnit, 0xFE363C83,
    (u32)entry_bomzou, 0x00000000, 0x0003005B, (u32)btlevtcmd_OnAttribute, 0xFE363C83, 0x01000000,
    0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0005005B, (u32)btlevtcmd_SetPos, 0xFE363C83, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00020032, 0xFE363C8A, 0xFE363C83, 0x0004005B, (u32)btlevtcmd_SpawnUnit, 0xFE363C83,
    (u32)entry_bomzou, 0x00000000, 0x0003005B, (u32)btlevtcmd_OnAttribute, 0xFE363C83, 0x01000000,
    0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0005005B, (u32)btlevtcmd_SetPos, 0xFE363C83, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00020032, 0xFE363C8B, 0xFE363C83, 0x0004005B, (u32)btlevtcmd_SpawnUnit, 0xFE363C83,
    (u32)entry_bomzou, 0x00000000, 0x0003005B, (u32)btlevtcmd_OnAttribute, 0xFE363C83, 0x01000000,
    0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0005005B, (u32)btlevtcmd_SetPos, 0xFE363C83, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00020032, 0xFE363C8C, 0xFE363C83, 0x0002005B, (u32)btlevtcmd_CommandGetWeaponActionLv, 0xFE363C80,
    0x0003005B, (u32)btlevtcmd_AcSetDifficulty, 0xFFFFFFFE, 0xFE363C80, 0x0002005B, (u32)btlevtcmd_AcGetDifficulty,
    0xFE363C80, 0x00020036, 0xFE363C80, 0x00000003, 0x00010022, 0xFE363C80,
    0x00010028, 0xFFFFFFFD, 0x00020032, 0xFE363C81, 0x00000050, 0x00020032,
    0xFE363C82, 0x0000000C, 0x00010024, 0xFFFFFFFE, 0x00020032, 0xFE363C81,
    0x00000050, 0x00020032, 0xFE363C82, 0x0000000F, 0x00010024, 0xFFFFFFFF,
    0x00020032, 0xFE363C81, 0x00000050, 0x00020032, 0xFE363C82, 0x00000014,
    0x00010024, 0x00000000, 0x00020032, 0xFE363C81, 0x00000050, 0x00020032,
    0xFE363C82, 0x00000016, 0x00010024, 0x00000001, 0x00020032, 0xFE363C81,
    0x00000050, 0x00020032, 0xFE363C82, 0x00000020, 0x00010024, 0x00000002,
    0x00020032, 0xFE363C81, 0x0000005A, 0x00020032, 0xFE363C82, 0x00000032,
    0x0000002A, 0x00020032, 0xFE363C81, 0x00000078, 0x00020032, 0xFE363C82,
    0x00000050, 0x00000031, 0x0003005B, (u32)btlevtcmd_GetUnitId, 0xFFFFFFFE, 0xFE363C80,
    0x0009005B, (u32)btlevtcmd_AcSetParamAll, 0xFE363C80, 0x00000003, 0xFE363C81, 0xFE363C81,
    0x00000000, 0x0000005A, 0xFE363C82, 0xF1194D80, 0x0005005B, (u32)btlevtcmd_SetupAC,
    0xFFFFFFFE, 0x00000007, 0x00000001, 0x00000000, 0x0002005B, (u32)btlevtcmd_AcSetFlag,
    0x00000001, 0x00010009, 0x00000014, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE,
    0x00000001, (u32)(char*)str_A_4A_804241b4, 0x0002005B, (u32)btlevtcmd_StartAC, 0x00000001, 0x0001006F,
    0xFE363C8F, 0x00020032, 0xFE363C8D, 0x00000000, 0x00020032, 0xFE363C8E,
    0x00000005, 0x00010005, 0xFE363C8E, 0x0003005B, (u32)btlevtcmd_AcGetOutputParam, 0x00000000,
    0xFE363C80, 0x00020019, 0xFE363C80, 0x00000000, 0x00010022, 0xFE363C8D,
    0x00010024, 0x00000000, 0x00020032, 0xFE363C83, 0xFE363C8A, 0x00010024,
    0x00000001, 0x00020032, 0xFE363C83, 0xFE363C8B, 0x0000002A, 0x00020032,
    0xFE363C83, 0xFE363C8C, 0x00000031, 0x0003005B, (u32)btlevtcmd_OffAttribute, 0xFE363C83,
    0x01000000, 0x0003005B, (u32)btlevtcmd_AcGetOutputParam, 0x00000002, 0xFE363C80, 0x0003005B,
    (u32)btlevtcmd_AcGetOutputParam, 0x00000001, 0xFE363C81, 0x00020035, 0xFE363C81, 0x00000014,
    0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_A_4B_804241bc, 0x0002005D,
    (u32)_shot_bomb_event, 0xFE363C89, 0x00020035, 0xFE363C8D, 0x00000001, 0x0002001D,
    0xFE363C8D, 0x00000003, 0x0003005B, (u32)btlevtcmd_AnimeWaitPlayComplete, 0xFFFFFFFE, 0x00000001,
    0x00010009, 0x0000000A, 0x0006005B, (u32)btlevtcmd_ACRStart, 0xFFFFFFFE, 0x00000000,
    0x0000000C, 0x0000000C, 0x0000000A, 0x0003005B, (u32)btlevtcmd_ACRGetResult, 0xFE363C86,
    0xFE363C87, 0x00010022, 0xFE363C86, 0x00010029, 0x00000002, 0x0003005B,
    (u32)btlevtcmd_CommandGetWeaponAddress, 0xFFFFFFFE, 0xFE363C80, 0x0006005B, (u32)btlevtcmd_AudienceDeclareAcrobatResult, 0xFE363C80,
    0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x0004005B, (u32)btlevtcmd_AnimeChangePose,
    0xFFFFFFFE, 0x00000001, (u32)(char*)str_Y_1_804241ac, 0x0000002A, 0x0001005B, (u32)evt_audience_acrobat_notry,
    0x00000031, 0x0002005B, (u32)btlevtcmd_WaitEventEnd, 0xFE363C89, 0x00000007, 0x00000021,
    0x0003005B, (u32)btlevtcmd_AnimeWaitPlayComplete, 0xFFFFFFFE, 0x00000001, 0x0004005B, (u32)btlevtcmd_AnimeChangePose,
    0xFFFFFFFE, 0x00000001, (u32)(char*)str_A_4A_804241b4, 0x0003005B, (u32)btlevtcmd_AcSetOutputParam, 0x00000000,
    0x00000000, 0x00000021, 0x00010009, 0x00000001, 0x00020032, 0xFE363C8E,
    0x00000005, 0x00000006, 0x00010071, (u32)(char*)str_event_end_802f8488, 0x00000070, 0x0001005B,
    (u32)btlevtcmd_ResultAC, 0x00010009, 0x0000003C, 0x0001005B, (u32)btlevtcmd_StopAC, 0x0002005B,
    (u32)btlevtcmd_WaitEventEnd, 0xFE363C8F, 0x00010003, 0x0000005A, 0x0001005B, (u32)evt_audience_ap_recovery,
    0x0001005B, (u32)btlevtcmd_InviteApInfoReport, 0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002,
    0x00000001
};

u32 _shot_bomb_event[] = {
    0x0008005B, (u32)_shot_move, 0xFE363C83, 0xFE363C80, 0xFE363C81, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0xFE363C85, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFE363C83,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00020034, 0xFE363C86, 0xFE363C85,
    0x0002003C, 0xFE363C86, 0xF24A9419, 0x0004005B, (u32)btlevtcmd_FaceDirectionAdd, 0xFE363C83,
    0xFE363C80, 0xFE363C86, 0x0003005B, (u32)btlevtcmd_SetFallAccel, 0xFE363C83, 0xF24A7C80,
    0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFE363C83, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00000010, 0xFFFFFFFF, 0x00020034, 0xFE363C86, 0xFE363C85, 0x0002003C,
    0xFE363C86, 0xF24A88E6, 0x0004005B, (u32)btlevtcmd_FaceDirectionAdd, 0xFE363C83, 0xFE363C80,
    0xFE363C86, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFE363C83, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0000000C, 0xFFFFFFFF, 0x00020034, 0xFE363C86, 0xFE363C85,
    0x0002003C, 0xFE363C86, 0xF24A8A80, 0x0004005B, (u32)btlevtcmd_FaceDirectionAdd, 0xFE363C83,
    0xFE363C80, 0xFE363C86, 0x0000006E, 0x0003005B, (u32)btlevtcmd_GetFaceDirection, 0xFE363C83,
    0xFE363C80, 0x00020037, 0xFE363C80, 0xFFFFFFEE, 0x0005005B, (u32)btlevtcmd_SetRotateOffset,
    0xFE363C83, 0x00000000, 0x00000007, 0x00000000, 0x00010005, 0x00000014,
    0x0005005B, (u32)btlevtcmd_AddRotate, 0xFE363C83, 0x00000000, 0x00000000, 0xFE363C80,
    0x00010009, 0x00000001, 0x00000006, 0x0005005B, (u32)btlevtcmd_SetRotate, 0xFE363C83,
    0x00000000, 0x00000000, 0x00000000, 0x00000070, 0x0008005B, (u32)btlevtcmd_MovePosition,
    0xFE363C83, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000014, 0xFFFFFFFF,
    0x00000000, 0x0003005B, (u32)_judge_on_stage, 0xFE363C83, 0xFE363C85, 0x00020018,
    0xFE363C85, 0x00000000, 0x0003005B, (u32)btlevtcmd_KillUnit, 0xFE363C83, 0x00000000,
    0x00000002, 0x00000021, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFE363C83, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0005005B, (u32)btlevtcmd_SetHomePos, 0xFE363C83, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFE363C83, 0x00000001,
    (u32)(char*)str_B_2_804241c4, 0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_BTL_THUNDERS_BOM_802f8498, 0xFE363C80, 0x0004005B,
    (u32)btlevtcmd_SetUnitWork, 0xFE363C83, 0x00000001, 0xFE363C80, 0x0001000A, 0x00000190,
    0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFE363C83, 0x00000002, 0x00000001
};

u32 sanders_appeal_event[] = {
    0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFFFFFFFE, 0x00000007, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType,
    0xFFFFFFFE, 0x00000001, 0x00000042, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_MARIO_APPEAL1_802f84b0,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000, 0x0003005B, (u32)btlevtcmd_AnimeWaitPlayComplete,
    0xFFFFFFFE, 0x00000001, 0x0002005B, (u32)btlevtcmd_AudienceAppeal, 0xFFFFFFFE, 0x0003005B,
    (u32)evt_snd_sfxon, (u32)(char*)str_SFX_BTL_THUNDERS_ATT_802f8424, 0x00000000, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0003005B, (u32)btlevtcmd_GetHeight, 0xFFFFFFFE,
    0xFE363C86, 0x00020038, 0xFE363C86, 0x00000002, 0x00020035, 0xFE363C81,
    0xFE363C86, 0x000F005B, (u32)evt_eff64, (u32)(u8*)zero_8042419c, (u32)(char*)str_bomb_n64_802f8440, 0x00000000,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0006005B,
    (u32)evt_btl_camera_shake_h, 0x00000000, 0x00000005, 0x00000000, 0x0000000A, 0x0000000D,
    0x0001005B, (u32)btlevtcmd_StageDispellFog, 0x0000006B, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000,
    0x00000000, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001, 0x0000006D,
    0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_J_1A_80424188, 0x0000006E,
    0x00010009, 0x0000001E, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001,
    (u32)(char*)str_J_1B_804241a0, 0x00000070, 0x0003005B, (u32)btlevtcmd_SetFallAccel, 0xFFFFFFFE, 0xF24A7C19,
    0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0000005A, 0xFFFFFFFF, 0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002,
    0x00000001
};

static u32 attack_audience_event[] = {
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
    0x0000001E, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000000, 0xFFFFFFFF, 0x00000000, 0x0003005B, (u32)evt_btl_camera_set_zoom,
    0x00000000, 0x000000C8, 0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_BTL_THUNDERS_ATT_802f8408, 0xFE363C8E,
    0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_A_3_80424194, 0x0001000A,
    0x000003E8, 0x0002005B, (u32)evt_snd_sfxoff, 0xFE363C8E, 0x0005005B, (u32)btlevtcmd_SetScale,
    0xFFFFFFFE, 0xF24A7E80, 0xF24A7E80, 0xF24A7E80, 0x0003005B, (u32)evt_snd_sfxon,
    (u32)(char*)str_SFX_BTL_THUNDERS_ATT_802f8424, 0x00000000, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x000F005B, (u32)evt_eff64, (u32)(u8*)zero_8042419c, (u32)(char*)str_bomb_n64_802f8440,
    0x00000001, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x0006005B, (u32)evt_btl_camera_shake_h, 0x00000000, 0x0000000A, 0x00000000, 0x00000014,
    0x0000000D, 0x0003005B, (u32)_attack_aud, 0xFE363C83, 0x00000000, 0x0003005B,
    (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000003, 0x0000006E, 0x0001000A, 0x000005DC,
    0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000, 0x0001000A, 0x000001F4,
    0x00000070, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_J_1A_80424188,
    0x0000006E, 0x00010009, 0x0000001E, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE,
    0x00000001, (u32)(char*)str_J_1B_804241a0, 0x00000070, 0x0003005B, (u32)btlevtcmd_SetFallAccel, 0xFFFFFFFE,
    0xF24A7C19, 0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0001006F, 0xFE363C8F, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0000005A, 0xFFFFFFFF, 0x00000070,
    0x00010009, 0x0000003C, 0x0001005F, 0xFE363C8F, 0x0005005B, (u32)btlevtcmd_GetPos,
    0xFFFFFFFE, 0xFE363C85, 0xFE363C86, 0xFE363C87, 0x0004005B, (u32)btlevtcmd_GetStageSize,
    0xF1194D80, 0xFE363C88, 0xF1194D80, 0x00020035, 0xFE363C86, 0xFE363C88,
    0x00020038, 0xFE363C86, 0x00000002, 0x0005005B, (u32)btlevtcmd_SetPos, 0xFFFFFFFE,
    0xFE363C85, 0xFE363C86, 0xFE363C87, 0x0003005B, (u32)btlevtcmd_SetFallAccel, 0xFFFFFFFE,
    0xF24A7B80, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0000001E, 0xFFFFFFFF, 0x0003005B, (u32)btlevtcmd_SetFallAccel, 0xFFFFFFFE,
    0xF24A7D4C, 0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0001006F, 0xFE363C8F, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000010, 0xFFFFFFFF, 0x00000070,
    0x0002005B, (u32)btlevtcmd_WaitEventEnd, 0xFE363C8F, 0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE,
    0x00000002, 0x00000001
};

u32 unk_evt_8038b678[] = {
    0x0006005B, (u32)btlevtcmd_OnOffStatus, 0xFFFFFFFE, 0x00000016, 0x00000000, 0x00000000,
    0x00000000, 0x0004005B, (u32)btlevtcmd_AnimeSetPoseTable, 0xFFFFFFFE, 0x00000001, (u32)pose_table_sanders_stay,
    0x00000002, 0x00000001
};

static u32 attack_event[] = {
    0x00000002, 0x00000001
};

u32 entry_bomzou[12] = {
    (u32)unit_bomzou, 0x02000000, 0x04000001, 0x447A0000, 0x00000000, 0x447A0000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000
};

static u32 party_win_reaction[] = {
    0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_Y_1_804241ac, 0x00010009,
    0x00000030, 0x00000002, 0x00000001
};
