#include "battle/battle_item_data.h"

extern void* _battleWorkPointer;
s32 evtGetValue(void* event, s32 arg);
s32 BattleTransID(void* event, s32 id);
void* BattleGetUnitPtr(void* battleWork, s32 unitId);
void BattleStatusWindowSystemOn(void);
void N_statusClearBlink(void);
void statusMarioHPBlink(s32 frames);
void statusPartyHPBlink(s32 frames);
void statusFPBlink(s32 frames);
s32 sysMsec2Frame(s32 msec);
f32 intplGetValue(s32 mode, s32 current, s32 total, f32 start, f32 end);
void* camGetPtr(s32 cameraId);
extern f32 float_720_80422824;
extern f32 float_0_80422804;

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


u8 _throw_item_icon(void* event, s32 isFirstCall) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;

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
    f32* work;
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
        *(void**)((s32)event + 0x78) = work;
        work[6] = endX;
        work[7] = endY;
        work[8] = endZ;
        work[3] = startX;
        work[4] = startY;
        work[5] = startZ;
        work[0] = startX;
        work[1] = startY;
        work[2] = startZ;
        pos.x = startX;
        pos.y = startY;
        pos.z = startZ;
        dist = distABf(work[0], work[2], work[6], work[8]);
        work[9] = (f32)((s32)(dist / 15.0f) + 30);
        work[11] = 0.5f;
        work[12] = work[11] * -work[9] * 0.5f + (work[7] - work[1]) / -work[9];
        work[10] = distABf(work[0], work[2], work[6], work[8]) / -work[9];
        *(s32*)((s32)event + 0x7C) = item;
        *(s32*)((s32)event + 0x80) = 0;
        *(s32*)((s32)event + 0x84) = psndSFXOn_3D(str_SFX_ITEM_COOKING_THR_802f0730, &pos);
    }

    work = *(f32**)((s32)event + 0x78);
    if (*(s32*)((s32)event + 0x80) == 0) {
        work[4] += work[12];
        work[12] -= work[11];
        if (work[12] < 0.0f && work[4] < work[7]) {
            work[4] = work[7];
        }
        btlMovePos(work[10], angleABf(work[0], work[2], work[6], work[8]), &work[3], &work[5]);
        work[9] = (f32)((s32)work[9] - 1);
        if ((s32)work[9] < 1) {
            *(s32*)((s32)event + 0x80) = 0x01000000;
            work[3] = work[6];
            work[4] = work[7];
            work[5] = work[8];
        }
        dispEntry(4, 1, _disp_item_icon, event, 900.0f);
        return 0;
    }

    psndSFXOff(*(s32*)((s32)event + 0x84));
    BattleFree(work);
    return 2;
}

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

