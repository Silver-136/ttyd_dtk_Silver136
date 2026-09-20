#include "battle/battle_item_data.h"

#include "driver/camdrv.h"
#include "statuswindow.h"
#include "system.h"

extern void* _battleWorkPointer;

/* Forward declarations for recovered battle_item_data .sdata item events. */
extern u32 ItemEvent_Dekadeka_Drink[];
extern u32 ItemEvent_Funyafunya_Kun[];
extern u32 ItemEvent_Kameno_Noroi[];
extern u32 ItemEvent_Kaminari_Dokkan[];
extern u32 ItemEvent_Kaminari_Gorogoro[];
extern u32 ItemEvent_Katikati_Koura[];
extern u32 ItemEvent_Madowaseno_Kona[];
extern u32 ItemEvent_Minimini_Kun[];
extern u32 ItemEvent_Shikaeshino_Kona[];
extern u32 ItemEvent_Sukkiri_Drink[];
extern u32 ItemEvent_Kaminari[];
extern u32 ItemEvent_Support_NoEffect[];
extern u32 ItemEvent_Support_RefreshEffect[];
extern u32 ItemWeaponData_Dekadeka_Drink[];
extern u32 ItemWeaponData_Funyafunya_Kun[];
extern u32 ItemWeaponData_Kameno_Noroi[];
extern u32 ItemWeaponData_Kaminari_Dokkan[];
extern u32 ItemWeaponData_Kaminari_Gorogoro[];
extern u32 ItemWeaponData_Katikati_Koura[];
extern u32 ItemWeaponData_Madowaseno_Kona[];
extern u32 ItemWeaponData_Minimini_Kun[];
extern u32 ItemWeaponData_Shikaeshino_Kona[];
extern u32 ItemWeaponData_Sukkiri_Drink[];

/* Forward declarations for recovered battle_item_data BattleWeapon records. */
extern u32 ItemWeaponData_CookingItem[];
extern u32 ItemWeaponData_Kinkyuu_Kinoko[];
extern u32 ItemWeaponData_Super_Kinoko[];
extern u32 ItemWeaponData_Maple_Syrup[];
extern u32 ItemWeaponData_Tankobu[];
extern u32 ItemWeaponData_SpaceFood[];
extern u32 ItemWeaponData_IceCandy[];
extern u32 ItemWeaponData_NancyFrappe[];
extern u32 ItemWeaponData_SnowRabbit[];
extern u32 ItemWeaponData_TeaKinoko[];
extern u32 ItemWeaponData_StarryDinner[];
extern u32 ItemWeaponData_KararinaPasta[];
extern u32 ItemWeaponData_MeromeroCake[];
extern u32 ItemWeaponData_BiribiriCandy[];
extern u32 ItemWeaponData_HealthySalad[];
extern u32 ItemWeaponData_FreshJuice[];
extern u32 ItemWeaponData_RedKararing[];
extern u32 ItemWeaponData_FutarideForever[];
extern u32 ItemWeaponData_NancyDynamite[];
extern u32 ItemWeaponData_CoconutsBomb[];
extern u32 ItemWeaponData_KachikachiDish[];
extern u32 ItemWeaponData_BomberEgg[];
extern u32 ItemWeaponData_LastDinner[];
extern s32 weaponGetPowerDefault();
extern s32 weaponGetPowerHPHalf2();

/* Forward declarations for recovered battle_item_data .data event bytecode. */
extern u32 ItemEvent_Attiike_Shisshi[];
extern u32 ItemEvent_Automatic_Target[];
extern u32 ItemEvent_Biribiri_Kinoko[];
extern u32 ItemEvent_Biribiri_Kinoko_Core[];
extern u32 ItemEvent_Fire_Flower[];
extern u32 ItemEvent_Fire_Flower_Core[];
extern u32 ItemEvent_GetTarget[];
extern u32 ItemEvent_Guruguru_Memawashi[];
extern u32 ItemEvent_Irekaeeru[];
extern u32 ItemEvent_Jiwajiwa_Kinoko[];
extern u32 ItemEvent_Jiwajiwa_Kinoko_Core[];
extern u32 ItemEvent_Jiwajiwa_Syrup[];
extern u32 ItemEvent_Jiwajiwa_Syrup_Core[];
extern u32 ItemEvent_Kaminari[];
extern u32 ItemEvent_Kaminari_Core[];
extern u32 ItemEvent_Kirakira_Otoshi[];
extern u32 ItemEvent_Kirakira_Otoshi_Core[];
extern u32 ItemEvent_Koorino_Ibuki[];
extern u32 ItemEvent_LastDinner[];
extern u32 ItemEvent_Naniga_Okorukana[];
extern u32 ItemEvent_Nemure_Yoikoyo[];
extern u32 ItemEvent_Poison_Kinoko[];
extern u32 ItemEvent_Pow_Block[];
extern u32 ItemEvent_Recovery[];
extern u32 ItemEvent_RecoveryAndStatus[];
extern u32 ItemEvent_Recovery_Core[];
extern u32 ItemEvent_Recovery_Core_Effect[];
extern u32 ItemEvent_Stop_Watch[];
extern u32 ItemEvent_Stop_Watch_Core[];
extern u32 ItemEvent_Suitooru[];
extern u32 ItemEvent_Support_NoEffect[];
extern u32 ItemEvent_Support_RefreshEffect[];
extern u32 ItemEvent_Support_Sub_Effect[];
extern u32 ItemEvent_Support_Sub_UseDeclere[];
extern u32 ItemEvent_Teki_Kyouka[];
extern u32 ItemEvent_Teki_Yokeeru[];
extern u32 ItemEvent_Teki_Yokeeru_Core[];
extern u32 ItemEvent_Teresano_Fuku[];
extern u32 ItemEvent_ThrowAttack[];
extern u32 ItemEvent_Yurayura_Jishin[];
extern u32 _return_home_event[];
extern u32 ItemWeaponData_Attiike_Shisshi[];
extern u32 ItemWeaponData_Biribiri_Kinoko[];
extern u32 ItemWeaponData_Fire_Flower[];
extern u32 ItemWeaponData_FirstLovePuddingBiribiri[];
extern u32 ItemWeaponData_FirstLovePuddingSleep[];
extern u32 ItemWeaponData_FirstLovePuddingTrans[];
extern u32 ItemWeaponData_Guruguru_Memawashi[];
extern u32 ItemWeaponData_Honey_Syrup[];
extern u32 ItemWeaponData_Irekaeeru[];
extern u32 ItemWeaponData_Jiwajiwa_Kinoko[];
extern u32 ItemWeaponData_Jiwajiwa_Syrup[];
extern u32 ItemWeaponData_Kinoko[];
extern u32 ItemWeaponData_Kirakira_Otoshi[];
extern u32 ItemWeaponData_Koorino_Ibuki[];
extern u32 ItemWeaponData_Naniga_Okorukana[];
extern u32 ItemWeaponData_Nemure_Yoikoyo[];
extern u32 ItemWeaponData_PeachTaltBiribiri[];
extern u32 ItemWeaponData_PeachTaltDodge[];
extern u32 ItemWeaponData_PeachTaltSleep[];
extern u32 ItemWeaponData_PoisonKinoko[];
extern u32 ItemWeaponData_Pow_Block[];
extern u32 ItemWeaponData_Royal_Jelly[];
extern u32 ItemWeaponData_Sinabita_Kinoko[];
extern u32 ItemWeaponData_Stop_Watch[];
extern u32 ItemWeaponData_Suitooru[];
extern u32 ItemWeaponData_Teki_Kyouka[];
extern u32 ItemWeaponData_Teki_Yokeeru[];
extern u32 ItemWeaponData_Teresano_Fuku[];
extern u32 ItemWeaponData_Ultra_Kinoko[];
extern u32 ItemWeaponData_Yurayura_Jishin[];
extern s32 btlevtcmd_AnimeChangePose();
extern s32 btlevtcmd_AnimeChangePoseType();
extern s32 btlevtcmd_CheckAttribute();
extern s32 btlevtcmd_CheckCommandUnit();
extern s32 btlevtcmd_CheckDamage();
extern s32 btlevtcmd_CheckWeaponPluralTarget();
extern s32 btlevtcmd_ChoiceSamplingEnemy();
extern s32 btlevtcmd_CommandFlyItem();
extern s32 btlevtcmd_CommandGetWeaponAddress();
extern s32 btlevtcmd_CommandGetWeaponItemId();
extern s32 btlevtcmd_ConsumeItem();
extern s32 btlevtcmd_FaceDirectionAdd();
extern s32 btlevtcmd_GetBodyId();
extern s32 btlevtcmd_GetConsumeItem();
extern s32 btlevtcmd_GetDamage();
extern s32 btlevtcmd_GetEnemyBelong();
extern s32 btlevtcmd_GetFaceDirection();
extern s32 btlevtcmd_GetFp();
extern s32 btlevtcmd_GetFriendBelong();
extern s32 btlevtcmd_GetHeight();
extern s32 btlevtcmd_GetHitPos();
extern s32 btlevtcmd_GetHomePos();
extern s32 btlevtcmd_GetHp();
extern s32 btlevtcmd_GetItemRecoverParam();
extern s32 btlevtcmd_GetMaxFp();
extern s32 btlevtcmd_GetMaxHp();
extern s32 btlevtcmd_GetPartsPos();
extern s32 btlevtcmd_GetPos();
extern s32 btlevtcmd_GetSelectEnemy();
extern s32 btlevtcmd_GetSelectNextEnemy();
extern s32 btlevtcmd_GetStageSize();
extern s32 btlevtcmd_GetStatusMg();
extern s32 btlevtcmd_GetUnitKind();
extern s32 btlevtcmd_GetWidth();
extern s32 btlevtcmd_JumpPosition();
extern s32 btlevtcmd_JumpSetting();
extern s32 btlevtcmd_MovePosition();
extern s32 btlevtcmd_PreCheckDamage();
extern s32 btlevtcmd_RecoverFp();
extern s32 btlevtcmd_RecoverHp();
extern s32 btlevtcmd_SamplingEnemy();
extern s32 btlevtcmd_SetFp();
extern s32 btlevtcmd_SetHp();
extern s32 btlevtcmd_SetMoveSpeed();
extern s32 btlevtcmd_StageDispellFog();
extern s32 btlevtcmd_StartAvoid();
extern s32 btlevtcmd_StartWaitEvent();
extern s32 btlevtcmd_StatusWindowOnOff();
extern s32 btlevtcmd_TransStageFloorPosition();
extern s32 btlevtcmd_WaitEventEnd();
extern s32 btlevtcmd_WeaponAftereffect();
extern s32 btlevtcmd_check_battleflag();
extern s32 btlevtcmd_snd_se();
extern s32 evt_audience_ap_recovery();
extern s32 evt_audience_reflesh_natural_all();
extern s32 evt_btl_camera_noshake();
extern s32 evt_btl_camera_set_homing_unit();
extern s32 evt_btl_camera_set_mode();
extern s32 evt_btl_camera_set_moveSpeedLv();
extern s32 evt_btl_camera_set_moveto();
extern s32 evt_btl_camera_set_zoom();
extern s32 evt_btl_camera_shake_h();
extern s32 evt_eff();
extern s32 evt_eff64();
extern s32 evt_env_blur_on();
extern s32 evt_mobj_delete();
extern s32 evt_mobj_entry();
extern s32 evt_mobj_set_anim();
extern s32 evt_mobj_set_position();
extern s32 evt_snd_sfxoff();
extern s32 evt_snd_sfxon();
extern s32 evt_snd_sfxon_3d();
extern s32 evt_sub_random();
extern s32 evt_sub_rumble_onoff();
extern const char str_A_1_804227c4[];
extern const char str_M_C_1_804227dc[];
extern const char str_bomb_804227e4[];
extern const char str_hit_804227c8[];
extern const char str_ibuki_804227cc[];
extern const char str_mist_804227f0[];
extern const char str_sheep_804227d4[];
extern u8 zero_804227c0;


s32 evtGetValue(void* event, s32 arg);
s32 BattleTransID(void* event, s32 id);
void* BattleGetUnitPtr(void* battleWork, s32 unitId);
void BattleStatusWindowSystemOn(void);
extern f32 float_720_80422824;
extern f32 float_0_80422804;
void _nancy_disp(void* unused, void* work);

s32 _sampling_random_target(void* event) {
    extern s32 BattleSamplingEnemy(void* targetWork, void* weapon, s32 attackerIdx, s32 enemyBelong,
                                   u32 targetClassFlags, u32 targetPropertyFlags, s32 direction);
    extern s32 evtSetValue(void* event, s32 arg, s32 value);
    extern s32 irand(s32 max);

    s32* args;
    void* battleWork;
    void* weapon;
    s32 outUnitArg;
    s32 outPartArg;
    s32 belong;
    s32 attackerIdx;
    s32 index;
    s32 entryIndex;
    u8 targetWork[0xA74];

    args = *(s32**)((s32)event + 0x18);
    battleWork = _battleWorkPointer;
    weapon = (void*)evtGetValue(event, args[0]);
    outUnitArg = args[1];
    outPartArg = args[2];
    belong = (s8)evtGetValue(event, args[3]);
    attackerIdx = BattleTransID(event, -2);

    BattleSamplingEnemy(targetWork,
                        weapon,
                        attackerIdx,
                        belong,
                        *(u32*)((s32)weapon + 0x64),
                        *(u32*)((s32)weapon + 0x68),
                        -(s8)*(u8*)((s32)battleWork + belong * 8 + 0xA));

    if ((s8)targetWork[0xA6C] < 1) {
        evtSetValue(event, outUnitArg, -1);
        evtSetValue(event, outPartArg, -1);
    } else {
        index = irand((s8)targetWork[0xA6C]);
        entryIndex = (s8)targetWork[index + 0xA6D];
        evtSetValue(event, outUnitArg, *(s16*)&targetWork[entryIndex * 0x24 + 4]);
        evtSetValue(event, outPartArg, *(s16*)&targetWork[entryIndex * 0x24 + 6]);
    }

    return 2;
}

