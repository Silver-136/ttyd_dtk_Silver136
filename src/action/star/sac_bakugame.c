#include "action/star/sac_bakugame.h"
#include "battle/battle.h"
#include "event/evt_cmd.h"

void bakuGameDispStar(void);
void bakuGameDecideButton(s32 flag);
s32 bakuGameAudienceCanThrowPos(s32 pos);
s32 bakuGamePartyExist(void* unit);
void bakuGameDisp2D(void);
void bakuGameDisp3D(void);

extern char str_c_star_bom_802ff6c0[];
extern f32 float_0_80427b70;
extern f32 float_1_80427b60;
extern f32 float_neg1_80427b5c;
extern f32 float_100_80427b78;
extern f32 float_90_80427b7c;
const char str_Z_1_80427c20[] = "Z_1";

s32 BtlUnit_GetBodyPartsId(void* unit);
void* BtlUnit_GetPartsPtr(void* unit, s32 partsId);
void btlDispPoseAnime(void* part);
s32 animGroupBaseAsync(void* name, s32 mode, s32 flags);
s32 animPoseEntry(void* name, s32 mode);
void animPoseSetAnim(s32 poseId, const void* name, s32 force);
void BtlUnit_SetBodyAnimType(void* unit, s32 type);
void effDelete(void* effect);

typedef struct BakuGameWork {
    u8 pad[0xEC];
    s32 poseId;
    u8 padF0[0x204 - 0xF0];
    s32 starAppear;
} BakuGameWork;

BakuGameWork* GetBakuGamePtr(void) {
    return (BakuGameWork*)((s32)_battleWorkPointer + 0x1F4C);
}

USER_FUNC(star_stone_appear) {
    BakuGameWork* wp;

    wp = GetBakuGamePtr();
    wp->starAppear = 1;
    return 2;
}

s32 main_star(void) {
    extern void* BattleGetMarioPtr(void* battleWork);
    extern void* effStarStoneEntry(s32 type, f32 x, f32 y, f32 z, f32 scale);
    extern void BtlUnit_GetPos(void* unit, f32* x, f32* y, f32* z);
    extern f32 intplGetValue(s32 type, s32 current, s32 total, f32 start, f32 end);
    extern void dispEntry(s32 cameraId, s32 renderMode, void* callback, s32 param, f32 order);
    extern f32 float_neg1000_80427bd8;
    extern f32 float_37_80427c24;
    extern f32 float_50_80427bd4;
    extern f32 float_2_80427bb4;
    extern f32 float_2160_80427c28;
    extern f32 float_300_80427b9c;
    extern f32 float_1p5_80427ba0;
    extern u32 vec3_802ff3d0[3];
    extern u32 vec3_802ff3dc[3];

    u8* work;
    u8* mario;
    u8* starWork;
    s32 state;
    s32 timer;
    f32 value;
    f32 start;
    f32 end;
    u32* srcA;
    u32* srcB;

    work = (u8*)GetBakuGamePtr();
    mario = (u8*)BattleGetMarioPtr(_battleWorkPointer);
    state = *(s32*)(work + 0x204);

    switch (state) {
        case 0:
            break;
        case 1:
            value = float_0_80427b70;
            *(s32*)(work + 0x204) = 2;
            *(s32*)(work + 0x208) = 0;
            *(void**)(work + 0x254) = effStarStoneEntry(1, value, float_neg1000_80427bd8, value, float_1_80427b60);

            BtlUnit_GetPos(mario, (f32*)(work + 0x20C), (f32*)(work + 0x210), (f32*)(work + 0x214));
            *(f32*)(work + 0x210) += (*(f32*)(mario + 0x114) * (f32)*(s16*)(mario + 0xCE)) + float_37_80427c24;

            srcA = vec3_802ff3d0;
            srcB = vec3_802ff3dc;

            *(u32*)(work + 0x218) = *(u32*)(work + 0x20C);
            *(u32*)(work + 0x21C) = *(u32*)(work + 0x210);
            *(u32*)(work + 0x220) = *(u32*)(work + 0x214);

            *(u32*)(work + 0x224) = *(u32*)(work + 0x20C);
            *(u32*)(work + 0x228) = *(u32*)(work + 0x210);
            *(u32*)(work + 0x22C) = *(u32*)(work + 0x214);
            *(f32*)(work + 0x228) += float_50_80427bd4;
            *(f32*)(work + 0x22C) += float_neg1_80427b5c;

            *(u32*)(work + 0x23C) = srcA[0];
            *(u32*)(work + 0x240) = srcA[1];
            *(u32*)(work + 0x244) = srcA[2];
            *(u32*)(work + 0x248) = srcB[0];
            *(u32*)(work + 0x24C) = srcB[1];
            *(u32*)(work + 0x250) = srcB[2];
            /* fall through */
        case 2:
            timer = *(s32*)(work + 0x208) + 1;
            *(s32*)(work + 0x208) = timer;
            if (timer <= 100) {
                start = *(f32*)(work + 0x218);
                end = *(f32*)(work + 0x224);
                timer = *(s32*)(work + 0x208);
                *(f32*)(work + 0x20C) = intplGetValue(0, timer, 100, start, end);

                start = *(f32*)(work + 0x21C);
                end = *(f32*)(work + 0x228);
                timer = *(s32*)(work + 0x208);
                *(f32*)(work + 0x210) = intplGetValue(0, timer, 100, start, end);

                start = *(f32*)(work + 0x220);
                end = *(f32*)(work + 0x22C);
                timer = *(s32*)(work + 0x208);
                *(f32*)(work + 0x214) = intplGetValue(0, timer, 100, start, end);

                start = float_0_80427b70;
                end = float_2_80427bb4;
                timer = *(s32*)(work + 0x208);
                value = intplGetValue(0, timer, 100, start, end);
                *(f32*)(work + 0x244) = value;
                *(f32*)(work + 0x240) = value;
                *(f32*)(work + 0x23C) = value;
            } else {
                value = float_2_80427bb4;
                *(f32*)(work + 0x20C) = *(f32*)(work + 0x224);
                *(f32*)(work + 0x210) = *(f32*)(work + 0x228);
                *(f32*)(work + 0x214) = *(f32*)(work + 0x22C);
                *(f32*)(work + 0x244) = value;
                *(f32*)(work + 0x240) = value;
                *(f32*)(work + 0x23C) = value;
            }

            start = float_0_80427b70;
            end = float_2160_80427c28;
            timer = *(s32*)(work + 0x208);
            *(f32*)(work + 0x24C) = intplGetValue(4, timer, 0x78, start, end);
            if (*(s32*)(work + 0x208) >= 0x78) {
                *(s32*)(work + 0x204) = 3;
                *(s32*)(work + 0x208) = 0;
                value = float_300_80427b9c;
                *(u32*)(work + 0x218) = *(u32*)(work + 0x20C);
                *(u32*)(work + 0x21C) = *(u32*)(work + 0x210);
                *(u32*)(work + 0x220) = *(u32*)(work + 0x214);
                *(f32*)(work + 0x228) = value;
            }
            break;
        case 3:
            timer = *(s32*)(work + 0x208);
            timer++;
            *(s32*)(work + 0x208) = timer;
            start = *(f32*)(work + 0x21C);
            end = *(f32*)(work + 0x228);
            timer = *(s32*)(work + 0x208);
            *(f32*)(work + 0x210) = intplGetValue(1, timer, 0x3C, start, end);
            if (*(s32*)(work + 0x208) >= 0x3C) {
                *(s32*)(work + 0x204) = 4;
                *(s32*)(work + 0x208) = 0;
            }
            break;
        case 4:
        default:
            break;
    }

    if (*(void**)(work + 0x254) != 0) {
        starWork = *(u8**)((s32)*(void**)(work + 0x254) + 0xC);
        value = float_1p5_80427ba0;
        *(f32*)(starWork + 0x8) = *(f32*)(work + 0x20C);
        *(f32*)(starWork + 0xC) = *(f32*)(work + 0x210);
        *(f32*)(starWork + 0x10) = *(f32*)(work + 0x214);
        *(f32*)(starWork + 0x18) = *(f32*)(work + 0x248);
        *(f32*)(starWork + 0x1C) = *(f32*)(work + 0x24C);
        *(f32*)(starWork + 0x20) = *(f32*)(work + 0x250);
        *(f32*)(starWork + 0x14) = value * *(f32*)(work + 0x23C);
    }

    dispEntry(4, 2, bakuGameDispStar, 0, float_0_80427b70);
    return 0;
}

USER_FUNC(init_bakugame) {
    memset(GetBakuGamePtr(), 0, 0x2B8);
    return 2;
}

USER_FUNC(end_bakugame) {
    BakuGameWork* work = GetBakuGamePtr();

    effDelete(*(void**)(*(s32*)(*(s32*)((s32)work + 0x254) + 0xC) + 0x34));
    effDelete(*(void**)((s32)work + 0x254));
    return 2;
}

