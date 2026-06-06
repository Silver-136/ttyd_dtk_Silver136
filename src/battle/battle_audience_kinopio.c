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


u8 BattleAudienceCtrlProcessKinopio(int param_1) {
    return 0;
}


u8 BattleAudienceCtrlProcessKinopioTransEvt(int param_1) {
    return 0;
}


u8 BattleAudienceCtrlProcessKinopioEnter(void) {
    return 0;
}


u8 BattleAudienceCtrlProcessKinopioSing(void) {
    return 0;
}


u8 BattleAudienceCtrlProcessKinopioSleep(int param_1, char param_2) {
    return 0;
}


u8 BattleAudienceCtrlProcessKinopioFire(int param_1, char param_2) {
    return 0;
}


u8 BattleAudienceCtrlProcessKinopioEscape(int param_1) {
    return 0;
}


u8 BattleAudienceCtrlProcessKinopioShell(int param_1) {
    return 0;
}


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
        }
        else if (audience[0x19] != 0x11) {
            BattleAudience_Delete(param_1);
            *(f32*)((s32)base + 0x1377C) -= float_1_804244f8;
        }
    }
}


u8 BattleAudienceCtrlProcessKinopioSingSub(int param_1) {
    return 0;
}