u8 _cam_rot(void* work, s32 isFirstCall) {
    f32 rot;
    f32 start;
    f32 end;

    if (isFirstCall != 0) {
        *(s32*)((s32)work + 0x78) = sysMsec2Frame(2000);
        *(s32*)((s32)work + 0x7C) = *(s32*)((s32)work + 0x78);
    }

    start = float_720_80422824;
    end = float_0_80422804;
    rot = intplGetValue(4, *(s32*)((s32)work + 0x78), *(s32*)((s32)work + 0x7C), start, end);
    *(f32*)((s32)camGetPtr(4) + 0x14C) = rot;

    *(s32*)((s32)work + 0x78) -= 1;
    if (*(s32*)((s32)work + 0x78) <= 0) {
        *(f32*)((s32)camGetPtr(4) + 0x14C) = float_0_80422804;
        return 1;
    }

    return 0;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 _rank_up(void* event) {
    s32* args;
    void* unit;
    s32 id;
    u8 level;

    args = *(s32**)((s32)event + 0x18);
    id = evtGetValue(event, args[0]);
    unit = BattleGetUnitPtr(_battleWorkPointer, BattleTransID(event, id));

    if (unit != 0) {
        level = *(u8*)((s32)unit + 0xD);
        *(u8*)((s32)unit + 0xD) = level + 5;
        if (*(u8*)((s32)unit + 0xD) > 99) {
            *(u8*)((s32)unit + 0xD) = 99;
        }
    }

    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on



#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 _hpfp_change_declare_1(void* event) {
    s32* args;
    void* battleWork;
    void* unit;
    s32 id;
    s32 unitKind;

    args = *(s32**)((s32)event + 0x18);
    battleWork = _battleWorkPointer;
    id = evtGetValue(event, args[0]);
    unit = BattleGetUnitPtr(battleWork, BattleTransID(event, id));

    if (unit != 0) {
        unitKind = *(s32*)((s32)unit + 8);
        if (unitKind == 0xDE) {
            BattleStatusWindowSystemOn();
        } else if (unitKind >= 0xE0 && unitKind < 0xE7) {
            BattleStatusWindowSystemOn();
        }
    }

    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 _hpfp_change_declare_2(void* event) {
    s32* args;
    void* battleWork;
    void* unit;
    s32 id;
    s32 unitKind;

    args = *(s32**)((s32)event + 0x18);
    battleWork = _battleWorkPointer;
    id = evtGetValue(event, args[0]);
    unit = BattleGetUnitPtr(battleWork, BattleTransID(event, id));

    if (unit != 0) {
        unitKind = *(s32*)((s32)unit + 8);
        if (unitKind == 0xDE) {
            N_statusClearBlink();
            statusMarioHPBlink(0x78);
            statusFPBlink(0x78);
        } else if (unitKind >= 0xE0 && unitKind < 0xE7) {
            N_statusClearBlink();
            statusPartyHPBlink(0x78);
            statusFPBlink(0x78);
        }
    }

    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


u32 _naniga_okorukana_check(void* event, s32 isFirstCall) {
    extern void BtlUnit_GetPos(void* unit, f32* x, f32* y, f32* z);
    extern s32 BtlUnit_GetHeight(void* unit);
    extern s32 irand(s32 max);
    extern void* effNanigaEntry(f32 x, f32 y, f32 z, s32 unused, s32* table, s32 item);
    extern void effSetName(void* eff, char* name);
    extern s32 evtSetValue(void* event, s32 arg, s32 value);
    extern void* effNameToPtr(char* name);
    extern s32 R_mystery_item_table[];
    extern char str_effNaniga_802f0748[];
    extern f32 float_5_80422820;

    s32* args;
    void* battleWork;
    void* unit;
    s32 id;
    s32 outArg;
    s32 item;
    s32 count;
    u8* attackingCount;
    u8* nonAttackingCount;
    f32 x;
    f32 y;
    f32 z;
    u32 flags;
    s32 kind;

    args = *(s32**)((s32)event + 0x18);
    battleWork = _battleWorkPointer;
    id = evtGetValue(event, args[0]);
    outArg = args[1];
    unit = BattleGetUnitPtr(battleWork, BattleTransID(event, id));
    BtlUnit_GetPos(unit, &x, &y, &z);

    flags = *(u32*)((s32)unit + 0x104);
    if ((flags & 0x10) == 0) {
        if ((flags & 2) != 0) {
            y -= float_5_80422820;
        } else {
            y += (f32)(BtlUnit_GetHeight(unit) + 20);
        }
    }

    if (isFirstCall != 0) {
        count = 0;
        while (R_mystery_item_table[count] != 0) {
            count++;
        }

        item = R_mystery_item_table[irand(count)];
        *(void**)((s32)event + 0x78) = effNanigaEntry(x, y, z, 0, R_mystery_item_table, item);
        effSetName(*(void**)((s32)event + 0x78), str_effNaniga_802f0748);
        evtSetValue(event, outArg, item);

        kind = *(s32*)((s32)unit + 8);
        if (kind == 0xDE) {
            attackingCount = (u8*)((s32)battleWork + 0x16F41);
            nonAttackingCount = (u8*)((s32)battleWork + 0x16F42);
        } else if (kind >= 0xE0 && kind < 0xE7) {
            attackingCount = (u8*)((s32)battleWork + 0x16F43);
            nonAttackingCount = (u8*)((s32)battleWork + 0x16F44);
        } else {
            attackingCount = 0;
            nonAttackingCount = 0;
        }

        if (attackingCount != 0) {
            if (item == 0x84 || (item >= 0x80 && item < 0x83) || item == 0x8C) {
                *attackingCount += 1;
            } else {
                *nonAttackingCount += 1;
            }
        }
    }

    return effNameToPtr(str_effNaniga_802f0748) != 0 ? 0 : 2;
}
s32 _get_throw_first_pos(void* event) {
    extern void BtlUnit_GetPos(void* unit, f32* x, f32* y, f32* z);
    extern s32 BtlUnit_GetHeight(void* unit);
    extern void evtSetFloat(void* event, s32 arg, f32 value);
    extern f32 float_10_8042281c;
    extern f32 float_5_80422820;

    s32* args;
    void* battleWork;
    void* unit;
    s32 id;
    s32 xArg;
    s32 yArg;
    s32 zArg;
    f32 x;
    f32 y;
    f32 z;
    u32 flags;

    args = *(s32**)((s32)event + 0x18);
    battleWork = _battleWorkPointer;
    id = evtGetValue(event, args[0]);
    xArg = args[1];
    yArg = args[2];
    zArg = args[3];
    unit = BattleGetUnitPtr(battleWork, BattleTransID(event, id));
    BtlUnit_GetPos(unit, &z, &y, &x);

    y += float_10_8042281c;
    flags = *(u32*)((s32)unit + 0x104);
    if ((flags & 0x10) == 0) {
        if ((flags & 2) != 0) {
            y -= float_5_80422820;
        } else {
            y += *(f32*)((s32)unit + 0x114) * (f32)BtlUnit_GetHeight(unit);
        }
    }

    evtSetFloat(event, xArg, z);
    evtSetFloat(event, yArg, y);
    evtSetFloat(event, zArg, x);
    return 2;
}


u8 _throw_item_icon(void* event, s32 isFirstCall) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;
    typedef struct ThrowItemWork {
        Vec start;
        Vec current;
        Vec end;
        s32 frames;
        f32 moveDist;
        f32 gravity;
        f32 velocity;
        u8 padding[0x48];
    } ThrowItemWork;

    extern s32 evtGetValue(void* event, s32 arg);
    extern f32 evtGetFloat(void* event, s32 arg);
    extern void* BattleAlloc(s32 size);
    extern void BattleFree(void* ptr);
    extern f32 distABf(f32 x1, f32 z1, f32 x2, f32 z2);
    extern f32 angleABf(f32 x1, f32 z1, f32 x2, f32 z2);
    extern void btlMovePos(f32 distance, f32 angle, f32* x, f32* z);
    extern s32 psndSFXOn_3D(char* name, Vec* pos);
    extern void psndSFXOff(s32 sound);
    extern void dispEntry(s32 cameraId, s32 renderMode, void* callback, void* param, f32 order);
    extern void _disp_item_icon(void* unused, void* work);
    extern char str_SFX_ITEM_COOKING_THR_802f0730[];

    s32* args;
    s32 item;
    f32 startX;
    f32 startY;
    f32 startZ;
    f32 endX;
    f32 endY;
    f32 endZ;
    ThrowItemWork* work;
    f32 dist;
    Vec pos;

    args = *(s32**)((s32)event + 0x18);
    item = evtGetValue(event, args[0]);
    startX = evtGetFloat(event, args[1]);
    startY = evtGetFloat(event, args[2]);
    startZ = evtGetFloat(event, args[3]);
    endX = evtGetFloat(event, args[4]);
    endY = evtGetFloat(event, args[5]);
    endZ = evtGetFloat(event, args[6]);

    if (isFirstCall != 0) {
        work = BattleAlloc(0x7C);
        *(ThrowItemWork**)((s32)event + 0x78) = work;
        work->end.x = endX;
        work->end.y = endY;
        work->end.z = endZ;
        pos.x = startX;
        pos.y = startY;
        pos.z = startZ;
        work->current = pos;
        work->start = pos;
        dist = distABf(work->start.x, work->start.z, work->end.x, work->end.z);
        work->frames = (s32)(dist / 15.0f) + 30;
        work->gravity = 0.5f;
        work->velocity = work->gravity * (f32)-work->frames * 0.5f +
                         (work->end.y - work->start.y) / (f32)-work->frames;
        work->moveDist = distABf(work->start.x, work->start.z, work->end.x, work->end.z) /
                         (f32)-work->frames;
        *(s32*)((s32)event + 0x7C) = item;
        *(s32*)((s32)event + 0x80) = 0;
        *(s32*)((s32)event + 0x84) = psndSFXOn_3D(str_SFX_ITEM_COOKING_THR_802f0730, &pos);
    }

    work = *(ThrowItemWork**)((s32)event + 0x78);
    if (*(s32*)((s32)event + 0x80) == 0) {
        work->current.y += work->velocity;
        work->velocity -= work->gravity;
        if (work->velocity < 0.0f && work->current.y < work->end.y) {
            work->current.y = work->end.y;
        }
        btlMovePos(work->moveDist,
                   angleABf(work->start.x, work->start.z, work->end.x, work->end.z),
                   &work->current.x, &work->current.z);
        work->frames--;
        if (work->frames < 1) {
            *(s32*)((s32)event + 0x80) = 1;
            work->current = work->end;
        }
        dispEntry(4, 1, _disp_item_icon, event, 900.0f);
        return 0;
    }

    psndSFXOff(*(s32*)((s32)event + 0x84));
    BattleFree(work);
    return 2;
}

void _disp_item_icon(void* unused, void* work) {
    extern u8 itemDataTable[];
    extern f32 float_1_80422800;
    extern void iconDispGx(void* pos, s32 flags, s32 iconId, f32 scale);

    void* pos = (void*)((s32)*(void**)((s32)work + 0x78) + 0xC);
    s32 item = *(s32*)((s32)work + 0x7C);
    s32 offset;
    u8* data = itemDataTable;
    s32 icon;
    f32 scale = float_1_80422800;

    offset = item * 0x28;
    data += offset;
    icon = *(u16*)(data + 0x20);
    iconDispGx(pos, 0, icon, scale);
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 _nancy_mirage(void* event, s32 isFirstCall) {
    extern f32 evtGetFloat(void* event, s32 arg);
    extern s32 animPoseEntry(char* name, u32 flags);
    extern void animPoseSetAnim(s32 poseId, char* animName, s32 force);
    extern void animPoseSetEffectAnim(s32 poseId, char* animName, s32 force);
    extern void animPoseSetEffect(s32 poseId, char* name, u32 flags);
    extern void animPoseRelease(s32 poseId);
    extern void animPoseMain(s32 poseId);
    extern void dispEntry(s32 cameraId, s32 renderMode, void* callback, void* param, f32 order);
    extern char str_c_cook_802f0720[];
    extern char str_A_2_8042280c;
    extern char str_PM_Y_1A_802f0728[];

    s32* args;
    f32 x;
    f32 y;
    f32 order;

    args = *(s32**)((s32)event + 0x18);
    x = evtGetFloat(event, args[0]);
    y = evtGetFloat(event, args[1]);
    order = evtGetFloat(event, args[2]);

    if (isFirstCall != 0) {
        *(s32*)((s32)event + 0x78) = 60;
        *(s32*)((s32)event + 0x7C) = animPoseEntry(str_c_cook_802f0720, 1);
        *(s32*)((s32)event + 0x80) = 0;
        *(s32*)((s32)event + 0x84) = (s32)x;
        *(s32*)((s32)event + 0x88) = (s32)y;
        *(s32*)((s32)event + 0x8C) = (s32)order;
        animPoseSetAnim(*(s32*)((s32)event + 0x7C), &str_A_2_8042280c, 1);
        animPoseSetEffectAnim(*(s32*)((s32)event + 0x7C), str_PM_Y_1A_802f0728, 1);
    }

    *(s32*)((s32)event + 0x78) -= 1;
    if (*(s32*)((s32)event + 0x78) < 0) {
        animPoseSetEffect(*(s32*)((s32)event + 0x7C), 0, 0);
        animPoseRelease(*(s32*)((s32)event + 0x7C));
        return 2;
    }

    animPoseMain(*(s32*)((s32)event + 0x7C));
    dispEntry(4, 2, _nancy_disp, event, order);
    return 0;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


void _nancy_disp(void* unused, void* work) {
    extern void PSMTXTrans(f32 m[3][4], f32 x, f32 y, f32 z);
    extern void PSMTXScale(f32 m[3][4], f32 x, f32 y, f32 z);
    extern void PSMTXIdentity(f32 m[3][4]);
    extern void PSMTXConcat(f32 a[3][4], f32 b[3][4], f32 out[3][4]);
    extern void animPoseSetMaterialEvtColor(s32 poseId, u32* color);
    extern void animPoseSetMaterialFlagOn(s32 poseId, u32 flag);
    extern void animPoseDrawMtx(s32 poseId, f32 m[3][4], s32 mode, f32 rot, f32 scale);
    extern f32 float_0p05_804227fc;
    extern f32 float_4_804227f8;
    extern f32 float_1_80422800;
    extern f32 float_2_80422808;
    extern u32 dat_804227ec;

    f32 scale;
    s32 alpha;
    s32 frames;
    f32 transMtx[3][4];
    f32 scaleMtx[3][4];
    f32 drawMtx[3][4];
    u32 color;
    u32 materialColor;

    if (*(s32*)((s32)work + 0x78) < 30) {
        frames = 30 - *(s32*)((s32)work + 0x78);
        alpha = (s8)(frames * -4 - 128);
        scale = float_0p05_804227fc * (f32)frames + float_4_804227f8;
    } else {
        alpha = -128;
        scale = float_4_804227f8;
    }

    PSMTXTrans(transMtx,
               (f32)*(s32*)((s32)work + 0x84),
               (f32)*(s32*)((s32)work + 0x88),
               (f32)*(s32*)((s32)work + 0x8C));
    PSMTXScale(scaleMtx, scale, scale, float_1_80422800);
    PSMTXIdentity(drawMtx);
    PSMTXConcat(scaleMtx, drawMtx, drawMtx);
    PSMTXConcat(transMtx, drawMtx, drawMtx);

    color = (dat_804227ec & 0xFFFFFF00) | (u8)alpha;
    materialColor = color;
    animPoseSetMaterialEvtColor(*(s32*)((s32)work + 0x7C), &materialColor);
    animPoseSetMaterialFlagOn(*(s32*)((s32)work + 0x7C), 0x40);
    animPoseDrawMtx(*(s32*)((s32)work + 0x7C), drawMtx, 1, float_0_80422804, float_2_80422808);
    animPoseDrawMtx(*(s32*)((s32)work + 0x7C), drawMtx, 2, float_0_80422804, float_2_80422808);
    animPoseDrawMtx(*(s32*)((s32)work + 0x7C), drawMtx, 3, float_0_80422804, float_2_80422808);
}
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 _full_ap_recover(void* event) {
    extern s32 BtlUnit_GetBelong(void* unit);
    extern s32 pouchGetMaxAP(void);
    extern void pouchSetAP(s32 ap);

    s32* args;
    void* battleWork;
    void* unit;
    s32 id;

    args = *(s32**)((s32)event + 0x18);
    battleWork = _battleWorkPointer;
    id = evtGetValue(event, args[0]);
    unit = BattleGetUnitPtr(battleWork, BattleTransID(event, id));

    if ((s8)BtlUnit_GetBelong(unit) == 0) {
        pouchSetAP(pouchGetMaxAP());
    }

    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

/* Recovered battle_item_data .rodata strings. */
const char str_recovery_802f04b8[] = "recovery";
const char str_SFX_REST_CONDITION1_802f04c4[] = "SFX_REST_CONDITION1";
const char str_BtCIPw_802f04d8[] = "_BtCIPw";
const char str_MOBJ_PowerBlock_802f04e0[] = "MOBJ_PowerBlock";
const char str_SFX_ITEM_POW_APPEAR1_802f04f0[] = "SFX_ITEM_POW_APPEAR1";
const char str_kemuri_test_802f0508[] = "kemuri_test";
const char str_SFX_MOBJ_POW_BLOCK1_802f0514[] = "SFX_MOBJ_POW_BLOCK1";
const char str_fireflower_n64_802f0528[] = "fireflower_n64";
const char str_SFX_ITEM_FIRE_FLOWER_802f0538[] = "SFX_ITEM_FIRE_FLOWER1";
const char str_thunder_n64_802f0550[] = "thunder_n64";
const char str_SFX_ITEM_ICE_THUNDER_802f055c[] = "SFX_ITEM_ICE_THUNDER_FALL1";
const char str_SFX_ITEM_KIRAKIRA_FA_802f0578[] = "SFX_ITEM_KIRAKIRA_FALL1";
const char str_sstar_n64_802f0590[] = "sstar_n64";
const char str_SFX_ITEM_ICE_WIND1_802f059c[] = "SFX_ITEM_ICE_WIND1";
const char str_hokori_n64_802f05b0[] = "hokori_n64";
const char str_SFX_ITEM_QUAKE1_802f05bc[] = "SFX_ITEM_QUAKE1";
const char str_SFX_ITEM_STOP_WATCH1_802f05cc[] = "SFX_ITEM_STOP_WATCH1";
const char str_timestamp_n64_802f05e4[] = "timestamp_n64";
const char str_SFX_ITEM_GURUGURU1_802f05f4[] = "SFX_ITEM_GURUGURU1";
const char str_SFX_ITEM_RANK_UP_ENE_802f0608[] = "SFX_ITEM_RANK_UP_ENEMY1";
const char str_rankup_802f0620[] = "rankup";
const char str_irekae_802f0628[] = "irekae";
const char str_SFX_ITEM_HP_FP_CHANG_802f0630[] = "SFX_ITEM_HP_FP_CHANGE1";
const char str_SFX_ITEM_HORROR1_802f0648[] = "SFX_ITEM_HORROR1";
const char str_SFX_ITEM_HORROR2_802f065c[] = "SFX_ITEM_HORROR2";
const char str_holography_n64_802f0670[] = "holography_n64";
const char str_SFX_BTL_THUNDERS_ATT_802f0680[] = "SFX_BTL_THUNDERS_ATTACK4";
const char str_hibashira_802f069c[] = "hibashira";
const char str_SFX_BTL_THUNDERS_ATT_802f06a8[] = "SFX_BTL_THUNDERS_ATTACK3";
const char str_charge_802f06c4[] = "charge";
const char str_SFX_ITEM_COOK_LAST1_802f06cc[] = "SFX_ITEM_COOK_LAST1";
const char str_SFX_ITEM_COOK_LAST2_802f06e0[] = "SFX_ITEM_COOK_LAST2";
const char str_indirect_802f06f4[] = "indirect";
const char str_SFX_ITEM_COOK_LAST3_802f0700[] = "SFX_ITEM_COOK_LAST3";
const char str_c_cook_802f0720[] = "c_cook";
const char str_PM_Y_1A_802f0728[] = "PM_Y_1A";
const char str_SFX_ITEM_COOKING_THR_802f0730[] = "SFX_ITEM_COOKING_THROW1";
const char str_effNaniga_802f0748[] = "effNaniga";

/* Recovered battle_item_data small item event bytecode (.sdata). */
u32 ItemEvent_Kaminari_Dokkan[] = {
    0x00020032, 0xFE363C8C, (u32)ItemWeaponData_Kaminari_Dokkan, 0x0001005E, (u32)ItemEvent_Kaminari, 0x00000002,
    0x00000001,
};

u32 ItemEvent_Kaminari_Gorogoro[] = {
    0x00020032, 0xFE363C8C, (u32)ItemWeaponData_Kaminari_Gorogoro, 0x0001005E, (u32)ItemEvent_Kaminari, 0x00000002,
    0x00000001,
};

u32 ItemEvent_Madowaseno_Kona[] = {
    0x00020032, 0xFE363C8C, (u32)ItemWeaponData_Madowaseno_Kona, 0x0001005E, (u32)ItemEvent_Support_NoEffect, 0x00000002,
    0x00000001,
};

u32 ItemEvent_Dekadeka_Drink[] = {
    0x00020032, 0xFE363C8C, (u32)ItemWeaponData_Dekadeka_Drink, 0x0001005E, (u32)ItemEvent_Support_NoEffect, 0x00000002,
    0x00000001,
};

u32 ItemEvent_Katikati_Koura[] = {
    0x00020032, 0xFE363C8C, (u32)ItemWeaponData_Katikati_Koura, 0x0001005E, (u32)ItemEvent_Support_NoEffect, 0x00000002,
    0x00000001,
};

u32 ItemEvent_Minimini_Kun[] = {
    0x00020032, 0xFE363C8C, (u32)ItemWeaponData_Minimini_Kun, 0x0001005E, (u32)ItemEvent_Support_NoEffect, 0x00000002,
    0x00000001,
};

u32 ItemEvent_Funyafunya_Kun[] = {
    0x00020032, 0xFE363C8C, (u32)ItemWeaponData_Funyafunya_Kun, 0x0001005E, (u32)ItemEvent_Support_NoEffect, 0x00000002,
    0x00000001,
};

u32 ItemEvent_Sukkiri_Drink[] = {
    0x00020032, 0xFE363C8C, (u32)ItemWeaponData_Sukkiri_Drink, 0x0001005E, (u32)ItemEvent_Support_RefreshEffect, 0x00000002,
    0x00000001,
};

u32 ItemEvent_Shikaeshino_Kona[] = {
    0x00020032, 0xFE363C8C, (u32)ItemWeaponData_Shikaeshino_Kona, 0x0001005E, (u32)ItemEvent_Support_NoEffect, 0x00000002,
    0x00000001,
};

u32 ItemEvent_Kameno_Noroi[] = {
    0x00020032, 0xFE363C8C, (u32)ItemWeaponData_Kameno_Noroi, 0x0001005E, (u32)ItemEvent_Support_NoEffect, 0x00000002,
    0x00000001,
};

/*
 * Recovered battle_item_data .data event bytecode.
 * Exact US target words are preserved; relocation words remain symbolic.
 * BattleWeapon records remain a separate later structured-data lane.
 */
u32 _return_home_event[] = {
    0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00010003, 0x0000005F, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C83,
    0xFE363C84, 0xFE363C85, 0x00020019, 0xFE363C80, 0xFE363C83, 0x0000006E,
    0x0003005B, (u32)btlevtcmd_GetBodyId, 0xFFFFFFFE, 0xFE363C80, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType,
    0xFFFFFFFE, 0xFE363C80, 0x0000002A, 0x00000070, 0x0003005B, (u32)btlevtcmd_SetMoveSpeed,
    0xFFFFFFFE, 0xF24A8E80, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000000, 0xFFFFFFFF, 0x00000000, 0x00000021,
    0x00000002, 0x00000001,
};

u32 ItemEvent_Automatic_Target[] = {
    0x0003005B, (u32)btlevtcmd_CheckCommandUnit, 0xFFFFFFFE, 0xFE363C80, 0x00020019, 0xFE363C80,
    0x00000000, 0x0003005B, (u32)btlevtcmd_GetEnemyBelong, 0xFFFFFFFE, 0xFE363C80, 0x0004005B,
    (u32)btlevtcmd_SamplingEnemy, 0xFFFFFFFE, 0xFE363C80, 0xFE363C88, 0x0003005B, (u32)btlevtcmd_GetSelectEnemy,
    0xFE363C8A, 0xFE363C8B, 0x00000020, 0x0003005B, (u32)btlevtcmd_GetEnemyBelong, 0xFFFFFFFE,
    0xFE363C80, 0x0004005B, (u32)btlevtcmd_SamplingEnemy, 0xFFFFFFFE, 0xFE363C80, 0xFE363C88,
    0x0004005B, (u32)btlevtcmd_ChoiceSamplingEnemy, 0xFE363C88, 0xFE363C8A, 0xFE363C8B, 0x00000021,
    0x00000002, 0x00000001,
};

u32 ItemEvent_Recovery_Core_Effect[] = {
    0x0004005B, (u32)evt_btl_camera_set_homing_unit, 0x00000000, 0xFE363C8A, 0xFFFFFFFF, 0x0003005B,
    (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001, 0x0003005B, (u32)evt_btl_camera_set_zoom, 0x00000000,
    0x000000C8, 0x0005005B, (u32)btlevtcmd_GetItemRecoverParam, 0xFE363C89, 0xFE363C85, 0xFE363C86,
    0xFE363C87, 0x0002001D, 0xFE363C85, 0x00000001, 0x0006005B, (u32)btlevtcmd_GetPartsPos,
    0xFE363C8A, 0xFE363C8B, 0xFE363C8C, 0xFE363C8D, 0xFE363C8E, 0x0003005B,
    (u32)btlevtcmd_GetStatusMg, 0xFE363C8A, 0xFE363C80, 0x0003005B, (u32)btlevtcmd_GetHeight, 0xFE363C8A,
    0xFE363C81, 0x0002003C, 0xFE363C80, 0xFE363C81, 0x00020035, 0xFE363C8D,
    0xFE363C80, 0x000F005B, (u32)evt_eff, 0x00000000, (u32)(char*)str_recovery_802f04b8, 0x00000000,
    0xFE363C8C, 0xFE363C8D, 0xFE363C8E, 0xFE363C85, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00010009,
    0x00000078, 0x0004005B, (u32)btlevtcmd_RecoverHp, 0xFE363C8A, 0xFE363C8B, 0xFE363C85,
    0x00010009, 0x0000003C, 0x00000021, 0x0002001D, 0xFE363C86, 0x00000001,
    0x0006005B, (u32)btlevtcmd_GetPartsPos, 0xFE363C8A, 0xFE363C8B, 0xFE363C8C, 0xFE363C8D,
    0xFE363C8E, 0x0003005B, (u32)btlevtcmd_GetStatusMg, 0xFE363C8A, 0xFE363C80, 0x0003005B,
    (u32)btlevtcmd_GetHeight, 0xFE363C8A, 0xFE363C81, 0x0002003C, 0xFE363C80, 0xFE363C81,
    0x00020035, 0xFE363C8D, 0xFE363C80, 0x000F005B, (u32)evt_eff, 0x00000000,
    (u32)(char*)str_recovery_802f04b8, 0x00000001, 0xFE363C8C, 0xFE363C8D, 0xFE363C8E, 0xFE363C86,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00010009, 0x00000078, 0x0004005B, (u32)btlevtcmd_RecoverFp, 0xFE363C8A,
    0xFE363C8B, 0xFE363C86, 0x00010009, 0x0000003C, 0x00000021, 0x0002001D,
    0xFE363C87, 0x00000001, 0x00000021, 0x00000002, 0x00000001,
};

u32 ItemEvent_Recovery_Core[] = {
    0x0001005E, (u32)ItemEvent_Recovery_Core_Effect, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000,
    0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001, 0x00010009, 0x0000003C,
    0x0001005E, (u32)_return_home_event, 0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFE363C8A, 0x0002005B,
    (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002, 0x00000001,
};

u32 ItemEvent_Recovery[] = {
    0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress, 0xFFFFFFFE, 0xFE363C88, 0x0002005B, (u32)btlevtcmd_WeaponAftereffect,
    0xFE363C88, 0x0003005B, (u32)btlevtcmd_CheckCommandUnit, 0xFFFFFFFE, 0xFE363C80, 0x00020019,
    0xFE363C80, 0x00000000, 0x0003005B, (u32)btlevtcmd_GetSelectEnemy, 0xFE363C8A, 0xFE363C8B,
    0x00000020, 0x0003005B, (u32)btlevtcmd_GetEnemyBelong, 0xFFFFFFFE, 0xFE363C80, 0x0004005B,
    (u32)btlevtcmd_SamplingEnemy, 0xFFFFFFFE, 0xFE363C80, 0xFE363C88, 0x0004005B, (u32)btlevtcmd_ChoiceSamplingEnemy,
    0xFE363C88, 0xFE363C8A, 0xFE363C8B, 0x00000021, 0x0003005B, (u32)evt_btl_camera_set_mode,
    0x00000000, 0x00000007, 0x0004005B, (u32)evt_btl_camera_set_homing_unit, 0x00000000, 0xFFFFFFFE,
    0xFFFFFFFF, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001, 0x0003005B,
    (u32)evt_btl_camera_set_zoom, 0x00000000, 0x000000C8, 0x0001000A, 0x000003E8, 0x0003005B,
    (u32)btlevtcmd_GetFriendBelong, 0xFFFFFFFE, 0xFE363C80, 0x00020018, 0xFE363C80, 0x00000000,
    0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0003005B, (u32)btlevtcmd_GetBodyId, 0xFFFFFFFE, 0xFE363C89, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType,
    0xFFFFFFFE, 0xFE363C89, 0x00000028, 0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE,
    0xF24A8680, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFFFFFFC4, 0xFE363C81,
    0xFE363C82, 0x00000000, 0xFFFFFFFF, 0x00000000, 0x00000021, 0x0003005B,
    (u32)btlevtcmd_GetBodyId, 0xFFFFFFFE, 0xFE363C89, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE,
    0xFE363C89, 0x00000034, 0x0003005B, (u32)btlevtcmd_CommandFlyItem, 0xFFFFFFFE, 0x0000003C,
    0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0xFE363C89, 0x00000045, 0x0002005B,
    (u32)btlevtcmd_ConsumeItem, 0xFFFFFFFE, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponItemId, 0xFE363C89, 0xFE363C88,
    0x00020018, 0xFE363C89, 0x00000000, 0x0002005B, (u32)btlevtcmd_GetConsumeItem, 0xFE363C89,
    0x00000021, 0x0001005E, (u32)ItemEvent_Recovery_Core, 0x00000002, 0x00000001,
};

u32 ItemEvent_Support_Sub_UseDeclere[] = {
    0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000007, 0x0004005B, (u32)evt_btl_camera_set_homing_unit,
    0x00000000, 0xFFFFFFFE, 0xFFFFFFFF, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000,
    0x00000001, 0x0003005B, (u32)evt_btl_camera_set_zoom, 0x00000000, 0x000000C8, 0x0001000A,
    0x00000258, 0x0003005B, (u32)btlevtcmd_GetBodyId, 0xFFFFFFFE, 0xFE363C89, 0x0003005B,
    (u32)btlevtcmd_GetFriendBelong, 0xFFFFFFFE, 0xFE363C80, 0x00020018, 0xFE363C80, 0x00000000,
    0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0003005B, (u32)btlevtcmd_GetBodyId, 0xFFFFFFFE, 0xFE363C89, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType,
    0xFFFFFFFE, 0xFE363C89, 0x00000028, 0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE,
    0xF24A8680, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFFFFFFC4, 0xFE363C81,
    0xFE363C82, 0x00000000, 0xFFFFFFFF, 0x00000000, 0x00000021, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0xFE363C89, 0x00000034, 0x0003005B, (u32)btlevtcmd_CommandFlyItem,
    0xFFFFFFFE, 0x00000028, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0xFE363C89,
    0x00000045, 0x0002005B, (u32)btlevtcmd_ConsumeItem, 0xFFFFFFFE, 0x0003005B, (u32)evt_btl_camera_set_mode,
    0x00000000, 0x00000000, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001,
    0x00000002, 0x00000001,
};

u32 ItemEvent_GetTarget[] = {
    0x0003005B, (u32)btlevtcmd_CheckCommandUnit, 0xFFFFFFFE, 0xFE363C80, 0x00020019, 0xFE363C80,
    0x00000000, 0x0003005B, (u32)btlevtcmd_CheckWeaponPluralTarget, 0xFE363C8C, 0xFE363C80, 0x00020019,
    0xFE363C80, 0x00000000, 0x0003005B, (u32)btlevtcmd_GetEnemyBelong, 0xFFFFFFFE, 0xFE363C80,
    0x0004005B, (u32)btlevtcmd_SamplingEnemy, 0xFFFFFFFE, 0xFE363C80, 0xFE363C8C, 0x00000021,
    0x0003005B, (u32)btlevtcmd_GetSelectEnemy, 0xFE363C8A, 0xFE363C8B, 0x00000020, 0x0003005B,
    (u32)btlevtcmd_CheckWeaponPluralTarget, 0xFE363C8C, 0xFE363C80, 0x00020019, 0xFE363C80, 0x00000000,
    0x0003005B, (u32)btlevtcmd_GetEnemyBelong, 0xFFFFFFFE, 0xFE363C80, 0x0004005B, (u32)btlevtcmd_SamplingEnemy,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C8C, 0x0003005B, (u32)btlevtcmd_GetSelectEnemy, 0xFE363C8A,
    0xFE363C8B, 0x00000020, 0x0003005B, (u32)btlevtcmd_GetEnemyBelong, 0xFFFFFFFE, 0xFE363C80,
    0x0004005B, (u32)btlevtcmd_SamplingEnemy, 0xFFFFFFFE, 0xFE363C80, 0xFE363C8C, 0x0004005B,
    (u32)btlevtcmd_ChoiceSamplingEnemy, 0xFE363C8C, 0xFE363C8A, 0xFE363C8B, 0x00000021, 0x00000021,
    0x00000002, 0x00000001,
};

u32 ItemEvent_Support_Sub_Effect[] = {
    0x0001005E, (u32)ItemEvent_GetTarget, 0x00010003, 0x0000000A, 0x00020018, 0xFE363C8A,
    0xFFFFFFFF, 0x00010004, 0x0000005A, 0x00000021, 0x0007005B, (u32)btlevtcmd_PreCheckDamage,
    0xFFFFFFFE, 0xFE363C8A, 0xFE363C8B, 0xFE363C8C, 0x00000100, 0xFE363C85,
    0x00010022, 0xFE363C85, 0x00010024, 0x00000003, 0x0003005B, (u32)btlevtcmd_StartAvoid,
    0xFE363C8A, 0x00000026, 0x00010024, 0x00000006, 0x0003005B, (u32)btlevtcmd_StartAvoid,
    0xFE363C8A, 0x00000027, 0x00010024, 0x00000002, 0x0003005B, (u32)btlevtcmd_StartAvoid,
    0xFE363C8A, 0x00000028, 0x00010024, 0x00000001, 0x0007005B, (u32)btlevtcmd_CheckDamage,
    0xFFFFFFFE, 0xFE363C8A, 0xFE363C8B, 0xFE363C8C, 0x00000100, 0xFE363C85,
    0x00000031, 0x0003005B, (u32)btlevtcmd_CheckWeaponPluralTarget, 0xFE363C8C, 0xFE363C80, 0x00020019,
    0xFE363C80, 0x00000000, 0x0003005B, (u32)btlevtcmd_GetSelectNextEnemy, 0xFE363C8A, 0xFE363C8B,
    0x00000020, 0x00020032, 0xFE363C8A, 0xFFFFFFFF, 0x00000021, 0x00010004,
    0x0000000A, 0x00010003, 0x0000005A, 0x00000002, 0x00000001,
};

u32 ItemEvent_Support_NoEffect[] = {
    0x0002005B, (u32)btlevtcmd_WeaponAftereffect, 0xFE363C8C, 0x0001005E, (u32)ItemEvent_Support_Sub_UseDeclere, 0x00010009,
    0x0000001E, 0x0001005E, (u32)ItemEvent_Support_Sub_Effect, 0x0001005B, (u32)evt_audience_ap_recovery, 0x00010009,
    0x00000096, 0x0001005E, (u32)_return_home_event, 0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE,
    0x00000002, 0x00000001,
};

u32 ItemEvent_Support_RefreshEffect[] = {
    0x0002005B, (u32)btlevtcmd_WeaponAftereffect, 0xFE363C8C, 0x0001005E, (u32)ItemEvent_Support_Sub_UseDeclere, 0x00010009,
    0x0000001E, 0x0001005E, (u32)ItemEvent_GetTarget, 0x00010003, 0x0000000A, 0x00020018,
    0xFE363C8A, 0xFFFFFFFF, 0x00010004, 0x0000005A, 0x00000021, 0x0007005B,
    (u32)btlevtcmd_PreCheckDamage, 0xFFFFFFFE, 0xFE363C8A, 0xFE363C8B, 0xFE363C8C, 0x00000100,
    0xFE363C85, 0x00010022, 0xFE363C85, 0x00010024, 0x00000003, 0x0003005B,
    (u32)btlevtcmd_StartAvoid, 0xFE363C8A, 0x00000026, 0x00010024, 0x00000006, 0x0003005B,
    (u32)btlevtcmd_StartAvoid, 0xFE363C8A, 0x00000027, 0x00010024, 0x00000002, 0x0003005B,
    (u32)btlevtcmd_StartAvoid, 0xFE363C8A, 0x00000028, 0x00010024, 0x00000001, 0x0006005B,
    (u32)btlevtcmd_GetHitPos, 0xFE363C8A, 0xFE363C8B, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_REST_CONDITION1_802f04c4, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00000000, 0x0007005B, (u32)btlevtcmd_CheckDamage, 0xFFFFFFFE, 0xFE363C8A, 0xFE363C8B,
    0xFE363C8C, 0x00000100, 0xFE363C85, 0x00000031, 0x0003005B, (u32)btlevtcmd_CheckWeaponPluralTarget,
    0xFE363C8C, 0xFE363C80, 0x00020019, 0xFE363C80, 0x00000000, 0x0003005B,
    (u32)btlevtcmd_GetSelectNextEnemy, 0xFE363C8A, 0xFE363C8B, 0x00000020, 0x00020032, 0xFE363C8A,
    0xFFFFFFFF, 0x00000021, 0x00010004, 0x0000000A, 0x00010003, 0x0000005A,
    0x0001005B, (u32)evt_audience_ap_recovery, 0x00010009, 0x00000096, 0x0001005E, (u32)_return_home_event,
    0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002, 0x00000001,
};

/* Recovered BattleWeapon record. */
u32 ItemWeaponData_CookingItem[48] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x64000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01100160, 0x10000000, 0x00000302, 0x00000000, 0x00030008,
    0x000007FF, 0x80000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)ItemEvent_Recovery, 0x00000064, 0x00000000, 0x00000000,
};

/* Recovered BattleWeapon record. */
u32 ItemWeaponData_Kinoko[48] = {
    0x00000000, 0x00000000, 0x00000094, 0x00000000, 0x64000000, 0x3F800000,
    0x01010101, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01100160, 0x10000000, 0x00000302, 0x00000000, 0x00030148,
    0x000007FF, 0x00001110, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)ItemEvent_Recovery, 0x00000064, 0x00000000, 0x00000000,
};

/* Recovered BattleWeapon record. */
u32 ItemWeaponData_Kinkyuu_Kinoko[48] = {
    0x00000000, 0x00000000, 0x00000097, 0x00000000, 0x64000000, 0x3F800000,
    0x01010101, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01100160, 0x10000000, 0x00000302, 0x00000000, 0x00030148,
    0x000007FF, 0x00001110, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)ItemEvent_Recovery, 0x00000064, 0x00000000, 0x00000000,
};

/* Recovered BattleWeapon record. */
u32 ItemWeaponData_Super_Kinoko[48] = {
    0x00000000, 0x00000000, 0x00000095, 0x00000000, 0x64000000, 0x3F800000,
    0x01010101, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01100160, 0x10000000, 0x00000302, 0x00000000, 0x00030148,
    0x000007FF, 0x00001110, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)ItemEvent_Recovery, 0x00000064, 0x00000000, 0x00000000,
};

/* Recovered BattleWeapon record. */
u32 ItemWeaponData_Ultra_Kinoko[48] = {
    0x00000000, 0x00000000, 0x00000096, 0x00000000, 0x64000000, 0x3F800000,
    0x01010101, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01100160, 0x10000000, 0x00000302, 0x00000000, 0x00030148,
    0x000007FF, 0x00001110, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)ItemEvent_Recovery, 0x00000064, 0x00000000, 0x00000000,
};

/* Recovered BattleWeapon record. */
u32 ItemWeaponData_Sinabita_Kinoko[48] = {
    0x00000000, 0x00000000, 0x00000098, 0x00000000, 0x64000000, 0x3F800000,
    0x01010101, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01100160, 0x10000000, 0x00000302, 0x00000000, 0x00030148,
    0x000007FF, 0x00001110, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)ItemEvent_Recovery, 0x00000064, 0x00000000, 0x00000000,
};