USER_FUNC(bakuGameDecideWeapon) {
    extern void* GetBakuGamePtr(void);
    extern void* memcpy(void* dst, const void* src, u32 size);
    extern u8 weapon[0xC0];

    s32* args;
    u8* weaponWork;
    s32 value;
    void* work;

    args = event->args;
    evtGetFloat(event, args[0]);
    value = evtGetValue(event, args[1]);
    work = GetBakuGamePtr();
    weaponWork = (u8*)((s32)work + 0x178);
    memcpy((void*)((s32)work + 0xF8), weapon, 0xC0);
    weaponWork[2] = 0x64;
    if (value != -1) {
        if (*(f32*)((s32)work + 0xD0) >= float_100_80427b78) {
            weaponWork[2] = 0x7F;
        } else {
            weaponWork[2] = 0x64;
        }
        weaponWork[3] = value - 1;
    } else {
        weaponWork[2] = 0;
        weaponWork[3] = 0;
    }
    evtSetValue(event, args[2], (s32)work + 0xF8);
    return 2;
}

USER_FUNC(bakuGameBombEntry) {
    BakuGameWork* work = GetBakuGamePtr();
    if (animGroupBaseAsync(str_c_star_bom_802ff6c0, 2, 0) == 0) {
        return 0;
    }
    work->poseId = animPoseEntry(str_c_star_bom_802ff6c0, 2);
    animPoseSetAnim(work->poseId, str_Z_1_80427c20, 1);
    return 2;
}

