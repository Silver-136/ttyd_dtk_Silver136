#include "battle/battle_audience_kinopio.h"

extern void* _battleWorkPointer;
extern void* BattleAudienceBaseGetPtr(void);
extern void* BattleAudienceGetPtr(s32 audienceId);
extern u8 BattleAudience_GetSysCtrl(s32 audienceId);
extern void BattleAudience_ChangeStatus(s32 audienceId, s32 status);
extern void BattleAudience_Delete(s32 audienceId);
extern void BattleAudience_SetAnim(s32 audienceId, s32 animId, s32 flags);
extern void BattleAudienceSoundBombIgnite(s32 audienceId, s32 param_2, s32 param_3);
extern s32 BattleAudience_GetAnimEnd(s32 audienceId);
extern u8 BattleAudience_GetEscapeChangeOK(s32 audienceId);
extern void* BattleAudienceItemGetPtr(s32 itemId);
extern void BattleAudienceSoundItemThrow(s32 itemId, s32 audienceId);
extern void BattleAudienceAddTargetNum(f32 value, f32 unused);
extern f32 intplGetValue(s32 type, s32 time, f32 start, f32 end, s32 maxTime);
extern s32 irand(s32 max);
extern s32 rand(void);
extern void psndSFXOn_3D(const char* name, void* pos);
extern const char str_SFX_AUDIENCE_PANIC_S_802f9698[];
extern f32 float_270_804244e0;
extern f32 float_90_804244e4;
extern f32 float_0_804244e8;
extern f32 float_1_804244f8;
extern f32 float_neg1_804244fc;
extern f32 float_0p1_80424504;
extern f32 float_5_80424510;
extern f32 float_10_80424524;
extern f32 float_20_8042453c;
extern f32 float_32767_80424554;
extern f32 float_8_80424564;
extern f32 float_neg0p15_80424570;
extern f32 float_4_80424574;
extern f32 float_neg0p2_80424578;
extern f32 float_3p4_8042457c;
extern f32 float_1p5_80424580;
extern f32 float_12_80424584;


u8 BattleAudienceCtrlProcessKinopioWait(void) {
    return 0;
}


u8 BattleAudienceCtrlProcessKinopioIntrude(void) {
    return 0;
}


