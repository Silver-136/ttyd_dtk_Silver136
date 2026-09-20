#include "battle/battle_mario.h"
#include "battle/battle_audience.h"
#include "effect/eff_energy.h"
#include "effect/eff_fire.h"
#include "effect/eff_majinai.h"
#include "effect/n64/eff_kameki_tornade_n64.h"
extern void* _battleWorkPointer;

extern s32 evtGetValue(void* evt, s32 value);
extern void evtSetValue(void* evt, s32 var, s32 value);

extern s32 BattleTransID(void* evt, s32 id);
extern void* BattleGetUnitPtr(void* battleWork, s32 unitId);
extern void* BattleGetMarioPtr(void* battleWork);
extern void* BattleGetUnitPartsPtr(s32 unitId, s32 partsId);
extern void animPoseSetMaterialLightFlagOff(void* animPose, s32 flag);
extern s32 irand(s32 max);
extern void BtlUnit_GetPos(void* unit, f32* x, f32* y, f32* z);
extern void* BtlUnit_GetPartsPtr(void* unit, s32 partsId);
extern void BtlUnit_GetPartsPos(void* part, f32* x, f32* y, f32* z);
extern s32 BtlUnit_GetHeight(void* unit);
extern void BtlUnit_SetPos(void* unit, f32 x, f32 y, f32 z);
extern void effDelete(void* eff);
extern void* effWhirlwindN64Entry(f32 height, s32 flip, s32 unk, s32 duration);
extern void* effHibashiraEntry(f32 x1, f32 y1, f32 z1, f32 x2, f32 y2, f32 z2, f32 scale, s32 unk0, s32 unk1, s32 duration);
extern void effSetName(void* eff, const char* name);
extern void* effNameToPtr(const char* name);

/* Forward references used by recovered BattleWeapon records. */
extern u32 marioAttackEvent_BakuGame[];
extern u32 marioAttackEvent_Deka[];
extern u32 marioAttackEvent_DokanJump[];
extern u32 marioAttackEvent_FireNaguri[];
extern u32 marioAttackEvent_FirstAttackKaitenHammer[];
extern u32 marioAttackEvent_FunyafunyaJump[];
extern u32 marioAttackEvent_GatsunHammer[];
extern u32 marioAttackEvent_GatsunJump[];
extern u32 marioAttackEvent_Genki0[];
extern u32 marioAttackEvent_Genki1[];
extern u32 marioAttackEvent_HammerNageru[];
extern u32 marioAttackEvent_IceNaguri[];
extern u32 marioAttackEvent_JishinHammer[];
extern u32 marioAttackEvent_JyabaraJump[];
extern u32 marioAttackEvent_KaitenHammer[];
extern u32 marioAttackEvent_KururinJump[];
extern u32 marioAttackEvent_MiniminiFumi[];
extern u32 marioAttackEvent_Muki[];
extern u32 marioAttackEvent_NemuraseFumi[];
extern u32 marioAttackEvent_NormalCharge[];
extern u32 marioAttackEvent_NormalChargeP[];
extern u32 marioAttackEvent_NormalHammer[];
extern u32 marioAttackEvent_NormalJump[];
extern u32 marioAttackEvent_RenzokuJump[];
extern u32 marioAttackEvent_Scissor[];
extern u32 marioAttackEvent_Suki[];
extern u32 marioAttackEvent_SuperCharge[];
extern u32 marioAttackEvent_SuperChargeP[];
extern u32 marioAttackEvent_TatsumakiJump[];
extern u32 marioAttackEvent_TsuranukiHammer[];
extern u32 marioAttackEvent_TugiTugiJump[];
extern u32 marioAttackEvent_UltraJishinHammer[];
extern u32 marioAttackEvent_ZubaStar[];
extern s32 weaponGetPowerDefault();
extern s32 weaponGetPowerFromMarioHammerLv();
extern s32 weaponGetPowerFromMarioJumpLv();
extern s32 weaponGetPowerOverlapHammer1();
extern s32 weaponGetPowerOverlapJump1();
extern s32 weaponGetPowerTatsumaki();
extern s32 weaponGetPower_Deka();
extern s32 weaponGetPower_Scissor();
extern s32 weaponGetPower_ZubaStar();

/* Forward declarations for recovered Mario battle event bytecode. */
extern u32 _MajinaiPoseReset[];
extern u32 _NormalCharge_core[];
extern u32 _SuperCharge_core[];
extern u32 marioAttackEvent_FireNaguri_object[];
extern u32 marioAttackEvent_GatsuDokaJump[];
extern u32 marioAttackEvent_HammerNageru_object[];
extern u32 marioAttackEvent_HammerRotate[];
extern u32 marioAttackEvent_MajinaiDefenceUp[];
extern u32 marioAttackEvent_MajinaiExpUp[];
extern u32 marioAttackEvent_MajinaiJump[];
extern u32 marioAttackEvent_MajinaiPowerUpCheck[];
extern u32 marioAttackEvent_NormalHammer_Core[];
extern s32 btldefaultevt_SuitoruBadgeEffect();
extern s32 btlevtcmd_ACHelpSet();
extern s32 btlevtcmd_ACRGetResult();
extern s32 btlevtcmd_ACRStart();
extern s32 btlevtcmd_ACSuccessEffect();
extern s32 btlevtcmd_AcGetDifficulty();
extern s32 btlevtcmd_AcGetOutputParam();
extern s32 btlevtcmd_AcSetDifficulty();
extern s32 btlevtcmd_AcSetFlag();
extern s32 btlevtcmd_AcSetParamAll();
extern s32 btlevtcmd_AddPartsRotate();
extern s32 btlevtcmd_AddRotate();
extern s32 btlevtcmd_AnimeChangePaperAnime();
extern s32 btlevtcmd_AnimeChangePaperGroup();
extern s32 btlevtcmd_AnimeChangePose();
extern s32 btlevtcmd_AnimeChangePoseType();
extern s32 btlevtcmd_AnimeWaitPlayComplete();
extern s32 btlevtcmd_AnnounceMessage();
extern s32 btlevtcmd_AttackDeclare();
extern s32 btlevtcmd_AttackDeclareAll();
extern s32 btlevtcmd_AudienceDeclareACResult();
extern s32 btlevtcmd_AudienceDeclareAcrobatResult();
extern s32 btlevtcmd_CalculateFaceDirection();
extern s32 btlevtcmd_ChangeFaceDirection();
extern s32 btlevtcmd_CheckActStatus();
extern s32 btlevtcmd_CheckDamage();
extern s32 btlevtcmd_CheckToken();
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
extern s32 btlevtcmd_FallPartsPosition();
extern s32 btlevtcmd_FallPosition();
extern s32 btlevtcmd_GetBodyId();
extern s32 btlevtcmd_GetDamage();
extern s32 btlevtcmd_GetEnemyBelong();
extern s32 btlevtcmd_GetFaceDirection();
extern s32 btlevtcmd_GetFirstAttackTarget();
extern s32 btlevtcmd_GetFriendBelong();
extern s32 btlevtcmd_GetHeight();
extern s32 btlevtcmd_GetHitPos();
extern s32 btlevtcmd_GetHomePos();
extern s32 btlevtcmd_GetMoveFrame();
extern s32 btlevtcmd_GetPartsPos();
extern s32 btlevtcmd_GetPos();
extern s32 btlevtcmd_GetResultAC();
extern s32 btlevtcmd_GetResultCountAC();
extern s32 btlevtcmd_GetResultPrizeLv();
extern s32 btlevtcmd_GetRotate();
extern s32 btlevtcmd_GetScale();
extern s32 btlevtcmd_GetSelectEnemy();
extern s32 btlevtcmd_GetSelectNextEnemy();
extern s32 btlevtcmd_GetStageSize();
extern s32 btlevtcmd_GetTakeoffPosition();
extern s32 btlevtcmd_GetUnitId();
extern s32 btlevtcmd_GetUnitWork();
extern s32 btlevtcmd_GetWeaponActionLv();
extern s32 btlevtcmd_InviteApInfoReport();
extern s32 btlevtcmd_JumpContinue();
extern s32 btlevtcmd_JumpPartsContinue();
extern s32 btlevtcmd_JumpPartsPosition();
extern s32 btlevtcmd_JumpPartsSetting();
extern s32 btlevtcmd_JumpPosition();
extern s32 btlevtcmd_JumpSetting();
extern s32 btlevtcmd_MarioJumpParam();
extern s32 btlevtcmd_MarioJumpPosition();
extern s32 btlevtcmd_MovePosition();
extern s32 btlevtcmd_OffPartsAttribute();
extern s32 btlevtcmd_OffToken();
extern s32 btlevtcmd_OnPartsAttribute();
extern s32 btlevtcmd_PadCheckNow();
extern s32 btlevtcmd_PayWeaponCost();
extern s32 btlevtcmd_PreCheckDamage();
extern s32 btlevtcmd_ResetFaceDirection();
extern s32 btlevtcmd_ResultAC();
extern s32 btlevtcmd_RunDataEventChild();
extern s32 btlevtcmd_SamplingEnemy();
extern s32 btlevtcmd_SetFallAccel();
extern s32 btlevtcmd_SetMoveSoundDataWork();
extern s32 btlevtcmd_SetMoveSpeed();
extern s32 btlevtcmd_SetPartsBlur();
extern s32 btlevtcmd_SetPartsPos();
extern s32 btlevtcmd_SetPartsRotate();
extern s32 btlevtcmd_SetPartsRotateOffset();
extern s32 btlevtcmd_SetPartsWork();
extern s32 btlevtcmd_SetPos();
extern s32 btlevtcmd_SetRotate();
extern s32 btlevtcmd_SetRotateOffset();
extern s32 btlevtcmd_SetScale();
extern s32 btlevtcmd_SetUnitWork();
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
extern s32 btlevtcmd_check_battleflag();
extern s32 btlevtcmd_ftof();
extern s32 btlevtcmd_ftomsec();
extern s32 btlevtcmd_snd_se();
extern s32 btlevtcmd_snd_voice();
extern s32 evt_audience_acrobat_notry();
extern s32 evt_audience_ap_recovery();
extern s32 evt_audience_reflesh_natural_all();
extern s32 evt_btl_camera_off_posoffset_manual();
extern s32 evt_btl_camera_set_homing_unit();
extern s32 evt_btl_camera_set_homing_unitparts();
extern s32 evt_btl_camera_set_mode();
extern s32 evt_btl_camera_set_moveSpeedLv();
extern s32 evt_btl_camera_set_posoffset();
extern s32 evt_btl_camera_set_zoom();
extern s32 evt_btl_camera_set_zoomSpeedLv();
extern s32 evt_btl_camera_shake_h();
extern s32 evt_eff();
extern s32 evt_eff64();
extern s32 evt_majinai_effect_se_btl();
extern s32 evt_snd_sfx_pit();
extern s32 evt_snd_sfx_pos();
extern s32 evt_snd_sfxoff();
extern s32 evt_snd_sfxon();
extern s32 evt_sub_intpl_get_float();
extern s32 evt_sub_intpl_get_value();
extern s32 evt_sub_intpl_init();
extern s32 evt_sub_intpl_init_float();
extern s32 evt_sub_random();

extern void* pouchGetPtr(void);

extern void BattleMajinaiDone(void);
extern void BattleMajinaiEndCheck(void);
extern const f32 vec3_802ef5d8[3];
extern const char str_mjef_coinup_802ef624[];

s32 _get_mario_hammer_lv(void* evt) {
    s32* args;
    s32 dst;
    u8 value;

    args = *(s32**)((s32)evt + 0x18);
    dst = args[0];

    value = *(u8*)((s32)pouchGetPtr() + 0x99);

    evtSetValue(evt, dst, (s8)value);

    return 2;
}


s32 _whirlwind_effect(void* evt) {
    s32* args;
    void* battleWork;
    s32 type;
    s32 duration;
    void* unit;
    void* eff;
    f32 z;
    f32 y;
    f32 x;
    s32 direction;

    args = *(s32**)((s32)evt + 0x18);
    battleWork = _battleWorkPointer;
    type = evtGetValue(evt, args[0]);
    duration = evtGetValue(evt, args[1]);
    unit = BattleGetUnitPtr(battleWork, BattleTransID(evt, type));
    BtlUnit_GetPos(unit, &x, &y, &z);
    direction = *(s8*)((s32)unit + 0x189);
    eff = effWhirlwindN64Entry(*(f32*)((s32)unit + 0x114), (u32)direction >> 31, 0, duration);
    *(f32*)((s32)*(void**)((s32)eff + 0xC) + 8) = x;
    *(f32*)((s32)*(void**)((s32)eff + 0xC) + 0xC) = y;
    *(f32*)((s32)*(void**)((s32)eff + 0xC) + 0x10) = z;

    return 2;
}

s32 _jump_star_effect(void* evt, s32 first) {
    extern const f32 float_neg10_804223c4;
    extern const f32 float_10_804223d0;
    extern const f32 float_0p1_804223dc;
    extern const f32 float_0p05_804223e4;
    extern const f32 float_0p8_804223f4;
    extern const f32 float_2_804223f8;
    s32* args;
    void* battleWork;
    void* unit;
    s32 type;
    s32 effectType;
    s32 timer;
    s32 step;
    s32 rise;
    f32 x;
    f32 y;
    f32 z;
    f32 left;
    f32 right;
    f32 scale;

    args = *(s32**)((s32)evt + 0x18);
    battleWork = _battleWorkPointer;
    type = evtGetValue(evt, args[0]);
    effectType = evtGetValue(evt, args[1]);
    unit = BattleGetUnitPtr(battleWork, BattleTransID(evt, type));

    if (first != 0) {
        *(s32*)((s32)evt + 0x78) = 0;
    }

    timer = *(s32*)((s32)evt + 0x78);
    step = timer / 4;
    if ((timer % 4) == 0) {
        if ((s8)*(u8*)((s32)unit + 0x189) >= 0) {
            left = float_10_804223d0;
            right = float_neg10_804223c4;
        } else {
            left = float_neg10_804223c4;
            right = float_10_804223d0;
        }

        BtlUnit_GetPos(unit, &x, &y, &z);
        rise = (step * 8) / 10;
        x += right * *(f32*)((s32)unit + 0x114);
        y += (f32)rise;
        scale = float_0p8_804223f4 + (float_0p1_804223dc * (f32)step);
        if (scale > float_2_804223f8) {
            scale = float_2_804223f8;
        }
        effEnergyEntry(effectType, 0x32, x, y, x, scale, left, float_0p05_804223e4);
    }

    if (*(s32*)((s32)evt + 0x78) <= 0x18) {
        *(s32*)((s32)evt + 0x78) += 1;
        return 0;
    }
    return 2;
}

s32 _bgset_iron_frame_check(void* evt) {
    s32* args;
    void* battleWork;
    s32 dst;
    u8 flags;

    battleWork = _battleWorkPointer;
    args = *(s32**)((s32)evt + 0x18);

    flags = *(u8*)((s32)battleWork + 0x180F8);
    dst = args[0];

    if (flags & 0x20) {
        evtSetValue(evt, dst, 0);
    } else {
        evtSetValue(evt, dst, 1);
    }

    return 2;
}


s32 _wait_jyabara_hit_iron_frame(void* evt) {
    extern const f32 float_130_804223f0;
    s32* args;
    s32 type;
    void* unit;
    f32 z;
    f32 y;
    f32 x;
    f32 height;

    args = *(s32**)((s32)evt + 0x18);
    type = evtGetValue(evt, args[0]);
    unit = BattleGetUnitPtr(_battleWorkPointer, BattleTransID(evt, type));
    BtlUnit_GetPos(unit, &x, &y, &z);
    height = (f32)BtlUnit_GetHeight(unit);
    if (y + height < float_130_804223f0) {
        return 0;
    }

    BtlUnit_SetPos(unit, x, (f32)(130 - BtlUnit_GetHeight(unit)), z);
    return 2;
}


s32 _tatsumaki_effect(void* evt, s32 first) {
    extern f32 intplGetValue(s32 type, s32 current, s32 total, f32 start, f32 end);
    extern f32 sinfd(f32 angle);
    extern f32 cosfd(f32 angle);
    extern s32 psndSFXOn(const char* name);
    extern void psndSFX_pos(s32 id, void* pos);
    extern void psndSFXOff(s32 id);
    extern const f32 float_0_804223b4;
    extern const f32 float_3_804223e8;
    extern const f32 float_80_804223ec;
    s32* args = *(s32**)((s32)evt + 0x18);
    void* unit = BattleGetUnitPtr(_battleWorkPointer, BattleTransID(evt, evtGetValue(evt, args[0])));
    s32 state;
    s32 timer;
    f32 radius;
    f32 angle;
    f32 dx;
    f32 dz;

    if (first != 0) {
        *(s32*)((s32)evt + 0x78) = 0;
    }

    state = *(s32*)((s32)evt + 0x78);
    switch (state) {
        case 0: {
            f32 pos[3];

            *(s32*)((s32)evt + 0x78) = state + 1;
            *(s32*)((s32)evt + 0x7C) = 0;
            *(s32*)((s32)evt + 0x80) = (s32)*(f32*)((s32)unit + 0x3C);
            *(s32*)((s32)evt + 0x84) = (s32)*(f32*)((s32)unit + 0x40) - 40;
            *(s32*)((s32)evt + 0x88) = (s32)*(f32*)((s32)unit + 0x44);
            *(void**)((s32)evt + 0x8C) = effKamekiTornadeN64Entry(
                0,
                0x5A,
                (f32)*(s32*)((s32)evt + 0x80),
                (f32)*(s32*)((s32)evt + 0x84),
                (f32)*(s32*)((s32)evt + 0x88),
                float_3_804223e8);
            *(void**)((s32)evt + 0x90) = effKamekiTornadeN64Entry(
                0,
                0x5A,
                (f32)*(s32*)((s32)evt + 0x80),
                (f32)*(s32*)((s32)evt + 0x84),
                (f32)*(s32*)((s32)evt + 0x88),
                float_3_804223e8);
            *(s32*)((s32)evt + 0x94) = psndSFXOn("SFX_BTL_JUMP_TATUMAKI2");
            pos[0] = (f32)*(s32*)((s32)evt + 0x80);
            pos[1] = (f32)*(s32*)((s32)evt + 0x84);
            pos[2] = (f32)*(s32*)((s32)evt + 0x88);
            psndSFX_pos(*(s32*)((s32)evt + 0x94), pos);
            break;
        }
        case 1:
            break;
        default:
            return 2;
    }

    timer = *(s32*)((s32)evt + 0x7C) + 1;
    *(s32*)((s32)evt + 0x7C) = timer;
    radius = intplGetValue(5, timer, 0x5A, float_0_804223b4, float_80_804223ec);
    angle = (f32)(timer * 4);
    dx = radius * cosfd(angle);
    dz = radius * sinfd(angle);

    *(f32*)((s32)*(void**)((s32)*(void**)((s32)evt + 0x8C) + 0xC) + 4) = (f32)*(s32*)((s32)evt + 0x80) + dx;
    *(f32*)((s32)*(void**)((s32)*(void**)((s32)evt + 0x8C) + 0xC) + 8) = (f32)*(s32*)((s32)evt + 0x84);
    *(f32*)((s32)*(void**)((s32)*(void**)((s32)evt + 0x8C) + 0xC) + 0xC) = (f32)*(s32*)((s32)evt + 0x88) + dz;
    *(f32*)((s32)*(void**)((s32)*(void**)((s32)evt + 0x90) + 0xC) + 4) = (f32)*(s32*)((s32)evt + 0x80) - dx;
    *(f32*)((s32)*(void**)((s32)*(void**)((s32)evt + 0x90) + 0xC) + 8) = (f32)*(s32*)((s32)evt + 0x84);
    *(f32*)((s32)*(void**)((s32)*(void**)((s32)evt + 0x90) + 0xC) + 0xC) = (f32)*(s32*)((s32)evt + 0x88) - dz;

    if (*(s32*)((s32)evt + 0x7C) >= 0x5A) {
        psndSFXOff(*(s32*)((s32)evt + 0x94));
        return 2;
    }
    return 0;
}

s32 _record_renzoku_count(void* evt) {
    s32* args;
    s32 count;
    void* battleWorkBase;
    void* pouch;

    args = *(s32**)((s32)evt + 0x18);
    count = evtGetValue(evt, args[0]);

    battleWorkBase = (void*)((s32)_battleWorkPointer + 0x10000);
    pouch = pouchGetPtr();

    if (count > *(u8*)((s32)battleWorkBase + 0x6F40)) {
        if (count > *(u16*)((s32)pouch + 0x9E)) {
            *(u16*)((s32)pouch + 0x9E) = count;
            BattleAudience_Case_JumpNewRecord();
        }

        if (count > 100) {
            count = 100;
        }

        *(u8*)((s32)battleWorkBase + 0x6F40) = count;
    }

    return 2;
}

s32 mario_get_renzoku_count_max(void* evt) {
    s32* args;
    s32 type;
    s32 count;
    s32 dst;
    void* battleWork;
    void* unit;
    void* part;
    s32 max;

    args = *(s32**)((s32)evt + 0x18);
    type = evtGetValue(evt, args[0]);
    count = evtGetValue(evt, args[1]);
    dst = args[2];
    battleWork = _battleWorkPointer;
    unit = BattleGetUnitPtr(battleWork, BattleTransID(evt, type));
    if (unit == 0) {
        evtSetValue(evt, dst, 0);
        return 2;
    }

    part = *(void**)((s32)unit + 0x10);
    max = *(u16*)((s32)part + 0x14);
    if (count < max) {
        evtSetValue(evt, dst, max);
        return 2;
    }

    if (count >= max * 2) {
        evtSetValue(evt, dst, count);
        return 2;
    }

    if (irand(100) < 33) {
        evtSetValue(evt, dst, count + 1);
    } else {
        evtSetValue(evt, dst, count);
    }

    return 2;
}

s32 _hammer_star_effect(void* evt, s32 first) {
    extern f32 intplGetValue(s32 type, s32 current, s32 total, f32 start, f32 end);
    extern f32 sinfd(f32 angle);
    extern f32 cosfd(f32 angle);
    extern const f32 float_210_804223bc;
    extern const f32 float_420_804223c0;
    extern const f32 float_neg10_804223c4;
    extern const f32 float_150_804223c8;
    extern const f32 float_neg60_804223cc;
    extern const f32 float_10_804223d0;
    extern const f32 float_0p85_804223d4;
    extern const f32 float_26_804223d8;
    extern const f32 float_0p1_804223dc;
    extern const f32 float_0p08_804223e0;
    extern const f32 float_0p05_804223e4;
    s32* args;
    void* battleWork;
    void* unit;
    s32 type;
    s32 effectType;
    f32 x;
    f32 y;
    f32 z;
    f32 angle;
    f32 side;
    f32 scale;

    args = *(s32**)((s32)evt + 0x18);
    battleWork = _battleWorkPointer;
    type = evtGetValue(evt, args[0]);
    effectType = evtGetValue(evt, args[1]);
    unit = BattleGetUnitPtr(battleWork, BattleTransID(evt, type));

    if (first != 0) {
        *(s32*)((s32)evt + 0x78) = 0;
    }

    if ((s8)*(u8*)((s32)unit + 0x189) >= 0) {
        angle = intplGetValue(1, *(s32*)((s32)evt + 0x78), 0xC, float_210_804223bc, float_420_804223c0);
        side = float_neg10_804223c4;
    } else {
        angle = intplGetValue(1, *(s32*)((s32)evt + 0x78), 0xC, float_150_804223c8, float_neg60_804223cc);
        side = float_10_804223d0;
    }

    BtlUnit_GetPos(unit, &x, &y, &z);
    y += *(f32*)((s32)unit + 0x114) * (float_0p85_804223d4 * (f32)BtlUnit_GetHeight(unit));
    x += *(f32*)((s32)unit + 0x114) * (float_26_804223d8 * -sinfd(angle));
    y += *(f32*)((s32)unit + 0x114) * (float_26_804223d8 * cosfd(angle));

    scale = float_0p1_804223dc + (float_0p08_804223e0 * (f32)*(s32*)((s32)evt + 0x78));
    effEnergyEntry(effectType, 0x1E, x, y, z, scale, side, float_0p05_804223e4);

    if (*(s32*)((s32)evt + 0x78) < 0xC) {
        *(s32*)((s32)evt + 0x78) += 1;
        return 0;
    }
    return 2;
}

s32 _kaiten_hammer_acrobat_rotate(void* evt, s32 first) {
    extern f32 reviseAngle(f32 angle);
    extern void BtlUnit_GetRotate(void* unit, f32* x, f32* y, f32* z);
    extern void BtlUnit_AddRotate(void* unit, f32 x, f32 y, f32 z);
    extern void BtlUnit_SetRotate(void* unit, f32 x, f32 y, f32 z);
    extern void BtlUnit_SetAnim(void* part, s32 anim);
    extern const f32 float_360_804223b8;
    extern const f32 float_0_804223b4;
    s32* args;
    void* battleWork;
    void* unit;
    void* part;
    s32 type;
    s32 partsId;
    s32 frames;
    s32 anim;
    f32 x;
    f32 y;
    f32 z;
    s32 angle;

    args = *(s32**)((s32)evt + 0x18);
    battleWork = _battleWorkPointer;

    type = evtGetValue(evt, args[0]);
    unit = BattleGetUnitPtr(battleWork, BattleTransID(evt, type));
    partsId = evtGetValue(evt, args[1]);
    part = BtlUnit_GetPartsPtr(unit, partsId);
    frames = evtGetValue(evt, args[2]);
    anim = evtGetValue(evt, args[3]);

    if (first != 0) {
        BtlUnit_GetRotate(unit, &x, &y, &z);
        *(s32*)((s32)evt + 0x78) = frames;
        if ((s8)*(u8*)((s32)unit + 0x189) >= 0) {
            angle = (s32)(float_360_804223b8 + (float_360_804223b8 - reviseAngle(y)));
        } else {
            angle = (s32)(float_360_804223b8 + reviseAngle(y));
        }
        *(s32*)((s32)evt + 0x7C) = angle;
        *(s32*)((s32)evt + 0x80) = ((s8)*(u8*)((s32)unit + 0x189) * angle) / frames;
        *(s32*)((s32)evt + 0x84) = 0;
    }

    BtlUnit_AddRotate(unit, float_0_804223b4, (f32)*(s32*)((s32)evt + 0x80), float_0_804223b4);

    *(s32*)((s32)evt + 0x78) -= 1;
    if (*(s32*)((s32)evt + 0x84) == 0) {
        *(s32*)((s32)evt + 0x7C) -= *(s32*)((s32)evt + 0x80);
        if (*(s32*)((s32)evt + 0x7C) <= 0x10E) {
            *(s32*)((s32)evt + 0x84) = 1;
            BtlUnit_SetAnim(part, anim);
        }
    }

    if (*(s32*)((s32)evt + 0x78) > 0) {
        return 0;
    }

    BtlUnit_SetRotate(unit, float_0_804223b4, float_0_804223b4, float_0_804223b4);
    return 2;
}

s32 _mario_fire_ball_controll(void* evt, s32 first) {
    s32* args;
    void* battleWork;
    s32 type;
    s32 partsId;
    void* unit;
    void* part;
    void* eff;
    void* effPart;

    args = *(s32**)((s32)evt + 0x18);
    battleWork = _battleWorkPointer;
    type = evtGetValue(evt, args[0]);
    partsId = evtGetValue(evt, args[1]);
    unit = BattleGetUnitPtr(battleWork, BattleTransID(evt, type));
    part = BtlUnit_GetPartsPtr(unit, partsId);
    if (first != 0) {
        *(s32*)((s32)evt + 0x78) = 0;
        *(void**)((s32)evt + 0x7C) = effFireEntry(0.0f, 0.0f, 0.0f, 1.0f, 5, 1000);
    }

    eff = *(void**)((s32)evt + 0x7C);
    if (*(s32*)((s32)part + 0x130) == 0) {
        effDelete(eff);
        return 2;
    }

    effPart = *(void**)((s32)eff + 0xC);
    BtlUnit_GetPartsPos(part,
                        (f32*)((s32)effPart + 4),
                        (f32*)((s32)effPart + 8),
                        (f32*)((s32)effPart + 0xC));
    return 0;
}

s32 _fire_wave(void* evt, s32 first) {
    extern const f32 float_0p6_804223b0;
    s32* args;
    f32 x;
    f32 y;
    f32 z;
    f32 scale;

    args = *(s32**)((s32)evt + 0x18);
    x = (f32)evtGetValue(evt, args[0]);
    y = (f32)evtGetValue(evt, args[1]);
    z = (f32)evtGetValue(evt, args[2]);
    scale = (f32)evtGetValue(evt, args[3]) * float_0p6_804223b0;
    if (first != 0) {
        *(void**)((s32)evt + 0x78) = effHibashiraEntry(x, y, z, x, y, z, scale, 0, 1, 0x5A);
    }

    return 2;
}

s32 _majinai_powerup_check(void* evt) {
    s32* args;
    void* battleWork;
    s32 dst;
    void* pouch;

    args = *(s32**)((s32)evt + 0x18);
    battleWork = _battleWorkPointer;
    dst = args[0];

    pouch = pouchGetPtr();

    if (*(u8*)((s32)pouch + 0x5BA) != 1) {
        evtSetValue(evt, dst, 0);
    } else {
        evtSetValue(evt, dst, 1);
        *(u8*)((s32)battleWork + 0x18FF9) = 1;
        BattleMajinaiDone();
        BattleMajinaiEndCheck();
    }

    return 2;
}


s32 _battle_majinai_effect(void* evt, s32 first) {
    f32 pos[3];

    BattleGetMarioPtr(_battleWorkPointer);
    if (first != 0) {
        ((s32*)pos)[0] = ((s32*)vec3_802ef5d8)[0];
        ((s32*)pos)[1] = ((s32*)vec3_802ef5d8)[1];
        ((s32*)pos)[2] = ((s32*)vec3_802ef5d8)[2];
        *(void**)((s32)evt + 0x78) = effMajinaiEntry(pos[0], pos[1], pos[2], 0);
        effSetName(*(void**)((s32)evt + 0x78), str_mjef_coinup_802ef624);
    }

    if (*(s32*)((s32)evt + 0x78) != 0 &&
        effNameToPtr(str_mjef_coinup_802ef624) != 0) {
        return 0;
    }

    return 2;
}

s32 _paper_light_off(void* evt) {
    s32* args;
    s32 unitId;
    s32 partsId;
    void* parts;

    args = *(s32**)((s32)evt + 0x18);

    unitId = evtGetValue(evt, args[0]);
    unitId = BattleTransID(evt, unitId);

    partsId = evtGetValue(evt, args[1]);

    parts = BattleGetUnitPartsPtr(unitId, partsId);
    animPoseSetMaterialLightFlagOff(*(void**)((s32)parts + 0x1C0), 1);

    return 2;
}

const char str_btl_wn_mario_normal__802eeaa8[] = "btl_wn_mario_normal_jump";
const char str_msg_normal_jump_802eeac4[] = "msg_normal_jump";
const char str_msg_ac_jump_802eead4[] = "msg_ac_jump";
const char str_btl_wn_mario_kururin_802eeae0[] = "btl_wn_mario_kururin_jump";
const char str_msg_kururin_jump_802eeafc[] = "msg_kururin_jump";
const char str_msg_ac_k_jump_802eeb10[] = "msg_ac_k_jump";
const char str_btl_wn_mario_jyabara_802eeb20[] = "btl_wn_mario_jyabara_jump";
const char str_msg_jyabara_jump_802eeb3c[] = "msg_jyabara_jump";
const char str_msg_ac_j_jump_802eeb50[] = "msg_ac_j_jump";
const char str_btl_wn_mario_normal__802eeb60[] = "btl_wn_mario_normal_hammer";
const char str_msg_normal_hammer_802eeb7c[] = "msg_normal_hammer";
const char str_msg_ac_hammer_802eeb90[] = "msg_ac_hammer";
const char str_btl_wn_mario_kaiten__802eeba0[] = "btl_wn_mario_kaiten_hammer";
const char str_msg_kaiten_hammer_802eebbc[] = "msg_kaiten_hammer";
const char str_msg_ac_kaiten_hammer_802eebd0[] = "msg_ac_kaiten_hammer";
const char str_btl_wn_mario_ultra_h_802eebe8[] = "btl_wn_mario_ultra_hammer";
const char str_msg_ultra_hammer_802eec04[] = "msg_ultra_hammer";
const char str_msg_ac_ultra_hammer_802eec18[] = "msg_ac_ultra_hammer";
const char str_msg_ac_tatsumaki_jum_802eec2c[] = "msg_ac_tatsumaki_jump";
const char str_msg_ultra_attack_802eec44[] = "msg_ultra_attack";
const char str_msg_ac_ultra_jishin__802eec58[] = "msg_ac_ultra_jishin_hammer";
const char str_msg_ac_hammer_nageru_802eec74[] = "msg_ac_hammer_nageru";
const char str_msg_ac_fire_naguri_802eec8c[] = "msg_ac_fire_naguri";
const char str_btl_wn_mario_normal__802eeca0[] = "btl_wn_mario_normal_charge";
const char str_btl_hlp_cmd_operatio_802eecbc[] = "btl_hlp_cmd_operation_charge";
const char str_btl_wn_mario_super_c_802eecdc[] = "btl_wn_mario_super_charge";
const char str_btl_hlp_cmd_operatio_802eecf8[] = "btl_hlp_cmd_operation_super_charge";
const char str_btl_wn_2kai_item_802eed1c[] = "btl_wn_2kai_item";
const char str_btl_wn_3kai_item_802eed30[] = "btl_wn_3kai_item";
const char str_btl_wn_sac_bakugame_802eed44[] = "btl_wn_sac_bakugame";
const char str_msg_bakugame_802eed58[] = "msg_bakugame";
const char str_msg_sac_3_802eed68[] = "msg_sac_3";
const char str_btl_wn_sac_scissor_802eed74[] = "btl_wn_sac_scissor";
const char str_msg_scissor_802eed88[] = "msg_scissor";
const char str_msg_sac_5_802eed94[] = "msg_sac_5";
const char str_btl_wn_sac_zubastar_802eeda0[] = "btl_wn_sac_zubastar";
const char str_msg_zubastar_802eedb4[] = "msg_zubastar";
const char str_msg_sac_8_802eedc4[] = "msg_sac_8";
const char str_btl_wn_sac_genki0_802eedd0[] = "btl_wn_sac_genki0";
const char str_msg_genkigenki_802eede4[] = "msg_genkigenki";
const char str_msg_sac_1_802eedf4[] = "msg_sac_1";
const char str_btl_wn_sac_genki1_802eee00[] = "btl_wn_sac_genki1";
const char str_msg_super_genki_802eee14[] = "msg_super_genki";
const char str_msg_sac_6_802eee24[] = "msg_sac_6";
const char str_btl_wn_sac_dekkaku_d_802eee30[] = "btl_wn_sac_dekkaku_dokkan";
const char str_msg_dekkaku_dokkan_802eee4c[] = "msg_dekkaku_dokkan";
const char str_msg_sac_2_802eee60[] = "msg_sac_2";
const char str_btl_wn_sac_mukimuki__802eee6c[] = "btl_wn_sac_mukimuki_body";
const char str_msg_mukimuki_body_802eee88[] = "msg_mukimuki_body";
const char str_msg_sac_4_802eee9c[] = "msg_sac_4";
const char str_btl_wn_sac_sukkari_s_802eeea8[] = "btl_wn_sac_sukkari_sukkiri";
const char str_msg_sukkari_sukkiri_802eeec4[] = "msg_sukkari_sukkiri";
const char str_msg_sac_7_802eeed8[] = "msg_sac_7";
const char str_SFX_BTL_MARIO_HAMMER_802eeee4[] = "SFX_BTL_MARIO_HAMMER_SHINE1";
const char str_M_J_1B_802eef00[] = "M_J_1B";
const char str_SFX_VOICE_MARIO_BATT_802eef08[] = "SFX_VOICE_MARIO_BATTLE_JUMP1";
const char str_SFX_VOICE_MARIO_LAND_802eef28[] = "SFX_VOICE_MARIO_LAND_DAMAGE1";
const char str_M_J_1D_802eef48[] = "M_J_1D";
const char str_M_A_1A_802eef50[] = "M_A_1A";
const char str_SFX_BTL_JUMP_KURURIN_802eef58[] = "SFX_BTL_JUMP_KURURIN1";
const char str_SFX_VOICE_MARIO_POWE_802eef70[] = "SFX_VOICE_MARIO_POWER1";
const char str_M_A_1B_802eef88[] = "M_A_1B";
const char str_p_jyabara_802eef90[] = "p_jyabara";
const char str_PM_J_1E_802eef9c[] = "PM_J_1E";
const char str_SFX_VOICE_MARIO_POWE_802eefa4[] = "SFX_VOICE_MARIO_POWER2";
const char str_SFX_BTL_JUMP_JABARA1_802eefbc[] = "SFX_BTL_JUMP_JABARA1";
const char str_PM_J_1A_802eefd4[] = "PM_J_1A";
const char str_PM_J_1B_802eefdc[] = "PM_J_1B";
const char str_SFX_BTL_JUMP_JABARA2_802eefe4[] = "SFX_BTL_JUMP_JABARA2";
const char str_PM_J_2A_802eeffc[] = "PM_J_2A";
const char str_PM_J_2B_802ef004[] = "PM_J_2B";
const char str_SFX_BTL_JUMP_JABARA3_802ef00c[] = "SFX_BTL_JUMP_JABARA3";
const char str_PM_J_3A_802ef024[] = "PM_J_3A";
const char str_PM_J_3B_802ef02c[] = "PM_J_3B";
const char str_SFX_BTL_JUMP_JABARA4_802ef034[] = "SFX_BTL_JUMP_JABARA4";
const char str_SFX_VOICE_MARIO_AC3__802ef04c[] = "SFX_VOICE_MARIO_AC3_5";
const char str_PM_J_1C_802ef064[] = "PM_J_1C";
const char str_SFX_BTL_JUMP_CONTINU_802ef06c[] = "SFX_BTL_JUMP_CONTINUE1";
const char str_PM_J_1H_802ef084[] = "PM_J_1H";
const char str_M_Z_11_802ef08c[] = "M_Z_11";
const char str_SFX_BTL_JUMP_JABARA5_802ef094[] = "SFX_BTL_JUMP_JABARA5";
const char str_SFX_BTL_JUMP_TATUMAK_802ef0ac[] = "SFX_BTL_JUMP_TATUMAKI1";
const char str_SFX_BTL_MARIO_FALL_C_802ef0c4[] = "SFX_BTL_MARIO_FALL_CRUSH1";
const char str_M_A_3B_802ef0e0[] = "M_A_3B";
const char str_BTLEF_802ef0e8[] = "_BTLEF";
const char str_confetti_802ef0f0[] = "confetti";
const char str_SFX_BTL_ACROBAT_FINI_802ef0fc[] = "SFX_BTL_ACROBAT_FINISH1";
const char str_SFX_BTL_ACROBAT_MISS_802ef114[] = "SFX_BTL_ACROBAT_MISS1";
const char str_SFX_BTL_JUMP_CONTINU_802ef12c[] = "SFX_BTL_JUMP_CONTINUE2";
const char str_SFX_BTL_JUMP_CONTINU_802ef144[] = "SFX_BTL_JUMP_CONTINUE3";
const char str_SFX_BTL_JUMP_CONTINU_802ef15c[] = "SFX_BTL_JUMP_CONTINUE4";
const char str_SFX_BTL_JUMP_CONTINU_802ef174[] = "SFX_BTL_JUMP_CONTINUE5";
const char str_SFX_BTL_JUMP_CONTINU_802ef18c[] = "SFX_BTL_JUMP_CONTINUE6";
const char str_SFX_BTL_JUMP_CONTINU_802ef1a4[] = "SFX_BTL_JUMP_CONTINUE7";
const char str_SFX_BTL_JUMP_CONTINU_802ef1bc[] = "SFX_BTL_JUMP_CONTINUE8";
const char str_SFX_BTL_JUMP_CONTINU_802ef1d4[] = "SFX_BTL_JUMP_CONTINUE9";
const char str_SFX_BTL_JUMP_CONTINU_802ef1ec[] = "SFX_BTL_JUMP_CONTINUE10";
const char str_SFX_BTL_JUMP_TUGITUG_802ef204[] = "SFX_BTL_JUMP_TUGITUGI1";
const char str_M_H_5A_802ef21c[] = "M_H_5A";
const char str_M_H_5B_802ef224[] = "M_H_5B";
const char str_M_H_14_802ef22c[] = "M_H_14";
const char str_M_H_9A_802ef234[] = "M_H_9A";
const char str_M_H_9B_802ef23c[] = "M_H_9B";
const char str_M_H_15_802ef244[] = "M_H_15";
const char str_M_H_1A_802ef24c[] = "M_H_1A";
const char str_M_H_1B_802ef254[] = "M_H_1B";
const char str_M_H_13_802ef25c[] = "M_H_13";
const char str_M_H_5C_802ef264[] = "M_H_5C";
const char str_M_H_9C_802ef26c[] = "M_H_9C";
const char str_M_H_1C_802ef274[] = "M_H_1C";
const char str_SFX_MARIO_HAMMER_HOL_802ef27c[] = "SFX_MARIO_HAMMER_HOLD2";
const char str_SFX_MARIO_HAMMER_HOL_802ef294[] = "SFX_MARIO_HAMMER_HOLD1";
const char str_SFX_MARIO_HAMMER_HOL_802ef2ac[] = "SFX_MARIO_HAMMER_HOLD3";
const char str_SFX_MARIO_HAMMER_SWI_802ef2c4[] = "SFX_MARIO_HAMMER_SWING2";
const char str_SFX_MARIO_HAMMER_SWI_802ef2dc[] = "SFX_MARIO_HAMMER_SWING1";
const char str_SFX_MARIO_HAMMER_SWI_802ef2f4[] = "SFX_MARIO_HAMMER_SWING3";
const char str_SFX_MARIO_HAMMER_TUR_802ef30c[] = "SFX_MARIO_HAMMER_TURANUKI1";
const char str_SFX_MARIO_HAMMER_HIT_802ef328[] = "SFX_MARIO_HAMMER_HIT_GROUND3";
const char str_SFX_MARIO_HAMMER_HIT_802ef348[] = "SFX_MARIO_HAMMER_HIT_GROUND2";
const char str_SFX_MARIO_HAMMER_HIT_802ef368[] = "SFX_MARIO_HAMMER_HIT_GROUND1";
const char str_M_H_16_802ef388[] = "M_H_16";
const char str_M_A_3A_802ef390[] = "M_A_3A";
const char str_M_H_6A_802ef398[] = "M_H_6A";
const char str_M_H_6B_802ef3a0[] = "M_H_6B";
const char str_M_H_10A_802ef3a8[] = "M_H_10A";
const char str_M_H_10B_802ef3b0[] = "M_H_10B";
const char str_M_H_2A_802ef3b8[] = "M_H_2A";
const char str_M_H_2B_802ef3c0[] = "M_H_2B";
const char str_p_kaiten_h_802ef3c8[] = "p_kaiten_h";
const char str_P_H_1A_802ef3d4[] = "P_H_1A";
const char str_SFX_MARIO_HAMMER_KAI_802ef3dc[] = "SFX_MARIO_HAMMER_KAITEN1";
const char str_SFX_MARIO_HAMMER_ULT_802ef3f8[] = "SFX_MARIO_HAMMER_ULTRA1";
const char str_P_H_1B_802ef410[] = "P_H_1B";
const char str_P_H_1C_802ef418[] = "P_H_1C";
const char str_SFX_MARIO_HAMMER_ULT_802ef420[] = "SFX_MARIO_HAMMER_ULTRA2";
const char str_SFX_BTL_MARIO_HAMMER_802ef438[] = "SFX_BTL_MARIO_HAMMER_KAITEN2";
const char str_SFX_MARIO_ATTACK_ULT_802ef458[] = "SFX_MARIO_ATTACK_ULTRAQUAKE1";
const char str_SFX_MARIO_HAMMER_ULT_802ef478[] = "SFX_MARIO_HAMMER_ULTRAQUAKE1";
const char str_M_H_11_802ef498[] = "M_H_11";
const char str_M_H_12_802ef4a0[] = "M_H_12";
const char str_SFX_MARIO_HAMMER_NAG_802ef4a8[] = "SFX_MARIO_HAMMER_NAGERU1";
const char str_M_H_18A_802ef4c4[] = "M_H_18A";
const char str_M_H_18B_802ef4cc[] = "M_H_18B";
const char str_M_H_18C_802ef4d4[] = "M_H_18C";
const char str_M_H_19A_802ef4dc[] = "M_H_19A";
const char str_M_H_19B_802ef4e4[] = "M_H_19B";
const char str_M_H_19C_802ef4ec[] = "M_H_19C";
const char str_M_H_17A_802ef4f4[] = "M_H_17A";
const char str_M_H_17B_802ef4fc[] = "M_H_17B";
const char str_M_H_17C_802ef504[] = "M_H_17C";
const char str_SFX_BTL_MARIO_HAMMER_802ef50c[] = "SFX_BTL_MARIO_HAMMER_FIRE1";
const char str_SFX_BTL_MARIO_HAMMER_802ef528[] = "SFX_BTL_MARIO_HAMMER_FIRE3";
const char str_SFX_BTL_MARIO_HAMMER_802ef544[] = "SFX_BTL_MARIO_HAMMER_FIRE2";
const char str_fire_spark_n64_802ef560[] = "fire_spark_n64";
const char str_kemuri_test_802ef570[] = "kemuri_test";
const char str_SFX_BTL_MARIO_HAMMER_802ef57c[] = "SFX_BTL_MARIO_HAMMER_FIRE4";
const char str_SFX_MARIO_CHARGE1_802ef598[] = "SFX_MARIO_CHARGE1";
const char str_noname_802ef5ac[] = "noname";
const char str_crystal_n64_802ef5b4[] = "crystal_n64";
const char str_SFX_MARIO_SUPER_CHAR_802ef5c0[] = "SFX_MARIO_SUPER_CHARGE1";

const f32 vec3_802ef5d8[3] = { 0.0f, 10.0f, 0.0f };
const char str_btl_msg_mj_power_up_802ef5e4[] = "btl_msg_mj_power_up";
const char str_btl_msg_mj_defence_u_802ef5f8[] = "btl_msg_mj_defence_up";
const char str_btl_msg_mj_exp_up_802ef610[] = "btl_msg_mj_exp_up";
const char str_mjef_coinup_802ef624[] = "mjef_coinup";

/*
 * Recovered BattleWeapon records from the US target object.
 * Each record is 0xC0 bytes (48 words); symbolic relocations are preserved.
 * Player-facing labels are cross-checked against the ttyd-utils BattleWeapon dump.
 */
/* Jump */
u32 marioDefaultWeapon_Jump[48] = {
    (u32)(char*)str_btl_wn_mario_normal__802eeaa8, 0x00A70000, 0x00000000, (u32)(char*)str_msg_normal_jump_802eeac4, 0x64000000, 0x3F800000,
    0x01016432, (u32)weaponGetPowerFromMarioJumpLv, 0x00000001, 0x00000001, 0x00000002, 0x00000002,
    0x00000003, 0x00000003, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01001260, 0x20001004, 0x00000302, (u32)(char*)str_msg_ac_jump_802eead4, 0x00035807,
    0x0000000C, 0x80002004, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)marioAttackEvent_NormalJump, 0x00000064, 0x00040201, 0x01000000,
};

/* Super Jump 1 */
u32 marioWeapon_KururinJump[48] = {
    (u32)(char*)str_btl_wn_mario_kururin_802eeae0, 0x00A90000, 0x00000000, (u32)(char*)str_msg_kururin_jump_802eeafc, 0x64020000, 0x3F800000,
    0x01056432, (u32)weaponGetPowerFromMarioJumpLv, 0x00000001, 0x00000001, 0x00000002, 0x00000002,
    0x00000003, 0x00000003, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01001260, 0x20001004, 0x00000302, (u32)(char*)str_msg_ac_k_jump_802eeb10, 0x00025807,
    0x0000000C, 0x80002004, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)marioAttackEvent_KururinJump, 0x00000064, 0x00040501, 0x01000000,
};

/* Super Jump 2 */
u32 marioWeapon_KururinJump2[48] = {
    (u32)(char*)str_btl_wn_mario_kururin_802eeae0, 0x00A90000, 0x00000000, (u32)(char*)str_msg_kururin_jump_802eeafc, 0x64000000, 0x3F800000,
    0x01056432, (u32)weaponGetPowerFromMarioJumpLv, 0x00000001, 0x00000002, 0x00000003, 0x00000004,
    0x00000005, 0x00000006, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01001260, 0x20001004, 0x00000302, (u32)(char*)str_msg_ac_k_jump_802eeb10, 0x00025807,
    0x0000000C, 0x80002004, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)marioAttackEvent_KururinJump, 0x00050564, 0x00140A05, 0x05050000,
};

/* Spring Jump 1 */
u32 marioWeapon_JyabaraJump[48] = {
    (u32)(char*)str_btl_wn_mario_jyabara_802eeb20, 0x00A80000, 0x00000000, (u32)(char*)str_msg_jyabara_jump_802eeb3c, 0x64040000, 0x3F800000,
    0x01056432, (u32)weaponGetPowerFromMarioJumpLv, 0x00000001, 0x00000002, 0x00000002, 0x00000004,
    0x00000005, 0x00000006, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01001260, 0x20001004, 0x00000302, (u32)(char*)str_msg_ac_j_jump_802eeb50, 0x00025807,
    0x0000000C, 0x80002004, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)marioAttackEvent_JyabaraJump, 0x00050564, 0x00140A05, 0x05050000,
};

/* Spring Jump 2 */
u32 marioWeapon_JyabaraJumpFailed[48] = {
    (u32)(char*)str_btl_wn_mario_jyabara_802eeb20, 0x00A80000, 0x00000000, (u32)(char*)str_msg_jyabara_jump_802eeb3c, 0x64040000, 0x3F800000,
    0x01050132, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01001260, 0x20001004, 0x00000302, 0x00000000, 0x00025807,
    0x0000000C, 0x80002004, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)marioAttackEvent_JyabaraJump, 0x00323264, 0x00000032, 0x00000000,
};

/* First Strike Super Hammer */
u32 marioDefaultWeapon_FirstAttackKaitenHammer[48] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x64000000, 0x3F800000,
    0x01010101, (u32)weaponGetPowerFromMarioHammerLv, 0x00000001, 0x00000002, 0x00000002, 0x00000004,
    0x00000003, 0x00000006, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01101260, 0x21002000, 0x000E0302, 0x00000000, 0x00020887,
    0x000007FF, 0x80002004, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)marioAttackEvent_FirstAttackKaitenHammer, 0x00000064, 0x00000000, 0x00000000,
};

/* First Strike Ultra Hammer */
u32 marioDefaultWeapon_FirstAttackUltraHammer[48] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x64000000, 0x3F800000,
    0x01010101, (u32)weaponGetPowerFromMarioHammerLv, 0x00000001, 0x00000002, 0x00000002, 0x00000004,
    0x00000003, 0x00000006, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01101260, 0x21002000, 0x00100302, 0x00000000, 0x00020887,
    0x000007FF, 0x80002004, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)marioAttackEvent_FirstAttackKaitenHammer, 0x00000064, 0x00000000, 0x00000000,
};

/* Hammer */
u32 marioDefaultWeapon_Hammer[48] = {
    (u32)(char*)str_btl_wn_mario_normal__802eeb60, 0x00A40000, 0x00000000, (u32)(char*)str_msg_normal_hammer_802eeb7c, 0x64000000, 0x3F800000,
    0x01016432, (u32)weaponGetPowerFromMarioHammerLv, 0x00000001, 0x00000002, 0x00000002, 0x00000004,
    0x00000003, 0x00000006, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01001260, 0x21002000, 0x00000302, (u32)(char*)str_msg_ac_hammer_802eeb90, 0x00030887,
    0x000007FB, 0x80002004, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)marioAttackEvent_NormalHammer, 0x00030364, 0x03060203, 0x03000000,
};

/* Super Hammer 1 */
u32 marioWeapon_KaitenHammer[48] = {
    (u32)(char*)str_btl_wn_mario_kaiten__802eeba0, 0x00A50000, 0x00000000, (u32)(char*)str_msg_kaiten_hammer_802eebbc, 0x64020000, 0x3F800000,
    0x01056432, (u32)weaponGetPowerFromMarioHammerLv, 0x00000001, 0x00000002, 0x00000002, 0x00000004,
    0x00000003, 0x00000006, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01001260, 0x21002000, 0x000E0302, (u32)(char*)str_msg_ac_kaiten_hammer_802eebd0, 0x00020887,
    0x000007FB, 0x80002004, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)marioAttackEvent_KaitenHammer, 0x00000064, 0x00060500, 0x00000000,
};

/* Super Hammer 2 */
u32 marioWeapon_KaitenHammerInvolved[48] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x64000000, 0x00000000,
    0x00000000, (u32)weaponGetPowerDefault, 0x00000001, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x02100060, 0x20102000, 0x000F0302, 0x00000000, 0x00020040,
    0x000007FF, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000064, 0x00000000, 0x00000000,
};

/* Ultra Hammer 1 */
u32 marioWeapon_UltraHammer[48] = {
    (u32)(char*)str_btl_wn_mario_ultra_h_802eebe8, 0x00A60000, 0x00000000, (u32)(char*)str_msg_ultra_hammer_802eec04, 0x64040000, 0x3F800000,
    0x01056432, (u32)weaponGetPowerFromMarioHammerLv, 0x00000001, 0x00000002, 0x00000002, 0x00000004,
    0x00000003, 0x00000006, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01001260, 0x21002000, 0x00100302, (u32)(char*)str_msg_ac_ultra_hammer_802eec18, 0x00020807,
    0x000007FB, 0x80002004, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)marioAttackEvent_KaitenHammer, 0x00000064, 0x00060500, 0x00000000,
};

/* Ultra Hammer 2 */
u32 marioWeapon_UltraHammerInvolved[48] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x64000000, 0x00000000,
    0x00000000, (u32)weaponGetPowerDefault, 0x00000003, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x02100060, 0x20102000, 0x00110302, 0x00000000, 0x00020040,
    0x000007FF, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000064, 0x00000000, 0x00000000,
};

/* Ultra Hammer 3 */
u32 marioWeapon_UltraHammerFinish[48] = {
    (u32)(char*)str_btl_wn_mario_ultra_h_802eebe8, 0x00A60000, 0x00000000, (u32)(char*)str_msg_ultra_hammer_802eec04, 0x64000000, 0x3F800000,
    0x01056432, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01100060, 0x20000000, 0x00130302, 0x00000000, 0x0002088F,
    0x000007FF, 0x80002004, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)marioAttackEvent_KaitenHammer, 0x00000064, 0x00000000, 0x00050000,
};

/* Multibounce */
u32 badgeWeapon_TugitugiJump[48] = {
    0x00000000, 0x00000000, 0x000000F1, 0x00000000, 0x64020000, 0x3F800000,
    0x01056432, (u32)weaponGetPowerOverlapJump1, 0x00000000, 0x00000000, 0x00000001, 0x00000001,
    0x00000002, 0x00000002, 0x000000F1, 0x00000001, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x02101260, 0x20001004, 0x00000302, (u32)(char*)str_msg_ac_jump_802eead4, 0x00025807,
    0x0000000C, 0x80002004, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)marioAttackEvent_TugiTugiJump, 0x00000064, 0x000A0502, 0x02000000,
};

/* Power Jump */
u32 badgeWeapon_GatsunJump[48] = {
    0x00000000, 0x00000000, 0x000000F0, 0x00000000, 0x64020000, 0x3F800000,
    0x01056432, (u32)weaponGetPowerOverlapJump1, 0x00000001, 0x00000002, 0x00000002, 0x00000004,
    0x00000003, 0x00000006, 0x000000F0, 0x00000002, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01001260, 0x20001004, 0x00000302, (u32)(char*)str_msg_ac_jump_802eead4, 0x00025807,
    0x0000000C, 0x80002004, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)marioAttackEvent_GatsunJump, 0x00191964, 0x19191905, 0x19050000,
};

/* Mega Jump */
u32 badgeWeapon_DokanJump[48] = {
    0x00000000, 0x00000000, 0x0000014C, 0x00000000, 0x64060000, 0x3F800000,
    0x01056432, (u32)weaponGetPowerOverlapJump1, 0x00000001, 0x00000002, 0x00000002, 0x00000004,
    0x00000003, 0x00000006, 0x0000014C, 0x00000004, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01001260, 0x20001004, 0x00000302, (u32)(char*)str_msg_ac_jump_802eead4, 0x00025807,
    0x0000000C, 0x80002004, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)marioAttackEvent_DokanJump, 0x00191964, 0x19050505, 0x05050000,
};

/* Power Bounce */
u32 badgeWeapon_RenzokuJump[48] = {
    0x00000000, 0x00000000, 0x000000F2, 0x00000000, 0x64030000, 0x3F800000,
    0x01056432, (u32)weaponGetPowerOverlapJump1, 0x00000000, 0x00000000, 0x00000001, 0x00000001,
    0x00000002, 0x00000002, 0x000000F2, 0x00000001, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01001260, 0x20001004, 0x00000302, (u32)(char*)str_msg_ac_jump_802eead4, 0x00025817,
    0x0000000C, 0x80002004, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)marioAttackEvent_RenzokuJump, 0x00000064, 0x000A0502, 0x02000000,
};

/* Tornado Jump 1 */
u32 badgeWeapon_TatsumakiJump[48] = {
    0x00000000, 0x00000000, 0x000000F3, 0x00000000, 0x64030000, 0x3F800000,
    0x01056432, (u32)weaponGetPowerOverlapJump1, 0x00000000, 0x00000001, 0x00000001, 0x00000003,
    0x00000002, 0x00000005, 0x000000F3, 0x00000001, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01001260, 0x20001004, 0x00000302, (u32)(char*)str_msg_ac_tatsumaki_jum_802eec2c, 0x00025807,
    0x0000000C, 0x80002004, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)marioAttackEvent_TatsumakiJump, 0x00000064, 0x00000000, 0x00000000,
};

/* Tornado Jump 2 */
u32 badgeWeapon_TatsumakiJumpInvolved[48] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x64000000, 0x00000000,
    0x00000000, (u32)weaponGetPowerTatsumaki, 0x00000000, 0x00000002, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x02101260, 0x20000010, 0x00000302, 0x00000000, 0x00020040,
    0x000007FF, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000064, 0x00000000, 0x00000000,
};

/* Shrink Stomp */
u32 badgeWeapon_MiniminiFumi[48] = {
    0x00000000, 0x00000000, 0x000000F4, 0x00000000, 0x64020000, 0x3F800000,
    0x01056432, (u32)weaponGetPowerFromMarioJumpLv, 0x00000001, 0x00000001, 0x00000002, 0x00000002,
    0x00000003, 0x00000003, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01001260, 0x20001004, 0x00000302, (u32)(char*)str_msg_ac_jump_802eead4, 0x00025807,
    0x0000000C, 0x80002004, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000064, 0x03FE0000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)marioAttackEvent_MiniminiFumi, 0x00000064, 0x00060501, 0x01000000,
};

/* Sleepy Stomp */
u32 badgeWeapon_NemuraseFumi[48] = {
    0x00000000, 0x00000000, 0x000000F5, 0x00000000, 0x64020000, 0x3F800000,
    0x01056432, (u32)weaponGetPowerFromMarioJumpLv, 0x00000001, 0x00000001, 0x00000002, 0x00000002,
    0x00000003, 0x00000003, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01001260, 0x20001004, 0x00000302, (u32)(char*)str_msg_ac_jump_802eead4, 0x00025807,
    0x0000000C, 0x80002004, 0x64050000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)marioAttackEvent_NemuraseFumi, 0x00000064, 0x00060501, 0x01000000,
};

/* Soft Stomp */
u32 badgeWeapon_FunyafunyaJump[48] = {
    0x00000000, 0x00000000, 0x000000F6, 0x00000000, 0x64020000, 0x3F800000,
    0x01056432, (u32)weaponGetPowerFromMarioJumpLv, 0x00000001, 0x00000001, 0x00000002, 0x00000002,
    0x00000003, 0x00000003, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01001260, 0x20001004, 0x00000302, (u32)(char*)str_msg_ac_jump_802eead4, 0x00025807,
    0x0000000C, 0x80002004, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x006403FD, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)marioAttackEvent_FunyafunyaJump, 0x00000064, 0x00060501, 0x01000000,
};

/* Power Smash */
u32 badgeWeapon_GatsunHammer[48] = {
    0x00000000, 0x00000000, 0x000000F7, 0x00000000, 0x64020000, 0x3F800000,
    0x01056432, (u32)weaponGetPowerOverlapHammer1, 0x00000001, 0x00000002, 0x00000002, 0x00000004,
    0x00000003, 0x00000006, 0x000000F7, 0x00000002, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01001260, 0x21002000, 0x00000302, (u32)(char*)str_msg_ac_hammer_802eeb90, 0x00020887,
    0x000007FB, 0x80002004, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)marioAttackEvent_GatsunHammer, 0x00191964, 0x19191905, 0x19050000,
};

/* Ultra Smash */
u32 badgeWeapon_DokanHammer[48] = {
    0x00000000, 0x00000000, 0x0000014D, 0x00000000, 0x64060000, 0x3F800000,
    0x01056432, (u32)weaponGetPowerOverlapHammer1, 0x00000001, 0x00000002, 0x00000002, 0x00000004,
    0x00000003, 0x00000006, 0x0000014D, 0x00000004, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01001260, 0x21002000, 0x00000302, (u32)(char*)str_msg_ac_hammer_802eeb90, 0x00020887,
    0x000007FB, 0x80002004, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)marioAttackEvent_GatsunHammer, 0x00191964, 0x19050505, 0x05050000,
};

/* Quake Hammer */
u32 badgeWeapon_JishinHammer[48] = {
    0x00000000, 0x00000000, 0x000000F8, 0x00000000, 0x64030000, 0x3F800000,
    0x01056432, (u32)weaponGetPowerOverlapHammer1, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000,
    0xFFFFFFFF, 0x00000000, 0x000000F8, 0x00000002, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x02101260, 0x20000008, 0x00000302, (u32)(char*)str_msg_ac_hammer_802eeb90, 0x000238C7,
    0x000007FF, 0x80002004, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)marioAttackEvent_JishinHammer, 0x00323264, 0x32191905, 0x19050000,
};

/* Mega Quake */
u32 badgeWeapon_UltraJishinHammer[48] = {
    0x00000000, 0x00000000, 0x0000014E, (u32)(char*)str_msg_ultra_attack_802eec44, 0x64070000, 0x3F800000,
    0x01056432, (u32)weaponGetPowerOverlapHammer1, 0x00000003, 0x00000006, 0x00000003, 0x00000006,
    0x00000003, 0x00000006, 0x0000014E, 0x00000004, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x02101260, 0x20000008, 0x00000302, (u32)(char*)str_msg_ac_ultra_jishin__802eec58, 0x000238C7,
    0x000007FF, 0x80002004, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)marioAttackEvent_UltraJishinHammer, 0x00323264, 0x32050505, 0x05050000,
};

/* Hammer Throw */
u32 badgeWeapon_HammerNageru[48] = {
    0x00000000, 0x00000000, 0x000000F9, 0x00000000, 0x64020000, 0x3F800000,
    0x01056432, (u32)weaponGetPowerOverlapHammer1, 0x00000000, 0x00000001, 0x00000001, 0x00000003,
    0x00000002, 0x00000005, 0x000000F9, 0x00000001, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01001260, 0x20000000, 0x00000302, (u32)(char*)str_msg_ac_hammer_nageru_802eec74, 0x00020887,
    0x000007FF, 0x80002004, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)marioAttackEvent_HammerNageru, 0x00000064, 0x00000000, 0x00000000,
};

/* Piercing Blow */
u32 badgeWeapon_TsuranukiNaguri[48] = {
    0x00000000, 0x00000000, 0x000000FA, 0x00000000, 0x64020000, 0x3F800000,
    0x01056432, (u32)weaponGetPowerFromMarioHammerLv, 0x00000001, 0x00000002, 0x00000002, 0x00000004,
    0x00000003, 0x00000006, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01001260, 0x21002000, 0x00000302, (u32)(char*)str_msg_ac_hammer_802eeb90, 0x000208C7,
    0x000007FB, 0x80002004, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)marioAttackEvent_TsuranukiHammer, 0x00191964, 0x000A0505, 0x05050000,
};

/* Head Rattle */
u32 badgeWeapon_ConfuseHammer[48] = {
    0x00000000, 0x00000000, 0x000000FB, 0x00000000, 0x64020000, 0x3F800000,
    0x01056432, (u32)weaponGetPowerFromMarioHammerLv, 0x00000001, 0x00000002, 0x00000002, 0x00000004,
    0x00000003, 0x00000006, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01001260, 0x21002000, 0x00000302, (u32)(char*)str_msg_ac_hammer_802eeb90, 0x00020887,
    0x000007FB, 0x80002004, 0x00000000, 0x00000000, 0x00640300, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)marioAttackEvent_NormalHammer, 0x00050564, 0x000A0505, 0x05050000,
};

/* Fire Drive 1 */
u32 badgeWeapon_FireNaguri[48] = {
    0x00000000, 0x00000000, 0x000000FC, 0x00000000, 0x64050000, 0x3F800000,
    0x01056432, (u32)weaponGetPowerOverlapHammer1, 0xFFFFFFFF, 0x00000003, 0x00000000, 0x00000003,
    0x00000001, 0x00000003, 0x000000FC, 0x00000002, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x02001260, 0x20002000, 0x01000302, (u32)(char*)str_msg_ac_fire_naguri_802eec8c, 0x00020867,
    0x000007FF, 0x80002004, 0x00000000, 0x00000000, 0x00000000, 0x00000064,
    0x03000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)marioAttackEvent_FireNaguri, 0x00000064, 0x000A0505, 0x05050000,
};

/* Fire Drive 2 */
u32 badgeWeapon_FireNaguriFailed[48] = {
    0x00000000, 0x00000000, 0x000000FC, 0x00000000, 0x64050000, 0x3F800000,
    0x01050132, (u32)weaponGetPowerFromMarioHammerLv, 0x00000001, 0x00000005, 0x00000002, 0x00000005,
    0x00000003, 0x00000005, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x02100160, 0x10002000, 0x01000302, 0x00000000, 0x00020007,
    0x000007FF, 0x80002004, 0x00000000, 0x00000000, 0x00000000, 0x00000064,
    0x03000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)marioAttackEvent_FireNaguri, 0x00000064, 0x000A0505, 0x05050000,
};

/* Ice Smash */
u32 badgeWeapon_IceNaguri[48] = {
    0x00000000, 0x00000000, 0x000000FD, 0x00000000, 0x64030000, 0x3F800000,
    0x01056432, (u32)weaponGetPowerFromMarioHammerLv, 0x00000001, 0x00000002, 0x00000002, 0x00000004,
    0x00000003, 0x00000006, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01001260, 0x21002000, 0x02000302, (u32)(char*)str_msg_ac_hammer_802eeb90, 0x00020807,
    0x000007FB, 0x80002004, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00640200, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)marioAttackEvent_IceNaguri, 0x00050564, 0x000A0505, 0x05050000,
};

/* Charge */
u32 badgeWeapon_Charge[48] = {
    (u32)(char*)str_btl_wn_mario_normal__802eeca0, 0x00A70000, 0x00000100, (u32)(char*)str_btl_hlp_cmd_operatio_802eecbc, 0x64010000, 0x3F800000,
    0x01010101, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01004020, 0x10000000, 0x00000302, 0x00000000, 0x00020108,
    0x000007FF, 0x80002004, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000002, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)marioAttackEvent_NormalCharge, 0x00000064, 0x00000000, 0x00000000,
};

/* Charge P */
u32 badgeWeapon_ChargeP[48] = {
    (u32)(char*)str_btl_wn_mario_normal__802eeca0, 0x00A70000, 0x00000101, (u32)(char*)str_btl_hlp_cmd_operatio_802eecbc, 0x64010000, 0x3F800000,
    0x01010101, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01004020, 0x10000000, 0x00000302, 0x00000000, 0x00020108,
    0x000007FF, 0x80002004, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000002, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)marioAttackEvent_NormalChargeP, 0x00000064, 0x00000000, 0x00000000,
};

/* Super Charge */
u32 badgeWeapon_SuperCharge[48] = {
    (u32)(char*)str_btl_wn_mario_super_c_802eecdc, 0x00A70000, 0x00000151, (u32)(char*)str_btl_hlp_cmd_operatio_802eecf8, 0x64020000, 0x3F800000,
    0x01010101, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01004020, 0x10000000, 0x00000302, 0x00000000, 0x00020108,
    0x000007FF, 0x80002004, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000002, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)marioAttackEvent_SuperCharge, 0x00000064, 0x00000000, 0x00000000,
};

/* Super Charge P */
u32 badgeWeapon_SuperChargeP[48] = {
    (u32)(char*)str_btl_wn_mario_super_c_802eecdc, 0x00A70000, 0x00000152, (u32)(char*)str_btl_hlp_cmd_operatio_802eecf8, 0x64020000, 0x3F800000,
    0x01010101, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01004020, 0x10000000, 0x00000302, 0x00000000, 0x00020108,
    0x000007FF, 0x80002004, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000002, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)marioAttackEvent_SuperChargeP, 0x00000064, 0x00000000, 0x00000000,
};

/* Double Dip */
u32 badgeWeapon_2KaiItem[48] = {
    (u32)(char*)str_btl_wn_2kai_item_802eed1c, 0x00000000, 0x000000FE, 0x00000000, 0x64040000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01004020, 0x10000000, 0x00000302, 0x00000000, 0x00020108,
    0x000007FF, 0x80002004, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000064, 0x00000000, 0x00000000,
};

/* Triple Dip */
u32 badgeWeapon_3KaiItem[48] = {
    (u32)(char*)str_btl_wn_3kai_item_802eed30, 0x00000000, 0x00000149, 0x00000000, 0x64080000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01004020, 0x10000000, 0x00000302, 0x00000000, 0x00020108,
    0x000007FF, 0x80002004, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000064, 0x00000000, 0x00000000,
};

/* Mario Defend */
u32 badgeWeapon_DefenceCommand[48] = {
    0x00000000, 0x00000000, 0x0000014F, 0x00000000, 0x64000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01004020, 0x10000000, 0x00000302, 0x00000000, 0x00020008,
    0x000007FF, 0x80002004, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00640101, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000064, 0x00000000, 0x00000000,
};

/* Partner Defend */
u32 badgeWeapon_DefenceCommandP[48] = {
    0x00000000, 0x00000000, 0x00000150, 0x00000000, 0x64000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01004020, 0x10000000, 0x00000302, 0x00000000, 0x00020008,
    0x000007FF, 0x80002004, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00640101, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000064, 0x00000000, 0x00000000,
};

/* Clock Out */
u32 marioWeapon_BakuGame[48] = {
    (u32)(char*)str_btl_wn_sac_bakugame_802eed44, 0x01A00000, 0x00000000, (u32)(char*)str_msg_bakugame_802eed58, 0x64000200, 0x3F800000,
    0x01056432, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x02100060, 0x20000000, 0x00000302, (u32)(char*)str_msg_sac_3_802eed68, 0x00020088,
    0x000007FF, 0x00000000, 0x00006401, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)marioAttackEvent_BakuGame, 0x00000064, 0x00000000, 0x00000000,
};

/* Art Attack */
u32 marioWeapon_Scissor[48] = {
    (u32)(char*)str_btl_wn_sac_scissor_802eed74, 0x01A40000, 0x00000000, (u32)(char*)str_msg_scissor_802eed88, 0x64000400, 0x3F800000,
    0x01056432, (u32)weaponGetPower_Scissor, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x02101260, 0x20000000, 0x00000302, (u32)(char*)str_msg_sac_5_802eed94, 0x000200C8,
    0x000007FF, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)marioAttackEvent_Scissor, 0x00000064, 0x00000000, 0x00000000,
};

/* Supernova */
u32 marioWeapon_ZubaStar[48] = {
    (u32)(char*)str_btl_wn_sac_zubastar_802eeda0, 0x01A30000, 0x00000000, (u32)(char*)str_msg_zubastar_802eedb4, 0x64000600, 0x3F800000,
    0x01056432, (u32)weaponGetPower_ZubaStar, 0x00000000, 0x00000003, 0x00000006, 0x00000009,
    0x0000000C, 0x0000000F, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x02101260, 0x20000000, 0x00000302, (u32)(char*)str_msg_sac_8_802eedc4, 0x000200C8,
    0x000007FF, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)marioAttackEvent_ZubaStar, 0x00000064, 0x00000000, 0x00000000,
};

/* Sweet Treat */
u32 marioWeapon_Genki0[48] = {
    (u32)(char*)str_btl_wn_sac_genki0_802eedd0, 0x01390000, 0x00000000, (u32)(char*)str_msg_genkigenki_802eede4, 0x64000100, 0x3F800000,
    0x01056432, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x02100070, 0x10000000, 0x00000302, (u32)(char*)str_msg_sac_1_802eedf4, 0x00020008,
    0x000007FF, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)marioAttackEvent_Genki0, 0x00000064, 0x00000000, 0x00000000,
};

/* Sweet Feast */
u32 marioWeapon_Genki1[48] = {
    (u32)(char*)str_btl_wn_sac_genki1_802eee00, 0x01A50000, 0x00000000, (u32)(char*)str_msg_super_genki_802eee14, 0x64000500, 0x3F800000,
    0x01056432, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x02100070, 0x10000000, 0x00000302, (u32)(char*)str_msg_sac_6_802eee24, 0x00020008,
    0x000007FF, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)marioAttackEvent_Genki1, 0x00000064, 0x00000000, 0x00000000,
};

/* Earth Tremor */
u32 marioWeapon_Deka[48] = {
    (u32)(char*)str_btl_wn_sac_dekkaku_d_802eee30, 0x019F0000, 0x00000000, (u32)(char*)str_msg_dekkaku_dokkan_802eee4c, 0x64000200, 0x3F800000,
    0x01056432, (u32)weaponGetPower_Deka, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x02101260, 0x20000000, 0x00000302, (u32)(char*)str_msg_sac_2_802eee60, 0x000200C8,
    0x000007FF, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)marioAttackEvent_Deka, 0x00000064, 0x00000000, 0x00000000,
};

/* Power Lift */
u32 marioWeapon_Muki[48] = {
    (u32)(char*)str_btl_wn_sac_mukimuki__802eee6c, 0x01A20000, 0x00000000, (u32)(char*)str_msg_mukimuki_body_802eee88, 0x64000300, 0x3F800000,
    0x01056432, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x02100070, 0x10000000, 0x00000302, (u32)(char*)str_msg_sac_4_802eee9c, 0x00020008,
    0x000007FF, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)marioAttackEvent_Muki, 0x00000064, 0x00000000, 0x00000000,
};

/* Showstopper */
u32 marioWeapon_Suki[48] = {
    (u32)(char*)str_btl_wn_sac_sukkari_s_802eeea8, 0x01A10000, 0x00000000, (u32)(char*)str_msg_sukkari_sukkiri_802eeec4, 0x64000200, 0x3F800000,
    0x01056432, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x02101260, 0x20000000, 0x00000302, (u32)(char*)str_msg_sac_7_802eeed8, 0x00020008,
    0x000007FF, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00006400, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)marioAttackEvent_Suki, 0x00000064, 0x00000000, 0x00000000,
};

u32 superActionTable[18] = {
    (u32)marioWeapon_Genki0, (u32)marioWeapon_Deka, (u32)marioWeapon_BakuGame, (u32)marioWeapon_Muki, (u32)marioWeapon_Scissor, (u32)marioWeapon_Genki1,
    (u32)marioWeapon_Suki, (u32)marioWeapon_ZubaStar, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
};

const char str_M_A_5_80422310[] = "M_A_5";
const char str_M_D_2_80422318[] = "M_D_2";
const char str_M_W_1_80422320[] = "M_W_1";
const char str_M_N_7_80422328[] = "M_N_7";
const char str_M_U_3_80422330[] = "M_U_3";
const char str_M_S_1_80422338[] = "M_S_1";
const char str_M_I_S_80422340[] = "M_I_S";
const char str_M_B_2_80422348[] = "M_B_2";
const char str_M_I_Y_80422350[] = "M_I_Y";
const char str_M_B_1_80422358[] = "M_B_1";
const char str_M_A_4_80422360[] = "M_A_4";
const char str_M_A_6_80422368[] = "M_A_6";
const char str_M_Z_1_80422370[] = "M_Z_1";
const char str_M_C_3_80422378[] = "M_C_3";
const char str_M_V_2_80422380[] = "M_V_2";
const char str_M_H_7_80422388[] = "M_H_7";
const char str_M_H_8_80422390[] = "M_H_8";
const char str_M_H_3_80422398[] = "M_H_3";
const char str_M_H_4_804223a0[] = "M_H_4";
const u8 zero_804223a8 = 0;
const f32 float_1_804223ac = 1.0f;
const f32 float_0p6_804223b0 = 0.6f;
const f32 float_0_804223b4 = 0.0f;
const f32 float_360_804223b8 = 360.0f;
const f32 float_210_804223bc = 210.0f;
const f32 float_420_804223c0 = 420.0f;
const f32 float_neg10_804223c4 = -10.0f;
const f32 float_150_804223c8 = 150.0f;
const f32 float_neg60_804223cc = -60.0f;
const f32 float_10_804223d0 = 10.0f;
const f32 float_0p85_804223d4 = 0.85f;
const f32 float_26_804223d8 = 26.0f;
const f32 float_0p1_804223dc = 0.1f;
const f32 float_0p08_804223e0 = 0.08f;
const f32 float_0p05_804223e4 = 0.05f;
const f32 float_3_804223e8 = 3.0f;
const f32 float_80_804223ec = 80.0f;
const f32 float_130_804223f0 = 130.0f;
const f32 float_0p8_804223f4 = 0.8f;
const f32 float_2_804223f8 = 2.0f;

/*
 * Recovered Mario battle event bytecode.
 * Exact US target words are preserved; relocation words remain symbolic.
 * Semantic script identities were cross-checked against the ttydasm US event dump.
 */
/* .data + 0x00002448 */
u32 marioAttackEvent_NormalJump[] = {
    0x0005005B, (u32)btlevtcmd_JumpSetting, 0xFFFFFFFE, 0x00000014, 0xF24A7A80, 0xF24A7D4C,
    0x0003005B, (u32)btlevtcmd_check_battleflag, 0xFE363C80, 0x00000002, 0x00020019, 0xFE363C80,
    0x00000000, 0x0003005B, (u32)btlevtcmd_GetFirstAttackTarget, 0xFE363C83, 0xFE363C84, 0x00020018,
    0xFE363C83, 0xFFFFFFFF, 0x00010004, 0x00000063, 0x00000021, 0x00020032,
    0xFE363C8C, (u32)marioDefaultWeapon_Jump, 0x0002005B, (u32)btlevtcmd_ACHelpSet, (u32)marioDefaultWeapon_Jump, 0x0003005B,
    (u32)evt_btl_camera_set_mode, 0x00000000, 0x0000000A, 0x0006005B, (u32)evt_btl_camera_set_homing_unitparts, 0x00000000,
    0xFE363C83, 0xFE363C84, 0xFFFFFFFE, 0x00000001, 0x0003005B, (u32)evt_btl_camera_set_zoom,
    0x00000000, 0x000000FA, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000002,
    0x00000020, 0x0003005B, (u32)btlevtcmd_GetSelectEnemy, 0xFE363C83, 0xFE363C84, 0x00020018,
    0xFE363C83, 0xFFFFFFFF, 0x00010004, 0x00000063, 0x00000021, 0x0003005B,
    (u32)btlevtcmd_CommandGetWeaponAddress, 0xFFFFFFFE, 0xFE363C8C, 0x0004005B, (u32)btlevtcmd_AttackDeclare, 0xFFFFFFFE,
    0xFE363C83, 0xFE363C84, 0x0001005B, (u32)btlevtcmd_WaitGuardMove, 0x0003005B, (u32)btlevtcmd_PayWeaponCost,
    0xFFFFFFFE, 0xFE363C8C, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFFFFFFFE, 0x00000007,
    0x0001005E, (u32)marioAttackEvent_MajinaiPowerUpCheck, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x0000000B,
    0x0006005B, (u32)evt_btl_camera_set_homing_unitparts, 0x00000000, 0xFFFFFFFE, 0x00000001, 0xFE363C83,
    0xFE363C84, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001, 0x0003005B,
    (u32)evt_btl_camera_set_zoom, 0x00000000, 0x000000FA, 0x00000021, 0x0003005B, (u32)btlevtcmd_check_battleflag,
    0xFE363C80, 0x00000002, 0x00020018, 0xFE363C80, 0x00000000, 0x0003005B,
    (u32)btlevtcmd_GetWeaponActionLv, 0xFE363C8C, 0xFE363C80, 0x0003005B, (u32)btlevtcmd_AcSetDifficulty, 0xFFFFFFFE,
    0xFE363C80, 0x0005005B, (u32)btlevtcmd_SetupAC, 0xFFFFFFFE, 0x00000001, 0x00000001,
    0x00000000, 0x00000021, 0x0003005B, (u32)btlevtcmd_check_battleflag, 0xFE363C80, 0x00000002,
    0x00020019, 0xFE363C80, 0x00000000, 0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE,
    0xFE363C80, 0x000000FA, 0x0005005B, (u32)btlevtcmd_SetPos, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0001000A, 0x000003E8, 0x0003005B, (u32)btlevtcmd_SetMoveSpeed,
    0xFFFFFFFE, 0xF24A8E80, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001,
    0x0000002A, 0x0007005B, (u32)btlevtcmd_GetTakeoffPosition, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0007005B, (u32)btlevtcmd_CalculateFaceDirection, 0xFFFFFFFE,
    0xFFFFFFFF, 0xFE363C80, 0xFE363C82, 0x00000001, 0xFE363C86, 0x0003005B,
    (u32)btlevtcmd_ChangeFaceDirection, 0xFFFFFFFE, 0xFE363C86, 0x0007005B, (u32)btlevtcmd_GetMoveFrame, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000, 0xFE363C87, 0x0002001B,
    0xFE363C87, 0x00000016, 0x00020036, 0xFE363C87, 0x00000016, 0x00000020,
    0x00020036, 0xFE363C87, 0x00000016, 0x00020037, 0xFE363C87, 0xFFFFFFFF,
    0x00000021, 0x0000006E, 0x00010009, 0xFE363C87, 0x0003005B, (u32)btlevtcmd_GetWeaponActionLv,
    0xFE363C8C, 0xFE363C85, 0x0003005B, (u32)btlevtcmd_AcSetDifficulty, 0xFFFFFFFE, 0xFE363C85,
    0x0005005B, (u32)btlevtcmd_SetupAC, 0xFFFFFFFE, 0x00000001, 0x00000001, 0x00000000,
    0x00000070, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000000, 0xFFFFFFFF, 0x00000000, 0x00000020, 0x0003005B,
    (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE, 0xF24A8680, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE,
    0x00000001, 0x0000002A, 0x0007005B, (u32)btlevtcmd_GetTakeoffPosition, 0xFFFFFFFE, 0xFE363C83,
    0xFE363C84, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0007005B, (u32)btlevtcmd_CalculateFaceDirection,
    0xFFFFFFFE, 0xFFFFFFFF, 0xFE363C80, 0xFE363C82, 0x00000001, 0xFE363C86,
    0x0003005B, (u32)btlevtcmd_ChangeFaceDirection, 0xFFFFFFFE, 0xFE363C86, 0x0000006E, 0x00010022,
    0xFE363C8C, 0x00010024, (u32)badgeWeapon_MiniminiFumi, 0x00020032, 0xFE363C8D, 0x00000004,
    0x00010024, (u32)badgeWeapon_NemuraseFumi, 0x00020032, 0xFE363C8D, 0x00000003, 0x00010024,
    (u32)badgeWeapon_FunyafunyaJump, 0x00020032, 0xFE363C8D, 0x00000001, 0x00000031, 0x00010022,
    0xFE363C8C, 0x0001002B, (u32)badgeWeapon_MiniminiFumi, 0x0001002B, (u32)badgeWeapon_NemuraseFumi, 0x0001002B,
    (u32)badgeWeapon_FunyafunyaJump, 0x0003005B, (u32)_jump_star_effect, 0xFFFFFFFE, 0xFE363C8D, 0x0005005B,
    (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0006005B,
    (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_BTL_MARIO_HAMMER_802eeee4, 0xF1194D80, 0x00000000, 0xF1194D80,
    0x0000002E, 0x00000031, 0x00000070, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000, 0xFFFFFFFF, 0x00000000,
    0x00000021, 0x0007005B, (u32)btlevtcmd_CalculateFaceDirection, 0xFFFFFFFE, 0xFFFFFFFF, 0xFE363C83,
    0xFE363C84, 0x00000010, 0xFE363C8F, 0x0003005B, (u32)btlevtcmd_ChangeFaceDirection, 0xFFFFFFFE,
    0xFE363C8F, 0x0002005B, (u32)btlevtcmd_StartAC, 0x00000001, 0x00020032, 0xFE363C8A,
    0x00000000, 0x00010003, 0x0000000A, 0x0002001D, 0xFE363C8A, 0x00000001,
    0x0007005B, (u32)btlevtcmd_PreCheckDamage, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0xFE363C8C,
    0x00100100, 0xFE363C85, 0x00000020, 0x0007005B, (u32)btlevtcmd_PreCheckDamage, 0xFFFFFFFE,
    0xFE363C83, 0xFE363C84, 0xFE363C8C, 0x00000100, 0xFE363C85, 0x00000021,
    0x00010022, 0xFE363C85, 0x0001002B, 0x00000002, 0x0001002B, 0x00000003,
    0x0001002B, 0x00000004, 0x0001002B, 0x00000006, 0x0000002E, 0x00010024,
    0x00000001, 0x0000002A, 0x0001000A, 0x000003E8, 0x00000031, 0x0006005B,
    (u32)btlevtcmd_GetHitPos, 0xFE363C83, 0xFE363C84, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00020035, 0xFE363C82, 0x00000005, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE,
    0x00000001, (u32)(char*)str_M_J_1B_802eef00, 0x0002001D, 0xFE363C8A, 0x00000001, 0x00020032,
    0xFE363C86, 0x00000024, 0x0006005B, (u32)btlevtcmd_MarioJumpParam, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0xFE363C86, 0x0000006E, 0x00010009, 0x00000003,
    0x00020032, 0xFE363C80, 0xFE363C86, 0x00020038, 0xFE363C80, 0x00000002,
    0x00020036, 0xFE363C80, 0x00000003, 0x00020032, 0xFE363C81, 0xFE363C80,
    0x00020032, 0xFE363C82, 0xFE363C80, 0x00020036, 0xFE363C81, 0x00000007,
    0x00020035, 0xFE363C82, 0x00000005, 0x00020032, 0xFE363C80, 0xFE363C86,
    0x00020036, 0xFE363C80, 0x00000004, 0x0006005B, (u32)btlevtcmd_ACRStart, 0xFFFFFFFE,
    0xFE363C81, 0xFE363C82, 0xFE363C80, 0x00000000, 0x0003005B, (u32)btlevtcmd_ACRGetResult,
    0xFE363C86, 0xFE363C87, 0x00010022, 0xFE363C86, 0x00010029, 0x00000002,
    0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_A_5_80422310, 0x0006005B,
    (u32)btlevtcmd_AudienceDeclareAcrobatResult, 0xFE363C8C, 0x00000001, 0xFFFFFFC4, 0xFFFFFFE7, 0x00000000,
    0x0003005B, (u32)btlevtcmd_snd_voice, 0xFFFFFFFE, 0x00000001, 0x0000002A, 0x0001005B,
    (u32)evt_audience_acrobat_notry, 0x00000031, 0x00000070, 0x0008005B, (u32)btlevtcmd_MarioJumpPosition, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0xFE363C86, 0x00000003, 0xFFFFFFFF,
    0x00000020, 0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_VOICE_MARIO_BATT_802eef08, 0xF1194D80,
    0x00000000, 0xF1194D80, 0x00020032, 0xFE363C86, 0x00000024, 0x0006005B,
    (u32)btlevtcmd_MarioJumpParam, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0xFE363C86,
    0x0008005B, (u32)btlevtcmd_MarioJumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0xFE363C86, 0x00000000, 0xFFFFFFFF, 0x00000021, 0x00020019, 0xFE363C85,
    0x00000001, 0x00020018, 0xFE363C85, 0x00000003, 0x0003005B, (u32)btlevtcmd_StartAvoid,
    0xFE363C83, 0x00000026, 0x00000021, 0x00020018, 0xFE363C85, 0x00000006,
    0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000027, 0x00000021, 0x00020018,
    0xFE363C85, 0x00000002, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000028,
    0x00000021, 0x0001005B, (u32)btlevtcmd_StopAC, 0x0002005B, (u32)btlevtcmd_JumpContinue, 0xFFFFFFFE,
    0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_VOICE_MARIO_LAND_802eef28, 0xF1194D80, 0x00000000,
    0xF1194D80, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_D_2_80422318,
    0x00010009, 0x00000028, 0x00010004, 0x0000005F, 0x00000021, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_J_1D_802eef48, 0x0003005B, (u32)btlevtcmd_ac_timing_flag_onoff,
    0x00000001, 0x00000001, 0x0002005B, (u32)btlevtcmd_ac_timing_get_success_frame, 0xFE363C80, 0x00020038,
    0xFE363C80, 0x00000002, 0x00010009, 0xFE363C80, 0x0003005B, (u32)btlevtcmd_ac_timing_flag_onoff,
    0x00000000, 0x00000001, 0x0001005B, (u32)btlevtcmd_ResultAC, 0x0002005B, (u32)btlevtcmd_GetResultAC,
    0xFE363C86, 0x0002001F, 0xFE363C86, 0x00000002, 0x0007005B, (u32)btlevtcmd_CheckDamage,
    0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0xFE363C8C, 0x00000100, 0xFE363C85,
    0x00020018, 0xFE363C8A, 0x00000000, 0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult, 0xFE363C8C,
    0xFFFFFFFF, 0x00000021, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000,
    0x0001005B, (u32)btlevtcmd_StopAC, 0x0001005B, (u32)evt_audience_ap_recovery, 0x0001005B, (u32)btlevtcmd_InviteApInfoReport,
    0x0002005B, (u32)btlevtcmd_WeaponAftereffect, 0xFE363C8C, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE,
    0x00000001, (u32)(char*)str_M_J_1B_802eef00, 0x0003005B, (u32)btlevtcmd_SetFallAccel, 0xFFFFFFFE, 0xF24A7D4C,
    0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xF1194D80, 0xFE363C81, 0xF1194D80,
    0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE, 0xFE363C80, 0x0000001B, 0x0007005B,
    (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000014,
    0xFFFFFFFF, 0x0003005B, (u32)btlevtcmd_SetFallAccel, 0xFFFFFFFE, 0xF24A7CE6, 0x0004005B,
    (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE, 0xFE363C80, 0x00000014, 0x0007005B, (u32)btlevtcmd_JumpPosition,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0000000C, 0xFFFFFFFF,
    0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE, 0xFE363C80, 0x00000008, 0x0007005B,
    (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000008,
    0xFFFFFFFF, 0x00010004, 0x0000005F, 0x00000021, 0x0003005B, (u32)btlevtcmd_snd_voice,
    0xFFFFFFFE, 0x00000002, 0x0002005B, (u32)btlevtcmd_GetResultCountAC, 0xFE363C86, 0x0002001C,
    0xFE363C86, 0x00000001, 0x0000006B, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000,
    0x00000000, 0x0003005B, (u32)evt_btl_camera_set_zoom, 0x00000000, 0x000000C8, 0x00010009,
    0x0000000A, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001, 0x0003005B,
    (u32)evt_btl_camera_set_zoom, 0x00000000, 0x000000FA, 0x0000006D, 0x0007005B, (u32)btlevtcmd_CheckDamage,
    0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0xFE363C8C, 0x00020000, 0xFE363C85,
    0x0006005B, (u32)btlevtcmd_GetHitPos, 0xFE363C83, 0xFE363C84, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0004005B, (u32)btlevtcmd_GetResultPrizeLv, 0xFE363C83, 0x00000000, 0xFE363C86,
    0x0005005B, (u32)btlevtcmd_ACSuccessEffect, 0xFE363C86, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult, 0xFE363C8C, 0xFE363C86, 0x00010009, 0x00000002,
    0x00020035, 0xFE363C8A, 0x00000001, 0x00010004, 0x0000000A, 0x00000020,
    0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000, 0x0007005B, (u32)btlevtcmd_CheckDamage,
    0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0xFE363C8C, 0x00020100, 0xFE363C85,
    0x00010009, 0x00000002, 0x00000021, 0x00010003, 0x0000005A, 0x0003005B,
    (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000, 0x0001005B, (u32)btlevtcmd_StopAC, 0x0002005B,
    (u32)btlevtcmd_WeaponAftereffect, 0xFE363C8C, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001,
    (u32)(char*)str_M_J_1B_802eef00, 0x0003005B, (u32)btlevtcmd_SetFallAccel, 0xFFFFFFFE, 0xF24A7D4C, 0x0005005B,
    (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B,
    (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xF1194D80, 0xFE363C81, 0xF1194D80, 0x0004005B,
    (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE, 0xFE363C80, 0x0000003C, 0x00020032, 0xFE363C86,
    0x00000020, 0x0000006E, 0x00010009, 0x00000003, 0x00020032, 0xFE363C80,
    0xFE363C86, 0x00020038, 0xFE363C80, 0x00000002, 0x00020036, 0xFE363C80,
    0x00000003, 0x00020032, 0xFE363C81, 0xFE363C80, 0x00020032, 0xFE363C82,
    0xFE363C80, 0x00020036, 0xFE363C81, 0x00000007, 0x00020035, 0xFE363C82,
    0x00000005, 0x00020032, 0xFE363C80, 0xFE363C86, 0x00020036, 0xFE363C80,
    0x00000004, 0x0006005B, (u32)btlevtcmd_ACRStart, 0xFFFFFFFE, 0xFE363C81, 0xFE363C82,
    0xFE363C80, 0x00000000, 0x0003005B, (u32)btlevtcmd_ACRGetResult, 0xFE363C86, 0xFE363C87,
    0x00010022, 0xFE363C86, 0x00010029, 0x00000002, 0x0004005B, (u32)btlevtcmd_AnimeChangePose,
    0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_A_5_80422310, 0x0006005B, (u32)btlevtcmd_AudienceDeclareAcrobatResult, 0xFE363C8C,
    0x00000001, 0xFFFFFFC4, 0xFFFFFFE7, 0x00000000, 0x0003005B, (u32)btlevtcmd_snd_voice,
    0xFFFFFFFE, 0x00000001, 0x0000002A, 0x0001005B, (u32)evt_audience_acrobat_notry, 0x00000031,
    0x00000070, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0xFE363C86, 0xFFFFFFFF, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE,
    0x00000001, (u32)(char*)str_M_J_1B_802eef00, 0x0001005B, (u32)evt_audience_ap_recovery, 0x0001005B, (u32)btlevtcmd_InviteApInfoReport,
    0x0003005B, (u32)btlevtcmd_SetFallAccel, 0xFFFFFFFE, 0xF24A7CE6, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub,
    0xFFFFFFFE, 0xFE363C80, 0x00000014, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0000000C, 0xFFFFFFFF, 0x0004005B,
    (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE, 0xFE363C80, 0x00000008, 0x0007005B, (u32)btlevtcmd_JumpPosition,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000008, 0xFFFFFFFF,
    0x00010003, 0x0000005F, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000,
    0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001, 0x0004005B, (u32)btlevtcmd_AnimeChangePose,
    0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_W_1_80422320, 0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE,
    0xF24A9280, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001, 0x00000028,
    0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00000000, 0xFFFFFFFF, 0x00000000, 0x00010003, 0x00000063, 0x0002005B,
    (u32)btlevtcmd_ResetFaceDirection, 0xFFFFFFFE, 0x0001005E, (u32)btldefaultevt_SuitoruBadgeEffect, 0x0002005B, (u32)btlevtcmd_StartWaitEvent,
    0xFFFFFFFE, 0x00000002, 0x00000001,
};

/* .data + 0x00003354 */
u32 marioAttackEvent_KururinJump[] = {
    0x0005005B, (u32)btlevtcmd_JumpSetting, 0xFFFFFFFE, 0x00000014, 0xF24A7A80, 0xF24A7C19,
    0x0003005B, (u32)btlevtcmd_check_battleflag, 0xFE363C80, 0x00000002, 0x00020019, 0xFE363C80,
    0x00000000, 0x00020032, 0xFE363C8C, (u32)marioWeapon_KururinJump, 0x0002005B, (u32)btlevtcmd_ACHelpSet,
    (u32)marioWeapon_KururinJump, 0x0003005B, (u32)btlevtcmd_GetFirstAttackTarget, 0xFE363C83, 0xFE363C84, 0x00020018,
    0xFE363C83, 0xFFFFFFFF, 0x00010004, 0x00000063, 0x00000021, 0x0003005B,
    (u32)evt_btl_camera_set_mode, 0x00000000, 0x0000000A, 0x0006005B, (u32)evt_btl_camera_set_homing_unitparts, 0x00000000,
    0xFE363C83, 0xFE363C84, 0xFFFFFFFE, 0x00000001, 0x0003005B, (u32)evt_btl_camera_set_zoom,
    0x00000000, 0x000000FA, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000002,
    0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE, 0xFE363C80, 0x000000FA, 0x0005005B,
    (u32)btlevtcmd_SetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00010009,
    0x0000001E, 0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE, 0xF24A8E80, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001, 0x0000002A, 0x0007005B, (u32)btlevtcmd_GetTakeoffPosition,
    0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0007005B, (u32)btlevtcmd_CalculateFaceDirection, 0xFFFFFFFE, 0xFFFFFFFF, 0xFE363C80, 0xFE363C82,
    0x00000001, 0xFE363C86, 0x0003005B, (u32)btlevtcmd_ChangeFaceDirection, 0xFFFFFFFE, 0xFE363C86,
    0x0007005B, (u32)btlevtcmd_GetMoveFrame, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00000000, 0xFE363C87, 0x0002001B, 0xFE363C87, 0x00000016, 0x00020036,
    0xFE363C87, 0x00000016, 0x00000020, 0x00020036, 0xFE363C87, 0x00000016,
    0x00020037, 0xFE363C87, 0xFFFFFFFF, 0x00000021, 0x0000006E, 0x00010009,
    0xFE363C87, 0x0003005B, (u32)btlevtcmd_GetWeaponActionLv, 0xFE363C8C, 0xFE363C85, 0x0003005B,
    (u32)btlevtcmd_AcSetDifficulty, 0xFFFFFFFE, 0xFE363C85, 0x0005005B, (u32)btlevtcmd_SetupAC, 0xFFFFFFFE,
    0x00000001, 0x00000001, 0x00000000, 0x00000070, 0x0008005B, (u32)btlevtcmd_MovePosition,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000, 0xFFFFFFFF,
    0x00000000, 0x00000020, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress, 0xFFFFFFFE, 0xFE363C8C,
    0x0003005B, (u32)btlevtcmd_GetSelectEnemy, 0xFE363C83, 0xFE363C84, 0x00020018, 0xFE363C83,
    0xFFFFFFFF, 0x00010004, 0x00000063, 0x00000021, 0x0004005B, (u32)btlevtcmd_AttackDeclare,
    0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0x0001005B, (u32)btlevtcmd_WaitGuardMove, 0x0003005B,
    (u32)btlevtcmd_PayWeaponCost, 0xFFFFFFFE, 0xFE363C8C, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFFFFFFFE,
    0x00000007, 0x0001005E, (u32)marioAttackEvent_MajinaiPowerUpCheck, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000,
    0x0000000B, 0x0006005B, (u32)evt_btl_camera_set_homing_unitparts, 0x00000000, 0xFFFFFFFE, 0x00000001,
    0xFE363C83, 0xFE363C84, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001,
    0x0003005B, (u32)evt_btl_camera_set_zoom, 0x00000000, 0x000000FA, 0x0003005B, (u32)btlevtcmd_SetMoveSpeed,
    0xFFFFFFFE, 0xF24A8680, 0x0003005B, (u32)btlevtcmd_GetWeaponActionLv, 0xFE363C8C, 0xFE363C80,
    0x0003005B, (u32)btlevtcmd_AcSetDifficulty, 0xFFFFFFFE, 0xFE363C80, 0x0005005B, (u32)btlevtcmd_SetupAC,
    0xFFFFFFFE, 0x00000001, 0x00000001, 0x00000000, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType,
    0xFFFFFFFE, 0x00000001, 0x0000002A, 0x0007005B, (u32)btlevtcmd_GetTakeoffPosition, 0xFFFFFFFE,
    0xFE363C83, 0xFE363C84, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0007005B,
    (u32)btlevtcmd_CalculateFaceDirection, 0xFFFFFFFE, 0xFFFFFFFF, 0xFE363C80, 0xFE363C82, 0x00000001,
    0xFE363C86, 0x0003005B, (u32)btlevtcmd_ChangeFaceDirection, 0xFFFFFFFE, 0xFE363C86, 0x0008005B,
    (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000,
    0xFFFFFFFF, 0x00000000, 0x00000021, 0x0007005B, (u32)btlevtcmd_CalculateFaceDirection, 0xFFFFFFFE,
    0xFFFFFFFF, 0xFE363C83, 0xFE363C84, 0x00000010, 0xFE363C8F, 0x0003005B,
    (u32)btlevtcmd_ChangeFaceDirection, 0xFFFFFFFE, 0xFE363C8F, 0x0002005B, (u32)btlevtcmd_StartAC, 0x00000001,
    0x00020032, 0xFE363C8A, 0x00000000, 0x00010003, 0x0000000A, 0x0002001D,
    0xFE363C8A, 0x00000001, 0x0007005B, (u32)btlevtcmd_PreCheckDamage, 0xFFFFFFFE, 0xFE363C83,
    0xFE363C84, 0xFE363C8C, 0x00100100, 0xFE363C85, 0x00000020, 0x0007005B,
    (u32)btlevtcmd_PreCheckDamage, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0xFE363C8C, 0x00000100,
    0xFE363C85, 0x00000021, 0x00010022, 0xFE363C85, 0x0001002B, 0x00000002,
    0x0001002B, 0x00000003, 0x0001002B, 0x00000004, 0x0001002B, 0x00000006,
    0x0000002E, 0x00010024, 0x00000001, 0x0000002A, 0x0001000A, 0x000003E8,
    0x00000031, 0x0002001C, 0xFE363C8A, 0x00000000, 0x0006005B, (u32)btlevtcmd_GetHitPos,
    0xFE363C83, 0xFE363C84, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00020035,
    0xFE363C82, 0x00000005, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001,
    (u32)(char*)str_M_J_1B_802eef00, 0x00020032, 0xFE363C86, 0x00000024, 0x0006005B, (u32)btlevtcmd_MarioJumpParam,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0xFE363C86, 0x0000006E,
    0x00020032, 0xFE363C80, 0xFE363C86, 0x00020032, 0xFE363C81, 0xFE363C86,
    0x00020038, 0xFE363C80, 0x00000002, 0x00020036, 0xFE363C81, 0x00000001,
    0x0006005B, (u32)btlevtcmd_ACRStart, 0xFFFFFFFE, 0x00000005, 0xFE363C80, 0xFE363C81,
    0x00000000, 0x0003005B, (u32)btlevtcmd_ACRGetResult, 0xFE363C86, 0xFE363C87, 0x00010022,
    0xFE363C86, 0x00010029, 0x00000002, 0x0003005B, (u32)btlevtcmd_snd_voice, 0xFFFFFFFE,
    0x00000001, 0x0006005B, (u32)btlevtcmd_AudienceDeclareAcrobatResult, 0xFE363C8C, 0x00000001, 0x00000000,
    0x00000000, 0x00000000, 0x0005005B, (u32)btlevtcmd_SetRotateOffset, 0xFFFFFFFE, 0x00000000,
    0x0000000E, 0x00000000, 0x00010005, 0x0000000F, 0x0005005B, (u32)btlevtcmd_AddRotate,
    0xFFFFFFFE, 0x00000000, 0x00000000, 0xFFFFFFE8, 0x00010009, 0x00000001,
    0x00000006, 0x0005005B, (u32)btlevtcmd_SetRotate, 0xFFFFFFFE, 0x00000000, 0x00000000,
    0x00000000, 0x0000002A, 0x0001005B, (u32)evt_audience_acrobat_notry, 0x00000031, 0x00000070,
    0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_VOICE_MARIO_BATT_802eef08, 0xF1194D80, 0x00000000,
    0xF1194D80, 0x0008005B, (u32)btlevtcmd_MarioJumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0xFE363C86, 0x00000000, 0xFFFFFFFF, 0x00020019, 0xFE363C85,
    0x00000001, 0x00020018, 0xFE363C85, 0x00000003, 0x0003005B, (u32)btlevtcmd_StartAvoid,
    0xFE363C83, 0x00000026, 0x00000021, 0x00020018, 0xFE363C85, 0x00000006,
    0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000027, 0x00000021, 0x00020018,
    0xFE363C85, 0x00000002, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000028,
    0x00000021, 0x0001005B, (u32)btlevtcmd_StopAC, 0x0002005B, (u32)btlevtcmd_JumpContinue, 0xFFFFFFFE,
    0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_VOICE_MARIO_LAND_802eef28, 0xF1194D80, 0x00000000,
    0xF1194D80, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_D_2_80422318,
    0x00010009, 0x00000028, 0x00010004, 0x0000005F, 0x00000021, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_J_1D_802eef48, 0x0003005B, (u32)btlevtcmd_ac_timing_flag_onoff,
    0x00000001, 0x00000001, 0x0002005B, (u32)btlevtcmd_ac_timing_get_success_frame, 0xFE363C80, 0x00020038,
    0xFE363C80, 0x00000002, 0x00010009, 0xFE363C80, 0x0003005B, (u32)btlevtcmd_ac_timing_flag_onoff,
    0x00000000, 0x00000001, 0x0001005B, (u32)btlevtcmd_ResultAC, 0x0002005B, (u32)btlevtcmd_GetResultAC,
    0xFE363C86, 0x0002001F, 0xFE363C86, 0x00000002, 0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult,
    0xFE363C8C, 0xFFFFFFFF, 0x0007005B, (u32)btlevtcmd_CheckDamage, 0xFFFFFFFE, 0xFE363C83,
    0xFE363C84, 0xFE363C8C, 0x00000100, 0xFE363C85, 0x0003005B, (u32)evt_btl_camera_set_mode,
    0x00000000, 0x00000000, 0x0001005B, (u32)btlevtcmd_StopAC, 0x0001005B, (u32)evt_audience_ap_recovery,
    0x0001005B, (u32)btlevtcmd_InviteApInfoReport, 0x0002005B, (u32)btlevtcmd_WeaponAftereffect, 0xFE363C8C, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_J_1B_802eef00, 0x0003005B, (u32)btlevtcmd_SetFallAccel,
    0xFFFFFFFE, 0xF24A7D4C, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xF1194D80,
    0xFE363C81, 0xF1194D80, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE, 0xFE363C80,
    0x0000001B, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000014, 0xFFFFFFFF, 0x0003005B, (u32)btlevtcmd_SetFallAccel, 0xFFFFFFFE,
    0xF24A7CE6, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE, 0xFE363C80, 0x00000014,
    0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0000000C, 0xFFFFFFFF, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE, 0xFE363C80,
    0x00000008, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000008, 0xFFFFFFFF, 0x00010004, 0x0000005F, 0x00000021,
    0x0000006B, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000000, 0x0003005B,
    (u32)evt_btl_camera_set_zoom, 0x00000000, 0x000000C8, 0x00010009, 0x0000000A, 0x0003005B,
    (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001, 0x0003005B, (u32)evt_btl_camera_set_zoom, 0x00000000,
    0x000000FA, 0x0000006D, 0x0007005B, (u32)btlevtcmd_CheckDamage, 0xFFFFFFFE, 0xFE363C83,
    0xFE363C84, 0xFE363C8C, 0x00020000, 0xFE363C85, 0x00020035, 0xFE363C8A,
    0x00000001, 0x0006005B, (u32)btlevtcmd_GetHitPos, 0xFE363C83, 0xFE363C84, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0004005B, (u32)btlevtcmd_GetResultPrizeLv, 0xFE363C83, 0x00000000,
    0xFE363C86, 0x0005005B, (u32)btlevtcmd_ACSuccessEffect, 0xFE363C86, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000021, 0x0001006F, 0xFE363C8E, 0x0003005B, (u32)btlevtcmd_SetFallAccel,
    0xFFFFFFFE, 0xF24A7C80, 0x0006005B, (u32)btlevtcmd_GetHitPos, 0xFE363C83, 0xFE363C84,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00020035, 0xFE363C82, 0x00000005,
    0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_J_1B_802eef00, 0x0006005B,
    (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_VOICE_MARIO_BATT_802eef08, 0xF1194D80, 0x00000000, 0xF1194D80,
    0x00020032, 0xFE363C86, 0x00000024, 0x0006005B, (u32)btlevtcmd_MarioJumpParam, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0xFE363C86, 0x0008005B, (u32)btlevtcmd_MarioJumpPosition,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0xFE363C86, 0x00000000,
    0xFFFFFFFF, 0x00000070, 0x00010009, 0x0000000F, 0x0001005F, 0xFE363C8E,
    0x00010004, 0x00000037, 0x00010003, 0x00000034, 0x0004005B, (u32)btlevtcmd_AnimeChangePose,
    0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_N_7_80422328, 0x00010009, 0x0000001E, 0x0005005B,
    (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B,
    (u32)btlevtcmd_TransStageFloorPosition, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0003005B, (u32)btlevtcmd_SetFallAccel,
    0xFFFFFFFE, 0xF24A7C80, 0x0006005B, (u32)btlevtcmd_FallPosition, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0000000E, 0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE,
    (u32)(char*)str_SFX_VOICE_MARIO_LAND_802eef28, 0xF1194D80, 0x00000000, 0xF1194D80, 0x0004005B, (u32)btlevtcmd_AnimeChangePose,
    0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_D_2_80422318, 0x00010009, 0x0000001C, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_U_3_80422330, 0x00010009, 0x0000001E,
    0x0001005B, (u32)btlevtcmd_StopAC, 0x0001005B, (u32)evt_audience_ap_recovery, 0x0001005B, (u32)btlevtcmd_InviteApInfoReport,
    0x00010004, 0x0000005F, 0x00010003, 0x00000037, 0x0003005B, (u32)btlevtcmd_GetFaceDirection,
    0xFFFFFFFE, 0xFE363C8F, 0x00020037, 0xFE363C8F, 0xFFFFFFE2, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_A_1A_802eef50, 0x0005005B, (u32)btlevtcmd_SetRotateOffset,
    0xFFFFFFFE, 0x00000000, 0x0000000C, 0x00000000, 0x0005005B, (u32)btlevtcmd_SetRotate,
    0xFFFFFFFE, 0x00000000, 0x00000000, 0x00000000, 0x00010009, 0x0000000F,
    0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_BTL_JUMP_KURURIN_802eef58, 0xF1194D80, 0x00000000,
    0xF1194D80, 0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_VOICE_MARIO_POWE_802eef70, 0xF1194D80,
    0x00000000, 0xF1194D80, 0x00010005, 0x0000000C, 0x0005005B, (u32)btlevtcmd_AddRotate,
    0xFFFFFFFE, 0x00000000, 0x00000000, 0xFE363C8F, 0x00010009, 0x00000001,
    0x00000006, 0x0005005B, (u32)btlevtcmd_SetRotate, 0xFFFFFFFE, 0x00000000, 0x00000000,
    0x00000000, 0x0005005B, (u32)btlevtcmd_SetRotateOffset, 0xFFFFFFFE, 0x00000000, 0x00000000,
    0x00000000, 0x00010009, 0x0000000F, 0x0006005B, (u32)btlevtcmd_GetHitPos, 0xFE363C83,
    0xFE363C84, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000A005B, (u32)btlevtcmd_DivePosition,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000004, 0x00000000,
    0x00000000, 0x00000000, 0xFFFFFFFF, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE,
    0x00000001, (u32)(char*)str_M_A_1B_802eef88, 0x00010009, 0x00000002, 0x0003005B, (u32)btlevtcmd_ac_timing_flag_onoff,
    0x00000001, 0x00000001, 0x0002005B, (u32)btlevtcmd_ac_timing_get_success_frame, 0xFE363C80, 0x00020038,
    0xFE363C80, 0x00000002, 0x00020035, 0xFE363C80, 0x00000001, 0x00010009,
    0xFE363C80, 0x0003005B, (u32)btlevtcmd_ac_timing_flag_onoff, 0x00000000, 0x00000001, 0x0003005B,
    (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000, 0x0001005B, (u32)btlevtcmd_ResultAC, 0x0002005B,
    (u32)btlevtcmd_GetResultAC, 0xFE363C86, 0x0002001E, 0xFE363C86, 0x00000002, 0x0007005B,
    (u32)btlevtcmd_CheckDamage, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84, (u32)marioWeapon_KururinJump2, 0x00020100,
    0xFE363C85, 0x0006005B, (u32)btlevtcmd_GetHitPos, 0xFE363C83, 0xFE363C84, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0004005B, (u32)btlevtcmd_GetResultPrizeLv, 0xFE363C83, 0x00000001,
    0xFE363C86, 0x0005005B, (u32)btlevtcmd_ACSuccessEffect, 0xFE363C86, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult, (u32)marioWeapon_KururinJump2, 0xFE363C86, 0x0003005B,
    (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000, 0x0001005B, (u32)btlevtcmd_StopAC, 0x0001005B,
    (u32)evt_audience_ap_recovery, 0x0001005B, (u32)btlevtcmd_InviteApInfoReport, 0x0002005B, (u32)btlevtcmd_WeaponAftereffect, 0xFE363C8C,
    0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_J_1B_802eef00, 0x0003005B,
    (u32)btlevtcmd_SetFallAccel, 0xFFFFFFFE, 0xF24A7D4C, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE,
    0xF1194D80, 0xFE363C81, 0xF1194D80, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE,
    0xFE363C80, 0x0000003C, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0000001E, 0xFFFFFFFF, 0x0003005B, (u32)btlevtcmd_SetFallAccel,
    0xFFFFFFFE, 0xF24A7CE6, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE, 0xFE363C80,
    0x00000014, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0000000C, 0xFFFFFFFF, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE,
    0xFE363C80, 0x00000008, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000008, 0xFFFFFFFF, 0x00000020, 0x0007005B,
    (u32)btlevtcmd_CheckDamage, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84, (u32)marioWeapon_KururinJump2, 0x00000100,
    0xFE363C85, 0x0004005B, (u32)btlevtcmd_GetResultPrizeLv, 0xFE363C83, 0x00000000, 0xFE363C86,
    0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult, (u32)marioWeapon_KururinJump2, 0xFE363C86, 0x0003005B, (u32)evt_btl_camera_set_mode,
    0x00000000, 0x00000000, 0x0001005B, (u32)btlevtcmd_StopAC, 0x0001005B, (u32)evt_audience_ap_recovery,
    0x0001005B, (u32)btlevtcmd_InviteApInfoReport, 0x0002005B, (u32)btlevtcmd_WeaponAftereffect, 0xFE363C8C, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_J_1B_802eef00, 0x0003005B, (u32)btlevtcmd_SetFallAccel,
    0xFFFFFFFE, 0xF24A7D4C, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xF1194D80,
    0xFE363C81, 0xF1194D80, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE, 0xFE363C80,
    0x0000001B, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000014, 0xFFFFFFFF, 0x0003005B, (u32)btlevtcmd_SetFallAccel, 0xFFFFFFFE,
    0xF24A7CE6, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE, 0xFE363C80, 0x00000014,
    0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0000000C, 0xFFFFFFFF, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE, 0xFE363C80,
    0x00000008, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000008, 0xFFFFFFFF, 0x00000021, 0x00010004, 0x0000005F,
    0x00010003, 0x0000005A, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000,
    0x0001005B, (u32)btlevtcmd_StopAC, 0x0001005B, (u32)evt_audience_ap_recovery, 0x0001005B, (u32)btlevtcmd_InviteApInfoReport,
    0x0002005B, (u32)btlevtcmd_WeaponAftereffect, 0xFE363C8C, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE,
    0x00000001, (u32)(char*)str_M_J_1B_802eef00, 0x0003005B, (u32)btlevtcmd_SetFallAccel, 0xFFFFFFFE, 0xF24A7D4C,
    0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xF1194D80, 0xFE363C81, 0xF1194D80,
    0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE, 0xFE363C80, 0x0000003C, 0x0007005B,
    (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0000001E,
    0xFFFFFFFF, 0x0003005B, (u32)btlevtcmd_SetFallAccel, 0xFFFFFFFE, 0xF24A7CE6, 0x0004005B,
    (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE, 0xFE363C80, 0x00000014, 0x0007005B, (u32)btlevtcmd_JumpPosition,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0000000C, 0xFFFFFFFF,
    0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE, 0xFE363C80, 0x00000008, 0x0007005B,
    (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000008,
    0xFFFFFFFF, 0x00010003, 0x0000005F, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000,
    0x00000000, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_W_1_80422320, 0x0003005B, (u32)btlevtcmd_SetMoveSpeed,
    0xFFFFFFFE, 0xF24A9280, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001,
    0x00000028, 0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000000, 0xFFFFFFFF, 0x00000000, 0x00010003, 0x00000063,
    0x0002005B, (u32)btlevtcmd_ResetFaceDirection, 0xFFFFFFFE, 0x0001005E, (u32)btldefaultevt_SuitoruBadgeEffect, 0x0002005B,
    (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002, 0x00000001,
};

/* .data + 0x0000463C */
u32 marioAttackEvent_JyabaraJump[] = {
    0x0005005B, (u32)btlevtcmd_JumpSetting, 0xFFFFFFFE, 0x00000014, 0xF24A7A80, 0xF24A7D4C,
    0x0003005B, (u32)btlevtcmd_GetSelectEnemy, 0xFE363C83, 0xFE363C84, 0x00020018, 0xFE363C83,
    0xFFFFFFFF, 0x00010004, 0x00000063, 0x00000021, 0x0004005B, (u32)btlevtcmd_AttackDeclare,
    0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0x0001005B, (u32)btlevtcmd_WaitGuardMove, 0x0003005B,
    (u32)btlevtcmd_CommandGetWeaponAddress, 0xFFFFFFFE, 0xFE363C8C, 0x0003005B, (u32)btlevtcmd_PayWeaponCost, 0xFFFFFFFE,
    0xFE363C8C, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFFFFFFFE, 0x00000007, 0x0001005E,
    (u32)marioAttackEvent_MajinaiPowerUpCheck, 0x0007005B, (u32)btlevtcmd_CalculateFaceDirection, 0xFFFFFFFE, 0xFFFFFFFF, 0xFE363C83,
    0xFE363C84, 0x00000010, 0xFE363C8F, 0x0003005B, (u32)btlevtcmd_ChangeFaceDirection, 0xFFFFFFFE,
    0xFE363C8F, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001, 0x00000028,
    0x0003005B, (u32)btlevtcmd_GetWeaponActionLv, 0xFE363C8C, 0xFE363C80, 0x0003005B, (u32)btlevtcmd_AcSetDifficulty,
    0xFFFFFFFE, 0xFE363C80, 0x0005005B, (u32)btlevtcmd_SetupAC, 0xFFFFFFFE, 0x00000001,
    0x00000001, 0x00000000, 0x00010009, 0x00000015, 0x0002005B, (u32)btlevtcmd_StartAC,
    0x00000001, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_J_1B_802eef00,
    0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0003005B, (u32)btlevtcmd_SetFallAccel, 0xFFFFFFFE, 0xF24A7C80, 0x0007005B, (u32)btlevtcmd_JumpPosition,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000014, 0xFFFFFFFF,
    0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_S_1_80422338, 0x00010009,
    0x00000001, 0x0005005B, (u32)btlevtcmd_AnimeChangePaperGroup, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_p_jyabara_802eef90,
    0x00000001, 0x0004005B, (u32)btlevtcmd_AnimeChangePaperAnime, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PM_J_1E_802eef9c,
    0x00010009, 0x00000010, 0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_VOICE_MARIO_POWE_802eefa4,
    0xF1194D80, 0x00000000, 0xF1194D80, 0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE,
    (u32)(char*)str_SFX_BTL_JUMP_JABARA1_802eefbc, 0xF1194D80, 0x00000000, 0xF1194D80, 0x0005005B, (u32)btlevtcmd_AnimeChangePaperGroup,
    0xFFFFFFFE, 0x00000001, (u32)(char*)str_p_jyabara_802eef90, 0x00000000, 0x0004005B, (u32)btlevtcmd_AnimeChangePaperAnime,
    0xFFFFFFFE, 0x00000001, (u32)(char*)str_PM_J_1A_802eefd4, 0x00010009, 0x00000008, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePaperAnime, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PM_J_1B_802eefdc, 0x0006005B, (u32)btlevtcmd_snd_se,
    0xFFFFFFFE, (u32)(char*)str_SFX_BTL_JUMP_JABARA2_802eefe4, 0xF1194D80, 0x00000000, 0xF1194D80, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePaperAnime, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PM_J_2A_802eeffc, 0x00010009, 0x00000008,
    0x0004005B, (u32)btlevtcmd_AnimeChangePaperAnime, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PM_J_2B_802ef004, 0x00010009,
    0x0000000A, 0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_BTL_JUMP_JABARA3_802ef00c, 0xF1194D80,
    0x00000000, 0xF1194D80, 0x0004005B, (u32)btlevtcmd_AnimeChangePaperAnime, 0xFFFFFFFE, 0x00000001,
    (u32)(char*)str_PM_J_3A_802ef024, 0x00010009, 0x0000000A, 0x0004005B, (u32)btlevtcmd_AnimeChangePaperAnime, 0xFFFFFFFE,
    0x00000001, (u32)(char*)str_PM_J_3B_802ef02c, 0x00010009, 0x00000014, 0x0006005B, (u32)btlevtcmd_snd_se,
    0xFFFFFFFE, (u32)(char*)str_SFX_BTL_JUMP_JABARA4_802ef034, 0xF1194D80, 0x00000000, 0xF1194D80, 0x0006005B,
    (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_VOICE_MARIO_AC3__802ef04c, 0xF1194D80, 0x00000000, 0xF1194D80,
    0x0002005B, (u32)_bgset_iron_frame_check, 0xFE363C80, 0x00020019, 0xFE363C80, 0x00000000,
    0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_J_1B_802eef00, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePaperAnime, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PM_J_1C_802ef064, 0x0001006C, 0xFE363C8E,
    0x00010005, 0x00000032, 0x0005005B, (u32)btlevtcmd_AddRotate, 0xFFFFFFFE, 0x00000000,
    0x00000030, 0x00000000, 0x00010009, 0x00000001, 0x00000006, 0x0005005B,
    (u32)btlevtcmd_SetRotate, 0xFFFFFFFE, 0x00000000, 0x00000000, 0x00000000, 0x0000006D,
    0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00020035, 0xFE363C82, 0x00000005, 0x0003005B, (u32)btlevtcmd_SetFallAccel, 0xFFFFFFFE,
    0xF24A7BB3, 0x0009005B, (u32)btlevtcmd_SetMoveSoundDataWork, 0xFFFFFFFE, (u32)(char*)str_SFX_BTL_JUMP_CONTINU_802ef06c, 0x00000000,
    0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x00000000, 0x0001006C,
    0xFE363C8F, 0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_VOICE_MARIO_BATT_802eef08, 0xF1194D80,
    0x00000000, 0xF1194D80, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0000005A, 0xFFFFFFFE, 0x0000006D, 0x0002005B,
    (u32)_wait_jyabara_hit_iron_frame, 0xFFFFFFFE, 0x0001005F, 0xFE363C8E, 0x0001005F, 0xFE363C8F,
    0x0005005B, (u32)btlevtcmd_SetRotate, 0xFFFFFFFE, 0x00000000, 0x00000000, 0x00000000,
    0x0002005B, (u32)btlevtcmd_WeaponAftereffect, (u32)marioWeapon_JyabaraJumpFailed, 0x0001005B, (u32)btlevtcmd_StopAC, 0x0006005B,
    (u32)evt_btl_camera_shake_h, 0x00000000, 0x00000002, 0x00000000, 0x0000000A, 0x0000000D,
    0x0000006E, 0x00020034, 0xFE363C80, 0xF24A7E80, 0x00010005, 0x00000005,
    0x00010009, 0x00000001, 0x0002003B, 0xFE363C80, 0xF24A7B4C, 0x0005005B,
    (u32)btlevtcmd_SetScale, 0xFFFFFFFE, 0xF24A7E80, 0xFE363C80, 0xF24A7E80, 0x00000006,
    0x00000070, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x000A005B, (u32)btlevtcmd_DivePosition, 0xFFFFFFFE, 0xFE363C80, 0x00000082,
    0xFE363C82, 0x00000005, 0x00000000, 0x00000000, 0x00000000, 0xFFFFFFFF,
    0x00010009, 0x00000010, 0x0005005B, (u32)btlevtcmd_SetScale, 0xFFFFFFFE, 0xF24A7E80,
    0xF24A7E80, 0xF24A7E80, 0x0004005B, (u32)btlevtcmd_AnimeChangePaperAnime, 0xFFFFFFFE, 0x00000001,
    (u32)(char*)str_PM_J_1H_802ef084, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C85, 0xFE363C86,
    0xFE363C87, 0x0004005B, (u32)btlevtcmd_TransStageFloorPosition, 0xFE363C85, 0xFE363C86, 0xFE363C87,
    0x0000006E, 0x00020032, 0xFE363C85, 0xFE363C86, 0x0005005B, (u32)btlevtcmd_GetPos,
    0xFFFFFFFE, 0xF1194D80, 0xFE363C89, 0xF1194D80, 0x00020036, 0xFE363C89,
    0xFE363C85, 0x00020038, 0xFE363C89, 0x00000004, 0x0005005B, (u32)btlevtcmd_GetPos,
    0xFFFFFFFE, 0xF1194D80, 0xFE363C86, 0xF1194D80, 0x00020036, 0xFE363C86,
    0xFE363C89, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C87,
    0xFE363C81, 0x0004005B, (u32)btlevtcmd_TransStageFloorPosition, 0xFE363C80, 0xFE363C87, 0xFE363C81,
    0x00020035, 0xFE363C87, 0xFE363C89, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C88, 0xFE363C81, 0x0004005B, (u32)btlevtcmd_TransStageFloorPosition, 0xFE363C80,
    0xFE363C88, 0xFE363C81, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xF1194D80,
    0xFE363C80, 0xF1194D80, 0x0005005B, (u32)evt_sub_intpl_init, 0x00000000, 0xFE363C80,
    0xFE363C86, 0x00000020, 0x00010005, 0x00000020, 0x0001005B, (u32)evt_sub_intpl_get_value,
    0x0005005B, (u32)btlevtcmd_SetPos, 0xFFFFFFFE, 0xF1194D80, 0xFE363C80, 0xF1194D80,
    0x00010009, 0x00000001, 0x00000006, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE,
    0xF1194D80, 0xFE363C80, 0xF1194D80, 0x0005005B, (u32)evt_sub_intpl_init, 0x00000000,
    0xFE363C80, 0xFE363C87, 0x00000040, 0x00010005, 0x00000040, 0x0001005B,
    (u32)evt_sub_intpl_get_value, 0x0005005B, (u32)btlevtcmd_SetPos, 0xFFFFFFFE, 0xF1194D80, 0xFE363C80,
    0xF1194D80, 0x00010009, 0x00000001, 0x00000006, 0x0005005B, (u32)btlevtcmd_GetPos,
    0xFFFFFFFE, 0xF1194D80, 0xFE363C80, 0xF1194D80, 0x0005005B, (u32)evt_sub_intpl_init,
    0x00000000, 0xFE363C80, 0xFE363C88, 0x00000020, 0x00010005, 0x00000020,
    0x0001005B, (u32)evt_sub_intpl_get_value, 0x0005005B, (u32)btlevtcmd_SetPos, 0xFFFFFFFE, 0xF1194D80,
    0xFE363C80, 0xF1194D80, 0x00010009, 0x00000001, 0x00000006, 0x00000070,
    0x0000006E, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xF1194D80,
    0xF1194D80, 0x0003005B, (u32)btlevtcmd_GetFaceDirection, 0xFFFFFFFE, 0xFE363C87, 0x00020037,
    0xFE363C87, 0xFFFFFFE2, 0x00020032, 0xFE363C81, 0xFE363C80, 0x00020035,
    0xFE363C81, 0xFE363C87, 0x0005005B, (u32)evt_sub_intpl_init, 0x0000000B, 0xFE363C80,
    0xFE363C81, 0x0000001A, 0x00010005, 0x0000001A, 0x0001005B, (u32)evt_sub_intpl_get_value,
    0x0005005B, (u32)btlevtcmd_SetPos, 0xFFFFFFFE, 0xFE363C80, 0xF1194D80, 0xF1194D80,
    0x00010009, 0x00000001, 0x00000006, 0x00010009, 0x00000006, 0x0005005B,
    (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xF1194D80, 0xF1194D80, 0x0003005B,
    (u32)btlevtcmd_GetFaceDirection, 0xFFFFFFFE, 0xFE363C87, 0x00020037, 0xFE363C87, 0x0000003C,
    0x00020032, 0xFE363C81, 0xFE363C80, 0x00020035, 0xFE363C81, 0xFE363C87,
    0x0005005B, (u32)evt_sub_intpl_init, 0x0000000B, 0xFE363C80, 0xFE363C81, 0x0000003A,
    0x00010005, 0x0000003A, 0x0001005B, (u32)evt_sub_intpl_get_value, 0x0005005B, (u32)btlevtcmd_SetPos,
    0xFFFFFFFE, 0xFE363C80, 0xF1194D80, 0xF1194D80, 0x00010009, 0x00000001,
    0x00000006, 0x00010009, 0x00000006, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE,
    0xFE363C80, 0xF1194D80, 0xF1194D80, 0x0003005B, (u32)btlevtcmd_GetFaceDirection, 0xFFFFFFFE,
    0xFE363C87, 0x00020037, 0xFE363C87, 0xFFFFFFE2, 0x00020032, 0xFE363C81,
    0xFE363C80, 0x00020035, 0xFE363C81, 0xFE363C87, 0x0005005B, (u32)btlevtcmd_GetHomePos,
    0xFFFFFFFE, 0xFE363C81, 0xF1194D80, 0xF1194D80, 0x0005005B, (u32)evt_sub_intpl_init,
    0x0000000B, 0xFE363C80, 0xFE363C81, 0x00000020, 0x00010005, 0x00000020,
    0x0001005B, (u32)evt_sub_intpl_get_value, 0x0005005B, (u32)btlevtcmd_SetPos, 0xFFFFFFFE, 0xFE363C80,
    0xF1194D80, 0xF1194D80, 0x00010009, 0x00000001, 0x00000006, 0x00000070,
    0x00010009, 0x00000082, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001,
    (u32)(char*)str_M_I_S_80422340, 0x0004005B, (u32)btlevtcmd_AnimeChangePaperAnime, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PM_J_1E_802eef9c,
    0x0001006F, 0xFE363C8F, 0x0005005B, (u32)evt_sub_intpl_init_float, 0x00000005, 0xF24A7A80,
    0xF24A7F4C, 0x0000000A, 0x00010005, 0x0000000A, 0x0003005B, (u32)evt_sub_intpl_get_float,
    0xFE363C85, 0xF1194D80, 0x0005005B, (u32)btlevtcmd_GetScale, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0005005B, (u32)btlevtcmd_SetScale, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C85, 0xFE363C82, 0x00010009, 0x00000001, 0x00000006, 0x0005005B,
    (u32)evt_sub_intpl_init_float, 0x00000007, 0xF24A7F4C, 0xF24A7E80, 0x0000001E, 0x00010005,
    0x0000001E, 0x0003005B, (u32)evt_sub_intpl_get_float, 0xFE363C85, 0xF1194D80, 0x0005005B,
    (u32)btlevtcmd_GetScale, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B,
    (u32)btlevtcmd_SetScale, 0xFFFFFFFE, 0xFE363C80, 0xFE363C85, 0xFE363C82, 0x00010009,
    0x00000001, 0x00000006, 0x00000070, 0x0002005B, (u32)btlevtcmd_WaitEventEnd, 0xFE363C8F,
    0x00010009, 0x00000014, 0x0001005B, (u32)evt_audience_ap_recovery, 0x0001005B, (u32)btlevtcmd_InviteApInfoReport,
    0x00010004, 0x0000005F, 0x00000021, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE,
    0x00000001, (u32)(char*)str_M_J_1B_802eef00, 0x0004005B, (u32)btlevtcmd_AnimeChangePaperAnime, 0xFFFFFFFE, 0x00000001,
    (u32)(char*)str_PM_J_1C_802ef064, 0x0000006E, 0x00010005, 0x00000032, 0x0005005B, (u32)btlevtcmd_AddRotate,
    0xFFFFFFFE, 0x00000000, 0x00000030, 0x00000000, 0x00010009, 0x00000001,
    0x00000006, 0x0005005B, (u32)btlevtcmd_SetRotate, 0xFFFFFFFE, 0x00000000, 0x00000000,
    0x00000000, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_A_1A_802eef50,
    0x000C005B, (u32)btlevtcmd_SetPartsBlur, 0xFFFFFFFE, 0x00000001, 0x000000FF, 0x000000FF,
    0x000000FF, 0x00000050, 0x000000FF, 0x000000FF, 0x000000FF, 0x00000000,
    0x00000000, 0x0004005B, (u32)btlevtcmd_OnPartsAttribute, 0xFFFFFFFE, 0x00000001, 0x04000000,
    0x00000070, 0x0006005B, (u32)btlevtcmd_GetHitPos, 0xFE363C83, 0xFE363C84, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00020035, 0xFE363C82, 0x00000005, 0x0003005B,
    (u32)btlevtcmd_SetFallAccel, 0xFFFFFFFE, 0xF24A7BB3, 0x0009005B, (u32)btlevtcmd_SetMoveSoundDataWork, 0xFFFFFFFE,
    (u32)(char*)str_SFX_BTL_JUMP_CONTINU_802ef06c, 0x00000000, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000,
    0x00000000, 0x0003005B, (u32)evt_sub_random, 0x00000014, 0xFE363C85, 0x00020036,
    0xFE363C85, 0x0000000A, 0x00020037, 0xFE363C85, 0x00000006, 0x00020035,
    0xFE363C85, 0x000000B4, 0x0003005B, (u32)btlevtcmd_SetFallAccel, 0xFFFFFFFE, 0xF24A7B19,
    0x0006005B, (u32)btlevtcmd_GetHitPos, 0xFE363C83, 0xFE363C84, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00020035, 0xFE363C82, 0x00000005, 0x0007005B, (u32)btlevtcmd_JumpPosition,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0xFE363C85, 0xFFFFFFFE,
    0x0007005B, (u32)btlevtcmd_PreCheckDamage, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0xFE363C8C,
    0x00000100, 0xFE363C85, 0x00020019, 0xFE363C85, 0x00000001, 0x00020018,
    0xFE363C85, 0x00000003, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000026,
    0x00000021, 0x00020018, 0xFE363C85, 0x00000006, 0x0003005B, (u32)btlevtcmd_StartAvoid,
    0xFE363C83, 0x00000027, 0x00000021, 0x00020018, 0xFE363C85, 0x00000002,
    0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000028, 0x00000021, 0x0001005B,
    (u32)btlevtcmd_StopAC, 0x0002005B, (u32)btlevtcmd_JumpContinue, 0xFFFFFFFE, 0x0004005B, (u32)btlevtcmd_AnimeChangePose,
    0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_B_2_80422348, 0x0005005B, (u32)btlevtcmd_AnimeChangePaperGroup, 0xFFFFFFFE,
    0x00000001, 0x00000000, 0x00000000, 0x000C005B, (u32)btlevtcmd_SetPartsBlur, 0xFFFFFFFE,
    0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0000006B, 0x00010009,
    0x0000000A, 0x0004005B, (u32)btlevtcmd_OffPartsAttribute, 0xFFFFFFFE, 0x00000001, 0x04000000,
    0x0000006D, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0004005B, (u32)btlevtcmd_FaceDirectionAdd, 0xFFFFFFFE, 0xFE363C80, 0x00000023,
    0x0003005B, (u32)btlevtcmd_SetFallAccel, 0xFFFFFFFE, 0xF24A7C80, 0x0005005B, (u32)btlevtcmd_SetRotateOffset,
    0xFFFFFFFE, 0x00000000, 0x0000000C, 0x00000000, 0x0000006E, 0x0003005B,
    (u32)btlevtcmd_GetFaceDirection, 0xFFFFFFFE, 0xFE363C80, 0x00020037, 0xFE363C80, 0xFFFFFFE8,
    0x00010005, 0x00000014, 0x0005005B, (u32)btlevtcmd_AddRotate, 0xFFFFFFFE, 0x00000000,
    0x00000000, 0xFE363C80, 0x00010009, 0x00000001, 0x00000006, 0x00000070,
    0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00000014, 0xFFFFFFFF, 0x0004005B, (u32)btlevtcmd_FaceDirectionAdd, 0xFFFFFFFE, 0xFE363C80,
    0x00000014, 0x0003005B, (u32)btlevtcmd_SetFallAccel, 0xFFFFFFFE, 0xF24A7C80, 0x0000006E,
    0x0003005B, (u32)btlevtcmd_GetFaceDirection, 0xFFFFFFFE, 0xFE363C80, 0x00020037, 0xFE363C80,
    0xFFFFFFEC, 0x00010005, 0x0000000C, 0x0005005B, (u32)btlevtcmd_AddRotate, 0xFFFFFFFE,
    0x00000000, 0x00000000, 0xFE363C80, 0x00010009, 0x00000001, 0x00000006,
    0x00000070, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0000000C, 0xFFFFFFFF, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE,
    0x00000001, (u32)(char*)str_M_Z_11_802ef08c, 0x0004005B, (u32)btlevtcmd_FaceDirectionAdd, 0xFFFFFFFE, 0xFE363C80,
    0x0000000A, 0x000A005B, (u32)btlevtcmd_DivePosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000014, 0x00000000, 0x00000004, 0x00000000, 0xFFFFFFFF,
    0x00010009, 0x0000001E, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001,
    (u32)(char*)str_M_U_3_80422330, 0x00010009, 0x0000000C, 0x00010004, 0x0000005F, 0x00000021,
    0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_J_1D_802eef48, 0x00010009,
    0x00000002, 0x0005005B, (u32)btlevtcmd_AnimeChangePaperGroup, 0xFFFFFFFE, 0x00000001, 0x00000000,
    0x00000000, 0x000C005B, (u32)btlevtcmd_SetPartsBlur, 0xFFFFFFFE, 0x00000001, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x0000006B, 0x00010009, 0x0000000A, 0x0004005B,
    (u32)btlevtcmd_OffPartsAttribute, 0xFFFFFFFE, 0x00000001, 0x04000000, 0x0000006D, 0x0003005B,
    (u32)btlevtcmd_ac_timing_flag_onoff, 0x00000001, 0x00000001, 0x0002005B, (u32)btlevtcmd_ac_timing_get_success_frame, 0xFE363C80,
    0x00020038, 0xFE363C80, 0x00000002, 0x00020035, 0xFE363C80, 0x00000004,
    0x00010009, 0xFE363C80, 0x0003005B, (u32)btlevtcmd_ac_timing_flag_onoff, 0x00000000, 0x00000001,
    0x0001005B, (u32)btlevtcmd_ResultAC, 0x0002005B, (u32)btlevtcmd_GetResultAC, 0xFE363C86, 0x0002001F,
    0xFE363C86, 0x00000002, 0x0007005B, (u32)btlevtcmd_CheckDamage, 0xFFFFFFFE, 0xFE363C83,
    0xFE363C84, 0xFE363C8C, 0x00000100, 0xFE363C85, 0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult,
    0xFE363C8C, 0xFFFFFFFF, 0x0003005B, (u32)btlevtcmd_GetDamage, 0xFE363C83, 0xFE363C80,
    0x0002001D, 0xFE363C80, 0x0000000A, 0x00020032, 0xFE363C80, 0x00000009,
    0x00000021, 0x0006005B, (u32)evt_btl_camera_shake_h, 0x00000000, 0xFE363C80, 0x00000000,
    0x0000000A, 0x0000000D, 0x0002005B, (u32)btlevtcmd_WeaponAftereffect, 0xFE363C8C, 0x0001005B,
    (u32)btlevtcmd_StopAC, 0x0001005B, (u32)evt_audience_ap_recovery, 0x0001005B, (u32)btlevtcmd_InviteApInfoReport, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_J_1B_802eef00, 0x0003005B, (u32)btlevtcmd_SetFallAccel,
    0xFFFFFFFE, 0xF24A7D4C, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xF1194D80,
    0xFE363C81, 0xF1194D80, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE, 0xFE363C80,
    0x0000001B, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000014, 0xFFFFFFFF, 0x0003005B, (u32)btlevtcmd_SetFallAccel, 0xFFFFFFFE,
    0xF24A7CE6, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE, 0xFE363C80, 0x00000014,
    0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0000000C, 0xFFFFFFFF, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE, 0xFE363C80,
    0x00000008, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000008, 0xFFFFFFFF, 0x00010004, 0x0000005F, 0x00000020,
    0x0007005B, (u32)btlevtcmd_CheckDamage, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0xFE363C8C,
    0x00020000, 0xFE363C85, 0x00000021, 0x0006005B, (u32)btlevtcmd_GetHitPos, 0xFE363C83,
    0xFE363C84, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B, (u32)btlevtcmd_GetResultPrizeLv,
    0xFE363C83, 0x00000000, 0xFE363C86, 0x0005005B, (u32)btlevtcmd_ACSuccessEffect, 0xFE363C86,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0001006F, 0xFE363C8E, 0x0003005B,
    (u32)btlevtcmd_SetFallAccel, 0xFFFFFFFE, 0xF24A7C80, 0x0006005B, (u32)btlevtcmd_GetHitPos, 0xFE363C83,
    0xFE363C84, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00020035, 0xFE363C82,
    0x00000005, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_J_1B_802eef00,
    0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_VOICE_MARIO_BATT_802eef08, 0xF1194D80, 0x00000000,
    0xF1194D80, 0x00020032, 0xFE363C86, 0x00000024, 0x0006005B, (u32)btlevtcmd_MarioJumpParam,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0xFE363C86, 0x0008005B,
    (u32)btlevtcmd_MarioJumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0xFE363C86,
    0x00000000, 0xFFFFFFFF, 0x00000070, 0x00010009, 0x0000000F, 0x0001005F,
    0xFE363C8E, 0x0003005B, (u32)btlevtcmd_GetFaceDirection, 0xFFFFFFFE, 0xFE363C8F, 0x00020037,
    0xFE363C8F, 0xFFFFFFE2, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001,
    (u32)(char*)str_M_A_1A_802eef50, 0x0005005B, (u32)btlevtcmd_SetRotateOffset, 0xFFFFFFFE, 0x00000000, 0x0000000C,
    0x00000000, 0x0005005B, (u32)btlevtcmd_SetRotate, 0xFFFFFFFE, 0x00000000, 0x00000000,
    0x00000000, 0x00010009, 0x0000000F, 0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE,
    (u32)(char*)str_SFX_VOICE_MARIO_POWE_802eef70, 0xF1194D80, 0x00000000, 0xF1194D80, 0x0006005B, (u32)btlevtcmd_snd_se,
    0xFFFFFFFE, (u32)(char*)str_SFX_BTL_JUMP_JABARA5_802ef094, 0xF1194D80, 0x00000000, 0xF1194D80, 0x00010005,
    0x0000000C, 0x0005005B, (u32)btlevtcmd_AddRotate, 0xFFFFFFFE, 0x00000000, 0x00000000,
    0xFE363C8F, 0x00010009, 0x00000001, 0x00000006, 0x0005005B, (u32)btlevtcmd_SetRotate,
    0xFFFFFFFE, 0x00000000, 0x00000000, 0x00000000, 0x0005005B, (u32)btlevtcmd_SetRotateOffset,
    0xFFFFFFFE, 0x00000000, 0x00000000, 0x00000000, 0x00010009, 0x0000000F,
    0x0006005B, (u32)btlevtcmd_GetHitPos, 0xFE363C83, 0xFE363C84, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x000A005B, (u32)btlevtcmd_DivePosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000004, 0x00000000, 0x00000000, 0x00000000, 0xFFFFFFFF,
    0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_A_1B_802eef88, 0x00010009,
    0x00000002, 0x0003005B, (u32)btlevtcmd_ac_timing_flag_onoff, 0x00000001, 0x00000001, 0x0002005B,
    (u32)btlevtcmd_ac_timing_get_success_frame, 0xFE363C80, 0x00020038, 0xFE363C80, 0x00000002, 0x00020035,
    0xFE363C80, 0x00000001, 0x00010009, 0xFE363C80, 0x0003005B, (u32)btlevtcmd_ac_timing_flag_onoff,
    0x00000000, 0x00000001, 0x0001005B, (u32)btlevtcmd_ResultAC, 0x0002005B, (u32)btlevtcmd_GetResultAC,
    0xFE363C86, 0x0002001E, 0xFE363C86, 0x00000002, 0x0007005B, (u32)btlevtcmd_CheckDamage,
    0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0xFE363C8C, 0x00020100, 0xFE363C85,
    0x0006005B, (u32)btlevtcmd_GetHitPos, 0xFE363C83, 0xFE363C84, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0004005B, (u32)btlevtcmd_GetResultPrizeLv, 0xFE363C83, 0x00000001, 0xFE363C86,
    0x0005005B, (u32)btlevtcmd_ACSuccessEffect, 0xFE363C86, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult, 0xFE363C8C, 0xFE363C86, 0x00000020, 0x0007005B,
    (u32)btlevtcmd_CheckDamage, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0xFE363C8C, 0x00000100,
    0xFE363C85, 0x0004005B, (u32)btlevtcmd_GetResultPrizeLv, 0xFE363C83, 0x00000000, 0xFE363C86,
    0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult, 0xFE363C8C, 0xFE363C86, 0x0003005B, (u32)btlevtcmd_GetDamage,
    0xFE363C83, 0xFE363C80, 0x0002001D, 0xFE363C80, 0x0000000A, 0x00020032,
    0xFE363C80, 0x00000009, 0x00000021, 0x0006005B, (u32)evt_btl_camera_shake_h, 0x00000000,
    0xFE363C80, 0x00000000, 0x0000000A, 0x0000000D, 0x0002005B, (u32)btlevtcmd_WeaponAftereffect,
    0xFE363C8C, 0x0001005B, (u32)btlevtcmd_StopAC, 0x0001005B, (u32)evt_audience_ap_recovery, 0x0001005B,
    (u32)btlevtcmd_InviteApInfoReport, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_J_1B_802eef00,
    0x0003005B, (u32)btlevtcmd_SetFallAccel, 0xFFFFFFFE, 0xF24A7D4C, 0x0005005B, (u32)btlevtcmd_GetPos,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)btlevtcmd_GetHomePos,
    0xFFFFFFFE, 0xF1194D80, 0xFE363C81, 0xF1194D80, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub,
    0xFFFFFFFE, 0xFE363C80, 0x0000001B, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000014, 0xFFFFFFFF, 0x0003005B,
    (u32)btlevtcmd_SetFallAccel, 0xFFFFFFFE, 0xF24A7CE6, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE,
    0xFE363C80, 0x00000014, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0000000C, 0xFFFFFFFF, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub,
    0xFFFFFFFE, 0xFE363C80, 0x00000008, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000008, 0xFFFFFFFF, 0x00010004,
    0x0000005F, 0x00000021, 0x00010003, 0x0000005A, 0x0003005B, (u32)btlevtcmd_GetDamage,
    0xFE363C83, 0xFE363C80, 0x0002001D, 0xFE363C80, 0x0000000A, 0x00020032,
    0xFE363C80, 0x00000009, 0x00000021, 0x0006005B, (u32)evt_btl_camera_shake_h, 0x00000000,
    0xFE363C80, 0x00000000, 0x0000000A, 0x0000000D, 0x0002005B, (u32)btlevtcmd_WeaponAftereffect,
    0xFE363C8C, 0x0001005B, (u32)btlevtcmd_StopAC, 0x0001005B, (u32)evt_audience_ap_recovery, 0x0001005B,
    (u32)btlevtcmd_InviteApInfoReport, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_J_1B_802eef00,
    0x0003005B, (u32)btlevtcmd_SetFallAccel, 0xFFFFFFFE, 0xF24A7D4C, 0x0005005B, (u32)btlevtcmd_GetPos,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)btlevtcmd_GetHomePos,
    0xFFFFFFFE, 0xF1194D80, 0xFE363C81, 0xF1194D80, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub,
    0xFFFFFFFE, 0xFE363C80, 0x0000003C, 0x0000006E, 0x00010009, 0x00000001,
    0x0006005B, (u32)btlevtcmd_ACRStart, 0xFFFFFFFE, 0x00000008, 0x00000011, 0x0000001D,
    0x00000000, 0x0003005B, (u32)btlevtcmd_ACRGetResult, 0xFE363C86, 0xFE363C87, 0x00010022,
    0xFE363C86, 0x00010029, 0x00000002, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE,
    0x00000001, (u32)(char*)str_M_A_5_80422310, 0x0003005B, (u32)btlevtcmd_snd_voice, 0xFFFFFFFE, 0x00000001,
    0x0006005B, (u32)btlevtcmd_AudienceDeclareAcrobatResult, 0xFE363C8C, 0x00000001, 0x00000000, 0x00000000,
    0x00000000, 0x0000002A, 0x0001005B, (u32)evt_audience_acrobat_notry, 0x00000031, 0x00000070,
    0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0000001E, 0xFFFFFFFF, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001,
    (u32)(char*)str_M_J_1B_802eef00, 0x0003005B, (u32)btlevtcmd_SetFallAccel, 0xFFFFFFFE, 0xF24A7CE6, 0x0004005B,
    (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE, 0xFE363C80, 0x00000014, 0x0007005B, (u32)btlevtcmd_JumpPosition,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0000000C, 0xFFFFFFFF,
    0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE, 0xFE363C80, 0x00000008, 0x0007005B,
    (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000008,
    0xFFFFFFFF, 0x00010003, 0x0000005F, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000,
    0x00000000, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_W_1_80422320, 0x0003005B, (u32)btlevtcmd_SetMoveSpeed,
    0xFFFFFFFE, 0xF24A9280, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001,
    0x00000028, 0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000000, 0xFFFFFFFF, 0x00000000, 0x00010003, 0x00000063,
    0x0002005B, (u32)btlevtcmd_ResetFaceDirection, 0xFFFFFFFE, 0x0001005E, (u32)btldefaultevt_SuitoruBadgeEffect, 0x0002005B,
    (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002, 0x00000001,
};

/* .data + 0x000060BC */
u32 marioAttackEvent_GatsuDokaJump[] = {
    0x0005005B, (u32)btlevtcmd_JumpSetting, 0xFFFFFFFE, 0x00000014, 0xF24A7A80, 0xF24A7D4C,
    0x0003005B, (u32)btlevtcmd_GetSelectEnemy, 0xFE363C83, 0xFE363C84, 0x00020018, 0xFE363C83,
    0xFFFFFFFF, 0x00010004, 0x00000063, 0x00000021, 0x0004005B, (u32)btlevtcmd_AttackDeclare,
    0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0x0001005B, (u32)btlevtcmd_WaitGuardMove, 0x0002005B,
    (u32)btlevtcmd_CommandPayWeaponCost, 0xFFFFFFFE, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFFFFFFFE, 0x00000007,
    0x0001005E, (u32)marioAttackEvent_MajinaiPowerUpCheck, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x0000000B,
    0x0006005B, (u32)evt_btl_camera_set_homing_unitparts, 0x00000000, 0xFFFFFFFE, 0x00000001, 0xFE363C83,
    0xFE363C84, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001, 0x0003005B,
    (u32)evt_btl_camera_set_zoom, 0x00000000, 0x000000FA, 0x0002005B, (u32)btlevtcmd_CommandGetWeaponActionLv, 0xFE363C80,
    0x0003005B, (u32)btlevtcmd_AcSetDifficulty, 0xFFFFFFFE, 0xFE363C80, 0x0005005B, (u32)btlevtcmd_SetupAC,
    0xFFFFFFFE, 0x00000001, 0x00000001, 0x00000000, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType,
    0xFFFFFFFE, 0x00000001, 0x0000002A, 0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE,
    0xF24A8680, 0x0007005B, (u32)btlevtcmd_GetTakeoffPosition, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0007005B, (u32)btlevtcmd_CalculateFaceDirection, 0xFFFFFFFE,
    0xFFFFFFFF, 0xFE363C80, 0xFE363C82, 0x00000001, 0xFE363C8F, 0x0003005B,
    (u32)btlevtcmd_ChangeFaceDirection, 0xFFFFFFFE, 0xFE363C8F, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000, 0xFFFFFFFF, 0x00000000,
    0x0007005B, (u32)btlevtcmd_CalculateFaceDirection, 0xFFFFFFFE, 0xFFFFFFFF, 0xFE363C83, 0xFE363C84,
    0x00000010, 0xFE363C8F, 0x0003005B, (u32)btlevtcmd_ChangeFaceDirection, 0xFFFFFFFE, 0xFE363C8F,
    0x0002005B, (u32)btlevtcmd_StartAC, 0x00000001, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE,
    0x00000001, (u32)(char*)str_M_I_Y_80422350, 0x00010009, 0x0000000A, 0x0006005B, (u32)btlevtcmd_CommandPreCheckDamage,
    0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0x00000100, 0xFE363C85, 0x00010022,
    0xFE363C85, 0x0001002B, 0x00000002, 0x0001002B, 0x00000003, 0x0001002B,
    0x00000006, 0x0001002B, 0x00000004, 0x0000002E, 0x00010024, 0x00000001,
    0x0000002A, 0x0001000A, 0x000003E8, 0x00000031, 0x0004005B, (u32)btlevtcmd_AnimeChangePose,
    0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_J_1B_802eef00, 0x0006005B, (u32)btlevtcmd_GetHitPos, 0xFE363C83,
    0xFE363C84, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00020035, 0xFE363C82,
    0x00000005, 0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_VOICE_MARIO_BATT_802eef08, 0xF1194D80,
    0x00000000, 0xF1194D80, 0x00020032, 0xFE363C86, 0x00000024, 0x0006005B,
    (u32)btlevtcmd_MarioJumpParam, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0xFE363C86,
    0x0000006E, 0x00020032, 0xFE363C80, 0xFE363C86, 0x00020032, 0xFE363C81,
    0xFE363C86, 0x00020038, 0xFE363C80, 0x00000002, 0x00020036, 0xFE363C81,
    0x00000001, 0x0006005B, (u32)btlevtcmd_ACRStart, 0xFFFFFFFE, 0x00000005, 0xFE363C80,
    0xFE363C81, 0x00000000, 0x0003005B, (u32)btlevtcmd_ACRGetResult, 0xFE363C86, 0xFE363C87,
    0x00010022, 0xFE363C86, 0x00010029, 0x00000002, 0x0003005B, (u32)btlevtcmd_snd_voice,
    0xFFFFFFFE, 0x00000001, 0x0006005B, (u32)btlevtcmd_AudienceDeclareAcrobatResult, 0xFE363C8C, 0x00000001,
    0x00000000, 0x00000000, 0x00000000, 0x0005005B, (u32)btlevtcmd_SetRotateOffset, 0xFFFFFFFE,
    0x00000000, 0x0000000E, 0x00000000, 0x00010005, 0x0000000F, 0x0005005B,
    (u32)btlevtcmd_AddRotate, 0xFFFFFFFE, 0x00000000, 0x00000000, 0xFFFFFFE8, 0x00010009,
    0x00000001, 0x00000006, 0x0005005B, (u32)btlevtcmd_SetRotate, 0xFFFFFFFE, 0x00000000,
    0x00000000, 0x00000000, 0x0000002A, 0x0001005B, (u32)evt_audience_acrobat_notry, 0x00000031,
    0x00000070, 0x0008005B, (u32)btlevtcmd_MarioJumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0xFE363C86, 0x00000000, 0xFFFFFFFF, 0x00020019, 0xFE363C85,
    0x00000001, 0x00020018, 0xFE363C85, 0x00000003, 0x0003005B, (u32)btlevtcmd_StartAvoid,
    0xFE363C83, 0x00000026, 0x00000021, 0x00020018, 0xFE363C85, 0x00000006,
    0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000027, 0x00000021, 0x00020018,
    0xFE363C85, 0x00000002, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000028,
    0x00000021, 0x0001005B, (u32)btlevtcmd_StopAC, 0x0002005B, (u32)btlevtcmd_JumpContinue, 0xFFFFFFFE,
    0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_VOICE_MARIO_LAND_802eef28, 0xF1194D80, 0x00000000,
    0xF1194D80, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_D_2_80422318,
    0x00010009, 0x00000028, 0x00010004, 0x0000005F, 0x00000021, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_J_1D_802eef48, 0x0003005B, (u32)btlevtcmd_ac_timing_flag_onoff,
    0x00000001, 0x00000001, 0x0002005B, (u32)btlevtcmd_ac_timing_get_success_frame, 0xFE363C80, 0x00020038,
    0xFE363C80, 0x00000002, 0x00010009, 0xFE363C80, 0x0003005B, (u32)btlevtcmd_ac_timing_flag_onoff,
    0x00000000, 0x00000001, 0x0001005B, (u32)btlevtcmd_ResultAC, 0x0002005B, (u32)btlevtcmd_GetResultAC,
    0xFE363C86, 0x0002001F, 0xFE363C86, 0x00000002, 0x0006005B, (u32)btlevtcmd_CommandCheckDamage,
    0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0x00000100, 0xFE363C85, 0x0003005B,
    (u32)btlevtcmd_CommandGetWeaponAddress, 0xFFFFFFFE, 0xFE363C80, 0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult, 0xFE363C80,
    0xFFFFFFFF, 0x0003005B, (u32)btlevtcmd_GetDamage, 0xFE363C83, 0xFE363C80, 0x0002001D,
    0xFE363C80, 0x0000000A, 0x00020032, 0xFE363C80, 0x00000009, 0x00000021,
    0x0006005B, (u32)evt_btl_camera_shake_h, 0x00000000, 0xFE363C80, 0x00000000, 0x0000000A,
    0x0000000D, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress, 0xFFFFFFFE, 0xFE363C80, 0x0002005B,
    (u32)btlevtcmd_WeaponAftereffect, 0xFE363C80, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000,
    0x0001005B, (u32)btlevtcmd_StopAC, 0x0001005B, (u32)evt_audience_ap_recovery, 0x0001005B, (u32)btlevtcmd_InviteApInfoReport,
    0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_J_1B_802eef00, 0x0003005B,
    (u32)btlevtcmd_SetFallAccel, 0xFFFFFFFE, 0xF24A7D4C, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE,
    0xF1194D80, 0xFE363C81, 0xF1194D80, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE,
    0xFE363C80, 0x0000001B, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000014, 0xFFFFFFFF, 0x0003005B, (u32)btlevtcmd_SetFallAccel,
    0xFFFFFFFE, 0xF24A7CE6, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE, 0xFE363C80,
    0x00000014, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0000000C, 0xFFFFFFFF, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE,
    0xFE363C80, 0x00000008, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000008, 0xFFFFFFFF, 0x00010004, 0x0000005F,
    0x00000021, 0x0006005B, (u32)btlevtcmd_CommandCheckDamage, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84,
    0x00020100, 0xFE363C85, 0x0006005B, (u32)btlevtcmd_GetHitPos, 0xFE363C83, 0xFE363C84,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B, (u32)btlevtcmd_GetResultPrizeLv, 0xFE363C83,
    0x00000000, 0xFE363C86, 0x0005005B, (u32)btlevtcmd_ACSuccessEffect, 0xFE363C86, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress, 0xFFFFFFFE, 0xFE363C80,
    0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult, 0xFE363C80, 0xFE363C86, 0x00010009, 0x00000002,
    0x00010003, 0x0000005A, 0x0003005B, (u32)btlevtcmd_GetDamage, 0xFE363C83, 0xFE363C80,
    0x0002001D, 0xFE363C80, 0x0000000A, 0x00020032, 0xFE363C80, 0x00000009,
    0x00000021, 0x0006005B, (u32)evt_btl_camera_shake_h, 0x00000000, 0xFE363C80, 0x00000000,
    0x0000000A, 0x0000000D, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress, 0xFFFFFFFE, 0xFE363C80,
    0x0002005B, (u32)btlevtcmd_WeaponAftereffect, 0xFE363C80, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000,
    0x00000000, 0x0001005B, (u32)btlevtcmd_StopAC, 0x0001005B, (u32)evt_audience_ap_recovery, 0x0001005B,
    (u32)btlevtcmd_InviteApInfoReport, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_J_1B_802eef00,
    0x0003005B, (u32)btlevtcmd_SetFallAccel, 0xFFFFFFFE, 0xF24A7D4C, 0x0005005B, (u32)btlevtcmd_GetPos,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)btlevtcmd_GetHomePos,
    0xFFFFFFFE, 0xF1194D80, 0xFE363C81, 0xF1194D80, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub,
    0xFFFFFFFE, 0xFE363C80, 0x0000003C, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0000001E, 0xFFFFFFFF, 0x0003005B,
    (u32)btlevtcmd_SetFallAccel, 0xFFFFFFFE, 0xF24A7CE6, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE,
    0xFE363C80, 0x00000014, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0000000C, 0xFFFFFFFF, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub,
    0xFFFFFFFE, 0xFE363C80, 0x00000008, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000008, 0xFFFFFFFF, 0x00010003,
    0x0000005F, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000, 0x0003005B,
    (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE,
    0x00000001, (u32)(char*)str_M_W_1_80422320, 0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE, 0xF24A9280,
    0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001, 0x00000028, 0x0005005B,
    (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0008005B,
    (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000,
    0xFFFFFFFF, 0x00000000, 0x00010003, 0x00000063, 0x0002005B, (u32)btlevtcmd_ResetFaceDirection,
    0xFFFFFFFE, 0x0001005E, (u32)btldefaultevt_SuitoruBadgeEffect, 0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE,
    0x00000002, 0x00000001,
};

/* .data + 0x00006AB4 */
u32 marioAttackEvent_TatsumakiJump[] = {
    0x0005005B, (u32)btlevtcmd_JumpSetting, 0xFFFFFFFE, 0x00000014, 0xF24A7A80, 0xF24A7D4C,
    0x0003005B, (u32)btlevtcmd_GetSelectEnemy, 0xFE363C83, 0xFE363C84, 0x00020018, 0xFE363C83,
    0xFFFFFFFF, 0x00010004, 0x00000063, 0x00000021, 0x0004005B, (u32)btlevtcmd_AttackDeclare,
    0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0x0001005B, (u32)btlevtcmd_WaitGuardMove, 0x0002005B,
    (u32)btlevtcmd_CommandPayWeaponCost, 0xFFFFFFFE, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFFFFFFFE, 0x00000007,
    0x0001005E, (u32)marioAttackEvent_MajinaiPowerUpCheck, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x0000000B,
    0x0006005B, (u32)evt_btl_camera_set_homing_unitparts, 0x00000000, 0xFFFFFFFE, 0x00000001, 0xFE363C83,
    0xFE363C84, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001, 0x0003005B,
    (u32)evt_btl_camera_set_zoom, 0x00000000, 0x000000FA, 0x0002005B, (u32)btlevtcmd_CommandGetWeaponActionLv, 0xFE363C80,
    0x0003005B, (u32)btlevtcmd_AcSetDifficulty, 0xFFFFFFFE, 0xFE363C80, 0x0005005B, (u32)btlevtcmd_SetupAC,
    0xFFFFFFFE, 0x00000001, 0x00000001, 0x00000000, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType,
    0xFFFFFFFE, 0x00000001, 0x0000002A, 0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE,
    0xF24A8680, 0x0007005B, (u32)btlevtcmd_GetTakeoffPosition, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0007005B, (u32)btlevtcmd_GetMoveFrame, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000, 0xFE363C87, 0x00020036,
    0xFE363C87, 0x00000015, 0x0002001A, 0xFE363C87, 0x00000000, 0x00020037,
    0xFE363C87, 0xFFFFFFFF, 0x00010009, 0xFE363C87, 0x00000021, 0x0007005B,
    (u32)btlevtcmd_CalculateFaceDirection, 0xFFFFFFFE, 0xFFFFFFFF, 0xFE363C80, 0xFE363C82, 0x00000001,
    0xFE363C8F, 0x0003005B, (u32)btlevtcmd_ChangeFaceDirection, 0xFFFFFFFE, 0xFE363C8F, 0x0008005B,
    (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000,
    0xFFFFFFFF, 0x00000000, 0x0007005B, (u32)btlevtcmd_CalculateFaceDirection, 0xFFFFFFFE, 0xFFFFFFFF,
    0xFE363C83, 0xFE363C84, 0x00000010, 0xFE363C8F, 0x0003005B, (u32)btlevtcmd_ChangeFaceDirection,
    0xFFFFFFFE, 0xFE363C8F, 0x0002005B, (u32)btlevtcmd_StartAC, 0x00000001, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_I_Y_80422350, 0x00010009, 0x0000000A,
    0x0006005B, (u32)btlevtcmd_CommandPreCheckDamage, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0x00000100,
    0xFE363C85, 0x00010022, 0xFE363C85, 0x0001002B, 0x00000002, 0x0001002B,
    0x00000003, 0x0001002B, 0x00000006, 0x0001002B, 0x00000004, 0x0000002E,
    0x00010024, 0x00000001, 0x0000002A, 0x0001000A, 0x000003E8, 0x00000031,
    0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_J_1B_802eef00, 0x0006005B,
    (u32)btlevtcmd_GetHitPos, 0xFE363C83, 0xFE363C84, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00020035, 0xFE363C82, 0x00000005, 0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE,
    (u32)(char*)str_SFX_VOICE_MARIO_BATT_802eef08, 0xF1194D80, 0x00000000, 0xF1194D80, 0x00020032, 0xFE363C86,
    0x00000024, 0x0006005B, (u32)btlevtcmd_MarioJumpParam, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0xFE363C86, 0x0008005B, (u32)btlevtcmd_MarioJumpPosition, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0xFE363C86, 0x00000000, 0xFFFFFFFF, 0x00020019,
    0xFE363C85, 0x00000001, 0x00020018, 0xFE363C85, 0x00000003, 0x0003005B,
    (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000026, 0x00000021, 0x00020018, 0xFE363C85,
    0x00000006, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000027, 0x00000021,
    0x00020018, 0xFE363C85, 0x00000002, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83,
    0x00000028, 0x00000021, 0x0001005B, (u32)btlevtcmd_StopAC, 0x0002005B, (u32)btlevtcmd_JumpContinue,
    0xFFFFFFFE, 0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_VOICE_MARIO_LAND_802eef28, 0xF1194D80,
    0x00000000, 0xF1194D80, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001,
    (u32)(char*)str_M_D_2_80422318, 0x00010009, 0x00000028, 0x00010004, 0x0000005F, 0x00000021,
    0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_J_1D_802eef48, 0x0003005B,
    (u32)btlevtcmd_ac_timing_flag_onoff, 0x00000001, 0x00000001, 0x0002005B, (u32)btlevtcmd_ac_timing_get_success_frame, 0xFE363C80,
    0x00020038, 0xFE363C80, 0x00000002, 0x00010009, 0xFE363C80, 0x0003005B,
    (u32)btlevtcmd_ac_timing_flag_onoff, 0x00000000, 0x00000001, 0x0001005B, (u32)btlevtcmd_ResultAC, 0x0002005B,
    (u32)btlevtcmd_GetResultAC, 0xFE363C86, 0x0002001F, 0xFE363C86, 0x00000002, 0x0006005B,
    (u32)btlevtcmd_CommandCheckDamage, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0x00000100, 0xFE363C85,
    0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress, 0xFFFFFFFE, 0xFE363C80, 0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult,
    0xFE363C80, 0xFFFFFFFF, 0x00010004, 0x0000005A, 0x00000021, 0x0000006B,
    0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000000, 0x0003005B, (u32)evt_btl_camera_set_zoom,
    0x00000000, 0x000000C8, 0x00010009, 0x0000000A, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv,
    0x00000000, 0x00000002, 0x0003005B, (u32)evt_btl_camera_set_zoom, 0x00000000, 0x000000FA,
    0x0000006D, 0x0006005B, (u32)btlevtcmd_CommandCheckDamage, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84,
    0x00020100, 0xFE363C85, 0x0003005B, (u32)btlevtcmd_GetFriendBelong, 0xFE363C83, 0xFE363C8F,
    0x0002005B, (u32)btlevtcmd_GetResultAC, 0xFE363C80, 0x0002001E, 0xFE363C80, 0x00000002,
    0x0006005B, (u32)btlevtcmd_GetHitPos, 0xFE363C83, 0xFE363C84, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0004005B, (u32)btlevtcmd_GetResultPrizeLv, 0xFE363C83, 0x00000000, 0xFE363C86,
    0x0005005B, (u32)btlevtcmd_ACSuccessEffect, 0xFE363C86, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00000021, 0x00010009, 0x00000002, 0x0001005B, (u32)btlevtcmd_StopAC, 0x0001006C,
    0xFE363C8D, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_J_1B_802eef00,
    0x0003005B, (u32)btlevtcmd_SetFallAccel, 0xFFFFFFFE, 0xF24A7B19, 0x0005005B, (u32)btlevtcmd_GetPos,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B, (u32)btlevtcmd_TransStageFloorPosition,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0008005B, (u32)btlevtcmd_MarioJumpPosition, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000050, 0x00000003, 0xFFFFFFFF,
    0x0000006D, 0x0000006E, 0x00010005, 0x00000018, 0x0005005B, (u32)btlevtcmd_AddRotate,
    0xFFFFFFFE, 0x00000000, 0x0000000F, 0x00000000, 0x00010009, 0x00000001,
    0x00000006, 0x00000070, 0x00010009, 0x0000001B, 0x0001005F, 0xFE363C8D,
    0x0002005B, (u32)btlevtcmd_CommandGetWeaponActionLv, 0xFE363C80, 0x0003005B, (u32)btlevtcmd_AcSetDifficulty, 0xFFFFFFFE,
    0xFE363C80, 0x0002005B, (u32)btlevtcmd_AcGetDifficulty, 0xFE363C80, 0x00020036, 0xFE363C80,
    0x00000003, 0x00010022, 0xFE363C80, 0x00010028, 0xFFFFFFFD, 0x0003005B,
    (u32)btlevtcmd_ftof, 0x00000258, 0xFE363C80, 0x00010024, 0xFFFFFFFE, 0x0003005B,
    (u32)btlevtcmd_ftof, 0x000001E0, 0xFE363C80, 0x00010024, 0xFFFFFFFF, 0x0003005B,
    (u32)btlevtcmd_ftof, 0x000001A4, 0xFE363C80, 0x00010024, 0x00000000, 0x0003005B,
    (u32)btlevtcmd_ftof, 0x00000168, 0xFE363C80, 0x00010024, 0x00000001, 0x0003005B,
    (u32)btlevtcmd_ftof, 0x0000012C, 0xFE363C80, 0x00010024, 0x00000002, 0x0003005B,
    (u32)btlevtcmd_ftof, 0x000000C8, 0xFE363C80, 0x0000002A, 0x0003005B, (u32)btlevtcmd_ftof,
    0x00000064, 0xFE363C80, 0x00000031, 0x0009005B, (u32)btlevtcmd_AcSetParamAll, 0xFE363C80,
    0x00000001, 0x00000003, 0xFFFFFFFC, 0x00000003, 0xF1194D80, 0xF1194D80,
    0xF1194D80, 0x0002005B, (u32)btlevtcmd_AcSetFlag, 0x00000000, 0x0005005B, (u32)btlevtcmd_SetupAC,
    0xFFFFFFFE, 0x0000000A, 0x00000001, 0x00000000, 0x00010009, 0x00000001,
    0x0002005B, (u32)btlevtcmd_StartAC, 0x00000001, 0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFD,
    0x00000005, 0x00000000, 0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFD, 0x00000006,
    0x00000000, 0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFD, 0x00000007, 0x00000000,
    0x0001006C, 0xFE363C8D, 0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000002,
    0x00000001, 0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000003, 0x0000001E,
    0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000004, 0x00000000, 0x00020032,
    0xFE363C80, 0x0000000F, 0x00010003, 0x0000003C, 0x0004005B, (u32)btlevtcmd_GetUnitWork,
    0xFFFFFFFE, 0x00000002, 0xFE363C81, 0x00020018, 0xFE363C81, 0x00000000,
    0x00010004, 0x0000003D, 0x00000021, 0x0004005B, (u32)btlevtcmd_GetUnitWork, 0xFFFFFFFE,
    0x00000003, 0xFE363C8A, 0x00020035, 0xFE363C80, 0x00000001, 0x0002001B,
    0xFE363C80, 0xFE363C8A, 0x00020032, 0xFE363C80, 0xFE363C8A, 0x00000021,
    0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000004, 0xFE363C80, 0x0005005B,
    (u32)btlevtcmd_AddRotate, 0xFFFFFFFE, 0x00000000, 0xFE363C80, 0x00000000, 0x00010009,
    0x00000001, 0x00010004, 0x0000003C, 0x00010003, 0x0000003D, 0x0000006D,
    0x0001006F, 0xFE363C8E, 0x00020032, 0xFE363C80, 0x00000000, 0x00010005,
    0x000003E8, 0x00010009, 0x00000001, 0x0003005B, (u32)btlevtcmd_AcGetOutputParam, 0x00000000,
    0xFE363C81, 0x00020019, 0xFE363C81, 0xFE363C80, 0x00020032, 0xFE363C80,
    0xFE363C81, 0x00010022, 0xFE363C80, 0x00010024, 0x00000000, 0x00010024,
    0x00000001, 0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFD, 0x00000005, 0x000003FF,
    0x0004005B, (u32)btlevtcmd_GetUnitWork, 0xFFFFFFFD, 0x00000006, 0xFE363C8A, 0x00020036,
    0xFE363C8A, 0x000003FF, 0x00020037, 0xFE363C8A, 0xFFFFFFFF, 0x00020038,
    0xFE363C8A, 0x0000000A, 0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFD, 0x00000007,
    0xFE363C8A, 0x00010024, 0x00000002, 0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE,
    0x00000003, 0x0000002D, 0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFD, 0x00000005,
    0x000007FE, 0x0004005B, (u32)btlevtcmd_GetUnitWork, 0xFFFFFFFD, 0x00000006, 0xFE363C8A,
    0x00020036, 0xFE363C8A, 0x000007FE, 0x00020037, 0xFE363C8A, 0xFFFFFFFF,
    0x00020038, 0xFE363C8A, 0x0000000A, 0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFD,
    0x00000007, 0xFE363C8A, 0x00010024, 0x00000003, 0x0004005B, (u32)btlevtcmd_SetUnitWork,
    0xFFFFFFFD, 0x00000005, 0x00000BFD, 0x0004005B, (u32)btlevtcmd_GetUnitWork, 0xFFFFFFFD,
    0x00000006, 0xFE363C8A, 0x00020036, 0xFE363C8A, 0x00000BFD, 0x00020037,
    0xFE363C8A, 0xFFFFFFFF, 0x00020038, 0xFE363C8A, 0x0000000A, 0x0004005B,
    (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFD, 0x00000007, 0xFE363C8A, 0x0000002A, 0x0004005B,
    (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000003, 0x0000003C, 0x00000031, 0x00000021,
    0x00000006, 0x00000070, 0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_BTL_JUMP_TATUMAK_802ef0ac, 0xFE363C8B,
    0x0001006F, 0xFE363C8C, 0x00020032, 0xFE363C85, 0x00000005, 0x00010005,
    0xFE363C85, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0005005B, (u32)evt_snd_sfx_pos, 0xFE363C8B, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0004005B, (u32)btlevtcmd_GetUnitWork, 0xFFFFFFFD, 0x00000005, 0xFE363C81,
    0x0004005B, (u32)btlevtcmd_GetUnitWork, 0xFFFFFFFD, 0x00000006, 0xFE363C80, 0x00020019,
    0xFE363C80, 0xFE363C81, 0x0004005B, (u32)btlevtcmd_GetUnitWork, 0xFFFFFFFD, 0x00000007,
    0xFE363C82, 0x00020035, 0xFE363C80, 0xFE363C82, 0x0002001D, 0xFE363C82,
    0x00000000, 0x0002001D, 0xFE363C80, 0xFE363C81, 0x00020032, 0xFE363C80,
    0xFE363C81, 0x00000021, 0x00000020, 0x0002001C, 0xFE363C80, 0xFE363C81,
    0x00020032, 0xFE363C80, 0xFE363C81, 0x00000021, 0x00000021, 0x0004005B,
    (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFD, 0x00000006, 0xFE363C80, 0x0003005B, (u32)evt_snd_sfx_pit,
    0xFE363C8B, 0xFE363C80, 0x00000021, 0x00010009, 0x00000001, 0x00020032,
    0xFE363C85, 0x00000005, 0x00000006, 0x00000070, 0x0001005B, (u32)btlevtcmd_ResultAC,
    0x00010009, 0x00000001, 0x0002005B, (u32)btlevtcmd_GetResultAC, 0xFE363C86, 0x0002001F,
    0xFE363C86, 0x00000002, 0x0001005F, 0xFE363C8E, 0x0000006E, 0x0004005B,
    (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFD, 0x00000005, 0xFFFFF558, 0x0004005B, (u32)btlevtcmd_GetUnitWork,
    0xFFFFFFFD, 0x00000006, 0xFE363C8A, 0x00020036, 0xFE363C8A, 0xFFFFF558,
    0x00020037, 0xFE363C8A, 0xFFFFFFFF, 0x00020038, 0xFE363C8A, 0x00000028,
    0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFD, 0x00000007, 0xFE363C8A, 0x00000070,
    0x0000006E, 0x0005005B, (u32)btlevtcmd_GetRotate, 0xFFFFFFFE, 0xF1194D80, 0xFE363C81,
    0xF1194D80, 0x00010005, 0x00000028, 0x0004005B, (u32)btlevtcmd_GetUnitWork, 0xFFFFFFFE,
    0x00000003, 0xFE363C80, 0x0002001B, 0xFE363C80, 0x0000000A, 0x00020036,
    0xFE363C80, 0x00000001, 0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000003,
    0xFE363C80, 0x00000021, 0x0005005B, (u32)btlevtcmd_GetRotate, 0xFFFFFFFE, 0xF1194D80,
    0xFE363C80, 0xF1194D80, 0x0002001A, 0xFE363C81, 0x0000005A, 0x0002001D,
    0xFE363C80, 0x0000005A, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001,
    (u32)(char*)str_M_N_7_80422328, 0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_BTL_MARIO_FALL_C_802ef0c4, 0xF1194D80,
    0x00000000, 0xF1194D80, 0x00000021, 0x00000021, 0x00020032, 0xFE363C81,
    0xFE363C80, 0x00010009, 0x00000001, 0x00000006, 0x00000070, 0x00010009,
    0x00000028, 0x0001005F, 0xFE363C8D, 0x0001005F, 0xFE363C8C, 0x0002005B,
    (u32)evt_snd_sfxoff, 0xFE363C8B, 0x0001006F, 0xFE363C8D, 0x00010005, 0x0000003C,
    0x0005005B, (u32)btlevtcmd_GetRotate, 0xFFFFFFFE, 0xF1194D80, 0xFE363C80, 0xF1194D80,
    0x0004005B, (u32)btlevtcmd_GetUnitWork, 0xFFFFFFFE, 0x00000003, 0xFE363C81, 0x00020035,
    0xFE363C80, 0xFE363C81, 0x0002001D, 0xFE363C80, 0x00000168, 0x0005005B,
    (u32)btlevtcmd_SetRotate, 0xFFFFFFFE, 0x00000000, 0x00000000, 0x00000000, 0x00000007,
    0x00000021, 0x0005005B, (u32)btlevtcmd_SetRotate, 0xFFFFFFFE, 0x00000000, 0xFE363C80,
    0x00000000, 0x00010009, 0x00000001, 0x00000006, 0x00000070, 0x0002005B,
    (u32)btlevtcmd_WaitEventEnd, 0xFE363C8D, 0x0005005B, (u32)btlevtcmd_SetRotate, 0xFFFFFFFE, 0x00000000,
    0x00000000, 0x00000000, 0x00010009, 0x0000003C, 0x0005005B, (u32)btlevtcmd_GetPos,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)btlevtcmd_JumpSetting,
    0xFFFFFFFE, 0x00000028, 0xF24A7A80, 0xF24A7AE6, 0x0004005B, (u32)btlevtcmd_TransStageFloorPosition,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0000006E, 0x00010009, 0x0000000C,
    0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_VOICE_MARIO_LAND_802eef28, 0xF1194D80, 0x00000000,
    0xF1194D80, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_D_2_80422318,
    0x00000070, 0x0006005B, (u32)btlevtcmd_FallPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000010, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress, 0xFFFFFFFE, 0xFE363C80,
    0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult, 0xFE363C80, 0xFFFFFFFF, 0x00010009, 0x00000020,
    0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_U_3_80422330, 0x00010009,
    0x0000000C, 0x0001005B, (u32)evt_audience_ap_recovery, 0x0001005B, (u32)btlevtcmd_InviteApInfoReport, 0x00010004,
    0x0000005F, 0x00000020, 0x0001005F, 0xFE363C8E, 0x0004005B, (u32)btlevtcmd_SetUnitWork,
    0xFFFFFFFE, 0x00000003, 0x0000002D, 0x00010003, 0x0000003E, 0x0004005B,
    (u32)btlevtcmd_GetUnitWork, 0xFFFFFFFE, 0x00000004, 0xFE363C80, 0x0002001A, 0xFE363C80,
    0x0000002C, 0x00010009, 0x00000001, 0x00010004, 0x0000003E, 0x00000021,
    0x0000006E, 0x00020032, 0xFE363C86, 0xFE363C83, 0x0002005B, (u32)_tatsumaki_effect,
    0xFFFFFFFE, 0x00020032, 0xFE363C80, 0xFE363C8F, 0x0004005B, (u32)btlevtcmd_SamplingEnemy,
    0xFFFFFFFB, 0xFE363C80, (u32)badgeWeapon_TatsumakiJumpInvolved, 0x0003005B, (u32)btlevtcmd_GetSelectEnemy, 0xFE363C83,
    0xFE363C84, 0x00010003, 0x00000032, 0x00020018, 0xFE363C83, 0xFFFFFFFF,
    0x00010004, 0x0000003B, 0x00000021, 0x00020018, 0xFE363C83, 0xFE363C86,
    0x00010004, 0x0000003A, 0x00000021, 0x0007005B, (u32)btlevtcmd_PreCheckDamage, 0xFFFFFFFB,
    0xFE363C83, 0xFE363C84, (u32)badgeWeapon_TatsumakiJumpInvolved, 0x00000100, 0xFE363C85, 0x00010022,
    0xFE363C85, 0x00010024, 0x00000002, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83,
    0x00000028, 0x00010024, 0x00000003, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83,
    0x00000026, 0x00010024, 0x00000006, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83,
    0x00000027, 0x00010024, 0x00000004, 0x00010024, 0x00000001, 0x0002005B,
    (u32)btlevtcmd_GetResultAC, 0xFE363C80, 0x0002001E, 0xFE363C80, 0x00000002, 0x0007005B,
    (u32)btlevtcmd_CheckDamage, 0xFFFFFFFB, 0xFE363C83, 0xFE363C84, (u32)badgeWeapon_TatsumakiJumpInvolved, 0x00020100,
    0xFE363C85, 0x0006005B, (u32)btlevtcmd_GetHitPos, 0xFE363C83, 0xFE363C84, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0004005B, (u32)btlevtcmd_GetResultPrizeLv, 0xFE363C83, 0x00000001,
    0xFE363C87, 0x0005005B, (u32)btlevtcmd_ACSuccessEffect, 0xFE363C87, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000020, 0x0007005B, (u32)btlevtcmd_CheckDamage, 0xFFFFFFFB, 0xFE363C83,
    0xFE363C84, (u32)badgeWeapon_TatsumakiJumpInvolved, 0x00000100, 0xFE363C85, 0x00000021, 0x0000002A,
    0x00000031, 0x00010003, 0x0000003A, 0x0003005B, (u32)btlevtcmd_GetSelectNextEnemy, 0xFE363C83,
    0xFE363C84, 0x00010004, 0x00000032, 0x00010003, 0x0000003B, 0x0003005B,
    (u32)btlevtcmd_CommandGetWeaponAddress, 0xFFFFFFFE, 0xFE363C80, 0x0004005B, (u32)btlevtcmd_GetResultPrizeLv, 0xFE363C83,
    0x00000001, 0xFE363C86, 0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult, 0xFE363C80, 0xFE363C86,
    0x00000070, 0x00010009, 0x0000003C, 0x0001006C, 0xFE363C8D, 0x00010005,
    0x0000001E, 0x0004005B, (u32)btlevtcmd_GetUnitWork, 0xFFFFFFFE, 0x00000003, 0xFE363C80,
    0x00020036, 0xFE363C80, 0x00000001, 0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE,
    0x00000003, 0xFE363C80, 0x00010009, 0x00000001, 0x00000021, 0x0004005B,
    (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000002, 0x00000000, 0x0005005B, (u32)btlevtcmd_SetRotate,
    0xFFFFFFFE, 0x00000000, 0x00000000, 0x00000000, 0x00010005, 0x00000006,
    0x00010009, 0x00000001, 0x0005005B, (u32)btlevtcmd_AddRotate, 0xFFFFFFFE, 0x00000000,
    0x0000002D, 0x00000000, 0x00000006, 0x00010005, 0x00000006, 0x00010009,
    0x00000001, 0x0005005B, (u32)btlevtcmd_AddRotate, 0xFFFFFFFE, 0x00000000, 0x0000000F,
    0x00000000, 0x00000006, 0x0005005B, (u32)btlevtcmd_SetRotate, 0xFFFFFFFE, 0x00000000,
    0x00000000, 0x00000000, 0x0000006D, 0x0002005B, (u32)btlevtcmd_WaitEventEnd, 0xFE363C8D,
    0x0001005F, 0xFE363C8C, 0x0002005B, (u32)evt_snd_sfxoff, 0xFE363C8B, 0x00010009,
    0x0000001E, 0x0000006E, 0x0006005B, (u32)btlevtcmd_ACRStart, 0xFFFFFFFE, 0x00000007,
    0x00000015, 0x00000015, 0x00000000, 0x0003005B, (u32)btlevtcmd_ACRGetResult, 0xFE363C86,
    0xFE363C87, 0x0002001D, 0xFE363C86, 0x00000002, 0x0004005B, (u32)btlevtcmd_AnimeChangePose,
    0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_B_1_80422358, 0x00000021, 0x00000070, 0x0005005B,
    (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B,
    (u32)btlevtcmd_TransStageFloorPosition, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0003005B, (u32)btlevtcmd_SetFallAccel,
    0xFFFFFFFE, 0xF24A7B19, 0x0006005B, (u32)btlevtcmd_FallPosition, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000010, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE,
    0x00000001, (u32)(char*)str_M_J_1D_802eef48, 0x00010009, 0x00000006, 0x0003005B, (u32)btlevtcmd_ACRGetResult,
    0xFE363C86, 0xFE363C87, 0x0002001D, 0xFE363C86, 0x00000002, 0x00010004,
    0x00000050, 0x00000021, 0x00000021, 0x0001005B, (u32)evt_audience_ap_recovery, 0x0001005B,
    (u32)btlevtcmd_InviteApInfoReport, 0x00010004, 0x0000005F, 0x00010003, 0x00000050, 0x0003005B,
    (u32)btlevtcmd_CommandGetWeaponAddress, 0xFFFFFFFE, 0xFE363C80, 0x0006005B, (u32)btlevtcmd_AudienceDeclareAcrobatResult, 0xFE363C80,
    0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x0003005B, (u32)btlevtcmd_SetMoveSpeed,
    0xFFFFFFFE, 0xF24A8A80, 0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0007005B, (u32)btlevtcmd_GetMoveFrame, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000000, 0xFE363C87, 0x00020032, 0xFE363C88,
    0xFE363C87, 0x00020035, 0xFE363C88, 0x0000000C, 0x0000006E, 0x00010009,
    0x00000001, 0x0006005B, (u32)btlevtcmd_ACRStart, 0xFFFFFFFE, 0xFE363C87, 0xFE363C88,
    0xFE363C88, 0x00000000, 0x00000070, 0x00020035, 0xFE363C88, 0x00000001,
    0x0003005B, (u32)btlevtcmd_GetFaceDirection, 0xFFFFFFFE, 0xFE363C8F, 0x00020037, 0xFE363C8F,
    0x0000001E, 0x0005005B, (u32)btlevtcmd_SetRotateOffset, 0xFFFFFFFE, 0x00000000, 0x0000000E,
    0x00000000, 0x0000006E, 0x00020032, 0xFE363C8A, 0x00000000, 0x00010005,
    0xFE363C88, 0x0005005B, (u32)btlevtcmd_AddRotate, 0xFFFFFFFE, 0x00000000, 0x00000000,
    0xFE363C8F, 0x00020018, 0xFE363C8A, 0x00000000, 0x0003005B, (u32)btlevtcmd_snd_voice,
    0xFFFFFFFE, 0x00000001, 0x00000021, 0x00020035, 0xFE363C8A, 0x00000001,
    0x0002001D, 0xFE363C8A, 0x0000000C, 0x00020032, 0xFE363C8A, 0x00000000,
    0x00000021, 0x00010009, 0x00000001, 0x00000006, 0x00000070, 0x0008005B,
    (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0xFE363C88,
    0xFFFFFFFF, 0x00000000, 0x0003005B, (u32)btlevtcmd_GetFaceDirection, 0xFFFFFFFE, 0xFE363C85,
    0x00020037, 0xFE363C85, 0x00000024, 0x0001006C, 0xFE363C8E, 0x0005005B,
    (u32)btlevtcmd_GetRotate, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00020039,
    0xFE363C82, 0x00000168, 0x00020038, 0xFE363C82, 0x0000000A, 0x00020035,
    0xFE363C82, 0xFE363C85, 0x00010005, 0x0000000A, 0x00000006, 0x0005005B,
    (u32)btlevtcmd_SetRotate, 0xFFFFFFFE, 0x00000000, 0x00000000, 0x00000000, 0x0000006D,
    0x0003005B, (u32)btlevtcmd_ACRGetResult, 0xFE363C86, 0xFE363C87, 0x00010022, 0xFE363C86,
    0x00010029, 0x00000002, 0x0002005B, (u32)btlevtcmd_WaitEventEnd, 0xFE363C8E, 0x0003005B,
    (u32)btlevtcmd_CommandGetWeaponAddress, 0xFFFFFFFE, 0xFE363C80, 0x0006005B, (u32)btlevtcmd_AudienceDeclareAcrobatResult, 0xFE363C80,
    0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x0004005B, (u32)btlevtcmd_AnimeChangePose,
    0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_A_3B_802ef0e0, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00020035, 0xFE363C81, 0x00000032,
    0x000F005B, (u32)evt_eff, (u32)(char*)str_BTLEF_802ef0e8, (u32)(char*)str_confetti_802ef0f0, 0x00000003, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0000005A, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0003005B, (u32)btlevtcmd_snd_voice,
    0xFFFFFFFE, 0x00000001, 0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_BTL_ACROBAT_FINI_802ef0fc, 0x00000000,
    0x00010009, 0x0000005A, 0x00010028, 0xFFFFFFFF, 0x0001005F, 0xFE363C8E,
    0x0005005B, (u32)btlevtcmd_SetRotate, 0xFFFFFFFE, 0x00000000, 0x00000000, 0x00000000,
    0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress, 0xFFFFFFFE, 0xFE363C80, 0x0006005B, (u32)btlevtcmd_AudienceDeclareAcrobatResult,
    0xFE363C80, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0003005B,
    (u32)evt_snd_sfxon, (u32)(char*)str_SFX_BTL_ACROBAT_MISS_802ef114, 0x00000000, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE,
    0x00000001, (u32)(char*)str_M_I_S_80422340, 0x0001000A, 0x000005DC, 0x0000002A, 0x0002005B,
    (u32)btlevtcmd_WaitEventEnd, 0xFE363C8E, 0x0005005B, (u32)btlevtcmd_SetRotate, 0xFFFFFFFE, 0x00000000,
    0x00000000, 0x00000000, 0x0001005B, (u32)evt_audience_acrobat_notry, 0x00000031, 0x0001005B,
    (u32)evt_audience_ap_recovery, 0x0001005B, (u32)btlevtcmd_InviteApInfoReport, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000,
    0x00000000, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001, 0x00010004,
    0x00000063, 0x00010003, 0x0000005A, 0x0001005B, (u32)btlevtcmd_StopAC, 0x0003005B,
    (u32)btlevtcmd_CommandGetWeaponAddress, 0xFFFFFFFE, 0xFE363C80, 0x0002005B, (u32)btlevtcmd_WeaponAftereffect, 0xFE363C80,
    0x0001005B, (u32)evt_audience_ap_recovery, 0x0001005B, (u32)btlevtcmd_InviteApInfoReport, 0x0004005B, (u32)btlevtcmd_AnimeChangePose,
    0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_J_1B_802eef00, 0x0003005B, (u32)btlevtcmd_SetFallAccel, 0xFFFFFFFE,
    0xF24A7D4C, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xF1194D80, 0xFE363C81,
    0xF1194D80, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE, 0xFE363C80, 0x0000001B,
    0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00000014, 0xFFFFFFFF, 0x0003005B, (u32)btlevtcmd_SetFallAccel, 0xFFFFFFFE, 0xF24A7CE6,
    0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE, 0xFE363C80, 0x00000014, 0x0007005B,
    (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0000000C,
    0xFFFFFFFF, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE, 0xFE363C80, 0x00000008,
    0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00000008, 0xFFFFFFFF, 0x00010003, 0x0000005F, 0x0003005B, (u32)evt_btl_camera_set_mode,
    0x00000000, 0x00000000, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001,
    0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_W_1_80422320, 0x0003005B,
    (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE, 0xF24A9280, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE,
    0x00000001, 0x00000028, 0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000000, 0xFFFFFFFF, 0x00000000, 0x00010003,
    0x00000063, 0x0002005B, (u32)btlevtcmd_ResetFaceDirection, 0xFFFFFFFE, 0x0001005E, (u32)btldefaultevt_SuitoruBadgeEffect,
    0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002, 0x00000001,
};

/* .data + 0x000084F0 */
u32 marioAttackEvent_RenzokuJump[] = {
    0x0005005B, (u32)btlevtcmd_JumpSetting, 0xFFFFFFFE, 0x00000014, 0xF24A7A80, 0xF24A7D4C,
    0x0003005B, (u32)btlevtcmd_GetSelectEnemy, 0xFE363C83, 0xFE363C84, 0x00020018, 0xFE363C83,
    0xFFFFFFFF, 0x00010004, 0x00000063, 0x00000021, 0x0004005B, (u32)btlevtcmd_AttackDeclare,
    0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0x0001005B, (u32)btlevtcmd_WaitGuardMove, 0x0002005B,
    (u32)btlevtcmd_CommandPayWeaponCost, 0xFFFFFFFE, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFFFFFFFE, 0x00000007,
    0x0001005E, (u32)marioAttackEvent_MajinaiPowerUpCheck, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x0000000B,
    0x0006005B, (u32)evt_btl_camera_set_homing_unitparts, 0x00000000, 0xFFFFFFFE, 0x00000001, 0xFE363C83,
    0xFE363C84, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001, 0x0003005B,
    (u32)evt_btl_camera_set_zoom, 0x00000000, 0x000000FA, 0x0002005B, (u32)btlevtcmd_CommandGetWeaponActionLv, 0xFE363C80,
    0x0003005B, (u32)btlevtcmd_AcSetDifficulty, 0xFFFFFFFE, 0xFE363C80, 0x0005005B, (u32)btlevtcmd_SetupAC,
    0xFFFFFFFE, 0x00000001, 0x00000001, 0x00000000, 0x0003005B, (u32)btlevtcmd_ac_timing_flag_onoff,
    0x00000001, 0x00000002, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001,
    0x0000002A, 0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE, 0xF24A8680, 0x0007005B,
    (u32)btlevtcmd_GetTakeoffPosition, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0007005B, (u32)btlevtcmd_CalculateFaceDirection, 0xFFFFFFFE, 0xFFFFFFFF, 0xFE363C80,
    0xFE363C82, 0x00000001, 0xFE363C8F, 0x0003005B, (u32)btlevtcmd_ChangeFaceDirection, 0xFFFFFFFE,
    0xFE363C8F, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000000, 0xFFFFFFFF, 0x00000000, 0x0007005B, (u32)btlevtcmd_CalculateFaceDirection,
    0xFFFFFFFE, 0xFFFFFFFF, 0xFE363C83, 0xFE363C84, 0x00000010, 0xFE363C8F,
    0x0003005B, (u32)btlevtcmd_ChangeFaceDirection, 0xFFFFFFFE, 0xFE363C8F, 0x0002005B, (u32)btlevtcmd_StartAC,
    0x00000001, 0x00020032, 0xFE363C8A, 0x00000000, 0x00010003, 0x0000000A,
    0x0002001D, 0xFE363C8A, 0x00000001, 0x0006005B, (u32)btlevtcmd_CommandPreCheckDamage, 0xFFFFFFFE,
    0xFE363C83, 0xFE363C84, 0x00100100, 0xFE363C85, 0x00000020, 0x0006005B,
    (u32)btlevtcmd_CommandPreCheckDamage, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0x00000100, 0xFE363C85,
    0x00000021, 0x00010022, 0xFE363C85, 0x0001002B, 0x00000002, 0x0001002B,
    0x00000003, 0x0001002B, 0x00000006, 0x0001002B, 0x00000004, 0x0000002E,
    0x00010024, 0x00000001, 0x0000002A, 0x0001000A, 0x000003E8, 0x00000031,
    0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_J_1B_802eef00, 0x0006005B,
    (u32)btlevtcmd_GetHitPos, 0xFE363C83, 0xFE363C84, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00020035, 0xFE363C82, 0x00000005, 0x0002001D, 0xFE363C8A, 0x00000001,
    0x00020032, 0xFE363C88, 0x00000024, 0x0006005B, (u32)btlevtcmd_MarioJumpParam, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0xFE363C88, 0x00010022, 0xFE363C8A,
    0x00010024, 0x00000001, 0x00020032, 0xFE363C86, (u32)(char*)str_SFX_BTL_JUMP_CONTINU_802ef12c, 0x00010024,
    0x00000002, 0x00020032, 0xFE363C86, (u32)(char*)str_SFX_BTL_JUMP_CONTINU_802ef144, 0x00010024, 0x00000003,
    0x00020032, 0xFE363C86, (u32)(char*)str_SFX_BTL_JUMP_CONTINU_802ef15c, 0x00010024, 0x00000004, 0x00020032,
    0xFE363C86, (u32)(char*)str_SFX_BTL_JUMP_CONTINU_802ef174, 0x00010024, 0x00000005, 0x00020032, 0xFE363C86,
    (u32)(char*)str_SFX_BTL_JUMP_CONTINU_802ef18c, 0x00010024, 0x00000006, 0x00020032, 0xFE363C86, (u32)(char*)str_SFX_BTL_JUMP_CONTINU_802ef1a4,
    0x00010024, 0x00000007, 0x00020032, 0xFE363C86, (u32)(char*)str_SFX_BTL_JUMP_CONTINU_802ef1bc, 0x00010024,
    0x00000008, 0x00020032, 0xFE363C86, (u32)(char*)str_SFX_BTL_JUMP_CONTINU_802ef1d4, 0x0000002A, 0x00020032,
    0xFE363C86, (u32)(char*)str_SFX_BTL_JUMP_CONTINU_802ef1ec, 0x00000031, 0x0009005B, (u32)btlevtcmd_SetMoveSoundDataWork, 0xFFFFFFFE,
    0xFE363C86, 0x00000000, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000,
    0x00000000, 0x0000006E, 0x00010009, 0x00000003, 0x0006005B, (u32)btlevtcmd_ACRStart,
    0xFFFFFFFE, 0x0000000A, 0x00000011, 0x0000001E, 0x00000000, 0x0003005B,
    (u32)btlevtcmd_ACRGetResult, 0xFE363C86, 0xFE363C87, 0x00010022, 0xFE363C86, 0x00010029,
    0x00000002, 0x0003005B, (u32)evt_sub_random, 0x00000002, 0xFE363C80, 0x00010022,
    0xFE363C80, 0x00010024, 0x00000000, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE,
    0x00000001, (u32)(char*)str_M_A_4_80422360, 0x00010024, 0x00000001, 0x0004005B, (u32)btlevtcmd_AnimeChangePose,
    0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_A_5_80422310, 0x0000002A, 0x0004005B, (u32)btlevtcmd_AnimeChangePose,
    0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_A_6_80422368, 0x00000031, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress,
    0xFFFFFFFE, 0xFE363C80, 0x0006005B, (u32)btlevtcmd_AudienceDeclareAcrobatResult, 0xFE363C80, 0x00000001,
    0xFFFFFFC4, 0xFFFFFFE2, 0x00000000, 0x0003005B, (u32)btlevtcmd_snd_voice, 0xFFFFFFFE,
    0x00000001, 0x0000002A, 0x0001005B, (u32)evt_audience_acrobat_notry, 0x00000031, 0x00000070,
    0x0008005B, (u32)btlevtcmd_MarioJumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0xFE363C88, 0x00000003, 0xFFFFFFFE, 0x00000020, 0x0006005B, (u32)btlevtcmd_snd_se,
    0xFFFFFFFE, (u32)(char*)str_SFX_VOICE_MARIO_BATT_802eef08, 0xF1194D80, 0x00000000, 0xF1194D80, 0x0009005B,
    (u32)btlevtcmd_SetMoveSoundDataWork, 0xFFFFFFFE, (u32)(char*)str_SFX_BTL_JUMP_CONTINU_802ef06c, 0x00000000, 0x00000000, 0xFFFFFFFF,
    0xFFFFFFFF, 0x00000000, 0x00000000, 0x00020032, 0xFE363C88, 0x00000024,
    0x0006005B, (u32)btlevtcmd_MarioJumpParam, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0xFE363C88, 0x0008005B, (u32)btlevtcmd_MarioJumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0xFE363C88, 0x00000000, 0xFFFFFFFE, 0x00000021, 0x00020019,
    0xFE363C85, 0x00000001, 0x00020018, 0xFE363C85, 0x00000003, 0x0003005B,
    (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000026, 0x00000021, 0x00020018, 0xFE363C85,
    0x00000006, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000027, 0x00000021,
    0x00020018, 0xFE363C85, 0x00000002, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83,
    0x00000028, 0x00000021, 0x0001005B, (u32)btlevtcmd_StopAC, 0x0002005B, (u32)btlevtcmd_JumpContinue,
    0xFFFFFFFE, 0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_VOICE_MARIO_LAND_802eef28, 0xF1194D80,
    0x00000000, 0xF1194D80, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001,
    (u32)(char*)str_M_D_2_80422318, 0x00010009, 0x00000028, 0x00010004, 0x0000005F, 0x00000021,
    0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_J_1D_802eef48, 0x0003005B,
    (u32)btlevtcmd_ac_timing_flag_onoff, 0x00000001, 0x00000001, 0x0002005B, (u32)btlevtcmd_ac_timing_get_success_frame, 0xFE363C80,
    0x00020038, 0xFE363C80, 0x00000002, 0x00010009, 0xFE363C80, 0x0003005B,
    (u32)btlevtcmd_ac_timing_flag_onoff, 0x00000000, 0x00000001, 0x0001005B, (u32)btlevtcmd_ResultAC, 0x0002005B,
    (u32)btlevtcmd_GetResultAC, 0xFE363C86, 0x0002001F, 0xFE363C86, 0x00000002, 0x0006005B,
    (u32)btlevtcmd_CommandCheckDamage, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0x00000100, 0xFE363C85,
    0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress, 0xFFFFFFFE, 0xFE363C80, 0x00020018, 0xFE363C8A,
    0x00000000, 0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult, 0xFE363C80, 0xFFFFFFFF, 0x00000020,
    0x00020032, 0xFE363C86, 0xFE363C8A, 0x00020036, 0xFE363C86, 0x00000001,
    0x0004005B, (u32)btlevtcmd_GetResultPrizeLv, 0xFE363C83, 0xFE363C86, 0xFE363C86, 0x0003005B,
    (u32)btlevtcmd_AudienceDeclareACResult, 0xFE363C80, 0xFE363C86, 0x00000021, 0x0001005B, (u32)btlevtcmd_StopAC,
    0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress,
    0xFFFFFFFE, 0xFE363C80, 0x0002005B, (u32)btlevtcmd_WeaponAftereffect, 0xFE363C80, 0x0001005B,
    (u32)evt_audience_ap_recovery, 0x0001005B, (u32)btlevtcmd_InviteApInfoReport, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE,
    0x00000001, (u32)(char*)str_M_J_1B_802eef00, 0x0003005B, (u32)btlevtcmd_SetFallAccel, 0xFFFFFFFE, 0xF24A7D4C,
    0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xF1194D80, 0xFE363C81, 0xF1194D80,
    0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE, 0xFE363C80, 0x0000001B, 0x0007005B,
    (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000014,
    0xFFFFFFFF, 0x0003005B, (u32)btlevtcmd_SetFallAccel, 0xFFFFFFFE, 0xF24A7CE6, 0x0004005B,
    (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE, 0xFE363C80, 0x00000014, 0x0007005B, (u32)btlevtcmd_JumpPosition,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0000000C, 0xFFFFFFFF,
    0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE, 0xFE363C80, 0x00000008, 0x0007005B,
    (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000008,
    0xFFFFFFFF, 0x00010004, 0x0000005F, 0x00000021, 0x0000006B, 0x0003005B,
    (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000000, 0x0003005B, (u32)evt_btl_camera_set_zoom, 0x00000000,
    0x000000C8, 0x00010009, 0x0000000A, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000,
    0x00000002, 0x0003005B, (u32)evt_btl_camera_set_zoom, 0x00000000, 0x000000FA, 0x0000006D,
    0x0003005B, (u32)btlevtcmd_snd_voice, 0xFFFFFFFE, 0x00000002, 0x0002005B, (u32)btlevtcmd_GetResultCountAC,
    0xFE363C86, 0x0002005B, (u32)_record_renzoku_count, 0xFE363C86, 0x0002005B, (u32)btlevtcmd_GetResultCountAC,
    0xFE363C86, 0x0004005B, (u32)mario_get_renzoku_count_max, 0xFE363C83, 0xFE363C86, 0xFE363C87,
    0x0002001A, 0xFE363C86, 0xFE363C87, 0x0006005B, (u32)btlevtcmd_CommandCheckDamage, 0xFFFFFFFE,
    0xFE363C83, 0xFE363C84, 0x00020000, 0xFE363C85, 0x00000020, 0x0006005B,
    (u32)btlevtcmd_CommandCheckDamage, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0x00020100, 0xFE363C85,
    0x00000021, 0x00020035, 0xFE363C8A, 0x00000001, 0x00020032, 0xFE363C81,
    0xF5DE01A0, 0x00020032, 0xFE363C82, 0xF5DE01A3, 0x00020037, 0xFE363C82,
    0x00000100, 0x00020035, 0xFE363C81, 0xFE363C82, 0x0002001B, 0xFE363C8A,
    0xFE363C81, 0x00020032, 0xFE363C80, 0xFE363C8A, 0x0002001B, 0xFE363C80,
    0x0000270F, 0x00020032, 0xFE363C80, 0x0000270F, 0x00000021, 0x00020032,
    0xFE363C81, 0xFE363C80, 0x00020039, 0xFE363C81, 0x00000100, 0x00020032,
    0xFE363C82, 0xFE363C80, 0x00020038, 0xFE363C82, 0x00000100, 0x00020032,
    0xF5DE01A0, 0xFE363C81, 0x00020032, 0xF5DE01A3, 0xFE363C82, 0x00000021,
    0x00020032, 0xFE363C80, 0xFE363C8A, 0x00020036, 0xFE363C80, 0x00000001,
    0x0004005B, (u32)btlevtcmd_GetResultPrizeLv, 0xFE363C83, 0xFE363C80, 0xFE363C86, 0x0006005B,
    (u32)btlevtcmd_GetHitPos, 0xFE363C83, 0xFE363C84, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0005005B, (u32)btlevtcmd_ACSuccessEffect, 0xFE363C86, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress, 0xFFFFFFFE, 0xFE363C80, 0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult,
    0xFE363C80, 0xFE363C86, 0x0002005B, (u32)btlevtcmd_GetResultCountAC, 0xFE363C86, 0x0002001A,
    0xFE363C86, 0xFE363C87, 0x00010009, 0x00000002, 0x0002005B, (u32)btlevtcmd_StartAC,
    0x00000000, 0x00010004, 0x0000000A, 0x00000021, 0x00010003, 0x0000005A,
    0x0001005B, (u32)btlevtcmd_StopAC, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000,
    0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress, 0xFFFFFFFE, 0xFE363C80, 0x0002005B, (u32)btlevtcmd_WeaponAftereffect,
    0xFE363C80, 0x0001005B, (u32)evt_audience_ap_recovery, 0x0001005B, (u32)btlevtcmd_InviteApInfoReport, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_J_1B_802eef00, 0x0003005B, (u32)btlevtcmd_SetFallAccel,
    0xFFFFFFFE, 0xF24A7D4C, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xF1194D80,
    0xFE363C81, 0xF1194D80, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE, 0xFE363C80,
    0x0000003C, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0000001E, 0xFFFFFFFF, 0x0003005B, (u32)btlevtcmd_SetFallAccel, 0xFFFFFFFE,
    0xF24A7CE6, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE, 0xFE363C80, 0x00000014,
    0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0000000C, 0xFFFFFFFF, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE, 0xFE363C80,
    0x00000008, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000008, 0xFFFFFFFF, 0x00010003, 0x0000005F, 0x0003005B,
    (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000,
    0x00000001, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_W_1_80422320,
    0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE, 0xF24A9280, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType,
    0xFFFFFFFE, 0x00000001, 0x00000028, 0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000, 0xFFFFFFFF, 0x00000000,
    0x00010003, 0x00000063, 0x0002005B, (u32)btlevtcmd_ResetFaceDirection, 0xFFFFFFFE, 0x0001005E,
    (u32)btldefaultevt_SuitoruBadgeEffect, 0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002, 0x00000001,
};

/* .data + 0x00009210 */
u32 marioAttackEvent_TugiTugiJump[] = {
    0x0005005B, (u32)btlevtcmd_JumpSetting, 0xFFFFFFFE, 0x00000014, 0xF24A7A80, 0xF24A7D4C,
    0x0002005B, (u32)btlevtcmd_AttackDeclareAll, 0xFFFFFFFE, 0x0001005B, (u32)btlevtcmd_WaitGuardMove, 0x0003005B,
    (u32)btlevtcmd_GetSelectEnemy, 0xFE363C83, 0xFE363C84, 0x0002005B, (u32)btlevtcmd_CommandPayWeaponCost, 0xFFFFFFFE,
    0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFFFFFFFE, 0x00000007, 0x0001005E, (u32)marioAttackEvent_MajinaiPowerUpCheck,
    0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x0000000B, 0x0006005B, (u32)evt_btl_camera_set_homing_unitparts,
    0x00000000, 0xFFFFFFFE, 0x00000001, 0xFE363C83, 0xFE363C84, 0x0003005B,
    (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001, 0x0003005B, (u32)evt_btl_camera_set_zoom, 0x00000000,
    0x000000FA, 0x0002005B, (u32)btlevtcmd_CommandGetWeaponActionLv, 0xFE363C80, 0x0003005B, (u32)btlevtcmd_AcSetDifficulty,
    0xFFFFFFFE, 0xFE363C80, 0x0005005B, (u32)btlevtcmd_SetupAC, 0xFFFFFFFE, 0x00000001,
    0x00000001, 0x00000000, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001,
    0x0000002A, 0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE, 0xF24A8680, 0x0007005B,
    (u32)btlevtcmd_GetTakeoffPosition, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000000, 0xFFFFFFFF, 0x00000000, 0x0002005B, (u32)btlevtcmd_StartAC,
    0x00000001, 0x00020032, 0xFE363C8A, 0x00000000, 0x00010003, 0x0000000A,
    0x0006005B, (u32)btlevtcmd_CommandPreCheckDamage, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0x00000100,
    0xFE363C85, 0x00010022, 0xFE363C85, 0x0001002B, 0x00000002, 0x0001002B,
    0x00000003, 0x0001002B, 0x00000006, 0x0001002B, 0x00000004, 0x0000002E,
    0x00010024, 0x00000001, 0x0000002A, 0x0001000A, 0x000003E8, 0x00000031,
    0x0009005B, (u32)btlevtcmd_SetMoveSoundDataWork, 0xFFFFFFFE, (u32)(char*)str_SFX_BTL_JUMP_TUGITUG_802ef204, 0x00000000, 0x00000000,
    0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x00000000, 0x0004005B, (u32)btlevtcmd_AnimeChangePose,
    0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_J_1B_802eef00, 0x0006005B, (u32)btlevtcmd_GetHitPos, 0xFE363C83,
    0xFE363C84, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00020035, 0xFE363C82,
    0x00000005, 0x00010022, 0xFE363C8A, 0x00010024, 0x00000000, 0x0006005B,
    (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_VOICE_MARIO_BATT_802eef08, 0xF1194D80, 0x00000000, 0xF1194D80,
    0x00020032, 0xFE363C88, 0x00000024, 0x0006005B, (u32)btlevtcmd_MarioJumpParam, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0xFE363C88, 0x0008005B, (u32)btlevtcmd_MarioJumpPosition,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0xFE363C88, 0x00000000,
    0xFFFFFFFE, 0x00010024, 0x00000001, 0x00020032, 0xFE363C88, 0x00000024,
    0x0006005B, (u32)btlevtcmd_MarioJumpParam, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0xFE363C88, 0x0000006E, 0x00010009, 0x00000003, 0x0006005B, (u32)btlevtcmd_ACRStart,
    0xFFFFFFFE, 0x0000000A, 0x00000013, 0x0000001E, 0x00000000, 0x0003005B,
    (u32)btlevtcmd_ACRGetResult, 0xFE363C86, 0xFE363C87, 0x00010022, 0xFE363C86, 0x00010029,
    0x00000002, 0x0003005B, (u32)evt_sub_random, 0x00000002, 0xFE363C80, 0x00010022,
    0xFE363C80, 0x00010024, 0x00000000, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE,
    0x00000001, (u32)(char*)str_M_A_4_80422360, 0x00010024, 0x00000001, 0x0004005B, (u32)btlevtcmd_AnimeChangePose,
    0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_A_5_80422310, 0x0000002A, 0x0004005B, (u32)btlevtcmd_AnimeChangePose,
    0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_A_6_80422368, 0x00000031, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress,
    0xFFFFFFFE, 0xFE363C80, 0x0006005B, (u32)btlevtcmd_AudienceDeclareAcrobatResult, 0xFE363C80, 0x00000001,
    0xFFFFFFC4, 0xFFFFFFE2, 0x00000000, 0x0000002A, 0x0001005B, (u32)evt_audience_acrobat_notry,
    0x00000031, 0x00000070, 0x0008005B, (u32)btlevtcmd_MarioJumpPosition, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0xFE363C88, 0x00000003, 0xFFFFFFFE, 0x0000002A,
    0x00020032, 0xFE363C88, 0x00000024, 0x0006005B, (u32)btlevtcmd_MarioJumpParam, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0xFE363C88, 0x0000006E, 0x00010009,
    0x00000003, 0x0006005B, (u32)btlevtcmd_ACRStart, 0xFFFFFFFE, 0x0000000A, 0x00000013,
    0x0000001E, 0x00000000, 0x0003005B, (u32)btlevtcmd_ACRGetResult, 0xFE363C86, 0xFE363C87,
    0x00010022, 0xFE363C86, 0x00010029, 0x00000002, 0x0003005B, (u32)evt_sub_random,
    0x00000002, 0xFE363C80, 0x00010022, 0xFE363C80, 0x00010024, 0x00000000,
    0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_A_4_80422360, 0x00010024,
    0x00000001, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_A_5_80422310,
    0x0000002A, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_A_6_80422368,
    0x00000031, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress, 0xFFFFFFFE, 0xFE363C80, 0x0006005B,
    (u32)btlevtcmd_AudienceDeclareAcrobatResult, 0xFE363C80, 0x00000001, 0xFFFFFFC4, 0xFFFFFFE2, 0x00000000,
    0x0000002A, 0x0001005B, (u32)evt_audience_acrobat_notry, 0x00000031, 0x00000070, 0x0008005B,
    (u32)btlevtcmd_MarioJumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0xFE363C88,
    0x00000004, 0xFFFFFFFE, 0x00000031, 0x00020019, 0xFE363C85, 0x00000001,
    0x00020018, 0xFE363C85, 0x00000003, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83,
    0x00000026, 0x00000021, 0x00020018, 0xFE363C85, 0x00000006, 0x0003005B,
    (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000027, 0x00000021, 0x00020018, 0xFE363C85,
    0x00000002, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000028, 0x00000021,
    0x0001005B, (u32)btlevtcmd_StopAC, 0x0002005B, (u32)btlevtcmd_JumpContinue, 0xFFFFFFFE, 0x0006005B,
    (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_VOICE_MARIO_LAND_802eef28, 0xF1194D80, 0x00000000, 0xF1194D80,
    0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_D_2_80422318, 0x00010009,
    0x00000028, 0x00010004, 0x0000005F, 0x00000021, 0x0004005B, (u32)btlevtcmd_AnimeChangePose,
    0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_J_1D_802eef48, 0x0003005B, (u32)btlevtcmd_ac_timing_flag_onoff, 0x00000001,
    0x00000001, 0x0002005B, (u32)btlevtcmd_ac_timing_get_success_frame, 0xFE363C80, 0x00020038, 0xFE363C80,
    0x00000002, 0x00010009, 0xFE363C80, 0x0003005B, (u32)btlevtcmd_ac_timing_flag_onoff, 0x00000000,
    0x00000001, 0x0001005B, (u32)btlevtcmd_ResultAC, 0x0002005B, (u32)btlevtcmd_GetResultAC, 0xFE363C86,
    0x0002001F, 0xFE363C86, 0x00000002, 0x0006005B, (u32)btlevtcmd_CommandCheckDamage, 0xFFFFFFFE,
    0xFE363C83, 0xFE363C84, 0x00000100, 0xFE363C85, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress,
    0xFFFFFFFE, 0xFE363C80, 0x00020018, 0xFE363C8A, 0x00000000, 0x0003005B,
    (u32)btlevtcmd_AudienceDeclareACResult, 0xFE363C80, 0xFFFFFFFF, 0x00000020, 0x00020032, 0xFE363C86,
    0xFE363C8A, 0x00020036, 0xFE363C86, 0x00000001, 0x0004005B, (u32)btlevtcmd_GetResultPrizeLv,
    0xFE363C83, 0xFE363C86, 0xFE363C86, 0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult, 0xFE363C80,
    0xFE363C86, 0x00000021, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000,
    0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001, 0x0001005B, (u32)btlevtcmd_StopAC,
    0x0001005B, (u32)evt_audience_ap_recovery, 0x0001005B, (u32)btlevtcmd_InviteApInfoReport, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress,
    0xFFFFFFFE, 0xFE363C80, 0x0002005B, (u32)btlevtcmd_WeaponAftereffect, 0xFE363C80, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_J_1B_802eef00, 0x0003005B, (u32)btlevtcmd_SetFallAccel,
    0xFFFFFFFE, 0xF24A7D4C, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xF1194D80,
    0xFE363C81, 0xF1194D80, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE, 0xFE363C80,
    0x0000001B, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000014, 0xFFFFFFFF, 0x0003005B, (u32)btlevtcmd_SetFallAccel, 0xFFFFFFFE,
    0xF24A7CE6, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE, 0xFE363C80, 0x00000014,
    0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0000000C, 0xFFFFFFFF, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE, 0xFE363C80,
    0x00000008, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000008, 0xFFFFFFFF, 0x00010004, 0x0000005F, 0x00000021,
    0x0006005B, (u32)btlevtcmd_CommandCheckDamage, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0x00020100,
    0xFE363C85, 0x0002005B, (u32)btlevtcmd_GetResultAC, 0xFE363C80, 0x0002001E, 0xFE363C80,
    0x00000002, 0x0003005B, (u32)btlevtcmd_snd_voice, 0xFFFFFFFE, 0x00000002, 0x0004005B,
    (u32)btlevtcmd_GetResultPrizeLv, 0xFE363C83, 0xFE363C8A, 0xFE363C86, 0x0006005B, (u32)btlevtcmd_GetHitPos,
    0xFE363C83, 0xFE363C84, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B,
    (u32)btlevtcmd_ACSuccessEffect, 0xFE363C86, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000021,
    0x0003005B, (u32)btlevtcmd_GetSelectNextEnemy, 0xFE363C83, 0xFE363C84, 0x00020019, 0xFE363C83,
    0xFFFFFFFF, 0x0000006B, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000002,
    0x0003005B, (u32)evt_btl_camera_set_zoomSpeedLv, 0x00000000, 0x00000000, 0x0003005B, (u32)evt_btl_camera_set_zoom,
    0x00000000, 0x000000C8, 0x00010009, 0x0000000A, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv,
    0x00000000, 0x00000002, 0x0003005B, (u32)evt_btl_camera_set_zoom, 0x00000000, 0x000000FA,
    0x0000006D, 0x00010009, 0x00000002, 0x00020035, 0xFE363C8A, 0x00000001,
    0x0000006B, 0x00010009, 0x0000000A, 0x0006005B, (u32)evt_btl_camera_set_homing_unitparts, 0x00000000,
    0xFFFFFFFE, 0x00000001, 0xFE363C83, 0xFE363C84, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv,
    0x00000000, 0x00000001, 0x0000006D, 0x00010004, 0x0000000A, 0x00000021,
    0x0004005B, (u32)btlevtcmd_GetResultPrizeLv, 0xFE363C83, 0xFE363C8A, 0xFE363C86, 0x0003005B,
    (u32)btlevtcmd_CommandGetWeaponAddress, 0xFFFFFFFE, 0xFE363C80, 0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult, 0xFE363C80,
    0xFE363C86, 0x00010003, 0x0000005A, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000,
    0x00000000, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001, 0x0001005B,
    (u32)btlevtcmd_StopAC, 0x0001005B, (u32)evt_audience_ap_recovery, 0x0001005B, (u32)btlevtcmd_InviteApInfoReport, 0x0003005B,
    (u32)btlevtcmd_CommandGetWeaponAddress, 0xFFFFFFFE, 0xFE363C80, 0x0002005B, (u32)btlevtcmd_WeaponAftereffect, 0xFE363C80,
    0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_J_1B_802eef00, 0x0003005B,
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
    0x00000000, 0x00000001, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001,
    (u32)(char*)str_M_W_1_80422320, 0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE, 0xF24A9280, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001, 0x00000028, 0x0005005B, (u32)btlevtcmd_GetHomePos,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0008005B, (u32)btlevtcmd_MovePosition,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000, 0xFFFFFFFF,
    0x00000000, 0x00010003, 0x00000063, 0x0002005B, (u32)btlevtcmd_ResetFaceDirection, 0xFFFFFFFE,
    0x0001005E, (u32)btldefaultevt_SuitoruBadgeEffect, 0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002,
    0x00000001,
};

/* .data + 0x00009E2C */
u32 marioAttackEvent_NormalHammer_Core[] = {
    0x0003005B, (u32)btlevtcmd_check_battleflag, 0xFE363C80, 0x00000002, 0x00020019, 0xFE363C80,
    0x00000000, 0x00020032, 0xFE363C8C, (u32)marioDefaultWeapon_Hammer, 0x0003005B, (u32)btlevtcmd_GetFirstAttackTarget,
    0xFE363C83, 0xFE363C84, 0x00020018, 0xFE363C83, 0xFFFFFFFF, 0x00010004,
    0x00000063, 0x00000021, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000007,
    0x0004005B, (u32)evt_btl_camera_set_homing_unit, 0x00000000, 0xFE363C83, 0xFFFFFFFF, 0x0003005B,
    (u32)evt_btl_camera_set_zoom, 0x00000000, 0x0000012C, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000,
    0x00000002, 0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE, 0xFE363C80, 0x000000FA,
    0x0005005B, (u32)btlevtcmd_SetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00010009, 0x0000001E, 0x00000020, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress, 0xFFFFFFFE,
    0xFE363C8C, 0x0003005B, (u32)btlevtcmd_GetSelectEnemy, 0xFE363C83, 0xFE363C84, 0x00020018,
    0xFE363C83, 0xFFFFFFFF, 0x00010004, 0x00000063, 0x00000021, 0x0004005B,
    (u32)btlevtcmd_AttackDeclare, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0x0001005B, (u32)btlevtcmd_WaitGuardMove,
    0x0003005B, (u32)btlevtcmd_PayWeaponCost, 0xFFFFFFFE, 0xFE363C8C, 0x0003005B, (u32)btlevtcmd_RunDataEventChild,
    0xFFFFFFFE, 0x00000007, 0x0001005E, (u32)marioAttackEvent_MajinaiPowerUpCheck, 0x0003005B, (u32)evt_btl_camera_set_mode,
    0x00000000, 0x0000000B, 0x0006005B, (u32)evt_btl_camera_set_homing_unitparts, 0x00000000, 0xFFFFFFFE,
    0x00000001, 0xFE363C83, 0xFE363C84, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000,
    0x00000001, 0x0003005B, (u32)evt_btl_camera_set_zoom, 0x00000000, 0x0000012C, 0x00000021,
    0x0003005B, (u32)btlevtcmd_check_battleflag, 0xFE363C80, 0x00000002, 0x00020018, 0xFE363C80,
    0x00000000, 0x0003005B, (u32)btlevtcmd_GetWeaponActionLv, 0xFE363C8C, 0xFE363C80, 0x0003005B,
    (u32)btlevtcmd_AcSetDifficulty, 0xFFFFFFFE, 0xFE363C80, 0x00010022, 0xFE363C8C, 0x00010024,
    (u32)marioDefaultWeapon_Hammer, 0x0009005B, (u32)btlevtcmd_AcSetParamAll, 0x00000004, 0x00000019, 0x00000000,
    0x00000000, 0x00000000, 0xF1194D80, 0xF1194D80, 0xF1194D80, 0x00010024,
    (u32)badgeWeapon_ConfuseHammer, 0x0009005B, (u32)btlevtcmd_AcSetParamAll, 0x00000004, 0x0000001E, 0x00000000,
    0x00000000, 0x00000000, 0xF1194D80, 0xF1194D80, 0xF1194D80, 0x00010024,
    (u32)badgeWeapon_TsuranukiNaguri, 0x0009005B, (u32)btlevtcmd_AcSetParamAll, 0x00000004, 0x0000001E, 0x00000000,
    0x00000000, 0x00000000, 0xF1194D80, 0xF1194D80, 0xF1194D80, 0x0000002A,
    0x0009005B, (u32)btlevtcmd_AcSetParamAll, 0x00000004, 0x00000028, 0x00000000, 0x00000000,
    0x00000000, 0xF1194D80, 0xF1194D80, 0xF1194D80, 0x00000031, 0x0002005B,
    (u32)btlevtcmd_AcSetFlag, 0x00000000, 0x0005005B, (u32)btlevtcmd_SetupAC, 0xFFFFFFFE, 0x00000003,
    0x00000001, 0x00000000, 0x00000021, 0x0003005B, (u32)btlevtcmd_check_battleflag, 0xFE363C80,
    0x00000002, 0x00020019, 0xFE363C80, 0x00000000, 0x0005005B, (u32)btlevtcmd_GetHomePos,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub,
    0xFFFFFFFE, 0xFE363C80, 0x000000FA, 0x0005005B, (u32)btlevtcmd_SetPos, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0001000A, 0x000003E8, 0x0003005B,
    (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE, 0xF24A8E80, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE,
    0x00000001, 0x0000002A, 0x0006005B, (u32)btlevtcmd_GetHitPos, 0xFE363C83, 0xFE363C84,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE,
    0xF1194D80, 0xFE363C81, 0xF1194D80, 0x0004005B, (u32)btlevtcmd_FaceDirectionAdd, 0xFE363C83,
    0xFE363C80, 0x00000023, 0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE, 0xF24A8E80,
    0x00020035, 0xFE363C82, 0x00000005, 0x0007005B, (u32)btlevtcmd_CalculateFaceDirection, 0xFFFFFFFE,
    0xFFFFFFFF, 0xFE363C80, 0xFE363C82, 0x00000001, 0xFE363C8F, 0x0003005B,
    (u32)btlevtcmd_ChangeFaceDirection, 0xFFFFFFFE, 0xFE363C8F, 0x0007005B, (u32)btlevtcmd_GetMoveFrame, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000, 0xFE363C87, 0x0002001B,
    0xFE363C87, 0x00000016, 0x00020036, 0xFE363C87, 0x00000016, 0x00000020,
    0x00020036, 0xFE363C87, 0x00000016, 0x00020037, 0xFE363C87, 0xFFFFFFFF,
    0x00000021, 0x0000006E, 0x00010009, 0xFE363C87, 0x0003005B, (u32)btlevtcmd_GetWeaponActionLv,
    0xFE363C8C, 0xFE363C80, 0x0003005B, (u32)btlevtcmd_AcSetDifficulty, 0xFFFFFFFE, 0xFE363C80,
    0x00010022, 0xFE363C8C, 0x00010024, (u32)marioDefaultWeapon_Hammer, 0x0009005B, (u32)btlevtcmd_AcSetParamAll,
    0x00000004, 0x00000019, 0x00000000, 0x00000000, 0x00000000, 0xF1194D80,
    0xF1194D80, 0xF1194D80, 0x00010024, (u32)badgeWeapon_ConfuseHammer, 0x0009005B, (u32)btlevtcmd_AcSetParamAll,
    0x00000004, 0x0000001E, 0x00000000, 0x00000000, 0x00000000, 0xF1194D80,
    0xF1194D80, 0xF1194D80, 0x00010024, (u32)badgeWeapon_TsuranukiNaguri, 0x0009005B, (u32)btlevtcmd_AcSetParamAll,
    0x00000004, 0x0000001E, 0x00000000, 0x00000000, 0x00000000, 0xF1194D80,
    0xF1194D80, 0xF1194D80, 0x0000002A, 0x0009005B, (u32)btlevtcmd_AcSetParamAll, 0x00000004,
    0x00000028, 0x00000000, 0x00000000, 0x00000000, 0xF1194D80, 0xF1194D80,
    0xF1194D80, 0x00000031, 0x0002005B, (u32)btlevtcmd_AcSetFlag, 0x00000000, 0x0005005B,
    (u32)btlevtcmd_SetupAC, 0xFFFFFFFE, 0x00000003, 0x00000001, 0x00000000, 0x00000070,
    0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00000000, 0xFFFFFFFF, 0x00000000, 0x00000020, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType,
    0xFFFFFFFE, 0x00000001, 0x0000002A, 0x0006005B, (u32)btlevtcmd_GetHitPos, 0xFE363C83,
    0xFE363C84, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)btlevtcmd_GetPos,
    0xFFFFFFFE, 0xF1194D80, 0xFE363C81, 0xF1194D80, 0x0004005B, (u32)btlevtcmd_FaceDirectionAdd,
    0xFE363C83, 0xFE363C80, 0x00000023, 0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE,
    0xF24A8680, 0x00020035, 0xFE363C82, 0x00000005, 0x0007005B, (u32)btlevtcmd_CalculateFaceDirection,
    0xFFFFFFFE, 0xFFFFFFFF, 0xFE363C80, 0xFE363C82, 0x00000001, 0xFE363C8F,
    0x0003005B, (u32)btlevtcmd_ChangeFaceDirection, 0xFFFFFFFE, 0xFE363C8F, 0x0008005B, (u32)btlevtcmd_MovePosition,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000, 0xFFFFFFFF,
    0x00000000, 0x00000021, 0x0007005B, (u32)btlevtcmd_CalculateFaceDirection, 0xFFFFFFFE, 0xFFFFFFFF,
    0xFE363C83, 0xFE363C84, 0x00000010, 0xFE363C8F, 0x0003005B, (u32)btlevtcmd_ChangeFaceDirection,
    0xFFFFFFFE, 0xFE363C8F, 0x0007005B, (u32)btlevtcmd_PreCheckDamage, 0xFFFFFFFE, 0xFE363C83,
    0xFE363C84, 0xFE363C8C, 0x00000100, 0xFE363C86, 0x00020018, 0xFE363C86,
    0x00000005, 0x0001005B, (u32)btlevtcmd_StopAC, 0x0007005B, (u32)btlevtcmd_CheckDamage, 0xFFFFFFFE,
    0xFE363C83, 0xFE363C84, 0xFE363C8C, 0x00000100, 0xFE363C85, 0x00000002,
    0x00000021, 0x00020018, 0xFE363C8C, (u32)badgeWeapon_TsuranukiNaguri, 0x0002005B, (u32)_get_mario_hammer_lv,
    0xFE363C80, 0x00010022, 0xFE363C80, 0x00010024, 0x00000002, 0x00020032,
    0xFE363C80, (u32)(char*)str_M_H_5A_802ef21c, 0x00020032, 0xFE363C81, (u32)(char*)str_M_H_5B_802ef224, 0x00020032,
    0xFE363C82, (u32)(char*)str_M_H_14_802ef22c, 0x00010024, 0x00000003, 0x00020032, 0xFE363C80,
    (u32)(char*)str_M_H_9A_802ef234, 0x00020032, 0xFE363C81, (u32)(char*)str_M_H_9B_802ef23c, 0x00020032, 0xFE363C82,
    (u32)(char*)str_M_H_15_802ef244, 0x0000002A, 0x00020032, 0xFE363C80, (u32)(char*)str_M_H_1A_802ef24c, 0x00020032,
    0xFE363C81, (u32)(char*)str_M_H_1B_802ef254, 0x00020032, 0xFE363C82, (u32)(char*)str_M_H_13_802ef25c, 0x00000031,
    0x00000020, 0x0002005B, (u32)_get_mario_hammer_lv, 0xFE363C80, 0x00010022, 0xFE363C80,
    0x00010024, 0x00000002, 0x00020032, 0xFE363C80, (u32)(char*)str_M_H_5A_802ef21c, 0x00020032,
    0xFE363C81, (u32)(char*)str_M_H_5B_802ef224, 0x00020032, 0xFE363C82, (u32)(char*)str_M_H_5C_802ef264, 0x00010024,
    0x00000003, 0x00020032, 0xFE363C80, (u32)(char*)str_M_H_9A_802ef234, 0x00020032, 0xFE363C81,
    (u32)(char*)str_M_H_9B_802ef23c, 0x00020032, 0xFE363C82, (u32)(char*)str_M_H_9C_802ef26c, 0x0000002A, 0x00020032,
    0xFE363C80, (u32)(char*)str_M_H_1A_802ef24c, 0x00020032, 0xFE363C81, (u32)(char*)str_M_H_1B_802ef254, 0x00020032,
    0xFE363C82, (u32)(char*)str_M_H_1C_802ef274, 0x00000031, 0x00000021, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv,
    0x00000000, 0x00000001, 0x0003005B, (u32)evt_btl_camera_set_zoom, 0x00000000, 0x0000015E,
    0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_VOICE_MARIO_POWE_802eefa4, 0xF1194D80, 0x00000000,
    0xF1194D80, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, 0xFE363C80,
    0x0002005B, (u32)_get_mario_hammer_lv, 0xFE363C80, 0x00010022, 0xFE363C80, 0x00010024,
    0x00000002, 0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_MARIO_HAMMER_HOL_802ef27c, 0xF1194D80,
    0x00000000, 0xF1194D80, 0x00010024, 0x00000001, 0x0006005B, (u32)btlevtcmd_snd_se,
    0xFFFFFFFE, (u32)(char*)str_SFX_MARIO_HAMMER_HOL_802ef294, 0xF1194D80, 0x00000000, 0xF1194D80, 0x0000002A,
    0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_MARIO_HAMMER_HOL_802ef2ac, 0xF1194D80, 0x00000000,
    0xF1194D80, 0x00000031, 0x0000006E, 0x00010022, 0xFE363C8C, 0x00010024,
    (u32)badgeWeapon_ConfuseHammer, 0x00020032, 0xFE363C8D, 0x00000000, 0x00010024, (u32)badgeWeapon_TsuranukiNaguri,
    0x00020032, 0xFE363C8D, 0x00000002, 0x00000031, 0x00010022, 0xFE363C8C,
    0x0001002B, (u32)badgeWeapon_ConfuseHammer, 0x0001002B, (u32)badgeWeapon_TsuranukiNaguri, 0x0003005B, (u32)btlevtcmd_ftomsec,
    0x00000009, 0xFE363C80, 0x0001000A, 0xFE363C80, 0x0003005B, (u32)_hammer_star_effect,
    0xFFFFFFFE, 0xFE363C8D, 0x0000002E, 0x00000031, 0x00010022, 0xFE363C8C,
    0x0001002B, (u32)badgeWeapon_ConfuseHammer, 0x0001002B, (u32)badgeWeapon_TsuranukiNaguri, 0x0005005B, (u32)btlevtcmd_GetPos,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0006005B, (u32)btlevtcmd_snd_se,
    0xFFFFFFFE, (u32)(char*)str_SFX_BTL_MARIO_HAMMER_802eeee4, 0xF1194D80, 0x00000000, 0xF1194D80, 0x0000002E,
    0x00000031, 0x00000070, 0x0003005B, (u32)btlevtcmd_ftomsec, 0x0000001E, 0xFE363C80,
    0x0001000A, 0xFE363C80, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001,
    0xFE363C81, 0x0002005B, (u32)btlevtcmd_StartAC, 0x00000000, 0x0001005B, (u32)btlevtcmd_ResultAC,
    0x0002005B, (u32)_get_mario_hammer_lv, 0xFE363C80, 0x00010022, 0xFE363C80, 0x00010024,
    0x00000002, 0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_MARIO_HAMMER_SWI_802ef2c4, 0x00000000, 0x00010024,
    0x00000001, 0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_MARIO_HAMMER_SWI_802ef2dc, 0x00000000, 0x0000002A,
    0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_MARIO_HAMMER_SWI_802ef2f4, 0x00000000, 0x00000031, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, 0xFE363C82, 0x00010009, 0x00000007,
    0x00020018, 0xFE363C8C, (u32)badgeWeapon_TsuranukiNaguri, 0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_MARIO_HAMMER_TUR_802ef30c,
    0x00000000, 0x00000020, 0x0002005B, (u32)_get_mario_hammer_lv, 0xFE363C80, 0x00010022,
    0xFE363C80, 0x00010024, 0x00000002, 0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_MARIO_HAMMER_HIT_802ef328,
    0x00000000, 0x00010024, 0x00000001, 0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_MARIO_HAMMER_HIT_802ef348,
    0x00000000, 0x0000002A, 0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_MARIO_HAMMER_HIT_802ef368, 0x00000000,
    0x00000031, 0x00000021, 0x00020019, 0xFE363C86, 0x00000001, 0x00020018,
    0xFE363C86, 0x00000003, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000026,
    0x00000021, 0x00020018, 0xFE363C86, 0x00000006, 0x0003005B, (u32)btlevtcmd_StartAvoid,
    0xFE363C83, 0x00000027, 0x00000021, 0x00020018, 0xFE363C86, 0x00000002,
    0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000028, 0x00000021, 0x00010004,
    0x0000005A, 0x00000021, 0x0002005B, (u32)btlevtcmd_GetResultAC, 0xFE363C86, 0x0002001F,
    0xFE363C86, 0x00000002, 0x0007005B, (u32)btlevtcmd_CheckDamage, 0xFFFFFFFE, 0xFE363C83,
    0xFE363C84, 0xFE363C8C, 0x00000100, 0xFE363C85, 0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult,
    0xFE363C8C, 0xFFFFFFFF, 0x00010004, 0x0000005A, 0x00000021, 0x0007005B,
    (u32)btlevtcmd_CheckDamage, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0xFE363C8C, 0x00020100,
    0xFE363C85, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000000, 0x0003005B,
    (u32)evt_btl_camera_set_zoom, 0x00000000, 0x000000FA, 0x0004005B, (u32)btlevtcmd_GetResultPrizeLv, 0xFE363C83,
    0x00000000, 0xFE363C86, 0x0006005B, (u32)btlevtcmd_GetHitPos, 0xFE363C83, 0xFE363C84,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)btlevtcmd_ACSuccessEffect, 0xFE363C86,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult, 0xFE363C8C,
    0xFE363C86, 0x00010003, 0x0000005A, 0x0001005B, (u32)btlevtcmd_StopAC, 0x0003005B,
    (u32)btlevtcmd_GetDamage, 0xFE363C83, 0xFE363C80, 0x0002001D, 0xFE363C80, 0x0000000A,
    0x00020032, 0xFE363C80, 0x00000009, 0x00000021, 0x0006005B, (u32)evt_btl_camera_shake_h,
    0x00000000, 0xFE363C80, 0x00000000, 0x0000000A, 0x0000000D, 0x0002005B,
    (u32)btlevtcmd_WeaponAftereffect, 0xFE363C8C, 0x0006005B, (u32)btlevtcmd_ACRStart, 0xFFFFFFFE, 0x00000000,
    0x0000000C, 0x0000003C, 0x00000000, 0x0003005B, (u32)btlevtcmd_ACRGetResult, 0xFE363C8F,
    0xFE363C8D, 0x00020018, 0xFE363C8F, 0x00000001, 0x00010004, 0x0000005B,
    0x00000021, 0x0002001C, 0xFE363C8F, 0xFFFFFFFF, 0x00010004, 0x0000005B,
    0x00000021, 0x00010004, 0x0000005C, 0x00010003, 0x0000005B, 0x0001005B,
    (u32)evt_audience_acrobat_notry, 0x00020032, 0xFE363C80, 0x0000003C, 0x00020036, 0xFE363C80,
    0xFE363C8D, 0x0002001D, 0xFE363C80, 0x00000001, 0x00010009, 0xFE363C80,
    0x00000021, 0x00020018, 0xFE363C8C, (u32)badgeWeapon_TsuranukiNaguri, 0x0004005B, (u32)btlevtcmd_AnimeChangePose,
    0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_H_16_802ef388, 0x00000020, 0x0004005B, (u32)btlevtcmd_AnimeChangePose,
    0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_S_1_80422338, 0x00000021, 0x00010009, 0x0000000A,
    0x00010004, 0x0000005F, 0x00010003, 0x0000005C, 0x00020019, 0xFE363C8F,
    0x00000000, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000, 0x0003005B,
    (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001, 0x0006005B, (u32)btlevtcmd_AudienceDeclareAcrobatResult, 0xFE363C8C,
    0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x0000006E, 0x0003005B,
    (u32)btlevtcmd_snd_voice, 0xFFFFFFFE, 0x00000001, 0x0005005B, (u32)btlevtcmd_SetRotateOffset, 0xFFFFFFFE,
    0x00000000, 0x00000014, 0x00000000, 0x00010005, 0x0000001E, 0x0005005B,
    (u32)btlevtcmd_AddRotate, 0xFFFFFFFE, 0x00000000, 0x00000000, 0x0000000C, 0x00010009,
    0x00000001, 0x00000006, 0x00000070, 0x0000006E, 0x00010009, 0x00000008,
    0x0006005B, (u32)btlevtcmd_ACRStart, 0xFFFFFFFE, 0x0000000C, 0x00000019, 0x00000019,
    0x00000000, 0x00000070, 0x0000006E, 0x00010009, 0x00000004, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_Z_1_80422370, 0x00000070, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_A_3A_802ef390, 0x0003005B, (u32)btlevtcmd_SetFallAccel,
    0xFFFFFFFE, 0xF24A7BB3, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE, 0xFE363C80,
    0x00000023, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0000001E, 0xFFFFFFFF, 0x00010009, 0x00000004, 0x0003005B,
    (u32)btlevtcmd_ACRGetResult, 0xFE363C80, 0xFE363C81, 0x00020018, 0xFE363C8E, 0x00000000,
    0x00020018, 0xFE363C80, 0x00000002, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE,
    0x00000001, (u32)(char*)str_M_A_3B_802ef0e0, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00020035, 0xFE363C81, 0x00000032, 0x000F005B,
    (u32)evt_eff, (u32)(char*)str_BTLEF_802ef0e8, (u32)(char*)str_confetti_802ef0f0, 0x00000003, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0000005A, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x0006005B, (u32)btlevtcmd_AudienceDeclareAcrobatResult, 0xFE363C8C,
    0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x00010009, 0x0000005A,
    0x00010004, 0x0000005F, 0x00000020, 0x0001005B, (u32)evt_audience_acrobat_notry, 0x00010004,
    0x0000005F, 0x00000021, 0x00000020, 0x00010022, 0xFE363C80, 0x00010024,
    0x00000002, 0x0006005B, (u32)btlevtcmd_AudienceDeclareAcrobatResult, 0xFE363C8C, 0x00000001, 0x00000000,
    0x00000000, 0x00000000, 0x00010024, 0x00000001, 0x0001005B, (u32)evt_audience_acrobat_notry,
    0x00010004, 0x0000005F, 0x00000031, 0x00000021, 0x0000006E, 0x0003005B,
    (u32)btlevtcmd_snd_voice, 0xFFFFFFFE, 0x00000001, 0x0005005B, (u32)btlevtcmd_SetRotateOffset, 0xFFFFFFFE,
    0x00000000, 0x00000014, 0x00000000, 0x00010005, 0x00000028, 0x0005005B,
    (u32)btlevtcmd_AddRotate, 0xFFFFFFFE, 0x00000000, 0x00000000, 0x00000012, 0x00010009,
    0x00000001, 0x00000006, 0x00000070, 0x0000006E, 0x00010009, 0x00000012,
    0x0006005B, (u32)btlevtcmd_ACRStart, 0xFFFFFFFE, 0x0000000C, 0x00000019, 0x00000019,
    0x00000000, 0x00000070, 0x0000006E, 0x00010009, 0x00000012, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_Z_1_80422370, 0x00000070, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_A_3A_802ef390, 0x0003005B, (u32)btlevtcmd_SetFallAccel,
    0xFFFFFFFE, 0xF24A7BB3, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE, 0xFE363C80,
    0x00000028, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000028, 0xFFFFFFFF, 0x00010009, 0x00000004, 0x0003005B,
    (u32)btlevtcmd_ACRGetResult, 0xFE363C80, 0xFE363C81, 0x00020019, 0xFE363C80, 0x00000002,
    0x00010004, 0x0000005F, 0x00000021, 0x0006005B, (u32)btlevtcmd_AudienceDeclareAcrobatResult, 0xFE363C8C,
    0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x0003005B, (u32)btlevtcmd_snd_voice,
    0xFFFFFFFE, 0x00000001, 0x0005005B, (u32)btlevtcmd_SetRotateOffset, 0xFFFFFFFE, 0x00000000,
    0x00000014, 0x00000000, 0x0000006E, 0x00010005, 0x0000003C, 0x0005005B,
    (u32)btlevtcmd_AddRotate, 0xFFFFFFFE, 0x00000000, 0x0000000C, 0x0000000C, 0x00010009,
    0x00000001, 0x00000006, 0x0005005B, (u32)btlevtcmd_SetRotate, 0xFFFFFFFE, 0x00000000,
    0x00000000, 0x00000000, 0x00000070, 0x0000006E, 0x00010009, 0x00000023,
    0x0006005B, (u32)btlevtcmd_ACRStart, 0xFFFFFFFE, 0x0000000F, 0x0000001C, 0x0000001C,
    0x00000000, 0x00000070, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001,
    (u32)(char*)str_M_C_3_80422378, 0x0003005B, (u32)btlevtcmd_SetFallAccel, 0xFFFFFFFE, 0xF24A7B4C, 0x0005005B,
    (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B,
    (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE, 0xFE363C80, 0x00000032, 0x0007005B, (u32)btlevtcmd_JumpPosition,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0000003C, 0xFFFFFFFF,
    0x00010009, 0x00000004, 0x0003005B, (u32)btlevtcmd_ACRGetResult, 0xFE363C80, 0xFE363C81,
    0x00020019, 0xFE363C80, 0x00000002, 0x00010004, 0x0000005F, 0x00000021,
    0x0006005B, (u32)btlevtcmd_AudienceDeclareAcrobatResult, 0xFE363C8C, 0x00000001, 0x00000000, 0x00000000,
    0x00000000, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_A_3B_802ef0e0,
    0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00020035, 0xFE363C81, 0x00000032, 0x000F005B, (u32)evt_eff, (u32)(char*)str_BTLEF_802ef0e8,
    (u32)(char*)str_confetti_802ef0f0, 0x00000003, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0000005A,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x0003005B, (u32)btlevtcmd_snd_voice, 0xFFFFFFFE, 0x00000001, 0x0003005B,
    (u32)evt_snd_sfxon, (u32)(char*)str_SFX_BTL_ACROBAT_FINI_802ef0fc, 0x00000000, 0x00010009, 0x0000005A, 0x00000021,
    0x00010003, 0x0000005F, 0x0001005B, (u32)evt_audience_ap_recovery, 0x0001005B, (u32)btlevtcmd_InviteApInfoReport,
    0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv,
    0x00000000, 0x00000001, 0x0005005B, (u32)btlevtcmd_SetRotate, 0xFFFFFFFE, 0x00000000,
    0x00000000, 0x00000000, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001,
    (u32)(char*)str_M_W_1_80422320, 0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE, 0xF24A9280, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001, 0x00000028, 0x0005005B, (u32)btlevtcmd_GetHomePos,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0008005B, (u32)btlevtcmd_MovePosition,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000, 0xFFFFFFFF,
    0x00000000, 0x00010003, 0x00000063, 0x0002005B, (u32)btlevtcmd_ResetFaceDirection, 0xFFFFFFFE,
    0x0001005E, (u32)btldefaultevt_SuitoruBadgeEffect, 0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002,
    0x00000001,
};

/* .data + 0x0000B300 */
u32 marioAttackEvent_KaitenHammer[] = {
    0x0003005B, (u32)btlevtcmd_check_battleflag, 0xFE363C80, 0x00000002, 0x00020019, 0xFE363C80,
    0x00000000, 0x0003005B, (u32)btlevtcmd_GetFirstAttackTarget, 0xFE363C83, 0xFE363C84, 0x00020018,
    0xFE363C83, 0xFFFFFFFF, 0x00010004, 0x00000063, 0x00000021, 0x0003005B,
    (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000007, 0x0004005B, (u32)evt_btl_camera_set_homing_unit, 0x00000000,
    0xFE363C83, 0xFFFFFFFF, 0x0003005B, (u32)evt_btl_camera_set_zoom, 0x00000000, 0x0000012C,
    0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000002, 0x0005005B, (u32)btlevtcmd_GetHomePos,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub,
    0xFFFFFFFE, 0xFE363C80, 0x000000FA, 0x0005005B, (u32)btlevtcmd_SetPos, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00010009, 0x0000001E, 0x00000020,
    0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress, 0xFFFFFFFE, 0xFE363C8C, 0x0003005B, (u32)btlevtcmd_GetSelectEnemy,
    0xFE363C83, 0xFE363C84, 0x00020018, 0xFE363C83, 0xFFFFFFFF, 0x00010004,
    0x00000063, 0x00000021, 0x0004005B, (u32)btlevtcmd_AttackDeclare, 0xFFFFFFFE, 0xFE363C83,
    0xFE363C84, 0x0001005B, (u32)btlevtcmd_WaitGuardMove, 0x0003005B, (u32)btlevtcmd_PayWeaponCost, 0xFFFFFFFE,
    0xFE363C8C, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFFFFFFFE, 0x00000007, 0x0001005E,
    (u32)marioAttackEvent_MajinaiPowerUpCheck, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x0000000B, 0x0006005B,
    (u32)evt_btl_camera_set_homing_unitparts, 0x00000000, 0xFFFFFFFE, 0x00000001, 0xFE363C83, 0xFE363C84,
    0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001, 0x0003005B, (u32)evt_btl_camera_set_zoom,
    0x00000000, 0x0000012C, 0x00000021, 0x0003005B, (u32)btlevtcmd_GetWeaponActionLv, 0xFE363C8C,
    0xFE363C80, 0x0003005B, (u32)btlevtcmd_AcSetDifficulty, 0xFFFFFFFE, 0xFE363C80, 0x00010022,
    0xFE363C8C, 0x0001002B, (u32)marioWeapon_UltraHammer, 0x0001002B, (u32)marioDefaultWeapon_FirstAttackUltraHammer, 0x0009005B,
    (u32)btlevtcmd_AcSetParamAll, 0x0000000A, 0x00000008, 0x00000000, 0x00000000, 0x00000000,
    0xF1194D80, 0xF1194D80, 0xF1194D80, 0x0000002E, 0x0000002A, 0x0009005B,
    (u32)btlevtcmd_AcSetParamAll, 0x00000007, 0x0000000F, 0x00000000, 0x00000000, 0x00000000,
    0xF1194D80, 0xF1194D80, 0xF1194D80, 0x00000031, 0x0002005B, (u32)btlevtcmd_AcSetFlag,
    0x00000001, 0x0005005B, (u32)btlevtcmd_SetupAC, 0xFFFFFFFE, 0x00000003, 0x00000001,
    0x00000000, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001, 0x0000002A,
    0x0006005B, (u32)btlevtcmd_GetHitPos, 0xFE363C83, 0xFE363C84, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xF1194D80, 0xFE363C81,
    0xF1194D80, 0x0004005B, (u32)btlevtcmd_FaceDirectionAdd, 0xFE363C83, 0xFE363C80, 0x0000001E,
    0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE, 0xF24A8680, 0x00020035, 0xFE363C82,
    0x00000005, 0x0007005B, (u32)btlevtcmd_GetMoveFrame, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000000, 0xFE363C87, 0x00020036, 0xFE363C87, 0x00000015,
    0x0002001A, 0xFE363C87, 0x00000000, 0x00020037, 0xFE363C87, 0xFFFFFFFF,
    0x00010009, 0xFE363C87, 0x00000021, 0x0007005B, (u32)btlevtcmd_CalculateFaceDirection, 0xFFFFFFFE,
    0xFFFFFFFF, 0xFE363C80, 0xFE363C82, 0x00000001, 0xFE363C8F, 0x0003005B,
    (u32)btlevtcmd_ChangeFaceDirection, 0xFFFFFFFE, 0xFE363C8F, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000, 0xFFFFFFFF, 0x00000000,
    0x0007005B, (u32)btlevtcmd_CalculateFaceDirection, 0xFFFFFFFE, 0xFFFFFFFF, 0xFE363C83, 0xFE363C84,
    0x00000010, 0xFE363C8F, 0x0003005B, (u32)btlevtcmd_ChangeFaceDirection, 0xFFFFFFFE, 0xFE363C8F,
    0x0003005B, (u32)evt_btl_camera_set_zoom, 0x00000000, 0x0000015E, 0x0007005B, (u32)btlevtcmd_PreCheckDamage,
    0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0xFE363C8C, 0x00000100, 0xFE363C87,
    0x00020018, 0xFE363C87, 0x00000005, 0x0001005B, (u32)btlevtcmd_StopAC, 0x0007005B,
    (u32)btlevtcmd_CheckDamage, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0xFE363C8C, 0x00000100,
    0xFE363C85, 0x00000002, 0x00000021, 0x0002005B, (u32)_get_mario_hammer_lv, 0xFE363C80,
    0x00010022, 0xFE363C80, 0x00010024, 0x00000002, 0x00020032, 0xFE363C80,
    (u32)(char*)str_M_H_6A_802ef398, 0x00020032, 0xFE363C81, (u32)(char*)str_M_H_6B_802ef3a0, 0x00010024, 0x00000003,
    0x00020032, 0xFE363C80, (u32)(char*)str_M_H_10A_802ef3a8, 0x00020032, 0xFE363C81, (u32)(char*)str_M_H_10B_802ef3b0,
    0x0000002A, 0x00020032, 0xFE363C80, (u32)(char*)str_M_H_2A_802ef3b8, 0x00020032, 0xFE363C81,
    (u32)(char*)str_M_H_2B_802ef3c0, 0x00000031, 0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_VOICE_MARIO_POWE_802eefa4,
    0xF1194D80, 0x00000000, 0xF1194D80, 0x0005005B, (u32)btlevtcmd_AnimeChangePaperGroup, 0xFFFFFFFE,
    0x00000001, (u32)(char*)str_p_kaiten_h_802ef3c8, 0x00000000, 0x0004005B, (u32)btlevtcmd_AnimeChangePaperAnime, 0xFFFFFFFE,
    0x00000001, (u32)(char*)str_P_H_1A_802ef3d4, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001,
    0xFE363C80, 0x0003005B, (u32)_paper_light_off, 0xFFFFFFFE, 0x00000001, 0x00010022,
    0xFE363C8C, 0x0001002B, (u32)marioWeapon_KaitenHammer, 0x0001002B, (u32)marioDefaultWeapon_FirstAttackKaitenHammer, 0x0006005B,
    (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_MARIO_HAMMER_KAI_802ef3dc, 0xF1194D80, 0x00000000, 0xFE363C8F,
    0x0000002E, 0x0001002B, (u32)marioWeapon_UltraHammer, 0x0001002B, (u32)marioDefaultWeapon_FirstAttackUltraHammer, 0x0006005B,
    (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_MARIO_HAMMER_ULT_802ef3f8, 0xF1194D80, 0x00000000, 0xFE363C8F,
    0x0000002E, 0x0000002A, 0x00000031, 0x00010009, 0x0000001C, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePaperAnime, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_P_H_1B_802ef410, 0x0002005B, (u32)btlevtcmd_StartAC,
    0x00000001, 0x0001005B, (u32)btlevtcmd_ResultAC, 0x0005005B, (u32)btlevtcmd_SetRotate, 0xFFFFFFFE,
    0x00000000, 0x00000000, 0x00000000, 0x00010022, 0xFE363C8C, 0x0001002B,
    (u32)marioWeapon_KaitenHammer, 0x0001002B, (u32)marioDefaultWeapon_FirstAttackKaitenHammer, 0x0002005B, (u32)evt_snd_sfxoff, 0xFE363C8F,
    0x0001002B, (u32)marioWeapon_UltraHammer, 0x0001002B, (u32)marioDefaultWeapon_FirstAttackUltraHammer, 0x0002005B, (u32)evt_snd_sfxoff,
    0xFE363C8F, 0x0000002E, 0x0000002E, 0x00000031, 0x0004005B, (u32)btlevtcmd_AnimeChangePose,
    0xFFFFFFFE, 0x00000001, 0xFE363C81, 0x0004005B, (u32)btlevtcmd_AnimeChangePaperAnime, 0xFFFFFFFE,
    0x00000001, (u32)(char*)str_P_H_1C_802ef418, 0x00010009, 0x0000000A, 0x0005005B, (u32)btlevtcmd_AnimeChangePaperGroup,
    0xFFFFFFFE, 0x00000001, 0x00000000, 0x00000000, 0x0003005B, (u32)evt_btl_camera_set_mode,
    0x00000000, 0x00000000, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001,
    0x00020019, 0xFE363C87, 0x00000001, 0x00020018, 0xFE363C87, 0x00000003,
    0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000026, 0x00000021, 0x00020018,
    0xFE363C87, 0x00000006, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000027,
    0x00000021, 0x00020018, 0xFE363C87, 0x00000002, 0x0003005B, (u32)btlevtcmd_StartAvoid,
    0xFE363C83, 0x00000028, 0x00000021, 0x00010004, 0x0000005A, 0x00000021,
    0x0002005B, (u32)btlevtcmd_GetResultAC, 0xFE363C86, 0x0002001F, 0xFE363C86, 0x00000002,
    0x0007005B, (u32)btlevtcmd_CheckDamage, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0xFE363C8C,
    0x00000100, 0xFE363C85, 0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult, 0xFE363C8C, 0xFFFFFFFF,
    0x00010004, 0x0000005A, 0x00000021, 0x00010022, 0xFE363C8C, 0x0001002B,
    (u32)marioWeapon_UltraHammer, 0x0001002B, (u32)marioDefaultWeapon_FirstAttackUltraHammer, 0x0007005B, (u32)btlevtcmd_CheckDamage, 0xFFFFFFFE,
    0xFE363C83, 0xFE363C84, 0xFE363C8C, 0x00020100, 0xFE363C85, 0x0000002E,
    0x0000002A, 0x0007005B, (u32)btlevtcmd_CheckDamage, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84,
    0xFE363C8C, 0x00020100, 0xFE363C85, 0x00000031, 0x0004005B, (u32)btlevtcmd_GetResultPrizeLv,
    0xFE363C83, 0x00000000, 0xFE363C86, 0x0006005B, (u32)btlevtcmd_GetHitPos, 0xFE363C83,
    0xFE363C84, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)btlevtcmd_ACSuccessEffect,
    0xFE363C86, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult,
    0xFE363C8C, 0xFE363C86, 0x00010003, 0x0000005A, 0x0001005B, (u32)btlevtcmd_StopAC,
    0x0002005B, (u32)btlevtcmd_WeaponAftereffect, 0xFE363C8C, 0x00010022, 0xFE363C8C, 0x0001002B,
    (u32)marioWeapon_UltraHammer, 0x0001002B, (u32)marioDefaultWeapon_FirstAttackUltraHammer, 0x00020032, 0xFE363C80, 0x00000001,
    0x0000002E, 0x0000002A, 0x00020032, 0xFE363C80, 0x00000000, 0x00000031,
    0x00020018, 0xFE363C80, 0x00000001, 0x0000006B, 0x0003005B, (u32)_whirlwind_effect,
    0xFFFFFFFE, 0x0000003C, 0x0000006D, 0x0003005B, (u32)btlevtcmd_GetFaceDirection, 0xFFFFFFFE,
    0xFE363C80, 0x00020037, 0xFE363C80, 0x00000024, 0x0001006F, 0xFE363C89,
    0x00010005, 0x00000006, 0x00010022, 0xFE363C8C, 0x0001002B, (u32)marioWeapon_UltraHammer,
    0x0001002B, (u32)marioDefaultWeapon_FirstAttackUltraHammer, 0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_MARIO_HAMMER_ULT_802ef420,
    0xF1194D80, 0x00000000, 0xF1194D80, 0x0000002E, 0x00000031, 0x00010005,
    0x0000000A, 0x0005005B, (u32)btlevtcmd_AddRotate, 0xFFFFFFFE, 0x00000000, 0xFE363C80,
    0x00000000, 0x00010009, 0x00000001, 0x00000006, 0x00000006, 0x0005005B,
    (u32)btlevtcmd_SetRotate, 0xFFFFFFFE, 0x00000000, 0x00000000, 0x00000000, 0x00000070,
    0x00020019, 0xFE363C87, 0x00000001, 0x00010009, 0x00000032, 0x0001005F,
    0xFE363C89, 0x0000006E, 0x00020038, 0xFE363C80, 0x00000003, 0x00010005,
    0x0000001E, 0x0005005B, (u32)btlevtcmd_AddRotate, 0xFFFFFFFE, 0x00000000, 0xFE363C80,
    0x00000000, 0x00010009, 0x00000001, 0x00000006, 0x0005005B, (u32)btlevtcmd_SetRotate,
    0xFFFFFFFE, 0x00000000, 0x00000000, 0x00000000, 0x00000070, 0x00010009,
    0x00000008, 0x0005005B, (u32)btlevtcmd_AnimeChangePaperGroup, 0xFFFFFFFE, 0x00000001, 0x00000000,
    0x00000000, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_I_S_80422340,
    0x00010009, 0x00000018, 0x0001000A, 0x000005DC, 0x00000020, 0x000C005B,
    (u32)btlevtcmd_SetPartsBlur, 0xFFFFFFFE, 0x00000001, 0x000000FF, 0x000000FF, 0x000000FF,
    0x00000050, 0x000000FF, 0x000000FF, 0x000000FF, 0x00000000, 0x00000000,
    0x0004005B, (u32)btlevtcmd_OnPartsAttribute, 0xFFFFFFFE, 0x00000001, 0x04000000, 0x0000006B,
    0x00010009, 0x00000032, 0x000C005B, (u32)btlevtcmd_SetPartsBlur, 0xFFFFFFFE, 0x00000001,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00010009, 0x0000000A, 0x0004005B,
    (u32)btlevtcmd_OffPartsAttribute, 0xFFFFFFFE, 0x00000001, 0x04000000, 0x0000006D, 0x0006005B,
    (u32)btlevtcmd_ACRStart, 0xFFFFFFFE, 0x00000028, 0x0000003F, 0x0000003F, 0x00000000,
    0x0003005B, (u32)btlevtcmd_ACRGetResult, 0xFE363C86, 0xF1194D80, 0x00010022, 0xFE363C86,
    0x00010029, 0x00000002, 0x0001005F, 0xFE363C89, 0x0005005B, (u32)btlevtcmd_AnimeChangePaperGroup,
    0xFFFFFFFE, 0x00000001, 0x00000000, 0x00000000, 0x0005005B, (u32)_kaiten_hammer_acrobat_rotate,
    0xFFFFFFFE, 0x00000001, 0x0000001E, (u32)(char*)str_M_V_2_80422380, 0x0006005B, (u32)btlevtcmd_AudienceDeclareAcrobatResult,
    0xFE363C8C, 0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x00010009,
    0x0000001E, 0x0000002A, 0x0002005B, (u32)btlevtcmd_WaitEventEnd, 0xFE363C89, 0x0005005B,
    (u32)btlevtcmd_SetRotate, 0xFFFFFFFE, 0x00000000, 0x00000000, 0x00000000, 0x0005005B,
    (u32)btlevtcmd_AnimeChangePaperGroup, 0xFFFFFFFE, 0x00000001, 0x00000000, 0x00000000, 0x0001005B,
    (u32)evt_audience_acrobat_notry, 0x00000031, 0x00000021, 0x00000020, 0x00020019, 0xFE363C87,
    0x00000001, 0x0003005B, (u32)btlevtcmd_GetFaceDirection, 0xFFFFFFFE, 0xFE363C80, 0x00020037,
    0xFE363C80, 0x00000024, 0x0001006F, 0xFE363C89, 0x00010005, 0x00000006,
    0x00010022, 0xFE363C8C, 0x0001002B, (u32)marioWeapon_KaitenHammer, 0x0001002B, (u32)marioDefaultWeapon_FirstAttackKaitenHammer,
    0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_BTL_MARIO_HAMMER_802ef438, 0xF1194D80, 0x00000000,
    0xF1194D80, 0x0000002E, 0x00000031, 0x00010005, 0x0000000A, 0x0005005B,
    (u32)btlevtcmd_AddRotate, 0xFFFFFFFE, 0x00000000, 0xFE363C80, 0x00000000, 0x00010009,
    0x00000001, 0x00000006, 0x00000006, 0x0005005B, (u32)btlevtcmd_SetRotate, 0xFFFFFFFE,
    0x00000000, 0x00000000, 0x00000000, 0x00000070, 0x00010009, 0x00000032,
    0x0001005F, 0xFE363C89, 0x0000006E, 0x00020038, 0xFE363C80, 0x00000003,
    0x00010005, 0x0000001E, 0x0005005B, (u32)btlevtcmd_AddRotate, 0xFFFFFFFE, 0x00000000,
    0xFE363C80, 0x00000000, 0x00010009, 0x00000001, 0x00000006, 0x0005005B,
    (u32)btlevtcmd_SetRotate, 0xFFFFFFFE, 0x00000000, 0x00000000, 0x00000000, 0x00000070,
    0x00010009, 0x00000008, 0x0005005B, (u32)btlevtcmd_AnimeChangePaperGroup, 0xFFFFFFFE, 0x00000001,
    0x00000000, 0x00000000, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001,
    (u32)(char*)str_M_I_S_80422340, 0x00010009, 0x00000018, 0x0001000A, 0x000005DC, 0x00000020,
    0x0000006B, 0x0003005B, (u32)_whirlwind_effect, 0xFFFFFFFE, 0x0000003C, 0x0000006D,
    0x0001006F, 0xFE363C89, 0x0003005B, (u32)btlevtcmd_GetFaceDirection, 0xFFFFFFFE, 0xFE363C80,
    0x00020037, 0xFE363C80, 0x00000024, 0x00010005, 0x00000006, 0x00010022,
    0xFE363C8C, 0x0001002B, (u32)marioWeapon_KaitenHammer, 0x0001002B, (u32)marioDefaultWeapon_FirstAttackKaitenHammer, 0x0006005B,
    (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_BTL_MARIO_HAMMER_802ef438, 0xF1194D80, 0x00000000, 0xF1194D80,
    0x0000002E, 0x00000031, 0x00010005, 0x0000000A, 0x0005005B, (u32)btlevtcmd_AddRotate,
    0xFFFFFFFE, 0x00000000, 0xFE363C80, 0x00000000, 0x00010009, 0x00000001,
    0x00000006, 0x00000006, 0x0005005B, (u32)btlevtcmd_SetRotate, 0xFFFFFFFE, 0x00000000,
    0x00000000, 0x00000000, 0x00000070, 0x0006005B, (u32)btlevtcmd_ACRStart, 0xFFFFFFFE,
    0x00000028, 0x0000003F, 0x0000003F, 0x00000000, 0x0003005B, (u32)btlevtcmd_ACRGetResult,
    0xFE363C86, 0xF1194D80, 0x00010022, 0xFE363C86, 0x00010029, 0x00000002,
    0x0001005F, 0xFE363C89, 0x0005005B, (u32)btlevtcmd_AnimeChangePaperGroup, 0xFFFFFFFE, 0x00000001,
    0x00000000, 0x00000000, 0x0005005B, (u32)_kaiten_hammer_acrobat_rotate, 0xFFFFFFFE, 0x00000001,
    0x0000001E, (u32)(char*)str_M_V_2_80422380, 0x0006005B, (u32)btlevtcmd_AudienceDeclareAcrobatResult, 0xFE363C8C, 0x00000001,
    0x00000000, 0x00000000, 0x00000000, 0x00010009, 0x0000001E, 0x0000002A,
    0x0002005B, (u32)btlevtcmd_WaitEventEnd, 0xFE363C89, 0x0005005B, (u32)btlevtcmd_SetRotate, 0xFFFFFFFE,
    0x00000000, 0x00000000, 0x00000000, 0x0005005B, (u32)btlevtcmd_AnimeChangePaperGroup, 0xFFFFFFFE,
    0x00000001, 0x00000000, 0x00000000, 0x0001005B, (u32)evt_audience_acrobat_notry, 0x00000031,
    0x00000021, 0x00000021, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001,
    0x00000028, 0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE, 0xF24A9280, 0x0005005B,
    (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0008005B,
    (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000,
    0xFFFFFFFF, 0x00000000, 0x00010022, 0xFE363C8C, 0x0001002B, (u32)marioWeapon_UltraHammer,
    0x0001002B, (u32)marioDefaultWeapon_FirstAttackUltraHammer, 0x0000002E, 0x0000002A, 0x00010004, 0x00000063,
    0x00000031, 0x0002005B, (u32)btlevtcmd_GetResultAC, 0xFE363C86, 0x0002001F, 0xFE363C86,
    0x00000002, 0x00010004, 0x00000063, 0x00000021, 0x00020019, 0xFE363C87,
    0x00000001, 0x00010004, 0x00000063, 0x00000021, 0x0004005B, (u32)btlevtcmd_CheckToken,
    0xFE363C83, 0x00000020, 0xFE363C80, 0x00020018, 0xFE363C80, 0x00000000,
    0x00010004, 0x00000063, 0x00000021, 0x00010003, 0x00000060, 0x0004005B,
    (u32)btlevtcmd_CheckToken, 0xFE363C83, 0x00000040, 0xFE363C80, 0x00020018, 0xFE363C80,
    0x00000000, 0x00010009, 0x00000001, 0x00010004, 0x00000060, 0x00000021,
    0x0003005B, (u32)btlevtcmd_OffToken, 0xFE363C83, 0x00000040, 0x00010003, 0x00000062,
    0x0004005B, (u32)btlevtcmd_CheckToken, 0xFE363C83, 0x00000080, 0xFE363C80, 0x00020018,
    0xFE363C80, 0x00000000, 0x0004005B, (u32)btlevtcmd_CheckToken, 0xFFFFFFFE, 0x00000100,
    0xFE363C80, 0x00020018, 0xFE363C80, 0x00000000, 0x00000021, 0x00010009,
    0x00000001, 0x00010004, 0x00000062, 0x00000021, 0x0007005B, (u32)btlevtcmd_CheckDamage,
    0xFFFFFFFE, 0xFE363C83, 0xFE363C84, (u32)marioWeapon_UltraHammerFinish, 0x00020100, 0xFE363C85,
    0x00010003, 0x00000063, 0x0001005B, (u32)evt_audience_ap_recovery, 0x0001005B, (u32)btlevtcmd_InviteApInfoReport,
    0x0002005B, (u32)btlevtcmd_ResetFaceDirection, 0xFFFFFFFE, 0x0001005E, (u32)btldefaultevt_SuitoruBadgeEffect, 0x0002005B,
    (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002, 0x00000001,
};

/* .data + 0x0000C390 */
u32 marioAttackEvent_FirstAttackKaitenHammer[] = {
    0x0002005B, (u32)_get_mario_hammer_lv, 0xFE363C80, 0x00020019, 0xFE363C80, 0x00000003,
    0x00020032, 0xFE363C8C, (u32)marioDefaultWeapon_FirstAttackKaitenHammer, 0x00000020, 0x00020032, 0xFE363C8C,
    (u32)marioDefaultWeapon_FirstAttackUltraHammer, 0x00000021, 0x0001005E, (u32)marioAttackEvent_KaitenHammer, 0x00000002, 0x00000002,
    0x00000001,
};

/* .data + 0x0000C3DC */
u32 marioAttackEvent_JishinHammer[] = {
    0x0002005B, (u32)btlevtcmd_AttackDeclareAll, 0xFFFFFFFE, 0x0001005B, (u32)btlevtcmd_WaitGuardMove, 0x0002005B,
    (u32)btlevtcmd_CommandPayWeaponCost, 0xFFFFFFFE, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFFFFFFFE, 0x00000007,
    0x0001005E, (u32)marioAttackEvent_MajinaiPowerUpCheck, 0x0002005B, (u32)btlevtcmd_CommandGetWeaponActionLv, 0xFE363C80, 0x0003005B,
    (u32)btlevtcmd_AcSetDifficulty, 0xFFFFFFFE, 0xFE363C80, 0x0009005B, (u32)btlevtcmd_AcSetParamAll, 0x00000004,
    0x00000019, 0x00000000, 0x00000000, 0x00000000, 0xF1194D80, 0xF1194D80,
    0xF1194D80, 0x0002005B, (u32)btlevtcmd_AcSetFlag, 0x00000000, 0x0005005B, (u32)btlevtcmd_SetupAC,
    0xFFFFFFFE, 0x00000003, 0x00000001, 0x00000000, 0x0003005B, (u32)evt_btl_camera_set_mode,
    0x00000000, 0x00000007, 0x0004005B, (u32)evt_btl_camera_set_homing_unit, 0x00000000, 0xFFFFFFFE,
    0xFFFFFFFE, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001, 0x0003005B,
    (u32)evt_btl_camera_set_zoom, 0x00000000, 0x000000C8, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE,
    0xF24A8680, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001, 0x0000002A,
    0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFFFFFFC4, 0xFE363C81, 0xFE363C82,
    0x00000000, 0xFFFFFFFF, 0x00000000, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000,
    0x00000004, 0x0003005B, (u32)evt_btl_camera_set_zoom, 0x00000000, 0x000001C2, 0x0002005B,
    (u32)_get_mario_hammer_lv, 0xFE363C80, 0x00010022, 0xFE363C80, 0x00010024, 0x00000002,
    0x00020032, 0xFE363C80, (u32)(char*)str_M_H_5A_802ef21c, 0x00020032, 0xFE363C81, (u32)(char*)str_M_H_5B_802ef224,
    0x00020032, 0xFE363C82, (u32)(char*)str_M_H_14_802ef22c, 0x00010024, 0x00000003, 0x00020032,
    0xFE363C80, (u32)(char*)str_M_H_9A_802ef234, 0x00020032, 0xFE363C81, (u32)(char*)str_M_H_9B_802ef23c, 0x00020032,
    0xFE363C82, (u32)(char*)str_M_H_15_802ef244, 0x0000002A, 0x00020032, 0xFE363C80, (u32)(char*)str_M_H_1A_802ef24c,
    0x00020032, 0xFE363C81, (u32)(char*)str_M_H_1B_802ef254, 0x00020032, 0xFE363C82, (u32)(char*)str_M_H_13_802ef25c,
    0x00000031, 0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_VOICE_MARIO_POWE_802eefa4, 0xF1194D80,
    0x00000000, 0xF1194D80, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001,
    0xFE363C80, 0x0002005B, (u32)_get_mario_hammer_lv, 0xFE363C80, 0x00010022, 0xFE363C80,
    0x00010024, 0x00000002, 0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_MARIO_HAMMER_HOL_802ef27c,
    0xF1194D80, 0x00000000, 0xF1194D80, 0x00010024, 0x00000001, 0x0006005B,
    (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_MARIO_HAMMER_HOL_802ef294, 0xF1194D80, 0x00000000, 0xF1194D80,
    0x0000002A, 0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_MARIO_HAMMER_HOL_802ef2ac, 0xF1194D80,
    0x00000000, 0xF1194D80, 0x00000031, 0x00010009, 0x0000001E, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, 0xFE363C81, 0x0002005B, (u32)btlevtcmd_StartAC,
    0x00000001, 0x00010009, 0x0000004B, 0x0001006F, 0xFE363C8D, 0x00020032,
    0xFE363C85, 0x00000005, 0x00010005, 0xFE363C85, 0x0003005B, (u32)btlevtcmd_AcGetOutputParam,
    0x00000001, 0xFE363C86, 0x0002001D, 0xFE363C86, 0x00000003, 0x00000007,
    0x00000021, 0x00010009, 0x00000001, 0x00020032, 0xFE363C85, 0x00000005,
    0x00000006, 0x00010009, 0x00000019, 0x00000070, 0x0001006F, 0xFE363C8E,
    0x00020032, 0xFE363C85, 0x00000005, 0x00010005, 0xFE363C85, 0x0003005B,
    (u32)btlevtcmd_AcGetOutputParam, 0x00000001, 0xFE363C86, 0x0002001D, 0xFE363C86, 0x00000003,
    0x00000007, 0x00000021, 0x00010009, 0x00000001, 0x00020032, 0xFE363C85,
    0x00000005, 0x00000006, 0x00010009, 0x00000005, 0x0003005B, (u32)btlevtcmd_GetFaceDirection,
    0xFFFFFFFE, 0xFE363C85, 0x00020037, 0xFE363C85, 0x0000002D, 0x0005005B,
    (u32)evt_sub_intpl_init, 0x0000000B, 0x00000000, 0xFE363C85, 0x0000000A, 0x00010005,
    0x0000000A, 0x0001005B, (u32)evt_sub_intpl_get_value, 0x0005005B, (u32)btlevtcmd_SetRotate, 0xFFFFFFFE,
    0x00000000, 0x00000000, 0xFE363C80, 0x00010009, 0x00000001, 0x00000006,
    0x00010009, 0x00000008, 0x0005005B, (u32)evt_sub_intpl_init, 0x0000000C, 0xFE363C85,
    0x00000000, 0x0000000F, 0x00010005, 0x0000000F, 0x0001005B, (u32)evt_sub_intpl_get_value,
    0x0005005B, (u32)btlevtcmd_SetRotate, 0xFFFFFFFE, 0x00000000, 0x00000000, 0xFE363C80,
    0x00010009, 0x00000001, 0x00000006, 0x00000070, 0x0001006F, 0xFE363C8F,
    0x00020032, 0xFE363C85, 0x00000005, 0x00010005, 0xFE363C85, 0x0003005B,
    (u32)btlevtcmd_AcGetOutputParam, 0x00000001, 0xFE363C86, 0x0002001D, 0xFE363C86, 0x00000003,
    0x00000007, 0x00000021, 0x00010009, 0x00000001, 0x00020032, 0xFE363C85,
    0x00000005, 0x00000006, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C86,
    0xFE363C87, 0xFE363C88, 0x0003005B, (u32)btlevtcmd_SetFallAccel, 0xFFFFFFFE, 0xF24A7BB3,
    0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C86, 0xFE363C87, 0xFE363C88,
    0x00000028, 0xFFFFFFFF, 0x00000070, 0x0001005B, (u32)btlevtcmd_ResultAC, 0x0002005B,
    (u32)btlevtcmd_GetResultAC, 0xFE363C88, 0x0003005B, (u32)btlevtcmd_AcGetOutputParam, 0x00000001, 0xFE363C80,
    0x0002001D, 0xFE363C80, 0x00000003, 0x00010004, 0x0000000A, 0x00000021,
    0x0001005F, 0xFE363C8D, 0x0001005F, 0xFE363C8E, 0x0001005F, 0xFE363C8F,
    0x0002005B, (u32)_get_mario_hammer_lv, 0xFE363C80, 0x00010022, 0xFE363C80, 0x00010024,
    0x00000002, 0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_MARIO_HAMMER_SWI_802ef2c4, 0x00000000, 0x00020032,
    0xFE363C82, (u32)(char*)str_M_H_14_802ef22c, 0x00010024, 0x00000001, 0x0003005B, (u32)evt_snd_sfxon,
    (u32)(char*)str_SFX_MARIO_HAMMER_SWI_802ef2dc, 0x00000000, 0x00020032, 0xFE363C82, (u32)(char*)str_M_H_13_802ef25c, 0x0000002A,
    0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_MARIO_HAMMER_SWI_802ef2f4, 0x00000000, 0x00020032, 0xFE363C82,
    (u32)(char*)str_M_H_15_802ef244, 0x00000031, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001,
    0xFE363C82, 0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_MARIO_ATTACK_ULT_802ef458, 0x00000000, 0x00010009,
    0x0000000A, 0x00010004, 0x00000028, 0x00010003, 0x0000000A, 0x0002005B,
    (u32)btlevtcmd_WaitEventEnd, 0xFE363C8D, 0x0002005B, (u32)_get_mario_hammer_lv, 0xFE363C80, 0x00010022,
    0xFE363C80, 0x00010024, 0x00000002, 0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_MARIO_HAMMER_SWI_802ef2c4,
    0x00000000, 0x00010024, 0x00000001, 0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_MARIO_HAMMER_SWI_802ef2dc,
    0x00000000, 0x0000002A, 0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_MARIO_HAMMER_SWI_802ef2f4, 0x00000000,
    0x00000031, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, 0xFE363C82,
    0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_MARIO_ATTACK_ULT_802ef458, 0x00000000, 0x0002005B, (u32)btlevtcmd_WaitEventEnd,
    0xFE363C8E, 0x0002005B, (u32)btlevtcmd_WaitEventEnd, 0xFE363C8F, 0x00010003, 0x00000028,
    0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000001, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv,
    0x00000000, 0x00000000, 0x0003005B, (u32)evt_btl_camera_set_zoom, 0x00000000, 0x00000000,
    0x0002005B, (u32)evt_btl_camera_off_posoffset_manual, 0x00000000, 0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_MARIO_HAMMER_ULT_802ef478,
    0x00000000, 0x0006005B, (u32)evt_btl_camera_shake_h, 0x00000000, 0x00000008, 0x00000000,
    0x0000002A, 0x0000000D, 0x0002005B, (u32)evt_audience_reflesh_natural_all, 0x00000064, 0x0002005B,
    (u32)btlevtcmd_GetResultAC, 0xFE363C87, 0x0002001E, 0xFE363C87, 0x00000002, 0x0004005B,
    (u32)btlevtcmd_GetResultPrizeLv, 0xFFFFFFFB, 0x00000000, 0xFE363C86, 0x0005005B, (u32)btlevtcmd_ACSuccessEffect,
    0xFE363C86, 0x00000000, 0x00000032, 0x00000000, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress,
    0xFFFFFFFE, 0xFE363C80, 0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult, 0xFE363C80, 0xFE363C86,
    0x00000020, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress, 0xFFFFFFFE, 0xFE363C80, 0x0003005B,
    (u32)btlevtcmd_AudienceDeclareACResult, 0xFE363C80, 0xFFFFFFFF, 0x00000021, 0x0003005B, (u32)btlevtcmd_GetSelectEnemy,
    0xFE363C83, 0xFE363C84, 0x00020018, 0xFE363C83, 0xFFFFFFFF, 0x0001005B,
    (u32)btlevtcmd_StopAC, 0x00010004, 0x0000005A, 0x00000021, 0x00010004, 0x00000033,
    0x00010003, 0x00000032, 0x0003005B, (u32)btlevtcmd_GetSelectNextEnemy, 0xFE363C83, 0xFE363C84,
    0x00020018, 0xFE363C83, 0xFFFFFFFF, 0x0001005B, (u32)btlevtcmd_StopAC, 0x00010004,
    0x0000005A, 0x00000021, 0x00010003, 0x00000033, 0x0006005B, (u32)btlevtcmd_CommandPreCheckDamage,
    0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0x00000100, 0xFE363C86, 0x00020018,
    0xFE363C86, 0x00000001, 0x0007005B, (u32)btlevtcmd_DamageDirect, 0xFE363C83, 0xFE363C84,
    0x00000000, 0x00000000, 0x00000002, 0x00000000, 0x0000006B, 0x00010009,
    0x0000002A, 0x0002001E, 0xFE363C87, 0x00000002, 0x0006005B, (u32)btlevtcmd_CommandCheckDamage,
    0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0x00020100, 0xFE363C85, 0x00000020,
    0x0006005B, (u32)btlevtcmd_CommandCheckDamage, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0x00000100,
    0xFE363C85, 0x00000021, 0x0000006D, 0x00000020, 0x00020018, 0xFE363C86,
    0x00000003, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000026, 0x00000021,
    0x00020018, 0xFE363C86, 0x00000006, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83,
    0x00000027, 0x00000021, 0x00020018, 0xFE363C86, 0x00000002, 0x0003005B,
    (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000028, 0x00000021, 0x00000021, 0x00010004,
    0x00000032, 0x00010003, 0x0000005A, 0x00010009, 0x0000000C, 0x0003005B,
    (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000,
    0x00000001, 0x00010009, 0x0000001E, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress, 0xFFFFFFFE,
    0xFE363C80, 0x0002005B, (u32)btlevtcmd_WeaponAftereffect, 0xFE363C80, 0x00010009, 0x0000003C,
    0x0001006F, 0xFE363C8F, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001,
    (u32)(char*)str_M_H_16_802ef388, 0x00010009, 0x00000012, 0x00000070, 0x0006005B, (u32)btlevtcmd_ACRStart,
    0xFFFFFFFE, 0x00000000, 0x00000011, 0x00000011, 0x00000000, 0x0003005B,
    (u32)btlevtcmd_ACRGetResult, 0xFE363C86, 0xFE363C87, 0x00010022, 0xFE363C86, 0x00010029,
    0x00000002, 0x0000002A, 0x0001005B, (u32)evt_audience_acrobat_notry, 0x0002005B, (u32)btlevtcmd_WaitEventEnd,
    0xFE363C8F, 0x0001005B, (u32)evt_audience_ap_recovery, 0x0001005B, (u32)btlevtcmd_InviteApInfoReport, 0x00010004,
    0x0000005F, 0x00000031, 0x0003005B, (u32)btlevtcmd_AnimeWaitPlayComplete, 0xFFFFFFFE, 0x00000001,
    0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv,
    0x00000000, 0x00000001, 0x0000006E, 0x0003005B, (u32)btlevtcmd_snd_voice, 0xFFFFFFFE,
    0x00000001, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress, 0xFFFFFFFE, 0xFE363C8C, 0x0006005B,
    (u32)btlevtcmd_AudienceDeclareAcrobatResult, 0xFE363C8C, 0x00000001, 0xFFFFFFE2, 0x00000014, 0x00000000,
    0x0005005B, (u32)btlevtcmd_SetRotateOffset, 0xFFFFFFFE, 0x00000000, 0x00000014, 0x00000000,
    0x00010005, 0x0000003C, 0x0005005B, (u32)btlevtcmd_AddRotate, 0xFFFFFFFE, 0x00000000,
    0x00000000, 0x0000000C, 0x00010009, 0x00000001, 0x00000006, 0x00000070,
    0x0000006E, 0x00010009, 0x00000008, 0x0006005B, (u32)btlevtcmd_ACRStart, 0xFFFFFFFE,
    0x0000002A, 0x00000037, 0x00000037, 0x00000000, 0x00000070, 0x0000006E,
    0x00010009, 0x00000004, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001,
    (u32)(char*)str_M_Z_1_80422370, 0x00000070, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001,
    (u32)(char*)str_M_A_3A_802ef390, 0x0003005B, (u32)btlevtcmd_SetFallAccel, 0xFFFFFFFE, 0xF24A7B4C, 0x0005005B,
    (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0007005B,
    (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0000003C,
    0xFFFFFFFF, 0x00010009, 0x00000004, 0x0003005B, (u32)btlevtcmd_ACRGetResult, 0xFE363C80,
    0xFE363C81, 0x00020018, 0xFE363C80, 0x00000002, 0x0004005B, (u32)btlevtcmd_AnimeChangePose,
    0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_A_3B_802ef0e0, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00020035, 0xFE363C81, 0x00000032,
    0x000F005B, (u32)evt_eff, (u32)(char*)str_BTLEF_802ef0e8, (u32)(char*)str_confetti_802ef0f0, 0x00000003, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0000005A, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress,
    0xFFFFFFFE, 0xFE363C8C, 0x0006005B, (u32)btlevtcmd_AudienceDeclareAcrobatResult, 0xFE363C8C, 0x00000001,
    0x00000000, 0x00000000, 0x00000000, 0x0001000A, 0x000005DC, 0x00000021,
    0x0001005B, (u32)evt_audience_ap_recovery, 0x0001005B, (u32)btlevtcmd_InviteApInfoReport, 0x00010003, 0x0000005F,
    0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_W_1_80422320, 0x0003005B,
    (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE, 0xF24A9280, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE,
    0x00000001, 0x00000028, 0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000000, 0xFFFFFFFF, 0x00000000, 0x00010003,
    0x00000063, 0x0002005B, (u32)btlevtcmd_ResetFaceDirection, 0xFFFFFFFE, 0x0001005E, (u32)btldefaultevt_SuitoruBadgeEffect,
    0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002, 0x00000001,
};

/* .data + 0x0000D110 */
u32 marioAttackEvent_UltraJishinHammer[] = {
    0x0002005B, (u32)btlevtcmd_AttackDeclareAll, 0xFFFFFFFE, 0x0001005B, (u32)btlevtcmd_WaitGuardMove, 0x0002005B,
    (u32)btlevtcmd_CommandPayWeaponCost, 0xFFFFFFFE, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFFFFFFFE, 0x00000007,
    0x0001005E, (u32)marioAttackEvent_MajinaiPowerUpCheck, 0x0002005B, (u32)btlevtcmd_CommandGetWeaponActionLv, 0xFE363C80, 0x0003005B,
    (u32)btlevtcmd_AcSetDifficulty, 0xFFFFFFFE, 0xFE363C80, 0x0009005B, (u32)btlevtcmd_AcSetParamAll, 0x00000004,
    0x00000010, 0x00000000, 0x00000000, 0x00000000, 0xF1194D80, 0xF1194D80,
    0xF1194D80, 0x0002005B, (u32)btlevtcmd_AcSetFlag, 0x00000000, 0x0005005B, (u32)btlevtcmd_SetupAC,
    0xFFFFFFFE, 0x00000003, 0x00000001, 0x00000000, 0x0003005B, (u32)evt_btl_camera_set_mode,
    0x00000000, 0x00000007, 0x0004005B, (u32)evt_btl_camera_set_homing_unit, 0x00000000, 0xFFFFFFFE,
    0xFFFFFFFE, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001, 0x0003005B,
    (u32)evt_btl_camera_set_zoom, 0x00000000, 0x000000C8, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE,
    0xF24A8680, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001, 0x0000002A,
    0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFFFFFFC4, 0xFE363C81, 0xFE363C82,
    0x00000000, 0xFFFFFFFF, 0x00000000, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000,
    0x00000004, 0x0003005B, (u32)evt_btl_camera_set_zoom, 0x00000000, 0x000001C2, 0x0002005B,
    (u32)_get_mario_hammer_lv, 0xFE363C80, 0x00010022, 0xFE363C80, 0x00010024, 0x00000002,
    0x00020032, 0xFE363C80, (u32)(char*)str_M_H_5A_802ef21c, 0x00020032, 0xFE363C81, (u32)(char*)str_M_H_5B_802ef224,
    0x00020032, 0xFE363C82, (u32)(char*)str_M_H_13_802ef25c, 0x00010024, 0x00000003, 0x00020032,
    0xFE363C80, (u32)(char*)str_M_H_9A_802ef234, 0x00020032, 0xFE363C81, (u32)(char*)str_M_H_9B_802ef23c, 0x00020032,
    0xFE363C82, (u32)(char*)str_M_H_14_802ef22c, 0x0000002A, 0x00020032, 0xFE363C80, (u32)(char*)str_M_H_1A_802ef24c,
    0x00020032, 0xFE363C81, (u32)(char*)str_M_H_1B_802ef254, 0x00020032, 0xFE363C82, (u32)(char*)str_M_H_15_802ef244,
    0x00000031, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, 0xFE363C80,
    0x0002005B, (u32)_get_mario_hammer_lv, 0xFE363C80, 0x00010022, 0xFE363C80, 0x00010024,
    0x00000002, 0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_MARIO_HAMMER_HOL_802ef27c, 0xF1194D80,
    0x00000000, 0xF1194D80, 0x00010024, 0x00000001, 0x0006005B, (u32)btlevtcmd_snd_se,
    0xFFFFFFFE, (u32)(char*)str_SFX_MARIO_HAMMER_HOL_802ef294, 0xF1194D80, 0x00000000, 0xF1194D80, 0x0000002A,
    0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_MARIO_HAMMER_HOL_802ef2ac, 0xF1194D80, 0x00000000,
    0xF1194D80, 0x00000031, 0x00010009, 0x0000001E, 0x0004005B, (u32)btlevtcmd_AnimeChangePose,
    0xFFFFFFFE, 0x00000001, 0xFE363C81, 0x0002005B, (u32)btlevtcmd_StartAC, 0x00000001,
    0x00010009, 0x00000020, 0x0001006F, 0xFE363C8D, 0x00020032, 0xFE363C85,
    0x00000005, 0x00010005, 0xFE363C85, 0x0003005B, (u32)btlevtcmd_AcGetOutputParam, 0x00000001,
    0xFE363C86, 0x0002001D, 0xFE363C86, 0x00000002, 0x00000007, 0x00000021,
    0x00010009, 0x00000001, 0x00020032, 0xFE363C85, 0x00000005, 0x00000006,
    0x00010009, 0x0000002C, 0x00000070, 0x0001006F, 0xFE363C8E, 0x00020032,
    0xFE363C85, 0x00000005, 0x00010005, 0xFE363C85, 0x0003005B, (u32)btlevtcmd_AcGetOutputParam,
    0x00000001, 0xFE363C86, 0x0002001D, 0xFE363C86, 0x00000002, 0x00000007,
    0x00000021, 0x00010009, 0x00000001, 0x00020032, 0xFE363C85, 0x00000005,
    0x00000006, 0x00010009, 0x0000000A, 0x0003005B, (u32)btlevtcmd_GetFaceDirection, 0xFFFFFFFE,
    0xFE363C85, 0x00020037, 0xFE363C85, 0x0000002D, 0x0005005B, (u32)evt_sub_intpl_init,
    0x0000000B, 0x00000000, 0xFE363C85, 0x0000000A, 0x00010005, 0x0000000F,
    0x0001005B, (u32)evt_sub_intpl_get_value, 0x0005005B, (u32)btlevtcmd_SetRotate, 0xFFFFFFFE, 0x00000000,
    0x00000000, 0xFE363C80, 0x00010009, 0x00000001, 0x00000006, 0x00010009,
    0x0000000E, 0x0005005B, (u32)evt_sub_intpl_init, 0x0000000C, 0xFE363C85, 0x00000000,
    0x0000000F, 0x00010005, 0x0000000F, 0x0001005B, (u32)evt_sub_intpl_get_value, 0x0005005B,
    (u32)btlevtcmd_SetRotate, 0xFFFFFFFE, 0x00000000, 0x00000000, 0xFE363C80, 0x00010009,
    0x00000001, 0x00000006, 0x00000070, 0x0001006F, 0xFE363C8F, 0x00020032,
    0xFE363C85, 0x00000005, 0x00010005, 0xFE363C85, 0x0003005B, (u32)btlevtcmd_AcGetOutputParam,
    0x00000001, 0xFE363C86, 0x0002001D, 0xFE363C86, 0x00000002, 0x00000007,
    0x00000021, 0x00010009, 0x00000001, 0x00020032, 0xFE363C85, 0x00000005,
    0x00000006, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C86, 0xFE363C87,
    0xFE363C88, 0x0003005B, (u32)btlevtcmd_SetFallAccel, 0xFFFFFFFE, 0xF24A7B75, 0x0007005B,
    (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C86, 0xFE363C87, 0xFE363C88, 0x00000037,
    0xFFFFFFFF, 0x00000070, 0x0001005B, (u32)btlevtcmd_ResultAC, 0x0002005B, (u32)btlevtcmd_GetResultAC,
    0xFE363C88, 0x0003005B, (u32)btlevtcmd_AcGetOutputParam, 0x00000001, 0xFE363C80, 0x0002001D,
    0xFE363C80, 0x00000002, 0x00010004, 0x0000000A, 0x00000021, 0x0001005F,
    0xFE363C8D, 0x0001005F, 0xFE363C8E, 0x0001005F, 0xFE363C8F, 0x0002005B,
    (u32)_get_mario_hammer_lv, 0xFE363C80, 0x00010022, 0xFE363C80, 0x00010024, 0x00000002,
    0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_MARIO_HAMMER_SWI_802ef2c4, 0x00000000, 0x00020032, 0xFE363C82,
    (u32)(char*)str_M_H_13_802ef25c, 0x00010024, 0x00000001, 0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_MARIO_HAMMER_SWI_802ef2dc,
    0x00000000, 0x00020032, 0xFE363C82, (u32)(char*)str_M_H_15_802ef244, 0x0000002A, 0x0003005B,
    (u32)evt_snd_sfxon, (u32)(char*)str_SFX_MARIO_HAMMER_SWI_802ef2f4, 0x00000000, 0x00020032, 0xFE363C82, (u32)(char*)str_M_H_14_802ef22c,
    0x00000031, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, 0xFE363C82,
    0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_MARIO_ATTACK_ULT_802ef458, 0x00000000, 0x00010009, 0x0000000A,
    0x00010004, 0x00000028, 0x00010003, 0x0000000A, 0x0002005B, (u32)btlevtcmd_WaitEventEnd,
    0xFE363C8D, 0x0002005B, (u32)_get_mario_hammer_lv, 0xFE363C80, 0x00010022, 0xFE363C80,
    0x00010024, 0x00000002, 0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_MARIO_HAMMER_SWI_802ef2c4, 0x00000000,
    0x00010024, 0x00000001, 0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_MARIO_HAMMER_SWI_802ef2dc, 0x00000000,
    0x0000002A, 0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_MARIO_HAMMER_SWI_802ef2f4, 0x00000000, 0x00000031,
    0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, 0xFE363C82, 0x0003005B,
    (u32)evt_snd_sfxon, (u32)(char*)str_SFX_MARIO_ATTACK_ULT_802ef458, 0x00000000, 0x0002005B, (u32)btlevtcmd_WaitEventEnd, 0xFE363C8E,
    0x0002005B, (u32)btlevtcmd_WaitEventEnd, 0xFE363C8F, 0x00010003, 0x00000028, 0x0003005B,
    (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000001, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000,
    0x00000000, 0x0003005B, (u32)evt_btl_camera_set_zoom, 0x00000000, 0x00000000, 0x0002005B,
    (u32)evt_btl_camera_off_posoffset_manual, 0x00000000, 0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_MARIO_HAMMER_ULT_802ef478, 0x00000000,
    0x0006005B, (u32)evt_btl_camera_shake_h, 0x00000000, 0x00000008, 0x00000000, 0x0000002A,
    0x0000000D, 0x0002005B, (u32)evt_audience_reflesh_natural_all, 0x00000064, 0x0002005B, (u32)btlevtcmd_GetResultAC,
    0xFE363C87, 0x0002001E, 0xFE363C87, 0x00000002, 0x0004005B, (u32)btlevtcmd_GetResultPrizeLv,
    0xFFFFFFFB, 0x00000000, 0xFE363C86, 0x0005005B, (u32)btlevtcmd_ACSuccessEffect, 0xFE363C86,
    0x00000000, 0x00000032, 0x00000000, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress, 0xFFFFFFFE,
    0xFE363C80, 0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult, 0xFE363C80, 0xFE363C86, 0x00000020,
    0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress, 0xFFFFFFFE, 0xFE363C80, 0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult,
    0xFE363C80, 0xFFFFFFFF, 0x00000021, 0x0003005B, (u32)btlevtcmd_GetSelectEnemy, 0xFE363C83,
    0xFE363C84, 0x00020018, 0xFE363C83, 0xFFFFFFFF, 0x0001005B, (u32)btlevtcmd_StopAC,
    0x00010004, 0x0000005A, 0x00000021, 0x00010004, 0x00000033, 0x00010003,
    0x00000032, 0x0003005B, (u32)btlevtcmd_GetSelectNextEnemy, 0xFE363C83, 0xFE363C84, 0x00020018,
    0xFE363C83, 0xFFFFFFFF, 0x0001005B, (u32)btlevtcmd_StopAC, 0x00010004, 0x0000005A,
    0x00000021, 0x00010003, 0x00000033, 0x0006005B, (u32)btlevtcmd_CommandPreCheckDamage, 0xFFFFFFFE,
    0xFE363C83, 0xFE363C84, 0x00000100, 0xFE363C86, 0x00020018, 0xFE363C86,
    0x00000001, 0x0007005B, (u32)btlevtcmd_DamageDirect, 0xFE363C83, 0xFE363C84, 0x00000000,
    0x00000000, 0x00000002, 0x00000000, 0x0000006B, 0x00010009, 0x0000002A,
    0x0002001E, 0xFE363C87, 0x00000002, 0x0006005B, (u32)btlevtcmd_CommandCheckDamage, 0xFFFFFFFE,
    0xFE363C83, 0xFE363C84, 0x00020100, 0xFE363C85, 0x00000020, 0x0006005B,
    (u32)btlevtcmd_CommandCheckDamage, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0x00000100, 0xFE363C85,
    0x00000021, 0x0000006D, 0x00000020, 0x00020018, 0xFE363C86, 0x00000003,
    0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000026, 0x00000021, 0x00020018,
    0xFE363C86, 0x00000006, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000027,
    0x00000021, 0x00020018, 0xFE363C86, 0x00000002, 0x0003005B, (u32)btlevtcmd_StartAvoid,
    0xFE363C83, 0x00000028, 0x00000021, 0x00000021, 0x00010004, 0x00000032,
    0x00010003, 0x0000005A, 0x00010009, 0x0000000C, 0x0003005B, (u32)evt_btl_camera_set_mode,
    0x00000000, 0x00000000, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001,
    0x00010009, 0x0000001E, 0x0001005B, (u32)evt_audience_ap_recovery, 0x0001005B, (u32)btlevtcmd_InviteApInfoReport,
    0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress, 0xFFFFFFFE, 0xFE363C80, 0x0002005B, (u32)btlevtcmd_WeaponAftereffect,
    0xFE363C80, 0x00010009, 0x0000003C, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE,
    0x00000001, (u32)(char*)str_M_H_16_802ef388, 0x00010009, 0x0000000A, 0x00010003, 0x0000005F,
    0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_W_1_80422320, 0x0003005B,
    (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE, 0xF24A9280, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE,
    0x00000001, 0x00000028, 0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000000, 0xFFFFFFFF, 0x00000000, 0x00010003,
    0x00000063, 0x0002005B, (u32)btlevtcmd_ResetFaceDirection, 0xFFFFFFFE, 0x0001005E, (u32)btldefaultevt_SuitoruBadgeEffect,
    0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002, 0x00000001,
};

/* .data + 0x0000DB74 */
u32 marioAttackEvent_HammerRotate[] = {
    0x00020032, 0xFE363C80, 0xFFFFFFE7, 0x0006005B, (u32)btlevtcmd_SetPartsRotateOffset, 0xFFFFFFFE,
    0x00000003, 0x00000000, 0x0000000C, 0x00000000, 0x00010003, 0x00000000,
    0x0006005B, (u32)btlevtcmd_AddPartsRotate, 0xFFFFFFFE, 0x00000003, 0x00000000, 0x00000000,
    0xFE363C80, 0x00010009, 0x00000001, 0x00010004, 0x00000000, 0x00000002,
    0x00000001,
};

/* .data + 0x0000DBD8 */
u32 marioAttackEvent_HammerNageru[] = {
    0x0003005B, (u32)btlevtcmd_GetSelectEnemy, 0xFE363C83, 0xFE363C84, 0x00020018, 0xFE363C83,
    0xFFFFFFFF, 0x00010004, 0x00000063, 0x00000021, 0x0004005B, (u32)btlevtcmd_AttackDeclare,
    0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0x0001005B, (u32)btlevtcmd_WaitGuardMove, 0x0003005B,
    (u32)btlevtcmd_CommandGetWeaponAddress, 0xFFFFFFFE, 0xFE363C8C, 0x0002005B, (u32)btlevtcmd_CommandPayWeaponCost, 0xFFFFFFFE,
    0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFFFFFFFE, 0x00000007, 0x0001005E, (u32)marioAttackEvent_MajinaiPowerUpCheck,
    0x0002005B, (u32)btlevtcmd_CommandGetWeaponActionLv, 0xFE363C80, 0x0003005B, (u32)btlevtcmd_AcSetDifficulty, 0xFFFFFFFE,
    0xFE363C80, 0x0009005B, (u32)btlevtcmd_AcSetParamAll, 0x00000005, 0x00000028, 0x00000000,
    0x00000000, 0x00000000, 0xF1194D80, 0xF1194D80, 0xF1194D80, 0x0002005B,
    (u32)btlevtcmd_AcSetFlag, 0x00000000, 0x0005005B, (u32)btlevtcmd_SetupAC, 0xFFFFFFFE, 0x00000003,
    0x00000001, 0x00000000, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000007,
    0x0004005B, (u32)evt_btl_camera_set_homing_unit, 0x00000000, 0xFFFFFFFE, 0xFFFFFFFE, 0x0003005B,
    (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001, 0x0003005B, (u32)evt_btl_camera_set_zoom, 0x00000000,
    0x000000C8, 0x0006005B, (u32)btlevtcmd_CommandPreCheckDamage, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84,
    0x00000100, 0xFE363C86, 0x0007005B, (u32)btlevtcmd_CalculateFaceDirection, 0xFFFFFFFE, 0xFFFFFFFF,
    0xFE363C83, 0xFE363C84, 0x00000010, 0xFE363C8F, 0x0003005B, (u32)btlevtcmd_ChangeFaceDirection,
    0xFFFFFFFE, 0xFE363C8F, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000004,
    0x0003005B, (u32)evt_btl_camera_set_zoom, 0x00000000, 0x000001C2, 0x0006005B, (u32)btlevtcmd_snd_se,
    0xFFFFFFFE, (u32)(char*)str_SFX_VOICE_MARIO_POWE_802eefa4, 0xF1194D80, 0x00000000, 0xF1194D80, 0x0002005B,
    (u32)_get_mario_hammer_lv, 0xFE363C80, 0x00010022, 0xFE363C80, 0x00010024, 0x00000002,
    0x00020032, 0xFE363C80, (u32)(char*)str_M_H_5A_802ef21c, 0x00020032, 0xFE363C81, (u32)(char*)str_M_H_5B_802ef224,
    0x00020032, 0xFE363C82, (u32)(char*)str_M_H_7_80422388, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE,
    0x00000003, (u32)(char*)str_M_H_8_80422390, 0x00010024, 0x00000003, 0x00020032, 0xFE363C80,
    (u32)(char*)str_M_H_9A_802ef234, 0x00020032, 0xFE363C81, (u32)(char*)str_M_H_9B_802ef23c, 0x00020032, 0xFE363C82,
    (u32)(char*)str_M_H_11_802ef498, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000003, (u32)(char*)str_M_H_12_802ef4a0,
    0x0000002A, 0x00020032, 0xFE363C80, (u32)(char*)str_M_H_1A_802ef24c, 0x00020032, 0xFE363C81,
    (u32)(char*)str_M_H_1B_802ef254, 0x00020032, 0xFE363C82, (u32)(char*)str_M_H_3_80422398, 0x0004005B, (u32)btlevtcmd_AnimeChangePose,
    0xFFFFFFFE, 0x00000003, (u32)(char*)str_M_H_4_804223a0, 0x00000031, 0x0004005B, (u32)btlevtcmd_AnimeChangePose,
    0xFFFFFFFE, 0x00000001, 0xFE363C80, 0x0002005B, (u32)_get_mario_hammer_lv, 0xFE363C80,
    0x00010022, 0xFE363C80, 0x00010024, 0x00000002, 0x0006005B, (u32)btlevtcmd_snd_se,
    0xFFFFFFFE, (u32)(char*)str_SFX_MARIO_HAMMER_HOL_802ef27c, 0xF1194D80, 0x00000000, 0xF1194D80, 0x00010024,
    0x00000001, 0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_MARIO_HAMMER_HOL_802ef294, 0xF1194D80,
    0x00000000, 0xF1194D80, 0x0000002A, 0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE,
    (u32)(char*)str_SFX_MARIO_HAMMER_HOL_802ef2ac, 0xF1194D80, 0x00000000, 0xF1194D80, 0x00000031, 0x00010009,
    0x0000001E, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, 0xFE363C81,
    0x0002005B, (u32)btlevtcmd_StartAC, 0x00000001, 0x0001005B, (u32)btlevtcmd_ResultAC, 0x0003005B,
    (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000,
    0x00000001, 0x0002005B, (u32)evt_btl_camera_off_posoffset_manual, 0x00000000, 0x0003005B, (u32)evt_snd_sfxon,
    (u32)(char*)str_SFX_MARIO_HAMMER_NAG_802ef4a8, 0x00000000, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001,
    0xFE363C82, 0x00010009, 0x00000003, 0x0002005D, (u32)marioAttackEvent_HammerNageru_object, 0xFE363C8F,
    0x0006005B, (u32)btlevtcmd_ACRStart, 0xFFFFFFFE, 0x00000000, 0x0000000F, 0x0000000F,
    0x00000000, 0x0003005B, (u32)btlevtcmd_ACRGetResult, 0xFE363C86, 0xFE363C87, 0x00010022,
    0xFE363C86, 0x00010029, 0x00000002, 0x0000002A, 0x0001005B, (u32)evt_audience_acrobat_notry,
    0x0001000A, 0x000003E8, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001,
    (u32)(char*)str_M_S_1_80422338, 0x0001000A, 0x0000029B, 0x00010004, 0x0000005E, 0x00000031,
    0x0000006E, 0x0003005B, (u32)btlevtcmd_snd_voice, 0xFFFFFFFE, 0x00000001, 0x0003005B,
    (u32)btlevtcmd_CommandGetWeaponAddress, 0xFFFFFFFE, 0xFE363C8C, 0x0006005B, (u32)btlevtcmd_AudienceDeclareAcrobatResult, 0xFE363C8C,
    0x00000001, 0xFFFFFFD8, 0x0000001E, 0x00000000, 0x0005005B, (u32)btlevtcmd_SetRotateOffset,
    0xFFFFFFFE, 0x00000000, 0x00000014, 0x00000000, 0x00010005, 0x0000001E,
    0x0005005B, (u32)btlevtcmd_AddRotate, 0xFFFFFFFE, 0x00000000, 0x00000000, 0xFFFFFFE8,
    0x00010009, 0x00000001, 0x00000006, 0x00000070, 0x0000006E, 0x00010009,
    0x00000008, 0x0006005B, (u32)btlevtcmd_ACRStart, 0xFFFFFFFE, 0x0000000C, 0x00000019,
    0x00000019, 0x00000000, 0x00000070, 0x0000006E, 0x00010009, 0x00000004,
    0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_Z_1_80422370, 0x00000070,
    0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_A_3A_802ef390, 0x0003005B,
    (u32)btlevtcmd_SetFallAccel, 0xFFFFFFFE, 0xF24A7B4C, 0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0000001E, 0xFFFFFFFF, 0x00010009,
    0x00000004, 0x0003005B, (u32)btlevtcmd_ACRGetResult, 0xFE363C80, 0xFE363C81, 0x00020018,
    0xFE363C80, 0x00000002, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001,
    (u32)(char*)str_M_A_3B_802ef0e0, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00020035, 0xFE363C81, 0x00000032, 0x000F005B, (u32)evt_eff,
    (u32)(char*)str_BTLEF_802ef0e8, (u32)(char*)str_confetti_802ef0f0, 0x00000003, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0000005A, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress, 0xFFFFFFFE, 0xFE363C8C,
    0x0006005B, (u32)btlevtcmd_AudienceDeclareAcrobatResult, 0xFE363C8C, 0x00000001, 0x00000000, 0x00000000,
    0x00000000, 0x0001000A, 0x000005DC, 0x00000021, 0x00010003, 0x0000005E,
    0x0002005B, (u32)btlevtcmd_WaitEventEnd, 0xFE363C8F, 0x0001005B, (u32)evt_audience_ap_recovery, 0x0001005B,
    (u32)btlevtcmd_InviteApInfoReport, 0x00010003, 0x0000005F, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000,
    0x00000000, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000005, 0x0002005B,
    (u32)evt_btl_camera_off_posoffset_manual, 0x00000000, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001,
    (u32)(char*)str_M_W_1_80422320, 0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE, 0xF24A9280, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001, 0x00000028, 0x0005005B, (u32)btlevtcmd_GetHomePos,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0008005B, (u32)btlevtcmd_MovePosition,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000, 0xFFFFFFFF,
    0x00000000, 0x00010003, 0x00000063, 0x0002005B, (u32)btlevtcmd_ResetFaceDirection, 0xFFFFFFFE,
    0x0001005E, (u32)btldefaultevt_SuitoruBadgeEffect, 0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002,
    0x00000001,
};

/* .data + 0x0000E2FC */
u32 marioAttackEvent_HammerNageru_object[] = {
    0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00020035, 0xFE363C81, 0x00000019, 0x0006005B, (u32)btlevtcmd_SetPartsPos, 0xFFFFFFFE,
    0x00000003, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B, (u32)btlevtcmd_OffPartsAttribute,
    0xFFFFFFFE, 0x00000003, 0x01000000, 0x00020032, 0xFE363C80, 0xFFFFFFE7,
    0x0006005B, (u32)btlevtcmd_SetPartsRotate, 0xFFFFFFFE, 0x00000003, 0x00000000, 0x00000000,
    0x00000000, 0x0002005D, (u32)marioAttackEvent_HammerRotate, 0xFE363C8D, 0x0006005B, (u32)btlevtcmd_JumpPartsSetting,
    0xFFFFFFFE, 0x00000003, 0x00000000, 0xF24A9280, 0xF24A7BE6, 0x0006005B,
    (u32)btlevtcmd_GetHitPos, 0xFE363C83, 0xFE363C84, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0008005B, (u32)btlevtcmd_JumpPartsPosition, 0xFFFFFFFE, 0x00000003, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000000, 0xFFFFFFFF, 0x00020019, 0xFE363C86, 0x00000001,
    0x00020018, 0xFE363C86, 0x00000003, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83,
    0x00000026, 0x00000021, 0x00020018, 0xFE363C86, 0x00000006, 0x0003005B,
    (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000027, 0x00000021, 0x00020018, 0xFE363C86,
    0x00000002, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000028, 0x00000021,
    0x0003005B, (u32)btlevtcmd_JumpPartsContinue, 0xFFFFFFFE, 0x00000003, 0x0002005B, (u32)btlevtcmd_WeaponAftereffect,
    0xFE363C8C, 0x0006005B, (u32)btlevtcmd_GetPartsPos, 0xFFFFFFFE, 0x00000003, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0004005B, (u32)btlevtcmd_FaceDirectionAdd, 0xFE363C83, 0xFE363C80,
    0xFFFFFFE2, 0x0008005B, (u32)btlevtcmd_JumpPartsPosition, 0xFFFFFFFE, 0x00000003, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000014, 0xFFFFFFFF, 0x0004005B, (u32)btlevtcmd_FaceDirectionAdd,
    0xFE363C83, 0xFE363C80, 0xFFFFFFF1, 0x0008005B, (u32)btlevtcmd_JumpPartsPosition, 0xFFFFFFFE,
    0x00000003, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0000000C, 0xFFFFFFFF,
    0x0001005F, 0xFE363C8D, 0x00010009, 0x0000001E, 0x0004005B, (u32)btlevtcmd_OnPartsAttribute,
    0xFFFFFFFE, 0x00000003, 0x01000000, 0x00010004, 0x0000005A, 0x00000021,
    0x0000006B, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000001, 0x0003005B,
    (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000004, 0x0003005B, (u32)evt_btl_camera_set_zoom, 0x00000000,
    0x00000000, 0x0002005B, (u32)evt_btl_camera_off_posoffset_manual, 0x00000000, 0x0000006D, 0x0002005B,
    (u32)btlevtcmd_WeaponAftereffect, 0xFE363C8C, 0x0002005B, (u32)btlevtcmd_GetResultAC, 0xFE363C80, 0x0002001E,
    0xFE363C80, 0x00000002, 0x0006005B, (u32)btlevtcmd_CommandCheckDamage, 0xFFFFFFFE, 0xFE363C83,
    0xFE363C84, 0x00020100, 0xFE363C85, 0x0004005B, (u32)btlevtcmd_GetResultPrizeLv, 0xFE363C83,
    0x00000000, 0xFE363C86, 0x0006005B, (u32)btlevtcmd_GetHitPos, 0xFE363C83, 0xFE363C84,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)btlevtcmd_ACSuccessEffect, 0xFE363C86,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress, 0xFFFFFFFE,
    0xFE363C80, 0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult, 0xFE363C80, 0xFE363C86, 0x00000020,
    0x0006005B, (u32)btlevtcmd_CommandCheckDamage, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0x00000100,
    0xFE363C85, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress, 0xFFFFFFFE, 0xFE363C80, 0x0003005B,
    (u32)btlevtcmd_AudienceDeclareACResult, 0xFE363C80, 0xFFFFFFFF, 0x00000021, 0x0003005B, (u32)btlevtcmd_GetDamage,
    0xFE363C83, 0xFE363C80, 0x0002001D, 0xFE363C80, 0x0000000A, 0x00020032,
    0xFE363C80, 0x00000009, 0x00000021, 0x0006005B, (u32)evt_btl_camera_shake_h, 0x00000000,
    0xFE363C80, 0x00000000, 0x0000000A, 0x0000000D, 0x0001005F, 0xFE363C8D,
    0x0004005B, (u32)btlevtcmd_OnPartsAttribute, 0xFFFFFFFE, 0x00000003, 0x01000000, 0x00010009,
    0x00000014, 0x00010003, 0x0000005A, 0x0001005B, (u32)btlevtcmd_StopAC, 0x00000002,
    0x00000001,
};

/* .data + 0x0000E6D8 */
u32 marioAttackEvent_FireNaguri[] = {
    0x0002005B, (u32)btlevtcmd_AttackDeclareAll, 0xFFFFFFFE, 0x0001005B, (u32)btlevtcmd_WaitGuardMove, 0x0002005B,
    (u32)btlevtcmd_CommandPayWeaponCost, 0xFFFFFFFE, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFFFFFFFE, 0x00000007,
    0x0001005E, (u32)marioAttackEvent_MajinaiPowerUpCheck, 0x0002005B, (u32)btlevtcmd_CommandGetWeaponActionLv, 0xFE363C80, 0x0003005B,
    (u32)btlevtcmd_AcSetDifficulty, 0xFFFFFFFE, 0xFE363C80, 0x0009005B, (u32)btlevtcmd_AcSetParamAll, 0x00000000,
    0x00000053, 0x0000003F, 0x00000053, 0xF1194D80, 0xF1194D80, 0xF1194D80,
    0xF1194D80, 0x0002005B, (u32)btlevtcmd_AcSetFlag, 0x00000010, 0x0005005B, (u32)btlevtcmd_SetupAC,
    0xFFFFFFFE, 0x0000000B, 0x00000001, 0x00000000, 0x00010009, 0x00000002,
    0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000007, 0x0004005B, (u32)evt_btl_camera_set_homing_unit,
    0x00000000, 0xFFFFFFFE, 0xFFFFFFFE, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000,
    0x00000001, 0x0003005B, (u32)evt_btl_camera_set_zoom, 0x00000000, 0x000000C8, 0x0005005B,
    (u32)evt_btl_camera_set_posoffset, 0x00000000, 0x00000028, 0x00000000, 0x00000000, 0x0005005B,
    (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B,
    (u32)btlevtcmd_FaceDirectionAdd, 0xFFFFFFFE, 0xFE363C80, 0x00000019, 0x0004005B, (u32)btlevtcmd_GetStageSize,
    0xF1194D80, 0xFE363C81, 0xF1194D80, 0x0006005B, (u32)btlevtcmd_SetPartsPos, 0xFFFFFFFE,
    0x00000003, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B, (u32)btlevtcmd_AnimeChangePose,
    0xFFFFFFFE, 0x00000003, (u32)(char*)str_M_H_8_80422390, 0x0005005B, (u32)btlevtcmd_SetPartsWork, 0xFFFFFFFE,
    0x00000003, 0x00000000, 0x00000001, 0x0000006E, 0x0003005B, (u32)_mario_fire_ball_controll,
    0xFFFFFFFE, 0x00000003, 0x00000070, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE,
    0xF1194D80, 0xFE363C81, 0xF1194D80, 0x00020035, 0xFE363C81, 0x0000008C,
    0x0001006F, 0xFE363C8F, 0x000B005B, (u32)btlevtcmd_DivePartsPosition, 0xFFFFFFFE, 0x00000003,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0000003C, 0x00000000, 0x00000005,
    0x00000000, 0xFFFFFFFF, 0x00000070, 0x00010003, 0x0000000A, 0x0004005B,
    (u32)btlevtcmd_PadCheckNow, 0x00000000, 0x00040000, 0xFE363C80, 0x00020018, 0xFE363C80,
    0x00000000, 0x00010009, 0x00000001, 0x00010004, 0x0000000A, 0x00000021,
    0x0002006A, 0xFE363C8F, 0xFE363C80, 0x00020019, 0xFE363C80, 0x00000000,
    0x0001005F, 0xFE363C8F, 0x00000021, 0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE,
    (u32)(char*)str_SFX_VOICE_MARIO_POWE_802eefa4, 0xF1194D80, 0x00000000, 0xF1194D80, 0x0002005B, (u32)_get_mario_hammer_lv,
    0xFE363C80, 0x00010022, 0xFE363C80, 0x00010024, 0x00000002, 0x00020032,
    0xFE363C80, (u32)(char*)str_M_H_18A_802ef4c4, 0x00020032, 0xFE363C81, (u32)(char*)str_M_H_18B_802ef4cc, 0x00020032,
    0xFE363C82, (u32)(char*)str_M_H_18C_802ef4d4, 0x00010024, 0x00000003, 0x00020032, 0xFE363C80,
    (u32)(char*)str_M_H_19A_802ef4dc, 0x00020032, 0xFE363C81, (u32)(char*)str_M_H_19B_802ef4e4, 0x00020032, 0xFE363C82,
    (u32)(char*)str_M_H_19C_802ef4ec, 0x0000002A, 0x00020032, 0xFE363C80, (u32)(char*)str_M_H_17A_802ef4f4, 0x00020032,
    0xFE363C81, (u32)(char*)str_M_H_17B_802ef4fc, 0x00020032, 0xFE363C82, (u32)(char*)str_M_H_17C_802ef504, 0x00000031,
    0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, 0xFE363C80, 0x0002005B,
    (u32)_get_mario_hammer_lv, 0xFE363C80, 0x00010022, 0xFE363C80, 0x00010024, 0x00000002,
    0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_MARIO_HAMMER_HOL_802ef27c, 0xF1194D80, 0x00000000,
    0xF1194D80, 0x00010024, 0x00000001, 0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE,
    (u32)(char*)str_SFX_MARIO_HAMMER_HOL_802ef294, 0xF1194D80, 0x00000000, 0xF1194D80, 0x0000002A, 0x0006005B,
    (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_MARIO_HAMMER_HOL_802ef2ac, 0xF1194D80, 0x00000000, 0xF1194D80,
    0x00000031, 0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_BTL_MARIO_HAMMER_802ef50c, 0xF1194D80,
    0x00000000, 0xFE363C8F, 0x0001006C, 0xFE363C8E, 0x0006005B, (u32)btlevtcmd_GetPartsPos,
    0xFFFFFFFE, 0x00000003, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B,
    (u32)btlevtcmd_TransStageFloorPosition, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0006005B, (u32)btlevtcmd_JumpPartsSetting,
    0xFFFFFFFE, 0x00000003, 0x00000000, 0xF24A8A80, 0xF24A7B4C, 0x0006005B,
    (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_BTL_MARIO_HAMMER_802ef528, 0xF1194D80, 0x00000000, 0xF1194D80,
    0x0007005B, (u32)btlevtcmd_FallPartsPosition, 0xFFFFFFFE, 0x00000003, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0000004B, 0x0000006D, 0x0002005B, (u32)btlevtcmd_StartAC, 0x00000001,
    0x00010009, 0x00000014, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001,
    0xFE363C81, 0x0001005B, (u32)btlevtcmd_ResultAC, 0x0002005B, (u32)btlevtcmd_GetResultAC, 0xFE363C80,
    0x0002001F, 0xFE363C80, 0x00000002, 0x0003005B, (u32)btlevtcmd_AcGetOutputParam, 0x00000000,
    0xFE363C80, 0x0000006E, 0x0002005B, (u32)_get_mario_hammer_lv, 0xFE363C80, 0x00010022,
    0xFE363C80, 0x00010024, 0x00000002, 0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_MARIO_HAMMER_SWI_802ef2c4,
    0x00000000, 0x00010024, 0x00000001, 0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_MARIO_HAMMER_SWI_802ef2dc,
    0x00000000, 0x0000002A, 0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_MARIO_HAMMER_SWI_802ef2f4, 0x00000000,
    0x00000031, 0x0002005B, (u32)evt_snd_sfxoff, 0xFE363C8F, 0x0006005B, (u32)btlevtcmd_snd_se,
    0xFFFFFFFE, (u32)(char*)str_SFX_BTL_MARIO_HAMMER_802ef544, 0xF1194D80, 0x00000000, 0xF1194D80, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, 0xFE363C82, 0x00010009, 0x00000007,
    0x00010009, 0x0000003C, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001,
    (u32)(char*)str_M_S_1_80422338, 0x00000070, 0x0002005B, (u32)btlevtcmd_WaitEventEnd, 0xFE363C8E, 0x0005005B,
    (u32)btlevtcmd_SetPartsWork, 0xFFFFFFFE, 0x00000003, 0x00000000, 0x00000000, 0x0001005B,
    (u32)evt_audience_ap_recovery, 0x0001005B, (u32)btlevtcmd_InviteApInfoReport, 0x0006005B, (u32)btlevtcmd_GetPartsPos, 0xFFFFFFFE,
    0x00000003, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000F005B, (u32)evt_eff64,
    (u32)(u8*)&zero_804223a8, (u32)(char*)str_fire_spark_n64_802ef560, 0x00000000, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0000001E, 0x00000028, 0xF24A7E80, 0x0000000C, 0x00000064, 0xF24A7E80,
    0xF24A7E80, 0x00000000, 0x00020035, 0xFE363C81, 0x00000005, 0x000F005B,
    (u32)evt_eff, (u32)(u8*)&zero_804223a8, (u32)(char*)str_kemuri_test_802ef570, 0x0000000B, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0xF24A7E80, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000,
    0x00000001, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001, 0x0003005B,
    (u32)evt_btl_camera_set_zoom, 0x00000000, 0x00000000, 0x0002005B, (u32)evt_btl_camera_off_posoffset_manual, 0x00000000,
    0x0003005B, (u32)btlevtcmd_GetUnitId, 0xFFFFFFFE, 0xFE363C86, 0x0003005B, (u32)btlevtcmd_GetEnemyBelong,
    0xFE363C86, 0xFE363C80, 0x0004005B, (u32)btlevtcmd_SamplingEnemy, 0xFFFFFFFB, 0xFE363C80,
    (u32)badgeWeapon_FireNaguriFailed, 0x0003005B, (u32)btlevtcmd_GetSelectEnemy, 0xFE363C83, 0xFE363C84, 0x00010003,
    0x0000000F, 0x00020018, 0xFE363C83, 0xFFFFFFFF, 0x00010004, 0x00000011,
    0x00000021, 0x00020018, 0xFE363C83, 0xFE363C86, 0x00010004, 0x00000010,
    0x00000021, 0x00010003, 0x00000010, 0x0003005B, (u32)btlevtcmd_GetSelectNextEnemy, 0xFE363C83,
    0xFE363C84, 0x00010004, 0x0000000F, 0x00010003, 0x00000011, 0x0003005B,
    (u32)btlevtcmd_GetUnitId, 0xFFFFFFFE, 0xFE363C86, 0x0003005B, (u32)btlevtcmd_GetEnemyBelong, 0xFE363C86,
    0xFE363C80, 0x0004005B, (u32)btlevtcmd_SamplingEnemy, 0xFFFFFFFB, 0xFE363C80, (u32)badgeWeapon_FireNaguriFailed,
    0x0003005B, (u32)btlevtcmd_GetSelectEnemy, 0xFE363C83, 0xFE363C84, 0x00010003, 0x00000014,
    0x00020018, 0xFE363C83, 0xFFFFFFFF, 0x00010004, 0x00000016, 0x00000021,
    0x00020019, 0xFE363C83, 0xFE363C86, 0x00010004, 0x00000015, 0x00000021,
    0x0006005B, (u32)btlevtcmd_CommandPreCheckDamage, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0x00000100,
    0xFE363C85, 0x00010003, 0x00000015, 0x0003005B, (u32)btlevtcmd_GetSelectNextEnemy, 0xFE363C83,
    0xFE363C84, 0x00010004, 0x00000014, 0x00010003, 0x00000016, 0x00010009,
    0x0000001E, 0x00010004, 0x00000063, 0x00000021, 0x0002005B, (u32)_get_mario_hammer_lv,
    0xFE363C80, 0x00010022, 0xFE363C80, 0x00010024, 0x00000002, 0x0003005B,
    (u32)evt_snd_sfxon, (u32)(char*)str_SFX_MARIO_HAMMER_SWI_802ef2c4, 0x00000000, 0x00010024, 0x00000001, 0x0003005B,
    (u32)evt_snd_sfxon, (u32)(char*)str_SFX_MARIO_HAMMER_SWI_802ef2dc, 0x00000000, 0x0000002A, 0x0003005B, (u32)evt_snd_sfxon,
    (u32)(char*)str_SFX_MARIO_HAMMER_SWI_802ef2f4, 0x00000000, 0x00000031, 0x0002005B, (u32)evt_snd_sfxoff, 0xFE363C8F,
    0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_BTL_MARIO_HAMMER_802ef544, 0xF1194D80, 0x00000000,
    0xF1194D80, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, 0xFE363C82,
    0x00010009, 0x00000007, 0x0001005F, 0xFE363C8E, 0x0003005B, (u32)evt_btl_camera_set_mode,
    0x00000000, 0x00000001, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001,
    0x0003005B, (u32)evt_btl_camera_set_zoom, 0x00000000, 0x00000000, 0x0002005B, (u32)evt_btl_camera_off_posoffset_manual,
    0x00000000, 0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_BTL_MARIO_HAMMER_802ef57c, 0xF1194D80,
    0x00000000, 0xF1194D80, 0x0002005D, (u32)marioAttackEvent_FireNaguri_object, 0xFE363C8F, 0x0006005B,
    (u32)btlevtcmd_ACRStart, 0xFFFFFFFE, 0x00000000, 0x0000000F, 0x0000000F, 0x00000000,
    0x0003005B, (u32)btlevtcmd_ACRGetResult, 0xFE363C86, 0xFE363C87, 0x00010022, 0xFE363C86,
    0x00010029, 0x00000002, 0x00010005, 0x00000014, 0x0005005B, (u32)btlevtcmd_AddRotate,
    0xFFFFFFFE, 0x00000000, 0x00000024, 0x00000000, 0x00010009, 0x00000001,
    0x00000006, 0x0005005B, (u32)btlevtcmd_SetRotate, 0xFFFFFFFE, 0x00000000, 0x00000000,
    0x00000000, 0x0003005B, (u32)btlevtcmd_snd_voice, 0xFFFFFFFE, 0x00000001, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_V_2_80422380, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress,
    0xFFFFFFFE, 0xFE363C8C, 0x0006005B, (u32)btlevtcmd_AudienceDeclareAcrobatResult, 0xFE363C8C, 0x00000001,
    0x00000000, 0x00000000, 0x00000000, 0x0001000A, 0x000001F4, 0x0000002A,
    0x0001005B, (u32)evt_audience_acrobat_notry, 0x00010009, 0x0000003C, 0x0004005B, (u32)btlevtcmd_AnimeChangePose,
    0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_S_1_80422338, 0x00000031, 0x0002005B, (u32)btlevtcmd_WaitEventEnd,
    0xFE363C8F, 0x0001005B, (u32)evt_audience_ap_recovery, 0x0001005B, (u32)btlevtcmd_InviteApInfoReport, 0x0003005B,
    (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000,
    0x00000001, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_W_1_80422320,
    0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE, 0xF24A9280, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType,
    0xFFFFFFFE, 0x00000001, 0x00000028, 0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000, 0xFFFFFFFF, 0x00000000,
    0x00010003, 0x00000063, 0x0002005B, (u32)btlevtcmd_ResetFaceDirection, 0xFFFFFFFE, 0x0001005E,
    (u32)btldefaultevt_SuitoruBadgeEffect, 0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002, 0x00000001,
};

/* .data + 0x0000F1D0 */
u32 marioAttackEvent_FireNaguri_object[] = {
    0x0003005B, (u32)btlevtcmd_GetSelectEnemy, 0xFE363C83, 0xFE363C84, 0x00020018, 0xFE363C83,
    0xFFFFFFFF, 0x0001005B, (u32)btlevtcmd_StopAC, 0x0006005B, (u32)btlevtcmd_GetPartsPos, 0xFFFFFFFE,
    0x00000003, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B, (u32)btlevtcmd_GetStageSize,
    0xFE363C85, 0xF1194D80, 0xF1194D80, 0x00020037, 0xFE363C85, 0x00000002,
    0x0004005B, (u32)btlevtcmd_FaceDirectionAdd, 0xFFFFFFFE, 0xFE363C80, 0xFE363C85, 0x0006005B,
    (u32)btlevtcmd_JumpPartsSetting, 0xFFFFFFFE, 0x00000003, 0x00000000, 0xF24A9280, 0xF24A7BB3,
    0x0007005B, (u32)btlevtcmd_FallPartsPosition, 0xFFFFFFFE, 0x00000003, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000000, 0x0005005B, (u32)btlevtcmd_SetPartsWork, 0xFFFFFFFE, 0x00000003,
    0x00000000, 0x00000000, 0x00000002, 0x00000021, 0x0006005B, (u32)btlevtcmd_GetHitPos,
    0xFE363C83, 0xFE363C84, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B,
    (u32)btlevtcmd_FaceDirectionAdd, 0xFE363C83, 0xFE363C80, 0x00000014, 0x0004005B, (u32)btlevtcmd_TransStageFloorPosition,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0006005B, (u32)btlevtcmd_JumpPartsSetting, 0xFFFFFFFE,
    0x00000003, 0x00000000, 0xF24A8A80, 0xF24A7C80, 0x0008005B, (u32)btlevtcmd_JumpPartsPosition,
    0xFFFFFFFE, 0x00000003, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000,
    0xFFFFFFFF, 0x0005005B, (u32)btlevtcmd_SetPartsWork, 0xFFFFFFFE, 0x00000003, 0x00000000,
    0x00000000, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress, 0xFFFFFFFE, 0xFE363C80, 0x0002005B,
    (u32)btlevtcmd_WeaponAftereffect, 0xFE363C80, 0x0002005B, (u32)btlevtcmd_GetResultAC, 0xFE363C80, 0x0002001E,
    0xFE363C80, 0x00000002, 0x0004005B, (u32)btlevtcmd_GetResultPrizeLv, 0xFFFFFFFB, 0x00000000,
    0xFE363C86, 0x0005005B, (u32)btlevtcmd_ACSuccessEffect, 0xFE363C86, 0x00000000, 0x00000032,
    0x00000000, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress, 0xFFFFFFFE, 0xFE363C80, 0x0003005B,
    (u32)btlevtcmd_AudienceDeclareACResult, 0xFE363C80, 0xFE363C86, 0x00000020, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress,
    0xFFFFFFFE, 0xFE363C80, 0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult, 0xFE363C80, 0xFFFFFFFF,
    0x00000021, 0x0006005B, (u32)btlevtcmd_GetPartsPos, 0xFFFFFFFE, 0x00000003, 0xFE363C87,
    0xFE363C88, 0xFE363C89, 0x0007005B, (u32)btlevtcmd_CalculateFaceDirection, 0xFFFFFFFE, 0xFFFFFFFF,
    0xFFFFFFFF, 0xFFFFFFFF, 0x00000002, 0xFE363C8C, 0x00020037, 0xFE363C8C,
    0x00000010, 0x00020032, 0xFE363C8D, 0x00000005, 0x0003005B, (u32)btlevtcmd_GetSelectEnemy,
    0xFE363C83, 0xFE363C84, 0x00020018, 0xFE363C83, 0xFFFFFFFF, 0x0001005B,
    (u32)btlevtcmd_StopAC, 0x00010004, 0x0000005A, 0x00000021, 0x00010004, 0x00000033,
    0x00010003, 0x00000032, 0x0003005B, (u32)btlevtcmd_GetSelectNextEnemy, 0xFE363C83, 0xFE363C84,
    0x00020018, 0xFE363C83, 0xFFFFFFFF, 0x0001005B, (u32)btlevtcmd_StopAC, 0x00010004,
    0x0000005A, 0x00000021, 0x00010003, 0x00000033, 0x0006005B, (u32)btlevtcmd_GetHitPos,
    0xFE363C83, 0xFE363C84, 0xFE363C8B, 0xF1194D80, 0xF1194D80, 0x0007005B,
    (u32)btlevtcmd_CalculateFaceDirection, 0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000002,
    0xFE363C8E, 0x0002001D, 0xFE363C8E, 0x00000000, 0x0002001C, 0xFE363C87,
    0xFE363C8B, 0x00010009, 0x00000006, 0x00020035, 0xFE363C87, 0xFE363C8C,
    0x0000006B, 0x0005005B, (u32)_fire_wave, 0xFE363C87, 0xFE363C88, 0xFE363C89,
    0xFE363C8D, 0x0000006D, 0x00010004, 0x00000033, 0x00000021, 0x00000020,
    0x0002001D, 0xFE363C87, 0xFE363C8B, 0x00010009, 0x00000006, 0x00020035,
    0xFE363C87, 0xFE363C8C, 0x0000006B, 0x0005005B, (u32)_fire_wave, 0xFE363C87,
    0xFE363C88, 0xFE363C89, 0xFE363C8D, 0x0000006D, 0x00010004, 0x00000033,
    0x00000021, 0x00000021, 0x0006005B, (u32)btlevtcmd_CommandPreCheckDamage, 0xFFFFFFFE, 0xFE363C83,
    0xFE363C84, 0x00000100, 0xFE363C86, 0x00020018, 0xFE363C86, 0x00000001,
    0x0002005B, (u32)btlevtcmd_GetResultAC, 0xFE363C80, 0x0002001E, 0xFE363C80, 0x00000002,
    0x0006005B, (u32)btlevtcmd_CommandCheckDamage, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0x00020100,
    0xFE363C85, 0x00000020, 0x0006005B, (u32)btlevtcmd_CommandCheckDamage, 0xFFFFFFFE, 0xFE363C83,
    0xFE363C84, 0x00000100, 0xFE363C85, 0x00000021, 0x0002001D, 0xFE363C8D,
    0x00000002, 0x00020036, 0xFE363C8D, 0x00000001, 0x00000021, 0x00000020,
    0x00020018, 0xFE363C86, 0x00000003, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83,
    0x00000026, 0x00000021, 0x00020018, 0xFE363C86, 0x00000006, 0x0003005B,
    (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000027, 0x00000021, 0x00020018, 0xFE363C86,
    0x00000002, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000028, 0x00000021,
    0x00000021, 0x00010004, 0x00000032, 0x00010003, 0x0000005A, 0x00010003,
    0x0000005C, 0x0007005B, (u32)btlevtcmd_CalculateFaceDirection, 0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF,
    0xFFFFFFFF, 0x00000002, 0xFE363C8A, 0x0004005B, (u32)btlevtcmd_GetStageSize, 0xFE363C8B,
    0xF1194D80, 0xF1194D80, 0x00020037, 0xFE363C8B, 0xFE363C8A, 0x0002001D,
    0xFE363C8B, 0x00000000, 0x0002001C, 0xFE363C87, 0xFE363C8B, 0x00010009,
    0x00000006, 0x00020035, 0xFE363C87, 0xFE363C8C, 0x0000006B, 0x0005005B,
    (u32)_fire_wave, 0xFE363C87, 0xFE363C88, 0xFE363C89, 0xFE363C8D, 0x0000006D,
    0x00010004, 0x0000005C, 0x00000021, 0x00000020, 0x0002001D, 0xFE363C87,
    0xFE363C8B, 0x00010009, 0x00000006, 0x00020035, 0xFE363C87, 0xFE363C8C,
    0x0000006B, 0x0005005B, (u32)_fire_wave, 0xFE363C87, 0xFE363C88, 0xFE363C89,
    0xFE363C8D, 0x0000006D, 0x00010004, 0x0000005C, 0x00000021, 0x00000021,
    0x00000002, 0x00000001,
};

/* .data + 0x0000F7A8 */
u32 _NormalCharge_core[] = {
    0x0003005B, (u32)btlevtcmd_GetUnitId, 0xFFFFFFFE, 0xFE363C83, 0x0003005B, (u32)btlevtcmd_GetBodyId,
    0xFFFFFFFE, 0xFE363C84, 0x00020018, 0xFE363C83, 0xFFFFFFFF, 0x00010004,
    0x00000063, 0x00000021, 0x0003005B, (u32)btlevtcmd_PayWeaponCost, 0xFFFFFFFE, 0xFE363C8C,
    0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFFFFFFFE, 0x00000007, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType,
    0xFFFFFFFE, 0x00000001, 0x0000002A, 0x00010009, 0x0000002D, 0x00010005,
    0x0000001E, 0x0005005B, (u32)btlevtcmd_AddRotate, 0xFFFFFFFE, 0x00000000, 0x00000018,
    0x00000000, 0x00010009, 0x00000001, 0x00000006, 0x0005005B, (u32)btlevtcmd_SetRotate,
    0xFFFFFFFE, 0x00000000, 0x00000000, 0x00000000, 0x0002005B, (u32)btlevtcmd_WeaponAftereffect,
    0xFE363C8C, 0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_MARIO_CHARGE1_802ef598, 0x00000000, 0x0005005B,
    (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0003005B,
    (u32)btlevtcmd_GetHeight, 0xFFFFFFFE, 0xFE363C85, 0x00020038, 0xFE363C85, 0x00000002,
    0x00020035, 0xFE363C81, 0xFE363C85, 0x00020035, 0xFE363C82, 0x0000000A,
    0x000F005B, (u32)evt_eff64, (u32)(char*)str_noname_802ef5ac, (u32)(char*)str_crystal_n64_802ef5b4, 0x00000007, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0xF24A8080, 0x0000003C, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00010009, 0x0000003C,
    0x0007005B, (u32)btlevtcmd_CheckDamage, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0xFE363C8C,
    0x00000100, 0xFE363C85, 0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002,
    0x00000001,
};

/* .data + 0x0000F944 */
u32 _SuperCharge_core[] = {
    0x0003005B, (u32)btlevtcmd_GetUnitId, 0xFFFFFFFE, 0xFE363C83, 0x0003005B, (u32)btlevtcmd_GetBodyId,
    0xFFFFFFFE, 0xFE363C84, 0x00020018, 0xFE363C83, 0xFFFFFFFF, 0x00010004,
    0x00000063, 0x00000021, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFFFFFFFE, 0x00000007,
    0x0003005B, (u32)btlevtcmd_PayWeaponCost, 0xFFFFFFFE, 0xFE363C8C, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType,
    0xFFFFFFFE, 0x00000001, 0x0000002A, 0x00010009, 0x0000002D, 0x00010005,
    0x0000001E, 0x0005005B, (u32)btlevtcmd_AddRotate, 0xFFFFFFFE, 0x00000000, 0x00000018,
    0x00000000, 0x00010009, 0x00000001, 0x00000006, 0x0005005B, (u32)btlevtcmd_SetRotate,
    0xFFFFFFFE, 0x00000000, 0x00000000, 0x00000000, 0x0002005B, (u32)btlevtcmd_WeaponAftereffect,
    0xFE363C8C, 0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_MARIO_SUPER_CHAR_802ef5c0, 0x00000000, 0x0005005B,
    (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0003005B,
    (u32)btlevtcmd_GetHeight, 0xFFFFFFFE, 0xFE363C85, 0x00020038, 0xFE363C85, 0x00000002,
    0x00020035, 0xFE363C81, 0xFE363C85, 0x00020035, 0xFE363C82, 0x0000000A,
    0x000F005B, (u32)evt_eff64, (u32)(char*)str_noname_802ef5ac, (u32)(char*)str_crystal_n64_802ef5b4, 0x00000007, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0xF24A8080, 0x0000003C, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00010009, 0x0000003C,
    0x0007005B, (u32)btlevtcmd_CheckDamage, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0xFE363C8C,
    0x00000100, 0xFE363C85, 0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002,
    0x00000001,
};

/* .data + 0x0000FAE0 */
u32 marioAttackEvent_MajinaiPowerUpCheck[] = {
    0x0002005B, (u32)_majinai_powerup_check, 0xFE363C80, 0x00020018, 0xFE363C80, 0x00000000,
    0x00000002, 0x00000021, 0x00020032, 0xFE363C80, 0x00000000, 0x00020032,
    0xFE363C81, 0x00000000, 0x00020032, 0xFE363C82, 0x00000000, 0x0001005C,
    (u32)evt_majinai_effect_se_btl, 0x0002005B, (u32)_battle_majinai_effect, 0xFFFFFFFD, 0x0001005E, (u32)marioAttackEvent_MajinaiJump,
    0x0006005B, (u32)btlevtcmd_AnnounceMessage, 0x00000002, 0x00000000, 0x00000000, (u32)(char*)str_btl_msg_mj_power_up_802ef5e4,
    0x000000B4, 0x00010009, 0x0000003C, 0x00000002, 0x00000001,
};

/* .data + 0x0000FB6C */
u32 marioAttackEvent_MajinaiJump[] = {
    0x0003005B, (u32)btlevtcmd_CheckActStatus, 0xFFFFFFFE, 0xFE363C80, 0x00020018, 0xFE363C80,
    0x00000000, 0x00000002, 0x00000021, 0x0003005B, (u32)btlevtcmd_SetFallAccel, 0xFFFFFFFE,
    0xF24A7C80, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0003005B, (u32)btlevtcmd_GetBodyId, 0xFFFFFFFE, 0xFE363C86, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0xFE363C86, 0x0000002C, 0x0007005B, (u32)btlevtcmd_JumpPosition,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0000001E, 0xFFFFFFFF,
    0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0xFE363C86, 0x00000023, 0x00000002,
    0x00000001,
};

/* .data + 0x0000FC18 */
u32 _MajinaiPoseReset[] = {
    0x0003005B, (u32)btlevtcmd_CheckActStatus, 0xFFFFFFFE, 0xFE363C80, 0x00020018, 0xFE363C80,
    0x00000000, 0x00000002, 0x00000021, 0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE,
    0x00000002, 0x00000001,
};

/* .data + 0x0000FC50 */
u32 marioAttackEvent_MajinaiDefenceUp[] = {
    0x00020032, 0xFE363C80, 0x00000000, 0x00020032, 0xFE363C81, 0x00000000,
    0x00020032, 0xFE363C82, 0x00000000, 0x0001005C, (u32)evt_majinai_effect_se_btl, 0x0002005B,
    (u32)_battle_majinai_effect, 0xFFFFFFFD, 0x0001005E, (u32)marioAttackEvent_MajinaiJump, 0x0006005B, (u32)btlevtcmd_AnnounceMessage,
    0x00000002, 0x00000000, 0x00000000, (u32)(char*)str_btl_msg_mj_defence_u_802ef5f8, 0x000000B4, 0x00010009,
    0x0000003C, 0x0001005E, (u32)_MajinaiPoseReset, 0x00000002, 0x00000001,
};

/* .data + 0x0000FCC4 */
u32 marioAttackEvent_MajinaiExpUp[] = {
    0x00020032, 0xFE363C80, 0x00000000, 0x00020032, 0xFE363C81, 0x00000000,
    0x00020032, 0xFE363C82, 0x00000000, 0x0001005C, (u32)evt_majinai_effect_se_btl, 0x0002005B,
    (u32)_battle_majinai_effect, 0xFFFFFFFD, 0x0001005E, (u32)marioAttackEvent_MajinaiJump, 0x0006005B, (u32)btlevtcmd_AnnounceMessage,
    0x00000002, 0x00000000, 0x00000000, (u32)(char*)str_btl_msg_mj_exp_up_802ef610, 0x000000B4, 0x00010009,
    0x0000003C, 0x00000002, 0x00000001,
};

/* .sdata + 0x00000000 */
u32 marioAttackEvent_GatsunJump[] = {
    0x00020032, 0xFE363C8C, (u32)badgeWeapon_GatsunJump, 0x0001005E, (u32)marioAttackEvent_GatsuDokaJump, 0x00000002,
    0x00000001,
};

/* .sdata + 0x0000001C */
u32 marioAttackEvent_DokanJump[] = {
    0x00020032, 0xFE363C8C, (u32)badgeWeapon_DokanJump, 0x0001005E, (u32)marioAttackEvent_GatsuDokaJump, 0x00000002,
    0x00000001,
};

/* .sdata + 0x00000038 */
u32 marioAttackEvent_MiniminiFumi[] = {
    0x0001005E, (u32)marioAttackEvent_NormalJump, 0x00000002, 0x00000001,
};

/* .sdata + 0x00000048 */
u32 marioAttackEvent_NemuraseFumi[] = {
    0x0001005E, (u32)marioAttackEvent_NormalJump, 0x00000002, 0x00000001,
};

/* .sdata + 0x00000058 */
u32 marioAttackEvent_FunyafunyaJump[] = {
    0x0001005E, (u32)marioAttackEvent_NormalJump, 0x00000002, 0x00000001,
};

/* .sdata + 0x00000068 */
u32 marioAttackEvent_NormalHammer[] = {
    0x00020032, 0xFE363C8E, 0x00000000, 0x0001005E, (u32)marioAttackEvent_NormalHammer_Core, 0x00000002,
    0x00000001,
};

/* .sdata + 0x00000084 */
u32 marioAttackEvent_GatsunHammer[] = {
    0x00020032, 0xFE363C8E, 0x00000001, 0x0001005E, (u32)marioAttackEvent_NormalHammer_Core, 0x00000002,
    0x00000001,
};

/* .sdata + 0x000000A0 */
u32 marioAttackEvent_IceNaguri[] = {
    0x00020032, 0xFE363C8E, 0x00000000, 0x0001005E, (u32)marioAttackEvent_NormalHammer_Core, 0x00000002,
    0x00000001,
};

/* .sdata + 0x000000BC */
u32 marioAttackEvent_TsuranukiHammer[] = {
    0x00020032, 0xFE363C8E, 0x00000001, 0x0001005E, (u32)marioAttackEvent_NormalHammer_Core, 0x00000002,
    0x00000001,
};

/* .sdata + 0x000000D8 */
u32 marioAttackEvent_NormalCharge[] = {
    0x00020032, 0xFE363C8C, (u32)badgeWeapon_Charge, 0x0001005E, (u32)_NormalCharge_core, 0x00000002,
    0x00000001,
};

/* .sdata + 0x000000F4 */
u32 marioAttackEvent_NormalChargeP[] = {
    0x00020032, 0xFE363C8C, (u32)badgeWeapon_ChargeP, 0x0001005E, (u32)_NormalCharge_core, 0x00000002,
    0x00000001,
};

/* .sdata + 0x00000110 */
u32 marioAttackEvent_SuperCharge[] = {
    0x00020032, 0xFE363C8C, (u32)badgeWeapon_SuperCharge, 0x0001005E, (u32)_SuperCharge_core, 0x00000002,
    0x00000001,
};

/* .sdata + 0x0000012C */
u32 marioAttackEvent_SuperChargeP[] = {
    0x00020032, 0xFE363C8C, (u32)badgeWeapon_SuperChargeP, 0x0001005E, (u32)_SuperCharge_core, 0x00000002,
    0x00000001,
};