s32 bakuGameMain(void* event, s32 isFirstCall) {
    extern BattleWork* _battleWorkPointer;
    extern s32 BattleTransID(void*, s32);
    extern s32 BattleAudience_GetWaiting(s32);
    extern s32 irand(s32);
    extern u16 keyGetButtonTrg(s32 channel);
    extern void memcpy(void*, void*, u32);
    extern void animPoseRelease(s32);
    extern void animPoseMain(void);
    extern void bakuGameMarioSurpriseReset(void*, void*);
    extern void bakuGameEnemySurpriseReset(void);
    extern void bakuGameAudienceSurpriseReset(void);
    extern void dispEntry(s32, s32, void*, void*, f32);
    extern void evtSetValue(void*, s32, s32);
    extern void evtSetFloat(void*, s32, f32);
    extern u8 weapon[];
    extern const u32 dat_802ff408[3];
    extern const char str_btl_wn_sac_bakugame_802ff3a0[];
    extern f32 float_1p5_80427ba0;
    extern f32 float_neg0p3_80427ba4;
    extern f32 float_110_80427ba8;
    extern const char str_SFX_BTL_SAC_TIME1_802ff628[];
    extern s32 psndSFXChk(u32 id);
    extern void psndSFXOff(s32 id);
    extern s32 psndSFXOn(const char* name);
    extern f32 intplGetValue(f32 start, f32 end, s32 type, s32 current, s32 total);
    u32* work;
    const u32* rodata = (const u32*)str_btl_wn_sac_bakugame_802ff3a0;
    void* evt = event;
    s32* args;
    u32 buttonMasks[3];
    BattleWork* battle;
    BattleWorkUnit* mario;
    BattleWorkUnit* party;
    typedef struct BakuVec {
        f32 x;
        f32 y;
        f32 z;
    } BakuVec;
    BakuVec vec20;
    BakuVec vec2C;
    BakuVec vec38;
    BakuVec vec44;
    f32 x;
    f32 y;
    f32 z;
    f32 hx;
    f32 hy;
    f32 hz;
    s32 i;

    work = (u32*)GetBakuGamePtr();
    battle = _battleWorkPointer;
    mario = BattleGetUnitPtr(battle, BattleTransID(evt, -3));
    party = BattleGetUnitPtr(battle, BattleTransID(evt, -4));
    buttonMasks[0] = rodata[0x68 / 4];
    buttonMasks[1] = rodata[0x6C / 4];
    buttonMasks[2] = rodata[0x70 / 4];
    args = *(s32**)((u8*)evt + 0x18);
    x = float_0_80427b70;
    y = float_0_80427b70;
    z = float_0_80427b70;
    hx = float_0_80427b70;
    hy = float_0_80427b70;
    hz = float_0_80427b70;
    if (isFirstCall != 0) {
        s32 canThrow = 0;
        s32 ceilingSafe = 1;
        BattleWorkUnit* unit;
        work[1] = 0;
        bakuGameDecideButton(1);
        for (i = 0; i < 3; i++) {
            u8* slot = (u8*)work + 0xC + i * 0x24;
            *(s32*)slot = 0;
            *(u32*)(slot + 8) = buttonMasks[i];
            *(f32*)(slot + 0x10) = 100.0f;
            *(f32*)(slot + 0x20) = 0.0f;
        }
        memcpy(work + 0x3E, weapon, 0xC0);
        for (i = 0; i < 0x40; i++) {
            unit = BattleGetUnitPtr(battle, i);
            if (unit != 0 && unit->alliance == 1) {
                if ((f32)unit->width >= float_100_80427b78) {
                    ceilingSafe = 1;
                    break;
                }
                if ((unit->attributes & 2) == 0) {
                    ceilingSafe = 0;
                }
            }
        }
        if (ceilingSafe) {
            *work |= 1;
        } else {
            *work &= ~1U;
        }
        for (i = 0; i < 200; i++) {
            if (bakuGameAudienceCanThrowPos(i) == 1 && BattleAudience_GetWaiting(i)) {
                canThrow = 1;
            }
        }
        if (canThrow) {
            *work &= ~2U;
        } else {
            *work |= 2;
        }
    }

    switch (work[1]) {
        case 0:
            for (i = 0; i < 3; i++) {
                if (*(s32*)((u8*)work + 0xC + i * 0x24) != 5) {
                    break;
                }
            }
            if (i == 3 && work[0x1F] == 8 &&
                (((*work & 1) == 0) || work[0x70] == 5)) {
                work[1] = 5;
            }
            break;
        case 5:
            work[1] = 10;
            work[2] = 0;
            for (i = 0; i < 3; i++) {
                u8* slot = (u8*)work + 0xC + i * 0x24;
                *(s32*)slot = 10;
                *(s32*)(slot + 4) = 0;
            }
            work[0x1F] = 10;
            work[0x3C] = irand(0x78) + 0x21C;
        case 10:
            work[2]++;
            if ((s32)work[2] > 60 && irand(0x96) == 0) {
                for (i = 0; i < 3; i++) {
                    *(s32*)((u8*)work + 0xC + i * 0x24) = 10;
                }
                bakuGameDecideButton(0);
                work[2] = 0;
            }
            if (work[2] == 180) {
                work[2] = 180;
                for (i = 0; i < 3; i++) {
                    *(s32*)((u8*)work + 0xC + i * 0x24) = 10;
                }
                bakuGameDecideButton(0);
                work[2] = 0;
            }
            {
                s32 lane = -1;
                if (*(u8*)((u8*)work + 0xCC) == 1) {
                    lane = irand(2) == 0 ? 0 : 2;
                } else if (*(u8*)((u8*)work + 0xCC) == 2) {
                    lane = irand(2) == 0 ? 0 : 1;
                }
                if (lane != -1) {
                    for (i = 0; i < 3; i++) {
                        u8* slot = (u8*)work + 0xC + i * 0x24;
                        if (*(s32*)(slot + 0xC) == lane) {
                            *(f32*)(slot + 0x10) -= 2.5f;
                        }
                    }
                }
            }
            break;
        case 20:
            for (i = 0; i < 3; i++) {
                *(s32*)((u8*)work + 0xC + i * 0x24) = 20;
            }
            work[0x1F] = 40;
            work[0x20] = 60;
            work[1] = 25;
            break;
        case 25:
            if (work[0x1F] == 50) {
                work[1] = 30;
            }
            break;
        case 30:
            animPoseRelease(work[0x3B]);
            work[0x3B] = -1;
            evtSetValue(evt, args[0], *(u8*)((u8*)work + 0xCC));
            evtSetFloat(evt, args[1], *(f32*)((u8*)work + 0xD0));
            {
                f32 value = *(f32*)((u8*)work + 0xD0);
                s32 result = -1;
                if (value > 0.0f && value < 33.0f) result = 2;
                if (value >= 33.0f && value < 66.0f) result = 3;
                if (value >= 66.0f) result = 4;
                if (result > 4) result = 4;
                evtSetValue(evt, args[2], result);
            }
            return 2;
    }
    for (i = 0; i < 3; i++) {
        u8* slot = (u8*)work + 0xC + i * 0x24;
        s32 state = *(s32*)slot;
        if (state == 0) {
            *(s32*)slot = 5;
        } else if (state == 10) {
            *(s32*)slot = 15;
            *(f32*)(slot + 0x20) = float_1_80427b60;
        } else if (state == 12) {
            *(f32*)(slot + 0x20) = intplGetValue(
                float_0_80427b70, float_1_80427b60, 4,
                *(s32*)(slot + 4), 30);
            if (++*(s32*)(slot + 4) > 30) {
                *(s32*)slot = 15;
            }
        } else if (state == 15) {
            if ((*(u32*)(slot + 8) & keyGetButtonTrg(0)) != 0 &&
                work[0x3D] == 0) {
                *(f32*)(slot + 0x10) -= float_100_80427b78;
            }
        } else if (state == 20) {
            *(BakuVec*)(slot + 0x14) =
                *(const BakuVec*)((const u8*)rodata + 0x74);
        }
    }
    switch (work[0x70]) {
    case 0:
        if ((*work & 1) != 0) {
            work[0x70] = 2;
            *(u8*)((u8*)work + 0x1CC) = 0xFF;
            work[0x80] = 0x5A;
            *(f32*)(work + 0x74) = 300.0f;
            *(f32*)(work + 0x77) = 300.0f;
            work[0x71] = 0;
            work[0x72] = 30;
        } else {
            break;
        }
    case 2:
        *(f32*)(work + 0x74) = intplGetValue(
            *(f32*)(work + 0x77), *(f32*)(work + 0x7A), 0,
            work[0x71], work[0x72]);
        i = (s32)work[0x71] % 12;
        if (i >= 0 && i <= 3) {
            work[0x80] = 0x5A;
        }
        if (i >= 4 && i <= 7) {
            work[0x80] = 0x5B;
        }
        if (i >= 8 && i <= 11) {
            work[0x80] = 0x5C;
        }
        work[0x71]++;
        if ((s32)work[0x71] > (s32)work[0x72]) {
            work[0x70] = 5;
            work[0x74] = work[0x7A];
            work[0x75] = work[0x7B];
            work[0x76] = work[0x7C];
        }
        break;
    case 5:
        work[0x80] = 0x52;
        break;
    case 10:
        work[0x80] = 0x54;
        break;
    }

    {
        extern void bakuGameMarioSurprise(void*, void*);
        extern void bakuGameEnemySurprise(void);
        extern void bakuGameAudienceSurprise(void);
        extern void BtlUnit_GetHomePos(BattleWorkUnit*, f32*, f32*, f32*);
        extern void BtlUnit_SetBodyAnim(BattleWorkUnit*, const char*);
        extern void BtlUnit_SetBodyAnimType(BattleWorkUnit*, s32);
        extern s32 BtlUnit_GetBodyPartsId(BattleWorkUnit*);
        extern BattleWorkUnitPart* BtlUnit_GetPartsPtr(BattleWorkUnit*, s32);
        extern s32 BattleAudience_GetExist(s32);
        extern void BattleAudience_GetHomePosition(s32, f32*, f32*, f32*);
        extern void BattleAudienceSoundBooing(void);
        extern void BattleAudienceSoundCallKind(s32);
        extern void BattleAudienceSoundWhistleKind(s32);
        extern void BattleAudienceSoundBooingKind(s32);
        extern void* effSandarsEntry(s32, f32, f32, f32, f32);
        extern void psndSFX_pit(u32 id, s32 pitch);

        extern f32 float_4096_80427bac;
        extern f32 float_0p25_80427bb0;
        extern f32 float_2_80427bb4;
        extern f32 float_3_80427bb8;
        extern f32 float_4_80427bbc;
        extern f32 float_5_80427bc0;
        extern f32 float_6_80427bc4;
        extern f32 float_50_80427bd4;
        extern f32 float_neg1000_80427bd8;
        extern f32 float_neg800_80427bdc;
        extern f32 float_80_80427be0;
        extern f32 float_neg0p5_80427be4;
        extern f32 float_70_80427be8;
        extern f32 float_neg200_80427bec;
        extern f32 float_20_80427b74;
        extern f32 float_0p1_80427bf0;
        extern f32 float_neg2_80427b8c;
        extern f32 float_7_80427bf4;
        extern f32 float_30_80427c08;
        extern f32 float_neg100_80427c0c;
        extern f32 float_26_80427c10;
        extern f32 float_40_80427b6c;

        extern const u32 vec3_802ff444[3];
        extern const u32 vec3_802ff450[3];
        extern const u32 vec3_802ff45c[3];
        extern const u32 vec3_802ff480[3];
        extern const u32 vec3_802ff48c[3];
        extern const u32 vec3_802ff498[3];
        extern const u32 vec3_802ff4a4[3];
        extern const u32 vec3_802ff4b0[3];
        extern const u32 vec3_802ff4bc[3];
        extern const u32 vec3_802ff4c8[3];
        extern const u32 vec3_802ff4d4[3];

        extern const char str_SFX_BTL_SAC_TIME4_802ff63c[];
        extern const char str_SFX_BTL_SAC_TIME2_802ff650[];
        extern const char str_SFX_VOICE_MARIO_SURP_802ff664[];
        extern const char str_SFX_BTL_SAC_TIME3_802ff680[];
        extern const char str_SFX_BTL_SAC_TIME5_802ff6a0[];
        extern const char str_S_1_80427bc8[];
        extern const char str_S_2_80427bcc[];
        extern const char str_S_3_80427bd0[];
        extern const char str_M_W_1_80427bf8[];
        extern const char str_M_U_2_80427c00[];
        extern const char str_M_C_1_80427c14[];

        BattleWorkUnit* unit;
        f32 factor;
        f32 ceilingGravity;
        f32 div;
        s32 state;

    if ((s32)work[0x1F] > 9 && (s32)work[0x1F] < 21 &&
            (s32)work[0x3C] < 1) {
            work[1] = 20;
            if (*(u8*)((u8*)work + 0xCC) == 1) {
                bakuGameEnemySurpriseReset();
            } else if (*(u8*)((u8*)work + 0xCC) == 0) {
                bakuGameMarioSurpriseReset(mario, party);
            } else if (*(u8*)((u8*)work + 0xCC) < 3) {
                bakuGameAudienceSurpriseReset();
            }
        }

        if ((s32)work[0x1F] > 9 && (s32)work[0x1F] < 31) {
            if ((keyGetButtonTrg(0) & 0x700) != 0) {
                *(f32*)(work + 0x35) = float_1p5_80427ba0;
            }

            *(f32*)(work + 0x36) = float_neg0p3_80427ba4;
            *(f32*)(work + 0x34) += *(f32*)(work + 0x35);
            *(f32*)(work + 0x34) += *(f32*)(work + 0x36);

            if (*(f32*)(work + 0x34) > float_110_80427ba8) {
                *(f32*)(work + 0x34) = float_110_80427ba8;
            }

            if (*(f32*)(work + 0x34) < float_0_80427b70) {
                *(f32*)(work + 0x34) = float_0_80427b70;
                if (psndSFXChk(work[0x6E]) == 0) {
                    psndSFXOff(work[0x6E]);
                }
            }

            if (*(f32*)(work + 0x34) > float_0_80427b70 &&
                psndSFXChk(work[0x6E]) == -1) {
                work[0x6E] = psndSFXOn(((const char*)rodata + 0x288));
            }

            if (psndSFXChk(work[0x6E]) == 0) {
                s32 pitch = (s32)((float_4096_80427bac *
                                   *(f32*)(work + 0x34)) /
                                  float_100_80427b78);
                psndSFX_pit(work[0x6E], pitch);
            }

            *(f32*)(work + 0x35) -= float_0p25_80427bb0;
            if (*(f32*)(work + 0x35) < float_0_80427b70) {
                *(f32*)(work + 0x35) = float_0_80427b70;
            }

            factor = float_1_80427b60;
            if (*(f32*)(work + 0x34) < float_100_80427b78) {
                factor = *(f32*)(work + 0x34) / float_100_80427b78;
            }
            *(f32*)(work + 0x3A) = factor;

            work[0x3C]--;
            work[0x3D]--;
            if ((s32)work[0x3D] < 0) {
                work[0x3D] = 0;
            }

            if (work[0x3C] == 0x1E0) {
                psndSFXOn(((const char*)rodata + 0x29C));
            }
            if ((s32)work[0x3C] > 0x1DF && (s32)work[0x3C] < 0x1EA) {
                factor = intplGetValue(
                    float_2_80427bb4, float_3_80427bb8, 0,
                    0x1E9 - work[0x3C], 10);
                *(f32*)(work + 0x31) = factor;
                *(f32*)(work + 0x32) = factor;
            }

            if (work[0x3C] == 0x168) {
                psndSFXOn(((const char*)rodata + 0x29C));
            }
            if ((s32)work[0x3C] > 0x167 && (s32)work[0x3C] < 0x172) {
                factor = intplGetValue(
                    float_3_80427bb8, float_4_80427bbc, 0,
                    0x171 - work[0x3C], 10);
                *(f32*)(work + 0x31) = factor;
                *(f32*)(work + 0x32) = factor;
            }

            if (work[0x3C] == 0xF0) {
                psndSFXOn(((const char*)rodata + 0x29C));
            }
            if ((s32)work[0x3C] > 0xEF && (s32)work[0x3C] < 0xFA) {
                factor = intplGetValue(
                    float_4_80427bbc, float_5_80427bc0, 0,
                    0xF9 - work[0x3C], 10);
                *(f32*)(work + 0x31) = factor;
                *(f32*)(work + 0x32) = factor;
            }

            if (work[0x3C] == 0x78) {
                psndSFXOn(((const char*)rodata + 0x29C));
            }
            if ((s32)work[0x3C] > 0x77 && (s32)work[0x3C] < 0x82) {
                factor = intplGetValue(
                    float_5_80427bc0, float_6_80427bc4, 0,
                    0x81 - work[0x3C], 10);
                *(f32*)(work + 0x31) = factor;
                *(f32*)(work + 0x32) = factor;
            }

            if (work[0x3C] == 599) {
                animPoseSetAnim(work[0x3B], str_S_1_80427bc8, 0);
            }
            if (work[0x3C] == 400) {
                animPoseSetAnim(work[0x3B], str_S_2_80427bcc, 0);
            }
            if (work[0x3C] == 200) {
                animPoseSetAnim(work[0x3B], str_S_3_80427bd0, 0);
            }
        }

        /*
         * Target uses a jump table for this gameplay-state dispatch.  Keep the
         * semantic blocks intact but use an explicit branch chain for this first
         * reconstruction so the body can be evaluated without creating a new
         * target-first anonymous .data table.
         */
        state = (s32)work[0x1F];

        if (state == 0) goto bg_state_0;
        if (state == 2) goto bg_state_2;
        if (state == 4) goto bg_state_4;
        if (state == 5) goto bg_state_5;
        if (state == 10) goto bg_state_10;
        if (state == 11) goto bg_state_11;
        if (state == 20) goto bg_state_20;
        if (state == 22) goto bg_state_22;
        if (state == 25) goto bg_state_25;
        if (state == 30) goto bg_state_30;
        if (state == 40) goto bg_state_40;
        goto bg_state_done;

    bg_state_0:
        work[0x1F] = 2;
        *(f32*)(work + 0x38) = float_50_80427bd4;
        work[0x20] = 0;
        work[0x22] = rodata[0xA4 / 4];
        work[0x23] = rodata[0xA8 / 4];
        work[0x24] = rodata[0xAC / 4];
        work[0x3C] = 0xFFFFFFFF;
        *(f32*)(work + 0x31) = float_2_80427bb4;
        *(f32*)(work + 0x32) = float_2_80427bb4;
        *(f32*)(work + 0x30) = float_90_80427b7c;
        goto bg_state_2;

    bg_state_2:
        *(f32*)(work + 0x37) = intplGetValue(
            float_neg1000_80427bd8, float_50_80427bd4, 4,
            work[0x20], 30);
        work[0x20]++;
        if ((s32)work[0x20] > 30) {
            work[0x1F] = 4;
        }
        goto bg_state_done;

    bg_state_4:
        work[0x1F] = 5;
        work[0x22] = rodata[0xB0 / 4];
        work[0x23] = rodata[0xB4 / 4];
        work[0x24] = rodata[0xB8 / 4];
        work[0x25] = work[0x22];
        work[0x26] = work[0x23];
        work[0x27] = work[0x24];
        work[0x28] = rodata[0xBC / 4];
        work[0x29] = rodata[0xC0 / 4];
        work[0x2A] = rodata[0xC4 / 4];
        *(f32*)(work + 0x2C) =
            (float_2_80427bb4 *
                 (*(f32*)(work + 0x29) - *(f32*)(work + 0x26)) -
             float_neg800_80427bdc) /
            float_80_80427be0;
        work[0x20] = 40;
        work[0x21] = 40;
        goto bg_state_5;

    bg_state_5:
        *(f32*)(work + 0x23) += *(f32*)(work + 0x2C);
        *(f32*)(work + 0x2C) += float_neg0p5_80427be4;
        work[0x20]--;
        if ((s32)work[0x20] < 0) {
            work[0x1F] = 8;
            work[0x23] = work[0x29];
            *(u8*)((u8*)work + 0xCC) = 0;
            bakuGameMarioSurprise(mario, party);
            btl_camera_shake_h(0, float_4_80427bbc,
                               float_0_80427b70, 10, 4);
            psndSFXOn(((const char*)rodata + 0x2B0));
            BtlUnit_snd_se_pos(
                mario, (s32)((const char*)rodata + 0x2C4),
                0x7F, 0, &mario->position);
        }
        goto bg_state_done;

    bg_state_10:
        work[0x1F] = 11;
        work[0x20] = 0;
        *(f32*)(work + 0x30) = float_70_80427be8;
        goto bg_state_11;

    bg_state_11:
        for (i = 0; i < 3; i++) {
            u8* slot = (u8*)work + 0xC + i * 0x24;
            if (*(f32*)(slot + 0x10) <= float_0_80427b70) {
                work[0x1F] = 20;
                work[0x20] = 0;
                *(u8*)((u8*)work + 0xCD) = (u8)*(s32*)(slot + 0xC);
                if (*(u8*)((u8*)work + 0xCC) ==
                    *(u8*)((u8*)work + 0xCD)) {
                    work[0x1F] = 30;
                }
                break;
            }
        }

        if (*(u8*)((u8*)work + 0xCC) == 0) {
            BtlUnit_GetPos(mario, &x, &y, &z);
            BtlUnit_GetHomePos(mario, &hx, &hy, &hz);
            mario->faceDirection = -1;

            if (y == hy) {
                if (irand(0x50) == 0) {
                    *(f32*)(work + 0x9D) =
                        -float_neg200_80427bec / float_20_80427b74;
                    BtlUnit_SetPos(mario, x,
                        y + float_0p1_80427bf0, z);
                }
            } else {
                y += *(f32*)(work + 0x9D);
                *(f32*)(work + 0x9D) += float_neg2_80427b8c;
                if (y < hy) {
                    y = hy;
                }
                BtlUnit_SetPos(mario, x, y, z);
            }

            if (bakuGamePartyExist(party) == 1) {
                BtlUnit_GetPos(party, &x, &y, &z);
                BtlUnit_GetHomePos(party, &hx, &hy, &hz);
                if (y == hy) {
                    if (irand(0x50) == 0) {
                        *(f32*)(work + 0xA6) =
                            -float_neg200_80427bec / float_20_80427b74;
                        BtlUnit_SetPos(party, x,
                            y + float_0p1_80427bf0, z);
                    }
                } else {
                    y += *(f32*)(work + 0xA6);
                    *(f32*)(work + 0xA6) += float_neg2_80427b8c;
                    if (y < hy) {
                        y = hy;
                    }
                    BtlUnit_SetPos(party, x, y, z);
                }
            }
        }
        goto bg_state_done;

    bg_state_20:
        switch (*(u8*)((u8*)work + 0xCC)) {
        case 0:
            if ((s32)work[0x20] == 0) {
                vec44 = *(const BakuVec*)((const u8*)rodata + 0xC8);
                vec38 = *(const BakuVec*)((const u8*)rodata + 0xD4);

                BtlUnit_GetPos(mario, &x, &y, &z);
                BtlUnit_GetHomePos(mario, &hx, &hy, &hz);
                BtlUnit_SetPos(mario, hx, hy, hz);

                vec44.x = x;
                vec44.y = hy;
                vec44.z = z;
                *(BakuVec*)(work + 0x96) = vec44;

                vec38.x = *(f32*)(work + 0x22);
                vec38.y = hy;
                vec38.z = float_7_80427bf4 + *(f32*)(work + 0x24);
                *(BakuVec*)(work + 0x99) = vec38;

                mario->faceDirection = -1;

                if (bakuGamePartyExist(party) == 1) {
                    BtlUnit_GetPos(party, &x, &y, &z);
                    BtlUnit_GetHomePos(party, &hx, &hy, &hz);
                    BtlUnit_SetPos(party, hx, hy, hz);
                }

                BtlUnit_SetBodyAnim(mario, str_M_W_1_80427bf8);
            }

            if ((s32)work[0x20] >= 0 && (s32)work[0x20] < 8) {
                x = intplGetValue(
                    *(f32*)(work + 0x96), *(f32*)(work + 0x99),
                    0, work[0x20], 7);
                y = *(f32*)(work + 0x97);
                z = intplGetValue(
                    *(f32*)(work + 0x98), *(f32*)(work + 0x9B),
                    0, work[0x20], 7);
                BtlUnit_SetPos(mario, x, y, z);
            }

            if ((s32)work[0x20] == 8) {
                BtlUnit_SetBodyAnim(mario, str_M_U_2_80427c00);
                mario->faceDirection = 1;

                work[0x28] = work[0x22];
                work[0x29] = work[0x23];
                work[0x2A] = work[0x24];
                work[0x25] = work[0x28];
                work[0x26] = work[0x29];
                work[0x27] = work[0x2A];
                *(f32*)(work + 0x29) +=
                    float_30_80427c08 * mario->sizeMultiplier;
            }

            if ((s32)work[0x20] > 7 && (s32)work[0x20] < 13) {
                *(f32*)(work + 0x23) = intplGetValue(
                    *(f32*)(work + 0x26), *(f32*)(work + 0x29),
                    4, work[0x20] - 8, 4);
            }

            if ((s32)work[0x20] > 15) {
                work[0x1F] = 22;
            }
                    break;
        case 1:
            if ((*work & 1) == 0) {
                if ((s32)work[0x20] == 26) {
                    *(f32*)(work + 0x7E) =
                        -float_neg100_80427c0c / float_20_80427b74;
                    *(f32*)(work + 0x75) = float_0_80427b70;
                }

                if ((s32)work[0x20] > 25) {
                    *(f32*)(work + 0x75) += *(f32*)(work + 0x7E);
                    *(f32*)(work + 0x7E) += float_neg1_80427b5c;
                }

                if ((s32)work[0x20] > 30) {
                    work[0x1F] = 22;
                }

                for (i = 0; i < 0x40; i++) {
                    unit = BattleGetUnitPtr(battle, i);
                    if (unit != 0 &&
                        (unit->attributes & 0x20000) == 0 &&
                        BtlUnit_CheckStatus(unit, STATUS_INSTAKILL) == 0 &&
                        unit->alliance == 1 &&
                        BtlUnit_CanActStatus(unit) != 0) {

                        if ((s32)work[0x20] == 0) {
                            BtlUnit_SetBodyAnimType(unit, 0x41);
                        }

                        if ((s32)work[0x20] == 26) {
                            btlDispPoseAnime(
                                BtlUnit_GetPartsPtr(
                                    unit, BtlUnit_GetBodyPartsId(unit)));

                            if ((unit->attributes & 4) == 0 &&
                                (unit->attributes & 2) == 0) {
                                BtlUnit_GetHomePos(unit, &hx, &hy, &hz);
                                BtlUnit_SetPos(unit, hx, hy, hz);
                            }
                        }

                        if ((s32)work[0x20] > 25 &&
                            (unit->attributes & 4) == 0 &&
                            (unit->attributes & 2) == 0) {
                            BtlUnit_GetHomePos(unit, &hx, &hy, &hz);
                            BtlUnit_SetPos(unit, hx,
                                hy + *(f32*)(work + 0x75), hz);
                        }
                    }
                }
            } else {
                work[0x70] = 15;

                if ((s32)work[0x20] == 26) {
                    ceilingGravity = float_neg2_80427b8c;
                    *(f32*)(work + 0x7E) =
                        -float_neg200_80427bec / float_20_80427b74;
                }

                if ((s32)work[0x20] >= 26) {
                    *(f32*)(work + 0x75) += *(f32*)(work + 0x7E);
                    *(f32*)(work + 0x7E) += ceilingGravity;
                    *(f32*)(work + 0x23) =
                        float_30_80427c08 + *(f32*)(work + 0x75);
                }

                if ((s32)work[0x20] > 30) {
                    work[0x1F] = 22;
                }
            }
                    break;
        case 2:
            div = float_1_80427b60;
            if (*(u8*)((u8*)work + 0xCD) == 1) {
                div = float_2_80427bb4;
            }

            if ((*work & 2) != 0) {
                work[0x1F] = 30;
                goto bg_state_done;
            }

            factor = float_26_80427c10 / div;

            if ((f32)(s32)work[0x20] == factor) {
                *(f32*)(work + 0xA9) = float_0_80427b70;
                *(f32*)(work + 0xAC) =
                    -float_neg200_80427bec / float_20_80427b74;
            }

            if ((f32)(s32)work[0x20] >= factor) {
                *(f32*)(work + 0xA9) += *(f32*)(work + 0xAC);
                *(f32*)(work + 0xAC) += float_neg2_80427b8c;
            }

            if ((f32)(s32)work[0x20] >
                float_30_80427c08 / div) {
                work[0x1F] = 22;
            }

            for (i = 0; i < 200; i++) {
                u8* audience = (u8*)BattleAudienceGetPtr(i);
                if (bakuGameAudienceCanThrowPos(i) == 1 &&
                    BattleAudience_GetExist(i) != 0 &&
                    (*(u32*)audience & 0x10) != 0) {
                    BattleAudience_GetHomePosition(i, &x, &y, &z);
                    BattleAudience_SetPosition(
                        i, x, y + *(f32*)(work + 0xA9), z);
                    *(f32*)(work + 0x23) =
                        float_20_80427b74 + y +
                        *(f32*)(work + 0xA9);
                }
            }
                    break;
        }

        work[0x20]++;

        if ((keyGetButtonTrg(0) & 0x400) != 0) {
            *(u8*)((u8*)work + 0xCD) =
                (u8)work[0x18];
        }
        if ((keyGetButtonTrg(0) & 0x200) != 0) {
            *(u8*)((u8*)work + 0xCD) =
                (u8)work[0x0F];
        }
        if ((keyGetButtonTrg(0) & 0x100) != 0) {
            *(u8*)((u8*)work + 0xCD) =
                (u8)work[0x06];
        }
        goto bg_state_done;

    bg_state_22:
        if (*(u8*)((u8*)work + 0xCC) == 1) {
            work[0x22] = rodata[0xEC / 4];
            work[0x23] = rodata[0xF0 / 4];
            work[0x24] = rodata[0xF4 / 4];
            work[0x25] = work[0x22];
            work[0x26] = work[0x23];
            work[0x27] = work[0x24];

            if ((*work & 1) != 0) {
                work[0x22] = work[0x74];
                work[0x23] = work[0x75];
                work[0x24] = work[0x76];
                work[0x25] = work[0x22];
                work[0x26] = work[0x23];
                work[0x27] = work[0x24];
                *(f32*)(work + 0x23) =
                    float_30_80427c08 + *(f32*)(work + 0x75);
                *(f32*)(work + 0x26) =
                    float_30_80427c08 + *(f32*)(work + 0x75);
            }
            bakuGameEnemySurpriseReset();
        } else if (*(u8*)((u8*)work + 0xCC) == 0) {
            work[0x22] = rodata[0xE0 / 4];
            work[0x23] = rodata[0xE4 / 4];
            work[0x24] = rodata[0xE8 / 4];
            work[0x25] = work[0x22];
            work[0x26] = work[0x23];
            work[0x27] = work[0x24];
            *(f32*)(work + 0x26) += float_30_80427c08;
            *(f32*)(work + 0x23) += float_30_80427c08;
            bakuGameMarioSurpriseReset(mario, party);
            BtlUnit_SetBodyAnim(mario, str_M_C_1_80427c14);
        } else if (*(u8*)((u8*)work + 0xCC) < 3) {
            work[0x22] = rodata[0xF8 / 4];
            work[0x23] = rodata[0xFC / 4];
            work[0x24] = rodata[0x100 / 4];
            work[0x25] = work[0x22];
            work[0x26] = work[0x23];
            work[0x27] = work[0x24];
            *(f32*)(work + 0x26) += float_30_80427c08;
            *(f32*)(work + 0x23) += float_20_80427b74;
            bakuGameAudienceSurpriseReset();
        }

        if (*(u8*)((u8*)work + 0xCD) == 1) {
            work[0x28] = rodata[0x110 / 4];
            work[0x29] = rodata[0x114 / 4];
            work[0x2A] = rodata[0x118 / 4];

            if ((*work & 1) != 0) {
                work[0x28] = rodata[0x11C / 4];
                work[0x29] = rodata[0x120 / 4];
                work[0x2A] = rodata[0x124 / 4];
                *(f32*)(work + 0x29) += float_30_80427c08;
            }
            bakuGameEnemySurprise();
        } else if (*(u8*)((u8*)work + 0xCD) == 0) {
            work[0x28] = rodata[0x104 / 4];
            work[0x29] = rodata[0x108 / 4];
            work[0x2A] = rodata[0x10C / 4];
            bakuGameMarioSurprise(mario, party);
        } else if (*(u8*)((u8*)work + 0xCD) < 3) {
            if ((*work & 2) == 0) {
                work[0x28] = rodata[0x134 / 4];
                work[0x29] = rodata[0x138 / 4];
                work[0x2A] = rodata[0x13C / 4];
                *(f32*)(work + 0x29) += float_20_80427b74;
            } else {
                work[0x28] = rodata[0x128 / 4];
                work[0x29] = rodata[0x12C / 4];
                work[0x2A] = rodata[0x130 / 4];
            }
            bakuGameAudienceSurprise();
        }

        work[0x1F] = 25;
        work[0x20] = (s32)float_40_80427b6c;
        work[0x21] = (s32)float_40_80427b6c;
        *(f32*)(work + 0x2C) =
            (float_2_80427bb4 *
                 (*(f32*)(work + 0x29) -
                  *(f32*)(work + 0x26)) -
             float_neg800_80427bdc) /
            float_80_80427be0;

        if (*(u8*)((u8*)work + 0xCC) == 0) {
            BtlUnit_GetPos(mario, &x, &y, &z);

            vec2C = *(const BakuVec*)((const u8*)rodata + 0x140);
            vec2C.x = x;
            vec2C.y = y;
            vec2C.z = z;
            *(BakuVec*)(work + 0x96) = vec2C;

            BtlUnit_GetHomePos(mario, &x, &y, &z);

            vec20 = *(const BakuVec*)((const u8*)rodata + 0x14C);
            vec20.x = x;
            vec20.y = y;
            vec20.z = z;
            *(BakuVec*)(work + 0x99) = vec20;
        }

        work[0x6F] = psndSFXOn(((const char*)rodata + 0x2E0));

        i = BattleAudience_GetAudienceNum();
        if (*(u8*)((u8*)work + 0xCD) == 1) {
            if (i > 0 && i < 50) {
                BattleAudienceSoundCallKind(1);
            }
            if (i > 49 && i < 100) {
                BattleAudienceSoundCallKind(1);
                BattleAudienceSoundWhistleKind(1);
            }
            if (i > 99 && i < 150) {
                BattleAudienceSoundCallKind(2);
                BattleAudienceSoundWhistleKind(2);
            }
            if (i > 149) {
                BattleAudienceSoundCallKind(2);
                BattleAudienceSoundWhistleKind(3);
            }
        } else if (*(u8*)((u8*)work + 0xCD) == 0) {
            BattleAudienceSoundBooing();
        } else if (*(u8*)((u8*)work + 0xCD) < 3) {
            if (i > 0 && i < 50) {
                BattleAudienceSoundBooingKind(5);
            }
            if (i > 49 && i < 100) {
                BattleAudienceSoundBooingKind(6);
            }
            if (i > 99 && i < 150) {
                BattleAudienceSoundBooingKind(7);
            }
            if (i > 149) {
                BattleAudienceSoundBooingKind(8);
            }
        }
        goto bg_state_25;

    bg_state_25:
        *(f32*)(work + 0x22) = intplGetValue(
            *(f32*)(work + 0x25), *(f32*)(work + 0x28), 0,
            work[0x21] - work[0x20], work[0x21]);
        *(f32*)(work + 0x24) = intplGetValue(
            *(f32*)(work + 0x27), *(f32*)(work + 0x2A), 0,
            work[0x21] - work[0x20], work[0x21]);
        *(f32*)(work + 0x23) += *(f32*)(work + 0x2C);
        *(f32*)(work + 0x2C) += float_neg0p5_80427be4;

        if (*(u8*)((u8*)work + 0xCC) == 1) {
            if (*(f32*)(work + 0x75) != float_0_80427b70) {
                *(f32*)(work + 0x75) += *(f32*)(work + 0x7E);
                *(f32*)(work + 0x7E) += float_neg1_80427b5c;
            }

            if (*(f32*)(work + 0x75) < float_0_80427b70) {
                *(f32*)(work + 0x75) = float_0_80427b70;
            }

            if ((*work & 1) == 0) {
                for (i = 0; i < 0x40; i++) {
                    unit = BattleGetUnitPtr(battle, i);
                    if (unit != 0 &&
                        unit->alliance == 1 &&
                        BtlUnit_CanActStatus(unit) != 0 &&
                        (unit->attributes & 0x20000) == 0 &&
                        BtlUnit_CheckStatus(unit, STATUS_INSTAKILL) == 0 &&
                        (unit->attributes & 4) == 0 &&
                        (unit->attributes & 2) == 0) {
                        BtlUnit_GetHomePos(unit, &hx, &hy, &hz);
                        BtlUnit_SetPos(unit, hx,
                            hy + *(f32*)(work + 0x75), hz);
                    }
                }
            } else {
                work[0x70] = 15;

                if (*(f32*)(work + 0x75) != float_0_80427b70) {
                    *(f32*)(work + 0x75) += *(f32*)(work + 0x7E);
                    *(f32*)(work + 0x7E) += float_neg2_80427b8c;
                }

                if (*(f32*)(work + 0x75) <= float_0_80427b70) {
                    *(f32*)(work + 0x75) = float_0_80427b70;
                    work[0x70] = 5;
                }
            }
        } else if (*(u8*)((u8*)work + 0xCC) == 0) {
            if (work[0x21] - work[0x20] == 20) {
                BtlUnit_SetBodyAnim(mario, str_M_W_1_80427bf8);
            }

            i = work[0x21] - work[0x20];
            if (i > 19 && i < 35) {
                x = intplGetValue(
                    *(f32*)(work + 0x96), *(f32*)(work + 0x99),
                    0, i - 20, 14);
                y = *(f32*)(work + 0x97);
                z = intplGetValue(
                    *(f32*)(work + 0x98), *(f32*)(work + 0x9B),
                    0, i - 20, 14);
                BtlUnit_SetPos(mario, x, y, z);
            }

            if (work[0x21] - work[0x20] == 34) {
                btlDispPoseAnime(
                    BtlUnit_GetPartsPtr(
                        mario, BtlUnit_GetBodyPartsId(mario)));
            }
        } else if (*(u8*)((u8*)work + 0xCC) < 3) {
            if (*(f32*)(work + 0xA9) != float_0_80427b70) {
                *(f32*)(work + 0xA9) += *(f32*)(work + 0xAC);
                *(f32*)(work + 0xAC) += float_neg2_80427b8c;
            }

            if (*(f32*)(work + 0xA9) <= float_0_80427b70) {
                *(f32*)(work + 0xA9) = float_0_80427b70;
            }

            for (i = 0; i < 200; i++) {
                u8* audience = (u8*)BattleAudienceGetPtr(i);
                if (bakuGameAudienceCanThrowPos(i) == 1 &&
                    BattleAudience_GetExist(i) != 0 &&
                    (*(u32*)audience & 0x10) != 0) {
                    BattleAudience_GetHomePosition(i, &x, &y, &z);
                    BattleAudience_SetPosition(
                        i, x, y + *(f32*)(work + 0xA9), z);
                }
            }
        }

        work[0x20]--;
        if ((s32)work[0x20] < 0) {
            work[0x1F] = 30;
            work[0x22] = work[0x28];
            work[0x23] = work[0x29];
            work[0x24] = work[0x2A];
            *(u8*)((u8*)work + 0xCC) =
                *(u8*)((u8*)work + 0xCD);

            btl_camera_shake_h(0, *(f32*)(work + 0x31),
                               float_0_80427b70, 10, 4);
            psndSFXOn(((const char*)rodata + 0x2B0));

            if (psndSFXChk(work[0x6F]) == 0) {
                psndSFXOff(work[0x6F]);
            }
        }
        goto bg_state_done;

    bg_state_30:
        work[0x1F] = 10;
        for (i = 0; i < 3; i++) {
            *(f32*)((u8*)work + 0x1C + i * 0x24) =
                float_100_80427b78;
        }

        if (*(u8*)((u8*)work + 0xCC) == 1 &&
            (*work & 1) != 0) {
            work[0x70] = 10;
        }

        if (*(u8*)((u8*)work + 0xCD) == 0) {
            BtlUnit_snd_se_pos(
                mario, (s32)((const char*)rodata + 0x2C4),
                0x7F, 0, &mario->position);
        }
        goto bg_state_done;

    bg_state_40:
        effSandarsEntry(3,
            *(f32*)(work + 0x22),
            *(f32*)(work + 0x23),
            *(f32*)(work + 0x24),
            float_0_80427b70);

        btl_camera_shake_h(0, *(f32*)(work + 0x31),
                           float_0_80427b70, 30, 4);

        psndSFXOn(((const char*)rodata + 0x300));

        if (psndSFXChk(work[0x6E]) == 0) {
            psndSFXOff(work[0x6E]);
        }
        if (psndSFXChk(work[0x6F]) == 0) {
            psndSFXOff(work[0x6F]);
        }

        work[0x1F] = 50;

        for (i = 0; i < 200; i++) {
            u32* audience = (u32*)BattleAudienceGetPtr(i);
            *audience &= ~0x10U;

            if (bakuGameAudienceCanThrowPos(i) == 1) {
                BattleAudience_GetHomePosition(i, &x, &y, &z);
                BattleAudience_SetPosition(i, x, y, z);
            }
        }

    bg_state_done:
        if ((s32)work[0x3B] != -1) {
            animPoseMain();
        }

        dispEntry(8, 1, bakuGameDisp2D, 0, 900.0f);
        dispEntry(4, 1, bakuGameDisp3D, 0, float_0_80427b70);
        return 0;
    }
}

