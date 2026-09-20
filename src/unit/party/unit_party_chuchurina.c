#include "unit/party/unit_party_chuchurina.h"

extern s32 mono_alpha;

/* Forward declarations for recovered Chuchurina .sdata support data. */
static u32 attack_event[2];
static u32 wait_event[6];
static u32 main_evt[4];
extern s32 btlevtcmd_AnimeChangePoseFromTable();

extern void* _battleWorkPointer;
extern f32 float_910_8042412c;
extern s32 BattleTransID();
extern void* BattleGetUnitPtr(void*, s32);

/* Forward declarations for recovered Chuchurina structured data. */
static u32 data_table[16];
extern u32 pose_table_chuchurina_stay[];
extern u32 unitpartsdata_Party_Chuchurina[];
extern u32 unitdata_Party_Chuchurina[];
extern u32 partyWeapon_ChuchurinaMadowaseAttack[];
extern u32 partyWeapon_ChuchurinaKiss[];
extern u32 btldefaultevt_Dummy[];
extern u32 btldefaultevt_avoid_counter_event[];
extern u32 lbl_80003500[];
extern u32 subset_attack_ready_jump_event[];
extern s32 weaponGetACOutputParam();

/* Forward declarations for recovered Chuchurina event bytecode. */
static u32 attack_audience_event[205];
static u32 battle_entry_event[59];
static u32 damage_event[16];
static u32 init_event[48];
static u32 party_win_reaction[18];
extern u32 partyChuchurinaAttack_NormalAttack[];
extern u32 partyChuchurinaAttack_MadowaseAttack[];
extern u32 partyChuchurinaAttack_ItemSteal[];
extern u32 partyChuchurinaAttack_Kiss[];
extern u32 chuchurina_appeal_event[];
extern s32 _attack_aud();
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
extern s32 btlevtcmd_AddRotate();
extern s32 btlevtcmd_AnimeChangePose();
extern s32 btlevtcmd_AnimeChangePoseType();
extern s32 btlevtcmd_AnimeFlagOnOff();
extern s32 btlevtcmd_AnimeWaitPlayComplete();
extern s32 btlevtcmd_AnnounceMessage();
extern s32 btlevtcmd_AnnounceSetParam();
extern s32 btlevtcmd_AttackDeclare();
extern s32 btlevtcmd_AttackDeclareAll();
extern s32 btlevtcmd_AudienceAppeal();
extern s32 btlevtcmd_AudienceDeclareACResult();
extern s32 btlevtcmd_AudienceDeclareAcrobatResult();
extern s32 btlevtcmd_BtlIconDelete();
extern s32 btlevtcmd_BtlIconEntryItemId();
extern s32 btlevtcmd_BtlIconSetPosition();
extern s32 btlevtcmd_CalculateFaceDirection();
extern s32 btlevtcmd_ChangeFaceDirection();
extern s32 btlevtcmd_CheckDamage();
extern s32 btlevtcmd_CommandCheckDamage();
extern s32 btlevtcmd_CommandGetWeaponActionLv();
extern s32 btlevtcmd_CommandGetWeaponAddress();
extern s32 btlevtcmd_CommandPayWeaponCost();
extern s32 btlevtcmd_CommandPreCheckDamage();
extern s32 btlevtcmd_DivePartsPosition();
extern s32 btlevtcmd_DivePosition();
extern s32 btlevtcmd_FaceDirectionAdd();
extern s32 btlevtcmd_FaceDirectionSub();
extern s32 btlevtcmd_FallPartsPosition();
extern s32 btlevtcmd_GetFaceDirection();
extern s32 btlevtcmd_GetHeight();
extern s32 btlevtcmd_GetHitPos();
extern s32 btlevtcmd_GetHomePos();
extern s32 btlevtcmd_GetMoveFrame();
extern s32 btlevtcmd_GetPartsPos();
extern s32 btlevtcmd_GetPartyTechLv();
extern s32 btlevtcmd_GetPos();
extern s32 btlevtcmd_GetResultAC();
extern s32 btlevtcmd_GetResultPrizeLv();
extern s32 btlevtcmd_GetRotate();
extern s32 btlevtcmd_GetSelectEnemy();
extern s32 btlevtcmd_GetSelectNextEnemy();
extern s32 btlevtcmd_GetStageAudienceAreaSize();
extern s32 btlevtcmd_GetStageSize();
extern s32 btlevtcmd_GetStatusMg();
extern s32 btlevtcmd_GetUnitId();
extern s32 btlevtcmd_GetUnitWork();
extern s32 btlevtcmd_GetWidth();
extern s32 btlevtcmd_InviteApInfoReport();
extern s32 btlevtcmd_JumpContinue();
extern s32 btlevtcmd_JumpPartsPosition();
extern s32 btlevtcmd_JumpPosition();
extern s32 btlevtcmd_JumpSetting();
extern s32 btlevtcmd_MovePosition();
extern s32 btlevtcmd_OffPartsAttribute();
extern s32 btlevtcmd_OffUnitFlag();
extern s32 btlevtcmd_OnPartsAttribute();
extern s32 btlevtcmd_OnUnitFlag();
extern s32 btlevtcmd_PreCheckCounter();
extern s32 btlevtcmd_PreCheckDamage();
extern s32 btlevtcmd_RecoverHp();
extern s32 btlevtcmd_ResetFaceDirection();
extern s32 btlevtcmd_ResultAC();
extern s32 btlevtcmd_RunDataEventChild();
extern s32 btlevtcmd_SetDiveSound();
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
extern s32 btlevtcmd_SetPartsBlur();
extern s32 btlevtcmd_SetPartsDiveSound();
extern s32 btlevtcmd_SetPartsFallAccel();
extern s32 btlevtcmd_SetPartsJumpSound();
extern s32 btlevtcmd_SetPartsMoveSpeed();
extern s32 btlevtcmd_SetPartsPos();
extern s32 btlevtcmd_SetPartsRotate();
extern s32 btlevtcmd_SetPartsRunSound();
extern s32 btlevtcmd_SetPartsWalkSound();
extern s32 btlevtcmd_SetPos();
extern s32 btlevtcmd_SetRGB();
extern s32 btlevtcmd_SetRotate();
extern s32 btlevtcmd_SetRotateOffset();
extern s32 btlevtcmd_SetRunSound();
extern s32 btlevtcmd_SetScale();
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
extern s32 btlevtcmd_ftof();
extern s32 btlevtcmd_ftomsec();
extern s32 btlevtcmd_snd_se();
extern s32 evt_audience_acrobat_notry();
extern s32 evt_audience_ap_recovery();
extern s32 evt_btl_camera_set_homing_unit();
extern s32 evt_btl_camera_set_homing_unit_audience();
extern s32 evt_btl_camera_set_homing_unitparts();
extern s32 evt_btl_camera_set_mode();
extern s32 evt_btl_camera_set_moveSpeedLv();
extern s32 evt_btl_camera_set_moveto();
extern s32 evt_btl_camera_set_zoom();
extern s32 evt_btl_camera_shake_h();
extern s32 evt_eff();
extern s32 evt_snd_sfxon();
extern s32 evt_snd_sfxon_3d();
extern s32 evt_sub_intpl_get_value();
extern s32 evt_sub_intpl_init();
extern u32 partyWeapon_ChuchurinaItemSteal[];
extern u32 partyWeapon_ChuchurinaNormalAttackLeft[];
extern u32 partyWeapon_ChuchurinaNormalAttackLeftLast[];
extern u32 partyWeapon_ChuchurinaNormalAttackRight[];
extern u32 partyWeapon_ChuchurinaNormalAttackRightLast[];
extern const char str_bomb_80424114[];
extern const char str_eff_80424124[];
extern const char str_kiss_8042411c[];
extern u32 unk_evt_803537c4[];
extern u32 zero_80424110[];

extern void* BtlUnit_GetPartsPtr(void*, s32);
extern void dispEntry(s32, s32, void*, void*, f32);
extern void* BattleGetPartyPtr(void* battleWork);
extern s32 evtGetValue(void* evt, s32 arg);
extern void evtSetValue(void* evt, s32 arg, s32 value);