u8 BattleAudienceCtrlProcessKinopioEat(int param_1) {
    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
u8 BattleAudienceCtrlProcessKinopio(int audienceId) {
    extern void* BattleAudienceGetPtr(s32 audienceId);
    extern s32 BattleAudience_GetFront(s32 audienceId);
    extern void BattleAudienceCtrlProcessKinopioWait(s32 audienceId);
    extern void BattleAudienceCtrlProcessKinopioWaitHurimuki(s32 audienceId);
    extern void BattleAudienceCtrlProcessKinopioWaitKubiKatamuke(s32 audienceId);
    extern void BattleAudienceCtrlProcessKinopioWaitJump(s32 audienceId, s32 front);
    extern void BattleAudienceCtrlProcessKinopioJoyRaiseArm(s32 audienceId, s32 front);
    extern void BattleAudienceCtrlProcessKinopioJoyJump(s32 audienceId, s32 front);
    extern void BattleAudienceCtrlProcessKinopioJoyJump2(s32 audienceId, s32 front);
    extern void BattleAudienceCtrlProcessKinopioItemOn(s32 audienceId, s32 front);
    extern void BattleAudienceCtrlProcessKinopioItemThrow(s32 audienceId, s32 front);
    extern void BattleAudienceCtrlProcessKinopioBooing(s32 audienceId);
    extern void BattleAudienceCtrlProcessKinopioDamage(s32 audienceId);
    extern void BattleAudienceCtrlProcessKinopioEnter(s32 audienceId);
    extern void BattleAudienceCtrlProcessKinopioEscape(s32 audienceId);
    extern void BattleAudienceCtrlProcessKinopioSleep(s32 audienceId, s32 front);
    extern void BattleAudienceCtrlProcessKinopioTransEvt(s32 audienceId);
    extern void BattleAudienceCtrlProcessKinopioIntrude(s32 audienceId);
    extern void BattleAudienceCtrlProcessKinopioSing(s32 audienceId);
    extern void BattleAudienceCtrlProcessKinopioShell(s32 audienceId);
    extern void BattleAudienceCtrlProcessKinopioIgnite(s32 audienceId);
    extern void BattleAudienceCtrlProcessKinopioFire(s32 audienceId, s32 front);
    extern void BattleAudienceCtrlProcessKinopioEat(s32 audienceId);
    extern f64 sin(f64);

    u8* audience;
    s32 front;

    audience = BattleAudienceGetPtr(audienceId);
    front = BattleAudience_GetFront(audienceId);
    switch (audience[0x19]) {
        case 0:
            BattleAudienceCtrlProcessKinopioWait(audienceId);
            break;
        case 1:
            BattleAudienceCtrlProcessKinopioWaitHurimuki(audienceId);
            break;
        case 2:
            BattleAudienceCtrlProcessKinopioWaitKubiKatamuke(audienceId);
            break;
        case 3:
            BattleAudienceCtrlProcessKinopioWaitJump(audienceId, front);
            break;
        case 4:
            BattleAudienceCtrlProcessKinopioJoyRaiseArm(audienceId, front);
            break;
        case 5:
            BattleAudienceCtrlProcessKinopioJoyJump(audienceId, front);
            break;
        case 6:
            BattleAudienceCtrlProcessKinopioJoyJump2(audienceId, front);
            break;
        case 7:
            BattleAudienceCtrlProcessKinopioItemOn(audienceId, front);
            break;
        case 8:
            BattleAudienceCtrlProcessKinopioItemThrow(audienceId, front);
            break;
        case 9:
            BattleAudienceCtrlProcessKinopioBooing(audienceId);
            break;
        case 10:
            BattleAudienceCtrlProcessKinopioDamage(audienceId);
            break;
        case 11:
            BattleAudienceCtrlProcessKinopioEnter(audienceId);
            break;
        case 12:
            BattleAudienceCtrlProcessKinopioEscape(audienceId);
            break;
        case 13:
            BattleAudienceCtrlProcessKinopioSleep(audienceId, front);
            break;
        case 14:
            BattleAudienceCtrlProcessKinopioTransEvt(audienceId);
            break;
        case 15:
            BattleAudienceCtrlProcessKinopioIntrude(audienceId);
            break;
        case 16:
            BattleAudienceCtrlProcessKinopioSing(audienceId);
            break;
        case 17:
            BattleAudienceCtrlProcessKinopioShell(audienceId);
            break;
        case 18:
            BattleAudienceCtrlProcessKinopioIgnite(audienceId);
            break;
        case 19:
            BattleAudienceCtrlProcessKinopioFire(audienceId, front);
            break;
        case 20:
            BattleAudienceCtrlProcessKinopioEat(audienceId);
            break;
    }

    *(f32*)(audience + 0x48) += *(f32*)(audience + 0x78);
    *(f32*)(audience + 0x78) += *(f32*)(audience + 0x84);
    *(f32*)(audience + 0x4C) += *(f32*)(audience + 0x7C);
    *(f32*)(audience + 0x7C) += *(f32*)(audience + 0x88);
    *(f32*)(audience + 0x50) += *(f32*)(audience + 0x80);
    *(f32*)(audience + 0x80) += *(f32*)(audience + 0x8C);
    if (*(f32*)(audience + 0x78) > 0.0f) {
        *(f32*)(audience + 0xE8) = 90.0f;
    }
    else if (*(f32*)(audience + 0x78) < 0.0f) {
        *(f32*)(audience + 0xE8) = 270.0f;
    }
    if (*(f32*)(audience + 0xD0) < 0.0f) {
        *(f32*)(audience + 0xD0) = 0.0f;
        *(f32*)(audience + 0xDC) = 0.0f;
        *(f32*)(audience + 0xE4) = 0.0f;
    }
    if (audience[0x1B] == 0xB) {
        *(f32*)(audience + 0xDC) = 0.0f;
        *(f32*)(audience + 0xD0) = 0.0f;
        *(f32*)(audience + 0xE4) = 0.0f;
    }
    else {
        *(f32*)(audience + 0xD0) += *(f32*)(audience + 0xDC);
        *(f32*)(audience + 0xDC) += *(f32*)(audience + 0xE4);
    }
    if (audience[0x1B] == 2) {
        *(f32*)(audience + 0x12C) += 0.05f;
        if (*(f32*)(audience + 0x12C) >= 6.2832f) {
            *(f32*)(audience + 0x12C) -= 6.2832f;
        }
        *(f32*)(audience + 0x130) = 5.0f * (f32)sin(*(f32*)(audience + 0x12C));
        *(f32*)(audience + 0xC0) = *(f32*)(audience + 0x48) + *(f32*)(audience + 0xCC);
        *(f32*)(audience + 0xC4) = *(f32*)(audience + 0x4C) + *(f32*)(audience + 0xD0) + *(f32*)(audience + 0x130);
        *(f32*)(audience + 0xC8) = *(f32*)(audience + 0x50) + *(f32*)(audience + 0xD4);
    }
    else {
        *(f32*)(audience + 0xC0) = *(f32*)(audience + 0x48) + *(f32*)(audience + 0xCC);
        *(f32*)(audience + 0xC4) = *(f32*)(audience + 0x4C) + *(f32*)(audience + 0xD0);
        *(f32*)(audience + 0xC8) = *(f32*)(audience + 0x50) + *(f32*)(audience + 0xD4);
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
u8 BattleAudienceCtrlProcessKinopioTransEvt(int audienceId) {
    extern void* _battleWorkPointer;
    extern void* BattleAudienceGetPtr(s32 audienceId);
    extern void BattleAudience_SetAnim(s32 audienceId, s32 animId, s32 flags);
    extern void BattleAudience_ChangeStatus(s32 audienceId, s32 status);
    extern void* BattleAudienceDetectTargetAll(void);
    extern s32 BtlUnit_GetBodyPartsId(void*);
    extern s32 BtlUnit_GetUnitId(void*);
    extern void* BattleGetUnitPartsPtr(s32, s32);
    extern void BtlUnit_GetHitPos(void*, void*, f32*, f32*, f32*);
    extern void BtlUnit_GetStatus(void*, s32, s8*, s8*);
    extern s32 BtlUnit_CheckShadowGuard(void*);
    extern void* BattleGetSystemPtr(void*);
    extern void BattleCheckDamage(void*, void*, void*, void*, s32);
    extern void BtlUnit_snd_se(void*, const char*, s32, s32);
    extern void psndSFXOn_3D(const char* name, void* pos);
    extern f32 intplGetValue(s32 type, s32 time, f32 start, f32 end, s32 maxTime);
    extern char str_SFX_AUDIENCE_TELESA__802f9648[];
    extern char str_SFX_AUDIENCE_TELESA__802f9664[];
    extern void* weapon_teresa_trans;

    void* battleWork;
    u8* audience;
    void* target;
    void* part;
    s32 partId;
    f32 x;
    f32 y;
    f32 z;
    s8 status;
    s8 turns;

    battleWork = _battleWorkPointer;
    audience = BattleAudienceGetPtr(audienceId);
    switch (*(s32*)(audience + 4)) {
        case 0:
            BattleAudience_SetAnim(audienceId, 1, 0);
            *(void**)(audience + 0xEC) = BattleAudienceDetectTargetAll();
            *(s32*)(audience + 8) = 0;
            psndSFXOn_3D(str_SFX_AUDIENCE_TELESA__802f9648, audience + 0x48);
            *(s32*)(audience + 4) = 5;
            /* fallthrough */
        case 5:
            target = *(void**)(audience + 0xEC);
            partId = BtlUnit_GetBodyPartsId(target);
            part = BattleGetUnitPartsPtr(BtlUnit_GetUnitId(target), partId);
            (*(s32*)(audience + 8))++;
            *(f32*)(audience + 0x48) = intplGetValue(5, *(s32*)(audience + 8), *(f32*)(audience + 0xB4), 0.0f, 60);
            *(f32*)(audience + 0x4C) = intplGetValue(5, *(s32*)(audience + 8), *(f32*)(audience + 0xB8), 80.0f, 60);
            *(f32*)(audience + 0x50) = intplGetValue(5, *(s32*)(audience + 8), *(f32*)(audience + 0xBC), 100.0f, 60);
            if (*(s32*)(audience + 8) >= 60) {
                BattleAudience_SetAnim(audienceId, 7, 0);
                BtlUnit_GetHitPos(target, part, &x, &y, &z);
                if (*(f32*)(audience + 0x48) >= x) {
                    *(f32*)(audience + 0xE8) = 270.0f;
                }
                else {
                    *(f32*)(audience + 0xE8) = 90.0f;
                }
                *(s32*)(audience + 4) = 10;
                *(s32*)(audience + 8) = 0;
                target = *(void**)(audience + 0xEC);
                partId = BtlUnit_GetBodyPartsId(target);
                part = BattleGetUnitPartsPtr(BtlUnit_GetUnitId(target), partId);
                BtlUnit_GetHitPos(target, part, &x, &y, &z);
                psndSFXOn_3D(str_SFX_AUDIENCE_TELESA__802f9648, audience + 0x48);
            }
            break;
        case 10:
            (*(s32*)(audience + 8))++;
            *(f32*)(audience + 0x48) = intplGetValue(0, *(s32*)(audience + 8), 0.0f, x, 30);
            *(f32*)(audience + 0x4C) = intplGetValue(4, *(s32*)(audience + 8), 80.0f, y - 15.0f, 30);
            *(f32*)(audience + 0x50) = intplGetValue(0, *(s32*)(audience + 8), 100.0f, z, 30);
            if (*(s32*)(audience + 8) == 30) {
                target = *(void**)(audience + 0xEC);
                partId = BtlUnit_GetBodyPartsId(target);
                part = BattleGetUnitPartsPtr(BtlUnit_GetUnitId(target), partId);
                BtlUnit_GetStatus(target, 0x12, &status, &turns);
                if (status < 1 && !BtlUnit_CheckShadowGuard(target)) {
                    BattleCheckDamage(BattleGetSystemPtr(battleWork), target, part, &weapon_teresa_trans, 0x100);
                    BtlUnit_snd_se(target, str_SFX_AUDIENCE_TELESA__802f9664, 0xF1194D80, 0);
                }
            }
            if (*(s32*)(audience + 8) >= 120) {
                BattleAudience_ChangeStatus(audienceId, 0xB);
            }
            break;
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
u8 BattleAudienceCtrlProcessKinopioEnter(int audienceId) {
    extern void BattleAudienceBaseGetPtr(void);
    extern void* BattleAudienceGetPtr(s32 audienceId);
    extern void* camGetPtr(s32 cameraId);
    extern void BattleAudience_SetAnim(s32 audienceId, s32 animId, s32 flags);
    extern void BattleAudience_ChangeStatus(s32 audienceId, s32 status);
    extern s32 irand(s32 max);
    extern f64 sqrt(f64);

    u8* audience;
    f32 dx;
    f32 dz;
    f32 dist;

    BattleAudienceBaseGetPtr();
    audience = BattleAudienceGetPtr(audienceId);
    camGetPtr(8);
    BattleAudience_SetAnim(audienceId, 7, 0);

    if (audience[0x1B] == 0xB) {
        switch (*(s32*)(audience + 4)) {
            case 0:
                *(f32*)(audience + 0x48) = *(f32*)(audience + 0xB4);
                *(f32*)(audience + 0x4C) = -100.0f;
                *(f32*)(audience + 0x50) = *(f32*)(audience + 0xBC);
                *(f32*)(audience + 0x78) = 0.0f;
                *(f32*)(audience + 0x7C) = 0.0f;
                *(f32*)(audience + 0x80) = 0.0f;
                *(s32*)(audience + 8) += irand(60) + 1;
                (*(s32*)(audience + 4))++;
                break;
            case 1:
                (*(s32*)(audience + 8))--;
                if (*(s32*)(audience + 8) <= 0) {
                    *(f32*)(audience + 0x78) = 0.0f;
                    *(f32*)(audience + 0x7C) = 1.0f;
                    *(f32*)(audience + 0x80) = 0.0f;
                    (*(s32*)(audience + 4))++;
                }
                break;
            case 2:
                if (*(f32*)(audience + 0x4C) >= *(f32*)(audience + 0xB8)) {
                    *(f32*)(audience + 0x4C) = *(f32*)(audience + 0xB8);
                    BattleAudience_ChangeStatus(audienceId, 0);
                    (*(s32*)(audience + 4))++;
                }
                break;
        }
    }
    else {
        switch (*(s32*)(audience + 4)) {
            case 0:
                *(f32*)(audience + 0x48) = (f32)(irand(400) - 200);
                *(f32*)(audience + 0x4C) = *(f32*)(audience + 0xB8);
                *(f32*)(audience + 0x50) = 1000.0f;
                *(f32*)(audience + 0x78) = 0.0f;
                *(f32*)(audience + 0x7C) = 0.0f;
                *(f32*)(audience + 0x80) = 0.0f;
                *(s32*)(audience + 8) += irand(60) + 1;
                (*(s32*)(audience + 4))++;
                break;
            case 1:
                (*(s32*)(audience + 8))--;
                if (*(s32*)(audience + 8) <= 0) {
                    dz = *(f32*)(audience + 0xBC) - *(f32*)(audience + 0x50);
                    dx = *(f32*)(audience + 0xB4) - *(f32*)(audience + 0x48);
                    dist = (f32)sqrt((dx * dx) + (dz * dz));
                    *(f32*)(audience + 0x78) = 10.0f * (dx / dist);
                    *(f32*)(audience + 0x7C) = 0.0f;
                    *(f32*)(audience + 0x80) = 10.0f * (dz / dist);
                    *(f32*)(audience + 0xC) = 10.0f;
                    *(f32*)(audience + 0x10) = 10.0f;
                    *(f32*)(audience + 0x14) = dist;
                    (*(s32*)(audience + 4))++;
                }
                break;
            case 2:
                *(f32*)(audience + 0xC) += *(f32*)(audience + 0x10);
                if (*(f32*)(audience + 0xC) >= *(f32*)(audience + 0x14)) {
                    *(f32*)(audience + 0x48) = *(f32*)(audience + 0xB4);
                    *(f32*)(audience + 0x4C) = *(f32*)(audience + 0xB8);
                    *(f32*)(audience + 0x50) = *(f32*)(audience + 0xBC);
                    BattleAudience_ChangeStatus(audienceId, 0);
                }
                break;
        }
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
u8 BattleAudienceCtrlProcessKinopioSing(int audienceId) {
    extern void* BattleAudienceBaseGetPtr(void);
    extern void* BattleAudienceGetPtr(s32 audienceId);
    extern void BattleAudience_SetAnim(s32 audienceId, s32 animId, s32 flags);
    extern void BattleAudience_ChangeStatus(s32 audienceId, s32 status);
    extern void BattleAudienceSoundSing(s32 audienceId);
    extern void BattleAudienceCtrlProcessKinopioSingSub(s32 audienceId);
    extern s32 irand(s32 max);
    extern void* gp;
    extern void effOnpuN64Entry(s32, f32, f32, f32);

    u8* base;
    u8* audience;
    s32 i;
    f32 x;
    f32 y;

    base = BattleAudienceBaseGetPtr();
    audience = BattleAudienceGetPtr(audienceId);
    switch (*(s32*)(audience + 4)) {
        case 0:
            *(s32*)(audience + 8) = 30;
            (*(s32*)(audience + 4))++;
            /* fallthrough */
        case 1:
            (*(s32*)(audience + 8))--;
            *(f32*)(audience + 0xA0) += 6.0f;
            if (*(s32*)(audience + 8) == 15) {
                BattleAudience_SetAnim(audienceId, 0xE, 0);
                BattleAudienceSoundSing(audienceId);
            }
            if (*(s32*)(audience + 8) <= 0) {
                *(s32*)(audience + 8) = 120;
                (*(s32*)(audience + 4))++;
            }
            break;
        case 2:
            (*(s32*)(audience + 8))--;
            if (*(s32*)(*(s32*)gp + 0x1C) % 10 == 0) {
                y = *(f32*)(audience + 0x50) + (f32)irand(10) - 5.0f;
                x = *(f32*)(audience + 0x4C) + (f32)irand(10);
                effOnpuN64Entry(0, *(f32*)(audience + 0x48) + (f32)irand(200) - 100.0f, x, y);
            }
            if (*(s32*)(audience + 8) <= 0) {
                for (i = 0; i < *(s32*)(base + 0x13790); i++) {
                    if (audienceId != i) {
                        BattleAudienceCtrlProcessKinopioSingSub(i);
                    }
                }
                *(s32*)(audience + 8) = 30;
                (*(s32*)(audience + 4))++;
            }
            break;
        case 3:
            (*(s32*)(audience + 8))--;
            *(f32*)(audience + 0xA0) += 6.0f;
            if (*(s32*)(audience + 8) <= 15) {
                BattleAudience_SetAnim(audienceId, 1, 0);
            }
            if (*(s32*)(audience + 8) <= 0) {
                BattleAudience_ChangeStatus(audienceId, 0);
            }
            break;
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
u8 BattleAudienceCtrlProcessKinopioSleep(int audienceId, char altAnim) {
    extern void* BattleAudienceBaseGetPtr(void);
    extern void* BattleAudienceGetPtr(s32 audienceId);
    extern void BattleAudience_SetAnim(s32 audienceId, s32 animId, s32 flags);
    extern void BattleAudience_ChangeStatus(s32 audienceId, s32 status);
    extern void BattleAudienceSoundSleep(s32 audienceId);
    extern void BattleAudienceSoundZZZ(s32 audienceId);
    extern s32 irand(s32 max);
    extern s32 rand(void);
    extern void* effSleepEntry(s32, s32, f32, f32, f32, f32, f32);

    u8* audience;

    BattleAudienceBaseGetPtr();
    audience = BattleAudienceGetPtr(audienceId);
    if (*(s32*)(audience + 4) <= 1) {
        if ((u8)altAnim == 1) {
            BattleAudience_SetAnim(audienceId, 0xA, 0);
        }
        else {
            BattleAudience_SetAnim(audienceId, 0xB, 0);
        }
        if (*(s32*)(audience + 0x30) >= 320) {
            *(f32*)(audience + 0x64) = 1.0f;
            *(s32*)(audience + 0x30) = 0;
        }
        if (*(s32*)(audience + 0x30) <= 160) {
            *(f32*)(audience + 0x64) -= 0.0025f;
        }
        else {
            *(f32*)(audience + 0x64) += 0.0025f;
        }
        (*(s32*)(audience + 0x30))++;
        audience[0x43] += audience[0x18];
        if (audience[0x43] >= 200) {
            audience[0x18] = -4;
        }
        else if (audience[0x43] <= 5) {
            audience[0x18] = 4;
        }
    }

    switch (*(s32*)(audience + 4)) {
        case 0:
            BattleAudienceSoundSleep(audienceId);
            (*(s32*)(audience + 4))++;
            /* fallthrough */
        case 1:
            if (irand(0x8CA0) == 0) {
                effSleepEntry(0, 0xB4, *(f32*)(audience + 0x48),
                              *(f32*)(audience + 0x4C) + 25.0f,
                              *(f32*)(audience + 0x50), 1.0f, 45.0f);
                BattleAudienceSoundZZZ(audienceId);
            }
            break;
        case 2:
            *(f32*)(audience + 0xDC) = 6.0f + ((f32)rand() / 32767.0f);
            *(f32*)(audience + 0xE4) = -0.5f;
            if ((u8)altAnim == 1) {
                BattleAudience_SetAnim(audienceId, 0, 0);
            }
            else {
                BattleAudience_SetAnim(audienceId, 1, 0);
            }
            *(f32*)(audience + 0x64) = 1.0f;
            audience[0x43] = 0;
            (*(s32*)(audience + 4))++;
            /* fallthrough */
        case 3:
            if (*(f32*)(audience + 0xE4) == 0.0f) {
                BattleAudience_ChangeStatus(audienceId, 0);
            }
            break;
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
u8 BattleAudienceCtrlProcessKinopioFire(int audienceId, char altAnim) {
    extern void* BattleAudienceBaseGetPtr(void);
    extern void* BattleAudienceGetPtr(s32 audienceId);
    extern void BattleAudience_SetAnim(s32 audienceId, s32 animId, s32 flags);
    extern void BattleAudience_Delete(s32 audienceId);
    extern void BattleAudienceSoundBombFire(s32 audienceId);
    extern void* effSandarsEntry(s32 type, f32 x, f32 y, f32 z, f32 scale);
    extern void btl_camera_shake_h(s32, s32, s32, f32, f32);
    extern s32 BattleAudience_GetAudienceNoFromOffset(s32 audienceId, s32 x, s32 y);
    extern void BattleAudienceCtrlProcessKinopioFireSub(s32 audienceId);
    extern void BattleAudiencePuniAllEscape(void);
    extern s32 dat_802f954c[];

    void* base;
    u8* audience;
    s32 offsets[48];
    s32 i;

    base = BattleAudienceBaseGetPtr();
    audience = BattleAudienceGetPtr(audienceId);
    if ((u8)altAnim == 1) {
        BattleAudience_SetAnim(audienceId, 0x10, 0);
    }
    else {
        BattleAudience_SetAnim(audienceId, 0x11, 0);
    }

    switch (*(s32*)(audience + 4)) {
        case 0:
            *(s32*)(audience + 8) = 10;
            (*(s32*)(audience + 4))++;
            /* fallthrough */
        case 1:
            *(f32*)(audience + 0x60) += 0.05f;
            *(f32*)(audience + 0x64) += 0.05f;
            (*(s32*)(audience + 8))--;
            if (*(s32*)(audience + 8) <= 0) {
                for (i = 0; i < 48; i++) {
                    offsets[i] = dat_802f954c[i];
                }
                BattleAudienceSoundBombFire(audienceId);
                *(s32*)audience &= ~6;
                if (audienceId < 60) {
                    effSandarsEntry(4, *(f32*)(audience + 0x48),
                                    *(f32*)(audience + 0x4C) + 35.0f,
                                    *(f32*)(audience + 0x50), 0.0f);
                }
                btl_camera_shake_h(0, 30, 0, 5.0f, 0.0f);
                for (i = 0; i < 24; i++) {
                    s32 target = BattleAudience_GetAudienceNoFromOffset(audienceId, offsets[i * 2], offsets[i * 2 + 1]);
                    if (target != -1) {
                        BattleAudienceCtrlProcessKinopioFireSub(target);
                    }
                }
                BattleAudiencePuniAllEscape();
                *(s32*)(audience + 8) = 60;
                (*(s32*)(audience + 4))++;
            }
            break;
        case 2:
            (*(s32*)(audience + 8))--;
            if (*(s32*)(audience + 8) <= 0) {
                (*(s32*)(audience + 4))++;
            }
            break;
        case 3:
            BattleAudience_Delete(audienceId);
            *(f32*)((s32)base + 0x1377C) -= 1.0f;
            break;
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
u8 BattleAudienceCtrlProcessKinopioEscape(int audienceId) {
    extern void BattleAudienceBaseGetPtr(void);
    extern void* BattleAudienceGetPtr(s32 audienceId);
    extern void BattleAudience_SetAnim(s32 audienceId, s32 animId, s32 flags);
    extern void BattleAudience_Delete(s32 audienceId);
    extern s32 rand(void);
    extern void psndSFXOn_3D(const char* name, void* pos);
    extern const char str_SFX_AUDIENCE_RUN3_802f9684[];

    u8* audience;

    BattleAudienceBaseGetPtr();
    audience = BattleAudienceGetPtr(audienceId);
    BattleAudience_SetAnim(audienceId, 7, 0);
    if (audience[0x1B] == 0xB) {
        switch (*(s32*)(audience + 4)) {
            case 0:
                (*(s32*)(audience + 8))--;
                if (*(s32*)(audience + 8) <= 0) {
                    *(f32*)(audience + 0x78) = 0.0f;
                    *(f32*)(audience + 0x7C) = -1.0f;
                    *(f32*)(audience + 0x80) = 0.0f;
                    (*(s32*)(audience + 4))++;
                }
                break;
            case 1:
                if (*(f32*)(audience + 0x4C) <= -100.0f) {
                    BattleAudience_Delete(audienceId);
                    (*(s32*)(audience + 4))++;
                }
                break;
        }
    }
    else {
        switch (*(s32*)(audience + 4)) {
            case 0:
                (*(s32*)(audience + 8))--;
                if (*(s32*)(audience + 8) <= 0) {
                    *(f32*)(audience + 0x78) = ((2.0f * (f32)rand()) / 32767.0f) - 1.0f - (*(f32*)(audience + 0x48) / 120.0f);
                    *(f32*)(audience + 0x7C) = 0.0f;
                    *(f32*)(audience + 0x80) = 8.0f;
                    *(f32*)(audience + 0xDC) = 6.0f;
                    *(f32*)(audience + 0xE4) = -0.5f;
                    (*(s32*)(audience + 4))++;
                    psndSFXOn_3D(str_SFX_AUDIENCE_RUN3_802f9684, audience + 0x48);
                }
                break;
            case 1:
                if (*(f32*)(audience + 0xE4) == 0.0f) {
                    (*(s32*)(audience + 4))++;
                }
                break;
            case 2:
                if (*(f32*)(audience + 0x50) >= 600.0f) {
                    BattleAudience_Delete(audienceId);
                }
                break;
        }
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
u8 BattleAudienceCtrlProcessKinopioShell(int audienceId) {
    extern void* BattleAudienceGetPtr(s32 audienceId);
    extern void BattleAudience_SetAnim(s32 audienceId, s32 animId, s32 flags);
    extern void BattleAudience_ChangeStatus(s32 audienceId, s32 status);
    extern s32 irand(s32 max);
    extern void BattleAudienceSoundShell(s32 audienceId);

    u8* audience;

    audience = BattleAudienceGetPtr(audienceId);
    if (*(s32*)(audience + 4) >= 2 && *(s32*)(audience + 4) <= 5) {
        BattleAudience_SetAnim(audienceId, 0xF, 0);
    }
    else {
        BattleAudience_SetAnim(audienceId, 1, 0);
    }

    switch (*(s32*)(audience + 4)) {
        case 0:
            *(s32*)(audience + 8) = irand(30);
            (*(s32*)(audience + 4))++;
            /* fallthrough */
        case 1:
            (*(s32*)(audience + 8))--;
            if (*(s32*)(audience + 8) <= 0) {
                (*(s32*)(audience + 4))++;
            }
            break;
        case 2:
            *(f32*)(audience + 0xDC) = 5.0f;
            *(f32*)(audience + 0xE4) = -0.3f;
            BattleAudienceSoundShell(audienceId);
            (*(s32*)(audience + 4))++;
            break;
        case 3:
            *(s32*)(audience + 8) = irand(30);
            (*(s32*)(audience + 4))++;
            /* fallthrough */
        case 4:
            (*(s32*)(audience + 8))--;
            if (*(s32*)(audience + 8) <= 0) {
                (*(s32*)(audience + 4))++;
            }
            break;
        case 5:
            *(f32*)(audience + 0xDC) = 5.0f;
            *(f32*)(audience + 0xE4) = -0.3f;
            *(s32*)(audience + 8) = 10;
            BattleAudienceSoundShell(audienceId);
            (*(s32*)(audience + 4))++;
            break;
        case 6:
            (*(s32*)(audience + 8))--;
            if (*(s32*)(audience + 8) <= 0) {
                BattleAudience_ChangeStatus(audienceId, 0);
            }
            *(s32*)(audience + 0x12C) = 0;
            break;
        case 7:
            break;
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

void BattleAudienceCtrlProcessKinopioItemOn(s32 param_1, u8 param_2) {
    void* battleWork;
    void* base;
    u8* audience;
    u8* item;
    s32 flags;

    battleWork = _battleWorkPointer;
    base = BattleAudienceBaseGetPtr();
    audience = BattleAudienceGetPtr(param_1);
    if (param_2 == 1) {
        BattleAudience_SetAnim(param_1, 0, 0);
    }
    else {
        BattleAudience_SetAnim(param_1, 5, 0);
    }

    flags = *(s32*)audience;
    if (flags & 0x40) {
        item = BattleAudienceItemGetPtr(*(s16*)(audience + 0x1C));
        *(s32*)audience = flags & ~0x60;
        *(s16*)(audience + 0x1C) = -1;
        *(s32*)item = 0;
        BattleAudience_ChangeStatus(param_1, 0xA);
        item = BattleAudienceItemGetPtr(0);
        if (*(s32*)(item + 0x10) < 0xEC || *(s32*)(item + 0x10) >= 0xF0) {
            base = BattleAudienceBaseGetPtr();
            BattleAudienceAddTargetNum(*(f32*)((s32)base + 0x13778) * float_neg0p15_80424570, float_0_804244e8);
        }
    }
    else if (!(flags & 0x20) && (*(u32*)((s32)battleWork + 0xEF4) & 0x80)) {
        item = BattleAudienceItemGetPtr(*(s16*)(audience + 0x1C));
        if (!(*(u32*)base & 0x40000)) {
            (*(s32*)(item + 8))--;
            if ((*(s32*)item & 1) && *(s32*)(item + 8) <= 0 && *(s32*)(item + 4) == 1) {
                BattleAudience_ChangeStatus(param_1, 8);
                BattleAudienceSoundItemThrow(0, param_1);
            }
        }
    }
}


void BattleAudienceCtrlProcessKinopioDamage(s32 param_1) {
    u8* audience;

    BattleAudienceBaseGetPtr();
    audience = BattleAudienceGetPtr(param_1);
    switch (*(s32*)(audience + 4)) {
        case 0:
            BattleAudience_SetAnim(param_1, 9, 0);
            *(f32*)(audience + 0xDC) = 8.0f;
            *(f32*)(audience + 0xE4) = -1.0f;
            *(s32*)(audience + 0x30) = 0;
            *(f32*)(audience + 0xE8) = 270.0f;
            (*(s32*)(audience + 4))++;
            /* fallthrough */
        case 1:
            if (*(s32*)(audience + 0x30) >= 8) {
                *(f32*)(audience + 0x64) = 1.0f;
                *(s32*)(audience + 0x30) = 0;
            }
            if (*(s32*)(audience + 0x30) <= 4) {
                *(f32*)(audience + 0x64) -= 0.1f;
            }
            else {
                *(f32*)(audience + 0x64) += 0.1f;
            }
            (*(s32*)(audience + 0x30))++;
            if (BattleAudience_GetAnimEnd(param_1) == 1) {
                *(f32*)(audience + 0x64) = 1.0f;
                *(s32*)(audience + 0x30) = 0;
                if (BattleAudience_GetEscapeChangeOK(param_1) == 1) {
                    BattleAudience_ChangeStatus(param_1, 0xC);
                    psndSFXOn_3D(str_SFX_AUDIENCE_PANIC_S_802f9698, audience + 0x48);
                }
                else {
                    BattleAudience_ChangeStatus(param_1, 0);
                }
            }
            break;
    }
}


void BattleAudienceCtrlProcessKinopioJoyJump2(s32 param_1, u8 param_2) {
    u8* audience;

    audience = BattleAudienceGetPtr(param_1);
    if (param_2 == 1) {
        BattleAudience_SetAnim(param_1, 2, 0);
    }
    else {
        BattleAudience_SetAnim(param_1, 3, 0);
    }

    switch (*(s32*)(audience + 4)) {
        case 0:
            *(s32*)(audience + 8) = 2;
            (*(s32*)(audience + 4))++;
            /* fallthrough */
        case 1:
            *(f32*)(audience + 0xDC) = 4.0f + ((f32)rand() / 32767.0f);
            *(f32*)(audience + 0xE4) = -0.2f;
            (*(s32*)(audience + 4))++;
            /* fallthrough */
        case 2:
            *(f32*)(audience + 0xA0) += 20.0f;
            if (*(f32*)(audience + 0xE4) == 0.0f) {
                (*(s32*)(audience + 8))--;
                if (*(s32*)(audience + 8) <= 0) {
                    BattleAudience_ChangeStatus(param_1, 0);
                }
                else {
                    *(s32*)(audience + 4) = 1;
                }
            }
            break;
    }
}


void BattleAudienceCtrlProcessKinopioWaitJump(s32 param_1, u8 param_2) {
    u8* audience;

    audience = BattleAudienceGetPtr(param_1);
    if (param_2 == 1) {
        BattleAudience_SetAnim(param_1, 2, 0);
    }
    else {
        BattleAudience_SetAnim(param_1, 3, 0);
    }

    switch (*(s32*)(audience + 4)) {
        case 0:
            if (audience[0x1B] == 7) {
                *(s32*)(audience + 8) = 3;
            }
            else {
                *(s32*)(audience + 8) = 1;
            }
            (*(s32*)(audience + 4))++;
            /* fallthrough */
        case 1:
            if (audience[0x1B] == 7) {
                *(f32*)(audience + 0xDC) = 5.0f;
                *(f32*)(audience + 0xE4) = -1.0f;
            }
            else {
                *(f32*)(audience + 0xDC) = 3.4f;
                *(f32*)(audience + 0xE4) = -0.2f;
            }
            (*(s32*)(audience + 4))++;
            /* fallthrough */
        case 2:
            if (*(f32*)(audience + 0xE4) == 0.0f) {
                (*(s32*)(audience + 8))--;
                if (*(s32*)(audience + 8) <= 0) {
                    BattleAudience_ChangeStatus(param_1, 0);
                }
                else {
                    *(s32*)(audience + 4) = 1;
                }
            }
            break;
    }
}


void BattleAudienceCtrlProcessKinopioJoyJump(s32 param_1, u8 param_2) {
    u8* audience;

    audience = BattleAudienceGetPtr(param_1);
    if (param_2 == 1) {
        BattleAudience_SetAnim(param_1, 2, 0);
    }
    else {
        BattleAudience_SetAnim(param_1, 3, 0);
    }

    switch (*(s32*)(audience + 4)) {
        case 0:
            *(s32*)(audience + 8) = 1;
            (*(s32*)(audience + 4))++;
            /* fallthrough */
        case 1:
            *(f32*)(audience + 0xDC) = 3.4f + ((f32)rand() / 32767.0f);
            *(f32*)(audience + 0xE4) = -0.2f;
            (*(s32*)(audience + 4))++;
            /* fallthrough */
        case 2:
            if (*(f32*)(audience + 0xE4) == 0.0f) {
                (*(s32*)(audience + 8))--;
                if (*(s32*)(audience + 8) <= 0) {
                    BattleAudience_ChangeStatus(param_1, 0);
                }
                else {
                    *(s32*)(audience + 4) = 1;
                }
            }
            break;
    }
}


void BattleAudienceCtrlProcessKinopioWaitKubiKatamuke(s32 param_1, u8 param_2) {
    u8* audience;
    s32 timer;

    audience = BattleAudienceGetPtr(param_1);
    if (param_2 == 1) {
        BattleAudience_SetAnim(param_1, 0, 0);
    }
    else {
        BattleAudience_SetAnim(param_1, 1, 0);
    }

    timer = *(s32*)(audience + 0x30);
    if (timer >= 0 && timer <= 10) {
        *(f32*)(audience + 0x6C) = float_1p5_80424580 * (f32)timer;
    }
    timer = *(s32*)(audience + 0x30);
    if (timer >= 20 && timer <= 40) {
        *(f32*)(audience + 0x6C) -= float_1p5_80424580;
    }
    timer = *(s32*)(audience + 0x30);
    if (timer >= 50 && timer <= 60) {
        *(f32*)(audience + 0x6C) += float_1p5_80424580;
    }
    timer = *(s32*)(audience + 0x30);
    if (timer > 60) {
        BattleAudience_ChangeStatus(param_1, 0);
    }
    else {
        *(s32*)(audience + 0x30) = timer + 1;
    }
}


void BattleAudienceCtrlProcessKinopioJoyRaiseArm(s32 param_1, u8 param_2) {
    u8* audience;

    audience = BattleAudienceGetPtr(param_1);
    if (param_2 == 0) {
        BattleAudience_SetAnim(param_1, 4, 0);
    }
    switch (*(s32*)(audience + 4)) {
        case 0:
            if (irand(2) == 0) {
                *(f32*)(audience + 0xE8) = float_270_804244e0;
            }
            else {
                *(f32*)(audience + 0xE8) = float_90_804244e4;
            }
            (*(s32*)(audience + 4))++;
            /* fallthrough */
        case 1:
            *(f32*)(audience + 0x70) = intplGetValue(4, *(s32*)(audience + 8), float_0_804244e8, float_10_80424524, 15);
            (*(s32*)(audience + 8))++;
            if (*(s32*)(audience + 8) > 30) {
                *(f32*)(audience + 0xC) -= float_1_804244f8;
                if (*(f32*)(audience + 0xC) <= float_0_804244e8) {
                    BattleAudience_ChangeStatus(param_1, 0);
                }
                else {
                    *(s32*)(audience + 8) = 0;
                }
            }
            break;
    }
}


void BattleAudienceCtrlProcessKinopioIgnite(s32 param_1, u8 param_2) {
    u8* audience;

    audience = BattleAudienceGetPtr(param_1);
    if (param_2 == 1) {
        BattleAudience_SetAnim(param_1, 0x10, 0);
    }
    else {
        BattleAudience_SetAnim(param_1, 0x11, 0);
    }

    switch (*(s32*)(audience + 4)) {
        case 0:
            *(s32*)(audience + 8) = 30;
            BattleAudienceSoundBombIgnite(param_1, 0xB4, 0x78);
            (*(s32*)(audience + 4))++;
            /* fallthrough */
        case 1:
            (*(s32*)(audience + 8))--;
            if (*(s32*)(audience + 8) <= 0) {
                *(s32*)(audience + 8) = 10;
                (*(s32*)(audience + 4))++;
            }
            break;
        case 2:
            (*(s32*)(audience + 8))--;
            if (*(s32*)(audience + 8) <= 0) {
                (*(s32*)(audience + 4))++;
            }
            break;
    }
}


void BattleAudienceCtrlProcessKinopioBooing(s32 param_1) {
    u8* audience;

    audience = BattleAudienceGetPtr(param_1);
    BattleAudience_SetAnim(param_1, 4, 0);
    switch (*(s32*)(audience + 4)) {
        case 0:
            if (irand(2) == 0) {
                *(f32*)(audience + 0xE8) = float_270_804244e0;
            }
            else {
                *(f32*)(audience + 0xE8) = float_90_804244e4;
            }
            (*(s32*)(audience + 4))++;
            /* fallthrough */
        case 1:
            *(f32*)(audience + 0x70) = intplGetValue(4, *(s32*)(audience + 8), float_0_804244e8, float_10_80424524, 15);
            (*(s32*)(audience + 8))++;
            if (*(s32*)(audience + 8) > 30) {
                *(f32*)(audience + 0xC) -= float_1_804244f8;
                if (*(f32*)(audience + 0xC) <= float_0_804244e8) {
                    BattleAudience_ChangeStatus(param_1, 0);
                }
                else {
                    *(s32*)(audience + 8) = 0;
                }
            }
            break;
    }
}


void BattleAudienceCtrlProcessKinopioWaitHurimuki(s32 param_1, u8 param_2) {
    u8* audience;
    s32 timer;

    audience = BattleAudienceGetPtr(param_1);
    if (param_2 == 1) {
        BattleAudience_SetAnim(param_1, 0, 0);
    }
    else {
        BattleAudience_SetAnim(param_1, 1, 0);
    }

    timer = *(s32*)(audience + 0x30);
    if (timer <= 15) {
        *(f32*)(audience + 0xA0) = float_12_80424584 * (f32)timer;
    }
    timer = *(s32*)(audience + 0x30);
    if (timer >= 30) {
        *(f32*)(audience + 0xA0) = float_12_80424584 * (f32)(45 - timer);
    }
    timer = *(s32*)(audience + 0x30);
    if (timer > 45) {
        BattleAudience_ChangeStatus(param_1, 0);
    }
    else {
        *(s32*)(audience + 0x30) = timer + 1;
    }
}


void BattleAudienceCtrlProcessKinopioItemThrow(s32 param_1) {
    u8* audience;
    u8* item;
    void* base;
    s32 flags;

    audience = BattleAudienceGetPtr(param_1);
    BattleAudience_SetAnim(param_1, 6, 0);
    if (BattleAudience_GetAnimEnd(param_1) == 1) {
        BattleAudience_ChangeStatus(param_1, 0);
    }
    flags = *(s32*)audience;
    if (flags & 0x40) {
        *(s32*)audience = flags & ~0x60;
        BattleAudience_ChangeStatus(param_1, 0xA);
        item = BattleAudienceItemGetPtr(0);
        if (*(s32*)(item + 0x10) < 0xEC || *(s32*)((s32)BattleAudienceItemGetPtr(0) + 0x10) >= 0xF0) {
            base = BattleAudienceBaseGetPtr();
            BattleAudienceAddTargetNum(*(f32*)((s32)base + 0x13778) * float_neg0p15_80424570, float_0_804244e8);
        }
    }
    else if (flags & 0x20) {
    }
}


void BattleAudienceCtrlProcessKinopioFireSub(s32 param_1) {
    void* base;
    u8* audience;

    base = BattleAudienceBaseGetPtr();
    audience = BattleAudienceGetPtr(param_1);
    if (*(u32*)audience & 0x100) {
        BattleAudienceCtrlProcessKinopioFireSub(param_1 - 1);
    }
    else if (BattleAudience_GetSysCtrl(param_1) != 0) {
        if (audience[0x1B] == 9) {
            if (audience[0x19] != 0x13) {
                BattleAudience_ChangeStatus(param_1, 0x13);
            }
            else {
                return;
            }
        }
        else if (audience[0x19] != 0x11) {
            BattleAudience_Delete(param_1);
            *(f32*)((s32)base + 0x1377C) -= float_1_804244f8;
        }
    }
}

void BattleAudienceCtrlProcessKinopioSingSub(s32 param_1) {
    u8* audience;

    audience = BattleAudienceGetPtr(param_1);
    if (*(u32*)audience & 0x100) {
        BattleAudienceCtrlProcessKinopioSingSub(param_1 - 1);
    }
    else if (audience[0x19] != 0x11) {
        BattleAudience_ChangeStatus(param_1, 0xD);
        *(s32*)(audience + 0xF4) = 3;
    }
}