/* stub-fill: bakuGameHeihoReturn | missing_definition | ghidra_signature */
s32 bakuGameHeihoReturn(s32 unused, s32 reset) {
    extern void* GetBakuGamePtr(void);
    extern f32 intplGetValue(s32 type, s32 current, s32 total, f32 start, f32 end);
    extern void dispEntry(s32 cameraId, s32 layer, void* func, s32 param, f32 z);
    extern u32 vec3_802ff4f8[3];
    extern f32 float_neg2_80427b8c;

    void* work = GetBakuGamePtr();
    s32 phase;
    s32 frameMod;

    if ((*(u32*)work & 1) == 0) {
        return 2;
    }

    if (reset != 0) {
        *(s32*)((s32)work + 0x1C0) = 0;
    }

    switch (*(s32*)((s32)work + 0x1C0)) {
        case 0:
            if (*(f32*)((s32)work + 0x1D4) != float_0_80427b70) {
                *(f32*)((s32)work + 0x1D4) += *(f32*)((s32)work + 0x1F8);
                *(f32*)((s32)work + 0x1F8) += float_neg2_80427b8c;
            }
            if (*(f32*)((s32)work + 0x1D4) <= float_0_80427b70) {
                *(f32*)((s32)work + 0x1D4) = float_0_80427b70;
                *(s32*)((s32)work + 0x1C0) = 5;
            }
            break;
        case 5:
            *(s32*)((s32)work + 0x1C0) = 10;
            *(s8*)((s32)work + 0x1CC) = 1;
            *(s32*)((s32)work + 0x1DC) = *(s32*)((s32)work + 0x1D0);
            *(s32*)((s32)work + 0x1E0) = *(s32*)((s32)work + 0x1D4);
            *(s32*)((s32)work + 0x1E4) = *(s32*)((s32)work + 0x1D8);
            *(s32*)((s32)work + 0x1E8) = vec3_802ff4f8[0];
            *(s32*)((s32)work + 0x1EC) = vec3_802ff4f8[1];
            *(s32*)((s32)work + 0x1F0) = vec3_802ff4f8[2];
            *(f32*)((s32)work + 0x1F0) = *(f32*)((s32)work + 0x1D8);
            *(s32*)((s32)work + 0x1C4) = 0;
            *(s32*)((s32)work + 0x1C8) = 0x1E;
            break;
        case 10:
            *(f32*)((s32)work + 0x1D0) = intplGetValue(0, *(s32*)((s32)work + 0x1C4),
                                                        *(s32*)((s32)work + 0x1C8),
                                                        *(f32*)((s32)work + 0x1DC),
                                                        *(f32*)((s32)work + 0x1E8));
            frameMod = *(s32*)((s32)work + 0x1C4) % 12;
            if (frameMod >= 0 && frameMod <= 3) {
                *(s32*)((s32)work + 0x200) = 0x5A;
            }
            if (frameMod >= 4 && frameMod <= 7) {
                *(s32*)((s32)work + 0x200) = 0x5B;
            }
            if (frameMod >= 8 && frameMod <= 11) {
                *(s32*)((s32)work + 0x200) = 0x5C;
            }
            phase = *(s32*)((s32)work + 0x1C4) + 1;
            *(s32*)((s32)work + 0x1C4) = phase;
            if (phase > *(s32*)((s32)work + 0x1C8)) {
                return 2;
            }
            break;
    }

    dispEntry(4, 1, bakuGameDisp3D, 0, float_0_80427b70);
    return 0;
}