s32 _chuchu_make_extra_work_area(void* evt) {
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
    extern s32 BattleTransPartyId(s32 id);
    extern s32 partyGetTechLv(s32 partyId);
    extern char* msgSearch(char* msg);
    extern u8 lbl_80381650[];
    s32 techLv;
    void* party;
    u8* data;
    u8* entry;

    party = BattleGetPartyPtr(_battleWorkPointer);
    techLv = partyGetTechLv(BattleTransPartyId(*(s32*)((s32)party + 8)));

    data = lbl_80381650;
    entry = (u8*)commandWork + *count * 0x1C;
    *(s32*)(entry + 0x90) = -1;
    entry = (u8*)commandWork + *count * 0x1C;
    *(s32*)(entry + 0x94) = 0;
    entry = (u8*)commandWork + *count * 0x1C;
    *(void**)(entry + 0x80) = data + 0x6A0;
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
        *(void**)(entry + 0x80) = data + 0x8E0;
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
        *(void**)(entry + 0x80) = data + 0x820;
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
        *(void**)(entry + 0x80) = data + 0x9A0;
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


s32 _get_binta_hit_position(int param_1) {
    extern f32 evtSetFloat(void* evt, s32 arg, f32 value);
    extern void BtlUnit_GetPos(void* unit, f32* x, f32* y, f32* z);

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

s32 _make_madowase_weapon(void* evt) {
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
    *(u8*)((s32)weapon + 0x84) = flag;
    evtSetValue(evt, out, (s32)weapon);
    return 2;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 _get_itemsteal_param(void* pEvt) {

    void* battleWork;
    s32* args;
    s32 id;
    s32 outRate;
    s32 outSteal;
    void* unit;
    s32 rate;
    s32 steal;

    args = *(s32**)((s32)pEvt + 0x18);
    id = evtGetValue(pEvt, args[0]);
    outRate = args[1];
    battleWork = _battleWorkPointer;
    outSteal = args[2];
    unit = BattleGetUnitPtr(battleWork, BattleTransID(pEvt, id));
    rate = 0x50 - ((*(u8*)((s32)battleWork + 0x1CBD) - 3) * 0x14);

    if (rate <= 5) {
        rate = 5;
    }
    steal = 0x64 - *(u8*)((s32)*(void**)((s32)unit + 0x10) + 0x8D);
    evtSetValue(pEvt, outRate, rate);
    evtSetValue(pEvt, outSteal, steal);
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

s32 _chuchu_item_steal(void* evt) {
    extern s32 BattleTransID(void* evt, s32 id);
    extern void* BattleGetUnitPtr(void* battleWork, s32 id);
    extern s32 irand(s32 max);
    extern s32 pouchGetItem(s32 itemId);
    extern s32 BtlUnit_CheckUnitFlag(void* unit, u32 flag);
    extern void BtlUnit_OffUnitFlag(void* unit, u32 flag);
    extern void BtlUnit_EquipItem(void* unit, s32 itemType, s32 itemId);
    s32* args;
    s32 id;
    s32 out;
    s32 flags;
    void* battleWork;
    void* unit;
    u16 itemId;
    void* itemTbl;
    s32 total;
    s16 weight;
    s32 roll;
    s32 unitId;
    void* battleUnitWork;

    args = *(s32**)((s32)evt + 0x18);
    battleWork = _battleWorkPointer;
    id = evtGetValue(evt, args[0]);
    out = args[1];
    flags = evtGetValue(evt, args[2]);
    unit = BattleGetUnitPtr(_battleWorkPointer, BattleTransID(evt, id));
    itemId = *(u16*)((s32)unit + 0x308);
    if (itemId == 0) {
        if (*(void**)((s32)unit + 0x30C) != 0) {
            irand(100);
        }
        itemTbl = *(void**)((s32)unit + 0x30C);
        if (itemTbl == 0) {
            itemId = 0;
        } else {
            total = 0;
            while ((weight = *(s16*)((s32)itemTbl + 4)) > 0 || *(s32*)itemTbl != 0) {
                total += weight;
                itemTbl = (void*)((s32)itemTbl + 8);
            }
            itemTbl = *(void**)((s32)unit + 0x30C);
            if (total <= 0) {
                itemId = 0;
            } else {
                roll = irand(total);
                while ((roll -= *(s16*)((s32)itemTbl + 4)) >= 0) {
                    itemTbl = (void*)((s32)itemTbl + 8);
                }
                itemId = *(u16*)itemTbl;
            }
        }
        if (itemId == 0) {
            itemId = 0x79;
        }
    }
    if ((flags & 2) == 0) {
        evtSetValue(evt, out, 0);
        return 2;
    }
    if (pouchGetItem(itemId) == 0) {
        evtSetValue(evt, out, 0);
        return 2;
    }
    *(s32*)((s32)unit + 0x308) = 0;
    if (BtlUnit_CheckUnitFlag(unit, 0x40000000) != 0) {
        BtlUnit_OffUnitFlag(unit, 0x40000000);
        unitId = *(s8*)((s32)unit + 0xE);
        if (unitId >= 0) {
            battleUnitWork = *(void**)(*(s32*)((s32)battleWork + 0x2738) + 0xC);
            *(s32*)((s32)battleUnitWork + unitId * 4 + 0x1C) = 0;
            *(s32*)((s32)battleUnitWork + unitId * 4 + 0x5C) = 0;
        }
    } else {
        unitId = *(s8*)((s32)unit + 0xE);
        if (unitId >= 0) {
            battleUnitWork = *(void**)(*(s32*)((s32)battleWork + 0x2738) + 0xC);
            *(s32*)((s32)battleUnitWork + unitId * 4 + 0x1C) = 0;
        }
    }
    if (itemId >= 0xF0 && itemId < 0x153) {
        if (*(s32*)((s32)unit + 8) == 0xDE) {
            BtlUnit_EquipItem(unit, 3, 0);
        } else if (*(s32*)((s32)unit + 8) >= 0xE0 && *(s32*)((s32)unit + 8) < 0xE7) {
            BtlUnit_EquipItem(unit, 5, 0);
        } else {
            BtlUnit_EquipItem(unit, 1, 0);
        }
    }
    evtSetValue(evt, out, itemId);
    return 2;
}
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void mono_capture(s32 param_1, void* unit) {
    extern u32 GXGetTexBufferSize(u16 width, u16 height, s32 format, s32 mipmap, s32 maxLod);
    extern void* smartAlloc(u32 size, s32 alignment);
    extern void GXSetTexCopySrc(u16 left, u16 top, u16 width, u16 height);
    extern void GXSetTexCopyDst(u16 width, u16 height, s32 format, s32 mipmap);
    extern void GXCopyTex(void* dest, s32 clear);
    extern void GXPixModeSync(void);

    void* parts = BtlUnit_GetPartsPtr(unit, 1);
    *(void**)((s32)unit + 0x31C) = smartAlloc(GXGetTexBufferSize(0x260, 0x1E0, 1, 0, 0), 1);
    GXSetTexCopySrc(0, 0, 0x260, 0x1E0);
    GXSetTexCopyDst(0x260, 0x1E0, 1, 0);
    GXCopyTex(**(void***)((s32)unit + 0x31C), 0);
    GXPixModeSync();
    *(u32*)((s32)parts + 0x204) &= ~0x100;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void mono_disp(int param_1, void* unit) {
    extern void* camGetPtr(s32 camId);
    extern void GXInitTexObj(void* obj, void* image, u16 width, u16 height, s32 format, s32 wrapS, s32 wrapT, s32 mipmap);
    extern void GXInitTexObjLOD(void* obj, s32 minFilt, s32 magFilt, f32 minLod, f32 maxLod, f32 lodBias, s32 biasClamp, s32 edgeLod, s32 maxAniso);
    extern void GXLoadTexObj(void* obj, s32 mapId);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXSetCurrentMtx(u32 id);
    extern void GXSetNumChans(u32 num);
    extern void GXSetNumTexGens(u32 num);
    extern void GXSetTexCoordGen2(s32 dst, s32 func, s32 src, u32 mtx, u32 normalize, s32 postMtx);
    extern void GXSetTevColor(s32 id, void* color);
    extern void GXSetNumTevStages(u32 num);
    extern void GXSetTevOrder(u32 stage, u32 texCoord, u32 texMap, u32 colorChan);
    extern void GXSetTevColorOp(s32 stage, u32 op, u32 bias, u32 scale, u32 clamp, u32 outReg);
    extern void GXSetTevAlphaOp(s32 stage, u32 op, u32 bias, u32 scale, u32 clamp, u32 outReg);
    extern void GXSetTevColorIn(s32 stage, u32 a, u32 b, u32 c, u32 d);
    extern void GXSetTevAlphaIn(s32 stage, u32 a, u32 b, u32 c, u32 d);
    extern void GXSetTevSwapMode(s32 stage, u32 rasSel, u32 texSel);
    extern void GXSetBlendMode(u32 type, u32 src, u32 dst, u32 op);
    extern void GXSetZCompLoc(u32 beforeTex);
    extern void GXSetAlphaCompare(u32 comp0, u32 ref0, u32 op, u32 comp1, u32 ref1);
    extern void GXSetZMode(u32 enable, u32 func, u32 update);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(u32 attr, u32 type);
    extern void GXSetVtxAttrFmt(u32 vtxFmt, u32 attr, u32 compCnt, u32 compType, u32 frac);
    extern void GXBegin(u32 prim, u32 vtxFmt, u16 nverts);
    extern s32 camGetCurNo(void);
    extern void camSetCurNo(s32 camNo);
    extern void btlUnitPartsDisp(s32 cameraId, void* part);
    extern u32 dat_80424128;
    extern f32 float_0_80424130;
    extern f32 float_neg304_80424134;
    extern f32 float_240_80424138;
    extern f32 float_304_8042413c;
    extern f32 float_1_80424140;
    extern f32 float_neg240_80424144;

    u32 colorTemp;
    u32 color;
    u8 texObj[0x20];
    void* camera;
    void* texture;
    void* party;
    void* part;
    s32 oldCam;
    volatile f32* fifo;

    camera = camGetPtr(param_1);
    texture = 0;
    if (*(void**)((s32)unit + 0x31C) != 0) {
        texture = **(void***)((s32)unit + 0x31C);
    }
    if (texture == 0) {
        return;
    }

    GXInitTexObj(texObj, texture, 0x260, 0x1E0, 1, 0, 0, 0);
    GXInitTexObjLOD(texObj, 1, 1, float_0_80424130, float_0_80424130, float_0_80424130, 0, 0, 0);
    GXLoadTexObj(texObj, 0);
    GXLoadPosMtxImm((void*)((s32)camera + 0x11C), 0);
    GXSetCurrentMtx(0);
    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);

    colorTemp = (dat_80424128 & 0xFFFFFF00) | (mono_alpha & 0xFF);
    color = colorTemp;
    GXSetTevColor(1, &color);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0xF, 0xF, 0xF, 8);
    GXSetTevAlphaIn(0, 7, 7, 7, 1);
    GXSetTevSwapMode(0, 0, 0);
    GXSetBlendMode(1, 4, 5, 0);
    GXSetZCompLoc(1);
    GXSetAlphaCompare(7, 0, 0, 7, 0);
    GXSetZMode(0, 3, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(0xD, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);
    GXBegin(0x80, 0, 4);

    fifo = (volatile f32*)0xCC008000;
    *fifo = float_neg304_80424134;
    *fifo = float_240_80424138;
    *fifo = float_0_80424130;
    *fifo = float_0_80424130;
    *fifo = float_0_80424130;
    *fifo = float_304_8042413c;
    *fifo = float_240_80424138;
    *fifo = float_0_80424130;
    *fifo = float_1_80424140;
    *fifo = float_0_80424130;
    *fifo = float_304_8042413c;
    *fifo = float_neg240_80424144;
    *fifo = float_0_80424130;
    *fifo = float_1_80424140;
    *fifo = float_1_80424140;
    *fifo = float_neg304_80424134;
    *fifo = float_neg240_80424144;
    *fifo = float_0_80424130;
    *fifo = float_0_80424130;
    *fifo = float_1_80424140;

    party = BattleGetPartyPtr(_battleWorkPointer);
    if (party != 0) {
        part = BtlUnit_GetPartsPtr(party, 1);
        oldCam = camGetCurNo();
        camSetCurNo(4);
        btlUnitPartsDisp(4, part);
        camSetCurNo(oldCam);
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on



s32 mono_main(int param_1) {
    s32 id = BattleTransID(param_1, -2);
    void* unit = BattleGetUnitPtr(_battleWorkPointer, id);

    dispEntry(8, 0, mono_disp, unit, float_910_8042412c);
    return 0;
}

s32 mono_capture_event(int param_1) {
    s32 id = BattleTransID(param_1, -2);
    void* unit = BattleGetUnitPtr(_battleWorkPointer, id);

    BtlUnit_GetPartsPtr(unit, 1);
    dispEntry(8, 0, mono_capture, unit, float_910_8042412c);
    return 2;
}

s32 mono_on(s32 param_1, int param_2) {
    if (param_2 != 0) {
        mono_alpha = 0;
    }
    mono_alpha += 10;
    if (mono_alpha > 0xFF) {
        mono_alpha = 0xFF;
        return 2;
    }
    return 0;
}

s32 mono_off(void* evt) {
    extern void smartFree(void* ptr);

    s32 id = BattleTransID(evt, -2);
    void* unit = BattleGetUnitPtr(_battleWorkPointer, id);

    mono_alpha -= 10;
    if (mono_alpha < 0) {
        mono_alpha = 0;
        BtlUnit_GetPartsPtr(unit, 1);
        if (*(void**)((s32)unit + 0x31C) != 0) {
            smartFree(*(void**)((s32)unit + 0x31C));
        }
        *(void**)((s32)unit + 0x31C) = 0;
        return 2;
    }
    return 0;
}

s32 mono_alpha;

/* Recovered Chuchurina .rodata strings. */
const char str_PCH_D_4_802f7d30[] = "PCH_D_4";
const char str_PCH_X_1_802f7d38[] = "PCH_X_1";
const char str_PCH_D_2_802f7d40[] = "PCH_D_2";
const char str_PCH_N_1_802f7d48[] = "PCH_N_1";
const char str_PCH_D_3_802f7d50[] = "PCH_D_3";
const char str_PCH_S_1_802f7d58[] = "PCH_S_1";
const char str_PCH_G_1_802f7d60[] = "PCH_G_1";
const char str_PCH_D_1_802f7d68[] = "PCH_D_1";
const char str_PCH_W_1_802f7d70[] = "PCH_W_1";
const char str_PCH_R_1_802f7d78[] = "PCH_R_1";
const char str_PCH_T_1_802f7d80[] = "PCH_T_1";
const char str_PCH_I_1_802f7d88[] = "PCH_I_1";
const char str_PCH_C_1_802f7d90[] = "PCH_C_1";
const char str_PCH_O_1_802f7d98[] = "PCH_O_1";
const char str_btl_un_chuchurina_802f7da0[] = "btl_un_chuchurina";
const char str_c_tyutyu_802f7db4[] = "c_tyutyu";
const char str_SFX_PARTY_DAMAGE1_802f7dc0[] = "SFX_PARTY_DAMAGE1";
const char str_SFX_MARIO_DAMAGE_FIR_802f7dd4[] = "SFX_MARIO_DAMAGE_FIRE1";
const char str_SFX_MARIO_DAMAGE_ICE_802f7dec[] = "SFX_MARIO_DAMAGE_ICE1";
const char str_SFX_MARIO_DAMAGE_BIR_802f7e04[] = "SFX_MARIO_DAMAGE_BIRIBIRI1";
const char str_SFX_PARTY_BATTLE_SUB_802f7e20[] = "SFX_PARTY_BATTLE_SUBMOVE1L";
const char str_SFX_PARTY_BATTLE_SUB_802f7e3c[] = "SFX_PARTY_BATTLE_SUBMOVE1R";
const char str_SFX_PARTY_BATTLE_MOV_802f7e58[] = "SFX_PARTY_BATTLE_MOVE1L";
const char str_SFX_PARTY_BATTLE_MOV_802f7e70[] = "SFX_PARTY_BATTLE_MOVE1R";
const char str_SFX_PARTY_BATTLE_JUM_802f7e88[] = "SFX_PARTY_BATTLE_JUMP1";
const char str_SFX_PARTY_BATTLE_LAN_802f7ea0[] = "SFX_PARTY_BATTLE_LANDING1";
const char str_btl_wn_pcr_normal_802f7ebc[] = "btl_wn_pcr_normal";
const char str_msg_pch_binta_802f7ed0[] = "msg_pch_binta";
const char str_msg_ac_binta_802f7ee0[] = "msg_ac_binta";
const char str_btl_wn_pcr_madowase_802f7ef0[] = "btl_wn_pcr_madowase";
const char str_msg_pch_madowaseru_802f7f04[] = "msg_pch_madowaseru";
const char str_msg_ac_madowaseru_802f7f18[] = "msg_ac_madowaseru";
const char str_btl_wn_pcr_steal_802f7f2c[] = "btl_wn_pcr_steal";
const char str_msg_pch_heart_catch_802f7f40[] = "msg_pch_heart_catch";
const char str_msg_ac_heart_catch_802f7f54[] = "msg_ac_heart_catch";
const char str_btl_wn_pcr_kiss_802f7f68[] = "btl_wn_pcr_kiss";
const char str_msg_pch_kiss_802f7f78[] = "msg_pch_kiss";
const char str_msg_ac_chuchurina_ki_802f7f88[] = "msg_ac_chuchurina_kiss";
const char str_PCH_A1_1_802f7fa0[] = "PCH_A1_1";
const char str_PCH_A1_2_802f7fac[] = "PCH_A1_2";
const char str_PCH_A2_4_802f7fb8[] = "PCH_A2_4";
const char str_SFX_BTL_CHUCHURINA_A_802f7fc4[] = "SFX_BTL_CHUCHURINA_ATTACK3";
const char str_SFX_BTL_CHUCHURINA_A_802f7fe0[] = "SFX_BTL_CHUCHURINA_ATTACK2";
const char str_SFX_BTL_CHUCHURINA_A_802f7ffc[] = "SFX_BTL_CHUCHURINA_ATTACK1";
const char str_PCH_A1_3_802f8018[] = "PCH_A1_3";
const char str_PCH_A4_1_802f8024[] = "PCH_A4_1";
const char str_SFX_BTL_CHURINA_CONF_802f8030[] = "SFX_BTL_CHURINA_CONFUSE_MOVE1L";
const char str_SFX_BTL_CHURINA_CONF_802f8050[] = "SFX_BTL_CHURINA_CONFUSE_MOVE1R";
const char str_SFX_BTL_CHURINA_CONF_802f8070[] = "SFX_BTL_CHURINA_CONFUSE_JUMP1";
const char str_SFX_BTL_CHURINA_CONF_802f8090[] = "SFX_BTL_CHURINA_CONFUSE_LANDING1";
const char str_PCH_A3_1_802f80b4[] = "PCH_A3_1";
const char str_SFX_BTL_CHURINA_CONF_802f80c0[] = "SFX_BTL_CHURINA_CONFUSE1";
const char str_PCH_A3_2_802f80dc[] = "PCH_A3_2";
const char str_PCH_A3_3_802f80e8[] = "PCH_A3_3";
const char str_PCH_B_1_802f80f4[] = "PCH_B_1";
const char str_SFX_BTL_CHURINA_CONF_802f80fc[] = "SFX_BTL_CHURINA_CONFUSE1_2";
const char str_PCH_Y_1_802f8118[] = "PCH_Y_1";
const char str_PCH_R_2_802f8120[] = "PCH_R_2";
const char str_PCH_K_1_802f8128[] = "PCH_K_1";
const char str_SFX_BTL_CHURINA_KISS_802f8130[] = "SFX_BTL_CHURINA_KISS1";
const char str_PCH_Z_1_802f8148[] = "PCH_Z_1";
const char str_SFX_BTL_CHURINA_EYE1_802f8150[] = "SFX_BTL_CHURINA_EYE1";
const char str_PCH_F_1_802f8168[] = "PCH_F_1";
const char str_SFX_BTL_CHURINA_HEAR_802f8170[] = "SFX_BTL_CHURINA_HEARTCATCH1";
const char str_PCH_A2_2_802f818c[] = "PCH_A2_2";
const char str_btl_msg_steal_item_g_802f8198[] = "btl_msg_steal_item_get";
const char str_recovery_802f81b0[] = "recovery";
const char str_SFX_MARIO_APPEAL1_802f81bc[] = "SFX_MARIO_APPEAL1";
const char str_PCH_O_3_802f81d0[] = "PCH_O_3";

/* Recovered Chuchurina .sdata support tables and small event scripts. */
static u8 defence[5] = { 0x00, 0x00, 0x00, 0x00, 0x00 };

static u8 defence_attr[5] = { 0x00, 0x00, 0x00, 0x00, 0x00 };

static u8 regist[22] = { 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64 };

static u32 attack_event[2] = {
    0x00000002, 0x00000001,
};

static u32 wait_event[6] = {
    0x0003005B, (u32)btlevtcmd_AnimeChangePoseFromTable, 0xFFFFFFFE, 0x00000001, 0x00000002, 0x00000001,
};

static u32 main_evt[4] = {
    0x0001005B, (u32)mono_main, 0x00000002, 0x00000001,
};

/*
 * Recovered Chuchurina battle event bytecode.
 * Exact US target words are preserved; relocation words remain symbolic.
 * Structured/unit/BattleWeapon data remains a separate later lane.
 */
/* Recovered Chuchurina pose/unit structured data. */
u32 pose_table_chuchurina_stay[48] = {
    0x00000009, (u32)(char*)str_PCH_D_4_802f7d30, 0x00000002, (u32)(char*)str_PCH_D_4_802f7d30, 0x00000003, (u32)(char*)str_PCH_X_1_802f7d38,
    0x00000004, (u32)(char*)str_PCH_D_2_802f7d40, 0x00000005, (u32)(char*)str_PCH_X_1_802f7d38, 0x00000001, (u32)(char*)str_PCH_N_1_802f7d48,
    0x0000001B, (u32)(char*)str_PCH_D_3_802f7d50, 0x0000001C, (u32)(char*)str_PCH_S_1_802f7d58, 0x00000025, (u32)(char*)str_PCH_G_1_802f7d60,
    0x0000001F, (u32)(char*)str_PCH_G_1_802f7d60, 0x00000027, (u32)(char*)str_PCH_D_1_802f7d68, 0x00000028, (u32)(char*)str_PCH_W_1_802f7d70,
    0x0000002A, (u32)(char*)str_PCH_R_1_802f7d78, 0x00000039, (u32)(char*)str_PCH_D_2_802f7d40, 0x00000038, (u32)(char*)str_PCH_D_2_802f7d40,
    0x0000003C, (u32)(char*)str_PCH_D_4_802f7d30, 0x0000003D, (u32)(char*)str_PCH_D_2_802f7d40, 0x00000041, (u32)(char*)str_PCH_T_1_802f7d80,
    0x00000034, (u32)(char*)str_PCH_I_1_802f7d88, 0x00000043, (u32)(char*)str_PCH_C_1_802f7d90, 0x00000040, (u32)(char*)str_PCH_X_1_802f7d38,
    0x00000031, (u32)(char*)str_PCH_O_1_802f7d98, 0x0000002D, (u32)(char*)str_PCH_G_1_802f7d60, 0x00000045, (u32)(char*)str_PCH_S_1_802f7d58,
};

static u32 data_table[16] = {
    0x00000005, (u32)chuchurina_appeal_event, 0x00000006, (u32)attack_audience_event, 0x00000001, (u32)__makeTechMenuFunc,
    0x00000030, (u32)btldefaultevt_Dummy, 0x00000040, (u32)party_win_reaction, 0x00000007, (u32)subset_attack_ready_jump_event,
    0x0000002F, (u32)btldefaultevt_avoid_counter_event, 0x00000000, 0x00000000,
};

u32 unitpartsdata_Party_Chuchurina[76] = {
    0x00000001, (u32)(char*)str_btl_un_chuchurina_802f7da0, (u32)(char*)str_c_tyutyu_802f7db4, 0x00000000, 0x00000000, 0x00000000,
    0x41800000, 0x41C00000, 0x40A00000, 0x00000000, 0x41C00000, 0x00000000,
    0x00140018, 0x00FF0000, (u32)defence, (u32)defence_attr, 0x00000009, 0x00000000,
    (u32)pose_table_chuchurina_stay, 0x00000002, (u32)(char*)str_btl_un_chuchurina_802f7da0, (u32)(char*)str_c_tyutyu_802f7db4, 0x00000000, 0x00000000,
    0x00000000, 0x41800000, 0x41C00000, 0x40A00000, 0x00000000, 0x41C00000,
    0x00000000, 0x00140018, 0x00FF0000, (u32)defence, (u32)defence_attr, 0x13010000,
    0x00000000, (u32)pose_table_chuchurina_stay, 0x00000003, (u32)(char*)str_btl_un_chuchurina_802f7da0, (u32)(char*)str_c_tyutyu_802f7db4, 0x00000000,
    0x00000000, 0x00000000, 0x41800000, 0x41C00000, 0x40A00000, 0x00000000,
    0x41C00000, 0x00000000, 0x00140018, 0x00FF0000, (u32)defence, (u32)defence_attr,
    0x13010000, 0x00000000, (u32)pose_table_chuchurina_stay, 0x00000004, (u32)(char*)str_btl_un_chuchurina_802f7da0, (u32)(char*)str_c_tyutyu_802f7db4,
    0x00000000, 0x00000000, 0x00000000, 0x41800000, 0x41C00000, 0x40A00000,
    0x00000000, 0x41C00000, 0x00000000, 0x00140018, 0x00FF0000, (u32)defence,
    (u32)defence_attr, 0x13010000, 0x00000000, (u32)pose_table_chuchurina_stay,
};

u32 unitdata_Party_Chuchurina[49] = {
    0x000000E6, (u32)(char*)str_btl_un_chuchurina_802f7da0, 0x000A0005, 0x05010100, 0x00000064, 0x00090014,
    0x0021000A, 0x00160000, 0x00000000, 0x41400000, 0x00000000, 0x00000000,
    0x00000000, 0x41F00000, 0x00000000, 0x00000000, 0x41F00000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x41200000,
    0x00000000, 0x00000000, 0x41200000, 0x41AB999A, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0000FF00, 0x64140000,
    0x00000000, 0x00000000, 0x00000000, (u32)(char*)str_SFX_PARTY_DAMAGE1_802f7dc0, (u32)(char*)str_SFX_MARIO_DAMAGE_FIR_802f7dd4, (u32)(char*)str_SFX_MARIO_DAMAGE_ICE_802f7dec,
    (u32)(char*)str_SFX_MARIO_DAMAGE_BIR_802f7e04, 0x02000000, (u32)regist, 0x04000000, (u32)unitpartsdata_Party_Chuchurina, (u32)init_event,
    (u32)data_table,
};

static u32 battle_entry_event[59] = {
    0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE, 0xFE363C80, 0x000000FA, 0x0005005B,
    (u32)btlevtcmd_SetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00010009,
    0x00000028, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001, 0x0000002A,
    0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE, 0xF24A8A80, 0x0009005B, (u32)btlevtcmd_SetMoveSoundDataWork,
    0xFFFFFFFE, (u32)(char*)str_SFX_PARTY_BATTLE_SUB_802f7e20, (u32)(char*)str_SFX_PARTY_BATTLE_SUB_802f7e3c, 0x00000000, 0x00000007, 0x00000007,
    0x00000000, 0x00000000, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000000, 0xFFFFFFFE, 0x00000000, 0x0003005B,
    (u32)btlevtcmd_AnimeChangePoseFromTable, 0xFFFFFFFE, 0x00000001, 0x00000002, 0x00000001,
};

static u32 init_event[48] = {
    0x0001005B, (u32)_chuchu_make_extra_work_area, 0x0003005B, (u32)btlevtcmd_SetEventWait, 0xFFFFFFFE, (u32)wait_event,
    0x0003005B, (u32)btlevtcmd_SetEventAttack, 0xFFFFFFFE, (u32)attack_event, 0x0003005B, (u32)btlevtcmd_SetEventDamage,
    0xFFFFFFFE, (u32)damage_event, 0x0003005B, (u32)btlevtcmd_SetEventConfusion, 0xFFFFFFFE, (u32)btldefaultevt_Confuse,
    0x0007005B, (u32)btlevtcmd_SetWalkSound, 0xFFFFFFFE, (u32)(char*)str_SFX_PARTY_BATTLE_MOV_802f7e58, (u32)(char*)str_SFX_PARTY_BATTLE_MOV_802f7e70, 0x00000000,
    0x0000000F, 0x0000000D, 0x0007005B, (u32)btlevtcmd_SetRunSound, 0xFFFFFFFE, (u32)(char*)str_SFX_PARTY_BATTLE_MOV_802f7e58,
    (u32)(char*)str_SFX_PARTY_BATTLE_MOV_802f7e70, 0x00000000, 0x00000007, 0x00000007, 0x0004005B, (u32)btlevtcmd_SetJumpSound,
    0xFFFFFFFE, (u32)(char*)str_SFX_PARTY_BATTLE_JUM_802f7e88, (u32)(char*)str_SFX_PARTY_BATTLE_LAN_802f7ea0, 0x0003005B, (u32)btlevtcmd_SetEventEntry, 0xFFFFFFFE,
    (u32)battle_entry_event, 0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002, 0x00000001,
};

static u32 damage_event[16] = {
    0x0005005B, (u32)btlevtcmd_SetRotate, 0xFFFFFFFE, 0x00000000, 0x00000000, 0x00000000,
    0x00020032, 0xFE363C8A, 0xFFFFFFFE, 0x00020032, 0xFE363C8B, 0x00000001,
    0x0001005E, (u32)btldefaultevt_Damage, 0x00000002, 0x00000001,
};

/* Recovered Chuchurina BattleWeapon records. */
u32 partyWeapon_ChuchurinaNormalAttackLeft[48] = {
    (u32)(char*)str_btl_wn_pcr_normal_802f7ebc, 0x01580000, 0x00000000, (u32)(char*)str_msg_pch_binta_802f7ed0, 0x64000000, 0x3F800000,
    0x01016432, 0x00000000, 0x00000002, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01101260, 0x21002000, 0x00150302, (u32)(char*)str_msg_ac_binta_802f7ee0, 0x00034047,
    0x00000202, 0x80002004, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)partyChuchurinaAttack_NormalAttack, 0x00000064, 0x00040300, 0x00000000,
};

u32 partyWeapon_ChuchurinaNormalAttackRight[48] = {
    (u32)(char*)str_btl_wn_pcr_normal_802f7ebc, 0x01580000, 0x00000000, (u32)(char*)str_msg_pch_binta_802f7ed0, 0x64000000, 0x3F800000,
    0x01016432, 0x00000000, 0x00000002, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01101260, 0x21002000, 0x00160302, (u32)(char*)str_msg_ac_binta_802f7ee0, 0x00034047,
    0x00000202, 0x80002004, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)partyChuchurinaAttack_NormalAttack, 0x00000064, 0x00040300, 0x00000000,
};

u32 partyWeapon_ChuchurinaNormalAttackLeftLast[48] = {
    (u32)(char*)str_btl_wn_pcr_normal_802f7ebc, 0x01580000, 0x00000000, (u32)(char*)str_msg_pch_binta_802f7ed0, 0x64000000, 0x3F800000,
    0x01016432, (u32)weaponGetACOutputParam, 0x00000002, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01101260, 0x21002000, 0x00170302, (u32)(char*)str_msg_ac_binta_802f7ee0, 0x00034047,
    0x00000002, 0x80002004, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)partyChuchurinaAttack_NormalAttack, 0x00000064, 0x00040200, 0x00000000,
};

u32 partyWeapon_ChuchurinaNormalAttackRightLast[48] = {
    (u32)(char*)str_btl_wn_pcr_normal_802f7ebc, 0x01580000, 0x00000000, (u32)(char*)str_msg_pch_binta_802f7ed0, 0x64000000, 0x3F800000,
    0x01016432, (u32)weaponGetACOutputParam, 0x00000002, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01101260, 0x21002000, 0x00180302, (u32)(char*)str_msg_ac_binta_802f7ee0, 0x00034047,
    0x00000002, 0x80002004, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)partyChuchurinaAttack_NormalAttack, 0x00000064, 0x00040200, 0x00000000,
};

u32 partyWeapon_ChuchurinaMadowaseAttack[48] = {
    (u32)(char*)str_btl_wn_pcr_madowase_802f7ef0, 0x015A0000, 0x00000000, (u32)(char*)str_msg_pch_madowaseru_802f7f04, 0x64030000, 0x3F800000,
    0x01056432, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x02101260, 0x20000000, 0x00000302, (u32)(char*)str_msg_ac_madowaseru_802f7f18, 0x00020000,
    0x000007FF, 0x80002004, 0x00000000, 0x64030000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)partyChuchurinaAttack_MadowaseAttack, 0x00000064, 0x00000500, 0x00000000,
};

u32 partyWeapon_ChuchurinaItemSteal[48] = {
    (u32)(char*)str_btl_wn_pcr_steal_802f7f2c, 0x01590000, 0x00000000, (u32)(char*)str_msg_pch_heart_catch_802f7f40, 0x64020000, 0x3F800000,
    0x01056432, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01101260, 0x21002000, 0x00140302, (u32)(char*)str_msg_ac_heart_catch_802f7f54, 0x00020000,
    0x00000202, 0x80002004, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)partyChuchurinaAttack_ItemSteal, 0x00000064, 0x00000500, 0x00000000,
};

