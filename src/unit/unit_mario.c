#include "unit/unit_mario.h"

void mapSetTevCallback(s32 type, void* callback);
void callback(void*);
s32 evtSetValue(void* event, s32 target, s32 value);
void* pouchGetPtr(void);
s32 BattleTransID(void* event, s32 type);
void* BattleGetUnitPartsPtr(s32 unitId, s32 partId);
void* animPoseGetAnimPosePtr(s32 poseId);
void BtlUnit_snd_se(void* unit, const char* name, u32 flags, s32 zero);

extern const char str_SFX_VOICE_MARIO_RELI_802f4250[];
extern const char str_SFX_VOICE_MARIO_RELI_802f4268[];
extern void* _battleWorkPointer;
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 _get_local_frame(void* event) {
    s32 arg0 = **(s32**)((s32)event + 0x18);
    void* parts = BattleGetUnitPartsPtr(BattleTransID(event, -2), 1);
    void* pose = animPoseGetAnimPosePtr(*(s32*)((s32)parts + 0x1C0));
    evtSetValue(event, arg0, (s32)*(f32*)((s32)pose + 0x24));
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on
void mario_pinch_pose_sound_callback(void* unit) {
    if (*(s32*)((s32)unit + 0x238) & 1) {
        BtlUnit_snd_se(unit, str_SFX_VOICE_MARIO_RELI_802f4250, 0xF1194D80, 0);
    } else {
        BtlUnit_snd_se(unit, str_SFX_VOICE_MARIO_RELI_802f4268, 0xF1194D80, 0);
    }
    *(s32*)((s32)unit + 0x238) = *(s32*)((s32)unit + 0x238) + 1;
}

void callback(void* tev) {
    extern u32 dat_80422f68;
    extern void GXSetTevColor(s32 id, void* color);
    extern void GXSetTevOrder(s32 stage, s32 texCoord, s32 texMap, s32 colorChan);
    extern void GXSetTevColorOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevAlphaOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevColorIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevAlphaIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevSwapMode(s32 stage, s32 rasSel, s32 texSel);
    u32 color;
    s32 stage;
    s32 texCoord;
    s32 texMap;
    s32 colorChan;

    color = dat_80422f68;
    stage = *(s32*)((s32)tev + 0xC);
    texCoord = *(s32*)((s32)tev + 0x10);
    texMap = *(s32*)((s32)tev + 0x14);
    colorChan = *(s32*)((s32)tev + 0x18);
    GXSetTevColor(1, &color);
    GXSetTevOrder(stage, 0xFF, 0xFF, 0xFF);
    GXSetTevColorOp(stage, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(stage, 0, 0, 0, 1, 0);
    GXSetTevColorIn(stage, 0xF, 0xF, 0xF, 2);
    GXSetTevAlphaIn(stage, 7, 7, 7, 0);
    GXSetTevSwapMode(stage, 0, 0);
    *(s32*)((s32)tev + 0xC) = stage + 1;
    *(s32*)((s32)tev + 0x14) = texMap;
    *(s32*)((s32)tev + 0x18) = colorChan;
    *(s32*)((s32)tev + 0x10) = texCoord;
}

void faker_mario_makkuro_set(void) {
    mapSetTevCallback(0xB, callback);
}
s32 _mario_makkuro_set(void) {
    extern void* BattleGetMarioPtr(void* battleWork);
    extern void* BtlUnit_GetPartsPtr(void* unit, s32 partId);
    extern void animPoseSetMaterialFlagOn(s32 poseId, u32 flags);
    void* mario = BattleGetMarioPtr(_battleWorkPointer);

    if (mario != 0) {
        mapSetTevCallback(0xB, callback);
        animPoseSetMaterialFlagOn(*(s32*)((s32)BtlUnit_GetPartsPtr(mario, 1) + 0x1C0), 0x0B000000);
        animPoseSetMaterialFlagOn(*(s32*)((s32)BtlUnit_GetPartsPtr(mario, 2) + 0x1C0), 0x0B000000);
        animPoseSetMaterialFlagOn(*(s32*)((s32)BtlUnit_GetPartsPtr(mario, 3) + 0x1C0), 0x0B000000);
    }
    return 2;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 _mario_super_emblem_anim_set(void* event, s32 releaseOld) {
    extern s32 evtGetValue(void* event, s32 value);
    extern s32 BattleTransID(void* event, s32 id);
    extern void* BattleGetUnitPtr(void* battleWork, s32 id);
    extern void* BattleGetUnitPartsPtr(s32 unitId, s32 partId);
    extern void* animPoseGetAnimPosePtr(s32 poseId);
    extern void animPoseRelease(s32 poseId);
    extern s32 animEffectAsync(char* name, s32 battle);
    extern s32 animPoseEntry(char* name, s32 battle);
    extern s32 pouchEquipCheckBadge(s32 badge);
    extern const char str_M_Z_10_802f3f70[];
    char* base;
    s32 unitId;
    char* poseName;
    void* part;
    void* battleWork;
    s32 badgeFlags;
    s32 id;

    base = (char*)str_M_Z_10_802f3f70;
    battleWork = _battleWorkPointer;
    id = evtGetValue(event, **(s32**)((s32)event + 0x18));
    unitId = BattleTransID(event, id);
    BattleGetUnitPtr(battleWork, unitId);

    badgeFlags = 0;
    if (pouchEquipCheckBadge(0x147) != 0) {
        badgeFlags = 1;
    }
    if (pouchEquipCheckBadge(0x148) != 0) {
        badgeFlags += 2;
    }

    switch (badgeFlags) {
        case 1:
            poseName = base + 0x2BC;
            break;
        case 2:
            poseName = base + 0x2C8;
            break;
        case 3:
            poseName = base + 0x2D4;
            break;
        default:
            poseName = 0;
            break;
    }

    if (releaseOld != 0) {
        if (poseName == 0) {
            return 2;
        }
        part = BattleGetUnitPartsPtr(unitId, 1);
        animPoseGetAnimPosePtr(*(s32*)((s32)part + 0x1C0));
        if (*(s32*)((s32)part + 0x1C0) != -1) {
            animPoseRelease(*(s32*)((s32)part + 0x1C0));
            *(s32*)((s32)part + 0x1C0) = -1;
        }
        part = BattleGetUnitPartsPtr(unitId, 2);
        animPoseGetAnimPosePtr(*(s32*)((s32)part + 0x1C0));
        if (*(s32*)((s32)part + 0x1C0) != -1) {
            animPoseRelease(*(s32*)((s32)part + 0x1C0));
            *(s32*)((s32)part + 0x1C0) = -1;
        }
        part = BattleGetUnitPartsPtr(unitId, 3);
        animPoseGetAnimPosePtr(*(s32*)((s32)part + 0x1C0));
        if (*(s32*)((s32)part + 0x1C0) != -1) {
            animPoseRelease(*(s32*)((s32)part + 0x1C0));
            *(s32*)((s32)part + 0x1C0) = -1;
        }
    }

    if (animEffectAsync(poseName, 1) != 0) {
        part = BattleGetUnitPartsPtr(unitId, 1);
        *(s32*)((s32)part + 0x1C0) = animPoseEntry(poseName, 1);
        part = BattleGetUnitPartsPtr(unitId, 2);
        *(s32*)((s32)part + 0x1C0) = animPoseEntry(poseName, 1);
        part = BattleGetUnitPartsPtr(unitId, 3);
        *(s32*)((s32)part + 0x1C0) = animPoseEntry(poseName, 1);
        return 2;
    }
    return 0;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on



#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 _get_mario_hammer_lv(void* event) {
    s32 arg0 = **(s32**)((s32)event + 0x18);
    evtSetValue(event, arg0, *(s8*)((s32)pouchGetPtr() + 0x99));
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

/* Recovered unit_mario .rodata strings. */
const char str_M_Z_10_802f3f70[] = "M_Z_10";
const char str_M_Z_11_802f3f78[] = "M_Z_11";
const char str_M_J_1B_802f3f80[] = "M_J_1B";
const char str_M_A_3B_802f3f88[] = "M_A_3B";
const char str_KRN_H_1_802f3f90[] = "KRN_H_1";
const char str_btl_un_mario_802f3f98[] = "btl_un_mario";
const char str_b_mario_802f3fa8[] = "b_mario";
const char str_SFX_MARIO_DAMAGE1_802f3fb0[] = "SFX_MARIO_DAMAGE1";
const char str_SFX_MARIO_DAMAGE_FIR_802f3fc4[] = "SFX_MARIO_DAMAGE_FIRE1";
const char str_SFX_MARIO_DAMAGE_ICE_802f3fdc[] = "SFX_MARIO_DAMAGE_ICE1";
const char str_SFX_MARIO_DAMAGE_BIR_802f3ff4[] = "SFX_MARIO_DAMAGE_BIRIBIRI1";
const char str_dontyo_802f4010[] = "dontyo";
const char str_SFX_MARIO_BATTLE_JUM_802f4018[] = "SFX_MARIO_BATTLE_JUMP1";
const char str_SFX_MARIO_JUMP_LANDI_802f4030[] = "SFX_MARIO_JUMP_LANDING1";
const char str_SFX_MARIO_BATTLE_MOV_802f4048[] = "SFX_MARIO_BATTLE_MOVE1L";
const char str_SFX_MARIO_BATTLE_MOV_802f4060[] = "SFX_MARIO_BATTLE_MOVE1R";
const char str_SFX_VOICE_MARIO_DOWN_802f4078[] = "SFX_VOICE_MARIO_DOWN1";
const char str_SFX_CONDITION_DOWN1_802f4090[] = "SFX_CONDITION_DOWN1";
const char str_recovery_802f40a4[] = "recovery";
const char str_SFX_MARIO_APPEAL1_802f40b0[] = "SFX_MARIO_APPEAL1";
const char str_SFX_VOICE_MARIO_BATT_802f40c4[] = "SFX_VOICE_MARIO_BATTLE_JUMP1";
const char str_M_J_1D_802f40e4[] = "M_J_1D";
const char str_M_H_5A_802f40ec[] = "M_H_5A";
const char str_M_H_5B_802f40f4[] = "M_H_5B";
const char str_M_H_14_802f40fc[] = "M_H_14";
const char str_M_H_9A_802f4104[] = "M_H_9A";
const char str_M_H_9B_802f410c[] = "M_H_9B";
const char str_M_H_15_802f4114[] = "M_H_15";
const char str_M_H_1A_802f411c[] = "M_H_1A";
const char str_M_H_1B_802f4124[] = "M_H_1B";
const char str_M_H_13_802f412c[] = "M_H_13";
const char str_SFX_MARIO_HAMMER_HOL_802f4134[] = "SFX_MARIO_HAMMER_HOLD2";
const char str_SFX_MARIO_HAMMER_HOL_802f414c[] = "SFX_MARIO_HAMMER_HOLD1";
const char str_SFX_MARIO_HAMMER_HOL_802f4164[] = "SFX_MARIO_HAMMER_HOLD3";
const char str_SFX_MARIO_HAMMER_SWI_802f417c[] = "SFX_MARIO_HAMMER_SWING2";
const char str_SFX_MARIO_HAMMER_SWI_802f4194[] = "SFX_MARIO_HAMMER_SWING1";
const char str_SFX_MARIO_HAMMER_SWI_802f41ac[] = "SFX_MARIO_HAMMER_SWING3";
const char str_SFX_MARIO_HAMMER_HIT_802f41c4[] = "SFX_MARIO_HAMMER_HIT_GROUND3";
const char str_SFX_MARIO_HAMMER_HIT_802f41e4[] = "SFX_MARIO_HAMMER_HIT_GROUND2";
const char str_SFX_MARIO_HAMMER_HIT_802f4204[] = "SFX_MARIO_HAMMER_HIT_GROUND1";
const char str_M_H_16_802f4224[] = "M_H_16";
const char str_b_mario_l_802f422c[] = "b_mario_l";
const char str_b_mario_w_802f4238[] = "b_mario_w";
const char str_b_mario_wl_802f4244[] = "b_mario_wl";
const char str_SFX_VOICE_MARIO_RELI_802f4250[] = "SFX_VOICE_MARIO_RELIEF2";
const char str_SFX_VOICE_MARIO_RELI_802f4268[] = "SFX_VOICE_MARIO_RELIEF1";

/* Recovered safe prefix of unit_mario .sdata2 pose literals. */
const char str_M_S_3_80422ed8[] = "M_S_3";
const char str_M_N_5_80422ee0[] = "M_N_5";
const char str_M_X_1_80422ee8[] = "M_X_1";
const char str_M_S_1_80422ef0[] = "M_S_1";
const char str_M_V_2_80422ef8[] = "M_V_2";
const char str_M_B_2_80422f00[] = "M_B_2";
const char str_M_D_6_80422f08[] = "M_D_6";
const char str_M_B_1_80422f10[] = "M_B_1";
const char str_M_V_1_80422f18[] = "M_V_1";
const char str_M_N_7_80422f20[] = "M_N_7";
const char str_M_D_1_80422f28[] = "M_D_1";
const char str_M_W_1_80422f30[] = "M_W_1";
const char str_M_R_1_80422f38[] = "M_R_1";
const char str_M_I_1_80422f40[] = "M_I_1";
const char str_M_D_2_80422f48[] = "M_D_2";
const char str_M_C_3_80422f50[] = "M_C_3";
const char str_M_N_8_80422f58[] = "M_N_8";
const char str_M_I_O_80422f60[] = "M_I_O";

/* Forward declarations for recovered unit_mario event bytecode. */
static u32 battle_entry_event[73];
extern u32 btlataudevtMarioHammer[];
extern u32 btlataudevtMarioJump[];
static u32 attack_event[2];
extern u32 btldefaultevt_Confuse[];
static u32 damage_event[10];
extern u32 pose_sound_timing_table[10];
static u32 wait_event[6];
extern const char str_M_I_S_80422f6c[];
extern s32 _attack_aud();
extern s32 _get_attack_aud_no();
extern s32 btlevtcmd_AnimeChangePose();
extern s32 btlevtcmd_AnimeChangePoseType();
extern s32 btlevtcmd_AnimeSetPoseSoundTable();
extern s32 btlevtcmd_AnimeWaitPlayComplete();
extern s32 btlevtcmd_AudienceAppeal();
extern s32 btlevtcmd_ClearAllStatus();
extern s32 btlevtcmd_ConsumeItemReserve();
extern s32 btlevtcmd_DispItemIcon();
extern s32 btlevtcmd_FaceDirectionAdd();
extern s32 btlevtcmd_FaceDirectionSub();
extern s32 btlevtcmd_GetFaceDirection();
extern s32 btlevtcmd_GetHomePos();
extern s32 btlevtcmd_GetPos();
extern s32 btlevtcmd_GetStageAudienceAreaSize();
extern s32 btlevtcmd_GetStageSize();
extern s32 btlevtcmd_JumpPosition();
extern s32 btlevtcmd_JumpSetting();
extern s32 btlevtcmd_KillUnit();
extern s32 btlevtcmd_MarioJumpParam();
extern s32 btlevtcmd_MarioJumpPosition();
extern s32 btlevtcmd_MovePosition();
extern s32 btlevtcmd_OnOffStatus();
extern s32 btlevtcmd_RecoverHp();
extern s32 btlevtcmd_SetEventAttack();
extern s32 btlevtcmd_SetEventConfusion();
extern s32 btlevtcmd_SetEventDamage();
extern s32 btlevtcmd_SetEventEntry();
extern s32 btlevtcmd_SetEventWait();
extern s32 btlevtcmd_SetFallAccel();
extern s32 btlevtcmd_SetHp();
extern s32 btlevtcmd_SetJumpSound();
extern s32 btlevtcmd_SetMoveSpeed();
extern s32 btlevtcmd_SetPartsWork();
extern s32 btlevtcmd_SetPos();
extern s32 btlevtcmd_SetRunSound();
extern s32 btlevtcmd_SetUnitWork();
extern s32 btlevtcmd_SetWalkSound();
extern s32 btlevtcmd_StartWaitEvent();
extern s32 btlevtcmd_WaitEventEnd();
extern s32 btlevtcmd_snd_se();
extern s32 evt_btl_camera_nomove_y_onoff();
extern s32 evt_btl_camera_set_homing_unit();
extern s32 evt_btl_camera_set_homing_unit_audience();
extern s32 evt_btl_camera_set_mode();
extern s32 evt_btl_camera_set_moveSpeedLv();
extern s32 evt_btl_camera_set_posoffset();
extern s32 evt_btl_camera_set_prilimit();
extern s32 evt_btl_camera_set_zoom();
extern s32 evt_btl_camera_shake_h();
extern s32 evt_eff();
extern s32 evt_map_replayanim();
extern s32 evt_snd_sfxon();
extern s32 evt_snd_sfxon_3d();
extern s32 evt_sub_random();

extern s32 btldefaultevt_Damage();
extern s32 btlevtcmd_AnimeChangePoseFromTable();

/* Forward declarations for recovered unit_mario structured data. */
static u8 defence[5];
static u8 defence_attr[5];
extern u32 mario_win_reaction[9];
extern u32 pose_table_object[6];
static u8 regist[22];
extern u32 attack_audience_event[];
extern u32 btldefaultevt_Dummy[];
extern u32 btldefaultevt_avoid_counter_event[];
extern u32 init_event[];
extern u32 mario_appeal_event[];
extern u32 mario_dead_event[];
extern u32 mario_emergency_revival_event[];
extern u32 mario_ride_btl_entry_event[];
extern u32 subset_attack_ready_jump_event[];

/* Recovered 0x308-byte unit_mario structured .data prefix. */
u32 pose_table_mario_stay[] = {
    0x00000004, (u32)(char*)str_M_S_3_80422ed8, 0x00000001, (u32)(char*)str_M_N_5_80422ee0, 0x00000002, (u32)(char*)str_M_Z_10_802f3f70,
    0x00000009, (u32)(char*)str_M_Z_10_802f3f70, 0x00000005, (u32)(char*)str_M_X_1_80422ee8, 0x00000003, (u32)(char*)str_M_X_1_80422ee8,
    0x0000001B, (u32)(char*)str_M_Z_11_802f3f78, 0x0000001C, (u32)(char*)str_M_S_1_80422ef0, 0x00000023, (u32)(char*)str_M_V_2_80422ef8,
    0x00000025, (u32)(char*)str_M_B_2_80422f00, 0x00000024, (u32)(char*)str_M_D_6_80422f08, 0x0000001F, (u32)(char*)str_M_B_1_80422f10,
    0x00000020, (u32)(char*)str_M_V_1_80422f18, 0x00000026, (u32)(char*)str_M_N_7_80422f20, 0x00000027, (u32)(char*)str_M_D_1_80422f28,
    0x0000002B, (u32)(char*)str_M_S_1_80422ef0, 0x00000028, (u32)(char*)str_M_W_1_80422f30, 0x0000002A, (u32)(char*)str_M_R_1_80422f38,
    0x0000002C, (u32)(char*)str_M_J_1B_802f3f80, 0x00000034, (u32)(char*)str_M_I_1_80422f40, 0x00000039, (u32)(char*)str_M_S_3_80422ed8,
    0x00000038, (u32)(char*)str_M_S_3_80422ed8, 0x0000003E, (u32)(char*)str_M_V_2_80422ef8, 0x0000003F, (u32)(char*)str_M_V_2_80422ef8,
    0x0000003A, (u32)(char*)str_M_A_3B_802f3f88, 0x0000003B, (u32)(char*)str_M_A_3B_802f3f88, 0x0000003C, (u32)(char*)str_M_D_2_80422f48,
    0x0000003D, (u32)(char*)str_M_V_1_80422f18, 0x00000043, (u32)(char*)str_M_C_3_80422f50, 0x00000042, (u32)(char*)str_M_N_8_80422f58,
    0x00000040, (u32)(char*)str_M_I_O_80422f60, 0x00000031, (u32)(char*)str_M_J_1B_802f3f80, 0x0000002D, (u32)(char*)str_M_B_2_80422f00,
    0x00000045, (u32)(char*)str_M_S_1_80422ef0,
};

u32 data_table_mario[] = {
    0x00000005, (u32)mario_appeal_event, 0x00000006, (u32)attack_audience_event, 0x00000030, (u32)btldefaultevt_Dummy,
    0x00000033, (u32)mario_emergency_revival_event, 0x00000031, (u32)mario_dead_event, 0x00000040, (u32)mario_win_reaction,
    0x00000007, (u32)subset_attack_ready_jump_event, 0x00000041, (u32)mario_ride_btl_entry_event, 0x0000002F, (u32)btldefaultevt_avoid_counter_event,
    0x00000000, 0x00000000,
};

u32 unitpartsdata_Mario[] = {
    0x00000001, (u32)(char*)str_btl_un_mario_802f3f98, (u32)(char*)str_b_mario_802f3fa8, 0x00000000, 0x00000000, 0x00000000,
    0x41800000, 0x42100000, 0x40A00000, 0x00000000, 0x42200000, 0x00000000,
    0x00140024, 0x00FF0000, (u32)defence, (u32)defence_attr, 0x00000009, 0x00000000,
    (u32)pose_table_mario_stay, 0x00000002, (u32)(char*)str_btl_un_mario_802f3f98, (u32)(char*)str_b_mario_802f3fa8, 0x00000000, 0x00000000,
    0x00000000, 0x41800000, 0x42100000, 0x40A00000, 0x00000000, 0x42200000,
    0x00000000, 0x00140024, 0x00FF0000, (u32)defence, (u32)defence_attr, 0x13010000,
    0x00000000, (u32)pose_table_object, 0x00000003, (u32)(char*)str_btl_un_mario_802f3f98, (u32)(char*)str_b_mario_802f3fa8, 0x00000000,
    0x00000000, 0x00000000, 0x41800000, 0x42100000, 0x40A00000, 0x00000000,
    0x42200000, 0x00000000, 0x00140024, 0x00FF0000, (u32)defence, (u32)defence_attr,
    0x13010000, 0x00000000, (u32)pose_table_object,
};

u32 unitdata_Mario[] = {
    0x000000DE, (u32)(char*)str_btl_un_mario_802f3f98, 0x000A0005, 0x05010100, 0x00000064, 0x00090016,
    0x0024000B, 0x00240000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x42200000, 0x00000000, 0x00000000, 0x42200000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x41300000,
    0x00000000, 0x00000000, 0x41300000, 0x41BB3333, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0000FF00, 0x64140000,
    0x00000000, 0x00000000, 0x00000000, (u32)(char*)str_SFX_MARIO_DAMAGE1_802f3fb0, (u32)(char*)str_SFX_MARIO_DAMAGE_FIR_802f3fc4, (u32)(char*)str_SFX_MARIO_DAMAGE_ICE_802f3fdc,
    (u32)(char*)str_SFX_MARIO_DAMAGE_BIR_802f3ff4, 0x02040000, (u32)regist, 0x03000000, (u32)unitpartsdata_Mario, (u32)init_event,
    (u32)data_table_mario,
};

/*
 * Recovered unit_mario battle event bytecode.
 * Exact US target words are preserved; relocation words remain symbolic.
 */
static u32 battle_entry_event[] = {
    0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE, 0xFE363C80, 0x000000FA, 0x0005005B,
    (u32)btlevtcmd_SetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0003005B,
    (u32)evt_map_replayanim, 0x00000000, (u32)(char*)str_dontyo_802f4010, 0x00010009, 0x00000028, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001, 0x0000002A, 0x0005005B, (u32)btlevtcmd_GetHomePos,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0008005B, (u32)btlevtcmd_MovePosition,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0000003D, 0xFFFFFFFF,
    0x00000000, 0x00010003, 0x00000000, 0x0002005B, (u32)_get_local_frame, 0xFE363C80,
    0x00020018, 0xFE363C80, 0x00000000, 0x00010004, 0x00000032, 0x00000021,
    0x00020018, 0xFE363C80, 0x0000000A, 0x00010004, 0x00000032, 0x00000021,
    0x00010009, 0x00000001, 0x00010004, 0x00000000, 0x00010003, 0x00000032,
    0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_S_1_80422ef0, 0x00000002,
    0x00000001,
};

u32 mario_ride_btl_entry_event[] = {
    0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE, 0xFE363C80, 0x000000FA, 0x0005005B,
    (u32)btlevtcmd_SetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0003005B,
    (u32)evt_map_replayanim, 0x00000000, (u32)(char*)str_dontyo_802f4010, 0x00010009, 0x0000003C, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001, 0x0000002A, 0x0005005B, (u32)btlevtcmd_GetHomePos,
    0xFFFFFFFC, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub,
    0xFFFFFFFE, 0xFE363C80, 0x0000001E, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000032, 0xFFFFFFFF, 0x00000000,
    0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001, 0x0000002C, 0x0004005B,
    (u32)btlevtcmd_FaceDirectionAdd, 0xFFFFFFFE, 0xFE363C80, 0x0000003C, 0x0003005B, (u32)btlevtcmd_SetFallAccel,
    0xFFFFFFFE, 0xF24A7C80, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000018, 0xFFFFFFFF, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType,
    0xFFFFFFFE, 0x00000001, 0x0000002A, 0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0000000A, 0xFFFFFFFF, 0x00000000,
    0x00010003, 0x00000000, 0x0002005B, (u32)_get_local_frame, 0xFE363C80, 0x00020018,
    0xFE363C80, 0x00000000, 0x00010004, 0x00000032, 0x00000021, 0x00020018,
    0xFE363C80, 0x0000000A, 0x00010004, 0x00000032, 0x00000021, 0x00010009,
    0x00000001, 0x00010004, 0x00000000, 0x00010003, 0x00000032, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_S_1_80422ef0, 0x00000002, 0x00000001,
};

static u32 init_event[] = {
    0x0003005B, (u32)btlevtcmd_SetEventWait, 0xFFFFFFFE, (u32)wait_event, 0x0003005B, (u32)btlevtcmd_SetEventAttack,
    0xFFFFFFFE, (u32)attack_event, 0x0003005B, (u32)btlevtcmd_SetEventDamage, 0xFFFFFFFE, (u32)damage_event,
    0x0003005B, (u32)btlevtcmd_SetEventConfusion, 0xFFFFFFFE, (u32)btldefaultevt_Confuse, 0x0004005B, (u32)btlevtcmd_SetJumpSound,
    0xFFFFFFFE, (u32)(char*)str_SFX_MARIO_BATTLE_JUM_802f4018, (u32)(char*)str_SFX_MARIO_JUMP_LANDI_802f4030, 0x0007005B, (u32)btlevtcmd_SetWalkSound, 0xFFFFFFFE,
    (u32)(char*)str_SFX_MARIO_BATTLE_MOV_802f4048, (u32)(char*)str_SFX_MARIO_BATTLE_MOV_802f4060, 0x00000000, 0x00000014, 0x00000014, 0x0007005B,
    (u32)btlevtcmd_SetRunSound, 0xFFFFFFFE, (u32)(char*)str_SFX_MARIO_BATTLE_MOV_802f4048, (u32)(char*)str_SFX_MARIO_BATTLE_MOV_802f4060, 0x00000000, 0x0000000A,
    0x0000000A, 0x0004005B, (u32)btlevtcmd_AnimeSetPoseSoundTable, 0xFFFFFFFE, 0x00000001, (u32)pose_sound_timing_table,
    0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000008, 0x00000000, 0x0005005B,
    (u32)btlevtcmd_SetPartsWork, 0xFFFFFFFE, 0x00000003, 0x00000000, 0x00000000, 0x0002005B,
    (u32)_mario_super_emblem_anim_set, 0xFFFFFFFE, 0x0003005B, (u32)btlevtcmd_SetEventEntry, 0xFFFFFFFE, (u32)battle_entry_event,
    0x0002001D, 0xF5DE0180, 0x000000C9, 0x0002001A, 0xF5DE0180, 0x000000D4,
    0x0001005B, (u32)_mario_makkuro_set, 0x00000021, 0x00000021, 0x0002005B, (u32)btlevtcmd_StartWaitEvent,
    0xFFFFFFFE, 0x00000002, 0x00000001,
};

u32 mario_dead_event[] = {
    0x0002005B, (u32)evt_btl_camera_set_prilimit, 0x00000001, 0x0003005B, (u32)evt_btl_camera_nomove_y_onoff, 0x00000001,
    0x00000000, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000001, 0x00000007, 0x0004005B,
    (u32)evt_btl_camera_set_homing_unit, 0x00000001, 0xFE363C8A, 0xFFFFFFFF, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv,
    0x00000001, 0x00000002, 0x0005005B, (u32)evt_btl_camera_set_posoffset, 0x00000001, 0x00000000,
    0x00000000, 0x00000000, 0x0003005B, (u32)evt_btl_camera_set_zoom, 0x00000001, 0x000001F4,
    0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_VOICE_MARIO_DOWN_802f4078, 0xF1194D80, 0x00000000,
    0xF1194D80, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFE363C8A, 0xFE363C8B, (u32)(char*)str_M_I_S_80422f6c,
    0x0001000A, 0x00000812, 0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_CONDITION_DOWN1_802f4090,
    0xF1194D80, 0x00000000, 0xF1194D80, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFE363C8A,
    0xFE363C8B, (u32)(char*)str_M_D_2_80422f48, 0x0001000A, 0x000007D0, 0x0003005B, (u32)btlevtcmd_KillUnit,
    0xFE363C8A, 0x00000001, 0x0002005B, (u32)evt_btl_camera_set_prilimit, 0x00000000, 0x00000002,
    0x00000001,
};

u32 mario_emergency_revival_event[] = {
    0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_VOICE_MARIO_DOWN_802f4078, 0xF1194D80, 0x00000000,
    0xF1194D80, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFE363C8A, 0xFE363C8B, (u32)(char*)str_M_I_S_80422f6c,
    0x0001000A, 0x00000812, 0x0002005B, (u32)btlevtcmd_ClearAllStatus, 0xFE363C8A, 0x0006005B,
    (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_CONDITION_DOWN1_802f4090, 0xF1194D80, 0x00000000, 0xF1194D80,
    0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFE363C8A, 0xFE363C8B, (u32)(char*)str_M_D_2_80422f48, 0x0001000A,
    0x000003E8, 0x0003005B, (u32)btlevtcmd_ConsumeItemReserve, 0xFE363C8A, 0x00000097, 0x0001000A,
    0x000003E8, 0x0000006E, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFE363C8A, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00020035, 0xFE363C81, 0x00000028, 0x0006005B,
    (u32)btlevtcmd_DispItemIcon, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000097, 0x00000078,
    0x00000070, 0x00010009, 0x00000078, 0x0006005B, (u32)btlevtcmd_OnOffStatus, 0xFE363C8A,
    0x0000001B, 0x00000000, 0x00000000, 0x00000000, 0x0003005B, (u32)btlevtcmd_SetHp,
    0xFE363C8A, 0x00000000, 0x0004005B, (u32)btlevtcmd_RecoverHp, 0xFE363C8A, 0xFE363C8B,
    0x0000000A, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFE363C8A, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00020035, 0xFE363C81, 0x0000001E, 0x000F005B, (u32)evt_eff,
    0x00000000, (u32)(char*)str_recovery_802f40a4, 0x00000000, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0000000A, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x0001000A, 0x000007D0, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType,
    0xFE363C8A, 0xFE363C8B, 0x0000002C, 0x0005005B, (u32)btlevtcmd_JumpSetting, 0xFE363C8A,
    0x00000000, 0xF24A7A80, 0xF24A7D4C, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFE363C8A,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000014, 0xFFFFFFFF, 0x0002005B,
    (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002, 0x00000001,
};

u32 mario_appeal_event[] = {
    0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001, 0x00000042, 0x0005005B,
    (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0006005B,
    (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_MARIO_APPEAL1_802f40b0, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000,
    0x00010009, 0x0000003C, 0x0002005B, (u32)btlevtcmd_AudienceAppeal, 0xFFFFFFFE, 0x00010009,
    0x0000001E, 0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002, 0x00000001,
};

static u32 attack_audience_event[] = {
    0x0003005B, (u32)evt_sub_random, 0x00000001, 0xFE363C80, 0x00020018, 0xFE363C80,
    0x00000000, 0x0001005E, (u32)btlataudevtMarioJump, 0x00000020, 0x0001005E, (u32)btlataudevtMarioHammer,
    0x00000021, 0x00000002, 0x00000001,
};

u32 btlataudevtMarioJump[] = {
    0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_J_1B_802f3f80, 0x0005005B,
    (u32)_get_attack_aud_no, 0xFE363C83, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0000006E,
    0x00010009, 0x0000000F, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x0000000D,
    0x0005005B, (u32)evt_btl_camera_set_homing_unit_audience, 0x00000000, 0xFFFFFFFE, 0x00000001, 0xFE363C83,
    0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000002, 0x0003005B, (u32)evt_btl_camera_set_zoom,
    0x00000000, 0x000000C8, 0x00000070, 0x00020032, 0xFE363C85, 0xFE363C81,
    0x00020035, 0xFE363C81, 0x0000001C, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE,
    0xFE363C80, 0x00000005, 0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE, 0xF24A9280,
    0x0003005B, (u32)btlevtcmd_SetFallAccel, 0xFFFFFFFE, 0xF24A7CE6, 0x0006005B, (u32)btlevtcmd_snd_se,
    0xFFFFFFFE, (u32)(char*)str_SFX_VOICE_MARIO_BATT_802f40c4, 0xF1194D80, 0x00000000, 0xF1194D80, 0x00020032,
    0xFE363C86, 0x00000024, 0x0006005B, (u32)btlevtcmd_MarioJumpParam, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0xFE363C86, 0x0008005B, (u32)btlevtcmd_MarioJumpPosition, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0xFE363C86, 0x00000000, 0xFFFFFFFF,
    0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_J_1D_802f40e4, 0x00010009,
    0x00000003, 0x0003005B, (u32)_attack_aud, 0xFE363C83, 0x00000000, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_J_1B_802f3f80, 0x0003005B, (u32)btlevtcmd_SetFallAccel,
    0xFFFFFFFE, 0xF24A7CE6, 0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE, 0xF24A9280,
    0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE, 0xFE363C80, 0x00000028, 0x00020032,
    0xFE363C81, 0xFE363C85, 0x00020035, 0xFE363C82, 0x00000005, 0x0007005B,
    (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000014,
    0xFFFFFFFF, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE, 0xFE363C80, 0x00000014,
    0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0000000A, 0xFFFFFFFF, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE, 0xFE363C80,
    0x0000000A, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000006, 0xFFFFFFFF, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE,
    0x00000001, 0x0000002A, 0x0001006F, 0xFE363C8F, 0x0001000A, 0x000001F4,
    0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000, 0x0001000A, 0x000003E8,
    0x00000070, 0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE, 0xF24A9A80, 0x00020032,
    0xFE363C80, 0x00000000, 0x0004005B, (u32)btlevtcmd_GetStageAudienceAreaSize, 0xFE363C84, 0xF1194D80,
    0xF1194D80, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE, 0xFE363C80, 0xFE363C84,
    0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00000000, 0xFFFFFFFF, 0x00000000, 0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00020032, 0xFE363C80, 0x00000000,
    0x0004005B, (u32)btlevtcmd_GetStageSize, 0xFE363C84, 0xF1194D80, 0xF1194D80, 0x0004005B,
    (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE, 0xFE363C80, 0xFE363C84, 0x0005005B, (u32)btlevtcmd_SetPos,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0002005B, (u32)btlevtcmd_WaitEventEnd,
    0xFE363C8F, 0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000000, 0xFFFFFFFF, 0x00000000, 0x0002005B, (u32)btlevtcmd_StartWaitEvent,
    0xFFFFFFFE, 0x00000002, 0x00000001,
};

u32 btlataudevtMarioHammer[] = {
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
    0x00000014, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000000, 0xFFFFFFFF, 0x00000000, 0x0003005B, (u32)evt_btl_camera_set_zoom,
    0x00000000, 0x000000C8, 0x0002005B, (u32)_get_mario_hammer_lv, 0xFE363C80, 0x00010022,
    0xFE363C80, 0x00010024, 0x00000002, 0x00020032, 0xFE363C80, (u32)(char*)str_M_H_5A_802f40ec,
    0x00020032, 0xFE363C81, (u32)(char*)str_M_H_5B_802f40f4, 0x00020032, 0xFE363C82, (u32)(char*)str_M_H_14_802f40fc,
    0x00010024, 0x00000003, 0x00020032, 0xFE363C80, (u32)(char*)str_M_H_9A_802f4104, 0x00020032,
    0xFE363C81, (u32)(char*)str_M_H_9B_802f410c, 0x00020032, 0xFE363C82, (u32)(char*)str_M_H_15_802f4114, 0x0000002A,
    0x00020032, 0xFE363C80, (u32)(char*)str_M_H_1A_802f411c, 0x00020032, 0xFE363C81, (u32)(char*)str_M_H_1B_802f4124,
    0x00020032, 0xFE363C82, (u32)(char*)str_M_H_13_802f412c, 0x00000031, 0x0004005B, (u32)btlevtcmd_AnimeChangePose,
    0xFFFFFFFE, 0x00000001, 0xFE363C80, 0x0002005B, (u32)_get_mario_hammer_lv, 0xFE363C80,
    0x00010022, 0xFE363C80, 0x00010024, 0x00000002, 0x0006005B, (u32)btlevtcmd_snd_se,
    0xFFFFFFFE, (u32)(char*)str_SFX_MARIO_HAMMER_HOL_802f4134, 0xF1194D80, 0x00000000, 0xF1194D80, 0x00010024,
    0x00000001, 0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_MARIO_HAMMER_HOL_802f414c, 0xF1194D80,
    0x00000000, 0xF1194D80, 0x0000002A, 0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE,
    (u32)(char*)str_SFX_MARIO_HAMMER_HOL_802f4164, 0xF1194D80, 0x00000000, 0xF1194D80, 0x00000031, 0x0001000A,
    0x000001F4, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, 0xFE363C81,
    0x00010009, 0x00000001, 0x0002005B, (u32)_get_mario_hammer_lv, 0xFE363C80, 0x00010022,
    0xFE363C80, 0x00010024, 0x00000002, 0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_MARIO_HAMMER_SWI_802f417c,
    0x00000000, 0x00010024, 0x00000001, 0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_MARIO_HAMMER_SWI_802f4194,
    0x00000000, 0x0000002A, 0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_MARIO_HAMMER_SWI_802f41ac, 0x00000000,
    0x00000031, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, 0xFE363C82,
    0x00010009, 0x00000007, 0x0002005B, (u32)_get_mario_hammer_lv, 0xFE363C80, 0x00010022,
    0xFE363C80, 0x00010024, 0x00000002, 0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_MARIO_HAMMER_HIT_802f41c4,
    0x00000000, 0x00010024, 0x00000001, 0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_MARIO_HAMMER_HIT_802f41e4,
    0x00000000, 0x0000002A, 0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_MARIO_HAMMER_HIT_802f4204, 0x00000000,
    0x00000031, 0x0003005B, (u32)_attack_aud, 0xFE363C83, 0x00000000, 0x0003005B,
    (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000003, 0x0006005B, (u32)evt_btl_camera_shake_h, 0x00000000,
    0x00000002, 0x00000000, 0x0000000A, 0x0000000D, 0x0001006F, 0xFE363C8F,
    0x0001000A, 0x000005DC, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000,
    0x0001000A, 0x000003E8, 0x00000070, 0x0001000A, 0x000001F4, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_H_16_802f4224, 0x0003005B, (u32)btlevtcmd_AnimeWaitPlayComplete,
    0xFFFFFFFE, 0x00000001, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001,
    0x0000002A, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE, 0xF24A9A80, 0x00020032,
    0xFE363C80, 0x00000000, 0x0004005B, (u32)btlevtcmd_GetStageAudienceAreaSize, 0xFE363C84, 0xF1194D80,
    0xF1194D80, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE, 0xFE363C80, 0xFE363C84,
    0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00000000, 0xFFFFFFFF, 0x00000000, 0x0002005B, (u32)btlevtcmd_WaitEventEnd, 0xFE363C8F,
    0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0005005B, (u32)btlevtcmd_SetPos, 0xFFFFFFFE, 0xFE363C88, 0xFE363C81, 0xFE363C82,
    0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00000000, 0xFFFFFFFF, 0x00000000, 0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE,
    0x00000002, 0x00000001,
};

/* Recovered unit_mario .sdata support tables and small event scripts. */
static u8 defence[5] = { 0x00, 0x00, 0x00, 0x00, 0x00 };

static u8 defence_attr[5] = { 0x00, 0x00, 0x00, 0x00, 0x00 };

static u8 regist[22] = { 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64 };

u32 pose_table_object[6] = {
    0x0000001C, (u32)(char*)str_KRN_H_1_802f3f90, 0x00000027, (u32)(char*)str_KRN_H_1_802f3f90, 0x00000045, (u32)(char*)str_KRN_H_1_802f3f90,
};

u32 pose_sound_timing_table[10] = {
    (u32)(char*)str_M_S_3_80422ed8, 0x3E800000, (u32)mario_pinch_pose_sound_callback, 0x00000000, 0x00000001, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000001,
};

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

u32 mario_win_reaction[9] = {
    0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_M_V_2_80422ef8, 0x0001000A,
    0x000003E8, 0x00000002, 0x00000001,
};