void bakuGameDecideButton(s32 flag) {
    typedef struct ButtonTable {
        s32 values[9];
    } ButtonTable;
    typedef struct PatternTable {
        s32 values[18];
    } PatternTable;
    extern void* GetBakuGamePtr(void);
    extern const ButtonTable dat_802ff504;
    extern const PatternTable dat_802ff528;
    extern s32 irand(s32 max);

    void* work;
    ButtonTable buttons;
    PatternTable patterns;
    s32 row;
    s32* rowBase;
    s32* buttonsBase;
    s32 workOffset;
    s32 rowOffset;
    s32 i;
    s32 entryOffset;
    s32 buttonOffset;
    s32* src;
    s32* dst;

    work = GetBakuGamePtr();
    buttons = dat_802ff504;
    patterns = dat_802ff528;
    row = irand(6) * 0xC;
    rowBase = (s32*)((s32)&patterns.values[0] + row);
    buttonsBase = buttons.values;
    workOffset = 0;
    rowOffset = 0;
    for (i = 0; i < 3; i++) {
        entryOffset = workOffset + 0xC;
        dst = (s32*)((s32)work + entryOffset);
        workOffset += 0x24;
        *(s32*)((s32)dst + 0xC) = *(s32*)((s32)rowBase + rowOffset);
        buttonOffset = *(s32*)((s32)rowBase + rowOffset) * 0xC;
        rowOffset += 4;
        src = (s32*)((s32)buttonsBase + buttonOffset);
        *(s32*)((s32)dst + 0x14) = src[0];
        *(s32*)((s32)dst + 0x18) = src[1];
        *(s32*)((s32)dst + 0x1C) = src[2];
    }
    if (flag == 0) {
        *(s32*)((s32)work + 0xF4) = 0x3C;
    }
}