/* Recovered BattleWeapon record. */
u32 ItemWeaponData_Honey_Syrup[48] = {
    0x00000000, 0x00000000, 0x0000009A, 0x00000000, 0x64000000, 0x3F800000,
    0x01010101, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01100160, 0x10000000, 0x00000302, 0x00000000, 0x00030148,
    0x000007FF, 0x80000002, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)ItemEvent_Recovery, 0x00000064, 0x00000000, 0x00000000,
};

/* Recovered BattleWeapon record. */
u32 ItemWeaponData_Maple_Syrup[48] = {
    0x00000000, 0x00000000, 0x0000009B, 0x00000000, 0x64000000, 0x3F800000,
    0x01010101, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01100160, 0x10000000, 0x00000302, 0x00000000, 0x00030148,
    0x000007FF, 0x80000002, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)ItemEvent_Recovery, 0x00000064, 0x00000000, 0x00000000,
};

/* Recovered BattleWeapon record. */
u32 ItemWeaponData_Royal_Jelly[48] = {
    0x00000000, 0x00000000, 0x0000009C, 0x00000000, 0x64000000, 0x3F800000,
    0x01010101, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01100160, 0x10000000, 0x00000302, 0x00000000, 0x00030148,
    0x000007FF, 0x80000002, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)ItemEvent_Recovery, 0x00000064, 0x00000000, 0x00000000,
};

/* Recovered BattleWeapon record. */
u32 ItemWeaponData_Tankobu[48] = {
    0x00000000, 0x00000000, 0x000000A5, 0x00000000, 0x64000000, 0x3F800000,
    0x01010101, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01100160, 0x10000000, 0x00000302, 0x00000000, 0x00030148,
    0x000007FF, 0x80000002, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)ItemEvent_Recovery, 0x00000064, 0x00000000, 0x00000000,
};