u32 partyWeapon_ChuchurinaKiss[48] = {
    (u32)(char*)str_btl_wn_pcr_kiss_802f7f68, 0x015B0000, 0x00000000, (u32)(char*)str_msg_pch_kiss_802f7f78, 0x640A0000, 0x3F800000,
    0x01056432, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01101160, 0x10000000, 0x00000302, (u32)(char*)str_msg_ac_chuchurina_ki_802f7f88, 0x00020008,
    0x000007FF, (u32)lbl_80003500, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)partyChuchurinaAttack_Kiss, 0x00000064, 0x00000500, 0x00000000,
};

u32 partyChuchurinaAttack_NormalAttack[] = {
    0x0005005B, (u32)btlevtcmd_JumpSetting, 0xFFFFFFFE, 0x00000014, 0xF24A7A80, 0xF24A7D4C,
    0x0003005B, (u32)btlevtcmd_GetSelectEnemy, 0xFE363C83, 0xFE363C84, 0x00020018, 0xFE363C83,
    0xFFFFFFFF, 0x00010004, 0x00000063, 0x00000021, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress,
    0xFFFFFFFE, 0xFE363C8C, 0x0002005B, (u32)btlevtcmd_WeaponAftereffect, 0xFE363C8C, 0x0004005B,
    (u32)btlevtcmd_AttackDeclare, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0x0001005B, (u32)btlevtcmd_WaitGuardMove,
    0x0002005B, (u32)btlevtcmd_CommandPayWeaponCost, 0xFFFFFFFE, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFFFFFFFE,
    0x00000007, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x0000000B, 0x0006005B,
    (u32)evt_btl_camera_set_homing_unitparts, 0x00000000, 0xFFFFFFFE, 0x00000001, 0xFE363C83, 0xFE363C84,
    0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000002, 0x0003005B, (u32)evt_btl_camera_set_zoom,
    0x00000000, 0x000000C8, 0x0006005B, (u32)btlevtcmd_CommandPreCheckDamage, 0xFFFFFFFE, 0xFE363C83,
    0xFE363C84, 0x00000100, 0xFE363C85, 0x00020018, 0xFE363C85, 0x00000005,
    0x0003005B, (u32)btlevtcmd_GetStatusMg, 0xFE363C83, 0xFE363C86, 0x0002003C, 0xFE363C86,
    0x00000028, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFE363C83, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0004005B, (u32)btlevtcmd_TransStageFloorPosition, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0004005B, (u32)btlevtcmd_FaceDirectionAdd, 0xFE363C83, 0xFE363C80, 0xFE363C86, 0x0007005B,
    (u32)btlevtcmd_CalculateFaceDirection, 0xFFFFFFFE, 0xFFFFFFFF, 0xFE363C80, 0xFE363C82, 0x00000001,
    0xFE363C8F, 0x0003005B, (u32)btlevtcmd_ChangeFaceDirection, 0xFFFFFFFE, 0xFE363C8F, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001, 0x00000028, 0x0003005B, (u32)btlevtcmd_SetMoveSpeed,
    0xFFFFFFFE, 0xF24A8680, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000000, 0xFFFFFFFF, 0x00000000, 0x0006005B,
    (u32)btlevtcmd_CommandCheckDamage, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0x00000100, 0xFE363C85,
    0x00000021, 0x0003005B, (u32)btlevtcmd_GetStatusMg, 0xFFFFFFFE, 0xFE363C85, 0x0002003C,
    0xFE363C85, 0x0000000C, 0x0007005B, (u32)_get_binta_hit_position, 0xFFFFFFFE, 0xFE363C83,
    0xFE363C84, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B, (u32)btlevtcmd_FaceDirectionAdd,
    0xFE363C83, 0xFE363C80, 0xFE363C85, 0x0004005B, (u32)btlevtcmd_TransStageFloorPosition, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0007005B, (u32)btlevtcmd_CalculateFaceDirection, 0xFFFFFFFE, 0xFFFFFFFF,
    0xFE363C80, 0xFE363C82, 0x00000001, 0xFE363C8F, 0x0003005B, (u32)btlevtcmd_ChangeFaceDirection,
    0xFFFFFFFE, 0xFE363C8F, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001,
    0x00000028, 0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE, 0xF24A8680, 0x00020035,
    0xFE363C82, 0x00000005, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000000, 0xFFFFFFFF, 0x00000000, 0x0007005B,
    (u32)btlevtcmd_CalculateFaceDirection, 0xFFFFFFFE, 0xFFFFFFFF, 0xFE363C83, 0xFE363C84, 0x00000010,
    0xFE363C8F, 0x0003005B, (u32)btlevtcmd_ChangeFaceDirection, 0xFFFFFFFE, 0xFE363C8F, 0x0003005B,
    (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000002, 0x0003005B, (u32)evt_btl_camera_set_zoom, 0x00000000,
    0x0000015E, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PCH_A1_1_802f7fa0,
    0x0001006F, 0xFE363C80, 0x0002005B, (u32)btlevtcmd_CommandGetWeaponActionLv, 0xFE363C80, 0x0003005B,
    (u32)btlevtcmd_AcSetDifficulty, 0xFFFFFFFE, 0xFE363C80, 0x0002005B, (u32)btlevtcmd_AcGetDifficulty, 0xFE363C80,
    0x00020036, 0xFE363C80, 0x00000003, 0x00010022, 0xFE363C80, 0x00010028,
    0xFFFFFFFD, 0x00020032, 0xFE363C81, 0x00000019, 0x00020032, 0xFE363C82,
    0x0000001E, 0x00010024, 0xFFFFFFFE, 0x00020032, 0xFE363C81, 0x00000014,
    0x00020032, 0xFE363C82, 0x0000001E, 0x00010024, 0xFFFFFFFF, 0x00020032,
    0xFE363C81, 0x00000011, 0x00020032, 0xFE363C82, 0x0000001E, 0x00010024,
    0x00000000, 0x00020032, 0xFE363C81, 0x0000000F, 0x00020032, 0xFE363C82,
    0x0000001E, 0x00010024, 0x00000001, 0x00020032, 0xFE363C81, 0x0000000E,
    0x00020032, 0xFE363C82, 0x0000001E, 0x00010024, 0x00000002, 0x00020032,
    0xFE363C81, 0x0000000D, 0x00020032, 0xFE363C82, 0x0000001E, 0x0000002A,
    0x00020032, 0xFE363C81, 0x0000000C, 0x00020032, 0xFE363C82, 0x0000001E,
    0x00000031, 0x0003005B, (u32)btlevtcmd_GetPartyTechLv, 0xFFFFFFFE, 0xFE363C80, 0x00010022,
    0xFE363C80, 0x00010024, 0x00000002, 0x0009005B, (u32)btlevtcmd_AcSetParamAll, 0x0000000E,
    0x000000D2, 0x000000B2, 0xFE363C81, 0xFE363C82, 0x00000022, 0x00000023,
    0xF1194D80, 0x0005005B, (u32)btlevtcmd_AcSetGaugeParam, 0x00000022, 0x00000044, 0x00000064,
    0x00000064, 0x0002005B, (u32)btlevtcmd_AcSetFlag, 0x00000040, 0x00010024, 0x00000001,
    0x0009005B, (u32)btlevtcmd_AcSetParamAll, 0x0000000E, 0x000000D2, 0x000000B2, 0xFE363C81,
    0xFE363C82, 0x00000032, 0x00000033, 0xF1194D80, 0x0005005B, (u32)btlevtcmd_AcSetGaugeParam,
    0x00000022, 0x00000044, 0x00000064, 0x00000064, 0x0002005B, (u32)btlevtcmd_AcSetFlag,
    0x00000040, 0x0000002A, 0x0009005B, (u32)btlevtcmd_AcSetParamAll, 0x0000000E, 0x000000D2,
    0x000000B2, 0xFE363C81, 0xFE363C82, 0x00000064, 0x00000064, 0xF1194D80,
    0x0005005B, (u32)btlevtcmd_AcSetGaugeParam, 0x00000064, 0x00000064, 0x00000064, 0x00000064,
    0x0002005B, (u32)btlevtcmd_AcSetFlag, 0x00000040, 0x00000031, 0x0005005B, (u32)btlevtcmd_SetupAC,
    0xFFFFFFFE, 0x00000006, 0x00000001, 0x00000000, 0x00010009, 0x00000001,
    0x0002005B, (u32)btlevtcmd_StartAC, 0x00000001, 0x00000070, 0x0002005B, (u32)btlevtcmd_WaitEventEnd,
    0xFE363C80, 0x0001006F, 0xFE363C8E, 0x00020032, 0xFE363C86, 0x00000000,
    0x00020032, 0xFE363C85, 0x00000005, 0x00010005, 0xFE363C85, 0x00020035,
    0xFE363C86, 0x00000001, 0x00020032, 0xFE363C80, 0xFE363C86, 0x00020038,
    0xFE363C80, 0x00000002, 0x00020039, 0xFE363C80, 0x00000002, 0x00020018,
    0xFE363C80, 0x00000000, 0x0005005B, (u32)btlevtcmd_SetScale, 0xFFFFFFFE, 0xF24A7EB3,
    0xF24A7EB3, 0xF24A7E80, 0x00000020, 0x0005005B, (u32)btlevtcmd_SetScale, 0xFFFFFFFE,
    0xF24A7E80, 0xF24A7E80, 0xF24A7E80, 0x00000021, 0x00010009, 0x00000001,
    0x00020032, 0xFE363C85, 0x00000005, 0x00000006, 0x00000070, 0x0001006F,
    0xFE363C8F, 0x00020032, 0xFE363C85, 0x00000005, 0x00010005, 0xFE363C85,
    0x0003005B, (u32)btlevtcmd_AcGetOutputParam, 0x00000001, 0xFE363C80, 0x00020032, 0xFE363C81,
    0x000000FF, 0x00020032, 0xFE363C82, 0x000000BE, 0x00020037, 0xFE363C82,
    0xFE363C80, 0x00020038, 0xFE363C82, 0x00000064, 0x00020036, 0xFE363C81,
    0xFE363C82, 0x0006005B, (u32)btlevtcmd_SetRGB, 0xFFFFFFFE, 0x00000001, 0x000000FF,
    0xFE363C81, 0xFE363C81, 0x00010009, 0x00000001, 0x00020032, 0xFE363C85,
    0x00000005, 0x00000006, 0x00000070, 0x0001005B, (u32)btlevtcmd_ResultAC, 0x0001005F,
    0xFE363C8E, 0x0001005F, 0xFE363C8F, 0x0003005B, (u32)btlevtcmd_GetPartyTechLv, 0xFFFFFFFE,
    0xFE363C80, 0x00010022, 0xFE363C80, 0x00010024, 0x00000002, 0x0004005B,
    (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000005, 0x00000004, 0x00010024, 0x00000001,
    0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000005, 0x00000003, 0x0000002A,
    0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000005, 0x00000002, 0x00000031,
    0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000002, 0x0003005B, (u32)evt_btl_camera_set_zoom,
    0x00000000, 0x0000012C, 0x0006005B, (u32)btlevtcmd_SetRGB, 0xFFFFFFFE, 0x00000001,
    0x000000FF, 0x000000FF, 0x000000FF, 0x0001006F, 0xFE363C8E, 0x00020032,
    0xFE363C88, 0xFFFFFFB5, 0x0005005B, (u32)btlevtcmd_GetRotate, 0xFFFFFFFE, 0xF1194D80,
    0xFE363C80, 0xF1194D80, 0x0002001D, 0xFE363C80, 0x000000B4, 0x00020036,
    0xFE363C80, 0x00000168, 0x00000021, 0x0005005B, (u32)evt_sub_intpl_init, 0x00000005,
    0xFE363C80, 0xFE363C88, 0x00000008, 0x00010005, 0x00000008, 0x0001005B,
    (u32)evt_sub_intpl_get_value, 0x0005005B, (u32)btlevtcmd_SetRotate, 0xFFFFFFFE, 0x00000000, 0xFE363C80,
    0x00000000, 0x00010009, 0x00000001, 0x00000006, 0x00020032, 0xFE363C88,
    0xFFFFFFC4, 0x00020032, 0xFE363C89, 0xFFFFFFB5, 0x0005005B, (u32)evt_sub_intpl_init,
    0x0000000B, 0xFE363C89, 0xFE363C88, 0x00000004, 0x00010005, 0x00000004,
    0x0001005B, (u32)evt_sub_intpl_get_value, 0x0005005B, (u32)btlevtcmd_SetRotate, 0xFFFFFFFE, 0x00000000,
    0xFE363C80, 0x00000000, 0x00010009, 0x00000001, 0x00000006, 0x00000070,
    0x0002005B, (u32)btlevtcmd_WaitEventEnd, 0xFE363C8E, 0x00020032, 0xFE363C8D, 0x00000012,
    0x00020032, 0xFE363C8A, 0x00000000, 0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE,
    0x00000004, 0x00000000, 0x00010003, 0x0000000A, 0x00020035, 0xFE363C8A,
    0x00000001, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PCH_A1_2_802f7fac,
    0x0002001D, 0xFE363C8A, 0x00000002, 0x0006005B, (u32)btlevtcmd_CommandPreCheckDamage, 0xFFFFFFFE,
    0xFE363C83, 0xFE363C84, 0x00100100, 0xFE363C85, 0x00000020, 0x0006005B,
    (u32)btlevtcmd_CommandPreCheckDamage, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0x00000100, 0xFE363C85,
    0x00000021, 0x00010022, 0xFE363C85, 0x0001002B, 0x00000002, 0x0001002B,
    0x00000003, 0x0001002B, 0x00000006, 0x0001002B, 0x00000004, 0x0000002E,
    0x00010024, 0x00000001, 0x0000002A, 0x0001000A, 0x000003E8, 0x00000031,
    0x00020019, 0xFE363C85, 0x00000001, 0x0001005F, 0xFE363C8E, 0x00020032,
    0xFE363C8E, 0x00000000, 0x0001005B, (u32)btlevtcmd_StopAC, 0x00020018, 0xFE363C85,
    0x00000003, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000026, 0x00000021,
    0x00020018, 0xFE363C85, 0x00000006, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83,
    0x00000027, 0x00000021, 0x00020018, 0xFE363C85, 0x00000002, 0x0003005B,
    (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000028, 0x00000021, 0x00010005, 0x00000014,
    0x0005005B, (u32)btlevtcmd_AddRotate, 0xFFFFFFFE, 0x00000000, 0x00000012, 0x00000000,
    0x00010009, 0x00000001, 0x00000006, 0x00010005, 0x00000024, 0x0005005B,
    (u32)btlevtcmd_AddRotate, 0xFFFFFFFE, 0x00000000, 0x0000000A, 0x00000000, 0x00010009,
    0x00000001, 0x00000006, 0x0005005B, (u32)btlevtcmd_SetRotate, 0xFFFFFFFE, 0x00000000,
    0x00000000, 0x00000000, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001,
    (u32)(char*)str_PCH_A2_4_802f7fb8, 0x00010009, 0x00000034, 0x0001005B, (u32)evt_audience_ap_recovery, 0x0001005B,
    (u32)btlevtcmd_InviteApInfoReport, 0x00010004, 0x0000005F, 0x00000021, 0x00020032, 0xFE363C80,
    0xFE363C8A, 0x00020039, 0xFE363C80, 0x00000002, 0x00020019, 0xFE363C80,
    0x00000000, 0x00020032, 0xFE363C8C, (u32)partyWeapon_ChuchurinaNormalAttackRight, 0x00000020, 0x00020032,
    0xFE363C8C, (u32)partyWeapon_ChuchurinaNormalAttackLeft, 0x00000021, 0x0004005B, (u32)btlevtcmd_GetUnitWork, 0xFFFFFFFE,
    0x00000004, 0xFE363C8F, 0x00020019, 0xFE363C8F, 0x00000000, 0x0001005F,
    0xFE363C8F, 0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000004, 0x00000000,
    0x00000021, 0x00020019, 0xFE363C8E, 0x00000000, 0x0001005F, 0xFE363C8E,
    0x00020032, 0xFE363C8E, 0x00000000, 0x00000021, 0x0004005B, (u32)btlevtcmd_GetUnitWork,
    0xFFFFFFFE, 0x00000005, 0xFE363C80, 0x0002001D, 0xFE363C8A, 0xFE363C80,
    0x00010004, 0x00000014, 0x00000020, 0x0002005B, (u32)btlevtcmd_GetResultAC, 0xFE363C86,
    0x0002001F, 0xFE363C86, 0x00000002, 0x00010004, 0x00000014, 0x00000021,
    0x00000021, 0x00010004, 0x00000015, 0x00010003, 0x00000014, 0x00020018,
    0xFE363C8C, (u32)partyWeapon_ChuchurinaNormalAttackLeft, 0x00020032, 0xFE363C8C, (u32)partyWeapon_ChuchurinaNormalAttackLeftLast, 0x00000020,
    0x00020032, 0xFE363C8C, (u32)partyWeapon_ChuchurinaNormalAttackRightLast, 0x00000021, 0x00010003, 0x00000015,
    0x0007005B, (u32)btlevtcmd_PreCheckCounter, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0xFE363C8C,
    0x00000100, 0xFE363C80, 0x00010022, 0xFE363C80, 0x0001002B, 0x00000007,
    0x0001002B, 0x00000008, 0x0001002B, 0x00000009, 0x0001002B, 0x0000000A,
    0x0001002B, 0x0000000B, 0x0001002B, 0x0000000C, 0x0001002B, 0x0000000D,
    0x0001002B, 0x0000000E, 0x0001002B, 0x0000000F, 0x0001002B, 0x00000010,
    0x0001002B, 0x00000011, 0x00020018, 0xFE363C8C, (u32)partyWeapon_ChuchurinaNormalAttackLeft, 0x00020032,
    0xFE363C8C, (u32)partyWeapon_ChuchurinaNormalAttackLeftLast, 0x00000020, 0x00020032, 0xFE363C8C, (u32)partyWeapon_ChuchurinaNormalAttackRightLast,
    0x00000021, 0x0001006F, 0xFE363C8F, 0x00020032, 0xFE363C87, 0xFE363C8D,
    0x0005005B, (u32)btlevtcmd_GetRotate, 0xFFFFFFFE, 0xF1194D80, 0xFE363C80, 0xF1194D80,
    0x0002001D, 0xFE363C80, 0x000000B4, 0x00020036, 0xFE363C80, 0x00000168,
    0x0005005B, (u32)evt_sub_intpl_init, 0x0000000B, 0xFE363C80, 0x00000000, 0x00000004,
    0x00010005, 0x00000004, 0x0001005B, (u32)evt_sub_intpl_get_value, 0x0005005B, (u32)btlevtcmd_SetRotate,
    0xFFFFFFFE, 0x00000000, 0x00000000, 0xFE363C80, 0x00010009, 0x00000001,
    0x00020018, 0xFE363C81, 0x00000001, 0x00000007, 0x00000021, 0x00000006,
    0x0005005B, (u32)btlevtcmd_SetRotate, 0xFFFFFFFE, 0x00000000, 0x00000000, 0x00000000,
    0x00000070, 0x0002005B, (u32)btlevtcmd_WaitEventEnd, 0xFE363C8F, 0x0002005B, (u32)btlevtcmd_GetResultAC,
    0xFE363C86, 0x0002001E, 0xFE363C86, 0x00000002, 0x0007005B, (u32)btlevtcmd_CheckDamage,
    0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0xFE363C8C, 0x00020100, 0xFE363C85,
    0x00000020, 0x0007005B, (u32)btlevtcmd_CheckDamage, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84,
    0xFE363C8C, 0x00000100, 0xFE363C85, 0x00000021, 0x00000002, 0x0000002E,
    0x00000031, 0x0006005B, (u32)evt_btl_camera_shake_h, 0x00000000, 0x00000002, 0x00000000,
    0x0000000C, 0x0000000D, 0x0004005B, (u32)btlevtcmd_GetUnitWork, 0xFFFFFFFE, 0x00000005,
    0xFE363C80, 0x0002001D, 0xFE363C8A, 0xFE363C80, 0x0003005B, (u32)evt_snd_sfxon,
    (u32)(char*)str_SFX_BTL_CHUCHURINA_A_802f7fc4, 0x00000000, 0x0002001E, 0xFE363C86, 0x00000002, 0x0007005B,
    (u32)btlevtcmd_CheckDamage, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0xFE363C8C, 0x00020100,
    0xFE363C85, 0x00000020, 0x0007005B, (u32)btlevtcmd_CheckDamage, 0xFFFFFFFE, 0xFE363C83,
    0xFE363C84, 0xFE363C8C, 0x00000100, 0xFE363C85, 0x00000021, 0x00000020,
    0x00020032, 0xFE363C80, 0xFE363C8A, 0x0002004D, 0xFE363C80, 0x00000001,
    0x00020018, 0xFE363C80, 0x00000000, 0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_BTL_CHUCHURINA_A_802f7fe0,
    0x00000000, 0x00000020, 0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_BTL_CHUCHURINA_A_802f7ffc, 0x00000000,
    0x00000021, 0x0002001E, 0xFE363C86, 0x00000002, 0x0007005B, (u32)btlevtcmd_CheckDamage,
    0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0xFE363C8C, 0x00020000, 0xFE363C85,
    0x00000020, 0x0007005B, (u32)btlevtcmd_CheckDamage, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84,
    0xFE363C8C, 0x00000100, 0xFE363C85, 0x00000021, 0x00000021, 0x0002005B,
    (u32)btlevtcmd_GetResultAC, 0xFE363C86, 0x0002001E, 0xFE363C86, 0x00000002, 0x0003005B,
    (u32)btlevtcmd_AcGetOutputParam, 0x00000002, 0xFE363C80, 0x00020036, 0xFE363C80, 0x00000002,
    0x0002001D, 0xFE363C80, 0x00000000, 0x0004005B, (u32)btlevtcmd_GetResultPrizeLv, 0xFE363C83,
    0xFE363C80, 0xFE363C8B, 0x0006005B, (u32)btlevtcmd_GetHitPos, 0xFE363C83, 0xFE363C84,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)btlevtcmd_ACSuccessEffect, 0xFE363C8B,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult, 0xFE363C8C,
    0xFE363C8B, 0x00000020, 0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult, 0xFE363C8C, 0xFFFFFFFF,
    0x00000021, 0x00000020, 0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult, 0xFE363C8C, 0xFFFFFFFF,
    0x00000021, 0x0002001E, 0xFE363C86, 0x00000002, 0x00010022, 0xFE363C8A,
    0x00010028, 0x00000002, 0x0001006F, 0xFE363C8F, 0x00020032, 0xFE363C87,
    0xFE363C8D, 0x0002001D, 0xFE363C87, 0x00000000, 0x00020032, 0xFE363C88,
    0x0000004B, 0x00000020, 0x00020032, 0xFE363C88, 0xFFFFFFB5, 0x00000021,
    0x0005005B, (u32)btlevtcmd_GetRotate, 0xFFFFFFFE, 0xF1194D80, 0xFE363C80, 0xF1194D80,
    0x0002001D, 0xFE363C80, 0x000000B4, 0x00020036, 0xFE363C80, 0x00000168,
    0x00000021, 0x0005005B, (u32)evt_sub_intpl_init, 0x00000005, 0xFE363C80, 0xFE363C88,
    0x0000000E, 0x00010005, 0x0000000E, 0x0001005B, (u32)evt_sub_intpl_get_value, 0x0005005B,
    (u32)btlevtcmd_SetRotate, 0xFFFFFFFE, 0x00000000, 0xFE363C80, 0x00000000, 0x00010009,
    0x00000001, 0x00000006, 0x0004005B, (u32)btlevtcmd_GetUnitWork, 0xFFFFFFFE, 0x00000005,
    0xFE363C80, 0x0002001A, 0xFE363C8A, 0xFE363C80, 0x00020032, 0xFE363C87,
    0xFE363C8D, 0x0002001D, 0xFE363C87, 0x00000000, 0x00020032, 0xFE363C88,
    0x0000004B, 0x00020032, 0xFE363C89, 0x0000004B, 0x00000020, 0x00020032,
    0xFE363C88, 0xFFFFFFB5, 0x00020032, 0xFE363C89, 0xFFFFFFB5, 0x00000021,
    0x0005005B, (u32)evt_sub_intpl_init, 0x0000000B, 0xFE363C89, 0xFE363C88, 0x00000006,
    0x00010005, 0x00000006, 0x0001005B, (u32)evt_sub_intpl_get_value, 0x0005005B, (u32)btlevtcmd_SetRotate,
    0xFFFFFFFE, 0x00000000, 0xFE363C80, 0x00000000, 0x00010009, 0x00000001,
    0x00000006, 0x00000020, 0x00020032, 0xFE363C87, 0xFE363C8D, 0x0002001D,
    0xFE363C87, 0x00000000, 0x00020032, 0xFE363C88, 0x00000000, 0x00020032,
    0xFE363C89, 0x0000004B, 0x00000020, 0x00020032, 0xFE363C88, 0x00000000,
    0x00020032, 0xFE363C89, 0xFFFFFFB5, 0x00000021, 0x0005005B, (u32)evt_sub_intpl_init,
    0x0000000B, 0xFE363C89, 0xFE363C88, 0x0000000C, 0x00010005, 0x0000000C,
    0x0001005B, (u32)evt_sub_intpl_get_value, 0x0005005B, (u32)btlevtcmd_SetRotate, 0xFFFFFFFE, 0x00000000,
    0xFE363C80, 0x00000000, 0x00010009, 0x00000001, 0x00000006, 0x00000021,
    0x00000070, 0x0002005B, (u32)btlevtcmd_WaitEventEnd, 0xFE363C8F, 0x0000002A, 0x0001006F,
    0xFE363C8F, 0x0005005B, (u32)btlevtcmd_GetRotate, 0xFFFFFFFE, 0xF1194D80, 0xFE363C88,
    0xF1194D80, 0x00020032, 0xFE363C87, 0x00000168, 0x0004005B, (u32)btlevtcmd_GetUnitWork,
    0xFFFFFFFE, 0x00000005, 0xFE363C80, 0x0002001D, 0xFE363C8A, 0xFE363C80,
    0x00020032, 0xFE363C80, 0xFE363C88, 0x0002001D, 0xFE363C88, 0x000000B4,
    0x00020036, 0xFE363C80, 0x00000168, 0x00000021, 0x0002001D, 0xFE363C8D,
    0x00000000, 0x00020036, 0xFE363C87, 0xFE363C80, 0x00000020, 0x00020035,
    0xFE363C87, 0xFE363C80, 0x00000021, 0x00000020, 0x00020035, 0xFE363C87,
    0x0000003C, 0x00020032, 0xFE363C80, 0xFE363C88, 0x0002001D, 0xFE363C88,
    0x000000B4, 0x00020036, 0xFE363C80, 0x00000168, 0x00000021, 0x0002001D,
    0xFE363C8D, 0x00000000, 0x00020036, 0xFE363C87, 0xFE363C80, 0x00000020,
    0x00020035, 0xFE363C87, 0xFE363C80, 0x00000021, 0x00000021, 0x0002001A,
    0xFE363C8D, 0x00000000, 0x00020037, 0xFE363C87, 0xFFFFFFFF, 0x00000021,
    0x0005005B, (u32)evt_sub_intpl_init, 0x00000005, 0x00000000, 0xFE363C87, 0x00000014,
    0x00010005, 0x00000014, 0x0001005B, (u32)evt_sub_intpl_get_value, 0x00020035, 0xFE363C80,
    0xFE363C88, 0x0005005B, (u32)btlevtcmd_SetRotate, 0xFFFFFFFE, 0x00000000, 0xFE363C80,
    0x00000000, 0x00010009, 0x00000001, 0x00000006, 0x00000070, 0x0004005B,
    (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000004, 0xFE363C8F, 0x00010009, 0x0000000A,
    0x00000031, 0x00000020, 0x0001006F, 0xFE363C8F, 0x0005005B, (u32)btlevtcmd_GetRotate,
    0xFFFFFFFE, 0xF1194D80, 0xFE363C88, 0xF1194D80, 0x00020032, 0xFE363C87,
    0x000002D0, 0x0002001D, 0xFE363C8A, 0x00000001, 0x00020032, 0xFE363C80,
    0xFE363C88, 0x0002001D, 0xFE363C88, 0x000000B4, 0x00020036, 0xFE363C80,
    0x00000168, 0x00000021, 0x00020036, 0xFE363C87, 0xFE363C80, 0x00000021,
    0x0002001A, 0xFE363C8D, 0x00000000, 0x00020037, 0xFE363C87, 0xFFFFFFFF,
    0x00000021, 0x0005005B, (u32)evt_sub_intpl_init, 0x00000000, 0x00000000, 0xFE363C87,
    0x0000003C, 0x00010005, 0x0000003C, 0x0001005B, (u32)evt_sub_intpl_get_value, 0x00020035,
    0xFE363C80, 0xFE363C88, 0x0005005B, (u32)btlevtcmd_SetRotate, 0xFFFFFFFE, 0x00000000,
    0xFE363C80, 0x00000000, 0x00010009, 0x00000001, 0x00000006, 0x00000070,
    0x0002005B, (u32)btlevtcmd_WaitEventEnd, 0xFE363C8F, 0x0005005B, (u32)btlevtcmd_SetRotate, 0xFFFFFFFE,
    0x00000000, 0x00000000, 0x00000000, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE,
    0x00000001, (u32)(char*)str_PCH_X_1_802f7d38, 0x00010009, 0x0000003C, 0x0001005B, (u32)btlevtcmd_StopAC,
    0x0001005B, (u32)evt_audience_ap_recovery, 0x0001005B, (u32)btlevtcmd_InviteApInfoReport, 0x00010004, 0x0000005F,
    0x00000021, 0x00020037, 0xFE363C8D, 0xFFFFFFFF, 0x0004005B, (u32)btlevtcmd_GetUnitWork,
    0xFFFFFFFE, 0x00000005, 0xFE363C80, 0x0002001A, 0xFE363C8A, 0xFE363C80,
    0x00010004, 0x0000000A, 0x00000021, 0x00010003, 0x0000005A, 0x0001005B,
    (u32)btlevtcmd_StopAC, 0x0001005B, (u32)evt_audience_ap_recovery, 0x0001005B, (u32)btlevtcmd_InviteApInfoReport, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PCH_A1_3_802f8018, 0x00010009, 0x00000028,
    0x00010003, 0x0000005F, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000,
    0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001, 0x0005005B, (u32)btlevtcmd_GetHomePos,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0007005B, (u32)btlevtcmd_CalculateFaceDirection,
    0xFFFFFFFE, 0xFFFFFFFF, 0xFE363C80, 0xFE363C82, 0x00000001, 0xFE363C8F,
    0x0003005B, (u32)btlevtcmd_ChangeFaceDirection, 0xFFFFFFFE, 0xFE363C8F, 0x0003005B, (u32)btlevtcmd_SetMoveSpeed,
    0xFFFFFFFE, 0xF24A8680, 0x0007005B, (u32)btlevtcmd_GetMoveFrame, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000000, 0xFE363C8E, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType,
    0xFFFFFFFE, 0x00000001, 0x0000002A, 0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0001006F, 0xFE363C8F, 0x0008005B,
    (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0xFE363C8E,
    0xFFFFFFFF, 0x00000000, 0x00000070, 0x00020032, 0xFE363C8B, 0xFE363C8E,
    0x00020036, 0xFE363C8B, 0x0000000B, 0x00020036, 0xFE363C8E, 0x00000001,
    0x0002001B, 0xFE363C8E, 0xFE363C8B, 0x00020032, 0xFE363C8D, 0xFE363C8E,
    0x00020036, 0xFE363C8D, 0x0000000A, 0x0002001E, 0xFE363C86, 0x00000002,
    0x0006005B, (u32)btlevtcmd_ACRStart, 0xFFFFFFFE, 0x00000000, 0xFE363C8D, 0xFE363C8E,
    0x00000000, 0x0003005B, (u32)btlevtcmd_ACRGetResult, 0xFE363C86, 0xFE363C87, 0x0002001D,
    0xFE363C86, 0x00000002, 0x0001005F, 0xFE363C8F, 0x0005005B, (u32)btlevtcmd_GetPos,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0003005B, (u32)btlevtcmd_SetFallAccel,
    0xFFFFFFFE, 0xF24A7C80, 0x0004005B, (u32)btlevtcmd_FaceDirectionAdd, 0xFFFFFFFE, 0xFE363C80,
    0x0000001E, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PCH_O_1_802f7d98,
    0x0006005B, (u32)btlevtcmd_AudienceDeclareAcrobatResult, 0xFE363C8C, 0x00000001, 0x00000000, 0x00000000,
    0x00000000, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000014, 0xFFFFFFFF, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE,
    0x00000001, 0x0000002A, 0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE, 0xF24A8680,
    0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00000000, 0xFFFFFFFF, 0x00000000, 0x0005005B, (u32)btlevtcmd_SetHomePos, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000020, 0x0002005B, (u32)btlevtcmd_WaitEventEnd,
    0xFE363C8F, 0x00000021, 0x00000020, 0x0002005B, (u32)btlevtcmd_WaitEventEnd, 0xFE363C8F,
    0x00000021, 0x00000020, 0x0002005B, (u32)btlevtcmd_WaitEventEnd, 0xFE363C8F, 0x00000021,
    0x00010003, 0x00000063, 0x0002005B, (u32)btlevtcmd_ResetFaceDirection, 0xFFFFFFFE, 0x0001005E,
    (u32)btldefaultevt_SuitoruBadgeEffect, 0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002, 0x00000001,
};

u32 partyChuchurinaAttack_MadowaseAttack[] = {
    0x0003005B, (u32)btlevtcmd_GetSelectEnemy, 0xFE363C83, 0xFE363C84, 0x00020018, 0xFE363C83,
    0xFFFFFFFF, 0x00010004, 0x00000063, 0x00000021, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress,
    0xFFFFFFFE, 0xFE363C8C, 0x0002005B, (u32)btlevtcmd_WeaponAftereffect, 0xFE363C8C, 0x0002005B,
    (u32)btlevtcmd_AttackDeclareAll, 0xFFFFFFFE, 0x0001005B, (u32)btlevtcmd_WaitGuardMove, 0x0002005B, (u32)btlevtcmd_CommandPayWeaponCost,
    0xFFFFFFFE, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFFFFFFFE, 0x00000007, 0x0002005B,
    (u32)btlevtcmd_CommandGetWeaponActionLv, 0xFE363C80, 0x0003005B, (u32)btlevtcmd_AcSetDifficulty, 0xFFFFFFFE, 0xFE363C80,
    0x0002005B, (u32)btlevtcmd_AcSetFlag, 0x00000029, 0x0002005B, (u32)btlevtcmd_AcGetDifficulty, 0xFE363C80,
    0x00020036, 0xFE363C80, 0x00000003, 0x00010022, 0xFE363C80, 0x00010028,
    0xFFFFFFFD, 0x00020032, 0xFE363C80, 0x00000003, 0x00020032, 0xFE363C81,
    0x00000032, 0x00010024, 0xFFFFFFFE, 0x00020032, 0xFE363C80, 0x00000004,
    0x00020032, 0xFE363C81, 0x00000050, 0x00010024, 0xFFFFFFFF, 0x00020032,
    0xFE363C80, 0x00000005, 0x00020032, 0xFE363C81, 0x00000064, 0x00010024,
    0x00000000, 0x00020032, 0xFE363C80, 0x00000007, 0x00020032, 0xFE363C81,
    0x00000064, 0x00010024, 0x00000001, 0x00020032, 0xFE363C80, 0x00000008,
    0x00020032, 0xFE363C81, 0x0000006E, 0x0002005B, (u32)btlevtcmd_AcSetFlag, 0x00000031,
    0x00010024, 0x00000002, 0x00020032, 0xFE363C80, 0x0000000A, 0x00020032,
    0xFE363C81, 0x00000078, 0x0002005B, (u32)btlevtcmd_AcSetFlag, 0x00000031, 0x0000002A,
    0x00020032, 0xFE363C80, 0x0000000C, 0x00020032, 0xFE363C81, 0x00000082,
    0x0002005B, (u32)btlevtcmd_AcSetFlag, 0x00000031, 0x00000031, 0x0009005B, (u32)btlevtcmd_AcSetParamAll,
    0x00000001, 0x000000F0, 0x000000B2, 0xFE363C80, 0xFE363C81, 0x00000064,
    0x00000001, 0xF1194D80, 0x0005005B, (u32)btlevtcmd_AcSetGaugeParam, 0x00000022, 0x00000044,
    0x00000064, 0x00000064, 0x0005005B, (u32)btlevtcmd_SetupAC, 0xFFFFFFFE, 0x00000006,
    0x00000001, 0x00000000, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80,
    0xF1194D80, 0xF1194D80, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000003,
    0x000A005B, (u32)evt_btl_camera_set_moveto, 0x00000001, 0xFE363C80, 0x00000049, 0x00000190,
    0xFE363C80, 0x0000002B, 0x00000000, 0x00000028, 0x00000005, 0x00010009,
    0x00000016, 0x0002005B, (u32)btlevtcmd_StartAC, 0x00000001, 0x0001005B, (u32)btlevtcmd_ResultAC,
    0x0001005B, (u32)btlevtcmd_StopAC, 0x00010009, 0x00000014, 0x0004005B, (u32)btlevtcmd_AnimeChangePose,
    0xFFFFFFFE, 0x00000001, (u32)(char*)str_PCH_I_1_802f7d88, 0x00010005, 0x00000028, 0x0005005B,
    (u32)btlevtcmd_AddRotate, 0xFFFFFFFE, 0x00000000, 0x00000024, 0x00000000, 0x00010009,
    0x00000001, 0x00000006, 0x0005005B, (u32)btlevtcmd_SetRotate, 0xFFFFFFFE, 0x00000000,
    0x00000000, 0x00000000, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001,
    (u32)(char*)str_PCH_A4_1_802f8024, 0x0007005B, (u32)btlevtcmd_SetWalkSound, 0xFFFFFFFE, (u32)(char*)str_SFX_BTL_CHURINA_CONF_802f8030, (u32)(char*)str_SFX_BTL_CHURINA_CONF_802f8050,
    0x00000000, 0x0000000F, 0x0000000D, 0x0007005B, (u32)btlevtcmd_SetRunSound, 0xFFFFFFFE,
    (u32)(char*)str_SFX_BTL_CHURINA_CONF_802f8030, (u32)(char*)str_SFX_BTL_CHURINA_CONF_802f8050, 0x00000000, 0x00000007, 0x00000007, 0x0007005B,
    (u32)btlevtcmd_SetDiveSound, 0xFFFFFFFE, (u32)(char*)str_SFX_BTL_CHURINA_CONF_802f8030, (u32)(char*)str_SFX_BTL_CHURINA_CONF_802f8050, 0x00000000, 0x00000007,
    0x00000007, 0x0004005B, (u32)btlevtcmd_SetJumpSound, 0xFFFFFFFE, (u32)(char*)str_SFX_BTL_CHURINA_CONF_802f8070, (u32)(char*)str_SFX_BTL_CHURINA_CONF_802f8090,
    0x0008005B, (u32)btlevtcmd_SetPartsWalkSound, 0xFFFFFFFE, 0x00000002, (u32)(char*)str_SFX_BTL_CHURINA_CONF_802f8030, (u32)(char*)str_SFX_BTL_CHURINA_CONF_802f8050,
    0x00000000, 0x0000000F, 0x0000000D, 0x0008005B, (u32)btlevtcmd_SetPartsRunSound, 0xFFFFFFFE,
    0x00000002, (u32)(char*)str_SFX_BTL_CHURINA_CONF_802f8030, (u32)(char*)str_SFX_BTL_CHURINA_CONF_802f8050, 0x00000000, 0x00000007, 0x00000007,
    0x0008005B, (u32)btlevtcmd_SetPartsDiveSound, 0xFFFFFFFE, 0x00000002, (u32)(char*)str_SFX_BTL_CHURINA_CONF_802f8030, (u32)(char*)str_SFX_BTL_CHURINA_CONF_802f8050,
    0x00000000, 0x00000007, 0x00000007, 0x0005005B, (u32)btlevtcmd_SetPartsJumpSound, 0xFFFFFFFE,
    0x00000002, (u32)(char*)str_SFX_BTL_CHURINA_CONF_802f8070, (u32)(char*)str_SFX_BTL_CHURINA_CONF_802f8090, 0x0008005B, (u32)btlevtcmd_SetPartsWalkSound, 0xFFFFFFFE,
    0x00000003, (u32)(char*)str_SFX_BTL_CHURINA_CONF_802f8030, (u32)(char*)str_SFX_BTL_CHURINA_CONF_802f8050, 0x00000000, 0x0000000F, 0x0000000D,
    0x0008005B, (u32)btlevtcmd_SetPartsRunSound, 0xFFFFFFFE, 0x00000003, (u32)(char*)str_SFX_BTL_CHURINA_CONF_802f8030, (u32)(char*)str_SFX_BTL_CHURINA_CONF_802f8050,
    0x00000000, 0x00000007, 0x00000007, 0x0008005B, (u32)btlevtcmd_SetPartsDiveSound, 0xFFFFFFFE,
    0x00000003, (u32)(char*)str_SFX_BTL_CHURINA_CONF_802f8030, (u32)(char*)str_SFX_BTL_CHURINA_CONF_802f8050, 0x00000000, 0x00000007, 0x00000007,
    0x0005005B, (u32)btlevtcmd_SetPartsJumpSound, 0xFFFFFFFE, 0x00000003, (u32)(char*)str_SFX_BTL_CHURINA_CONF_802f8070, (u32)(char*)str_SFX_BTL_CHURINA_CONF_802f8090,
    0x0008005B, (u32)btlevtcmd_SetPartsWalkSound, 0xFFFFFFFE, 0x00000004, (u32)(char*)str_SFX_BTL_CHURINA_CONF_802f8030, (u32)(char*)str_SFX_BTL_CHURINA_CONF_802f8050,
    0x00000000, 0x0000000F, 0x0000000D, 0x0008005B, (u32)btlevtcmd_SetPartsRunSound, 0xFFFFFFFE,
    0x00000004, (u32)(char*)str_SFX_BTL_CHURINA_CONF_802f8030, (u32)(char*)str_SFX_BTL_CHURINA_CONF_802f8050, 0x00000000, 0x00000007, 0x00000007,
    0x0008005B, (u32)btlevtcmd_SetPartsDiveSound, 0xFFFFFFFE, 0x00000004, (u32)(char*)str_SFX_BTL_CHURINA_CONF_802f8030, (u32)(char*)str_SFX_BTL_CHURINA_CONF_802f8050,
    0x00000000, 0x00000007, 0x00000007, 0x0005005B, (u32)btlevtcmd_SetPartsJumpSound, 0xFFFFFFFE,
    0x00000004, (u32)(char*)str_SFX_BTL_CHURINA_CONF_802f8070, (u32)(char*)str_SFX_BTL_CHURINA_CONF_802f8090, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00020035, 0xFE363C81, 0x00000028,
    0x00020036, 0xFE363C82, 0x00000014, 0x0004005B, (u32)btlevtcmd_FaceDirectionAdd, 0xFFFFFFFE,
    0xFE363C80, 0x00000014, 0x0006005B, (u32)btlevtcmd_SetPartsPos, 0xFFFFFFFE, 0x00000002,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0006005B, (u32)btlevtcmd_SetPartsRotate, 0xFFFFFFFE,
    0x00000002, 0x00000000, 0x000000B4, 0x00000000, 0x0004005B, (u32)btlevtcmd_AnimeChangePose,
    0xFFFFFFFE, 0x00000002, (u32)(char*)str_PCH_A3_1_802f80b4, 0x0004005B, (u32)btlevtcmd_OffPartsAttribute, 0xFFFFFFFE,
    0x00000002, 0x03000000, 0x000F005B, (u32)evt_eff, (u32)zero_80424110, (u32)(char*)str_bomb_80424114,
    0x00000000, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0xF24A7E80, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_BTL_CHURINA_CONF_802f80c0, 0xF1194D80, 0x00000000,
    0xF1194D80, 0x0000006E, 0x00010009, 0x0000001E, 0x0004005B, (u32)btlevtcmd_SetPartsFallAccel,
    0xFFFFFFFE, 0x00000002, 0xF24A7BB3, 0x0006005B, (u32)btlevtcmd_GetPartsPos, 0xFFFFFFFE,
    0x00000002, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B, (u32)btlevtcmd_TransStageFloorPosition,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0007005B, (u32)btlevtcmd_FallPartsPosition, 0xFFFFFFFE,
    0x00000002, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000019, 0x0006005B,
    (u32)btlevtcmd_SetPartsRotate, 0xFFFFFFFE, 0x00000002, 0x00000000, 0x00000000, 0x00000000,
    0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000002, 0x0000002B, 0x00000070,
    0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00020035, 0xFE363C81, 0x00000014, 0x00020036, 0xFE363C82, 0x0000000A,
    0x0004005B, (u32)btlevtcmd_FaceDirectionAdd, 0xFFFFFFFE, 0xFE363C80, 0x0000000A, 0x0006005B,
    (u32)btlevtcmd_SetPartsPos, 0xFFFFFFFE, 0x00000003, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0006005B, (u32)btlevtcmd_SetPartsRotate, 0xFFFFFFFE, 0x00000003, 0x00000000, 0x00000000,
    0x00000000, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000003, (u32)(char*)str_PCH_A3_2_802f80dc,
    0x0004005B, (u32)btlevtcmd_OffPartsAttribute, 0xFFFFFFFE, 0x00000003, 0x03000000, 0x000F005B,
    (u32)evt_eff, (u32)zero_80424110, (u32)(char*)str_bomb_80424114, 0x00000000, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0xF24A7E80, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x0000006E, 0x00010009, 0x0000001E,
    0x0004005B, (u32)btlevtcmd_SetPartsFallAccel, 0xFFFFFFFE, 0x00000003, 0xF24A7BB3, 0x0006005B,
    (u32)btlevtcmd_GetPartsPos, 0xFFFFFFFE, 0x00000003, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0004005B, (u32)btlevtcmd_TransStageFloorPosition, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0007005B,
    (u32)btlevtcmd_FallPartsPosition, 0xFFFFFFFE, 0x00000003, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00000019, 0x0006005B, (u32)btlevtcmd_SetPartsRotate, 0xFFFFFFFE, 0x00000003, 0x00000000,
    0x00000000, 0x00000000, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000003,
    0x0000002B, 0x00000070, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00020035, 0xFE363C81, 0x0000001E, 0x00020035,
    0xFE363C82, 0x0000000A, 0x0004005B, (u32)btlevtcmd_FaceDirectionAdd, 0xFFFFFFFE, 0xFE363C80,
    0xFFFFFFF6, 0x0006005B, (u32)btlevtcmd_SetPartsPos, 0xFFFFFFFE, 0x00000004, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0006005B, (u32)btlevtcmd_SetPartsRotate, 0xFFFFFFFE, 0x00000004,
    0x00000000, 0x000000B4, 0x00000000, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE,
    0x00000004, (u32)(char*)str_PCH_A3_3_802f80e8, 0x0004005B, (u32)btlevtcmd_OffPartsAttribute, 0xFFFFFFFE, 0x00000004,
    0x03000000, 0x000F005B, (u32)evt_eff, (u32)zero_80424110, (u32)(char*)str_bomb_80424114, 0x00000000,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0xF24A7E80, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0000006E,
    0x00010009, 0x0000001E, 0x0004005B, (u32)btlevtcmd_SetPartsFallAccel, 0xFFFFFFFE, 0x00000004,
    0xF24A7BB3, 0x0006005B, (u32)btlevtcmd_GetPartsPos, 0xFFFFFFFE, 0x00000004, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0004005B, (u32)btlevtcmd_TransStageFloorPosition, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0007005B, (u32)btlevtcmd_FallPartsPosition, 0xFFFFFFFE, 0x00000004, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000019, 0x0006005B, (u32)btlevtcmd_SetPartsRotate, 0xFFFFFFFE,
    0x00000004, 0x00000000, 0x00000000, 0x00000000, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType,
    0xFFFFFFFE, 0x00000004, 0x0000002B, 0x00000070, 0x00010009, 0x00000055,
    0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001, 0x0000002B, 0x0004005B,
    (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000000, 0x00000001, 0x0004005B, (u32)btlevtcmd_SetUnitWork,
    0xFFFFFFFE, 0x00000001, 0x00000001, 0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE,
    0x00000002, 0x00000001, 0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000003,
    0x00000001, 0x000C005B, (u32)btlevtcmd_SetPartsBlur, 0xFFFFFFFE, 0x00000001, 0x000000FF,
    0x000000FF, 0x000000FF, 0x000000FF, 0x00000064, 0x00000064, 0x00000064,
    0x00000032, 0x00000001, 0x0004005B, (u32)btlevtcmd_OnPartsAttribute, 0xFFFFFFFE, 0x00000001,
    0x04000000, 0x000C005B, (u32)btlevtcmd_SetPartsBlur, 0xFFFFFFFE, 0x00000002, 0x000000FF,
    0x000000FF, 0x000000FF, 0x000000FF, 0x00000064, 0x00000064, 0x00000064,
    0x00000032, 0x00000001, 0x0004005B, (u32)btlevtcmd_OnPartsAttribute, 0xFFFFFFFE, 0x00000002,
    0x04000000, 0x000C005B, (u32)btlevtcmd_SetPartsBlur, 0xFFFFFFFE, 0x00000003, 0x000000FF,
    0x000000FF, 0x000000FF, 0x000000FF, 0x00000064, 0x00000064, 0x00000064,
    0x00000032, 0x00000001, 0x0004005B, (u32)btlevtcmd_OnPartsAttribute, 0xFFFFFFFE, 0x00000003,
    0x04000000, 0x000C005B, (u32)btlevtcmd_SetPartsBlur, 0xFFFFFFFE, 0x00000004, 0x000000FF,
    0x000000FF, 0x000000FF, 0x000000FF, 0x00000064, 0x00000064, 0x00000064,
    0x00000032, 0x00000001, 0x0004005B, (u32)btlevtcmd_OnPartsAttribute, 0xFFFFFFFE, 0x00000004,
    0x04000000, 0x0000006E, 0x00010009, 0x00000028, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType,
    0xFFFFFFFE, 0x00000001, 0x0000002A, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B, (u32)btlevtcmd_GetStageSize, 0xFE363C85,
    0xF1194D80, 0xF1194D80, 0x00020038, 0xFE363C85, 0x00000003, 0x00020032,
    0xFE363C80, 0x00000000, 0x0004005B, (u32)btlevtcmd_FaceDirectionAdd, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C85, 0x0007005B, (u32)btlevtcmd_GetMoveFrame, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0xF24A8680, 0xFE363C87, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000,
    0x00000003, 0x000A005B, (u32)evt_btl_camera_set_moveto, 0x00000001, 0xFE363C80, 0x0000005A,
    0x000001F4, 0xFE363C80, 0x0000003C, 0x00000000, 0xFE363C87, 0x00000000,
    0x00000070, 0x0000006E, 0x0003005B, (u32)btlevtcmd_SetFallAccel, 0xFFFFFFFE, 0xF24A7D4C,
    0x00010009, 0x00000028, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001,
    0x0000002A, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0004005B, (u32)btlevtcmd_GetStageSize, 0xFE363C85, 0xF1194D80, 0xF1194D80,
    0x00020038, 0xFE363C85, 0x00000003, 0x00020032, 0xFE363C80, 0x00000000,
    0x0004005B, (u32)btlevtcmd_FaceDirectionAdd, 0xFFFFFFFE, 0xFE363C80, 0xFE363C85, 0x0003005B,
    (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE, 0xF24A8680, 0x000A005B, (u32)btlevtcmd_DivePosition, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000, 0x00000000, 0x00000002,
    0x00000000, 0xFFFFFFFF, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00020035, 0xFE363C80, 0x00000028, 0x0007005B,
    (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000014,
    0xFFFFFFFF, 0x00020036, 0xFE363C80, 0x0000003C, 0x0005005B, (u32)btlevtcmd_SetRotate,
    0xFFFFFFFE, 0x00000000, 0x000000B4, 0x00000000, 0x0007005B, (u32)btlevtcmd_JumpPosition,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000014, 0xFFFFFFFF,
    0x00020035, 0xFE363C80, 0x00000028, 0x0005005B, (u32)btlevtcmd_SetRotate, 0xFFFFFFFE,
    0x00000000, 0x00000000, 0x00000000, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000014, 0xFFFFFFFF, 0x00020036,
    0xFE363C80, 0x0000003C, 0x0005005B, (u32)btlevtcmd_SetRotate, 0xFFFFFFFE, 0x00000000,
    0x000000B4, 0x00000000, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000014, 0xFFFFFFFF, 0x00020035, 0xFE363C80,
    0x00000028, 0x0005005B, (u32)btlevtcmd_SetRotate, 0xFFFFFFFE, 0x00000000, 0x00000000,
    0x00000000, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000014, 0xFFFFFFFF, 0x000F005B, (u32)evt_eff, (u32)zero_80424110,
    (u32)(char*)str_bomb_80424114, 0x00000000, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0xF24A7E80,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x0004005B, (u32)btlevtcmd_OffPartsAttribute, 0xFFFFFFFE, 0x00000001, 0x04000000,
    0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000000, 0x00000000, 0x00000070,
    0x0000006E, 0x00020032, 0xFE363C86, 0x00000002, 0x0004005B, (u32)btlevtcmd_SetPartsFallAccel,
    0xFFFFFFFE, 0xFE363C86, 0xF24A7D4C, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE,
    0xFE363C86, 0x0000002A, 0x0006005B, (u32)btlevtcmd_GetPartsPos, 0xFFFFFFFE, 0xFE363C86,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B, (u32)btlevtcmd_GetStageSize, 0xFE363C85,
    0xF1194D80, 0xF1194D80, 0x00020038, 0xFE363C85, 0x00000003, 0x00020032,
    0xFE363C80, 0x00000000, 0x0004005B, (u32)btlevtcmd_FaceDirectionAdd, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C85, 0x0004005B, (u32)btlevtcmd_SetPartsMoveSpeed, 0xFFFFFFFE, 0xFE363C86, 0xF24A8680,
    0x000B005B, (u32)btlevtcmd_DivePartsPosition, 0xFFFFFFFE, 0xFE363C86, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000000, 0x00000000, 0x00000002, 0x00000000, 0xFFFFFFFF,
    0x0006005B, (u32)btlevtcmd_GetPartsPos, 0xFFFFFFFE, 0xFE363C86, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00020035, 0xFE363C80, 0x0000003C, 0x0008005B, (u32)btlevtcmd_JumpPartsPosition,
    0xFFFFFFFE, 0xFE363C86, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000019,
    0xFFFFFFFF, 0x00020036, 0xFE363C80, 0x0000005A, 0x0006005B, (u32)btlevtcmd_SetPartsRotate,
    0xFFFFFFFE, 0xFE363C86, 0x00000000, 0x000000B4, 0x00000000, 0x0008005B,
    (u32)btlevtcmd_JumpPartsPosition, 0xFFFFFFFE, 0xFE363C86, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00000019, 0xFFFFFFFF, 0x00020035, 0xFE363C80, 0x0000003C, 0x0006005B,
    (u32)btlevtcmd_SetPartsRotate, 0xFFFFFFFE, 0xFE363C86, 0x00000000, 0x00000000, 0x00000000,
    0x0008005B, (u32)btlevtcmd_JumpPartsPosition, 0xFFFFFFFE, 0xFE363C86, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000019, 0xFFFFFFFF, 0x00020036, 0xFE363C80, 0x0000005A,
    0x0006005B, (u32)btlevtcmd_SetPartsRotate, 0xFFFFFFFE, 0xFE363C86, 0x00000000, 0x000000B4,
    0x00000000, 0x0008005B, (u32)btlevtcmd_JumpPartsPosition, 0xFFFFFFFE, 0xFE363C86, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000019, 0xFFFFFFFF, 0x00020036, 0xFE363C80,
    0x0000001E, 0x0006005B, (u32)btlevtcmd_SetPartsRotate, 0xFFFFFFFE, 0xFE363C86, 0x00000000,
    0x000000B4, 0x00000000, 0x0008005B, (u32)btlevtcmd_JumpPartsPosition, 0xFFFFFFFE, 0xFE363C86,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000019, 0xFFFFFFFF, 0x00020035,
    0xFE363C80, 0x0000001E, 0x0006005B, (u32)btlevtcmd_SetPartsRotate, 0xFFFFFFFE, 0xFE363C86,
    0x00000000, 0x00000000, 0x00000000, 0x0008005B, (u32)btlevtcmd_JumpPartsPosition, 0xFFFFFFFE,
    0xFE363C86, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000019, 0xFFFFFFFF,
    0x000F005B, (u32)evt_eff, (u32)zero_80424110, (u32)(char*)str_bomb_80424114, 0x00000000, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0xF24A7E80, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0004005B, (u32)btlevtcmd_OnPartsAttribute,
    0xFFFFFFFE, 0x00000002, 0x03000000, 0x0004005B, (u32)btlevtcmd_OffPartsAttribute, 0xFFFFFFFE,
    0x00000002, 0x04000000, 0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000001,
    0x00000000, 0x00000070, 0x0000006E, 0x00020032, 0xFE363C86, 0x00000003,
    0x0004005B, (u32)btlevtcmd_SetPartsFallAccel, 0xFFFFFFFE, 0xFE363C86, 0xF24A7D4C, 0x00010009,
    0x00000014, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0xFE363C86, 0x0000002A,
    0x0006005B, (u32)btlevtcmd_GetPartsPos, 0xFFFFFFFE, 0xFE363C86, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0004005B, (u32)btlevtcmd_GetStageSize, 0xFE363C85, 0xF1194D80, 0xF1194D80,
    0x00020038, 0xFE363C85, 0x00000003, 0x00020032, 0xFE363C80, 0x00000000,
    0x0004005B, (u32)btlevtcmd_FaceDirectionAdd, 0xFFFFFFFE, 0xFE363C80, 0xFE363C85, 0x0004005B,
    (u32)btlevtcmd_SetPartsMoveSpeed, 0xFFFFFFFE, 0xFE363C86, 0xF24A8680, 0x000B005B, (u32)btlevtcmd_DivePartsPosition,
    0xFFFFFFFE, 0xFE363C86, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000,
    0x00000000, 0x00000002, 0x00000000, 0xFFFFFFFF, 0x0006005B, (u32)btlevtcmd_GetPartsPos,
    0xFFFFFFFE, 0xFE363C86, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00020036,
    0xFE363C80, 0x00000050, 0x0006005B, (u32)btlevtcmd_SetPartsRotate, 0xFFFFFFFE, 0xFE363C86,
    0x00000000, 0x000000B4, 0x00000000, 0x0008005B, (u32)btlevtcmd_JumpPartsPosition, 0xFFFFFFFE,
    0xFE363C86, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000019, 0xFFFFFFFF,
    0x00020035, 0xFE363C80, 0x000000A0, 0x0006005B, (u32)btlevtcmd_SetPartsRotate, 0xFFFFFFFE,
    0xFE363C86, 0x00000000, 0x00000000, 0x00000000, 0x0008005B, (u32)btlevtcmd_JumpPartsPosition,
    0xFFFFFFFE, 0xFE363C86, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000028,
    0xFFFFFFFF, 0x00020036, 0xFE363C80, 0x00000028, 0x0006005B, (u32)btlevtcmd_SetPartsRotate,
    0xFFFFFFFE, 0xFE363C86, 0x00000000, 0x000000B4, 0x00000000, 0x0008005B,
    (u32)btlevtcmd_JumpPartsPosition, 0xFFFFFFFE, 0xFE363C86, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0000000F, 0xFFFFFFFF, 0x00020036, 0xFE363C80, 0x0000003C, 0x0006005B,
    (u32)btlevtcmd_SetPartsRotate, 0xFFFFFFFE, 0xFE363C86, 0x00000000, 0x000000B4, 0x00000000,
    0x0008005B, (u32)btlevtcmd_JumpPartsPosition, 0xFFFFFFFE, 0xFE363C86, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000014, 0xFFFFFFFF, 0x00020035, 0xFE363C80, 0x00000014,
    0x0006005B, (u32)btlevtcmd_SetPartsRotate, 0xFFFFFFFE, 0xFE363C86, 0x00000000, 0x00000000,
    0x00000000, 0x0008005B, (u32)btlevtcmd_JumpPartsPosition, 0xFFFFFFFE, 0xFE363C86, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000014, 0xFFFFFFFF, 0x000F005B, (u32)evt_eff,
    (u32)zero_80424110, (u32)(char*)str_bomb_80424114, 0x00000000, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0xF24A7E80, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x0004005B, (u32)btlevtcmd_OnPartsAttribute, 0xFFFFFFFE, 0x00000003,
    0x03000000, 0x0004005B, (u32)btlevtcmd_OffPartsAttribute, 0xFFFFFFFE, 0x00000003, 0x04000000,
    0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000002, 0x00000000, 0x00000070,
    0x0000006E, 0x00020032, 0xFE363C86, 0x00000004, 0x0004005B, (u32)btlevtcmd_SetPartsFallAccel,
    0xFFFFFFFE, 0xFE363C86, 0xF24A7D4C, 0x00010009, 0x0000003C, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0xFE363C86, 0x0000002A, 0x0006005B, (u32)btlevtcmd_GetPartsPos,
    0xFFFFFFFE, 0xFE363C86, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B,
    (u32)btlevtcmd_GetStageSize, 0xFE363C85, 0xF1194D80, 0xF1194D80, 0x00020038, 0xFE363C85,
    0x00000003, 0x00020032, 0xFE363C80, 0x00000000, 0x0004005B, (u32)btlevtcmd_FaceDirectionAdd,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C85, 0x0004005B, (u32)btlevtcmd_SetPartsMoveSpeed, 0xFFFFFFFE,
    0xFE363C86, 0xF24A8680, 0x000B005B, (u32)btlevtcmd_DivePartsPosition, 0xFFFFFFFE, 0xFE363C86,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000, 0x00000000, 0x00000002,
    0x00000000, 0xFFFFFFFF, 0x0006005B, (u32)btlevtcmd_GetPartsPos, 0xFFFFFFFE, 0xFE363C86,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00020035, 0xFE363C80, 0x0000001E,
    0x0008005B, (u32)btlevtcmd_JumpPartsPosition, 0xFFFFFFFE, 0xFE363C86, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0000000A, 0xFFFFFFFF, 0x00020035, 0xFE363C80, 0x0000001E,
    0x0006005B, (u32)btlevtcmd_SetPartsRotate, 0xFFFFFFFE, 0xFE363C86, 0x00000000, 0x00000000,
    0x00000000, 0x0008005B, (u32)btlevtcmd_JumpPartsPosition, 0xFFFFFFFE, 0xFE363C86, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0000000A, 0xFFFFFFFF, 0x00020036, 0xFE363C80,
    0x0000001E, 0x0006005B, (u32)btlevtcmd_SetPartsRotate, 0xFFFFFFFE, 0xFE363C86, 0x00000000,
    0x000000B4, 0x00000000, 0x0008005B, (u32)btlevtcmd_JumpPartsPosition, 0xFFFFFFFE, 0xFE363C86,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0000000A, 0xFFFFFFFF, 0x00020036,
    0xFE363C80, 0x0000001E, 0x0006005B, (u32)btlevtcmd_SetPartsRotate, 0xFFFFFFFE, 0xFE363C86,
    0x00000000, 0x000000B4, 0x00000000, 0x0008005B, (u32)btlevtcmd_JumpPartsPosition, 0xFFFFFFFE,
    0xFE363C86, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0000000A, 0xFFFFFFFF,
    0x00020036, 0xFE363C80, 0x0000001E, 0x0006005B, (u32)btlevtcmd_SetPartsRotate, 0xFFFFFFFE,
    0xFE363C86, 0x00000000, 0x000000B4, 0x00000000, 0x0008005B, (u32)btlevtcmd_JumpPartsPosition,
    0xFFFFFFFE, 0xFE363C86, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0000000A,
    0xFFFFFFFF, 0x00020036, 0xFE363C80, 0x0000001E, 0x0006005B, (u32)btlevtcmd_SetPartsRotate,
    0xFFFFFFFE, 0xFE363C86, 0x00000000, 0x000000B4, 0x00000000, 0x0008005B,
    (u32)btlevtcmd_JumpPartsPosition, 0xFFFFFFFE, 0xFE363C86, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0000000A, 0xFFFFFFFF, 0x00020035, 0xFE363C80, 0x0000001E, 0x0006005B,
    (u32)btlevtcmd_SetPartsRotate, 0xFFFFFFFE, 0xFE363C86, 0x00000000, 0x00000000, 0x00000000,
    0x0008005B, (u32)btlevtcmd_JumpPartsPosition, 0xFFFFFFFE, 0xFE363C86, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0000000A, 0xFFFFFFFF, 0x00020036, 0xFE363C80, 0x0000001E,
    0x0006005B, (u32)btlevtcmd_SetPartsRotate, 0xFFFFFFFE, 0xFE363C86, 0x00000000, 0x000000B4,
    0x00000000, 0x0008005B, (u32)btlevtcmd_JumpPartsPosition, 0xFFFFFFFE, 0xFE363C86, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0000000A, 0xFFFFFFFF, 0x000F005B, (u32)evt_eff,
    (u32)zero_80424110, (u32)(char*)str_bomb_80424114, 0x00000000, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0xF24A7E80, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x0004005B, (u32)btlevtcmd_OnPartsAttribute, 0xFFFFFFFE, 0x00000004,
    0x03000000, 0x0004005B, (u32)btlevtcmd_OffPartsAttribute, 0xFFFFFFFE, 0x00000004, 0x04000000,
    0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000003, 0x00000000, 0x00000070,
    0x00010003, 0x00000009, 0x00020032, 0xFE363C80, 0x00000000, 0x0004005B,
    (u32)btlevtcmd_GetUnitWork, 0xFFFFFFFE, 0x00000000, 0xFE363C81, 0x00020035, 0xFE363C80,
    0xFE363C81, 0x0004005B, (u32)btlevtcmd_GetUnitWork, 0xFFFFFFFE, 0x00000001, 0xFE363C81,
    0x00020035, 0xFE363C80, 0xFE363C81, 0x0004005B, (u32)btlevtcmd_GetUnitWork, 0xFFFFFFFE,
    0x00000002, 0xFE363C81, 0x00020035, 0xFE363C80, 0xFE363C81, 0x0004005B,
    (u32)btlevtcmd_GetUnitWork, 0xFFFFFFFE, 0x00000003, 0xFE363C81, 0x00020035, 0xFE363C80,
    0xFE363C81, 0x00020019, 0xFE363C80, 0x00000000, 0x00010009, 0x00000001,
    0x00010004, 0x00000009, 0x00000021, 0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_BTL_CHURINA_CONF_802f80c0,
    0x00000000, 0x00020032, 0xFE363C8A, 0x00000000, 0x00020032, 0xFE363C8F,
    0x00000000, 0x00010003, 0x0000000A, 0x0006005B, (u32)btlevtcmd_CommandPreCheckDamage, 0xFFFFFFFE,
    0xFE363C83, 0xFE363C84, 0x00000100, 0xFE363C86, 0x00020019, 0xFE363C86,
    0x00000001, 0x00020018, 0xFE363C86, 0x00000003, 0x0003005B, (u32)btlevtcmd_StartAvoid,
    0xFE363C83, 0x00000026, 0x00000021, 0x00020018, 0xFE363C86, 0x00000006,
    0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000027, 0x00000021, 0x00020018,
    0xFE363C86, 0x00000002, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000028,
    0x00000021, 0x00010004, 0x00000032, 0x00000021, 0x0003005B, (u32)btlevtcmd_AcGetOutputParam,
    0x00000001, 0xFE363C80, 0x0003005B, (u32)_make_madowase_weapon, 0xFE363C8C, 0xFE363C80,
    0x0002005B, (u32)btlevtcmd_GetResultAC, 0xFE363C80, 0x0002001E, 0xFE363C80, 0x00000002,
    0x0007005B, (u32)btlevtcmd_CheckDamage, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0xFE363C8C,
    0x00020100, 0xFE363C85, 0x0004005B, (u32)btlevtcmd_GetResultPrizeLv, 0xFE363C83, 0x00000000,
    0xFE363C86, 0x0006005B, (u32)btlevtcmd_GetHitPos, 0xFE363C83, 0xFE363C84, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0002001D, 0xFE363C86, 0x00000000, 0x0005005B,
    (u32)btlevtcmd_ACSuccessEffect, 0xFE363C86, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00020035,
    0xFE363C8F, 0x00000001, 0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult, 0xFE363C8C, 0xFE363C86,
    0x00000021, 0x00000020, 0x0007005B, (u32)btlevtcmd_CheckDamage, 0xFFFFFFFE, 0xFE363C83,
    0xFE363C84, 0xFE363C8C, 0x00000100, 0xFE363C85, 0x00000021, 0x00010003,
    0x00000032, 0x0003005B, (u32)btlevtcmd_GetSelectNextEnemy, 0xFE363C83, 0xFE363C84, 0x00020019,
    0xFE363C83, 0xFFFFFFFF, 0x00020035, 0xFE363C8A, 0x00000001, 0x00010004,
    0x0000000A, 0x00000021, 0x00020018, 0xFE363C8F, 0x00000000, 0x0003005B,
    (u32)btlevtcmd_AudienceDeclareACResult, 0xFE363C8C, 0xFFFFFFFF, 0x00000021, 0x00010003, 0x00000050,
    0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv,
    0x00000000, 0x00000001, 0x0002005B, (u32)btlevtcmd_ResetFaceDirection, 0xFFFFFFFE, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PCH_B_1_802f80f4, 0x0005005B, (u32)btlevtcmd_GetPos,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub,
    0xFFFFFFFE, 0xFE363C80, 0x00000050, 0x0003005B, (u32)btlevtcmd_SetFallAccel, 0xFFFFFFFE,
    0xF24A7B19, 0x0000006E, 0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_BTL_CHURINA_CONF_802f80fc,
    0xF1194D80, 0x00000000, 0xF1194D80, 0x0005005B, (u32)btlevtcmd_SetRotateOffset, 0xFFFFFFFE,
    0x00000000, 0x00000005, 0x00000000, 0x0003005B, (u32)btlevtcmd_GetFaceDirection, 0xFFFFFFFE,
    0xFE363C85, 0x00020037, 0xFE363C85, 0x00000009, 0x00010005, 0x00000028,
    0x0005005B, (u32)btlevtcmd_AddRotate, 0xFFFFFFFE, 0x00000000, 0x00000000, 0xFE363C85,
    0x00010009, 0x00000001, 0x00000006, 0x0003005B, (u32)btlevtcmd_GetFaceDirection, 0xFFFFFFFE,
    0xFE363C85, 0x00020037, 0xFE363C85, 0x00000024, 0x00010005, 0x00000014,
    0x0005005B, (u32)btlevtcmd_AddRotate, 0xFFFFFFFE, 0x00000000, 0x00000000, 0xFE363C85,
    0x00010009, 0x00000001, 0x00000006, 0x0005005B, (u32)btlevtcmd_SetRotate, 0xFFFFFFFE,
    0x00000000, 0x00000000, 0x00000000, 0x00000070, 0x0000006E, 0x0006005B,
    (u32)btlevtcmd_ACRStart, 0xFFFFFFFE, 0x00000028, 0x0000003B, 0x0000003B, 0x00000000,
    0x00000070, 0x0007005B, (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0000003C, 0xFFFFFFFF, 0x0003005B, (u32)btlevtcmd_ACRGetResult, 0xFE363C86,
    0xFE363C87, 0x00010022, 0xFE363C86, 0x00010029, 0x00000002, 0x0006005B,
    (u32)btlevtcmd_AudienceDeclareAcrobatResult, 0xFE363C8C, 0x00000001, 0x00000000, 0x00000000, 0x00000000,
    0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PCH_Y_1_802f8118, 0x0003005B,
    (u32)btlevtcmd_AnimeWaitPlayComplete, 0xFFFFFFFE, 0x00000001, 0x0001000A, 0x0000012C, 0x0001005B,
    (u32)evt_audience_ap_recovery, 0x0001005B, (u32)btlevtcmd_InviteApInfoReport, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000,
    0x00000000, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001, 0x0000002A, 0x0005005B, (u32)btlevtcmd_AddRotate,
    0xFFFFFFFE, 0x00000000, 0x000000B4, 0x00000000, 0x0005005B, (u32)btlevtcmd_GetHomePos,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000A005B, (u32)btlevtcmd_DivePosition,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000, 0x00000000,
    0x00000002, 0x00000000, 0xFFFFFFFF, 0x0005005B, (u32)btlevtcmd_SetRotate, 0xFFFFFFFE,
    0x00000000, 0x00000000, 0x00000000, 0x0000002A, 0x0005005B, (u32)btlevtcmd_SetRotate,
    0xFFFFFFFE, 0x00000000, 0x00000000, 0x00000000, 0x0001005B, (u32)evt_audience_acrobat_notry,
    0x0001005B, (u32)evt_audience_ap_recovery, 0x0001005B, (u32)btlevtcmd_InviteApInfoReport, 0x0003005B, (u32)evt_btl_camera_set_mode,
    0x00000000, 0x00000000, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001,
    0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PCH_R_2_802f8120, 0x0005005B,
    (u32)btlevtcmd_AddRotate, 0xFFFFFFFE, 0x00000000, 0x000000B4, 0x00000000, 0x0005005B,
    (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000A005B,
    (u32)btlevtcmd_DivePosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000,
    0x00000000, 0x00000002, 0x00000000, 0xFFFFFFFF, 0x0005005B, (u32)btlevtcmd_SetRotate,
    0xFFFFFFFE, 0x00000000, 0x00000000, 0x00000000, 0x00000031, 0x00010003,
    0x00000063, 0x0002005B, (u32)btlevtcmd_ResetFaceDirection, 0xFFFFFFFE, 0x0001005E, (u32)btldefaultevt_SuitoruBadgeEffect,
    0x0007005B, (u32)btlevtcmd_SetWalkSound, 0xFFFFFFFE, (u32)(char*)str_SFX_PARTY_BATTLE_MOV_802f7e58, (u32)(char*)str_SFX_PARTY_BATTLE_MOV_802f7e70, 0x00000000,
    0x0000000F, 0x0000000D, 0x0007005B, (u32)btlevtcmd_SetRunSound, 0xFFFFFFFE, (u32)(char*)str_SFX_PARTY_BATTLE_MOV_802f7e58,
    (u32)(char*)str_SFX_PARTY_BATTLE_MOV_802f7e70, 0x00000000, 0x00000007, 0x00000007, 0x0007005B, (u32)btlevtcmd_SetDiveSound,
    0xFFFFFFFE, (u32)(char*)str_SFX_PARTY_BATTLE_MOV_802f7e58, (u32)(char*)str_SFX_PARTY_BATTLE_MOV_802f7e70, 0x00000000, 0x00000007, 0x00000007,
    0x0004005B, (u32)btlevtcmd_SetJumpSound, 0xFFFFFFFE, (u32)(char*)str_SFX_PARTY_BATTLE_JUM_802f7e88, (u32)(char*)str_SFX_PARTY_BATTLE_LAN_802f7ea0, 0x0002005B,
    (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002, 0x00000001,
};

u32 partyChuchurinaAttack_ItemSteal[] = {
    0x0003005B, (u32)btlevtcmd_GetSelectEnemy, 0xFE363C83, 0xFE363C84, 0x00020018, 0xFE363C83,
    0xFFFFFFFF, 0x00010004, 0x00000063, 0x00000021, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress,
    0xFFFFFFFE, 0xFE363C8C, 0x0002005B, (u32)btlevtcmd_WeaponAftereffect, 0xFE363C8C, 0x0002005B,
    (u32)btlevtcmd_AttackDeclareAll, 0xFFFFFFFE, 0x0001005B, (u32)btlevtcmd_WaitGuardMove, 0x0002005B, (u32)btlevtcmd_CommandPayWeaponCost,
    0xFFFFFFFE, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFFFFFFFE, 0x00000007, 0x00020032,
    0xFE363C8E, 0x00000000, 0x0007005B, (u32)btlevtcmd_PreCheckDamage, 0xFFFFFFFE, 0xFE363C83,
    0xFE363C84, (u32)partyWeapon_ChuchurinaItemSteal, 0x00000100, 0xFE363C85, 0x00020018, 0xFE363C85,
    0x00000005, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001, 0x0000002A,
    0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE, 0xF24A8680, 0x0003005B, (u32)btlevtcmd_GetWidth,
    0xFE363C83, 0xFE363C8F, 0x00020038, 0xFE363C8F, 0x00000002, 0x0006005B,
    (u32)btlevtcmd_GetHitPos, 0xFE363C83, 0xFE363C84, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0004005B, (u32)btlevtcmd_FaceDirectionAdd, 0xFE363C83, 0xFE363C80, 0xFE363C8F, 0x0004005B,
    (u32)btlevtcmd_FaceDirectionAdd, 0xFE363C83, 0xFE363C80, 0x00000014, 0x0004005B, (u32)btlevtcmd_TransStageFloorPosition,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0007005B, (u32)btlevtcmd_GetMoveFrame, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000, 0xFE363C88, 0x0001006F,
    0xFE363C89, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0xFE363C88, 0xFFFFFFFF, 0x00000000, 0x00000070, 0x0002001B,
    0xFE363C88, 0x00000005, 0x00020036, 0xFE363C88, 0x00000002, 0x00020032,
    0xFE363C8F, 0xFE363C88, 0x00020038, 0xFE363C8F, 0x00000003, 0x00020032,
    0xFE363C88, 0xFE363C8F, 0x00020037, 0xFE363C88, 0x00000002, 0x0006005B,
    (u32)btlevtcmd_ACRStart, 0xFFFFFFFE, 0xFE363C8F, 0xFE363C88, 0xFE363C88, 0x00000000,
    0x0003005B, (u32)btlevtcmd_ACRGetResult, 0xFE363C86, 0xFE363C87, 0x00010022, 0xFE363C86,
    0x00010029, 0x00000002, 0x0001005F, 0xFE363C89, 0x0006005B, (u32)btlevtcmd_AudienceDeclareAcrobatResult,
    0xFE363C8C, 0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PCH_Y_1_802f8118, 0x0003005B, (u32)btlevtcmd_AnimeWaitPlayComplete,
    0xFFFFFFFE, 0x00000001, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001,
    0x0000002A, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000000, 0xFFFFFFFF, 0x00000000, 0x0000002A, 0x0001005B,
    (u32)evt_audience_acrobat_notry, 0x00000031, 0x00000020, 0x0002005B, (u32)btlevtcmd_WaitEventEnd, 0xFE363C89,
    0x00000021, 0x0007005B, (u32)btlevtcmd_CheckDamage, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84,
    (u32)partyWeapon_ChuchurinaItemSteal, 0x00000100, 0xFE363C85, 0x00000002, 0x00000021, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001, 0x0000002A, 0x0003005B, (u32)btlevtcmd_SetMoveSpeed,
    0xFFFFFFFE, 0xF24A8680, 0x0003005B, (u32)btlevtcmd_GetWidth, 0xFE363C83, 0xFE363C8F,
    0x00020037, 0xFE363C8F, 0x00000002, 0x00020038, 0xFE363C8F, 0x00000003,
    0x00020035, 0xFE363C8F, 0x0000000A, 0x0006005B, (u32)btlevtcmd_GetHitPos, 0xFE363C83,
    0xFE363C84, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B, (u32)btlevtcmd_FaceDirectionAdd,
    0xFE363C83, 0xFE363C80, 0xFE363C8F, 0x00020035, 0xFE363C82, 0x00000008,
    0x0004005B, (u32)btlevtcmd_TransStageFloorPosition, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0007005B,
    (u32)btlevtcmd_GetMoveFrame, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000,
    0xFE363C88, 0x0001006F, 0xFE363C89, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0xFE363C88, 0xFFFFFFFF, 0x00000000,
    0x00000070, 0x0002001B, 0xFE363C88, 0x00000005, 0x00020036, 0xFE363C88,
    0x00000002, 0x00020032, 0xFE363C8F, 0xFE363C88, 0x00020038, 0xFE363C8F,
    0x00000003, 0x00020032, 0xFE363C88, 0xFE363C8F, 0x00020037, 0xFE363C88,
    0x00000002, 0x0006005B, (u32)btlevtcmd_ACRStart, 0xFFFFFFFE, 0xFE363C8F, 0xFE363C88,
    0xFE363C88, 0x00000000, 0x0003005B, (u32)btlevtcmd_ACRGetResult, 0xFE363C86, 0xFE363C87,
    0x00010022, 0xFE363C86, 0x00010029, 0x00000002, 0x0001005F, 0xFE363C89,
    0x0006005B, (u32)btlevtcmd_AudienceDeclareAcrobatResult, 0xFE363C8C, 0x00000001, 0x00000000, 0x00000000,
    0x00000000, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PCH_Y_1_802f8118,
    0x0003005B, (u32)btlevtcmd_AnimeWaitPlayComplete, 0xFFFFFFFE, 0x00000001, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType,
    0xFFFFFFFE, 0x00000001, 0x0000002A, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000, 0xFFFFFFFF, 0x00000000,
    0x0000002A, 0x0001005B, (u32)evt_audience_acrobat_notry, 0x0002005B, (u32)btlevtcmd_WaitEventEnd, 0xFE363C89,
    0x00000031, 0x00000020, 0x0002005B, (u32)btlevtcmd_WaitEventEnd, 0xFE363C89, 0x00000021,
    0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000007, 0x0004005B, (u32)evt_btl_camera_set_homing_unit,
    0x00000000, 0xFFFFFFFE, 0x00000001, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000,
    0x00000002, 0x0003005B, (u32)evt_btl_camera_set_zoom, 0x00000000, 0x0000015E, 0x0001000A,
    0x000000C8, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PCH_K_1_802f8128,
    0x00010009, 0x00000006, 0x00020019, 0xFE363C85, 0x00000001, 0x00010009,
    0x00000014, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001, 0x0000002B,
    0x00010022, 0xFE363C85, 0x0001002B, 0x00000004, 0x0000002E, 0x00010024,
    0x00000003, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000026, 0x00010024,
    0x00000006, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000027, 0x00010024,
    0x00000002, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C83, 0x00000028, 0x00010024,
    0x00000001, 0x0000002A, 0x00000031, 0x00010009, 0x0000001E, 0x0001005B,
    (u32)btlevtcmd_StopAC, 0x00010004, 0x0000005A, 0x00000021, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv,
    0x00000000, 0x00000002, 0x0003005B, (u32)evt_btl_camera_set_zoom, 0x00000000, 0x0000015E,
    0x0007005B, (u32)btlevtcmd_CheckDamage, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84, (u32)partyWeapon_ChuchurinaItemSteal,
    0x00000100, 0xFE363C85, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0003005B, (u32)btlevtcmd_GetHeight, 0xFFFFFFFE, 0xFE363C8F,
    0x00020037, 0xFE363C8F, 0x00000002, 0x00020038, 0xFE363C8F, 0x00000003,
    0x00020035, 0xFE363C81, 0xFE363C8F, 0x0004005B, (u32)btlevtcmd_FaceDirectionAdd, 0xFFFFFFFE,
    0xFE363C80, 0x00000014, 0x000F005B, (u32)evt_eff, (u32)zero_80424110, (u32)(char*)str_kiss_8042411c,
    0x00000000, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000001, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_BTL_CHURINA_KISS_802f8130, 0xF1194D80, 0x00000000,
    0xF1194D80, 0x0003005B, (u32)btlevtcmd_OnUnitFlag, 0xFE363C83, 0x02000000, 0x00010009,
    0x00000014, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PCH_Z_1_802f8148,
    0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000003, 0x0005005B, (u32)btlevtcmd_AnimeFlagOnOff,
    0xFFFFFFFE, 0x00000001, 0x00000100, 0x00000001, 0x00010009, 0x00000001,
    0x0001005B, (u32)mono_capture_event, 0x0002005D, (u32)main_evt, 0xFE363C8D, 0x0000006B,
    0x0001005B, (u32)mono_on, 0x0000006D, 0x0002005B, (u32)btlevtcmd_CommandGetWeaponActionLv, 0xFE363C80,
    0x0003005B, (u32)btlevtcmd_AcSetDifficulty, 0xFFFFFFFE, 0xFE363C80, 0x0004005B, (u32)_get_itemsteal_param,
    0xFE363C83, 0xFE363C80, 0xFE363C81, 0x0009005B, (u32)btlevtcmd_AcSetParamAll, 0x00000001,
    0xFE363C80, 0x00000002, 0x00000002, 0xFE363C81, 0xF1194D80, 0xF1194D80,
    0xF1194D80, 0x0002005B, (u32)btlevtcmd_AcSetFlag, 0x00000000, 0x0005005B, (u32)btlevtcmd_SetupAC,
    0xFFFFFFFE, 0x0000000F, 0x00000001, 0x00000000, 0x00010009, 0x00000016,
    0x0002005B, (u32)btlevtcmd_StartAC, 0x00000001, 0x0001005B, (u32)btlevtcmd_ResultAC, 0x0002005B,
    (u32)btlevtcmd_GetResultAC, 0xFE363C86, 0x0002001E, 0xFE363C86, 0x00000002, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PCH_A4_1_802f8024, 0x00000021, 0x00010009,
    0x00000023, 0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_BTL_CHURINA_EYE1_802f8150, 0x00000000, 0x00010009,
    0x0000002F, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFE363C83, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0003005B, (u32)btlevtcmd_GetWidth, 0xFE363C83, 0xFE363C8F, 0x00020038,
    0xFE363C8F, 0x00000002, 0x00020035, 0xFE363C80, 0xFE363C8F, 0x0003005B,
    (u32)btlevtcmd_GetHeight, 0xFE363C83, 0xFE363C8F, 0x00020035, 0xFE363C81, 0xFE363C8F,
    0x00020035, 0xFE363C81, 0x00000005, 0x00020035, 0xFE363C82, 0x00000008,
    0x0003005B, (u32)btlevtcmd_SetFallAccel, 0xFFFFFFFE, 0xF24A7D4C, 0x0007005B, (u32)btlevtcmd_JumpPosition,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000010, 0xFFFFFFFF,
    0x0002005B, (u32)btlevtcmd_GetResultAC, 0xFE363C86, 0x0004005B, (u32)_chuchu_item_steal, 0xFE363C83,
    0xFE363C8E, 0xFE363C86, 0x00020018, 0xFE363C8E, 0x00000000, 0x0002005B,
    (u32)btlevtcmd_JumpContinue, 0xFFFFFFFE, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001,
    (u32)(char*)str_PCH_F_1_802f8168, 0x00010009, 0x00000016, 0x0000006B, 0x0001005B, (u32)mono_off,
    0x0001005F, 0xFE363C8D, 0x0000006D, 0x00010009, 0x00000014, 0x0003005B,
    (u32)btlevtcmd_OffUnitFlag, 0xFE363C83, 0x02000000, 0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult, 0xFE363C8C,
    0xFFFFFFFF, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000, 0x0003005B,
    (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001, 0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE,
    0xF24A8680, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PCH_R_2_802f8120,
    0x0005005B, (u32)btlevtcmd_AddRotate, 0xFFFFFFFE, 0x00000000, 0x000000B4, 0x00000000,
    0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x000A005B, (u32)btlevtcmd_DivePosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00000000, 0x00000000, 0x00000002, 0x00000000, 0xFFFFFFFF, 0x0005005B,
    (u32)btlevtcmd_SetRotate, 0xFFFFFFFE, 0x00000000, 0x00000000, 0x00000000, 0x00010004,
    0x00000063, 0x00000021, 0x0004005B, (u32)btlevtcmd_GetResultPrizeLv, 0x00000000, 0x00000000,
    0xFE363C86, 0x0006005B, (u32)btlevtcmd_GetHitPos, 0xFE363C83, 0xFE363C84, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0005005B, (u32)btlevtcmd_ACSuccessEffect, 0xFE363C86, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult, 0xFE363C8C, 0xFE363C86,
    0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_BTL_CHURINA_HEAR_802f8170, 0x00000000, 0x0003005B, (u32)btlevtcmd_OffUnitFlag,
    0xFE363C83, 0x02000000, 0x0000006B, 0x0001005B, (u32)mono_off, 0x0001005F,
    0xFE363C8D, 0x0000006D, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0004005B, (u32)btlevtcmd_FaceDirectionSub, 0xFFFFFFFE, 0xFE363C80,
    0x00000032, 0x0004005B, (u32)btlevtcmd_TransStageFloorPosition, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0003005B, (u32)btlevtcmd_SetFallAccel, 0xFFFFFFFE, 0xF24A7BB3, 0x0000006B, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PCH_A3_1_802f80b4, 0x0005005B, (u32)btlevtcmd_SetRotateOffset,
    0xFFFFFFFE, 0x00000000, 0x0000000A, 0x00000000, 0x00010005, 0x00000012,
    0x0005005B, (u32)btlevtcmd_AddRotate, 0xFFFFFFFE, 0x00000000, 0x00000000, 0x00000014,
    0x00010009, 0x00000001, 0x00000006, 0x0005005B, (u32)btlevtcmd_SetRotate, 0xFFFFFFFE,
    0x00000000, 0x00000000, 0x00000000, 0x0005005B, (u32)btlevtcmd_SetRotateOffset, 0xFFFFFFFE,
    0x00000000, 0x00000000, 0x00000000, 0x0000006D, 0x0007005B, (u32)btlevtcmd_JumpPosition,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0000001E, 0xFFFFFFFF,
    0x00010003, 0x0000005A, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000,
    0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001, 0x00020019, 0xFE363C8E,
    0x00000000, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0003005B, (u32)btlevtcmd_GetHeight, 0xFFFFFFFE, 0xFE363C85, 0x00020035,
    0xFE363C81, 0xFE363C85, 0x00020035, 0xFE363C81, 0x00000005, 0x0006005B,
    (u32)btlevtcmd_BtlIconEntryItemId, 0xFE363C8E, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0xFE363C88,
    0x0001006F, 0xFE363C8F, 0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE, 0xF24A8680,
    0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001, 0x0000002A, 0x0005005B,
    (u32)btlevtcmd_AddRotate, 0xFFFFFFFE, 0x00000000, 0x000000B4, 0x00000000, 0x0005005B,
    (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000A005B,
    (u32)btlevtcmd_DivePosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000,
    0x00000000, 0x00000002, 0x00000000, 0xFFFFFFFF, 0x0005005B, (u32)btlevtcmd_SetRotate,
    0xFFFFFFFE, 0x00000000, 0x00000000, 0x00000000, 0x00000070, 0x00010003,
    0x0000005B, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0003005B, (u32)btlevtcmd_GetHeight, 0xFFFFFFFE, 0xFE363C85, 0x00020035,
    0xFE363C81, 0xFE363C85, 0x00020035, 0xFE363C81, 0x00000005, 0x0005005B,
    (u32)btlevtcmd_BtlIconSetPosition, 0xFE363C88, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0002006A,
    0xFE363C8F, 0xFE363C80, 0x00020019, 0xFE363C80, 0x00000000, 0x00010009,
    0x00000001, 0x00010004, 0x0000005B, 0x00000021, 0x0004005B, (u32)btlevtcmd_AnimeChangePose,
    0xFFFFFFFE, 0x00000001, (u32)(char*)str_PCH_A2_2_802f818c, 0x00010009, 0x00000004, 0x0003005B,
    (u32)btlevtcmd_AnnounceSetParam, 0x00000000, 0xFE363C8E, 0x0006005B, (u32)btlevtcmd_AnnounceMessage, 0x00000001,
    0x00000000, 0x00000000, (u32)(char*)str_btl_msg_steal_item_g_802f8198, 0x0000005A, 0x0002005B, (u32)btlevtcmd_BtlIconDelete,
    0xFE363C88, 0x00000020, 0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE, 0xF24A8680,
    0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001, 0x0000002A, 0x0005005B,
    (u32)btlevtcmd_AddRotate, 0xFFFFFFFE, 0x00000000, 0x000000B4, 0x00000000, 0x0005005B,
    (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000A005B,
    (u32)btlevtcmd_DivePosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000,
    0x00000000, 0x00000002, 0x00000000, 0xFFFFFFFF, 0x0005005B, (u32)btlevtcmd_SetRotate,
    0xFFFFFFFE, 0x00000000, 0x00000000, 0x00000000, 0x00000021, 0x00010003,
    0x00000063, 0x0001005B, (u32)evt_audience_ap_recovery, 0x0001005B, (u32)btlevtcmd_InviteApInfoReport, 0x0002005B,
    (u32)btlevtcmd_ResetFaceDirection, 0xFFFFFFFE, 0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002,
    0x00000001,
};

u32 partyChuchurinaAttack_Kiss[] = {
    0x0003005B, (u32)btlevtcmd_GetSelectEnemy, 0xFE363C83, 0xFE363C84, 0x00020018, 0xFE363C83,
    0xFFFFFFFF, 0x00010004, 0x00000063, 0x00000021, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress,
    0xFFFFFFFE, 0xFE363C8C, 0x0002005B, (u32)btlevtcmd_WeaponAftereffect, 0xFE363C8C, 0x0002005B,
    (u32)btlevtcmd_CommandPayWeaponCost, 0xFFFFFFFE, 0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFFFFFFFE, 0x00000007,
    0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000007, 0x0004005B, (u32)evt_btl_camera_set_homing_unit,
    0x00000000, 0xFFFFFFFE, 0xFFFFFFFE, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000,
    0x00000001, 0x0003005B, (u32)evt_btl_camera_set_zoom, 0x00000000, 0x000000FA, 0x0003005B,
    (u32)btlevtcmd_GetUnitId, 0xFFFFFFFE, 0xFE363C80, 0x00020019, 0xFE363C80, 0xFE363C83,
    0x0007005B, (u32)btlevtcmd_CalculateFaceDirection, 0xFFFFFFFE, 0xFFFFFFFF, 0xFE363C83, 0xFE363C84,
    0x00000010, 0xFE363C8F, 0x0003005B, (u32)btlevtcmd_ChangeFaceDirection, 0xFFFFFFFE, 0xFE363C8F,
    0x00000021, 0x0001005E, (u32)unk_evt_803537c4, 0x00020019, 0xFE363C80, 0x00000000,
    0x0007005B, (u32)btlevtcmd_CalculateFaceDirection, 0xFE363C83, 0xFE363C84, 0xFFFFFFFE, 0xFFFFFFFF,
    0x00000010, 0xFE363C8F, 0x0003005B, (u32)btlevtcmd_ChangeFaceDirection, 0xFE363C83, 0xFE363C8F,
    0x00000021, 0x0002005B, (u32)btlevtcmd_CommandGetWeaponActionLv, 0xFE363C80, 0x0003005B, (u32)btlevtcmd_AcSetDifficulty,
    0xFFFFFFFE, 0xFE363C80, 0x0002005B, (u32)btlevtcmd_AcGetDifficulty, 0xFE363C80, 0x00020036,
    0xFE363C80, 0x00000003, 0x00010022, 0xFE363C80, 0x00010028, 0xFFFFFFFD,
    0x00020032, 0xFE363C80, 0x0000012C, 0x00020032, 0xFE363C81, 0x00000014,
    0x00020032, 0xFE363C82, 0x00000014, 0x00010024, 0xFFFFFFFE, 0x00020032,
    0xFE363C80, 0x0000010E, 0x00020032, 0xFE363C81, 0x00000012, 0x00020032,
    0xFE363C82, 0x00000016, 0x00010024, 0xFFFFFFFF, 0x00020032, 0xFE363C80,
    0x000000F0, 0x00020032, 0xFE363C81, 0x00000011, 0x00020032, 0xFE363C82,
    0x00000018, 0x00010024, 0x00000000, 0x00020032, 0xFE363C80, 0x000000D2,
    0x00020032, 0xFE363C81, 0x00000010, 0x00020032, 0xFE363C82, 0x00000019,
    0x00010024, 0x00000001, 0x00020032, 0xFE363C80, 0x000000C3, 0x00020032,
    0xFE363C81, 0x0000000E, 0x00020032, 0xFE363C82, 0x0000001A, 0x00010024,
    0x00000002, 0x00020032, 0xFE363C80, 0x000000B4, 0x00020032, 0xFE363C81,
    0x0000000D, 0x00020032, 0xFE363C82, 0x0000001B, 0x0000002A, 0x00020032,
    0xFE363C80, 0x00000096, 0x00020032, 0xFE363C81, 0x0000000C, 0x00020032,
    0xFE363C82, 0x0000001E, 0x00000031, 0x0009005B, (u32)btlevtcmd_AcSetParamAll, 0x0000000B,
    0xFE363C80, 0x000000B2, 0xFE363C81, 0xFE363C82, 0x0000000C, 0x00000025,
    0xF1194D80, 0x0005005B, (u32)btlevtcmd_AcSetGaugeParam, 0x00000024, 0x00000048, 0x00000064,
    0x00000064, 0x0002005B, (u32)btlevtcmd_AcSetFlag, 0x00000040, 0x0005005B, (u32)btlevtcmd_SetupAC,
    0xFFFFFFFE, 0x00000006, 0x00000001, 0x00000000, 0x00010009, 0x00000016,
    0x0002005B, (u32)btlevtcmd_StartAC, 0x00000001, 0x0000006E, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType,
    0xFFFFFFFE, 0x00000001, 0x00000028, 0x0003005B, (u32)btlevtcmd_GetStatusMg, 0xFFFFFFFE,
    0xFE363C86, 0x0003005B, (u32)btlevtcmd_GetStatusMg, 0xFE363C83, 0xFE363C87, 0x0002003C,
    0xFE363C86, 0x0000000C, 0x0002003C, 0xFE363C87, 0x00000007, 0x0005005B,
    (u32)btlevtcmd_GetPos, 0xFE363C83, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B,
    (u32)btlevtcmd_FaceDirectionAdd, 0xFE363C83, 0xFE363C80, 0xFE363C86, 0x0004005B, (u32)btlevtcmd_FaceDirectionAdd,
    0xFE363C83, 0xFE363C80, 0xFE363C87, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000000EB, 0xFFFFFFFF, 0x00000000,
    0x0007005B, (u32)btlevtcmd_CalculateFaceDirection, 0xFFFFFFFE, 0xFFFFFFFF, 0xFE363C83, 0xFE363C84,
    0x00000010, 0xFE363C8F, 0x0003005B, (u32)btlevtcmd_ChangeFaceDirection, 0xFFFFFFFE, 0xFE363C8F,
    0x00000070, 0x0001005B, (u32)btlevtcmd_ResultAC, 0x0003005B, (u32)btlevtcmd_AcGetOutputParam, 0x00000002,
    0xFE363C85, 0x00010022, 0xFE363C85, 0x00010029, 0x0000000A, 0x00020032,
    0xFE363C85, 0x00000002, 0x00010029, 0x00000007, 0x00020032, 0xFE363C85,
    0x00000001, 0x00010029, 0x00000004, 0x00020032, 0xFE363C85, 0x00000000,
    0x0000002A, 0x00020032, 0xFE363C85, 0xFFFFFFFF, 0x00000031, 0x0002001D,
    0xFE363C85, 0x00000000, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0004005B, (u32)btlevtcmd_GetResultPrizeLv, 0xFFFFFFFB, 0xFE363C85,
    0xFE363C86, 0x0005005B, (u32)btlevtcmd_ACSuccessEffect, 0xFE363C86, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult, 0xFE363C8C, 0xFE363C86, 0x00000020,
    0x0003005B, (u32)btlevtcmd_AudienceDeclareACResult, 0xFE363C8C, 0xFFFFFFFF, 0x00000021, 0x0002005B,
    (u32)btlevtcmd_GetResultAC, 0xFE363C86, 0x0001005B, (u32)btlevtcmd_StopAC, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv,
    0x00000000, 0x00000002, 0x0003005B, (u32)evt_btl_camera_set_zoom, 0x00000000, 0x0000015E,
    0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PCH_K_1_802f8128, 0x00010009,
    0x00000006, 0x0002001D, 0xFE363C85, 0x00000000, 0x0001005E, (u32)unk_evt_803537c4,
    0x00020019, 0xFE363C80, 0x00000000, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFE363C83,
    0xFE363C84, 0x0000003B, 0x00000021, 0x00000021, 0x0003005B, (u32)btlevtcmd_GetStatusMg,
    0xFFFFFFFE, 0xFE363C86, 0x0002003C, 0xFE363C86, 0x00000010, 0x0005005B,
    (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B,
    (u32)btlevtcmd_FaceDirectionAdd, 0xFFFFFFFE, 0xFE363C80, 0xFE363C86, 0x0003005B, (u32)btlevtcmd_GetStatusMg,
    0xFFFFFFFE, 0xFE363C86, 0x0002003C, 0xFE363C86, 0x00000028, 0x00020035,
    0xFE363C81, 0xFE363C86, 0x0003005B, (u32)btlevtcmd_GetFaceDirection, 0xFE363C83, 0xFE363C86,
    0x00020037, 0xFE363C86, 0x0000002D, 0x000F005B, (u32)evt_eff, 0x00000000,
    (u32)(char*)str_kiss_8042411c, 0x00000000, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0xFE363C86,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_BTL_CHURINA_KISS_802f8130, 0xF1194D80,
    0x00000000, 0xF1194D80, 0x00010009, 0x00000028, 0x0006005B, (u32)btlevtcmd_GetPartsPos,
    0xFE363C83, 0xFE363C84, 0xFE363C8C, 0xFE363C8D, 0xFE363C8E, 0x0003005B,
    (u32)btlevtcmd_GetHeight, 0xFE363C83, 0xFE363C85, 0x0003005B, (u32)btlevtcmd_GetStatusMg, 0xFE363C83,
    0xFE363C86, 0x0002003C, 0xFE363C85, 0xFE363C86, 0x00020035, 0xFE363C8D,
    0xFE363C85, 0x0003005B, (u32)btlevtcmd_AcGetOutputParam, 0x00000002, 0xFE363C85, 0x000F005B,
    (u32)evt_eff, (u32)(char*)str_eff_80424124, (u32)(char*)str_recovery_802f81b0, 0x00000000, 0xFE363C8C, 0xFE363C8D,
    0xFE363C8E, 0xFE363C85, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x0001006C, 0xFE363C8F, 0x00010009,
    0x00000078, 0x0004005B, (u32)btlevtcmd_RecoverHp, 0xFE363C83, 0xFE363C84, 0xFE363C85,
    0x0000006D, 0x0006005B, (u32)btlevtcmd_ACRStart, 0xFFFFFFFE, 0x00000000, 0x0000000F,
    0x0000000F, 0x00000014, 0x0003005B, (u32)btlevtcmd_ACRGetResult, 0xFE363C86, 0xFE363C87,
    0x00010022, 0xFE363C86, 0x00010029, 0x00000002, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress,
    0xFFFFFFFE, 0xFE363C8C, 0x0006005B, (u32)btlevtcmd_AudienceDeclareAcrobatResult, 0xFE363C8C, 0x00000001,
    0x00000000, 0x00000000, 0x00000000, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE,
    0x00000001, (u32)(char*)str_PCH_Y_1_802f8118, 0x0003005B, (u32)btlevtcmd_AnimeWaitPlayComplete, 0xFFFFFFFE, 0x00000001,
    0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001, 0x0000002B, 0x0001000A,
    0x0000012C, 0x0000002A, 0x0001005B, (u32)evt_audience_acrobat_notry, 0x00000031, 0x0002005B,
    (u32)btlevtcmd_WaitEventEnd, 0xFE363C8F, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000,
    0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001, 0x0001005B, (u32)evt_audience_ap_recovery,
    0x0001005B, (u32)btlevtcmd_InviteApInfoReport, 0x0001005E, (u32)unk_evt_803537c4, 0x00020019, 0xFE363C80,
    0x00000000, 0x0002005B, (u32)btlevtcmd_ResetFaceDirection, 0xFE363C83, 0x0002005B, (u32)btlevtcmd_StartWaitEvent,
    0xFE363C83, 0x00000021, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001,
    0x0000002A, 0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE, 0xF24A8A80, 0x0008005B,
    (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000,
    0xFFFFFFFF, 0x00000000, 0x0002005B, (u32)btlevtcmd_ResetFaceDirection, 0xFFFFFFFE, 0x0002005B,
    (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002, 0x00000001,
};

u32 chuchurina_appeal_event[] = {
    0x0003005B, (u32)btlevtcmd_RunDataEventChild, 0xFFFFFFFE, 0x00000007, 0x0005005B, (u32)btlevtcmd_GetPos,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0006005B, (u32)evt_snd_sfxon_3d,
    (u32)(char*)str_SFX_MARIO_APPEAL1_802f81bc, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PCH_O_3_802f81d0, 0x0002005B, (u32)btlevtcmd_AudienceAppeal,
    0xFFFFFFFE, 0x0003005B, (u32)btlevtcmd_ftomsec, 0x0000002D, 0xFE363C80, 0x0001000A,
    0xFE363C80, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0004005B, (u32)btlevtcmd_FaceDirectionAdd, 0xFFFFFFFE, 0xFE363C80, 0x0000000F,
    0x00020035, 0xFE363C81, 0x0000001E, 0x0003005B, (u32)btlevtcmd_GetFaceDirection, 0xFFFFFFFE,
    0xFE363C8E, 0x00020037, 0xFE363C8E, 0x0000002D, 0x000F005B, (u32)evt_eff,
    (u32)zero_80424110, (u32)(char*)str_kiss_8042411c, 0x00000000, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0xFE363C8E, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_BTL_CHURINA_KISS_802f8130,
    0xF1194D80, 0x00000000, 0xF1194D80, 0x0003005B, (u32)btlevtcmd_AnimeWaitPlayComplete, 0xFFFFFFFE,
    0x00000001, 0x00010009, 0x0000001E, 0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE,
    0x00000002, 0x00000001,
};

static u32 party_win_reaction[18] = {
    0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PCH_Y_1_802f8118, 0x0003005B,
    (u32)btlevtcmd_AnimeWaitPlayComplete, 0xFFFFFFFE, 0x00000001, 0x0001000A, 0x0000014D, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001, 0x0000002B, 0x00000002, 0x00000001,
};

static u32 attack_audience_event[205] = {
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
    0x00000018, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000000, 0xFFFFFFFF, 0x00000000, 0x0003005B, (u32)evt_btl_camera_set_zoom,
    0x00000000, 0x000000C8, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001,
    (u32)(char*)str_PCH_A4_1_802f8024, 0x0003005B, (u32)btlevtcmd_ftof, 0x00000034, 0xFE363C80, 0x00010009,
    0xFE363C80, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_PCH_Y_1_802f8118,
    0x0003005B, (u32)btlevtcmd_ftof, 0x00000007, 0xFE363C80, 0x00010009, 0xFE363C80,
    0x0003005B, (u32)_attack_aud, 0xFE363C83, 0x00000000, 0x0003005B, (u32)evt_btl_camera_set_mode,
    0x00000000, 0x00000003, 0x0001006F, 0xFE363C8F, 0x0001000A, 0x000005DC,
    0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000, 0x0001000A, 0x000003E8,
    0x00000070, 0x0003005B, (u32)btlevtcmd_ftof, 0x00000023, 0xFE363C80, 0x00010009,
    0xFE363C80, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001, 0x0000002A,
    0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE, 0xF24A8A80, 0x0008005B, (u32)btlevtcmd_MovePosition,
    0xFFFFFFFE, 0xFE363C86, 0xFE363C81, 0xFE363C82, 0x00000000, 0xFFFFFFFF,
    0x00000000, 0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0005005B, (u32)btlevtcmd_SetPos, 0xFFFFFFFE, 0xFE363C88, 0xFE363C81,
    0xFE363C82, 0x0002005B, (u32)btlevtcmd_WaitEventEnd, 0xFE363C8F, 0x0008005B, (u32)btlevtcmd_MovePosition,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000, 0xFFFFFFFF,
    0x00000000, 0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002, 0x00000001,
    0x00000000,
};