s32 bakuGamePartyExist(void* unit) {
    if (unit == 0) {
        return 0;
    }
    if (BtlUnit_CheckStatus(unit, 0x1B) != 0) {
        return 0;
    }
    return BtlUnit_CanActStatus(unit) != 0;
}

s32 bakuGameAudienceCanThrowPos(s32 pos) {
    s32 i;
    s32 base = 0;
    for (i = 0; i < 3; i++) {
        if (pos >= base + 8 && pos <= base + 11) {
            return 1;
        }
        base += 0x14;
    }
    return 0;
}

void bakuGameMarioSurpriseReset(void* mario, void* party) {
    btlDispPoseAnime(BtlUnit_GetPartsPtr(mario, BtlUnit_GetBodyPartsId(mario)));
    if (bakuGamePartyExist(party) != 0) {
        btlDispPoseAnime(BtlUnit_GetPartsPtr(party, BtlUnit_GetBodyPartsId(party)));
    }
}

void bakuGameEnemySurpriseReset(void) {
    extern void* BattleGetUnitPtr(void* battleWork, s32 unitId);
    extern s32 BtlUnit_CheckStatus(void* unit, s32 status);
    extern s32 BtlUnit_GetBodyPartsId(void* unit);
    extern void* BtlUnit_GetPartsPtr(void* unit, s32 partsId);
    extern void btlDispPoseAnime(void* part);

    s32 i;
    void* battleWork;
    void* unit;

    battleWork = _battleWorkPointer;
    for (i = 0; i < 0x40; i++) {
        unit = BattleGetUnitPtr(battleWork, i);
        if (unit != 0 && *(s8*)((s32)unit + 0xC) == 1 && (*(u32*)((s32)unit + 0x104) & 0x20000) == 0
            && BtlUnit_CheckStatus(unit, 0x1B) == 0) {
            btlDispPoseAnime(BtlUnit_GetPartsPtr(unit, BtlUnit_GetBodyPartsId(unit)));
        }
    }
}