u32 ItemEvent_Pow_Block[] = {
    0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000007, 0x0004005B, (u32)evt_btl_camera_set_homing_unit,
    0x00000000, 0xFFFFFFFE, 0xFFFFFFFF, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000,
    0x00000001, 0x0003005B, (u32)evt_btl_camera_set_zoom, 0x00000000, 0x000000C8, 0x0001000A,
    0x000003E8, 0x0003005B, (u32)btlevtcmd_GetFriendBelong, 0xFFFFFFFE, 0xFE363C80, 0x00020018,
    0xFE363C80, 0x00000000, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0003005B, (u32)btlevtcmd_GetBodyId, 0xFFFFFFFE, 0xFE363C89,
    0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0xFE363C89, 0x00000028, 0x0003005B,
    (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE, 0xF24A8680, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE,
    0xFFFFFFC4, 0xFE363C81, 0xFE363C82, 0x00000000, 0xFFFFFFFF, 0x00000000,
    0x00000021, 0x0003005B, (u32)btlevtcmd_GetBodyId, 0xFFFFFFFE, 0xFE363C89, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0xFE363C89, 0x00000034, 0x0003005B, (u32)btlevtcmd_CommandFlyItem,
    0xFFFFFFFE, 0x00000028, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0xFE363C89,
    0x00000045, 0x0002005B, (u32)btlevtcmd_ConsumeItem, 0xFFFFFFFE, 0x0003005B, (u32)btlevtcmd_CheckCommandUnit,
    0xFFFFFFFE, 0xFE363C80, 0x00020019, 0xFE363C80, 0x00000000, 0x0005005B,
    (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0003005B,
    (u32)btlevtcmd_GetHeight, 0xFFFFFFFE, 0xFE363C85, 0x0004005B, (u32)btlevtcmd_FaceDirectionAdd, 0xFFFFFFFE,
    0xFE363C80, 0x0000001E, 0x00020035, 0xFE363C81, 0x00000010, 0x00020035,
    0xFE363C81, 0xFE363C85, 0x00000020, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0003005B, (u32)btlevtcmd_GetHeight, 0xFFFFFFFE,
    0xFE363C85, 0x00020035, 0xFE363C81, 0x00000010, 0x00020035, 0xFE363C81,
    0xFE363C85, 0x00000021, 0x0000006E, 0x0001000A, 0x00000064, 0x0003005B,
    (u32)evt_mobj_entry, (u32)(char*)str_BtCIPw_802f04d8, (u32)(char*)str_MOBJ_PowerBlock_802f04e0, 0x0005005B, (u32)evt_mobj_set_position, (u32)(char*)str_BtCIPw_802f04d8,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_ITEM_POW_APPEAR1_802f04f0,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000, 0x00000070, 0x00020035,
    0xFE363C81, 0x0000000A, 0x00020035, 0xFE363C82, 0x00000019, 0x000F005B,
    (u32)evt_eff, (u32)&zero_804227c0, (u32)(char*)str_kemuri_test_802f0508, 0x00000002, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0xF24A7E80, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x0000006B, 0x0001000A, 0x00000802,
    0x000F005B, (u32)evt_eff, (u32)&zero_804227c0, (u32)(char*)str_kemuri_test_802f0508, 0x00000002, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0xF24A7E80, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0000006D, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0xFE363C89, 0x00000045, 0x00010009, 0x00000005,
    0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv,
    0x00000000, 0x00000001, 0x00010009, 0x0000001E, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType,
    0xFFFFFFFE, 0xFE363C89, 0x0000002A, 0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE,
    0xF24A8680, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xF1194D80, 0xFE363C81,
    0xF1194D80, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000000, 0xFFFFFFFF, 0x00000000, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType,
    0xFFFFFFFE, 0xFE363C89, 0x0000002B, 0x00010009, 0x0000001E, 0x0000006E,
    0x0005005B, (u32)btlevtcmd_JumpSetting, 0xFFFFFFFE, 0x00000014, 0xF24A7A80, 0xF24A7DB3,
    0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0xFE363C89, 0x0000002C, 0x0005005B,
    (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0007005B,
    (u32)btlevtcmd_JumpPosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0000000E,
    0xFFFFFFFF, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0xFE363C89, 0x0000002B,
    0x00010009, 0x0000003C, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0xFE363C89,
    0x0000002A, 0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE, 0xF24A8680, 0x0008005B,
    (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000,
    0xFFFFFFFF, 0x00000000, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0xFE363C89,
    0x0000002B, 0x00000070, 0x0000006B, 0x00010009, 0x00000005, 0x0005005B,
    (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0006005B,
    (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_MOBJ_POW_BLOCK1_802f0514, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000,
    0x0003005B, (u32)evt_mobj_set_anim, (u32)(char*)str_BtCIPw_802f04d8, (u32)(char*)str_A_1_804227c4, 0x00010009, 0x00000003,
    0x0006005B, (u32)evt_btl_camera_shake_h, 0x00000000, 0x00000008, 0x00000000, 0x0000000A,
    0x0000000D, 0x00010009, 0x00000032, 0x0002005B, (u32)evt_mobj_delete, (u32)(char*)str_BtCIPw_802f04d8,
    0x0000006D, 0x0002005B, (u32)evt_audience_reflesh_natural_all, 0x00000064, 0x00010009, 0x00000007,
    0x0003005B, (u32)btlevtcmd_GetEnemyBelong, 0xFFFFFFFE, 0xFE363C80, 0x0004005B, (u32)btlevtcmd_SamplingEnemy,
    0xFFFFFFFE, 0xFE363C80, (u32)ItemWeaponData_Pow_Block, 0x0003005B, (u32)btlevtcmd_GetSelectEnemy, 0xFE363C8A,
    0xFE363C8B, 0x00010003, 0x0000000A, 0x00020018, 0xFE363C8A, 0xFFFFFFFF,
    0x00010004, 0x0000005A, 0x00000021, 0x0007005B, (u32)btlevtcmd_PreCheckDamage, 0xFFFFFFFE,
    0xFE363C8A, 0xFE363C8B, (u32)ItemWeaponData_Pow_Block, 0x00000100, 0xFE363C85, 0x00010022,
    0xFE363C85, 0x00010024, 0x00000003, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C8A,
    0x00000026, 0x00010024, 0x00000006, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C8A,
    0x00000027, 0x00010024, 0x00000002, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C8A,
    0x00000028, 0x00010024, 0x00000001, 0x0007005B, (u32)btlevtcmd_CheckDamage, 0xFFFFFFFE,
    0xFE363C8A, 0xFE363C8B, (u32)ItemWeaponData_Pow_Block, 0x00000100, 0xFE363C85, 0x00000031,
    0x0003005B, (u32)btlevtcmd_GetSelectNextEnemy, 0xFE363C8A, 0xFE363C8B, 0x00010004, 0x0000000A,
    0x00010003, 0x0000005A, 0x0001005B, (u32)evt_audience_ap_recovery, 0x0002005B, (u32)btlevtcmd_WeaponAftereffect,
    (u32)ItemWeaponData_Pow_Block, 0x00010009, 0x00000096, 0x0001005E, (u32)_return_home_event, 0x0002005B,
    (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002, 0x00000001,
};

/* Recovered BattleWeapon record. */
u32 ItemWeaponData_Pow_Block[48] = {
    0x00000000, 0x00000000, 0x0000008B, 0x00000000, 0x64000000, 0x3F800000,
    0x01010101, (u32)weaponGetPowerDefault, 0x00000002, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x02101260, 0x20000000, 0x00000302, 0x00000000, 0x00033040,
    0x000007FF, 0x80000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)ItemEvent_Pow_Block, 0x00646464, 0x4B0A0505, 0x05050000,
};

u32 ItemEvent_Fire_Flower_Core[] = {
    0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv,
    0x00000000, 0x00000001, 0x0001000A, 0x00000064, 0x0005005B, (u32)btlevtcmd_GetPos,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B, (u32)btlevtcmd_FaceDirectionAdd,
    0xFFFFFFFE, 0xFE363C80, 0x00000028, 0x00020032, 0xFE363C83, 0xFE363C80,
    0x00020032, 0xFE363C84, 0xFE363C81, 0x00020032, 0xFE363C85, 0xFE363C82,
    0x0004005B, (u32)btlevtcmd_FaceDirectionAdd, 0xFFFFFFFE, 0xFE363C83, 0x000000F0, 0x0004005B,
    (u32)btlevtcmd_TransStageFloorPosition, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0002001A, 0xFE363C80,
    0xFE363C83, 0x000F005B, (u32)evt_eff64, (u32)&zero_804227c0, (u32)(char*)str_fireflower_n64_802f0528, 0x00000000,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000000B4, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0000006E,
    0x00010009, 0x0000002D, 0x00010005, 0x00000024, 0x0006005B, (u32)evt_snd_sfxon_3d,
    (u32)(char*)str_SFX_ITEM_FIRE_FLOWER_802f0538, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000, 0x00010009,
    0x00000005, 0x00000006, 0x00000070, 0x00000020, 0x000F005B, (u32)evt_eff64,
    (u32)&zero_804227c0, (u32)(char*)str_fireflower_n64_802f0528, 0x00000001, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x000000B4, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x0000006E, 0x00010009, 0x0000002D, 0x00010005,
    0x00000024, 0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_ITEM_FIRE_FLOWER_802f0538, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000000, 0x00010009, 0x00000005, 0x00000006, 0x00000070,
    0x00000021, 0x0001000A, 0x00000AF0, 0x0003005B, (u32)btlevtcmd_GetEnemyBelong, 0xFFFFFFFE,
    0xFE363C80, 0x0004005B, (u32)btlevtcmd_SamplingEnemy, 0xFFFFFFFE, 0xFE363C80, (u32)ItemWeaponData_Fire_Flower,
    0x0003005B, (u32)btlevtcmd_GetSelectEnemy, 0xFE363C8A, 0xFE363C8B, 0x00010003, 0x0000000A,
    0x00020018, 0xFE363C8A, 0xFFFFFFFF, 0x00010004, 0x0000005A, 0x00000021,
    0x0007005B, (u32)btlevtcmd_PreCheckDamage, 0xFFFFFFFE, 0xFE363C8A, 0xFE363C8B, (u32)ItemWeaponData_Fire_Flower,
    0x00000100, 0xFE363C85, 0x00010022, 0xFE363C85, 0x00010024, 0x00000003,
    0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C8A, 0x00000026, 0x00010024, 0x00000006,
    0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C8A, 0x00000027, 0x00010024, 0x00000002,
    0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C8A, 0x00000028, 0x00010024, 0x00000001,
    0x0007005B, (u32)btlevtcmd_CheckDamage, 0xFFFFFFFE, 0xFE363C8A, 0xFE363C8B, (u32)ItemWeaponData_Fire_Flower,
    0x00000100, 0xFE363C85, 0x00000031, 0x0003005B, (u32)btlevtcmd_GetSelectNextEnemy, 0xFE363C8A,
    0xFE363C8B, 0x0001000A, 0x00000050, 0x00010004, 0x0000000A, 0x00010003,
    0x0000005A, 0x0001005B, (u32)evt_audience_ap_recovery, 0x00010009, 0x00000096, 0x0001005E,
    (u32)_return_home_event, 0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002, 0x00000001,
};

u32 ItemEvent_Fire_Flower[] = {
    0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000007, 0x0004005B, (u32)evt_btl_camera_set_homing_unit,
    0x00000000, 0xFFFFFFFE, 0xFFFFFFFF, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000,
    0x00000001, 0x0003005B, (u32)evt_btl_camera_set_zoom, 0x00000000, 0x000000C8, 0x00020032,
    0xFE363C8C, (u32)ItemWeaponData_Fire_Flower, 0x0002005B, (u32)btlevtcmd_WeaponAftereffect, 0xFE363C8C, 0x0001005E,
    (u32)ItemEvent_Support_Sub_UseDeclere, 0x00010009, 0x0000001E, 0x0001005E, (u32)ItemEvent_Fire_Flower_Core, 0x00000002,
    0x00000001,
};

/* Recovered BattleWeapon record. */
u32 ItemWeaponData_Fire_Flower[48] = {
    0x00000000, 0x00000000, 0x00000084, 0x00000000, 0x64000000, 0x3F800000,
    0x01010101, (u32)weaponGetPowerDefault, 0x00000003, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x02101260, 0x20000000, 0x01000302, 0x00000000, 0x00030040,
    0x000007FF, 0x80000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)ItemEvent_Fire_Flower, 0x00000064, 0x00000000, 0x00000000,
};

u32 ItemEvent_Kaminari_Core[] = {
    0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv,
    0x00000000, 0x00000001, 0x00010009, 0x0000001E, 0x0003005B, (u32)btlevtcmd_CheckCommandUnit,
    0xFFFFFFFE, 0xFE363C80, 0x00020019, 0xFE363C80, 0x00000000, 0x0003005B,
    (u32)btlevtcmd_CheckWeaponPluralTarget, 0xFE363C8C, 0xFE363C80, 0x00020019, 0xFE363C80, 0x00000000,
    0x0003005B, (u32)btlevtcmd_GetEnemyBelong, 0xFFFFFFFE, 0xFE363C80, 0x0004005B, (u32)btlevtcmd_SamplingEnemy,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C8C, 0x00000021, 0x0003005B, (u32)btlevtcmd_GetSelectEnemy,
    0xFE363C8A, 0xFE363C8B, 0x00000020, 0x0003005B, (u32)btlevtcmd_CheckWeaponPluralTarget, 0xFE363C8C,
    0xFE363C80, 0x00020019, 0xFE363C80, 0x00000000, 0x0003005B, (u32)btlevtcmd_GetEnemyBelong,
    0xFFFFFFFE, 0xFE363C80, 0x0004005B, (u32)btlevtcmd_SamplingEnemy, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C8C, 0x0003005B, (u32)btlevtcmd_GetSelectEnemy, 0xFE363C8A, 0xFE363C8B, 0x00000020,
    0x0003005B, (u32)btlevtcmd_GetEnemyBelong, 0xFFFFFFFE, 0xFE363C80, 0x0004005B, (u32)btlevtcmd_SamplingEnemy,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C8C, 0x0004005B, (u32)btlevtcmd_ChoiceSamplingEnemy, 0xFE363C8C,
    0xFE363C8A, 0xFE363C8B, 0x00000021, 0x00000021, 0x00010003, 0x0000000A,
    0x00020018, 0xFE363C8A, 0xFFFFFFFF, 0x00010004, 0x0000005A, 0x00000021,
    0x0003005B, (u32)evt_sub_random, 0x00000002, 0xFE363C84, 0x00020035, 0xFE363C84,
    0x00000003, 0x0006005B, (u32)btlevtcmd_GetHitPos, 0xFE363C8A, 0xFE363C8B, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0003005B, (u32)btlevtcmd_GetWidth, 0xFE363C8A, 0xFE363C85,
    0x0003005B, (u32)btlevtcmd_GetHeight, 0xFE363C8A, 0xFE363C86, 0x000F005B, (u32)evt_eff64,
    (u32)&zero_804227c0, (u32)(char*)str_thunder_n64_802f0550, 0xFE363C84, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0xFE363C85, 0xFE363C86, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_ITEM_ICE_THUNDER_802f055c, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000000, 0x0006005B, (u32)evt_btl_camera_shake_h, 0x00000000,
    0x00000003, 0x00000000, 0x00000019, 0x0000000D, 0x0003005B, (u32)evt_sub_rumble_onoff,
    0x00000000, 0x00000000, 0x0001000A, 0x00000053, 0x0007005B, (u32)btlevtcmd_PreCheckDamage,
    0xFFFFFFFE, 0xFE363C8A, 0xFE363C8B, 0xFE363C8C, 0x00000100, 0xFE363C85,
    0x00010022, 0xFE363C85, 0x00010024, 0x00000003, 0x0003005B, (u32)btlevtcmd_StartAvoid,
    0xFE363C8A, 0x00000026, 0x00010024, 0x00000006, 0x0003005B, (u32)btlevtcmd_StartAvoid,
    0xFE363C8A, 0x00000027, 0x00010024, 0x00000002, 0x0003005B, (u32)btlevtcmd_StartAvoid,
    0xFE363C8A, 0x00000028, 0x00010024, 0x00000001, 0x0007005B, (u32)btlevtcmd_CheckDamage,
    0xFFFFFFFE, 0xFE363C8A, 0xFE363C8B, 0xFE363C8C, 0x00000100, 0xFE363C85,
    0x00000031, 0x0001000A, 0x000000A7, 0x0003005B, (u32)evt_sub_rumble_onoff, 0x00000001,
    0x00000000, 0x0001000A, 0x000000A7, 0x0003005B, (u32)btlevtcmd_CheckWeaponPluralTarget, 0xFE363C8C,
    0xFE363C80, 0x00020019, 0xFE363C80, 0x00000000, 0x0003005B, (u32)btlevtcmd_GetSelectNextEnemy,
    0xFE363C8A, 0xFE363C8B, 0x00000020, 0x00020032, 0xFE363C8A, 0xFFFFFFFF,
    0x00000021, 0x00010004, 0x0000000A, 0x00010003, 0x0000005A, 0x0001005B,
    (u32)evt_audience_ap_recovery, 0x00010009, 0x0000001E, 0x0001005E, (u32)_return_home_event, 0x0002005B,
    (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002, 0x00000001,
};

u32 ItemEvent_Kaminari[] = {
    0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000007, 0x0004005B, (u32)evt_btl_camera_set_homing_unit,
    0x00000000, 0xFFFFFFFE, 0xFFFFFFFF, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000,
    0x00000001, 0x0003005B, (u32)evt_btl_camera_set_zoom, 0x00000000, 0x000000C8, 0x0001000A,
    0x000003E8, 0x0003005B, (u32)btlevtcmd_GetFriendBelong, 0xFFFFFFFE, 0xFE363C80, 0x00020018,
    0xFE363C80, 0x00000000, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0003005B, (u32)btlevtcmd_GetBodyId, 0xFFFFFFFE, 0xFE363C89,
    0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0xFE363C89, 0x00000028, 0x0003005B,
    (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE, 0xF24A8680, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE,
    0xFFFFFFC4, 0xFE363C81, 0xFE363C82, 0x00000000, 0xFFFFFFFF, 0x00000000,
    0x00000021, 0x0002005B, (u32)btlevtcmd_WeaponAftereffect, 0xFE363C8C, 0x0003005B, (u32)btlevtcmd_GetBodyId,
    0xFFFFFFFE, 0xFE363C89, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0xFE363C89,
    0x00000034, 0x0003005B, (u32)btlevtcmd_CommandFlyItem, 0xFFFFFFFE, 0x00000028, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0xFE363C89, 0x00000045, 0x0002005B, (u32)btlevtcmd_ConsumeItem,
    0xFFFFFFFE, 0x0001005E, (u32)ItemEvent_Kaminari_Core, 0x00000002, 0x00000001,
};

/* Recovered BattleWeapon record. */
u32 ItemWeaponData_Kaminari_Dokkan[48] = {
    0x00000000, 0x00000000, 0x00000081, 0x00000000, 0x64000000, 0x3F800000,
    0x01010101, (u32)weaponGetPowerDefault, 0x00000005, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x02101260, 0x20000000, 0x04000302, 0x00000000, 0x00030040,
    0x000007FF, 0x80000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)ItemEvent_Kaminari_Dokkan, 0x00000064, 0x00050505, 0x05000000,
};

/* Recovered BattleWeapon record. */
u32 ItemWeaponData_Kaminari_Gorogoro[48] = {
    0x00000000, 0x00000000, 0x00000080, 0x00000000, 0x64000000, 0x3F800000,
    0x01010101, (u32)weaponGetPowerDefault, 0x00000005, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01101260, 0x20000000, 0x04000302, 0x00000000, 0x00030040,
    0x000007FF, 0x80000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)ItemEvent_Kaminari_Gorogoro, 0x00000064, 0x00050505, 0x05000000,
};

u32 ItemEvent_Kirakira_Otoshi_Core[] = {
    0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv,
    0x00000000, 0x00000001, 0x00010009, 0x0000001E, 0x0003005B, (u32)btlevtcmd_CheckCommandUnit,
    0xFFFFFFFE, 0xFE363C80, 0x00020019, 0xFE363C80, 0x00000000, 0x00020032,
    0xFE363C8F, 0x00000000, 0x0003005B, (u32)btlevtcmd_CheckWeaponPluralTarget, 0xFE363C8C, 0xFE363C80,
    0x00020019, 0xFE363C80, 0x00000000, 0x0003005B, (u32)btlevtcmd_GetEnemyBelong, 0xFFFFFFFE,
    0xFE363C80, 0x0004005B, (u32)btlevtcmd_SamplingEnemy, 0xFFFFFFFE, 0xFE363C80, 0xFE363C8C,
    0x00020032, 0xFE363C8F, 0x00000001, 0x00000021, 0x0003005B, (u32)btlevtcmd_GetSelectEnemy,
    0xFE363C8A, 0xFE363C8B, 0x00000020, 0x0003005B, (u32)btlevtcmd_CheckWeaponPluralTarget, 0xFE363C8C,
    0xFE363C80, 0x00020019, 0xFE363C80, 0x00000000, 0x00020032, 0xFE363C8F,
    0x00000001, 0x0003005B, (u32)btlevtcmd_GetEnemyBelong, 0xFFFFFFFE, 0xFE363C80, 0x0004005B,
    (u32)btlevtcmd_SamplingEnemy, 0xFFFFFFFE, 0xFE363C80, 0xFE363C8C, 0x0003005B, (u32)btlevtcmd_GetSelectEnemy,
    0xFE363C8A, 0xFE363C8B, 0x00000020, 0x00020032, 0xFE363C8F, 0x00000000,
    0x0003005B, (u32)btlevtcmd_GetEnemyBelong, 0xFFFFFFFE, 0xFE363C80, 0x0004005B, (u32)btlevtcmd_SamplingEnemy,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C8C, 0x0004005B, (u32)btlevtcmd_ChoiceSamplingEnemy, 0xFE363C8C,
    0xFE363C8A, 0xFE363C8B, 0x00000021, 0x00000021, 0x0003005B, (u32)evt_snd_sfxon,
    (u32)(char*)str_SFX_ITEM_KIRAKIRA_FA_802f0578, 0x00000000, 0x00020018, 0xFE363C8F, 0x00000000, 0x0005005B,
    (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B,
    (u32)btlevtcmd_GetStageSize, 0xF1194D80, 0xFE363C81, 0xF1194D80, 0x0006005B, (u32)btlevtcmd_GetHitPos,
    0xFE363C8A, 0xFE363C8B, 0xFE363C83, 0xFE363C84, 0xFE363C85, 0x000F005B,
    (u32)evt_eff64, (u32)&zero_804227c0, (u32)(char*)str_sstar_n64_802f0590, 0x00000002, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0xFE363C83, 0xFE363C84, 0xFE363C85, 0xF24AA280, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_ITEM_KIRAKIRA_FA_802f0578,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000, 0x00010009, 0x00000032,
    0x00000020, 0x0000006E, 0x0001000A, 0x000001F4, 0x00010005, 0x00000005,
    0x0006005B, (u32)evt_btl_camera_shake_h, 0x00000000, 0x00000005, 0x00000000, 0x00000014,
    0x0000000C, 0x0001000A, 0x00000190, 0x0001000A, 0x000000C8, 0x00000006,
    0x00000070, 0x0001006F, 0xFE363C8F, 0x00010005, 0x00000005, 0x0003005B,
    (u32)evt_sub_random, 0x00000006, 0xFE363C80, 0x00020035, 0xFE363C80, 0x0000001B,
    0x00010009, 0xFE363C80, 0x0003005B, (u32)btlevtcmd_GetEnemyBelong, 0xFFFFFFFE, 0xFE363C80,
    0x0005005B, (u32)_sampling_random_target, 0xFE363C8C, 0xFE363C8A, 0xFE363C8B, 0xFE363C80,
    0x00020019, 0xFE363C8A, 0xFFFFFFFF, 0x0006005B, (u32)btlevtcmd_GetHitPos, 0xFE363C8A,
    0xFE363C8B, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000F005B, (u32)evt_eff,
    0x00000000, (u32)(char*)str_hit_804227c8, 0x00000000, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0xF24A7E80, 0x00000003, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000021, 0x00000006, 0x00000070, 0x0000006E,
    0x00010005, 0x00000007, 0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_ITEM_KIRAKIRA_FA_802f0578, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000000, 0x0001000A, 0x000001F4, 0x00000006,
    0x00000070, 0x00010005, 0x0000003C, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B, (u32)btlevtcmd_GetStageSize, 0xF1194D80,
    0xFE363C81, 0xF1194D80, 0x0003005B, (u32)evt_sub_random, 0x000000C8, 0xFE363C80,
    0x0003005B, (u32)evt_sub_random, 0x00000140, 0xFE363C83, 0x0003005B, (u32)btlevtcmd_GetFriendBelong,
    0xFE363C8A, 0xFE363C86, 0x00020018, 0xFE363C86, 0x00000000, 0x00020037,
    0xFE363C83, 0xFFFFFFFF, 0x00020032, 0xFE363C84, 0x00000000, 0x00000020,
    0x00020037, 0xFE363C80, 0xFFFFFFFF, 0x00020032, 0xFE363C84, 0x00000000,
    0x00000021, 0x0003005B, (u32)btlevtcmd_check_battleflag, 0xFE363C88, 0x00000100, 0x00020018,
    0xFE363C88, 0x00000000, 0x00020032, 0xFE363C82, 0x00000000, 0x00020032,
    0xFE363C85, 0x00000000, 0x00000020, 0x00020032, 0xFE363C82, 0x000000C8,
    0x00020032, 0xFE363C85, 0x000000C8, 0x00000021, 0x0003005B, (u32)evt_sub_random,
    0x00000078, 0xFE363C88, 0x00020036, 0xFE363C88, 0x0000003C, 0x00020035,
    0xFE363C85, 0xFE363C88, 0x0003005B, (u32)evt_sub_random, 0x00000078, 0xFE363C88,
    0x00020036, 0xFE363C88, 0x0000003C, 0x00020035, 0xFE363C82, 0xFE363C88,
    0x0004005B, (u32)btlevtcmd_TransStageFloorPosition, 0xFE363C83, 0xFE363C84, 0xFE363C85, 0x000F005B,
    (u32)evt_eff64, (u32)&zero_804227c0, (u32)(char*)str_sstar_n64_802f0590, 0x00000002, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0xFE363C83, 0xFE363C84, 0xFE363C85, 0xF24A9A80, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00010009, 0x00000003, 0x00000006,
    0x00010009, 0x00000032, 0x0002005B, (u32)btlevtcmd_WaitEventEnd, 0xFE363C8F, 0x0003005B,
    (u32)btlevtcmd_CheckCommandUnit, 0xFFFFFFFE, 0xFE363C80, 0x00020019, 0xFE363C80, 0x00000000,
    0x0003005B, (u32)btlevtcmd_GetEnemyBelong, 0xFFFFFFFE, 0xFE363C80, 0x0004005B, (u32)btlevtcmd_SamplingEnemy,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C8C, 0x0003005B, (u32)btlevtcmd_GetSelectEnemy, 0xFE363C8A,
    0xFE363C8B, 0x00000020, 0x0003005B, (u32)btlevtcmd_GetEnemyBelong, 0xFFFFFFFE, 0xFE363C80,
    0x0004005B, (u32)btlevtcmd_SamplingEnemy, 0xFFFFFFFE, 0xFE363C80, 0xFE363C8C, 0x0003005B,
    (u32)btlevtcmd_GetSelectEnemy, 0xFE363C8A, 0xFE363C8B, 0x00000021, 0x00000021, 0x00010003,
    0x0000000A, 0x00020018, 0xFE363C8A, 0xFFFFFFFF, 0x00010004, 0x0000005A,
    0x00000021, 0x0007005B, (u32)btlevtcmd_PreCheckDamage, 0xFFFFFFFE, 0xFE363C8A, 0xFE363C8B,
    0xFE363C8C, 0x00000100, 0xFE363C85, 0x00010022, 0xFE363C85, 0x00010024,
    0x00000003, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C8A, 0x00000026, 0x00010024,
    0x00000006, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C8A, 0x00000027, 0x00010024,
    0x00000002, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C8A, 0x00000028, 0x00010024,
    0x00000001, 0x0007005B, (u32)btlevtcmd_CheckDamage, 0xFFFFFFFE, 0xFE363C8A, 0xFE363C8B,
    0xFE363C8C, 0x00000100, 0xFE363C85, 0x00000031, 0x0003005B, (u32)btlevtcmd_CheckWeaponPluralTarget,
    0xFE363C8C, 0xFE363C80, 0x00020019, 0xFE363C80, 0x00000000, 0x0003005B,
    (u32)btlevtcmd_GetSelectNextEnemy, 0xFE363C8A, 0xFE363C8B, 0x00000020, 0x00020032, 0xFE363C8A,
    0xFFFFFFFF, 0x00000021, 0x0001000A, 0x00000050, 0x00010004, 0x0000000A,
    0x00010003, 0x0000005A, 0x0001005B, (u32)evt_audience_ap_recovery, 0x00010009, 0x00000096,
    0x0001005E, (u32)_return_home_event, 0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002,
    0x00000001,
};

u32 ItemEvent_Kirakira_Otoshi[] = {
    0x00020032, 0xFE363C8C, (u32)ItemWeaponData_Kirakira_Otoshi, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000,
    0x00000007, 0x0004005B, (u32)evt_btl_camera_set_homing_unit, 0x00000000, 0xFFFFFFFE, 0xFFFFFFFF,
    0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001, 0x0003005B, (u32)evt_btl_camera_set_zoom,
    0x00000000, 0x000000C8, 0x0002005B, (u32)btlevtcmd_WeaponAftereffect, 0xFE363C8C, 0x0001005E,
    (u32)ItemEvent_Support_Sub_UseDeclere, 0x0001005E, (u32)ItemEvent_Kirakira_Otoshi_Core, 0x00000002, 0x00000001,
};

/* Recovered BattleWeapon record. */
u32 ItemWeaponData_Kirakira_Otoshi[48] = {
    0x00000000, 0x00000000, 0x00000082, 0x00000000, 0x64000000, 0x3F800000,
    0x01010101, (u32)weaponGetPowerDefault, 0x00000006, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x02101260, 0x20000000, 0x00000302, 0x00000000, 0x00030040,
    0x000007FF, 0x80000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)ItemEvent_Kirakira_Otoshi, 0x000A0A64, 0x0A000000, 0x00000000,
};

u32 ItemEvent_Koorino_Ibuki[] = {
    0x00020032, 0xFE363C8C, (u32)ItemWeaponData_Koorino_Ibuki, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000,
    0x00000007, 0x0004005B, (u32)evt_btl_camera_set_homing_unit, 0x00000000, 0xFFFFFFFE, 0xFFFFFFFF,
    0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001, 0x0003005B, (u32)evt_btl_camera_set_zoom,
    0x00000000, 0x000000C8, 0x0001000A, 0x000003E8, 0x0003005B, (u32)btlevtcmd_GetFriendBelong,
    0xFFFFFFFE, 0xFE363C80, 0x00020018, 0xFE363C80, 0x00000000, 0x0005005B,
    (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0003005B,
    (u32)btlevtcmd_GetBodyId, 0xFFFFFFFE, 0xFE363C89, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE,
    0xFE363C89, 0x00000028, 0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE, 0xF24A8680,
    0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFFFFFFC4, 0xFE363C81, 0xFE363C82,
    0x00000000, 0xFFFFFFFF, 0x00000000, 0x00000021, 0x0002005B, (u32)btlevtcmd_WeaponAftereffect,
    0xFE363C8C, 0x0003005B, (u32)btlevtcmd_GetBodyId, 0xFFFFFFFE, 0xFE363C89, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0xFE363C89, 0x00000034, 0x0003005B, (u32)btlevtcmd_CommandFlyItem,
    0xFFFFFFFE, 0x00000028, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0xFE363C89,
    0x00000045, 0x0002005B, (u32)btlevtcmd_ConsumeItem, 0xFFFFFFFE, 0x0003005B, (u32)evt_btl_camera_set_mode,
    0x00000000, 0x00000000, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001,
    0x00010009, 0x0000001E, 0x0003005B, (u32)btlevtcmd_CheckCommandUnit, 0xFFFFFFFE, 0xFE363C80,
    0x00020019, 0xFE363C80, 0x00000000, 0x0003005B, (u32)btlevtcmd_CheckWeaponPluralTarget, 0xFE363C8C,
    0xFE363C80, 0x00020019, 0xFE363C80, 0x00000000, 0x0003005B, (u32)btlevtcmd_GetEnemyBelong,
    0xFFFFFFFE, 0xFE363C80, 0x0004005B, (u32)btlevtcmd_SamplingEnemy, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C8C, 0x00000021, 0x0003005B, (u32)btlevtcmd_GetSelectEnemy, 0xFE363C8A, 0xFE363C8B,
    0x00000020, 0x0003005B, (u32)btlevtcmd_CheckWeaponPluralTarget, 0xFE363C8C, 0xFE363C80, 0x00020019,
    0xFE363C80, 0x00000000, 0x0003005B, (u32)btlevtcmd_GetEnemyBelong, 0xFFFFFFFE, 0xFE363C80,
    0x0004005B, (u32)btlevtcmd_SamplingEnemy, 0xFFFFFFFE, 0xFE363C80, 0xFE363C8C, 0x0003005B,
    (u32)btlevtcmd_GetSelectEnemy, 0xFE363C8A, 0xFE363C8B, 0x00000020, 0x0003005B, (u32)btlevtcmd_GetEnemyBelong,
    0xFFFFFFFE, 0xFE363C80, 0x0004005B, (u32)btlevtcmd_SamplingEnemy, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C8C, 0x0004005B, (u32)btlevtcmd_ChoiceSamplingEnemy, 0xFE363C8C, 0xFE363C8A, 0xFE363C8B,
    0x00000021, 0x00000021, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_ITEM_ICE_WIND1_802f059c, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0xFE363C8F, 0x0004005B, (u32)btlevtcmd_TransStageFloorPosition, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0003005B, (u32)btlevtcmd_GetFaceDirection, 0xFFFFFFFE, 0xFE363C85,
    0x00020035, 0xFE363C81, 0x0000000A, 0x00020035, 0xFE363C82, 0x00000014,
    0x0002001D, 0xFE363C85, 0x00000000, 0x000F005B, (u32)evt_eff, 0x00000000,
    (u32)(char*)str_ibuki_804227cc, 0x00000000, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000000B4,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000020, 0x000F005B, (u32)evt_eff, 0x00000000, (u32)(char*)str_ibuki_804227cc,
    0x00000001, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000000B4, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000021, 0x0001000A, 0x000001F4, 0x0001005B, (u32)btlevtcmd_StageDispellFog, 0x0001000A,
    0x000007D0, 0x0002005B, (u32)evt_snd_sfxoff, 0xFE363C8F, 0x00010003, 0x0000000A,
    0x00020018, 0xFE363C8A, 0xFFFFFFFF, 0x00010004, 0x0000005A, 0x00000021,
    0x0007005B, (u32)btlevtcmd_PreCheckDamage, 0xFFFFFFFE, 0xFE363C8A, 0xFE363C8B, 0xFE363C8C,
    0x00000100, 0xFE363C85, 0x00010022, 0xFE363C85, 0x00010024, 0x00000003,
    0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C8A, 0x00000026, 0x00010024, 0x00000006,
    0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C8A, 0x00000027, 0x00010024, 0x00000002,
    0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C8A, 0x00000028, 0x00010024, 0x00000001,
    0x0007005B, (u32)btlevtcmd_CheckDamage, 0xFFFFFFFE, 0xFE363C8A, 0xFE363C8B, 0xFE363C8C,
    0x00000100, 0xFE363C85, 0x00000031, 0x0003005B, (u32)btlevtcmd_CheckWeaponPluralTarget, 0xFE363C8C,
    0xFE363C80, 0x00020019, 0xFE363C80, 0x00000000, 0x0003005B, (u32)btlevtcmd_GetSelectNextEnemy,
    0xFE363C8A, 0xFE363C8B, 0x00000020, 0x00020032, 0xFE363C8A, 0xFFFFFFFF,
    0x00000021, 0x00010004, 0x0000000A, 0x00010003, 0x0000005A, 0x0001005B,
    (u32)evt_audience_ap_recovery, 0x00010009, 0x00000096, 0x0001005E, (u32)_return_home_event, 0x0002005B,
    (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002, 0x00000001,
};

/* Recovered BattleWeapon record. */
u32 ItemWeaponData_Koorino_Ibuki[48] = {
    0x00000000, 0x00000000, 0x00000083, 0x00000000, 0x64000000, 0x3F800000,
    0x01010101, (u32)weaponGetPowerDefault, 0x00000003, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x02101260, 0x20000000, 0x02000302, 0x00000000, 0x00030040,
    0x000007FF, 0x80000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x005F0200, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)ItemEvent_Koorino_Ibuki, 0x00000064, 0x00000000, 0x00000000,
};

u32 ItemEvent_Yurayura_Jishin[] = {
    0x00020032, 0xFE363C8C, (u32)ItemWeaponData_Yurayura_Jishin, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000,
    0x00000007, 0x0004005B, (u32)evt_btl_camera_set_homing_unit, 0x00000000, 0xFFFFFFFE, 0xFFFFFFFF,
    0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001, 0x0003005B, (u32)evt_btl_camera_set_zoom,
    0x00000000, 0x000000C8, 0x0001000A, 0x000003E8, 0x0003005B, (u32)btlevtcmd_GetFriendBelong,
    0xFFFFFFFE, 0xFE363C80, 0x00020018, 0xFE363C80, 0x00000000, 0x0005005B,
    (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0003005B,
    (u32)btlevtcmd_GetBodyId, 0xFFFFFFFE, 0xFE363C89, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE,
    0xFE363C89, 0x00000028, 0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE, 0xF24A8680,
    0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFFFFFFC4, 0xFE363C81, 0xFE363C82,
    0x00000000, 0xFFFFFFFF, 0x00000000, 0x00000021, 0x0002005B, (u32)btlevtcmd_WeaponAftereffect,
    0xFE363C8C, 0x0003005B, (u32)btlevtcmd_GetBodyId, 0xFFFFFFFE, 0xFE363C89, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0xFE363C89, 0x00000034, 0x0003005B, (u32)btlevtcmd_CommandFlyItem,
    0xFFFFFFFE, 0x00000028, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0xFE363C89,
    0x00000045, 0x0002005B, (u32)btlevtcmd_ConsumeItem, 0xFFFFFFFE, 0x0003005B, (u32)evt_btl_camera_set_mode,
    0x00000000, 0x00000000, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001,
    0x00010009, 0x0000001E, 0x0000006E, 0x000F005B, (u32)evt_eff64, 0x00000000,
    (u32)(char*)str_hokori_n64_802f05b0, 0x00000000, 0x00000000, 0x0000012C, 0x00000000, 0x00000096,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x0001000A, 0x00000258, 0x000F005B, (u32)evt_eff64, 0x00000000,
    (u32)(char*)str_hokori_n64_802f05b0, 0x00000000, 0x00000000, 0x0000012C, 0x00000000, 0x0000003C,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x0001000A, 0x00000258, 0x000F005B, (u32)evt_eff64, 0x00000000,
    (u32)(char*)str_hokori_n64_802f05b0, 0x00000000, 0x00000000, 0x0000012C, 0x00000000, 0x0000003C,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x0001000A, 0x00000258, 0x000F005B, (u32)evt_eff64, 0x00000000,
    (u32)(char*)str_hokori_n64_802f05b0, 0x00000000, 0x00000000, 0x0000012C, 0x00000000, 0x0000003C,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000070, 0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_ITEM_QUAKE1_802f05bc, 0xFE363C8A,
    0x0006005B, (u32)evt_btl_camera_shake_h, 0x00000000, 0x00000000, 0x00000005, 0x0000005A,
    0x00000000, 0x0001000A, 0x000005DC, 0x0006005B, (u32)evt_btl_camera_shake_h, 0x00000000,
    0x00000005, 0x00000005, 0x00000028, 0x00000000, 0x0001000A, 0x0000029B,
    0x0006005B, (u32)evt_btl_camera_shake_h, 0x00000000, 0x00000005, 0x00000000, 0x0000001E,
    0x0000000D, 0x0001000A, 0x0000014D, 0x0002005B, (u32)evt_snd_sfxoff, 0xFE363C8A,
    0x0003005B, (u32)btlevtcmd_CheckCommandUnit, 0xFFFFFFFE, 0xFE363C80, 0x00020019, 0xFE363C80,
    0x00000000, 0x0003005B, (u32)btlevtcmd_CheckWeaponPluralTarget, 0xFE363C8C, 0xFE363C80, 0x00020019,
    0xFE363C80, 0x00000000, 0x0003005B, (u32)btlevtcmd_GetEnemyBelong, 0xFFFFFFFE, 0xFE363C80,
    0x0004005B, (u32)btlevtcmd_SamplingEnemy, 0xFFFFFFFE, 0xFE363C80, 0xFE363C8C, 0x00000021,
    0x0003005B, (u32)btlevtcmd_GetSelectEnemy, 0xFE363C8A, 0xFE363C8B, 0x00000020, 0x0003005B,
    (u32)btlevtcmd_CheckWeaponPluralTarget, 0xFE363C8C, 0xFE363C80, 0x00020019, 0xFE363C80, 0x00000000,
    0x0003005B, (u32)btlevtcmd_GetEnemyBelong, 0xFFFFFFFE, 0xFE363C80, 0x0004005B, (u32)btlevtcmd_SamplingEnemy,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C8C, 0x0003005B, (u32)btlevtcmd_GetSelectEnemy, 0xFE363C8A,
    0xFE363C8B, 0x00000020, 0x0003005B, (u32)btlevtcmd_GetEnemyBelong, 0xFFFFFFFE, 0xFE363C80,
    0x0004005B, (u32)btlevtcmd_SamplingEnemy, 0xFFFFFFFE, 0xFE363C80, 0xFE363C8C, 0x0004005B,
    (u32)btlevtcmd_ChoiceSamplingEnemy, 0xFE363C8C, 0xFE363C8A, 0xFE363C8B, 0x00000021, 0x00000021,
    0x00010003, 0x0000000A, 0x00020018, 0xFE363C8A, 0xFFFFFFFF, 0x00010004,
    0x0000005A, 0x00000021, 0x0007005B, (u32)btlevtcmd_PreCheckDamage, 0xFFFFFFFE, 0xFE363C8A,
    0xFE363C8B, 0xFE363C8C, 0x00000100, 0xFE363C85, 0x00010022, 0xFE363C85,
    0x00010024, 0x00000003, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C8A, 0x00000026,
    0x00010024, 0x00000006, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C8A, 0x00000027,
    0x00010024, 0x00000002, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C8A, 0x00000028,
    0x00010024, 0x00000001, 0x0007005B, (u32)btlevtcmd_CheckDamage, 0xFFFFFFFE, 0xFE363C8A,
    0xFE363C8B, 0xFE363C8C, 0x00000100, 0xFE363C85, 0x00000031, 0x0003005B,
    (u32)btlevtcmd_CheckWeaponPluralTarget, 0xFE363C8C, 0xFE363C80, 0x00020019, 0xFE363C80, 0x00000000,
    0x0003005B, (u32)btlevtcmd_GetSelectNextEnemy, 0xFE363C8A, 0xFE363C8B, 0x00000020, 0x00020032,
    0xFE363C8A, 0xFFFFFFFF, 0x00000021, 0x00010004, 0x0000000A, 0x00010003,
    0x0000005A, 0x0001005B, (u32)evt_audience_ap_recovery, 0x00010009, 0x00000096, 0x0001005E,
    (u32)_return_home_event, 0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002, 0x00000001,
};

/* Recovered BattleWeapon record. */
u32 ItemWeaponData_Yurayura_Jishin[48] = {
    0x00000000, 0x00000000, 0x00000085, 0x00000000, 0x64000000, 0x3F800000,
    0x01010101, (u32)weaponGetPowerDefault, 0x00000005, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x02101260, 0x20000008, 0x00000302, 0x00000000, 0x00030040,
    0x000007FF, 0x80000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)ItemEvent_Yurayura_Jishin, 0x00646464, 0x4B0A0505, 0x05050000,
};

u32 ItemEvent_Teresano_Fuku[] = {
    0x00020032, 0xFE363C8C, (u32)ItemWeaponData_Teresano_Fuku, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000,
    0x00000007, 0x0004005B, (u32)evt_btl_camera_set_homing_unit, 0x00000000, 0xFFFFFFFE, 0xFFFFFFFF,
    0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001, 0x0003005B, (u32)evt_btl_camera_set_zoom,
    0x00000000, 0x000000C8, 0x0001000A, 0x000003E8, 0x0003005B, (u32)btlevtcmd_GetFriendBelong,
    0xFFFFFFFE, 0xFE363C80, 0x00020018, 0xFE363C80, 0x00000000, 0x0005005B,
    (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0003005B,
    (u32)btlevtcmd_GetBodyId, 0xFFFFFFFE, 0xFE363C89, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE,
    0xFE363C89, 0x00000028, 0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE, 0xF24A8680,
    0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFFFFFFC4, 0xFE363C81, 0xFE363C82,
    0x00000000, 0xFFFFFFFF, 0x00000000, 0x00000021, 0x0002005B, (u32)btlevtcmd_WeaponAftereffect,
    0xFE363C8C, 0x0003005B, (u32)btlevtcmd_GetBodyId, 0xFFFFFFFE, 0xFE363C89, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0xFE363C89, 0x00000034, 0x0003005B, (u32)btlevtcmd_CommandFlyItem,
    0xFFFFFFFE, 0x00000028, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0xFE363C89,
    0x00000045, 0x0002005B, (u32)btlevtcmd_ConsumeItem, 0xFFFFFFFE, 0x0003005B, (u32)evt_btl_camera_set_mode,
    0x00000000, 0x00000000, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001,
    0x00010009, 0x0000001E, 0x0003005B, (u32)btlevtcmd_CheckCommandUnit, 0xFFFFFFFE, 0xFE363C80,
    0x00020019, 0xFE363C80, 0x00000000, 0x0003005B, (u32)btlevtcmd_CheckWeaponPluralTarget, 0xFE363C8C,
    0xFE363C80, 0x00020019, 0xFE363C80, 0x00000000, 0x0003005B, (u32)btlevtcmd_GetEnemyBelong,
    0xFFFFFFFE, 0xFE363C80, 0x0004005B, (u32)btlevtcmd_SamplingEnemy, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C8C, 0x00000021, 0x0003005B, (u32)btlevtcmd_GetSelectEnemy, 0xFE363C8A, 0xFE363C8B,
    0x00000020, 0x0003005B, (u32)btlevtcmd_CheckWeaponPluralTarget, 0xFE363C8C, 0xFE363C80, 0x00020019,
    0xFE363C80, 0x00000000, 0x0003005B, (u32)btlevtcmd_GetEnemyBelong, 0xFFFFFFFE, 0xFE363C80,
    0x0004005B, (u32)btlevtcmd_SamplingEnemy, 0xFFFFFFFE, 0xFE363C80, 0xFE363C8C, 0x0003005B,
    (u32)btlevtcmd_GetSelectEnemy, 0xFE363C8A, 0xFE363C8B, 0x00000020, 0x0003005B, (u32)btlevtcmd_GetEnemyBelong,
    0xFFFFFFFE, 0xFE363C80, 0x0004005B, (u32)btlevtcmd_SamplingEnemy, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C8C, 0x0004005B, (u32)btlevtcmd_ChoiceSamplingEnemy, 0xFE363C8C, 0xFE363C8A, 0xFE363C8B,
    0x00000021, 0x00000021, 0x00010003, 0x0000000A, 0x00020018, 0xFE363C8A,
    0xFFFFFFFF, 0x00010004, 0x0000005A, 0x00000021, 0x0007005B, (u32)btlevtcmd_PreCheckDamage,
    0xFFFFFFFE, 0xFE363C8A, 0xFE363C8B, 0xFE363C8C, 0x00000100, 0xFE363C85,
    0x00010022, 0xFE363C85, 0x00010024, 0x00000003, 0x0003005B, (u32)btlevtcmd_StartAvoid,
    0xFE363C8A, 0x00000026, 0x00010024, 0x00000006, 0x0003005B, (u32)btlevtcmd_StartAvoid,
    0xFE363C8A, 0x00000027, 0x00010024, 0x00000002, 0x0003005B, (u32)btlevtcmd_StartAvoid,
    0xFE363C8A, 0x00000028, 0x00010024, 0x00000001, 0x0007005B, (u32)btlevtcmd_CheckDamage,
    0xFFFFFFFE, 0xFE363C8A, 0xFE363C8B, 0xFE363C8C, 0x00000100, 0xFE363C85,
    0x00000031, 0x0003005B, (u32)btlevtcmd_CheckWeaponPluralTarget, 0xFE363C8C, 0xFE363C80, 0x00020019,
    0xFE363C80, 0x00000000, 0x0003005B, (u32)btlevtcmd_GetSelectNextEnemy, 0xFE363C8A, 0xFE363C8B,
    0x00000020, 0x00020032, 0xFE363C8A, 0xFFFFFFFF, 0x00000021, 0x00010004,
    0x0000000A, 0x00010003, 0x0000005A, 0x0001005B, (u32)evt_audience_ap_recovery, 0x00010009,
    0x00000096, 0x0001005E, (u32)_return_home_event, 0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE,
    0x00000002, 0x00000001,
};

/* Recovered BattleWeapon record. */
u32 ItemWeaponData_Teresano_Fuku[48] = {
    0x00000000, 0x00000000, 0x00000086, 0x00000000, 0x64000000, 0x3F800000,
    0x01010101, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01100160, 0x10000000, 0x00000302, 0x00000000, 0x00030108,
    0x000007FF, 0x80000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x64020000, 0x00000000, (u32)ItemEvent_Teresano_Fuku, 0x00000064, 0x00000000, 0x00000000,
};

u32 ItemEvent_Biribiri_Kinoko_Core[] = {
    0x0001005E, (u32)ItemEvent_Support_Sub_Effect, 0x0001005B, (u32)evt_audience_ap_recovery, 0x00010009, 0x00000096,
    0x0001005E, (u32)_return_home_event, 0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002,
    0x00000001,
};

u32 ItemEvent_Biribiri_Kinoko[] = {
    0x00020032, 0xFE363C8C, (u32)ItemWeaponData_Biribiri_Kinoko, 0x0002005B, (u32)btlevtcmd_WeaponAftereffect, 0xFE363C8C,
    0x0001005E, (u32)ItemEvent_Support_Sub_UseDeclere, 0x00010009, 0x0000001E, 0x0001005E, (u32)ItemEvent_Biribiri_Kinoko_Core,
    0x00000002, 0x00000001,
};

/* Recovered BattleWeapon record. */
u32 ItemWeaponData_Biribiri_Kinoko[48] = {
    0x00000000, 0x00000000, 0x00000087, 0x00000000, 0x64000000, 0x3F800000,
    0x01010101, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01100160, 0x10000000, 0x00000302, 0x00000000, 0x00030108,
    0x000007FF, 0x80000000, 0x00000000, 0x00000000, 0x00000064, 0x05000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)ItemEvent_Biribiri_Kinoko, 0x00000064, 0x00000000, 0x00000000,
};

u32 ItemEvent_Teki_Yokeeru_Core[] = {
    0x0001005E, (u32)ItemEvent_Support_Sub_Effect, 0x0001005B, (u32)evt_audience_ap_recovery, 0x00010009, 0x00000096,
    0x0001005E, (u32)_return_home_event, 0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002,
    0x00000001,
};

u32 ItemEvent_Teki_Yokeeru[] = {
    0x00020032, 0xFE363C8C, (u32)ItemWeaponData_Teki_Yokeeru, 0x0002005B, (u32)btlevtcmd_WeaponAftereffect, 0xFE363C8C,
    0x0001005E, (u32)ItemEvent_Support_Sub_UseDeclere, 0x00010009, 0x0000001E, 0x0001005E, (u32)ItemEvent_Teki_Yokeeru_Core,
    0x00000002, 0x00000001,
};

/* Recovered BattleWeapon record. */
u32 ItemWeaponData_Teki_Yokeeru[48] = {
    0x00000000, 0x00000000, 0x00000088, 0x00000000, 0x64000000, 0x3F800000,
    0x01010101, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01100160, 0x10000000, 0x00000302, 0x00000000, 0x00030108,
    0x000007FF, 0x80000000, 0x00000000, 0x00000000, 0x00000000, 0x00640400,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)ItemEvent_Teki_Yokeeru, 0x00000064, 0x00000000, 0x00000000,
};

/* Recovered BattleWeapon record. */
u32 ItemWeaponData_Madowaseno_Kona[48] = {
    0x00000000, 0x00000000, 0x00000089, 0x00000000, 0x64000000, 0x3F800000,
    0x01010101, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x02101260, 0x20000000, 0x00000302, 0x00000000, 0x00030000,
    0x000007FF, 0x80000000, 0x00000000, 0x00000000, 0x00640300, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)ItemEvent_Madowaseno_Kona, 0x00000064, 0x00000000, 0x00000000,
};

u32 ItemEvent_Nemure_Yoikoyo[] = {
    0x00020032, 0xFE363C8C, (u32)ItemWeaponData_Nemure_Yoikoyo, 0x0002005B, (u32)btlevtcmd_WeaponAftereffect, 0xFE363C8C,
    0x0001005E, (u32)ItemEvent_Support_Sub_UseDeclere, 0x00010009, 0x0000001E, 0x0006005B, (u32)evt_btl_camera_shake_h,
    0x00000000, 0x00000001, 0x00000000, 0x00002710, 0x00000000, 0x0003005B,
    (u32)btlevtcmd_check_battleflag, 0xFE363C80, 0x00000100, 0x00020018, 0xFE363C80, 0x00000000,
    0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x000F005B, (u32)evt_eff, 0x00000000, (u32)(char*)str_sheep_804227d4, 0x00000000, 0xFE363C80,
    0xFE363C81, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000020, 0x0005005B,
    (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00020035,
    0xFE363C82, 0x0000000F, 0x000F005B, (u32)evt_eff, 0x00000000, (u32)(char*)str_sheep_804227d4,
    0x00000000, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000021, 0x00010009, 0x0000012C, 0x0002005B, (u32)evt_btl_camera_noshake, 0x00000000,
    0x0001005E, (u32)ItemEvent_Support_Sub_Effect, 0x0001005B, (u32)evt_audience_ap_recovery, 0x00010009, 0x00000096,
    0x0001005E, (u32)_return_home_event, 0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002,
    0x00000001,
};

/* Recovered BattleWeapon record. */
u32 ItemWeaponData_Nemure_Yoikoyo[48] = {
    0x00000000, 0x00000000, 0x0000008A, 0x00000000, 0x64000000, 0x3F800000,
    0x01010101, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x02101260, 0x20000000, 0x00000302, 0x00000000, 0x00030000,
    0x000007FF, 0x80000000, 0x5F050000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)ItemEvent_Nemure_Yoikoyo, 0x00000064, 0x00000000, 0x00000000,
};

u32 ItemEvent_Stop_Watch_Core[] = {
    0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv,
    0x00000000, 0x00000001, 0x00010009, 0x0000001E, 0x0003005B, (u32)evt_snd_sfxon,
    (u32)(char*)str_SFX_ITEM_STOP_WATCH1_802f05cc, 0x00000000, 0x0003005B, (u32)btlevtcmd_check_battleflag, 0xFE363C80, 0x00000100,
    0x00020018, 0xFE363C80, 0x00000000, 0x000F005B, (u32)evt_eff64, 0x00000000,
    (u32)(char*)str_timestamp_n64_802f05e4, 0x00000000, 0x00000000, 0x00000000, 0x00000067, 0xF24A7E80,
    0x000000C8, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000020, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00020035, 0xFE363C82, 0x0000001E, 0x000F005B,
    (u32)evt_eff64, 0x00000000, (u32)(char*)str_timestamp_n64_802f05e4, 0x00000000, 0x00000000, 0x00000000,
    0xFE363C82, 0xF24A7E80, 0x000000C8, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000021, 0x00010009, 0x000000C8,
    0x0003005B, (u32)btlevtcmd_GetEnemyBelong, 0xFFFFFFFE, 0xFE363C80, 0x0004005B, (u32)btlevtcmd_SamplingEnemy,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C8C, 0x0003005B, (u32)btlevtcmd_GetSelectEnemy, 0xFE363C8A,
    0xFE363C8B, 0x00010003, 0x0000000A, 0x00020018, 0xFE363C8A, 0xFFFFFFFF,
    0x00010004, 0x0000005A, 0x00000021, 0x0007005B, (u32)btlevtcmd_PreCheckDamage, 0xFFFFFFFE,
    0xFE363C8A, 0xFE363C8B, 0xFE363C8C, 0x00000100, 0xFE363C85, 0x00010022,
    0xFE363C85, 0x00010024, 0x00000003, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C8A,
    0x00000026, 0x00010024, 0x00000006, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C8A,
    0x00000027, 0x00010024, 0x00000002, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C8A,
    0x00000028, 0x00010024, 0x00000001, 0x0007005B, (u32)btlevtcmd_CheckDamage, 0xFFFFFFFE,
    0xFE363C8A, 0xFE363C8B, 0xFE363C8C, 0x00000100, 0xFE363C85, 0x00000031,
    0x0003005B, (u32)btlevtcmd_GetSelectNextEnemy, 0xFE363C8A, 0xFE363C8B, 0x00010004, 0x0000000A,
    0x00010003, 0x0000005A, 0x0001005B, (u32)evt_audience_ap_recovery, 0x0001005E, (u32)_return_home_event,
    0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002, 0x00000001,
};

u32 ItemEvent_Stop_Watch[] = {
    0x00020032, 0xFE363C8C, (u32)ItemWeaponData_Stop_Watch, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000,
    0x00000007, 0x0004005B, (u32)evt_btl_camera_set_homing_unit, 0x00000000, 0xFFFFFFFE, 0xFFFFFFFF,
    0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001, 0x0003005B, (u32)evt_btl_camera_set_zoom,
    0x00000000, 0x000000C8, 0x0001000A, 0x000003E8, 0x0003005B, (u32)btlevtcmd_GetFriendBelong,
    0xFFFFFFFE, 0xFE363C80, 0x00020018, 0xFE363C80, 0x00000000, 0x0005005B,
    (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0003005B,
    (u32)btlevtcmd_GetBodyId, 0xFFFFFFFE, 0xFE363C89, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE,
    0xFE363C89, 0x00000028, 0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE, 0xF24A8680,
    0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFFFFFFC4, 0xFE363C81, 0xFE363C82,
    0x00000000, 0xFFFFFFFF, 0x00000000, 0x00000021, 0x0002005B, (u32)btlevtcmd_WeaponAftereffect,
    0xFE363C8C, 0x0003005B, (u32)btlevtcmd_GetBodyId, 0xFFFFFFFE, 0xFE363C89, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0xFE363C89, 0x00000034, 0x0003005B, (u32)btlevtcmd_CommandFlyItem,
    0xFFFFFFFE, 0x00000028, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0xFE363C89,
    0x00000045, 0x0002005B, (u32)btlevtcmd_ConsumeItem, 0xFFFFFFFE, 0x0001005E, (u32)ItemEvent_Stop_Watch_Core,
    0x00000002, 0x00000001,
};

/* Recovered BattleWeapon record. */
u32 ItemWeaponData_Stop_Watch[48] = {
    0x00000000, 0x00000000, 0x0000008C, 0x00000000, 0x64000000, 0x3F800000,
    0x01010101, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x02101260, 0x20000000, 0x00000302, 0x00000000, 0x00030000,
    0x000007FF, 0x80000000, 0x00005F03, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)ItemEvent_Stop_Watch, 0x00000064, 0x00000000, 0x00000000,
};

u32 ItemEvent_Guruguru_Memawashi[] = {
    0x00020032, 0xFE363C8C, (u32)ItemWeaponData_Guruguru_Memawashi, 0x0002005B, (u32)btlevtcmd_WeaponAftereffect, 0xFE363C8C,
    0x0001005E, (u32)ItemEvent_Support_Sub_UseDeclere, 0x00010009, 0x0000001E, 0x0003005B, (u32)evt_snd_sfxon,
    (u32)(char*)str_SFX_ITEM_GURUGURU1_802f05f4, 0x00000000, 0x0000006E, 0x0003005B, (u32)evt_env_blur_on, 0x00000000,
    0x000007D0, 0x00000070, 0x0001005B, (u32)_cam_rot, 0x0001005E, (u32)ItemEvent_Support_Sub_Effect,
    0x0001005B, (u32)evt_audience_ap_recovery, 0x00010009, 0x00000096, 0x0001005E, (u32)_return_home_event,
    0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002, 0x00000001,
};

/* Recovered BattleWeapon record. */
u32 ItemWeaponData_Guruguru_Memawashi[48] = {
    0x00000000, 0x00000000, 0x0000008D, 0x00000000, 0x64000000, 0x3F800000,
    0x01010101, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x02101260, 0x20000000, 0x00000302, 0x00000000, 0x00030000,
    0x000007FF, 0x80000000, 0x00000000, 0x64030000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)ItemEvent_Guruguru_Memawashi, 0x00000064, 0x00000000, 0x00000000,
};

/* Recovered BattleWeapon record. */
u32 ItemWeaponData_Dekadeka_Drink[48] = {
    0x00000000, 0x00000000, 0x0000008E, 0x00000000, 0x64000000, 0x3F800000,
    0x01010101, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01100160, 0x10000000, 0x00000302, 0x00000000, 0x00030108,
    0x000007FF, 0x80000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000064, 0x03020000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)ItemEvent_Dekadeka_Drink, 0x00000064, 0x00000000, 0x00000000,
};

/* Recovered BattleWeapon record. */
u32 ItemWeaponData_Katikati_Koura[48] = {
    0x00000000, 0x00000000, 0x0000008F, 0x00000000, 0x64000000, 0x3F800000,
    0x01010101, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01100160, 0x10000000, 0x00000302, 0x00000000, 0x00030108,
    0x000007FF, 0x80000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00640303, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)ItemEvent_Katikati_Koura, 0x00000064, 0x00000000, 0x00000000,
};

u32 ItemEvent_Suitooru[] = {
    0x00020032, 0xFE363C8C, (u32)ItemWeaponData_Suitooru, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000,
    0x00000007, 0x0004005B, (u32)evt_btl_camera_set_homing_unit, 0x00000000, 0xFFFFFFFE, 0xFFFFFFFF,
    0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001, 0x0003005B, (u32)evt_btl_camera_set_zoom,
    0x00000000, 0x000000C8, 0x0001000A, 0x000003E8, 0x0003005B, (u32)btlevtcmd_GetFriendBelong,
    0xFFFFFFFE, 0xFE363C80, 0x00020018, 0xFE363C80, 0x00000000, 0x0005005B,
    (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0003005B,
    (u32)btlevtcmd_GetBodyId, 0xFFFFFFFE, 0xFE363C89, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE,
    0xFE363C89, 0x00000028, 0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE, 0xF24A8680,
    0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFFFFFFC4, 0xFE363C81, 0xFE363C82,
    0x00000000, 0xFFFFFFFF, 0x00000000, 0x00000021, 0x0002005B, (u32)btlevtcmd_WeaponAftereffect,
    0xFE363C8C, 0x0003005B, (u32)btlevtcmd_GetBodyId, 0xFFFFFFFE, 0xFE363C89, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0xFE363C89, 0x00000034, 0x0003005B, (u32)btlevtcmd_CommandFlyItem,
    0xFFFFFFFE, 0x00000028, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0xFE363C89,
    0x00000045, 0x0002005B, (u32)btlevtcmd_ConsumeItem, 0xFFFFFFFE, 0x0003005B, (u32)evt_btl_camera_set_mode,
    0x00000000, 0x00000000, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001,
    0x00010009, 0x0000001E, 0x0003005B, (u32)btlevtcmd_CheckCommandUnit, 0xFFFFFFFE, 0xFE363C80,
    0x00020019, 0xFE363C80, 0x00000000, 0x0003005B, (u32)btlevtcmd_CheckWeaponPluralTarget, 0xFE363C8C,
    0xFE363C80, 0x00020019, 0xFE363C80, 0x00000000, 0x0003005B, (u32)btlevtcmd_GetEnemyBelong,
    0xFFFFFFFE, 0xFE363C80, 0x0004005B, (u32)btlevtcmd_SamplingEnemy, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C8C, 0x00000021, 0x0003005B, (u32)btlevtcmd_GetSelectEnemy, 0xFE363C8A, 0xFE363C8B,
    0x00000020, 0x0003005B, (u32)btlevtcmd_CheckWeaponPluralTarget, 0xFE363C8C, 0xFE363C80, 0x00020019,
    0xFE363C80, 0x00000000, 0x0003005B, (u32)btlevtcmd_GetEnemyBelong, 0xFFFFFFFE, 0xFE363C80,
    0x0004005B, (u32)btlevtcmd_SamplingEnemy, 0xFFFFFFFE, 0xFE363C80, 0xFE363C8C, 0x0003005B,
    (u32)btlevtcmd_GetSelectEnemy, 0xFE363C8A, 0xFE363C8B, 0x00000020, 0x0003005B, (u32)btlevtcmd_GetEnemyBelong,
    0xFFFFFFFE, 0xFE363C80, 0x0004005B, (u32)btlevtcmd_SamplingEnemy, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C8C, 0x0004005B, (u32)btlevtcmd_ChoiceSamplingEnemy, 0xFE363C8C, 0xFE363C8A, 0xFE363C8B,
    0x00000021, 0x00000021, 0x00010003, 0x0000000A, 0x00020018, 0xFE363C8A,
    0xFFFFFFFF, 0x00010004, 0x0000005A, 0x00000021, 0x0007005B, (u32)btlevtcmd_PreCheckDamage,
    0xFFFFFFFE, 0xFE363C8A, 0xFE363C8B, 0xFE363C8C, 0x00000100, 0xFE363C85,
    0x00010022, 0xFE363C85, 0x00010024, 0x00000003, 0x0003005B, (u32)btlevtcmd_StartAvoid,
    0xFE363C8A, 0x00000026, 0x00010024, 0x00000006, 0x0003005B, (u32)btlevtcmd_StartAvoid,
    0xFE363C8A, 0x00000027, 0x00010024, 0x00000002, 0x0003005B, (u32)btlevtcmd_StartAvoid,
    0xFE363C8A, 0x00000028, 0x00010024, 0x00000001, 0x0007005B, (u32)btlevtcmd_CheckDamage,
    0xFFFFFFFE, 0xFE363C8A, 0xFE363C8B, 0xFE363C8C, 0x00000100, 0xFE363C85,
    0x00000031, 0x0003005B, (u32)btlevtcmd_GetDamage, 0xFE363C8A, 0xFE363C80, 0x0002001D,
    0xFE363C80, 0x00000001, 0x0004005B, (u32)btlevtcmd_RecoverHp, 0xFFFFFFFE, 0xFE363C89,
    0xFE363C80, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C85, 0xFE363C86,
    0xFE363C87, 0x0003005B, (u32)btlevtcmd_GetHeight, 0xFFFFFFFE, 0xFE363C88, 0x00020035,
    0xFE363C86, 0xFE363C88, 0x00020035, 0xFE363C87, 0x00000003, 0x000F005B,
    (u32)evt_eff, 0x00000000, (u32)(char*)str_recovery_802f04b8, 0x00000000, 0xFE363C85, 0xFE363C86,
    0xFE363C87, 0xFE363C80, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000021, 0x0003005B, (u32)btlevtcmd_CheckWeaponPluralTarget,
    0xFE363C8C, 0xFE363C80, 0x00020019, 0xFE363C80, 0x00000000, 0x0003005B,
    (u32)btlevtcmd_GetSelectNextEnemy, 0xFE363C8A, 0xFE363C8B, 0x00000020, 0x00020032, 0xFE363C8A,
    0xFFFFFFFF, 0x00000021, 0x00010004, 0x0000000A, 0x00010003, 0x0000005A,
    0x0001005B, (u32)evt_audience_ap_recovery, 0x00010009, 0x0000003C, 0x0001005E, (u32)_return_home_event,
    0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002, 0x00000001,
};

/* Recovered BattleWeapon record. */
u32 ItemWeaponData_Suitooru[48] = {
    0x00000000, 0x00000000, 0x00000090, 0x00000000, 0x64000000, 0x3F800000,
    0x01010101, (u32)weaponGetPowerDefault, 0x00000005, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01101260, 0x20000000, 0x00000302, 0x00000000, 0x00030140,
    0x000007FF, 0x80000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)ItemEvent_Suitooru, 0x00000064, 0x00000000, 0x00000000,
};

u32 ItemEvent_Teki_Kyouka[] = {
    0x00020032, 0xFE363C8C, (u32)ItemWeaponData_Teki_Kyouka, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000,
    0x00000007, 0x0004005B, (u32)evt_btl_camera_set_homing_unit, 0x00000000, 0xFFFFFFFE, 0xFFFFFFFF,
    0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001, 0x0003005B, (u32)evt_btl_camera_set_zoom,
    0x00000000, 0x000000C8, 0x0001000A, 0x000003E8, 0x0003005B, (u32)btlevtcmd_GetFriendBelong,
    0xFFFFFFFE, 0xFE363C80, 0x00020018, 0xFE363C80, 0x00000000, 0x0005005B,
    (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0003005B,
    (u32)btlevtcmd_GetBodyId, 0xFFFFFFFE, 0xFE363C89, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE,
    0xFE363C89, 0x00000028, 0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE, 0xF24A8680,
    0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFFFFFFC4, 0xFE363C81, 0xFE363C82,
    0x00000000, 0xFFFFFFFF, 0x00000000, 0x00000021, 0x0002005B, (u32)btlevtcmd_WeaponAftereffect,
    0xFE363C8C, 0x0003005B, (u32)btlevtcmd_GetBodyId, 0xFFFFFFFE, 0xFE363C89, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0xFE363C89, 0x00000034, 0x0003005B, (u32)btlevtcmd_CommandFlyItem,
    0xFFFFFFFE, 0x00000028, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0xFE363C89,
    0x00000045, 0x0002005B, (u32)btlevtcmd_ConsumeItem, 0xFFFFFFFE, 0x0003005B, (u32)evt_btl_camera_set_mode,
    0x00000000, 0x00000000, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001,
    0x00010009, 0x0000001E, 0x0003005B, (u32)btlevtcmd_CheckCommandUnit, 0xFFFFFFFE, 0xFE363C80,
    0x00020019, 0xFE363C80, 0x00000000, 0x0003005B, (u32)btlevtcmd_CheckWeaponPluralTarget, 0xFE363C8C,
    0xFE363C80, 0x00020019, 0xFE363C80, 0x00000000, 0x0003005B, (u32)btlevtcmd_GetEnemyBelong,
    0xFFFFFFFE, 0xFE363C80, 0x0004005B, (u32)btlevtcmd_SamplingEnemy, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C8C, 0x00000021, 0x0003005B, (u32)btlevtcmd_GetSelectEnemy, 0xFE363C8A, 0xFE363C8B,
    0x00000020, 0x0003005B, (u32)btlevtcmd_CheckWeaponPluralTarget, 0xFE363C8C, 0xFE363C80, 0x00020019,
    0xFE363C80, 0x00000000, 0x0003005B, (u32)btlevtcmd_GetEnemyBelong, 0xFFFFFFFE, 0xFE363C80,
    0x0004005B, (u32)btlevtcmd_SamplingEnemy, 0xFFFFFFFE, 0xFE363C80, 0xFE363C8C, 0x0003005B,
    (u32)btlevtcmd_GetSelectEnemy, 0xFE363C8A, 0xFE363C8B, 0x00000020, 0x0003005B, (u32)btlevtcmd_GetEnemyBelong,
    0xFFFFFFFE, 0xFE363C80, 0x0004005B, (u32)btlevtcmd_SamplingEnemy, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C8C, 0x0004005B, (u32)btlevtcmd_ChoiceSamplingEnemy, 0xFE363C8C, 0xFE363C8A, 0xFE363C8B,
    0x00000021, 0x00000021, 0x00010003, 0x0000000A, 0x00020018, 0xFE363C8A,
    0xFFFFFFFF, 0x00010004, 0x0000005A, 0x00000021, 0x0006005B, (u32)btlevtcmd_snd_se,
    0xFE363C8A, (u32)(char*)str_SFX_ITEM_RANK_UP_ENE_802f0608, 0xF1194D80, 0x00000000, 0xF1194D80, 0x0006005B,
    (u32)btlevtcmd_GetHitPos, 0xFE363C8A, 0xFE363C8B, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00020035, 0xFE363C81, 0x00000014, 0x00020035, 0xFE363C82, 0x00000005,
    0x000F005B, (u32)evt_eff, (u32)&zero_804227c0, (u32)(char*)str_rankup_802f0620, 0x00000000, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0002005B, (u32)_rank_up,
    0xFE363C8A, 0x0003005B, (u32)btlevtcmd_CheckWeaponPluralTarget, 0xFE363C8C, 0xFE363C80, 0x00020019,
    0xFE363C80, 0x00000000, 0x0003005B, (u32)btlevtcmd_GetSelectNextEnemy, 0xFE363C8A, 0xFE363C8B,
    0x00000020, 0x00020032, 0xFE363C8A, 0xFFFFFFFF, 0x00000021, 0x00010004,
    0x0000000A, 0x00010003, 0x0000005A, 0x0001005B, (u32)evt_audience_ap_recovery, 0x00010009,
    0x00000096, 0x0001005E, (u32)_return_home_event, 0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE,
    0x00000002, 0x00000001,
};

/* Recovered BattleWeapon record. */
u32 ItemWeaponData_Teki_Kyouka[48] = {
    0x00000000, 0x00000000, 0x00000091, 0x00000000, 0x64000000, 0x3F800000,
    0x01010101, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x02101260, 0x20000000, 0x00000302, 0x00000000, 0x00030108,
    0x000007FF, 0x80000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)ItemEvent_Teki_Kyouka, 0x00000064, 0x00000000, 0x00000000,
};

/* Recovered BattleWeapon record. */
u32 ItemWeaponData_Minimini_Kun[48] = {
    0x00000000, 0x00000000, 0x00000092, 0x00000000, 0x64000000, 0x3F800000,
    0x01010101, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x02101260, 0x20000000, 0x00000302, 0x00000000, 0x00030000,
    0x000007FF, 0x80000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000064, 0x03FE0000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)ItemEvent_Minimini_Kun, 0x00000064, 0x00000000, 0x00000000,
};

