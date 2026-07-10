#include "battle/battle_mario.h"
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
extern void* effFireEntry(f32 x, f32 y, f32 z, f32 scale, s32 type, s32 duration);
extern void effDelete(void* eff);
extern void* effWhirlwindN64Entry(f32 height, s32 flip, s32 unk, s32 duration);
extern void* effHibashiraEntry(f32 x1, f32 y1, f32 z1, f32 x2, f32 y2, f32 z2, f32 scale, s32 unk0, s32 unk1, s32 duration);
extern void* effMajinaiEntry(f32 x, f32 y, f32 z, s32 type);
extern void effSetName(void* eff, const char* name);
extern void* effNameToPtr(const char* name);

extern void* pouchGetPtr(void);

extern void BattleMajinaiDone(void);
extern void BattleMajinaiEndCheck(void);
extern void BattleAudience_Case_JumpNewRecord(void);
extern const f32 vec3_802ef5d8[3];
extern const char str_mjef_coinup_802ef624[];

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


u8 _tatsumaki_effect(void) {
    return 0;
}


s32 _hammer_star_effect(void* evt, s32 first) {
    extern f32 intplGetValue(s32 type, s32 current, s32 total, f32 start, f32 end);
    extern f32 sinfd(f32 angle);
    extern f32 cosfd(f32 angle);
    extern void* effEnergyEntry(s32 type, s32 timer, f32 x, f32 y, f32 z, f32 field10, f32 field20, f32 field18);
    extern f32 float_210_804223bc;
    extern f32 float_420_804223c0;
    extern f32 float_neg10_804223c4;
    extern f32 float_150_804223c8;
    extern f32 float_neg60_804223cc;
    extern f32 float_10_804223d0;
    extern f32 float_0p85_804223d4;
    extern f32 float_26_804223d8;
    extern f32 float_0p1_804223dc;
    extern f32 float_0p08_804223e0;
    extern f32 float_0p05_804223e4;
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
    extern f32 float_360_804223b8;
    extern f32 float_0_804223b4;
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

s32 _jump_star_effect(void* evt, s32 first) {
    extern void* effEnergyEntry(s32 type, s32 timer, f32 x, f32 y, f32 z, f32 field10, f32 field20, f32 field18);
    extern f32 float_neg10_804223c4;
    extern f32 float_10_804223d0;
    extern f32 float_0p1_804223dc;
    extern f32 float_0p05_804223e4;
    extern f32 float_0p8_804223f4;
    extern f32 float_2_804223f8;
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

s32 _fire_wave(void* evt, s32 first) {
    extern f32 float_0p6_804223b0;
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


s32 _wait_jyabara_hit_iron_frame(void* evt) {
    extern f32 float_130_804223f0;
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