void bakuGameAudienceSurpriseReset(void) {
    extern void* BattleAudienceGetPtr(s32 id);
    extern s32 BattleAudience_GetExist(s32 id);

    s32 i;
    void* audience;
    f32 angle;

    i = 0;
    angle = float_90_80427b7c;
    while (i < 0xC8) {
        if ((u8)BattleAudience_GetExist(i) != 0) {
            audience = BattleAudienceGetPtr(i);
            *(f32*)((s32)audience + 0xE8) = angle;
            if (bakuGameAudienceCanThrowPos(i) == 0) {
                *(u32*)audience &= ~0x10;
            }
        }
        i++;
    }
}

void bakuGameMarioSurprise(void* mario, void* party) {
    extern void BtlUnit_SetBodyAnim(void* unit, const char* name);
    extern const char str_btl_wn_sac_bakugame_802ff3a0[];
    extern const char str_M_N_7_80427b84[6];

    const char* base;

    base = str_btl_wn_sac_bakugame_802ff3a0;
    BtlUnit_SetBodyAnim(mario, str_M_N_7_80427b84);
    if (bakuGamePartyExist(party) != 0) {
        switch (*(s32*)((s32)party + 4) - 0xE0) {
            case 0:
                BtlUnit_SetBodyAnim(party, base + 0x24C);
                break;
            case 1:
                BtlUnit_SetBodyAnim(party, base + 0x254);
                break;
            case 2:
                BtlUnit_SetBodyAnim(party, base + 0x25C);
                break;
            case 3:
                BtlUnit_SetBodyAnim(party, base + 0x264);
                break;
            case 4:
                BtlUnit_SetBodyAnim(party, base + 0x26C);
                break;
            case 5:
                BtlUnit_SetBodyAnim(party, base + 0x274);
                break;
            case 6:
                BtlUnit_SetBodyAnim(party, base + 0x27C);
                break;
        }
    }
}

void bakuGameEnemySurprise(void) {
    s32 i;
    void* battleWork = _battleWorkPointer;
    for (i = 0; i < 0x40; i++) {
        void* unit = BattleGetUnitPtr(battleWork, i);
        if (unit != 0 && *(s8*)((s32)unit + 0xC) == 1 && (*(u32*)((s32)unit + 0x104) & 0x20000) == 0
            && BtlUnit_CheckStatus(unit, 0x1B) == 0) {
            BtlUnit_SetBodyAnimType(unit, 0x27);
        }
    }
}