/* Recovered BattleWeapon record. */
u32 ItemWeaponData_Funyafunya_Kun[48] = {
    0x00000000, 0x00000000, 0x00000093, 0x00000000, 0x64000000, 0x3F800000,
    0x01010101, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x02101260, 0x20000000, 0x00000302, 0x00000000, 0x00030000,
    0x000007FF, 0x80000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x006403FD, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)ItemEvent_Funyafunya_Kun, 0x00000064, 0x00000000, 0x00000000,
};

/* Recovered BattleWeapon record. */
u32 ItemWeaponData_Sukkiri_Drink[48] = {
    0x00000000, 0x00000000, 0x00000099, 0x00000000, 0x64000000, 0x3F800000,
    0x01010101, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01100160, 0x10000000, 0x00000302, 0x00000000, 0x00030108,
    0x000007FF, 0x80000000, 0x64006400, 0x64006400, 0x00640000, 0x00000064,
    0x00640064, 0x00000000, 0x00640000, 0x64000000, 0x00006400, 0x00000000,
    0x00000000, 0x00000000, (u32)ItemEvent_Sukkiri_Drink, 0x00000064, 0x00000000, 0x00000000,
};

u32 ItemEvent_Jiwajiwa_Kinoko_Core[] = {
    0x0001005E, (u32)ItemEvent_Support_Sub_Effect, 0x0001005B, (u32)evt_audience_ap_recovery, 0x00010009, 0x00000096,
    0x0001005E, (u32)_return_home_event, 0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002,
    0x00000001,
};

