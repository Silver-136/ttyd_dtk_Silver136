#include "unit/party/unit_party_yoshi.h"

void _btlYoshiDisp(void* camera, void* work) {
    extern f32 float_0_804240b4;
    extern f32 float_0p4_804240ac;
    extern f32 float_neg0p4_804240b0;
    extern f32 float_20_804240b8;
    extern s32 dat_804240a8;
    extern u8 _yoshig_tpl_no_tbl;
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


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
u8 _gundan_yoshi_run_effect(void* evt, s32 first) {
    typedef struct LocalVec {
        f32 x;
        f32 y;
        f32 z;
    } LocalVec;

    extern void* _battleWorkPointer;
    extern s32 evtGetValue(void* evt, s32 arg);
    extern s32 BattleTransID(void* evt, s32 id);
    extern void* BattleGetUnitPtr(void* battleWork, s32 id);
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
    extern char str_SFX_BTL_YOSHI_TAIGUN_802f7cb8[];
    extern f32 float_0_804240b4;
    extern f32 float_3_804240bc;
    extern f32 float_0p1_804240c0;
    extern f32 float_0p001_804240c4;
    extern f32 float_65_804240c8;
    extern f32 float_neg1_804240cc;
    extern f32 float_neg80_804240d0;
    extern f32 float_300_804240d4;
    extern f32 float_1_804240d8;
    extern f32 float_0p05_804240dc;
    extern f32 float_5_804240e0;
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
        *(s32*)((s32)evt + 0x7C) = psndSFXOn(str_SFX_BTL_YOSHI_TAIGUN_802f7cb8);
    } else {
        work = *(s32**)((s32)unit + 0x314);
    }

    if (work[0] == 999) {
        psndSFXOff(*(s32*)((s32)evt + 0x7C));
        BattleFree(*(void**)((s32)unit + 0x314));
        *(void**)((s32)unit + 0x314) = NULL;
        return 2;
    }
    if (work[0] == 0) {
        work[0]++;
    }

    active = 0;
    *(f32*)((s32)work + 0x3008) = float_0_804240b4;
    p = work + 1;
    for (i = 0; i < 0x100; i++, p += 0xC) {
        *(u8*)((s32)p + 0x24) = *(u8*)((s32)p + 0x24) + 1;
        if (*(u8*)((s32)p + 0x24) > 2) {
            *(u8*)((s32)p + 0x24) = 0;
            *(u8*)((s32)p + 0x25) = *(u8*)((s32)p + 0x25) + 1;
            if (*(u8*)((s32)p + 0x25) > 5) {
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
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

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
        diff = target - cur;
        if (diff < float_0_804240b4) {
            diff = -diff;
        }
        speed = *(f32*)((s32)unit + 0x170);
        if (speed <= diff) {
            *(f32*)((s32)unit + 0x154) = cur + speed * (f32)*(s8*)((s32)unit + 0x188);
        } else {
            *(f32*)((s32)unit + 0x154) = target;
        }
    }

    cur = *(f32*)((s32)unit + 0x15C);
    target = *(f32*)((s32)unit + 0x168);
    if (cur != target) {
        diff = target - cur;
        if (diff < float_0_804240b4) {
            diff = -diff;
        }
        speed = *(f32*)((s32)unit + 0x170);
        if (speed <= diff) {
            if (target - *(f32*)((s32)unit + 0x150) <= float_0_804240b4) {
                *(f32*)((s32)unit + 0x15C) = cur - speed;
            } else {
                *(f32*)((s32)unit + 0x15C) = cur + speed;
            }
        } else {
            *(f32*)((s32)unit + 0x15C) = target;
        }
    }

    frame = *(s32*)((s32)unit + 0x16C);
    phase = frame % 90;
    jumpFrames = *(f32*)((s32)unit + 0x184);
    *(f32*)((s32)unit + 0x158) =
        *(f32*)((s32)unit + 0x178) + *(f32*)((s32)unit + 0x14C) +
        ((*(f32*)((s32)unit + 0x164) - *(f32*)((s32)unit + 0x14C)) *
         (jumpFrames - (f32)frame)) / jumpFrames;
    *(f32*)((s32)unit + 0x178) =
        float_10_804240e8 *
        (f32)sinfd(float_180_804240ec +
                   (float_360_804240f0 * (jumpFrames - (f32)phase)) / float_90_804240f4);

    phase = *(s32*)((s32)unit + 0x16C) % 90;
    *(s32*)((s32)unit + 0x1C + 5 * 4) =
        (s32)-(float)(float_25_804240fc *
                      (f32)sinfd((float_360_804240f0 * (jumpFrames - (f32)phase)) /
                                 float_90_804240f4) -
                      float_75_804240f8);
    BtlUnit_GetPartsPtr(unit, 1);
    BtlUnit_SetPos(*(f32*)((s32)unit + 0x154), *(f32*)((s32)unit + 0x158),
                   *(f32*)((s32)unit + 0x15C), unit);

    *(s32*)((s32)evt + 0x7C) = *(s32*)((s32)evt + 0x7C) + 1;
    if (*(s32*)((s32)evt + 0x7C) < 0x15) {
        effect = *(s32*)((s32)evt + 0x80);
        if (*(s32*)((s32)evt + 0x7C) < 0x10) {
            *(f32*)(*(s32*)((s32)effect + 0xC) + 4) = *(f32*)((s32)unit + 0x154);
            height = (f32)BtlUnit_GetHeight(unit);
            *(f32*)(*(s32*)((s32)effect + 0xC) + 8) =
                *(f32*)((s32)unit + 0x114) * float_0p5_80424108 * height +
                *(f32*)((s32)unit + 0x158);
            *(f32*)(*(s32*)((s32)effect + 0xC) + 0xC) = *(f32*)((s32)unit + 0x15C);
        }
    } else {
        f32 xOffset;
        *(s32*)((s32)evt + 0x7C) = 0;
        xOffset = float_50_80424104;
        if (*(s8*)((s32)unit + 0x188) > -1) {
            xOffset = float_neg50_80424100;
        }
        height = (f32)BtlUnit_GetHeight(unit);
        *(s32*)((s32)evt + 0x80) =
            effSweatN64Entry(*(f32*)((s32)unit + 0x154),
                             *(f32*)((s32)unit + 0x114) * float_0p5_80424108 * height +
                                 *(f32*)((s32)unit + 0x158),
                             *(f32*)((s32)unit + 0x15C),
                             float_5_804240e0 * *(f32*)((s32)unit + 0x114), xOffset, 1, 0x10);
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

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void __makeTechMenuFunc(void* commandWork, s32* count) {
    extern void* _battleWorkPointer;
    extern void* BattleGetPartyPtr(void* battleWork);
    extern s32 BattleTransPartyId(s32 id);
    extern s32 partyGetTechLv(s32 partyId);
    extern char* msgSearch(char* msg);
    extern u8 lbl_8037C440[];
    s32 techLv;
    void* party;
    u8* data;

    party = BattleGetPartyPtr(_battleWorkPointer);
    techLv = partyGetTechLv(BattleTransPartyId(*(s32*)((s32)party + 8)));

    data = lbl_8037C440;
    *(s32*)((s32)commandWork + *count * 0x1C + 0x90) = -1;
    *(s32*)((s32)commandWork + *count * 0x1C + 0x94) = 0;
    *(void**)((s32)commandWork + *count * 0x1C + 0x80) = data + 0x624;
    *(s32*)((s32)commandWork + *count * 0x1C + 0x84) = 0;
    *(u16*)((s32)commandWork + *count * 0x1C + 0x8C) =
        *(u16*)(*(s32*)((s32)commandWork + *count * 0x1C + 0x80) + 4);
    *(void**)((s32)commandWork + *count * 0x1C + 0x88) =
        msgSearch(**(char***)((s32)commandWork + *count * 0x1C + 0x80));
    *count = *count + 1;

    if (techLv >= 0) {
        *(s32*)((s32)commandWork + *count * 0x1C + 0x90) = -1;
        *(s32*)((s32)commandWork + *count * 0x1C + 0x94) = 0;
        *(void**)((s32)commandWork + *count * 0x1C + 0x80) = data + 0x6E4;
        *(s32*)((s32)commandWork + *count * 0x1C + 0x84) = 0;
        *(u16*)((s32)commandWork + *count * 0x1C + 0x8C) =
            *(u16*)(*(s32*)((s32)commandWork + *count * 0x1C + 0x80) + 4);
        *(void**)((s32)commandWork + *count * 0x1C + 0x88) =
            msgSearch(**(char***)((s32)commandWork + *count * 0x1C + 0x80));
        *count = *count + 1;
    }
    if (techLv >= 1) {
        *(s32*)((s32)commandWork + *count * 0x1C + 0x90) = -1;
        *(s32*)((s32)commandWork + *count * 0x1C + 0x94) = 0;
        *(void**)((s32)commandWork + *count * 0x1C + 0x80) = data + 0xAA4;
        *(s32*)((s32)commandWork + *count * 0x1C + 0x84) = 0;
        *(u16*)((s32)commandWork + *count * 0x1C + 0x8C) =
            *(u16*)(*(s32*)((s32)commandWork + *count * 0x1C + 0x80) + 4);
        *(void**)((s32)commandWork + *count * 0x1C + 0x88) =
            msgSearch(**(char***)((s32)commandWork + *count * 0x1C + 0x80));
        *count = *count + 1;
    }
    if (techLv >= 2) {
        *(s32*)((s32)commandWork + *count * 0x1C + 0x90) = -1;
        *(s32*)((s32)commandWork + *count * 0x1C + 0x94) = 0;
        *(void**)((s32)commandWork + *count * 0x1C + 0x80) = data + 0xB64;
        *(s32*)((s32)commandWork + *count * 0x1C + 0x84) = 0;
        *(u16*)((s32)commandWork + *count * 0x1C + 0x8C) =
            *(u16*)(*(s32*)((s32)commandWork + *count * 0x1C + 0x80) + 4);
        *(void**)((s32)commandWork + *count * 0x1C + 0x88) =
            msgSearch(**(char***)((s32)commandWork + *count * 0x1C + 0x80));
        *count = *count + 1;
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

u8 btl_yoshi_yoroyoro_jump_calc_param(void* evt) {
    extern void* _battleWorkPointer;
    extern s32 evtGetValue(void* evt, s32 arg);
    extern f32 evtGetFloat(void* evt, s32 arg);
    extern void evtSetFloat(void* evt, s32 arg, f32 value);
    extern void evtSetValue(void* evt, s32 arg, s32 value);
    extern s32 BattleTransID(void* evt, s32 id);
    extern void* BattleGetUnitPtr(void* battleWork, s32 id);
    extern void BtlUnit_SetMoveTargetPos(void* unit, f32 x, f32 y, f32 z);
    extern void BtlUnit_GetPos(void* unit, f32* x, f32* y, f32* z);
    extern void BtlUnit_SetMoveStartPos(void* unit, f32 x, f32 y, f32 z);
    extern void BtlUnit_SetMoveCurrentPos(void* unit, f32 x, f32 y, f32 z);
    extern f32 angleABf(f32 x0, f32 z0, f32 x1, f32 z1);
    extern f32 distABf(f32 x0, f32 z0, f32 x1, f32 z1);
    extern f32 float_0_804240b4;
    extern f32 float_1_804240d8;
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
s32 yoshi_original_color_anim_set(void* evt, s32 releaseOld) {
    extern void* _battleWorkPointer;
    extern char str_PYS_D_5_802f77d8[];
    extern s32 evtGetValue(void* evt, s32 arg);
    extern s32 BattleTransID(void* evt, s32 id);
    extern void* BattleGetUnitPtr(void* battleWork, s32 id);
    extern s32 pouchGetPartyColor(s32 partyId);
    extern void* BattleGetUnitPartsPtr(s32 unitId, s32 partsId);
    extern void* animPoseGetAnimPosePtr(s32 poseId);
    extern void animPoseRelease(s32 poseId);
    extern s32 animEffectAsync(char* name, s32 flags);
    extern s32 animPoseEntry(char* name, s32 flags);

    char* base = str_PYS_D_5_802f77d8;
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


s32 _get_nomikomi_hit_position(int param_1) {
    extern void* _battleWorkPointer;
    extern s32 evtGetValue(void* evt, s32 arg);
    extern f32 evtSetFloat(void* evt, s32 arg, f32 value);
    extern s32 BattleTransID(void* evt, s32 id);
    extern void* BattleGetUnitPtr(void* battleWork, s32 id);
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


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 _check_swallow_attribute(void* evt) {
    extern void* _battleWorkPointer;
    extern s32 evtGetValue(void* evt, s32 arg);
    extern void evtSetValue(void* evt, s32 arg, s32 value);
    extern s32 BattleTransID(void* evt, s32 id);
    extern void* BattleGetUnitPtr(void* battleWork, s32 id);

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
    extern void* _battleWorkPointer;
    extern f32 float_0_804240b4;
    extern s32 evtGetValue(void* evt, s32 arg);
    extern s32 BattleTransID(void* evt, s32 id);
    extern void* BattleGetUnitPtr(void* battleWork, s32 id);

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


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 _get_swallow_param(void* evt) {
    extern void* _battleWorkPointer;
    extern s32 evtGetValue(void* evt, s32 arg);
    extern void evtSetValue(void* evt, s32 arg, s32 value);
    extern s32 BattleTransID(void* evt, s32 id);
    extern void* BattleGetUnitPtr(void* battleWork, s32 id);

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

u32 _wait_yoshig_complete(void* evt) {
    extern void* _battleWorkPointer;
    extern s32 evtGetValue(void* evt, s32 arg);
    extern s32 BattleTransID(void* evt, s32 id);
    extern void* BattleGetUnitPtr(void* battleWork, s32 id);
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

void _yoshi_slide_move_sound(void* unit, void* move) {
    extern void BtlUnit_snd_se(void* unit, s32 soundId, s32 unk0, s32 volume, s32 unk1);
    s32 volume = (s16)((*(s32*)((s32)unit + 0x228) * *(s32*)((s32)unit + 0x22C)) / 100);

    BtlUnit_snd_se(unit, *(s32*)((s32)move + 0xC), -250000000, volume, 0x51EB851F);
}