void bakuGameAudienceSurprise(void) {
    extern void* BattleAudienceGetPtr(s32 id);
    extern s32 BattleAudience_GetWaiting(s32 id);
    extern void BattleAudience_SetAnim(s32 id, s32 anim, s32 pose);
    extern s32 irand(s32 max);
    extern void* memset(void* dst, int value, u32 size);
    extern f32 float_270_80427b80;

    s32 i;
    void* audience;

    for (i = 0; i < 0xC8; i++) {
        audience = BattleAudienceGetPtr(i);
        if ((u8)BattleAudience_GetWaiting(i) != 0) {
            *(u32*)audience |= 0x10;
            memset((void*)((s32)audience + 0xF8), 0, 0x34);
            if (bakuGameAudienceCanThrowPos(i) == 0) {
                BattleAudience_SetAnim(i, 7, 0);
                if (irand(2) == 0) {
                    *(f32*)((s32)audience + 0xE8) = float_90_80427b7c;
                } else {
                    *(f32*)((s32)audience + 0xE8) = float_270_80427b80;
                }
            } else {
                BattleAudience_SetAnim(i, 3, 0);
            }
        }
    }
}

/* CHATGPT STUB FILL: main/action/star/sac_bakugame 20260624_184823 */

/* stub-fill: bakuGameDisp2D | missing_definition | ghidra_signature */
void bakuGameDisp2D(void) {
    extern void* GetBakuGamePtr(void);
    extern void btlGetScreenPoint(void* pos, void* out);
    extern void iconDispGx(void* pos, s32 size, s32 iconId, f32 scale);
    extern void BattleAcDrawGauge(s32 x, s32 y, s32 icon, s32 unk1, s32 unk2, s32 unk3, s32 unk4, f32 value);

    void* work = GetBakuGamePtr();
    void* battle = _battleWorkPointer;
    f32 screen[2];
    s32 i;
    s32 offset;
    s32 icon;

    for (i = 0, offset = 0; i < 3; i++, offset += 0x24) {
        void* item = (void*)((s32)work + offset + 0xC);
        switch (*(s32*)((s32)item + 8)) {
            case 0x100:
                icon = 0x6C;
                break;
            case 0x200:
                icon = 0x6E;
                break;
            case 0x400:
                icon = 0x70;
                break;
            default:
                icon = 0;
                break;
        }
        if ((u16)icon != 0) {
            btlGetScreenPoint((void*)((s32)item + 0x14), screen);
            iconDispGx(screen, 0x10, icon, *(f32*)((s32)item + 0x20));
        }
    }

    if (*(f32*)((s32)work + 0xD0) >= float_100_80427b78) {
        *(u8*)((s32)battle + 0x1F2C) = 0;
        *(u8*)((s32)battle + 0x1F2F) = 0xFF;
        *(u8*)((s32)battle + 0x1F2D) += *(u8*)((s32)battle + 0x1F31);
        *(u8*)((s32)battle + 0x1F2E) += *(u8*)((s32)battle + 0x1F32);
        if (*(u8*)((s32)battle + 0x1F2D) <= 0x80 && *(s8*)((s32)battle + 0x1F31) <= 0) {
            *(u8*)((s32)battle + 0x1F31) = 0x10;
            *(u8*)((s32)battle + 0x1F32) = 0x10;
        }
        if (*(u8*)((s32)battle + 0x1F2D) >= 0xF5 && *(s8*)((s32)battle + 0x1F31) >= 0) {
            *(u8*)((s32)battle + 0x1F31) = -0x10;
            *(u8*)((s32)battle + 0x1F32) = -0x10;
        }
    }

    BattleAcDrawGauge((s32)*(f32*)((s32)work + 0xDC), (s32)*(f32*)((s32)work + 0xE0),
                      0xB1, 3, 0x21, 0x42, 0x64, *(f32*)((s32)work + 0xE8));
}

void bakuGameDisp3D(void) {
    typedef f32 Mtx[3][4];
    extern void* camGetPtr(s32 cameraId);
    extern void PSMTXScale(Mtx m, f32 x, f32 y, f32 z);
    extern void PSMTXTrans(Mtx m, f32 x, f32 y, f32 z);
    extern void PSMTXConcat(void* a, void* b, void* c);
    extern void animPoseDrawMtx(s32 poseId, void* mtx, s32 mode, f32 rot, f32 scale);
    extern void GXSetCullMode(s32 mode);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32 attr, s32 type);
    extern void GXSetVtxAttrFmt(s32 vtxfmt, s32 attr, s32 comptype, s32 compsize, s32 frac);
    extern void GXSetNumChans(s32 num);
    extern void GXSetChanCtrl(s32 chan, s32 enable, s32 ambSrc, s32 matSrc, s32 lightMask, s32 diffFn, s32 attnFn);
    extern void GXSetChanMatColor(s32 chan, void* color);
    extern void GXSetNumTexGens(s32 num);
    extern void GXSetTexCoordGen2(s32 texgen, s32 type, s32 src, s32 mtxsrc, s32 normalize, s32 postmtx);
    extern void GXSetNumTevStages(s32 num);
    extern void GXSetTevColorOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevColorIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevAlphaOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevAlphaIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevOrder(s32 stage, s32 texcoord, s32 texmap, s32 color);
    extern void* BattleAudienceBaseGetPtr(void);
    extern void TEXGetGXTexObjFromPalette(void* palette, void* texObj, s32 id);
    extern void GXLoadTexObj(void* texObj, s32 mapid);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXBegin(s32 primitive, s32 vtxfmt, s32 nverts);
    extern u32 dat_80427b58;
    extern u32 dat_802ff570[16];
    extern f32 float_180_80427b64;
    extern f32 float_neg20_80427b68;
    extern f32 float_40_80427b6c;
    extern f32 float_20_80427b74;

    u8* work;
    void* cam;
    Mtx modelMtx;
    Mtx transMtx;
    Mtx scaleMtx;
    u32 color;
    u32 texObj[8];
    u32 uvWords[16];
    void* audience;
    void* tpl;
    s32 i;
    s32 base;
    volatile f32* fifo;
    f32* uv;

    work = (u8*)GetBakuGamePtr();
    cam = camGetPtr(4);

    PSMTXScale(scaleMtx, float_neg1_80427b5c, float_1_80427b60, float_1_80427b60);
    PSMTXTrans(transMtx, *(f32*)(work + 0x88), *(f32*)(work + 0x8C), *(f32*)(work + 0x90));
    PSMTXConcat(transMtx, scaleMtx, modelMtx);

    if (*(s32*)(work + 0xEC) != -1) {
        animPoseDrawMtx(*(s32*)(work + 0xEC), modelMtx, 2, float_180_80427b64, *(f32*)(work + 0xC4));
    }

    if ((*(u32*)work & 1) != 0) {
        GXSetCullMode(0);
        GXClearVtxDesc();
        GXSetVtxDesc(9, 1);
        GXSetVtxDesc(0xD, 1);
        GXSetVtxAttrFmt(0, 9, 1, 4, 0);
        GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);
        GXSetNumChans(1);
        GXSetChanCtrl(4, 0, 1, 0, 0, 2, 2);
        color = dat_80427b58;
        GXSetChanMatColor(4, &color);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
        GXSetNumTevStages(1);
        GXSetTevColorOp(0, 0, 0, 0, 1, 0);
        GXSetTevColorIn(0, 0xF, 0xC, 8, 0xA);
        GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
        GXSetTevAlphaIn(0, 7, 5, 4, 7);
        GXSetTevOrder(0, 0, 0, 4);

        audience = BattleAudienceBaseGetPtr();
        tpl = *(void**)(*(s32*)(*(s32*)((s32)audience + 0xC) + 0xA0) + 0);
        TEXGetGXTexObjFromPalette(tpl, texObj, *(s32*)(work + 0x200));
        GXLoadTexObj(texObj, 0);

        PSMTXTrans(transMtx, *(f32*)(work + 0x1D0), *(f32*)(work + 0x1D4), *(f32*)(work + 0x1D8));
        PSMTXConcat((void*)((s32)cam + 0x11C), transMtx, modelMtx);
        GXLoadPosMtxImm(modelMtx, 0);

        for (i = 0; i < 16; i++) {
            uvWords[i] = dat_802ff570[i];
        }
        uv = (f32*)uvWords;
        if (*(s8*)(work + 0x1CC) == 1) {
            base = 4;
        } else {
            base = 0;
        }

        GXBegin(0x80, 0, 4);
        fifo = (volatile f32*)0xCC008000;
        *fifo = float_neg20_80427b68;
        *fifo = float_40_80427b6c;
        *fifo = float_0_80427b70;
        *fifo = uv[base * 2];
        *fifo = uv[base * 2 + 1];
        *fifo = float_20_80427b74;
        *fifo = float_40_80427b6c;
        *fifo = float_0_80427b70;
        *fifo = uv[(base + 1) * 2];
        *fifo = uv[(base + 1) * 2 + 1];
        *fifo = float_20_80427b74;
        *fifo = float_0_80427b70;
        *fifo = float_0_80427b70;
        *fifo = uv[(base + 2) * 2];
        *fifo = uv[(base + 2) * 2 + 1];
        *fifo = float_neg20_80427b68;
        *fifo = float_0_80427b70;
        *fifo = float_0_80427b70;
        *fifo = uv[(base + 3) * 2];
        *fifo = uv[(base + 3) * 2 + 1];
    }
}

void bakuGameDispStar(void) {
    ;
}