u32 ItemEvent_Jiwajiwa_Kinoko[] = {
    0x00020032, 0xFE363C8C, (u32)ItemWeaponData_Jiwajiwa_Kinoko, 0x0002005B, (u32)btlevtcmd_WeaponAftereffect, 0xFE363C8C,
    0x0001005E, (u32)ItemEvent_Support_Sub_UseDeclere, 0x00010009, 0x0000001E, 0x0001005E, (u32)ItemEvent_Jiwajiwa_Kinoko_Core,
    0x00000002, 0x00000001,
};

/* Recovered BattleWeapon record. */
u32 ItemWeaponData_Jiwajiwa_Kinoko[48] = {
    0x00000000, 0x00000000, 0x0000009D, 0x00000000, 0x64000000, 0x3F800000,
    0x01010101, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01100160, 0x10000000, 0x00000302, 0x00000000, 0x00030108,
    0x000007FF, 0x00001110, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000A02, 0x00000000, (u32)ItemEvent_Jiwajiwa_Kinoko, 0x00000064, 0x00000000, 0x00000000,
};

u32 ItemEvent_Jiwajiwa_Syrup_Core[] = {
    0x0001005E, (u32)ItemEvent_Support_Sub_Effect, 0x0001005B, (u32)evt_audience_ap_recovery, 0x00010009, 0x00000096,
    0x0001005E, (u32)_return_home_event, 0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002,
    0x00000001,
};

u32 ItemEvent_Jiwajiwa_Syrup[] = {
    0x00020032, 0xFE363C8C, (u32)ItemWeaponData_Jiwajiwa_Syrup, 0x0002005B, (u32)btlevtcmd_WeaponAftereffect, 0xFE363C8C,
    0x0001005E, (u32)ItemEvent_Support_Sub_UseDeclere, 0x00010009, 0x0000001E, 0x0001005E, (u32)ItemEvent_Jiwajiwa_Syrup_Core,
    0x00000002, 0x00000001,
};

/* Recovered BattleWeapon record. */
u32 ItemWeaponData_Jiwajiwa_Syrup[48] = {
    0x00000000, 0x00000000, 0x0000009E, 0x00000000, 0x64000000, 0x3F800000,
    0x01010101, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01100160, 0x10000000, 0x00000302, 0x00000000, 0x00030108,
    0x000007FF, 0x80000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x0A020000, (u32)ItemEvent_Jiwajiwa_Syrup, 0x00000064, 0x00000000, 0x00000000,
};

u32 ItemEvent_Irekaeeru[] = {
    0x00020032, 0xFE363C8C, (u32)ItemWeaponData_Irekaeeru, 0x0001005E, (u32)ItemEvent_GetTarget, 0x0002005B,
    (u32)btlevtcmd_WeaponAftereffect, 0xFE363C8C, 0x0001005E, (u32)ItemEvent_Support_Sub_UseDeclere, 0x00010009, 0x0000001E,
    0x0002005B, (u32)_hpfp_change_declare_1, 0xFE363C8A, 0x0006005B, (u32)btlevtcmd_GetHitPos, 0xFE363C8A,
    0xFE363C8B, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00020035, 0xFE363C81,
    0x00000014, 0x00020035, 0xFE363C82, 0x00000005, 0x000F005B, (u32)evt_eff,
    (u32)&zero_804227c0, (u32)(char*)str_irekae_802f0628, 0x00000000, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x0006005B, (u32)btlevtcmd_snd_se, 0xFE363C8A, (u32)(char*)str_SFX_ITEM_HP_FP_CHANG_802f0630,
    0xF1194D80, 0x00000000, 0xF1194D80, 0x00010009, 0x00000078, 0x0004005B,
    (u32)btlevtcmd_CheckAttribute, 0xFE363C8A, 0x00100000, 0xFE363C80, 0x00020018, 0xFE363C80,
    0x00000000, 0x0003005B, (u32)btlevtcmd_GetMaxHp, 0xFE363C8A, 0xFE363C85, 0x0003005B,
    (u32)btlevtcmd_GetHp, 0xFE363C8A, 0xFE363C86, 0x0003005B, (u32)btlevtcmd_GetMaxFp, 0xFE363C8A,
    0xFE363C87, 0x0003005B, (u32)btlevtcmd_GetFp, 0xFE363C8A, 0xFE363C88, 0x0002001B,
    0xFE363C88, 0xFE363C85, 0x00020032, 0xFE363C88, 0xFE363C85, 0x00000021,
    0x0003005B, (u32)btlevtcmd_SetHp, 0xFE363C8A, 0xFE363C88, 0x0002001B, 0xFE363C86,
    0xFE363C87, 0x00020032, 0xFE363C86, 0xFE363C87, 0x00000021, 0x0003005B,
    (u32)btlevtcmd_SetFp, 0xFE363C8A, 0xFE363C86, 0x0002005B, (u32)_hpfp_change_declare_2, 0xFE363C8A,
    0x00020018, 0xFE363C88, 0x00000000, 0x0007005B, (u32)btlevtcmd_PreCheckDamage, 0xFFFFFFFE,
    0xFE363C8A, 0xFE363C8B, 0xFE363C8C, 0x00000100, 0xFE363C85, 0x00010022,
    0xFE363C85, 0x00010024, 0x00000003, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C8A,
    0x00000026, 0x00010024, 0x00000006, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C8A,
    0x00000027, 0x00010024, 0x00000002, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C8A,
    0x00000028, 0x00010024, 0x00000001, 0x0007005B, (u32)btlevtcmd_CheckDamage, 0xFFFFFFFE,
    0xFE363C8A, 0xFE363C8B, 0xFE363C8C, 0x00000100, 0xFE363C85, 0x00000031,
    0x00000021, 0x00000021, 0x0001005B, (u32)evt_audience_ap_recovery, 0x00010009, 0x0000001E,
    0x0001005E, (u32)_return_home_event, 0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002,
    0x00000001,
};

/* Recovered BattleWeapon record. */
u32 ItemWeaponData_Irekaeeru[48] = {
    0x00000000, 0x00000000, 0x000000A1, 0x00000000, 0x64000000, 0x3F800000,
    0x01010101, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01100160, 0x10000000, 0x001B0302, 0x00000000, 0x00030108,
    0x000007FF, 0x80000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)ItemEvent_Irekaeeru, 0x00000064, 0x00000000, 0x00000000,
};

u32 ItemEvent_Attiike_Shisshi[] = {
    0x00020032, 0xFE363C8C, (u32)ItemWeaponData_Attiike_Shisshi, 0x0002005B, (u32)btlevtcmd_WeaponAftereffect, 0xFE363C8C,
    0x0001005E, (u32)ItemEvent_Support_Sub_UseDeclere, 0x00010009, 0x0000001E, 0x0006005B, (u32)btlevtcmd_snd_se,
    0xFFFFFFFE, (u32)(char*)str_SFX_ITEM_HORROR1_802f0648, 0xF1194D80, 0x00000000, 0xF1194D80, 0x0000006E,
    0x00010009, 0x0000003C, 0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_ITEM_HORROR2_802f065c,
    0xF1194D80, 0x00000000, 0xF1194D80, 0x00000070, 0x0005005B, (u32)btlevtcmd_GetPos,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000F005B, (u32)evt_eff64,
    0x00000000, (u32)(char*)str_holography_n64_802f0670, 0x00000000, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0xF24A7E80, 0x00000078, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x0000006E, 0x0001000A, 0x00000190, 0x0003005B,
    (u32)evt_env_blur_on, 0x00000000, 0x00000640, 0x00000070, 0x0000006E, 0x0001000A,
    0x00000190, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000003, 0x000A005B,
    (u32)evt_btl_camera_set_moveto, 0x00000000, 0x00000000, 0x0000005A, 0x000001C2, 0x00000000,
    0x0000003C, 0x00000000, 0x0000001E, 0x0000000B, 0x0001000A, 0x000000FA,
    0x000A005B, (u32)evt_btl_camera_set_moveto, 0x00000000, 0x00000000, 0x0000006E, 0x000002EE,
    0x00000000, 0x0000003C, 0x00000000, 0x0000001E, 0x0000000B, 0x0001000A,
    0x000000FA, 0x000A005B, (u32)evt_btl_camera_set_moveto, 0x00000000, 0x00000000, 0x0000005A,
    0x000001C2, 0x00000000, 0x0000003C, 0x00000000, 0x0000001E, 0x0000000B,
    0x0001000A, 0x000000FA, 0x000A005B, (u32)evt_btl_camera_set_moveto, 0x00000000, 0x00000000,
    0x0000006E, 0x000002EE, 0x00000000, 0x0000003C, 0x00000000, 0x0000001E,
    0x0000000B, 0x0001000A, 0x000000FA, 0x000A005B, (u32)evt_btl_camera_set_moveto, 0x00000000,
    0x00000000, 0x0000005A, 0x000001C2, 0x00000000, 0x0000003C, 0x00000000,
    0x0000001E, 0x0000000B, 0x0001000A, 0x000000FA, 0x000A005B, (u32)evt_btl_camera_set_moveto,
    0x00000000, 0x00000000, 0x0000006E, 0x000002EE, 0x00000000, 0x0000003C,
    0x00000000, 0x0000001E, 0x0000000B, 0x0001000A, 0x000000FA, 0x0003005B,
    (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000,
    0x00000001, 0x00000070, 0x0001000A, 0x000007D0, 0x0001005E, (u32)ItemEvent_Support_Sub_Effect,
    0x0001005B, (u32)evt_audience_ap_recovery, 0x0001000A, 0x000009C4, 0x0001005E, (u32)_return_home_event,
    0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002, 0x00000001,
};

/* Recovered BattleWeapon record. */
u32 ItemWeaponData_Attiike_Shisshi[48] = {
    0x00000000, 0x00000000, 0x000000A2, 0x00000000, 0x64000000, 0x3F800000,
    0x01010101, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x02101260, 0x20000000, 0x00000302, 0x00000000, 0x00030000,
    0x000007FF, 0x80000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x64000000,
    0x00000000, 0x00000000, (u32)ItemEvent_Attiike_Shisshi, 0x00000064, 0x00000000, 0x00000000,
};

u32 ItemEvent_Naniga_Okorukana[] = {
    0x00020032, 0xFE363C8C, (u32)ItemWeaponData_Naniga_Okorukana, 0x0001005E, (u32)ItemEvent_Support_Sub_UseDeclere, 0x00010009,
    0x0000001E, 0x0003005B, (u32)_naniga_okorukana_check, 0xFFFFFFFE, 0xFE363C8D, 0x00010022,
    0xFE363C8D, 0x00010024, 0x00000094, 0x00020032, 0xFE363C8C, (u32)ItemWeaponData_Kinoko,
    0x00020032, 0xFE363C88, 0xFE363C8C, 0x0001005E, (u32)ItemEvent_Automatic_Target, 0x00020032,
    0xFE363C89, 0x00000094, 0x0001005E, (u32)ItemEvent_Recovery_Core, 0x00010024, 0x00000096,
    0x00020032, 0xFE363C8C, (u32)ItemWeaponData_Ultra_Kinoko, 0x00020032, 0xFE363C88, 0xFE363C8C,
    0x0001005E, (u32)ItemEvent_Automatic_Target, 0x00020032, 0xFE363C89, 0x00000096, 0x0001005E,
    (u32)ItemEvent_Recovery_Core, 0x00010024, 0x00000098, 0x00020032, 0xFE363C8C, (u32)ItemWeaponData_Sinabita_Kinoko,
    0x00020032, 0xFE363C88, 0xFE363C8C, 0x0001005E, (u32)ItemEvent_Automatic_Target, 0x00020032,
    0xFE363C89, 0x00000098, 0x0001005E, (u32)ItemEvent_Recovery_Core, 0x00010024, 0x0000009A,
    0x00020032, 0xFE363C8C, (u32)ItemWeaponData_Honey_Syrup, 0x00020032, 0xFE363C88, 0xFE363C8C,
    0x0001005E, (u32)ItemEvent_Automatic_Target, 0x00020032, 0xFE363C89, 0x0000009A, 0x0001005E,
    (u32)ItemEvent_Recovery_Core, 0x00010024, 0x0000009C, 0x00020032, 0xFE363C8C, (u32)ItemWeaponData_Royal_Jelly,
    0x00020032, 0xFE363C88, 0xFE363C8C, 0x0001005E, (u32)ItemEvent_Automatic_Target, 0x00020032,
    0xFE363C89, 0x0000009C, 0x0001005E, (u32)ItemEvent_Recovery_Core, 0x00010024, 0x00000087,
    0x00020032, 0xFE363C8C, (u32)ItemWeaponData_Biribiri_Kinoko, 0x00020032, 0xFE363C88, 0xFE363C8C,
    0x0001005E, (u32)ItemEvent_Automatic_Target, 0x0001005E, (u32)ItemEvent_Biribiri_Kinoko_Core, 0x00010024, 0x00000088,
    0x00020032, 0xFE363C8C, (u32)ItemWeaponData_Teki_Yokeeru, 0x00020032, 0xFE363C88, 0xFE363C8C,
    0x0001005E, (u32)ItemEvent_Automatic_Target, 0x0001005E, (u32)ItemEvent_Teki_Yokeeru_Core, 0x00010024, 0x0000009D,
    0x00020032, 0xFE363C8C, (u32)ItemWeaponData_Jiwajiwa_Kinoko, 0x00020032, 0xFE363C88, 0xFE363C8C,
    0x0001005E, (u32)ItemEvent_Automatic_Target, 0x0001005E, (u32)ItemEvent_Jiwajiwa_Kinoko_Core, 0x00010024, 0x0000009E,
    0x00020032, 0xFE363C8C, (u32)ItemWeaponData_Jiwajiwa_Syrup, 0x00020032, 0xFE363C88, 0xFE363C8C,
    0x0001005E, (u32)ItemEvent_Automatic_Target, 0x0001005E, (u32)ItemEvent_Jiwajiwa_Syrup_Core, 0x00010024, 0x00000080,
    0x00020032, 0xFE363C8C, (u32)ItemWeaponData_Kaminari_Gorogoro, 0x00020032, 0xFE363C88, 0xFE363C8C,
    0x0001005E, (u32)ItemEvent_Automatic_Target, 0x0001005E, (u32)ItemEvent_Kaminari_Core, 0x00010024, 0x00000081,
    0x00020032, 0xFE363C8C, (u32)ItemWeaponData_Kaminari_Dokkan, 0x00020032, 0xFE363C88, 0xFE363C8C,
    0x0001005E, (u32)ItemEvent_Automatic_Target, 0x0001005E, (u32)ItemEvent_Kaminari_Core, 0x00010024, 0x00000084,
    0x00020032, 0xFE363C8C, (u32)ItemWeaponData_Fire_Flower, 0x00020032, 0xFE363C88, 0xFE363C8C,
    0x0001005E, (u32)ItemEvent_Automatic_Target, 0x0001005E, (u32)ItemEvent_Fire_Flower_Core, 0x00010024, 0x00000082,
    0x00020032, 0xFE363C8C, (u32)ItemWeaponData_Kirakira_Otoshi, 0x00020032, 0xFE363C88, 0xFE363C8C,
    0x0001005E, (u32)ItemEvent_Automatic_Target, 0x0001005E, (u32)ItemEvent_Kirakira_Otoshi_Core, 0x00010024, 0x0000008C,
    0x00020032, 0xFE363C8C, (u32)ItemWeaponData_Stop_Watch, 0x00020032, 0xFE363C88, 0xFE363C8C,
    0x0001005E, (u32)ItemEvent_Automatic_Target, 0x0001005E, (u32)ItemEvent_Stop_Watch_Core, 0x0000002A, 0x0002005B,
    (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002, 0x00000031, 0x0002005B, (u32)btlevtcmd_StartWaitEvent,
    0xFFFFFFFE, 0x00000002, 0x00000001,
};

/* Recovered BattleWeapon record. */
u32 ItemWeaponData_Naniga_Okorukana[48] = {
    0x00000000, 0x00000000, 0x000000A3, 0x00000000, 0x64000000, 0x3F800000,
    0x01010101, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x02100060, 0x10000000, 0x00000302, 0x00000000, 0x00030108,
    0x000007FF, 0x80000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)ItemEvent_Naniga_Okorukana, 0x00000064, 0x00000000, 0x00000000,
};

/* Recovered mystery-item lookup table. */
s32 R_mystery_item_table[14] = {
    0x00000094, 0x0000009A, 0x0000009C, 0x00000087, 0x00000088, 0x0000009D,
    0x0000009E, 0x00000096, 0x00000098, 0x00000081, 0x00000084, 0x00000082,
    0x0000008C, 0x00000000,
};

/* Recovered BattleWeapon record. */
u32 ItemWeaponData_Shikaeshino_Kona[48] = {
    0x00000000, 0x00000000, 0x000000B1, 0x00000000, 0x64000000, 0x3F800000,
    0x01010101, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01100160, 0x10000000, 0x00000302, 0x00000000, 0x00030108,
    0x000007FF, 0x80000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000300,
    0x00000000, 0x00000000, (u32)ItemEvent_Shikaeshino_Kona, 0x00000064, 0x00000000, 0x00000000,
};

/* Recovered BattleWeapon record. */
u32 ItemWeaponData_Kameno_Noroi[48] = {
    0x00000000, 0x00000000, 0x000000B2, 0x00000000, 0x64000000, 0x3F800000,
    0x01010101, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01101260, 0x20000000, 0x00000302, 0x00000000, 0x00030008,
    0x000007FF, 0x80000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00006403, 0x00000000,
    0x00000000, 0x00000000, (u32)ItemEvent_Kameno_Noroi, 0x00000064, 0x00000000, 0x00000000,
};

u32 ItemEvent_RecoveryAndStatus[] = {
    0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress, 0xFFFFFFFE, 0xFE363C88, 0x0002005B, (u32)btlevtcmd_WeaponAftereffect,
    0xFE363C88, 0x0003005B, (u32)btlevtcmd_CheckCommandUnit, 0xFFFFFFFE, 0xFE363C80, 0x00020019,
    0xFE363C80, 0x00000000, 0x0003005B, (u32)btlevtcmd_GetSelectEnemy, 0xFE363C8A, 0xFE363C8B,
    0x00000020, 0x0003005B, (u32)btlevtcmd_GetEnemyBelong, 0xFFFFFFFE, 0xFE363C80, 0x0004005B,
    (u32)btlevtcmd_SamplingEnemy, 0xFFFFFFFE, 0xFE363C80, 0xFE363C88, 0x0004005B, (u32)btlevtcmd_ChoiceSamplingEnemy,
    0xFE363C88, 0xFE363C8A, 0xFE363C8B, 0x00000021, 0x0003005B, (u32)evt_btl_camera_set_mode,
    0x00000000, 0x00000007, 0x0004005B, (u32)evt_btl_camera_set_homing_unit, 0x00000000, 0xFFFFFFFE,
    0xFFFFFFFF, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001, 0x0003005B,
    (u32)evt_btl_camera_set_zoom, 0x00000000, 0x000000C8, 0x0001000A, 0x000003E8, 0x0003005B,
    (u32)btlevtcmd_GetFriendBelong, 0xFFFFFFFE, 0xFE363C80, 0x00020018, 0xFE363C80, 0x00000000,
    0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0003005B, (u32)btlevtcmd_GetBodyId, 0xFFFFFFFE, 0xFE363C89, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType,
    0xFFFFFFFE, 0xFE363C89, 0x00000028, 0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE,
    0xF24A8680, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFFFFFFC4, 0xFE363C81,
    0xFE363C82, 0x00000000, 0xFFFFFFFF, 0x00000000, 0x00000021, 0x0003005B,
    (u32)btlevtcmd_GetBodyId, 0xFFFFFFFE, 0xFE363C89, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE,
    0xFE363C89, 0x00000034, 0x0003005B, (u32)btlevtcmd_CommandFlyItem, 0xFFFFFFFE, 0x0000003C,
    0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0xFE363C89, 0x00000045, 0x0002005B,
    (u32)btlevtcmd_ConsumeItem, 0xFFFFFFFE, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponItemId, 0xFE363C89, 0xFE363C88,
    0x00020018, 0xFE363C89, 0x00000000, 0x0002005B, (u32)btlevtcmd_GetConsumeItem, 0xFE363C89,
    0x00000021, 0x0001005E, (u32)ItemEvent_Recovery_Core_Effect, 0x00010022, 0xFE363C89, 0x00010024,
    0x000000D8, 0x0003005B, (u32)evt_sub_random, 0x00000064, 0xFE363C80, 0x0002001A,
    0xFE363C80, 0x00000028, 0x00020032, 0xFE363C88, (u32)ItemWeaponData_FirstLovePuddingBiribiri, 0x00000020,
    0x0002001A, 0xFE363C80, 0x00000050, 0x00020032, 0xFE363C88, (u32)ItemWeaponData_FirstLovePuddingTrans,
    0x00000020, 0x00020032, 0xFE363C88, (u32)ItemWeaponData_FirstLovePuddingSleep, 0x00000021, 0x00000021,
    0x00010024, 0x000000E2, 0x0003005B, (u32)evt_sub_random, 0x00000064, 0xFE363C80,
    0x0002001A, 0xFE363C80, 0x00000028, 0x00020032, 0xFE363C88, (u32)ItemWeaponData_PeachTaltBiribiri,
    0x00000020, 0x0002001A, 0xFE363C80, 0x00000050, 0x00020032, 0xFE363C88,
    (u32)ItemWeaponData_PeachTaltDodge, 0x00000020, 0x00020032, 0xFE363C88, (u32)ItemWeaponData_PeachTaltSleep, 0x00000021,
    0x00000021, 0x0001002B, 0x000000E9, 0x0001002B, 0x000000EB, 0x0005005B,
    (u32)btlevtcmd_GetPos, 0xFE363C8A, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0006005B,
    (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_REST_CONDITION1_802f04c4, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000,
    0x0000002E, 0x00000031, 0x00020032, 0xFE363C8C, 0xFE363C88, 0x0007005B,
    (u32)btlevtcmd_PreCheckDamage, 0xFFFFFFFE, 0xFE363C8A, 0xFE363C8B, 0xFE363C8C, 0x00000100,
    0xFE363C85, 0x00010022, 0xFE363C85, 0x00010024, 0x00000003, 0x0003005B,
    (u32)btlevtcmd_StartAvoid, 0xFE363C8A, 0x00000026, 0x00010024, 0x00000006, 0x0003005B,
    (u32)btlevtcmd_StartAvoid, 0xFE363C8A, 0x00000027, 0x00010024, 0x00000002, 0x0003005B,
    (u32)btlevtcmd_StartAvoid, 0xFE363C8A, 0x00000028, 0x00010024, 0x00000001, 0x0007005B,
    (u32)btlevtcmd_CheckDamage, 0xFFFFFFFE, 0xFE363C8A, 0xFE363C8B, 0xFE363C8C, 0x00000100,
    0xFE363C85, 0x00000031, 0x0001005E, (u32)_return_home_event, 0x0002005B, (u32)btlevtcmd_StartWaitEvent,
    0xFE363C8A, 0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002, 0x00000001,
};

/* Recovered BattleWeapon record. */
u32 ItemWeaponData_SpaceFood[48] = {
    0x00000000, 0x00000000, 0x000000C6, 0x00000000, 0x64000000, 0x3F800000,
    0x01010101, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01100160, 0x10000000, 0x00000302, 0x00000000, 0x00030108,
    0x000007FF, 0x80000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x32030000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)ItemEvent_RecoveryAndStatus, 0x00000064, 0x00000000, 0x00000000,
};

/* Recovered BattleWeapon record. */
u32 ItemWeaponData_IceCandy[48] = {
    0x00000000, 0x00000000, 0x000000C7, 0x00000000, 0x64000000, 0x3F800000,
    0x01010101, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01100160, 0x10000000, 0x00000302, 0x00000000, 0x00030108,
    0x000007FF, 0x80000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00140200, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)ItemEvent_RecoveryAndStatus, 0x00000064, 0x00000000, 0x00000000,
};

/* Recovered BattleWeapon record. */
u32 ItemWeaponData_NancyFrappe[48] = {
    0x00000000, 0x00000000, 0x000000C8, 0x00000000, 0x64000000, 0x3F800000,
    0x01010101, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01100160, 0x10000000, 0x00000302, 0x00000000, 0x00030108,
    0x000007FF, 0x80000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00140200, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)ItemEvent_RecoveryAndStatus, 0x00000064, 0x00000000, 0x00000000,
};

/* Recovered BattleWeapon record. */
u32 ItemWeaponData_SnowRabbit[48] = {
    0x00000000, 0x00000000, 0x000000C9, 0x00000000, 0x64000000, 0x3F800000,
    0x01010101, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01100160, 0x10000000, 0x00000302, 0x00000000, 0x00030108,
    0x000007FF, 0x80000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00320300, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)ItemEvent_RecoveryAndStatus, 0x00000064, 0x00000000, 0x00000000,
};

/* Recovered BattleWeapon record. */
u32 ItemWeaponData_TeaKinoko[48] = {
    0x00000000, 0x00000000, 0x000000D4, 0x00000000, 0x64000000, 0x3F800000,
    0x01010101, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01100160, 0x10000000, 0x00000302, 0x00000000, 0x00030108,
    0x000007FF, 0x80000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x006403FF, 0x00000000, 0x00000000, 0x00000000,
    0x00000502, 0x00000000, (u32)ItemEvent_RecoveryAndStatus, 0x00000064, 0x00000000, 0x00000000,
};

/* Recovered BattleWeapon record. */
u32 ItemWeaponData_FirstLovePuddingBiribiri[48] = {
    0x00000000, 0x00000000, 0x000000D8, 0x00000000, 0x64000000, 0x3F800000,
    0x01010101, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01100160, 0x10000000, 0x00000302, 0x00000000, 0x00030108,
    0x000007FF, 0x80000000, 0x00000000, 0x00000000, 0x00000064, 0x02000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)ItemEvent_RecoveryAndStatus, 0x00000064, 0x00000000, 0x00000000,
};

/* Recovered BattleWeapon record. */
u32 ItemWeaponData_FirstLovePuddingTrans[48] = {
    0x00000000, 0x00000000, 0x000000D8, 0x00000000, 0x64000000, 0x3F800000,
    0x01010101, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01100160, 0x10000000, 0x00000302, 0x00000000, 0x00030108,
    0x000007FF, 0x80000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x64020000, 0x00000000, (u32)ItemEvent_RecoveryAndStatus, 0x00000064, 0x00000000, 0x00000000,
};

/* Recovered BattleWeapon record. */
u32 ItemWeaponData_FirstLovePuddingSleep[48] = {
    0x00000000, 0x00000000, 0x000000D8, 0x00000000, 0x64000000, 0x3F800000,
    0x01010101, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01100160, 0x10000000, 0x00000302, 0x00000000, 0x00030108,
    0x000007FF, 0x80000000, 0x64020000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)ItemEvent_RecoveryAndStatus, 0x00000064, 0x00000000, 0x00000000,
};

/* Recovered BattleWeapon record. */
u32 ItemWeaponData_StarryDinner[48] = {
    0x00000000, 0x00000000, 0x000000D9, 0x00000000, 0x64000000, 0x3F800000,
    0x01010101, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01100160, 0x10000000, 0x00000302, 0x00000000, 0x00030108,
    0x000007FF, 0x80000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000302, 0x00000000, (u32)ItemEvent_RecoveryAndStatus, 0x00000064, 0x00000000, 0x00000000,
};

/* Recovered BattleWeapon record. */
u32 ItemWeaponData_KararinaPasta[48] = {
    0x00000000, 0x00000000, 0x000000E0, 0x00000000, 0x64000000, 0x3F800000,
    0x01010101, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01100160, 0x10000000, 0x00000302, 0x00000000, 0x00030108,
    0x000007FF, 0x80000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000001, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)ItemEvent_RecoveryAndStatus, 0x00000064, 0x00000000, 0x00000000,
};

/* Recovered BattleWeapon record. */
u32 ItemWeaponData_MeromeroCake[48] = {
    0x00000000, 0x00000000, 0x000000E1, 0x00000000, 0x64000000, 0x3F800000,
    0x01010101, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01100160, 0x10000000, 0x00000302, 0x00000000, 0x00030108,
    0x000007FF, 0x80000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x006403FF, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)ItemEvent_RecoveryAndStatus, 0x00000064, 0x00000000, 0x00000000,
};

/* Recovered BattleWeapon record. */
u32 ItemWeaponData_PeachTaltBiribiri[48] = {
    0x00000000, 0x00000000, 0x000000E2, 0x00000000, 0x64000000, 0x3F800000,
    0x01010101, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01100160, 0x10000000, 0x00000302, 0x00000000, 0x00030108,
    0x000007FF, 0x80000000, 0x00000000, 0x00000000, 0x00000064, 0x02000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)ItemEvent_RecoveryAndStatus, 0x00000064, 0x00000000, 0x00000000,
};

/* Recovered BattleWeapon record. */
u32 ItemWeaponData_PeachTaltDodge[48] = {
    0x00000000, 0x00000000, 0x000000E2, 0x00000000, 0x64000000, 0x3F800000,
    0x01010101, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01100160, 0x10000000, 0x00000302, 0x00000000, 0x00030108,
    0x000007FF, 0x80000000, 0x00000000, 0x00000000, 0x00000000, 0x00640200,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)ItemEvent_RecoveryAndStatus, 0x00000064, 0x00000000, 0x00000000,
};

/* Recovered BattleWeapon record. */
u32 ItemWeaponData_PeachTaltSleep[48] = {
    0x00000000, 0x00000000, 0x000000E2, 0x00000000, 0x64000000, 0x3F800000,
    0x01010101, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01100160, 0x10000000, 0x00000302, 0x00000000, 0x00030108,
    0x000007FF, 0x80000000, 0x64020000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)ItemEvent_RecoveryAndStatus, 0x00000064, 0x00000000, 0x00000000,
};

/* Recovered BattleWeapon record. */
u32 ItemWeaponData_BiribiriCandy[48] = {
    0x00000000, 0x00000000, 0x000000E3, 0x00000000, 0x64000000, 0x3F800000,
    0x01010101, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01100160, 0x10000000, 0x00000302, 0x00000000, 0x00030108,
    0x000007FF, 0x80000000, 0x00000000, 0x00000000, 0x00000064, 0x04000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)ItemEvent_RecoveryAndStatus, 0x00000064, 0x00000000, 0x00000000,
};

/* Recovered BattleWeapon record. */
u32 ItemWeaponData_HealthySalad[48] = {
    0x00000000, 0x00000000, 0x000000E9, 0x00000000, 0x64000000, 0x3F800000,
    0x01010101, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01100160, 0x10000000, 0x00000302, 0x00000000, 0x00030108,
    0x000007FF, 0x80000000, 0x64006400, 0x64006400, 0x00640000, 0x00000064,
    0x00640064, 0x00000000, 0x00640000, 0x64000000, 0x00006400, 0x00000000,
    0x00000000, 0x00000000, (u32)ItemEvent_RecoveryAndStatus, 0x00000064, 0x00000000, 0x00000000,
};

/* Recovered BattleWeapon record. */
u32 ItemWeaponData_FreshJuice[48] = {
    0x00000000, 0x00000000, 0x000000EB, 0x00000000, 0x64000000, 0x3F800000,
    0x01010101, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01100160, 0x10000000, 0x00000302, 0x00000000, 0x00030108,
    0x000007FF, 0x80000000, 0x64006400, 0x64006400, 0x00640000, 0x00000064,
    0x00640064, 0x00000000, 0x00640000, 0x64000000, 0x00006400, 0x00000000,
    0x00000000, 0x00000000, (u32)ItemEvent_RecoveryAndStatus, 0x00000064, 0x00000000, 0x00000000,
};

/* Recovered BattleWeapon record. */
u32 ItemWeaponData_RedKararing[48] = {
    0x00000000, 0x00000000, 0x000000AD, 0x00000000, 0x64000000, 0x3F800000,
    0x01010101, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01100160, 0x10000000, 0x00000302, 0x00000000, 0x00030108,
    0x000007FF, 0x80000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000001, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)ItemEvent_RecoveryAndStatus, 0x00000064, 0x00000000, 0x00000000,
};

/* Recovered BattleWeapon record. */
u32 ItemWeaponData_FutarideForever[48] = {
    0x00000000, 0x00000000, 0x000000DB, 0x00000000, 0x64000000, 0x3F800000,
    0x01010101, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01101160, 0x10000000, 0x00000302, 0x00000000, 0x00030108,
    0x000007FF, 0x80000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000A02, 0x00000000, (u32)ItemEvent_RecoveryAndStatus, 0x00000064, 0x00000000, 0x00000000,
};

u32 ItemEvent_ThrowAttack[] = {
    0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress, 0xFFFFFFFE, 0xFE363C8C, 0x0003005B, (u32)evt_btl_camera_set_mode,
    0x00000000, 0x00000007, 0x0004005B, (u32)evt_btl_camera_set_homing_unit, 0x00000000, 0xFFFFFFFE,
    0xFFFFFFFF, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001, 0x0003005B,
    (u32)evt_btl_camera_set_zoom, 0x00000000, 0x000000C8, 0x0001000A, 0x000003E8, 0x0003005B,
    (u32)btlevtcmd_GetFriendBelong, 0xFFFFFFFE, 0xFE363C80, 0x00020018, 0xFE363C80, 0x00000000,
    0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0003005B, (u32)btlevtcmd_GetBodyId, 0xFFFFFFFE, 0xFE363C89, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType,
    0xFFFFFFFE, 0xFE363C89, 0x00000028, 0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE,
    0xF24A8680, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFFFFFFC4, 0xFE363C81,
    0xFE363C82, 0x00000000, 0xFFFFFFFF, 0x00000000, 0x00000021, 0x0003005B,
    (u32)btlevtcmd_GetBodyId, 0xFFFFFFFE, 0xFE363C89, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE,
    0xFE363C89, 0x00000034, 0x0003005B, (u32)btlevtcmd_CommandFlyItem, 0xFFFFFFFE, 0x00000028,
    0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0xFE363C89, 0x00000045, 0x0003005B,
    (u32)btlevtcmd_CommandGetWeaponItemId, 0xFE363C89, 0xFE363C8C, 0x00020018, 0xFE363C89, 0x00000000,
    0x0002005B, (u32)btlevtcmd_GetConsumeItem, 0xFE363C89, 0x00000021, 0x0002005B, (u32)btlevtcmd_ConsumeItem,
    0xFFFFFFFE, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000, 0x0003005B,
    (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000001, 0x00010009, 0x0000001E, 0x0002005B,
    (u32)btlevtcmd_WeaponAftereffect, 0xFE363C8C, 0x0003005B, (u32)btlevtcmd_CheckCommandUnit, 0xFFFFFFFE, 0xFE363C80,
    0x00020019, 0xFE363C80, 0x00000000, 0x0003005B, (u32)btlevtcmd_CheckWeaponPluralTarget, 0xFE363C8C,
    0xFE363C80, 0x00020019, 0xFE363C80, 0x00000000, 0x0003005B, (u32)btlevtcmd_GetEnemyBelong,
    0xFFFFFFFE, 0xFE363C80, 0x0004005B, (u32)btlevtcmd_SamplingEnemy, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C8C, 0x00000021, 0x0003005B, (u32)btlevtcmd_GetSelectEnemy, 0xFE363C8A, 0xFE363C8B,
    0x00000020, 0x0003005B, (u32)btlevtcmd_CheckWeaponPluralTarget, 0xFE363C8C, 0xFE363C80, 0x00020019,
    0xFE363C80, 0x00000000, 0x0003005B, (u32)btlevtcmd_GetEnemyBelong, 0xFFFFFFFE, 0xFE363C80,
    0x0004005B, (u32)btlevtcmd_SamplingEnemy, 0xFFFFFFFE, 0xFE363C80, 0xFE363C8C, 0x0003005B,
    (u32)btlevtcmd_GetSelectEnemy, 0xFE363C8A, 0xFE363C8B, 0x00000020, 0x0003005B, (u32)btlevtcmd_GetEnemyBelong,
    0xFFFFFFFE, 0xFE363C80, 0x0004005B, (u32)btlevtcmd_SamplingEnemy, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C8C, 0x0004005B, (u32)btlevtcmd_ChoiceSamplingEnemy, 0xFE363C8C, 0xFE363C8A, 0xFE363C8B,
    0x00000021, 0x00000021, 0x0003005B, (u32)btlevtcmd_GetUnitKind, 0xFFFFFFFE, 0xFE363C80,
    0x00020018, 0xFE363C80, 0x000000DE, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE,
    0x00000001, (u32)(char*)str_M_C_1_804227dc, 0x0000006E, 0x0001000A, 0x000001F4, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001, 0x0000002B, 0x00000070, 0x00000021,
    0x0003005B, (u32)btlevtcmd_CheckWeaponPluralTarget, 0xFE363C8C, 0xFE363C80, 0x00020018, 0xFE363C80,
    0x00000000, 0x0006005B, (u32)btlevtcmd_GetHitPos, 0xFE363C8A, 0xFE363C8B, 0xFE363C85,
    0xFE363C86, 0xFE363C87, 0x00000020, 0x0003005B, (u32)btlevtcmd_GetFriendBelong, 0xFE363C8A,
    0xFE363C80, 0x00020018, 0xFE363C80, 0x00000000, 0x0005005B, (u32)btlevtcmd_GetPos,
    0xFFFFFFFE, 0xFE363C85, 0xFE363C86, 0xFE363C87, 0x00020032, 0xFE363C85,
    0xFFFFFF88, 0x00000020, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C85,
    0xFE363C86, 0xFE363C87, 0x00020032, 0xFE363C85, 0x00000064, 0x00000021,
    0x0004005B, (u32)btlevtcmd_TransStageFloorPosition, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000021,
    0x0005005B, (u32)_get_throw_first_pos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0008005B, (u32)_throw_item_icon, 0xFE363C89, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0xFE363C85, 0xFE363C86, 0xFE363C87, 0x00010022, 0xFE363C89, 0x00010024,
    0x000000C4, 0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_BTL_THUNDERS_ATT_802f0680, 0xFE363C85, 0xFE363C86,
    0xFE363C87, 0x00000000, 0x00020032, 0xFE363C80, 0xFE363C85, 0x00020032,
    0xFE363C81, 0xFE363C86, 0x00020032, 0xFE363C82, 0xFE363C87, 0x00020035,
    0xFE363C80, 0x00000064, 0x000F005B, (u32)evt_eff, 0x00000000, (u32)(char*)str_hibashira_802f069c,
    0x00000005, 0xFE363C85, 0xFE363C86, 0xFE363C86, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000006, 0xF24A8019, 0x0000003C, 0x00000000, 0x00000000,
    0x00020036, 0xFE363C80, 0x000000C8, 0x000F005B, (u32)evt_eff, 0x00000000,
    (u32)(char*)str_hibashira_802f069c, 0x00000005, 0xFE363C85, 0xFE363C86, 0xFE363C86, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000006, 0xF24A8019, 0x0000003C, 0x00000000,
    0x00000000, 0x0000006B, 0x00020035, 0xFE363C87, 0x00000014, 0x0004005B,
    (u32)_nancy_mirage, 0xFE363C85, 0xFE363C86, 0xFE363C87, 0x0000006D, 0x0001000A,
    0x000001F4, 0x0001002B, 0x000000CA, 0x0001002B, 0x000000D1, 0x000F005B,
    (u32)evt_eff, 0x00000000, (u32)(char*)str_bomb_804227e4, 0x00000000, 0xFE363C85, 0xFE363C86,
    0xFE363C87, 0xF24A7E80, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_BTL_THUNDERS_ATT_802f06a8,
    0xFE363C85, 0xFE363C86, 0xFE363C87, 0x00000000, 0x0000002E, 0x00000031,
    0x00010003, 0x0000000A, 0x00020018, 0xFE363C8A, 0xFFFFFFFF, 0x00010004,
    0x0000005A, 0x00000021, 0x0007005B, (u32)btlevtcmd_PreCheckDamage, 0xFFFFFFFE, 0xFE363C8A,
    0xFE363C8B, 0xFE363C8C, 0x00000100, 0xFE363C85, 0x00010022, 0xFE363C85,
    0x00010024, 0x00000003, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C8A, 0x00000026,
    0x00010024, 0x00000006, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C8A, 0x00000027,
    0x00010024, 0x00000002, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C8A, 0x00000028,
    0x00010024, 0x00000001, 0x0007005B, (u32)btlevtcmd_CheckDamage, 0xFFFFFFFE, 0xFE363C8A,
    0xFE363C8B, 0xFE363C8C, 0x00000100, 0xFE363C85, 0x00000031, 0x0003005B,
    (u32)btlevtcmd_CheckWeaponPluralTarget, 0xFE363C8C, 0xFE363C80, 0x00020019, 0xFE363C80, 0x00000000,
    0x0003005B, (u32)btlevtcmd_GetSelectNextEnemy, 0xFE363C8A, 0xFE363C8B, 0x00000020, 0x00020032,
    0xFE363C8A, 0xFFFFFFFF, 0x00000021, 0x00010004, 0x0000000A, 0x00010003,
    0x0000005A, 0x0001005B, (u32)evt_audience_ap_recovery, 0x00010009, 0x0000001E, 0x0001005E,
    (u32)_return_home_event, 0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002, 0x00000001,
};

/* Recovered BattleWeapon record. */
u32 ItemWeaponData_NancyDynamite[48] = {
    0x00000000, 0x00000000, 0x000000C4, 0x00000000, 0x64000000, 0x3F800000,
    0x01010101, (u32)weaponGetPowerDefault, 0x00000007, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x02101260, 0x20000000, 0x03000302, 0x00000000, 0x00030040,
    0x000007FF, 0x80000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)ItemEvent_ThrowAttack, 0x00000064, 0x00050505, 0x05000000,
};

/* Recovered BattleWeapon record. */
u32 ItemWeaponData_CoconutsBomb[48] = {
    0x00000000, 0x00000000, 0x000000CA, 0x00000000, 0x64000000, 0x3F800000,
    0x01010101, (u32)weaponGetPowerDefault, 0x00000007, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01101260, 0x20000000, 0x03000302, 0x00000000, 0x00030040,
    0x000007FF, 0x80000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)ItemEvent_ThrowAttack, 0x00000064, 0x00050505, 0x05000000,
};

/* Recovered BattleWeapon record. */
u32 ItemWeaponData_KachikachiDish[48] = {
    0x00000000, 0x00000000, 0x000000CB, 0x00000000, 0x64000000, 0x3F800000,
    0x01010101, (u32)weaponGetPowerDefault, 0x00000004, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01101260, 0x20000000, 0x00000302, 0x00000000, 0x00030040,
    0x000007FF, 0x80000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)ItemEvent_ThrowAttack, 0x00000064, 0x00050505, 0x05000000,
};

/* Recovered BattleWeapon record. */
u32 ItemWeaponData_BomberEgg[48] = {
    0x00000000, 0x00000000, 0x000000D1, 0x00000000, 0x64000000, 0x3F800000,
    0x01010101, (u32)weaponGetPowerDefault, 0x00000006, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01101260, 0x20000000, 0x03000302, 0x00000000, 0x00030040,
    0x000007FF, 0x80000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)ItemEvent_ThrowAttack, 0x00000064, 0x00050505, 0x05000000,
};

u32 ItemEvent_Poison_Kinoko[] = {
    0x00020032, 0xFE363C8C, (u32)ItemWeaponData_PoisonKinoko, 0x0001005E, (u32)ItemEvent_Support_Sub_UseDeclere, 0x00010009,
    0x0000001E, 0x0002005B, (u32)btlevtcmd_WeaponAftereffect, 0xFE363C8C, 0x0003005B, (u32)evt_sub_random,
    0x00000064, 0xFE363C80, 0x0002001A, 0xFE363C80, 0x00000050, 0x0001005E,
    (u32)ItemEvent_Support_Sub_Effect, 0x00000020, 0x0001005E, (u32)ItemEvent_GetTarget, 0x0004005B, (u32)evt_btl_camera_set_homing_unit,
    0x00000000, 0xFE363C8A, 0xFFFFFFFF, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000,
    0x00000001, 0x0003005B, (u32)evt_btl_camera_set_zoom, 0x00000000, 0x000000C8, 0x0006005B,
    (u32)btlevtcmd_GetPartsPos, 0xFE363C8A, 0xFE363C8B, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0003005B, (u32)btlevtcmd_GetStatusMg, 0xFE363C8A, 0xFE363C85, 0x0002003C, 0xFE363C85,
    0x00000032, 0x00020035, 0xFE363C81, 0xFE363C85, 0x000F005B, (u32)evt_eff,
    0x00000000, (u32)(char*)str_recovery_802f04b8, 0x00000004, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00010009, 0x0000003C, 0x0004005B, (u32)btlevtcmd_RecoverHp,
    0xFE363C8A, 0xFE363C8B, 0x000003E7, 0x00000021, 0x00000021, 0x0001005B,
    (u32)evt_audience_ap_recovery, 0x0001005E, (u32)_return_home_event, 0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE,
    0x00000002, 0x00000001,
};

/* Recovered BattleWeapon record. */
u32 ItemWeaponData_PoisonKinoko[48] = {
    0x00000000, 0x00000000, 0x000000D5, 0x00000000, 0x64000000, 0x3F800000,
    0x01010101, (u32)weaponGetPowerHPHalf2, 0x00000007, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01100160, 0x10000000, 0x00000302, 0x00000000, 0x00030048,
    0x000007FF, 0x80000000, 0x00000000, 0x00006403, 0x01000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)ItemEvent_Poison_Kinoko, 0x00000064, 0x00050505, 0x05000000,
};

u32 ItemEvent_LastDinner[] = {
    0x00020032, 0xFE363C8C, (u32)ItemWeaponData_PoisonKinoko, 0x0001005E, (u32)ItemEvent_Support_Sub_UseDeclere, 0x00010009,
    0x0000001E, 0x0002005B, (u32)btlevtcmd_WeaponAftereffect, 0xFE363C8C, 0x0001005E, (u32)ItemEvent_GetTarget,
    0x0005005B, (u32)btlevtcmd_GetPos, 0xFE363C8A, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0003005B, (u32)btlevtcmd_GetHeight, 0xFE363C8A, 0xFE363C85, 0x0003005B, (u32)btlevtcmd_GetStatusMg,
    0xFE363C8A, 0xFE363C86, 0x0002003C, 0xFE363C85, 0xFE363C86, 0x00020038,
    0xFE363C85, 0x00000002, 0x00020035, 0xFE363C81, 0xFE363C85, 0x00020035,
    0xFE363C82, 0x0000000A, 0x000F005B, (u32)evt_eff, 0x00000000, (u32)(char*)str_charge_802f06c4,
    0x00000001, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0xF24A7E80, 0x00000078,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_ITEM_COOK_LAST1_802f06cc, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00000000, 0x000F005B, (u32)evt_eff, 0x00000000, (u32)(char*)str_mist_804227f0, 0x00000006,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000, 0x000000B4, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0006005B,
    (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_ITEM_COOK_LAST2_802f06e0, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0xFE363C87,
    0x0001000A, 0x000007D0, 0x0002005B, (u32)evt_snd_sfxoff, 0xFE363C87, 0x0003005B,
    (u32)btlevtcmd_SetHp, 0xFE363C8A, 0x00000001, 0x0003005B, (u32)btlevtcmd_SetFp, 0xFE363C8A,
    0x00000000, 0x0002005B, (u32)_full_ap_recover, 0xFE363C8A, 0x0002005B, (u32)btlevtcmd_StatusWindowOnOff,
    0x00000002, 0x000F005B, (u32)evt_eff, 0x00000000, (u32)(char*)str_indirect_802f06f4, 0x00000000,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0000003C, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x000F005B,
    (u32)evt_eff, 0x00000000, (u32)(char*)str_hit_804227c8, 0x00000000, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0xF24A7E80, 0x00000003, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_ITEM_COOK_LAST3_802f0700,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000, 0x0001000A, 0x000000FA,
    0x000F005B, (u32)evt_eff, 0x00000000, (u32)(char*)str_hit_804227c8, 0x00000001, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0xF24A7E80, 0x00000003, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0006005B, (u32)evt_snd_sfxon_3d,
    (u32)(char*)str_SFX_ITEM_COOK_LAST3_802f0700, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000, 0x0001000A,
    0x000000FA, 0x000F005B, (u32)evt_eff, 0x00000000, (u32)(char*)str_hit_804227c8, 0x00000002,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0xF24A7E80, 0x00000003, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0006005B,
    (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_ITEM_COOK_LAST3_802f0700, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000,
    0x0001000A, 0x000000FA, 0x000F005B, (u32)evt_eff, 0x00000000, (u32)(char*)str_hit_804227c8,
    0x00000003, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0xF24A7E80, 0x00000003,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_ITEM_COOK_LAST3_802f0700, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00000000, 0x0001000A, 0x000006D6, 0x0001005B, (u32)evt_audience_ap_recovery, 0x0001005E,
    (u32)_return_home_event, 0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002, 0x00000001,
};

/* Recovered BattleWeapon record. */
u32 ItemWeaponData_LastDinner[48] = {
    0x00000000, 0x00000000, 0x000000DA, 0x00000000, 0x64000000, 0x3F800000,
    0x01010101, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01100160, 0x10000000, 0x00000302, 0x00000000, 0x00030108,
    0x000007FF, 0x80000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)ItemEvent_LastDinner, 0x00000064, 0x00000000, 0x00000000,
};
